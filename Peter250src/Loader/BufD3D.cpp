
#define	DIRECT3D_VERSION 0x0800

#include "Main.h"

/***************************************************************************\
*																			*
*									D3D buffery								*
*																			*
\***************************************************************************/

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

////////////////////////////////////////////////////////////////////
// Matice:
// ------
// Pøi poøadí rotací X-Y-Z se násobí maticemi v tomto poøadí:
//
//   zmìna mìøítka:
//  sx    0    0    0
//   0   sy    0    0
//   0    0	  sz    0
//   0    0    0    1
//
//  rotace podle osy x:
//   1    0    0    0
//   0   cosx sinx  0
//   0  -sinx cosx  0
//   0    0    0    1
//
//  rotace podle osy y:
//  cosy  0  -siny  0
//   0    1    0    0
//  siny  0   cosy  0
//   0    0    0    1
//
//  rotace podle osy z:
//  cosz sinz  0    0
// -sinz cosz  0    0
//   0    0    1    0
//   0    0    0    1
//
//       posun:
//   1    0    0    0
//   0    1    0    0
//   0    0    1    0
//  tx   ty   tz    1
//
// Po vynásobení matic v uvedeném poøadí je výsledná transformaèní matice:
// ---------------------------------------------------------------------------
//        sx*cosy*cosz                  sx*cosy*sinz             -sx*siny    0
// sy*(sinx*siny*cosz-cosx*sinz) sy*(sinx*siny*sinz+cosx*cosz) sy*sinx*cosy  0
// sz*(cosx*siny*cosz+sinx*sinz) sz*(cosx*siny*sinz-sinx*cosz) sz*cosx*cosy  0
//              tx                           ty                     tz       1
// ---------------------------------------------------------------------------
//
// Známe-li výslednou transformaèní matici ve tvaru:
//  a11  a12  a13  a14
//  a21  a22  a23  a24
//  a31  a32  a33  a34
//  a41  a42  a43  a44
//
// potom lze zpìtnì vypoèítat pùvodní transformace:
//
//  tx = a41
//  ty = a42
//  tz = a43
//
//  sx = sqrt(a11*a11 + a12*a12 + a13*a13)
//  sy = sqrt(a21*a21 + a22*a22 + a23*a23)
//  sz = sqrt(a31*a31 + a32*a32 + a33*a33)
//
// pro a11<>0 nebo a12<>0 je:
//  uhel z = atan2(a12, a11)
//  uhel x = atan2(a23/sy, a33/sz);
//  uhel y = atan2(-a13, a12/sinz);
//
// pro a11=0, a12=0 a a13>=0 je:
//  uhel y = pi*3/2
//  uhel x = atan2(a32, a31)
//  uhel z = 0

// pro a11=0, a12=0 a a13<0 je:
//  uhel y = pi/2
//  uhel x = atan2(-a32, a31)
//  uhel z = 0

/***************************************************************************\
*																			*
*								D3D buffer - textury						*
*																			*
\***************************************************************************/

////////////////////////////////////////////////////////////////////
// Podporované formáty textur (pøíkazem CreateTexture):
//	D3DFMT_A8R8G8B8	(32 bitù s alpha)
//	D3DFMT_X8R8G8B8 (32 bitù)
//	D3DFMT_R5G6B5 (16 bitù)
//	D3DFMT_X1R5G5B5 (15 bitù)
//	D3DFMT_A1R5G5B5 (15 bitù s alpha)
//	D3DFMT_A4R4G4B4 (12 bitù s alpha)
//	D3DFMT_DXT1 (komprese DXT1)
//	D3DFMT_DXT2 (komprese DXT2)
//	D3DFMT_DXT3 (komprese DXT3)
//	D3DFMT_DXT4 (komprese DXT4)
//	D3DFMT_DXT5 (komprese DXT5)

////////////////////////////////////////////////////////////////////
// data bufferu textur

int				D3DT_Num = 0;		// poèet položek v bufferu textur
int				D3DT_Max = 0;		// velikost bufferu textur (položek)
D3DTITEM*		D3DT_Data = NULL;	// buffer textur

int				D3DT_Active[MAX_STAGES]; // aktivnì nastavená textura (i je-li vypnutá)
bool			D3DT_On[MAX_STAGES];	// textura je zapnuta

//D3DFITEM_*		D3D_Rend[3*MAX_ZBIAS]; // položky k renderování (bìžné/blending/2D)

////////////////////////////////////////////////////////////////////
// pøidání textury do seznamu textur, vrací index textury
// (nesmí to být prázdný obrázek !!!!)
// (!!!! v souèasnosti podporovaný pouze formát D3DFMT_A8R8G8B8 !!!!!);

int D3DT_Add(CPicture pic)
{
	ASSERT(pic.IsNotEmpty());

// test, zda obrázek je již v bufferu uložen
	PICTUREDATA* picdata = pic.Data();			// data obrázku
	int inx = picdata->TextInx;					// možný index položky
	if (inx >= 0)								// je to platný index?
	{
		ASSERT(D3DT_IsValid(inx));
		ASSERT(D3DT_Data[inx].Pic == pic);
		D3DT_Data[inx].Ref++;					// zvýšení èítaèe referencí na položku
		return inx;
	}
			
// nalezení volné položky
	inx = 0;
	D3DTITEM* item = D3DT_Data;

	for (; inx < D3DT_Num;)
	{
		if (item->Ref == 0) break;
		item++;
		inx++;
	}

// bude nová položka - zvýšení velikosti bufferu
	if (inx == D3DT_Num) D3DT_Num++;			// zvýšení poètu položek do bufferu

	if (inx == D3DT_Max)
	{
		D3DT_Max = 2*inx;						// nová velikost bufferu
		if (inx == 0) D3DT_Max = 64;			// minimální velikost bufferu
		MemBuf(D3DT_Data, D3DT_Max);			// zvýšení velikosti bufferu
	}
	item = D3DT_Data + inx;

// inicializace nové položky
	item->Ref = 2;								// poèet referencí na položku
	item->Pic.Init(picdata);					// uložení obrázku do položky
	item->Mips = FALSE;
	item->Data1 = NULL;							// nulování dat textury
	item->Data2 = NULL;
	item->MipMaps = 0;							// poèet mipmaps neomezen
	picdata->TextInx = inx;						// odkaz z obrázku do bufferu

//	MemFill(item->Rend, 3*MAX_ZBIAS*sizeof(D3DFITEM_*), 0);

// nastavení požadovaného zjemnìní (není-li to textura ze souboru)
	if (picdata->TextData == NULL)
	{
		picdata->TextSmooth = D3DSmooth;
	}

	return inx;
}

////////////////////////////////////////////////////////////////////
// uvolnìní textury (kontroluje platnost indexu)
// (obrázek textury pøi svém zrušení volá funkci pD3TextureRes(titem))

void D3DT_Del(int index)
{
	if (D3DT_IsValid(index))
	{
		D3DTITEM* item = D3DT_Data + index;
		item->Ref--;

		if ((item->Ref == 1) && (item->Pic.Data()->Refer == 1))
		{
			item->Pic.Term();
			item->Ref = 0;
		}
	}
}


////////////////////////////////////////////////////////////////////
// resetování cachování bufferu (pøed ukonèením ovladaèe)

void D3DT_Reset()
{
	for (int i = 0; i < MAX_STAGES; i++)
	{
		pD3TextureOff(i);
		D3DT_On[i] = false;
		D3DT_Active[i] = -1;
	}

	D3DTITEM* item = D3DT_Data;

	for (i = D3DT_Num; i > 0; i--)
	{
		pD3TextureRes(item);
		item->Mips = FALSE;
		item++;
	}

}

////////////////////////////////////////////////////////////////////
// nastavení aktivní textury (-1 = vypnuta)
//
// Pro starší verze DirectX se nastavením materiálu zruší aktivní textura,
// bude nastaveno D3DT_Activ na -1, D3DT_On zùstane nezmìnìno

void D3DT_Akt(int index, int stage)
{
// test, zda bude textura vypnuta
	if (!D3DT_IsValid(index))
	{
		if (D3DT_On[stage])
		{
			pD3TextureOff(stage);
			D3DT_On[stage] = false;
		}
		return;
	}

// adresa položky
	D3DTITEM* item = D3DT_Data + index;

// adresa obrázku
	PICTUREDATA* picdata = item->Pic.Data();

// zrušení textury, pokud byla vytvoøena bez mipmaps
	if (D3DMipFilterAkt[stage])
	{
		bool akt = ((picdata->TextData != NULL) && !picdata->TextIsMip);

		if (!item->Mips || akt)
		{
			if (index == D3DT_Active[stage])
			{
				if (D3DT_On[stage])
				{
					pD3TextureOff(stage);
					D3DT_On[stage] = false;
				}
				D3DT_Active[stage] = -1;
			}

			pD3TextureRes(item);

			item->Mips = TRUE;
		}

		if (akt)
		{
			MemFree(picdata->TextData);
			picdata->TextData = NULL;
			MemFree(picdata->TextDataRGBA);
			picdata->TextDataRGBA = NULL;
			MemFree(picdata->TextDataR8G8B8);
			picdata->TextDataR8G8B8 = NULL;
			MemFree(picdata->TextDataR5G6B5);
			picdata->TextDataR5G6B5 = NULL;
			MemFree(picdata->TextDataA1R5G5B5);
			picdata->TextDataA1R5G5B5 = NULL;
			MemFree(picdata->TextDataA4R4G4B4);
			picdata->TextDataA4R4G4B4 = NULL;
		}
	}

// inicializace textury
	item->Pic.InitTexture(stage);

	if (index == D3DT_Active[stage])
	{
		if (!D3DT_On[stage])
		{
			pD3TextureOn(item, stage);
		}
	}
	else
	{
		pD3TextureSet(item, stage);
		D3DT_Active[stage] = index;
	}
	D3DT_On[stage] = true;
}


/***************************************************************************\
*																			*
*								D3D buffer - materiál						*
*																			*
\***************************************************************************/

////////////////////////////////////////////////////////////////////
// data bufferu materiálù (položka 0 = implicitní materiál)

int				D3DM_Num = 0;		// poèet položek v bufferu materiálù
int				D3DM_Max = 0;		// velikost bufferu materiálù (položek)	
int				D3DM_Akt = -1;		// aktivnì nastavený materiál
D3DMITEM*		D3DM_Data = NULL;	// buffer materiálù

////////////////////////////////////////////////////////////////////
// pøidání materiálu do bufferu materiálù (vrací index materiálu)
// - pokud materiál již existuje, vrací index existujícího materiálu
// (POZOR - volá se bìhem inicializace, ještì pøed inicializací obsluhy 3D!)

int D3DM_Add(D3DMATERIAL8* mat)
{
// velikost materiálu musí být násobek 4 !!!
#pragma warning ( disable: 4127)				// hlášení - konstantní podmínka
	ASSERT(sizeof(D3DMATERIAL8) == 0x44);
#pragma warning ( default: 4127)				// hlášení - konstantní podmínka

// vyhledání již existujícího materiálu v bufferu
	int inx = 0;								// ukazatel indexu materiálu
	D3DMITEM* mat2 = D3DM_Data;				// ukazatel položek materiálu

	for (; inx < D3DM_Num; inx++)				// cyklus pøes všechny materiály
	{
		int* src = (int*)mat;					// adresa hledané položky
		int* dst = (int*)(&mat2->Material);		// adresa položky v bufferu
		int j = sizeof(D3DMATERIAL8)/4;			// velikost položky
		for (; j > 0; j--)
		{
			if (*dst != *src) break;
			src++;
			dst++;
		}
		if (j == 0) return inx;					// nalezena shodná položka

		mat2++;									// zvýšení ukazatele adresy položky
	}
		
// bude nová položka - zvýšení velikosti bufferu (nyní inx == D3DM_MatNum)
	if (inx == D3DM_Max)
	{
		D3DM_Max = 2*inx;						// nová velikost bufferu
		if (inx == 0) D3DM_Max = 64;			// minimální velikost bufferu
		MemBuf(D3DM_Data, D3DM_Max);				// zvýšení velikosti bufferu
	}
	D3DM_Num++;									// zvýšení poètu položek do bufferu

// uložení nové položky do bufferu
	MemCopy(&(D3DM_Data[inx].Material), mat, sizeof(D3DMATERIAL8));
	D3DM_Data[inx].Data1 = NULL;
	D3DM_Data[inx].Data2 = NULL;

	return inx;
}

////////////////////////////////////////////////////////////////////
// nastavení materiálu jako aktivní (bez kontroly indexu)
//
// Pro starší verze DirectX se nastavením materiálu zruší aktivní textura,
// bude nastaveno D3DT_Activ na -1, D3DT_On zùstane nezmìnìno

void D3DM_Set(const int index)
{
	ASSERT(D3DM_IsValid(index));

// kontrola, zda je materiál již nastaven
	if (index != D3DM_Akt)
	{
		D3DM_Akt = index;
		pD3SetMaterial(D3DM_Data + index);
	}
}

////////////////////////////////////////////////////////////////////
// resetování cachování bufferu (pøed ukonèením ovladaèe)

void D3DM_Reset()
{
	D3DM_Akt = -1;

	D3DMITEM* item = D3DM_Data;
	for (int i = D3DM_Num; i > 0; i--)
	{
		pD3MatReset(item);
		item++;
	}
}

/***************************************************************************\
*																			*
*								D3D buffer - svìtla							*
*																			*
\***************************************************************************/

////////////////////////////////////////////////////////////////////
// data bufferu svìtel

int				D3DL_Num = 0;		// poèet položek v bufferu svìtel (vèetnì volných)
int				D3DL_Max = 0;		// velikost bufferu svìtel (položek)	
D3DLITEM*		D3DL_Data = NULL;	// buffer svìtel

////////////////////////////////////////////////////////////////////
// vytvoøení nového svìtla (vrací index svìtla)
// (POZOR - volá se bìhem inicializace, ještì pøed inicializací obsluhy 3D!)

int D3DL_New()
{

// nalezení volné položky svìtla
	int index = 0;
	for (; index < D3DL_Num; index++)
	{
		if (!D3DL_Data[index].IsUsed) break;
	}

// vytvoøení nového svìtla, pokud nebyla nalezena volná položka
	if (index == D3DL_Num)
	{
		D3DL_Num++;
		if (D3DL_Num > D3DL_Max)
		{
			D3DL_Max = 2*D3DL_Max;
			if (D3DL_Max == 0) D3DL_Max = 16;
			MemBuf(D3DL_Data, D3DL_Max);
		}
	}

// inicializace položky svìtla
	D3DLITEM* item = D3DL_Data + index;
	item->IsUsed = true;

	D3DLIGHT8* l = &(item->Light);
	l->Type = D3DLIGHT_POINT;

	l->Diffuse.r = 1;
	l->Diffuse.g = 1;
	l->Diffuse.b = 1;
	l->Diffuse.a = 1;

	l->Specular.r = 1;
	l->Specular.g = 1;
	l->Specular.b = 1;
	l->Specular.a = 1;

	l->Ambient.r = 0;
	l->Ambient.g = 0;
	l->Ambient.b = 0;
	l->Ambient.a = 1;

	l->Position.x = 0;
	l->Position.y = 0;
	l->Position.z = 0;

	l->Direction.x = 0;
	l->Direction.y = 0;
	l->Direction.z = 1;

	l->Range = 100;
	l->Falloff = 1;
	l->Attenuation0 = 1;
	l->Attenuation1 = 0;
	l->Attenuation2 = 0;

	l->Theta = (float)uhel45;
	l->Phi = l->Theta;

	item->Akt = true;
	item->Enable = true;
	item->Frame = 0;
	item->Diffuse = 0xffffff;
	item->Range = 100;
//	item->Attenuation0 = 1;
	item->Intens = 1;
	item->Attenuation1 = 0;
	item->Attenuation2 = 0;
	item->Theta = uhel45;
	item->Phi = 0;
	item->Data1 = NULL;
	item->Data2 = NULL;

	return index;
}

////////////////////////////////////////////////////////////////////
// zrušení svìtla (kontroluje platnost indexu)

void D3DL_Del(int index)
{
	if (D3DL_IsValid(index)) 
	{
		D3DLITEM* item = D3DL_Data + index;
		if (item->Enable)
		{
			item->Enable = false;
			item->Akt = true;
		}
		item->IsUsed = false;
	}
}

////////////////////////////////////////////////////////////////////
// aktualizace svìtel

void D3DL_AktLight()
{
// cyklus pøes všechna svìtla
	D3DLITEM* item = D3DL_Data;
	for (int i = 0; i < D3DL_Num; i++)
	{

// pøíprava pozice a smìru
		D3DFITEM* frame = D3DF_Get(item->Frame);

		D3DLIGHT8* light = &(item->Light);
	// transformaèní matice se vynásobí vektorem poèátku, tj. (0, 0, 0)
		light->Position.x = frame->MatrixWorld._41;
		light->Position.y = frame->MatrixWorld._42;
		light->Position.z = frame->MatrixWorld._43;
	// transformaèní matice se vynásobí vektorem smìru, tj. (0, 0, 1)
	// (a odeète se vektor nového poèátku - pøíslušné èleny se vyruší)
		light->Direction.x = frame->MatrixWorld._31;
		light->Direction.y = frame->MatrixWorld._32;
		light->Direction.z = frame->MatrixWorld._33;

// aktualizace svìtla
		pD3AktLight(i, item);
		item->Akt = false;

		item++;
	}
}

////////////////////////////////////////////////////////////////////
// resetování bufferu svìtel

void D3DL_Reset()
{
	D3DLITEM* item = D3DL_Data;
	for (int i = 0; i < D3DL_Num; i++)
	{
		pD3LightReset(i, item);
		item->Akt = true;
		item++;
	}
}


/***************************************************************************\
*																			*
*								D3D buffer - rámy							*
*																			*
\***************************************************************************/

// vzor jednotkové matice
D3DMATRIX	Matrix1Vzor	=
{
	1,	0,	0,	0,
	0,	1,	0,	0,
	0,	0,	1,	0,
	0,	0,	0,	1
};

////////////////////////////////////////////////////////////////////
// data bufferu rámù

D3DFITEM*	D3DF_Data = NULL;	// ukazatel na data
bool*		D3DF_Valid = NULL;	// pøíznaky platnosti položek
int			D3DF_Max = 0;		// velikost bufferu (položek)
int			D3DF_Next = -1;		// pøíští volná položka (-1 = není)

////////////////////////////////////////////////////////////////////
// seznam objektù pro Blending

//BLENDITEM*	D3DB_Data = NULL;	// ukazatel na data
//int			D3DB_Max = 0;		// velikost bufferu (velikost jako D3DF_Max)
//int			D3DB_Num = 0;		// poèet položek v bufferu

BLENDTYPE	D3DSrcBlend = BLEND_INVALID; // aktuální nastavení blending operace zdroje
BLENDTYPE	D3DDstBlend = BLEND_INVALID; // aktuální nastavení blending operace cíle
int			D3DAlphaRef = -1;			// aktuální nastavení alfa úrovnì

int			D3DColorOp[MAX_STAGES];		// aktuální nastavení operací barev
int			D3DColorArg1[MAX_STAGES];	// aktuální nastavení argumentu 1 operací barev
int			D3DColorArg2[MAX_STAGES];	// aktuální nastavení argumentu 2 operací barev
int			D3DAlphaOp[MAX_STAGES];		// aktuální nastavení operací barev
int			D3DAlphaArg1[MAX_STAGES];	// aktuální nastavení argumentu 1 operací barev
int			D3DAlphaArg2[MAX_STAGES];	// aktuální nastavení argumentu 2 operací barev
int			D3DAddressU[MAX_STAGES];	// aktuální adresování textur U
int			D3DAddressV[MAX_STAGES];	// aktuální adresování textur V
//int			D3DBorder[MAX_STAGES];		// barva okolí textur
float		D3DBias[MAX_STAGES];		// zjemnìní vzdálených textur
DWORD		D3DTFactor = MAXDWORD;		// aktuální nastavení faktoru textur
BOOL		D3DZWrite = -1;				// aktuální hodnota hloubkového zápisu
int			D3DZTest = -1;				// aktuální hodnota hloubkového testu

BOOL		D3DStencilEnable = FALSE;	// aktuální hodnota povolení stencil operací
bool		D3DStencilCleared = false;	// stencil buffer je v aktuálním snímku vymazán
int			D3DStencilFunc = -1;		// aktuální hodnota testu stencil bufferu
int			D3DStencilRef = -1;			// aktuální referenèní hodnota stencil testu
int			D3DStencilMask = -1;		// aktuální hodnota masky stencil testu
int			D3DStencilWriteMask = -1;	// aktuální hodnota zápisové masky do stencil bufferu
int			D3DStencilFail = -1;		// aktuální hodnota operace pøi neúspìchu stencil testu
int			D3DStencilZFail = -1;		// aktuální hodnota operace pøi úspìchu stencil testu a neúspìchu Z testu
int			D3DStencilZPass = -1;		// aktuální hodnota operace pøi úspìchu stencil testu a úspìchu Z testu

BOOL		RendSort[MAX_RENDGROUP];	// pøíznaky tøídìní skupin
int			RendNum[MAX_RENDGROUP];		// poèty položek v bufferech
D3DFITEM**	RendItems[MAX_RENDGROUP];	// seznam položek k renderování
int			RendMax = 0;				// velikost bufferù položek

int			ShadowNum;					// poèet položek v bufferu stínù
D3DFITEM**	ShadowItems;				// seznam položek k renderování stínù

int			D3DAktMat = -1;				// první položka k aktualizaci matic (-1=není)

////////////////////////////////////////////////////////////////////
// seznam objektù 2D obrázkù

D3DFITEM**	D3DP_Items = NULL;	// ukazatel na data seznamu obrázkù
int			D3DP_Num = 0;		// poèet obrázkù v seznamu


////////////////////////////////////////////////////////////////////
// transpozice matice (zdrojová a cílová matice mùže být stejná)

void _fastcall MatrixTrans(D3DMATRIX* dst, D3DMATRIX* src)
{
	*(int*)(&dst->_11) = *(int*)(&src->_11);
	*(int*)(&dst->_22) = *(int*)(&src->_22);
	*(int*)(&dst->_33) = *(int*)(&src->_33);
	*(int*)(&dst->_44) = *(int*)(&src->_44);

	int tmp;

	tmp = *(int*)(&src->_21);
	*(int*)(&dst->_21) = *(int*)(&src->_12);
	*(int*)(&dst->_12) = tmp;

	tmp = *(int*)(&src->_31);
	*(int*)(&dst->_31) = *(int*)(&src->_13);
	*(int*)(&dst->_13) = tmp;

	tmp = *(int*)(&src->_41);
	*(int*)(&dst->_41) = *(int*)(&src->_14);
	*(int*)(&dst->_14) = tmp;

	tmp = *(int*)(&src->_32);
	*(int*)(&dst->_32) = *(int*)(&src->_23);
	*(int*)(&dst->_23) = tmp;

	tmp = *(int*)(&src->_42);
	*(int*)(&dst->_42) = *(int*)(&src->_24);
	*(int*)(&dst->_24) = tmp;

	tmp = *(int*)(&src->_43);
	*(int*)(&dst->_43) = *(int*)(&src->_34);
	*(int*)(&dst->_34) = tmp;
}

////////////////////////////////////////////////////////////////////
// inverze matice (zdrojová a cílová matice mùže být stejná)

void MatrixInv(D3DMATRIX* dst, D3DMATRIX* src)
{

// pomocná matice - 2 matice vedle sebe (4 øádky po 8 prvcích)
	float r1[8], r2[8], r3[8], r4[8];
	float* s[4];
	
	s[0] = &r1[0];
	s[1] = &r2[0];
	s[2] = &r3[0];
	s[3] = &r4[0];

// inicializace matice (1.matice = pùvodní, 2.matice = jednotková)
	int i, j;

	for(i=0; i<4; i++)
	{
		for(j=0; j<4; j++)
		{
			s[i][j] = src->m[i][j];

			if(i==j) s[i][j+4] = 1;
			else     s[i][j+4] = 0;
		}
	}

// nalezení nejvìtšího prvku v každém øádku
	float scp[4];
	float scp_max;
	float* s0 = &s[0][0];
	float s1;

	for(i=0; i<4; i++)
	{
		scp_max = (float)fabs(s0[0]); s0++;
		s1 = (float)fabs(s0[0]); s0++; if (s1 > scp_max) scp_max = s1;
		s1 = (float)fabs(s0[0]); s0++; if (s1 > scp_max) scp_max = s1;
		s1 = (float)fabs(s0[0]); s0++; if (s1 > scp_max) scp_max = s1;

		if(scp_max == 0) return;	// je to singulární matice

		scp[i] = scp_max;
	}

	
	int p,jj;
	float* tmprow;

	int pivot_to;

	for(i=0; i<4; i++)
	{

// nalezení øádku s nejvìtší váhou ("osa" matice)
		pivot_to = i;
		scp_max = (float)fabs(s[i][i]/scp[i]);

		for(p=i+1; p<4; p++)
		{
			s1 = (float)fabs(s[p][i]/scp[p]);

			if(s1 > scp_max)
			{ 
				scp_max = s1; pivot_to = p; 
			}

			if(pivot_to != i)
			{
				tmprow = s[i];
				s[i] = s[pivot_to];
				s[pivot_to] = tmprow;
				float tmpscp;
				tmpscp = scp[i];
				scp[i] = scp[pivot_to];
				scp[pivot_to] = tmpscp;
			}
		}
			
		float mji;

// Gaussova eliminace
		s1 = s[i][i];

		for(j=i+1;j<4;j++)
		{
			mji = s[j][i]/s1;
			s[j][i] = 0.0;

			for(jj=i+1;jj<8;jj++)
			{
				s[j][jj] -= mji*s[i][jj];
			}
		}
	}

	if(s[3][3] == 0) return; // singulární matice

// pøevod trojúhelníkové matice na jednotkovou (odeètením prvkù)
	//  x x x x | y y y y
	//  0 x x x | y y y y 
	//  0 0 x x | y y y y
	//  0 0 0 x | y y y y
	//
	//          \/
	//
	//  1 0 0 0 | z z z z
	//  0 1 0 0 | z z z z
	//  0 0 1 0 | z z z z
	//  0 0 0 1 | z z z z 
	
	float mij;
	for(i=3; i>0; i--)
	{
		s1 = s[i][i];

		for(j=i-1; j>=0; j--)
		{
			mij = s[j][i]/s1;

			for(jj=j+1; jj<8; jj++)
			{
				s[j][jj] -= mij*s[i][jj];
			}
		}
	}
	
// uložení výsledné matice
	for(i=0; i<4; i++)
	{
		s1 = s[i][i];

		for(j=0; j<4; j++)
		{
			dst->m[i][j] = s[i][j+4] / s1;
		}
	}
}

////////////////////////////////////////////////////////////////////
// vynásobení vektoru maticí (provádí se pøi snížené pøesnosti FPU)
// (zdrojový i cílový vektor mohou být shodné)

void VecXMat(D3DVECTOR* dst, D3DVECTOR* src, D3DMATRIX* mat)
{
	float x = src->x;
	float y = src->y;
	float z = src->z;

	dst->x = x*mat->_11 + y*mat->_21 + z*mat->_31 + mat->_41;
	dst->y = x*mat->_12 + y*mat->_22 + z*mat->_32 + mat->_42;
	dst->z = x*mat->_13 + y*mat->_23 + z*mat->_33 + mat->_43;
}

////////////////////////////////////////////////////////////////////
// inverzní rotace pohledové matice podle osy X (funkce SIN negovaná)

void _fastcall D3DF_ViewRotateX(D3DFITEM* item)
{
	if (item->IsRotX)
	{
		double s = item->SinX;
		double c = item->CosX;

		float* m = &(D3DViewMatrix._12);

		for (int i = 4; i > 0; i--)
		{
			float t1 = m[0];
			float t2 = m[1];
			m[0] = (float)(t1*c + t2*s);
			m[1] = (float)(t2*c - t1*s);

			m += 4;
		}
	}
}

////////////////////////////////////////////////////////////////////
// inverzní rotace pohledové matice podle osy Y (funkce SIN negovaná)

void _fastcall D3DF_ViewRotateY(D3DFITEM* item)
{
	if (item->IsRotY)
	{
		double s = item->SinY;
		double c = item->CosY;

		float* m = &(D3DViewMatrix._11);

		for (int i = 4; i > 0; i--)
		{
			float t1 = m[0];
			float t2 = m[2];
			m[0] = (float)(t1*c - t2*s);
			m[2] = (float)(t2*c + t1*s);

			m += 4;
		}
	}
}

////////////////////////////////////////////////////////////////////
// inverzní rotace pohledové matice podle osy Z (funkce SIN negovaná)

void _fastcall D3DF_ViewRotateZ(D3DFITEM* item)
{
	if (item->IsRotZ)
	{
		double s = item->SinZ;
		double c = item->CosZ;

		float* m = &(D3DViewMatrix._11);

		for (int i = 4; i > 0; i--)
		{
			float t1 = m[0];
			float t2 = m[1];
			m[0] = (float)(t1*c + t2*s);
			m[1] = (float)(t2*c - t1*s);

			m += 4;
		}
	}
}

////////////////////////////////////////////////////////////////////
// generování pohledové matice (inverznì v poøadí od root položky)

void GenViewMat(int index)
{

// adresa položky
	D3DFITEM* item = D3DF_Data + index;

// je-li platný rodiè (ne scéna), generování nejdøíve rodièe
	int par = item->Parent;
	if (par > 0) GenViewMat(par);

// zpìtná transformace posunu
	D3DViewMatrix._41 = (float)(D3DViewMatrix._41 - item->TransX);
	D3DViewMatrix._42 = (float)(D3DViewMatrix._42 - item->TransY);
	D3DViewMatrix._43 = (float)(D3DViewMatrix._43 - item->TransZ);

// provedení rotací (v opaèném poøadí)
	switch (item->Order)
	{
	case D3DFORDER_XYZ:
		D3DF_ViewRotateZ(item);
		D3DF_ViewRotateY(item);
		D3DF_ViewRotateX(item);
		break;

	case D3DFORDER_XZY:
		D3DF_ViewRotateY(item);
		D3DF_ViewRotateZ(item);
		D3DF_ViewRotateZ(item);
		break;

	case D3DFORDER_YXZ:
		D3DF_ViewRotateZ(item);
		D3DF_ViewRotateX(item);
		D3DF_ViewRotateY(item);
		break;

	case D3DFORDER_YZX:
		D3DF_ViewRotateX(item);
		D3DF_ViewRotateZ(item);
		D3DF_ViewRotateY(item);
		break;

	case D3DFORDER_ZXY:
		D3DF_ViewRotateY(item);
		D3DF_ViewRotateX(item);
		D3DF_ViewRotateZ(item);
		break;

	case D3DFORDER_ZYX:
		D3DF_ViewRotateX(item);
		D3DF_ViewRotateY(item);
		D3DF_ViewRotateZ(item);
		break;
	}

// zpìtná transformace mìøítka
	if (item->ScaleX != 1)
	{
		double k = 1/item->ScaleX;
		D3DViewMatrix._11 = (float)(D3DViewMatrix._11*k);
		D3DViewMatrix._21 = (float)(D3DViewMatrix._21*k);
		D3DViewMatrix._31 = (float)(D3DViewMatrix._31*k);
		D3DViewMatrix._41 = (float)(D3DViewMatrix._41*k);
	}

	if (item->ScaleY != 1)
	{
		double k = 1/item->ScaleY;
		D3DViewMatrix._12 = (float)(D3DViewMatrix._12*k);
		D3DViewMatrix._22 = (float)(D3DViewMatrix._22*k);
		D3DViewMatrix._32 = (float)(D3DViewMatrix._32*k);
		D3DViewMatrix._42 = (float)(D3DViewMatrix._42*k);
	}

	if (item->ScaleZ != 1)
	{
		double k = 1/item->ScaleZ;
		D3DViewMatrix._13 = (float)(D3DViewMatrix._13*k);
		D3DViewMatrix._23 = (float)(D3DViewMatrix._23*k);
		D3DViewMatrix._33 = (float)(D3DViewMatrix._33*k);
		D3DViewMatrix._43 = (float)(D3DViewMatrix._43*k);
	}
}


////////////////////////////////////////////////////////////////////
// rotace vlastní matice podle osy X (provádí se pøi snížené pøesnosti FPU!)
//
// Rotaèní matice:
//		1	0	0	0		11, 12*c - 13*s, 12*s + 13*c, 14
//		0	c	s	0		21, 22*c - 23*s, 22*s + 23*c, 24
//		0	-s	c	0		31, 32*c - 33*s, 32*s + 33*c, 34
//		0	0	0	1		41, 42*c - 43*s, 42*s + 43*c, 44

void _fastcall D3DF_RotateX(D3DFITEM* item)
{
	if (item->IsRotX)
	{
		double s = item->SinX;
		double c = item->CosX;

		float* m = &(item->MatrixOwn._12);

		for (int i = 4; i > 0; i--)
		{
			float t1 = m[0];
			float t2 = m[1];
			m[0] = (float)(t1*c - t2*s);
			m[1] = (float)(t1*s + t2*c);

			m += 4;
		}
	}
}

////////////////////////////////////////////////////////////////////
// rotace vlastní matice podle osy Y (provádí se pøi snížené pøesnosti FPU!)
//
// Rotaèní matice:
//		c	0	-s	0		11*c + 13*s, 12, 13*c - 11*s, 14
//		0	1	0	0		21*c + 23*s, 22, 23*c - 21*s, 24
//		s	0	c	0		31*c + 33*s, 32, 33*c - 31*s, 34
//		0	0	0	1		41*c + 43*s, 42, 43*c - 41*s, 44

void _fastcall D3DF_RotateY(D3DFITEM* item)
{
	if (item->IsRotY)
	{
		double s = item->SinY;
		double c = item->CosY;

		float* m = &(item->MatrixOwn._11);

		for (int i = 4; i > 0; i--)
		{
			float t1 = m[0];
			float t2 = m[2];
			m[0] = (float)(t1*c + t2*s);
			m[2] = (float)(t2*c - t1*s);

			m += 4;
		}
	}
}

////////////////////////////////////////////////////////////////////
// rotace vlastní matice podle osy Z (provádí se pøi snížené pøesnosti FPU!)
//
// Rotaèní matice:
//		c	s	0	0		11*c - 12*s, 11*s + 12*c, 13, 14,
//		-s	c	0	0		21*c - 22*s, 21*s + 22*c, 23, 24,
//		0	0	1	0		31*c - 32*s, 31*s + 32*c, 33, 34,
//		0	0	0	1		41*c - 42*s, 41*s + 42*c, 43, 44,

void _fastcall D3DF_RotateZ(D3DFITEM* item)
{
	if (item->IsRotZ)
	{
		double s = item->SinZ;
		double c = item->CosZ;

		float* m = &(item->MatrixOwn._11);

		for (int i = 4; i > 0; i--)
		{
			float t1 = m[0];
			float t2 = m[1];
			m[0] = (float)(t1*c - t2*s);
			m[1] = (float)(t1*s + t2*c);

			m += 4;
		}
	}
}

////////////////////////////////////////////////////////////////////
// vynásobení dvou matic (m = m1 * m2), výsledná matice nesmí být vstupní!

void _fastcall MatrixMul(D3DMATRIX* m, const D3DMATRIX* m1, const D3DMATRIX* m2)
{
	ASSERT(m != m1);
	ASSERT(m != m2);

	for (int i = 4; i > 0; i--)
	{
		m->_11 = m1->_11*m2->_11 + m1->_12*m2->_21 + m1->_13*m2->_31 + m1->_14*m2->_41;
		m->_12 = m1->_11*m2->_12 + m1->_12*m2->_22 + m1->_13*m2->_32 + m1->_14*m2->_42;
		m->_13 = m1->_11*m2->_13 + m1->_12*m2->_23 + m1->_13*m2->_33 + m1->_14*m2->_43;
		m->_14 = m1->_11*m2->_14 + m1->_12*m2->_24 + m1->_13*m2->_34 + m1->_14*m2->_44;

		m = (D3DMATRIX*)(&(m->_21));
		m1 = (D3DMATRIX*)(&(m1->_21));
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace mìøítka pro hranici

void AktScaleB(D3DFITEM* item)
{
	double s = fabs(item->ScaleX);
	double s2 = fabs(item->ScaleY);
	if (s2 > s) s = s2;
	s2 = fabs(item->ScaleZ);
	if (s2 > s) s = s2;
	item->ScaleB = s;
}

////////////////////////////////////////////////////////////////////
// aktualizace sektorù objektu (kontroluje index)

void D3DF_AktSector(int index)
{
// adresa položky
	if (D3DF_IsValid(index))
	{
		D3DFITEM* item = D3DF_Data + index;
		item->AktSector = FALSE;
		item->UseSector = FALSE;

// jiné objekty než D3DFTYPE_OBJECT a D3DFTYPE_SECTOR nemají nikdy více vrcholù
		if ((item->Type != D3DFTYPE_OBJECT) &&
			(item->Type != D3DFTYPE_SECTOR))
		{
			return;
		}
	
// zrušení starých sektorù
		int index2 = item->Child;
		while (index2 > 0)
		{
			D3DFITEM* item2 = D3DF_Data + index2;
			int index3 = index2;
			index2 = item2->Next;

			if (item2->Type == D3DFTYPE_SECTOR)
			{
				D3DF_Del(index3);
			}
		}

// test, zda budou použity sektory
		int vertN = item->VertNum;

		BOOL usesector = (vertN > MaxVertexNum2);
		if (usesector)
		{
			item->UseSector = TRUE;
			int i;

// pøíprava bufferù sektorù
			double sumx[8];		// souèet souøadnic X
			double sumy[8];		// souèet souøadnic Y
			double sumz[8];		// souèet souøadnic Z
			int fN[8];			// poèet plošek v sektoru
			int vN[8];			// poèet vrcholù v sektoru
			int* vmap[8];		// mapování vrcholù pro sektor

			for (i = 0; i < 8; i++)
			{
				sumx[i] = 0;
				sumy[i] = 0;
				sumz[i] = 0;
				fN[i] = 0;
				vN[i] = 0;
				vmap[i] = (int*)MemGet(vertN * sizeof(int));
				MemFill(vmap[i], vertN * sizeof(int), -1);
			}

// rozdìlení plošek do sektorù
			int faceN = item->FaceNum;	// poèet plošek celkem
			int* sec = (int*)MemGet(faceN * sizeof(int)); // indexy sektorù plošek
			int* face = item->Face;		// buffer plošek
			D3DVECTOR* vert = item->Vertex;	// buffer vrcholù

			for (i = 0; i < faceN; i++)
			{
			// zjištìní sektoru, do kterého ploška patøí
				D3DVECTOR* vv = vert + face[0];
				double x = vv->x;
				double y = vv->y;
				double z = vv->z;

				vv = vert + face[1];
				x += vv->x;
				y += vv->y;
				z += vv->z;

				vv = vert + face[2];
				x += vv->x;
				y += vv->y;
				z += vv->z;

				int s = 0;
				if (x < 0) s++;
				if (y < 0) s += 2;
				if (z < 0) s += 4;

				sec[i] = s;

			// pøiètení vrcholù
				fN[s]++;
				sumx[s] += x;
				sumy[s] += y;
				sumz[s] += z;

			// zmapování vrcholù plošky
				int v = face[0];
				if (vmap[s][v] < 0)
				{
					vmap[s][v] = vN[s];
					vN[s]++;
				}

				v = face[1];
				if (vmap[s][v] < 0)
				{
					vmap[s][v] = vN[s];
					vN[s]++;
				}

				v = face[2];
				if (vmap[s][v] < 0)
				{
					vmap[s][v] = vN[s];
					vN[s]++;
				}

				face += 3;
			}

// vytvoøení nových objektù
			for (i = 0; i < 8; i++)
			{
				int fn = fN[i];

				if (fn > 0)
				{
				// vytvoøení nové položky
					int index2 = D3DF_New();
					D3DF_Link(index, index2);
					D3DFITEM* item2 = D3DF_Data + index2;
					item = D3DF_Data + index;

				// kopie obsahu položky
					int parent = item2->Parent;
					int child = item2->Child;
					int prev = item2->Prev;
					int next = item2->Next;

					D3DF_AktMatUnlink(item2);

					MemCopy(item2, item, sizeof(D3DFITEM));

					item2->AktMat = false;
					item2->Index = index2;
					item2->Next = next;
					item2->Prev = prev;
					item2->Child = child;
					item2->Parent = parent;
					item2->CloneNext = index2;
					item2->ClonePrev = index2;

					DuplikData((void**)&(item2->AutoMap), sizeof(AUTOMAP));

				// inicializace parametrù
					item2->Type = D3DFTYPE_SECTOR;
					item2->AktSector = TRUE;
					item2->UseSector = FALSE;
					item2->Visible = true;
					item2->AktOwn = true;
					item2->IsTerrain = false;
					item2->TerMap = NULL;

					D3DF_AktMatLink(item2);

					item2->MorphMax = 0;
					item2->MorphStage = 0;
					item2->MorphVert = NULL;
					item2->MorphNorm = NULL;
					item2->MorphUV = NULL;

					item2->ScaleX = 1;
					item2->ScaleY = 1;
					item2->ScaleZ = 1;
					item2->ScaleB = 1;
					item2->RotateX = 0;
					item2->SinX = 0;
					item2->CosX = 1;
					item2->RotateY = 0;
					item2->RotateY2D = 0;
					item2->SinY = 0;
					item2->CosY = 1;
					item2->RotateZ = 0;
					item2->SinZ = 0;
					item2->CosZ = 1;
					item2->IsRotX = false;
					item2->IsRotY = false;
					item2->IsRotZ = false;

				// vytvoøení bufferù objektu
					int vn = vN[i];
					item2->VertNum = vn;

					D3DVECTOR* v2 = (D3DVECTOR*)MemGet(vn * sizeof(D3DVECTOR));
					item2->Vertex = v2;

					D3DVECTOR* n2 = NULL;
					if (item->Normal != NULL)
					{
						n2 = (D3DVECTOR*)MemGet(vn * sizeof(D3DVECTOR));
					}
					item2->Normal = n2;

					DWORD* c2 = NULL;
					D3DCOLORVALUE* cf2 = NULL;
					if (item->VertCol != NULL)
					{
						c2 = (DWORD*)MemGet(vn * sizeof(DWORD));
						cf2 = (D3DCOLORVALUE*)MemGet(vn * sizeof(D3DCOLORVALUE));
					}
					item2->VertCol = c2;
					item2->VertColF = cf2;

					float* uv2[MAX_STAGES];
					for (int j = 0; j < MAX_STAGES; j++)
					{
						uv2[j] = NULL;
						if (item->TextUV[j] != NULL)
						{
							uv2[j] = (float*)MemGet(vn * 2 * sizeof(float));
						}

						item2->TextUV[j] = uv2[j];
						item2->Texture[j] = -1;
					}
					
					item2->FaceNum = fn;
					int* f2 = (int*)MemGet(fn * 3 * sizeof(int));
					item2->Face = f2;
					item2->FaceW = NULL;

					item2->MatSource = MATSOURCE_PARENT;

					item2->ShadowPoint.x = 0;
					item2->ShadowPoint.y = 0;
					item2->ShadowPoint.z = 0;
					item2->ShadeVert = NULL;
					item2->ShadeFace = NULL;
					item2->ShadeNorm = NULL;
					item2->ShadeEdgeV = NULL;
					item2->ShadeEdgeN = NULL;
					item2->ShadeEdgeS = NULL;
					item2->ShadeNext = NULL;
					item2->ShadeLeft = NULL;
					item2->ShadowFan = NULL;
					item2->ShadowVolume = NULL;

					item2->Data1 = NULL;
					item2->Data2 = NULL;
					item2->Data3 = NULL;

				// korekce transformace objektu
					double tx = sumx[i] / (fn * 3);
					double ty = sumy[i] / (fn * 3);
					double tz = sumz[i] / (fn * 3);
					item2->TransX = tx;
					item2->TransY = ty;
					item2->TransZ = tz;

					tx = -tx;
					ty = -ty;
					tz = -tz;

				// pøenesení vrcholù
					int* vm = vmap[i];

					for (j = 0; j < vertN; j++)
					{
						int k = vm[j];

						if (k >= 0)
						{
							item2->Vertex[k].x = (float)(item->Vertex[j].x + tx);
							item2->Vertex[k].y = (float)(item->Vertex[j].y + ty);
							item2->Vertex[k].z = (float)(item->Vertex[j].z + tz);

							if (item->Normal != NULL)
							{
								item2->Normal[k].x = item->Normal[j].x;
								item2->Normal[k].y = item->Normal[j].y;
								item2->Normal[k].z = item->Normal[j].z;
							}

							if (item->VertCol != NULL)
							{
								item2->VertCol[k] = item->VertCol[j];
								item2->VertColF[k].r = item->VertColF[j].r;
								item2->VertColF[k].g = item->VertColF[j].g;
								item2->VertColF[k].b = item->VertColF[j].b;
								item2->VertColF[k].a = item->VertColF[j].a;
							}

							for (int s = 0; s < MAX_STAGES; s++)
							{
								if (item->TextUV[s] != NULL)
								{
									item2->TextUV[s][2*k] = item->TextUV[s][2*j];
									item2->TextUV[s][2*k+1] = item->TextUV[s][2*j+1];
								}
							}
						}
					}

				// pøenesení plošek
					int ff = 0;

					for (j = 0; j < faceN; j++)
					{
						if (sec[j] == i)
						{
							item2->Face[ff] = vm[item->Face[j*3]];
							item2->Face[ff+1] = vm[item->Face[j*3+1]];
							item2->Face[ff+2] = vm[item->Face[j*3+2]];
							ff += 3;
						}
					}	 

				// aktualizace hranice objektu (rodièe není potøeba aktualizovat)
					D3DF_MapBound(index2);
				}
			}

// zrušení bufferù sektorù
			for (i = 0; i < 8; i++)
			{
				MemFree(vmap[i]);
			}
			MemFree(sec);
		}
	}
}

////////////////////////////////////////////////////////////////////
// pøipojení do øetìzce aktualizace matic (kontroluje, zda je pøipojen)

void _fastcall D3DF_AktMatLink(D3DFITEM* item)
{
	if (!item->AktMat && (item->Index != 0))
	{
		item->AktMat = true;

		item->AktMatPrev = -1;

		int next = D3DAktMat;
		D3DAktMat = item->Index;
		item->AktMatNext = next;

		if (next >= 0)
		{
			D3DF_Data[next].AktMatPrev = item->Index;
		}
	}
}

////////////////////////////////////////////////////////////////////
// odpojení z øetìzce aktualizace matic (kontroluje, zda je pøipojen)

void _fastcall D3DF_AktMatUnlink(D3DFITEM* item)
{
	if (item->AktMat)
	{
		item->AktMat = false;

		int prev = item->AktMatPrev;
		int next = item->AktMatNext;

		if (prev >= 0)
		{
			D3DF_Data[prev].AktMatNext = next;
		}
		else
		{
			D3DAktMat = next;
		}

		if (next >= 0)
		{
			D3DF_Data[next].AktMatPrev = prev;
		}
	}
}

////////////////////////////////////////////////////////////////////
// aktualizace transformaèních matic všech objektù
// (provádí se pøi snížené pøesnosti FPU!)

void D3DF_AktMatrix()
{

// další položka k aktualizaci
	for (;;) 
	{
		int index = D3DAktMat;
		if (index < 0) break;
		D3DFITEM* item = D3DF_Data + index;

// aktualizace hranice
		if (item->AktBound)
		{
			item->AktBound = false;

			double r = item->BoundR0;

			int next = item->Child;

			while (next > 0)
			{
				D3DFITEM* item2 = D3DF_Data + next;

				float sx = item2->MatrixOwn._41;
				float sy = item2->MatrixOwn._42;
				float sz = item2->MatrixOwn._43;

				double r2 = item2->BoundR*item2->ScaleB + sqrt(sx*sx + sy*sy + sz*sz);

				if (r2 > r) r = r2;

				next = item2->Next;
			}

			if (r != item->BoundR)
			{
				item->BoundR = r;

				int par = item->Parent;

				if (par > 0)
				{
					D3DF_Data[par].AktBound = true;
					D3DF_AktMatLink(D3DF_Data + par);
				}
			}
		}

// aktualizace sektorù objektu
		if (item->AktSector) // || ((item->VertNum > MaxVertexNum2) && !item->UseSector))
		{
			D3DF_AktSector(index);
			item = D3DF_Data + index;
		}

// odpojení z aktualizací matic (AktSektor mùže zaøazovat do øetìzce)
		D3DF_AktMatUnlink(item);

// test, zda je 2D objekt
		if (item->Type == D3DFTYPE_DECAL)
		{

// naètení souøadnic relativnì ke kameøe (nevadí, že se použijí staré souøadnice)
			double x = item->MatrixWorld._41 - D3DF_Data[1].MatrixWorld._41;
			double z = item->MatrixWorld._43 - D3DF_Data[1].MatrixWorld._43;

// test, zda je dostateèná zmìna pro aktualizaci
			if ((fabs(x) < 1) ||
				(fabs(z) < 1) ||
				(fabs((item->RotateX2D - x)/x) >= 0.0625) ||
				(fabs((item->RotateZ2D - z)/z) >= 0.0625))
			{
				item->RotateX2D = x;
				item->RotateZ2D = z;

// úhel smìru ke kameøe
				double alpha;
				if (item->Parent == 1)
				{
					alpha = 0;
				}
				else
				{
					alpha = uhel90 - atan2(z, x);
				}

// nastavení úhlu
				if (alpha != item->RotateY2D)
				{
					item->RotateY2D = alpha;
					alpha += item->RotateY;
					item->SinY = sin(alpha);
					item->CosY = cos(alpha);
					item->IsRotY = true;
					item->AktOwn = true;
				}
			}
		}

// test, zda je potøeba aktualizovat vlastní matici
		if (item->AktOwn)
		{
			item->AktOwn = false;				// zrušení požadavku aktualizace
			item->AktWorld = true;				// požadavek aktualizace výsledné matice

// pøíprava matice mìøítka
			D3DMATRIX* m = &(item->MatrixOwn);
			Matrix0(m);							// vynulování matice
			m->_11 = (float)item->ScaleX;		// mìøítko ve smìru X
			m->_22 = (float)item->ScaleY;		// mìøítko ve smìru Y
			m->_33 = (float)item->ScaleZ;		// mìøítko ve smìru Z
			m->_44 = 1;

// aktualizace rotací
			switch (item->Order)
			{
			case D3DFORDER_XYZ:
				D3DF_RotateX(item);
				D3DF_RotateY(item);
				D3DF_RotateZ(item);
				break;

			case D3DFORDER_XZY:
				D3DF_RotateX(item);
				D3DF_RotateZ(item);
				D3DF_RotateY(item);
				break;

			case D3DFORDER_YXZ:
				D3DF_RotateY(item);
				D3DF_RotateX(item);
				D3DF_RotateZ(item);
				break;

			case D3DFORDER_YZX:
				D3DF_RotateY(item);
				D3DF_RotateZ(item);
				D3DF_RotateX(item);
				break;

			case D3DFORDER_ZXY:
				D3DF_RotateZ(item);
				D3DF_RotateX(item);
				D3DF_RotateY(item);
				break;

			case D3DFORDER_ZYX:
				D3DF_RotateZ(item);
				D3DF_RotateY(item);
				D3DF_RotateX(item);
				break;
			}
			
// pøidání posunu
			m->_41 = (float)(m->_41 + item->TransX);
			m->_42 = (float)(m->_42 + item->TransY);
			m->_43 = (float)(m->_43 + item->TransZ);

// je nutné aktualizovat polomìr hranice rodièe
			int par = item->Parent;
			if (par > 0)
			{
				D3DF_Data[par].AktBound = true;
				D3DF_AktMatLink(D3DF_Data + par);
			}
		}

// aktualizace výsledné transformaèní matice
		if (item->AktWorld)
		{
			item->AktWorld = false;				// zrušení požadavku aktualizace matice

			item->ShadowPoint.x = 0;			// aktualizovat stín
			item->ShadowPoint.y = 0;
			item->ShadowPoint.z = 0;

// požadavek k aktualizici svìtla
			if (item->Type == D3DFTYPE_LIGHT)
			{
				D3DL_Get(item->Light)->Akt = true;
			}

// pøegenerování lensfalre
//			if (item->Type == D3DFTYPE_LENSFLARE)
//			{
//				pD3Reset(item);
//			}

// je-li rodièem scéna, matice se nekombinují
			int index2 = item->Parent;

			if (index2 == 0)
			{
				MemCopy(&(item->MatrixWorld), &(item->MatrixOwn), sizeof(D3DMATRIX));
			}

// pro jiné rodièe kombinace s jejich maticí
			else
			{
				D3DMATRIX* m2 = &(D3DF_Data[index2].MatrixWorld);
				MatrixMul(&(item->MatrixWorld), &(item->MatrixOwn), m2);
			}

// aktualizace pohledové matice, je-li to kamera
			if (index == 1)
			{

// inicializace pohledové matice na jednotkovou matici
				Matrix1(&D3DViewMatrix);

// vygenerování matice
				GenViewMat(1);

// požadavek aktualizace pohledové matice
				D3DViewAkt = true;
			}

// požadavek aktualizace matice všech potomkù
			int next = item->Child;

			while (next > 0)
			{
				D3DFITEM* item2 = D3DF_Data + next;

				item2->AktWorld = true;

				D3DF_AktMatLink(item2);

				next = item2->Next;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////
// nastavení transformací podle transformaèní matice

void D3DF_SetTransMatrix(int index, D3DMATRIX* m)
{
	if (D3DF_IsValid(index))
	{
		D3DFITEM* item = D3DF_Data + index;

		item->TransX = m->_41;
		item->TransY = m->_42;
		item->TransZ = m->_43;

		double m11 = m->_11;
		double m12 = m->_12;
		item->ScaleX = sqrt(m->_11*m->_11 + m->_12*m->_12 + m->_13*m->_13);
		item->ScaleY = sqrt(m->_21*m->_21 + m->_22*m->_22 + m->_23*m->_23);
		item->ScaleZ = sqrt(m->_31*m->_31 + m->_32*m->_32 + m->_33*m->_33);
		AktScaleB(item);

		double rotx = 0;
		double roty = 0;
		double rotz = 0;

#define EPS 0.000001
		if ((m11 > EPS) || (m11 < -EPS) || (m12 > EPS) || (m12 < -EPS))
		{
			rotz = atan2(m12, m11);
			rotx = atan2(m->_23/item->ScaleY, m->_33/item->ScaleZ);
			roty = atan2(-m->_13, m12/item->SinZ);
		}
		else
		{
			rotz = 0;

			if (m->_13 >= 0)
			{
				roty = uhel270;
				rotx = atan2(m->_32, m->_31);
			}
			else
			{
				roty = uhel90;
				rotx = atan2(-m->_32, m->_31);
			}
		}
		item->Order = D3DFORDER_XYZ;

		item->RotateX = rotx;
		item->IsRotX = (rotx != 0);
		item->SinX = sin(rotx);
		item->CosX = cos(rotx);

		item->RotateY = roty;
		item->IsRotY = (roty != 0);
		item->SinY = sin(roty);
		item->CosY = cos(roty);

		item->RotateZ = rotz;
		item->IsRotZ = (rotz != 0);
		item->SinZ = sin(rotz);
		item->CosZ = cos(rotz);

		item->AktOwn = true;

		D3DF_AktMatLink(item);
	}
}

////////////////////////////////////////////////////////////////////
// vytvoøení nové položky bufferu (vrací index položky)
// (POZOR - volá se bìhem inicializace, ještì pøed inicializací obsluhy 3D!)

int D3DF_New()
{
// bude zvýšení velikosti bufferu
	if (D3DF_Next < 0)			// je další položka?
	{
		int i = D3DF_Max;		// to bude poèet nových položek
		if (i == 0) i = 64;		// implicitní poèet položek
		int next = D3DF_Max;	// pøíští položka - 1
		D3DF_Next = next;		// odkaz na první novou položku
		D3DF_Max = next + i;	// zvýšení poètu položek v bufferu

// zvýšení velikosti bufferù
		MemBuf(D3DF_Data, D3DF_Max); // zvýšení velikosti bufferu dat
		MemBuf(D3DF_Valid, D3DF_Max); // zvýšení velikosti bufferu platnosti
		MemFill(D3DF_Valid + next, i*sizeof(bool), false); // nastavení pøíznakú na neplatné položky

// inicializace nových odkazù
		D3DFITEM* data = D3DF_Data + next - 1; // ukazatel dat - 1
		for (; i > 0; i--)
		{
			data++;				// zvýšení ukazatele položek
			next++;				// zvýšení indexu pøíští položky
			*(int*)data = next;	// odkaz na pøíští položku
		}
		*(int*)data = -1;		// není další položka
	}

// zplatnìní nové položky
	int i = D3DF_Next;			// pøíští volná položka
	D3DFITEM* item = D3DF_Data + i; // adresa položky
	D3DF_Next = *(int*)item;	// nová další položka
	D3DF_Valid[i] = true;		// nastavení pøíznaku platnosti položky

// inicializace položky
	MemFill(item, sizeof(D3DFITEM), 0);
	item->Index = i;
	item->Parent = -1;
	item->Child = -1;
	item->Prev = -1;
	item->Next = -1;

	item->ClonePrev = i;
	item->CloneNext = i;

// pøipojení položky jako potomek scény (není-li to scéna)
	if (i != 0)
	{
		item->Parent = 0;	// rodièem bude scéna

		D3DFITEM* item2 = D3DF_Data + 0; // adresa položky scény

		int i2 = item2->Child;		// první potomek scény
		item->Next = i2;			// ukazatel na dalšího potomka
		item2->Child = i;			// položka bude prvním potomkem scény

		if (i2 >= 0)				// byl již nìjaký jiný potomek?
		{
			D3DFITEM* item3 = D3DF_Data + i2;	// adresa jiného potomka
			item3->Prev = i;		// položka bude jeho pøedcházejícím sousedem
		}
	}

// inicializace ostatních parametrù
	item->Visible = true;
	item->ScaleX = 1;
	item->ScaleY = 1;
	item->ScaleZ = 1;
	item->ScaleB = 1;

	item->CosX = 1;
	item->CosY = 1;
	item->CosZ = 1;

	item->Wireframe = true;
	item->Lighton = true;
	item->Shades = true;

	for (int j = 0; j < MAX_STAGES; j++)
	{
		item->MinFilter[j] = true;
		item->MagFilter[j] = true;
		item->MipFilter[j] = true;
		item->Texture[j] = -1;

		item->ColorOp[j] = D3DTOP_DISABLE;
		item->ColorArg1[j] = D3DTA_TEXTURE;
		item->ColorArg2[j] = D3DTA_CURRENT;
		item->AlphaOp[j] = D3DTOP_DISABLE; 
		item->AlphaArg1[j] = D3DTA_TEXTURE;
		item->AlphaArg2[j] = D3DTA_CURRENT;
		item->AddressU[j] = D3DTADDRESS_WRAP;
		item->AddressV[j] = D3DTADDRESS_WRAP;
//		item->BorderRGB[j] = 0xff000000;
//		item->BorderF[j].a = 1;
	}

	item->LODPrev = -1;
	item->LODNext = -1;
	item->LODMax = 1e15;
	item->LODMax2 = (float)1e30;

	item->ColorOp[0] = D3DTOP_MODULATE;
	item->AlphaOp[0] = D3DTOP_MODULATE;
	item->UseText[0] = true;

	item->TFactor = 0xffffff;
	item->TFactorRGB = MAXDWORD;
	item->TFactorF.r = 1;
	item->TFactorF.g = 1;
	item->TFactorF.b = 1;
	item->TFactorF.a = 1;

	item->Culling = 1;
	item->Light = -1;
	item->Diffuse = 0xffffff;
	item->Ambient = MAXDWORD;
	item->AutoCol = MAXDWORD;
	item->SrcBlend = BLEND_ONE;
	item->SrcDstBlend = 1;
	item->BoundR0 = 0.001;
	item->BoundR = 0.001;
	item->AktBound = true;
	item->RendGroup = RENDNORM;
	item->ZTest = D3DCMP_LESSEQUAL;
	item->ZWrite = TRUE;

	Matrix1(&item->MatrixOwn);
	Matrix1(&item->MatrixWorld);

	D3DF_AktMatLink(item);

	return i;
}

////////////////////////////////////////////////////////////////////
// pøíprava položky k modifikaci (odpojení od klonù)

void D3DF_Modi(int index)
{
// kontrola položky
	if (!D3DF_IsValid(index)) return;

// adresa položky
	D3DFITEM* item = D3DF_Data + index;

// test, zda je souèástí øetìzce klonù
	int cloneprev = item->ClonePrev;
	int clonenext = item->CloneNext;
	if (cloneprev == item->Index) return;

// odpojení od klonù
	D3DF_Data[cloneprev].CloneNext = clonenext;
	D3DF_Data[clonenext].ClonePrev = cloneprev;
	item->CloneNext = index;
	item->ClonePrev = index;

// duplikace terénu (kopie datového bufferu)
	if (item->IsTerrain)
	{
		DuplikData((void**)&(item->TerMap), (item->TerWidth+1)*(item->TerHeight+1)*sizeof(float));
	}

// duplikace objektu (kopie datových bufferù)
	int i = item->VertNum;
	DuplikData((void**)&(item->Vertex), i * sizeof(D3DVECTOR));
	DuplikData((void**)&(item->Normal), i * sizeof(D3DVECTOR));
	DuplikData((void**)&(item->VertCol), i * sizeof(DWORD));
	DuplikData((void**)&(item->VertColF), i * sizeof(D3DCOLORVALUE));

	int j;
	for (j = 0; j < MAX_STAGES; j++)
	{
		DuplikData((void**)&(item->TextUV[j]), i * 2 * sizeof(float));
	}

	DuplikData((void**)&(item->Face), item->FaceNum * 3 * sizeof(int));
	item->FaceW = NULL;

// duplikace stínù
	if (item->ShadeVert != NULL)
	{
		DuplikData((void**)&(item->ShadeVert), item->ShadeVertN * sizeof(D3DVECTOR));
		i = item->FaceNum;
		DuplikData((void**)&(item->ShadeFace), i * 3 * sizeof(int));
		DuplikData((void**)&(item->ShadeNorm), i * sizeof(D3DVECTOR));
		DuplikData((void**)&(item->ShadeEdgeV), i * 3 * 2 * sizeof(int));
		DuplikData((void**)&(item->ShadeEdgeN), i * 3 * sizeof(int));
		DuplikData((void**)&(item->ShadeNext), i * 3 * sizeof(int));
		DuplikData((void**)&(item->ShadeLeft), i * 3 * sizeof(int));
	}

// buffery jsou neplatné
	item->Data1 = NULL;
	item->Data2 = NULL;
}

////////////////////////////////////////////////////////////////////
// duplikace jedné položky (bez potomkù) (nekontroluje platnost položky)

int D3DF_Copy1(int index)
{
// pro neplatnou položku se vytvoøí pouze skupina
	if (index <= 1) return D3DF_New();

// vytvoøení nové položky
	int newindex = D3DF_New();

// adresa staré i nové položky
	D3DFITEM* olditem = D3DF_Data + index;
	D3DFITEM* newitem = D3DF_Data + newindex;

// kopie obsahu položky
	int parent = newitem->Parent;
	int child = newitem->Child;
	int prev = newitem->Prev;
	int next = newitem->Next;

	D3DF_AktMatUnlink(newitem);

	MemCopy(newitem, olditem, sizeof(D3DFITEM));

	newitem->AktMat = false;
	newitem->Index = newindex;
	newitem->Next = next;
	newitem->Prev = prev;
	newitem->Child = child;
	newitem->Parent = parent;

	D3DF_AktMatLink(newitem);

	newitem->LODPrev = -1;
	newitem->LODNext = -1;
	newitem->LODMin2 = 0;

	DuplikData((void**)&(newitem->AutoMap), sizeof(AUTOMAP));

// zaèlenìní do øetìzce klonù
	int clonenext = olditem->CloneNext;
	olditem->CloneNext = newindex;
	newitem->ClonePrev = index;
	newitem->CloneNext = clonenext;
	D3DF_Data[clonenext].ClonePrev = newindex;

// duplikace svìtla (vytvoøení nového svìtla se stejnými parametry)
	int i = newitem->Light;
	if (i >= 0)
	{
		D3DLITEM* oldlight = D3DL_Get(i);
		i = D3DL_New();
		newitem->Light = i;
		D3DLITEM* newlight = D3DL_Get(i);
		MemCopy(newlight, oldlight, sizeof(D3DLITEM));
		newlight->IsUsed = true;
		newlight->Akt = true;
		newlight->Frame = newindex;
	}

// aktualizovat matici (rodièem je scéna)
	newitem->AktWorld = true;
	newitem->AktBound = true;

// duplikace textury (pouze reference na texturu)
	for (int j = 0; j < MAX_STAGES; j++)
	{
		i = newitem->Texture[j];
		if (i >= 0)
		{
			D3DTITEM* newtext = D3DT_Get(i);
			newtext->Ref++;
		}
	}

// buffery jsou neplatné
	newitem->ShadowPoint.x = 0;
	newitem->ShadowPoint.y = 0;
	newitem->ShadowPoint.z = 0;
	newitem->ShadeEdgeS = NULL;
	newitem->ShadowFanN = 0;
	newitem->ShadowFan0 = 0;
	newitem->ShadowFan = NULL;
	newitem->ShadowVolume = NULL;

	newitem->Data3 = NULL;

// duplikace bufferù morfování
	int n = newitem->MorphMax;
	if (n > 0)
	{
		DuplikData((void**)&(newitem->MorphVert), n * sizeof(D3DVECTOR*));
		DuplikData((void**)&(newitem->MorphNorm), n * sizeof(D3DVECTOR*));
		DuplikData((void**)&(newitem->MorphUV), n * MAX_STAGES * sizeof(float*));

		int v = newitem->VertNum;

		for (i = 0; i < n; i++)
		{
			DuplikData((void**)&(newitem->MorphVert[i]), v * sizeof(D3DVECTOR));
			DuplikData((void**)&(newitem->MorphNorm[i]), v * sizeof(D3DVECTOR));

			for (j = 0; j < MAX_STAGES; j++)
			{
				DuplikData((void**)&(newitem->MorphUV[i*MAX_STAGES + j]), v * 2 * sizeof(float));
			}
		}
	}

// obrázek musí mít vlastní buffery souøadnic
	if ((newitem->Type == D3DFTYPE_PICTURE) ||
		(newitem->Type == D3DFTYPE_LENSFLARE))
	{
		D3DF_Modi(newindex);
	}

	return newindex;
}

////////////////////////////////////////////////////////////////////
// kopie položky s potomky (vrací index nové položky)

int D3DF_Copy(int oldindex)
{
// pro neplatnou položku se vytvoøí pouze skupina
	if ((oldindex <= 1) || !D3DF_IsValid(oldindex))
	{
		return D3DF_New();
	}

// duplikace výchozí položky
	int newindex = D3DF_Copy1(oldindex);

// duplikace všech potomkù
	int oldinx = oldindex;
	int newinx = newindex;
	int newpar = 0;

	D3DFITEM* olditem;
	D3DFITEM* newitem;

	for (;;)
	{

// adresa aktuální pùvodní položky
		olditem = D3DF_Data + oldinx;
		newitem = D3DF_Data + newinx;

// vnoøení do potomka
		int oldinx2 = olditem->Child;
		if (oldinx2 >= 0)
		{
			oldinx = oldinx2;					// index potomka
			newpar = newinx;					// nový rodiè vytváøené položky
		}

// jinak pøejítí k sousedu stejné úrovnì (pozn.: soused nemùže mít index=0, to je scéna)
		else
		{
			if (oldinx == oldindex) break;		// je to výchozí položka - konec
			oldinx = olditem->Next;				// následující soused

// není-li soused, pøejítí k rodièi a jeho následující položce
			while (oldinx < 0)					// není další soused?
			{
				oldinx = olditem->Parent;		// návrat k dalšímu rodièi
				newinx = newpar;

				if (oldinx == oldindex) break;	// je již opìt výchozí položka, konec

				olditem = D3DF_Data + oldinx;	// adresa rodièe
				newitem = D3DF_Data + newinx;
				newpar = newitem->Parent;
				oldinx = olditem->Next;			// další soused rodièe
			}

			if (oldinx == oldindex) break;	// je již opìt výchozí položka, konec
		}

// kopie položky
		newinx = D3DF_Copy1(oldinx);

// pøipojení položky k rodièi
		D3DF_Link(newpar, newinx);
	}

// kopie položky LOD
	olditem = D3DF_Data + oldindex;
	int lod = olditem->LODNext;

	if (lod >= 0)
	{
		float lodmax2 = olditem->LODMax2;

		int lodnext = D3DF_Copy(lod);

		newitem = D3DF_Data + newindex;
		newitem->LODNext = lodnext;

		olditem = D3DF_Data + lodnext;
		olditem->LODPrev = newindex;
		olditem->LODMin2 = lodmax2;
	}

	return newindex;
}

////////////////////////////////////////////////////////////////////
// zrušení všech položek v bufferu (ukládání zaène opìt po øadì)

void D3DF_DelAll()
{
// zrušení položek
	for (int i = D3DF_Max-1; i >= 0; i--)
	{
		D3DF_Del(i);
	}

// zrušení bufferu
	MemBuf(D3DF_Data, 0);		// zrušení bufferu dat
	MemBuf(D3DF_Valid, 0);		// zrušení bufferu platnosti
	D3DF_Max = 0;				// není žádná položka v bufferu
	D3DF_Next = -1;				// není pøíští položka
}

////////////////////////////////////////////////////////////////////
// zrušení bufferù morfování

void D3DF_MorphReset(D3DFITEM* item)
{
	int n = item->MorphMax;
	if (n > 0)
	{
		for (int i = 0; i < n; i++)
		{
			MemFree(item->MorphVert[i]);
			MemFree(item->MorphNorm[i]);

			for (int j = 0; j < MAX_STAGES; j++)
			{
				MemFree(item->MorphUV[i*MAX_STAGES + j]);
			}
		}

		MemFree(item->MorphVert); item->MorphVert = NULL;
		MemFree(item->MorphNorm); item->MorphNorm = NULL;
		MemFree(item->MorphUV); item->MorphUV = NULL;

		item->MorphMax = 0;
		item->MorphStage = 0;
	}
}

////////////////////////////////////////////////////////////////////
// zrušení položky (kontroluje platnost indexu)

void _fastcall D3DF_Del(const int index)
{
	if (D3DF_IsValid(index))					// je index platný?
	{

// adresa položky
		D3DFITEM* item = D3DF_Data + index;

// zrušení LOD objektù
		D3DF_Del(item->LODNext);

// odpojení od pøedcházejícího LOD
		int lod = item->LODPrev;
		if (lod >= 0)
		{
			D3DF_Data[lod].LODNext = -1;
			item->LODPrev = -1;
		}

// zrušení textury
		for (int j = 0; j < MAX_STAGES; j++)
		{
			D3DT_Del(item->Texture[j]);
			item->Texture[j] = -1;
		}

// zrušení svìtla
		D3DL_Del(item->Light);
		item->Light = -1;

// odpojení z øetìzce klonù
		int cloneprev = item->ClonePrev;
		int clonenext = item->CloneNext;

		if (cloneprev != item->Index)
		{
			D3DF_Data[cloneprev].CloneNext = clonenext;
			D3DF_Data[clonenext].ClonePrev = cloneprev;

			item->CloneNext = index;
			item->ClonePrev = index;

			pD3SReset(item);
		}
		else
		{

// zrušení bufferu terénu
			MemFree(item->TerMap);

// resetování objektu
			pD3Reset(item);

// zrušení bufferù vzoru objektu
			MemFree(item->Vertex);
			MemFree(item->Normal);
			MemFree(item->VertCol);
			MemFree(item->VertColF);

			for (j = 0; j < MAX_STAGES; j++)
			{
				MemFree(item->TextUV[j]);
			}

			MemFree(item->Face);
			MemFree(item->FaceW);

// zrušení bufferù stínù
			MemFree(item->ShadeVert);
			MemFree(item->ShadeFace);
			MemFree(item->ShadeNorm);
			MemFree(item->ShadeEdgeV);
			MemFree(item->ShadeEdgeN);
			MemFree(item->ShadeNext);
			MemFree(item->ShadeLeft);
		}

// zrušení ostatních bufferù
		MemFree(item->ShadeEdgeS);
		MemFree(item->ShadowFan);
		MemFree(item->ShadowVolume);
		MemFree(item->AutoMap);

// oznaèení bufferù jako zrušené
		item->TerMap = NULL;

		item->Vertex = NULL;
		item->Normal = NULL;
		item->VertCol = NULL;
		item->VertColF = NULL;

		for (j = 0; j < MAX_STAGES; j++)
		{
			item->TextUV[j] = NULL;
		}

		item->Face = NULL;
		item->FaceW = NULL;

		item->ShadeVert = NULL;
		item->ShadeFace = NULL;
		item->ShadeNorm = NULL;
		item->ShadeEdgeV = NULL;
		item->ShadeEdgeN = NULL;
		item->ShadeNext = NULL;
		item->ShadeLeft = NULL;
		item->ShadeEdgeS = NULL;
		item->ShadowFan = NULL;
		item->ShadowVolume = NULL;
		item->AutoMap = NULL;

// zrušení bufferù morfování
		D3DF_MorphReset(item);

// zrušení všech potomkù položky (kamera se jen odpojí)
		while (item->Child >= 0)
		{
			if (item->Child == 1)
			{
				D3DF_Link(0, 1);
			}
			else
			{
				D3DF_Del(item->Child); // zrušení potomkù
			}
		}

// odpojení od øetìzce aktualizací matic
		D3DF_AktMatUnlink(item);

// navázání na další položku z pøedcházející položky
		int prev = item->Prev;						// pøedcházející položka
		int next = item->Next;						// následující položka
		if (prev >= 0)
		{
			D3DF_Data[prev].Next = next;			// napojení na další položku
		}

// navázání na pøedcházející položku z další položky
		if (next >= 0)
		{
			D3DF_Data[next].Prev = prev;			// napojení na pøedcházející položku
		}

// navázání na nového potomka z rodièe
		int par = item->Parent;						// rodiè
		if (par >= 0)
		{
			D3DFITEM* itempar = D3DF_Data + par;	// adresa rodièe
			
			if (itempar->Child == index)
			{
				itempar->Child = next;				// napojení na dalšího potomka
			}
		}

// zrušení platnosti položky
		*(int*)item = D3DF_Next;				// pøíští volná položka
		D3DF_Valid[index] = false;				// zrušení pøíznaku platnosti
		D3DF_Next = index;						// odkaz na tuto položku
	}
}

////////////////////////////////////////////////////////////////////
// resetování cachování bufferu (pøed ukonèením ovladaèe)

void D3DF_Reset()
{
	D3DSrcBlend = BLEND_INVALID; // aktuální nastavení blending operace zdroje
	D3DDstBlend = BLEND_INVALID; // aktuální nastavení blending operace cíle
	D3DAlphaRef = -1;			// aktuální nastavení alfa úrovnì
	D3DTFactor = MAXDWORD;		// aktuální nastavení faktoru textur
	D3DZWrite = -1;				// aktuální hodnota hloubkového zápisu
	D3DZTest = -1;				// aktuální hodnota hloubkového testu

	int i;
	for (i = 0; i < MAX_STAGES; i++)
	{
		D3DColorOp[i] = -1;				// aktuální nastavení operací barev
		D3DColorArg1[i] = -1;			// aktuální nastavení argumentu 1 operací barev
		D3DColorArg2[i] = -1;			// aktuální nastavení argumentu 2 operací barev
		D3DAlphaOp[i] = -1;				// aktuální nastavení operací barev
		D3DAlphaArg1[i] = -1;			// aktuální nastavení argumentu 1 operací barev
		D3DAlphaArg2[i] = -1;			// aktuální nastavení argumentu 2 operací barev
		D3DAddressU[i] = -1;			// aktuální adresování textur U
		D3DAddressV[i] = -1;			// aktuální adresování textur V
//		D3DBorder[i] = -1;				// aktuální barva okolí textur
		D3DBias[i] = -123456;			// aktuální zjemnìní vzdálených textur
	}

	for (i = D3DF_Max-1; i >= 0; i--)
	{
		if (D3DF_Valid[i])
		{
			D3DFITEM* item = D3DF_Data + i;
			pD3Reset(item);
		}
	}
}

////////////////////////////////////////////////////////////////////
// inicializace manažeru vrcholù (index položky musí být platný!)

D3DFITEM*	UsedItem = NULL;		// adresa položky
int			UsedVert = 0;			// poèet aktuálních vrcholù
int			UsedMax = 1;			// velikost bufferù vrcholù
bool*		UsedMap = NULL;			// mapa použitých vrcholù

void D3DF_UsedInit(int inx)
{
	D3DF_Modi(inx);
	UsedItem = D3DF_Get(inx);
	D3DF_AktMatLink(UsedItem);
	UsedVert = UsedItem->VertNum;
	UsedMax = 1;
	MemFree(UsedMap);
	UsedMap = (bool*)MemGet(UsedVert * sizeof(bool));
	MemFill(UsedMap, UsedVert * sizeof(bool), false);
}

////////////////////////////////////////////////////////////////////
// duplikace vrcholu (parametr = index vrcholu plošky), vrací nový index

int D3DF_UsedDuplik(int faceinx)
{

// index duplikované položky
	int vert = UsedItem->Face[faceinx];
	if (UsedItem->MorphMax > 0) return vert;	// pro morfovací buffery zákaz operace
	int newvert = UsedVert;
	UsedVert++;
	UsedItem->VertNum = UsedVert;
	UsedItem->Face[faceinx] = newvert;

	UsedItem->AktSector = TRUE;

// zvýšení velikosti bufferù
	if (UsedMax <= UsedVert)
	{
		while (UsedMax <= newvert) UsedMax = (UsedMax << 1);

		MemBuf(UsedItem->Vertex, UsedMax);
		MemBuf(UsedMap, UsedMax);

		if (UsedItem->Normal != NULL)
		{
			MemBuf(UsedItem->Normal, UsedMax);
		}

		if (UsedItem->VertCol != NULL)
		{
			MemBuf(UsedItem->VertCol, UsedMax);
		}

		if (UsedItem->VertColF != NULL)
		{
			MemBuf(UsedItem->VertColF, UsedMax);
		}

		for (int i = 0; i < MAX_STAGES; i++)
		{
			if (UsedItem->TextUV[i] != NULL)
			{
				MemBuf(UsedItem->TextUV[i], UsedMax*2);
			}
		}
	}

// pøenesení dat starého vrcholu
	UsedItem->Vertex[newvert] = UsedItem->Vertex[vert];

	if (UsedItem->Normal != NULL)
	{
		UsedItem->Normal[newvert] = UsedItem->Normal[vert];
	}

	if (UsedItem->VertCol != NULL)
	{
		UsedItem->VertCol[newvert] = UsedItem->VertCol[vert];
	}

	if (UsedItem->VertColF != NULL)
	{
		UsedItem->VertColF[newvert] = UsedItem->VertColF[vert];
	}

	for (int i = 0; i < MAX_STAGES; i++)
	{
		if (UsedItem->TextUV[i] != NULL)
		{
			UsedItem->TextUV[i][newvert*2] = UsedItem->TextUV[i][vert*2];
			UsedItem->TextUV[i][newvert*2+1] = UsedItem->TextUV[i][vert*2+1];
		}
	}

	UsedMap[newvert] = true;

	return newvert;
}

////////////////////////////////////////////////////////////////////
// nastavení normály vrcholu (pro daný index vrcholu plošky)

void D3DF_UsedNormal(int faceinx, float nx, float ny, float nz)
{
// index vrcholu
	int vert = UsedItem->Face[faceinx];

// vrchol je použitý
	if (UsedMap[vert])
	{

// test, zda se data liší
		if (((nx != UsedItem->Normal[vert].x) ||
			(ny != UsedItem->Normal[vert].y) ||
			(nz != UsedItem->Normal[vert].z)) &&
			(UsedVert < 0x10000))
		{
			int newvert = D3DF_UsedDuplik(faceinx);

			UsedItem->Normal[newvert].x = nx;
			UsedItem->Normal[newvert].y = ny;
			UsedItem->Normal[newvert].z = nz;
		}
	}

// vrchol není použitý
	else
	{
		UsedItem->Normal[vert].x = nx;
		UsedItem->Normal[vert].y = ny;
		UsedItem->Normal[vert].z = nz;
		UsedMap[vert] = true;
	}
}

////////////////////////////////////////////////////////////////////
// nastavení barvy vrcholu (pro daný index vrcholu plošky)

void D3DF_UsedColorB(int faceinx, BYTE r, BYTE g, BYTE b, BYTE a)
{
	D3DF_UsedColor(
		faceinx,
		((a << 24) | (r << 16) | (g << 8) | b),
		(float)((float)r/255),
		(float)((float)g/255),
		(float)((float)b/255),
		(float)((float)a/255)
		);
}

void D3DF_UsedColor(int faceinx, DWORD col, float r, float g, float b, float a)
{
// index vrcholu
	int vert = UsedItem->Face[faceinx];

// vrchol je použitý
	if (UsedMap[vert])
	{

// test, zda se data liší
		if (((col != UsedItem->VertCol[vert]) ||
			(r != UsedItem->VertColF[vert].r) ||
			(g != UsedItem->VertColF[vert].g) ||
			(b != UsedItem->VertColF[vert].b) ||
			(a != UsedItem->VertColF[vert].a)) &&
			(UsedVert < 0x10000))
		{
			int newvert = D3DF_UsedDuplik(faceinx);

			UsedItem->VertCol[newvert] = col;
			UsedItem->VertColF[newvert].r = r;
			UsedItem->VertColF[newvert].g = g;
			UsedItem->VertColF[newvert].b = b;
			UsedItem->VertColF[newvert].a = a;
		}
	}

// vrchol není použitý
	else
	{
		UsedItem->VertCol[vert] = col;
		UsedItem->VertColF[vert].r = r;
		UsedItem->VertColF[vert].g = g;
		UsedItem->VertColF[vert].b = b;
		UsedItem->VertColF[vert].a = a;

		UsedMap[vert] = true;
	}
}

////////////////////////////////////////////////////////////////////
// nastavení adresy textury vrcholu (pro daný index vrcholu plošky)

void D3DF_UsedTextUV(int faceinx, float u, float v, int stage)
{
// index vrcholu
	int vert = UsedItem->Face[faceinx];

// vrchol je použitý
	if (UsedMap[vert])
	{

// test, zda se data liší
		if (((u != UsedItem->TextUV[stage][2*vert]) ||
			(v != UsedItem->TextUV[stage][2*vert+1])) &&
			(UsedVert < 0x10000))
		{
			int newvert = D3DF_UsedDuplik(faceinx);

			UsedItem->TextUV[stage][2*newvert] = u;
			UsedItem->TextUV[stage][2*newvert+1] = v;
		}
	}

// vrchol není použitý
	else
	{
		UsedItem->TextUV[stage][2*vert] = u;
		UsedItem->TextUV[stage][2*vert+1] = v;

		UsedMap[vert] = true;
	}
}

////////////////////////////////////////////////////////////////////
// napojení položky na rodièe (s odpojením od pøedcházejícího rodièe)
// kontroluje platnost obou indexù

void _fastcall D3DF_Link(const int parent, const int child)
{

// kontrola platnosti položek a zda není položka již pøipojena
	if (D3DF_IsValid(parent) && 
		D3DF_IsValid(child) && 
		(parent != child) &&
		(child != 0) &&
		(D3DF_Data[child].Parent != parent))
	{

// kontrola, zda není cyklické propojení položek
		int i = child;

		do 
		{
			D3DFITEM* item = D3DF_Data + i;

			int i2 = item->Child;
			if (i2 >= 0)
			{
				i = i2;
			}
			else
			{
				if (i == child) break;

				i = item->Next;

				while (i < 0)
				{
					i = item->Parent;
					if (i == child)break;

					item = D3DF_Data + i;
					i = item->Next;
				}
			}
			if (i == parent) return;

		} while (i != child);

// odpojení potomka ze starého místa
		D3DFITEM* itemchild = D3DF_Data + child;	 // adresa potomka

		int prev = itemchild->Prev;					// pøedcházející položka
		int next = itemchild->Next;					// následující položka
		if (prev >= 0)
		{
			D3DF_Data[prev].Next = next;			// napojení na další položku
		}

		if (next >= 0)
		{
			D3DF_Data[next].Prev = prev;			// napojení na pøedcházející položku
		}

		int par = itemchild->Parent;				// starý rodiè
		if (par >= 0)
		{
			D3DFITEM* itempar = D3DF_Data + par;	// adresa rodièe
			
			if (itempar->Child == child)
			{
				itempar->Child = next;				// napojení na dalšího potomka
			}
		}

// napojení potomka do nového místa
		D3DFITEM* itempar = D3DF_Data + parent;		// adresa nového rodièe

		next = itempar->Child;						// pùvodní potomek rodièe
		itempar->Child = child;						// pøipojení nového potomka
		itempar->AktBound = true;					// aktualizovat hranici s potomky
		D3DF_AktMatLink(itempar);
		itemchild->Parent = parent;					// nový rodiè položky
		itemchild->Next = next;						// další položka
		itemchild->Prev = -1;						// není pøedcházející položka

		itemchild->AktWorld = true;					// je nutné aktualizovat transformace
		D3DF_AktMatLink(itemchild);

		if (next >= 0)
		{
			D3DF_Data[next].Prev = child;			// je to pøedcházející položka pùvodního potomka
		}

// pøipojení sousedních stupòù LOD (tento je již pøipojen = nebude zacyklení)
		int lod = itemchild->LODPrev;
		if (lod >= 0) D3DF_Link(parent, lod);
		lod = itemchild->LODNext;
		if (lod >= 0) D3DF_Link(parent, lod);
	}
}

////////////////////////////////////////////////////////////////////
// zmapování vlastní hranice (index musí být platný)

void D3DF_MapBound(int index)
{
	ASSERT(D3DF_IsValid(index));

// adresa rámu
	D3DFITEM* item = D3DF_Data + index;

// cyklus pøes všechny vrcholy
	double r = 0;
	double d;
	double x;
	double y;
	double z;

	D3DVECTOR* v = item->Vertex;
	for (int i = item->VertNum; i > 0; i--)
	{
		x = v->x;
		y = v->y;
		z = v->z;
		d = x*x + y*y + z*z;
		if (d > r) r = d;

		v++;
	}

// nastavení nového polomìru hranice
	item->BoundR0 = sqrt(r);
	item->AktBound = true;

	D3DF_AktMatLink(item);
}


////////////////////////////////////////////////////////////////////
// vytvoøení objektu (vrací index objektu, pro 0 vrcholù to bude skupina)

int D3DF_CreateMesh(int vertN, D3DVECTOR* vert, int faceN, int* face)
{
// vytvoøení nového rámu
	int inx = D3DF_New();
	D3DFITEM* item = D3DF_Data + inx;

// kontrola poètu vrcholù a plošek (pøi chybì zùstane jako skupina)
	if ((vertN < 3) || (faceN < 1)) return inx;

// vytvoøení bufferù pro vrcholy
	item->VertNum = vertN;
	item->Vertex = (D3DVECTOR*)MemGet(vertN * sizeof(D3DVECTOR));
	MemCopy(item->Vertex, vert, vertN * sizeof(D3DVECTOR));

// vytvoøení bufferù pro plošky
	item->FaceNum = faceN;
	item->Face = (int*)MemGet(faceN * 3 * sizeof(int));
	MemCopy(item->Face, face, faceN * 3 * sizeof(int));

// požadavek aktualizace objektu
//	item->AktMat = true;
	item->Type = D3DFTYPE_OBJECT;

// aktualizovat sektory
	item->AktSector = TRUE;

	D3DF_AktMatLink(item);

// zmapování vlastní hranice
	D3DF_MapBound(inx);

	return inx;
}

////////////////////////////////////////////////////////////////////
// nastavení normál objektu (poèet normál musí souhlasit s poètem vrcholù!)

void D3DF_SetNormal(int index, D3DVECTOR* norm)
{
	ASSERT(D3DF_IsValid(index));

// adresa rámu
	D3DFITEM* item = D3DF_Data + index;
	ASSERT(item->VertNum > 0);

// vytvoøení bufferu pro vektory normál
	int size = item->VertNum * sizeof(D3DVECTOR);
	if (item->Normal == NULL)
	{
		item->Normal = (D3DVECTOR*)MemGet(size);
	}

// uložení bufferu normál (s normalizací normál)
	D3DVECTOR* dst = item->Normal;
	D3DVECTOR* src = norm;

	for (int i = item->VertNum; i > 0; i--)
	{
		double x = src->x;
		double y = src->y;
		double z = src->z;

		double n = sqrt(x*x + y*y + z*z);
		if (n == 0)
		{
			z = 1;
			n = 1;
		}

		dst->x = (float)(x/n);
		dst->y = (float)(y/n);
		dst->z = (float)(z/n);

		src++;
		dst++;
	}

// aktualizovat sektory
	item->AktSector = TRUE;

	D3DF_AktMatLink(item);
}

////////////////////////////////////////////////////////////////////
// korekce novì vytvoøeného aktuálního objektu pro pravoruký souøadný systém

void D3DF_KorRightHand()
{
	if (D3DRightHand && IsValidID())
	{
		D3DFITEM* item = GetID();

		D3DVECTOR* v = item->Vertex;
		D3DVECTOR* n = item->Normal;

		for (int i = item->VertNum; i > 0; i--)
		{
			v->z = -(v->z);
			n->z = -(n->z);

			v++;
			n++;
		}

// aktualizovat sektory
		item->AktSector = TRUE;

		D3DF_AktMatLink(item);
	}
}

////////////////////////////////////////////////////////////////////
// nastavení barev vrcholù objektu (poèet musí souhlasit s poètem vrcholù!)

void D3DF_SetVertCol(int index, D3DCOLORVALUE* col)
{
	ASSERT(D3DF_IsValid(index));

// adresa rámu
	D3DFITEM* item = D3DF_Data + index;
	int n = item->VertNum;
	ASSERT(n > 0);

// zrušení bufferu vrcholù (musí se pøegenerovat)
	pD3Reset(item);

// vytvoøení bufferù pro barvy
	DWORD* dst = item->VertCol;
	if (dst == NULL)
	{
		dst = (DWORD*)MemGet(n * sizeof(DWORD));
		item->VertCol = dst;
	}

	D3DCOLORVALUE* dst2 = item->VertColF;
	if (dst2 == NULL)
	{
		dst2 = (D3DCOLORVALUE*)MemGet(n * sizeof(D3DCOLORVALUE));
		item->VertColF = dst2;
	}

// uložení bufferù barev
	for (; n > 0; n--)
	{
		double r = col->r;
		if (r < 0) r = 0;
		if (r > 1) r = 1;
		dst2->r = (float)r;
		*dst = (Round(r*255) << 16);

		double g = col->g;
		if (g < 0) g = 0;
		if (g > 1) g = 1;
		dst2->g = (float)g;
		*dst = *dst | (Round(g*255) << 8);

		double b = col->b;
		if (b < 0) b = 0;
		if (b > 1) b = 1;
		dst2->b = (float)b;
		*dst = *dst | Round(b*255);

		double a = col->a;
		if (a < 0) a = 0;
		if (a > 1) a = 1;
		dst2->a = (float)a;
		*dst = *dst | (Round(a*255) << 24);

		col++;
		dst++;
		dst2++;
	}

// zrušení pøíznaku automatické barvy
	item->AutoCol = MAXDWORD;

// aktualizovat sektory
	item->AktSector = TRUE;

	D3DF_AktMatLink(item);
}

////////////////////////////////////////////////////////////////////
// nastavení mapování textur (poèet musí souhlasit s poètem vrcholù!)

void D3DF_SetTextUV(int index, float* textuv, int stage)
{
	ASSERT(D3DF_IsValid(index));

// adresa rámu
	D3DFITEM* item = D3DF_Data + index;
	int n = item->VertNum;
	ASSERT(n > 0);

// vytvoøení bufferù pro mapování textur
	float* dst = item->TextUV[stage];
	int size = n * 2 * sizeof(float);
	if (dst == NULL)
	{
		dst = (float*)MemGet(size);
		item->TextUV[stage] = dst;
	}

// pøenesení mapování textur
	MemCopy(dst, textuv, size);

// aktualizovat sektory
	item->AktSector = TRUE;

	D3DF_AktMatLink(item);

// resetování objektu
	pD3Reset(item);
}

////////////////////////////////////////////////////////////////////
// nastavení adresování a zalamování textury

void D3DF_SetAddress(int index, int adru, int adrv, int stage)
{
	if (D3DF_IsValid(index))
	{
		D3DFITEM* item = D3DF_Data + index;

		item->AddressU[stage] = adru;
		item->AddressV[stage] = adrv;
	}
}

////////////////////////////////////////////////////////////////////
// nastavení blending módu bìhem renderování

void D3DF_SetBlendMode(BLENDTYPE srcblend, BLENDTYPE dstblend)
{
	if ((srcblend != D3DSrcBlend) || (dstblend != D3DDstBlend))
	{
		D3DFITEM item;

		int blendmode = (int)srcblend + 10*(int)dstblend;

		item.SrcDstBlend = blendmode;
		item.DstBlend = dstblend;
		item.SrcBlend = srcblend;
		D3DSrcBlend = srcblend;
		D3DDstBlend = dstblend;

		item.IsBlend = (blendmode != 1);

		item.BlendRend = (	(dstblend != BLEND_ZERO) ||
							(srcblend >= BLEND_DSTCOLOR));

		pD3AktBlend(&item);
	}
}


////////////////////////////////////////////////////////////////////
// nastavení blending módu (1=normal, 11=oheò, 20=sklo, 54=pøekryv)

void D3DF_SetBlend(int index, int blendmode)
{
	if (D3DF_IsValid(index))
	{
		D3DFITEM* item = D3DF_Data + index;

		if ((DWORD)blendmode > (DWORD)99) blendmode = 1;

		item->SrcDstBlend = blendmode;
		item->DstBlend = (BLENDTYPE)(blendmode / 10);
		item->SrcBlend = (BLENDTYPE)(blendmode % 10);

		item->IsBlend = (blendmode != 1);

		item->BlendRend = (	(item->DstBlend != BLEND_ZERO) ||
							(item->SrcBlend >= BLEND_DSTCOLOR));
	}
}

////////////////////////////////////////////////////////////////////
// vygenerování normál (kontroluje index)

void D3DF_AutoNormals(int index, double angle)
{
	if (D3DF_IsValid(index))
	{

// odpojení dat od klonù
		D3DF_Modi(index);

		D3DFITEM* item = D3DF_Data + index;

		if ((item->VertNum > 0) && (item->FaceNum > 0))
		{

// vytvoøení bufferu pro vektory normál
			if (item->Normal == NULL)
			{
				item->Normal = (D3DVECTOR*)MemGet(item->VertNum * sizeof(D3DVECTOR));
			}

// aktualizovat sektory
			item->AktSector = TRUE;

			D3DF_AktMatLink(item);

			if (item->AutoMap != NULL)
			{
				MemFill(&(item->AutoMap->m), sizeof(D3DMATRIX), 0);
			}

// pøíprava manažeru vrcholù
			D3DF_UsedInit(index);

// výpoèet normál plošek
			int i;
			int fN = item->FaceNum * 3;

			for (i = 0; i < fN;)
			{
				int j1 = item->Face[i];
				int j2 = item->Face[i+1];
				int j3 = item->Face[i+2];

				D3DVECTOR* v1 = item->Vertex + j1;
				D3DVECTOR* v2 = item->Vertex + j2;
				D3DVECTOR* v3 = item->Vertex + j3;

				double ax = v2->x - v1->x;
				double ay = v2->y - v1->y;
				double az = v2->z - v1->z;

				double bx = v3->x - v2->x;
				double by = v3->y - v2->y;
				double bz = v3->z - v2->z;

				double nx = ay*bz - az*by;
				double ny = az*bx - ax*bz;
				double nz = ax*by - ay*bx;

				double r = sqrt(nx*nx + ny*ny + nz*nz);
				if (r == 0)
				{
					r = 1;
					ny = 1;
				}

				r = 1/r;
				float nx0 = (float)(nx * r);
				float ny0 = (float)(ny * r);
				float nz0 = (float)(nz * r);

				D3DF_UsedNormal(i, nx0, ny0, nz0);
				i++;
				D3DF_UsedNormal(i, nx0, ny0, nz0);
				i++;
				D3DF_UsedNormal(i, nx0, ny0, nz0);
				i++;
			}

// normalizace referenèního úhlu
			while (angle >= pi2) angle -= pi2;
			while (angle < 0) angle += pi2;
			if (angle >= pi) angle = pi2 - angle;

// úschova normál
			if (angle != 0)
			{
				double cosa = cos(angle);

				int vN = item->VertNum;
				D3DVECTOR* norm0 = (D3DVECTOR*)MemGet(vN * sizeof(D3DVECTOR));
				MemCopy(norm0, item->Normal, vN * sizeof(D3DVECTOR));

// buffer použitých ploch
// ... nebylo vidìt žádné zlepšení a vznikaly zlomy na málo odchýlených plochách
//#define USEDMAX 100

//				D3DVECTOR* used = (D3DVECTOR*)MemGet(USEDMAX * sizeof(D3DVECTOR));
//				int usedN;

// cyklus pøes všechny vrcholy
				for (i = 0; i < vN; i++)
				{
					D3DVECTOR* v = item->Vertex + i;
					D3DVECTOR* n1 = norm0 + i;

					D3DVECTOR nn = *n1;

					BOOL smooth = FALSE;

//					usedN = 1;
//					used[0] = *n1;

// vyhledání stejných vrcholù
					D3DVECTOR* v2 = item->Vertex;

					for (int i2 = 0; i2 < vN; i2++)
					{
						if ((i2 != i) &&
//							((fabs(v->x - v2->x) + fabs(v->y - v2->y) + fabs(v->z - v2->z)) < 0.01))
							(*(DWORD*)(&v->x) == *(DWORD*)(&v2->x)) && 
							(*(DWORD*)(&v->y) == *(DWORD*)(&v2->y)) && 
							(*(DWORD*)(&v->z) == *(DWORD*)(&v2->z)))
						{

// výpoèet úhlu normál
							D3DVECTOR* n2 = norm0 + i2;

//							double a = acos(n1->x*n2->x + n1->y*n2->y + n1->z*n2->z);
							double a = n1->x*n2->x + n1->y*n2->y + n1->z*n2->z;

//							if (a <= angle)
							if (a >= cosa)
							{

// kontrola, zda byla rovina již použita
//								BOOL used0 = FALSE;

//								for (int u = 0; u < usedN; u++)
//								{
//									if ((used[u].x*n2->x + used[u].y*n2->y + used[u].z*n2->z) > 0.9999)
//									{
//										used0 = TRUE;
//										break;
//									}
//								}

// pøidání normály
//								if (!used0)
//								{
//									if (usedN < USEDMAX)
//									{
//										used[usedN] = *n2;
//										usedN++;
//									}
//
									nn.x = nn.x + n2->x;
									nn.y = nn.y + n2->y;
									nn.z = nn.z + n2->z;								
									smooth = TRUE;
//								}
							}
						}

						v2++;
					}

// normalizace normály
					if (smooth)
					{
						double r = sqrt(nn.x*nn.x + nn.y*nn.y + nn.z*nn.z);
						if (r == 0)
						{
							r = 1;
							nn.y = 1;
						}
						r = 1/r;

						nn.x = (float)(nn.x * r);
						nn.y = (float)(nn.y * r);
						nn.z = (float)(nn.z * r);

						item->Normal[i] = nn;
					}
				}

// zrušení bufferu úschovy normál
				MemFree(norm0);
//				MemFree(used);
			}
		}


// obsluha potomkù
		int nxt = item->Child;

		while (nxt >= 0)
		{
			D3DF_AutoNormals(nxt, angle);
			nxt = D3DF_Data[nxt].Next;
		}
	}
}

////////////////////////////////////////////////////////////////////
// vygenerování automatické barvy, je-li vypnuto osvìtlení

void D3DF_GenAutoCol(D3DFITEM* item)
{
	if (((item->AutoCol != MAXDWORD) && (item->AutoCol != item->Diffuse)) ||
		(item->AutoCol == MAXDWORD) && (item->VertCol == NULL))
	{
		int n = item->VertNum;
		ASSERT(n > 0);

// zrušení bufferu vrcholù (musí se pøegenerovat)
		pD3Reset(item);

// vytvoøení bufferù pro barvy
		DWORD* dst = item->VertCol;
		if (dst == NULL)
		{
			dst = (DWORD*)MemGet(n * sizeof(DWORD));
			item->VertCol = dst;
		}

		D3DCOLORVALUE* dst2 = item->VertColF;
		if (dst2 == NULL)
		{
			dst2 = (D3DCOLORVALUE*)MemGet(n * sizeof(D3DCOLORVALUE));
			item->VertColF = dst2;
		}

// pøíprava barvy
		DWORD dif = item->Diffuse;
		if (dif == MAXDWORD) dif--;
		item->AutoCol = dif;
		DWORD col = XRGB(dif);
		D3DCOLORVALUE colf;
		BGRFloat(dif, &colf);

// uložení bufferù barev
		for (; n > 0; n--)
		{
			*dst = col;
			*dst2 = colf;
			dst++;
			dst2++;
		}

// nastavení adres i pro klony
		dst = item->VertCol;
		dst2 = item->VertColF;

		D3DFITEM* item2 = item;

		for (;;)
		{
			item2 = D3DF_Data + item2->CloneNext;
			if (item2 == item) break;

			item2->VertCol = dst;
			item2->VertColF = dst2;
			item2->AutoCol = dif;
		}
	}
}

////////////////////////////////////////////////////////////////////
// tabulka blending operací pro Direct3D

int	BlendTabD3D[11] =
{
	D3DBLEND_ZERO,			//	BLEND_ZERO			= 0,	// (0, 0, 0, 0)
	D3DBLEND_ONE,			//	BLEND_ONE			= 1,	// (1, 1, 1, 1)
	D3DBLEND_SRCCOLOR,		//	BLEND_SRCCOLOR		= 2,	// (Rs, Gs, Bs, As)
	D3DBLEND_INVSRCCOLOR,	//	BLEND_INVSRCCOLOR	= 3,	// (1-Rs, 1-Gs, 1-Bs, 1-As)
	D3DBLEND_SRCALPHA,		//	BLEND_SRCALPHA		= 4,	// (As, As, As, As)
	D3DBLEND_INVSRCALPHA,	//	BLEND_INVSRCALPHA	= 5,	// (1-As, 1-As, 1-As, 1-As)
	D3DBLEND_DESTCOLOR,		//	BLEND_DSTCOLOR		= 6,	// (Rd, Gd, Bd, Ad)
	D3DBLEND_INVDESTCOLOR,	//	BLEND_INVDSTCOLOR	= 7,	// (1-Rd, 1-Gd, 1-Bd, 1-Ad)
	D3DBLEND_DESTALPHA,		//	BLEND_DSTALPHA		= 8,	// (Ad, Ad, Ad, Ad)
	D3DBLEND_INVDESTALPHA,	//	BLEND_INVDSTALPHA	= 9,	// (1-Ad, 1-Ad, 1-Ad, 1-Ad)
	D3DBLEND_SRCALPHASAT	//	BLEND_SATURATE		= 10,	// menší ze složek SRC/DST
};

////////////////////////////////////////////////////////////////////
// tabulka blending operací pro OpenGL

int	BlendTabOGL[11] =
{
	GL_ZERO,				//	BLEND_ZERO			= 0,	// (0, 0, 0, 0)
	GL_ONE,					//	BLEND_ONE			= 1,	// (1, 1, 1, 1)
	GL_SRC_COLOR,			//	BLEND_SRCCOLOR		= 2,	// (Rs, Gs, Bs, As)
	GL_ONE_MINUS_SRC_COLOR,	//	BLEND_INVSRCCOLOR	= 3,	// (1-Rs, 1-Gs, 1-Bs, 1-As)
	GL_SRC_ALPHA,			//	BLEND_SRCALPHA		= 4,	// (As, As, As, As)
	GL_ONE_MINUS_SRC_ALPHA,	//	BLEND_INVSRCALPHA	= 5,	// (1-As, 1-As, 1-As, 1-As)
	GL_DST_COLOR,			//	BLEND_DSTCOLOR		= 6,	// (Rd, Gd, Bd, Ad)
	GL_ONE_MINUS_DST_COLOR,	//	BLEND_INVDSTCOLOR	= 7,	// (1-Rd, 1-Gd, 1-Bd, 1-Ad)
	GL_DST_ALPHA,			//	BLEND_DSTALPHA		= 8,	// (Ad, Ad, Ad, Ad)
	GL_ONE_MINUS_DST_ALPHA,	//	BLEND_INVDSTALPHA	= 9,	// (1-Ad, 1-Ad, 1-Ad, 1-Ad)
	GL_SRC_ALPHA_SATURATE	//	BLEND_SATURATE		= 10,	// menší ze složek SRC/DST
};

////////////////////////////////////////////////////////////////////
// pøíprava bodu dopadu stínu

void PrepShadowPoint(D3DFITEM* item, D3DVECTOR* posun)
{
	if (item->ShadowParent && (item->Parent >= 0))
	{
		D3DFITEM* par = D3DF_Get(item->Parent);

		if ((par->ShadowPoint.x != 0) ||
			(par->ShadowPoint.y != 0) ||
			(par->ShadowPoint.z != 0))
		{
			posun->x = par->ShadowPoint.x - item->MatrixWorld._41 + par->MatrixWorld._41;
			posun->y = par->ShadowPoint.y - item->MatrixWorld._42 + par->MatrixWorld._42;
			posun->z = par->ShadowPoint.z - item->MatrixWorld._43 + par->MatrixWorld._43;
			return;
		}
	}

	D3DLITEM* l = D3DL_Get(0);

	if (l->Light.Type == D3DLIGHT_DIRECTIONAL)
	{
		posun->x = l->Light.Direction.x;
		posun->y = l->Light.Direction.y;
		posun->z = l->Light.Direction.z;
	}
	else
	{
		posun->x = (float)(item->MatrixWorld._41 - l->Light.Position.x);
		posun->y = (float)(item->MatrixWorld._42 - l->Light.Position.y);
		posun->z = (float)(item->MatrixWorld._43 - l->Light.Position.z);
	}

	posun->x = (float)(item->ShadowRange * posun->x);
	posun->y = (float)(item->ShadowRange * posun->y);
	posun->z = (float)(item->ShadowRange * posun->z);
}

////////////////////////////////////////////////////////////////////
// renderování stínu (rodiè musí být již obsloužený)

void D3DF_RendShadow(D3DFITEM* item)
{
// pøíprava vektoru dopadu
	D3DVECTOR posun;
	PrepShadowPoint(item, &posun);

// test, zda je vektor dopadu zmìnìn
	if ((*(int*)(&posun.x) != *(int*)(&item->ShadowPoint.x)) ||
		(*(int*)(&posun.y) != *(int*)(&item->ShadowPoint.y)) ||
		(*(int*)(&posun.z) != *(int*)(&item->ShadowPoint.z)) ||
		(item->AktWorld) ||
		(item->ShadowFanN == 0))
	{
		item->ShadowPoint.x = posun.x;
		item->ShadowPoint.y = posun.y;
		item->ShadowPoint.z = posun.z;

// transformace vzdálenosti na souøadnice objektu
		posun.x = (float)(item->MatrixWorld._41 + posun.x);
		posun.y = (float)(item->MatrixWorld._42 + posun.y);
		posun.z = (float)(item->MatrixWorld._43 + posun.z);

		D3DMATRIX m;
		MatrixTrans(&m, &(item->MatrixWorld));
		MatrixInv(&m, &m);
		MatrixTrans(&m, &m);

		VecXMat(&posun, &posun, &m);

// pøíprava vektoru svìtla
		D3DVECTOR smer;

		if (item->Culling == 2)	// viditelná zadní strana plošky
		{
			smer.x = posun.x;
			smer.y = posun.y;
			smer.z = posun.z;
		}
		else
		{
			smer.x = -posun.x;
			smer.y = -posun.y;
			smer.z = -posun.z;
		}

		float d = (float)sqrt(smer.x*smer.x + smer.y*smer.y + smer.z*smer.z);
		if (d != 0)
		{
			smer.x = (float)(smer.x / d);
			smer.y = (float)(smer.y / d);
			smer.z = (float)(smer.z / d);
		}
		else
		{
			smer.y = -1;
		}

// zrušení starých bufferù
		MemFree(item->ShadowFan); item->ShadowFan = NULL;
		MemFree(item->ShadowVolume); item->ShadowVolume = NULL;

// zrušení bufferu stínu
		pD3SReset(item);

// je pøegenerování objektu
		int i;
		if (item->ShadeVert == NULL)
		{
			if (item->FaceNum == 0) return;

// zrušení starých bufferù
			MemFree(item->ShadeFace);
			MemFree(item->ShadeNorm);
			MemFree(item->ShadeEdgeV);
			MemFree(item->ShadeEdgeN);
			MemFree(item->ShadeEdgeS);
			MemFree(item->ShadeNext);
			MemFree(item->ShadeLeft);

// vytvoøení nových bufferù
			i = item->FaceNum;
			item->ShadeFace = (int*)MemGet(i*3 * sizeof(int));
			item->ShadeNorm = (D3DVECTOR*)MemGet(i * sizeof(D3DVECTOR));
			item->ShadeEdgeV = (int*)MemGet(i*3*2 * sizeof(int));
			item->ShadeEdgeN = (int*)MemGet(i*3 * sizeof(int));
			item->ShadeEdgeS = (char*)MemGet(i*3 * sizeof(char));
			item->ShadeNext = (int*)MemGet(i*3 * sizeof(int));
			item->ShadeLeft = (int*)MemGet(i*3 * sizeof(int));

// inicializace bufferù vrcholù a plošek
			D3DVECTOR* vert = (D3DVECTOR*)MemGet(item->VertNum * sizeof(D3DVECTOR));
			int* face = item->ShadeFace;
			int* facesrc = item->Face;
			int vertn = 0;

			for (i=i*3; i>0; i--)
			{
				int n = *facesrc;
				facesrc++;

				ASSERT(n >= 0);
				ASSERT(n < item->VertNum);

				D3DVECTOR* v = item->Vertex + n;

				n = vertn - 1;

				D3DVECTOR* v2 = vert + n;

				for (; n >= 0; n--)
				{
					if ((((int*)v)[0] == ((int*)v2)[0]) &&
						(((int*)v)[1] == ((int*)v2)[1]) &&
						(((int*)v)[2] == ((int*)v2)[2]))
					{
						break;
					}

					v2--;
				}

				if (n < 0)
				{
					vert[vertn] = *v;
					n = vertn;
					vertn++;
				}

				*face = n;
				face++;
			}

			item->ShadeVertN = vertn;
			vert = (D3DVECTOR*)MemSize(vert, vertn * sizeof(D3DVECTOR));
			item->ShadeVert = vert;

// výpoèet normál plošek
			D3DVECTOR* norm = item->ShadeNorm;
			face = item->ShadeFace;

			for (i=item->FaceNum; i > 0; i--)
			{
				D3DVECTOR* v1 = vert + face[0];
				D3DVECTOR* v2 = vert + face[1];

				double ax = v2->x - v1->x;
				double ay = v2->y - v1->y;
				double az = v2->z - v1->z;

				D3DVECTOR* v3 = vert + face[2];
				face += 3;

				double bx = v3->x - v2->x;
				double by = v3->y - v2->y;
				double bz = v3->z - v2->z;

				double nx = ay*bz - az*by;
				double ny = az*bx - ax*bz;
				double nz = ax*by - ay*bx;

				double r = sqrt(nx*nx + ny*ny + nz*nz);
				if (r == 0)
				{
					r = 1;
					ny = 1;
				}

				r = 1/r;

				norm->x = (float)(nx * r);
				norm->y = (float)(ny * r);
				norm->z = (float)(nz * r);

				norm++;
			}

// inicializace bufferu hran
			face = item->ShadeFace;
			int* edgev = item->ShadeEdgeV;

			for (i=item->FaceNum; i>0; i--)
			{
				int v1 = face[0];
				edgev[0] = v1;
				edgev[5] = v1;

				int v2 = face[1];
				edgev[3] = v2;
				edgev[4] = v2;

				int v3 = face[2];
				edgev[1] = v3;
				edgev[2] = v3;

				edgev += 6; 
				face += 3;
			}

// vyhledání protichùdných (paralelních) hran
			int n = item->FaceNum*3;
			int* edgen = item->ShadeEdgeN;

			for (i = n; i > 0; i--)
			{
				*edgen = -1;
				edgen++;
			}

			edgen = item->ShadeEdgeN;
			edgev = item->ShadeEdgeV;
			int v1,v2,j;

			for (i = n - 1; i >= 0; i--)
			{
				edgen[i] = -1;
				v1 = edgev[2*i];
				v2 = edgev[2*i+1];

				for (j = n - 1; j >= 0; j--)
				{
					if ((edgev[2*j] == v2) &&
						(edgev[2*j+1] == v1) &&
						((edgen[j] < 0) || (edgen[j] == i)))	// spoleènou hranu mohou mít jen 2 plošky
					{
						edgen[i] = j;
						break;
					}
				}
			}

// vyhledání hrany smìrem doleva
			edgen = item->ShadeEdgeN;
			int* left = item->ShadeLeft;

			for (i=0; i<n; i++)
			{
				int u = i + 2;
				if ((u % 3) != 2) u -= 3;

				int k = edgen[u];

				if (k < 0)
				{
					k = i;

					int ss = 100;		// ochrana proti zacyklení (povrch mùže být nespojitý)

					while (edgen[k] >= 0)
					{
						int s = edgen[k] + 1;
						if ((s % 3) == 0) s -= 3;
						if (s == i) break;
						k = s;

						ss--;
						if (ss == 0)
						{
							k = i;
							break;
						}
					}
				}

				left[i] = k;
			}

// vyhledání následující hrany (z 2. vrcholu vpravo)
			int* next = item->ShadeNext;
			edgen = item->ShadeEdgeN;
			left = item->ShadeLeft;

			for (i=0; i<n; i++)
			{
				int k = edgen[i];

				if ((k < 0) || (left[k] == k))
				{
					k = i+1;
					if ((k % 3) == 0) k -= 3;

					int p = k;

					int ss = 100;		// ochrana proti zacyklení (povrch mùže být nespojitý)

					while (edgen[k] >= 0)
					{
						int s = edgen[k] + 1;
						if ((s % 3) == 0) s -= 3;
						if (s == p) break;
						k = s;

						ss--;
						if (ss == 0)
						{
							k = p;
							break;
						}
					}

					*next = k;
				}
				else
				{
					*next = left[k];
				}

				next++;
			}

// aktualizace klonù
			D3DFITEM* item2 = item;

			for (;;)
			{
				item2 = D3DF_Data + item2->CloneNext;
				if (item2 == item) break;

				MemFree(item2->ShadeEdgeS);
				item2->ShadeEdgeS = (char*)MemGet(item->FaceNum * 3 * sizeof(char));

				item2->ShadeVertN = item->ShadeVertN;
				item2->ShadeVert = item->ShadeVert;
				item2->ShadeFace = item->ShadeFace;
				item2->ShadeNorm = item->ShadeNorm;
				item2->ShadeEdgeV = item->ShadeEdgeV;
				item2->ShadeEdgeN = item->ShadeEdgeN;
				item2->ShadeNext = item->ShadeNext;
				item2->ShadeLeft = item->ShadeLeft;

				item2->ShadowPoint.x = 0;
				item2->ShadowPoint.y = 0;
				item2->ShadowPoint.z = 0;
			}
		}

// zjištìní osvícení plošek
		char* edges = item->ShadeEdgeS;

		for (i=item->FaceNum*3; i>0; i--)
		{
			*edges = 0;
			edges++;
		}

		D3DVECTOR* norm = item->ShadeNorm;
		edges = item->ShadeEdgeS;
		int* edgen = item->ShadeEdgeN;

		for (i=item->FaceNum; i>0; i--)
		{
			BOOL lit = ((norm->x*smer.x + norm->y*smer.y + norm->z*smer.z) >= 0);

			if (lit)
			{
				edges[0] |= 1;
				edges[1] |= 1;
				edges[2] |= 1;

				int k =  edgen[0];
				if (k >= 0) item->ShadeEdgeS[k] |= 2;

				k =  edgen[1];
				if (k >= 0) item->ShadeEdgeS[k] |= 2;

				k =  edgen[2];
				if (k >= 0) item->ShadeEdgeS[k] |= 2;
			}

			norm++;
			edges += 3;
			edgen += 3;
		}

// sestavení modelu stínu
		int n = item->FaceNum;
		int* fan = (int*)MemGet(n *sizeof(int*));
		item->ShadowFan = fan;
		D3DVECTOR* vol = (D3DVECTOR*)MemGet(n*5* sizeof(D3DVECTOR));
		item->ShadowVolume = vol;

		item->ShadowFanN = 0;
		item->ShadowFan0 = 0;

		i = 3*n - 1;
		edges = item->ShadeEdgeS;

		for (; i >= 0; i--)
		{
			if (edges[i] == 1)
			{
				item->ShadowFanN++;
				*fan = 2;
				*vol = posun;
				vol++;
				*vol = item->ShadeVert[item->ShadeEdgeV[2*i]];
				vol++;

				int k = i;
				int p = k;

				for (int s = 100; s > 0; s--)	// mùže zabloudit napø. v pøípadì duplikované plošky (nevrátí se na pùvodní hranu)
				{
					if (edges[k] == 1)
					{
						edges[k] = 0;
						*vol = item->ShadeVert[item->ShadeEdgeV[2*k+1]];
						vol++;
						*fan = *fan + 1;

						k = item->ShadeNext[k];
						p = k;
					}
					else
					{
						k = item->ShadeLeft[k];
						if (p == k) break;
					}
				}

				item->ShadowFan0 += *fan;

				fan++;
			}
		}
	}

	if (D3DSBufferFormatOK)
	{
		pD3SetSOp(D3DSTENCILOP_KEEP, D3DSTENCILOP_KEEP, D3DSTENCILOP_INCR);

		SetD3DCullingKorig(1);

		pD3Shadow(item);

		pD3SetSOp(D3DSTENCILOP_KEEP, D3DSTENCILOP_KEEP, D3DSTENCILOP_DECR);

		SetD3DCullingKorig(2);

		pD3Shadow(item);
	}
	else
	{
		D3DF_SetBlendMode(BLEND_ZERO, BLEND_SRCALPHA);

		SetD3DCullingKorig(1);

		pD3Shadow(item);

		D3DF_SetBlendMode(BLEND_DSTCOLOR, BLEND_ONE);

		SetD3DCullingKorig(2);

		pD3Shadow(item);
	}
}

////////////////////////////////////////////////////////////////////
// renderování jednoho objektu

void D3DF_Render1(D3DFITEM* item)
{
// kontrola pøeteèení vrcholù
	if (item->VertNum > MaxVertexNum)
	{
		item->AktSector = TRUE;
		D3DF_AktMatLink(item);
		return;
	}

// pøíprava položky s parametry (pro sektor se použije originál)
	D3DFITEM* item2 = item;
	while (item2->Type == D3DFTYPE_SECTOR)
	{
		item2 = D3DF_Data + item2->Parent;
		ASSERT(D3DF_IsValid(item2->Parent));
	}

// materiál a textura od rodièe
	int parmat = item2->ParMat;			// návrat materiálu od rodièe
	DWORD pardif = item2->ParDif;		// návrat difusní barvy od rodièe
	int partext = item2->ParText;		// návrat textury od rodièe

// nastavení materiálu objektu
	if (item2->MatSource == MATSOURCE_PARENT)
	{
		D3DM_Set(parmat);
	}
	else
	{
		D3DM_Set(item2->Material);
	}

// cyklus pøes stupnì textur
	int i;
	for (i = 0; i < MaxTextureSimult; i++)
	{

// test, zda je stupeò povolen
		int colorop = item2->ColorOp[i];
		int texture = item2->Texture[i];

		if (colorop != D3DTOP_DISABLE)
		{

// textura nebude využita
			if (!item2->UseText[i])
			{
				texture = -1;
			}
// zdìdìní textury od rodièe
			else
			{
				if ((i == 0) && (item2->MatSource == MATSOURCE_PARENT) && (texture < 0))
				{
					texture = partext;
				}
			}
				
// pøednastavení Mip filtru (je potøeba znát kvùli generování textur)
			if (texture >= 0)
			{
				SetD3DMinMipFilter(item2->MinFilter[i], item2->MipFilter[i], i);
			}
			else
			{
				if (item2->UseText[i]) colorop = D3DTOP_DISABLE;
			}
		}

// vypnutí textury, je-li stupeò vypnut
		else
		{
			texture = -1;
		}

// nastavení textury (musí být za nastavením materiálu, starší verze DirectX ruší texturu)
		D3DT_Akt(texture, i);

// nastavení barevných operací
		int colorarg1 = item2->ColorArg1[i];
		int colorarg2 = item2->ColorArg2[i];

		if ((colorop != D3DColorOp[i]) || 
			(colorarg1 != D3DColorArg1[i]) || 
			(colorarg2 != D3DColorArg2[i]))
		{
			D3DColorOp[i] = colorop;
			D3DColorArg1[i] = colorarg1;
			D3DColorArg2[i] = colorarg2;

			pD3SetColorOp(colorop, colorarg1, colorarg2, i);
		}

// vypnutí ostatních stupòù
		if (colorop == D3DTOP_DISABLE)
		{
			for (int j = i+1; j < MAX_STAGES; j++)
			{
				if ((D3DTOP_DISABLE != D3DColorOp[j]) || 
					(D3DTA_TEXTURE != D3DColorArg1[j]) || 
					(D3DTA_CURRENT != D3DColorArg2[j]))
				{
					D3DColorOp[j] = D3DTOP_DISABLE;
					D3DColorArg1[j] = D3DTA_TEXTURE;
					D3DColorArg2[j] = D3DTA_CURRENT;

					pD3SetColorOp(D3DTOP_DISABLE, D3DTA_TEXTURE, D3DTA_CURRENT, j);
				}

				if ((D3DTOP_DISABLE != D3DAlphaOp[j]) || 
					(D3DTA_TEXTURE != D3DAlphaArg1[j]) || 
					(D3DTA_CURRENT != D3DAlphaArg2[j]))
				{
					D3DAlphaOp[j] = D3DTOP_DISABLE;
					D3DAlphaArg1[j] = D3DTA_TEXTURE;
					D3DAlphaArg2[j] = D3DTA_CURRENT;

					pD3SetAlphaOp(D3DTOP_DISABLE, D3DTA_TEXTURE, D3DTA_CURRENT, j);
				}

				D3DT_Akt(-1, j);
			}
			break;
		}

// nastavení alfa operací
		int alphaop = item2->AlphaOp[i];
		int alphaarg1 = item2->AlphaArg1[i];
		int alphaarg2 = item2->AlphaArg2[i];

		if ((alphaop != D3DAlphaOp[i]) || 
			(alphaarg1 != D3DAlphaArg1[i]) ||
			(alphaarg2 != D3DAlphaArg2[i]))
		{
			D3DAlphaOp[i] = alphaop;
			D3DAlphaArg1[i] = alphaarg1;
			D3DAlphaArg2[i] = alphaarg2;

			pD3SetAlphaOp(alphaop, alphaarg1, alphaarg2, i);
		}

// nastavení adresování textur
		int adru = item2->AddressU[i];
		int adrv = item2->AddressV[i];

		if ((adru != D3DAddressU[i]) || (adrv != D3DAddressV[i]))
		{
			D3DAddressU[i] = adru;
			D3DAddressV[i] = adrv;

			pD3SetAddress(adru, adrv, i);
		}

// nastavení barvy okolí textur
//			int border = item2->Border[i];

//			if (border != D3DBorder[i])
//			{
//				D3DBorder[i] = border;
//
//				pD3SetBorder(item2, i);
//			}

// nastavení zjemnìní vzdálených textur
		float bias = item2->Bias[i];

		if (bias != D3DBias[i])
		{
			D3DBias[i] = bias;
			pD3SetBias(bias, i);
		}
	}

// nastavení faktoru textur
	if (item2->TFactor != D3DTFactor)
	{
		D3DTFactor = item2->TFactor;
		pD3SetTFactor(item2);
	}

// nastavení Z zápisu
	BOOL zwrite = item2->ZWrite;
	if (zwrite != D3DZWrite)
	{
		D3DZWrite = zwrite;
		pD3SetZWrite(zwrite);
	}

// nastavení Z funkce
	int ztest = item2->ZTest;
	if ((ztest == D3DCMP_ALWAYS) && !zwrite) ztest = 0;

	if (ztest != D3DZTest)
	{
		D3DZTest = ztest;
		pD3SetZFunc(ztest);
	}

// úschova poètu zjištìných stupòù
	if (item->Stages != i)
	{
		if (item->Stages > 0) D3DF_Modi(item->Index);
		pD3Reset(item);

		D3DFITEM* item2 = item;

		do {
			item2->Stages = i;
			item2 = D3DF_Data + item2->CloneNext;
		} while (item2 != item);
	}

// pøepínaèe se musí nastavit až po aktivaci textury - tam mohou být modifikovány (viz glBlendTexture)
	if ((item2->Type == D3DFTYPE_PICTURE) || (item2->Type == D3DFTYPE_LENSFLARE))
	{
		bool oldfill = D3DWireframeGlobal;
		D3DWireframeGlobal = true;
		SetD3DWireframe(true);					// zapnutí módu výplnì
		D3DWireframeGlobal = oldfill;
	}
	else
	{
		SetD3DWireframe(item2->Wireframe);
	}

	SetD3DFogOn((item2->Type != D3DFTYPE_PICTURE) && (item2->Type != D3DFTYPE_LENSFLARE));	// pro obrázek se mlha vypne
	SetD3DLightOn(item2->Lighton);
	SetD3DShades(item2->Shades);

	if ((item2->Type != D3DFTYPE_PICTURE) && (item2->Type != D3DFTYPE_LENSFLARE))
	{
		SetD3DCullingKorig(item2->Culling);
	}
	else
	{
		SetD3DCulling(item2->Culling);
	}

// nastavení filtrù textur
	for (int j = 0; j < i; j++)
	{
		if ((item2->Texture[j] >= 0) || ((j == 0) && (item2->MatSource == MATSOURCE_PARENT) && (partext >= 0)))
		{
			SetD3DMagFilter(item2->MagFilter[j], j);
			SetD3DMinMipFilter(item2->MinFilter[j], item2->MipFilter[j], j);
		}
	}

// vygenerování automatické barvy, je-li vypnuto osvìtlení
	if (!D3DLightOnAkt)
	{
		item->Diffuse = item2->Diffuse;
		if (item2->MatSource == MATSOURCE_PARENT)
		{	
			DWORD olddif = item->Diffuse;
			item->Diffuse = pardif;
			D3DF_GenAutoCol(item);
			item->Diffuse = olddif;
		}
		else
		{
			D3DF_GenAutoCol(item);
		}
	}

// aktualizace blending módu
	if ((item2->SrcBlend != D3DSrcBlend) || (item2->DstBlend != D3DDstBlend))
	{
		D3DSrcBlend = item2->SrcBlend;
		D3DDstBlend = item2->DstBlend;
		pD3AktBlend(item2);
	}

// aktualizace referenèní úrovnì alfa
	if (item2->AlphaRef2 != D3DAlphaRef)
	{
		D3DAlphaRef = item2->AlphaRef2;
		pD3AktAlfa(item2);
	}

// vyrenderování objektu (pro sektor pøechodnì mód materiálu od originálu)
	MATSOURCE oldmat = item->MatSource;
	item->MatSource = item2->MatSource;
	pD3Render(item);
	item->MatSource = oldmat;

// renderování stínu
	if (item->IsShadow && D3DShadows && D3DL_IsValid(0))
	{
		ShadowItems[ShadowNum] = item;
		ShadowNum++;
	}
}

////////////////////////////////////////////////////////////////////
// renderování objektù (pro Direct3D musí být BeginScene a EndScene!)

void D3DF_Render()
{
// aktualizace posledních zmìn (pojistka aby se dodateènì nezmìnila adresa bufferu)
	D3DF_AktMatrix();

// inicializace bufferu seznamu položek
	for (int i = 0; i < MAX_RENDGROUP; i++)
	{
		RendNum[i] = 0;
		if (RendMax < D3DF_Max)
		{
			MemFree(RendItems[i]);
			RendItems[i] = (D3DFITEM**)MemGet(D3DF_Max * sizeof(D3DFITEM*));
		}
	}

	ShadowNum = 0;
	if (RendMax < D3DF_Max)
	{
		MemFree(ShadowItems);
		ShadowItems = (D3DFITEM**)MemGet(D3DF_Max * sizeof(D3DFITEM*));
	}

	RendMax = D3DF_Max;

// výchozí položkou bude scéna
	int index = 0;								// výchozí index = scéna
	D3DFITEM* item = D3DF_Data;					// adresa položky scény
	int parmat = item->Material;				// implicitní materiál od rodièe
	DWORD pardif = item->Diffuse;				// difusní barva od rodièe
	int partext = item->Texture[0];				// implicitní textura od rodièe
	double scale = 1;							// mìøítko hranice od rodièe
	bool parclip = true;						// pøíznak nutnosti testovat viditelnost
	bool aktclip = true;						// nutné testovat viditelnost položky
	bool visible = true;						// položka je viditelná
	BOOL parsector = false;						// pøíznak sektorù od rodièe

// cyklus pøes všechny objekty ve scénì (objekt scény se nerenderuje)
	for (;;) 
	{

// vnoøení do potomka (pozn.: potomek nemùže mít index=0, to je scéna)
		int index2 = item->Child;

		if ((index2 >= 0) && item->Visible && visible)
		{
			item->ParMat = parmat;				// úschova materiálu od rodièe
			item->ParDif = pardif;				// úschova difusní barvy od rodièe
			item->ParText = partext;			// úschova textury od rodièe
			item->ParScale = scale;				// úschova mìøítka od rodièe
			item->ParClip = parclip;			// úschova pøíznaku nutnosti testovat viditelnost
			item->ParSector = parsector;		// úschova pøíznaku použití sektorù

			if (item->MatSource == MATSOURCE_FRAME)
			{
				parmat = item->Material;		// nový materiál od rodièe
				pardif = item->Diffuse;			// nová difusní barva od rodièe
			}

			if ((item->MatSource != MATSOURCE_PARENT) || (item->Texture[0] >= 0))
			{
				partext = item->Texture[0];		// nová textura od rodièe
			}

			scale *= item->ScaleB;				// nové mìøítko pro hranici
			parclip = aktclip;					// potomek dìdí pøíznak oøezávání
			parsector = item->UseSector;		// pøíznak používání sektorù

			index = index2;						// index potomka
		}

// jinak pøejítí k sousedu stejné úrovnì (pozn.: soused nemùže mít index=0, to je scéna)
		else
		{
			if (index == 0) break;				// scéna nemá následujícího souseda
			index = item->Next;					// následující soused

// není-li soused, pøejítí k rodièi a jeho následující položce
			while (index < 0)					// není další soused?
			{
				index = item->Parent;			// návrat k dalšímu rodièi
				if (index == 0) break;			// je již opìt scéna, mùžeme skonèit

				item = D3DF_Data + index;		// adresa rodièe
				index = item->Next;				// další soused rodièe

				parmat = item->ParMat;			// návrat materiálu od rodièe
				pardif = item->ParDif;			// návrat difusní barvy od rodièe
				partext = item->ParText;		// návrat textury od rodièe
				scale = item->ParScale;			// návrat mìøítka od rodièe
				parclip = item->ParClip;		// návrat pøíznaku nutnosti testovat viditelnost
				parsector = item->ParSector;	// návrat pøíznaku používání sektorù
			}

			if (index == 0) break;				// je již opìt scéna, mùžeme skonèit
		}
		aktclip = parclip;
		visible = true;

// adresa položky
		item = D3DF_Data + index;

// kontrola viditelnosti položky
		if (item->Visible)
		{

// kontrola vzdálenosti pro LOD
			float sx = item->MatrixWorld._41 - D3DF_Data[1].MatrixWorld._41;
			float sy = item->MatrixWorld._42 - D3DF_Data[1].MatrixWorld._42;
			float sz = item->MatrixWorld._43 - D3DF_Data[1].MatrixWorld._43;
			float r = (float)(sx*sx + sy*sy + sz*sz);

			if((item->Type == D3DFTYPE_PICTURE) ||
				((r >= item->LODMin2) && (r < item->LODMax2)))
			{

// kontrola typu objektu
				if ((item->Type == D3DFTYPE_DECAL) ||
					(item->Type == D3DFTYPE_OBJECT) ||
					(item->Type == D3DFTYPE_GROUP) ||
					(item->Type == D3DFTYPE_LENSFLARE) ||
					((item->Type == D3DFTYPE_SECTOR) && parsector))
				{
				
// pøíprava transformovaného støedu a polomìru hranièní koule
					if (parclip)
					{
						sx = item->MatrixWorld._41;
						sy = item->MatrixWorld._42;
						sz = item->MatrixWorld._43;
						r = (float)(item->BoundR * scale * item->ScaleB);
						if (item->IsShadow && D3DShadows) r = (float)(r * 2);

// test, zda je objekt v pohledovém kuželu
						PLANE* p = D3DFrustum;
						float d;

						aktclip = false;

						for (int i = 6; i > 0; i--)
						{
							d = p->A*sx + p->B*sy + p->C*sz + p->D;
							if (d < -r)
							{
								visible = false;
								break;
							}

							if (d <= r)
							{
								aktclip = true;
							}

							p++;
						}
					}

					if (visible)
					{

// materiál a textura zdìdìné od rodièe
						item->ParMat = parmat;				// úschova materiálu od rodièe
						item->ParDif = pardif;				// úschova difusní barvy od rodièe
						item->ParText = partext;			// úschova textury od rodièe
						item->ParScale = scale;				// úschova mìøítka od rodièe
						item->ParClip = parclip;			// úschova pøíznaku nutnosti testovat viditelnost
						item->AktClip = aktclip;			// položku je nutné testovat

// pro 2D objekt upøesnìní natoèení
						if (item->Type == D3DFTYPE_DECAL)
						{
							D3DF_AktMatLink(item);
						}

// pro skupinu pøíprava vektoru dopadu
						if (item->Type == D3DFTYPE_GROUP)
						{
							PrepShadowPoint(item, &(item->ShadowPoint));
						}
						else
						{

// test platnosti položky
							if ((item->Vertex != NULL) &&
								(item->Normal != NULL) &&
								(item->TextUV != NULL) &&
								!item->UseSector)
							{

// automatické mapování textur (mapují se sektory namísto celých objektù)
								if (item->AutoMap != NULL)
								{
									D3DTextMap(item);
									D3DF_AktMatUnlink(item);	// nesmí zmìnit adresu bufferu sektorù
								}

// uložení objektu do seznamu k renderování
								if(item->Type == D3DFTYPE_LENSFLARE)
								{
									item->AktClip = true;
								}
								int group = item->RendGroup;
								(RendItems[group])[RendNum[group]] = item;
								RendNum[group]++;
							}
						}
					}
				}
				else
				{

// úschova 2D obrázku
					if(item->Type == D3DFTYPE_PICTURE)
					{
						item->AktClip = true;
						int group = item->RendGroup;
						(RendItems[group])[RendNum[group]] = item;
						RendNum[group]++;
					}
				}
			}
			else
			{
				visible = false;
			}
		}
	}

// aktualizace natoèení DECAL objektù (nesmí zmìnit adresu bufferu objektù!!!!)
	D3DF_AktMatrix();

// cyklus pøes skupiny k renderování
	for (int group = 0; group < MAX_RENDGROUP; group++)
	{
		D3DFITEM** items;
		int n = RendNum[group] - 1;

// skupina bude tøídìná
		if ((n > 0) && RendSort[group])
		{

// pøíprava vzdáleností objektù
			items = RendItems[group];

			for (i = n; i >= 0; i--)
			{
				item = *items;

				double dx;
				double dy;
				double dz;

				if ((item->Type == D3DFTYPE_PICTURE) || (item->Type == D3DFTYPE_LENSFLARE))
				{
					dx = 0;
					dy = 0;
					dz = item->TransZ;
				}
				else
				{
					dx = item->MatrixWorld._41 - D3DF_Data[1].MatrixWorld._41;
					dy = item->MatrixWorld._42 - D3DF_Data[1].MatrixWorld._42;
					dz = item->MatrixWorld._43 - D3DF_Data[1].MatrixWorld._43;
				}

				double dr = D3DBackClip - D3DFrontClip;

				item->Dist = Round((dx*dx + dy*dy + dz*dz)/(dr*dr) * 0x04000000);

				items++;
			}

// tøídìní objektù v seznamu
			items = RendItems[group];

			for (i = 0; i < n;)
			{
				int d1 = items[0]->Dist;
				int d2 = items[1]->Dist;

				if (d1 < d2)
				{
					D3DFITEM* k = items[0];
					items[0] = items[1];
					items[1] = k;

					if (i > 0)
					{
						items--;
						i--;
					}
					else
					{
						items++;
						i++;
					}
				}
				else
				{
					items++;
					i++;
				}
			}
		}

// vyrenderování objektù
		items = RendItems[group];

		for (i = n; i >= 0; i--)
		{
			D3DF_Render1(*items);
			items++;
		}

// vyrenderování stínù
		if ((group == RENDTRANS-1) && (ShadowNum > 0))
		{
			pD3SetColorOp(D3DTOP_MODULATE, D3DTA_TEXTURE, D3DTA_CURRENT, 0);
			pD3SetAlphaOp(D3DTOP_MODULATE, D3DTA_TEXTURE, D3DTA_CURRENT, 0);
			D3DT_Akt(-1, 0);

			D3DZWrite = FALSE;
			pD3SetZWrite(FALSE);

			D3DZTest = D3DCMP_LESSEQUAL;
			pD3SetZFunc(D3DCMP_LESSEQUAL);

			SetD3DShades(false);
			SetD3DLightOn(false);					// osvìtlení vypnuto
			SetD3DFogOn(FALSE);						// vypnutí mlhy

			if (D3DSBufferFormatOK)
			{
				if (D3DIntAkt > 2)
				{
					D3DF_SetBlendMode(BLEND_ZERO, BLEND_ONE);
				}
				else
				{
					D3DF_SetBlendMode(BLEND_ONE, BLEND_ZERO);
				}
				
				pD3SEnable(true);
				pD3SetSFunc(D3DCMP_ALWAYS, 1, 0xffffffff);
				pD3SetSMask(0xffffffff);

				D3DFITEM** items = ShadowItems;

				for (i = ShadowNum-1; i >= 0; i--)
				{
					D3DF_RendShadow(*items);
					items++;
				}

				D3DF_SetBlendMode(BLEND_ZERO, BLEND_SRCCOLOR);

				pD3SetSFunc(D3DCMP_NOTEQUAL, 0, 0xffffffff);
				pD3SetSOp(D3DSTENCILOP_KEEP, D3DSTENCILOP_KEEP, D3DSTENCILOP_KEEP);

				SetD3DCullingKorig(1);						// zobrazena pouze pøední strana

				D3DZTest = 0;
				pD3SetZFunc(0);

				pD3Shadows();

				D3DF_SetBlendMode(BLEND_ONE, BLEND_ZERO);

				D3DSBufferClear = true;

				pD3SEnable(false);
			}
			else
			{
				D3DFITEM** items = ShadowItems;

				for (i = ShadowNum-1; i >= 0; i--)
				{
					D3DF_RendShadow(*items);
					items++;
				}
			}

			D3DZWrite = TRUE;
			pD3SetZWrite(TRUE);
		}
	}
}


#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------
