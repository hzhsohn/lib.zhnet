#include "StdAfx.h"
#include ".\gchelog.h"
#include "GCHIniFile.h"
/*
* Copyright (c) 2010,
* All rights reserved.

* Filename: GCHELog.cpp
* Description:

* Author:bloodspider
* Time:2007-3-14   23:27
* Modify connect:

* Alter:Sohn
*Time:2010-3-29 22:03

*/

TCHAR GCHELog::m_szAppName[_MAX_PATH] = _T("");
TCHAR GCHELog::m_szLogPath[_MAX_PATH];
FILE* GCHELog::m_fLog = NULL;
bool GCHELog::m_isLoad=false;
int	GCHELog::m_nDate = 0;
CRITICAL_SECTION GCHELog::m_cs;


GCHELog::GCHELog()
{

}
GCHELog::~GCHELog()
{
	
}

void GCHELog::InitLogFile()
{
	if(m_isLoad)return;
		m_isLoad=true;
	TCHAR szPath[MAX_PATH];
	memset(szPath, 0, sizeof(szPath));
	GetModuleFileName(NULL, szPath, MAX_PATH);

	TCHAR	szDrv[_MAX_DRIVE];
	TCHAR	szDir[_MAX_DIR];
	TCHAR	szFileName[_MAX_FNAME];
	ZeroMemory(szDrv, sizeof(szDrv));
	ZeroMemory(szDir, sizeof(szDir));
	ZeroMemory(szFileName, sizeof(szFileName));
	_tsplitpath(szPath, szDrv, szDir, szFileName, NULL);

	_stprintf(m_szLogPath, _T("%s\\%s\\log"), szDrv, szDir);
	_stprintf(m_szAppName, szFileName);

	InitializeCriticalSection(&m_cs);
	
}
string GCHELog::ws2s(const wchar_t* _Source)
    {
        size_t _Dsize = WideCharToMultiByte(CP_ACP, 0, _Source, -1, NULL, 0, NULL, NULL);
        char *_Dest = new char[_Dsize];
        WideCharToMultiByte(CP_ACP, 0, _Source, -1, _Dest, _Dsize, NULL, NULL);
        string result = _Dest;
        delete []_Dest;
        return result;
    }

void GCHELog::RecordData(TCHAR* const pszTitle, TCHAR* lpszFormat, ...)
{
	if(m_isLoad==false)return;
	EnterCriticalSection(&m_cs);
	FILE *fp = 0;
	int err = GetLastError();

	va_list args;
	va_start(args, lpszFormat);
	
	int nBuf=0;
	TCHAR szBuffer[MAX_LINE_LEN*2];
	TCHAR szLogLine[MAX_LINE_LEN*4];

	ZeroMemory(szBuffer, sizeof(szBuffer));
	ZeroMemory(szLogLine, sizeof(szLogLine));

	nBuf = _vsntprintf(szBuffer, MAX_LINE_LEN*2, lpszFormat, args);

	if(szBuffer[nBuf-1] != _T('\n'))
	{
		szBuffer[nBuf] = _T('\n');
		szBuffer[nBuf+1] = _T('\0');
	}
	
	TCHAR szCurrentTime[MAX_DATE_TIME_LEN];
	ZeroMemory(szCurrentTime, sizeof(szCurrentTime));
	GetStrTime(szCurrentTime);

	_stprintf(szLogLine,  _T("[%s][%5d][%5d]:%s->%s"), szCurrentTime, GetCurrentThreadId(), err, pszTitle, szBuffer);
	
	if(m_szAppName[0] != _T('\0'))
	{	
		int nPos = 0;
		if(m_fLog != NULL)
		{
			nPos = ftell(m_fLog);
		}
		if(m_fLog == NULL || m_nDate != GetIntDate() || nPos>= 50000000)
		{			
			CloseLog();
			
			TCHAR szLogPath[MAX_PATH];
			memset(szLogPath, 0, sizeof(szLogPath));
			TCHAR szCurrentTime2[MAX_DATE_TIME_LEN];
			ZeroMemory(szCurrentTime2, sizeof(szCurrentTime2));
			GetStrDateTime2(szCurrentTime2);

			_stprintf(szLogPath, _T("%s\\%s-%s.log"), m_szLogPath, m_szAppName, szCurrentTime2);
									
			fp=_tfopen(szLogPath,_T("w"));

			CIniFile iniFile(GCH_EXCEPTLOG_INI);
			iniFile.SetString(m_szAppName,_T("logname"),szLogPath);
			if(fp != NULL)
			{
				m_fLog = fp;
				m_nDate = GetIntDate();
				fseek(fp, 0, SEEK_END);
			}
		}
		else
		{
			fp = m_fLog;
		}

		if(fp!=NULL) 
		{
			#ifdef UNICODE
				fprintf(fp, ws2s(szLogLine).c_str());
			#else
				_ftprintf(fp, szLogLine);
			#endif		
			fflush(fp);
		}
		else
			_tprintf(szLogLine);
	}
	else
	_tprintf(szLogLine);

	va_end(args);
	SetLastError(err);
	LeaveCriticalSection(&m_cs);
}

void GCHELog::CloseLog()
{
	if(m_fLog != NULL)
		fclose(m_fLog);
	m_fLog = NULL;
}