
//////////////////////////////////////////////////////////////////////////////
// p�ep�na�e preprocesoru:
//	_DEBUG ............. je debugger
//	_OPTIM ............. je optimalizace
//	_MT ................ v�cevl�knov� re�im
//	_UNICODE ........... k�dov�n� znak� UNICODE
//
//	_M_IX86 ............ procesor Intel 86
//	_M_ALPHA ........... procesor DEC Alpha
//	_M_MPPC ............ procesor Power Macintosh
//	_M_MRX000 .......... procesor MIPS
//	_M_PPC ............. procesor PowerPC


// Pro p�eklada� nastavit �rove� varov�n� 4 (p�ep�na� /W4)

//////////////////////////////////////////////////////////////////////////////
// obsluha debuggeru

#ifdef _DEBUG
#ifdef _M_IX86
#define debugbreak _asm{int 3}
#else	// _M_IX86
#define debugbreak DebugBreak()
#endif	// _M_IX86
#endif	// _DEBUG

#if defined(_DEBUG) && !defined(_OPTIM)
#define ASSERT(f)			{ if (!(f)) debugbreak; }
#define VERIFY(f)			ASSERT(f)
#define ASSERTERROR			debugbreak
#else	// _DEBUG
#define ASSERT(f)			((void)0)
#define VERIFY(f)			((void)(f))
#define ASSERTERROR			((void)0)
#endif	// _DEBUG

#ifdef _UNICODE
#ifndef UNICODE
#define UNICODE
#endif
#endif

//////////////////////////////////////////////////////////////////////////////
// standardn� vno�en� sekce

#pragma warning ( disable: 4201)		// hl�en� - nepojmenovan� struktura

#include <windows.h>					// z�kladn� definice WINDOWS
#include <math.h>						// matematick� operace
//#include <alphaops.h>					// matematick� konstanty
#include <tchar.h>						// obsluha znak� UNICODE/MB
#include <commctrl.h>					// dopl�kov� ovl�dac� prvky
//#include <mmreg.h>						// multimedi�ln� definice
#include "resource.h"

#define DOUBLE_INFINITY_VALUE       ((ULONGLONG)0x7ff0000000000000)

#pragma warning ( default: 4201)		// hl�en� - nepojmenovan� struktura

#pragma warning ( disable: 4100)		// hl�en� - nepou�it� form�ln� parametr
#pragma warning ( disable: 4710)		// hl�en� - funkce nen� inline

//////////////////////////////////////////////////////////////////////////////
// p�eddefinice t��d

class CPicture; class CSprite;
class CString;
class CBufPic; class CBufSprite; class CBufText;
class CFileMap;

//////////////////////////////////////////////////////////////////////////////
// glob�ln� prom�nn�

extern	CString		CommandLine;		// p��kazov� ��dek
extern	int			VerzeOS;			// verze syst�mu
extern	HINSTANCE	hInstance;			// instance programu

extern	BYTE*		StdPalImport;		// tabulka importu palet
extern	BYTE*		StdPalImportDither;	// tabulka importu palet s ditheringem
extern	BITMAPINFO* StdBitmapInfo;		// standardn� z�hlav� BMP
extern	HPALETTE	StdPalette;			// vlastn� logick� palety
extern	BYTE*		KonvPal;			// konverzn� tabulka palet

extern	bool	Dither;					// pou��t dithering

//////////////////////////////////////////////////////////////////////////////
// pr�zdn� objekty (nap�. pro n�vrat neplatn� polo�ky z funkce)

extern	CString		EmptyString;		// pr�zdn� �et�zec
extern	CPicture	EmptyPicture;		// pr�zdn� obr�zek
extern	CSprite		EmptySprite;		// pr�zdn� sprajt

//////////////////////////////////////////////////////////////////////////////
// glob�ln� konstanty

//////////////////////////////////////////////////////////////////////////////
// glob�ln� konstanty

#define BackCol 0					// pr�hledn� barva (barva pozad�)
#define ShadCol 1					// polopr�hledn� barva (st�n)

#define ResCols 2					// po�et rezervovan�ch barev na po��tku barev (=pozad� a st�n)

#define BACKCOLOR_RED	149			// �erven� slo�ka barvy pozad�
#define BACKCOLOR_GREEN	34			// zelen� slo�ka barvy pozad�
#define BACKCOLOR_BLUE	140			// modr� slo�ka barvy pozad�

#define SHADCOLOR_RED	97			// �erven� slo�ka barvy st�nu
#define SHADCOLOR_GREEN	30			// zelen� slo�ka barvy st�nu
#define SHADCOLOR_BLUE	111			// modr� slo�ka barvy st�nu

extern const int ColLev;			// po�et �rovn� standardn�ch palet
extern const int ColCol;			// po�et barev standardn�ch palet
extern const int StdColors;			// po�et vlastn�ch palet (za��naj� od 0)
//extern const BYTE BackCol;			// pr�hledn� barva

//////////////////////////////////////////////////////////////////////////////
// import palet

// konverze barev
inline void KonvCopy(BYTE* dst, BYTE* src, int num)
{
	for (; num > 0; num--)
	{
		dst[0] = KonvPal[src[0]];
		dst++;
		src++;
	}
}

// import palety
inline BYTE PalImport(BYTE red, BYTE green, BYTE blue)
{
	return StdPalImport[(red/4) | (green/4 * 64) | (blue/4 * 64 * 64)];
}

inline BYTE PalImport(int color)
{
	return StdPalImport[
		((color & (63*4))/4) |							// �erven� slo�ka
		((color & (63*4*256))/(256*4/64)) |				// zelen� slo�ka
		((color & (63*4*256*256))/(256*256*4/64/64))];	// modr� slo�ka
}

// import palety s ditheringem
inline BYTE PalImportDither(BYTE red, BYTE green, BYTE blue, int x, int y)
{
	return StdPalImportDither[(red & ~3) | ((green & ~3) * 64) | ((blue & ~3) * 64 * 64) | ((x & 1) + (y & 1)*2)];
}

inline BYTE PalImportDither(int color, int x, int y)
{
	return StdPalImportDither[
		(color & (63*4)) |								// �erven� slo�ka
		((color & (63*4*256))/(256/64)) |				// zelen� slo�ka
		((color & (63*4*256*256))/(256*256/64/64)) |	// modr� slo�ka
		((x & 1) + (y & 1)*2)];
}

//////////////////////////////////////////////////////////////////////////////
// ukon�en� aplikace

void	Exit(int code);					// ukon�en� programu

#define	EXITCODE_MEMINIT	120			// chyba pam�ti p�i inicializaci
#define EXITCODE_MAINFRAME	115			// chyba vytvo�en� hlavn�ho okna
#define EXITCODE_LOADRES	110			// chyba �ten� resource

#define EXITCODE_INIT		100			// hranice inicializa�n�ch k�d�

#define	EXITCODE_MEMERR		80			// chyba pam�ti p�i b�hu programu
#define EXITCODE_OK			0			// ��dn� ukon�en� programu


/////////////////////////////////////////////////////////////////////////////
// generov�n� konverzn� tabulky barev obr�zku

void GenKonvPal(BITMAPINFO* bmp);


/////////////////////////////////////////////////////////////////////////////
// vygenerov�n� st�edn� barvy (ze 4 bod�)

BYTE _fastcall ColAvrg(BYTE col1, BYTE col2, BYTE col3, BYTE col4);
BYTE ColAvrgDither(BYTE col1, BYTE col2, BYTE col3, BYTE col4, int x, int y);


/////////////////////////////////////////////////////////////////////////////
// dekomprese dat

void DeKomp(BYTE* dstBuf, int dstNum, BYTE* srcBuf, int srcNum);


/////////////////////////////////////////////////////////////////////////////
// zaokrouhlen� ��sel

// p�evod re�ln�ho ��sla na cel� ��slo se zaokrouhlen�m k nejbli��� hodnot�
int Round(double num);

// p�evod re�ln�ho ��sla na cel� ��slo s o��znut�m k nule
int Round0(double num);

// p�evod re�ln�ho ��sla na cel� ��slo se zaokrouhlen�m nahoru (ke kladn� hodnot�)
int RoundP(double num);

// p�evod re�ln�ho ��sla na cel� ��slo se zaokrouhlen�m dol� (k z�porn� hodnot�)
int RoundM(double num);


//////////////////////////////////////////////////////////////////////////////
// pomocn� konstanty

#define ICONWIDTH 32								// ���ka ikon
#define ICONHEIGHT 32								// v��ka ikon
#define ICONSIZE (ICONWIDTH*ICONHEIGHT)				// velikost ikon v bajtech

//////////////////////////////////////////////////////////////////////////////
// vlastn� vno�en� sekce

#include "Memory.h"						// obsluha pam�ti
#include "Bitmap.h"						// obsluha bitmap a ikon
#include "BufPic.h"						// buffer obr�zk� (pozad�)
#include "BufSprt.h"					// buffer sprajt�
#include "BufText.h"					// buffer text�, textov� �et�zce
#include "File.h"						// obsluha soubor�
#include "Compress.h"					// obsluha komprese dat
