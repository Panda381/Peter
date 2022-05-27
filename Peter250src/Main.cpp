
#include "Main.h"

/***************************************************************************\
*																			*
*							Hlavn� modul aplikace							*
*																			*
\***************************************************************************/

/*=========================================================================*\
+																			+
+						Glob�ln� prom�nn� a konstanty						+
+																			+
\*=========================================================================*/

//////////////////////////////////////////////////////////////////////////////
// inicializa�n� segmenty CRT (konstruktory a destruktory)

typedef void (__cdecl *_PVFV)(void);		// ukazatel na funkci void fnc(void)
typedef void (*PFV)(void);					// funkce void fnc(void)

#pragma data_seg(".CRT$XCA")
_PVFV	__xc_a[] = { NULL };				// konstruktory C++
#pragma data_seg(".CRT$XCZ")
_PVFV	__xc_z[] = { NULL };

#pragma data_seg()							// resetov�n� na b�nou datovou sekci

#pragma comment(linker, "/MERGE:.CRT=.data")	// p�ipojen� CRT sekc� do datov� sekce


//////////////////////////////////////////////////////////////////////////////
// glob�ln� prom�nn�

TCHAR*		CommandLine = NULL;			// p��kazov� ��dek
int			VerzeOS;					// verze syst�mu
OSVERSIONINFO	OSVersionInfo;			// informace o syst�mu
HINSTANCE	hInstance = NULL;			// instance programu
int			ScreenWidth;				// ���ka klientsk� oblasti displeje
int			ScreenHeight;				// v��ka klientsk� oblasti displeje
int			TimerConst;					// konstanta pro �asova� (pro 55 ms)

HACCEL		Accel = NULL;				// akcelera�n� tabulka

BYTE*		StdPalImport;				// tabulka importu palet
BITMAPINFO* StdBitmapInfo;				// standardn� z�hlav� BMP
HPALETTE	StdPalette = NULL;			// vlastn� logick� palety
BYTE*		KonvPal;					// konverzn� tabulka palet
bool		KonvPalOK = false;			// p��znak shodn� konverzn� tabulky (lze prov�st kopii)

HWND		PrevWindow = NULL;			// p�ede�l� aktivn� okno

bool		Dither = true;				// pou��vat dithering

bool		ShortName = false;			// zkracovat jm�na soubor�

//bool		Profi = true;				// profesion�ln� verze programu

__int64	DiskSize = 0;			// velikost disku (z funkce GetDiskSpace)
__int64	DiskFree = 0;			// voln� m�sto disku (z funkce GetDiskSpace)
__int64	DiskFreeUser = 0;		// voln� m�sto u�ivatele (z funkce GetDiskSpace)

GETDISKFREESPACEEX	pGetDiskFreeSpaceEx = NULL;	// funkce GetDiskFreeSpaceEx (NULL=nen�)

CMultiText	Jmeno;						// jm�no editovan�ho souboru (bez cesty)
CText		JmenoLoad;					// pln� jm�no souboru pro �ten� (s cestou - vzorov� adres��)
CText		JmenoSave;					// pln� jm�no souboru pro z�pis
CText		Cesta;						// cesta k editovan�mu souboru (s p��p. "\" na konci)
CText		AktDir;						// aktivn� adres�� u�ivatele (s "\" na konci)
CText		HomeDir;					// cesta do domovsk�ho adres��e s EXE (s "\" na konci)
CText		ExeFileName;				// jm�no tohoto programu
CText		IniFileName;				// jm�no konfigura�n�ho souboru
//CText		HelpFileName;				// jm�no souboru n�pov�dy
CText		TextExe;					// text ".exe"
CText		TextExe2;					// text ".EXE"
CText		CDRom;						// cesta k CD-ROM (pr�zdn� = nen�)

//////////////////////////////////////////////////////////////////////////////
// cesty do adres��� programu (v�echny jsou zakon�eny s "\") - implicitn� aktu�ln� adres��

CText		ProgramPath;				// cesta do adres��e program� "Program" (*.exe)

CText		BackupPath;					// cesta do adres��e z�loh "Backup"

CText		FunctionPath;				// cesta do adres��e funkc� "Function" (*.fnc)
CText		NumberPath;					// cesta do adres��e ��sel "Number" (*.num)
CText		TextPath;					// cesta do adres��e text� "Text" (*.txt)
CText		BoolPath;					// cesta do adres��e logick�ch hodnot "Bool" (*.log)
CText		IconPath;					// cesta do adres��e ikon "Icon" (*.ico)
CText		MapPath;					// cesta do adres��e ploch "Map" (*.map)
CText		PicturePath;				// cesta do adres��e obr�zk� "Picture" (*.bmp)
CText		SpritePath;					// cesta do adres��e sprajt� "Sprite" (*.spr)
CText		SoundPath;					// cesta do adres��e zvuk� "Sound" (*.wav)
CText		MusicPath;					// cesta do adres��e hudby "Music" (*.mid)

// alternativn� knihovny - implicitn� v podadres��i programu Petr
CText		ProgramPath2;				// cesta do adres��e program� "Program" (*.exe)

CText		FunctionPath2;				// cesta do adres��e funkc� "Function" (*.fnc)
CText		NumberPath2;				// cesta do adres��e ��sel "Number" (*.num)
CText		TextPath2;					// cesta do adres��e text� "Text" (*.txt)
CText		BoolPath2;					// cesta do adres��e logick�ch hodnot "Bool" (*.log)
CText		IconPath2;					// cesta do adres��e ikon "Icon" (*.ico)
CText		MapPath2;					// cesta do adres��e ploch "Map" (*.map)
CText		PicturePath2;				// cesta do adres��e obr�zk� "Picture" (*.bmp)
CText		SpritePath2;				// cesta do adres��e sprajt� "Sprite" (*.spr)
CText		SoundPath2;					// cesta do adres��e zvuk� "Sound" (*.wav)
CText		MusicPath2;					// cesta do adres��e hudby "Music" (*.mid)

// alternativn� knihovny - implicitn� v ROOT CD-ROM
CText		ProgramPath3;				// cesta do adres��e program� "Program" (*.exe)

CText		FunctionPath3;				// cesta do adres��e funkc� "Function" (*.fnc)
CText		NumberPath3;				// cesta do adres��e ��sel "Number" (*.num)
CText		TextPath3;					// cesta do adres��e text� "Text" (*.txt)
CText		BoolPath3;					// cesta do adres��e logick�ch hodnot "Bool" (*.log)
CText		IconPath3;					// cesta do adres��e ikon "Icon" (*.ico)
CText		MapPath3;					// cesta do adres��e ploch "Map" (*.map)
CText		PicturePath3;				// cesta do adres��e obr�zk� "Picture" (*.bmp)
CText		SpritePath3;				// cesta do adres��e sprajt� "Sprite" (*.spr)
CText		SoundPath3;					// cesta do adres��e zvuk� "Sound" (*.wav)
CText		MusicPath3;					// cesta do adres��e hudby "Music" (*.mid)

//////////////////////////////////////////////////////////////////////////////
// jazyk

// textov� konstanty
const TEXTCONST TextConst000[] =		// neutr�ln� jazyk (jm�na soubor�)
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

const TEXTCONST TextConstCz[] =			// �esky
{
#include "TextCz.inc"
};

const TEXTCONST TextConstDan[] =		// d�nsky
{
#include "TextDan.inc"
};

const TEXTCONST TextConstGer[] =		// n�mecky
{
#include "TextGer.inc"
};

const TEXTCONST TextConstRec[] =		// �ecky
{
#include "TextRec.inc"
};

const TEXTCONST TextConstEng[] =		// anglicky
{
#include "TextEng.inc"
};

const TEXTCONST TextConstSpa[] =		// �pan�lsky
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

const TEXTCONST TextConstMad[] =		// ma�arsky
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

const TEXTCONST TextConstAlb[] =		// alb�nsky
{
#include "TextAlb.inc"
};

const TEXTCONST TextConstSwe[] =		// �w�dsky
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

int		JazykUser = JAZYKAUT;			// u�ivatelem zvolen� jazyk
int		Jazyk = JAZYKCZ;				// skute�n� nastaven� jazyk - mus� m�t platn� rozsah!
int		JazykDef = JAZYKENG;			// implicitn� jazyk (podle syst�mu)
int		JazykPrev = JAZYKENG;			// p�edch�zej�c� jazyk
DWORD	LangID = 0x09;					// identifik�tor jazyku LANGID
BOOL	MultiJazyk = FALSE;				// multijazy�nost

DWORD	CharSet = DEFAULT_CHARSET;		// aktu�ln� znakov� sada font� (1=implicitn�)
DWORD	CodePage = CP_ACP;				// aktu�ln� k�dov� str�nka (0=implicitn�)
DWORD	DefCharSet = DEFAULT_CHARSET;	// implicitn� znakov� sada font�
DWORD	DefCodePage = CP_ACP;			// implicitn� k�dov� str�nka
char	Carka = '.';					// odd�lova� desetinn� ��rky p�i zobrazen� ��sel

// JAKJAK - WINNT.H ... Primary language IDs. - to jsou ta hex ��sla, zde je: #define LANG_TURKISH 0x1f, #define LANG_VIETNAMESE 0x2a

const JAZYKINFO JazykInfo[JAZYKNUM] =
{
	TextConst000,	0x00,	_T("LANGID 0"),		1252,	ANSI_CHARSET,		ButtonEng,	_T("Neutral"),		0,		FALSE,	_T("EN"),	FALSE,	// neutr�ln� - obsahuje jm�na soubor�
	TextConstAra,	0x01,	_T("LANGID 1"),		1256,	ARABIC_CHARSET,		ButtonAra,	_T("Arabic"),		IDN_JAZYKARA,	FALSE,	_T("AR"),	TRUE,	// arabsky
	TextConstBul,	0x02,	_T("LANGID 2"),		1251,	RUSSIAN_CHARSET,	ButtonBul,	_T("Bulgaria"),		IDN_JAZYKBUL,	FALSE,	_T("BU"),	FALSE,	// bulharsky
	TextConstCz,	0x05,	_T("LANGID 5"),		1250,	EASTEUROPE_CHARSET,	ButtonCz,	_T("Czech"),		IDN_JAZYKCZ,	TRUE,	_T("CZ"),	FALSE,	// �esky
	TextConstDan,	0x06,	_T("LANGID 6"),		1252,	ANSI_CHARSET,		ButtonDan,	_T("Danish"),		IDN_JAZYKDAN,	FALSE,	_T("DA"),	FALSE,	// d�nsky
	TextConstGer,	0x07,	_T("LANGID 7"),		1252,	ANSI_CHARSET,		ButtonGer,	_T("German"),		IDN_JAZYKGER,	TRUE,	_T("GE"),	FALSE,	// n�mecky
	TextConstRec,	0x08,	_T("LANGID 8"),		1253,	GREEK_CHARSET,		ButtonRec,	_T("Greek"),		IDN_JAZYKREC,	FALSE,	_T("GR"),	FALSE,	// �ecky
	TextConstEng,	0x09,	_T("LANGID 9"),		1252,	ANSI_CHARSET,		ButtonEng,	_T("English"),		IDN_JAZYKENG,	TRUE,	_T("EN"),	FALSE,	// anglicky
	TextConstSpa,	0x0a,	_T("LANGID 10"),	1252,	ANSI_CHARSET,		ButtonSpa,	_T("Spanish"),		IDN_JAZYKSPA,	FALSE,	_T("SP"),	FALSE,	// �pan�lsky
	TextConstFin,	0x0b,	_T("LANGID 11"),	1257,	BALTIC_CHARSET,		ButtonFin,	_T("Finnish"),		IDN_JAZYKFIN,	FALSE,	_T("FI"),	FALSE,	// finsky
	TextConstFra,	0x0c,	_T("LANGID 12"),	1252,	ANSI_CHARSET,		ButtonFra,	_T("France"),		IDN_JAZYKFRA,	FALSE,	_T("FR"),	FALSE,	// francouzsky
	TextConstHeb,	0x0d,	_T("LANGID 13"),	1255,	HEBREW_CHARSET,		ButtonHeb,	_T("Hebrew"),		IDN_JAZYKHEB,	FALSE,	_T("HE"),	TRUE,	// hebrejsky
	TextConstMad,	0x0e,	_T("LANGID 14"),	1250,	EASTEUROPE_CHARSET,	ButtonMad,	_T("Hungarian"),	IDN_JAZYKMAD,	FALSE,	_T("HU"),	FALSE,	// ma�arsky
	TextConstIsl,	0x0f,	_T("LANGID 15"),	1257,	BALTIC_CHARSET,		ButtonIsl,	_T("Icelandic"),	IDN_JAZYKISL,	FALSE,	_T("IC"),	FALSE,	// islandsky
	TextConstIta,	0x10,	_T("LANGID 16"),	1252,	ANSI_CHARSET,		ButtonIta,	_T("Italian"),		IDN_JAZYKITA,	FALSE,	_T("IT"),	FALSE,	// italsky
	TextConstHol,	0x13,	_T("LANGID 19"),	1252,	ANSI_CHARSET,		ButtonHol,	_T("Dutch"),		IDN_JAZYKHOL,	FALSE,	_T("DU"),	FALSE,	// holansky
	TextConstNor,	0x14,	_T("LANGID 20"),	1257,	BALTIC_CHARSET,		ButtonNor,	_T("Norwegian"),	IDN_JAZYKNOR,	FALSE,	_T("NO"),	FALSE,	// norsky
	TextConstPol,	0x15,	_T("LANGID 21"),	1250,	EASTEUROPE_CHARSET,	ButtonPol,	_T("Polish"),		IDN_JAZYKPOL,	FALSE,	_T("PL"),	FALSE,	// polsky
	TextConstPor,	0x16,	_T("LANGID 22"),	1252,	ANSI_CHARSET,		ButtonPor,	_T("Portuguese"),	IDN_JAZYKPOR,	FALSE,	_T("PO"),	FALSE,	// portugalsky
	TextConstRum,	0x18,	_T("LANGID 24"),	1251,	RUSSIAN_CHARSET,	ButtonRum,	_T("Romania"),		IDN_JAZYKRUM,	FALSE,	_T("RO"),	FALSE,	// rumunsky
	TextConstRus,	0x19,	_T("LANGID 25"),	1251,	RUSSIAN_CHARSET,	ButtonRus,	_T("Russian"),		IDN_JAZYKRUS,	FALSE,	_T("RU"),	FALSE,	// rusky
	TextConstSrb,	0x1a,	_T("LANGID 26"),	1250,	EASTEUROPE_CHARSET,	ButtonSrb,	_T("Serbo-Croat"),	IDN_JAZYKSRB,	FALSE,	_T("SC"),	FALSE,	// srbochorv�tsky
	TextConstSlo,	0x1b,	_T("LANGID 27"),	1250,	EASTEUROPE_CHARSET,	ButtonSlo,	_T("Slovakia"),		IDN_JAZYKSLO,	TRUE,	_T("SK"),	FALSE,	// slovensky
	TextConstAlb,	0x1c,	_T("LANGID 28"),	1253,	GREEK_CHARSET,		ButtonAlb,	_T("Albanian"),		IDN_JAZYKALB,	FALSE,	_T("AL"),	FALSE,	// alb�nsky
	TextConstSwe,	0x1d,	_T("LANGID 29"),	1257,	BALTIC_CHARSET,		ButtonSwe,	_T("Swedish"),		IDN_JAZYKSWE,	FALSE,	_T("SW"),	FALSE,	// �v�dsky
	TextConstTur,	0x1f,	_T("LANGID 31"),	1254,	TURKISH_CHARSET,	ButtonTur,	_T("Turkish"),		IDN_JAZYKTUR,	FALSE,	_T("TU"),	FALSE,	// turecky
	TextConstVie,	0x2a,	_T("LANGID 42"),	1258,	VIETNAMESE_CHARSET,	ButtonVie,	_T("Vietnamese"),	IDN_JAZYKVIE,	FALSE,	_T("VI"),	FALSE,	// vietnamsky
};

const int TextConstNum = sizeof(TextConstCz)/sizeof(TextConstCz[0]);


//////////////////////////////////////////////////////////////////////////////
// fonty

HFONT		FontDefault = NULL;			// implicitn� syst�mov� font
HFONT		FontDefaultBold = NULL;		// implicitn� syst�mov� font BOLD
HFONT		FontDefaultFixed = NULL;	// implicitn� syst�mov� font FIXED
HFONT		FontDefaultBoldFixed = NULL;// implicitn� syst�mov� font BOLD FIXED

HFONT		FontBezny = NULL;			// b�n� font pro dialogy a stavov� ��dek
HFONT		FontBold = NULL;			// tu�n� font pro dialogy a nadpisy oken
HFONT		FontFixed = NULL;			// font s pevnou rozte�� pro dialogy
HFONT		FontFixedBold = NULL;		// tu�n� font s pevnou rozte�� pro dialogy

CText		UserFont(_T("Impact"));		// u�ivatelsk� font (SERIF i FIXED nastaveny)

CText		SetupVzor;					// vzor textu pro nastaven� p�sma

CFont		SelectFont;					// font v�b�ru programu
CFont		TreeFont;					// font editoru programu
CFont		TextFont;					// font editoru textu
CFont		MenuFont;					// font nab�dky
CFont		StatusFont;					// font stavov� li�ty
CFont		GraphicFont;				// font grafick�ho editoru

//////////////////////////////////////////////////////////////////////////////
// standardn� fonty (pro ta�en� ikon ve stromu) (nen� komprimov�no!)

CBufIcon	StdFonts;					// buffer standardn�ch font� (indexy 0 a� 255)
CBufIndex	StdFontsWidth;				// ���ky znak� standardn�ch font� (indexy 0 a� 255)


//////////////////////////////////////////////////////////////////////////////
// lok�ln� prom�nn�

#ifdef _MT
static	CRITICAL_SECTION	ExitCriticalSection;	// kritick� sekce pro konec programu
#endif

unsigned	RandomSeed;					// prom�nn� pro gener�tor n�hody


//////////////////////////////////////////////////////////////////////////////
// konfigurace

int Scale;					// m���tko pro relativn� rozm�ry okna

int MainLeft;				// relativn� pozice hlavn�ho okna vlevo
int MainTop;				// relativn� pozice hlavn�ho okna naho�e
int MainWidth;				// relativn� ���ka hlavn�ho okna
int MainHeight;				// relativn� v��ka hlavn�ho okna

int LeftWidth;				// relativn� ���ka lev�ho pole (objekty)
int LeftHeight;				// relativn� v��ka lev�ho horn�ho okna (glob�ln� objekty)
int RightWidth;				// relativn� ���ka prav�ho pole (t��dy)
int RightHeight;			// relativn� v��ka prav�ho horn�ho okna (struktury)

BOOL Maximized;				// p��znak maximalizace okna

CText FindString;			// hledan� text

SMALLICON BufZoom[PROGBUFNUM];	// p��znaky zoom oken (TRUE=zv�t�eno)

//////////////////////////////////////////////////////////////////////////////
// konstanty

const long double pi  = 3.14159265358979323846;		// Ludolfovo ��slo
const long double pi2 = 6.28318530717958647693;		// Ludolfovo ��slo * 2
const long double eul = 2.71828182845904523536;		// Eulerovo ��slo

const long double uhel22  = 0.39269908169872415481;	// �hel 22.5 stup�� v radi�nech
const long double uhel30  = 0.52359877559829887308; // �hel 30 stup�� v radi�nech
const long double uhel45  = 0.78539816339744830962;	// �hel 45 stup�� v radi�nech
const long double uhel60  = 1.04719755119659774615; // �hel 60 stup�� v radi�nech
const long double uhel67  = 1.17809724509617246442; // �hel 67.5 stup�� v radi�nech
const long double uhel90  = 1.57079632679489661923;	// �hel 90 stup�� v radi�nech
const long double uhel135 = 2.35619449019234492885;	// �hel 135 stup�� v radi�nech
const long double uhel180 = 3.14159265358979323846;	// �hel 180 stup�� v radi�nech
const long double uhel225 = 3.92699081698724154808;	// �hel 225 stup�� v radi�nech
const long double uhel270 = 4.71238898038468985769;	// �hel 270 stup�� v radi�nech
const long double uhel315 = 5.49778714378213816731;	// �hel 315 stup�� v radi�nech

const long double ln10 = 2.3025850929940456840;	// p�irozen� logaritmus 10

const long double degrad = 0.017453292519943295769237;	// konstanta pro p�evod z DEG na RAD (pi/180)
const long double raddeg = 57.2957795130823208768;	// konstanta pro p�evod z RAD na DEG (180/pi)

CText		MemErrNadpis;							// nadpis okna chybov�ho hl�en� nedostatku pam�ti
LPCTSTR		MemErrNadpisP0 = _T("Memory Error");
LPCTSTR		MemErrNadpisP = MemErrNadpisP0;
CText		MemErrText;								// text okna chybov�ho hl�en� nedostatku pam�ti
LPCTSTR		MemErrTextP0 = _T("Insufficient memory to run PETER.EXE.");
LPCTSTR		MemErrTextP = MemErrTextP0;

/*=========================================================================*\
+																			+
+								Palety a barvy								+
+																			+
\*=========================================================================*/

// tabulka hladin standardn�ch barev (rozsah 0 a� 256)
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
// po zm�n� palet zru� soubor PALIMP.DAT a spus� DEBUG OPTIM verzi pro nov� vygenerov�n�
// (pozor - generov�n� m��e trvat des�tky minut). Potom znovu spustit p�eklad.
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// tabulka odst�n� standardn�ch barev (0 a� 255) - po�ad� B, G, R, F,
const RGBQUAD ColColTab[] = 
{ 
	0,		0,		255,		0,			//  0: �erven�
	72,		72,		255,		0,			//  1:
	116,	116,	255,		0,			//  2:
	158,	166,	255,		0,			//  3:
	210,	220,	255,		0,			//  4:
	0,		114,	255,		0,			//  5: hn�d�
	150,	198,	255,		0,			//  6:
	0,		182,	255,		0,			//  7: oran�ov�
	96,		192,	255,		0,			//  8:
	0,		255,	255,		0,			//  9: �lut�
	128,	255,	255,		0,			// 10:
	192,	255,	255,		0,			// 11: sv�tle �lut�
	0,		255,	178,		0,			// 12: �lutozelen�
	0,		255,	0,			0,			// 13: zelen�
	128,	255,	128,		0,			// 14:
	192,	255,	192,		0,			// 15:
	146,	255,	0,			0,			// 16: namodrale zelen�
	222,	255,	138,		0,			// 17:
	255,	255,	0,			0,			// 18: modrozelen�
	255,	186,	0,			0,			// 19:
	255,	222,	124,		0,			// 20:
	255,	128,	0,			0,			// 21:
	255,	0,		0,			0,			// 22: modr�
	255,	80,		72,			0,			// 23:
	255,	144,	128,		0,			// 24:
	255,	192,	154,		0,			// 25:
	255,	238,	210,		0,			// 26:
	255,	0,		114,		0,			// 27:
	255,	0,		174,		0,			// 28:
	255,	162,	198,		0,			// 29:
	255,	0,		255,		0,			// 30: fialov�
	255,	128,	255,		0,			// 31: sv�tle fialov�
	138,	0,		255,		0,			// 32: fialov� �erven�
	192,	128,	255,		0,			// 33:
	210,	186,	255,		0,			// 34:

	255,	255,	255,		0,			// 35: b�l� (nepou�ije se, vygeneruje se)
	96,		96,		96,			0,			// 36: �ed� (nepou�ije se, vygeneruje se)
};

const int ColLev = sizeof(ColLevTab)/sizeof(ColLevTab[0]);	// po�et hladin barev
const int ColCol = sizeof(ColColTab)/sizeof(ColColTab[0]);	// po�et odst�n� barev
const int StdColors = ResCols + ColCol*ColLev;				// po�et vlastn�ch palet (za��naj� od 0)
const BYTE WhiteCol = StdColors - 2*ColLev;			// b�l� barva
const BYTE BlackCol = StdColors - 1;				// �ern� barva

const BYTE BlueCol = 22*ColLev + ResCols;			// modr� barva
const BYTE RedCol = 0*ColLev + ResCols;				// �erven� barva
const BYTE YellowCol = 9*ColLev + ResCols;			// �lut� barva
const BYTE GreenCol = 13*ColLev + ResCols;			// zelen� barva
const BYTE LtYellowCol = 11*ColLev + ResCols;		// sv�tle �lut� barva
const BYTE OrangeCol = 7*ColLev + ResCols;			// oran�ov�

/////////////////////////////////////////////////////////////////////////////
// vygenerov�n� tabulky pro import palet

#ifndef _OPTIM
#ifdef _DEBUG

void GenPalImport()
{
// lok�ln� prom�nn�
	short i;				// pracovn� ��ta�
	short akt;				// vzd�lenost od aktu�ln� barvy
	BYTE last = 0;			// posledn� nalezen� nejbli��� barva
	short min;				// vzd�lenost od nalezen� nejbli��� barvy
	BYTE r,g,b;				// hledan� barva
	short ri, gi, bi;		// ��ta�e barev
	RGBQUAD* rgb;			// ukazatel standardn�ch palet

// cyklus p�es v�echny hodnoty barev
	for (ri = 63; ri >=0; ri--)
	{
		r = (BYTE)(ri*4 + ri/16);

		for (gi = 63; gi >= 0; gi--)
		{
			g = (BYTE)(gi*4 + gi/16);

			for (bi = 63; bi >= 0; bi--)
			{
				b = (BYTE)(bi*4 + bi/16);


// p��prava ukazatel�
				min = 3*256;		// asi tak n�jak� velk� vzd�lenost
				rgb = &(StdBitmapInfo->bmiColors[ResCols]);	// ukazatel barev

// cyklus p�es v�echny standardn� barvy
				for (i = ResCols; i < StdColors; i++)
				{

// vzd�lenost od aktu�ln� barvy
					akt =	(short)(
							abs(rgb->rgbRed - r) +
							abs(rgb->rgbGreen - g) +
							abs(rgb->rgbBlue - b));

// kontrola vzd�lenosti od standardn� barvy
					if (akt < min)
					{
						min = akt;			// nov� vzd�lenost
						last = (BYTE)i;		// nov� index nejbli��� barvy
					}

// posun na dal�� standardn� barvu
					rgb++;
				}

// konverzn� hodnota
				StdPalImport[ri + 64*gi + 64*64*bi] = last;
			}
		}
	}
}

#endif // _DEBUG
#endif // _OPTIM

/*=========================================================================*\
+																			+
+								P�lt�nov� �t�tec							+
+																			+
\*=========================================================================*/

//////////////////////////////////////////////////////////////////////////////
// z�sobn�k objekt�

HBRUSH		HalfToneBrush;				// p�lt�nov� �t�tec

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
// inicializace p�lt�nov�ho �t�tce

void InitHalfTone()
{
// vytvo�en� bitmapy s v�pln�
	HBITMAP bmp = ::CreateBitmap(8, 8, 1, 1, HalfTonePattern);

// vytvo�en� �t�tce
	HalfToneBrush = ::CreatePatternBrush(bmp);

// zru�en� bitmapy
	::DeleteObject(bmp);
}


/*=========================================================================*\
+																			+
+								Inicializace								+
+																			+
\*=========================================================================*/

//////////////////////////////////////////////////////////////////////////////
// pomocn� texty pro INI

// celkov�
LPCTSTR tGlobGroup		= _T("Global");
//LPCTSTR tDither			= _T("Dithering");
LPCTSTR tShortName		= _T("ShortName");

// jazyk
LPCTSTR tLanguage		= _T("Language");
LPCTSTR tLangPrev		= _T("LangPrev");
LPCTSTR	tLangMulti		= _T("LangMulti");
LPCTSTR tLanguageAuto	= _T("Auto");

// hlavn� okno
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

// posledn� pou��t�
LPCTSTR	tHistGroup		= _T("History");

LPCTSTR	tLastFile		= _T("LastFile");
LPCTSTR	tLastPath		= _T("LastPath");
LPCTSTR	tFindText		= _T("FindText");

//////////////////////////////////////////////////////////////////////////////
// na�ten� konfigurace z INI souboru

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
// hlavn� okno
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

// posledn� pou�it�
	Jmeno.MultiText(JAZYK000, ReadIniTxt(tHistGroup, tLastFile, Jmeno.MultiText((int)JAZYK000)));
	Cesta				= ReadIniTxt(tHistGroup, tLastPath,		Cesta);
	FindString			= ReadIniTxt(tHistGroup, tFindText,		EmptyText);

// zkr�cen� jm�na
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

// p�ede�l� jazyk
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
// ulo�en� konfigurace do INI souboru

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
// zkr�cen� jm�na
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

// hlavn� okno
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

// posledn� pou�it�
	WriteIniTxt(tHistGroup, tLastFile,		Jmeno.MultiText((int)JAZYK000));
	WriteIniTxt(tHistGroup, tLastPath,		Cesta);
	WriteIniTxt(tHistGroup, tFindText,		FindString);
}


//////////////////////////////////////////////////////////////////////////////
// inicializace cest do adres���

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

// cesty do lok�ln�ch adres���
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

// test, zda to je stejn� adres��
	CText aktdir = AktDir;
	CText homedir = HomeDir;
	aktdir.UpperCase();
	homedir.UpperCase();

	if (aktdir != homedir)
	{

// cesty do vzorov�ch adres���
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

// cesty do vzorov�ch adres��� CD-ROM (m�n� se p�i otev�en� CD-ROM)
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
+								Jazykov� podpora							+
+																			+
\*=========================================================================*/

//////////////////////////////////////////////////////////////////////////////
// tabulka k prov��en� textov�ch konstant

#ifdef _DEBUG

const int TestTextTab[] = 
{
// [0] R�zn� texty
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
// [1] Menu "Soubor" v�b�ru program� - dopl�uje menu "Soubor" editoru
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
// [1] Podmenu "P�smo" je sou��st� menu "Soubor"
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
// [1] Podmenu "Jazyk" - je sou��st� menu "Soubor"
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
// [1] Menu "Pomoc" (N�pov�da)
	IDN_HLP0,
	IDN_HELP0,
	IDN_HELP,
	IDN_HELPT0,
	IDN_HELPT,
	IDN_HELPF0,
	IDN_HELPF,
	IDN_ABOUT0,
	IDN_ABOUT,
// [1] Menu "Soubor" editoru - dopl�uje menu "Soubor" v�b�ru program�
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
// [1] Menu "�pravy" (Editace)
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
// [1] Menu "Zobrazen�"
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
// [3] Dialog chybov�ho hl�en� o nedostatku pam�ti
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
// [3] Dialog interpreteru hl�en� nespustiteln�ho programu
	IDN_NESPUSTITELNY,
	IDN_NESPUSTITELNY2,
// [3] Dialog nastaven� p�sma oken strom�
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
// na�ten� ��seln�ho registru (0=nenalezen)

int GetRegNum(CText key, CText name)
{
// v�sledn� hodnota
	DWORD num = 0;

// otev�en� kl��e
	HKEY hkey;
	if (::RegOpenKeyEx(
		HKEY_LOCAL_MACHINE,
		key,
		0,
		KEY_QUERY_VALUE,
		&hkey) == ERROR_SUCCESS)
	{

// na�ten� hodnoty kl��e
		DWORD type = REG_DWORD;
		DWORD bufsize = 4;

		::RegQueryValueEx(
			hkey,
			name,
			0,
			&type,
			(BYTE*)&num,
			&bufsize);

// uzav�en� kl��e
		::RegCloseKey(hkey);
	}

	return num;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� ��seln�ho registru

void SetRegNum(CText key, CText name, DWORD num)
{
// otev�en� kl��e
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

// nastaven� hodnoty registru
		::RegSetValueEx(
			hkey,
			name,
			0,
			REG_DWORD,
			(const BYTE*)&num,
			4);

// uzav�en� kl��e
		::RegCloseKey(hkey);
	}
}
			 

//////////////////////////////////////////////////////////////////////////////
// nastaven� identifik�toru jazyku do registr� Windows (0=implicitn�)

void SetJazykReg(int langID)
{
// korekce na implicitn� jazyk
	if (langID == (::GetUserDefaultLangID() & 0xff)) langID = 0;

// zji�t�n� aktu�ln� hodnoty p�ep�na�e
	int oldID = GetRegNum(ProductKey0, _T("Language"));

// nastaven� nov� hodnoty
	if (oldID != langID)
	{
		SetRegNum(ProductKey0, _T("Language"), langID);
	}
}

//////////////////////////////////////////////////////////////////////////////
// zm�na jazyku

void SetJazyk(int jazyk)
{
// nastaven� nov�ho jazyka
	JazykUser = jazyk;

// automatick� jazyk
	if ((DWORD)jazyk >= (DWORD)JAZYKNUM) jazyk = JazykDef;

	int oldjazyk = Jazyk;
	Jazyk = jazyk;	

// �schova p�edch�zej�c�ho jazyku
	if (jazyk != oldjazyk) JazykPrev = oldjazyk;

// nastaven� k�du text� a k�du jazyku
	LangID = JazykInfo[Jazyk].LangID;

// desetinn� te�ka/��rka (te�ku pouz�v� pouze USA)
	if (Jazyk == JAZYKENG)
	{
		Carka = '.';
	}
	else
	{
		Carka = ',';
	}

// nastaven� k�du jazyku do registr� Windows
	SetJazykReg(LangID);

// p��prava znakov� sady (pou��v� se i pro interpreter)
	CharSet = JazykInfo[Jazyk].CharSet;
	CodePage = JazykInfo[Jazyk].CodePage;

	if (Jazyk == JAZYKENG)
	{
		CharSet = DefCharSet;
		CodePage = DefCodePage;
	}

// p��prava b�n�ch font�
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

// p��prava menu a text�
	MenuAkt = NULL;

	if (MainFrame != NULL)
	{

// nastaven� sm�ru textu zprava doleva   JAKJAK
           SetRightToLeft(JazykInfo[Jazyk].RightToLeft);

// nastaven� nov�ho menu a font�
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

// aktualizace text� strom�
		if (oldjazyk != Jazyk)
		{
			ProgReloadJazyk();
		}

// aktualizace nadpisu okna
		MainFrameNameInit();
		MainFrameText.Empty();
		ZobrazJmeno();
	}

// inicializace text� pro hl�en� chyby pam�ti
	MemErrNadpis.Load(IDN_MEMERR_NADPIS);
	MemErrNadpisP = MemErrNadpis;
	MemErrText.Load(IDN_MEMERR_TEXT);
	MemErrTextP = MemErrText;

// aktualizace textu p�ep�na�e logick� prom�nn�
	if (EditLogWnd != NULL)
	{
		CText txt;
		txt.Load(IDN_ZAPNUTO);
		txt.SetWindowText(EditLogWnd);
	}

// aktualizace editoru textu
	EditText::StartReInit();
	EditText::UpdateClipboard();

// vzor textu p�sma
	if ((oldjazyk != Jazyk) || SetupVzor.IsEmpty())
	{
		SetupVzor.Load(IDN_SETUP_VZOR2);
	}

// nastaven� fontu stavov� li�ty
	SetStatusFont();

// zru�en� p�vodn�ch b�n�ch font�
	DelFont(oldfontbezny);
	DelFont(oldfontbold);
	DelFont(oldfontfixed);
	DelFont(oldfontfixedbold);

/* JAKJAKJAK 051222

// nastaven� kl�vesnice
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
// nastaven� font� v okn�

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
+							Hlavn� start programu							+
+																			+
\*=========================================================================*/

//////////////////////////////////////////////////////////////////////////////
// inicializace �seku inicializa�n�ch/termina�n�ch funkc�

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
// hlavn� start programu

void WinMainCRTStartup()
{

// kontrola konstant p�eklada�e
#ifndef _OPTIM
#pragma warning ( disable: 4127)				// hl�en� - konstantn� podm�nka
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
#pragma warning ( default: 4127)				// hl�en� - konstantn� podm�nka
#endif

// �schova adresy p��kazov�ho ��dku
	CommandLine = ::GetCommandLine();

// �schova verze syst�mu
	VerzeOS = (int)::GetVersion();
	OSVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx(&OSVersionInfo);

// handle instance programu
	hInstance = ::GetModuleHandle(NULL);

// nastaven� chybov�ho m�du
	::SetErrorMode(::SetErrorMode(0) | SEM_FAILCRITICALERRORS | SEM_NOOPENFILEERRORBOX); 

// zji�t�n� implicitn�ho jazyku
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

// detekce implicitn� znakov� sady
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

// �schova p�ede�l�ho aktivn�ho okna
	PrevWindow = ::GetForegroundWindow();

// inicializace informac� o displeji
	ScreenWidth = ::GetSystemMetrics(SM_CXFULLSCREEN);
	ScreenHeight = ::GetSystemMetrics(SM_CYFULLSCREEN);

// inicializace uzamyk�n� ukon�en� programu
#ifdef _MT
	::InitializeCriticalSection(&ExitCriticalSection);
#endif

// inicializace spr�vce pam�ti
	if (!MemInit()) 
	{
MEMERROR:
		::MessageBox(MainFrame, MemErrTextP0, MemErrNadpisP0, MB_OK | MB_ICONSTOP);
		Exit(EXITCODE_MEMERRINI);
	}

// p��prava �asova�e
	if (VerzeOS > 0) 
	{
		TimerConst = 60;			// �asova� pro NT
	}
	else
	{
		TimerConst = 30;			// �asova� pro 95
	}

// p��prava standardn�ch font�
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

// inicializace gener�toru n�hory
	SYSTEMTIME systime;
	::GetLocalTime(&systime);
	unsigned time = systime.wMilliseconds + 137*systime.wSecond + 2567*systime.wMinute + 358946*systime.wDay;
	RandomSeed = time + 486874*time;	// inicializace gener�toru n�hody

// inicializace koprocesoru
#ifdef _M_IX86
	WORD stat;
	_asm {
		wait						// synchronizace
		fnstcw		stat			// ulo�en� ��dic�ho slova
		wait						// synchronizace
		mov			ax,stat			// stav ��dic�ho slova
		and			ah,not 0xc		// implicitn� zaokrouhlov�n�
		or			ah,3			// p�esnost 64 bit�
		mov			stat,ax			// nov� stav ��dic�ho slova
		fldcw		stat			// nastaven� nov�ho ��dic�ho slova
	}
#endif

// inicializace obsluhy ikon
	if (!InitIcon()) goto MEMERROR;

// inicializace obsluhy obr�zk�
	if (!InitPicture()) goto MEMERROR;

// inicializace obsluhy sprajt�
	if (!InitSprite()) goto MEMERROR;

// inicializace glob�ln�ch objekt�
	InitTerm(__xc_a, __xc_z);

// inicializace text� pro hl�en� chyby pam�ti
	MemErrNadpis.Load(IDN_MEMERR_NADPIS);
	MemErrNadpisP = MemErrNadpis;
	MemErrText.Load(IDN_MEMERR_TEXT);
	MemErrTextP = MemErrText;

// na�ten� jm�na programu
	TextExe = _T(".exe");
	TextExe2 = _T(".EXE");
	TCHAR buf[_MAX_PATH];
	buf[0] = 0;
	::GetModuleFileName(hInstance, buf, _MAX_PATH);
	ExeFileName = buf;

// aktu�ln� adres�� u�ivatele
	AktDir.GetAktDir();
	if (AktDir.LastChar() != _T('\\'))
	{
		AktDir += _T('\\');
	}

// domovsk� adres�� programu
	HomeDir = ExeFileName.Left(ExeFileName.RevFind(_T('\\'))+1);
	if (HomeDir.IsEmpty()) HomeDir = AktDir;
	if (HomeDir.LastChar() != _T('\\'))
	{
		HomeDir += _T('\\');
	}

// p��prava jm�na souboru n�pov�dy
//	HelpFileName = HomeDir + _T("Peter_en.hlp");

	IniFileName = AktDir + _T("Peter.ini");

// inicializace standardn�ho z�hlav� BMP
	StdBitmapInfo = (BITMAPINFO*) MemGet(sizeof(BITMAPINFO) + sizeof(RGBQUAD)*255);
	if (StdBitmapInfo == NULL) goto MEMERROR;
	MemFill(StdBitmapInfo, sizeof(BITMAPINFO) + sizeof(RGBQUAD)*255, 0);	// vynulov�n�
	StdBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);	// velikost z�hlav�
	StdBitmapInfo->bmiHeader.biWidth = ICONWIDTH;	// ���ka bitmapy
	StdBitmapInfo->bmiHeader.biHeight = ICONHEIGHT;	// v��ka bitmapy
	StdBitmapInfo->bmiHeader.biPlanes = 1;			// po�et barevn�ch rovin
	StdBitmapInfo->bmiHeader.biBitCount = 8;		// po�et bit� na bod
	StdBitmapInfo->bmiHeader.biCompression = BI_RGB; // nen� komprese
	StdBitmapInfo->bmiHeader.biClrImportant = StdColors; // po�et d�le�it�ch palet

// vytvo�en� standardn�ch palet
	RGBQUAD* rgb = StdBitmapInfo->bmiColors;		// za��tek tabulky palet

// nem�nit paletu barvy pozad� - pou��v� se p�i importu obr�zk�
	rgb->rgbRed =	BACKCOLOR_RED;					// pr�hledn� barva
	rgb->rgbGreen =	BACKCOLOR_GREEN;
	rgb->rgbBlue =	BACKCOLOR_BLUE;
	rgb++;

	rgb->rgbRed =	SHADCOLOR_RED;					// barva st�nu
	rgb->rgbGreen =	SHADCOLOR_GREEN;
	rgb->rgbBlue =	SHADCOLOR_BLUE;
	rgb++;

	int i,j,k;
	for (i = 0; i < (ColCol-2); i++)				// pro v�echny barvy bez b�l� a �ed�
	{
		*rgb = ColColTab[i];						// p�enesen� z�kladn� barvy

		for (j = 1; j < ColLev; j++)				// pro v�echny odst�ny
		{
			k = ColLevTab[j];						// n�sob�c� koeficient
			rgb[j].rgbRed = (BYTE)(rgb->rgbRed*k/256);		// �erven�
			rgb[j].rgbGreen = (BYTE)(rgb->rgbGreen*k/256);	// zelen�
			rgb[j].rgbBlue = (BYTE)(rgb->rgbBlue*k/256);	// modr�
		}
		rgb += ColLev;								// adresa dal�� barvy
	}

	i = WhiteCol;									// index b�l� barvy
	for (; i <= BlackCol; i++)						// od b�l� barvy a� po �ernou
	{
		k = (BlackCol-i)*255/(2*ColLev-1);			// odst�n b�l� barvy
		rgb->rgbRed = (BYTE)k;						// �erven�
		rgb->rgbGreen = (BYTE)k;					// zelen�
		rgb->rgbBlue = (BYTE)k;						// modr�
		rgb++;										// zv��en� adresy barvy
	}

// vygenerov�n� tabulky pro import palet a tabulky palet (pro konverzi BMP)
	StdPalImport = (BYTE*)MemGet(64 * 64 * 64);		// tabulka pro import palet
	if (StdPalImport == NULL) goto MEMERROR;

#ifndef _OPTIM
#ifdef _DEBUG
	if ((int)::GetFileAttributes(_T("PALIMP.DAT")) == -1)
	{
		if (::MessageBox(NULL, _T("Bude generov�na tabulka importu barev.\nOperace potrv� n�kolik minut.\nChcete pokra�ovat?"),
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

// prom�nn� pro palety (p�eddeklarace kv�li skoku)
	LOGPALETTE* pStdPalette = NULL;
	PALETTEENTRY* pal = NULL;
#ifdef _DEBUG
	int cc, cc0, l;
#endif // _DEBUG

// na�ten� tabulky importu palet
	HRSRC hRes = ::FindResource(hInstance, MAKEINTRESOURCE(IDN_PALIMP), _T("LOADER"));
	HGLOBAL hData = ::LoadResource(hInstance, hRes);
	ASSERT((hRes != NULL) && (hData != NULL));
	if ((hRes == NULL) || (hData == NULL)) goto RESERROR;
	DeKomp(StdPalImport, 64*64*64, (BYTE*)::LockResource(hData)+6, ::SizeofResource(hInstance, hRes)-6);

// vytvo�en� konverzn� tabulky palet pro �mport soubor� BMP
	KonvPal = (BYTE*)MemGet(256);			// konverzn� tabulka palet
	if (KonvPal == NULL) goto MEMERROR;

// vytvo�en� vlastn�ch logick�ch palet
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

// inicializace p�lt�nov�ho �t�tce
	InitHalfTone();

// na�ten� konfigurace
	ReadIni();

// inicializace cest do adres���
	InitPaths();

// prov��en� tabulky textov�ch konstant
#ifdef _DEBUG

#pragma warning ( disable: 4127)				// hl�en� - konstantn� podm�nka
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
#pragma warning ( default: 4127)				// hl�en� - konstantn� podm�nka

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

// buffer standardn�ch font�
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

// vytvo�en� hlavn�ho okna aplikace
	if (!MainFrameCreate()) 
	{
		CText txt1;
		txt1.Load(IDN_RESOURCE_ERR1);
		CText txt2;
		txt2.Load(IDN_RESOURCE_ERR2);
		::MessageBox(NULL, txt2, txt1, MB_OK | MB_ICONSTOP);
		Exit(EXITCODE_MAINFRAME);
	}

// test, zda nebyla chyba pam�ti
	if (MemoryError) goto MEMERROR;

// aktualizace jazyku po vytvo�en� hlavn�ho okna
	SetJazyk(JazykUser);

// hlavn� obslu�n� smy�ka aplikace
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
//					ErrText = _T("Syst�m vyvolal chybovou v�jimku ");
//					ErrText += ErrText2;
//					::MessageBox(
//						NULL, 
//						ErrText,						
//						_T("V�jimka syst�mu"), 
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
// ukon�en� programu (p�i chyb� neukl�d� konfiguraci)

void Exit(int code)
{
// uzamknut� ukon�en� programu
#ifdef _MT
	::EnterCriticalSection(&ExitCriticalSection);
#endif

// ulo�en� konfigurace
	if (code < EXITCODE_INIT)
	{
		WriteIni();
	}

// vypnut� k�du jazyku v registrech Windows
	SetJazykReg(0);

// zru�en� palet
	::DeleteObject(StdPalette);

// ukon�en� spr�vce pam�ti
	MemTerm();

// uvoln�n� uzamyk�n� ukon�en� programu
#ifdef _MT
	::DeleteCriticalSection(&ExitCriticalSection);
#endif

// nastaven� fokusu na p�ede�l� okno
	::SetForegroundWindow(PrevWindow);

// ukon�en� programu
	ExitProcess(code);
}

//////////////////////////////////////////////////////////////////////////////
// chyba pro DEBUG OPTIM m�d

#if defined(_OPTIM) && defined(_M_IX86) && defined(_DEBUG)
void debugBreak(char* file, int line, char* date)
{
	CText txt("Intern� chyba programu PETER. Nahlaste, pros�m, dodavateli n�sleduj�c�\n"
			  "�daje a popi�te situaci, za kter� tato chyba nastala. D�kujeme za pochopen�.\n"
			   "\n     soubor = ");

	CText txt2(file);
	txt.Add(txt2.Right(txt2.Length() - txt2.RevFind('\\') - 1));
	txt.Add("           ��dek = ");
	txt.AddInt(line);
	txt.Add("           verze = ");
	txt.Add(date);
	txt.Add('\n');

	::MessageBox(
		MainFrame,
		txt,
		"Intern� chyba",
		MB_OK | MB_ICONERROR);
}
#endif

//////////////////////////////////////////////////////////////////////////////
// vyst�ed�n� okna proti jin�mu oknu

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
// spu�t�n� programu

int Exec(CText command, CText aktdir, BOOL wait)
{
	int result = 0;
	command.TrimLeft();
	if (command.IsEmpty()) return 255;

// korekce aktivn�ho adres��e
	if (aktdir.Length() > 1)
	{
		if (aktdir.LastChar() == '\\')
		{
			aktdir.Delete(aktdir.Length()-1);
		}
	}

// inicializace startovac� tabulky
	STARTUPINFO si;
	MemFill(&si, sizeof(si), 0);
	si.cb = sizeof(si);

// inicializace tabulky procesu
	PROCESS_INFORMATION pi;
	MemFill(&pi, sizeof(pi), 0);

// spu�t�n� programu
	::CreateProcess(
		NULL,								// jm�no programu
		(LPTSTR)(LPCTSTR)command,			// p��kazov� ��dek
		NULL,								// ochrann� atributy procesu
		NULL,								// ochrann� atributy vl�kna
		FALSE,								// d�di�nost handl�
#ifdef _UNICODE
		CREATE_UNICODE_ENVIRONMENT			// parametry
#else
		0
#endif
		| CREATE_DEFAULT_ERROR_MODE,
		NULL,								// prost�ed�
		aktdir.IsEmpty() ? NULL : (LPCTSTR)aktdir, // v�choz� adres��
		&si,								// adresa STARTUPINFO
		&pi);								// adresa PROCESS_INFORMATION

// �ek�n� na ukon�en� programu
	if (wait)
	{
		::WaitForSingleObject(pi.hProcess, INFINITE);	// �ek�n� na ukon�en� procesu
		::GetExitCodeProcess(pi.hProcess, (DWORD*)&result);	// zji�t�n� n�vratov�ho k�du
	}

// uzav�en� handle procesu
	::CloseHandle(pi.hProcess);				// uzav�en� handle procesu
	::CloseHandle(pi.hThread);				// uzav�en� handle toku

	return result;
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� fontu

HFONT GetFont(CFont* fnt)
{
// p��prava znakov� sady
	DWORD charset = CharSet;
//	if (charset == DefCharSet) charset = DEFAULT_CHARSET;

// ur�en� parametry fontu
	BOOL standard = fnt->Standard;
	BOOL bold = fnt->Bold;
	BOOL italic = fnt->Italic;
	BOOL underline = fnt->Underline;
	BOOL strikeout = fnt->Strikeout;
	BOOL serif = fnt->Serif;
	BOOL fixed = fnt->Fixed;

// p��prava zadan� v��ky a vypo�ten� v��ky
	int height = fnt->Height;
	if (height < 0) height = -height;
	int height0 = height;
	if (height0 == 0) height0 = 18;

// p��prava n�sob�c�ho koeficientu ���ky
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

// p��prava ���ky a vypo�ten� ���ky
	int width = Round(fabs(fnt->Width) * height0 * koef - 0.1);
	int width0 = width;
	if (width0 == 0) width0 = Round(height0 * koef - 0.1);

// korekce parametr� pro standardn� font
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

// �schova vypo�ten�ch parametr�
	fnt->Height0 = height0;
	fnt->Width0 = width0;

// standardn� fonty
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

// jm�no fontu
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

// vytvo�en� fontu
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

// druh� pokus, nen�-li p��tomna spr�vn� znakov� sada
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

// p�i ne�sp�chu n�hradn� font
	if (font == NULL) font = FontDefaultBold;

// n�vrat fontu
	return font;
}

/////////////////////////////////////////////////////////////////////////////
// zru�en� fontu

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
// generov�n� konverzn� tabulky barev obr�zku

void GenKonvPal(BITMAPINFO* bmp)
{
// lok�ln� prom�nn�
	int			i;									// ��ta� barev
	int			palet = bmp->bmiHeader.biClrUsed;	// po�et palet v bitmap�
	RGBQUAD*	col = bmp->bmiColors;				// ukazatel barevn�ch slo�ek
	BYTE*		pal = KonvPal;						// ukazatel konverzn�ch palet
	DWORD		BackColData = *(DWORD*)(StdBitmapInfo->bmiColors + BackCol); // paleta pozad�
	DWORD		ShadColData = *(DWORD*)(StdBitmapInfo->bmiColors + ShadCol); // paleta st�nu

// p��prava po�tu palet
	if (palet == 0) palet = (1 << bmp->bmiHeader.biBitCount);
	i = palet;
	if ((palet < 1) || (palet > 256)) return;

// cyklus p�es platn� barvy
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

// vymaz�n� zbyl�ch neplatn�ch barev (import na �ernou barvu)
	MemFill(pal, 256-palet, BlackCol);

// test, zda je tabulka standardn�
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
// vygenerov�n� st�edn� barvy (ze 4 bod�)

BYTE ColAvrg(BYTE col1, BYTE col2, BYTE col3, BYTE col4)
{
// lok�ln� prom�nn�
	int			r = 0;									// �erven� slo�ka
	int			g = 0;									// zelen� slo�ka
	int			b = 0;									// modr� slo�ka
	BYTE		n = 0;									// po�et platn�ch bod�
	RGBQUAD*	rgb;									// ukazatel palet
	int			shad = 0;								// ��ta� st�n�

// prvn� bod
	if (col1 != BackCol)
	{
		if (col1 == ShadCol)
		{
			shad++;
		}
		else
		{
			n++;											// zv��en� ��ta�e bod�
			rgb = StdBitmapInfo->bmiColors + col1;			// definice palet bodu
			r = rgb->rgbRed;								// �erven� slo�ka
			g = rgb->rgbGreen;								// zelen� slo�ka
			b = rgb->rgbBlue;								// modr� slo�ka
		}
	}

// druh� bod
	if (col2 != BackCol)
	{
		if (col2 == ShadCol)
		{
			shad++;
		}
		else
		{
			n++;											// zv��en� ��ta�e bod�
			rgb = StdBitmapInfo->bmiColors + col2;			// definice palet bodu
			r += rgb->rgbRed;								// �erven� slo�ka
			g += rgb->rgbGreen;								// zelen� slo�ka
			b += rgb->rgbBlue;								// modr� slo�ka
		}
	}

// t�et� bod
	if (col3 != BackCol)
	{
		if (col3 == ShadCol)
		{
			shad++;
		}
		else
		{
			n++;											// zv��en� ��ta�e bod�
			rgb = StdBitmapInfo->bmiColors + col3;			// definice palet bodu
			r += rgb->rgbRed;								// �erven� slo�ka
			g += rgb->rgbGreen;								// zelen� slo�ka
			b += rgb->rgbBlue;								// modr� slo�ka
		}
	}

// �tvrt� bod
	if (col4 != BackCol)
	{
		if (col4 == ShadCol)
		{
			shad++;
		}
		else
		{
			n++;											// zv��en� ��ta�e bod�
			rgb = StdBitmapInfo->bmiColors + col4;			// definice palet bodu
			r += rgb->rgbRed;								// �erven� slo�ka
			g += rgb->rgbGreen;								// zelen� slo�ka
			b += rgb->rgbBlue;								// modr� slo�ka
		}
	}

// bod byl st�n
	if (shad > 2)
	{
		return ShadCol;
	}

// bod byl pozad�
	if (n < 2)
	{
		return BackCol;
	}

// jinak vygenerov�n� bodu podle barevn�ch slo�ek
	else
	{
		return PalImport((BYTE)(r/n), (BYTE)(g/n), (BYTE)(b/n));
	}
}

/////////////////////////////////////////////////////////////////////////////
// dekomprese dat (pou��v� se pouze pro dekompresi importu palet PALIMP.DAT)

#define MAXLENX 25					// min. d�lka dlouh�ho �et�zce
#define MAXLEN (MAXLENX+254)		// maxim�ln� d�lka �et�zce
#define SUBSTLEN	7				// d�lka nahrazen� dlouh�m k�dem

void DeKomp(BYTE* dstBuf, int dstNum, BYTE* srcBuf, int srcNum)
{
// na�ten� jednoho bitu ze stavov�ho slova
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


	BYTE* dst = dstBuf;				// ukazatel c�lov� adresy
	int dsti = 0;					// ��ta� c�lov�ch dat
	BYTE* src = srcBuf;				// ukazatel zdrojov� adresy
	int srci = 0;					// ��ta� zdrojov�ch dat
	BYTE* src2;						// pomocn� ukazatel
	int srci2;

	WORD status = 0;				// st�ada� stavov�ho slova
	BYTE offsetL, offsetH;			// offset k opakov�n�
	int delka;						// d�lka k opakov�n�
	int bit;						// 1 = na�ten� bit

	for (;;)
	{
// na�ten� prvn�ho bitu p��znaku
		DekBit

// p�esun bajtu bez komprese
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

// jinak bude opakov�n� �et�zce
		else
		{
			offsetH = 0;
			offsetL = 0;

// prvn� bit d�lky
			DekBit
			delka = bit;

// zv��en� ��ta�e d�lky
			for (;;)
			{
				delka++;
				delka++;

// na�ten� p��znaku konce k�du
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

// korekce pro n�hradn� k�d
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

// stanoven� offsetu - vy��� bajt
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

// na�ten� offsetu - ni��� bajt
			if (srci >= srcNum) break;
			offsetL = *src;
			src++;
			srci++;

// p�enesen� �et�zce
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
// gener�tor n�hody

int rand()
{
	RandomSeed = RandomSeed * 214013 + 2531011;
	return ((RandomSeed >> 16) & 0x7fff);
}

double Rand()
{
	RandomSeed = RandomSeed * 214013 + 2531011;
	return (double)RandomSeed / (double) 4294967296;	// vyd�len� rozsahem LONG INT
}


/////////////////////////////////////////////////////////////////////////////
// p�evod re�ln�ho ��sla na cel� ��slo se zaokrouhlen�m k nejbli��� hodnot�

int Round(double num)
{
#ifdef _M_IX86

	DWORD		result;				// v�sledek operace

	_asm {
		fld			num				// na�ten� ��sla k proveden� operace
		fistp		result			// p�evod ��sla na cel� ��slo
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
// p�evod re�ln�ho ��sla na cel� ��slo s o��znut�m k nule

int Round0(double num)
{
#ifdef _M_IX86

	WORD		oldstat;			// star� stav ��dic�ho slova
	WORD		newstat;			// nov� stav ��dic�ho slova
	int			result;				// v�sledek operace

	_asm {
		fld			num				// na�ten� ��sla k proveden� operace
		wait						// synchronizace
		fnstcw		oldstat			// ulo�eni ��dic�ho slova
		wait						// synchronizace
		mov			ax,oldstat		// star� stav ��dic�ho slova
		or			ah,0xc			// m�d zaokrouhlen� sm�rem k nule
		mov			newstat,ax		// nov� stav ��dic�ho slova
		fldcw		newstat			// nastaven� nov�ho ��dic�ho slova
		fistp		result			// p�evod ��sla na cel� ��slo
		fldcw		oldstat			// navr�cen� p�vodn�ho ��dic�ho slova
	}
	return result;

#else

	return (int)num;

#endif
}

/////////////////////////////////////////////////////////////////////////////
// p�evod re�ln�ho ��sla na cel� ��slo se zaokrouhlen�m nahoru (ke kladn� hodnot�)

int RoundP(double num)
{
#ifdef _M_IX86

	WORD		oldstat;			// star� stav ��dic�ho slova
	WORD		newstat;			// nov� stav ��dic�ho slova
	int			result;				// v�sledek operace

	_asm {
		fld			num				// na�ten� ��sla k proveden� operace
		wait						// synchronizace
		fnstcw		oldstat			// ulo�eni ��dic�ho slova
		wait						// synchronizace
		mov			ax,oldstat		// star� stav ��dic�ho slova
		and			ah,not 0xc;		// vynulov�n� ��dic�ch bit� pro zaokrouhlen�
		or			ah,8			// m�d zaokrouhlen� sm�rem nahoru
		mov			newstat,ax		// nov� stav ��dic�ho slova
		fldcw		newstat			// nastaven� nov�ho ��dic�ho slova
		fistp		result			// p�evod ��sla na cel� ��slo
		fldcw		oldstat			// navr�cen� p�vodn�ho ��dic�ho slova
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
// p�evod re�ln�ho ��sla na cel� ��slo se zaokrouhlen�m dol� (k z�porn� hodnot�)

int RoundM(double num)
{
#ifdef _M_IX86

	WORD		oldstat;			// star� stav ��dic�ho slova
	WORD		newstat;			// nov� stav ��dic�ho slova
	int			result;				// v�sledek operace

	_asm {
		fld			num				// na�ten� ��sla k proveden� operace
		wait						// synchronizace
		fnstcw		oldstat			// ulo�eni ��dic�ho slova
		wait						// synchronizace
		mov			ax,oldstat		// star� stav ��dic�ho slova
		and			ah,not 0xc;		// vynulov�n� ��dic�ch bit� pro zaokrouhlen�
		or			ah,4			// m�d zaokrouhlen� sm�rem dol�
		mov			newstat,ax		// nov� stav ��dic�ho slova
		fldcw		newstat			// nastaven� nov�ho ��dic�ho slova
		fistp		result			// p�evod ��sla na cel� ��slo
		fldcw		oldstat			// navr�cen� p�vodn�ho ��dic�ho slova
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
// poskytnut� aktivn�ho okna, je-li typu Edit (NULL = nen�)

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
// vytvo�en� adres��e (pokud existuje, je v�e OK)

BOOL CreateDir(CText txt)
{
// o�ez�n� mezer
	txt.TrimLeft();
	txt.TrimRight();

// o�ez�n� koncov�ch "\"
	while (txt.LastChar() == _T('\\'))
	{
		txt.Delete(txt.Length() - 1);
		txt.TrimRight();
	}

// ROOT je OK
	if (txt.IsEmpty()) return TRUE;

// vytvo�en� adres��e
	BOOL res = ::CreateDirectory(txt, NULL);
	if (res) return TRUE;

// p�i ne�sp�chu kontrola, zda adres�� ji� existuje
	int attrib = (int)::GetFileAttributes(txt);
	return ((attrib != -1) && (attrib & FILE_ATTRIBUTE_DIRECTORY));
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� adres��e (i v�cestup�ov� - v�etn� cesty)

BOOL CreatePath(CText txt)
{
// prvn� pokus o vytvo�en� adres��e
	if (CreateDir(txt)) return TRUE;

// o�ez�n� mezer
	txt.TrimLeft();
	txt.TrimRight();

// o�ez�n� koncov�ch "\"
	while (txt.LastChar() == _T('\\'))
	{
		txt.Delete(txt.Length() - 1);
		txt.TrimRight();
	}

// adres�� se bude zkracovat
	CText txt2 = txt;
	CText txt3;

// zkracov�n� adres��e
	int pos;
	while ((pos = txt2.RevFind(_T('\\'))) >= 0)
	{
		txt2.Delete(pos);
		if (CreateDir(txt2)) break;
	}

// prodlu�ov�n� adres��e
	while (txt2.Length() < txt.Length())
	{
		txt2.Add(_T('\\'));
		txt3 = txt.Mid(txt2.Length(), txt.Length());
		pos = txt3.Find(_T('\\'));
		if (pos < 0) break;
		txt2.Add(txt3.Left(pos));
		if (!CreateDir(txt2)) return FALSE;
	}

// vytvo�en� posledn�ho stupn� (to je posledn� pokus)
	return CreateDir(txt);
}


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� informac� o velikosti a voln�m m�stu disku

void GetDiskSpace(CText path)
{
// zaji�t�n� \  na konci cesty
	if (path.LastChar() != _T('\\')) path.Add(_T('\\'));

// na�ten� informac� nov�j�� funkc�
	if (!pGetDiskFreeSpaceEx ||
		!pGetDiskFreeSpaceEx(path, &DiskFreeUser, &DiskSize, &DiskFree))

// na�ten� informac� star�� metodou
	{

// nalezen� ROOT adres��e
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
// na�ten� registru DWORD (-1 = chyba)

int GetRegUserDWORD(CText key, CText name)
{
// buffer v�sledn� hodnoty
	int num = -1;

// otev�en� kl��e
	HKEY hkey;

	if (::RegOpenKeyEx(
		HKEY_CURRENT_USER,
		key,
		0,
		KEY_QUERY_VALUE,
		&hkey) == ERROR_SUCCESS)
	{

// na�ten� hodnoty kl��e
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

// uzav�en� kl��e
		::RegCloseKey(hkey);
	}
	return num;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� registru DWORD (nevytv��� kl��)

void SetRegUserDWORD(CText key, CText name, int data)
{
// otev�en� kl��e
	HKEY hkey;

	if (::RegOpenKeyEx(
		HKEY_CURRENT_USER,
		key,
		0,
		KEY_SET_VALUE,
		&hkey) == ERROR_SUCCESS)
	{

// nastaven� hodnoty registru
		::RegSetValueEx(
			hkey,
			name,
			0,
			REG_DWORD,
			(const BYTE*)&data,
			4);

// uzav�en� kl��e
		::RegCloseKey(hkey);
	}
}
*/

