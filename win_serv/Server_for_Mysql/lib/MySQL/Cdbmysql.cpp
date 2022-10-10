/*
g_DBMySql.cpp: implementation of the CzhMySql class.
*/
#include "StdAfx.h"
#include "Cdbmysql.h"

CzhMySql::CzhMySql()
{
	m_handle = NULL;
	m_res = NULL;
	SetLog(true);
}

CzhMySql::~CzhMySql()
{
	Close();
}

bool CzhMySql::Connect(const char *host, int port,const char *name,const char *pwd,const char *db)
{
	m_dbinfo.host = host;
	m_dbinfo.port = port;
	m_dbinfo.name = name;
	m_dbinfo.pwd = pwd;
	m_dbinfo.db = db;

	if (m_dbinfo.port == 0) 
		m_dbinfo.port = MYSQL_PORT;

	return Connect();
}

bool  CzhMySql::Connect(TzhMysqlInfo* info)
{
	m_dbinfo.host = info->host;
	m_dbinfo.port = info->port;
	m_dbinfo.name = info->name;
	m_dbinfo.pwd = info->pwd;
	m_dbinfo.db = info->db;

	if (m_dbinfo.port == 0) 
		m_dbinfo.port = MYSQL_PORT;

	return Connect();
}

bool CzhMySql::Connect()
{
	m_handle = mysql_init(NULL);
	if (m_handle==NULL) return false;
	
	if (mysql_real_connect(m_handle, m_dbinfo.host.c_str(), m_dbinfo.name.c_str(), m_dbinfo.pwd.c_str(), NULL, m_dbinfo.port, NULL, CLIENT_MULTI_STATEMENTS) == NULL)
	{
		ShowError();
		return false;
	}
	if (mysql_select_db(m_handle, m_dbinfo.db.c_str()) != 0) 
	{
		ShowError();
		return false;
	}
	return true;
}
bool CzhMySql::Close()
{
	if (m_handle)
	{
		mysql_close(m_handle);
		m_handle = NULL;
	}
	if (m_res)
	{
		mysql_free_result(m_res);
		m_res = NULL;
	}

	return true;
}

bool CzhMySql::Query(const char *sql,int len)
{
	if (len == 0) 
		len = (int)strlen(sql);

	short i = 3;
	while(i--)
	{
		if (m_handle == NULL && !Connect())
			continue;
		
		if (mysql_real_query(m_handle, sql, len) != 0) 
		{
			ShowError();
			continue;
		}
		if (m_res != NULL) 
		{
			mysql_free_result(m_res);
			m_res=NULL;
		}
		m_res = mysql_store_result(m_handle);
		if (m_res != NULL)
		{
			m_fieldCnt = mysql_num_fields(m_res);
			m_fields = mysql_fetch_fields(m_res);
		}
		return true;
	}
	printf("CzhMySql query error!!!\n");
	return false;
}

int CzhMySql::GetFieldCount()
{
	return m_fieldCnt;
}

int CzhMySql::GetRowCount()
{
	return (int)mysql_num_rows(m_res);
}

char* CzhMySql::ShowError()
{
	if(!m_handle)return NULL;
	if(m_log)
	{
		sprintf(m_err,"SQL EER = [%s]", mysql_error(m_handle));
		setlog("%s",m_err);
		Close();
	}
	return m_err;
}

bool CzhMySql::GetRow()
{
	m_row = mysql_fetch_row(m_res);
	if (m_row == NULL) return false;
	return true;
}

char *CzhMySql::GetField(char *fname, int *pnLen)
{
	int i;
	unsigned long *lengths;
	lengths = mysql_fetch_lengths(m_res);
	for(i = 0; i < m_fieldCnt; i++)
	{
		if (strcmp(fname,m_fields[i].name)==0)
		{
			if(pnLen) *pnLen = lengths[i];
			return m_row[i];
		}
	}
	return NULL;
}

//-------------------------------------
// Get Field Name , Data Type And Value
//-------------------------------------
bool CzhMySql::GetField(int idx,char *field,int *type,char **value)
{
	if(idx<0 || idx>m_fieldCnt)
		return false;
	strcpy(field,m_fields[idx].name);
	*type = m_fields[idx].type;
	*value = m_row[idx];
	return true;
}

//------------------------
// Get Field Name And Type
//------------------------
int CzhMySql::GetFieldInfo(int nIdx, char *pszName)
{
	if(nIdx<0 || nIdx>=m_fieldCnt)
    {
        return 0;
    }
	if(pszName)
    {
        strcpy(pszName, m_fields[nIdx].name);
    }
	return m_fields[nIdx].type;
}

//------------------------------
// Get Field Data Type And Value
//------------------------------
bool CzhMySql::GetFieldContent(int nIdx , int *pnType , char **ppValue)
{
	if(nIdx<0 || nIdx>=m_fieldCnt)
    {
        return false;
    }
	*pnType  = m_fields[nIdx].type;
	*ppValue = m_row[nIdx];
    return true;
}

unsigned long CzhMySql::GetInsertId()
{
	return (unsigned long)mysql_insert_id(m_handle);
}

long CzhMySql::GetAffectedRows()
{
	return (unsigned long)mysql_affected_rows(m_handle);
}

void CzhMySql::SetLog(bool log)
{
	m_log = log;
}


bool CzhMySql::HandleState()
{
	return m_handle?true:false;
}

TzhMysqlInfo* CzhMySql::getInfo()
{
	return &m_dbinfo;
}

void CzhMySql::freeQueryRes()
{
	if(m_handle)
	{
		do 
		{ 
			m_res = mysql_store_result( m_handle ); 
			mysql_free_result(m_res); 
		}while( !mysql_next_result( m_handle ) );
	}
}