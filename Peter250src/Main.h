
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
#define VerzeTxt		_T("2.50")	// verze - text
#define VerzeTxt0		_T("250")	// verze - text zkrácený (pro clipboard)

//////////////////////////////////////////////////////////////////////////////
// obsluha debuggeru

#ifdef _DEBUG
#ifdef _M_IX86
#ifdef _OPTIM
#define debugbreak debugBreak(__FILE__, __LINE__, __DATE__)
void debugBreak(char* file, int line, char* date);
#else
#define debugbreak _asm{int 3}
#endif	// _OPTIM
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
#include <mmreg.h>						// multimediální definice
#include "resource.h"

#pragma warning ( default: 4201)		// hlášení - nepojmenovaná struktura

#pragma warning ( disable: 4100)		// hlášení - nepoužitý formální parametr
#pragma warning ( disable: 4710)		// hlášení - funkce není inline

#ifndef SPI_GETLISTBOXSMOOTHSCROLLING
#define SPI_GETLISTBOXSMOOTHSCROLLING 0x1006
#define SPI_SETLISTBOXSMOOTHSCROLLING 0x1007
#endif

#define DOUBLE_INFINITY_VALUE       ((ULONGLONG)0x7ff0000000000000)

//////////////////////////////////////////////////////////////////////////////
// pøeddefinice tøíd

class CIcon; class CMap; class CMusic; class CPicture; class CSound; class CSprite;
class CText; class CFont; class CMultiText;
class CBufBool; class CBufIcon; class CBufInt; class CBufMap; class CBufMusic;
class CBufPic; class CBufReal; class CBufSound; class CBufSprite; class CBufText;
class CBufProg; class CBufIndex; class CBufUndo;
class CFileMap; class CBuf;

//////////////////////////////////////////////////////////////////////////////
// globální promìnné

extern	TCHAR*		CommandLine;		// pøíkazový øádek
extern	int			VerzeOS;			// verze systému
extern	HINSTANCE	hInstance;			// instance programu
extern	int			ScreenWidth;		// šíøka klientské oblasti displeje
extern	int			ScreenHeight;		// výška klientské oblasti displeje
extern	int			TimerConst;			// konstanta pro èasovaè (pro 55 ms)

extern	HACCEL		Accel;				// akceleraèní tabulka

extern	BYTE*		StdPalImport;		// tabulka importu palet
extern	BITMAPINFO* StdBitmapInfo;		// standardní záhlaví BMP
extern	HPALETTE	StdPalette;			// vlastní logické palety
extern	BYTE*		KonvPal;			// konverzní tabulka palet
extern	bool		KonvPalOK;			// pøíznak shodné konverzní tabulky (lze provést kopii)

extern	bool		Dither;				// používat dithering

extern	bool		ShortName;			// zkracovat jména souborù

//extern	bool		Profi;				// profesionální verze programu

#ifdef _UNICODE
typedef	BOOL (WINAPI *GETDISKFREESPACEEX) (LPCWSTR, __int64*, __int64*, __int64*);
#else
typedef	BOOL (WINAPI *GETDISKFREESPACEEX) (LPCSTR, __int64*, __int64*, __int64*);
#endif
extern GETDISKFREESPACEEX	pGetDiskFreeSpaceEx; // funkce GetDiskFreeSpaceEx (NULL=není)
extern	__int64	DiskSize;			// velikost disku (z funkce GetDiskSpace)
extern	__int64	DiskFree;			// volné místo disku (z funkce GetDiskSpace)
extern	__int64	DiskFreeUser;		// volné místo uživatele (z funkce GetDiskSpace)

extern	CMultiText	Jmeno;				// jméno editovaného souboru (bez cesty)
extern	CText		JmenoLoad;			// plné jméno souboru pro ètení (s cestou - vzorový adresáø)
extern	CText		JmenoSave;			// plné jméno souboru pro zápis
extern	CText		Cesta;				// cesta k editovanému souboru (s pøíp. "\" na konci)
extern	CText		AktDir;				// aktivní adresáø uživatele (s "\" na konci)
extern	CText		HomeDir;			// cesta do domovského adresáøe s EXE (s "\" na konci)
extern	CText		ExeFileName;		// jméno programu
extern	CText		IniFileName;		// jméno konfiguraèního souboru
//extern	CText		HelpFileName;		// jméno souboru nápovìdy
extern	CText		TextExe;			// text ".exe"
extern	CText		TextExe2;			// text ".EXE"
extern	CText		CDRom;				// cesta k CD-ROM (prázdný = není)

extern	HWND		PrevWindow;			// pøedešlé aktivní okno

extern	CText		MemErrNadpis;		// nadpis okna chybového hlášení nedostatku pamìti
extern	LPCTSTR		MemErrNadpisP;
extern	CText		MemErrText;			// text okna chybového hlášení nedostatku pamìti
extern	LPCTSTR		MemErrTextP;

//////////////////////////////////////////////////////////////////////////////
// cesty do adresáøù programu (všechny jsou zakonèeny s "\")

extern	CText		ProgramPath;		// cesta do adresáøe programù "Program" (*.exe)

extern	CText		BackupPath;			// cesta do adresáøe záloh "Backup"

extern	CText		FunctionPath;		// cesta do adresáøe funkcí "Function" (*.fnc)
extern	CText		NumberPath;			// cesta do adresáøe èísel "Number" (*.num)
extern	CText		TextPath;			// cesta do adresáøe textù "Text" (*.txt)
extern	CText		BoolPath;			// cesta do adresáøe logických hodnot "Bool" (*.log)
extern	CText		IconPath;			// cesta do adresáøe ikon "Icon" (*.ico)
extern	CText		MapPath;			// cesta do adresáøe ploch "Map" (*.map)
extern	CText		PicturePath;		// cesta do adresáøe obrázkù "Picture" (*.bmp)
extern	CText		SpritePath;			// cesta do adresáøe sprajtù "Sprite" (*.spr)
extern	CText		SoundPath;			// cesta do adresáøe zvukù "Sound" (*.wav)
extern	CText		MusicPath;			// cesta do adresáøe hudby "Music" (*.mid)

// alternativní knihovny - implicitnì v podadresáøi programu Petr
extern	CText		ProgramPath2;		// cesta do adresáøe programù "Program" (*.exe)

extern	CText		FunctionPath2;		// cesta do adresáøe funkcí "Function" (*.fnc)
extern	CText		NumberPath2;		// cesta do adresáøe èísel "Number" (*.num)
extern	CText		TextPath2;			// cesta do adresáøe textù "Text" (*.txt)
extern	CText		BoolPath2;			// cesta do adresáøe logických hodnot "Bool" (*.log)
extern	CText		IconPath2;			// cesta do adresáøe ikon "Icon" (*.ico)
extern	CText		MapPath2;			// cesta do adresáøe ploch "Map" (*.map)
extern	CText		PicturePath2;		// cesta do adresáøe obrázkù "Picture" (*.bmp)
extern	CText		SpritePath2;		// cesta do adresáøe sprajtù "Sprite" (*.spr)
extern	CText		SoundPath2;			// cesta do adresáøe zvukù "Sound" (*.wav)
extern	CText		MusicPath2;			// cesta do adresáøe hudby "Music" (*.mid)

// alternativní knihovny - implicitnì v ROOT CD-ROM
extern	CText		ProgramPath3;		// cesta do adresáøe programù "Program" (*.exe)

extern	CText		FunctionPath3;		// cesta do adresáøe funkcí "Function" (*.fnc)
extern	CText		NumberPath3;		// cesta do adresáøe èísel "Number" (*.num)
extern	CText		TextPath3;			// cesta do adresáøe textù "Text" (*.txt)
extern	CText		BoolPath3;			// cesta do adresáøe logických hodnot "Bool" (*.log)
extern	CText		IconPath3;			// cesta do adresáøe ikon "Icon" (*.ico)
extern	CText		MapPath3;			// cesta do adresáøe ploch "Map" (*.map)
extern	CText		PicturePath3;		// cesta do adresáøe obrázkù "Picture" (*.bmp)
extern	CText		SpritePath3;		// cesta do adresáøe sprajtù "Sprite" (*.spr)
extern	CText		SoundPath3;			// cesta do adresáøe zvukù "Sound" (*.wav)
extern	CText		MusicPath3;			// cesta do adresáøe hudby "Music" (*.mid)

//////////////////////////////////////////////////////////////////////////////
// jazyk

// textové konstanty
typedef struct TEXTCONST_
{
	short		TextID;					// identifikaèní kód textu
	const char*	Text;					// text
} TEXTCONST;

extern const int TextConstNum;			// poèet textových konstant

#define	JAZYKAUT -1						// automatický jazyk

enum JAZYKID {							// (identifikátor jazyku - hledej LANGID)
		JAZYK000,						// neutrální jazyk (0) - obsahuje jména souborù
		JAZYKARA,						// arabsky (1)
		JAZYKBUL,						// bulharsky (2)
		JAZYKCZ,						// èesky (5)
		JAZYKDAN,						// dánsky (6)
		JAZYKGER,						// nìmecky (7)
		JAZYKREC,						// øecky (8)
		JAZYKENG,						// anglicky (9)
		JAZYKSPA,						// španìlsky (10)
		JAZYKFIN,						// finsky (11)
		JAZYKFRA,						// francouzsky (12)
		JAZYKHEB,						// hebrejsky (13)
		JAZYKMAD,						// maïarsky (14)
		JAZYKISL,						// islandsky (15)
		JAZYKITA,						// italsky (16)
		JAZYKHOL,						// holandsky (19)
		JAZYKNOR,						// norsky (20)
		JAZYKPOL,						// polsky (21)
		JAZYKPOR,						// portugalsky (22)
		JAZYKRUM,						// rumunsky (24)
		JAZYKRUS,						// rusky (25)
		JAZYKSRB,						// srbochorvatsky (26)
		JAZYKSLO,						// slovensky (27)
		JAZYKALB,						// albánsky (28)
		JAZYKSWE,						// šwédsky (29)
		JAZYKTUR,						// turecky (31)    = 0x1f
		JAZYKVIE,						// vietnamsky (42) = 0x2a

		JAZYKNUM						// poèet jazykù
};

extern int	JazykUser;					// uživatelem zvolený jazyk
extern int	Jazyk;						// skuteènì nastavený jazyk - musí mít platný rozsah!
extern int	JazykDef;					// implicitní jazyk (podle systému)
extern int	JazykPrev;					// pøedcházející jazyk

extern	DWORD	CharSet;				// aktuální znaková sada fontù (1=implicitní)
extern	DWORD	CodePage;				// aktuální kódová stránka (0=implicitní)
extern	DWORD	LangID;					// identifikátor jazyku LANGID (0=jiný)
extern	BOOL	MultiJazyk;				// multijazyènost
extern	char	Carka;					// oddìlovaè desetinné èárky pøi zobrazení èísel

// jazykové informace
typedef struct JAZYKINFO_
{
	const TEXTCONST*	TextConst;		// tabulka textù
	int					LangID;			// identifikátor jazyku
	LPCTSTR				LangIDini;		// identifikátor jazyku pro ALIASES.INI
	int					CodePage;		// kódová stránka znakù
	int					CharSet;		// znaková sada fontù
	int					Button;			// kód tlaèítka
	LPCTSTR				Name;			// jméno jazyku pro INI
	int					MenuID;			// identifikaèní kód pro menu
	BOOL				InstOK;			// jazyk je instalován
	LPCTSTR				HelpName;		// jméno souboru nápovìdy (2 písmena: CZ, EN, ..)
	BOOL				RightToLeft;		// Right-To-Left, píše se zprava doleva
} JAZYKINFO;

extern const JAZYKINFO JazykInfo[JAZYKNUM]; // tabulka definic jazykù

//////////////////////////////////////////////////////////////////////////////
// nastavení identifikátoru jazyku do registrù Windows (0=implicitní)

void SetJazykReg(int langID);

//////////////////////////////////////////////////////////////////////////////
// zmìna jazyku

void SetJazyk(int jazyk);

//////////////////////////////////////////////////////////////////////////////
// standardní fonty (pro tažení ikon ve stromu)

extern CBufIcon StdFonts;			// buffer standardních fontù (indexy 0 až 255)
extern CBufIndex StdFontsWidth;		// šíøky znakù standardních fontù (indexy 0 až 255)


//////////////////////////////////////////////////////////////////////////////
// zásobník objektù

extern	HBRUSH		HalfToneBrush;		// pùltónový štìtec

//////////////////////////////////////////////////////////////////////////////
// konfigurace

extern int Scale;					// mìøítko pro relativní rozmìry okna

extern int MainLeft;				// relativní pozice hlavního okna vlevo
extern int MainTop;					// relativní pozice hlavního okna nahoøe
extern int MainWidth;				// relativní šíøka hlavního okna
extern int MainHeight;				// relativní výška hlavního okna

extern int LeftWidth;				// relativní šíøka levého pole (objekty)
extern int LeftHeight;				// relativní výška levého horního okna (globální objekty)
extern int RightWidth;				// relativní šíøka pravého pole (tøídy)
extern int RightHeight;				// relativní výška pravého horního okna (struktury)

extern BOOL Maximized;				// pøíznak maximalizace okna

extern CText FindString;			// hledaný text

// velikost ikony
typedef enum _SMALLICON {
	SI_SMALL = 0,					// malá, 16x16
	SI_BIG = 1,						// velká, 32x32
	SI_MEDIUM = 2,					// støední, 24x24
} SMALLICON;

extern SMALLICON BufZoom[];			// pøíznaky zoom oken (TRUE=zvìtšeno)

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
extern const BYTE WhiteCol;			// bílá barva
extern const BYTE BlackCol;			// èerná barva
extern const BYTE BlueCol;			// modrá barva
extern const BYTE RedCol;			// èervená barva
extern const BYTE YellowCol;		// žlutá barva
extern const BYTE GreenCol;			// zelená barva
extern const BYTE LtYellowCol;		// svìtle žlutá barva
extern const BYTE OrangeCol;		// oranžová

extern const long double pi;		// Ludolfovo èíslo
extern const long double pi2;		// Ludolfovo èíslo * 2
extern const long double eul;		// Eulerovo èíslo

extern const long double uhel22;	// úhel 22.5 stupòù v radiánech
extern const long double uhel30;	// úhel 30 stupòù v radiánech
extern const long double uhel45;	// úhel 45 stupòù v radiánech
extern const long double uhel60;	// úhel 60 stupòù v radiánech
extern const long double uhel67;	// úhel 67.5 stupòù v radiánech
extern const long double uhel90;	// úhel 90 stupòù v radiánech
extern const long double uhel135;	// úhel 135 stupòù v radiánech
extern const long double uhel180;	// úhel 180 stupòù v radiánech
extern const long double uhel225;	// úhel 215 stupòù v radiánech
extern const long double uhel270;	// úhel 270 stupòù v radiánech
extern const long double uhel315;	// úhel 315 stupòù v radiánech

extern const long double ln10;		// pøirozený logaritmus 10

extern const long double degrad;	// konstanta pro pøevod z DEG na RAD (pi/180)
extern const long double raddeg;	// konstanta pro pøevod z RAD na DEG (180/pi)


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
// ukonèení aplikace

void	Exit(int code);					// ukonèení programu

#define	EXITCODE_MEMERRINI	125			// chyba pamìti pøi startu programu
#define	EXITCODE_MEMINIT	120			// chyba pamìti pøi inicializaci
#define EXITCODE_MAINFRAME	115			// chyba vytvoøení hlavního okna
#define EXITCODE_LOADRES	110			// chyba ètení resource

#define EXITCODE_INIT		100			// hranice inicializaèních kódù

#define	EXITCODE_MEMERR		80			// chyba pamìti pøi bìhu programu
#define EXITCODE_OK			0			// øádné ukonèení programu


/////////////////////////////////////////////////////////////////////////////
// test licence

BOOL TestLic();


//////////////////////////////////////////////////////////////////////////////
// vystøedìní okna proti jinému oknu

void CenterWindow(HWND child, HWND parent = NULL);


//////////////////////////////////////////////////////////////////////////////
// spuštìní programu

int Exec(CText command, CText aktdir, BOOL wait);


/////////////////////////////////////////////////////////////////////////////
// generování konverzní tabulky barev obrázku

void GenKonvPal(BITMAPINFO* bmp);


/////////////////////////////////////////////////////////////////////////////
// vygenerování støední barvy (ze 4 bodù)

BYTE ColAvrg(BYTE col1, BYTE col2, BYTE col3, BYTE col4);
BYTE ColAvrgDither(BYTE col1, BYTE col2, BYTE col3, BYTE col4, int x, int y);


/////////////////////////////////////////////////////////////////////////////
// zjištìní editaèního okna s fokusem (NULL = není)

HWND GetEditFocus();


/////////////////////////////////////////////////////////////////////////////
// vytvoøení adresáøe (pokud existuje, je vše OK)

BOOL CreateDir(CText txt);


/////////////////////////////////////////////////////////////////////////////
// vytvoøení adresáøe (i vícestupòovì - vèetnì cesty)

BOOL CreatePath(CText txt);


/////////////////////////////////////////////////////////////////////////////
// dekomprese dat

void DeKomp(BYTE* dstBuf, int dstNum, BYTE* srcBuf, int srcNum);


/////////////////////////////////////////////////////////////////////////////
// generátor náhody

int rand();
double Rand();

inline int sqr(int num) { return num*num; }

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


/////////////////////////////////////////////////////////////////////////////
// zjištìní informací o velikosti a volném místu disku

void GetDiskSpace(CText path);


/////////////////////////////////////////////////////////////////////////////
// mód editace ikony (* = je v editoru ikony, + = je v editoru obrázku))

enum EditMode
{ 
	EditModePen,					// * + pero
	EditModePaint,					// *   štìtec

	EditModeLine,					// * + èára
	EditModeRect,					// * + obdélník
	EditModeRectFill,				//   + vyplnìný obdélník
	EditModeRound,					//   + kružnice
	EditModeRoundFill,				// * + kruh
	EditModeElip,					//   + elipsa
	EditModeElipFill,				// * + ovál

	EditModeWidth1,					//   + pero 1
	EditModeWidth2,					//   + pero 2
	EditModeWidth3,					//   + pero 3
	EditModeWidth5,					//   + pero 5
	EditModeWidth9,					//   + pero 9
	EditModeWidth20,				//   + pero 21

	EditModePipe,					// * + kapátko
	EditModeFill,					// * + výplò
	EditModeSpray,					// * + sprej
	EditModeText,					//   + text

	EditModeZoomIn,					//   + mìøítko zvìtšit
	EditModeZoomOut,				//   + mìøítko zmenšit

	EditModeSelect,					// * + blok
	EditModeXFlip,					// * + horizontální pøevrácení
	EditModeYFlip,					// * + vertikální pøevrácení
	EditModeRRot,					// * + otoèení R
	EditModeLRot,					// * + otoèení L
	EditModeXCol,					// * + zámìna barev
	EditModeSCol,					// * + nastavení barvy

	EditModeMul,					// * + zvìtšení
	EditModeDiv,					// * + zmenšení

	EditMode45,						// * + otoèení o 45 stupòù
	EditMode30,						// * + otoèení o 30 stupòù
	EditMode22,						// * + otoèení o 22.5 stupòù
};


//////////////////////////////////////////////////////////////////////////////
// pomocné konstanty

#define ICONWIDTH 32								// šíøka ikon
#define ICONHEIGHT 32								// výška ikon
#define ICONSIZE (ICONWIDTH*ICONHEIGHT)				// velikost ikon v bajtech

#define MOUSEINV -100000							// neplatná souøadnice myši

//////////////////////////////////////////////////////////////////////////////
// vlastní vnoøené sekce

#include "Memory.h"						// obsluha pamìti
#include "Bitmap.h"						// obsluha bitmap a ikon
#include "Select.h"						// výbìr souboru k editaci
#include "Buffer.h"						// šablona pro buffery
#include "BufIcon.h"					// buffer ikon, data ikon
#include "BufInt.h"						// buffer celých èísel a logických promìnných
#include "BufMap.h"						// buffer mapy plochy
#include "BufMus.h"						// buffer hudby
#include "BufPic.h"						// buffer obrázkù (pozadí)
#include "JPEG.h"						// obsluha JPEG souborù
#include "BufReal.h"					// buffer reálných èísel
#include "BufSnd.h"						// buffer zvuku
#include "BufSprt.h"					// buffer sprajtù
#include "BufText.h"					// buffer textù, textové øetìzce
#include "MainFrm.h"					// hlavní okno aplikace


//////////////////////////////////////////////////////////////////////////////
// fonty

// definice parametrù fontu
class CFont
{
public:
	BOOL	Standard;					// pøíznak standardního fontu
	BOOL	Bold;						// pøíznak tuèného fontu
	BOOL	Italic;						// pøíznak kurzívy (pro standardní font se ignoruje)
	BOOL	Underline;					// pøíznak podtržení (pro standardní font se ignoruje)
	BOOL	Strikeout;					// pøíznak pøeškrtnutí (pro standardní font se ignoruje)
	BOOL	Serif;						// pøíznak patièkového písma (pro standardní font se ignoruje)
	BOOL	Fixed;						// pøíznak písma s pevnou rozteèí
	int		Height;						// výška fontù, 0=bìžná (pro standardní font se ignoruje)
	int		Height0;					// skuteèná výška fontù (vypoètená pøi vytváøení fontu)
	double	Width;						// relativní šíøka fontù, 0=bìžná (pro standardní font se ignoruje)
	int		Width0;						// skuteèná šíøka fontù (vypoètená pøi vytváøení fontu)
	CText	User;						// uživatelsky definovaný font (SERIF i FIXED nastaveny)

// konstruktor a destruktor
	inline CFont()
	{ 
		Standard = TRUE;				// pøíznak standardního fontu
		Bold = FALSE;					// pøíznak tuèného fontu
		Italic = FALSE;					// pøíznak kurzívy
		Underline = FALSE;				// pøíznak podtržení
		Strikeout = FALSE;				// pøíznak pøeškrtnutí
		Serif = FALSE;					// pøíznak patièek
		Fixed = FALSE;					// pøíznak pevných rozteèí
		Height = 0;						// výška fontù
		Height0 = 15;					// skuteèná výška fontù
		Width = 0;						// relativní šíøka fontù
		Width0 = 8;						// skuteèná šíøka fontù
	};

	inline void Init()
	{ 
		Standard = TRUE;				// pøíznak standardního fontu
		Bold = FALSE;					// pøíznak tuèného fontu
		Italic = FALSE;					// pøíznak kurzívy
		Underline = FALSE;				// pøíznak podtržení
		Strikeout = FALSE;				// pøíznak pøeškrtnutí
		Serif = FALSE;					// pøíznak patièek
		Fixed = FALSE;					// pøíznak pevných rozteèí
		Height = 0;						// výška fontù
		Height0 = 15;					// skuteèná výška fontù
		Width = 0;						// relativní šíøka fontù
		Width0 = 8;						// skuteèná šíøka fontù
		User.Init();					// uživatelský font
	};

	inline void Term() { User.Term(); };

	inline const CFont& operator= (const CFont& src)
	{ 
		Standard	= src.Standard;
		Bold		= src.Bold;
		Italic		= src.Italic;
		Underline	= src.Underline;
		Strikeout	= src.Strikeout;
		Serif		= src.Serif;
		Fixed		= src.Fixed;
		Height		= src.Height;
		Height0		= src.Height0;
		Width		= src.Width;
		Width0		= src.Width0;
		User		= src.User;
		return *this; 
	}

	inline friend BOOL operator==(const CFont& fnt1, const CFont& fnt2)
	{
		return (
			(fnt1.Standard	== fnt2.Standard) &&
			(fnt1.Bold		== fnt2.Bold) &&
			(fnt1.Italic	== fnt2.Italic) &&
			(fnt1.Underline	== fnt2.Underline) &&
			(fnt1.Strikeout	== fnt2.Strikeout) &&
			(fnt1.Serif		== fnt2.Serif) &&
			(fnt1.Fixed		== fnt2.Fixed) &&
			(fnt1.Height	== fnt2.Height) &&
			(fnt1.Width		== fnt2.Width) &&
			(fnt1.User		== fnt2.User));
	};
};

extern HFONT	FontBezny;				// bìžný font pro dialogy a stavový øádek
extern HFONT	FontBold;				// tuèný font pro dialogy a nadpisy oken
extern HFONT	FontFixed;				// font s pevnou rozteèí pro dialogy

extern CText	SetupVzor;				// vzor textu pro nastavení písma

extern CFont	SelectFont;				// font výbìru programu
extern CFont	TreeFont;				// font editoru programu
extern CFont	TextFont;				// font editoru textu
extern CFont	MenuFont;				// font nabídky
extern CFont	StatusFont;				// font stavové lišty
extern CFont	GraphicFont;			// font grafického editoru

/////////////////////////////////////////////////////////////////////////////
// vytvoøení fontu

HFONT GetFont(CFont* fnt);


/////////////////////////////////////////////////////////////////////////////
// zrušení fontu

void DelFont(HFONT font);


//////////////////////////////////////////////////////////////////////////////
// nastavení fontù v oknì

void SetFontBezny(HWND wnd);
void SetFontBold(HWND wnd);
void SetFontFixed(HWND wnd);
void SetFontFixedBold(HWND wnd);


#include "BufProg.h"					// buffer programu
#include "BufInx.h"						// buffer indexù
#include "BufUndo.h"					// buffer undo a redo
#include "EditIcon.h"					// editor ikon a obrázkù
#include "EditLog.h"					// editor logických hodnot
#include "EditMap.h"					// editor ploch
#include "EditMus.h"					// editor hudby
#include "EditNum.h"					// editor èísel
#include "EditSnd.h"					// editor zvukù
#include "EditSprt.h"					// editor sprajtù
#include "EditText.h"					// editor textù
#include "File.h"						// obsluha souborù
#include "Compress.h"					// obsluha komprese dat
#include "Prog.h"						// obsluha editace souboru
#include "ProgDrag.h"					// tažení ikon programu
#include "ProgFile.h"					// obsluha souborù
#include "ProgClip.h"					// obsluha clipboardu
#include "ProgExp.h"					// export a import programu
#include "ProgCol.h"					// výbìr barvy
#include "ProgHist.h"					// historie editace
#include "ProgLib.h"					// knihovny


//////////////////////////////////////////////////////////////////////////////
// import palet

// konverze barev
inline void KonvCopy(BYTE* dst, BYTE* src, int num)
{
	if (KonvPalOK)
	{
		MemCopy(dst, src, num);
	}
	else
	{
		for (; num > 0; num--)
		{
			dst[0] = KonvPal[src[0]];
			dst++;
			src++;
		}
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


/////////////////////////////////////////////////////////////////////////////
// naètení registru DWORD (-1 = chyba)

//int GetRegUserDWORD(CText key, CText name);


/////////////////////////////////////////////////////////////////////////////
// nastavení registru DWORD (nevytváøí klíè)

//void SetRegUserDWORD(CText key, CText name, int data);

