#pragma once
#include "GCHNetUtil.h"


#define PACKET_NUMBER_CLIENT_DEFAULT	1
#define PACKET_NUMBER_CLIENT_MAX		0x7FFF
#define GC_KEY_CONNECT_TIMEOUT			30//��  ,��ʱʱ��

#pragma pack(1)


#define ZH_NET_FRAME_FLAG	0x0A	//zhNet�����֡�̶�ֵ

struct GCSTH_Packet_Frame_Header
{
/*
 *Packet type, EzhPackType structural break
 *
 *��������,EzhPackType�ṹ��
*/
	BYTE    yFrameFlag;	//�̶� ZH_NET_FRAME_FLAG
	WORD	wDataLen;   //����İ���ĳ���
	WORD	wCRC16;     //�����CRCУ��,��������Ϊ0ʱCRCУ��ֵΪ0
};

struct GCSTH_Packet_Frame
{
	GCSTH_Packet_Frame_Header header;
	char	data[PACKET_ONE_LENGTH];
};

#pragma pack()

