// GCHEncrypt.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "stdlib.h"
#include "time.h"
#include "stdio.h"

#define GCHMAC_KEY_CHANGE		37

int		g_nKeyCount				= 100;

 
BOOL Encrypt1(char* buf, int len, int nEncryptKey);
BOOL Decrypt1(char* buf, int len, int nEncryptKey);

//初始化未获取钥匙前默认秘密
DLLEXPORT_MULT int GCApiGetDefaultKey()
{
	//默认钥匙密码;
	return 0;
}

DLLEXPORT_MULT int GCApiGetEncryptKey()
{
	srand(GetTickCount()+g_nKeyCount+g_nKeyCount);
	g_nKeyCount++;
	BYTE yar[4];
	memset(yar, 0, 4);

	int i;
	int n,dd;

RE:

	for(i=0; i<4; i++)
	{
		n = rand()%256;
		while(n == 0)
		{
			n = rand()%256;
		}
		yar[i] = (BYTE)n;
	}
	memcpy(&dd,yar,4);
	if(dd==GCApiGetDefaultKey())
		goto RE;

	memcpy(&n, yar, 4);
	//printf("dwEncryptKey=%08x\n", dw);
	return n;
}

DLLEXPORT_MULT BOOL GCApiEncrypt(BOOL isVariFlowEncrypt, char* buf, int len, int nEncryptKey)
{
	if(isVariFlowEncrypt)
	{
		Encrypt1(buf, len, nEncryptKey);
	}
	return TRUE;
}
DLLEXPORT_MULT BOOL GCApiDecrypt(BOOL isVariFlowEncrypt, char* buf, int len, int nEncryptKey)
{
	if(isVariFlowEncrypt)
	{
		Decrypt1(buf, len, nEncryptKey);
	}
	return TRUE;
}

BOOL Encrypt1(char* buf, int len, int nEncryptKey)
{
	BYTE yar[4];
	int i,j;
	for(i=0; i<len; i++)
	{
		nEncryptKey += i*GCHMAC_KEY_CHANGE;
		memcpy(yar, &nEncryptKey,4);	
		for(j=0; j<4; j++)
		{
			buf[i] = buf[i]^yar[j];
		}
	}
	return TRUE;
}
BOOL Decrypt1(char* buf, int len,int nEncryptKey)
{
	BYTE yar[4];
	int i,j;
	for(i=0; i<len; i++)
	{
		nEncryptKey += i*GCHMAC_KEY_CHANGE;
		memcpy(yar, &nEncryptKey, 4);
		for(j=3; j>=0; j--)
		{
			buf[i] = buf[i]^yar[j];
		}
	}
	return TRUE;
}