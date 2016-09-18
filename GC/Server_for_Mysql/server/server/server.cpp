// server.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <io.h>
#include <iostream>
#include "Userlist.h"
#include "GCSetPack.h"
#include "get_path.h"

using namespace std;

//启用服务器
void ProcessStartupServ();

CMySQLProc	g_MySql;

//ini配置信息
int SERVICES_PORT;

//数据库信息
TzhMysqlInfo DB_INFO;

//逻辑变量
BOOL g_bIsServicesOpen;
Userlist	g_userList;


int func_main()
{
	char iniCfgPath[512];
	getCurrentPath("zh_config.ini",iniCfgPath,sizeof(iniCfgPath));

	GCH_INIT_LOG();

	//检测INI配置文件,不存在的话就生成
	if(access(iniCfgPath,0))
	{
		char tmpBuf[128];

		//不存在,生成
		SERVICES_PORT=8777;

		DB_INFO.host="localhost";
		DB_INFO.port=3306;
		DB_INFO.name="root";
		DB_INFO.pwd="123";
		DB_INFO.db="test";

		WritePrivateProfileString("INFO","SERVICES_PORT","8777",iniCfgPath);

		WritePrivateProfileString("MYSQL","HOST",DB_INFO.host.c_str(),iniCfgPath);
		sprintf(tmpBuf,"%d",DB_INFO.port);
		WritePrivateProfileString("MYSQL","PORT",tmpBuf,iniCfgPath);
		WritePrivateProfileString("MYSQL","USER",DB_INFO.name.c_str(),iniCfgPath);
		WritePrivateProfileString("MYSQL","PASSWORD",DB_INFO.pwd.c_str(),iniCfgPath);
		WritePrivateProfileString("MYSQL","DB",DB_INFO.db.c_str(),iniCfgPath);

		msgshow("create ini document.");
	}
	else
	{
		char tmpBuf[512];
		SERVICES_PORT =GetPrivateProfileInt("INFO","SERVICES_PORT",8777,iniCfgPath);

		GetPrivateProfileString("MYSQL","HOST","",tmpBuf,sizeof(tmpBuf),iniCfgPath);
		DB_INFO.host=tmpBuf;
		DB_INFO.port =GetPrivateProfileInt("info","PORT",3306,iniCfgPath);
		GetPrivateProfileString("MYSQL","USER","",tmpBuf,sizeof(tmpBuf),iniCfgPath);
		DB_INFO.name=tmpBuf;
		GetPrivateProfileString("MYSQL","PASSWORD","",tmpBuf,sizeof(tmpBuf),iniCfgPath);
		DB_INFO.pwd=tmpBuf;
		GetPrivateProfileString("MYSQL","DB","",tmpBuf,sizeof(tmpBuf),iniCfgPath);
		DB_INFO.db=tmpBuf;

		msgshow("----------------------------------");
		msgshow("read ini [INFO]");
		msgshow("read ini SERVICES_PORT=%d",SERVICES_PORT);
		msgshow("----------------------------------");
		msgshow("read ini [MYSQL]");
		msgshow("read ini HOST=\"%s\"",DB_INFO.host.c_str());
		msgshow("read ini PORT=%d",DB_INFO.port);
		msgshow("read ini USER=\"%s\"",DB_INFO.name.c_str());
		msgshow("read ini PASSWORD=\"%s\"",DB_INFO.pwd.c_str());
		msgshow("read ini DB=\"%s\"",DB_INFO.db.c_str());
	}

	//启用服务器
	ProcessStartupServ();
	return 0;
}

void ProcessStartupServ()
{	
	if(g_bIsServicesOpen)
	{
		msgshow("Services alread open as server.");
		return;
	}

	//数据流加密
	//GSREncryptEnable(YES);
	
	//start services
	GSRTBegin();	
	
	if(!GSRTInit(RecvDataCallBack,DissconnectCallBack,AcceptCallBack, SERVICES_PORT))
	{
		errlog("Services startup failed as server.");
		return;
	}
	setlog("Open Services Successfully. Port=%d",SERVICES_PORT);

	//设置服务器信息.
	g_MySql.Initizal(ProcMysql);	
	setlog("set mysql %s:%d user=%s password=%s db=%s",
		DB_INFO.host.c_str(),
		DB_INFO.port,
		DB_INFO.name.c_str(),
		DB_INFO.pwd.c_str(),
		DB_INFO.db.c_str());

	g_MySql.setMysqlInfo(DB_INFO.host.c_str(),
		DB_INFO.port,
		DB_INFO.name.c_str(),
		DB_INFO.pwd.c_str(),
		DB_INFO.db.c_str());
	
	g_bIsServicesOpen=true;
}

void func_main_release()
{
	if(!g_bIsServicesOpen)
	{
		msgshow("Services alread closed as server.");
		return;
	}
	setlog("Closeing ,Please wait a minute...");

	g_MySql.Destory();
	GSRTEnd();

	setlog("Destory Mysql Thread.");

	setlog("Close Services Successfully.");
	g_bIsServicesOpen=false;
	GCH_CLOSE_LOG();
}

void func_main_loop()
{
}