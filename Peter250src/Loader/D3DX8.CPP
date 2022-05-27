
// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

#define	DIRECT3D_VERSION 0x0800

#include "Main.h"

/***************************************************************************\
*																			*
*							3D rozhraní - DirectX 8							*
*																			*
\***************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// deklarace

// funkce Direct3DCreate8
typedef	IDirect3D8* (WINAPI *DIRECT3DCREATE8) (UINT SDKVersion);

/////////////////////////////////////////////////////////////////////////////
// data

// Direct3D 8
bool				D3DX8D3Test			= false;	// probìhl test rozhraní Direct3D 8
HINSTANCE			D3DX8Lib			= NULL;		// handle modulu D3D8.DLL
DIRECT3DCREATE8		pDirect3DCreate8	= NULL;		// funkce Direct3DCreate8 (NULL=není)
IDirect3D8*			D3DX8D3				= NULL;		// objekt Direct3D 8 (NULL=není)

// zaøízení Direct3DDev 8
IDirect3DDevice8*	D3DX8Dev			= NULL;		// zaøízení Direct3DDev8
D3DPRESENT_PARAMETERS	D3DX8Par;					// parametry zaøízení
int						D3DX8RendWidth		= 0;	// aktuální šíøka renderovacího povrchu	
int						D3DX8RendHeight		= 0;	// aktuální výška renderovacího povrchu	

BOOL				D3DX8VertCol		= -1;		// aktuální stav pøepínaèe barev z vrcholù (-1=neplatný)

// tabulka typù zaøízení
int	D3DX8DevTab[D3DEVICES] =
{
	0,					// 0
	D3DDEVTYPE_HAL,		// 1 HAL
	0,					// 2 TnLHal
	D3DDEVTYPE_REF,		// 3 REF
	D3DDEVTYPE_SW,		// 4 RGB
	0,					// 5 MMX
	0,					// 6 Ramp
//	0,					// 7 Null
};

// viewport
D3DVIEWPORT8 D3DX8View = {
	0,				// X
	0,				// Y
	640,			// Width
	480,			// Height
	0,				// MinZ
	1				// MaxZ
};

// cílové souøadnice pro vykreslování v souøadnicích displeje
RECT					D3DX8Dest;						// cílová oblast pro vykreslování
D3DRECT					D3DX8Src;						// zdrojová oblast viewportu

IDirect3DVertexBuffer8*	D3DX8BackBuf = NULL;			// buffer pozadí scény

IDirect3DVertexBuffer8*	D3DX8ShadowBuf = NULL;			// buffer stínu

BOOL					D3DX8Clipping = -1;			// aktuální stav CLIPPING pøepínaèe

/////////////////////////////////////////////////////////////////////////////
// inicializace rozhraní Direct3D 8 (vrací TRUE=rozhraní je vytvoøeno)

bool D3DX8InitD3()
{
// test, zda inicializace již probìhla
	if (!D3DX8D3Test)
	{
		D3DX8D3Test = true;

// inicializace funkce Direct3DCreate8 (mìní pøesnost koprocesoru!)
		if (D3DX8Lib == NULL)
		{
			D3DX8Lib = ::LoadLibrary(_T("D3D8.DLL"));
			FPUDouble();
		}

		if (D3DX8Lib != NULL)
		{
			pDirect3DCreate8 = (DIRECT3DCREATE8)::GetProcAddress(D3DX8Lib, "Direct3DCreate8");
		}

// vytvoøení objektu Direct3D8
		if (pDirect3DCreate8 != NULL)
		{
			D3DX8D3 = pDirect3DCreate8(D3D_SDK_VERSION);
		}
	}

// test, zda je objekt Direct3D 8 vytvoøen
	return (D3DX8D3 != NULL);
}

/////////////////////////////////////////////////////////////////////////////
// ukonèení rozhraní Direct3D 8

void D3DX8TermD3()
{
// zrušení objektu Direct3D 8
	if (D3DX8D3 != NULL)
	{
		D3DX8D3->Release();
		D3DX8D3 = NULL;
	}
	
// uvolnìní knihovny D3D8.DLL
	if (D3DX8Lib != NULL)
	{
		::FreeLibrary(D3DX8Lib);
		D3DX8Lib = NULL;
	}
	pDirect3DCreate8 = NULL;

// zrušení pøíznaku, že byla inicializace
	D3DX8D3Test = false;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení bufferu stínu

void D3DX8TermShadow()
{
	if (D3DX8ShadowBuf != NULL)
	{
		D3DX8ShadowBuf->Release();
		D3DX8ShadowBuf = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// zrušení bufferu pozadí scény

void D3DX8TermBack()
{
	if (D3DX8BackBuf != NULL)
	{
		D3DX8BackBuf->Release();
		D3DX8BackBuf = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// výpoèet nových souøadnic zobrazené oblasti

void D3DX8CalcView()
{
	D3DX8Src.x2 = D3DWidth;
	D3DX8Src.y2 = D3DHeight;

	D3DX8View.Width = D3DWidth;
	D3DX8View.Height = D3DHeight;

	D3DX8Dest.left = D3DLeft;
	D3DX8Dest.top = D3DTop;
	D3DX8Dest.right = D3DLeft + D3DWidth;
	D3DX8Dest.bottom = D3DTop + D3DHeight;
}


/////////////////////////////////////////////////////////////////////////////
// nastavení viewportu (vrací TRUE=operace OK)

bool D3DX8SetView()
{
	D3DX8CalcView();
	if (D3DX8Dev == NULL) return false;
	return (D3DX8Dev->SetViewport(&D3DX8View) == D3D_OK);
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace viewportu po zmìnì velikosti

void D3DX8SizeView()
{
// pøegenerování bufferu pozadí scény
	D3DX8TermBack();

// pøegenerování bufferu stínù
	D3DX8TermShadow();

// pøepoèet videportu
	D3DX8CalcView();

// resetování bufferù 2D obrázkù
	D3DFITEM* item = D3DF_Get(1);
	int inx = item->Child;
	while (inx > 1)
	{
		item = D3DF_Get(inx);
		if ((item->Type == D3DFTYPE_PICTURE) || (item->Type == D3DFTYPE_LENSFLARE))
		{
			D3DX8Reset(item);
		}
		inx = item->Next;
	}

// zvìtšení bufferu
	if ((D3DHeight > D3DX8RendHeight) || (D3DWidth > D3DX8RendWidth))
	{
		FPUDouble();
		UserD3DDevice();
	}

// nastavení viewportu
	else
	{
		if (D3DX8Dev != NULL)
		{
			D3DX8Dev->SetViewport(&D3DX8View);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace viewportu po pøesunu

void D3DX8MoveView()
{
	D3DX8CalcView();
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení bufferu pozadí scény, není-li vytvoøen

struct BACKVERTEX
{
    D3DXVECTOR4 position;
	float tu, tv;
};

#define D3DFVF_BACKVERTEX (D3DFVF_XYZRHW|D3DFVF_TEX1)

void D3DX8InitBack()
{
	if (D3DX8BackBuf == NULL)
	{
		D3DX8Dev->CreateVertexBuffer(4*sizeof(BACKVERTEX),
							  D3DUSAGE_WRITEONLY, D3DFVF_BACKVERTEX,
							  D3DPOOL_MANAGED, &D3DX8BackBuf);

		if (D3DX8BackBuf != NULL)
		{
			BACKVERTEX* pVertices;
			if(D3DX8BackBuf->Lock( 0, 0, (BYTE**)&pVertices, 0 ) == D3D_OK)
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

				D3DX8BackBuf->Unlock();
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení bufferu stínu, není-li vytvoøen

struct SHADOWVERTEX
{
    D3DXVECTOR4 position;
	D3DCOLOR color;
};

#define D3DFVF_SHADOWVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

void D3DX8InitShadow()
{
	if (D3DX8ShadowBuf == NULL)
	{
		D3DX8Dev->CreateVertexBuffer(4*sizeof(SHADOWVERTEX),
							  D3DUSAGE_WRITEONLY, D3DFVF_SHADOWVERTEX,
							  D3DPOOL_MANAGED, &D3DX8ShadowBuf);

		if (D3DX8ShadowBuf != NULL)
		{
			SHADOWVERTEX* pVertices;
			if(D3DX8ShadowBuf->Lock( 0, 0, (BYTE**)&pVertices, 0 ) == D3D_OK)
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

				D3DX8ShadowBuf->Unlock();
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení renderovacího zaøízení (vrací TRUE=inicializace OK)
// typ zaøízení: 1=HAL,2=TnLHal,3=REF,4=RGB,5=MMX,6=Ramp,7=Null

D3DDISPLAYMODE D3DX8dm;		// aktuální mód displeje

bool D3DX8InitDev(D3DDEVTYPE devtype, int vertproc, D3DFORMAT form)
{
// pøíprava parametrù
	MemFill(&D3DX8Par, sizeof(D3DX8Par), 0);
	D3DX8Par.Windowed = TRUE;
	D3DX8Par.BackBufferWidth = D3DWidth;
	D3DX8RendWidth = D3DWidth;
	D3DX8Par.BackBufferHeight = D3DHeight;
	D3DX8RendHeight = D3DHeight;
	D3DX8Par.BackBufferCount = 1;
	D3DX8Par.SwapEffect = D3DSWAPEFFECT_COPY;
	D3DX8Par.hDeviceWindow = MainFrame;
	D3DX8Par.EnableAutoDepthStencil = TRUE;
	D3DX8Par.AutoDepthStencilFormat = form;
	D3DX8Par.BackBufferFormat = D3DX8dm.Format;

// vytvoøení zaøízení
	if ( D3DX8D3->CreateDevice(
				D3DADAPTER_DEFAULT,			// implicitní adaptér displeje
				devtype,					// typ zaøízení
				MainFrame,					// okno
				vertproc,					// zpracování vrcholù
				&D3DX8Par,					// parametry
				&D3DX8Dev) == D3D_OK)		// interface zaøízení
	{
		switch (form)
		{
		case D3DFMT_D24S8:
			D3DSBufferFormatOK = true;
			D3DSBufferDepth = 8;
			D3DSBufferMax = 0xff;
			break;

		case D3DFMT_D24X4S4:
			D3DSBufferFormatOK = true;
			D3DSBufferDepth = 4;
			D3DSBufferMax = 0xf;
			break;

		case D3DFMT_D15S1:
			D3DSBufferFormatOK = true;
			D3DSBufferDepth = 1;
			D3DSBufferMax = 1;
			break;
		}

		return true;
	}
	else
	{
		return false;
	}
}

bool D3DX8InitDev2(D3DDEVTYPE devtype, D3DFORMAT form)
{
// kontrola formátu hloubkového bufferu
	if (D3DX8D3->CheckDepthStencilMatch(
				D3DADAPTER_DEFAULT,			// implicitní adaptér displeje
				devtype,					// typ zaøízení
				D3DX8dm.Format,				// formát displeje
				D3DX8dm.Format,				// formát renderovacího povrchu
				form) != D3D_OK) return false;	// formát hloubkového bufferu

	return (D3DX8InitDev(devtype, D3DCREATE_SOFTWARE_VERTEXPROCESSING, form) ||
			D3DX8InitDev(devtype, D3DCREATE_HARDWARE_VERTEXPROCESSING, form));
}

/////////////////////////////////////////////////////////////////////////////
// zrušení renderovacího zaøízení

void D3DX8TermDev()
{
	D3DX8TermBack();

	D3DX8TermShadow();

	if (D3DX8Dev != NULL)
	{
		D3DX8Dev->Release();
		D3DX8Dev = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// zahájení funkce zaøízení (vrací TRUE=inicializace OK)
// typ zaøízení: 1=HAL,2=TnLHal,3=REF,4=RGB,5=MMX,6=Ramp,7=Null

bool D3DX8Start(int type)
{
// kontrola platnosti zaøízení
	if (D3DX8DevTab[type] == 0) return false;
	D3DDEVTYPE devtype = (D3DDEVTYPE)D3DX8DevTab[type];	// typ zaøízení

// zrušení starých objektù
	D3DX8Stop();

// inicializace rozhraní Direct3D
	if (!D3DX8InitD3()) return false;

// zjištìní aktuálního módu displeje
	D3DX8dm.Format = D3DFMT_R5G6B5;
	D3DX8D3->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &D3DX8dm);

// vytvoøení renderovacího zaøízení
	if (!(
		D3DX8InitDev2(devtype, D3DFMT_D24S8) ||
		D3DX8InitDev2(devtype, D3DFMT_D24X4S4) ||
		D3DX8InitDev2(devtype, D3DFMT_D15S1) ||
		D3DX8InitDev2(devtype, D3DFMT_D32) ||
		D3DX8InitDev2(devtype, D3DFMT_D24X8) ||
		D3DX8InitDev2(devtype, D3DFMT_D16)
		)) return false;

// nastavení viewportu
	if (!D3DX8SetView())
	{
		D3DX8Stop();
		return false;
	}

// inicializace adres funkcí zaøízení
	pD3Stop = D3DX8Stop;			// ukonèení funkce zaøízení
	pD3SizeView = D3DX8SizeView;	// aktualizace viewportu po zmìnì velikosti
	pD3MoveView = D3DX8MoveView;	// aktualizace viewportu po pøesunu
	pD3Disp = D3DX8Disp;			// zobrazení renderovacího bufferu
	pD3Clear = D3DX8Clear;			// vymazání renderovací plochy
	pD3AktProj = D3DX8AktProj;		// aktualizace projekèní matice
	pD3AktView = D3DX8AktView;		// aktualizace pohledové matice
	pD3Begin = D3DX8Begin;			// zahájení renderování scény
	pD3End = D3DX8End;				// ukonèení renderování scény
	pD3Render = D3DX8Render;		// vyrenderování jednoho objektu
	pD3Reset = D3DX8Reset;			// resetování objektu pøed ukonèením ovladaèe
	pD3SReset = D3DX8SReset;		// resetování stínu objektu pøed ukonèením ovladaèe
	pD3AktWireframe = D3DX8AktWireframe;
	pD3AktLightOn = D3DX8AktLightOn;
	pD3AktShades = D3DX8AktShades;
	pD3AktCulling = D3DX8AktCulling;
	pD3AktMagFilter = D3DX8AktMagFilter;
	pD3AktMinMipFilter = D3DX8AktMinMipFilter;
	pD3AktLight = D3DX8AktLight;
	pD3SetMaterial = D3DX8SetMaterial;
	pD3TextureRes = D3DX8TextureRes;
	pD3TextureSet = D3DX8TextureSet;
	pD3TextureOn = D3DX8TextureOn;
	pD3TextureOff = D3DX8TextureOff;
	pD3AktAmbient = D3DX8AktAmbient;
	pD3Free = D3DX8Free;
	pD3AktFog = D3DX8AktFog;
	pD3FogOn = D3DX8FogOn;
	pD3AktState = D3DX8AktState;
	pD3AktBlend = D3DX8AktBlend;
	pD3AktAlfa = D3DX8AktAlfa;
	pD3SetColorOp = D3DX8SetColorOp;
	pD3SetAlphaOp = D3DX8SetAlphaOp;
	pD3SetAddress = D3DX8SetAddress;
//	pD3SetBorder = D3DX8SetBorder;
	pD3SetBias = D3DX8SetBias;
	pD3SetTFactor = D3DX8SetTFactor;
	pD3SetZFunc = D3DX8SetZFunc;
	pD3SetZWrite = D3DX8SetZWrite;
	pD3SEnable = D3DX8SEnable;
	pD3SetSOp = D3DX8SetSOp;
	pD3SetSFunc = D3DX8SetSFunc;
	pD3SetSMask = D3DX8SetSMask;
	pD3Shadow = D3DX8Shadow;
	pD3Shadows = D3DX8Shadows;

// inicializace informací o zaøízení
	D3DCAPS8 D3DCaps;

	D3DX8Dev->GetDeviceCaps(&D3DCaps);

	MaxTextureWidth = D3DCaps.MaxTextureWidth;
	MaxTextureHeight = D3DCaps.MaxTextureHeight;
	MaxTextureRepeat = D3DCaps.MaxTextureRepeat;

	SquareTexture = ((D3DCaps.TextureCaps & D3DPTEXTURECAPS_SQUAREONLY) != 0);
	TexturePow2 = ((D3DCaps.TextureCaps & D3DPTEXTURECAPS_POW2) != 0);
	IsBlending = ((D3DCaps.DestBlendCaps & (D3DPBLENDCAPS_ONE | D3DPBLENDCAPS_INVSRCALPHA)) != 0);
//					(type >= 1) && (type <= 3));
	IsMagFilter = ((D3DCaps.TextureFilterCaps & D3DPTFILTERCAPS_MAGFLINEAR) != 0); // && (type >= 1) && (type <= 3));
	IsMinFilter = ((D3DCaps.TextureFilterCaps & D3DPTFILTERCAPS_MINFLINEAR) != 0); // && (type >= 1) && (type <= 3));
	IsMipMap = ((D3DCaps.TextureCaps & D3DPTEXTURECAPS_MIPMAP) != 0); // && (type >= 1) && (type <= 3));
	IsHWRaster = ((D3DCaps.DevCaps & D3DDEVCAPS_HWRASTERIZATION) != 0);
	IsHWTransLight = ((D3DCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) != 0);
	IsPureDevice = ((D3DCaps.DevCaps & D3DDEVCAPS_PUREDEVICE) != 0);
	AlphaGreaterEqual = ((D3DCaps.AlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL) != 0);
	AlphaGreater = ((D3DCaps.AlphaCmpCaps & D3DPCMPCAPS_GREATER) != 0);
	MaxTextureStages = D3DCaps.MaxTextureBlendStages;
	MaxTextureSimult = D3DCaps.MaxSimultaneousTextures;

	if ((DWORD)D3DCaps.MaxVertexIndex < (DWORD)0x10000)
	{
		MaxVertexNum = D3DCaps.MaxVertexIndex + 1;
	}

	bool isfog = ((D3DCaps.ShadeCaps & D3DPSHADECAPS_FOGGOURAUD) != 0);
	IsTableFog = ((D3DCaps.RasterCaps & D3DPRASTERCAPS_FOGTABLE) != 0) && isfog;
	IsVertexFog = ((D3DCaps.RasterCaps & D3DPRASTERCAPS_FOGVERTEX) != 0) && isfog;
	IsRangeFog = ((D3DCaps.RasterCaps & D3DPRASTERCAPS_FOGRANGE) != 0) && isfog;

	D3DX8VertCol = -1;	// aktuální stav pøepínaèe barev z vrcholù (-1=neplatný)

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// ukonèení funkce zaøízení

void D3DX8Stop()
{
	D3DX8TermDev();			// zrušení zaøízení
	FPUDouble();			// návrat pøesnosti koprocesoru
}

/////////////////////////////////////////////////////////////////////////////
// vymazání renderovací plochy

void D3DX8Clear()
{
// test, zda je textura
	if (D3DBackText >= 0)
	{

// vytvoøení bufferu pozadí scény
		D3DX8InitBack();

		if (D3DX8BackBuf != NULL)
		{

// vypnutí barev vrcholù
			if (D3DX8VertCol != FALSE)
			{
				D3DX8VertCol = FALSE;
				D3DX8Dev->SetRenderState(D3DRS_COLORVERTEX, FALSE);
				D3DX8Dev->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
				D3DX8Dev->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
			}

// renderování
			D3DX8Dev->SetStreamSource(0, D3DX8BackBuf, sizeof(BACKVERTEX));
			D3DX8Dev->SetVertexShader(D3DFVF_BACKVERTEX);
			D3DX8Dev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}

// závìreèné vymazání Z bufferu
		D3DX8Dev->Clear(1, &D3DX8Src, D3DCLEAR_ZBUFFER | (D3DSBufferClear ? D3DCLEAR_STENCIL : 0), 0, 1, 0);
	}
	else
	{

// pozadí se neinicializuje
		if (D3DBackCol == (MAXDWORD-1))
		{
			if (D3DSBufferClear)
			{
				D3DX8Dev->Clear(1, &D3DX8Src,  D3DCLEAR_STENCIL, 0, 1, 0);
			}
			return;
		}

// není textura, je barva - vymazání barvy i Z bufferu
		if (D3DBackCol != MAXDWORD)
		{
			D3DX8Dev->Clear(1, &D3DX8Src, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | (D3DSBufferClear ? D3DCLEAR_STENCIL : 0), D3DBackColRGB, 1, 0);
		}
		else

// jinak pouze vymazání Z bufferu
		{
			D3DX8Dev->Clear(1, &D3DX8Src, D3DCLEAR_ZBUFFER | (D3DSBufferClear ? D3DCLEAR_STENCIL : 0), 0, 1, 0);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// zobrazení renderovacího bufferu

void D3DX8Disp()
{
	if (D3DX8Dev == NULL) return;

// test ztraceného zaøízení
	HRESULT res = D3DX8Dev->TestCooperativeLevel();

	if (res == D3D_OK)
	{

// zobrazení renderovacího bufferu
		D3DX8Dev->Present((RECT*)&D3DX8Src, &D3DX8Dest, MainFrame, NULL);
	}
	else
	{
		if (res == D3DERR_DEVICENOTRESET)
		{
			D3DX8Dev->Reset(&D3DX8Par);

			D3DM_Reset();
			D3DF_Reset();
			D3DL_Reset();
			D3DT_Reset();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace projekèní matice

void D3DX8AktProj()
{
	D3DX8Dev->SetTransform(D3DTS_PROJECTION, &D3DProjMatrix);
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace pohledové matice

void D3DX8AktView()
{
	D3DX8Dev->SetTransform(D3DTS_VIEW, &D3DViewMatrix);
}


/////////////////////////////////////////////////////////////////////////////
// zahájení renderování scény

void D3DX8Begin()
{
	D3DX8Dev->BeginScene();
}


/////////////////////////////////////////////////////////////////////////////
// ukonèení renderování scény

void D3DX8End()
{
	D3DX8Dev->EndScene();
}


/////////////////////////////////////////////////////////////////////////////
// vyrenderování jednoho objektu

void D3DX8Render(D3DFITEM* item)
{
// nastavení zdroje barev
	BOOL vertcol = ((item->VertCol != NULL) && ((item->MatSource == MATSOURCE_OBJECT) || !D3DLightOnAkt));

	if (vertcol != D3DX8VertCol)
	{
		D3DX8VertCol = vertcol;

		if (vertcol)
		{
			D3DX8Dev->SetRenderState(D3DRS_COLORVERTEX, TRUE);
			D3DX8Dev->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
			D3DX8Dev->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);
		}
		else
		{
			D3DX8Dev->SetRenderState(D3DRS_COLORVERTEX, FALSE);
			D3DX8Dev->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
			D3DX8Dev->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
		}
	}

// pøíprava stupòù souøadnic
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

// typ položky
	bool isFlare = (item->Type == D3DFTYPE_LENSFLARE);
	bool isPic = ((item->Type == D3DFTYPE_PICTURE) || isFlare);

	bool isCol = ((item->VertCol != NULL) /*|| isFlare*/);

//	DWORD flareCol = item->Diffuse;	

	int itemsize = 2*sizeof(D3DXVECTOR3);
	int itemtype = (D3DFVF_XYZ|D3DFVF_NORMAL);

	if (isPic)
	{
		itemsize = sizeof(D3DXVECTOR3) + sizeof(float);
		itemtype = D3DFVF_XYZRHW;
	}

	itemsize += 2*stages*sizeof(float);
	itemtype |= (D3DFVF_TEX0 + (D3DFVF_TEX1-D3DFVF_TEX0)*stages);

	if(isCol)
	{
		itemsize += sizeof(DWORD);
		itemtype |= D3DFVF_DIFFUSE;
	}

// pøegenerování bufferu pro LensFlare
	if (isFlare) D3DX8Reset(item);

// vytvoøení bufferu vektorù (Data1), není-li ještì vytvoøen
	if (item->Data1 == NULL)
	{
		IDirect3DVertexBuffer8* vertbuf = NULL;
		D3DX8Dev->CreateVertexBuffer(item->VertNum*itemsize, D3DUSAGE_WRITEONLY,
								itemtype, D3DPOOL_MANAGED, &vertbuf);
		item->Data1 = vertbuf;

		if (vertbuf != NULL)
		{
			void* vert;

			if(vertbuf->Lock( 0, 0, (BYTE**)&vert, 0 ) == D3D_OK)
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

//						bool flareVisible = false;

						if (vv.z > 0.001)
						{
							sx = vv.x/vv.z * item->ScaleZ / D3DWidthHeight2;
							sy = vv.y/vv.z * item->ScaleZ * D3DWidthHeight2;

//							flareVisible = true;

						}
						else
						{
							sx = -1;
							sy = -1;
						}

//						if (!flareVisible)
//						{
//							flareCol = 0;
//						}
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
//						if (isFlare)
//						{
//							*((DWORD*)vert) = flareCol;
//						}
//						else
//						{
							*((DWORD*)vert) = item->VertCol[i];
//						}
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

// aktualizace klonù
		D3DFITEM* item2 = item;

		for (;;)
		{
			item2 = D3DF_Get(item2->CloneNext);
			if (item2 == item) break;

			item2->Data1 = item->Data1;
		}
	}

// vytvoøení bufferu indexù, není-li ještì vytvoøen
	if (item->Data2 == NULL)
	{
		D3DX8Dev->CreateIndexBuffer(item->FaceNum*3*sizeof(WORD),
								D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
								D3DPOOL_MANAGED, (IDirect3DIndexBuffer8**)&(item->Data2));

		IDirect3DIndexBuffer8* indbuf = (IDirect3DIndexBuffer8*)(item->Data2);
		if (indbuf != NULL)
		{
			WORD* pFaces;
			if (indbuf->Lock(0, 0, (BYTE**)&pFaces, 0) == D3D_OK)
			{
				int* fs = item->Face;

				for (int i = item->FaceNum*3; i > 0; i--)
				{
					*pFaces = (WORD)*fs;
					fs++;
					pFaces++;
				}
				indbuf->Unlock();
			}
		}

// aktualizace klonù
		D3DFITEM* item2 = item;

		for (;;)
		{
			item2 = D3DF_Get(item2->CloneNext);
			if (item2 == item) break;

			item2->Data2 = item->Data2;
		}
	}

// nastavení transformaèní matice
	if (!isPic)
	{
		D3DX8Dev->SetTransform(D3DTS_WORLD, &item->MatrixWorld);
	}

// vyrenderování objektu
	IDirect3DVertexBuffer8* vertbuf = (IDirect3DVertexBuffer8*)(item->Data1);
	IDirect3DIndexBuffer8* indbuf = (IDirect3DIndexBuffer8*)(item->Data2);

	if ((vertbuf != NULL) && (indbuf != NULL))
	{
		BOOL clip = (BOOL)item->AktClip;
		if (clip != D3DX8Clipping)
		{
			D3DX8Clipping = clip;
			D3DX8Dev->SetRenderState(D3DRS_CLIPPING, clip);
		}

		D3DX8Dev->SetIndices(indbuf, 0);
		D3DX8Dev->SetStreamSource(0, vertbuf, itemsize);
		D3DX8Dev->SetVertexShader(itemtype);
		D3DX8Dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, item->VertNum, 0, item->FaceNum);
	}
}

/////////////////////////////////////////////////////////////////////////////
// vyrenderování stínu

struct SVERTEX
{
    D3DXVECTOR3 pos;
	D3DCOLOR col;
};

void D3DX8Shadow(D3DFITEM* item)
{
// nastavení zdroje barev
	int itemsize;
	int itemtype;

	if (D3DSBufferFormatOK)
	{
		itemsize = sizeof(D3DXVECTOR3);
		itemtype = (D3DFVF_XYZ);

		if (D3DX8VertCol)
		{
			D3DX8VertCol = FALSE;
			D3DX8Dev->SetRenderState(D3DRS_COLORVERTEX, FALSE);
			D3DX8Dev->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
			D3DX8Dev->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
		}
	}
	else
	{
		itemsize = sizeof(D3DXVECTOR3) + sizeof(D3DCOLOR);
		itemtype = (D3DFVF_XYZ  | D3DFVF_DIFFUSE);

		if (!D3DX8VertCol)
		{
			D3DX8VertCol = TRUE;
			D3DX8Dev->SetRenderState(D3DRS_COLORVERTEX, TRUE);
			D3DX8Dev->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
			D3DX8Dev->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);
		}
	}

// vytvoøení bufferu vektorù (Data3), není-li ještì vytvoøen
	if ((item->Data3 == NULL) && (item->ShadowFan0 > 0))
	{
		IDirect3DVertexBuffer8* vertbuf = NULL;
		D3DX8Dev->CreateVertexBuffer(item->ShadowFan0*itemsize, D3DUSAGE_WRITEONLY,
								itemtype, D3DPOOL_MANAGED, &vertbuf);
		item->Data3 = vertbuf;

		if (vertbuf != NULL)
		{
			void* vert;

			if(vertbuf->Lock( 0, 0, (BYTE**)&vert, 0 ) == D3D_OK)
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
	D3DX8Dev->SetTransform(D3DTS_WORLD, &item->MatrixWorld);

// vyrenderování objektu
	IDirect3DVertexBuffer8* vertbuf = (IDirect3DVertexBuffer8*)(item->Data3);

	if (vertbuf != NULL)
	{
		if (!D3DX8Clipping)
		{
			D3DX8Clipping = TRUE;
			D3DX8Dev->SetRenderState(D3DRS_CLIPPING, TRUE);
		}

		D3DX8Dev->SetStreamSource(0, vertbuf, itemsize);
		D3DX8Dev->SetVertexShader(itemtype);

		int v = 0;

		for (int i = 0; i < item->ShadowFanN; i++)
		{
			D3DX8Dev->DrawPrimitive(D3DPT_TRIANGLEFAN, v, item->ShadowFan[i] - 2);
			v += item->ShadowFan[i];
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// resetování stínu objektu pøed ukonèením ovladaèe

void D3DX8SReset(D3DFITEM* item)
{
// resetování bufferu stínù
	if (item->Data3 != NULL)
	{
		((IDirect3DIndexBuffer8*)(item->Data3))->Release();
		item->Data3 = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// resetování objektu pøed ukonèením ovladaèe

void D3DX8Reset(D3DFITEM* item)
{
// resetování bufferu vrcholù
	if (item->Data1 != NULL)
	{
		((IDirect3DVertexBuffer8*)(item->Data1))->Release();

		D3DFITEM* item2 = item;

		do {
			item2->Data1 = NULL;

			item2 = D3DF_Get(item2->CloneNext);
		} while (item != item2);
	}

// resetování bufferu indexù
	if (item->Data2 != NULL)
	{
		((IDirect3DIndexBuffer8*)(item->Data2))->Release();

		D3DFITEM* item2 = item;

		do {
			item2->Data2 = NULL;

			item2 = D3DF_Get(item2->CloneNext);
		} while (item != item2);
	}

// resetování bufferu stínù
	D3DX8SReset(item);
}

/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè vyplòování ploch

void D3DX8AktWireframe()
{
	D3DX8Dev->SetRenderState(D3DRS_FILLMODE, (D3DWireframeAkt ? D3DFILL_SOLID : D3DFILL_WIREFRAME));
}


/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè osvìtlení

void D3DX8AktLightOn()
{
	D3DX8Dev->SetRenderState(D3DRS_LIGHTING, (D3DLightOnAkt ? TRUE : FALSE));
}


/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè plynulého stinování

void D3DX8AktShades()
{
	D3DX8Dev->SetRenderState(D3DRS_SHADEMODE, (D3DShadesAkt ? D3DSHADE_GOURAUD : D3DSHADE_FLAT));
}


/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè odstraòování ploch

void D3DX8AktCulling()
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

	D3DX8Dev->SetRenderState(D3DRS_CULLMODE, n);
}


/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè filtrace zvìtšených textur

void D3DX8AktMagFilter(int stage)
{
	D3DX8Dev->SetTextureStageState(stage, D3DTSS_MAGFILTER, (D3DMagFilterAkt[stage] ? D3DTEXF_LINEAR : D3DTEXF_POINT));
}


/////////////////////////////////////////////////////////////////////////////
// aktualizovat pøepínaè filtrace zmenšených a vzdálených textur

void D3DX8AktMinMipFilter(int stage)
{
	DWORD min = D3DTEXF_POINT;
	DWORD mip = D3DTEXF_NONE;

	if (D3DMipFilterAkt[stage]) mip = D3DTEXF_POINT;

	if (D3DMinFilterAkt[stage])
	{
		min = D3DTEXF_LINEAR;
		if (D3DMipFilterAkt[stage]) mip = D3DTEXF_LINEAR;
	}

	D3DX8Dev->SetTextureStageState(stage, D3DTSS_MINFILTER, min);
	D3DX8Dev->SetTextureStageState(stage, D3DTSS_MIPFILTER, mip);
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace položky svìtla

void D3DX8AktLight(int inx, D3DLITEM* item)
{
	if (item->Akt)
	{
		if (item->Enable)
		{
			D3DX8Dev->SetLight(inx, &(item->Light));
			D3DX8Dev->LightEnable(inx, TRUE);
		}
		else
		{
			D3DX8Dev->LightEnable(inx, FALSE);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení materiálu

void D3DX8SetMaterial(D3DMITEM* mat)
{
	D3DX8Dev->SetMaterial(&mat->Material);
}


/////////////////////////////////////////////////////////////////////////////
// resetování položky textury pøed ukonèením ovladaèe

void D3DX8TextureRes(D3DTITEM* item)
{
	if (item->Data1 != NULL)
	{
		((IDirect3DTexture8*)(item->Data1))->Release();
		item->Data1 = NULL;
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastavení a zapnutí textury

int tx8Width;
int tx8Height;
int tx8Mip;

BYTE* tx8Data;
int tx8WidthMul;

IDirect3DTexture8* tx8Tex;

bool _fastcall D3DX8TextCreate(D3DFORMAT f, BYTE* data, int mul)
{
	tx8Data = data;
	tx8WidthMul = mul;

	tx8Tex = NULL;

	return ((D3DX8Dev->CreateTexture(
		tx8Width,							// šíøka textury
		tx8Height,							// výška textury
		tx8Mip,								// poèet generovaných úrovní
		0,									// použití
		f,									// formát
		D3DPOOL_MANAGED,					// správa pamìti
		&tx8Tex) == D3D_OK) &&
		(tx8Tex != NULL));
}

void D3DX8TextureSet(D3DTITEM* item, int stage)
{
// data obrázku
	PICTUREDATA* picdata = item->Pic.Data();

// vytvoøení textury
	if (item->Data1 == NULL)
	{
		tx8Width = picdata->TextWidth;
		tx8Height = picdata->TextHeight;
		tx8Mip = picdata->TextMipMaps;
		if (!IsMipMap || !item->Mips) tx8Mip = 1;

		// omezení poètu úrovní - pøi vyšším stupni se prolínají ikony textury terénu
		int mipmaps = item->MipMaps;
		if (mipmaps > 0)
		{
			if (tx8Mip > mipmaps) tx8Mip = mipmaps;
		}

		if ((!picdata->TextTrans && (D3DX8TextCreate(D3DFMT_R5G6B5, picdata->TextDataR5G6B5, 2) ||
									D3DX8TextCreate(D3DFMT_A1R5G5B5, picdata->TextDataA1R5G5B5, 2))) ||
			((D3DLowText || (picdata->TextSmooth < 1.25)) && 
								(D3DX8TextCreate(D3DFMT_A4R4G4B4, picdata->TextDataA4R4G4B4, 2) ||
									D3DX8TextCreate(D3DFMT_A1R5G5B5, picdata->TextDataA1R5G5B5, 2))) ||
			D3DX8TextCreate(D3DFMT_A8R8G8B8, picdata->TextData, 4) ||
			D3DX8TextCreate(D3DFMT_A4R4G4B4, picdata->TextDataA4R4G4B4, 2) ||
			D3DX8TextCreate(D3DFMT_A1R5G5B5, picdata->TextDataA1R5G5B5, 2) ||
			D3DX8TextCreate(D3DFMT_R5G6B5, picdata->TextDataR5G6B5, 2) ||
			D3DX8TextCreate(D3DFMT_X1R5G5B5, picdata->TextDataA1R5G5B5, 2) ||
			D3DX8TextCreate(D3DFMT_R8G8B8, picdata->TextDataR8G8B8, 3) ||
			D3DX8TextCreate(D3DFMT_X8R8G8B8, picdata->TextData, 4) ||
			D3DX8TextCreate(D3DFMT_X4R4G4B4, picdata->TextDataA4R4G4B4, 2))
		{
			item->Data1 = tx8Tex;

			D3DLOCKED_RECT dr;

			int m = 0;
			BYTE* src = tx8Data;

			for (; tx8Mip > 0; tx8Mip--)
			{
				if (tx8Tex->LockRect(m, &dr, NULL, 0) == D3D_OK)
				{
					BYTE* dst = (BYTE*)(dr.pBits);
					int pitch = dr.Pitch;

					int widthbyte = tx8Width * tx8WidthMul;

					if (pitch >= widthbyte)
					{
						for (int i = tx8Height; i > 0; i--)
						{
							MemCopy(dst, src, widthbyte);
							dst += pitch;
							src += widthbyte;
						}
					}

					tx8Tex->UnlockRect(m);
				}

				m++;
				tx8Width = (tx8Width >> 1);
				if (tx8Width == 0) tx8Width++;
				tx8Height = (tx8Height >> 1);
				if (tx8Height == 0) tx8Height++;
			}
		}
	}

// nastavení textury
	if (item->Data1 != NULL)
	{
		D3DX8Dev->SetTextureStageState(stage, D3DTSS_TEXCOORDINDEX, stage);
		D3DX8Dev->SetTexture(stage, (IDirect3DTexture8*)(item->Data1));
	}
}


/////////////////////////////////////////////////////////////////////////////
// vypnutí textury

void D3DX8TextureOff(int stage)
{
	D3DX8Dev->SetTexture(stage, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace ambient osvìtlení

void D3DX8AktAmbient()
{
	D3DX8Dev->SetRenderState(D3DRS_AMBIENT, D3DAmbientColRGB);
}

/////////////////////////////////////////////////////////////////////////////
// volná videopamì

int D3DX8Free()
{
	return D3DX8Dev->GetAvailableTextureMem();
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace mlhy

void D3DX8AktFog()
{
	D3DX8Dev->SetRenderState(D3DRS_FOGCOLOR, FogColorRGB);

	int n = FogType;
	if (n == 0) n = 3;

	bool vertexmode = (FogKorig || D3DVertFog || !IsTableFog);
	if (!IsVertexFog) vertexmode = false;

	if (vertexmode)
	{
		D3DX8Dev->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
		D3DX8Dev->SetRenderState(D3DRS_FOGVERTEXMODE, n);
	}
	else
	{
		D3DX8Dev->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
		D3DX8Dev->SetRenderState(D3DRS_FOGTABLEMODE, n);
	}
	D3DX8Dev->SetRenderState(D3DRS_RANGEFOGENABLE, (BOOL)(FogKorig && IsRangeFog));

	float x = (float)FogStart;
	D3DX8Dev->SetRenderState(D3DRS_FOGSTART, *((DWORD*)&x));

	x = (float)FogEnd;
	D3DX8Dev->SetRenderState(D3DRS_FOGEND, *((DWORD*)&x));

	x = (float)FogDens;
	D3DX8Dev->SetRenderState(D3DRS_FOGDENSITY, *((DWORD*)&x));
}
			  
/////////////////////////////////////////////////////////////////////////////
// zapnutí mlhy

void D3DX8FogOn(BOOL on)
{
	D3DX8Dev->SetRenderState(D3DRS_FOGENABLE, on);
	if (!on) D3DX8Dev->SetRenderState(D3DRS_RANGEFOGENABLE, FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace stavu

void D3DX8AktState()
{
	D3DX8Dev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	D3DX8Dev->SetRenderState(D3DRS_LOCALVIEWER, TRUE);
	D3DX8Dev->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	D3DX8Dev->SetRenderState(D3DRS_DITHERENABLE, ((ScreenBits < 20) ? TRUE : FALSE));
	D3DX8Dev->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
	D3DX8Dev->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);
	D3DX8Dev->SetRenderState(D3DRS_LASTPIXEL, TRUE);

	for (int i = D3DRS_WRAP0; i <= D3DRS_WRAP7; i++)
	{
		D3DX8Dev->SetRenderState((D3DRENDERSTATETYPE)i, 0);
	}

	D3DX8Clipping = -1;
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace blending operací objektu

void D3DX8AktBlend(D3DFITEM* item)
{
	if (IsBlending)
	{
		D3DX8Dev->SetRenderState(D3DRS_SRCBLEND, BlendTabD3D[item->SrcBlend]);
		D3DX8Dev->SetRenderState(D3DRS_DESTBLEND, BlendTabD3D[item->DstBlend]);
	}
	D3DX8Dev->SetRenderState(D3DRS_ALPHABLENDENABLE, (BOOL)(item->IsBlend && IsBlending));
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace referenèní úrovnì alfa

void D3DX8AktAlfa(D3DFITEM* item)
{
	BOOL on = (BOOL)(item->IsAlphaRef);
	D3DX8Dev->SetRenderState(D3DRS_ALPHATESTENABLE, on);

	if (on)
	{
		D3DX8Dev->SetRenderState(D3DRS_ALPHAREF, item->AlphaRef2);

		if (AlphaGreaterEqual)
		{
			D3DX8Dev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		}
		else
		{
			D3DX8Dev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení operace barev

void D3DX8SetColorOp(int op, int arg1, int arg2, int stage)
{
	D3DX8Dev->SetTextureStageState(stage, D3DTSS_COLOROP, op);
	D3DX8Dev->SetTextureStageState(stage, D3DTSS_COLORARG1, arg1);
	D3DX8Dev->SetTextureStageState(stage, D3DTSS_COLORARG2, arg2);
}

/////////////////////////////////////////////////////////////////////////////
// nastavení operace alfa

void D3DX8SetAlphaOp(int op, int arg1, int arg2, int stage)
{
	D3DX8Dev->SetTextureStageState(stage, D3DTSS_ALPHAOP, op);
	D3DX8Dev->SetTextureStageState(stage, D3DTSS_ALPHAARG1, arg1);
	D3DX8Dev->SetTextureStageState(stage, D3DTSS_ALPHAARG2, arg2);
}

/////////////////////////////////////////////////////////////////////////////
// nastavení adresování textur

void D3DX8SetAddress(int adru, int adrv, int stage)
{
	D3DX8Dev->SetTextureStageState(stage, D3DTSS_ADDRESSU, adru);
	D3DX8Dev->SetTextureStageState(stage, D3DTSS_ADDRESSV, adrv);
}

/////////////////////////////////////////////////////////////////////////////
// nastavení barvy okolí textur

//void D3DX8SetBorder(D3DFITEM* item, int stage)
//{
//	D3DX8Dev->SetTextureStageState(stage, D3DTSS_BORDERCOLOR, item->BorderRGB[stage]);
//}

/////////////////////////////////////////////////////////////////////////////
// nastavení zjemnìní vzdálených textur

void D3DX8SetBias(float bias, int stage)
{
	D3DX8Dev->SetTextureStageState(stage, D3DTSS_MIPMAPLODBIAS, *(DWORD*)(&bias));
}

/////////////////////////////////////////////////////////////////////////////
// nastavení barvy faktoru textur

void D3DX8SetTFactor(D3DFITEM* item)
{
	D3DX8Dev->SetRenderState(D3DRS_TEXTUREFACTOR, item->TFactorRGB);
}

/////////////////////////////////////////////////////////////////////////////
// nastavení Z funkce (1 až 8, v kódu DirectX, 0=vypnuto)

void D3DX8SetZFunc(int func)
{
	if (func == 0)
	{
		D3DX8Dev->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		D3DX8Dev->SetRenderState(D3DRS_ZENABLE, FALSE);
	}
	else
	{
		D3DX8Dev->SetRenderState(D3DRS_ZENABLE, TRUE);
		D3DX8Dev->SetRenderState(D3DRS_ZFUNC, func);
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení Z zápisu

void D3DX8SetZWrite(BOOL write)
{
	D3DX8Dev->SetRenderState(D3DRS_ZWRITEENABLE, write);
}

/////////////////////////////////////////////////////////////////////////////
// povolení stencil operací

void D3DX8SEnable(BOOL enable)
{
	D3DX8Dev->SetRenderState(D3DRS_STENCILENABLE, enable);
}

/////////////////////////////////////////////////////////////////////////////
// nastavení stencil operací

void D3DX8SetSOp(int fail, int zfail, int zpass)
{
	D3DX8Dev->SetRenderState(D3DRS_STENCILFAIL, fail);
	D3DX8Dev->SetRenderState(D3DRS_STENCILZFAIL, zfail);
	D3DX8Dev->SetRenderState(D3DRS_STENCILPASS, zpass);
}

/////////////////////////////////////////////////////////////////////////////
// nastavení stencil testu

void D3DX8SetSFunc(int func, int ref, DWORD mask)
{
	D3DX8Dev->SetRenderState(D3DRS_STENCILFUNC, func);
	D3DX8Dev->SetRenderState(D3DRS_STENCILREF, ref);
	D3DX8Dev->SetRenderState(D3DRS_STENCILMASK, mask);
}

/////////////////////////////////////////////////////////////////////////////
// nastavení zápisové masky do stencil bufferu

void D3DX8SetSMask(DWORD mask)
{
	D3DX8Dev->SetRenderState(D3DRS_STENCILWRITEMASK, mask);
}

/////////////////////////////////////////////////////////////////////////////
// vykreslení stínù

void D3DX8Shadows()
{
// vytvoøení bufferu stínù
	D3DX8InitShadow();

	if (D3DX8ShadowBuf != NULL)
	{

// zapnutí barev vrcholù
		if (!D3DX8VertCol)
		{
			D3DX8VertCol = TRUE;
			D3DX8Dev->SetRenderState(D3DRS_COLORVERTEX, TRUE);
			D3DX8Dev->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
			D3DX8Dev->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);
		}

		D3DX8Dev->SetTexture(0, NULL);

// renderování
		D3DX8Dev->SetStreamSource(0, D3DX8ShadowBuf, sizeof(SHADOWVERTEX));
		D3DX8Dev->SetVertexShader(D3DFVF_SHADOWVERTEX);
		D3DX8Dev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}


#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------
