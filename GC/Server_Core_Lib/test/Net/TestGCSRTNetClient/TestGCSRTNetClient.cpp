// TestGCSRTNet.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "GCSRTNet.h"
#include "GCHUtil.h"
#include "GCHELog.h"
#include "GCSetPack.h"

#pragma comment(lib,"GCSetPack.lib")

//接收客户端的连接请求的回调函数
void WINAPI AcceptCallBack(HANDLE handle, TCHAR *pszIP, WORD wPort);
//断开客户端连接的回调函数
void WINAPI DissconnectCallBack(HANDLE handle);
//接收到数据包时的回调函数
void WINAPI RecvDataCallBack(HANDLE handle, WORD wCmd, int nLen, char* pData);

//接收客户端的连接请求的回调函数
void WINAPI AcceptCallBack(HANDLE handle, TCHAR *pszIP, WORD wPort)
{
	_tprintf(_T("AcceptCallBack-->handle=%p, pszIP=%s, wPort=%04d \n"), handle, pszIP, wPort);	
	return;
}
//断开客户端连接的回调函数
void WINAPI DissconnectCallBack(HANDLE handle)
{	
	//直接退出,或放入重链表
	_tprintf(_T("DissconnectCallBack-->handle=%p\n"), handle);
	return;
}

//接收到数据包时的回调函数
void WINAPI RecvDataCallBack(HANDLE handle, int nLen, char* pData)
{
	_tprintf(_T("NoReplyCmd-->handle=%p, nLen=%d\n"), handle,  nLen);
}

int _tmain(int argc, _TCHAR* argv[])
{
	//_T("TestGCSRTNetClient.log")
	GSRTBegin();
	GCH_INIT_LOG();
	_tprintf(_T("The application is client.\n"));

	if(!GSRTInit(RecvDataCallBack,DissconnectCallBack,AcceptCallBack))
	{
		_tprintf(_T("GCEApiSRTNetInit failed as client.\n"));
		getchar();
		return 0;
	}
	
	for(int i=3;i>0;i--)
	{
		_tprintf(_T("After %d Sec Begin\n"),i);
		Sleep(1000);
	}

	HANDLE handle;
	TCHAR ip[16];
	GSRTGetIp(_T("localhost"),ip);
	handle=GSRTConnect(ip,2323);
	if(handle == NULL)
	{
		_tprintf(_T("GCEApiSRTNetConnect to failed .\n"));
		getchar();
		return 0;
	}

	while(true)
	{		
		GCSetPack pack;
		pack.beginWrite();
		pack.writeInt( 2);
		pack.writeString("可爱");
		if(!GCEApiSRTNetSendPacket(handle, pack.getPackSize(), pack.getPackBuf()))
		{
			_tprintf(_T("GCEApiSRTNetNoReplySendPacket failed .\n"));
			getchar();
			return 0;
		}
		Sleep(2000);
	}
	GCH_CLOSE_LOG();
	GSRTEnd();
	return 0;
}

