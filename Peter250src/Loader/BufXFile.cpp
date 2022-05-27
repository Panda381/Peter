
#include "Main.h"


/***************************************************************************\
*																			*
*							Buffer objekt� X souboru						*
*																			*
\***************************************************************************/

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

/////////////////////////////////////////////////////////////////////////////
// definice

// p�eddefinovan� �ablony
typedef enum _TEMPNAME
{
	TEMP_HEADER,
	TEMP_VECTOR,
	TEMP_COORDS2D,
	TEMP_QUATERNION,
	TEMP_MATRIX4X4,
	TEMP_COLORRGBA,
	TEMP_COLORRGB,
	TEMP_INDEXEDCOLOR,
	TEMP_BOOLEAN,
	TEMP_BOOLEAN2D,
	TEMP_MATERIAL,
	TEMP_TEXTUREFILENAME,
	TEMP_MESHFACE,
	TEMP_MESHFACEWRAPS,
	TEMP_MESHTEXTURECOORDS,
	TEMP_MESHNORMALS,
	TEMP_MESHVERTEXCOLORS,
	TEMP_MESHMATERIALLIST,
	TEMP_MESH,
	TEMP_FRAMETRANSFORMMATRIX,
	TEMP_FRAME,
	TEMP_FLOATKEYS,
	TEMP_TIMEDFLOATKEYS,
	TEMP_ANIMATIONKEY,
	TEMP_ANIMATIONOPTIONS,
	TEMP_ANIMATION,
	TEMP_ANIMATIONSET,
	TEMP_PATCH,
	TEMP_PATCHMESH,
	TEMP_VERTEXDUPLICATIONINDICES,
	TEMP_XSKINMESHHEADER,
	TEMP_SKINWEIGHTS,

	TEMP_TEMPLATE,
//	TEMP_PROGRESSIVEMESH,

	TEMPNAME_LAST,
} TEMPNAME;

#define TEMPNAME_FIRST TEMP_HEADER
#define TEMPNAME_NUM (TEMPNAME_LAST - TEMPNAME_FIRST)

// jm�na �ablon
CString TempNames[TEMPNAME_NUM] = 
{
	"Header",
	"Vector",
	"Coords2d",
	"Quaternion",
	"Matrix4x4",
	"ColorRGBA",
	"ColorRGB",
	"IndexedColor",
	"Boolean",
	"Boolean2d",
	"Material",
	"TextureFilename",
	"MeshFace",
	"MeshFaceWraps",
	"MeshTextureCoords",
	"MeshNormals",
	"MeshVertexColors",
	"MeshMaterialList",
	"Mesh",
	"FrameTransformMatrix",
	"Frame",
	"FloatKeys",
	"TimedFloatKeys",
	"AnimationKey",
	"AnimationOptions",
	"Animation",
	"AnimationSet",
	"Patch",
	"PatchMesh",
	"VertexDuplicationIndices",
	"XSkinMeshHeader",
	"SkinWeights",

	"template",
//	"ProgressiveMesh",
};

// jm�na �ablon	velk�mi p�smeny
CString TempNamesUpper[TEMPNAME_NUM];	// jm�na �ablon velk�mi p�smeny

// slova bin�rn�ho form�tu X (ka�d� slovo je (2) WORD)
#define TOKEN_NAME 1			// jm�no �ablony (n�sleduje: (4) d�lka x jm�na, (x) jm�no)
#define TOKEN_STRING 2			// text (n�sleduje: (4) d�lka x textu, (x) text, (4) termin�tor st�edn�k nebo ��rka
#define TOKEN_INTEGER 3			// cel� ��slo (n�sleduje: (4) hodnota ��sla)
#define TOKEN_GUID 5			// GUID (n�sleduje: (4) pole 1, (2) pole 2, (2) pole 3, (8) pole 4)
#define TOKEN_INTEGER_LIST 6	// seznam cel�ch ��sel (n�sleduje: (4) po�et x ��sel, (4*x) ��sla)
#define TOKEN_FLOAT_LIST 7		// seznam desetinn�ch ��sel (n�sleduje: (4) po�et x ��sel, (4*x nebo 8*x) ��sla)

#define TOKEN_OBRACE 10			// lev� slo�en� z�vorka {
#define TOKEN_CBRACE 11			// prav� slo�en� z�vorka }
#define TOKEN_OPAREN 12			// lev� z�vorka (
#define TOKEN_CPAREN 13			// prav� z�vorka )
#define TOKEN_OBRACKET 14		// lev� lomen� z�vorka [
#define TOKEN_CBRACKET 15		// prav� lomen� z�vorka ]
#define TOKEN_OANGLE 16			// lev� �hlov� z�vorka
#define TOKEN_CANGLE 17			// prav� �hlov� z�vorka
#define TOKEN_DOT 18			// te�ka
#define TOKEN_COMMA 19			// ��rka
#define TOKEN_SEMICOLON 20		// st�edn�k
#define TOKEN_TEMPLATE 31		// �ablona
#define TOKEN_WORD 40			// ��slo WORD
#define TOKEN_DWORD 41			// ��slo DWORD
#define TOKEN_FLOAT 42			// ��slo FLOAT
#define TOKEN_DOUBLE 43			// ��slo DOUBLE
#define TOKEN_CHAR 44			// ��slo CHAR
#define TOKEN_UCHAR 45			// ��slo BYTE
#define TOKEN_SWORD 46			// ��slo SHORT INT
#define TOKEN_SDWORD 47			// ��slo LONG INT
#define TOKEN_VOID 48			// data
#define TOKEN_LPSTR 49			// text	zakon�en� 0
#define TOKEN_UNICODE 50		// text UNICODE
#define TOKEN_CSTRING 51		// form�tovan� text s d�lkou
#define TOKEN_ARRAY 52			// pole

/////////////////////////////////////////////////////////////////////////////
// polo�ka seznamu objekt�

typedef struct MESHITEM_
{
// obecn� parametry
	TEMPNAME		Type;			// typ objektu
	int				Level;			// �rove� v hiearchii (0 = root)
	CString			Name;			// jm�no objektu

// ---- n�sleduj� povinn� parametry objektu

// Header
	int				HeaderMaj;		// verze - major
	int				HeaderMin;		// verze - minor
	int				HeaderFlags;	// p��znaky

// Vector
	D3DVECTOR		Vector;			// data vektoru

// Coords2d
	float			CoordsU;		// sou�adnice U
	float			CoordsV;		// sou�adnice V

// Quaternion
	float			QuaterS;		// �hel oto�en� quaternionu
	D3DVECTOR		QuaterV;		// sm�r quaternionu

// Matrix4x4 a FrameTransformMatrix
	D3DMATRIX		Matrix;			// objekt Matrix4x4

// ColorRGBA a ColorRGB
	D3DCOLORVALUE	Color;			// barva

// IndexedColor
	DWORD			ColorIndex;		// index barvy
	D3DCOLORVALUE	ColorIndexCol;	// indexov� barva

// Boolean
	bool			Boolean;		// logick� hodnota

// Boolead2d
	bool			BooleanU;		// opakov�n� sou�adnice U
	bool			BooleanV;		// opakov�n� sou�adnice V

// Material
	D3DMATERIAL8	Material;		// materi�l
//	D3DCOLORVALUE	MaterialDif;	// difusn� barva materi�lu
//	float			MaterialPower;	// matnost
//	D3DCOLORVALUE	MaterialSpec;	// barva odlesku
//	D3DCOLORVALUE	MaterialEmis;	// barva sv�tivosti

// TextureFilename
	CString			TextFilename;	// jm�no souboru s texturou

// MeshFace
	int				FaceNum;		// po�et index� vrchol�
	int*			Face;			// buffer index� vrchol� plo�ek

// MeshFaceWraps
	int				FaceWrapNum;	// po�et polo�ek parametr� wrap (m�l by b�t = FaceNum)
	bool*			FaceWrap;		// buffer parametr� wrap (ka�d� polo�ka m� 2 hodnoty bool)

// MeshTextureCoords
	DWORD			TextureCoordsNum; // po�et polo�ek sou�adnic textur (0=zru�eno, jsou sam� 0)
	float*			TextureCoords;	// buffer sou�adnic textur (ka�d� polo�ka m� 2 hodnoty float)

// MeshNormals
	int				NormalNum;		// po�et norm�l
	D3DVECTOR*		Normal;			// buffer norm�l
	int				NormalFaceNum;	// po�et plo�ek definice norm�l
	int*			NormalFace;		// buffer plo�ek (st��d� se po�et index� a pole index�)

// MeshVertexColors
	int				VertexColorNum;	// po�et indexov�ch barev
	int*			VertexColorInx;	// buffer index� indexov�ch barev
	D3DCOLORVALUE*	VertexColor;	// buffer barev indexov�ch barev

// MeshMaterialList
	int				MaterialNum;	// po�et definic materi�l�
	int				MaterialFaceNum; // po�et index� plo�ek
	int*			MaterialFace;	// buffer index� plo�ek materi�l�

// Mesh
	int				MeshVerticesNum;// po�et vrchol� objektu
	D3DVECTOR*		MeshVertices;	// buffer vrchol� objektu
	int				MeshFaceNum;	// po�et plo�ek objektu (ka�d� 3 vrcholy)
	int*			MeshFace;		// buffer plo�ek objektu (ka�d� plo�ka 3 indexy)
	int				MeshFaceOldNum;	// p�vodn� po�et plo�ek (bez p�evodu na troj�heln�ky)
	int*			MeshFaceOld;	// po�et odpov�daj�c�ch nov�ch plo�ek (po 3 bodech)

// Frame

// FloatKeys
//	DWORD			FloatKeysNum;	// po�et kl���
//	float*			FloatKeys;		// buffer kl���

// TimedFloatKeys
//	DWORD			FloatTime;		// po�et �asov�ch kl���
//	DWORD*			FloatTimeKeysNum; // po�ty polo�ek �asov�ch kl���
//	DWORD			FloatTimeKeysSize; // velikost bufferu �asov�ch kl��� (polo�ek float)
//	float*			FloatTimeKeys;	// buffer �asov�ch kl��� (kl��e jsou sekven�n� za sebou)

// AnimationKey
//	DWORD			AnimKeyType;	// typ kl��e
//	DWORD			AnimKeysNum;	// po�et kl���

// AnimationOptions
//	DWORD			AnimOpen;		// p��znak otev�en� animace
//	DWORD			AnimQuality;	// kvalita animace

// Animation

// AnimationSet

// Patch
//	DWORD			PatchIndNum;	// po�et index� patch
//	DWORD*			PatchInd;		// buffer index� patch

// PatchMesh
//	DWORD			PatchVertNum;	// po�et vrchol�
//	D3DVECTOR*		PatchVer;		// buffer vrchol�
//	DWORD			PatchNum;		// po�et patch
//	DWORD*			Patch;			// buffer patch

// VertexDuplicationIndices
//	DWORD			DuplIndNum;		// po�et index�
//	DWORD			DuplIndOrig;	// po�et p�vodn�ch vrchol�
//	DWORD*			DuplInd;		// buffer index�

// XSkinMeshHeader
//	WORD			MaxSkinVertWeight; // max. v�ha na vrchol
//	WORD			MaxSkinFaceWeight; // max. v�ha na plo�ku
//	WORD			Bones;				// po�et kost�

// SkinWeights
	CString			SkinNodeName;	// jm�no v�tve
//	DWORD			SkinWeightNum;	// po�et vah
//	DWORD*			SkinVertInd;	// buffer index� vrchol�
//	float*			SkinWeight;		// buffer vah vrchol�
//	D3DMATRIX		SkinMatrix;		// offset

} MESHITEM;

/////////////////////////////////////////////////////////////////////////////
// data

// buffer se vstupn�mi daty
BYTE*		MeshData = NULL;		// data s definic� objektu
int			MeshDataN = 0;			// velikost dat (bajt�)
CString		MeshPath;				// domovsk� cesta do adres��e objektu (pr�zdn� = nen�)

// pracovn� prom�nn�
int			MeshDataR = 0;			// ukazatel �ten�ch dat z bufferu (ukazuje i za konec dat)
CString		MeshText;				// na�ten� textov� �et�zec
int			MeshArrayN = 0;			// ��ta� na��tan�ch dat z pole v bin�rn�m tvaru (0=nen�)
bool		MeshArrayTypeFloat = false;	// typ polo�ek na��tan�ho pole v bin�rn�m tvaru je FLOAT
bool		MeshNumIsValid = false;	// p��znak, �e bylo na�teno platn� ��slo

int			MeshLevel = 0;			// aktu�ln� �rove� vno�en� objekt�

// buffer objekt�
MESHITEM*	Data = NULL;			// buffer dat objekt�
int			DataNum = 0;			// po�et polo�ek v bufferu
int			DataMax = 0;			// velikost bufferu (polo�ek)

// parametry na�ten� ze z�hlav�
int			MeshMajorVer;			// hlavn� ��slo verze
int			MeshMinorVer;			// vedlej�� ��slo verze
bool		MeshTextFormat;			// je textov� form�t dat
bool		MeshDoubleFormat;		// je velikost dat Double

// na�ten� bajtu z bufferu (nejsou-li dal�� data, vrac� 0);
BYTE MeshReadByte()
{
	int r = MeshDataR;
	MeshDataR = r + 1;
	if (r < MeshDataN)
	{
		return MeshData[r];
	}
	else
	{
		return 0;
	}
}

// navr�cen� bajtu do bufferu (vr�t� ukazatel)
void MeshReadBack()
{
	if (MeshDataR > 0) MeshDataR--;
}

// vypu�t�n� mezer p�ed textem (v�etn� koment���)
void MeshReadFlush()
{
	for (;;)
	{
		BYTE data = MeshReadByte();

		if (data == '/')
		{
			data = MeshReadByte();

			if (data == '/')
			{
				data = '#';
			}
			else
			{
				MeshReadBack();
			}
		}

		if (data == '#')
		{
			do {
				data = MeshReadByte();
			} while ((data != 13) && (data != 10) && (data != 0));
			MeshReadBack();
		}
		else
		{
			if ((data == 0) || (data > 32))
			{
				MeshReadBack();
				break;
			}
		}
	}
}

// na�ten� bin�rn�ch data
void MeshReadData(void* data, int num)
{
	for (; num > 0; num--)
	{
		*(BYTE*)data = MeshReadByte();
		data = ((BYTE*)data + 1);
	}
}

// na�ten� textu ��sla (dot = te�ka povolena)
void MeshReadNum(bool dot)
{
	MeshText.Empty();
	MeshReadFlush();

	bool ignor = false;		// ignorovat zbytek ��sla (za te�kou)
	bool first = true;		// je to prvn� znak

	for (;;)
	{
		BYTE data = MeshReadByte();

		switch (data)
		{
		case 'E':
		case 'e':
			if (first)
			{
				MeshReadBack();
				return;
			}
			ignor = false;

		case '-':
		case '+':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if (!ignor)
			{
				MeshText += (TCHAR)data;
			}
			break;

	// nen�-li te�ka povolen�, zbytek ��sla se ignoruje
		case '.':
			if (dot)
			{
				MeshText += (TCHAR)data;
			}
			else
			{
				ignor = true;
			}
			break;

		default:
			MeshReadBack();
			return;
		}

		first = false;
	}
}

// na�ten� ��sla BYTE nebo UCHAR
BYTE MeshReadUChar()
{
	if (MeshTextFormat)
	{
		MeshReadNum(false);
		return (BYTE)Int(MeshText);
	}
	else
	{
		return MeshReadByte();
	}
}

// na�ten� ��sla CHAR
char MeshReadChar()
{
	if (MeshTextFormat)
	{
		MeshReadNum(false);
		return (char)Int(MeshText);
	}
	else
	{
		return (char)MeshReadByte();
	}
}

// na�ten� ��sla WORD
WORD MeshReadWord()
{
	if (MeshTextFormat)
	{
		MeshReadNum(false);
		return (WORD)Int(MeshText);
	}
	else
	{
		WORD data;
		MeshReadData(&data, sizeof(WORD));
		return data;
	}
}

// na�ten� ��sla SWORD
short int MeshReadSWord()
{
	if (MeshTextFormat)
	{
		MeshReadNum(false);
		return (short int)Int(MeshText);
	}
	else
	{
		short int data;
		MeshReadData(&data, sizeof(short int));
		return data;
	}
}

// na�ten� ��sla DWORD
DWORD MeshReadDWord()
{
	if (MeshTextFormat)
	{
		MeshReadNum(false);
		return (DWORD)Int(MeshText);
	}
	else
	{
		DWORD data;
		MeshReadData(&data, sizeof(DWORD));
		return data;
	}
}

// na�ten� ��sla SDWORD
long int MeshReadSDWord()
{
	if (MeshTextFormat)
	{
		MeshReadNum(false);
		return (long int)Int(MeshText);
	}
	else
	{
		long int data;
		MeshReadData(&data, sizeof(long int));
		return data;
	}
}

// na�ten� ��sla FLOAT
float MeshReadFloat()
{
	if (MeshTextFormat)
	{
		MeshReadNum(true);
		return (float)Double(MeshText);
	}
	else
	{
		float data;
		MeshReadData(&data, sizeof(float));
		return data;
	}
}

// na�ten� ��sla DOUBLE
double MeshReadDouble()
{
	if (MeshTextFormat)
	{
		MeshReadNum(true);
		return Double(MeshText);
	}
	else
	{
		double data;
		MeshReadData(&data, sizeof(double));
		return data;
	}
}

// na�ten� jednoznakov�ho slova (textov� tvar)
bool _fastcall MeshReadTokenText(BYTE token)
{
	MeshReadFlush();
	BYTE data = MeshReadByte();
	if (data == token) return true;
	MeshReadBack();
	return false;
}

// na�ten� jednoznakov�ho slova (bin�rn� tvar)
bool _fastcall MeshReadTokenBin(WORD token)
{
	if (MeshArrayN > 0) return false;

	WORD data = MeshReadWord();
	if (data != token)
	{
		MeshReadBack();
		MeshReadBack();
		return false;
	}
	return true;
}

// na�ten� slova (bin�rn� nebo textov� tvar)
bool _fastcall MeshReadToken(WORD tokenbin, BYTE tokentext)
{
	if (MeshTextFormat)
	{
		return MeshReadTokenText(tokentext);
	}
	else
	{
		return MeshReadTokenBin(tokenbin);
	}
}

// na�ten� lev� slo�en� z�vorky
bool MeshReadOBrace()
{
	if (MeshReadToken(TOKEN_OBRACE, '{'))
	{
		MeshLevel++;
		return true;
	}
	return false;
}

// na�ten� prav� slo�en� z�vorky
bool MeshReadCBrace()
{
	if (MeshReadToken(TOKEN_CBRACE, '}'))
	{
		if (MeshLevel > 0) MeshLevel--;
		return true;
	}
	return false;
}

// na�ten� ��rky nebo st�edn�ku
bool MeshReadCommaSemicolon()
{
	return (bool)
		(MeshReadToken(TOKEN_SEMICOLON, ';') ||
		MeshReadToken(TOKEN_COMMA, ','));
}

// na�ten� cel�ho ��sla (automatick� form�t)
int MeshReadInt()
{
	MeshNumIsValid = false;

	if (MeshTextFormat)
	{
		MeshReadNum(false);
		if (MeshText.IsNotEmpty())
		{
			MeshNumIsValid = true;
		}
		int num = Int(MeshText);
		if (MeshReadCommaSemicolon())
		{
			MeshNumIsValid = true;
		}
		return num;
	}
	else
	{

// na�ten� ��sla z pole
		if (MeshArrayN > 0)
		{
			MeshNumIsValid = true;

			MeshArrayN--;

			if (MeshArrayTypeFloat)
			{
				if (MeshDoubleFormat)
				{
					return (int)MeshReadDouble();
				}
				else
				{
					return (int)MeshReadFloat();
				}
			}
			else
			{
				return (int)MeshReadDWord();
			}
		}

// na�ten� dal��ho slova
		WORD token = MeshReadWord();

// za��tek pole - cel� ��sla
		if (token == TOKEN_INTEGER_LIST)
		{
			MeshArrayTypeFloat = false;
			MeshArrayN = (int)MeshReadDWord();
			return MeshReadInt();
		}

// za��tek pole - desetinn� ��sla
		if (token == TOKEN_FLOAT_LIST)
		{
			MeshArrayTypeFloat = true;
			MeshArrayN = (int)MeshReadDWord();
			return MeshReadInt();
		}
		   
// cel� ��slo
		if (token == TOKEN_INTEGER)
		{
			int num = (int)MeshReadDWord();
			MeshReadCommaSemicolon();
			MeshNumIsValid = true;
			return num;
		}

		if (token == TOKEN_WORD)
		{
			int num = (int)MeshReadWord();
			MeshReadCommaSemicolon();
			MeshNumIsValid = true;
			return num;
		}

		if (token == TOKEN_DWORD)
		{
			int num = (int)MeshReadDWord();
			MeshReadCommaSemicolon();
			MeshNumIsValid = true;
			return num;
		}

		if (token == TOKEN_FLOAT)
		{
			int num = (int)MeshReadFloat();
			MeshReadCommaSemicolon();
			MeshNumIsValid = true;
			return num;
		}

		if (token == TOKEN_DOUBLE)
		{
			int num = (int)MeshReadDouble();
			MeshReadCommaSemicolon();
			MeshNumIsValid = true;
			return num;
		}

		if (token == TOKEN_CHAR)
		{
			int num = (int)MeshReadChar();
			MeshReadCommaSemicolon();
			MeshNumIsValid = true;
			return num;
		}

		if (token == TOKEN_UCHAR)
		{
			int num = (int)MeshReadUChar();
			MeshReadCommaSemicolon();
			MeshNumIsValid = true;
			return num;
		}

		if (token == TOKEN_SWORD)
		{
			int num = (int)MeshReadSWord();
			MeshReadCommaSemicolon();
			MeshNumIsValid = true;
			return num;
		}

		if (token == TOKEN_SDWORD)
		{
			int num = (int)MeshReadSDWord();
			MeshReadCommaSemicolon();
			MeshNumIsValid = true;
			return num;
		}

// neplatn� slovo - n�vrat
		MeshReadBack();
		MeshReadBack();
		return 0;
	}
}

// na�ten� cel�ho ��sla s omezen�m na 64KB
int MeshReadInt64()
{
	int i = MeshReadInt();
	if (i < 0) i = 0;
	if (i > 0x1000000) i = 0x1000000;
	return i;
}


// na�ten� desetinn�ho ��sla (automatick� form�t)
float MeshReadReal()
{
	MeshNumIsValid = false;

	if (MeshTextFormat)
	{
		MeshReadNum(true);
		if (MeshText.IsNotEmpty())
		{
			MeshNumIsValid = true;
		}
		float num = (float)Double(MeshText);
		if (MeshReadCommaSemicolon())
		{
			MeshNumIsValid = true;
		}
		return num;
	}
	else
	{

// na�ten� ��sla z pole
		if (MeshArrayN > 0)
		{
			MeshNumIsValid = true;

			MeshArrayN--;

			if (MeshArrayTypeFloat)
			{
				if (MeshDoubleFormat)
				{
					return (float)MeshReadDouble();
				}
				else
				{
					return MeshReadFloat();
				}
			}
			else
			{
				return (float)MeshReadDWord();
			}
		}

// na�ten� dal��ho slova
		WORD token = MeshReadWord();

// za��tek pole - cel� ��sla
		if (token == TOKEN_INTEGER_LIST)
		{
			MeshArrayTypeFloat = false;
			MeshArrayN = (int)MeshReadDWord();
			return MeshReadReal();
		}

// za��tek pole - desetinn� ��sla
		if (token == TOKEN_FLOAT_LIST)
		{
			MeshArrayTypeFloat = true;
			MeshArrayN = (int)MeshReadDWord();
			return MeshReadReal();
		}
		   
// cel� ��slo
		if (token == TOKEN_INTEGER)
		{
			int num = (int)MeshReadDWord();
			MeshReadCommaSemicolon();
			MeshNumIsValid = true;
			return (float)num;
		}

		if (token == TOKEN_WORD)
		{
			WORD num = MeshReadWord();
			MeshReadCommaSemicolon();
			MeshNumIsValid = true;
			return (float)num;
		}

		if (token == TOKEN_DWORD)
		{
			DWORD num = MeshReadDWord();
			MeshReadCommaSemicolon();
			MeshNumIsValid = true;
			return (float)num;
		}

		if (token == TOKEN_FLOAT)
		{
			float num = MeshReadFloat();
			MeshReadCommaSemicolon();
			MeshNumIsValid = true;
			return num;
		}

		if (token == TOKEN_DOUBLE)
		{
			double num = MeshReadDouble();
			MeshReadCommaSemicolon();
			MeshNumIsValid = true;
			return (float)num;
		}

		if (token == TOKEN_CHAR)
		{
			char num = MeshReadChar();
			MeshReadCommaSemicolon();
			MeshNumIsValid = true;
			return (float)num;
		}

		if (token == TOKEN_UCHAR)
		{
			BYTE num = MeshReadUChar();
			MeshReadCommaSemicolon();
			MeshNumIsValid = true;
			return (float)num;
		}

		if (token == TOKEN_SWORD)
		{
			short int num = MeshReadSWord();
			MeshReadCommaSemicolon();
			MeshNumIsValid = true;
			return (float)num;
		}

		if (token == TOKEN_SDWORD)
		{
			long int num = MeshReadSDWord();
			MeshReadCommaSemicolon();
			MeshNumIsValid = true;
			return (float)num;
		}

// neplatn� slovo - n�vrat
		MeshReadBack();
		MeshReadBack();
		return 0;
	}
}

// na�ten� vektoru
void MeshReadVector(D3DVECTOR* v)
{
	v->x = MeshReadReal();
	v->y = MeshReadReal();
	v->z = MeshReadReal();
	MeshReadCommaSemicolon();
}

// na�ten� matice
void MeshReadMatrix(D3DMATRIX* m)
{
	float* mm = &(m->_11);
	for (int i = 15; i >= 0; i--)
	{
		*mm = MeshReadReal();
		mm++;
	}
	MeshReadCommaSemicolon();
}

// na�ten� barvy RGB
void MeshReadRGB(D3DCOLORVALUE* col)
{
	col->r = MeshReadReal();
	col->g = MeshReadReal();
	col->b = MeshReadReal();
	col->a = 1;
	MeshReadCommaSemicolon();
}

// na�ten� barvy RGBA
void MeshReadRGBA(D3DCOLORVALUE* col)
{
	col->r = MeshReadReal();
	col->g = MeshReadReal();
	col->b = MeshReadReal();
	col->a = MeshReadReal();
	MeshReadCommaSemicolon();
}

// p�esko�en� zbyl�ch polo�ek pole
void MeshReadFlushArray()
{
	do {
		MeshReadInt();
	} while (MeshNumIsValid);
}

// na�ten� jm�na
void MeshReadName()
{
	MeshText.Empty();
	if (MeshTextFormat)
	{
		MeshReadFlush();

		bool first = true;

		for (;;)
		{
			BYTE data = MeshReadByte();

			if (((data < 'A') || (data > 'Z')) &&
				((data < 'a') || (data > 'z')) &&
				(data != '_') &&
				(data != '-') &&
				((data < '0') || (data > '9') || first))
			{
				MeshReadBack();
				return;
			}

			MeshText += TCHAR(data);
			first = false;
		}
	}
	else
	{
		if (!MeshReadTokenBin(TOKEN_NAME)) return;

		DWORD count = MeshReadDWord();
		if (count > 2048) count = 2048;

		for (; count != 0; count--)
		{
			BYTE data = MeshReadByte();
			if (data != 0) MeshText += TCHAR(data);
		}
	}
}

// na�ten� textu (jm�na souboru textury)
void MeshReadString()
{
	MeshText.Empty();
	if (MeshTextFormat)
	{
		MeshReadFlush();

		BYTE data = MeshReadByte();
		if (data == '"')
		{
			for (;;)
			{
				BYTE data = MeshReadByte();

				if ((data == 0) || (data == 13) || (data == 10))
				{
					MeshReadBack();
					break;
				}

				if (data == '"')
				{
					break;
				}

				MeshText += TCHAR(data);
			}
		}
		else
		{
			MeshReadBack();

			for (;;)
			{
				BYTE data = MeshReadByte();

				if (data == '"')
				{
					break;
				}

				if ((data <= 32) ||
					(data == ';') ||
					(data == ',') ||
					(data == '*') ||
					(data == '|') ||
					(data == '<') ||
					(data == '>') ||
					(data == '?') ||
					(data == '/'))
				{
					MeshReadBack();
					break;
				}

				MeshText += TCHAR(data);
			}
		}
	}
	else
	{
		if (MeshReadTokenBin(TOKEN_STRING))
		{
			DWORD count = MeshReadDWord();
			if (count > 2048) count = 2048;

			for (; count != 0; count--)
			{
				BYTE data = MeshReadByte();
				if (data != 0) MeshText += TCHAR(data);
			}
		}
	}

	MeshReadCommaSemicolon();
}

// na�ten� 1 textov�ho znaku do bufferu textu
void MeshRead1Text()
{
	BYTE data = MeshReadByte();
	if (data == 0)
	{
		MeshReadBack();
	}
	else
	{
		MeshText += TCHAR(data);
	}
}

// na�ten� textu o d�lce 3 znaky ze z�hlav� + 1 mezera za textem se vypust�
void ReadMeshHeadText()
{
	MeshText.Empty();		// vypr�zdn�n� bufferu textu
	MeshReadFlush();		// vypu�t�n� mezer

	MeshRead1Text();		// na�ten� 1. znaku
	MeshRead1Text();		// na�ten� 2. znaku
	MeshRead1Text();		// na�ten� 3. znaku

	MeshText.LowerCase();	// p�evod na mal� p�smena

	if (MeshReadByte() != ' ') MeshReadBack(); // vypu�t�n� 1 mezery
}

// na�ten� ��sla o pevn� d�lce ze z�hlav� (jako text)
int ReadMeshHeadNum(int len)
{
	MeshText.Empty();		// vypr�zdn�n� bufferu textu
	MeshReadFlush();		// vypu�t�n� mezer

	for (; len > 0; len--)
	{
		BYTE data = MeshReadByte();
		if ((data < '0') || (data > '9'))
		{
			MeshReadBack();
		}
		else
		{
			MeshText += TCHAR(data);
		}
	}

	return Int(MeshText);
}

// na�ten� z�hlav� objektu
void ReadMeshHead()
{
// implicitn� parametry souboru
	MeshMajorVer = 3;					// implicitn� hlavn� ��slo verze
	MeshMinorVer = 2;					// implicitn� vedlej�� ��slo verze
	MeshTextFormat = true;				// implicitn� bude textov� form�t
	MeshDoubleFormat = false;			// implicitn� jsou data float

// na�ten� z�hlav� "xof"
	int olddatar = MeshDataR;			// �schova ukazatele dat

	ReadMeshHeadText();					// na�ten� identifik�toru z�hlav�
	if (MeshText == "xof")
	{

// na�ten� hlavn�ho ��sla verze
		olddatar = MeshDataR;			// �schova ukazatele dat
		
		int n = ReadMeshHeadNum(2);
		if (n > 0)
		{
			MeshMajorVer = n;

// na�ten� vedlej��ho ��sla verze
			MeshMinorVer = ReadMeshHeadNum(2);
			olddatar = MeshDataR;		// �schova ukazatele dat

// na�ten� identifik�toru form�tu
			ReadMeshHeadText();

			if (MeshText == "bin")
			{
				olddatar = MeshDataR;			// �schova ukazatele dat
				MeshTextFormat = false;
			}
			else
			{
				if (MeshText == "txt")
				{
					olddatar = MeshDataR;			// �schova ukazatele dat
				}
				else
				{
					if (MeshText == "com")
					{
						MeshTextFormat = false;
						ReadMeshHeadText();				// na�ten� typu komprese
						olddatar = MeshDataR;			// �schova ukazatele dat
					}
					else
					{
						MeshDataR = olddatar;
					}
				}
			}

// na�ten� rozm�ru ��sel
			n = ReadMeshHeadNum(4);
			if (n == 64)
			{
				MeshDoubleFormat = true;
				olddatar = MeshDataR;			// �schova ukazatele dat
			}

			if (n == 32)
			{
				olddatar = MeshDataR;			// �schova ukazatele dat
			}
		}
	}
	
	MeshDataR = olddatar;
}



// na�ten� jm�na �ablony (-1 = nen�)
int ReadTemplateName()
{
	MeshReadName();

	if (MeshText.IsEmpty()) return -1;

// vyhled�n� jm�na �ablony
	for (int i = 0; i < TEMPNAME_NUM; i++)
	{
		if (MeshText == TempNames[i])
		{
			return (i + TEMPNAME_FIRST);
		}
	}

// druh� pokus - bez rozli�en� velk�ch a mal�ch p�smen
	MeshText.UpperCase();

	for (i = 0; i < TEMPNAME_NUM; i++)
	{
		if (MeshText == TempNamesUpper[i])
		{
			return (i + TEMPNAME_FIRST);
		}
	}

// na�ten� bin�rn�ch k�d�
	if (!MeshTextFormat)
	{
		if (MeshReadTokenBin(TOKEN_TEMPLATE)) return TOKEN_TEMPLATE;
	}

	return -1;
}

// duplikace datov�ho bloku
void DuplikData(void** adr, int size)
{
	if (*adr == NULL) return;

	void* dst = MemGet(size);
	MemCopy(dst, *adr, size);
	*adr = dst;
}


// na�ten� objekt� jedn� hladiny
void ReadMeshLevel()
{

// na�ten� jm�na �ablony
	for (;;)
	{

// test, zda je vno�en� objekt {}
		if (MeshReadOBrace())
		{
			MeshReadName();

			if (MeshText.IsNotEmpty())
			{
				int aktualinx = DataNum;

				for (int i = 0; i < aktualinx; i++)
				{
					if (MeshText == Data[i].Name)
					{
						int toplevel = Data[i].Level;

// kopie objektu (a v�ech vno�en�ch objekt�)

						do {
							int j = DataNum;

							if (j == DataMax)
							{
								DataMax = j*2;
								MemBuf(Data, DataMax);
							}
							DataNum = j + 1;
							MESHITEM* item = Data + j;
							MemCopy(item, Data + i, sizeof(MESHITEM));
							item->Level += (MeshLevel - toplevel);

							CString txt2 = item->Name;
							item->Name.Init(txt2);
							txt2 = item->TextFilename;
							item->TextFilename.Init(txt2);
							txt2 = item->SkinNodeName;
							item->SkinNodeName.Init(txt2);

							DuplikData((void**)&item->Face, item->FaceNum * sizeof(int));
							DuplikData((void**)&item->FaceWrap, item->FaceWrapNum * 2 * sizeof(bool));
							DuplikData((void**)&item->TextureCoords, item->TextureCoordsNum * 2 * sizeof(float));
							DuplikData((void**)&item->Normal, item->NormalNum * sizeof(D3DVECTOR));
							DuplikData((void**)&item->NormalFace, item->NormalFaceNum * 3 * sizeof(int));
							DuplikData((void**)&item->VertexColorInx, item->VertexColorNum * sizeof(int));
							DuplikData((void**)&item->VertexColor, item->VertexColorNum * sizeof(D3DCOLORVALUE));
							DuplikData((void**)&item->MaterialFace, item->MaterialFaceNum * sizeof(int));
							DuplikData((void**)&item->MeshVertices, item->MeshVerticesNum * sizeof(D3DVECTOR));
							DuplikData((void**)&item->MeshFace, item->MeshFaceNum * 3 * sizeof(int));
							DuplikData((void**)&item->MeshFaceOld, item->MeshFaceOldNum * sizeof(int));

							i++;

						} while ((i < aktualinx) && (Data[i].Level > toplevel));

						break;
					}
				}
			}

			MeshReadFlushArray();
			ReadMeshLevel();
			MeshReadCBrace();
			continue;
		}

// na�ten� identifik�toru �ablony
		int tempID = ReadTemplateName();
		if (tempID < 0) return;

// p�esko�en� template
		if (tempID == TEMP_TEMPLATE)
		{
			if (MeshTextFormat)
			{
				MeshReadName();
				if (MeshReadOBrace())
				{
					for (;;)
					{
						BYTE data = MeshReadByte();
						if (data == 0)
						{
							MeshReadBack();
							break;
						}

						if (data == '}')
						{
							MeshLevel--;
							break;
						}
					}
				}
			}
			continue;
		}

// vytvo�en� polo�ky objektu
		int i = DataNum;	// aktu�l� po�et polo�ek
		if (i == DataMax)
		{
			if (i == 0)
			{
				DataMax = 64;
			}
			else
			{
				DataMax = i*2;
			}
			MemBuf(Data, DataMax);
		}
		DataNum = i + 1;
		MESHITEM* item = Data + i;
		MemFill(item, sizeof(MESHITEM));
		item->Type = (TEMPNAME)tempID;
		item->Level = MeshLevel;
		item->Name.Init();
		item->TextFilename.Init();
		item->SkinNodeName.Init();

// na�ten� jm�na objektu
		MeshReadName();
		item->Name = MeshText;

// otev�en� definice objektu
		if (MeshReadOBrace())
		{

// na�ten� povinn� definice polo�ky
			switch(tempID)
			{
			case TEMP_HEADER:
				item->HeaderMaj = MeshReadInt();
				item->HeaderMin = MeshReadInt();
				item->HeaderFlags = MeshReadInt();
				break;

			case TEMP_VECTOR:
				MeshReadVector(&(item->Vector));
				break;

			case TEMP_COORDS2D:
				item->CoordsU = MeshReadReal();
				item->CoordsV = MeshReadReal();
				break;

			case TEMP_QUATERNION:
				item->QuaterS = MeshReadReal();
				MeshReadVector(&(item->QuaterV));
				break;

			case TEMP_MATRIX4X4:
			case TEMP_FRAMETRANSFORMMATRIX:
				MeshReadMatrix(&(item->Matrix));
				break;

			case TEMP_COLORRGBA:
				MeshReadRGBA(&(item->Color));
				break;

			case TEMP_COLORRGB:
				MeshReadRGB(&(item->Color));
				break;

			case TEMP_INDEXEDCOLOR:
				item->ColorIndex = MeshReadInt();
				MeshReadRGBA(&(item->ColorIndexCol));
				break;

			case TEMP_BOOLEAN:
				item->Boolean = (MeshReadInt() != 0);
				break;

			case TEMP_BOOLEAN2D:
				item->BooleanU = (MeshReadInt() != 0);
				item->BooleanV = (MeshReadInt() != 0);
				break;

			case TEMP_MATERIAL:
				MeshReadRGBA(&(item->Material.Diffuse));
				item->Material.Ambient = item->Material.Diffuse;
				item->Material.Power = MeshReadReal();
				if (item->Material.Power > 128) item->Material.Power = 128;
				MeshReadRGB(&(item->Material.Specular));
				MeshReadRGB(&(item->Material.Emissive));
//				MeshReadRGBA(&(item->MaterialDif));
//				item->MaterialPower = MeshReadReal();
//				MeshReadRGB(&(item->MaterialSpec));
//				MeshReadRGB(&(item->MaterialEmis));
				break;				

			case TEMP_TEXTUREFILENAME:
				MeshReadString();
				MeshText.TrimLeft();
				MeshText.TrimRight();
				item->TextFilename = MeshText;
				break;

			case TEMP_MESHFACE:
				{
					int i = MeshReadInt64();
					item->FaceNum = i;
					MemBuf(item->Face, i);
					for (int j = 0; j < i; j++)
					{
						item->Face[j] = MeshReadInt64();
					}
					MeshReadCommaSemicolon();
				}
				break;

			case TEMP_MESHFACEWRAPS:
				{
					int i = MeshReadInt64();
					item->FaceWrapNum = i;
					MemBuf(item->FaceWrap, i * 2);
					for (int j = 0; j < i; j++)
					{
						item->FaceWrap[2*j] = (MeshReadInt() != 0);
						item->FaceWrap[2*j+1] = (MeshReadInt() != 0);
						MeshReadCommaSemicolon();
					}
				}
				break;

			case TEMP_MESHTEXTURECOORDS:
				{
					int i = MeshReadInt64();
					item->TextureCoordsNum = i;
					MemBuf(item->TextureCoords, i * 2);
					bool nzero = false;

					for (int j = 0; j < i; j++)
					{
						float c = MeshReadReal();
						item->TextureCoords[2*j] = c;
						if (c != 0) nzero = true;
						c = MeshReadReal();
						item->TextureCoords[2*j+1] = c;
						if (c != 0) nzero = true;
						MeshReadCommaSemicolon();
					}
					if (!nzero) item->TextureCoordsNum = 0;
				}
				break;

			case TEMP_MESHNORMALS:
				{
					int normals = MeshReadInt64();
					item->NormalNum = normals;
					MemBuf(item->Normal, normals);
					for (int j = 0; j < normals; j++)
					{
						MeshReadVector(item->Normal + j);
					}

					int i = MeshReadInt64();
					item->NormalFaceNum = 0;

					int max = 0;
					int num = 0;

					for (; i > 0; i--)
					{
						int n0 = MeshReadInt64();

						if (n0 > 0)
						{
							int n = n0 - 2;
							if (n <= 0) n = 1;

							item->NormalFaceNum += n;

							while (3*n + num > max)
							{
								max = 2*max;
								if (max == 0) max = 64;
								MemBuf(item->NormalFace, max);
							}

							int m0 = MeshReadInt64();
							if (m0 >= normals) m0 = 0;
							item->NormalFace[num] = m0;
							num++;

							int m1 = m0;
							if (n0 > 1)
							{
								m1 = MeshReadInt64();
							}
							if (m1 >= normals) m1 = 0;
							item->NormalFace[num] = m1;
							num++;

							if (n0 > 2)
							{
								m1 = MeshReadInt64();
							}
							if (m1 >= normals) m1 = 0;
							item->NormalFace[num] = m1;
							num++;

							for (n--; n > 0; n--)
							{
								item->NormalFace[num] = m0;
								num++;

								item->NormalFace[num] = item->NormalFace[num-2];
								num++;

								m1 = MeshReadInt64();
								if (m1 >= normals) m1 = 0;
								item->NormalFace[num] = m1;
								num++;
							}
						}

						MeshReadCommaSemicolon();
					}
					MeshReadCommaSemicolon();
				}
				break;

			case TEMP_MESHVERTEXCOLORS:
				{
					int i = MeshReadInt64();
					item->VertexColorNum = i;

					MemBuf(item->VertexColorInx, i);
					MemBuf(item->VertexColor, i);

					for (int j = 0; j < i; j++)
					{
						item->VertexColorInx[j] = MeshReadInt64();
						MeshReadRGBA(item->VertexColor + j);
						MeshReadCommaSemicolon();
					}
				}
				break;

			case TEMP_MESHMATERIALLIST:
				{
					item->MaterialNum = MeshReadInt64();

					int i = MeshReadInt64();
					item->MaterialFaceNum = i;
					MemBuf(item->MaterialFace, i);

					for (int j = 0; j < i; j++)
					{
						item->MaterialFace[j] = MeshReadInt64();
					}
				}
				break;

			case TEMP_MESH:
				{
					int vertices = MeshReadInt64();
					item->MeshVerticesNum = vertices;
					MemBuf(item->MeshVertices, vertices);
					for (int j = 0; j < vertices; j++)
					{
						MeshReadVector(item->MeshVertices + j);
					}

					int i = MeshReadInt64();
					item->MeshFaceOldNum = i;
					MemBuf(item->MeshFaceOld, i);

					item->MeshFaceNum = 0;

					int max = 0;
					int num = 0;

					j = 0;
					for (; i > 0; i--)
					{
						int n = MeshReadInt64() - 2;

						if (n > 0)
						{
							item->MeshFaceNum += n;

							item->MeshFaceOld[j] = n;

							while (3*n + num > max)
							{
								max = 2*max;
								if (max == 0) max = 64;
								MemBuf(item->MeshFace, max);
							}

							int m0 = MeshReadInt64();
							if (m0 >= vertices) m0 = 0;
							item->MeshFace[num] = m0;
							num++;

							int m1 = MeshReadInt64();
							if (m1 >= vertices) m1 = 0;
							item->MeshFace[num] = m1;
							num++;

							m1 = MeshReadInt64();
							if (m1 >= vertices) m1 = 0;
							item->MeshFace[num] = m1;
							num++;

							for (n--; n > 0; n--)
							{
								item->MeshFace[num] = m0;
								num++;

								item->MeshFace[num] = item->MeshFace[num-2];
								num++;

								m1 = MeshReadInt64();
								if (m1 >= vertices) m1 = 0;
								item->MeshFace[num] = m1;
								num++;
							}
						}
						else
						{
							if (n == 0) MeshReadInt();	// 2 vrcholy
							if (n == -1) MeshReadInt();	// 1 vrchol
							item->MeshFaceOld[j] = 0;
						}

						MeshReadCommaSemicolon();
						j++;
					}
					MeshReadCommaSemicolon();
				}
				break;

			case TEMP_FRAME:
				break;

			case TEMP_FLOATKEYS:
/*
				{
					int i = MeshReadInt64();
					item->FloatKeysNum = i;
					MemBuf(item->FloatKeys, i);
					for (int j = 0; j < i; j++)
					{
						item->FloatKeys[j] = MeshReadReal();
					}
				}
*/
				break;

			case TEMP_TIMEDFLOATKEYS:
			/*
				{
					int i = MeshReadInt64();
					item->FloatTime = i;
					MemBuf(item->FloatTimeKeysNum, i);

					int max = 0;
					int num = 0;

					for (int j = 0; j < i; j++)
					{
						int n = MeshReadInt64();
						item->FloatTimeKeysNum[j] = n;

						while (n + num > max)
						{
							max = 2*max;
							if (max == 0) max = 64;
							MemBuf(item->FloatTimeKeys, max);
						}

						for (; n > 0; n--)
						{
							item->FloatTimeKeys[num] = MeshReadReal();
							num++;
						}
						MeshReadCommaSemicolon();
					}

					item->FloatTimeKeysSize = num;
				}
			*/
				break;

			case TEMP_ANIMATIONKEY:
				break;

			case TEMP_ANIMATIONOPTIONS:
				break;

			case TEMP_ANIMATION:
				break;

			case TEMP_ANIMATIONSET:
				break;

			case TEMP_PATCH:
				break;

			case TEMP_PATCHMESH:
				break;

			case TEMP_VERTEXDUPLICATIONINDICES:
				break;

			case TEMP_XSKINMESHHEADER:
				break;

			case TEMP_SKINWEIGHTS:
				break;

//			case TEMP_PROGRESSIVEMESH:
//				break;

			default:
				ASSERTERROR;
			}

// p�esko�en� nadbyte�n�ch ��seln�ch polo�ek
			MeshReadFlushArray();

// na�ten� nepovinn� definice objekt�
			ReadMeshLevel();

// uzav�rac� z�vorka
			MeshReadCBrace();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nalezen� objektu (vrac� index nebo -1 = nenalezeno)

int FindObject(int first, int level, TEMPNAME type)
{
	MESHITEM* item = Data + first;

	for (; first < DataNum; first++)
	{
		if (item->Level < level) break;

		if ((item->Type == type) &&
			(item->Level == level))
		{
			return first;
		}

		item++;
	}

	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// ulo�en� definice objektu

void SetMesh(int iteminx, int datainx)
{
	D3DFITEM* dst = D3DF_Get(iteminx);
	MESHITEM* src = Data + datainx;

	int vertices = src->MeshVerticesNum;
	int faces = src->MeshFaceNum;

// test platnosti parametr�
	if ((vertices > 0) && (faces > 0))
	{
		dst->Type = D3DFTYPE_OBJECT;
		int i;

		int MeshFaceOldNum = src->MeshFaceOldNum;
		int* MeshFaceOld = src->MeshFaceOld;

// vytvo�en� buffer�
		dst->VertNum = vertices;
		dst->Vertex = (D3DVECTOR*)MemGet(vertices * sizeof(D3DVECTOR));
		dst->Normal = (D3DVECTOR*)MemGet(vertices * sizeof(D3DVECTOR));
		dst->TextUV[0] = (float*)MemGet(vertices * 2 * sizeof(float));

// p�enesen� definice vrchol�
		MemCopy(dst->Vertex, src->MeshVertices, vertices * sizeof(D3DVECTOR));

// automatick� sou�adnice textury
		for (i = 0; i < vertices; i++)
		{
			dst->TextUV[0][2*i] = (float)(dst->Vertex[i].x + 0.5);
			dst->TextUV[0][2*i+1] = (float)(0.5 - dst->Vertex[i].y);
		}

// vytvo�en� bufferu plo�ek
		int* facedst = (int*)MemGet(faces * 3 * sizeof(int));
//		MemFill(facedst, faces * 3 * sizeof(WORD), 0);
		dst->Face = facedst;
		dst->FaceNum = faces;

// na�ten� plo�ek
		MemCopy(facedst, src->MeshFace, faces * 3 * sizeof(int));

/*
		WORD* facesrc = src->MeshFace;

		for (i = 0; i < faces; i++)
		{
			WORD i1 = *facesrc;
			*facedst = i1;
			facesrc++;
			facedst++;

			WORD i2 = *facesrc;
			*facedst = i2;
			facesrc++;
			facedst++;

			WORD i3 = *facesrc;
			*facedst = i3;
			facesrc++;
			facedst++;

// v�po�et automatick� norm�ly
			D3DVECTOR v1 = dst->Vertex[facedst[0]];
			D3DVECTOR v2 = dst->Vertex[facedst[1]];
			D3DVECTOR v3 = dst->Vertex[facedst[2]];

			D3DVECTOR n1;
			D3DVECTOR n2;

			n1.x = v2.x - v1.x;
			n1.y = v2.y - v1.y;
			n1.z = v2.z - v1.z;

			n2.x = v3.x - v2.x;
			n2.y = v3.y - v2.y;
			n2.z = v3.z - v2.z;

			D3DVECTOR n0;

			n0.x = n1.y * n2.z - n1.z * n2.y;
			n0.y = n1.z * n2.x - n1.x * n2.z;
			n0.z = n1.x * n2.y - n1.y * n2.x;

			dst->Normal[facedst[0]] = n0;
			dst->Normal[facedst[1]] = n0;
			dst->Normal[facedst[2]] = n0;

//			facedst += 3;
		}
*/

// velmi p�ibli�n� nastaven� norm�l - jako vektory vrchol�
		MemCopy(dst->Normal, dst->Vertex, vertices * sizeof(D3DVECTOR));

// v�po�et hranice
		D3DF_MapBound(iteminx);


		int newlevel = src->Level + 1;

// na�ten� sou�adnice textury
		i = FindObject(datainx + 1, newlevel, TEMP_MESHTEXTURECOORDS);
		if (i >= 0)
		{
			src = Data + i;
			int n = src->TextureCoordsNum;
			if (n > vertices) n = vertices;
			MemCopy(dst->TextUV[0], src->TextureCoords, n * 2 * sizeof(float));
		}

// na�ten� barvy vrchol� (mus� b�t p�ed �ten�m materi�l�, proto�e zat�m souhlas� ��sla vrchol�)
		i = FindObject(datainx + 1, newlevel, TEMP_MESHVERTEXCOLORS);
		if (i >= 0)
		{
//			dst->AktMat = true;
			src = Data + i;
			int colors = src->VertexColorNum;
			if (colors > 0)
			{
				int j;
				if (dst->VertCol == NULL)
				{
					MemBuf(dst->VertCol, vertices);
					for (j = 0; j < vertices; j++)
					{
						dst->VertCol[j] = 0xffffffff;
					}
				}

				if (dst->VertColF == NULL)
				{
					MemBuf(dst->VertColF, vertices);
					for (j = 0; j < vertices; j++)
					{
						dst->VertColF[j].r = 1;
						dst->VertColF[j].g = 1;
						dst->VertColF[j].b = 1;
						dst->VertColF[j].a = 1;
					}
				}

				for (i = 0; i < colors; i++)
				{
					int n = src->VertexColorInx[i];
					if (n < vertices)
					{
						dst->VertColF[n].r = src->VertexColor[i].r;
						dst->VertColF[n].g = src->VertexColor[i].g;
						dst->VertColF[n].b = src->VertexColor[i].b;
						dst->VertColF[n].a = src->VertexColor[i].a;
						dst->VertCol[n] = (Round(src->VertexColor[i].b*255) & 0xff) | 
											((Round(src->VertexColor[i].g*255) & 0xff) << 8) |
											((Round(src->VertexColor[i].r*255) & 0xff) << 16) |
											((Round(src->VertexColor[i].a*255) & 0xff) << 24);
					}
				}
			}
		}

// na�ten� materi�ly plo�ek
		i = FindObject(datainx + 1, newlevel, TEMP_MESHMATERIALLIST);
		if (i >= 0)
		{
//			dst->AktMat = true;
			src = Data + i;
			int matnum = src->MaterialNum;

			int i2 = FindObject(i + 1, newlevel + 1, TEMP_MATERIAL);
			if (i2 >= 0)
			{
				MESHITEM* src2 = Data + i2;
				D3DMATERIAL8* m = &(src2->Material);
				dst->Material = D3DM_Add(m);

				dst->Diffuse = (Round(m->Diffuse.r*255) & 0xff) |
					((Round(m->Diffuse.g*255) & 0xff) << 8) |
					((Round(m->Diffuse.b*255) & 0xff) << 16) |
					(((Round(m->Diffuse.a*255) & 0xff) ^ 0xff) << 24);

				dst->Emissive = (Round(m->Emissive.r*255) & 0xff) | 
					((Round(m->Emissive.g*255) & 0xff) << 8) |
					((Round(m->Emissive.b*255) & 0xff) << 16) |
					(((Round(m->Emissive.a*255) & 0xff) ^ 0xff) << 24);

				dst->Specular = (Round(m->Specular.r*255) & 0xff) | 
					((Round(m->Specular.g*255) & 0xff) << 8) |
					((Round(m->Specular.b*255) & 0xff) << 16) |
					(((Round(m->Specular.a*255) & 0xff) ^ 0xff) << 24);

				dst->Power = m->Power;

				dst->MatSource = MATSOURCE_FRAME;

				i2 = FindObject(i2 + 1, newlevel + 2, TEMP_TEXTUREFILENAME);
				if (i2 >= 0)
				{
					MESHITEM* src2 = Data + i2;
					CString filename = src2->TextFilename;
					if (filename.IsNotEmpty())
					{
						if ((filename.Find('\\') < 0) &&
							MeshPath.IsNotEmpty())
						{
							filename = MeshPath + '\\' + filename;
						}

					// uzav�en� soubor�
						FileClose();

					// nulov�n� offsetu	pro �ten�
						FileReadOff = 0;

					// nastaven� jm�na souboru
						FileRead = filename;

					// na�ten� obr�zku
						ASSERT(dst->Texture[0] == -1);

						CPicture pic;
						pic.LoadFile(true);
						if (pic.IsNotEmpty())
						{
//							dst->AktMat = true;
							dst->Texture[0] = D3DT_Add(pic);
						}
					}
				}
			}

			int facenum = src->MaterialFaceNum;
			if (facenum > MeshFaceOldNum) facenum = MeshFaceOldNum;

			if ((matnum > 0) && (facenum > 0))
			{
				DWORD* collist = (DWORD*)MemGet(matnum * sizeof(DWORD));
				D3DCOLORVALUE* collistf = (D3DCOLORVALUE*)MemGet(matnum * sizeof(D3DCOLORVALUE));

				int j;
				i2 = i;
				for (j = 0; j < matnum; j++)
				{
					int i3 = FindObject(i2 + 1, newlevel + 1, TEMP_MATERIAL);
					if (i3 >= 0)
					{
						MESHITEM* src3 = Data + i3;
						D3DMATERIAL8* m = &(src3->Material);

						collist[j] = (Round(m->Diffuse.b*255) & 0xff) | 
							((Round(m->Diffuse.g*255) & 0xff) << 8) |
							((Round(m->Diffuse.r*255) & 0xff) << 16) |
							((Round(m->Diffuse.a*255) & 0xff) << 24);

						collistf[j].r = m->Diffuse.r;
						collistf[j].g = m->Diffuse.g;
						collistf[j].b = m->Diffuse.b;
						collistf[j].a = m->Diffuse.a;

						i2 = i3;
					}
					else
					{
						collist[j] = 0xffffffff;
						collistf[j].r = 1;
						collistf[j].g = 1;
						collistf[j].b = 1;
						collistf[j].a = 1;
					}
				}
		
				dst->MatSource = MATSOURCE_OBJECT;

				if (dst->VertCol == NULL)
				{
					MemBuf(dst->VertCol, vertices);
					for (j = 0; j < vertices; j++)
					{
						dst->VertCol[j] = collist[0];
					}
				}

				if (dst->VertColF == NULL)
				{
					MemBuf(dst->VertColF, vertices);
					for (j = 0; j < vertices; j++)
					{
						dst->VertColF[j].r = collistf[0].r;
						dst->VertColF[j].g = collistf[0].g;
						dst->VertColF[j].b = collistf[0].b;
						dst->VertColF[j].a = collistf[0].a;
					}
				}

				int max = 1;
				bool* used = (bool*)MemGet(vertices * sizeof(bool));
				MemFill(used, vertices, false);

				int p = 0;
				for (j = 0; j < facenum; j++)
				{
					int k = src->MaterialFace[j];
					if (k > matnum) k = 0;

					for (int l = MeshFaceOld[j]*3; l > 0; l--)
					{
						int p2 = dst->Face[p];

						if (used[p2])
						{
							if (
								((dst->VertCol[p2] != collist[k]) ||
								(dst->VertColF[p2].r != collistf[k].r) ||
								(dst->VertColF[p2].g != collistf[k].g) ||
								(dst->VertColF[p2].b != collistf[k].b) ||
								(dst->VertColF[p2].a != collistf[k].a)))
							{
								if (max <= vertices)
								{
									while (max <= vertices) max = (max << 1);
									MemBuf(dst->Vertex, max);
									MemBuf(dst->Normal, max);
									MemBuf(dst->TextUV[0], 2*max);
									MemBuf(dst->VertCol, max);
									MemBuf(dst->VertColF, max);
									MemBuf(used, max);
								}

								dst->Face[p] = vertices;
								MemCopy(dst->Vertex + vertices, dst->Vertex + p2, sizeof(D3DVECTOR));
								MemCopy(dst->Normal + vertices, dst->Normal + p2, sizeof(D3DVECTOR));
								dst->TextUV[0][2*vertices] = dst->TextUV[0][2*p2];
								dst->TextUV[0][2*vertices+1] = dst->TextUV[0][2*p2+1];
								used[vertices] = true;
								dst->VertCol[vertices] = collist[k];
								dst->VertColF[vertices].r = collistf[k].r;
								dst->VertColF[vertices].g = collistf[k].g;
								dst->VertColF[vertices].b = collistf[k].b;
								dst->VertColF[vertices].a = collistf[k].a;
								vertices++;
								dst->VertNum = vertices;
							}
						}
						else
						{
							used[p2] = true;
							dst->VertCol[p2] = collist[k];
							dst->VertColF[p2].r = collistf[k].r;
							dst->VertColF[p2].g = collistf[k].g;
							dst->VertColF[p2].b = collistf[k].b;
							dst->VertColF[p2].a = collistf[k].a;
						}
						p++;
					}
				}

				MemFree(used);
				MemFree(collist);
				MemFree(collistf);
			}
		}

// na�ten� norm�ly
		i = FindObject(datainx + 1, newlevel, TEMP_MESHNORMALS);
		if (i >= 0)
		{
			int max = 1;
			bool* used = (bool*)MemGet(vertices * sizeof(bool));
			MemFill(used, vertices, false);

			src = Data + i;
			int nfaces = src->NormalFaceNum;
			if (nfaces > faces) nfaces = faces;
			nfaces *= 3;

			for (i = 0; i < nfaces; i++)
			{
				int j = src->NormalFace[i];
				int k = dst->Face[i];
				if (used[k])
				{
					if (((dst->Normal[k].x != src->Normal[j].x) ||
						(dst->Normal[k].y != src->Normal[j].y) ||
						(dst->Normal[k].z != src->Normal[j].z)))
					{
						if (max <= vertices)
						{
							while (max <= vertices) max = (max << 1);
							MemBuf(dst->Vertex, max);
							MemBuf(dst->Normal, max);
							MemBuf(dst->TextUV[0], 2*max);
							if (dst->VertCol) MemBuf(dst->VertCol, max);
							if (dst->VertColF) MemBuf(dst->VertColF, max);
							MemBuf(used, max);
						}

						dst->Face[i] = vertices;
						MemCopy(dst->Vertex + vertices, dst->Vertex + k, sizeof(D3DVECTOR));
						MemCopy(dst->Normal + vertices, src->Normal + j, sizeof(D3DVECTOR));
						dst->TextUV[0][2*vertices] = dst->TextUV[0][2*k];
						dst->TextUV[0][2*vertices+1] = dst->TextUV[0][2*k+1];
						if (dst->VertCol) dst->VertCol[vertices] = dst->VertCol[k];
						if (dst->VertColF) dst->VertColF[vertices] = dst->VertColF[k];
						used[vertices] = true;
						vertices++;
						dst->VertNum = vertices;
					}
				}
				else
				{
					used[k] = true;
					MemCopy(dst->Normal + k, src->Normal + j, sizeof(D3DVECTOR));
				}
			}

			MemFree(used);
		}
		else
		{
			D3DF_AutoNormals(iteminx, 0.5);
		}
	}
}

void AddMeshes(int parent, int first, int level);

/////////////////////////////////////////////////////////////////////////////
// ulo�en� definice r�mu

void SetFrame(int iteminx, int datainx)
{
// nastaven� matice r�mu
	int i = FindObject(datainx + 1, Data[datainx].Level + 1, TEMP_FRAMETRANSFORMMATRIX);

	if (i >= 0)
	{
		D3DF_SetTransMatrix(iteminx, &(Data[i].Matrix));
	}

// nalezen� objektu pat��c�ho do r�mu
	i = FindObject(datainx + 1, Data[datainx].Level + 1, TEMP_MESH);

	if (i >= 0)
	{
		SetMesh(iteminx, i);
	}

// p�ipojen� ostatn�ch r�m�
	AddMeshes(iteminx, datainx+1, Data[datainx].Level + 1);
}

/////////////////////////////////////////////////////////////////////////////
// p�ipojen� objekt� zadan� �rovn�
// parent: 0=je prvn� objekt

void AddMeshes(int parent, int first, int level)
{
	int i;
	int oldfirst = first;

// nalezen� v�ech r�m�
	for (;;)
	{
		i = FindObject(first, level, TEMP_FRAME);

		if (i >= 0)
		{
			if (parent == 0)
			{
				if (FindObject(i + 1, Data[i].Level + 1, TEMP_FRAMETRANSFORMMATRIX) >= 0)
				{
					parent = D3D_ID;
					int inx = D3DF_New();
					SetFrame(inx, i);
					D3DF_Link(parent, inx);
				}
				else
				{
					SetFrame(D3D_ID, i);
					parent = D3D_ID;
				}
			}
			else
			{
				int inx = D3DF_New();
				SetFrame(inx, i);
				D3DF_Link(parent, inx);
			}

			first = i + 1;
		}
		else
		{
			break;
		}
	}

// k tomu p�ipojen� v�ech objekt� bez r�m� (nejsou-li r�my)
	if (parent == 0)
	{
		first = oldfirst;

		for (;;)
		{
			i = FindObject(first, level, TEMP_MESH);

			if (i >= 0)
			{
				if (parent == 0)
				{
					SetMesh(D3D_ID, i);
					parent = D3D_ID;
				}
				else
				{
					int inx = D3DF_New();
					SetMesh(inx, i);
					D3DF_Link(parent, inx);
				}

				first = i + 1;
			}
			else
			{
				break;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// na�ten� objektu z bufferu

void ReadMesh(BYTE* data, int size, CString path)
{

// �schova adresy a velikosti dat
	MeshData = data;
	MeshDataN = size;
	if (path.LastChar() == '\\') path.Delete(path.Length() - 1, 1);
	MeshPath = path;

// p��prava jmen �ablon s velk�mi p�smeny
	int i;
	if (TempNamesUpper[0].IsEmpty())
	{
		for (i = 0; i < TEMPNAME_NUM; i++)
		{
			TempNamesUpper[i] = TempNames[i];
			TempNamesUpper[i].UpperCase();
		}
	}

// inicializace pracovn�ch prom�nn�ch
	MeshDataR = 0;
	MeshArrayN = 0;
	MeshLevel = 0;

// na�ten� z�hlav� objektu
	ReadMeshHead();

// na�ten� objekt�
	ReadMeshLevel();
//	ASSERT(MeshDataR >= MeshDataN);

// vytvo�en� pr�zdn�ho objektu
	D3D_ID = D3DF_New();

// nalezen� r�mu
	AddMeshes(0, 0, 0);

// zru�en� buffer�
	for (i = 0; i < DataNum; i++)
	{
		MESHITEM* item = Data + i;
		item->Name.Term();
		item->TextFilename.Term();
		item->SkinNodeName.Term();
		MemFree(item->Face);
		MemFree(item->FaceWrap);
		MemFree(item->FaceWrap);
		MemFree(item->TextureCoords);
		MemFree(item->Normal);
		MemFree(item->NormalFace);
		MemFree(item->VertexColorInx);
		MemFree(item->VertexColor);
		MemFree(item->MaterialFace);
		MemFree(item->MeshVertices);
		MemFree(item->MeshFace);
		MemFree(item->MeshFaceOld);
	}

	MemFree(Data);
	Data = NULL;
	DataNum = 0;
	DataMax = 0;
}





#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------
