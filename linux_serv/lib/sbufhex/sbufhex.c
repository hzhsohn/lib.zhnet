#include "sbufhex.h"

int sbufDecode(char *src, char *hex) 
{
    int i, n = 0;
	for (i = 0; src[i]; i ++) {
		src[i]=toupper((int)src[i]);
    }
    for (i = 0; src[i]; i += 2) {
		if (src[i] >= 'A' && src[i] <= 'F')
            hex[n] = src[i] - 'A' + 10;
        else hex[n] = src[i] - '0';
        if (src[i + 1] >= 'A' && src[i + 1] <= 'F')
            hex[n] = (hex[n] << 4) | (src[i + 1] - 'A' + 10);
        else hex[n] = (hex[n] << 4) | (src[i + 1] - '0');
        ++n;
    }
    return n;
}

//�����Ҫ�ͷ�
char* sbufEncode(const unsigned char *p_pBuff, int p_pBuffLen,int* dstLen) 
{
	char* ssbuf;
    int i;
    if (NULL != p_pBuff) {
		ssbuf=(char*)malloc(p_pBuffLen*2+10);
		if(ssbuf)
		{
			for (i = 0; i < p_pBuffLen; i++) {
				sprintf(ssbuf + i * 2, "%02X", (unsigned char) p_pBuff[i]);
			}
			//printf("[%s]\n", dst_buffer);
			i*=2;
			ssbuf[i]=0;
			*dstLen=i;
			return ssbuf;
		}
    }

	return NULL;
}
