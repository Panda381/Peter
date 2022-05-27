
/***************************************************************************\
*																			*
*							Hlavn� okno aplikace							*
*																			*
\***************************************************************************/

//#define OPENGL11								// povoleno pou��vat rozhran� OpenGl 1.1


// obsluha DirectDraw (inicializace pouze jednou, p�i prvn�m pou�it�)
//extern bool					IsInitDirectDraw;	// prob�hla inicializace DirectDraw
//extern LPDIRECTDRAW			DirectDraw;			// objekt DirectDraw (NULL=nen�)
//extern LPDIRECTDRAWSURFACE	DirectSurface;		// povrch pro DirectDraw (NULL=nen�)
//extern LPDIRECTDRAWPALETTE	DirectPalette;		// palety pro DirectDraw (NULL=nejsou)
extern	bool				Resizing;			// prob�h� programov� zm�na okna

extern	bool				AppActive;			// aplikace je aktivn�
extern	bool				AppMinim;			// aplikace minimalizovan�

#define MAX_STAGES	8				// max. po�et stup�� operac� textur
#define MAX_RENDGROUP	16			// po�et renderovac�ch skupin
#define	RENDNORM	4				// skupina norm�ln�ch objekt� k renderov�n�
#define	RENDTRANS	10				// skupina pr�hledn�ch objekt� k renderov�n�
#define	RENDPIC		14				// skupina obr�zk� k renderov�n�

//#define MAX_ZBIAS	16				// maxim�ln� po�et BIAS odsazen� (je to o 1 m�n�, ne� je povoleno)
//#define MID_ZBIAS	8				// st�edn� (implicitn�) odsazen� (bude -8 a� +7)

// funkce DirectDrawCreate
//typedef	HRESULT (WINAPI *DIRECTDRAWCREATE) (GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter);
//extern DIRECTDRAWCREATE	pDirectDrawCreate; // funkce DirectDrawCreate (NULL=nen�)

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
	float		A;								// slo�ka X norm�ly
	float		B;								// slo�ka Y norm�ly
	float		C;								// slo�ka Z norm�ly
	float		D;								// posun od 0 proti sm�ru norm�ly
} PLANE;

// videom�d DirectDraw
typedef struct VIDEOMODEITEM_
{
	int			Width;							// ���ka
	int			Height;							// v��ka
	int			Bits;							// po�et bit� na bod (8, 16, 24 nebo 32)
	int			res;							// rezervov�no
//	D3DFORMAT	Format;							// format videom�du Direct3D
} VIDEOMODEITEM;

#define MAXVIDEOMODES 64

extern	bool				FullScreen;			// je celoobrazovkov� re�im s DirectDraw
extern	BOOL				OldMaximized;		// uschovan� p��znak maximalizace
extern	int					NumVideoModes;		// po�et videom�d� v tabulce
extern	VIDEOMODEITEM*		TabVideoModes;		// tabulka videom�d�

// po�adovan� u�ivatelsk� videom�d
extern	int					UserScreenWidth;	// u�ivatelsk� ���ka videom�du (0=implicitn�)
extern	int					UserScreenHeight;	// u�ivatelsk� v��ka videom�du (0=implicitn�)
//extern	bool				UserFullScreen;		// u�ivatelem po�adov�n celoobrazovkov� m�d 3D
extern	int					UserScreenBits;		// u�ivatelsk� po�et bit� (0=implicitn�)


// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI
	  
// zvolen� rozhran� a ovlada�
extern	int					D3DIntUser;			// u�ivatelem zadan� rozhran� 0 a� D3INTERFACES-1 (0=automatick�)
extern	int					D3DIntAkt;			// typ aktivn�ho rozhran� (0=nen�,1=GL0,2=GL1,3=GL2,4=DX3,5=DX5,6=DX6,7=DX7,8=DX8)
extern	int					D3DDevUser;			// u�ivatelem zadan� typ ovlada�e 0 a� D3DEVICES-1 (0=automatick�)
extern	int					D3DDevAkt;			// typ aktivn�ho ovlada�e (0=nen�,1=HAL,2=TnLHal,3=REF,4=RGB,5=MMX,6=Ramp)



// ovlada�e a rozhran� 3D
enum D3DEVICE {
	NDefDev = 0,			// implicitn� ovlada�
	NHALDev = 1,			// HAL
	NTnLHalDev = 2,			// TnLHal
	NRefDev = 3,			// REF
	NRGBDev = 4,			// RGB
	NMMXDev = 5,			// MMX
	NRampDev = 6,			// Ramp
	D3DEVICES = 7,			// po�et typ� ovlada��
};

enum D3INTERFACE {
	NDefInt = 0,			// implicitn� rozhran�
	NGL0Int = 1,			// OpenGL 1.0
	NGL1Int = 2,			// OpenGL 1.1
	NGL2Int = 3,			// OpenGL 2.0
	NDX3Int = 4,			// DirectX 3
	NDX5Int = 5,			// DirectX 5
	NDX6Int = 6,			// DirectX 6
	NDX7Int = 7,			// DirectX 7
	NDX8Int = 8,			// DirectX 8
	D3INTERFACES = 9,		// po�et typ� rozhran�
};

// tabulka ID k�d� ovlada�� Direct3D 
extern	const GUID* Dev3DTab[D3DEVICES];

// videom�d Direct3D
extern	bool				D3DFullScreen;		// je celoobrazovkov� re�im s Direct3D
extern	int					D3DNumVideoModes;	// po�et videom�d� v tabulce
extern	VIDEOMODEITEM*		D3DTabVideoModes;	// tabulka videom�d�
//extern	CString				D3DListVideoModes;	// textov� seznam videom�d�

// vlastnosti za��zen�
//extern	D3DCAPS8*			D3DCaps8;			// buffer vlastnost� za��zen� Direct3D
extern	int					MaxTextureWidth;	// maxim�ln� ���ka textury
extern	int					MaxTextureHeight;	// maxim�ln� v��ka textury
extern	int					MaxTextureRepeat;	// maxim�ln� po�et opakov�n� textury
extern	bool				SquareTexture;		// textury mus� b�t �tvercov�
extern	bool				TexturePow2;		// rozm�r textur mus� b�t mocnina 2
extern	bool				IsHWRaster;			// je HW rasterizace
extern	bool				IsHWTransLight;		// je HW transformace a osv�tlen�
extern	bool				IsPureDevice;		// pouze HW obsluha
extern	bool				IsBlending;			// jsou blending operace
extern	bool				IsMagFilter;		// podporuje filtr p�i zv�t�en�
extern	bool				IsMinFilter;		// podporuje filtr p�i zmen�en�
extern	bool				IsMipMap;			// podporuje mipmap textury
extern	bool				AlphaGreaterEqual;	// lze porovn�vat Alpha	>=
extern	bool				AlphaGreater;		// lze porovn�vat Alpha >
extern	int					MaxVertexNum;		// maxim�ln� po�et vrchol� skute�n�
extern	int					MaxVertexNum2;		// maxim�ln� po�et vrchol� pro renderov�n�
extern	int					MaxTextureStages;	// maxim�ln� po�et stup�� textur
extern	int					MaxTextureSimult;	// maxim�ln� po�et textur pro jeden pr�chod
extern	bool				IsTableFog;			// podporov�na tabulkov� mlha
extern	bool				IsVertexFog;		// podporov�na vrcholov� mlha
extern	bool				IsRangeFog;			// podporov�na vzd�lenostn� mlha

// p�ep�na�e nastaven�
extern	bool				D3DWireframeAkt;	// vypl�ov�n� ploch aktu�ln� zapnut
extern	bool				D3DWireframeGlobal;	// vypl�ov�n� ploch glob�ln� povoleno
extern	bool				D3DLightOnAkt;		// osv�tlen� aktu�ln� zapnuto
extern	bool				D3DLightOnGlobal;	// osv�tlen� glob�ln� povoleno
extern	bool				D3DShadesAkt;		// plynul� st�nov�n� aktu�ln� zapnuto
extern	bool				D3DShadesGlobal;	// plynul� st�nov�n� glob�ln� povoleno
extern	bool				D3DMinFilterAkt[MAX_STAGES];	// filtrace zmen�en�ch textur zapnuta
extern	bool				D3DMinFilterGlobal;	// filtrace zmen�en�ch textur glob�ln� povolena
extern	bool				D3DMagFilterAkt[MAX_STAGES];	// filtrace zv�t�en�ch textur zapnuta
extern	bool				D3DMagFilterGlobal;	// filtrace zv�t�en�ch textur glob�ln� povolena
extern	bool				D3DMipFilterAkt[MAX_STAGES];	// filtrace vzd�len�ch textur zapnuta
extern	bool				D3DMipFilterGlobal;	// filtrace vzd�len�ch textur glob�ln� povolena


extern	int					D3DAktStage;		// aktivn� stupe� textur

extern	bool				D3DRamp;			// je ovlada� Ramp

extern	bool				D3DVSync;			// synchronizovat s VSYN

extern	bool				D3DVertFog;			// pouze vektorov� mlha (SW driver)


extern	int ReSet3DCount;
extern	int ReSet3DCountN;


extern	bool				D3DCullingUnknown;	// stav p�ep�na�e odstra�ov�n� ploch nezn�m�
extern	short				D3DCullingAkt;		// volba zobrazen�ch ploch
extern	bool				D3DCameraInvert;	// je inverzn� kamera, invertovat plo�ky
//extern	bool				D3DCullingGlobal;	// odstra�ov�n� ploch glob�ln� povoleno

// funkce Direct3DCreate8
//typedef	IDirect3D8* (WINAPI *DIRECT3DCREATE8) (UINT SDKVersion);
//extern DIRECT3DCREATE8	pDirect3DCreate8;		// funkce Direct3DCreate8 (NULL=nen�)

// ovlada�e Direct3D8
extern	bool				IsDirect3D;			// prob�hla inicializace Direct3D
//extern	IDirect3D8*			Direct3D;			// objekt Direct3D verze 8 (NULL=nen�)
//extern  IDirect3DDevice8*	Direct3DDev;		// za��zen� Direct3D (NULL=nen�)
//extern	int					D3DDevUser;			// u�ivatelem zadan� typ za��zen� (0=automatick�)
//extern	int					D3DDevAkt;			// typ aktivn�ho za��zen� (0=nen�,1=HAL,2=TnLHal,3=REF,4=RGB,5=MMX,6=Ramp)
//extern	int					D3DIntUser;			// u�ivatelem zadan� interface (0=automatick�)
//extern	int					D3DIntAkt;			// typ aktivn�ho rozhran� (0=nen�,1=GL0,2=GL1,3=GL2,4=DX4,5=DX5,6=DX6,7=DX7,8=DX8)

// okno Direct3D
extern	bool				D3D;				// je okno Direct3D
//extern	bool				D3DDX;				// je okno 3D - ovlada� DirectX
//extern	bool				D3DGL;				// je okno 3D - ovlada� OpenGL
extern	bool				D3D2D;				// je kombinace 3D s 2D grafikou
extern	bool				D3DFull;			// okno 3D vyu��v� celou plochu hlavn�ho okna

extern	bool				D3DReturn;			// blokov�n� Alt+Enter p�i D3D m�du

// form�ty textur (dwRGBBitCount=0 pro neplatn� form�t)
extern	DDPIXELFORMAT		D3DTextFormatA8R8G8B8;			// form�t textury D3DFMT_A8R8G8B8
extern	DDPIXELFORMAT		D3DTextFormatA4R4G4B4;			// form�t textury D3DFMT_A4R4G4B4
extern	DDPIXELFORMAT		D3DTextFormatA1R5G5B5;			// form�t textury D3DFMT_A1R5G5B5
extern	DDPIXELFORMAT		D3DTextFormatR8G8B8;			// form�t textury D3DFMT_R8G8B8
extern	DDPIXELFORMAT		D3DTextFormatR5G6B5;			// form�t textury D3DFMT_R5G6B5
extern	DDPIXELFORMAT		D3DTextFormatX1R5G5B5;			// form�t textury D3DFMT_X1R5G5B5
extern	DDPIXELFORMAT		D3DTextFormatX8R8G8B8;			// form�t textury D3DFMT_X8R8G8B8
extern	DDPIXELFORMAT		D3DTextFormatX4R4G4B4;			// form�t textury D3DFMT_X4R4G4B4
extern	bool				D3DLowText;			// pou��vat ni��� kvalitu textur
extern	bool				D3DTextFormatOK;	// nalezen platn� form�t textur

// form�ty Z-buffer� (dwZBufferBitDepth=0 pro neplatn� form�t) - jen pro hled�n� form�tu Z bufferu
extern	DDPIXELFORMAT		D3DZBufferFormat24S8;			// form�t bufferu 24 bit�, stencil 8 bit�
extern	DDPIXELFORMAT		D3DZBufferFormat24S4;			// form�t bufferu 24 bit�, stencil 4 bity
extern	DDPIXELFORMAT		D3DZBufferFormat15S1;			// form�t bufferu 15 bit�, stencil 1 bit
extern	DDPIXELFORMAT		D3DZBufferFormat32S0;			// form�t bufferu 32 bit�, stencil 0 bit�
extern	DDPIXELFORMAT		D3DZBufferFormat24S0;			// form�t bufferu 24 bit�, stencil 0 bit�
extern	DDPIXELFORMAT		D3DZBufferFormat16S0;			// form�t bufferu 16 bit�, stencil 0 bit�
extern	DDPIXELFORMAT		D3DZBufferFormat8S0;			// form�t bufferu 8 bit�, stencil 0 bit�
extern	bool				D3DZBufferFormatOK;				// nalezen platn� form�t Z-bufferu

// stencil buffer
extern	bool				D3DSBufferFormatOK;				// nalezen Stencil buffer
extern	int					D3DSBufferDepth;				// hloubka Stencil bufferu (bit�)
extern	int					D3DSBufferMax;					// maxim�ln� hodnota Stencil bufferu
extern	bool				D3DSBufferClear;				// po�adavek k vymaz�n� Stencil bufferu
extern	bool				D3DShadows;						// povoleno pou��v�n� st�n�

// projek�n� matice (pozor, �len _34 matice nesm� b�t < 0 !!!)
extern	bool				D3DProjAkt;			// po�adavek aktualizace projek�n� matice
//extern	double				D3DHalfHeight;		// v��ka horizontu
//extern	double				D3DAspectRatio;		// pom�r v��ka/���ka
extern	double				D3DFrontClip;		// vzd�lenost p�edn� omezuj�c� roviny
extern	double				D3DBackClip;		// vzd�lenost zadn� omezuj�c� roviny
//extern	double				D3DViewAngle;		// �hel ���ky pohledu
extern	D3DMATRIX			D3DProjMatrix;		// projek�n� matice (nepou�it� �leny jsou = 0)
extern	int					D3DProjection;		// typ projekce (0=persp, 1=orto, 2=r-persp, 3=r-orto)
extern	bool				D3DRightHand;		// pravoruk� projekce (typ projekce 2 a 3)

// pohledov� matice (odvozen� z kamery)
extern	bool				D3DViewAkt;			// po�adavek aktualizace pohledov� matice
extern	D3DMATRIX			D3DViewMatrix;		// pohledov� matice (vytvo�en� z objektu kamery)

// kombinovan� matice projek�n�*pohledov� (pro p�epo�et 2D objekt�)
//extern	bool				D3DViewProjAkt;		// aktualizovat v�po�et kombinovan� matice
//extern	D3DMATRIX			D3DViewProjMatrix;	// kombinovan� matice

// rozhran� OpenGL
//extern	bool				IsOpenGL;			// prob�hla inicializace OpenGL

//extern	bool				OKOpenGL;			// inicializace OpenGL je �sp�n�

//#ifdef OPENGL11
//extern	bool				OKOpenGL11;			// inicializace OpenGL 1.1 je �sp�n�
//#endif // OPENGL11

//extern	HINSTANCE			OpenGLLib;			// knihovna OPENGL32.DLL
//extern	HGLRC				GLContext;			// renderovac� plocha OpenGL
//extern	HPALETTE			GLPalette;			// palety pro OpenGL

// pohledov� matice (odvozen� z kamery)
extern	bool				D3DViewAkt;			// po�adavek aktualizace pohledov� matice
extern	D3DMATRIX			D3DViewMatrix;		// pohledov� matice (vytvo�en� z objektu kamery)

// frustum (odvozeno z pohledov� a projek�n� matice)
extern	bool				D3DFrustumAkt;		// aktualizovat frustum
extern	PLANE				D3DFrustum[6];		// 6 rovin projek�n�ho jehlanu

// rozm�ry okna Direct3D zadan� u�ivatelem, bez omezen� rozsahu
extern	int					D3DX0;				// po��tek X okna Direct3D (zleva) zadan� u�ivatelem
extern	int					D3DY0;				// po��tek Y okna Direct3D (zdola) zadan� u�ivatelem
extern	int					D3DW0;				// ���ka okna Direct3D zadan� u�ivatelem
extern	int					D3DH0;				// v��ka okna Direct3D zadan� u�ivatelem

// rozm�ry okna Direct3D zadan� u�ivatelem, s omezen�m na plochu (pro zobrazen� grafiky)
extern	int					D3DX;				// po��tek X okna Direct3D (zleva)
extern	int					D3DY;				// po��tek Y okna Direct3D (zdola)
extern	int					D3DW;				// ���ka okna Direct3D
extern	int					D3DH;				// v��ka okna Direct3D

// skute�n� rozm�ry okna Direct3D na obrazovce (omezen� na klientsk� sou�adnice okna)
extern	int					D3DLeft;			// skute�n� po��tek X okna Direct3D
extern	int					D3DTop;				// skute�n� po��tek Y okna Direct3D (shora)
extern	int					D3DWidth;			// skute�n� ���ka okna Direct3D
extern	int					D3DHeight;			// skute�n� v��ka okna Direct3D
extern	double				D3DWidthHeight2;	// odmocnina s pom�ru D3DWidth/D3DHeight

// u�ivatelsk� nastaven�
//extern	bool				D3DDither;			// pou��t dithering Direct3D
//extern	bool				D3DRendReq;			// po�adavek p�erenderov�n� Direct3D
//extern	int					D3DQuality;			// kvalita renderov�n�

// objekty Direct3D
//extern	LPDIRECTDRAWSURFACE D3Surface;			// povrch pro Direct 3D (NULL=nen�)
//extern	LPDIRECTDRAWSURFACE D3ZBuffer;			// povrch pro Direct 3D Z-buffer (NULL=nen�)
//extern	LPDIRECT3DVIEWPORT	D3DViewport;		// viewport pro Direct 3D
//extern	LPDIRECT3DMATERIAL	D3DBackMaterial;	// materi�l pozad� sc�ny
//extern	D3DMATERIALHANDLE	hD3DBackMaterial;	// handle materi�lu pozad� sc�ny

// objekty Direct3DRM
//extern	LPDIRECT3DRM		Direct3DRM;			// objekt Direct3D RM
//extern	LPDIRECT3DRM2		Direct3DRM2;		// objekt Direct3D RM 2
//extern	LPDIRECT3DRMDEVICE	Direct3DRMDev;		// za��zen� Direct3D RM
//extern	LPDIRECT3DRMDEVICE2	Direct3DRMDev2;		// za��zen� Direct3D RM 2
//extern	LPDIRECT3DRMFRAME2	D3Scene;			// r�m sc�ny
//extern	LPDIRECT3DRMFRAME2	D3Camera;			// r�m kamery
//extern	LPDIRECT3DRMVIEWPORT D3View;			// okno pohledu


// pozad� sc�ny
extern	DWORD				D3DBackCol;			// barva pozad� sc�ny (MAXDWORD = nemazat)
extern	DWORD				D3DBackColRGB;		// barva pozad� sc�ny ve form�tu RGB
extern	D3DCOLORVALUE		D3DBackColF;		// barva pozad� sc�ny ve form�tu float
extern	bool				D3DBackColAkt;		// barva pozad� zm�n�na
extern	int					D3DBackText;		// textura pozad� sc�ny (-1 = nen�)
extern	bool				D3DBackTextAkt;		// textura pozad� zm�n�na
extern	int					D3DBackMat;			// materi�l pozad� sc�ny
extern	D3DMATERIAL8		D3DBackMatMat;		// buffer materi�lu pozad� sc�ny
//extern	IDirect3DVertexBuffer8*	D3DBackBuf;		// buffer textury Direct3D

// nastaven� mlhy
extern	BOOL				D3DFogOn;			// mlha je aktu�ln� zapnuta (-1 = nezn�m�)
extern	bool				FogAkt;				// mlhu je nutno aktualizovat
extern	bool				FogKorig;			// korigovan� mlha (p�i��t� se +4 k typu mlhy)
extern	bool				FogOn;				// mlha je zapnuta
extern	DWORD				FogColor;			// barva mlhy ve form�tu Petra BGR (MAXDWORD=vypnuto)
extern	DWORD				FogColorRGB;		// barva mlhy ve form�ty RGB
extern	D3DCOLORVALUE		FogColorF;			// barva mlhy ve form�tu float
extern	int					FogType;			// typ mlhy (0=line�rn�, 1=exponenci�ln�, 2=kvadratick�)
extern	double				FogStart;			// za��tek line�rn� mlhy
extern	double				FogEnd;				// konec line�rn� mlhy
extern	double				FogDens;			// hustota exponenci�ln� a kvadratick� mlhy (0 a� 1)

// ambient osv�tlen�
extern	DWORD				D3DAmbientCol;		// ambient osv�tlen� ve form�tu Petra BGR
extern	DWORD				D3DAmbientColRGB;	// ambient osv�tlen� ve form�tu RGB
extern	D3DCOLORVALUE		D3DAmbientColF;		// ambient osv�tlen� ve form�tu float
extern	bool				D3DAmbientColAkt;	// ambient osv�tlen� zm�n�no



//extern	CPicture			D3ScenePic;			// textura pozad� sc�ny
//extern	bool				D3SceneBack;		// je pozad� sc�ny

extern	int					D3DLevel;			// slo�itost objekt� (2 a v�ce)
extern	bool				D3DCreateUpper;		// vytv��et horn� podstavu objekt�
extern	bool				D3DCreateLower;		// vytv��et doln� podstavu objekt�
extern	int					D3D_ID;				// aktivn� objekt (r�m) Direct3D
extern	int					TerenID;			// naposledy zvolen� objekt ter�nu (-1=nen�)

//extern	int					D3DFilter0;			// filtrace textur zadan� u�ivatelem (-1=implicitn�)
//extern	int					D3DFilter;			// aktu�ln� filtrace textur
//extern	bool				D3DMipmaps;			// pou��vaj� se mipmap
extern	double				D3DSmooth;			// vyhlazen� ditheringu textur (1=ne)

// zrychlen� renderov�n�
//extern	bool				OpenGLQMat;			// pou��vat zkr�cen� v�po�ty matic
													// - pohledov� matice se n�sob�
													//   s projek�n� matic�, pro objekty
													//   se nastav� pouze modelov� matice
													// - v jin�m p��pad� se pohledov�
													//   matice n�sob� s ka�dou modelovou
													//   matic�
													//   (mus� b�t vypnuto v p��pad� mlhy)

// nastaven� kamery a mlhy
//extern	D3DVALUE			FogStart;			// za��tek mlhy
//extern	D3DVALUE			FogEnd;				// konec mlhy
//extern	D3DVALUE			FogDens;			// hustota mlhy
//extern	D3DRMPROJECTIONTYPE D3DProj;			// typ projekce


// m��en� uplynul�ho �asu
extern	double D3DLastTime;							// posledn� �as renderov�n�
extern	double D3DElapsedTime;						// uplynul� �as od posledn�ho renderov�n�
extern	double D3DAverageTime;						// st�ada� st�edn�ho �asu
extern	double D3DAverageFreq;						// st�edn� frekvence
extern	int D3DAverageFreqI;						// frekvence Integer


#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------

/////////////////////////////////////////////////////////////////////////////
// glob�ln� prom�nn�

extern	HWND		MainFrame;				// hlavn� okno aplikace
extern	TCHAR		MainFrameClass[];		// n�zev t��dy hlavn�ho okna

extern	int			MainFrameStyleEx;		// roz���en� styl
extern	int			MainFrameStyle;			// styl okna

//extern	TCHAR		MainFrameSubClass[];	// n�zev t��dy podokna
extern	bool		MainFrameVisible;		// hlavn� okno zobrazeno
extern	HWND		StatusBar;				// stavov� li�ta

extern	int			MainFrameX;				// sou�adnice X st�edu hlavn�ho okna
extern	int			MainFrameY;				// sou�adnice Y st�edu hlavn�ho okna

extern	bool		ReqClose;				// po�adavek uzav�en� okna od u�ivatele

extern	bool		StatusVisible;			// p��znak viditelnosti stavov�ho ��dku

extern	RECT		ClientRect;				// klientsk� oblast hlavn�ho okna
extern	int			ClientWidth;			// ���ka klientsk� oblasti
extern	int			ClientHeight;			// v��ka klientsk� oblasti

//extern	HCURSOR		CurAkt;					// aktivn� kurzor
extern	HCURSOR		CurArrow;				// (standardn�) �ipka
extern	HCURSOR		CurArrow2;				// (standardn�) �ipka pro DirectX
//extern	HCURSOR		CurWait;				// �ek�n�

extern	CString		AktCaptionText;			// text titulku hlavn�ho okna
extern	CString		AktStatusText;			// zobrazen� text stavov� li�ty

// stav my�i (aktualizov�no v PreTranslateMessage)
extern	bool		LMouseDown;				// lev� tla��tko stisknuto
extern	bool		LMouseClick;			// klik lev�ho tla��tka
extern	bool		LMouseDClick;			// dvojklik lev�ho tla��tka

extern	bool		RMouseDown;				// prav� tla��tko stisknuto
extern	bool		RMouseClick;			// klik prav�ho tla��tka
extern	bool		RMouseDClick;			// dvojklik prav�ho tla��tka

extern	POINT		MouseScreen;			// sou�adnice my�i na displeji
extern	POINT		MouseMain;				// sou�adnice my�i v hlavn�m okn�
extern	bool		MouseValid;				// p��znak aktualizace sou�adnice my�i

extern	bool		ProgramFill;			// pln�n� prvku okna programem

// buffer hledan�ho textu v okn� RichEdit
extern	LPTSTR		FindBuff;				// buffer hledan�ho textu (v okn� RichEdit)
extern	bool		FindMatchCase;			// p�i hled�n� rozli�ena velk� a mal� p�smena
extern	bool		FindWholeWord;			// hledat jen cel� slova
extern	HWND		FindDialogBox;			// handle na dialog hled�n� (NULL=nen�)
extern	UINT		FindMsgString;			// identifik�tor zpr�vy hled�n�

/////////////////////////////////////////////////////////////////////////////
// p��prava po�adovan�ch rozm�r� okna

void AdjustMainFrame(RECT* wrc);


/////////////////////////////////////////////////////////////////////////////
// prvn� zobrazen� hlavn�ho okna

void MainFrameShow();


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� hlavn�ho okna aplikace

bool MainFrameCreate();

/////////////////////////////////////////////////////////////////////////////
// p�epo�et rozm�r� okna Direct3D (podle �daj� zadan�ch u�ivatelem)
// vrac� TRUE=m� b�t okno 3D

bool RecalcD3D();

/////////////////////////////////////////////////////////////////////////////
// inicializace obsluhy DirectDraw (vrac� TRUE=je DirectDraw)

bool InitDirectDraw();

/////////////////////////////////////////////////////////////////////////////
// inicializace seznamu videom�d� (vol� se p�i prvn�m pou�it�)

void InitVideoModes();

/////////////////////////////////////////////////////////////////////////////
// set��d�n� videom�d� displeje (na za��tku je nejmen�� rozli�en� a nejni��� po�et bit�)

void SortDisplayModes(VIDEOMODEITEM* tab, int num);

/////////////////////////////////////////////////////////////////////////////
// start videom�du (vrac� TRUE=OK)

bool StartVideoMode();

/////////////////////////////////////////////////////////////////////////////
// stop aktivn�ho videom�du

void StopVideoMode();

/////////////////////////////////////////////////////////////////////////////
// konverze barvy z form�tu Petra na form�t D3DCOLORVALUE (float)

void BGRFloat(DWORD src, D3DCOLORVALUE* dst);

/////////////////////////////////////////////////////////////////////////////
// inicializace objekt� sc�ny (vol� se je�t� p�ed inicializac� 3D!)

void Init3D();

/////////////////////////////////////////////////////////////////////////////
// nalezen� nejbli���ho videom�du v tabulce (vrac� -1=nenalezeno)

int FindVideoMode(VIDEOMODEITEM* tab, int tabn, int width, int height, int bits);


/////////////////////////////////////////////////////////////////////////////
// aktivace videom�du DirectX (vrac� TRUE=OK)

//bool InitDXVideoMode(int width, int height, int bits);


/////////////////////////////////////////////////////////////////////////////
// ukon�en� obsluhy DirectDraw

//void TermDirectDraw();

/////////////////////////////////////////////////////////////////////////////
// inicializace obsluhy Direct3D (vrac� TRUE=je Direct3D)

//bool InitDirect3D();

/////////////////////////////////////////////////////////////////////////////
// p�id�n� form�tu textur

void D3DEnumPixelFormat(DDPIXELFORMAT* pf);

/////////////////////////////////////////////////////////////////////////////
// p�id�n� form�tu Z-bufferu

void D3DEnumZBufferFormat(DDPIXELFORMAT* pf);

/////////////////////////////////////////////////////////////////////////////
// v�b�r u�ivatelsk�ho ovlada�e Direct3D

bool UserD3DDevice();

// p�enastaven� filtrace textur
//void ReSetFilter();

// p�enastaven� kvality renderov�n�
//void ReSetD3DQuality();

// p�enastaven� p�ep�na�e Dithering
//void ReSetD3DDither();
	
// z�m�na modr� a �erven� barvy v RGB
DWORD XRGB(DWORD col);
	
// zji�t�n� parametr� mlhy
//void GetFogPar();

// nastaven� parametr� mlhy
//void SetFogPar();
	
// zobrazen� animace
//void DispAnimate();
		  
// vyrenderov�n� sc�ny
void D3Render(bool disp);

// animace
//void Animate();

/////////////////////////////////////////////////////////////////////////////
// nastaven� stavu Direct3D (nekontroluje rozsahy a platnosti!)

void _fastcall SetD3DRenderState(D3DRENDERSTATETYPE state, DWORD value);

/////////////////////////////////////////////////////////////////////////////
// nastaven� m�du vypl�ov�n� ploch

void SetD3DWireframe(bool on);

/////////////////////////////////////////////////////////////////////////////
// nastaven� m�du zapnut� osv�tlen�

void SetD3DLightOn(bool on);

/////////////////////////////////////////////////////////////////////////////
// nastaven� m�du plynul�ho st�nov�n�

void SetD3DShades(bool on);

/////////////////////////////////////////////////////////////////////////////
// nastaven� m�du filtrace textur p�i zv�t�en�

void SetD3DMagFilter(bool on, int stage);

/////////////////////////////////////////////////////////////////////////////
// nastaven� m�du filtrace zmen�en�ch a vzd�len�ch textur

void SetD3DMinMipFilter(bool min, bool mip, int stage);

/////////////////////////////////////////////////////////////////////////////
// nastaven� odstra�ov�n� ploch

void SetD3DCullingKorig(short val);
void SetD3DCulling(short val);

/////////////////////////////////////////////////////////////////////////////
// zapnut� mlhy

void D3DCheckFogOn();

void SetD3DFogOn(BOOL on);

/////////////////////////////////////////////////////////////////////////////
// resetov�n� p�ep�na�� nastaven� 

void ResetD3DSwitch();

/////////////////////////////////////////////////////////////////////////////
// uvoln�n� ovlada�e Direct3D

void DeSelectD3DDev();

/////////////////////////////////////////////////////////////////////////////
// aktualizace viewportu (nap�. po zm�n� velikosti okna)

bool UpdateViewport();


/////////////////////////////////////////////////////////////////////////////
// zapnut�/vypnut� celoobrazovkov�ho m�du

void SetFullScreen(bool full);


/////////////////////////////////////////////////////////////////////////////
// znovunastaven� videom�du v celoobrazovkov�m m�du

void ReSetDisplayMode();


/////////////////////////////////////////////////////////////////////////////
// aktualizace klientsk�ch sou�adnic

void InitClientRect(WINITEM* item);


/////////////////////////////////////////////////////////////////////////////
// zobrazen� kurzoru �ek�n� (zah�jen� a ukon�en� mus� b�t do p�ru!)

//void BeginWaitCursor();


/////////////////////////////////////////////////////////////////////////////
// vypnut� kurzoru �ek�n� (zah�jen� a ukon�en� mus� b�t do p�ru!)

//void EndWaitCursor();


/////////////////////////////////////////////////////////////////////////////
// nastaven� textu stavov� li�ty

void SetStatusText(CString& text);


/////////////////////////////////////////////////////////////////////////////
// zobrazen� stavov�ho ��dku

void SetStatusVisible(bool show);


/////////////////////////////////////////////////////////////////////////////
// nastaven� titulku pro konzolu

void SetConsoleText();


/////////////////////////////////////////////////////////////////////////////
// zobrazen� jm�na v titulku okna

void SetCaptionText(CString& text);

/////////////////////////////////////////////////////////////////////////////
// obsluha zpr�vy p�ed rozesl�n�m do oken

BOOL PreTranslateMessage(MSG* msg);


/////////////////////////////////////////////////////////////////////////////
// aktualizace p�ede�l�ho okna

void AktPrevWindow();
