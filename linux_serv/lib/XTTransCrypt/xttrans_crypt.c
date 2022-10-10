#include "xttrans_crypt.h"
#include <memory.h>

int XTTransEncrypt(char*key_32,char* buf, int len,char* dst_buf)
{
	int i=0,j=0;
	int n=0;
	memcpy(dst_buf,buf,len);
	for(i=0; i<len; i++)
	{
		for(j=0; j<4; j++)
		{
			n=(i+j)%32;
			dst_buf[i] = dst_buf[i]^key_32[n];
		}
	}
	return len;
}

int XTTransDecrypt(char*key_32,char* buf, int len)
{
	int i=0,j=0;
	int n=0;
	for(i=0; i<len; i++)
	{
		for(j=3; j>=0; j--)
		{
			n=(i+j)%32;
			buf[i] = buf[i]^key_32[n];
		}
	}
	return len;
}
