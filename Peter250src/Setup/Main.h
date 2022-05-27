
//////////////////////////////////////////////////////////////////////////////
// pøepínaèe preprocesoru:
//	_DEBUG ............. je debugger
//	_OPTIM ............. je optimalizace
//	_MT ................ vícetokový režim
//	_UNICODE ........... kódování znakù UNICODE
//
//	_M_IX86 ............ procesor Intel 86
//	_M_ALPHA ........... procesor DEC Alpha
//	_M_MPPC ............ procesor Power Macintosh
//	_M_MRX000 .......... procesor MIPS
//	_M_PPC ............. procesor PowerPC

//#define _MT

// Pro pøekladaè nastavit úroveò varování 4 (pøepínaè /W4)
// pro inline funkce nepoužívat "bool" ale radìji "BOOL" - lépe optimalizováno


#define VerzeMaj		2			// verze - hlavní èíslice (jednotky)
#define VerzeMin		5			// verze - vedlejší èíslice (desetiny)
#define VerzeRel		0			// verze - èíslice vydání (setiny)
#define VerzeCom		0			// verze - èíslice kompilace (tisíciny)
#define VerzeFileTxt	_T("250")	// verze ve jménu souboru
#define VerzeTxt		_T("2.50")	// verze - text


#define COMPACT			// kompaktní instalaèní balík

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
// standardní vnoøené sekce

#pragma warning ( disable: 4201)		// hlášení - nepojmenovaná struktura

#include <windows.h>					// základní definice WINDOWS
#include <math.h>						// matematické operace
//#include <alphaops.h>					// matematické konstanty
#include <tchar.h>						// obsluha znakù UNICODE/MB
#include <commctrl.h>					// doplòkové ovládací prvky
#include <richedit.h>					// RichEdit
#include <shlobj.h>
#include <objbase.h>
#include "resource.h"

#define DOUBLE_INFINITY_VALUE       ((ULONGLONG)0x7ff0000000000000)

#pragma warning ( default: 4201)		// hlášení - nepojmenovaná struktura

#pragma warning ( disable: 4100)		// hlášení - nepoužitý formální parametr
#pragma warning ( disable: 4710)		// hlášení - funkce není inline

//////////////////////////////////////////////////////////////////////////////
// pøeddefinice tøíd

class CText; class CPicture;


//////////////////////////////////////////////////////////////////////////////
// globální promìnné

extern	TCHAR*		CommandLine;		// pøíkazový øádek
extern	int			VerzeOS;			// verze systému
extern	HINSTANCE	hInstance;			// instance programu
extern	int			ScreenWidth;		// šíøka klientské oblasti displeje
extern	int			ScreenHeight;		// výška klientské oblasti displeje

extern	BOOL		Dither;
extern	HWND		MainFrame;			// hlavní okno aplikace

extern	int			Waiting;			// pøíznak zobrazení kurzoru èekání

#ifdef _UNICODE
typedef	BOOL (WINAPI *GETDISKFREESPACEEX) (LPCWSTR, __int64*, __int64*, __int64*);
#else
typedef	BOOL (WINAPI *GETDISKFREESPACEEX) (LPCSTR, __int64*, __int64*, __int64*);
#endif
extern GETDISKFREESPACEEX	pGetDiskFreeSpaceEx; // funkce GetDiskFreeSpaceEx (NULL=není)
extern	__int64	DiskSize;			// velikost disku (z funkce GetDiskSpace)
extern	__int64	DiskFree;			// volné místo disku (z funkce GetDiskSpace)
extern	__int64	DiskFreeUser;		// volné místo uživatele (z funkce GetDiskSpace)

//////////////////////////////////////////////////////////////////////////////
// pomocné konstanty

#define ICONWIDTH 32								// šíøka ikon
#define ICONHEIGHT 32								// výška ikon
#define ICONSIZE (ICONWIDTH*ICONHEIGHT)				// velikost ikon v bajtech

#define		BackCol 231							// barva pozadí (prùhledná)
#define		ShadCol 216							// barva stínu
#define		DarkCol 219							// tmavá barva blokování voleb
#define		LightCol 214						// svìtlá barva
#define		WhiteCol 212						// bílá barva

#define		GemtreeN 90							// poèet obrázkù znaku Gemtree
//#define		GemtreeX1 110						// souøadnice znaku X pro volbu Instal
#define		GemtreeX2 118						// souøadnice znaku X pro ostatní volby
#define		GemtreeY 190						// souøadnice znaku Y

#define MOUSEINV -100000							// neplatná souøadnice myši

#define MENUTOP 176									// poèáteèní souøadnice Y voleb menu
#define MENUHEIGHT 50								// výška jedné volby menu

#define KonvCopy MemCopy

//////////////////////////////////////////////////////////////////////////////
// struktura instalaèních dat

#ifdef MINI
#define GROUPSNUM 3							// poèet skupin
#else
#define GROUPSNUM 9							// poèet skupin
#endif

// definice jednoho souboru v seznamu (9 B + text)
typedef struct INSTFILE_ {
	long			Size;					// (4) velikost souboru v bajtech (po dekompresi)
	long			Check;					// (4) kontrolní souèet dat souboru (výchozí 0, pøièten bajt, rotace vlevo s pøenosem)
	BYTE			NameN;					// (1) délka jména souboru vèetnì podcesty - ve znacích
	char			Name[1];				// (n) jméno souboru (vèetnì podcesty)
} INSTFILE;

// definice jedné skupiny (16 B)
typedef struct INSTGROUP_ {
	long			Files;					// (4) poèet souborù ve skupinì
	long			Size;					// (4) velikost skupiny v KB (po nainstalování) - soubory zaokrouhleny na alokaèní bloky 8 KB
	long			SizeFiles;				// (4) velikost seznamu souborù (bajtù)
	long			SizeGroup;				// (4) velikost komprimovaných dat (bajtù)
} INSTGROUP;

// záhlaví instalaèních dat (16 B + skupiny)
typedef struct INSTHEAD_ {
	char			Ident[4];				// (4) identifikace (text "SET" + bínárnì poèet sekcí)
	long			Check;					// (4) kontrolní souèet zbytku záhlaví vèetnì seznamu souborù
	FILETIME		DateTime;				// (8) lokální (!) datum a èas souborù
	INSTGROUP		Groups[GROUPSNUM];		// definice skupin
} INSTHEAD;


//////////////////////////////////////////////////////////////////////////////
// pøíznaky typu ikony a obrázku
enum PICPARAM {
	PicParamPic,			// pouze obrázek bez pozadí
	PicParamMix,			// obrázek mixovaný s pozadím
	PicParamBack,			// pouze pozadí (prázdný obrázek)
	PicParamNone,			// neznámý obsah
	PicParamComp,			// komprimovaná data
};

//////////////////////////////////////////////////////////////////////////////
// stránky instalátoru

extern	int			AktPage;		// aktivní stránka instalátoru

enum PAGES {
	PAGELANG,						// stránka volby jazyku
	PAGEMAIN,						// hlavní stránka instalátoru
	PAGESELECT,						// stránka výbìru prvkù k instalaci
	PAGEINSTAL,						// stránka probíhající instalace
	PAGEDINSTAL,					// stránka probíhající odinstalace
	PAGEOK,							// instalace ukonèena OK
	PAGEDINSTOK,					// odinstalace ukonèena OK
	PAGEISDINST,					// potvrzení odinstalování
};

//////////////////////////////////////////////////////////////////////////////
// ukonèení aplikace

void	Exit();						// ukonèení programu

/////////////////////////////////////////////////////////////////////////////
// obsluha zprávy pøed rozesláním do oken (TRUE = zpráva zpracována)

BOOL PreTranslateMessage(MSG* msg);

//////////////////////////////////////////////////////////////////////////////
// spuštìní programu

int Exec(CText command, CText aktdir, BOOL wait);


/////////////////////////////////////////////////////////////////////////////
// nastavení stránky instalátoru

void SetPage(int page);

/////////////////////////////////////////////////////////////////////////////
// pøekreslení okna

void RePaint();
void RePaintOK();
void RePaintDInstOK();
void RePaintDInst();
void RePaintGemtree();

/////////////////////////////////////////////////////////////////////////////
// naètení registru

CText GetReg(CText key, CText name);

/////////////////////////////////////////////////////////////////////////////
// nastavení registru

void SetReg(CText key, CText name, CText data);

/////////////////////////////////////////////////////////////////////////////
// zrušení klíèe

void DelReg(CText key, CText name);

/////////////////////////////////////////////////////////////////////////////
// vytvoøení hlavního okna aplikace

bool MainFrameCreate();

/////////////////////////////////////////////////////////////////////////////
// zobrazení kurzoru èekání (zahájení a ukonèení musí být do páru!)

void BeginWaitCursor();

/////////////////////////////////////////////////////////////////////////////
// naètení informací o souborech (vrací FALSE=pøerušit)

BOOL OpenSetup();

/////////////////////////////////////////////////////////////////////////////
// vypnutí kurzoru èekání (zahájení a ukonèení musí být do páru!)

void EndWaitCursor();

//////////////////////////////////////////////////////////////////////////////
// vlastní vnoøené sekce

#include "Memory.h"						// obsluha pamìti
#include "BufText.h"					// buffer textù, textové øetìzce
#include "File.h"						// soubory, buffery a resource
#include "Bitmap.h"						// obrázky
#include "BufPic.h"						// buffer obrázkù
#include "Compress.h"					// komprese

