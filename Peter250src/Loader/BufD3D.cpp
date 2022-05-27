
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
// P�i po�ad� rotac� X-Y-Z se n�sob� maticemi v tomto po�ad�:
//
//   zm�na m���tka:
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
// Po vyn�soben� matic v uveden�m po�ad� je v�sledn� transforma�n� matice:
// ---------------------------------------------------------------------------
//        sx*cosy*cosz                  sx*cosy*sinz             -sx*siny    0
// sy*(sinx*siny*cosz-cosx*sinz) sy*(sinx*siny*sinz+cosx*cosz) sy*sinx*cosy  0
// sz*(cosx*siny*cosz+sinx*sinz) sz*(cosx*siny*sinz-sinx*cosz) sz*cosx*cosy  0
//              tx                           ty                     tz       1
// ---------------------------------------------------------------------------
//
// Zn�me-li v�slednou transforma�n� matici ve tvaru:
//  a11  a12  a13  a14
//  a21  a22  a23  a24
//  a31  a32  a33  a34
//  a41  a42  a43  a44
//
// potom lze zp�tn� vypo��tat p�vodn� transformace:
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
// Podporovan� form�ty textur (p��kazem CreateTexture):
//	D3DFMT_A8R8G8B8	(32 bit� s alpha)
//	D3DFMT_X8R8G8B8 (32 bit�)
//	D3DFMT_R5G6B5 (16 bit�)
//	D3DFMT_X1R5G5B5 (15 bit�)
//	D3DFMT_A1R5G5B5 (15 bit� s alpha)
//	D3DFMT_A4R4G4B4 (12 bit� s alpha)
//	D3DFMT_DXT1 (komprese DXT1)
//	D3DFMT_DXT2 (komprese DXT2)
//	D3DFMT_DXT3 (komprese DXT3)
//	D3DFMT_DXT4 (komprese DXT4)
//	D3DFMT_DXT5 (komprese DXT5)

////////////////////////////////////////////////////////////////////
// data bufferu textur

int				D3DT_Num = 0;		// po�et polo�ek v bufferu textur
int				D3DT_Max = 0;		// velikost bufferu textur (polo�ek)
D3DTITEM*		D3DT_Data = NULL;	// buffer textur

int				D3DT_Active[MAX_STAGES]; // aktivn� nastaven� textura (i je-li vypnut�)
bool			D3DT_On[MAX_STAGES];	// textura je zapnuta

//D3DFITEM_*		D3D_Rend[3*MAX_ZBIAS]; // polo�ky k renderov�n� (b�n�/blending/2D)

////////////////////////////////////////////////////////////////////
// p�id�n� textury do seznamu textur, vrac� index textury
// (nesm� to b�t pr�zdn� obr�zek !!!!)
// (!!!! v sou�asnosti podporovan� pouze form�t D3DFMT_A8R8G8B8 !!!!!);

int D3DT_Add(CPicture pic)
{
	ASSERT(pic.IsNotEmpty());

// test, zda obr�zek je ji� v bufferu ulo�en
	PICTUREDATA* picdata = pic.Data();			// data obr�zku
	int inx = picdata->TextInx;					// mo�n� index polo�ky
	if (inx >= 0)								// je to platn� index?
	{
		ASSERT(D3DT_IsValid(inx));
		ASSERT(D3DT_Data[inx].Pic == pic);
		D3DT_Data[inx].Ref++;					// zv��en� ��ta�e referenc� na polo�ku
		return inx;
	}
			
// nalezen� voln� polo�ky
	inx = 0;
	D3DTITEM* item = D3DT_Data;

	for (; inx < D3DT_Num;)
	{
		if (item->Ref == 0) break;
		item++;
		inx++;
	}

// bude nov� polo�ka - zv��en� velikosti bufferu
	if (inx == D3DT_Num) D3DT_Num++;			// zv��en� po�tu polo�ek do bufferu

	if (inx == D3DT_Max)
	{
		D3DT_Max = 2*inx;						// nov� velikost bufferu
		if (inx == 0) D3DT_Max = 64;			// minim�ln� velikost bufferu
		MemBuf(D3DT_Data, D3DT_Max);			// zv��en� velikosti bufferu
	}
	item = D3DT_Data + inx;

// inicializace nov� polo�ky
	item->Ref = 2;								// po�et referenc� na polo�ku
	item->Pic.Init(picdata);					// ulo�en� obr�zku do polo�ky
	item->Mips = FALSE;
	item->Data1 = NULL;							// nulov�n� dat textury
	item->Data2 = NULL;
	item->MipMaps = 0;							// po�et mipmaps neomezen
	picdata->TextInx = inx;						// odkaz z obr�zku do bufferu

//	MemFill(item->Rend, 3*MAX_ZBIAS*sizeof(D3DFITEM_*), 0);

// nastaven� po�adovan�ho zjemn�n� (nen�-li to textura ze souboru)
	if (picdata->TextData == NULL)
	{
		picdata->TextSmooth = D3DSmooth;
	}

	return inx;
}

////////////////////////////////////////////////////////////////////
// uvoln�n� textury (kontroluje platnost indexu)
// (obr�zek textury p�i sv�m zru�en� vol� funkci pD3TextureRes(titem))

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
// resetov�n� cachov�n� bufferu (p�ed ukon�en�m ovlada�e)

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
// nastaven� aktivn� textury (-1 = vypnuta)
//
// Pro star�� verze DirectX se nastaven�m materi�lu zru�� aktivn� textura,
// bude nastaveno D3DT_Activ na -1, D3DT_On z�stane nezm�n�no

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

// adresa polo�ky
	D3DTITEM* item = D3DT_Data + index;

// adresa obr�zku
	PICTUREDATA* picdata = item->Pic.Data();

// zru�en� textury, pokud byla vytvo�ena bez mipmaps
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
*								D3D buffer - materi�l						*
*																			*
\***************************************************************************/

////////////////////////////////////////////////////////////////////
// data bufferu materi�l� (polo�ka 0 = implicitn� materi�l)

int				D3DM_Num = 0;		// po�et polo�ek v bufferu materi�l�
int				D3DM_Max = 0;		// velikost bufferu materi�l� (polo�ek)	
int				D3DM_Akt = -1;		// aktivn� nastaven� materi�l
D3DMITEM*		D3DM_Data = NULL;	// buffer materi�l�

////////////////////////////////////////////////////////////////////
// p�id�n� materi�lu do bufferu materi�l� (vrac� index materi�lu)
// - pokud materi�l ji� existuje, vrac� index existuj�c�ho materi�lu
// (POZOR - vol� se b�hem inicializace, je�t� p�ed inicializac� obsluhy 3D!)

int D3DM_Add(D3DMATERIAL8* mat)
{
// velikost materi�lu mus� b�t n�sobek 4 !!!
#pragma warning ( disable: 4127)				// hl�en� - konstantn� podm�nka
	ASSERT(sizeof(D3DMATERIAL8) == 0x44);
#pragma warning ( default: 4127)				// hl�en� - konstantn� podm�nka

// vyhled�n� ji� existuj�c�ho materi�lu v bufferu
	int inx = 0;								// ukazatel indexu materi�lu
	D3DMITEM* mat2 = D3DM_Data;				// ukazatel polo�ek materi�lu

	for (; inx < D3DM_Num; inx++)				// cyklus p�es v�echny materi�ly
	{
		int* src = (int*)mat;					// adresa hledan� polo�ky
		int* dst = (int*)(&mat2->Material);		// adresa polo�ky v bufferu
		int j = sizeof(D3DMATERIAL8)/4;			// velikost polo�ky
		for (; j > 0; j--)
		{
			if (*dst != *src) break;
			src++;
			dst++;
		}
		if (j == 0) return inx;					// nalezena shodn� polo�ka

		mat2++;									// zv��en� ukazatele adresy polo�ky
	}
		
// bude nov� polo�ka - zv��en� velikosti bufferu (nyn� inx == D3DM_MatNum)
	if (inx == D3DM_Max)
	{
		D3DM_Max = 2*inx;						// nov� velikost bufferu
		if (inx == 0) D3DM_Max = 64;			// minim�ln� velikost bufferu
		MemBuf(D3DM_Data, D3DM_Max);				// zv��en� velikosti bufferu
	}
	D3DM_Num++;									// zv��en� po�tu polo�ek do bufferu

// ulo�en� nov� polo�ky do bufferu
	MemCopy(&(D3DM_Data[inx].Material), mat, sizeof(D3DMATERIAL8));
	D3DM_Data[inx].Data1 = NULL;
	D3DM_Data[inx].Data2 = NULL;

	return inx;
}

////////////////////////////////////////////////////////////////////
// nastaven� materi�lu jako aktivn� (bez kontroly indexu)
//
// Pro star�� verze DirectX se nastaven�m materi�lu zru�� aktivn� textura,
// bude nastaveno D3DT_Activ na -1, D3DT_On z�stane nezm�n�no

void D3DM_Set(const int index)
{
	ASSERT(D3DM_IsValid(index));

// kontrola, zda je materi�l ji� nastaven
	if (index != D3DM_Akt)
	{
		D3DM_Akt = index;
		pD3SetMaterial(D3DM_Data + index);
	}
}

////////////////////////////////////////////////////////////////////
// resetov�n� cachov�n� bufferu (p�ed ukon�en�m ovlada�e)

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
*								D3D buffer - sv�tla							*
*																			*
\***************************************************************************/

////////////////////////////////////////////////////////////////////
// data bufferu sv�tel

int				D3DL_Num = 0;		// po�et polo�ek v bufferu sv�tel (v�etn� voln�ch)
int				D3DL_Max = 0;		// velikost bufferu sv�tel (polo�ek)	
D3DLITEM*		D3DL_Data = NULL;	// buffer sv�tel

////////////////////////////////////////////////////////////////////
// vytvo�en� nov�ho sv�tla (vrac� index sv�tla)
// (POZOR - vol� se b�hem inicializace, je�t� p�ed inicializac� obsluhy 3D!)

int D3DL_New()
{

// nalezen� voln� polo�ky sv�tla
	int index = 0;
	for (; index < D3DL_Num; index++)
	{
		if (!D3DL_Data[index].IsUsed) break;
	}

// vytvo�en� nov�ho sv�tla, pokud nebyla nalezena voln� polo�ka
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

// inicializace polo�ky sv�tla
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
// zru�en� sv�tla (kontroluje platnost indexu)

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
// aktualizace sv�tel

void D3DL_AktLight()
{
// cyklus p�es v�echna sv�tla
	D3DLITEM* item = D3DL_Data;
	for (int i = 0; i < D3DL_Num; i++)
	{

// p��prava pozice a sm�ru
		D3DFITEM* frame = D3DF_Get(item->Frame);

		D3DLIGHT8* light = &(item->Light);
	// transforma�n� matice se vyn�sob� vektorem po��tku, tj. (0, 0, 0)
		light->Position.x = frame->MatrixWorld._41;
		light->Position.y = frame->MatrixWorld._42;
		light->Position.z = frame->MatrixWorld._43;
	// transforma�n� matice se vyn�sob� vektorem sm�ru, tj. (0, 0, 1)
	// (a ode�te se vektor nov�ho po��tku - p��slu�n� �leny se vyru��)
		light->Direction.x = frame->MatrixWorld._31;
		light->Direction.y = frame->MatrixWorld._32;
		light->Direction.z = frame->MatrixWorld._33;

// aktualizace sv�tla
		pD3AktLight(i, item);
		item->Akt = false;

		item++;
	}
}

////////////////////////////////////////////////////////////////////
// resetov�n� bufferu sv�tel

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
*								D3D buffer - r�my							*
*																			*
\***************************************************************************/

// vzor jednotkov� matice
D3DMATRIX	Matrix1Vzor	=
{
	1,	0,	0,	0,
	0,	1,	0,	0,
	0,	0,	1,	0,
	0,	0,	0,	1
};

////////////////////////////////////////////////////////////////////
// data bufferu r�m�

D3DFITEM*	D3DF_Data = NULL;	// ukazatel na data
bool*		D3DF_Valid = NULL;	// p��znaky platnosti polo�ek
int			D3DF_Max = 0;		// velikost bufferu (polo�ek)
int			D3DF_Next = -1;		// p��t� voln� polo�ka (-1 = nen�)

////////////////////////////////////////////////////////////////////
// seznam objekt� pro Blending

//BLENDITEM*	D3DB_Data = NULL;	// ukazatel na data
//int			D3DB_Max = 0;		// velikost bufferu (velikost jako D3DF_Max)
//int			D3DB_Num = 0;		// po�et polo�ek v bufferu

BLENDTYPE	D3DSrcBlend = BLEND_INVALID; // aktu�ln� nastaven� blending operace zdroje
BLENDTYPE	D3DDstBlend = BLEND_INVALID; // aktu�ln� nastaven� blending operace c�le
int			D3DAlphaRef = -1;			// aktu�ln� nastaven� alfa �rovn�

int			D3DColorOp[MAX_STAGES];		// aktu�ln� nastaven� operac� barev
int			D3DColorArg1[MAX_STAGES];	// aktu�ln� nastaven� argumentu 1 operac� barev
int			D3DColorArg2[MAX_STAGES];	// aktu�ln� nastaven� argumentu 2 operac� barev
int			D3DAlphaOp[MAX_STAGES];		// aktu�ln� nastaven� operac� barev
int			D3DAlphaArg1[MAX_STAGES];	// aktu�ln� nastaven� argumentu 1 operac� barev
int			D3DAlphaArg2[MAX_STAGES];	// aktu�ln� nastaven� argumentu 2 operac� barev
int			D3DAddressU[MAX_STAGES];	// aktu�ln� adresov�n� textur U
int			D3DAddressV[MAX_STAGES];	// aktu�ln� adresov�n� textur V
//int			D3DBorder[MAX_STAGES];		// barva okol� textur
float		D3DBias[MAX_STAGES];		// zjemn�n� vzd�len�ch textur
DWORD		D3DTFactor = MAXDWORD;		// aktu�ln� nastaven� faktoru textur
BOOL		D3DZWrite = -1;				// aktu�ln� hodnota hloubkov�ho z�pisu
int			D3DZTest = -1;				// aktu�ln� hodnota hloubkov�ho testu

BOOL		D3DStencilEnable = FALSE;	// aktu�ln� hodnota povolen� stencil operac�
bool		D3DStencilCleared = false;	// stencil buffer je v aktu�ln�m sn�mku vymaz�n
int			D3DStencilFunc = -1;		// aktu�ln� hodnota testu stencil bufferu
int			D3DStencilRef = -1;			// aktu�ln� referen�n� hodnota stencil testu
int			D3DStencilMask = -1;		// aktu�ln� hodnota masky stencil testu
int			D3DStencilWriteMask = -1;	// aktu�ln� hodnota z�pisov� masky do stencil bufferu
int			D3DStencilFail = -1;		// aktu�ln� hodnota operace p�i ne�sp�chu stencil testu
int			D3DStencilZFail = -1;		// aktu�ln� hodnota operace p�i �sp�chu stencil testu a ne�sp�chu Z testu
int			D3DStencilZPass = -1;		// aktu�ln� hodnota operace p�i �sp�chu stencil testu a �sp�chu Z testu

BOOL		RendSort[MAX_RENDGROUP];	// p��znaky t��d�n� skupin
int			RendNum[MAX_RENDGROUP];		// po�ty polo�ek v bufferech
D3DFITEM**	RendItems[MAX_RENDGROUP];	// seznam polo�ek k renderov�n�
int			RendMax = 0;				// velikost buffer� polo�ek

int			ShadowNum;					// po�et polo�ek v bufferu st�n�
D3DFITEM**	ShadowItems;				// seznam polo�ek k renderov�n� st�n�

int			D3DAktMat = -1;				// prvn� polo�ka k aktualizaci matic (-1=nen�)

////////////////////////////////////////////////////////////////////
// seznam objekt� 2D obr�zk�

D3DFITEM**	D3DP_Items = NULL;	// ukazatel na data seznamu obr�zk�
int			D3DP_Num = 0;		// po�et obr�zk� v seznamu


////////////////////////////////////////////////////////////////////
// transpozice matice (zdrojov� a c�lov� matice m��e b�t stejn�)

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
// inverze matice (zdrojov� a c�lov� matice m��e b�t stejn�)

void MatrixInv(D3DMATRIX* dst, D3DMATRIX* src)
{

// pomocn� matice - 2 matice vedle sebe (4 ��dky po 8 prvc�ch)
	float r1[8], r2[8], r3[8], r4[8];
	float* s[4];
	
	s[0] = &r1[0];
	s[1] = &r2[0];
	s[2] = &r3[0];
	s[3] = &r4[0];

// inicializace matice (1.matice = p�vodn�, 2.matice = jednotkov�)
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

// nalezen� nejv�t��ho prvku v ka�d�m ��dku
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

		if(scp_max == 0) return;	// je to singul�rn� matice

		scp[i] = scp_max;
	}

	
	int p,jj;
	float* tmprow;

	int pivot_to;

	for(i=0; i<4; i++)
	{

// nalezen� ��dku s nejv�t�� v�hou ("osa" matice)
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

	if(s[3][3] == 0) return; // singul�rn� matice

// p�evod troj�heln�kov� matice na jednotkovou (ode�ten�m prvk�)
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
	
// ulo�en� v�sledn� matice
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
// vyn�soben� vektoru matic� (prov�d� se p�i sn�en� p�esnosti FPU)
// (zdrojov� i c�lov� vektor mohou b�t shodn�)

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
// inverzn� rotace pohledov� matice podle osy X (funkce SIN negovan�)

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
// inverzn� rotace pohledov� matice podle osy Y (funkce SIN negovan�)

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
// inverzn� rotace pohledov� matice podle osy Z (funkce SIN negovan�)

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
// generov�n� pohledov� matice (inverzn� v po�ad� od root polo�ky)

void GenViewMat(int index)
{

// adresa polo�ky
	D3DFITEM* item = D3DF_Data + index;

// je-li platn� rodi� (ne sc�na), generov�n� nejd��ve rodi�e
	int par = item->Parent;
	if (par > 0) GenViewMat(par);

// zp�tn� transformace posunu
	D3DViewMatrix._41 = (float)(D3DViewMatrix._41 - item->TransX);
	D3DViewMatrix._42 = (float)(D3DViewMatrix._42 - item->TransY);
	D3DViewMatrix._43 = (float)(D3DViewMatrix._43 - item->TransZ);

// proveden� rotac� (v opa�n�m po�ad�)
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

// zp�tn� transformace m���tka
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
// rotace vlastn� matice podle osy X (prov�d� se p�i sn�en� p�esnosti FPU!)
//
// Rota�n� matice:
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
// rotace vlastn� matice podle osy Y (prov�d� se p�i sn�en� p�esnosti FPU!)
//
// Rota�n� matice:
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
// rotace vlastn� matice podle osy Z (prov�d� se p�i sn�en� p�esnosti FPU!)
//
// Rota�n� matice:
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
// vyn�soben� dvou matic (m = m1 * m2), v�sledn� matice nesm� b�t vstupn�!

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
// aktualizace m���tka pro hranici

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
// aktualizace sektor� objektu (kontroluje index)

void D3DF_AktSector(int index)
{
// adresa polo�ky
	if (D3DF_IsValid(index))
	{
		D3DFITEM* item = D3DF_Data + index;
		item->AktSector = FALSE;
		item->UseSector = FALSE;

// jin� objekty ne� D3DFTYPE_OBJECT a D3DFTYPE_SECTOR nemaj� nikdy v�ce vrchol�
		if ((item->Type != D3DFTYPE_OBJECT) &&
			(item->Type != D3DFTYPE_SECTOR))
		{
			return;
		}
	
// zru�en� star�ch sektor�
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

// test, zda budou pou�ity sektory
		int vertN = item->VertNum;

		BOOL usesector = (vertN > MaxVertexNum2);
		if (usesector)
		{
			item->UseSector = TRUE;
			int i;

// p��prava buffer� sektor�
			double sumx[8];		// sou�et sou�adnic X
			double sumy[8];		// sou�et sou�adnic Y
			double sumz[8];		// sou�et sou�adnic Z
			int fN[8];			// po�et plo�ek v sektoru
			int vN[8];			// po�et vrchol� v sektoru
			int* vmap[8];		// mapov�n� vrchol� pro sektor

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

// rozd�len� plo�ek do sektor�
			int faceN = item->FaceNum;	// po�et plo�ek celkem
			int* sec = (int*)MemGet(faceN * sizeof(int)); // indexy sektor� plo�ek
			int* face = item->Face;		// buffer plo�ek
			D3DVECTOR* vert = item->Vertex;	// buffer vrchol�

			for (i = 0; i < faceN; i++)
			{
			// zji�t�n� sektoru, do kter�ho plo�ka pat��
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

			// p�i�ten� vrchol�
				fN[s]++;
				sumx[s] += x;
				sumy[s] += y;
				sumz[s] += z;

			// zmapov�n� vrchol� plo�ky
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

// vytvo�en� nov�ch objekt�
			for (i = 0; i < 8; i++)
			{
				int fn = fN[i];

				if (fn > 0)
				{
				// vytvo�en� nov� polo�ky
					int index2 = D3DF_New();
					D3DF_Link(index, index2);
					D3DFITEM* item2 = D3DF_Data + index2;
					item = D3DF_Data + index;

				// kopie obsahu polo�ky
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

				// inicializace parametr�
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

				// vytvo�en� buffer� objektu
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

				// p�enesen� vrchol�
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

				// p�enesen� plo�ek
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

				// aktualizace hranice objektu (rodi�e nen� pot�eba aktualizovat)
					D3DF_MapBound(index2);
				}
			}

// zru�en� buffer� sektor�
			for (i = 0; i < 8; i++)
			{
				MemFree(vmap[i]);
			}
			MemFree(sec);
		}
	}
}

////////////////////////////////////////////////////////////////////
// p�ipojen� do �et�zce aktualizace matic (kontroluje, zda je p�ipojen)

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
// odpojen� z �et�zce aktualizace matic (kontroluje, zda je p�ipojen)

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
// aktualizace transforma�n�ch matic v�ech objekt�
// (prov�d� se p�i sn�en� p�esnosti FPU!)

void D3DF_AktMatrix()
{

// dal�� polo�ka k aktualizaci
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

// aktualizace sektor� objektu
		if (item->AktSector) // || ((item->VertNum > MaxVertexNum2) && !item->UseSector))
		{
			D3DF_AktSector(index);
			item = D3DF_Data + index;
		}

// odpojen� z aktualizac� matic (AktSektor m��e za�azovat do �et�zce)
		D3DF_AktMatUnlink(item);

// test, zda je 2D objekt
		if (item->Type == D3DFTYPE_DECAL)
		{

// na�ten� sou�adnic relativn� ke kame�e (nevad�, �e se pou�ij� star� sou�adnice)
			double x = item->MatrixWorld._41 - D3DF_Data[1].MatrixWorld._41;
			double z = item->MatrixWorld._43 - D3DF_Data[1].MatrixWorld._43;

// test, zda je dostate�n� zm�na pro aktualizaci
			if ((fabs(x) < 1) ||
				(fabs(z) < 1) ||
				(fabs((item->RotateX2D - x)/x) >= 0.0625) ||
				(fabs((item->RotateZ2D - z)/z) >= 0.0625))
			{
				item->RotateX2D = x;
				item->RotateZ2D = z;

// �hel sm�ru ke kame�e
				double alpha;
				if (item->Parent == 1)
				{
					alpha = 0;
				}
				else
				{
					alpha = uhel90 - atan2(z, x);
				}

// nastaven� �hlu
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

// test, zda je pot�eba aktualizovat vlastn� matici
		if (item->AktOwn)
		{
			item->AktOwn = false;				// zru�en� po�adavku aktualizace
			item->AktWorld = true;				// po�adavek aktualizace v�sledn� matice

// p��prava matice m���tka
			D3DMATRIX* m = &(item->MatrixOwn);
			Matrix0(m);							// vynulov�n� matice
			m->_11 = (float)item->ScaleX;		// m���tko ve sm�ru X
			m->_22 = (float)item->ScaleY;		// m���tko ve sm�ru Y
			m->_33 = (float)item->ScaleZ;		// m���tko ve sm�ru Z
			m->_44 = 1;

// aktualizace rotac�
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
			
// p�id�n� posunu
			m->_41 = (float)(m->_41 + item->TransX);
			m->_42 = (float)(m->_42 + item->TransY);
			m->_43 = (float)(m->_43 + item->TransZ);

// je nutn� aktualizovat polom�r hranice rodi�e
			int par = item->Parent;
			if (par > 0)
			{
				D3DF_Data[par].AktBound = true;
				D3DF_AktMatLink(D3DF_Data + par);
			}
		}

// aktualizace v�sledn� transforma�n� matice
		if (item->AktWorld)
		{
			item->AktWorld = false;				// zru�en� po�adavku aktualizace matice

			item->ShadowPoint.x = 0;			// aktualizovat st�n
			item->ShadowPoint.y = 0;
			item->ShadowPoint.z = 0;

// po�adavek k aktualizici sv�tla
			if (item->Type == D3DFTYPE_LIGHT)
			{
				D3DL_Get(item->Light)->Akt = true;
			}

// p�egenerov�n� lensfalre
//			if (item->Type == D3DFTYPE_LENSFLARE)
//			{
//				pD3Reset(item);
//			}

// je-li rodi�em sc�na, matice se nekombinuj�
			int index2 = item->Parent;

			if (index2 == 0)
			{
				MemCopy(&(item->MatrixWorld), &(item->MatrixOwn), sizeof(D3DMATRIX));
			}

// pro jin� rodi�e kombinace s jejich matic�
			else
			{
				D3DMATRIX* m2 = &(D3DF_Data[index2].MatrixWorld);
				MatrixMul(&(item->MatrixWorld), &(item->MatrixOwn), m2);
			}

// aktualizace pohledov� matice, je-li to kamera
			if (index == 1)
			{

// inicializace pohledov� matice na jednotkovou matici
				Matrix1(&D3DViewMatrix);

// vygenerov�n� matice
				GenViewMat(1);

// po�adavek aktualizace pohledov� matice
				D3DViewAkt = true;
			}

// po�adavek aktualizace matice v�ech potomk�
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
// nastaven� transformac� podle transforma�n� matice

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
// vytvo�en� nov� polo�ky bufferu (vrac� index polo�ky)
// (POZOR - vol� se b�hem inicializace, je�t� p�ed inicializac� obsluhy 3D!)

int D3DF_New()
{
// bude zv��en� velikosti bufferu
	if (D3DF_Next < 0)			// je dal�� polo�ka?
	{
		int i = D3DF_Max;		// to bude po�et nov�ch polo�ek
		if (i == 0) i = 64;		// implicitn� po�et polo�ek
		int next = D3DF_Max;	// p��t� polo�ka - 1
		D3DF_Next = next;		// odkaz na prvn� novou polo�ku
		D3DF_Max = next + i;	// zv��en� po�tu polo�ek v bufferu

// zv��en� velikosti buffer�
		MemBuf(D3DF_Data, D3DF_Max); // zv��en� velikosti bufferu dat
		MemBuf(D3DF_Valid, D3DF_Max); // zv��en� velikosti bufferu platnosti
		MemFill(D3DF_Valid + next, i*sizeof(bool), false); // nastaven� p��znak� na neplatn� polo�ky

// inicializace nov�ch odkaz�
		D3DFITEM* data = D3DF_Data + next - 1; // ukazatel dat - 1
		for (; i > 0; i--)
		{
			data++;				// zv��en� ukazatele polo�ek
			next++;				// zv��en� indexu p��t� polo�ky
			*(int*)data = next;	// odkaz na p��t� polo�ku
		}
		*(int*)data = -1;		// nen� dal�� polo�ka
	}

// zplatn�n� nov� polo�ky
	int i = D3DF_Next;			// p��t� voln� polo�ka
	D3DFITEM* item = D3DF_Data + i; // adresa polo�ky
	D3DF_Next = *(int*)item;	// nov� dal�� polo�ka
	D3DF_Valid[i] = true;		// nastaven� p��znaku platnosti polo�ky

// inicializace polo�ky
	MemFill(item, sizeof(D3DFITEM), 0);
	item->Index = i;
	item->Parent = -1;
	item->Child = -1;
	item->Prev = -1;
	item->Next = -1;

	item->ClonePrev = i;
	item->CloneNext = i;

// p�ipojen� polo�ky jako potomek sc�ny (nen�-li to sc�na)
	if (i != 0)
	{
		item->Parent = 0;	// rodi�em bude sc�na

		D3DFITEM* item2 = D3DF_Data + 0; // adresa polo�ky sc�ny

		int i2 = item2->Child;		// prvn� potomek sc�ny
		item->Next = i2;			// ukazatel na dal��ho potomka
		item2->Child = i;			// polo�ka bude prvn�m potomkem sc�ny

		if (i2 >= 0)				// byl ji� n�jak� jin� potomek?
		{
			D3DFITEM* item3 = D3DF_Data + i2;	// adresa jin�ho potomka
			item3->Prev = i;		// polo�ka bude jeho p�edch�zej�c�m sousedem
		}
	}

// inicializace ostatn�ch parametr�
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
// p��prava polo�ky k modifikaci (odpojen� od klon�)

void D3DF_Modi(int index)
{
// kontrola polo�ky
	if (!D3DF_IsValid(index)) return;

// adresa polo�ky
	D3DFITEM* item = D3DF_Data + index;

// test, zda je sou��st� �et�zce klon�
	int cloneprev = item->ClonePrev;
	int clonenext = item->CloneNext;
	if (cloneprev == item->Index) return;

// odpojen� od klon�
	D3DF_Data[cloneprev].CloneNext = clonenext;
	D3DF_Data[clonenext].ClonePrev = cloneprev;
	item->CloneNext = index;
	item->ClonePrev = index;

// duplikace ter�nu (kopie datov�ho bufferu)
	if (item->IsTerrain)
	{
		DuplikData((void**)&(item->TerMap), (item->TerWidth+1)*(item->TerHeight+1)*sizeof(float));
	}

// duplikace objektu (kopie datov�ch buffer�)
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

// duplikace st�n�
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

// buffery jsou neplatn�
	item->Data1 = NULL;
	item->Data2 = NULL;
}

////////////////////////////////////////////////////////////////////
// duplikace jedn� polo�ky (bez potomk�) (nekontroluje platnost polo�ky)

int D3DF_Copy1(int index)
{
// pro neplatnou polo�ku se vytvo�� pouze skupina
	if (index <= 1) return D3DF_New();

// vytvo�en� nov� polo�ky
	int newindex = D3DF_New();

// adresa star� i nov� polo�ky
	D3DFITEM* olditem = D3DF_Data + index;
	D3DFITEM* newitem = D3DF_Data + newindex;

// kopie obsahu polo�ky
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

// za�len�n� do �et�zce klon�
	int clonenext = olditem->CloneNext;
	olditem->CloneNext = newindex;
	newitem->ClonePrev = index;
	newitem->CloneNext = clonenext;
	D3DF_Data[clonenext].ClonePrev = newindex;

// duplikace sv�tla (vytvo�en� nov�ho sv�tla se stejn�mi parametry)
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

// aktualizovat matici (rodi�em je sc�na)
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

// buffery jsou neplatn�
	newitem->ShadowPoint.x = 0;
	newitem->ShadowPoint.y = 0;
	newitem->ShadowPoint.z = 0;
	newitem->ShadeEdgeS = NULL;
	newitem->ShadowFanN = 0;
	newitem->ShadowFan0 = 0;
	newitem->ShadowFan = NULL;
	newitem->ShadowVolume = NULL;

	newitem->Data3 = NULL;

// duplikace buffer� morfov�n�
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

// obr�zek mus� m�t vlastn� buffery sou�adnic
	if ((newitem->Type == D3DFTYPE_PICTURE) ||
		(newitem->Type == D3DFTYPE_LENSFLARE))
	{
		D3DF_Modi(newindex);
	}

	return newindex;
}

////////////////////////////////////////////////////////////////////
// kopie polo�ky s potomky (vrac� index nov� polo�ky)

int D3DF_Copy(int oldindex)
{
// pro neplatnou polo�ku se vytvo�� pouze skupina
	if ((oldindex <= 1) || !D3DF_IsValid(oldindex))
	{
		return D3DF_New();
	}

// duplikace v�choz� polo�ky
	int newindex = D3DF_Copy1(oldindex);

// duplikace v�ech potomk�
	int oldinx = oldindex;
	int newinx = newindex;
	int newpar = 0;

	D3DFITEM* olditem;
	D3DFITEM* newitem;

	for (;;)
	{

// adresa aktu�ln� p�vodn� polo�ky
		olditem = D3DF_Data + oldinx;
		newitem = D3DF_Data + newinx;

// vno�en� do potomka
		int oldinx2 = olditem->Child;
		if (oldinx2 >= 0)
		{
			oldinx = oldinx2;					// index potomka
			newpar = newinx;					// nov� rodi� vytv��en� polo�ky
		}

// jinak p�ej�t� k sousedu stejn� �rovn� (pozn.: soused nem��e m�t index=0, to je sc�na)
		else
		{
			if (oldinx == oldindex) break;		// je to v�choz� polo�ka - konec
			oldinx = olditem->Next;				// n�sleduj�c� soused

// nen�-li soused, p�ej�t� k rodi�i a jeho n�sleduj�c� polo�ce
			while (oldinx < 0)					// nen� dal�� soused?
			{
				oldinx = olditem->Parent;		// n�vrat k dal��mu rodi�i
				newinx = newpar;

				if (oldinx == oldindex) break;	// je ji� op�t v�choz� polo�ka, konec

				olditem = D3DF_Data + oldinx;	// adresa rodi�e
				newitem = D3DF_Data + newinx;
				newpar = newitem->Parent;
				oldinx = olditem->Next;			// dal�� soused rodi�e
			}

			if (oldinx == oldindex) break;	// je ji� op�t v�choz� polo�ka, konec
		}

// kopie polo�ky
		newinx = D3DF_Copy1(oldinx);

// p�ipojen� polo�ky k rodi�i
		D3DF_Link(newpar, newinx);
	}

// kopie polo�ky LOD
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
// zru�en� v�ech polo�ek v bufferu (ukl�d�n� za�ne op�t po �ad�)

void D3DF_DelAll()
{
// zru�en� polo�ek
	for (int i = D3DF_Max-1; i >= 0; i--)
	{
		D3DF_Del(i);
	}

// zru�en� bufferu
	MemBuf(D3DF_Data, 0);		// zru�en� bufferu dat
	MemBuf(D3DF_Valid, 0);		// zru�en� bufferu platnosti
	D3DF_Max = 0;				// nen� ��dn� polo�ka v bufferu
	D3DF_Next = -1;				// nen� p��t� polo�ka
}

////////////////////////////////////////////////////////////////////
// zru�en� buffer� morfov�n�

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
// zru�en� polo�ky (kontroluje platnost indexu)

void _fastcall D3DF_Del(const int index)
{
	if (D3DF_IsValid(index))					// je index platn�?
	{

// adresa polo�ky
		D3DFITEM* item = D3DF_Data + index;

// zru�en� LOD objekt�
		D3DF_Del(item->LODNext);

// odpojen� od p�edch�zej�c�ho LOD
		int lod = item->LODPrev;
		if (lod >= 0)
		{
			D3DF_Data[lod].LODNext = -1;
			item->LODPrev = -1;
		}

// zru�en� textury
		for (int j = 0; j < MAX_STAGES; j++)
		{
			D3DT_Del(item->Texture[j]);
			item->Texture[j] = -1;
		}

// zru�en� sv�tla
		D3DL_Del(item->Light);
		item->Light = -1;

// odpojen� z �et�zce klon�
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

// zru�en� bufferu ter�nu
			MemFree(item->TerMap);

// resetov�n� objektu
			pD3Reset(item);

// zru�en� buffer� vzoru objektu
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

// zru�en� buffer� st�n�
			MemFree(item->ShadeVert);
			MemFree(item->ShadeFace);
			MemFree(item->ShadeNorm);
			MemFree(item->ShadeEdgeV);
			MemFree(item->ShadeEdgeN);
			MemFree(item->ShadeNext);
			MemFree(item->ShadeLeft);
		}

// zru�en� ostatn�ch buffer�
		MemFree(item->ShadeEdgeS);
		MemFree(item->ShadowFan);
		MemFree(item->ShadowVolume);
		MemFree(item->AutoMap);

// ozna�en� buffer� jako zru�en�
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

// zru�en� buffer� morfov�n�
		D3DF_MorphReset(item);

// zru�en� v�ech potomk� polo�ky (kamera se jen odpoj�)
		while (item->Child >= 0)
		{
			if (item->Child == 1)
			{
				D3DF_Link(0, 1);
			}
			else
			{
				D3DF_Del(item->Child); // zru�en� potomk�
			}
		}

// odpojen� od �et�zce aktualizac� matic
		D3DF_AktMatUnlink(item);

// nav�z�n� na dal�� polo�ku z p�edch�zej�c� polo�ky
		int prev = item->Prev;						// p�edch�zej�c� polo�ka
		int next = item->Next;						// n�sleduj�c� polo�ka
		if (prev >= 0)
		{
			D3DF_Data[prev].Next = next;			// napojen� na dal�� polo�ku
		}

// nav�z�n� na p�edch�zej�c� polo�ku z dal�� polo�ky
		if (next >= 0)
		{
			D3DF_Data[next].Prev = prev;			// napojen� na p�edch�zej�c� polo�ku
		}

// nav�z�n� na nov�ho potomka z rodi�e
		int par = item->Parent;						// rodi�
		if (par >= 0)
		{
			D3DFITEM* itempar = D3DF_Data + par;	// adresa rodi�e
			
			if (itempar->Child == index)
			{
				itempar->Child = next;				// napojen� na dal��ho potomka
			}
		}

// zru�en� platnosti polo�ky
		*(int*)item = D3DF_Next;				// p��t� voln� polo�ka
		D3DF_Valid[index] = false;				// zru�en� p��znaku platnosti
		D3DF_Next = index;						// odkaz na tuto polo�ku
	}
}

////////////////////////////////////////////////////////////////////
// resetov�n� cachov�n� bufferu (p�ed ukon�en�m ovlada�e)

void D3DF_Reset()
{
	D3DSrcBlend = BLEND_INVALID; // aktu�ln� nastaven� blending operace zdroje
	D3DDstBlend = BLEND_INVALID; // aktu�ln� nastaven� blending operace c�le
	D3DAlphaRef = -1;			// aktu�ln� nastaven� alfa �rovn�
	D3DTFactor = MAXDWORD;		// aktu�ln� nastaven� faktoru textur
	D3DZWrite = -1;				// aktu�ln� hodnota hloubkov�ho z�pisu
	D3DZTest = -1;				// aktu�ln� hodnota hloubkov�ho testu

	int i;
	for (i = 0; i < MAX_STAGES; i++)
	{
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
// inicializace mana�eru vrchol� (index polo�ky mus� b�t platn�!)

D3DFITEM*	UsedItem = NULL;		// adresa polo�ky
int			UsedVert = 0;			// po�et aktu�ln�ch vrchol�
int			UsedMax = 1;			// velikost buffer� vrchol�
bool*		UsedMap = NULL;			// mapa pou�it�ch vrchol�

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
// duplikace vrcholu (parametr = index vrcholu plo�ky), vrac� nov� index

int D3DF_UsedDuplik(int faceinx)
{

// index duplikovan� polo�ky
	int vert = UsedItem->Face[faceinx];
	if (UsedItem->MorphMax > 0) return vert;	// pro morfovac� buffery z�kaz operace
	int newvert = UsedVert;
	UsedVert++;
	UsedItem->VertNum = UsedVert;
	UsedItem->Face[faceinx] = newvert;

	UsedItem->AktSector = TRUE;

// zv��en� velikosti buffer�
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

// p�enesen� dat star�ho vrcholu
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
// nastaven� norm�ly vrcholu (pro dan� index vrcholu plo�ky)

void D3DF_UsedNormal(int faceinx, float nx, float ny, float nz)
{
// index vrcholu
	int vert = UsedItem->Face[faceinx];

// vrchol je pou�it�
	if (UsedMap[vert])
	{

// test, zda se data li��
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

// vrchol nen� pou�it�
	else
	{
		UsedItem->Normal[vert].x = nx;
		UsedItem->Normal[vert].y = ny;
		UsedItem->Normal[vert].z = nz;
		UsedMap[vert] = true;
	}
}

////////////////////////////////////////////////////////////////////
// nastaven� barvy vrcholu (pro dan� index vrcholu plo�ky)

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

// vrchol je pou�it�
	if (UsedMap[vert])
	{

// test, zda se data li��
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

// vrchol nen� pou�it�
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
// nastaven� adresy textury vrcholu (pro dan� index vrcholu plo�ky)

void D3DF_UsedTextUV(int faceinx, float u, float v, int stage)
{
// index vrcholu
	int vert = UsedItem->Face[faceinx];

// vrchol je pou�it�
	if (UsedMap[vert])
	{

// test, zda se data li��
		if (((u != UsedItem->TextUV[stage][2*vert]) ||
			(v != UsedItem->TextUV[stage][2*vert+1])) &&
			(UsedVert < 0x10000))
		{
			int newvert = D3DF_UsedDuplik(faceinx);

			UsedItem->TextUV[stage][2*newvert] = u;
			UsedItem->TextUV[stage][2*newvert+1] = v;
		}
	}

// vrchol nen� pou�it�
	else
	{
		UsedItem->TextUV[stage][2*vert] = u;
		UsedItem->TextUV[stage][2*vert+1] = v;

		UsedMap[vert] = true;
	}
}

////////////////////////////////////////////////////////////////////
// napojen� polo�ky na rodi�e (s odpojen�m od p�edch�zej�c�ho rodi�e)
// kontroluje platnost obou index�

void _fastcall D3DF_Link(const int parent, const int child)
{

// kontrola platnosti polo�ek a zda nen� polo�ka ji� p�ipojena
	if (D3DF_IsValid(parent) && 
		D3DF_IsValid(child) && 
		(parent != child) &&
		(child != 0) &&
		(D3DF_Data[child].Parent != parent))
	{

// kontrola, zda nen� cyklick� propojen� polo�ek
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

// odpojen� potomka ze star�ho m�sta
		D3DFITEM* itemchild = D3DF_Data + child;	 // adresa potomka

		int prev = itemchild->Prev;					// p�edch�zej�c� polo�ka
		int next = itemchild->Next;					// n�sleduj�c� polo�ka
		if (prev >= 0)
		{
			D3DF_Data[prev].Next = next;			// napojen� na dal�� polo�ku
		}

		if (next >= 0)
		{
			D3DF_Data[next].Prev = prev;			// napojen� na p�edch�zej�c� polo�ku
		}

		int par = itemchild->Parent;				// star� rodi�
		if (par >= 0)
		{
			D3DFITEM* itempar = D3DF_Data + par;	// adresa rodi�e
			
			if (itempar->Child == child)
			{
				itempar->Child = next;				// napojen� na dal��ho potomka
			}
		}

// napojen� potomka do nov�ho m�sta
		D3DFITEM* itempar = D3DF_Data + parent;		// adresa nov�ho rodi�e

		next = itempar->Child;						// p�vodn� potomek rodi�e
		itempar->Child = child;						// p�ipojen� nov�ho potomka
		itempar->AktBound = true;					// aktualizovat hranici s potomky
		D3DF_AktMatLink(itempar);
		itemchild->Parent = parent;					// nov� rodi� polo�ky
		itemchild->Next = next;						// dal�� polo�ka
		itemchild->Prev = -1;						// nen� p�edch�zej�c� polo�ka

		itemchild->AktWorld = true;					// je nutn� aktualizovat transformace
		D3DF_AktMatLink(itemchild);

		if (next >= 0)
		{
			D3DF_Data[next].Prev = child;			// je to p�edch�zej�c� polo�ka p�vodn�ho potomka
		}

// p�ipojen� sousedn�ch stup�� LOD (tento je ji� p�ipojen = nebude zacyklen�)
		int lod = itemchild->LODPrev;
		if (lod >= 0) D3DF_Link(parent, lod);
		lod = itemchild->LODNext;
		if (lod >= 0) D3DF_Link(parent, lod);
	}
}

////////////////////////////////////////////////////////////////////
// zmapov�n� vlastn� hranice (index mus� b�t platn�)

void D3DF_MapBound(int index)
{
	ASSERT(D3DF_IsValid(index));

// adresa r�mu
	D3DFITEM* item = D3DF_Data + index;

// cyklus p�es v�echny vrcholy
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

// nastaven� nov�ho polom�ru hranice
	item->BoundR0 = sqrt(r);
	item->AktBound = true;

	D3DF_AktMatLink(item);
}


////////////////////////////////////////////////////////////////////
// vytvo�en� objektu (vrac� index objektu, pro 0 vrchol� to bude skupina)

int D3DF_CreateMesh(int vertN, D3DVECTOR* vert, int faceN, int* face)
{
// vytvo�en� nov�ho r�mu
	int inx = D3DF_New();
	D3DFITEM* item = D3DF_Data + inx;

// kontrola po�tu vrchol� a plo�ek (p�i chyb� z�stane jako skupina)
	if ((vertN < 3) || (faceN < 1)) return inx;

// vytvo�en� buffer� pro vrcholy
	item->VertNum = vertN;
	item->Vertex = (D3DVECTOR*)MemGet(vertN * sizeof(D3DVECTOR));
	MemCopy(item->Vertex, vert, vertN * sizeof(D3DVECTOR));

// vytvo�en� buffer� pro plo�ky
	item->FaceNum = faceN;
	item->Face = (int*)MemGet(faceN * 3 * sizeof(int));
	MemCopy(item->Face, face, faceN * 3 * sizeof(int));

// po�adavek aktualizace objektu
//	item->AktMat = true;
	item->Type = D3DFTYPE_OBJECT;

// aktualizovat sektory
	item->AktSector = TRUE;

	D3DF_AktMatLink(item);

// zmapov�n� vlastn� hranice
	D3DF_MapBound(inx);

	return inx;
}

////////////////////////////////////////////////////////////////////
// nastaven� norm�l objektu (po�et norm�l mus� souhlasit s po�tem vrchol�!)

void D3DF_SetNormal(int index, D3DVECTOR* norm)
{
	ASSERT(D3DF_IsValid(index));

// adresa r�mu
	D3DFITEM* item = D3DF_Data + index;
	ASSERT(item->VertNum > 0);

// vytvo�en� bufferu pro vektory norm�l
	int size = item->VertNum * sizeof(D3DVECTOR);
	if (item->Normal == NULL)
	{
		item->Normal = (D3DVECTOR*)MemGet(size);
	}

// ulo�en� bufferu norm�l (s normalizac� norm�l)
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
// korekce nov� vytvo�en�ho aktu�ln�ho objektu pro pravoruk� sou�adn� syst�m

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
// nastaven� barev vrchol� objektu (po�et mus� souhlasit s po�tem vrchol�!)

void D3DF_SetVertCol(int index, D3DCOLORVALUE* col)
{
	ASSERT(D3DF_IsValid(index));

// adresa r�mu
	D3DFITEM* item = D3DF_Data + index;
	int n = item->VertNum;
	ASSERT(n > 0);

// zru�en� bufferu vrchol� (mus� se p�egenerovat)
	pD3Reset(item);

// vytvo�en� buffer� pro barvy
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

// ulo�en� buffer� barev
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

// zru�en� p��znaku automatick� barvy
	item->AutoCol = MAXDWORD;

// aktualizovat sektory
	item->AktSector = TRUE;

	D3DF_AktMatLink(item);
}

////////////////////////////////////////////////////////////////////
// nastaven� mapov�n� textur (po�et mus� souhlasit s po�tem vrchol�!)

void D3DF_SetTextUV(int index, float* textuv, int stage)
{
	ASSERT(D3DF_IsValid(index));

// adresa r�mu
	D3DFITEM* item = D3DF_Data + index;
	int n = item->VertNum;
	ASSERT(n > 0);

// vytvo�en� buffer� pro mapov�n� textur
	float* dst = item->TextUV[stage];
	int size = n * 2 * sizeof(float);
	if (dst == NULL)
	{
		dst = (float*)MemGet(size);
		item->TextUV[stage] = dst;
	}

// p�enesen� mapov�n� textur
	MemCopy(dst, textuv, size);

// aktualizovat sektory
	item->AktSector = TRUE;

	D3DF_AktMatLink(item);

// resetov�n� objektu
	pD3Reset(item);
}

////////////////////////////////////////////////////////////////////
// nastaven� adresov�n� a zalamov�n� textury

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
// nastaven� blending m�du b�hem renderov�n�

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
// nastaven� blending m�du (1=normal, 11=ohe�, 20=sklo, 54=p�ekryv)

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
// vygenerov�n� norm�l (kontroluje index)

void D3DF_AutoNormals(int index, double angle)
{
	if (D3DF_IsValid(index))
	{

// odpojen� dat od klon�
		D3DF_Modi(index);

		D3DFITEM* item = D3DF_Data + index;

		if ((item->VertNum > 0) && (item->FaceNum > 0))
		{

// vytvo�en� bufferu pro vektory norm�l
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

// p��prava mana�eru vrchol�
			D3DF_UsedInit(index);

// v�po�et norm�l plo�ek
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

// normalizace referen�n�ho �hlu
			while (angle >= pi2) angle -= pi2;
			while (angle < 0) angle += pi2;
			if (angle >= pi) angle = pi2 - angle;

// �schova norm�l
			if (angle != 0)
			{
				double cosa = cos(angle);

				int vN = item->VertNum;
				D3DVECTOR* norm0 = (D3DVECTOR*)MemGet(vN * sizeof(D3DVECTOR));
				MemCopy(norm0, item->Normal, vN * sizeof(D3DVECTOR));

// buffer pou�it�ch ploch
// ... nebylo vid�t ��dn� zlep�en� a vznikaly zlomy na m�lo odch�len�ch ploch�ch
//#define USEDMAX 100

//				D3DVECTOR* used = (D3DVECTOR*)MemGet(USEDMAX * sizeof(D3DVECTOR));
//				int usedN;

// cyklus p�es v�echny vrcholy
				for (i = 0; i < vN; i++)
				{
					D3DVECTOR* v = item->Vertex + i;
					D3DVECTOR* n1 = norm0 + i;

					D3DVECTOR nn = *n1;

					BOOL smooth = FALSE;

//					usedN = 1;
//					used[0] = *n1;

// vyhled�n� stejn�ch vrchol�
					D3DVECTOR* v2 = item->Vertex;

					for (int i2 = 0; i2 < vN; i2++)
					{
						if ((i2 != i) &&
//							((fabs(v->x - v2->x) + fabs(v->y - v2->y) + fabs(v->z - v2->z)) < 0.01))
							(*(DWORD*)(&v->x) == *(DWORD*)(&v2->x)) && 
							(*(DWORD*)(&v->y) == *(DWORD*)(&v2->y)) && 
							(*(DWORD*)(&v->z) == *(DWORD*)(&v2->z)))
						{

// v�po�et �hlu norm�l
							D3DVECTOR* n2 = norm0 + i2;

//							double a = acos(n1->x*n2->x + n1->y*n2->y + n1->z*n2->z);
							double a = n1->x*n2->x + n1->y*n2->y + n1->z*n2->z;

//							if (a <= angle)
							if (a >= cosa)
							{

// kontrola, zda byla rovina ji� pou�ita
//								BOOL used0 = FALSE;

//								for (int u = 0; u < usedN; u++)
//								{
//									if ((used[u].x*n2->x + used[u].y*n2->y + used[u].z*n2->z) > 0.9999)
//									{
//										used0 = TRUE;
//										break;
//									}
//								}

// p�id�n� norm�ly
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

// normalizace norm�ly
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

// zru�en� bufferu �schovy norm�l
				MemFree(norm0);
//				MemFree(used);
			}
		}


// obsluha potomk�
		int nxt = item->Child;

		while (nxt >= 0)
		{
			D3DF_AutoNormals(nxt, angle);
			nxt = D3DF_Data[nxt].Next;
		}
	}
}

////////////////////////////////////////////////////////////////////
// vygenerov�n� automatick� barvy, je-li vypnuto osv�tlen�

void D3DF_GenAutoCol(D3DFITEM* item)
{
	if (((item->AutoCol != MAXDWORD) && (item->AutoCol != item->Diffuse)) ||
		(item->AutoCol == MAXDWORD) && (item->VertCol == NULL))
	{
		int n = item->VertNum;
		ASSERT(n > 0);

// zru�en� bufferu vrchol� (mus� se p�egenerovat)
		pD3Reset(item);

// vytvo�en� buffer� pro barvy
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

// p��prava barvy
		DWORD dif = item->Diffuse;
		if (dif == MAXDWORD) dif--;
		item->AutoCol = dif;
		DWORD col = XRGB(dif);
		D3DCOLORVALUE colf;
		BGRFloat(dif, &colf);

// ulo�en� buffer� barev
		for (; n > 0; n--)
		{
			*dst = col;
			*dst2 = colf;
			dst++;
			dst2++;
		}

// nastaven� adres i pro klony
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
// tabulka blending operac� pro Direct3D

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
	D3DBLEND_SRCALPHASAT	//	BLEND_SATURATE		= 10,	// men�� ze slo�ek SRC/DST
};

////////////////////////////////////////////////////////////////////
// tabulka blending operac� pro OpenGL

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
	GL_SRC_ALPHA_SATURATE	//	BLEND_SATURATE		= 10,	// men�� ze slo�ek SRC/DST
};

////////////////////////////////////////////////////////////////////
// p��prava bodu dopadu st�nu

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
// renderov�n� st�nu (rodi� mus� b�t ji� obslou�en�)

void D3DF_RendShadow(D3DFITEM* item)
{
// p��prava vektoru dopadu
	D3DVECTOR posun;
	PrepShadowPoint(item, &posun);

// test, zda je vektor dopadu zm�n�n
	if ((*(int*)(&posun.x) != *(int*)(&item->ShadowPoint.x)) ||
		(*(int*)(&posun.y) != *(int*)(&item->ShadowPoint.y)) ||
		(*(int*)(&posun.z) != *(int*)(&item->ShadowPoint.z)) ||
		(item->AktWorld) ||
		(item->ShadowFanN == 0))
	{
		item->ShadowPoint.x = posun.x;
		item->ShadowPoint.y = posun.y;
		item->ShadowPoint.z = posun.z;

// transformace vzd�lenosti na sou�adnice objektu
		posun.x = (float)(item->MatrixWorld._41 + posun.x);
		posun.y = (float)(item->MatrixWorld._42 + posun.y);
		posun.z = (float)(item->MatrixWorld._43 + posun.z);

		D3DMATRIX m;
		MatrixTrans(&m, &(item->MatrixWorld));
		MatrixInv(&m, &m);
		MatrixTrans(&m, &m);

		VecXMat(&posun, &posun, &m);

// p��prava vektoru sv�tla
		D3DVECTOR smer;

		if (item->Culling == 2)	// viditeln� zadn� strana plo�ky
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

// zru�en� star�ch buffer�
		MemFree(item->ShadowFan); item->ShadowFan = NULL;
		MemFree(item->ShadowVolume); item->ShadowVolume = NULL;

// zru�en� bufferu st�nu
		pD3SReset(item);

// je p�egenerov�n� objektu
		int i;
		if (item->ShadeVert == NULL)
		{
			if (item->FaceNum == 0) return;

// zru�en� star�ch buffer�
			MemFree(item->ShadeFace);
			MemFree(item->ShadeNorm);
			MemFree(item->ShadeEdgeV);
			MemFree(item->ShadeEdgeN);
			MemFree(item->ShadeEdgeS);
			MemFree(item->ShadeNext);
			MemFree(item->ShadeLeft);

// vytvo�en� nov�ch buffer�
			i = item->FaceNum;
			item->ShadeFace = (int*)MemGet(i*3 * sizeof(int));
			item->ShadeNorm = (D3DVECTOR*)MemGet(i * sizeof(D3DVECTOR));
			item->ShadeEdgeV = (int*)MemGet(i*3*2 * sizeof(int));
			item->ShadeEdgeN = (int*)MemGet(i*3 * sizeof(int));
			item->ShadeEdgeS = (char*)MemGet(i*3 * sizeof(char));
			item->ShadeNext = (int*)MemGet(i*3 * sizeof(int));
			item->ShadeLeft = (int*)MemGet(i*3 * sizeof(int));

// inicializace buffer� vrchol� a plo�ek
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

// v�po�et norm�l plo�ek
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

// vyhled�n� protich�dn�ch (paraleln�ch) hran
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
						((edgen[j] < 0) || (edgen[j] == i)))	// spole�nou hranu mohou m�t jen 2 plo�ky
					{
						edgen[i] = j;
						break;
					}
				}
			}

// vyhled�n� hrany sm�rem doleva
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

					int ss = 100;		// ochrana proti zacyklen� (povrch m��e b�t nespojit�)

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

// vyhled�n� n�sleduj�c� hrany (z 2. vrcholu vpravo)
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

					int ss = 100;		// ochrana proti zacyklen� (povrch m��e b�t nespojit�)

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

// aktualizace klon�
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

// zji�t�n� osv�cen� plo�ek
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

// sestaven� modelu st�nu
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

				for (int s = 100; s > 0; s--)	// m��e zabloudit nap�. v p��pad� duplikovan� plo�ky (nevr�t� se na p�vodn� hranu)
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
// renderov�n� jednoho objektu

void D3DF_Render1(D3DFITEM* item)
{
// kontrola p�ete�en� vrchol�
	if (item->VertNum > MaxVertexNum)
	{
		item->AktSector = TRUE;
		D3DF_AktMatLink(item);
		return;
	}

// p��prava polo�ky s parametry (pro sektor se pou�ije origin�l)
	D3DFITEM* item2 = item;
	while (item2->Type == D3DFTYPE_SECTOR)
	{
		item2 = D3DF_Data + item2->Parent;
		ASSERT(D3DF_IsValid(item2->Parent));
	}

// materi�l a textura od rodi�e
	int parmat = item2->ParMat;			// n�vrat materi�lu od rodi�e
	DWORD pardif = item2->ParDif;		// n�vrat difusn� barvy od rodi�e
	int partext = item2->ParText;		// n�vrat textury od rodi�e

// nastaven� materi�lu objektu
	if (item2->MatSource == MATSOURCE_PARENT)
	{
		D3DM_Set(parmat);
	}
	else
	{
		D3DM_Set(item2->Material);
	}

// cyklus p�es stupn� textur
	int i;
	for (i = 0; i < MaxTextureSimult; i++)
	{

// test, zda je stupe� povolen
		int colorop = item2->ColorOp[i];
		int texture = item2->Texture[i];

		if (colorop != D3DTOP_DISABLE)
		{

// textura nebude vyu�ita
			if (!item2->UseText[i])
			{
				texture = -1;
			}
// zd�d�n� textury od rodi�e
			else
			{
				if ((i == 0) && (item2->MatSource == MATSOURCE_PARENT) && (texture < 0))
				{
					texture = partext;
				}
			}
				
// p�ednastaven� Mip filtru (je pot�eba zn�t kv�li generov�n� textur)
			if (texture >= 0)
			{
				SetD3DMinMipFilter(item2->MinFilter[i], item2->MipFilter[i], i);
			}
			else
			{
				if (item2->UseText[i]) colorop = D3DTOP_DISABLE;
			}
		}

// vypnut� textury, je-li stupe� vypnut
		else
		{
			texture = -1;
		}

// nastaven� textury (mus� b�t za nastaven�m materi�lu, star�� verze DirectX ru�� texturu)
		D3DT_Akt(texture, i);

// nastaven� barevn�ch operac�
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

// vypnut� ostatn�ch stup��
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

// nastaven� alfa operac�
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

// nastaven� adresov�n� textur
		int adru = item2->AddressU[i];
		int adrv = item2->AddressV[i];

		if ((adru != D3DAddressU[i]) || (adrv != D3DAddressV[i]))
		{
			D3DAddressU[i] = adru;
			D3DAddressV[i] = adrv;

			pD3SetAddress(adru, adrv, i);
		}

// nastaven� barvy okol� textur
//			int border = item2->Border[i];

//			if (border != D3DBorder[i])
//			{
//				D3DBorder[i] = border;
//
//				pD3SetBorder(item2, i);
//			}

// nastaven� zjemn�n� vzd�len�ch textur
		float bias = item2->Bias[i];

		if (bias != D3DBias[i])
		{
			D3DBias[i] = bias;
			pD3SetBias(bias, i);
		}
	}

// nastaven� faktoru textur
	if (item2->TFactor != D3DTFactor)
	{
		D3DTFactor = item2->TFactor;
		pD3SetTFactor(item2);
	}

// nastaven� Z z�pisu
	BOOL zwrite = item2->ZWrite;
	if (zwrite != D3DZWrite)
	{
		D3DZWrite = zwrite;
		pD3SetZWrite(zwrite);
	}

// nastaven� Z funkce
	int ztest = item2->ZTest;
	if ((ztest == D3DCMP_ALWAYS) && !zwrite) ztest = 0;

	if (ztest != D3DZTest)
	{
		D3DZTest = ztest;
		pD3SetZFunc(ztest);
	}

// �schova po�tu zji�t�n�ch stup��
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

// p�ep�na�e se mus� nastavit a� po aktivaci textury - tam mohou b�t modifikov�ny (viz glBlendTexture)
	if ((item2->Type == D3DFTYPE_PICTURE) || (item2->Type == D3DFTYPE_LENSFLARE))
	{
		bool oldfill = D3DWireframeGlobal;
		D3DWireframeGlobal = true;
		SetD3DWireframe(true);					// zapnut� m�du v�pln�
		D3DWireframeGlobal = oldfill;
	}
	else
	{
		SetD3DWireframe(item2->Wireframe);
	}

	SetD3DFogOn((item2->Type != D3DFTYPE_PICTURE) && (item2->Type != D3DFTYPE_LENSFLARE));	// pro obr�zek se mlha vypne
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

// nastaven� filtr� textur
	for (int j = 0; j < i; j++)
	{
		if ((item2->Texture[j] >= 0) || ((j == 0) && (item2->MatSource == MATSOURCE_PARENT) && (partext >= 0)))
		{
			SetD3DMagFilter(item2->MagFilter[j], j);
			SetD3DMinMipFilter(item2->MinFilter[j], item2->MipFilter[j], j);
		}
	}

// vygenerov�n� automatick� barvy, je-li vypnuto osv�tlen�
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

// aktualizace blending m�du
	if ((item2->SrcBlend != D3DSrcBlend) || (item2->DstBlend != D3DDstBlend))
	{
		D3DSrcBlend = item2->SrcBlend;
		D3DDstBlend = item2->DstBlend;
		pD3AktBlend(item2);
	}

// aktualizace referen�n� �rovn� alfa
	if (item2->AlphaRef2 != D3DAlphaRef)
	{
		D3DAlphaRef = item2->AlphaRef2;
		pD3AktAlfa(item2);
	}

// vyrenderov�n� objektu (pro sektor p�echodn� m�d materi�lu od origin�lu)
	MATSOURCE oldmat = item->MatSource;
	item->MatSource = item2->MatSource;
	pD3Render(item);
	item->MatSource = oldmat;

// renderov�n� st�nu
	if (item->IsShadow && D3DShadows && D3DL_IsValid(0))
	{
		ShadowItems[ShadowNum] = item;
		ShadowNum++;
	}
}

////////////////////////////////////////////////////////////////////
// renderov�n� objekt� (pro Direct3D mus� b�t BeginScene a EndScene!)

void D3DF_Render()
{
// aktualizace posledn�ch zm�n (pojistka aby se dodate�n� nezm�nila adresa bufferu)
	D3DF_AktMatrix();

// inicializace bufferu seznamu polo�ek
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

// v�choz� polo�kou bude sc�na
	int index = 0;								// v�choz� index = sc�na
	D3DFITEM* item = D3DF_Data;					// adresa polo�ky sc�ny
	int parmat = item->Material;				// implicitn� materi�l od rodi�e
	DWORD pardif = item->Diffuse;				// difusn� barva od rodi�e
	int partext = item->Texture[0];				// implicitn� textura od rodi�e
	double scale = 1;							// m���tko hranice od rodi�e
	bool parclip = true;						// p��znak nutnosti testovat viditelnost
	bool aktclip = true;						// nutn� testovat viditelnost polo�ky
	bool visible = true;						// polo�ka je viditeln�
	BOOL parsector = false;						// p��znak sektor� od rodi�e

// cyklus p�es v�echny objekty ve sc�n� (objekt sc�ny se nerenderuje)
	for (;;) 
	{

// vno�en� do potomka (pozn.: potomek nem��e m�t index=0, to je sc�na)
		int index2 = item->Child;

		if ((index2 >= 0) && item->Visible && visible)
		{
			item->ParMat = parmat;				// �schova materi�lu od rodi�e
			item->ParDif = pardif;				// �schova difusn� barvy od rodi�e
			item->ParText = partext;			// �schova textury od rodi�e
			item->ParScale = scale;				// �schova m���tka od rodi�e
			item->ParClip = parclip;			// �schova p��znaku nutnosti testovat viditelnost
			item->ParSector = parsector;		// �schova p��znaku pou�it� sektor�

			if (item->MatSource == MATSOURCE_FRAME)
			{
				parmat = item->Material;		// nov� materi�l od rodi�e
				pardif = item->Diffuse;			// nov� difusn� barva od rodi�e
			}

			if ((item->MatSource != MATSOURCE_PARENT) || (item->Texture[0] >= 0))
			{
				partext = item->Texture[0];		// nov� textura od rodi�e
			}

			scale *= item->ScaleB;				// nov� m���tko pro hranici
			parclip = aktclip;					// potomek d�d� p��znak o�ez�v�n�
			parsector = item->UseSector;		// p��znak pou��v�n� sektor�

			index = index2;						// index potomka
		}

// jinak p�ej�t� k sousedu stejn� �rovn� (pozn.: soused nem��e m�t index=0, to je sc�na)
		else
		{
			if (index == 0) break;				// sc�na nem� n�sleduj�c�ho souseda
			index = item->Next;					// n�sleduj�c� soused

// nen�-li soused, p�ej�t� k rodi�i a jeho n�sleduj�c� polo�ce
			while (index < 0)					// nen� dal�� soused?
			{
				index = item->Parent;			// n�vrat k dal��mu rodi�i
				if (index == 0) break;			// je ji� op�t sc�na, m��eme skon�it

				item = D3DF_Data + index;		// adresa rodi�e
				index = item->Next;				// dal�� soused rodi�e

				parmat = item->ParMat;			// n�vrat materi�lu od rodi�e
				pardif = item->ParDif;			// n�vrat difusn� barvy od rodi�e
				partext = item->ParText;		// n�vrat textury od rodi�e
				scale = item->ParScale;			// n�vrat m���tka od rodi�e
				parclip = item->ParClip;		// n�vrat p��znaku nutnosti testovat viditelnost
				parsector = item->ParSector;	// n�vrat p��znaku pou��v�n� sektor�
			}

			if (index == 0) break;				// je ji� op�t sc�na, m��eme skon�it
		}
		aktclip = parclip;
		visible = true;

// adresa polo�ky
		item = D3DF_Data + index;

// kontrola viditelnosti polo�ky
		if (item->Visible)
		{

// kontrola vzd�lenosti pro LOD
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
				
// p��prava transformovan�ho st�edu a polom�ru hrani�n� koule
					if (parclip)
					{
						sx = item->MatrixWorld._41;
						sy = item->MatrixWorld._42;
						sz = item->MatrixWorld._43;
						r = (float)(item->BoundR * scale * item->ScaleB);
						if (item->IsShadow && D3DShadows) r = (float)(r * 2);

// test, zda je objekt v pohledov�m ku�elu
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

// materi�l a textura zd�d�n� od rodi�e
						item->ParMat = parmat;				// �schova materi�lu od rodi�e
						item->ParDif = pardif;				// �schova difusn� barvy od rodi�e
						item->ParText = partext;			// �schova textury od rodi�e
						item->ParScale = scale;				// �schova m���tka od rodi�e
						item->ParClip = parclip;			// �schova p��znaku nutnosti testovat viditelnost
						item->AktClip = aktclip;			// polo�ku je nutn� testovat

// pro 2D objekt up�esn�n� nato�en�
						if (item->Type == D3DFTYPE_DECAL)
						{
							D3DF_AktMatLink(item);
						}

// pro skupinu p��prava vektoru dopadu
						if (item->Type == D3DFTYPE_GROUP)
						{
							PrepShadowPoint(item, &(item->ShadowPoint));
						}
						else
						{

// test platnosti polo�ky
							if ((item->Vertex != NULL) &&
								(item->Normal != NULL) &&
								(item->TextUV != NULL) &&
								!item->UseSector)
							{

// automatick� mapov�n� textur (mapuj� se sektory nam�sto cel�ch objekt�)
								if (item->AutoMap != NULL)
								{
									D3DTextMap(item);
									D3DF_AktMatUnlink(item);	// nesm� zm�nit adresu bufferu sektor�
								}

// ulo�en� objektu do seznamu k renderov�n�
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

// �schova 2D obr�zku
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

// aktualizace nato�en� DECAL objekt� (nesm� zm�nit adresu bufferu objekt�!!!!)
	D3DF_AktMatrix();

// cyklus p�es skupiny k renderov�n�
	for (int group = 0; group < MAX_RENDGROUP; group++)
	{
		D3DFITEM** items;
		int n = RendNum[group] - 1;

// skupina bude t��d�n�
		if ((n > 0) && RendSort[group])
		{

// p��prava vzd�lenost� objekt�
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

// t��d�n� objekt� v seznamu
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

// vyrenderov�n� objekt�
		items = RendItems[group];

		for (i = n; i >= 0; i--)
		{
			D3DF_Render1(*items);
			items++;
		}

// vyrenderov�n� st�n�
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
			SetD3DLightOn(false);					// osv�tlen� vypnuto
			SetD3DFogOn(FALSE);						// vypnut� mlhy

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

				SetD3DCullingKorig(1);						// zobrazena pouze p�edn� strana

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
// --------------------- konec vypnut� pro MINI verzi -------------------
