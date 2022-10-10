#pragma once
#include <WinSock.h>
#include <wtypes.h>
#include "GCHEncrypt.h"
#include "GCHUtilMarco.h"

/** @defgroup Util util
*  ������
*  @author  bloodspider
*  @date    2007-7-26 17:19
*  @{
*/

/**@defgroup UtilClass class
* ���ݴ�������
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
	/**  ��ǰʹ�õĻ����� */
	char*		m_pBuf;
	/**  ��ǰʹ�õĻ�����ʹ�õ�λ�� */
	size_t		m_stPos;
	/**  �Ƿ������� */
	bool		m_bSubClass;
	/**  �������͵���󳤶� */
	size_t		m_nMaxSize;

	/** ȡ��"short"�͵ı�����ַ \n
	*  @param[in] ntCurAddress ��ǰ��ַ
	*  @return У����ĵ�ַ
	*/
	INT_PTR GetShortAddress(INT_PTR ntCurAddress);
	
	/** ȡ��"WORD"�͵ı�����ַ \n
	*  @param[in] ntCurAddress ��ǰ��ַ
	*  @return У����ĵ�ַ
	*/
	INT_PTR GetWordAddress(INT_PTR ntCurAddress);

	/** ȡ��"DWORD"�͵ı�����ַ \n
	*  @param[in] ntCurAddress ��ǰ��ַ
	*  @return У����ĵ�ַ
	*/
	INT_PTR GetDwordAddress(INT_PTR ntCurAddress);

	/** ȡ��"int"�͵ı�����ַ \n
	*  @param[in] ntCurAddress ��ǰ��ַ
	*  @return У����ĵ�ַ
	*/
	INT_PTR GetintAddress(INT_PTR ntCurAddress);

	/** ȡ��"long"�͵ı�����ַ \n
	*  @param[in] ntCurAddress ��ǰ��ַ
	*  @return У����ĵ�ַ
	*/
	INT_PTR GetLongAddress(INT_PTR ntCurAddress);	

	/** ȡ��"TCHAR"�͵ı�����ַ \n
	*  @param[in] ntCurAddress ��ǰ��ַ
	*  @return У����ĵ�ַ
	*/
	INT_PTR GetTCharAddress(INT_PTR ntCurAddress);	

	/** ȡ��"BYTE"�͵ı�����ַ \n
	*  @param[in] ntCurAddress ��ǰ��ַ
	*  @return У����ĵ�ַ
	*/
	INT_PTR GetByteAddress(INT_PTR ntCurAddress);

	/** ȡ������ĵ�ַ \n
	*  @param[in] ntCurAddress ��ǰ��ַ
	*  @return У����ĵ�ַ
	*/
	INT_PTR GetClassAddress(INT_PTR ntCurAddress);

public:
	/** �򻺳���ѹ��һ��"BYTE" \n
	*  @param[in] byValue ѹ�����
	*  @return
	*/
	void PushByte(BYTE byValue);
	/** �ӻ����е���һ��"BYTE" \n
	*  @return ����һ��"BYTE"
	*/
	BYTE PopByte();

	/** �򻺳���ѹ��һ��"short" \n
	*  @param[in] shValue ѹ�����
	*  @return
	*/
	void PushShort(short shValue);
	/** �ӻ����е���һ��"short" \n
	*  @return ����һ��"short"
	*/
	short PopShort();

	/** �򻺳���ѹ��һ��"int" \n
	*  @param[in] nValue ѹ�����
	*  @return
	*/
	void PushInt(int nValue);
	/** �ӻ����е���һ��"int" \n
	*  @return ����һ��"int"
	*/
	int PopInt();

	/** �򻺳���ѹ��һ��"long" \n
	*  @param[in] lValue ѹ�����
	*  @return
	*/
	void PushLong(long lValue);
	/** �ӻ����е���һ��"long" \n
	*  @return ����һ��"long"
	*/
	long PopLong();

	/** �򻺳���ѹ��һ��"WORD" \n
	*  @param[in] wValue ѹ�����
	*  @return
	*/
	void PushWord(WORD wValue);
	/** �ӻ����е���һ��"WORD" \n
	*  @return ����һ��"WORD"
	*/
	WORD PopWord();

	/** �򻺳���ѹ��һ��"DWORD" \n
	*  @param[in] dwValue ѹ�����
	*  @return
	*/
	void PushDword(DWORD dwValue);
	/** �ӻ����е���һ��"DWORD" \n
	*  @return ����һ��"DWORD"
	*/
	DWORD PopDword();

	/** �򻺳���ѹ��һ��"TCHAR" \n
	*  @param[in] tcValue ѹ�����
	*  @return
	*/
	void PushTChar(TCHAR tcValue);
	/** �ӻ����е���һ��"TCHAR" \n
	*  @return ����һ��"TCHAR"
	*/
	TCHAR PopTChar();

	/** �򻺳���ѹ��һ��"������" \n
	*  @param[in] pBuf ��ѹ�����ݵĻ�������ַ
	*  @param[in] nLen ��ѹ�����ݵ�������
	*  @return
	*/
	void PushBuf(char* pBuf, int nLen);
	/** �ӻ����е���һ��"������" \n
	*  @param[in] pBuf ���������ݵı��滺������ַ
	*  @param[in] nLen ���������ݵĳ���
	*/
	void PopBuf(char* pBuf, int nLen);

	/** �򻺳���ѹ��һ��"TCHAR���ַ���" \n
	*  @param[in] pszBuf �ַ�����ַ
	*  @return
	*/
	void PushString(TCHAR* pszBuf, int len);
	/** �ӻ����е���һ��"TCHAR���ַ���" \n
	*  @param[in] pszBuf �ַ�����ַ
	*  @return
	*/
	void PopString(TCHAR* pszBuf, int len);

	/** ��� \n
	*  @param[in] pLocalStruct ��������ݽṹ��ַ
	*  @param[in] stLocalStructLen ��������ݽṹ����
	*  @param[in] pNetPacket ���������ݴ洢��ַ
	*  @param[in] stNetPacketLen �洢���ݵĻ���������
	*  @param[in] pszStructDesc �ṹ������
	*  @return ����������������
	*/
	int Pack(char* pLocalStruct, size_t stLocalStructLen, char* pNetPacket, size_t stNetPacketLen, char* pszStructDesc);

	/** ��� \n
	*  @param[in] pLocalStruct �������ݵĽṹ��ַ
	*  @param[in] stLocalStructLen �������ݵĽṹ����
	*  @param[in] pNetPacket ���ʱ�����ݴ洢��ַ
	*  @param[in] stNetPacketLen ���ʱ�����ݵĻ���������
	*  @param[in] pszStructDesc �ṹ������
	*  @return ����������������
	*/
	int UnPack(char* pLocalStruct, size_t stLocalStructLen, char* pNetPacket, size_t stNetPacketLen, char* pszStructDesc);
};

/** @}*/ // class

/** @}*/ // util
