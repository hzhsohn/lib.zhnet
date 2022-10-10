#include "proc.hh"
#include <os_define.h>

Proc g_proc;


int main(int argc,char *argv[])
{

  bool showDebugMsg=false;
  printf("show debug message = %d ",showDebugMsg);
  
  #ifndef Q_OS_MACX
   int isNoDaemon=0;
   if(argc>1)
   {
	   printf("argc = %d argv[1]=%s\r\n",argc,argv[1]);
	   if(0==strcasecmp(argv[1],"DEBUG") || 0==strcasecmp(argv[1],"-debug"))
	   {
		   printf("linux start for debug...\r\n");
       showDebugMsg=true;
		   isNoDaemon=1;
	   }
   }

   if(0==isNoDaemon)
   {
	   printf("linux enable daemon...\r\n");
	   daemon(0,0);
   }
  #endif
  
  //�򿪹ػ��ߵ�����Ϣ����ʾ--
	PrintDebugMsgEnable(showDebugMsg);	
	g_proc.Init();
	return 0;
}

