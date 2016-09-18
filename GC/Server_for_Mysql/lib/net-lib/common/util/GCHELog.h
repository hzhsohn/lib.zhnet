#pragma once

/*
* Copyright (c) 2004, game carrier
* All rights reserved.

* Filename: GCHELog.h

* Author:bloodspider
* Time:2007-3-14   0:19
* Modify connect:

*/

#include "stdlib.h"
#include "stdio.h"
#include "tchar.h"

#include "GCHUtilMarco.h"
#include "GCHUtilTime.h"
#include <iostream>
using namespace std;

#define MAX_LOG_FILE_CNT		10		//��һ������ģ����������д��LOG�ļ���
#define MAX_LINE_LEN			1024	//LOG�ļ���һ�е�����ַ���

//����ֻ��һЩͨ�õ�TRACE�궨�壬�Ծɿ��Զ����µĺ꣬���벻Ҫ���������ӣ��붨����ÿ�����̵�stdafx.h�ļ��С�

//Log��ʼ����
#define GCH_INIT_LOG GCHELog::InitLogFile
//��װLOG
#define GCH_CLOSE_LOG GCHELog::CloseLog

#define GCH_TRACE	GCHELog::RecordData
#define GCH_VTRACE(s,...)	GCH_TRACE(_T("[�������]"),s,__VA_ARGS__)//�������
#define GCH_KTRACE(s,...)	//GCH_TRACE(_T("[�߼�����]"),s,__VA_ARGS__)//�߼�����
#define GCH_ETRACE(s,...)	//GCH_TRACE(_T("[�������]"),s,__VA_ARGS__)//�������
#define GCH_XTRACE(s,...)	GCH_TRACE(_T("[��Ϣ���]"),s,__VA_ARGS__)//�������
#define GCH_MTRACE(s,...)	GCH_TRACE(_T("[ǿ�����]"),s,__VA_ARGS__)//ǿ�����
#define GCH_WORKLOG(s,...)  GCH_TRACE(_T("[��������]"),s,__VA_ARGS__)

class DLLEXPORT_CLASS  GCHELog
{
private:
	
	static char m_szAppName[_MAX_PATH];
	static char m_szLogPath[_MAX_PATH];
	static FILE* m_fLog;
	static int	m_nDate;
	static bool m_isLoad;
	static CRITICAL_SECTION		m_cs;
public:
	GCHELog(void);
	~GCHELog(void);
	static void InitLogFile();
	static void RecordData(char* const pszTitle, char* lpszFormat, ...);
	static string ws2s(const wchar_t* _Source);
	static void CloseLog();
};
