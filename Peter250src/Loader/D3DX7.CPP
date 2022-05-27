
// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

#define	DIRECT3D_VERSION 0x0700

#include "Main.h"

/***************************************************************************\
*																			*
*							3D rozhraní - DirectX 7							*
*																			*
\***************************************************************************/

// DirectX 7 nepodporuje 256 barev, proto nejsou použity palety

/////////////////////////////////////////////////////////////////////////////
// deklarace

// funkce DirectDrawCreateEx
typedef	HRESULT (WINAPI *DIRECTDRAWCREATEEX) (GUID FAR *lpGUID, void** lplpDD, REFIID iid, IUnknown FAR *pUnkOuter);


/////////////////////////////////////////////////////////////////////////////
// data

// DirectDraw 7
bool					D3DX7DDTest			= false;	// probìhl test rozhraní DirectDraw 7
HINSTANCE				D3DX7DDLib			= NULL;		// handle modulu DDRAW.DLL
DIRECTDRAWCREATEEX		pDirectDrawCreateEx	= NULL;		// funkce DirectDrawCreateEx (NULL=není)
IDirectDraw7*			D3DX7DD				= NULL;		// objekt DirectDraw 7 (NULL=není)

// Direct3D	7
bool					D3DX7D3Test			= false;	// probìhl test rozhraní Direct3D 7
IDirect3D7*				D3DX7D3				= NULL;		// objekt Direct3D 7 (NULL=není)

// objekty
IDirectDrawSurface7*	D3DX7Prim			= NULL;		// primární povrch
IDirectDrawClipper*		D3DX7Clip			= NULL;		// clipper
IDirectDrawSurface7*	D3DX7Rend			= NULL;		// renderovací povrch
int						D3DX7RendWidth		= 0;		// aktuální šíøka renderovacího povrchu	
int						D3DX7RendHeight		= 0;		// aktuální výška renderovacího povrchu	
IDirectDrawSurface7*	D3DX7ZBuf			= NULL;		// Z buffer
IDirect3DDevice7*		D3DX7Dev			= NULL;		// renderovací zaøízení

//DDSURFACEDESC2			D3DX7ZBufDesc;					// popisovaè Z bufferu

BOOL					D3DX7VertCol		= -1;		// aktuální stav pøepínaèe barev z vrcholù (-1=neplatný)

// viewport
D3DVIEWPORT7 D3DX7View = {
	0,				// dwX
	0,				// dwY
	640,			// dwWidth
	480,			// dwHeight
	0,				// dvMinZ
	1				// dvMaxZ
};

// cílové souøadnice pro vykreslování v souøadnicích displeje
RECT					D3DX7Dest;						// cílová oblast pro vykreslování
D3DRECT					D3DX7Src;					// zdrojová oblast viewportu

// formát textur
//DDPIXELFORMAT			D3DX7TextFormat;			// použitý formát textur

IDirect3DVertexBuffer7*	D3DX7BackBuf = NULL;		// buffer pozadí scény

IDirect3DVertexBuffer7*	D3DX7ShadowBuf = NULL;		// buffer stínu

BOOL					D3DX7Clipping = -1;			// aktuální stav CLIPPING pøepínaèe

/////////////////////////////////////////////////////////////////////////////
// inicializace rozhraní DirectDraw 7 (vrací TRUE=rozhraní je vytvoøeno)

bool D3DX7InitDD()
{
// test, zda již byla provádìna inicializace
	if (!D3DX7DDTest)
	{
		D3DX7DDTest = true;

// inicializace funkce DirectDrawCreateEx (mìní pøesnost koprocesoru!)
		if (D3DX7DDLib == NULL)
		{
			D3DX7DDLib = ::LoadLibrary(_T("DDRAW.DLL"));
		}

		if (D3DX7DDLib != NULL)
		{
			pDirectDrawCreateEx = (DIRECTDRAWCREATEEX)::GetProcAddress(D3DX7DDLib, "DirectDrawCreateEx");
		}

// vytvoøení objektu DirectDraw 7
		if (pDirectDrawCreateEx != NULL)
		{
			if (pDirectDrawCreateEx(NULL, (void**)&D3DX7DD, IID_IDirectDraw7, NULL) != DD_OK)
			{
				D3DX7DD = NULL;
			}
			else
			{
				D3DX7DD->SetCooperativeLevel(MainFrame, DDSCL_NORMAL | DDSCL_FPUSETUP);
			}
		}
		FPUDouble();
	}

// test, zda je objekt DirectDraw 7 vytvoøen
	return (D3DX7DD != NULL);
}

/////////////////////////////////////////////////////////////////////////////
// ukonèení rozhraní DirectDraw 7

void D3DX7TermDD()
{
// zrušení rozhraní Direct3D 7
	D3DX7TermD3();

// zrušení Z bufferu
	D3DX7TermZBuf();

// zrušení renderovacího povrchu
	D3DX7TermRend();

// zrušení clipperu
	D3DX7TermClip();

// zrušení primárního povrchu
	D3DX7TermPrim();

// zrušení objektu DirectDraw 7
	if (D3DX7DD != NULL)
	{
		D3DX7DD->Release();
		D3DX7DD = NULL;
	}

// uvolnìní knihovny DirectDraw
	if (D3DX7DDLib != NULL)
	{
		::FreeLibrary(D3DX7DDLib);
		D3DX7DDLib = NULL;
	}
	pDirectDrawCreateEx = NULL;

// zrušení pøíznaku, že byla inicializace
	D3DX7DDTest = false;
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení povrchu DirectDrawSurface7 (vrací TRUE=OK)

bool D3DX7InitSurf(IDirectDrawSurface7** surf, DDSURFACEDESC2* ddsd)
{
	ddsd->dwSize = sizeof(DDSURFACEDESC2);

	if (!D3DX7InitDD() ||
		(D3DX7DD->CreateSurface(ddsd, surf, NULL) != DD_OK) ||
		(*surf == NULL))
	{
		*surf = NULL;
		return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení primárniho povrchu, není-li vytvoøen (vrací TRUE=OK)

bool D3DX7InitPrim()
{
	if (D3DX7Prim == NULL)
	{
		DDSURFACEDESC2 ddsd;
		MemFill(&ddsd, sizeof(ddsd), 0);
		ddsd.dwFlags = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

		if (!D3DX7InitSurf(&D3DX7Prim, &ddsd)) return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení primárního povrchu

void D3DX7TermPrim()
{
	if (D3DX7Prim != NULL)
	{
		D3DX7Prim->Release();
		D3DX7Prim = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení clipperu, není-li vytvoøen (vrací TRUE=OK)

bool D3DX7InitClip()
{
	if (D3DX7Clip == NULL)
	{
		if (!D3DX7InitPrim() ||
			(D3DX7DD->CreateClipper(0, &D3DX7Clip, NULL) != DD_OK) ||
			(D3DX7Clip == NULL) ||
			(D3DX7Clip->SetHWnd(0, MainFrame) != DD_OK) ||
			(D3DX7Prim->SetClipper(D3DX7Clip) != DD_OK))
		{
			D3DX7TermClip();
			return false;
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení clipperu (musí se volat pøed zrušením primárního povrchu)

void D3DX7TermClip()
{
	if (D3DX7Clip != NULL)
	{
		if (D3DX7Prim != NULL) D3DX7Prim->SetClipper(NULL);
		D3DX7Clip->Release();
		D3DX7Clip = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// inicializace rozhraní Direct3D 7 (vrací TRUE=rozhraní je vytvoøeno)

bool D3DX7InitD3()
{
// test, zda inicializace již probìhla
	if (!D3DX7D3Test)
	{
		D3DX7D3Test = true;

// inicializace DirectDraw 7
		if (D3DX7InitDD())
		{

// vytvoøení objektu Direct3D 7 (mìní pøesnost koprocesoru!)
			if (D3DX7DD->QueryInterface(IID_IDirect3D7, (void**)&D3DX7D3) != DD_OK)
			{
				D3DX7D3 = NULL;
			}
		}

		FPUDouble();
	}

// test, zda je objekt Direct3D 7 vytvoøen
	return (D3DX7D3 != NULL);
}

/////////////////////////////////////////////////////////////////////////////
// ukonèení rozhraní Direct3D 7

void D3DX7TermD3()
{
// ukonèení renderování
	D3DX7Stop();

// zrušení objektu Direct3D 7
	if (D3DX7D3 != NULL)
	{
		D3DX7D3->Release();
		D3DX7D3 = NULL;
	}

// zrušení pøíznaku, že byla inicializace
	D3DX7D3Test = false;
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení renderovacího povrchu, není-li vytvoøen (vrací TRUE=OK)

bool D3DX7InitRend(bool hw)
{
	if (D3DX7Rend == NULL)
	{

// pøíprava typu pamìti
		DWORD memtype = 0;
		if (hw) memtype = DDSCAPS_VIDEOMEMORY;

// pøíprava popisovaèe povrchu
		DDSURFACEDESC2 ddsd;
		MemFill(&ddsd, sizeof(ddsd), 0);
		ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
		ddsd.dwWidth = D3DWidth;
		D3DX7RendWidth = D3DWidth;
		ddsd.dwHeight = D3DHeight;
		D3DX7RendHeight = D3DHeight;
		ddsd.ddsCaps.dwCaps = DDSCAPS_3DDEVICE | DDSCAPS_OFFSCREENPLAIN | memtype;

// vytvoøení povrchu
		if (!D3DX7InitSurf(&D3DX7Rend, &ddsd))
		{
			return false;
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení renderovacího povrchu

void D3DX7TermRend()
{
	D3DX7TermDev();

	if (D3DX7Rend != NULL)
	{
		D3DX7Rend->Release();
		D3DX7Rend = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení Z-bufferu, není-li vytvoøen (vrací TRUE=OK)

bool D3DX7InitZBuf(bool hw, DDPIXELFORMAT* pf)
{
	if (D3DX7ZBuf == NULL)
	{

// kontrola renderovacího povrchu
		if ((D3DX7Rend == NULL) || 
			(D3DX7D3 == NULL) ||
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
		if (!D3DX7InitSurf(&D3DX7ZBuf, &sd))
		{
			return false;
		}

// pøipojení k renderovacímu povrchu
		if (D3DX7Rend->AddAttachedSurface(D3DX7ZBuf) != DD_OK)
		{
			D3DX7TermZBuf();
			return false;
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení Z bufferu

void D3DX7TermZBuf()
{
	if (D3DX7ZBuf != NULL)
	{
		D3DX7ZBuf->Release();

		if (D3DX7Rend != NULL)
		{
			D3DX7Rend->DeleteAttachedSurface(0, D3DX7ZBuf);
		}

		D3DX7ZBuf = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// zrušení bufferu stínu

void D3DX7TermShadow()
{
	if (D3DX7ShadowBuf != NULL)
	{
		D3DX7ShadowBuf->Release();
		D3DX7ShadowBuf = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// zrušení bufferu pozadí scény

void D3DX7TermBack()
{
	if (D3DX7BackBuf != NULL)
	{
		D3DX7BackBuf->Release();
		D3DX7BackBuf = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// výpoèet nových souøadnic zobrazené oblasti

void D3DX7CalcView()
{
	D3DX7Src.x2 = D3DWidth;
	D3DX7Src.y2 = D3DHeight;

	D3DX7View.dwWidth = D3DWidth;
	D3DX7View.dwHeight = D3DHeight;

	D3DX7Dest.left = D3DLeft;
	D3DX7Dest.top = D3DTop;
	::ClientToScreen(MainFrame, (POINT*)&D3DX7Dest.left);

	D3DX7Dest.right = D3DX7Dest.left + D3DWidth;
	D3DX7Dest.bottom = D3DX7Dest.top + D3DHeight;
}

/////////////////////////////////////////////////////////////////////////////
// inicializaèní nastavení viewportu (vrací TRUE=operace OK)

bool D3DX7SetView()
{		   
	D3DX7CalcView();
	if (D3DX7Dev == NULL) return false;
	return (D3DX7Dev->SetViewport(&D3DX7View) == D3D_OK);
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace viewportu po zmìnì velikosti

void D3DX7SizeView()
{
// pøegenerování bufferu pozadí scény
	D3DX7TermBack();

// pøegenerování bufferu stínù
	D3DX7TermShadow();

// pøepoèet videportu
	D3DX7CalcView();

// resetování bufferù 2D obrázkù
	D3DFITEM* item = D3DF_Get(1);
	int inx = item->Child;
	while (inx > 1)
	{
		item = D3DF_Get(inx);
		if ((item->Type == D3DFTYPE_PICTURE) || (item->Type == D3DFTYPE_LENSFLARE))
		{
			D3DX7Reset(item);
		}
		inx = item->Next;
	}

// zvìtšení bufferu
	if ((D3DHeight > D3DX7RendHeight) || (D3DWidth > D3DX7RendWidth))
	{
		FPUDouble();
		UserD3DDevice();
	}

// nastavení viewportu
	else
	{
		if (D3DX7Dev != NULL)
		{
			D3DX7Dev->SetViewport(&D3DX7View);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace viewportu po pøesunu

void D3DX7MoveView()
{
	D3DX7CalcView();
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení bufferu pozadí scény, není-li vytvoøen

struct BACKVERTEX
{
    D3DXVECTOR4 position;
	float tu, tv;
};

#define D3DFVF_BACKVERTEX (D3DFVF_XYZRHW|D3DFVF_TEX1)

void D3DX7InitBack()
{
	if (D3DX7BackBuf == NULL)
	{
		D3DVERTEXBUFFERDESC vb;
		vb.dwSize = sizeof(D3DVERTEXBUFFERDESC);
		vb.dwCaps = D3DVBCAPS_WRITEONLY | D3DVBCAPS_SYSTEMMEMORY;
		vb.dwFVF = D3DFVF_BACKVERTEX;
		vb.dwNumVertices = 4;

		D3DX7D3->CreateVertexBuffer(&vb, &D3DX7BackBuf, 0);

		if (D3DX7BackBuf != NULL)
		{
			BACKVERTEX* pVertices;
			if(D3DX7BackBuf->Lock(DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS, (void**)&pVertices, NULL) == D3D_OK)
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

				D3DX7BackBuf->Unlock();
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// callback enumerace Z bufferù

HRESULT WINAPI D3DX7EnumZBufferCallback(DDPIXELFORMAT* pddpf, void* user)
{
	D3DEnumZBufferFormat(pddpf);

    return D3DENUMRET_OK;
}

/////////////////////////////////////////////////////////////////////////////
// callback enumerace formátu textury

HRESULT CALLBACK D3DX7EnumPixelFormatsCallback(DDPIXELFORMAT* pf, void* user)
{
//	if (((pf->dwFlags == (DDPF_RGB | DDPF_ALPHAPIXELS)) && 
//			(pf->dwRGBBitCount == 32)))
//	{
//		MemCopy(&D3DX7TextFormat, pf, sizeof(DDPIXELFORMAT));
//	}

	D3DEnumPixelFormat(pf);

	return D3DENUMRET_OK;
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení bufferu stínu, není-li vytvoøen

struct SHADOWVERTEX
{
    D3DXVECTOR4 position;
	D3DCOLOR color;
};

#define D3DFVF_SHADOWVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

void D3DX7InitShadow()
{
	if (D3DX7ShadowBuf == NULL)
	{
		D3DVERTEXBUFFERDESC vb;
		vb.dwSize = sizeof(D3DVERTEXBUFFERDESC);
		vb.dwCaps = D3DVBCAPS_WRITEONLY | D3DVBCAPS_SYSTEMMEMORY;
		vb.dwFVF = D3DFVF_SHADOWVERTEX;
		vb.dwNumVertices = 4;

		D3DX7D3->CreateVertexBuffer(&vb, &D3DX7ShadowBuf, 0);

		if (D3DX7ShadowBuf != NULL)
		{
			SHADOWVERTEX* pVertices;
			if(D3DX7ShadowBuf->Lock(DDLOCK_WRITEONLY | DDLOCK_DISCARDCONTENTS, (void**)&pVertices, NULL) == D3D_OK)
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

				D3DX7ShadowBuf->Unlock();
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// inicializace zaøízení pro Z-buffer

bool D3DX7InitDev2(bool hw, int type, DDPIXELFORMAT* pf)
{
	if (D3DX7InitZBuf(hw, pf))
	{

// vytvoøení zaøízení (mìní pøesnost koprocesoru!)
		if ((D3DX7D3->CreateDevice(*Dev3DTab[type], D3DX7Rend, &D3DX7Dev) == DD_OK) &&
			(D3DX7Dev != NULL))
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

		D3DX7Dev = NULL;

// zrušení Z bufferu
		D3DX7TermZBuf();
	}
	return false;
}

bool D3DX7InitDev3(bool hw, int type)
{
	return (
		D3DX7InitDev2(hw, type, &D3DZBufferFormat24S8) ||
		D3DX7InitDev2(hw, type, &D3DZBufferFormat24S4) ||
		D3DX7InitDev2(hw, type, &D3DZBufferFormat15S1) ||
		D3DX7InitDev2(hw, type, &D3DZBufferFormat32S0) ||
		D3DX7InitDev2(hw, type, &D3DZBufferFormat24S0) ||
		D3DX7InitDev2(hw, type, &D3DZBufferFormat16S0) ||
		D3DX7InitDev2(hw, type, &D3DZBufferFormat8S0)
	);
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení renderovacího zaøízení (vrací TRUE=inicializace OK)
// typ zaøízení: 1=HAL,2=TnLHal,3=REF,4=RGB,5=MMX,6=Ramp,7=Null

bool D3DX7InitDev(int type)
{
// zrušení starých objektù
	D3DX7Stop();

// vytvoøení primárního povrchu, clipperu a palet
	if (D3DX7InitD3() &&
		D3DX7InitPrim() &&
		D3DX7InitClip())
	{

// enumerace Z bufferù
		if ((D3DX7D3->EnumZBufferFormats(*Dev3DTab[type],
				D3DX7EnumZBufferCallback, NULL) != D3D_OK) ||
				!D3DZBufferFormatOK)
		{
			D3DX7Stop();
			return false;
		}

// pøíznak HW ovladaèe
		bool hw = (type <= 2);

// vytvoøení renderovacího povrchu a Z bufferu
		if (!(D3DX7InitRend(hw) || D3DX7InitRend(!hw)) ||
			!(D3DX7InitDev3(hw, type) || D3DX7InitDev3(!hw, type)))
		{
			D3DX7Stop();
			return false;
		}
		
// enumerace formátù textur
		D3DX7Dev->EnumTextureFormats(D3DX7EnumPixelFormatsCallback, NULL);
		if (!D3DTextFormatOK)
		{
			D3DX7Stop();
			return false;
		}

		FPUDouble();
		return true;
	}

// pøi chybì zrušení vytvoøených objektù
	D3DX7Stop();

	return false;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení renderovacího zaøízení

void D3DX7TermDev()
{
	D3DX7TermBack();

	D3DX7TermShadow();

	if (D3DX7Dev != NULL)
	{
		D3DX7Dev->Release();
		D3DX7Dev = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// zahájení funkce zaøízení (vrací TRUE=inicializace OK)
// typ zaøízení: 1=HAL,2=TnLHal,3=REF,4=RGB,5=MMX,6=Ramp,7=Null

bool D3DX7Start(int type)
{
// vytvoøení renderovacího zaøízení
	if (!D3DX7InitDev(type)) return false;

// nastavení viewportu
	D3DX7Src.x1 = 0;
	D3DX7Src.y1 = 0;

	if (!D3DX7SetView())
	{
		D3DX7Stop();
		return false;
	}

// inicializace adres funkcí zaøízení
	pD3Stop = D3DX7Stop;			// ukonèení funkce zaøízení
	pD3SizeView = D3DX7SizeView;	// aktualizace viewportu po zmìnì velikosti
	pD3MoveView = D3DX7MoveView;	// aktualizace viewportu po pøesunu
	pD3Disp = D3DX7Disp;			// zobrazení renderovacího bufferu
	pD3Clear = D3DX7Clear;			// vymazání renderovací plochy
	pD3AktProj = D3DX7AktProj;		// aktualizace projekèní matice
	pD3AktView = D3DX7AktView;		// aktualizace pohledové matice
	pD3Begin = D3DX7Begin;			// zahájení renderování scény
	pD3End = D3DX7End;				// ukonèení renderování scény
	pD3Render = D3DX7Render;		// vyrenderování jednoho objektu
	pD3Reset = D3DX7Reset;			// resetování objektu pøed ukonèením ovladaèe
	pD3SReset = D3DX7SReset;		// resetování stínu objektu pøed ukonèením ovladaèe
	pD3AktWireframe = D3DX7AktWireframe;
	pD3AktLightOn = D3DX7AktLightOn;
	pD3AktShades = D3DX7AktShades;
	pD3AktCulling = D3DX7AktCulling;
	pD3AktMagFilter = D3DX7AktMagFilter;
	pD3AktMinMipFilter = D3DX7AktMinMipFilter;
	pD3AktLight = D3DX7AktLight;
	pD3SetMaterial = D3DX7SetMaterial;
	pD3TextureRes = D3DX7TextureRes;
	pD3TextureSet = D3DX7TextureSet;
	pD3TextureOn = D3DX7TextureOn;
	pD3TextureOff = D3DX7TextureOff;
	pD3AktAmbient = D3DX7AktAmbient;
	pD3Free = D3DX7Free;
	pD3AktFog = D3DX7AktFog;
	pD3FogOn = D3DX7FogOn;
	pD3AktState = D3DX7AktState;
	pD3AktBlend = D3DX7AktBlend;
	pD3AktAlfa = D3DX7AktAlfa;
	pD3SetColorOp = D3DX7SetColorOp;
	pD3SetAlphaOp = D3DX7SetAlphaOp;
	pD3SetAddress = D3DX7SetAddress;
//	pD3SetBorder = D3DX7SetBorder;
	pD3SetBias = D3DX7SetBias;
	pD3SetTFactor = D3DX7SetTFactor;
	pD3SetZFunc = D3DX7SetZFunc;
	pD3SetZWrite = D3DX7SetZWrite;
	pD3SEnable = D3DX7SEnable;
	pD3SetSOp = D3DX7SetSOp;
	pD3SetSFunc = D3DX7SetSFunc;
	pD3SetSMask = D3DX7SetSMask;
	pD3Shadow = D3DX7Shadow;
	pD3Shadows = D3DX7Shadows;

// inicializace informací o zaøízení
	D3DDEVICEDESC7 D3DCaps;

	D3DX7Dev->GetCaps(&D3DCaps);

	MaxTextureWidth = D3DCaps.dwMaxTextureWidth;
	MaxTextureHeight = D3DCaps.dwMaxTextureHeight;
	MaxTextureRepeat = D3DCaps.dwMaxTextureRepeat;

	SquareTexture = ((D3DCaps.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_SQUAREONLY) != 0);
	TexturePow2 = ((D3DCaps.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_POW2) != 0);
	IsBlending = ((D3DCaps.dpcTriCaps.dwDestBlendCaps & (D3DPBLENDCAPS_ONE | D3DPBLENDCAPS_INVSRCALPHA)) != 0);
//					(type >= 1) && (type <= 3));
	IsMagFilter = ((D3DCaps.dpcTriCaps.dwTextureFilterCaps & D3DPTFILTERCAPS_LINEAR) != 0);
//					(type >= 1) && (type <= 3));
	IsMinFilter = IsMagFilter;
	IsMipMap = ((D3DCaps.dpcTriCaps.dwTextureFilterCaps & 
					(D3DPTFILTERCAPS_MIPNEAREST | D3DPTFILTERCAPS_MIPLINEAR)) != 0);
//					(type >= 1) && (type <= 3));
	IsHWRaster = ((D3DCaps.dwDevCaps & D3DDEVCAPS_HWRASTERIZATION) != 0);
	IsHWTransLight = ((D3DCaps.dwDevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) != 0);
	AlphaGreaterEqual = ((D3DCaps.dpcTriCaps.dwAlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL) != 0);
	AlphaGreater = ((D3DCaps.dpcTriCaps.dwAlphaCmpCaps & D3DPCMPCAPS_GREATER) != 0);
	MaxTextureStages = D3DCaps.wMaxTextureBlendStages;
	MaxTextureSimult = D3DCaps.wMaxSimultaneousTextures;

	bool isfog = ((D3DCaps.dpcTriCaps.dwShadeCaps & D3DPSHADECAPS_FOGGOURAUD) != 0);
	IsTableFog = ((D3DCaps.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_FOGTABLE) != 0) && isfog;
	IsVertexFog = ((D3DCaps.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_FOGVERTEX) != 0) && isfog;
	IsRangeFog = ((D3DCaps.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_FOGRANGE) != 0) && isfog;

	D3DX7VertCol = -1;	// aktuální stav pøepínaèe barev z vrcholù (-1=neplatný)

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// ukonèení funkce zaøízení

void D3DX7Stop()
{
	D3DX7TermZBuf();		// zrušení Z bufferu
	D3DX7TermRend();		// zrušení renderovacího povrchu
	D3DX7TermClip();		// zrušení clipperu
	D3DX7TermPrim();		// zrušení primárního povrchu
	FPUDouble();			// návrat pøesnosti koprocesoru
}

/////////////////////////////////////////////////////////////////////////////
// vymazání renderovací plochy

void D3DX7Clear()
{
// test, zda je textura
	if (D3DBackText >= 0)
	{

// vytvoøení bufferu pozadí scény
		D3DX7InitBack();

		if (D3DX7BackBuf != NULL)
		{

// vypnutí barev vrcholù
			if (D3DX7VertCol != FALSE)
			{
				D3DX7VertCol = FALSE;
				D3DX7Dev->SetRenderState(D3DRENDERSTATE_COLORVERTEX, FALSE);
				D3DX7Dev->SetRenderState(D3DRENDERSTATE_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
				D3DX7Dev->SetRenderState(D3DRENDERSTATE_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
			}

// renderování
			D3DX7Dev->DrawPrimitiveVB(
				D3DPT_TRIANGLESTRIP,
				D3DX7BackBuf,
				0,
				4,
				0);
		}

// závìreèné vymazání Z bufferu
		D3DX7Dev->Clear(1, &D3DX7Src, D3DCLEAR_ZBUFFER | (D3DSBufferClear ? D3DCLEAR_STENCIL : 0), 0, 1, 0);
	}
	else
	{

// pozadí se neinicializuje
		if (D3DBackCol == (MAXDWORD-1))
		{
			if (D3DSBufferClear)
			{
				D3DX7Dev->Clear(1, &D3DX7Src,  D3DCLEAR_STENCIL, 0, 1, 0);
			}
			return;
		}

// není textura, je barva - vymazání barvy i Z bufferu
		if (D3DBackCol != MAXDWORD)
		{
			D3DX7Dev->Clear(1, &D3DX7Src, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | (D3DSBufferClear ? D3DCLEAR_STENCIL : 0), D3DBackColRGB, 1, 0);
		}
		else

// jinak pouze vymazání Z bufferu
		{
			D3DX7Dev->Clear(1, &D3DX7Src, D3DCLEAR_ZBUFFER | (D3DSBufferClear ? D3DCLEAR_STENCIL : 0), 0, 1, 0);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// zobrazení renderovacího bufferu

void D3DX7Disp()
{
	if ((D3DX7DD == NULL) || (D3DX7Prim == NULL)) return;
	if (D3DVSync) D3DX7DD->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, 0);
	if (D3DX7Prim->Blt(&D3DX7Dest, D3DX7Rend, (RECT*)&D3DX7Src, DDBLT_WAIT, NULL) == DDERR_SURFACELOST)
	{
		D3DX7Prim->Restore();
		D3DX7Rend->Restore();
		D3DX7ZBuf->Restore();
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace projekèní matice

void D3DX7AktProj()
{
	D3DX7Dev->SetTransform(D3DTRANSFORMSTATE_PROJECTION, &D3DProjMatrix);
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace pohledové matice

void D3DX7AktView()
{
	D3DX7Dev->SetTransform(D3DTRANSFORMSTATE_VIEW, &D3DViewMatrix);
}


/////////////////////////////////////////////////////////////////////////////
// zahájení renderování scény

void D3DX7Begin()
{
	D3DX7Dev->BeginScene();
}


/////////////////////////////////////////////////////////////////////////////
// ukonèení renderování scény

void D3DX7End()
{
	D3DX7Dev->EndScene();
}


/////////////////////////////////////////////////////////////////////////////
// vyrenderování jednoho objektu

void D3DX7Render(D3DFITEM* item)
{
// nastavení zdroje barev
	BOOL vertcol = ((item->VertCol != NULL) && ((item->MatSource == MATSOURCE_OBJECT) || !D3DLightOnAkt));

	if (vertcol != D3DX7VertCol)
	{
		D3DX7VertCol = vertcol;

		if (vertcol)
		{
			D3DX7Dev->SetRenderState(D3DRENDERSTATE_COLORVERTEX, TRUE);
			D3DX7Dev->SetRenderState(D3DRENDERSTATE_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
			D3DX7Dev->SetRenderState(D3DRENDERSTATE_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);
		}
		else
		{
			D3DX7Dev->SetRenderState(D3DRENDERSTATE_COLORVERTEX, FALSE);
			D3DX7Dev->SetRenderState(D3DRENDERSTATE_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
			D3DX7Dev->SetRenderState(D3DRENDERSTATE_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
		}
	}

	bool isFlare = (item->Type == D3DFTYPE_LENSFLARE);
	bool isPic = ((item->Type == D3DFTYPE_PICTURE) || isFlare);

// pøegenerování bufferu pro LensFlare
	if (isFlare) D3DX7Reset(item);

// vytvoøení bufferu vektorù (Data1), není-li ještì vytvoøen
	if (item->Data1 == NULL)
	{
		bool isCol = (item->VertCol != NULL);

		D3DVERTEXBUFFERDESC vb;
		vb.dwSize = sizeof(D3DVERTEXBUFFERDESC);
		vb.dwCaps = D3DVBCAPS_WRITEONLY | D3DVBCAPS_SYSTEMMEMORY;
		vb.dwFVF = D3DFVF_XYZ|D3DFVF_NORMAL;
		vb.dwNumVertices = item->VertNum;
		if (isPic) vb.dwFVF = D3DFVF_XYZRHW;
		if (isCol) vb.dwFVF |= D3DFVF_DIFFUSE;

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

		IDirect3DVertexBuffer7* vertbuf = NULL;
		D3DX7D3->CreateVertexBuffer(&vb, &vertbuf, 0);
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

						((D3DVECTOR*)vert)->x = item->Normal[i].x;
						((D3DVECTOR*)vert)->y = item->Normal[i].y;
						((D3DVECTOR*)vert)->z = item->Normal[i].z;
						vert = ((D3DVECTOR*)vert) + 1;
					}

					if (isCol)
					{
						*((DWORD*)vert) = item->VertCol[i];
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
			item2->FaceW = item->FaceW;
		}
	}

// nastavení transformaèní matice
	if (!isPic)
	{
		D3DX7Dev->SetTransform(D3DTRANSFORMSTATE_WORLD, &item->MatrixWorld);
	}

// vyrenderování objektu
	IDirect3DVertexBuffer7* vertbuf = (IDirect3DVertexBuffer7*)(item->Data1);

	if (vertbuf != NULL)
	{
		BOOL clip = (BOOL)item->AktClip;
		if (clip != D3DX7Clipping)
		{
			D3DX7Clipping = clip;
			D3DX7Dev->SetRenderState(D3DRENDERSTATE_CLIPPING, clip);
		}

		D3DX7Dev->DrawIndexedPrimitiveVB(
			D3DPT_TRIANGLELIST,
			vertbuf,
			0,
			item->VertNum,
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

void D3DX7Shadow(D3DFITEM* item)
{
// pøíprava typu položky
	int itemtype;

	if (D3DSBufferFormatOK)
	{
		itemtype = (D3DFVF_XYZ);

		if (D3DX7VertCol)
		{
			D3DX7VertCol = FALSE;
			D3DX7Dev->SetRenderState(D3DRENDERSTATE_COLORVERTEX, FALSE);
			D3DX7Dev->SetRenderState(D3DRENDERSTATE_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
			D3DX7Dev->SetRenderState(D3DRENDERSTATE_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
		}
	}
	else
	{
		itemtype = (D3DFVF_XYZ  | D3DFVF_DIFFUSE);

		if (!D3DX7VertCol)
		{
			D3DX7VertCol = TRUE;
			D3DX7Dev->SetRenderState(D3DRENDERSTATE_COLORVERTEX, TRUE);
			D3DX7Dev->SetRenderState(D3DRENDERSTATE_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
			D3DX7Dev->SetRenderState(D3DRENDERSTATE_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);
		}
	}

// vytvoøení bufferu vektorù (Data3), není-li ještì vytvoøen
	if ((item->Data3 == NULL) && (item->ShadowFan0 > 0))
	{
		D3DVERTEXBUFFERDESC vb;
		vb.dwSize = sizeof(D3DVERTEXBUFFERDESC);
		vb.dwCaps = D3DVBCAPS_WRITEONLY | D3DVBCAPS_SYSTEMMEMORY;
		vb.dwFVF = itemtype;
		vb.dwNumVertices = item->ShadowFan0;

		IDirect3DVertexBuffer7* vertbuf = NULL;
		D3DX7D3->CreateVertexBuffer(&vb, &vertbuf, 0);
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
	D3DX7Dev->SetTransform(D3DTRANSFORMSTATE_WORLD, &item->MatrixWorld);

// vyrenderování objektu
	IDirect3DVertexBuffer7* vertbuf = (IDirect3DVertexBuffer7*)(item->Data3);

	if (vertbuf != NULL)
	{
		if (!D3DX7Clipping)
		{
			D3DX7Clipping = TRUE;
			D3DX7Dev->SetRenderState(D3DRENDERSTATE_CLIPPING, TRUE);
		}

		int v = 0;

		for (int i = 0; i < item->ShadowFanN; i++)
		{
			int vn = item->ShadowFan[i];

			D3DX7Dev->DrawPrimitiveVB(
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

void D3DX7SReset(D3DFITEM* item)
{
// resetování bufferu stínù
	if (item->Data3 != NULL)
	{
		((IDirect3DVertexBuffer7*)(item->Data3))->Release();
		item->Data3 = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// resetování objektu pøed ukonèením ovladaèe

void D3DX7Reset(D3DFITEM* item)
{
// resetování bufferu vrcholù
	if (item->Data1 != NULL)
	{
		((IDirect3DVertexBuffer7*)(item->Data1))->Release();

		D3DFITEM* item2 = item;

		do {
			item2->Data1 = NULL;

			item2 = D3DF_Get(item2->CloneNext);
		} while (item != item2);
	}

// resetování bufferu stínù
	D3DX7SReset(item);
}

/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè vyplòování ploch

void D3DX7AktWireframe()
{
	D3DX7Dev->SetRenderState(D3DRENDERSTATE_FILLMODE, (D3DWireframeAkt ? D3DFILL_SOLID : D3DFILL_WIREFRAME));
}


/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè osvìtlení

void D3DX7AktLightOn()
{
	D3DX7Dev->SetRenderState(D3DRENDERSTATE_LIGHTING, (D3DLightOnAkt ? TRUE : FALSE));
}


/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè plynulého stinování

void D3DX7AktShades()
{
	D3DX7Dev->SetRenderState(D3DRENDERSTATE_SHADEMODE, (D3DShadesAkt ? D3DSHADE_GOURAUD : D3DSHADE_FLAT));
}


/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè odstraòování ploch

void D3DX7AktCulling()
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

	D3DX7Dev->SetRenderState(D3DRENDERSTATE_CULLMODE, n);
}


/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè filtrace zvìtšených textur

void D3DX7AktMagFilter(int stage)
{
	D3DX7Dev->SetTextureStageState(stage, D3DTSS_MAGFILTER, (D3DMagFilterAkt[stage] ? D3DTFG_LINEAR : D3DTFG_POINT));
}


/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè filtrace zmenšených a vzdálených textur

void D3DX7AktMinMipFilter(int stage)
{
	DWORD min = D3DTFN_POINT;
	DWORD mip = D3DTFP_NONE;

	if (D3DMipFilterAkt[stage]) mip = D3DTFP_POINT;

	if (D3DMinFilterAkt[stage])
	{
		min = D3DTFN_LINEAR;
		if (D3DMipFilterAkt[stage]) mip = D3DTFP_LINEAR;
	}

	D3DX7Dev->SetTextureStageState(stage, D3DTSS_MINFILTER, min);
	D3DX7Dev->SetTextureStageState(stage, D3DTSS_MIPFILTER, mip);
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace položky svìtla

void D3DX7AktLight(int inx, D3DLITEM* item)
{
	if (item->Akt)
	{
		if (item->Enable)
		{
			D3DX7Dev->SetLight(inx, (D3DLIGHT7*)&(item->Light));
			D3DX7Dev->LightEnable(inx, TRUE);
		}
		else
		{
			D3DX7Dev->LightEnable(inx, FALSE);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení materiálu

void D3DX7SetMaterial(D3DMITEM* mat)
{
	D3DX7Dev->SetMaterial((D3DMATERIAL7*)(&mat->Material));
}


/////////////////////////////////////////////////////////////////////////////
// resetování položky textury pøed ukonèením ovladaèe

void D3DX7TextureRes(D3DTITEM* item)
{
	if (item->Data1 != NULL)
	{
		((IDirectDrawSurface7*)(item->Data1))->Release();
		item->Data1 = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení a zapnutí textury

int tx7Width;
int tx7Height;
int tx7Mip;

BYTE* tx7Data;
int tx7WidthMul;

IDirectDrawSurface7* tx7Tex;

BOOL _fastcall D3DX7TextCreate(DDPIXELFORMAT* pf, BYTE* data, int mul)
{
	if (pf->dwRGBBitCount == 0) return FALSE;

	tx7Data = data;
	tx7WidthMul = mul;

	tx7Tex = NULL;

	DDSURFACEDESC2 ddsd;
	MemFill(&ddsd, sizeof(ddsd), 0);
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwWidth = tx7Width;
	ddsd.dwHeight = tx7Height;
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE;
	ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
	MemCopy(&(ddsd.ddpfPixelFormat), pf, sizeof(DDPIXELFORMAT));

	if (tx7Mip > 1)
	{
		ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_MIPMAPCOUNT | DDSD_PIXELFORMAT;
		ddsd.dwMipMapCount = tx7Mip;
		ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP | DDSCAPS_COMPLEX;
	}

	if (!IsHWRaster)	// manažer v SW módu neobsluhuje správnì vyšší úrovnì mipmap
	{
		ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
		ddsd.ddsCaps.dwCaps2 = 0;
	}

	return ((D3DX7DD->CreateSurface(&ddsd, &tx7Tex, NULL) == DD_OK) &&
			(tx7Tex != NULL));
}

void D3DX7TextureSet(D3DTITEM* item, int stage)
{
// data obrázku
	PICTUREDATA* picdata = item->Pic.Data();

// vytvoøení textury
	if (item->Data1 == NULL)
	{
		tx7Width = picdata->TextWidth;
		tx7Height = picdata->TextHeight;

// poèet mipmap
		tx7Mip = 1;
		if (IsMipMap && item->Mips)
		{
			int n = tx7Width;
			if (n > tx7Height) n = tx7Height;

			while (n > 1)
			{
				n = (n >> 1);
				tx7Mip++;
			}
		}
		if (tx7Mip > picdata->TextMipMaps) tx7Mip = picdata->TextMipMaps;

		// omezení poètu úrovní - pøi vyšším stupni se prolínají ikony textury terénu
		int mipmaps = item->MipMaps;
		if (mipmaps > 0)
		{
			if (tx7Mip > mipmaps) tx7Mip = mipmaps;
		}

// vytvoøení povrchu
		if ((!picdata->TextTrans && (D3DX7TextCreate(&D3DTextFormatR5G6B5, picdata->TextDataR5G6B5, 2) ||
									D3DX7TextCreate(&D3DTextFormatA1R5G5B5, picdata->TextDataA1R5G5B5, 2))) ||
			((D3DLowText || (picdata->TextSmooth < 1.25)) && 
								(D3DX7TextCreate(&D3DTextFormatA4R4G4B4, picdata->TextDataA4R4G4B4, 2) ||
									D3DX7TextCreate(&D3DTextFormatA1R5G5B5, picdata->TextDataA1R5G5B5, 2))) ||
			D3DX7TextCreate(&D3DTextFormatA8R8G8B8, picdata->TextData, 4) ||
			D3DX7TextCreate(&D3DTextFormatA4R4G4B4, picdata->TextDataA4R4G4B4, 2) ||
			D3DX7TextCreate(&D3DTextFormatA1R5G5B5, picdata->TextDataA1R5G5B5, 2) ||
			D3DX7TextCreate(&D3DTextFormatR5G6B5, picdata->TextDataR5G6B5, 2) ||
			D3DX7TextCreate(&D3DTextFormatX1R5G5B5, picdata->TextDataA1R5G5B5, 2) ||
			D3DX7TextCreate(&D3DTextFormatR8G8B8, picdata->TextDataR8G8B8, 3) ||
			D3DX7TextCreate(&D3DTextFormatX8R8G8B8, picdata->TextData, 4) ||
			D3DX7TextCreate(&D3DTextFormatX4R4G4B4, picdata->TextDataA4R4G4B4, 2))
		{
			item->Data1 = tx7Tex;

			tx7Tex->AddRef();
			BYTE* src = tx7Data;
				
			DDSCAPS2 caps;
			MemFill(&caps, sizeof(DDSCAPS2), 0);
			caps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_MIPMAP;

			for (; tx7Mip > 0; tx7Mip--)
			{
				DDSURFACEDESC2 ddsd;
				MemFill(&ddsd, sizeof(ddsd), 0);
				ddsd.dwSize = sizeof(ddsd);

				if (tx7Tex->Lock(NULL, &ddsd, 0, NULL) == DD_OK)
				{
					int pitch = ddsd.lPitch;
					BYTE* dst = (BYTE*)ddsd.lpSurface;
					int widthbyte = tx7Width * tx7WidthMul;

					if (pitch >= widthbyte)
					{
						for (int i = tx7Height; i > 0; i--)
						{
							MemCopy(dst, src, widthbyte);
							dst += pitch;
							src += widthbyte;
						}
					}
					tx7Tex->Unlock(NULL);
				}

				tx7Width = (tx7Width >> 1);
				tx7Height = (tx7Height >> 1);

				IDirectDrawSurface7* dsnext = NULL;

				if (tx7Mip > 1) tx7Tex->GetAttachedSurface(&caps, &dsnext);
				tx7Tex->Release();
				tx7Tex = dsnext;
				if (tx7Tex == NULL) break;
			}
		}
	}

// nastavení textury
	if (item->Data1 != NULL)
	{
		D3DX7Dev->SetTextureStageState(stage, D3DTSS_TEXCOORDINDEX, stage);
		D3DX7Dev->SetTexture(stage, (IDirectDrawSurface7*)(item->Data1));
	}
}


/////////////////////////////////////////////////////////////////////////////
// vypnutí textury

void D3DX7TextureOff(int stage)
{
	D3DX7Dev->SetTexture(stage, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace ambient osvìtlení

void D3DX7AktAmbient()
{
	D3DX7Dev->SetRenderState(D3DRENDERSTATE_AMBIENT, D3DAmbientColRGB);
}

/////////////////////////////////////////////////////////////////////////////
// volná videopamì

int D3DX7Free()
{
	DDSCAPS2 cap;
	cap.dwCaps = DDSCAPS_TEXTURE;
	cap.dwCaps2 = 0;
	cap.dwCaps3 = 0;
	cap.dwCaps4 = 0;

	DWORD total = 0;
	DWORD free = 0;
	D3DX7DD->GetAvailableVidMem(&cap, &total, &free);

	return free;
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace mlhy

void D3DX7AktFog()
{
	D3DX7Dev->SetRenderState(D3DRENDERSTATE_FOGCOLOR, FogColorRGB);

	int n = FogType;
	if (n == 0) n = 3;

	bool vertexmode = (FogKorig || D3DVertFog || !IsTableFog);
	if (!IsVertexFog) vertexmode = false;

	if (vertexmode)
	{
		D3DX7Dev->SetRenderState(D3DRENDERSTATE_FOGTABLEMODE, D3DFOG_NONE);
		D3DX7Dev->SetRenderState(D3DRENDERSTATE_FOGVERTEXMODE, n);
	}
	else
	{
		D3DX7Dev->SetRenderState(D3DRENDERSTATE_FOGVERTEXMODE, D3DFOG_NONE);
		D3DX7Dev->SetRenderState(D3DRENDERSTATE_FOGTABLEMODE, n);
	}
	D3DX7Dev->SetRenderState(D3DRENDERSTATE_RANGEFOGENABLE, (BOOL)(FogKorig && IsRangeFog));

	float x = (float)FogStart;
	D3DX7Dev->SetRenderState(D3DRENDERSTATE_FOGSTART, *((DWORD*)&x));

	x = (float)FogEnd;
	D3DX7Dev->SetRenderState(D3DRENDERSTATE_FOGEND, *((DWORD*)&x));

	x = (float)FogDens;
	D3DX7Dev->SetRenderState(D3DRENDERSTATE_FOGDENSITY, *((DWORD*)&x));
}

/////////////////////////////////////////////////////////////////////////////
// zapnutí mlhy

void D3DX7FogOn(BOOL on)
{
	D3DX7Dev->SetRenderState(D3DRENDERSTATE_FOGENABLE, on);
	if (!on) D3DX7Dev->SetRenderState(D3DRENDERSTATE_RANGEFOGENABLE, FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace stavu

void D3DX7AktState()
{
	D3DX7Dev->SetRenderState(D3DRENDERSTATE_TEXTUREPERSPECTIVE, TRUE);
	D3DX7Dev->SetRenderState(D3DRENDERSTATE_NORMALIZENORMALS, TRUE);
	D3DX7Dev->SetRenderState(D3DRENDERSTATE_LOCALVIEWER, TRUE);
	D3DX7Dev->SetRenderState(D3DRENDERSTATE_SPECULARENABLE, TRUE);
	D3DX7Dev->SetRenderState(D3DRENDERSTATE_DITHERENABLE, ((ScreenBits < 20) ? TRUE : FALSE));
	D3DX7Dev->SetRenderState(D3DRENDERSTATE_LASTPIXEL, TRUE);

	for (int i = D3DRENDERSTATE_WRAP0; i <= D3DRENDERSTATE_WRAP7; i++)
	{
		D3DX7Dev->SetRenderState((D3DRENDERSTATETYPE)i, 0);
	}

	D3DX7Clipping = -1;
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace blending operací objektu

void D3DX7AktBlend(D3DFITEM* item)
{
	if (IsBlending)
	{
		D3DX7Dev->SetRenderState(D3DRENDERSTATE_SRCBLEND, BlendTabD3D[item->SrcBlend]);
		D3DX7Dev->SetRenderState(D3DRENDERSTATE_DESTBLEND, BlendTabD3D[item->DstBlend]);
	}
	D3DX7Dev->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, (BOOL)(item->IsBlend && IsBlending));
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace referenèní úrovnì alfa

void D3DX7AktAlfa(D3DFITEM* item)
{
	BOOL on = (BOOL)(item->IsAlphaRef);
	D3DX7Dev->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, on);

	if (on)
	{
		D3DX7Dev->SetRenderState(D3DRENDERSTATE_ALPHAREF, item->AlphaRef2);

		if (AlphaGreaterEqual)
		{
			D3DX7Dev->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		}
		else
		{
			D3DX7Dev->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATER);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení operace barev

void D3DX7SetColorOp(int op, int arg1, int arg2, int stage)
{
	D3DX7Dev->SetTextureStageState(stage, D3DTSS_COLOROP, op);
	D3DX7Dev->SetTextureStageState(stage, D3DTSS_COLORARG1, arg1);
	D3DX7Dev->SetTextureStageState(stage, D3DTSS_COLORARG2, arg2);
}

/////////////////////////////////////////////////////////////////////////////
// nastavení operace alfa

void D3DX7SetAlphaOp(int op, int arg1, int arg2, int stage)
{
	D3DX7Dev->SetTextureStageState(stage, D3DTSS_ALPHAOP, op);
	D3DX7Dev->SetTextureStageState(stage, D3DTSS_ALPHAARG1, arg1);
	D3DX7Dev->SetTextureStageState(stage, D3DTSS_ALPHAARG2, arg2);
}

/////////////////////////////////////////////////////////////////////////////
// nastavení adresování textur

void D3DX7SetAddress(int adru, int adrv, int stage)
{
	D3DX7Dev->SetTextureStageState(stage, D3DTSS_ADDRESSU, adru);
	D3DX7Dev->SetTextureStageState(stage, D3DTSS_ADDRESSV, adrv);
}

/////////////////////////////////////////////////////////////////////////////
// nastavení barvy okolí textur

//void D3DX7SetBorder(D3DFITEM* item, int stage)
//{
//	D3DX7Dev->SetTextureStageState(stage, D3DTSS_BORDERCOLOR, item->BorderRGB[stage]);
//}

/////////////////////////////////////////////////////////////////////////////
// nastavení zjemnìní vzdálených textur

void D3DX7SetBias(float bias, int stage)
{
	D3DX7Dev->SetTextureStageState(stage, D3DTSS_MIPMAPLODBIAS, *(DWORD*)(&bias));
}

/////////////////////////////////////////////////////////////////////////////
// nastavení barvy faktoru textur

void D3DX7SetTFactor(D3DFITEM* item)
{
	D3DX7Dev->SetRenderState(D3DRENDERSTATE_TEXTUREFACTOR, item->TFactorRGB);
}

/////////////////////////////////////////////////////////////////////////////
// nastavení Z funkce (1 až 8, v kódu DirectX, 0=vypnuto)

void D3DX7SetZFunc(int func)
{
	if (func == 0)
	{
		D3DX7Dev->SetRenderState(D3DRENDERSTATE_ZFUNC, D3DCMP_ALWAYS);
		D3DX7Dev->SetRenderState(D3DRENDERSTATE_ZENABLE, FALSE);
	}
	else
	{
		D3DX7Dev->SetRenderState(D3DRENDERSTATE_ZENABLE, TRUE);
		D3DX7Dev->SetRenderState(D3DRENDERSTATE_ZFUNC, func);
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení Z zápisu

void D3DX7SetZWrite(BOOL write)
{
	D3DX7Dev->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, write);
}

/////////////////////////////////////////////////////////////////////////////
// povolení stencil operací

void D3DX7SEnable(BOOL enable)
{
	D3DX7Dev->SetRenderState(D3DRENDERSTATE_STENCILENABLE, enable);
}

/////////////////////////////////////////////////////////////////////////////
// nastavení stencil operací

void D3DX7SetSOp(int fail, int zfail, int zpass)
{
	D3DX7Dev->SetRenderState(D3DRENDERSTATE_STENCILFAIL, fail);
	D3DX7Dev->SetRenderState(D3DRENDERSTATE_STENCILZFAIL, zfail);
	D3DX7Dev->SetRenderState(D3DRENDERSTATE_STENCILPASS, zpass);
}

/////////////////////////////////////////////////////////////////////////////
// nastavení stencil testu

void D3DX7SetSFunc(int func, int ref, DWORD mask)
{
	D3DX7Dev->SetRenderState(D3DRENDERSTATE_STENCILFUNC, func);
	D3DX7Dev->SetRenderState(D3DRENDERSTATE_STENCILREF, ref);
	D3DX7Dev->SetRenderState(D3DRENDERSTATE_STENCILMASK, mask);
}

/////////////////////////////////////////////////////////////////////////////
// nastavení zápisové masky do stencil bufferu

void D3DX7SetSMask(DWORD mask)
{
	D3DX7Dev->SetRenderState(D3DRENDERSTATE_STENCILWRITEMASK, mask);
}

/////////////////////////////////////////////////////////////////////////////
// vykreslení stínù

void D3DX7Shadows()
{
// vytvoøení bufferu stínù
	D3DX7InitShadow();

	if (D3DX7ShadowBuf != NULL)
	{

// zapnutí barev vrcholù
		if (!D3DX7VertCol)
		{
			D3DX7VertCol = TRUE;
			D3DX7Dev->SetRenderState(D3DRENDERSTATE_COLORVERTEX, TRUE);
			D3DX7Dev->SetRenderState(D3DRENDERSTATE_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
			D3DX7Dev->SetRenderState(D3DRENDERSTATE_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);
		}

		D3DX7Dev->SetTexture(0, NULL);

// renderování
		D3DX7Dev->DrawPrimitiveVB(
			D3DPT_TRIANGLESTRIP,
			D3DX7ShadowBuf,
			0,
			4,
			0);
	}
}


#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------
