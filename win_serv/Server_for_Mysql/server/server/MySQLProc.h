#pragma once
#ifndef _MYSQL_DB_PROC_H_


#include "GCHUtilMarco.h"
#include "Cdbmysql.h"
#include <iostream>
#include <deque>
#include <list>
using namespace std;

struct TzhSqlInfo{
	void* pTarget;
	char szSql[2000];
};

enum EzhMysqlOperator
{
	ezhMysqlOperatSuccess,
	ezhMysqlOperatSqlFail,
	ezhMysqlOperatConnectFail
};

//result是执行结果,成功或失败
typedef void MYSQL_PROC_CALLBACK(EzhMysqlOperator result,CzhMySql *mysql,TzhSqlInfo*info);

typedef deque<TzhSqlInfo>  DEQUE_SQLRECORD;
typedef DEQUE_SQLRECORD::iterator  DEQUE_SQLRECORD_IT;

typedef list<TzhMysqlInfo>  LIST_DBLIST;
typedef LIST_DBLIST::iterator  LIST_DBLIST_IT;

class CMySQLProc
{
private:

	//线程使用的计数器
	DWORD					_ThreadUsing;
	DWORD					_dwThreadNum;
	
	//SQL储存的链表
	TYPE_CS					_csSqlList;
	DEQUE_SQLRECORD			_lstSql;

	//断开检测
	DWORD					_dwRecheckTime;

	//回调处理
	MYSQL_PROC_CALLBACK	*	_mysql_proc;
	
public:
	CMySQLProc(void);
	~CMySQLProc(void);
	
	//服务器使用的连接信息
	TzhMysqlInfo			_info;

	//服务器计时器,最后一次执行时间
	DWORD  lastExecuteTime;

	bool setMysqlInfo(const char *host, int port,const char *name,const char *pwd,const char *db);
	bool Initizal(MYSQL_PROC_CALLBACK *mysql_proc);
	void Destory();

	void Query(void* pTarget,char *szSql);
	static void _thread(CMySQLProc*p);
	char *ErrMsg();
};


#define _MYSQL_DB_PROC_H_
#endif