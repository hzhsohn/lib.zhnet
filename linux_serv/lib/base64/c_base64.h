
#ifndef __CC_BASE64_DECODE_H
#define __CC_BASE64_DECODE_H

#ifdef __cplusplus
extern "C" {
#endif	
	

#include "stdio.h"
#include "stdlib.h"

long int base64_encode( char *src,long int src_len, char *dst);

int _base64Decode( unsigned char *input, unsigned int input_len, unsigned char *output, unsigned int *output_len );

/*
例子: 使用完之后需要释放内存
unsigned char * out=NULL;
int outlen=0;
outlen=base64Decode((unsigned char*)jsonValBuf->valuestring,jsonValLen->valueint,&out);
free(out);
*/
int base64Decode(unsigned char *in, unsigned int inLength, unsigned char **out);

#ifdef __cplusplus
}
#endif	

#endif // __CC_BASE64_DECODE_H
