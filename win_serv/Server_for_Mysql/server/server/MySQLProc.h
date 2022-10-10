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

//result��ִ�н��,�ɹ���ʧ��
typedef void MYSQL_PROC_CALLBACK(EzhMysqlOperator result,CzhMySql *mysql,TzhSqlInfo*info);

typedef deque<TzhSqlInfo>  DEQUE_SQLRECORD;
typedef DEQUE_SQLRECORD::iterator  DEQUE_SQLRECORD_IT;

typedef list<TzhMysqlInfo>  LIST_DBLIST;
typedef LIST_DBLIST::iterator  LIST_DBLIST_IT;

class CMySQLProc
{
private:

	//�߳�ʹ�õļ�����
	DWORD					_ThreadUsing;
	DWORD					_dwThreadNum;
	
	//SQL���������
	TYPE_CS					_csSqlList;
	DEQUE_SQLRECORD			_lstSql;

	//�Ͽ����
	DWORD					_dwRecheckTime;

	//�ص�����
	MYSQL_PROC_CALLBACK	*	_mysql_proc;
	
public:
	CMySQLProc(void);
	~CMySQLProc(void);
	
	//������ʹ�õ�������Ϣ
	TzhMysqlInfo			_info;

	//��������ʱ��,���һ��ִ��ʱ��
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