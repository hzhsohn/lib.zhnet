#pragma once

#include "GCHELog.h"
#include "GCSRTNet.h"
#include "GCHUtil.h"
#include "MySQLProc.h"

//���տͻ��˵���������Ļص�����
void WINAPI AcceptCallBack(HANDLE handle, TCHAR *pszIP, WORD wPort);
//�Ͽ��ͻ������ӵĻص�����
void WINAPI DissconnectCallBack(HANDLE handle);
//���յ����ݰ�ʱ�Ļص�����
void WINAPI RecvDataCallBack(HANDLE handle,int nLen, char* pData);
//MYSQL������
void ProcMysql(EzhMysqlOperator result,CzhMySql *mysql,TzhSqlInfo*info);

