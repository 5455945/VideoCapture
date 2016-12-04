#ifndef __COMMON_H__
#define __COMMON_H__

#include <DShow.h>
#include "qedit.h"
#include <strsafe.h>
#include <assert.h>
#pragma comment(lib,"Strmiids.lib")

//define release maco
#define ReleaseInterface(x) \
	if ( NULL != x ) \
{ \
	x->Release( ); \
	x = NULL; \
}
// Application-defined message to notify app of filter graph events
#define WM_GRAPHNOTIFY  WM_APP+100

void Msg(HWND hwnd,TCHAR *szFormat, ...);
bool Bstr_Compare(BSTR bstrFilter,BSTR bstrDevice);

#endif// __COMMON_H__