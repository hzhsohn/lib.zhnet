#pragma once
/** @defgroup GroupLength Length
 *  description.....
 *  <pre><b>Copyright (c) 2006 Game Carrier.  All Rights Reserved</b></pre> 
 *  @author  Zzg
 *  @date    17:5:2007   14:30
 *  @{
 */

//guid的最大长以，guid字符串序列
#define GCHMAC_GUID_LEN										32

#define GCHMAC_MAX_IP_LEN									16

#define GCHMAC_VERSION_NO_LEN								32

//字符串Md5码的最大长度
#define GCHMAC_MAX_MD5_STRING_LEN							32
//二进制Md5码的最大长度
#define GCHMAC_MAX_MD5_BIN_LEN								16

//名称的最大长度
#define GCHMAC_MAX_NAME_LEN									32
//密码的最大长度
#define GCHMAC_MAX_PASSWD_LEN								32
//! ServerName 最大长度
#define GCHMAC_SERVERNAME_LEN 								32
/** 最多能开的游戏房间数目 */								
#define GCHMAC_MAX_ROOM_NUM									2
/** 最大用户名和密码长度 */
#define GCHMAC_MAX_USERNAME_PASSWORD						32
/** 最大用户名和密码长度 */
#define GCHMAC_MAX_ROOMNAME									32
/** 休闲游戏根目录ID */
#define GCHMAC_GAMETREE_ROOTID								0
/** 休闲游戏用户最近游戏节点ID */
#define GCHMAC_GAMETREE_LOCALUSERGAMEID						1

//room
//房间中的最大用户数
#define	GCHMAC_MAX_USER_NUMBER_IN_ROOM						256
/**********************************************************************************************/
/** 用户信息宏定义*/
//用户名的最大长度
#define GCHMAC_MAX_USER_NAME_LEN		32
//用户昵称或签名的最大长度
#define GCHMAC_MAX_NICK_NAME_LEN		100
//密码的最大长度
#define GCHMAC_MAX_USER_PASSWD_LEN		32
//数字账号的最大长度
#define GCHMAC_MAX_NUMBER_LEN			16
//用户图标名的最大长度
#define GCHMAC_MAX_ICON_NAME_LEN		40
//用户描述的最大长度
#define GCHMAC_MAX_REMARK_LEN			200
/**********************************************************************************************/

/**********************************************************************************************/
/** 房间信息宏定义 */
#define GCHMAC_MAX_USER_IN_ROOM			300
#define GCHMAC_MAX_MSG_LEN				128
/**********************************************************************************************/

/**********************************************************************************************/
/** 所有关于数据库的定义 */
#define GCHMAC_MAX_DB_NAME_LEN					32
#define GCHMAC_MAX_DB_ACCOUNT_LEN				32
#define GCHMAC_MAX_DB_PASSWD_LEN					32
/**********************************************************************************************/

/**********************************************************************************************/
/** 配制文件每行的最大长度 */
#define GCHMAC_INI_LINE_LEN					256
/**********************************************************************************************/


/**********************************************************************************************/
//可以在游戏服务器使用的最大道具数量
#define GCHMAC_MAX_ITEM_COUNT				32
//用户玩过的最大游戏数
#define GCHMAC_MAX_USER_GAME_COUNT			32

#define  GCSMAC_MAX_ROOM_CNT				500			//一个游戏服务器最多允许带的房间数
/**********************************************************************************************/

//最大好友数
#define GCHMAC_MAX_FRIEND_COUNT				150

//语音服务器验证用户信息时扩展字段的长度
#define GCHMAC_MAX_NONCE_LEN				8
#define GCHMAC_MAX_ATTCH_LEN				32
#define GCHMAC_MAX_REALM_LEN				32
#define GCHMAC_MAX_RESPENSE_LEN				32

#define MAX_DATE_TIME_LEN					64

//一次返回的最多记录数
#define GCHMAC_MAX_MAX_RECORD_CNT			10

//包的最大长充
#define GCHMAC_PACKET_DATA_LEN_MAX			0x10000
//发送文件数据时，每次发送的最大长度
#define GCHMAC_MAX_PACKET_FILEDATA_LEN		1024

//错误描述的最大长度
#define GCHMAC_MAX_ERR_DESC_LEN				1024

//按钮标题的最大长度
#define GCHMAC_MAX_BTN_TITLEN_LEN			64