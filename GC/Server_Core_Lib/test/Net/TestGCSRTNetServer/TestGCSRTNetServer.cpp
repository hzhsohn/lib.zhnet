// TestGCSRTNet.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "time.h"
#include "GCSRTNet.h"
#include "GCHUtil.h"
#include "GCHELog.h"
#include "GCSetPack.h"

#pragma comment(lib,"GCSetPack.lib")

//���տͻ��˵���������Ļص�����
void WINAPI AcceptCallBack(HANDLE handle, TCHAR *pszIP, WORD wPort);
//�Ͽ��ͻ������ӵĻص�����
void WINAPI DissconnectCallBack(HANDLE handle);
//���յ����ݰ�ʱ�Ļص�����
void WINAPI RecvDataCallBack(HANDLE handle, int nLen, char* pData);

//���տͻ��˵���������Ļص�����
void WINAPI AcceptCallBack(HANDLE handle, TCHAR *pszIP, WORD wPort)
{
	_tprintf(_T("AcceptCallBack-->handle=%p, pszIP=%s, wPort=%04d \n"), handle, pszIP, wPort);	
	return;
}
//�Ͽ��ͻ������ӵĻص�����
void WINAPI DissconnectCallBack(HANDLE handle)
{	
	//ֱ���˳�,�����������
	_tprintf(_T("DissconnectCallBack-->handle=%p\n"), handle);
	return;
}

//���յ����ݰ�ʱ�Ļص�����
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

	//���ص��ͻ���
	pack.beginWrite();
	pack.writeString("����,���������ҽ��յ���..");
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

