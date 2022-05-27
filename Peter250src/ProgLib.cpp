
#include "Main.h"

/***************************************************************************\
*																			*
*									Knihovny								*
*																			*
\***************************************************************************/

namespace ProgLib
{

// prom�nn�
BOOL	m_Loading = FALSE;	// prob�h� obsluha na��t�n�
int		m_Parent;			// index polo�ky k na�ten� obsahu
int		m_ParParam;			// parametry rodi�e polo�ek
int		m_Root;				// ROOT polo�ka v�tve
int		m_Func;				// identifikace funkce
BOOL	m_CD = FALSE;		// p��znak alternativn�ho adres��e (R/O)
BOOL	m_CD2 = FALSE;		// p��znak alternativn�ho adres��e 2 (R/O, CD-ROM)
BOOL	m_CDItem = FALSE;	// p��znak alternativn� polo�ky (m��e to b�t i adres�� v norm�ln�m adres��i)
BOOL	m_CDItem2 = FALSE;	// p��znak alternativn� polo�ky 2 (m��e to b�t i adres�� v norm�ln�m adres��i)
BOOL	m_AltExt = FALSE;	// p��znak alternativn� p��pony (JPG, MP3, RMI, RTF)
CText	m_RootPath;			// v�choz� cesta (s "\" na konci)
CText	m_RootPath2;		// alternativn� cesta (pr�zdn� = nen�, jinak s "\" na konci)
CText	m_RootPath3;		// alternativn� cesta 2 (pr�zdn� = nen�, jinak s "\" na konci)
CText	m_RootPath3Key;		// kl�� ve FOLDERS.INI na CD-ROM
CText	m_SubPath;			// podcesta (s "\" na konci nebo pr�zdn�)
CText	m_Path;				// cesta do adres��e (s "\" na konci)
CText	m_Aliases;			// jm�no INI souboru s ALIASES
CText	m_Ext;				// p��pona soubor� 3 znaky
CText	m_Ext2;				// alternativn� p��pona soubor� 3 znaky (pr�zdn� = nen�)
CText	m_Name;				// pln� jm�no souboru
CText	m_NameKey;			// jm�no souboru pou�it� jako kl�� INI (jm�no s p��ponou)
CText	m_IconName;			// jm�no ikony k souboru (pr�zdn� = nen�)

HANDLE*	m_pDirChange = NULL; // adresa hl�en� o zm�n� adres��e
HANDLE*	m_pDirChange2 = NULL; // adresa hl�en� o zm�n� alternativn�ho adres��e

/////////////////////////////////////////////////////////////////////////////
// statick� inicializace obsluhy

void StartInit()
{
}


/////////////////////////////////////////////////////////////////////////////
// p��prava jm�na souboru

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
// p��prava cesty (index = posledn� adres��)

void _fastcall InitPath(int index)
{
// �schova indexu polo�ky
	ASSERT(BufCls.IsValid(index));
	m_Parent = index;

// �schova parametr� rodi�e polo�ek
	m_ParParam = BufCls[index].Param;

// p��znak alternativn�ho adres��e
	m_CD = (m_ParParam & PR_CD);
	m_CD2 = (m_ParParam & PR_CD2);

// nalezen� ROOT polo�ky (a p��prava ��sti adres��e)
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

// �schova typu polo�ky
	m_Func = BufCls[index].Func;

// p��prava adres��e a specifikace polo�ek
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
// na�ten� ALIAS jm�na souboru

CText ReadAliasName(int lang, LPCTSTR key)
{
	TCHAR buf[1024];
	return CText(buf, ::GetPrivateProfileString(JazykInfo[lang].LangIDini, key, EmptyText, buf, 1023, m_Aliases));
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� obsahu polo�ky v okn� t��d

void Load(int index)
{
// kontrola, zda neprob�h� na��t�n�
	if (m_Loading) return;

// ov��en� platnosti polo�ky
	if (BufCls.IsNotValid(index)) return;

// nastaven� p��znaku obsluhy
	m_Loading = TRUE;

// zru�en� v�ech potomk� polo�ky
	while (BufCls[index].Child >= 0)
	{
		BufCls.Del(BufCls[index].Child);
	}

// UNDO nelze navracet
	Undo.DelAll();

// p��prava cesty pro polo�ku
	InitPath(index);

// vytvo�en� cesty
	if (!(m_CD || m_CD2))
	{
		CreatePath(m_Path);
	}

// zah�jen� hl�en� zm�n adres���, pokud adres�� p�edt�m neexistoval
	BeginDirChange(m_pDirChange, m_pDirChange2, m_RootPath, m_RootPath2);

// lok�ln� prom�nn�
	WIN32_FIND_DATA wfd;			// struktura pro hled�n� soubor�
	HANDLE handle;					// handle hled�n� soubor�
	PROGITEM item;
	CText jmeno;					// jm�no nalezen�ho souboru
	CMultiText jmeno2;				// v�cejazy�n� jm�no souboru
	CIcon icon;						// ikona nalezen�ho souboru

// p��prava polo�ky k na�ten� adres���
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

// zd�d�n� parametr� uzamknut� a vypnut�
	if (m_ParParam & (PR_LOCK | PR_LOCK_DEP))
	{
		item.Param |= PR_LOCK_DEP;
	}

	if (m_ParParam & (PR_OFF | PR_OFF_DEP))
	{
		item.Param |= PR_OFF_DEP;
	}

// na�ten� podadres���
	handle = ::FindFirstFile(m_Path + _T("*.*"), &wfd);
	if (handle != INVALID_HANDLE_VALUE)
	{
		do 
		{
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				jmeno = wfd.cFileName;

				item.Param &= ~(PR_LOCK | PR_OFF);

			// na�ten� atribut� podadres��e
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

			// uzamknut� vzorov�ho adres��e
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

// p�id�n� alternativn�ho adres��e
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

// p��prava polo�ky k na�ten� soubor�
	item.Func = m_Func;
	item.RefBlok = BufIntID;
	item.RefIndex = m_Func - IDF;
	item.Param = BufCls[m_Root].Param & ~(PR_INTERN | PR_EXP | PR_ALTEXT);
	item.SrcMask = BufCls[m_Root].SrcMask;
	item.DstMask = BufCls[m_Root].DstMask;

// zd�d�n� parametr� uzamknut� a vypnut�
	if (m_ParParam & (PR_LOCK | PR_LOCK_DEP))
	{
		item.Param |= PR_LOCK_DEP;
	}

	if (m_ParParam & (PR_OFF | PR_OFF_DEP))
	{
		item.Param |= PR_OFF_DEP;
	}

// vyhled�n� soubor�
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

// vyhled�n� alternativn�ch soubor�
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

// oprava zobrazen� tla��tka
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

// vypnut� p��znaku obsluhy
	m_Loading = FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// zru�en� polo�ky z bufferu t��d (se zru�en�m souboru)

void Delete(int index)
{
// kontrola platnosti polo�ky
	if (BufCls.IsNotValid(index)) return;

// adresa polo�ky
	PROGITEM* item = &BufCls[index];

// z�kladn� kontrola povolen� operace
	if ((item->Parent < 0) || 
		(item->Param & (PR_NOMOVE | PR_INTERN | PR_LOCK | PR_LOCK_DEP)))
	{
		return;
	}

// zji�t�n� potomk� skupiny
	if ((item->Func == IDF_GROUP) && (item->Child < 0))
	{
		Load(index);
		item = &BufCls[index];
	}

// kontrola povolen� operace
	if (item->Child >= 0) return;

// dotaz na potvrzen� operace
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

// zapnut� �ekac�ho kurzoru
	BeginWaitCursor();

// p��prava jm�na souboru
	InitName(index);

// zru�en� souboru (u slo�ky se ov���, zda opravdu neobsahuje ��dn� platn� soubory)
	BOOL result;
	if (item->Func == IDF_GROUP)
	{
		WIN32_FIND_DATA wfd;			// struktura pro hled�n� soubor�
		HANDLE handle;					// handle hled�n� soubor�
		result = TRUE;					// povoleno zru�en� slo�ky

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

// zru�en� ikony
	if (result && m_IconName.IsNotEmpty())
	{
		::DeleteFile(m_IconName);
	}

// zru�en� Aliases
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

// ukon�en� p�ede�l�ho nahr�v�n� z�znamu UNDO operace (na�ten� knihovny)
	Undo.Stop();

// zru�en� polo�ky z bufferu
	if (result)
	{
		BufCls.Del(index);
	}

// UNDO nelze navracet
	Undo.DelAll();

// po n�jakou dobu se ignoruje hl�en� zm�n adres��e
	ProgDirChangeIgnore = 2;

// vypnut� �ekac�ho kurzoru
	EndWaitCursor();

// aktualizace informac� o polo�ce, aktualizace menu clipboardu
	ProgAktItem();
}


/////////////////////////////////////////////////////////////////////////////
// kopie polo�ky z jin�ho okna (mus� b�t platn�!)

int Copy(int parent, int pos, int bufID, int src)
{
// zaji�t�n� na�ten� c�lov� polo�ky
	if ((BufCls[parent].Child < 0) &&
		((BufCls[parent].Func == IDF_GROUP) || (BufCls[parent].Parent < 0)))
	{
		Load(parent);
	}

// adresa zdrojov�ho bufferu
	CBufProg* buf = BufProg + bufID;

// adresa zdrojov� polo�ky
	PROGITEM* item = &buf->At(src);

// ------------------------------------------
// kopie polo�ek v okn� t��d
	if (bufID == BufClsID)
	{
// zapnut� �ekac�ho kurzoru
		BeginWaitCursor();

// proveden� kopie polo�ky
		pos = BufCls.Copy(parent, pos, bufID, src);

		if (pos >= 0)
		{
			item = &buf->At(src);

// zaji�t�n� jm�na polo�ky
			if (BufCls[pos].Name < 0)
			{
				BufCls[pos].Name = Text.Dup(BufCls.GetText(pos));
			}

// p��prava jm�na souboru
			InitName(pos);

// p��prava jm�na polo�ky
			CMultiText txt2 = Text.Get(BufCls[pos].Name);
			CText txt = txt2.MultiText();

// oprava jm�na polo�ky
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

// existuje soubor nebo adres�� tohoto jm�na?
			int nn = 1;
			while (((int)::GetFileAttributes(m_Name) != -1) ||
					(m_Ext2.IsNotEmpty() && ((int)::GetFileAttributes(altname) != -1)))
			{

// zv��en� ��ta�e indexu jm�na
				nn++;
				if (nn >= 10000) break;

// sestaven� nov�ho jm�na polo�ky
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

// zm�na textu pro polo�ku
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

// p��prava nov�ho jm�na souboru
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

// oprava zobrazen� jm�na polo�ky
			TV_ITEM tvi;
			tvi.hItem = BufCls[pos].HTree;
			tvi.mask = TVIF_TEXT;
			tvi.pszText = (LPTSTR)(LPCTSTR)txt2.MultiText();
			::SendMessage(BufCls.Tree(), TVM_SETITEM, 0, (LPARAM)&tvi);

// fyzick� proveden� kopie soubor�
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

// z�pis aliases
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

// vypnut� �ekac�ho kurzoru
		EndWaitCursor();

// n�vrat nov� pozice prvku
		return pos;
	}

// ------------------------------------------
// vytvo�en� adres��e
	if ((item->Func == IDF_GROUP) && (item->Child < 0) && 
		((BufCls[parent].Func == IDF_GROUP) || (BufCls[parent].Parent < 0)))
	{

// zapnut� �ekac�ho kurzoru
		BeginWaitCursor();

// proveden� kopie polo�ky
		pos = BufCls.Copy(parent, pos, bufID, src);

		if (pos >= 0)
		{

// zaji�t�n� jm�na polo�ky
			if (BufCls[pos].Name < 0)
			{
				BufCls[pos].Name = Text.Dup(BufCls.GetText(pos));
			}

// p��prava jm�na souboru
			InitName(pos);

// p��prava jm�na polo�ky
			CMultiText txt2 = Text.Get(BufCls[pos].Name);
			CText txt = txt2.MultiText();

// oprava jm�na polo�ky
			txt.KorigShort();

			txt2.MultiText(JAZYK000, txt);
			Text.Set(BufCls[pos].Name, txt2);

			InitName(pos);

// existuje soubor nebo adres�� tohoto jm�na?
			int nn = 1;
			while ((int)::GetFileAttributes(m_Name) != -1)
			{

// zv��en� ��ta�e indexu jm�na
				nn++;
				if (nn >= 10000) break;

// sestaven� nov�ho jm�na polo�ky
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

// zm�na textu pro polo�ku
				txt2.MultiText(JAZYK000, txt0);
				Text.Set(BufCls[pos].Name, txt2);

// p��prava nov�ho jm�na souboru
				InitName(pos);
			}

// vytvo�en� zadan�ho adres��e
			CreatePath(m_Name);

// oprava zobrazen� jm�na polo�ky
			TV_ITEM tvi;
			tvi.hItem = BufCls[pos].HTree;
			tvi.mask = TVIF_CHILDREN | TVIF_TEXT;
			tvi.pszText = (LPTSTR)(LPCTSTR)txt2.MultiText();
			tvi.cChildren = 0;
			::SendMessage(BufCls.Tree(), TVM_SETITEM, 0, (LPARAM)&tvi);

// z�pis aliases
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

// vypnut� �ekac�ho kurzoru
		EndWaitCursor();

// n�vrat nov� pozice prvku
		return pos;
	}

// ------------------------------------------
// bude vytvo�en� datov� polo�ky
	if ((item->DatBlok >= 0) && (item->DatIndex >= 0) && ((item->Child < 0) || (item->Func == IDF_SPRITE)))
	{

// zapnut� �ekac�ho kurzoru
		BeginWaitCursor();

// proveden� kopie polo�ky
		pos = BufCls.Copy(parent, pos, bufID, src);

		if (pos >= 0)
		{

// zaji�t�n� jm�na polo�ky
			if (BufCls[pos].Name < 0)
			{
				BufCls[pos].Name = Text.Dup(BufCls.GetText(pos));
			}

// p��prava jm�na souboru
			InitName(pos);

// p��prava jm�na polo�ky
			CMultiText txt2 = Text.Get(BufCls[pos].Name);
			CText txt = txt2.MultiText();

// oprava jm�na polo�ky
			txt.KorigShort();

			txt2.MultiText(JAZYK000, txt + _T('.') + m_Ext);
			Text.Set(BufCls[pos].Name, txt2);

			InitName(pos);

			CText altname;
			altname = m_Name.Left(m_Name.Length() - 3) + m_Ext2;

// existuje soubor nebo adres�� tohoto jm�na?
			int nn = 1;
			while (((int)::GetFileAttributes(m_Name) != -1) ||
					(m_Ext2.IsNotEmpty() && ((int)::GetFileAttributes(altname) != -1)))
			{

// zv��en� ��ta�e indexu jm�na
				nn++;
				if (nn >= 10000) break;

// sestaven� nov�ho jm�na polo�ky
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

// zm�na textu pro polo�ku
				txt2.MultiText(JAZYK000, txt0 + _T('.') + m_Ext);
				Text.Set(BufCls[pos].Name, txt2);

// p��prava nov�ho jm�na souboru
				InitName(pos);
				altname = m_Name.Left(m_Name.Length() - 3) + m_Ext2;
			}

// oprava zobrazen� jm�na polo�ky
			TV_ITEM tvi;
			tvi.hItem = BufCls[pos].HTree;
			tvi.mask = TVIF_CHILDREN | TVIF_TEXT;
			tvi.pszText = (LPTSTR)(LPCTSTR)txt2.MultiText();
			tvi.cChildren = 0;
			::SendMessage(BufCls.Tree(), TVM_SETITEM, 0, (LPARAM)&tvi);

// ulo�en� dat souboru
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

// ulo�en� ikony k souboru
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

// z�pis aliases
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

// vypnut� �ekac�ho kurzoru
		EndWaitCursor();

// n�vrat nov� pozice prvku
		return pos;
	}

	return -1;
}


/////////////////////////////////////////////////////////////////////////////
// p�esun polo�ky v okn�

int Move(int parent, int pos, int src)
{
// zaji�t�n� na�ten� c�lov� polo�ky
	if ((BufCls[parent].Child < 0) &&
		((BufCls[parent].Func == IDF_GROUP) || (BufCls[parent].Parent < 0)))
	{
		Load(parent);
	}

// zapnut� �ekac�ho kurzoru
	BeginWaitCursor();

// �schova star�ho jm�na polo�ky
	InitName(src);
	CText oldname = m_Name;
	CText oldicon = m_IconName;
	CText oldalias = m_Aliases;
	CText oldkey = m_NameKey;

// proveden� p�esunu polo�ky
	pos = BufCls.Move(parent, pos, src);

	if ((pos >= 0) && (BufCls[pos].Name >= 0))
	{

// p��prava nov�ho jm�na polo�ky
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

		InitName(pos);						// p��prava jm�na souboru

		CText altname;
		if (m_AltExt)
		{
			altname = m_Name.Left(m_Name.Length() - 3) + m_Ext;
		}
		else
		{
			altname = m_Name.Left(m_Name.Length() - 3) + m_Ext2;
		}

// existuje soubor nebo adres�� tohoto jm�na?
		int nn = 1;
		while (((int)::GetFileAttributes(m_Name) != -1) ||
				(m_Ext2.IsNotEmpty() && ((int)::GetFileAttributes(altname) != -1)))
		{

// zv��en� ��ta�e indexu jm�na
			nn++;
			if (nn >= 10000) break;

// sestaven� nov�ho jm�na polo�ky
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

// zm�na textu pro polo�ku
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

// p��prava nov�ho jm�na souboru
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

// p�ejmenov�n� polo�ky
		if (::MoveFile(oldname, m_Name))
		{
			if (m_IconName.IsNotEmpty())
			{
				::MoveFile(oldicon, m_IconName);
			}
		}

// oprava zobrazen� jm�na polo�ky
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

// vypnut� �ekac�ho kurzoru
	EndWaitCursor();

// n�vrat nov� pozice prvku
	return pos;
}


};
