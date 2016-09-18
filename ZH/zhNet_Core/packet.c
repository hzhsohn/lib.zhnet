/* 
  packet.h:the base data processed send to network
*/

/* 
  Copyright (C) 2008-2, Han.zhihong, Developer. Created 2008. 
  All rights reserved.

  License to copy and use this software is granted provided that it
  is identified as the "Han.zhihong. Message-Digest
  Algorithm" in all material mentioning or referencing this software
  or this function.
  
  Han.zhihong. makes no representations concerning either
  the merchantability of this software or the suitability of this
  software for any particular purpose. It is provided "as is"
  without express or implied warranty of any kind.
	  
  These notices must be retained in any copies of any part of this
  documentation and/or software.
*/

#include <memory.h>
#include <string.h>
#include "packet.h"

//
//initizal read data
//
//初始化读取数据
//
void zhPackReadInit(TzhPacket *p,unsigned char*szBuf,int nBufSize)
{
	p->wPos=0;
	p->wSize=nBufSize;
	memcpy(p->btBuf,szBuf,nBufSize);
}
void zhPackWriteInit(TzhPacket *p)
{
	p->wPos=0;
	p->wSize=0;
	memset(p->btBuf,0,ZH_NET_PACKET_BODY_LENGTH);
}

int zhPackWriteChar(TzhPacket *p, char i)
{
	if(p->wPos+sizeof(char)>ZH_NET_PACKET_BODY_LENGTH){return 0;}
	memcpy(&p->btBuf[p->wPos], &i, sizeof(char));
	p->wPos+=sizeof(char);
	p->wSize+=sizeof(char);
	return sizeof(char);
}
int zhPackReadChar(TzhPacket *p, char* i)
{
	memcpy(i, &p->btBuf[p->wPos], sizeof(char));
	p->wPos+=sizeof(char);
	return sizeof(char);
}

int zhPackWriteShort(TzhPacket *p, short i)
{
	if(p->wPos+sizeof(short)>ZH_NET_PACKET_BODY_LENGTH){return 0;}
	RE_MEMORY(&i,sizeof(short));
	memcpy(&p->btBuf[p->wPos], &i, sizeof(short));
	p->wPos+=sizeof(short);
	p->wSize+=sizeof(short);
	return sizeof(short);
}
int zhPackReadShort(TzhPacket *p, short* i)
{	
	memcpy(i, &p->btBuf[p->wPos], sizeof(short));
	RE_MEMORY(i,sizeof(short));
	p->wPos+=sizeof(short);
	return sizeof(short);
}
int zhPackWriteInt(TzhPacket *p, int i)
{
	if(p->wPos+sizeof(int)>ZH_NET_PACKET_BODY_LENGTH){return 0;}
	RE_MEMORY(&i,sizeof(int));
	memcpy(&p->btBuf[p->wPos], &i, sizeof(int));
	p->wPos+=sizeof(int);
	p->wSize+=sizeof(int);
	return sizeof(int);
}
int zhPackReadInt(TzhPacket *p, int* i)
{
	memcpy(i, &p->btBuf[p->wPos], sizeof(int));
	RE_MEMORY(i,sizeof(int));
	p->wPos+=sizeof(int);
	return sizeof(int);
}
int zhPackWriteLong(TzhPacket *p, long i)
{
	if(p->wPos+sizeof(long)>ZH_NET_PACKET_BODY_LENGTH){return 0;}
	RE_MEMORY(&i,sizeof(long));
	memcpy(&p->btBuf[p->wPos], &i, sizeof(long));
	p->wPos+=sizeof(long);
	p->wSize+=sizeof(long);
	return sizeof(long);
}
int zhPackReadLong(TzhPacket *p, long* i)
{
	memcpy(i, &p->btBuf[p->wPos], sizeof(long));
	RE_MEMORY(i,sizeof(long));
	p->wPos+=sizeof(long);
	return sizeof(long);
}
int zhPackWriteString(TzhPacket *p, char* str)
{
	size_t len=0;
	
	len =strlen(str);

	if(p->wPos+len>ZH_NET_PACKET_BODY_LENGTH){return 0;}

	strcpy(&p->btBuf[p->wPos], str);
	p->btBuf[len + p->wPos] =0;
	len += 1;
	
	p->wPos+=(unsigned short)len;
	p->wSize+=(unsigned short)len;
	return (int)len;
}

int zhPackWriteStringUnicode(TzhPacket *p, char* str)
{
	size_t len=0;

	while(str[len]!=0 || str[len+1]!=0)
	{len+=2;}

	if(p->wPos+len>ZH_NET_PACKET_BODY_LENGTH){return 0;}

	memcpy(&p->btBuf[p->wPos], str,len);
	p->btBuf[p->wPos+len] = 0;
	p->btBuf[p->wPos+len+1] = 0;
	len += 2;

	p->wPos+=(unsigned short)len;
	p->wSize+=(unsigned short)len;
	return (int)len;
}

int zhPackReadString(TzhPacket *p, char* str)
{
	size_t len=0;

	len = strlen(&p->btBuf[p->wPos]);
	strcpy(str, &p->btBuf[p->wPos]);
	str[len] = 0;
	len += 1;

	p->wPos+=(unsigned short)len;
	return (int)len;
}

int zhPackReadStringUnicode(TzhPacket *p, char* str)
{
	size_t len=0;

	while(p->btBuf[p->wPos+len]!=0 || p->btBuf[p->wPos+len+1]!=0)
	{len+=2;}

	memcpy(str,&p->btBuf[p->wPos], len);
	str[len] = 0;
	str[len+1] = 0;
	len += 2;
	
	p->wPos+=(unsigned short)len;
	return (int)len;
}

int zhPackWriteBinary(TzhPacket *p, char* str ,int len)
{
	if(p->wPos+len>ZH_NET_PACKET_BODY_LENGTH || len<0){return 0;}
	memcpy(&p->btBuf[p->wPos], str,len);
	p->wPos+=len;
	p->wSize+=len;
	return len;
}
int zhPackReadBinary(TzhPacket *p, char* str ,int len)
{
	if(len<0 || len>ZH_NET_PACKET_BODY_LENGTH){return 0;}
	memcpy(str, &p->btBuf[p->wPos],len);
	p->wPos+=len;
	return len;
}
int zhPackWriteBool(TzhPacket *p,bool b)
{
	if(p->wPos+sizeof(bool)>ZH_NET_PACKET_BODY_LENGTH){return 0;}
	RE_MEMORY(&b,sizeof(bool));
	memcpy(&p->btBuf[p->wPos], &b, sizeof(bool));
	p->wPos+=sizeof(bool);
	p->wSize+=sizeof(bool);
	return sizeof(bool);
}
int zhPackReadBool(TzhPacket *p, bool*b)
{
	memcpy(b, &p->btBuf[p->wPos], sizeof(bool));
	RE_MEMORY(b,sizeof(bool));
	p->wPos+=sizeof(bool);
	return sizeof(bool);
}


////////////////////////////
int zhPackWriteUnsignedChar(TzhPacket *p,unsigned char i)
{
	if(p->wPos+sizeof(unsigned char)>ZH_NET_PACKET_BODY_LENGTH){return 0;}
	memcpy(&p->btBuf[p->wPos], &i, sizeof(unsigned char));
	p->wPos+=sizeof(unsigned char);
	p->wSize+=sizeof(unsigned char);
	return sizeof(unsigned char);
}
int zhPackReadUnsignedChar(TzhPacket *p,unsigned char* i)
{
	memcpy(i, &p->btBuf[p->wPos], sizeof(unsigned char));
	p->wPos+=sizeof(unsigned char);
	return sizeof(unsigned char);
}

int zhPackWriteUnsignedShort(TzhPacket *p,unsigned short i)
{
	if(p->wPos+sizeof(unsigned short)>ZH_NET_PACKET_BODY_LENGTH){return 0;}
	RE_MEMORY(&i,sizeof(unsigned short));
	memcpy(&p->btBuf[p->wPos], &i, sizeof(unsigned short));
	p->wPos+=sizeof(unsigned short);
	p->wSize+=sizeof(unsigned short);
	return sizeof(unsigned short);
}
int zhPackReadUnsignedShort(TzhPacket *p,unsigned short* i)
{
	memcpy(i, &p->btBuf[p->wPos], sizeof(unsigned short));
	RE_MEMORY(i,sizeof(unsigned short));
	p->wPos+=sizeof(unsigned short);
	return sizeof(unsigned short);
}
int zhPackWriteUnsignedInt(TzhPacket *p,unsigned int i)
{
	if(p->wPos+sizeof(unsigned int)>ZH_NET_PACKET_BODY_LENGTH){return 0;}
	RE_MEMORY(&i,sizeof(unsigned int));
	memcpy(&p->btBuf[p->wPos], &i, sizeof(unsigned int));
	p->wPos+=sizeof(unsigned int);
	p->wSize+=sizeof(unsigned int);
	return sizeof(unsigned int);
}
int zhPackReadUnsignedInt(TzhPacket *p,unsigned int* i)
{
	memcpy(i, &p->btBuf[p->wPos], sizeof(unsigned int));
	RE_MEMORY(i,sizeof(unsigned int));
	p->wPos+=sizeof(unsigned int);
	return sizeof(unsigned int);
}
int zhPackWriteUnsignedLong(TzhPacket *p,unsigned long i)
{
	if(p->wPos+sizeof(unsigned long)>ZH_NET_PACKET_BODY_LENGTH){return 0;}
	RE_MEMORY(&i,sizeof(unsigned long));
	memcpy(&p->btBuf[p->wPos], &i, sizeof(unsigned long));
	p->wPos+=sizeof(unsigned long);
	p->wSize+=sizeof(unsigned long);
	return sizeof(unsigned long);
}
int zhPackReadUnsignedLong(TzhPacket *p,unsigned long* i)
{
	memcpy(i, &p->btBuf[p->wPos], sizeof(unsigned long));
	RE_MEMORY(i,sizeof(unsigned long));
	p->wPos+=sizeof(unsigned long);
	return sizeof(unsigned long);
}


int zhPackWriteFloat(TzhPacket *p,float f)
{
	if(p->wPos+sizeof(float)>ZH_NET_PACKET_BODY_LENGTH){return 0;}
	RE_MEMORY(&f,sizeof(float));
	memcpy(&p->btBuf[p->wPos], &f, sizeof(float));
	p->wPos+=sizeof(float);
	p->wSize+=sizeof(float);
	return sizeof(float);
}
int zhPackReadFloat(TzhPacket *p, float*f)
{
	memcpy(f, &p->btBuf[p->wPos], sizeof(float));
	RE_MEMORY(f,sizeof(float));
	p->wPos+=sizeof(float);
	return sizeof(float);
}

int zhPackWriteDouble(TzhPacket *p,double d)
{
	if(p->wPos+sizeof(double)>ZH_NET_PACKET_BODY_LENGTH){return 0;}
	RE_MEMORY(&d,sizeof(double));
	memcpy(&p->btBuf[p->wPos], &d, sizeof(double));
	p->wPos+=sizeof(double);
	p->wSize+=sizeof(double);
	return sizeof(double);
}
int zhPackReadDouble(TzhPacket *p, double*d)
{
	memcpy(d, &p->btBuf[p->wPos], sizeof(double));
	RE_MEMORY(d,sizeof(double));
	p->wPos+=sizeof(double);
	return sizeof(double);
}
