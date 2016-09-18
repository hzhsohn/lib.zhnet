#pragma once

/*
* Copyright (c) 2004, game carrier
* All rights reserved.

* Filename: GCHELog.h
* Description:

* Author:bloodspider
* Time:2007-3-14   0:19
* Modify connect:

*/
//导出API函数时使用下面的宏定义
#ifndef DLLEXPORT_API
	#define DLLEXPORT_API extern "C" __declspec(dllexport) 
#endif
//导出类时使用下面的宏定义
#ifndef DLLEXPORT_CLASS
	#define DLLEXPORT_CLASS __declspec(dllexport) 
#endif

//导出类时使用下面的宏定义
#ifndef DLLEXPORT_MULT
#define DLLEXPORT_MULT __declspec(dllexport) 
#endif


//安全删除指针
#define GCSAFE_DELETE(p)				{ if(NULL != (p)) { delete (p);     (p)=NULL; } }

//安全删除数组
#define GCSAFE_DELETE_ARRAY(p)			{ if(NULL != (p)) { delete[] (p);   (p)=NULL; } }

//安全释放对象
#define GCSAFE_RELEASE(p)				{ if(NULL != (p)) { (p)->Release(); (p)=NULL; } }

//检查窗口是否存在
#define	GCCHECK_HWND(p)					if(NULL != (p) && ::IsWindow((p)->GetSafeHwnd())) 

//检查窗口是否为非法窗口
#define	GCCHECK_FALSEHWND(p)			if(NULL == (p) || !::IsWindow((p)p->GetSafeHwnd())) 

#define GCH_EXCEPTLOG_INI _T("cfg\\exceptlog.ini")



#ifndef _ZH_NET_MACRO_H_
//网络层宏

#define GSRTBegin		GCApiSRTNetProgramLoad
#define GSRTEnd			GCApiSRTNetProgramUnLoad

#define GSRTInit		GCEApiSRTNetInit
#define GSRTGetIp		GCEApiSRTNetGetIp
#define GSRTConnect		GCEApiSRTNetConnect
#define GSRTDisconnect	GCEApiSRTNetDisconnect

#define GSRGetPeerIP	GCEApiSRTNetGetPeerIP
#define GSRGetPeerPort	GCEApiSRTNetGetPeerPort
#define GSRSend			GCEApiSRTNetSendPacket

//线程
#define CREATE_THREAD(func,arg)		CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)func,(void*)arg,NULL,NULL)
#define CREATE_THREAD_RET(ret)		((ret)==0)

//临界区
#define TYPE_CS			CRITICAL_SECTION 
#define LOCK_CS(p)		EnterCriticalSection(p)	
#define UNLOCK_CS(p)		LeaveCriticalSection(p)
#define INIT_CS(p)		InitializeCriticalSection(p)
#define DELETE_CS(p)		DeleteCriticalSection(p)

//时间
#define GET_TIME_MS		GetTickCount()
#define GET_TIME_S		time(NULL)

#define _ZH_NET_MACRO_H_
#endif
