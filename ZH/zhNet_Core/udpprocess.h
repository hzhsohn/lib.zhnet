/*
  udpprocess.h - udp transmission verify moudle
*/

/* Copyright (C) 2008-2, Han.zhihong, Developer. Created 2008. All
rights reserved.

  License to copy and use this software is granted provided that it
  is identified as the "Han.zhihong. Message-Digest
  Algorithm" in all material mentioning or referencing this software
  or this function.
  
  Han.zhihong. makes no representations concerning either
  the merchantability of this software or the suitability of this
  software for any particular purpose. It is provided "as is"
  without express or implied warranty of any kind.
	  
  These notices must be retained in any copies of any part of this
  documentation and/or software.
*/


#ifndef __C_ZH_NET_UDP_SOCKET_H__
#include "socket.h"
#include "packet.h"

#ifdef __cplusplus
extern "C"{
#endif

/*
 *udp sendto function,the packet not more than 1024B to well 
 *
 *��Ϊ��UDP����,����ÿ������Ҫ����1024Byte,Ϊ���
 *zhUdpSendTo ԭʼ���ݸ�ʽ: ���ֽ�CRC16У��+��������
 *
 *����ֵΪ���͵��ֽ�
 */
int zhUdpSendToPack(SOCKET s,TzhPacket *pack,struct sockaddr_in *addr);
int zhUdpSendTo(SOCKET s,unsigned char *buf,int len,struct sockaddr_in *addr);

/*
 *udp recvfrom function
 *
 *����UDP���ݺ���
 *zhUdpRecvFrom ԭʼ���ݸ�ʽ:����+���ֽ�CRC16У��
 */
int zhUdpRecvFrom(SOCKET s,unsigned char *buf,int len,struct sockaddr_in *addr,int *addrlen);

/*
	��ַת������
*/
#define zhUdpAddrToPram		zhSockAddrToPram
#define zhUdpPramToAddr		zhSockPramToAddr

#ifdef __cplusplus
}
#endif
#define __C_ZH_NET_UDP_SOCKET_H__
#endif
