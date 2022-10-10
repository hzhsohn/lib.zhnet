#include "stdafx.h"
#include "GCHUtilFilePath.h"
#include "stdlib.h"
#include <stdio.h>
LPTSTR GetCurrentPath()
{
	static TCHAR	path[_MAX_PATH];
	TCHAR	drv[_MAX_DRIVE];
	TCHAR	dir[_MAX_DIR];
	//path[0] = 0;
	memset(path,0,sizeof(path));
	memset(drv,0,sizeof(drv));
	memset(dir,0,sizeof(dir));

	GetModuleFileName(NULL, path, _MAX_PATH);
	_tsplitpath(path, drv, dir, NULL, NULL);
	_tmakepath(path, drv, dir, NULL,NULL);

	return path;
}

BOOL GetCurrentPath(TCHAR* pszCurPath)
{
	if(pszCurPath == NULL)
		return FALSE;
	TCHAR	drv[_MAX_DRIVE];
	TCHAR	dir[_MAX_DIR];
	//path[0] = 0;
	memset(drv,0,sizeof(drv));
	memset(dir,0,sizeof(dir));

	GetModuleFileName(NULL, pszCurPath, _MAX_PATH);
	_tsplitpath(pszCurPath, drv, dir, NULL, NULL);
	_tmakepath(pszCurPath, drv, dir, NULL,NULL);
	return TRUE;

}
BOOL GetFullPathBaseName(TCHAR* pszFullPath)
{
	TCHAR	szDrv[_MAX_DRIVE];
	TCHAR	szDir[_MAX_DIR];
	TCHAR	szFileName[_MAX_FNAME];
	ZeroMemory(szDrv, sizeof(szDrv));
	ZeroMemory(szDir, sizeof(szDir));
	ZeroMemory(szFileName, sizeof(szFileName));
	_tsplitpath(pszFullPath, szDrv, szDir, szFileName, NULL);
	_tcscpy(pszFullPath, szFileName);
	return TRUE;
}
void GetFileName(const TCHAR* pszFilePath, TCHAR* pszFileName)
{
	TCHAR	szDrv[_MAX_DRIVE];
	TCHAR	szDir[_MAX_DIR];
	TCHAR	szFileName[_MAX_FNAME];
	TCHAR	szExt[_MAX_EXT];
	ZeroMemory(szDrv, sizeof(szDrv));
	ZeroMemory(szDir, sizeof(szDir));
	ZeroMemory(szFileName, sizeof(szFileName));
	ZeroMemory(szExt, sizeof(szExt));
	_tsplitpath(pszFilePath, szDrv, szDir, szFileName, szExt);
	_stprintf(pszFileName, _T("%s%s"), szFileName, szExt);

}
BOOL LoopCreateDir(LPCTSTR lpPath,TCHAR szSolidus)
{
	TCHAR szDstDir[MAX_PATH];
	DWORD  dwFlag = GetFileAttributes(lpPath);
	INT_PTR len;

	if ( INVALID_FILE_ATTRIBUTES == dwFlag ){
		//目录不存在创建目录
		if(!CreateDirectory(lpPath, NULL)){
			int err = GetLastError();
			if(err == ERROR_PATH_NOT_FOUND){
				ZeroMemory(szDstDir, sizeof(szDstDir));
				len =  (INT_PTR)_tcsrchr(lpPath,szSolidus) - (INT_PTR)lpPath;
				memcpy(szDstDir, lpPath, len);
				if(LoopCreateDir(szDstDir)){
					if(!CreateDirectory(lpPath, NULL)){
						return FALSE;
					}
				}else{
					return FALSE;
				}
			}else{
				return FALSE;
			}
		}
	}
	return TRUE;
}
