
#define	DIRECT3D_VERSION 0x0800

#include "Main.h"

/***************************************************************************\
*																			*
*							Hlavní okno aplikace							*
*																			*
\***************************************************************************/

// všeobecné informace o oknì
bool				Resizing			= false;	// probíhá programová zmìna okna
bool				AppActive			= true;		// aplikace je aktivní
bool				AppMinim			= false;	// aplikace minimalizovaná

// parametry celoobrazovkového módu pro aktivaci aplikace
int					FullScreenWidth		= 640;
int					FullScreenHeight	= 480;
int					FullScreenBits		= 16;

// celoobrazovkovı reim
bool				FullScreen			= false;	// je celoobrazovkovı reim
BOOL				OldMaximized		= FALSE;	// uschovanı pøíznak maximalizace
int					NumVideoModes		= 0;		// poèet videomódù v tabulce
VIDEOMODEITEM*		TabVideoModes		= NULL;		// tabulka videomódù
bool				IsVideoModes		= false;	// seznam videomódù je inicializován

// poadovanı uivatelskı videomód
int					UserScreenWidth		= 0;		// uivatelská šíøka videomódu (0=implicitní)
int					UserScreenHeight	= 0;		// uivatelská vıška videomódu (0=implicitní)
int					UserScreenBits		= 0;		// uivatelskı poèet bitù (0=implicitní)


// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

// ----------------------------------------------------------------------------
// !!! POZOR POZOR - funkce Direct3D pøepínají koprocesor na float pøesnost !!!
// ----------------------------------------------------------------------------

// pøíznak aktivního reimu 3D
bool				D3D					= false;	// je zobrazeno okno 3D

// zvolené rozhraní a ovladaè
int					D3DIntUser			= 0;		// uivatelem zadané rozhraní 0 a D3INTERFACES-1 (0=automatickı)
int					D3DIntAkt			= 0;		// typ aktivního rozhraní (0=není,1=GL0,2=GL1,3=GL2,4=DX3,5=DX5,6=DX6,7=DX7,8=DX8)
int					D3DDevUser			= 0;		// uivatelem zadanı typ ovladaèe 0 a D3DEVICES-1 (0=automatickı)
int					D3DDevAkt			= 0;		// typ aktivního ovladaèe (0=není,1=HAL,2=TnLHal,3=REF,4=RGB,5=MMX,6=Ramp)

// funkce aktivního rozhraní (jsou vy platné i bez aktivního ovladaèe)
D3STOP				pD3Stop				= D3NoStop;			// ukonèení funkce zaøízení
D3SIZEVIEW			pD3SizeView			= D3NoSizeView;		// aktualizace viewportu po zmìnì velikosti
D3MOVEVIEW			pD3MoveView			= D3NoMoveView;		// aktualizace viewportu po pøesunu
D3DISP				pD3Disp				= D3NoDisp;			// zobrazení renderovacího bufferu
D3CLEAR				pD3Clear			= D3NoClear;		// vymazání renderovací plochy
D3AKTPROJ			pD3AktProj			= D3NoAktProj;		// aktualizace projekèní matice
D3AKTVIEW			pD3AktView			= D3NoAktView;		// aktualizace pohledové matice
D3BEGIN				pD3Begin			= D3NoBegin;		// zahájení renderování scény
D3END				pD3End				= D3NoEnd;			// ukonèení renderování scény
D3RENDER			pD3Render			= D3NoRender;		// vyrenderování jednoho objektu
D3RESET				pD3Reset			= D3NoReset;		// resetování objektu pøed ukonèením ovladaèe
D3SRESET			pD3SReset			= D3NoSReset;		// resetování stínu objektu pøed ukonèením ovladaèe
D3AKTWIREFRAME		pD3AktWireframe		= D3NoAktWireframe;	// aktualizovat pøepínaè vyplòování ploch
D3AKTLIGHTON		pD3AktLightOn		= D3NoAktLightOn;	// aktualizovat pøepínaè osvìtlení
D3AKTSHADES			pD3AktShades		= D3NoAktShades;	// aktualizovat pøepínaè plynulého stínování
D3AKTCULLING		pD3AktCulling		= D3NoAktCulling;	// aktualizovat pøepínaè odstraòování ploch
D3AKTMAGFILTER		pD3AktMagFilter		= D3NoAktMagFilter;	// aktualizovat pøepínaè filtrace zvìtšenıch textur
D3AKTMINMIPFILTER	pD3AktMinMipFilter	= D3NoAktMinMipFilter; // aktualizovat pøepínaè filtrace zmenšenıch a vzdálenıch textur
D3AKTLIGHT			pD3AktLight			= D3NoAktLight;		// aktualizace poloky svìtla
D3SETMATERIAL		pD3SetMaterial		= D3NoSetMaterial;	// nastavení materiálu
D3TEXTURERES		pD3TextureRes		= D3NoTextureRes;	// resetování poloky textury pøed ukonèením ovladaèe
D3TEXTURESET		pD3TextureSet		= D3NoTextureSet;	// nastavení a zapnutí textury
D3TEXTUREON			pD3TextureOn		= D3NoTextureOn;	// zapnutí pùvodní textury
D3TEXTUREOFF		pD3TextureOff		= D3NoTextureOff;	// vypnutí textury
D3MATRESET			pD3MatReset			= D3NoMatReset;		// resetování materiálu
D3LIGHTRESET		pD3LightReset		= D3NoLightReset;	// resetování svìtla
D3AKTAMBIENT		pD3AktAmbient		= D3NoAktAmbient;	// aktualizace ambient osvìtlení
D3FREE				pD3Free				= D3NoFree;			// volná videopamì
D3AKTFOG			pD3AktFog			= D3NoAktFog;		// aktualizace mlhy
D3FOGON				pD3FogOn			= D3NoFogOn;		// zapnutí mlhy
D3AKTSTATE			pD3AktState			= D3NoAktState;		// aktualizace stavu
D3AKTBLEND			pD3AktBlend			= D3NoAktBlend;		// aktualizace blending operací objektu
D3AKTALPHA			pD3AktAlfa			= D3NoAktAlfa;		// aktualizace referenèní úrovnì alfa
D3AKTPALETTE		pD3AktPalette		= D3NoAktPalette;	// aktualizace palet
D3SETCOLOROP		pD3SetColorOp		= D3NoSetColorOp;	// nastavení operace barev
D3SETALPHAOP		pD3SetAlphaOp		= D3NoSetAlphaOp;	// nastavení operace alfa
D3SETADDRESS		pD3SetAddress		= D3NoSetAddress;	// nastavení adresování textur
//D3SETBORDER			pD3SetBorder		= D3NoSetBorder;	// nastavení barvy okolí textur
D3SETBIAS			pD3SetBias			= D3NoSetBias;		// nastavení zjemnìní vzdálenıch textur
D3SETTFACTOR		pD3SetTFactor		= D3NoSetTFactor;	// nastavení barvy faktoru textur
D3SETZFUNC			pD3SetZFunc			= D3NoSetZFunc;		// nastavení Z funkce (1 a 8, v kódu DirectX)
D3SETZWRITE			pD3SetZWrite		= D3NoSetZWrite;	// nastavení hloubkového zápisu
D3SENABLE			pD3SEnable			= D3NoSEnable;		// povolení stencil operací
D3SETSOP			pD3SetSOp			= D3NoSetSOp;		// nastavení stencil operací
D3SETSFUNC			pD3SetSFunc			= D3NoSetSFunc;		// nastavení stencil testu
D3SETSMASK			pD3SetSMask			= D3NoSetSMask;		// nastavení zápisové masky do stencil bufferu
D3SHADOW			pD3Shadow			= D3NoShadow;		// vyrenderování stínu jednoho objektu
D3SHADOWS			pD3Shadows			= D3NoShadows;		// vykreslení stínù

// rozmìry okna Direct3D zadané uivatelem, bez omezení rozsahu
int					D3DX0				= 0;		// poèátek X okna Direct3D (zleva) zadanı uivatelem
int					D3DY0				= 0;		// poèátek Y okna Direct3D (zdola) zadanı uivatelem
int					D3DW0				= 0;		// šíøka okna Direct3D zadaná uivatelem
int					D3DH0				= 0;		// vıška okna Direct3D zadaná uivatelem

// rozmìry okna Direct3D zadané uivatelem, s omezením na plochu (pro zobrazení grafiky)
int					D3DX				= 0;		// poèátek X okna Direct3D (zleva) - zadáno
int					D3DY				= 0;		// poèátek Y okna Direct3D (zdola) - zadáno
int					D3DW				= 0;		// šíøka okna Direct3D - zadáno
int					D3DH				= 0;		// vıška okna Direct3D - zadáno
bool				D3D2D				= false;	// je zobrazena 2D grafika (spolu s oknem 3D)

// skuteèné rozmìry okna Direct3D na obrazovce (omezení na klientské souøadnice okna)
int					D3DLeft				= 0;		// skuteènı poèátek X okna Direct3D
int					D3DTop				= 0;		// skuteènı poèátek Y okna Direct3D (shora)
int					D3DWidth			= 0;		// skuteèná šíøka okna Direct3D
int					D3DHeight			= 0;		// skuteèná vıška okna Direct3D
double				D3DWidthHeight2		= 1;		// odmocnina s pomìru D3DWidth/D3DHeight
bool				D3DFull				= false;	// okno 3D vyuívá celou plochu hlavního okna

// pozadí scény
DWORD				D3DBackCol			= 0;		// barva pozadí scény ve formátu Petra BGR(MAXDWORD=nemazat, MAXDWORD-1=neinicializovat)
D3DCOLOR			D3DBackColRGB		= 0xff000000;	// barva pozadí scény ve formátu RGB
D3DCOLORVALUE		D3DBackColF			= {0, 0, 0, 1};	// barva pozadí scény ve formátu float
bool				D3DBackColAkt		= false;	// barva pozadí zmìnìna
//int					D3DBackMat			= 0;		// materiál pozadí scény
//D3DMATERIAL8		D3DBackMatMat;					// buffer materiálu pozadí scény
int					D3DBackText			= -1;		// textura pozadí scény (-1=není)
bool				D3DBackTextAkt		= false;	// textura pozadí zmìnìna

// nastavení mlhy
BOOL				D3DFogOn			= -1;		// mlha je aktuálnì zapnuta (-1 = neznámé)
bool				FogAkt				= true;		// mlhu je nutno aktualizovat
bool				FogKorig			= false;	// korigovaná mlha (pøièítá se +4 k typu mlhy)
bool				FogOn				= false;	// mlha je zapnuta
DWORD				FogColor			= MAXDWORD;	// barva mlhy ve formátu Petra BGR (MAXDWORD=vypnuto)
DWORD				FogColorRGB			= 0xff000000; // barva mlhy ve formáty RGB
D3DCOLORVALUE		FogColorF			= {0, 0, 0, 1}; // barva mlhy ve formátu float
int					FogType				= 1;		// typ mlhy (0=lineární, 1=exponenciální, 2=kvadratická)
double				FogStart			= 0;		// zaèátek lineární mlhy
double				FogEnd				= 20;		// konec lineární mlhy
double				FogDens				= 0.1;		// hustota exponenciální a kvadratické mlhy (0 a 1)

// ambient osvìtlení
DWORD				D3DAmbientCol		= 0x404040;	// ambient osvìtlení ve formátu Petra BGR
DWORD				D3DAmbientColRGB	= 0xff404040; // ambient osvìtlení ve formátu RGB
#define AMBCOL ((float)0.25)
D3DCOLORVALUE		D3DAmbientColF		= {AMBCOL, AMBCOL, AMBCOL, 1};	// ambient osvìtlení ve formátu float
bool				D3DAmbientColAkt	= true;		// ambient osvìtlení zmìnìno

// projekèní matice (pozor, èlen _34 matice nesmí bıt < 0 !!!)
bool				D3DProjAkt			= false;	// poadavek aktualizace projekèní matice
double				D3DFrontClip		= 1;		// vzdálenost pøední omezující roviny
double				D3DBackClip			= 100;		// vzdálenost zadní omezující roviny
D3DMATRIX			D3DProjMatrix;					// projekèní matice
int					D3DProjection		= 0;		// typ projekce (0=persp, 1=orto, 2=r-persp, 3=r-orto)
bool				D3DRightHand		= false;	// pravoruká projekce (typ projekce 2 a 3)

// pohledová matice (odvozená z kamery)
bool				D3DViewAkt			= false;	// poadavek aktualizace pohledové matice
D3DMATRIX			D3DViewMatrix;					// pohledová matice (vytvoøená z objektu kamery)

// frustum (odvozeno z pohledové a projekèní matice)
bool				D3DFrustumAkt		= false;	// aktualizovat frustum
PLANE				D3DFrustum[6];					// 6 rovin projekèního jehlanu

// pøepínaèe nastavení
bool				D3DWireframeUnknown	= true;		// stav pøepínaèe vyplòování ploch neznámı
bool				D3DWireframeAkt		= false;	// vyplòování ploch aktuálnì zapnut
bool				D3DWireframeGlobal	= true;		// vyplòování ploch globálnì povoleno

bool				D3DLightOnUnknown	= true;		// stav pøepínaèe osvìtlení neznámı
bool				D3DLightOnAkt		= false;	// osvìtlení aktuálnì zapnuto
bool				D3DLightOnGlobal	= true;		// osvìtlení globálnì povoleno

bool				D3DShadesUnknown	= true;		// stav pøepínaèe plynulého stínování neznámı
bool				D3DShadesAkt		= false;	// plynulé stínování aktuálnì zapnuto
bool				D3DShadesGlobal		= true;		// plynulé stínování globálnì povoleno

bool				D3DCullingUnknown	= true;		// stav pøepínaèe odstraòování ploch neznámı
short				D3DCullingAkt		= 0;		// volba zobrazenıch ploch (0=obì, 1=pøední, 2=zadní)
bool				D3DCameraInvert		= false;	// je inverzní kamera, invertovat plošky

bool				D3DMagFilterUnknown[MAX_STAGES]; // stav pøepínaèe filtrace zvìtšenıch textur neznámı
bool				D3DMagFilterAkt[MAX_STAGES];	// filtrace zvìtšenıch textur zapnuta
bool				D3DMagFilterGlobal	= true;		// filtrace zvìtšenıch textur globálnì povolena

bool				D3DMinMipFilterUnknown[MAX_STAGES];	// stav pøepínaèe filtrace zmenšenıch a vzdálenıch textur neznámı
bool				D3DMinFilterAkt[MAX_STAGES];	// filtrace zmenšenıch textur zapnuta
bool				D3DMinFilterGlobal	= true;		// filtrace zmenšenıch textur globálnì povolena
bool				D3DMipFilterAkt[MAX_STAGES];	// filtrace vzdálenıch textur zapnuta
bool				D3DMipFilterGlobal	= true;		// filtrace vzdálenıch textur globálnì povolena

int					D3DAktStage			= 0;		// aktivní stupeò textur (0 a 7)

bool				D3DRamp				= false;	// je ovladaè Ramp

bool				D3DVSync			= false;	// synchronizovat s VSYN

bool				D3DVertFog			= false;	// pouze vektorová mlha (SW driver)

bool				D3DStateAkt			= false;	// aktualizovat stav

bool				D3DReturn			= false;	// blokování Alt+Enter pøi D3D módu

// formáty textur (dwRGBBitCount=0 pro neplatnı formát)
DDPIXELFORMAT		D3DTextFormatA8R8G8B8;			// formát textury D3DFMT_A8R8G8B8
DDPIXELFORMAT		D3DTextFormatA4R4G4B4;			// formát textury D3DFMT_A4R4G4B4
DDPIXELFORMAT		D3DTextFormatA1R5G5B5;			// formát textury D3DFMT_A1R5G5B5
DDPIXELFORMAT		D3DTextFormatR8G8B8;			// formát textury D3DFMT_R8G8B8
DDPIXELFORMAT		D3DTextFormatR5G6B5;			// formát textury D3DFMT_R5G6B5
DDPIXELFORMAT		D3DTextFormatX1R5G5B5;			// formát textury D3DFMT_X1R5G5B5
DDPIXELFORMAT		D3DTextFormatX8R8G8B8;			// formát textury D3DFMT_X8R8G8B8
DDPIXELFORMAT		D3DTextFormatX4R4G4B4;			// formát textury D3DFMT_X4R4G4B4
bool				D3DLowText			= false;	// pouívat niší kvalitu textur
bool				D3DTextFormatOK		= false;	// nalezen platnı formát textur

// formáty Z-bufferù (dwZBufferBitDepth=0 pro neplatnı formát) - jen pro hledání formátu Z bufferu
DDPIXELFORMAT		D3DZBufferFormat24S8;			// formát bufferu 24 bitù, stencil 8 bitù
DDPIXELFORMAT		D3DZBufferFormat24S4;			// formát bufferu 24 bitù, stencil 4 bity
DDPIXELFORMAT		D3DZBufferFormat15S1;			// formát bufferu 15 bitù, stencil 1 bit
DDPIXELFORMAT		D3DZBufferFormat32S0;			// formát bufferu 32 bitù, stencil 0 bitù
DDPIXELFORMAT		D3DZBufferFormat24S0;			// formát bufferu 24 bitù, stencil 0 bitù
DDPIXELFORMAT		D3DZBufferFormat16S0;			// formát bufferu 16 bitù, stencil 0 bitù
DDPIXELFORMAT		D3DZBufferFormat8S0;			// formát bufferu 8 bitù, stencil 0 bitù
bool				D3DZBufferFormatOK	= false;	// nalezen platnı formát Z-bufferu

// stencil buffer
bool				D3DSBufferFormatOK	= false;	// nalezen Stencil buffer
int					D3DSBufferDepth		= 0;		// hloubka Stencil bufferu (bitù)
int					D3DSBufferMax		= 0;		// maximální hodnota Stencil bufferu
bool				D3DSBufferClear		= false;	// poadavek k vymazání Stencil bufferu
bool				D3DShadows			= true;		// povoleno pouívání stínù

// vlastnosti zaøízení
int					MaxTextureWidth		= 0x8000;	// maximální šíøka textury
int					MaxTextureHeight	= 0x8000;	// maximální vıška textury
int					MaxTextureRepeat	= 0x8000;	// maximální poèet opakování textury
bool				SquareTexture		= false;	// textury musí bıt ètvercové
bool				TexturePow2			= true;		// rozmìr textur musí bıt mocnina 2 (pro MipMap je nutné)
bool				IsBlending			= true;		// jsou blending operace
bool				IsMagFilter			= true;		// podporuje filtr pøi zvìtšení
bool				IsMinFilter			= true;		// podporuje filtr pøi zmenšení
bool				IsMipMap			= true;		// podporuje mipmap textury
bool				IsHWRaster			= false;	// je HW rasterizace
bool				IsHWTransLight		= false;	// je HW transformace a osvìtlení
bool				IsPureDevice		= false;	// pouze HW obsluha
bool				AlphaGreaterEqual	= true;		// lze porovnávat Alpha >=
bool				AlphaGreater		= true;		// lze porovnávat Alpha >
int					MaxVertexNum		= 0xf000;	// maximální poèet vrcholù skuteènı
int					MaxVertexNum2		= MAXVERTICES;	// maximální poèet vrcholù pro renderování
int					MaxTextureStages	= MAX_STAGES; // maximální poèet stupòù textur (nepouívá se)
int					MaxTextureSimult	= MAX_STAGES; // maximální poèet textur pro jeden prùchod
bool				IsTableFog			= true;		// podporována tabulková mlha
bool				IsVertexFog			= true;		// podporována vrcholová mlha
bool				IsRangeFog			= true;		// podporována vzdálenostní mlha

int					D3DLevel			= 4;		// sloitost objektù (2 a více)
bool				D3DCreateUpper		= true;		// vytváøet horní podstavu objektù
bool				D3DCreateLower		= true;		// vytváøet dolní podstavu objektù
int					D3D_ID				= 1;		// aktivní objekt (rám) Direct3D
int					TerenID				= -1;		// naposledy zvolenı objekt terénu (-1=není)

double				D3DSmooth			= 1;		// vyhlazení textur (1=ne)

// tabulka ID kódù ovladaèù Direct3D

DEFINE_GUID( IID_IDirect3DHALDevice,    0x84E63dE0,0x46AA,0x11CF,0x81,0x6F,0x00,0x00,0xC0,0x20,0x15,0x6E );
DEFINE_GUID( IID_IDirect3DTnLHalDevice, 0xf5049e78, 0x4861, 0x11d2, 0xa4, 0x7, 0x0, 0xa0, 0xc9, 0x6, 0x29, 0xa8);
DEFINE_GUID( IID_IDirect3DRefDevice,    0x50936643, 0x13e9, 0x11d1, 0x89, 0xaa, 0x0, 0xa0, 0xc9, 0x5, 0x41, 0x29);
DEFINE_GUID( IID_IDirect3DRGBDevice,    0xA4665C60,0x2673,0x11CF,0xA3,0x1A,0x00,0xAA,0x00,0xB9,0x33,0x56 );
DEFINE_GUID( IID_IDirect3DMMXDevice,    0x881949a1,0xd6f3,0x11d0,0x89,0xab,0x00,0xa0,0xc9,0x05,0x41,0x29 );
DEFINE_GUID( IID_IDirect3DRampDevice,   0xF2086B20,0x259F,0x11CF,0xA3,0x1A,0x00,0xAA,0x00,0xB9,0x33,0x56 );
//DEFINE_GUID( IID_IDirect3DNullDevice,	0x8767df22, 0xbacc, 0x11d1, 0x89, 0x69, 0x0, 0xa0, 0xc9, 0x6, 0x29, 0xa8);

const GUID* Dev3DTab[D3DEVICES] =
{
	NULL,							// 0
	&IID_IDirect3DHALDevice,		// 1 HAL
	&IID_IDirect3DTnLHalDevice,		// 2 TnLHal
	&IID_IDirect3DRefDevice,		// 3 REF
	&IID_IDirect3DRGBDevice,		// 4 RGB
	&IID_IDirect3DMMXDevice,		// 5 MMX
	&IID_IDirect3DRampDevice,		// 6 Ramp
//	&IID_IDirect3DNullDevice,		// 7 Null
};

// mìøení uplynulého èasu
double D3DLastTime = 0;							// poslední èas renderování
double D3DElapsedTime = 0;						// uplynulı èas od posledního renderování
double D3DAverageTime = 0;						// støední ubìhlı èas
double D3DAverageFreq = 0;						// støední frekvence
int D3DAverageFreqI = 0;						// frekvence Integer
double D3DAverageRound = 0;						// zaokrouhlovací korekce
int D3DValidTime = 0;							// èítaè do zahájení mìøení èasu

//int	D3DTimeStamp = 0;							// èítaè snímkù (jako èasové razítko)

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------

/////////////////////////////////////////////////////////////////////////////
// globální a lokální promìnné

int			MainFrameStyleEx = (WS_EX_APPWINDOW | WS_EX_CONTROLPARENT | WS_EX_WINDOWEDGE); // rozšíøenı styl
int			MainFrameStyle = (WS_BORDER | WS_CAPTION | /* WS_CLIPCHILDREN | */ WS_MAXIMIZEBOX |
				WS_MINIMIZEBOX | /* WS_CLIPSIBLINGS | */ WS_OVERLAPPED | WS_SYSMENU | WS_THICKFRAME); // styl okna

TCHAR		MainFrameClass[] = _T("PeterProgClass");	// název tøídy hlavního okna
HWND		MainFrame = NULL;						// hlavní okno aplikace
bool		MainFrameVisible = false;				// hlavní okno zobrazeno
HWND		StatusBar = NULL;						// stavová lišta

bool		StatusVisible = false;					// pøíznak viditelnosti stavového øádku
HFONT		StatusFont = NULL;						// font stavového øádku

RECT		ClientRect;								// klientská oblast hlavního okna
int			ClientWidth = 0;						// šíøka klientské oblasti
int			ClientHeight = 0;						// vıška klientské oblasti

int			MainFrameX;								// souøadnice X støedu hlavního okna
int			MainFrameY;								// souøadnice Y støedu hlavního okna

bool		ReqClose = false;						// poadavek uzavøení okna od uivatele

CString		AktCaptionText;							// text titulku hlavního okna
CString		AktStatusText;							// zobrazenı text stavové lišty

HCURSOR		CurArrow;								// (standardní) šipka
HCURSOR		CurArrow2;								// (standardní) šipka pro DirectX

// stav myši (aktualizováno v PreTranslateMessage)
bool		LMouseDown = false;						// levé tlaèítko stisknuto
bool		LMouseClick = false;					// klik levého tlaèítka
bool		LMouseDClick = false;					// dvojklik levého tlaèítka

bool		RMouseDown = false;						// pravé tlaèítko stisknuto
bool		RMouseClick = false;					// klik pravého tlaèítka
bool		RMouseDClick = false;					// dvojklik pravého tlaèítka

POINT		MouseScreen = {MOUSEINV, MOUSEINV};		// souøadnice myši na displeji
POINT		MouseMain = {MOUSEINV, MOUSEINV};		// souøadnice myši v hlavním oknì
bool		MouseValid = false;						// pøíznak aktualizace souøadnice myši

bool		ProgramFill = false;					// plnìní prvku okna programem

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

// buffer hledaného textu v oknì RichEdit
#define		FINDBUFFSIZE 128						// velikost bufferu hledaného textu
LPTSTR		FindBuff = NULL;						// buffer hledaného textu (v oknì RichEdit)
bool		FindMatchCase = false;					// pøi hledání rozlišena velká a malá písmena
bool		FindWholeWord = false;					// hledat jen celá slova
HWND		FindDialogBox = NULL;					// handle na dialog hledání (NULL=není)
UINT		FindMsgString = 0;						// identifikátor zprávy hledání
FINDREPLACE FindStruc;								// struktura pro hledání


// ============================== hlavní okno ===============================

/////////////////////////////////////////////////////////////////////////////
// pøíprava poadovanıch rozmìrù okna podle velikosti hlavní plochy

void AdjustMainFrame(RECT* wrc)
{
// poadované rozmìry plochy
	ASSERT(Map.IsValid(0));
	int width = RoundP(ICONWIDTH * Map[0].Width() * Meritko0);
	int height = RoundP(ICONHEIGHT * Map[0].Height() * Meritko0);
	if (width > ScreenWidth0) width = ScreenWidth0;
	if (height > ScreenHeight0) height = ScreenHeight0;

// zjištìní poadovanıch rozmìrù okna
	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = width;
	rc.bottom = height;
	::AdjustWindowRectEx(&rc, MainFrameStyle, FALSE, MainFrameStyleEx);

// korekce o vıšku stavové lišty
	if (StatusVisible)
	{
		RECT src;
		if (::GetWindowRect(StatusBar, &src))
		{
			rc.bottom += src.bottom - src.top;
		}
	}

// korekce rozmìrù
	width = rc.right - rc.left;
	height = rc.bottom - rc.top;
	if (width > ScreenWidth0) width = ScreenWidth0;
	if (height > ScreenHeight0) height = ScreenHeight0;

// souøadnice
	int left = MainFrameX - width/2;
	if (left < 0) left = 0;
	int top = MainFrameY - height/2;
	if (top < 0) top = 0;

// uloení rozmìrù
	wrc->left = left;
	wrc->top = top;
	wrc->right = left + width;
	wrc->bottom = top + height;
}

/////////////////////////////////////////////////////////////////////////////
// první zobrazení hlavního okna

void MainFrameShow()
{
	if (!MainFrameVisible)
	{
		MainFrameVisible = true;
		OnSize(&Win[0]);
		::ShowWindow(MainFrame, SW_SHOW);
		::SetFocus(MainFrame);
	}
}


/////////////////////////////////////////////////////////////////////////////
// vytvoøení hlavního okna aplikace

bool MainFrameCreate()
{
// text jména programu
	for (int i = BufObjN-1; i >= 0; i--)
	{
		if ((BufObj[i].DatBlok == BufLocID) && (BufObj[i].DatIndex == 0))
		{
			AktCaptionText = Text.Get(BufObj[i].Name);
			break;
		}
	}

// naètení standardních barev okenních prvkù
	StdColorBtnText = (::GetSysColor(COLOR_BTNTEXT) & 0xffffff);
	StdColorBtnFace = (::GetSysColor(COLOR_BTNFACE) & 0xffffff);
	StdBrushBtn = ::CreateSolidBrush(StdColorBtnFace);

	StdColorWindowText = (::GetSysColor(COLOR_WINDOWTEXT) & 0xffffff);
	StdColorWindow = (::GetSysColor(COLOR_WINDOW) & 0xffffff);
	StdBrushWindow = ::CreateSolidBrush(StdColorWindow);

	StdColorHighlightText = (::GetSysColor(COLOR_HIGHLIGHTTEXT) & 0xffffff);
	StdColorHighlight = (::GetSysColor(COLOR_HIGHLIGHT) & 0xffffff);
	StdBrushHighlight = ::CreateSolidBrush(StdColorHighlight);

// registrace tøídy okna
	WNDCLASS wcl;
	wcl.style = CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW;// | CS_OWNDC;
	wcl.lpfnWndProc = WindowProc;
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 4;
	wcl.hInstance = hInstance;
	wcl.hIcon = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDN_MAINFRAME));
	wcl.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wcl.hbrBackground = NULL;
	wcl.lpszMenuName = NULL;
	wcl.lpszClassName = MainFrameClass;
	::RegisterClass(&wcl);

// vytvoøení hlavního okna
	RECT rc;
	AdjustMainFrame(&rc);

	MainFrame = ::CreateWindowEx(
		MainFrameStyleEx,								// rozšíøenı styl
		MainFrameClass,									// jméno tøídy okna
		AktCaptionText,									// titulek okna
		MainFrameStyle,									// styl okna
		rc.left,										// X
		rc.top,											// Y
		rc.right - rc.left,								// šíøka
		rc.bottom - rc.top,								// vıška
		NULL,											// rodiè
		NULL,											// menu
		hInstance,										// instance
		NULL);											// parametry
	if (MainFrame == NULL) return false;

// aktualizace informací o displeji
	AktDispInfo();

// vytvoøení prvku okna
	Win.New();
	Win[0].Wnd = MainFrame;
	Win[0].Parent = -1;
	Win[0].Typ = WINITEM_WINDOW;
	Win[0].Text = AktCaptionText;
	Win[0].Show = true;
	Win[0].FontCol = StdColorBtnText;
	Win[0].FontBack = StdColorBtnFace;
	Win[0].FontBrush = StdBrushBtn;
	Win[0].HasBorder = true;
	Win[0].HasCaption = true;
	Win[0].HasSize = true;
	Win[0].MayClose = true;
	Win[0].AlwaysTop = false;
	::SetWindowLong(MainFrame, 0, 0);

// vytvoøení stavové lišty - zatím nezobrazena
#define IDN_STATUS 12524
	StatusBar = ::CreateStatusWindow(
		WS_CHILD | SBARS_SIZEGRIP,
		AktStatusText,
		MainFrame,
		IDN_STATUS);
	if (StatusBar == NULL) return false;

// naètení kurzorù myši
	CurArrow = ::LoadCursor(0, MAKEINTRESOURCE(IDC_ARROW)); ASSERT(CurArrow != NULL);
	CurArrow2 = ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_ARROW2)); ASSERT(CurArrow2 != NULL);
	EmptyIconData.Cursor = CurArrow;

// aktualizace klientské oblasti
	InitClientRect(&Win[0]);

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// pøepoèet rozmìrù okna Direct3D (podle údajù zadanıch uivatelem)
// (musí bıt zobrazené okno, aby byly platné údaje o oknì)
// vrací TRUE=má bıt okno 3D

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

bool RecalcD3D()
{
// pøíprava rozmìrù omezenıch na grafickou plochu
	D3DX = D3DX0;
	D3DY = D3DY0;
	D3DW = D3DW0;
	D3DH = D3DH0;

// omezení poèátku X a Y okna na grafickou plochu
	if (D3DX < 0) D3DX = 0;
	if (D3DX > Width) D3DX = Width;
	if (D3DY < 0) D3DY = 0;
	if (D3DY > Height) D3DY = Height;

// omezení šíøky a vıšky okna na grafickou plochu
	if (D3DW < 0) D3DW = 0;
	if (D3DH < 0) D3DH = 0;
	if ((D3DX + D3DW) > Width) D3DW = Width - D3DX;
	if ((D3DY + D3DH) > Height) D3DH = Height - D3DY;

// test, zda bude zobrazena 2D grafika
	D3D2D = ((D3DW != Width) || (D3DH != Height));

// pøíprava mìøítka a souøadnic
	double mer = Meritko;
	int left = DispLeft;
	int top = DispTop;
	int height = DispHeight;

	if (DialogMode && !DialogGraph)
	{
		mer = 1;
		left = 0;
		top = 0;
		height = ClientHeight;
	}

// vıpoèet skuteènıch souøadnic okna Direct3D
	D3DLeft = RoundM(D3DX0 * mer) + left;
	if (D3DLeft < 0) D3DLeft = 0;
	if (D3DLeft > ClientWidth) D3DLeft = ClientWidth;

	D3DTop = RoundM(D3DY0 * mer) + (ClientHeight - top - height);
	if (D3DTop < 0) D3DTop = 0;
	if (D3DTop > ClientHeight) D3DTop = ClientHeight;

	D3DWidth = RoundM(D3DW0 * mer);
	if (D3DWidth < 0) D3DWidth = 0;
	if ((D3DLeft + D3DWidth) > ClientWidth) D3DWidth = ClientWidth - D3DLeft;

	D3DHeight = RoundM(D3DH0 * mer);
	if (D3DHeight < 0) D3DHeight = 0;
	if ((D3DTop + D3DHeight) > ClientHeight) D3DHeight = ClientHeight - D3DTop;

	D3DWidthHeight2 = sqrt((double)D3DWidth/D3DHeight);

	D3DTop = ClientHeight - D3DTop - D3DHeight;

// test, zda bude celá plocha hlavního okna
	D3DFull = ((D3DWidth == ClientWidth) && (D3DHeight == ClientHeight) && !StatusVisible);

// test, zda bude okno Direct3D zapnuto
	return ((D3DWidth != 0) && (D3DHeight != 0));
}

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------


// ================================ videomód ================================

/////////////////////////////////////////////////////////////////////////////
// setøídìní videomódù displeje (na zaèátku je nejmenší rozlišení a nejniší poèet bitù)

void SortDisplayModes(VIDEOMODEITEM* tab, int num)
{
	VIDEOMODEITEM* ti = tab;

	for (int i = num; i > 0; i--)
	{
		int wi = ti->Width;
		int hi = ti->Height;
		int bi = ti->Bits;
//		D3DFORMAT fi = ti->Format;

		VIDEOMODEITEM* tj = ti + 1;

		for (int j = i - 1; j > 0; j--)
		{
			int wj = tj->Width;
			int hj = tj->Height;
			int bj = tj->Bits;
//			D3DFORMAT fj = tj->Format;
			
			if ((wi > wj) ||
				(
					(wi == wj) &&
					(
						(hi > hj) || 
						(
							(hi == hj) && (bi > bj)
						)
					)
				)
			   )
			{
				tj->Width = wi;
				wi = wj;
				ti->Width = wi;

				tj->Height = hi;
				hi = hj;
				ti->Height = hi;

				tj->Bits = bi;
				bi = bj;
				ti->Bits = bi;

//				tj->Format = fi;
//				fi = fj;
//				ti->Format = fi;
			}

			tj++;
		}

		ti++;
	}
}

/////////////////////////////////////////////////////////////////////////////
// inicializace seznamu videomódù (volá se pøi prvním pouití)

void InitVideoModes()
{

// test, zda byl seznam ji sestaven
	if (!IsVideoModes)
	{
		IsVideoModes = true;

// vytvoøení bufferu seznamu videomódù
		TabVideoModes = (VIDEOMODEITEM*)MemGet(MAXVIDEOMODES * sizeof(VIDEOMODEITEM));

// zjištìní všech videomódù
		int inx = 0;

		for (; NumVideoModes < MAXVIDEOMODES; inx++)
		{
			DEVMODE mode;
			MemFill(&mode, sizeof(DEVMODE), 0);
			mode.dmSize = sizeof(DEVMODE);
			mode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
			if (::EnumDisplaySettings(NULL, inx, &mode) == 0) break;

// test platnosti videomódu
			int width = mode.dmPelsWidth;
			int height = mode.dmPelsHeight;
			int bits = mode.dmBitsPerPel;

			if ((width >= 160) &&
				(width <= 10000) &&
				(height >= 100) &&
				(height <= 10000) &&
				((bits == 8) ||
				 (bits == 16) ||
				 (bits == 24) ||
				 (bits == 32)))
			{

// test, zda videomód ji existuje
				VIDEOMODEITEM* item = TabVideoModes;

				for (int i = NumVideoModes; i > 0; i--)
				{
					if ((item->Width == width) &&
						(item->Height == height) &&
						(item->Bits == bits))
					{
						break;
					}
					item++;
				}

// je to novı videomód
				if (i == 0)
				{
					item->Width = width;
					item->Height = height;
					item->Bits = bits;
					NumVideoModes++;
				}
			}
		}

// setøídìní videomódù
		SortDisplayModes(TabVideoModes, NumVideoModes);
	}
}

/////////////////////////////////////////////////////////////////////////////
// nalezení nejblišího videomódu v tabulce (vrací -1=nenalezeno)
// (0=parametr je implicitní)

int FindVideoMode(int width, int height, int bits)
{
// kontrola poètu videomódù
	int tabn = NumVideoModes;
	if (tabn <= 0) return -1;

// implicitní šíøka a vıška
	if (width == 0)
	{
		if (height == 0)
		{
			width = ICONWIDTH * Map[0].Width();
			height = ICONHEIGHT * Map[0].Height();
		}
		else
		{
			width = height * Map[0].Width() / Map[0].Height();
		}
	}
	if (height == 0) height = width * Map[0].Height() / Map[0].Width();

// omezení šíøky a vıšky na maximální videomód (aby byl nalezen alespoò 1 videomód)
	tabn -= 1;
	VIDEOMODEITEM* tab = TabVideoModes + tabn;
	if (width > tab->Width) width = tab->Width;
	if (height > tab->Height) height = tab->Height;

// nalezení nejvhodnìjšího videomódu v tabulkách (nejbliší vyšší)
	int dist = 1000000;
	int inx = -1;

	for (; tabn >= 0; tabn--)
	{
		if ((tab->Width >= width) &&
			(tab->Height >= height) &&
			((tab->Bits == bits) || (bits == 0)))
		{
			int d = (tab->Width - width) +
					(tab->Height - height);

			if (bits == 0)
			{
				switch (tab->Bits)
				{
				case 8:
					d += 3;
					break;

				case 16:
					d += 2;
					break;

				case 24:
					d += 1;
					break;

				case 32:
					break;
				}
			}

			if (d < dist)
			{
				dist = d;
				inx = tabn;
			}
		}
		tab--;
	}

	return inx;
}

/////////////////////////////////////////////////////////////////////////////
// stop aktivního videomódu (voláno i po neúspìšném pokusu o celoobrazovkovı reim)

void StopVideoMode()
{
// ukonèení 3D módu
#ifndef _MINI
	if (FullScreen && D3D) DeSelectD3DDev();
#endif

// zapnutí módu zmìny okna
	Resizing = true;

// vypnutí kurzoru myši
	::ShowCursor(FALSE);

// ukonèení videomódu
	if (FullScreen)
	{
		::ChangeDisplaySettings(NULL, 0);
	}

// obnovení stavového øádku
	StatusAkt = true;

// návrat pozice okna
	::SetWindowLong(MainFrame, GWL_EXSTYLE, MainFrameStyleEx);
	::SetWindowLong(MainFrame, GWL_STYLE, MainFrameStyle | WS_VISIBLE);

	if (FullScreen)
	{
		FullScreen = false;
		AktDispInfo();

		if (StatusText.IsNotEmpty() && OldMaximized) StatusVisible = true;
		RECT rc;
		AdjustMainFrame(&rc);
		StatusVisible = false;

		::SetWindowPos(MainFrame, HWND_NOTOPMOST, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
			SWP_DRAWFRAME | SWP_FRAMECHANGED | SWP_NOCOPYBITS | SWP_SHOWWINDOW);

		if (OldMaximized) ::ShowWindow(MainFrame, SW_SHOWMAXIMIZED);
	}

// zapnutí kurzoru myši
	::ShowCursor(TRUE);

// vypnutí módu zmìny okna
	Resizing = false;

// aktualizace informací o oknì	
	InitClientRect(&Win[0]);
	OnSize(&Win[0]);

// poadavek k pøekreslení 2D grafiky
	AktAllIcon();

// start 3D módu
#ifndef _MINI
	if (!D3D) UserD3DDevice();
#endif

//	::ShowCursor(FALSE);
//	::ShowCursor(TRUE);
//	::SetCursor(CurArrow2);
//	::SetCursor(CurArrow);

// aktualizace kurzoru
//	::SetCursor(NULL);
//	::SetCursor(CurArrow);
}

/////////////////////////////////////////////////////////////////////////////
// start videomódu (vrací TRUE=OK)

bool StartVideoMode()
{

// ukonèení 3D módu, pøechodnì vypnuté 3D okno
#ifndef _MINI
	if (D3D) DeSelectD3DDev();
	int oldX = D3DW0;
	D3DW0 = 0;
#endif

// ukonèení pøedešlého videoreimu
	if (FullScreen) StopVideoMode();

// vypnutí kurzoru myši
	::ShowCursor(FALSE);

// hlavní okno musí bıt zapnuto
	MainFrameShow();

// vypnutí stavové lišty (nesmí se teï aktualizovat, zùstal by dole prázdnı pruh)
	StatusAkt = false;
	SetStatusVisible(false);

// úschova parametrù hlavního okna
	if (!FullScreen)
	{
		OldMaximized = ::IsZoomed(MainFrame);
	}

// korekce parametrù hlavního okna na fullscreen
	::SetWindowLong(MainFrame, GWL_EXSTYLE, WS_EX_CONTROLPARENT);
	::SetWindowLong(MainFrame, GWL_STYLE, WS_POPUP | WS_VISIBLE);

// inicializace seznamu videomódù
	InitVideoModes();

// poadavek k pøekreslení 2D grafiky
	AktAllIcon();

// zapnutí módu zmìny okna
	Resizing = true;

// nalezení videomódu
	int inx = FindVideoMode(UserScreenWidth, UserScreenHeight, UserScreenBits);
	if (inx < 0) inx = FindVideoMode(UserScreenWidth, UserScreenHeight, 0);

	bool isup = true;

// cyklus k niším videomódùm
	for (; inx >= 0; )
	{

// popisovaè videomódu
		VIDEOMODEITEM* item = TabVideoModes + inx;

// start videomódu
		DEVMODE dm;
		MemFill(&dm, sizeof(dm), 0);
		dm.dmSize = sizeof(dm);
		dm.dmBitsPerPel = item->Bits;
		dm.dmPelsWidth = item->Width;
		dm.dmPelsHeight = item->Height;
		dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

		if (::ChangeDisplaySettings(&dm, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL)
		{
			FullScreenWidth = item->Width;
			FullScreenHeight = item->Height;
			FullScreenBits = item->Bits;

// úschova parametrù videomódu
			ScreenWidth = item->Width;
			ScreenHeight = item->Height;
			ScreenWidth0 = ScreenWidth;
			ScreenHeight0 = ScreenHeight;
			ScreenBits = item->Bits;

// korekce pozice okna
			::SetWindowPos(MainFrame, HWND_TOPMOST, 0, 0, ScreenWidth, ScreenHeight,
					SWP_DRAWFRAME | SWP_FRAMECHANGED | SWP_NOCOPYBITS | SWP_SHOWWINDOW);

// zapnutí kurzoru myši
			::ShowCursor(TRUE);

// vypnutí módu zmìny okna
			Resizing = false;

// zapnutí pøíznaku celoobrazovkového módu
			FullScreen = true;

// aktualizace informací o oknì
			InitClientRect(&Win[0]);
			OnSize(&Win[0]);

// start 3D módu
#ifndef _MINI
#ifndef _MINI
			D3DW0 = oldX;
#endif

			if (!RecalcD3D() || UserD3DDevice())
			{
				StatusAkt = true;
				return true;
			}

// pokraèují další pokusy
			Resizing = true;
			::ShowCursor(FALSE);
			D3DW0 = 0;
#else
			StatusAkt = true;
			return true;
#endif
		}

		if (isup)
		{
			inx++;
			if ((inx >= NumVideoModes) ||
				(TabVideoModes[inx].Width != item->Width) ||
				(TabVideoModes[inx].Height != item->Height))
			{
				inx--;
				inx--;
				isup = false;
			}
		}
		else
		{
			inx--;
		}
	}

// zapnutí kurzoru myši
	::ShowCursor(TRUE);

// vypnutí módu zmìny okna
	Resizing = false;
	StatusAkt = true;

// návrat parametrù hlavního okna
#ifndef _MINI
	D3DW0 = oldX;
#endif
	StopVideoMode();

	return false;
}

/////////////////////////////////////////////////////////////////////////////
// nastavení/znovunastavení implicitního videomódu

void ReSetDisplayMode()
{
	StartVideoMode();
}

/////////////////////////////////////////////////////////////////////////////
// zapnutí/vypnutí celoobrazovkového módu

void SetFullScreen(bool full)
{
	if (full != FullScreen)
	{
		if (full)
		{
			ReSetDisplayMode();
		}
		else
		{
			StopVideoMode();
		}
	}
}

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

/////////////////////////////////////////////////////////////////////////////
// nastavení módu vyplòování ploch

void SetD3DWireframe(bool on)
{
	if (!D3DWireframeGlobal) on = false;

	if (D3DWireframeUnknown || (on != D3DWireframeAkt))
	{
		D3DWireframeAkt = on;
		D3DWireframeUnknown = false;

		pD3AktWireframe();
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení módu zapnutí osvìtlení

void SetD3DLightOn(bool on)
{
	if (!D3DLightOnGlobal) on = false;

	if (D3DLightOnUnknown || (on != D3DLightOnAkt))
	{
		D3DLightOnAkt = on;
		D3DLightOnUnknown = false;

		pD3AktLightOn();
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení módu plynulého stínování

void SetD3DShades(bool on)
{
	if (!D3DShadesGlobal) on = false;

	if (D3DShadesUnknown || (on != D3DShadesAkt))
	{
		D3DShadesAkt = on;
		D3DShadesUnknown = false;

		pD3AktShades();
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení módu filtrace textur pøi zvìtšení

void SetD3DMagFilter(bool on, int stage)
{
	if (!D3DMagFilterGlobal || D3DRamp || !IsMagFilter) on = false;

	if (D3DMagFilterUnknown[stage] || (on != D3DMagFilterAkt[stage]))
	{
		D3DMagFilterAkt[stage] = on;
		D3DMagFilterUnknown[stage] = false;

		pD3AktMagFilter(stage);
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení módu filtrace textur pøi zmenšení

void SetD3DMinMipFilter(bool min, bool mip, int stage)
{
	if (!D3DMinFilterGlobal || D3DRamp || !IsMinFilter) min = false;
	if (!D3DMipFilterGlobal || D3DRamp || !IsMipMap) mip = false;

	if (D3DMinMipFilterUnknown[stage] || (min != D3DMinFilterAkt[stage]) || (mip != D3DMipFilterAkt[stage]))
	{
		D3DMinFilterAkt[stage] = min;
		D3DMipFilterAkt[stage] = mip;
		D3DMinMipFilterUnknown[stage] = false;

		pD3AktMinMipFilter(stage);
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení odstraòování ploch

void SetD3DCullingKorig(short val)
{
	if (D3DCameraInvert && (val != 0)) val = (short)(val ^ 3);
	SetD3DCulling(val);
}

void SetD3DCulling(short val)
{
	if (D3DCullingUnknown || (val != D3DCullingAkt))
	{
		D3DCullingAkt = val;
		D3DCullingUnknown = false;

		pD3AktCulling();
	}
}

/////////////////////////////////////////////////////////////////////////////
// zapnutí mlhy

void D3DCheckFogOn()
{
	FogOn = ((FogColor != MAXDWORD) && (IsTableFog || IsVertexFog) && !D3DRamp);
}

void SetD3DFogOn(BOOL on)
{
	if (!FogOn) on = FALSE;

	if (on != D3DFogOn)
	{
		D3DFogOn = on;
		pD3FogOn(on);
	}
}

/////////////////////////////////////////////////////////////////////////////
// resetování pøepínaèù nastavení 

void ResetD3DSwitch()
{
	D3DWireframeUnknown	= true;		// stav pøepínaèe vyplòování ploch neznámı
	D3DLightOnUnknown	= true;		// stav pøepínaèe osvìtlení neznámı
	D3DShadesUnknown	= true;		// stav pøepínaèe plynulého stínování neznámı
	D3DCullingUnknown	= true;		// stav pøepínaèe odstraòování ploch neznámı
	D3DFogOn = -1;					// stav zapnutí mlhy neznámı

	D3DSrcBlend = BLEND_INVALID; // aktuální nastavení blending operace zdroje
	D3DDstBlend = BLEND_INVALID; // aktuální nastavení blending operace cíle
	D3DAlphaRef = -1;			// aktuální nastavení alfa úrovnì
	D3DTFactor = MAXDWORD;			// aktuální nastavení faktoru textur
	D3DZWrite = -1;				// aktuální hodnota hloubkového zápisu
	D3DZTest = -1;				// aktuální hodnota hloubkového testu

	for (int i = 0; i < MAX_STAGES; i++)
	{
		D3DMagFilterUnknown[i]	= true;		// stav pøepínaèe filtrace zvìtšenıch textur neznámı
		D3DMinMipFilterUnknown[i] = true;	// stav pøepínaèe filtrace zmenšenıch a vzdálenıch textur neznámı
		D3DColorOp[i] = -1;				// aktuální nastavení operací barev
		D3DColorArg1[i] = -1;			// aktuální nastavení argumentu 1 operací barev
		D3DColorArg2[i] = -1;			// aktuální nastavení argumentu 2 operací barev
		D3DAlphaOp[i] = -1;				// aktuální nastavení operací barev
		D3DAlphaArg1[i] = -1;			// aktuální nastavení argumentu 1 operací barev
		D3DAlphaArg2[i] = -1;			// aktuální nastavení argumentu 2 operací barev
		D3DAddressU[i] = -1;			// aktuální adresování textur U
		D3DAddressV[i] = -1;			// aktuální adresování textur V
//		D3DBorder[i] = -1;				// aktuální barva okolí textur
		D3DBias[i] = -123456;			// aktuální zjemnìní vzdálenıch textur
	}
}

/////////////////////////////////////////////////////////////////////////////
// inicializace objektù scény (volá se pøi startu programu, pøed inicializací 3D!)

void Init3D()
{
// inicializace bufferù skupin
	int i;
	for (i = 0; i < MAX_RENDGROUP; i++)
	{
		RendSort[i] = FALSE;
		RendItems[i] = NULL;
	}

	RendSort[8] = TRUE;
	RendSort[9] = TRUE;
	RendSort[10] = TRUE;
	RendSort[11] = TRUE;

// implicitní materiál
	D3DMATERIAL8 mt;
	MemFill(&mt, sizeof(D3DMATERIAL8), 0);

	mt.Diffuse.r = 1;
	mt.Diffuse.g = 1;
	mt.Diffuse.b = 1;
	mt.Diffuse.a = 1;

	mt.Ambient.r = 1;
	mt.Ambient.g = 1;
	mt.Ambient.b = 1;
	mt.Ambient.a = 1;

	mt.Specular.a = 1;

	mt.Emissive.a = 1;

	D3DM_Add(&mt);

// inicializace pohledové matice
	Matrix1(&D3DViewMatrix);

// vytvoøení objektu scény
	i = D3DF_New();
	ASSERT(i == 0);

// vytvoøení objektu kamery
	i = D3DF_New();
	ASSERT(i == 1);
	D3DFITEM* item = D3DF_Get(i);
	item->Type = D3DFTYPE_CAMERA;
	item->TransZ = -2;
	item->AktOwn = true;

// vytvoøení implicitního svìtla
	i = D3DF_New();
	ASSERT(i == 2);
	item = D3DF_Get(i);
	item->Type = D3DFTYPE_LIGHT;
	item->Light = D3DL_New();
	D3DL_Get(item->Light)->Frame = i;
	D3DL_Get(item->Light)->Light.Type = D3DLIGHT_DIRECTIONAL;

	item->RotateX = uhel45;
	item->SinX = sin(uhel45);
	item->CosX = cos(uhel45);
	item->IsRotX = true;

	item->RotateY = uhel45;
	item->SinY = item->SinX;
	item->CosY = item->CosX;
	item->IsRotY = true;

	item->TransX = -10;
	item->TransY = 10;
	item->TransZ = -10;

	item->AktOwn = true;
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace viewportu (napø. po zmìnì velikosti okna)
/*
bool UpdateViewport()
{
	return false;


	if (Resizing) return true;

	RecalcD3D();
	FPUFloat();
	D3DProjAkt = true;
	D3DViewAkt = true;

// reset pøepínaèù (nìkteré ztratí hodnotu)
	ReSet3DCount = 0;				// resetování opakované inicializace
	ReSet3DCountN = 1;				// inicializace pøíštího èítaèe
	ReSet3D();

	if (Direct3DDev != NULL)
	{  

		D3DParam.BackBufferWidth = D3DWidth;
		D3DParam.BackBufferHeight = D3DHeight;

		if (D3DBackBuf != NULL)
		{
			D3DBackBuf->Release();
			D3DBackBuf = NULL;
		}

		D3DM_SoftReset();
		D3DF_SoftReset();
		D3DL_SoftReset();
		D3DT_SoftReset();
		
		Direct3DDev->Reset(&D3DParam);

		D3DVIEWPORT8 v;
		v.X = 0;
		v.Y = 0;
		v.Width = D3DWidth;
		v.Height = D3DHeight;
		v.MinZ = 0;
		v.MaxZ = 1;

		Direct3DDev->SetViewport(&v);
	}

	if (D3DGL)
	{
		D3DM_SoftReset();
		D3DF_SoftReset();
		D3DL_SoftReset();
		D3DT_SoftReset();

//		if (GLContext)
//		{
//			pwglMakeCurrent(GLDC, NULL);
//			pwglDeleteContext(GLContext);
//			GLContext = pwglCreateContext(GLDC);
//			if (GLContext) pwglMakeCurrent(GLDC, GLContext);
//		}

		pglViewport(0, 0, D3DWidth, D3DHeight);

//		DeSelectD3DDev();
//		UserD3DDevice();

	}

	FPUDouble();

//	if (D3View != NULL)
//	{
//		return (D3View->Configure(0, 0, D3DWidth, D3DHeight) == D3DRM_OK);
//	}
	return true;
}
*/

/////////////////////////////////////////////////////////////////////////////
// opakovaná inicializace vıchozího stavu 3D

int ReSet3DCount = 0;
int ReSet3DCountN = 1;

void ReSet3D()
{
	if (--ReSet3DCount < 0)
	{ 

// obnovení èítaèe
		int i = ReSet3DCountN;
		ReSet3DCount = i;
		i = (i << 1);
		if (i > 256) i = 256;
		ReSet3DCountN = i;

// restart pøepínaèù
		ResetD3DSwitch();

// opakovanì aktualizovat
		D3DProjAkt = true;				// aktualizovat projekèní matici
		D3DViewAkt = true;				// aktualizovat pohledovou matici
		D3DFrustumAkt = true;			// aktualizovat frustum
		D3DBackColAkt = true;			// aktualizovat barvu pozadí
		FogAkt = true;					// aktualizovat mlhu
		D3DAmbientColAkt = true;		// aktualizovat ambient osvìtlení
		D3DStateAkt = true;				// aktualizovat stav
	}
}				

/////////////////////////////////////////////////////////////////////////////
// uvolnìní ovladaèe Direct3D

void DeSelectD3DDev()
{
// vypnutí kurzoru myši
	::ShowCursor(FALSE);

	FPUFloat();

// èekání na dokonèení renderování
	D3DVSync = false;
	pD3Disp();

// resetování bufferù
	D3DM_Reset();
	D3DF_Reset();
	D3DL_Reset();
	D3DT_Reset();

// ukonèení funkce aktivního zaøízení
	pD3Stop();
	FPUDouble();

// zrušení èísla aktivního zaøízení
	D3DIntAkt = 0;
	D3DDevAkt = 0;
	D3D = false;

// nulování èasù (pro uivatele)
	D3DElapsedTime = 0;
	D3DAverageFreqI = 0;

// implicitní informace o zaøízení
	MaxTextureWidth = 0x8000;
	MaxTextureHeight = 0x8000;
	MaxTextureRepeat = 0x8000;
	SquareTexture = false;
	TexturePow2 = true;
	IsBlending = true;
	IsMagFilter = true;
	IsMinFilter = true;
	IsMipMap = true;
	IsHWRaster = false;
	IsHWTransLight = false;
	IsPureDevice = false;
	AlphaGreaterEqual = true;
	AlphaGreater = true;
	MaxVertexNum = 0xf000;
	MaxVertexNum2 = MAXVERTICES;
	MaxTextureStages = MAX_STAGES;
	MaxTextureSimult = MAX_STAGES;
	IsTableFog = true;
	IsVertexFog = true;
	IsRangeFog = true;
	D3DSBufferFormatOK = false;
	D3DSBufferClear = false;
	D3DShadows = true;

// nulování adres zaøízení
	pD3Stop = D3NoStop;
	pD3SizeView = D3NoSizeView;
	pD3MoveView = D3NoMoveView;
	pD3Disp = D3NoDisp;
	pD3Clear = D3NoClear;
	pD3AktProj = D3NoAktProj;
	pD3AktView = D3NoAktView;
	pD3Begin = D3NoBegin;
	pD3End = D3NoEnd;
	pD3Render = D3NoRender;
	pD3Reset = D3NoReset;
	pD3SReset = D3NoSReset;
	pD3AktWireframe = D3NoAktWireframe;
	pD3AktLightOn = D3NoAktLightOn;
	pD3AktShades = D3NoAktShades;
	pD3AktCulling = D3NoAktCulling;
	pD3AktMagFilter = D3NoAktMagFilter;
	pD3AktMinMipFilter = D3NoAktMinMipFilter;
	pD3AktLight = D3NoAktLight;
	pD3SetMaterial = D3NoSetMaterial;
	pD3TextureRes = D3NoTextureRes;
	pD3TextureSet = D3NoTextureSet;
	pD3TextureOn = D3NoTextureOn;
	pD3TextureOff = D3NoTextureOff;
	pD3MatReset = D3NoMatReset;
	pD3LightReset = D3NoLightReset;
	pD3AktAmbient = D3NoAktAmbient;
	pD3Free = D3NoFree;
	pD3AktFog = D3NoAktFog;
	pD3AktState = D3NoAktState;
	pD3AktPalette = D3NoAktPalette;
	pD3SetColorOp = D3NoSetColorOp;
	pD3SetAlphaOp = D3NoSetAlphaOp;
	pD3SetAddress = D3NoSetAddress;
//	pD3SetBorder = D3NoSetBorder;
	pD3SetBias = D3NoSetBias;
	pD3SetTFactor = D3NoSetTFactor;
	pD3SetZFunc = D3NoSetZFunc;
	pD3SetZWrite = D3NoSetZWrite;
	pD3SEnable = D3NoSEnable;
	pD3SetSOp = D3NoSetSOp;
	pD3SetSFunc = D3NoSetSFunc;
	pD3SetSMask = D3NoSetSMask;
	pD3Shadow = D3NoShadow;
	pD3Shadows = D3NoShadows;

// poadavek k pøekreslení 2D grafiky
	AktAllIcon();

// obnovení palet
	pD3AktPalette(MainFrame);

// zapnutí kurzoru myši
	::ShowCursor(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// pøidání formátu textur

void D3DEnumPixelFormat(DDPIXELFORMAT* pf)
{
	if ((pf->dwFlags & DDPF_RGB) != 0)
	{
		DDPIXELFORMAT* dst = NULL;

		bool alpha = ((pf->dwFlags & DDPF_ALPHAPIXELS) != 0);
		if (pf->dwRGBAlphaBitMask == 0) alpha = false;

		switch (pf->dwRGBBitCount)
		{
		case 32:
			dst = &D3DTextFormatX8R8G8B8;
			if (alpha) dst = &D3DTextFormatA8R8G8B8;
			break;

		case 24:
			dst = &D3DTextFormatR8G8B8;
			break;

		case 16:
		case 15:
			if (pf->dwBBitMask == 0xf)
			{
				dst = &D3DTextFormatX4R4G4B4;
				if (alpha) dst = &D3DTextFormatA4R4G4B4;
			}
			else
			{
				if (pf->dwGBitMask == 0x7e0)
				{
					dst = &D3DTextFormatR5G6B5;
				}
				else
				{
					dst = &D3DTextFormatX1R5G5B5;
					if (alpha) dst = &D3DTextFormatA1R5G5B5;
				}
			}
		}

		if (dst != NULL)
		{
			D3DTextFormatOK = true;
			MemCopy(dst, pf, sizeof(DDPIXELFORMAT));
			dst->dwSize = sizeof(DDPIXELFORMAT);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// pøidání formátu Z-bufferu

void D3DEnumZBufferFormat(DDPIXELFORMAT* pf)
{
	if ((pf->dwFlags & DDPF_ZBUFFER) != 0)
	{
		DDPIXELFORMAT* dst = NULL;

		int zbits = pf->dwZBufferBitDepth;

		if (pf->dwFlags & DDPF_STENCILBUFFER) zbits += 64 * pf->dwStencilBitDepth;

		switch (zbits)
		{
		case 32 + 64*8:
			dst = &D3DZBufferFormat24S8;
			break;

		case 32 + 64*4:
			dst = &D3DZBufferFormat24S4;
			break;

		case 16 + 64*1:
			dst = &D3DZBufferFormat15S1;
			break;

		default:

			zbits &= 0x3f;

			switch (zbits)
			{
			case 32:
				dst = &D3DZBufferFormat32S0;
				break;

			case 24:
				dst = &D3DZBufferFormat24S0;
				break;

			case 16:
			case 15:
				dst = &D3DZBufferFormat16S0;
				break;

			case 8:
				dst = &D3DZBufferFormat8S0;
				break;
			}
		}

		if (dst != NULL)
		{
			D3DZBufferFormatOK = true;
			MemCopy(dst, pf, sizeof(DDPIXELFORMAT));
			dst->dwSize = sizeof(DDPIXELFORMAT);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// vıbìr ovladaèe 3D (vrací TRUE=inicializace OK)
// inter = rozhraní (1=GL0,2=GL1,3=GL2,4=DX3,5=DX5,6=DX6,7=DX7,8=DX8)
// device = ovladaè (1=HAL,2=TnLHal,3=REF,4=RGB,5=MMX,6=Ramp)

bool SelectD3DDevice(int inter, int dev)
{
	D3DIntAkt = inter;
	D3DDevAkt = dev;
	D3DRamp = (dev == NRampDev);

	D3DTextFormatA8R8G8B8.dwRGBBitCount=0;
	D3DTextFormatA4R4G4B4.dwRGBBitCount=0;
	D3DTextFormatA1R5G5B5.dwRGBBitCount=0;
	D3DTextFormatR8G8B8.dwRGBBitCount=0;
	D3DTextFormatR5G6B5.dwRGBBitCount=0;
	D3DTextFormatX1R5G5B5.dwRGBBitCount=0;
	D3DTextFormatX8R8G8B8.dwRGBBitCount=0;
	D3DTextFormatX4R4G4B4.dwRGBBitCount=0;

	D3DTextFormatOK = false;

	D3DZBufferFormat24S8.dwZBufferBitDepth = 0;
	D3DZBufferFormat24S4.dwZBufferBitDepth = 0;
	D3DZBufferFormat15S1.dwZBufferBitDepth = 0;
	D3DZBufferFormat32S0.dwZBufferBitDepth = 0;
	D3DZBufferFormat24S0.dwZBufferBitDepth = 0;
	D3DZBufferFormat16S0.dwZBufferBitDepth = 0;
	D3DZBufferFormat8S0.dwZBufferBitDepth = 0;

	D3DZBufferFormatOK = false;

	D3DSBufferFormatOK = false;
	D3DSBufferDepth = 0;
	D3DSBufferMax = 0;
	D3DSBufferClear = false;

	switch (inter)
	{
	case NGL0Int:	return D3GL0Start(dev);
	case NGL1Int:	return D3GL1Start(dev);
	case NGL2Int:	return D3GL2Start(dev);
	case NDX3Int:	return D3DX3Start(dev);
	case NDX5Int:	return D3DX5Start(dev);
	case NDX6Int:	return D3DX6Start(dev);
	case NDX7Int:	return D3DX7Start(dev);
	case NDX8Int:	return D3DX8Start(dev);
	default:
		ASSERTERROR;
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////
// automatickı vıbìr rozhraní

bool SelectD3DDeviceInt(int dev)
{
	return (
		SelectD3DDevice(NDX8Int, dev) ||	// DirectX 8
		SelectD3DDevice(NDX7Int, dev) ||	// DirectX 7
		SelectD3DDevice(NDX6Int, dev) ||	// DirectX 6
		SelectD3DDevice(NGL1Int, dev) ||	// OpenGL 1.1
		SelectD3DDevice(NGL0Int, dev) ||	// OpenGL 1.0
		SelectD3DDevice(NDX5Int, dev) ||	// DirectX 5
		SelectD3DDevice(NDX3Int, dev));		// DirectX 3
}


/////////////////////////////////////////////////////////////////////////////
// vıbìr uivatelského ovladaèe 3D (vrací TRUE=nastavení OK)
// Pozn.: V této chvíli je ji hlavní okno zobrazeno (funkce FDirect3DView())

int korigSpeed = 0;		// èítaè korekce pomalého rozhraní

bool UserD3DDevice()
{
// zrušení starého ovladaèe
	DeSelectD3DDev();

// pøepoèet rozmìrù
	if (!RecalcD3D()) return false;

// vypnutí kurzoru myši
	::ShowCursor(FALSE);

// zadáno rozhraní
	bool res = false;
	if (D3DIntUser > 0)
	{

// zadáno rozhraní i ovladaè
		if (D3DDevUser > 0)
		{
			res = SelectD3DDevice(D3DIntUser, D3DDevUser);
		}

// zadáno rozhraní, ovladaè je automatickı
		else
		{
			res = (
				SelectD3DDevice(D3DIntUser, NHALDev) ||		// HAL
				SelectD3DDevice(D3DIntUser, NTnLHalDev) ||	// TnLHal
				SelectD3DDevice(D3DIntUser, NMMXDev) ||		// MMX
				SelectD3DDevice(D3DIntUser, NRGBDev) ||		// RGB
				SelectD3DDevice(D3DIntUser, NRampDev) ||	// Ramp
				SelectD3DDevice(D3DIntUser, NRefDev));		// REF
		}
	}

// rozhraní je automatické
	else
	{

// zadán ovladaè, rozhraní je automatické
		if (D3DDevUser > 0)
		{
			res = SelectD3DDeviceInt(D3DDevUser);
		}

// ovladaè i rozhraní jsou automatické
		else
		{
			res = (
				SelectD3DDeviceInt(NHALDev) ||		// HAL
				SelectD3DDeviceInt(NTnLHalDev) ||	// TnLHal
				SelectD3DDeviceInt(NMMXDev) ||		// MMX
				SelectD3DDeviceInt(NRGBDev) ||		// RGB
				SelectD3DDeviceInt(NRampDev) ||		// Ramp
				SelectD3DDeviceInt(NRefDev));		// REF
		}
	}

	FPUDouble();

// poadavek k pøekreslení 2D grafiky
	AktAllIcon();

// funkce 3D zaøízení aktivována OK
	if (res)
	{
		D3D = true;

// aktualizace pøíznaku zapnutí mlhy
		D3DCheckFogOn();

// korekce informací o zaøízení
		if (IsMipMap) TexturePow2 = true;
		if ((DWORD)(MaxTextureWidth-1) >= 0x8000) MaxTextureWidth = 0x8000;
		if ((DWORD)(MaxTextureHeight-1) >= 0x8000) MaxTextureHeight = 0x8000;
		if ((DWORD)(MaxTextureRepeat-1) >= 0x8000) MaxTextureRepeat = 0x8000;
		if (MaxVertexNum < 128) MaxVertexNum = 128;
		if (MaxVertexNum > 0xf000) MaxVertexNum = 0xf000;
		MaxVertexNum2 = MaxVertexNum;
		if (MaxVertexNum2 > MAXVERTICES) MaxVertexNum2 = MAXVERTICES;
		if (MaxTextureStages < 1) MaxTextureStages = 1;
		if (MaxTextureStages > MAX_STAGES) MaxTextureStages = MAX_STAGES;
		if (MaxTextureSimult < 1) MaxTextureSimult = 1;
		if (MaxTextureSimult > MaxTextureStages) MaxTextureSimult = MaxTextureStages;

		D3DShadows = ((D3DSBufferFormatOK || IsBlending) && (D3DIntAkt != 4));
		D3DSBufferClear = D3DSBufferFormatOK;

		if (TexturePow2)
		{
			int i = 0x8000;
			while (i > MaxTextureWidth) i = (i >> 1);
			MaxTextureWidth = i;

			i = 0x8000;
			while (i > MaxTextureHeight) i = (i >> 1);
			MaxTextureHeight = i;
		}

		if (SquareTexture)
		{
			if (MaxTextureWidth > MaxTextureHeight)
			{
				MaxTextureWidth = MaxTextureHeight;
			}
			else
			{
				MaxTextureHeight = MaxTextureWidth;
			}
		}

		korigSpeed = 5;

// kvalita textur
		D3DLowText = (pD3Free() < 5000000);

// aktualizace parametrù
		Matrix0(&D3DProjMatrix);		// nulování projekèní matice
		ReSet3DCount = 0;				// resetování opakované inicializace
		ReSet3DCountN = 1;				// inicializace pøíštího èítaèe
		ReSet3D();						// resetování pøepínaèù a stavù

// nulování øetìzce k renderování
//		MemFill(D3D_Rend, 3*MAX_ZBIAS*sizeof(D3DFITEM_*), 0);

// inicializace mìøení èasu
		D3DLastTime = FPreciseTimer();
		D3DElapsedTime = 0;
		D3DAverageTime = 0;
		D3DAverageFreq = 0;
		D3DAverageFreqI = 0;
		D3DValidTime = 10;

// inicializace pøepínaèù
		D3DVSync = true;
		D3DVertFog = (!IsHWRaster && (D3DDevAkt != NRefDev));

		pD3AktPalette(MainFrame);

// vymazání plochy
		FPUFloat();
		pD3Begin();
		pD3Clear();
		D3DSBufferClear = false;
		pD3End();
		FPUDouble();

// zapnutí kurzoru myši
		::ShowCursor(TRUE);

		return true;
	}

// chyba inicializace
	else
	{
		D3DIntAkt = 0;
		D3DDevAkt = 0;

// zapnutí kurzoru myši
		::ShowCursor(TRUE);

		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace projekèní matice

void AktProjMatrix()
{
	// pøední a zadní omezovací rovina
	double front = fabs(D3DFrontClip);
	double back = fabs(D3DBackClip);

	// oprava poøadí omezovacích rovin
	if (back < front)
	{
		double tmp = front;
		front = back;
		back = tmp;
	}

	// omezení rovin (èíslo je zvoleno jen
	// pøiblinì, navíc aby bylo bezeztrátovì
	// vyjádøené v double i float = 1/128)
	double eps = 0.0078125;
	if (front < eps) front = eps;
	if ((back - front) < eps) back = front + eps;

	// pøiblíení nejblišího zobrazeného objektu
	double front2 = front / 2;
	if (D3DIntAkt >= 4) front2 /= 2;	// DirectX pouívá Z buffer 0 a 1, OpenGL -1 a +1

	// ortografická projekce
	if ((D3DProjection == 1) || (D3DProjection == 3))
	{
		if (D3DIntAkt >= 4)
		{
			double q = 1 / (back - front2);
			D3DProjMatrix._33 =  (float)(q);
			D3DProjMatrix._43 =  (float)(-q*front2);
		}
		else
		{
			front2 /= 2;
			double q = 2 / (back - front2);
			D3DProjMatrix._33 =  (float)(q);
			D3DProjMatrix._43 =  (float)(-1-q*front2);
		}

		D3DProjMatrix._44 = 1;
		D3DProjMatrix._34 = 0;

		front = 1;
	}
	else
	{
		double q = back / (back - front2);
		D3DProjMatrix._33 =  (float)q;
		D3DProjMatrix._43 =  (float)(-q * front2);

		D3DProjMatrix._44 = 0;
		D3DProjMatrix._34 = 1;
	}

	// koeficienty šíøky a vıšky projekce
	double k = (double)D3DHeight/D3DWidth;
	D3DProjMatrix._11 =  (float)(front*(1+k));
	D3DProjMatrix._22 =  (float)(front*(1+1/k));

	if (D3DProjection >= 2)
	{
		D3DProjMatrix._33 = -D3DProjMatrix._33;
		D3DProjMatrix._34 = -D3DProjMatrix._34;
	}

	pD3AktProj();
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace projekèního jehlanu

void AktFrustum()
{
// pøíprava kombinované matice
	D3DMATRIX mat;
	MatrixMul(&mat, &D3DViewMatrix, &D3DProjMatrix);

// pravá rovina
	float a = mat._14 - mat._11;
	float b = mat._24 - mat._21;
	float c = mat._34 - mat._31;
	float d = mat._44 - mat._41;
	float t = (float)sqrt(a*a + b*b + c*c);

	D3DFrustum[0].A = a/t;
	D3DFrustum[0].B = b/t;
	D3DFrustum[0].C = c/t;
	D3DFrustum[0].D = d/t;

// levá rovina
	a = mat._14 + mat._11;
	b = mat._24 + mat._21;
	c = mat._34 + mat._31;
	d = mat._44 + mat._41;
	t = (float)sqrt(a*a + b*b + c*c);

	D3DFrustum[1].A = a/t;
	D3DFrustum[1].B = b/t;
	D3DFrustum[1].C = c/t;
	D3DFrustum[1].D = d/t;

// dolní rovina
	a = mat._14 + mat._12;
	b = mat._24 + mat._22;
	c = mat._34 + mat._32;
	d = mat._44 + mat._42;
	t = (float)sqrt(a*a + b*b + c*c);

	D3DFrustum[2].A = a/t;
	D3DFrustum[2].B = b/t;
	D3DFrustum[2].C = c/t;
	D3DFrustum[2].D = d/t;

// horní rovina
	a = mat._14 - mat._12;
	b = mat._24 - mat._22;
	c = mat._34 - mat._32;
	d = mat._44 - mat._42;
	t = (float)sqrt(a*a + b*b + c*c);

	D3DFrustum[3].A = a/t;
	D3DFrustum[3].B = b/t;
	D3DFrustum[3].C = c/t;
	D3DFrustum[3].D = d/t;

// vzdálená rovina
	a = mat._14 - mat._13;
	b = mat._24 - mat._23;
	c = mat._34 - mat._33;
	d = mat._44 - mat._43;
	t = (float)sqrt(a*a + b*b + c*c);

	D3DFrustum[4].A = a/t;
	D3DFrustum[4].B = b/t;
	D3DFrustum[4].C = c/t;
	D3DFrustum[4].D = d/t;

// blízká rovina
	a = mat._14 + mat._13;
	b = mat._24 + mat._23;
	c = mat._34 + mat._33;
	d = mat._44 + mat._43;
	t = (float)sqrt(a*a + b*b + c*c);

	D3DFrustum[5].A = a/t;
	D3DFrustum[5].B = b/t;
	D3DFrustum[5].C = c/t;
	D3DFrustum[5].D = d/t;
}

/////////////////////////////////////////////////////////////////////////////
// vyrenderování scény

void D3Render(bool disp)
{
// test, zda je reim 3D
	if (D3D)
	{

// zapnutí sníené pøesnosti procesoru
		FPUFloat();

// restart vıchozího stavu 3D
		ReSet3D();

// aktualizace všech matic objektù
		D3DF_AktMatrix();

// aktualizace pohledové matice (zmìnìna v aktualizaci matice kamery)
		if (D3DViewAkt)
		{
			D3DCameraInvert = (((D3DF_Data[1].ScaleX < 0) && (D3DF_Data[1].ScaleY >= 0)) ||
							((D3DF_Data[1].ScaleX >= 0) && (D3DF_Data[1].ScaleY < 0)));
			D3DViewAkt = false;
			D3DFrustumAkt = true;
			pD3AktView();
		}

// aktualizace projekèní matice
		if (D3DProjAkt)
		{
			D3DProjAkt = false;
			D3DFrustumAkt = true;
			AktProjMatrix();
		}

// aktualizace rovin projekèního jehlanu
		if (D3DFrustumAkt)
		{
			D3DFrustumAkt = false;
			AktFrustum();
		}

// zahájení renderování scény (pro OpenGL pøíprava k aktualizaci svìtel)
		pD3Begin();

// aktualizace svìtel (musí následovat za zahájením scény)
		D3DL_AktLight();

// aktualizace stavu
		if (D3DStateAkt)
		{
			pD3AktState();
			D3DStateAkt = false;
		}

// inicializace Z funkce (kvùli obsluze vymazání pozadí)
		D3DZWrite = TRUE;
		pD3SetZWrite(TRUE);
		D3DZTest = 0;
		pD3SetZFunc(0);

// vymazání renderovací plochy
		if (D3DBackText >= 0)
		{
			bool filtr = D3DF_Get(0)->MagFilter[0];
			SetD3DMinMipFilter(filtr, false, 0);	// filtr pro zmenšení - pøednastavení nutné pro generování textury

			D3DM_Set(0);							// standardní materiál
			D3DT_Akt(D3DBackText, 0);				// nastavení textury

			D3DColorOp[0] = D3DTOP_SELECTARG1;
			D3DColorArg1[0] = D3DTA_TEXTURE;
			D3DColorArg2[0] = D3DTA_CURRENT;
			pD3SetColorOp(D3DTOP_SELECTARG1, D3DTA_TEXTURE, D3DTA_CURRENT, 0);

			D3DAlphaOp[0] = D3DTOP_SELECTARG1;
			D3DAlphaArg1[0] = D3DTA_TEXTURE;
			D3DAlphaArg2[0] = D3DTA_CURRENT;
			pD3SetAlphaOp(D3DTOP_SELECTARG1, D3DTA_TEXTURE, D3DTA_CURRENT, 0);

			D3DAddressU[0] = D3DTADDRESS_WRAP;
			D3DAddressV[0] = D3DTADDRESS_WRAP;
			pD3SetAddress(D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, 0);

			bool oldfill = D3DWireframeGlobal;
			D3DWireframeGlobal = true;
			SetD3DWireframe(true);					// zapnutí módu vıplnì
			D3DWireframeGlobal = oldfill;

			SetD3DLightOn(false);					// osvìtlení vypnuto
			SetD3DShades(false);					// vypnuto plynulé stínování

			SetD3DCulling(1);						// zobrazena pouze pøední strana

			SetD3DMagFilter(filtr, 0);				// filtr pro zvìtšení
			SetD3DMinMipFilter(filtr, false, 0);	// filtr pro zmenšení

			D3DFITEM item;
			item.IsBlend = false;
			D3DSrcBlend = BLEND_ONE;
			item.SrcBlend = BLEND_ONE;
			D3DDstBlend = BLEND_ZERO;
			item.DstBlend = BLEND_ZERO;
			pD3AktBlend(&item);						// vypnutí blendingu

			D3DAlphaRef = 0;
			item.IsAlphaRef = false;
			item.AlphaRef = 0;
			item.AlphaRef2 = 0;
			pD3AktAlfa(&item);						// vypnutí alfa reference

			SetD3DFogOn(FALSE);						// vypnutí mlhy
		}

		pD3Clear();									// vymazání plochy
		D3DSBufferClear = false;

// altualizace mlhy
		if (FogAkt)
		{
			if (FogOn) pD3AktFog();
			FogAkt = false;
		}

// aktualizace ambient osvìtlení
		if (D3DAmbientColAkt)
		{
			pD3AktAmbient();
			D3DAmbientColAkt = false;
		}

// renderování objektù ve scénì
		D3DF_Render();

// renderování objektù s blending
//		D3DF_RenderBlend();

// renderování 2D obrázkù
//		if (!D3DRamp)
//		{
//			D3DF_Render2D();
//		}

// ukonèení renderování scény
		pD3End();

// zobrazení scény
		if (disp) pD3Disp();

// návrat standardní pøesnosti procesoru
		FPUDouble();

// èítání èasového razítka
//		D3DTimeStamp++;

// naètení ubìhlého pøesného èasu
		if (disp)
		{
			double newtime = FPreciseTimer();
			double oldtime = D3DLastTime;
			D3DLastTime = newtime;
			double elapsed = (newtime - oldtime);
			if (elapsed < 0) elapsed = 0;
			D3DElapsedTime = elapsed;
			if (elapsed > 0.25) D3DElapsedTime = 0.25;
			if (elapsed > 1.25) elapsed = 1.25;

// vıpoèet støední frekvence
			if (D3DAverageTime != 0)
			{
				if (D3DValidTime > 0)
				{
					D3DValidTime--;
					elapsed = (D3DAverageTime + elapsed)/2;
				}
				else
				{
					double dif;
					if (elapsed < D3DAverageTime)
					{
						dif = elapsed/D3DAverageTime;
					}
					else
					{
						dif = D3DAverageTime/elapsed;
					}

					int freq = D3DAverageFreqI;

					switch(Round(dif*8))
					{
					case 0:		freq /= 16;		break;
					case 1:		freq /= 4;		break;
					case 2:						break;
					case 3:		freq *= 2;		break;
					case 4:		freq *= 4;		break;
					case 5:		freq *= 8;		break;
					case 6:		freq *= 16;		break;
					case 7:		freq *= 32;		break;
					case 8:		freq *= 64;		break;
					}

					if (freq == 0) freq++;
					elapsed = (D3DAverageTime*(freq-1) + elapsed)/freq;
				}
			}

			D3DAverageTime = elapsed;

			if (elapsed == 0)
			{
				D3DAverageFreq = 0;
			}
			else
			{
				D3DAverageFreq = 1/elapsed;
			}

			D3DAverageFreqI = Round(D3DAverageFreq + D3DAverageRound);
			if (D3DAverageFreqI > D3DAverageFreq)
			{
				D3DAverageRound = +0.4;
			}
			else
			{
				D3DAverageRound = -0.4;
			}

// korekce VSync
			if (D3DAverageFreqI > 80)
			{
				D3DVSync = true;
			}
			else
			{
				if (D3DAverageFreqI < 50)
				{
					D3DVSync = false;
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// zámìna modré a èervené barvy v RGB (invertuje alpha kanál)

DWORD XRGB(DWORD col)
{
	return ((col & 0xff00) | (((BYTE)col) << 16) | ((BYTE)(col >> 16)) | ((col & 0xff000000) ^ 0xff000000));
}

/////////////////////////////////////////////////////////////////////////////
// konverze barvy z formátu Petra na formát D3DCOLORVALUE (float)

void BGRFloat(DWORD src, D3DCOLORVALUE* dst)
{
	dst->r = (float)((double)(BYTE)src / 255);
	dst->g = (float)((double)(BYTE)(src >> 8) / 255);
	dst->b = (float)((double)(BYTE)(src >> 16) / 255);
	dst->a = (float)((double)((BYTE)(src >> 24) ^ 0xff) / 255);
}

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------

/////////////////////////////////////////////////////////////////////////////
// aktualizace klientskıch souøadnic (pouívá se také pro prvky)
// POZOR - statusbar nemusí bıt ještì na správnıch pozicích!

#pragma warning ( disable: 4701)	// hlášení - neinicializovaná poloka "wnd"

void InitClientRect(WINITEM* item)
{

// úschova rozmìrù a polohy okna
	RECT wnd;

	if ((item->Typ == WINITEM_WINDOW) || (item->Typ == WINITEM_WINDOWRO))
	{
		::GetWindowRect(item->Wnd, &wnd);
		item->Left = wnd.left;
		item->Top = wnd.top;
		item->Width = wnd.right - wnd.left;
		item->Height = wnd.bottom - wnd.top;
	}

	if (item == &Win[0])
	{

// souøadnice pro DirectDraw
		if (FullScreen)
		{
			ClientWidth = ScreenWidth;
			ClientHeight = ScreenHeight;
			item->ClientWidth = ClientWidth;
			item->ClientHeight = ClientHeight;
			return;
		}

		if (::IsZoomed(MainFrame) == FALSE)
		{
			MainFrameX = (wnd.left + wnd.right)/2;
			MainFrameY = (wnd.top + wnd.bottom)/2;
		}

// naètení souøadnic klientské oblasti okna
		::GetClientRect(item->Wnd, &ClientRect);

// sníení vıšky o stavovı øádek
		if (StatusVisible)
		{
			RECT rc;
			::GetWindowRect(StatusBar, &rc);
			ClientRect.bottom -= rc.bottom - rc.top;
		}

// urèení vıšky a šíøky klientské oblasti
		ClientWidth = ClientRect.right - ClientRect.left;
		ClientHeight = ClientRect.bottom - ClientRect.top;
		item->ClientWidth = ClientWidth;
		item->ClientHeight = ClientHeight;
	}

// klientské rozmìry pro ostatní okna a pro ostatní prvky
	else
	{
		::GetClientRect(item->Wnd, &wnd);
		item->ClientWidth = wnd.right - wnd.left;
		item->ClientHeight = wnd.bottom - wnd.top;
	}
}

#pragma warning ( default: 4701)	// hlášení - neinicializovaná poloka "wnd"

/////////////////////////////////////////////////////////////////////////////
// nastavení textu stavové lišty

void SetStatusText(CString& text)
{
	if ((text != AktStatusText) && (StatusBar != NULL))
	{
		AktStatusText = text;
		AktStatusText.SetWindowText(StatusBar);
	}
}

/////////////////////////////////////////////////////////////////////////////
// zobrazení stavového øádku

void SetStatusVisible(bool show)
{
	if (show != StatusVisible)
	{
		StatusVisible = show;

		RECT wnd;
		::GetWindowRect(MainFrame, &wnd);

		RECT rc;
		::GetWindowRect(StatusBar, &rc);

		if (show)
		{
			wnd.bottom += rc.bottom - rc.top;
			::MoveWindow(MainFrame, wnd.left, wnd.top,
				wnd.right - wnd.left, wnd.bottom - wnd.top, TRUE);
// JAKJAK		HFONT oldfont = StatusFont;
//			StatusFont = GetFont(false, false, false, false, false, false, 0, 0, 0);
//			::SendMessage(StatusBar, WM_SETFONT, (WPARAM)StatusFont, 0);
			::ShowWindow(StatusBar, SW_SHOW);
// JAKJAK		FreeFont(oldfont);
		}
		else
		{
			wnd.bottom -= rc.bottom - rc.top;
			::ShowWindow(StatusBar, SW_HIDE);
			::MoveWindow(MainFrame, wnd.left, wnd.top,
				wnd.right - wnd.left, wnd.bottom - wnd.top, TRUE);
		}
//		InitClientRect(&Win[0]);
		OnSize(&Win[0]);
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastavení titulku pro konzolu

void SetConsoleText()
{
 	if (ConsoleOn)
	{
//		::SetConsoleCP(437);
//		int len = AktCaptionText.Length();
//		char* buf = (char*)MemGet(len+1);
//		::CharToOemBuff(AktCaptionText.DataData(), buf, len);
//		buf[len] = 0;
//		::SetConsoleTitle(buf);
//		MemFree(buf);
		::SetConsoleTitle(AktCaptionText);
	}
}


/////////////////////////////////////////////////////////////////////////////
// zobrazení jména v titulku okna

void SetCaptionText(CString& text)
{
	if (text != AktCaptionText) 					// mìní se titulek ?
	{
		AktCaptionText = text;						// úschova nového titulku
		AktCaptionText.SetWindowText(MainFrame);	// nastavení nového titulku
		Win[0].Text = AktCaptionText;
		Win[0].TextValid = true;
		SetConsoleText();
	}
}


/////////////////////////////////////////////////////////////////////////////
// pøemapování klávesy

int MapKey(WPARAM wParam, LPARAM lParam)
{
	if ((lParam & 0x01000000) != 0)
	{
		switch (wParam)
		{
		case VK_CONTROL:	return VK_RCONTROL;
		case VK_MENU:		return VK_RMENU;
		case VK_RETURN:		return VK_RETURN_NUM;

		case VK_SHIFT:	if ((lParam & 0x00FF0000) == 0x00360000)
							return VK_RSHIFT;
						else
							return VK_SHIFT;

		default:			return wParam;
		}
	}
	else
	{
		switch (wParam)
		{
		case VK_CONTROL:	return VK_CONTROL;
		case VK_MENU:		return VK_MENU;
		case VK_RETURN:		return VK_RETURN;
		case VK_HOME:		return VK_NUMPAD7;
		case VK_UP:			return VK_NUMPAD8;
		case VK_PRIOR:		return VK_NUMPAD9;
		case VK_LEFT:		return VK_NUMPAD4;
		case VK_CLEAR:		return VK_NUMPAD5;
		case VK_RIGHT:		return VK_NUMPAD6;
		case VK_END:		return VK_NUMPAD1;
		case VK_DOWN:		return VK_NUMPAD2;
		case VK_NEXT:		return VK_NUMPAD3;
		case VK_INSERT:		return VK_NUMPAD0;
		case VK_DELETE:		return VK_DECIMAL;
		case VK_SHIFT:	if ((lParam & 0x00FF0000) == 0x00360000)
							return VK_RSHIFT;
						else
							return VK_SHIFT;

		default:			return wParam;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// obsluha zprávy pøed rozesláním do oken (TRUE = zpráva zpracována)

// obsluha v grafickém módu (provádí se i v dialogovém módu, pouze se ignoruje vısledek)
BOOL PreTranslateMessageGraf(MSG* msg)
{
	HWND hWnd = msg->hwnd;
	WPARAM wParam = msg->wParam;
	LPARAM lParam = msg->lParam;

	switch (msg->message)
	{
	case WM_LBUTTONDBLCLK:
		LMouseDClick = true;
	case WM_LBUTTONDOWN:
		LMouseClick = true;
		goto MOUSECLICK;

	case WM_RBUTTONDBLCLK:
		RMouseDClick = true;
	case WM_RBUTTONDOWN:
		RMouseClick = true;

	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:

MOUSECLICK:
		LMouseDown = (msg->wParam & MK_LBUTTON) != 0;
		RMouseDown = (msg->wParam & MK_RBUTTON) != 0;

		MouseScreen.x = LOWORD(lParam);
		MouseScreen.y = HIWORD(lParam);

		::ClientToScreen(hWnd, &MouseScreen);
		MouseMain = MouseScreen;
		::ScreenToClient(Win[DialogParent].Wnd, &MouseMain);
		MouseValid = true;
		break;

	case WM_NCLBUTTONDBLCLK:
	case WM_NCLBUTTONDOWN:
//		LMouseDown = true;		// pøi kliknutí na nadpis se aktivuje, ale u neuvolní
		goto NCMOUSECLICK;

	case WM_NCRBUTTONDBLCLK:
	case WM_NCRBUTTONDOWN:
//		RMouseDown = true;
		goto NCMOUSECLICK;

	case WM_NCLBUTTONUP:
		LMouseDown = false;
		goto NCMOUSECLICK;

	case WM_NCRBUTTONUP:
		RMouseDown = false;

NCMOUSECLICK:
		MouseScreen.x = MAKEPOINTS(lParam).x;
		MouseScreen.y = MAKEPOINTS(lParam).y;

		MouseMain = MouseScreen;
		::ScreenToClient(Win[DialogParent].Wnd, &MouseMain);
		MouseValid = true;
		break;


	case WM_SYSCHAR:
	case WM_CHAR:
		{
			TCHAR znak = (TCHAR)wParam;
			CharBuf[CharBufWrite] = znak;
			CharBufWrite++;
			if (CharBufWrite >= KEYBUFSIZE) CharBufWrite = 0;
			if (CharBufWrite == CharBufRead)
			{
				CharBufRead++;
				if (CharBufRead >= KEYBUFSIZE) CharBufRead = 0;
			}
		}
		return FALSE;

	case WM_SYSKEYDOWN:
#ifndef _MINI
		if ((msg->wParam == VK_RETURN) && !D3DReturn)
#else
		if (msg->wParam == VK_RETURN)
#endif
		{
			SetFullScreen(!FullScreen);
			return TRUE;
		}

	case WM_KEYDOWN:
		{
			int znak = MapKey(wParam, lParam);

			if (Pause)
			{
				Pause = false;
				break;
			}				

			switch (znak)
			{
			case VK_CONTROL:
				CtrlAgain = KeyMap[VK_CONTROL];
				break;

			case VK_NUMLOCK:
				if (!AktNumLock) NumLock = (::GetKeyState(VK_NUMLOCK) & 1) != 0;
				break;

			case VK_CAPITAL:
				if (!AktCapsLock) CapsLock = (::GetKeyState(VK_CAPITAL) & 1) != 0;
				break;

			case VK_SCROLL:
				if (!AktScrollLock) ScrollLock = (::GetKeyState(VK_SCROLL) & 1) != 0;
				break;

			case VK_INSERT:
				if (!AktInsertLock) InsertLock = (::GetKeyState(VK_INSERT) & 1) != 0;
				break;

			case VK_PAUSE:
				Pause = TRUE;
				return FALSE;

			case VK_RMENU:
				if (KeyBufWrite != KeyBufRead)
				{
					int write = KeyBufWrite - 1;
					if (write < 0) write = KEYBUFSIZE-1;
					if (KeyBuf[write] == VK_CONTROL)
					{
						KeyBufWrite = write;
						if (!CtrlAgain)
						{
							if (KeyMap[VK_CONTROL])
							{
								KeyMap[VK_CONTROL] = false;
								KeyMapNum--;
							}
						}
					}
				}
			}

			if ((DWORD)znak < KEYMAPSIZE)
			{
				if (!KeyMap[znak])
				{
					KeyMap[znak] = true;
					KeyMapNum++;
				}
			}

			KeyBuf[KeyBufWrite] = znak;
			KeyBufWrite++;
			if (KeyBufWrite >= KEYBUFSIZE) KeyBufWrite = 0;
			if (KeyBufWrite == KeyBufRead)
			{
				KeyBufRead++;
				if (KeyBufRead >= KEYBUFSIZE) KeyBufRead = 0;
			}

			if ((znak == VK_MENU) || (znak == VK_F10)) return TRUE;
		}
		return FALSE;

	case WM_SYSKEYUP:
	case WM_KEYUP:
		{
			int znak = MapKey(wParam, lParam);

			if ((DWORD)znak < KEYMAPSIZE)
			{
				if (KeyMap[znak])
				{
					KeyMap[znak] = false;
					KeyMapNum--;
				}
			}

			switch (znak)
			{
			case VK_SHIFT:
				if (KeyMap[VK_RSHIFT])
				{
					if (::GetAsyncKeyState(VK_SHIFT) >= 0)
					{
						KeyMap[VK_RSHIFT] = false;
						KeyMapNum--;
					}
				}
				break;

			case VK_RSHIFT:
				if (KeyMap[VK_SHIFT])
				{
					if (::GetAsyncKeyState(VK_SHIFT) >= 0)
					{
						KeyMap[VK_SHIFT] = false;
						KeyMapNum--;
					}
				}
				break;
			}
			if ((znak == VK_MENU) || (znak == VK_F10)) return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}

// obsluha v dialogovém módu
BOOL PreTranslateMessageDialog(MSG* msg)
{
	if (msg->message == WM_SYSKEYDOWN)
	{
		if (DialogGraph && ((msg->wParam == VK_MENU) || (msg->wParam == VK_F10))) return TRUE;
	}

	if (msg->message == WM_KEYDOWN)
	{
		switch (msg->wParam)
		{
		case VK_TAB:
			{
				if (::GetAsyncKeyState(VK_SHIFT) >= 0)
				{
					if (Win.IsValid(WindowFocus) &&
						(Win[WindowFocus].Typ == WINITEM_RICHMEMO) &&
						(!Win[WindowFocus].Disable))
					{
						return FALSE;
					}

					FocusNext();
				}
				else
				{
					FocusPrev();
				}
				return TRUE;
			}
			break;

		case VK_DOWN:
			if (Win.IsValid(WindowFocus) &&
				(Win[WindowFocus].Typ == WINITEM_EDIT))
			{
				return TRUE;
			}

		case VK_RIGHT:
			if (Win.IsValid(WindowFocus))
			{
				switch (Win[WindowFocus].Typ)
				{
				case WINITEM_BUTTONRADIO:
				case WINITEM_BUTTONRADIO2:
				case WINITEM_BUTTON:
				case WINITEM_BUTTONICON:
				case WINITEM_BUTTONCHECK2:
				case WINITEM_DEFBUTTON:
					RadioNext();
					return TRUE;
				}
			}
			break;

		case VK_UP:
			if (Win.IsValid(WindowFocus) &&
				(Win[WindowFocus].Typ == WINITEM_EDIT))
			{
				return TRUE;
			}

		case VK_LEFT:
			if (Win.IsValid(WindowFocus))
			{
				switch (Win[WindowFocus].Typ)
				{
				case WINITEM_BUTTONRADIO:
				case WINITEM_BUTTONRADIO2:
				case WINITEM_BUTTON:
				case WINITEM_BUTTONICON:
				case WINITEM_BUTTONCHECK2:
				case WINITEM_DEFBUTTON:
					RadioPrev();
					return TRUE;
				}
			}
			break;

		case VK_RETURN:
			if (Win.IsValid(WindowFocus))
			{
				WINITEM* item = &Win[WindowFocus];

				switch (item->Typ)
				{
				case WINITEM_BUTTON:
				case WINITEM_BUTTONICON:
				case WINITEM_BUTTONCHECK2:
				case WINITEM_DEFBUTTON:
					::SendMessage(item->Wnd, WM_KEYDOWN, VK_SPACE, 0);
					::SendMessage(item->Wnd, WM_KEYUP, VK_SPACE, 0);
					return TRUE;

				case WINITEM_MEMO:
				case WINITEM_RICHMEMO:
				case WINITEM_RICHEDIT:
					if (!Win[WindowFocus].Disable)
					{
						return FALSE;
					}
				}
			}

			{
				for (int i = Win[DialogParent].Next; i != DialogParent; i = Win[i].Next)
				{
					if ((Win[i].Typ == WINITEM_DEFBUTTON) && !Win[i].Disable)
					{
						WindowFocus = i;
						::SetFocus(Win[i].Wnd);
						::SendMessage(Win[i].Wnd, WM_KEYDOWN, VK_SPACE, 0);
						::SendMessage(Win[i].Wnd, WM_KEYUP, VK_SPACE, 0);
						return TRUE;
					}
				}
			}
			break;

		case 'F':
			if (Win.IsValid(WindowFocus))
			{
				WINITEM* item = &Win[WindowFocus];

				switch (item->Typ)
				{
				case WINITEM_RICHMEMO:
				case WINITEM_RICHEDIT:
					if (!item->Disable && (FindDialogBox == NULL))
					{
						if ((::GetAsyncKeyState(VK_CONTROL) < 0) &&
							(::GetAsyncKeyState(VK_MENU) >= 0))
						{
							if (FindBuff == NULL)
							{
								FindBuff = (LPTSTR)MemGet(FINDBUFFSIZE*sizeof(TCHAR));
								FindBuff[0] = 0;
							}

							if (FindMsgString == 0)
							{
								FindMsgString = ::RegisterWindowMessage(FINDMSGSTRING);
							}

							FindStruc.lStructSize = sizeof(FINDREPLACE);
							FindStruc.hwndOwner = MainFrame;
							FindStruc.Flags = FR_HIDEUPDOWN;
							if (FindMatchCase) FindStruc.Flags |= FR_MATCHCASE;
							if (FindWholeWord) FindStruc.Flags |= FR_WHOLEWORD;
							FindStruc.lpstrFindWhat = FindBuff;
							FindStruc.wFindWhatLen = FINDBUFFSIZE;
							FindStruc.lCustData = WindowFocus;

							FindDialogBox = ::FindText(&FindStruc);
							return TRUE;
						}
					}
				}
			}
			break;

		case VK_F3:
			if (Win.IsValid(WindowFocus))
			{
				WINITEM* item = &Win[WindowFocus];

				switch (item->Typ)
				{
				case WINITEM_RICHMEMO:
				case WINITEM_RICHEDIT:
					if (!item->Disable &&
						(::GetAsyncKeyState(VK_CONTROL) >= 0) &&
						(::GetAsyncKeyState(VK_SHIFT) >= 0) &&
						(::GetAsyncKeyState(VK_MENU) >= 0))
					{
						if ((FindBuff != NULL) && (FindBuff[0] != 0))
						{
							FINDTEXT ft;

							::SendMessage(item->Wnd, EM_EXGETSEL, 0, (LPARAM)&(ft.chrg));
							ft.lpstrText = FindBuff;
							ft.chrg.cpMin++;
							ft.chrg.cpMax = -1;

							int pos = ::SendMessage(item->Wnd, EM_FINDTEXT, 
							FindStruc.Flags & (FR_MATCHCASE | FR_WHOLEWORD), (LPARAM)&ft);

							if (pos >= 0)
							{
								ft.chrg.cpMin = pos;
								ft.chrg.cpMax = pos + ::lstrlen(FindBuff);
								::SendMessage(item->Wnd, EM_EXSETSEL, 0, (LPARAM)&(ft.chrg));
								::SendMessage(item->Wnd, EM_SCROLLCARET, 0, 0);
							}

						}
						return TRUE;
					}
				}
			}
			break;


		case VK_ESCAPE:
			if (Win.IsValid(ButtonEsc) && !Win[ButtonEsc].Disable &&
				(	
					(Win[ButtonEsc].Typ == WINITEM_WINDOW) ||
					(Win[ButtonEsc].Typ == WINITEM_WINDOWRO) ||
					(Win[ButtonEsc].Typ == WINITEM_BUTTON) ||
					(Win[ButtonEsc].Typ == WINITEM_DEFBUTTON) ||
					(Win[ButtonEsc].Typ == WINITEM_BUTTONICON) ||
					(Win[ButtonEsc].Typ == WINITEM_BUTTONCHECK2) ||
					(Win[ButtonEsc].Typ == WINITEM_WINTEXT) ||
					(Win[ButtonEsc].Typ == WINITEM_WINICON) ||
					(Win[ButtonEsc].Typ == WINITEM_WINPIC)
				)
			  )
			{
				Win[ButtonEsc].Click = true;
				return TRUE;
			}
			break;
		}
	}

	return FALSE;
}

// obsluha zpráv
BOOL PreTranslateMessage(MSG* msg)
{
// obsluha dialogového módu
	if (DialogMode || (DialogParent != 0))
	{

// pro tlaèítka náhrada dvojkliku myší obyèejnım klikem
		if (msg->message == WM_LBUTTONDBLCLK)
		{
			TCHAR buf[21];
			::GetClassName(msg->hwnd, buf, 20);
			if (ButtonClassName == buf)
			{
				msg->message = WM_LBUTTONDOWN;
			}
		}

// obsluha zprávy
		PreTranslateMessageGraf(msg);
		return PreTranslateMessageDialog(msg);
	}

// obsluha grafického módu
	return PreTranslateMessageGraf(msg);
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace pøedešlého okna

void AktPrevWindow()
{
	if ((MainFrame != NULL) && ::IsWindow(MainFrame))
	{
		HWND wnd = MainFrame;
		for (int i = 100; i > 0; i--)
		{
			wnd = ::GetNextWindow(wnd, GW_HWNDNEXT);
			if (wnd == NULL) break;
			long styl = ::GetWindowLong(wnd, GWL_STYLE);
			if (styl != 0)
			{
				if ((styl & (WS_VISIBLE | WS_DISABLED | WS_OVERLAPPEDWINDOW | WS_POPUP | WS_CHILD)) == (WS_VISIBLE | WS_OVERLAPPEDWINDOW))
				{
					PrevWindow = wnd;
					break;
				}
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// obsluha zpráv okna

LRESULT CALLBACK WindowProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	if (!MemoryOK)
	{
		return ::DefWindowProc(hWnd, nMsg, wParam, lParam);
	}

	int inx = ::GetWindowLong(hWnd, 0);

// OpenGL okno
/*
	if (inx == -1)
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
				::GetCursorPos(&pt);	// naètení souøadnic myši
				::ScreenToClient(Win[DialogParent].Wnd, &pt); // pøevod na klientské souøadnice
				if (OnSetCursor(pt.x, pt.y))
				{
					return TRUE;
				}
			}
			break;
		}

		return ::DefWindowProc(hWnd, nMsg, wParam, lParam);
	}
*/

	if (Win.IsNotValid(inx))
	{
		return ::DefWindowProc(hWnd, nMsg, wParam, lParam);
	}
	WINITEM* item = &Win[inx];

	if ((nMsg == FindMsgString) &&
		(FindMsgString != 0) &&
		((FINDREPLACE*)lParam == &FindStruc) &&
		(Win.IsValid(FindStruc.lCustData)))
	{
		WINITEM* it = &Win[FindStruc.lCustData];

		FindMatchCase = ((FindStruc.Flags & FR_MATCHCASE) != 0);
		FindWholeWord = ((FindStruc.Flags & FR_WHOLEWORD) != 0);

		if (FindStruc.Flags & FR_DIALOGTERM)
		{
			FindDialogBox = NULL;
		}

		if (FindStruc.Flags & FR_FINDNEXT)
		{
			::SendMessage(FindDialogBox, WM_CLOSE, 0, 0);

			FINDTEXT ft;

			::SendMessage(it->Wnd, EM_EXGETSEL, 0, (LPARAM)&(ft.chrg));
			ft.lpstrText = FindStruc.lpstrFindWhat;
			ft.chrg.cpMax = -1;

			int pos = ::SendMessage(it->Wnd, EM_FINDTEXT, 
					FindStruc.Flags & (FR_MATCHCASE | FR_WHOLEWORD), (LPARAM)&ft);

			if (pos >= 0)
			{
				ft.chrg.cpMin = pos;
				ft.chrg.cpMax = pos + ::lstrlen(FindStruc.lpstrFindWhat);
				::SendMessage(it->Wnd, EM_EXSETSEL, 0, (LPARAM)&(ft.chrg));
				::SendMessage(it->Wnd, EM_SCROLLCARET, 0, 0);
			}

		}

		return 0;
	}

	switch (nMsg)
	{
	case WM_CLOSE:
		if (item->MayClose)
		{
			if (inx == 0)
			{
				AktPrevWindow();
				::PostQuitMessage(0);
			}
			else
			{
				WindowDel(inx);
			}
		}
		else
		{
			ReqClose = true;
		}
		return 0;

	case WM_PAINT:
		OnPaint(item, inx);
		return 0;

	case WM_SIZE:
		if (!Resizing)
		{
			AppMinim = (wParam == SIZE_MINIMIZED);
			OnSize(item);

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI
			if (D3D && (item == &Win[0]))
			{
				ReSet3DCount = 0;				// resetování opakované inicializace
				ReSet3DCountN = 1;				// inicializace pøíštího èítaèe

				RecalcD3D();
				FPUFloat();
				pD3SizeView();
				pD3AktPalette(MainFrame);
				FPUDouble();
			}
#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------
		}
		return 0;

	case WM_MOVE:
		if (!Resizing)
		{
			InitClientRect(item);

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI
			if (D3D && (item == &Win[0]))
			{
				ReSet3DCount = 0;				// resetování opakované inicializace
				ReSet3DCountN = 1;				// inicializace pøíštího èítaèe

				RecalcD3D();
				FPUFloat();
				pD3MoveView();
				pD3AktPalette(MainFrame);
				FPUDouble();
			}
#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------

		}
		return 0;

	case WM_SIZING:
		if (inx == 0) OnSizing((RECT*)lParam, wParam);
		return TRUE;

	case WM_QUERYNEWPALETTE:
		pD3AktPalette(item->Wnd);

//		{
//			HDC dc = ::GetDC(item->Wnd);
//			HPALETTE oldPal;

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI
//			if (GLPalette && GLContext && pwglMakeCurrent)
//			{
//				pwglMakeCurrent(dc, GLContext);
//				::UnrealizeObject(GLPalette);
//				oldPal = ::SelectPalette(dc, GLPalette, FALSE);
//			}
//			if (D3GL0Pal && D3GL0Cont && pwglMakeCurrent)
//			{
//				pwglMakeCurrent(dc, D3GL0Cont);
//				::UnrealizeObject(D3GL0Pal);
//				oldPal = ::SelectPalette(dc, D3GL0Pal, FALSE);
//			}
//			else

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------

//			{
//				::UnrealizeObject(StdPalette);
//				oldPal = ::SelectPalette(dc, StdPalette, FALSE);
//			}
//
//			::RealizePalette(dc);
//			::SelectPalette(dc, oldPal, FALSE);
//			::ReleaseDC(item->Wnd, dc);

			RePaint(item);
			return TRUE;
//		}
//		break;

	case WM_PALETTECHANGED:
		if ((HWND)wParam != item->Wnd)
		{
//			HDC dc = ::GetDC(item->Wnd);

//			HPALETTE oldPal;

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI
//			if (GLPalette && GLContext && pwglMakeCurrent)
//			{
//				pwglMakeCurrent(dc, GLContext);
//				::UnrealizeObject(GLPalette);
//				oldPal = ::SelectPalette(dc, GLPalette, FALSE);
//			}
//			if (D3GL0Pal && D3GL0Cont && pwglMakeCurrent)
//			{
//				pwglMakeCurrent(dc, D3GL0Cont);
//				::UnrealizeObject(D3GL0Pal);
//				oldPal = ::SelectPalette(dc, D3GL0Pal, FALSE);
//			}
//			else

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------

//			{
//				::UnrealizeObject(StdPalette);
//				oldPal = ::SelectPalette(dc, StdPalette, FALSE);
//			}
  //
//			::RealizePalette(dc);
//			::SelectPalette(dc, oldPal, FALSE);
//			::ReleaseDC(item->Wnd, dc);

			pD3AktPalette(item->Wnd);

			RePaint(item);
			return TRUE;
		}
		return FALSE;

	case MM_MCINOTIFY:
		if (inx == 0)
		if (((UINT)lParam == MusicDevice) && (wParam == MCI_NOTIFY_SUCCESSFUL))
		{
			if (MusicEnd) OnMusicEnd();
			MusicEnd = true;			// pøíznak ukonèení hudby
		}
		return 0;

	case WM_NOTIFY:
		{
			NMHDR* hdr = (NMHDR*)lParam;
			int id = hdr->idFrom;

			if (Win.IsValid(id))
			{
				WINITEM* item = &Win[id];
				if (item->Wnd == hdr->hwndFrom)
				{
					switch (hdr->code)
					{
					case TCN_SELCHANGE:
						if (item->Typ == WINITEM_TABS)
						{
							item->Click = true;

							id = item->Parent;
							if (Win.IsValid(id))
							{
								Win[id].Click = true;
							}
						}
						break;

					case NM_CLICK:
						if (item->Typ == WINITEM_TABS)
						{
							if (item->Wnd == ::GetFocus())
							{
								WindowFocus = id;
							}
						}
						break;

					case LVN_GETDISPINFO:
						if (item->Typ == WINITEM_TABLE)
						{
							NMLVDISPINFO* di = (NMLVDISPINFO*)lParam;

							LVITEM* lvi = &(di->item);

							int i = lvi->iItem;
							int s = lvi->iSubItem;
							int cols = item->TabCols;

							if (((DWORD)i < (DWORD)item->TabRows) &&
								((DWORD)s < (DWORD)cols))
							{
								lvi->pszText = (LPTSTR)(LPCTSTR)(item->TabData[i * cols + s]);
							}
							else
							{
								lvi->pszText = NULL;
							}
						}
						break;

					case NM_SETFOCUS:
						WindowFocus = id;
						break;

					case NM_CUSTOMDRAW:
						if (item->Typ == WINITEM_TABLE)
						{
							NMLVCUSTOMDRAW* cd = (NMLVCUSTOMDRAW*)lParam;

							if (cd->nmcd.dwDrawStage == CDDS_PREPAINT)
							{
								return CDRF_NOTIFYITEMDRAW;
							}

							if (cd->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
							{
								HDC dc = cd->nmcd.hdc;
								RECT rc;
								RECT rc2;
								int n = cd->nmcd.dwItemSpec;
								HWND wnd = item->Wnd;

							// pøíznak aktivní poloky
								int state = ::SendMessage(wnd, LVM_GETITEMSTATE, n, LVIS_SELECTED);
								BOOL sel = (state & LVIS_SELECTED);
								BOOL focus = (::GetFocus() == wnd);

							// vymazání podkladu celé poloky
								rc.left = LVIR_BOUNDS;
								::SendMessage(wnd, LVM_GETITEMRECT, n, (LPARAM)&rc);
								::FillRect(dc, &rc, (sel ? (focus ? StdBrushHighlight : StdBrushBtn) : item->FontBrush));

							// zobrazení oddìlovacích èar a textù
								if ((DWORD)n < (DWORD)item->TabRows)
								{
									::SetTextColor(dc, (sel ? (focus ? StdColorHighlightText : StdColorBtnText) : item->FontCol));

									HBRUSH brush = (HBRUSH)::GetStockObject(GRAY_BRUSH);

									int cols = item->TabCols;

									CString* txt = &(item->TabData[n*cols]);

									for (int j = 0; j < cols; j++)
									{										
										rc.top = j;
										rc.left = LVIR_LABEL;
										::SendMessage(wnd, LVM_GETSUBITEMRECT, n, (LPARAM)&rc);

										rc2 = rc;
										rc2.left--;
										rc2.top = rc2.bottom - 1;
										::FillRect(dc, &rc2, brush);
		
										rc2 = rc;
										rc2.left = rc2.right - 1;
										::FillRect(dc, &rc2, brush);

										int alg;

										switch (item->TabAlign[j])
										{
										case 1:
											alg = DT_RIGHT;
											break;

										case 2:
											alg = DT_CENTER;
											break;

										default:
											alg = DT_LEFT;
										}

										::DrawText(dc, (LPCTSTR)txt[0], txt[0].Length(), &rc, alg);

										txt++;
									}
								}

								if (sel)
								{
									if((DWORD)item->TabCol < (DWORD)item->TabCols) item->TabRow = n;

									if (n != item->TabRowF)
									{
										item->TabRowF = n;
										item->Click = true;
										item->TextValid = false;
									}
								}

								return CDRF_SKIPDEFAULT;
							}
						}
						break;
					}
				}
			}
		}
		return 0;

	case WM_COMMAND:
		if (!ProgramFill)
		{
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:		// == CBN_SELCHANGE
//			case TCN_SELCHANGE:
			case EN_CHANGE:
			case CBN_EDITCHANGE:
				{
					int id = LOWORD(wParam);
					if (Win.IsValid(id))
					{
						Win[id].TextValid = false;
					}
				}
// pokraèuje BN_CLICKED !!!
			case BN_CLICKED:		// == STN_CLICKED
				{
					int id = LOWORD(wParam);
					if (Win.IsValid(id))
					{
						WINITEM* item = &Win[id];

						bool click = false;

						if ((item->Typ == WINITEM_BUTTONRADIO) ||
							(item->Typ == WINITEM_BUTTONRADIO2))
						{
							int check = ::SendMessage(item->Wnd, BM_GETCHECK, 0, 0);
							item->Check = (check == BST_CHECKED);
							item->Check3 = (check == BST_INDETERMINATE);

							if (item->Check)
							{
								click = true;
							}
						}
						else
						{
							click = true;
						}
		
						if ((item->Typ == WINITEM_BUTTONCHECK) || 
							(item->Typ == WINITEM_BUTTONCHECK2) || 
							(item->Typ == WINITEM_BUTTONICON) ||
							(item->Typ == WINITEM_BUTTON3))
						{
							int check = ::SendMessage(item->Wnd, BM_GETCHECK, 0, 0);
							item->Check = (check == BST_CHECKED);
							item->Check3 = (check == BST_INDETERMINATE);
						}

						if (click)
						{
							item->Click = true;

							id = item->Parent;
							if (Win.IsValid(id))
							{
								Win[id].Click = true;
							}
						}
					}
				}
				break;

			case BN_SETFOCUS:
			case EN_SETFOCUS:
			case LBN_SETFOCUS:
			case CBN_SETFOCUS:
				{
					int id = LOWORD(wParam);
					if (Win.IsValid(id))
					{
						WindowFocus = id;
					}
				}
				break;
			}
		}
		return 0;

	case WM_HSCROLL:
	case WM_VSCROLL:
		{
			HWND wnd = (HWND)lParam;

			for (int i = Win[DialogParent].Next; i != DialogParent; i = Win[i].Next)
			{
				if (Win[i].Wnd == wnd)
				{
					WINITEM* item = &Win[i];

					if ((item->Typ == WINITEM_HSCROLLBAR) ||
						(item->Typ == WINITEM_VSCROLLBAR))
					{
						int oldpos = Round(item->ScrollMax * item->ScrollPos);
						int newpos = oldpos;
						int page = Round(SCROLLRANGE * item->ScrollPage);
						int line = Round(SCROLLRANGE * item->ScrollPage / 10);
						int maxpos = item->ScrollMax;

						if (line < 1) line = 1;

						switch (LOWORD(wParam))
						{
						case SB_TOP:
							newpos = 0;
							break;

						case SB_BOTTOM:
							newpos = maxpos;
							break;

						case SB_LINELEFT:
//						case SB_LINEUP:
							newpos -= line;
							break;

						case SB_LINERIGHT:
//						case SB_LINEDOWN:
							newpos += line;
							break;

						case SB_PAGELEFT:
//						case SB_PAGEUP:
							newpos -= page;
							break;

						case SB_PAGERIGHT:
//						case SB_PAGEDOWN:
							newpos += page;
							break;

						case SB_THUMBPOSITION:
						case SB_THUMBTRACK:
							newpos = HIWORD(wParam);
							break;
						}

						if (newpos > maxpos) newpos = maxpos;
						if (newpos < 0) newpos = 0;

						if (newpos != oldpos)
						{
							item->Click = true;
							Win[DialogParent].Click = true;

							if (maxpos <= 0) maxpos = 1;
							item->ScrollPos = (double)newpos / maxpos;

							::SetScrollPos(item->Wnd, SB_CTL, newpos, TRUE);
						}

						return 0;
					}

					else

					if ((item->Typ == WINITEM_HTRACKBAR) ||
						(item->Typ == WINITEM_VTRACKBAR))
					{
						item->Click = true;
						Win[DialogParent].Click = true;

					}

					break;
				}
			}
		}
		break;

	case WM_SETFOCUS:
		if (Win.IsValid(WindowFocus) &&
			(Win[WindowFocus].Parent == inx))
		{
			::SetFocus(Win[WindowFocus].Wnd);
		}
		else
		{
			WindowFocus = inx;
		}

		if (inx == 0)
		{
			MemFill(KeyMap, KEYMAPSIZE, false);
			KeyMapNum = 0;
			
			if (!AktNumLock) NumLock = (::GetKeyState(VK_NUMLOCK) & 1) != 0;
			if (!AktCapsLock) CapsLock = (::GetKeyState(VK_CAPITAL) & 1) != 0;
			if (!AktScrollLock) ScrollLock = (::GetKeyState(VK_SCROLL) & 1) != 0;
			if (!AktInsertLock) InsertLock = (::GetKeyState(VK_INSERT) & 1) != 0;
		}
		break;

	case WM_DISPLAYCHANGE:			// 0x07E
		if (!Resizing)
		{
			AktDispInfo();

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

			if (D3D)
			{
				if (!FullScreen)
				{
					Resizing = true;
					UserD3DDevice();
					Resizing = false;
				}
			}

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------

		}
		break;

	case WM_ACTIVATEAPP:			// 0x01C
		AppActive = (wParam != FALSE);

		LMouseDown = false;
		RMouseDown = false;

		if (FullScreen)
		{
			if (AppActive)
			{
				::ShowWindow(MainFrame, SW_SHOWMAXIMIZED);

				DEVMODE dm;
				MemFill(&dm, sizeof(dm), 0);
				dm.dmSize = sizeof(dm);
				dm.dmBitsPerPel = FullScreenBits;
				dm.dmPelsWidth = FullScreenWidth;
				dm.dmPelsHeight = FullScreenHeight;
				dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

				::ChangeDisplaySettings(&dm, CDS_FULLSCREEN);

#ifndef _MINI
				D3DM_Reset();
				D3DF_Reset();
				D3DL_Reset();
				D3DT_Reset();

				ReSet3DCount = 0;				// resetování opakované inicializace
				ReSet3DCountN = 1;				// inicializace pøíštího èítaèe
#endif // _MINI
			}
			else
			{
				::ChangeDisplaySettings(NULL, 0);
				::ShowWindow(MainFrame, SW_SHOWMINIMIZED);
			}
		}

//		if (FullScreen && (DirectSurface != NULL))
//		{
//			DirectSurface->Restore();
//		}

	// obnovení palet - jen podle nìkterıch programù, nevím zda je nutné
//		if ((DirectSurface != NULL) && (DirectPalette != NULL))
//		{
//			DirectSurface->SetPalette(DirectPalette);
//		}

		pD3AktPalette(MainFrame);

		AktPrevWindow();
		break;

/*
	case WM_ACTIVATE:
		if (D3D && (Direct3DRMDev2 != NULL))
		{
			LPDIRECT3DRMWINDEVICE windev = NULL;
			if ((Direct3DRMDev2->QueryInterface(IID_IDirect3DRMWinDevice, (void**)&windev) == DD_OK) &&
				(windev != NULL))
			{
				windev->HandleActivate((WORD)wParam);
				windev->Release();
			}
		}
		break;
*/

	case WM_SYSCOMMAND:
		if (wParam == SC_SCREENSAVE)
		{
			if (FullScreen ||
				(Win[0].AlwaysTop &&
				!Win[0].HasBorder &&
				!Win[0].HasCaption))
			{
				return FALSE;
			}
		}

//		if (FullScreen)
//		{
//			switch (wParam)
//			{
//			case SC_MOVE:
//			case SC_SIZE:
//			case SC_MAXIMIZE:
//			case SC_KEYMENU:
//			case SC_MONITORPOWER:
//				return TRUE;
//			}
//		}
		break;

	case WM_SETCURSOR:
		{
			POINT pt;
			::GetCursorPos(&pt);	// naètení souøadnic myši
			::ScreenToClient(Win[DialogParent].Wnd, &pt); // pøevod na klientské souøadnice
			if (OnSetCursor(pt.x, pt.y))
			{
				return TRUE;
			}
		}
		break;

	case WM_ERASEBKGND:
		if (item != &Win[0])
		{
			HDC dc = (HDC)wParam;
			RECT rc;
			rc.left = 0;
			rc.top = 0;
			rc.right = item->ClientWidth;
			rc.bottom = item->ClientHeight;
			::FillRect(dc, &rc, item->FontBrush);
		}
		return TRUE;

	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORDLG:
	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORMSGBOX:
	case WM_CTLCOLORSCROLLBAR:
		{
			HDC dc = (HDC)wParam;
			HWND wnd = (HWND)lParam;

			int id = ::GetWindowLong(wnd, GWL_ID);
			if (Win.IsValid(id))
			{
				WINITEM* item = &Win[id];
				if (item->Wnd == wnd)
				{
					::SetTextColor(dc, item->FontCol);
					::SetBkColor(dc, item->FontBack);
					return (LRESULT) item->FontBrush;
				}
			}
		}
		break;

	case WM_DRAWITEM:
		{
			int id = LOWORD(wParam);
			LPDRAWITEMSTRUCT dis = (LPDRAWITEMSTRUCT)lParam;

			if ((dis->CtlType == ODT_STATIC) && Win.IsValid(id))
			{
				WINITEM* item = &Win[id];
				HDC dc = dis->hDC;

				switch (item->Typ)
				{
				case WINITEM_WINPIC:
					{
						CPicture pic = item->Picture;
						pic.DeComp();

						HPALETTE OldPal = ::SelectPalette(dc, StdPalette, FALSE);
						::RealizePalette(dc);

						int width = pic.Width();
						int height = pic.Height();

						StdBitmapInfo->bmiHeader.biWidth = width;
						StdBitmapInfo->bmiHeader.biHeight = height;

						BYTE* data = pic.DataData();

						int widthbyte = (width + 3) & ~3;

						if (widthbyte != width)
						{
							if (item->PictureData == NULL)
							{
								BYTE* src = data;
								BYTE* dst = (BYTE*)MemGet(widthbyte * height);
								item->PictureData = dst;

								for (int i = height; i > 0; i--)
								{
									MemCopy(dst, src, width);
									src += width;
									dst += widthbyte;
								}
							}
							data = item->PictureData;
						}

						::SetStretchBltMode(dc, COLORONCOLOR);

						::StretchDIBits(dc,
							dis->rcItem.left, dis->rcItem.top,
							(dis->rcItem.right - dis->rcItem.left),
							(dis->rcItem.bottom - dis->rcItem.top),
							0, 0, width, height, 
							data, StdBitmapInfo,
							DIB_RGB_COLORS, SRCCOPY);

						::SelectPalette(dc, OldPal, FALSE);
					}
					break;
				}
			}
		}
		break;

// obsluha DDE
#ifndef _MINI

	case WM_DDE_ACK:
		{
			HWND serv = (HWND)wParam;

			if (DDEAppLoad && (serv != MainFrame))
			{
				ATOM app = LOWORD(lParam);
				ATOM top = HIWORD(lParam);

				LPTSTR buf = (LPTSTR)MemGet(256);
				CString appname(buf, ::GlobalGetAtomName(app, buf, 256));
				CString topname(buf, ::GlobalGetAtomName(top, buf, 256));

				if (appname.Length() > 0)
				{
					int iapp;
					int iserv = -1;

		// vyhledání aplikace stejného jména
					DDEAPPITEM* appitem = DDEAppList;

					for (iapp = 0; iapp < DDEAppNum; iapp++)
					{
						if (appitem->AppName == appname)
						{
							iserv = appitem->ServFirst;
							break;
						}

						appitem++;
					}

		// nová aplikace
					if (iapp == DDEAppNum)
					{
						if (iapp >= DDEAppMax)
						{
							if (DDEAppMax == 0) DDEAppMax = 8;
							DDEAppMax *= 2;
							MemBuf(DDEAppList, DDEAppMax);
						}

						DDEAppNum = iapp + 1;
						appitem = DDEAppList + iapp;

						appitem->AppName.Init(appname);
						appitem->ServFirst = DDEServNum;
					}

		// vyhledání serveru s tímto handle
					DDESERVITEM* servitem;
					int itop;

					while (iserv >= 0)
					{
						servitem = DDEServList + iserv;

						if (servitem->ServHandle == serv)
						{
							itop = servitem->TopicFirst;

							while (itop >= 0);
							{
								DDETOPITEM* topitem = DDETopList + itop;

								itop = topitem->TopicNext;

								if (itop < 0) topitem->TopicNext = DDETopNum;
							}

							break;
						}

						iserv = servitem->ServNext;
						if (iserv < 0) servitem->ServNext = DDEServNum;
					}

		// novı server
					if (iserv < 0)
					{
						iserv = DDEServNum;

						if (iserv >= DDEServMax)
						{
							if (DDEServMax == 0) DDEServMax = 8;
							DDEServMax *= 2;
							MemBuf(DDEServList, DDEServMax);
						}

						DDEServNum = iserv + 1;

						servitem = DDEServList + iserv;

						servitem->AppIndex = iapp;

						CString hx;
						hx.Hex((DWORD)serv);
						CString servname('[');
						servname += hx;
						servname.Add("] ", 2);
						servname += appname;

						servitem->ServName.Init(servname);
						servitem->ServNext = -1;
						servitem->ServHandle = serv;
						servitem->ServServ = 0;
						servitem->Execute.Init();
						servitem->TopicFirst = DDETopNum;
					}

		// pøidání tématu
					itop = DDETopNum;

					if (itop >= DDETopMax)
					{
						if (DDETopMax == 0) DDETopMax = 8;
						DDETopMax *= 2;
						MemBuf(DDETopList, DDETopMax);
					}

					DDETopNum = itop + 1;

					DDETOPITEM* topitem = DDETopList + itop;

					topitem->ServIndex = iserv;
					topitem->TopicName.Init(topname);
					topitem->TopicNext = -1;
					topitem->TopicServ = 0;
					topitem->DataFirst = -1;
				}
			}
		}
		break;

#endif // _MINI

	}

	return ::DefWindowProc(hWnd, nMsg, wParam, lParam);
}
