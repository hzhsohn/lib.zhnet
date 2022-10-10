#include "stdafx.h"

#include <MSTcpIP.h>
#include "GCSSockMgr.h"
#include "GCHDealQueMgr.h"
#include "GCHpacket.h"
#include "GCSRTNet.h"
#include "GCHDealPacket.h"

DWORD	g_dwPacketThreadID[2]={0,0};

PFUN_RECV_DATA_CALLBACK		g_pfnReceiveDataCallback=NULL;
PFUN_DISCONNECT_CALLBACK		g_pfnDisconnectCallback=NULL;
PFUN_ACCEPT_CALLBACK		g_pfnAcceptCallback=NULL;

HANDLE g_hThreadHandle[1024];		//保存线程句柄，用于在程序结束时，关闭线程
int g_nThreadHandleCnt = 0;			//线程数

CGCSSockMgr* g_pSockMgr=NULL;
int g_nWorkThreadCnt = 1;		//工作线程数量，一个工作线程对应一个CGCSSockMgr，一个完成端口句柄
bool g_bGCThreadRun;

BOOL GCEApiSRTNetDestroy();

CGCSSockMgr*  GetSocketMgr(HANDLE handle)
{
	GCSTS_Sock_Info* p = (GCSTS_Sock_Info*)handle;
	int i;
	for(i=0; i<g_nWorkThreadCnt; i++)
	{
		if(g_pSockMgr[i].IsExist(p))
			return &g_pSockMgr[i];
	}
	return NULL;
}

void  DeleteAllSocketMgr()
{
	int i;
	for(i=0; i<g_nWorkThreadCnt; i++)
	{
		g_pSockMgr[i].DisconnectAll();
	}
}

void DestroySocketMgr()
{
	if(g_pSockMgr == NULL)
		return;
	int i;
	for(i=0; i<g_nWorkThreadCnt; i++)
	{
		if(g_pSockMgr)
		g_pSockMgr[i].Destroy();
	}
	delete []g_pSockMgr;
	g_pSockMgr = NULL;
}

DLLEXPORT_API VOID WINAPI GCApiSRTNetProgramLoad()
{
		g_nThreadHandleCnt = 0;
		g_nWorkThreadCnt = 1;
		g_bGCThreadRun=true;
		CGCHDealQueMgr::Init();
		CGCSIoCP::Init();
}
DLLEXPORT_API VOID WINAPI GCApiSRTNetProgramUnLoad()
{
		DeleteAllSocketMgr();
		GCEApiSRTNetDestroy();
		g_bGCThreadRun=false;
		CGCHDealQueMgr::Destroy();
		CGCSIoCP::Destroy();
		
		//不调用DeleteAllSocketMgr();GCEApiSRTNetDestroy();
		//如果带连接加了这句在服务器关闭的时候会出错
		DestroySocketMgr();
}
/////////////////////////////////////////////////////////////////////////
//不管是服务器端，还是客户端都调用这个初始化函数
DLLEXPORT_API BOOL WINAPI GCEApiSRTNetInit(PFUN_RECV_DATA_CALLBACK pfnRecvDataCallback,
										   PFUN_DISCONNECT_CALLBACK pfnDisconnectCallback, 
										   PFUN_ACCEPT_CALLBACK pfnAcceptCallback, 
										   WORD wPort)
{
	GCH_MTRACE(_T("GCEApiSRTNetInit pfnRecvDataCallback=%p, pfnDisconnectCallback=%p, pfnAcceptCallback=%p"), pfnRecvDataCallback, pfnDisconnectCallback, pfnAcceptCallback);
	GCH_MTRACE(_T("GCEApiSRTNetInit wPort=%d"), wPort);

	if(pfnRecvDataCallback == NULL || pfnDisconnectCallback==NULL || pfnAcceptCallback == NULL)
	{
		GCH_ETRACE(_T("GCEApiSRTNetInit pfnRecvDataCallback == NULL || pfnDisconnectCallback==NULL || pfnAcceptCallback == NULL."));
		return FALSE;
	}
	g_pfnReceiveDataCallback = pfnRecvDataCallback;
	g_pfnDisconnectCallback = pfnDisconnectCallback;
	g_pfnAcceptCallback = pfnAcceptCallback;

	DWORD dwThreadId = 0;
	HANDLE hThread = NULL;

	if(!CGCSIoCP::InitNet(wPort))
	{
		GCH_ETRACE(_T("GCEApiSRTNetInit CGCSIoCP::Init failed.port=%d"), wPort);
		return FALSE;
	}

	//计算工作线程数
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	g_nWorkThreadCnt = systemInfo.dwNumberOfProcessors * 2 + 2;
	g_pSockMgr = new CGCSSockMgr[g_nWorkThreadCnt];
	
	int i;
	for(i=0; i<g_nWorkThreadCnt; i++)
	{
		g_pSockMgr[i].Init(CGCSIoCP::m_hRecvEvent);
	}

	//3.创建接收处理线程
	hThread = CreateThread(NULL, 0, CGCSIoCP::DealRecvDataThread, NULL, 0, &dwThreadId);
	if(hThread==NULL)
	{
		GCH_ETRACE(_T("GCEApiSRTNetInit CreateThread CGCSIoCP::DealRecvDataThread failed."));
		return FALSE;
	}
	g_hThreadHandle[g_nThreadHandleCnt++] = hThread;

	//4.创建接收了连接线程
	if(pfnAcceptCallback != NULL && wPort != 0)
	{
		hThread = CreateThread(NULL, 0, CGCSIoCP::AcceptThread, &wPort, 0, &dwThreadId);
		if(hThread==NULL)
		{
			GCH_ETRACE(_T("GCEApiSRTNetInit CreateThread CGCSIoCP::AcceptThread failed."));
			return FALSE;
		}
		g_hThreadHandle[g_nThreadHandleCnt++] = hThread;
	}

	CGCHDealPacket::SetNoreplySendCallBack(GCEApiSRTNetSendPacket);
	Sleep(100);

	return TRUE;
}
BOOL GCEApiSRTNetDestroy()
{
	int i;

	for(i=0; i<g_nThreadHandleCnt; i++)
	{
		TerminateThread(g_hThreadHandle[i], -1);
		CloseHandle(g_hThreadHandle[i]);
		GCH_MTRACE(_T("GCEApiSRTNetDestroy g_hThreadHandle[%d]=%d."), i, g_hThreadHandle[i]);
	}
	g_nThreadHandleCnt=0;
	CGCSIoCP::Destroy();
	return TRUE;
}

DLLEXPORT_API char* WINAPI GCEApiSRTNetGetIp(char*host,char*ip)
{
	WSADATA wsaData;
	PHOSTENT hostinfo; 
	char*ip_cstr;
	unsigned long lgIP;
	ip_cstr=NULL;
	lgIP = inet_addr(host);   
	//输入的IP字符串,这是适应WINCE
	if(lgIP != INADDR_NONE)  
	{
		return host;
	}

	if(WSAStartup(MAKEWORD(2,0), &wsaData)== 0)
	{ 
		if((hostinfo = gethostbyname(host)) != NULL)
		{
			ip_cstr = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list); 
		} 
		WSACleanup();
	}

	if(ip_cstr)
	{
		strcpy(ip,ip_cstr);
	}
	else
	{
		strcpy(ip,_T(""));
	}
	return ip;
}


DLLEXPORT_API HANDLE WINAPI GCEApiSRTNetConnect(char* pszIP, WORD wPort, int nTimeOut)
{
	GCSTS_Sock_Info *pSockInfo = NULL;
	short nhKeyCalcul=0;

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	if(s == INVALID_SOCKET)
	{
		GCH_ETRACE(_T("GCEApiSRTNetConnect s == INVALID_SOCKET"));
		return NULL;
	}

	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(wPort);
	addr.sin_addr.s_addr = inet_addr(pszIP);

	if(connect(s, (SOCKADDR*)&addr, sizeof(addr))==SOCKET_ERROR)
	{
		GCH_ETRACE(_T("GCEApiSRTNetConnect connect to %s:%d faield."), pszIP, wPort);
		GCH_ETRACE(_T("GCEApiSRTNetConnect closesocket  start s=%d"), s);	
		closesocket(s);
		GCH_ETRACE(_T("GCEApiSRTNetConnect closesocket  end s=%d"), s);	
		return NULL;
	}

	//设置积极连接超时
	struct tcp_keepalive tcpin;
	tcpin.onoff=1;
	tcpin.keepaliveinterval=1000;
	tcpin.keepalivetime=6000;
	DWORD dwSize = 0;
	int err=WSAIoctl(s,SIO_KEEPALIVE_VALS, &tcpin, sizeof(tcpin), NULL,0, &dwSize,NULL,NULL);

	pSockInfo=g_pSockMgr[s%g_nWorkThreadCnt].NewSocket(s, (SOCKADDR*)&addr);
	
	if(pSockInfo == NULL)
	{
		GCH_ETRACE(_T("GCEApiSRTNetConnect pHandle == NULL."));
	}
	else
	{
		//未获取钥匙前不前进		
		while(true)
		{
			if(pSockInfo->isAlreadyGetEncryptSeed)
				break;
			if(nhKeyCalcul++>GC_KEY_CONNECT_TIMEOUT)
			{
				ResetEvent(pSockInfo->hKey);
				GCH_ETRACE(_T("GCEApiSRTNetConnect GetKey Fail. nhKeyCalcul=%d pSockInfo->nKey=%d"), nhKeyCalcul,pSockInfo->nKey);
				closesocket(pSockInfo->s);
				pSockInfo=NULL;
				break;
			}
			if(false==g_bGCThreadRun)return NULL; 
			WaitForSingleObject(pSockInfo->hKey,1000);

		}
	}
	if(false==g_bGCThreadRun)return NULL; 
	return pSockInfo;
}

DLLEXPORT_API BOOL WINAPI GCEApiSRTNetDisconnect(HANDLE handle)
{
	CGCSSockMgr* pSockMgr = GetSocketMgr(handle);
	if(pSockMgr == NULL)
		return FALSE;

	pSockMgr->DeleteSock(handle);

	return TRUE;
}


BOOL WINAPI GCEApiSRTNetSendPacket(HANDLE handle, int nLen, char* pData)
{
	CGCSSockMgr* pSockMgr = GetSocketMgr(handle);
	if(pSockMgr == NULL)
		return FALSE;

	return pSockMgr->SendPacket(handle, nLen, pData);
}

BOOL WINAPI  GCEApiSRTNetSetOutDat(HANDLE handle, INT_PTR pData, BYTE yDataType)
{
	CGCSSockMgr* pSockMgr = GetSocketMgr(handle);
	if(pSockMgr == NULL)
		return FALSE;

	return pSockMgr->SetOutDat(handle, pData, yDataType);
}

INT_PTR WINAPI GCEApiSRTNetGetOutDat(HANDLE handle, BYTE& yDataType)
{
	CGCSSockMgr* pSockMgr = GetSocketMgr(handle);
	if(pSockMgr == NULL)
		return NULL;

	return pSockMgr->GetOutDat(handle, yDataType);
}

char* WINAPI GCEApiSRTNetGetPeerIP(HANDLE handle, char* pszIp)
{
	CGCSSockMgr* pSockMgr = GetSocketMgr(handle);
	if(pSockMgr == NULL)
		return NULL;

	return pSockMgr->GetPeerIP(handle, pszIp);
}

WORD WINAPI GCEApiSRTNetGetPeerPort(HANDLE handle)
{
	CGCSSockMgr* pSockMgr = GetSocketMgr(handle);
	if(pSockMgr == NULL)
		return 0;

	return pSockMgr->GetPeerPort(handle);
}
