
#define	DIRECT3D_VERSION 0x0800

#include "Main.h"

/***************************************************************************\
*																			*
*							Hlavn� okno aplikace							*
*																			*
\***************************************************************************/

// v�eobecn� informace o okn�
bool				Resizing			= false;	// prob�h� programov� zm�na okna
bool				AppActive			= true;		// aplikace je aktivn�
bool				AppMinim			= false;	// aplikace minimalizovan�

// parametry celoobrazovkov�ho m�du pro aktivaci aplikace
int					FullScreenWidth		= 640;
int					FullScreenHeight	= 480;
int					FullScreenBits		= 16;

// celoobrazovkov� re�im
bool				FullScreen			= false;	// je celoobrazovkov� re�im
BOOL				OldMaximized		= FALSE;	// uschovan� p��znak maximalizace
int					NumVideoModes		= 0;		// po�et videom�d� v tabulce
VIDEOMODEITEM*		TabVideoModes		= NULL;		// tabulka videom�d�
bool				IsVideoModes		= false;	// seznam videom�d� je inicializov�n

// po�adovan� u�ivatelsk� videom�d
int					UserScreenWidth		= 0;		// u�ivatelsk� ���ka videom�du (0=implicitn�)
int					UserScreenHeight	= 0;		// u�ivatelsk� v��ka videom�du (0=implicitn�)
int					UserScreenBits		= 0;		// u�ivatelsk� po�et bit� (0=implicitn�)


// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

// ----------------------------------------------------------------------------
// !!! POZOR POZOR - funkce Direct3D p�ep�naj� koprocesor na float p�esnost !!!
// ----------------------------------------------------------------------------

// p��znak aktivn�ho re�imu 3D
bool				D3D					= false;	// je zobrazeno okno 3D

// zvolen� rozhran� a ovlada�
int					D3DIntUser			= 0;		// u�ivatelem zadan� rozhran� 0 a� D3INTERFACES-1 (0=automatick�)
int					D3DIntAkt			= 0;		// typ aktivn�ho rozhran� (0=nen�,1=GL0,2=GL1,3=GL2,4=DX3,5=DX5,6=DX6,7=DX7,8=DX8)
int					D3DDevUser			= 0;		// u�ivatelem zadan� typ ovlada�e 0 a� D3DEVICES-1 (0=automatick�)
int					D3DDevAkt			= 0;		// typ aktivn�ho ovlada�e (0=nen�,1=HAL,2=TnLHal,3=REF,4=RGB,5=MMX,6=Ramp)

// funkce aktivn�ho rozhran� (jsou v�y platn� i bez aktivn�ho ovlada�e)
D3STOP				pD3Stop				= D3NoStop;			// ukon�en� funkce za��zen�
D3SIZEVIEW			pD3SizeView			= D3NoSizeView;		// aktualizace viewportu po zm�n� velikosti
D3MOVEVIEW			pD3MoveView			= D3NoMoveView;		// aktualizace viewportu po p�esunu
D3DISP				pD3Disp				= D3NoDisp;			// zobrazen� renderovac�ho bufferu
D3CLEAR				pD3Clear			= D3NoClear;		// vymaz�n� renderovac� plochy
D3AKTPROJ			pD3AktProj			= D3NoAktProj;		// aktualizace projek�n� matice
D3AKTVIEW			pD3AktView			= D3NoAktView;		// aktualizace pohledov� matice
D3BEGIN				pD3Begin			= D3NoBegin;		// zah�jen� renderov�n� sc�ny
D3END				pD3End				= D3NoEnd;			// ukon�en� renderov�n� sc�ny
D3RENDER			pD3Render			= D3NoRender;		// vyrenderov�n� jednoho objektu
D3RESET				pD3Reset			= D3NoReset;		// resetov�n� objektu p�ed ukon�en�m ovlada�e
D3SRESET			pD3SReset			= D3NoSReset;		// resetov�n� st�nu objektu p�ed ukon�en�m ovlada�e
D3AKTWIREFRAME		pD3AktWireframe		= D3NoAktWireframe;	// aktualizovat p�ep�na� vypl�ov�n� ploch
D3AKTLIGHTON		pD3AktLightOn		= D3NoAktLightOn;	// aktualizovat p�ep�na� osv�tlen�
D3AKTSHADES			pD3AktShades		= D3NoAktShades;	// aktualizovat p�ep�na� plynul�ho st�nov�n�
D3AKTCULLING		pD3AktCulling		= D3NoAktCulling;	// aktualizovat p�ep�na� odstra�ov�n� ploch
D3AKTMAGFILTER		pD3AktMagFilter		= D3NoAktMagFilter;	// aktualizovat p�ep�na� filtrace zv�t�en�ch textur
D3AKTMINMIPFILTER	pD3AktMinMipFilter	= D3NoAktMinMipFilter; // aktualizovat p�ep�na� filtrace zmen�en�ch a vzd�len�ch textur
D3AKTLIGHT			pD3AktLight			= D3NoAktLight;		// aktualizace polo�ky sv�tla
D3SETMATERIAL		pD3SetMaterial		= D3NoSetMaterial;	// nastaven� materi�lu
D3TEXTURERES		pD3TextureRes		= D3NoTextureRes;	// resetov�n� polo�ky textury p�ed ukon�en�m ovlada�e
D3TEXTURESET		pD3TextureSet		= D3NoTextureSet;	// nastaven� a zapnut� textury
D3TEXTUREON			pD3TextureOn		= D3NoTextureOn;	// zapnut� p�vodn� textury
D3TEXTUREOFF		pD3TextureOff		= D3NoTextureOff;	// vypnut� textury
D3MATRESET			pD3MatReset			= D3NoMatReset;		// resetov�n� materi�lu
D3LIGHTRESET		pD3LightReset		= D3NoLightReset;	// resetov�n� sv�tla
D3AKTAMBIENT		pD3AktAmbient		= D3NoAktAmbient;	// aktualizace ambient osv�tlen�
D3FREE				pD3Free				= D3NoFree;			// voln� videopam�
D3AKTFOG			pD3AktFog			= D3NoAktFog;		// aktualizace mlhy
D3FOGON				pD3FogOn			= D3NoFogOn;		// zapnut� mlhy
D3AKTSTATE			pD3AktState			= D3NoAktState;		// aktualizace stavu
D3AKTBLEND			pD3AktBlend			= D3NoAktBlend;		// aktualizace blending operac� objektu
D3AKTALPHA			pD3AktAlfa			= D3NoAktAlfa;		// aktualizace referen�n� �rovn� alfa
D3AKTPALETTE		pD3AktPalette		= D3NoAktPalette;	// aktualizace palet
D3SETCOLOROP		pD3SetColorOp		= D3NoSetColorOp;	// nastaven� operace barev
D3SETALPHAOP		pD3SetAlphaOp		= D3NoSetAlphaOp;	// nastaven� operace alfa
D3SETADDRESS		pD3SetAddress		= D3NoSetAddress;	// nastaven� adresov�n� textur
//D3SETBORDER			pD3SetBorder		= D3NoSetBorder;	// nastaven� barvy okol� textur
D3SETBIAS			pD3SetBias			= D3NoSetBias;		// nastaven� zjemn�n� vzd�len�ch textur
D3SETTFACTOR		pD3SetTFactor		= D3NoSetTFactor;	// nastaven� barvy faktoru textur
D3SETZFUNC			pD3SetZFunc			= D3NoSetZFunc;		// nastaven� Z funkce (1 a� 8, v k�du DirectX)
D3SETZWRITE			pD3SetZWrite		= D3NoSetZWrite;	// nastaven� hloubkov�ho z�pisu
D3SENABLE			pD3SEnable			= D3NoSEnable;		// povolen� stencil operac�
D3SETSOP			pD3SetSOp			= D3NoSetSOp;		// nastaven� stencil operac�
D3SETSFUNC			pD3SetSFunc			= D3NoSetSFunc;		// nastaven� stencil testu
D3SETSMASK			pD3SetSMask			= D3NoSetSMask;		// nastaven� z�pisov� masky do stencil bufferu
D3SHADOW			pD3Shadow			= D3NoShadow;		// vyrenderov�n� st�nu jednoho objektu
D3SHADOWS			pD3Shadows			= D3NoShadows;		// vykreslen� st�n�

// rozm�ry okna Direct3D zadan� u�ivatelem, bez omezen� rozsahu
int					D3DX0				= 0;		// po��tek X okna Direct3D (zleva) zadan� u�ivatelem
int					D3DY0				= 0;		// po��tek Y okna Direct3D (zdola) zadan� u�ivatelem
int					D3DW0				= 0;		// ���ka okna Direct3D zadan� u�ivatelem
int					D3DH0				= 0;		// v��ka okna Direct3D zadan� u�ivatelem

// rozm�ry okna Direct3D zadan� u�ivatelem, s omezen�m na plochu (pro zobrazen� grafiky)
int					D3DX				= 0;		// po��tek X okna Direct3D (zleva) - zad�no
int					D3DY				= 0;		// po��tek Y okna Direct3D (zdola) - zad�no
int					D3DW				= 0;		// ���ka okna Direct3D - zad�no
int					D3DH				= 0;		// v��ka okna Direct3D - zad�no
bool				D3D2D				= false;	// je zobrazena 2D grafika (spolu s oknem 3D)

// skute�n� rozm�ry okna Direct3D na obrazovce (omezen� na klientsk� sou�adnice okna)
int					D3DLeft				= 0;		// skute�n� po��tek X okna Direct3D
int					D3DTop				= 0;		// skute�n� po��tek Y okna Direct3D (shora)
int					D3DWidth			= 0;		// skute�n� ���ka okna Direct3D
int					D3DHeight			= 0;		// skute�n� v��ka okna Direct3D
double				D3DWidthHeight2		= 1;		// odmocnina s pom�ru D3DWidth/D3DHeight
bool				D3DFull				= false;	// okno 3D vyu��v� celou plochu hlavn�ho okna

// pozad� sc�ny
DWORD				D3DBackCol			= 0;		// barva pozad� sc�ny ve form�tu Petra BGR(MAXDWORD=nemazat, MAXDWORD-1=neinicializovat)
D3DCOLOR			D3DBackColRGB		= 0xff000000;	// barva pozad� sc�ny ve form�tu RGB
D3DCOLORVALUE		D3DBackColF			= {0, 0, 0, 1};	// barva pozad� sc�ny ve form�tu float
bool				D3DBackColAkt		= false;	// barva pozad� zm�n�na
//int					D3DBackMat			= 0;		// materi�l pozad� sc�ny
//D3DMATERIAL8		D3DBackMatMat;					// buffer materi�lu pozad� sc�ny
int					D3DBackText			= -1;		// textura pozad� sc�ny (-1=nen�)
bool				D3DBackTextAkt		= false;	// textura pozad� zm�n�na

// nastaven� mlhy
BOOL				D3DFogOn			= -1;		// mlha je aktu�ln� zapnuta (-1 = nezn�m�)
bool				FogAkt				= true;		// mlhu je nutno aktualizovat
bool				FogKorig			= false;	// korigovan� mlha (p�i��t� se +4 k typu mlhy)
bool				FogOn				= false;	// mlha je zapnuta
DWORD				FogColor			= MAXDWORD;	// barva mlhy ve form�tu Petra BGR (MAXDWORD=vypnuto)
DWORD				FogColorRGB			= 0xff000000; // barva mlhy ve form�ty RGB
D3DCOLORVALUE		FogColorF			= {0, 0, 0, 1}; // barva mlhy ve form�tu float
int					FogType				= 1;		// typ mlhy (0=line�rn�, 1=exponenci�ln�, 2=kvadratick�)
double				FogStart			= 0;		// za��tek line�rn� mlhy
double				FogEnd				= 20;		// konec line�rn� mlhy
double				FogDens				= 0.1;		// hustota exponenci�ln� a kvadratick� mlhy (0 a� 1)

// ambient osv�tlen�
DWORD				D3DAmbientCol		= 0x404040;	// ambient osv�tlen� ve form�tu Petra BGR
DWORD				D3DAmbientColRGB	= 0xff404040; // ambient osv�tlen� ve form�tu RGB
#define AMBCOL ((float)0.25)
D3DCOLORVALUE		D3DAmbientColF		= {AMBCOL, AMBCOL, AMBCOL, 1};	// ambient osv�tlen� ve form�tu float
bool				D3DAmbientColAkt	= true;		// ambient osv�tlen� zm�n�no

// projek�n� matice (pozor, �len _34 matice nesm� b�t < 0 !!!)
bool				D3DProjAkt			= false;	// po�adavek aktualizace projek�n� matice
double				D3DFrontClip		= 1;		// vzd�lenost p�edn� omezuj�c� roviny
double				D3DBackClip			= 100;		// vzd�lenost zadn� omezuj�c� roviny
D3DMATRIX			D3DProjMatrix;					// projek�n� matice
int					D3DProjection		= 0;		// typ projekce (0=persp, 1=orto, 2=r-persp, 3=r-orto)
bool				D3DRightHand		= false;	// pravoruk� projekce (typ projekce 2 a 3)

// pohledov� matice (odvozen� z kamery)
bool				D3DViewAkt			= false;	// po�adavek aktualizace pohledov� matice
D3DMATRIX			D3DViewMatrix;					// pohledov� matice (vytvo�en� z objektu kamery)

// frustum (odvozeno z pohledov� a projek�n� matice)
bool				D3DFrustumAkt		= false;	// aktualizovat frustum
PLANE				D3DFrustum[6];					// 6 rovin projek�n�ho jehlanu

// p�ep�na�e nastaven�
bool				D3DWireframeUnknown	= true;		// stav p�ep�na�e vypl�ov�n� ploch nezn�m�
bool				D3DWireframeAkt		= false;	// vypl�ov�n� ploch aktu�ln� zapnut
bool				D3DWireframeGlobal	= true;		// vypl�ov�n� ploch glob�ln� povoleno

bool				D3DLightOnUnknown	= true;		// stav p�ep�na�e osv�tlen� nezn�m�
bool				D3DLightOnAkt		= false;	// osv�tlen� aktu�ln� zapnuto
bool				D3DLightOnGlobal	= true;		// osv�tlen� glob�ln� povoleno

bool				D3DShadesUnknown	= true;		// stav p�ep�na�e plynul�ho st�nov�n� nezn�m�
bool				D3DShadesAkt		= false;	// plynul� st�nov�n� aktu�ln� zapnuto
bool				D3DShadesGlobal		= true;		// plynul� st�nov�n� glob�ln� povoleno

bool				D3DCullingUnknown	= true;		// stav p�ep�na�e odstra�ov�n� ploch nezn�m�
short				D3DCullingAkt		= 0;		// volba zobrazen�ch ploch (0=ob�, 1=p�edn�, 2=zadn�)
bool				D3DCameraInvert		= false;	// je inverzn� kamera, invertovat plo�ky

bool				D3DMagFilterUnknown[MAX_STAGES]; // stav p�ep�na�e filtrace zv�t�en�ch textur nezn�m�
bool				D3DMagFilterAkt[MAX_STAGES];	// filtrace zv�t�en�ch textur zapnuta
bool				D3DMagFilterGlobal	= true;		// filtrace zv�t�en�ch textur glob�ln� povolena

bool				D3DMinMipFilterUnknown[MAX_STAGES];	// stav p�ep�na�e filtrace zmen�en�ch a vzd�len�ch textur nezn�m�
bool				D3DMinFilterAkt[MAX_STAGES];	// filtrace zmen�en�ch textur zapnuta
bool				D3DMinFilterGlobal	= true;		// filtrace zmen�en�ch textur glob�ln� povolena
bool				D3DMipFilterAkt[MAX_STAGES];	// filtrace vzd�len�ch textur zapnuta
bool				D3DMipFilterGlobal	= true;		// filtrace vzd�len�ch textur glob�ln� povolena

int					D3DAktStage			= 0;		// aktivn� stupe� textur (0 a� 7)

bool				D3DRamp				= false;	// je ovlada� Ramp

bool				D3DVSync			= false;	// synchronizovat s VSYN

bool				D3DVertFog			= false;	// pouze vektorov� mlha (SW driver)

bool				D3DStateAkt			= false;	// aktualizovat stav

bool				D3DReturn			= false;	// blokov�n� Alt+Enter p�i D3D m�du

// form�ty textur (dwRGBBitCount=0 pro neplatn� form�t)
DDPIXELFORMAT		D3DTextFormatA8R8G8B8;			// form�t textury D3DFMT_A8R8G8B8
DDPIXELFORMAT		D3DTextFormatA4R4G4B4;			// form�t textury D3DFMT_A4R4G4B4
DDPIXELFORMAT		D3DTextFormatA1R5G5B5;			// form�t textury D3DFMT_A1R5G5B5
DDPIXELFORMAT		D3DTextFormatR8G8B8;			// form�t textury D3DFMT_R8G8B8
DDPIXELFORMAT		D3DTextFormatR5G6B5;			// form�t textury D3DFMT_R5G6B5
DDPIXELFORMAT		D3DTextFormatX1R5G5B5;			// form�t textury D3DFMT_X1R5G5B5
DDPIXELFORMAT		D3DTextFormatX8R8G8B8;			// form�t textury D3DFMT_X8R8G8B8
DDPIXELFORMAT		D3DTextFormatX4R4G4B4;			// form�t textury D3DFMT_X4R4G4B4
bool				D3DLowText			= false;	// pou��vat ni��� kvalitu textur
bool				D3DTextFormatOK		= false;	// nalezen platn� form�t textur

// form�ty Z-buffer� (dwZBufferBitDepth=0 pro neplatn� form�t) - jen pro hled�n� form�tu Z bufferu
DDPIXELFORMAT		D3DZBufferFormat24S8;			// form�t bufferu 24 bit�, stencil 8 bit�
DDPIXELFORMAT		D3DZBufferFormat24S4;			// form�t bufferu 24 bit�, stencil 4 bity
DDPIXELFORMAT		D3DZBufferFormat15S1;			// form�t bufferu 15 bit�, stencil 1 bit
DDPIXELFORMAT		D3DZBufferFormat32S0;			// form�t bufferu 32 bit�, stencil 0 bit�
DDPIXELFORMAT		D3DZBufferFormat24S0;			// form�t bufferu 24 bit�, stencil 0 bit�
DDPIXELFORMAT		D3DZBufferFormat16S0;			// form�t bufferu 16 bit�, stencil 0 bit�
DDPIXELFORMAT		D3DZBufferFormat8S0;			// form�t bufferu 8 bit�, stencil 0 bit�
bool				D3DZBufferFormatOK	= false;	// nalezen platn� form�t Z-bufferu

// stencil buffer
bool				D3DSBufferFormatOK	= false;	// nalezen Stencil buffer
int					D3DSBufferDepth		= 0;		// hloubka Stencil bufferu (bit�)
int					D3DSBufferMax		= 0;		// maxim�ln� hodnota Stencil bufferu
bool				D3DSBufferClear		= false;	// po�adavek k vymaz�n� Stencil bufferu
bool				D3DShadows			= true;		// povoleno pou��v�n� st�n�

// vlastnosti za��zen�
int					MaxTextureWidth		= 0x8000;	// maxim�ln� ���ka textury
int					MaxTextureHeight	= 0x8000;	// maxim�ln� v��ka textury
int					MaxTextureRepeat	= 0x8000;	// maxim�ln� po�et opakov�n� textury
bool				SquareTexture		= false;	// textury mus� b�t �tvercov�
bool				TexturePow2			= true;		// rozm�r textur mus� b�t mocnina 2 (pro MipMap je nutn�)
bool				IsBlending			= true;		// jsou blending operace
bool				IsMagFilter			= true;		// podporuje filtr p�i zv�t�en�
bool				IsMinFilter			= true;		// podporuje filtr p�i zmen�en�
bool				IsMipMap			= true;		// podporuje mipmap textury
bool				IsHWRaster			= false;	// je HW rasterizace
bool				IsHWTransLight		= false;	// je HW transformace a osv�tlen�
bool				IsPureDevice		= false;	// pouze HW obsluha
bool				AlphaGreaterEqual	= true;		// lze porovn�vat Alpha >=
bool				AlphaGreater		= true;		// lze porovn�vat Alpha >
int					MaxVertexNum		= 0xf000;	// maxim�ln� po�et vrchol� skute�n�
int					MaxVertexNum2		= MAXVERTICES;	// maxim�ln� po�et vrchol� pro renderov�n�
int					MaxTextureStages	= MAX_STAGES; // maxim�ln� po�et stup�� textur (nepou��v� se)
int					MaxTextureSimult	= MAX_STAGES; // maxim�ln� po�et textur pro jeden pr�chod
bool				IsTableFog			= true;		// podporov�na tabulkov� mlha
bool				IsVertexFog			= true;		// podporov�na vrcholov� mlha
bool				IsRangeFog			= true;		// podporov�na vzd�lenostn� mlha

int					D3DLevel			= 4;		// slo�itost objekt� (2 a v�ce)
bool				D3DCreateUpper		= true;		// vytv��et horn� podstavu objekt�
bool				D3DCreateLower		= true;		// vytv��et doln� podstavu objekt�
int					D3D_ID				= 1;		// aktivn� objekt (r�m) Direct3D
int					TerenID				= -1;		// naposledy zvolen� objekt ter�nu (-1=nen�)

double				D3DSmooth			= 1;		// vyhlazen� textur (1=ne)

// tabulka ID k�d� ovlada�� Direct3D

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

// m��en� uplynul�ho �asu
double D3DLastTime = 0;							// posledn� �as renderov�n�
double D3DElapsedTime = 0;						// uplynul� �as od posledn�ho renderov�n�
double D3DAverageTime = 0;						// st�edn� ub�hl� �as
double D3DAverageFreq = 0;						// st�edn� frekvence
int D3DAverageFreqI = 0;						// frekvence Integer
double D3DAverageRound = 0;						// zaokrouhlovac� korekce
int D3DValidTime = 0;							// ��ta� do zah�jen� m��en� �asu

//int	D3DTimeStamp = 0;							// ��ta� sn�mk� (jako �asov� raz�tko)

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------

/////////////////////////////////////////////////////////////////////////////
// glob�ln� a lok�ln� prom�nn�

int			MainFrameStyleEx = (WS_EX_APPWINDOW | WS_EX_CONTROLPARENT | WS_EX_WINDOWEDGE); // roz���en� styl
int			MainFrameStyle = (WS_BORDER | WS_CAPTION | /* WS_CLIPCHILDREN | */ WS_MAXIMIZEBOX |
				WS_MINIMIZEBOX | /* WS_CLIPSIBLINGS | */ WS_OVERLAPPED | WS_SYSMENU | WS_THICKFRAME); // styl okna

TCHAR		MainFrameClass[] = _T("PeterProgClass");	// n�zev t��dy hlavn�ho okna
HWND		MainFrame = NULL;						// hlavn� okno aplikace
bool		MainFrameVisible = false;				// hlavn� okno zobrazeno
HWND		StatusBar = NULL;						// stavov� li�ta

bool		StatusVisible = false;					// p��znak viditelnosti stavov�ho ��dku
HFONT		StatusFont = NULL;						// font stavov�ho ��dku

RECT		ClientRect;								// klientsk� oblast hlavn�ho okna
int			ClientWidth = 0;						// ���ka klientsk� oblasti
int			ClientHeight = 0;						// v��ka klientsk� oblasti

int			MainFrameX;								// sou�adnice X st�edu hlavn�ho okna
int			MainFrameY;								// sou�adnice Y st�edu hlavn�ho okna

bool		ReqClose = false;						// po�adavek uzav�en� okna od u�ivatele

CString		AktCaptionText;							// text titulku hlavn�ho okna
CString		AktStatusText;							// zobrazen� text stavov� li�ty

HCURSOR		CurArrow;								// (standardn�) �ipka
HCURSOR		CurArrow2;								// (standardn�) �ipka pro DirectX

// stav my�i (aktualizov�no v PreTranslateMessage)
bool		LMouseDown = false;						// lev� tla��tko stisknuto
bool		LMouseClick = false;					// klik lev�ho tla��tka
bool		LMouseDClick = false;					// dvojklik lev�ho tla��tka

bool		RMouseDown = false;						// prav� tla��tko stisknuto
bool		RMouseClick = false;					// klik prav�ho tla��tka
bool		RMouseDClick = false;					// dvojklik prav�ho tla��tka

POINT		MouseScreen = {MOUSEINV, MOUSEINV};		// sou�adnice my�i na displeji
POINT		MouseMain = {MOUSEINV, MOUSEINV};		// sou�adnice my�i v hlavn�m okn�
bool		MouseValid = false;						// p��znak aktualizace sou�adnice my�i

bool		ProgramFill = false;					// pln�n� prvku okna programem

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

// buffer hledan�ho textu v okn� RichEdit
#define		FINDBUFFSIZE 128						// velikost bufferu hledan�ho textu
LPTSTR		FindBuff = NULL;						// buffer hledan�ho textu (v okn� RichEdit)
bool		FindMatchCase = false;					// p�i hled�n� rozli�ena velk� a mal� p�smena
bool		FindWholeWord = false;					// hledat jen cel� slova
HWND		FindDialogBox = NULL;					// handle na dialog hled�n� (NULL=nen�)
UINT		FindMsgString = 0;						// identifik�tor zpr�vy hled�n�
FINDREPLACE FindStruc;								// struktura pro hled�n�


// ============================== hlavn� okno ===============================

/////////////////////////////////////////////////////////////////////////////
// p��prava po�adovan�ch rozm�r� okna podle velikosti hlavn� plochy

void AdjustMainFrame(RECT* wrc)
{
// po�adovan� rozm�ry plochy
	ASSERT(Map.IsValid(0));
	int width = RoundP(ICONWIDTH * Map[0].Width() * Meritko0);
	int height = RoundP(ICONHEIGHT * Map[0].Height() * Meritko0);
	if (width > ScreenWidth0) width = ScreenWidth0;
	if (height > ScreenHeight0) height = ScreenHeight0;

// zji�t�n� po�adovan�ch rozm�r� okna
	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = width;
	rc.bottom = height;
	::AdjustWindowRectEx(&rc, MainFrameStyle, FALSE, MainFrameStyleEx);

// korekce o v��ku stavov� li�ty
	if (StatusVisible)
	{
		RECT src;
		if (::GetWindowRect(StatusBar, &src))
		{
			rc.bottom += src.bottom - src.top;
		}
	}

// korekce rozm�r�
	width = rc.right - rc.left;
	height = rc.bottom - rc.top;
	if (width > ScreenWidth0) width = ScreenWidth0;
	if (height > ScreenHeight0) height = ScreenHeight0;

// sou�adnice
	int left = MainFrameX - width/2;
	if (left < 0) left = 0;
	int top = MainFrameY - height/2;
	if (top < 0) top = 0;

// ulo�en� rozm�r�
	wrc->left = left;
	wrc->top = top;
	wrc->right = left + width;
	wrc->bottom = top + height;
}

/////////////////////////////////////////////////////////////////////////////
// prvn� zobrazen� hlavn�ho okna

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
// vytvo�en� hlavn�ho okna aplikace

bool MainFrameCreate()
{
// text jm�na programu
	for (int i = BufObjN-1; i >= 0; i--)
	{
		if ((BufObj[i].DatBlok == BufLocID) && (BufObj[i].DatIndex == 0))
		{
			AktCaptionText = Text.Get(BufObj[i].Name);
			break;
		}
	}

// na�ten� standardn�ch barev okenn�ch prvk�
	StdColorBtnText = (::GetSysColor(COLOR_BTNTEXT) & 0xffffff);
	StdColorBtnFace = (::GetSysColor(COLOR_BTNFACE) & 0xffffff);
	StdBrushBtn = ::CreateSolidBrush(StdColorBtnFace);

	StdColorWindowText = (::GetSysColor(COLOR_WINDOWTEXT) & 0xffffff);
	StdColorWindow = (::GetSysColor(COLOR_WINDOW) & 0xffffff);
	StdBrushWindow = ::CreateSolidBrush(StdColorWindow);

	StdColorHighlightText = (::GetSysColor(COLOR_HIGHLIGHTTEXT) & 0xffffff);
	StdColorHighlight = (::GetSysColor(COLOR_HIGHLIGHT) & 0xffffff);
	StdBrushHighlight = ::CreateSolidBrush(StdColorHighlight);

// registrace t��dy okna
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

// vytvo�en� hlavn�ho okna
	RECT rc;
	AdjustMainFrame(&rc);

	MainFrame = ::CreateWindowEx(
		MainFrameStyleEx,								// roz���en� styl
		MainFrameClass,									// jm�no t��dy okna
		AktCaptionText,									// titulek okna
		MainFrameStyle,									// styl okna
		rc.left,										// X
		rc.top,											// Y
		rc.right - rc.left,								// ���ka
		rc.bottom - rc.top,								// v��ka
		NULL,											// rodi�
		NULL,											// menu
		hInstance,										// instance
		NULL);											// parametry
	if (MainFrame == NULL) return false;

// aktualizace informac� o displeji
	AktDispInfo();

// vytvo�en� prvku okna
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

// vytvo�en� stavov� li�ty - zat�m nezobrazena
#define IDN_STATUS 12524
	StatusBar = ::CreateStatusWindow(
		WS_CHILD | SBARS_SIZEGRIP,
		AktStatusText,
		MainFrame,
		IDN_STATUS);
	if (StatusBar == NULL) return false;

// na�ten� kurzor� my�i
	CurArrow = ::LoadCursor(0, MAKEINTRESOURCE(IDC_ARROW)); ASSERT(CurArrow != NULL);
	CurArrow2 = ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_ARROW2)); ASSERT(CurArrow2 != NULL);
	EmptyIconData.Cursor = CurArrow;

// aktualizace klientsk� oblasti
	InitClientRect(&Win[0]);

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// p�epo�et rozm�r� okna Direct3D (podle �daj� zadan�ch u�ivatelem)
// (mus� b�t zobrazen� okno, aby byly platn� �daje o okn�)
// vrac� TRUE=m� b�t okno 3D

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

bool RecalcD3D()
{
// p��prava rozm�r� omezen�ch na grafickou plochu
	D3DX = D3DX0;
	D3DY = D3DY0;
	D3DW = D3DW0;
	D3DH = D3DH0;

// omezen� po��tku X a Y okna na grafickou plochu
	if (D3DX < 0) D3DX = 0;
	if (D3DX > Width) D3DX = Width;
	if (D3DY < 0) D3DY = 0;
	if (D3DY > Height) D3DY = Height;

// omezen� ���ky a v��ky okna na grafickou plochu
	if (D3DW < 0) D3DW = 0;
	if (D3DH < 0) D3DH = 0;
	if ((D3DX + D3DW) > Width) D3DW = Width - D3DX;
	if ((D3DY + D3DH) > Height) D3DH = Height - D3DY;

// test, zda bude zobrazena 2D grafika
	D3D2D = ((D3DW != Width) || (D3DH != Height));

// p��prava m���tka a sou�adnic
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

// v�po�et skute�n�ch sou�adnic okna Direct3D
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

// test, zda bude cel� plocha hlavn�ho okna
	D3DFull = ((D3DWidth == ClientWidth) && (D3DHeight == ClientHeight) && !StatusVisible);

// test, zda bude okno Direct3D zapnuto
	return ((D3DWidth != 0) && (D3DHeight != 0));
}

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------


// ================================ videom�d ================================

/////////////////////////////////////////////////////////////////////////////
// set��d�n� videom�d� displeje (na za��tku je nejmen�� rozli�en� a nejni��� po�et bit�)

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
// inicializace seznamu videom�d� (vol� se p�i prvn�m pou�it�)

void InitVideoModes()
{

// test, zda byl seznam ji� sestaven
	if (!IsVideoModes)
	{
		IsVideoModes = true;

// vytvo�en� bufferu seznamu videom�d�
		TabVideoModes = (VIDEOMODEITEM*)MemGet(MAXVIDEOMODES * sizeof(VIDEOMODEITEM));

// zji�t�n� v�ech videom�d�
		int inx = 0;

		for (; NumVideoModes < MAXVIDEOMODES; inx++)
		{
			DEVMODE mode;
			MemFill(&mode, sizeof(DEVMODE), 0);
			mode.dmSize = sizeof(DEVMODE);
			mode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
			if (::EnumDisplaySettings(NULL, inx, &mode) == 0) break;

// test platnosti videom�du
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

// test, zda videom�d ji� existuje
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

// je to nov� videom�d
				if (i == 0)
				{
					item->Width = width;
					item->Height = height;
					item->Bits = bits;
					NumVideoModes++;
				}
			}
		}

// set��d�n� videom�d�
		SortDisplayModes(TabVideoModes, NumVideoModes);
	}
}

/////////////////////////////////////////////////////////////////////////////
// nalezen� nejbli���ho videom�du v tabulce (vrac� -1=nenalezeno)
// (0=parametr je implicitn�)

int FindVideoMode(int width, int height, int bits)
{
// kontrola po�tu videom�d�
	int tabn = NumVideoModes;
	if (tabn <= 0) return -1;

// implicitn� ���ka a v��ka
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

// omezen� ���ky a v��ky na maxim�ln� videom�d (aby byl nalezen alespo� 1 videom�d)
	tabn -= 1;
	VIDEOMODEITEM* tab = TabVideoModes + tabn;
	if (width > tab->Width) width = tab->Width;
	if (height > tab->Height) height = tab->Height;

// nalezen� nejvhodn�j��ho videom�du v tabulk�ch (nejbli��� vy���)
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
// stop aktivn�ho videom�du (vol�no i po ne�sp�n�m pokusu o celoobrazovkov� re�im)

void StopVideoMode()
{
// ukon�en� 3D m�du
#ifndef _MINI
	if (FullScreen && D3D) DeSelectD3DDev();
#endif

// zapnut� m�du zm�ny okna
	Resizing = true;

// vypnut� kurzoru my�i
	::ShowCursor(FALSE);

// ukon�en� videom�du
	if (FullScreen)
	{
		::ChangeDisplaySettings(NULL, 0);
	}

// obnoven� stavov�ho ��dku
	StatusAkt = true;

// n�vrat pozice okna
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

// zapnut� kurzoru my�i
	::ShowCursor(TRUE);

// vypnut� m�du zm�ny okna
	Resizing = false;

// aktualizace informac� o okn�	
	InitClientRect(&Win[0]);
	OnSize(&Win[0]);

// po�adavek k p�ekreslen� 2D grafiky
	AktAllIcon();

// start 3D m�du
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
// start videom�du (vrac� TRUE=OK)

bool StartVideoMode()
{

// ukon�en� 3D m�du, p�echodn� vypnut� 3D okno
#ifndef _MINI
	if (D3D) DeSelectD3DDev();
	int oldX = D3DW0;
	D3DW0 = 0;
#endif

// ukon�en� p�ede�l�ho videore�imu
	if (FullScreen) StopVideoMode();

// vypnut� kurzoru my�i
	::ShowCursor(FALSE);

// hlavn� okno mus� b�t zapnuto
	MainFrameShow();

// vypnut� stavov� li�ty (nesm� se te� aktualizovat, z�stal by dole pr�zdn� pruh)
	StatusAkt = false;
	SetStatusVisible(false);

// �schova parametr� hlavn�ho okna
	if (!FullScreen)
	{
		OldMaximized = ::IsZoomed(MainFrame);
	}

// korekce parametr� hlavn�ho okna na fullscreen
	::SetWindowLong(MainFrame, GWL_EXSTYLE, WS_EX_CONTROLPARENT);
	::SetWindowLong(MainFrame, GWL_STYLE, WS_POPUP | WS_VISIBLE);

// inicializace seznamu videom�d�
	InitVideoModes();

// po�adavek k p�ekreslen� 2D grafiky
	AktAllIcon();

// zapnut� m�du zm�ny okna
	Resizing = true;

// nalezen� videom�du
	int inx = FindVideoMode(UserScreenWidth, UserScreenHeight, UserScreenBits);
	if (inx < 0) inx = FindVideoMode(UserScreenWidth, UserScreenHeight, 0);

	bool isup = true;

// cyklus k ni���m videom�d�m
	for (; inx >= 0; )
	{

// popisova� videom�du
		VIDEOMODEITEM* item = TabVideoModes + inx;

// start videom�du
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

// �schova parametr� videom�du
			ScreenWidth = item->Width;
			ScreenHeight = item->Height;
			ScreenWidth0 = ScreenWidth;
			ScreenHeight0 = ScreenHeight;
			ScreenBits = item->Bits;

// korekce pozice okna
			::SetWindowPos(MainFrame, HWND_TOPMOST, 0, 0, ScreenWidth, ScreenHeight,
					SWP_DRAWFRAME | SWP_FRAMECHANGED | SWP_NOCOPYBITS | SWP_SHOWWINDOW);

// zapnut� kurzoru my�i
			::ShowCursor(TRUE);

// vypnut� m�du zm�ny okna
			Resizing = false;

// zapnut� p��znaku celoobrazovkov�ho m�du
			FullScreen = true;

// aktualizace informac� o okn�
			InitClientRect(&Win[0]);
			OnSize(&Win[0]);

// start 3D m�du
#ifndef _MINI
#ifndef _MINI
			D3DW0 = oldX;
#endif

			if (!RecalcD3D() || UserD3DDevice())
			{
				StatusAkt = true;
				return true;
			}

// pokra�uj� dal�� pokusy
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

// zapnut� kurzoru my�i
	::ShowCursor(TRUE);

// vypnut� m�du zm�ny okna
	Resizing = false;
	StatusAkt = true;

// n�vrat parametr� hlavn�ho okna
#ifndef _MINI
	D3DW0 = oldX;
#endif
	StopVideoMode();

	return false;
}

/////////////////////////////////////////////////////////////////////////////
// nastaven�/znovunastaven� implicitn�ho videom�du

void ReSetDisplayMode()
{
	StartVideoMode();
}

/////////////////////////////////////////////////////////////////////////////
// zapnut�/vypnut� celoobrazovkov�ho m�du

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
// nastaven� m�du vypl�ov�n� ploch

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
// nastaven� m�du zapnut� osv�tlen�

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
// nastaven� m�du plynul�ho st�nov�n�

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
// nastaven� m�du filtrace textur p�i zv�t�en�

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
// nastaven� m�du filtrace textur p�i zmen�en�

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
// nastaven� odstra�ov�n� ploch

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
// zapnut� mlhy

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
// resetov�n� p�ep�na�� nastaven� 

void ResetD3DSwitch()
{
	D3DWireframeUnknown	= true;		// stav p�ep�na�e vypl�ov�n� ploch nezn�m�
	D3DLightOnUnknown	= true;		// stav p�ep�na�e osv�tlen� nezn�m�
	D3DShadesUnknown	= true;		// stav p�ep�na�e plynul�ho st�nov�n� nezn�m�
	D3DCullingUnknown	= true;		// stav p�ep�na�e odstra�ov�n� ploch nezn�m�
	D3DFogOn = -1;					// stav zapnut� mlhy nezn�m�

	D3DSrcBlend = BLEND_INVALID; // aktu�ln� nastaven� blending operace zdroje
	D3DDstBlend = BLEND_INVALID; // aktu�ln� nastaven� blending operace c�le
	D3DAlphaRef = -1;			// aktu�ln� nastaven� alfa �rovn�
	D3DTFactor = MAXDWORD;			// aktu�ln� nastaven� faktoru textur
	D3DZWrite = -1;				// aktu�ln� hodnota hloubkov�ho z�pisu
	D3DZTest = -1;				// aktu�ln� hodnota hloubkov�ho testu

	for (int i = 0; i < MAX_STAGES; i++)
	{
		D3DMagFilterUnknown[i]	= true;		// stav p�ep�na�e filtrace zv�t�en�ch textur nezn�m�
		D3DMinMipFilterUnknown[i] = true;	// stav p�ep�na�e filtrace zmen�en�ch a vzd�len�ch textur nezn�m�
		D3DColorOp[i] = -1;				// aktu�ln� nastaven� operac� barev
		D3DColorArg1[i] = -1;			// aktu�ln� nastaven� argumentu 1 operac� barev
		D3DColorArg2[i] = -1;			// aktu�ln� nastaven� argumentu 2 operac� barev
		D3DAlphaOp[i] = -1;				// aktu�ln� nastaven� operac� barev
		D3DAlphaArg1[i] = -1;			// aktu�ln� nastaven� argumentu 1 operac� barev
		D3DAlphaArg2[i] = -1;			// aktu�ln� nastaven� argumentu 2 operac� barev
		D3DAddressU[i] = -1;			// aktu�ln� adresov�n� textur U
		D3DAddressV[i] = -1;			// aktu�ln� adresov�n� textur V
//		D3DBorder[i] = -1;				// aktu�ln� barva okol� textur
		D3DBias[i] = -123456;			// aktu�ln� zjemn�n� vzd�len�ch textur
	}
}

/////////////////////////////////////////////////////////////////////////////
// inicializace objekt� sc�ny (vol� se p�i startu programu, p�ed inicializac� 3D!)

void Init3D()
{
// inicializace buffer� skupin
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

// implicitn� materi�l
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

// inicializace pohledov� matice
	Matrix1(&D3DViewMatrix);

// vytvo�en� objektu sc�ny
	i = D3DF_New();
	ASSERT(i == 0);

// vytvo�en� objektu kamery
	i = D3DF_New();
	ASSERT(i == 1);
	D3DFITEM* item = D3DF_Get(i);
	item->Type = D3DFTYPE_CAMERA;
	item->TransZ = -2;
	item->AktOwn = true;

// vytvo�en� implicitn�ho sv�tla
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
// aktualizace viewportu (nap�. po zm�n� velikosti okna)
/*
bool UpdateViewport()
{
	return false;


	if (Resizing) return true;

	RecalcD3D();
	FPUFloat();
	D3DProjAkt = true;
	D3DViewAkt = true;

// reset p�ep�na�� (n�kter� ztrat� hodnotu)
	ReSet3DCount = 0;				// resetov�n� opakovan� inicializace
	ReSet3DCountN = 1;				// inicializace p��t�ho ��ta�e
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
// opakovan� inicializace v�choz�ho stavu 3D

int ReSet3DCount = 0;
int ReSet3DCountN = 1;

void ReSet3D()
{
	if (--ReSet3DCount < 0)
	{ 

// obnoven� ��ta�e
		int i = ReSet3DCountN;
		ReSet3DCount = i;
		i = (i << 1);
		if (i > 256) i = 256;
		ReSet3DCountN = i;

// restart p�ep�na��
		ResetD3DSwitch();

// opakovan� aktualizovat
		D3DProjAkt = true;				// aktualizovat projek�n� matici
		D3DViewAkt = true;				// aktualizovat pohledovou matici
		D3DFrustumAkt = true;			// aktualizovat frustum
		D3DBackColAkt = true;			// aktualizovat barvu pozad�
		FogAkt = true;					// aktualizovat mlhu
		D3DAmbientColAkt = true;		// aktualizovat ambient osv�tlen�
		D3DStateAkt = true;				// aktualizovat stav
	}
}				

/////////////////////////////////////////////////////////////////////////////
// uvoln�n� ovlada�e Direct3D

void DeSelectD3DDev()
{
// vypnut� kurzoru my�i
	::ShowCursor(FALSE);

	FPUFloat();

// �ek�n� na dokon�en� renderov�n�
	D3DVSync = false;
	pD3Disp();

// resetov�n� buffer�
	D3DM_Reset();
	D3DF_Reset();
	D3DL_Reset();
	D3DT_Reset();

// ukon�en� funkce aktivn�ho za��zen�
	pD3Stop();
	FPUDouble();

// zru�en� ��sla aktivn�ho za��zen�
	D3DIntAkt = 0;
	D3DDevAkt = 0;
	D3D = false;

// nulov�n� �as� (pro u�ivatele)
	D3DElapsedTime = 0;
	D3DAverageFreqI = 0;

// implicitn� informace o za��zen�
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

// nulov�n� adres za��zen�
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

// po�adavek k p�ekreslen� 2D grafiky
	AktAllIcon();

// obnoven� palet
	pD3AktPalette(MainFrame);

// zapnut� kurzoru my�i
	::ShowCursor(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// p�id�n� form�tu textur

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
// p�id�n� form�tu Z-bufferu

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
// v�b�r ovlada�e 3D (vrac� TRUE=inicializace OK)
// inter = rozhran� (1=GL0,2=GL1,3=GL2,4=DX3,5=DX5,6=DX6,7=DX7,8=DX8)
// device = ovlada� (1=HAL,2=TnLHal,3=REF,4=RGB,5=MMX,6=Ramp)

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
// automatick� v�b�r rozhran�

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
// v�b�r u�ivatelsk�ho ovlada�e 3D (vrac� TRUE=nastaven� OK)
// Pozn.: V t�to chv�li je ji� hlavn� okno zobrazeno (funkce FDirect3DView())

int korigSpeed = 0;		// ��ta� korekce pomal�ho rozhran�

bool UserD3DDevice()
{
// zru�en� star�ho ovlada�e
	DeSelectD3DDev();

// p�epo�et rozm�r�
	if (!RecalcD3D()) return false;

// vypnut� kurzoru my�i
	::ShowCursor(FALSE);

// zad�no rozhran�
	bool res = false;
	if (D3DIntUser > 0)
	{

// zad�no rozhran� i ovlada�
		if (D3DDevUser > 0)
		{
			res = SelectD3DDevice(D3DIntUser, D3DDevUser);
		}

// zad�no rozhran�, ovlada� je automatick�
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

// rozhran� je automatick�
	else
	{

// zad�n ovlada�, rozhran� je automatick�
		if (D3DDevUser > 0)
		{
			res = SelectD3DDeviceInt(D3DDevUser);
		}

// ovlada� i rozhran� jsou automatick�
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

// po�adavek k p�ekreslen� 2D grafiky
	AktAllIcon();

// funkce 3D za��zen� aktivov�na OK
	if (res)
	{
		D3D = true;

// aktualizace p��znaku zapnut� mlhy
		D3DCheckFogOn();

// korekce informac� o za��zen�
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

// aktualizace parametr�
		Matrix0(&D3DProjMatrix);		// nulov�n� projek�n� matice
		ReSet3DCount = 0;				// resetov�n� opakovan� inicializace
		ReSet3DCountN = 1;				// inicializace p��t�ho ��ta�e
		ReSet3D();						// resetov�n� p�ep�na�� a stav�

// nulov�n� �et�zce k renderov�n�
//		MemFill(D3D_Rend, 3*MAX_ZBIAS*sizeof(D3DFITEM_*), 0);

// inicializace m��en� �asu
		D3DLastTime = FPreciseTimer();
		D3DElapsedTime = 0;
		D3DAverageTime = 0;
		D3DAverageFreq = 0;
		D3DAverageFreqI = 0;
		D3DValidTime = 10;

// inicializace p�ep�na��
		D3DVSync = true;
		D3DVertFog = (!IsHWRaster && (D3DDevAkt != NRefDev));

		pD3AktPalette(MainFrame);

// vymaz�n� plochy
		FPUFloat();
		pD3Begin();
		pD3Clear();
		D3DSBufferClear = false;
		pD3End();
		FPUDouble();

// zapnut� kurzoru my�i
		::ShowCursor(TRUE);

		return true;
	}

// chyba inicializace
	else
	{
		D3DIntAkt = 0;
		D3DDevAkt = 0;

// zapnut� kurzoru my�i
		::ShowCursor(TRUE);

		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace projek�n� matice

void AktProjMatrix()
{
	// p�edn� a zadn� omezovac� rovina
	double front = fabs(D3DFrontClip);
	double back = fabs(D3DBackClip);

	// oprava po�ad� omezovac�ch rovin
	if (back < front)
	{
		double tmp = front;
		front = back;
		back = tmp;
	}

	// omezen� rovin (��slo je zvoleno jen
	// p�ibli�n�, nav�c aby bylo bezeztr�tov�
	// vyj�d�en� v double i float = 1/128)
	double eps = 0.0078125;
	if (front < eps) front = eps;
	if ((back - front) < eps) back = front + eps;

	// p�ibl�en� nejbli���ho zobrazen�ho objektu
	double front2 = front / 2;
	if (D3DIntAkt >= 4) front2 /= 2;	// DirectX pou��v� Z buffer 0 a� 1, OpenGL -1 a� +1

	// ortografick� projekce
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

	// koeficienty ���ky a v��ky projekce
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
// aktualizace projek�n�ho jehlanu

void AktFrustum()
{
// p��prava kombinovan� matice
	D3DMATRIX mat;
	MatrixMul(&mat, &D3DViewMatrix, &D3DProjMatrix);

// prav� rovina
	float a = mat._14 - mat._11;
	float b = mat._24 - mat._21;
	float c = mat._34 - mat._31;
	float d = mat._44 - mat._41;
	float t = (float)sqrt(a*a + b*b + c*c);

	D3DFrustum[0].A = a/t;
	D3DFrustum[0].B = b/t;
	D3DFrustum[0].C = c/t;
	D3DFrustum[0].D = d/t;

// lev� rovina
	a = mat._14 + mat._11;
	b = mat._24 + mat._21;
	c = mat._34 + mat._31;
	d = mat._44 + mat._41;
	t = (float)sqrt(a*a + b*b + c*c);

	D3DFrustum[1].A = a/t;
	D3DFrustum[1].B = b/t;
	D3DFrustum[1].C = c/t;
	D3DFrustum[1].D = d/t;

// doln� rovina
	a = mat._14 + mat._12;
	b = mat._24 + mat._22;
	c = mat._34 + mat._32;
	d = mat._44 + mat._42;
	t = (float)sqrt(a*a + b*b + c*c);

	D3DFrustum[2].A = a/t;
	D3DFrustum[2].B = b/t;
	D3DFrustum[2].C = c/t;
	D3DFrustum[2].D = d/t;

// horn� rovina
	a = mat._14 - mat._12;
	b = mat._24 - mat._22;
	c = mat._34 - mat._32;
	d = mat._44 - mat._42;
	t = (float)sqrt(a*a + b*b + c*c);

	D3DFrustum[3].A = a/t;
	D3DFrustum[3].B = b/t;
	D3DFrustum[3].C = c/t;
	D3DFrustum[3].D = d/t;

// vzd�len� rovina
	a = mat._14 - mat._13;
	b = mat._24 - mat._23;
	c = mat._34 - mat._33;
	d = mat._44 - mat._43;
	t = (float)sqrt(a*a + b*b + c*c);

	D3DFrustum[4].A = a/t;
	D3DFrustum[4].B = b/t;
	D3DFrustum[4].C = c/t;
	D3DFrustum[4].D = d/t;

// bl�zk� rovina
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
// vyrenderov�n� sc�ny

void D3Render(bool disp)
{
// test, zda je re�im 3D
	if (D3D)
	{

// zapnut� sn�en� p�esnosti procesoru
		FPUFloat();

// restart v�choz�ho stavu 3D
		ReSet3D();

// aktualizace v�ech matic objekt�
		D3DF_AktMatrix();

// aktualizace pohledov� matice (zm�n�na v aktualizaci matice kamery)
		if (D3DViewAkt)
		{
			D3DCameraInvert = (((D3DF_Data[1].ScaleX < 0) && (D3DF_Data[1].ScaleY >= 0)) ||
							((D3DF_Data[1].ScaleX >= 0) && (D3DF_Data[1].ScaleY < 0)));
			D3DViewAkt = false;
			D3DFrustumAkt = true;
			pD3AktView();
		}

// aktualizace projek�n� matice
		if (D3DProjAkt)
		{
			D3DProjAkt = false;
			D3DFrustumAkt = true;
			AktProjMatrix();
		}

// aktualizace rovin projek�n�ho jehlanu
		if (D3DFrustumAkt)
		{
			D3DFrustumAkt = false;
			AktFrustum();
		}

// zah�jen� renderov�n� sc�ny (pro OpenGL p��prava k aktualizaci sv�tel)
		pD3Begin();

// aktualizace sv�tel (mus� n�sledovat za zah�jen�m sc�ny)
		D3DL_AktLight();

// aktualizace stavu
		if (D3DStateAkt)
		{
			pD3AktState();
			D3DStateAkt = false;
		}

// inicializace Z funkce (kv�li obsluze vymaz�n� pozad�)
		D3DZWrite = TRUE;
		pD3SetZWrite(TRUE);
		D3DZTest = 0;
		pD3SetZFunc(0);

// vymaz�n� renderovac� plochy
		if (D3DBackText >= 0)
		{
			bool filtr = D3DF_Get(0)->MagFilter[0];
			SetD3DMinMipFilter(filtr, false, 0);	// filtr pro zmen�en� - p�ednastaven� nutn� pro generov�n� textury

			D3DM_Set(0);							// standardn� materi�l
			D3DT_Akt(D3DBackText, 0);				// nastaven� textury

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
			SetD3DWireframe(true);					// zapnut� m�du v�pln�
			D3DWireframeGlobal = oldfill;

			SetD3DLightOn(false);					// osv�tlen� vypnuto
			SetD3DShades(false);					// vypnuto plynul� st�nov�n�

			SetD3DCulling(1);						// zobrazena pouze p�edn� strana

			SetD3DMagFilter(filtr, 0);				// filtr pro zv�t�en�
			SetD3DMinMipFilter(filtr, false, 0);	// filtr pro zmen�en�

			D3DFITEM item;
			item.IsBlend = false;
			D3DSrcBlend = BLEND_ONE;
			item.SrcBlend = BLEND_ONE;
			D3DDstBlend = BLEND_ZERO;
			item.DstBlend = BLEND_ZERO;
			pD3AktBlend(&item);						// vypnut� blendingu

			D3DAlphaRef = 0;
			item.IsAlphaRef = false;
			item.AlphaRef = 0;
			item.AlphaRef2 = 0;
			pD3AktAlfa(&item);						// vypnut� alfa reference

			SetD3DFogOn(FALSE);						// vypnut� mlhy
		}

		pD3Clear();									// vymaz�n� plochy
		D3DSBufferClear = false;

// altualizace mlhy
		if (FogAkt)
		{
			if (FogOn) pD3AktFog();
			FogAkt = false;
		}

// aktualizace ambient osv�tlen�
		if (D3DAmbientColAkt)
		{
			pD3AktAmbient();
			D3DAmbientColAkt = false;
		}

// renderov�n� objekt� ve sc�n�
		D3DF_Render();

// renderov�n� objekt� s blending
//		D3DF_RenderBlend();

// renderov�n� 2D obr�zk�
//		if (!D3DRamp)
//		{
//			D3DF_Render2D();
//		}

// ukon�en� renderov�n� sc�ny
		pD3End();

// zobrazen� sc�ny
		if (disp) pD3Disp();

// n�vrat standardn� p�esnosti procesoru
		FPUDouble();

// ��t�n� �asov�ho raz�tka
//		D3DTimeStamp++;

// na�ten� ub�hl�ho p�esn�ho �asu
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

// v�po�et st�edn� frekvence
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
// z�m�na modr� a �erven� barvy v RGB (invertuje alpha kan�l)

DWORD XRGB(DWORD col)
{
	return ((col & 0xff00) | (((BYTE)col) << 16) | ((BYTE)(col >> 16)) | ((col & 0xff000000) ^ 0xff000000));
}

/////////////////////////////////////////////////////////////////////////////
// konverze barvy z form�tu Petra na form�t D3DCOLORVALUE (float)

void BGRFloat(DWORD src, D3DCOLORVALUE* dst)
{
	dst->r = (float)((double)(BYTE)src / 255);
	dst->g = (float)((double)(BYTE)(src >> 8) / 255);
	dst->b = (float)((double)(BYTE)(src >> 16) / 255);
	dst->a = (float)((double)((BYTE)(src >> 24) ^ 0xff) / 255);
}

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------

/////////////////////////////////////////////////////////////////////////////
// aktualizace klientsk�ch sou�adnic (pou��v� se tak� pro prvky)
// POZOR - statusbar nemus� b�t je�t� na spr�vn�ch pozic�ch!

#pragma warning ( disable: 4701)	// hl�en� - neinicializovan� polo�ka "wnd"

void InitClientRect(WINITEM* item)
{

// �schova rozm�r� a polohy okna
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

// sou�adnice pro DirectDraw
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

// na�ten� sou�adnic klientsk� oblasti okna
		::GetClientRect(item->Wnd, &ClientRect);

// sn�en� v��ky o stavov� ��dek
		if (StatusVisible)
		{
			RECT rc;
			::GetWindowRect(StatusBar, &rc);
			ClientRect.bottom -= rc.bottom - rc.top;
		}

// ur�en� v��ky a ���ky klientsk� oblasti
		ClientWidth = ClientRect.right - ClientRect.left;
		ClientHeight = ClientRect.bottom - ClientRect.top;
		item->ClientWidth = ClientWidth;
		item->ClientHeight = ClientHeight;
	}

// klientsk� rozm�ry pro ostatn� okna a pro ostatn� prvky
	else
	{
		::GetClientRect(item->Wnd, &wnd);
		item->ClientWidth = wnd.right - wnd.left;
		item->ClientHeight = wnd.bottom - wnd.top;
	}
}

#pragma warning ( default: 4701)	// hl�en� - neinicializovan� polo�ka "wnd"

/////////////////////////////////////////////////////////////////////////////
// nastaven� textu stavov� li�ty

void SetStatusText(CString& text)
{
	if ((text != AktStatusText) && (StatusBar != NULL))
	{
		AktStatusText = text;
		AktStatusText.SetWindowText(StatusBar);
	}
}

/////////////////////////////////////////////////////////////////////////////
// zobrazen� stavov�ho ��dku

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
// nastaven� titulku pro konzolu

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
// zobrazen� jm�na v titulku okna

void SetCaptionText(CString& text)
{
	if (text != AktCaptionText) 					// m�n� se titulek ?
	{
		AktCaptionText = text;						// �schova nov�ho titulku
		AktCaptionText.SetWindowText(MainFrame);	// nastaven� nov�ho titulku
		Win[0].Text = AktCaptionText;
		Win[0].TextValid = true;
		SetConsoleText();
	}
}


/////////////////////////////////////////////////////////////////////////////
// p�emapov�n� kl�vesy

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
// obsluha zpr�vy p�ed rozesl�n�m do oken (TRUE = zpr�va zpracov�na)

// obsluha v grafick�m m�du (prov�d� se i v dialogov�m m�du, pouze se ignoruje v�sledek)
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
//		LMouseDown = true;		// p�i kliknut� na nadpis se aktivuje, ale u� neuvoln�
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

// obsluha v dialogov�m m�du
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

// obsluha zpr�v
BOOL PreTranslateMessage(MSG* msg)
{
// obsluha dialogov�ho m�du
	if (DialogMode || (DialogParent != 0))
	{

// pro tla��tka n�hrada dvojkliku my�� oby�ejn�m klikem
		if (msg->message == WM_LBUTTONDBLCLK)
		{
			TCHAR buf[21];
			::GetClassName(msg->hwnd, buf, 20);
			if (ButtonClassName == buf)
			{
				msg->message = WM_LBUTTONDOWN;
			}
		}

// obsluha zpr�vy
		PreTranslateMessageGraf(msg);
		return PreTranslateMessageDialog(msg);
	}

// obsluha grafick�ho m�du
	return PreTranslateMessageGraf(msg);
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace p�ede�l�ho okna

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
// obsluha zpr�v okna

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
				::GetCursorPos(&pt);	// na�ten� sou�adnic my�i
				::ScreenToClient(Win[DialogParent].Wnd, &pt); // p�evod na klientsk� sou�adnice
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
				ReSet3DCount = 0;				// resetov�n� opakovan� inicializace
				ReSet3DCountN = 1;				// inicializace p��t�ho ��ta�e

				RecalcD3D();
				FPUFloat();
				pD3SizeView();
				pD3AktPalette(MainFrame);
				FPUDouble();
			}
#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------
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
				ReSet3DCount = 0;				// resetov�n� opakovan� inicializace
				ReSet3DCountN = 1;				// inicializace p��t�ho ��ta�e

				RecalcD3D();
				FPUFloat();
				pD3MoveView();
				pD3AktPalette(MainFrame);
				FPUDouble();
			}
#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------

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
// --------------------- konec vypnut� pro MINI verzi -------------------

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
// --------------------- konec vypnut� pro MINI verzi -------------------

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
			MusicEnd = true;			// p��znak ukon�en� hudby
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

							// p��znak aktivn� polo�ky
								int state = ::SendMessage(wnd, LVM_GETITEMSTATE, n, LVIS_SELECTED);
								BOOL sel = (state & LVIS_SELECTED);
								BOOL focus = (::GetFocus() == wnd);

							// vymaz�n� podkladu cel� polo�ky
								rc.left = LVIR_BOUNDS;
								::SendMessage(wnd, LVM_GETITEMRECT, n, (LPARAM)&rc);
								::FillRect(dc, &rc, (sel ? (focus ? StdBrushHighlight : StdBrushBtn) : item->FontBrush));

							// zobrazen� odd�lovac�ch �ar a text�
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
// pokra�uje BN_CLICKED !!!
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
// --------------------- konec vypnut� pro MINI verzi -------------------

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

				ReSet3DCount = 0;				// resetov�n� opakovan� inicializace
				ReSet3DCountN = 1;				// inicializace p��t�ho ��ta�e
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

	// obnoven� palet - jen podle n�kter�ch program�, nev�m zda je nutn�
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
			::GetCursorPos(&pt);	// na�ten� sou�adnic my�i
			::ScreenToClient(Win[DialogParent].Wnd, &pt); // p�evod na klientsk� sou�adnice
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

		// vyhled�n� aplikace stejn�ho jm�na
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

		// nov� aplikace
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

		// vyhled�n� serveru s t�mto handle
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

		// nov� server
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

		// p�id�n� t�matu
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
