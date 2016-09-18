#pragma once
#include "GCHUtilMarco.h"
#include <wtypes.h>
#include <iostream>
#include <tchar.h>
#include "GCHNetUtil.h"
using namespace std;

#ifndef _GCSetPacket_

DLLEXPORT_API size_t WINAPI _PushChar(char* buf, char c);
DLLEXPORT_API size_t WINAPI _UnPushChar(char* buf, char* c);
DLLEXPORT_API size_t WINAPI _PushUnsignedChar(char* buf,unsigned char c);
DLLEXPORT_API size_t WINAPI _UnPushUnsignedChar(char* buf,unsigned char* c);

DLLEXPORT_API size_t WINAPI _PushShort(char* buf, short i);
DLLEXPORT_API size_t WINAPI _UnPushShort(char* buf, short* i);
DLLEXPORT_API size_t WINAPI _PushUnsignedShort(char* buf,unsigned short i);
DLLEXPORT_API size_t WINAPI _UnPushUnsignedShort(char* buf,unsigned short* i);

DLLEXPORT_API size_t WINAPI _PushInt(char* buf, int i);
DLLEXPORT_API size_t WINAPI _UnPushInt(char* buf, int* i);
DLLEXPORT_API size_t WINAPI _PushUnsignedInt(char* buf,unsigned int i);
DLLEXPORT_API size_t WINAPI _UnPushUnsignedInt(char* buf,unsigned int* i);

DLLEXPORT_API size_t WINAPI _PushLong(char* buf, long i);
DLLEXPORT_API size_t WINAPI _UnPushLong(char* buf, long* i);
DLLEXPORT_API size_t WINAPI _PushUnsignedLong(char* buf,unsigned long i);
DLLEXPORT_API size_t WINAPI _UnPushUnsignedLong(char* buf,unsigned long* i);

DLLEXPORT_API size_t WINAPI _PushString(char* buf, char* str);
DLLEXPORT_API size_t WINAPI _UnPushString(char* buf, char* str);
DLLEXPORT_API size_t WINAPI _PushStringUnicode(char* buf, char* str);
DLLEXPORT_API size_t WINAPI _UnPushStringUnicode(char* buf, char* str);
DLLEXPORT_API size_t WINAPI _PushBinary(char* buf, char* str ,int len);
DLLEXPORT_API size_t WINAPI _UnPushBinary(char* buf, char* str ,int len);
DLLEXPORT_API size_t WINAPI _PushBool(char* buf, bool b);
DLLEXPORT_API size_t WINAPI _UnPushBool(char* buf, bool* b);

DLLEXPORT_API size_t WINAPI _PushFloat(char* buf, float f);
DLLEXPORT_API size_t WINAPI _UnPushFloat(char* buf, float* f);

DLLEXPORT_API size_t WINAPI _PushDouble(char* buf, double d);
DLLEXPORT_API size_t WINAPI _UnPushDouble(char* buf, double* d);

class DLLEXPORT_CLASS GCSetPack
{
private:
	size_t m_enlen;
	size_t m_unlen;
	char m_buf[PACKET_ONE_LENGTH];
	void Reset();

public:
	GCSetPack(void);
	~GCSetPack(void);
	void beginWrite();
	bool beginRead(char*buf,size_t len);

	size_t getPackSize();
	size_t getCurUnPushPos();
	char* getPackBuf();
	
	size_t writeChar(char c);
	size_t readChar(char* c);
	size_t writeUnsignedChar(unsigned char c);
	size_t readUnsignedChar(unsigned char* c);

	size_t writeShort(short i);
	size_t readShort(short* i);
	size_t writeUnsignedShort(unsigned short i);
	size_t readUnsignedShort(unsigned short* i);

	size_t writeInt(int i);
	size_t readInt(int* i);
	size_t writeUnsignedInt(unsigned int i);
	size_t readUnsignedInt(unsigned int* i);
	size_t writeLong(long i);
	size_t readLong(long* i);
	size_t writeUnsignedLong(unsigned long i);
	size_t readUnsignedLong(unsigned long* i);

	size_t writeString(char* str);
	size_t readString(char* str);
	size_t writeStringUnicode(char* str);
	size_t readStringUnicode(char* str);

	size_t writeBinary(char* str ,int len);
	size_t readBinary(char* str ,int len);

	size_t writeBool(bool b);
	size_t readBool(bool* b);

	size_t writeFloat(float f);
	size_t readFloat(float* f);

	size_t writeDouble(double d);
	size_t readDouble(double* d);
};

#define _GCSetPacket_
#endif