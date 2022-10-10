/*
/////////////////////////////////////////////////////////////////////////
ʹ������
  ���g_ocCmd->endΪ0xEA ����ʹ��CRC16У��,���Ϊ0xEB��ʹ��У��

  uchar g_cache[256];
  int g_len=0;
  TzhDTRSFrame_Cmd g_ocCmd;
  uchar g_isGetCmdOk;

  int tmp;
  int n;
  
  //�����յ������ݷŵ�������
  g_cache[g_len]=UART1_DR;
  g_len++;
  
  //����������ȡ����֡
  tmp=dtrsNetGetFrame(g_cache,g_len,&g_ocCmd,&g_isGetCmdOk);
  //����ָ��
  if(g_isGetCmdOk)
  {
    ����
    //func(g_ocCmd.flag,g_ocCmd.parameter,g_ocCmd.parameter_len);
  }
  //����������
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
 ����:����ָ��  
    ����ָ��ĳ���
*/
int dtrsNetCreateFrame( uint parm_len,
                  const uchar* parm,
				  uchar is_need_crc16,
                  uchar* dst_buf);

/*
����:��ȡhx-kong��Э��
   �ӻ�������ȡָ��,�����һ���ֽڲ���OCЭ���ͷ��ʶ,���ȡʧ��
   ���ؽ�ȡ����,���ָ����Ч�Ļ�,���ص����������ݵĳ���
*/
int dtrsNetGetFrame(uchar* cache,int cache_len,TzhDTRSFrame_Cmd* pcmd,uchar* is_get_cmd_success);

#ifdef __cplusplus
}
#endif
#define _HXKON_DTRS_PROTOCOL_H__
#endif
