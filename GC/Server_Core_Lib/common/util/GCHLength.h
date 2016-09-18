#pragma once
/** @defgroup GroupLength Length
 *  description.....
 *  <pre><b>Copyright (c) 2006 Game Carrier.  All Rights Reserved</b></pre> 
 *  @author  Zzg
 *  @date    17:5:2007   14:30
 *  @{
 */

//guid������ԣ�guid�ַ�������
#define GCHMAC_GUID_LEN										32

#define GCHMAC_MAX_IP_LEN									16

#define GCHMAC_VERSION_NO_LEN								32

//�ַ���Md5�����󳤶�
#define GCHMAC_MAX_MD5_STRING_LEN							32
//������Md5�����󳤶�
#define GCHMAC_MAX_MD5_BIN_LEN								16

//���Ƶ���󳤶�
#define GCHMAC_MAX_NAME_LEN									32
//�������󳤶�
#define GCHMAC_MAX_PASSWD_LEN								32
//! ServerName ��󳤶�
#define GCHMAC_SERVERNAME_LEN 								32
/** ����ܿ�����Ϸ������Ŀ */								
#define GCHMAC_MAX_ROOM_NUM									2
/** ����û��������볤�� */
#define GCHMAC_MAX_USERNAME_PASSWORD						32
/** ����û��������볤�� */
#define GCHMAC_MAX_ROOMNAME									32
/** ������Ϸ��Ŀ¼ID */
#define GCHMAC_GAMETREE_ROOTID								0
/** ������Ϸ�û������Ϸ�ڵ�ID */
#define GCHMAC_GAMETREE_LOCALUSERGAMEID						1

//room
//�����е�����û���
#define	GCHMAC_MAX_USER_NUMBER_IN_ROOM						256
/**********************************************************************************************/
/** �û���Ϣ�궨��*/
//�û�������󳤶�
#define GCHMAC_MAX_USER_NAME_LEN		32
//�û��ǳƻ�ǩ������󳤶�
#define GCHMAC_MAX_NICK_NAME_LEN		100
//�������󳤶�
#define GCHMAC_MAX_USER_PASSWD_LEN		32
//�����˺ŵ���󳤶�
#define GCHMAC_MAX_NUMBER_LEN			16
//�û�ͼ��������󳤶�
#define GCHMAC_MAX_ICON_NAME_LEN		40
//�û���������󳤶�
#define GCHMAC_MAX_REMARK_LEN			200
/**********************************************************************************************/

/**********************************************************************************************/
/** ������Ϣ�궨�� */
#define GCHMAC_MAX_USER_IN_ROOM			300
#define GCHMAC_MAX_MSG_LEN				128
/**********************************************************************************************/

/**********************************************************************************************/
/** ���й������ݿ�Ķ��� */
#define GCHMAC_MAX_DB_NAME_LEN					32
#define GCHMAC_MAX_DB_ACCOUNT_LEN				32
#define GCHMAC_MAX_DB_PASSWD_LEN					32
/**********************************************************************************************/

/**********************************************************************************************/
/** �����ļ�ÿ�е���󳤶� */
#define GCHMAC_INI_LINE_LEN					256
/**********************************************************************************************/


/**********************************************************************************************/
//��������Ϸ������ʹ�õ�����������
#define GCHMAC_MAX_ITEM_COUNT				32
//�û�����������Ϸ��
#define GCHMAC_MAX_USER_GAME_COUNT			32

#define  GCSMAC_MAX_ROOM_CNT				500			//һ����Ϸ���������������ķ�����
/**********************************************************************************************/

//��������
#define GCHMAC_MAX_FRIEND_COUNT				150

//������������֤�û���Ϣʱ��չ�ֶεĳ���
#define GCHMAC_MAX_NONCE_LEN				8
#define GCHMAC_MAX_ATTCH_LEN				32
#define GCHMAC_MAX_REALM_LEN				32
#define GCHMAC_MAX_RESPENSE_LEN				32

#define MAX_DATE_TIME_LEN					64

//һ�η��ص�����¼��
#define GCHMAC_MAX_MAX_RECORD_CNT			10

//������󳤳�
#define GCHMAC_PACKET_DATA_LEN_MAX			0x10000
//�����ļ�����ʱ��ÿ�η��͵���󳤶�
#define GCHMAC_MAX_PACKET_FILEDATA_LEN		1024

//������������󳤶�
#define GCHMAC_MAX_ERR_DESC_LEN				1024

//��ť�������󳤶�
#define GCHMAC_MAX_BTN_TITLEN_LEN			64