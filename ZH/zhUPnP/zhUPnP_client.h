
#ifndef __ZH_C_NET_UPNP_H__
#define __ZH_C_NET_UPNP_H__

#include "socket.h"


#ifdef __cplusplus
extern "C"{
#endif

//�˿ڵĽṹ��Ϣ
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


//�����Ƿ����UPNP�豸
bool zhUPnPFindDevice();

//��ȡ�豸����
bool zhUPnPGetDeviceDescription();

//��ȡ�����Ѿ����ڵĵ�ַӳ��
int zhUPnPGetExternalIPAddress();

//�ж������豸�Ƿ�����
bool zhUPnPIsGatewayOK();
//��ȡIP��Ϣ
char* zhUPnPGetExternalIP();
char* zhUPnPGetDeviceIP();

/*
���һ��ӳ��
	ExternalPort �����˿�
	PortMappingProtocal ӳ���Э�� ,��"TCP" �� "UDP"һ��Ҫ��д 
	InternalPort �����˿�
	InternalClient ����ӳ���������ַ
	PortMappingDescription ӳ����Ϣ����
	PortMappingEnable �˿�ӳ������״̬ Ĭ��Ϊ 1
	PortMappingLeaseDuration Ĭ��Ϊ 0
*/
bool zhUPnPAddPortMapping(unsigned short ExternalPort,
                        const char *PortMappingProtocal,
                        unsigned short InternalPort,
                        const char *InternalClient,
                        const char *PortMappingDescription,
                        int PortMappingEnable,
                        unsigned long PortMappingLeaseDuration);

/*
ɾ��һ��ӳ��
	ExternalPort �����˿�
	PortMappingProtocal ӳ���Э�� ,��"TCP" �� "UDP"һ��Ҫ��д
*/
bool zhUPnPDeletePortMapping(unsigned short ExternalPort,
                           const char *PortMappingProtocal);

//��ȡӳ��˿ڵ���Ϣ
bool zhUPnPGetPortMappingInfo( int PortMappingIndex, TzhUPnP_PortMapInfo *info);


/*
 * �����ִ�Сд�����ַ�
*/
char *zhUPnPStrstri(const char *phaystack, const char *pneedle);

#ifdef __cplusplus
}
#endif
#endif
