/* ///////////////////////////sample//////////////////////////////////


TzhFileHandle fHandle;
TzhFileData fdata;
int main()
{
printf("search file:\n");
	if(zhFindInit(&fHandle,"./"))
	{
     while(zhFindNext(&fHandle,&fdata))
     {
        printf("path=%s\tname=%s\tattrib=%d\t\n",fdata.path,fdata.filename,fdata.attrib);
     }
     zhFindDestory(&fHandle);
	}
printf("end search\n");
getchar();
	return 0;
}

*////////////////////////////////////////////////////////////////////
 
#ifndef _ZH_FIND_FILES_SEACH_H__

#ifdef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <windows.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#endif

#ifdef __cplusplus
extern "C"{
#endif
	    
typedef enum _EzhFileAttrib{
    ezhFileAttribUnknow,
    ezhFileAttribFile,
    ezhFileAttribDir
}EzhFileAttrib;

typedef struct _TzhFileHandle
{
	char path[256]; //目录路径
#ifdef _WIN32
    struct _finddata_t file; 
    long h_handle;
#else
	  DIR *pdir;
    struct dirent *pdirent;
#endif
}TzhFileHandle;

typedef struct _TzhFileData
{
	char path[256];
	char filename[128];
	EzhFileAttrib attrib;
}TzhFileData;

//函数
/*
返回值
1=可继续获取文件或目录
0=已经不存在信息
*/
int zhFindInit(TzhFileHandle *hle,char*path);

/*
返回值
1=可继续获取文件或目录
0=已经不存在信息了
*/
int zhFindNext(TzhFileHandle *hle,TzhFileData*out_data);
void zhFindDestory(TzhFileHandle *hle);

#ifdef __cplusplus
}
#endif

#define _ZH_FIND_FILES_SEACH_H__
#endif
