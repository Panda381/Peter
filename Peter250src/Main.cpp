
#include "Main.h"

/***************************************************************************\
*																			*
*							Hlavní modul aplikace							*
*																			*
\***************************************************************************/

/*=========================================================================*\
+																			+
+						Globální promìnné a konstanty						+
+																			+
\*=========================================================================*/

//////////////////////////////////////////////////////////////////////////////
// inicializaèní segmenty CRT (konstruktory a destruktory)

typedef void (__cdecl *_PVFV)(void);		// ukazatel na funkci void fnc(void)
typedef void (*PFV)(void);					// funkce void fnc(void)

#pragma data_seg(".CRT$XCA")
_PVFV	__xc_a[] = { NULL };				// konstruktory C++
#pragma data_seg(".CRT$XCZ")
_PVFV	__xc_z[] = { NULL };

#pragma data_seg()							// resetování na bìžnou datovou sekci

#pragma comment(linker, "/MERGE:.CRT=.data")	// pøipojení CRT sekcí do datové sekce


//////////////////////////////////////////////////////////////////////////////
// globální promìnné

TCHAR*		CommandLine = NULL;			// pøíkazový øádek
int			VerzeOS;					// verze systému
OSVERSIONINFO	OSVersionInfo;			// informace o systému
HINSTANCE	hInstance = NULL;			// instance programu
int			ScreenWidth;				// šíøka klientské oblasti displeje
int			ScreenHeight;				// výška klientské oblasti displeje
int			TimerConst;					// konstanta pro èasovaè (pro 55 ms)

HACCEL		Accel = NULL;				// akceleraèní tabulka

BYTE*		StdPalImport;				// tabulka importu palet
BITMAPINFO* StdBitmapInfo;				// standardní záhlaví BMP
HPALETTE	StdPalette = NULL;			// vlastní logické palety
BYTE*		KonvPal;					// konverzní tabulka palet
bool		KonvPalOK = false;			// pøíznak shodné konverzní tabulky (lze provést kopii)

HWND		PrevWindow = NULL;			// pøedešlé aktivní okno

bool		Dither = true;				// používat dithering

bool		ShortName = false;			// zkracovat jména souborù

//bool		Profi = true;				// profesionální verze programu

__int64	DiskSize = 0;			// velikost disku (z funkce GetDiskSpace)
__int64	DiskFree = 0;			// volné místo disku (z funkce GetDiskSpace)
__int64	DiskFreeUser = 0;		// volné místo uživatele (z funkce GetDiskSpace)

GETDISKFREESPACEEX	pGetDiskFreeSpaceEx = NULL;	// funkce GetDiskFreeSpaceEx (NULL=není)

CMultiText	Jmeno;						// jméno editovaného souboru (bez cesty)
CText		JmenoLoad;					// plné jméno souboru pro ètení (s cestou - vzorový adresáø)
CText		JmenoSave;					// plné jméno souboru pro zápis
CText		Cesta;						// cesta k editovanému souboru (s pøíp. "\" na konci)
CText		AktDir;						// aktivní adresáø uživatele (s "\" na konci)
CText		HomeDir;					// cesta do domovského adresáøe s EXE (s "\" na konci)
CText		ExeFileName;				// jméno tohoto programu
CText		IniFileName;				// jméno konfiguraèního souboru
//CText		HelpFileName;				// jméno souboru nápovìdy
CText		TextExe;					// text ".exe"
CText		TextExe2;					// text ".EXE"
CText		CDRom;						// cesta k CD-ROM (prázdný = není)

//////////////////////////////////////////////////////////////////////////////
// cesty do adresáøù programu (všechny jsou zakonèeny s "\") - implicitnì aktuální adresáø

CText		ProgramPath;				// cesta do adresáøe programù "Program" (*.exe)

CText		BackupPath;					// cesta do adresáøe záloh "Backup"

CText		FunctionPath;				// cesta do adresáøe funkcí "Function" (*.fnc)
CText		NumberPath;					// cesta do adresáøe èísel "Number" (*.num)
CText		TextPath;					// cesta do adresáøe textù "Text" (*.txt)
CText		BoolPath;					// cesta do adresáøe logických hodnot "Bool" (*.log)
CText		IconPath;					// cesta do adresáøe ikon "Icon" (*.ico)
CText		MapPath;					// cesta do adresáøe ploch "Map" (*.map)
CText		PicturePath;				// cesta do adresáøe obrázkù "Picture" (*.bmp)
CText		SpritePath;					// cesta do adresáøe sprajtù "Sprite" (*.spr)
CText		SoundPath;					// cesta do adresáøe zvukù "Sound" (*.wav)
CText		MusicPath;					// cesta do adresáøe hudby "Music" (*.mid)

// alternativní knihovny - implicitnì v podadresáøi programu Petr
CText		ProgramPath2;				// cesta do adresáøe programù "Program" (*.exe)

CText		FunctionPath2;				// cesta do adresáøe funkcí "Function" (*.fnc)
CText		NumberPath2;				// cesta do adresáøe èísel "Number" (*.num)
CText		TextPath2;					// cesta do adresáøe textù "Text" (*.txt)
CText		BoolPath2;					// cesta do adresáøe logických hodnot "Bool" (*.log)
CText		IconPath2;					// cesta do adresáøe ikon "Icon" (*.ico)
CText		MapPath2;					// cesta do adresáøe ploch "Map" (*.map)
CText		PicturePath2;				// cesta do adresáøe obrázkù "Picture" (*.bmp)
CText		SpritePath2;				// cesta do adresáøe sprajtù "Sprite" (*.spr)
CText		SoundPath2;					// cesta do adresáøe zvukù "Sound" (*.wav)
CText		MusicPath2;					// cesta do adresáøe hudby "Music" (*.mid)

// alternativní knihovny - implicitnì v ROOT CD-ROM
CText		ProgramPath3;				// cesta do adresáøe programù "Program" (*.exe)

CText		FunctionPath3;				// cesta do adresáøe funkcí "Function" (*.fnc)
CText		NumberPath3;				// cesta do adresáøe èísel "Number" (*.num)
CText		TextPath3;					// cesta do adresáøe textù "Text" (*.txt)
CText		BoolPath3;					// cesta do adresáøe logických hodnot "Bool" (*.log)
CText		IconPath3;					// cesta do adresáøe ikon "Icon" (*.ico)
CText		MapPath3;					// cesta do adresáøe ploch "Map" (*.map)
CText		PicturePath3;				// cesta do adresáøe obrázkù "Picture" (*.bmp)
CText		SpritePath3;				// cesta do adresáøe sprajtù "Sprite" (*.spr)
CText		SoundPath3;					// cesta do adresáøe zvukù "Sound" (*.wav)
CText		MusicPath3;					// cesta do adresáøe hudby "Music" (*.mid)

//////////////////////////////////////////////////////////////////////////////
// jazyk

// textové konstanty
const TEXTCONST TextConst000[] =		// neutrální jazyk (jména souborù)
{
#include "TextEng.inc"
};

const TEXTCONST TextConstAra[] =		// arabsky - arabic - JAKJAK
{
#include "TextAra.inc"
};

const TEXTCONST TextConstBul[] =		// bulharsky
{
#include "TextBul.inc"
};

const TEXTCONST TextConstCz[] =			// èesky
{
#include "TextCz.inc"
};

const TEXTCONST TextConstDan[] =		// dánsky
{
#include "TextDan.inc"
};

const TEXTCONST TextConstGer[] =		// nìmecky
{
#include "TextGer.inc"
};

const TEXTCONST TextConstRec[] =		// øecky
{
#include "TextRec.inc"
};

const TEXTCONST TextConstEng[] =		// anglicky
{
#include "TextEng.inc"
};

const TEXTCONST TextConstSpa[] =		// španìlsky
{
#include "TextSpa.inc"
};

const TEXTCONST TextConstFin[] =		// finsky
{
#include "TextFin.inc"
};

const TEXTCONST TextConstFra[] =		// francouzsky
{
#include "TextFra.inc"
};

const TEXTCONST TextConstHeb[] =		// hebrejsky - hebrew - JAKJAK
{
#include "TextHeb.inc"
};

const TEXTCONST TextConstMad[] =		// maïarsky
{
#include "TextMad.inc"
};

const TEXTCONST TextConstIsl[] =		// islandsky
{
#include "TextIsl.inc"
};

const TEXTCONST TextConstIta[] =		// italsky
{
#include "TextIta.inc"
};

const TEXTCONST TextConstHol[] =		// holandsky
{
#include "TextHol.inc"
};

const TEXTCONST TextConstNor[] =		// norsky
{
#include "TextNor.inc"
};

const TEXTCONST TextConstPol[] =		// polsky
{
#include "TextPol.inc"
};

const TEXTCONST TextConstPor[] =		// portugalsky
{
#include "TextPor.inc"
};

const TEXTCONST TextConstRum[] =		// rumunsky
{
#include "TextRum.inc"
};

const TEXTCONST TextConstRus[] =		// rusky
{
#include "TextRus.inc"
};

const TEXTCONST TextConstSrb[] =		// srbochorvatsky
{
#include "TextSrb.inc"
};

const TEXTCONST TextConstSlo[] =		// slovensky
{
#include "TextSlo.inc"
};

const TEXTCONST TextConstAlb[] =		// albánsky
{
#include "TextAlb.inc"
};

const TEXTCONST TextConstSwe[] =		// šwédsky
{
#include "TextSwe.inc"
};

const TEXTCONST TextConstTur[] =		// turecky
{
#include "TextTur.inc"
};

const TEXTCONST TextConstVie[] =		// vietnamsky - vietnamese - JAKJAK
{
#include "TextVie.inc"
};

int		JazykUser = JAZYKAUT;			// uživatelem zvolený jazyk
int		Jazyk = JAZYKCZ;				// skuteènì nastavený jazyk - musí mít platný rozsah!
int		JazykDef = JAZYKENG;			// implicitní jazyk (podle systému)
int		JazykPrev = JAZYKENG;			// pøedcházející jazyk
DWORD	LangID = 0x09;					// identifikátor jazyku LANGID
BOOL	MultiJazyk = FALSE;				// multijazyènost

DWORD	CharSet = DEFAULT_CHARSET;		// aktuální znaková sada fontù (1=implicitní)
DWORD	CodePage = CP_ACP;				// aktuální kódová stránka (0=implicitní)
DWORD	DefCharSet = DEFAULT_CHARSET;	// implicitní znaková sada fontù
DWORD	DefCodePage = CP_ACP;			// implicitní kódová stránka
char	Carka = '.';					// oddìlovaè desetinné èárky pøi zobrazení èísel

// JAKJAK - WINNT.H ... Primary language IDs. - to jsou ta hex èísla, zde je: #define LANG_TURKISH 0x1f, #define LANG_VIETNAMESE 0x2a

const JAZYKINFO JazykInfo[JAZYKNUM] =
{
	TextConst000,	0x00,	_T("LANGID 0"),		1252,	ANSI_CHARSET,		ButtonEng,	_T("Neutral"),		0,		FALSE,	_T("EN"),	FALSE,	// neutrální - obsahuje jména souborù
	TextConstAra,	0x01,	_T("LANGID 1"),		1256,	ARABIC_CHARSET,		ButtonAra,	_T("Arabic"),		IDN_JAZYKARA,	FALSE,	_T("AR"),	TRUE,	// arabsky
	TextConstBul,	0x02,	_T("LANGID 2"),		1251,	RUSSIAN_CHARSET,	ButtonBul,	_T("Bulgaria"),		IDN_JAZYKBUL,	FALSE,	_T("BU"),	FALSE,	// bulharsky
	TextConstCz,	0x05,	_T("LANGID 5"),		1250,	EASTEUROPE_CHARSET,	ButtonCz,	_T("Czech"),		IDN_JAZYKCZ,	TRUE,	_T("CZ"),	FALSE,	// èesky
	TextConstDan,	0x06,	_T("LANGID 6"),		1252,	ANSI_CHARSET,		ButtonDan,	_T("Danish"),		IDN_JAZYKDAN,	FALSE,	_T("DA"),	FALSE,	// dánsky
	TextConstGer,	0x07,	_T("LANGID 7"),		1252,	ANSI_CHARSET,		ButtonGer,	_T("German"),		IDN_JAZYKGER,	TRUE,	_T("GE"),	FALSE,	// nìmecky
	TextConstRec,	0x08,	_T("LANGID 8"),		1253,	GREEK_CHARSET,		ButtonRec,	_T("Greek"),		IDN_JAZYKREC,	FALSE,	_T("GR"),	FALSE,	// øecky
	TextConstEng,	0x09,	_T("LANGID 9"),		1252,	ANSI_CHARSET,		ButtonEng,	_T("English"),		IDN_JAZYKENG,	TRUE,	_T("EN"),	FALSE,	// anglicky
	TextConstSpa,	0x0a,	_T("LANGID 10"),	1252,	ANSI_CHARSET,		ButtonSpa,	_T("Spanish"),		IDN_JAZYKSPA,	FALSE,	_T("SP"),	FALSE,	// španìlsky
	TextConstFin,	0x0b,	_T("LANGID 11"),	1257,	BALTIC_CHARSET,		ButtonFin,	_T("Finnish"),		IDN_JAZYKFIN,	FALSE,	_T("FI"),	FALSE,	// finsky
	TextConstFra,	0x0c,	_T("LANGID 12"),	1252,	ANSI_CHARSET,		ButtonFra,	_T("France"),		IDN_JAZYKFRA,	FALSE,	_T("FR"),	FALSE,	// francouzsky
	TextConstHeb,	0x0d,	_T("LANGID 13"),	1255,	HEBREW_CHARSET,		ButtonHeb,	_T("Hebrew"),		IDN_JAZYKHEB,	FALSE,	_T("HE"),	TRUE,	// hebrejsky
	TextConstMad,	0x0e,	_T("LANGID 14"),	1250,	EASTEUROPE_CHARSET,	ButtonMad,	_T("Hungarian"),	IDN_JAZYKMAD,	FALSE,	_T("HU"),	FALSE,	// maïarsky
	TextConstIsl,	0x0f,	_T("LANGID 15"),	1257,	BALTIC_CHARSET,		ButtonIsl,	_T("Icelandic"),	IDN_JAZYKISL,	FALSE,	_T("IC"),	FALSE,	// islandsky
	TextConstIta,	0x10,	_T("LANGID 16"),	1252,	ANSI_CHARSET,		ButtonIta,	_T("Italian"),		IDN_JAZYKITA,	FALSE,	_T("IT"),	FALSE,	// italsky
	TextConstHol,	0x13,	_T("LANGID 19"),	1252,	ANSI_CHARSET,		ButtonHol,	_T("Dutch"),		IDN_JAZYKHOL,	FALSE,	_T("DU"),	FALSE,	// holansky
	TextConstNor,	0x14,	_T("LANGID 20"),	1257,	BALTIC_CHARSET,		ButtonNor,	_T("Norwegian"),	IDN_JAZYKNOR,	FALSE,	_T("NO"),	FALSE,	// norsky
	TextConstPol,	0x15,	_T("LANGID 21"),	1250,	EASTEUROPE_CHARSET,	ButtonPol,	_T("Polish"),		IDN_JAZYKPOL,	FALSE,	_T("PL"),	FALSE,	// polsky
	TextConstPor,	0x16,	_T("LANGID 22"),	1252,	ANSI_CHARSET,		ButtonPor,	_T("Portuguese"),	IDN_JAZYKPOR,	FALSE,	_T("PO"),	FALSE,	// portugalsky
	TextConstRum,	0x18,	_T("LANGID 24"),	1251,	RUSSIAN_CHARSET,	ButtonRum,	_T("Romania"),		IDN_JAZYKRUM,	FALSE,	_T("RO"),	FALSE,	// rumunsky
	TextConstRus,	0x19,	_T("LANGID 25"),	1251,	RUSSIAN_CHARSET,	ButtonRus,	_T("Russian"),		IDN_JAZYKRUS,	FALSE,	_T("RU"),	FALSE,	// rusky
	TextConstSrb,	0x1a,	_T("LANGID 26"),	1250,	EASTEUROPE_CHARSET,	ButtonSrb,	_T("Serbo-Croat"),	IDN_JAZYKSRB,	FALSE,	_T("SC"),	FALSE,	// srbochorvátsky
	TextConstSlo,	0x1b,	_T("LANGID 27"),	1250,	EASTEUROPE_CHARSET,	ButtonSlo,	_T("Slovakia"),		IDN_JAZYKSLO,	TRUE,	_T("SK"),	FALSE,	// slovensky
	TextConstAlb,	0x1c,	_T("LANGID 28"),	1253,	GREEK_CHARSET,		ButtonAlb,	_T("Albanian"),		IDN_JAZYKALB,	FALSE,	_T("AL"),	FALSE,	// albánsky
	TextConstSwe,	0x1d,	_T("LANGID 29"),	1257,	BALTIC_CHARSET,		ButtonSwe,	_T("Swedish"),		IDN_JAZYKSWE,	FALSE,	_T("SW"),	FALSE,	// švédsky
	TextConstTur,	0x1f,	_T("LANGID 31"),	1254,	TURKISH_CHARSET,	ButtonTur,	_T("Turkish"),		IDN_JAZYKTUR,	FALSE,	_T("TU"),	FALSE,	// turecky
	TextConstVie,	0x2a,	_T("LANGID 42"),	1258,	VIETNAMESE_CHARSET,	ButtonVie,	_T("Vietnamese"),	IDN_JAZYKVIE,	FALSE,	_T("VI"),	FALSE,	// vietnamsky
};

const int TextConstNum = sizeof(TextConstCz)/sizeof(TextConstCz[0]);


//////////////////////////////////////////////////////////////////////////////
// fonty

HFONT		FontDefault = NULL;			// implicitní systémový font
HFONT		FontDefaultBold = NULL;		// implicitní systémový font BOLD
HFONT		FontDefaultFixed = NULL;	// implicitní systémový font FIXED
HFONT		FontDefaultBoldFixed = NULL;// implicitní systémový font BOLD FIXED

HFONT		FontBezny = NULL;			// bìžný font pro dialogy a stavový øádek
HFONT		FontBold = NULL;			// tuèný font pro dialogy a nadpisy oken
HFONT		FontFixed = NULL;			// font s pevnou rozteèí pro dialogy
HFONT		FontFixedBold = NULL;		// tuèný font s pevnou rozteèí pro dialogy

CText		UserFont(_T("Impact"));		// uživatelský font (SERIF i FIXED nastaveny)

CText		SetupVzor;					// vzor textu pro nastavení písma

CFont		SelectFont;					// font výbìru programu
CFont		TreeFont;					// font editoru programu
CFont		TextFont;					// font editoru textu
CFont		MenuFont;					// font nabídky
CFont		StatusFont;					// font stavové lišty
CFont		GraphicFont;				// font grafického editoru

//////////////////////////////////////////////////////////////////////////////
// standardní fonty (pro tažení ikon ve stromu) (není komprimováno!)

CBufIcon	StdFonts;					// buffer standardních fontù (indexy 0 až 255)
CBufIndex	StdFontsWidth;				// šíøky znakù standardních fontù (indexy 0 až 255)


//////////////////////////////////////////////////////////////////////////////
// lokální promìnné

#ifdef _MT
static	CRITICAL_SECTION	ExitCriticalSection;	// kritická sekce pro konec programu
#endif

unsigned	RandomSeed;					// promìnná pro generátor náhody


//////////////////////////////////////////////////////////////////////////////
// konfigurace

int Scale;					// mìøítko pro relativní rozmìry okna

int MainLeft;				// relativní pozice hlavního okna vlevo
int MainTop;				// relativní pozice hlavního okna nahoøe
int MainWidth;				// relativní šíøka hlavního okna
int MainHeight;				// relativní výška hlavního okna

int LeftWidth;				// relativní šíøka levého pole (objekty)
int LeftHeight;				// relativní výška levého horního okna (globální objekty)
int RightWidth;				// relativní šíøka pravého pole (tøídy)
int RightHeight;			// relativní výška pravého horního okna (struktury)

BOOL Maximized;				// pøíznak maximalizace okna

CText FindString;			// hledaný text

SMALLICON BufZoom[PROGBUFNUM];	// pøíznaky zoom oken (TRUE=zvìtšeno)

//////////////////////////////////////////////////////////////////////////////
// konstanty

const long double pi  = 3.14159265358979323846;		// Ludolfovo èíslo
const long double pi2 = 6.28318530717958647693;		// Ludolfovo èíslo * 2
const long double eul = 2.71828182845904523536;		// Eulerovo èíslo

const long double uhel22  = 0.39269908169872415481;	// úhel 22.5 stupòù v radiánech
const long double uhel30  = 0.52359877559829887308; // úhel 30 stupòù v radiánech
const long double uhel45  = 0.78539816339744830962;	// úhel 45 stupòù v radiánech
const long double uhel60  = 1.04719755119659774615; // úhel 60 stupòù v radiánech
const long double uhel67  = 1.17809724509617246442; // úhel 67.5 stupòù v radiánech
const long double uhel90  = 1.57079632679489661923;	// úhel 90 stupòù v radiánech
const long double uhel135 = 2.35619449019234492885;	// úhel 135 stupòù v radiánech
const long double uhel180 = 3.14159265358979323846;	// úhel 180 stupòù v radiánech
const long double uhel225 = 3.92699081698724154808;	// úhel 225 stupòù v radiánech
const long double uhel270 = 4.71238898038468985769;	// úhel 270 stupòù v radiánech
const long double uhel315 = 5.49778714378213816731;	// úhel 315 stupòù v radiánech

const long double ln10 = 2.3025850929940456840;	// pøirozený logaritmus 10

const long double degrad = 0.017453292519943295769237;	// konstanta pro pøevod z DEG na RAD (pi/180)
const long double raddeg = 57.2957795130823208768;	// konstanta pro pøevod z RAD na DEG (180/pi)

CText		MemErrNadpis;							// nadpis okna chybového hlášení nedostatku pamìti
LPCTSTR		MemErrNadpisP0 = _T("Memory Error");
LPCTSTR		MemErrNadpisP = MemErrNadpisP0;
CText		MemErrText;								// text okna chybového hlášení nedostatku pamìti
LPCTSTR		MemErrTextP0 = _T("Insufficient memory to run PETER.EXE.");
LPCTSTR		MemErrTextP = MemErrTextP0;

/*=========================================================================*\
+																			+
+								Palety a barvy								+
+																			+
\*=========================================================================*/

// tabulka hladin standardních barev (rozsah 0 až 256)
const int ColLevTab[] = 
{
	64*4,
	54*4,
	44*4,
	33*4,
	22*4,
	11*4,
};

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// po zmìnì palet zruš soubor PALIMP.DAT a spus DEBUG OPTIM verzi pro nové vygenerování
// (pozor - generování mùže trvat desítky minut). Potom znovu spustit pøeklad.
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// tabulka odstínù standardních barev (0 až 255) - poøadí B, G, R, F,
const RGBQUAD ColColTab[] = 
{ 
	0,		0,		255,		0,			//  0: èervená
	72,		72,		255,		0,			//  1:
	116,	116,	255,		0,			//  2:
	158,	166,	255,		0,			//  3:
	210,	220,	255,		0,			//  4:
	0,		114,	255,		0,			//  5: hnìdá
	150,	198,	255,		0,			//  6:
	0,		182,	255,		0,			//  7: oranžová
	96,		192,	255,		0,			//  8:
	0,		255,	255,		0,			//  9: žlutá
	128,	255,	255,		0,			// 10:
	192,	255,	255,		0,			// 11: svìtle žlutá
	0,		255,	178,		0,			// 12: žlutozelená
	0,		255,	0,			0,			// 13: zelená
	128,	255,	128,		0,			// 14:
	192,	255,	192,		0,			// 15:
	146,	255,	0,			0,			// 16: namodrale zelená
	222,	255,	138,		0,			// 17:
	255,	255,	0,			0,			// 18: modrozelená
	255,	186,	0,			0,			// 19:
	255,	222,	124,		0,			// 20:
	255,	128,	0,			0,			// 21:
	255,	0,		0,			0,			// 22: modrá
	255,	80,		72,			0,			// 23:
	255,	144,	128,		0,			// 24:
	255,	192,	154,		0,			// 25:
	255,	238,	210,		0,			// 26:
	255,	0,		114,		0,			// 27:
	255,	0,		174,		0,			// 28:
	255,	162,	198,		0,			// 29:
	255,	0,		255,		0,			// 30: fialová
	255,	128,	255,		0,			// 31: svìtle fialová
	138,	0,		255,		0,			// 32: fialovì èervená
	192,	128,	255,		0,			// 33:
	210,	186,	255,		0,			// 34:

	255,	255,	255,		0,			// 35: bílá (nepoužije se, vygeneruje se)
	96,		96,		96,			0,			// 36: šedá (nepoužije se, vygeneruje se)
};

const int ColLev = sizeof(ColLevTab)/sizeof(ColLevTab[0]);	// poèet hladin barev
const int ColCol = sizeof(ColColTab)/sizeof(ColColTab[0]);	// poèet odstínù barev
const int StdColors = ResCols + ColCol*ColLev;				// poèet vlastních palet (zaèínají od 0)
const BYTE WhiteCol = StdColors - 2*ColLev;			// bílá barva
const BYTE BlackCol = StdColors - 1;				// èerná barva

const BYTE BlueCol = 22*ColLev + ResCols;			// modrá barva
const BYTE RedCol = 0*ColLev + ResCols;				// èervená barva
const BYTE YellowCol = 9*ColLev + ResCols;			// žlutá barva
const BYTE GreenCol = 13*ColLev + ResCols;			// zelená barva
const BYTE LtYellowCol = 11*ColLev + ResCols;		// svìtle žlutá barva
const BYTE OrangeCol = 7*ColLev + ResCols;			// oranžová

/////////////////////////////////////////////////////////////////////////////
// vygenerování tabulky pro import palet

#ifndef _OPTIM
#ifdef _DEBUG

void GenPalImport()
{
// lokální promìnné
	short i;				// pracovní èítaè
	short akt;				// vzdálenost od aktuální barvy
	BYTE last = 0;			// poslední nalezená nejbližší barva
	short min;				// vzdálenost od nalezené nejbližší barvy
	BYTE r,g,b;				// hledaná barva
	short ri, gi, bi;		// èítaèe barev
	RGBQUAD* rgb;			// ukazatel standardních palet

// cyklus pøes všechny hodnoty barev
	for (ri = 63; ri >=0; ri--)
	{
		r = (BYTE)(ri*4 + ri/16);

		for (gi = 63; gi >= 0; gi--)
		{
			g = (BYTE)(gi*4 + gi/16);

			for (bi = 63; bi >= 0; bi--)
			{
				b = (BYTE)(bi*4 + bi/16);


// pøíprava ukazatelù
				min = 3*256;		// asi tak nìjaká velká vzdálenost
				rgb = &(StdBitmapInfo->bmiColors[ResCols]);	// ukazatel barev

// cyklus pøes všechny standardní barvy
				for (i = ResCols; i < StdColors; i++)
				{

// vzdálenost od aktuální barvy
					akt =	(short)(
							abs(rgb->rgbRed - r) +
							abs(rgb->rgbGreen - g) +
							abs(rgb->rgbBlue - b));

// kontrola vzdálenosti od standardní barvy
					if (akt < min)
					{
						min = akt;			// nová vzdálenost
						last = (BYTE)i;		// nový index nejbližší barvy
					}

// posun na další standardní barvu
					rgb++;
				}

// konverzní hodnota
				StdPalImport[ri + 64*gi + 64*64*bi] = last;
			}
		}
	}
}

#endif // _DEBUG
#endif // _OPTIM

/*=========================================================================*\
+																			+
+								Pùltónový štìtec							+
+																			+
\*=========================================================================*/

//////////////////////////////////////////////////////////////////////////////
// zásobník objektù

HBRUSH		HalfToneBrush;				// pùltónový štìtec

const WORD HalfTonePattern[8] = 
{
	0x5555,
	0xaaaa,
	0x5555,
	0xaaaa,
	0x5555,
	0xaaaa,
	0x5555,
	0xaaaa
};

//////////////////////////////////////////////////////////////////////////////
// inicializace pùltónového štìtce

void InitHalfTone()
{
// vytvoøení bitmapy s výplní
	HBITMAP bmp = ::CreateBitmap(8, 8, 1, 1, HalfTonePattern);

// vytvoøení štìtce
	HalfToneBrush = ::CreatePatternBrush(bmp);

// zrušení bitmapy
	::DeleteObject(bmp);
}


/*=========================================================================*\
+																			+
+								Inicializace								+
+																			+
\*=========================================================================*/

//////////////////////////////////////////////////////////////////////////////
// pomocné texty pro INI

// celkové
LPCTSTR tGlobGroup		= _T("Global");
//LPCTSTR tDither			= _T("Dithering");
LPCTSTR tShortName		= _T("ShortName");

// jazyk
LPCTSTR tLanguage		= _T("Language");
LPCTSTR tLangPrev		= _T("LangPrev");
LPCTSTR	tLangMulti		= _T("LangMulti");
LPCTSTR tLanguageAuto	= _T("Auto");

// hlavní okno
LPCTSTR	tFramGroup		= _T("Frame");

LPCTSTR	tScale			= _T("Scale");

LPCTSTR	tMainLeft		= _T("Left");
LPCTSTR	tMainTop		= _T("Top");
LPCTSTR	tMainWidth		= _T("Width");
LPCTSTR	tMainHeight		= _T("Height");

LPCTSTR	tLeftWidth		= _T("LeftWidth");
LPCTSTR	tLeftHeight		= _T("LeftHeight");
LPCTSTR	tRightWidth		= _T("RightWidth");
LPCTSTR	tRightHeight	= _T("RightHeight");

LPCTSTR	tMaximized		= _T("Maximized");

LPCTSTR	tZoomCls		= _T("ZoomClass");
LPCTSTR	tZoomObj		= _T("ZoomGlobal");
LPCTSTR	tZoomLoc		= _T("ZoomLocal");
LPCTSTR	tZoomEdi		= _T("ZoomEdit");
LPCTSTR	tZoomStr		= _T("ZoomStruc");

// fonty
LPCTSTR tSelectFontGrp	= _T("SelectFont");
LPCTSTR tTreeFontGrp	= _T("TreeFont");
LPCTSTR tTextFontGrp	= _T("TextFont");
LPCTSTR tMenuFontGrp	= _T("MenuFont");
LPCTSTR tStatusFontGrp	= _T("StatusFont");
LPCTSTR tGraphicFontGrp	= _T("GraphicFont");

LPCTSTR tFontStd		= _T("Standard");
LPCTSTR tFontBold		= _T("Bold");
LPCTSTR tFontItalic		= _T("Italic");
LPCTSTR tFontUnderline	= _T("Underline");
LPCTSTR tFontStrikeout	= _T("StrikeOut");
LPCTSTR tFontSerif		= _T("Serif");
LPCTSTR tFontFixed		= _T("Courier");
LPCTSTR tFontHeight		= _T("Height");
LPCTSTR tFontWidth		= _T("Width");
LPCTSTR tFontUser		= _T("User");

// poslední použítí
LPCTSTR	tHistGroup		= _T("History");

LPCTSTR	tLastFile		= _T("LastFile");
LPCTSTR	tLastPath		= _T("LastPath");
LPCTSTR	tFindText		= _T("FindText");

//////////////////////////////////////////////////////////////////////////////
// naètení konfigurace z INI souboru

int ReadIniInt(LPCTSTR group, LPCTSTR key, int def)
{
	int result = ::GetPrivateProfileInt(group, key, def, IniFileName);
	if (result < -Scale) result = -Scale;
	if (result > Scale) result = Scale;
	if ((key != tMainLeft) && (result < 0)) result = 0;
	return result;
}

CText ReadIniTxt(LPCTSTR group, LPCTSTR key, LPCTSTR def)
{
	TCHAR buf[512];
	return CText(buf, ::GetPrivateProfileString(group, key, def, buf, 512, IniFileName));
}

double ReadIniNum(LPCTSTR group, LPCTSTR key, double def)
{
	CText txtdef;
	txtdef.Double(def);
	CText txt;
	txt = ReadIniTxt(group, key, txtdef);
	return Double(txt);
}

void ReadIniFnt(LPCTSTR grp, CFont* fnt)
{
	fnt->Standard		= ReadIniInt(grp,	tFontStd,		fnt->Standard);
	fnt->Bold			= ReadIniInt(grp,	tFontBold,		fnt->Bold);
	fnt->Italic			= ReadIniInt(grp,	tFontItalic,	fnt->Italic);
	fnt->Underline		= ReadIniInt(grp,	tFontUnderline,	fnt->Underline);
	fnt->Strikeout		= ReadIniInt(grp,	tFontStrikeout,	fnt->Strikeout);
	fnt->Serif			= ReadIniInt(grp,	tFontSerif,		fnt->Serif);
	fnt->Fixed			= ReadIniInt(grp,	tFontFixed,		fnt->Fixed);
	fnt->Height			= ReadIniInt(grp,	tFontHeight,	fnt->Height);
	fnt->Width			= ReadIniNum(grp,	tFontWidth,		fnt->Width);
	fnt->User			= ReadIniTxt(grp,	tFontUser,		fnt->User);
}

void ReadIni()
{
// hlavní okno
	Scale				= 100000;
	Scale				= ReadIniInt(tFramGroup, tScale,		10000);
	if (Scale < 100) Scale = 100;

	MainLeft			= ReadIniInt(tFramGroup, tMainLeft,		Scale * 1/32);
	MainTop				= ReadIniInt(tFramGroup, tMainTop,		Scale * 1/32);
	MainWidth			= ReadIniInt(tFramGroup, tMainWidth,	Scale * 30/32);
	MainHeight			= ReadIniInt(tFramGroup, tMainHeight,	Scale * 30/32);

	LeftWidth			= ReadIniInt(tFramGroup, tLeftWidth,	Scale * 1/5);
	LeftHeight			= ReadIniInt(tFramGroup, tLeftHeight,	Scale * 2/3);
	RightWidth			= ReadIniInt(tFramGroup, tRightWidth,	Scale * 1/5);
	RightHeight			= ReadIniInt(tFramGroup, tRightHeight,	Scale * 2/3);

	Maximized			= ReadIniInt(tFramGroup, tMaximized,	FALSE);

	BufZoom[BufClsID]	= (SMALLICON)ReadIniInt(tFramGroup, tZoomCls,		(int)((ScreenWidth >= (800+1024)/2) ? SI_BIG : SI_MEDIUM));
	BufZoom[BufObjID]	= (SMALLICON)ReadIniInt(tFramGroup, tZoomObj,		(int)((ScreenWidth >= (800+1024)/2) ? SI_BIG : SI_MEDIUM));
	BufZoom[BufLocID]	= (SMALLICON)ReadIniInt(tFramGroup, tZoomLoc,		(int)((ScreenWidth >= (800+1024)/2) ? SI_BIG : SI_MEDIUM));
	BufZoom[BufEdiID]	= (SMALLICON)ReadIniInt(tFramGroup, tZoomEdi,		(int)((ScreenWidth >= (640+800)/2) ? SI_BIG : SI_MEDIUM));
	BufZoom[BufStrID]	= (SMALLICON)ReadIniInt(tFramGroup, tZoomStr,		(int)((ScreenWidth >= (800+1024)/2) ? SI_BIG : SI_MEDIUM));

// fonty
	SelectFont.User = _T("Arial");
	ReadIniFnt(tSelectFontGrp,	&SelectFont);

	TreeFont.User = _T("Impact");
	TreeFont.Height = 20;
	ReadIniFnt(tTreeFontGrp,	&TreeFont);

	TextFont.Bold = TRUE;
	TextFont.User = _T("Arial");
	ReadIniFnt(tTextFontGrp,	&TextFont);

	MenuFont.User = _T("Arial");
	ReadIniFnt(tMenuFontGrp,	&MenuFont);

	StatusFont.User = _T("Arial");
	ReadIniFnt(tStatusFontGrp,	&StatusFont);

	GraphicFont.User = _T("Arial");
	ReadIniFnt(tGraphicFontGrp,	&GraphicFont);

// poslední použití
	Jmeno.MultiText(JAZYK000, ReadIniTxt(tHistGroup, tLastFile, Jmeno.MultiText((int)JAZYK000)));
	Cesta				= ReadIniTxt(tHistGroup, tLastPath,		Cesta);
	FindString			= ReadIniTxt(tHistGroup, tFindText,		EmptyText);

// zkrácená jména
	ShortName			= (ReadIniInt(tGlobGroup, tShortName, FALSE) != FALSE);

// jazyk
	CText jazyk			= ReadIniTxt(tGlobGroup, tLanguage,		tLanguageAuto);
	JazykUser = JAZYKAUT;
	Jazyk = JazykDef;
	for (int i = 0; i < JAZYKNUM; i++)
	{
		CText txt(JazykInfo[i].Name);
		if (jazyk == txt)
		{
			JazykUser = i;
			Jazyk = i;
		}
	}

// pøedešlý jazyk
	jazyk				= ReadIniTxt(tGlobGroup, tLangPrev,		JazykInfo[JAZYKENG].Name);
	JazykPrev = JAZYKENG;
	for (i = 0; i < JAZYKNUM; i++)
	{
		CText txt(JazykInfo[i].Name);
		if (jazyk == txt)
		{
			JazykPrev = i;
		}
	}

// multijazyk
	MultiJazyk			= ReadIniInt(tGlobGroup, tLangMulti,	FALSE);
}

//////////////////////////////////////////////////////////////////////////////
// uložení konfigurace do INI souboru

void WriteIniInt(LPCTSTR group, LPCTSTR key, int num)
{
	CText txt;
	txt.Int(num);
	::WritePrivateProfileString(group, key, txt, IniFileName);
}

void WriteIniTxt(LPCTSTR group, LPCTSTR key, CText txt)
{
	::WritePrivateProfileString(group, key, 
			CText(_T('"') + txt + _T('"')), IniFileName);
}

void WriteIniNum(LPCTSTR group, LPCTSTR key, double num)
{
	CText txt;
	txt.Double(num);
	WriteIniTxt(group, key, txt);
}

void WriteIniFnt(LPCTSTR grp, CFont* fnt)
{
	WriteIniInt(grp,	tFontStd,		fnt->Standard);
	WriteIniInt(grp,	tFontBold,		fnt->Bold);
	WriteIniInt(grp,	tFontItalic,	fnt->Italic);
	WriteIniInt(grp,	tFontUnderline,	fnt->Underline);
	WriteIniInt(grp,	tFontStrikeout,	fnt->Strikeout);
	WriteIniInt(grp,	tFontSerif,		fnt->Serif);
	WriteIniInt(grp,	tFontFixed,		fnt->Fixed);
	WriteIniInt(grp,	tFontHeight,	fnt->Height);
	WriteIniNum(grp,	tFontWidth,		fnt->Width);
	WriteIniTxt(grp,	tFontUser,		fnt->User);
}

void WriteIni()
{
// zkrácená jména
	WriteIniInt(tGlobGroup, tShortName, ShortName);

// jazyk
	if ((DWORD)JazykUser > (DWORD)JAZYKNUM)
	{
		WriteIniTxt(tGlobGroup, tLanguage, tLanguageAuto);
	}
	else
	{
		WriteIniTxt(tGlobGroup, tLanguage, JazykInfo[JazykUser].Name);
	}
	WriteIniTxt(tGlobGroup, tLangPrev, JazykInfo[JazykPrev].Name);

	if (MultiJazyk)
	{
		WriteIniInt(tGlobGroup, tLangMulti, TRUE);
	}
	else
	{
		::WritePrivateProfileString(tGlobGroup, tLangMulti, NULL, IniFileName);
	}

// hlavní okno
	WriteIniInt(tFramGroup, tScale,			Scale);

	WriteIniInt(tFramGroup, tMainLeft,		MainLeft);
	WriteIniInt(tFramGroup, tMainTop,		MainTop);
	WriteIniInt(tFramGroup, tMainWidth,		MainWidth);
	WriteIniInt(tFramGroup, tMainHeight,	MainHeight);

	WriteIniInt(tFramGroup, tLeftWidth,		LeftWidth);
	WriteIniInt(tFramGroup, tLeftHeight,	LeftHeight);
	WriteIniInt(tFramGroup, tRightWidth,	RightWidth);
	WriteIniInt(tFramGroup, tRightHeight,	RightHeight);
	
	WriteIniInt(tFramGroup, tMaximized,		Maximized);

	WriteIniInt(tFramGroup, tZoomCls,		(int)BufZoom[BufClsID]);
	WriteIniInt(tFramGroup, tZoomObj,		(int)BufZoom[BufObjID]);
	WriteIniInt(tFramGroup, tZoomLoc,		(int)BufZoom[BufLocID]);
	WriteIniInt(tFramGroup, tZoomEdi,		(int)BufZoom[BufEdiID]);
	WriteIniInt(tFramGroup, tZoomStr,		(int)BufZoom[BufStrID]);

// fonty
	WriteIniFnt(tSelectFontGrp,	&SelectFont);
	WriteIniFnt(tTreeFontGrp,	&TreeFont);
	WriteIniFnt(tTextFontGrp,	&TextFont);
	WriteIniFnt(tMenuFontGrp,	&MenuFont);
	WriteIniFnt(tStatusFontGrp,	&StatusFont);
	WriteIniFnt(tGraphicFontGrp,&GraphicFont);

// poslední použití
	WriteIniTxt(tHistGroup, tLastFile,		Jmeno.MultiText((int)JAZYK000));
	WriteIniTxt(tHistGroup, tLastPath,		Cesta);
	WriteIniTxt(tHistGroup, tFindText,		FindString);
}


//////////////////////////////////////////////////////////////////////////////
// inicializace cest do adresáøù

void InitPaths()
{
// cesta k CD-ROM
	for (int i = _T('C'); i <= _T('Z'); i++)
	{
		CDRom = (TCHAR)i;
		CDRom += _T(":\\");
		if (::GetDriveType(CDRom) == DRIVE_CDROM) break;
	}
	if (i > _T('Z')) CDRom.Empty();

// cesty do lokálních adresáøù
	ProgramPath		= AktDir + _T("Program\\");
	BackupPath		= AktDir + _T("Backup\\");
	FunctionPath	= AktDir + _T("Function\\");
	NumberPath		= AktDir + _T("Number\\");
	TextPath		= AktDir + _T("Text\\");
	BoolPath		= AktDir + _T("Bool\\");
	IconPath		= AktDir + _T("Icon\\");
	MapPath			= AktDir + _T("Map\\");
	PicturePath		= AktDir + _T("Picture\\");
	SpritePath		= AktDir + _T("Sprite\\");
	SoundPath		= AktDir + _T("Sound\\");
	MusicPath		= AktDir + _T("Music\\");

// test, zda to je stejný adresáø
	CText aktdir = AktDir;
	CText homedir = HomeDir;
	aktdir.UpperCase();
	homedir.UpperCase();

	if (aktdir != homedir)
	{

// cesty do vzorových adresáøù
		homedir = HomeDir + _T("Program");
		if ((int)::GetFileAttributes(homedir) != -1)
		{
			ProgramPath2 = homedir + _T('\\');
		}

		homedir = HomeDir + _T("Function");
		if ((int)::GetFileAttributes(homedir) != -1)
		{
			FunctionPath2 = homedir + _T('\\');
		}

		homedir = HomeDir + _T("Number");
		if ((int)::GetFileAttributes(homedir) != -1)
		{
			NumberPath2 = homedir + _T('\\');
		}

		homedir = HomeDir + _T("Text");
		if ((int)::GetFileAttributes(homedir) != -1)
		{
			TextPath2 = homedir + _T('\\');
		}

		homedir = HomeDir + _T("Bool");
		if ((int)::GetFileAttributes(homedir) != -1)
		{
			BoolPath2 = homedir + _T('\\');
		}

		homedir = HomeDir + _T("Icon");
		if ((int)::GetFileAttributes(homedir) != -1)
		{
			IconPath2 = homedir + _T('\\');
		}

		homedir = HomeDir + _T("Map");
		if ((int)::GetFileAttributes(homedir) != -1)
		{
			MapPath2 = homedir + _T('\\');
		}

		homedir = HomeDir + _T("Picture");
		if ((int)::GetFileAttributes(homedir) != -1)
		{
			PicturePath2 = homedir + _T('\\');
		}

		homedir = HomeDir + _T("Sprite");
		if ((int)::GetFileAttributes(homedir) != -1)
		{
			SpritePath2 = homedir + _T('\\');
		}

		homedir = HomeDir + _T("Sound");
		if ((int)::GetFileAttributes(homedir) != -1)
		{
			SoundPath2 = homedir + _T('\\');
		}

		homedir = HomeDir + _T("Music");
		if ((int)::GetFileAttributes(homedir) != -1)
		{
			MusicPath2 = homedir + _T('\\');
		}
	}

// cesty do vzorových adresáøù CD-ROM (mìní se pøi otevøení CD-ROM)
		ProgramPath3	= CDRom;
		FunctionPath3	= CDRom;
		NumberPath3		= CDRom;
		TextPath3		= CDRom;
		BoolPath3		= CDRom;
		IconPath3		= CDRom;
		MapPath3		= CDRom;
		PicturePath3	= CDRom;
		SpritePath3		= CDRom;
		SoundPath3		= CDRom;
		MusicPath3		= CDRom;
}

/*=========================================================================*\
+																			+
+								Jazyková podpora							+
+																			+
\*=========================================================================*/

//////////////////////////////////////////////////////////////////////////////
// tabulka k provìøení textových konstant

#ifdef _DEBUG

const int TestTextTab[] = 
{
// [0] Rùzné texty
	IDN_MAINFRAME,
	IDN_NADPISOBJ,
	IDN_NADPISLOC,
	IDN_NADPISSTR,
	IDN_NADPISCLS,
	IDN_ZAKLADNI_OBJEKTY,
	IDN_NEWFILE_NAME,
	IDN_NEWDIR_NAME,
	IDN_TEXTVZOR,
	IDN_STAT_REF,
	IDT_RADEK,
	IDT_POZICE,
	IDN_ZAPNUTO,
	IDN_DATUM,
	IDN_CAS,
	IDN_VELIKOST,
	IDN_VOLNE,
	IDN_KB,
	IDN_MB,
	IDN_OPAKOVANI,
	IDN_OPAKOVANE,
	IDN_DEMO,
	IDN_PROFI,
	IDN_LITE,
// [1] Menu "Soubor" výbìru programù - doplòuje menu "Soubor" editoru
	IDN_FILE0,
	IDN_START0,
	IDN_START,
	IDN_OPEN0,
	IDN_OPEN,
	IDN_NEW0,
	IDN_NEW,
	IDN_RENAME0,
	IDN_RENAME,
	IDN_NEWCOPY0,
	IDN_NEWCOPY,
	IDN_PATH0,
	IDN_PATH,
	IDN_DELETE0,
	IDN_DELETE,
	IDN_SHORTNAME0,
	IDN_SHORTNAME,
	IDN_EXIT0,
	IDN_EXIT,
// [1] Podmenu "Písmo" je souèástí menu "Soubor"
	IDN_SETUP0,
	IDN_SETUP10,
	IDN_SETUP1,
	IDN_SETUP20,
	IDN_SETUP2,
	IDN_SETUP30,
	IDN_SETUP3,
	IDN_SETUP40,
	IDN_SETUP4,
	IDN_SETUP50,
	IDN_SETUP5,
// [1] Podmenu "Jazyk" - je souèástí menu "Soubor"
	IDN_JAZYK0,
	IDN_JAZYKAUT0,
	IDN_JAZYKAUT,
	IDN_JAZYKPRE0,
	IDN_JAZYKPRE,
	IDN_JAZYKBUL0,
	IDN_JAZYKBUL,
	IDN_JAZYKCZ0,
	IDN_JAZYKCZ,
	IDN_JAZYKDAN0,
	IDN_JAZYKDAN,
	IDN_JAZYKGER0,
	IDN_JAZYKGER,
	IDN_JAZYKREC0,
	IDN_JAZYKREC,
	IDN_JAZYKENG0,
	IDN_JAZYKENG,
	IDN_JAZYKSPA0,
	IDN_JAZYKSPA,
	IDN_JAZYKFIN0,
	IDN_JAZYKFIN,
	IDN_JAZYKFRA0,
	IDN_JAZYKFRA,
	IDN_JAZYKMAD0,
	IDN_JAZYKMAD,
	IDN_JAZYKISL0,
	IDN_JAZYKISL,
	IDN_JAZYKITA0,
	IDN_JAZYKITA,
	IDN_JAZYKHOL0,
	IDN_JAZYKHOL,
	IDN_JAZYKNOR0,
	IDN_JAZYKNOR,
	IDN_JAZYKPOL0,
	IDN_JAZYKPOL,
	IDN_JAZYKPOR0,
	IDN_JAZYKPOR,
	IDN_JAZYKRUM0,
	IDN_JAZYKRUM,
	IDN_JAZYKRUS0,
	IDN_JAZYKRUS,
	IDN_JAZYKSRB0,
	IDN_JAZYKSRB,
	IDN_JAZYKSLO0,
	IDN_JAZYKSLO,
	IDN_JAZYKALB0,
	IDN_JAZYKALB,
	IDN_JAZYKSWE0,
	IDN_JAZYKSWE,
	IDN_JAZYKTUR0,
	IDN_JAZYKTUR,
	IDN_JAZYKVIE0,
	IDN_JAZYKVIE,
	IDN_JAZYKARA0,
	IDN_JAZYKARA,
	IDN_JAZYKHEB0,
	IDN_JAZYKHEB,
// [1] Menu "Pomoc" (Nápovìda)
	IDN_HLP0,
	IDN_HELP0,
	IDN_HELP,
	IDN_HELPT0,
	IDN_HELPT,
	IDN_HELPF0,
	IDN_HELPF,
	IDN_ABOUT0,
	IDN_ABOUT,
// [1] Menu "Soubor" editoru - doplòuje menu "Soubor" výbìru programù
	IDN_CLOSE0,
	IDN_CLOSE,
	IDN_SAVE0,
	IDN_SAVE,
	IDN_SAVEN0,
	IDN_SAVEN,
	IDN_SAVE_AS0,
	IDN_SAVE_AS,
	IDN_SAVE_REL0,
	IDN_SAVE_REL,
	IDN_EXPORT0,
	IDN_EXPORT,
	IDN_IMPORT0,
	IDN_IMPORT,
// [1] Menu "Úpravy" (Editace)
	IDN_EDIT0,
	IDN_UNDO0,
	IDN_UNDO,
	IDN_REDO0,
	IDN_REDO,
	IDN_CUT0,
	IDN_CUT,
	IDN_COPY0,
	IDN_COPY,
	IDN_PASTE0,
	IDN_PASTE,
	IDN_CLEAR0,
	IDN_CLEAR,
	IDN_ALL0,
	IDN_ALL,
	IDN_DITHER0,
	IDN_DITHER,
// [1] Menu "Zobrazení"
	IDN_ZOBRAZ0,
	IDN_IN0,
	IDN_IN,
	IDN_OUT0,
	IDN_OUT,
	IDN_HIST_PREV0,
	IDN_HIST_PREV,
	IDN_HIST_NEXT0,
	IDN_HIST_NEXT,
	IDN_FIND0,
	IDN_FIND,
	IDN_FIND_PREV0,
	IDN_FIND_PREV,
	IDN_FIND_NEXT0,
	IDN_FIND_NEXT,
	IDN_PROGRAM0,
	IDN_LOCK0,
	IDN_LOCK,
	IDN_OFF0,
	IDN_OFF,
	IDN_EDIT_NAME0,
	IDN_EDIT_NAME,
	IDN_DEF_NAME0,
	IDN_DEF_NAME,
	IDN_EDIT_ICON0,
	IDN_EDIT_ICON,
	IDN_DEF_ICON0,
	IDN_DEF_ICON,
	IDN_REF_DEF0,
	IDN_REF_DEF,
	IDN_REF_PREV0,
	IDN_REF_PREV,
	IDN_REF_NEXT0,
	IDN_REF_NEXT,
	IDN_OBRAZEK0,
	IDN_DIMEN0,
	IDN_DIMEN,
	IDN_RASTR0,
	IDN_RASTR,
	IDN_EDITBLOK0,
	IDN_EDITBLOK,
	IDN_OTISK0,
	IDN_OTISK,
	IDN_YFLIP0,
	IDN_YFLIP,
	IDN_XFLIP0,
	IDN_XFLIP,
	IDN_LROT0,
	IDN_LROT,
	IDN_RROT0,
	IDN_RROT,
	IDN_MUL20,
	IDN_MUL2,
	IDN_DIV20,
	IDN_DIV2,
	IDN_MULDIV0,
	IDN_MULDIV,
	IDN_XCOL0,
	IDN_XCOL,
	IDN_SCOL0,
	IDN_SCOL,
	IDN_NASTROJE0,
	IDN_PEN0,
	IDN_PEN,
	IDN_LINE0,
	IDN_LINE,
	IDN_BOX0,
	IDN_BOX,
	IDN_FILLBOX0,
	IDN_FILLBOX,
	IDN_CIRCLE0,
	IDN_CIRCLE,
	IDN_FILLCIRCLE0,
	IDN_FILLCIRCLE,
	IDN_ELLIPSE0,
	IDN_ELLIPSE,
	IDN_FILLELLIPSE0,
	IDN_FILLELLIPSE,
	IDN_SPHERE0,
	IDN_SPHERE,
	IDN_SELECT0,
	IDN_SELECT,
	IDN_SPRAY0,
	IDN_SPRAY,
	IDN_PIPE0,
	IDN_PIPE,
	IDN_FILL0,
	IDN_FILL,
	IDN_TEXT0,
	IDN_TEXT,
	IDN_TLOUSTKA0,
	IDN_WIDTH10,
	IDN_WIDTH1,
	IDN_WIDTH20,
	IDN_WIDTH2,
	IDN_WIDTH30,
	IDN_WIDTH3,
	IDN_WIDTH40,
	IDN_WIDTH4,
	IDN_WIDTH50,
	IDN_WIDTH5,
	IDN_PLOCHA0,
	IDN_EDITMAP0,
	IDN_EDITMAP,
	IDN_FILLMAP0,
	IDN_FILLMAP,
	IDN_MAP0,
	IDN_MAP,
	IDN_SWC10,
	IDN_SWC1,
	IDN_SWC20,
	IDN_SWC2,
	IDN_SWC30,
	IDN_SWC3,
	IDN_SWC40,
	IDN_SWC4,
	IDN_SWC50,
	IDN_SWC5,
	IDN_SWCN10,
	IDN_SWCN1,
	IDN_SWCN20,
	IDN_SWCN2,
	IDN_SWCN30,
	IDN_SWCN3,
	IDN_SPRAJT0,
	IDN_SPRITEPAR0,
	IDN_SPRITEPAR,
	IDN_TEST0,
	IDN_TEST,
	IDN_SOUND0,
	IDN_PLAY0,
	IDN_PLAY,
	IDN_PAUSE0,
	IDN_PAUSE,
	IDN_STOP0,
	IDN_STOP,
	IDN_LOOP0,
	IDN_LOOP,
	IDN_RECORD0,
	IDN_RECORD,
	IDN_MUSIC0,
	IDN_REW0,
	IDN_REW,
	IDN_FRW0,
	IDN_FRW,
	IDN_ABOUTBOX,
	IDC_VERZE,
	IDC_COPYNAME,
	IDC_FIRMA,
	IDC_POBOX,
	IDC_PSC,
	IDC_ADRMESTO,
	IDC_REPUBLIKA,
	IDC_DEMOTEXT1,
	IDC_DEMOTEXT2,
	IDC_DEMOTEXT3,
	IDC_DEMOTEXT4,
	IDC_DEMOTEXT5,
	IDC_LICTITLE,
	IDC_LICTITLE0,
	IDC_LICTITLE1,
	IDC_LICTITLE2,
	IDC_LICTITLE3,
	IDC_TELEFON,
	IDC_LITETEXT1,
	IDC_LITETEXT2,
	IDC_LITETEXT3,
	IDC_LITETEXT4,
	IDC_LITETEXT5,
	IDN_FIND_DIALOG,
	IDN_FINDTEXT,
	IDN_CANCEL0,
	IDN_MAPDIMEN,
	IDN_SIRKA,
	IDN_VYSKA,
	IDN_OPENERR,
	IDN_OPENERR1,
	IDN_OPENERR2,
	IDN_OPENERR3,
	IDN_READERR,
	IDN_READERR1,
	IDN_READERR2,
	IDN_READERR3,
	IDN_LITEERR,
	IDN_LITEERR1,
	IDN_LITEERR2,
	IDN_LITEERR3,
	IDN_PICBLOK,
	IDN_PREVRACENI,
	IDN_YFLIP_TXT,
	IDN_XFLIP_TXT,
	IDN_OTOCENI,
	IDN_LROT_TXT,
	IDN_RROT_TXT,
	IDN_LROT67_TXT,
	IDN_RROT67_TXT,
	IDN_LROT60_TXT,
	IDN_RROT60_TXT,
	IDN_LROT45_TXT,
	IDN_RROT45_TXT,
	IDN_LROT30_TXT,
	IDN_RROT30_TXT,
	IDN_LROT22_TXT,
	IDN_RROT22_TXT,
	IDN_ZVETSENI,
	IDN_MUL2_TXT,
	IDN_DIV2_TXT,
	IDN_MULDIV_TXT,
	IDN_BARVY,
	IDN_XCOL_TXT,
	IDN_SCOL_TXT,
	IDN_PICDIMEN,
	IDN_ROZMERY,
	IDC_KROK,
	IDC_BOD,
	IDN_PICRESIZE,
	IDN_SOUCASNE_ROZMERY,
	IDN_KROKU,
	IDN_BODU,
	IDN_NOVE_ROZMERY,
	IDN_SIRKA2,
	IDN_VYSKA2,
	IDN_PICRESIZE_STEP,
	IDN_PICRESIZE_POINT,
	IDN_PICRESIZE_PROC,
	IDN_PICRESIZE_PROP,
	IDN_PICRESIZE_INT,
	IDN_PICRESIZE_DITH,
	IDN_SELECT_COLOR,
	IDN_SELECT_CREATE,
	IDN_SELECT_CREATE1,
	IDN_JINE_JMENO,
	IDN_SELECT_DEL,
	IDN_SELECT_DEL1,
	IDN_SELECT_DEL2,
	IDN_SELECT_DELDIR,
	IDN_SELECT_DELDIR1,
	IDN_SELECT_DELDIRERROR,
	IDN_SELECT_DELDIRERROR1,
	IDN_SELECT_DELERROR,
	IDN_SELECT_DELERROR1,
	IDN_SELECT_DIR,
	IDN_SELECT_DIR1,
	IDN_SELECT_DIRCREATE,
	IDN_SELECT_DIRCREATE1,
	IDN_SELECT_DIREXIST,
	IDN_SELECT_DIREXIST1,
	IDN_SELECT_EXIST,
	IDN_SELECT_EXIST1,
	IDN_SELECT_NEW,
	IDN_SELECT_NEW1,
	IDN_SELECT_NEWCOPY,
	IDN_SELECT_NEWCOPY1,
	IDN_SPRITEDIMEN,
	IDN_ANIMACE,
	IDN_PRODLEVA_MEZI_FAZEMI,
	IDN_MILISEKUND,
	IDN_FAZI_NA_KROK,
	IDN_HLADINA_K_ZOBRAZENI,
	IDN_POCET_OBRAZKU,
	IDN_FAZI_PRO_KLID,
	IDN_FAZI_PRO_POHYB,
	IDN_SMERU,
	IDN_ROZMER_OBRAZKU,
	IDN_SIRKA_OBRAZKU,
	IDN_VYSKA_OBRAZKU,
	IDN_SPRITETEST,
	IDN_WRITEERR,
	IDN_WRITEERR1,
	IDN_WRITEERR2,
	IDN_WRITEERR3,
	IDN_WRITEERR4,
	IDN_WRITEERR5,
	IDN_FILEMODICAP,
	IDN_FILEMODI,
	IDN_TESTREFER2,
	IDN_TESTREFER,
	IDN_CLASSDEL2,
	IDN_CLASSDEL,
// [3] Dialog chybového hlášení o nedostatku pamìti
	IDN_MEMERR_NADPIS,
	IDN_MEMERR_TEXT,
	IDN_LOADER_MEMERR_TEXT,
	IDN_LICERR2,
	IDN_LICERR,
	IDN_LICTIMEERR2,
	IDN_LICTIMEERR,
	IDN_LICTIME1,
	IDN_LICTIME2,
	IDN_RESOURCE_ERR1,
	IDN_RESOURCE_ERR2,
	IDN_DAMAGE_ERR1,
	IDN_DAMAGE_ERR2,
	IDN_SAVE_AS1,
	IDN_PROGRAMY,
	IDN_VSECHNY_SOUBORY,
	IDN_SAVE_DISTR1,
	IDN_SAVE_DISTR2,
	IDN_SAVE_DISTR3,
// [3] Dialog interpreteru hlášení nespustitelného programu
	IDN_NESPUSTITELNY,
	IDN_NESPUSTITELNY2,
// [3] Dialog nastavení písma oken stromù
	IDN_SETUP,
	IDN_SETUP_VZOR,
	IDN_SETUP_VZOR2,
	IDN_SETUP_STD,
	IDN_SETUP_BOLD,
	IDN_SETUP_ITALIC,
	IDN_SETUP_UNDERLINE,
	IDN_SETUP_STRIKEOUT,
	IDN_SETUP_NORMAL,
	IDN_SETUP_SERIF,
	IDN_SETUP_COURIER,
	IDN_SETUP_USER,
	IDN_SETUP_TYP,
	IDN_SETUP_HEIGHT,
	IDN_SETUP_WIDTH,
	IDN_SETUP_RAM1,
	IDN_SETUP_RAM2,
	IDN_EXPORT2,
	IDN_IMPORT2,
	IDN_EXPIMP,
};

#endif // _DEBUG

CText ProductKey0(_T("Software\\Gemtree Software\\Peter"));

/////////////////////////////////////////////////////////////////////////////
// naètení èíselného registru (0=nenalezen)

int GetRegNum(CText key, CText name)
{
// výsledná hodnota
	DWORD num = 0;

// otevøení klíèe
	HKEY hkey;
	if (::RegOpenKeyEx(
		HKEY_LOCAL_MACHINE,
		key,
		0,
		KEY_QUERY_VALUE,
		&hkey) == ERROR_SUCCESS)
	{

// naètení hodnoty klíèe
		DWORD type = REG_DWORD;
		DWORD bufsize = 4;

		::RegQueryValueEx(
			hkey,
			name,
			0,
			&type,
			(BYTE*)&num,
			&bufsize);

// uzavøení klíèe
		::RegCloseKey(hkey);
	}

	return num;
}


/////////////////////////////////////////////////////////////////////////////
// nastavení èíselného registru

void SetRegNum(CText key, CText name, DWORD num)
{
// otevøení klíèe
	HKEY hkey;
	DWORD disp;

	if (::RegCreateKeyEx(
		HKEY_LOCAL_MACHINE,
		key,
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_WRITE,
		NULL,
		&hkey,
		&disp) == ERROR_SUCCESS)
	{

// nastavení hodnoty registru
		::RegSetValueEx(
			hkey,
			name,
			0,
			REG_DWORD,
			(const BYTE*)&num,
			4);

// uzavøení klíèe
		::RegCloseKey(hkey);
	}
}
			 

//////////////////////////////////////////////////////////////////////////////
// nastavení identifikátoru jazyku do registrù Windows (0=implicitní)

void SetJazykReg(int langID)
{
// korekce na implicitní jazyk
	if (langID == (::GetUserDefaultLangID() & 0xff)) langID = 0;

// zjištìní aktuální hodnoty pøepínaèe
	int oldID = GetRegNum(ProductKey0, _T("Language"));

// nastavení nové hodnoty
	if (oldID != langID)
	{
		SetRegNum(ProductKey0, _T("Language"), langID);
	}
}

//////////////////////////////////////////////////////////////////////////////
// zmìna jazyku

void SetJazyk(int jazyk)
{
// nastavení nového jazyka
	JazykUser = jazyk;

// automatický jazyk
	if ((DWORD)jazyk >= (DWORD)JAZYKNUM) jazyk = JazykDef;

	int oldjazyk = Jazyk;
	Jazyk = jazyk;	

// úschova pøedcházejícího jazyku
	if (jazyk != oldjazyk) JazykPrev = oldjazyk;

// nastavení kódu textù a kódu jazyku
	LangID = JazykInfo[Jazyk].LangID;

// desetinná teèka/èárka (teèku pouzívá pouze USA)
	if (Jazyk == JAZYKENG)
	{
		Carka = '.';
	}
	else
	{
		Carka = ',';
	}

// nastavení kódu jazyku do registrù Windows
	SetJazykReg(LangID);

// pøíprava znakové sady (používá se i pro interpreter)
	CharSet = JazykInfo[Jazyk].CharSet;
	CodePage = JazykInfo[Jazyk].CodePage;

	if (Jazyk == JAZYKENG)
	{
		CharSet = DefCharSet;
		CodePage = DefCodePage;
	}

// pøíprava bìžných fontù
	CFont fnt;

	HFONT oldfontbezny = FontBezny;
	FontBezny = GetFont(&fnt);

	HFONT oldfontbold = FontBold;
	fnt.Bold = TRUE;
	FontBold = GetFont(&fnt);

	HFONT oldfontfixedbold = FontFixedBold;
	fnt.Fixed = TRUE;
	FontFixedBold = GetFont(&fnt);

	HFONT oldfontfixed = FontFixed;
	fnt.Bold = FALSE;
	FontFixed = GetFont(&fnt);

// pøíprava menu a textù
	MenuAkt = NULL;

	if (MainFrame != NULL)
	{

// nastavení smìru textu zprava doleva   JAKJAK
           SetRightToLeft(JazykInfo[Jazyk].RightToLeft);

// nastavení nového menu a fontù
		if (ProgMode)
		{
			ProgSetMenu();
		}
		else
		{
			SelectSetMenu();
			if ((oldjazyk != Jazyk) && (SelectItemNum > 0))
			{
				SelectLoad();
			}
		}

// aktualizace textù stromù
		if (oldjazyk != Jazyk)
		{
			ProgReloadJazyk();
		}

// aktualizace nadpisu okna
		MainFrameNameInit();
		MainFrameText.Empty();
		ZobrazJmeno();
	}

// inicializace textù pro hlášení chyby pamìti
	MemErrNadpis.Load(IDN_MEMERR_NADPIS);
	MemErrNadpisP = MemErrNadpis;
	MemErrText.Load(IDN_MEMERR_TEXT);
	MemErrTextP = MemErrText;

// aktualizace textu pøepínaèe logické promìnné
	if (EditLogWnd != NULL)
	{
		CText txt;
		txt.Load(IDN_ZAPNUTO);
		txt.SetWindowText(EditLogWnd);
	}

// aktualizace editoru textu
	EditText::StartReInit();
	EditText::UpdateClipboard();

// vzor textu písma
	if ((oldjazyk != Jazyk) || SetupVzor.IsEmpty())
	{
		SetupVzor.Load(IDN_SETUP_VZOR2);
	}

// nastavení fontu stavové lišty
	SetStatusFont();

// zrušení pùvodních bìžných fontù
	DelFont(oldfontbezny);
	DelFont(oldfontbold);
	DelFont(oldfontfixed);
	DelFont(oldfontfixedbold);

/* JAKJAKJAK 051222

// nastavení klávesnice
	CText keyid;
	int langid = LangID;
	keyid.Hex(langid);

	while (::LoadKeyboardLayout(keyid, KLF_ACTIVATE | KLF_SUBSTITUTE_OK) == NULL)
	{
		langid += 0x400;
		if (langid >= 0x1c00) break;
		keyid.Hex(langid);
	}

*/

}

//////////////////////////////////////////////////////////////////////////////
// nastavení fontù v oknì

void SetFontBezny(HWND wnd)
{
	if ((wnd != NULL) && (FontBezny != NULL))
	{
		::SendMessage(wnd, WM_SETFONT, (WPARAM)FontBezny, MAKELPARAM(TRUE, 0));
	}
}

void SetFontBold(HWND wnd)
{
	if ((wnd != NULL) && (FontBold != NULL))
	{
		::SendMessage(wnd, WM_SETFONT, (WPARAM)FontBold, MAKELPARAM(TRUE, 0));
	}
}

void SetFontFixed(HWND wnd)
{
	if ((wnd != NULL) && (FontFixed != NULL))
	{
		::SendMessage(wnd, WM_SETFONT, (WPARAM)FontFixed, MAKELPARAM(TRUE, 0));
	}
}

void SetFontFixedBold(HWND wnd)
{
	if ((wnd != NULL) && (FontFixedBold != NULL))
	{
		::SendMessage(wnd, WM_SETFONT, (WPARAM)FontFixedBold, MAKELPARAM(TRUE, 0));
	}
}

/*=========================================================================*\
+																			+
+							Hlavní start programu							+
+																			+
\*=========================================================================*/

//////////////////////////////////////////////////////////////////////////////
// inicializace úseku inicializaèních/terminaèních funkcí

void InitTerm(_PVFV* beg, _PVFV* end)
{
	for (; beg < end; beg++)
	{
		if ((*beg != NULL) && (*beg != (PFV)-1))
		{
			(**beg)();
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
// hlavní start programu

void WinMainCRTStartup()
{

// kontrola konstant pøekladaèe
#ifndef _OPTIM
#pragma warning ( disable: 4127)				// hlášení - konstantní podmínka
	ASSERT(sizeof(CIcon) >= sizeof(int));		// CBufIcon
	ASSERT(sizeof(CMap) >= sizeof(int));		// CBufMap
	ASSERT(sizeof(CMusic) >= sizeof(int));		// CBufMusic
	ASSERT(sizeof(CPicture) >= sizeof(int));	// CBufPic
	ASSERT(sizeof(PROGITEM) == SIZEOFPROGITEM);	// CBufProg
	ASSERT(SIZEOFPROGITEM == 64);				// CBufProg
	ASSERT(sizeof(double) >= sizeof(int));		// CBufReal
	ASSERT(sizeof(CSound) >= sizeof(int));		// CBufSound
	ASSERT(sizeof(CSprite) >= sizeof(int));		// CBufSprite
	ASSERT(sizeof(CText) >= sizeof(int));		// CBufText
	ASSERT(LF_FACESIZE == 32);					// EditIcon.cpp / SetMode()
	ASSERT(2*ColLev <= 20);						// EditIcon.cpp / SetKoule()
	ASSERT(sizeof(CPicture) == sizeof(PICTUREDATA*)); // EditIcon.cpp / LimitUndoSize()
	ASSERT(StdColors < (256-20));				// Main.cpp
	ASSERT(BufIntID == 0);						// ProgFile.cpp / InitImportTab()
#pragma warning ( default: 4127)				// hlášení - konstantní podmínka
#endif

// úschova adresy pøíkazového øádku
	CommandLine = ::GetCommandLine();

// úschova verze systému
	VerzeOS = (int)::GetVersion();
	OSVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx(&OSVersionInfo);

// handle instance programu
	hInstance = ::GetModuleHandle(NULL);

// nastavení chybového módu
	::SetErrorMode(::SetErrorMode(0) | SEM_FAILCRITICALERRORS | SEM_NOOPENFILEERRORBOX); 

// zjištìní implicitního jazyku
	int loc = (::GetUserDefaultLangID() & 0xff);
	JazykDef = JAZYKENG;

	for (int ll = 0; ll < JAZYKNUM; ll++)
	{
		if (JazykInfo[ll].LangID == loc)
		{
			JazykDef = ll;
			break;
		}
	}

// detekce implicitní znakové sady
	DefCodePage = ::GetACP();

	switch (DefCodePage)
	{
	case 1250:
		DefCharSet = EASTEUROPE_CHARSET;
		break;

	case 1251:
		DefCharSet = RUSSIAN_CHARSET;
		break;

	case 1252:
		DefCharSet = ANSI_CHARSET;
		break;

	case 1253:
		DefCharSet = GREEK_CHARSET;
		break;

	case 1254:
		DefCharSet = TURKISH_CHARSET;
		break;

	case 1255:
		CharSet = HEBREW_CHARSET;
		break;

	case 1256:
		DefCharSet = ARABIC_CHARSET;
		break;

	case 1257:
		DefCharSet = BALTIC_CHARSET;
		break;

	case 1258:
		CharSet = VIETNAMESE_CHARSET;
		break;

	default:
		DefCharSet = DEFAULT_CHARSET;
	}

// úschova pøedešlého aktivního okna
	PrevWindow = ::GetForegroundWindow();

// inicializace informací o displeji
	ScreenWidth = ::GetSystemMetrics(SM_CXFULLSCREEN);
	ScreenHeight = ::GetSystemMetrics(SM_CYFULLSCREEN);

// inicializace uzamykání ukonèení programu
#ifdef _MT
	::InitializeCriticalSection(&ExitCriticalSection);
#endif

// inicializace správce pamìti
	if (!MemInit()) 
	{
MEMERROR:
		::MessageBox(MainFrame, MemErrTextP0, MemErrNadpisP0, MB_OK | MB_ICONSTOP);
		Exit(EXITCODE_MEMERRINI);
	}

// pøíprava èasovaèe
	if (VerzeOS > 0) 
	{
		TimerConst = 60;			// èasovaè pro NT
	}
	else
	{
		TimerConst = 30;			// èasovaè pro 95
	}

// pøíprava standardních fontù
	FontDefault = (HFONT)::GetStockObject(ANSI_VAR_FONT);
	FontDefaultBold = (HFONT)::GetStockObject(SYSTEM_FONT);
	FontDefaultFixed = (HFONT)::GetStockObject(ANSI_FIXED_FONT);
	FontDefaultBoldFixed = (HFONT)::GetStockObject(SYSTEM_FIXED_FONT);

	if (FontDefault == NULL) FontDefault = FontDefaultBold;
	if (FontDefaultFixed == NULL) FontDefaultFixed = FontDefaultBold;
	if (FontDefaultBoldFixed == NULL) FontDefaultBoldFixed = FontDefaultBold;

// inicializace funkce GetDiskFreeSpaceEx pro OSR2
	HINSTANCE hinstkern = ::LoadLibrary(_T("KERNEL32.DLL"));

	if (((OSVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) &&
		(LOWORD(OSVersionInfo.dwBuildNumber) > 1000)) ||
		(OSVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT))
	{
		if (hinstkern)
		{
#ifdef _UNICODE
			pGetDiskFreeSpaceEx = (GETDISKFREESPACEEX)
				::GetProcAddress(hinstkern,	"GetDiskFreeSpaceExW");
#else
			pGetDiskFreeSpaceEx = (GETDISKFREESPACEEX)
				::GetProcAddress(hinstkern,	"GetDiskFreeSpaceExA");
#endif
		}
	}

// inicializace generátoru náhory
	SYSTEMTIME systime;
	::GetLocalTime(&systime);
	unsigned time = systime.wMilliseconds + 137*systime.wSecond + 2567*systime.wMinute + 358946*systime.wDay;
	RandomSeed = time + 486874*time;	// inicializace generátoru náhody

// inicializace koprocesoru
#ifdef _M_IX86
	WORD stat;
	_asm {
		wait						// synchronizace
		fnstcw		stat			// uložení øídicího slova
		wait						// synchronizace
		mov			ax,stat			// stav øídicího slova
		and			ah,not 0xc		// implicitní zaokrouhlování
		or			ah,3			// pøesnost 64 bitù
		mov			stat,ax			// nový stav øídicího slova
		fldcw		stat			// nastavení nového øídicího slova
	}
#endif

// inicializace obsluhy ikon
	if (!InitIcon()) goto MEMERROR;

// inicializace obsluhy obrázkù
	if (!InitPicture()) goto MEMERROR;

// inicializace obsluhy sprajtù
	if (!InitSprite()) goto MEMERROR;

// inicializace globálních objektù
	InitTerm(__xc_a, __xc_z);

// inicializace textù pro hlášení chyby pamìti
	MemErrNadpis.Load(IDN_MEMERR_NADPIS);
	MemErrNadpisP = MemErrNadpis;
	MemErrText.Load(IDN_MEMERR_TEXT);
	MemErrTextP = MemErrText;

// naètení jména programu
	TextExe = _T(".exe");
	TextExe2 = _T(".EXE");
	TCHAR buf[_MAX_PATH];
	buf[0] = 0;
	::GetModuleFileName(hInstance, buf, _MAX_PATH);
	ExeFileName = buf;

// aktuální adresáø uživatele
	AktDir.GetAktDir();
	if (AktDir.LastChar() != _T('\\'))
	{
		AktDir += _T('\\');
	}

// domovský adresáø programu
	HomeDir = ExeFileName.Left(ExeFileName.RevFind(_T('\\'))+1);
	if (HomeDir.IsEmpty()) HomeDir = AktDir;
	if (HomeDir.LastChar() != _T('\\'))
	{
		HomeDir += _T('\\');
	}

// pøíprava jména souboru nápovìdy
//	HelpFileName = HomeDir + _T("Peter_en.hlp");

	IniFileName = AktDir + _T("Peter.ini");

// inicializace standardního záhlaví BMP
	StdBitmapInfo = (BITMAPINFO*) MemGet(sizeof(BITMAPINFO) + sizeof(RGBQUAD)*255);
	if (StdBitmapInfo == NULL) goto MEMERROR;
	MemFill(StdBitmapInfo, sizeof(BITMAPINFO) + sizeof(RGBQUAD)*255, 0);	// vynulování
	StdBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);	// velikost záhlaví
	StdBitmapInfo->bmiHeader.biWidth = ICONWIDTH;	// šíøka bitmapy
	StdBitmapInfo->bmiHeader.biHeight = ICONHEIGHT;	// výška bitmapy
	StdBitmapInfo->bmiHeader.biPlanes = 1;			// poèet barevných rovin
	StdBitmapInfo->bmiHeader.biBitCount = 8;		// poèet bitù na bod
	StdBitmapInfo->bmiHeader.biCompression = BI_RGB; // není komprese
	StdBitmapInfo->bmiHeader.biClrImportant = StdColors; // poèet dùležitých palet

// vytvoøení standardních palet
	RGBQUAD* rgb = StdBitmapInfo->bmiColors;		// zaèátek tabulky palet

// nemìnit paletu barvy pozadí - používá se pøi importu obrázkù
	rgb->rgbRed =	BACKCOLOR_RED;					// prùhledná barva
	rgb->rgbGreen =	BACKCOLOR_GREEN;
	rgb->rgbBlue =	BACKCOLOR_BLUE;
	rgb++;

	rgb->rgbRed =	SHADCOLOR_RED;					// barva stínu
	rgb->rgbGreen =	SHADCOLOR_GREEN;
	rgb->rgbBlue =	SHADCOLOR_BLUE;
	rgb++;

	int i,j,k;
	for (i = 0; i < (ColCol-2); i++)				// pro všechny barvy bez bílé a šedé
	{
		*rgb = ColColTab[i];						// pøenesení základní barvy

		for (j = 1; j < ColLev; j++)				// pro všechny odstíny
		{
			k = ColLevTab[j];						// násobící koeficient
			rgb[j].rgbRed = (BYTE)(rgb->rgbRed*k/256);		// èervená
			rgb[j].rgbGreen = (BYTE)(rgb->rgbGreen*k/256);	// zelená
			rgb[j].rgbBlue = (BYTE)(rgb->rgbBlue*k/256);	// modrá
		}
		rgb += ColLev;								// adresa další barvy
	}

	i = WhiteCol;									// index bílé barvy
	for (; i <= BlackCol; i++)						// od bílé barvy až po èernou
	{
		k = (BlackCol-i)*255/(2*ColLev-1);			// odstín bílé barvy
		rgb->rgbRed = (BYTE)k;						// èervená
		rgb->rgbGreen = (BYTE)k;					// zelená
		rgb->rgbBlue = (BYTE)k;						// modrá
		rgb++;										// zvýšení adresy barvy
	}

// vygenerování tabulky pro import palet a tabulky palet (pro konverzi BMP)
	StdPalImport = (BYTE*)MemGet(64 * 64 * 64);		// tabulka pro import palet
	if (StdPalImport == NULL) goto MEMERROR;

#ifndef _OPTIM
#ifdef _DEBUG
	if ((int)::GetFileAttributes(_T("PALIMP.DAT")) == -1)
	{
		if (::MessageBox(NULL, _T("Bude generována tabulka importu barev.\nOperace potrvá nìkolik minut.\nChcete pokraèovat?"),
			_T("Tabulka importu barev"), MB_YESNO | MB_ICONWARNING) == IDYES)
		{
			GenPalImport();

			::DeleteFile(_T("PALIMP0.DAT"));
			::DeleteFile(_T("PALIMP.DAT"));

			HANDLE file = ::CreateFile(_T("PALIMP0.DAT"), GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL, NULL);
		
			ASSERT(file != INVALID_HANDLE_VALUE);

			DWORD writen;
			::WriteFile(file, StdPalImport, 64*64*64, &writen, NULL);
			::CloseHandle(file);

			Exec(_T("HLPGEN.EXE PALIMP0.DAT PALIMP.DAT"), EmptyText, TRUE);

			::DeleteFile(_T("PALIMP0.DAT"));
		}
	}

	if ((int)::GetFileAttributes(_T("PALETY.BMP")) == -1)
	{
		BYTE* buf = (BYTE*)MemGet(2102);
		MemFill(buf, 2102, 0);

		BYTE* dst = buf + 14 + sizeof(BITMAPINFO) + sizeof(RGBQUAD)*255;
		for (i = 0; i < ColCol*ColLev; i++)
		{
			*dst = (BYTE)i; dst++;
			*dst = (BYTE)i; dst++;
			*dst = (BYTE)i; dst++;
			*dst = (BYTE)i; dst++;
		}

		::DeleteFile(_T("PALETY.BMP"));
		HANDLE file2 = ::CreateFile(_T("PALETY.BMP"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL);
		DWORD writen2;
		buf[0]=0x42;buf[1]=0x4d;buf[2]=0x36;buf[3]=8;buf[10]=0x36;buf[11]=4;
		MemCopy(buf+14, StdBitmapInfo, sizeof(BITMAPINFO) + sizeof(RGBQUAD)*255);
		::WriteFile(file2, buf, 2102, &writen2, NULL);
		::CloseHandle(file2);

		MemFree(buf);
	}
#endif // _DEBUG
#endif // _OPTIM

// promìnné pro palety (pøeddeklarace kvùli skoku)
	LOGPALETTE* pStdPalette = NULL;
	PALETTEENTRY* pal = NULL;
#ifdef _DEBUG
	int cc, cc0, l;
#endif // _DEBUG

// naètení tabulky importu palet
	HRSRC hRes = ::FindResource(hInstance, MAKEINTRESOURCE(IDN_PALIMP), _T("LOADER"));
	HGLOBAL hData = ::LoadResource(hInstance, hRes);
	ASSERT((hRes != NULL) && (hData != NULL));
	if ((hRes == NULL) || (hData == NULL)) goto RESERROR;
	DeKomp(StdPalImport, 64*64*64, (BYTE*)::LockResource(hData)+6, ::SizeofResource(hInstance, hRes)-6);

// vytvoøení konverzní tabulky palet pro ímport souborù BMP
	KonvPal = (BYTE*)MemGet(256);			// konverzní tabulka palet
	if (KonvPal == NULL) goto MEMERROR;

// vytvoøení vlastních logických palet
	pStdPalette = (LOGPALETTE*) MemGet(sizeof(PALETTEENTRY)*(StdColors-1)+sizeof(LOGPALETTE));
	if (pStdPalette == NULL) goto MEMERROR;
	pStdPalette->palVersion = 0x300;
	pStdPalette->palNumEntries = StdColors;
	pal = pStdPalette->palPalEntry;
	rgb = StdBitmapInfo->bmiColors;
	for (i = StdColors; i > 0; i--)
	{
		pal->peRed = rgb->rgbRed;
		pal->peGreen = rgb->rgbGreen; 
		pal->peBlue = rgb->rgbBlue;
		pal->peFlags = 0;
		pal++;
		rgb++;
	}
	StdPalette = ::CreatePalette(pStdPalette);
	ASSERT(StdPalette != NULL);
	MemFree(pStdPalette);

// inicializace pùltónového štìtce
	InitHalfTone();

// naètení konfigurace
	ReadIni();

// inicializace cest do adresáøù
	InitPaths();

// provìøení tabulky textových konstant
#ifdef _DEBUG

#pragma warning ( disable: 4127)				// hlášení - konstantní podmínka
	ASSERT(TextConstNum == sizeof(TextConstAra)/sizeof(TextConstAra[0]));
	ASSERT(TextConstNum == sizeof(TextConstBul)/sizeof(TextConstBul[0]));
	ASSERT(TextConstNum == sizeof(TextConstCz)/sizeof(TextConstCz[0]));
	ASSERT(TextConstNum == sizeof(TextConstDan)/sizeof(TextConstDan[0]));
	ASSERT(TextConstNum == sizeof(TextConstGer)/sizeof(TextConstGer[0]));
	ASSERT(TextConstNum == sizeof(TextConstRec)/sizeof(TextConstRec[0]));
	ASSERT(TextConstNum == sizeof(TextConstEng)/sizeof(TextConstEng[0]));
	ASSERT(TextConstNum == sizeof(TextConstSpa)/sizeof(TextConstSpa[0]));
	ASSERT(TextConstNum == sizeof(TextConstFin)/sizeof(TextConstFin[0]));
	ASSERT(TextConstNum == sizeof(TextConstFra)/sizeof(TextConstFra[0]));
	ASSERT(TextConstNum == sizeof(TextConstHeb)/sizeof(TextConstHeb[0]));
	ASSERT(TextConstNum == sizeof(TextConstMad)/sizeof(TextConstMad[0]));
	ASSERT(TextConstNum == sizeof(TextConstIsl)/sizeof(TextConstIsl[0]));
	ASSERT(TextConstNum == sizeof(TextConstIta)/sizeof(TextConstIta[0]));
	ASSERT(TextConstNum == sizeof(TextConstHol)/sizeof(TextConstHol[0]));
	ASSERT(TextConstNum == sizeof(TextConstNor)/sizeof(TextConstNor[0]));
	ASSERT(TextConstNum == sizeof(TextConstPol)/sizeof(TextConstPol[0]));
	ASSERT(TextConstNum == sizeof(TextConstPor)/sizeof(TextConstPor[0]));
	ASSERT(TextConstNum == sizeof(TextConstRum)/sizeof(TextConstRum[0]));
	ASSERT(TextConstNum == sizeof(TextConstRus)/sizeof(TextConstRus[0]));
	ASSERT(TextConstNum == sizeof(TextConstSrb)/sizeof(TextConstSrb[0]));
	ASSERT(TextConstNum == sizeof(TextConstSlo)/sizeof(TextConstSlo[0]));
	ASSERT(TextConstNum == sizeof(TextConstAlb)/sizeof(TextConstAlb[0]));
	ASSERT(TextConstNum == sizeof(TextConstSwe)/sizeof(TextConstSwe[0]));
	ASSERT(TextConstNum == sizeof(TextConstTur)/sizeof(TextConstTur[0]));
	ASSERT(TextConstNum == sizeof(TextConstVie)/sizeof(TextConstVie[0]));
#pragma warning ( default: 4127)				// hlášení - konstantní podmínka

	cc0 = sizeof(TestTextTab) / sizeof(TestTextTab[0]);

	for (l = 0; l < JAZYKNUM; l++)
	{
		for (cc = 0; cc < cc0 + FncNum; cc++)
		{
			int ix;
			if (cc < cc0)
			{
				ix = TestTextTab[cc];
			}
			else
			{
				ix = IDF + cc - cc0;
			}

			ASSERT(ix == JazykInfo[l].TextConst[cc].TextID);
		}
	}

#endif // _DEBUG

// aktualizace jazyku
	SetJazyk(JazykUser);

// buffer standardních fontù
	StdFonts.IconSize(16, 16);
	if (!StdFonts.Load(IDN_STDFONTY, 256))
	{
RESERROR:
		CText txt1;
		txt1.Load(IDN_DAMAGE_ERR1);
		CText txt2;
		txt2.Load(IDN_DAMAGE_ERR2);

		::MessageBox(NULL, txt2, txt1, MB_OK | MB_ICONSTOP);
		Exit(EXITCODE_LOADRES);
	}

	if (!StdFontsWidth.Num(256)) goto RESERROR;
	for (i = 0; i < 256; i++)
	{
		int width = 16;
		BYTE* data = StdFonts[i].DataData() + 15;
		for (; width > 0; width--)
		{
			if ((data[ 0*16] != BackCol) ||
				(data[ 1*16] != BackCol) ||
				(data[ 2*16] != BackCol) ||
				(data[ 3*16] != BackCol) ||
				(data[ 4*16] != BackCol) ||
				(data[ 5*16] != BackCol) ||
				(data[ 6*16] != BackCol) ||
				(data[ 7*16] != BackCol) ||
				(data[ 8*16] != BackCol) ||
				(data[ 9*16] != BackCol) ||
				(data[10*16] != BackCol) ||
				(data[11*16] != BackCol) ||
				(data[12*16] != BackCol) ||
				(data[13*16] != BackCol) ||
				(data[14*16] != BackCol) ||
				(data[15*16] != BackCol))
			{
				break;
			}
			data--;
		}
		StdFontsWidth[i] = width;
	}
	StdFontsWidth[32] = 2;

// vytvoøení hlavního okna aplikace
	if (!MainFrameCreate()) 
	{
		CText txt1;
		txt1.Load(IDN_RESOURCE_ERR1);
		CText txt2;
		txt2.Load(IDN_RESOURCE_ERR2);
		::MessageBox(NULL, txt2, txt1, MB_OK | MB_ICONSTOP);
		Exit(EXITCODE_MAINFRAME);
	}

// test, zda nebyla chyba pamìti
	if (MemoryError) goto MEMERROR;

// aktualizace jazyku po vytvoøení hlavního okna
	SetJazyk(JazykUser);

// hlavní obslužná smyèka aplikace
	MSG msg;

	while (::GetMessage(&msg, NULL, 0, 0))
	{
		MemoryError = FALSE;

		if (!PreTranslateMessage(&msg))
		{
			if (::TranslateAccelerator(MainFrame, Accel, &msg) == 0)
			{
				::TranslateMessage(&msg);

//#ifdef _M_IX86
//				__try 
//				{
//					::DispatchMessage(&msg);
//				}
//				__except (EXCEPTION_EXECUTE_HANDLER) 
//				{
//#ifdef _DEBUG
//					_asm
//					{
//						mov		[ExceptCode],eax
//					}
//					ErrText2.Hex(ExceptCode);
//					ErrText = _T("Systém vyvolal chybovou výjimku ");
//					ErrText += ErrText2;
//					::MessageBox(
//						NULL, 
//						ErrText,						
//						_T("Výjimka systému"), 
///						MB_OK | MB_ICONSTOP
//					);
//#endif
//				};

//#else // _M_IX86
				::DispatchMessage(&msg);
//#endif // _M_IX86
			}
		}
	}

// konec programu
	Exit(EXITCODE_OK);
};


//CText regname(_T("Control Panel\\Desktop"));
//CText keyname(_T("SmoothScroll"));

//	int old = GetRegUserDWORD(regname, keyname);
//	if ((old == 0) || (old == 1))
//	{
//		SetRegUserDWORD(regname, keyname, 0);
//	}

//	if ((old == 0) || (old == 1))
//	{
//		SetRegUserDWORD(regname, keyname, old);
//	}



//////////////////////////////////////////////////////////////////////////////
// ukonèení programu (pøi chybì neukládá konfiguraci)

void Exit(int code)
{
// uzamknutí ukonèení programu
#ifdef _MT
	::EnterCriticalSection(&ExitCriticalSection);
#endif

// uložení konfigurace
	if (code < EXITCODE_INIT)
	{
		WriteIni();
	}

// vypnutí kódu jazyku v registrech Windows
	SetJazykReg(0);

// zrušení palet
	::DeleteObject(StdPalette);

// ukonèení správce pamìti
	MemTerm();

// uvolnìní uzamykání ukonèení programu
#ifdef _MT
	::DeleteCriticalSection(&ExitCriticalSection);
#endif

// nastavení fokusu na pøedešlé okno
	::SetForegroundWindow(PrevWindow);

// ukonèení programu
	ExitProcess(code);
}

//////////////////////////////////////////////////////////////////////////////
// chyba pro DEBUG OPTIM mód

#if defined(_OPTIM) && defined(_M_IX86) && defined(_DEBUG)
void debugBreak(char* file, int line, char* date)
{
	CText txt("Interní chyba programu PETER. Nahlaste, prosím, dodavateli následující\n"
			  "údaje a popište situaci, za které tato chyba nastala. Dìkujeme za pochopení.\n"
			   "\n     soubor = ");

	CText txt2(file);
	txt.Add(txt2.Right(txt2.Length() - txt2.RevFind('\\') - 1));
	txt.Add("           øádek = ");
	txt.AddInt(line);
	txt.Add("           verze = ");
	txt.Add(date);
	txt.Add('\n');

	::MessageBox(
		MainFrame,
		txt,
		"Interní chyba",
		MB_OK | MB_ICONERROR);
}
#endif

//////////////////////////////////////////////////////////////////////////////
// vystøedìní okna proti jinému oknu

void CenterWindow(HWND child, HWND parent)
{
	RECT rc1, rc2;

	if (parent == NULL)
	{
		::SystemParametersInfo(SPI_GETWORKAREA, NULL, &rc1, NULL);
	}
	else
	{
		::GetWindowRect(parent, &rc1);
	}
	::GetWindowRect(child, &rc2);

	::SetWindowPos(child, NULL, 
		(rc1.left + rc1.right)/2 - (rc2.right - rc2.left)/2,
		(rc1.top + rc1.bottom)/2 - (rc2.bottom - rc2.top)/2, 0, 0,
		SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
}

//////////////////////////////////////////////////////////////////////////////
// spuštìní programu

int Exec(CText command, CText aktdir, BOOL wait)
{
	int result = 0;
	command.TrimLeft();
	if (command.IsEmpty()) return 255;

// korekce aktivního adresáøe
	if (aktdir.Length() > 1)
	{
		if (aktdir.LastChar() == '\\')
		{
			aktdir.Delete(aktdir.Length()-1);
		}
	}

// inicializace startovací tabulky
	STARTUPINFO si;
	MemFill(&si, sizeof(si), 0);
	si.cb = sizeof(si);

// inicializace tabulky procesu
	PROCESS_INFORMATION pi;
	MemFill(&pi, sizeof(pi), 0);

// spuštìní programu
	::CreateProcess(
		NULL,								// jméno programu
		(LPTSTR)(LPCTSTR)command,			// pøíkazový øádek
		NULL,								// ochranné atributy procesu
		NULL,								// ochranné atributy vlákna
		FALSE,								// dìdiènost handlù
#ifdef _UNICODE
		CREATE_UNICODE_ENVIRONMENT			// parametry
#else
		0
#endif
		| CREATE_DEFAULT_ERROR_MODE,
		NULL,								// prostøedí
		aktdir.IsEmpty() ? NULL : (LPCTSTR)aktdir, // výchozí adresáø
		&si,								// adresa STARTUPINFO
		&pi);								// adresa PROCESS_INFORMATION

// èekání na ukonèení programu
	if (wait)
	{
		::WaitForSingleObject(pi.hProcess, INFINITE);	// èekání na ukonèení procesu
		::GetExitCodeProcess(pi.hProcess, (DWORD*)&result);	// zjištìní návratového kódu
	}

// uzavøení handle procesu
	::CloseHandle(pi.hProcess);				// uzavøení handle procesu
	::CloseHandle(pi.hThread);				// uzavøení handle toku

	return result;
}


/////////////////////////////////////////////////////////////////////////////
// vytvoøení fontu

HFONT GetFont(CFont* fnt)
{
// pøíprava znakové sady
	DWORD charset = CharSet;
//	if (charset == DefCharSet) charset = DEFAULT_CHARSET;

// urèené parametry fontu
	BOOL standard = fnt->Standard;
	BOOL bold = fnt->Bold;
	BOOL italic = fnt->Italic;
	BOOL underline = fnt->Underline;
	BOOL strikeout = fnt->Strikeout;
	BOOL serif = fnt->Serif;
	BOOL fixed = fnt->Fixed;

// pøíprava zadané výšky a vypoètené výšky
	int height = fnt->Height;
	if (height < 0) height = -height;
	int height0 = height;
	if (height0 == 0) height0 = 18;

// pøíprava násobícího koeficientu šíøky
	double koef;

	if (fixed)
	{
		if (serif)
		{
			koef = 0.6;
		}
		else
		{
			koef = 0.58;
		}
	}
	else
	{
		if (serif)
		{
			koef = 0.37;
		}
		else
		{
			koef = 0.40;
		}
	}

	if (fixed && italic)
	{
		koef *= 0.9;
	}

	if (bold) 
	{
		if (fixed)
		{
			koef *= 0.9;
		}
		else
		{
			koef *= 1.06;
		}
	}

	if (serif && fixed)
	{
		koef = 0.4;
	}

	koef *= 0.9;

// pøíprava šíøky a vypoètené šíøky
	int width = Round(fabs(fnt->Width) * height0 * koef - 0.1);
	int width0 = width;
	if (width0 == 0) width0 = Round(height0 * koef - 0.1);

// korekce parametrù pro standardní font
	if (standard)
	{
		italic = FALSE;
		underline = FALSE;
		strikeout = FALSE;
		serif = FALSE;

		if (bold)
		{
			if (fixed)
			{
				height = 17;
				width = 9;
			}
			else
			{
				height = 16;
				width = 6;
			}
		}
		else
		{
			if (fixed)
			{
				height = 12;
				width = 9;
			}
			else
			{
				height = 14;
				width = 5;
			}
		}

		height0 = height;
		width0 = width;
	}

// úschova vypoètených parametrù
	fnt->Height0 = height0;
	fnt->Width0 = width0;

// standardní fonty
/*
	if (standard && (charset == DEFAULT_CHARSET))
	{
		if (bold)
		{
			if (fixed)
			{
				return FontDefaultBoldFixed;
			}
			else
			{
				return FontDefaultBold;
			}
		}
		else
		{
			if (fixed)
			{
				return FontDefaultFixed;
			}
			else
			{
				return FontDefault;
			}
		}
	}
*/

// jméno fontu
	CText name;

	if (fixed)
	{
		if (serif)
		{
			name = fnt->User;
		}
		else
		{
			name = _T("Courier New");
		}
	}
	else
	{
		if (serif)
		{
			name = _T("Times New Roman");
		}
		else
		{
			name = _T("Arial");
		}
	}

// vytvoøení fontu
	HFONT font = ::CreateFont(
		height,
		width,
		0,
		0,
		(bold) ? FW_BOLD : FW_NORMAL,
		italic,
		underline,
		strikeout,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, name);

// druhý pokus, není-li pøítomna správná znaková sada
	if (font == NULL)
	{
		font = ::CreateFont(
			height,
			width,
			0,
			0,
			(bold) ? FW_BOLD : FW_NORMAL,
			italic,
			underline,
			strikeout,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, name);
	}

// pøi neúspìchu náhradní font
	if (font == NULL) font = FontDefaultBold;

// návrat fontu
	return font;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení fontu

void DelFont(HFONT font)
{
	if ((font != NULL) &&
		(font != FontDefault) &&
		(font != FontDefaultBold) &&
		(font != FontDefaultFixed) &&
		(font != FontDefaultBoldFixed))
	{
		::DeleteObject(font);
	}
}

/////////////////////////////////////////////////////////////////////////////
// generování konverzní tabulky barev obrázku

void GenKonvPal(BITMAPINFO* bmp)
{
// lokální promìnné
	int			i;									// èítaè barev
	int			palet = bmp->bmiHeader.biClrUsed;	// poèet palet v bitmapì
	RGBQUAD*	col = bmp->bmiColors;				// ukazatel barevných složek
	BYTE*		pal = KonvPal;						// ukazatel konverzních palet
	DWORD		BackColData = *(DWORD*)(StdBitmapInfo->bmiColors + BackCol); // paleta pozadí
	DWORD		ShadColData = *(DWORD*)(StdBitmapInfo->bmiColors + ShadCol); // paleta stínu

// pøíprava poètu palet
	if (palet == 0) palet = (1 << bmp->bmiHeader.biBitCount);
	i = palet;
	if ((palet < 1) || (palet > 256)) return;

// cyklus pøes platné barvy
	for (; i > 0; i--)
	{
		if (*(DWORD*)col == BackColData)
		{
			*pal = BackCol;
		}
		else
		{
			if (*(DWORD*)col == ShadColData)
			{
				*pal = ShadCol;
			}
			else
			{
				*pal = PalImport(col->rgbRed, col->rgbGreen, col->rgbBlue);
			}
		}
		pal++;
		col++;
	}

// vymazání zbylých neplatných barev (import na èernou barvu)
	MemFill(pal, 256-palet, BlackCol);

// test, zda je tabulka standardní
	KonvPalOK = false;
	if ((palet > 100) && (palet <= StdColors))
	{
		KonvPalOK = true;
		for (i = 0; i < palet; i++)
		{
			if (KonvPal[i] != i)
			{
				KonvPalOK = false;
				break;
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// vygenerování støední barvy (ze 4 bodù)

BYTE ColAvrg(BYTE col1, BYTE col2, BYTE col3, BYTE col4)
{
// lokální promìnné
	int			r = 0;									// èervená složka
	int			g = 0;									// zelená složka
	int			b = 0;									// modrá složka
	BYTE		n = 0;									// poèet platných bodù
	RGBQUAD*	rgb;									// ukazatel palet
	int			shad = 0;								// èítaè stínù

// první bod
	if (col1 != BackCol)
	{
		if (col1 == ShadCol)
		{
			shad++;
		}
		else
		{
			n++;											// zvýšení èítaèe bodù
			rgb = StdBitmapInfo->bmiColors + col1;			// definice palet bodu
			r = rgb->rgbRed;								// èervená složka
			g = rgb->rgbGreen;								// zelená složka
			b = rgb->rgbBlue;								// modrá složka
		}
	}

// druhý bod
	if (col2 != BackCol)
	{
		if (col2 == ShadCol)
		{
			shad++;
		}
		else
		{
			n++;											// zvýšení èítaèe bodù
			rgb = StdBitmapInfo->bmiColors + col2;			// definice palet bodu
			r += rgb->rgbRed;								// èervená složka
			g += rgb->rgbGreen;								// zelená složka
			b += rgb->rgbBlue;								// modrá složka
		}
	}

// tøetí bod
	if (col3 != BackCol)
	{
		if (col3 == ShadCol)
		{
			shad++;
		}
		else
		{
			n++;											// zvýšení èítaèe bodù
			rgb = StdBitmapInfo->bmiColors + col3;			// definice palet bodu
			r += rgb->rgbRed;								// èervená složka
			g += rgb->rgbGreen;								// zelená složka
			b += rgb->rgbBlue;								// modrá složka
		}
	}

// ètvrtý bod
	if (col4 != BackCol)
	{
		if (col4 == ShadCol)
		{
			shad++;
		}
		else
		{
			n++;											// zvýšení èítaèe bodù
			rgb = StdBitmapInfo->bmiColors + col4;			// definice palet bodu
			r += rgb->rgbRed;								// èervená složka
			g += rgb->rgbGreen;								// zelená složka
			b += rgb->rgbBlue;								// modrá složka
		}
	}

// bod byl stín
	if (shad > 2)
	{
		return ShadCol;
	}

// bod byl pozadí
	if (n < 2)
	{
		return BackCol;
	}

// jinak vygenerování bodu podle barevných složek
	else
	{
		return PalImport((BYTE)(r/n), (BYTE)(g/n), (BYTE)(b/n));
	}
}

/////////////////////////////////////////////////////////////////////////////
// dekomprese dat (používá se pouze pro dekompresi importu palet PALIMP.DAT)

#define MAXLENX 25					// min. délka dlouhého øetìzce
#define MAXLEN (MAXLENX+254)		// maximální délka øetìzce
#define SUBSTLEN	7				// délka nahrazená dlouhým kódem

void DeKomp(BYTE* dstBuf, int dstNum, BYTE* srcBuf, int srcNum)
{
// naètení jednoho bitu ze stavového slova
#define DekBit		bit = status & 1;				\
					status >>= 1;					\
					if (status == 0)				\
					{								\
						if (srci >= srcNum) break;	\
						status = *(WORD*)src;		\
						src++;						\
						src++;						\
						srci++;						\
						srci++;						\
						bit = status & 1;			\
						status >>= 1;				\
						status |= 0x8000;			\
					}


	BYTE* dst = dstBuf;				// ukazatel cílové adresy
	int dsti = 0;					// èítaè cílových dat
	BYTE* src = srcBuf;				// ukazatel zdrojové adresy
	int srci = 0;					// èítaè zdrojových dat
	BYTE* src2;						// pomocný ukazatel
	int srci2;

	WORD status = 0;				// støadaè stavového slova
	BYTE offsetL, offsetH;			// offset k opakování
	int delka;						// délka k opakování
	int bit;						// 1 = naètený bit

	for (;;)
	{
// naètení prvního bitu pøíznaku
		DekBit

// pøesun bajtu bez komprese
		if (bit == 0)
		{
			if (srci >= srcNum) break;
			if (dsti >= dstNum) break;
			*dst = *src;
			dst++;
			dsti++;
			src++;
			srci++;
		}

// jinak bude opakování øetìzce
		else
		{
			offsetH = 0;
			offsetL = 0;

// první bit délky
			DekBit
			delka = bit;

// zvýšení èítaèe délky
			for (;;)
			{
				delka++;
				delka++;

// naètení pøíznaku konce kódu
				DekBit

				if (bit == 0)
				{
					break;
				}

				if (delka >= (MAXLENX-4+1))
				{
					delka++;
					delka++;
					break;
				}
			}

// korekce pro náhradní kód
			if (delka >= SUBSTLEN)
			{
				if (delka == SUBSTLEN)
				{
					if (srci >= srcNum) break;
					delka = *src;
					src++;
					srci++;
					if (delka == 255) break;
					delka += MAXLENX;
				}
				else
				{
					delka--;
				}
			}

// stanovení offsetu - vyšší bajt
			if (delka != 2)
			{
				DekBit
				if (bit == 0)
				{
					DekBit
					offsetH <<= 1;
					offsetH |= bit;

					DekBit
					offsetH <<= 1;
					offsetH |= bit;

					DekBit
					offsetH <<= 1;
					offsetH |= bit;

					if (offsetH > 1)
					{
						DekBit
						offsetH <<= 1;
						offsetH |= bit;

						offsetH--;
						if (offsetH > 6)
						{
							DekBit
							offsetH <<= 1;
							offsetH |= bit;

							offsetH -= 7;
							if (offsetH > 13)
							{
								DekBit
								offsetH <<= 1;
								offsetH |= bit;

								offsetH -= 14;
							}
						}
					}
					else
					{
						offsetH++;
					}
				}
			}

// naètení offsetu - nižší bajt
			if (srci >= srcNum) break;
			offsetL = *src;
			src++;
			srci++;

// pøenesení øetìzce
			srci2 = dsti - (WORD)(offsetL + offsetH*256);
			if (srci2 < 0) break;
			src2 = &(dstBuf[srci2]);

			for (; delka > 0; delka--)
			{
				if (srci2 >= dstNum) break;
				if (dsti >= dstNum) break;
				*dst = *src2;
				dst++;
				dsti++;
				src2++;
				srci2++;
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// generátor náhody

int rand()
{
	RandomSeed = RandomSeed * 214013 + 2531011;
	return ((RandomSeed >> 16) & 0x7fff);
}

double Rand()
{
	RandomSeed = RandomSeed * 214013 + 2531011;
	return (double)RandomSeed / (double) 4294967296;	// vydìlení rozsahem LONG INT
}


/////////////////////////////////////////////////////////////////////////////
// pøevod reálného èísla na celé èíslo se zaokrouhlením k nejbližší hodnotì

int Round(double num)
{
#ifdef _M_IX86

	DWORD		result;				// výsledek operace

	_asm {
		fld			num				// naètení èísla k provedení operace
		fistp		result			// pøevod èísla na celé èíslo
	}
	return result;

#else

	if (num >= 0)
	{
		return (int)(num + 0.5);
	}
	else
	{
		return (int)(num - 0.5);
	}

#endif
}

/////////////////////////////////////////////////////////////////////////////
// pøevod reálného èísla na celé èíslo s oøíznutím k nule

int Round0(double num)
{
#ifdef _M_IX86

	WORD		oldstat;			// starý stav øídicího slova
	WORD		newstat;			// nový stav øídicího slova
	int			result;				// výsledek operace

	_asm {
		fld			num				// naètení èísla k provedení operace
		wait						// synchronizace
		fnstcw		oldstat			// uloženi øídicího slova
		wait						// synchronizace
		mov			ax,oldstat		// starý stav øídicího slova
		or			ah,0xc			// mód zaokrouhlení smìrem k nule
		mov			newstat,ax		// nový stav øídicího slova
		fldcw		newstat			// nastavení nového øídicího slova
		fistp		result			// pøevod èísla na celé èíslo
		fldcw		oldstat			// navrácení pùvodního øídicího slova
	}
	return result;

#else

	return (int)num;

#endif
}

/////////////////////////////////////////////////////////////////////////////
// pøevod reálného èísla na celé èíslo se zaokrouhlením nahoru (ke kladné hodnotì)

int RoundP(double num)
{
#ifdef _M_IX86

	WORD		oldstat;			// starý stav øídicího slova
	WORD		newstat;			// nový stav øídicího slova
	int			result;				// výsledek operace

	_asm {
		fld			num				// naètení èísla k provedení operace
		wait						// synchronizace
		fnstcw		oldstat			// uloženi øídicího slova
		wait						// synchronizace
		mov			ax,oldstat		// starý stav øídicího slova
		and			ah,not 0xc;		// vynulování øídicích bitù pro zaokrouhlení
		or			ah,8			// mód zaokrouhlení smìrem nahoru
		mov			newstat,ax		// nový stav øídicího slova
		fldcw		newstat			// nastavení nového øídicího slova
		fistp		result			// pøevod èísla na celé èíslo
		fldcw		oldstat			// navrácení pùvodního øídicího slova
	}
	return result;

#else

	if (num >= 0)
	{
		int i = (int)num;
		if ((double)i == num) return i;
		return i+1;
	}
	else
	{
		return (int)num;
	}

#endif
}

/////////////////////////////////////////////////////////////////////////////
// pøevod reálného èísla na celé èíslo se zaokrouhlením dolù (k záporné hodnotì)

int RoundM(double num)
{
#ifdef _M_IX86

	WORD		oldstat;			// starý stav øídicího slova
	WORD		newstat;			// nový stav øídicího slova
	int			result;				// výsledek operace

	_asm {
		fld			num				// naètení èísla k provedení operace
		wait						// synchronizace
		fnstcw		oldstat			// uloženi øídicího slova
		wait						// synchronizace
		mov			ax,oldstat		// starý stav øídicího slova
		and			ah,not 0xc;		// vynulování øídicích bitù pro zaokrouhlení
		or			ah,4			// mód zaokrouhlení smìrem dolù
		mov			newstat,ax		// nový stav øídicího slova
		fldcw		newstat			// nastavení nového øídicího slova
		fistp		result			// pøevod èísla na celé èíslo
		fldcw		oldstat			// navrácení pùvodního øídicího slova
	}
	return result;

#else

	if (num >= 0)
	{
		return (int)num;
	}
	else
	{
		int i = (int)num;
		if ((double)i == num) return i;
		return i-1;
	}

#endif
}


/////////////////////////////////////////////////////////////////////////////
// funkce pro vypnutou optimalizaci

#ifndef _OPTIM
int abs(int num) 
{ 
	if (num < 0) 
	{
		return -num; 
	} 
	else 
	{ 
		return num; 
	} 
};
#endif

/////////////////////////////////////////////////////////////////////////////
// poskytnutí aktivního okna, je-li typu Edit (NULL = není)

HWND GetEditFocus()
{
	HWND wnd = ::GetFocus();
	if (wnd != NULL)
	{
		CText edi(_T("Edit"));
		CText edi2(_T("EDIT"));
		TCHAR buf[21];
		::GetClassName(wnd, buf, 20);
		if ((edi != buf) && (edi2 != buf))
		{
			wnd = NULL;
		}
	}
	return wnd;
}


/////////////////////////////////////////////////////////////////////////////
// vytvoøení adresáøe (pokud existuje, je vše OK)

BOOL CreateDir(CText txt)
{
// oøezání mezer
	txt.TrimLeft();
	txt.TrimRight();

// oøezání koncových "\"
	while (txt.LastChar() == _T('\\'))
	{
		txt.Delete(txt.Length() - 1);
		txt.TrimRight();
	}

// ROOT je OK
	if (txt.IsEmpty()) return TRUE;

// vytvoøení adresáøe
	BOOL res = ::CreateDirectory(txt, NULL);
	if (res) return TRUE;

// pøi neúspìchu kontrola, zda adresáø již existuje
	int attrib = (int)::GetFileAttributes(txt);
	return ((attrib != -1) && (attrib & FILE_ATTRIBUTE_DIRECTORY));
}


/////////////////////////////////////////////////////////////////////////////
// vytvoøení adresáøe (i vícestupòovì - vèetnì cesty)

BOOL CreatePath(CText txt)
{
// první pokus o vytvoøení adresáøe
	if (CreateDir(txt)) return TRUE;

// oøezání mezer
	txt.TrimLeft();
	txt.TrimRight();

// oøezání koncových "\"
	while (txt.LastChar() == _T('\\'))
	{
		txt.Delete(txt.Length() - 1);
		txt.TrimRight();
	}

// adresáø se bude zkracovat
	CText txt2 = txt;
	CText txt3;

// zkracování adresáøe
	int pos;
	while ((pos = txt2.RevFind(_T('\\'))) >= 0)
	{
		txt2.Delete(pos);
		if (CreateDir(txt2)) break;
	}

// prodlužování adresáøe
	while (txt2.Length() < txt.Length())
	{
		txt2.Add(_T('\\'));
		txt3 = txt.Mid(txt2.Length(), txt.Length());
		pos = txt3.Find(_T('\\'));
		if (pos < 0) break;
		txt2.Add(txt3.Left(pos));
		if (!CreateDir(txt2)) return FALSE;
	}

// vytvoøení posledního stupnì (to je poslední pokus)
	return CreateDir(txt);
}


/////////////////////////////////////////////////////////////////////////////
// zjištìní informací o velikosti a volném místu disku

void GetDiskSpace(CText path)
{
// zajištìní \  na konci cesty
	if (path.LastChar() != _T('\\')) path.Add(_T('\\'));

// naètení informací novìjší funkcí
	if (!pGetDiskFreeSpaceEx ||
		!pGetDiskFreeSpaceEx(path, &DiskFreeUser, &DiskSize, &DiskFree))

// naètení informací starší metodou
	{

// nalezení ROOT adresáøe
		int n;
		while ((n = path.RevFind(_T('\\'))) >= 0) path.Delete(n);
		path.Add(_T('\\'));

		DWORD sectc;
		DWORD bytes;
		DWORD freec;
		DWORD sizec;

		if (!::GetDiskFreeSpace(path, &sectc, &bytes, &freec, &sizec) &&
			!::GetDiskFreeSpace(NULL, &sectc, &bytes, &freec, &sizec))
		{
			sectc = 0;
			bytes = 0;
			freec = 0;
			sizec = 0;
		}

		DiskSize = sectc * bytes * sizec;
		DiskFree = sectc * bytes * freec;
		DiskFreeUser = DiskFree;
	}
}

/*
/////////////////////////////////////////////////////////////////////////////
// naètení registru DWORD (-1 = chyba)

int GetRegUserDWORD(CText key, CText name)
{
// buffer výsledné hodnoty
	int num = -1;

// otevøení klíèe
	HKEY hkey;

	if (::RegOpenKeyEx(
		HKEY_CURRENT_USER,
		key,
		0,
		KEY_QUERY_VALUE,
		&hkey) == ERROR_SUCCESS)
	{

// naètení hodnoty klíèe
		DWORD type = REG_DWORD;
		DWORD bufsize = 4;

		if ((::RegQueryValueEx(
			hkey,
			name,
			0,
			&type,
			(BYTE*)&num,
			&bufsize) == ERROR_SUCCESS) &&
			(type == REG_DWORD) &&
			(bufsize == 4))
		{

		}
		else
		{
			num = -1;
		}

// uzavøení klíèe
		::RegCloseKey(hkey);
	}
	return num;
}


/////////////////////////////////////////////////////////////////////////////
// nastavení registru DWORD (nevytváøí klíè)

void SetRegUserDWORD(CText key, CText name, int data)
{
// otevøení klíèe
	HKEY hkey;

	if (::RegOpenKeyEx(
		HKEY_CURRENT_USER,
		key,
		0,
		KEY_SET_VALUE,
		&hkey) == ERROR_SUCCESS)
	{

// nastavení hodnoty registru
		::RegSetValueEx(
			hkey,
			name,
			0,
			REG_DWORD,
			(const BYTE*)&data,
			4);

// uzavøení klíèe
		::RegCloseKey(hkey);
	}
}
*/

