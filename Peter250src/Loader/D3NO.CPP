
#include "Main.h"

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

/***************************************************************************\
*																			*
*							3D rozhraní - nulové							*
*																			*
\***************************************************************************/

void D3No() { }

void D3NoRender(D3DFITEM* item) { }

void D3NoAktLight(int inx, D3DLITEM* item) { }

void D3NoSetMaterial(D3DMITEM* item) { }

void D3NoTextureRes(D3DTITEM* item) { }

void D3NoAktMagFilter(int stage) { }

void D3NoTextureSet(D3DTITEM* item, int stage) { }

void D3NoSetColorOp(int op, int arg1, int arg2, int stage) { }

void D3NoSetAddress(int adru, int adrv, int stage) { };

//void D3NoSetBorder(D3DFITEM* item, int stage) { };

void D3NoSetBias(float bias, int stage) { };

int D3NoFree() { return 0; }


/////////////////////////////////////////////////////////////////////////////
// start videomódu (vrací TRUE=OK)

bool D3NoStartVMode(VIDEOMODEITEM* item)
{
	return false;
}

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------

/////////////////////////////////////////////////////////////////////////////
// aktualizace palet

void D3NoAktPalette(HWND wnd)
{
	HDC dc = ::GetDC(wnd);
//	::UnrealizeObject(StdPalette);
	HPALETTE oldPal = ::SelectPalette(dc, StdPalette, FALSE);
	::RealizePalette(dc);
	::SelectPalette(dc, oldPal, FALSE);
	::ReleaseDC(wnd, dc);

}

