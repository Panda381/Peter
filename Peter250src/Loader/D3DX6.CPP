
// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

#define	DIRECT3D_VERSION 0x0600

#include "Main.h"

/***************************************************************************\
*																			*
*							3D rozhraní - DirectX 6							*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// deklarace

// DirectDraw 5
extern	IDirectDraw2*		D3DX5DD;		// objekt DirectDraw 5 (NULL=není)


/////////////////////////////////////////////////////////////////////////////
// data

// DirectDraw 6
bool				D3DX6DDTest			= false;	// probìhl test rozhraní DirectDraw 6
IDirectDraw4*		D3DX6DD				= NULL;		// objekt DirectDraw 6 (NULL=není)

// Direct3D	6
bool				D3DX6D3Test			= false;	// probìhl test rozhraní Direct3D 6
IDirect3D3*			D3DX6D3				= NULL;		// objekt Direct3D 6 (NULL=není)

// objekty
IDirectDrawSurface4*	D3DX6Prim		= NULL;		// primární povrch
IDirectDrawClipper*		D3DX6Clip		= NULL;		// clipper
IDirectDrawPalette*		D3DX6Pal		= NULL;		// palety
IDirectDrawSurface4*	D3DX6Rend		= NULL;		// renderovací povrch
int						D3DX6RendWidth	= 0;		// aktuální šíøka renderovacího povrchu	
int						D3DX6RendHeight	= 0;		// aktuální výška renderovacího povrchu	
IDirectDrawSurface4*	D3DX6ZBuf		= NULL;		// Z buffer
IDirect3DDevice3*		D3DX6Dev		= NULL;		// renderovací zaøízení
IDirect3DViewport3*		D3DX6View		= NULL;		// viewport

//DDSURFACEDESC2			D3DX6ZBufDesc;				// popisovaè Z bufferu

// cílové souøadnice pro vykreslování v souøadnicích displeje
RECT					D3DX6Dest;					// cílová oblast pro vykreslování
D3DRECT					D3DX6Src;					// zdrojová oblast viewportu
D3DVIEWPORT2			D3DX6ViewBuf;				// buffer nastavení pro viewport

// pozadí scény
IDirect3DMaterial3*		D3DX6BackMat	= NULL;		// materiál pozadí scény
D3DMATERIAL				D3DX6BackMatBuf;			// buffer materiálu pozadí scény
IDirect3DVertexBuffer*	D3DX6BackBuf = NULL;		// buffer pozadí scény

IDirect3DVertexBuffer*	D3DX6ShadowBuf = NULL;		// buffer stínu

// formát textur
//DDPIXELFORMAT			D3DX6TextFormat;			// použitý formát textur

/////////////////////////////////////////////////////////////////////////////
// inicializace rozhraní DirectDraw 6 (vrací TRUE=rozhraní je vytvoøeno)

bool D3DX6InitDD()
{
// test, zda již byla provádìna inicializace
	if (!D3DX6DDTest)
	{
		D3DX6DDTest = true;

// vytvoøení rozhraní DirectDraw 5
		if (D3DX5InitDD())
		{

// vytvoøení objektu DirectDraw 6
			if (D3DX5DD->QueryInterface(IID_IDirectDraw4, (void**)&D3DX6DD) != DD_OK)
			{
				D3DX6DD = NULL;
			}
		}
	}

// test, zda je objekt DirectDraw 6 vytvoøen
	return (D3DX6DD != NULL);
}

/////////////////////////////////////////////////////////////////////////////
// ukonèení rozhraní DirectDraw 6

void D3DX6TermDD()
{
// ukonèení rozhraní Direct3D 6
	D3DX6TermD3();

// zrušení viewportu
	D3DX6TermView();

// zrušení Z bufferu
	D3DX6TermZBuf();

// zrušení renderovacího povrchu
	D3DX6TermRend();

// zrušení palet
	D3DX6TermPal();

// zrušení clipperu
	D3DX6TermClip();

// zrušení primárního povrchu
	D3DX6TermPrim();

// zrušení objektu DirectDraw 6
	if (D3DX6DD != NULL)
	{
		D3DX6DD->Release();
		D3DX6DD = NULL;
	}

// zrušení pøíznaku, že byla inicializace
	D3DX6DDTest = false;
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení povrchu DirectDrawSurface4 (vrací TRUE=OK)

bool D3DX6InitSurf(IDirectDrawSurface4** surf, DDSURFACEDESC2* ddsd)
{
	ddsd->dwSize = sizeof(DDSURFACEDESC2);

	if (!D3DX6InitDD() ||
		(D3DX6DD->CreateSurface(ddsd, surf, NULL) != DD_OK) ||
		(*surf == NULL))
	{
		*surf = NULL;
		return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení primárniho povrchu, není-li vytvoøen (vrací TRUE=OK)

bool D3DX6InitPrim()
{
	if (D3DX6Prim == NULL)
	{
		DDSURFACEDESC2 ddsd;
		MemFill(&ddsd, sizeof(ddsd), 0);
		ddsd.dwFlags = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

		if (!D3DX6InitSurf(&D3DX6Prim, &ddsd)) return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení primárního povrchu

void D3DX6TermPrim()
{
	if (D3DX6Prim != NULL)
	{
		D3DX6Prim->Release();
		D3DX6Prim = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení clipperu, není-li vytvoøen (vrací TRUE=OK)

bool D3DX6InitClip()
{
	if (D3DX6Clip == NULL)
	{
		if (!D3DX6InitPrim() ||
			(D3DX6DD->CreateClipper(0, &D3DX6Clip, NULL) != DD_OK) ||
			(D3DX6Clip == NULL) ||
			(D3DX6Clip->SetHWnd(0, MainFrame) != DD_OK) ||
			(D3DX6Prim->SetClipper(D3DX6Clip) != DD_OK))
		{
			D3DX6TermClip();
			return false;
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení clipperu (musí se volat pøed zrušením primárního povrchu)

void D3DX6TermClip()
{
	if (D3DX6Clip != NULL)
	{
		if (D3DX6Prim != NULL) D3DX6Prim->SetClipper(NULL);
		D3DX6Clip->Release();
		D3DX6Clip = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení palet, nejsou-li vytvoøeny (vrací TRUE=OK)

bool D3DX6InitPal()
{
	if (D3DX6Pal == NULL)
	{
		
// test, zda je potøeba použít palety
		if (!D3DX6InitPrim()) return false;
		DDSURFACEDESC2 ddsd;
		MemFill(&ddsd, sizeof(ddsd), 0);
		ddsd.dwSize = sizeof(ddsd);
		if (D3DX6Prim->GetSurfaceDesc(&ddsd) != DD_OK) return false;

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
			MemCopy(&SysPal[ResSysPal], pStdPalette->palPalEntry, StdColors*sizeof(PALETTEENTRY));
			int i;
			for (i=0; i < 256; i++) SysPal[i].peFlags = D3DPAL_READONLY;
			for (i = 10; i < 246; i++) SysPal[i].peFlags = D3DPAL_FREE | PC_RESERVED;

// vytvoøení palet
			if ((D3DX6DD->CreatePalette(DDPCAPS_8BIT, SysPal, &D3DX6Pal, NULL) != DD_OK) ||
				(D3DX6Pal == NULL))
			{
				D3DX6Pal = NULL;
				return false;
			}

// nastavení palet
			if (D3DX6Prim->SetPalette(D3DX6Pal) != DD_OK)
			{
				D3DX6TermPal();
				return false;
			}
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení palet (volat pøed zrušením primárního povrchu)

void D3DX6TermPal()
{
	if (D3DX6Pal != NULL)
	{
		D3DX6Pal->Release();
		D3DX6Pal = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// inicializace rozhraní Direct3D 6 (vrací TRUE=rozhraní je vytvoøeno)

bool D3DX6InitD3()
{
// test, zda inicializace již probìhla
	if (!D3DX6D3Test)
	{
		D3DX6D3Test = true;

// inicializace DirectDraw 6
		if (D3DX6InitDD())
		{

// vytvoøení objektu Direct3D 6 (mìní pøesnost koprocesoru!)
			if (D3DX6DD->QueryInterface(IID_IDirect3D3, (void**)&D3DX6D3) != DD_OK)
			{
				D3DX6D3 = NULL;
			}
		}

		FPUDouble();
	}

// test, zda je objekt Direct3D 6 vytvoøen
	return (D3DX6D3 != NULL);
}

/////////////////////////////////////////////////////////////////////////////
// ukonèení rozhraní Direct3D 6

void D3DX6TermD3()
{
// ukonèení renderování
	D3DX6Stop();

// zrušení objektu Direct3D 6
	if (D3DX6D3 != NULL)
	{
		D3DX6D3->Release();
		D3DX6D3 = NULL;
	}

// zrušení pøíznaku, že byla inicializace
	D3DX6D3Test = false;
}
	
/////////////////////////////////////////////////////////////////////////////
// vytvoøení renderovacího povrchu, není-li vytvoøen (vrací TRUE=OK)

bool D3DX6InitRend(bool hw)
{
	if (D3DX6Rend == NULL)
	{

// pøíprava typu pamìti
		DWORD memtype = 0;
		if (hw) memtype = DDSCAPS_VIDEOMEMORY;

// pøíprava popisovaèe povrchu
		DDSURFACEDESC2 ddsd;
		MemFill(&ddsd, sizeof(ddsd), 0);
		ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
		ddsd.dwWidth = D3DWidth;
		D3DX6RendWidth = D3DWidth;
		ddsd.dwHeight = D3DHeight;
		D3DX6RendHeight = D3DHeight;
		ddsd.ddsCaps.dwCaps = DDSCAPS_3DDEVICE | DDSCAPS_OFFSCREENPLAIN | memtype;

// vytvoøení povrchu
		if (!D3DX6InitSurf(&D3DX6Rend, &ddsd))
		{
			return false;
		}

// pøipojení palet
		if ((D3DX6Pal != NULL) &&
			(D3DX6Rend->SetPalette(D3DX6Pal) != DD_OK))
		{
			D3DX6TermRend();
			return false;
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení renderovacího povrchu

void D3DX6TermRend()
{
	D3DX6TermDev();

	if (D3DX6Rend != NULL)
	{
		D3DX6Rend->Release();
		D3DX6Rend = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení Z-bufferu, není-li vytvoøen (vrací TRUE=OK)

bool D3DX6InitZBuf(bool hw, DDPIXELFORMAT* pf)
{
	if (D3DX6ZBuf == NULL)
	{

// kontrola renderovacího povrchu
		if ((D3DX6Rend == NULL) || 
			(D3DX6D3 == NULL) ||
			(pf->dwZBufferBitDepth == 0)) return false;

// pøíprava typu pamìti
		DWORD memtype = DDSCAPS_SYSTEMMEMORY;
		if (hw) memtype = DDSCAPS_VIDEOMEMORY;

// pøíprava popisovaèe povrchu
		DDSURFACEDESC2 sd;						// popisovaè Z bufferu
		MemFill(&sd, sizeof(sd), 0);

		sd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
		sd.dwWidth = D3DWidth;
		sd.dwHeight = D3DHeight;
		sd.ddsCaps.dwCaps = DDSCAPS_ZBUFFER | memtype;
		MemCopy(&(sd.ddpfPixelFormat), pf, sizeof(DDPIXELFORMAT));

// vytvoøení povrchu
		if (!D3DX6InitSurf(&D3DX6ZBuf, &sd))
		{
//			sd.ddsCaps.dwCaps ^= (DDSCAPS_SYSTEMMEMORY ^ DDSCAPS_VIDEOMEMORY);
//
//			if (!D3DX6InitSurf(&D3DX6ZBuf, &sd))
//			{
				return false;
//			}
		}

// pøipojení k renderovacímu povrchu
		if (D3DX6Rend->AddAttachedSurface(D3DX6ZBuf) != DD_OK)
		{
			D3DX6TermZBuf();
			return false;
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení Z bufferu

void D3DX6TermZBuf()
{
	if (D3DX6ZBuf != NULL)
	{
		D3DX6ZBuf->Release();

		if (D3DX6Rend != NULL)
		{
			D3DX6Rend->DeleteAttachedSurface(0, D3DX6ZBuf);
		}

		D3DX6ZBuf = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// callback enumerace Z bufferù

HRESULT WINAPI D3DX6EnumZBufferCallback(DDPIXELFORMAT* pddpf, void* user)
{
	D3DEnumZBufferFormat(pddpf);

    return D3DENUMRET_OK;
}

/////////////////////////////////////////////////////////////////////////////
// callback enumerace formátu textury

HRESULT CALLBACK D3DX6EnumPixelFormatsCallback(DDPIXELFORMAT* pf, void* user)
{
//	if (((pf->dwFlags == (DDPF_RGB | DDPF_ALPHAPIXELS)) && 
//			(pf->dwRGBBitCount == 32)))
//	{
//		MemCopy(&D3DX6TextFormat, pf, sizeof(DDPIXELFORMAT));
//	}

	D3DEnumPixelFormat(pf);

	return D3DENUMRET_OK;
}

/////////////////////////////////////////////////////////////////////////////
// inicializace zaøízení pro Z-buffer

bool _fastcall D3DX6InitDev2(bool hw, int type, DDPIXELFORMAT* pf)
{
	if (D3DX6InitZBuf(hw, pf))
	{

// vytvoøení zaøízení (mìní pøesnost koprocesoru!)
		if ((D3DX6D3->CreateDevice(*Dev3DTab[type], D3DX6Rend, &D3DX6Dev, NULL) == DD_OK) &&
			(D3DX6Dev != NULL))
		{
			int bits = pf->dwStencilBitDepth;

			if ((pf->dwFlags & DDPF_STENCILBUFFER) && (bits > 0))
			{
				D3DSBufferFormatOK = true;
				D3DSBufferDepth = bits;
				D3DSBufferMax = ((1 << bits) - 1);
			}

			return true;
		}

		D3DX6Dev = NULL;

// zrušení Z bufferu
		D3DX6TermZBuf();
	}
	return false;
}
 
bool D3DX6InitDev3(bool hw, int type)
{
	if (ScreenBits <= 16)
	{
		return (
			D3DX6InitDev2(hw, type, &D3DZBufferFormat16S0) ||
			D3DX6InitDev2(hw, type, &D3DZBufferFormat15S1) ||
			D3DX6InitDev2(hw, type, &D3DZBufferFormat32S0) ||
			D3DX6InitDev2(hw, type, &D3DZBufferFormat24S8) ||
			D3DX6InitDev2(hw, type, &D3DZBufferFormat24S4) ||
			D3DX6InitDev2(hw, type, &D3DZBufferFormat24S0) ||
			D3DX6InitDev2(hw, type, &D3DZBufferFormat8S0));
	}
	else
	{
		return (
			D3DX6InitDev2(hw, type, &D3DZBufferFormat24S8) ||
			D3DX6InitDev2(hw, type, &D3DZBufferFormat24S4) ||
			D3DX6InitDev2(hw, type, &D3DZBufferFormat15S1) ||
			D3DX6InitDev2(hw, type, &D3DZBufferFormat32S0) ||
			D3DX6InitDev2(hw, type, &D3DZBufferFormat24S0) ||
			D3DX6InitDev2(hw, type, &D3DZBufferFormat16S0) ||
			D3DX6InitDev2(hw, type, &D3DZBufferFormat8S0));
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení renderovacího zaøízení (vrací TRUE=inicializace OK)
// typ zaøízení: 1=HAL,2=TnLHal,3=REF,4=RGB,5=MMX,6=Ramp,7=Null

bool D3DX6InitDev(int type)
{
// zrušení starých objektù
	D3DX6Stop();

// vytvoøení primárního povrchu, clipperu a palet
	if (D3DX6InitD3() &&
		D3DX6InitPrim() &&
		D3DX6InitClip() &&
		D3DX6InitPal())
	{

// enumerace Z bufferù
		if ((D3DX6D3->EnumZBufferFormats(*Dev3DTab[type],
				D3DX6EnumZBufferCallback, NULL) != D3D_OK) ||
				!D3DZBufferFormatOK)
		{
			D3DX6Stop();
			return false;
		}

// pøíznak HW ovladaèe
		bool hw = (type <= 2);

// vytvoøení renderovacího povrchu a Z bufferu
		if (!(D3DX6InitRend(hw) || D3DX6InitRend(!hw)) ||
			!(D3DX6InitDev3(hw, type) || D3DX6InitDev3(!hw, type)))
		{
			D3DX6Stop();
			return false;
		}
		
// enumerace formátù textur
		D3DX6Dev->EnumTextureFormats(D3DX6EnumPixelFormatsCallback, NULL);
		if (!D3DTextFormatOK)
		{
			D3DX6Stop();
			return false;
		}

		FPUDouble();
		return true;
	}

// pøi chybì zrušení vytvoøených objektù
	D3DX6Stop();

	return false;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení bufferu stínu

void D3DX6TermShadow()
{
	if (D3DX6ShadowBuf != NULL)
	{
		D3DX6ShadowBuf->Release();
		D3DX6ShadowBuf = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// zrušení renderovacího zaøízení

void D3DX6TermDev()
{
	D3DX6TermShadow();

	if (D3DX6Dev != NULL)
	{
		D3DX6Dev->Release();
		D3DX6Dev = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace materiálu pozadí scény

void D3DX6AktBackMat()
{
	if (D3DBackColAkt)
	{
		D3DBackColAkt = false;
		D3DX6BackMatBuf.diffuse.r = D3DBackColF.r;
		D3DX6BackMatBuf.diffuse.g = D3DBackColF.g;
		D3DX6BackMatBuf.diffuse.b = D3DBackColF.b;

		D3DX6BackMatBuf.ambient.r = D3DBackColF.r;
		D3DX6BackMatBuf.ambient.g = D3DBackColF.g;
		D3DX6BackMatBuf.ambient.b = D3DBackColF.b;

		D3DX6BackMat->SetMaterial(&D3DX6BackMatBuf);
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení materiálu pozadí scény

bool D3DX6InitBack()
{
	if (D3DX6BackMat == NULL)
	{

// musí existovat 3D objekt
		if (D3DX6D3 == NULL) return false;

// vytvoøení materiálu pozadí scény
		if ((D3DX6D3->CreateMaterial(&D3DX6BackMat, NULL) != D3D_OK) ||
			(D3DX6BackMat == NULL)) return false;

// inicializace materiálu
		MemFill(&D3DX6BackMatBuf, sizeof(D3DMATERIAL), 0);
		D3DX6BackMatBuf.dwSize = sizeof(D3DMATERIAL);
		D3DX6BackMatBuf.diffuse.a = 1;
		D3DX6BackMatBuf.ambient.a = 1;
		D3DX6BackMatBuf.specular.a = 1;
		D3DX6BackMatBuf.emissive.a = 1;
		D3DX6BackMatBuf.power = 0;
		D3DX6BackMatBuf.dwRampSize = 1;
		D3DBackColAkt = true;
		D3DX6AktBackMat();
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení materiálu pozadí scény (zøejmì až po zrušení viewportu)

void D3DX6TermBack()
{
	if (D3DX6BackMat != NULL)
	{
		D3DX6BackMat->Release();
		D3DX6BackMat = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// zrušení bufferu textury pozadí scény

void D3DX6TermBackBuf()
{
	if (D3DX6BackBuf != NULL)
	{
		D3DX6BackBuf->Release();
		D3DX6BackBuf = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// výpoèet nových souøadnic zobrazené oblasti

void D3DX6CalcView()
{
	D3DX6Src.x2 = D3DWidth;
	D3DX6Src.y2 = D3DHeight;

	D3DX6Dest.left = D3DLeft;
	D3DX6Dest.top = D3DTop;
	::ClientToScreen(MainFrame, (POINT*)&D3DX6Dest.left);

	D3DX6Dest.right = D3DX6Dest.left + D3DWidth;
	D3DX6Dest.bottom = D3DX6Dest.top + D3DHeight;
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace viewportu po zmìnì velikosti

void D3DX6SizeView()
{
// pøegenerování bufferu pozadí scény
	D3DX6TermBackBuf();

// pøegenerování bufferu stínù
	D3DX6TermShadow();

// pøepoèet videportu
	D3DX6CalcView();
	D3DX6ViewBuf.dwWidth = D3DWidth;
	D3DX6ViewBuf.dwHeight = D3DHeight;

// resetování bufferù 2D obrázkù
	D3DFITEM* item = D3DF_Get(1);
	int inx = item->Child;
	while (inx > 1)
	{
		item = D3DF_Get(inx);
		if ((item->Type == D3DFTYPE_PICTURE) || (item->Type == D3DFTYPE_LENSFLARE))
		{
			D3DX6Reset(item);
		}
		inx = item->Next;
	}

// zvìtšení bufferu
	if ((D3DHeight > D3DX6RendHeight) || (D3DWidth > D3DX6RendWidth))
	{
		FPUDouble();
		UserD3DDevice();
	}

// nastavení viewportu
	else
	{
		if (D3DX6Dev != NULL)
		{
			D3DX6View->SetViewport2(&D3DX6ViewBuf);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace viewportu po pøesunu

void D3DX6MoveView()
{
	D3DX6CalcView();
}


/////////////////////////////////////////////////////////////////////////////
// vytvoøení viewportu (vrací TRUE=inicializace OK)

bool D3DX6InitView()
{
	if (D3DX6View == NULL)
	{

// musí existovat 3D zaøízení a 3D objekt
		if (D3DX6Dev == NULL) return false;

// vytvoøení viewportu
		if ((D3DX6D3->CreateViewport(&D3DX6View, NULL) != D3D_OK) ||
			(D3DX6View == NULL)) return false;

		D3DMATERIALHANDLE mh = NULL;

// pøipojení viewportu k 3D zaøízení
		if ((D3DX6Dev->AddViewport(D3DX6View) != D3D_OK) ||
			(D3DX6Dev->SetCurrentViewport(D3DX6View) != D3D_OK) ||

// vytvoøení materiálu pozadí scény a pøipojení k viewportu
			!D3DX6InitBack() ||

// handle materiálu
			(D3DX6BackMat->GetHandle(D3DX6Dev, &mh) != D3D_OK) ||
			(mh == NULL) ||

// pøipojení materiálu k viewportu
			(D3DX6View->SetBackground(mh) != D3D_OK))
		{
			D3DX6TermView();
			return false;
		}

// inicializace bufferu dat viewportu
		D3DX6Src.x1 = 0;
		D3DX6Src.y1 = 0;
		D3DX6ViewBuf.dwSize = sizeof(D3DX6ViewBuf);
		D3DX6ViewBuf.dwX = 0;
		D3DX6ViewBuf.dwY = 0;
		D3DX6ViewBuf.dvClipX = -1;
		D3DX6ViewBuf.dvClipWidth = 2;
		D3DX6ViewBuf.dvClipY = 1;
		D3DX6ViewBuf.dvClipHeight = 2;
		D3DX6ViewBuf.dvMinZ = 0;
		D3DX6ViewBuf.dvMaxZ = 1;

		D3DX6SizeView();
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení viewportu

void D3DX6TermView()
{
	if (D3DX6View != NULL)
	{
		D3DX6View->Release();
		D3DX6View = NULL;
	}

	D3DX6TermBack();		// zrušení materiálu pozadí scény
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení bufferu stínu, není-li vytvoøen

struct SHADOWVERTEX
{
    D3DXVECTOR4 position;
	D3DCOLOR color;
};

#define D3DFVF_SHADOWVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

void D3DX6InitShadow()
{
	if (D3DX6ShadowBuf == NULL)
	{
		D3DVERTEXBUFFERDESC vb;
		vb.dwSize = sizeof(D3DVERTEXBUFFERDESC);
		vb.dwCaps = D3DVBCAPS_WRITEONLY | D3DVBCAPS_SYSTEMMEMORY;
		vb.dwFVF = D3DFVF_SHADOWVERTEX;
		vb.dwNumVertices = 4;

		D3DX6D3->CreateVertexBuffer(&vb, &D3DX6ShadowBuf, 0, NULL);

		if (D3DX6ShadowBuf != NULL)
		{
			SHADOWVERTEX* pVertices;
			if(D3DX6ShadowBuf->Lock(DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS, (void**)&pVertices, NULL) == D3D_OK)
			{
				pVertices[0].position.x = 0;
				pVertices[0].position.y = 0;
				pVertices[0].position.z = 1;
				pVertices[0].position.w = 1;
				pVertices[0].color = 0x80808080;

				pVertices[1].position.x = (float)D3DWidth;
				pVertices[1].position.y = 0;
				pVertices[1].position.z = 1;
				pVertices[1].position.w = 1;
				pVertices[1].color = 0x80808080;

				pVertices[2].position.x = 0;
				pVertices[2].position.y = (float)D3DHeight;
				pVertices[2].position.z = 1;
				pVertices[2].position.w = 1;
				pVertices[2].color = 0x80808080;

				pVertices[3].position.x = (float)D3DWidth;
				pVertices[3].position.y = (float)D3DHeight;
				pVertices[3].position.z = 1;
				pVertices[3].position.w = 1;
				pVertices[3].color = 0x80808080;

				D3DX6ShadowBuf->Unlock();
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení bufferu pozadí scény, není-li vytvoøen

struct BACKVERTEX
{
    D3DXVECTOR4 position;
	float tu, tv;
};

#define D3DFVF_BACKVERTEX (D3DFVF_XYZRHW|D3DFVF_TEX1)

void D3DX6InitBackBuf()
{
	if (D3DX6BackBuf == NULL)
	{
		D3DVERTEXBUFFERDESC vb;
		vb.dwSize = sizeof(D3DVERTEXBUFFERDESC);
		vb.dwCaps = D3DVBCAPS_WRITEONLY | D3DVBCAPS_SYSTEMMEMORY;
		vb.dwFVF = D3DFVF_BACKVERTEX;
		vb.dwNumVertices = 4;

		D3DX6D3->CreateVertexBuffer(&vb, &D3DX6BackBuf, 0, NULL);

		if (D3DX6BackBuf != NULL)
		{
			BACKVERTEX* pVertices;
			if(D3DX6BackBuf->Lock(DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS, (void**)&pVertices, NULL) == D3D_OK)
			{
				pVertices[0].position.x = 0;
				pVertices[0].position.y = 0;
				pVertices[0].position.z = 1;
				pVertices[0].position.w = 1;
				pVertices[0].tu = 0;
				pVertices[0].tv = 0;

				pVertices[1].position.x = (float)D3DWidth;
				pVertices[1].position.y = 0;
				pVertices[1].position.z = 1;
				pVertices[1].position.w = 1;
				pVertices[1].tu = 1;
				pVertices[1].tv = 0;

				pVertices[2].position.x = 0;
				pVertices[2].position.y = (float)D3DHeight;
				pVertices[2].position.z = 1;
				pVertices[2].position.w = 1;
				pVertices[2].tu = 0;
				pVertices[2].tv = 1;

				pVertices[3].position.x = (float)D3DWidth;
				pVertices[3].position.y = (float)D3DHeight;
				pVertices[3].position.z = 1;
				pVertices[3].position.w = 1;
				pVertices[3].tu = 1;
				pVertices[3].tv = 1;

				D3DX6BackBuf->Unlock();
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// zahájení funkce zaøízení (vrací TRUE=inicializace OK)
// typ zaøízení: 1=HAL,2=TnLHal,3=REF,4=RGB,5=MMX,6=Ramp,7=Null

bool D3DX6Start(int type)
{
// vytvoøení renderovacího zaøízení
	if (!D3DX6InitDev(type)) return false;

// vytvoøení viewportu
	if (!D3DX6InitView())
	{
		D3DX6Stop();
		return false;
	}

// inicializace adres funkcí zaøízení
	pD3Stop = D3DX6Stop;			// ukonèení funkce zaøízení
	pD3SizeView = D3DX6SizeView;	// aktualizace viewportu po zmìnì velikosti
	pD3MoveView = D3DX6MoveView;	// aktualizace viewportu po pøesunu
	pD3Disp = D3DX6Disp;			// zobrazení renderovacího bufferu
	pD3Clear = D3DX6Clear;			// vymazání renderovací plochy
	pD3AktProj = D3DX6AktProj;		// aktualizace projekèní matice
	pD3AktView = D3DX6AktView;		// aktualizace pohledové matice
	pD3Begin = D3DX6Begin;			// zahájení renderování scény
	pD3End = D3DX6End;				// ukonèení renderování scény
	pD3Render = D3DX6Render;		// vyrenderování jednoho objektu
	pD3Reset = D3DX6Reset;			// resetování objektu pøed ukonèením ovladaèe
	pD3SReset = D3DX6SReset;		// resetování stínu objektu pøed ukonèením ovladaèe
	pD3AktWireframe = D3DX6AktWireframe;
	pD3AktLightOn = D3DX6AktLightOn;
	pD3AktShades = D3DX6AktShades;
	pD3AktCulling = D3DX6AktCulling;
	pD3AktMagFilter = D3DX6AktMagFilter;
	pD3AktMinMipFilter = D3DX6AktMinMipFilter;
	pD3AktLight = D3DX6AktLight;
	pD3SetMaterial = D3DX6SetMaterial;
	pD3TextureRes = D3DX6TextureRes;
	pD3TextureSet = D3DX6TextureSet;
	pD3TextureOn = D3DX6TextureOn;
	pD3TextureOff = D3DX6TextureOff;
	pD3MatReset = D3DX6MatReset;
	pD3LightReset = D3DX6LightReset;
	pD3AktAmbient = D3DX6AktAmbient;
	pD3Free = D3DX6Free;
	pD3AktFog = D3DX6AktFog;
	pD3FogOn = D3DX6FogOn;
	pD3AktState = D3DX6AktState;
	pD3AktBlend = D3DX6AktBlend;
	pD3AktAlfa = D3DX6AktAlfa;
	pD3SetColorOp = D3DX6SetColorOp;
	pD3SetAlphaOp = D3DX6SetAlphaOp;
	pD3SetAddress = D3DX6SetAddress;
//	pD3SetBorder = D3DX6SetBorder;
	pD3SetBias = D3DX6SetBias;
	pD3AktPalette = D3DX6AktPalette;
	pD3SetTFactor = D3DX6SetTFactor;
	pD3SetZFunc = D3DX6SetZFunc;
	pD3SetZWrite = D3DX6SetZWrite;
	pD3SEnable = D3DX6SEnable;
	pD3SetSOp = D3DX6SetSOp;
	pD3SetSFunc = D3DX6SetSFunc;
	pD3SetSMask = D3DX6SetSMask;
	pD3Shadow = D3DX6Shadow;
	pD3Shadows = D3DX6Shadows;

// inicializace informací o zaøízení
	D3DDEVICEDESC D3DCapsHW;
	D3DDEVICEDESC D3DCapsSW;
	MemFill(&D3DCapsHW, sizeof(D3DDEVICEDESC), 0);
	D3DCapsHW.dwSize = sizeof(D3DDEVICEDESC);
	MemFill(&D3DCapsSW, sizeof(D3DDEVICEDESC), 0);
	D3DCapsSW.dwSize = sizeof(D3DDEVICEDESC);

	D3DX6Dev->GetCaps(&D3DCapsHW, &D3DCapsSW);

	D3DDEVICEDESC* D3DCaps;
	if (D3DCapsHW.dcmColorModel != 0)
	{
		D3DCaps = &D3DCapsHW;
	}
	else
	{
		D3DCaps = &D3DCapsSW;
	}

	MaxTextureWidth = D3DCaps->dwMaxTextureWidth;
	MaxTextureHeight = D3DCaps->dwMaxTextureHeight;
	MaxTextureRepeat = D3DCaps->dwMaxTextureRepeat;

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
	MaxTextureStages = D3DCaps->wMaxTextureBlendStages;
	MaxTextureSimult = D3DCaps->wMaxSimultaneousTextures;

	IsHWRaster = (type <= 2);

	if (D3DCaps->dwMaxVertexCount > 0)
	{
		MaxVertexNum = D3DCaps->dwMaxVertexCount;
	}

	bool isfog = ((D3DCaps->dpcTriCaps.dwShadeCaps & D3DPSHADECAPS_FOGGOURAUD) != 0);
	IsTableFog = ((D3DCaps->dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_FOGTABLE) != 0) && isfog;
	IsVertexFog = false;
	IsRangeFog = ((D3DCaps->dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_FOGRANGE) != 0) && isfog;

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// ukonèení funkce zaøízení

void D3DX6Stop()
{
	D3DX6TermBackBuf();		// zrušení bufferu pozadí scény
	D3DX6TermShadow();		// zrušení bufferu stínu
	D3DX6TermView();		// zrušení viewportu
	D3DX6TermZBuf();		// zrušení Z bufferu
	D3DX6TermRend();		// zrušení renderovacího povrchu
	D3DX6TermPal();			// zrušení palet
	D3DX6TermClip();		// zrušení clipperu
	D3DX6TermPrim();		// zrušení primárního povrchu
	FPUDouble();			// návrat pøesnosti koprocesoru
}

/////////////////////////////////////////////////////////////////////////////
// vymazání renderovací plochy

void D3DX6Clear()
{
// aktualizace materiálu pozadí scény
	D3DX6AktBackMat();

// test, zda je textura
	if (D3DBackText >= 0)
	{

// vytvoøení bufferu pozadí scény
		D3DX6InitBackBuf();

		if (D3DX6BackBuf != NULL)
		{

// renderování
			D3DX6Dev->DrawPrimitiveVB(
				D3DPT_TRIANGLESTRIP,
				D3DX6BackBuf,
				0,
				4,
				0);
		}

// závìreèné vymazání Z bufferu
		D3DX6View->Clear(1, &D3DX6Src, D3DCLEAR_ZBUFFER | (D3DSBufferClear ? D3DCLEAR_STENCIL : 0));
	}
	else
	{

// pozadí se neinicializuje
		if (D3DBackCol == (MAXDWORD-1))
		{
			if (D3DSBufferClear)
			{
				D3DX6View->Clear(1, &D3DX6Src,  D3DCLEAR_STENCIL);
			}
			return;
		}

// není textura, je barva - vymazání barvy i Z bufferu
		if (D3DBackCol != MAXDWORD)
		{
			D3DX6View->Clear(1, &D3DX6Src, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | (D3DSBufferClear ? D3DCLEAR_STENCIL : 0));
		}
		else

// jinak pouze vymazání Z bufferu
		{
			D3DX6View->Clear(1, &D3DX6Src, D3DCLEAR_ZBUFFER | (D3DSBufferClear ? D3DCLEAR_STENCIL : 0));
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// zobrazení renderovacího bufferu

void D3DX6Disp()
{
	if ((D3DX6DD == NULL) || (D3DX6Prim == NULL)) return;
	if (D3DVSync) D3DX6DD->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, 0);
	if (D3DX6Prim->Blt(&D3DX6Dest, D3DX6Rend, (RECT*)&D3DX6Src, DDBLT_WAIT, NULL) == DDERR_SURFACELOST)
	{
		D3DX6Prim->Restore();
		D3DX6Rend->Restore();
		D3DX6ZBuf->Restore();
	}
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace projekèní matice

void D3DX6AktProj()
{
	D3DX6Dev->SetTransform(D3DTRANSFORMSTATE_PROJECTION, &D3DProjMatrix);
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace pohledové matice

void D3DX6AktView()
{
	D3DX6Dev->SetTransform(D3DTRANSFORMSTATE_VIEW, &D3DViewMatrix);
}


/////////////////////////////////////////////////////////////////////////////
// zahájení renderování scény

void D3DX6Begin()
{
	D3DX6Dev->BeginScene();
}


/////////////////////////////////////////////////////////////////////////////
// ukonèení renderování scény

void D3DX6End()
{
	D3DX6Dev->EndScene();
}


/////////////////////////////////////////////////////////////////////////////
// vyrenderování jednoho objektu

void D3DX6Render(D3DFITEM* item)
{
// Pozn.: Nefunguje souèasnì normály a barva, barva vrcholù proto lze
//        využít jen bez svìtla, podobnì jako v DirectX 5

// zrušení bufferu, je-li zmìna osvìtlení
	if ((void*)D3DLightOnAkt != item->Data2)
	{
		D3DX6Reset(item);
		item->Data2 = (void*)D3DLightOnAkt;
	}

	bool isFlare = (item->Type == D3DFTYPE_LENSFLARE);
	bool isPic = ((item->Type == D3DFTYPE_PICTURE) || isFlare);

// pøegenerování bufferu pro LensFlare
	if (isFlare) D3DX6Reset(item);

// vytvoøení bufferu vektorù (Data1), není-li ještì vytvoøen
	if (item->Data1 == NULL)
	{
		bool isNorm = (D3DLightOnAkt && !isPic);

		D3DVERTEXBUFFERDESC vb;
		vb.dwSize = sizeof(D3DVERTEXBUFFERDESC);
		vb.dwCaps = D3DVBCAPS_WRITEONLY | D3DVBCAPS_SYSTEMMEMORY;
		vb.dwFVF = D3DFVF_XYZ;
		if (isPic) vb.dwFVF = D3DFVF_XYZRHW;
		vb.dwNumVertices = item->VertNum;
		D3DCOLOR col = XRGB(item->Diffuse);

		if (isNorm)
		{
			vb.dwFVF |= D3DFVF_NORMAL;
		}
		else
		{
			vb.dwFVF |= D3DFVF_DIFFUSE;
		}

		int stages = item->Stages;
		float* textUV[MAX_STAGES];
		int stage;

		ASSERT(item->TextUV[0] != NULL);

		for (stage = 0; stage < stages; stage++)
		{
			if (item->TextUV[stage] != NULL)
			{
				textUV[stage] = item->TextUV[stage];
			}
			else
			{
				textUV[stage] = textUV[stage-1];
			}
		}

		vb.dwFVF |= (D3DFVF_TEX0 + (D3DFVF_TEX1-D3DFVF_TEX0)*stages);

		IDirect3DVertexBuffer* vertbuf = NULL;
		D3DX6D3->CreateVertexBuffer(&vb, &vertbuf, 0, NULL);
		item->Data1 = vertbuf;

		if (vertbuf != NULL)
		{
			void* vert = NULL;

			if(vertbuf->Lock(DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS, &vert, NULL) == D3D_OK)
			{
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
				
				for (int i = 0; i < item->VertNum; i++)
				{
					if (isPic)
					{
						double x = item->Vertex[i].x*item->ScaleX;
						double y = item->Vertex[i].y*item->ScaleY;

						((D3DVECTOR*)vert)->x = (float)((x*item->CosZ - y*sinz1 + sx + 0.5)*D3DWidth-0.5);
						((D3DVECTOR*)vert)->y = (float)((0.5 - (x*sinz2 + y*item->CosZ) - sy)*D3DHeight-0.5);
						((D3DVECTOR*)vert)->z = 0;
						vert = ((D3DVECTOR*)vert) + 1;

						*((float*)vert) = 1;
						vert = ((float*)vert) + 1;
					}
					else
					{
						((D3DVECTOR*)vert)->x = item->Vertex[i].x;
						((D3DVECTOR*)vert)->y = item->Vertex[i].y;
						((D3DVECTOR*)vert)->z = item->Vertex[i].z;
						vert = ((D3DVECTOR*)vert) + 1;
					}

					if (isNorm)
					{
						((D3DVECTOR*)vert)->x = item->Normal[i].x;
						((D3DVECTOR*)vert)->y = item->Normal[i].y;
						((D3DVECTOR*)vert)->z = item->Normal[i].z;
						vert = ((D3DVECTOR*)vert) + 1;
					}
					else
					{
						*((DWORD*)vert) = col;
						vert = ((DWORD*)vert) + 1;
					}

					for (stage = 0; stage < stages; stage++)
					{
						*((float*)vert) = textUV[stage][2*i];
						vert = ((float*)vert) + 1;
						*((float*)vert) = textUV[stage][2*i+1];
						vert = ((float*)vert) + 1;
					}
				}
				vertbuf->Unlock();
			}
		}

		int i = item->FaceNum * 3;
		MemFree(item->FaceW);
		WORD* fd = (WORD*)MemGet(i * sizeof(WORD));
		item->FaceW = fd;
		int* fs = item->Face;

		for (; i > 0; i--)
		{
			*fd = (WORD)*fs;
			fs++;
			fd++;
		}

// aktualizace klonù
		D3DFITEM* item2 = item;

		for (;;)
		{
			item2 = D3DF_Get(item2->CloneNext);
			if (item2 == item) break;

			item2->Data1 = item->Data1;
			item2->Data2 = item->Data2;
			item2->FaceW = item->FaceW;
		}
	}

// nastavení transformaèní matice
	D3DX6Dev->SetTransform(D3DTRANSFORMSTATE_WORLD, &item->MatrixWorld);

// vyrenderování objektu
	IDirect3DVertexBuffer* vertbuf = (IDirect3DVertexBuffer*)(item->Data1);

	if (vertbuf != NULL)
	{
		D3DX6Dev->DrawIndexedPrimitiveVB(
			D3DPT_TRIANGLELIST,
			vertbuf,
			item->FaceW,
			item->FaceNum*3,
			0);
	}
}

/////////////////////////////////////////////////////////////////////////////
// vyrenderování stínu

struct SVERTEX
{
    D3DXVECTOR3 pos;
	D3DCOLOR col;
};

void D3DX6Shadow(D3DFITEM* item)
{
// pøíprava typu položky
	int itemtype;

	if (D3DSBufferFormatOK)
	{
		itemtype = (D3DFVF_XYZ);
	}
	else
	{
		itemtype = (D3DFVF_XYZ  | D3DFVF_DIFFUSE);
	}

// vytvoøení bufferu vektorù (Data3), není-li ještì vytvoøen
	if ((item->Data3 == NULL) && (item->ShadowFan0 > 0))
	{
		D3DVERTEXBUFFERDESC vb;
		vb.dwSize = sizeof(D3DVERTEXBUFFERDESC);
		vb.dwCaps = D3DVBCAPS_WRITEONLY | D3DVBCAPS_SYSTEMMEMORY;
		vb.dwFVF = itemtype;
		vb.dwNumVertices = item->ShadowFan0;

		IDirect3DVertexBuffer* vertbuf = NULL;
		D3DX6D3->CreateVertexBuffer(&vb, &vertbuf, 0, NULL);
		item->Data3 = vertbuf;

		if (vertbuf != NULL)
		{
			void* vert = NULL;

			if(vertbuf->Lock(DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS, &vert, NULL) == D3D_OK)
			{
				if (D3DSBufferFormatOK)
				{
					MemCopy(vert, item->ShadowVolume, item->ShadowFan0 * sizeof(D3DVECTOR));
				}
				else
				{
					D3DVECTOR* src = item->ShadowVolume;
					SVERTEX* dst = (SVERTEX*)vert;

					for (int i = item->ShadowFan0; i > 0; i--)
					{
						dst->pos.x = src->x;
						dst->pos.y = src->y;
						dst->pos.z = src->z;
						dst->col = 0xaa808080;

						dst++;
						src++;
					}
				}

				vertbuf->Unlock();
			}
		}
	}

// nastavení transformaèní matice
	D3DX6Dev->SetTransform(D3DTRANSFORMSTATE_WORLD, &item->MatrixWorld);

// vyrenderování objektu
	IDirect3DVertexBuffer* vertbuf = (IDirect3DVertexBuffer*)(item->Data3);

	if (vertbuf != NULL)
	{
		int v = 0;

		for (int i = 0; i < item->ShadowFanN; i++)
		{
			int vn = item->ShadowFan[i];

			D3DX6Dev->DrawPrimitiveVB(
				D3DPT_TRIANGLEFAN,
				vertbuf,
				v,
				vn,
				0);

			v += vn;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// resetování stínu objektu pøed ukonèením ovladaèe

void D3DX6SReset(D3DFITEM* item)
{
// resetování bufferu stínù
	if (item->Data3 != NULL)
	{
		((IDirect3DVertexBuffer*)(item->Data3))->Release();
		item->Data3 = NULL;
	}
}


/////////////////////////////////////////////////////////////////////////////
// resetování objektu pøed ukonèením ovladaèe

void D3DX6Reset(D3DFITEM* item)
{
// resetování bufferu vrcholù
	if (item->Data1 != NULL)
	{
		((IDirect3DVertexBuffer*)(item->Data1))->Release();

		D3DFITEM* item2 = item;

		do {
			item2->Data1 = NULL;
			item2->Data2 = NULL;

			item2 = D3DF_Get(item2->CloneNext);
		} while (item != item2);
	}

// resetování bufferu stínù
	D3DX6SReset(item);
}

/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè vyplòování ploch

void D3DX6AktWireframe()
{
	D3DX6Dev->SetRenderState(D3DRENDERSTATE_FILLMODE, (D3DWireframeAkt ? D3DFILL_SOLID : D3DFILL_WIREFRAME));
}


/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè osvìtlení

void D3DX6AktLightOn() { }


/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè plynulého stinování

void D3DX6AktShades()
{
	D3DX6Dev->SetRenderState(D3DRENDERSTATE_SHADEMODE, (D3DShadesAkt ? D3DSHADE_GOURAUD : D3DSHADE_FLAT));
}


/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè odstraòování ploch

void D3DX6AktCulling()
{
	int n;

	switch (D3DCullingAkt)
	{
	case 1:
		n = D3DCULL_CCW;
		break;

	case 2:
		n = D3DCULL_CW;
		break;

	default:
		n = D3DCULL_NONE;
	}

	D3DX6Dev->SetRenderState(D3DRENDERSTATE_CULLMODE, n);
}


/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè filtrace zvìtšených textur

void D3DX6AktMagFilter(int stage)
{
	D3DX6Dev->SetTextureStageState(stage, D3DTSS_MAGFILTER, (D3DMagFilterAkt[stage] ? D3DTFG_LINEAR : D3DTFG_POINT));
}


/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè filtrace zmenšených a vzdálených textur

void D3DX6AktMinMipFilter(int stage)
{
	DWORD min = D3DTFN_POINT;
	DWORD mip = D3DTFP_NONE;

	if (D3DMipFilterAkt[stage]) mip = D3DTFP_POINT;

	if (D3DMinFilterAkt[stage])
	{
		min = D3DTFN_LINEAR;
		if (D3DMipFilterAkt[stage]) mip = D3DTFP_LINEAR;
	}

	D3DX6Dev->SetTextureStageState(stage, D3DTSS_MINFILTER, min);
	D3DX6Dev->SetTextureStageState(stage, D3DTSS_MIPFILTER, mip);
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace položky svìtla

void D3DX6AktLight(int inx, D3DLITEM* item)
{
	if (item->Akt)
	{
		if (item->Enable)
		{
			if (item->Data1 == NULL)
			{
				D3DX6D3->CreateLight((IDirect3DLight**)&(item->Data1), NULL);
			}

			if (item->Data1 != NULL)
			{
				D3DLIGHT2 l;
				l.dwSize = sizeof(D3DLIGHT2);

				D3DLIGHT8* l2 = &(item->Light);

				l.dltType = l2->Type;

				l.dcvColor.r = l2->Diffuse.r;
				l.dcvColor.g = l2->Diffuse.g;
				l.dcvColor.b = l2->Diffuse.b;
				l.dcvColor.a = l2->Diffuse.a;

				MemCopy(&(l.dvPosition), &(l2->Position), 2*sizeof(D3DVECTOR) + 7*sizeof(float));

				float a1 = l.dvAttenuation1;
				float a2 = l.dvAttenuation2;
				float r = l.dvRange;

		// pro D3DLIGHT2 je rovnice jasu svìtla = b0 + b1*(1-d/r) + b2*(1-d/r)^2
		// -> ve vzdálenosti dosvitu (d=r) je jas = b0
		// -> v místì zdroje svìtla (d=0) je jas = b0 + b1 + b2 = 1, odtud b2 = 1 - b1 - b0
		// -> v polovièní vzdálenosti dosvitu (d=r/2) je jas = b0 + b1/2 + b2/4 = m
		//    odtud úpravami b1 = 4*m - 3*b0 - 1

				float b0 = (float)(1/(1 + a1*r + a2*r*r));	// jas v bodì dosvitu
				float m = (float)(1/(1 + a1*r/2 + a2*r*r/4)); // jas v polovièní vzdálenosti
				float b1 = (float)(4*m - 3*b0 - 1);			// koeficient 1
				float b2 = (float)(1 - b1 - b0);			// koeficient 2

				l.dvAttenuation0 = b0;
				l.dvAttenuation1 = b1;
				l.dvAttenuation2 = b2;

				l.dwFlags = D3DLIGHT_ACTIVE;

				((IDirect3DLight*)(item->Data1))->SetLight((D3DLIGHT*)&l);
			}

			if ((item->Data2 == NULL) && (item->Data1 != NULL))
			{
				D3DX6View->AddLight((IDirect3DLight*)(item->Data1));
				item->Data2 = (void*)TRUE;
			}
		}
		else
		{
			if (item->Data2 != NULL)
			{
				D3DX6View->DeleteLight((IDirect3DLight*)(item->Data1));
				item->Data2 = NULL;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení materiálu

void D3DX6SetMaterial(D3DMITEM* item)
{
// vytvoøení objektu materiálu
	if (item->Data1 == NULL)
	{
		IDirect3DMaterial3* mat = NULL;
		D3DX6D3->CreateMaterial(&mat, NULL);
		if (mat != NULL)
		{
			item->Data1 = mat;
			
			D3DMATERIAL m;
			m.dwSize = sizeof(D3DMATERIAL);
			D3DMATERIAL8* m2 = &(item->Material);
			MemCopy(&(m.diffuse), &(m2->Diffuse), 4*sizeof(D3DCOLORVALUE) + sizeof(float));
			m.hTexture = NULL;
			m.dwRampSize = 20;

			mat->SetMaterial(&m);
			mat->GetHandle(D3DX6Dev, (D3DMATERIALHANDLE*) &(item->Data2));
		}
	}

// nastavení materiálu
	D3DX6Dev->SetLightState(D3DLIGHTSTATE_MATERIAL, (DWORD)item->Data2);

// ruší aktivní texturu
	D3DT_Active[0] = -1;
}


/////////////////////////////////////////////////////////////////////////////
// resetování položky textury pøed ukonèením ovladaèe

void D3DX6TextureRes(D3DTITEM* item)
{
	if (item->Data1 != NULL)
	{
		((IDirect3DTexture2*)(item->Data1))->Release();
		item->Data1 = NULL;
		item->Data2 = NULL;
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastavení a zapnutí textury

int tx6Width;
int tx6Height;
int tx6Mip;

BYTE* tx6Data;
int tx6WidthMul;

IDirectDrawSurface4* tx6Tex;
DDPIXELFORMAT* tx6PF;

BOOL _fastcall D3DX6TextCreate(DDPIXELFORMAT* pf, BYTE* data, int mul)
{
	if (pf->dwRGBBitCount == 0) return FALSE;

	tx6Data = data;
	tx6WidthMul = mul;

	tx6Tex = NULL;
	tx6PF = pf;

	DDSURFACEDESC2 ddsd;
	MemFill(&ddsd, sizeof(ddsd), 0);
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwWidth = tx6Width;
	ddsd.dwHeight = tx6Height;
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_SYSTEMMEMORY;
	MemCopy(&(ddsd.ddpfPixelFormat), pf, sizeof(DDPIXELFORMAT));

	if (tx6Mip > 1)
	{
		ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_MIPMAPCOUNT | DDSD_PIXELFORMAT;
		ddsd.dwMipMapCount = tx6Mip;
		ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP | DDSCAPS_COMPLEX | DDSCAPS_SYSTEMMEMORY;
	}

	return ((D3DX6DD->CreateSurface(&ddsd, &tx6Tex, NULL) == DD_OK) &&
			(tx6Tex != NULL));
}


void D3DX6TextureSet(D3DTITEM* item, int stage)
{
// data obrázku
	PICTUREDATA* picdata = item->Pic.Data();

// vytvoøení textury
	if (item->Data1 == NULL)
	{
		tx6Width = picdata->TextWidth;
		tx6Height = picdata->TextHeight;

// poèet mipmap
		tx6Mip = 1;
		if (IsMipMap && item->Mips)
		{
			int n = tx6Width;
			if (n > tx6Height) n = tx6Height;
  
			while (n > 1)
			{
				n = (n >> 1);
				tx6Mip++;
			}
		}
		if (tx6Mip > picdata->TextMipMaps) tx6Mip = picdata->TextMipMaps;

		// omezení poètu úrovní - pøi vyšším stupni se prolínají ikony textury terénu
		int mipmaps = item->MipMaps;
		if (mipmaps > 0)
		{
			if (tx6Mip > mipmaps) tx6Mip = mipmaps;
		}

		int mip0 = tx6Mip;

// vytvoøení povrchu v systémové pamìti
		if ((!picdata->TextTrans && (D3DX6TextCreate(&D3DTextFormatR5G6B5, picdata->TextDataR5G6B5, 2) ||
									D3DX6TextCreate(&D3DTextFormatA1R5G5B5, picdata->TextDataA1R5G5B5, 2))) ||
			((D3DLowText || (picdata->TextSmooth < 1.25)) && 
								(D3DX6TextCreate(&D3DTextFormatA4R4G4B4, picdata->TextDataA4R4G4B4, 2) ||
									D3DX6TextCreate(&D3DTextFormatA1R5G5B5, picdata->TextDataA1R5G5B5, 2))) ||
			D3DX6TextCreate(&D3DTextFormatA8R8G8B8, picdata->TextData, 4) ||
			D3DX6TextCreate(&D3DTextFormatA4R4G4B4, picdata->TextDataA4R4G4B4, 2) ||
			D3DX6TextCreate(&D3DTextFormatA1R5G5B5, picdata->TextDataA1R5G5B5, 2) ||
			D3DX6TextCreate(&D3DTextFormatR5G6B5, picdata->TextDataR5G6B5, 2) ||
			D3DX6TextCreate(&D3DTextFormatX1R5G5B5, picdata->TextDataA1R5G5B5, 2) ||
			D3DX6TextCreate(&D3DTextFormatR8G8B8, picdata->TextDataR8G8B8, 3) ||
			D3DX6TextCreate(&D3DTextFormatX8R8G8B8, picdata->TextData, 4) ||
			D3DX6TextCreate(&D3DTextFormatX4R4G4B4, picdata->TextDataA4R4G4B4, 2))
		{
			IDirectDrawSurface4* ds0 = tx6Tex;

			tx6Tex->AddRef();
			BYTE* src = tx6Data;
				
			DDSCAPS2 caps;
			MemFill(&caps, sizeof(DDSCAPS2), 0);
			caps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;

			for (; tx6Mip > 0; tx6Mip--)
			{
				DDSURFACEDESC2 ddsd;
				MemFill(&ddsd, sizeof(ddsd), 0);
				ddsd.dwSize = sizeof(ddsd);

				if (tx6Tex->Lock(NULL, &ddsd, 0, NULL) == DD_OK)
				{
					int pitch = ddsd.lPitch;
					BYTE* dst = (BYTE*)ddsd.lpSurface;
					int widthbyte = tx6Width * tx6WidthMul;

					if (pitch >= widthbyte)
					{
						for (int i = tx6Height; i > 0; i--)
						{
							MemCopy(dst, src, widthbyte);
							dst += pitch;
							src += widthbyte;
						}
					}

					tx6Tex->Unlock(NULL);
				}

				tx6Width = (tx6Width >> 1);
				tx6Height = (tx6Height >> 1);

				IDirectDrawSurface4* dsnext = NULL;

				if (tx6Mip > 1) tx6Tex->GetAttachedSurface(&caps, &dsnext);
				tx6Tex->Release();
				tx6Tex = dsnext;
				if (tx6Tex == NULL) break;
			}

// pro HW rasterizer pøenesení do videopamìti
			if (IsHWRaster)
			{	 
				DDSURFACEDESC2 ddsd;
				MemFill(&ddsd, sizeof(ddsd), 0);
				ddsd.dwSize = sizeof(ddsd);
				ddsd.dwWidth = picdata->TextWidth;
				ddsd.dwHeight = picdata->TextHeight;
				ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
				ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_VIDEOMEMORY | DDSCAPS_ALLOCONLOAD;
				MemCopy(&(ddsd.ddpfPixelFormat), tx6PF, sizeof(DDPIXELFORMAT));

				if (mip0 > 1)
				{
					ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_MIPMAPCOUNT | DDSD_PIXELFORMAT;
					ddsd.dwMipMapCount = mip0;
					ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP | DDSCAPS_COMPLEX | DDSCAPS_VIDEOMEMORY | DDSCAPS_ALLOCONLOAD;
				}

				if (D3DX6DD->CreateSurface(&ddsd, &tx6Tex, NULL) == DD_OK)
				{
					IDirect3DTexture2* tx = NULL;
					ds0->QueryInterface(IID_IDirect3DTexture2, (void**)&tx);

					if (tx != NULL)
					{
						IDirect3DTexture2* tx2 = NULL;

						tx6Tex->QueryInterface(IID_IDirect3DTexture2, (void**)&tx2);

						if (tx2 != NULL)
						{
							tx2->Load(tx);
							tx2->Release();
						}

						tx->Release();
					}

					ds0->Release();
					ds0 = tx6Tex;
				}
			}

			ds0->QueryInterface(IID_IDirect3DTexture2, &item->Data1);
			ds0->Release();
		}
	}

// nastavení textury
	if (item->Data1 != NULL)
	{
		D3DX6Dev->SetTextureStageState(stage, D3DTSS_TEXCOORDINDEX, stage);
		D3DX6Dev->SetTexture(stage, (IDirect3DTexture2*)(item->Data1));
	}
}


/////////////////////////////////////////////////////////////////////////////
// vypnutí textury

void D3DX6TextureOff(int stage)
{
	D3DX6Dev->SetTexture(stage, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// resetování položky materiálu

void D3DX6MatReset(D3DMITEM* item)
{
	if (item->Data1 != NULL)
	{
		((IDirect3DMaterial3*)(item->Data1))->Release();
		item->Data1 = NULL;
		item->Data2 = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// resetování položky svìtla

void D3DX6LightReset(int inx, D3DLITEM* item)
{
	if (item->Data1 != NULL)
	{
		IDirect3DLight* light = (IDirect3DLight*)(item->Data1);

		if (item->Data2 != NULL)
		{
			D3DX6View->DeleteLight(light);
		}
			
		light->Release();
		item->Data1 = NULL;
		item->Data2 = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace ambient osvìtlení

void D3DX6AktAmbient()
{
	D3DX6Dev->SetLightState(D3DLIGHTSTATE_AMBIENT, D3DAmbientColRGB);
}

/////////////////////////////////////////////////////////////////////////////
// volná videopamì

int D3DX6Free()
{
	DDSCAPS2 cap;
	cap.dwCaps = DDSCAPS_TEXTURE;
	cap.dwCaps2 = 0;
	cap.dwCaps3 = 0;
	cap.dwCaps4 = 0;

	DWORD total = 0;
	DWORD free = 0;
	D3DX6DD->GetAvailableVidMem(&cap, &total, &free);

	return free;
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace mlhy

void D3DX6AktFog()
{
	D3DX6Dev->SetRenderState(D3DRENDERSTATE_FOGCOLOR, FogColorRGB);

	int n = FogType;
	if (n == 0) n = 3;

	D3DX6Dev->SetRenderState(D3DRENDERSTATE_FOGTABLEMODE, n);

	D3DX6Dev->SetRenderState(D3DRENDERSTATE_RANGEFOGENABLE, (BOOL)(FogKorig && IsRangeFog));

	float x = (float)FogStart;
	D3DX6Dev->SetRenderState(D3DRENDERSTATE_FOGTABLESTART, *((DWORD*)&x));

	x = (float)FogEnd;
	D3DX6Dev->SetRenderState(D3DRENDERSTATE_FOGTABLEEND, *((DWORD*)&x));

	x = (float)FogDens;
	D3DX6Dev->SetRenderState(D3DRENDERSTATE_FOGTABLEDENSITY, *((DWORD*)&x));
}

/////////////////////////////////////////////////////////////////////////////
// zapnutí mlhy

void D3DX6FogOn(BOOL on)
{
	if (!D3DVertFog)
	{
		D3DX6Dev->SetRenderState(D3DRENDERSTATE_FOGENABLE, on);
		if (!on) D3DX6Dev->SetRenderState(D3DRENDERSTATE_RANGEFOGENABLE, FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace stavu

void D3DX6AktState()
{
	D3DX6Dev->SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, TRUE);
	D3DX6Dev->SetRenderState(D3DRENDERSTATE_SPECULARENABLE, TRUE);
	D3DX6Dev->SetRenderState(D3DRENDERSTATE_DITHERENABLE, ((ScreenBits < 20) ? TRUE : FALSE));
	D3DX6Dev->SetRenderState(D3DRENDERSTATE_LASTPIXEL, TRUE);

	for (int i = D3DRENDERSTATE_WRAP0; i <= D3DRENDERSTATE_WRAP7; i++)
	{
		D3DX6Dev->SetRenderState((D3DRENDERSTATETYPE)i, 0);
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace blending operací objektu

void D3DX6AktBlend(D3DFITEM* item)
{
	if (IsBlending)
	{
		D3DX6Dev->SetRenderState(D3DRENDERSTATE_SRCBLEND, BlendTabD3D[item->SrcBlend]);
		D3DX6Dev->SetRenderState(D3DRENDERSTATE_DESTBLEND, BlendTabD3D[item->DstBlend]);
	}
	D3DX6Dev->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, (BOOL)(item->IsBlend && IsBlending));
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace referenèní úrovnì alfa

void D3DX6AktAlfa(D3DFITEM* item)
{
	BOOL on = (BOOL)(item->IsAlphaRef);
	D3DX6Dev->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, on);

	if (on)
	{
		D3DX6Dev->SetRenderState(D3DRENDERSTATE_ALPHAREF, item->AlphaRef2);

		if (AlphaGreaterEqual)
		{
			D3DX6Dev->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		}
		else
		{
			D3DX6Dev->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATER);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení operace barev

void D3DX6SetColorOp(int op, int arg1, int arg2, int stage)
{
	D3DX6Dev->SetTextureStageState(stage, D3DTSS_COLOROP, op);
	D3DX6Dev->SetTextureStageState(stage, D3DTSS_COLORARG1, arg1);
	D3DX6Dev->SetTextureStageState(stage, D3DTSS_COLORARG2, arg2);
}

/////////////////////////////////////////////////////////////////////////////
// nastavení operace alfa

void D3DX6SetAlphaOp(int op, int arg1, int arg2, int stage)
{
	D3DX6Dev->SetTextureStageState(stage, D3DTSS_ALPHAOP, op);
	D3DX6Dev->SetTextureStageState(stage, D3DTSS_ALPHAARG1, arg1);
	D3DX6Dev->SetTextureStageState(stage, D3DTSS_ALPHAARG2, arg2);
}

/////////////////////////////////////////////////////////////////////////////
// nastavení adresování textur

void D3DX6SetAddress(int adru, int adrv, int stage)
{
	D3DX6Dev->SetTextureStageState(stage, D3DTSS_ADDRESSU, adru);
	D3DX6Dev->SetTextureStageState(stage, D3DTSS_ADDRESSV, adrv);
}

/////////////////////////////////////////////////////////////////////////////
// nastavení barvy okolí textur

//void D3DX6SetBorder(D3DFITEM* item, int stage)
//{
//	D3DX6Dev->SetTextureStageState(stage, D3DTSS_BORDERCOLOR, item->BorderRGB[stage]);
//}

/////////////////////////////////////////////////////////////////////////////
// nastavení zjemnìní vzdálených textur

void D3DX6SetBias(float bias, int stage)
{
	D3DX6Dev->SetTextureStageState(stage, D3DTSS_MIPMAPLODBIAS, *(DWORD*)(&bias));
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace palet

void D3DX6AktPalette(HWND wnd)
{
	if (D3DX6Pal != NULL)
	{
		D3DX6Prim->SetPalette(D3DX6Pal);
		D3DX6Rend->SetPalette(D3DX6Pal);
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení barvy faktoru textur

void D3DX6SetTFactor(D3DFITEM* item)
{
	D3DX6Dev->SetRenderState(D3DRENDERSTATE_TEXTUREFACTOR, item->TFactorRGB);
}

/////////////////////////////////////////////////////////////////////////////
// nastavení Z funkce (1 až 8, v kódu DirectX, 0=vypnuto)

void D3DX6SetZFunc(int func)
{
	if (func == 0)
	{
		D3DX6Dev->SetRenderState(D3DRENDERSTATE_ZFUNC, D3DCMP_ALWAYS);
		D3DX6Dev->SetRenderState(D3DRENDERSTATE_ZENABLE, FALSE);
	}
	else
	{
		D3DX6Dev->SetRenderState(D3DRENDERSTATE_ZENABLE, TRUE);
		D3DX6Dev->SetRenderState(D3DRENDERSTATE_ZFUNC, func);
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení Z zápisu

void D3DX6SetZWrite(BOOL write)
{
	D3DX6Dev->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, write);
}

/////////////////////////////////////////////////////////////////////////////
// povolení stencil operací

void D3DX6SEnable(BOOL enable)
{
	D3DX6Dev->SetRenderState(D3DRENDERSTATE_STENCILENABLE, enable);
}

/////////////////////////////////////////////////////////////////////////////
// nastavení stencil operací

void D3DX6SetSOp(int fail, int zfail, int zpass)
{
	D3DX6Dev->SetRenderState(D3DRENDERSTATE_STENCILFAIL, fail);
	D3DX6Dev->SetRenderState(D3DRENDERSTATE_STENCILZFAIL, zfail);
	D3DX6Dev->SetRenderState(D3DRENDERSTATE_STENCILPASS, zpass);
}

/////////////////////////////////////////////////////////////////////////////
// nastavení stencil testu

void D3DX6SetSFunc(int func, int ref, DWORD mask)
{
	D3DX6Dev->SetRenderState(D3DRENDERSTATE_STENCILFUNC, func);
	D3DX6Dev->SetRenderState(D3DRENDERSTATE_STENCILREF, ref);
	D3DX6Dev->SetRenderState(D3DRENDERSTATE_STENCILMASK, mask);
}

/////////////////////////////////////////////////////////////////////////////
// nastavení zápisové masky do stencil bufferu

void D3DX6SetSMask(DWORD mask)
{
	D3DX6Dev->SetRenderState(D3DRENDERSTATE_STENCILWRITEMASK, mask);
}

/////////////////////////////////////////////////////////////////////////////
// vykreslení stínù

void D3DX6Shadows()
{
// vytvoøení bufferu stínù
	D3DX6InitShadow();

	if (D3DX6ShadowBuf != NULL)
	{
		D3DX6Dev->SetTexture(0, NULL);

		D3DX6Dev->DrawPrimitiveVB(
			D3DPT_TRIANGLESTRIP,
			D3DX6ShadowBuf,
			0,
			4,
			0);
	}
}


#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------
