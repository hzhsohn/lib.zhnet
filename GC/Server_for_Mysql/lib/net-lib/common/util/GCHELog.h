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

#define MAX_LOG_FILE_CNT		10		//在一个程序模块中最多可以写的LOG文件数
#define MAX_LINE_LEN			1024	//LOG文件中一行的最大字符数

//这里只是一些通用的TRACE宏定义，仍旧可以定义新的宏，但请不要往这里增加，请定义在每个工程的stdafx.h文件中、

//Log初始化宏
#define GCH_INIT_LOG GCHELog::InitLogFile
//关装LOG
#define GCH_CLOSE_LOG GCHELog::CloseLog

#define GCH_TRACE	GCHELog::RecordData
#define GCH_VTRACE(s,...)	GCH_TRACE(_T("[输出变量]"),s,__VA_ARGS__)//输出变量
#define GCH_KTRACE(s,...)	//GCH_TRACE(_T("[逻辑跟踪]"),s,__VA_ARGS__)//逻辑跟踪
#define GCH_ETRACE(s,...)	//GCH_TRACE(_T("[错误输出]"),s,__VA_ARGS__)//错误输出
#define GCH_XTRACE(s,...)	GCH_TRACE(_T("[消息输出]"),s,__VA_ARGS__)//错误输出
#define GCH_MTRACE(s,...)	GCH_TRACE(_T("[强制输出]"),s,__VA_ARGS__)//强制输出
#define GCH_WORKLOG(s,...)  GCH_TRACE(_T("[操作跟踪]"),s,__VA_ARGS__)

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
