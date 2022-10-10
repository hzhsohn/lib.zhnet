/* 
  Copyright (C) 20018-9, Han.zhihong, Developer.
  All rights reserved. 
*/
#ifndef __XTTRANS_CRYPT_H__


#ifdef  __cplusplus
extern "C" {
#endif

/*
Р§зг:
char key_32[33]="12345678123456781234567812345678";
	char a[]="abcdefg1234567890-abcdefg1234567890-abcdefg1234567890-abcdefg1234567890";
	char b[200]={0};
	int c=XTTransEncrypt(key_32,a,sizeof(a),b);
	XTTransDecrypt(key_32,b,c);

*/

int XTTransEncrypt(char*key_32,char* buf, int len,char* dst_buf);
int XTTransDecrypt(char*key_32,char* buf, int len);


#ifdef  __cplusplus
}
#endif


#define __XTTRANS_CRYPT_H__
#endif
