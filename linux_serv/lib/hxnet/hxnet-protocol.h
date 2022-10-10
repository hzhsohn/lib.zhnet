/*
  此模块针对hxnet协议
/////////////////////////////////////////////////////////////////////////
使用例子
  如果g_ocCmd->end为0xBA 即不使用CRC16校验,如果为0xBB即使用校验
  hx_uchar g_cache[256];
  int g_len=0;
  TzhNetFrame_Cmd g_ocCmd;
  hx_uchar g_isGetCmdOk;
  int tmp;
  int n;
  
  //将接收到的数据放到缓冲区
  g_cache[g_len]=UART1_DR;
  g_len++;
  
  //处理缓冲区获取数据帧
  tmp=hxNetGetFrame(g_cache,g_len,&g_ocCmd,&g_isGetCmdOk);
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

#ifndef _HX_NET__PROTOCOL_H__

#include <stdio.h>

#ifdef __cplusplus
extern "C"{
#endif
  
#define hx_U16     unsigned short
#define hx_uchar   unsigned char
#define hx_uint   unsigned int

#ifndef NULL
#define NULL    0
#endif

//是否使用辅助函数
#define _IS_MCU_        0
#if _IS_MCU_
 int memcmp(char* src,char* dst,int len);
 void memcpy(void* dest, void* src, int count);
 int strlen(const char *str);
 char *strcat(char *str1, char *str2);
 char* strcpy(char* dest, const char* src);
 int strcmp(const char *str1,const char *str2);
 char *itoa(int num, char *str, int radix);//radix是多少进制一般为10
#endif

//-------------------------------------------------
typedef struct _TzhNetFrame_Cmd
{
  hx_uchar head;
  char*flag;
  hx_uint parameter_len;
  hx_uchar* parameter;
  hx_uchar end;
  hx_U16 checksum;
  //
  hx_uchar*frame_head;
  hx_uint frame_len;
}TzhNetFrame_Cmd;
 
/*
 功能:创建指令  
    返回指令的长度
*/
int hxNetCreateFrame(const char* flag,
                  hx_uint parm_len,
                  const hx_uchar* parm,
				  hx_uchar is_need_crc16,
                  hx_uchar* dst_buf);

/*
功能:获取hx-kong的协议
   从缓冲区截取指令,如果第一个字节不是OC协议的头标识,会截取失败
   返回截取长度,如果指令无效的话,返回的是无用数据的长度
*/
int hxNetGetFrame(hx_uchar* cache,int cache_len,TzhNetFrame_Cmd* pcmd,hx_uchar* is_get_cmd_success);

#ifdef __cplusplus
}
#endif
#define _HX_NET__PROTOCOL_H__
#endif
