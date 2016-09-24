/*
Cdbmysql.h: interface for the CG_DBMySql class.

例子:

初始化------------------------------------
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
//执行SQL
int cmdID=1;
g_MySql.Query(&cmdID,(char*)sql);

读取----------------------------------------
//MYSQL处理函数
void ProcMysql(EzhMysqlOperator result,CzhMySql *mysql,TzhSqlInfo*info)
{
	int cmdID=*(int*)info->pTarget;
	TzhSqlExc*p=(TzhSqlExc*)info->pTarget;
	while(mysql->GetRow())
	{
		setlog("%s",_S2WS(mysql->GetField("dev_uuid")));
	}
	mysql->freeQueryRes();//一定要执行这句
}
*/

#ifndef __DB_MYSQL_H__
#define __DB_MYSQL_H__

#include<string>
using namespace std;

#include "MySQL/include/mysql.h"

//数据库信息
typedef struct _TzhMysqlInfo
{
	string host;
	int port;
	string name;
	string pwd;
	string db;
}TzhMysqlInfo,*PTzhMysqlInfo;

class CzhMySql
{
public:
	unsigned long GetInsertId();
	long  GetAffectedRows();
	bool  Query(const char *sql,int len=0);
	bool  Connect(const char *host,int port,const char *name,const char *pwd,const char *db);
	bool  Connect();
	bool  Connect(TzhMysqlInfo* info);
	bool  GetField(int idx,char *field,int *type,char **value);
	bool  GetFieldContent(int nIdx , int *pnType , char **ppValue);
    bool  GetRow();
	void  SetLog(bool log);
	char*  ShowError();
	bool  Close();
	int   GetFieldInfo(int nIdx, char *pszName);
    int   GetRowCount();
	int   GetFieldCount();
	char *GetField(char *fname,int *len = NULL);
	bool HandleState();
	TzhMysqlInfo* getInfo();
	void freeQueryRes();

	CzhMySql();
	virtual ~CzhMySql();
	
private:
	bool			m_log;
	MYSQL			*m_handle;
	MYSQL_RES		*m_res;
	MYSQL_ROW		m_row;
	MYSQL_FIELD		*m_fields;
	int				m_fieldCnt;
	char			m_err[256];

	TzhMysqlInfo		m_dbinfo;
};

#endif 
