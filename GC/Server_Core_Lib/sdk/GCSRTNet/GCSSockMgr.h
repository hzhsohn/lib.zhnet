#pragma once

#include <list>
#include <algorithm>
#include "memMgr.h"
#include "GCHLength.h"
#include "GCHType.h"

using namespace std;


//最多管理的连接数，应该从配制文件中读取，同时也是最大动态内存数
#define GCHMAC_FREE_SOCK_COUNT			2000
#define GCHMAC_MAX_SOCK_COUNT			20000

//最大接收缓冲区，应该和允许的最大连接数一至
#define GCHMAC_FREE_OVERLAPPED_COUNT	4000  
#define GCHMAC_MAX_OVERLAPPED_COUNT		200000  

enum GCEM_Commplet_Port_Operate_Type
{
	GCE_Operate_Accept,
	GCE_Operate_Recv_Head,
	GCE_Operate_Recv_Body,
	GCE_Operate_Send,
};

struct GCSTS_Base_Overlapped
{	
	OVERLAPPED	Overlapped;
	DWORD		dwOperatCode;
	WORD		wLeft;		//剩余的数量.
	WORD		wIndex;	//当前位置
	char		szBuff[PACKET_ONE_HEADER_LENGTH];	
	WSABUF wsabuf;
};

struct GCSTS_Sock_Info
{
	SOCKET	s;
	BOOL	bActive;
	char	szIp[GCHMAC_MAX_IP_LEN];
	WORD	wPort;
	BYTE	yDataType;
	INT_PTR	m_pOutDat;	
	int     nLastActiveTime;
	GCSTS_Base_Overlapped*   pRecvOverlapped;
	GCH_Lst_Point lstSendOverlapped;

	//加密钥匙和加密类型
	BOOL		isAlreadyGetEncryptSeed;			//是否已经获取加密钥匙
	int			nKey;				//加密钥匙
	BOOL		isVariFlowEncrypt;  //是变流加密
	HANDLE		hKey;

	GCSTS_Sock_Info()
	{
		s = 0;
		memset(szIp, 0, sizeof(szIp));
		wPort = 0;
		yDataType = 0;
		m_pOutDat = NULL;
		nLastActiveTime = 0;
		pRecvOverlapped = NULL;
		isVariFlowEncrypt=GCApiGetEncryptState();
		nKey= GCApiGetDefaultKey();
		isAlreadyGetEncryptSeed=FALSE;
		hKey=CreateEvent(NULL,true,false,NULL);
	}
};

typedef map <INT_PTR, INT_PTR>			GCH_Map_PPoint;
typedef GCH_Map_PPoint::iterator		GCH_Map_PPoint_Ite;


class CGCSSockMgr
{
private:
	HANDLE				m_hEvent;

	GCH_Map_PPoint		m_mapSocket;
	
	
	CMemMgr<GCSTS_Sock_Info>		m_memSocket;
	CMemMgr<GCSTS_Base_Overlapped>  m_memOverlapped;	

	HANDLE m_hCompletionPort;
public:
	CGCSSockMgr();
	~CGCSSockMgr();
	CRITICAL_SECTION		m_cs;
	BOOL Init(HANDLE hEvent);
	void DisconnectAll(void);
	BOOL Destroy(void);
	void DelOverlapped(HANDLE h, GCSTS_Base_Overlapped * pOverLapped);
	GCSTS_Base_Overlapped * NewOverlapped(GCSTS_Sock_Info * pSockInfo);
	GCSTS_Sock_Info *NewSocket(SOCKET s, SOCKADDR* pAddr);
	BOOL IsExist(GCSTS_Sock_Info* pSockInfo);
	BOOL IsValidSock(GCSTS_Sock_Info* pSockInfo);
	BOOL AddSock(GCSTS_Sock_Info* pSockInfo);
	void CloseSocket(HANDLE handle);
	BOOL DeleteSock(HANDLE handle);

	BOOL RecvDataPart(HANDLE handle, DWORD dwBytesTransferred, DWORD dwPacketBodyLen, GCSTS_Base_Overlapped *pOverlapped, int nOpCode, BOOL bClose=TRUE);
	BOOL SendPacket(HANDLE handle, int nLen, char* pData);
	BOOL SetOutDat(HANDLE handle, INT_PTR pData, BYTE yDataType);
	INT_PTR GetOutDat(HANDLE handle, BYTE& yDataType);
	char* GetPeerIP(HANDLE handle, char* pszIp);
	WORD GetPeerPort(HANDLE handle);
	BOOL GetPeerAddress(HANDLE handle, char* pszIp, WORD& wPort);

	void OnCompleteEvent(DWORD dwBytesTransferred, HANDLE h, GCSTS_Base_Overlapped *pOverlapped);
	BOOL SendComplete(DWORD dwBytesTransferred, HANDLE h, GCSTS_Base_Overlapped *pOverlapped);
	BOOL RecvComplete(DWORD dwOperatCode, DWORD dwBytesTransferred, HANDLE h, GCSTS_Base_Overlapped *pOverlapped, GCSTS_Sock_Info *pSockInfo);

	static DWORD WINAPI WorkDealThread( void *pVoid) ;
	BOOL ErrDeal(BOOL ret, DWORD dwBytesTransferred,  HANDLE h, GCSTS_Base_Overlapped *pOverlapped);
};
