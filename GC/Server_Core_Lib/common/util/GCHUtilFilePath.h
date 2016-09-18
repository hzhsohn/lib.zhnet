#pragma once
/** @defgroup GroupGCHUtilFilePath GCHUtilFilePath 
 *  <pre><b>Copyright (c) 2006 Game Carrier.  All Rights Reserved</b></pre> 
 *  @author  Zzg
 *  @date    8:5:2007   14:07
 *  @{
 */

/** GetCurrentPath \n
 *  @deprecated ��ȡ��ǰ�������ڵ�·��
 *  @return \n
 *      If the function succeeds, the return value is TCHAR* \n
 *      If the function fails, the return value is NULL. 

 *  @author  Zzg
 *  @date    4:6:2007   15:37
 */
LPTSTR GetCurrentPath();

/** GetCurrentPath \n
 *  @deprecated ��ȡ��ǰ�������ڵ�·��
 *  @param[in/Out] pszCurPath
 *  - : ָ��һ���Ѿ�����õ�TCHAR*ָ�룬����ʱ����ȡ���ص�ǰ·��,��׼��СʱMAX_PATH
 *  @return \n
 *      If the function succeeds, the return value TRUE \n
 *      If the function fails, the return value is FALSE. 

 *  @author  Zzg
 *  @date    4:6:2007   15:37
 */
BOOL GetCurrentPath(TCHAR* pszCurPath);

/** GetFullPathBaseName \n
 *  @deprecated ͨ�����ļ�����·����ȡ�ļ��ĸ�·��
 *  @param[in/Out] pszFullPath
 *  - : ָ��һ���Ѿ�����õ�TCHAR*ָ�룬����ʱ����ȡ���ص�ǰ·��,��׼��СʱMAX_PATH
 *  @author  Zzg
 *  @date    4:6:2007   15:37
 */
BOOL GetFullPathBaseName(TCHAR* pszFullPath);

/** GetFileName \n
 *  @deprecated ͨ��ȫ·����ȡ��·���е��ļ���
 *  @param[in] pszFilePath
 *  - : ָ��һ����Ҫ��ȡ�ļ�����ȫ·��
 *  @param[in/Out] pszFileName
 *  - : ���ػ�ȡ���ļ���
 *  @author  Zzg
 *  @date    4:6:2007   15:37
 */
void GetFileName(const TCHAR* pszFilePath, TCHAR* pszFileName);

/** LoopCreateDir \n
*  @deprecated ѭ������Ŀ¼
*  @param[in] pszFilePath
*  - : ָ��һ����Ҫ��ȡ�ļ�����ȫ·��
*  @param[in/Out] pszFileName
*  - : ���ػ�ȡ���ļ���
*  @author  Zzg
*  @date    4:6:2007   15:37
*/
BOOL LoopCreateDir(LPCTSTR lpPath,TCHAR szSolidus = _T('/'));


/** @}*/ // GCHUtilFilePath

