#include "ProtocolData.hh"
#include "proc.hh"
#include <handy.h>

extern Proc g_proc;
extern map<unsigned long, TagInfo> g_users;
extern map<unsigned int, TzhBusinessConInfo> g_businessServList;
extern int g_servID;
extern int g_servBindPort;
extern int g_maxConn;


void ProtocolData::initProtocolData()
{
 //DEBUG_PRINTF("ProtocolData::initProtocolData");
}

void ProtocolData::userSignToServ(const char* conn_flag,
                     int servID,
							unsigned int session_id)
{
  char bf[12]={0};
	int bfsize=9;
  int bflen=0;
  
    //DEBUG_PRINTF("ProtocolData::bfsize %d",bfsize);
   // DEBUG_PRINTF("ProtocolData::userSignToServ");
    
	memset(&bf[bflen],0,bfsize);
	//事件标识--
	bf[bflen]='A';
	bflen++;
	//服务器标识--
	memcpy(&bf[bflen],&servID,4);
	bflen+=4;
	//通讯ID--
	memcpy(&bf[bflen],&session_id,4);
	bflen+=4;
	//送到服务器--
  for(auto it=g_businessServList.begin();it!=g_businessServList.end();it++)
    {
        if(!it->second.serv_conn_flag.compare(conn_flag))
        {
          businessSend(it->second.con,bf,bflen);
        }
    }
    
}

void ProtocolData::userDataToServ(const char* conn_flag,
                     int servID,
							unsigned int session_id,
							const char* data,int len)
{
	int bfsize=len+9;
	int bflen=0;
  char *_bf=(char*)malloc(bfsize+1);
    //DEBUG_PRINTF("ProtocolData::bfsize %d",bfsize);
   // DEBUG_PRINTF("ProtocolData::userDataToServ");
  
	memset(_bf,0,bfsize);
	//事件标识--
	_bf[bflen]='B';
	bflen++;
	//服务器标识--
	memcpy(&_bf[bflen],&servID,4);
	bflen+=4;
	//通讯ID--
	memcpy(&_bf[bflen],&session_id,4);
	bflen+=4;
	//
	memcpy(&_bf[bflen],data,len);
	bflen+=len;
  //
  for(auto it=g_businessServList.begin();it!=g_businessServList.end();it++)
    {
        if(!it->second.serv_conn_flag.compare(conn_flag))
        {
          businessSend(it->second.con,_bf,bflen);
        }
    }
    
    
  free(_bf);
  _bf=NULL;
}

void ProtocolData::userDisconnectToServ(const char* conn_flag,
                     int servID,
									unsigned int session_id)
{
  char bf[12]={0};
	int bfsize=9;
  int bflen=0;

    //DEBUG_PRINTF("ProtocolData::bfsize %d,_bfsize %d",bfsize,_bfsize);
    //DEBUG_PRINTF("ProtocolData::userDisconnectToServ");

	memset(&bf[bflen],0,bfsize);
	//事件标识--
	bf[bflen]='C';
	bflen++;
	//服务器标识--
	memcpy(&bf[bflen],&servID,4);
	bflen+=4;
	//通讯ID
	memcpy(&bf[bflen],&session_id,4);
	bflen+=4;
  //
  for(auto it=g_businessServList.begin();it!=g_businessServList.end();it++)
    {
        if(!it->second.serv_conn_flag.compare(conn_flag))
        {
          businessSend(it->second.con,bf,bflen);
        }
    }
}

