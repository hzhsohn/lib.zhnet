#include <string>
using namespace std;
/** @defgroup GroupGCHUtilCString GCHUtilCString
*	Copyright (c) 2004, game carrier All rights reserved.
*  @date    8:5:2007   13:48
*  @{
*/
#pragma once
/** GCConvertStringA2TEx \n
*  转换ANSI字符串到宽字符串
*  @param[in] pStrIn
*  - char* 以空为终止符的字符串
*  @param[Out] pStrOut
*  - char* 以空为终止符指针，保存转换后的数据
*  @param[In/Out] nOutBufLen
*  - int& pStrOut buf的大小和转换后buf的大小

*  @return \n
*      If the function succeeds, the return value is char* \n
*      If the function fails, the return value is NULL. 

*  @author  Zzg
*  @date    30:5:2007   15:38

*  @see ConvertStringT2A
*/
BOOL GCConvertStringA2TEx(char* pStrIn,char* pStrOut,int& nOutBufLen);


/** GCConvertStringT2A \n
*  转换宽字符串串到ANSI字符
*  @param[in] pStrIn
*  - char* 以空为终止符的字符串，需要转换的
*  @param[Out] pStrOut
*  - char* 以空为终止符指针，保存转换后的数据
*  @param[In/Out] nOutBufLen
*  - int& pStrOut buf的大小和转换后buf的大小

*  @return \n
*      If the function succeeds, the return value is char* \n
*      If the function fails, the return value is NULL. 

*  @author  Zzg
*  @date    30:5:2007   15:38

*  @see ConvertStringT2A
*  @note 使用这个字符创的时候必须注意，必须把保存的内容存到一个Buf中\n
，因为返回的指针是一个静态的指针
*/
BOOL GCConvertStringT2AEx(char* pStrIn,char* pStrOut,int& nOutBufLen);

/** ConvertStringA2T \n
*  转换ANSI字符串到宽字符串
*  @param[in] str
*  - char* 以空为终止符的字符串

*  @return \n
*      If the function succeeds, the return value is char* \n
*      If the function fails, the return value is NULL. 

*  @author  Zzg
*  @date    30:5:2007   15:38

*  @see ConvertStringT2A
*/
wstring s2ws( char* _Source);
char* GCConvertStringA2T(char* str);


/** GCConvertStringT2A \n
*  转换宽字符串串到ANSI字符
*  @param[in] str
*  - char* 以空为终止符的字符串

*  @return \n
*      If the function succeeds, the return value is char* \n
*      If the function fails, the return value is NULL. 

*  @author  Zzg
*  @date    30:5:2007   15:38

*  @see ConvertStringT2A
*  @note 使用这个字符创的时候必须注意，必须把保存的内容存到一个Buf中\n
，因为返回的指针是一个静态的指针
*/
char* GCConvertStringT2A(char* str);

/** GCConvertBSTR2StringT \n
*  转换BSTR字符串到宽字符串
*  @param[in] str
*  - BSTR 以空为终止符的字符串

*  @return \n
*      If the function succeeds, the return value is char* \n
*      If the function fails, the return value is NULL. 

*  @author  Zzg
*  @date    30:5:2007   15:38

*  @see ConvertStringT2A
*  @note 使用这个字符创的时候必须注意，必须把保存的内容存到一个Buf中\n
，因为返回的指针是一个静态的指针
*/
//char* GCConvertBSTR2StringT(BSTR bstrText);

/** GCConvertStringT2BSTR \n
*  转换宽字符串到BSTR字符串
*  @param[in] str
*  - char* 以空为终止符的字符串

*  @return \n
*      If the function succeeds, the return value is char* \n
*      If the function fails, the return value is NULL. 

*  @author  Zzg
*  @date    30:5:2007   15:38

*  @see ConvertStringT2A
*/
//BSTR GCConvertStringT2BSTR(char* strt);
int GetStrByte(char *str);

/** @}*/ // GCHUtilCString