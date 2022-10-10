#pragma once

class CGCSRouteInfo
{
public:
	CGCSRouteInfo(void);
	~CGCSRouteInfo(void);

	static bool SetRouteInfo(long  lCounter, int& nLen, char* pData);
	static bool GetRouteInfo(long& lCounter, int& nLen, char* pData);
};
