#include "stdafx.h"
#include <comutil.h>
#include "GCHELog.h"


void GCMessageBox( LPCTSTR strFormat, ... )
{
	va_list args;
	TCHAR str[MAX_PATH];
	memset( str, 0, sizeof( str ) );
	va_start( args, strFormat );
#ifdef UNICODE
	_vsnwprintf( str, MAX_PATH, strFormat, args );
#else
	_vsnprintf( str, MAX_PATH, strFormat, args );
#endif

	//sprintf( str, strFormat, args ); //Not work well
	va_end( args );
#ifdef __AFX_H__
	AfxMessageBox( str );
#else
	::MessageBox(NULL,str,_T("GCMessageBox"),MB_OK);
#endif

	GCH_TRACE(_T(""),str);
}
