#include "session.h"

bool zhSionStartup(TzhNetListen* lisn,unsigned short wBindPort,bool isVariableFlowEncrypt)
{
	//srand(zhPlatGetTime());
	if(!zhSockInit(&lisn->s,ZH_SOCK_TCP)){ZH_NET_DEBUG_PRINTF("Init Socket Fail..!!");return false;}
    if(!zhSockSetNonBlocking(lisn->s,true)){ZH_NET_DEBUG_PRINTF("SetNonBlocking Fail..!!");return false;}
	if(!zhSockSetReuseAddr(lisn->s,true)){ZH_NET_DEBUG_PRINTF("Reuse Addr Fail..!!");return false;}
	if(!zhSockBindAddr(lisn->s,NULL,wBindPort)){ZH_NET_DEBUG_PRINTF("Bind Fail..!!");return false;}
	if(!zhSockListen(lisn->s))return false;

	lisn->isStartupVariableFlowEncrypt=isVariableFlowEncrypt;
	lisn->nEncryptKey=0;
	return true;
}

bool zhSionInit(TzhNetSession *sion,unsigned short wBindPort)
{
	//srand(zhPlatGetTime());
	if(!zhSockInit(&sion->s,ZH_SOCK_TCP)){return false;}
    if(!zhSockSetNonBlocking(sion->s,true)){ZH_NET_DEBUG_PRINTF("SetNonBlocking Fail..!!");return false;}
	
	INIT_CS(&sion->csSendData);
	sion->pInfo=NULL;
	sion->cState=ezhNetStateInitizal;
	sion->isStartupVariableFlowEncrypt=false;
	sion->nEncryptKey=0;
	sion->isAlreadyGetEncryptSeed=false;

	if(wBindPort>0)
	{	
        if(!zhSockSetReuseAddr(sion->s,true)){ZH_NET_DEBUG_PRINTF("Reuse Addr Fail..!!");return false;}
        if(!zhSockBindAddr(sion->s,NULL,wBindPort)){return false;}
    }
	return true;
}

void zhSionSetInfo(TzhNetSession *sion,void* pInfo)
{
	sion->pInfo=pInfo;
}

bool zhSionConnect(TzhNetSession *sion,char*szIp,unsigned short wRemotoPort)
{
	if(false==zhSockConnect(sion->s,szIp,wRemotoPort,0)){
		return false;
	}
	sion->cState=ezhNetStateWaitSeed;
	sion->dwStartTime=zhPlatGetTime();
	
	sion->tagPack.bNetPackRecvBuf=false;
	sion->tagPack.wNetPackPos=0;
	sion->sendDataList=NULL;
	sion->pSendDataList_last=NULL;
	sion->sendDataListCount=0;
	sion->isAlreadyGetEncryptSeed=false;
	return true;
}

void zhSionSetBigSockCache(TzhNetSession *sion,int buf_size)
{
	//�����������������շ����ݵĻ�����������  zhSionConnect   zhSionAccept �ĺ�������ִ��
	zhSockSetSendBufferSize(sion->s,buf_size);//���÷��ͻ�����
	zhSockSetRecvBufferSize(sion->s,buf_size);//���ý��ջ�����
}

bool zhSionAccept(TzhNetListen* lisn,TzhNetSession *sion)
{
    sion->s=zhSockAccept(lisn->s);
	if(sion->s)
	{
		sion->dwStartTime=zhPlatGetTime();		
		sion->pInfo=NULL;

		sion->tagPack.bNetPackRecvBuf=false;
		sion->tagPack.wNetPackPos=0;
		sion->sendDataList=NULL;
		sion->pSendDataList_last=NULL;
		sion->sendDataListCount=0;
		INIT_CS(&sion->csSendData);

		sion->cState=ezhNetStateInitizal;

		//������Ϣ
		sion->isStartupVariableFlowEncrypt=lisn->isStartupVariableFlowEncrypt;
		sion->nEncryptKey=lisn->nEncryptKey;
		_zhSionSendKeyEncrypt(sion);
		sion->isAlreadyGetEncryptSeed=true;
		return true;
	}
	return false;
}

bool zhSionSafeClose(TzhNetSession *sion)
{
    if(ezhNetStateZero!=sion->cState)
	{
        sion->cState=ezhNetStateDead;
        return true;
    }
	return false;
}

bool zhSionSend(TzhNetSession *sion,char* szPack,int nLen)
{
	int nSendLen=0;
	TzhPackFrameHeader pktHeader={0};
	unsigned char* sendBuf=NULL;
	bool ret;
	unsigned short tmp=0;

	if(ezhNetStateDead==sion->cState || ezhNetStateZero==sion->cState)
	{
		return false;
	}

	/*
	if(sion->cState!=ezhNetStateConnected)
	{
		ZH_NET_DEBUG_PRINTF("zhSionSend Error->disconnecting...");
		if(sion->pfError)
		{sion->pfError(sion,sion->pInfo,ezhNetErrorSend);}
		return 0;
	}*/
	
	nSendLen=sizeof(TzhPackFrameHeader)+nLen;
	sendBuf=(unsigned char*)malloc(nSendLen);

	//++++++++++++++++++
	//��ͷ����
	pktHeader.yFrameFlag=ZH_NET_FRAME_FLAG;
	pktHeader.wDataLenL1=nLen&0xFF;
	pktHeader.wDataLenL2=nLen>>8;
	tmp=zhPlatCRC16((unsigned char*)szPack,nLen);
	pktHeader.wCRC16L1=tmp&0xFF;
	pktHeader.wCRC16L2=tmp>>8;

	//-----------------
	memcpy(sendBuf,&pktHeader,sizeof(TzhPackFrameHeader));
	memcpy(&sendBuf[sizeof(TzhPackFrameHeader)],szPack,nLen);

	//zhPlatPrint16(4,&sion->nEncryptKey);
	//Encrypt Key
	zhNetEncrypt(sion->isStartupVariableFlowEncrypt,(char*)sendBuf,sizeof(TzhPackFrameHeader),sion->nEncryptKey);//֡ͷ
	zhNetEncrypt(sion->isStartupVariableFlowEncrypt,(char*)&sendBuf[sizeof(TzhPackFrameHeader)],nLen,sion->nEncryptKey);//����
	//��ӵ������б�
	ret=zhSionInsertSendList(sion,(char*)sendBuf,nSendLen);
	
	free(sendBuf);
	sendBuf=NULL;
	return ret;
}

bool zhSionInsertSendList(TzhNetSession *sion,char* yBuf,int nLen)
{
	bool ret=true;

	//��ӵ������б�
	LOCK_CS(&sion->csSendData);
	if(NULL==sion->sendDataList)
	{
		sion->sendDataList=(TzhNetPacketList*)malloc(sizeof(TzhNetPacketList));
		if(sion->sendDataList)
		{
			memset(sion->sendDataList,0,sizeof(TzhNetPacketList));
			memcpy(sion->sendDataList->packet,yBuf,nLen);
			sion->sendDataList->packet_len=nLen;
			sion->pSendDataList_last=sion->sendDataList;
		}
		else
		{
			ret=false;
		}
	}
	else
	{
		sion->pSendDataList_last->_next=(TzhNetPacketList*)malloc(sizeof(TzhNetPacketList));
		if(sion->pSendDataList_last->_next)
		{
			memset(sion->pSendDataList_last->_next,0,sizeof(TzhNetPacketList));
			memcpy(sion->pSendDataList_last->_next->packet,yBuf,nLen);
			sion->pSendDataList_last->_next->packet_len=nLen;
			sion->pSendDataList_last=sion->pSendDataList_last->_next;
		}
		else
		{
			ret=false;
		}
	}

	sion->sendDataListCount++;
	UNLOCK_CS(&sion->csSendData);
	return ret;
}

bool zhSionSendPacket(TzhNetSession *sion,TzhPacket*pack)
{
	return zhSionSend(sion,(char*)pack->btBuf,pack->wSize);;
}

int zhSionGetSurplusSendCount(TzhNetSession *sion)
{
	return sion->sendDataListCount;
}

bool zhSionCacheData(TzhNetSession *sion,EzhNetError* err)
{	
	char		szTmpBuf[ZH_NET_PACKET_BODY_LENGTH];
	int			nBufSize;
	
	if(!sion->s)
	{
		*err=ezhNetErrorSocket;
		return false;
	}

	nBufSize=zhSockRecv(sion->s,szTmpBuf,ZH_NET_PACKET_BODY_LENGTH);

	if(nBufSize!=SOCKET_ERROR)
	{
		if(nBufSize>0)
		{		
			//�����������ѹ̫��Ļ�ֱ�ӶϿ�SOCKET
			if(sion->tagPack.wNetPackPos+nBufSize>=ZH_NET_PACKET_SIZE)
			{
				ZH_NET_DEBUG_PRINTF("Cache Waring Socket=%d , wNetPackPos=%d , nBufSize=%d",sion->s,sion->tagPack.wNetPackPos,nBufSize);
				*err=ezhNetErrorCacheOverflow;
				return false;
			}
			else
			{				
				memcpy(&sion->tagPack.btCache[sion->tagPack.wNetPackPos],szTmpBuf,nBufSize);
				sion->tagPack.wNetPackPos+=nBufSize;

				//�ͻ��˻�ȡ����Կ��
				if(false==sion->isAlreadyGetEncryptSeed)
				{
					if(sion->tagPack.wNetPackPos>=4)//4���ֽڼ�����Ϣ
					{
							sion->nEncryptKey=szTmpBuf[0]		|
											  szTmpBuf[1]<<8	|
											  szTmpBuf[2]<<16	|
											  szTmpBuf[3]<<24;
							//zhPlatPrint16(4,&sion->nEncryptKey);
							if(0==sion->nEncryptKey)//������Ϣ��0Ϊ���Ǳ�������
							{
								sion->isStartupVariableFlowEncrypt=false;
							}
							else{
								sion->isStartupVariableFlowEncrypt=true;
							}
							ZH_NET_DEBUG_PRINTF("Get Encrypt GetKey=%d !!",sion->nEncryptKey);
							sion->cState=ezhNetStateConnecting;
				
							//ʣ���ȫ�ӵ�֡��������
							sion->tagPack.wNetPackPos-=4;//4���ֽ���֤
							if(sion->tagPack.wNetPackPos>0)
							{
								memmove(&sion->tagPack.btCache[sion->tagPack.wNetPackPos],&szTmpBuf[4],sion->tagPack.wNetPackPos);
							}
							sion->isAlreadyGetEncryptSeed=true;
					}
				}
			}
		}
	}
	else
	{
		if(sion->cState!=ezhNetStateZero && sion->cState!=ezhNetStateWaitSeed)
		{sion->cState=ezhNetStateDead;}
	}

	*err=ezhNetNoError;
	return true;
}

int zhSionReadData(TzhNetSession *sion,unsigned char *frame,EzhNetError* err)
{
		int nRet;
		int nTmpPos1,nTmpPos2;
		
		nRet=0;
		if(sion->tagPack.wNetPackPos>=sizeof(TzhPackFrameHeader))
		{
			TzhPackFrameHeader packetHeader;
			unsigned short wDataLen=0,wCRC16=0;
			memcpy(&packetHeader,sion->tagPack.btCache,sizeof(TzhPackFrameHeader));
			//zhPlatPrint16(4,&sion->nEncryptKey);
			//���ܰ�ͷ
			zhNetDecrypt(sion->isStartupVariableFlowEncrypt,(char*)&packetHeader,sizeof(TzhPackFrameHeader),sion->nEncryptKey);			
			//
			wDataLen=packetHeader.wDataLenL1|packetHeader.wDataLenL2<<8;
			wCRC16=packetHeader.wCRC16L1|packetHeader.wCRC16L2<<8;
			//++++++++++++++++++++++++����������ͷ+++++++++++++++++++++++++++++++++++++++++
			if(false==sion->tagPack.bNetPackRecvBuf)
			{
				//������Ͽ�
				if(packetHeader.yFrameFlag !=ZH_NET_FRAME_FLAG)
				{
					int findPos;
					unsigned char*pHeader;
					
					//Ѱ�Ұ���ʼ���ֽ�
					pHeader=(unsigned char*)&packetHeader;
					findPos=sizeof(packetHeader.yFrameFlag);
					while(findPos<sion->tagPack.wNetPackPos)
					{
						if(ZH_NET_FRAME_FLAG==pHeader[findPos])
						{break;} 
						findPos++;
					}
					sion->tagPack.wNetPackPos-=findPos;
					if(sion->tagPack.wNetPackPos>0)
					{
						memmove(sion->tagPack.btCache,&sion->tagPack.btCache[findPos],sion->tagPack.wNetPackPos);
					}

					ZH_NET_DEBUG_PRINTF("Packet Error->Socket=%d , wDataLen=%d",
							sion->s,wDataLen);
					
					*err=ezhNetErrorPacketInvalid;
					return -1;
				}
				else
				{
					sion->tagPack.bNetPackRecvBuf=true;
				}
			}
			//++++++++++++++++++++++++�������֡ͷ��������+++++++++++++++++++++++++++++++++++++++++
			if(true==sion->tagPack.bNetPackRecvBuf)
			{
				unsigned short wCRCTmp;
				//++++++++++++++++++++++++��������������+++++++++++++++++++++++++++++++++++++++++
    			nTmpPos1=sizeof(TzhPackFrameHeader)+wDataLen;
    			if(sion->tagPack.wNetPackPos >=nTmpPos1)
    			{
					unsigned char* tmpFrame;
					tmpFrame=(unsigned char*)malloc(nTmpPos1);//�½�һ��֡
					memcpy(tmpFrame,&packetHeader,sizeof(TzhPackFrameHeader));
					memcpy(&tmpFrame[sizeof(TzhPackFrameHeader)],&sion->tagPack.btCache[sizeof(TzhPackFrameHeader)],wDataLen);
					//������֡
					zhNetDecrypt(sion->isStartupVariableFlowEncrypt,
								(char*)&tmpFrame[sizeof(TzhPackFrameHeader)],
								wDataLen,
								sion->nEncryptKey);
					
					//���ݰ�����----------------------
					wCRCTmp=zhPlatCRC16(&tmpFrame[sizeof(TzhPackFrameHeader)],
											wDataLen);
					if(wCRCTmp==wCRC16)
					{
							memcpy(frame,&tmpFrame[sizeof(TzhPackFrameHeader)],wDataLen);
							nRet=wDataLen;

							//�ͷ���ʱ֡
							free(tmpFrame);
							tmpFrame=NULL;
					}
					else
					{
						int findPos;
						unsigned char*pPacket;
						
						//Ѱ�Ұ���ʼ���ֽ�
						pPacket=tmpFrame;
						findPos=sizeof(packetHeader.yFrameFlag);
						while(findPos<sion->tagPack.wNetPackPos)
						{
							if(ZH_NET_FRAME_FLAG==pPacket[findPos])
							{break;}
							findPos++;
						}
						sion->tagPack.wNetPackPos-=findPos;
						if(sion->tagPack.wNetPackPos>0)
						{
							memmove(sion->tagPack.btCache,&sion->tagPack.btCache[findPos],sion->tagPack.wNetPackPos);
						}

						ZH_NET_DEBUG_PRINTF("CRC16 error wCRCTmp=%d , Frame->header.wCRC16=%d",wCRCTmp,wCRC16);
						//CRCУ�����
						*err=ezhNetErrorCRC16;

						//�ͷ���ʱ֡
						free(tmpFrame);
						tmpFrame=NULL;
						return -1;
					}
					//----------------------------------

    				nTmpPos2=sion->tagPack.wNetPackPos-nTmpPos1;
					if(nTmpPos2>=0)
					{
						if(nTmpPos2>0)
    					memmove(sion->tagPack.btCache,&sion->tagPack.btCache[nTmpPos1],nTmpPos2);
    					sion->tagPack.wNetPackPos=nTmpPos2;
					}
    				sion->tagPack.bNetPackRecvBuf=false;
    			}
				else
				{
					//����ѭ��if(sion->tagPack.wNetPackPos >=nTmpPos)
					return 0;
				}
			}
			else
			{
				//����ѭ��if(true==sion->tagPack.bNetPackRecvBuf)
				return 0;
			}
		}
		else
		{
			//����ѭ��if(sion->tagPack.wNetPackPos>=sizeof(TzhPackFrameHeader))
			return 0;
		}

		*err=ezhNetNoError;
		return nRet;
}

EzhNetEvent zhSionStateThread(TzhNetSession*sion)
{
	//--------------------------------------------
	//�����б���
	TzhNetPacketList *p1,*p2;
	int ret;

	if(ezhNetStateZero!=sion->cState)
	{
		LOCK_CS(&sion->csSendData);
		p1 = sion->sendDataList;
		while(p1 != NULL)
		{
			p2 = p1->_next;
			ret=p1->packet_len - p1->send_pos;
			if(ret<=0)
			{
				//���ݴ�������
				p1->packet_len=0;
				p1->send_pos=0;
				ret=0;
			}
			else
			{
				ret=zhSockSend(sion->s, &p1->packet[p1->send_pos] , ret);
			}
			if(ret>0 || 0==p1->packet_len)
			{
				p1->send_pos+=ret;
				if(p1->send_pos==p1->packet_len)
				{
					free (p1);
					p1 = p2;
					//�������
					sion->sendDataListCount--;
					sion->sendDataList=p1;
					if(NULL==sion->sendDataList)
					{sion->pSendDataList_last=NULL;}
				}
			}
			else
			{
				
				break;
			}
		}
		UNLOCK_CS(&sion->csSendData);
	}
	//--------------------------------------------
	//״̬����
	switch(sion->cState)
	{
	case ezhNetStateWaitSeed:
		{
			//��ʱ��ʾ
			unsigned long dwTmp;
			unsigned long nTmp;
			dwTmp=zhPlatGetTime();
			nTmp=dwTmp-sion->dwStartTime;
			if(zhSockHasExcept(sion->s,0,0) || nTmp>ZH_NET_CONNECT_TIMEOUT)
			{
				sion->cState=ezhNetStateDead;
				return ezhNetEventConnectTimeout;
			}
		}
		break;
	case ezhNetStateConnecting:
			sion->cState=ezhNetStateConnected;
			return ezhNetEventConnected;
		break;
	case ezhNetStateConnected:

		break;
	case ezhNetStateDead:
		{
			TzhNetPacketList *p1,*p2;

			//�ر�˫��ͨ��
			zhSockShutdown(sion->s,ezhNetShutDownRDWR);
			//��շ���������������
			p1 = sion->sendDataList;
			while(p1 != NULL)
			{
				p2 = p1->_next;
				free (p1);
				p1 = p2;
			}

			//�ر�����
			zhSockClose(sion->s);
			//
			sion->tagPack.wNetPackPos=0;
			sion->tagPack.bNetPackRecvBuf=false;
			sion->sendDataList=NULL;
			sion->pSendDataList_last=NULL;
			DELETE_CS(&sion->csSendData);
			sion->cState=ezhNetStateZero;
			return ezhNetEventDisconnect;
		}
		break;
	case ezhNetStateZero:
			//ZH_NET_DEBUG_PRINTF("ezhNetStateDead  Socket=%d",sion->s);
		break;
	}
	return ezhNetNoEvent;
}

bool zhSionLocalAddr(TzhNetSession *sion,char *addr,unsigned short *port,unsigned int *ip)
{
	return zhSockGetLocalAddr(sion->s,addr,port,(unsigned long*)ip);
}

bool zhSionRemoteAddr(TzhNetSession *sion,char *addr,unsigned short *port,unsigned int *ip)
{
	return zhSockGetRemoteAddr(sion->s,addr,port,(unsigned long*)ip);
}

//��������Կ��
void _zhSionSendKeyEncrypt(TzhNetSession *sion)
{
	char sendbuf[4]={0};
	unsigned int nEncryptKey;

	//�̶�����4���ֽ�	
	if(sion->isStartupVariableFlowEncrypt)
	{
		nEncryptKey=zhNetGetRandEncryptKey();		
	}
	else
	{
		nEncryptKey=0;
	}
	//zhPlatPrint16(4,&nEncryptKey);
	sendbuf[0]=nEncryptKey&0xFF;
	sendbuf[1]=nEncryptKey>>8&0xFF;
	sendbuf[2]=nEncryptKey>>16&0xFF;
	sendbuf[3]=nEncryptKey>>24&0xFF;

	if(zhSionInsertSendList(sion,sendbuf,4))
	{sion->cState=ezhNetStateConnected;}
	//��������Կ��
	sion->nEncryptKey=nEncryptKey;

	ZH_NET_DEBUG_PRINTF("SetKey Sock=%d, nEncryptKey=%d",sion->s,sion->nEncryptKey);
}
