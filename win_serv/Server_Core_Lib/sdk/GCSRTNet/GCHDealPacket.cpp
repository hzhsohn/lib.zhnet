#include "StdAfx.h"
#include "gchdealpacket.h"

namespace CGCHDealPacket
{
	PFUN_NOREPLYSEND_CALLBACK	m_funNoReplySend;

	void SetNoreplySendCallBack(PFUN_NOREPLYSEND_CALLBACK funNoReplySend)
	{
		m_funNoReplySend = funNoReplySend;
	}
}
