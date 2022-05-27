
#define WIN32_LEAN_AND_MEAN
#define INITGUID
#include "Main.h"

//////////////////////////////////////////////////////////////////////////////
// sekce programu Pet��ka

PETHEAD*	PetAdr = NULL;				// adresa dat programu
int			PetSize = 0;				// velikost dat programu
CBuf		PetBuf;						// buffer dat programu

char DATAsize[] =									// buffer import� od p�eklada�e
{
	"DATAsize"										// sem bude ulo�ena adresa a velikost dat, sou�asn� je to identifik�tor
};

//////////////////////////////////////////////////////////////////////////////
// definice jazyk�

int		JazykDef = JAZYKENG;			// implicitn� jazyk
DWORD	DefCharSet = DEFAULT_CHARSET;	// implicitn� znakov� sada font� podle syst�mu

int		JazykDefImp = 0;				// implicitn� importovan� jazyk

int		Jazyk = JAZYKENG;				// aktu�ln� jazyk
DWORD	LangID = 0x09;					// aktu�ln� identifik�tor jazyku
DWORD	CharSet = DEFAULT_CHARSET;		// aktu�ln� znakov� sada font�
DWORD	CodePage = CP_ACP;				// aktu�ln� k�dov� str�nka
char	Carka = '.';					// odd�lova� desetinn� ��rky p�i zobrazen� ��sel

int		JazykUkaz0 = 0;					// ukazatel indexu na��tan�ho textu
char*	JazykUkaz[JAZYKNUM];			// ukazatele text� p�i na��t�n�

JAZYKINFO JazykInfo[JAZYKNUM] =
{
// arabsky
	0x01,	1256,	ARABIC_CHARSET,	NULL,
										_T("Cancel"),	_T("All files"),
										_T("Insufficient Memory"),
										_T("Sorry, this program will be terminated due to the insufficient memory!"),
// bulharsky
	0x02,	1251,	RUSSIAN_CHARSET,	NULL,
										_T("Cancel"),	_T("All files"),
										_T("Insufficient Memory"),
										_T("Sorry, this program will be terminated due to the insufficient memory!"),
// �esky
	0x05,	1250,	EASTEUROPE_CHARSET,	NULL,
										_T("Storno"),	_T("V�echny soubory"),
										_T("Nedostatek pam�ti"),
										_T("Lituji, program bude ukon�en z d�vodu nedostatku pam�ti!"),
// d�nsky
	0x06,	1252,	ANSI_CHARSET,		NULL,
										_T("Cancel"),	_T("All files"),
										_T("Insufficient Memory"),
										_T("Sorry, this program will be terminated due to the insufficient memory!"),
// n�mecky
	0x07,	1252,	ANSI_CHARSET,		NULL,
										_T("Abbrechen"),_T("Alle Dateien"),
										_T("Speicherplatzmangel"),
										_T("Ich bedauere, das Programm wird wegen Speicherplatzmangel beendet!"),
// �ecky
	0x08,	1253,	GREEK_CHARSET,		NULL,
										_T("Cancel"),	_T("All files"),
										_T("Insufficient Memory"),
										_T("Sorry, this program will be terminated due to the insufficient memory!"),
// anglicky
	0x09,	1252,	ANSI_CHARSET,		NULL,
										_T("Cancel"),	_T("All files"),
										_T("Insufficient Memory"),
										_T("Sorry, this program will be terminated due to the insufficient memory!"),
// �pan�lsky
	0x0a,	1252,	ANSI_CHARSET,		NULL,
										_T("Cancel"),	_T("All files"),
										_T("Insufficient Memory"),
										_T("Sorry, this program will be terminated due to the insufficient memory!"),
// finsky
	0x0b,	1257,	BALTIC_CHARSET,		NULL,
										_T("Cancel"),	_T("All files"),
										_T("Insufficient Memory"),
										_T("Sorry, this program will be terminated due to the insufficient memory!"),
// francouzsky
	0x0c,	1252,	ANSI_CHARSET,		NULL,
										_T("Cancel"),	_T("All files"),
										_T("Insufficient Memory"),
										_T("Sorry, this program will be terminated due to the insufficient memory!"),
// hebrejsky
	0x0d,	1255,	HEBREW_CHARSET,		NULL,
										_T("Cancel"),	_T("All files"),
										_T("Insufficient Memory"),
										_T("Sorry, this program will be terminated due to the insufficient memory!"),
// ma�arsky
	0x0e,	1250,	EASTEUROPE_CHARSET,	NULL,
										_T("Cancel"),	_T("All files"),
										_T("Insufficient Memory"),
										_T("Sorry, this program will be terminated due to the insufficient memory!"),
// islandsky
	0x0f,	1257,	BALTIC_CHARSET,		NULL,
										_T("Cancel"),	_T("All files"),
										_T("Insufficient Memory"),
										_T("Sorry, this program will be terminated due to the insufficient memory!"),
// italsky
	0x10,	1252,	ANSI_CHARSET,		NULL,
										_T("Cancel"),	_T("All files"),
										_T("Insufficient Memory"),
										_T("Sorry, this program will be terminated due to the insufficient memory!"),
// holansky
	0x13,	1252,	ANSI_CHARSET,		NULL,
										_T("Cancel"),	_T("All files"),
										_T("Insufficient Memory"),
										_T("Sorry, this program will be terminated due to the insufficient memory!"),
// norsky
	0x14,	1257,	BALTIC_CHARSET,		NULL,
										_T("Cancel"),	_T("All files"),
										_T("Insufficient Memory"),
										_T("Sorry, this program will be terminated due to the insufficient memory!"),
// polsky
	0x15,	1250,	EASTEUROPE_CHARSET,	NULL,
										_T("Anuluj"),	_T("Wszystkie pliki"),
										_T("Brak pami�ci"),
										_T("�a�uj�, program b�dzie zako�czeny z powodu braku pami�ci!"),
// portugalsky
	0x16,	1252,	ANSI_CHARSET,		NULL,
										_T("Cancel"),	_T("All files"),
										_T("Insufficient Memory"),
										_T("Sorry, this program will be terminated due to the insufficient memory!"),
// rumunsky
	0x18,	1251,	RUSSIAN_CHARSET,	NULL,
										_T("Cancel"),	_T("All files"),
										_T("Insufficient Memory"),
										_T("Sorry, this program will be terminated due to the insufficient memory!"),
// rusky
	0x19,	1251,	RUSSIAN_CHARSET,	NULL,
										_T("Cancel"),	_T("All files"),
										_T("Insufficient Memory"),
										_T("Sorry, this program will be terminated due to the insufficient memory!"),
// srbochorv�tsky
	0x1a,	1250,	EASTEUROPE_CHARSET,	NULL,
										_T("Cancel"),	_T("All files"),
										_T("Insufficient Memory"),
										_T("Sorry, this program will be terminated due to the insufficient memory!"),
// slovensky
	0x1b,	1250,	EASTEUROPE_CHARSET,	NULL,
										_T("Storno"),	_T("V�etky s�bory"),
										_T("Nedostatok pam�ti"),
										_T("�utujem, program bude ukon�en� z d�vodu nedostatku pam�ti!"),
// alb�nsky
	0x1c,	1253,	GREEK_CHARSET,		NULL,
										_T("Cancel"),	_T("All files"),
										_T("Insufficient Memory"),
										_T("Sorry, this program will be terminated due to the insufficient memory!"),
// �v�dsky
	0x1d,	1257,	BALTIC_CHARSET,		NULL,
										_T("Cancel"),	_T("All files"),
										_T("Insufficient Memory"),
										_T("Sorry, this program will be terminated due to the insufficient memory!"),
// turecky
	0x1f,	1254,	TURKISH_CHARSET,	NULL,
										_T("Cancel"),	_T("All files"),
										_T("Insufficient Memory"),
										_T("Sorry, this program will be terminated due to the insufficient memory!"),
// vietnamsky
	0x2a,	1258,	VIETNAMESE_CHARSET,	NULL,
										_T("Cancel"),	_T("All files"),
										_T("Insufficient Memory"),
										_T("Sorry, this program will be terminated due to the insufficient memory!"),
};

LPCTSTR ProductKey0 = _T("Software\\Gemtree Software\\Peter");

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

CString		CommandLine;				// p��kazov� ��dek
int			VerzeOS;					// verze syst�mu
OSVERSIONINFO	OSVersionInfo;			// informace o syst�mu
HINSTANCE	hInstance = NULL;			// instance programu
int			ScreenWidth;				// ���ka klientsk� oblasti displeje (t� v re�imu DirectDraw)
int			ScreenHeight;				// v��ka klientsk� oblasti displeje (t� v re�imu DirectDraw)
int			ScreenWidth0;				// ���ka videom�du displeje
int			ScreenHeight0;				// v��ka videom�du displeje
int			ScreenBits;					// po�et bit� na bod obrazovky
int			TimerConst;					// konstanta pro �asova� (pro 55 ms)

HINSTANCE	KernLibrary = NULL;			// handle modulu KERNEL32.DLL

DWORD		ReturnCode = EXITCODE_OK;	// n�vratov� k�d (pro operaci OK)

BYTE*		StdPalImport;				// tabulka importu barev Windows na vlastn� barvy
BITMAPINFO* StdBitmapInfo;				// standardn� z�hlav� BMP
HPALETTE	StdPalette = NULL;			// vlastn� logick� palety
LOGPALETTE* pStdPalette = NULL;			// tabulka vlastn�ch logick� palet (pot�ebn� t� pro DirectDraw)

//#ifndef _MINI
//D3DRMPALETTEENTRY* pStdPalette2 = NULL;	// tabulka palet s alfa kan�lem (pro Direct3D)
//#endif // _MINI

BYTE*		KonvPal;					// konverzn� tabulka palet pro import soubor�

PALETTEENTRY* SysPal = NULL;			// tabulka palet pro DirectDraw 256 barev

DWORD*		StdPalExport;				// tabulka exportu vlastn�ch barev na barvy Windows

HWND		PrevWindow = NULL;			// p�ede�l� aktivn� okno

bool		Dither = true;				// pou��vat dithering

bool		Release = false;			// je distribu�n� verze

BYTE*		Shadows;					// tabulka pro generov�n� st�nu

CString		ExeFileName;				// jm�no tohoto programu

GETDISKFREESPACEEX	pGetDiskFreeSpaceEx = NULL;	// funkce GetDiskFreeSpaceEx (NULL=nen�)

//////////////////////////////////////////////////////////////////////////////
// pr�zdn� objekty (nap�. pro n�vrat neplatn� polo�ky z funkce)

CString		EmptyString;				// pr�zdn� �et�zec
CIcon		EmptyIcon;					// pr�zdn� ikona
CMap		EmptyMap;					// pr�zdn� plocha
CPicture	EmptyPicture;				// pr�zdn� obr�zek
CSprite		EmptySprite;				// pr�zdn� sprajt
CSound		EmptySound;					// pr�zdn� zvuk
CMusic		EmptyMusic;					// pr�zdn� hudba

/////////////////////////////////////////////////////////////////////////////
// buffery

int			TextListBeg;							// index po��tku glob�ln�ch seznam�
int			TextListEnd;							// index konce glob�ln�ch seznam�

//CBufProg	BufProg[PROGBUFNUM];					// (0 a� 5) buffery programu (0=intern�)
CBufReal	Real;									// buffer re�ln�ch ��sel
CBufText	Text;									// buffer text�
CBufBool	Bool;									// buffer logick�ch prom�nn�ch
CBufIcon	Icon;									// buffer ikon
CBufMap		Map;									// buffer ploch
CBufPic		Picture;								// buffer obr�zk�
CBufSprite	Sprite;									// buffer sprajt�
CBufSound	Sound;									// buffer zvuk�
CBufMusic	Music;									// buffer hudby
CBufList	List;									// buffer seznam�
CBufWin		Win;									// buffer prvk� oken

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

//CBufD3DO	D3DO;									// buffer objekt� Direct3D
//CBufD3DF	D3DF;									// buffer r�m� Direct3D

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------

//////////////////////////////////////////////////////////////////////////////
// konstanty

const char*	Compiled =	"Gemtree 32-compiler"; // jen tak pro zmaten�

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

//const int ColLev = sizeof(ColLevTab)/sizeof(ColLevTab[0]);	// po�et hladin barev
//const int ColCol = sizeof(ColColTab)/sizeof(ColColTab[0]);	// po�et odst�n� barev
//const int StdColors = ResCols + ColCol*ColLev;				// po�et vlastn�ch palet (za��naj� od 0)
const BYTE WhiteCol = StdColors - 2*ColLev;			// b�l� barva
const BYTE BlackCol = StdColors - 1;				// �ern� barva

const BYTE BlueCol = 22*ColLev + ResCols;			// modr� barva
const BYTE RedCol = 0*ColLev + ResCols;				// �erven� barva
const BYTE YellowCol = 9*ColLev + ResCols;			// �lut� barva
const BYTE GreenCol = 13*ColLev + ResCols;			// zelen� barva
const BYTE LtYellowCol = 11*ColLev + ResCols;		// sv�tle �lut� barva
const BYTE OrangeCol = 7*ColLev + ResCols;			// oran�ov�

const double pi  = 3.14159265358979323846;			// Ludolfovo ��slo
const double pi2 = 6.28318530717958647693;			// Ludolfovo ��slo * 2
const double pi8  = 25.13274122871834590768;		// Ludolfovo ��slo * 8
const double pi2m = -6.28318530717958647693;		// - Ludolfovo ��slo * 2
const double pi8m  = -25.13274122871834590768;		// - Ludolfovo ��slo * 8
const double eul = 2.71828182845904523536;			// Eulerovo ��slo

const double uhel22  = 0.39269908169872415481;		// �hel 22.5 stup�� v radi�nech
const double uhel30  = 0.52359877559829887308;		// �hel 30 stup�� v radi�nech
const double uhel45  = 0.78539816339744830962;		// �hel 45 stup�� v radi�nech
const double uhel60  = 1.04719755119659774615;		// �hel 60 stup�� v radi�nech
const double uhel67  = 1.17809724509617246442;		// �hel 67.5 stup�� v radi�nech
const double uhel90  = 1.57079632679489661923;		// �hel 90 stup�� v radi�nech
const double uhel135 = 2.35619449019234492885;		// �hel 135 stup�� v radi�nech
const double uhel180 = 3.14159265358979323846;		// �hel 180 stup�� v radi�nech
const double uhel225 = 3.92699081698724154808;		// �hel 225 stup�� v radi�nech
const double uhel270 = 4.71238898038468985769;		// �hel 270 stup�� v radi�nech
const double uhel315 = 5.49778714378213816731;		// �hel 315 stup�� v radi�nech

const double ln10 = 2.3025850929940456840;			// p�irozen� logaritmus 10

const double degrad = 0.017453292519943295769237;	// konstanta pro p�evod z DEG na RAD (pi/180)
const double raddeg = 57.2957795130823208768;		// konstanta pro p�evod z RAD na DEG (180/pi)

double* FactTab;									// tabulka faktori�l�

//////////////////////////////////////////////////////////////////////////////
// lok�ln� prom�nn�

#ifdef _MT
static	CRITICAL_SECTION	ExitCriticalSection;	// kritick� sekce pro konec programu
#endif

unsigned	RandomSeed;					// prom�nn� pro gener�tor n�hody


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
// inicializace jazyku

void InitLang(int jazyk)
{
	Jazyk = jazyk;
	LangID = JazykInfo[jazyk].LangID;
	CharSet = JazykInfo[jazyk].CharSet;
	CodePage = JazykInfo[jazyk].CodePage;

// desetinn� te�ka/��rka (te�ku pouz�v� pouze USA)
	if (jazyk == JAZYKENG)
	{
		Carka = '.';
	}
	else
	{
		Carka = ',';
	}

// implicitn� jazyk pro import
	switch (jazyk)
	{
	case JAZYKCZ:
		JazykDefImp = JAZYKSLO;
		break;

	case JAZYKSLO:
	case JAZYKPOL:
		JazykDefImp = JAZYKCZ;
		break;

	default:
		JazykDefImp = JAZYKENG;
	}
}


//////////////////////////////////////////////////////////////////////////////
// inicializace kl�vesnice

void InitKeyb()
{

/* JAKJAKJAK 051222

	CString keyid;
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
// aktualizace informac� o obrazovce (nen�-li celoobrazovkov� m�d)

void AktDispInfo0()
{
	ScreenWidth = ::GetSystemMetrics(SM_CXFULLSCREEN);
	ScreenHeight = ::GetSystemMetrics(SM_CYFULLSCREEN);
	ScreenWidth0 = ::GetSystemMetrics(SM_CXSCREEN);
	ScreenHeight0 = ::GetSystemMetrics(SM_CYSCREEN);

	HDC dc = ::GetDC(MainFrame);
	if (dc)
	{
		ScreenBits = ::GetDeviceCaps(dc, BITSPIXEL);
		::ReleaseDC(MainFrame, dc);
	}
}

void AktDispInfo()
{
	if (!FullScreen) AktDispInfo0();
}

//////////////////////////////////////////////////////////////////////////////
// hlavn� start programu

void WinMainCRTStartup()
{
// �schova verze syst�mu
	VerzeOS = (int)::GetVersion();
	OSVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx(&OSVersionInfo);

// handle instance programu
	hInstance = ::GetModuleHandle(NULL);

// detekce implicitn� znakov� sady
	int acp = ::GetACP();

	switch (acp)
	{
	case 1250:
		DefCharSet = EASTEUROPE_CHARSET;
		goto ACPSET;

	case 1251:
		DefCharSet = RUSSIAN_CHARSET;
		break;

	case 1252:
		DefCharSet = ANSI_CHARSET;
		break;

	case 1253:
		DefCharSet = GREEK_CHARSET;
		goto ACPSET;

	case 1254:
		DefCharSet = TURKISH_CHARSET;
		goto ACPSET;

	case 1255:
		CharSet = HEBREW_CHARSET;
		break;

	case 1256:
		DefCharSet = ARABIC_CHARSET;
ACPSET:
		JazykInfo[JAZYKENG].CodePage = acp;
		JazykInfo[JAZYKENG].CharSet = DefCharSet;
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

// zji�t�n� aktu�ln�ho jazyku
	{
		DWORD num = 0;
		HKEY hkey;

		if (::RegOpenKeyEx(
			HKEY_LOCAL_MACHINE,
			ProductKey0,
			0,
			KEY_QUERY_VALUE,
			&hkey) == ERROR_SUCCESS)
		{
			DWORD type = REG_DWORD;
			DWORD bufsize = 4;

			::RegQueryValueEx(
				hkey,
				_T("Language"),
				0,
				&type,
				(BYTE*)&num,
				&bufsize);

			::RegCloseKey(hkey);
		}

		if (num == 0)
		{
			num = (::GetUserDefaultLangID() & 0xff);
		}

		for (int ll = 0; ll < JAZYKNUM; ll++)
		{
			if (JazykInfo[ll].LangID == (int)num)
			{
				JazykDef = ll;
				break;
			}
		}

		InitLang(JazykDef);
	}

// inicializace funkce GetDiskFreeSpaceEx pro OSR2
	if (((OSVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) &&
		(LOWORD(OSVersionInfo.dwBuildNumber) > 1000)) ||
		(OSVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT))
	{
		KernLibrary = ::LoadLibrary(_T("KERNEL32.DLL"));

		if (KernLibrary != NULL)
		{
#ifdef _UNICODE
			pGetDiskFreeSpaceEx = (GETDISKFREESPACEEX)
				::GetProcAddress(KernLibrary,	"GetDiskFreeSpaceExW");
#else
			pGetDiskFreeSpaceEx = (GETDISKFREESPACEEX)
				::GetProcAddress(KernLibrary,	"GetDiskFreeSpaceExA");
#endif
		}
	}

// �schova p�ede�l�ho aktivn�ho okna
	PrevWindow = ::GetForegroundWindow();

// inicializace informac� o displeji
	AktDispInfo();
	MainFrameX = ScreenWidth/2;
	MainFrameY = ScreenHeight/2;

// inicializace uzamyk�n� ukon�en� programu
#ifdef _MT
	::InitializeCriticalSection(&ExitCriticalSection);
#endif

// inicializace spr�vce pam�ti
	if (!MemInit()) 
	{
		Exit(EXITCODE_MEMERR);
	}

// p��prava �asova�e
	if (VerzeOS > 0) 
		TimerConst = 60;			// �asova� pro NT
	else
		TimerConst = 30;			// �asova� pro 95

// inicializace gener�toru n�hory
	SYSTEMTIME systime;
	::GetLocalTime(&systime);
	unsigned time = systime.wMilliseconds + 137*systime.wSecond + 2567*systime.wMinute + 358946*systime.wDay;
	RandomSeed = time + 486874*time;	// inicializace gener�toru n�hody

// inicializace koprocesoru
	FPUDouble();

// inicializace dat pr�zdn� ikony
	EmptyIconData.Data = (BYTE*)MemGet(ICONSIZE);
	MemFill(EmptyIconData.Data, ICONSIZE, BackCol);
	EmptyPictureData.Data = (BYTE*)MemGet(ICONSIZE);
	MemFill(EmptyPictureData.Data, ICONSIZE, BackCol);

// inicializace obsluhy sprajt�
	InitSprite();

// inicializace obsluhy ploch
	InitMap();

// inicializace glob�ln�ch objekt�
	InitTerm(__xc_a, __xc_z);

// na�ten� jm�na tohoto programu
	TCHAR buf[_MAX_PATH];
	::GetModuleFileName(hInstance, buf, _MAX_PATH);
	ExeFileName = buf;

// inicializace standardn�ho z�hlav� BMP
	StdBitmapInfo = (BITMAPINFO*) MemGet(sizeof(BITMAPINFO) + sizeof(RGBQUAD)*255);
	MemFill(StdBitmapInfo, sizeof(BITMAPINFO) + sizeof(RGBQUAD)*255);	// vynulov�n�
	StdBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);	// velikost z�hlav�
	StdBitmapInfo->bmiHeader.biWidth = ICONWIDTH;	// ���ka bitmapy
	StdBitmapInfo->bmiHeader.biHeight = ICONHEIGHT;	// v��ka bitmapy
	StdBitmapInfo->bmiHeader.biPlanes = 1;			// po�et barevn�ch rovin
	StdBitmapInfo->bmiHeader.biBitCount = 8;		// po�et bit� na bod
	StdBitmapInfo->bmiHeader.biCompression = BI_RGB; // nen� komprese
	StdBitmapInfo->bmiHeader.biClrImportant = StdColors; // po�et d�le�it�ch palet

// vytvo�en� standardn�ch palet
	RGBQUAD* rgb = StdBitmapInfo->bmiColors;		// za��tek tabulky palet

// nem�nit paletu barvu pozad� - pou��v� se p�i importu obr�zk�
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

// vygenerov�n� tabulky pro import palet Windows do vlatn�ch barev
	StdPalImport = (BYTE*)MemGet(64 * 64 * 64);		// tabulka pro import palet

// na�ten� tabulky importu palet
	HRSRC hRes = ::FindResource(hInstance, MAKEINTRESOURCE(IDN_PALIMP), _T("LOADER"));
	HGLOBAL hData = ::LoadResource(hInstance, hRes);
	ASSERT((hRes != NULL) && (hData != NULL));
	if ((hRes == NULL) || (hData == NULL)) Exit(EXITCODE_LOADRES);
	DeKomp(StdPalImport, 64*64*64, (BYTE*)::LockResource(hData)+6, ::SizeofResource(hInstance, hRes)-6);

// vytvo�en� konverzn� tabulky palet pro �mport soubor� BMP
	KonvPal = (BYTE*)MemGet(256);			// konverzn� tabulka palet

// vytvo�en� vlastn�ch logick�ch palet
	pStdPalette = (LOGPALETTE*) MemGet(sizeof(PALETTEENTRY)*(256-1)+sizeof(LOGPALETTE));
	MemFill(pStdPalette, sizeof(PALETTEENTRY)*(256-1)+sizeof(LOGPALETTE), 0);
	pStdPalette->palVersion = 0x300;
	pStdPalette->palNumEntries = StdColors;
	PALETTEENTRY* pal = pStdPalette->palPalEntry;
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

// palety pro Direct3D s alfa kan�lem
#ifndef _MINI
//	pStdPalette2 = (D3DRMPALETTEENTRY*) MemGet(sizeof(D3DRMPALETTEENTRY)*256);
//	MemCopy(pStdPalette2, pStdPalette->palPalEntry, sizeof(D3DRMPALETTEENTRY)*256);
//	for (i = 0; i < 256; i++)
//	{
//		pStdPalette2[i].flags = 0xff;
//	}
//	pStdPalette2[BackCol].flags = 0;
//	pStdPalette2[ShadCol].flags = 0x7f;
#endif // _MINI

// vygenerov�n� tabulky pro export vlastn�ch barev do palet Windows BGR
	StdPalExport = (DWORD*)MemGet(256*sizeof(DWORD));
	DWORD* exppal = StdPalExport;
	rgb = StdBitmapInfo->bmiColors;

	for (i = 0; i < 256; i++)
	{
		*exppal = *(DWORD*)rgb;

		BYTE c = ((BYTE*)exppal)[0];
		((BYTE*)exppal)[0] = ((BYTE*)exppal)[2];
		((BYTE*)exppal)[2] = c;
		((BYTE*)exppal)[3] = 0;

		if (i == BackCol) *exppal = 0xff000000;
		if (i == ShadCol) *exppal = 0x70000000;
		if (i >= StdColors) *exppal = 0;

		exppal++;
		rgb++;
	}

// vygenerov�n� tabulky faktori�l� (posledn� hodnota = p�ete�en�)
	FactTab = (double*)MemGet((MAXFACT+2) * sizeof(double));
	FactTab[0] = 0;
	FactTab[1] = 1;
	for (i = 2; i <= MAXFACT+1; i++)
	{
		FactTab[i] = FactTab[i-1] * i;
	}

// vygenerov�n� tabulky st�n�
	Shadows = (BYTE*)MemGet(256);
	for (i = 0; i < 256; i++)
	{
		int col;

		if ((i >= StdColors) ||
			(i < ResCols) ||
			((((i - ResCols + 1) % ColLev) == 0) &&
			(i != (StdColors - ColLev))))
		{
			col = BlackCol;
		}
		else
		{
			col = i+1;
			if ((col > WhiteCol) && (col < BlackCol)) col++;
		}

		Shadows[i] = (BYTE)col;
	}

// �schova p��kazov�ho ��dku
	CommandLine = ::GetCommandLine();

// inicializace bufferu dat
	PetAdr = (PETHEAD*)((DWORD)hInstance + *(DWORD*)DATAsize);
	PetSize = *((long*)DATAsize + 1);

#if defined(_DEBUG)
//	&& !defined(_OPTIM)
	CFileMap file;
	VERIFY(file.Open(_T("C:\\VC\\Peter\\Program\\Peter.exe")) ||
			file.Open(_T("D:\\VC\\Peter\\Program\\Peter.exe")) ||
			file.Open(_T("E:\\VC\\Peter\\Program\\Peter.exe")));
	IMAGE_NT_HEADERS* hdr = file.NTHeader();
	ASSERT(hdr);
	IMAGE_SECTION_HEADER* sec = file.NTSection(hdr, ".petprg");
	if (sec == NULL) sec = file.NTSection(hdr, ".rtexts");
	ASSERT(sec);
	PetAdr = (PETHEAD*)(file.Adr() + sec->PointerToRawData);
	PetSize = sec->SizeOfRawData;
	ASSERT(file.IsValid(PetAdr, PetSize));
#endif

	PetBuf.Adr((BYTE*)PetAdr);
	PetBuf.Size(PetSize);
	if (PetSize < 500) Exit(EXITCODE_LOADRES);

	if (*(DWORD*)PetAdr == '\1LER')
	{
		Release = true;
	}
	else
	{
		if (*(DWORD*)PetAdr == '\1TEP')
		{
			Release = false;
		}
		else
		{
			Exit(EXITCODE_LOADRES);
		}
	}

// inicializace obsluhy UDP (inicializace buffer�)
#ifndef _MINI
	InitUDP();
#endif // _MINI

// na�ten� programu
	Load();

// kompilace programu
	Comp();

// vytvo�en� hlavn�ho okna aplikace
	if (!MainFrameCreate()) 
	{
		Exit(EXITCODE_MAINFRAME);
	}

// inicializace kl�vesnice (ne pro angli�tinu)
	if (LangID != 9)
	{
		InitKeyb();
	}

// prov�d�n� programu
	Exec();

#if defined(_DEBUG) && !defined(_OPTIM)
	file.Close();
#endif

// konec programu
	Exit(ReturnCode);
}


//void mainCRTStartup()
//{
//	WinMainCRTStartup();
//}


//////////////////////////////////////////////////////////////////////////////
// ukon�en� programu

void Exit(DWORD code)
{
// uzamknut� ukon�en� programu
#ifdef _MT
	::EnterCriticalSection(&ExitCriticalSection);
#endif

// zru�en� palet
	::DeleteObject(StdPalette);

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI
	
// uvoln�n� knihovny COM
	COMUninit();

// uvoln�n� knihovny WSA
	WSADeinitialize();
#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------

	if (KernLibrary != NULL)
	{
		::FreeLibrary(KernLibrary);
	}

// ukon�en� spr�vce pam�ti
	MemTerm();

// uvoln�n� uzamyk�n� ukon�en� programu
#ifdef _MT
	::DeleteCriticalSection(&ExitCriticalSection);
#endif

// nastaven� fokusu na p�ede�l� okno
	if (AppActive)
	{
		::SetForegroundWindow(PrevWindow);
	}

// ukon�en� programu
	::ExitProcess(code);
}


//////////////////////////////////////////////////////////////////////////////
// chyba pro DEBUG OPTIM m�d

#if defined(_OPTIM) && defined(_M_IX86) && defined(_DEBUG)
void debugBreak(char* file, int line, char* date)
{
	CString txt("Intern� chyba interpreteru PETER. Nahlaste, pros�m, dodavateli n�sleduj�c�\n"
			  "�daje a popi�te situaci, za kter� tato chyba nastala. D�kujeme za pochopen�.\n"
			   "\n     soubor = X");

	CString txt2(file);
	txt += txt2.Right(txt2.Length() - txt2.RevFind('\\') - 1);
	txt += "           ��dek = ";
	txt2.Int(line);
	txt += txt2;
	txt += "           verze = ";
	txt += date;
	txt += '\n';

	::MessageBox(
		NULL,
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
// nastaven� vy��� p�esnosti procesoru

// P�i startu programu: CTRL = 0x027F (53 bit�)
// Vlastn� p�esnost: CTRL = 0x037F (64 bit�)
// Knihovna Direct3D: CTRL = 0x007F (p�esnost float)

// Podle doporu�en� DirectX7:
//		and ah,not 3 .... p�esnost single
//		or  al,0x3f ..... zak�zat v�jimky
//		and ah,not 0xc0 . zaokrouhlov�n� k nejbli���mu

void FPUDouble()
{

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

}

//////////////////////////////////////////////////////////////////////////////
// nastaven� ni��� p�esnosti procesoru

void FPUFloat()
{

#ifdef _M_IX86

	WORD stat;
	_asm {
		wait						// synchronizace
		fnstcw		stat			// ulo�en� ��dic�ho slova
		wait						// synchronizace
		mov			ax,stat			// stav ��dic�ho slova
		and			ah,not 0xf		// implicitn� zaokrouhlov�n�
		mov			stat,ax			// nov� stav ��dic�ho slova
		fldcw		stat			// nastaven� nov�ho ��dic�ho slova
	}
#endif

}

//////////////////////////////////////////////////////////////////////////////
// spu�t�n� programu

DWORD Exec(CString command, BOOL wait)
{
	DWORD result = 0;
	command.TrimLeft();
	if (command.IsEmpty()) return 0;

// inicializace startovac� tabulky
	STARTUPINFO si;
	MemFill(&si, sizeof(si));
	si.cb = sizeof(si);

// inicializace tabulky procesu
	PROCESS_INFORMATION pi;
	MemFill(&pi, sizeof(pi));

// spu�t�n� programu
	if (
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
		| (wait ? 0 : DETACHED_PROCESS)
		| CREATE_DEFAULT_ERROR_MODE,
		NULL,								// prost�ed�
		NULL,								// v�choz� adres��
		&si,								// adresa STARTUPINFO
		&pi) == 0)							// adresa PROCESS_INFORMATION
	{
		if ((command.Length() == 2) && (command[1] == ':'))
		{
			if (::SetCurrentDirectory(command))
			{
				return 0;
			}
			else
			{
				FileError = true;
				return 1;
			}
		}

		if ((command.Length() > 2) && ((command[0] == 'c') || (command[0] == 'C'))
			&& ((command[1] == 'd') || (command[1] == 'D')) &&
			((command[2] == ' ') || (command[2] == 9) || 
			 (command[2] == '.') || (command[2] == '\\')))
		{
			command.Delete(0, 2);
			command.TrimLeft();
			command.TrimRight();
			if (::SetCurrentDirectory(command))
			{
				return 0;
			}
			else
			{
				FileError = true;
				return 1;
			}
		}

		TCHAR* buf = (TCHAR*)MemGet(1024*sizeof(TCHAR));
		int len = ::GetWindowsDirectory(buf, 1024);
		CString windir(buf, len);

		if (VerzeOS > 0)					// NT
		{

			command = windir + _T("\\system32\\cmd.exe /c") + command;
		}
		else
		{
			command = windir + _T("\\command.com /c") + command;
		}
		MemFree(buf);
		
		if (
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
			| (wait ? 0 : DETACHED_PROCESS)
			| CREATE_DEFAULT_ERROR_MODE,
			NULL,								// prost�ed�
			NULL,								// v�choz� adres��
			&si,								// adresa STARTUPINFO
			&pi) == 0)							// adresa PROCESS_INFORMATION
		{
			FileError = true;
			return 1;
		}
	}

// �ek�n� na ukon�en� programu
	if (wait)
	{
		::WaitForSingleObject(pi.hProcess, INFINITE);	// �ek�n� na ukon�en� procesu
		::GetExitCodeProcess(pi.hProcess, &result);		// zji�t�n� n�vratov�ho k�du
	}

// uzav�en� handle procesu
	::CloseHandle(pi.hProcess);				// uzav�en� handle procesu
	::CloseHandle(pi.hThread);				// uzav�en� handle toku

	return result;
}

/*
int Exec(CString name, CString param, BOOL wait)
{
	int result = 0;

// p��prava jm�na programu
	name.TrimLeft();
	name.TrimRight();
	CString name0 = name;
	bool uvoz = false;

	for (int i = name.Length()-1; i >= 0; i--)
	{
		if (name[i] == _T('"'))
		{
			name = name.Left(i) + _T('"') + name.Right(name.Length() - i);
			uvoz = true;
		}

		if ((name[i] == _T(' ')) || (name[i] == 9)) uvoz = true;
	}
	if (uvoz) name = _T('"') + name + _T('"');

// p��prava p��kazov�ho ��dku
	TCHAR* commandline = (TCHAR*)MemGet(4096 * sizeof(TCHAR));
	TCHAR* dst = commandline;

	int len = name.Length();
	if (len > 2000) len = 2000;
	MemCopy(dst, (LPCTSTR)name, len * sizeof(TCHAR));
	dst += len;

	*dst = _T(' ');
	dst++;

	len = param.Length();
	if (len > 2000) len = 2000;
	MemCopy(dst, (LPCTSTR)param, len * sizeof(TCHAR));
	dst += len;

	*dst = 0;

// inicializace startovac� tabulky
	STARTUPINFO si;
	MemFill(&si, sizeof(si));
	si.cb = sizeof(si);

// inicializace tabulky procesu
	PROCESS_INFORMATION pi;
	MemFill(&pi, sizeof(pi));

// spu�t�n� programu
	::CreateProcess(
		name0,								// jm�no programu
		commandline,						// p��kazov� ��dek
		NULL,								// ochrann� atributy procesu
		NULL,								// ochrann� atributy vl�kna
		FALSE,								// d�di�nost handl�
#ifdef _UNICODE
		CREATE_UNICODE_ENVIRONMENT			// parametry
#else
		0
#endif
		| DETACHED_PROCESS,
		NULL,								// prost�ed�
		NULL,								// v�choz� adres��
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

	MemFree(commandline);

	return result;
}
*/

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
}


/////////////////////////////////////////////////////////////////////////////
// vygenerov�n� st�edn� barvy (ze 4 bod�)

BYTE _fastcall ColAvrg(BYTE col1, BYTE col2, BYTE col3, BYTE col4)
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
// dekomprese dat

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

/*
/////////////////////////////////////////////////////////////////////////////
// mocnina X^Y

static double NumConst1 = 1;

#pragma warning ( disable: 4035)	// hl�en� - nen� n�vratov� hodnota
#pragma warning ( disable: 4725)	// hl�en� - probl�mov� instrukce

double Power(double x, double y)
{
#ifdef _M_IX86


// lok�ln� prom�nn�
	WORD		oldstat;			// star� stav ��dic�ho slova
	WORD		newstat;			// nov� stav ��dic�ho slova - zaokrouhlen� k nule
	int			mocn;				// celo��seln� ��st mocnitele

	_asm {

// p��prava ��dic�ho slova pro zaokrouhlen� k nule
		wait						// synchronizace
		fnstcw		oldstat			// ulo�eni ��dic�ho slova
		wait						// synchronizace
		mov			ax,oldstat		// star� stav ��dic�ho slova
		or			ah,0xc			// m�d zaokrouhlen� sm�rem k nule
		mov			newstat,ax		// nov� stav ��dic�ho slova

// p��prava celo��seln� ��sti mocnitele (mocnitel z�stane v z�sobn�ku)
		fldcw		newstat			// nastaven� nov�ho ��dic�ho slova
		fld			y				// na�ten� mocnitele
		fist		mocn			// ulo�en� celo��seln� ��sti mocnitele
		fldcw		oldstat			// navr�cen� p�vodn�ho ��dic�ho slova

// test, zda je mocnitel celo��seln�
		ficom		mocn			// porovn�n� s celo��selnou ��st�
		fnstsw		ax				// na�ten� stavov�ho slova
		test		ah,0x40			// je mocnitel roven celo��seln�mu ��slu?
		jz			P1				// nen� to cel� ��slo

// uvoln�n� mocnitele z vrcholu z�sobn�ku
		fstp		st(0)			// zru�en� ��sla na vrcholu z�sobn�ku

// p��prava mocnitele
		mov			eax,mocn		// celo��seln� ��st mocnitele
		or			eax,eax			// je mocnitel z�porn�?
		jns			M1				// mocnitel nen� z�porn�
		neg			eax				// oprava znam�nka mocnitele

// p��prava ��sla a st�ada�e, test zda je mocnitel = 0
M1:		fld1						// na�ten� 1 jako st�ada� -> ST(0)
		or			eax,eax			// je ji� cel� mocnitel?
		jz			M5				// je ji� cel� mocnitel
		fld			x				// na�ten� mocn�nce -> ST(0), 1 -> ST(1)
		jmp			M3

// p�id�n� jednoho prvku
M2:   	fmul		st(0), st(0)	// p��prava dal��ho prvku
M3:		shr			eax,1			// uplatn� se sou�initel?
		jnc			M4				// neuplatn� se
		fmul		st(1), st(0)	// vyn�soben� prvkem

// kontrola, zda je v�po�et ji� ukon�en
M4:		or			eax,eax			// je ji� v�po�et ukon�en?
		jnz			M2				// bude dal�� prvek
		fstp		st(0)			// zru�en� prvku v ST(0)		
	
// pro z�porn� mocnitel p�evr�cen� hodnota v�sledku

M5:		mov			eax,mocn		// celo��seln� ��st mocnitele
		or			eax,eax			// je mocnitel z�porn�?
		jns			X9				// mocnitel nen� z�porn�
		fdivr		NumConst1		// p�evr�cen� hodnota ��sla
		jmp			X9				// konec


P1:




X9:
	}
*/	
/*
	int stat;

	_asm {
// na�ten� mocnitele
		fld		num2		// na�ten� mocnitele Y -> ST(0)



		fld		num1		// na�ten� mocn�nce X -> ST(0), Y -> ST(1)

// p��prava znam�nka v�sledku
		ftst				// test 



// v�po�et Y * log(2)X (dvojkov� exponent v�sledku)
		fabs				// je povolena pouze kladn� hodnota X
		fyl2x				// Y * log(2)X, ST(1) uvoln�no

// ohrani�en� meziv�sledku do intervalu -1 a� +1
		fld		st(0)		// duplikace meziv�sledku
		wait				// synchronizace
		fnstcw		stat	// ulo�eni ��dic�ho slova
		or			byte ptr [stat+1],0xc // zaokrouhlen� k nule
		wait				// synchronizace
		fldcw		stat	// nastaven� nov�ho ��dic�ho slova
		frndint				// cel� ��st ��sla - exponent
		and			byte ptr [stat+1],not 0xc // zaokrouhlen� k nejbli���mu
		wait				// synchronizace
		fldcw		stat	// nastaven� standardn�ho ��dic�ho slova
		fxch		st(1)	// z�m�na cel� ��sti a meziv�sledku
		fsub		st(0), st(1) // ode�ten� cel� ��sti od meziv�sledku

// v�po�et 2^meziv�sledek
		f2xm1				// 2^X - 1
		fld1				// 1 -> ST(0), meziv�sledek -> ST(1)
		faddp	st(1),st(0)	// v�sledek -> ST(1), ST(0) uvoln�no
		fscale				// vyn�soben� celo��selnou ��st� (exponent)
		fstp	num1		// ulo�en� v�sledku operace
	}
*/
/*
#else

	return pow(x, y);

#endif
}

#pragma warning ( default: 4725)	// hl�en� - probl�mov� instrukce
#pragma warning ( default: 4035)	// hl�en� - nen� n�vratov� hodnota
*/
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
		TCHAR buf[21];
		::GetClassName(wnd, buf, 20);
		if (EditClassName != buf)
		{
			wnd = NULL;
		}
	}
	return wnd;
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� adres��e (pokud existuje, je v�e OK)

BOOL CreateDir(CString txt)
{
// o�ez�n� mezer
	txt.TrimLeft();
	txt.TrimRight();

// o�ez�n� koncov�ch "\"
	while (txt.IsNotEmpty() && (txt[txt.Length() - 1] == _T('\\')))
	{
		txt.Delete(txt.Length() - 1);
		txt.TrimRight();
	}

// vytvo�en� adres��e
	BOOL res = ::CreateDirectory(txt, NULL);
	if (res) return TRUE;

// p�i ne�sp�chu kontrola, zda adres�� ji� existuje
	int attrib = (int)::GetFileAttributes(txt);
	return ((attrib != -1) && (attrib & FILE_ATTRIBUTE_DIRECTORY));
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� cesty (adres��e v�cestup�ov�)

BOOL CreatePath(CString txt)
{
// prvn� pokus o vytvo�en� adres��e
	if (CreateDir(txt)) return TRUE;

// o�ez�n� mezer
	txt.TrimLeft();
	txt.TrimRight();

// o�ez�n� koncov�ch "\"
	while (txt.IsNotEmpty() && (txt[txt.Length() - 1] == _T('\\')))
	{
		txt.Delete(txt.Length() - 1);
		txt.TrimRight();
	}

// adres�� se bude zkracovat
	CString txt2 = txt;
	CString txt3;

// zkracov�n� adres��e
	while (txt2.RevFind(_T('\\')) >= 0)
	{
		txt2.Delete(txt2.RevFind(_T('\\')));
		if (CreateDir(txt2)) break;
	}

// prodlu�ov�n� adres��e
	while (txt2.Length() < txt.Length())
	{
		txt2 += _T('\\');
		txt3 = txt.Mid(txt2.Length(), txt.Length());
		int pos = txt3.Find(_T('\\'));
		if (pos < 0) break;
		txt2 += txt3.Left(pos);
		if (!CreateDir(txt2)) return FALSE;
	}

// vytvo�en� posledn�ho stupn� (to je posledn� pokus)
	return CreateDir(txt);
}
