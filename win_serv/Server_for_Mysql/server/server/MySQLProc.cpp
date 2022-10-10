#include "stdafx.h"
#include "MySQLProc.h"

CMySQLProc::CMySQLProc(void)
{
	//计算工作线程数
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	_dwThreadNum = systemInfo.dwNumberOfProcessors;//建议多条MYSQL通道

	INIT_CS(&_csSqlList);
}

CMySQLProc::~CMySQLProc(void)
{
	Destory();
	Sleep(100);
	DELETE_CS(&_csSqlList);
}

bool CMySQLProc::setMysqlInfo(const char *host, int port,const char *name,const char *pwd,const char *db)
{
	CzhMySql mysql;

	if(0==port) return false;
	if(NULL==host)return false;
	if(NULL==name)return false;
	if(NULL==pwd)return false;
	if(NULL==db)return false;

	_info.host = host;
	_info.port = port;
	_info.name = name;
	_info.pwd = pwd;
	_info.db = db;

	if(mysql.Connect(&_info))
	{
		setlog("connect mysql success.");
	}
	else
	{
		setlog("connect mysql fail.");
		return false;
	}
	mysql.Close();
	return true;
}

bool CMySQLProc::Initizal(MYSQL_PROC_CALLBACK *mysql_proc)
{
	_mysql_proc=mysql_proc;	

	_ThreadUsing=true;
	for(DWORD i=0;i<_dwThreadNum;i++)
		CREATE_THREAD(_thread,this);
	return true;
}

void CMySQLProc::Destory()
{
	LOCK_CS(&_csSqlList);
	_ThreadUsing=false;	
	_lstSql.clear();
	UNLOCK_CS(&_csSqlList);
}

void CMySQLProc::_thread(CMySQLProc*p)
{
	TzhSqlInfo sql;
	CzhMySql mysql;
	DWORD tmpTimeCal; //时间计数
	
	//初始化参数
	p->lastExecuteTime=GetTickCount();

	while(p->_ThreadUsing)
	{
		tmpTimeCal=GetTickCount()-p->lastExecuteTime;
		//如果2分钟(120秒)后依然无任何动作,即关闭连接
		if(tmpTimeCal>120000)
		{
			//更新最后一次执行时间
			p->lastExecuteTime=GetTickCount();
			mysql.Close();
			//printf("free timeout disconnect from mysql\n");
		}

		LOCK_CS(&p->_csSqlList);
		if(p->_lstSql.size()<=0)
		{			
			UNLOCK_CS(&p->_csSqlList);
			Sleep(1);
			continue;
		}
		else
		{
			//更新最后一次执行时间
			p->lastExecuteTime=GetTickCount();
		}
		
		memset(&sql,0,sizeof(TzhSqlInfo));
		sql=*p->_lstSql.begin();
		p->_lstSql.pop_front();
		UNLOCK_CS(&p->_csSqlList);

		//取出数据处理,做处理
		if(mysql.HandleState())
		{
			//如果已经连接成功
			if(mysql.Query(sql.szSql))
			{
				//处理MYSQL数据
				p->_mysql_proc(ezhMysqlOperatSuccess,&mysql,&sql);
			}
			else
			{
				p->_mysql_proc(ezhMysqlOperatSqlFail,&mysql,&sql);				
			}
		}
		else
		{
			//连接没有连接就新建
			if(mysql.Connect(&p->_info))
			{
				if(mysql.Query(sql.szSql))
				{
					//处理MYSQL数据
					p->_mysql_proc(ezhMysqlOperatSuccess,&mysql,&sql);
				}
				else
				{
					p->_mysql_proc(ezhMysqlOperatSqlFail,&mysql,&sql);
				}
				
			}
			else
			{
				p->_mysql_proc(ezhMysqlOperatConnectFail,&mysql,&sql);
			}
		}
	}
}


void CMySQLProc::Query(void* pTarget,char *szSql)
{
	TzhSqlInfo sql;
	LOCK_CS(&_csSqlList);
	if(false==_ThreadUsing)
	{
		UNLOCK_CS(&_csSqlList);
		return;
	}

	if(0>=strlen(szSql))
	{
		UNLOCK_CS(&_csSqlList);
		return; 
	}

	sql.pTarget=pTarget;
	strcpy_s(sql.szSql,szSql);
		
	_lstSql.push_back(sql);
	UNLOCK_CS(&_csSqlList);
}