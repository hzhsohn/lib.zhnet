#include "StdAfx.h"
#include ".\gcsiocp.h"
#include "GCSSockMgr.h"
#include "GCSRTNet.h"
#include <MSTcpIP.h>
#include "GCHPacket.H"
#include "GCHNetFun.h"
#include "GCHDealPacket.h"
#include "GCHNetEncrypt.h"
#include "GC_CRC.h"

extern bool g_bGCThreadRun;

SOCKET	CGCSIoCP::m_sListen = 0;
HANDLE CGCSIoCP::m_hRecvEvent = NULL;
BOOL CGCSIoCP::m_bActive=FALSE;
WORD CGCSIoCP::m_dwPacketNumber;

extern CGCSSockMgr* g_pSockMgr;
extern int g_nWorkThreadCnt;

extern PFUN_ACCEPT_CALLBACK			g_pfnAcceptCallback;
extern PFUN_RECV_DATA_CALLBACK		g_pfnReceiveDataCallback;
extern PFUN_DISCONNECT_CALLBACK		g_pfnDisconnectCallback;


BOOL CGCSIoCP::Init(void)
{
	return TRUE;
}

BOOL CGCSIoCP::Destroy()
{
	m_bActive = FALSE;
	closesocket(m_sListen);
	return TRUE;
}

BOOL CGCSIoCP::InitNet(WORD wPort)
{
	WORD wVersionRequested;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD(1, 1);

	if(WSAStartup( wVersionRequested, &wsaData ) !=0 )
	{
		GCH_ETRACE(_T("CGCSIoCP::Init"), _T("WSAStartup failed."));
		return FALSE;
	}

	m_hRecvEvent =  CreateEvent(NULL,FALSE, FALSE, NULL);

	if(m_hRecvEvent==NULL)
	{
		GCH_ETRACE(_T("CGCSIoCP::Init"), _T("CreateEvent failed."));
		return FALSE;
	}	

	if(wPort != 0)
	{
		if(!InitListenSocket(wPort))
		{
			GCH_ETRACE(_T("CGCSIoCP::Init"), _T("InitListenSocket failed. port=%d"), wPort);
			return FALSE;
		}
	}

	m_bActive = TRUE;

	return TRUE;
}

BOOL CGCSIoCP::InitListenSocket(WORD wPort)
{
	//监听端口
	m_sListen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	if(m_sListen == INVALID_SOCKET)
	{
		GCH_ETRACE(_T("CGCSIoCP::Init"), _T("WSASocket failed."));
		return FALSE;
	}

	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(wPort);

	if(bind(m_sListen, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		GCH_ETRACE(_T("CGCSIoCP::Init"), _T("bind failed. port=%d"), wPort);
		closesocket(m_sListen);
		return FALSE;
	}

	if(listen(m_sListen, 100) == SOCKET_ERROR)
	{
		GCH_ETRACE(_T("CGCSIoCP::Init"), _T("listen failed. m_sListen=%d"), m_sListen);
		closesocket(m_sListen);
		return FALSE;
	}

	return TRUE;
}


/*
*处理数据帧的函数.根据服务器形式对数据不稳定的客户端,
*或者数据对SOCKET接收缓冲区限制有影响的客户端,进行处理
*/
void CGCSIoCP::ProcessData(GCSTH_RecvDataInfo* pRecvData,GCSTS_Sock_Info* pSock, int nLen, char* pData)
{
	if(ZH_NET_FRAME_FLAG==pRecvData->yFrameFlag)
	{
		WORD wCRCTmp=GC_CRC::CRC16((unsigned char*)pData,nLen);
		if(wCRCTmp==pRecvData->wCRC16)
		{
			//处理接收到的包--Begin-->>
			g_pfnReceiveDataCallback(pRecvData->handle, nLen, pData);
			
			//GCH_ETRACE(_T("DealRecvDataThread1"), _T("g_pfnRecvDataCallback"));
			//处理接收到的包--End-->>
		}
		else
		{
			GCH_XTRACE("CRC16 error wCRCTmp=%d , Frame->header.wCRC16=%d",wCRCTmp,pRecvData->wCRC16);
			//CRC校验出错
		}
	}
}

/*
*  发送加密钥匙到客户端
*/
BOOL  CGCSIoCP::SendEncryptKey(CGCSSockMgr* pSockMgr,GCSTS_Sock_Info *pSockInfo)
{
	int nRet;
	DWORD dwSendBufferlen=0;

	EnterCriticalSection(&pSockMgr->m_cs);
	if(pSockInfo->s == 0)
	{
		LeaveCriticalSection(&pSockMgr->m_cs);
		return FALSE;
	}

	GCSTS_Base_Overlapped *pOverlapped =  pSockMgr->NewOverlapped(pSockInfo);
	if(pOverlapped == NULL)
	{
		pSockMgr->CloseSocket(pSockInfo);
		LeaveCriticalSection(&pSockMgr->m_cs);
		return FALSE;
	}
	pOverlapped->dwOperatCode = GCE_Operate_Send;
	
	//种子钥匙
	char nKey[4]={0};
	pOverlapped->wLeft=4;
	memcpy(&pOverlapped->szBuff[0],nKey,4);

	//发送
	pOverlapped->wsabuf.buf=pOverlapped->szBuff;
	pOverlapped->wsabuf.len=pOverlapped->wLeft;
	nRet=WSASend(pSockInfo->s, &pOverlapped->wsabuf, 1, &dwSendBufferlen, 0, &pOverlapped->Overlapped,NULL);
	if(SOCKET_ERROR==nRet)
	{
		DWORD dwErr = WSAGetLastError();
		if (dwErr != ERROR_IO_PENDING)
		{
			GCH_ETRACE(_T("CGCSSockMgr::SendPacket WriteFile failed. dwErr=%d, pSockInfo(handle)=%p, s=%d, pOverlapped=%p, wLeft=%d "),
				dwErr, pSockInfo, pSockInfo->s, pOverlapped, pOverlapped->wLeft);
			pSockMgr->CloseSocket(pSockInfo);
			LeaveCriticalSection(&pSockMgr->m_cs);
			return FALSE;
		}
	}

	//设置发送种子成功
	pSockInfo->isAlreadyGetEncryptSeed=TRUE;

	LeaveCriticalSection(&pSockMgr->m_cs);

	return TRUE;
}

DWORD WINAPI CGCSIoCP::AcceptThread(void* pVoid)
{
	SOCKADDR_IN addrAccept;
	int addrlen = sizeof(SOCKADDR_IN);
	ZeroMemory(&addrAccept, addrlen);
	HANDLE h = NULL;
	SOCKET sockAccept= INVALID_SOCKET;

	struct tcp_keepalive tcpin;
	tcpin.onoff=1;
	tcpin.keepaliveinterval=1000;
	tcpin.keepalivetime=6000;

	while(g_bGCThreadRun)
	{
		//try
		//{
		GCH_KTRACE(_T("CGCSIoCP::AcceptThread 1"));
			if((sockAccept=accept(m_sListen, (SOCKADDR*)&addrAccept, &addrlen))==INVALID_SOCKET)
			{
				int err=WSAGetLastError();
				if(err != ERROR_IO_PENDING)
				{
					GCH_ETRACE(_T("CGCSIoCP::AcceptThread accept failed. err=%d"), GetLastError());
				}
				continue;
			}
		GCH_KTRACE(_T("CGCSIoCP::AcceptThread 2"));
			DWORD dwSize = 0;
			int err=WSAIoctl(sockAccept,SIO_KEEPALIVE_VALS, &tcpin, sizeof(tcpin),	NULL,0, &dwSize,NULL,NULL);
			if(err==SOCKET_ERROR)
			{
				GCH_ETRACE(_T("CGCSIoCP::AcceptThread WSAIoctl failed err=%d"), GetLastError());
				closesocket(sockAccept);
				continue;
			}
		GCH_KTRACE(_T("CGCSIoCP::AcceptThread 3"));
			if((h=g_pSockMgr[sockAccept%g_nWorkThreadCnt].NewSocket(sockAccept, (SOCKADDR*)&addrAccept)) == NULL)
			{
				GCH_ETRACE(_T("CGCSIoCP::AcceptThread NewSocket = NULL"));
			}
			else
			{
				GCH_KTRACE(_T("CGCSIoCP::AcceptThread 4"));
				CGCHDealQueMgr::InsertOtherData(h, GCE_ACCEPT_COONECT);
				SetEvent(m_hRecvEvent);
				GCH_KTRACE(_T("CGCSIoCP::AcceptThread 5"));
			}

		//}
		//catch(...)
		//{	
		//	GCH_ETRACE(_T("CGCSIoCP::AcceptThread"), _T("catch m_sListen=%d, &addrAccept=%p, addrlen=%d, err=%d"), m_sListen, &addrAccept, addrlen, GetLastError());	
		//}		
	}
	return 0;
}

DWORD WINAPI CGCSIoCP::DealRecvDataThread( void *pVoid)
{
	GCSTH_RecvDataInfo recvData;
	GCSTS_Sock_Info* pSock = NULL;
	while(m_bActive)
	{
		ZeroMemory(&recvData, sizeof(recvData));
		if(CGCHDealQueMgr::GetDealData(&recvData))
		{
			pSock = (GCSTS_Sock_Info*)recvData.handle;
			if(recvData.enOpType == GCE_CLOSE_SOCKET)
			{
				g_pfnDisconnectCallback(recvData.handle);
				if(pSock != NULL && pSock->s != 0)
					g_pSockMgr[pSock->s%g_nWorkThreadCnt].DeleteSock(recvData.handle);				
			}
			else if(recvData.enOpType == GCE_ACCEPT_COONECT)
			{				
				TCHAR szIp[GCHMAC_MAX_IP_LEN];
				memset(szIp, 0, sizeof(szIp));
				WORD wPort = 0;
				if(pSock != NULL && pSock->s != 0)
					g_pSockMgr[pSock->s%g_nWorkThreadCnt].GetPeerAddress(recvData.handle, szIp, wPort);
				SendEncryptKey(g_pSockMgr,pSock);
				g_pfnAcceptCallback(recvData.handle, szIp, wPort);
			}
			else
			{
				//没有在内部处理
				ProcessData(&recvData, pSock,recvData.nLen, recvData.buf);
			}
		}		
		else
		{
			WaitForSingleObject(m_hRecvEvent, INFINITE);
			GCH_KTRACE("CGCSIoCP::DealRecvDataThread 1");
		}
	}

	return 0;
}
