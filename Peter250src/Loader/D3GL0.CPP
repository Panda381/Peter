
// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

#include "Main.h"

/***************************************************************************\
*																			*
*							3D rozhranÌ - OpenGL 1.0						*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// data

// funkce OpenGL 1.0
WGLCREATECONTEXT	pwglCreateContext	= NULL;
WGLDELETECONTEXT	pwglDeleteContext	= NULL;
WGLGETCURRENTDC		pwglGetCurrentDC	= NULL;
WGLMAKECURRENT		pwglMakeCurrent		= NULL;

GLALPHAFUNC			pglAlphaFunc		= NULL;
GLBEGIN				pglBegin			= NULL;
GLBLENDFUNC			pglBlendFunc		= NULL;
GLCLEAR				pglClear			= NULL;
GLCLEARCOLOR		pglClearColor		= NULL;
GLCLEARSTENCIL		pglClearStencil		= NULL;
GLCOLORMASK			pglColorMask		= NULL;
GLCOLOR3F			pglColor3f			= NULL;
GLCOLOR3FV			pglColor3fv			= NULL;
GLCOLOR4F			pglColor4f			= NULL;
GLCOLOR4FV			pglColor4fv			= NULL;
GLCULLFACE			pglCullFace			= NULL;
GLDEPTHFUNC			pglDepthFunc		= NULL;
GLDEPTHMASK			pglDepthMask		= NULL;
GLDEPTHRANGE		pglDepthRange		= NULL;
GLDISABLE			pglDisable			= NULL;
GLENABLE			pglEnable			= NULL;
GLEND				pglEnd				= NULL;
GLFOGFV				pglFogfv			= NULL;
GLFRONTFACE			pglFrontFace		= NULL;
GLGETINTEGERV		pglGetIntegerv		= NULL;
GLHINT				pglHint				= NULL;
GLLIGHTFV			pglLightfv			= NULL;
GLLIGHTMODELFV		pglLightModelfv		= NULL;
GLLOADIDENTITY		pglLoadIdentity		= NULL;
GLLOADMATRIXF		pglLoadMatrixf		= NULL;
GLMATERIALFV		pglMaterialfv		= NULL;
GLMATRIXMODE		pglMatrixMode		= NULL;
GLMULTMATRIXF		pglMultMatrixf		= NULL;
GLNORMAL3FV			pglNormal3fv		= NULL;
GLPIXELSTOREI		pglPixelStorei		= NULL;
GLPOLYGONMODE		pglPolygonMode		= NULL;
GLPOPMATRIX			pglPopMatrix		= NULL;
GLPUSHMATRIX		pglPushMatrix		= NULL;
GLSHADEMODEL		pglShadeModel		= NULL;
GLTEXCOORD2FV		pglTexCoord2fv		= NULL;
GLTEXIMAGE2D		pglTexImage2D		= NULL;
GLTEXENVI			pglTexEnvi			= NULL;
GLTEXGENI			pglTexGeni			= NULL;
GLTEXPARAMETERI		pglTexParameteri	= NULL;
GLVERTEX2I			pglVertex2i			= NULL;
GLVERTEX2F			pglVertex2f			= NULL;
GLVERTEX3F			pglVertex3f			= NULL;
GLVERTEX3FV			pglVertex3fv		= NULL;
GLVIEWPORT			pglViewport			= NULL;
GLSTENCILOP			pglStencilOp		= NULL;
GLSTENCILFUNC		pglStencilFunc		= NULL;
GLSTENCILMASK		pglStencilMask		= NULL;

// OpenGL 1.0
bool				D3GL0D3Test			= false;	// probÏhl test rozhranÌ OpenGL 1.0
HINSTANCE			D3GL0Lib			= NULL;		// handle modulu OPENGL32.DLL
bool				D3GL0OK				= false;	// rozhranÌ OpenGL 1.0 vytvo¯eno OK

// renderovacÌ povrch
HGLRC				D3GL0Cont			= NULL;		// renderovacÌ povrch
HPALETTE			D3GL0Pal			= NULL;		// palety OpenGL

HWND				D3GL0Win			= NULL;		// renderovacÌ okno

int					D3GL0AktScreenBits	= -1;		// aktivnÌ poËet bit˘

TCHAR		OpenGLClass[] = _T("OpenGLWindowClass");	// n·zev t¯Ìdy hlavnÌho okna

////////////////////////////////////////////////////////////////////
// tabulka stencil operacÌ

int	StencilTabOGL[9] =
{
	GL_KEEP,				// = 0
	GL_KEEP,				// D3DSTENCILOP_KEEP = 1
	GL_ZERO,				// D3DSTENCILOP_KEEP = 2
	GL_REPLACE,				// D3DSTENCILOP_REPLACE = 3
	GL_INCR,				// D3DSTENCILOP_INCRSAT = 4
	GL_DECR,				// D3DSTENCILOP_DECRSAT = 5
	GL_INVERT,				// D3DSTENCILOP_INVERT = 6
	GL_INCR,				// D3DSTENCILOP_INCR = 7
	GL_DECR,				// D3DSTENCILOP_DECR = 8
};

/////////////////////////////////////////////////////////////////////////////
// inicializace rozhranÌ OpenGL 1.0 (vracÌ TRUE=rozhranÌ je vytvo¯eno)

bool D3GL0InitD3()
{
// test, zda inicializace jiû probÏhla
	if (!D3GL0D3Test)
	{
		D3GL0D3Test = true;

// naËtenÌ knihovny OpenGL32.DLL
		if (D3GL0Lib == NULL)
		{
			D3GL0Lib = ::LoadLibrary(_T("OPENGL32.DLL"));
			FPUDouble();
		}

// test, zda byla knihovna nalezena
		if (D3GL0Lib != NULL)
		{

// vyhled·nÌ funkcÌ OpenGL 1.0
			pwglCreateContext = (WGLCREATECONTEXT)::GetProcAddress(D3GL0Lib, "wglCreateContext");
			pwglDeleteContext = (WGLDELETECONTEXT)::GetProcAddress(D3GL0Lib, "wglDeleteContext");
			pwglGetCurrentDC = (WGLGETCURRENTDC)::GetProcAddress(D3GL0Lib, "wglGetCurrentDC");
			pwglMakeCurrent = (WGLMAKECURRENT)::GetProcAddress(D3GL0Lib, "wglMakeCurrent");

			pglAlphaFunc = (GLALPHAFUNC)::GetProcAddress(D3GL0Lib, "glAlphaFunc");
			pglBegin = (GLBEGIN)::GetProcAddress(D3GL0Lib, "glBegin");
			pglBlendFunc = (GLBLENDFUNC)::GetProcAddress(D3GL0Lib, "glBlendFunc");
			pglClear = (GLCLEAR)::GetProcAddress(D3GL0Lib, "glClear");
			pglClearColor = (GLCLEARCOLOR)::GetProcAddress(D3GL0Lib, "glClearColor");
			pglClearStencil = (GLCLEARSTENCIL)::GetProcAddress(D3GL0Lib, "glClearStencil");
			pglColorMask = (GLCOLORMASK)::GetProcAddress(D3GL0Lib, "glColorMask");
			pglColor3f = (GLCOLOR3F)::GetProcAddress(D3GL0Lib, "glColor3f");
			pglColor3fv = (GLCOLOR3FV)::GetProcAddress(D3GL0Lib, "glColor3fv");
			pglColor4f = (GLCOLOR4F)::GetProcAddress(D3GL0Lib, "glColor4f");
			pglColor4fv = (GLCOLOR4FV)::GetProcAddress(D3GL0Lib, "glColor4fv");
			pglCullFace = (GLCULLFACE)::GetProcAddress(D3GL0Lib, "glCullFace");
			pglDepthFunc = (GLDEPTHFUNC)::GetProcAddress(D3GL0Lib, "glDepthFunc");
			pglDepthMask = (GLDEPTHMASK)::GetProcAddress(D3GL0Lib, "glDepthMask");
			pglDepthRange = (GLDEPTHRANGE)::GetProcAddress(D3GL0Lib, "glDepthRange");
			pglDisable = (GLDISABLE)::GetProcAddress(D3GL0Lib, "glDisable");
			pglEnable = (GLENABLE)::GetProcAddress(D3GL0Lib, "glEnable");
			pglEnd = (GLEND)::GetProcAddress(D3GL0Lib, "glEnd");
			pglFogfv = (GLFOGFV)::GetProcAddress(D3GL0Lib, "glFogfv");
			pglFrontFace = (GLFRONTFACE)::GetProcAddress(D3GL0Lib, "glFrontFace");
			pglGetIntegerv = (GLGETINTEGERV)::GetProcAddress(D3GL0Lib, "glGetIntegerv");
			pglHint = (GLHINT)::GetProcAddress(D3GL0Lib, "glHint");
			pglLightfv = (GLLIGHTFV)::GetProcAddress(D3GL0Lib, "glLightfv");
			pglLightModelfv = (GLLIGHTMODELFV)::GetProcAddress(D3GL0Lib, "glLightModelfv");
			pglLoadIdentity = (GLLOADIDENTITY)::GetProcAddress(D3GL0Lib, "glLoadIdentity");
			pglLoadMatrixf = (GLLOADMATRIXF)::GetProcAddress(D3GL0Lib, "glLoadMatrixf");
			pglMaterialfv = (GLMATERIALFV)::GetProcAddress(D3GL0Lib, "glMaterialfv");
			pglMatrixMode = (GLMATRIXMODE)::GetProcAddress(D3GL0Lib, "glMatrixMode");
			pglMultMatrixf = (GLMULTMATRIXF)::GetProcAddress(D3GL0Lib, "glMultMatrixf");
			pglNormal3fv = (GLNORMAL3FV)::GetProcAddress(D3GL0Lib, "glNormal3fv");
			pglPixelStorei = (GLPIXELSTOREI)::GetProcAddress(D3GL0Lib, "glPixelStorei");
			pglPolygonMode = (GLPOLYGONMODE)::GetProcAddress(D3GL0Lib, "glPolygonMode");
			pglPopMatrix = (GLPOPMATRIX)::GetProcAddress(D3GL0Lib, "glPopMatrix");
			pglPushMatrix = (GLPUSHMATRIX)::GetProcAddress(D3GL0Lib, "glPushMatrix");
			pglShadeModel = (GLSHADEMODEL)::GetProcAddress(D3GL0Lib, "glShadeModel");
			pglTexCoord2fv = (GLTEXCOORD2FV)::GetProcAddress(D3GL0Lib, "glTexCoord2fv");
			pglTexEnvi = (GLTEXENVI)::GetProcAddress(D3GL0Lib, "glTexEnvi");
			pglTexGeni = (GLTEXGENI)::GetProcAddress(D3GL0Lib, "glTexGeni");
			pglTexImage2D = (GLTEXIMAGE2D)::GetProcAddress(D3GL0Lib, "glTexImage2D");
			pglTexParameteri = (GLTEXPARAMETERI)::GetProcAddress(D3GL0Lib, "glTexParameteri");
			pglVertex2i = (GLVERTEX2I)::GetProcAddress(D3GL0Lib, "glVertex2i");
			pglVertex2f = (GLVERTEX2F)::GetProcAddress(D3GL0Lib, "glVertex2f");
			pglVertex3f = (GLVERTEX3F)::GetProcAddress(D3GL0Lib, "glVertex3f");
			pglVertex3fv = (GLVERTEX3FV)::GetProcAddress(D3GL0Lib, "glVertex3fv");
			pglViewport = (GLVIEWPORT)::GetProcAddress(D3GL0Lib, "glViewport");
			pglStencilOp = (GLSTENCILOP)::GetProcAddress(D3GL0Lib, "glStencilOp");
			pglStencilFunc = (GLSTENCILFUNC)::GetProcAddress(D3GL0Lib, "glStencilFunc");
			pglStencilMask = (GLSTENCILMASK)::GetProcAddress(D3GL0Lib, "glStencilMask");

// kontrola funkcÌ
			if (
				(pwglCreateContext != NULL) &&
				(pwglDeleteContext != NULL) &&
				(pwglGetCurrentDC != NULL) &&
				(pwglMakeCurrent != NULL) &&

				(pglAlphaFunc != NULL) &&
				(pglBegin != NULL) &&
				(pglBlendFunc != NULL) &&
				(pglClear != NULL) &&
				(pglClearColor != NULL) &&
				(pglClearStencil != NULL) &&
				(pglColorMask != NULL) &&
				(pglColor3f != NULL) &&
				(pglColor3fv != NULL) &&
				(pglColor4f != NULL) &&
				(pglColor4fv != NULL) &&
				(pglCullFace != NULL) &&
				(pglDepthFunc != NULL) &&
				(pglDepthMask != NULL) &&
				(pglDepthRange != NULL) &&
				(pglDisable != NULL) &&
				(pglEnable != NULL) &&
				(pglEnd != NULL) &&
				(pglFogfv != NULL) &&
				(pglFrontFace != NULL) &&
				(pglGetIntegerv != NULL) &&
				(pglHint != NULL) &&
				(pglLightfv != NULL) &&
				(pglLightModelfv != NULL) &&
				(pglLoadIdentity != NULL) &&
				(pglLoadMatrixf != NULL) &&
				(pglMaterialfv != NULL) &&
				(pglMatrixMode != NULL) &&
				(pglMultMatrixf != NULL) &&
				(pglNormal3fv != NULL) &&
				(pglPixelStorei != NULL) &&
				(pglPolygonMode != NULL) &&
				(pglPopMatrix != NULL) &&
				(pglPushMatrix != NULL) &&
				(pglShadeModel != NULL) &&
				(pglTexCoord2fv != NULL) &&
				(pglTexEnvi != NULL) &&
				(pglTexGeni != NULL) &&
				(pglTexImage2D != NULL) &&
				(pglTexParameteri != NULL) &&
				(pglVertex2i != NULL) &&
				(pglVertex2f != NULL) &&
				(pglVertex3f != NULL) &&
				(pglVertex3fv != NULL) &&
				(pglViewport != NULL) &&
				(pglStencilOp != NULL) &&
				(pglStencilFunc != NULL) &&
				(pglStencilMask != NULL)
				)
			{
				D3GL0OK = true;
			}
		}
	}

// test, zda je rozhranÌ OpenGL 1.0 funkËnÌ
	return D3GL0OK;
}

/////////////////////////////////////////////////////////////////////////////
// ukonËenÌ rozhranÌ OpenGL 1.0

void D3GL0TermD3()
{
// zruöenÌ z·vislÈho rozhranÌ
	D3GL1TermD3();

// zruöenÌ renderovacÌho okna
	if (D3GL0Win != NULL)
	{
		::DestroyWindow(D3GL0Win);
		D3GL0Win = NULL;
	}

// uvolnÏnÌ knihovny OpenGL32.dll
	if (D3GL0Lib != NULL)
	{
		::FreeLibrary(D3GL0Lib);
		D3GL0Lib = NULL;
	}

// zruöenÌ funkcÌ
/*
	pwglCreateContext = NULL;
	pwglDeleteContext = NULL;
	pwglGetCurrentDC = NULL;
	pwglMakeCurrent = NULL;

	pglAlphaFunc = NULL;
	pglBegin = NULL;
	pglBlendFunc = NULL;
	pglClear = NULL;
	pglClearColor = NULL;
	pglClearStencil = NULL;
	pglColorMask = NULL;
	pglColor3f = NULL;
	pglColor3fv = NULL;
	pglColor4f = NULL;
	pglColor4fv = NULL;
	pglCullFace = NULL;
	pglDepthFunc = NULL;
	pglDepthMask = NULL;
	pglDepthRange = NULL;
	pglDisable = NULL;
	pglEnable = NULL;
	pglEnd = NULL;
	pglFogfv = NULL;
	pglFrontFace = NULL;
	pglGetIntegerv = NULL;
	pglHint = NULL;
	pglLightfv = NULL;
	pglLightModelfv = NULL;
	pglLoadIdentity = NULL;
	pglLoadMatrixf = NULL;
	pglMaterialfv = NULL;
	pglMatrixMode = NULL;
	pglMultMatrixf = NULL;
	pglNormal3fv = NULL;
	pglPixelStorei = NULL;
	pglPolygonMode = NULL;
	pglPopMatrix = NULL;
	pglPushMatrix = NULL;
	pglShadeModel = NULL;
	pglTexCoord2fv = NULL;
	pglTexEnvi = NULL;
	pglTexGeni = NULL;
	pglTexImage2D = NULL;
	pglTexParameteri = NULL;
	pglVertex2i = NULL;
	pglVertex2f = NULL;
	pglVertex3f = NULL;
	pglVertex3fv = NULL;
	pglViewport = NULL;
	pglStencilOp = NULL;
	pglStencilFunc = NULL;
	pglStencilMask = NULL;
*/

// zruöenÌ p¯Ìznaku, ûe byla inicializace
	D3GL0D3Test = false;
	D3GL0OK = false;

	D3GL0AktScreenBits = -1;

	FPUDouble();
}

/////////////////////////////////////////////////////////////////////////////
// nastavenÌ viewportu

void D3GL0SetView()
{
	if ((D3GL0Win == NULL) || (D3GL0Cont == NULL)) return;

//	if (D3GL0Win != NULL)
	{
		::MoveWindow(D3GL0Win, D3DLeft, D3DTop, D3DWidth, D3DHeight, FALSE);
	}

	if (pglViewport != NULL)
	{
		pglViewport(0, 0, D3DWidth, D3DHeight);
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace viewportu po zmÏnÏ velikosti

void D3GL0SizeView()
{
	D3GL0SetView();
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace viewportu po p¯esunu

void D3GL0MoveView() { }

/////////////////////////////////////////////////////////////////////////////
// inicializace palet pro OpenGL

void D3GL0InitPal(HDC dc)
{

// zruöenÌ star˝ch palet
	D3GL0TermPal();

// zjiötÏnÌ aktu·lnÌho form·tu
	int pixelFormat = ::GetPixelFormat(dc);
	PIXELFORMATDESCRIPTOR pfd;
	LOGPALETTE* pPal;
	int paletteSize;

	int redMask, greenMask, blueMask;
	int i;

// test, zda je pot¯eba vytv·¯et palety
	::DescribePixelFormat(dc, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	if ((pfd.dwFlags & PFD_NEED_PALETTE) == 0) return;

// p¯Ìprava bufferu pro palety
	paletteSize = 1 << pfd.cColorBits;

	pPal = (LOGPALETTE*)MemGet(sizeof(LOGPALETTE) + paletteSize * sizeof(PALETTEENTRY));
	pPal->palVersion = 0x300;
	pPal->palNumEntries = (short)paletteSize;

// vygenerov·nÌ palet
	redMask = (1 << pfd.cRedBits) - 1;
	greenMask = (1 << pfd.cGreenBits) - 1;
	blueMask = (1 << pfd.cBlueBits) - 1;

	for (i=0; i<paletteSize; ++i)
	{
		pPal->palPalEntry[i].peRed = (BYTE)(
			(((i >> pfd.cRedShift) & redMask) * 255) / redMask);
		pPal->palPalEntry[i].peGreen = (BYTE)(
			(((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask);
		pPal->palPalEntry[i].peBlue = (BYTE)(
			(((i >> pfd.cBlueShift) & blueMask) * 255) / blueMask);
		pPal->palPalEntry[i].peFlags = 0;
	}

// nastavenÌ palet
	D3GL0Pal = ::CreatePalette(pPal);
	MemFree(pPal);

	if (D3GL0Pal)
	{
		::SelectPalette(dc, D3GL0Pal, FALSE);
		::RealizePalette(dc);
	}
}

/////////////////////////////////////////////////////////////////////////////
// zruöenÌ palet

void D3GL0TermPal()
{
	if (D3GL0Pal != NULL)
	{
		::UnrealizeObject(D3GL0Pal);
		::DeleteObject(D3GL0Pal);
		D3GL0Pal = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// pokusnÈ nastavenÌ kontextu OpenGL (vracÌ TRUE=inicializace OK)

bool D3GL0TestGL(HDC dc, BYTE cColorBits, BYTE cDepthBits, BYTE cStencBits, DWORD flags)
{
// p¯Ìprava popisovaËe form·tu
	PIXELFORMATDESCRIPTOR pfd;
	MemFill(&pfd, sizeof(pfd), 0);
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags =	PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | flags,
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = cColorBits;
	pfd.cDepthBits = cDepthBits;
	pfd.cStencilBits = cStencBits;
	pfd.iLayerType = PFD_MAIN_PLANE;

// pozor - funkce nefunguje, nenÌ-li naËtena knihovna OpenGL
// Funkce modifikuje p¯esnost koprocesoru!
	DWORD pf = ::ChoosePixelFormat(dc, &pfd);

// vytvo¯enÌ kontextu za¯ÌzenÌ
	if (pf > 0)
	{
		if (::SetPixelFormat(dc, pf, &pfd) != FALSE)
		{
			D3GL0InitPal(dc);

			D3GL0Cont = pwglCreateContext(dc);

			if (D3GL0Cont != NULL)
			{
				if (pwglMakeCurrent(dc, D3GL0Cont)) return true;

				pwglDeleteContext(D3GL0Cont);
				D3GL0Cont = NULL;
			}
			D3GL0TermPal();
		}
	}

	return false;
}

bool D3GL0TestGL2(HDC dc, BYTE cColorBits)
{
	if (cColorBits <= 16)
	{
		return (
			D3GL0TestGL(dc, cColorBits, 16, 0, PFD_DOUBLEBUFFER) ||
			D3GL0TestGL(dc, cColorBits, 15, 1, PFD_DOUBLEBUFFER) ||
			D3GL0TestGL(dc, cColorBits, 24, 8, PFD_DOUBLEBUFFER) ||
			D3GL0TestGL(dc, cColorBits, 24, 4, PFD_DOUBLEBUFFER) ||
			D3GL0TestGL(dc, cColorBits, 32, 0, PFD_DOUBLEBUFFER) ||
			D3GL0TestGL(dc, cColorBits, 24, 0, PFD_DOUBLEBUFFER) ||
			D3GL0TestGL(dc, cColorBits, 8, 0, PFD_DOUBLEBUFFER)
		);
	}
	else
	{
		return (
			D3GL0TestGL(dc, cColorBits, 24, 8, PFD_DOUBLEBUFFER) ||
			D3GL0TestGL(dc, cColorBits, 24, 4, PFD_DOUBLEBUFFER) ||
			D3GL0TestGL(dc, cColorBits, 15, 1, PFD_DOUBLEBUFFER) ||
			D3GL0TestGL(dc, cColorBits, 32, 0, PFD_DOUBLEBUFFER) ||
			D3GL0TestGL(dc, cColorBits, 24, 0, PFD_DOUBLEBUFFER) ||
			D3GL0TestGL(dc, cColorBits, 16, 0, PFD_DOUBLEBUFFER) ||
			D3GL0TestGL(dc, cColorBits, 8, 0, PFD_DOUBLEBUFFER)
		);
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvo¯enÌ kontextu za¯ÌzenÌ (vracÌ TRUE=OK)

bool D3GL0InitCont()
{
// zruöenÌ starÈho kontextu
	D3GL0TermCont();

// inicializace rozhranÌ
	if (!D3GL0InitD3()) return false;

// DC okna
	HDC dc = ::GetDC(D3GL0Win);

// vytvo¯enÌ novÈho kontextu
	bool res;
	
	switch (ScreenBits)
	{
	case 24:
		res = (	D3GL0TestGL2(dc, 24) ||
				D3GL0TestGL2(dc, 32) ||
				D3GL0TestGL2(dc, 16) ||
				D3GL0TestGL2(dc, 8));
		break;

	case 16:
		res = (	D3GL0TestGL2(dc, 16) ||
				D3GL0TestGL2(dc, 32) ||
				D3GL0TestGL2(dc, 24) ||
				D3GL0TestGL2(dc, 8));
		break;

	case 8:
		res = (	D3GL0TestGL2(dc, 8) ||
				D3GL0TestGL2(dc, 16) ||
				D3GL0TestGL2(dc, 24) ||
				D3GL0TestGL2(dc, 32));
		break;

	default:
		res = (	D3GL0TestGL2(dc, 32) ||
				D3GL0TestGL2(dc, 24) ||
				D3GL0TestGL2(dc, 16) ||
				D3GL0TestGL2(dc, 8));
	}

	if (res)
	{
		FPUDouble();
		::ReleaseDC(D3GL0Win, dc);
		return true;
	}

	::ReleaseDC(D3GL0Win, dc);
	D3GL0Stop();
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// zruöenÌ renderovacÌho povrchu

void D3GL0TermCont()
{
	if (D3GL0Cont != NULL)
	{
		HDC dc = ::GetDC(D3GL0Win);
		pwglMakeCurrent(dc, NULL);
		pwglDeleteContext(D3GL0Cont);
		D3GL0Cont = NULL;
		::ReleaseDC(D3GL0Win, dc);
	}
}

/////////////////////////////////////////////////////////////////////////////
// obsluha zpr·v okna

LRESULT CALLBACK OpenGLWindowProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			::BeginPaint(hWnd, &ps);
			::EndPaint(hWnd, &ps);
		}
		return 0;

	case WM_SETCURSOR:
		{
			POINT pt;
			::GetCursorPos(&pt);	// naËtenÌ sou¯adnic myöi
			::ScreenToClient(MainFrame, &pt); // p¯evod na klientskÈ sou¯adnice
			if (OnSetCursor(pt.x, pt.y))
			{
				return TRUE;
			}
		}
		break;
	}

	return ::DefWindowProc(hWnd, nMsg, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// zah·jenÌ funkce za¯ÌzenÌ (vracÌ TRUE=inicializace OK)
// typ za¯ÌzenÌ: 1=HAL,2=TnLHal,3=REF,4=RGB,5=MMX,6=Ramp,7=Null

bool D3GL0Start(int type)
{
// kontrola typu za¯ÌzenÌ
	if (type != NHALDev) return false;

// test, zda nenÌ zmÏna poËtu bit˘ obrazovky (po naËtenÌ jiû nelze mÏnit)
	int bits = ScreenBits;

	HDC dc = ::GetDC(MainFrame);
	if (dc)
	{
		bits = ::GetDeviceCaps(dc, BITSPIXEL);
		::ReleaseDC(MainFrame, dc);
	}

	if ((D3GL0AktScreenBits != bits) && (D3GL0AktScreenBits > 0))
	{
		D3GL0TermD3();
	}
	D3GL0AktScreenBits = bits;

// inicializace rozhranÌ
	if (!D3GL0InitD3()) return false;

// vytvo¯enÌ renderovacÌho okna
	if (D3GL0Win != NULL)
	{
		::ShowWindow(D3GL0Win, SW_SHOW);
	}
	else
	{
		WNDCLASS wcl;
		wcl.style = CS_DBLCLKS | CS_OWNDC;
		wcl.lpfnWndProc = OpenGLWindowProc;
		wcl.cbClsExtra = 0;
		wcl.cbWndExtra = 0;
		wcl.hInstance = hInstance;
		wcl.hIcon = NULL;
		wcl.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		wcl.hbrBackground = NULL;
		wcl.lpszMenuName = NULL;
		wcl.lpszClassName = OpenGLClass;
		::RegisterClass(&wcl);


		D3GL0Win = ::CreateWindowEx(
			0,												// rozöÌ¯en˝ styl
			OpenGLClass,									// jmÈno t¯Ìdy okna
			NULL,											// titulek okna
			WS_CHILD | WS_VISIBLE,							// styl okna
			D3DLeft,										// X
			D3DTop,											// Y
			D3DWidth,										// öÌ¯ka
			D3DHeight,										// v˝öka
			MainFrame,										// rodiË
			NULL,											// menu
			hInstance,										// instance
			NULL);											// parametry

		if (D3GL0Win == NULL) return false;
	}

// inicializace ovladaËe OpenGL
	if (!D3GL0InitCont())
	{
		D3GL0Stop();
		return false;
	}

// nastavenÌ viewportu
	D3GL0SetView();

// inicializace adres funkcÌ za¯ÌzenÌ
	pD3Stop = D3GL0Stop;			// ukonËenÌ funkce za¯ÌzenÌ
	pD3SizeView = D3GL0SizeView;	// aktualizace viewportu po zmÏnÏ velikosti
	pD3MoveView = D3GL0MoveView;	// aktualizace viewportu po p¯esunu
	pD3Disp = D3GL0Disp;			// zobrazenÌ renderovacÌho bufferu
	pD3Clear = D3GL0Clear;			// vymaz·nÌ renderovacÌ plochy
	pD3AktProj = D3GL0AktProj;		// aktualizace projekËnÌ matice
	pD3AktView = D3GL0AktView;		// aktualizace pohledovÈ matice
	pD3Begin = D3GL0Begin;			// zah·jenÌ renderov·nÌ scÈny
	pD3End = D3GL0End;				// ukonËenÌ renderov·nÌ scÈny
	pD3Render = D3GL0Render;		// vyrenderov·nÌ jednoho objektu
	pD3Reset = D3GL0Reset;			// resetov·nÌ objektu p¯ed ukonËenÌm ovladaËe
	pD3AktWireframe = D3GL0AktWireframe;
	pD3AktLightOn = D3GL0AktLightOn;
	pD3AktShades = D3GL0AktShades;
	pD3AktCulling = D3GL0AktCulling;
	pD3AktMagFilter = D3GL0AktMagFilter;
	pD3AktMinMipFilter = D3GL0AktMinMipFilter;
	pD3AktLight = D3GL0AktLight;
	pD3SetMaterial = D3GL0SetMaterial;
	pD3TextureRes = D3GL0TextureRes;
	pD3TextureSet = D3GL0TextureSet;
	pD3TextureOn = D3GL0TextureOn;
	pD3TextureOff = D3GL0TextureOff;
	pD3AktAmbient = D3GL0AktAmbient;
	pD3Free = D3GL0Free;
	pD3AktFog = D3GL0AktFog;
	pD3FogOn = D3GL0FogOn;
	pD3AktState = D3GL0AktState;
	pD3AktBlend = D3GL0AktBlend;
	pD3AktAlfa = D3GL0AktAlfa;
	pD3SetColorOp = D3GL0SetColorOp;
	pD3SetAddress = D3GL0SetAddress;
	pD3AktPalette = D3GL0AktPalette;
	pD3SetZFunc = D3GL0SetZFunc;
	pD3SetZWrite = D3GL0SetZWrite;
	pD3SEnable = D3GL0SEnable;
	pD3SetSOp = D3GL0SetSOp;
	pD3SetSFunc = D3GL0SetSFunc;
	pD3SetSMask = D3GL0SetSMask;
	pD3Shadow = D3GL0Shadow;
	pD3Shadows = D3GL0Shadows;

// inicializace informacÌ o za¯ÌzenÌ
	pglGetIntegerv(GL_MAX_TEXTURE_SIZE, &MaxTextureWidth);
	MaxTextureHeight = MaxTextureWidth;
	MaxTextureRepeat = 0x8000;

	pglGetIntegerv(GL_STENCIL_BITS, &D3DSBufferDepth);
	D3DSBufferFormatOK = (D3DSBufferDepth > 0);
	D3DSBufferMax = (1 << D3DSBufferDepth) - 1;

	SquareTexture = false;
	TexturePow2 = true;
	IsBlending = true;
	IsMagFilter = true;
	IsMinFilter = true;
	IsMipMap = true;
	IsHWRaster = true;
	IsHWTransLight = false;
	IsPureDevice = false;
	AlphaGreaterEqual = true;
	AlphaGreater = true;

	MaxVertexNum = 0x10000;
	MaxTextureStages = 1;
//	MaxTextureSimult = 1;

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// ukonËenÌ funkce za¯ÌzenÌ

void D3GL0Stop()
{
	// s oknem se musÌ nejd¯Ìve pohnout, jinak se m˘ûe ve FullScreen zaseknout zobrazov·nÌ na displej
	if (D3GL0Win != NULL)
	{
		::MoveWindow(D3GL0Win, 32, 32, 32, 32, FALSE);
	}

	D3GL0TermCont();
	D3GL0TermPal();

	FPUDouble();			// n·vrat p¯esnosti koprocesoru

	if (D3GL0Win != NULL)
	{
		::ShowWindow(D3GL0Win, SW_HIDE);
	}

	FPUDouble();			// n·vrat p¯esnosti koprocesoru
}

/////////////////////////////////////////////////////////////////////////////
// vymaz·nÌ renderovacÌ plochy

void D3GL0Clear()
{
// aktualizace nastavenÌ barvy pozadÌ scÈny
	if (D3DBackColAkt)
	{
		pglClearColor(D3DBackColF.r, D3DBackColF.g, D3DBackColF.b, D3DBackColF.a);
		D3DBackColAkt = false;
	}

// test, zda je textura
	if (D3DBackText >= 0)
	{

// zobrazenÌ textury
		pglColor4f(1, 1, 1, 1);

		pglMatrixMode(GL_MODELVIEW);
		pglLoadIdentity();
		pglMatrixMode(GL_PROJECTION);
		pglPushMatrix();
		pglLoadIdentity();

		pglBegin(GL_TRIANGLE_STRIP);

		pglTexCoord2fv(wall_text + 2*0);
		pglVertex2i(-1, 1);

		pglTexCoord2fv(wall_text + 2*1);
		pglVertex2i(1, 1);

		pglTexCoord2fv(wall_text + 2*2);
		pglVertex2i(-1, -1);

		pglTexCoord2fv(wall_text + 2*3);
		pglVertex2i(1, -1);

		pglEnd();

		pglPopMatrix();

// z·vÏreËnÈ vymaz·nÌ Z bufferu
		pglClear(GL_DEPTH_BUFFER_BIT | (D3DSBufferClear ? GL_STENCIL_BUFFER_BIT : 0));
	}
	else
	{

// pozadÌ se neinicializuje
		if (D3DBackCol == (MAXDWORD-1))
		{
			if (D3DSBufferClear)
			{
				pglClear(GL_STENCIL_BUFFER_BIT);
			}
			return;
		}

// nenÌ textura, je barva - vymaz·nÌ barvy i Z bufferu
		if (D3DBackCol != MAXDWORD)
		{
			pglClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | (D3DSBufferClear ? GL_STENCIL_BUFFER_BIT : 0));
		}
		else

// jinak pouze vymaz·nÌ Z bufferu
		{
			pglClear(GL_DEPTH_BUFFER_BIT | (D3DSBufferClear ? GL_STENCIL_BUFFER_BIT : 0));
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// zobrazenÌ renderovacÌho bufferu

void D3GL0Disp()
{
	if ((D3GL0Win == NULL) || (D3GL0Cont == NULL)) return;
	HDC dc = ::GetDC(D3GL0Win);
	SwapBuffers(dc);
	::ReleaseDC(D3GL0Win, dc);
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace projekËnÌ matice

void D3GL0AktProj()
{
	pglMatrixMode(GL_PROJECTION);
	pglLoadMatrixf((float*)&D3DProjMatrix);
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace pohledovÈ matice (v OpenGL se prov·dÌ u kaûdÈho objektu zvl·öù)

void D3GL0AktView() { }


/////////////////////////////////////////////////////////////////////////////
// zah·jenÌ renderov·nÌ scÈny

void D3GL0Begin()
{ 
// p¯Ìprava transformaËnÌ matice pro aktualizaci svÏtel
	pglMatrixMode(GL_MODELVIEW);
	pglLoadMatrixf((float*)&D3DViewMatrix);
}


/////////////////////////////////////////////////////////////////////////////
// ukonËenÌ renderov·nÌ scÈny

void D3GL0End() { }


/////////////////////////////////////////////////////////////////////////////
// vyrenderov·nÌ jednoho objektu

void D3GL0Render(D3DFITEM* item)
{
	pglMatrixMode(GL_MODELVIEW);
	pglEnable(GL_COLOR_MATERIAL);

	int* face = item->Face;

	bool isFlare = (item->Type == D3DFTYPE_LENSFLARE);
	bool isPic = ((item->Type == D3DFTYPE_PICTURE) || isFlare);
	int j = item->FaceNum * 3;

	if (isPic)
	{
		pglLoadIdentity();
		pglMatrixMode(GL_PROJECTION);
		pglPushMatrix();
		pglLoadIdentity();

		double sinz1 = item->SinZ*D3DHeight/D3DWidth;
		double sinz2 = item->SinZ*D3DWidth/D3DHeight;

		double sx = item->TransX;
		double sy = item->TransY;

		if (isFlare)
		{
			D3DVECTOR vv;
			vv.x = item->MatrixWorld._41;
			vv.y = item->MatrixWorld._42;
			vv.z = item->MatrixWorld._43;

			VecXMat(&vv, &vv, &(D3DViewMatrix));

			if (vv.z > 0.001)
			{
				sx = vv.x/vv.z * item->ScaleZ / D3DWidthHeight2;
				sy = vv.y/vv.z * item->ScaleZ * D3DWidthHeight2;
			}
			else
			{
				sx = -1;
				sy = -1;
			}
		}

		pglBegin(GL_TRIANGLES);

		for (; j > 0; j--)
		{
			int vert = *face;
			pglTexCoord2fv(item->TextUV[0] + 2*vert);

			double x = item->Vertex[vert].x*item->ScaleX;
			double y = item->Vertex[vert].y*item->ScaleY;

			pglVertex2f((float)(2*(x*item->CosZ - y*sinz1 + sx) - 0.25/D3DWidth),
						(float)(2*(x*sinz2 + y*item->CosZ + sy) - 0.25/D3DHeight));
			face++;
		}

		pglEnd();

		pglPopMatrix();
	}
	else
	{
		pglLoadMatrixf((float*)&D3DViewMatrix);
		pglMultMatrixf((float*)&item->MatrixWorld);

		pglBegin(GL_TRIANGLES);

		if ((item->VertColF != NULL) && (item->MatSource == MATSOURCE_OBJECT))
		{
			for (; j > 0; j--)
			{
				int vert = *face;
				pglColor4fv((float*)((float*)(item->VertColF + vert)));
				pglTexCoord2fv(item->TextUV[0] + 2*vert);
				pglNormal3fv((float*)(item->Normal + vert));
				pglVertex3fv((float*)(item->Vertex + vert));
				face++;
			}
		}
		else
		{
			for (; j > 0; j--)
			{
				int vert = *face;
				pglTexCoord2fv(item->TextUV[0] + 2*vert);
				pglNormal3fv((float*)(item->Normal + vert));
				pglVertex3fv((float*)(item->Vertex + vert));
				face++;
			}
		}
		pglEnd();
	}
}

/////////////////////////////////////////////////////////////////////////////
// vyrenderov·nÌ stÌnu

void D3GL0Shadow(D3DFITEM* item)
{
	if (D3DSBufferFormatOK)
	{
		pglColorMask(0, 0, 0, 0);
	}
	else
	{
		pglColor4f(0.5, 0.5, 0.5, (float)0.6666666666);
	}

	pglMatrixMode(GL_MODELVIEW);
	pglEnable(GL_COLOR_MATERIAL);

	pglLoadMatrixf((float*)&D3DViewMatrix);
	pglMultMatrixf((float*)&item->MatrixWorld);

	D3DVECTOR* src = item->ShadowVolume;

	for (int i = 0; i < item->ShadowFanN; i++)
	{
		pglBegin(GL_TRIANGLE_FAN);

		for (int j = item->ShadowFan[i]; j > 0; j--)
		{
			pglVertex3fv((float*)src);
			src++;
		}

		pglEnd();
	}

	if (D3DSBufferFormatOK)
	{
		pglColorMask(1, 1, 1, 1);
	}
}

/////////////////////////////////////////////////////////////////////////////
// resetov·nÌ objektu p¯ed ukonËenÌm ovladaËe

void D3GL0Reset(D3DFITEM* item)
{

}

/////////////////////////////////////////////////////////////////////////////
// aktualizovat p¯epÌnaË vyplÚov·nÌ ploch

void D3GL0AktWireframe()
{
	pglPolygonMode(GL_FRONT_AND_BACK, (D3DWireframeAkt ? GL_FILL : GL_LINE));
}


/////////////////////////////////////////////////////////////////////////////
// aktualizovat p¯epÌnaË osvÏtlenÌ

void D3GL0AktLightOn()
{
	if (D3DLightOnAkt)
	{
		pglEnable(GL_LIGHTING);
	}
	else
	{
		pglDisable(GL_LIGHTING);
	}
}


/////////////////////////////////////////////////////////////////////////////
// aktualizovat p¯epÌnaË plynulÈho stinov·nÌ

void D3GL0AktShades()
{
	pglShadeModel(D3DShadesAkt ? GL_SMOOTH : GL_FLAT);
}


/////////////////////////////////////////////////////////////////////////////
// aktualizovat p¯epÌnaË odstraÚov·nÌ ploch

void D3GL0AktCulling()
{
	if (D3DCullingAkt == 0)
	{
		pglDisable(GL_CULL_FACE);
	}
	else
	{
		pglEnable(GL_CULL_FACE);
		pglCullFace(GL_BACK);
		pglFrontFace((D3DCullingAkt == 1) ? GL_CW : GL_CCW);
	}
}


/////////////////////////////////////////////////////////////////////////////
// aktualizovat p¯epÌnaË filtrace zvÏtöen˝ch textur

void D3GL0AktMagFilter(int stage)
{
	if (stage != 0) return;
	pglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (D3DMagFilterAkt[0] ? GL_LINEAR : GL_NEAREST));
}


/////////////////////////////////////////////////////////////////////////////
// aktualizovat p¯epÌnaË filtrace zmenöen˝ch a vzd·len˝ch textur

void D3GL0AktMinMipFilter(int stage)
{
	if (stage != 0) return;

	int value;

	if (D3DMinFilterAkt[0])
	{
		value = GL_LINEAR;

		if (D3DMipFilterAkt[0])
		{
			value = GL_LINEAR_MIPMAP_LINEAR;
		}
	}
	else
	{
		value = GL_NEAREST;

		if (D3DMipFilterAkt[0])
		{
			value = GL_NEAREST_MIPMAP_NEAREST;
		}
	}

	pglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, value);
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace poloûky svÏtla

void D3GL0AktLight(int inx, D3DLITEM* item)
{
	int n = GL_LIGHT0 + inx;
	D3DLIGHT8* light = &(item->Light);
	D3DLIGHTTYPE type = light->Type;

// oprava pozice a smÏru
	if (item->Enable)
	{
		float v[4];
	
		if (type == D3DLIGHT_DIRECTIONAL)
		{
			v[0] = -light->Direction.x;
			v[1] = -light->Direction.y;
			v[2] = -light->Direction.z;
			v[3] = 0;
		}
		else
		{
			v[0] = light->Position.x;
			v[1] = light->Position.y;
			v[2] = light->Position.z;
			v[3] = 1;
		}

		pglLightfv(n, GL_POSITION, v);
		
		pglLightfv(n, GL_SPOT_DIRECTION, (float*)&light->Direction);
	}

// oprava svÏteln˝ch parametr˘
	if (item->Akt)
	{
		if (item->Enable)
		{
			pglLightfv(n, GL_AMBIENT, (float*)&light->Ambient);
			pglLightfv(n, GL_DIFFUSE, (float*)&light->Diffuse);
			pglLightfv(n, GL_SPECULAR, (float*)&light->Specular);

			float expo = 0;
			pglLightfv(n, GL_SPOT_EXPONENT, &expo);

			float angle = 180;
			if (type == D3DLIGHT_SPOT) angle = (float)((light->Phi + light->Theta) * raddeg * 0.25);
			pglLightfv(n, GL_SPOT_CUTOFF, &angle);

			pglLightfv(n, GL_CONSTANT_ATTENUATION, &light->Attenuation0);
			pglLightfv(n, GL_LINEAR_ATTENUATION, &light->Attenuation1);
			pglLightfv(n, GL_QUADRATIC_ATTENUATION, &light->Attenuation2);
			 
			pglEnable(n);
		}
		else
		{
			pglDisable(n);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavenÌ materi·lu

void D3GL0SetMaterial(D3DMITEM* mat)
{
	pglMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (float*)&(mat->Material.Diffuse));
	pglMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (float*)&(mat->Material.Ambient));
	pglMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (float*)&(mat->Material.Specular));
	pglMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, (float*)&(mat->Material.Emissive));

	float power = (float)mat->Material.Power;
	pglMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &power);

	pglColor4fv((float*)&(mat->Material.Diffuse));
}


/////////////////////////////////////////////////////////////////////////////
// resetov·nÌ poloûky textury p¯ed ukonËenÌm ovladaËe

void D3GL0TextureRes(D3DTITEM* item) { }

////////////////////////////////////////////////////////////////////
// zobrazenÌ textury OpenGL

void GLTexture(PICTUREDATA* picdata, BOOL mips)
{
	if (picdata->TextDataRGBA != NULL)
	{
		int width = picdata->TextWidth;
		int height = picdata->TextHeight;
		int mip = picdata->TextMipMaps;
		if (!mips) mip = 1;

		int m = 0;
		BYTE* src = picdata->TextDataRGBA;

		for (; mip > 0; mip--)
		{
			pglTexImage2D(
				GL_TEXTURE_2D,
				m,
				4,
				width,
				height,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				src
				);

			src += (width*height*4);
			m++;
			width = (width >> 1);
			if (width == 0) width = 1;
			height = (height >> 1);
			if (height == 0) height = 1;
		}

		if (mips)
		{
			pglTexImage2D(
				GL_TEXTURE_2D,
				m,
				4,
				0,
				0,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				src
				);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavenÌ a zapnutÌ textury

void D3GL0TextureSet(D3DTITEM* item, int stage)
{
	if (stage != 0) return;

	pglEnable(GL_TEXTURE_2D);
	PICTUREDATA* picdata = item->Pic.Data();
	GLTexture(picdata, D3DMipFilterAkt && item->Mips);
	ResetD3DSwitch();
}

/////////////////////////////////////////////////////////////////////////////
// vypnutÌ textury

void D3GL0TextureOff(int stage)
{
	if (stage != 0) return;
	pglDisable(GL_TEXTURE_2D);
}

/////////////////////////////////////////////////////////////////////////////
// zapnutÌ textury

void D3GL0TextureOn(D3DTITEM* item, int stage)
{
	if (stage != 0) return;
	pglEnable(GL_TEXTURE_2D);
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace ambient osvÏtlenÌ

void D3GL0AktAmbient()
{
	pglLightModelfv(GL_LIGHT_MODEL_AMBIENT, (float*)&D3DAmbientColF);
}

/////////////////////////////////////////////////////////////////////////////
// voln· videopamÏù

int D3GL0Free()
{
	return (4*1024*1024);
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace mlhy

void D3GL0AktFog()
{
	pglHint(GL_FOG_HINT, (FogKorig ? GL_FASTEST : GL_NICEST));

	float x;
	switch (FogType)
	{
	case 0:
		x = GL_LINEAR;
		break;

	case 1:
		x = GL_EXP;
		break;

	default:
		x = GL_EXP2;
	}

	pglFogfv(GL_FOG_MODE, &x);

	x = (float)FogDens;
	pglFogfv(GL_FOG_DENSITY, &x);

	x = (float)FogStart;
	pglFogfv(GL_FOG_START, &x);

	x = (float)FogEnd;
	pglFogfv(GL_FOG_END, &x);

	pglFogfv(GL_FOG_COLOR, (float*)&FogColorF);
}

/////////////////////////////////////////////////////////////////////////////
// zapnutÌ mlhy

void D3GL0FogOn(BOOL on)
{
	if (on)
	{
		pglEnable(GL_FOG);
	}
	else
	{
		pglDisable(GL_FOG);
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace stavu

void D3GL0AktState()
{
	pglClearStencil(0);
	pglColorMask(1, 1, 1, 1);

	pglPixelStorei(GL_PACK_ALIGNMENT, 1);
	pglPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	pglHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
	pglHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
	pglHint(GL_POLYGON_SMOOTH_HINT, (D3DMinFilterAkt ? GL_NICEST : GL_FASTEST));
	pglHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	pglDepthRange(0, 1);
	pglEnable(GL_AUTO_NORMAL);
	pglDisable(GL_COLOR_MATERIAL);
	pglEnable(GL_NORMALIZE);

	float model = 0;
	pglLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, &model);
	model = 1;
	pglLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, &model);

	//pglLineWidth(1);
	pglDisable(GL_LINE_SMOOTH);
	pglDisable(GL_LINE_STIPPLE);
	pglDisable(GL_POLYGON_SMOOTH);
	pglDisable(GL_POLYGON_STIPPLE);

	if (ScreenBits < 20)
	{
		pglEnable(GL_DITHER);
	}
	else
	{
		pglDisable(GL_DITHER);
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace blending operacÌ objektu

void D3GL0AktBlend(D3DFITEM* item)
{
	int src = BlendTabOGL[item->SrcBlend];
	int dst = BlendTabOGL[item->DstBlend];

	if (src == GL_SRC_COLOR) src = GL_SRC_ALPHA;
	if (src == GL_ONE_MINUS_SRC_COLOR) src = GL_ONE_MINUS_SRC_ALPHA;
	if (dst == GL_DST_COLOR) dst = GL_DST_ALPHA;
	if (dst == GL_ONE_MINUS_DST_COLOR) dst = GL_ONE_MINUS_DST_ALPHA;

	if (IsBlending)
	{
		pglBlendFunc(src, dst);
	}

	if (item->IsBlend && IsBlending)
	{
		pglEnable(GL_BLEND);
	}
	else
	{
		pglDisable(GL_BLEND);
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace referenËnÌ ˙rovnÏ alfa

void D3GL0AktAlfa(D3DFITEM* item)
{
	if (item->IsAlphaRef)
	{
		pglAlphaFunc(GL_GREATER, (float)item->AlphaRef);
		pglEnable(GL_ALPHA_TEST);
	}
	else
	{
		pglDisable(GL_ALPHA_TEST);
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavenÌ operace barev

void D3GL0SetColorOp(int op, int arg1, int arg2, int stage)
{
	if (stage != 0) return;

	int n = GL_MODULATE;

	switch (op)
	{
	case 1:				// D3DTOP_DISABLE:
		n = -1;
		break;

	case 2:				// D3DTOP_SELECTARG1:
		n = -1;
		if ((arg1 & 0xf) == 2)	// D3DTA_TEXTURE
		{
			n = GL_DECAL;
		}
		break;

	case 3:				// D3DTOP_SELECTARG2:
		n = -1;
		if ((arg2 & 0xf) == 2)	// D3DTA_TEXTURE
		{
			n = GL_DECAL;
		}
		break;
		
	case 7:				// D3DTOP_ADD
	case 8:				// D3DTOP_ADDSIGNED
	case 9:				// D3DTOP_ADDSIGNED2X
		n = GL_DECAL;
		break;
	}

	if (n == -1)
	{
		D3DT_Akt(-1, 0);
	}
	else
	{
		pglTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, n);
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavenÌ adresov·nÌ textur

void D3GL0SetAddress(int adru, int adrv, int stage)
{
	if (stage != 0) return;

	int u;
	int v;

	switch (adru)
	{
	case D3DTADDRESS_WRAP:
	case D3DTADDRESS_MIRROR:
		u = GL_REPEAT;
		break;

	default:
		u = GL_CLAMP;
	}

	switch (adrv)
	{
	case D3DTADDRESS_WRAP:
	case D3DTADDRESS_MIRROR:
		v = GL_REPEAT;
		break;

	default:
		v = GL_CLAMP;
	}

	pglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, u);
	pglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, v);
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace palet

void D3GL0AktPalette(HWND wnd)
{
	if (D3GL0Cont == NULL) return;

	HDC dc = ::GetDC(D3GL0Win);
	pwglMakeCurrent(dc, D3GL0Cont);

	if (D3D2D)
	{
		::ReleaseDC(D3GL0Win, dc);
		D3NoAktPalette(wnd);
	}
	else
	{
		if (D3GL0Pal)
		{
			HPALETTE oldpal = ::SelectPalette(dc, D3GL0Pal, FALSE);
			::RealizePalette(dc);
			::SelectPalette(dc, oldpal, FALSE);
		}
		::ReleaseDC(D3GL0Win, dc);
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavenÌ Z funkce (1 aû 8, v kÛdu DirectX, 0=vypnuto)

void D3GL0SetZFunc(int func)
{
	if (func == 0)
	{
		pglDepthFunc(GL_ALWAYS);
		pglDisable(GL_DEPTH_TEST);
	}
	else
	{
		pglEnable(GL_DEPTH_TEST);
		pglDepthFunc(func + (GL_NEVER - D3DCMP_NEVER));
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavenÌ Z z·pisu

void D3GL0SetZWrite(BOOL write)
{
	pglDepthMask((GLboolean)write);
}

/////////////////////////////////////////////////////////////////////////////
// povolenÌ stencil operacÌ

void D3GL0SEnable(BOOL enable)
{
	if (enable)
	{
		pglEnable(GL_STENCIL_TEST);
	}
	else
	{
		pglDisable(GL_STENCIL_TEST);
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavenÌ stencil operacÌ

void D3GL0SetSOp(int fail, int zfail, int zpass)
{
	pglStencilOp(StencilTabOGL[fail], StencilTabOGL[zfail], StencilTabOGL[zpass]);
}

/////////////////////////////////////////////////////////////////////////////
// nastavenÌ stencil testu

void D3GL0SetSFunc(int func, int ref, DWORD mask)
{
	pglStencilFunc(func - D3DCMP_NEVER + GL_NEVER, ref, mask);
}

/////////////////////////////////////////////////////////////////////////////
// nastavenÌ z·pisovÈ masky do stencil bufferu

void D3GL0SetSMask(DWORD mask)
{
	pglStencilMask(mask);
}

/////////////////////////////////////////////////////////////////////////////
// vykreslenÌ stÌn˘

void D3GL0Shadows()
{
	pglColor4f(0.5, 0.5, 0.5, 0.5);

	pglMatrixMode(GL_MODELVIEW);
	pglLoadIdentity();
	pglMatrixMode(GL_PROJECTION);
	pglPushMatrix();
	pglLoadIdentity();

	pglBegin(GL_TRIANGLE_STRIP);

	pglVertex2i(-1, 1);
	pglVertex2i(1, 1);
	pglVertex2i(-1, -1);
	pglVertex2i(1, -1);

	pglEnd();

	pglPopMatrix();
}


#endif // _MINI
// --------------------- konec vypnutÌ pro MINI verzi -------------------

