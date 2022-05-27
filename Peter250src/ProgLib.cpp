
#include "Main.h"

/***************************************************************************\
*																			*
*									Knihovny								*
*																			*
\***************************************************************************/

namespace ProgLib
{

// promìnné
BOOL	m_Loading = FALSE;	// probíhá obsluha naèítání
int		m_Parent;			// index položky k naètení obsahu
int		m_ParParam;			// parametry rodièe položek
int		m_Root;				// ROOT položka vìtve
int		m_Func;				// identifikace funkce
BOOL	m_CD = FALSE;		// pøíznak alternativního adresáøe (R/O)
BOOL	m_CD2 = FALSE;		// pøíznak alternativního adresáøe 2 (R/O, CD-ROM)
BOOL	m_CDItem = FALSE;	// pøíznak alternativní položky (mùže to být i adresáø v normálním adresáøi)
BOOL	m_CDItem2 = FALSE;	// pøíznak alternativní položky 2 (mùže to být i adresáø v normálním adresáøi)
BOOL	m_AltExt = FALSE;	// pøíznak alternativní pøípony (JPG, MP3, RMI, RTF)
CText	m_RootPath;			// výchozí cesta (s "\" na konci)
CText	m_RootPath2;		// alternativní cesta (prázdné = není, jinak s "\" na konci)
CText	m_RootPath3;		// alternativní cesta 2 (prázdné = není, jinak s "\" na konci)
CText	m_RootPath3Key;		// klíè ve FOLDERS.INI na CD-ROM
CText	m_SubPath;			// podcesta (s "\" na konci nebo prázdný)
CText	m_Path;				// cesta do adresáøe (s "\" na konci)
CText	m_Aliases;			// jméno INI souboru s ALIASES
CText	m_Ext;				// pøípona souborù 3 znaky
CText	m_Ext2;				// alternativní pøípona souborù 3 znaky (prázdné = není)
CText	m_Name;				// plné jméno souboru
CText	m_NameKey;			// jméno souboru použité jako klíè INI (jméno s pøíponou)
CText	m_IconName;			// jméno ikony k souboru (prázdné = není)

HANDLE*	m_pDirChange = NULL; // adresa hlášení o zmìnì adresáøe
HANDLE*	m_pDirChange2 = NULL; // adresa hlášení o zmìnì alternativního adresáøe

/////////////////////////////////////////////////////////////////////////////
// statická inicializace obsluhy

void StartInit()
{
}


/////////////////////////////////////////////////////////////////////////////
// pøíprava jména souboru

void _fastcall InitName(int index)
{
	m_Name.Empty();
	m_NameKey.Empty();
	m_IconName.Empty();
	ASSERT(BufCls.IsValid(index));

	if (BufCls.IsNotValid(index) || (BufCls[index].Parent < 0)) return;

	m_CDItem = (BufCls[index].Param & PR_CD);
	m_CDItem2 = (BufCls[index].Param & PR_CD2);
	m_AltExt = (BufCls[index].Param & PR_ALTEXT);

	InitPath(BufCls[index].Parent);

	if (m_CDItem && !m_CD)
	{
		m_Path = m_RootPath2;
		m_Name = m_Path;
	}
	else
	{
		if (m_CDItem2 && !m_CD2)
		{
			m_Path = m_RootPath3;
			m_Name = m_Path;
		}
		else
		{
			m_NameKey = Text.Get(BufCls[index].Name).MultiText(JAZYK000);
			m_Name = m_Path + m_NameKey;

			if (BufCls[index].Func != IDF_GROUP)
			{
				if ((m_Name.Length() > 4) && (m_Name[m_Name.Length() - 4] == _T('.')))
				{
					m_Name.Delete(m_Name.Length()-4);
				}

				if (m_Func != IDF_ICON)
				{
					m_IconName = m_Name;
					m_IconName.Add(_T(".ICO"));
				}

				m_Name.Add(_T('.'));

				if (m_AltExt)
				{
					m_Name.Add(m_Ext2);
				}
				else
				{
					m_Name.Add(m_Ext);
				}
			}
		}
	}

	m_Aliases = m_Path;
	m_Aliases += _T("ALIASES.INI");
}


/////////////////////////////////////////////////////////////////////////////
// pøíprava cesty (index = poslední adresáø)

void _fastcall InitPath(int index)
{
// úschova indexu položky
	ASSERT(BufCls.IsValid(index));
	m_Parent = index;

// úschova parametrù rodièe položek
	m_ParParam = BufCls[index].Param;

// pøíznak alternativního adresáøe
	m_CD = (m_ParParam & PR_CD);
	m_CD2 = (m_ParParam & PR_CD2);

// nalezení ROOT položky (a pøíprava èásti adresáøe)
	m_SubPath.Empty();

	if (m_CD || m_CD2)
	{
		if (BufCls[index].Parent >= 0)
		{
			while (BufCls[BufCls[index].Parent].Parent >= 0)
			{
				m_SubPath = Text.Get(BufCls.GetText(index)).MultiText(JAZYK000) + _T('\\') + m_SubPath;
				index = BufCls[index].Parent;
			}
			index = BufCls[index].Parent;
		}
	}
	else
	{
		while (BufCls[index].Parent >= 0)
		{
			m_SubPath = Text.Get(BufCls.GetText(index)).MultiText(JAZYK000) + _T('\\') + m_SubPath;
			index = BufCls[index].Parent;
		}
	}

	m_Root = index;

// úschova typu položky
	m_Func = BufCls[index].Func;

// pøíprava adresáøe a specifikace položek
	m_Ext2.Empty();

	switch (m_Func)
	{
	case IDF_FNC:
		m_RootPath = FunctionPath;
		m_RootPath2 = FunctionPath2;
		m_RootPath3 = FunctionPath3;
		m_RootPath3Key = _T("Function");
		m_pDirChange = &FunctionDirChange;
		m_pDirChange2 = &FunctionDirChange2;
		m_Ext = _T("FNC");
		break;

	case IDF_REAL:
		m_RootPath = NumberPath;
		m_RootPath2 = NumberPath2;
		m_RootPath3 = NumberPath3;
		m_RootPath3Key = _T("Number");
		m_pDirChange = &NumberDirChange;
		m_pDirChange2 = &NumberDirChange2;
		m_Ext = _T("NUM");
		break;

	case IDF_BOOL:
		m_RootPath = BoolPath;
		m_RootPath2 = BoolPath2;
		m_RootPath3 = BoolPath3;
		m_RootPath3Key = _T("Bool");
		m_pDirChange = &BoolDirChange;
		m_pDirChange2 = &BoolDirChange2;
		m_Ext = _T("LOG");
		break;

	case IDF_TEXT:
		m_RootPath = TextPath;
		m_RootPath2 = TextPath2;
		m_RootPath3 = TextPath3;
		m_RootPath3Key = _T("Text");
		m_pDirChange = &TextDirChange;
		m_pDirChange2 = &TextDirChange2;
		m_Ext = _T("TXT");
		m_Ext2 = _T("RTF");
		break;

	case IDF_ICON:
		m_RootPath = IconPath;
		m_RootPath2 = IconPath2;
		m_RootPath3 = IconPath3;
		m_RootPath3Key = _T("Icon");
		m_pDirChange = &IconDirChange;
		m_pDirChange2 = &IconDirChange2;
		m_Ext = _T("ICO");
		break;

	case IDF_MAP:
		m_RootPath = MapPath;
		m_RootPath2 = MapPath2;
		m_RootPath3 = MapPath3;
		m_RootPath3Key = _T("Map");
		m_pDirChange = &MapDirChange;
		m_pDirChange2 = &MapDirChange2;
		m_Ext = _T("MAP");
		break;

	case IDF_PIC:
		m_RootPath = PicturePath;
		m_RootPath2 = PicturePath2;
		m_RootPath3 = PicturePath3;
		m_RootPath3Key = _T("Picture");
		m_pDirChange = &PictureDirChange;
		m_pDirChange2 = &PictureDirChange2;
		m_Ext = _T("BMP");
		m_Ext2 = _T("JPG");
		break;

	case IDF_SND:
		m_RootPath = SoundPath;
		m_RootPath2 = SoundPath2;
		m_RootPath3 = SoundPath3;
		m_RootPath3Key = _T("Sound");
		m_pDirChange = &SoundDirChange;
		m_pDirChange2 = &SoundDirChange2;
		m_Ext = _T("WAV");
		m_Ext2 = _T("MP3");
		break;

	case IDF_MUS:
		m_RootPath = MusicPath;
		m_RootPath2 = MusicPath2;
		m_RootPath3 = MusicPath3;
		m_RootPath3Key = _T("Music");
		m_pDirChange = &MusicDirChange;
		m_pDirChange2 = &MusicDirChange2;
		m_Ext = _T("MID");
		m_Ext2 = _T("RMI");
		break;

	case IDF_SPRITE:
		m_RootPath = SpritePath;
		m_RootPath2 = SpritePath2;
		m_RootPath3 = SpritePath3;
		m_RootPath3Key = _T("Sprite");
		m_pDirChange = &SpriteDirChange;
		m_pDirChange2 = &SpriteDirChange2;
		m_Ext = _T("SPR");
		break;

	default:
		ASSERTERROR;
	}

	if (m_CD)
	{
		m_Path = m_RootPath2 + m_SubPath;
	}
	else
	{
		if (m_CD2)
		{
			if (m_SubPath.IsEmpty())
			{
				m_RootPath3.Delete(3);
				m_Path = m_RootPath3 + _T("FOLDERS.INI");

				TCHAR buf[512];
				CText txt(buf, ::GetPrivateProfileString(_T("Folders"), m_RootPath3Key, EmptyText, buf, 511, m_Path));

				txt.TrimLeft();
				if ((txt.Length() >= 2) && (txt[1] == _T(':')))
				{
					txt.Delete(0, 2);
					txt.TrimLeft();
				}

				while (txt.IsNotEmpty() && (txt[0] == _T('\\')))
				{
					txt.Delete(0, 1);
					txt.TrimLeft();
				}

				txt.TrimRight();
				while (txt.LastChar() == _T('\\'))
				{
					txt.Delete(txt.Length() - 1, 1);
					txt.TrimRight();
				}
	
				m_RootPath3 += txt;
				if (m_RootPath3.LastChar() != _T('\\')) m_RootPath3 += _T('\\');

				switch (m_Func)
				{
				case IDF_FNC:
					FunctionPath3 = m_RootPath3;
					break;

				case IDF_REAL:
					NumberPath3 = m_RootPath3;
					break;

				case IDF_BOOL:
					BoolPath3 = m_RootPath3;
					break;

				case IDF_TEXT:
					TextPath3 = m_RootPath3;
					break;

				case IDF_ICON:
					IconPath3 = m_RootPath3;
					break;

				case IDF_MAP:
					MapPath3 = m_RootPath3;
					break;

				case IDF_PIC:
					PicturePath3 = m_RootPath3;
					break;

				case IDF_SND:
					SoundPath3 = m_RootPath3;
					break;

				case IDF_MUS:
					MusicPath3 = m_RootPath3;
					break;

				case IDF_SPRITE:
					SpritePath3 = m_RootPath3;
					break;
				}
			}

			m_Path = m_RootPath3 + m_SubPath;
		}
		else
		{
			m_Path = m_RootPath + m_SubPath;
		}
	}

	m_Aliases = m_Path;
	m_Aliases += _T("ALIASES.INI");
}

/////////////////////////////////////////////////////////////////////////////
// naètení ALIAS jména souboru

CText ReadAliasName(int lang, LPCTSTR key)
{
	TCHAR buf[1024];
	return CText(buf, ::GetPrivateProfileString(JazykInfo[lang].LangIDini, key, EmptyText, buf, 1023, m_Aliases));
}


/////////////////////////////////////////////////////////////////////////////
// naètení obsahu položky v oknì tøíd

void Load(int index)
{
// kontrola, zda neprobíhá naèítání
	if (m_Loading) return;

// ovìøení platnosti položky
	if (BufCls.IsNotValid(index)) return;

// nastavení pøíznaku obsluhy
	m_Loading = TRUE;

// zrušení všech potomkù položky
	while (BufCls[index].Child >= 0)
	{
		BufCls.Del(BufCls[index].Child);
	}

// UNDO nelze navracet
	Undo.DelAll();

// pøíprava cesty pro položku
	InitPath(index);

// vytvoøení cesty
	if (!(m_CD || m_CD2))
	{
		CreatePath(m_Path);
	}

// zahájení hlášení zmìn adresáøù, pokud adresáø pøedtím neexistoval
	BeginDirChange(m_pDirChange, m_pDirChange2, m_RootPath, m_RootPath2);

// lokální promìnné
	WIN32_FIND_DATA wfd;			// struktura pro hledání souborù
	HANDLE handle;					// handle hledání souborù
	PROGITEM item;
	CText jmeno;					// jméno nalezeného souboru
	CMultiText jmeno2;				// vícejazyèné jméno souboru
	CIcon icon;						// ikona nalezeného souboru

// pøíprava položky k naètení adresáøù
	MemFill(&item, sizeof(PROGITEM), -1);
	item.Func = IDF_GROUP;
	item.RefBlok = BufStrID;
	item.RefIndex = GroupIndex;
	item.Param = 0;
	if (m_CD)
	{
		item.Param |= PR_CD;
	}

	if (m_CD2)
	{
		item.Param |= PR_CD2;
	}

	item.SrcMask = PR_ALL;
	item.DstMask = PR_ALL;

// zdìdìní parametrù uzamknutí a vypnutí
	if (m_ParParam & (PR_LOCK | PR_LOCK_DEP))
	{
		item.Param |= PR_LOCK_DEP;
	}

	if (m_ParParam & (PR_OFF | PR_OFF_DEP))
	{
		item.Param |= PR_OFF_DEP;
	}

// naètení podadresáøù
	handle = ::FindFirstFile(m_Path + _T("*.*"), &wfd);
	if (handle != INVALID_HANDLE_VALUE)
	{
		do 
		{
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				jmeno = wfd.cFileName;

				item.Param &= ~(PR_LOCK | PR_OFF);

			// naètení atributù podadresáøe
				int atr = (int)::GetFileAttributes(m_Path + jmeno);
				if (atr != -1)
				{
					if (atr & FILE_ATTRIBUTE_READONLY)
					{
						item.Param |= PR_LOCK;
					}

					if (atr & FILE_ATTRIBUTE_HIDDEN)
					{
						item.Param |= PR_OFF;
					}
				}				

			// uzamknutí vzorového adresáøe
				if (m_CD || m_CD2)
				{
					item.Param |= PR_LOCK;
				}

				BOOL txtok = FALSE;

				if ((jmeno != _T(".")) && (jmeno != _T("..")))
				{
					jmeno2.MultiText(JAZYK000, jmeno);

					for (int lang = 0; lang < JAZYKNUM; lang++)
					{
						if (lang != JAZYK000)
						{
							jmeno2.MultiText(lang, ReadAliasName(lang, jmeno));
							if (jmeno2.MultiText(lang).IsNotEmpty()) txtok = TRUE;
						}
					}

					if (!txtok) jmeno2.MultiText(JazykDef, jmeno);

					item.Name = Text.Add(jmeno2);
					BufCls.Insert(&item, index, -4);
				}
			}
			
		} while (::FindNextFile(handle, &wfd));
		::FindClose(handle);
	}

// pøidání alternativního adresáøe
	if (!(m_CD || m_CD2) && (BufCls[index].Parent < 0))
	{
		if (m_RootPath2.IsNotEmpty())
		{
			CText txt;
			txt.Load(IDN_TEXTVZOR);
			item.Param = PR_LOCK | PR_CD;
			item.Name = Text.AddTxt(txt);
			BufCls.Insert(&item, index, -2);
		}

		if (m_RootPath3.IsNotEmpty())
		{
			CText txt;
			txt.Load(IDF_VZOR2);
			item.Param = PR_LOCK | PR_CD2;
			item.Name = Text.AddTxt(txt);
			item.Icon = Icon.Dup(IDF_VZOR2 - IDF);
			BufCls.Insert(&item, index, -2);
		}
	}

// pøíprava položky k naètení souborù
	item.Func = m_Func;
	item.RefBlok = BufIntID;
	item.RefIndex = m_Func - IDF;
	item.Param = BufCls[m_Root].Param & ~(PR_INTERN | PR_EXP | PR_ALTEXT);
	item.SrcMask = BufCls[m_Root].SrcMask;
	item.DstMask = BufCls[m_Root].DstMask;

// zdìdìní parametrù uzamknutí a vypnutí
	if (m_ParParam & (PR_LOCK | PR_LOCK_DEP))
	{
		item.Param |= PR_LOCK_DEP;
	}

	if (m_ParParam & (PR_OFF | PR_OFF_DEP))
	{
		item.Param |= PR_OFF_DEP;
	}

// vyhledání souborù
	handle = ::FindFirstFile(m_Path + _T("*.") + m_Ext, &wfd);
	BOOL errpath = FALSE;

	if (handle != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			{
				jmeno = wfd.cFileName;

				item.Param &= ~(PR_LOCK | PR_OFF);
				int atr = (int)::GetFileAttributes(m_Path + jmeno);
				if (atr != -1)
				{
					if (atr & FILE_ATTRIBUTE_READONLY)
					{
						item.Param |= PR_LOCK;
					}

					if (atr & FILE_ATTRIBUTE_HIDDEN)
					{
						item.Param |= PR_OFF;
					}
				}				

				if (m_CD || m_CD2) 
				{
					item.Param |= PR_LOCK;
				}

				jmeno2.MultiText(JAZYK000, jmeno);

				BOOL txtok = FALSE;

				for (int lang = 0; lang < JAZYKNUM; lang++)
				{
					if (lang != JAZYK000)
					{
						jmeno2.MultiText(lang, ReadAliasName(lang, jmeno));
						if (jmeno2.MultiText(lang).IsNotEmpty()) txtok = TRUE;
					}
				}

				if ((jmeno.Length() > 4) && (jmeno[jmeno.Length() - 4] == _T('.')))
				{
					jmeno.Delete(jmeno.Length()-4);
				}

				if (!txtok) jmeno2.MultiText(JazykDef, jmeno);

				item.Name = Text.Add(jmeno2);

				item.Icon = -1;
				if (icon.LoadFile(m_Path + jmeno + _T(".ICO")))
				{
					item.Icon = Icon.Add(icon);
				}

				BufCls.Insert(&item, index, -4);
			}
			
		} while (::FindNextFile(handle, &wfd));
		::FindClose(handle);
	}
	else
	{
		errpath = (::GetLastError() != ERROR_FILE_NOT_FOUND);
	}

// vyhledání alternativních souborù
	if (m_Ext2.IsNotEmpty())
	{
		handle = ::FindFirstFile(m_Path + _T("*.") + m_Ext2, &wfd);

		if (handle != INVALID_HANDLE_VALUE)
		{
			do
			{
				if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
				{
					jmeno = wfd.cFileName;

					item.Param &= ~(PR_LOCK | PR_OFF);
					item.Param |= PR_ALTEXT;
					int atr = (int)::GetFileAttributes(m_Path + jmeno);
					if (atr != -1)
					{
						if (atr & FILE_ATTRIBUTE_READONLY)
						{
							item.Param |= PR_LOCK;
						}

						if (atr & FILE_ATTRIBUTE_HIDDEN)
						{
							item.Param |= PR_OFF;
						}
					}				

					if (m_CD || m_CD2)
					{
						item.Param |= PR_LOCK;
					}

					jmeno2.MultiText(JAZYK000, jmeno);
	
					BOOL txtok = FALSE;

					for (int lang = 0; lang < JAZYKNUM; lang++)
					{
						if (lang != JAZYK000)
						{
							jmeno2.MultiText(lang, ReadAliasName(lang, jmeno));
							if (jmeno2.MultiText(lang).IsNotEmpty()) txtok = TRUE;
						}
					}

					if ((jmeno.Length() > 4) && (jmeno[jmeno.Length() - 4] == _T('.')))
					{
						jmeno.Delete(jmeno.Length()-4);
					}

					if (!txtok) jmeno2.MultiText(JazykDef, jmeno);

					item.Name = Text.Add(jmeno2);

					item.Icon = -1;
					if (icon.LoadFile(m_Path + jmeno + _T(".ICO")))
					{
						item.Icon = Icon.Add(icon);
					}

					BufCls.Insert(&item, index, -4);
				}
				
			} while (::FindNextFile(handle, &wfd));
			::FindClose(handle);
		}
		else
		{
			if (::GetLastError() != ERROR_FILE_NOT_FOUND)
			{
				errpath = TRUE;
			}
		}
	}

// oprava zobrazení tlaèítka
	TV_ITEM tvi;
	tvi.hItem = BufCls[index].HTree;
	tvi.mask = TVIF_CHILDREN;
	tvi.cChildren = 0;

	if ((BufCls[index].Child >= 0) || errpath)
	{
		tvi.cChildren = 1;
	}
	::SendMessage(BufCls.Tree(), TVM_SETITEM, 0, (LPARAM)&tvi);

// UNDO nelze navracet
	Undo.DelAll();

// vypnutí pøíznaku obsluhy
	m_Loading = FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// zrušení položky z bufferu tøíd (se zrušením souboru)

void Delete(int index)
{
// kontrola platnosti položky
	if (BufCls.IsNotValid(index)) return;

// adresa položky
	PROGITEM* item = &BufCls[index];

// základní kontrola povolení operace
	if ((item->Parent < 0) || 
		(item->Param & (PR_NOMOVE | PR_INTERN | PR_LOCK | PR_LOCK_DEP)))
	{
		return;
	}

// zjištìní potomkù skupiny
	if ((item->Func == IDF_GROUP) && (item->Child < 0))
	{
		Load(index);
		item = &BufCls[index];
	}

// kontrola povolení operace
	if (item->Child >= 0) return;

// dotaz na potvrzení operace
	if (item->Func != IDF_GROUP)
	{
		CText text;
		text.Load(IDN_CLASSDEL);
		CText nadpis;
		nadpis.Load(IDN_CLASSDEL2);

		if (::MessageBox(MainFrame, text, nadpis, 
			MB_OKCANCEL | MB_ICONEXCLAMATION) != IDOK)
		{
			return;
		}
	}

// zapnutí èekacího kurzoru
	BeginWaitCursor();

// pøíprava jména souboru
	InitName(index);

// zrušení souboru (u složky se ovìøí, zda opravdu neobsahuje žádné platné soubory)
	BOOL result;
	if (item->Func == IDF_GROUP)
	{
		WIN32_FIND_DATA wfd;			// struktura pro hledání souborù
		HANDLE handle;					// handle hledání souborù
		result = TRUE;					// povoleno zrušení složky

		handle = ::FindFirstFile(m_Name + _T("\\*.") + m_Ext, &wfd);
		if (handle != INVALID_HANDLE_VALUE)
		{
			::FindClose(handle);
			result = FALSE;
		}

		if (result && m_Ext2.IsNotEmpty())
		{
			handle = ::FindFirstFile(m_Name + _T("\\*.") + m_Ext2, &wfd);
			if (handle != INVALID_HANDLE_VALUE)
			{
				::FindClose(handle);
				result = FALSE;
			}
		}

		if (result)
		{
			::DeleteFile(m_Name + _T("\\ALIASES.INI"));
			result = ::RemoveDirectory(m_Name);
		}
	}
	else
	{
		result = ::DeleteFile(m_Name);
	}

// zrušení ikony
	if (result && m_IconName.IsNotEmpty())
	{
		::DeleteFile(m_IconName);
	}

// zrušení Aliases
	if (result)
	{
		for (int lang = 0; lang < JAZYKNUM; lang++)
		{
			if (lang != JAZYK000)
			{
				::WritePrivateProfileString(JazykInfo[lang].LangIDini, m_NameKey, NULL, m_Aliases);
			}
		}
	}

// ukonèení pøedešlého nahrávání záznamu UNDO operace (naètení knihovny)
	Undo.Stop();

// zrušení položky z bufferu
	if (result)
	{
		BufCls.Del(index);
	}

// UNDO nelze navracet
	Undo.DelAll();

// po nìjakou dobu se ignoruje hlášení zmìn adresáøe
	ProgDirChangeIgnore = 2;

// vypnutí èekacího kurzoru
	EndWaitCursor();

// aktualizace informací o položce, aktualizace menu clipboardu
	ProgAktItem();
}


/////////////////////////////////////////////////////////////////////////////
// kopie položky z jiného okna (musí být platné!)

int Copy(int parent, int pos, int bufID, int src)
{
// zajištìní naètení cílové položky
	if ((BufCls[parent].Child < 0) &&
		((BufCls[parent].Func == IDF_GROUP) || (BufCls[parent].Parent < 0)))
	{
		Load(parent);
	}

// adresa zdrojového bufferu
	CBufProg* buf = BufProg + bufID;

// adresa zdrojové položky
	PROGITEM* item = &buf->At(src);

// ------------------------------------------
// kopie položek v oknì tøíd
	if (bufID == BufClsID)
	{
// zapnutí èekacího kurzoru
		BeginWaitCursor();

// provedení kopie položky
		pos = BufCls.Copy(parent, pos, bufID, src);

		if (pos >= 0)
		{
			item = &buf->At(src);

// zajištìní jména položky
			if (BufCls[pos].Name < 0)
			{
				BufCls[pos].Name = Text.Dup(BufCls.GetText(pos));
			}

// pøíprava jména souboru
			InitName(pos);

// pøíprava jména položky
			CMultiText txt2 = Text.Get(BufCls[pos].Name);
			CText txt = txt2.MultiText();

// oprava jména položky
			txt.KorigShort();

			if (BufCls[pos].Func == IDF_GROUP)
			{
				txt2.MultiText(JAZYK000, txt);
			}
			else
			{
				if (m_AltExt)
				{
					txt2.MultiText(JAZYK000, txt + _T('.') + m_Ext2);
				}
				else
				{
					txt2.MultiText(JAZYK000, txt + _T('.') + m_Ext);
				}
			}
			Text.Set(BufCls[pos].Name, txt2);

			InitName(pos);

			CText altname;
			if (m_AltExt)
			{
				altname = m_Name.Left(m_Name.Length() - 3) + m_Ext;
			}
			else
			{
				altname = m_Name.Left(m_Name.Length() - 3) + m_Ext2;
			}

// existuje soubor nebo adresáø tohoto jména?
			int nn = 1;
			while (((int)::GetFileAttributes(m_Name) != -1) ||
					(m_Ext2.IsNotEmpty() && ((int)::GetFileAttributes(altname) != -1)))
			{

// zvýšení èítaèe indexu jména
				nn++;
				if (nn >= 10000) break;

// sestavení nového jména položky
				CText txt0;

				if (ShortName)
				{
					CText txtint;
					txtint.Int(nn);

					while ((txt.Length() + txtint.Length()) > 8)
					{
						txt.Delete(txt.Length() - 1);
					}
					txt0 = txt + txtint;
				}
				else
				{
					txt0 = txt;
					txt0.SetNumObj(nn);
				}

// zmìna textu pro položku
				if (BufCls[pos].Func == IDF_GROUP)
				{
					txt2.MultiText(JAZYK000, txt0);
				}
				else
				{
					if (m_AltExt)
					{
						txt2.MultiText(JAZYK000, txt0 + _T('.') + m_Ext2);
					}
					else
					{
						txt2.MultiText(JAZYK000, txt0 + _T('.') + m_Ext);
					}
				}
				Text.Set(BufCls[pos].Name, txt2);

// pøíprava nového jména souboru
				InitName(pos);
				if (m_AltExt)
				{
					altname = m_Name.Left(m_Name.Length() - 3) + m_Ext;
				}
				else
				{
					altname = m_Name.Left(m_Name.Length() - 3) + m_Ext2;
				}
			}

// oprava zobrazení jména položky
			TV_ITEM tvi;
			tvi.hItem = BufCls[pos].HTree;
			tvi.mask = TVIF_TEXT;
			tvi.pszText = (LPTSTR)(LPCTSTR)txt2.MultiText();
			::SendMessage(BufCls.Tree(), TVM_SETITEM, 0, (LPARAM)&tvi);

// fyzické provedení kopie souborù
			int dst = pos;
			int nextsrc = BufCls[src].Next;
			int src0 = src;
			while (nextsrc < 0)
			{
				nextsrc = BufCls[src0].Parent;
				src0 = nextsrc;
				if (nextsrc < 0)
				{
					nextsrc = BufCls.First();
					src0 = nextsrc;
				}
				else
				{
					nextsrc = BufCls[nextsrc].Next;
				}
			}

			CText oldname;
			CText oldicon;
			CMultiText mtext;

			do {
				mtext = Text.Get(BufCls.GetText(src));

				InitName(src);
				oldname = m_Name;
				oldicon = m_IconName;

				InitName(dst);

				if (BufCls[src].Func == IDF_GROUP)
				{
					CreatePath(m_Name);
				}
				else
				{
					::CopyFile(oldname, m_Name, TRUE);
					::SetFileAttributes(m_Name, FILE_ATTRIBUTE_NORMAL);

					if (m_IconName.IsNotEmpty())
					{
						::CopyFile(oldicon, m_IconName, TRUE);
						::SetFileAttributes(m_IconName, FILE_ATTRIBUTE_NORMAL);
					}
				}

// zápis aliases
				for (int lang = 0; lang < JAZYKNUM; lang++)
				{
					if (lang != JAZYK000)
					{
						if ((mtext.MultiText(lang)).IsEmpty())
						{
							::WritePrivateProfileString(JazykInfo[lang].LangIDini, m_NameKey, NULL, m_Aliases);
						}
						else
						{
							::WritePrivateProfileString(JazykInfo[lang].LangIDini, m_NameKey, mtext.MultiText(lang), m_Aliases);
						}
					}
				}

				dst = BufCls.NextItem(dst);
				src = BufCls.NextItem(src);

			} while (src != nextsrc);
		}

// UNDO nelze navracet
		Undo.DelAll();

		ProgDirChangeIgnore = 2;

// vypnutí èekacího kurzoru
		EndWaitCursor();

// návrat nové pozice prvku
		return pos;
	}

// ------------------------------------------
// vytvoøení adresáøe
	if ((item->Func == IDF_GROUP) && (item->Child < 0) && 
		((BufCls[parent].Func == IDF_GROUP) || (BufCls[parent].Parent < 0)))
	{

// zapnutí èekacího kurzoru
		BeginWaitCursor();

// provedení kopie položky
		pos = BufCls.Copy(parent, pos, bufID, src);

		if (pos >= 0)
		{

// zajištìní jména položky
			if (BufCls[pos].Name < 0)
			{
				BufCls[pos].Name = Text.Dup(BufCls.GetText(pos));
			}

// pøíprava jména souboru
			InitName(pos);

// pøíprava jména položky
			CMultiText txt2 = Text.Get(BufCls[pos].Name);
			CText txt = txt2.MultiText();

// oprava jména položky
			txt.KorigShort();

			txt2.MultiText(JAZYK000, txt);
			Text.Set(BufCls[pos].Name, txt2);

			InitName(pos);

// existuje soubor nebo adresáø tohoto jména?
			int nn = 1;
			while ((int)::GetFileAttributes(m_Name) != -1)
			{

// zvýšení èítaèe indexu jména
				nn++;
				if (nn >= 10000) break;

// sestavení nového jména položky
				CText txt0;

				if (ShortName)
				{
					CText txtint;
					txtint.Int(nn);

					while ((txt.Length() + txtint.Length()) > 8)
					{
						txt.Delete(txt.Length() - 1);
					}
					txt0 = txt + txtint;
				}
				else
				{
					txt0 = txt;
					txt0.SetNumObj(nn);
				}

// zmìna textu pro položku
				txt2.MultiText(JAZYK000, txt0);
				Text.Set(BufCls[pos].Name, txt2);

// pøíprava nového jména souboru
				InitName(pos);
			}

// vytvoøení zadaného adresáøe
			CreatePath(m_Name);

// oprava zobrazení jména položky
			TV_ITEM tvi;
			tvi.hItem = BufCls[pos].HTree;
			tvi.mask = TVIF_CHILDREN | TVIF_TEXT;
			tvi.pszText = (LPTSTR)(LPCTSTR)txt2.MultiText();
			tvi.cChildren = 0;
			::SendMessage(BufCls.Tree(), TVM_SETITEM, 0, (LPARAM)&tvi);

// zápis aliases
			CMultiText txt3 = Text.Get(BufInt.GetText(BufCls[pos].Func - IDF));

			for (int lang = 0; lang < JAZYKNUM; lang++)
			{
				if (lang != JAZYK000)
				{
					if (((txt2.MultiText(lang)).IsEmpty()) || 
						(!MultiJazyk && !JazykInfo[lang].InstOK && (lang != Jazyk) &&
								(txt2.MultiText(lang) == txt3.MultiText(lang))))
					{
						::WritePrivateProfileString(JazykInfo[lang].LangIDini, m_NameKey, NULL, m_Aliases);
					}
					else
					{
						::WritePrivateProfileString(JazykInfo[lang].LangIDini, m_NameKey, txt2.MultiText(lang), m_Aliases);
					}
				}
			}
		}

// UNDO nelze navracet
		Undo.DelAll();

		ProgDirChangeIgnore = 2;

// vypnutí èekacího kurzoru
		EndWaitCursor();

// návrat nové pozice prvku
		return pos;
	}

// ------------------------------------------
// bude vytvoøení datové položky
	if ((item->DatBlok >= 0) && (item->DatIndex >= 0) && ((item->Child < 0) || (item->Func == IDF_SPRITE)))
	{

// zapnutí èekacího kurzoru
		BeginWaitCursor();

// provedení kopie položky
		pos = BufCls.Copy(parent, pos, bufID, src);

		if (pos >= 0)
		{

// zajištìní jména položky
			if (BufCls[pos].Name < 0)
			{
				BufCls[pos].Name = Text.Dup(BufCls.GetText(pos));
			}

// pøíprava jména souboru
			InitName(pos);

// pøíprava jména položky
			CMultiText txt2 = Text.Get(BufCls[pos].Name);
			CText txt = txt2.MultiText();

// oprava jména položky
			txt.KorigShort();

			txt2.MultiText(JAZYK000, txt + _T('.') + m_Ext);
			Text.Set(BufCls[pos].Name, txt2);

			InitName(pos);

			CText altname;
			altname = m_Name.Left(m_Name.Length() - 3) + m_Ext2;

// existuje soubor nebo adresáø tohoto jména?
			int nn = 1;
			while (((int)::GetFileAttributes(m_Name) != -1) ||
					(m_Ext2.IsNotEmpty() && ((int)::GetFileAttributes(altname) != -1)))
			{

// zvýšení èítaèe indexu jména
				nn++;
				if (nn >= 10000) break;

// sestavení nového jména položky
				CText txt0;

				if (ShortName)
				{
					CText txtint;
					txtint.Int(nn);

					while ((txt.Length() + txtint.Length()) > 8)
					{
						txt.Delete(txt.Length() - 1);
					}
					txt0 = txt + txtint;
				}
				else
				{
					txt0 = txt;
					txt0.SetNumObj(nn);
				}

// zmìna textu pro položku
				txt2.MultiText(JAZYK000, txt0 + _T('.') + m_Ext);
				Text.Set(BufCls[pos].Name, txt2);

// pøíprava nového jména souboru
				InitName(pos);
				altname = m_Name.Left(m_Name.Length() - 3) + m_Ext2;
			}

// oprava zobrazení jména položky
			TV_ITEM tvi;
			tvi.hItem = BufCls[pos].HTree;
			tvi.mask = TVIF_CHILDREN | TVIF_TEXT;
			tvi.pszText = (LPTSTR)(LPCTSTR)txt2.MultiText();
			tvi.cChildren = 0;
			::SendMessage(BufCls.Tree(), TVM_SETITEM, 0, (LPARAM)&tvi);

// uložení dat souboru
			switch (item->DatBlok)
			{
			case BufNumID:
				Real.SaveFile(Real.Get(item->DatIndex), m_Name);
				break;

			case BufTxtID:
				Text.GetTxt(item->DatIndex).SaveFile(m_Name);
				break;

			case BufLogID:
				Bool.SaveFile(Bool.Get(item->DatIndex), m_Name);
				break;

			case BufIcoID:
				Icon.Get(item->DatIndex).SaveFile(m_Name);
				break;

			case BufMapID:
				Map.Get(item->DatIndex).SaveFile(m_Name);
				break;

			case BufPicID:
				Picture.Get(item->DatIndex).SaveFile(m_Name);
				break;

			case BufSprID:
				Sprite.Get(item->DatIndex).SaveFile(m_Name);
				break;

			case BufSndID:
				Sound.Get(item->DatIndex).SaveFile(m_Name);
				break;

			case BufMusID:
				Music.Get(item->DatIndex).SaveFile(m_Name);
				break;
			}

// uložení ikony k souboru
			PROGITEM* newitem = &BufCls[pos];
			if ((item->Icon >= 0) && (newitem->Icon < 0))
			{
				if (item->DatBlok == BufIcoID)
				{
					newitem->Icon = Icon.Dup(item->DatIndex);
					newitem->DatIndex = newitem->Icon;
				}
				else
				{
					newitem->Icon = Icon.Dup(BufCls.GetIcon(pos));
				}
			}

			if ((newitem->Icon >= 0) && (m_IconName.IsNotEmpty()))
			{
				Icon.Get(newitem->Icon).SaveFile(m_IconName);
			}

// zápis aliases
			CMultiText txt3 = Text.Get(BufInt.GetText(BufCls[pos].Func - IDF));

			for (int lang = 0; lang < JAZYKNUM; lang++)
			{
				if (lang != JAZYK000)
				{
					if (((txt2.MultiText(lang)).IsEmpty()) || 
						(!MultiJazyk && !JazykInfo[lang].InstOK && (lang != Jazyk) &&
								(txt2.MultiText(lang) == txt3.MultiText(lang))))
					{
						::WritePrivateProfileString(JazykInfo[lang].LangIDini, m_NameKey, NULL, m_Aliases);
					}
					else
					{
						::WritePrivateProfileString(JazykInfo[lang].LangIDini, m_NameKey, txt2.MultiText(lang), m_Aliases);
					}
				}
			}
		}

// UNDO nelze navracet
		Undo.DelAll();

		ProgDirChangeIgnore = 2;

// vypnutí èekacího kurzoru
		EndWaitCursor();

// návrat nové pozice prvku
		return pos;
	}

	return -1;
}


/////////////////////////////////////////////////////////////////////////////
// pøesun položky v oknì

int Move(int parent, int pos, int src)
{
// zajištìní naètení cílové položky
	if ((BufCls[parent].Child < 0) &&
		((BufCls[parent].Func == IDF_GROUP) || (BufCls[parent].Parent < 0)))
	{
		Load(parent);
	}

// zapnutí èekacího kurzoru
	BeginWaitCursor();

// úschova starého jména položky
	InitName(src);
	CText oldname = m_Name;
	CText oldicon = m_IconName;
	CText oldalias = m_Aliases;
	CText oldkey = m_NameKey;

// provedení pøesunu položky
	pos = BufCls.Move(parent, pos, src);

	if ((pos >= 0) && (BufCls[pos].Name >= 0))
	{

// pøíprava nového jména položky
		InitName(pos);
		CMultiText txt2 = Text.Get(BufCls[pos].Name);
		CText txt = txt2.MultiText();
		txt.KorigShort();

		if (BufCls[pos].Func == IDF_GROUP)
		{
			txt2.MultiText(JAZYK000, txt);
		}
		else
		{
			if (m_AltExt)
			{
				txt2.MultiText(JAZYK000, txt + _T('.') + m_Ext2);
			}
			else
			{
				txt2.MultiText(JAZYK000, txt + _T('.') + m_Ext);
			}
		}
		Text.Set(BufCls[pos].Name, txt2);

		InitName(pos);						// pøíprava jména souboru

		CText altname;
		if (m_AltExt)
		{
			altname = m_Name.Left(m_Name.Length() - 3) + m_Ext;
		}
		else
		{
			altname = m_Name.Left(m_Name.Length() - 3) + m_Ext2;
		}

// existuje soubor nebo adresáø tohoto jména?
		int nn = 1;
		while (((int)::GetFileAttributes(m_Name) != -1) ||
				(m_Ext2.IsNotEmpty() && ((int)::GetFileAttributes(altname) != -1)))
		{

// zvýšení èítaèe indexu jména
			nn++;
			if (nn >= 10000) break;

// sestavení nového jména položky
			CText txt0;

			if (ShortName)
			{
				CText txtint;
				txtint.Int(nn);

				while ((txt.Length() + txtint.Length()) > 8)
				{
					txt.Delete(txt.Length() - 1);
				}
				txt0 = txt + txtint;
			}
			else
			{
				txt0 = txt;
				txt0.SetNumObj(nn);
			}

// zmìna textu pro položku
			if (BufCls[pos].Func == IDF_GROUP)
			{
				txt2.MultiText(JAZYK000, txt0);
			}
			else
			{
				if (m_AltExt)
				{
					txt2.MultiText(JAZYK000, txt0 + _T('.') + m_Ext2);
				}
				else
				{
					txt2.MultiText(JAZYK000, txt0 + _T('.') + m_Ext);
				}
			}
			Text.Set(BufCls[pos].Name, txt2);

// pøíprava nového jména souboru
			InitName(pos);
			if (m_AltExt)
			{
				altname = m_Name.Left(m_Name.Length() - 3) + m_Ext;
			}
			else
			{
				altname = m_Name.Left(m_Name.Length() - 3) + m_Ext2;
			}
		}

// pøejmenování položky
		if (::MoveFile(oldname, m_Name))
		{
			if (m_IconName.IsNotEmpty())
			{
				::MoveFile(oldicon, m_IconName);
			}
		}

// oprava zobrazení jména položky
		TV_ITEM tvi;
		tvi.hItem = BufCls[pos].HTree;
		tvi.mask = TVIF_TEXT;
		tvi.pszText = (LPTSTR)(LPCTSTR)txt2.MultiText();
		::SendMessage(BufCls.Tree(), TVM_SETITEM, 0, (LPARAM)&tvi);

// oprava aliases
		for (int lang = 0; lang < JAZYKNUM; lang++)
		{
			if (lang != JAZYK000)
			{
				::WritePrivateProfileString(JazykInfo[lang].LangIDini, oldkey, NULL, oldalias);

				if ((txt2.MultiText(lang)).IsEmpty())
				{
					::WritePrivateProfileString(JazykInfo[lang].LangIDini, m_NameKey, NULL, m_Aliases);
				}
				else
				{
					::WritePrivateProfileString(JazykInfo[lang].LangIDini, m_NameKey, txt2.MultiText(lang), m_Aliases);
				}
			}
		}

// UNDO nelze navracet
		Undo.DelAll();

		ProgDirChangeIgnore = 2;
	}

// vypnutí èekacího kurzoru
	EndWaitCursor();

// návrat nové pozice prvku
	return pos;
}


};
