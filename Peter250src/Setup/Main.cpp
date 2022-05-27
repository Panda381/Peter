
#include "Main.h"

// Upozorn�n�:
// -----------
// Obr�zky mus� m�t v�echny shodn� palety, nelze proto pou��vat
// editor VC, obr�zky se konvertuj� pomoc� ALCHEMY spu�t�n�m !KONV.BAT

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

// v�echny obr�zky mus� m�t rozm�r jako plocha okna
CPicture	Picture0;					// obr�zek 0 (bez volby)
CPicture	Picture1;					// obr�zek 1 (menu 1 - instalovat)
CPicture	Picture2;					// obr�zek 2 (menu 2 - p�idat/debrat)
CPicture	Picture3;					// obr�zek 3 (menu 3 - odinstalovat)
CPicture	Picture4;					// obr�zek 4 (menu 4 - konec)
CPicture	Shadow;						// st�n
CPicture	InstalOK;					// obr�zek instalace OK
CPicture	DInstalOK;					// obr�zek odinstalace OK
CPicture	DInstal;					// obr�zek odinstalace

// pozor - vlajky mus� m�t ���ku n�sobky 8 !!!! (kv�li zobrazen�)
CPicture	FlagCZ;						// �esk� vlajka
CPicture	FlagENG;					// anglick� vlajka

int			KeyDisp = 0;				// zobrazen� obr�zek volby hlavn�ho okna (0 a� 4)

int			GemtreeFaze = 0;			// f�ze aktivn�ho obr�zku
CPicture	GemLogo;					// obr�zek znaku Gemtree (90 obr�zk� horizont�ln� s rozm�ry 40x40)

BITMAPINFO* StdBitmapInfo;				// standardn� z�hlav� BMP
HPALETTE	StdPalette = NULL;			// vlastn� logick� palety

DWORD		BackColCol = 0;				// barva pozad�
HBRUSH		BackBrush = NULL;			// �t�tec pozad�

TCHAR		MainFrameClass[] = _T("PeterSetupClass");	// n�zev t��dy hlavn�ho okna
HWND		MainFrame = NULL;			// hlavn� okno aplikace

CText		PeterMainFrameClass(_T("PeterMainFrameClass"));	// n�zev t��dy hlavn�ho okna editoru Petr
CText		PeterSetupClass(MainFrameClass);	// n�zev t��dy instal�toru Petra

BOOL		Dither = FALSE;

int			MainFrameWidth = 620;		// ���ka hlavn�ho okna
int			MainFrameHeight = 420;		// v��ka hlavn�ho okna
#define		ClientWidth 608				// ���ka klientsk� oblasti (mus� to b�t n�sobek 8 a stejn� jako Picture)
#define		ClientHeight 384			// v��ka klientsk� oblasti (mus� b�t stejn� jako Picture)
BYTE*		ClientBuf = NULL;			// buffer k zobrazen� obr�zku v klientsk� oblasti

int			OffX = MOUSEINV;			// offset st�nu X (MOUSEINV = neplatn�)
int			OffY = MOUSEINV;			// offset st�nu Y (MOUSEINV = neplatn�)

int			RelX = 0;					// kurzor my�i relativn� v okn� X
int			RelY = 0;					// kurzor my�i relativn� v okn� Y

int			Waiting = 0;				// p��znak zobrazen� kurzoru �ek�n�

#define		TimerID 1234				// ID �asova�e
int			Timer = 0;					// �asova�

HCURSOR		CurArrow;					// (standardn�) �ipka
HCURSOR		CurWait;					// �ek�n�
HCURSOR		CurPeter;					// kurzor Petr

int			KeySelect = -1;				// v�b�r volby na hlavn� str�nce kl�vesnic� (-1 = nen�)

int			AktPage = PAGELANG;			// aktivn� str�nka instal�toru

__int64	DiskSize = 0;					// velikost disku (z funkce GetDiskSpace)
__int64	DiskFree = 0;					// voln� m�sto disku (z funkce GetDiskSpace)
__int64	DiskFreeUser = 0;				// voln� m�sto u�ivatele (z funkce GetDiskSpace)

int			DiskFre = 0;				// voln� m�sto c�lov�ho disku v MB
int			DiskReq = 0;				// po�adovan� voln� m�sto v MB

GETDISKFREESPACEEX	pGetDiskFreeSpaceEx = NULL;	// funkce GetDiskFreeSpaceEx (NULL=nen�)

CText		DelExe;						// spu�t�n� programu p�i ukon�en� (Empty=nen�)

CText	InstPath;						// instala�n� cesta

//////////////////////////////////////////////////////////////////////////////
// jazyky

#define	JAZYKENG	0					// anglicky
#define	JAZYKCZ		1					// �esky

int		Jazyk = JAZYKENG;				// vybran� jazyk

//////////////////////////////////////////////////////////////////////////////
// prvky okna instal�toru

HIMAGELIST CheckList = NULL;			// obr�zky tla��tek

HWND	ButtonBack = NULL;				// tla��tko zp�t
HWND	ButtonNext = NULL;				// tla��tko dal��
HWND	ButtonExit = NULL;				// tla��tko ukon�en�
HWND	LineWnd = NULL;					// ��ra nad tla��tky
HWND	CheckWnd = NULL;				// okno p�ep�na��
HWND	CheckNadp = NULL;				// nadpis p�ep�na��
HWND	SmlouvaWnd = NULL;				// okno licen�n� smlouvy
HWND	SmlouvaNadp = NULL;				// nadpis nad smlouvou
HWND	PathWndNadp = NULL;				// instala�n� cesta - nadpis
HWND	PathWnd = NULL;					// instala�n� cesta
HWND	ButtonList = NULL;				// tla��tko Proch�zet
HWND	UserRam = NULL;					// r�m u�ivatele
HWND	LicTextNadp = NULL;				// licen�n� text - nadpis
HWND	LicTextNum = NULL;				// licen�n� text - ��slo
HWND	LicTextName = NULL;				// licen�n� text - jm�no
HWND	LicTextAdr = NULL;				// licen�n� text - adresa
HWND	LicTextMest = NULL;				// licen�n� text - m�sto
HWND	LicTextIco = NULL;				// licen�n� text - I�O
HWND	DiskReqNadp = NULL;				// po�adovan� voln� m�sto - text
HWND	DiskReqNum = NULL;				// po�adovan� voln� m�sto - ��slo
HWND	DiskFreeNadp = NULL;			// voln� m�sto c�lov�ho disku - text
HWND	DiskFreeNum = NULL;				// voln� m�sto c�lov�ho disku - ��slo
HWND	ButtonCanc = NULL;				// tla��tko p�eru�en�
HWND	IndWnd = NULL;					// indik�tor instalace
HWND	IndWndNadp = NULL;				// indik�tor instalace - nadpis
HWND	IndWndFile = NULL;				// indik�tor instalace - soubor
HWND	IndWnd000 = NULL;				// indik�tor - 0 %
HWND	IndWnd025 = NULL;				// indik�tor - 25 %
HWND	IndWnd050 = NULL;				// indik�tor - 50 %
HWND	IndWnd075 = NULL;				// indik�tor - 75 %
HWND	IndWnd100 = NULL;				// indik�tor - 100 %
HWND	FlagCZWnd = NULL;				// tla��tko �esky
HWND	FlagENGWnd = NULL;				// tla��tko Anglicky

enum {
	IDN_BUTTONBACK = 13000,				// tla��tko Zp�t
	IDN_BUTTONNEXT,						// tla��tko Dal��
	IDN_BUTTONEXIT,						// tla��tko Konec
	IDN_LINEWND,						// ��ra nad tla��tky
	IDN_CHECKWND,						// okno p�ep�na��
	IDN_CHECKNADP,						// nadpis p�ep�na��
	IDN_SMLOUVAWND,						// licen�n� smlouva
	IDN_SMLOUVANADP,					// nadpis licen�n� smlouvy
	IDN_PATHWNDNADP,					// instala�n� cesta - nadpis
	IDN_PATHWND,						// instala�n� cesta
	IDN_BUTTONLIST,						// tla��tko Proch�zet
	IDN_USERRAM,						// r�m u�ivatele
	IDN_LICTEXTNADP,					// licen�n� text - nadpis
	IDN_LICTEXTNUM,						// licen�n� text - ��slo
	IDN_LICTEXTNAME,					// licen�n� text - jm�no
	IDN_LICTEXTADR,						// licen�n� text - adresa
	IDN_LICTEXTMEST,					// licen�n� text - m�sto
	IDN_LICTEXTICO,						// licen�n� text - I�O
	IDN_DISKREQNADP,					// po�adovan� voln� m�sto - text
	IDN_DISKREQNUM,						// po�adovan� voln� m�sto - ��slo
	IDN_DISKFREENADP,					// voln� m�sto c�lov�ho disku - text
	IDN_DISKFREENUM,					// voln� m�sto c�lov�ho disku - ��slo
	IDN_BUTTONCANC,						// tla��tko p�eru�en�
	IDN_INDWND,							// indik�tor instalace
	IDN_INDWNDNADP,						// indik�tor instalace - nadpis
	IDN_INDWNDFILE,						// indik�tor instalace - soubor
	IDN_INDWND000,						// indik�tor instalace - 0%
	IDN_INDWND025,						// indik�tor instalace - 25%
	IDN_INDWND050,						// indik�tor instalace - 50%
	IDN_INDWND075,						// indik�tor instalace - 75%
	IDN_INDWND100,						// indik�tor instalace - 100%
	IDN_FLAGWNDCZ,						// tla��tko �esky
	IDN_FLAGWNDENG,						// tla��tko Anglicky
};

//////////////////////////////////////////////////////////////////////////////
// p�ep�na�e instalovan�ch ��st�

CText		ExeFileName;				// jm�no tohoto programu SETUP.EXE
HANDLE		HFile = NULL;				// handle souboru SETUP.EXE
INSTHEAD	Head;						// z�hlav� instala�n�ch dat

BOOL		Uninst = FALSE;				// je odinstal�tor Uninstal.exe

int			RawData = 0;				// offset za��tku dat v souboru
int			RawSize = 0;				// velikost dat v souboru
int			DataOff = 0;				// offset za��tku dat
int			VirtAdr = 0;				// virtu�ln� adresa k na�ten� dat

int			SizeOff = 0;				// offset velikosti souboru v z�hlav�
int			SizeImageOff = 0;			// offset velikosti obrazu programu v z�hlav�
int			VirtSizeOff = 0;			// offset virtu�ln� velikosti dat v z�hlav�
int			GDIModeOff = 0;				// offset p��znaku GDI m�du programu v z�hlav�

#define		PROGMAX		128				// rozsah indik�toru
int			DataSize = 0;				// v�choz� velikost instalovan�ch dat
int			DataSizeOK = 0;				// velikost zpracovan�ch instalovan�ch dat
BOOL		Storno = FALSE;				// po�adavek p�eru�en� operace instalace

typedef struct GROUPITEM_ {
	LPCTSTR name;						// jm�no p�ep�na�e volby
	bool	initcheck;					// inicializa�n� p��znak volby ��sti
	bool	check;						// p��znak volby ��sti
	bool	instal;						// p��znak, �e prvek je nainstalov�n
	BYTE*	files;						// seznam soubor�
} GROUPITEM;

#ifdef MINI
GROUPITEM GroupsCZ[GROUPSNUM] =
{
	_T("hlavn� program aplikace Petr"),		TRUE,	TRUE,	FALSE,	NULL,
	_T("p��klady vytvo�en�ch program�"),	TRUE,	TRUE,	FALSE,	NULL,
	_T("uk�zkov� knihovny"),				TRUE,	TRUE,	FALSE,	NULL,
};

GROUPITEM GroupsEN[GROUPSNUM] =
{
	_T("main program of the Peter application"),TRUE,	TRUE,	FALSE,	NULL,
	_T("examples of created programs"),		TRUE,	TRUE,	FALSE,	NULL,
	_T("example libraries"),				TRUE,	TRUE,	FALSE,	NULL,
};
#else // MINI
GROUPITEM GroupsCZ[GROUPSNUM] =
{
	_T("hlavn� program aplikace Petr"),		TRUE,	TRUE,	FALSE,	NULL,
	_T("p��klady vytvo�en�ch program�"),	TRUE,	TRUE,	FALSE,	NULL,
	_T("knihovna p�edm�t� (ikon)"),			TRUE,	TRUE,	FALSE,	NULL,
	_T("knihovna obr�zk�"),					TRUE,	TRUE,	FALSE,	NULL,
	_T("knihovna sprajt�"),					TRUE,	TRUE,	FALSE,	NULL,
	_T("knihovna zvuk�"),					TRUE,	TRUE,	FALSE,	NULL,
	_T("knihovna hudeb"),					TRUE,	TRUE,	FALSE,	NULL,
	_T("ostatn� knihovny"),					TRUE,	TRUE,	FALSE,	NULL,
	_T("zdroje pro generov�n� sprajt�"),	TRUE,	TRUE,	FALSE,	NULL,
};

GROUPITEM GroupsEN[GROUPSNUM] =
{
	_T("main program of the Peter application"),TRUE,	TRUE,	FALSE,	NULL,
	_T("examples of created programs"),		TRUE,	TRUE,	FALSE,	NULL,
	_T("item library (icons)"),				TRUE,	TRUE,	FALSE,	NULL,
	_T("picture library"),					TRUE,	TRUE,	FALSE,	NULL,
	_T("sprite library"),					TRUE,	TRUE,	FALSE,	NULL,
	_T("sound library"),					TRUE,	TRUE,	FALSE,	NULL,
	_T("music library"),					TRUE,	TRUE,	FALSE,	NULL,
	_T("other libraries"),					TRUE,	TRUE,	FALSE,	NULL,
	_T("sources to generate sprites"),		TRUE,	TRUE,	FALSE,	NULL,
};
#endif // MINI

GROUPITEM*	Groups = GroupsEN;

CText ProductKey0(_T("Software\\Gemtree Software\\Peter"));
CText ProductKey(_T("Software\\Gemtree Software\\Peter\\" VerzeTxt));
CText UninstalKey0(_T("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall"));
CText UninstalKey(_T("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Peter" VerzeTxt));

BOOL	FirstInst = TRUE;				// je prvn� instalace

/*=========================================================================*\
+																			+
+							Hlavn� start programu							+
+																			+
\*=========================================================================*/

//////////////////////////////////////////////////////////////////////////////
// rozli�en� textu podle jazyku

LPCTSTR X(LPCTSTR cesky, LPCTSTR anglicky)
{
	switch (Jazyk)
	{
	case JAZYKCZ:
		return cesky;

	default:
		return anglicky;
	}
}

CText X(const CText& cesky, const CText& anglicky)
{
	switch (Jazyk)
	{
	case JAZYKCZ:
		return cesky;

	default:
		return anglicky;
	}
}

int X(int cesky, int anglicky)
{
	switch (Jazyk)
	{
	case JAZYKCZ:
		return cesky;

	default:
		return anglicky;
	}
}

//////////////////////////////////////////////////////////////////////////////
// sestaven� obr�zk� pro aktivn� jazyk

void InitPic()
{
// p��prava skupiny p�ep�na��
	switch (Jazyk)
	{
	case JAZYKCZ:
		Groups = GroupsCZ;
		break;

	default:
		Groups = GroupsEN;
	}

// nastaven� titulku okna
	CText txt(X(_T("Instal�tor aplikace Petr "), _T("Installator of the Peter Application ")));
	txt += VerzeTxt;
	txt.SetWindowText(MainFrame);

// p��prava pr�zdn�ho obr�zku plochy
	Picture0.Clear(ClientWidth, ClientHeight);

// p�id�n� obr�zku Petra
	Picture0.AddCopy(IDN_PETR, 25, 237);

// p�id�n� titulku
	Picture0.AddCopy(X(IDN_TITLE_CZ, IDN_TITLE_EN), 170, 230);

// dek�dov�n� ��sla verze (x = 496, y = 326, ���ka ��slice = 12. v��ka ��slice = 16)
#define VERZEX 496
#define VERZEY 325
	CPicture pic;
	pic.Load(IDN_DIGIT);
	Picture0.AddCopy(pic, VERZEX,			VERZEY, 12, 16, VerzeMaj*12, 0);
	Picture0.AddCopy(pic, VERZEX+12,		VERZEY, 12, 16,	      10*12, 0);
	Picture0.AddCopy(pic, VERZEX+12+8,		VERZEY, 12, 16, VerzeMin*12, 0);
	Picture0.AddCopy(pic, VERZEX+12+8+12,	VERZEY, 12, 16, VerzeRel*12, 0);

// p�id�n� linky
	Picture0.AddCopy(IDN_LINKA, 0, 213);

// p�id�n� textu Gemtree
	Picture0.AddCopy(IDN_GEMTREE, 30, 160);

// p�id�n� adresy
	Picture0.AddCopy(X(IDN_ADR_CZ, IDN_ADR_EN), 0, 74);

// p�id�n� telefonu
	Picture0.AddCopy(X(IDN_PHONE_CZ, IDN_PHONE_EN), 0, 0);

// p�id�n� voleb
#define MENX	230
#define MENY	22
#define	MENDY	50
	switch (Jazyk)
	{
	case JAZYKCZ:
		Picture0.AddCopy(FirstInst ? IDN_INST0_CZ : IDN_INST2_CZ, MENX, MENY + 3*MENDY);
		Picture0.AddCopy(FirstInst ? IDN_ADD2_CZ  : IDN_ADD0_CZ,  MENX, MENY + 2*MENDY);
		Picture0.AddCopy(FirstInst ? IDN_UINS2_CZ : IDN_UINS0_CZ, MENX, MENY + 1*MENDY);
		Picture0.AddCopy(IDN_END0_CZ,  MENX, MENY + 0*MENDY);
		break;

	default:
		Picture0.AddCopy(FirstInst ? IDN_INST0_EN : IDN_INST2_EN, MENX, MENY + 3*MENDY);
		Picture0.AddCopy(FirstInst ? IDN_ADD2_EN  : IDN_ADD0_EN,  MENX, MENY + 2*MENDY);
		Picture0.AddCopy(FirstInst ? IDN_UINS2_EN : IDN_UINS0_EN, MENX, MENY + 1*MENDY);
		Picture0.AddCopy(IDN_END0_EN,  MENX, MENY + 0*MENDY);
	}

// konverze st�nu
	{
		Shadow.New((int)(Picture0.Width()*1.052+0.5),(int)(Picture0.Height()*1.052+0.5));

		for (int yd = Shadow.Height()-1; yd >= 0; yd--)
		{
			int ys = yd*Picture0.Height()/Shadow.Height();
			if (ys < 0) ys = 0;
			if (ys >= Picture0.Height()) ys = Picture0.Height()-1;

			for (int xd = Shadow.Width()-1; xd >= 0; xd--)
			{
				int xs = xd*Picture0.Width()/Shadow.Width();
				if (xs < 0) xs = 0;
				if (xs >= Picture0.Width()) xs = Picture0.Width()-1;

				BYTE* src = Picture0.DataData() + (xs + ys*Picture0.Width());
				BYTE* dst = Shadow.DataData() + (xd + yd*Shadow.Width());

				BYTE col = *src;
				if (col == BackCol)
				{
					*dst = col;
				}
				else
				{
					*dst = ShadCol;
				}
			}
		}
	}

// vypnut� p�ep�na�e
#define SWCX	193
#define SWCY	19
#define SWCDY	50
	Picture0.AddCopy(IDN_SWCOFF, SWCX, SWCY + 0*SWCDY);
	Picture0.AddCopy(IDN_SWCOFF, SWCX, SWCY + 1*SWCDY);
	Picture0.AddCopy(IDN_SWCOFF, SWCX, SWCY + 2*SWCDY);
	Picture0.AddCopy(IDN_SWCOFF, SWCX, SWCY + 3*SWCDY);

// kopie do ostatn�ch obr�zk� menu
	Picture1 = Picture0;
	Picture2 = Picture0;
	Picture3 = Picture0;
	Picture4 = Picture0;

// p�ep�na�e ostatn�ch obr�zk� menu
	if (FirstInst) Picture1.AddCopy(IDN_SWCON, SWCX, SWCY + 3*SWCDY);
	if (!FirstInst) Picture2.AddCopy(IDN_SWCON, SWCX, SWCY + 2*SWCDY);
	if (!FirstInst) Picture3.AddCopy(IDN_SWCON, SWCX, SWCY + 1*SWCDY);
	Picture4.AddCopy(IDN_SWCON, SWCX, SWCY + 0*SWCDY);

// volby ostatn�ch obr�zk� menu
	switch (Jazyk)
	{
	case JAZYKCZ:
		Picture1.AddCopy(FirstInst ? IDN_INST1_CZ : IDN_INST3_CZ, MENX, MENY + 3*MENDY);
		Picture2.AddCopy(FirstInst ? IDN_ADD3_CZ  : IDN_ADD1_CZ,  MENX, MENY + 2*MENDY);
		Picture3.AddCopy(FirstInst ? IDN_UINS3_CZ : IDN_UINS1_CZ, MENX, MENY + 1*MENDY);
		Picture4.AddCopy(IDN_END1_CZ,  MENX, MENY + 0*MENDY);
		break;

	default:
		Picture1.AddCopy(FirstInst ? IDN_INST1_EN : IDN_INST3_EN, MENX, MENY + 3*MENDY);
		Picture2.AddCopy(FirstInst ? IDN_ADD3_EN  : IDN_ADD1_EN,  MENX, MENY + 2*MENDY);
		Picture3.AddCopy(FirstInst ? IDN_UINS3_EN : IDN_UINS1_EN, MENX, MENY + 1*MENDY);
		Picture4.AddCopy(IDN_END1_EN,  MENX, MENY + 0*MENDY);
	}

// p�id�n� v�b�rov�ch kurzor�
#define SELX	173
#define SELY	11
#define SELDY	50
	Picture1.AddCopy(IDN_SELECT, SELX,	SELY + 3*SELDY);
	Picture2.AddCopy(IDN_SELECT, SELX,	SELY + 2*SELDY);
	Picture3.AddCopy(IDN_SELECT, SELX,	SELY + 1*SELDY);
	Picture4.AddCopy(IDN_SELECT, SELX,	SELY + 0*SELDY);

// n�pov�da k volb�m
#define HELPX	473
	Picture1.AddCopy(X(IDN_SEL1_CZ, IDN_SEL1_EN), HELPX, 157);
	Picture2.AddCopy(X(IDN_SEL2_CZ, IDN_SEL2_EN), HELPX, 99);
	Picture3.AddCopy(X(IDN_SEL3_CZ, IDN_SEL3_EN), HELPX, 58);
	Picture4.AddCopy(X(IDN_SEL4_CZ, IDN_SEL4_EN), HELPX, 13);

// obr�zek "Instalace dokon�ena"
	InstalOK.Load(IDN_INSOK);
	InstalOK.AddCopy(X(IDN_INSOK_CZ, IDN_INSOK_EN), 0, 0);

// obr�zek "Chcete odinstalovat?"
	DInstal.Load(IDN_DINST);
	DInstal.AddCopy(X(IDN_DINST_CZ, IDN_DINST_EN), 0, 0);

// obr�zek "Odinstalov�n� dokon�eno"
	DInstalOK.Load(IDN_DINOK);
	DInstalOK.AddCopy(X(IDN_DINOK_CZ, IDN_DINOK_EN), 0, 0);
}


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

/////////////////////////////////////////////////////////////////////////////
// na�ten� cesty Program Files

CText GetProgramFiles()
{
// text v�sledn� hodnoty
	CText txt;

// otev�en� kl��e
	HKEY hkey;
	if (::RegOpenKeyEx(
		HKEY_LOCAL_MACHINE,
		_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion"),
		0,
		KEY_QUERY_VALUE,
		&hkey) == ERROR_SUCCESS)
	{

// na�ten� hodnoty kl��e
		DWORD type = REG_SZ;
#define REGBUFSIZE 300
		BYTE buf[REGBUFSIZE+1];
		DWORD bufsize = REGBUFSIZE;

		if ((::RegQueryValueEx(
			hkey,
			_T("ProgramFilesDir"),
			0,
			&type,
			buf,
			&bufsize) == ERROR_SUCCESS) &&
			(type == REG_SZ) &&
			(bufsize > 0) &&
			(bufsize <= REGBUFSIZE))
		{
			txt.Set((char*)buf, bufsize-1);
			txt.KorigNul();
			txt.TrimLeft();
			txt.TrimRight();
		}

// uzav�en� kl��e
		::RegCloseKey(hkey);
	}

	return txt;
}

//////////////////////////////////////////////////////////////////////////////
// hl�en� chyby

#ifdef _DEBUG
void Hlaseni(LPCTSTR text)
{
	::MessageBox(MainFrame, text, NULL, MB_OK);
}
#else
#define Hlaseni(f) ((void)0)
#endif

//////////////////////////////////////////////////////////////////////////////
// obsluha enumerace oken

BOOL PeterRun = FALSE;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	TCHAR buf[64];
	CText name(buf, ::GetClassName(hwnd, buf, 64));

	if ((name == PeterMainFrameClass) || (name == PeterSetupClass))
	{
		PeterRun = TRUE;
		return FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
// hlavn� start programu

void WinMainCRTStartup()
{
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

// p��prava implicitn�ho jazyku
	switch (::GetUserDefaultLangID() & 0xff)
	{
	case 5:				// �esky
	case 21:			// polsky
	case 27:			// slovensky
		Jazyk = JAZYKCZ;
	}

// inicializace informac� o displeji
	ScreenWidth = ::GetSystemMetrics(SM_CXFULLSCREEN);
	ScreenHeight = ::GetSystemMetrics(SM_CYFULLSCREEN);

// inicializace spr�vce pam�ti
	if (!MemInit()) 
	{
MEMERROR:
		::MessageBox(NULL, 
			X(_T("Nedostatek pam�ti ke spu�t�n� instal�toru."),
				_T("Insufficient memory to run installator.")),
			X(_T("Chyba pam�ti"), _T("Memory Error")),
			MB_OK | MB_ICONSTOP);
		Exit();
	}

// inicializace funkce GetDiskFreeSpaceEx pro OSR2
	if (((OSVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) &&
		(LOWORD(OSVersionInfo.dwBuildNumber) > 1000)) ||
		(OSVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT))
	{
		HINSTANCE hinst = ::LoadLibrary(_T("KERNEL32.DLL"));

		if (hinst)
		{
#ifdef _UNICODE
			pGetDiskFreeSpaceEx = (GETDISKFREESPACEEX)
				::GetProcAddress(hinst,	"GetDiskFreeSpaceExW");
#else
			pGetDiskFreeSpaceEx = (GETDISKFREESPACEEX)
				::GetProcAddress(hinst,	"GetDiskFreeSpaceExA");
#endif
		}
	}

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

// inicializace obsluhy obr�zk�
	if (!InitPicture()) goto MEMERROR;

// inicializace glob�ln�ch objekt�
	InitTerm(__xc_a, __xc_z);

// test, zda nen� spu�t�n editor Petr (nap�. p�i zobrazen� knihovny na CD)
	::EnumWindows(EnumWindowsProc, 0);
	if (PeterRun) Exit();

// up�esn�n� jazyku
//	CText txt = GetReg(ProductKey0, _T("SetupLang"));
//	if ((txt == "5") || (txt == "21") || (txt == "27")) Jazyk = JAZYKCZ;
//	if ((txt == "7") || (txt == "9") || (txt == "12")) Jazyk = JAZYKENG;

	CText txt = GetReg(ProductKey, _T("SetupLang"));
	if ((txt == "5") || (txt == "21") || (txt == "27")) Jazyk = JAZYKCZ;
	if ((txt == "7") || (txt == "9") || (txt == "12")) Jazyk = JAZYKENG;

// na�ten� knihovny RichEdit
	::LoadLibrary(_T("RICHED32.DLL"));

// vytvo�en� bufferu k zobrazen� obr�zk�
	ClientBuf = (BYTE*)MemGet(ClientWidth * ClientHeight);

// standardn� z�hlav� bitmap
	CResource pic;
	pic.Open(IDN_PETR, RT_BITMAP);
	StdBitmapInfo = (BITMAPINFO*)MemGet(sizeof(BITMAPINFO) + sizeof(RGBQUAD)*255);
	MemCopy(StdBitmapInfo, pic.Adr(), sizeof(BITMAPINFO) + sizeof(RGBQUAD)*255);
	StdBitmapInfo->bmiHeader.biCompression = 0;

	BackColCol = StdBitmapInfo->bmiColors[BackCol].rgbRed + StdBitmapInfo->bmiColors[BackCol].rgbGreen*256
				+ StdBitmapInfo->bmiColors[BackCol].rgbBlue*256*256;
	BackBrush = ::CreateSolidBrush(BackColCol);

// vytvo�en� vlastn�ch logick�ch palet
	LOGPALETTE* pStdPalette = (LOGPALETTE*) MemGet(sizeof(PALETTEENTRY)*(256-1)+sizeof(LOGPALETTE));
	MemFill(pStdPalette, sizeof(PALETTEENTRY)*(256-1)+sizeof(LOGPALETTE), 0);
	pStdPalette->palVersion = 0x300;
	pStdPalette->palNumEntries = 250;
	PALETTEENTRY* pal = pStdPalette->palPalEntry;
	RGBQUAD* rgb = StdBitmapInfo->bmiColors;
	int i;
	for (i = 255; i >= 0; i--)
	{
		pal->peRed = rgb->rgbRed;
		pal->peGreen = rgb->rgbGreen; 
		pal->peBlue = rgb->rgbBlue;
		pal->peFlags = 0;
		pal++;
		rgb++;
	}
	StdPalette = ::CreatePalette(pStdPalette);

// kurzory
	CurPeter = ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_PETER));
	CurArrow = ::LoadCursor(0, MAKEINTRESOURCE(IDC_ARROW));
	CurWait = ::LoadCursor(0, MAKEINTRESOURCE(IDC_WAIT));

// zaji�t�n� knihovny roz���en�ch ovl�dac�ch prvk�
	::InitCommonControls();

// obr�zek checkbox�
	CheckList = ::ImageList_LoadImage(hInstance, MAKEINTRESOURCE(IDN_CHECKBOX), 20, 10, CLR_NONE, IMAGE_BITMAP, LR_DEFAULTCOLOR);

// vytvo�en� hlavn�ho okna aplikace
	if (!MainFrameCreate()) 
	{
//RESERROR:
		::MessageBox(NULL, 
			X(_T("Nedostatek syst�mov�ch prost�edk� ke spu�t�n� instal�toru."),
				_T("Insufficient system resources to run installator.")),
			X(_T("Chyba syst�mu"), _T("System Error")),
			MB_OK | MB_ICONSTOP);
		Exit();
	}

// na�ten� cesty
	InstPath = GetReg(ProductKey, _T("Path"));

// test, zda je prvn� instalace
	if (InstPath.IsNotEmpty())
	{
		FirstInst = FALSE;

// na�ten� p�ep�na��
		CText grpnum;
		CText res;
		for (i = 0; i < GROUPSNUM; i++)
		{
			grpnum.Int(i);
			bool val = ((GetReg(ProductKey, _T("Group") + grpnum) == _T('1')) != FALSE);
			GroupsCZ[i].check = val;
			GroupsCZ[i].instal = val;

			GroupsEN[i].check = val;
			GroupsEN[i].instal = val;
		}

// �schova instala�n� cesty
		SetReg(ProductKey0, _T("LastPath"), InstPath);
	}
	else
	{
		// cesta k program�m
		InstPath = GetProgramFiles();
		if (InstPath.IsEmpty()) InstPath = _T("C:\\Program Files\\");
		if (InstPath.LastChar() != _T('\\')) InstPath += _T('\\');

		InstPath += _T("Peter");
	}


// p��prava obr�zk�
	FlagCZ.Load(IDN_FLAGCZ);
	FlagENG.Load(IDN_FLAGENG);

	GemLogo.Clear(90*40, 40);
	GemLogo.AddCopy(IDN_GEMLOGO1, 0*18*40, 0);
	GemLogo.AddCopy(IDN_GEMLOGO2, 1*18*40, 0);
	GemLogo.AddCopy(IDN_GEMLOGO3, 2*18*40, 0);
	GemLogo.AddCopy(IDN_GEMLOGO4, 3*18*40, 0);
	GemLogo.AddCopy(IDN_GEMLOGO5, 4*18*40, 0);

	InitPic();

// p��prava jm�na programu
	TCHAR buf[_MAX_PATH];
	buf[0] = 0;
	::GetModuleFileName(hInstance, buf, _MAX_PATH);
	ExeFileName = buf;
	if ((int)::GetFileAttributes(ExeFileName) == -1)
	{
		ExeFileName = _T("Setup.exe");
	}

#ifdef _DEBUG
	ExeFileName = "c:\\vc\\Peter\\Gener\\Setup.exe";

	if ((int)::GetFileAttributes(ExeFileName) == -1)
	{
		Hlaseni("Nenalezen soubor c:\\vc\\Peter\\Gener\\Setup.exe");
	}

#endif // _DEBUG

// rozli�en� instal�toru podle jm�na (to je p�kn� blbost!)
	Uninst = FALSE;

	CText ttt;

	ttt = ExeFileName.Right(10);
	ttt.UpperCase();
	if (ttt == _T("UNINST.EXE")) Uninst = TRUE;
	if (ttt == _T("DEINST.EXE")) Uninst = TRUE;
	if (ttt == _T("REMOVE.EXE")) Uninst = TRUE;

	ttt = ExeFileName.Right(12);
	ttt.UpperCase();
	if (ttt == _T("UNINSTAL.EXE")) Uninst = TRUE;
	if (ttt == _T("DEINSTAL.EXE")) Uninst = TRUE;
	if (ttt == _T("DINSTALL.EXE")) Uninst = TRUE;
	if (ttt == _T("UINSTALL.EXE")) Uninst = TRUE;

// test, zda nebyla chyba pam�ti
	if (MemoryError) goto MEMERROR;

// vytvo�en� �asova�e
	Timer = ::SetTimer(MainFrame, TimerID, 55, NULL);

// zapnut� hlavn� str�nky
	if (Uninst)
	{
		if (FirstInst)
		{
			SetPage(PAGEDINSTOK);
		}
		else
		{
			SetPage(PAGEISDINST);

			if (!OpenSetup())
			{
				Exit();
			}
		}
	}
	else
	{
		if (FirstInst)
		{
			SetPage(PAGELANG);
		}
		else
		{
			SetPage(PAGEMAIN);
		}
	}

// hlavn� obslu�n� smy�ka aplikace
	MSG msg;

	while (::GetMessage(&msg, NULL, 0, 0))
	{
		MemoryError = FALSE;

		if (!PreTranslateMessage(&msg))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

// zru�en� �asova�e
	::KillTimer(MainFrame, Timer);

// konec programu
	Exit();
};

//////////////////////////////////////////////////////////////////////////////
// ukon�en� programu

void Exit()
{
	if (DelExe.IsNotEmpty())
	{
		Exec(DelExe, EmptyText, FALSE);
	}

// zru�en� palet
	::DeleteObject(StdPalette);

// ukon�en� spr�vce pam�ti
	MemTerm();

// ukon�en� programu
	ExitProcess(0);
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


//////////////////////////////////////////////////////////////////////////////
// pr�b�n� zpracov�n� zpr�v

void PumpMessage()
{
	MSG msg;

	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		MemoryError = FALSE;

		if (!PreTranslateMessage(&msg))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// hl�en� chyby �ten� ze souboru SETUP.EXE (TRUE=p�eru�it)

BOOL ReadError(CText jmeno)
{
	CText txt;
	if (Uninst)
	{
		txt = X(_T(	"Chyba �ten� odinstala�n�ch dat.\n"
					"Soubor UNINSTAL.EXE je po�kozen."),

				_T(	"Error reading uninstallation data.\n"
					"File UNINSTAL.EXE is damaged."));
	}
	else
	{
		txt = X(_T(	"Chyba �ten� instala�n�ch dat ze souboru ") + jmeno + _T(".\n"
					"Pokud byl instala�n� CD-ROM vyjmut z mechaniky,\n"
					"zasu�te jej, pros�m, zp�t. Jinak se z�ejm� jedn�\n"
					"o po�kozen� instala�n� soubor."),

				_T(	"Error reading installation data from file ") + jmeno + _T(".\n"
					"If installation CD-ROM was removed from the drive,\n"
					"insert it back. Otherwise the installation file\n"
					"is maybe damaged."));
	}

	return (::MessageBox(MainFrame,
				txt,
				X(_T("Chyba �ten� instala�n�ch dat"), _T("Error Reading Installation Data")),
				MB_RETRYCANCEL | MB_ICONERROR) != IDRETRY);
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� informac� o souborech (vrac� FALSE=p�eru�it)

BOOL OpenSetup0()
{
// otev�en� souboru SETUP.EXE
	HFile = ::CreateFile(ExeFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_READONLY, NULL);

	if (HFile == INVALID_HANDLE_VALUE) return FALSE;

// buffer k na�ten� z�hlav� souboru
	BYTE* buf;
#define BUFSIZE 0x2000
	buf = (BYTE*)MemGet(BUFSIZE);
	CBuf bf;
	bf.Adr(buf);
	bf.Size(BUFSIZE);

// na�ten� za��tku souboru
	DWORD read;
	if (!::ReadFile(HFile, buf, BUFSIZE, &read, NULL) || (read != BUFSIZE))
	{
		MemFree(buf);
		::CloseHandle(HFile);
		return FALSE;
	}

// nalezen� NT sekce
	IMAGE_NT_HEADERS* hdr = bf.NTHeader();
	if (hdr == NULL)
	{
		MemFree(buf);
		::CloseHandle(HFile);
		return FALSE;
	}

// nalezen� datov� sekce
	IMAGE_SECTION_HEADER* sec = bf.NTSection(hdr, ".setupd");
	if (sec == NULL)
	{
		MemFree(buf);
		::CloseHandle(HFile);
		return FALSE;
	}
	RawData = sec->PointerToRawData;
	RawSize = sec->SizeOfRawData;
	VirtAdr = sec->VirtualAddress;

	SizeOff = (BYTE*)&sec->SizeOfRawData - buf;
	SizeImageOff = (BYTE*)&hdr->OptionalHeader.SizeOfImage - buf;
	VirtSizeOff = (BYTE*)&sec->Misc.VirtualSize - buf;
	GDIModeOff = (BYTE*)&hdr->OptionalHeader.Subsystem - buf;

// zru�en� bufferu
	MemFree(buf);
	if (RawSize < 10) return FALSE;

// na�ten� a kontrola z�hlav� dat
	::SetFilePointer(HFile, RawData, NULL, FILE_BEGIN);

	if (!::ReadFile(HFile, &Head, sizeof(Head), &read, NULL) || 
		(read != sizeof(Head)) ||
		(Head.Ident[0] != 'S') ||
		(Head.Ident[1] != 'E') ||
		(Head.Ident[2] != 'T') ||
		(Head.Ident[3] != GROUPSNUM)
	   )
	{
		::CloseHandle(HFile);
		return FALSE;
	}

// kontroln� sou�et z�hlav�
	long check = 0;
	BYTE* data = (BYTE*)&Head + 8;
	int n = sizeof(INSTHEAD) - 8;

	for (; n > 0; n--)
	{
		check += *data;
		if (check < 0)
		{
			check *= 2;
			check++;
		}
		else
		{
			check *= 2;
		}

		data++;
	}

// na�ten� seznam� soubor�
	for (int i = 0; i < GROUPSNUM; i++)
	{
		n = Head.Groups[i].SizeFiles;
		MemFree(Groups[i].files);
		Groups[i].files = (BYTE*)MemGet(n);

		if (!::ReadFile(HFile, Groups[i].files, n, &read, NULL) || ((int)read != n))
		{
			::CloseHandle(HFile);
			return FALSE;
		}

// kontroln� sou�et seznamu soubor�
		data = Groups[i].files;

		for (; n > 0; n--)
		{
			check += *data;
			if (check < 0)
			{
				check *= 2;
				check++;
			}
			else
			{
				check *= 2;
			}

			data++;
		}
	}

// �schova za��tku dat
	DataOff = ::SetFilePointer(HFile, 0, NULL, FILE_CURRENT);

// uzav�en� souboru SETUP.EXE
	::CloseHandle(HFile);

// kontrola kontroln�ho sou�tu
	return (Head.Check == check);
}

BOOL OpenSetup()
{
	BOOL res;
	do {
		res = OpenSetup0();

		if (!res)
		{
#ifdef COMPACT
#ifdef MINI
			if (ReadError("PETERMIN.EXE")) return FALSE;
#else
			if (ReadError(_T("PETER") VerzeFileTxt _T(".EXE"))) return FALSE;
#endif
			if (ReadError("SETUP.EXE")) return FALSE;
#endif
		}

	} while (!res);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// zobrazen� po�adovan�ho m�sta

void DispDiskReq()
{
	DiskReq = 0;

	for (int i = 0; i < GROUPSNUM; i++)
	{
		if (Groups[i].check)
		{
			DiskReq += (Head.Groups[i].Size + 1023)/1024;
		}

		if (Groups[i].instal)
		{
			DiskReq -= (Head.Groups[i].Size + 1023)/1024;
		}
	}

	CText txt(_T('0'));
	if (DiskReq > 0)
	{
		txt.Int(DiskReq);
	}
	txt = _T("  ") + txt + _T(" MB");
	txt.SetWindowText(DiskReqNum);

// blokov�n� tla��tka Next
	if (ButtonNext)
	{
		BOOL enable = FALSE;
		for (int i = 0; i < GROUPSNUM; i++)
		{
			if (Groups[i].check != Groups[i].instal)
			{
				enable = TRUE;
			}
		}
		::EnableWindow(ButtonNext, enable);
	}
}


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� tla��tka s fokusem (-1 = nen�)

int GetButton()
{
	HWND wnd = ::GetFocus();
	if (wnd == NULL) return -1;

	if (wnd == ButtonBack) return IDN_BUTTONBACK;
	if (wnd == ButtonNext) return IDN_BUTTONNEXT;
	if (wnd == ButtonExit) return IDN_BUTTONEXIT;
	if (wnd == ButtonList) return IDN_BUTTONLIST;
	if (wnd == ButtonCanc) return IDN_BUTTONCANC;
	if (wnd == CheckWnd) return IDN_CHECKWND;
	if (wnd == SmlouvaWnd) return IDN_SMLOUVAWND;
	if (wnd == PathWnd) return IDN_PATHWND;
	if (wnd == FlagCZWnd) return IDN_FLAGWNDCZ;
	if (wnd == FlagENGWnd) return IDN_FLAGWNDENG;

	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� tla��tka s fokusem

void SetButton(int id)
{
	switch (id)
	{
	case IDN_BUTTONBACK:
		::SetFocus(ButtonBack);
		break;

	case IDN_BUTTONNEXT:
		::SetFocus(ButtonNext);
		break;

	case IDN_BUTTONEXIT:
		::SetFocus(ButtonExit);
		break;

	case IDN_BUTTONLIST:
		::SetFocus(ButtonList);
		break;

	case IDN_BUTTONCANC:
		::SetFocus(ButtonCanc);
		break;

	case IDN_CHECKWND:
		::SetFocus(CheckWnd);
		break;

	case IDN_SMLOUVAWND:
		::SetFocus(SmlouvaWnd);
		break;

	case IDN_PATHWND:
		::SetFocus(PathWnd);
		break;

	case IDN_FLAGWNDCZ:
		::SetFocus(FlagCZWnd);
		break;

	case IDN_FLAGWNDENG:
		::SetFocus(FlagENGWnd);
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// obsluha zpr�vy p�ed rozesl�n�m do oken (TRUE = zpr�va zpracov�na)

BOOL PreTranslateMessage(MSG* msg)
{
	WPARAM wParam = msg->wParam;

	switch (msg->message)
	{
	case WM_KEYDOWN:
		if (wParam == VK_TAB)
		{
			if (::GetKeyState(VK_SHIFT) < 0)
			{
				if (AktPage == PAGESELECT)
				{
					switch (GetButton())
					{
					case IDN_PATHWND:
						SetButton(IDN_CHECKWND);
						return TRUE;

					case IDN_BUTTONBACK:
						SetButton(IDN_PATHWND);
						return TRUE;
					}
				}
				wParam = VK_LEFT;
			}
			else
			{
				if (AktPage == PAGESELECT)
				{
					switch (GetButton())
					{
					case IDN_PATHWND:
						SetButton(IDN_BUTTONBACK);
						return TRUE;

					case IDN_CHECKWND:
						SetButton(IDN_PATHWND);
						return TRUE;
					}
				}
				wParam = VK_RIGHT;
			}
		}

		switch (AktPage)
		{
		case PAGELANG:
			switch (wParam)
			{
			case VK_ESCAPE:
				::ShowWindow(MainFrame, SW_HIDE);
				::SendMessage(MainFrame, WM_CLOSE, 0, 0);
				return TRUE;

			case VK_UP:
			case VK_LEFT:
				switch (GetButton())
				{
				case IDN_FLAGWNDCZ:
					SetButton(IDN_BUTTONEXIT);
					return TRUE;

				case IDN_FLAGWNDENG:
					SetButton(IDN_FLAGWNDCZ);
					return TRUE;

				case IDN_BUTTONEXIT:
					SetButton(IDN_FLAGWNDENG);
					return TRUE;
				}
				break;

			case VK_DOWN:
			case VK_RIGHT:
				switch (GetButton())
				{
				case IDN_FLAGWNDCZ:
					SetButton(IDN_FLAGWNDENG);
					return TRUE;

				case IDN_FLAGWNDENG:
					SetButton(IDN_BUTTONEXIT);
					return TRUE;

				case IDN_BUTTONEXIT:
					SetButton(IDN_FLAGWNDCZ);
					return TRUE;
				}
				break;

			case VK_RETURN:
				msg->wParam = VK_SPACE;
				break;
			}
			break;

		case PAGEMAIN:
			switch (wParam)
			{
			case VK_ESCAPE:
				::ShowWindow(MainFrame, SW_HIDE);
				::SendMessage(MainFrame, WM_CLOSE, 0, 0);
				return TRUE;

			case VK_DOWN:
			case VK_RIGHT:
				KeySelect++;
				if (KeySelect > 3) KeySelect = 0;
				RePaint();
				return TRUE;

			case VK_UP:
			case VK_LEFT:
				KeySelect--;
				if (KeySelect < 0) KeySelect = 3;
				RePaint();
				return TRUE;

			case VK_HOME:
				KeySelect = 0;
				RePaint();
				return TRUE;

			case VK_END:
				KeySelect = 3;
				RePaint();
				return TRUE;

			case VK_RETURN:
				switch(KeySelect)
				{
				case 0:
					if (FirstInst)
					{
						if (OpenSetup())
						{
							SetPage(PAGESELECT);
						}
					}
					return TRUE;

				case 1:
					if (!FirstInst)
					{
						if (OpenSetup())
						{
							SetPage(PAGESELECT);
						}
					}
					return TRUE;

				case 2:
					if (!FirstInst)
					{
						SetPage(PAGEISDINST);

						if (!OpenSetup())
						{
							SetPage(PAGEMAIN);
						}
					}
					return TRUE;

				case 3:
					::ShowWindow(MainFrame, SW_HIDE);
					::SendMessage(MainFrame, WM_CLOSE, 0, 0);
					return TRUE;

				default:
					goto STARTMAIN;
				}
			}
			break;

		case PAGEISDINST:
			switch (wParam)
			{
			case VK_ESCAPE:
				::ShowWindow(MainFrame, SW_HIDE);
				::SendMessage(MainFrame, WM_CLOSE, 0, 0);
				return TRUE;

			case VK_UP:
			case VK_LEFT:
				switch (GetButton())
				{
				case IDN_BUTTONBACK:
					SetButton(IDN_BUTTONEXIT);
					return TRUE;

				case IDN_BUTTONNEXT:
					SetButton(IDN_BUTTONBACK);
					return TRUE;

				case IDN_BUTTONEXIT:
					SetButton(IDN_BUTTONNEXT);
					return TRUE;
				}
				break;

			case VK_DOWN:
			case VK_RIGHT:
				switch (GetButton())
				{
				case IDN_BUTTONBACK:
					SetButton(IDN_BUTTONNEXT);
					return TRUE;

				case IDN_BUTTONNEXT:
					SetButton(IDN_BUTTONEXIT);
					return TRUE;

				case IDN_BUTTONEXIT:
					SetButton(IDN_BUTTONBACK);
					return TRUE;
				}
				break;

			case VK_RETURN:
				msg->wParam = VK_SPACE;
				break;
			}
			break;

		case PAGESELECT:
			switch (wParam)
			{
			case VK_ESCAPE:
				::ShowWindow(MainFrame, SW_HIDE);
				::SendMessage(MainFrame, WM_CLOSE, 0, 0);
				return TRUE;

			case VK_HOME:
			case VK_END:
				if (GetButton() == IDN_PATHWND) return FALSE;

			case VK_PRIOR:
			case VK_NEXT:
			case VK_UP:
			case VK_DOWN:
				SetButton(IDN_CHECKWND);
				msg->hwnd = CheckWnd;
				return FALSE;

			case VK_LEFT:
				switch (GetButton())
				{
				case IDN_PATHWND:
					return FALSE;

				case IDN_CHECKWND:
					SetButton(IDN_BUTTONLIST);
					return TRUE;

				case IDN_BUTTONBACK:
					SetButton(IDN_CHECKWND);
					return TRUE;

				case IDN_BUTTONNEXT:
					SetButton(IDN_BUTTONBACK);
					return TRUE;

				case IDN_BUTTONEXIT:
					SetButton(IDN_BUTTONNEXT);
					return TRUE;

				case IDN_BUTTONLIST:
					SetButton(IDN_BUTTONEXIT);
					return TRUE;
				}
				break;

			case VK_RIGHT:
				switch (GetButton())
				{
				case IDN_PATHWND:
					return FALSE;

				case IDN_CHECKWND:
					SetButton(IDN_BUTTONBACK);
					return TRUE;

				case IDN_BUTTONBACK:
					SetButton(IDN_BUTTONNEXT);
					return TRUE;

				case IDN_BUTTONNEXT:
					SetButton(IDN_BUTTONEXIT);
					return TRUE;

				case IDN_BUTTONEXIT:
					SetButton(IDN_BUTTONLIST);
					return TRUE;

				case IDN_BUTTONLIST:
					SetButton(IDN_CHECKWND);
					return TRUE;
				}
				break;

			case VK_RETURN:
			case VK_SPACE:
				switch (GetButton())
				{
				case IDN_CHECKWND:
					{
						int inx = ::SendMessage(CheckWnd, LVM_GETNEXTITEM, (WPARAM)-1, LVNI_SELECTED);
						if ((DWORD)inx < (DWORD)GROUPSNUM)
						{
							Groups[inx].check = !Groups[inx].check;
							LVITEM lvi;
							lvi.mask = LVIF_IMAGE;
							lvi.iItem = inx;
							lvi.iSubItem = 0;
							lvi.iImage = Groups[inx].check ? 1 : 0;
							if (Groups[inx].instal) lvi.iImage += 2;
							::SendMessage(CheckWnd, LVM_SETITEM, 0, (LPARAM)&lvi);
							DispDiskReq();
						}
					}
					break;

				default:
					msg->wParam = VK_SPACE;
				}
				break;
			}
			break;

		case PAGEINSTAL:
			switch (wParam)
			{
			case VK_ESCAPE:
				Storno = TRUE;
				break;

			case VK_RETURN:
				msg->wParam = VK_SPACE;
				break;
			}
			break;

		case PAGEOK:
		case PAGEDINSTOK:
			switch (wParam)
			{
			case VK_ESCAPE:
				::ShowWindow(MainFrame, SW_HIDE);
				::SendMessage(MainFrame, WM_CLOSE, 0, 0);
				return TRUE;

			case VK_RETURN:
				msg->wParam = VK_SPACE;
				break;
			}
			break;
		}
		break;

	case WM_KEYUP:
		switch (AktPage)
		{
		case PAGESELECT:
			if (GetButton() == IDN_CHECKWND) break;

		case PAGEISDINST:
		case PAGEINSTAL:
		case PAGEOK:
		case PAGEDINSTOK:
		case PAGELANG:
			switch (wParam)
			{
			case VK_RETURN:
				msg->wParam = VK_SPACE;
				break;
			}
		}
		break;

// stisk lev�ho tla��tka my�i
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
		if (AktPage == PAGEMAIN)
		{
STARTMAIN:
			if ((RelX >= 0) && (RelX < ClientWidth) && (RelY >= MENUTOP) && (RelY < MENUTOP + 4*MENUHEIGHT))
			{
				if (RelY < MENUTOP + MENUHEIGHT)
				{
					if (FirstInst)
					{
						if (OpenSetup())
						{
							SetPage(PAGESELECT);
						}
					}
				}
				else
				{
					if (RelY < MENUTOP + 2*MENUHEIGHT)
					{
						if (!FirstInst)
						{
							if (OpenSetup())
							{
								SetPage(PAGESELECT);
							}
						}
					}
					else
					{
						if (RelY < MENUTOP + 3*MENUHEIGHT)
						{
							if (!FirstInst)
							{
								SetPage(PAGEISDINST);

								if (!OpenSetup())
								{
									SetPage(PAGEMAIN);
								}
							}
						}
						else
						{
							::ShowWindow(MainFrame, SW_HIDE);
							::SendMessage(MainFrame, WM_CLOSE, 0, 0);
							return TRUE;
						}
					}
				}
			}
		}
		break;

	}
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// zobrazen� kurzoru �ek�n� (zah�jen� a ukon�en� mus� b�t do p�ru!)

void BeginWaitCursor()
{
// zv��en� ��ta�e �ek�n�
	Waiting++;

// zobrazen� kurzoru
	if (Waiting)
	{
		::SetCursor(CurWait);
	}
}


/////////////////////////////////////////////////////////////////////////////
// vypnut� kurzoru �ek�n� (zah�jen� a ukon�en� mus� b�t do p�ru!)

void EndWaitCursor()
{
// sn�en� ��ta�e �ek�n�
	Waiting--;

// zobrazen� kurzoru
	if (!Waiting)
	{
		if (AktPage == PAGEMAIN)
		{
			::SetCursor(CurPeter);
		}
		else
		{
			::SetCursor(CurArrow);
		}
	}
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
// dekomprese dat

#ifdef COMPACT

#define MAXLENX 25					// min. d�lka dlouh�ho �et�zce
#define MAXLEN (MAXLENX+254)		// maxim�ln� d�lka �et�zce
#define SUBSTLEN	7				// d�lka nahrazen� dlouh�m k�dem

class CDekomp
{
private:
	BYTE*	m_DstBeg;				// v�stupn� buffer - za��tek nezpracovan�ch dat
	BYTE*	m_DstEnd;				// konec v�stupn�ho bufferu
	BYTE*	m_Dst;					// ukl�dac� adresa

	BYTE*	m_SrcEnd;				// konec dat ve vstupn�m bufferu
	BYTE*	m_Src;					// �tec� adresa

	WORD	m_Status;				// st�ada� stavov�ho slova

public:
	void Init()	{ m_Status = 0;	}	// inicializace

	inline void DstBeg(BYTE* beg) { m_DstBeg = beg; }
	inline BYTE* DstBeg() { return m_DstBeg; }

	inline void DstEnd(BYTE* end) { m_DstEnd = end; }
	inline BYTE* DstEnd() { return m_DstEnd; }

	inline void Dst(BYTE* dst) { m_Dst = dst; }
	inline BYTE* Dst() { return m_Dst; }

	inline void SrcEnd(BYTE* end) { m_SrcEnd = end; }
	inline BYTE* SrcEnd() { return m_SrcEnd; }

	inline void Src(BYTE* src) { m_Src = src; }
	inline BYTE* Src() { return m_Src; }

// na�ten� jednoho bitu (v�stup = hodnota 0 nebo 1)
	inline int DekBit()
	{
		int result = m_Status & 1;
		m_Status >>= 1;

		if (m_Status == 0)
		{
			m_Status = *(WORD*)m_Src;
			m_Src++;
			m_Src++;
			result = m_Status & 1;
			m_Status >>= 1;
			m_Status |= 0x8000;
		}
		return result;
	}

// dekomprese bloku dat
//  Ve zdrojov�m bloku je pot�eba nechat za koncem dat rezervu asi tak n�jak 10 bajt� (pokud mo�no platn�ch)
//  V c�lov�m bloku je pot�eba nechat za koncem dat rezervu asi tak n�jak 300 bajt�,
//	p�ed za��tkem c�lov�ch dat je pot�eba ponechat rezervu 32 KB star�ch dat.

	void DeKomp()
	{

// lok�ln� prom�nn�
		BYTE offsetL, offsetH;			// offset k opakov�n�
		int delka;						// d�lka k opakov�n�

// kontrola p�ete�en� buffer�
		while ((m_Dst < m_DstEnd) && (m_Src < m_SrcEnd))
		{

// p�esun bajtu bez komprese
			if (DekBit() == 0)
			{
				*m_Dst = *m_Src;
				m_Dst++;
				m_Src++;
			}

// jinak bude opakov�n� �et�zce
			else
			{
				offsetH = 0;
				offsetL = 0;

// prvn� bit d�lky
				delka = DekBit();

// zv��en� ��ta�e d�lky
				for (;;)
				{
					delka++;
					delka++;

// na�ten� p��znaku konce k�du
					if (DekBit() == 0) break;

// dosa�en� maxim�ln� d�lky
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
						delka = *m_Src;
						m_Src++;
						if (delka == 255) break;	// konec dekomprese
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
					if (DekBit() == 0)
					{
						offsetH = (BYTE)DekBit();

						offsetH <<= 1;
						offsetH |= DekBit();

						offsetH <<= 1;
						offsetH |= DekBit();

						if (offsetH > 1)
						{
							offsetH <<= 1;
							offsetH |= DekBit();

							offsetH--;
							if (offsetH > 6)
							{
								offsetH <<= 1;
								offsetH |= DekBit();

								offsetH -= 7;
								if (offsetH > 13)
								{
									offsetH <<= 1;
									offsetH |= DekBit();

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
				offsetL = *m_Src;
				m_Src++;

// p�enesen� �et�zce (nepou��vat MemCopy - nep�en�� po bajtech!!!)
				BYTE* src2 = m_Dst - (WORD)(offsetL + offsetH*256);
				for (; delka > 0; delka--)
				{
					*m_Dst = *src2;
					m_Dst++;
					src2++;
				}
			}
		}
	}
};

#endif // COMPACT

/////////////////////////////////////////////////////////////////////////////
// na�ten� registru

CText GetReg(CText key, CText name)
{
// text v�sledn� hodnoty
	CText txt;

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
		DWORD type = REG_SZ;
#define REGBUFSIZE 300
		BYTE buf[REGBUFSIZE+1];
		DWORD bufsize = REGBUFSIZE;

		if ((::RegQueryValueEx(
			hkey,
			name,
			0,
			&type,
			buf,
			&bufsize) == ERROR_SUCCESS) &&
			(type == REG_SZ) &&
			(bufsize > 0) &&
			(bufsize <= REGBUFSIZE))
		{
			txt.Set((char*)buf, bufsize-1);
			txt.KorigNul();
			txt.TrimLeft();
			txt.TrimRight();
		}

// uzav�en� kl��e
		::RegCloseKey(hkey);
	}

	return txt;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� registru

void SetReg(CText key, CText name, CText data)
{
// otev�en� kl��e
	HKEY hkey;
	DWORD pos;

	if (::RegCreateKeyEx(
		HKEY_LOCAL_MACHINE,
		key,
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		NULL,
		&hkey,
		&pos) == ERROR_SUCCESS)
	{

// nastaven� hodnoty registru
		::RegSetValueEx(
			hkey,
			name,
			0,
			REG_SZ,
			(const BYTE*)(const char*)data,
			data.Length()+1);

// uzav�en� kl��e
		::RegCloseKey(hkey);
	}
}

/////////////////////////////////////////////////////////////////////////////
// zru�en� registru

void DelReg(CText key, CText name)
{
// text v�sledn� hodnoty
	CText txt;

// otev�en� kl��e
	HKEY hkey;
	if (::RegOpenKeyEx(
		HKEY_LOCAL_MACHINE,
		key,
		0,
		KEY_ALL_ACCESS,
		&hkey) == ERROR_SUCCESS)
	{

// zru�en� registru
		::RegDeleteValue(
			hkey,
			name);

// uzav�en� kl��e
		::RegCloseKey(hkey);
	}
}

/////////////////////////////////////////////////////////////////////////////
// na�ten� cesty Shell

CText GetShell(LPCTSTR name)
{
// text v�sledn� hodnoty
	CText txt;

// otev�en� kl��e
	HKEY hkey;
	if (::RegOpenKeyEx(
		HKEY_CURRENT_USER,
		_T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"),
		0,
		KEY_QUERY_VALUE,
		&hkey) == ERROR_SUCCESS)
	{

// na�ten� hodnoty kl��e
		DWORD type = REG_SZ;
#define REGBUFSIZE 300
		BYTE buf[REGBUFSIZE+1];
		DWORD bufsize = REGBUFSIZE;

		if ((::RegQueryValueEx(
			hkey,
			name,
			0,
			&type,
			buf,
			&bufsize) == ERROR_SUCCESS) &&
			(type == REG_SZ) &&
			(bufsize > 0) &&
			(bufsize <= REGBUFSIZE))
		{
			txt.Set((char*)buf, bufsize-1);
			txt.KorigNul();
			txt.TrimLeft();
			txt.TrimRight();
		}

// uzav�en� kl��e
		::RegCloseKey(hkey);
	}

	return txt;
}


/////////////////////////////////////////////////////////////////////////////
// proveden� instalace a p�einstalace

void Install()
{
	Storno = FALSE;
	int i,j;

// cesta k ukl�d�n� soubor�
	CText path = InstPath;
	if (path.LastChar() != _T('\\')) path += _T('\\');

// odinstalov�n� p�i p�einstalov�n�
	if (!FirstInst)
	{

// zji�t�n� po�tu soubor� (kv�li indikaci)
		DataSize = 0;
		DataSizeOK = 0;
		for (i = 0; i < GROUPSNUM; i++)
		{
			if (!Groups[i].check && Groups[i].instal)
			{
				DataSize += Head.Groups[i].Files;
			}
		}

// z�h�jen� odinstalov�n�
		if (DataSize > 0)
		{
			::SetWindowText(IndWndNadp, X(_T("Prob�h� odinstalov�n� ��st� aplikace Petr ..."), _T("Proceeding uninstallation of the Peter application parts ...")));

// cyklus p�es odinstalov�van� skupiny
			WIN32_FIND_DATA fnd;

			for (i = 0; i < GROUPSNUM; i++)
			{
				if (!Groups[i].check && Groups[i].instal)
				{

// vypnut� p��znaku instalace
					CText grpnum;
					grpnum.Int(i);
					SetReg(ProductKey, _T("Group") + grpnum, _T('0'));
					Groups[i].instal = false;

// cyklus p�es v�echny soubory
					BYTE* data = Groups[i].files;
					for (j = Head.Groups[i].Files; j > 0; j--)
					{

// jm�no souboru
						int size = ((INSTFILE*)data)->Size;
						CText name(((INSTFILE*)data)->Name, ((INSTFILE*)data)->NameN);
						data += 2*sizeof(long) + sizeof(BYTE) + ((INSTFILE*)data)->NameN;
						name = path + name;

// na�ten� parametr� souboru
						HANDLE file = ::FindFirstFile(name, &fnd);

// zru�en� souboru
						if (file != INVALID_HANDLE_VALUE)
						{

// zobrazen� jm�na souboru
							name.SetWindowText(IndWndFile);

							FILETIME datetime;
							::FileTimeToLocalFileTime(&fnd.ftLastWriteTime, &datetime);
							FILETIME datetime2 = datetime;
							*(__int64*)&datetime2 += (__int64)10000000*60*60;
							FILETIME datetime3 = datetime;
							*(__int64*)&datetime3 -= (__int64)10000000*60*60;

							if ((MemCompare(&datetime, &Head.DateTime, sizeof(FILETIME)) ||
								MemCompare(&datetime2, &Head.DateTime, sizeof(FILETIME)) ||
								MemCompare(&datetime3, &Head.DateTime, sizeof(FILETIME))) &&
								((int)fnd.nFileSizeLow == size))
							{
								if (fnd.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
								{
									::SetFileAttributes(name, FILE_ATTRIBUTE_NORMAL);
								}
								::DeleteFile(name);

// zru�en� adres��e
								while (name.Length() > InstPath.Length())
								{
									int pos = name.RevFind(_T('\\'));
									if (pos < 0) break;
									name.Delete(pos);
									if (!::RemoveDirectory(name)) break;
								}
							}

// uzav�en� hled�n�
							::FindClose(file);
						}

// zobrazen� indik�toru operace
						DataSizeOK++;
						::SendMessage(IndWnd, PBM_SETPOS, ::MulDiv(DataSizeOK, PROGMAX, DataSize), 0);

// obsluha zpr�v
						PumpMessage();
					}
				}
			}
		}

		::SetWindowText(IndWndNadp, X(_T("Prob�h� p�iinstalov�n� ��st� aplikace Petr ..."), _T("Proceeding add-installation of the Peter application parts ...")));
	}

// zji�t�n� velikosti soubor� (kv�li indikaci)
	DataSize = 0;
	DataSizeOK = 0;
	for (i = 0; i < GROUPSNUM; i++)
	{
		if (Groups[i].check && !Groups[i].instal)
		{
			BYTE* data = Groups[i].files;

			for (j = Head.Groups[i].Files; j > 0; j--)
			{
				DataSize += ((INSTFILE*)data)->Size;
				data += 2*sizeof(long) + sizeof(BYTE) + ((INSTFILE*)data)->NameN;
			}
		}
	}

// nen� p�iinstalov�n� (bylo jen odinstalov�n�)
	if (DataSize == 0)
	{
		SetPage(PAGEOK);
		return;
	}

// dekompresn� buffer
#ifdef COMPACT
	CDekomp dek;
#endif

// otev�en� vstupn�ho souboru
	do {
		HFile = ::CreateFile(ExeFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_READONLY, NULL);

		if (HFile == INVALID_HANDLE_VALUE)
		{
#ifdef COMPACT
#ifdef MINI
			if (ReadError("PETERMIN.EXE"))
#else
			if (ReadError(_T("PETER") VerzeFileTxt _T(".EXE")))
#endif
			if (ReadError("SETUP.EXE"))
#endif
			{
				SetPage(PAGESELECT);
				return;
			}
		}

	} while (HFile == INVALID_HANDLE_VALUE);

// vstupn� a v�stupn� buffery
#define SRCBUFSIZE 0x8000						// celkov� velikost zdrojov�ho bufferu
#define SRCBUFREZ  32							// rezerva za zdrojov�mi daty
	BYTE* srcbuf = (BYTE*)MemGet(SRCBUFSIZE + 1024);
#define DSTBUFSIZE 0x12000						// celkov� velikost c�lov�ho bufferu
#define DSTBUFDATA (DSTBUFSIZE/2)				// za��tek dat v c�lov�m bufferu
#define DSTBUFREZ  1024							// rezerva za c�lov�mi daty (a na za��tku bufferu)
	BYTE* dstbuf = (BYTE*)MemGet(DSTBUFSIZE + 1024);

// p��prava ukazatele dat
	DWORD readwrite;
	int off = DataOff;

// p�i prvn� instalaci ulo�en� licence a odinstal�toru
	if (FirstInst)
	{

// vytvo�en� c�lov�ho adres��e
		CreatePath(InstPath);

// jm�no odinstala�n�ho souboru
		CText name;
		name = path + _T("Uninstal.exe");

// vytvo�en� c�lov�ho souboru
		BOOL del = FALSE;
		::SetFileAttributes(name, FILE_ATTRIBUTE_NORMAL);
		HANDLE file = ::CreateFile(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (file != INVALID_HANDLE_VALUE)
		{

// kopie prvn�ho bloku dat souboru (s korekc� velikosti souboru)
			int size = (DataOff + 0x1ff) & ~0x1ff;
			int size0 = size;
			if (size0 > SRCBUFSIZE) size0 = SRCBUFSIZE;

			if (::ReadFile(HFile, srcbuf, size0, &readwrite, NULL) && (size0 == (int)readwrite))
			{
				*(long*)(srcbuf + SizeOff) = (size - RawData + 0x1ff) & ~0x1ff;
				*(long*)(srcbuf + SizeImageOff) = (VirtAdr + size - RawData + 0x1fff) & ~0x1fff;
				*(long*)(srcbuf + VirtSizeOff) = size - RawData;
				*(WORD*)(srcbuf + GDIModeOff) = IMAGE_SUBSYSTEM_WINDOWS_GUI;

				if (::WriteFile(file, srcbuf, size0, &readwrite, NULL) && (size0 == (int)readwrite))
				{
					size -= size0;

// kopie dat souboru
					while (size > 0)
					{
						size0 = size;
						if (size0 > SRCBUFSIZE) size0 = SRCBUFSIZE;
						size -= size0;

						if (!::ReadFile(HFile, srcbuf, size0, &readwrite, NULL) ||
							(size0 != (int)readwrite) ||
							!::WriteFile(file, srcbuf, size0, &readwrite, NULL) ||
							(size0 != (int)readwrite))
						{
							del = TRUE;
							break;
						}
					}
				}
				else
				{
					del = TRUE;
				}
			}
			else
			{
				del = TRUE;
			}

// nastaven� data a �asu souboru
			FILETIME datetime;
			::LocalFileTimeToFileTime(&Head.DateTime, &datetime);
			::SetFileTime(file, NULL, NULL, &datetime);

// uzav�en� c�lov�ho souboru
			::CloseHandle(file);
		}

// p�i chyb� zru�en� souboru
		if (del) ::DeleteFile(name);

// obsluha zpr�v
		PumpMessage();
	}

// cyklus p�es v�echny skupiny
	for (i = 0; i < GROUPSNUM; i++)
	{

// test, zda m� b�t skupina instalov�na
		if (Groups[i].check && !Groups[i].instal)
		{

// na�ten� prvn�ho bloku dat skupiny
			BOOL ok;

#ifdef COMPACT
			int srcsize = Head.Groups[i].SizeGroup;						// velikost dat skupiny
			int srcsize0 = srcsize;
			if (srcsize0 > SRCBUFSIZE) srcsize0 = SRCBUFSIZE;			// velikost dat k naeten� do bufferu
			srcsize -= srcsize0;

			do {
				::SetFilePointer(HFile, off, NULL, FILE_BEGIN);
				ok = (
						(srcsize0 > 0) && 
						::ReadFile(HFile, srcbuf, srcsize0, &readwrite, NULL) && 
						(srcsize0 == (int)readwrite) &&
						(srcbuf[0] == 27) &&
						(srcbuf[1] == 'X') &&
						(srcbuf[2] == 'C') &&
						(srcbuf[3] == 'D') &&
						(srcbuf[4] == 0) &&
						(srcbuf[5] == 0)
					);

				if (!ok)
				{
					if (ReadError("PETERMIN.EXE"))
					{
						MemFree(srcbuf);
						MemFree(dstbuf);
						SetPage(PAGESELECT);
						return;
					}
				}

			} while (!ok);

// prubi�n� ukazatel eten�ch dat
			int off0 = off + srcsize0;

// po�prava dekompresn�ho bufferu - poe�teen� dekomprese
			dek.Init();
			dek.DstBeg(dstbuf + DSTBUFDATA);
			dek.DstEnd(dstbuf + DSTBUFSIZE - DSTBUFREZ);
			dek.Dst(dstbuf + DSTBUFDATA);
			if (srcsize0 == SRCBUFSIZE)
			{
				dek.SrcEnd(srcbuf + srcsize0 - SRCBUFREZ);
			}
			else
			{
				dek.SrcEnd(srcbuf + srcsize0);
			}
			dek.Src(srcbuf + 6);
			dek.DeKomp();
#endif // COMPACT

// cyklus p�es v�echny soubory
			BYTE* data = Groups[i].files;
			for (j = Head.Groups[i].Files; j > 0; j--)
			{

// parametry souboru
				int check0 = 0;
				int size = ((INSTFILE*)data)->Size;
				int check = ((INSTFILE*)data)->Check;
				CText name(((INSTFILE*)data)->Name, ((INSTFILE*)data)->NameN);
#ifndef COMPACT
				CText name0(name);
				if (name0 == "PETER.EXE") name0 = "SETUP.DAT";
#endif
				data += 2*sizeof(long) + sizeof(BYTE) + ((INSTFILE*)data)->NameN;
				name = path + name;
				name.SetWindowText(IndWndFile);

// vytvo�en� c�lov�ho souboru
				HANDLE file = ::CreateFile(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

// p�i ne�sp�chu druh� pokus - s vytvo�en�m adres��e
				if (file == INVALID_HANDLE_VALUE)
				{
					CreatePath(name.Left(name.RevFind(_T('\\'))));
					file = ::CreateFile(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

// dal�� pokus - zru�en� atributu R/O souboru
					if (file == INVALID_HANDLE_VALUE)
					{
						::SetFileAttributes(name, FILE_ATTRIBUTE_NORMAL);
						file = ::CreateFile(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
					}
				}

// obsluha zpr�v
				PumpMessage();

// chyba - soubor nelze vytvo�it
				if (file == INVALID_HANDLE_VALUE)
				{
					::MessageBox(MainFrame,
						X(
						CText(
							_T("Chyba - nelze vytvo�it c�lov� soubor\n\n") +
							name +
							_T("\n\nInstalace nem��e d�le pokra�ovat a bude p�eru�ena. D�vodem m��e b�t:"
								"\n   - p�episovan� c�lov� program EXE je spu�t�n"
								"\n   - p�episovan� c�lov� soubor je otev�en n�kter�m programem"
								"\n   - chybn� zad�n� c�lov� slo�ky (nap�. neexistuj�c� disk)"
								"\n   - pln� c�lov� disk nebo z�kaz z�pisu na disk")),
						CText(
							_T("Error - could not create destination file\n\n") +
							name +
							_T("\n\nInstallation can't continue and will be interrupted. It can be caused by:"
								"\n   - overwriten destination EXE program is running"
								"\n   - overwriten destination file is opened by some other program"
								"\n   - wrongly entered destination folder (e.g. invalid disk)"
								"\n   - destination disk is full or it is write protected"))),
						X(_T("Chyba z�pisu"), _T("Write Error")),
						MB_OK | MB_ICONERROR);

					MemFree(srcbuf);
					MemFree(dstbuf);
					SetPage(PAGESELECT);
					return;
				}

// otev�en� vstupn�ho souboru
#ifndef COMPACT
				HANDLE ifile;
				do {
					ifile = ::CreateFile(name0, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
						FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_READONLY, NULL);

					if (ifile == INVALID_HANDLE_VALUE)
					{
						if (ReadError(name0))
						{
							::CloseHandle(file);
							::DeleteFile(name);

							MemFree(srcbuf);
							MemFree(dstbuf);
							SetPage(PAGESELECT);
							return;
						}
					}

				} while (ifile == INVALID_HANDLE_VALUE);
#endif // COMPACT

// cyklus ulo�en� v�ech dat
				while (size > 0)
				{
#ifdef COMPACT
// velikost bloku dat poipraven�ch v bufferu
					int size0 = dek.Dst() - dek.DstBeg();
					if (size0 <= 0)
					{

// naeten� dal��ho bloku zdrojo�ch dat
						if (srcsize <= 0)		// aby bylo je�ti nico naeteno za koncem dat
						{
							srcsize = 1024;
							off0 -= 1024;
						}

						int n = (srcbuf + SRCBUFSIZE) - dek.Src();
						if (n < 0) n = 0;
						MemCopy(srcbuf, dek.Src(), n);
						srcsize0 = srcsize;
						if (srcsize0 > SRCBUFSIZE - n) srcsize0 = SRCBUFSIZE - n;

						srcsize -= srcsize0;

						do {
							::SetFilePointer(HFile, off0, NULL, FILE_BEGIN);
							ok = (
									::ReadFile(HFile, srcbuf + n, srcsize0, &readwrite, NULL) && 
									(srcsize0 == (int)readwrite)
								);

							if (!ok)
							{

								if (ReadError("PETERMIN.EXE"))
								{
									::CloseHandle(file);
									::DeleteFile(name);

									MemFree(srcbuf);
									MemFree(dstbuf);
									SetPage(PAGESELECT);
									return;
								}
							}

						} while (!ok);

						off0 += srcsize0;

// posun ukazatelu a dat v bufferu
						if ((srcsize0 + n) == SRCBUFSIZE)
						{
							dek.SrcEnd(srcbuf + n + srcsize0 - SRCBUFREZ);
						}
						else
						{
							dek.SrcEnd(srcbuf + n + srcsize0);
						}
						dek.Src(srcbuf);

						n = dek.Dst() - (dstbuf + DSTBUFDATA);
						MemCopy(dstbuf + DSTBUFDATA - n, dstbuf + DSTBUFDATA, n);

						dek.DstBeg(dstbuf + DSTBUFDATA);
						dek.DstEnd(dstbuf + DSTBUFSIZE - DSTBUFREZ);
						dek.Dst(dstbuf + DSTBUFDATA);

						dek.DeKomp();

						size0 = dek.Dst() - dek.DstBeg();
					}
#else // COMPACT
					int size0 = DSTBUFSIZE;
					if (size0 > size) size0 = size;
					do {
						ok = ( ::ReadFile(ifile, dstbuf, size0, &readwrite, NULL) &&
										(size0 == (int)readwrite)
									);

						if (!ok)
						{
							if (ReadError(name0))
							{
								::CloseHandle(file);
								::DeleteFile(name);
								::CloseHandle(ifile);

								MemFree(srcbuf);
								MemFree(dstbuf);
								SetPage(PAGESELECT);
								return;
							}
						}
					} while (!ok);
#endif // COMPACT

// z�pis bloku dat
					if (size0 > size) size0 = size;
					size -= size0;
#ifdef COMPACT
					if (!::WriteFile(file, dek.DstBeg(), size0, &readwrite, NULL) || (size0 != (int)readwrite))
#else
					if (!::WriteFile(file, dstbuf, size0, &readwrite, NULL) || (size0 != (int)readwrite))
#endif
					{
						::CloseHandle(file);
						::DeleteFile(name);
#ifndef COMPACT
						::CloseHandle(ifile);
#endif
						::MessageBox(MainFrame,
							X(
							CText(
								_T("Chyba z�pisu do c�lov�ho souboru\n\n") +
								name +
								_T("\n\nC�lov� disk je z�ejm� pln�."
									"\nInstalace nem��e d�le pokra�ovat a bude p�eru�ena.")),
							CText(
								_T("Error writing destination file\n\n") +
								name +
								_T("\n\nDestination disk is maybe full."
									"\nInstallation can't continue and will be interrupted."))),
							X(_T("Chyba z�pisu"), _T("Write Error")),
							MB_OK | MB_ICONERROR);

						MemFree(srcbuf);
						MemFree(dstbuf);
						SetPage(PAGESELECT);
						return;
					}
#ifdef COMPACT
					BYTE* d = dek.DstBeg();
					dek.DstBeg(dek.DstBeg() + size0);
#else
					BYTE* d = dstbuf;
#endif

// zobrazen� indik�toru operace
					DataSizeOK += size0;
					::SendMessage(IndWnd, PBM_SETPOS, ::MulDiv(DataSizeOK, PROGMAX, DataSize), 0);

// obsluha zpr�v
					PumpMessage();

// kontroln� sou�et bloku dat
					for (; size0 > 0; size0--)
					{
						check0 += *d;
						if (check0 < 0)
						{
							check0 *= 2;
							check0++;
						}
						else
						{
							check0 *= 2;
						}

						d++;
					}
				}

// nastaven� data a �asu souboru
				FILETIME datetime;
				::LocalFileTimeToFileTime(&Head.DateTime, &datetime);
				::SetFileTime(file, NULL, NULL, &datetime);

// uzav�en� c�lov�ho souboru
				::CloseHandle(file);
#ifndef COMPACT
				::CloseHandle(ifile);
#endif

// kontrola kontroln�ho sou�tu souboru
				if (check != check0)
				{
					::DeleteFile(name);

					::MessageBox(MainFrame,
						X(
						CText(
							_T("Chyba instalace souboru\n\n") +
							name +
#ifdef COMPACT
#ifdef MINI
							_T("\n\nInstala�n� program PETERMIN.EXE je po�kozen.") +
#else
							_T("\n\nInstala�n� program PETER") VerzeFileTxt _T(".EXE je po�kozen.") +
#endif
#else
							_T("\n\nSoubor ") + name0 + _T(" je po�kozen.") +
#endif
								_T("\nInstalace nem��e d�le pokra�ovat a bude p�eru�ena.")),

						CText(
							_T("Installation error of the file\n\n") +
							name +
#ifdef COMPACT
#ifdef MINI
							_T("\n\nInstallation program PETERMIN.EXE is damaged.") +
#else
							_T("\n\nInstallation program PETER") VerzeFileTxt _T(".EXE is damaged.") +
#endif
#else
							_T("\n\nFile ") + name0 + _T(" is damaged.") +
#endif
								_T("\nInstallation can't continue and will be interrupted."))),

						X(_T("Chyba instalace"), _T("Installation Error")),
						MB_OK | MB_ICONERROR);

					MemFree(srcbuf);
					MemFree(dstbuf);
					SetPage(PAGESELECT);
					return;
				}

// obsluha zpr�v
				PumpMessage();

// p�eru�en� instalace
				if (Storno)
				{
					if (::MessageBox(MainFrame,
							X(
							CText(
								_T("P�eru�en�m instalace z�stanou ve slo�ce ") +
								InstPath +
								_T("\nji� nainstalovan� soubory. Soubory m��ete zru�it Pr�zkumn�kem."
									"\n\nOpravdu chcete instalaci p�eru�it?")),
							CText(
								_T("There will remain already installed files in folder ") +
								InstPath +
								_T("\nif you break installation. You can delete those files by File Explorer."
									"\n\nDo you really want to break the installation?"))),

							X(_T("P�eru�en� instalace"), _T("Installation Break")),
							MB_YESNO | MB_ICONWARNING) == IDYES)
					{
						MemFree(srcbuf);
						MemFree(dstbuf);
						SetPage(PAGESELECT);
						return;
					}
					SetButton(IDN_BUTTONCANC);
				}
				Storno = FALSE;
			}
		}

// posun ukazatele dat
		off += Head.Groups[i].SizeGroup;
	}

// zru�en� vypisovan�ho textu souboru
	EmptyText.SetWindowText(IndWndFile);

// zru�en� bufferu
	MemFree(srcbuf);
	MemFree(dstbuf);

// registrace c�lov� cesty
	SetReg(ProductKey0, _T("LastPath"), InstPath);
	SetReg(ProductKey, _T("Path"), InstPath);

// registrace jazyku
	SetReg(ProductKey0, _T("SetupLang"), X("5", "9"));
	SetReg(ProductKey, _T("SetupLang"), X("5", "9"));

// registrace volby skupin
	CText grpnum;
	for (i = 0; i < GROUPSNUM; i++)
	{
		grpnum.Int(i);
		SetReg(ProductKey, _T("Group") + grpnum, Groups[i].check ? _T('1') : _T('0'));
	}

// inicializace p�i prvn� instalaci
	if (FirstInst)
	{

// registrace odinstalov�n� (ve Windows NT nen�)
		HKEY hkey;
		if (::RegOpenKeyEx(
			HKEY_LOCAL_MACHINE,
			UninstalKey0,
			0,
			KEY_QUERY_VALUE,
			&hkey) == ERROR_SUCCESS)
		{
			::RegCloseKey(hkey);
		
			SetReg(UninstalKey, _T("DisplayName"), X(_T("Petr "), _T("Peter ")) + CText(VerzeTxt));
			SetReg(UninstalKey, _T("UninstallString"), path + _T("Uninstal.exe"));
		}

// cesta k dokument�m
		CText dokumenty;
		dokumenty = GetShell(_T("Personal"));
		if (dokumenty.IsEmpty()) dokumenty = X(_T("C:\\Dokumenty"), _T("C:\\Dokuments"));

// vytvo�en� pracovn�ho adres��e
		if (dokumenty.LastChar() != _T('\\')) dokumenty += _T('\\');
		dokumenty += _T("Peter");
		CreatePath(dokumenty);
		SetReg(ProductKey, _T("User"), dokumenty);

// vytvo�en� konfigura�n�ho souboru (p��padn� existuj�c� soubor se p�ep�e)
		CText peterini;
		peterini = dokumenty + _T("\\Peter.ini");
		HANDLE file = ::CreateFile(peterini, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (file != INVALID_HANDLE_VALUE)
		{
			DWORD writen;
			CText txt("[Global]");
			txt += 13;
			txt += 10;

			txt += "Language=";
			txt += 34;
			txt += X("Czech", "English");
			txt += 34;
			txt += 13;
			txt += 10;

			txt += "LangPrev=";
			txt += 34;
			txt += X("English", "Czech");
			txt += 34;
			txt += 13;
			txt += 10;

			::WriteFile(file, (LPCSTR)txt, txt.Length(), &writen, NULL);

			::CloseHandle(file);
		}

// cesta k menu program�
		CText programs;
		programs = GetShell(_T("Programs"));
		if (programs.LastChar() != _T('\\')) programs += _T('\\');
		programs += X(_T("Petr "), _T("Peter "));
		programs += VerzeTxt;
		CreatePath(programs);
		SetReg(ProductKey, _T("Menu"), programs);
		programs += _T('\\');

// cesta k hlavn� plo�e
		CText desktop;
		desktop = GetShell(_T("Desktop"));
		SetReg(ProductKey, _T("Desktop"), desktop);
		if (desktop.LastChar() != _T('\\')) desktop += _T('\\');

// p�ikop�rov�n� datov�ch soubor� do pracovn�ho adres��e
		CText txt;

// inicializace COM knihovny
		if (::CoInitialize(NULL) == S_OK)
		{

// interface na IShellLink
			IShellLink* psl;
			if (::CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
					IID_IShellLink, (void**)&psl) == S_OK)
			{

// interface na IPersistFile
				IPersistFile* ppf;
				if (psl->QueryInterface(IID_IPersistFile, (void **)&ppf) == S_OK)
				{
					WCHAR dest[512];

// ulo�en� odkazu do plochy
					psl->SetPath(path + _T("Peter.exe"));
					psl->SetDescription(X(_T("Petr"), _T("Peter")));
					psl->SetWorkingDirectory(dokumenty);
					txt = desktop;
					txt += X(_T("Petr.lnk"), _T("Peter.lnk"));
					txt.WriteNull(dest);
					ppf->Save(dest, TRUE);

// ulo�en� odkazu do menu - pracovn� instalace
					psl->SetPath(path + _T("Peter.exe"));
					psl->SetDescription(X(_T("Petr"), _T("Peter")));
					psl->SetWorkingDirectory(dokumenty);
					txt = programs;
					txt += X(_T("Petr.lnk"), _T("Peter.lnk"));
					txt.WriteNull(dest);
					ppf->Save(dest, TRUE);

// ulo�en� odkazu do menu - spr�vce knihovny
					psl->SetPath(path + _T("Peter.exe"));
					psl->SetDescription(X(_T("Petr s modifikac� vzorov� knihovny"), _T("Peter with modification of example library")));
					psl->SetWorkingDirectory(InstPath);
					txt = programs;
					txt += X(_T("Petr s modifikac� vzorov� knihovny.lnk"), _T("Peter with modification of example library.lnk"));
					txt.WriteNull(dest);
					ppf->Save(dest, TRUE);

// ulo�en� odkazu do menu - odinstalov�n�
					psl->SetPath(path + _T("Uninstal.exe"));
					psl->SetDescription(X(_T("Odinstalov�n�"), _T("Uninstall")));
					psl->SetWorkingDirectory(InstPath);
					txt = programs;
					txt += X(_T("Odinstalov�n�.lnk"), _T("Uninstall.lnk"));
					txt.WriteNull(dest);
					ppf->Save(dest, TRUE);

// uvoln�n� interface IPersistFile
					ppf->Release();
				}

// uvoln�n� interface na IShellLink
				psl->Release();
			}

// deinicializace COM knihovny
			::CoUninitialize();
		}
	}

// instalace ukon�ena OK
	SetPage(PAGEOK);
}


/////////////////////////////////////////////////////////////////////////////
// zru�en� cesty s podadres��i (neru�� soubory)

void DeletePath(CText path)
{
	CText txt;

// dopln�n� "\" na konec
	if (path.LastChar() != _T('\\')) path += _T('\\');

// vyhled�n� podadres���
	WIN32_FIND_DATA fnd;
	HANDLE file = ::FindFirstFile(path + _T("*.*"), &fnd);
	if (file != INVALID_HANDLE_VALUE)
	{
		do {
			if (fnd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				txt = fnd.cFileName;
				if (txt.IsNotEmpty() && (txt != _T(".")) && (txt != _T("..")))
				{
					DeletePath(path + txt);
				}
			}
		} while (::FindNextFile(file, &fnd));

		::FindClose(file);
	}
	path.Delete(path.Length()-1);
	::RemoveDirectory(path);
}

/////////////////////////////////////////////////////////////////////////////
// proveden� odinstalov�n�

void DInstall()
{
	BeginWaitCursor();

// cesta k ru�en� soubor�
	CText path = InstPath;
	if (path.LastChar() != _T('\\')) path += _T('\\');

	Storno = FALSE;
	int i,j;
	WIN32_FIND_DATA fnd;

// cesta k dokument�m
	CText dokumenty;
	dokumenty = GetReg(ProductKey, _T("User"));

// cesta k menu
	CText programs;
	programs = GetReg(ProductKey, _T("Menu"));

// cesta k hlavn� plo�e
	CText desktop;
	desktop = GetReg(ProductKey, _T("Desktop"));
	if (desktop.LastChar() != _T('\\')) desktop += _T('\\');
	desktop += X(_T("Petr.lnk"), _T("Peter.lnk"));

// zru�en� registr� cest
	SetReg(ProductKey, _T("Path"), EmptyText);
	DelReg(ProductKey, _T("Path"));
	SetReg(ProductKey, _T("User"), EmptyText);
	DelReg(ProductKey, _T("User"));
	SetReg(ProductKey, _T("Menu"), EmptyText);
	DelReg(ProductKey, _T("Menu"));
	SetReg(ProductKey, _T("Desktop"), EmptyText);
	DelReg(ProductKey, _T("Desktop"));

// zru�en� registr� skupin
	CText grpnum;
	for (i = 0; i < GROUPSNUM; i++)
	{
		grpnum.Int(i);
		SetReg(ProductKey, _T("Group") + grpnum, EmptyText);
		DelReg(ProductKey, _T("Group") + grpnum);
	}

// zru�en� kl��e
	::RegDeleteKey(
		HKEY_LOCAL_MACHINE,
		ProductKey);

// zru�en� posledn� cesty
	SetReg(ProductKey0, _T("LastPath"), EmptyText);
	DelReg(ProductKey0, _T("LastPath"));

// zru�en� registrace odinstalov�n� (ve Windows NT nen�)
	HKEY hkey;
	if (::RegOpenKeyEx(
		HKEY_LOCAL_MACHINE,
		UninstalKey0,
		0,
		KEY_QUERY_VALUE,
		&hkey) == ERROR_SUCCESS)
	{
		::RegCloseKey(hkey);

		SetReg(UninstalKey, _T("DisplayName"), EmptyText);
		SetReg(UninstalKey, _T("UninstallString"), EmptyText);
		DelReg(UninstalKey, _T("DisplayName"));
		DelReg(UninstalKey, _T("UninstallString"));

		::RegDeleteKey(
			HKEY_LOCAL_MACHINE,
			UninstalKey);
	}

// zru�en� menu
	CText txt;
	if (programs.IsNotEmpty())
	{
		txt = programs;
		if (txt.LastChar() != _T('\\')) txt += _T('\\');
		::DeleteFile(txt + X(_T("Petr.lnk"), _T("Peter.lnk")));
		::DeleteFile(txt + X(_T("Petr s modifikac� vzorov� knihovny.lnk"), _T("Peter with modification of example library.lnk")));
		::DeleteFile(txt + X(_T("Odinstalov�n�.lnk"), _T("Uninstall.lnk")));
		::RemoveDirectory(programs);
	}

// --------- zru�en� odkazu z hlavn� plochy (s kontrolou platnosti)

// inicializace COM knihovny
	if (::CoInitialize(NULL) == S_OK)
	{
		BOOL del = FALSE;

// interface na IShellLink
		IShellLink* psl;
		if (::CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
				IID_IShellLink, (void**)&psl) == S_OK)
		{

// interface na IPersistFile
			IPersistFile* ppf;
			if (psl->QueryInterface(IID_IPersistFile, (void **)&ppf) == S_OK)
			{
				WCHAR dest[512];

				desktop.WriteNull(dest);
				if (ppf->Load(dest, STGM_SIMPLE | STGM_READ | STGM_SHARE_DENY_NONE) == S_OK)
				{
					TCHAR buf[512];
					if (psl->GetPath(buf, 512, &fnd, 0) == NOERROR)
					{
						txt = buf;
						txt.UpperCase();

						if (psl->GetWorkingDirectory(buf, 512) == NOERROR)
						{
							CText txt2 = buf;

							CText txt3 = path + _T("PETER.EXE");
							txt3.UpperCase();

							if ((txt == txt3) &&
								(txt2 == dokumenty))
							{
								del = TRUE;
							}
						}
					}

// uvoln�n� interface IPersistFile
					ppf->Release();
				}

// uvoln�n� interface na IShellLink
				psl->Release();
			}

// deinicializace COM knihovny
			::CoUninitialize();
		}

// zru�en� souboru odkazu
		if (del) 
		{
			::DeleteFile(desktop);
		}
	}

// zru�en� datov�ch soubor�
	FirstInst = TRUE;

// zji�t�n� po�tu soubor� (kv�li indikaci)
	DataSize = 0;
	DataSizeOK = 0;
	for (i = 0; i < GROUPSNUM; i++)
	{
		DataSize += Head.Groups[i].Files;
	}

// cyklus p�es v�echny skupiny
	for (i = 0; i < GROUPSNUM; i++)
	{

// cyklus p�es v�echny soubory
		BYTE* data = Groups[i].files;
		for (j = Head.Groups[i].Files; j > 0; j--)
		{

// jm�no souboru
			int size = ((INSTFILE*)data)->Size;
			CText name(((INSTFILE*)data)->Name, ((INSTFILE*)data)->NameN);
			data += 2*sizeof(long) + sizeof(BYTE) + ((INSTFILE*)data)->NameN;
			name = path + name;

// na�ten� parametr� souboru
			HANDLE file = ::FindFirstFile(name, &fnd);

// zru�en� souboru
			if (file != INVALID_HANDLE_VALUE)
			{
				FILETIME datetime;
				::FileTimeToLocalFileTime(&fnd.ftLastWriteTime, &datetime);
				FILETIME datetime2 = datetime;
				*(__int64*)&datetime2 += (__int64)10000000*60*60;
				FILETIME datetime3 = datetime;
				*(__int64*)&datetime3 -= (__int64)10000000*60*60;

				if ((MemCompare(&datetime, &Head.DateTime, sizeof(FILETIME)) ||
					MemCompare(&datetime2, &Head.DateTime, sizeof(FILETIME)) ||
					MemCompare(&datetime3, &Head.DateTime, sizeof(FILETIME))) &&
					((int)fnd.nFileSizeLow == size))
				{
					if (fnd.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
					{
						::SetFileAttributes(name, FILE_ATTRIBUTE_NORMAL);
					}
					::DeleteFile(name);
				}

// uzav�en� hled�n�
				::FindClose(file);
			}

// zobrazen� indik�toru operace
			DataSizeOK++;
			if (IndWnd)
			{
				::SendMessage(IndWnd, PBM_SETPOS, ::MulDiv(DataSizeOK, PROGMAX, DataSize), 0);
			}

// obsluha zpr�v
			PumpMessage();
		}
	}

// zru�en� souboru INI
	::DeleteFile(path + _T("Peter.ini"));

	::SetFileAttributes(path + _T("Peter.gid"), FILE_ATTRIBUTE_NORMAL);
	::DeleteFile(path + _T("Peter.gid"));

// zru�en� souboru Uninstal.exe
	txt = path + _T("Uninstal.exe");
	BOOL res = ::DeleteFile(txt);
	if (Uninst && !res)
	{
		CResource rsrc;
		rsrc.OpenCopy(IDN_DELEXE, _T("EXE"));

		char* dat = (char*)rsrc.Adr();
		int i;
		for (i = rsrc.Size(); i > 0; i--)
		{
			if ((*(long*)dat == 'ELIF') &&
				(*(long*)(dat + 4) == 'ELIF'))
			{
				txt.WriteNull(dat);
				break;
			}
			dat++;
		}

		dat = (char*)rsrc.Adr();
		for (i = rsrc.Size(); i > 0; i--)
		{
			if ((*(long*)dat == 'HTAP') &&
				(*(long*)(dat + 4) == 'HTAP'))
			{
				InstPath.WriteNull(dat);
				break;
			}
			dat++;
		}

		TCHAR tmppath[201];
		tmppath[0] = 0;
		::GetTempPath(200, tmppath);

		CText pth = tmppath;
		if (pth.LastChar() != _T('\\')) pth += _T('\\');

		for (i = 0; i < 1000; i++)
		{
			txt.Int(i);
			txt = pth + _T("~TEMP") + txt + _T(".EXE");

			if ((int)::GetFileAttributes(txt) == -1) break;
		}

		HANDLE file = ::CreateFile(txt, GENERIC_WRITE,
			0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);

		if (file != INVALID_HANDLE_VALUE)
		{
			DWORD writen;
			::WriteFile(file, rsrc.Adr(), rsrc.Size(), &writen, NULL);
			::CloseHandle(file);
			DelExe = txt;
		}
	}

// zru�en� adres���
	DeletePath(InstPath);

	EndWaitCursor();
	SetPage(PAGEDINSTOK);
}


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� informac� o voln�m m�stu na c�lov�m disku

void GetDiskSpace()
{
// �prava na pln� jm�no
	InstPath.TrimLeft();
	InstPath.TrimRight();

	TCHAR buf[1024];
	TCHAR* jmeno;
	int len = ::GetFullPathName(InstPath, 1024, buf, &jmeno);
	if (len > 0)
	{
		InstPath.Term();
		InstPath.Init(buf, len);
		InstPath.SetWindowText(PathWnd);
	}

// disk
	CText txt;
	txt = InstPath.Left(3);

// na�ten� informac� nov�j�� funkc�
	if (!pGetDiskFreeSpaceEx ||
		!pGetDiskFreeSpaceEx(txt, &DiskFreeUser, &DiskSize, &DiskFree))

// na�ten� informac� star�� metodou
	{
		DWORD sectc;
		DWORD bytes;
		DWORD freec;
		DWORD sizec;

		if (!::GetDiskFreeSpace(txt, &sectc, &bytes, &freec, &sizec))
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

// voln� m�sto
	DiskFre = *(long*)((BYTE*)&DiskFree + 2) >> 4;

// zobrazen� informac�
	txt.Int(DiskFre);
	txt += _T(" MB");
	txt.SetWindowText(DiskFreeNum);
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� ��ry nad tla��tky

void CreateLineWnd()
{
	if (LineWnd == NULL)
	{
		LineWnd = ::CreateWindowEx(
			0,												// roz���en� styl
			_T("STATIC"),									// jm�no t��dy okna
			NULL,											// titulek okna
			SS_ETCHEDHORZ | WS_VISIBLE | WS_CHILD,			// styl okna
			10,												// X
			ClientHeight - 58,								// Y
			ClientWidth - 20,								// ���ka
			6,												// v��ka
			MainFrame,										// rodi�
			(HMENU)IDN_LINEWND,								// menu
			hInstance,										// instance
			NULL);											// parametry
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� r�mu u�ivatele

void CreateUserRam()
{
// r�m kolem okna
	if (UserRam == NULL)
	{
		UserRam = ::CreateWindowEx(
			0,												// roz���en� styl
			_T("STATIC"),									// jm�no t��dy okna
			NULL,											// titulek okna
			SS_ETCHEDFRAME | WS_VISIBLE | WS_CHILD,			// styl okna
			320,											// X
			40,												// Y
			ClientWidth - 320 - 10,							// ���ka
			150,											// v��ka
			MainFrame,										// rodi�
			(HMENU)IDN_USERRAM,								// menu
			hInstance,										// instance
			NULL);											// parametry
	}
}

/////////////////////////////////////////////////////////////////////////////
// vlo�en� polo�ky do seznamu p�ep�na��

void InsertCheck(int inx, BOOL check, BOOL instal, LPCTSTR text, LPCTSTR text2)
{
	LVITEM lvi;

	lvi.iSubItem = 0;
	lvi.iItem = inx;
	lvi.iImage = check ? 1 : 0;
	if (instal) lvi.iImage += 2;
	lvi.mask = LVIF_TEXT | LVIF_IMAGE;
	lvi.pszText = (LPTSTR)text;
	::SendMessage(CheckWnd, LVM_INSERTITEM, 0, (LPARAM)&lvi);

	lvi.iSubItem = 1;
	lvi.mask = LVIF_TEXT;
	lvi.pszText = (LPTSTR)text2;
	::SendMessage(CheckWnd, LVM_SETITEM, inx, (LPARAM)&lvi);
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� okna p�ep�na��

void CreateCheckWnd()
{
	if (CheckWnd == NULL)
	{

// vytvo�en� okna
		CheckWnd = ::CreateWindowEx(
			WS_EX_CLIENTEDGE,								// roz���en� styl
			WC_LISTVIEW,									// jm�no t��dy okna
			NULL,											// titulek okna
			LVS_NOCOLUMNHEADER | LVS_REPORT | LVS_SHAREIMAGELISTS | LVS_SINGLESEL |
			WS_VISIBLE | WS_CHILD,							// styl okna
			10,												// X
			40,												// Y
			300,											// ���ka
			220,											// v��ka
			MainFrame,										// rodi�
			(HMENU)IDN_CHECKWND,								// menu
			hInstance,										// instance
			NULL);											// parametry

// p�ipojen� senzamu ikon pro v�b�r
		::SendMessage(CheckWnd, LVM_SETIMAGELIST, LVSIL_SMALL, (LPARAM)CheckList);

// nastaven� font� v okn�
		::SendMessage(CheckWnd, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);

// vytvo�en� sloupce p�ep�na��
		LVCOLUMN lvc;
		lvc.mask = LVCF_FMT | LVCF_WIDTH;
		lvc.fmt = LVCFMT_LEFT;
		lvc.cx = 208;
		::SendMessage(CheckWnd, LVM_INSERTCOLUMN, 0, (LPARAM)&lvc);

// vytvo�en� sloupce velikost� ��st�
		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM;
		lvc.fmt = LVCFMT_RIGHT;
		lvc.cx = 64;
		lvc.iSubItem = 1;
		::SendMessage(CheckWnd, LVM_INSERTCOLUMN, 1, (LPARAM)&lvc);

// napln�n� okna p�ep�na�i
		CText txt;
		for (int i = 0; i < GROUPSNUM; i++)
		{
			txt.Int((Head.Groups[i].Size + 1023)/1024);
			txt += _T(" MB");			
			InsertCheck(i, Groups[i].check, Groups[i].instal, Groups[i].name, txt);
		}

// p�ednastaven� v�b�ru na prvn� polo�ku
		LVITEM lvi;
		lvi.iSubItem = 0;
		lvi.iItem = 0;
		lvi.state = LVIS_SELECTED;
		lvi.mask = LVIF_STATE;
		lvi.stateMask = LVIS_SELECTED;
		::SendMessage(CheckWnd, LVM_SETITEMSTATE, 0, (LPARAM)&lvi);
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� nadpisu p�ep�na��

void CreateCheckNadp()
{
	if (CheckNadp == NULL)
	{
		if (FirstInst)
		{
			CheckNadp = ::CreateWindowEx(
				0,												// roz���en� styl
				_T("STATIC"),									// jm�no t��dy okna
				X(
					_T("Vyberte instalovan� ��sti a c�lovou slo�ku. Stiskem tla��tka 'Start' zah�j�te instalaci."), 
					_T("Select installed parts and destination folder. By pressing 'Start' you begin installation.")),
				SS_CENTER | WS_VISIBLE | WS_CHILD,				// styl okna
				0,												// X
				10,												// Y
				ClientWidth,									// ���ka
				18,												// v��ka
				MainFrame,										// rodi�
				(HMENU)IDN_CHECKNADP,							// menu
				hInstance,										// instance
				NULL);											// parametry
		}
		else
		{
			CheckNadp = ::CreateWindowEx(
				0,												// roz���en� styl
				_T("STATIC"),									// jm�no t��dy okna
				X(
					_T("Zm��te v�b�r nainstalovan�ch ��st�. Stiskem tla��tka 'Start' zah�j�te zm�nu instalace."),
					_T("Change selection of installed parts. By pressing 'Start' you begin reinstallation.")),
				SS_CENTER | WS_VISIBLE | WS_CHILD,				// styl okna
				0,												// X
				10,												// Y
				ClientWidth,									// ���ka
				18,												// v��ka
				MainFrame,										// rodi�
				(HMENU)IDN_CHECKNADP,							// menu
				hInstance,										// instance
				NULL);											// parametry
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� instala�n� cesty

void CreatePathWnd()
{
	if (PathWndNadp == NULL)
	{
		PathWndNadp = ::CreateWindowEx(
			0,												// roz���en� styl
			_T("STATIC"),									// jm�no t��dy okna
			X(_T("C�lov� slo�ka:"), _T("Destination folder:")),	// titulek okna
			SS_LEFT | WS_VISIBLE | WS_CHILD,				// styl okna
			10,												// X
			ClientHeight - 112,								// Y
			200,											// ���ka
			20,												// v��ka
			MainFrame,										// rodi�
			(HMENU)IDN_PATHWNDNADP,							// menu
			hInstance,										// instance
			NULL);											// parametry

		::SendMessage(PathWndNadp, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);
	}

	if (PathWnd == NULL)
	{
		PathWnd = ::CreateWindowEx(
			WS_EX_CLIENTEDGE,								// roz���en� styl
			_T("EDIT"),										// jm�no t��dy okna
			InstPath,										// titulek okna
			ES_AUTOHSCROLL | ES_LEFT | WS_GROUP | WS_TABSTOP | WS_VISIBLE | WS_CHILD,			// styl okna
			20,												// X
			ClientHeight - 93,								// Y
			ClientWidth - 150,								// ���ka
			20,												// v��ka
			MainFrame,										// rodi�
			(HMENU)IDN_PATHWND,								// menu
			hInstance,										// instance
			NULL);											// parametry

		::SendMessage(PathWnd, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� tla��tek volby jazyku

HWND CreateFlag(int id, int y, LPCTSTR text)
{
	HWND wnd = ::CreateWindowEx(
			0,												// roz���en� styl
			_T("BUTTON"),									// jm�no t��dy okna
			text,											// titulek okna
			BS_PUSHBUTTON | BS_TEXT | BS_NOTIFY | WS_GROUP | WS_TABSTOP | WS_VISIBLE | WS_CHILD,// styl okna
			270,											// X
			y,												// Y
			150,											// ���ka
			44,												// v��ka
			MainFrame,										// rodi�
			(HMENU)id,										// menu
			hInstance,										// instance
			NULL);											// parametry
	::SendMessage(wnd, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);

	return wnd;
}


void CreateFlags()
{
	FlagCZWnd = CreateFlag(IDN_FLAGWNDCZ, 104, "�esky (Czech)");
	FlagENGWnd = CreateFlag(IDN_FLAGWNDENG, 204, "English");

	HWND foc = FlagENGWnd;

	switch (Jazyk)
	{
	case JAZYKCZ:
		foc = FlagCZWnd;
		break;
	}

	::SetFocus(foc);
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� tla��tka

HWND CreateButton(int id, int x, LPCTSTR text)
{
	HWND wnd = ::CreateWindowEx(
			0,												// roz���en� styl
			_T("BUTTON"),									// jm�no t��dy okna
			text,											// titulek okna
			BS_PUSHBUTTON | BS_TEXT | BS_NOTIFY | WS_GROUP | WS_TABSTOP | WS_VISIBLE | WS_CHILD,// styl okna
			x,												// X
			ClientHeight - 40,								// Y
			100,											// ���ka
			26,												// v��ka
			MainFrame,										// rodi�
			(HMENU)id,										// menu
			hInstance,										// instance
			NULL);											// parametry
	::SendMessage(wnd, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);

	return wnd;
}

HWND CreateDefButton(int id, int x, LPCTSTR text)
{
	HWND wnd = ::CreateWindowEx(
			0,												// roz���en� styl
			_T("BUTTON"),									// jm�no t��dy okna
			text,											// titulek okna
			BS_DEFPUSHBUTTON | BS_TEXT | BS_NOTIFY | WS_GROUP | WS_TABSTOP | WS_VISIBLE | WS_CHILD,// styl okna
			x,												// X
			ClientHeight - 40,								// Y
			100,												// ���ka
			26,												// v��ka
			MainFrame,										// rodi�
			(HMENU)id,										// menu
			hInstance,										// instance
			NULL);											// parametry
	::SendMessage(wnd, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);

	return wnd;
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� tla��tka Zp�t

void CreateBack()
{
	if (ButtonBack == NULL)
	{
		ButtonBack = CreateButton(IDN_BUTTONBACK, ClientWidth - 20 - 100 - 40 - 100 - 100, X(_T("< Zp�t"), _T("< Back")));
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� tla��tka Dal��

void CreateNext()
{
	if (ButtonNext == NULL)
	{
		switch (AktPage)
		{
		case PAGEISDINST:
		case PAGESELECT:
			ButtonNext = CreateDefButton(IDN_BUTTONNEXT, ClientWidth - 20 - 100 - 40 - 100, X(_T("Start"), _T("Start")));
			break;

		default:
			ButtonNext = CreateDefButton(IDN_BUTTONNEXT, ClientWidth - 20 - 100 - 40 - 100, X(_T("Dal�� >"), _T("Next >")));
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� tla��tka Konec

void CreateExit()
{
	if (ButtonExit == NULL)
	{
		if ((AktPage == PAGEOK) || (AktPage == PAGEDINSTOK) || (AktPage == PAGELANG))
		{
			ButtonExit = CreateButton(IDN_BUTTONEXIT, (ClientWidth - 100)/2, X(_T("Konec"), _T("Quit")));
		}
		else
		{
			ButtonExit = CreateButton(IDN_BUTTONEXIT, ClientWidth - 20 - 100, X(_T("Konec"), _T("Quit")));
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� tla��tka Storno

void CreateCanc()
{
	if (ButtonCanc == NULL)
	{
		ButtonCanc = CreateButton(IDN_BUTTONCANC, (ClientWidth - 100)/2, X(_T("Storno"), _T("Cancel")));
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� tla��tka Proch�zet

void CreateList()
{
	if (ButtonList == NULL)
	{
		ButtonList = ::CreateWindowEx(
				0,												// roz���en� styl
				_T("BUTTON"),									// jm�no t��dy okna
				X(_T("Proch�zet..."), _T("Browse...")),	// titulek okna
				BS_PUSHBUTTON | BS_TEXT | BS_NOTIFY | WS_GROUP | WS_TABSTOP | WS_VISIBLE | WS_CHILD,// styl okna
				ClientWidth - 20 - 100,							// X
				ClientHeight - 100,								// Y
				100,											// ���ka
				26,												// v��ka
				MainFrame,										// rodi�
				(HMENU)IDN_BUTTONLIST,							// menu
				hInstance,										// instance
				NULL);											// parametry
		::SendMessage(ButtonList, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);

		if (!FirstInst)
		{
			::EnableWindow(ButtonList, FALSE);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� indik�toru instalace

void CreateIndWnd()
{
	if (IndWnd == NULL)
	{
		IndWnd = ::CreateWindowEx(
				WS_EX_CLIENTEDGE,								// roz���en� styl
				PROGRESS_CLASS,									// jm�no t��dy okna
				NULL,											// titulek okna
				WS_CHILD | WS_VISIBLE,							// styl okna
				20,												// X
				200,											// Y
				ClientWidth - 40,								// ���ka
				20,												// v��ka
				MainFrame,										// rodi�
				(HMENU)IDN_INDWND,								// menu
				hInstance,										// instance
				NULL);											// parametry

		if (IndWnd)
		{
			::SendMessage(IndWnd, PBM_SETRANGE, 0, MAKELPARAM(0, PROGMAX));
			::SendMessage(IndWnd, PBM_SETPOS, 0, 0);
		}
	}

	if (IndWndNadp == NULL)
	{
		if (AktPage == PAGEDINSTAL)
		{
			IndWndNadp = ::CreateWindowEx(
				0,												// roz���en� styl
				_T("STATIC"),									// jm�no t��dy okna
				X(_T("Prob�h� odinstalov�n� aplikace Petr ..."), _T("Proceeding uninstallation of the Peter application ...")),	// titulek okna
				SS_CENTER | WS_VISIBLE | WS_CHILD,				// styl okna
				30,												// X
				100,											// Y
				ClientWidth - 60,								// ���ka
				20,												// v��ka
				MainFrame,										// rodi�
				(HMENU)IDN_INDWNDNADP,							// menu
				hInstance,										// instance
				NULL);											// parametry
		}
		else
		{
			IndWndNadp = ::CreateWindowEx(
				0,												// roz���en� styl
				_T("STATIC"),									// jm�no t��dy okna
				X(_T("Prob�h� instalace aplikace Petr ..."), _T("Proceeding installation of the Peter application ...")),		// titulek okna
				SS_CENTER | WS_VISIBLE | WS_CHILD,				// styl okna
				30,												// X
				100,											// Y
				ClientWidth - 60,								// ���ka
				20,												// v��ka
				MainFrame,										// rodi�
				(HMENU)IDN_INDWNDNADP,							// menu
				hInstance,										// instance
				NULL);											// parametry
		}
	}

	if (IndWndFile == NULL)
	{
		IndWndFile = ::CreateWindowEx(
			0,												// roz���en� styl
			_T("STATIC"),									// jm�no t��dy okna
			NULL,											// titulek okna
			SS_LEFT | WS_VISIBLE | WS_CHILD,				// styl okna
			20,												// X
			170,											// Y
			ClientWidth - 30,								// ���ka
			20,												// v��ka
			MainFrame,										// rodi�
			(HMENU)IDN_INDWNDFILE,							// menu
			hInstance,										// instance
			NULL);											// parametry
		::SendMessage(IndWndFile, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);
	}

	if (IndWnd000 == NULL)
	{
		IndWnd000 = ::CreateWindowEx(
			0,												// roz���en� styl
			_T("STATIC"),									// jm�no t��dy okna
			_T("0%"),										// titulek okna
			SS_CENTER | WS_VISIBLE | WS_CHILD,				// styl okna
			5,												// X
			225,											// Y
			30,												// ���ka
			20,												// v��ka
			MainFrame,										// rodi�
			(HMENU)IDN_INDWND000,							// menu
			hInstance,										// instance
			NULL);											// parametry
		::SendMessage(IndWnd000, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);
	}

	if (IndWnd025 == NULL)
	{
		IndWnd025 = ::CreateWindowEx(
			0,												// roz���en� styl
			_T("STATIC"),									// jm�no t��dy okna
			_T("25%"),										// titulek okna
			SS_CENTER | WS_VISIBLE | WS_CHILD,				// styl okna
			5 + 1*(ClientWidth - 40)/4,						// X
			225,											// Y
			30,												// ���ka
			20,												// v��ka
			MainFrame,										// rodi�
			(HMENU)IDN_INDWND025,							// menu
			hInstance,										// instance
			NULL);											// parametry
		::SendMessage(IndWnd025, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);
	}

	if (IndWnd050 == NULL)
	{
		IndWnd050 = ::CreateWindowEx(
			0,												// roz���en� styl
			_T("STATIC"),									// jm�no t��dy okna
			_T("50%"),										// titulek okna
			SS_CENTER | WS_VISIBLE | WS_CHILD,				// styl okna
			5 + 2*(ClientWidth - 40)/4,						// X
			225,											// Y
			30,												// ���ka
			20,												// v��ka
			MainFrame,										// rodi�
			(HMENU)IDN_INDWND050,							// menu
			hInstance,										// instance
			NULL);											// parametry
		::SendMessage(IndWnd050, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);
	}

	if (IndWnd075 == NULL)
	{
		IndWnd075 = ::CreateWindowEx(
			0,												// roz���en� styl
			_T("STATIC"),									// jm�no t��dy okna
			_T("75%"),										// titulek okna
			SS_CENTER | WS_VISIBLE | WS_CHILD,				// styl okna
			5 + 3*(ClientWidth - 40)/4,						// X
			225,											// Y
			30,												// ���ka
			20,												// v��ka
			MainFrame,										// rodi�
			(HMENU)IDN_INDWND075,							// menu
			hInstance,										// instance
			NULL);											// parametry
		::SendMessage(IndWnd075, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);
	}

	if (IndWnd100 == NULL)
	{
		IndWnd100 = ::CreateWindowEx(
			0,												// roz���en� styl
			_T("STATIC"),									// jm�no t��dy okna
			_T("100%"),										// titulek okna
			SS_CENTER | WS_VISIBLE | WS_CHILD,				// styl okna
			-2 + (ClientWidth - 40),						// X
			225,											// Y
			30,												// ���ka
			20,												// v��ka
			MainFrame,										// rodi�
			(HMENU)IDN_INDWND100,							// menu
			hInstance,										// instance
			NULL);											// parametry
		::SendMessage(IndWnd100, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� informac� o disku

void CreateDiskFree()
{
	if (DiskReqNadp == NULL)
	{
		DiskReqNadp = ::CreateWindowEx(
			0,												// roz���en� styl
			_T("STATIC"),									// jm�no t��dy okna
			X(_T("Po�adovan� voln� m�sto:"), _T("Required free space:")),					// titulek okna
			SS_LEFT | WS_VISIBLE | WS_CHILD,				// styl okna
			330,											// X
			210,											// Y
			180,											// ���ka
			18,												// v��ka
			MainFrame,										// rodi�
			(HMENU)IDN_DISKREQNADP,							// menu
			hInstance,										// instance
			NULL);											// parametry
		::SendMessage(DiskReqNadp, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);
	}

	if (DiskReqNum == NULL)
	{
		DiskReq = 0;

		for (int i = 0; i < GROUPSNUM; i++)
		{
			if (Groups[i].check)
			{
				DiskReq += (Head.Groups[i].Size + 1023)/1024;
			}

			if (Groups[i].instal)
			{
				DiskReq -= (Head.Groups[i].Size + 1023)/1024;
			}
		}
		
		CText txt(_T('0'));
		if (DiskReq > 0)
		{
			txt.Int(DiskReq);
		}
		txt += _T(" MB");

		DiskReqNum = ::CreateWindowEx(
			0,												// roz���en� styl
			_T("STATIC"),									// jm�no t��dy okna
			txt,											// titulek okna
			SS_RIGHT | WS_VISIBLE | WS_CHILD,				// styl okna
			510,											// X
			210,											// Y
			70,												// ���ka
			18,												// v��ka
			MainFrame,										// rodi�
			(HMENU)IDN_DISKREQNADP,							// menu
			hInstance,										// instance
			NULL);											// parametry
		::SendMessage(DiskReqNum, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);
	}

	if (DiskFreeNadp == NULL)
	{
		DiskFreeNadp = ::CreateWindowEx(
			0,												// roz���en� styl
			_T("STATIC"),									// jm�no t��dy okna
			X(_T("Voln� m�sto na c�lov�m disku:"), _T("Free space on destination disk:")),	// titulek okna
			SS_LEFT | WS_VISIBLE | WS_CHILD,				// styl okna
			330,											// X
			235,											// Y
			180,											// ���ka
			18,												// v��ka
			MainFrame,										// rodi�
			(HMENU)IDN_DISKFREENADP,							// menu
			hInstance,										// instance
			NULL);											// parametry
		::SendMessage(DiskFreeNadp, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);
	}

	if (DiskFreeNum == NULL)
	{
		CText txt;
		txt.Int(DiskFre);
		txt += _T(" MB");

		DiskFreeNum = ::CreateWindowEx(
			0,												// roz���en� styl
			_T("STATIC"),									// jm�no t��dy okna
			txt,											// titulek okna
			SS_RIGHT | WS_VISIBLE | WS_CHILD,				// styl okna
			510,											// X
			235,											// Y
			70,												// ���ka
			18,												// v��ka
			MainFrame,										// rodi�
			(HMENU)IDN_DISKFREENUM,							// menu
			hInstance,										// instance
			NULL);											// parametry
		::SendMessage(DiskFreeNum, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);
	}

	if (ButtonNext)
	{
// blokov�n� tla��tka Next
		BOOL enable = FALSE;
		for (int i = 0; i < GROUPSNUM; i++)
		{
			if (Groups[i].check != Groups[i].instal)
			{
				enable = TRUE;
			}
		}
		::EnableWindow(ButtonNext, enable);
	}
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� nadpisu smlouvy

void CreateSmlouvaNadp()
{
	if (SmlouvaNadp == NULL)
	{
		SmlouvaNadp = ::CreateWindowEx(
			0,												// roz���en� styl
			_T("STATIC"),									// jm�no t��dy okna
			X(
				_T("P�e�t�te si, pros�m, Licen�n� smlouvu a stvr�te souhlas s n� stiskem 'Souhlas�m'."),
				_T("Please read License Agreement and confirm agreement with it by pressing 'I Agree'.")),
			SS_CENTER | WS_VISIBLE | WS_CHILD,				// styl okna
			1,												// X
			10,												// Y
			ClientWidth - 2,								// ���ka
			18,												// v��ka
			MainFrame,										// rodi�
			(HMENU)IDN_SMLOUVANADP,							// menu
			hInstance,										// instance
			NULL);											// parametry
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� okna s licen�n� smlouvou (nesm� b�t UNICODE !!!!)

CText smlouvatxt;
int smlouvaoff;

DWORD CALLBACK CreateSmlouvaCall(DWORD cookie, LPBYTE buf, LONG cb, LONG* pcb)
{
	if (cb < 0) cb = 0;
	if ((smlouvatxt.Length() - smlouvaoff) < cb)
	{
		cb = smlouvatxt.Length() - smlouvaoff;
	}

#ifdef _UNICODE
	ASSERTERROR;
#endif

	MemCopy(buf, smlouvatxt.DataData() + smlouvaoff, cb);
	smlouvaoff += cb;
	*pcb = cb;
	return 0;
}

void CreateSmlouva()
{
	if (SmlouvaWnd == NULL)
	{
		SmlouvaWnd = ::CreateWindowEx(
				WS_EX_CLIENTEDGE,								// roz���en� styl
				_T("RichEdit"),									// jm�no t��dy okna
				NULL,											// titulek okna
				ES_AUTOVSCROLL | ES_SAVESEL | ES_SUNKEN | WS_VSCROLL |
					ES_READONLY | ES_MULTILINE | WS_CHILD,// styl okna
				10,												// X
				40,												// Y
				ClientWidth - 20,								// ���ka
				ClientHeight - 40 - 56,							// v��ka
				MainFrame,										// rodi�
				(HMENU)IDN_SMLOUVAWND,							// menu
				hInstance,										// instance
				NULL);											// parametry

// na�ten� textu
		CResource res;
		res.Open(X(IDR_LIC_CZ, IDR_LIC_EN), _T("RTF"));
		smlouvatxt.Set((char*)res.Adr(), res.Size());
		smlouvaoff = 0;

// nastaven� textu
		EDITSTREAM str;
		str.dwCookie = 0;
		str.dwError = 0;
		str.pfnCallback = CreateSmlouvaCall;	
		::SendMessage(SmlouvaWnd, EM_STREAMIN, SF_RTF, (LPARAM)&str);

// zobrazen� okna
		::ShowWindow(SmlouvaWnd, SW_SHOW);
	}
}


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

/////////////////////////////////////////////////////////////////////////////
// v�b�r c�lov�ho adres��e

UINT CALLBACK OFNHookProc(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	HWND dlg = ::GetParent(hdlg);

	switch (uiMsg)
	{
	case WM_INITDIALOG:
		{
			CText txt;

			txt = X(_T("&Zobrazit:"), _T("&Display:"));
			txt.SetDialogText(dlg, stc2);

			txt = X(_T("&N�zev slo�ky:"), _T("Folder &Name:"));
			txt.SetDialogText(dlg, stc3);

			txt = X(_T("&Cesta:"), _T("&Path:"));
			txt.SetDialogText(dlg, stc4);

			CenterWindow(dlg, MainFrame);
		}
		break;
	}

	return FALSE;
}
 

void SelectDest()
{
// p��prava struktury k zad�n� jm�na souboru
	OPENFILENAME ofn;
	MemFill(&ofn, sizeof(ofn), 0);
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = MainFrame;
	ofn.lpstrFilter = X(_T("Slo�ky soubor�\0 . \0\0"), _T("File folders\0 . \0\0"));
	ofn.Flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_SHAREAWARE | OFN_ENABLEHOOK | OFN_ENABLESIZING |
			OFN_NOVALIDATE | OFN_NOTESTFILECREATE | OFN_LONGNAMES | OFN_EXPLORER;
	ofn.lpstrTitle = X(_T("C�lov� slo�ka k instalaci aplikace Petr"), _T("Destination Folder to Install the Peter Application"));
	ofn.lpfnHook = OFNHookProc;

// aktu�ln� adres��
	CText path;
	path = InstPath;
	path.Delete(path.RevFind(_T('\\')));
	ofn.lpstrInitialDir = path;

// p�ednastaven� jm�no souboru
	CText name;
	name = InstPath.Right(InstPath.Length() - (InstPath.RevFind(_T('\\')) + 1));
	int n = name.Length() + 1;
	if (n < 1024) n = 1024;

// buffer k zad�n� jm�na souboru
	TCHAR* file = (TCHAR*)MemGet(n*sizeof(TCHAR));
	if (file == NULL) return;
	MemFill(file, n*sizeof(TCHAR), 0);
	name.Write(file);
	ofn.lpstrFile = file;
	ofn.nMaxFile = n;

// zad�n� jm�na souboru
	if (!::GetSaveFileName(&ofn))
	{
		MemFree(file);
		SetButton(IDN_BUTTONLIST);
		return;
	}

// korekce jm�na souboru
	name = file;
	MemFree(file);
	name.PathName(0);
	name.TrimLeft();
	name.TrimRight();

	if ((name.Length() < 2) || (name[1] != _T(':')))
	{
		name = _T("c:") + name;
	}

	if ((name.Length() < 3) || (name[2] != _T('\\')))
	{
		name = name.Left(2) + _T('\\') + name.Right(name.Length() - 2);
	}

	name.PathName(0);

	if (name.LastChar() == _T('\\'))
	{
		name += _T("Peter");
	}

// ulo�en� jm�na souboru
	InstPath = name;
	name.SetWindowText(PathWnd);

// aktualizace voln�ho m�sta na c�lov�m disku
	GetDiskSpace();
//	name.Int(DiskFre);
//	name += _T(" MB");
//	name.SetWindowText(DiskFreeNum);

// aktualizace zobrazen� okna
	::InvalidateRect(MainFrame, NULL, TRUE);
	::UpdateWindow(MainFrame);

	SetButton(IDN_BUTTONLIST);
}


/////////////////////////////////////////////////////////////////////////////
// vykreslen� vlajek

void RePaintFlag()
{
// otev�en� DC okna
	HDC dc = ::GetDC(MainFrame);

// nastaven� vlastn�ch palet
	HPALETTE OldPal = ::SelectPalette(dc, StdPalette, FALSE);
	::RealizePalette(dc);

// vykreslen� �esk� vlajky
	StdBitmapInfo->bmiHeader.biWidth = FlagCZ.Width();
	StdBitmapInfo->bmiHeader.biHeight = FlagCZ.Height();

	::StretchDIBits(dc,
		190, 105, FlagCZ.Width(), FlagCZ.Height(),
		0, 0, FlagCZ.Width(), FlagCZ.Height(),
		FlagCZ.DataData(),
		StdBitmapInfo,
		DIB_RGB_COLORS, SRCCOPY);

// vykreslen� anglick� vlajky
	StdBitmapInfo->bmiHeader.biWidth = FlagENG.Width();
	StdBitmapInfo->bmiHeader.biHeight = FlagENG.Height();

	::StretchDIBits(dc,
		190, 210, FlagENG.Width(), FlagENG.Height(),
		0, 0, FlagENG.Width(), FlagENG.Height(),
		FlagENG.DataData(),
		StdBitmapInfo,
		DIB_RGB_COLORS, SRCCOPY);

// n�vrat p�vodn�ch palet
	::SelectPalette(dc, OldPal, FALSE);

// uzav�en� DC okna
	::ReleaseDC(MainFrame, dc);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� str�nky instal�toru

void SetPage(int page)
{

// nov� aktivn� str�nka
	AktPage = page;

// zru�en� v�ech prvk�
	if (ButtonBack)
	{
		::DestroyWindow(ButtonBack);
		ButtonBack = NULL;
	}

	if (ButtonNext)
	{
		::DestroyWindow(ButtonNext);
		ButtonNext = NULL;
	}

	if (ButtonExit)
	{
		::DestroyWindow(ButtonExit);
		ButtonExit = NULL;
	}

	if (ButtonList)
	{
		::DestroyWindow(ButtonList);
		ButtonList = NULL;
	}

	if (ButtonCanc)
	{
		::DestroyWindow(ButtonCanc);
		ButtonCanc = NULL;
	}

	if (LineWnd)
	{
		::DestroyWindow(LineWnd);
		LineWnd = NULL;
	}

	if (CheckWnd)
	{
		::DestroyWindow(CheckWnd);
		CheckWnd = NULL;
	}

	if (CheckNadp)
	{
		::DestroyWindow(CheckNadp);
		CheckNadp = NULL;
	}

	if (SmlouvaWnd)
	{
		::DestroyWindow(SmlouvaWnd);
		SmlouvaWnd = NULL;
	}

	if (SmlouvaNadp)
	{
		::DestroyWindow(SmlouvaNadp);
		SmlouvaNadp = NULL;
	}

	if (PathWnd)
	{
		::DestroyWindow(PathWnd);
		PathWnd = NULL;
	}

	if (PathWndNadp)
	{
		::DestroyWindow(PathWndNadp);
		PathWndNadp = NULL;
	}

	if (UserRam)
	{
		::DestroyWindow(UserRam);
		UserRam = NULL;
	}

	if (LicTextNadp)
	{
		::DestroyWindow(LicTextNadp);
		LicTextNadp = NULL;
	}

	if (LicTextNum)
	{
		::DestroyWindow(LicTextNum);
		LicTextNum = NULL;
	}

	if (LicTextName)
	{
		::DestroyWindow(LicTextName);
		LicTextName = NULL;
	}

	if (LicTextAdr)
	{
		::DestroyWindow(LicTextAdr);
		LicTextAdr = NULL;
	}

	if (LicTextMest)
	{
		::DestroyWindow(LicTextMest);
		LicTextMest = NULL;
	}

	if (LicTextIco)
	{
		::DestroyWindow(LicTextIco);
		LicTextIco = NULL;
	}

	if (DiskReqNadp)
	{
		::DestroyWindow(DiskReqNadp);
		DiskReqNadp = NULL;
	}

	if (DiskReqNum)
	{
		::DestroyWindow(DiskReqNum);
		DiskReqNum = NULL;
	}

	if (DiskFreeNadp)
	{
		::DestroyWindow(DiskFreeNadp);
		DiskFreeNadp = NULL;
	}

	if (DiskFreeNum)
	{
		::DestroyWindow(DiskFreeNum);
		DiskFreeNum = NULL;
	}

	if (IndWnd)
	{
		::DestroyWindow(IndWnd);
		IndWnd = NULL;
	}

	if (IndWndNadp)
	{
		::DestroyWindow(IndWndNadp);
		IndWndNadp = NULL;
	}

	if (IndWndFile)
	{
		::DestroyWindow(IndWndFile);
		IndWndFile = NULL;
	}

	if (IndWnd000)
	{
		::DestroyWindow(IndWnd000);
		IndWnd000 = NULL;
	}

	if (IndWnd025)
	{
		::DestroyWindow(IndWnd025);
		IndWnd025 = NULL;
	}

	if (IndWnd050)
	{
		::DestroyWindow(IndWnd050);
		IndWnd050 = NULL;
	}

	if (IndWnd075)
	{
		::DestroyWindow(IndWnd075);
		IndWnd075 = NULL;
	}

	if (IndWnd100)
	{
		::DestroyWindow(IndWnd100);
		IndWnd100 = NULL;
	}

	if (FlagCZWnd)
	{
		::DestroyWindow(FlagCZWnd);
		FlagCZWnd = NULL;
	}

	if (FlagENGWnd)
	{
		::DestroyWindow(FlagENGWnd);
		FlagENGWnd = NULL;
	}

// vytvo�en� nov�ho okna
	switch (page)
	{
// v�b�r jazyku
	case PAGELANG:
		RePaint();						// vymaz�n� podkladu
		CreateExit();					// vytvo�en� tla��tka Konec
		CreateFlags();					// vytvo�en� tla��tek vlajek
		CreateLineWnd();				// vytvo�en� ��ry nad tla��tky
		::SetCursor(CurArrow);			// kurzor - �ipka
		RePaintFlag();					// vykreslen� vlajek
		break;

// hlavn� okno
	case PAGEMAIN:
		if (FirstInst)
		{
			InstPath = GetProgramFiles();
			if (InstPath.IsEmpty()) InstPath = _T("C:\\Program Files\\");
			if (InstPath.LastChar() != _T('\\')) InstPath += _T('\\');

			InstPath += _T("Peter");

			for (int i = 0; i < GROUPSNUM; i++)
			{
				Groups[i].check = Groups[i].initcheck;
			}
		}
		else
		{
			InstPath = GetReg(ProductKey, _T("Path"));
			for (int i = 0; i < GROUPSNUM; i++)
			{
				Groups[i].check = Groups[i].instal;
			}
		}
		
		::SetCursor(CurPeter);			// kurzor - Petr
		RePaint();						// vykreslen� okna
		break;

// okno v�b�ru slo�ek	
	case PAGESELECT:
		RePaint();						// vymaz�n� podkladu
		GetDiskSpace();					// na�ten� informac� o c�lov�m disku
		CreateBack();					// vytvo�en� tla��tka Zp�t
		CreateNext();					// vytvo�en� tla��tka Dal��
		CreateExit();					// vytvo�en� tla��tka Konec
		CreateList();					// vytvo�en� tla��tka Proch�zet
		CreateLineWnd();				// vytvo�en� ��ry nad tla��tky
		CreateCheckWnd();				// vytvo�en� pole p�ep�na��
		CreateCheckNadp();				// vytvo�en� nadpisu p�ep�na��
		CreatePathWnd();				// vytvo�en� textu cesty
		CreateUserRam();				// vytvo�en� u�ivatelsk�ho r�mu
		CreateDiskFree();				// zobrazen� informac� o disku

		::SetFocus(ButtonNext);			// fokus na tla��tko Next
		::SetCursor(CurArrow);			// kurzor - �ipka
		break;	

// okno instalace
	case PAGEINSTAL:
		RePaint();						// vymaz�n� podkladu
		CreateCanc();					// vytvo�en� tla��tka Storno
		CreateIndWnd();					// vytvo�en� indik�toru
		CreateLineWnd();				// vytvo�en� ��ry nad tla��tky

		::SetFocus(ButtonCanc);			// fokus na tla��tko Storno
		::SetCursor(CurArrow);			// kurzor - �ipka

		Install();						// proveden� instalace (p��p. p�eru�en�)
		break;	

// okno odinstalace
	case PAGEDINSTAL:
		RePaint();						// vymaz�n� podkladu
		CreateCanc();					// vytvo�en� tla��tka Storno (to jen aby tam n�co bylo)
		::EnableWindow(ButtonCanc, FALSE);	// z�kaz tla��tka Storno
		CreateIndWnd();					// vytvo�en� indik�toru
		CreateLineWnd();				// vytvo�en� ��ry nad tla��tky

		::SetFocus(ButtonCanc);			// fokus na tla��tko Storno
		::SetCursor(CurArrow);			// kurzor - �ipka

		DInstall();						// proveden� odinstalace
		break;	

// okno potvrzen� odinstalov�n�
	case PAGEISDINST:
		RePaint();						// vymaz�n� podkladu
		if (!Uninst)
		{
			CreateBack();					// vytvo�en� tla��tka Zp�t
		}
		CreateNext();					// vytvo�en� tla��tka Dal��
		CreateExit();					// vytvo�en� tla��tka Konec
		CreateLineWnd();				// vytvo�en� ��ry nad tla��tky

		::SetFocus(ButtonNext);			// fokus na tla��tko Next
		::SetCursor(CurArrow);			// kurzor - �ipka
		RePaintDInst();					// zobrazen� obr�zku
		break;	

// okno OK
	case PAGEOK:
		RePaint();						// vymaz�n� podkladu
		CreateExit();					// vytvo�en� tla��tka Storno
		CreateLineWnd();				// vytvo�en� ��ry nad tla��tky

		::SetFocus(ButtonExit);			// fokus na tla��tko Next
		::SetCursor(CurArrow);			// kurzor - �ipka
		RePaintOK();					// zobrazen� obr�zku OK
		break;	

// okno odinstalov�n� OK
	case PAGEDINSTOK:
		RePaint();						// vymaz�n� podkladu
		CreateExit();					// vytvo�en� tla��tka Storno
		CreateLineWnd();				// vytvo�en� ��ry nad tla��tky

		::SetFocus(ButtonExit);			// fokus na tla��tko Next
		::SetCursor(CurArrow);			// kurzor - �ipka
		RePaintDInstOK();				// zobrazen� obr�zku OK
		break;	
	}
}


/////////////////////////////////////////////////////////////////////////////
// p�ekreslen� okna

void RePaint()
{
// lok�ln� prom�nn�
	int srcinc;
	BYTE* src;
	BYTE* dst;
	int i,j;

// inicializa�n� vymaz�n� bufferu
	MemFill(ClientBuf, ClientWidth * ClientHeight, (char)(BYTE)BackCol);

// vykreslen� bude jen pro hlavn� okno, v ostatn�ch bude vymaz�n�
	if (AktPage == PAGEMAIN)
	{

// p�enesen� st�nu (jen je-li kurzor my�i v okn�)
		if ((OffX != MOUSEINV) && (OffY != MOUSEINV))
		{
			srcinc = Shadow.Width();
			src = Shadow.DataData();
			dst = ClientBuf;
			int width = ClientWidth;
			int height = ClientHeight;

			if (OffX < 0)
			{
				width += OffX;
				dst -= OffX;
			}
			else
			{
				src += OffX;
				if (OffX + width > srcinc)
				{
					width = srcinc - OffX;
				}
			}

			if (OffY < 0)
			{
				height += OffY;
				dst -= OffY*ClientWidth;
			}
			else
			{
				src += OffY*srcinc;
				if (OffY + height > Shadow.Height())
				{
					height = Shadow.Height() - OffY;
				}
			}

			if (width > 0)
			{
				for (i = height; i > 0; i--)
				{
					MemCopy(dst, src, width);
					dst += ClientWidth;
					src += srcinc;
				}
			}
		}

// adresa obr�zku k vykreslen�
		switch (KeySelect)
		{
		case 0:
			src = Picture1.DataData();
			KeyDisp = 1;
			break;

		case 1:
			src = Picture2.DataData();
			KeyDisp = 2;
			break;

		case 2:
			src = Picture3.DataData();
			KeyDisp = 3;
			break;

		case 3:
			src = Picture4.DataData();
			KeyDisp = 4;
			break;

		default:
			if ((RelX < 0) || (RelX >= ClientWidth) || (RelY < MENUTOP) || (RelY >= MENUTOP + 4*MENUHEIGHT))
			{
				src = Picture0.DataData();
				KeyDisp = 0;
			}
			else
			{
				if (RelY < MENUTOP + MENUHEIGHT)
				{
					src = Picture1.DataData();
					KeyDisp = 1;
				}
				else
				{
					if (RelY < MENUTOP + 2*MENUHEIGHT)
					{
						src = Picture2.DataData();
						KeyDisp = 2;
					}
					else
					{
						if (RelY < MENUTOP + 3*MENUHEIGHT)
						{
							src = Picture3.DataData();
							KeyDisp = 3;
						}
						else
						{
							src = Picture4.DataData();
							KeyDisp = 4;
						}
					}
				}
			}
		}

// p�id�n� obr�zku
		dst = ClientBuf;
		for (i = ClientHeight; i > 0; i--)
		{
			for (j = ClientWidth; j > 0; j--)
			{
				if (*src != BackCol) *dst = *src;
				src++;
				dst++;
			}
		}
	}

// p�id�n� znaku
	if (AktPage == PAGEMAIN)
	{
		src = GemLogo.DataData() + GemtreeFaze*40;

		dst = ClientBuf + (ClientHeight - GemtreeY - 40)*ClientWidth + GemtreeX2;

		for (i = 40; i > 0; i--)
		{
			for (j = 40; j > 0; j--)
			{
				if (*src != BackCol) *dst = *src;
				src++;
				dst++;
			}
			src += 90*40 - 40;
			dst += ClientWidth - 40;
		}
	}

// otev�en� DC okna
	HDC dc = ::GetDC(MainFrame);

// nastaven� vlastn�ch palet
	HPALETTE OldPal = ::SelectPalette(dc, StdPalette, FALSE);
	::RealizePalette(dc);

// vykreslen� plochy
	StdBitmapInfo->bmiHeader.biWidth = ClientWidth;
	StdBitmapInfo->bmiHeader.biHeight = ClientHeight;

	::StretchDIBits(dc,
		0, 0, ClientWidth, ClientHeight,
		0, 0, ClientWidth, ClientHeight, 
		ClientBuf,
		StdBitmapInfo,
		DIB_RGB_COLORS, SRCCOPY);

// n�vrat p�vodn�ch palet
	::SelectPalette(dc, OldPal, FALSE);

// uzav�en� DC okna
	::ReleaseDC(MainFrame, dc);
}


/////////////////////////////////////////////////////////////////////////////
// p�ekreslen� znaku Gemtree

void RePaintGemtree()
{

// otev�en� DC okna
	HDC dc = ::GetDC(MainFrame);

// nastaven� vlastn�ch palet
	HPALETTE OldPal = ::SelectPalette(dc, StdPalette, FALSE);
	::RealizePalette(dc);

// vykreslen� plochy
	StdBitmapInfo->bmiHeader.biWidth = 40*90;
	StdBitmapInfo->bmiHeader.biHeight = 40;

	::StretchDIBits(dc,
		GemtreeX2, GemtreeY, 40, 40,
		GemtreeFaze*40, 0, 40, 40,
		GemLogo.DataData(),
		StdBitmapInfo,
		DIB_RGB_COLORS, SRCCOPY);

// n�vrat p�vodn�ch palet
	::SelectPalette(dc, OldPal, FALSE);

// uzav�en� DC okna
	::ReleaseDC(MainFrame, dc);
}


/////////////////////////////////////////////////////////////////////////////
// p�ekreslen� okna instalace OK

void RePaintOK()
{

// otev�en� DC okna
	HDC dc = ::GetDC(MainFrame);

// nastaven� vlastn�ch palet
	HPALETTE OldPal = ::SelectPalette(dc, StdPalette, FALSE);
	::RealizePalette(dc);

// p��prava obr�zku
	CPicture pic;
	pic = InstalOK;

// vykreslen� plochy
	StdBitmapInfo->bmiHeader.biWidth = pic.Width();
	StdBitmapInfo->bmiHeader.biHeight = pic.Height();

	::StretchDIBits(dc,
		(ClientWidth - pic.Width())/2, 15, pic.Width(), pic.Height(),
		0, 0, pic.Width(), pic.Height(),
		pic.DataData(),
		StdBitmapInfo,
		DIB_RGB_COLORS, SRCCOPY);

// n�vrat p�vodn�ch palet
	::SelectPalette(dc, OldPal, FALSE);

// uzav�en� DC okna
	::ReleaseDC(MainFrame, dc);
}

/////////////////////////////////////////////////////////////////////////////
// p�ekreslen� okna odinstalace OK

void RePaintDInstOK()
{

// otev�en� DC okna
	HDC dc = ::GetDC(MainFrame);

// nastaven� vlastn�ch palet
	HPALETTE OldPal = ::SelectPalette(dc, StdPalette, FALSE);
	::RealizePalette(dc);

// p��prava obr�zku
	CPicture pic;
	pic = DInstalOK;

// vykreslen� plochy
	StdBitmapInfo->bmiHeader.biWidth = pic.Width();
	StdBitmapInfo->bmiHeader.biHeight = pic.Height();

	::StretchDIBits(dc,
		(ClientWidth - pic.Width())/2, 15, pic.Width(), pic.Height(),
		0, 0, pic.Width(), pic.Height(),
		pic.DataData(),
		StdBitmapInfo,
		DIB_RGB_COLORS, SRCCOPY);

// n�vrat p�vodn�ch palet
	::SelectPalette(dc, OldPal, FALSE);

// uzav�en� DC okna
	::ReleaseDC(MainFrame, dc);
}

/////////////////////////////////////////////////////////////////////////////
// p�ekreslen� okna odinstalace

void RePaintDInst()
{

// otev�en� DC okna
	HDC dc = ::GetDC(MainFrame);

// nastaven� vlastn�ch palet
	HPALETTE OldPal = ::SelectPalette(dc, StdPalette, FALSE);
	::RealizePalette(dc);

// p��prava obr�zku
	CPicture pic;
	pic = DInstal;

// vykreslen� plochy
	StdBitmapInfo->bmiHeader.biWidth = pic.Width();
	StdBitmapInfo->bmiHeader.biHeight = pic.Height();

	::StretchDIBits(dc,
		(ClientWidth - pic.Width())/2, 15, pic.Width(), pic.Height(),
		0, 0, pic.Width(), pic.Height(),
		pic.DataData(),
		StdBitmapInfo,
		DIB_RGB_COLORS, SRCCOPY);

// n�vrat p�vodn�ch palet
	::SelectPalette(dc, OldPal, FALSE);

// uzav�en� DC okna
	::ReleaseDC(MainFrame, dc);
}

/////////////////////////////////////////////////////////////////////////////
// obsluha zpr�v okna

LRESULT CALLBACK WindowProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
	case WM_CLOSE:
		::PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			::BeginPaint(hWnd, &ps);
			::EndPaint(hWnd, &ps);
		}

		if (AktPage == PAGELANG)
		{
			RePaintFlag();
		}

		if (AktPage == PAGEMAIN)
		{
			RePaint();
		}

		if (AktPage == PAGEOK)
		{
			RePaintOK();
		}

		if (AktPage == PAGEISDINST)
		{
			RePaintDInst();
		}

		if (AktPage == PAGEDINSTOK)
		{
			RePaintDInstOK();
		}
		return 0;

	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
		case 0:
		case 1:
			switch (LOWORD(wParam))
			{
			case IDN_FLAGWNDCZ:
				Jazyk = JAZYKCZ;
				InitPic();
				SetPage(PAGEMAIN);
				return 0;

			case IDN_FLAGWNDENG:
				Jazyk = JAZYKENG;
				InitPic();
				SetPage(PAGEMAIN);
				return 0;

			case IDN_BUTTONEXIT:
				::ShowWindow(MainFrame, SW_HIDE);
				::SendMessage(MainFrame, WM_CLOSE, 0, 0);
				return 0;

			case IDN_BUTTONBACK:
				switch (AktPage)
				{
				case PAGESELECT:
					SetPage(PAGEMAIN);
					return 0;

				case PAGEISDINST:
					SetPage(PAGEMAIN);
					return 0;
				}
				return 0;

			case IDN_BUTTONNEXT:
				switch (AktPage)
				{
				case PAGESELECT:
					SetPage(PAGEINSTAL);
					return 0;

				case PAGEISDINST:
					SetPage(PAGEDINSTAL);
					return 0;
				}
				return 0;

			case IDN_BUTTONLIST:
				switch (AktPage)
				{
				case PAGESELECT:
					SelectDest();
					return 0;
				}
				return 0;

			case IDN_BUTTONCANC:
				Storno = TRUE;
				return 0;
			}
			break;

		case EN_KILLFOCUS:
			switch (LOWORD(wParam))
			{
			case IDN_PATHWND:
				if (PathWnd != NULL)
				{
					InstPath.GetWindowText(PathWnd);
					GetDiskSpace();
				}
				return 0;	
			}
			break;
		}
		break;

	case WM_NOTIFY:					// 0x04E
		{
			NMHDR* nmhdr = ((NMHDR*)lParam);
			switch (nmhdr->code)
			{
			case NM_DBLCLK:
			case NM_CLICK:
				if (nmhdr->hwndFrom == CheckWnd)
				{
					int inx = ::SendMessage(CheckWnd, LVM_GETNEXTITEM, (WPARAM)-1, LVNI_SELECTED);
					if ((DWORD)inx < (DWORD)GROUPSNUM)
					{
						Groups[inx].check = !Groups[inx].check;
						LVITEM lvi;
						lvi.mask = LVIF_IMAGE;
						lvi.iItem = inx;
						lvi.iSubItem = 0;
						lvi.iImage = Groups[inx].check ? 1 : 0;
						if (Groups[inx].instal) lvi.iImage += 2;
						::SendMessage(CheckWnd, LVM_SETITEM, 0, (LPARAM)&lvi);
						DispDiskReq();
					}
				}
				break;
			}
		}
		break;

	case WM_SETCURSOR:
		if (Waiting)
		{
			::SetCursor(CurWait);
		}
		else
		{
			if (AktPage == PAGEMAIN)
			{
				if ((KeySelect != -1) && (RelY >= MENUTOP))
				{
					KeySelect = -1;
					RePaint();
				}

				POINT cur;
				::GetCursorPos(&cur);
				::ScreenToClient(MainFrame, &cur);
				
				if ((cur.x < 0) || 
					(cur.x >= ClientWidth) ||
					(cur.y < 0) ||
					(cur.y >= ClientHeight))
				{
					break;
				}

				if ((cur.y < MENUTOP) ||
					(cur.y >= MENUTOP + 4*MENUHEIGHT))
				{
					::SetCursor(CurArrow);
				}
				else
				{
					::SetCursor(CurPeter);
				}
			}
			else
			{
				::SetCursor(CurArrow);
			}
		}
		return TRUE;

	case WM_TIMER:
		{
// sou�adnice kurzoru my�i
			POINT cur;
			::GetCursorPos(&cur);

// sou�adnice st�edu okna
			POINT win;
			win.x = 0;
			win.y = 0;
			::ClientToScreen(MainFrame, &win);

// relativn� pozice kurzoru my�i v okn�
			int oldx = RelX;
			int oldy = RelY;
			RelX = cur.x - win.x;
			RelY = cur.y - win.y;

// v�po�et posunu obr�zku
			int offx =  - RelX + RelX*Shadow.Width()/ClientWidth;
			int offy = Shadow.Height() - ClientHeight
						+ RelY - RelY*Shadow.Height()/ClientHeight;

// p�i zm�n� posunu p�ekreslen� okna
			GemtreeFaze++;
			if (GemtreeFaze >= GemtreeN) GemtreeFaze = 0;

			if ((oldx != RelX) || (oldy != RelY))
			{
				OffX = offx;
				OffY = offy;
				if (AktPage == PAGEMAIN)
				{
					RePaint();
				}
			}
			else
			{
				if (AktPage == PAGEMAIN)
				{
					if (RelY < -150)
					{
						RePaint();			// aby byl vid�t st�n
					}
					else
					{
						RePaintGemtree();
					}
				}
			}

			return 0;
		}

	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORDLG:
	case WM_CTLCOLORMSGBOX:
	case WM_CTLCOLORSCROLLBAR:
		{
			HDC dc = (HDC)wParam;
			::SetTextColor(dc, 0);
			::SetBkColor(dc, BackColCol);
			return (LRESULT) BackBrush;
		}
		break;
	}

	return ::DefWindowProc(hWnd, nMsg, wParam, lParam);
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� hlavn�ho okna aplikace


bool MainFrameCreate()
{
// registrace t��dy okna
	WNDCLASS wcl;
	wcl.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcl.lpfnWndProc = WindowProc;
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;
	wcl.hInstance = hInstance;
	wcl.hIcon = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDN_MAINFRAME));
	wcl.hCursor = NULL;
	wcl.hbrBackground =	BackBrush;
	wcl.lpszMenuName = NULL;
	wcl.lpszClassName = MainFrameClass;
	::RegisterClass(&wcl);

// zji�t�n� po�adovan�ch rozm�r� okna
	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = ClientWidth;
	rc.bottom = ClientHeight;
	::AdjustWindowRectEx(&rc, WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_VISIBLE, FALSE, WS_EX_APPWINDOW | WS_EX_CONTROLPARENT | WS_EX_DLGMODALFRAME);
	MainFrameWidth = rc.right - rc.left;
	MainFrameHeight = rc.bottom - rc.top;

// vytvo�en� hlavn�ho okna
	MainFrame = ::CreateWindowEx(
		WS_EX_APPWINDOW | WS_EX_CONTROLPARENT | WS_EX_DLGMODALFRAME,// roz���en� styl
		MainFrameClass,									// jm�no t��dy okna
		"",												// titulek okna
		WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,// styl okna
		(ScreenWidth - MainFrameWidth)/2,				// X
		(ScreenHeight - MainFrameHeight)/2,				// Y
		MainFrameWidth,									// ���ka
		MainFrameHeight,								// v��ka
		NULL,											// rodi�
		NULL,											// menu
		hInstance,										// instance
		NULL);											// parametry
	if (MainFrame == NULL) return false;

	return true;
}
