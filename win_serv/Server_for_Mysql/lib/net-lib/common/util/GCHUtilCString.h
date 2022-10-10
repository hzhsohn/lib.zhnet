#include <string>
using namespace std;
/** @defgroup GroupGCHUtilCString GCHUtilCString
*	Copyright (c) 2004, game carrier All rights reserved.
*  @date    8:5:2007   13:48
*  @{
*/
#pragma once
/** GCConvertStringA2TEx \n
*  ת��ANSI�ַ��������ַ���
*  @param[in] pStrIn
*  - char* �Կ�Ϊ��ֹ�����ַ���
*  @param[Out] pStrOut
*  - char* �Կ�Ϊ��ֹ��ָ�룬����ת���������
*  @param[In/Out] nOutBufLen
*  - int& pStrOut buf�Ĵ�С��ת����buf�Ĵ�С

*  @return \n
*      If the function succeeds, the return value is char* \n
*      If the function fails, the return value is NULL. 

*  @author  Zzg
*  @date    30:5:2007   15:38

*  @see ConvertStringT2A
*/
BOOL GCConvertStringA2TEx(char* pStrIn,char* pStrOut,int& nOutBufLen);


/** GCConvertStringT2A \n
*  ת�����ַ�������ANSI�ַ�
*  @param[in] pStrIn
*  - char* �Կ�Ϊ��ֹ�����ַ�������Ҫת����
*  @param[Out] pStrOut
*  - char* �Կ�Ϊ��ֹ��ָ�룬����ת���������
*  @param[In/Out] nOutBufLen
*  - int& pStrOut buf�Ĵ�С��ת����buf�Ĵ�С

*  @return \n
*      If the function succeeds, the return value is char* \n
*      If the function fails, the return value is NULL. 

*  @author  Zzg
*  @date    30:5:2007   15:38

*  @see ConvertStringT2A
*  @note ʹ������ַ�����ʱ�����ע�⣬����ѱ�������ݴ浽һ��Buf��\n
����Ϊ���ص�ָ����һ����̬��ָ��
*/
BOOL GCConvertStringT2AEx(char* pStrIn,char* pStrOut,int& nOutBufLen);

/** ConvertStringA2T \n
*  ת��ANSI�ַ��������ַ���
*  @param[in] str
*  - char* �Կ�Ϊ��ֹ�����ַ���

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
*  ת�����ַ�������ANSI�ַ�
*  @param[in] str
*  - char* �Կ�Ϊ��ֹ�����ַ���

*  @return \n
*      If the function succeeds, the return value is char* \n
*      If the function fails, the return value is NULL. 

*  @author  Zzg
*  @date    30:5:2007   15:38

*  @see ConvertStringT2A
*  @note ʹ������ַ�����ʱ�����ע�⣬����ѱ�������ݴ浽һ��Buf��\n
����Ϊ���ص�ָ����һ����̬��ָ��
*/
char* GCConvertStringT2A(char* str);

/** GCConvertBSTR2StringT \n
*  ת��BSTR�ַ��������ַ���
*  @param[in] str
*  - BSTR �Կ�Ϊ��ֹ�����ַ���

*  @return \n
*      If the function succeeds, the return value is char* \n
*      If the function fails, the return value is NULL. 

*  @author  Zzg
*  @date    30:5:2007   15:38

*  @see ConvertStringT2A
*  @note ʹ������ַ�����ʱ�����ע�⣬����ѱ�������ݴ浽һ��Buf��\n
����Ϊ���ص�ָ����һ����̬��ָ��
*/
//char* GCConvertBSTR2StringT(BSTR bstrText);

/** GCConvertStringT2BSTR \n
*  ת�����ַ�����BSTR�ַ���
*  @param[in] str
*  - char* �Կ�Ϊ��ֹ�����ַ���

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