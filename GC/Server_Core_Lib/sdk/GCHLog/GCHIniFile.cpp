// ProfileIni.cpp: implementation of the CIniFile class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GCHIniFile.h"
#include <shlwapi.h>
#include "stdio.h"
#pragma comment(lib, "shlwapi.lib")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIniFile::CIniFile(LPCTSTR strFile) // example: CIniFile ini("user//set.ini");
{
	TCHAR Path[MAX_PATH];
	ZeroMemory(Path,sizeof(Path));
	ZeroMemory(m_strIniFile,sizeof(m_strIniFile));

	GetExeCurrentPath(Path);
	PathAppend(Path,strFile);
	_tcscat(m_strIniFile,Path);
}

CIniFile::CIniFile()
{
	ZeroMemory(m_strIniFile,sizeof(m_strIniFile));


}

CIniFile::~CIniFile()
{
}
void CIniFile::GetExeCurrentPath(LPTSTR strFile)
{
		TCHAR	drv[_MAX_DRIVE];
	TCHAR	dir[_MAX_DIR];

	GetModuleFileName(NULL, strFile, _MAX_PATH);
	_tsplitpath(strFile, drv, dir, NULL, NULL);
	_tmakepath(strFile, drv, dir, NULL,NULL);

}
BOOL CIniFile::SetFile(LPCTSTR strFile)
{
	TCHAR Path[MAX_PATH];
	ZeroMemory(Path,sizeof(Path));

	GetExeCurrentPath(Path);
	PathAppend(Path,strFile);
	ZeroMemory(m_strIniFile,sizeof(m_strIniFile));
	_tcscpy(m_strIniFile,Path);
	DWORD  dwFlag = GetFileAttributes(m_strIniFile);

	if(INVALID_FILE_ATTRIBUTES == dwFlag)
	{
		ZeroMemory(m_strIniFile,sizeof(m_strIniFile));
		return FALSE;

	}

	return TRUE;;
    
}

int CIniFile::GetInt(LPCTSTR strSectionName, LPCTSTR strKeyName)
{
    return GetPrivateProfileInt(strSectionName, strKeyName, 0, m_strIniFile);
}

int CIniFile::SetInt(LPCTSTR strSectionName, LPCTSTR strKeyName, int nValue)
{
    TCHAR szInt[10];

	_stprintf(szInt, _T("%d"), nValue);
    return WritePrivateProfileString(strSectionName, strKeyName, szInt, m_strIniFile);
}


int CIniFile::GetString(LPCTSTR strSectionName, LPCTSTR strKeyName, LPTSTR strBuffer,int nLen)
{
    TCHAR *pszBuffer = new TCHAR[nLen];
    
    int n = GetPrivateProfileString(strSectionName, strKeyName, NULL, pszBuffer, nLen, m_strIniFile);

	if(n<=0)
		_tcscpy(strBuffer,_T(""));
	else
		_tcscpy(strBuffer,pszBuffer);

    delete []pszBuffer;
    return n;
}

int CIniFile::SetString(LPCTSTR strSectionName, LPCTSTR strKeyName, LPCTSTR strBuffer)
{
    return WritePrivateProfileString(strSectionName, strKeyName, strBuffer,m_strIniFile);
}

int CIniFile::GetBinary(LPCTSTR strSectionName, LPCTSTR strKeyName, void *pBuffer,int nLen)
{
    int n = GetPrivateProfileStruct(strSectionName, strKeyName, pBuffer, nLen, m_strIniFile);
    return n;
}

int CIniFile::SetBinary(LPCTSTR strSectionName, LPCTSTR strKeyName, void *pBuffer,int nLen)
{
    return WritePrivateProfileStruct(strSectionName, strKeyName, pBuffer, nLen, m_strIniFile);
}
BOOL CIniFile::AddSection(LPCTSTR strSectionName,LPCTSTR strBuffer)
{
	return WritePrivateProfileSection(strSectionName,strBuffer,m_strIniFile);
}
