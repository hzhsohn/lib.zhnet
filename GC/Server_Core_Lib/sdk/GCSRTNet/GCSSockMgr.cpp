#include "StdAfx.h"

#include ".\GCSsockmgr.h"
#include "GCHPacket.h"
#include "GCHNetFun.h"
#include "GCHNetEncrypt.h"
#include "GC_CRC.h"

extern int g_nWorkThreadCnt;
extern bool g_bGCThreadRun;

CGCSSockMgr::CGCSSockMgr()
{}
CGCSSockMgr::~CGCSSockMgr()
{}

BOOL CGCSSockMgr::Init(HANDLE hEvent)
{
	m_memSocket.Init(GCHMAC_FREE_SOCK_COUNT/g_nWorkThreadCnt, GCHMAC_MAX_SOCK_COUNT/g_nWorkThreadCnt);
	m_memOverlapped.Init(GCHMAC_FREE_OVERLAPPED_COUNT/g_nWorkThreadCnt, GCHMAC_MAX_OVERLAPPED_COUNT/g_nWorkThreadCnt);	

	m_hEvent = hEvent;

	//创建完成端口
	m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if(m_hCompletionPort==NULL)
	{
		GCH_ETRACE(_T("CGCSSockMgr::Init  CreateThread CreateIoCompletionPort failed."));
		return FALSE;
	}

	//创建工作线程
	DWORD dwThreadId = 0;
	HANDLE hThread = NULL;
	hThread = CreateThread(NULL, 0, WorkDealThread, this, 0, &dwThreadId);
	if(hThread==NULL)
	{		
		GCH_ETRACE(_T("CGCSSockMgr::Init CreateThread WorkDealThread failed."));
		return FALSE;
	}

	InitializeCriticalSection(&m_cs);
	return TRUE;
}
void CGCSSockMgr::DisconnectAll(void)
{
	GCSTS_Sock_Info* pinfo;
	HANDLE h;
	for(GCH_Map_PPoint_Ite pos =  m_mapSocket.begin();pos !=  m_mapSocket.end();)
	{		
		pinfo=(GCSTS_Sock_Info*)(pos->first);
		ResetEvent(pinfo->hKey);
		h=pinfo;
		pos++;
		DeleteSock(h);
		GCH_KTRACE("CGCSSockMgr::DisconnectAll   handle=%p",h);
	}
}
BOOL CGCSSockMgr::Destroy(void)
{
	DeleteCriticalSection(&m_cs);
	return TRUE;
}
void CGCSSockMgr::DelOverlapped(HANDLE h, GCSTS_Base_Overlapped * pOverLapped)
{
	//GCH_KTRACE("CGCSSockMgr::DelOverlapped h=%p, pOverLapped=%p", h, pOverLapped);
	if(h == NULL)
		return;

	EnterCriticalSection(&m_cs);
	GCSTS_Sock_Info* pSockInfo = (GCSTS_Sock_Info*)h;
	if(IsValidSock(pSockInfo))
	{
		GCH_Lst_Point_Ite pos;
		GCSTS_Base_Overlapped*   pOverlapped2 = NULL;
		for(pos=pSockInfo->lstSendOverlapped.begin(); pos!=pSockInfo->lstSendOverlapped.end(); pos++)
		{
			pOverlapped2 = (GCSTS_Base_Overlapped*)*pos;
			if(pOverlapped2 == pOverLapped)
			{
				m_memOverlapped.MyDel(pOverLapped);
				//GCH_KTRACE("CGCSSockMgr::DelOverlapped h=%p, pOverLapped=%p", h, pOverLapped);
				pSockInfo->lstSendOverlapped.erase(pos);
				break;
			}		
		}	
	}
	
	LeaveCriticalSection(&m_cs);
}
GCSTS_Base_Overlapped * CGCSSockMgr::NewOverlapped(GCSTS_Sock_Info * pSockInfo)
{
	GCSTS_Base_Overlapped *pOverlapped =  m_memOverlapped.MyNew();
	//GCH_KTRACE("CGCSSockMgr::NewOverlapped pSockInfo=%p, pOverLapped=%p", pSockInfo, pOverlapped);
	if(pSockInfo != NULL && pOverlapped != NULL)
		pSockInfo->lstSendOverlapped.push_back((INT_PTR)pOverlapped);

	if(pOverlapped != NULL)
		memset(pOverlapped, 0, sizeof(GCSTS_Base_Overlapped));
	else
		GCH_KTRACE(_T("CGCSSockMgr::NewOverlapped pOverLapped=NULL, m_memOverlapped.GetUseCount=%d"), m_memOverlapped.GetUseCount());
	return pOverlapped;
}

GCSTS_Sock_Info * CGCSSockMgr::NewSocket(SOCKET s, SOCKADDR* pAddr)
{
	//保存sock信息，用于HANDLE
	GCSTS_Sock_Info * pSockInfo = NULL;
	HANDLE hCompletionPort;
	if(false==g_bGCThreadRun)
	{
		closesocket(s);
		return NULL; 
	}
	GCH_KTRACE(_T("CGCSSockMgr::NewSocket s=%d"), s);
	pSockInfo = m_memSocket.MyNew();
	if(pSockInfo == NULL)
	{
		GCH_ETRACE(_T("CGCSSockMgr::NewSocket pSocketInfo == NULL. m_memSocket.GetUseCount()=%d"), m_memSocket.GetUseCount());
		return NULL;
	}
	GCH_KTRACE("CGCSSockMgr::NewSocket 1");
	pSockInfo->s = s;
	pSockInfo->bActive = TRUE;
	GCH_GetIpPortFromAddr(*(SOCKADDR_IN*)pAddr, pSockInfo->szIp, pSockInfo->wPort);

	GCH_KTRACE("CGCSSockMgr::NewSocket 2");
	hCompletionPort=CreateIoCompletionPort((HANDLE) s, m_hCompletionPort,(INT_PTR)(pSockInfo),0);
	if(hCompletionPort==NULL)
	{
		GCH_ETRACE(_T("CGCSIoCP::AcceptThread CreateIoCompletionPort failed err=%d"), GetLastError());
		m_memSocket.MyDel(pSockInfo);
		closesocket(s);
		return NULL;
	}

	GCH_KTRACE("CGCSSockMgr::NewSocket 3");
	//读取头信息
	pSockInfo->pRecvOverlapped = NewOverlapped(NULL);
	if(pSockInfo->pRecvOverlapped == NULL)
	{
		GCH_ETRACE(_T("CGCSIoCP::AcceptThread pSockInfo->pRecvOverlapped == NULL"));
		m_memSocket.MyDel(pSockInfo);
		closesocket(s);
		return NULL;
	}
	GCH_KTRACE("CGCSSockMgr::NewSocket 4");
	ZeroMemory(pSockInfo->pRecvOverlapped, sizeof(GCSTS_Base_Overlapped));

	if(!AddSock(pSockInfo))
	{
		DeleteSock(pSockInfo);
		return NULL;
	}

	GCH_KTRACE("CGCSSockMgr::NewSocket 5");
	if(!RecvDataPart(pSockInfo, 0, 0, pSockInfo->pRecvOverlapped, GCE_Operate_Recv_Head, FALSE))
	{
		DeleteSock(pSockInfo);
		GCH_ETRACE(_T("CGCSSockMgr::NewSocket RecvDataPart failed. s=%d"), s);
		return NULL;
	}

	GCH_KTRACE("CGCSSockMgr::NewSocket 6");
	return pSockInfo;
}

BOOL CGCSSockMgr::RecvDataPart(HANDLE handle, DWORD dwBytesTransferred, DWORD dwPacketBodyLen, GCSTS_Base_Overlapped *pOverlapped, int nOpCode, BOOL bClose)
{	
	int nRet;
	DWORD dwFlags=0;
	GCSTS_Sock_Info* pSockInfo = (GCSTS_Sock_Info*)handle;
	EnterCriticalSection(&m_cs);

	if(!CGCSSockMgr::IsValidSock(pSockInfo))
	{
		LeaveCriticalSection(&m_cs);
		return FALSE;
	}

	pOverlapped->dwOperatCode = nOpCode;

	if(nOpCode == GCE_Operate_Recv_Head && dwBytesTransferred==0)
	{
		//开始接收包
		pOverlapped->wLeft = sizeof(GCSTH_Packet_Frame_Header);
		pOverlapped->wIndex = 0;
	}
	else if(nOpCode == GCE_Operate_Recv_Body && dwBytesTransferred==0)
	{
		//开始接收包体
		pOverlapped->wLeft = (WORD)dwPacketBodyLen;
		pOverlapped->wIndex = sizeof(GCSTH_Packet_Frame_Header);
	}
	else
	{
		//接收包剩余的部分，包头或包体
		pOverlapped->wLeft -= (WORD)dwBytesTransferred;
		pOverlapped->wIndex += (WORD)dwBytesTransferred;
	}

	DWORD dwReceiveBytes = 0;

	if(PACKET_ONE_LENGTH < pOverlapped->wLeft || pOverlapped->wLeft == 0)
	{
		GCH_ETRACE(_T("CGCSSockMgr::RecvDataPart PACKET_MAX_LENGTH=%d < pOverlapped->wLeft=%d"), PACKET_MAX_LENGTH, pOverlapped->wLeft);
		if(bClose)
			CloseSocket(pSockInfo);
		LeaveCriticalSection(&m_cs);
		return FALSE;
	}	

	pOverlapped->wsabuf.buf=&pOverlapped->szBuff[pOverlapped->wIndex];
	pOverlapped->wsabuf.len=pOverlapped->wLeft;
	nRet=WSARecv(pSockInfo->s,&pOverlapped->wsabuf,1,&dwReceiveBytes,&dwFlags,&pOverlapped->Overlapped,NULL);
	if(SOCKET_ERROR==nRet)
	{
		DWORD dwErr = WSAGetLastError();
		if (dwErr != ERROR_IO_PENDING)
		{
			GCH_ETRACE(_T("CGCSSockMgr::RecvDataPart ReadFile failed. dwErr=%d, pSockInfo=%p, s=%d, pOverlapped=%p, wLeft=%d"),
				dwErr, pSockInfo, pSockInfo->s, pOverlapped, pOverlapped->wLeft);			
			if(bClose)
				CloseSocket(pSockInfo);
			LeaveCriticalSection(&m_cs);
			return FALSE;
		}
	}
	LeaveCriticalSection(&m_cs);
	return TRUE;
}
BOOL CGCSSockMgr::IsExist(GCSTS_Sock_Info* pSockInfo)
{
	EnterCriticalSection(&m_cs);
	BOOL bExist = FALSE;
	GCH_Map_PPoint_Ite pos = m_mapSocket.find((INT_PTR)pSockInfo);
	
	if(pos != m_mapSocket.end())
		bExist = TRUE;
	LeaveCriticalSection(&m_cs);
	return bExist;	
}
BOOL CGCSSockMgr::IsValidSock(GCSTS_Sock_Info* pSockInfo)
{
	if(pSockInfo == NULL)
	{
		return FALSE;
	}
	
	GCH_Map_PPoint_Ite pos = m_mapSocket.find((INT_PTR)pSockInfo);
	
	if(pos != m_mapSocket.end())
	{
		return pSockInfo->bActive;
	}
	return FALSE;	
}

BOOL CGCSSockMgr::AddSock(GCSTS_Sock_Info* pSockInfo)
{
	GCH_KTRACE(_T("CGCSSockMgr::AddSock  1  handle=%p"), pSockInfo);
	EnterCriticalSection(&m_cs);

	m_mapSocket[(INT_PTR)pSockInfo] = (INT_PTR)pSockInfo;
	GCH_KTRACE(_T("CGCSSockMgr::AddSock pSockInfo=%p, pSockInfo->s=%d, SocketCount=%d, memSocketCount=%d"), pSockInfo, pSockInfo->s, m_mapSocket.size(), m_memSocket.GetUseCount());
	LeaveCriticalSection(&m_cs);
	
	return TRUE;
}
void CGCSSockMgr::CloseSocket(HANDLE handle)
{
	GCH_KTRACE(_T("CGCSSockMgr::CloseSocket 1 handle=%p"), handle);
	
	EnterCriticalSection(&m_cs);
	GCSTS_Sock_Info* pSockInfo = (GCSTS_Sock_Info*)handle;
	GCH_KTRACE(_T("CGCSSockMgr::CloseSocket 2 handle=%p"), handle);
	if(IsValidSock(pSockInfo))
	{
		GCH_KTRACE(_T("CGCSSockMgr::CloseSocket pSockInfo=%p, pSockInfo->s=%d"), pSockInfo, pSockInfo->s);

		if(pSockInfo->pRecvOverlapped != NULL)
		{
			m_memOverlapped.MyDel(pSockInfo->pRecvOverlapped);
			pSockInfo->pRecvOverlapped = NULL;
		}
		GCH_Lst_Point_Ite pos;
		GCSTS_Base_Overlapped*   pOverlapped = NULL;
		for(pos=pSockInfo->lstSendOverlapped.begin(); pos!=pSockInfo->lstSendOverlapped.end(); pos++)
		{
			pOverlapped = (GCSTS_Base_Overlapped*)*pos;
			m_memOverlapped.MyDel(pOverlapped);
		}
		pSockInfo->lstSendOverlapped.clear();
		shutdown(pSockInfo->s, 0);
		closesocket(pSockInfo->s);
		pSockInfo->bActive = FALSE;

		CGCHDealQueMgr::InsertOtherData(pSockInfo, GCE_CLOSE_SOCKET);
		SetEvent(m_hEvent);
	}
	GCH_KTRACE(_T("CGCSSockMgr::CloseSocket 3 handle=%p"), handle);
	LeaveCriticalSection(&m_cs);
}
BOOL CGCSSockMgr::DeleteSock(HANDLE handle)
{
	if(handle == NULL)
		return FALSE;

	GCH_KTRACE(_T("CGCSSockMgr::DeleteSock 1"));
	EnterCriticalSection(&m_cs);
	GCH_Map_PPoint_Ite pos =  m_mapSocket.find((INT_PTR)handle);
	if(pos != m_mapSocket.end())
	{		
		m_mapSocket.erase((INT_PTR)handle);		
		GCSTS_Sock_Info* pSockInfo = (GCSTS_Sock_Info*)handle;
		GCH_XTRACE(_T("CGCSSockMgr::DeleteSock ,pSockInfo=%p ,pSockInfo->s=%d, SocketCount=%d, memSocketCount=%d"), pSockInfo, pSockInfo->s, m_mapSocket.size(), m_memSocket.GetUseCount());		
		
		if(pSockInfo->pRecvOverlapped != NULL)
		{
			m_memOverlapped.MyDel(pSockInfo->pRecvOverlapped);
			pSockInfo->pRecvOverlapped = NULL;
		}
		GCH_KTRACE(_T("CGCSSockMgr::DeleteSock 2"));

		GCH_Lst_Point_Ite pos;
		GCSTS_Base_Overlapped*   pOverlapped = NULL;
		for(pos=pSockInfo->lstSendOverlapped.begin(); pos!=pSockInfo->lstSendOverlapped.end(); pos++)
		{
			pOverlapped = (GCSTS_Base_Overlapped*)*pos;
			m_memOverlapped.MyDel(pOverlapped);
		}
		pSockInfo->lstSendOverlapped.clear();
		GCH_KTRACE(_T("CGCSSockMgr::DeleteSock 3"));

		if(pSockInfo->bActive)
		{
			shutdown(pSockInfo->s, 0);
			closesocket(pSockInfo->s);
			pSockInfo->bActive = FALSE;
			pSockInfo->s = 0;
		}
		GCH_KTRACE(_T("CGCSSockMgr::DeleteSock 4"));
		m_memSocket.MyDel(pSockInfo);
		GCH_KTRACE(_T("CGCSSockMgr::DeleteSock 5"));
	}
	LeaveCriticalSection(&m_cs);
	GCH_KTRACE(_T("CGCSSockMgr::DeleteSock 6"));
	return TRUE;
}

BOOL CGCSSockMgr::SendPacket(HANDLE handle, int nLen, char* pData)
{
	int nRet;
	DWORD dwSendBufferlen;

	GCSTS_Sock_Info *pSockInfo = (GCSTS_Sock_Info*)handle;
	if(nLen < 0 || nLen > PACKET_ONE_LENGTH)
	{
		GCH_ETRACE(_T("CGCSSockMgr::SendPacket , nLen==%d"), nLen);
		return FALSE;
	}	

	EnterCriticalSection(&m_cs);

	if(!CGCSSockMgr::IsValidSock(pSockInfo) || pSockInfo->s == 0)
	{
		LeaveCriticalSection(&m_cs);
		return FALSE;
	}

	int nHeadLen = sizeof(GCSTH_Packet_Frame_Header);

	GCSTS_Base_Overlapped *pOverlapped =  NewOverlapped(pSockInfo);
	if(pOverlapped == NULL)
	{
		CloseSocket(pSockInfo);
		LeaveCriticalSection(&m_cs);
		return FALSE;
	}
	pOverlapped->dwOperatCode = GCE_Operate_Send;

	GCSTH_Packet_Frame_Header* pHeader = (GCSTH_Packet_Frame_Header*)pOverlapped->szBuff;				

	//将包体打包到内
	memcpy(&pOverlapped->szBuff[nHeadLen], pData, nLen);

	pOverlapped->wLeft = (WORD)(nLen + sizeof(GCSTH_Packet_Frame_Header));			

	//添入包头
	pHeader->yFrameFlag=ZH_NET_FRAME_FLAG;
	pHeader->wDataLen = pOverlapped->wLeft-nHeadLen;
	pHeader->wCRC16=GC_CRC::CRC16((unsigned char*)pData,nLen);

	pOverlapped->wsabuf.buf=pOverlapped->szBuff;
	pOverlapped->wsabuf.len=pOverlapped->wLeft;
	nRet=WSASend(pSockInfo->s, &pOverlapped->wsabuf, 1, &dwSendBufferlen, 0, &pOverlapped->Overlapped,NULL);
	if(SOCKET_ERROR==nRet)
	{
		DWORD dwErr = WSAGetLastError();
		if (dwErr != ERROR_IO_PENDING)
		{
			GCH_ETRACE(_T("CGCSSockMgr::SendPacket , WriteFile failed. dwErr=%d, pSockInfo=%p, s=%d, pOverlapped=%p, wLeft=%d"),
				dwErr, pSockInfo, pSockInfo->s, pOverlapped, pOverlapped->wLeft);
			CloseSocket(pSockInfo);
			LeaveCriticalSection(&m_cs);
			return FALSE;
		}
	}

	LeaveCriticalSection(&m_cs);
	return TRUE;
}
BOOL CGCSSockMgr::SetOutDat(HANDLE handle, INT_PTR pData, BYTE yDataType)
{
	if(handle == NULL)
		return FALSE;

	EnterCriticalSection(&m_cs);
	GCSTS_Sock_Info *pSockInfo = (GCSTS_Sock_Info*)handle;

	GCH_Map_PPoint_Ite pos = m_mapSocket.find((INT_PTR)pSockInfo);
	if(pos != m_mapSocket.end())
	{	
		pSockInfo->yDataType = yDataType;
		pSockInfo->m_pOutDat = pData;
	}
	
	LeaveCriticalSection(&m_cs);
	return TRUE;
}
INT_PTR CGCSSockMgr::GetOutDat(HANDLE handle, BYTE& yDataType)
{
	if(handle == NULL)
		return FALSE;
	
	INT_PTR pOutDat = NULL;
	EnterCriticalSection(&m_cs);
	GCSTS_Sock_Info *pSockInfo = (GCSTS_Sock_Info*)handle;
	GCH_Map_PPoint_Ite pos = m_mapSocket.find((INT_PTR)pSockInfo);
	if(pos != m_mapSocket.end())
	{
		yDataType = pSockInfo->yDataType;
		pOutDat = pSockInfo->m_pOutDat;
	}
	
	LeaveCriticalSection(&m_cs);
	return pOutDat;
}

char* CGCSSockMgr::GetPeerIP(HANDLE handle, char* pszIp)
{
	if(handle == NULL)
		return 0;

	EnterCriticalSection(&m_cs);
	GCSTS_Sock_Info *pSockInfo = (GCSTS_Sock_Info*)handle;

	if(CGCSSockMgr::IsValidSock(pSockInfo))
	{
		strcpy(pszIp, pSockInfo->szIp);
		LeaveCriticalSection(&m_cs);
		return pszIp;
	}
	LeaveCriticalSection(&m_cs);
	return NULL;

}
WORD CGCSSockMgr::GetPeerPort(HANDLE handle)
{
	WORD wPort = 0;

	if(handle == NULL)
		return 0;

	GCSTS_Sock_Info *pSockInfo = (GCSTS_Sock_Info*)handle;

	EnterCriticalSection(&m_cs);
	if(CGCSSockMgr::IsValidSock(pSockInfo))
	{
		wPort = ntohs(pSockInfo->wPort);
	}
	LeaveCriticalSection(&m_cs);
	return wPort;
}
BOOL CGCSSockMgr::GetPeerAddress(HANDLE handle, char* pszIp, WORD& wPort)
{
	if(handle == NULL)
		return 0;

	GCSTS_Sock_Info *pSockInfo = (GCSTS_Sock_Info*)handle;

	EnterCriticalSection(&m_cs);
	if(CGCSSockMgr::IsValidSock(pSockInfo))
	{
		wPort = ntohs(pSockInfo->wPort);
		strcpy(pszIp, pSockInfo->szIp);
	}
	LeaveCriticalSection(&m_cs);
	return TRUE;
}
void CGCSSockMgr::OnCompleteEvent(DWORD dwBytesTransferred, HANDLE h, GCSTS_Base_Overlapped *pOverlapped)
{
	DWORD dwOperatCode = 0;
	
	GCSTS_Sock_Info *pSockInfo = (GCSTS_Sock_Info*)h;

	EnterCriticalSection(&m_cs);
	
	
	if(!CGCSSockMgr::IsValidSock(pSockInfo))
	{
		LeaveCriticalSection(&m_cs);
		return;
	}

	if(pOverlapped != pSockInfo->pRecvOverlapped)
	{
		GCH_Lst_Point_Ite pos;
		GCSTS_Base_Overlapped*   pOverlapped2 = NULL;
		for(pos=pSockInfo->lstSendOverlapped.begin(); pos!=pSockInfo->lstSendOverlapped.end(); pos++)
		{
			pOverlapped2 = (GCSTS_Base_Overlapped*)*pos;
			if(pOverlapped2 == pOverlapped)
			{
				break;
			}
		}

		if(pos==pSockInfo->lstSendOverlapped.end())
		{
			CloseSocket(h);
			LeaveCriticalSection(&m_cs);
			return;
		}
	}

	dwOperatCode = pOverlapped->dwOperatCode;
	if(GCE_Operate_Send == dwOperatCode)
	{
		//发送完成
		SendComplete(dwBytesTransferred, h, (GCSTS_Base_Overlapped*)pOverlapped);	
	}
	else
	{
		//接收数据
		RecvComplete(dwOperatCode, dwBytesTransferred, h,pOverlapped, pSockInfo);
	}

	LeaveCriticalSection(&m_cs);
}
BOOL CGCSSockMgr::SendComplete(DWORD dwBytesTransferred, HANDLE h, GCSTS_Base_Overlapped *pOverlapped)
{
	DelOverlapped(h, pOverlapped);
	return TRUE;
}
BOOL CGCSSockMgr::RecvComplete(DWORD dwOperatCode, DWORD dwBytesTransferred, HANDLE h, GCSTS_Base_Overlapped *pOverlapped, GCSTS_Sock_Info *pSockInfo)
{
	//检测是否已经获取加密种子
	if(FALSE==pSockInfo->isAlreadyGetEncryptSeed)
	{
		if(0==pOverlapped->wIndex)
		{
			pOverlapped->wLeft=4;//固定初始化是4个字节的加密钥匙
		}
		if(dwBytesTransferred == pOverlapped->wLeft)
		{
			pSockInfo->isAlreadyGetEncryptSeed=TRUE;

			//正式进入接收帧数据
			if(!RecvDataPart(h, 0, 0, pOverlapped, GCE_Operate_Recv_Head))
			{
				return FALSE;
			}
		}
		else
		{
			//继续接收加密种子
			if(!RecvDataPart(h, dwBytesTransferred, 0, pOverlapped, GCE_Operate_Recv_Body))
			{
				return FALSE;
			}
		}
		return TRUE;
	}
	//已经获取加密种子
	if(dwBytesTransferred == pOverlapped->wLeft)
	{

		switch(dwOperatCode)
		{
		case GCE_Operate_Recv_Head:
			{
				//接收到的是数据头
				GCSTH_Packet_Frame_Header *pHeader = (GCSTH_Packet_Frame_Header*)(pOverlapped->szBuff);
				
				if(pHeader->wDataLen > PACKET_ONE_HEADER_LENGTH)
				{
					//数据体长度错误，关闭SOCKET
					CloseSocket(h);
				}
				else if(pHeader->wDataLen > 0)
				{
					//接收数据体
					if(!RecvDataPart(h, 0, pHeader->wDataLen, pOverlapped, GCE_Operate_Recv_Body))
					{
						return FALSE;	
					}
				}
				else
				{
					//没有数据体
					//插入数据
					GCSTH_Packet_Frame *pPacket = (GCSTH_Packet_Frame*)pOverlapped->szBuff;
					CGCHDealQueMgr::InsertWaitDealData(h, 
						pPacket->header.yFrameFlag,
						pPacket->header.wCRC16,
						GCE_OP_NONE, 
						pPacket->header.wDataLen, 
						pPacket->data);
					SetEvent(m_hEvent);
					//继续接收新的包头
					if(!RecvDataPart(h, 0, 0, pOverlapped, GCE_Operate_Recv_Head))
					{
						return FALSE;	
					}
				}
			}
			break;
		case GCE_Operate_Recv_Body:
			{
				//收到了完整的数据包
				GCSTH_Packet_Frame* pPacket = (GCSTH_Packet_Frame*)pOverlapped->szBuff;
				
				//GCH_TRACE(_T("CGCSIoCP::RecvComplete wDataLen=%d.yTotal=%d, yPacketNo=%d, data=%s"), 
				//	pPacket->header.wDataLen, pPacket->header.yTotal, pPacket->header.yPacketNo, pPacket->data);
				//插入数据
				CGCHDealQueMgr::InsertWaitDealData(h,
					pPacket->header.yFrameFlag,
					pPacket->header.wCRC16,
					GCE_OP_NONE, 
					pPacket->header.wDataLen, 
					pPacket->data);
				SetEvent(m_hEvent);
				
				//接收新的包头
				if(!RecvDataPart(h, 0, 0, pOverlapped, GCE_Operate_Recv_Head))
				{
					return FALSE;
				}
			}
			break;
		default:
			GCH_ETRACE(_T("CGCSIoCP::RecvComplete 1 dwOperatCode=%d, dwBytesTransferred=%d, h=%p, pOverlapped=%p"), 
				dwOperatCode, dwBytesTransferred, h, pOverlapped);
			return false;
			break;
		}
	}
	else if(dwBytesTransferred < pOverlapped->wLeft)
	{
		switch(dwOperatCode)
		{
		case GCE_Operate_Recv_Head:
			//包头没有接收完成 
			if(!RecvDataPart(h, dwBytesTransferred, 0, pOverlapped, GCE_Operate_Recv_Head))
			{
				return FALSE;
			}
			break;
		case GCE_Operate_Recv_Body:
			//包体没有接收完成
			if(!RecvDataPart(h, dwBytesTransferred, 0, pOverlapped, GCE_Operate_Recv_Body))
			{
				return FALSE;
			}
			break;
		default:
			GCH_ETRACE(_T("CGCSIoCP::RecvComplete 2 dwOperatCode=%d, dwBytesTransferred=%d, h=%p, pOverlapped=%p"), 
				dwOperatCode, dwBytesTransferred, h, pOverlapped);
			return FALSE;
			break;
		}
	}
	else
	{
		GCH_ETRACE(_T("CGCSIoCP::RecvComplete dwBytesTransferred=%d > pOverlapped->wLeft=%d"), dwBytesTransferred, pOverlapped->wLeft);				
		return FALSE;
	}
	return TRUE;
}
DWORD WINAPI CGCSSockMgr::WorkDealThread( void *pVoid) 
{
	CGCSSockMgr* pSockMgr = (CGCSSockMgr*)pVoid;
	HANDLE * h = NULL;
	GCSTS_Base_Overlapped *pOverlapped=NULL; 

	while(g_bGCThreadRun)
	{

		GCH_KTRACE(_T("CGCSIoCP::WorkDealThread 1"));
		DWORD dwBytesTransferred = 0;

		BOOL ret =GetQueuedCompletionStatus(pSockMgr->m_hCompletionPort, &dwBytesTransferred,(LPDWORD)&h,  (LPOVERLAPPED *)&pOverlapped, INFINITE);
		if(!pSockMgr->ErrDeal(ret, dwBytesTransferred, h, pOverlapped))
		{
			continue;
		}
		GCH_KTRACE(_T("CGCSIoCP::WorkDealThread 2"));
		pSockMgr->OnCompleteEvent(dwBytesTransferred, h, pOverlapped);
		GCH_KTRACE(_T("CGCSIoCP::WorkDealThread 3"));

	}
	return 0;
}
BOOL CGCSSockMgr::ErrDeal(BOOL ret, DWORD dwBytesTransferred,  HANDLE h, GCSTS_Base_Overlapped *pOverlapped)
{
	int err = WSAGetLastError();

	//GCH_ETRACE(_T("CGCSSockMgr::ErrDeal "), _T("ret=%d, err=%d, dwBytesTransferred=%d, h=%p, pOverlapped=%p, "), ret, err, dwBytesTransferred, h, pOverlapped);
	//	If a socket handle associated with a completion port is closed, GetQueuedCompletionStatus returns ERROR_SUCCESS, 
	//with lpNumberOfBytes equal zero.
	if(dwBytesTransferred==0/* && err == ERROR_SUCCESS*/)
	{
		CloseSocket(h);
		return FALSE;
	}

	//	If *lpOverlapped is NULL and the function does not dequeue a completion packet from the completion port, the return value is zero. 
	//The function does not store information in the variables pointed to by the lpNumberOfBytes and lpCompletionKey parameters. To get extended error
	//information, call GetLastError. If the function did not dequeue a completion packet because the wait timed out, GetLastError returns WAIT_TIMEOUT.
	if(ret == FALSE && pOverlapped == NULL && err == WAIT_TIMEOUT)
	{
		return FALSE;
	}
	//	If *lpOverlapped is not NULL and the function dequeues a completion packet for a failed I/O operation from the completion port, 
	//the return value is zero. The function stores information in the variables pointed to by lpNumberOfBytes, lpCompletionKey, 
	//and lpOverlapped. To get extended error information, call GetLa stError.
	if(ret == FALSE && pOverlapped != NULL && err != ERROR_IO_PENDING)
	{
		CloseSocket(h);
		return FALSE;
	}

	//	If the function dequeues a completion packet for a successful I/O operation from the completion port, the return value is nonzero. 
	//The function stores information in the variables pointed to by the lpNumberOfBytesTransferred, lpCompletionKey, and lpOverlapped parameters.
	if(ret == TRUE)
	{
		return TRUE;
	}

	CloseSocket(h);

	return FALSE;
}
