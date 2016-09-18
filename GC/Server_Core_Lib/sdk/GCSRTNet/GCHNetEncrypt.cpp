#include "stdafx.h"
#include "GCHNetEncrypt.h"

BOOL EncryptPack(BOOL isNeedEncrypt, DWORD dwEncryptKey,char* buf, int len)
{
	GCSTH_Packet_Frame* pPacket = (GCSTH_Packet_Frame*)buf;
	EncryptPackHead(isNeedEncrypt, dwEncryptKey, &pPacket->header);
	EncryptPackBody(isNeedEncrypt, dwEncryptKey, pPacket->data, len-sizeof(GCSTH_Packet_Frame_Header));
	return TRUE;
}

BOOL EncryptPackHead(BOOL isNeedEncrypt, DWORD dwEncryptKey, GCSTH_Packet_Frame_Header *pHeader)
{
	if(pHeader)
	{
		//���ܻ�����
		GCApiEncrypt(isNeedEncrypt, (char*)pHeader, sizeof(GCSTH_Packet_Frame_Header), dwEncryptKey);
		return TRUE;
	}
	return FALSE;

}
BOOL EncryptPackBody(BOOL isNeedEncrypt, DWORD dwEncryptKey, char* buf, int len)
{
	if(len> 0)
	{
		//��Ҫ�����
		GCApiEncrypt(isNeedEncrypt, buf, len, dwEncryptKey);
		return TRUE;
	}
	return FALSE;
}
BOOL DecryptPackHead(BOOL isNeedEncrypt, DWORD dwEncryptKey,GCSTH_Packet_Frame_Header *pHeader)
{
	if(pHeader)
	{
		//���ܰ�ͷ
		GCApiDecrypt(isNeedEncrypt, (char*)pHeader, sizeof(GCSTH_Packet_Frame_Header), dwEncryptKey);
		return TRUE;
	}
	return FALSE;
}
BOOL DecryptPackBody(BOOL isNeedEncrypt, DWORD dwEncryptKey,char* buf,int len)
{
	if(len> 0)
	{
		//��Ҫ�����
		GCApiDecrypt(isNeedEncrypt, buf,len,dwEncryptKey);
		return TRUE;
	}
	return FALSE;
}
