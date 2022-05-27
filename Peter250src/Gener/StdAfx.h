// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__909E13E2_0D50_11D3_9CEF_00C06C470665__INCLUDED_)
#define AFX_STDAFX_H__909E13E2_0D50_11D3_9CEF_00C06C470665__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////////
// obsluha debuggeru

void debugbreak(const char* text);

#define ASSERT(f,t)			{ if (!(f)) debugbreak(t); }
#define VERIFY(f,t)			{ if (!(f)) debugbreak(t); }
#define ASSERTERROR(t)		debugbreak(t)

#ifdef _UNICODE
#ifndef UNICODE
#define UNICODE
#endif	// UNICODE
#endif	// _UNICODE

#define ICONWIDTH 32								// šíøka ikon
#define ICONHEIGHT 32								// výška ikon
#define ICONSIZE (ICONWIDTH*ICONHEIGHT)				// velikost ikon v bajtech

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <stdio.h>
#include <windows.h>					// základní definice WINDOWS
#include <math.h>						// matematické operace
//#include <alphaops.h>					// matematické konstanty
#include <tchar.h>						// obsluha znakù UNICODE/MB
#include <commctrl.h>					// doplòkové ovládací prvky
#include <richedit.h>					// RichEdit
#include <malloc.h>

#define DOUBLE_INFINITY_VALUE       ((ULONGLONG)0x7ff0000000000000)

#define _MAX_PATH 260
extern	HINSTANCE	hInstance;			// instance programu

inline void* _fastcall MemGet(int size)
{
	return malloc(size);
}

inline void _fastcall MemFree(void* adr)
{
	free(adr);
}

inline void* _fastcall MemSize(void* adr, int size)
{
	return realloc(adr, size);
}

inline void _fastcall MemCopy(void* dst, const void* src, DWORD count)
{
	memcpy(dst, src, count);
}

inline BOOL _fastcall MemCompare(const void* buf1, const void* buf2, int count)
{
	return ((count <= 0) || (memcmp(buf1, buf2, count) == 0));
}

inline void LongIncrement(long* num)
{
	(*num)++;
};

inline BOOL LongDecrement(long* num)
{
	return (--(*num) == 0);
};

inline void _fastcall MemFill(void* dst, int count, char val)
{
	char* dst1 = (char*)dst;

	for (; count > 0; count--)
	{
		*dst1 = val;
		dst1++;
	}
}

class CText;

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__909E13E2_0D50_11D3_9CEF_00C06C470665__INCLUDED_)
