#pragma once
#include <WinSock.h>
#include <wtypes.h>
#include "GCHEncrypt.h"
#include "GCHUtilMarco.h"

/** @defgroup Util util
*  基本类
*  @author  bloodspider
*  @date    2007-7-26 17:19
*  @{
*/

/**@defgroup UtilClass class
* 数据打包解包类
* @author  bloodspider
* @date    12:7:2007   15:33
* @{
*/
class DLLEXPORT_CLASS CGCHPack
{
public:
	CGCHPack(void);
	CGCHPack(char* pBuf);
	~CGCHPack(void);

private:
	DWORD		m_dwEncryptKey;
	/**  当前使用的缓冲区 */
	char*		m_pBuf;
	/**  当前使用的缓冲区使用的位置 */
	size_t		m_stPos;
	/**  是否是子类 */
	bool		m_bSubClass;
	/**  基本类型的最大长度 */
	size_t		m_nMaxSize;

	/** 取得"short"型的变量地址 \n
	*  @param[in] ntCurAddress 当前地址
	*  @return 校正后的地址
	*/
	INT_PTR GetShortAddress(INT_PTR ntCurAddress);
	
	/** 取得"WORD"型的变量地址 \n
	*  @param[in] ntCurAddress 当前地址
	*  @return 校正后的地址
	*/
	INT_PTR GetWordAddress(INT_PTR ntCurAddress);

	/** 取得"DWORD"型的变量地址 \n
	*  @param[in] ntCurAddress 当前地址
	*  @return 校正后的地址
	*/
	INT_PTR GetDwordAddress(INT_PTR ntCurAddress);

	/** 取得"int"型的变量地址 \n
	*  @param[in] ntCurAddress 当前地址
	*  @return 校正后的地址
	*/
	INT_PTR GetintAddress(INT_PTR ntCurAddress);

	/** 取得"long"型的变量地址 \n
	*  @param[in] ntCurAddress 当前地址
	*  @return 校正后的地址
	*/
	INT_PTR GetLongAddress(INT_PTR ntCurAddress);	

	/** 取得"TCHAR"型的变量地址 \n
	*  @param[in] ntCurAddress 当前地址
	*  @return 校正后的地址
	*/
	INT_PTR GetTCharAddress(INT_PTR ntCurAddress);	

	/** 取得"BYTE"型的变量地址 \n
	*  @param[in] ntCurAddress 当前地址
	*  @return 校正后的地址
	*/
	INT_PTR GetByteAddress(INT_PTR ntCurAddress);

	/** 取得子类的地址 \n
	*  @param[in] ntCurAddress 当前地址
	*  @return 校正后的地址
	*/
	INT_PTR GetClassAddress(INT_PTR ntCurAddress);

public:
	/** 向缓冲中压入一个"BYTE" \n
	*  @param[in] byValue 压入变量
	*  @return
	*/
	void PushByte(BYTE byValue);
	/** 从缓冲中弹出一个"BYTE" \n
	*  @return 返回一个"BYTE"
	*/
	BYTE PopByte();

	/** 向缓冲中压入一个"short" \n
	*  @param[in] shValue 压入变量
	*  @return
	*/
	void PushShort(short shValue);
	/** 从缓冲中弹出一个"short" \n
	*  @return 返回一个"short"
	*/
	short PopShort();

	/** 向缓冲中压入一个"int" \n
	*  @param[in] nValue 压入变量
	*  @return
	*/
	void PushInt(int nValue);
	/** 从缓冲中弹出一个"int" \n
	*  @return 返回一个"int"
	*/
	int PopInt();

	/** 向缓冲中压入一个"long" \n
	*  @param[in] lValue 压入变量
	*  @return
	*/
	void PushLong(long lValue);
	/** 从缓冲中弹出一个"long" \n
	*  @return 返回一个"long"
	*/
	long PopLong();

	/** 向缓冲中压入一个"WORD" \n
	*  @param[in] wValue 压入变量
	*  @return
	*/
	void PushWord(WORD wValue);
	/** 从缓冲中弹出一个"WORD" \n
	*  @return 返回一个"WORD"
	*/
	WORD PopWord();

	/** 向缓冲中压入一个"DWORD" \n
	*  @param[in] dwValue 压入变量
	*  @return
	*/
	void PushDword(DWORD dwValue);
	/** 从缓冲中弹出一个"DWORD" \n
	*  @return 返回一个"DWORD"
	*/
	DWORD PopDword();

	/** 向缓冲中压入一个"TCHAR" \n
	*  @param[in] tcValue 压入变量
	*  @return
	*/
	void PushTChar(TCHAR tcValue);
	/** 从缓冲中弹出一个"TCHAR" \n
	*  @return 返回一个"TCHAR"
	*/
	TCHAR PopTChar();

	/** 向缓冲中压入一块"数据区" \n
	*  @param[in] pBuf 被压入数据的缓冲区地址
	*  @param[in] nLen 被压入数据的区长度
	*  @return
	*/
	void PushBuf(char* pBuf, int nLen);
	/** 从缓冲中弹出一个"数据区" \n
	*  @param[in] pBuf 被弹出数据的保存缓冲区地址
	*  @param[in] nLen 被弹出数据的长度
	*/
	void PopBuf(char* pBuf, int nLen);

	/** 向缓冲中压入一个"TCHAR型字符串" \n
	*  @param[in] pszBuf 字符串地址
	*  @return
	*/
	void PushString(TCHAR* pszBuf, int len);
	/** 从缓冲中弹出一个"TCHAR型字符串" \n
	*  @param[in] pszBuf 字符串地址
	*  @return
	*/
	void PopString(TCHAR* pszBuf, int len);

	/** 打包 \n
	*  @param[in] pLocalStruct 被打包数据结构地址
	*  @param[in] stLocalStructLen 被打包数据结构长度
	*  @param[in] pNetPacket 打包后的数据存储地址
	*  @param[in] stNetPacketLen 存储数据的缓冲区长度
	*  @param[in] pszStructDesc 结构描述串
	*  @return 打包后的数据区长度
	*/
	int Pack(char* pLocalStruct, size_t stLocalStructLen, char* pNetPacket, size_t stNetPacketLen, char* pszStructDesc);

	/** 解包 \n
	*  @param[in] pLocalStruct 保存数据的结构地址
	*  @param[in] stLocalStructLen 保存数据的结构长度
	*  @param[in] pNetPacket 解包时的数据存储地址
	*  @param[in] stNetPacketLen 解包时的数据的缓冲区长度
	*  @param[in] pszStructDesc 结构描述串
	*  @return 解包后的数据区长度
	*/
	int UnPack(char* pLocalStruct, size_t stLocalStructLen, char* pNetPacket, size_t stNetPacketLen, char* pszStructDesc);
};

/** @}*/ // class

/** @}*/ // util
