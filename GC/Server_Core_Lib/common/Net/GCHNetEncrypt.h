#pragma once
#include "windef.h"
#include "GCHPacket.h"

//���ܰ�
BOOL EncryptPack(BOOL isNeedEncrypt, DWORD dwEncryptKey,char* buf, int len);
//���ܰ�
BOOL DecryptPack(BOOL isNeedEncrypt,DWORD dwEncryptKey,char* buf, int len);

//���ܰ�ͷ
BOOL EncryptPackHead(BOOL isNeedEncrypt, DWORD dwEncryptKey, GCSTH_Packet_Frame_Header *pHeader);
//���ܰ���
BOOL EncryptPackBody(BOOL isNeedEncrypt, DWORD dwEncryptKey, char* buf, int len);
//���ܰ�ͷ
BOOL DecryptPackHead(BOOL isNeedEncrypt, DWORD dwEncryptKey,GCSTH_Packet_Frame_Header *pHeader);
//���ܰ��壬buf�а�����ͷ����
BOOL DecryptPackBody(BOOL isNeedEncrypt, DWORD dwEncryptKey,char* buf,int len);

