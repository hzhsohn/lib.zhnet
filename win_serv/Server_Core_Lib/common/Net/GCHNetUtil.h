#pragma once

#define PACKET_ONE_LENGTH			1048						//ÿ�η�������󳤶ȣ���������ͷ
#define PACKET_ONE_HEADER_LENGTH	1056						//ÿ�η�������󳤶ȣ�������ͷ

typedef void(WINAPI* PFUN_ACCEPT_CALLBACK)(HANDLE handle, char* pszIP, WORD wPort);
typedef void (WINAPI* PFUN_RECV_DATA_CALLBACK)(HANDLE handle, int nLen, char* pData);
typedef void (WINAPI* PFUN_DISCONNECT_CALLBACK)(HANDLE handle);