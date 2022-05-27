
/***************************************************************************\
*																			*
*									D3D buffery								*
*																			*
\***************************************************************************/

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

/***************************************************************************\
*																			*
*								D3D buffer - textury						*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// struktura položky textury

// Do referencí se zapoèítává i reference z obrázku. Položka je proto zrušena
// z bufferu až ve chvíli zrušení obrázku, potom až je ref=0

typedef struct D3DTITEM_
{
	int					Ref;		// poèet referencí na texturu (0=položka je volná)
	CPicture			Pic;		// obrázek textury
	BOOL				Mips;		// aktuální textura vytvoøena s mipmaps
	int					MipMaps;	// max. poèet úrovní mipmaps (0=neomezeno)
	void*				Data1;		// data textury pro rozhraní 1 (NULL=není)
	void*				Data2;		// data textury pro rozhraní 2 (NULL=není)
} D3DTITEM;

////////////////////////////////////////////////////////////////////
// data bufferu textur

extern	int				D3DT_Num;	// poèet položek v bufferu textur
extern	int				D3DT_Max;	// velikost bufferu textur (položek)
extern	D3DTITEM*		D3DT_Data;	// buffer textur

extern	int				D3DT_Active[MAX_STAGES];	// aktivnì nastavená textura (i je-li vypnutá)
extern	bool			D3DT_On[MAX_STAGES];	// textura je zapnuta

struct D3DFITEM_;

//extern	D3DFITEM_*		D3D_Rend[3*MAX_ZBIAS]; // položky k renderování (bìžné/blending/2D)

////////////////////////////////////////////////////////////////////
// pøidání textury do seznamu textur, vrací index textury

int D3DT_Add(CPicture pic);

////////////////////////////////////////////////////////////////////
// uvolnìní textury (kontroluje platnost indexu)

void D3DT_Del(int index);

////////////////////////////////////////////////////////////////////
// kontrola platnosti položky

inline BOOL D3DT_IsValid(const int index)
{ 
	return (((DWORD)index < (DWORD)D3DT_Num) &&
				(D3DT_Data[index].Ref > 0)); 
};

////////////////////////////////////////////////////////////////////
// poskytnutí adresy položky (bez kontroly indexu)

inline D3DTITEM* D3DT_Get(const int index)
{ 
	ASSERT(D3DT_IsValid(index));
	return &(D3DT_Data[index]);
}

////////////////////////////////////////////////////////////////////
// resetování cachování bufferu (pøed ukonèením ovladaèe)

void D3DT_Reset();


////////////////////////////////////////////////////////////////////
// mìkké resetování cachování bufferu (pøed resetováním ovladaèe pøi zmìnì viewportu)
// - v souèasnosti není potøeba žádná obsluha

inline void D3DT_SoftReset() {}

////////////////////////////////////////////////////////////////////
// nastavení aktivní textury (-1 = vypnuta)

void D3DT_Akt(int index, int stage);


/***************************************************************************\
*																			*
*								D3D buffer - materiál						*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// struktura položky materiálu

typedef struct D3DMITEM_
{
	D3DMATERIAL8		Material;	// materiál objektu
	void*				Data1;		// data objektu 1 (NULL=není)
	void*				Data2;		// data objektu 2 (NULL=není)
} D3DMITEM;

/////////////////////////////////////////////////////////////////////////////
// data bufferu materiálù (položka 0 = implicitní materiál)

extern	int				D3DM_Num;		// poèet položek v bufferu materiálù
extern	int				D3DM_Max;		// velikost bufferu materiálù (položek)	
extern	int				D3DM_Akt;		// aktivnì nastavený materiál
extern	D3DMITEM*		D3DM_Data;		// buffer materiálù

////////////////////////////////////////////////////////////////////
// pøidání materiálu do bufferu materiálù (vrací index materiálu)
// - pokud materiál již existuje, vrací index existujícího materiálu

int D3DM_Add(D3DMATERIAL8* mat);

////////////////////////////////////////////////////////////////////
// nastavení materiálu jako aktivní (bez kontroly indexu)

void D3DM_Set(const int index);

////////////////////////////////////////////////////////////////////
// resetování cachování bufferu (pøed ukonèením ovladaèe)

void D3DM_Reset();


////////////////////////////////////////////////////////////////////
// mìkké resetování cachování bufferu (pøed resetováním ovladaèe pøi zmìnì viewportu)

inline void D3DM_SoftReset() { D3DM_Reset(); }


////////////////////////////////////////////////////////////////////
// kontrola platnosti položky

inline BOOL D3DM_IsValid(const int index)
{ 
	return ((DWORD)index < (DWORD)D3DM_Num);
};


////////////////////////////////////////////////////////////////////
// poskytnutí adresy položky (bez kontroly indexu)

inline D3DMITEM* D3DM_Get(const int index)
{ 
	ASSERT(D3DM_IsValid(index));
	return &(D3DM_Data[index]);
}

/***************************************************************************\
*																			*
*								D3D buffer - svìtla							*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// struktura položky svìtla

typedef struct D3DLITEM_
{
	bool				IsUsed;		// svìtlo je použité
	bool				Akt;		// je nutné aktualizovat svìtlo
									// (platí i pro zrušené svìtlo)
	bool				Enable;		// svìtlo je zapnuto
	D3DLIGHT8			Light;		// popisovaè svìtla
	int					Frame;		// index rámu s odkazem na svìtlo
	DWORD				Diffuse;	// difusní (a specular) barva zadaná uživatelem
	double				Range;		// dosah zadaný uživatelem
	double				Intens;		// intenzita svìtla zadaná uživatelem (1=normal)
	double				Attenuation1; // lineární zeslabení zadané uživatelem
	double				Attenuation2; // kvadratické zeslabení zadané uživatelem
	double				Theta;		// úhel svìtelného kužele (vnitøní kužel)
	double				Phi;		// úhel polostínu (vnìjší kužel)
	void*				Data1;		// data objektu 1 (NULL=není)
	void*				Data2;		// data objektu 2 (NULL=není)
} D3DLITEM;

////////////////////////////////////////////////////////////////////
// data bufferu svìtel

extern	int				D3DL_Num;	// poèet položek v bufferu svìtel (vèetnì volných)
extern	int				D3DL_Max;	// velikost bufferu svìtel (položek)	
extern	D3DLITEM*		D3DL_Data;	// buffer svìtel

////////////////////////////////////////////////////////////////////
// vytvoøení nového svìtla (vrací index svìtla)

int D3DL_New();

////////////////////////////////////////////////////////////////////
// aktualizace svìtel

void D3DL_AktLight();

////////////////////////////////////////////////////////////////////
// zrušení svìtla (kontroluje platnost indexu)

void D3DL_Del(int index);

////////////////////////////////////////////////////////////////////
// resetování bufferu svìtel

void D3DL_Reset();

////////////////////////////////////////////////////////////////////
// mìkké resetování cachování bufferu (pøed resetováním ovladaèe pøi zmìnì viewportu)

inline void D3DL_SoftReset() { D3DL_Reset(); }

////////////////////////////////////////////////////////////////////
// kontrola platnosti položky

inline BOOL D3DL_IsValid(const int index)
{ 
	return (((DWORD)index < (DWORD)D3DL_Num) && 
		(D3DL_Data[index].IsUsed));
};

////////////////////////////////////////////////////////////////////
// poskytnutí adresy položky (bez kontroly indexu)

inline D3DLITEM* D3DL_Get(const int index)
{ 
	ASSERT(D3DL_IsValid(index));
	return &(D3DL_Data[index]);
}


/***************************************************************************\
*																			*
*								D3D buffer - rámy							*
*																			*
\***************************************************************************/

// omezení poètu vrcholù
#define MAXVERTICES	1024	// maximální poèet vrcholù pro jeden objekt

// vzor jednotkové matice
extern	D3DMATRIX	Matrix1Vzor;

// typy rámù
typedef enum _D3DFTYPE
{
	D3DFTYPE_GROUP = 0,		// prázdný rám (nebo scéna, index 0)
	D3DFTYPE_CAMERA,		// kamera (index 1)
	D3DFTYPE_OBJECT,		// 3D objekt (mesh, terén)
	D3DFTYPE_DECAL,			// 2D objekt
	D3DFTYPE_PICTURE,		// 2D obrázek (pøipojen ke kameøe, index 1)
	D3DFTYPE_LIGHT,			// svìtlo
	D3DFTYPE_SECTOR,		// sektor objektu rozdìleného na èásti
	D3DFTYPE_LENSFLARE,		// èoèkový odlesk (pøipojen ke kameøe, index 1)
} D3DFTYPE;

// typy poøadí rotací
typedef enum _D3DORDER
{
	D3DFORDER_XYZ = 0,		// XYZ
	D3DFORDER_XZY = 1,		// XZY
	D3DFORDER_YXZ = 2,		// YXZ
	D3DFORDER_YZX = 3,		// YZX
	D3DFORDER_ZXY = 4,		// ZXY
	D3DFORDER_ZYX = 5		// ZYX
} D3DORDER;

// zdroj materiálu
typedef enum _MATSOURCE
{
	MATSOURCE_OBJECT = 0,	// vnitøní materiál objektu (=implicitnì)
	MATSOURCE_PARENT,		// materiál zdìdìn od rodièe
	MATSOURCE_FRAME			// implicitní materiál tohoto rámu
} MATSOURCE;

// typy prùhlednosti (nastaveno pro zdroj nebo cíl blending operace)
typedef enum _BLENDTYPE
{
	BLEND_ZERO			= 0,	// (0, 0, 0, 0)
	BLEND_ONE			= 1,	// (1, 1, 1, 1)
	BLEND_SRCCOLOR		= 2,	// (Rs, Gs, Bs, As)
	BLEND_INVSRCCOLOR	= 3,	// (1-Rs, 1-Gs, 1-Bs, 1-As)
	BLEND_SRCALPHA		= 4,	// (As, As, As, As)
	BLEND_INVSRCALPHA	= 5,	// (1-As, 1-As, 1-As, 1-As)
	BLEND_DSTCOLOR		= 6,	// (Rd, Gd, Bd, Ad)
	BLEND_INVDSTCOLOR	= 7,	// (1-Rd, 1-Gd, 1-Bd, 1-Ad)
	BLEND_DSTALPHA		= 8,	// (Ad, Ad, Ad, Ad)
	BLEND_INVDSTALPHA	= 9,	// (1-Ad, 1-Ad, 1-Ad, 1-Ad)
	BLEND_SATURATE		= 10,	// menší ze složek SRC/DST
	BLEND_INVALID		= -1,	// neplatný typ operace
} BLENDTYPE;

extern	int	BlendTabD3D[11];	// tabulka blending operací pro Direct3D
extern	int	BlendTabOGL[11];	// tabulka blending operací pro OpenGL

extern	BLENDTYPE	D3DSrcBlend; // aktuální nastavení blending operace zdroje
extern	BLENDTYPE	D3DDstBlend; // aktuální nastavení blending operace cíle
extern	int			D3DAlphaRef;// aktuální nastavení alfa úrovnì

extern	int			D3DColorOp[MAX_STAGES];		// aktuální nastavení operací barev
extern	int			D3DColorArg1[MAX_STAGES];	// aktuální nastavení argumentu 1 operací barev
extern	int			D3DColorArg2[MAX_STAGES];	// aktuální nastavení argumentu 2 operací barev
extern	int			D3DAlphaOp[MAX_STAGES];		// aktuální nastavení operací barev
extern	int			D3DAlphaArg1[MAX_STAGES];	// aktuální nastavení argumentu 1 operací barev
extern	int			D3DAlphaArg2[MAX_STAGES];	// aktuální nastavení argumentu 2 operací barev
extern	int			D3DAddressU[MAX_STAGES];	// aktuální adresování textur U
extern	int			D3DAddressV[MAX_STAGES];	// aktuální adresování textur V
//extern	int			D3DBorder[MAX_STAGES];		// barva okolí textur
extern	float		D3DBias[MAX_STAGES];		// zjemnìní vzdálených textur
extern	DWORD		D3DTFactor;					// aktuální nastavení faktoru textur
extern	BOOL		D3DZWrite;					// aktuální hodnota hloubkového zápisu
extern	int			D3DZTest;					// aktuální hodnota hloubkového testu

extern	int			D3DAktMat;					// první položka k aktualizaci matic (-1=není)

/////////////////////////////////////////////////////////////////////////////
// popisovaè automatického mapování textur

typedef struct AUTOMAP_
{
	int				method;			// metoda mapování (0=adresy, 1=vrcholy, 2=plošky)
	int				type;			// typ mapování (0=plocha, 1=válec, 2=koule, 3=chrom, 4=èoèka)
	double			ox;				// souøadnice X poèátku mapování
	double			oy;				// souøadnice Y poèátku mapování
	double			oz;				// souøadnice Z poèátku mapování
	double			rz;				// rotace podle osy Z
	double			rx;				// rotace podle osy X
	double			ry;				// rotace podle osy Y
	double			ou;				// poèátek textury horizontálnì
	double			ov;				// poèátek textury vertikálnì
	double			su;				// mìøítko textury horizontálnì
	double			sv;				// mìøítko textury vertikálnì

	D3DMATRIX		m;				// použitá transformaèní matice
									//   testují se jen èleny 1 až 3,
									//	 èleny _41 až _43 uchovávají
									//	 vektor k pozorovateli
									// Pro neplatné mapování (napø. zmìna
									// parametrù) je _41 až _43 = 0

} AUTOMAP;

/////////////////////////////////////////////////////////////////////////////
// struktura položky rámu D3D (výpoèty se provádí v pøesnosti float)

typedef struct D3DFITEM_
{
	D3DFTYPE			Type;		// typ rámu

	int					Index;		// index této položky

// hiearchie (na zaèátku je scéna s indexem 0, nemá sousedy ani rodièe)
	int					Parent;		// index rodièe (-1 = není, 0 = scéna)
	int					Child;		// první potomek (-1 = není)
	int					Prev;		// pøedcházející soused stejné úrovnì (-1 = není)
	int					Next;		// další soused stejné úrovnì (-1 = není)

// informace k renderování
	int					Dist;		// druhá mocnina vzdálenosti od kamery k položce
	int					RendGroup;	// renderovací skupina (0 až 15)
	BOOL				ZWrite;		// zapisovat hloubkovou informaci
	int					ZTest;		// hloubkový test (1 až 8)

// rozdìlení objektu na èásti
	BOOL				AktSector;	// aktualizovat generování sektorù
	BOOL				UseSector;	// používají se sektory namísto objektu
	BOOL				ParSector;	// meziúschova parametru od rodièe
	BOOL				res2;

// viditelnost rámu
	bool				Visible;	// zapnuta viditelnost

// nastavení objektu
	bool				Wireframe;	// vyplòování ploch
	bool				Lighton;	// osvìtlení
	bool				Shades;		// plynulé stínování
	bool				MagFilter[MAX_STAGES];	// filtrace zvìtšených textur
	bool				MinFilter[MAX_STAGES];	// filtrace zmenšených textur
	bool				MipFilter[MAX_STAGES];	// filtrace vzdálených textur
	short				Culling;	// zobrazené plochy (0=obì, 1=pøední, 2=zadní)
	short				res3;

// snížená úroveò detailù LOD
	int					LODPrev;	// pøedcházející objekt pro LOD (-1=není)
	int					LODNext;	// další objekt pro LOD (-1=není)
	double				LODMax;		// maximální vzdálenost pro LOD
	float				LODMin2;	// minimální vzdálenost pro LOD ^2 (vèetnì této hodnoty)
	float				LODMax2;	// maximální vzdálenost pro LOD ^2 (bez této hodnoty)

// multitexturové operace
	int					Stages;		// aktivní poèet stupòù, pro které jsou vygenerovány buffery
	int					ColorOp[MAX_STAGES];	// operace barvy
	int					ColorArg1[MAX_STAGES];	// první argument barvy
	int					ColorArg2[MAX_STAGES];	// druhý argument barvy
	int					AlphaOp[MAX_STAGES];	// operace alfa
	int					AlphaArg1[MAX_STAGES];	// první argument alfa
	int					AlphaArg2[MAX_STAGES];	// druhý argument alfa
	int					AddressU[MAX_STAGES];	// mód adresování U
	int					AddressV[MAX_STAGES];	// mód adresování V
//	int					Border[MAX_STAGES];		// barva okolí textury v barvách Petra (BGR)
//	DWORD				BorderRGB[MAX_STAGES];	// barva okolí v módu RGB
//	D3DCOLORVALUE		BorderF[MAX_STAGES];	// barva okolí v módu float
	float				Bias[MAX_STAGES];		// zjemnìní vzdálených textur (0=bìžné)
	bool				UseText[MAX_STAGES];	// stupeò používá pøíkazy využívající texturu
	DWORD				TFactor;				// faktor textur v barvách Petra (BGR)
	DWORD				TFactorRGB;				// faktor textur v barvách RGB
	D3DCOLORVALUE		TFactorF;				// faktor textur v barvách float

// hranice (koule)
	double				BoundR0;	// vlastní netransformovaný polomìr hranice
	double				BoundR;		// netransformovaný polomìr s potomky
	double				ParScale;	// pøechodnì uschované mìøítko od rodièe
	bool				ParClip;	// uschovaný pøíznak od rodièe - testovat viditelnost
	bool				AktClip;	// položku je nutné testovat bìhem renderování
	bool				AktBound;	// aktualizovat polomìr hranice s potomky
	bool				res4;

// svìtlo
	int					Light;		// index svìtla (-1 = není)

// transformace rámu
	D3DORDER			Order;		// poøadí rotací
	double				ScaleX;		// zmìna mìøítka ve smìru X
	double				ScaleY;		// zmìna mìøítka ve smìru Y
	double				ScaleZ;		// zmìna mìøítka ve smìru Z
	double				ScaleB;		// mìøítko pro hranici (vìtší z mìøítek)
	double				RotateX;	// rotace podle osy X (normalizováno 0 až 2pi)
	double				SinX;		// sinus úhlu rotace podle X
	double				CosX;		// kosinus úhlu rotace podle X
	double				RotateY;	// rotace podle osy Y (normalizováno 0 až 2pi)
	double				RotateY2D;	// pøídavné natoèení 2D objektu na kameru
	double				RotateX2D;	// uschovaná souøadnice X relativnì ke kameøe
	double				RotateZ2D;	// uschovaná souøadnice Z relativnì ke kameøe
	int					res8;

	double				SinY;		// sinus úhlu rotace podle Y
	double				CosY;		// kosinus úhlu rotace podle Y
	double				RotateZ;	// rotace podle osy Z (normalizováno 0 až 2pi)
	double				SinZ;		// sinus úhlu rotace podle Z
	double				CosZ;		// kosinus úhlu rotace podle Z
	double				TransX;		// posun ve smìru X
	double				TransY;		// posun ve smìru Y
	double				TransZ;		// posun ve smìru Z

	bool				IsRotX;		// je rotace podle osy X (tj. úhel != 0)
	bool				IsRotY;		// je rotace podle osy Y (tj. úhel != 0)
	bool				IsRotZ;		// je rotace podle osy Z (tj. úhel != 0)

	bool				AktOwn;		// aktualizovat vlastní matici
	bool				AktWorld;	// aktualizovat výslednou matici

	bool				AktMat;		// zahrnut do øetìzce aktualizace matic
	short				res5;

	int					AktMatPrev;	// pøedchozí položka pro aktualizaci matic (-1=je první)
	int					AktMatNext;	// následující položka pro aktualizaci matic (-1=je poslední)

	D3DMATRIX			MatrixOwn;	// vlastní transformaèní matice
	D3DMATRIX			MatrixWorld;// výsledná transformaèní matice (+matice rodièe)

// automatické mapování textur
	AUTOMAP*			AutoMap;	// automatické mapování textur (NULL=není)

// parametry terénu
	bool				IsTerrain;	// objekt je terén
	bool				res6;
	short				res7;
	int					TerWidth;	// šíøka terénu (políèek)
	int					TerHeight;	// výška terénu (políèek)
	float*				TerMap;		// mapa terénu ((width+1)*(height+1) hodnot 0 až 1)

// klonování (pro 1 objekt je ClonePrev = CloneNext = Index)
// u klonù jsou sdílené tyto buffery:

//		TerMap

//		Vertex
//		Normal
//		VertCol
//		VertColF
//		TextUV
//		Face
//		FaceW

//		ShadeVert
//		ShadeFace
//		ShadeNorm
//		ShadeEdgeV
//		ShadeEdgeN
//		ShadeNext
//		ShadeLeft

//		Data1
//		Data2

	int					ClonePrev;	// pøedcházející klon (není klon: ClonePrev == Index)
	int					CloneNext;	// další klon (není klon: CloneNext == Index)

// vzor objektu
	int					VertNum;	// poèet vrcholù v seznamu vrcholù, normál, barev a souøadnic textur
	D3DVECTOR*			Vertex;		// seznam vektorù vrcholù
	D3DVECTOR*			Normal;		// seznam vektorù normál
	DWORD				AutoCol;	// aktuální automatická barva vrcholù (MAXDWORD = není)
	DWORD*				VertCol;	// seznam barev vrcholù RGBA (NULL=není)
	D3DCOLORVALUE*		VertColF;	// seznam barev vrcholù FLOAT (NULL=není)
	float*				TextUV[MAX_STAGES];	// souøadnice U a V textur (NULL=nejsou)
	int					FaceNum;	// poèet plošek v seznamu plošek
	int*				Face;		// seznam plošek (každá 3 vrcholy)
	WORD*				FaceW;		// seznam plošek ve formátu WORD (jen pokud to vyžaduje driver)

// implicitní povrch (barvy (int) jsou ve formátru Petra, tj. BGR)
	MATSOURCE			MatSource;	// použitý zdroj materiálu
	DWORD				Diffuse;	// difusní barva materiálu zadaná uživatelem
	DWORD				Ambient;	// ambient barva zadaná uživatelem (MAXDWORD=jako difusní)
	DWORD				Emissive;	// emisivní barva materiálu zadaná uživatelem
	DWORD				Specular;	// barva odlesku materiálu zadaná uživatelem
	double				Power;		// mocnina barvy odlesku zadaná uživatelem (0 až 128)
	int					Material;	// index materiálu (0 = implicitní)
	int					ParMat;		// index materiálu rodièe
									// (používá se pouze k pøechodné úschovì
									// materiálu rodièe bìhem renderování)
	DWORD				ParDif;		// difusní barva materiálu rodièe (pouze k úschovì)

// morfování
	int					MorphMax;	// poèet stupòù morfování (0=není)
	double				MorphStage;	// aktuální stupeò morfování (0 až MorphMax)
	D3DVECTOR**			MorphVert;	// buffery vrcholù
	D3DVECTOR**			MorphNorm;	// buffery normál
	float**				MorphUV;	// buffery souøadnic textur (pro každý stupeò MAX_STAGES bufferù)

// textura
	int					Texture[MAX_STAGES]; // index textury (-1 = není)
	int					ParText;	// index textury 0 rodièe
									// (používá se pouze k pøechodné úschovì
									// textury rodièe bìhem renderování)

// prùhlednost
	bool				IsBlend;	// je zapnut alpha blending (SRC<>1 nebo DST<>0)
	bool				IsAlphaRef;	// je zapnuto alfa porovnávání
	bool				BlendRend;	// vyžaduje dodateèný prùchod pro alfa blending
	BLENDTYPE			SrcBlend;	// blending operace pro zdroj
	BLENDTYPE			DstBlend;	// blending operace pro cíl
	int					SrcDstBlend; // kód blending operace v kódu uživatele = SRC + 10*DST
									// (1=normal, 11=oheò, 20=sklo, 54=pøekryv)
	double				AlphaRef;	// referenèní úroveò alfa kanálu (0=normal)
	int					AlphaRef2;	// referenèní úroveò ve tvaru BYTE (0=normal)

// stín (pro pøegenerování bufferù staèí zrušit jen "ShadeVert")
	double				ShadowRange; // dosah stínu
	bool				IsShadow;	// je zapnut stín
	bool				ShadowParent; // odvozovat vektor dopadu stínu od rodièe
	D3DVECTOR			ShadowPoint; // poslední vektor dopadu stínu ([0,0,0]=neplatný)

	int					ShadeVertN;	// poèet vrcholù
	D3DVECTOR*			ShadeVert;	// buffer vrcholù (vypuštìné zdvojené vrcholy)
	int*				ShadeFace;	// buffer plošek (každá ploška 3 indexy vrcholù)
	D3DVECTOR*			ShadeNorm;	// buffer normál plošek
	// hrany jsou kolem plošky orientovány proti smìru hodinových ruèièek (=kladný smìr pro vìjíø)
	// vrcholy hran: 0: v1-v3, 1: v3-v2, 2: v2-v1
	int*				ShadeEdgeV;	// vrcholy hran plošek (každá ploška 3 hrany po 2 vrcholech)
	int*				ShadeEdgeN;	// protichùdná hrana sousední plošky (každá ploška 3 indexy hran, -1=není)
	int*				ShadeNext;	// navazující hrana první vpravo z 2. vrcholu (každá ploška 3 indexy)
	int*				ShadeLeft;	// další hrana z vrcholu 1 smìrem doleva (každá ploška 3 indexy)

	char*				ShadeEdgeS;	// stav hrany (každá ploška 3 hrany)
									//   bit 0: ploška vlevo osvícená
									//   bit 1: ploška vpravo osvícená

	int					ShadowFanN;	// poèet obrysù v bufferu (poèet "vìjíøù")
	int					ShadowFan0;	// délka bufferu stínu celkem (=poèet vrcholù)
	int*				ShadowFan;	// poèty vrcholù pro jednotlivé obrysy ("vìjíøe")
	D3DVECTOR*			ShadowVolume; // buffer stínu (vìjíøe, 1 vrchol poèátek, zbytek vrcholy vìjíøe)

// datové buffery ovladaèù
	void*				Data1;		// datový buffer 1 (vrcholy), NULL=není
	void*				Data2;		// datový buffer 2 (indexy), NULL=není
	void*				Data3;		// datový buffer pro stíny, NULL=není

} D3DFITEM;


////////////////////////////////////////////////////////////////////
// popisovaèe bufferu rámù

extern	D3DFITEM*	D3DF_Data;		// ukazatel na data
extern	bool*		D3DF_Valid;		// pøíznaky platnosti položek
extern	int			D3DF_Max;		// velikost bufferu (položek)
extern	int			D3DF_Next;		// pøíští volná položka (-1 = není)

////////////////////////////////////////////////////////////////////
// seznam skupin renderování

extern	BOOL		RendSort[MAX_RENDGROUP];	// pøíznaky tøídìní skupin
extern	int			RendNum[MAX_RENDGROUP];		// poèty položek v bufferech
extern	D3DFITEM**	RendItems[MAX_RENDGROUP];	// seznam položek k renderování
extern	int			RendMax;					// velikost bufferù položek

////////////////////////////////////////////////////////////////////
// vynulování matice

inline void Matrix0(D3DMATRIX* m)
{
	MemFill(m, sizeof(D3DMATRIX), 0);
}

////////////////////////////////////////////////////////////////////
// vytvoøení jednotkové matice

inline void Matrix1(D3DMATRIX* m)
{
	MemCopy(m, &Matrix1Vzor, sizeof(D3DMATRIX));
}

////////////////////////////////////////////////////////////////////
// aktualizace transformaèních matic všech objektù
// (provádí se pøi snížené pøesnosti FPU!)

void D3DF_AktMatrix();

////////////////////////////////////////////////////////////////////
// transpozice matice (zdrojová a cílová matice mùže být stejná)

void _fastcall MatrixTrans(D3DMATRIX* dst, D3DMATRIX* src);

////////////////////////////////////////////////////////////////////
// vynásobení dvou matic (m = m1 * m2), výsledná matice nesmí být vstupní!

void _fastcall MatrixMul(D3DMATRIX* m, const D3DMATRIX* m1, const D3DMATRIX* m2);

////////////////////////////////////////////////////////////////////
// inverze matice (zdrojová a cílová matice mùže být stejná)

void MatrixInv(D3DMATRIX* dst, D3DMATRIX* src);

////////////////////////////////////////////////////////////////////
// vynásobení vektoru maticí (provádí se pøi snížené pøesnosti FPU)
// (zdrojový i cílový vektor mohou být shodné)

void VecXMat(D3DVECTOR* dst, D3DVECTOR* src, D3DMATRIX* mat);


////////////////////////////////////////////////////////////////////
// nastavení transformací podle transformaèní matice

void D3DF_SetTransMatrix(int index, D3DMATRIX* m);

////////////////////////////////////////////////////////////////////
// pøipojení do øetìzce aktualizace matic (kontroluje, zda je pøipojen)

void _fastcall D3DF_AktMatLink(D3DFITEM* item);

////////////////////////////////////////////////////////////////////
// odpojení z øetìzce aktualizace matic (kontroluje, zda je pøipojen)

void _fastcall D3DF_AktMatUnlink(D3DFITEM* item);

////////////////////////////////////////////////////////////////////
// vytvoøení nové položky bufferu (vrací index položky)

int D3DF_New();

////////////////////////////////////////////////////////////////////
// pøíprava položky k modifikaci (odpojení od klonù)

void D3DF_Modi(int index);

////////////////////////////////////////////////////////////////////
// kopie položky s potomky (vrací index nové položky)

int D3DF_Copy(int index);

////////////////////////////////////////////////////////////////////
// zrušení položky (kontroluje platnost indexu)

void _fastcall D3DF_Del(const int index);

////////////////////////////////////////////////////////////////////
// resetování cachování bufferu (pøed ukonèením ovladaèe)

void D3DF_Reset();

////////////////////////////////////////////////////////////////////
// mìkké resetování cachování bufferu (pøed resetováním ovladaèe pøi zmìnì viewportu)
// - v souèasnosti není potøeba žádná obsluha

inline void D3DF_SoftReset() {}

////////////////////////////////////////////////////////////////////
// zrušení všech položek v bufferu (ukládání zaène opìt po øadì)

void D3DF_DelAll();

////////////////////////////////////////////////////////////////////
// zrušení bufferù morfování

void D3DF_MorphReset(D3DFITEM* item);

/////////////////////////////////////////////////////////////////////////////
// aktualizace mìøítka pro hranici

void AktScaleB(D3DFITEM* item);

////////////////////////////////////////////////////////////////////
// inicializace manažeru vrcholù (index položky musí být platný!)

void D3DF_UsedInit(int inx);

////////////////////////////////////////////////////////////////////
// duplikace vrcholu (parametr = index vrcholu plošky), vrací nový index

int D3DF_UsedDuplik(int faceinx);

////////////////////////////////////////////////////////////////////
// nastavení normály vrcholu (pro daný index vrcholu plošky)

void D3DF_UsedNormal(int faceinx, float nx, float ny, float nz);

////////////////////////////////////////////////////////////////////
// nastavení barvy vrcholu (pro daný index vrcholu plošky)

void D3DF_UsedColor(int faceinx, DWORD col, float r, float g, float b, float a);

void D3DF_UsedColorB(int faceinx, BYTE r, BYTE g, BYTE b, BYTE a);

////////////////////////////////////////////////////////////////////
// nastavení adresy textury vrcholu (pro daný index vrcholu plošky)

void D3DF_UsedTextUV(int faceinx, float u, float v, int stage);

////////////////////////////////////////////////////////////////////
// napojení položky na rodièe (s odpojením od pøedcházejícího rodièe)
// kontroluje platnost obou indexù

void _fastcall D3DF_Link(const int parent, const int child);

////////////////////////////////////////////////////////////////////
// zmapování vlastní hranice (index musí být platný)

void D3DF_MapBound(int index);

////////////////////////////////////////////////////////////////////
// vytvoøení objektu (vrací index objektu)
// Po vytvoøení objektu je nutné nastavit normály!!!

int D3DF_CreateMesh(int vertN, D3DVECTOR* vert, int faceN, int* face);

////////////////////////////////////////////////////////////////////
// nastavení normál objektu (poèet normál musí souhlasit s poètem vrcholù!)

void D3DF_SetNormal(int index, D3DVECTOR* norm);

////////////////////////////////////////////////////////////////////
// korekce novì vytvoøeného aktuálního objektu pro pravoruký souøadný systém

void D3DF_KorRightHand();

////////////////////////////////////////////////////////////////////
// nastavení barev vrcholù objektu (poèet musí souhlasit s poètem vrcholù!)

void D3DF_SetVertCol(int index, D3DCOLORVALUE* col);

////////////////////////////////////////////////////////////////////
// nastavení mapování textur (poèet musí souhlasit s poètem vrcholù!)

void D3DF_SetTextUV(int index, float* textuv, int stage);

////////////////////////////////////////////////////////////////////
// nastavení adresování a zalamování textury

void D3DF_SetAddress(int index, int adru, int adrv, int stage);

////////////////////////////////////////////////////////////////////
// nastavení blending módu (1=normal, 11=oheò, 20=sklo, 54=pøekryv, 100=saturace)

void D3DF_SetBlend(int index, int blendmode);

////////////////////////////////////////////////////////////////////
// vygenerování normál (kontroluje index)

void D3DF_AutoNormals(int index, double angle);

////////////////////////////////////////////////////////////////////
// renderování objektù (pro Direct3D musí být BeginScene a EndScene!)

void D3DF_Render();

////////////////////////////////////////////////////////////////////
// renderování objektù s blending (pro Direct3D musí být BeginScene a EndScene!)

void D3DF_RenderBlend();

////////////////////////////////////////////////////////////////////
// renderování 2D obrázkù (pro Direct3D musí být BeginScene a EndScene!)

void D3DF_Render2D();

////////////////////////////////////////////////////////////////////
// kontrola platnosti položky

inline BOOL D3DF_IsValid(const int index)
{ 
	return (((DWORD)index < (DWORD)D3DF_Max) &&
				D3DF_Valid[index]); 
};

inline BOOL IsValidID() { return D3DF_IsValid(D3D_ID); }

////////////////////////////////////////////////////////////////////
// poskytnutí adresy položky (bez kontroly indexu)

inline D3DFITEM* D3DF_Get(const int index)
{ 
	ASSERT(D3DF_IsValid(index));
	return &(D3DF_Data[index]);
}

inline D3DFITEM* GetID() { return D3DF_Get(D3D_ID); }


// funkce aktivního rozhraní - definice
typedef	void (*D3STOP) ();						// ukonèení àunkce zaøízení
typedef	void (*D3SIZEVIEW) ();					// aktualizace viewportu po zmìnì velikosti
typedef	void (*D3MOVEVIEW) ();					// aktualizace viewportu po pøesunu
typedef	void (*D3DISP) ();						// zobrazení renderovacího bufferu
typedef	void (*D3CLEAR) ();						// vymazání renderovací plochy
typedef	void (*D3AKTPROJ) ();					// aktualizace projekèní matice
typedef	void (*D3AKTVIEW) ();					// aktualizace pohledové matice
typedef	void (*D3BEGIN) ();						// zahájení renderování scény
typedef	void (*D3END) ();						// ukonèení renderování scény
typedef void (*D3RENDER) (D3DFITEM* item);		// vyrenderování jednoho objektu
typedef void (*D3RESET) (D3DFITEM* item);		// resetování objektu pøed ukonèením ovladaèe
typedef void (*D3SRESET) (D3DFITEM* item);		// resetování stínu objektu pøed ukonèením ovladaèe
typedef void (*D3AKTWIREFRAME) ();				// aktualizovat pøepínaè vyplòování ploch
typedef void (*D3AKTLIGHTON) ();				// aktualizovat pøepínaè osvìtlení
typedef void (*D3AKTSHADES) ();					// aktualizovat pøepínaè plynulého stínování
typedef void (*D3AKTCULLING) ();				// aktualizovat pøepínaè odstraòování ploch
typedef void (*D3AKTMAGFILTER) (int stage);		// aktualizovat pøepínaè filtrace zvìtšených textur
typedef void (*D3AKTMINMIPFILTER) (int stage);	// aktualizovat pøepínaè filtrace zmenšených a vzdálených textur
typedef void (*D3AKTLIGHT) (int inx, D3DLITEM* item);	// aktualizace položky svìtla
typedef void (*D3SETMATERIAL) (D3DMITEM* item);	// nastavení materiálu
typedef void (*D3TEXTURERES) (D3DTITEM* item);	// resetování položky textury pøed ukonèením ovladaèe
typedef void (*D3TEXTURESET) (D3DTITEM* item, int stage);	// nastavení a zapnutí textury
typedef void (*D3TEXTUREON) (D3DTITEM* item, int stage);	// zapnutí pùvodní textury
typedef void (*D3TEXTUREOFF) (int stage);				// vypnutí textury
typedef void (*D3MATRESET) (D3DMITEM* item);	// resetování materiálu
typedef void (*D3LIGHTRESET) (int inx, D3DLITEM* item);	// resetování svìtla
typedef void (*D3AKTAMBIENT) ();				// aktualizace ambient osvìtlení
typedef int (*D3FREE) ();						// volná videopamì
typedef void (*D3AKTFOG) ();					// aktualizace mlhy
typedef void (*D3FOGON) (BOOL on);				// zapnutí mlhy
typedef void (*D3AKTSTATE) ();					// aktualizace stavu
typedef void (*D3AKTBLEND) (D3DFITEM* item);	// aktualizace blending operací objektu
typedef void (*D3AKTALPHA) (D3DFITEM* item);	// aktualizace referenèní úrovnì alfa
typedef void (*D3AKTPALETTE) (HWND wnd);		// aktualizace palet
typedef void (*D3SETCOLOROP) (int op, int arg1, int arg2, int stage); // nastavení operace barev
typedef void (*D3SETALPHAOP) (int op, int arg1, int arg2, int stage); // nastavení operace alfa
typedef void (*D3SETADDRESS) (int adru, int adrv, int stage); // nastavení adresování textur
//typedef void (*D3SETBORDER) (D3DFITEM* item, int stage); // nastavení barvy okolí textur
typedef void (*D3SETBIAS) (float bias, int stage);	// nastavení zjemnìní vzdálených textur
typedef void (*D3SETTFACTOR) (D3DFITEM* item);	// nastavení barvy faktoru textur
typedef void (*D3SETZFUNC)(int func);			// nastavení Z funkce (1 až 8, v kódu DirectX, 0=vypnuto)
typedef void (*D3SETZWRITE)(BOOL write);		// nastavení Z zápisu
typedef void (*D3SENABLE) (BOOL enable);		// povolení stencil operací
typedef void (*D3SETSOP) (int fail, int zfail, int zpass); // nastavení stencil operací
typedef void (*D3SETSFUNC) (int func, int ref, DWORD mask); // nastavení stencil testu
typedef void (*D3SETSMASK) (DWORD mask);		// nastavení zápisové masky do stencil bufferu
typedef void (*D3SHADOW) (D3DFITEM* item);		// vyrenderování stínu jednoho objektu
typedef void (*D3SHADOWS) ();					// vykreslení stínù

// funkce aktivního rozhraní
extern	D3STOP			pD3Stop;				// ukonèení funkce zaøízení
extern	D3SIZEVIEW		pD3SizeView;			// aktualizace viewportu po zmìnì velikosti
extern	D3MOVEVIEW		pD3MoveView;			// aktualizace viewportu po pøesunu
extern	D3DISP			pD3Disp;				// zobrazení renderovacího bufferu
extern	D3DISP			pD3Disp;				// zobrazení renderovacího bufferu
extern	D3CLEAR			pD3Clear;				// vymazání renderovací plochy
extern	D3AKTPROJ		pD3AktProj;				// aktualizace projekèní matice
extern	D3AKTVIEW		pD3AktView;				// aktualizace pohledové matice
extern	D3BEGIN			pD3Begin;				// zahájení renderování scény
extern	D3END			pD3End;					// ukonèení renderování scény
extern	D3RENDER		pD3Render;				// vyrenderování jednoho objektu
extern	D3RESET			pD3Reset;				// resetování objektu pøed ukonèením ovladaèe
extern	D3SRESET		pD3SReset;				// resetování stínu objektu pøed ukonèením ovladaèe
extern	D3AKTWIREFRAME	pD3AktWireframe;		// aktualizovat pøepínaè vyplòování ploch
extern	D3AKTLIGHTON	pD3AktLightOn;			// aktualizovat pøepínaè osvìtlení
extern	D3AKTSHADES		pD3AktShades;			// aktualizovat pøepínaè plynulého stínování
extern	D3AKTCULLING	pD3AktCulling;			// aktualizovat pøepínaè odstraòování ploch
extern	D3AKTMAGFILTER	pD3AktMagFilter;		// aktualizovat pøepínaè filtrace zvìtšených textur
extern	D3AKTMINMIPFILTER pD3AktMinMipFilter;	// aktualizovat pøepínaè filtrace zmenšených a vzdalených textur
extern	D3AKTLIGHT		pD3AktLight;			// aktualizace položky svìtla
extern	D3SETMATERIAL	pD3SetMaterial;			// nastavení materiálu
extern	D3TEXTURERES	pD3TextureRes;			// resetování položky textury pøed ukonèením ovladaèe
extern	D3TEXTURESET	pD3TextureSet;			// nastavení a zapnutí textury
extern	D3TEXTUREON		pD3TextureOn;			// zapnutí pùvodní textury
extern	D3TEXTUREOFF	pD3TextureOff;			// vypnutí textury
extern	D3MATRESET		pD3MatReset;			// resetování materiálu
extern	D3LIGHTRESET	pD3LightReset;			// resetování svìtla
extern	D3AKTAMBIENT	pD3AktAmbient;			// aktualizace ambient osvìtlení
extern	D3FREE			pD3Free;				// volná videopamì
extern	D3AKTFOG		pD3AktFog;				// aktualizace mlhy
extern	D3FOGON			pD3FogOn;				// zapnutí mlhy
extern	D3AKTSTATE		pD3AktState;			// aktualizace stavu
extern	D3AKTBLEND		pD3AktBlend;			// aktualizace blending operací objektu
extern	D3AKTALPHA		pD3AktAlfa;				// aktualizace referenèní úrovnì alfa
extern	D3AKTPALETTE	pD3AktPalette;			// aktualizace palet
extern	D3SETCOLOROP	pD3SetColorOp;			// nastavení operace barev
extern	D3SETALPHAOP	pD3SetAlphaOp;			// nastavení operace alfa
extern	D3SETADDRESS	pD3SetAddress;			// nastavení adresování textur
//extern	D3SETBORDER		pD3SetBorder;			// nastavení barvy okolí textur
extern	D3SETBIAS		pD3SetBias;				// nastavení zjemnìní vzdálených textur
extern	D3SETTFACTOR	pD3SetTFactor;			// nastavení barvy faktoru textur
extern	D3SETZFUNC		pD3SetZFunc;			// nastavení Z funkce (1 až 8, v kódu DirectX)
extern	D3SETZWRITE		pD3SetZWrite;			// nastavení hloubkového zápisu
extern	D3SENABLE		pD3SEnable;				// povolení stencil operací
extern	D3SETSOP		pD3SetSOp;				// nastavení stencil operací
extern	D3SETSFUNC		pD3SetSFunc;			// nastavení stencil testu
extern	D3SETSMASK		pD3SetSMask;			// nastavení zápisové masky do stencil bufferu
extern	D3SHADOW		pD3Shadow;				// vyrenderování stínu jednoho objektu
extern	D3SHADOWS		pD3Shadows;				// vykreslení stínù


#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------
