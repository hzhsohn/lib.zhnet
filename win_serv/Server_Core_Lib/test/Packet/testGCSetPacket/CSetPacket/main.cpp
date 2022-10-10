// m_UserEdit.cpp : 定义控制台应用程序的入口点。
//

#include <iostream>
#include <windows.h>
#include "GCSetPack.h"

using namespace std;

#pragma comment(lib,"GCSetPack.lib")

int main(int argc, char* argv[])
{
////////////////////////////////////////////////////////////////////////1
	GCSetPack pkt;

	TCHAR a[100]=_T("我A是1靓$仔");
	TCHAR b[100]=_T("性别：男");
	pkt.beginWrite();
	pkt.writeString(a);
	pkt.writeString(b);
	pkt.writeBool(true);
	pkt.writeInt(1000);
	pkt.writeBinary("嘿嘿，完成！\0",13);
	pkt.writeShort(1001);
	pkt.writeUnsignedShort(1002);
	pkt.writeUnsignedInt(1003);
	pkt.writeLong(1004);
	pkt.writeUnsignedLong(1005);
	float ffs=111.123;
	double dds=9999.12350;
	pkt.writeFloat(ffs);
	pkt.writeDouble(dds);
	
	printf("解压前：\n");
	printf("sizeof(a)=%d\n",sizeof(a));
	printf("sizeof(b)=%d\n",sizeof(b));
	printf("pkt.GetPackSize=%d\n",pkt.getPackSize());
	printf("\n");

	GCSetPack pkt2;
	TCHAR name[200];
	TCHAR msg[200];
	bool t;
	int i;
	short st;
	unsigned short ust;
	unsigned int ui;
	long l;
	unsigned long ul;
	char bary[20];
	float f=0;
	double d=0;
	pkt2.beginRead(pkt.getPackBuf(),pkt.getPackSize());
	pkt2.readString(name);
	pkt2.readString(msg);
	pkt2.readBool(&t);
	pkt2.readInt(&i);
	pkt2.readBinary(bary,13);
	pkt2.readShort(&st);
	pkt2.readUnsignedShort(&ust);
	pkt2.readUnsignedInt(&ui);
	pkt2.readLong(&l);
	pkt2.readUnsignedLong(&ul);
	pkt2.readFloat(&f);
	pkt2.readDouble(&d);

	printf("解压后：\n");
	printf("name=%s\n",name);
	printf("msg=%s\n",msg);
	printf("t=%d\n",t);
	printf("i=%d\n",i);
	printf("bary=%s\n",bary);
	printf("st=%d\n",st);
	printf("ust=%d\n",ust);
	printf("ui=%d\n",ui);
	printf("l=%d\n",l);
	printf("ul=%d\n",ul);
	printf("f=%f\n",f);
	printf("d=%f\n",d);
	printf("=======================================================\n");
////////////////////////////////////////////////////////////////////////2
	_tcscpy(a,_T("靓仔没用"));
	_tcscpy(b,_T("性别：男"));
	pkt.beginWrite();
	pkt.writeString(a);
	pkt.writeString(b);
	pkt.writeBool(true);
	pkt.writeInt(5000);
	pkt.writeBinary("嘿嘿，完成！我是第二次\0",23);
	pkt.writeShort(2001);
	
	printf("解压前：\n");
	printf("sizeof(a)=%d\n",sizeof(a));
	printf("sizeof(b)=%d\n",sizeof(b));
	printf("pkt.GetPackSize=%d\n",pkt.getPackSize());
	printf("\n");

	pkt2.beginRead(pkt.getPackBuf(),pkt.getPackSize());
	pkt2.readString(name);
	pkt2.readString(msg);
	pkt2.readBool(&t);
	pkt2.readInt(&i);
	pkt2.readBinary(bary,23);
	pkt2.readShort(&st);
	
	printf("解压后：\n");
	printf("name=%s\n",name);
	printf("msg=%s\n",msg);
	printf("t=%d\n",t);
	printf("i=%d\n",i);
	printf("bary=%s\n",bary);
	printf("st=%d\n",st);	
	getchar();
	return 0;
}

