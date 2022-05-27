
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

int			VerzeOS;					// verze systému
OSVERSIONINFO	OSVersionInfo;			// informace o systému
HINSTANCE	hInstance = NULL;			// instance programu
int			ScreenWidth;				// šíøka klientské oblasti displeje
int			ScreenHeight;				// výška klientské oblasti displeje

TCHAR		MainFrameClass[] = _T("PeterDataInstClass");	// název tøídy hlavního okna
HWND		MainFrame = NULL;			// hlavní okno aplikace

int			MainFrameWidth = 620;		// šíøka hlavního okna
int			MainFrameHeight = 420;		// výška hlavního okna
#define		ClientWidth 608				// šíøka klientské oblasti (musí to být násobek 8 a stejné jako Picture)
#define		ClientHeight 384			// výška klientské oblasti (musí být stejné jako Picture)

int			AktPage = PAGESELECT;			// aktivní stránka instalátoru

__int64	DiskSize = 0;					// velikost disku (z funkce GetDiskSpace)
__int64	DiskFree = 0;					// volné místo disku (z funkce GetDiskSpace)
__int64	DiskFreeUser = 0;				// volné místo uživatele (z funkce GetDiskSpace)

int			DiskFre = 0;				// volné místo cílového disku v MB
int			DiskReq = 0;				// požadované volné místo v MB

GETDISKFREESPACEEX	pGetDiskFreeSpaceEx = NULL;	// funkce GetDiskFreeSpaceEx (NULL=není)

//////////////////////////////////////////////////////////////////////////////
// jazyky

#define	JAZYKENG	0					// anglicky
#define	JAZYKCZ		1					// èesky

int		Jazyk = JAZYKENG;				// vybraný jazyk


//////////////////////////////////////////////////////////////////////////////
// licenèní údaje

CText	InstPath;						// instalaèní cesta (bez \ na konci)
CText	Title;							// titulek


//////////////////////////////////////////////////////////////////////////////
// prvky okna instalátoru

HWND	ButtonNext = NULL;				// tlaèítko další
HWND	ButtonExit = NULL;				// tlaèítko ukonèení
HWND	LineWnd = NULL;					// èára nad tlaèítky
HWND	PathWndNadp = NULL;				// instalaèní cesta - nadpis
HWND	PathWnd = NULL;					// instalaèní cesta
HWND	ButtonList = NULL;				// tlaèítko Procházet
HWND	UserRam = NULL;					// rám uživatele
HWND	LicTextNum = NULL;				// licenení text - eíslo
HWND	LicTextName = NULL;				// licenení text - jméno
HWND	LicTextNadp = NULL;				// licenení text - nadpis
HWND	DiskReqNadp = NULL;				// požadované volné místo - text
HWND	DiskReqNum = NULL;				// požadované volné místo - èíslo
HWND	DiskFreeNadp = NULL;			// volné místo cílového disku - text
HWND	DiskFreeNum = NULL;				// volné místo cílového disku - èíslo
HWND	ButtonCanc = NULL;				// tlaèítko pøerušení
HWND	IndWnd = NULL;					// indikátor instalace
HWND	IndWndNadp = NULL;				// indikátor instalace - nadpis
HWND	IndWndFile = NULL;				// indikátor instalace - soubor
HWND	IndWnd000 = NULL;				// indikátor - 0 %
HWND	IndWnd025 = NULL;				// indikátor - 25 %
HWND	IndWnd050 = NULL;				// indikátor - 50 %
HWND	IndWnd075 = NULL;				// indikátor - 75 %
HWND	IndWnd100 = NULL;				// indikátor - 100 %

enum {
	IDN_BUTTONNEXT = 13001,				// tlaèítko Další
	IDN_BUTTONEXIT,						// tlaèítko Konec
	IDN_LINEWND,						// èára nad tlaèítky
	IDN_PATHWNDNADP,					// instalaèní cesta - nadpis
	IDN_PATHWND,						// instalaèní cesta
	IDN_BUTTONLIST,						// tlaèítko Procházet
	IDN_USERRAM,						// rám uživatele
	IDN_LICTEXTNUM,						// licenèní text - èíslo
	IDN_LICTEXTNAME,					// licenèní text - jméno
	IDN_LICTEXTNADP,					// licenèní text - nadpis
	IDN_DISKREQNADP,					// požadované volné místo - text
	IDN_DISKREQNUM,						// požadované volné místo - èíslo
	IDN_DISKFREENADP,					// volné místo cílového disku - text
	IDN_DISKFREENUM,					// volné místo cílového disku - èíslo
	IDN_BUTTONCANC,						// tlaèítko pøerušení
	IDN_INDWND,							// indikátor instalace
	IDN_INDWNDNADP,						// indikátor instalace - nadpis
	IDN_INDWNDFILE,						// indikátor instalace - soubor
	IDN_INDWND000,						// indikátor instalace - 0%
	IDN_INDWND025,						// indikátor instalace - 25%
	IDN_INDWND050,						// indikátor instalace - 50%
	IDN_INDWND075,						// indikátor instalace - 75%
	IDN_INDWND100,						// indikátor instalace - 100%
};

//////////////////////////////////////////////////////////////////////////////
// pøepínaèe instalovaných èástí

CText		ExeFileName;				// jméno tohoto programu SETUP.EXE
HANDLE		HFile = NULL;				// handle souboru SETUP.EXE
INSTHEAD	Head;						// záhlaví instalaèních dat
FILETIME	datetime;					// datum a èas souborù

int			RawData = 0;				// offset zaèátku dat v souboru
int			RawSize = 0;				// velikost dat v souboru
int			DataOff = 0;				// offset zaèátku dat

#define		PROGMAX		128				// rozsah indikátoru
int			DataSize = 0;				// výchozí velikost instalovaných dat
int			DataSizeOK = 0;				// velikost zpracovaných instalovaných dat
BOOL		Storno = FALSE;				// požadavek pøerušení operace instalace

typedef struct GROUPITEM_ {
	BYTE*	files;						// seznam souborù
} GROUPITEM;

GROUPITEM Groups[GROUPSNUM] =
{
	NULL,
	NULL,
};

CText ProductKey(_T("Software\\Gemtree Software\\Peter"));

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
// rozlišení textu podle jazyku

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
// hlavní start programu

void WinMainCRTStartup()
{
// úschova verze systému
	VerzeOS = (int)::GetVersion();
	OSVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx(&OSVersionInfo);

// handle instance programu
	hInstance = ::GetModuleHandle(NULL);

// nastavení chybového módu
	::SetErrorMode(::SetErrorMode(0) | SEM_FAILCRITICALERRORS | SEM_NOOPENFILEERRORBOX); 

// pøíprava implicitního jazyku
	switch (::GetUserDefaultLangID() & 0xff)
	{
	case 5:				// èesky
	case 21:			// polsky
	case 27:			// slovensky
		Jazyk = JAZYKCZ;
	}

// inicializace informací o displeji
	ScreenWidth = ::GetSystemMetrics(SM_CXFULLSCREEN);
	ScreenHeight = ::GetSystemMetrics(SM_CYFULLSCREEN);

// inicializace správce pamìti
	if (!MemInit()) 
	{
MEMERROR:
		::MessageBox(NULL, 
			X(_T("Nedostatek pamìti ke spuštìní instalátoru."),
				_T("Insufficient memory to run installator.")),
			X(_T("Chyba pamìti"), _T("Memory Error")),
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

// inicializace globálních objektù
	InitTerm(__xc_a, __xc_z);

// zajištìní knihovny rozšíøených ovládacích prvkù
	::InitCommonControls();

// upøesnìní jazyku
	CText txt = GetReg(ProductKey, _T("SetupLang"));
	if ((txt == "5") || (txt == "21") || (txt == "27")) Jazyk = JAZYKCZ;
	if ((txt == "7") || (txt == "9") || (txt == "12")) Jazyk = JAZYKENG;

// vytvoøení hlavního okna aplikace
	if (!MainFrameCreate()) 
	{
		::MessageBox(NULL, 
			X(_T("Nedostatek systémových prostøedkù ke spuštìní instalátoru."),
				_T("Insufficient system resources to run installator.")),
			X(_T("Chyba systému"), _T("System Error")),
			MB_OK | MB_ICONSTOP);
		Exit();
	}

// naètení cesty
	InstPath = GetReg(ProductKey, _T("LastPath"));

// test, zda je první instalace
	if (InstPath.IsEmpty())
	{
		InstPath = _T("C:\\Program Files\\Peter");
	}
	if (InstPath.LastChar() == _T('\\')) InstPath.Delete(InstPath.Length() - 1);

// pøíprava jména programu
	TCHAR buf[_MAX_PATH];
	buf[0] = 0;
	::GetModuleFileName(hInstance, buf, _MAX_PATH);
	ExeFileName = buf;

#ifdef _DEBUG
	ExeFileName = "c:\\vc\\Peter\\Gener\\DataInst.exe";
	ASSERT((int)::GetFileAttributes(ExeFileName) != -1);
#endif

// test, zda nebyla chyba pamìti
	if (MemoryError) goto MEMERROR;

// zapnutí hlavní stránky
	SetPage(PAGESELECT);

// naètení instalaèních informací
	if (!OpenSetup()) Exit();

// pøekreslení hlavní stránky s novými údaji
	SetPage(PAGESELECT);

// hlavní obslužná smyèka aplikace
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

// konec programu
	Exit();
};

//////////////////////////////////////////////////////////////////////////////
// ukonèení programu

void Exit()
{
// ukonèení správce pamìti
	MemTerm();

// ukonèení programu
	ExitProcess(0);
}


//////////////////////////////////////////////////////////////////////////////
// prùbìžné zpracování zpráv

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
// hlášení chyby ètení ze souboru SETUP.EXE (TRUE=pøerušit)

BOOL ReadError()
{
	return (::MessageBox(MainFrame,
				X(
				_T(	"Chyba ètení instalaèních dat,\n"
					"instalaèní program je zøejmì poškozen."),
				_T(	"Error reading installation data,\n"
					"installation program is maybe damaged.")),
				X(_T("Chyba ètení instalaèních dat"), _T("Error Reading Installation Data")),
				MB_RETRYCANCEL | MB_ICONERROR) != IDRETRY);
}


/////////////////////////////////////////////////////////////////////////////
// naètení informací o souborech (vrací FALSE=pøerušit)

BOOL OpenSetup0()
{
// otevøení souboru SETUP.EXE
	HFile = ::CreateFile(ExeFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_READONLY, NULL);

	if (HFile == INVALID_HANDLE_VALUE) return FALSE;

// buffer k naètení záhlaví souboru
	BYTE* buf;
#define BUFSIZE 0x2000
	buf = (BYTE*)MemGet(BUFSIZE);
	CBuf bf;
	bf.Adr(buf);
	bf.Size(BUFSIZE);

// naètení zaèátku souboru
	DWORD read;
	if (!::ReadFile(HFile, buf, BUFSIZE, &read, NULL) || (read != BUFSIZE))
	{
		MemFree(buf);
		::CloseHandle(HFile);
		return FALSE;
	}

// nalezení NT sekce
	IMAGE_NT_HEADERS* hdr = bf.NTHeader();
	if (hdr == NULL)
	{
		MemFree(buf);
		::CloseHandle(HFile);
		return FALSE;
	}

// nalezení datové sekce
	IMAGE_SECTION_HEADER* sec = bf.NTSection(hdr, ".setupd");
	if (sec == NULL)
	{
		MemFree(buf);
		::CloseHandle(HFile);
		return FALSE;
	}
	RawData = sec->PointerToRawData;
	RawSize = sec->SizeOfRawData;

// zrušení bufferu
	MemFree(buf);
	if (RawSize < 1000) return FALSE;

// naètení a kontrola záhlaví dat
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

// kontrolní souèet záhlaví
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

// naètení seznamù souborù
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

// kontrolní souèet seznamu souborù
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

// úschova zaèátku dat
	DataOff = ::SetFilePointer(HFile, 0, NULL, FILE_CURRENT);

// uzavøení souboru SETUP.EXE
	::CloseHandle(HFile);

// kontrola kontrolního souètu
	if (Head.Check != check) return FALSE;
	if (Head.Groups[0].Files < 2) return FALSE;

// korekce data a èasu souborù
	datetime = Head.DateTime;
	::LocalFileTimeToFileTime(&Head.DateTime, &datetime);

// pøíprava instalaèní složky (pozor - smí se provést pouze jednou, jinak se zmìní InstPath !!!!)
	data = Groups[0].files;
	CText pth;
	pth.Set(((INSTFILE*)data)->Name, ((INSTFILE*)data)->NameN);
	data += 2*sizeof(long) + sizeof(BYTE) + ((INSTFILE*)data)->NameN;
	if (pth.LastChar() == _T('\\')) pth.Delete(pth.Length() - 1);
	if (pth.IsNotEmpty() && (pth[0] == _T('\\'))) pth.Delete(0,1);
	if (pth.IsNotEmpty())
	{
		InstPath = InstPath + _T('\\') + pth;
	}

// pøíprava titulku
	pth.Set(((INSTFILE*)data)->Name, ((INSTFILE*)data)->NameN);

	switch (Jazyk)
	{
	case JAZYKCZ:
		Title = pth.GetLine(0);
		break;

	default:
		Title = pth.GetLine(1);
	}

	if (Title.IsEmpty()) Title = pth.GetLine(0);

	return TRUE;
}

BOOL OpenSetup()
{
	BOOL res;
	do {
		res = OpenSetup0();

		if (!res)
		{
			if (ReadError()) return FALSE;
		}

	} while (!res);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// zobrazení požadovaného místa

void DispDiskReq()
{
	DiskReq = 0;

	for (int i = 1; i < GROUPSNUM; i++)
	{
		DiskReq += (Head.Groups[i].Size + 1023)/1024;
	}

	CText txt;
	txt.Int(DiskReq);
	txt = _T("  ") + txt + _T(" MB");
	txt.SetWindowText(DiskReqNum);
}


/////////////////////////////////////////////////////////////////////////////
// zjištìní tlaèítka s fokusem (-1 = není)

int GetButton()
{
	HWND wnd = ::GetFocus();
	if (wnd == NULL) return -1;

	if (wnd == ButtonNext) return IDN_BUTTONNEXT;
	if (wnd == ButtonExit) return IDN_BUTTONEXIT;
	if (wnd == ButtonList) return IDN_BUTTONLIST;
	if (wnd == ButtonCanc) return IDN_BUTTONCANC;
	if (wnd == PathWnd) return IDN_PATHWND;

	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// nastavení tlaèítka s fokusem

void SetButton(int id)
{
	switch (id)
	{
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

	case IDN_PATHWND:
		::SetFocus(PathWnd);
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// obsluha zprávy pøed rozesláním do oken (TRUE = zpráva zpracována)

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
						SetButton(IDN_BUTTONLIST);
						return TRUE;

					case IDN_BUTTONNEXT:
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
						SetButton(IDN_BUTTONNEXT);
						return TRUE;

					case IDN_BUTTONLIST:
						SetButton(IDN_PATHWND);
						return TRUE;
					}
				}
				wParam = VK_RIGHT;
			}
		}

		switch (AktPage)
		{
		case PAGESELECT:
			switch (wParam)
			{
			case VK_ESCAPE:
				::ShowWindow(MainFrame, SW_HIDE);
				::SendMessage(MainFrame, WM_CLOSE, 0, 0);
				return TRUE;

			case VK_LEFT:
				switch (GetButton())
				{
				case IDN_PATHWND:
					return FALSE;

				case IDN_BUTTONNEXT:
					SetButton(IDN_BUTTONLIST);
					return TRUE;

				case IDN_BUTTONLIST:
					SetButton(IDN_BUTTONEXIT);
					return TRUE;

				case IDN_BUTTONEXIT:
					SetButton(IDN_BUTTONNEXT);
					return TRUE;
				}
				break;

			case VK_RIGHT:
				switch (GetButton())
				{
				case IDN_BUTTONNEXT:
					SetButton(IDN_BUTTONEXIT);
					return TRUE;

				case IDN_BUTTONEXIT:
					SetButton(IDN_BUTTONLIST);
					return TRUE;

				case IDN_BUTTONLIST:
					SetButton(IDN_BUTTONNEXT);
					return TRUE;

				case IDN_PATHWND:
					return FALSE;
				}
				break;

			case VK_RETURN:
				msg->wParam = VK_SPACE;
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
		switch (wParam)
		{
		case VK_RETURN:
			msg->wParam = VK_SPACE;
			break;
		}
		break;
	}
	return FALSE;
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
// dekomprese dat

#define MAXLENX 25					// min. délka dlouhého øetìzce
#define MAXLEN (MAXLENX+254)		// maximální délka øetìzce
#define SUBSTLEN	7				// délka nahrazená dlouhým kódem

class CDekomp
{
private:
	BYTE*	m_DstBeg;				// výstupní buffer - zaèátek nezpracovaných dat
	BYTE*	m_DstEnd;				// konec výstupního bufferu
	BYTE*	m_Dst;					// ukládací adresa

	BYTE*	m_SrcEnd;				// konec dat ve vstupním bufferu
	BYTE*	m_Src;					// ètecí adresa

	WORD	m_Status;				// støadaè stavového slova

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

// naètení jednoho bitu (výstup = hodnota 0 nebo 1)
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
//  Ve zdrojovém bloku je potøeba nechat za koncem dat rezervu asi tak nìjak 10 bajtù (pokud možno platných)
//  V cílovém bloku je potøeba nechat za koncem dat rezervu asi tak nìjak 300 bajtù,
//	pøed zaèátkem cílových dat je potøeba ponechat rezervu 32 KB starých dat.

	void DeKomp()
	{

// lokální promìnné
		BYTE offsetL, offsetH;			// offset k opakování
		int delka;						// délka k opakování

// kontrola pøeteèení bufferù
		while ((m_Dst < m_DstEnd) && (m_Src < m_SrcEnd))
		{

// pøesun bajtu bez komprese
			if (DekBit() == 0)
			{
				*m_Dst = *m_Src;
				m_Dst++;
				m_Src++;
			}

// jinak bude opakování øetìzce
			else
			{
				offsetH = 0;
				offsetL = 0;

// první bit délky
				delka = DekBit();

// zvýšení èítaèe délky
				for (;;)
				{
					delka++;
					delka++;

// naètení pøíznaku konce kódu
					if (DekBit() == 0) break;

// dosažení maximální délky
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

// stanovení offsetu - vyšší bajt
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

// naètení offsetu - nižší bajt
				offsetL = *m_Src;
				m_Src++;

// pøenesení øetìzce (nepoužívat MemCopy - nepøenáší po bajtech!!!)
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


/////////////////////////////////////////////////////////////////////////////
// naètení registru

CText GetReg(CText key, CText name)
{
// text výsledné hodnoty
	CText txt;

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

// uzavøení klíèe
		::RegCloseKey(hkey);
	}

	return txt;
}


/////////////////////////////////////////////////////////////////////////////
// provedení instalace

void Install()
{
	Storno = FALSE;
	int i,j;

// cesta k ukládání souborù
	CText path = InstPath;
	if (path.LastChar() != _T('\\')) path += _T('\\');

// zjištìní velikosti souborù (kvùli indikaci)
	DataSize = 0;
	DataSizeOK = 0;
	for (i = 1; i < GROUPSNUM; i++)
	{
		BYTE* data = Groups[i].files;

		for (j = Head.Groups[i].Files; j > 0; j--)
		{
			DataSize += ((INSTFILE*)data)->Size;
			data += 2*sizeof(long) + sizeof(BYTE) + ((INSTFILE*)data)->NameN;
		}
	}

// dekompresní buffer
	CDekomp dek;

// otevøení vstupního souboru
	do {
		HFile = ::CreateFile(ExeFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_READONLY, NULL);

		if (HFile == INVALID_HANDLE_VALUE)
		{
			if (ReadError())
			{
				SetPage(PAGESELECT);
				return;
			}
		}

	} while (HFile == INVALID_HANDLE_VALUE);

// vstupní a výstupní buffery
#define SRCBUFSIZE 0x8000						// celková velikost zdrojového bufferu
#define SRCBUFREZ  32							// rezerva za zdrojovými daty
	BYTE* srcbuf = (BYTE*)MemGet(SRCBUFSIZE + 1024);
#define DSTBUFSIZE 0x12000						// celková velikost cílového bufferu
#define DSTBUFDATA (DSTBUFSIZE/2)				// zaèátek dat v cílovém bufferu
#define DSTBUFREZ  1024							// rezerva za cílovými daty (a na zaèátku bufferu)
	BYTE* dstbuf = (BYTE*)MemGet(DSTBUFSIZE + 1024);

// pøíprava ukazatele dat
	DWORD readwrite;
	int off = DataOff;

// buffery pro naèítání aliases
	CText AliasGroup;			// jméno skupiny
	CText AliasKey;				// jméno parametru
	CText AliasLine;			// jeden naètený øádek textu
	CText AliasText;			// støadaè naèítaného textu

// cyklus pøes všechny skupiny
	for (i = 1; i < GROUPSNUM; i++)
	{

// naètení prvního bloku dat skupiny
			int srcsize = Head.Groups[i].SizeGroup;						// velikost dat skupiny
			int srcsize0 = srcsize;
			if (srcsize0 > SRCBUFSIZE) srcsize0 = SRCBUFSIZE;			// velikost dat k naètení do bufferu
			srcsize -= srcsize0;

			BOOL ok;
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
					if (ReadError())
					{
						MemFree(srcbuf);
						MemFree(dstbuf);
						SetPage(PAGESELECT);
						return;
					}
				}

			} while (!ok);

// prùbìžný ukazatel ètených dat
			int off0 = off + srcsize0;

// pøíprava dekompresního bufferu - poèáteèní dekomprese
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

// cyklus pøes všechny soubory
			BYTE* data = Groups[i].files;
			for (j = Head.Groups[i].Files; j > 0; j--)
			{

// parametry souboru
				int check0 = 0;
				int size = ((INSTFILE*)data)->Size;
				int check = ((INSTFILE*)data)->Check;
				CText name(((INSTFILE*)data)->Name, ((INSTFILE*)data)->NameN);
				data += 2*sizeof(long) + sizeof(BYTE) + ((INSTFILE*)data)->NameN;
				name = path + name;
				name.SetWindowText(IndWndFile);

// test, zda to je soubor ALIASES.INI
				CText ali = name.Right(11);
				ali.UpperCase();
				bool IsAli = false;
				if (ali == _T("ALIASES.INI")) IsAli = true;

// pro ALIASES vytvoøení cesty a povolení zápisu do ALIASES
				HANDLE file = INVALID_HANDLE_VALUE;

				if (IsAli)
				{
					CreatePath(name.Left(name.RevFind(_T('\\'))));
					::SetFileAttributes(name, FILE_ATTRIBUTE_NORMAL);

					AliasGroup = _T("LANGID 9");
					AliasText.Empty();
				}
				else				

// vytvoøení cílového souboru
				{
					file = ::CreateFile(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

// pøi neúspìchu druhý pokus - s vytvoøením adresáøe
					if (file == INVALID_HANDLE_VALUE)
					{
						CreatePath(name.Left(name.RevFind(_T('\\'))));
						file = ::CreateFile(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

// další pokus - zrušení atributu R/O souboru
						if (file == INVALID_HANDLE_VALUE)
						{
							::SetFileAttributes(name, FILE_ATTRIBUTE_NORMAL);
							file = ::CreateFile(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
						}
					}
				}

// obsluha zpráv
				PumpMessage();

// chyba - soubor nelze vytvoøit
				if (!IsAli && (file == INVALID_HANDLE_VALUE))
				{
					::MessageBox(MainFrame,
						X(
						CText(
							_T("Chyba - nelze vytvoøit cílový soubor\n\n") +
							name +
							_T("\n\nInstalace nemùže dále pokraèovat a bude pøerušena. Dùvodem mùže být:"
								"\n   - pøepisovaný cílový program EXE je spuštìn"
								"\n   - pøepisovaný cílový soubor je otevøen nìkterým programem"
								"\n   - chybné zadání cílové složky (napø. neexistující disk)"
								"\n   - plný cílový disk nebo zákaz zápisu na disk")),
						CText(
							_T("Error - could not create destination file\n\n") +
							name +
							_T("\n\nInstallation can't continue and will be interrupted. It can be caused by:"
								"\n   - overwriten destination EXE program is running"
								"\n   - overwriten destination file is opened by some other program"
								"\n   - wrongly entered destination folder (e.g. invalid disk)"
								"\n   - full destination disk or write protection on disk"))),
						X(_T("Chyba zápisu"), _T("Write Error")),
						MB_OK | MB_ICONERROR);

					MemFree(srcbuf);
					MemFree(dstbuf);
					SetPage(PAGESELECT);
					return;
				}

// cyklus uložení všech dat
				while (size > 0)
				{

// velikost bloku dat pøipravených v bufferu
					int size0 = dek.Dst() - dek.DstBeg();
					if (size0 <= 0)
					{

// naètení dalšího bloku zdrojoých dat
						if (srcsize <= 0)		// aby bylo ještì nìco naèteno za koncem dat
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
								if (ReadError())
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

// posun ukazatelù a dat v bufferu
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

					if (size0 > size) size0 = size;
					size -= size0;

// zápis ALIASES
					if (IsAli)
					{
						AliasLine.Set((char*)dek.DstBeg(), size0);
						AliasText += AliasLine;

						while (AliasText.IsNotEmpty() && 
								((size == 0) || (AliasText.Find(_T(13)) >= 0)))
						{
							AliasLine = AliasText.GetLine(0);
							AliasText.Delete(0, AliasLine.Length());

							while (AliasText.IsNotEmpty() && 
									((AliasText[0] == _T(13)) || (AliasText[0] == _T(10))))
							{
								AliasText.Delete(0, 1);
							}

							AliasLine.TrimLeft();
							AliasLine.TrimRight();

							if (AliasLine.IsNotEmpty())
							{
								if (AliasLine[0] == _T('['))
								{
									AliasGroup = AliasLine;
									AliasGroup.Delete(0, 1);
									AliasGroup.Delete(AliasGroup.Length() - 1, 1);
								}
								else
								{
									int pos = AliasLine.Find(_T('='));
									if (pos >= 0)
									{
										AliasKey = AliasLine.Left(pos);
										AliasKey.TrimRight();

										AliasLine.Delete(0, pos + 1);

										::WritePrivateProfileString(AliasGroup, AliasKey, AliasLine, name);
									}
								}
							}
						}
					}

// zápis bloku dat
					else
					{
						if (!::WriteFile(file, dek.DstBeg(), size0, &readwrite, NULL) || (size0 != (int)readwrite))
						{
							::CloseHandle(file);
							::DeleteFile(name);

							::MessageBox(MainFrame,
								X(
								CText(
									_T("Chyba zápisu do cílového souboru\n\n") +
									name +
									_T("\n\nCílový disk je zøejmì plný."
										"\nInstalace nemùže dále pokraèovat a bude pøerušena.")),
								CText(
									_T("Error writing destination file\n\n") +
									name +
									_T("\n\nDestination disk is maybe full."
										"\nInstallation can't continue and will be interrupted."))),
								X(_T("Chyba zápisu"), _T("Write Error")),
								MB_OK | MB_ICONERROR);

							MemFree(srcbuf);
							MemFree(dstbuf);
							SetPage(PAGESELECT);
							return;
						}
					}

					BYTE* d = dek.DstBeg();
					dek.DstBeg(dek.DstBeg() + size0);

// zobrazení indikátoru operace
					DataSizeOK += size0;
					::SendMessage(IndWnd, PBM_SETPOS, ::MulDiv(DataSizeOK, PROGMAX, DataSize), 0);

// obsluha zpráv
					PumpMessage();

// kontrolní souèet bloku dat
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

// nastavení data a èasu souboru
				if (!IsAli)
				{
					::SetFileTime(file, NULL, NULL, &datetime);

// uzavøení cílového souboru
					::CloseHandle(file);
				}

// kontrola kontrolního souètu souboru
				if (check != check0)
				{
					::DeleteFile(name);

					::MessageBox(MainFrame,
						X(
						CText(
							_T("Chyba instalace souboru\n\n") +
							name +
							_T("\n\nInstalaèní program je poškozen."
								"\nInstalace nemùže dále pokraèovat a bude pøerušena.")),
						CText(
							_T("Installation error of the file\n\n") +
							name +
							_T("\n\nInstall program is damaged."
								"\nInstallation can't continue and will be interrupted."))),
						X(_T("Chyba instalace"), _T("Installation Error")),
						MB_OK | MB_ICONERROR);

					MemFree(srcbuf);
					MemFree(dstbuf);
					SetPage(PAGESELECT);
					return;
				}

// obsluha zpráv
				PumpMessage();

// pøerušení instalace
				if (Storno)
				{
					if (::MessageBox(MainFrame,
							X(
							CText(
								_T("Pøerušením instalace zùstanou ve složce ") +
								path +
								_T("\njiž nainstalované soubory. Soubory mùžete zrušit Prùzkumníkem."
									"\n\nOpravdu chcete instalaci pøerušit?")),
							CText(
								_T("There will remain already installed files in folder ") +
								path +
								_T("\nif you break installation. You can delete those files by File Explorer."
									"\n\nDo you really want to break the installation?"))),

							X(_T("Pøerušení instalace"), _T("Installation Break")),
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

// posun ukazatele dat
		off += Head.Groups[i].SizeGroup;
	}

// zrušení vypisovaného textu souboru
	EmptyText.SetWindowText(IndWndFile);

// zrušení bufferu
	MemFree(srcbuf);
	MemFree(dstbuf);

// instalace ukonèena OK
	SetPage(PAGEOK);
}


/////////////////////////////////////////////////////////////////////////////
// zjištìní informací o volném místu na cílovém disku

void GetDiskSpace()
{
// úprava na plné jméno
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

// naètení informací novìjší funkcí
	if (!pGetDiskFreeSpaceEx ||
		!pGetDiskFreeSpaceEx(txt, &DiskFreeUser, &DiskSize, &DiskFree))

// naètení informací starší metodou
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

// volné místo
	DiskFre = *(long*)((BYTE*)&DiskFree + 2) >> 4;

// zobrazení informací
	txt.Int(DiskFre);
	txt += _T(" MB");
	txt.SetWindowText(DiskFreeNum);
}


/////////////////////////////////////////////////////////////////////////////
// vytvoøení èáry nad tlaèítky

void CreateLineWnd()
{
	if (LineWnd == NULL)
	{
		LineWnd = ::CreateWindowEx(
			0,												// rozšíøený styl
			_T("STATIC"),									// jméno tøídy okna
			NULL,											// titulek okna
			SS_ETCHEDHORZ | WS_VISIBLE | WS_CHILD,			// styl okna
			10,												// X
			ClientHeight - 58,								// Y
			ClientWidth - 20,								// šíøka
			6,												// výška
			MainFrame,										// rodiè
			(HMENU)IDN_LINEWND,								// menu
			hInstance,										// instance
			NULL);											// parametry
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení rámu uživatele

void CreateUserRam()
{
	if (AktPage == PAGEOK)
	{
		if (LicTextName == NULL)
		{
			LicTextName = ::CreateWindowEx(
				0,												// rozšíøený styl
				_T("STATIC"),									// jméno tøídy okna
				X(_T("Instalace byla úspìšnì dokonèena."), _T("Installation was successfuly completed.")),
				SS_CENTER | WS_VISIBLE | WS_CHILD,				// styl okna
				10,												// X
				160,											// Y
				ClientWidth - 20,								// šíøka
				20,												// výška
				MainFrame,										// rodiè
				(HMENU)IDN_LICTEXTNAME,							// menu
				hInstance,										// instance
				NULL);											// parametry
		}
	}
	else
	{
		if (LicTextName == NULL)
		{
			LicTextName = ::CreateWindowEx(
				0,												// rozšíøený styl
				_T("STATIC"),									// jméno tøídy okna
				X(_T("Stiskem tlaèítka 'Dokonèit' zahájíte instalaci."), _T("You can start installation by pressing the pushbutton 'Finish'.")),
				SS_CENTER | WS_VISIBLE | WS_CHILD,				// styl okna
				10,												// X
				35,												// Y
				ClientWidth - 20,								// šíøka
				20,												// výška
				MainFrame,										// rodiè
				(HMENU)IDN_LICTEXTNAME,							// menu
				hInstance,										// instance
				NULL);											// parametry
		}

		if (UserRam == NULL)
		{
			UserRam = ::CreateWindowEx(
				0,												// rozšíøený styl
				_T("STATIC"),									// jméno tøídy okna
				NULL,											// titulek okna
				SS_ETCHEDFRAME | WS_VISIBLE | WS_CHILD,			// styl okna
				10,												// X
				80,												// Y
				ClientWidth - 20,								// šíøka
				100,											// výška
				MainFrame,										// rodiè
				(HMENU)IDN_USERRAM,								// menu
				hInstance,										// instance
				NULL);											// parametry
		}

		if (LicTextNadp == NULL)
		{
			LicTextNadp = ::CreateWindowEx(
				0,												// rozšíøený styl
				_T("STATIC"),									// jméno tøídy okna
				X(_T("Instalovaný doplnìk:"), _T("Installed supplement:")),
				SS_LEFT | WS_VISIBLE | WS_CHILD,				// styl okna
				20,												// X
				85,												// Y
				ClientWidth - 40,								// šíøka
				20,												// výška
				MainFrame,										// rodiè
				(HMENU)IDN_LICTEXTNADP,							// menu
				hInstance,										// instance
				NULL);											// parametry
			::SendMessage(LicTextNadp, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);
		}

		if (LicTextNum == NULL)
		{
			LicTextNum = ::CreateWindowEx(
				0,												// rozšíøený styl
				_T("STATIC"),									// jméno tøídy okna
				Title,
				SS_CENTER | WS_VISIBLE | WS_CHILD,				// styl okna
				20,												// X
				123,											// Y
				ClientWidth - 40,								// šíøka
				20,												// výška
				MainFrame,										// rodiè
				(HMENU)IDN_LICTEXTNUM,							// menu
				hInstance,										// instance
				NULL);											// parametry
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// vytvoøení instalaèní cesty

void CreatePathWnd()
{
	if (PathWndNadp == NULL)
	{
		PathWndNadp = ::CreateWindowEx(
			0,												// rozšíøený styl
			_T("STATIC"),									// jméno tøídy okna
			X(_T("Cílová složka:"),	_T("Destination folder:")),	// titulek okna
			SS_LEFT | WS_VISIBLE | WS_CHILD,				// styl okna
			10,												// X
			ClientHeight - 115,								// Y
			200,											// šíøka
			20,												// výška
			MainFrame,										// rodiè
			(HMENU)IDN_PATHWNDNADP,							// menu
			hInstance,										// instance
			NULL);											// parametry

		::SendMessage(PathWndNadp, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);
	}

	if (PathWnd == NULL)
	{
		PathWnd = ::CreateWindowEx(
			WS_EX_CLIENTEDGE,								// rozšíøený styl
			_T("EDIT"),										// jméno tøídy okna
			InstPath,										// titulek okna
			ES_AUTOHSCROLL | ES_LEFT | WS_GROUP | WS_TABSTOP | WS_VISIBLE | WS_CHILD,			// styl okna
			20,												// X
			ClientHeight - 93,								// Y
			ClientWidth - 150,								// šíøka
			20,												// výška
			MainFrame,										// rodiè
			(HMENU)IDN_PATHWND,								// menu
			hInstance,										// instance
			NULL);											// parametry

		::SendMessage(PathWnd, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení tlaèítka

HWND CreateButton(int id, int x, LPCTSTR text)
{
	HWND wnd = ::CreateWindowEx(
			0,												// rozšíøený styl
			_T("BUTTON"),									// jméno tøídy okna
			text,											// titulek okna
			BS_PUSHBUTTON | BS_TEXT | BS_NOTIFY | WS_GROUP | WS_TABSTOP | WS_VISIBLE | WS_CHILD,// styl okna
			x,												// X
			ClientHeight - 40,								// Y
			100,											// šíøka
			26,												// výška
			MainFrame,										// rodiè
			(HMENU)id,										// menu
			hInstance,										// instance
			NULL);											// parametry
	::SendMessage(wnd, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);

	return wnd;
}

HWND CreateDefButton(int id, int x, LPCTSTR text)
{
	HWND wnd = ::CreateWindowEx(
			0,												// rozšíøený styl
			_T("BUTTON"),									// jméno tøídy okna
			text,											// titulek okna
			BS_DEFPUSHBUTTON | BS_TEXT | BS_NOTIFY | WS_GROUP | WS_TABSTOP | WS_VISIBLE | WS_CHILD,// styl okna
			x,												// X
			ClientHeight - 40,								// Y
			100,												// šíøka
			26,												// výška
			MainFrame,										// rodiè
			(HMENU)id,										// menu
			hInstance,										// instance
			NULL);											// parametry
	::SendMessage(wnd, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);

	return wnd;
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení tlaèítka Další

void CreateNext()
{
	if (ButtonNext == NULL)
	{
		ButtonNext = CreateDefButton(IDN_BUTTONNEXT, ClientWidth/2 - 100 - 30, X(_T("Dokonèit"), _T("Finish")));
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení tlaèítka Konec

void CreateExit()
{
	if (ButtonExit == NULL)
	{
		if (AktPage == PAGEOK)
		{
			ButtonExit = CreateButton(IDN_BUTTONEXIT, (ClientWidth - 100)/2, X(_T("Konec"), _T("Quit")));
		}
		else
		{
			ButtonExit = CreateButton(IDN_BUTTONEXIT, ClientWidth/2 + 30, X(_T("Konec"), _T("Quit")));
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// vytvoøení tlaèítka Storno

void CreateCanc()
{
	if (ButtonCanc == NULL)
	{
		ButtonCanc = CreateButton(IDN_BUTTONCANC, (ClientWidth - 100)/2, X(_T("Storno"), _T("Cancel")));
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení tlaèítka Procházet

void CreateList()
{
	if (ButtonList == NULL)
	{
		ButtonList = ::CreateWindowEx(
				0,												// rozšíøený styl
				_T("BUTTON"),									// jméno tøídy okna
				X(_T("Procházet..."), _T("Browse...")),	// titulek okna
				BS_PUSHBUTTON | BS_TEXT | BS_NOTIFY | WS_GROUP | WS_TABSTOP | WS_VISIBLE | WS_CHILD,// styl okna
				ClientWidth - 20 - 100,							// X
				ClientHeight - 100,								// Y
				100,											// šíøka
				26,												// výška
				MainFrame,										// rodiè
				(HMENU)IDN_BUTTONLIST,							// menu
				hInstance,										// instance
				NULL);											// parametry
		::SendMessage(ButtonList, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení indikátoru instalace

void CreateIndWnd()
{
	if (IndWnd == NULL)
	{
		IndWnd = ::CreateWindowEx(
				WS_EX_CLIENTEDGE,								// rozšíøený styl
				PROGRESS_CLASS,									// jméno tøídy okna
				NULL,											// titulek okna
				WS_CHILD | WS_VISIBLE,							// styl okna
				20,												// X
				200,											// Y
				ClientWidth - 40,								// šíøka
				20,												// výška
				MainFrame,										// rodiè
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
		IndWndNadp = ::CreateWindowEx(
			0,												// rozšíøený styl
			_T("STATIC"),									// jméno tøídy okna
			X(_T("Probíhá instalace doplòkù aplikace Petr ..."), _T("Proceeding installation of Peter application supplements ...")),		// titulek okna
			SS_CENTER | WS_VISIBLE | WS_CHILD,				// styl okna
			30,												// X
			100,											// Y
			ClientWidth - 60,								// šíøka
			20,												// výška
			MainFrame,										// rodiè
			(HMENU)IDN_INDWNDNADP,							// menu
			hInstance,										// instance
			NULL);											// parametry
	}

	if (IndWndFile == NULL)
	{
		IndWndFile = ::CreateWindowEx(
			0,												// rozšíøený styl
			_T("STATIC"),									// jméno tøídy okna
			NULL,											// titulek okna
			SS_LEFT | WS_VISIBLE | WS_CHILD,				// styl okna
			20,												// X
			170,											// Y
			ClientWidth - 30,								// šíøka
			20,												// výška
			MainFrame,										// rodiè
			(HMENU)IDN_INDWNDFILE,							// menu
			hInstance,										// instance
			NULL);											// parametry
		::SendMessage(IndWndFile, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);
	}

	if (IndWnd000 == NULL)
	{
		IndWnd000 = ::CreateWindowEx(
			0,												// rozšíøený styl
			_T("STATIC"),									// jméno tøídy okna
			_T("0%"),										// titulek okna
			SS_CENTER | WS_VISIBLE | WS_CHILD,				// styl okna
			5,												// X
			225,											// Y
			30,												// šíøka
			20,												// výška
			MainFrame,										// rodiè
			(HMENU)IDN_INDWND000,							// menu
			hInstance,										// instance
			NULL);											// parametry
		::SendMessage(IndWnd000, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);
	}

	if (IndWnd025 == NULL)
	{
		IndWnd025 = ::CreateWindowEx(
			0,												// rozšíøený styl
			_T("STATIC"),									// jméno tøídy okna
			_T("25%"),										// titulek okna
			SS_CENTER | WS_VISIBLE | WS_CHILD,				// styl okna
			5 + 1*(ClientWidth - 40)/4,						// X
			225,											// Y
			30,												// šíøka
			20,												// výška
			MainFrame,										// rodiè
			(HMENU)IDN_INDWND025,							// menu
			hInstance,										// instance
			NULL);											// parametry
		::SendMessage(IndWnd025, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);
	}

	if (IndWnd050 == NULL)
	{
		IndWnd050 = ::CreateWindowEx(
			0,												// rozšíøený styl
			_T("STATIC"),									// jméno tøídy okna
			_T("50%"),										// titulek okna
			SS_CENTER | WS_VISIBLE | WS_CHILD,				// styl okna
			5 + 2*(ClientWidth - 40)/4,						// X
			225,											// Y
			30,												// šíøka
			20,												// výška
			MainFrame,										// rodiè
			(HMENU)IDN_INDWND050,							// menu
			hInstance,										// instance
			NULL);											// parametry
		::SendMessage(IndWnd050, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);
	}

	if (IndWnd075 == NULL)
	{
		IndWnd075 = ::CreateWindowEx(
			0,												// rozšíøený styl
			_T("STATIC"),									// jméno tøídy okna
			_T("75%"),										// titulek okna
			SS_CENTER | WS_VISIBLE | WS_CHILD,				// styl okna
			5 + 3*(ClientWidth - 40)/4,						// X
			225,											// Y
			30,												// šíøka
			20,												// výška
			MainFrame,										// rodiè
			(HMENU)IDN_INDWND075,							// menu
			hInstance,										// instance
			NULL);											// parametry
		::SendMessage(IndWnd075, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);
	}

	if (IndWnd100 == NULL)
	{
		IndWnd100 = ::CreateWindowEx(
			0,												// rozšíøený styl
			_T("STATIC"),									// jméno tøídy okna
			_T("100%"),										// titulek okna
			SS_CENTER | WS_VISIBLE | WS_CHILD,				// styl okna
			-2 + (ClientWidth - 40),						// X
			225,											// Y
			30,												// šíøka
			20,												// výška
			MainFrame,										// rodiè
			(HMENU)IDN_INDWND100,							// menu
			hInstance,										// instance
			NULL);											// parametry
		::SendMessage(IndWnd100, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení informací o disku

void CreateDiskFree()
{
	if (DiskReqNadp == NULL)
	{
		DiskReqNadp = ::CreateWindowEx(
			0,												// rozšíøený styl
			_T("STATIC"),									// jméno tøídy okna
			X(_T("Požadované volné místo:"), _T("Required free space:")),					// titulek okna
			SS_LEFT | WS_VISIBLE | WS_CHILD,				// styl okna
			330,											// X
			210,											// Y
			180,											// šíøka
			18,												// výška
			MainFrame,										// rodiè
			(HMENU)IDN_DISKREQNADP,							// menu
			hInstance,										// instance
			NULL);											// parametry
		::SendMessage(DiskReqNadp, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);
	}

	if (DiskReqNum == NULL)
	{
		DiskReq = 0;

		for (int i = 1; i < GROUPSNUM; i++)
		{
			DiskReq += (Head.Groups[i].Size + 1023)/1024;
		}
		
		CText txt;
		txt.Int(DiskReq);
		txt += _T(" MB");

		DiskReqNum = ::CreateWindowEx(
			0,												// rozšíøený styl
			_T("STATIC"),									// jméno tøídy okna
			txt,											// titulek okna
			SS_RIGHT | WS_VISIBLE | WS_CHILD,				// styl okna
			510,											// X
			210,											// Y
			70,												// šíøka
			18,												// výška
			MainFrame,										// rodiè
			(HMENU)IDN_DISKREQNADP,							// menu
			hInstance,										// instance
			NULL);											// parametry
		::SendMessage(DiskReqNum, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);
	}

	if (DiskFreeNadp == NULL)
	{
		DiskFreeNadp = ::CreateWindowEx(
			0,												// rozšíøený styl
			_T("STATIC"),									// jméno tøídy okna
			X(_T("Volné místo na cílovém disku:"), _T("Free space on destination disk:")),	// titulek okna
			SS_LEFT | WS_VISIBLE | WS_CHILD,				// styl okna
			330,											// X
			230,											// Y
			180,											// šíøka
			18,												// výška
			MainFrame,										// rodiè
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
			0,												// rozšíøený styl
			_T("STATIC"),									// jméno tøídy okna
			txt,											// titulek okna
			SS_RIGHT | WS_VISIBLE | WS_CHILD,				// styl okna
			510,											// X
			230,											// Y
			70,												// šíøka
			18,												// výška
			MainFrame,										// rodiè
			(HMENU)IDN_DISKFREENUM,							// menu
			hInstance,										// instance
			NULL);											// parametry
		::SendMessage(DiskFreeNum, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);
	}
}


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

/////////////////////////////////////////////////////////////////////////////
// výbìr cílového adresáøe

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

			txt = X(_T("&Název složky:"), _T("Folder &Name:"));
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
// pøíprava struktury k zadání jména souboru
	OPENFILENAME ofn;
	MemFill(&ofn, sizeof(ofn), 0);
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = MainFrame;
	ofn.lpstrFilter = X(_T("Složky souborù\0 . \0\0"), _T("File folders\0 . \0\0"));
	ofn.Flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_SHAREAWARE | OFN_ENABLEHOOK | OFN_ENABLESIZING |
			OFN_NOVALIDATE | OFN_NOTESTFILECREATE | OFN_LONGNAMES | OFN_EXPLORER;
	ofn.lpstrTitle = X(_T("Cílová složka k instalaci doplòku aplikace Petr"), _T("Destination Folder to Install Peter Application Supplement"));
	ofn.lpfnHook = OFNHookProc;

// aktuální adresáø
	CText path;
	path = InstPath;
	path.Delete(path.RevFind(_T('\\')));
	ofn.lpstrInitialDir = path;

// pøednastavené jméno souboru
	CText name;
	name = InstPath.Right(InstPath.Length() - (InstPath.RevFind(_T('\\')) + 1));
	int n = name.Length() + 1;
	if (n < 1024) n = 1024;

// buffer k zadání jména souboru
	TCHAR* file = (TCHAR*)MemGet(n*sizeof(TCHAR));
	if (file == NULL) return;
	MemFill(file, n*sizeof(TCHAR), 0);
	name.Write(file);
	ofn.lpstrFile = file;
	ofn.nMaxFile = n;

// zadání jména souboru
	if (!::GetSaveFileName(&ofn))
	{
		MemFree(file);
		SetButton(IDN_BUTTONLIST);
		return;
	}

// korekce jména souboru
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
		name.Delete(name.Length() - 1);
	}

// uložení jména souboru
	InstPath = name;
	name.SetWindowText(PathWnd);

// aktualizace volného místa na cílovém disku
	GetDiskSpace();

// aktualizace zobrazení okna
	::InvalidateRect(MainFrame, NULL, TRUE);
	::UpdateWindow(MainFrame);

	SetButton(IDN_BUTTONLIST);
}


/////////////////////////////////////////////////////////////////////////////
// nastavení stránky instalátoru

void SetPage(int page)
{

// nová aktivní stránka
	AktPage = page;

// zrušení všech prvkù
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

	if (LicTextNadp)
	{
		::DestroyWindow(LicTextNadp);
		LicTextNadp = NULL;
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

// vytvoøení nového okna
	switch (page)
	{

// okno výbìru složek	
	case PAGESELECT:
		GetDiskSpace();					// naètení informací o cílovém disku
		CreateNext();					// vytvoøení tlaèítka Další
		CreateExit();					// vytvoøení tlaèítka Konec
		CreateList();					// vytvoøení tlaèítka Procházet
		CreateLineWnd();				// vytvoøení èáry nad tlaèítky
		CreatePathWnd();				// vytvoøení textu cesty
		CreateUserRam();				// vytvoøení uživatelského rámu
		CreateDiskFree();				// zobrazení informací o disku

		::SetFocus(ButtonNext);			// fokus na tlaèítko Next
		break;	

// okno instalace
	case PAGEINSTAL:
		CreateCanc();					// vytvoøení tlaèítka Storno
		CreateIndWnd();					// vytvoøení indikátoru
		CreateLineWnd();				// vytvoøení èáry nad tlaèítky

		::SetFocus(ButtonCanc);			// fokus na tlaèítko Storno
		Install();						// provedení instalace (pøíp. pøerušení)
		break;	

// okno OK
	case PAGEOK:
		CreateExit();					// vytvoøení tlaèítka Storno
		CreateLineWnd();				// vytvoøení èáry nad tlaèítky
		CreateUserRam();				// vytvoøení uživatelského rámu

		::SetFocus(ButtonExit);			// fokus na tlaèítko Next
		break;	
	}
}


/////////////////////////////////////////////////////////////////////////////
// obsluha zpráv okna

LRESULT CALLBACK WindowProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
	case WM_CLOSE:
		::PostQuitMessage(0);
		return 0;

	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
		case 0:
		case 1:
			switch (LOWORD(wParam))
			{
			case IDN_BUTTONEXIT:
				::ShowWindow(MainFrame, SW_HIDE);
				::SendMessage(MainFrame, WM_CLOSE, 0, 0);
				return 0;

			case IDN_BUTTONNEXT:
				SetPage(PAGEINSTAL);
				return 0;

			case IDN_BUTTONLIST:
				SelectDest();
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
	}

	return ::DefWindowProc(hWnd, nMsg, wParam, lParam);
}


/////////////////////////////////////////////////////////////////////////////
// vytvoøení hlavního okna aplikace


bool MainFrameCreate()
{
// registrace tøídy okna
	WNDCLASS wcl;
	wcl.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcl.lpfnWndProc = WindowProc;
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;
	wcl.hInstance = hInstance;
	wcl.hIcon = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDN_MAINFRAME));
	wcl.hCursor = ::LoadCursor(0, MAKEINTRESOURCE(IDC_ARROW));
	wcl.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wcl.lpszMenuName = NULL;
	wcl.lpszClassName = MainFrameClass;
	::RegisterClass(&wcl);

// zjištìní požadovaných rozmìrù okna
	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = ClientWidth;
	rc.bottom = ClientHeight;
	::AdjustWindowRectEx(&rc, WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_VISIBLE, FALSE, WS_EX_APPWINDOW | WS_EX_CONTROLPARENT | WS_EX_DLGMODALFRAME);
	MainFrameWidth = rc.right - rc.left;
	MainFrameHeight = rc.bottom - rc.top;

// vytvoøení hlavního okna
	MainFrame = ::CreateWindowEx(
		WS_EX_APPWINDOW | WS_EX_CONTROLPARENT | WS_EX_DLGMODALFRAME,// rozšíøený styl
		MainFrameClass,									// jméno tøídy okna
		X(_T("Instalátor doplòkù aplikace Petr"), _T("Installator of Peter Application Supplements")),			// titulek okna
		WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,// styl okna
		(ScreenWidth - MainFrameWidth)/2,				// X
		(ScreenHeight - MainFrameHeight)/2,				// Y
		MainFrameWidth,									// šíøka
		MainFrameHeight,								// výška
		NULL,											// rodiè
		NULL,											// menu
		hInstance,										// instance
		NULL);											// parametry
	if (MainFrame == NULL) return false;

	return true;
}
