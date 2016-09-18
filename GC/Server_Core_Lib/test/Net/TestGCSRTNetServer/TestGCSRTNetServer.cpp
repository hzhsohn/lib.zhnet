// TestGCSRTNet.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "time.h"
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
void WINAPI RecvDataCallBack(HANDLE handle, int nLen, char* pData);

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
	TCHAR ip[32];
	GSRGetPeerIP(handle,ip);
	
	_tprintf(_T("ip=%s handle=%p,  nLen=%d\n"),ip,handle, nLen);
	
	GCSetPack pack;
	int i;
	char str[1024];

	pack.beginRead(pData,nLen);
	pack.readInt(&i);
	pack.readString(str);

	_tprintf(_T("Recv Socket=%p ,i =%d , str=%s time=%d\n\n"),handle,i,str,time(NULL));

	//返回到客户端
	pack.beginWrite();
	pack.writeString("好啦,服务器的我接收到了..");
	GSRSend(handle, pack.getPackSize(), pack.getPackBuf());
}

int _tmain(int argc, _TCHAR* argv[])
{
	GSRTBegin();
	GCH_INIT_LOG();
	_tprintf(_T("The application is server.\n"));
	if(!GSRTInit(RecvDataCallBack,DissconnectCallBack,AcceptCallBack, 2323))
	{
		_tprintf(_T("GCEApiSRTNetInit failed as server.\n"));
		getchar();
		return 0;
	}

	int nExitCode;
	while(true)
	{
		scanf( "%d", &nExitCode);
		if(nExitCode == 0)
		{
			break;
		}
	}
	GCH_CLOSE_LOG();
	GSRTEnd();
	_tprintf(_T("\nend ok..\n"));
	return 0;
}

