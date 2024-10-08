/*
* Copyright (c) 2004, game carrier
* All rights reserved.

* Filename: GCSRTNetS.h
* Description:����ʱ���ı�����ԭʼ����

* Author:bloodspider
* Time:2007-4-19   22:52
* Modify connect:

*/

#pragma once

#include "GCHUtilMarco.h"
#include "wtypes.h"


//��ʼ��������Ϣ
DLLEXPORT_API VOID WINAPI GCApiSetEncryptEnable(BOOL isVariFlowEncrypt);

//��ʼ��δ��ȡԿ��ǰĬ������
DLLEXPORT_MULT int GCApiGetDefaultKey();
DLLEXPORT_MULT BOOL GCApiGetEncryptState();//�Ƿ����ñ�������

//ȡ�ü�����Կ
DLLEXPORT_MULT int GCApiGetEncryptKey();

//����һ��������
DLLEXPORT_MULT BOOL GCApiEncrypt(BOOL isVariFlowEncrypt, char* buf, int nLen, int nEncryptKey);
//����һ��������
DLLEXPORT_MULT BOOL GCApiDecrypt(BOOL isVariFlowEncrypt, char* buf, int nLen, int nEncryptKey);
