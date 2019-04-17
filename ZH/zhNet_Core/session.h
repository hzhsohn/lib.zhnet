/* 
  Copyright (C) 2008-2, Han.zhihong, Developer. Created 2008. 
  All rights reserved.

  License to copy and use this software is granted provided that it
  is identified as the "Han.zhihong. Message-Digest
  Algorithm" in all material mentioning or referencing this software
  or this function.
  
  Han.zhihong. makes no representations concerning either
  the merchantability of this software or the suitability of this
  software for any particular purpose. It is provided "as is"
  without express or implied warranty of any kind.
	  
  These notices must be retained in any copies of any part of this
  documentation and/or software.


  ==================================================================
  session.h:the connect session manage

  ֡�ṹ:
	unsigned char   yFrameFlag;	//�̶� ZH_NET_FRAME_FLAG
	unsigned short	wDataLen;   //����İ���ĳ���
	unsigned short	wCRC16;     //�����CRCУ��,��������Ϊ0ʱCRCУ��ֵΪ0
	unsigned char	...;		//����wDataLen�����İ���


  ����ṹ�嶨����gcdef.hͷ�ļ�����
*/

#pragma once

#ifndef __ZH_C_2_SESSION_H_


#ifdef __cplusplus
extern "C"{
#endif

#include "encrypt.h"
#include "socket.h"
#include "packet.h"
#include "packet_heap.h"
#include "gcdef.h"

/*
 *error message
 *
 *������Ϣ
*/
typedef enum _EzhNetError
{
	//û�д���
	ezhNetNoError			=0	,
	//�յ����ݰ�����
	ezhNetErrorPacketInvalid	,
	//SOCKET����
	ezhNetErrorSocket			,
	//���������
	ezhNetErrorCacheOverflow	,
	//CRCУ�����
	ezhNetErrorCRC16			,
	//Ŀ�껺��������
	ezhNetErrorCacheNotEnough
}EzhNetError;

/*
 *session's net event
 *
 *SESSION�Ự��������¼�
*/
typedef enum _EzhNetEvent
{
	//û���¼�
	ezhNetNoEvent				,
	//���ӳɹ�
	ezhNetEventConnected		,
	//����ʧ��
	ezhNetEventConnectTimeout	,
	//�Ͽ�����
	ezhNetEventDisconnect		,		
}EzhNetEvent;


/*
 *connect timeout value
 *
 *���ӳ�ʱʱ��
*/
#define ZH_NET_CONNECT_TIMEOUT		15*1000

/*
 *the stick package management buffer
 *
 *ճ����������
*/
#define ZH_NET_PACKET_SIZE			ZH_NET_TCP_CACHE_LENGTH


/*
 *enum session status
 *
 *ö�ٻỰ״̬
*/
typedef enum _EzhNetState {
	ezhNetStateZero			=0,
	ezhNetStateInitizal		=1,
	ezhNetStateWaitSeed		=2,
	ezhNetStateConnecting	=3,
	ezhNetStateConnected	=4,
	ezhNetStateDead			=5
}EzhNetState;

/*
 *enum packet/cache/buffer size
 *
 *ö�ٻ�������С
*/
typedef enum _EzhPackCacheType {
	ezhPackCacheDefault=1024*8,
	ezhPackCache10K=1024*10,
	ezhPackCache30K=1024*30,
	ezhPackCache50K=1024*50,
	ezhPackCache64K=1024*64
}EzhPackCacheType;

/*
 *session packet logic manage 
 *
 *�Ự�����ݰ�����
*/
typedef struct _TzhNetBPack
{
	bool					bNetPackRecvBuf;
	unsigned short			wNetPackPos;
	unsigned char			*btCache;
}TzhNetBPack;

/*
 *listern subject
 *
 *�����Ľṹ
*/
typedef struct _TzhNetListen
{
	SOCKET		s;
	/*
	 *encrypt key and type,using for the transmission logic
	 *
	 *����Կ�׺ͼ�������
	*/
	bool		isStartupVariableFlowEncrypt;
}TzhNetListen;

/*
 *session subject
 *
 *�����Ự�ṹ
*/
typedef struct _TzhNetSession
{
	SOCKET		s;

	/*session status*/
	EzhNetState	cState;
	
	/*
	 *encrypt key and type,using for the transmission logic
	 *
	 *����Կ�׺ͼ�������
	*/
	unsigned char		nEncryptKey[4];
	bool		isStartupVariableFlowEncrypt;
	
	/*
	 *check encrypt
	 *
	 *��ǿͻ��Ƿ��Ѿ���ȡ��������
	*/
	bool isAlreadyGetEncryptSeed;

	/*
	 *begin connect time
	 *
	 *��ʼ���ӵ�ʱ��
	*/
	unsigned long		dwStartTime;

	/*
	 *stick packet processing
	 *
	 *���ճ��
	*/
	TzhNetBPack			tagPack;

	/*
	 *infomation point
	 *
	 *���Ͻṹָ��
	*/
	void			*pInfo;

}TzhNetSession;

/*
 *server port for initizal bind and listen function
 *
 *�����������õĳ�ʼ������
 *GCHEM_Encrypt_Type type ����ʹ�õļ���ģʽ
 *encrypt_default_key ��ʼ�����ܵ�����
*/
bool zhSionStartup(TzhNetListen* lisn,unsigned short wBindPort,bool isStartupVariableFlowEncrypt);

/*
 *client port for initizal function
 *
 *�ͻ����õĳ�ʼ������
*/
bool zhSionInit(TzhNetSession *sion,unsigned short wBindPort);

/*
 *set session info data
 *
 *�豸session��Ӧ�����ݽṹ��
*/
void zhSionSetInfo(TzhNetSession *sion,void* pInfo);


/*
 *client connect function,using by not blacking
 *
 *�ͻ������Ӻ���,�Ƿ�����ʹ��
 *
 *����ֵ
 *   true Ϊ���ӳ�ʼ���ɹ�
 *   false ��ʼ����ʧ��
*/
bool zhSionConnect(TzhNetSession *sion,char*szIp,unsigned short wRemotoPort);

/*
 *server accept session event,cycle using 
 *
 *�������ӽ���,ѭ��ʹ��
*/
bool zhSionAccept(TzhNetListen* lisn,TzhNetSession *sion);

/*
 *close session link
 *
 *�ر�����
*/
bool zhSionSafeClose(TzhNetSession *sion);

/*
 *setting socket transmission buffer size
 *
 *����SOCKET�Ĵ��仺������С
*/
void zhSionSetBigSockCache(TzhNetSession *sion,int buf_size);

/*
 *send data function
 *
 *���ͺ���,�����ݰ��������������
 *
 *����ֵ
 *  true ���뵽��������ɹ�
 *  false ���뵽��������ʧ��,һ�����ڴ治������
*/
int zhSionSend(TzhNetSession *sion,char*szPack,int nLen);
int zhSionSendPacket(TzhNetSession *sion,TzhPacket*pack);

/*
 *network system process,must be using in the program,cycle using
 *
 *�������ݵ����������ռ���,ѭ������
 *
 *����ֵ
 *   false ����ʧ��
 *    true ��ȷ 
*/
bool zhSionCacheData(TzhNetSession *sion,EzhNetError* err);

/*
 *
 *�������ݻ������Ĺ���,���������zhSionCacheData,�⺯����Զ���������ݷ���
 *�����ڷ��ص���zhSionReadDataͬһ��ʱ,�Ѿ���������,У��,�ų����ϸ������
 *
 *����ֵ
 *    -1= �д���,
 *     0= ���������ݲ���һ֡
 *     0> ����֡��С
 *     frame ����֡����
 *     err �������Ϣ
 *
 *����:
 *     int ret;
 *     while(1)
 *     {
 *         ret=zhSionReadData(...);
 *		   if(0==ret)
 *		   { break; }
 *		   else if(-1==ret)
 *		   {
 *             switch(err)
 *             {...}
 *         }
 *		   else
 *		   { ����frame����... }
 *     }
*/
int zhSionReadData(TzhNetSession *sion,unsigned char *frame,int frame_len,EzhNetError* err);

/*
 *����������¼�
 * 
 *����
 *    switch(zhSionStateThread(...))
 *    {...}
*/
EzhNetEvent zhSionStateThread(TzhNetSession *sion);

/*
 *get local ip and port for session
 *
 *�ӻỰ�л�ȡ����IP�Ͷ˿���Ϣ
*/
bool zhSionLocalAddr (TzhNetSession *sion,char *addr,unsigned short *port,unsigned int *ip);

/*
 *get remote socket ip and port for session
 *
 *�ӻỰ�л�ȡ�Է�IP�Ͷ˿���Ϣ
*/
bool zhSionRemoteAddr(TzhNetSession *sion,char *addr,unsigned short *port,unsigned int *ip);


/*
 *logical function of the session layer, using private 
 *
 *�Ự���߼�����,��˽ʹ��
*/
void _zhSionSendKeyEncrypt(TzhNetSession *sion);

#ifdef __cplusplus
}
#endif

#define __ZH_C_2_SESSION_H_
#endif
