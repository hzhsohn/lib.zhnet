/*
* Copyright (c) 2004, game carrier
* All rights reserved.

* Filename: GCSRTNetS.h
* Description:加密时不改变数据原始长度

* Author:bloodspider
* Time:2007-4-19   22:52
* Modify connect:

*/

#pragma once

#include "GCHUtilMarco.h"
#include "wtypes.h"



//初始化未获取钥匙前默认秘密
DLLEXPORT_MULT int GCApiGetDefaultKey();

//取得加密密钥
DLLEXPORT_MULT int GCApiGetEncryptKey();

//加密一块数据区
DLLEXPORT_MULT BOOL GCApiEncrypt(BOOL isVariFlowEncrypt, char* buf, int nLen, int nEncryptKey);
//解密一块数据区
DLLEXPORT_MULT BOOL GCApiDecrypt(BOOL isVariFlowEncrypt, char* buf, int nLen, int nEncryptKey);

