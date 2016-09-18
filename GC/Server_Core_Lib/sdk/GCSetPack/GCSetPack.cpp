#include "GCSetPack.h"


//BOOL APIENTRY DllMain( HANDLE hModule, 
//					  DWORD  ul_reason_for_call, 
//					  LPVOID lpReserved
//					  )
//{
//	return TRUE;
//}

GCSetPack::GCSetPack(void)
{
	Reset();
}

GCSetPack::~GCSetPack(void)
{
}

void GCSetPack::Reset()
{
	m_enlen=0;
	m_unlen=0;
}


/* 返回包的大小 */
size_t GCSetPack::getPackSize()
{
	return m_enlen;
}
/* 返回当前解压的位置 */
size_t GCSetPack::getCurUnPushPos()
{
	return m_unlen;
}
/* 返回包的内容 */
char* GCSetPack::getPackBuf()
{
	return m_buf;
}

/*
	包的最大值根据 m_MaxLen 而定,默认为 SETPACKET_SIZE
*/
void GCSetPack::beginWrite()
{
	Reset();
}

/*
	true=success
	false=fail
*/
bool GCSetPack::beginRead(char*buf,size_t len)
{
	if(len>PACKET_ONE_LENGTH)
		return false;

	Reset();
	memcpy(m_buf,buf,len);
	return true;
}

//===========================  Push Begin =============================//
size_t GCSetPack::writeChar(char c)
{
	m_enlen+=_PushChar(&m_buf[m_enlen],c);
	return m_enlen;
}

size_t GCSetPack::writeUnsignedChar(unsigned char c)
{
	m_enlen+=_PushUnsignedChar(&m_buf[m_enlen],c);
	return m_enlen;
}

size_t GCSetPack::writeShort(short i)
{
	m_enlen+=_PushShort(&m_buf[m_enlen],i);
	return m_enlen;
}
size_t GCSetPack::writeUnsignedShort(unsigned short i)
{
	m_enlen+=_PushUnsignedShort(&m_buf[m_enlen],i);
	return m_enlen;
}
size_t GCSetPack::writeInt(int i)
{
	m_enlen+=_PushInt(&m_buf[m_enlen],i);
	return m_enlen;
}
size_t GCSetPack::writeUnsignedInt(unsigned int i)
{
	m_enlen+=_PushUnsignedInt(&m_buf[m_enlen],i);
	return m_enlen;
}
size_t GCSetPack::writeLong(long i)
{
	m_enlen+=_PushLong(&m_buf[m_enlen],i);
	return m_enlen;
}
size_t GCSetPack::writeUnsignedLong(unsigned long i)
{
	m_enlen+=_PushUnsignedLong(&m_buf[m_enlen],i);
	return m_enlen;
}
size_t GCSetPack::writeString(char* str)
{
	m_enlen+=_PushString(&m_buf[m_enlen],str);
	return m_enlen;
}
size_t GCSetPack::writeStringUnicode(char* str)
{
	m_enlen+=_PushStringUnicode(&m_buf[m_enlen],str);
	return m_enlen;
}
size_t GCSetPack::writeBinary(char* str ,int len)
{
	m_enlen+=_PushBinary(&m_buf[m_enlen],str,len);
	return m_enlen;
}
size_t GCSetPack::writeBool(bool b)
{
	m_enlen+=_PushBool(&m_buf[m_enlen],b);
	return m_enlen;
}

size_t GCSetPack::writeFloat(float f)
{
	m_enlen+=_PushFloat(&m_buf[m_enlen],f);
	return m_enlen;
}

size_t GCSetPack::writeDouble(double d)
{
	m_enlen+=_PushDouble(&m_buf[m_enlen],d);
	return m_enlen;
}
//===========================  Push End =============================//

//===========================  UnPush Begin =============================//
size_t GCSetPack::readChar(char* c)
{
	m_unlen+=_UnPushChar(&m_buf[m_unlen],c);
	return m_unlen;
}
size_t GCSetPack::readUnsignedChar(unsigned char* c)
{
	m_unlen+=_UnPushUnsignedChar(&m_buf[m_unlen],c);
	return m_unlen;
}
size_t GCSetPack::readShort(short* i)
{
	m_unlen+=_UnPushShort(&m_buf[m_unlen],i);
	return m_unlen;
}
size_t GCSetPack::readUnsignedShort(unsigned short* i)
{
	m_unlen+=_UnPushUnsignedShort(&m_buf[m_unlen],i);
	return m_unlen;
}
size_t GCSetPack::readInt(int* i)
{
	m_unlen+=_UnPushInt(&m_buf[m_unlen],i);
	return m_unlen;
}
size_t GCSetPack::readUnsignedInt(unsigned int* i)
{
	m_unlen+=_UnPushUnsignedInt(&m_buf[m_unlen],i);
	return m_unlen;
}
size_t GCSetPack::readLong(long* i)
{
	m_unlen+=_UnPushLong(&m_buf[m_unlen],i);
	return m_unlen;
}
size_t GCSetPack::readUnsignedLong(unsigned long* i)
{
	m_unlen+=_UnPushUnsignedLong(&m_buf[m_unlen],i);
	return m_unlen;
}
size_t GCSetPack::readString(char* str)
{
	m_unlen+=_UnPushString(&m_buf[m_unlen],str);
	return m_unlen;
}
size_t GCSetPack::readStringUnicode(char* str)
{
	m_unlen+=_UnPushStringUnicode(&m_buf[m_unlen],str);
	return m_unlen;
}
size_t GCSetPack::readBinary(char* str ,int len)
{
	m_unlen+=_UnPushBinary(&m_buf[m_unlen],str,len);
	return m_unlen;
}
size_t GCSetPack::readBool(bool* b)
{
	m_unlen+=_UnPushBool(&m_buf[m_unlen],b);
	return m_unlen;
}
size_t GCSetPack::readFloat(float* f)
{
	m_unlen+=_UnPushFloat(&m_buf[m_unlen],f);
	return m_unlen;
}
size_t GCSetPack::readDouble(double* d)
{
	m_unlen+=_UnPushDouble(&m_buf[m_unlen],d);
	return m_unlen;
}
//===========================  UnPush End =============================//


/////////////////////////////// 公共函数 ///////////////////////////////////
DLLEXPORT_API size_t WINAPI _PushChar(char* buf, char c)
{
	memcpy(buf, &c, sizeof(char));
	return sizeof(char);
}
DLLEXPORT_API size_t WINAPI _UnPushChar(char* buf, char* c)
{
	memcpy(c, buf, sizeof(char));
	return sizeof(char);
}
DLLEXPORT_API size_t WINAPI _PushUnsignedChar(char* buf,unsigned char c)
{
	memcpy(buf, &c, sizeof(unsigned char));
	return sizeof(unsigned char);
}
DLLEXPORT_API size_t WINAPI _UnPushUnsignedChar(char* buf,unsigned char* c)
{
	memcpy(c, buf, sizeof(unsigned char));
	return sizeof(unsigned char);
}

DLLEXPORT_API size_t WINAPI _PushShort(char* buf, short i)
{
	memcpy(buf, &i, sizeof(short));
	return sizeof(short);
}
DLLEXPORT_API size_t WINAPI _UnPushShort(char* buf, short* i)
{
	memcpy(i, buf, sizeof(short));
	return sizeof(short);
}
DLLEXPORT_API size_t WINAPI _PushUnsignedShort(char* buf,unsigned short i)
{
	memcpy(buf, &i, sizeof(unsigned short));
	return sizeof(unsigned short);
}
DLLEXPORT_API size_t WINAPI _UnPushUnsignedShort(char* buf,unsigned short* i)
{
	memcpy(i, buf, sizeof(unsigned short));
	return sizeof(unsigned short);
}
DLLEXPORT_API size_t WINAPI _PushInt(char* buf, int i)
{
	memcpy(buf, &i, sizeof(int));
	return sizeof(int);
}
DLLEXPORT_API size_t WINAPI _UnPushInt(char* buf, int* i)
{
	memcpy(i, buf, sizeof(int));
	return sizeof(int);
}
DLLEXPORT_API size_t WINAPI _PushUnsignedInt(char* buf,unsigned int i)
{
	memcpy(buf, &i, sizeof(unsigned int));
	return sizeof(unsigned int);
}
DLLEXPORT_API size_t WINAPI _UnPushUnsignedInt(char* buf,unsigned int* i)
{
	memcpy(i, buf, sizeof(unsigned int));
	return sizeof(unsigned int);
}

DLLEXPORT_API size_t WINAPI _PushLong(char* buf, long i)
{
	memcpy(buf, &i, sizeof(long));
	return sizeof(long);
}
DLLEXPORT_API size_t WINAPI _UnPushLong(char* buf, long* i)
{
	memcpy(i, buf, sizeof(long));
	return sizeof(long);
}
DLLEXPORT_API size_t WINAPI _PushUnsignedLong(char* buf,unsigned long i)
{
	memcpy(buf, &i, sizeof(unsigned long));
	return sizeof(unsigned long);
}
DLLEXPORT_API size_t WINAPI _UnPushUnsignedLong(char* buf,unsigned long* i)
{
	memcpy(i, buf, sizeof(unsigned long));
	return sizeof(unsigned long);
}
DLLEXPORT_API size_t WINAPI _PushString(char* buf, char* str)
{
	size_t len =strlen(str);
	strcpy(buf, str);
	buf[len] =0;
	len += 1;
	return len;
}
DLLEXPORT_API size_t WINAPI _UnPushString(char* buf, char* str)
{
	size_t len = strlen(buf);
	strcpy(str, buf);
	str[len] = 0;
	len += 1;
	return len;
}
DLLEXPORT_API size_t WINAPI _PushStringUnicode(char* buf, char* str)
{
	size_t len=0;

	while(str[len]!=0 || str[len+1]!=0)
	{len+=2;}

	memcpy(buf, str,len);
	buf[len] = 0;
	buf[len+1] = 0;
	len += 2;
	return len;
}
DLLEXPORT_API size_t WINAPI _UnPushStringUnicode(char* buf, char* str)
{
	int len=0;
	while(buf[len]!=0 || buf[len+1]!=0)
		len+=2;

	memcpy(str,buf, len);
	str[len] = 0;
	str[len+1] = 0;
	len += 2;
	return len;
}
DLLEXPORT_API size_t WINAPI _PushBinary(char* buf, char* str ,int len)
{
	memcpy(buf, str,len);
	return len;
}
DLLEXPORT_API size_t WINAPI _UnPushBinary(char* buf, char* str ,int len)
{
	memcpy(str, buf,len);
	return len;
}
DLLEXPORT_API size_t WINAPI _PushBool(char* buf, bool b)
{
	memcpy(buf, &b, sizeof(bool));
	return sizeof(bool);
}
DLLEXPORT_API size_t WINAPI _UnPushBool(char* buf, bool* b)
{
	memcpy(b, buf, sizeof(bool));
	return sizeof(bool);
}


DLLEXPORT_API size_t WINAPI _PushFloat(char* buf, float f)
{
	memcpy(buf, &f, sizeof(float));
	return sizeof(float);
}
DLLEXPORT_API size_t WINAPI _UnPushFloat(char* buf, float*f)
{
	memcpy(f, buf, sizeof(float));
	return sizeof(float);
}

DLLEXPORT_API size_t WINAPI _PushDouble(char* buf, double d)
{
	memcpy(buf, &d, sizeof(double));
	return sizeof(double);
}
DLLEXPORT_API size_t WINAPI _UnPushDouble(char* buf, double* d)
{
	memcpy(d, buf, sizeof(double));
	return sizeof(double);
}


