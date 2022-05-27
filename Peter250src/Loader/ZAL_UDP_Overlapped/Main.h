
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

//#define _MT
//#undef _M_IX86

// Pro p�eklada� nastavit �rove� varov�n� 4 (p�ep�na� /W4)

// U "inline" funkc� nepou��vat "bool" ale rad�ji "BOOL" - efektivn�j�� p�eklad !

//#pragma comment(linker, "/SUBSYSTEM:CONSOLE")


//////////////////////////////////////////////////////////////////////////////
// u verze MINI chyb�:
//  - ukl�d�n� a na��t�n� obr�zk�, zvuk�, hudby, ikon

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
#endif  // _DEBUG

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

typedef unsigned __int64 QWORD;

#define MAXDWORD	0xffffffff

//////////////////////////////////////////////////////////////////////////////
// standardn� vno�en� sekce

//#define UDP_OVER						// flag - pro UDP pou��t overlapped

#ifdef UDP_OVER
#include <winsock2.h>					// WinSock 2
#endif

#include <windows.h>					// z�kladn� definice WINDOWS
#pragma warning ( disable: 4201)		// hl�en� - nepojmenovan� struktura
#include <mmsystem.h>					// multim�dia
#include <aviriff.h>					// definice pro AVI

#ifndef UDP_OVER
#include <winsock.h>					// WinSock
#endif

#include <windowsx.h>					// roz���en� definice WINDOWS
#include <math.h>						// matematick� operace
//#include <alphaops.h>					// matematick� konstanty
#include <tchar.h>						// obsluha znak� UNICODE/MB
#include <commctrl.h>					// dopl�kov� ovl�dac� prvky
#include <mmreg.h>						// multimedi�ln� definice
#include "dx\ddraw.h"					// DirectDraw

#if (DIRECT3D_VERSION < 0x0500)
typedef DWORD D3DCOLORMODEL;
#endif

#if (DIRECT3D_VERSION >= 0x0800)
#include "dx\d3d8.h"					// Direct3D
#else
#include "dx\d3d.h"						// Direct3D
#endif

//#include <d3dx8.h>
#include "dx\dplay.h"					// DirectPlay
#include "dx\dsound.h"					// DirectSound
#include "dx\gl.h"						// obsluha OpenGL
//#include <glu.h>						// utility OpenGL
#include <richedit.h>					// RichEdit
#include <msacm.h>						// konverze zvuku ACM
#include "resource.h"

#define DOUBLE_INFINITY_VALUE       ((ULONGLONG)0x7ff0000000000000)

#pragma warning ( default: 4201)		// hl�en� - nepojmenovan� struktura

#pragma warning ( disable: 4100)		// hl�en� - nepou�it� form�ln� parametr
#pragma warning ( disable: 4710)		// hl�en� - funkce nen� inline

#ifndef D3DVECTOR_DEFINED
typedef struct _D3DVECTOR {
    float x;
    float y;
    float z;
} D3DVECTOR;
#define D3DVECTOR_DEFINED
#endif

typedef struct _D3DXVECTOR3
{
	float	x;
	float	y;
	float	z;
}D3DXVECTOR3;

typedef struct _D3DXVECTOR4
{
	float	x;
	float	y;
	float	z;
	float	w;
}D3DXVECTOR4;

#ifndef D3DMATRIX_DEFINED
typedef struct _D3DMATRIX {
//    union {
//        struct {
            float        _11, _12, _13, _14;
            float        _21, _22, _23, _24;
            float        _31, _32, _33, _34;
            float        _41, _42, _43, _44;

//        };
  //      float m[4][4];
//    };
} D3DMATRIX;
#define D3DMATRIX_DEFINED
#endif

#ifndef D3DCOLORVALUE_DEFINED
typedef struct _D3DCOLORVALUE {
    float r;
    float g;
    float b;
    float a;
} D3DCOLORVALUE;
#define D3DCOLORVALUE_DEFINED
#endif

//////////////////////////////////////////////////////////////////////////////
// p�eddefinice t��d

class CIcon; class CMap; class CMusic; class CPicture; class CSound; class CSprite;
class CString;
class CBufBool; class CBufIcon; class CBufInt; class CBufMap; class CBufMusic;
class CBufPic; class CBufReal; class CBufSound; class CBufSprite; class CBufText;
class CBufIndex; class CBuf; class CBufList;

struct PETHEAD_;

//////////////////////////////////////////////////////////////////////////////
// glob�ln� prom�nn�

extern	PETHEAD_*	PetAdr;				// adresa dat programu
extern	int			PetSize;			// velikost dat programu
extern	CBuf		PetBuf;				// buffer dat programu
extern	char		DATAsize[];			// buffer import� od p�eklada�e

extern	CString		CommandLine;		// p��kazov� ��dek
extern	int			VerzeOS;			// verze syst�mu
extern	OSVERSIONINFO	OSVersionInfo;	// informace o syst�mu
extern	HINSTANCE	hInstance;			// instance programu
extern	int			ScreenWidth;		// ���ka klientsk� oblasti displeje (t� v re�imu DirectDraw)
extern	int			ScreenHeight;		// v��ka klientsk� oblasti displeje (t� v re�imu DirectDraw)
extern	int			ScreenWidth0;		// ���ka videom�du displeje
extern	int			ScreenHeight0;		// v��ka videom�du displeje
extern	int			ScreenBits;			// po�et bit� na bod obrazovky
extern	int			TimerConst;			// konstanta pro �asova� (pro 55 ms)

extern	DWORD		ReturnCode;			// n�vratov� k�d (pro operaci OK)

extern	BYTE*		StdPalImport;		// tabulka importu palet
extern	BITMAPINFO* StdBitmapInfo;		// standardn� z�hlav� BMP
extern	HPALETTE	StdPalette;			// vlastn� logick� palety
extern	LOGPALETTE* pStdPalette;		// tabulka vlastn�ch logick� palet (pot�ebn� t� pro DirectDraw)
extern	BYTE*		KonvPal;			// konverzn� tabulka palet

//#ifndef _MINI
//extern	D3DRMPALETTEENTRY* pStdPalette2;	// tabulka palet s alfa kan�lem (pro Direct3D)
//#endif // _MINI

extern	PALETTEENTRY* SysPal;			// tabulka palet pro DirectDraw 256 barev

#define	ResSysPal	20					// po�et rezervovan�ch syst�mov�ch palet (na za��tku tabulky)

extern	DWORD*		StdPalExport;		// tabulka exportu vlastn�ch barev na barvy Windows

extern	BYTE*		Shadows;			// tabulka pro generov�n� st�nu

extern	bool		Dither;				// pou��vat dithering

extern	bool		Release;			// je distribu�n� verze

extern	CString		ExeFileName;		// jm�no programu

#ifdef _UNICODE
typedef	BOOL (WINAPI *GETDISKFREESPACEEX) (LPCWSTR, __int64*, __int64*, __int64*);
#else
typedef	BOOL (WINAPI *GETDISKFREESPACEEX) (LPCSTR, __int64*, __int64*, __int64*);
#endif
extern GETDISKFREESPACEEX	pGetDiskFreeSpaceEx; // funkce GetDiskFreeSpaceEx (NULL=nen�)

extern	HWND		PrevWindow;			// p�ede�l� aktivn� okno

//////////////////////////////////////////////////////////////////////////////
// definice jazyk�

enum JAZYKID {							// (identifik�tor jazyku - hledej LANGID)
		JAZYKARA,						// arabsky (0x401)
		JAZYKBUL,						// bulharsky (0x402)
		JAZYKCZ,						// �esky (0x405)
		JAZYKDAN,						// d�nsky (0x406)
		JAZYKGER,						// n�mecky (0x407)
		JAZYKREC,						// �ecky (0x408)
		JAZYKENG,						// anglicky (0x409)
		JAZYKSPA,						// �pan�lsky (0x40A)
		JAZYKFIN,						// finsky (0x40b)
		JAZYKFRA,						// francouzsky (0x40C)
		JAZYKHEB,						// hebrejsky (0x40d)
		JAZYKMAD,						// ma�arsky (0x40e)
		JAZYKISL,						// islandsky (0x40f)
		JAZYKITA,						// italsky (0x410)
		JAZYKHOL,						// holandsky (0x413)
		JAZYKNOR,						// norsky (0x414)
		JAZYKPOL,						// polsky (0x415)
		JAZYKPOR,						// portugalsky (0x816)
		JAZYKRUM,						// rumunsky (0x418)
		JAZYKRUS,						// rusky (0x419)
		JAZYKSRB,						// srbochorvatsky (0x41a)
		JAZYKSLO,						// slovensky (0x41b)
		JAZYKALB,						// alb�nsky (0x41c)
		JAZYKSWE,						// �w�dsky (0x41d)
		JAZYKTUR,						// turecky (0x41f)
		JAZYKVIE,						// vietnamsky (0x42a)

		JAZYKNUM						// po�et jazyk�
};

// jazykov� informace
typedef struct JAZYKINFO_
{
	int					LangID;			// identifik�tor jazyku
	int					CodePage;		// k�dov� str�nka znak�
	int					CharSet;		// znakov� sada font�
	char*				TextTab;		// adresa za��tku text� v programu (NULL=nen�)
	LPCTSTR				CancelTxt;		// text "Storno"
	LPCTSTR				AllFilesTxt;	// text "v�echny soubory"
	LPCTSTR				MemErrNadp;		// text nadpisu okna hl�en� o nedostatku pam�ti
	LPCTSTR				MemErrText;		// text okna hl�en� o nedostatku pam�ti
} JAZYKINFO;

extern	int		JazykDef;				// implicitn� jazyk
extern	DWORD	DefCharSet;				// implicitn� znakov� sada font� podle syst�mu

extern	int		JazykDefImp;			// implicitn� importovan� jazyk

extern	int		Jazyk;					// aktu�ln� jazyk
extern	DWORD	LangID;					// aktu�ln� identifik�tor jazyku
extern	DWORD	CharSet;				// aktu�ln� znakov� sada font�
extern	DWORD	CodePage;				// aktu�ln� k�dov� str�nka
extern	char	Carka;					// odd�lova� desetinn� ��rky p�i zobrazen� ��sel

extern	int		JazykUkaz0;				// ukazatel indexu na��tan�ho textu
extern	char*	JazykUkaz[JAZYKNUM];	// ukazatele text� p�i na��t�n�

extern JAZYKINFO JazykInfo[JAZYKNUM];	// tabulka definic jazyk�

//////////////////////////////////////////////////////////////////////////////
// pr�zdn� objekty (nap�. pro n�vrat neplatn� polo�ky z funkce)

extern	CString		EmptyString;		// pr�zdn� �et�zec
extern	CIcon		EmptyIcon;			// pr�zdn� ikona
extern	CMap		EmptyMap;			// pr�zdn� plocha
extern	CPicture	EmptyPicture;		// pr�zdn� obr�zek
extern	CSprite		EmptySprite;		// pr�zdn� sprajt
extern	CSound		EmptySound;			// pr�zdn� zvuk
extern	CMusic		EmptyMusic;			// pr�zdn� hudba

//////////////////////////////////////////////////////////////////////////////
// glob�ln� konstanty

#define ResCols 2					// po�et rezervovan�ch barev na po��tku barev (=pozad� a st�n)

//extern const int ColLev;			// po�et �rovn� standardn�ch palet
#define	ColLev 6					// po�et �rovn� standardn�ch palet
//extern const int ColCol;			// po�et barev standardn�ch palet
#define	ColCol 37					// po�et barev standardn�ch palet
//extern const int StdColors;			// po�et vlastn�ch palet (za��naj� od 0)
#define	StdColors (ResCols+ColCol*ColLev)	// po�et vlastn�ch palet (za��naj� od 0)

#define BackCol 0					// pr�hledn� barva (barva pozad�)
#define ShadCol 1					// polopr�hledn� barva (st�n)

#define BACKCOLOR_RED	149			// �erven� slo�ka barvy pozad�
#define BACKCOLOR_GREEN	34			// zelen� slo�ka barvy pozad�
#define BACKCOLOR_BLUE	140			// modr� slo�ka barvy pozad�

#define SHADCOLOR_RED	97			// �erven� slo�ka barvy st�nu
#define SHADCOLOR_GREEN	30			// zelen� slo�ka barvy st�nu
#define SHADCOLOR_BLUE	111			// modr� slo�ka barvy st�nu

extern const BYTE WhiteCol;			// b�l� barva
extern const BYTE BlackCol;			// �ern� barva
extern const BYTE BlueCol;			// modr� barva
extern const BYTE RedCol;			// �erven� barva
extern const BYTE YellowCol;		// �lut� barva
extern const BYTE GreenCol;			// zelen� barva
extern const BYTE LtYellowCol;		// sv�tle �lut� barva
extern const BYTE OrangeCol;		// oran�ov�

extern const double pi;				// Ludolfovo ��slo
extern const double pi2;			// Ludolfovo ��slo * 2
extern const double pi8;			// Ludolfovo ��slo * 8
extern const double pi2m;			// - Ludolfovo ��slo * 2
extern const double pi8m;			// - Ludolfovo ��slo * 8
extern const double eul;			// Eulerovo ��slo

extern const double uhel22;			// �hel 22.5 stup�� v radi�nech
extern const double uhel30;			// �hel 30 stup�� v radi�nech
extern const double uhel45;			// �hel 45 stup�� v radi�nech
extern const double uhel60;			// �hel 60 stup�� v radi�nech
extern const double uhel67;			// �hel 67.5 stup�� v radi�nech
extern const double uhel90;			// �hel 90 stup�� v radi�nech
extern const double uhel135;		// �hel 135 stup�� v radi�nech
extern const double uhel180;		// �hel 180 stup�� v radi�nech
extern const double uhel225;		// �hel 215 stup�� v radi�nech
extern const double uhel270;		// �hel 270 stup�� v radi�nech
extern const double uhel315;		// �hel 315 stup�� v radi�nech
extern const double uhel360;		// �hel 360 stup�� v radi�nech

extern const double ln10;			// p�irozen� logaritmus 10

extern const double degrad;			// konstanta pro p�evod z DEG na RAD (pi/180)
extern const double raddeg;			// konstanta pro p�evod z RAD na DEG (180/pi)

#define BigInt 0x40000000			// velk� ��slo INT

extern double* FactTab;				// tabulka faktori�l�
#define	MAXFACT	170					// maxim�ln� hodnota faktori�lu

#define	CR	13						// ASCII znak CR
#define	LF	10						// ASCII znak LF

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

inline BYTE PalImport(DWORD color)
{
	if (color >= 0x60000000)
	{
		if (color >= 0x80000000)
		{
			return BackCol;
		}
		return ShadCol;
	}

	return StdPalImport[
		((color & (63*4))/4) |							// �erven� slo�ka
		((color & (63*4*256))/(256*4/64)) |				// zelen� slo�ka
		((color & (63*4*256*256))/(256*256*4/64/64))];	// modr� slo�ka
}

// export palety (zajistit rozsah barvy 0 a� 255 !!!)
inline DWORD PalExport(int col)
{
	return StdPalExport[col];
}

//////////////////////////////////////////////////////////////////////////////
// ukon�en� aplikace

void	Exit(DWORD code);				// ukon�en� programu

#define	EXITCODE_MEMERR		253			// chyba pam�ti p�i b�hu programu
#define EXITCODE_MAINFRAME	252			// chyba vytvo�en� hlavn�ho okna
#define EXITCODE_LOADRES	251			// chyba �ten� resource

#define EXITCODE_OK			0			// ��dn� ukon�en� programu


//////////////////////////////////////////////////////////////////////////////
// aktualizace informac� o obrazovce (nen�-li celoobrazovkov� m�d)

void AktDispInfo0();
void AktDispInfo();


//////////////////////////////////////////////////////////////////////////////
// nastaven� vy��� p�esnosti procesoru

void FPUDouble();

//////////////////////////////////////////////////////////////////////////////
// nastaven� ni��� p�esnosti procesoru

void FPUFloat();


//////////////////////////////////////////////////////////////////////////////
// vyst�ed�n� okna proti jin�mu oknu

void CenterWindow(HWND child, HWND parent = NULL);


//////////////////////////////////////////////////////////////////////////////
// inicializace jazyku

void InitLang(int jazyk);


//////////////////////////////////////////////////////////////////////////////
// inicializace kl�vesnice

void InitKeyb();


//////////////////////////////////////////////////////////////////////////////
// spu�t�n� programu

//int Exec(CString name, CString param, BOOL wait);
DWORD Exec(CString command, BOOL wait);


/////////////////////////////////////////////////////////////////////////////
// generov�n� konverzn� tabulky barev obr�zku

void GenKonvPal(BITMAPINFO* bmp);


/////////////////////////////////////////////////////////////////////////////
// vygenerov�n� st�edn� barvy (ze 4 bod�)

BYTE _fastcall ColAvrg(BYTE col1, BYTE col2, BYTE col3, BYTE col4);


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� edita�n�ho okna s fokusem (NULL = nen�)

HWND GetEditFocus();


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� adres��e (pokud existuje, je v�e OK)

BOOL CreateDir(CString txt);

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� cesty (adres��e v�cestup�ov�)

BOOL CreatePath(CString txt);


/////////////////////////////////////////////////////////////////////////////
// dekomprese dat

void DeKomp(BYTE* dstBuf, int dstNum, BYTE* srcBuf, int srcNum);


/////////////////////////////////////////////////////////////////////////////
// gener�tor n�hody

int rand();
double Rand();

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
// mocnina X^Y

//double Power(double x, double y);


//////////////////////////////////////////////////////////////////////////////
// pomocn� konstanty

#define ICONWIDTH 32								// ���ka ikon
#define ICONHEIGHT 32								// v��ka ikon
#define ICONSIZE (ICONWIDTH*ICONHEIGHT)				// velikost ikon v bajtech

#define MOUSEINV -100000							// neplatn� sou�adnice my�i


#define		PROGBUFNUM	6				// po�et programov�ch buffer�

// identifikace buffer� (nem�nit po�ad� - muselo by se zm�nit p�i ukl�d�n� souboru!)
#define		BufIntID	0				// intern� buffer seznamu funkc� (nem� strom)
#define		BufClsID	1				// buffer t��d objekt�
#define		BufObjID	2				// buffer glob�ln�ch objekt�
#define		BufLocID	3				// buffer lok�ln�ch objekt�
#define		BufEdiID	4				// buffer editace
#define		BufStrID	5				// buffer struktur

// datov� buffery (mus� odpov�dat index�m v SaveHead !!!!)
#define		BufNumID	PROGBUFNUM		// (6) buffer ��sel
#define		BufTxtID	PROGBUFNUM+1	// (7) buffer text�
#define		BufLogID	PROGBUFNUM+2	// (8) buffer logick�ch prom�nn�ch
#define		BufIcoID	PROGBUFNUM+3	// (9) buffer ikon
#define		BufMapID	PROGBUFNUM+4	// (10) buffer ploch
#define		BufPicID	PROGBUFNUM+5	// (11) buffer obr�zk�
#define		BufSprID	PROGBUFNUM+6	// (12) buffer sprajt�
#define		BufSndID	PROGBUFNUM+7	// (13) buffer zvuk�
#define		BufMusID	PROGBUFNUM+8	// (14) buffer hudby
#define		BufPalID	PROGBUFNUM+9	// (15) buffer palet

// P�i zm�n� zm�nit definice i v Peter.EXE !!!!!

// p��znaky typu ikony a obr�zku
enum PICPARAM {
	PicParamPic,			// pouze obr�zek bez pozad�
	PicParamMix,			// obr�zek mixovan� s pozad�m (nebo st�n)
	PicParamBack,			// pouze pozad� (pr�zdn� obr�zek)
	PicParamNone,			// nezn�m� obsah
	PicParamComp,			// komprimovan� data (R/O - neru�it buffer!)
};

//////////////////////////////////////////////////////////////////////////////
// vlastn� vno�en� sekce - z�kladn� deklarace

#include "Memory.h"						// obsluha pam�ti
#include "Bitmap.h"						// obsluha bitmap a ikon
#include "BufText.h"					// buffer text�, textov� �et�zce
#include "BufIcon.h"					// buffer ikon, data ikon
#include "BufPic.h"						// buffer obr�zk� (pozad�)
#include "BufWin.h"						// buffer prvk� oken
#include "MainFrm.h"					// hlavn� okno aplikace
#include "BufInt.h"						// buffer cel�ch ��sel a logick�ch prom�nn�ch
#include "BufList.h"					// buffer seznam�
#include "BufMap.h"						// buffer mapy plochy
#include "BufMus.h"						// buffer hudby
#include "JPEG.h"						// obsluha JPEG
#include "BufReal.h"					// buffer re�ln�ch ��sel
#include "BufSnd.h"						// buffer zvuku
#include "BufSprt.h"					// buffer sprajt�
#include "BufD3D.h"						// buffer Direct3D
#include "BufXFile.h"					// buffer objekt� X souboru
#include "File.h"						// obsluha soubor�
#include "Compress.h"					// obsluha komprese dat
#include "Load.h"						// na��t�n� programu

#include "D3NO.h"						// ovlada� 3D - nen�
#include "D3GL0.h"						// ovlada� 3D - OpenGL 1.0
#include "D3GL1.h"						// ovlada� 3D - OpenGL 1.1
#include "D3GL2.h"						// ovlada� 3D - OpenGL 2.0
#include "D3DX4.h"						// ovlada� 3D - DirectX 4
#include "D3DX5.h"						// ovlada� 3D - DirectX 5
#include "D3DX6.h"						// ovlada� 3D - DirectX 6
#include "D3DX7.h"						// ovlada� 3D - DirectX 7
#include "D3DX8.h"						// ovlada� 3D - DirectX 8

//////////////////////////////////////////////////////////////////////////////
// modely funkc�

typedef void	(			*PROCCOM)	();					// p��kaz
typedef double	(			*PROCNUM)	();					// ��slo
typedef void	(_fastcall	*PROCTXT)	(CString& text);	// text
typedef bool	(			*PROCLOG)	();					// logick� v�raz
typedef void	(_fastcall	*PROCICO)	(CIcon& icon);		// ikona
typedef void	(_fastcall	*PROCMAP)	(CMap& map);		// plocha
typedef void	(_fastcall	*PROCPIC)	(CPicture& pic);	// obr�zek
typedef void	(_fastcall	*PROCSPR)	(CSprite& sprite);	// sprajt
typedef	void	(_fastcall	*PROCSND)	(CSound& sound);	// zvuk
typedef void	(_fastcall	*PROCMUS)	(CMusic& music);	// hudba


/////////////////////////////////////////////////////////////////////////////
// polo�ka bufferu programu

#pragma warning ( disable: 4201)		// hl�en� - nepojmenovan� struktura

typedef struct EXECITEM_ {
	union 
	{
		PROCCOM		ExecCom;		// p��kaz
		PROCNUM		ExecNum;		// ��slo
		PROCTXT		ExecTxt;		// text
		PROCLOG		ExecLog;		// logick� v�raz
		PROCICO		ExecIco;		// ikona
		PROCMAP		ExecMap;		// plocha
		PROCPIC		ExecPic;		// obr�zek
		PROCSPR		ExecSpr;		// sprajt
		PROCSND		ExecSnd;		// zvuk
		PROCMUS		ExecMus;		// hudba
	};

	union 
	{
		double		Double;			// re�ln� data (��seln� konstanta)

		struct
		{	
			int		Data;			// data
			int		List;			// seznam
		};
	};

	int			Jump;				// offset ke zrychlen�mu p�esko�en� prvku ve skupin�
} EXECITEM;

#pragma warning ( default: 4201)	// hl�en� - nepojmenovan� struktura

extern	EXECITEM*	ExecItem;		// ukazatel prov�d�n�ho programu


//////////////////////////////////////////////////////////////////////////////
// vlastn� vno�en� sekce - p�eklad programu

#include "Comp.h"						// kompilace programu
#include "CompCom.h"					// kompilace programu - p��kaz
#include "CompIco.h"					// kompilace programu - ikona
#include "CompLog.h"					// kompilace programu - logick� v�raz
#include "CompMap.h"					// kompilace programu - plocha
#include "CompMus.h"					// kompilace programu - hudba
#include "CompNum.h"					// kompilace programu - ��seln� v�raz
#include "CompPic.h"					// kompilace programu - obr�zek
#include "CompSnd.h"					// kompilace programu - zvuk
#include "CompSpr.h"					// kompilace programu - sprajt
#include "CompTxt.h"					// kompilace programu - text

#include "ExecCom.h"					// prov�d�n� programu - p��kaz
#include "ExecIco.h"					// prov�d�n� programu - ikona
#include "ExecLog.h"					// prov�d�n� programu - logick� v�raz
#include "ExecMap.h"					// prov�d�n� programu - plocha
#include "ExecMus.h"					// prov�d�n� programu - hudba
#include "ExecNum.h"					// prov�d�n� programu - ��seln� v�raz
#include "ExecPic.h"					// prov�d�n� programu - obr�zek
#include "ExecSnd.h"					// prov�d�n� programu - zvuk
#include "ExecSpr.h"					// prov�d�n� programu - sprajt
#include "ExecTxt.h"					// prov�d�n� programu - text
#include "Exec.h"						// prov�d�n� programu


/////////////////////////////////////////////////////////////////////////////
// buffery

extern	int			TextListBeg;					// index po��tku glob�ln�ch seznam�
extern	int			TextListEnd;					// index konce glob�ln�ch seznam�

//extern	CBufProg	BufProg[PROGBUFNUM];			// buffery programu
extern	CBufReal	Real;							// buffer re�ln�ch ��sel
extern	CBufText	Text;							// buffer text�
extern	CBufBool	Bool;							// buffer logick�ch prom�nn�ch
extern	CBufIcon	Icon;							// buffer ikon
extern	CBufMap		Map;							// buffer ploch
extern	CBufPic		Picture;						// buffer obr�zk�
extern	CBufSprite	Sprite;							// buffer sprajt�
extern	CBufSound	Sound;							// buffer zvuk�
extern	CBufMusic	Music;							// buffer hudby
extern	CBufList	List;							// buffer seznam�
extern	CBufWin		Win;							// buffer prvk� oken
//extern	CBufD3DO	D3DO;							// buffer objekt� Direct3D
//extern	CBufD3DF	D3DF;							// buffer r�m� Direct3D

//#define	BufInt		BufProg[BufIntID]				// intern� buffer
//#define	BufCls		BufProg[BufClsID]				// buffer t��d
//#define	BufObj		BufProg[BufObjID]				// glob�ln� buffer
//#define BufLoc		BufProg[BufLocID]				// lok�ln� buffer
//#define	BufEdi		BufProg[BufEdiID]				// edita�n� buffer
//#define	BufStr		BufProg[BufStrID]				// buffer struktur

