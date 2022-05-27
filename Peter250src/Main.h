
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


#define VerzeMaj		2			// verze - hlavn� ��slice (jednotky)
#define VerzeMin		5			// verze - vedlej�� ��slice (desetiny)
#define VerzeRel		0			// verze - ��slice vyd�n� (setiny)
#define VerzeCom		0			// verze - ��slice kompilace (tis�ciny)
#define VerzeTxt		_T("2.50")	// verze - text
#define VerzeTxt0		_T("250")	// verze - text zkr�cen� (pro clipboard)

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
// standardn� vno�en� sekce

#pragma warning ( disable: 4201)		// hl�en� - nepojmenovan� struktura

#include <windows.h>					// z�kladn� definice WINDOWS
#include <math.h>						// matematick� operace
//#include <alphaops.h>					// matematick� konstanty
#include <tchar.h>						// obsluha znak� UNICODE/MB
#include <commctrl.h>					// dopl�kov� ovl�dac� prvky
#include <richedit.h>					// RichEdit
#include <mmreg.h>						// multimedi�ln� definice
#include "resource.h"

#pragma warning ( default: 4201)		// hl�en� - nepojmenovan� struktura

#pragma warning ( disable: 4100)		// hl�en� - nepou�it� form�ln� parametr
#pragma warning ( disable: 4710)		// hl�en� - funkce nen� inline

#ifndef SPI_GETLISTBOXSMOOTHSCROLLING
#define SPI_GETLISTBOXSMOOTHSCROLLING 0x1006
#define SPI_SETLISTBOXSMOOTHSCROLLING 0x1007
#endif

#define DOUBLE_INFINITY_VALUE       ((ULONGLONG)0x7ff0000000000000)

//////////////////////////////////////////////////////////////////////////////
// p�eddefinice t��d

class CIcon; class CMap; class CMusic; class CPicture; class CSound; class CSprite;
class CText; class CFont; class CMultiText;
class CBufBool; class CBufIcon; class CBufInt; class CBufMap; class CBufMusic;
class CBufPic; class CBufReal; class CBufSound; class CBufSprite; class CBufText;
class CBufProg; class CBufIndex; class CBufUndo;
class CFileMap; class CBuf;

//////////////////////////////////////////////////////////////////////////////
// glob�ln� prom�nn�

extern	TCHAR*		CommandLine;		// p��kazov� ��dek
extern	int			VerzeOS;			// verze syst�mu
extern	HINSTANCE	hInstance;			// instance programu
extern	int			ScreenWidth;		// ���ka klientsk� oblasti displeje
extern	int			ScreenHeight;		// v��ka klientsk� oblasti displeje
extern	int			TimerConst;			// konstanta pro �asova� (pro 55 ms)

extern	HACCEL		Accel;				// akcelera�n� tabulka

extern	BYTE*		StdPalImport;		// tabulka importu palet
extern	BITMAPINFO* StdBitmapInfo;		// standardn� z�hlav� BMP
extern	HPALETTE	StdPalette;			// vlastn� logick� palety
extern	BYTE*		KonvPal;			// konverzn� tabulka palet
extern	bool		KonvPalOK;			// p��znak shodn� konverzn� tabulky (lze prov�st kopii)

extern	bool		Dither;				// pou��vat dithering

extern	bool		ShortName;			// zkracovat jm�na soubor�

//extern	bool		Profi;				// profesion�ln� verze programu

#ifdef _UNICODE
typedef	BOOL (WINAPI *GETDISKFREESPACEEX) (LPCWSTR, __int64*, __int64*, __int64*);
#else
typedef	BOOL (WINAPI *GETDISKFREESPACEEX) (LPCSTR, __int64*, __int64*, __int64*);
#endif
extern GETDISKFREESPACEEX	pGetDiskFreeSpaceEx; // funkce GetDiskFreeSpaceEx (NULL=nen�)
extern	__int64	DiskSize;			// velikost disku (z funkce GetDiskSpace)
extern	__int64	DiskFree;			// voln� m�sto disku (z funkce GetDiskSpace)
extern	__int64	DiskFreeUser;		// voln� m�sto u�ivatele (z funkce GetDiskSpace)

extern	CMultiText	Jmeno;				// jm�no editovan�ho souboru (bez cesty)
extern	CText		JmenoLoad;			// pln� jm�no souboru pro �ten� (s cestou - vzorov� adres��)
extern	CText		JmenoSave;			// pln� jm�no souboru pro z�pis
extern	CText		Cesta;				// cesta k editovan�mu souboru (s p��p. "\" na konci)
extern	CText		AktDir;				// aktivn� adres�� u�ivatele (s "\" na konci)
extern	CText		HomeDir;			// cesta do domovsk�ho adres��e s EXE (s "\" na konci)
extern	CText		ExeFileName;		// jm�no programu
extern	CText		IniFileName;		// jm�no konfigura�n�ho souboru
//extern	CText		HelpFileName;		// jm�no souboru n�pov�dy
extern	CText		TextExe;			// text ".exe"
extern	CText		TextExe2;			// text ".EXE"
extern	CText		CDRom;				// cesta k CD-ROM (pr�zdn� = nen�)

extern	HWND		PrevWindow;			// p�ede�l� aktivn� okno

extern	CText		MemErrNadpis;		// nadpis okna chybov�ho hl�en� nedostatku pam�ti
extern	LPCTSTR		MemErrNadpisP;
extern	CText		MemErrText;			// text okna chybov�ho hl�en� nedostatku pam�ti
extern	LPCTSTR		MemErrTextP;

//////////////////////////////////////////////////////////////////////////////
// cesty do adres��� programu (v�echny jsou zakon�eny s "\")

extern	CText		ProgramPath;		// cesta do adres��e program� "Program" (*.exe)

extern	CText		BackupPath;			// cesta do adres��e z�loh "Backup"

extern	CText		FunctionPath;		// cesta do adres��e funkc� "Function" (*.fnc)
extern	CText		NumberPath;			// cesta do adres��e ��sel "Number" (*.num)
extern	CText		TextPath;			// cesta do adres��e text� "Text" (*.txt)
extern	CText		BoolPath;			// cesta do adres��e logick�ch hodnot "Bool" (*.log)
extern	CText		IconPath;			// cesta do adres��e ikon "Icon" (*.ico)
extern	CText		MapPath;			// cesta do adres��e ploch "Map" (*.map)
extern	CText		PicturePath;		// cesta do adres��e obr�zk� "Picture" (*.bmp)
extern	CText		SpritePath;			// cesta do adres��e sprajt� "Sprite" (*.spr)
extern	CText		SoundPath;			// cesta do adres��e zvuk� "Sound" (*.wav)
extern	CText		MusicPath;			// cesta do adres��e hudby "Music" (*.mid)

// alternativn� knihovny - implicitn� v podadres��i programu Petr
extern	CText		ProgramPath2;		// cesta do adres��e program� "Program" (*.exe)

extern	CText		FunctionPath2;		// cesta do adres��e funkc� "Function" (*.fnc)
extern	CText		NumberPath2;		// cesta do adres��e ��sel "Number" (*.num)
extern	CText		TextPath2;			// cesta do adres��e text� "Text" (*.txt)
extern	CText		BoolPath2;			// cesta do adres��e logick�ch hodnot "Bool" (*.log)
extern	CText		IconPath2;			// cesta do adres��e ikon "Icon" (*.ico)
extern	CText		MapPath2;			// cesta do adres��e ploch "Map" (*.map)
extern	CText		PicturePath2;		// cesta do adres��e obr�zk� "Picture" (*.bmp)
extern	CText		SpritePath2;		// cesta do adres��e sprajt� "Sprite" (*.spr)
extern	CText		SoundPath2;			// cesta do adres��e zvuk� "Sound" (*.wav)
extern	CText		MusicPath2;			// cesta do adres��e hudby "Music" (*.mid)

// alternativn� knihovny - implicitn� v ROOT CD-ROM
extern	CText		ProgramPath3;		// cesta do adres��e program� "Program" (*.exe)

extern	CText		FunctionPath3;		// cesta do adres��e funkc� "Function" (*.fnc)
extern	CText		NumberPath3;		// cesta do adres��e ��sel "Number" (*.num)
extern	CText		TextPath3;			// cesta do adres��e text� "Text" (*.txt)
extern	CText		BoolPath3;			// cesta do adres��e logick�ch hodnot "Bool" (*.log)
extern	CText		IconPath3;			// cesta do adres��e ikon "Icon" (*.ico)
extern	CText		MapPath3;			// cesta do adres��e ploch "Map" (*.map)
extern	CText		PicturePath3;		// cesta do adres��e obr�zk� "Picture" (*.bmp)
extern	CText		SpritePath3;		// cesta do adres��e sprajt� "Sprite" (*.spr)
extern	CText		SoundPath3;			// cesta do adres��e zvuk� "Sound" (*.wav)
extern	CText		MusicPath3;			// cesta do adres��e hudby "Music" (*.mid)

//////////////////////////////////////////////////////////////////////////////
// jazyk

// textov� konstanty
typedef struct TEXTCONST_
{
	short		TextID;					// identifika�n� k�d textu
	const char*	Text;					// text
} TEXTCONST;

extern const int TextConstNum;			// po�et textov�ch konstant

#define	JAZYKAUT -1						// automatick� jazyk

enum JAZYKID {							// (identifik�tor jazyku - hledej LANGID)
		JAZYK000,						// neutr�ln� jazyk (0) - obsahuje jm�na soubor�
		JAZYKARA,						// arabsky (1)
		JAZYKBUL,						// bulharsky (2)
		JAZYKCZ,						// �esky (5)
		JAZYKDAN,						// d�nsky (6)
		JAZYKGER,						// n�mecky (7)
		JAZYKREC,						// �ecky (8)
		JAZYKENG,						// anglicky (9)
		JAZYKSPA,						// �pan�lsky (10)
		JAZYKFIN,						// finsky (11)
		JAZYKFRA,						// francouzsky (12)
		JAZYKHEB,						// hebrejsky (13)
		JAZYKMAD,						// ma�arsky (14)
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
		JAZYKALB,						// alb�nsky (28)
		JAZYKSWE,						// �w�dsky (29)
		JAZYKTUR,						// turecky (31)    = 0x1f
		JAZYKVIE,						// vietnamsky (42) = 0x2a

		JAZYKNUM						// po�et jazyk�
};

extern int	JazykUser;					// u�ivatelem zvolen� jazyk
extern int	Jazyk;						// skute�n� nastaven� jazyk - mus� m�t platn� rozsah!
extern int	JazykDef;					// implicitn� jazyk (podle syst�mu)
extern int	JazykPrev;					// p�edch�zej�c� jazyk

extern	DWORD	CharSet;				// aktu�ln� znakov� sada font� (1=implicitn�)
extern	DWORD	CodePage;				// aktu�ln� k�dov� str�nka (0=implicitn�)
extern	DWORD	LangID;					// identifik�tor jazyku LANGID (0=jin�)
extern	BOOL	MultiJazyk;				// multijazy�nost
extern	char	Carka;					// odd�lova� desetinn� ��rky p�i zobrazen� ��sel

// jazykov� informace
typedef struct JAZYKINFO_
{
	const TEXTCONST*	TextConst;		// tabulka text�
	int					LangID;			// identifik�tor jazyku
	LPCTSTR				LangIDini;		// identifik�tor jazyku pro ALIASES.INI
	int					CodePage;		// k�dov� str�nka znak�
	int					CharSet;		// znakov� sada font�
	int					Button;			// k�d tla��tka
	LPCTSTR				Name;			// jm�no jazyku pro INI
	int					MenuID;			// identifika�n� k�d pro menu
	BOOL				InstOK;			// jazyk je instalov�n
	LPCTSTR				HelpName;		// jm�no souboru n�pov�dy (2 p�smena: CZ, EN, ..)
	BOOL				RightToLeft;		// Right-To-Left, p�e se zprava doleva
} JAZYKINFO;

extern const JAZYKINFO JazykInfo[JAZYKNUM]; // tabulka definic jazyk�

//////////////////////////////////////////////////////////////////////////////
// nastaven� identifik�toru jazyku do registr� Windows (0=implicitn�)

void SetJazykReg(int langID);

//////////////////////////////////////////////////////////////////////////////
// zm�na jazyku

void SetJazyk(int jazyk);

//////////////////////////////////////////////////////////////////////////////
// standardn� fonty (pro ta�en� ikon ve stromu)

extern CBufIcon StdFonts;			// buffer standardn�ch font� (indexy 0 a� 255)
extern CBufIndex StdFontsWidth;		// ���ky znak� standardn�ch font� (indexy 0 a� 255)


//////////////////////////////////////////////////////////////////////////////
// z�sobn�k objekt�

extern	HBRUSH		HalfToneBrush;		// p�lt�nov� �t�tec

//////////////////////////////////////////////////////////////////////////////
// konfigurace

extern int Scale;					// m���tko pro relativn� rozm�ry okna

extern int MainLeft;				// relativn� pozice hlavn�ho okna vlevo
extern int MainTop;					// relativn� pozice hlavn�ho okna naho�e
extern int MainWidth;				// relativn� ���ka hlavn�ho okna
extern int MainHeight;				// relativn� v��ka hlavn�ho okna

extern int LeftWidth;				// relativn� ���ka lev�ho pole (objekty)
extern int LeftHeight;				// relativn� v��ka lev�ho horn�ho okna (glob�ln� objekty)
extern int RightWidth;				// relativn� ���ka prav�ho pole (t��dy)
extern int RightHeight;				// relativn� v��ka prav�ho horn�ho okna (struktury)

extern BOOL Maximized;				// p��znak maximalizace okna

extern CText FindString;			// hledan� text

// velikost ikony
typedef enum _SMALLICON {
	SI_SMALL = 0,					// mal�, 16x16
	SI_BIG = 1,						// velk�, 32x32
	SI_MEDIUM = 2,					// st�edn�, 24x24
} SMALLICON;

extern SMALLICON BufZoom[];			// p��znaky zoom oken (TRUE=zv�t�eno)

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
extern const BYTE WhiteCol;			// b�l� barva
extern const BYTE BlackCol;			// �ern� barva
extern const BYTE BlueCol;			// modr� barva
extern const BYTE RedCol;			// �erven� barva
extern const BYTE YellowCol;		// �lut� barva
extern const BYTE GreenCol;			// zelen� barva
extern const BYTE LtYellowCol;		// sv�tle �lut� barva
extern const BYTE OrangeCol;		// oran�ov�

extern const long double pi;		// Ludolfovo ��slo
extern const long double pi2;		// Ludolfovo ��slo * 2
extern const long double eul;		// Eulerovo ��slo

extern const long double uhel22;	// �hel 22.5 stup�� v radi�nech
extern const long double uhel30;	// �hel 30 stup�� v radi�nech
extern const long double uhel45;	// �hel 45 stup�� v radi�nech
extern const long double uhel60;	// �hel 60 stup�� v radi�nech
extern const long double uhel67;	// �hel 67.5 stup�� v radi�nech
extern const long double uhel90;	// �hel 90 stup�� v radi�nech
extern const long double uhel135;	// �hel 135 stup�� v radi�nech
extern const long double uhel180;	// �hel 180 stup�� v radi�nech
extern const long double uhel225;	// �hel 215 stup�� v radi�nech
extern const long double uhel270;	// �hel 270 stup�� v radi�nech
extern const long double uhel315;	// �hel 315 stup�� v radi�nech

extern const long double ln10;		// p�irozen� logaritmus 10

extern const long double degrad;	// konstanta pro p�evod z DEG na RAD (pi/180)
extern const long double raddeg;	// konstanta pro p�evod z RAD na DEG (180/pi)


//////////////////////////////////////////////////////////////////////////////
// p��znaky typu ikony a obr�zku
enum PICPARAM {
	PicParamPic,			// pouze obr�zek bez pozad�
	PicParamMix,			// obr�zek mixovan� s pozad�m
	PicParamBack,			// pouze pozad� (pr�zdn� obr�zek)
	PicParamNone,			// nezn�m� obsah
	PicParamComp,			// komprimovan� data
};


//////////////////////////////////////////////////////////////////////////////
// ukon�en� aplikace

void	Exit(int code);					// ukon�en� programu

#define	EXITCODE_MEMERRINI	125			// chyba pam�ti p�i startu programu
#define	EXITCODE_MEMINIT	120			// chyba pam�ti p�i inicializaci
#define EXITCODE_MAINFRAME	115			// chyba vytvo�en� hlavn�ho okna
#define EXITCODE_LOADRES	110			// chyba �ten� resource

#define EXITCODE_INIT		100			// hranice inicializa�n�ch k�d�

#define	EXITCODE_MEMERR		80			// chyba pam�ti p�i b�hu programu
#define EXITCODE_OK			0			// ��dn� ukon�en� programu


/////////////////////////////////////////////////////////////////////////////
// test licence

BOOL TestLic();


//////////////////////////////////////////////////////////////////////////////
// vyst�ed�n� okna proti jin�mu oknu

void CenterWindow(HWND child, HWND parent = NULL);


//////////////////////////////////////////////////////////////////////////////
// spu�t�n� programu

int Exec(CText command, CText aktdir, BOOL wait);


/////////////////////////////////////////////////////////////////////////////
// generov�n� konverzn� tabulky barev obr�zku

void GenKonvPal(BITMAPINFO* bmp);


/////////////////////////////////////////////////////////////////////////////
// vygenerov�n� st�edn� barvy (ze 4 bod�)

BYTE ColAvrg(BYTE col1, BYTE col2, BYTE col3, BYTE col4);
BYTE ColAvrgDither(BYTE col1, BYTE col2, BYTE col3, BYTE col4, int x, int y);


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� edita�n�ho okna s fokusem (NULL = nen�)

HWND GetEditFocus();


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� adres��e (pokud existuje, je v�e OK)

BOOL CreateDir(CText txt);


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� adres��e (i v�cestup�ov� - v�etn� cesty)

BOOL CreatePath(CText txt);


/////////////////////////////////////////////////////////////////////////////
// dekomprese dat

void DeKomp(BYTE* dstBuf, int dstNum, BYTE* srcBuf, int srcNum);


/////////////////////////////////////////////////////////////////////////////
// gener�tor n�hody

int rand();
double Rand();

inline int sqr(int num) { return num*num; }

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


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� informac� o velikosti a voln�m m�stu disku

void GetDiskSpace(CText path);


/////////////////////////////////////////////////////////////////////////////
// m�d editace ikony (* = je v editoru ikony, + = je v editoru obr�zku))

enum EditMode
{ 
	EditModePen,					// * + pero
	EditModePaint,					// *   �t�tec

	EditModeLine,					// * + ��ra
	EditModeRect,					// * + obd�ln�k
	EditModeRectFill,				//   + vypln�n� obd�ln�k
	EditModeRound,					//   + kru�nice
	EditModeRoundFill,				// * + kruh
	EditModeElip,					//   + elipsa
	EditModeElipFill,				// * + ov�l

	EditModeWidth1,					//   + pero 1
	EditModeWidth2,					//   + pero 2
	EditModeWidth3,					//   + pero 3
	EditModeWidth5,					//   + pero 5
	EditModeWidth9,					//   + pero 9
	EditModeWidth20,				//   + pero 21

	EditModePipe,					// * + kap�tko
	EditModeFill,					// * + v�pl�
	EditModeSpray,					// * + sprej
	EditModeText,					//   + text

	EditModeZoomIn,					//   + m���tko zv�t�it
	EditModeZoomOut,				//   + m���tko zmen�it

	EditModeSelect,					// * + blok
	EditModeXFlip,					// * + horizont�ln� p�evr�cen�
	EditModeYFlip,					// * + vertik�ln� p�evr�cen�
	EditModeRRot,					// * + oto�en� R
	EditModeLRot,					// * + oto�en� L
	EditModeXCol,					// * + z�m�na barev
	EditModeSCol,					// * + nastaven� barvy

	EditModeMul,					// * + zv�t�en�
	EditModeDiv,					// * + zmen�en�

	EditMode45,						// * + oto�en� o 45 stup��
	EditMode30,						// * + oto�en� o 30 stup��
	EditMode22,						// * + oto�en� o 22.5 stup��
};


//////////////////////////////////////////////////////////////////////////////
// pomocn� konstanty

#define ICONWIDTH 32								// ���ka ikon
#define ICONHEIGHT 32								// v��ka ikon
#define ICONSIZE (ICONWIDTH*ICONHEIGHT)				// velikost ikon v bajtech

#define MOUSEINV -100000							// neplatn� sou�adnice my�i

//////////////////////////////////////////////////////////////////////////////
// vlastn� vno�en� sekce

#include "Memory.h"						// obsluha pam�ti
#include "Bitmap.h"						// obsluha bitmap a ikon
#include "Select.h"						// v�b�r souboru k editaci
#include "Buffer.h"						// �ablona pro buffery
#include "BufIcon.h"					// buffer ikon, data ikon
#include "BufInt.h"						// buffer cel�ch ��sel a logick�ch prom�nn�ch
#include "BufMap.h"						// buffer mapy plochy
#include "BufMus.h"						// buffer hudby
#include "BufPic.h"						// buffer obr�zk� (pozad�)
#include "JPEG.h"						// obsluha JPEG soubor�
#include "BufReal.h"					// buffer re�ln�ch ��sel
#include "BufSnd.h"						// buffer zvuku
#include "BufSprt.h"					// buffer sprajt�
#include "BufText.h"					// buffer text�, textov� �et�zce
#include "MainFrm.h"					// hlavn� okno aplikace


//////////////////////////////////////////////////////////////////////////////
// fonty

// definice parametr� fontu
class CFont
{
public:
	BOOL	Standard;					// p��znak standardn�ho fontu
	BOOL	Bold;						// p��znak tu�n�ho fontu
	BOOL	Italic;						// p��znak kurz�vy (pro standardn� font se ignoruje)
	BOOL	Underline;					// p��znak podtr�en� (pro standardn� font se ignoruje)
	BOOL	Strikeout;					// p��znak p�e�krtnut� (pro standardn� font se ignoruje)
	BOOL	Serif;						// p��znak pati�kov�ho p�sma (pro standardn� font se ignoruje)
	BOOL	Fixed;						// p��znak p�sma s pevnou rozte��
	int		Height;						// v��ka font�, 0=b�n� (pro standardn� font se ignoruje)
	int		Height0;					// skute�n� v��ka font� (vypo�ten� p�i vytv��en� fontu)
	double	Width;						// relativn� ���ka font�, 0=b�n� (pro standardn� font se ignoruje)
	int		Width0;						// skute�n� ���ka font� (vypo�ten� p�i vytv��en� fontu)
	CText	User;						// u�ivatelsky definovan� font (SERIF i FIXED nastaveny)

// konstruktor a destruktor
	inline CFont()
	{ 
		Standard = TRUE;				// p��znak standardn�ho fontu
		Bold = FALSE;					// p��znak tu�n�ho fontu
		Italic = FALSE;					// p��znak kurz�vy
		Underline = FALSE;				// p��znak podtr�en�
		Strikeout = FALSE;				// p��znak p�e�krtnut�
		Serif = FALSE;					// p��znak pati�ek
		Fixed = FALSE;					// p��znak pevn�ch rozte��
		Height = 0;						// v��ka font�
		Height0 = 15;					// skute�n� v��ka font�
		Width = 0;						// relativn� ���ka font�
		Width0 = 8;						// skute�n� ���ka font�
	};

	inline void Init()
	{ 
		Standard = TRUE;				// p��znak standardn�ho fontu
		Bold = FALSE;					// p��znak tu�n�ho fontu
		Italic = FALSE;					// p��znak kurz�vy
		Underline = FALSE;				// p��znak podtr�en�
		Strikeout = FALSE;				// p��znak p�e�krtnut�
		Serif = FALSE;					// p��znak pati�ek
		Fixed = FALSE;					// p��znak pevn�ch rozte��
		Height = 0;						// v��ka font�
		Height0 = 15;					// skute�n� v��ka font�
		Width = 0;						// relativn� ���ka font�
		Width0 = 8;						// skute�n� ���ka font�
		User.Init();					// u�ivatelsk� font
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

extern HFONT	FontBezny;				// b�n� font pro dialogy a stavov� ��dek
extern HFONT	FontBold;				// tu�n� font pro dialogy a nadpisy oken
extern HFONT	FontFixed;				// font s pevnou rozte�� pro dialogy

extern CText	SetupVzor;				// vzor textu pro nastaven� p�sma

extern CFont	SelectFont;				// font v�b�ru programu
extern CFont	TreeFont;				// font editoru programu
extern CFont	TextFont;				// font editoru textu
extern CFont	MenuFont;				// font nab�dky
extern CFont	StatusFont;				// font stavov� li�ty
extern CFont	GraphicFont;			// font grafick�ho editoru

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� fontu

HFONT GetFont(CFont* fnt);


/////////////////////////////////////////////////////////////////////////////
// zru�en� fontu

void DelFont(HFONT font);


//////////////////////////////////////////////////////////////////////////////
// nastaven� font� v okn�

void SetFontBezny(HWND wnd);
void SetFontBold(HWND wnd);
void SetFontFixed(HWND wnd);
void SetFontFixedBold(HWND wnd);


#include "BufProg.h"					// buffer programu
#include "BufInx.h"						// buffer index�
#include "BufUndo.h"					// buffer undo a redo
#include "EditIcon.h"					// editor ikon a obr�zk�
#include "EditLog.h"					// editor logick�ch hodnot
#include "EditMap.h"					// editor ploch
#include "EditMus.h"					// editor hudby
#include "EditNum.h"					// editor ��sel
#include "EditSnd.h"					// editor zvuk�
#include "EditSprt.h"					// editor sprajt�
#include "EditText.h"					// editor text�
#include "File.h"						// obsluha soubor�
#include "Compress.h"					// obsluha komprese dat
#include "Prog.h"						// obsluha editace souboru
#include "ProgDrag.h"					// ta�en� ikon programu
#include "ProgFile.h"					// obsluha soubor�
#include "ProgClip.h"					// obsluha clipboardu
#include "ProgExp.h"					// export a import programu
#include "ProgCol.h"					// v�b�r barvy
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
		((color & (63*4))/4) |							// �erven� slo�ka
		((color & (63*4*256))/(256*4/64)) |				// zelen� slo�ka
		((color & (63*4*256*256))/(256*256*4/64/64))];	// modr� slo�ka
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� registru DWORD (-1 = chyba)

//int GetRegUserDWORD(CText key, CText name);


/////////////////////////////////////////////////////////////////////////////
// nastaven� registru DWORD (nevytv��� kl��)

//void SetRegUserDWORD(CText key, CText name, int data);

