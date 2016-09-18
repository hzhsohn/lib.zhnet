#include "stdafx.h"
#include <stdio.h>
#include <time.h>

/*
* Copyright (c) 2004, game carrier
* All rights reserved.

* Filename: GCHUtilTime.cpp
* Description:

* Author:bloodspider
* Time:2007-3-14   23:27
* Modify connect:

*/


BOOL GetStrTime(TCHAR *pszCurrentTime)
{
	//12:15:06
	_tstrtime(pszCurrentTime);
	return TRUE;
}

BOOL GetStrDate(TCHAR* pszDateTime)
{
	//1900-01-01
	SYSTEMTIME sm;

	GetLocalTime(&sm);

	_stprintf(pszDateTime,_T("%d-%02d-%02d"),
		sm.wYear, sm.wMonth, sm.wDay);

	return TRUE;
}
int GetIntDate()
{
	//19000101
	SYSTEMTIME sm;

	GetLocalTime(&sm);
	return sm.wYear*10000+sm.wMonth*100+sm.wDay;
}
BOOL GetStrDateTime(TCHAR* pszDateTime)
{
	//1900-01-01 00:00:00
	SYSTEMTIME sm;

	GetLocalTime(&sm);

	_stprintf(pszDateTime,_T("%d-%02d-%02d %02d:%02d:%02d"),
		sm.wYear, sm.wMonth, sm.wDay, sm.wHour, sm.wMinute, sm.wSecond);

	return TRUE;
}

BOOL GetStrDateTime2(TCHAR* pszDateTime)
{
	//19000101-000000
	SYSTEMTIME sm;

	GetLocalTime(&sm);

	_stprintf(pszDateTime,_T("%d%02d%02d-%02d%02d%02d"),
		sm.wYear, sm.wMonth, sm.wDay, sm.wHour, sm.wMinute, sm.wSecond);

	return TRUE;
}
//1900-01-01
long StrDate2Long(TCHAR* pszDate)
{
	struct tm t;
	memset(&t, 0, sizeof(t));
	_stscanf(pszDate,_T("%4d-%2d-%2d"),&t.tm_year,&t.tm_mon,&t.tm_mday);
	t.tm_year -= 1900;    
	t.tm_mon -= 1;   

	long t2 = mktime(&t);
	if(t2 < 0)
		return 0;
	return t2;	
}
//1900-01-01
TCHAR* LongDate2Str(long t, TCHAR* pszDate)
{
	struct  tm *p;	
	p = localtime((time_t*)&t);
	_tcsftime( pszDate, 32,_T("%Y%m%d"), p);
	return pszDate;
}

//1900-01-01 00:00:00
long StrDateTime2Long(TCHAR* pszDateTime)
{
	//1900-01-01 00:00:00
	struct tm t;
	memset(&t, 0, sizeof(t));
	_stscanf(pszDateTime,_T("%4d-%2d-%2d %2d:%2d:%2d"),&t.tm_year,&t.tm_mon,&t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
	t.tm_year -= 1900;    
	t.tm_mon -= 1;   

	long t2 = mktime(&t);
	if(t2 < 0)
		return 0;
	return t2;	
}

//1900-01-01 00:00:00
TCHAR* LongDateTime2Str(long t, TCHAR* pszDateTime)
{
	struct  tm *p;	
	p = localtime((time_t*)&t);
	_tcsftime( pszDateTime, 32, _T("%Y-%m-%d %H:%M:%S"), p);

	return pszDateTime;
}