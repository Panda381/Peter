
#include "Main.h"

/***************************************************************************\
*																			*
*							V�b�r souboru k editaci							*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// seznam polo�ek

// polo�ka seznamu
typedef struct SELECTITEM_ 
{
	CIcon		Icon;								// ikona
	CMultiText	Name;								// jm�no polo�ky
	CText		NameLoad;							// pln� specifikace polo�ky pro �ten�
	CText		NameSave;							// pln� specifikace polo�ky pro z�pis
	int			Typ;								// typ polo�ky
	BOOL		Altern;								// je alternativn� (vzorov�) adres��
													//	- u program� m� p�ednost lok�ln� program (kv�li spou�t�n� modifikac�)
													//	- u adres��� m� p�ednost vzorov� adres�� (z�kaz p�ejmenov�n�)
	int			Index;								// index polo�ky v seznamu (-1 = neplatn�)
} SELECTITEM;

SELECTITEM*		SelectItem = NULL;					// buffer polo�ek seznamu
int				SelectItemMax = 0;					// velikost bufferu polo�ek seznamu
int				SelectItemNum = 0;					// po�et polo�ek v seznamu

// typy polo�ek (ur�uje t��d�n� polo�ek !!!!)
#define TYP0 0										// nen� platn� polo�ka
#define	TYPUPDIR 1									// polo�ka je nadadres�� ".."
#define TYPSUBDIR 2									// polo�a je podadres��
#define TYPFILE 3									// polo�ka je soubor

int				SelectInx = -1;						// index aktivn� vybran� polo�ky (-1 = nen�)
int				SelectIndex = -1;					// index aktivn� polo�ky seznamu (-1 = neplatn�)
int				SelectTyp = TYP0;					// typ vybran� polo�ky (TYP0 = nen� ��dn�)
BOOL			SelectAltern = FALSE;				// aktivn� polo�ka je v alternativn�m (vzorov�m) adres��i

BOOL			SelectFill = FALSE;					// p��znak na��t�n� obsahu okna

CBufIcon	SelectIcon0;							// vzor ikon adres���

CText		SelectAliases;							// jm�no souboru ALIASES.INI
CText		SelectAliases2;							// jm�no souboru ALIASES.INI ve vzorov�m adres��i

/////////////////////////////////////////////////////////////////////////////
// glob�ln� a lok�ln� prom�nn�

BOOL		SelectMode = FALSE;						// p��znak m�du v�b�ru soubor�
HWND		SelectView = NULL;						// seznam soubor�
HIMAGELIST	SelectImageList = NULL;					// seznam obr�zk�

HFONT		SelFont = NULL;							// nastaven� font v okn�

int			SelectStatusWidth = 400;				// ���ka druh�ho pole stavov�ho ��dku

CText		SelectNewText;							// zadan� jm�no nov�ho programu
CMultiText	SelectNewMultiText;						// zadan� jm�no v�cejazy�n�

BOOL		SelectNewFill = FALSE;					// pln� se editor jm�na nov�ho programu
HWND		SelectNewEdit = NULL;					// handle editoru jm�na nov�ho programu

BOOL		SelEditName = FALSE;					// p��znak prob�haj�c� editace
HWND		SelEditNameWnd = NULL;					// edita�n� okno

BOOL		IsWaitCursor = FALSE;					// zapnut �ekac� kurzor

#define		ICONSUBDIR	0							// index ikony podadres��e
#define		ICONUPDIR	1							// index ikony nadadres��e

/////////////////////////////////////////////////////////////////////////////
// hl�en� o zm�n� adres��e

HANDLE		SelectDirChange = INVALID_HANDLE_VALUE;	// hl�en� o zm�n� adres��e
HANDLE		SelectDirChange2 = INVALID_HANDLE_VALUE;// hl�en� o zm�n� adres��e 2
#define		SelectDirChangeTimerID 19112			// ID �asova�e hl�en� zm�ny adres��e
UINT		SelectDirChangeTimer = 0;				// �asova� pro hl�en� zm�n adres��e


/////////////////////////////////////////////////////////////////////////////
// opo�d�n� zapnut� editace

#define		SelectTimerID 19125						// ID �asova�e pro zapnut� editace
UINT		SelectTimer = 0;						// �asova� pro zapnut� editace
int			SelectTimerN = 0;						// ��ta� pro zapnut� editace


/////////////////////////////////////////////////////////////////////////////
// definice tla��tek pro toolbar Select

TBBUTTON ToolBarSelect[] = {
	{ ButtonStart,	IDN_START,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{ ButtonOpen,	IDN_OPEN,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{ 0,			0,				TBSTATE_ENABLED, TBSTYLE_SEP,		0, 0},
	{ ButtonNew,	IDN_NEW,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{ ButtonDirNew,	IDN_PATH,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{ ButtonCopy,	IDN_NEWCOPY,	TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{ ButtonEditName,IDN_RENAME,	TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{ ButtonClear,	IDN_DELETE,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
};

#define ToolBarSelectNum (sizeof(ToolBarSelect)/sizeof(TBBUTTON))

void SelectLoad();				// na�ten� seznamu soubor�

/////////////////////////////////////////////////////////////////////////////
// definice menu

MENUITEM* MenuSelect[] =
{
	MenuSoubor1,
	MenuHlp,
	NULL
};

/////////////////////////////////////////////////////////////////////////////
// z�hlav� nov�ho programu

const struct { 
	char ident[3];			// (3) identifikace = "PET"
	BYTE verze;				// (1) verze = 1
	WORD Editor;			// (2) verze editoru v tis�cin�ch
	WORD Param;				// (2) parametry = 0
	long data;				// (4) offset za��tku dat od za��tku z�hlav� (= d�lka z�hlav�)
	long pocet;				// (4) po�et datov�ch blok�
} NewHead =
{
	'P', 'E', 'T',			// (3) identifikace = "PET"
	1,						// (1) verze = 1
	VerzeCom + VerzeRel*10 + VerzeMin*100 + VerzeMaj*1000, // verze editoru
	0,						// (4) parametry = 0
	3 + 1 + 4 + 4 + 4,		// (4) offset za��tku dat od za��tku z�hlav� (= d�lka z�hlav�)
	0,						// (4) po�et datov�ch blok�
};

/////////////////////////////////////////////////////////////////////////////
//	zru�en� buffer polo�ek

void SelectItemDelAll()
{
	for (int i = 0; i < SelectItemNum; i++)
	{
		SelectItem[i].Icon.Term();
		SelectItem[i].Name.Term();
	}

	MemFree(SelectItem);
	SelectItem = NULL;
	SelectItemMax = 0;
	SelectItemNum = 0;
	SelectInx = -1;
	SelectIndex = -1;
	SelectTyp = TYP0;
	SelectAltern = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// �schova parametr� vybran� polo�ky (-1 = ��dn�)

void SelectSelect(int inx)
{
	if ((DWORD)inx >= (DWORD)SelectItemNum)
	{
		SelectInx = -1;
		SelectIndex = -1;
		SelectTyp = TYP0;
		SelectAltern = FALSE;
		Jmeno.Empty();
		JmenoLoad.Empty();
		JmenoSave.Empty();
	}
	else
	{
		SELECTITEM* item = SelectItem + inx;
		SelectInx = inx;
		SelectIndex = item->Index;
		SelectTyp = item->Typ;
		SelectAltern = item->Altern;
		Jmeno = item->Name;
		JmenoLoad = item->NameLoad;
		JmenoSave = item->NameSave;
	}
}

/////////////////////////////////////////////////////////////////////////////
// p�id�n� polo�ky do bufferu polo�ek (vrac� index polo�ky, p�i chyb� pam�ti vrac� -1)

int SelectItemAdd(CIcon& icon, CMultiText& name, int typ, BOOL altern)
{
	SELECTITEM* item;
	int i = SelectItemNum;

	if (i >= SelectItemMax)
	{
		item = (SELECTITEM*)MemSize(SelectItem, (SelectItemMax + 64)*sizeof(SELECTITEM));
		if (item == NULL) return -1;
		SelectItem = item;
		SelectItemMax += 64;
	}

	item = SelectItem + i;

	item->Icon.Init(icon);
	item->Name.Init(name);
	item->Typ = typ;
	item->Altern = altern;
	item->Index = i;

	item->NameSave.Init(ProgramPath + Cesta + name.MultiText((int)JAZYK000));
	if (altern)
	{
		item->NameLoad.Init(ProgramPath2 + Cesta + name.MultiText((int)JAZYK000));
	}
	else
	{
		item->NameLoad.Init(item->NameSave);
	}

	SelectItemNum = i + 1;

	return i;
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� menu a font� programu

void SelectSetMenu()
{
// nastaven� menu
	SetNewMenu(MenuSelect);

// nastaven� fontu okna
	HFONT oldfont = SelFont;
	SelFont = GetFont(&SelectFont);
	::SendMessage(SelectView, WM_SETFONT, (WPARAM)SelFont, TRUE);
	DelFont(oldfont);

// aktualizace stavov�ho ��dku
	SetStatusFont();
	UpdateStartOpen();
}


/////////////////////////////////////////////////////////////////////////////
// prvn� inicializace p�i startu programu (hlavn� okno je ji� vytvo�eno)

void SelectStartInit()
{
}


/////////////////////////////////////////////////////////////////////////////
// zah�jen� re�imu v�b�ru souboru k editaci

void SelectInit()
{
// kontrola, zda nen� re�im v�b�ru souboru
	if (SelectMode) return;			// je ji� re�im v�b�ru soubor�
	SelectMode = TRUE;
	SelEditName = FALSE;			// nen� editace jm�na souboru

// nastaven� tla��tek pro re�im v�b�ru
	ToolBarAdd(ToolBarSelect, ToolBarSelectNum); // p�id�n� tla��tek

// posun panelu n�stroj�
	ToolBarResize();

// vytvo�en� seznamu soubor�
	SelectView = ::CreateWindowEx(
		WS_EX_CLIENTEDGE,
		WC_LISTVIEW,
		NULL,
		WS_VISIBLE | WS_CHILD | 
		LVS_ALIGNTOP | LVS_ICON | LVS_SINGLESEL | LVS_SHAREIMAGELISTS 
				| LVS_SHOWSELALWAYS | LVS_EDITLABELS,
		ClientRect.left,
		ClientRect.top,
		ClientWidth,
		ClientHeight,
		MainFrame,
		(HMENU)IDN_SELECT1,
		hInstance,
		NULL);
	ASSERT(SelectView);

// nastaven� nov�ho menu a font�
	SelectSetMenu();

// vytvo�en� seznamu obr�zk� a p�ipojen� k seznamu
	SelectImageList = ::ImageList_Create(32, 32, ILC_COLORDDB | ILC_MASK, 1, 1);
	ASSERT(SelectImageList);
	::SendMessage(SelectView, LVM_SETIMAGELIST, LVSIL_NORMAL, (LPARAM)SelectImageList);

// aktualizace rozm�r� okna
	SelectOnSize();

// p��prava vzorov�ch ikon
	SelectIcon0.DelAll();
	SelectIcon0.Load(IDN_SELECT, 2);
	ASSERT(SelectIcon0.Num() == 2);

// na�ten� seznamu soubor�
	SelectLoad();

// nastaven� fokusu na aktivni okno
	SelectSetFocus();
}


/////////////////////////////////////////////////////////////////////////////
// opo�d�n� zah�jen� editace

void SelectEdit()
{
	if (!IsWaitCursor)
	{
		IsWaitCursor = TRUE;
		BeginWaitCursor();
	}

	SelectTimerN = 2;
	if (SelectTimer == 0)
	{
		SelectTimer = ::SetTimer(MainFrame, SelectTimerID, TimerConst, NULL);
	}
}


/////////////////////////////////////////////////////////////////////////////
// ukon�en� re�imu v�b�ru souboru

void SelectTerm()
{
// kontrola, zda je re�im v�b�ru
	if (!SelectMode) return;
	SelectMode = FALSE;

// vypnut� automatick� editace
	SelectTimerN = 0;
	if (SelectTimer)
	{
		::KillTimer(MainFrame, SelectTimer);
		SelectTimer = 0;
	}

// zru�en� okna pro v�b�r soubor�
	::DestroyWindow(SelectView);

// zru�en� seznamu obr�zk�
	::ImageList_Destroy(SelectImageList);
	SelectImageList = NULL;

// zru�en� hl�en� o zm�n� adres��e
	if (SelectDirChangeTimer)
	{
		::KillTimer(MainFrame, SelectDirChangeTimer);
		SelectDirChangeTimer = 0;
	}

	if (SelectDirChange != INVALID_HANDLE_VALUE)
	{
		::FindCloseChangeNotification(SelectDirChange);
		SelectDirChange = INVALID_HANDLE_VALUE;
	}

	if (SelectDirChange2 != INVALID_HANDLE_VALUE)
	{
		::FindCloseChangeNotification(SelectDirChange2);
		SelectDirChange2 = INVALID_HANDLE_VALUE;
	}

// zru�en� v�ech tla��tek v panelu n�stroj�
	ToolBarClear(0);

	SetStatusText2(EmptyText);
}


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� informace o ikon� k zobrazen�

void SelectOnGetDispInfo(LV_DISPINFO* lvd)
{
// kontrola, zda je re�im v�b�ru
	if (!SelectMode) return;

// kontrola, zda je vytvo�en platn� seznam obr�zk�
	if (SelectImageList == NULL) return;

// na�ten� ikony
	Icon1.DelAll();

	int inx = lvd->item.lParam;
	if ((DWORD)inx < (DWORD)SelectItemNum)
	{
		Icon1.Add(SelectItem[inx].Icon);
	}
	else
	{
		Icon1.Add(SelectIcon0[0]);
	}

// vygenerov�n� nov�ho seznamu ikon
	::ImageList_Remove(SelectImageList, -1);
	Icon1.GenerList(FALSE, SI_BIG, SelectImageList);

// nastaven� ikony k zobrazen�
	lvd->item.iImage = 0;
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� fokusu na okno v�b�ru

void SelectSetFocus()
{
// kontrola, zda je re�im v�b�ru
	if (!SelectMode) return;

// nastaven� fokusu na okno
	::SetFocus(SelectView);
}


/////////////////////////////////////////////////////////////////////////////
// zm�na velikosti okna (WM_SIZE)

void SelectOnSize()
{
// kontrola, zda je re�im v�b�ru
	if (!SelectMode) return;

// posun panelu n�stroj�
	ToolBarResize();

// zah�jen� p�esouv�n� oken
	HDWP hdwp = ::BeginDeferWindowPos(10);

// na�ten� v��ky stavov� li�ty (i kdy� to z�ejm� nen� pot�eba)
	RECT rcStat;
	::GetWindowRect(StatusBar, &rcStat);

// posun stavov�ho okna
	hdwp = ::DeferWindowPos(hdwp, StatusBar, NULL, ClientRect.left, ClientRect.bottom, ClientWidth, rcStat.bottom - rcStat.top,
			SWP_DRAWFRAME | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER);

// inicializace klientsk� oblasti
	InitClientRect();

// aktualizace p�ed�l� stavov�ho okna
	SetStatusWidth(SelectStatusWidth);

// zm�na velikosti okna v�b�ru
	hdwp = ::DeferWindowPos(hdwp, SelectView, NULL, ClientRect.left, ClientRect.top, ClientWidth, ClientHeight,
			SWP_DRAWFRAME | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER);

// konec p�esouv�n� oken
	::EndDeferWindowPos(hdwp);

// p�earan�ov�n� ikon v seznamu
	::SendMessage(SelectView, LVM_ARRANGE, LVA_ALIGNTOP, 0);

// aktualizace stavov�ho ��dku
	UpdateStartOpen();

// p�ekreslen� okna
	::UpdateWindow(MainFrame);
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� ALIAS jm�na souboru

CText ReadAliasName(int lang, LPCTSTR key, LPCTSTR aliases)
{
	TCHAR buf[1024];
	buf[0] = 0;
	return CText(buf, ::GetPrivateProfileString(JazykInfo[lang].LangIDini, key, EmptyText, buf, 1023, aliases));
}

/////////////////////////////////////////////////////////////////////////////
// porovn�n� dvou polo�ek seznamu

int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	if (((DWORD)lParam1 >= (DWORD)SelectItemNum) || 
			((DWORD)lParam2 >= (DWORD)SelectItemNum))
	{
		return 0;
	}

	SELECTITEM* item1 = SelectItem + (int)lParam1;
	SELECTITEM* item2 = SelectItem + (int)lParam2;

	if (item1->Typ < item2->Typ) return -1;
	if (item1->Typ > item2->Typ) return 1;

	if (item1->Name.MultiText() < item2->Name.MultiText()) return -1;
	if (item1->Name.MultiText() > item2->Name.MultiText()) return 1;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// na�ten� seznamu soubor�

void SelectLoad()
{
// p��znak pln�n� okna
	SelectFill = TRUE;

// inicializace seznamu
	SelectItemDelAll();

// nen� vybran� polo�ka
	SelectInx = -1;
	SelectIndex = -1;
	SelectTyp = TYP0;
	SelectAltern = FALSE;

// normalizace cesty - o�ez�n� mezer a "\" ze za��tku a konce
	Cesta.TrimLeft();
	Cesta.TrimRight();
	while (Cesta.Get(0) == _T('\\'))
	{
		Cesta.Delete(0,1);
		Cesta.TrimLeft();
	}
	while (Cesta.LastChar() == _T('\\'))
	{
		Cesta.Delete(Cesta.Length()-1);
		Cesta.TrimRight();
	}

// vytvo�en� adres��e pro programy
	CreatePath(ProgramPath);
	CreatePath(ProgramPath + Cesta);

// zji�t�n� platnosti cesty
	BOOL ok = FALSE;
	int attrib;
	while (!ok && Cesta.IsNotEmpty())
	{
		attrib = (int)::GetFileAttributes(ProgramPath + Cesta);
		if ((attrib != -1) && ((attrib & FILE_ATTRIBUTE_DIRECTORY) != 0))
		{
			ok = TRUE;
		}
		else
		{
			Cesta.Delete(Cesta.RevFind(_T('\\')));
			Cesta.TrimRight();
		}
	}

// zaji�t�n� znaku "\" na konci cesty
	if (Cesta.IsNotEmpty()) Cesta.Add(_T('\\'));

// cesta do adres��e, p��znak alternativn�ho adres��e
	CText path = ProgramPath + Cesta;
	BOOL altern = FALSE;

// p��prava soubor� ALIASES
	SelectAliases = path + _T("ALIASES.INI");
	SelectAliases2 = ProgramPath2 + Cesta + _T("ALIASES.INI");

// vypnut� p�ekreslov�n� okna
	::SendMessage(SelectView, WM_SETREDRAW, FALSE, 0);

// zru�en� v�ech polo�ek v seznamu
	::SendMessage(SelectView, LVM_DELETEALLITEMS, 0, 0);

// prom�nn� pro na�ten� seznamu
	WIN32_FIND_DATA wfd;			// struktura pro hled�n� soubor�
	HANDLE handle;					// handle hled�n� soubor�
	CText jmeno;					// jm�no nalezen�ho souboru
	LV_ITEM lvi;					// vkl�dan� polo�ka do seznamu
	CIcon icon;						// ikona
	CFileMap file;					// pam�ov� mapovan� soubor
	CMultiText name;				// jm�no polo�ky
	CText aliases(SelectAliases);	// jm�no souboru aliases
	BOOL aliases2 = (ProgramPath2.IsNotEmpty() && ((int)::GetFileAttributes(SelectAliases2) != -1)); // povoleno na�ten� aliases 2

// p��prava polo�ky seznamu
	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	lvi.iSubItem = 0;
	lvi.iImage = I_IMAGECALLBACK;

// dopln�n� polo�ky "..", je-li podadres��
	if (Cesta.IsNotEmpty())
	{
		name.MultiText(JAZYK000, CText(_T("..")));
		lvi.lParam = SelectItemAdd(SelectIcon0[ICONUPDIR], name, TYPUPDIR, false);
		lvi.iItem = lvi.lParam;
		lvi.pszText = (LPTSTR)(LPCTSTR)(name.MultiText((int)JAZYK000));
		::SendMessage(SelectView, LVM_INSERTITEM, 0, (LPARAM)&lvi);
	}

// zde bude druh� pokus - pro vzorov� adres��
DRUHYPOKUS:

// na�ten� v�ech podadres���
	handle = ::FindFirstFile(path + _T("*.*"), &wfd);
	if (handle != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) &&
				((wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == 0))
			{
				jmeno = wfd.cFileName;

				if ((jmeno != _T(".")) && (jmeno != _T("..")))
				{
					name.Empty();
					name.MultiText(JAZYK000, jmeno);

// na�ten� aliases jm�na adres��e
					BOOL nameok = FALSE;

					for (int lang = 0; lang < JAZYKNUM; lang++)
					{
						if (lang != JAZYK000)
						{
							name.MultiText(lang, ReadAliasName(lang, jmeno, aliases));
							if (name.MultiText(lang).IsNotEmpty()) nameok = TRUE;
						}
					}

					if (!nameok && aliases2)
					{
						for (lang = 0; lang < JAZYKNUM; lang++)
						{
							if (lang != JAZYK000)
							{
								name.MultiText(lang, ReadAliasName(lang, jmeno, SelectAliases2));
							}
						}
					}

// test duplikace polo�ky ze vzorov�ho adres��e
					BOOL duplik = FALSE;
					SELECTITEM* item = SelectItem;
					int i = 0;

					if (altern)
					{
						CText name0 = name.MultiText((int)JAZYK000);

						for (; i < SelectItemNum; i++)
						{
							if (!item->Altern && (item->Typ == TYPSUBDIR) && (item->Name.MultiText((int)JAZYK000) == name0))
							{
								duplik = TRUE;
								break;
							}
							item++;
						}		   
					}

// p�id�n� polo�ky do seznamu
					if (!duplik)
					{
						i = SelectItemAdd(SelectIcon0[ICONSUBDIR], name, TYPSUBDIR, altern);
						lvi.lParam = i;
						lvi.iItem = i;
						lvi.pszText = (LPTSTR)(LPCTSTR)(name.MultiText());
						::SendMessage(SelectView, LVM_INSERTITEM, 0, (LPARAM)&lvi);
					}
					else
					{
						item->Name = name;
						item->NameLoad = ProgramPath2 + Cesta + name.MultiText((int)JAZYK000);
						item->Altern = TRUE;
						lvi.lParam = i;
						lvi.iItem = i;
						lvi.pszText = (LPTSTR)(LPCTSTR)(name.MultiText());
						::SendMessage(SelectView, LVM_SETITEM, 0, (LPARAM)&lvi);
					}
				}
			}
			
		} while (::FindNextFile(handle, &wfd));
		::FindClose(handle);
	}

// vyhled�n� soubor�
	handle = ::FindFirstFile(path + _T("*.exe"), &wfd);
	if (handle != INVALID_HANDLE_VALUE)
	{
		do
		{

// otev�en� souboru
			jmeno = wfd.cFileName;

			if (file.Open(path + jmeno))
			{

// p��prava NT z�hlav� souboru EXE
				IMAGE_NT_HEADERS* hdr = file.NTHeader();
				if (hdr != NULL)
				{

// nalezen� NT sekce Pet��ka
					IMAGE_SECTION_HEADER* sec = file.NTSection(hdr, ".petprg");
					if (sec != NULL)
					{

// adresa z�hlav� Pet��ka
						PETHEAD* buf = (PETHEAD*)(file.Adr() + sec->PointerToRawData);
						if (file.IsValid(buf, sizeof(NewHead)))
						{

// kontrola z�hlav� Pet��ka
							if ((buf->Ident[0] == 'P') &&
								(buf->Ident[1] == 'E') &&
								(buf->Ident[2] == 'T') &&
								(buf->Verze == 1) &&
								(buf->Pocet < 20))
							{

// uzav�en� souboru (aby se uvolnil pro na�ten� ikony)
								file.Close();

// na�ten� ikony ze souboru
								if (icon.Extract(path + jmeno))
								{

// �schova jm�na programu
									name.Empty();
									name.MultiText(JAZYK000, jmeno);

									BOOL nameok = FALSE;
// na�ten� aliases
									for (int lang = 0; lang < JAZYKNUM; lang++)
									{
										if (lang != JAZYK000)
										{
											name.MultiText(lang, ReadAliasName(lang, jmeno, aliases));
											if (name.MultiText(lang).IsNotEmpty()) nameok = TRUE;
										}
									}

									if (!nameok && aliases2)
									{
										for (lang = 0; lang < JAZYKNUM; lang++)
										{
											if (lang != JAZYK000)
											{
												name.MultiText(lang, ReadAliasName(lang, jmeno, SelectAliases2));
												if (name.MultiText(lang).IsNotEmpty()) nameok = TRUE;
											}
										}
									}

// zru�en� p��pony jm�na souboru
									if ((jmeno.Length() > 4) && (jmeno[jmeno.Length() - 4] == _T('.')))
									{
										jmeno.Delete(jmeno.Length()-4);
									}

									if (!nameok) name.MultiText(jmeno);

// test duplikace polo�ky (vzorov� adres��)
									BOOL duplik = FALSE;

									if (altern)
									{
										CText name0 = name.MultiText((int)JAZYK000);
										SELECTITEM* item = SelectItem;

										for (int i = 0; i < SelectItemNum; i++)
										{
											if (!item->Altern && (item->Typ == TYPFILE) && (item->Name.MultiText((int)JAZYK000) == name0))
											{
												duplik = TRUE;
												break;
											}
											item++;
										}		   
									}

// p�id�n� polo�ky do seznamu
									if (!duplik)
									{
										int i = SelectItemAdd(icon, name, TYPFILE, altern);
										lvi.lParam = i;
										lvi.iItem = i;
										lvi.pszText = (LPTSTR)(LPCTSTR)(name.MultiText());
										::SendMessage(SelectView, LVM_INSERTITEM, 0, (LPARAM)&lvi);
									}
								}
								continue;
							}
						}
					}
				}

// uzav�en� souboru
				file.Close();
			}

// nalezen� dal��ho souboru
		} while (::FindNextFile(handle, &wfd));
		::FindClose(handle);
	}

// prohled�n� vzorov�ho adres��e
	if (!altern && ProgramPath2.IsNotEmpty())
	{
		path = ProgramPath2 + Cesta;
		aliases = SelectAliases2;
		altern = TRUE;
		aliases2 = FALSE;
		goto DRUHYPOKUS;
	}

// p�ednastaven� na prvn� jm�no, nen�-li ��dn� jm�no p�ednastaveno
	if (Jmeno.MultiText((int)JAZYK000).IsEmpty())
	{
		SelectSelect(0);
	}
	else
	{

// nalezen� polo�ky se zadan�m jm�nem
		int i;
		for (i = 0; i < SelectItemNum; i++)
		{
			if (Jmeno.MultiText((int)JAZYK000) == SelectItem[i].Name.MultiText((int)JAZYK000)) break;
		}

// p�i ne�sp�chu se pou�ije prvn� jm�no v seznamu
		if (i >= SelectItemNum) i = 0;

// nastaven� indexu a jm�na polo�ky
		SelectSelect(i);
	}

// nastaven� kurzoru polo�ky
	if ((DWORD)SelectInx < (DWORD)SelectItemNum)
	{
		lvi.stateMask = LVIS_FOCUSED | LVIS_SELECTED;
		lvi.state = LVIS_FOCUSED | LVIS_SELECTED;
		lvi.mask = LVIF_STATE;
		lvi.iSubItem = 0;
		lvi.iItem = SelectInx;
		::SendMessage(SelectView, LVM_SETITEMSTATE, SelectInx, (LPARAM) &lvi);
	}

// set��d�n� polo�ek
	::SendMessage(SelectView, LVM_SORTITEMS, 0, (LPARAM)(PFNLVCOMPARE)CompareFunc);

// zji�t�n� nov�ch index� polo�ek
	for (int i = 0; i < SelectItemNum; i++)
	{
		SelectItem[i].Index = -1;
	}

	ASSERT(SelectItemNum == ::SendMessage(SelectView, LVM_GETITEMCOUNT, 0, 0));

	for (i = 0; i < SelectItemNum; i++)
	{
		lvi.mask = LVIF_PARAM;
		lvi.iSubItem = 0;
		lvi.iItem = i;
		lvi.lParam = -1;
		::SendMessage(SelectView, LVM_GETITEM, 0, (LPARAM)&lvi);
		if ((DWORD)lvi.lParam < (DWORD)SelectItemNum)
		{
			SelectItem[lvi.lParam].Index = i;
		}
	}

// zapnut� p�ekreslov�n� okna
	::SendMessage(SelectView, WM_SETREDRAW, TRUE, 0);

// aktualizace zobrazen� jm�na v titulku okna
	ZobrazJmeno();

// aktualizace tla��tek a polo�ek menu
	UpdateStartOpen();

// zru�en� hl�en� o zm�n� adres��e
	if (SelectDirChange != INVALID_HANDLE_VALUE)
	{
		::FindCloseChangeNotification(SelectDirChange);
		SelectDirChange = INVALID_HANDLE_VALUE;
	}

// zru�en� hl�en� o zm�n� adres��e
	if (SelectDirChange2 != INVALID_HANDLE_VALUE)
	{
		::FindCloseChangeNotification(SelectDirChange2);
		SelectDirChange2 = INVALID_HANDLE_VALUE;
	}

// zah�jen� hl�en� zm�n adres��e
	SelectDirChange = ::FindFirstChangeNotification(
		ProgramPath + Cesta,			// cesta k adres��i
		FALSE,							// nehl�sit podadred��e
		FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME);

	if (ProgramPath2.IsNotEmpty())
	{
		SelectDirChange2 = ::FindFirstChangeNotification(
			ProgramPath2 + Cesta,			// cesta k adres��i
			FALSE,							// nehl�sit podadred��e
			FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME);
	}

	if (SelectDirChangeTimer == 0)
	{
		SelectDirChangeTimer = ::SetTimer(MainFrame, SelectDirChangeTimerID, 2000, NULL);
	}

// zaji�t�n� p�ekreslen� okna
	TimeRepaint(-1);

// ukon�en� pln�n� okna
	SelectFill = FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace volby Start, Open, Rename, Kopie a Delete

void UpdateStartOpen()
{
	CText txt;
	CText txt2;

// start, otev�en�, ru�en�, kopie - mus� b�t platn� jm�no pod kurzorem
	BOOL enable = (SelectTyp != TYP0);
	EnableCommand(IDN_START, enable);
	EnableCommand(IDN_OPEN, enable);

// polo�ku ".." ani vzorov� program nelze ru�it, p�ejmenovat ani kop�rovat
	if (SelectTyp == TYPUPDIR) enable = FALSE;
	EnableCommand(IDN_RENAME, enable && !SelectAltern);
	EnableCommand(IDN_DELETE, enable && !SelectAltern);

// adres�� nelze kop�rovat
	if (SelectTyp != TYPFILE) enable = FALSE;
	EnableCommand(IDN_NEWCOPY, enable);

// zkr�cen� stavov�ho ��dku
	int stat2 = SelectStatusWidth - StatusWidth;

// p��prava data a �asu souboru/adres��e
	if ((SelectTyp == TYPSUBDIR) || (SelectTyp == TYPFILE))
	{
		WIN32_FIND_DATA wfd;
		HANDLE fnd;
		fnd = ::FindFirstFile(JmenoLoad, &wfd);

		if (fnd != INVALID_HANDLE_VALUE)
		{

// nen�-li adres��, je v�t�� rezerva ve stavov�m ��dku (nen� velikost)
			if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
			{
				stat2 -= 80;
			}			

// p��prava lok�ln�ho data a �asu
			FILETIME loc;
			::FileTimeToLocalFileTime(&wfd.ftLastWriteTime, &loc);

			SYSTEMTIME sys;
			::FileTimeToSystemTime(&loc, &sys);

// datum souboru
			if (stat2 < 5)
			{
				txt.Load(IDN_DATUM);
				txt.Add(_T(' '));
			}

			switch (Jazyk)
			{
// polsky
			case JAZYKPOL:
				txt2.Int(sys.wYear);
				txt.Add(txt2);
				txt.Add(_T('-'));
				txt2.Int(sys.wMonth);
				txt.Add(txt2);
				txt.Add(_T('-'));
				txt2.Int(sys.wDay);
				txt.Add(txt2);
				break;

// anglicky
			case JAZYKENG:
				txt2.Int(sys.wMonth);
				txt.Add(txt2);
				txt.Add(_T('/'));
				txt2.Int(sys.wDay);
				txt.Add(txt2);
				txt.Add(_T('/'));
				txt2.Int(sys.wYear);
				txt.Add(txt2);
				break;

// francouzsky, �pan�lsky, italsky
			case JAZYKFRA:
			case JAZYKSPA:
			case JAZYKITA:
				txt2.Int(sys.wDay);
				txt.Add(txt2);
				txt.Add(_T('/'));
				txt2.Int(sys.wMonth);
				txt.Add(txt2);
				txt.Add(_T('/'));
				txt2.Int(sys.wYear);
				txt.Add(txt2);
				break;

// �esky, slovensky, n�mecky
			default:
				txt2.Int(sys.wDay);
				txt.Add(txt2);
				txt.Add(_T('.'));
				txt2.Int(sys.wMonth);
				txt.Add(txt2);
				txt.Add(_T('.'));
				txt2.Int(sys.wYear);
				txt.Add(txt2);
			}

// �as souboru
			txt.Add(_T(", "));
			if (stat2 < 5)
			{
				txt2.Load(IDN_CAS);
				txt.Add(txt2);
				txt.Add(_T(' '));
			}

			txt2.Int(sys.wHour);
			txt.Add(txt2);
			txt.Add(_T(':'));
			if (sys.wMinute < 10)
			{
				txt.Add(_T('0'));
			}
			txt2.Int(sys.wMinute);
			txt.Add(txt2);

// velikost souboru
			if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			{
				txt.Add(_T(", "));
				if (stat2 < 60)
				{
					txt2.Load(IDN_VELIKOST);
					txt.Add(txt2);
					txt.Add(_T(' '));
				}
				txt2.Int(((int)wfd.nFileSizeLow + 1023)/1024);
				txt.Add(txt2);
				txt.Add(_T(' '));
				txt2.Load(IDN_KB);
				txt.Add(txt2);
			}

			::FindClose(fnd);
			txt.Add(_T(" / "));
		}
	}

// voln� m�sto disku
	GetDiskSpace(ProgramPath);

	if (stat2 < 90)
	{
		txt2.Load(IDN_VOLNE);
		txt.Add(txt2);
		txt += _T(' ');
	}
	txt2.Int(Round0((double)DiskFree/(1024*1024)));
	txt.Add(txt2);
	txt.Add(_T(' '));
	txt2.Load(IDN_MB);
	txt.Add(txt2);

// text data souboru
	SetStatusText2(txt);
}


/////////////////////////////////////////////////////////////////////////////
// zm�na aktu�ln� polo�ky (LVN_ITEMCHANGED)

void SelectOnChange(HWND hWnd, NM_LISTVIEW* nmlv)
{
// kontrola, zda je re�im v�b�ru
	if (!SelectMode) return;

// kontrola, zda je zpr�va od okna v�b�ru
	if (hWnd != SelectView) return;

// kontrola platnosti polo�ky
	if ((DWORD)nmlv->lParam < (DWORD)SelectItemNum)
	{

// kontrola, zda je zm�na zam��en� ze star� polo�ky na novou
		if (((nmlv->uNewState & LVIS_FOCUSED) != 0) &&
			((nmlv->uOldState & LVIS_FOCUSED) == 0))
		{

// nastaven� nov�ho jm�na souboru
			if (!SelectFill) SelectSelect(nmlv->lParam);
			ZobrazJmeno();
		}
	}

// aktualizace tla��tek a polo�ek menu
	UpdateStartOpen();
}


/////////////////////////////////////////////////////////////////////////////
// test, zda je spustiteln� zavad�� programu (TRUE=je)

BOOL SelectTestExe()
{
	if (SelectTyp != TYPFILE) return FALSE;

	CFileMap file;

// otev�en� souboru
	if (file.Open(JmenoLoad))
	{

// p��prava NT z�hlav� souboru EXE
		IMAGE_NT_HEADERS* hdr = file.NTHeader();
		if (hdr != NULL)
		{

// kontrola velikosti k�du programu
			return (hdr->OptionalHeader.SizeOfCode > 0x4000);
		}
	}
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// otev�en� souboru k editaci

void SelectOnOpen()
{
// kontrola, zda je re�im v�b�ru
	if (!SelectMode) return;

// pro adres�� se provede zm�na adres��e
	if (SelectTyp != TYPFILE)
	{
		SelectOnStart();
	}

// pro soubor otev�en� k editaci
	else
	{

// zah�jen� re�imu editace soubor�
		SelectEdit();
	}
}


/////////////////////////////////////////////////////////////////////////////
// dvojklik my�� - start programu

void SelectOnDblClk(HWND hWnd)
{
// kontrola, zda je re�im v�b�ru
	if (!SelectMode) return;

// kontrola, zda je zpr�va od okna v�b�ru
	if (hWnd != SelectView) return;

// start aktivn�ho programu
	SelectOnStart();
}


/////////////////////////////////////////////////////////////////////////////
// start programu nebo zm�na adres��e

void SelectOnStart()
{
// kontrola, zda je re�im v�b�ru
	if (!SelectMode) return;

// pouze je-li n�jak� platn� polo�ka
	if (SelectTyp != TYP0)
	{

// kontrola, zda to je adres��
		if (SelectTyp != TYPFILE)
		{

// vyno�en� z adres��e
			if (SelectTyp == TYPUPDIR)
			{
				Cesta.Delete(Cesta.Length()-1);		// odstran�n� koncov�ho "\"
				int index = Cesta.RevFind(_T('\\'))+1;	// nalezen� p�ede�l�ho "\" nebo 0
				Jmeno.Empty();
				Jmeno.MultiText(JAZYK000, Cesta.Right(Cesta.Length()-index)); // posledn� adres��
				Cesta.Delete(index);				// odstran�n� posledn�ho adres��e
			}

// vno�en� do adres��e
			else
			{
				Cesta.Add(Jmeno.MultiText((int)JAZYK000) + _T('\\')); // nov� cesta
				Jmeno.Empty();
				Jmeno.MultiText(JAZYK000, CText(_T("..")));
			}
			SelectLoad();							// znovuna�ten� adres��e
		}

// je to soubor - start programu
		else
		{
			if (SelectTestExe())
			{
				SetJazykReg(LangID);

				Exec(CText(_T('"')) + JmenoLoad + _T('"'), 
					ProgramPath + Cesta,
					FALSE);	// spu�t�n� programu
			}

// bez zavad��e se program otev�e k editaci
			else
			{
				SelectEdit();
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nalezen� jedine�n�ho nov�ho jm�na k zad�n� (over = p�eskakovan� polo�ka, -1=nen�)

void SelectNewNameUnique(int over)
{
	int nn = 0;

	for (int i = 0; i < SelectItemNum; i++)
	{
		if ((SelectItem[i].Name.MultiText() == SelectNewText) && (over != i))
		{
			nn++;
			if (nn >= 10000) break;
			SelectNewText.SetNumObj(nn);
			SelectNewMultiText.SetNumObj(nn);
			i = -1;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nalezen� jedine�n�ho zadan�ho jm�na souboru/adres��e (over = p�eskakovan� polo�ka, -1=nen�)

CText SelectNewFileUnique(int over)
{
// p��prava jedine�n�ho jm�na (pro nezkr�cen� jm�na se nastavuje v cyklu)
	int nn = 0;
	if (ShortName)
	{
		nn = 1;
		SelectNewNameUnique(over);
	}

// p��prava kr�tk�ho jm�na souboru
	CText shortname = SelectNewText;
	shortname.KorigShort();

// nalezen� neexistuj�c�ho jm�na souboru/adres��e
	CText txtint;
	CText txt = shortname;

	CText overname;

	if ((DWORD)over < (DWORD)SelectItemNum)
	{
		overname = SelectItem[over].Name.MultiText((int)JAZYK000);
		if (SelectItem[over].Typ == TYPFILE)
		{
			overname.Delete(overname.Length() - 4);
		}
	}

	while (((int)::GetFileAttributes(ProgramPath + Cesta + txt + TextExe) != -1) ||
			((int)::GetFileAttributes(ProgramPath + Cesta + txt) != -1) ||
			(ProgramPath2.IsNotEmpty() && 
					(((int)::GetFileAttributes(ProgramPath2 + Cesta + txt + TextExe) != -1) ||
					((int)::GetFileAttributes(ProgramPath2 + Cesta + txt) != -1))
				)
			)
	{
		nn++;
		if (nn >= 10000) break;

		if (txt == overname) break;

		if (ShortName)
		{
			txtint.Int(nn);

			while ((shortname.Length() + txtint.Length()) > 8)
			{
				shortname.Delete(shortname.Length() - 1);
			}
			txt = shortname + txtint;
		}
		else
		{
			txt = shortname;
			txt.SetNumObj(nn);
			SelectNewText.SetNumObj(nn);
			SelectNewMultiText.SetNumObj(nn);
		}
	}
	return txt;
}


/////////////////////////////////////////////////////////////////////////////
// obsluha dialogu

BOOL CALLBACK SelectDialogProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
	case WM_INITDIALOG:
		{
			CenterWindow(hWnd, MainFrame);

			CText txt;
			txt.Load(lParam);
			txt.SetWindowTextFont(hWnd);

			switch (lParam)
			{
			case IDN_SELECT_CREATE:
				txt.Load(IDN_CANCEL0);
				txt.SetDialogTextFont(hWnd, IDCANCEL);

				DialogCustomText(hWnd, IDN_SELECT_CREATE1);
				DialogCustomText(hWnd, IDN_JINE_JMENO);
				break;

			case IDN_SELECT_DEL:
				txt.Load(IDN_CANCEL0);
				txt.SetDialogTextFont(hWnd, IDCANCEL);

				DialogCustomText(hWnd, IDN_SELECT_DEL1);
				DialogCustomText(hWnd, IDN_SELECT_DEL2);
				break;

			case IDN_SELECT_DELDIR:
				txt.Load(IDN_CANCEL0);
				txt.SetDialogTextFont(hWnd, IDCANCEL);

				DialogCustomText(hWnd, IDN_SELECT_DELDIR1);
				break;

			case IDN_SELECT_DELDIRERROR:
				DialogCustomText(hWnd, IDN_SELECT_DELDIRERROR1);
				break;

			case IDN_SELECT_DELERROR:
				DialogCustomText(hWnd, IDN_SELECT_DELERROR1);
				break;

			case IDN_SELECT_DIR:
				txt.Load(IDN_CANCEL0);
				txt.SetDialogTextFont(hWnd, IDCANCEL);

				DialogCustomText(hWnd, IDN_SELECT_DIR1);
				break;

			case IDN_SELECT_DIRCREATE:
				txt.Load(IDN_CANCEL0);
				txt.SetDialogTextFont(hWnd, IDCANCEL);

				DialogCustomText(hWnd, IDN_SELECT_DIRCREATE1);
				DialogCustomText(hWnd, IDN_JINE_JMENO);
				break;

			case IDN_SELECT_DIREXIST:
				txt.Load(IDN_CANCEL0);
				txt.SetDialogTextFont(hWnd, IDCANCEL);

				DialogCustomText(hWnd, IDN_SELECT_DIREXIST1);
				DialogCustomText(hWnd, IDN_JINE_JMENO);
				break;

			case IDN_SELECT_EXIST:
				txt.Load(IDN_CANCEL0);
				txt.SetDialogTextFont(hWnd, IDCANCEL);

				DialogCustomText(hWnd, IDN_SELECT_EXIST1);
				DialogCustomText(hWnd, IDN_JINE_JMENO);
				break;

			case IDN_SELECT_NEW:
				txt.Load(IDN_CANCEL0);
				txt.SetDialogTextFont(hWnd, IDCANCEL);

				DialogCustomText(hWnd, IDN_SELECT_NEW1);
				break;

			case IDN_SELECT_NEWCOPY:
				txt.Load(IDN_CANCEL0);
				txt.SetDialogTextFont(hWnd, IDCANCEL);

				DialogCustomText(hWnd, IDN_SELECT_NEWCOPY1);
				break;
			}

			SelectNewEdit = ::GetDlgItem(hWnd, IDN_SELECT_EDIT);
			SetFontBezny(SelectNewEdit);
			::SetWindowText(SelectNewEdit, SelectNewText);
			if ((lParam == IDN_SELECT_DEL) || (lParam == IDN_SELECT_DELDIR))
			{
				::SetFocus(::GetDlgItem(hWnd, IDCANCEL));
				return FALSE;
			}
		}
		return TRUE;

	case WM_DRAWITEM:				// 0x02B
		if (DialogDraw(hWnd, lParam)) return TRUE;
		return FALSE;

	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
		case 0:
		case 1:
			switch (LOWORD(wParam))
			{
			case IDOK:
			case IDCANCEL:
				SelectNewText.GetWindowText(SelectNewEdit);
				::EndDialog(hWnd, LOWORD(wParam));
				return TRUE;
			}
			break;

//		case EN_CHANGE:						// nen� pot�eba ji� obsluhovat (pou��vaj� se aliases)
//			if (!SelectNewFill)
//			{
//				SelectNewFill = TRUE;
//	
//				int selstart;
//				::SendMessage(SelectNewEdit, EM_GETSEL, (WPARAM)&selstart, NULL);
//				SelectNewText.GetWindowText(SelectNewEdit);
//
//				int oldlen = SelectNewText.Length();
//				selstart = SelectNewText.FileName(selstart);
//  
//				if (oldlen != SelectNewText.Length())
//				{
//					SelectNewText.SetWindowText(SelectNewEdit);
//					::SendMessage(SelectNewEdit, EM_SETSEL, selstart, selstart);
//				}
//
//				SelectNewFill = FALSE;
//			}
//			break;
		}
		break;

	}
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� nov�ho souboru

// velikost str�nky zarovn�v�n� pam�ti
#ifndef _M_ALPHA
#define	PAGESIZE		0x1000			// velikost aloka�n� str�nky pro ostatn� procesory (4096)
#define PAGEFILE		0x1000			// velikost str�nky v souboru
#else
#define	PAGESIZE		0x2000			// velikost aloka�n� str�nky pro procesor Alpha (8192)
#define PAGEFILE		0x2000			// velikost str�nky v souboru
#endif

void SelectOnNew()
{
// kontrola, zda je re�im v�b�ru
	if (!SelectMode) return;

// p��prava zadan�ho jm�na souboru
	if (SelectTyp == TYPFILE)
	{
		SelectNewText = Jmeno.MultiText();
	}
	else
	{
		SelectNewText.Empty();
	}

// implicitn� jm�no, nen�-li ��dn� vzor
	if (SelectNewText.IsEmpty()) SelectNewText.Load(IDN_NEWFILE_NAME);

// nalezen� neexistuj�c�ho souboru
	SelectNewNameUnique(-1);

// cyklus opakov�n� zad�n�
	int dialogID = IDN_SELECT_NEW;

	for (;;)
	{

// zad�n� jm�na programu
		if (::DialogBoxParam(
					hInstance,
					MAKEINTRESOURCE(dialogID),
					MainFrame,
					(DLGPROC)SelectDialogProc,
					(LPARAM)dialogID) != IDOK) return;

// p�ednastaven� p��t�ho dialogu
		dialogID = IDN_SELECT_CREATE;

// korekce zadan�ho textu
		SelectNewText.TrimLeft();
		SelectNewText.TrimRight();
		if (SelectNewText.IsEmpty()) return;

// p��prava jm�na souboru
		CText txt = SelectNewFileUnique(-1);

// jm�no souboru
		if (ShortName)
		{
			txt += TextExe2;
		}
		else
		{
			txt += TextExe;
		}
		CText filename = ProgramPath + Cesta + txt;

// vytvo�en� souboru
		HANDLE file = ::CreateFile(filename, GENERIC_WRITE,
			0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

// chyba vytvo�en� - test, zda soubor ji� existuje (jinak hl�ena nezn�m� chyba)
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!! to ji� nemus� b�t !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if (file == INVALID_HANDLE_VALUE)
		{
			if ((int) :: GetFileAttributes(filename) != -1)
			{
				dialogID = IDN_SELECT_EXIST;
			}
		}

// p��prava mal�ho zavad��e programu
		else
		{
			CResource loader;
			if (loader.OpenCopy(IDN_LOADERM, _T("LOADER")))
			{

// adresa z�hlav� NT
				IMAGE_NT_HEADERS* hdr = loader.NTHeader();
				if (hdr != NULL)
				{

// offset za��tku dat v souboru (= zarovnan� velikost zavad��e)
					int SaveDataOff = (loader.Size() + (PAGEFILE-1)) & ~(PAGEFILE-1);

// virtu�ln� adresa dat
					int SaveDataVirt = (hdr->OptionalHeader.SizeOfImage + PAGESIZE-1) & ~(PAGESIZE-1);

// velikost obrazu v�ech dat
					hdr->OptionalHeader.SizeOfImage = (SaveDataVirt + sizeof(NewHead) + PAGESIZE-1) & ~(PAGESIZE-1);

// adresa vkl�dan� sekce
					IMAGE_SECTION_HEADER* sec = (IMAGE_SECTION_HEADER*)((BYTE*)(&hdr->OptionalHeader) 
							+ hdr->FileHeader.SizeOfOptionalHeader + hdr->FileHeader.NumberOfSections 
							* sizeof(IMAGE_SECTION_HEADER));

// zv��en� ��ta�e sekc�
					hdr->FileHeader.NumberOfSections++;

// nastaven� sekce
					MemCopy(sec, &PetProgHeader, sizeof(IMAGE_SECTION_HEADER));

// nastaven� offsetu za��tku dat v souboru
					sec->PointerToRawData = SaveDataOff;
					sec->VirtualAddress = SaveDataVirt;

// zarovnan� velikost souboru
					int oldsize = SaveDataOff + sizeof(NewHead);
					int newsize = (oldsize + 0x1ff) & ~0x1ff;

// velikost dat programu v souboru
					sec->SizeOfRawData = newsize - SaveDataOff;

// virtu�ln� velikost dat programu
					sec->Misc.VirtualSize = sizeof(NewHead);

// z�pis zavad��e
					DWORD writen;				// po�et zapsan�ch dat
					if ((!::WriteFile(file, loader.Adr(), loader.Size(), &writen, NULL)) ||
						(loader.Size() != (int)writen))
					{
						loader.CloseCopy();
					}					
					else
					{

// zarovn�n� d�lky zavad��e na souborovou str�nku
						int i = SaveDataOff - loader.Size();
						if (i > 0)
						{
							BYTE* dst = (BYTE*)MemGet(i);
							if (dst != NULL)
							{
								MemFill(dst, i, 0);
								::WriteFile(file, dst, i, &writen, NULL);
							}
						}

// zru�en� bufferu zavad��e
						loader.CloseCopy();

// z�pis programu
						::WriteFile(file, &NewHead, sizeof(NewHead), &writen, NULL);

// zarovn�n� na sektor
						i = newsize - oldsize;
						if (i > 0)
						{
							BYTE* dst = (BYTE*)MemGet(i);
							if (dst != NULL)
							{
								MemFill(dst, i, 0);
								::WriteFile(file, dst, i, &writen, NULL);
							}
						}

// uzav�en� souboru
						::CloseHandle(file);

// nastaven� nov�ho jm�na souboru
						Jmeno.Empty();
						Jmeno.MultiText(JAZYK000, txt);
						Jmeno.MultiText(SelectNewText);
						JmenoLoad = filename;
						JmenoSave = filename;
						::WritePrivateProfileString(JazykInfo[Jazyk].LangIDini, txt, SelectNewText, SelectAliases);
						ZobrazJmeno();

// zah�jen� editace programu (nepou��vat SelectOnOpen - nesouhlas� parametry aktivn�ho programu!)
						SelectEdit();
						return;
					}
				}
			}

// uzav�en� a zru�en� souboru (a pokra�ov�n� chybov�m hl�en�m)
			::CloseHandle(file);
			::DeleteFile(filename);
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// operace PASTE p�i editaci jm�na souboru

void SelectOnPaste()
{
// kontrola, zda je re�im v�b�ru
	if (!SelectMode) return;

// editace jm�na souboru
	if (SelEditName)
	{
		::SendMessage(SelEditNameWnd, WM_PASTE, 0, 0);
		return;
	}
}


/////////////////////////////////////////////////////////////////////////////
// operace CUT p�i editaci jm�na souboru

void SelectOnCut()
{
// kontrola, zda je re�im v�b�ru
	if (!SelectMode) return;

// editace jm�na souboru
	if (SelEditName)
	{
		::SendMessage(SelEditNameWnd, WM_CUT, 0, 0);
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////
// operace UNDO p�i editaci jm�na souboru

void SelectOnUndo()
{
// kontrola, zda je re�im v�b�ru
	if (!SelectMode) return;

// editace jm�na souboru
	if (SelEditName)
	{
		::SendMessage(SelEditNameWnd, EM_UNDO, 0, 0);
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� nov�ho souboru kopi�

void SelectOnNewCopy()
{
// kontrola, zda je re�im v�b�ru
	if (!SelectMode) return;

// editace jm�na souboru
	if (SelEditName)
	{
		::SendMessage(SelEditNameWnd, WM_COPY, 0, 0);
		return;
	}

// kontrola, zda je platn� jm�no souboru pod kurzorem
	if (SelectTyp != TYPFILE) return;

// p��prava zadan�ho jm�na souboru
	SelectNewText = Jmeno.MultiText();
	SelectNewMultiText = Jmeno;

// nalezen� neexistuj�c�ho souboru
	SelectNewNameUnique(-1);

// cyklus opakov�n� zad�n�
	int dialogID = IDN_SELECT_NEWCOPY;

	for (;;)
	{

// zad�n� jm�na programu
		if (::DialogBoxParam(
					hInstance,
					MAKEINTRESOURCE(dialogID),
					MainFrame,
					(DLGPROC)SelectDialogProc,
					(LPARAM)dialogID) != IDOK) return;

// p�ednastaven� p��t�ho dialogu
		dialogID = IDN_SELECT_CREATE;

// korekce zadan�ho textu
		SelectNewText.TrimLeft();
		SelectNewText.TrimRight();
		if (SelectNewText.IsEmpty()) return;

// p��prava jm�na souboru
		CText txt = SelectNewFileUnique(-1);

// jm�no souboru
		if (ShortName)
		{
			txt += TextExe2;
		}
		else
		{
			txt += TextExe;
		}
		CText newname = ProgramPath + Cesta + txt;

// kopie souboru
		if (::CopyFile(JmenoLoad, newname, TRUE))
		{

// zru�en� atributu R/O u kopie
			::SetFileAttributes(newname, FILE_ATTRIBUTE_NORMAL);

// nastaven� nov�ho jm�na souboru
			Jmeno = SelectNewMultiText;
			Jmeno.MultiText(JAZYK000, txt);
			Jmeno.MultiText(SelectNewText);
			JmenoLoad = newname;
			JmenoSave = newname;

			for (int lang = 0; lang < JAZYKNUM; lang++)
			{
				if (lang != JAZYK000)
				{
					if ((Jmeno.MultiText(lang)).IsNotEmpty())
					{
						::WritePrivateProfileString(JazykInfo[lang].LangIDini, txt, Jmeno.MultiText(lang), SelectAliases);
					}
				}
			}

			ZobrazJmeno();

// zah�jen� editace programu (nepou��vat SelectOnOpen - nesouhlas� parametry aktivn�ho programu!)
			SelectEdit();
			return;
		}

// chyba vytvo�en� - test, zda soubor ji� existuje (jinak hl�ena nezn�m� chyba)
		if ((int)::GetFileAttributes(newname) != -1)
		{
			dialogID = IDN_SELECT_EXIST;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� adres��e

void SelectOnPath()
{
// kontrola, zda je re�im v�b�ru
	if (!SelectMode) return;

// p��prava zadan�ho jm�na adres��e
	if (SelectTyp == TYPUPDIR)
	{
		SelectNewText = Jmeno.MultiText();
	}
	else
	{
		SelectNewText.Empty();
	}

// implicitn� jm�no, nen�-li ��dn� vzor
	if (SelectNewText.IsEmpty()) SelectNewText.Load(IDN_NEWDIR_NAME);

// nalezen� neexistuj�c�ho adres��e
	SelectNewNameUnique(-1);

// cyklus opakov�n� zad�n�
	int dialogID = IDN_SELECT_DIR;

	for (;;)
	{

// zad�n� jm�na adres��e
		if (::DialogBoxParam(
					hInstance,
					MAKEINTRESOURCE(dialogID),
					MainFrame,
					(DLGPROC)SelectDialogProc,
					(LPARAM)dialogID) != IDOK) return;

// p�ednastaven� p��t�ho dialogu - adres�� existuje
		dialogID = IDN_SELECT_DIREXIST;

// korekce zadan�ho textu
		SelectNewText.TrimLeft();
		SelectNewText.TrimRight();
		if (SelectNewText.IsEmpty()) return;

// p��prava jm�na adres��e
		CText txt = SelectNewFileUnique(-1);
		CText dirname = ProgramPath + Cesta + txt;

// vytvo�en� adres��e
		if (CreateDir(dirname))
		{

// nastaven� nov�ho jm�na souboru
			Jmeno.Empty();
			Jmeno.MultiText(JAZYK000, txt);
			Jmeno.MultiText(SelectNewText);
			JmenoLoad = dirname;
			JmenoSave = dirname;
			::WritePrivateProfileString(JazykInfo[Jazyk].LangIDini, txt, SelectNewText, SelectAliases);
			ZobrazJmeno();

// znovuna�ten� obsahu adres��e
			SelectLoad();
			return;
		}

// p�i chyb� test, zda adres�� ji� existuje (pokud ne, je n�jak� jin� chyba)
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!! to ji� nemus� b�t !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if ((int)::GetFileAttributes(dirname) == -1)
		{
			dialogID = IDN_SELECT_DIRCREATE;
		}
	}		
}		


/////////////////////////////////////////////////////////////////////////////
// zru�en� souboru nebo adres��e

void SelectOnDelete()
{
// kontrola, zda je re�im v�b�ru
	if (!SelectMode) return;

// editace jm�na prvku
	if (SelEditName)
	{
		long int start, end;
		::SendMessage(SelEditNameWnd, EM_GETSEL, (WPARAM)&start, (WPARAM)&end);

		if (start == end)
		{
			::SendMessage(SelEditNameWnd, WM_KEYDOWN,	VK_DELETE, 0x01530001);
			::SendMessage(SelEditNameWnd, WM_KEYUP,	VK_DELETE, 0xC1530001);
		}
		else
		{
			::SendMessage(SelEditNameWnd, WM_CLEAR, 0, 0);
		}
		return;
	}

// kontrola, zda je platn� jm�no
	if ((SelectTyp == TYP0) || (SelectTyp == TYPUPDIR) || SelectAltern) return;

// kr�tk� jm�no polo�ky
	CText txt0 = Jmeno.MultiText((int)JAZYK000);

// rozli�en�, zda to je adres�� nebo soubor
	if (SelectTyp == TYPSUBDIR)
	{

// zobrazen� dialogu k potvrzen� ru�en� adres��e
		if (::DialogBoxParam(
					hInstance,
					MAKEINTRESOURCE(IDN_SELECT_DELDIR),
					MainFrame,
					(DLGPROC)SelectDialogProc,
					(LPARAM)IDN_SELECT_DELDIR) != IDOK) return;

// zru�en� aliases, neobsahuje-li slo�ka ��dn� program
		WIN32_FIND_DATA wfd;
		HANDLE handle = ::FindFirstFile(ProgramPath + Cesta + txt0 + _T("\\*.exe"), &wfd);
		if (handle != INVALID_HANDLE_VALUE)
		{
			::FindClose(handle);
		}
		else
		{
			::DeleteFile(ProgramPath + Cesta + txt0 + _T("\\ALIASES.INI"));
		}

// zru�en� adres��e
		if (!::RemoveDirectory(JmenoSave))
		{

// adres�� nelze zru�it
			::DialogBoxParam(
					hInstance,
					MAKEINTRESOURCE(IDN_SELECT_DELDIRERROR),
					MainFrame,
					(DLGPROC)SelectDialogProc,
					(LPARAM)IDN_SELECT_DELDIRERROR);
			return;
		}
	}
	else
	{

// zobrazen� dialogu k potvrzen� ru�en� souboru
		if (::DialogBoxParam(
					hInstance,
					MAKEINTRESOURCE(IDN_SELECT_DEL),
					MainFrame,
					(DLGPROC)SelectDialogProc,
					(LPARAM)IDN_SELECT_DEL) != IDOK) return;

// kontrola, zda ru�en� soubor nen� chr�n�n proti z�pisu
		int attrib = (int)::GetFileAttributes(JmenoSave);
		if ((attrib != -1) && ((attrib & FILE_ATTRIBUTE_READONLY) != 0))
		{

// soubor nelze zru�it
			::DialogBoxParam(
					hInstance,
					MAKEINTRESOURCE(IDN_SELECT_DELERROR),
					MainFrame,
					(DLGPROC)SelectDialogProc,
					(LPARAM)IDN_SELECT_DELERROR);
			return;
		}

// z�lohov�n� a zru�en� souboru
		CText backpath = BackupPath + Cesta;
		CreatePath(backpath);
		CText backname = backpath + txt0;
		::DeleteFile(backname);
		::MoveFile(JmenoSave, backname);
		::DeleteFile(JmenoSave);

// z�lohov�n� aliases
		CText aliases = backpath + _T("\\ALIASES.INI");
		for (int lang = 0; lang < JAZYKNUM; lang++)
		{
			if (lang != JAZYK000)
			{
				if ((Jmeno.MultiText(lang)).IsEmpty())
				{
					::WritePrivateProfileString(JazykInfo[lang].LangIDini, txt0, NULL, aliases);
				}
				else
				{
					::WritePrivateProfileString(JazykInfo[lang].LangIDini, txt0, Jmeno.MultiText(lang), aliases);
				}
			}
		}
	}

// zru�en� aliases
	for (int lang = 0; lang < JAZYKNUM; lang++)
	{
		if (lang != JAZYK000)
		{
			::WritePrivateProfileString(JazykInfo[lang].LangIDini, txt0, NULL, SelectAliases);
		}
	}

// test, zda je vzorov� polo�ka (zda z�stane jm�no nezm�n�no)
	if (ProgramPath2.IsEmpty() || 
		((int)::GetFileAttributes(ProgramPath2 + Cesta + txt0) == -1))
	{
		int index = SelectItem[SelectInx].Index + 1;
		if (index >= SelectItemNum) index -= 2;

		for (int i = 0; i < SelectItemNum; i++)
		{
			if (SelectItem[i].Index == index)
			{
				Jmeno = SelectItem[i].Name;
			}
		}

		ZobrazJmeno();
	}

// znovuna�ten� soubor�
	SelectLoad();
}


/////////////////////////////////////////////////////////////////////////////
// obsluha kl�vesy (vrac� TRUE=zru�en� kl�vesy)

BOOL SelectOnKeyDown(HWND hWnd, int key, int data)
{
// kontrola, zda je re�im v�b�ru
	if (!SelectMode || (hWnd != SelectView)) return FALSE;

// rozli�en� podle kl�vesy
	switch (key)
	{

// Alt-Enter = p�ejmenov�n�
	case VK_RETURN:
		if ((!SelEditName) && (::GetKeyState(VK_MENU) < 0) && 
			(SelectTyp != TYP0) && (SelectTyp != TYPUPDIR) && !SelectAltern)
		{

// zah�jen� editace jm�na prvku
			::PostMessage(SelectView, LVM_EDITLABEL, (LPARAM)SelectItem[SelectInx].Index, 0);
		}
		break;
	}

	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// zah�jen� editace jm�na souboru/adres��e

void SelectOnRename()
{
// kontrola, zda je re�im v�b�ru a zda nen� editace
	if (!SelectMode || SelEditName) return;

// zah�jen� editace jm�na prvku
	if ((SelectTyp != TYP0) && (SelectTyp != TYPUPDIR) && !SelectAltern)
	{
		::PostMessage(SelectView, LVM_EDITLABEL, (LPARAM)SelectItem[SelectInx].Index, 0);
	}
}


/////////////////////////////////////////////////////////////////////////////
// zah�jen� editace jm�na prvku (vrac� TRUE=p�eru�it editaci)

BOOL SelectOnBeginLabelEdit(HWND hWnd, int iItem)
{
// kontrola, zda je re�im v�b�ru
	if (!SelectMode || SelEditName || 
		(hWnd != SelectView) || 
		(SelectTyp == TYP0) || 
		(SelectTyp == TYPUPDIR) || 
		SelectAltern)
	{
		return TRUE;
	}

// na�ten� handle edita�n�ho okna
	SelEditNameWnd = (HWND)::SendMessage(SelectView, LVM_GETEDITCONTROL, 0, 0);
	if (SelEditNameWnd == NULL) return TRUE;

// p��znak zah�jen� editace
	SelEditName = TRUE;

// p��znak povolen� editace
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// ukon�en� editace jm�na prvku

void SelectOnEndLabelEdit(HWND hWnd, LV_ITEM* lvi)
{
// kontrola, zda je re�im v�b�ru
	if (!SelectMode || !SelEditName || (hWnd != SelectView)) return;

// test, zda je text platn�
	if ((lvi->pszText != NULL) && (SelectTyp != TYP0) && (SelectTyp != TYPUPDIR) && !SelectAltern)
	{

// na�ten� zadan�ho textu
		SelectNewText = lvi->pszText;
		SelectNewText.TrimLeft();
		SelectNewText.TrimRight();
		SelectNewMultiText = Jmeno;

// aktu�ln� jm�no bez p��pony
		CText oldtxt = Jmeno.MultiText((int)JAZYK000);
		if (SelectTyp == TYPFILE) oldtxt.Delete(oldtxt.Length() - 4);

// korekce implicitn�ho jm�na
		BOOL impl = TRUE;
		int lan;
		for (lan = 0; lan < JAZYKNUM; lan++)
		{
			if ((lan != JAZYK000) && SelectNewMultiText.MultiText(lan).IsNotEmpty())
			{
				impl = FALSE;
				break;
			}
		}
		if (impl) SelectNewText = oldtxt;

		SelectNewMultiText.MultiText(SelectNewText);

// p��prava nov�ho jm�na souboru
//		CText txt = SelectNewMultiText.MultiText();
//		txt.KorigShort();
//		if (txt != oldtxt) txt = SelectNewFileUnique();
		CText txt = SelectNewFileUnique(SelectInx);

// p�id�n� p��pony
		if (SelectTyp == TYPFILE)
		{
			if (ShortName)
			{
				txt += TextExe2;
			}
			else
			{
				txt += TextExe;
			}
		}

		oldtxt = Jmeno.MultiText((int)JAZYK000);
		CText newname = ProgramPath + Cesta + txt;

// p�ejmenov�n� souboru nebo adres��e (adres�� se po chv�li automaticky na�te!!!!!)
		if ((oldtxt == txt) || ::MoveFile(JmenoSave, newname))
		{
			if (oldtxt == txt)
			{
				::WritePrivateProfileString(JazykInfo[Jazyk].LangIDini, txt, SelectNewMultiText.MultiText(Jazyk), SelectAliases);
			}
			else
			{
				SelectNewMultiText.MultiText(JAZYK000, txt);

				for (int lang = 0; lang < JAZYKNUM; lang++)
				{
					if (lang != JAZYK000)
					{
						::WritePrivateProfileString(JazykInfo[lang].LangIDini, oldtxt, NULL, SelectAliases);

						if ((SelectNewMultiText.MultiText(lang)).IsEmpty())
						{
							::WritePrivateProfileString(JazykInfo[lang].LangIDini, txt, NULL, SelectAliases);
						}
						else
						{
							::WritePrivateProfileString(JazykInfo[lang].LangIDini, txt, SelectNewMultiText.MultiText(lang), SelectAliases);
						}
					}
				}
			}

			SelectItem[SelectInx].Name = SelectNewMultiText;
			SelectItem[SelectInx].NameLoad = ProgramPath + Cesta + SelectNewMultiText.MultiText((int)JAZYK000);
			SelectItem[SelectInx].NameSave = SelectItem[SelectInx].NameLoad;

			SelectSelect(SelectInx);

			lvi->pszText = (LPTSTR)(LPCTSTR)Jmeno.MultiText();
		}
		ZobrazJmeno();
	}

// p��znak ukon�en� editace
	SelEditName = FALSE;
	SelEditNameWnd = NULL;
}


/////////////////////////////////////////////////////////////////////////////
// obsluha �asova�e (TRUE=obslou�eno)

BOOL SelectOnTimer(UINT timerID)
{
// kontrola, zda je re�im v�b�ru
	if (!SelectMode) return FALSE;

// nastaven� automatick�ho fokusu
	if (timerID == SelectTimerID)
	{
		if (SelectTimerN > 0)
		{
			SelectTimerN--;
			if (SelectTimerN == 0)
			{

// ukon�en� re�imu v�b�ru soubor�
				SelectTerm();

// zah�jen� re�imu editace soubor�
				ProgInit();

				if (IsWaitCursor)
				{
					IsWaitCursor = FALSE;
					EndWaitCursor();
				}
			}
			return TRUE;
		}
	}

// hl�en� o zm�n� adres��e
	if (timerID == SelectDirChangeTimerID)
	{
		BOOL load = FALSE;

		if (SelectDirChange != INVALID_HANDLE_VALUE)
		{
			while (::WaitForSingleObject(SelectDirChange, 0) == WAIT_OBJECT_0)
			{
				::FindNextChangeNotification(SelectDirChange);
				load = TRUE;
			}
		}

		if (SelectDirChange2 != INVALID_HANDLE_VALUE)
		{
			while (::WaitForSingleObject(SelectDirChange2, 0) == WAIT_OBJECT_0)
			{
				::FindNextChangeNotification(SelectDirChange2);
				load = TRUE;
			}
		}

		if (load)
		{
			SelectLoad();
		}
	}

	return FALSE;
}
