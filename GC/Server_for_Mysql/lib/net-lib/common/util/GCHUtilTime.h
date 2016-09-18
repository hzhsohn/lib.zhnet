#pragma once

/** @defgroup GroupGCHUtilTime GCHUtilTime
 *  <pre><b>Copyright (c) 2006 Game Carrier  All Rights Reserved</b></pre> 
 *  @author  Zzg
 *  @date    8:5:2007   14:09
 *  @{
 */

#include "tchar.h"


#define MAX_DATE_TIME_LEN		64

/** GetStrTime \n
 *  @author  bloodspider
 *  @date    8:5:2007   14:10
 */
BOOL GetStrTime(TCHAR *pszCurrentTime);
/** GetStrDateTime \n
*  @author  bloodspider
*  @date    8:5:2007   14:10
*/

BOOL GetStrDate(TCHAR* pszDateTime);
int GetIntDate();
BOOL GetStrDateTime(TCHAR* pszDateTime);
BOOL GetStrDateTime2(TCHAR* pszDateTime);
//1900-01-01
long StrDate2Long(TCHAR* pszDate);
//1900-01-01
TCHAR* LongDate2Str(long t, TCHAR* pszDate);
//1900-01-01 00:00:00
long StrDateTime2Long(TCHAR* pszDateTime);
//1900-01-01 00:00:00
char* LongDateTime2Str(long t, TCHAR* pszDateTime);
