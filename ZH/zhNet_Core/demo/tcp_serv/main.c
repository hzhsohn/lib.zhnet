#include "session.h"

TzhNetListen g_listern;
TzhNetSession clientUser;

void zhRecv(TzhNetSession *sion,void* info,char*szBuf,int nLen);
void zhDisconnect(TzhNetSession*sion,void* info);
void zhError(TzhNetSession* sion,void* info,EzhNetError err);

int main(int argc,char *argv[])
{
	unsigned char frame[ZH_NET_PACKET_BODY_LENGTH];
	EzhNetError err;
	int ret;
	TzhNetSession tmpUser;
	
	//初始化网络,加密通讯
	ret=zhSionStartup(&g_listern,2323,true);
	if(ret)
	{
		PRINTF("Startup Server ok..!!");
		PRINTF("BindPort=%d",2323);
		while(true)
		{
				zhPlatSleep(1);
		
				//
				if(zhSionAccept(&g_listern,&tmpUser))
				{
					PRINTF("新TCP连接..!!");
					clientUser=tmpUser;
				}

				if(clientUser.s)
				{
							//网络数据处理-------begin
							if(false==zhSionCacheData(&clientUser,&err))
							{
								zhError(&clientUser,&clientUser.pInfo,err);
							}
							while(1)
							{
								ret=zhSionReadData(&clientUser,frame,sizeof(frame),&err);
								if(0==ret)
								{ break; }
								else if(ret>0)
								{
									//处理frame数据
									zhRecv(&clientUser,
										clientUser.pInfo,
										frame,
										ret);
								}
								if(err!=ezhNetNoError)
								{
									zhError(&clientUser, clientUser.pInfo, err);
								}
							}
							switch(zhSionStateThread(&clientUser))
							{
								//断开连接
								case ezhNetEventDisconnect:
									zhDisconnect(&clientUser,
										clientUser.pInfo);
									break;
							}
							//------------------end
				}
		}
	}
	return 0;
}

void zhRecv(TzhNetSession *sion,void* info,unsigned char*szBuf,int nLen)
{
	TzhPacket pack;
	int idid;
	char sstr[550];

	zhPackReadInit(&pack,(BYTE*)szBuf,nLen);
	zhPackReadInt(&pack,&idid);
	zhPackReadString(&pack,sstr);

	PRINTF("zhRecv  nLen=%d  id=%d str=%s",nLen,idid,sstr);

	zhPackWriteInit(&pack);
	zhPackWriteString(&pack,"服务器发送过去的消息..");	
	zhSionSendPacket(sion,&pack);
}

void zhDisconnect(TzhNetSession*sion,void* info)
{
	PRINTF("Disconnected..!!  Socket=%d ",sion->s);
}

void zhError(TzhNetSession* sion,void* info,EzhNetError err)
{
	PRINTF("zhError..!!  Socket=%d  Error=%d",sion->s,err);
}
