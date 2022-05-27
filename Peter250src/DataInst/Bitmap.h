
/***************************************************************************\
*																			*
*							Obsluha ikon a bitmap							*
*																			*
\***************************************************************************/


////////////////////////////////////////////////////////////////////
// definice z�hlav� souboru ICO

#pragma pack( push )
#pragma pack( 2 )

typedef struct ICONDIRENTRY_
{
	BYTE	Width;					// ���ka
	BYTE	Height;					// v��ka
	BYTE	Colors;					// po�et barev (0 = 256 barev)
	BYTE	Reserved;				// ... rezervov�no, mus� b�t v�dy 0
	WORD	Planes;					// po�et barevn�ch rovin (nastaveno na 0)
	WORD	Bits;					// po�et bit� na bod (nastaveno na 0)
	DWORD	Size;					// velikost obr�zku v bajtech
	DWORD	Offset;					// offset obr�zku v souboru
} ICONDIRENTRY;

#define SIZEOFICONDIRENTRY (4*sizeof(BYTE) + 2*sizeof(WORD) + 2*sizeof(DWORD)) // 16 bajt�

typedef struct ICONDIR_
{
	WORD			Reserved;		// ... rezervov�no, mus� b�t v�dy 0
	WORD			Type;			// typ souboru (ikona = 1, kurzor = 2)
	WORD			Count;			// po�et polo�ek v adres��i
	ICONDIRENTRY	Data[1];		// polo�ky ikon
} ICONDIR;

#define SIZEOFICONDIR (3*sizeof(WORD))	// 6 bajt�

// Obr�zek ikony:
//	BITMAPINFOHEADER	- z�hlav� BMP
//			biSize a� biBitCount a biSizeImage = platn� polo�ky, ostatn� = 0
//	RGBQUAD				- palety
//	BYTE Xor[]			- data barev
//	BYTE And[]			- maska obr�zku

#pragma pack( pop )


////////////////////////////////////////////////////////////////////
// test platnosti z�hlav� BMP (TRUE=je OK)

BOOL TestBMP(const BITMAPINFO* bitmap);


////////////////////////////////////////////////////////////////////
// dekomprese dat bitmapy BI_RLE8

void DekompRLE8(BYTE* dst, int dstN, BYTE* src, int srcN);


////////////////////////////////////////////////////////////////////
// dekomprese dat bitmapy BI_RLE4

void DekompRLE4(BYTE* dst, int dstN, BYTE* src, int srcN);


////////////////////////////////////////////////////////////////////
// komprese dat bitmapy BI_RLE8, vrac� velikost dat

int KompRLE8(BYTE* dstbuf, BYTE* srcbuf, int width, int height);
