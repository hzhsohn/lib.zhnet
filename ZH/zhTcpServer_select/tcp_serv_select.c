#include "tcp_serv_select.h"
#include "session.h"


#define TCP_PRINT_LOG		PRINTF

TzhNetListen g_listern;
TYPE_CS g_userCs;
TzhNetList g_userList;//Ψһ�û�����
TYPE_CS g_threadCs;
TzhNetList g_threadList;//�̴߳����������

time_t g_dwNewTime;
time_t g_dwOldTime;

//�ص�
ZH_ON_ACCEPT		*g_pfAccept=NULL;
ZH_ON_DISCONNECT	*g_pfDisconnect=NULL;
ZH_ON_RECEIVE		*g_pfReceive=NULL;
ZH_ON_ERROR			*g_pfError=NULL;
ZH_ON_REALTIME		*g_pfRealTime=NULL;

/////////////////////////////////////////////////////////////////////////
//
//add client session to the session list
//
//���ͻ��˵ĻỰ��ӵ�SOCKET������
//
TagUserNode* ConnectAdd(TzhNetSession tmpSion);

//
//collection of multiple threads processing linked list,
//every 64 sockets in the chain will create a new thread processing
//
//����̴߳���������,��������ÿ�ﵽ64��socket�ͻ��½�һ���߳̽��д���
//
void ProcThread(TagThreadNode *pThreadNode);

/////////////////////////////////////////////////////////////////////////////////////////
TagUserNode* ConnectAdd(TzhNetSession tmpSion)
{
	TagThreadNode *pThreadNode;
	TagUserNode*node;

	LOCK_CS(&g_userCs);
	LOCK_CS(&g_threadCs);
	node=UserAdd(&g_userList,tmpSion);
	if(!node)
	{
		goto _end;
	}
	//��ʼ���û�����
	initizalUser(&node->userinfo);
	//��ֵ�ṹ
	zhSionSetInfo(&node->sion,(void*)&node->userinfo);
	for(pThreadNode = (TagThreadNode *)zhNetListFirst(&g_threadList); 
		pThreadNode != NULL;
		pThreadNode = (TagThreadNode *)zhNetListNext((TzhNetNode *)pThreadNode))
	{
		if(true==pThreadNode->bThread)
		{
			if(zhNetListCount(&pThreadNode->pUserListNodeList)<TCP_SPLIT_SIZE)
			{
				UserListAdd(&pThreadNode->pUserListNodeList,node);
				goto _end;
			}
		}
	}

	pThreadNode=ThreadAdd(&g_threadList);
	UserListAdd(&pThreadNode->pUserListNodeList,node);
	CREATE_THREAD(ProcThread,pThreadNode);		

_end:
	UNLOCK_CS(&g_threadCs);
	UNLOCK_CS(&g_userCs);
	TCP_PRINT_LOG("ClientCount=%d",zhNetListCount(&g_userList));
	return node;
}

void ProcThread(TagThreadNode *pThreadNode)
{
	time_t dwTmp;
	int nDataProc;
	TagUserListNode *pUser;
	
	unsigned char frame[ZH_NET_PACKET_BODY_LENGTH];
	EzhNetError err;
	int ret;

	if(pThreadNode)
	{
		TCP_PRINT_LOG("Create New Proc Thread... Current Count=%d",zhNetListCount(&g_threadList));
		while(pThreadNode->bThread)
		{
			nDataProc=0;

			LOCK_CS(&g_threadCs);
			for(pUser = (TagUserListNode *)zhNetListFirst(&pThreadNode->pUserListNodeList);pUser != NULL;)
			{
				if(pUser->user_node->sion.cState==ezhNetStateZero)
				{
					//ִ���������ӵĲ�����ʱ��ǵ�Ҫ���� g_userCs �ٽ���,��Ȼ���л�����ĳ��
					//���ӵ��ߵ�ʱ����ɳ���p��,���߱���������
					LOCK_CS(&g_userCs);
					UserDelete(&g_userList,pUser->user_node->sion);
					pUser=UserListDelete(&pThreadNode->pUserListNodeList,pUser->user_node);
					TCP_PRINT_LOG("ClientCount=%d",zhNetListCount(&g_userList));
					UNLOCK_CS(&g_userCs);
				}
				else
				{
					//�������ݴ���-------begin
					if(false==zhSionCacheData(&pUser->user_node->sion,&err))
					{
						g_pfError(&pUser->user_node->sion,&pUser->user_node->sion.pInfo,err);
					}
					while(1)
					{
						ret=zhSionReadData(&pUser->user_node->sion,frame,sizeof(frame),&err);
						if(0==ret)
						{ break; }
						else if(ret>0)
						{
							nDataProc++;
							//����frame����
							g_pfReceive(&pUser->user_node->sion,
								pUser->user_node->sion.pInfo,
								frame,
								ret);
						}
						if(err!=ezhNetNoError)
						{
							g_pfError(&pUser->user_node->sion,
								pUser->user_node->sion.pInfo,
								err);
						}
					}
					switch(zhSionStateThread(&pUser->user_node->sion))
					{
							//���ӳɹ�
						case ezhNetEventConnected:
							break;
							//����ʧ��
						case ezhNetEventConnectTimeout:
							break;
							//�Ͽ�����
						case ezhNetEventDisconnect:
							g_pfDisconnect(&pUser->user_node->sion,
								pUser->user_node->sion.pInfo);
							break;
							//û���¼�
						case ezhNetNoEvent:
							break;
					}
					//------------------end
					g_pfRealTime(&pUser->user_node->sion,pUser->user_node->sion.pInfo);//�������ʱ���
					pUser = (TagUserListNode *)zhNetListNext((TzhNetNode *)pUser);
				}
			}
			
			if(zhNetListCount(&pThreadNode->pUserListNodeList)<=0)
			{
				pThreadNode->bThread=false;
			}
			UNLOCK_CS(&g_threadCs);

			//�������300�������ϲ����߼�ִ��һ��,�������̫Ƶ����ȷ�������߳���������
			//nDataProc������������ݼ�������
			dwTmp=zhPlatGetTime();
			if(0==nDataProc || dwTmp-pThreadNode->dwSleepTime>300)
			{
				pThreadNode->dwSleepTime=dwTmp;
				zhPlatSleep(1);
			}
		}
		LOCK_CS(&g_threadCs);
		ThreadDelete(&g_threadList,pThreadNode);
		pThreadNode=NULL;
		TCP_PRINT_LOG("Release Proc Thread... Current Count=%d",zhNetListCount(&g_threadList));
		UNLOCK_CS(&g_threadCs);
	}
}

void zhInitTcpProc(unsigned short		wBindPort,
				    bool				isVariableFlowEncrypt,
				    ZH_ON_ACCEPT		*pfAccept,
					ZH_ON_DISCONNECT	*pfDisconnect,
					ZH_ON_RECEIVE		*pfReceive,
					ZH_ON_ERROR			*pfError,
					ZH_ON_REALTIME		*pfRealTime)
{
	TzhNetSession tmpUser;
	bool ret;

	g_pfAccept=pfAccept;
	g_pfDisconnect=pfDisconnect,
	g_pfReceive=pfReceive,
	g_pfError=pfError,
	g_pfRealTime=pfRealTime;

	INIT_CS(&g_threadCs);
	INIT_CS(&g_userCs);

	//��ʼ������
	zhNetListInit(&g_userList);
	g_dwOldTime=zhPlatGetTime();

	ret=zhSionStartup(&g_listern,wBindPort,isVariableFlowEncrypt);
	if(ret)
	{
		TCP_PRINT_LOG("Startup Server ok..!!");
		TCP_PRINT_LOG("BindPort=%d",wBindPort);
		while(true)
		{
			//���㴦��ʱ��,��С��80�������������������������Ҫ1�������ʱ
			g_dwNewTime=zhPlatGetTime();
			if(g_dwNewTime-g_dwOldTime>100){zhPlatSleep(1);}
		
			if(zhSionAccept(&g_listern,&tmpUser))
			{
				//�������������������������
				if(zhNetListCount(&g_userList)<TCP_MAX_CONNECTED)
				{
					TagUserNode*unode;
					//�Ƿ�ȡ�ô�SOCKET(64K)������,��ȡ�ò�Ȼ��ʱ���ʹ���8K���ϵİ��ᶪʧ
					//�����С��Ƕ��ʽ�豸3K���㹻��,�������ʹ��
					zhSionSetBigSockCache(&tmpUser,ezhPackCache64K);
					unode=ConnectAdd(tmpUser);
					if(unode)
					{
						TCP_PRINT_LOG("Session Connect, Socket=%d , dwStartTime=%d,user_count=%d",tmpUser.s,tmpUser.dwStartTime,zhNetListCount(&g_userList));
						g_dwOldTime=zhPlatGetTime();
						g_pfAccept(&unode->sion,unode->sion.pInfo);
					}
				}
				else
				{
					TCP_PRINT_LOG("Session connected too much...");
					zhSionSafeClose(&tmpUser);
				}
			}
		}
	}
	else
	{TCP_PRINT_LOG("Startup Server Fail..!!");}
	DELETE_CS(&g_threadCs);
	DELETE_CS(&g_userCs);
}

void zhSendAllUser(TzhPacket*pack)
{
	TagUserNode *pUser;
	for(pUser=(TagUserNode *)zhNetListFirst(&g_userList);pUser != NULL;pUser = (TagUserNode *)zhNetListNext((TzhNetNode *)pUser))
	{
		zhSionSend(&pUser->sion,(char*)pack->btBuf,pack->wSize);
	}
}
