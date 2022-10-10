#include "proc.hh"
#include "micro.h"
#include "print_msg.h"
#include "findfile.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include "ProtocolData.hh"
#include "session.h"

using namespace std;

//生命周期比连接更长，必须放在Base前--
map<unsigned int, TzhBusinessConInfo> g_businessServList;


struct TagRecvToClient
{
  unsigned int sessionID;
  char* bf;
  int bfLen;
};
struct TagUserToBusiness
{
  string flag;
  unsigned int serverID;
  unsigned int sessionID;
  char* bf;
  int bfLen;
};
list<TagRecvToClient> g_listRecvToCli;
list<TagUserToBusiness> g_listUserToBusin;

//服务器信息-----------
//是否已经获取服务器ID-
int g_servID=0;
int g_maxConn=0;
int g_servBindPort=0;
//
int g_clientID = 0;
int g_userConnID = 0;
//---------------------------
void businessState(const TcpConnPtr& con);
void businessRead(const TcpConnPtr& con);

void handyState(const TcpConnPtr& con);
void handyRead(const TcpConnPtr& con);

void zhRecv(const TcpConnPtr& con ,const char* data,int len);
void zhDisconnect(const TcpConnPtr& con );

//--------------------------------
Proc::Proc()
{
	//setloglevel("TRACE");
	//TCP服务器信息--
	srand(time(NULL));
	g_maxConn=100000;
	g_servBindPort=6200;
	//
	ProtocolData::initProtocolData();
}


//bussiness connected service --
void Proc::Init()
{
    DEBUG_PRINTF("---------------------------------------------");
    DEBUG_PRINTF("-- ");
    DEBUG_PRINTF("-- dtrs transmis service");
    DEBUG_PRINTF("-- version 1.0 ");
    DEBUG_PRINTF("-- ");
    DEBUG_PRINTF("---------------------------------------------");
    
    EventBase base;
    Signal::signal(SIGINT, [&]{ base.exit(); });
    
    //----------------------------
    //业务服务器对接--
    TcpServerPtr svr = TcpServer::startServer(&base, "", 6696);
    exitif(svr == NULL, "start tcp server failed");

    svr->onConnState(businessState);
    svr->onConnRead(businessRead);
    
    TimerId tid =base.runAfter(0, [](){
        for(auto it=g_businessServList.begin();it!=g_businessServList.end();it++)
        {
            if(FALSE==it->second.isFree)
            {
                //20秒内收不到任何保活包就断开--
                time_t dwTmp=zhPlatGetTime()-it->second.dwLastKeepTme;
                //DEBUG_PRINTF("check business.count=%d ,serv_conn_flag=%s last keep = %d",
                //                  g_businessServList.size(),it->second.serv_conn_flag.c_str(),dwTmp);
                if(dwTmp >20000)
                {
                    it->second.dwLastKeepTme=zhPlatGetTime();
                    it->second.isFree=TRUE;
                    DEBUG_PRINTF("disconnect business server\n");
                }
            }
        }
        
        for(auto it=g_businessServList.begin();
                  it!=g_businessServList.end();)
        {
            if(it->second.isFree)
            {
                it->second.con->close();
                // 清空接收缓存--
                it->second.con->getOutput().clear();   
                it->second.con->getInput().clear();
                it=g_businessServList.erase(it); 
                DEBUG_PRINTF("disconnect businessServList.count=%d" ,g_businessServList.size()); 
            }
            else
            {it++;}
        }
        
        //发送链表数据到业务服务器--
        for(auto it=g_listUserToBusin.begin();
                it!=g_listUserToBusin.end();)
        {        
            ProtocolData::userDataToServ(	it->flag.c_str(),it->serverID,it->sessionID,it->bf,it->bfLen);
            it=g_listUserToBusin.erase(it);
        }
    }, 10);
 
    //--------------
    base.loop();
    
}

//------------------------------------------

void zhRecv(const TcpConnPtr& con ,const char* data,int len)
{
    //DEBUG_PRINTF("zhRecv len=%d->",len);
    //zhPlatPrint16(len,(char*)data);
		
		unsigned char cmd=data[0];
    char*pdata=(char*)&data[1];
    switch(cmd)
    {
      case 'E'://E 与业务服务器的保活包--
      {
            TzhBusinessConInfo* p=(TzhBusinessConInfo*)&g_businessServList[con->context<int>()];
            memcpy(&g_servID,&pdata[0],4);          
            p->serv_conn_flag=&pdata[4];
            p->dwLastKeepTme=zhPlatGetTime();
            DEBUG_PRINTF("keep server_id=%d flag=%s",g_servID,p->serv_conn_flag.c_str());
            //回复--
            TzhPacket sp;
            zhPackWriteInit(&sp);
            zhPackWriteChar(&sp,'D');
            zhPackWriteUnsignedShort(&sp,g_servBindPort);
            zhPackWriteInt(&sp,g_maxConn);
            zhPackWriteInt(&sp,g_users.size());
            businessSend(p->con,(char*)sp.btBuf,sp.wSize);
      }
      break;
      case 'F'://F 业务层给数据客户端内容--
      {
        //4字节会话ID--
        //数据内容--
        int pos=0;
        int dlen=0;
        unsigned int asession_id=0;
        memcpy(&asession_id,&pdata[pos],4);
        pos+=4;
        //DEBUG_PRINTF("RECV F----------g_users.count=%d ",g_users.size());
        //将数据转发到客户客户端--
        dlen=len-pos-1;
        //放到链表后再转发--
        
        DEBUG_PRINTF("F to user[%d], session_id=%d, trans data = %d",con->context<int>(),asession_id,dlen);
        TagRecvToClient c;
        c.sessionID=asession_id;
        c.bf=(char*)malloc(dlen);
        memcpy(c.bf,&pdata[pos],dlen);
        c.bfLen=dlen;
        g_listRecvToCli.push_back(c);
      }
        break;
      case 'G'://G 让服务器断开客户端--
      {
        //4字节会话ID--
        int pos=0;
        unsigned int asession_id=0;
        memcpy(&asession_id,&pdata[pos],4);
        //
        //DEBUG_PRINTF("RECV G----------g_users.count=%d ",g_users.size());
        //
        auto it=g_users.find(asession_id);
        if(it!=g_users.end())
        {
          DEBUG_PRINTF("G to user[%d] disconnect",asession_id);
          //断开连接--
          it->second.isSign=FALSE;
          it->second.con->close();
        }
      }
        break;
      default:
      {
        DEBUG_PRINTF("ProtocolData::dataToClient unknow command length=%d",len);
      }
        break;
    }
}
void zhDisconnect(const TcpConnPtr& con )
{
    DEBUG_PRINTF("disconnect dtrs server");
}

int businessSend(TcpConnPtr con,char* szPack,int nLen)
{
	int nSendLen=0;
	TzhPackFrameHeader pktHeader={0};
	unsigned char* sendBuf=NULL;
	unsigned short tmp=0;

	nSendLen=sizeof(TzhPackFrameHeader)+nLen;
	sendBuf=(unsigned char*)malloc(nSendLen);

	//++++++++++++++++++
	//包头数据
	pktHeader.yFrameFlag=ZH_NET_FRAME_FLAG;
	pktHeader.wDataLenL1=nLen&0xFF;
	pktHeader.wDataLenL2=nLen>>8;
	tmp=zhPlatCRC16((unsigned char*)szPack,nLen);
	pktHeader.wCRC16L1=tmp&0xFF;
	pktHeader.wCRC16L2=tmp>>8;

	//-----------------
	memcpy(sendBuf,&pktHeader,sizeof(TzhPackFrameHeader));
	memcpy(&sendBuf[sizeof(TzhPackFrameHeader)],szPack,nLen);

	//zhPlatPrint16(4,&sion->nEncryptKey);
	//添加到发送列表--
	con->send((char*)sendBuf,nSendLen);
	
	free(sendBuf);
	sendBuf=NULL;
	return nLen;
}

void businessState(const TcpConnPtr& con)
{
  if (con->getState() == TcpConn::Connected) {    
          
          if(g_businessServList.size()>20)
          {
            con->close();
            return;
          }
          //生成用户ID--
          while(1)
          {
              if(0==g_clientID)
              {g_clientID=1;}
              if (g_businessServList.find(g_clientID) == g_businessServList.end())
              {break;}
              else
              {g_clientID++; zhPlatSleep(1);}
          }
          //连接初始化--
          con->context<int>() = g_clientID;
          //
          TzhBusinessConInfo pinfo;
          pinfo.isFree=FALSE;
          pinfo.con=con;
          pinfo.dwLastKeepTme=zhPlatGetTime();
          g_businessServList[con->context<int>()] = pinfo;
          DEBUG_PRINTF("new business id=%d, businessServList.count=%d" ,con->context<int>(), g_businessServList.size());
          
          //zhNet反回4个0x00字节--
          char sbuf[4]={0,0,0,0};
          con->send(sbuf,4); 
          
    } else if (con->getState() == TcpConn::Closed) {
    
          auto it=g_businessServList.find(con->context<int>());
          it->second.isFree=TRUE;  
    }
}

void businessRead(const TcpConnPtr& con) 
{
    TzhBusinessConInfo* pci=(TzhBusinessConInfo*)&g_businessServList[con->context<int>()];
    char *pData=con->getInput().data();    
		
    /*DEBUG_PRINTF("businessRead userid=%d  dtrsCacheLen=%d  datalen=%d   totol=%d",
                  con->context<int>(),
                  pci->dtrsCacheLen ,
                  con->getInput().size(),
                  (pci->dtrsCacheLen + con->getInput().size()));*/
    //zhPlatPrint16(con->getInput().size(),pData);
    
    if((pci->dtrsCacheLen + con->getInput().size()) < sizeof(pci->dtrsCache))
    {
        memcpy(&pci->dtrsCache[pci->dtrsCacheLen],pData,con->getInput().size());
        pci->dtrsCacheLen += con->getInput().size();
    }
    else
    {
        DEBUG_PRINTF("businessRead data too long to clear userid=%d",con->context<int>());
        pci->dtrsCacheLen=0;
    }    
    con->getInput().clear(); 
    
    while(1)
    {
        //DEBUG_PRINTF("businessRead proc packet userid=%d",con->context<int>());
        if(pci->dtrsCacheLen >= sizeof(TzhPackFrameHeader))
        {
            //判断第一个字节是否为标识--
            if(pci->dtrsCache[0]!=ZH_NET_FRAME_FLAG)
            {
                int findPos=0;
                
                //寻找包开始的字节--
                while(findPos < pci->dtrsCacheLen)
                {
                  if(ZH_NET_FRAME_FLAG==pci->dtrsCache[findPos])
                  {break;} 
                  findPos++;
                }
                pci->dtrsCacheLen-=findPos;
                if(pci->dtrsCacheLen>0)
                {
                  memmove(pci->dtrsCache,&pci->dtrsCache[findPos],pci->dtrsCacheLen);
                }
            }
            //正确的数据帧开头--
            TzhPackFrameHeader packetHeader;
            unsigned short wDataLen=0,wCRC16=0;
            memcpy(&packetHeader,pci->dtrsCache,sizeof(TzhPackFrameHeader));
            //
            wDataLen=packetHeader.wDataLenL1|packetHeader.wDataLenL2<<8;
            wCRC16=packetHeader.wCRC16L1|packetHeader.wCRC16L2<<8;
            //
            //数据长度是否足够--
            int nTmpPos1=sizeof(TzhPackFrameHeader)+wDataLen;
            if(pci->dtrsCacheLen >= nTmpPos1)
            {
                //验证数据帧--
                int wCRCTmp=zhPlatCRC16((unsigned char*)&pci->dtrsCache[sizeof(TzhPackFrameHeader)],wDataLen);
                if(wCRCTmp==wCRC16)
                {
                  //数据正常--
                  zhRecv(con , &pci->dtrsCache[sizeof(TzhPackFrameHeader)], nTmpPos1-sizeof(TzhPackFrameHeader));
                  pci->dtrsCacheLen -= nTmpPos1;
                  if(pci->dtrsCacheLen > 0)
                  {
                    memmove(pci->dtrsCache,&pci->dtrsCache[nTmpPos1],pci->dtrsCacheLen);
                  }
                }
                else
                {
                  //数据异常--
                  //将首字节向前移--
                  memmove(pci->dtrsCache,&pci->dtrsCache[1],pci->dtrsCacheLen-1);
                  DEBUG_PRINTF("warning: pci->dtrsCache frame check warning");   
                }
            }
        }
        else
        {
          //数据已经低于数据帧,退出数据处理循环--
          break;
        }
		}
    
}




