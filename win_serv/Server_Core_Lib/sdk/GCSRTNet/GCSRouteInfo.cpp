#include "StdAfx.h"
#include "gcsrouteinfo.h"

CGCSRouteInfo::CGCSRouteInfo(void)
{
}

CGCSRouteInfo::~CGCSRouteInfo(void)
{

}

bool CGCSRouteInfo::SetRouteInfo(long  lCounter, int& nLen, char* pData)
{
	long* pCounter = (long*)&pData[nLen];
	*pCounter = lCounter;
	nLen += sizeof(long);
	//
	//int* pNumber = (int*)&pData[nLen];
	//*pNumber = wNumber;
	//nLen += sizeof(int);
	//
	return true;
}
bool CGCSRouteInfo::GetRouteInfo(long& lCounter, int& nLen, char* pData)
{
	nLen -= sizeof(long);
	lCounter = *(long*)&(pData[nLen]);
	
	//nLen -= sizeof(HANDLE);
	//handle = *(HANDLE*)&(pData[nLen]);

	return true;
}

