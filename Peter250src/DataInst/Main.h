
//////////////////////////////////////////////////////////////////////////////
// p�ep�na�e preprocesoru:
//	_DEBUG ............. je debugger
//	_OPTIM ............. je optimalizace
//	_MT ................ v�cetokov� re�im
//	_UNICODE ........... k�dov�n� znak� UNICODE
//
//	_M_IX86 ............ procesor Intel 86
//	_M_ALPHA ........... procesor DEC Alpha
//	_M_MPPC ............ procesor Power Macintosh
//	_M_MRX000 .......... procesor MIPS
//	_M_PPC ............. procesor PowerPC

//#define _MT

// Pro p�eklada� nastavit �rove� varov�n� 4 (p�ep�na� /W4)
// pro inline funkce nepou��vat "bool" ale rad�ji "BOOL" - l�pe optimalizov�no

//////////////////////////////////////////////////////////////////////////////
// obsluha debuggeru

#ifdef _DEBUG
#ifdef _M_IX86
#define debugbreak _asm{int 3}
#else	// _M_IX86
#define debugbreak DebugBreak()
#endif	// _M_IX86
#endif	// _DEBUG

#if defined(_DEBUG)
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
#endif	// UNICODE
#endif	// _UNICODE

//////////////////////////////////////////////////////////////////////////////
// standardn� vno�en� sekce

#pragma warning ( disable: 4201)		// hl�en� - nepojmenovan� struktura

#include <windows.h>					// z�kladn� definice WINDOWS
#include <math.h>						// matematick� operace
//#include <alphaops.h>					// matematick� konstanty
#include <tchar.h>						// obsluha znak� UNICODE/MB
#include <commctrl.h>					// dopl�kov� ovl�dac� prvky
#include <richedit.h>					// RichEdit
#include <shlobj.h>
#include <objbase.h>
#include "resource.h"

#define DOUBLE_INFINITY_VALUE       ((ULONGLONG)0x7ff0000000000000)

#pragma warning ( default: 4201)		// hl�en� - nepojmenovan� struktura

#pragma warning ( disable: 4100)		// hl�en� - nepou�it� form�ln� parametr
#pragma warning ( disable: 4710)		// hl�en� - funkce nen� inline

//////////////////////////////////////////////////////////////////////////////
// p�eddefinice t��d

class CText;


//////////////////////////////////////////////////////////////////////////////
// glob�ln� prom�nn�

extern	int			VerzeOS;			// verze syst�mu
extern	HINSTANCE	hInstance;			// instance programu
extern	int			ScreenWidth;		// ���ka klientsk� oblasti displeje
extern	int			ScreenHeight;		// v��ka klientsk� oblasti displeje

extern	HWND		MainFrame;			// hlavn� okno aplikace

extern	int			Waiting;			// p��znak zobrazen� kurzoru �ek�n�

#ifdef _UNICODE
typedef	BOOL (WINAPI *GETDISKFREESPACEEX) (LPCWSTR, __int64*, __int64*, __int64*);
#else
typedef	BOOL (WINAPI *GETDISKFREESPACEEX) (LPCSTR, __int64*, __int64*, __int64*);
#endif
extern GETDISKFREESPACEEX	pGetDiskFreeSpaceEx; // funkce GetDiskFreeSpaceEx (NULL=nen�)
extern	__int64	DiskSize;			// velikost disku (z funkce GetDiskSpace)
extern	__int64	DiskFree;			// voln� m�sto disku (z funkce GetDiskSpace)
extern	__int64	DiskFreeUser;		// voln� m�sto u�ivatele (z funkce GetDiskSpace)

//////////////////////////////////////////////////////////////////////////////
// struktura instala�n�ch dat

#define GROUPSNUM 2							// po�et skupin

// definice jednoho souboru v seznamu (9 B + text)
typedef struct INSTFILE_ {
	long			Size;					// (4) velikost souboru v bajtech (po dekompresi)
	long			Check;					// (4) kontroln� sou�et dat souboru (v�choz� 0, p�i�ten bajt, rotace vlevo s p�enosem)
	BYTE			NameN;					// (1) d�lka jm�na souboru v�etn� podcesty - ve znac�ch
	char			Name[1];				// (n) jm�no souboru (v�etn� podcesty)
} INSTFILE;

// definice jedn� skupiny (16 B)
typedef struct INSTGROUP_ {
	long			Files;					// (4) po�et soubor� ve skupin�
	long			Size;					// (4) velikost skupiny v KB (po nainstalov�n�) - soubory zaokrouhleny na aloka�n� bloky 8 KB
	long			SizeFiles;				// (4) velikost seznamu soubor� (bajt�)
	long			SizeGroup;				// (4) velikost komprimovan�ch dat (bajt�)
} INSTGROUP;

// z�hlav� instala�n�ch dat (16 B + skupiny)
typedef struct INSTHEAD_ {
	char			Ident[4];				// (4) identifikace (text "SET" + b�n�rn� po�et sekc�)
	long			Check;					// (4) kontroln� sou�et zbytku z�hlav� v�etn� seznamu soubor�
	FILETIME		DateTime;				// (8) datum a �as soubor�
	INSTGROUP		Groups[GROUPSNUM];		// definice skupin
} INSTHEAD;

//////////////////////////////////////////////////////////////////////////////
// pomocn� konstanty

#define ICONWIDTH 32								// ���ka ikon
#define ICONHEIGHT 32								// v��ka ikon
#define ICONSIZE (ICONWIDTH*ICONHEIGHT)				// velikost ikon v bajtech

//////////////////////////////////////////////////////////////////////////////
// str�nky instal�toru

extern	int			AktPage;		// aktivn� str�nka instal�toru

enum PAGES {
	PAGESELECT,						// str�nka v�b�ru prvk� k instalaci
	PAGEINSTAL,						// str�nka prob�haj�c� instalace
	PAGEOK,							// instalace ukon�ena OK
};

//////////////////////////////////////////////////////////////////////////////
// ukon�en� aplikace

void	Exit();						// ukon�en� programu

/////////////////////////////////////////////////////////////////////////////
// obsluha zpr�vy p�ed rozesl�n�m do oken (TRUE = zpr�va zpracov�na)

BOOL PreTranslateMessage(MSG* msg);

/////////////////////////////////////////////////////////////////////////////
// nastaven� str�nky instal�toru

void SetPage(int page);

/////////////////////////////////////////////////////////////////////////////
// na�ten� registru

CText GetReg(CText key, CText name);

/////////////////////////////////////////////////////////////////////////////
// nastaven� registru

void SetReg(CText key, CText name, CText data);

/////////////////////////////////////////////////////////////////////////////
// zru�en� kl��e

void DelReg(CText key, CText name);

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� hlavn�ho okna aplikace

bool MainFrameCreate();

/////////////////////////////////////////////////////////////////////////////
// zobrazen� kurzoru �ek�n� (zah�jen� a ukon�en� mus� b�t do p�ru!)

void BeginWaitCursor();


/////////////////////////////////////////////////////////////////////////////
// na�ten� informac� o souborech (vrac� FALSE=p�eru�it)

BOOL OpenSetup();

/////////////////////////////////////////////////////////////////////////////
// vypnut� kurzoru �ek�n� (zah�jen� a ukon�en� mus� b�t do p�ru!)

void EndWaitCursor();

//////////////////////////////////////////////////////////////////////////////
// vlastn� vno�en� sekce

#include "Memory.h"						// obsluha pam�ti
#include "BufText.h"					// buffer text�, textov� �et�zce
#include "File.h"						// soubory, buffery a resource
#include "Compress.h"					// komprese
