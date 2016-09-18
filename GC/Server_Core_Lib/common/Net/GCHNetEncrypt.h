#pragma once
#include "windef.h"
#include "GCHPacket.h"

//加密包
BOOL EncryptPack(BOOL isNeedEncrypt, DWORD dwEncryptKey,char* buf, int len);
//解密包
BOOL DecryptPack(BOOL isNeedEncrypt,DWORD dwEncryptKey,char* buf, int len);

//加密包头
BOOL EncryptPackHead(BOOL isNeedEncrypt, DWORD dwEncryptKey, GCSTH_Packet_Frame_Header *pHeader);
//加密包体
BOOL EncryptPackBody(BOOL isNeedEncrypt, DWORD dwEncryptKey, char* buf, int len);
//解密包头
BOOL DecryptPackHead(BOOL isNeedEncrypt, DWORD dwEncryptKey,GCSTH_Packet_Frame_Header *pHeader);
//解密包体，buf中包括包头部分
BOOL DecryptPackBody(BOOL isNeedEncrypt, DWORD dwEncryptKey,char* buf,int len);

