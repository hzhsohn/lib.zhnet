#include "dtrs-protocol.h"

/*
  创建一条新的指令
    返回指令的长度buf
*/
int dtrsNetCreateFrame( uint parm_len,
                  const uchar* parm,
				  uchar is_need_crc16,
                  uchar* dst_buf)
{
    int flag_len=0;
    U16 crc16=0;
    int n=0;
    int i=0;
 
    dst_buf[n]=0xDA;
    n++;
        
    if(parm_len<0xFF)
    {
      dst_buf[n]=parm_len;
      n++;
    }
    else
    {
	  dst_buf[n]=0xFF;
      n++;
      dst_buf[n]=parm_len&0xFF;
      n++;
      dst_buf[n]=(parm_len&0xFF00)>>8;
      n++;
    }
    
    for(i=0;i<parm_len;i++)
    {
		if(parm)
		{
			dst_buf[n]=parm[i];
			n++;
		}
    }

	if(is_need_crc16)
	{
		dst_buf[n]=0xEB;
		n++;
    
		//CRC16检验
		crc16=dtrsCRC16(&dst_buf[0],n);
		dst_buf[n]=crc16&0xff;
		dst_buf[n+1]=crc16>>8;
		n+=2;
	}
	else
	{
		dst_buf[n]=0xEA;
		n++;
	}
    return n;
}



int dtrsNetGetFrame(uchar* cache,int cache_len,TzhDTRSFrame_Cmd* pcmd,uchar* is_get_cmd_success)
{
    int n=0;
    int flag_len=0;
    U16 checksum=0;
	int tmpInvaidN=0;
    
    *is_get_cmd_success=0;
    
    //最少长度为6--
    //6=0xDA+协议标识字符串结尾\0+指令+参数长度+(0xEB+校验码)或(0xEA)--
    if(cache_len<4)
        return 0;
    
    for(n=0;n<cache_len;n++)
    {
        //判断是否dtrs协议头
        if(0xDA==cache[n])
        {
			pcmd->frame_head=&cache[n];
			tmpInvaidN=n;
            break;
        }
    }
    //去掉头--
    if(n>0)
        return n;
    
    pcmd->head=0xDA;
    
    //跳到去指令--
    n=flag_len+1;
    if(0xFF==cache[n])
    {
        pcmd->parameter_len=cache[n+1]|cache[n+2]<<8;
        n+=3;
    }
    else
    {
        pcmd->parameter_len=cache[n];
        n++;
    }
    
    //参数--
    pcmd->parameter=&cache[n];
    n+=pcmd->parameter_len;
    
    //判断数据长度,3=帧尾0xbb+crc16
    if(cache_len<n+1)
        return 0;
       
    //校验尾标识--
    if(0xEB!=cache[n] && 0xEA!=cache[n])
	  {return 1;}
    pcmd->end=cache[n];
    n++;
    
	if(0xEB==pcmd->end)
	{
		U16 t_checksum = 0;
		//判断数据长度是否足够--
		if (cache_len < (n + 2))
		{
			return 0;
		}

		//二次校检码--
		checksum=cache[n]|cache[n+1]<<8;
		t_checksum=dtrsCRC16(&cache[0] ,n);
		if(checksum!=t_checksum)
		{ return 1; }
		pcmd->checksum=checksum;
		//总长度;
		n+=2;
	}
    
    //数据校验成功
    *is_get_cmd_success=1;
	pcmd->frame_len=n-tmpInvaidN;
    return n;
}
