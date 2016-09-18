#include "StdAfx.h"
#include "GCSNetWork.h"
#include "StringUtil.h"
#include "Userlist.h"
#include "GCSetPack.h"

extern CMySQLProc	g_MySql;

extern Userlist	g_userList;

//���տͻ��˵���������Ļص�����
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
//�Ͽ��ͻ������ӵĻص�����
void WINAPI DissconnectCallBack(HANDLE handle)
{	
	//ֱ���˳�,�����������
	setlog("DissconnectCallBack-->handle=%p", handle);
	g_userList.removeUser(handle);
	return;
}

//���յ����ݰ�ʱ�Ļص�����
void WINAPI RecvDataCallBack(HANDLE handle, int nLen, char* pData)
{	
	//��ȡ����
	short cmd;
	memcpy_s(&cmd,2,pData,2);
	
	switch (cmd)
	{		
	case 1: //��ȡ����ת����������ַ
		{
			//����+�˺������ַ���
			char account[256]={0};
			strcpy_s(account,&pData[2]);
			if(0==strnlen_s(account,sizeof(account)))
			{
				//�˺�Ϊ��,��ִ��
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
			//�������MYSQL������
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
