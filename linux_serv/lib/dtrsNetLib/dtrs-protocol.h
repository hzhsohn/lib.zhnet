/*
/////////////////////////////////////////////////////////////////////////
使用例子
  如果g_ocCmd->end为0xEA 即不使用CRC16校验,如果为0xEB即使用校验

  uchar g_cache[256];
  int g_len=0;
  TzhDTRSFrame_Cmd g_ocCmd;
  uchar g_isGetCmdOk;

  int tmp;
  int n;
  
  //将接收到的数据放到缓冲区
  g_cache[g_len]=UART1_DR;
  g_len++;
  
  //处理缓冲区获取数据帧
  tmp=dtrsNetGetFrame(g_cache,g_len,&g_ocCmd,&g_isGetCmdOk);
  //处理指令
  if(g_isGetCmdOk)
  {
    处理
    //func(g_ocCmd.flag,g_ocCmd.parameter,g_ocCmd.parameter_len);
  }
  //调整缓冲区
  if(tmp>0)
  {
   g_len-=tmp;
   for(n=0;n<g_len;n++)
   {
      g_cache[n]=g_cache[tmp+n];
   }
  }
/////////////////////////////////////////////////////////////////////////
*/

#ifndef _HXKON_DTRS_PROTOCOL_H__

#include <stdio.h>
#include "dtrs-crc.h"

#ifdef __cplusplus
extern "C"{
#endif
  
//-------------------------------------------------
typedef struct _TzhDTRSFrame_Cmd
{
  uchar head;
  uint parameter_len;
  uchar* parameter;
  uchar end;
  U16 checksum;
  //
  uchar*frame_head;
  uint frame_len;
}TzhDTRSFrame_Cmd;
 
/*
 功能:创建指令  
    返回指令的长度
*/
int dtrsNetCreateFrame( uint parm_len,
                  const uchar* parm,
				  uchar is_need_crc16,
                  uchar* dst_buf);

/*
功能:获取hx-kong的协议
   从缓冲区截取指令,如果第一个字节不是OC协议的头标识,会截取失败
   返回截取长度,如果指令无效的话,返回的是无用数据的长度
*/
int dtrsNetGetFrame(uchar* cache,int cache_len,TzhDTRSFrame_Cmd* pcmd,uchar* is_get_cmd_success);

#ifdef __cplusplus
}
#endif
#define _HXKON_DTRS_PROTOCOL_H__
#endif
