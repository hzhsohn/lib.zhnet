// ProfileIni.h: interface for the CProfileIni class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CIniFile  
{
public:
	CIniFile();
	CIniFile(LPCTSTR strFile);
	virtual ~CIniFile();

    TCHAR m_strIniFile[MAX_PATH];
    BOOL SetFile(LPCTSTR strFile);
  	void operator =(const CIniFile obj)
	{
		ZeroMemory(m_strIniFile,sizeof(m_strIniFile));
		_tcscpy(m_strIniFile,obj.m_strIniFile);
	};
    void GetExeCurrentPath(LPTSTR strFile);

	int SetInt(LPCTSTR strSectionName, LPCTSTR strKeyName, int nValue);
    int GetInt(LPCTSTR strSectionName, LPCTSTR strKeyName);
	int SetString(LPCTSTR strSectionName, LPCTSTR strKeyName, LPCTSTR strBuffer);
	int GetString(LPCTSTR strSectionName, LPCTSTR strKeyName, LPTSTR strBuffer,int nLen);
    int GetBinary(LPCTSTR strSectionName, LPCTSTR strKeyName, void *pBuffer,int nLen);
    int SetBinary(LPCTSTR strSectionName, LPCTSTR strKeyName, void *pBuffer,int nLen);
	BOOL AddSection(LPCTSTR strSectionName,LPCTSTR strBuffer);

};
