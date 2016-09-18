#pragma once
#include "GCHDealQueMgr.h"
#include "gchdealquemgr.h"
#include "GCSMemManager.h"
#include "GCSSockMgr.h"

class CGCSIoCP
{
public:
	static BOOL Init(void);
	static BOOL Destroy();
	
	static BOOL InitNet(WORD wPort);
	static BOOL InitListenSocket(WORD wPort);
	
	static void ProcessData(GCSTH_RecvDataInfo* pRecvData,GCSTS_Sock_Info* pSock, int nLen, char* pData);
	static DWORD WINAPI AcceptThread(void* pVoid);
	static DWORD WINAPI WorkDealThread( void *pVoid);
	static DWORD WINAPI DealRecvDataThread( void *pVoid);

	static BOOL SendEncryptKey(CGCSSockMgr* pSockMgr,GCSTS_Sock_Info *pSockInfo);	

	static HANDLE m_hRecvEvent;

private:
	static SOCKET	m_sListen;
	static WORD m_dwPacketNumber;
	
	static BOOL m_bActive;
};
