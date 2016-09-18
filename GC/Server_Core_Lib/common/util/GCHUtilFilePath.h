#pragma once
/** @defgroup GroupGCHUtilFilePath GCHUtilFilePath 
 *  <pre><b>Copyright (c) 2006 Game Carrier.  All Rights Reserved</b></pre> 
 *  @author  Zzg
 *  @date    8:5:2007   14:07
 *  @{
 */

/** GetCurrentPath \n
 *  @deprecated 获取当前程序所在的路径
 *  @return \n
 *      If the function succeeds, the return value is TCHAR* \n
 *      If the function fails, the return value is NULL. 

 *  @author  Zzg
 *  @date    4:6:2007   15:37
 */
LPTSTR GetCurrentPath();

/** GetCurrentPath \n
 *  @deprecated 获取当前程序所在的路径
 *  @param[in/Out] pszCurPath
 *  - : 指向一个已经分配好的TCHAR*指针，返回时保存取到地当前路径,标准大小时MAX_PATH
 *  @return \n
 *      If the function succeeds, the return value TRUE \n
 *      If the function fails, the return value is FALSE. 

 *  @author  Zzg
 *  @date    4:6:2007   15:37
 */
BOOL GetCurrentPath(TCHAR* pszCurPath);

/** GetFullPathBaseName \n
 *  @deprecated 通过带文件名的路径获取文件的根路径
 *  @param[in/Out] pszFullPath
 *  - : 指向一个已经分配好的TCHAR*指针，返回时保存取到地当前路径,标准大小时MAX_PATH
 *  @author  Zzg
 *  @date    4:6:2007   15:37
 */
BOOL GetFullPathBaseName(TCHAR* pszFullPath);

/** GetFileName \n
 *  @deprecated 通过全路径获取该路径中的文件名
 *  @param[in] pszFilePath
 *  - : 指向一个需要获取文件名的全路径
 *  @param[in/Out] pszFileName
 *  - : 返回获取的文件名
 *  @author  Zzg
 *  @date    4:6:2007   15:37
 */
void GetFileName(const TCHAR* pszFilePath, TCHAR* pszFileName);

/** LoopCreateDir \n
*  @deprecated 循环创建目录
*  @param[in] pszFilePath
*  - : 指向一个需要获取文件名的全路径
*  @param[in/Out] pszFileName
*  - : 返回获取的文件名
*  @author  Zzg
*  @date    4:6:2007   15:37
*/
BOOL LoopCreateDir(LPCTSTR lpPath,TCHAR szSolidus = _T('/'));


/** @}*/ // GCHUtilFilePath

