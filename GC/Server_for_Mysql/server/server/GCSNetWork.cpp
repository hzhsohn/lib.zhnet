#include "StdAfx.h"
#include "GCSNetWork.h"
#include "StringUtil.h"
#include "Userlist.h"
#include "GCSetPack.h"

extern CMySQLProc	g_MySql;

extern Userlist	g_userList;

//接收客户端的连接请求的回调函数
void WINAPI AcceptCallBack(HANDLE handle, TCHAR *pszIP, WORD wPort)
{
	setlog("AcceptCallBack-->handle=%p, pszIP=%s, wPort=%04d", handle, pszIP, wPort);	
	/*
	TzhUser usr={0};
	usr.h=handle;
	usr.my_username="";
	g_userList.addUser(handle,usr);
	*/
	return;
}
//断开客户端连接的回调函数
void WINAPI DissconnectCallBack(HANDLE handle)
{	
	//直接退出,或放入重链表
	setlog("DissconnectCallBack-->handle=%p", handle);
	g_userList.removeUser(handle);
	return;
}

//接收到数据包时的回调函数
void WINAPI RecvDataCallBack(HANDLE handle, int nLen, char* pData)
{	
	//获取命令
	short cmd;
	memcpy_s(&cmd,2,pData,2);
	
	switch (cmd)
	{		
	case 1: //获取最优转发服务器地址
		{
			//命令+账号名称字符串
			char account[256]={0};
			strcpy_s(account,&pData[2]);
			if(0==strnlen_s(account,sizeof(account)))
			{
				//账号为空,不执行
				return;
			}
			GCSetPack pack;
			CString sql;
			sql.Format("call sp_dev_get_addr('%s')",account);
			g_MySql.Query(handle,sql.GetBuffer());
		}
		break;
	default:
		break;
	}
}

void ProcMysql(EzhMysqlOperator result,CzhMySql *mysql,TzhSqlInfo*info)
{
	switch (result)
	{
	case ezhMysqlOperatSuccess:
		{
			/*
			//处理各种MYSQL的数据
			TCHAR ip[100];
			GCEApiSRTNetGetPeerIP(info->handle,ip);
			GSRSend(info->handle, info->wCmd, sizeof(ip), (char*)ip);
			;*/
			GCSetPack pack;
			pack.beginWrite();
			pack.writeBool(true);
			HANDLE handle=info->pTarget;
			GSRSend(handle,pack.getPackSize(), pack.getPackBuf());

			while(mysql->GetRow())
			{
				setlog("%s",_S2WS(mysql->GetField("dev_uuid")));
			}
		}
		break;
	case ezhMysqlOperatSqlFail:
		{
			setlog("ezhMysqlOperatSqlFail sql=%s",info->szSql);
		}
		break;
	case ezhMysqlOperatConnectFail:
		{
			TzhMysqlInfo* p=mysql->getInfo();
			errlog("ezhMysqlOperatConnectFail local=%s port=%d db=%s user=%s password=%s",				
				p->host.c_str(),p->port,p->db.c_str(),p->name.c_str(),p->pwd.c_str());
		}
		break;
	}
}
