#pragma once

#include "GCHELog.h"
#include "GCSRTNet.h"
#include "GCHUtil.h"
#include "MySQLProc.h"

//接收客户端的连接请求的回调函数
void WINAPI AcceptCallBack(HANDLE handle, TCHAR *pszIP, WORD wPort);
//断开客户端连接的回调函数
void WINAPI DissconnectCallBack(HANDLE handle);
//接收到数据包时的回调函数
void WINAPI RecvDataCallBack(HANDLE handle,int nLen, char* pData);
//MYSQL处理函数
void ProcMysql(EzhMysqlOperator result,CzhMySql *mysql,TzhSqlInfo*info);

