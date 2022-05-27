
//////////////////////////////////////////////////////////////////////////////
// pøepínaèe preprocesoru:
//	_DEBUG ............. je debugger
//	_OPTIM ............. je optimalizace
//	_MT ................ vícevláknový režim
//	_UNICODE ........... kódování znakù UNICODE
//
//	_M_IX86 ............ procesor Intel 86
//	_M_ALPHA ........... procesor DEC Alpha
//	_M_MPPC ............ procesor Power Macintosh
//	_M_MRX000 .......... procesor MIPS
//	_M_PPC ............. procesor PowerPC


// Pro pøekladaè nastavit úroveò varování 4 (pøepínaè /W4)

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
// standardní vnoøené sekce

#pragma warning ( disable: 4201)		// hlášení - nepojmenovaná struktura

#include <windows.h>					// základní definice WINDOWS
#include <math.h>						// matematické operace
//#include <alphaops.h>					// matematické konstanty
#include <tchar.h>						// obsluha znakù UNICODE/MB
#include <commctrl.h>					// doplòkové ovládací prvky
//#include <mmreg.h>						// multimediální definice
#include "resource.h"

#define DOUBLE_INFINITY_VALUE       ((ULONGLONG)0x7ff0000000000000)

#pragma warning ( default: 4201)		// hlášení - nepojmenovaná struktura

#pragma warning ( disable: 4100)		// hlášení - nepoužitý formální parametr
#pragma warning ( disable: 4710)		// hlášení - funkce není inline

//////////////////////////////////////////////////////////////////////////////
// pøeddefinice tøíd

class CPicture; class CSprite;
class CString;
class CBufPic; class CBufSprite; class CBufText;
class CFileMap;

//////////////////////////////////////////////////////////////////////////////
// globální promìnné

extern	CString		CommandLine;		// pøíkazový øádek
extern	int			VerzeOS;			// verze systému
extern	HINSTANCE	hInstance;			// instance programu

extern	BYTE*		StdPalImport;		// tabulka importu palet
extern	BYTE*		StdPalImportDither;	// tabulka importu palet s ditheringem
extern	BITMAPINFO* StdBitmapInfo;		// standardní záhlaví BMP
extern	HPALETTE	StdPalette;			// vlastní logické palety
extern	BYTE*		KonvPal;			// konverzní tabulka palet

extern	bool	Dither;					// použít dithering

//////////////////////////////////////////////////////////////////////////////
// prázdné objekty (napø. pro návrat neplatné položky z funkce)

extern	CString		EmptyString;		// prázdný øetìzec
extern	CPicture	EmptyPicture;		// prázdný obrázek
extern	CSprite		EmptySprite;		// prázdný sprajt

//////////////////////////////////////////////////////////////////////////////
// globální konstanty

//////////////////////////////////////////////////////////////////////////////
// globální konstanty

#define BackCol 0					// prùhledná barva (barva pozadí)
#define ShadCol 1					// poloprùhledná barva (stín)

#define ResCols 2					// poèet rezervovaných barev na poèátku barev (=pozadí a stín)

#define BACKCOLOR_RED	149			// èervená složka barvy pozadí
#define BACKCOLOR_GREEN	34			// zelená složka barvy pozadí
#define BACKCOLOR_BLUE	140			// modrá složka barvy pozadí

#define SHADCOLOR_RED	97			// èervená složka barvy stínu
#define SHADCOLOR_GREEN	30			// zelená složka barvy stínu
#define SHADCOLOR_BLUE	111			// modrá složka barvy stínu

extern const int ColLev;			// poèet úrovní standardních palet
extern const int ColCol;			// poèet barev standardních palet
extern const int StdColors;			// poèet vlastních palet (zaèínají od 0)
//extern const BYTE BackCol;			// prùhledná barva

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
		((color & (63*4))/4) |							// èervená složka
		((color & (63*4*256))/(256*4/64)) |				// zelená složka
		((color & (63*4*256*256))/(256*256*4/64/64))];	// modrá složka
}

// import palety s ditheringem
inline BYTE PalImportDither(BYTE red, BYTE green, BYTE blue, int x, int y)
{
	return StdPalImportDither[(red & ~3) | ((green & ~3) * 64) | ((blue & ~3) * 64 * 64) | ((x & 1) + (y & 1)*2)];
}

inline BYTE PalImportDither(int color, int x, int y)
{
	return StdPalImportDither[
		(color & (63*4)) |								// èervená složka
		((color & (63*4*256))/(256/64)) |				// zelená složka
		((color & (63*4*256*256))/(256*256/64/64)) |	// modrá složka
		((x & 1) + (y & 1)*2)];
}

//////////////////////////////////////////////////////////////////////////////
// ukonèení aplikace

void	Exit(int code);					// ukonèení programu

#define	EXITCODE_MEMINIT	120			// chyba pamìti pøi inicializaci
#define EXITCODE_MAINFRAME	115			// chyba vytvoøení hlavního okna
#define EXITCODE_LOADRES	110			// chyba ètení resource

#define EXITCODE_INIT		100			// hranice inicializaèních kódù

#define	EXITCODE_MEMERR		80			// chyba pamìti pøi bìhu programu
#define EXITCODE_OK			0			// øádné ukonèení programu


/////////////////////////////////////////////////////////////////////////////
// generování konverzní tabulky barev obrázku

void GenKonvPal(BITMAPINFO* bmp);


/////////////////////////////////////////////////////////////////////////////
// vygenerování støední barvy (ze 4 bodù)

BYTE _fastcall ColAvrg(BYTE col1, BYTE col2, BYTE col3, BYTE col4);
BYTE ColAvrgDither(BYTE col1, BYTE col2, BYTE col3, BYTE col4, int x, int y);


/////////////////////////////////////////////////////////////////////////////
// dekomprese dat

void DeKomp(BYTE* dstBuf, int dstNum, BYTE* srcBuf, int srcNum);


/////////////////////////////////////////////////////////////////////////////
// zaokrouhlení èísel

// pøevod reálného èísla na celé èíslo se zaokrouhlením k nejbližší hodnotì
int Round(double num);

// pøevod reálného èísla na celé èíslo s oøíznutím k nule
int Round0(double num);

// pøevod reálného èísla na celé èíslo se zaokrouhlením nahoru (ke kladné hodnotì)
int RoundP(double num);

// pøevod reálného èísla na celé èíslo se zaokrouhlením dolù (k záporné hodnotì)
int RoundM(double num);


//////////////////////////////////////////////////////////////////////////////
// pomocné konstanty

#define ICONWIDTH 32								// šíøka ikon
#define ICONHEIGHT 32								// výška ikon
#define ICONSIZE (ICONWIDTH*ICONHEIGHT)				// velikost ikon v bajtech

//////////////////////////////////////////////////////////////////////////////
// vlastní vnoøené sekce

#include "Memory.h"						// obsluha pamìti
#include "Bitmap.h"						// obsluha bitmap a ikon
#include "BufPic.h"						// buffer obrázkù (pozadí)
#include "BufSprt.h"					// buffer sprajtù
#include "BufText.h"					// buffer textù, textové øetìzce
#include "File.h"						// obsluha souborù
#include "Compress.h"					// obsluha komprese dat
