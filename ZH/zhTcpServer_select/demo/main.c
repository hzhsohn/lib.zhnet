#include "../tcp_serv_select.h"

//
//client callback event,include recv,dissconnect
//
//�Ự�ص��¼�
//
void zhAccept(TzhNetSession* sion,void* info);
void zhRecv(TzhNetSession* sion,void* info,char* szBuf,int nLen);
void zhDisconnect(TzhNetSession* sion,void* info);
void zhError(TzhNetSession* sion,void* info,EzhNetError err);
void zhRealTime(TzhNetSession *sion,void* info);
void zhRecvPack(TzhNetSession* sion,TagUserInfo* info,unsigned short wCmd,TzhPacket* pack);

////////////////////////////////////////////////////////////////////////
//�����
void NetKeepTimeout(TzhNetSession*sion)
{
	TagUserInfo *info;
	time_t dwTmp;
	if(ezhNetStateConnected==sion->cState)
	{
		info=(TagUserInfo*)sion->pInfo;
		dwTmp=zhPlatGetTime();

		//32��һ�β������ʱ���
		if(dwTmp - info->dwKeepTime>32000)
		{
			zhSionSafeClose(sion);
			PRINTF("close sock=%d",sion->s);
			PRINTF("no packet receive...");
		}
	}
}

int main(int argc,char *argv[])
{
	zhInitTcpProc(2323,true,zhAccept,zhDisconnect,zhRecv,zhError,zhRealTime);
	getchar();
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////
void zhAccept(TzhNetSession*sion,TagUserInfo* info)
{
	strcpy((char*)info->szAccount,"һ��������");
	PRINTF("zhAccept..!!  Socket=%d ",sion->s);
}

void zhRecv(TzhNetSession *sion,void* info,unsigned char*szBuf,int nLen)
{
	TzhPacket pack;

	PRINTF("zhRecv  nLen=%d",nLen);

	zhPackWriteInit(&pack);
	zhPackWriteString(&pack,"���������͹�ȥ����Ϣ..");	
	zhSionSendPacket(sion,&pack);

	//ˢ�±����ʱ��
	((TagUserInfo*)info)->dwKeepTime=zhPlatGetTime();
}

void zhDisconnect(TzhNetSession*sion,void* info)
{
	PRINTF("Disconnected..!!  Socket=%d ",sion->s);
}

void zhError(TzhNetSession* sion,void* info,EzhNetError err)
{
	switch (err)
	{
		//û�д���
		case ezhNetNoError:
		break;
		//�յ����ݰ�����
		case ezhNetErrorPacketInvalid:
		break;
		//���������
		case ezhNetErrorCacheOverflow:
		break;
		//CRCУ�����
		case ezhNetErrorCRC16:
		break;
	}

	printf("zhtcp err=%d\n",err);

	//�������������Ͽ�����
	//zhSionSafeClose(sion);
}

void zhRealTime(TzhNetSession *sion,void* info)
{
	//printf("zhRealTime\n");
	NetKeepTimeout(sion);
}

void zhRecvPack(TzhNetSession *sion,TagUserInfo*info,unsigned short wCmd,TzhPacket *pack)
{
	int i;
	char str[1024];

	zhPackReadInt(pack, &i);
	zhPackReadString(pack, str);
	
	zhPackWriteInit(pack);
	zhPackWriteString(pack,"���������͹�ȥ����Ϣ..");	
	zhSendAllUser(pack);

	PRINTF("wCmd=%d,i=%d,str=%s",wCmd,i,str);
}
