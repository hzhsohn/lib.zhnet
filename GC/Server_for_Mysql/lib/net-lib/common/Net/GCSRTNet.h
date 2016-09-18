#pragma once
#include <wtypes.h>
#include "GCHUtilMarco.h"
#include "GCHNetUtil.h"
#include "GCHEncrypt.h"

DLLEXPORT_API VOID WINAPI GCApiSRTNetProgramLoad();
DLLEXPORT_API VOID WINAPI GCApiSRTNetProgramUnLoad();

DLLEXPORT_API BOOL WINAPI GCEApiSRTNetInit(PFUN_RECV_DATA_CALLBACK pfnRecvDataCallback,
											PFUN_DISCONNECT_CALLBACK pfnDisconnectCallback, 
											PFUN_ACCEPT_CALLBACK pfnAcceptCallback, 
											WORD wPort=0,
										    BOOL isUseEnpty=FALSE);
DLLEXPORT_API BOOL WINAPI GCEApiSRTNetSendPacket(HANDLE handle, int nLen, char* pData);

DLLEXPORT_API char* WINAPI GCEApiSRTNetGetIp(char*host,char*ip);//域名获取IP
DLLEXPORT_API HANDLE WINAPI GCEApiSRTNetConnect(char* pszIP, WORD wPort, int nTimeOut=5000);//默认10秒
DLLEXPORT_API BOOL WINAPI GCEApiSRTNetDisconnect(HANDLE handle=NULL);

DLLEXPORT_API BOOL WINAPI  GCEApiSRTNetSetOutDat(HANDLE handle, INT_PTR pData, BYTE yDataType);
DLLEXPORT_API INT_PTR WINAPI GCEApiSRTNetGetOutDat(HANDLE handle, BYTE& yDataType);
DLLEXPORT_API char* WINAPI GCEApiSRTNetGetPeerIP(HANDLE handle, char* pszIp);
DLLEXPORT_API WORD WINAPI GCEApiSRTNetGetPeerPort(HANDLE handle);
