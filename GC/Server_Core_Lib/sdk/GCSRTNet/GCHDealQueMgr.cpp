#include "StdAfx.h"
#include ".\gchdealquemgr.h"

list<GCSTH_RecvDataInfo*> CGCHDealQueMgr::m_lstWaitDealData;
list<GCSTH_RecvDataInfo*> CGCHDealQueMgr::m_lstDealData;

CRITICAL_SECTION CGCHDealQueMgr::m_cs;
int CGCHDealQueMgr::m_nCurQueLen = 0;
CMemMgr<GCSTH_RecvDataInfo> CGCHDealQueMgr::m_memManager;

BOOL CGCHDealQueMgr::Init(void)
{
	InitializeCriticalSection(&m_cs);
	m_memManager.Init(GCHMAC_FREE_WAITDEAL_COUNT, GCHMAC_MAX_WAITDEAL_COUNT);
	return TRUE;
}

BOOL CGCHDealQueMgr::Destroy(void)
{
	GCSTH_RecvDataInfo* pTmp = NULL;
	for(list<GCSTH_RecvDataInfo*>::iterator it=m_lstWaitDealData.begin();it!=m_lstWaitDealData.end();++it)
	{		
		pTmp = (GCSTH_RecvDataInfo*)(*it);
		m_memManager.MyDel(pTmp);
	}
	for(list<GCSTH_RecvDataInfo*>::iterator it=m_lstDealData.begin();it!=m_lstDealData.end();++it)
	{		
		pTmp = (GCSTH_RecvDataInfo*)(*it);
		m_memManager.MyDel(pTmp);
	}
	m_lstWaitDealData.clear();
	m_lstDealData.clear();
	m_memManager.Destroy();
	DeleteCriticalSection(&m_cs);
	return TRUE;
}

BOOL CGCHDealQueMgr::InsertWaitDealData(void *handle,BYTE yFrameFlag,WORD wCRC16, GCEM_OPERATE_TYPE enOpType, int nLen, char* pBuf)
{
	BOOL ret = FALSE;

	GCSTH_RecvDataInfo* pRecvDataInfo = NULL;

	EnterCriticalSection(&m_cs);

	if(m_nCurQueLen >= GCHMAC_MAX_WAITDEAL_COUNT)
	{
		GCH_ETRACE(_T("CGCHDealQueMgr::InsertWaitDealData"), _T("m_nCurQueLen=%d >= GCHMAC_MAX_WAITDEAL_COUNT=%d"), m_nCurQueLen, GCHMAC_MAX_WAITDEAL_COUNT);
		goto exit;
	}

	pRecvDataInfo = m_memManager.MyNew();
	//GCH_ETRACE(_T("CGCHDealQueMgr::InsertWaitDealData "), _T("pRecvDataInfo == %d. m_memManager.GetUseCount=%d"), pRecvDataInfo, m_memManager.GetUseCount());
	if(pRecvDataInfo == NULL)
	{
		GCH_ETRACE(_T("CGCHDealQueMgr::InsertWaitDealData "), _T("pRecvDataInfo == NULL. m_memManager.GetUseCount=%d"), m_memManager.GetUseCount());
		goto exit;
	}
	
	pRecvDataInfo->handle = handle;
	pRecvDataInfo->yFrameFlag=yFrameFlag;
	pRecvDataInfo->wCRC16 = wCRC16;
	pRecvDataInfo->dwTickCount = GetTickCount();
	pRecvDataInfo->enOpType = enOpType;
	pRecvDataInfo->nLen = nLen;
	CopyMemory(pRecvDataInfo->buf, pBuf, nLen);	

	m_lstWaitDealData.push_back(pRecvDataInfo);
	m_nCurQueLen++;
	
	ret = TRUE;
exit:
	LeaveCriticalSection(&m_cs);
	return ret;
}

BOOL CGCHDealQueMgr::InsertOtherData(void *handle, GCEM_OPERATE_TYPE enOpType)
{
	BOOL ret = FALSE;
	
	if(GCE_CLOSE_SOCKET == enOpType)
	{
		GCH_KTRACE(_T("CGCHDealQueMgr::InsertOtherData"), _T("GCH_ClOSE_LOG == enOpType"));
	}

	GCSTH_RecvDataInfo* pRecvDataInfo = NULL;

	EnterCriticalSection(&m_cs);

	if(m_nCurQueLen >= GCHMAC_MAX_WAITDEAL_COUNT)
	{
		GCH_ETRACE(_T("CGCHDealQueMgr::InsertOtherData"), _T("m_nCurQueLen=%d >= GCHMAC_MAX_WAITDEAL_COUNT=%d"), m_nCurQueLen, GCHMAC_MAX_WAITDEAL_COUNT);
		goto exit;
	}

	pRecvDataInfo = m_memManager.MyNew();
	//GCH_ETRACE(_T("CGCHDealQueMgr::InsertOtherData "), _T("pRecvDataInfo == %d. m_memManager.GetUseCount=%d"), pRecvDataInfo, m_memManager.GetUseCount());
	if(pRecvDataInfo == NULL)
	{
		GCH_ETRACE(_T("CGCHDealQueMgr::InsertOtherData "), _T("pRecvDataInfo == NULL. m_memManager.GetUseCount=%d"), m_memManager.GetUseCount());
		goto exit;
	}

	pRecvDataInfo->handle = handle;
	pRecvDataInfo->enOpType = enOpType;

	m_lstWaitDealData.push_back(pRecvDataInfo);
	m_nCurQueLen++;
	
	ret = TRUE;

exit:
	LeaveCriticalSection(&m_cs);
	return ret;
}

BOOL CGCHDealQueMgr::GetDealData(GCSTH_RecvDataInfo* pRecvDataInfo)
{
	GCSTH_RecvDataInfo* pTmp = NULL;

	EnterCriticalSection(&m_cs);
	if(m_lstDealData.size() <= 0 && m_lstWaitDealData.size() > 0)
	{
		m_lstDealData.swap(m_lstWaitDealData);	
		m_nCurQueLen = 0;		
	}

	if(m_lstDealData.size() <= 0)
	{		
		LeaveCriticalSection(&m_cs);
		return FALSE;
	}
	
	pTmp = (GCSTH_RecvDataInfo*)m_lstDealData.front();
	CopyMemory(pRecvDataInfo, pTmp, sizeof(GCSTH_RecvDataInfo));

	GCH_ETRACE(_T("CGCHDealQueMgr::GetDealData  pTmp == %d. m_memManager.GetUseCount=%d"), pTmp, m_memManager.GetUseCount());

	m_memManager.MyDel(pTmp);
	m_lstDealData.pop_front();

	LeaveCriticalSection(&m_cs);

	return TRUE;
}
