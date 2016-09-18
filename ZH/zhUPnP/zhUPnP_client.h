
#ifndef __ZH_C_NET_UPNP_H__
#define __ZH_C_NET_UPNP_H__

#include "socket.h"


#ifdef __cplusplus
extern "C"{
#endif

//端口的结构信息
typedef struct _TzhUPnP_PortMapInfo
{
    char ExternalIP[16];
    unsigned short ExternalPort;
    int Protocol;//0:TCP 1:UDP
    char InternalIP[16];
    unsigned short InternalPort;
    int Enabled;//
    char Description[64];
    long Duration;
}TzhUPnP_PortMapInfo;


//查找是否存在UPNP设备
bool zhUPnPFindDevice();

//获取设备描述
bool zhUPnPGetDeviceDescription();

//获取外网已经存在的地址映射
int zhUPnPGetExternalIPAddress();

//判断网关设备是否正常
bool zhUPnPIsGatewayOK();
//获取IP信息
char* zhUPnPGetExternalIP();
char* zhUPnPGetDeviceIP();

/*
添加一个映射
	ExternalPort 外网端口
	PortMappingProtocal 映射的协议 ,填"TCP" 或 "UDP"一定要大写 
	InternalPort 内网端口
	InternalClient 内网映射的主机地址
	PortMappingDescription 映射信息描述
	PortMappingEnable 端口映射启用状态 默认为 1
	PortMappingLeaseDuration 默认为 0
*/
bool zhUPnPAddPortMapping(unsigned short ExternalPort,
                        const char *PortMappingProtocal,
                        unsigned short InternalPort,
                        const char *InternalClient,
                        const char *PortMappingDescription,
                        int PortMappingEnable,
                        unsigned long PortMappingLeaseDuration);

/*
删除一个映射
	ExternalPort 外网端口
	PortMappingProtocal 映射的协议 ,填"TCP" 或 "UDP"一定要大写
*/
bool zhUPnPDeletePortMapping(unsigned short ExternalPort,
                           const char *PortMappingProtocal);

//获取映射端口的信息
bool zhUPnPGetPortMappingInfo( int PortMappingIndex, TzhUPnP_PortMapInfo *info);


/*
 * 不区分大小写查找字符
*/
char *zhUPnPStrstri(const char *phaystack, const char *pneedle);

#ifdef __cplusplus
}
#endif
#endif
