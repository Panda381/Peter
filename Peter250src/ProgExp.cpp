
#include "Main.h"

/***************************************************************************\
*																			*
*								Export a import								*
*																			*
\***************************************************************************/

/*
namespace ProgExp
{
	TCHAR* m_Filtr = NULL;			// filtr souborù k zadání v dialogu
	OPENFILENAME m_Ofn;				// struktura pro dialog
	CText m_Nadpis;					// nadpis okna dialogu
	CText m_Path;					// výchozí adresáø
	CText m_Name;					// jméno souboru PET
	TCHAR* m_File = NULL;			// buffer k zadání jména souboru
	HANDLE m_Handle = NULL;			// handle souboru
	BYTE* m_Data = NULL;			// datový buffer
	int m_Read = 0;					// ètecí ukazatel z datového bufferu
	int m_Num = 0;					// poèet bajtù v datovém bufferu

	int m_GrpInx = 0;				// index hlavní skupiny (-1 = není)
	int	m_FncInx = 0;				// index hlavní funkce
	int m_MapInx = 0;				// index hlavní plochy
	int m_IcoInx = 0;				// index hlavní ikony
	int m_PetInx = 0;				// index sprajtu Petra
	int m_LucInx = 0;				// index sprajtu Lucky

	CBufText m_NameInt;				// buffer interních jmen
	CBufText m_NameObj;				// buffer globálních jmen
	CBufText m_NameLoc;				// buffer lokálních jmen

#define BUFSIZE 4096				// velikost datového bufferu


/////////////////////////////////////////////////////////////////////////////
// dialog pro výbìr programu k exportu/importu (vrací TRUE=operace OK)
// øetìzec _T("Programy Petra (*.pet)\0*.PET\0Všechny soubory (*.*)\0*.*\0\0")

bool InitFiltr(bool export)
{
// vytvoøení bufferu pro filtr
	MemFree(m_Filtr);
	TCHAR* m_Filtr = (TCHAR*)MemGet(1024);
	if (m_Filtr == NULL) return false;

// text "Programy Petra"
	CText txt;

	txt.Load(IDN_EXPIMP);
	txt.Add(_T(" (*.pet)"));
	txt.Write(m_Filtr);
	int filtrn = txt.Length();

	m_Filtr[filtrn] = 0;
	filtrn++;

// pøidání masky souborù "*.pet"
	txt = _T("*.PET");
	txt.Write(m_Filtr + filtrn);
	filtrn += txt.Length();

	m_Filtr[filtrn] = 0;
	filtrn++;

// pøidání textu "Všechny soubory"
	txt.Load(IDN_VSECHNY_SOUBORY);
	txt.Add(_T(" (*.*)"));
	txt.Write(m_Filtr + filtrn);
	filtrn += txt.Length();

	m_Filtr[filtrn] = 0;
	filtrn++;

// pøidání masky souborù "*.*"
	txt = _T("*.*");
	txt.Write(m_Filtr + filtrn);
	filtrn += txt.Length();

	m_Filtr[filtrn] = 0;
	filtrn++;

// koncová nula
	m_Filtr[filtrn] = 0;

// pøíprava struktury k zadání jména souboru
	MemFill(&m_Ofn, sizeof(m_Ofn), 0);
	m_Ofn.lStructSize = sizeof(m_Ofn);
	m_Ofn.hwndOwner = MainFrame;
	m_Ofn.lpstrFilter = m_Filtr;
	if (export)
	{
		m_Ofn.Flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
	}
	else
	{
		m_Ofn.Flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST;
	}

// pøíprava titulku okna
	if (export)
	{
		m_Nadpis.Load(IDN_EXPORT2);
	}
	else
	{
		m_Nadpis.Load(IDN_IMPORT2);
	}
	m_Ofn.lpstrTitle = m_Nadpis;

// výchozí adresáø
	m_Path = ProgramPath + Cesta;
	m_Ofn.lpstrInitialDir = m_Path;

// pøednastavené jméno souboru
	m_Name = Jmeno + _T(".pet");
	int n = m_Name.Length() + 1;
	if (n < 1024) n = 1024;

// buffer k zadání jména souboru
	MemFree(m_File);
	m_File = (TCHAR*)MemGet(n*sizeof(TCHAR));
	if (m_File == NULL) return false;
	MemFill(m_File, n*sizeof(TCHAR), 0);
	m_Name.Write(m_File);
	m_Ofn.lpstrFile = m_File;
	m_Ofn.nMaxFile = n;

// zadání jména souboru
	if (export)
	{
		if (!::GetSaveFileName(&m_Ofn)) return false;
	}
	else
	{
		if (!::GetOpenFileName(&m_Ofn)) return false;
	}

// korekce jména souboru
	m_Name = m_File;
	m_Name.TrimLeft();
	m_Name.TrimRight();
	if (m_Name.IsEmpty()) return false;

	if (m_Name.RevFind(_T('.')) < 0)
	{
		m_Name += _T(".pet");
	}

// pøíprava cesty do cílového adresáøe
	m_Path.Empty();
	n = m_Name.RevFind('\\');
	if (n >= 0)
	{
		m_Path = m_Name;
		m_Path.Delete(n + 1);
	}

// pøíprava datového bufferu
	MemFree(m_Data);
	m_Data = (BYTE*)MemGet(BUFSIZE);
	if (m_Data == NULL) return false;
	m_Read = 0;
	m_Num = 0;

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zápis výstupního bufferu (vrací FALSE=chyba)

bool WriteFlush()
{
	if (m_Num > 0)
	{
		DWORD writen = 0;
		if (!::WriteFile(m_Handle, m_Data, m_Num, &writen, NULL) || ((int)writen != m_Num))
		{
			return false;
		}
		m_Num = 0;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zápis bajtu do výstupního souboru (vrací FALSE=chyba)

bool WriteByte(BYTE data)
{
	if (m_Num >= BUFSIZE)
	{
		if (!WriteFlush()) return false;
	}

	m_Data[m_Num] = data;
	m_Num++;
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zápis dat do výstupního souboru (vrací FALSE=chyba)

bool Write(BYTE* data, int num)
{
	for (; num > 0; num--)
	{
		if (!WriteByte(*data)) return false;
		data++;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zápis textu do výstupního souboru (vrací FALSE=chyba)

bool WriteText(CText txt)
{
#ifdef _UNICODE
	char* buf = (WCHAR*)MemGet(txt.Length() * sizeof(char) + 1);
	if (buf == NULL) return false;
	txt.Write(buf);
	bool res = Write((BYTE*)buf, txt.Length());
	MemFree(buf);
	return res;
#else
	return Write((BYTE*)(char*)txt.DataData(), txt.Length());
#endif
}


/////////////////////////////////////////////////////////////////////////////
// exportování jednoho prvku (bez potomkù) (vrací FALSE=chyba)

bool ExportItem(PROGITEM* item)
{
// index deklaraèního jména
	int deklbuf = item->RefBlok;
	int deklinx = item->RefIndex;

// z bufferu struktur zmìna na interní buffer
	if (deklbuf == BufStrID)
	{
		deklbuf = BufStr[deklinx].RefBlok;
		deklinx = BufStr[deklinx].RefIndex;
	}

// z bufferu tøíd zmìna na interní buffer
	if (deklbuf == BufClsID)
	{
		deklbuf = BufCls[deklinx].RefBlok;
		deklinx = BufCls[deklinx].RefIndex;
	}

// zjištìní deklaraèní položky
	CText name;
	CBufText* bf;

	switch (deklbuf)
	{
	case BufIntID:
		name = Text.GetTxt(BufInt.GetText(deklinx));
		bf = &m_NameInt;
		break;

	case BufObjID:
		name = Text.GetTxt(BufObj.GetText(deklinx));
		bf = &m_NameObj;
		break;

	case BufLocID:
		name = Text.GetTxt(BufLoc.GetText(deklinx));
		bf = &m_NameLoc;
		break;

	default:
		ASSERTERROR;
		return false;
	}

// test, zda je deklaraèní jméno již známé
	if (bf->At(deklinx).IsEmpty())
	{

// deklaraèní jméno pro interní prvek
//		if (deklbuf == BufIntID)
//		{
//			m_NameInt[deklinx] = InitTabFunc[deklinx].Import;
//		}

// deklaraèní jméno pro globální a lokální buffer
//		else
		{
//			CText name2;
//			for (int j = 0; j < name.Length(); j++)
//			{
//				char znak = name[j];
//				if (znak == 32) znak = _T('_');
//
//
//
//			}

			name = "èeský ÈESKÝ";

			WCHAR* buf = (WCHAR*)MemGet(1024);

			::MultiByteToWideChar(CodePage, 0, name, name.Length(), buf, 500);

			char* buf2 = (char*)MemGet(1024);

			::WideCharToMultiByte(1252, 0, buf, name.Length(), buf2, 500, NULL, NULL);
			
			bf->At(deklinx) = buf;
			MemFree(buf);
			MemFree(buf2);


		}

	}

// exportované jméno
	WriteText(bf->At(deklinx));


	return true;
}


/////////////////////////////////////////////////////////////////////////////
// exportování programu

void OnExport()
{
// dialog k zadání cíle
	if (!InitFiltr(true)) return;

// pøíprava bufferu iterních jmen
	m_NameInt.DelAll();
	for (int i = BufInt.Max(); i > 0; i--)
	{
		m_NameInt.New();
	}

// pøíprava bufferu globálních jmen
	m_NameObj.DelAll();
	for (i = BufObj.Max(); i > 0; i--)
	{
		m_NameObj.New();
	}

// pøíprava bufferu lokálních jmen
	m_NameLoc.DelAll();
	for (i = BufLoc.Max(); i > 0; i--)
	{
		m_NameLoc.New();
	}

// lokální promìnné
	int index;

// vytvoøení souboru
	m_Handle = ::CreateFile(m_Name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_Handle == INVALID_HANDLE_VALUE) goto CHYBA;

// nalezení indexu hlavní funkce
	index = BufLoc.SrcDat(BufEdiID, 0);
	if (index < 0) goto CHYBA;
	m_FncInx = BufObj.SrcDat(BufLocID, index);
	if (m_FncInx < 0) goto CHYBA;

// nalezení indexu hlavní plochy
	m_MapInx = BufObj.SrcDat(BufMapID, 0);
	if (m_MapInx < 0) goto CHYBA;

// nalezení indexu hlavní ikony
	m_IcoInx = BufObj.SrcDat(BufIcoID, 0);
	if (m_IcoInx < 0) goto CHYBA;

// nalezení indexu sprajtu Petra
	m_PetInx = BufObj.SrcDat(BufSprID, 0);
	if (m_PetInx < 0) goto CHYBA;

// nalezení indexu sprajtu Lucky
	m_LucInx = BufObj.SrcDat(BufSprID, 1);
	if (m_LucInx < 0) goto CHYBA;

// uložení hlavní skupiny
	m_GrpInx = BufObj[m_FncInx].Parent;
	if (m_GrpInx >= 0)
	{
		if (!ExportItem(&BufObj[m_GrpInx])) goto CHYBA;
	}





// cyklus pøes všechny globální objekty
//	index = BufObj.First();













// vyprázdnìní zápisového bufferu
	if (!WriteFlush()) goto CHYBA;

// uzavøení souboru
	::CloseHandle(m_Handle);
	m_Handle = INVALID_HANDLE_VALUE;
	return;

// chybové hlášení
CHYBA:
	if (m_Handle != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_Handle);
		m_Handle = INVALID_HANDLE_VALUE;
	}
	::MessageBox(MainFrame, "CHYBA", "CHYBA", 0);
}


/////////////////////////////////////////////////////////////////////////////
// importování programu

void OnImport()
{
// dialog k zadání cíle
	if (!InitFiltr(false)) return;


}




}
*/
