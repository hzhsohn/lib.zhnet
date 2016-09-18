#pragma once

namespace CGCHDealPacket
{
	typedef BOOL (WINAPI* PFUN_NOREPLYSEND_CALLBACK)(HANDLE handle,int nLen, char* pData);
	void SetNoreplySendCallBack(PFUN_NOREPLYSEND_CALLBACK funNoReplySend);
};
