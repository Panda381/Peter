
/***************************************************************************\
*																			*
*							Obsluha ikon a bitmap							*
*																			*
\***************************************************************************/


////////////////////////////////////////////////////////////////////
// definice záhlaví souboru ICO

#pragma pack( push )
#pragma pack( 2 )

typedef struct ICONDIRENTRY_
{
	BYTE	Width;					// šíøka
	BYTE	Height;					// výška
	BYTE	Colors;					// poèet barev (0 = 256 barev)
	BYTE	Reserved;				// ... rezervováno, musí být vždy 0
	WORD	Planes;					// poèet barevných rovin (nastaveno na 0)
	WORD	Bits;					// poèet bitù na bod (nastaveno na 0)
	DWORD	Size;					// velikost obrázku v bajtech
	DWORD	Offset;					// offset obrázku v souboru
} ICONDIRENTRY;

#define SIZEOFICONDIRENTRY (4*sizeof(BYTE) + 2*sizeof(WORD) + 2*sizeof(DWORD)) // 16 bajtù

typedef struct ICONDIR_
{
	WORD			Reserved;		// ... rezervováno, musí být vždy 0
	WORD			Type;			// typ souboru (ikona = 1, kurzor = 2)
	WORD			Count;			// poèet položek v adresáøi
	ICONDIRENTRY	Data[1];		// položky ikon
} ICONDIR;

#define SIZEOFICONDIR (3*sizeof(WORD))	// 6 bajtù

// Obrázek ikony:
//	BITMAPINFOHEADER	- záhlaví BMP
//			biSize až biBitCount a biSizeImage = platné položky, ostatní = 0
//	RGBQUAD				- palety
//	BYTE Xor[]			- data barev
//	BYTE And[]			- maska obrázku

#pragma pack( pop )


////////////////////////////////////////////////////////////////////
// test platnosti záhlaví BMP (TRUE=je OK)

BOOL TestBMP(const BITMAPINFO* bitmap);


////////////////////////////////////////////////////////////////////
// dekomprese dat bitmapy BI_RLE8

void DekompRLE8(BYTE* dst, int dstN, BYTE* src, int srcN);


////////////////////////////////////////////////////////////////////
// dekomprese dat bitmapy BI_RLE4

void DekompRLE4(BYTE* dst, int dstN, BYTE* src, int srcN);


////////////////////////////////////////////////////////////////////
// komprese dat bitmapy BI_RLE8, vrací velikost dat

int KompRLE8(BYTE* dstbuf, BYTE* srcbuf, int width, int height);
