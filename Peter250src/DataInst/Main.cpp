
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

int			VerzeOS;					// verze syst�mu
OSVERSIONINFO	OSVersionInfo;			// informace o syst�mu
HINSTANCE	hInstance = NULL;			// instance programu
int			ScreenWidth;				// ���ka klientsk� oblasti displeje
int			ScreenHeight;				// v��ka klientsk� oblasti displeje

TCHAR		MainFrameClass[] = _T("PeterDataInstClass");	// n�zev t��dy hlavn�ho okna
HWND		MainFrame = NULL;			// hlavn� okno aplikace

int			MainFrameWidth = 620;		// ���ka hlavn�ho okna
int			MainFrameHeight = 420;		// v��ka hlavn�ho okna
#define		ClientWidth 608				// ���ka klientsk� oblasti (mus� to b�t n�sobek 8 a stejn� jako Picture)
#define		ClientHeight 384			// v��ka klientsk� oblasti (mus� b�t stejn� jako Picture)

int			AktPage = PAGESELECT;			// aktivn� str�nka instal�toru

__int64	DiskSize = 0;					// velikost disku (z funkce GetDiskSpace)
__int64	DiskFree = 0;					// voln� m�sto disku (z funkce GetDiskSpace)
__int64	DiskFreeUser = 0;				// voln� m�sto u�ivatele (z funkce GetDiskSpace)

int			DiskFre = 0;				// voln� m�sto c�lov�ho disku v MB
int			DiskReq = 0;				// po�adovan� voln� m�sto v MB

GETDISKFREESPACEEX	pGetDiskFreeSpaceEx = NULL;	// funkce GetDiskFreeSpaceEx (NULL=nen�)

//////////////////////////////////////////////////////////////////////////////
// jazyky

#define	JAZYKENG	0					// anglicky
#define	JAZYKCZ		1					// �esky

int		Jazyk = JAZYKENG;				// vybran� jazyk


//////////////////////////////////////////////////////////////////////////////
// licen�n� �daje

CText	InstPath;						// instala�n� cesta (bez \ na konci)
CText	Title;							// titulek


//////////////////////////////////////////////////////////////////////////////
// prvky okna instal�toru

HWND	ButtonNext = NULL;				// tla��tko dal��
HWND	ButtonExit = NULL;				// tla��tko ukon�en�
HWND	LineWnd = NULL;					// ��ra nad tla��tky
HWND	PathWndNadp = NULL;				// instala�n� cesta - nadpis
HWND	PathWnd = NULL;					// instala�n� cesta
HWND	ButtonList = NULL;				// tla��tko Proch�zet
HWND	UserRam = NULL;					// r�m u�ivatele
HWND	LicTextNum = NULL;				// licenen� text - e�slo
HWND	LicTextName = NULL;				// licenen� text - jm�no
HWND	LicTextNadp = NULL;				// licenen� text - nadpis
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

enum {
	IDN_BUTTONNEXT = 13001,				// tla��tko Dal��
	IDN_BUTTONEXIT,						// tla��tko Konec
	IDN_LINEWND,						// ��ra nad tla��tky
	IDN_PATHWNDNADP,					// instala�n� cesta - nadpis
	IDN_PATHWND,						// instala�n� cesta
	IDN_BUTTONLIST,						// tla��tko Proch�zet
	IDN_USERRAM,						// r�m u�ivatele
	IDN_LICTEXTNUM,						// licen�n� text - ��slo
	IDN_LICTEXTNAME,					// licen�n� text - jm�no
	IDN_LICTEXTNADP,					// licen�n� text - nadpis
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
};

//////////////////////////////////////////////////////////////////////////////
// p�ep�na�e instalovan�ch ��st�

CText		ExeFileName;				// jm�no tohoto programu SETUP.EXE
HANDLE		HFile = NULL;				// handle souboru SETUP.EXE
INSTHEAD	Head;						// z�hlav� instala�n�ch dat
FILETIME	datetime;					// datum a �as soubor�

int			RawData = 0;				// offset za��tku dat v souboru
int			RawSize = 0;				// velikost dat v souboru
int			DataOff = 0;				// offset za��tku dat

#define		PROGMAX		128				// rozsah indik�toru
int			DataSize = 0;				// v�choz� velikost instalovan�ch dat
int			DataSizeOK = 0;				// velikost zpracovan�ch instalovan�ch dat
BOOL		Storno = FALSE;				// po�adavek p�eru�en� operace instalace

typedef struct GROUPITEM_ {
	BYTE*	files;						// seznam soubor�
} GROUPITEM;

GROUPITEM Groups[GROUPSNUM] =
{
	NULL,
	NULL,
};

CText ProductKey(_T("Software\\Gemtree Software\\Peter"));

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
// hlavn� start programu

void WinMainCRTStartup()
{
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

// inicializace glob�ln�ch objekt�
	InitTerm(__xc_a, __xc_z);

// zaji�t�n� knihovny roz���en�ch ovl�dac�ch prvk�
	::InitCommonControls();

// up�esn�n� jazyku
	CText txt = GetReg(ProductKey, _T("SetupLang"));
	if ((txt == "5") || (txt == "21") || (txt == "27")) Jazyk = JAZYKCZ;
	if ((txt == "7") || (txt == "9") || (txt == "12")) Jazyk = JAZYKENG;

// vytvo�en� hlavn�ho okna aplikace
	if (!MainFrameCreate()) 
	{
		::MessageBox(NULL, 
			X(_T("Nedostatek syst�mov�ch prost�edk� ke spu�t�n� instal�toru."),
				_T("Insufficient system resources to run installator.")),
			X(_T("Chyba syst�mu"), _T("System Error")),
			MB_OK | MB_ICONSTOP);
		Exit();
	}

// na�ten� cesty
	InstPath = GetReg(ProductKey, _T("LastPath"));

// test, zda je prvn� instalace
	if (InstPath.IsEmpty())
	{
		InstPath = _T("C:\\Program Files\\Peter");
	}
	if (InstPath.LastChar() == _T('\\')) InstPath.Delete(InstPath.Length() - 1);

// p��prava jm�na programu
	TCHAR buf[_MAX_PATH];
	buf[0] = 0;
	::GetModuleFileName(hInstance, buf, _MAX_PATH);
	ExeFileName = buf;

#ifdef _DEBUG
	ExeFileName = "c:\\vc\\Peter\\Gener\\DataInst.exe";
	ASSERT((int)::GetFileAttributes(ExeFileName) != -1);
#endif

// test, zda nebyla chyba pam�ti
	if (MemoryError) goto MEMERROR;

// zapnut� hlavn� str�nky
	SetPage(PAGESELECT);

// na�ten� instala�n�ch informac�
	if (!OpenSetup()) Exit();

// p�ekreslen� hlavn� str�nky s nov�mi �daji
	SetPage(PAGESELECT);

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

// konec programu
	Exit();
};

//////////////////////////////////////////////////////////////////////////////
// ukon�en� programu

void Exit()
{
// ukon�en� spr�vce pam�ti
	MemTerm();

// ukon�en� programu
	ExitProcess(0);
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

BOOL ReadError()
{
	return (::MessageBox(MainFrame,
				X(
				_T(	"Chyba �ten� instala�n�ch dat,\n"
					"instala�n� program je z�ejm� po�kozen."),
				_T(	"Error reading installation data,\n"
					"installation program is maybe damaged.")),
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

// zru�en� bufferu
	MemFree(buf);
	if (RawSize < 1000) return FALSE;

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
	if (Head.Check != check) return FALSE;
	if (Head.Groups[0].Files < 2) return FALSE;

// korekce data a �asu soubor�
	datetime = Head.DateTime;
	::LocalFileTimeToFileTime(&Head.DateTime, &datetime);

// p��prava instala�n� slo�ky (pozor - sm� se prov�st pouze jednou, jinak se zm�n� InstPath !!!!)
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

// p��prava titulku
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
// zobrazen� po�adovan�ho m�sta

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
// zji�t�n� tla��tka s fokusem (-1 = nen�)

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
// nastaven� tla��tka s fokusem

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
// proveden� instalace

void Install()
{
	Storno = FALSE;
	int i,j;

// cesta k ukl�d�n� soubor�
	CText path = InstPath;
	if (path.LastChar() != _T('\\')) path += _T('\\');

// zji�t�n� velikosti soubor� (kv�li indikaci)
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

// dekompresn� buffer
	CDekomp dek;

// otev�en� vstupn�ho souboru
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

// buffery pro na��t�n� aliases
	CText AliasGroup;			// jm�no skupiny
	CText AliasKey;				// jm�no parametru
	CText AliasLine;			// jeden na�ten� ��dek textu
	CText AliasText;			// st�ada� na��tan�ho textu

// cyklus p�es v�echny skupiny
	for (i = 1; i < GROUPSNUM; i++)
	{

// na�ten� prvn�ho bloku dat skupiny
			int srcsize = Head.Groups[i].SizeGroup;						// velikost dat skupiny
			int srcsize0 = srcsize;
			if (srcsize0 > SRCBUFSIZE) srcsize0 = SRCBUFSIZE;			// velikost dat k na�ten� do bufferu
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

// pr�b�n� ukazatel �ten�ch dat
			int off0 = off + srcsize0;

// p��prava dekompresn�ho bufferu - po��te�n� dekomprese
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

// cyklus p�es v�echny soubory
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

// pro ALIASES vytvo�en� cesty a povolen� z�pisu do ALIASES
				HANDLE file = INVALID_HANDLE_VALUE;

				if (IsAli)
				{
					CreatePath(name.Left(name.RevFind(_T('\\'))));
					::SetFileAttributes(name, FILE_ATTRIBUTE_NORMAL);

					AliasGroup = _T("LANGID 9");
					AliasText.Empty();
				}
				else				

// vytvo�en� c�lov�ho souboru
				{
					file = ::CreateFile(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

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
				}

// obsluha zpr�v
				PumpMessage();

// chyba - soubor nelze vytvo�it
				if (!IsAli && (file == INVALID_HANDLE_VALUE))
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
								"\n   - full destination disk or write protection on disk"))),
						X(_T("Chyba z�pisu"), _T("Write Error")),
						MB_OK | MB_ICONERROR);

					MemFree(srcbuf);
					MemFree(dstbuf);
					SetPage(PAGESELECT);
					return;
				}

// cyklus ulo�en� v�ech dat
				while (size > 0)
				{

// velikost bloku dat p�ipraven�ch v bufferu
					int size0 = dek.Dst() - dek.DstBeg();
					if (size0 <= 0)
					{

// na�ten� dal��ho bloku zdrojo�ch dat
						if (srcsize <= 0)		// aby bylo je�t� n�co na�teno za koncem dat
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

// posun ukazatel� a dat v bufferu
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

// z�pis ALIASES
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

// z�pis bloku dat
					else
					{
						if (!::WriteFile(file, dek.DstBeg(), size0, &readwrite, NULL) || (size0 != (int)readwrite))
						{
							::CloseHandle(file);
							::DeleteFile(name);

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
					}

					BYTE* d = dek.DstBeg();
					dek.DstBeg(dek.DstBeg() + size0);

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
				if (!IsAli)
				{
					::SetFileTime(file, NULL, NULL, &datetime);

// uzav�en� c�lov�ho souboru
					::CloseHandle(file);
				}

// kontrola kontroln�ho sou�tu souboru
				if (check != check0)
				{
					::DeleteFile(name);

					::MessageBox(MainFrame,
						X(
						CText(
							_T("Chyba instalace souboru\n\n") +
							name +
							_T("\n\nInstala�n� program je po�kozen."
								"\nInstalace nem��e d�le pokra�ovat a bude p�eru�ena.")),
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

// obsluha zpr�v
				PumpMessage();

// p�eru�en� instalace
				if (Storno)
				{
					if (::MessageBox(MainFrame,
							X(
							CText(
								_T("P�eru�en�m instalace z�stanou ve slo�ce ") +
								path +
								_T("\nji� nainstalovan� soubory. Soubory m��ete zru�it Pr�zkumn�kem."
									"\n\nOpravdu chcete instalaci p�eru�it?")),
							CText(
								_T("There will remain already installed files in folder ") +
								path +
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

// posun ukazatele dat
		off += Head.Groups[i].SizeGroup;
	}

// zru�en� vypisovan�ho textu souboru
	EmptyText.SetWindowText(IndWndFile);

// zru�en� bufferu
	MemFree(srcbuf);
	MemFree(dstbuf);

// instalace ukon�ena OK
	SetPage(PAGEOK);
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
	if (AktPage == PAGEOK)
	{
		if (LicTextName == NULL)
		{
			LicTextName = ::CreateWindowEx(
				0,												// roz���en� styl
				_T("STATIC"),									// jm�no t��dy okna
				X(_T("Instalace byla �sp�n� dokon�ena."), _T("Installation was successfuly completed.")),
				SS_CENTER | WS_VISIBLE | WS_CHILD,				// styl okna
				10,												// X
				160,											// Y
				ClientWidth - 20,								// ���ka
				20,												// v��ka
				MainFrame,										// rodi�
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
				0,												// roz���en� styl
				_T("STATIC"),									// jm�no t��dy okna
				X(_T("Stiskem tla��tka 'Dokon�it' zah�j�te instalaci."), _T("You can start installation by pressing the pushbutton 'Finish'.")),
				SS_CENTER | WS_VISIBLE | WS_CHILD,				// styl okna
				10,												// X
				35,												// Y
				ClientWidth - 20,								// ���ka
				20,												// v��ka
				MainFrame,										// rodi�
				(HMENU)IDN_LICTEXTNAME,							// menu
				hInstance,										// instance
				NULL);											// parametry
		}

		if (UserRam == NULL)
		{
			UserRam = ::CreateWindowEx(
				0,												// roz���en� styl
				_T("STATIC"),									// jm�no t��dy okna
				NULL,											// titulek okna
				SS_ETCHEDFRAME | WS_VISIBLE | WS_CHILD,			// styl okna
				10,												// X
				80,												// Y
				ClientWidth - 20,								// ���ka
				100,											// v��ka
				MainFrame,										// rodi�
				(HMENU)IDN_USERRAM,								// menu
				hInstance,										// instance
				NULL);											// parametry
		}

		if (LicTextNadp == NULL)
		{
			LicTextNadp = ::CreateWindowEx(
				0,												// roz���en� styl
				_T("STATIC"),									// jm�no t��dy okna
				X(_T("Instalovan� dopln�k:"), _T("Installed supplement:")),
				SS_LEFT | WS_VISIBLE | WS_CHILD,				// styl okna
				20,												// X
				85,												// Y
				ClientWidth - 40,								// ���ka
				20,												// v��ka
				MainFrame,										// rodi�
				(HMENU)IDN_LICTEXTNADP,							// menu
				hInstance,										// instance
				NULL);											// parametry
			::SendMessage(LicTextNadp, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);
		}

		if (LicTextNum == NULL)
		{
			LicTextNum = ::CreateWindowEx(
				0,												// roz���en� styl
				_T("STATIC"),									// jm�no t��dy okna
				Title,
				SS_CENTER | WS_VISIBLE | WS_CHILD,				// styl okna
				20,												// X
				123,											// Y
				ClientWidth - 40,								// ���ka
				20,												// v��ka
				MainFrame,										// rodi�
				(HMENU)IDN_LICTEXTNUM,							// menu
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
			X(_T("C�lov� slo�ka:"),	_T("Destination folder:")),	// titulek okna
			SS_LEFT | WS_VISIBLE | WS_CHILD,				// styl okna
			10,												// X
			ClientHeight - 115,								// Y
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
// vytvo�en� tla��tka Dal��

void CreateNext()
{
	if (ButtonNext == NULL)
	{
		ButtonNext = CreateDefButton(IDN_BUTTONNEXT, ClientWidth/2 - 100 - 30, X(_T("Dokon�it"), _T("Finish")));
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� tla��tka Konec

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
		IndWndNadp = ::CreateWindowEx(
			0,												// roz���en� styl
			_T("STATIC"),									// jm�no t��dy okna
			X(_T("Prob�h� instalace dopl�k� aplikace Petr ..."), _T("Proceeding installation of Peter application supplements ...")),		// titulek okna
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

		for (int i = 1; i < GROUPSNUM; i++)
		{
			DiskReq += (Head.Groups[i].Size + 1023)/1024;
		}
		
		CText txt;
		txt.Int(DiskReq);
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
			230,											// Y
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
			230,											// Y
			70,												// ���ka
			18,												// v��ka
			MainFrame,										// rodi�
			(HMENU)IDN_DISKFREENUM,							// menu
			hInstance,										// instance
			NULL);											// parametry
		::SendMessage(DiskFreeNum, WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), TRUE);
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
	ofn.lpstrTitle = X(_T("C�lov� slo�ka k instalaci dopl�ku aplikace Petr"), _T("Destination Folder to Install Peter Application Supplement"));
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
		name.Delete(name.Length() - 1);
	}

// ulo�en� jm�na souboru
	InstPath = name;
	name.SetWindowText(PathWnd);

// aktualizace voln�ho m�sta na c�lov�m disku
	GetDiskSpace();

// aktualizace zobrazen� okna
	::InvalidateRect(MainFrame, NULL, TRUE);
	::UpdateWindow(MainFrame);

	SetButton(IDN_BUTTONLIST);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� str�nky instal�toru

void SetPage(int page)
{

// nov� aktivn� str�nka
	AktPage = page;

// zru�en� v�ech prvk�
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

// vytvo�en� nov�ho okna
	switch (page)
	{

// okno v�b�ru slo�ek	
	case PAGESELECT:
		GetDiskSpace();					// na�ten� informac� o c�lov�m disku
		CreateNext();					// vytvo�en� tla��tka Dal��
		CreateExit();					// vytvo�en� tla��tka Konec
		CreateList();					// vytvo�en� tla��tka Proch�zet
		CreateLineWnd();				// vytvo�en� ��ry nad tla��tky
		CreatePathWnd();				// vytvo�en� textu cesty
		CreateUserRam();				// vytvo�en� u�ivatelsk�ho r�mu
		CreateDiskFree();				// zobrazen� informac� o disku

		::SetFocus(ButtonNext);			// fokus na tla��tko Next
		break;	

// okno instalace
	case PAGEINSTAL:
		CreateCanc();					// vytvo�en� tla��tka Storno
		CreateIndWnd();					// vytvo�en� indik�toru
		CreateLineWnd();				// vytvo�en� ��ry nad tla��tky

		::SetFocus(ButtonCanc);			// fokus na tla��tko Storno
		Install();						// proveden� instalace (p��p. p�eru�en�)
		break;	

// okno OK
	case PAGEOK:
		CreateExit();					// vytvo�en� tla��tka Storno
		CreateLineWnd();				// vytvo�en� ��ry nad tla��tky
		CreateUserRam();				// vytvo�en� u�ivatelsk�ho r�mu

		::SetFocus(ButtonExit);			// fokus na tla��tko Next
		break;	
	}
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
	wcl.hCursor = ::LoadCursor(0, MAKEINTRESOURCE(IDC_ARROW));
	wcl.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
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
		X(_T("Instal�tor dopl�k� aplikace Petr"), _T("Installator of Peter Application Supplements")),			// titulek okna
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
