
// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

#define	DIRECT3D_VERSION 0x0400

#include "Main.h"

DEFINE_GUID( IID_IDirect3DHALDevice,    0x84E63dE0,0x46AA,0x11CF,0x81,0x6F,0x00,0x00,0xC0,0x20,0x15,0x6E );
DEFINE_GUID( IID_IDirect3DMMXDevice,    0x881949a1,0xd6f3,0x11d0,0x89,0xab,0x00,0xa0,0xc9,0x05,0x41,0x29 );
DEFINE_GUID( IID_IDirect3DRampDevice,   0xF2086B20,0x259F,0x11CF,0xA3,0x1A,0x00,0xAA,0x00,0xB9,0x33,0x56 );
DEFINE_GUID( IID_IDirect3DRGBDevice,    0xA4665C60,0x2673,0x11CF,0xA3,0x1A,0x00,0xAA,0x00,0xB9,0x33,0x56 );

/***************************************************************************\
*																			*
*							3D rozhraní - DirectX 3							*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// deklarace

// funkce DirectDrawCreate
typedef	HRESULT (WINAPI *DIRECTDRAWCREATE) (GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter);

#define D3DCOLOR_MONO	1
#define D3DCOLOR_RGB	2

/////////////////////////////////////////////////////////////////////////////
// data

// DirectDraw 3
bool				D3DX3DDTest			= false;	// probìhl test rozhraní DirectDraw 3
HINSTANCE			D3DX3DDLib			= NULL;		// handle modulu DDRAW.DLL
DIRECTDRAWCREATE	pDirectDrawCreate	= NULL;		// funkce DirectDrawCreate (NULL=není)
IDirectDraw*		D3DX3DD				= NULL;		// objekt DirectDraw 3 (NULL=není)

// Direct3D	3
bool				D3DX3D3Test			= false;	// probìhl test rozhraní Direct3D 3
IDirect3D*			D3DX3D3				= NULL;		// objekt Direct3D 3 (NULL=není)
const GUID* D3DX3DevTab[D3DEVICES];					// tabulka identifikátorù ovladaèù

// objekty
IDirectDrawSurface*	D3DX3Prim			= NULL;		// primární povrch
IDirectDrawClipper*	D3DX3Clip			= NULL;		// clipper
IDirectDrawPalette*	D3DX3Pal			= NULL;		// palety
IDirectDrawSurface*	D3DX3Rend			= NULL;		// renderovací povrch
int					D3DX3RendWidth		= 0;		// aktuální šíøka renderovacího povrchu	
int					D3DX3RendHeight		= 0;		// aktuální výška renderovacího povrchu	
IDirectDrawSurface*	D3DX3ZBuf			= NULL;		// Z buffer
IDirect3DDevice*	D3DX3Dev			= NULL;		// renderovací zaøízení
IDirect3DViewport*	D3DX3View			= NULL;		// viewport

// cílové souøadnice pro vykreslování v souøadnicích displeje
RECT				D3DX3Dest;						// cílová oblast pro vykreslování
D3DRECT				D3DX3Src;						// zdrojová oblast viewportu
D3DVIEWPORT			D3DX3ViewBuf;					// buffer nastavení pro viewport

// pozadí scény
IDirect3DMaterial*	D3DX3BackMat	= NULL;			// materiál pozadí scény
D3DMATERIAL			D3DX3BackMatBuf;				// buffer materiálu pozadí scény

// transformaèní matice
D3DMATRIXHANDLE		D3DX3WrldMat		= 0;		// matice transformace objektu
D3DMATRIXHANDLE		D3DX3ProjMat		= 0;		// matice projekèní transformace
D3DMATRIXHANDLE		D3DX3ViewMat		= 0;		// matice pohledové transformace

// formát textur
//DDPIXELFORMAT		D3DX3TextFormat;				// použitý formát textur

IDirect3DExecuteBuffer* D3DX3State		= NULL;		// pøíkaz pro obnovení stavu

IDirect3DExecuteBuffer* D3DX3TextNull	= NULL;		// pøíkaz pro vypnutí textury

/////////////////////////////////////////////////////////////////////////////
// inicializace rozhraní DirectDraw 3 (vrací TRUE=rozhraní je vytvoøeno)

bool D3DX3InitDD()
{
// test, zda již byla provádìna inicializace
	if (!D3DX3DDTest)
	{
		D3DX3DDTest = true;

// inicializace funkce DirectDrawCreate (mìní pøesnost koprocesoru!)
		if (D3DX3DDLib == NULL)
		{
			D3DX3DDLib = ::LoadLibrary(_T("DDRAW.DLL"));
		}

		if (D3DX3DDLib != NULL)
		{
			pDirectDrawCreate = (DIRECTDRAWCREATE)::GetProcAddress(D3DX3DDLib, "DirectDrawCreate");
		}

// vytvoøení objektu DirectDraw 3
		if (pDirectDrawCreate != NULL)
		{
			if (pDirectDrawCreate(NULL, &D3DX3DD, NULL) != DD_OK)
			{
				D3DX3DD = NULL;
			}
			else
			{
				D3DX3DD->SetCooperativeLevel(MainFrame, DDSCL_NORMAL);
			}
		}

		FPUDouble();
	}

// test, zda je objekt DirectDraw 3 vytvoøen
	return (D3DX3DD != NULL);
}

/////////////////////////////////////////////////////////////////////////////
// ukonèení rozhraní DirectDraw 3

void D3DX3TermDD()
{
// ukonèení rozhraní Direct3D 3
	D3DX3TermD3();

// ukonèení závislého rozhraní DirectDraw 5
	D3DX5TermDD();

// zrušení viewportu
	D3DX3TermView();

// zrušení Z bufferu
	D3DX3TermZBuf();

// zrušení renderovacího povrchu
	D3DX3TermRend();

// ukonžení palet
	D3DX3TermPal();

// ukonèení clipperu
	D3DX3TermClip();

// ukonèení primárního povrchu
	D3DX3TermPrim();

// zrušení objektu DirectDraw 3
	if (D3DX3DD != NULL)
	{
		D3DX3DD->Release();
		D3DX3DD = NULL;
	}

// uvolnìní knihovny DirectDraw
	if (D3DX3DDLib != NULL)
	{
		::FreeLibrary(D3DX3DDLib);
		D3DX3DDLib = NULL;
	}
	pDirectDrawCreate = NULL;

// zrušení pøíznaku, že byla inicializace
	D3DX3DDTest = false;
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení primárniho povrchu, není-li vytvoøen (vrací TRUE=OK)

bool D3DX3InitPrim()
{
	if (D3DX3Prim == NULL)
	{
		DDSURFACEDESC ddsd;
		MemFill(&ddsd, sizeof(ddsd), 0);
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

		if (!D3DX3InitDD() ||
			(D3DX3DD->CreateSurface(&ddsd, &D3DX3Prim, NULL) != DD_OK) ||
			(D3DX3Prim == NULL))
		{
			D3DX3Prim = NULL;
			return false;
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení primárního povrchu

void D3DX3TermPrim()
{
	if (D3DX3Prim != NULL)
	{
		D3DX3Prim->Release();
		D3DX3Prim = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení clipperu, není-li vytvoøen (vrací TRUE=OK)

bool D3DX3InitClip()
{
	if (D3DX3Clip == NULL)
	{
		if (!D3DX3InitPrim() ||
			(D3DX3DD->CreateClipper(0, &D3DX3Clip, NULL) != DD_OK) ||
			(D3DX3Clip == NULL) ||
			(D3DX3Clip->SetHWnd(0, MainFrame) != DD_OK) ||
			(D3DX3Prim->SetClipper(D3DX3Clip) != DD_OK))
		{
			D3DX3TermClip();
			return false;
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení clipperu (musí se volat pøed zrušením primárního povrchu)

void D3DX3TermClip()
{
	if (D3DX3Clip != NULL)
	{
		if (D3DX3Prim != NULL) D3DX3Prim->SetClipper(NULL);
		D3DX3Clip->Release();
		D3DX3Clip = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení palet, nejsou-li vytvoøeny (vrací TRUE=OK)

bool D3DX3InitPal()
{
	if (D3DX3Pal == NULL)
	{
		
// test, zda je potøeba použít palety
		if (!D3DX3InitPrim()) return false;
		DDSURFACEDESC ddsd;
		MemFill(&ddsd, sizeof(ddsd), 0);
		ddsd.dwSize = sizeof(ddsd);
		if (D3DX3Prim->GetSurfaceDesc(&ddsd) != DD_OK) return false;

		if (ddsd.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8)
		{

// vytvoøení bufferu palet
			if (SysPal == NULL)
			{
				SysPal = (PALETTEENTRY*)MemGet(sizeof(PALETTEENTRY)*256);
			}

// naètení systémových palet
			HDC dc = ::GetDC(NULL);
			if (dc)
			{
				::GetSystemPaletteEntries(dc, 0, 256, SysPal);
				::ReleaseDC(NULL, dc);
			}

// doplnìní vlastních palet
			int i;
			for (i=0; i < 256; i++) SysPal[i].peFlags = D3DPAL_READONLY;
			for (i = 10; i < 246; i++) SysPal[i].peFlags = D3DPAL_FREE | PC_RESERVED;

// vytvoøení palet
			if ((D3DX3DD->CreatePalette(DDPCAPS_8BIT, SysPal, &D3DX3Pal, NULL) != DD_OK) ||
				(D3DX3Pal == NULL))
			{
				D3DX3Pal = NULL;
				return false;
			}

// nastavení palet
			if (D3DX3Prim->SetPalette(D3DX3Pal) != DD_OK)
			{
				D3DX3TermPal();
				return false;
			}
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení palet (volat pøed zrušením primárního povrchu)

void D3DX3TermPal()
{
	if (D3DX3Pal != NULL)
	{
		D3DX3Pal->Release();
		D3DX3Pal = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// callback pro enumeraci ovladaèù

HRESULT WINAPI D3DX3EnumDeviceCallback(GUID* lpGUID, LPSTR devDesc, LPSTR devName,
									   D3DDEVICEDESC* descHW, D3DDEVICEDESC* descSW, void* user)
{
	if (lpGUID != NULL)
	{
		D3DCOLORMODEL cm = descHW->dcmColorModel;

// ovladaè HAL (pøednost má RGB)
		if (cm != 0)
		{
			if ((D3DX3DevTab[NHALDev] == NULL) || 
				((cm & D3DCOLOR_RGB) != 0))
			{
				D3DX3DevTab[NHALDev] = lpGUID;
			}
		}

		cm = descSW->dcmColorModel;

// ovladaè HEL
		if (cm != 0)
		{
			if ((cm & D3DCOLOR_RGB) != 0)
			{
				D3DX3DevTab[NRGBDev] = lpGUID;
			}

			if ((cm & D3DCOLOR_MONO) != 0)
			{
				D3DX3DevTab[NRampDev] = lpGUID;
			}
		}
	}

	return D3DENUMRET_OK;
}

/////////////////////////////////////////////////////////////////////////////
// inicializace rozhraní Direct3D 3 (vrací TRUE=rozhraní je vytvoøeno)

bool D3DX3InitD3()
{
// test, zda inicializace již probìhla
	if (!D3DX3D3Test)
	{
		D3DX3D3Test = true;

// inicializace DirectDraw 3
		if (D3DX3InitDD())
		{

// vytvoøení objektu Direct3D 3 (mìní pøesnost koprocesoru!)
			if (D3DX3DD->QueryInterface(IID_IDirect3D, (void**)&D3DX3D3) != DD_OK)
			{
				D3DX3D3 = NULL;
			}

// enumerace ovladaèù
			if (D3DX3D3 != NULL)
			{
				MemCopy(D3DX3DevTab, Dev3DTab, D3DEVICES*sizeof(GUID*));
				D3DX3DevTab[NHALDev] = NULL;
				D3DX3D3->EnumDevices(D3DX3EnumDeviceCallback, NULL);
				if (D3DX3DevTab[NHALDev] == NULL)
				{
					D3DX3DevTab[NHALDev] = Dev3DTab[NHALDev];
				}
			}
		}
	}

// test, zda je objekt Direct3D 3 vytvoøen
	return (D3DX3D3 != NULL);
}

/////////////////////////////////////////////////////////////////////////////
// ukonèení rozhraní Direct3D 3

void D3DX3TermD3()
{
// ukonèení renderování
	D3DX3Stop();

// zrušení objektu Direct3D 3
	if (D3DX3D3 != NULL)
	{
		D3DX3D3->Release();
		D3DX3D3 = NULL;
	}

// zrušení pøíznaku, že byla inicializace
	D3DX3D3Test = false;
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení renderovacího povrchu, není-li vytvoøen (vrací TRUE=OK)

bool D3DX3InitRend(bool hw)
{
	if (D3DX3Rend == NULL)
	{

// inicializace DirectDraw
		if (!D3DX3InitDD()) return false;

// pøíprava typu pamìti
		DWORD memtype = 0;
		if (hw) memtype = DDSCAPS_VIDEOMEMORY;

// pøíprava popisovaèe povrchu
		DDSURFACEDESC ddsd;
		MemFill(&ddsd, sizeof(ddsd), 0);
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
		ddsd.dwWidth = D3DWidth;
		D3DX3RendWidth = D3DWidth;
		ddsd.dwHeight = D3DHeight;
		D3DX3RendHeight = D3DHeight;
		ddsd.ddsCaps.dwCaps = DDSCAPS_3DDEVICE | DDSCAPS_OFFSCREENPLAIN | memtype;

// vytvoøení povrchu
		if ((D3DX3DD->CreateSurface(&ddsd, &D3DX3Rend, NULL) != DD_OK) ||
			(D3DX3Rend == NULL))
		{
			return false;
		}

// pøipojení palet
		if ((D3DX3Pal != NULL) &&
			(D3DX3Rend->SetPalette(D3DX3Pal) != DD_OK))
		{
			D3DX3TermRend();
			return false;
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení renderovacího povrchu

void D3DX3TermRend()
{
	D3DX3TermDev();

	if (D3DX3Rend != NULL)
	{
		D3DX3Rend->Release();
		D3DX3Rend = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení Z-bufferu, není-li vytvoøen (vrací TRUE=OK)

bool D3DX3InitZBuf(bool hw, int depth)
{
	if (D3DX3ZBuf == NULL)
	{

// kontrola renderovacího povrchu
		if (D3DX3Rend == NULL) return false;

// pøíprava typu pamìti
		DWORD memtype = DDSCAPS_SYSTEMMEMORY;
		if (hw) memtype = DDSCAPS_VIDEOMEMORY;

// pøíprava popisovaèe povrchu
		DDSURFACEDESC ddsd;
		MemFill(&ddsd, sizeof(ddsd), 0);
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_ZBUFFERBITDEPTH;
		ddsd.dwWidth = D3DWidth;
		ddsd.dwHeight = D3DHeight;
		ddsd.dwZBufferBitDepth = depth;
		ddsd.ddsCaps.dwCaps = DDSCAPS_ZBUFFER | memtype;

// vytvoøení povrchu
		if ((D3DX3DD->CreateSurface(&ddsd, &D3DX3ZBuf, NULL) != DD_OK) ||
			(D3DX3ZBuf == NULL))
		{
			return false;
		}

// pøipojení k renderovacímu povrchu
		if (D3DX3Rend->AddAttachedSurface(D3DX3ZBuf) != DD_OK)
		{
			D3DX3TermZBuf();
			return false;
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení Z bufferu

void D3DX3TermZBuf()
{
	if (D3DX3ZBuf != NULL)
	{
		D3DX3ZBuf->Release();

		if (D3DX3Rend != NULL)
		{
			D3DX3Rend->DeleteAttachedSurface(0, D3DX3ZBuf);
		}

		D3DX3ZBuf = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// callback enumerace formátu textury

HRESULT CALLBACK D3DX3EnumPixelFormatsCallback(DDSURFACEDESC* sd, void* user)
{
//	if (((sd->ddpfPixelFormat.dwFlags == (DDPF_RGB | DDPF_ALPHAPIXELS)) && 
//			(sd->ddpfPixelFormat.dwRGBBitCount == 32)))
//	{
//		MemCopy(&D3DX3TextFormat, &(sd->ddpfPixelFormat), sizeof(DDPIXELFORMAT));
//	}

	D3DEnumPixelFormat(&(sd->ddpfPixelFormat));

	return D3DENUMRET_OK;
}

/////////////////////////////////////////////////////////////////////////////
// inicializace zaøízení pro Z-buffer

bool _fastcall D3DX3InitDev2(bool hw, int type, int depth)
{
	if (D3DX3InitZBuf(hw, depth))
	{

// vytvoøení zaøízení (mìní pøesnost koprocesoru!)
		if ((D3DX3Rend->QueryInterface(*D3DX3DevTab[type], (void**)&D3DX3Dev) == DD_OK) &&
			(D3DX3Dev != NULL))
		{
			return true;
		}

		D3DX3Dev = NULL;

// zrušení Z bufferu
		D3DX3TermZBuf();
	}
	return false;
}

bool D3DX3InitDev3(bool hw, int type)
{
	return (
		D3DX3InitDev2(hw, type, 16) ||
		D3DX3InitDev2(hw, type, 24) ||
		D3DX3InitDev2(hw, type, 32) ||
		D3DX3InitDev2(hw, type, 8)
	);
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení renderovacího zaøízení (vrací TRUE=inicializace OK)
// typ zaøízení: 1=HAL,2=TnLHal,3=REF,4=RGB,5=MMX,6=Ramp,7=Null

bool D3DX3InitDev(int type)
{
// zrušení starých objektù
	D3DX3Stop();

// vytvoøení primárního povrchu, clipperu a palet
	if (D3DX3InitD3() &&
		D3DX3InitPrim() &&
		D3DX3InitClip() &&
		D3DX3InitPal())
	{

// pøíznak HW ovladaèe
		bool hw = (type <= 2);

// vytvoøení renderovacího povrchu a Z bufferu
		if (!(D3DX3InitRend(hw) || D3DX3InitRend(!hw)) ||
			!(D3DX3InitDev3(hw, type) || D3DX3InitDev3(!hw, type)))
		{
			D3DX3Stop();
			return false;
		}
		
// enumerace formátù textur
		D3DX3Dev->EnumTextureFormats(D3DX3EnumPixelFormatsCallback, NULL);
		if (!D3DRamp && !D3DTextFormatOK)
		{
			D3DX3Stop();
			return false;
		}

		FPUDouble();
		return true;
	}

// pøi chybì zrušení vytvoøených objektù
	D3DX3Stop();

	return false;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení renderovacího zaøízení

void D3DX3TermDev()
{
	if (D3DX3Dev != NULL)
	{
		D3DX3Dev->Release();
		D3DX3Dev = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace materiálu pozadí scény

void D3DX3AktBackMat()
{
	if (D3DBackColAkt)
	{
		D3DBackColAkt = false;
		D3DX3BackMatBuf.diffuse.r = D3DBackColF.r;
		D3DX3BackMatBuf.diffuse.g = D3DBackColF.g;
		D3DX3BackMatBuf.diffuse.b = D3DBackColF.b;

		D3DX3BackMatBuf.ambient.r = D3DBackColF.r;
		D3DX3BackMatBuf.ambient.g = D3DBackColF.g;
		D3DX3BackMatBuf.ambient.b = D3DBackColF.b;

		D3DX3BackMat->SetMaterial(&D3DX3BackMatBuf);
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení materiálu pozadí scény

bool D3DX3InitBack()
{
	if (D3DX3BackMat == NULL)
	{

// musí existovat 3D objekt
		if (D3DX3D3 == NULL) return false;

// vytvoøení materiálu pozadí scény
		if ((D3DX3D3->CreateMaterial(&D3DX3BackMat, NULL) != D3D_OK) ||
			(D3DX3BackMat == NULL)) return false;

// inicializace materiálu
		MemFill(&D3DX3BackMatBuf, sizeof(D3DMATERIAL), 0);
		D3DX3BackMatBuf.dwSize = sizeof(D3DMATERIAL);
		D3DX3BackMatBuf.diffuse.a = 1;
		D3DX3BackMatBuf.ambient.a = 1;
		D3DX3BackMatBuf.specular.a = 1;
		D3DX3BackMatBuf.emissive.a = 1;
		D3DX3BackMatBuf.power = 0;
		D3DX3BackMatBuf.dwRampSize = 1;
		D3DBackColAkt = true;
		D3DX3AktBackMat();
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení materiálu pozadí scény (zøejmì až po zrušení viewportu)

void D3DX3TermBack()
{
	if (D3DX3BackMat != NULL)
	{
		D3DX3BackMat->Release();
		D3DX3BackMat = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// výpoèet nových souøadnic zobrazené oblasti

void D3DX3CalcView()
{
	D3DX3Src.x2 = D3DWidth;
	D3DX3Src.y2 = D3DHeight;

	D3DX3Dest.left = D3DLeft;
	D3DX3Dest.top = D3DTop;
	::ClientToScreen(MainFrame, (POINT*)&D3DX3Dest.left);

	D3DX3Dest.right = D3DX3Dest.left + D3DWidth;
	D3DX3Dest.bottom = D3DX3Dest.top + D3DHeight;
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace viewportu po zmìnì velikosti

void D3DX3SizeView()
{
// pøepoèet videportu
	D3DX3CalcView();
	D3DX3ViewBuf.dwWidth = D3DWidth;
	D3DX3ViewBuf.dwHeight = D3DHeight;
	D3DX3ViewBuf.dvScaleX = (float)D3DWidth/2;
	D3DX3ViewBuf.dvScaleY = (float)D3DHeight/2;

// resetování bufferù 2D obrázkù
	D3DFITEM* item = D3DF_Get(1);
	int inx = item->Child;
	while (inx > 1)
	{
		item = D3DF_Get(inx);
		if ((item->Type == D3DFTYPE_PICTURE) || (item->Type == D3DFTYPE_LENSFLARE))
		{
			D3DX3Reset(item);
		}
		inx = item->Next;
	}

// zvìtšení bufferu
	if ((D3DHeight > D3DX3RendHeight) || (D3DWidth > D3DX3RendWidth))
	{
		FPUDouble();
		UserD3DDevice();
	}

// nastavení viewportu
	else
	{
		if (D3DX3Dev != NULL)
		{
			D3DX3View->SetViewport(&D3DX3ViewBuf);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace viewportu po pøesunu

void D3DX3MoveView()
{
	D3DX3CalcView();
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení viewportu (vrací TRUE=inicializace OK)

bool D3DX3InitView()
{
	if (D3DX3View == NULL)
	{

// musí existovat 3D zaøízení a 3D objekt
		if (D3DX3Dev == NULL) return false;

// vytvoøení viewportu
		if ((D3DX3D3->CreateViewport(&D3DX3View, NULL) != D3D_OK) ||
			(D3DX3View == NULL)) return false;

		D3DMATERIALHANDLE mh = NULL;

// pøipojení viewportu k 3D zaøízení
		if ((D3DX3Dev->AddViewport(D3DX3View) != D3D_OK) ||

// vytvoøení materiálu pozadí scény a pøipojení k viewportu
			!D3DX3InitBack() ||

// handle materiálu
			(D3DX3BackMat->GetHandle(D3DX3Dev, &mh) != D3D_OK) ||
			(mh == NULL) ||

// pøipojení materiálu k viewportu
			(D3DX3View->SetBackground(mh) != D3D_OK))
		{
			D3DX3TermView();
			return false;
		}

// inicializace bufferu dat viewportu
		D3DX3Src.x1 = 0;
		D3DX3Src.y1 = 0;
		D3DX3ViewBuf.dwSize = sizeof(D3DX3ViewBuf);
		D3DX3ViewBuf.dwX = 0;
		D3DX3ViewBuf.dwY = 0;
		D3DX3ViewBuf.dvMaxX = 1;
		D3DX3ViewBuf.dvMaxY = 1;
		D3DX3ViewBuf.dvMinZ = 0;
		D3DX3ViewBuf.dvMaxZ = 1;

		D3DX3SizeView();
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení viewportu

void D3DX3TermView()
{
	if (D3DX3View != NULL)
	{
		D3DX3View->Release();
		D3DX3View = NULL;
	}

	D3DX3TermBack();		// zrušení materiálu pozadí scény
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení transformaèních matic (vrací TRUE=operace OK)

bool D3DX3InitMatrix()
{
	if (D3DX3Dev == NULL) return false;

	if (D3DX3WrldMat == 0)
	{
		if (D3DX3Dev->CreateMatrix(&D3DX3WrldMat) != D3D_OK) return false;
	}

	if (D3DX3ProjMat == 0)
	{
		if (D3DX3Dev->CreateMatrix(&D3DX3ProjMat) != D3D_OK) return false;
	}

	if (D3DX3ViewMat == 0)
	{
		if (D3DX3Dev->CreateMatrix(&D3DX3ViewMat) != D3D_OK) return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení transformaèních matic

void D3DX3TermMatrix()
{
	if (D3DX3Dev != NULL)
	{
		if (D3DX3WrldMat != 0)
		{
			D3DX3Dev->DeleteMatrix(D3DX3WrldMat);
		}

		if (D3DX3ProjMat != 0)
		{
			D3DX3Dev->DeleteMatrix(D3DX3ProjMat);
		}

		if (D3DX3ViewMat != 0)
		{
			D3DX3Dev->DeleteMatrix(D3DX3ViewMat);
		}
	}

	D3DX3WrldMat = 0;
	D3DX3ProjMat = 0;
	D3DX3ViewMat = 0;
}

/////////////////////////////////////////////////////////////////////////////
// zahájení funkce zaøízení (vrací TRUE=inicializace OK)
// typ zaøízení: 1=HAL,2=TnLHal,3=REF,4=RGB,5=MMX,6=Ramp,7=Null

bool D3DX3Start(int type)
{
// vytvoøení renderovacího zaøízení
	if (!D3DX3InitDev(type)) return false;

// vytvoøení viewportu, vytvoøení transformaèních matic
	if (!D3DX3InitView() ||
		!D3DX3InitMatrix())
	{
		D3DX3Stop();
		return false;
	}

// inicializace informací o zaøízení
	D3DDEVICEDESC D3DCapsHW;
	D3DDEVICEDESC D3DCapsSW;
	MemFill(&D3DCapsHW, sizeof(D3DDEVICEDESC), 0);
	D3DCapsHW.dwSize = sizeof(D3DDEVICEDESC);
	MemFill(&D3DCapsSW, sizeof(D3DDEVICEDESC), 0);
	D3DCapsSW.dwSize = sizeof(D3DDEVICEDESC);

	D3DX3Dev->GetCaps(&D3DCapsHW, &D3DCapsSW);

	D3DDEVICEDESC* D3DCaps;
	if (D3DCapsHW.dcmColorModel != 0)
	{
		D3DCaps = &D3DCapsHW;
	}
	else
	{
		D3DCaps = &D3DCapsSW;
	}

	SquareTexture = ((D3DCaps->dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_SQUAREONLY) != 0);
	TexturePow2 = ((D3DCaps->dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_POW2) != 0);
	IsBlending = ((D3DCaps->dpcTriCaps.dwDestBlendCaps & (D3DPBLENDCAPS_ONE | D3DPBLENDCAPS_INVSRCALPHA)) != 0);
//					(type >= 1) && (type <= 3));
	IsMagFilter = ((D3DCaps->dpcTriCaps.dwTextureFilterCaps & D3DPTFILTERCAPS_LINEAR) != 0);
//					(type >= 1) && (type <= 3));
	IsMinFilter = IsMagFilter;
	IsMipMap = ((D3DCaps->dpcTriCaps.dwTextureFilterCaps & 
					(D3DPTFILTERCAPS_MIPNEAREST | D3DPTFILTERCAPS_MIPLINEAR)) != 0);
//					(type >= 1) && (type <= 3));
	AlphaGreaterEqual = ((D3DCaps->dpcTriCaps.dwAlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL) != 0);
	AlphaGreater = ((D3DCaps->dpcTriCaps.dwAlphaCmpCaps & D3DPCMPCAPS_GREATER) != 0);

	IsHWRaster = (type <= 2);

	if (D3DCaps->dwMaxVertexCount > 0)
	{
		MaxVertexNum = D3DCaps->dwMaxVertexCount;
	}
	MaxTextureStages = 1;
//	MaxTextureSimult = 1;

	bool isfog = ((D3DCaps->dpcTriCaps.dwShadeCaps & D3DPSHADECAPS_FOGGOURAUD) != 0);
	IsTableFog = ((D3DCaps->dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_FOGTABLE) != 0) && isfog;
	IsVertexFog = false;
	IsRangeFog = false;

// ----- nastavení globálních pøepínaèù pomocí provádìcího bufferu

// pøíprava velikosti bufferu
	int size = 	3*sizeof(D3DINSTRUCTION) + 8*sizeof(D3DSTATE);

// pøíprava popisovaèe dat
	D3DEXECUTEBUFFERDESC d;
	d.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
	d.dwFlags = D3DDEB_BUFSIZE;
	d.dwBufferSize = size;

// vytvoøení provádìcího bufferu
	if (D3DX3Dev->CreateExecuteBuffer(&d, &D3DX3State, NULL) == D3D_OK)
	{

// naplnìní provádìcího bufferu
		if ((D3DX3State->Lock(&d) == D3D_OK) && (d.lpData != NULL))
		{

// nastavení transformaèních matic
			D3DINSTRUCTION* ins = (D3DINSTRUCTION*)(d.lpData);
			ins->bOpcode = D3DOP_STATETRANSFORM;
			ins->bSize = sizeof(D3DSTATE);
			ins->wCount = 3;
			ins++;
			D3DSTATE* state = (D3DSTATE*)ins;
			state->dtstTransformStateType = D3DTRANSFORMSTATE_WORLD;
			state->dwArg[0] = D3DX3WrldMat;
			state++;
			state->dtstTransformStateType = D3DTRANSFORMSTATE_VIEW;
			state->dwArg[0] = D3DX3ViewMat;
			state++;
			state->dtstTransformStateType = D3DTRANSFORMSTATE_PROJECTION;
			state->dwArg[0] = D3DX3ProjMat;
			state++;

// nastavení pøepínaèù
			ins = (D3DINSTRUCTION*)state;
			ins->bOpcode = D3DOP_STATERENDER;
			ins->bSize = sizeof(D3DSTATE);
			ins->wCount = 5;
			ins++;
			state = (D3DSTATE*)ins;
			state->drstRenderStateType = D3DRENDERSTATE_TEXTUREPERSPECTIVE;
			state->dwArg[0] = TRUE;
			state++;
			state->drstRenderStateType = D3DRENDERSTATE_SPECULARENABLE;
			state->dwArg[0] = TRUE;
			state++;
			state->drstRenderStateType = D3DRENDERSTATE_DITHERENABLE;
			state->dwArg[0] = ((ScreenBits < 20) ? TRUE : FALSE);
			state++;
			state->drstRenderStateType = D3DRENDERSTATE_WRAPU;
			state->dwArg[0] = FALSE;
			state++;
			state->drstRenderStateType = D3DRENDERSTATE_WRAPV;
			state->dwArg[0] = FALSE;
			state++;

// ukonèení provádìcího bufferu
			ins = (D3DINSTRUCTION*)state;
			ins->bOpcode = D3DOP_EXIT;
			ins->bSize = 0;
			ins->wCount = 0;

			ASSERT((BYTE*)(ins + 1) == ((BYTE*)(d.lpData) + size));

// odemknutí provádìcího bufferu
			D3DX3State->Unlock();

// nastavení parametrù provádìcího bufferu
			D3DEXECUTEDATA ed;
			MemFill(&ed, sizeof(ed), 0);
			ed.dwSize = sizeof(D3DEXECUTEDATA);
			ed.dwInstructionLength = size;
			D3DX3State->SetExecuteData(&ed);
		}
	}
	else
	{
		D3DX3Stop();
		return false;
	}

// ------- provádìcí buffer pro vypnutí textury

// pøíprava velikosti bufferu
	size = 	2*sizeof(D3DINSTRUCTION) + sizeof(D3DSTATE);

// pøíprava popisovaèe dat
	d.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
	d.dwFlags = D3DDEB_BUFSIZE;
	d.dwBufferSize = size;

// vytvoøení provádìcího bufferu
	if (D3DX3Dev->CreateExecuteBuffer(&d, &D3DX3TextNull, NULL) == D3D_OK)
	{

// naplnìní provádìcího bufferu
		if ((D3DX3TextNull->Lock(&d) == D3D_OK) && (d.lpData != NULL))
		{
			D3DINSTRUCTION* ins = (D3DINSTRUCTION*)(d.lpData);
			ins->bOpcode = D3DOP_STATERENDER;
			ins->bSize = sizeof(D3DSTATE);
			ins->wCount = 1;
			ins++;
			D3DSTATE* state = (D3DSTATE*)ins;
			state->drstRenderStateType = D3DRENDERSTATE_TEXTUREHANDLE;
			state->dwArg[0] = NULL;
			state++;

// ukonèení provádìcího bufferu
			ins = (D3DINSTRUCTION*)state;
			ins->bOpcode = D3DOP_EXIT;
			ins->bSize = 0;
			ins->wCount = 0;

			ASSERT((BYTE*)(ins + 1) == ((BYTE*)(d.lpData) + size));

// odemknutí provádìcího bufferu
			D3DX3TextNull->Unlock();
		}

// nastavení parametrù provádìcího bufferu
		D3DEXECUTEDATA ed;
		MemFill(&ed, sizeof(ed), 0);
		ed.dwSize = sizeof(D3DEXECUTEDATA);
		ed.dwVertexCount = 0;
		ed.dwInstructionOffset = 0;
		ed.dwInstructionLength = size;
		D3DX3TextNull->SetExecuteData(&ed);
	}
	else
	{
		D3DX3Stop();
		return false;
	}

// inicializace adres funkcí zaøízení
	pD3Stop = D3DX3Stop;			// ukonèení funkce zaøízení
	pD3SizeView = D3DX3SizeView;	// aktualizace viewportu po zmìnì velikosti
	pD3MoveView = D3DX3MoveView;	// aktualizace viewportu po pøesunu
	pD3Disp = D3DX3Disp;			// zobrazení renderovacího bufferu
	pD3Clear = D3DX3Clear;			// vymazání renderovací plochy
	pD3AktProj = D3DX3AktProj;		// aktualizace projekèní matice
	pD3AktView = D3DX3AktView;		// aktualizace pohledové matice
	pD3Begin = D3DX3Begin;			// zahájení renderování scény
	pD3End = D3DX3End;				// ukonèení renderování scény
	pD3Render = D3DX3Render;		// vyrenderování jednoho objektu
	pD3Reset = D3DX3Reset;			// resetování objektu pøed ukonèením ovladaèe
	pD3AktWireframe = D3DX3AktWireframe;
	pD3AktLightOn = D3DX3AktLightOn;
	pD3AktShades = D3DX3AktShades;
	pD3AktCulling = D3DX3AktCulling;
	pD3AktMagFilter = D3DX3AktMagFilter;
	pD3AktMinMipFilter = D3DX3AktMinMipFilter;
	pD3AktLight = D3DX3AktLight;
	pD3SetMaterial = D3DX3SetMaterial;
	pD3TextureRes = D3DX3TextureRes;
	pD3TextureSet = D3DX3TextureSet;
	pD3TextureOn = D3DX3TextureOn;
	pD3TextureOff = D3DX3TextureOff;
	pD3MatReset = D3DX3MatReset;
	pD3LightReset = D3DX3LightReset;
	pD3AktAmbient = D3DX3AktAmbient;
	pD3Free = D3DX3Free;
	pD3AktFog = D3DX3AktFog;
	pD3FogOn = D3DX3FogOn;
	pD3AktState = D3DX3AktState;
	pD3AktBlend = D3DX3AktBlend;
	pD3AktAlfa = D3DX3AktAlfa;
	pD3SetColorOp = D3DX3SetColorOp;
	pD3SetAddress = D3DX3SetAddress;
	pD3AktPalette = D3DX3AktPalette;
	pD3SetZFunc = D3DX3SetZFunc;
	pD3SetZWrite = D3DX3SetZWrite;

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// ukonèení funkce zaøízení

void D3DX3Stop()
{
	if (D3DX3TextNull != NULL)
	{
		D3DX3TextNull->Release();
		D3DX3TextNull = NULL;
	}

	if (D3DX3State != NULL)
	{
		D3DX3State->Release();
		D3DX3State = NULL;
	}
	
	D3DX3TermMatrix();		// zrušení transformaèních matic
	D3DX3TermView();		// zrušení viewportu
	D3DX3TermZBuf();		// zrušení Z bufferu
	D3DX3TermRend();		// zrušení renderovacího povrchu
	D3DX3TermPal();			// zrušení palet
	D3DX3TermClip();		// zrušení clipperu
	D3DX3TermPrim();		// zrušení primárního povrchu
	FPUDouble();			// návrat pøesnosti koprocesoru
}

/////////////////////////////////////////////////////////////////////////////
// vymazání renderovací plochy

void D3DX3Clear()
{
// vypnutí textury
	if ((D3DBackText < 0) && (D3DX3BackMatBuf.hTexture != NULL))
	{
		D3DX3BackMatBuf.hTexture = NULL;
		D3DBackColAkt = true;
	}

// aktualizace materiálu pozadí scény
	D3DX3AktBackMat();

// pozadí se neinicializuje
	if (D3DBackCol == (MAXDWORD-1)) return;

// vymazání barvy i Z bufferu
	if ((D3DBackCol != MAXDWORD) || (D3DBackText >= 0))
	{
		D3DX3View->Clear(1, &D3DX3Src, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER);
	}
	else

// jinak pouze vymazání Z bufferu
	{
		D3DX3View->Clear(1, &D3DX3Src, D3DCLEAR_ZBUFFER);
	}
}

/////////////////////////////////////////////////////////////////////////////
// zobrazení renderovacího bufferu

void D3DX3Disp()
{
	if ((D3DX3DD == NULL) || (D3DX3Prim == NULL)) return;
	if (D3DVSync) D3DX3DD->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, 0);
	if (D3DX3Prim->Blt(&D3DX3Dest, D3DX3Rend, (RECT*)&D3DX3Src, DDBLT_WAIT, NULL) == DDERR_SURFACELOST)
	{
		D3DX3Prim->Restore();
		D3DX3Rend->Restore();
		D3DX3ZBuf->Restore();
	}
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace projekèní matice

void D3DX3AktProj()
{
	D3DX3Dev->SetMatrix(D3DX3ProjMat, &D3DProjMatrix);
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace pohledové matice

void D3DX3AktView()
{
	D3DX3Dev->SetMatrix(D3DX3ViewMat, &D3DViewMatrix);
}


/////////////////////////////////////////////////////////////////////////////
// zahájení renderování scény

void D3DX3Begin()
{
	D3DX3Dev->BeginScene();
}


/////////////////////////////////////////////////////////////////////////////
// ukonèení renderování scény

void D3DX3End()
{
	D3DX3Dev->EndScene();
}

/////////////////////////////////////////////////////////////////////////////
// nastavení stavu

void D3DX3SetRenderState(D3DRENDERSTATETYPE statetype, DWORD value)
{
// pøíprava velikosti bufferu
	int size = 2*sizeof(D3DINSTRUCTION) + sizeof(D3DSTATE);

// pøíprava popisovaèe dat
	D3DEXECUTEBUFFERDESC d;
	d.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
	d.dwFlags = D3DDEB_BUFSIZE;
	d.dwBufferSize = size;

// vytvoøení provádìcího bufferu
	IDirect3DExecuteBuffer* ex = NULL;
	if (D3DX3Dev->CreateExecuteBuffer(&d, &ex, NULL) == D3D_OK)
	{

// naplnìní provádìcího bufferu
		if ((ex->Lock(&d) == D3D_OK) && (d.lpData != NULL))
		{
			D3DINSTRUCTION* ins = (D3DINSTRUCTION*)(d.lpData);
			ins->bOpcode = D3DOP_STATERENDER;
			ins->bSize = sizeof(D3DSTATE);
			ins->wCount = 1;
			ins++;
			D3DSTATE* state = (D3DSTATE*)ins;
			state->drstRenderStateType = statetype;
			state->dwArg[0] = value;
			state++;

// ukonèení provádìcího bufferu
			ins = (D3DINSTRUCTION*)state;
			ins->bOpcode = D3DOP_EXIT;
			ins->bSize = 0;
			ins->wCount = 0;

			ASSERT((BYTE*)(ins + 1) == ((BYTE*)(d.lpData) + size));

// odemknutí provádìcího bufferu
			ex->Unlock();

// nastavení parametrù provádìcího bufferu
			D3DEXECUTEDATA ed;
			MemFill(&ed, sizeof(ed), 0);
			ed.dwSize = sizeof(D3DEXECUTEDATA);
			ed.dwVertexCount = 0;
			ed.dwInstructionOffset = 0;
			ed.dwInstructionLength = size;
			ex->SetExecuteData(&ed);

// provedení bufferu
			D3DX3Dev->Execute(ex, D3DX3View, D3DEXECUTE_UNCLIPPED);
		}

// zrušení bufferu
		ex->Release();
	}
}

/////////////////////////////////////////////////////////////////////////////
// vyrenderování jednoho objektu

void D3DX3Render(D3DFITEM* item)
{
// pøi zmìnì stavu osvìtlení zrušení bufferu
	if ((BOOL)D3DLightOnAkt != (BOOL)(item->Data2)) D3DX3Reset(item);
	item->Data2 = (void*)D3DLightOnAkt;

	bool isFlare = (item->Type == D3DFTYPE_LENSFLARE);
	bool isPic = ((item->Type == D3DFTYPE_PICTURE) || isFlare);

// pøegenerování bufferu pro LensFlare
	if (isFlare) D3DX3Reset(item);

// vytvoøení renderovacího bufferu
	if (item->Data1 == NULL)
	{

// pøíprava velikosti bufferu
		int vertN = item->VertNum;
		int faceN = item->FaceNum;
		int size = vertN*sizeof(D3DVERTEX) +	// = sizeof(D3DTLVERTEX)
					3*sizeof(D3DINSTRUCTION) +
					1*sizeof(D3DPROCESSVERTICES) +
					faceN*sizeof(D3DTRIANGLE);					

// pøíprava popisovaèe dat
		D3DEXECUTEBUFFERDESC d;
		d.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
		d.dwFlags = D3DDEB_BUFSIZE;
		d.dwBufferSize = size;

// vytvoøení provádìcího bufferu
		IDirect3DExecuteBuffer* ex = NULL;
		if (D3DX3Dev->CreateExecuteBuffer(&d, &ex, NULL) == D3D_OK)
		{

// naplnìní provádìcího bufferu
			if ((ex->Lock(&d) == D3D_OK) && (d.lpData != NULL))
			{
				D3DVERTEX* vert = (D3DVERTEX*)(d.lpData);

				D3DVECTOR* v = item->Vertex;
				D3DVECTOR* n = item->Normal;
				float* uv = item->TextUV[0];
				D3DCOLOR col = XRGB(item->Diffuse);

				int i = vertN;

				double sinz1 = 0;
				double sinz2 = 0;

				double sx = 0;
				double sy = 0;

				if (isPic)
				{
					sinz1 = item->SinZ*D3DHeight/D3DWidth;
					sinz2 = item->SinZ*D3DWidth/D3DHeight;

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
					else
					{
						sx = item->TransX;
						sy = item->TransY;
					}
				}

// uložení dat vrcholù
				for (; i > 0; i--)
				{
					if (isPic)
					{
						double x = v->x*item->ScaleX;
						double y = v->y*item->ScaleY;
						vert->x = (float)((x*item->CosZ - y*sinz1 + sx + 0.5)*D3DWidth-0.5);
						vert->y = (float)((0.5 - (x*sinz2 + y*item->CosZ) - sy)*D3DHeight-0.5);
						vert->z = 0;
						v++;
						vert->nx = 1;	// = rhw
						*(D3DCOLOR*)(&vert->ny) = col;
						*(D3DCOLOR*)(&vert->nz) = 0;
					}
					else
					{
						vert->x = v->x;
						vert->y = v->y;
						vert->z = v->z;
						v++;

						if (D3DLightOnAkt)
						{
							vert->nx = n->x;
							vert->ny = n->y;
							vert->nz = n->z;
							n++;
						}
						else
						{
							*(DWORD*)(&vert->nx) = 0;
							*(D3DCOLOR*)(&vert->ny) = col;
							*(D3DCOLOR*)(&vert->nz) = 0;
						}
					}

					vert->tu = *uv;
					uv++;
					vert->tv = *uv;
					uv++;

					vert++;
				}

// pøíkaz pro zpracování vrcholù
				D3DINSTRUCTION* ins = (D3DINSTRUCTION*)vert;
				ins->bOpcode = D3DOP_PROCESSVERTICES;
				ins->bSize = sizeof(D3DPROCESSVERTICES);
				ins->wCount = 1;
				ins++;
				D3DPROCESSVERTICES* proc = (D3DPROCESSVERTICES*)ins;
				proc->dwFlags = (isPic ? D3DPROCESSVERTICES_COPY : (D3DLightOnAkt ? D3DPROCESSVERTICES_TRANSFORMLIGHT : D3DPROCESSVERTICES_TRANSFORM));
				proc->wStart = 0;
				proc->wDest = 0;
				proc->dwCount = vertN;
				proc->dwReserved = 0;
				proc++;

// pøíkaz pro vykreslení trojúhelníku
				ins = (D3DINSTRUCTION*)proc;
				ins->bOpcode = D3DOP_TRIANGLE;
				ins->bSize = sizeof(D3DTRIANGLE);
				ins->wCount = (WORD)faceN;
				ins++;
				D3DTRIANGLE* tri = (D3DTRIANGLE*)ins;

				i = faceN;
				int* face = item->Face;

				WORD f = 0;

				for (; i > 0; i--)
				{
					if (*face < vertN) f = (WORD)*face;
					tri->v1 = f;
					face++;

					if (*face < vertN) f = (WORD)*face;
					tri->v2 = f;
					face++;

					if (*face < vertN) f = (WORD)*face;
					tri->v3 = f;
					face++;

					tri->wFlags = D3DTRIFLAG_EDGEENABLETRIANGLE;
					tri++;
				}

				ins = (D3DINSTRUCTION*)tri;

// ukonèení provádìcího bufferu
				ins->bOpcode = D3DOP_EXIT;
				ins->bSize = 0;
				ins->wCount = 0;

				ASSERT((BYTE*)(ins + 1) == ((BYTE*)(d.lpData) + size));

// odemknutí provádìcího bufferu
				ex->Unlock();

// nastavení parametrù provádìcího bufferu
				D3DEXECUTEDATA ed;
				MemFill(&ed, sizeof(ed), 0);
				ed.dwSize = sizeof(D3DEXECUTEDATA);
				ed.dwVertexCount = vertN;
				ed.dwInstructionOffset = vertN * sizeof(D3DVERTEX);
				ed.dwInstructionLength = size - ed.dwInstructionOffset;
				ex->SetExecuteData(&ed);

// aktualizace klonù
				D3DFITEM* item2 = item;

				do
				{
					item2->Data1 = ex;

					item2 = D3DF_Get(item2->CloneNext);
				} while (item2 != item);
			}
			else
			{
				ex->Release();
			}
		}
	}

// nastavení transformaèní matice
	D3DX3Dev->SetMatrix(D3DX3WrldMat, &item->MatrixWorld);

// vykreslení objektu
	IDirect3DExecuteBuffer* e = (IDirect3DExecuteBuffer*)(item->Data1);
	if (e != NULL)
	{
		D3DX3Dev->Execute(e, D3DX3View, (item->AktClip ? D3DEXECUTE_CLIPPED : D3DEXECUTE_UNCLIPPED));
	}
}

/////////////////////////////////////////////////////////////////////////////
// resetování objektu pøed ukonèením ovladaèe

void D3DX3Reset(D3DFITEM* item)
{
	if (item->Data1 != NULL)
	{
		((IDirect3DExecuteBuffer*)(item->Data1))->Release();

		D3DFITEM* item2 = item;

		do {
			item2->Data1 = NULL;
			item2->Data2 = NULL;

			item2 = D3DF_Get(item2->CloneNext);
		} while (item != item2);
	}
}


/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè vyplòování ploch

void D3DX3AktWireframe()
{
	D3DX3SetRenderState(D3DRENDERSTATE_FILLMODE, (D3DWireframeAkt ? D3DFILL_SOLID : D3DFILL_WIREFRAME));
}

/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè osvìtlení

void D3DX3AktLightOn() { }


/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè plynulého stinování

void D3DX3AktShades()
{
	D3DX3SetRenderState(D3DRENDERSTATE_SHADEMODE, (D3DShadesAkt ? D3DSHADE_GOURAUD : D3DSHADE_FLAT));
}

/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè odstraòování ploch

void D3DX3AktCulling()
{
	D3DX3SetRenderState(D3DRENDERSTATE_CULLMODE, ((D3DCullingAkt == 1) ? D3DCULL_CCW : ((D3DCullingAkt == 2) ? D3DCULL_CW : D3DCULL_NONE)));
}

/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè filtrace zvìtšených textur

void D3DX3AktMagFilter(int stage)
{
	if (stage != 0) return;
	D3DX3SetRenderState(D3DRENDERSTATE_TEXTUREMAG, (D3DMagFilterAkt[0] ? D3DFILTER_LINEAR : D3DFILTER_NEAREST));
}

/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè filtrace zmenšených a vzdálených textur

void D3DX3AktMinMipFilter(int stage)
{
	if (stage != 0) return;
	D3DX3SetRenderState(D3DRENDERSTATE_TEXTUREMIN, (D3DMinFilterAkt[0] ? 
					((D3DMipFilterAkt[0]) ? D3DFILTER_LINEARMIPLINEAR : D3DFILTER_LINEAR) :
					((D3DMipFilterAkt[0]) ? D3DFILTER_MIPNEAREST : D3DFILTER_NEAREST)));
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace položky svìtla

void D3DX3AktLight(int inx, D3DLITEM* item)
{
	if (item->Akt)
	{
		if (item->Enable)
		{
			if (item->Data1 == NULL)
			{
				D3DX3D3->CreateLight((IDirect3DLight**)&(item->Data1), NULL);
			}

			if (item->Data1 != NULL)
			{
				D3DLIGHT l;
				l.dwSize = sizeof(D3DLIGHT);

				D3DLIGHT8* l2 = &(item->Light);

				l.dltType = l2->Type;

				l.dcvColor.r = l2->Diffuse.r;
				l.dcvColor.g = l2->Diffuse.g;
				l.dcvColor.b = l2->Diffuse.b;
				l.dcvColor.a = l2->Diffuse.a;

				MemCopy(&(l.dvPosition), &(l2->Position), 2*sizeof(D3DVECTOR) + 7*sizeof(float));

				((IDirect3DLight*)(item->Data1))->SetLight(&l);
			}

			if ((item->Data2 == NULL) && (item->Data1 != NULL))
			{
				D3DX3View->AddLight((IDirect3DLight*)(item->Data1));
				item->Data2 = (void*)TRUE;
			}
		}
		else
		{
			if (item->Data2 != NULL)
			{
				D3DX3View->DeleteLight((IDirect3DLight*)(item->Data1));
				item->Data2 = NULL;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// resetování položky svìtla

void D3DX3LightReset(int inx, D3DLITEM* item)
{
	if (item->Data1 != NULL)
	{
		IDirect3DLight* light = (IDirect3DLight*)(item->Data1);

		if (item->Data2 != NULL)
		{
			D3DX3View->DeleteLight(light);
		}
			
		light->Release();
		item->Data1 = NULL;
		item->Data2 = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení materiálu

void D3DX3SetMaterial(D3DMITEM* item)
{

// vytvoøení objektu materiálu
	if (item->Data1 == NULL)
	{

// vytvoøení materiálu
		IDirect3DMaterial* mat;
		if (D3DX3D3->CreateMaterial(&mat, NULL) == D3D_OK)
		{
			item->Data1 = mat;

// inicializace materiálu
			D3DMATERIAL dm;
			dm.dwSize = sizeof(D3DMATERIAL);
			MemCopy(&(dm.diffuse), &(item->Material.Diffuse), 4*sizeof(D3DCOLORVALUE) + sizeof(float));
			dm.hTexture = NULL;
			dm.dwRampSize = 20;
			mat->SetMaterial(&dm);

// pøíprava velikosti bufferu
			int size = 	2*sizeof(D3DINSTRUCTION) + sizeof(D3DSTATE);

// pøíprava popisovaèe dat
			D3DEXECUTEBUFFERDESC d;
			d.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
			d.dwFlags = D3DDEB_BUFSIZE;
			d.dwBufferSize = size;

// vytvoøení provádìcího bufferu
			IDirect3DExecuteBuffer* ex;
			if (D3DX3Dev->CreateExecuteBuffer(&d, &ex, NULL) == D3D_OK)
			{
				item->Data2 = ex;

// naplnìní provádìcího bufferu
				if ((ex->Lock(&d) == D3D_OK) && (d.lpData != NULL))
				{
					D3DINSTRUCTION* ins = (D3DINSTRUCTION*)(d.lpData);
					ins->bOpcode = D3DOP_STATELIGHT;
					ins->bSize = sizeof(D3DSTATE);
					ins->wCount = 1;
					ins++;
					D3DSTATE* state = (D3DSTATE*)ins;
					state->dlstLightStateType = D3DLIGHTSTATE_MATERIAL;
					mat->GetHandle(D3DX3Dev, (D3DMATERIALHANDLE*) &(state->dwArg[0]));
					state++;

// ukonèení provádìcího bufferu
					ins = (D3DINSTRUCTION*)state;
					ins->bOpcode = D3DOP_EXIT;
					ins->bSize = 0;
					ins->wCount = 0;

					ASSERT((BYTE*)(ins + 1) == ((BYTE*)(d.lpData) + size));

// odemknutí provádìcího bufferu
					ex->Unlock();
				}

// nastavení parametrù provádìcího bufferu
				D3DEXECUTEDATA ed;
				MemFill(&ed, sizeof(ed), 0);
				ed.dwSize = sizeof(D3DEXECUTEDATA);
				ed.dwVertexCount = 0;
				ed.dwInstructionOffset = 0;
				ed.dwInstructionLength = size;
				ex->SetExecuteData(&ed);
			}
		}
	}

// nastavení materiálu
	IDirect3DExecuteBuffer* e = (IDirect3DExecuteBuffer*)(item->Data2);
	if (e != NULL)
	{
		D3DX3Dev->Execute(e, D3DX3View, D3DEXECUTE_UNCLIPPED);
	}

// ruší aktivní texturu
	D3DT_Active[0] = -1;
}

/////////////////////////////////////////////////////////////////////////////
// resetování položky materiálu

void D3DX3MatReset(D3DMITEM* item)
{	  
	if (item->Data1 != NULL)
	{
		((IDirect3DMaterial*)(item->Data1))->Release();
		item->Data1 = NULL;
	}

	if (item->Data2 != NULL)
	{
		((IDirect3DExecuteBuffer*)(item->Data2))->Release();
		item->Data2 = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// resetování položky textury pøed ukonèením ovladaèe

void D3DX3TextureRes(D3DTITEM* item)
{
	if (item->Data1 != NULL)
	{
		if (D3DRamp)
		{
			((IDirect3DMaterial*)(item->Data1))->Release();
		}
		else
		{
			((IDirect3DTexture*)(item->Data1))->Release();
		}
		item->Data1 = NULL;
	}

	if (item->Data2 != NULL)
	{
		((IDirect3DExecuteBuffer*)(item->Data2))->Release();
		item->Data2 = NULL;
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastavení a zapnutí textury

int tx3Width;
int tx3Height;
int tx3Mip;

BYTE* tx3Data;
int tx3WidthMul;

IDirectDrawSurface* tx3Tex;
DDPIXELFORMAT* tx3PF;

BOOL _fastcall D3DX3TextCreate(DDPIXELFORMAT* pf, BYTE* data, int mul)
{
	if (pf->dwRGBBitCount == 0) return FALSE;

	tx3Data = data;
	tx3WidthMul = mul;

	tx3Tex = NULL;
	tx3PF = pf;

	DDSURFACEDESC ddsd;
	MemFill(&ddsd, sizeof(ddsd), 0);
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwWidth = tx3Width;
	ddsd.dwHeight = tx3Height;
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_SYSTEMMEMORY;
	MemCopy(&(ddsd.ddpfPixelFormat), pf, sizeof(DDPIXELFORMAT));

	if (tx3Mip > 1)
	{
		ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_MIPMAPCOUNT | DDSD_PIXELFORMAT;
		ddsd.dwMipMapCount = tx3Mip;
		ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP | DDSCAPS_COMPLEX | DDSCAPS_SYSTEMMEMORY;
	}

	return ((D3DX3DD->CreateSurface(&ddsd, &tx3Tex, NULL) == DD_OK) &&
			(tx3Tex != NULL));
}


void D3DX3TextureSet(D3DTITEM* item, int stage)
{
	if (stage != 0) return;

// data obrázku
	PICTUREDATA* picdata = item->Pic.Data();

// vytvoøení textury
	if (item->Data1 == NULL)
	{
// pro Ramp se nastaví barva materiálu (z poslední MipMap)
		if (D3DRamp)
		{
			int width = picdata->TextWidth;
			int height = picdata->TextHeight;
		
			int size = 0;
			int mips = picdata->TextMipMaps;
			if (mips > 1) mips--;

			for (; mips > 0; mips--)
			{
				size += (width*height*4);
				width = (width >> 1);
				if (width == 0) width++;
				height = (height >> 1);
				if (height == 0) height++;
			}

			BYTE* src = picdata->TextData + size - 4;

			IDirect3DMaterial* mat = NULL;
			D3DX3D3->CreateMaterial(&mat, NULL);
			if (mat != NULL)
			{
				item->Data1 = mat;
				
				D3DMATERIAL m;
				MemFill(&m, sizeof(D3DMATERIAL), 0);
				m.dwSize = sizeof(D3DMATERIAL);
				m.diffuse.b = (float)*src/255;
				src++;
				m.diffuse.g = (float)*src/255;
				src++;
				m.diffuse.r = (float)*src/255;
				src++;
				m.diffuse.a = 1;
				src++;

				m.ambient = m.diffuse;

				m.specular.a = 1;
				m.emissive.a = 1;
				m.power = 0;

				m.hTexture = NULL;
				m.dwRampSize = 20;

				mat->SetMaterial(&m);

// pøíprava velikosti bufferu
				int size = 	2*sizeof(D3DINSTRUCTION) + sizeof(D3DSTATE);

// pøíprava popisovaèe dat
				D3DEXECUTEBUFFERDESC d;
				d.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
				d.dwFlags = D3DDEB_BUFSIZE;
				d.dwBufferSize = size;

// vytvoøení provádìcího bufferu
				IDirect3DExecuteBuffer* ex;
				if (D3DX3Dev->CreateExecuteBuffer(&d, &ex, NULL) == D3D_OK)
				{
					item->Data2 = ex;

// naplnìní provádìcího bufferu
					if ((ex->Lock(&d) == D3D_OK) && (d.lpData != NULL))
					{
						D3DINSTRUCTION* ins = (D3DINSTRUCTION*)(d.lpData);
						ins->bOpcode = D3DOP_STATELIGHT;
						ins->bSize = sizeof(D3DSTATE);
						ins->wCount = 1;
						ins++;
						D3DSTATE* state = (D3DSTATE*)ins;
						state->dlstLightStateType = D3DLIGHTSTATE_MATERIAL;
						mat->GetHandle(D3DX3Dev, (D3DMATERIALHANDLE*) &(state->dwArg[0]));
						state++;

// ukonèení provádìcího bufferu
						ins = (D3DINSTRUCTION*)state;
						ins->bOpcode = D3DOP_EXIT;
						ins->bSize = 0;
						ins->wCount = 0;

						ASSERT((BYTE*)(ins + 1) == ((BYTE*)(d.lpData) + size));

// odemknutí provádìcího bufferu
						ex->Unlock();
					}

// nastavení parametrù provádìcího bufferu
					D3DEXECUTEDATA ed;
					MemFill(&ed, sizeof(ed), 0);
					ed.dwSize = sizeof(D3DEXECUTEDATA);
					ed.dwVertexCount = 0;
					ed.dwInstructionOffset = 0;
					ed.dwInstructionLength = size;
					ex->SetExecuteData(&ed);
				}
			}

		}
		else
		{

			tx3Width = picdata->TextWidth;
			tx3Height = picdata->TextHeight;

// poèet mipmap
			tx3Mip = 1;
			if (IsMipMap && item->Mips)
			{
				int n = tx3Width;
				if (n > tx3Height) n = tx3Height;

				while (n > 1)
				{
					n = (n >> 1);
					tx3Mip++;
				}
			}
			if (tx3Mip > picdata->TextMipMaps) tx3Mip = picdata->TextMipMaps;

			// omezení poètu úrovní - pøi vyšším stupni se prolínají ikony textury terénu
			int mipmaps = item->MipMaps;
			if (mipmaps > 0)
			{
				if (tx3Mip > mipmaps) tx3Mip = mipmaps;
			}

			int mip0 = tx3Mip;

// vytvoøení povrchu
			if ((!picdata->TextTrans && (D3DX3TextCreate(&D3DTextFormatR5G6B5, picdata->TextDataR5G6B5, 2) ||
									D3DX3TextCreate(&D3DTextFormatA1R5G5B5, picdata->TextDataA1R5G5B5, 2))) ||
				D3DX3TextCreate(&D3DTextFormatA4R4G4B4, picdata->TextDataA4R4G4B4, 2) ||
				((D3DLowText || (picdata->TextSmooth < 1.25)) && D3DX3TextCreate(&D3DTextFormatA1R5G5B5, picdata->TextDataA1R5G5B5, 2)) ||
				D3DX3TextCreate(&D3DTextFormatA8R8G8B8, picdata->TextData, 4) ||
				D3DX3TextCreate(&D3DTextFormatA1R5G5B5, picdata->TextDataA1R5G5B5, 2) ||
				D3DX3TextCreate(&D3DTextFormatR5G6B5, picdata->TextDataR5G6B5, 2) ||
				D3DX3TextCreate(&D3DTextFormatX1R5G5B5, picdata->TextDataA1R5G5B5, 2) ||
				D3DX3TextCreate(&D3DTextFormatR8G8B8, picdata->TextDataR8G8B8, 3) ||
				D3DX3TextCreate(&D3DTextFormatX8R8G8B8, picdata->TextData, 4) ||
				D3DX3TextCreate(&D3DTextFormatX4R4G4B4, picdata->TextDataA4R4G4B4, 2))
			{
				IDirectDrawSurface* ds0 = tx3Tex;

				tx3Tex->AddRef();
				BYTE* src = tx3Data;
				
				DDSCAPS caps;
				MemFill(&caps, sizeof(DDSCAPS), 0);
				caps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;

				for (; tx3Mip > 0; tx3Mip--)
				{
					DDSURFACEDESC ddsd;
					MemFill(&ddsd, sizeof(ddsd), 0);
					ddsd.dwSize = sizeof(ddsd);

					if (tx3Tex->Lock(NULL, &ddsd, 0, NULL) == DD_OK)
					{
						int pitch = ddsd.lPitch;
						BYTE* dst = (BYTE*)ddsd.lpSurface;
						int widthbyte = tx3Width * tx3WidthMul;

						if (pitch >= widthbyte)
						{
							for (int i = tx3Height; i > 0; i--)
							{
								MemCopy(dst, src, widthbyte);
								dst += pitch;
								src += widthbyte;
							}
						}

						tx3Tex->Unlock(NULL);
					}

					tx3Width = (tx3Width >> 1);
					tx3Height = (tx3Height >> 1);

					IDirectDrawSurface* dsnext = NULL;

					if (tx3Mip > 1) tx3Tex->GetAttachedSurface(&caps, &dsnext);
					tx3Tex->Release();
					tx3Tex = dsnext;
					if (tx3Tex == NULL) break;
				}

// pro HW rasterizer pøenesení do videopamìti
				if (IsHWRaster)
				{	 
					DDSURFACEDESC ddsd;
					MemFill(&ddsd, sizeof(ddsd), 0);
					ddsd.dwSize = sizeof(ddsd);
					ddsd.dwWidth = picdata->TextWidth;
					ddsd.dwHeight = picdata->TextHeight;
					ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
					ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_VIDEOMEMORY | DDSCAPS_ALLOCONLOAD;
					MemCopy(&(ddsd.ddpfPixelFormat), tx3PF, sizeof(DDPIXELFORMAT));

					if (mip0 > 1)
					{
						ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_MIPMAPCOUNT | DDSD_PIXELFORMAT;
						ddsd.dwMipMapCount = mip0;
						ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP | DDSCAPS_COMPLEX | DDSCAPS_VIDEOMEMORY | DDSCAPS_ALLOCONLOAD;
					}

					if (D3DX3DD->CreateSurface(&ddsd, &tx3Tex, NULL) == DD_OK)
					{
						IDirect3DTexture* tx = NULL;
						ds0->QueryInterface(IID_IDirect3DTexture, (void**)&tx);

						if (tx != NULL)
						{
							IDirect3DTexture* tx2 = NULL;

							tx3Tex->QueryInterface(IID_IDirect3DTexture, (void**)&tx2);

							if (tx2 != NULL)
							{
								tx2->Load(tx);
								tx2->Release();
							}

							tx->Release();
						}

						ds0->Release();
						ds0 = tx3Tex;
					}
				}

				ds0->QueryInterface(IID_IDirect3DTexture, &item->Data1);
				ds0->Release();
			}
		}
	}

// pøíkaz pro nastavení textury
	if ((item->Data2 == NULL) && (item->Data1 != NULL) && !D3DRamp)
	{
		D3DTEXTUREHANDLE h;
		((IDirect3DTexture*)(item->Data1))->GetHandle(D3DX3Dev, &h);

// pøíprava velikosti bufferu
		int size = 	2*sizeof(D3DINSTRUCTION) + sizeof(D3DSTATE);

// pøíprava popisovaèe dat
		D3DEXECUTEBUFFERDESC d;
		d.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
		d.dwFlags = D3DDEB_BUFSIZE;
		d.dwBufferSize = size;

// vytvoøení provádìcího bufferu
		IDirect3DExecuteBuffer* ex;
		if (D3DX3Dev->CreateExecuteBuffer(&d, &ex, NULL) == D3D_OK)
		{
			item->Data2 = ex;

// naplnìní provádìcího bufferu
			if ((ex->Lock(&d) == D3D_OK) && (d.lpData != NULL))
			{
				D3DINSTRUCTION* ins = (D3DINSTRUCTION*)(d.lpData);
				ins->bOpcode = D3DOP_STATERENDER;
				ins->bSize = sizeof(D3DSTATE);
				ins->wCount = 1;
				ins++;
				D3DSTATE* state = (D3DSTATE*)ins;
				state->drstRenderStateType = D3DRENDERSTATE_TEXTUREHANDLE;
				state->dwArg[0] = h;
				state++;

// ukonèení provádìcího bufferu
				ins = (D3DINSTRUCTION*)state;
				ins->bOpcode = D3DOP_EXIT;
				ins->bSize = 0;
				ins->wCount = 0;

				ASSERT((BYTE*)(ins + 1) == ((BYTE*)(d.lpData) + size));

// odemknutí provádìcího bufferu
				ex->Unlock();
			}

// nastavení parametrù provádìcího bufferu
			D3DEXECUTEDATA ed;
			MemFill(&ed, sizeof(ed), 0);
			ed.dwSize = sizeof(D3DEXECUTEDATA);
			ed.dwVertexCount = 0;
			ed.dwInstructionOffset = 0;
			ed.dwInstructionLength = size;
			ex->SetExecuteData(&ed);
		}
	}

// nastavení textury
	IDirect3DExecuteBuffer* e = (IDirect3DExecuteBuffer*)(item->Data2);
	if (e != NULL)
	{
		D3DX3Dev->Execute(e, D3DX3View, D3DEXECUTE_UNCLIPPED);
		if (D3DRamp)
		{
			D3DM_Akt = -1;
		}
		else
		{
			if ((D3DBackText >= 0) && (item == D3DT_Get(D3DBackText)))
			{
				D3DTEXTUREHANDLE h;
				((IDirect3DTexture*)(item->Data1))->GetHandle(D3DX3Dev, &h);

				if (h != D3DX3BackMatBuf.hTexture)
				{
					D3DX3BackMatBuf.hTexture = h;
					D3DBackColAkt = true;
				}
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// vypnutí textury

void D3DX3TextureOff(int stage)
{
	if (stage != 0) return;
	D3DX3Dev->Execute(D3DX3TextNull, D3DX3View, D3DEXECUTE_UNCLIPPED);
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace ambient osvìtlení

void D3DX3AktAmbient()
{

// pøíprava velikosti bufferu
	int size = 	2*sizeof(D3DINSTRUCTION) + sizeof(D3DSTATE);

// pøíprava popisovaèe dat
	D3DEXECUTEBUFFERDESC d;
	d.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
	d.dwFlags = D3DDEB_BUFSIZE;
	d.dwBufferSize = size;

// vytvoøení provádìcího bufferu
	IDirect3DExecuteBuffer* ex;
	if (D3DX3Dev->CreateExecuteBuffer(&d, &ex, NULL) == D3D_OK)
	{

// naplnìní provádìcího bufferu
		if ((ex->Lock(&d) == D3D_OK) && (d.lpData != NULL))
		{
			D3DINSTRUCTION* ins = (D3DINSTRUCTION*)(d.lpData);
			ins->bOpcode = D3DOP_STATELIGHT;
			ins->bSize = sizeof(D3DSTATE);
			ins->wCount = 1;
			ins++;
			D3DSTATE* state = (D3DSTATE*)ins;
			state->dlstLightStateType = D3DLIGHTSTATE_AMBIENT;
			state->dwArg[0] = D3DAmbientColRGB;
			state++;

// ukonèení provádìcího bufferu
			ins = (D3DINSTRUCTION*)state;
			ins->bOpcode = D3DOP_EXIT;
			ins->bSize = 0;
			ins->wCount = 0;

			ASSERT((BYTE*)(ins + 1) == ((BYTE*)(d.lpData) + size));

// odemknutí provádìcího bufferu
			ex->Unlock();
		}

// nastavení parametrù provádìcího bufferu
		D3DEXECUTEDATA ed;
		MemFill(&ed, sizeof(ed), 0);
		ed.dwSize = sizeof(D3DEXECUTEDATA);
		ed.dwVertexCount = 0;
		ed.dwInstructionOffset = 0;
		ed.dwInstructionLength = size;
		ex->SetExecuteData(&ed);

// provedení bufferu
		D3DX3Dev->Execute(ex, D3DX3View, D3DEXECUTE_UNCLIPPED);
	}
}

/////////////////////////////////////////////////////////////////////////////
// volná videopamì

int D3DX3Free()
{
	DDCAPS cap;
	MemFill(&cap, sizeof(DDCAPS), 0);
	cap.dwSize = sizeof(DDCAPS);

	D3DX3DD->GetCaps(&cap, NULL);

	return cap.dwVidMemFree;
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace mlhy

void D3DX3AktFog()
{

// pøíprava velikosti bufferu
	int size = 	2*sizeof(D3DINSTRUCTION) + 5*sizeof(D3DSTATE);

// pøíprava popisovaèe dat
	D3DEXECUTEBUFFERDESC d;
	d.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
	d.dwFlags = D3DDEB_BUFSIZE;
	d.dwBufferSize = size;

// vytvoøení provádìcího bufferu
	IDirect3DExecuteBuffer* ex;
	if (D3DX3Dev->CreateExecuteBuffer(&d, &ex, NULL) == D3D_OK)
	{

// naplnìní provádìcího bufferu
		if ((ex->Lock(&d) == D3D_OK) && (d.lpData != NULL))
		{

// nastavení pøepínaèù
			D3DINSTRUCTION* ins = (D3DINSTRUCTION*)(d.lpData);
			ins->bOpcode = D3DOP_STATERENDER;
			ins->bSize = sizeof(D3DSTATE);
			ins->wCount = 5;
			ins++;
			D3DSTATE* state = (D3DSTATE*)ins;
			state->drstRenderStateType = D3DRENDERSTATE_FOGCOLOR;
			state->dwArg[0] = FogColorRGB;
			state++;
			state->drstRenderStateType = D3DRENDERSTATE_FOGTABLEMODE;
			state->dwArg[0] = ((FogType == 0) ? 3 : FogType);
			state++;
			state->drstRenderStateType = D3DRENDERSTATE_FOGTABLESTART;
			float x = (float)FogStart;
			state->dwArg[0] = *((DWORD*)&x);
			state++;
			state->drstRenderStateType = D3DRENDERSTATE_FOGTABLEEND;
			x = (float)FogEnd;
			state->dwArg[0] = *((DWORD*)&x);
			state++;
			state->drstRenderStateType = D3DRENDERSTATE_FOGTABLEDENSITY;
			x = (float)FogDens;
			state->dwArg[0] = *((DWORD*)&x);
			state++;

// ukonèení provádìcího bufferu
			ins = (D3DINSTRUCTION*)state;
			ins->bOpcode = D3DOP_EXIT;
			ins->bSize = 0;
			ins->wCount = 0;

			ASSERT((BYTE*)(ins + 1) == ((BYTE*)(d.lpData) + size));

// odemknutí provádìcího bufferu
			ex->Unlock();

// nastavení parametrù provádìcího bufferu
			D3DEXECUTEDATA ed;
			MemFill(&ed, sizeof(ed), 0);
			ed.dwSize = sizeof(D3DEXECUTEDATA);
			ed.dwInstructionLength = size;
			ex->SetExecuteData(&ed);

			D3DX3Dev->Execute(ex, D3DX3View, D3DEXECUTE_UNCLIPPED);
		}
		ex->Release();
	}
}

/////////////////////////////////////////////////////////////////////////////
// zapnutí mlhy

void D3DX3FogOn(BOOL on)
{
	if (!D3DVertFog)
	{
		D3DX3SetRenderState(D3DRENDERSTATE_FOGENABLE, on);
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace stavu

void D3DX3AktState()
{
	D3DX3Dev->Execute(D3DX3State, D3DX3View, D3DEXECUTE_UNCLIPPED);
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace blending operací objektu

void D3DX3AktBlend(D3DFITEM* item)
{
// pøíprava velikosti bufferu
	int size = 	2*sizeof(D3DINSTRUCTION) + 3*sizeof(D3DSTATE);

// pøíprava popisovaèe dat
	D3DEXECUTEBUFFERDESC d;
	d.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
	d.dwFlags = D3DDEB_BUFSIZE;
	d.dwBufferSize = size;

// vytvoøení provádìcího bufferu
	IDirect3DExecuteBuffer* ex;
	if (D3DX3Dev->CreateExecuteBuffer(&d, &ex, NULL) == D3D_OK)
	{

// naplnìní provádìcího bufferu
		if ((ex->Lock(&d) == D3D_OK) && (d.lpData != NULL))
		{

// nastavení pøepínaèù
			D3DINSTRUCTION* ins = (D3DINSTRUCTION*)(d.lpData);
			ins->bOpcode = D3DOP_STATERENDER;
			ins->bSize = sizeof(D3DSTATE);
			ins->wCount = 3;
			ins++;
			D3DSTATE* state = (D3DSTATE*)ins;
			state->drstRenderStateType = D3DRENDERSTATE_SRCBLEND;
			state->dwArg[0] = BlendTabD3D[item->SrcBlend];
			state++;
			state->drstRenderStateType = D3DRENDERSTATE_DESTBLEND;
			state->dwArg[0] = BlendTabD3D[item->DstBlend];
			state++;
#define D3DRENDERSTATE_BLENDENABLE (D3DRENDERSTATETYPE)27 // chybí v D3DTYPES.H
			state->drstRenderStateType = D3DRENDERSTATE_BLENDENABLE;
			state->dwArg[0] = (BOOL)(item->IsBlend && !D3DRamp && IsBlending);
			state++;

// ukonèení provádìcího bufferu
			ins = (D3DINSTRUCTION*)state;
			ins->bOpcode = D3DOP_EXIT;
			ins->bSize = 0;
			ins->wCount = 0;

			ASSERT((BYTE*)(ins + 1) == ((BYTE*)(d.lpData) + size));

// odemknutí provádìcího bufferu
			ex->Unlock();

// nastavení parametrù provádìcího bufferu
			D3DEXECUTEDATA ed;
			MemFill(&ed, sizeof(ed), 0);
			ed.dwSize = sizeof(D3DEXECUTEDATA);
			ed.dwInstructionLength = size;
			ex->SetExecuteData(&ed);

			D3DX3Dev->Execute(ex, D3DX3View, D3DEXECUTE_UNCLIPPED);
		}
		ex->Release();
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace referenèní úrovnì alfa

void D3DX3AktAlfa(D3DFITEM* item)
{
// pøíprava velikosti bufferu
	int size = 	2*sizeof(D3DINSTRUCTION) + 3*sizeof(D3DSTATE);

// pøíprava popisovaèe dat
	D3DEXECUTEBUFFERDESC d;
	d.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
	d.dwFlags = D3DDEB_BUFSIZE;
	d.dwBufferSize = size;

// vytvoøení provádìcího bufferu
	IDirect3DExecuteBuffer* ex;
	if (D3DX3Dev->CreateExecuteBuffer(&d, &ex, NULL) == D3D_OK)
	{

// naplnìní provádìcího bufferu
		if ((ex->Lock(&d) == D3D_OK) && (d.lpData != NULL))
		{

// nastavení pøepínaèù
			D3DINSTRUCTION* ins = (D3DINSTRUCTION*)(d.lpData);
			ins->bOpcode = D3DOP_STATERENDER;
			ins->bSize = sizeof(D3DSTATE);
			ins->wCount = 3;
			ins++;
			D3DSTATE* state = (D3DSTATE*)ins;
			state->drstRenderStateType = D3DRENDERSTATE_ALPHAFUNC;
			state->dwArg[0] = (AlphaGreaterEqual ? D3DCMP_GREATEREQUAL : D3DCMP_GREATER);
			state++;
			state->drstRenderStateType = D3DRENDERSTATE_ALPHAREF;
			state->dwArg[0] = item->AlphaRef2;
			state++;
			state->drstRenderStateType = D3DRENDERSTATE_ALPHATESTENABLE;
			state->dwArg[0] = (BOOL)(item->IsAlphaRef && !D3DRamp);
			state++;

// ukonèení provádìcího bufferu
			ins = (D3DINSTRUCTION*)state;
			ins->bOpcode = D3DOP_EXIT;
			ins->bSize = 0;
			ins->wCount = 0;

			ASSERT((BYTE*)(ins + 1) == ((BYTE*)(d.lpData) + size));

// odemknutí provádìcího bufferu
			ex->Unlock();

// nastavení parametrù provádìcího bufferu
			D3DEXECUTEDATA ed;
			MemFill(&ed, sizeof(ed), 0);
			ed.dwSize = sizeof(D3DEXECUTEDATA);
			ed.dwInstructionLength = size;
			ex->SetExecuteData(&ed);

			D3DX3Dev->Execute(ex, D3DX3View, D3DEXECUTE_UNCLIPPED);
		}
		ex->Release();
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení operace barev

void D3DX3SetColorOp(int op, int arg1, int arg2, int stage)
{
	if (stage != 0) return;

	int n = D3DTBLEND_MODULATE;

	switch (op)
	{
	case 1:				// D3DTOP_DISABLE:
		n = -1;
		break;

	case 2:				// D3DTOP_SELECTARG1:
		n = -1;
		if ((arg1 & 0xf) == 2)	// D3DTA_TEXTURE
		{
			n = D3DTBLEND_DECAL;
		}
		break;

	case 3:				// D3DTOP_SELECTARG2:
		n = -1;
		if ((arg2 & 0xf) == 2)	// D3DTA_TEXTURE
		{
			n = D3DTBLEND_DECAL;
		}
		break;

	case 4:				// D3DTOP_MODULATE:
		break;

	case 5:				// D3DTOP_MODULATE2X:
		n = D3DTBLEND_MODULATEALPHA;
		break;

	case 6:				// D3DTOP_MODULATE4X:
		n = D3DTBLEND_DECALMASK;
		break;
		
	case 7:				// D3DTOP_ADD
	case 8:				// D3DTOP_ADDSIGNED
		n = D3DTBLEND_DECALALPHA;
		break;

	case 9:				// D3DTOP_ADDSIGNED2X
		n = D3DTBLEND_MODULATEMASK;
		break;
	}

	if (n == -1)
	{
		D3DT_Akt(-1, 0);
	}
	else
	{
		D3DX3SetRenderState(D3DRENDERSTATE_TEXTUREMAPBLEND, n);
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení adresování textur

void D3DX3SetAddress(int adru, int adrv, int stage)
{
	if (stage != 0) return;
	D3DX3SetRenderState(D3DRENDERSTATE_TEXTUREADDRESS, adru);
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace palet

void D3DX3AktPalette(HWND wnd)
{
	if (D3DX3Pal != NULL)
	{
		D3DX3Prim->SetPalette(D3DX3Pal);
		D3DX3Rend->SetPalette(D3DX3Pal);
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení Z funkce (1 až 8, v kódu DirectX, 0=vypnuto)

void D3DX3SetZFunc(int func)
{
	if (func == 0)
	{
		D3DX3SetRenderState(D3DRENDERSTATE_ZFUNC, D3DCMP_ALWAYS);
		D3DX3SetRenderState(D3DRENDERSTATE_ZENABLE, FALSE);
	}
	else
	{
		D3DX3SetRenderState(D3DRENDERSTATE_ZENABLE, TRUE);
		D3DX3SetRenderState(D3DRENDERSTATE_ZFUNC, func);
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení Z zápisu

void D3DX3SetZWrite(BOOL write)
{
	D3DX3SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, write);
}

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------
