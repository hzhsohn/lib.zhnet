#pragma once
#include "GCHNetUtil.h"


#define PACKET_NUMBER_CLIENT_DEFAULT	1
#define PACKET_NUMBER_CLIENT_MAX		0x7FFF
#define GC_KEY_CONNECT_TIMEOUT			30//秒  ,超时时间

#pragma pack(1)


#define ZH_NET_FRAME_FLAG	0x0A	//zhNet里面的帧固定值

struct GCSTH_Packet_Frame_Header
{
/*
 *Packet type, EzhPackType structural break
 *
 *包的类型,EzhPackType结构休
*/
	BYTE    yFrameFlag;	//固定 ZH_NET_FRAME_FLAG
	WORD	wDataLen;   //后随的包体的长度
	WORD	wCRC16;     //包体的CRC校检,当包长度为0时CRC校检值为0
};

struct GCSTH_Packet_Frame
{
	GCSTH_Packet_Frame_Header header;
	char	data[PACKET_ONE_LENGTH];
};

#pragma pack()

