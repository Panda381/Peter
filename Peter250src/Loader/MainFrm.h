
/***************************************************************************\
*																			*
*							Hlavní okno aplikace							*
*																			*
\***************************************************************************/

//#define OPENGL11								// povoleno používat rozhraní OpenGl 1.1


// obsluha DirectDraw (inicializace pouze jednou, pøi prvním použití)
//extern bool					IsInitDirectDraw;	// probìhla inicializace DirectDraw
//extern LPDIRECTDRAW			DirectDraw;			// objekt DirectDraw (NULL=není)
//extern LPDIRECTDRAWSURFACE	DirectSurface;		// povrch pro DirectDraw (NULL=není)
//extern LPDIRECTDRAWPALETTE	DirectPalette;		// palety pro DirectDraw (NULL=nejsou)
extern	bool				Resizing;			// probíhá programová zmìna okna

extern	bool				AppActive;			// aplikace je aktivní
extern	bool				AppMinim;			// aplikace minimalizovaná

#define MAX_STAGES	8				// max. poèet stupòù operací textur
#define MAX_RENDGROUP	16			// poèet renderovacích skupin
#define	RENDNORM	4				// skupina normálních objektù k renderování
#define	RENDTRANS	10				// skupina prùhledných objektù k renderování
#define	RENDPIC		14				// skupina obrázkù k renderování

//#define MAX_ZBIAS	16				// maximální poèet BIAS odsazení (je to o 1 ménì, než je povoleno)
//#define MID_ZBIAS	8				// støední (implicitní) odsazení (bude -8 až +7)

// funkce DirectDrawCreate
//typedef	HRESULT (WINAPI *DIRECTDRAWCREATE) (GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter);
//extern DIRECTDRAWCREATE	pDirectDrawCreate; // funkce DirectDrawCreate (NULL=není)

#if (DIRECT3D_VERSION < 0x0800)
typedef enum _D3DFORMAT
{
    D3DFMT_UNKNOWN              =  0,

    D3DFMT_R8G8B8               = 20,
    D3DFMT_A8R8G8B8             = 21,
    D3DFMT_X8R8G8B8             = 22,
    D3DFMT_R5G6B5               = 23,
    D3DFMT_X1R5G5B5             = 24,
    D3DFMT_A1R5G5B5             = 25,
    D3DFMT_A4R4G4B4             = 26,
    D3DFMT_R3G3B2               = 27,
    D3DFMT_A8                   = 28,
    D3DFMT_A8R3G3B2             = 29,
    D3DFMT_X4R4G4B4             = 30,

    D3DFMT_A8P8                 = 40,
    D3DFMT_P8                   = 41,

    D3DFMT_L8                   = 50,
    D3DFMT_A8L8                 = 51,
    D3DFMT_A4L4                 = 52,

    D3DFMT_V8U8                 = 60,
    D3DFMT_L6V5U5               = 61,
    D3DFMT_X8L8V8U8             = 62,
    D3DFMT_Q8W8V8U8             = 63,
    D3DFMT_V16U16               = 64,
    D3DFMT_W11V11U10            = 65,

    D3DFMT_UYVY                 = MAKEFOURCC('U', 'Y', 'V', 'Y'),
    D3DFMT_YUY2                 = MAKEFOURCC('Y', 'U', 'Y', '2'),
    D3DFMT_DXT1                 = MAKEFOURCC('D', 'X', 'T', '1'),
    D3DFMT_DXT2                 = MAKEFOURCC('D', 'X', 'T', '2'),
    D3DFMT_DXT3                 = MAKEFOURCC('D', 'X', 'T', '3'),
    D3DFMT_DXT4                 = MAKEFOURCC('D', 'X', 'T', '4'),
    D3DFMT_DXT5                 = MAKEFOURCC('D', 'X', 'T', '5'),

    D3DFMT_D16_LOCKABLE         = 70,
    D3DFMT_D32                  = 71,
    D3DFMT_D15S1                = 73,
    D3DFMT_D24S8                = 75,
    D3DFMT_D16                  = 80,
    D3DFMT_D24X8                = 77,
    D3DFMT_D24X4S4              = 79,


    D3DFMT_VERTEXDATA           =100,
    D3DFMT_INDEX16              =101,
    D3DFMT_INDEX32              =102,

    D3DFMT_FORCE_DWORD          =0x7fffffff
} D3DFORMAT;
#endif

#if (DIRECT3D_VERSION < 0x0800)
typedef struct _D3DMATERIAL8 {
    D3DCOLORVALUE   Diffuse;        /* Diffuse color RGBA */
    D3DCOLORVALUE   Ambient;        /* Ambient color RGB */
    D3DCOLORVALUE   Specular;       /* Specular 'shininess' */
    D3DCOLORVALUE   Emissive;       /* Emissive color RGB */
    float           Power;          /* Sharpness if specular highlight */
} D3DMATERIAL8;

typedef struct _D3DLIGHT8 {
    D3DLIGHTTYPE    Type;            /* Type of light source */
    D3DCOLORVALUE   Diffuse;         /* Diffuse color of light */
    D3DCOLORVALUE   Specular;        /* Specular color of light */
    D3DCOLORVALUE   Ambient;         /* Ambient color of light */
    D3DVECTOR       Position;         /* Position in world space */
    D3DVECTOR       Direction;        /* Direction in world space */
    float           Range;            /* Cutoff range */
    float           Falloff;          /* Falloff */
    float           Attenuation0;     /* Constant attenuation */
    float           Attenuation1;     /* Linear attenuation */
    float           Attenuation2;     /* Quadratic attenuation */
    float           Theta;            /* Inner angle of spotlight cone */
    float           Phi;              /* Outer angle of spotlight cone */
} D3DLIGHT8;



#endif

// definice roviny
typedef struct PLANE_
{
	float		A;								// složka X normály
	float		B;								// složka Y normály
	float		C;								// složka Z normály
	float		D;								// posun od 0 proti smìru normály
} PLANE;

// videomód DirectDraw
typedef struct VIDEOMODEITEM_
{
	int			Width;							// šíøka
	int			Height;							// výška
	int			Bits;							// poèet bitù na bod (8, 16, 24 nebo 32)
	int			res;							// rezervováno
//	D3DFORMAT	Format;							// format videomódu Direct3D
} VIDEOMODEITEM;

#define MAXVIDEOMODES 64

extern	bool				FullScreen;			// je celoobrazovkový režim s DirectDraw
extern	BOOL				OldMaximized;		// uschovaný pøíznak maximalizace
extern	int					NumVideoModes;		// poèet videomódù v tabulce
extern	VIDEOMODEITEM*		TabVideoModes;		// tabulka videomódù

// požadovaný uživatelský videomód
extern	int					UserScreenWidth;	// uživatelská šíøka videomódu (0=implicitní)
extern	int					UserScreenHeight;	// uživatelská výška videomódu (0=implicitní)
//extern	bool				UserFullScreen;		// uživatelem požadován celoobrazovkový mód 3D
extern	int					UserScreenBits;		// uživatelský poèet bitù (0=implicitní)


// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI
	  
// zvolené rozhraní a ovladaè
extern	int					D3DIntUser;			// uživatelem zadané rozhraní 0 až D3INTERFACES-1 (0=automatický)
extern	int					D3DIntAkt;			// typ aktivního rozhraní (0=není,1=GL0,2=GL1,3=GL2,4=DX3,5=DX5,6=DX6,7=DX7,8=DX8)
extern	int					D3DDevUser;			// uživatelem zadaný typ ovladaèe 0 až D3DEVICES-1 (0=automatický)
extern	int					D3DDevAkt;			// typ aktivního ovladaèe (0=není,1=HAL,2=TnLHal,3=REF,4=RGB,5=MMX,6=Ramp)



// ovladaèe a rozhraní 3D
enum D3DEVICE {
	NDefDev = 0,			// implicitní ovladaè
	NHALDev = 1,			// HAL
	NTnLHalDev = 2,			// TnLHal
	NRefDev = 3,			// REF
	NRGBDev = 4,			// RGB
	NMMXDev = 5,			// MMX
	NRampDev = 6,			// Ramp
	D3DEVICES = 7,			// poèet typù ovladaèù
};

enum D3INTERFACE {
	NDefInt = 0,			// implicitní rozhraní
	NGL0Int = 1,			// OpenGL 1.0
	NGL1Int = 2,			// OpenGL 1.1
	NGL2Int = 3,			// OpenGL 2.0
	NDX3Int = 4,			// DirectX 3
	NDX5Int = 5,			// DirectX 5
	NDX6Int = 6,			// DirectX 6
	NDX7Int = 7,			// DirectX 7
	NDX8Int = 8,			// DirectX 8
	D3INTERFACES = 9,		// poèet typù rozhraní
};

// tabulka ID kódù ovladaèù Direct3D 
extern	const GUID* Dev3DTab[D3DEVICES];

// videomód Direct3D
extern	bool				D3DFullScreen;		// je celoobrazovkový režim s Direct3D
extern	int					D3DNumVideoModes;	// poèet videomódù v tabulce
extern	VIDEOMODEITEM*		D3DTabVideoModes;	// tabulka videomódù
//extern	CString				D3DListVideoModes;	// textový seznam videomódù

// vlastnosti zaøízení
//extern	D3DCAPS8*			D3DCaps8;			// buffer vlastností zaøízení Direct3D
extern	int					MaxTextureWidth;	// maximální šíøka textury
extern	int					MaxTextureHeight;	// maximální výška textury
extern	int					MaxTextureRepeat;	// maximální poèet opakování textury
extern	bool				SquareTexture;		// textury musí být ètvercové
extern	bool				TexturePow2;		// rozmìr textur musí být mocnina 2
extern	bool				IsHWRaster;			// je HW rasterizace
extern	bool				IsHWTransLight;		// je HW transformace a osvìtlení
extern	bool				IsPureDevice;		// pouze HW obsluha
extern	bool				IsBlending;			// jsou blending operace
extern	bool				IsMagFilter;		// podporuje filtr pøi zvìtšení
extern	bool				IsMinFilter;		// podporuje filtr pøi zmenšení
extern	bool				IsMipMap;			// podporuje mipmap textury
extern	bool				AlphaGreaterEqual;	// lze porovnávat Alpha	>=
extern	bool				AlphaGreater;		// lze porovnávat Alpha >
extern	int					MaxVertexNum;		// maximální poèet vrcholù skuteèný
extern	int					MaxVertexNum2;		// maximální poèet vrcholù pro renderování
extern	int					MaxTextureStages;	// maximální poèet stupòù textur
extern	int					MaxTextureSimult;	// maximální poèet textur pro jeden prùchod
extern	bool				IsTableFog;			// podporována tabulková mlha
extern	bool				IsVertexFog;		// podporována vrcholová mlha
extern	bool				IsRangeFog;			// podporována vzdálenostní mlha

// pøepínaèe nastavení
extern	bool				D3DWireframeAkt;	// vyplòování ploch aktuálnì zapnut
extern	bool				D3DWireframeGlobal;	// vyplòování ploch globálnì povoleno
extern	bool				D3DLightOnAkt;		// osvìtlení aktuálnì zapnuto
extern	bool				D3DLightOnGlobal;	// osvìtlení globálnì povoleno
extern	bool				D3DShadesAkt;		// plynulé stínování aktuálnì zapnuto
extern	bool				D3DShadesGlobal;	// plynulé stínování globálnì povoleno
extern	bool				D3DMinFilterAkt[MAX_STAGES];	// filtrace zmenšených textur zapnuta
extern	bool				D3DMinFilterGlobal;	// filtrace zmenšených textur globálnì povolena
extern	bool				D3DMagFilterAkt[MAX_STAGES];	// filtrace zvìtšených textur zapnuta
extern	bool				D3DMagFilterGlobal;	// filtrace zvìtšených textur globálnì povolena
extern	bool				D3DMipFilterAkt[MAX_STAGES];	// filtrace vzdálených textur zapnuta
extern	bool				D3DMipFilterGlobal;	// filtrace vzdálených textur globálnì povolena


extern	int					D3DAktStage;		// aktivní stupeò textur

extern	bool				D3DRamp;			// je ovladaè Ramp

extern	bool				D3DVSync;			// synchronizovat s VSYN

extern	bool				D3DVertFog;			// pouze vektorová mlha (SW driver)


extern	int ReSet3DCount;
extern	int ReSet3DCountN;


extern	bool				D3DCullingUnknown;	// stav pøepínaèe odstraòování ploch neznámý
extern	short				D3DCullingAkt;		// volba zobrazených ploch
extern	bool				D3DCameraInvert;	// je inverzní kamera, invertovat plošky
//extern	bool				D3DCullingGlobal;	// odstraòování ploch globálnì povoleno

// funkce Direct3DCreate8
//typedef	IDirect3D8* (WINAPI *DIRECT3DCREATE8) (UINT SDKVersion);
//extern DIRECT3DCREATE8	pDirect3DCreate8;		// funkce Direct3DCreate8 (NULL=není)

// ovladaèe Direct3D8
extern	bool				IsDirect3D;			// probìhla inicializace Direct3D
//extern	IDirect3D8*			Direct3D;			// objekt Direct3D verze 8 (NULL=není)
//extern  IDirect3DDevice8*	Direct3DDev;		// zaøízení Direct3D (NULL=není)
//extern	int					D3DDevUser;			// uživatelem zadaný typ zaøízení (0=automatický)
//extern	int					D3DDevAkt;			// typ aktivního zaøízení (0=není,1=HAL,2=TnLHal,3=REF,4=RGB,5=MMX,6=Ramp)
//extern	int					D3DIntUser;			// uživatelem zadaný interface (0=automatický)
//extern	int					D3DIntAkt;			// typ aktivního rozhraní (0=není,1=GL0,2=GL1,3=GL2,4=DX4,5=DX5,6=DX6,7=DX7,8=DX8)

// okno Direct3D
extern	bool				D3D;				// je okno Direct3D
//extern	bool				D3DDX;				// je okno 3D - ovladaè DirectX
//extern	bool				D3DGL;				// je okno 3D - ovladaè OpenGL
extern	bool				D3D2D;				// je kombinace 3D s 2D grafikou
extern	bool				D3DFull;			// okno 3D využívá celou plochu hlavního okna

extern	bool				D3DReturn;			// blokování Alt+Enter pøi D3D módu

// formáty textur (dwRGBBitCount=0 pro neplatný formát)
extern	DDPIXELFORMAT		D3DTextFormatA8R8G8B8;			// formát textury D3DFMT_A8R8G8B8
extern	DDPIXELFORMAT		D3DTextFormatA4R4G4B4;			// formát textury D3DFMT_A4R4G4B4
extern	DDPIXELFORMAT		D3DTextFormatA1R5G5B5;			// formát textury D3DFMT_A1R5G5B5
extern	DDPIXELFORMAT		D3DTextFormatR8G8B8;			// formát textury D3DFMT_R8G8B8
extern	DDPIXELFORMAT		D3DTextFormatR5G6B5;			// formát textury D3DFMT_R5G6B5
extern	DDPIXELFORMAT		D3DTextFormatX1R5G5B5;			// formát textury D3DFMT_X1R5G5B5
extern	DDPIXELFORMAT		D3DTextFormatX8R8G8B8;			// formát textury D3DFMT_X8R8G8B8
extern	DDPIXELFORMAT		D3DTextFormatX4R4G4B4;			// formát textury D3DFMT_X4R4G4B4
extern	bool				D3DLowText;			// používat nižší kvalitu textur
extern	bool				D3DTextFormatOK;	// nalezen platný formát textur

// formáty Z-bufferù (dwZBufferBitDepth=0 pro neplatný formát) - jen pro hledání formátu Z bufferu
extern	DDPIXELFORMAT		D3DZBufferFormat24S8;			// formát bufferu 24 bitù, stencil 8 bitù
extern	DDPIXELFORMAT		D3DZBufferFormat24S4;			// formát bufferu 24 bitù, stencil 4 bity
extern	DDPIXELFORMAT		D3DZBufferFormat15S1;			// formát bufferu 15 bitù, stencil 1 bit
extern	DDPIXELFORMAT		D3DZBufferFormat32S0;			// formát bufferu 32 bitù, stencil 0 bitù
extern	DDPIXELFORMAT		D3DZBufferFormat24S0;			// formát bufferu 24 bitù, stencil 0 bitù
extern	DDPIXELFORMAT		D3DZBufferFormat16S0;			// formát bufferu 16 bitù, stencil 0 bitù
extern	DDPIXELFORMAT		D3DZBufferFormat8S0;			// formát bufferu 8 bitù, stencil 0 bitù
extern	bool				D3DZBufferFormatOK;				// nalezen platný formát Z-bufferu

// stencil buffer
extern	bool				D3DSBufferFormatOK;				// nalezen Stencil buffer
extern	int					D3DSBufferDepth;				// hloubka Stencil bufferu (bitù)
extern	int					D3DSBufferMax;					// maximální hodnota Stencil bufferu
extern	bool				D3DSBufferClear;				// požadavek k vymazání Stencil bufferu
extern	bool				D3DShadows;						// povoleno používání stínù

// projekèní matice (pozor, èlen _34 matice nesmí být < 0 !!!)
extern	bool				D3DProjAkt;			// požadavek aktualizace projekèní matice
//extern	double				D3DHalfHeight;		// výška horizontu
//extern	double				D3DAspectRatio;		// pomìr výška/šíøka
extern	double				D3DFrontClip;		// vzdálenost pøední omezující roviny
extern	double				D3DBackClip;		// vzdálenost zadní omezující roviny
//extern	double				D3DViewAngle;		// úhel šíøky pohledu
extern	D3DMATRIX			D3DProjMatrix;		// projekèní matice (nepoužité èleny jsou = 0)
extern	int					D3DProjection;		// typ projekce (0=persp, 1=orto, 2=r-persp, 3=r-orto)
extern	bool				D3DRightHand;		// pravoruká projekce (typ projekce 2 a 3)

// pohledová matice (odvozená z kamery)
extern	bool				D3DViewAkt;			// požadavek aktualizace pohledové matice
extern	D3DMATRIX			D3DViewMatrix;		// pohledová matice (vytvoøená z objektu kamery)

// kombinovaná matice projekèní*pohledová (pro pøepoèet 2D objektù)
//extern	bool				D3DViewProjAkt;		// aktualizovat výpoèet kombinované matice
//extern	D3DMATRIX			D3DViewProjMatrix;	// kombinovaná matice

// rozhraní OpenGL
//extern	bool				IsOpenGL;			// probìhla inicializace OpenGL

//extern	bool				OKOpenGL;			// inicializace OpenGL je úspìšná

//#ifdef OPENGL11
//extern	bool				OKOpenGL11;			// inicializace OpenGL 1.1 je úspìšná
//#endif // OPENGL11

//extern	HINSTANCE			OpenGLLib;			// knihovna OPENGL32.DLL
//extern	HGLRC				GLContext;			// renderovací plocha OpenGL
//extern	HPALETTE			GLPalette;			// palety pro OpenGL

// pohledová matice (odvozená z kamery)
extern	bool				D3DViewAkt;			// požadavek aktualizace pohledové matice
extern	D3DMATRIX			D3DViewMatrix;		// pohledová matice (vytvoøená z objektu kamery)

// frustum (odvozeno z pohledové a projekèní matice)
extern	bool				D3DFrustumAkt;		// aktualizovat frustum
extern	PLANE				D3DFrustum[6];		// 6 rovin projekèního jehlanu

// rozmìry okna Direct3D zadané uživatelem, bez omezení rozsahu
extern	int					D3DX0;				// poèátek X okna Direct3D (zleva) zadaný uživatelem
extern	int					D3DY0;				// poèátek Y okna Direct3D (zdola) zadaný uživatelem
extern	int					D3DW0;				// šíøka okna Direct3D zadaná uživatelem
extern	int					D3DH0;				// výška okna Direct3D zadaná uživatelem

// rozmìry okna Direct3D zadané uživatelem, s omezením na plochu (pro zobrazení grafiky)
extern	int					D3DX;				// poèátek X okna Direct3D (zleva)
extern	int					D3DY;				// poèátek Y okna Direct3D (zdola)
extern	int					D3DW;				// šíøka okna Direct3D
extern	int					D3DH;				// výška okna Direct3D

// skuteèné rozmìry okna Direct3D na obrazovce (omezení na klientské souøadnice okna)
extern	int					D3DLeft;			// skuteèný poèátek X okna Direct3D
extern	int					D3DTop;				// skuteèný poèátek Y okna Direct3D (shora)
extern	int					D3DWidth;			// skuteèná šíøka okna Direct3D
extern	int					D3DHeight;			// skuteèná výška okna Direct3D
extern	double				D3DWidthHeight2;	// odmocnina s pomìru D3DWidth/D3DHeight

// uživatelské nastavení
//extern	bool				D3DDither;			// použít dithering Direct3D
//extern	bool				D3DRendReq;			// požadavek pøerenderování Direct3D
//extern	int					D3DQuality;			// kvalita renderování

// objekty Direct3D
//extern	LPDIRECTDRAWSURFACE D3Surface;			// povrch pro Direct 3D (NULL=není)
//extern	LPDIRECTDRAWSURFACE D3ZBuffer;			// povrch pro Direct 3D Z-buffer (NULL=není)
//extern	LPDIRECT3DVIEWPORT	D3DViewport;		// viewport pro Direct 3D
//extern	LPDIRECT3DMATERIAL	D3DBackMaterial;	// materiál pozadí scény
//extern	D3DMATERIALHANDLE	hD3DBackMaterial;	// handle materiálu pozadí scény

// objekty Direct3DRM
//extern	LPDIRECT3DRM		Direct3DRM;			// objekt Direct3D RM
//extern	LPDIRECT3DRM2		Direct3DRM2;		// objekt Direct3D RM 2
//extern	LPDIRECT3DRMDEVICE	Direct3DRMDev;		// zaøízení Direct3D RM
//extern	LPDIRECT3DRMDEVICE2	Direct3DRMDev2;		// zaøízení Direct3D RM 2
//extern	LPDIRECT3DRMFRAME2	D3Scene;			// rám scény
//extern	LPDIRECT3DRMFRAME2	D3Camera;			// rám kamery
//extern	LPDIRECT3DRMVIEWPORT D3View;			// okno pohledu


// pozadí scény
extern	DWORD				D3DBackCol;			// barva pozadí scény (MAXDWORD = nemazat)
extern	DWORD				D3DBackColRGB;		// barva pozadí scény ve formátu RGB
extern	D3DCOLORVALUE		D3DBackColF;		// barva pozadí scény ve formátu float
extern	bool				D3DBackColAkt;		// barva pozadí zmìnìna
extern	int					D3DBackText;		// textura pozadí scény (-1 = není)
extern	bool				D3DBackTextAkt;		// textura pozadí zmìnìna
extern	int					D3DBackMat;			// materiál pozadí scény
extern	D3DMATERIAL8		D3DBackMatMat;		// buffer materiálu pozadí scény
//extern	IDirect3DVertexBuffer8*	D3DBackBuf;		// buffer textury Direct3D

// nastavení mlhy
extern	BOOL				D3DFogOn;			// mlha je aktuálnì zapnuta (-1 = neznámé)
extern	bool				FogAkt;				// mlhu je nutno aktualizovat
extern	bool				FogKorig;			// korigovaná mlha (pøièítá se +4 k typu mlhy)
extern	bool				FogOn;				// mlha je zapnuta
extern	DWORD				FogColor;			// barva mlhy ve formátu Petra BGR (MAXDWORD=vypnuto)
extern	DWORD				FogColorRGB;		// barva mlhy ve formáty RGB
extern	D3DCOLORVALUE		FogColorF;			// barva mlhy ve formátu float
extern	int					FogType;			// typ mlhy (0=lineární, 1=exponenciální, 2=kvadratická)
extern	double				FogStart;			// zaèátek lineární mlhy
extern	double				FogEnd;				// konec lineární mlhy
extern	double				FogDens;			// hustota exponenciální a kvadratické mlhy (0 až 1)

// ambient osvìtlení
extern	DWORD				D3DAmbientCol;		// ambient osvìtlení ve formátu Petra BGR
extern	DWORD				D3DAmbientColRGB;	// ambient osvìtlení ve formátu RGB
extern	D3DCOLORVALUE		D3DAmbientColF;		// ambient osvìtlení ve formátu float
extern	bool				D3DAmbientColAkt;	// ambient osvìtlení zmìnìno



//extern	CPicture			D3ScenePic;			// textura pozadí scény
//extern	bool				D3SceneBack;		// je pozadí scény

extern	int					D3DLevel;			// složitost objektù (2 a více)
extern	bool				D3DCreateUpper;		// vytváøet horní podstavu objektù
extern	bool				D3DCreateLower;		// vytváøet dolní podstavu objektù
extern	int					D3D_ID;				// aktivní objekt (rám) Direct3D
extern	int					TerenID;			// naposledy zvolený objekt terénu (-1=není)

//extern	int					D3DFilter0;			// filtrace textur zadaná uživatelem (-1=implicitní)
//extern	int					D3DFilter;			// aktuální filtrace textur
//extern	bool				D3DMipmaps;			// používají se mipmap
extern	double				D3DSmooth;			// vyhlazení ditheringu textur (1=ne)

// zrychlené renderování
//extern	bool				OpenGLQMat;			// používat zkrácené výpoèty matic
													// - pohledová matice se násobí
													//   s projekèní maticí, pro objekty
													//   se nastaví pouze modelová matice
													// - v jiném pøípadì se pohledová
													//   matice násobí s každou modelovou
													//   maticí
													//   (musí být vypnuto v pøípadì mlhy)

// nastavení kamery a mlhy
//extern	D3DVALUE			FogStart;			// zaèátek mlhy
//extern	D3DVALUE			FogEnd;				// konec mlhy
//extern	D3DVALUE			FogDens;			// hustota mlhy
//extern	D3DRMPROJECTIONTYPE D3DProj;			// typ projekce


// mìøení uplynulého èasu
extern	double D3DLastTime;							// poslední èas renderování
extern	double D3DElapsedTime;						// uplynulý èas od posledního renderování
extern	double D3DAverageTime;						// støadaè støedního èasu
extern	double D3DAverageFreq;						// støední frekvence
extern	int D3DAverageFreqI;						// frekvence Integer


#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------

/////////////////////////////////////////////////////////////////////////////
// globální promìnné

extern	HWND		MainFrame;				// hlavní okno aplikace
extern	TCHAR		MainFrameClass[];		// název tøídy hlavního okna

extern	int			MainFrameStyleEx;		// rozšíøený styl
extern	int			MainFrameStyle;			// styl okna

//extern	TCHAR		MainFrameSubClass[];	// název tøídy podokna
extern	bool		MainFrameVisible;		// hlavní okno zobrazeno
extern	HWND		StatusBar;				// stavová lišta

extern	int			MainFrameX;				// souøadnice X støedu hlavního okna
extern	int			MainFrameY;				// souøadnice Y støedu hlavního okna

extern	bool		ReqClose;				// požadavek uzavøení okna od uživatele

extern	bool		StatusVisible;			// pøíznak viditelnosti stavového øádku

extern	RECT		ClientRect;				// klientská oblast hlavního okna
extern	int			ClientWidth;			// šíøka klientské oblasti
extern	int			ClientHeight;			// výška klientské oblasti

//extern	HCURSOR		CurAkt;					// aktivní kurzor
extern	HCURSOR		CurArrow;				// (standardní) šipka
extern	HCURSOR		CurArrow2;				// (standardní) šipka pro DirectX
//extern	HCURSOR		CurWait;				// èekání

extern	CString		AktCaptionText;			// text titulku hlavního okna
extern	CString		AktStatusText;			// zobrazený text stavové lišty

// stav myši (aktualizováno v PreTranslateMessage)
extern	bool		LMouseDown;				// levé tlaèítko stisknuto
extern	bool		LMouseClick;			// klik levého tlaèítka
extern	bool		LMouseDClick;			// dvojklik levého tlaèítka

extern	bool		RMouseDown;				// pravé tlaèítko stisknuto
extern	bool		RMouseClick;			// klik pravého tlaèítka
extern	bool		RMouseDClick;			// dvojklik pravého tlaèítka

extern	POINT		MouseScreen;			// souøadnice myši na displeji
extern	POINT		MouseMain;				// souøadnice myši v hlavním oknì
extern	bool		MouseValid;				// pøíznak aktualizace souøadnice myši

extern	bool		ProgramFill;			// plnìní prvku okna programem

// buffer hledaného textu v oknì RichEdit
extern	LPTSTR		FindBuff;				// buffer hledaného textu (v oknì RichEdit)
extern	bool		FindMatchCase;			// pøi hledání rozlišena velká a malá písmena
extern	bool		FindWholeWord;			// hledat jen celá slova
extern	HWND		FindDialogBox;			// handle na dialog hledání (NULL=není)
extern	UINT		FindMsgString;			// identifikátor zprávy hledání

/////////////////////////////////////////////////////////////////////////////
// pøíprava požadovaných rozmìrù okna

void AdjustMainFrame(RECT* wrc);


/////////////////////////////////////////////////////////////////////////////
// první zobrazení hlavního okna

void MainFrameShow();


/////////////////////////////////////////////////////////////////////////////
// vytvoøení hlavního okna aplikace

bool MainFrameCreate();

/////////////////////////////////////////////////////////////////////////////
// pøepoèet rozmìrù okna Direct3D (podle údajù zadaných uživatelem)
// vrací TRUE=má být okno 3D

bool RecalcD3D();

/////////////////////////////////////////////////////////////////////////////
// inicializace obsluhy DirectDraw (vrací TRUE=je DirectDraw)

bool InitDirectDraw();

/////////////////////////////////////////////////////////////////////////////
// inicializace seznamu videomódù (volá se pøi prvním použití)

void InitVideoModes();

/////////////////////////////////////////////////////////////////////////////
// setøídìní videomódù displeje (na zaèátku je nejmenší rozlišení a nejnižší poèet bitù)

void SortDisplayModes(VIDEOMODEITEM* tab, int num);

/////////////////////////////////////////////////////////////////////////////
// start videomódu (vrací TRUE=OK)

bool StartVideoMode();

/////////////////////////////////////////////////////////////////////////////
// stop aktivního videomódu

void StopVideoMode();

/////////////////////////////////////////////////////////////////////////////
// konverze barvy z formátu Petra na formát D3DCOLORVALUE (float)

void BGRFloat(DWORD src, D3DCOLORVALUE* dst);

/////////////////////////////////////////////////////////////////////////////
// inicializace objektù scény (volá se ještì pøed inicializací 3D!)

void Init3D();

/////////////////////////////////////////////////////////////////////////////
// nalezení nejbližšího videomódu v tabulce (vrací -1=nenalezeno)

int FindVideoMode(VIDEOMODEITEM* tab, int tabn, int width, int height, int bits);


/////////////////////////////////////////////////////////////////////////////
// aktivace videomódu DirectX (vrací TRUE=OK)

//bool InitDXVideoMode(int width, int height, int bits);


/////////////////////////////////////////////////////////////////////////////
// ukonèení obsluhy DirectDraw

//void TermDirectDraw();

/////////////////////////////////////////////////////////////////////////////
// inicializace obsluhy Direct3D (vrací TRUE=je Direct3D)

//bool InitDirect3D();

/////////////////////////////////////////////////////////////////////////////
// pøidání formátu textur

void D3DEnumPixelFormat(DDPIXELFORMAT* pf);

/////////////////////////////////////////////////////////////////////////////
// pøidání formátu Z-bufferu

void D3DEnumZBufferFormat(DDPIXELFORMAT* pf);

/////////////////////////////////////////////////////////////////////////////
// výbìr uživatelského ovladaèe Direct3D

bool UserD3DDevice();

// pøenastavení filtrace textur
//void ReSetFilter();

// pøenastavení kvality renderování
//void ReSetD3DQuality();

// pøenastavení pøepínaèe Dithering
//void ReSetD3DDither();
	
// zámìna modré a èervené barvy v RGB
DWORD XRGB(DWORD col);
	
// zjištìní parametrù mlhy
//void GetFogPar();

// nastavení parametrù mlhy
//void SetFogPar();
	
// zobrazení animace
//void DispAnimate();
		  
// vyrenderování scény
void D3Render(bool disp);

// animace
//void Animate();

/////////////////////////////////////////////////////////////////////////////
// nastavení stavu Direct3D (nekontroluje rozsahy a platnosti!)

void _fastcall SetD3DRenderState(D3DRENDERSTATETYPE state, DWORD value);

/////////////////////////////////////////////////////////////////////////////
// nastavení módu vyplòování ploch

void SetD3DWireframe(bool on);

/////////////////////////////////////////////////////////////////////////////
// nastavení módu zapnutí osvìtlení

void SetD3DLightOn(bool on);

/////////////////////////////////////////////////////////////////////////////
// nastavení módu plynulého stínování

void SetD3DShades(bool on);

/////////////////////////////////////////////////////////////////////////////
// nastavení módu filtrace textur pøi zvìtšení

void SetD3DMagFilter(bool on, int stage);

/////////////////////////////////////////////////////////////////////////////
// nastavení módu filtrace zmenšených a vzdálených textur

void SetD3DMinMipFilter(bool min, bool mip, int stage);

/////////////////////////////////////////////////////////////////////////////
// nastavení odstraòování ploch

void SetD3DCullingKorig(short val);
void SetD3DCulling(short val);

/////////////////////////////////////////////////////////////////////////////
// zapnutí mlhy

void D3DCheckFogOn();

void SetD3DFogOn(BOOL on);

/////////////////////////////////////////////////////////////////////////////
// resetování pøepínaèù nastavení 

void ResetD3DSwitch();

/////////////////////////////////////////////////////////////////////////////
// uvolnìní ovladaèe Direct3D

void DeSelectD3DDev();

/////////////////////////////////////////////////////////////////////////////
// aktualizace viewportu (napø. po zmìnì velikosti okna)

bool UpdateViewport();


/////////////////////////////////////////////////////////////////////////////
// zapnutí/vypnutí celoobrazovkového módu

void SetFullScreen(bool full);


/////////////////////////////////////////////////////////////////////////////
// znovunastavení videomódu v celoobrazovkovém módu

void ReSetDisplayMode();


/////////////////////////////////////////////////////////////////////////////
// aktualizace klientských souøadnic

void InitClientRect(WINITEM* item);


/////////////////////////////////////////////////////////////////////////////
// zobrazení kurzoru èekání (zahájení a ukonèení musí být do páru!)

//void BeginWaitCursor();


/////////////////////////////////////////////////////////////////////////////
// vypnutí kurzoru èekání (zahájení a ukonèení musí být do páru!)

//void EndWaitCursor();


/////////////////////////////////////////////////////////////////////////////
// nastavení textu stavové lišty

void SetStatusText(CString& text);


/////////////////////////////////////////////////////////////////////////////
// zobrazení stavového øádku

void SetStatusVisible(bool show);


/////////////////////////////////////////////////////////////////////////////
// nastavení titulku pro konzolu

void SetConsoleText();


/////////////////////////////////////////////////////////////////////////////
// zobrazení jména v titulku okna

void SetCaptionText(CString& text);

/////////////////////////////////////////////////////////////////////////////
// obsluha zprávy pøed rozesláním do oken

BOOL PreTranslateMessage(MSG* msg);


/////////////////////////////////////////////////////////////////////////////
// aktualizace pøedešlého okna

void AktPrevWindow();
