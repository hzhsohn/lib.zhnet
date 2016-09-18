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
		//加密缓冲区
		GCApiEncrypt(isNeedEncrypt, (char*)pHeader, sizeof(GCSTH_Packet_Frame_Header), dwEncryptKey);
		return TRUE;
	}
	return FALSE;

}
BOOL EncryptPackBody(BOOL isNeedEncrypt, DWORD dwEncryptKey, char* buf, int len)
{
	if(len> 0)
	{
		//需要解包体
		GCApiEncrypt(isNeedEncrypt, buf, len, dwEncryptKey);
		return TRUE;
	}
	return FALSE;
}
BOOL DecryptPackHead(BOOL isNeedEncrypt, DWORD dwEncryptKey,GCSTH_Packet_Frame_Header *pHeader)
{
	if(pHeader)
	{
		//解密包头
		GCApiDecrypt(isNeedEncrypt, (char*)pHeader, sizeof(GCSTH_Packet_Frame_Header), dwEncryptKey);
		return TRUE;
	}
	return FALSE;
}
BOOL DecryptPackBody(BOOL isNeedEncrypt, DWORD dwEncryptKey,char* buf,int len)
{
	if(len> 0)
	{
		//需要解包体
		GCApiDecrypt(isNeedEncrypt, buf,len,dwEncryptKey);
		return TRUE;
	}
	return FALSE;
}
