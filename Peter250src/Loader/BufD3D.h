
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
// struktura polo�ky textury

// Do referenc� se zapo��t�v� i reference z obr�zku. Polo�ka je proto zru�ena
// z bufferu a� ve chv�li zru�en� obr�zku, potom a� je ref=0

typedef struct D3DTITEM_
{
	int					Ref;		// po�et referenc� na texturu (0=polo�ka je voln�)
	CPicture			Pic;		// obr�zek textury
	BOOL				Mips;		// aktu�ln� textura vytvo�ena s mipmaps
	int					MipMaps;	// max. po�et �rovn� mipmaps (0=neomezeno)
	void*				Data1;		// data textury pro rozhran� 1 (NULL=nen�)
	void*				Data2;		// data textury pro rozhran� 2 (NULL=nen�)
} D3DTITEM;

////////////////////////////////////////////////////////////////////
// data bufferu textur

extern	int				D3DT_Num;	// po�et polo�ek v bufferu textur
extern	int				D3DT_Max;	// velikost bufferu textur (polo�ek)
extern	D3DTITEM*		D3DT_Data;	// buffer textur

extern	int				D3DT_Active[MAX_STAGES];	// aktivn� nastaven� textura (i je-li vypnut�)
extern	bool			D3DT_On[MAX_STAGES];	// textura je zapnuta

struct D3DFITEM_;

//extern	D3DFITEM_*		D3D_Rend[3*MAX_ZBIAS]; // polo�ky k renderov�n� (b�n�/blending/2D)

////////////////////////////////////////////////////////////////////
// p�id�n� textury do seznamu textur, vrac� index textury

int D3DT_Add(CPicture pic);

////////////////////////////////////////////////////////////////////
// uvoln�n� textury (kontroluje platnost indexu)

void D3DT_Del(int index);

////////////////////////////////////////////////////////////////////
// kontrola platnosti polo�ky

inline BOOL D3DT_IsValid(const int index)
{ 
	return (((DWORD)index < (DWORD)D3DT_Num) &&
				(D3DT_Data[index].Ref > 0)); 
};

////////////////////////////////////////////////////////////////////
// poskytnut� adresy polo�ky (bez kontroly indexu)

inline D3DTITEM* D3DT_Get(const int index)
{ 
	ASSERT(D3DT_IsValid(index));
	return &(D3DT_Data[index]);
}

////////////////////////////////////////////////////////////////////
// resetov�n� cachov�n� bufferu (p�ed ukon�en�m ovlada�e)

void D3DT_Reset();


////////////////////////////////////////////////////////////////////
// m�kk� resetov�n� cachov�n� bufferu (p�ed resetov�n�m ovlada�e p�i zm�n� viewportu)
// - v sou�asnosti nen� pot�eba ��dn� obsluha

inline void D3DT_SoftReset() {}

////////////////////////////////////////////////////////////////////
// nastaven� aktivn� textury (-1 = vypnuta)

void D3DT_Akt(int index, int stage);


/***************************************************************************\
*																			*
*								D3D buffer - materi�l						*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// struktura polo�ky materi�lu

typedef struct D3DMITEM_
{
	D3DMATERIAL8		Material;	// materi�l objektu
	void*				Data1;		// data objektu 1 (NULL=nen�)
	void*				Data2;		// data objektu 2 (NULL=nen�)
} D3DMITEM;

/////////////////////////////////////////////////////////////////////////////
// data bufferu materi�l� (polo�ka 0 = implicitn� materi�l)

extern	int				D3DM_Num;		// po�et polo�ek v bufferu materi�l�
extern	int				D3DM_Max;		// velikost bufferu materi�l� (polo�ek)	
extern	int				D3DM_Akt;		// aktivn� nastaven� materi�l
extern	D3DMITEM*		D3DM_Data;		// buffer materi�l�

////////////////////////////////////////////////////////////////////
// p�id�n� materi�lu do bufferu materi�l� (vrac� index materi�lu)
// - pokud materi�l ji� existuje, vrac� index existuj�c�ho materi�lu

int D3DM_Add(D3DMATERIAL8* mat);

////////////////////////////////////////////////////////////////////
// nastaven� materi�lu jako aktivn� (bez kontroly indexu)

void D3DM_Set(const int index);

////////////////////////////////////////////////////////////////////
// resetov�n� cachov�n� bufferu (p�ed ukon�en�m ovlada�e)

void D3DM_Reset();


////////////////////////////////////////////////////////////////////
// m�kk� resetov�n� cachov�n� bufferu (p�ed resetov�n�m ovlada�e p�i zm�n� viewportu)

inline void D3DM_SoftReset() { D3DM_Reset(); }


////////////////////////////////////////////////////////////////////
// kontrola platnosti polo�ky

inline BOOL D3DM_IsValid(const int index)
{ 
	return ((DWORD)index < (DWORD)D3DM_Num);
};


////////////////////////////////////////////////////////////////////
// poskytnut� adresy polo�ky (bez kontroly indexu)

inline D3DMITEM* D3DM_Get(const int index)
{ 
	ASSERT(D3DM_IsValid(index));
	return &(D3DM_Data[index]);
}

/***************************************************************************\
*																			*
*								D3D buffer - sv�tla							*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// struktura polo�ky sv�tla

typedef struct D3DLITEM_
{
	bool				IsUsed;		// sv�tlo je pou�it�
	bool				Akt;		// je nutn� aktualizovat sv�tlo
									// (plat� i pro zru�en� sv�tlo)
	bool				Enable;		// sv�tlo je zapnuto
	D3DLIGHT8			Light;		// popisova� sv�tla
	int					Frame;		// index r�mu s odkazem na sv�tlo
	DWORD				Diffuse;	// difusn� (a specular) barva zadan� u�ivatelem
	double				Range;		// dosah zadan� u�ivatelem
	double				Intens;		// intenzita sv�tla zadan� u�ivatelem (1=normal)
	double				Attenuation1; // line�rn� zeslaben� zadan� u�ivatelem
	double				Attenuation2; // kvadratick� zeslaben� zadan� u�ivatelem
	double				Theta;		// �hel sv�teln�ho ku�ele (vnit�n� ku�el)
	double				Phi;		// �hel polost�nu (vn�j�� ku�el)
	void*				Data1;		// data objektu 1 (NULL=nen�)
	void*				Data2;		// data objektu 2 (NULL=nen�)
} D3DLITEM;

////////////////////////////////////////////////////////////////////
// data bufferu sv�tel

extern	int				D3DL_Num;	// po�et polo�ek v bufferu sv�tel (v�etn� voln�ch)
extern	int				D3DL_Max;	// velikost bufferu sv�tel (polo�ek)	
extern	D3DLITEM*		D3DL_Data;	// buffer sv�tel

////////////////////////////////////////////////////////////////////
// vytvo�en� nov�ho sv�tla (vrac� index sv�tla)

int D3DL_New();

////////////////////////////////////////////////////////////////////
// aktualizace sv�tel

void D3DL_AktLight();

////////////////////////////////////////////////////////////////////
// zru�en� sv�tla (kontroluje platnost indexu)

void D3DL_Del(int index);

////////////////////////////////////////////////////////////////////
// resetov�n� bufferu sv�tel

void D3DL_Reset();

////////////////////////////////////////////////////////////////////
// m�kk� resetov�n� cachov�n� bufferu (p�ed resetov�n�m ovlada�e p�i zm�n� viewportu)

inline void D3DL_SoftReset() { D3DL_Reset(); }

////////////////////////////////////////////////////////////////////
// kontrola platnosti polo�ky

inline BOOL D3DL_IsValid(const int index)
{ 
	return (((DWORD)index < (DWORD)D3DL_Num) && 
		(D3DL_Data[index].IsUsed));
};

////////////////////////////////////////////////////////////////////
// poskytnut� adresy polo�ky (bez kontroly indexu)

inline D3DLITEM* D3DL_Get(const int index)
{ 
	ASSERT(D3DL_IsValid(index));
	return &(D3DL_Data[index]);
}


/***************************************************************************\
*																			*
*								D3D buffer - r�my							*
*																			*
\***************************************************************************/

// omezen� po�tu vrchol�
#define MAXVERTICES	1024	// maxim�ln� po�et vrchol� pro jeden objekt

// vzor jednotkov� matice
extern	D3DMATRIX	Matrix1Vzor;

// typy r�m�
typedef enum _D3DFTYPE
{
	D3DFTYPE_GROUP = 0,		// pr�zdn� r�m (nebo sc�na, index 0)
	D3DFTYPE_CAMERA,		// kamera (index 1)
	D3DFTYPE_OBJECT,		// 3D objekt (mesh, ter�n)
	D3DFTYPE_DECAL,			// 2D objekt
	D3DFTYPE_PICTURE,		// 2D obr�zek (p�ipojen ke kame�e, index 1)
	D3DFTYPE_LIGHT,			// sv�tlo
	D3DFTYPE_SECTOR,		// sektor objektu rozd�len�ho na ��sti
	D3DFTYPE_LENSFLARE,		// �o�kov� odlesk (p�ipojen ke kame�e, index 1)
} D3DFTYPE;

// typy po�ad� rotac�
typedef enum _D3DORDER
{
	D3DFORDER_XYZ = 0,		// XYZ
	D3DFORDER_XZY = 1,		// XZY
	D3DFORDER_YXZ = 2,		// YXZ
	D3DFORDER_YZX = 3,		// YZX
	D3DFORDER_ZXY = 4,		// ZXY
	D3DFORDER_ZYX = 5		// ZYX
} D3DORDER;

// zdroj materi�lu
typedef enum _MATSOURCE
{
	MATSOURCE_OBJECT = 0,	// vnit�n� materi�l objektu (=implicitn�)
	MATSOURCE_PARENT,		// materi�l zd�d�n od rodi�e
	MATSOURCE_FRAME			// implicitn� materi�l tohoto r�mu
} MATSOURCE;

// typy pr�hlednosti (nastaveno pro zdroj nebo c�l blending operace)
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
	BLEND_SATURATE		= 10,	// men�� ze slo�ek SRC/DST
	BLEND_INVALID		= -1,	// neplatn� typ operace
} BLENDTYPE;

extern	int	BlendTabD3D[11];	// tabulka blending operac� pro Direct3D
extern	int	BlendTabOGL[11];	// tabulka blending operac� pro OpenGL

extern	BLENDTYPE	D3DSrcBlend; // aktu�ln� nastaven� blending operace zdroje
extern	BLENDTYPE	D3DDstBlend; // aktu�ln� nastaven� blending operace c�le
extern	int			D3DAlphaRef;// aktu�ln� nastaven� alfa �rovn�

extern	int			D3DColorOp[MAX_STAGES];		// aktu�ln� nastaven� operac� barev
extern	int			D3DColorArg1[MAX_STAGES];	// aktu�ln� nastaven� argumentu 1 operac� barev
extern	int			D3DColorArg2[MAX_STAGES];	// aktu�ln� nastaven� argumentu 2 operac� barev
extern	int			D3DAlphaOp[MAX_STAGES];		// aktu�ln� nastaven� operac� barev
extern	int			D3DAlphaArg1[MAX_STAGES];	// aktu�ln� nastaven� argumentu 1 operac� barev
extern	int			D3DAlphaArg2[MAX_STAGES];	// aktu�ln� nastaven� argumentu 2 operac� barev
extern	int			D3DAddressU[MAX_STAGES];	// aktu�ln� adresov�n� textur U
extern	int			D3DAddressV[MAX_STAGES];	// aktu�ln� adresov�n� textur V
//extern	int			D3DBorder[MAX_STAGES];		// barva okol� textur
extern	float		D3DBias[MAX_STAGES];		// zjemn�n� vzd�len�ch textur
extern	DWORD		D3DTFactor;					// aktu�ln� nastaven� faktoru textur
extern	BOOL		D3DZWrite;					// aktu�ln� hodnota hloubkov�ho z�pisu
extern	int			D3DZTest;					// aktu�ln� hodnota hloubkov�ho testu

extern	int			D3DAktMat;					// prvn� polo�ka k aktualizaci matic (-1=nen�)

/////////////////////////////////////////////////////////////////////////////
// popisova� automatick�ho mapov�n� textur

typedef struct AUTOMAP_
{
	int				method;			// metoda mapov�n� (0=adresy, 1=vrcholy, 2=plo�ky)
	int				type;			// typ mapov�n� (0=plocha, 1=v�lec, 2=koule, 3=chrom, 4=�o�ka)
	double			ox;				// sou�adnice X po��tku mapov�n�
	double			oy;				// sou�adnice Y po��tku mapov�n�
	double			oz;				// sou�adnice Z po��tku mapov�n�
	double			rz;				// rotace podle osy Z
	double			rx;				// rotace podle osy X
	double			ry;				// rotace podle osy Y
	double			ou;				// po��tek textury horizont�ln�
	double			ov;				// po��tek textury vertik�ln�
	double			su;				// m���tko textury horizont�ln�
	double			sv;				// m���tko textury vertik�ln�

	D3DMATRIX		m;				// pou�it� transforma�n� matice
									//   testuj� se jen �leny 1 a� 3,
									//	 �leny _41 a� _43 uchov�vaj�
									//	 vektor k pozorovateli
									// Pro neplatn� mapov�n� (nap�. zm�na
									// parametr�) je _41 a� _43 = 0

} AUTOMAP;

/////////////////////////////////////////////////////////////////////////////
// struktura polo�ky r�mu D3D (v�po�ty se prov�d� v p�esnosti float)

typedef struct D3DFITEM_
{
	D3DFTYPE			Type;		// typ r�mu

	int					Index;		// index t�to polo�ky

// hiearchie (na za��tku je sc�na s indexem 0, nem� sousedy ani rodi�e)
	int					Parent;		// index rodi�e (-1 = nen�, 0 = sc�na)
	int					Child;		// prvn� potomek (-1 = nen�)
	int					Prev;		// p�edch�zej�c� soused stejn� �rovn� (-1 = nen�)
	int					Next;		// dal�� soused stejn� �rovn� (-1 = nen�)

// informace k renderov�n�
	int					Dist;		// druh� mocnina vzd�lenosti od kamery k polo�ce
	int					RendGroup;	// renderovac� skupina (0 a� 15)
	BOOL				ZWrite;		// zapisovat hloubkovou informaci
	int					ZTest;		// hloubkov� test (1 a� 8)

// rozd�len� objektu na ��sti
	BOOL				AktSector;	// aktualizovat generov�n� sektor�
	BOOL				UseSector;	// pou��vaj� se sektory nam�sto objektu
	BOOL				ParSector;	// mezi�schova parametru od rodi�e
	BOOL				res2;

// viditelnost r�mu
	bool				Visible;	// zapnuta viditelnost

// nastaven� objektu
	bool				Wireframe;	// vypl�ov�n� ploch
	bool				Lighton;	// osv�tlen�
	bool				Shades;		// plynul� st�nov�n�
	bool				MagFilter[MAX_STAGES];	// filtrace zv�t�en�ch textur
	bool				MinFilter[MAX_STAGES];	// filtrace zmen�en�ch textur
	bool				MipFilter[MAX_STAGES];	// filtrace vzd�len�ch textur
	short				Culling;	// zobrazen� plochy (0=ob�, 1=p�edn�, 2=zadn�)
	short				res3;

// sn�en� �rove� detail� LOD
	int					LODPrev;	// p�edch�zej�c� objekt pro LOD (-1=nen�)
	int					LODNext;	// dal�� objekt pro LOD (-1=nen�)
	double				LODMax;		// maxim�ln� vzd�lenost pro LOD
	float				LODMin2;	// minim�ln� vzd�lenost pro LOD ^2 (v�etn� t�to hodnoty)
	float				LODMax2;	// maxim�ln� vzd�lenost pro LOD ^2 (bez t�to hodnoty)

// multitexturov� operace
	int					Stages;		// aktivn� po�et stup��, pro kter� jsou vygenerov�ny buffery
	int					ColorOp[MAX_STAGES];	// operace barvy
	int					ColorArg1[MAX_STAGES];	// prvn� argument barvy
	int					ColorArg2[MAX_STAGES];	// druh� argument barvy
	int					AlphaOp[MAX_STAGES];	// operace alfa
	int					AlphaArg1[MAX_STAGES];	// prvn� argument alfa
	int					AlphaArg2[MAX_STAGES];	// druh� argument alfa
	int					AddressU[MAX_STAGES];	// m�d adresov�n� U
	int					AddressV[MAX_STAGES];	// m�d adresov�n� V
//	int					Border[MAX_STAGES];		// barva okol� textury v barv�ch Petra (BGR)
//	DWORD				BorderRGB[MAX_STAGES];	// barva okol� v m�du RGB
//	D3DCOLORVALUE		BorderF[MAX_STAGES];	// barva okol� v m�du float
	float				Bias[MAX_STAGES];		// zjemn�n� vzd�len�ch textur (0=b�n�)
	bool				UseText[MAX_STAGES];	// stupe� pou��v� p��kazy vyu��vaj�c� texturu
	DWORD				TFactor;				// faktor textur v barv�ch Petra (BGR)
	DWORD				TFactorRGB;				// faktor textur v barv�ch RGB
	D3DCOLORVALUE		TFactorF;				// faktor textur v barv�ch float

// hranice (koule)
	double				BoundR0;	// vlastn� netransformovan� polom�r hranice
	double				BoundR;		// netransformovan� polom�r s potomky
	double				ParScale;	// p�echodn� uschovan� m���tko od rodi�e
	bool				ParClip;	// uschovan� p��znak od rodi�e - testovat viditelnost
	bool				AktClip;	// polo�ku je nutn� testovat b�hem renderov�n�
	bool				AktBound;	// aktualizovat polom�r hranice s potomky
	bool				res4;

// sv�tlo
	int					Light;		// index sv�tla (-1 = nen�)

// transformace r�mu
	D3DORDER			Order;		// po�ad� rotac�
	double				ScaleX;		// zm�na m���tka ve sm�ru X
	double				ScaleY;		// zm�na m���tka ve sm�ru Y
	double				ScaleZ;		// zm�na m���tka ve sm�ru Z
	double				ScaleB;		// m���tko pro hranici (v�t�� z m���tek)
	double				RotateX;	// rotace podle osy X (normalizov�no 0 a� 2pi)
	double				SinX;		// sinus �hlu rotace podle X
	double				CosX;		// kosinus �hlu rotace podle X
	double				RotateY;	// rotace podle osy Y (normalizov�no 0 a� 2pi)
	double				RotateY2D;	// p��davn� nato�en� 2D objektu na kameru
	double				RotateX2D;	// uschovan� sou�adnice X relativn� ke kame�e
	double				RotateZ2D;	// uschovan� sou�adnice Z relativn� ke kame�e
	int					res8;

	double				SinY;		// sinus �hlu rotace podle Y
	double				CosY;		// kosinus �hlu rotace podle Y
	double				RotateZ;	// rotace podle osy Z (normalizov�no 0 a� 2pi)
	double				SinZ;		// sinus �hlu rotace podle Z
	double				CosZ;		// kosinus �hlu rotace podle Z
	double				TransX;		// posun ve sm�ru X
	double				TransY;		// posun ve sm�ru Y
	double				TransZ;		// posun ve sm�ru Z

	bool				IsRotX;		// je rotace podle osy X (tj. �hel != 0)
	bool				IsRotY;		// je rotace podle osy Y (tj. �hel != 0)
	bool				IsRotZ;		// je rotace podle osy Z (tj. �hel != 0)

	bool				AktOwn;		// aktualizovat vlastn� matici
	bool				AktWorld;	// aktualizovat v�slednou matici

	bool				AktMat;		// zahrnut do �et�zce aktualizace matic
	short				res5;

	int					AktMatPrev;	// p�edchoz� polo�ka pro aktualizaci matic (-1=je prvn�)
	int					AktMatNext;	// n�sleduj�c� polo�ka pro aktualizaci matic (-1=je posledn�)

	D3DMATRIX			MatrixOwn;	// vlastn� transforma�n� matice
	D3DMATRIX			MatrixWorld;// v�sledn� transforma�n� matice (+matice rodi�e)

// automatick� mapov�n� textur
	AUTOMAP*			AutoMap;	// automatick� mapov�n� textur (NULL=nen�)

// parametry ter�nu
	bool				IsTerrain;	// objekt je ter�n
	bool				res6;
	short				res7;
	int					TerWidth;	// ���ka ter�nu (pol��ek)
	int					TerHeight;	// v��ka ter�nu (pol��ek)
	float*				TerMap;		// mapa ter�nu ((width+1)*(height+1) hodnot 0 a� 1)

// klonov�n� (pro 1 objekt je ClonePrev = CloneNext = Index)
// u klon� jsou sd�len� tyto buffery:

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

	int					ClonePrev;	// p�edch�zej�c� klon (nen� klon: ClonePrev == Index)
	int					CloneNext;	// dal�� klon (nen� klon: CloneNext == Index)

// vzor objektu
	int					VertNum;	// po�et vrchol� v seznamu vrchol�, norm�l, barev a sou�adnic textur
	D3DVECTOR*			Vertex;		// seznam vektor� vrchol�
	D3DVECTOR*			Normal;		// seznam vektor� norm�l
	DWORD				AutoCol;	// aktu�ln� automatick� barva vrchol� (MAXDWORD = nen�)
	DWORD*				VertCol;	// seznam barev vrchol� RGBA (NULL=nen�)
	D3DCOLORVALUE*		VertColF;	// seznam barev vrchol� FLOAT (NULL=nen�)
	float*				TextUV[MAX_STAGES];	// sou�adnice U a V textur (NULL=nejsou)
	int					FaceNum;	// po�et plo�ek v seznamu plo�ek
	int*				Face;		// seznam plo�ek (ka�d� 3 vrcholy)
	WORD*				FaceW;		// seznam plo�ek ve form�tu WORD (jen pokud to vy�aduje driver)

// implicitn� povrch (barvy (int) jsou ve form�tru Petra, tj. BGR)
	MATSOURCE			MatSource;	// pou�it� zdroj materi�lu
	DWORD				Diffuse;	// difusn� barva materi�lu zadan� u�ivatelem
	DWORD				Ambient;	// ambient barva zadan� u�ivatelem (MAXDWORD=jako difusn�)
	DWORD				Emissive;	// emisivn� barva materi�lu zadan� u�ivatelem
	DWORD				Specular;	// barva odlesku materi�lu zadan� u�ivatelem
	double				Power;		// mocnina barvy odlesku zadan� u�ivatelem (0 a� 128)
	int					Material;	// index materi�lu (0 = implicitn�)
	int					ParMat;		// index materi�lu rodi�e
									// (pou��v� se pouze k p�echodn� �schov�
									// materi�lu rodi�e b�hem renderov�n�)
	DWORD				ParDif;		// difusn� barva materi�lu rodi�e (pouze k �schov�)

// morfov�n�
	int					MorphMax;	// po�et stup�� morfov�n� (0=nen�)
	double				MorphStage;	// aktu�ln� stupe� morfov�n� (0 a� MorphMax)
	D3DVECTOR**			MorphVert;	// buffery vrchol�
	D3DVECTOR**			MorphNorm;	// buffery norm�l
	float**				MorphUV;	// buffery sou�adnic textur (pro ka�d� stupe� MAX_STAGES buffer�)

// textura
	int					Texture[MAX_STAGES]; // index textury (-1 = nen�)
	int					ParText;	// index textury 0 rodi�e
									// (pou��v� se pouze k p�echodn� �schov�
									// textury rodi�e b�hem renderov�n�)

// pr�hlednost
	bool				IsBlend;	// je zapnut alpha blending (SRC<>1 nebo DST<>0)
	bool				IsAlphaRef;	// je zapnuto alfa porovn�v�n�
	bool				BlendRend;	// vy�aduje dodate�n� pr�chod pro alfa blending
	BLENDTYPE			SrcBlend;	// blending operace pro zdroj
	BLENDTYPE			DstBlend;	// blending operace pro c�l
	int					SrcDstBlend; // k�d blending operace v k�du u�ivatele = SRC + 10*DST
									// (1=normal, 11=ohe�, 20=sklo, 54=p�ekryv)
	double				AlphaRef;	// referen�n� �rove� alfa kan�lu (0=normal)
	int					AlphaRef2;	// referen�n� �rove� ve tvaru BYTE (0=normal)

// st�n (pro p�egenerov�n� buffer� sta�� zru�it jen "ShadeVert")
	double				ShadowRange; // dosah st�nu
	bool				IsShadow;	// je zapnut st�n
	bool				ShadowParent; // odvozovat vektor dopadu st�nu od rodi�e
	D3DVECTOR			ShadowPoint; // posledn� vektor dopadu st�nu ([0,0,0]=neplatn�)

	int					ShadeVertN;	// po�et vrchol�
	D3DVECTOR*			ShadeVert;	// buffer vrchol� (vypu�t�n� zdvojen� vrcholy)
	int*				ShadeFace;	// buffer plo�ek (ka�d� plo�ka 3 indexy vrchol�)
	D3DVECTOR*			ShadeNorm;	// buffer norm�l plo�ek
	// hrany jsou kolem plo�ky orientov�ny proti sm�ru hodinov�ch ru�i�ek (=kladn� sm�r pro v�j��)
	// vrcholy hran: 0: v1-v3, 1: v3-v2, 2: v2-v1
	int*				ShadeEdgeV;	// vrcholy hran plo�ek (ka�d� plo�ka 3 hrany po 2 vrcholech)
	int*				ShadeEdgeN;	// protich�dn� hrana sousedn� plo�ky (ka�d� plo�ka 3 indexy hran, -1=nen�)
	int*				ShadeNext;	// navazuj�c� hrana prvn� vpravo z 2. vrcholu (ka�d� plo�ka 3 indexy)
	int*				ShadeLeft;	// dal�� hrana z vrcholu 1 sm�rem doleva (ka�d� plo�ka 3 indexy)

	char*				ShadeEdgeS;	// stav hrany (ka�d� plo�ka 3 hrany)
									//   bit 0: plo�ka vlevo osv�cen�
									//   bit 1: plo�ka vpravo osv�cen�

	int					ShadowFanN;	// po�et obrys� v bufferu (po�et "v�j���")
	int					ShadowFan0;	// d�lka bufferu st�nu celkem (=po�et vrchol�)
	int*				ShadowFan;	// po�ty vrchol� pro jednotliv� obrysy ("v�j��e")
	D3DVECTOR*			ShadowVolume; // buffer st�nu (v�j��e, 1 vrchol po��tek, zbytek vrcholy v�j��e)

// datov� buffery ovlada��
	void*				Data1;		// datov� buffer 1 (vrcholy), NULL=nen�
	void*				Data2;		// datov� buffer 2 (indexy), NULL=nen�
	void*				Data3;		// datov� buffer pro st�ny, NULL=nen�

} D3DFITEM;


////////////////////////////////////////////////////////////////////
// popisova�e bufferu r�m�

extern	D3DFITEM*	D3DF_Data;		// ukazatel na data
extern	bool*		D3DF_Valid;		// p��znaky platnosti polo�ek
extern	int			D3DF_Max;		// velikost bufferu (polo�ek)
extern	int			D3DF_Next;		// p��t� voln� polo�ka (-1 = nen�)

////////////////////////////////////////////////////////////////////
// seznam skupin renderov�n�

extern	BOOL		RendSort[MAX_RENDGROUP];	// p��znaky t��d�n� skupin
extern	int			RendNum[MAX_RENDGROUP];		// po�ty polo�ek v bufferech
extern	D3DFITEM**	RendItems[MAX_RENDGROUP];	// seznam polo�ek k renderov�n�
extern	int			RendMax;					// velikost buffer� polo�ek

////////////////////////////////////////////////////////////////////
// vynulov�n� matice

inline void Matrix0(D3DMATRIX* m)
{
	MemFill(m, sizeof(D3DMATRIX), 0);
}

////////////////////////////////////////////////////////////////////
// vytvo�en� jednotkov� matice

inline void Matrix1(D3DMATRIX* m)
{
	MemCopy(m, &Matrix1Vzor, sizeof(D3DMATRIX));
}

////////////////////////////////////////////////////////////////////
// aktualizace transforma�n�ch matic v�ech objekt�
// (prov�d� se p�i sn�en� p�esnosti FPU!)

void D3DF_AktMatrix();

////////////////////////////////////////////////////////////////////
// transpozice matice (zdrojov� a c�lov� matice m��e b�t stejn�)

void _fastcall MatrixTrans(D3DMATRIX* dst, D3DMATRIX* src);

////////////////////////////////////////////////////////////////////
// vyn�soben� dvou matic (m = m1 * m2), v�sledn� matice nesm� b�t vstupn�!

void _fastcall MatrixMul(D3DMATRIX* m, const D3DMATRIX* m1, const D3DMATRIX* m2);

////////////////////////////////////////////////////////////////////
// inverze matice (zdrojov� a c�lov� matice m��e b�t stejn�)

void MatrixInv(D3DMATRIX* dst, D3DMATRIX* src);

////////////////////////////////////////////////////////////////////
// vyn�soben� vektoru matic� (prov�d� se p�i sn�en� p�esnosti FPU)
// (zdrojov� i c�lov� vektor mohou b�t shodn�)

void VecXMat(D3DVECTOR* dst, D3DVECTOR* src, D3DMATRIX* mat);


////////////////////////////////////////////////////////////////////
// nastaven� transformac� podle transforma�n� matice

void D3DF_SetTransMatrix(int index, D3DMATRIX* m);

////////////////////////////////////////////////////////////////////
// p�ipojen� do �et�zce aktualizace matic (kontroluje, zda je p�ipojen)

void _fastcall D3DF_AktMatLink(D3DFITEM* item);

////////////////////////////////////////////////////////////////////
// odpojen� z �et�zce aktualizace matic (kontroluje, zda je p�ipojen)

void _fastcall D3DF_AktMatUnlink(D3DFITEM* item);

////////////////////////////////////////////////////////////////////
// vytvo�en� nov� polo�ky bufferu (vrac� index polo�ky)

int D3DF_New();

////////////////////////////////////////////////////////////////////
// p��prava polo�ky k modifikaci (odpojen� od klon�)

void D3DF_Modi(int index);

////////////////////////////////////////////////////////////////////
// kopie polo�ky s potomky (vrac� index nov� polo�ky)

int D3DF_Copy(int index);

////////////////////////////////////////////////////////////////////
// zru�en� polo�ky (kontroluje platnost indexu)

void _fastcall D3DF_Del(const int index);

////////////////////////////////////////////////////////////////////
// resetov�n� cachov�n� bufferu (p�ed ukon�en�m ovlada�e)

void D3DF_Reset();

////////////////////////////////////////////////////////////////////
// m�kk� resetov�n� cachov�n� bufferu (p�ed resetov�n�m ovlada�e p�i zm�n� viewportu)
// - v sou�asnosti nen� pot�eba ��dn� obsluha

inline void D3DF_SoftReset() {}

////////////////////////////////////////////////////////////////////
// zru�en� v�ech polo�ek v bufferu (ukl�d�n� za�ne op�t po �ad�)

void D3DF_DelAll();

////////////////////////////////////////////////////////////////////
// zru�en� buffer� morfov�n�

void D3DF_MorphReset(D3DFITEM* item);

/////////////////////////////////////////////////////////////////////////////
// aktualizace m���tka pro hranici

void AktScaleB(D3DFITEM* item);

////////////////////////////////////////////////////////////////////
// inicializace mana�eru vrchol� (index polo�ky mus� b�t platn�!)

void D3DF_UsedInit(int inx);

////////////////////////////////////////////////////////////////////
// duplikace vrcholu (parametr = index vrcholu plo�ky), vrac� nov� index

int D3DF_UsedDuplik(int faceinx);

////////////////////////////////////////////////////////////////////
// nastaven� norm�ly vrcholu (pro dan� index vrcholu plo�ky)

void D3DF_UsedNormal(int faceinx, float nx, float ny, float nz);

////////////////////////////////////////////////////////////////////
// nastaven� barvy vrcholu (pro dan� index vrcholu plo�ky)

void D3DF_UsedColor(int faceinx, DWORD col, float r, float g, float b, float a);

void D3DF_UsedColorB(int faceinx, BYTE r, BYTE g, BYTE b, BYTE a);

////////////////////////////////////////////////////////////////////
// nastaven� adresy textury vrcholu (pro dan� index vrcholu plo�ky)

void D3DF_UsedTextUV(int faceinx, float u, float v, int stage);

////////////////////////////////////////////////////////////////////
// napojen� polo�ky na rodi�e (s odpojen�m od p�edch�zej�c�ho rodi�e)
// kontroluje platnost obou index�

void _fastcall D3DF_Link(const int parent, const int child);

////////////////////////////////////////////////////////////////////
// zmapov�n� vlastn� hranice (index mus� b�t platn�)

void D3DF_MapBound(int index);

////////////////////////////////////////////////////////////////////
// vytvo�en� objektu (vrac� index objektu)
// Po vytvo�en� objektu je nutn� nastavit norm�ly!!!

int D3DF_CreateMesh(int vertN, D3DVECTOR* vert, int faceN, int* face);

////////////////////////////////////////////////////////////////////
// nastaven� norm�l objektu (po�et norm�l mus� souhlasit s po�tem vrchol�!)

void D3DF_SetNormal(int index, D3DVECTOR* norm);

////////////////////////////////////////////////////////////////////
// korekce nov� vytvo�en�ho aktu�ln�ho objektu pro pravoruk� sou�adn� syst�m

void D3DF_KorRightHand();

////////////////////////////////////////////////////////////////////
// nastaven� barev vrchol� objektu (po�et mus� souhlasit s po�tem vrchol�!)

void D3DF_SetVertCol(int index, D3DCOLORVALUE* col);

////////////////////////////////////////////////////////////////////
// nastaven� mapov�n� textur (po�et mus� souhlasit s po�tem vrchol�!)

void D3DF_SetTextUV(int index, float* textuv, int stage);

////////////////////////////////////////////////////////////////////
// nastaven� adresov�n� a zalamov�n� textury

void D3DF_SetAddress(int index, int adru, int adrv, int stage);

////////////////////////////////////////////////////////////////////
// nastaven� blending m�du (1=normal, 11=ohe�, 20=sklo, 54=p�ekryv, 100=saturace)

void D3DF_SetBlend(int index, int blendmode);

////////////////////////////////////////////////////////////////////
// vygenerov�n� norm�l (kontroluje index)

void D3DF_AutoNormals(int index, double angle);

////////////////////////////////////////////////////////////////////
// renderov�n� objekt� (pro Direct3D mus� b�t BeginScene a EndScene!)

void D3DF_Render();

////////////////////////////////////////////////////////////////////
// renderov�n� objekt� s blending (pro Direct3D mus� b�t BeginScene a EndScene!)

void D3DF_RenderBlend();

////////////////////////////////////////////////////////////////////
// renderov�n� 2D obr�zk� (pro Direct3D mus� b�t BeginScene a EndScene!)

void D3DF_Render2D();

////////////////////////////////////////////////////////////////////
// kontrola platnosti polo�ky

inline BOOL D3DF_IsValid(const int index)
{ 
	return (((DWORD)index < (DWORD)D3DF_Max) &&
				D3DF_Valid[index]); 
};

inline BOOL IsValidID() { return D3DF_IsValid(D3D_ID); }

////////////////////////////////////////////////////////////////////
// poskytnut� adresy polo�ky (bez kontroly indexu)

inline D3DFITEM* D3DF_Get(const int index)
{ 
	ASSERT(D3DF_IsValid(index));
	return &(D3DF_Data[index]);
}

inline D3DFITEM* GetID() { return D3DF_Get(D3D_ID); }


// funkce aktivn�ho rozhran� - definice
typedef	void (*D3STOP) ();						// ukon�en� �unkce za��zen�
typedef	void (*D3SIZEVIEW) ();					// aktualizace viewportu po zm�n� velikosti
typedef	void (*D3MOVEVIEW) ();					// aktualizace viewportu po p�esunu
typedef	void (*D3DISP) ();						// zobrazen� renderovac�ho bufferu
typedef	void (*D3CLEAR) ();						// vymaz�n� renderovac� plochy
typedef	void (*D3AKTPROJ) ();					// aktualizace projek�n� matice
typedef	void (*D3AKTVIEW) ();					// aktualizace pohledov� matice
typedef	void (*D3BEGIN) ();						// zah�jen� renderov�n� sc�ny
typedef	void (*D3END) ();						// ukon�en� renderov�n� sc�ny
typedef void (*D3RENDER) (D3DFITEM* item);		// vyrenderov�n� jednoho objektu
typedef void (*D3RESET) (D3DFITEM* item);		// resetov�n� objektu p�ed ukon�en�m ovlada�e
typedef void (*D3SRESET) (D3DFITEM* item);		// resetov�n� st�nu objektu p�ed ukon�en�m ovlada�e
typedef void (*D3AKTWIREFRAME) ();				// aktualizovat p�ep�na� vypl�ov�n� ploch
typedef void (*D3AKTLIGHTON) ();				// aktualizovat p�ep�na� osv�tlen�
typedef void (*D3AKTSHADES) ();					// aktualizovat p�ep�na� plynul�ho st�nov�n�
typedef void (*D3AKTCULLING) ();				// aktualizovat p�ep�na� odstra�ov�n� ploch
typedef void (*D3AKTMAGFILTER) (int stage);		// aktualizovat p�ep�na� filtrace zv�t�en�ch textur
typedef void (*D3AKTMINMIPFILTER) (int stage);	// aktualizovat p�ep�na� filtrace zmen�en�ch a vzd�len�ch textur
typedef void (*D3AKTLIGHT) (int inx, D3DLITEM* item);	// aktualizace polo�ky sv�tla
typedef void (*D3SETMATERIAL) (D3DMITEM* item);	// nastaven� materi�lu
typedef void (*D3TEXTURERES) (D3DTITEM* item);	// resetov�n� polo�ky textury p�ed ukon�en�m ovlada�e
typedef void (*D3TEXTURESET) (D3DTITEM* item, int stage);	// nastaven� a zapnut� textury
typedef void (*D3TEXTUREON) (D3DTITEM* item, int stage);	// zapnut� p�vodn� textury
typedef void (*D3TEXTUREOFF) (int stage);				// vypnut� textury
typedef void (*D3MATRESET) (D3DMITEM* item);	// resetov�n� materi�lu
typedef void (*D3LIGHTRESET) (int inx, D3DLITEM* item);	// resetov�n� sv�tla
typedef void (*D3AKTAMBIENT) ();				// aktualizace ambient osv�tlen�
typedef int (*D3FREE) ();						// voln� videopam�
typedef void (*D3AKTFOG) ();					// aktualizace mlhy
typedef void (*D3FOGON) (BOOL on);				// zapnut� mlhy
typedef void (*D3AKTSTATE) ();					// aktualizace stavu
typedef void (*D3AKTBLEND) (D3DFITEM* item);	// aktualizace blending operac� objektu
typedef void (*D3AKTALPHA) (D3DFITEM* item);	// aktualizace referen�n� �rovn� alfa
typedef void (*D3AKTPALETTE) (HWND wnd);		// aktualizace palet
typedef void (*D3SETCOLOROP) (int op, int arg1, int arg2, int stage); // nastaven� operace barev
typedef void (*D3SETALPHAOP) (int op, int arg1, int arg2, int stage); // nastaven� operace alfa
typedef void (*D3SETADDRESS) (int adru, int adrv, int stage); // nastaven� adresov�n� textur
//typedef void (*D3SETBORDER) (D3DFITEM* item, int stage); // nastaven� barvy okol� textur
typedef void (*D3SETBIAS) (float bias, int stage);	// nastaven� zjemn�n� vzd�len�ch textur
typedef void (*D3SETTFACTOR) (D3DFITEM* item);	// nastaven� barvy faktoru textur
typedef void (*D3SETZFUNC)(int func);			// nastaven� Z funkce (1 a� 8, v k�du DirectX, 0=vypnuto)
typedef void (*D3SETZWRITE)(BOOL write);		// nastaven� Z z�pisu
typedef void (*D3SENABLE) (BOOL enable);		// povolen� stencil operac�
typedef void (*D3SETSOP) (int fail, int zfail, int zpass); // nastaven� stencil operac�
typedef void (*D3SETSFUNC) (int func, int ref, DWORD mask); // nastaven� stencil testu
typedef void (*D3SETSMASK) (DWORD mask);		// nastaven� z�pisov� masky do stencil bufferu
typedef void (*D3SHADOW) (D3DFITEM* item);		// vyrenderov�n� st�nu jednoho objektu
typedef void (*D3SHADOWS) ();					// vykreslen� st�n�

// funkce aktivn�ho rozhran�
extern	D3STOP			pD3Stop;				// ukon�en� funkce za��zen�
extern	D3SIZEVIEW		pD3SizeView;			// aktualizace viewportu po zm�n� velikosti
extern	D3MOVEVIEW		pD3MoveView;			// aktualizace viewportu po p�esunu
extern	D3DISP			pD3Disp;				// zobrazen� renderovac�ho bufferu
extern	D3DISP			pD3Disp;				// zobrazen� renderovac�ho bufferu
extern	D3CLEAR			pD3Clear;				// vymaz�n� renderovac� plochy
extern	D3AKTPROJ		pD3AktProj;				// aktualizace projek�n� matice
extern	D3AKTVIEW		pD3AktView;				// aktualizace pohledov� matice
extern	D3BEGIN			pD3Begin;				// zah�jen� renderov�n� sc�ny
extern	D3END			pD3End;					// ukon�en� renderov�n� sc�ny
extern	D3RENDER		pD3Render;				// vyrenderov�n� jednoho objektu
extern	D3RESET			pD3Reset;				// resetov�n� objektu p�ed ukon�en�m ovlada�e
extern	D3SRESET		pD3SReset;				// resetov�n� st�nu objektu p�ed ukon�en�m ovlada�e
extern	D3AKTWIREFRAME	pD3AktWireframe;		// aktualizovat p�ep�na� vypl�ov�n� ploch
extern	D3AKTLIGHTON	pD3AktLightOn;			// aktualizovat p�ep�na� osv�tlen�
extern	D3AKTSHADES		pD3AktShades;			// aktualizovat p�ep�na� plynul�ho st�nov�n�
extern	D3AKTCULLING	pD3AktCulling;			// aktualizovat p�ep�na� odstra�ov�n� ploch
extern	D3AKTMAGFILTER	pD3AktMagFilter;		// aktualizovat p�ep�na� filtrace zv�t�en�ch textur
extern	D3AKTMINMIPFILTER pD3AktMinMipFilter;	// aktualizovat p�ep�na� filtrace zmen�en�ch a vzdalen�ch textur
extern	D3AKTLIGHT		pD3AktLight;			// aktualizace polo�ky sv�tla
extern	D3SETMATERIAL	pD3SetMaterial;			// nastaven� materi�lu
extern	D3TEXTURERES	pD3TextureRes;			// resetov�n� polo�ky textury p�ed ukon�en�m ovlada�e
extern	D3TEXTURESET	pD3TextureSet;			// nastaven� a zapnut� textury
extern	D3TEXTUREON		pD3TextureOn;			// zapnut� p�vodn� textury
extern	D3TEXTUREOFF	pD3TextureOff;			// vypnut� textury
extern	D3MATRESET		pD3MatReset;			// resetov�n� materi�lu
extern	D3LIGHTRESET	pD3LightReset;			// resetov�n� sv�tla
extern	D3AKTAMBIENT	pD3AktAmbient;			// aktualizace ambient osv�tlen�
extern	D3FREE			pD3Free;				// voln� videopam�
extern	D3AKTFOG		pD3AktFog;				// aktualizace mlhy
extern	D3FOGON			pD3FogOn;				// zapnut� mlhy
extern	D3AKTSTATE		pD3AktState;			// aktualizace stavu
extern	D3AKTBLEND		pD3AktBlend;			// aktualizace blending operac� objektu
extern	D3AKTALPHA		pD3AktAlfa;				// aktualizace referen�n� �rovn� alfa
extern	D3AKTPALETTE	pD3AktPalette;			// aktualizace palet
extern	D3SETCOLOROP	pD3SetColorOp;			// nastaven� operace barev
extern	D3SETALPHAOP	pD3SetAlphaOp;			// nastaven� operace alfa
extern	D3SETADDRESS	pD3SetAddress;			// nastaven� adresov�n� textur
//extern	D3SETBORDER		pD3SetBorder;			// nastaven� barvy okol� textur
extern	D3SETBIAS		pD3SetBias;				// nastaven� zjemn�n� vzd�len�ch textur
extern	D3SETTFACTOR	pD3SetTFactor;			// nastaven� barvy faktoru textur
extern	D3SETZFUNC		pD3SetZFunc;			// nastaven� Z funkce (1 a� 8, v k�du DirectX)
extern	D3SETZWRITE		pD3SetZWrite;			// nastaven� hloubkov�ho z�pisu
extern	D3SENABLE		pD3SEnable;				// povolen� stencil operac�
extern	D3SETSOP		pD3SetSOp;				// nastaven� stencil operac�
extern	D3SETSFUNC		pD3SetSFunc;			// nastaven� stencil testu
extern	D3SETSMASK		pD3SetSMask;			// nastaven� z�pisov� masky do stencil bufferu
extern	D3SHADOW		pD3Shadow;				// vyrenderov�n� st�nu jednoho objektu
extern	D3SHADOWS		pD3Shadows;				// vykreslen� st�n�


#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------
