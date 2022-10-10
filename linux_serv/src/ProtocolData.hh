
#ifndef _ZH_TO_CENTER_SERV_H_

#include <stdio.h>
#include <handy.h>
#include "micro.h"

using namespace std;
using namespace handy;


struct TzhBusinessConInfo{
  
  TcpConnPtr  con;
  BOOL isFree;
  
  char dtrsCache[3000];
  int dtrsCacheLen;

  //transfrom device flag--
  string serv_conn_flag;

  //last keep time--
  time_t dwLastKeepTme;

  TzhBusinessConInfo()
  {
    serv_conn_flag="";
    dtrsCacheLen=0;
    dwLastKeepTme=0;
  }
};

int businessSend(TcpConnPtr con,char* szPack,int nLen);

class ProtocolData
{
public:
  static void initProtocolData();
	//
	static void userSignToServ(const char* conn_flag,int servID,unsigned int session_id);
	static void userDataToServ(const char* conn_flag,int servID,unsigned int session_id,const char* data,int len);
	static void userDisconnectToServ(const char* conn_flag,int servID,unsigned int session_id);

};

#define _ZH_TO_CENTER_SERV_H_
#endif

