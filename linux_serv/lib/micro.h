#ifndef __MICRO_H_


#include "os_define.h"
#include "xml/mxml.h"
#include "print_msg/print_msg.h"
#include "zhlist/zhlist_exp.h"
#include "zhNet_Core/session.h"
#include "zhNet_Core/platform.h"
#include "md5/md5.h"
#include "zhNet_Core/platform.h"


#ifdef Q_OS_WIN32
	//
	#pragma warning (disable:4819)
	#pragma warning (disable:4996)

    #define MKDIR(path)					mkdir(path)
	#define RMDIR(path)					rmdir(path)
#endif

#ifdef Q_OS_MACX
    //#define MKDIR(path)    [[NSFileManager defaultManager] createDirectoryAtPath: [NSString stringWithUTF8String:path] withIntermediateDirectories:YES attributes:nil error:nil]
	//#define RMDIR(path)    [[NSFileManager defaultManager] removeItemAtPath:path error:nil]
    #define MKDIR(path)                    mkdir(path,S_IRWXU)
    #define RMDIR(path)                    rmdir(path)
#endif

#ifdef Q_OS_LINUX 
    #define MKDIR(path)					mkdir(path,S_IRWXU)
	#define RMDIR(path)					rmdir(path)
#endif

#ifndef __cplusplus
	#ifndef bool
	#define bool unsigned char
	#define true	1
	#define false	0
	#endif
#endif

//
#ifdef Q_OS_WIN32
	#include <direct.h>
	//
	#define strcasecmp  strcmpi
	#define strtok_r    strtok_s
	#define snprintf    sprintf_s
	//
	#define localtime_r(a,b) localtime_s(b,a)
	#define asctime_r(a,b) asctime_s(b,a)
#else
	#include <sys/stat.h>
	#include <sys/types.h>
	//
	#define strcmpi     strcasecmp
	#define strtok_s 	strtok_r
	#define snprintf_s	snprintf
	//
	#define localtime_s(a,b) localtime_r(b,a)
	#define asctime_s(a,b) asctime_r(b,a)

	#define BOOL int
	#define FALSE 0
	#define TRUE 1
	#define DWORD unsigned int
	#define WORD unsigned short

	//windows safe function
	#define fprintf_s 	fprintf
	#define sprintf_s	sprintf
#endif


#define U8     unsigned char
#define U16     unsigned short
#define uchar   unsigned char
#define uint   unsigned int


#define __MICRO_H_
#endif
