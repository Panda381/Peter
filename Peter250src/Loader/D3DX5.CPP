
// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

#define	DIRECT3D_VERSION 0x0500

#include "Main.h"

/***************************************************************************\
*																			*
*							3D rozhraní - DirectX 5							*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// deklarace

// DirectDraw 3
extern	IDirectDraw*		D3DX3DD;		// objekt DirectDraw 3 (NULL=není)


/////////////////////////////////////////////////////////////////////////////
// data

// DirectDraw 5
bool				D3DX5DDTest			= false;	// probìhl test rozhraní DirectDraw 5
IDirectDraw2*		D3DX5DD				= NULL;		// objekt DirectDraw 5 (NULL=není)

// Direct3D	5
bool				D3DX5D3Test			= false;	// probìhl test rozhraní Direct3D 5
IDirect3D2*			D3DX5D3				= NULL;		// objekt Direct3D 5 (NULL=není)

// objekty
IDirectDrawSurface*		D3DX5Prim1		= NULL;		// primární povrch (verze 1)
IDirectDrawSurface3*	D3DX5Prim		= NULL;		// primární povrch
IDirectDrawClipper*		D3DX5Clip		= NULL;		// clipper
IDirectDrawPalette*		D3DX5Pal		= NULL;		// palety
IDirectDrawSurface*		D3DX5Rend1		= NULL;		// renderovací povrch (verze 1)
IDirectDrawSurface3*	D3DX5Rend		= NULL;		// renderovací povrch
int						D3DX5RendWidth	= 0;		// aktuální šíøka renderovacího povrchu
int						D3DX5RendHeight	= 0;		// aktuální výška renderovacího povrchu
IDirectDrawSurface*		D3DX5ZBuf1		= NULL;		// Z buffer (verze 1)
IDirectDrawSurface3*	D3DX5ZBuf		= NULL;		// Z buffer
IDirect3DDevice2*		D3DX5Dev		= NULL;		// renderovací zaøízení
IDirect3DViewport2*		D3DX5View		= NULL;		// viewport

// cílové souøadnice pro vykreslování v souøadnicích displeje
RECT					D3DX5Dest;					// cílová oblast pro vykreslování
D3DRECT					D3DX5Src;					// zdrojová oblast viewportu
D3DVIEWPORT2			D3DX5ViewBuf;				// buffer nastavení pro viewport

// pozadí scény
IDirect3DMaterial2*		D3DX5BackMat	= NULL;		// materiál pozadí scény
D3DMATERIAL				D3DX5BackMatBuf;			// buffer materiálu pozadí scény

// formát textur
//DDPIXELFORMAT			D3DX5TextFormat;			// použitý formát textur

/////////////////////////////////////////////////////////////////////////////
// inicializace rozhraní DirectDraw 5 (vrací TRUE=rozhraní je vytvoøeno)

bool D3DX5InitDD()
{
// test, zda již byla provádìna inicializace
	if (!D3DX5DDTest)
	{
		D3DX5DDTest = true;

// vytvoøení rozhraní DirectDraw 3
		if (D3DX3InitDD())
		{

// vytvoøení objektu DirectDraw 5
			if (D3DX3DD->QueryInterface(IID_IDirectDraw2, (void**)&D3DX5DD) != DD_OK)
			{
				D3DX5DD = NULL;
			}
		}
	}

// test, zda je objekt DirectDraw 5 vytvoøen
	return (D3DX5DD != NULL);
}

/////////////////////////////////////////////////////////////////////////////
// ukonèení rozhraní DirectDraw 5

void D3DX5TermDD()
{
// zrušení rozhraní Direct3D 5
	D3DX5TermD3();

// zrušení závislého rozhraní DirectDraw 6
	D3DX6TermDD();

// zrušení viewportu
	D3DX5TermView();

// zrušení Z bufferu
	D3DX5TermZBuf();

// zrušení renderovacího povrchu
	D3DX5TermRend();

// zrušení palet
	D3DX5TermPal();

// zrušení clipperu
	D3DX5TermClip();

// zrušení primárního povrchu
	D3DX5TermPrim();

// zrušení objektu DirectDraw 5
	if (D3DX5DD != NULL)
	{
		D3DX5DD->Release();
		D3DX5DD = NULL;
	}

// zrušení pøíznaku, že byla inicializace
	D3DX5DDTest = false;
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení povrchu DirectDrawSurface3 (vrací TRUE=OK)

bool D3DX5InitSurf(IDirectDrawSurface** surf1, IDirectDrawSurface3** surf, DDSURFACEDESC* ddsd)
{
	ddsd->dwSize = sizeof(DDSURFACEDESC);

	if (!D3DX5InitDD() ||
		(D3DX5DD->CreateSurface(ddsd, surf1, NULL) != DD_OK) ||
		(*surf1 == NULL))
	{
		*surf1 = NULL;
		return false;
	}

	if (((*surf1)->QueryInterface(IID_IDirectDrawSurface3, (void**)surf) != DD_OK) ||
		(*surf == NULL))
	{
		*surf = NULL;
		(*surf1)->Release();
		*surf1 = NULL;
		return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení primárniho povrchu, není-li vytvoøen (vrací TRUE=OK)

bool D3DX5InitPrim()
{
	if (D3DX5Prim == NULL)
	{
		DDSURFACEDESC ddsd;
		MemFill(&ddsd, sizeof(ddsd), 0);
		ddsd.dwFlags = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

		if (!D3DX5InitSurf(&D3DX5Prim1, &D3DX5Prim, &ddsd)) return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení primárního povrchu

void D3DX5TermPrim()
{
	if (D3DX5Prim != NULL)
	{
		D3DX5Prim->Release();
		D3DX5Prim = NULL;
	}

	if (D3DX5Prim1 != NULL)
	{
		D3DX5Prim1->Release();
		D3DX5Prim1 = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení clipperu, není-li vytvoøen (vrací TRUE=OK)

bool D3DX5InitClip()
{
	if (D3DX5Clip == NULL)
	{
		if (!D3DX5InitPrim() ||
			(D3DX5DD->CreateClipper(0, &D3DX5Clip, NULL) != DD_OK) ||
			(D3DX5Clip == NULL) ||
			(D3DX5Clip->SetHWnd(0, MainFrame) != DD_OK) ||
			(D3DX5Prim->SetClipper(D3DX5Clip) != DD_OK))
		{
			D3DX5TermClip();
			return false;
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení clipperu (musí se volat pøed zrušením primárního povrchu)

void D3DX5TermClip()
{
	if (D3DX5Clip != NULL)
	{
		if (D3DX5Prim != NULL) D3DX5Prim->SetClipper(NULL);
		D3DX5Clip->Release();
		D3DX5Clip = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení palet, nejsou-li vytvoøeny (vrací TRUE=OK)

bool D3DX5InitPal()
{
	if (D3DX5Pal == NULL)
	{
		
// test, zda je potøeba použít palety
		if (!D3DX5InitPrim()) return false;
		DDSURFACEDESC ddsd;
		MemFill(&ddsd, sizeof(ddsd), 0);
		ddsd.dwSize = sizeof(ddsd);
		if (D3DX5Prim->GetSurfaceDesc(&ddsd) != DD_OK) return false;

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
			if ((D3DX5DD->CreatePalette(DDPCAPS_8BIT, SysPal, &D3DX5Pal, NULL) != DD_OK) ||
				(D3DX5Pal == NULL))
			{
				D3DX5Pal = NULL;
				return false;
			}

// nastavení palet
			if (D3DX5Prim->SetPalette(D3DX5Pal) != DD_OK)
			{
				D3DX5TermPal();
				return false;
			}
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení palet (volat pøed zrušením primárního povrchu)

void D3DX5TermPal()
{
	if (D3DX5Pal != NULL)
	{
		D3DX5Pal->Release();
		D3DX5Pal = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// inicializace rozhraní Direct3D 5 (vrací TRUE=rozhraní je vytvoøeno)

bool D3DX5InitD3()
{
// test, zda inicializace již probìhla
	if (!D3DX5D3Test)
	{
		D3DX5D3Test = true;

// inicializace DirectDraw 5
		if (D3DX5InitDD())
		{

// vytvoøení objektu Direct3D 5 (mìní pøesnost koprocesoru!)
			if (D3DX5DD->QueryInterface(IID_IDirect3D2, (void**)&D3DX5D3) != DD_OK)
			{
				D3DX5D3 = NULL;
			}
		}

		FPUDouble();
	}

// test, zda je objekt Direct3D 5 vytvoøen
	return (D3DX5D3 != NULL);
}

/////////////////////////////////////////////////////////////////////////////
// ukonèení rozhraní Direct3D 5

void D3DX5TermD3()
{
// ukonèení renderování
	D3DX5Stop();

// zrušení objektu Direct3D 5
	if (D3DX5D3 != NULL)
	{
		D3DX5D3->Release();
		D3DX5D3 = NULL;
	}

// zrušení pøíznaku, že byla inicializace
	D3DX5D3Test = false;
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení renderovacího povrchu, není-li vytvoøen (vrací TRUE=OK)

bool D3DX5InitRend(bool hw)
{
	if (D3DX5Rend == NULL)
	{

// pøíprava typu pamìti
		DWORD memtype = 0;
		if (hw) memtype = DDSCAPS_VIDEOMEMORY;

// pøíprava popisovaèe povrchu
		DDSURFACEDESC ddsd;
		MemFill(&ddsd, sizeof(ddsd), 0);
		ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
		ddsd.dwWidth = D3DWidth;
		D3DX5RendWidth = D3DWidth;
		ddsd.dwHeight = D3DHeight;
		D3DX5RendHeight = D3DHeight;
		ddsd.ddsCaps.dwCaps = DDSCAPS_3DDEVICE | DDSCAPS_OFFSCREENPLAIN | memtype;

// vytvoøení povrchu
		if (!D3DX5InitSurf(&D3DX5Rend1, &D3DX5Rend, &ddsd))
		{
			return false;
		}

// pøipojení palet
		if ((D3DX5Pal != NULL) &&
			(D3DX5Rend->SetPalette(D3DX5Pal) != DD_OK))
		{
			D3DX5TermRend();
			return false;
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení renderovacího povrchu

void D3DX5TermRend()
{
	D3DX5TermDev();

	if (D3DX5Rend != NULL)
	{
		D3DX5Rend->Release();
		D3DX5Rend = NULL;
	}

	if (D3DX5Rend1 != NULL)
	{
		D3DX5Rend1->Release();
		D3DX5Rend1 = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení Z-bufferu, není-li vytvoøen (vrací TRUE=OK)

bool D3DX5InitZBuf(bool hw, int depth)
{
	if (D3DX5ZBuf == NULL)
	{

// kontrola renderovacího povrchu
		if (D3DX5Rend == NULL) return false;

// pøíprava typu pamìti
		DWORD memtype = DDSCAPS_SYSTEMMEMORY;
		if (hw) memtype = DDSCAPS_VIDEOMEMORY;

// pøíprava popisovaèe povrchu
		DDSURFACEDESC ddsd;
		MemFill(&ddsd, sizeof(ddsd), 0);

		ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_ZBUFFERBITDEPTH;
		ddsd.dwWidth = D3DWidth;
		ddsd.dwHeight = D3DHeight;
		ddsd.dwZBufferBitDepth = depth;
		ddsd.ddsCaps.dwCaps = DDSCAPS_ZBUFFER | memtype;

// vytvoøení povrchu
		if (!D3DX5InitSurf(&D3DX5ZBuf1, &D3DX5ZBuf, &ddsd))
		{
			return false;
		}

// pøipojení k renderovacímu povrchu
		if (D3DX5Rend->AddAttachedSurface(D3DX5ZBuf) != DD_OK)
		{
			D3DX5TermZBuf();
			return false;
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení Z bufferu

void D3DX5TermZBuf()
{
	if (D3DX5ZBuf != NULL)
	{
		D3DX5ZBuf->Release();

		if (D3DX5Rend != NULL)
		{
			D3DX5Rend->DeleteAttachedSurface(0, D3DX5ZBuf);
		}

		D3DX5ZBuf = NULL;
	}

	if (D3DX5ZBuf1 != NULL)
	{
		D3DX5ZBuf1->Release();
		D3DX5ZBuf1 = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// callback enumerace formátu textury

HRESULT CALLBACK D3DX5EnumPixelFormatsCallback(DDSURFACEDESC* sd, void* user)
{
//	if (((sd->ddpfPixelFormat.dwFlags == (DDPF_RGB | DDPF_ALPHAPIXELS)) && 
//			(sd->ddpfPixelFormat.dwRGBBitCount == 32)))
//	{
//		MemCopy(&D3DX5TextFormat, &(sd->ddpfPixelFormat), sizeof(DDPIXELFORMAT));
//	}

	D3DEnumPixelFormat(&(sd->ddpfPixelFormat));

	return D3DENUMRET_OK;
}

/////////////////////////////////////////////////////////////////////////////
// inicializace zaøízení pro Z-buffer

bool _fastcall D3DX5InitDev2(bool hw, int type, int depth)
{
	if (D3DX5InitZBuf(hw, depth))
	{

// vytvoøení zaøízení (mìní pøesnost koprocesoru!)
		if ((D3DX5D3->CreateDevice(*Dev3DTab[type], D3DX5Rend1, &D3DX5Dev) == DD_OK) &&
			(D3DX5Dev != NULL))
		{
			return true;
		}

		D3DX5Dev = NULL;

// zrušení Z bufferu
		D3DX5TermZBuf();
	}
	return false;
}

bool D3DX5InitDev3(bool hw, int type)
{
	return (
		D3DX5InitDev2(hw, type, 16) ||
		D3DX5InitDev2(hw, type, 24) ||
		D3DX5InitDev2(hw, type, 32) ||
		D3DX5InitDev2(hw, type, 8)
	);
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení renderovacího zaøízení (vrací TRUE=inicializace OK)
// typ zaøízení: 1=HAL,2=TnLHal,3=REF,4=RGB,5=MMX,6=Ramp,7=Null

bool D3DX5InitDev(int type)
{
// zrušení starých objektù
	D3DX5Stop();

// vytvoøení primárního povrchu, clipperu a palet
	if (D3DX5InitD3() &&
		D3DX5InitPrim() &&
		D3DX5InitClip() &&
		D3DX5InitPal())
	{

// pøíznak HW ovladaèe
		bool hw = (type <= 2);

// vytvoøení renderovacího povrchu a Z bufferu
		if (!(D3DX5InitRend(hw) || D3DX5InitRend(!hw)) ||
			!(D3DX5InitDev3(hw, type) || D3DX5InitDev3(!hw, type)))
		{
			D3DX5Stop();
			return false;
		}
		
// enumerace formátù textur
		D3DX5Dev->EnumTextureFormats(D3DX5EnumPixelFormatsCallback, NULL);
		if (!D3DRamp && !D3DTextFormatOK)
		{
			D3DX5Stop();
			return false;
		}

		FPUDouble();
		return true;
	}

// pøi chybì zrušení vytvoøených objektù
	D3DX5Stop();

	return false;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení renderovacího zaøízení

void D3DX5TermDev()
{
	if (D3DX5Dev != NULL)
	{
		D3DX5Dev->Release();
		D3DX5Dev = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace materiálu pozadí scény

void D3DX5AktBackMat()
{
	if (D3DBackColAkt)
	{
		D3DBackColAkt = false;
		D3DX5BackMatBuf.diffuse.r = D3DBackColF.r;
		D3DX5BackMatBuf.diffuse.g = D3DBackColF.g;
		D3DX5BackMatBuf.diffuse.b = D3DBackColF.b;

		D3DX5BackMatBuf.ambient.r = D3DBackColF.r;
		D3DX5BackMatBuf.ambient.g = D3DBackColF.g;
		D3DX5BackMatBuf.ambient.b = D3DBackColF.b;

		D3DX5BackMat->SetMaterial(&D3DX5BackMatBuf);
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení materiálu pozadí scény

bool D3DX5InitBack()
{
	if (D3DX5BackMat == NULL)
	{

// musí existovat 3D objekt
		if (D3DX5D3 == NULL) return false;

// vytvoøení materiálu pozadí scény
		if ((D3DX5D3->CreateMaterial(&D3DX5BackMat, NULL) != D3D_OK) ||
			(D3DX5BackMat == NULL)) return false;

// inicializace materiálu
		MemFill(&D3DX5BackMatBuf, sizeof(D3DMATERIAL), 0);
		D3DX5BackMatBuf.dwSize = sizeof(D3DMATERIAL);
		D3DX5BackMatBuf.diffuse.a = 1;
		D3DX5BackMatBuf.ambient.a = 1;
		D3DX5BackMatBuf.specular.a = 1;
		D3DX5BackMatBuf.emissive.a = 1;
		D3DX5BackMatBuf.power = 0;
		D3DX5BackMatBuf.dwRampSize = 1;
		D3DBackColAkt = true;
		D3DX5AktBackMat();
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení materiálu pozadí scény (zøejmì až po zrušení viewportu)

void D3DX5TermBack()
{
	if (D3DX5BackMat != NULL)
	{
		D3DX5BackMat->Release();
		D3DX5BackMat = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// výpoèet nových souøadnic zobrazené oblasti

void D3DX5CalcView()
{
	D3DX5Src.x2 = D3DWidth;
	D3DX5Src.y2 = D3DHeight;

	D3DX5Dest.left = D3DLeft;
	D3DX5Dest.top = D3DTop;
	::ClientToScreen(MainFrame, (POINT*)&D3DX5Dest.left);

	D3DX5Dest.right = D3DX5Dest.left + D3DWidth;
	D3DX5Dest.bottom = D3DX5Dest.top + D3DHeight;
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace viewportu po zmìnì velikosti

void D3DX5SizeView()
{
// pøepoèet videportu
	D3DX5CalcView();
	D3DX5ViewBuf.dwWidth = D3DWidth;
	D3DX5ViewBuf.dwHeight = D3DHeight;

// resetování bufferù 2D obrázkù
	D3DFITEM* item = D3DF_Get(1);
	int inx = item->Child;
	while (inx > 1)
	{
		item = D3DF_Get(inx);
		if ((item->Type == D3DFTYPE_PICTURE) || (item->Type == D3DFTYPE_LENSFLARE))
		{
			D3DX5Reset(item);
		}
		inx = item->Next;
	}

// zvìtšení bufferu
	if ((D3DHeight > D3DX5RendHeight) || (D3DWidth > D3DX5RendWidth))
	{
		FPUDouble();
		UserD3DDevice();
	}

// nastavení viewportu
	else
	{
		if (D3DX5Dev != NULL)
		{
			D3DX5View->SetViewport2(&D3DX5ViewBuf);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace viewportu po pøesunu

void D3DX5MoveView()
{
	D3DX5CalcView();
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení viewportu (vrací TRUE=inicializace OK)

bool D3DX5InitView()
{
	if (D3DX5View == NULL)
	{

// musí existovat 3D zaøízení a 3D objekt
		if (D3DX5Dev == NULL) return false;

// vytvoøení viewportu
		if ((D3DX5D3->CreateViewport(&D3DX5View, NULL) != D3D_OK) ||
			(D3DX5View == NULL)) return false;

		D3DMATERIALHANDLE mh = NULL;

// pøipojení viewportu k 3D zaøízení
		if ((D3DX5Dev->AddViewport(D3DX5View) != D3D_OK) ||
			(D3DX5Dev->SetCurrentViewport(D3DX5View) != D3D_OK) ||

// vytvoøení materiálu pozadí scény a pøipojení k viewportu
			!D3DX5InitBack() ||

// handle materiálu
			(D3DX5BackMat->GetHandle(D3DX5Dev, &mh) != D3D_OK) ||
			(mh == NULL) ||

// pøipojení materiálu k viewportu
			(D3DX5View->SetBackground(mh) != D3D_OK))
		{
			D3DX5TermView();
			return false;
		}		

// inicializace bufferu dat viewportu
		D3DX5Src.x1 = 0;
		D3DX5Src.y1 = 0;
		D3DX5ViewBuf.dwSize = sizeof(D3DX5ViewBuf);
		D3DX5ViewBuf.dwX = 0;
		D3DX5ViewBuf.dwY = 0;
		D3DX5ViewBuf.dvClipX = -1;
		D3DX5ViewBuf.dvClipWidth = 2;
		D3DX5ViewBuf.dvClipY = 1;
		D3DX5ViewBuf.dvClipHeight = 2;
		D3DX5ViewBuf.dvMinZ = 0;
		D3DX5ViewBuf.dvMaxZ = 1;

		D3DX5SizeView();
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení viewportu

void D3DX5TermView()
{
	if (D3DX5View != NULL)
	{
		D3DX5View->Release();
		D3DX5View = NULL;
	}

	D3DX5TermBack();		// zrušení materiálu pozadí scény
}

/////////////////////////////////////////////////////////////////////////////
// zahájení funkce zaøízení (vrací TRUE=inicializace OK)
// typ zaøízení: 1=HAL,2=TnLHal,3=REF,4=RGB,5=MMX,6=Ramp,7=Null

bool D3DX5Start(int type)
{
// vytvoøení renderovacího zaøízení
	if (!D3DX5InitDev(type)) return false;

// vytvoøení viewportu
	if (!D3DX5InitView())
	{
		D3DX5Stop();
		return false;
	}

// inicializace adres funkcí zaøízení
	pD3Stop = D3DX5Stop;			// ukonèení funkce zaøízení
	pD3SizeView = D3DX5SizeView;	// aktualizace viewportu po zmìnì velikosti
	pD3MoveView = D3DX5MoveView;	// aktualizace viewportu po pøesunu
	pD3Disp = D3DX5Disp;			// zobrazení renderovacího bufferu
	pD3Clear = D3DX5Clear;			// vymazání renderovací plochy
	pD3AktProj = D3DX5AktProj;		// aktualizace projekèní matice
	pD3AktView = D3DX5AktView;		// aktualizace pohledové matice
	pD3Begin = D3DX5Begin;			// zahájení renderování scény
	pD3End = D3DX5End;				// ukonèení renderování scény
	pD3Render = D3DX5Render;		// vyrenderování jednoho objektu
	pD3Reset = D3DX5Reset;			// resetování objektu pøed ukonèením ovladaèe
	pD3SReset = D3DX5SReset;		// resetování stínu objektu pøed ukonèením ovladaèe
	pD3AktWireframe = D3DX5AktWireframe;
	pD3AktLightOn = D3DX5AktLightOn;
	pD3AktShades = D3DX5AktShades;
	pD3AktCulling = D3DX5AktCulling;
	pD3AktMagFilter = D3DX5AktMagFilter;
	pD3AktMinMipFilter = D3DX5AktMinMipFilter;
	pD3AktLight = D3DX5AktLight;
	pD3SetMaterial = D3DX5SetMaterial;
	pD3TextureRes = D3DX5TextureRes;
	pD3TextureSet = D3DX5TextureSet;
	pD3TextureOn = D3DX5TextureOn;
	pD3TextureOff = D3DX5TextureOff;
	pD3MatReset = D3DX5MatReset;
	pD3LightReset = D3DX5LightReset;
	pD3AktAmbient = D3DX5AktAmbient;
	pD3Free = D3DX5Free;
	pD3AktFog = D3DX5AktFog;
	pD3FogOn = D3DX5FogOn;
	pD3AktState = D3DX5AktState;
	pD3AktBlend = D3DX5AktBlend;
	pD3AktAlfa = D3DX5AktAlfa;
	pD3SetColorOp = D3DX5SetColorOp;
	pD3SetAddress = D3DX5SetAddress;
//	pD3SetBorder = D3DX5SetBorder;
	pD3SetBias = D3DX5SetBias;
	pD3AktPalette = D3DX5AktPalette;
	pD3SetZFunc = D3DX5SetZFunc;
	pD3SetZWrite = D3DX5SetZWrite;
	pD3Shadow = D3DX5Shadow;

// inicializace informací o zaøízení
	D3DDEVICEDESC D3DCapsHW;
	D3DDEVICEDESC D3DCapsSW;
	MemFill(&D3DCapsHW, sizeof(D3DDEVICEDESC), 0);
	D3DCapsHW.dwSize = sizeof(D3DDEVICEDESC);
	MemFill(&D3DCapsSW, sizeof(D3DDEVICEDESC), 0);
	D3DCapsSW.dwSize = sizeof(D3DDEVICEDESC);

	D3DX5Dev->GetCaps(&D3DCapsHW, &D3DCapsSW);

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
	IsRangeFog = ((D3DCaps->dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_FOGRANGE) != 0) && isfog;

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// ukonèení funkce zaøízení

void D3DX5Stop()
{
	D3DX5TermView();		// zrušení viewportu
	D3DX5TermZBuf();		// zrušení Z bufferu
	D3DX5TermRend();		// zrušení renderovacího povrchu
	D3DX5TermPal();			// zrušení palet
	D3DX5TermClip();		// zrušení clipperu
	D3DX5TermPrim();		// zrušení primárního povrchu
	FPUDouble();			// návrat pøesnosti koprocesoru
}

/////////////////////////////////////////////////////////////////////////////
// vymazání renderovací plochy

void D3DX5Clear()
{
// vypnutí textury
	if ((D3DBackText < 0) && (D3DX5BackMatBuf.hTexture != NULL))
	{
		D3DX5BackMatBuf.hTexture = NULL;
		D3DBackColAkt = true;
	}

// aktualizace materiálu pozadí scény
	D3DX5AktBackMat();

// pozadí se neinicializuje
	if (D3DBackCol == (MAXDWORD-1)) return;

// vymazání barvy i Z bufferu
	if ((D3DBackCol != MAXDWORD) || (D3DBackText >= 0))
	{
		D3DX5View->Clear(1, &D3DX5Src, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER);
	}
	else

// jinak pouze vymazání Z bufferu
	{
		D3DX5View->Clear(1, &D3DX5Src, D3DCLEAR_ZBUFFER);
	}
}

/////////////////////////////////////////////////////////////////////////////
// zobrazení renderovacího bufferu

void D3DX5Disp()
{
	if ((D3DX5DD == NULL) || (D3DX5Prim == NULL)) return;
	if (D3DVSync) D3DX5DD->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, 0);
	if (D3DX5Prim->Blt(&D3DX5Dest, D3DX5Rend, (RECT*)&D3DX5Src, DDBLT_WAIT, NULL) == DDERR_SURFACELOST)
	{
		D3DX5Prim->Restore();
		D3DX5Rend->Restore();
		D3DX5ZBuf->Restore();
	}
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace projekèní matice

void D3DX5AktProj()
{
	D3DX5Dev->SetTransform(D3DTRANSFORMSTATE_PROJECTION, &D3DProjMatrix);
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace pohledové matice

void D3DX5AktView()
{
	D3DX5Dev->SetTransform(D3DTRANSFORMSTATE_VIEW, &D3DViewMatrix);
}


/////////////////////////////////////////////////////////////////////////////
// zahájení renderování scény

void D3DX5Begin()
{
	D3DX5Dev->BeginScene();
}


/////////////////////////////////////////////////////////////////////////////
// ukonèení renderování scény

void D3DX5End()
{
	D3DX5Dev->EndScene();
}


/////////////////////////////////////////////////////////////////////////////
// vyrenderování jednoho objektu

void D3DX5Render(D3DFITEM* item)
{
// zrušení bufferu, je-li zmìna osvìtlení
	if ((void*)D3DLightOnAkt != item->Data2)
	{
		D3DX5Reset(item);
		item->Data2 = (void*)D3DLightOnAkt;
	}

	bool isFlare = (item->Type == D3DFTYPE_LENSFLARE);
	bool isPic = ((item->Type == D3DFTYPE_PICTURE) || isFlare);

// pøegenerování bufferu pro LensFlare
	if (isFlare) D3DX5Reset(item);

// vytvoøení bufferu vektorù (Data1), není-li ještì vytvoøen
	if (item->Data1 == NULL)
	{
		int i = item->VertNum;
		D3DCOLOR col = XRGB(item->Diffuse);
		D3DVECTOR* v = item->Vertex;
		float* uv = item->TextUV[0];

		if (isPic)
		{
			D3DTLVERTEX* vert = (D3DTLVERTEX*)MemGet(i * sizeof(D3DTLVERTEX));
			item->Data1 = vert;

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

			for (; i > 0; i--)
			{
				double x = v->x*item->ScaleX;
				double y = v->y*item->ScaleY;
				vert->sx = (float)((x*item->CosZ - y*sinz1 + sx + 0.5)*D3DWidth-0.5);
				vert->sy = (float)((0.5 - (x*sinz2 + y*item->CosZ) - sy)*D3DHeight-0.5);
				vert->sz = 0;
				vert->rhw = 1;
				vert->color = col;
				vert->specular = 0;
				v++;
				vert->tu = *uv;
				uv++;
				vert->tv = *uv;
				uv++;

				vert++;
			}
		}
		else
		{
			bool isNorm = (item->Data2 != NULL);

			D3DVERTEX* vert = (D3DVERTEX*)MemGet(i * sizeof(D3DVERTEX));
			item->Data1 = vert;
			D3DVECTOR* n = item->Normal;

			for (; i > 0; i--)
			{
				vert->x = v->x;
				vert->y = v->y;
				vert->z = v->z;
				v++;

				if (isNorm)
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

				vert->tu = *uv;
				uv++;
				vert->tv = *uv;
				uv++;

				vert++;
			}
		}

		i = item->FaceNum * 3;
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
	D3DX5Dev->SetTransform(D3DTRANSFORMSTATE_WORLD, &item->MatrixWorld);

// vykreslení objektu
	D3DX5Dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, (isPic ? D3DVT_TLVERTEX : ((item->Data2 == NULL) ? D3DVT_LVERTEX : D3DVT_VERTEX)),
				item->Data1, item->VertNum, item->FaceW, item->FaceNum*3,
				D3DDP_DONOTUPDATEEXTENTS | D3DDP_OUTOFORDER | (item->AktClip ? 0 : D3DDP_DONOTCLIP));
}

/////////////////////////////////////////////////////////////////////////////
// vyrenderování stínu

void D3DX5Shadow(D3DFITEM* item)
{
	int i = item->ShadowFan0;

// vytvoøení bufferu vektorù (Data3), není-li ještì vytvoøen
	if ((item->Data3 == NULL) && (i > 0))
	{
		D3DLVERTEX* vertbuf = (D3DLVERTEX*)MemGet(i * sizeof(D3DLVERTEX));
		item->Data3 = vertbuf;

		D3DVECTOR* src = item->ShadowVolume;

		for (; i > 0; i--)
		{
			vertbuf->x = src->x;
			vertbuf->y = src->y;
			vertbuf->z = src->z;

			vertbuf->dwReserved = 0;
			vertbuf->color = 0xaa808080;
			vertbuf->specular = 0;

			vertbuf->tu = 0;
			vertbuf->tv = 0;

			vertbuf++;
			src++;
		}
	}

// nastavení transformaèní matice
	D3DX5Dev->SetTransform(D3DTRANSFORMSTATE_WORLD, &item->MatrixWorld);

// vyrenderování objektu
	D3DLVERTEX* vertbuf = (D3DLVERTEX*)(item->Data3);

	if (vertbuf != NULL)
	{
		int v = 0;

		for (int i = 0; i < item->ShadowFanN; i++)
		{
			int vn = item->ShadowFan[i];

			D3DX5Dev->DrawPrimitive(
				D3DPT_TRIANGLEFAN,
				D3DVT_LVERTEX,
				vertbuf + v,
				vn,
				0);

			v += vn;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// resetování stínu objektu pøed ukonèením ovladaèe

void D3DX5SReset(D3DFITEM* item)
{
	if (item->Data3 != NULL)
	{
		MemFree(item->Data3);
		item->Data3 = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// resetování objektu pøed ukonèením ovladaèe

void D3DX5Reset(D3DFITEM* item)
{
	if (item->Data1 != NULL)
	{
		MemFree(item->Data1);

		D3DFITEM* item2 = item;

		do {
			item2->Data1 = NULL;
			item2->Data2 = NULL;

			item2 = D3DF_Get(item2->CloneNext);
		} while (item != item2);
	}

	D3DX5SReset(item);
}

/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè vyplòování ploch

void D3DX5AktWireframe()
{
	D3DX5Dev->SetRenderState(D3DRENDERSTATE_FILLMODE, (D3DWireframeAkt ? D3DFILL_SOLID : D3DFILL_WIREFRAME));
}


/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè osvìtlení

void D3DX5AktLightOn() { }


/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè plynulého stinování

void D3DX5AktShades()
{
	D3DX5Dev->SetRenderState(D3DRENDERSTATE_SHADEMODE, (D3DShadesAkt ? D3DSHADE_GOURAUD : D3DSHADE_FLAT));
}


/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè odstraòování ploch

void D3DX5AktCulling()
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

	D3DX5Dev->SetRenderState(D3DRENDERSTATE_CULLMODE, n);
}


/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè filtrace zvìtšených textur

void D3DX5AktMagFilter(int stage)
{
	if (stage != 0) return;
	D3DX5Dev->SetRenderState(D3DRENDERSTATE_TEXTUREMAG, (D3DMagFilterAkt[0] ? D3DFILTER_LINEAR : D3DFILTER_NEAREST));
}


/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè filtrace zmenšených a vzdálených textur

void D3DX5AktMinMipFilter(int stage)
{
	if (stage != 0) return;

	int n;

	if (D3DMinFilterAkt[0])
	{
		n = D3DFILTER_LINEAR;

		if (D3DMipFilterAkt[0])
		{
			n = D3DFILTER_LINEARMIPLINEAR;
		}
	}
	else
	{
		n = D3DFILTER_NEAREST;

		if (D3DMipFilterAkt[0])
		{
			n = D3DFILTER_MIPNEAREST;
		}
	}

	D3DX5Dev->SetRenderState(D3DRENDERSTATE_TEXTUREMIN, n);
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace položky svìtla

void D3DX5AktLight(int inx, D3DLITEM* item)
{
	if (item->Akt)
	{
		if (item->Enable)
		{
			if (item->Data1 == NULL)
			{
				D3DX5D3->CreateLight((IDirect3DLight**)&(item->Data1), NULL);
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
				D3DX5View->AddLight((IDirect3DLight*)(item->Data1));
				item->Data2 = (void*)TRUE;
			}
		}
		else
		{
			if (item->Data2 != NULL)
			{
				D3DX5View->DeleteLight((IDirect3DLight*)(item->Data1));
				item->Data2 = NULL;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení materiálu

void D3DX5SetMaterial(D3DMITEM* item)
{
// vytvoøení objektu materiálu
	if (item->Data1 == NULL)
	{
		IDirect3DMaterial2* mat = NULL;
		D3DX5D3->CreateMaterial(&mat, NULL);
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
			mat->GetHandle(D3DX5Dev, (D3DMATERIALHANDLE*) &(item->Data2));
		}
	}

// nastavení materiálu
	D3DX5Dev->SetLightState(D3DLIGHTSTATE_MATERIAL, (DWORD)item->Data2);

// ruší aktivní texturu
	D3DT_Active[0] = -1;
}

/////////////////////////////////////////////////////////////////////////////
// resetování položky materiálu

void D3DX5MatReset(D3DMITEM* item)
{
	if (item->Data1 != NULL)
	{
		((IDirect3DMaterial2*)(item->Data1))->Release();
		item->Data1 = NULL;
		item->Data2 = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// resetování položky textury pøed ukonèením ovladaèe

void D3DX5TextureRes(D3DTITEM* item)
{
	if (item->Data1 != NULL)
	{
		if (D3DRamp)
		{
			((IDirect3DMaterial2*)(item->Data1))->Release();
		}
		else
		{
			((IDirect3DTexture2*)(item->Data1))->Release();
		}
		item->Data1 = NULL;
		item->Data2 = NULL;
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastavení a zapnutí textury

int tx5Width;
int tx5Height;
int tx5Mip;

BYTE* tx5Data;
int tx5WidthMul;

IDirectDrawSurface* tx5Tex;
DDPIXELFORMAT* tx5PF;

BOOL _fastcall D3DX5TextCreate(DDPIXELFORMAT* pf, BYTE* data, int mul)
{
	if (pf->dwRGBBitCount == 0) return FALSE;

	tx5Data = data;
	tx5WidthMul = mul;

	tx5Tex = NULL;
	tx5PF = pf;

	DDSURFACEDESC ddsd;
	MemFill(&ddsd, sizeof(ddsd), 0);
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwWidth = tx5Width;
	ddsd.dwHeight = tx5Height;
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_SYSTEMMEMORY;
	MemCopy(&(ddsd.ddpfPixelFormat), pf, sizeof(DDPIXELFORMAT));

	if (tx5Mip > 1)
	{
		ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_MIPMAPCOUNT | DDSD_PIXELFORMAT;
		ddsd.dwMipMapCount = tx5Mip;
		ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP | DDSCAPS_COMPLEX | DDSCAPS_SYSTEMMEMORY;
	}

	return ((D3DX5DD->CreateSurface(&ddsd, &tx5Tex, NULL) == DD_OK) &&
			(tx5Tex != NULL));
}

void D3DX5TextureSet(D3DTITEM* item, int stage)
{
	if (stage != 0) return;

// data obrázku
	PICTUREDATA* picdata = item->Pic.Data();

// vytvoøení textury
	if (item->Data1 == NULL)
	{

// pro Ramp se nastaví barva materiálu (z konce MipMap)
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

			IDirect3DMaterial2* mat = NULL;
			D3DX5D3->CreateMaterial(&mat, NULL);
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
				mat->GetHandle(D3DX5Dev, (D3DMATERIALHANDLE*) &(item->Data2));
			}
		}
		else
		{
			tx5Width = picdata->TextWidth;
			tx5Height = picdata->TextHeight;

// poèet mipmap
			tx5Mip = 1;
			if (IsMipMap && item->Mips)
			{
				int n = tx5Width;
				if (n > tx5Height) n = tx5Height;

				while (n > 1)
				{
					n = (n >> 1);
					tx5Mip++;
				}
			}
			if (tx5Mip > picdata->TextMipMaps) tx5Mip = picdata->TextMipMaps;

			// omezení poètu úrovní - pøi vyšším stupni se prolínají ikony textury terénu
			int mipmaps = item->MipMaps;
			if (mipmaps > 0)
			{
				if (tx5Mip > mipmaps) tx5Mip = mipmaps;
			}

			int mip0 = tx5Mip;

// vytvoøení povrchu v systémové pamìti
			if ((!picdata->TextTrans && (D3DX5TextCreate(&D3DTextFormatR5G6B5, picdata->TextDataR5G6B5, 2) ||
									D3DX5TextCreate(&D3DTextFormatA1R5G5B5, picdata->TextDataA1R5G5B5, 2))) ||
				D3DX5TextCreate(&D3DTextFormatA4R4G4B4, picdata->TextDataA4R4G4B4, 2) ||
				((D3DLowText || (picdata->TextSmooth < 1.25)) && D3DX5TextCreate(&D3DTextFormatA1R5G5B5, picdata->TextDataA1R5G5B5, 2)) ||
				D3DX5TextCreate(&D3DTextFormatA8R8G8B8, picdata->TextData, 4) ||
				D3DX5TextCreate(&D3DTextFormatA1R5G5B5, picdata->TextDataA1R5G5B5, 2) ||
				D3DX5TextCreate(&D3DTextFormatR5G6B5, picdata->TextDataR5G6B5, 2) ||
				D3DX5TextCreate(&D3DTextFormatX1R5G5B5, picdata->TextDataA1R5G5B5, 2) ||
				D3DX5TextCreate(&D3DTextFormatR8G8B8, picdata->TextDataR8G8B8, 3) ||
				D3DX5TextCreate(&D3DTextFormatX8R8G8B8, picdata->TextData, 4) ||
				D3DX5TextCreate(&D3DTextFormatX4R4G4B4, picdata->TextDataA4R4G4B4, 2))
			{
				IDirectDrawSurface* ds0 = tx5Tex;

				tx5Tex->AddRef();
				BYTE* src = tx5Data;
				
				DDSCAPS caps;
				MemFill(&caps, sizeof(DDSCAPS), 0);
				caps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;

				for (; tx5Mip > 0; tx5Mip--)
				{
					DDSURFACEDESC ddsd;
					MemFill(&ddsd, sizeof(ddsd), 0);
					ddsd.dwSize = sizeof(ddsd);

					if (tx5Tex->Lock(NULL, &ddsd, 0, NULL) == DD_OK)
					{
						int pitch = ddsd.lPitch;
						BYTE* dst = (BYTE*)ddsd.lpSurface;
						int widthbyte = tx5Width * tx5WidthMul;

						if (pitch >= widthbyte)
						{
							for (int i = tx5Height; i > 0; i--)
							{
								MemCopy(dst, src, widthbyte);
								dst += pitch;
								src += widthbyte;
							}
						}

						tx5Tex->Unlock(NULL);
					}

					tx5Width = (tx5Width >> 1);
					tx5Height = (tx5Height >> 1);

					IDirectDrawSurface* dsnext = NULL;

					if (tx5Mip > 1) tx5Tex->GetAttachedSurface(&caps, &dsnext);
					tx5Tex->Release();
					tx5Tex = dsnext;
					if (tx5Tex == NULL) break;
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
					MemCopy(&(ddsd.ddpfPixelFormat), tx5PF, sizeof(DDPIXELFORMAT));

					if (mip0 > 1)
					{
						ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_MIPMAPCOUNT | DDSD_PIXELFORMAT;
						ddsd.dwMipMapCount = mip0;
						ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP | DDSCAPS_COMPLEX | DDSCAPS_VIDEOMEMORY | DDSCAPS_ALLOCONLOAD;
					}

					if (D3DX5DD->CreateSurface(&ddsd, &tx5Tex, NULL) == DD_OK)
					{
						IDirect3DTexture2* tx = NULL;
						ds0->QueryInterface(IID_IDirect3DTexture2, (void**)&tx);

						if (tx != NULL)
						{
							IDirect3DTexture2* tx2 = NULL;

							tx5Tex->QueryInterface(IID_IDirect3DTexture2, (void**)&tx2);

							if (tx2 != NULL)
							{
								tx2->Load(tx);
								tx2->Release();
							}

							tx->Release();
						}

						ds0->Release();
						ds0 = tx5Tex;
					}
				}

				ds0->QueryInterface(IID_IDirect3DTexture2, &item->Data1);
				ds0->Release();
			}
		}
	}

// nastavení textury
	if (D3DRamp)
	{
		if (item->Data2 != NULL)
		{
			D3DX5Dev->SetLightState(D3DLIGHTSTATE_MATERIAL, (DWORD)item->Data2);
			D3DM_Akt = -1;
		}
	}
	else
	{
		if (item->Data1 != NULL) 
		{
			D3DTEXTUREHANDLE h;
			((IDirect3DTexture2*)(item->Data1))->GetHandle(D3DX5Dev, &h);

			D3DX5Dev->SetRenderState(D3DRENDERSTATE_TEXTUREHANDLE, h);

// aktualizace textury pozadí scény
			if ((D3DBackText >= 0) && (item == D3DT_Get(D3DBackText)))
			{
				if (h != D3DX5BackMatBuf.hTexture)
				{
					D3DX5BackMatBuf.hTexture = h;
					D3DBackColAkt = true;
				}
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// vypnutí textury

void D3DX5TextureOff(int stage)
{
	if (stage != 0) return;
	D3DX5Dev->SetRenderState(D3DRENDERSTATE_TEXTUREHANDLE, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// resetování položky svìtla

void D3DX5LightReset(int inx, D3DLITEM* item)
{
	if (item->Data1 != NULL)
	{
		IDirect3DLight* light = (IDirect3DLight*)(item->Data1);

		if (item->Data2 != NULL)
		{
			D3DX5View->DeleteLight(light);
		}
			
		light->Release();
		item->Data1 = NULL;
		item->Data2 = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace ambient osvìtlení

void D3DX5AktAmbient()
{
	D3DX5Dev->SetLightState(D3DLIGHTSTATE_AMBIENT, D3DAmbientColRGB);
}

/////////////////////////////////////////////////////////////////////////////
// volná videopamì

int D3DX5Free()
{
	DDSCAPS cap;
	cap.dwCaps = DDSCAPS_TEXTURE;

	DWORD total = 0;
	DWORD free = 0;
	D3DX5DD->GetAvailableVidMem(&cap, &total, &free);

	return free;
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace mlhy

void D3DX5AktFog()
{
	D3DX5Dev->SetRenderState(D3DRENDERSTATE_FOGCOLOR, FogColorRGB);

	int n = FogType;
	if (n == 0) n = 3;
	D3DX5Dev->SetRenderState(D3DRENDERSTATE_FOGTABLEMODE, n);

	D3DX5Dev->SetRenderState(D3DRENDERSTATE_RANGEFOGENABLE, (BOOL)(FogKorig && IsRangeFog));

	float x = (float)FogStart;
	D3DX5Dev->SetRenderState(D3DRENDERSTATE_FOGTABLESTART, *((DWORD*)&x));

	x = (float)FogEnd;
	D3DX5Dev->SetRenderState(D3DRENDERSTATE_FOGTABLEEND, *((DWORD*)&x));

	x = (float)FogDens;
	D3DX5Dev->SetRenderState(D3DRENDERSTATE_FOGTABLEDENSITY, *((DWORD*)&x));
}

/////////////////////////////////////////////////////////////////////////////
// zapnutí mlhy

void D3DX5FogOn(BOOL on)
{
	if (!D3DVertFog)
	{
		D3DX5Dev->SetRenderState(D3DRENDERSTATE_FOGENABLE, on);
		if (!on) D3DX5Dev->SetRenderState(D3DRENDERSTATE_RANGEFOGENABLE, FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace stavu

void D3DX5AktState()
{
	D3DX5Dev->SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, TRUE);
	D3DX5Dev->SetRenderState(D3DRENDERSTATE_SPECULARENABLE, TRUE);
	D3DX5Dev->SetRenderState(D3DRENDERSTATE_DITHERENABLE, ((ScreenBits < 20) ? TRUE : FALSE));
	D3DX5Dev->SetRenderState(D3DRENDERSTATE_WRAPU, FALSE);
	D3DX5Dev->SetRenderState(D3DRENDERSTATE_WRAPV, FALSE);
	D3DX5Dev->SetRenderState(D3DRENDERSTATE_LASTPIXEL, TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace blending operací objektu

void D3DX5AktBlend(D3DFITEM* item)
{
	if (D3DRamp || !IsBlending)
	{
		D3DX5Dev->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
	}
	else
	{
		D3DX5Dev->SetRenderState(D3DRENDERSTATE_SRCBLEND, BlendTabD3D[item->SrcBlend]);
		D3DX5Dev->SetRenderState(D3DRENDERSTATE_DESTBLEND, BlendTabD3D[item->DstBlend]);
		D3DX5Dev->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, (BOOL)(item->IsBlend));
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace referenèní úrovnì alfa

void D3DX5AktAlfa(D3DFITEM* item)
{
	BOOL on = (BOOL)(item->IsAlphaRef && !D3DRamp);
	D3DX5Dev->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, on);

	if (on)
	{
		D3DX5Dev->SetRenderState(D3DRENDERSTATE_ALPHAREF, item->AlphaRef2);

		if (AlphaGreaterEqual)
		{
			D3DX5Dev->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		}
		else
		{
			D3DX5Dev->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATER);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení operace barev

void D3DX5SetColorOp(int op, int arg1, int arg2, int stage)
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
		n = D3DTBLEND_ADD;
		break;

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
		D3DX5Dev->SetRenderState(D3DRENDERSTATE_TEXTUREMAPBLEND, n);
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace zalamování textur

void D3DX5SetAddress(int adru, int adrv, int stage)
{
	if (stage != 0) return;
	D3DX5Dev->SetRenderState(D3DRENDERSTATE_TEXTUREADDRESSU, adru);
	D3DX5Dev->SetRenderState(D3DRENDERSTATE_TEXTUREADDRESSV, adrv);
}

/////////////////////////////////////////////////////////////////////////////
// nastavení zjemnìní vzdálených textur

void D3DX5SetBias(float bias, int stage)
{
	if (stage != 0) return;
	D3DX5Dev->SetRenderState(D3DRENDERSTATE_MIPMAPLODBIAS, *(DWORD*)(&bias));
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace palet

void D3DX5AktPalette(HWND wnd)
{
	if (D3DX5Pal != NULL)
	{
		D3DX5Prim->SetPalette(D3DX5Pal);
		D3DX5Rend->SetPalette(D3DX5Pal);
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastavení Z funkce (1 až 8, v kódu DirectX, 0=vypnuto)

void D3DX5SetZFunc(int func)
{
	if (func == 0)
	{
		D3DX5Dev->SetRenderState(D3DRENDERSTATE_ZFUNC, D3DCMP_ALWAYS);
		D3DX5Dev->SetRenderState(D3DRENDERSTATE_ZENABLE, FALSE);
	}
	else
	{
		D3DX5Dev->SetRenderState(D3DRENDERSTATE_ZENABLE, TRUE);
		D3DX5Dev->SetRenderState(D3DRENDERSTATE_ZFUNC, func);
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení Z zápisu

void D3DX5SetZWrite(BOOL write)
{
	D3DX5Dev->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, write);
}

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------
