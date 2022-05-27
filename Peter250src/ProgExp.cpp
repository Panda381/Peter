
#include "Main.h"

/***************************************************************************\
*																			*
*								Export a import								*
*																			*
\***************************************************************************/

/*
namespace ProgExp
{
	TCHAR* m_Filtr = NULL;			// filtr soubor� k zad�n� v dialogu
	OPENFILENAME m_Ofn;				// struktura pro dialog
	CText m_Nadpis;					// nadpis okna dialogu
	CText m_Path;					// v�choz� adres��
	CText m_Name;					// jm�no souboru PET
	TCHAR* m_File = NULL;			// buffer k zad�n� jm�na souboru
	HANDLE m_Handle = NULL;			// handle souboru
	BYTE* m_Data = NULL;			// datov� buffer
	int m_Read = 0;					// �tec� ukazatel z datov�ho bufferu
	int m_Num = 0;					// po�et bajt� v datov�m bufferu

	int m_GrpInx = 0;				// index hlavn� skupiny (-1 = nen�)
	int	m_FncInx = 0;				// index hlavn� funkce
	int m_MapInx = 0;				// index hlavn� plochy
	int m_IcoInx = 0;				// index hlavn� ikony
	int m_PetInx = 0;				// index sprajtu Petra
	int m_LucInx = 0;				// index sprajtu Lucky

	CBufText m_NameInt;				// buffer intern�ch jmen
	CBufText m_NameObj;				// buffer glob�ln�ch jmen
	CBufText m_NameLoc;				// buffer lok�ln�ch jmen

#define BUFSIZE 4096				// velikost datov�ho bufferu


/////////////////////////////////////////////////////////////////////////////
// dialog pro v�b�r programu k exportu/importu (vrac� TRUE=operace OK)
// �et�zec _T("Programy Petra (*.pet)\0*.PET\0V�echny soubory (*.*)\0*.*\0\0")

bool InitFiltr(bool export)
{
// vytvo�en� bufferu pro filtr
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

// p�id�n� masky soubor� "*.pet"
	txt = _T("*.PET");
	txt.Write(m_Filtr + filtrn);
	filtrn += txt.Length();

	m_Filtr[filtrn] = 0;
	filtrn++;

// p�id�n� textu "V�echny soubory"
	txt.Load(IDN_VSECHNY_SOUBORY);
	txt.Add(_T(" (*.*)"));
	txt.Write(m_Filtr + filtrn);
	filtrn += txt.Length();

	m_Filtr[filtrn] = 0;
	filtrn++;

// p�id�n� masky soubor� "*.*"
	txt = _T("*.*");
	txt.Write(m_Filtr + filtrn);
	filtrn += txt.Length();

	m_Filtr[filtrn] = 0;
	filtrn++;

// koncov� nula
	m_Filtr[filtrn] = 0;

// p��prava struktury k zad�n� jm�na souboru
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

// p��prava titulku okna
	if (export)
	{
		m_Nadpis.Load(IDN_EXPORT2);
	}
	else
	{
		m_Nadpis.Load(IDN_IMPORT2);
	}
	m_Ofn.lpstrTitle = m_Nadpis;

// v�choz� adres��
	m_Path = ProgramPath + Cesta;
	m_Ofn.lpstrInitialDir = m_Path;

// p�ednastaven� jm�no souboru
	m_Name = Jmeno + _T(".pet");
	int n = m_Name.Length() + 1;
	if (n < 1024) n = 1024;

// buffer k zad�n� jm�na souboru
	MemFree(m_File);
	m_File = (TCHAR*)MemGet(n*sizeof(TCHAR));
	if (m_File == NULL) return false;
	MemFill(m_File, n*sizeof(TCHAR), 0);
	m_Name.Write(m_File);
	m_Ofn.lpstrFile = m_File;
	m_Ofn.nMaxFile = n;

// zad�n� jm�na souboru
	if (export)
	{
		if (!::GetSaveFileName(&m_Ofn)) return false;
	}
	else
	{
		if (!::GetOpenFileName(&m_Ofn)) return false;
	}

// korekce jm�na souboru
	m_Name = m_File;
	m_Name.TrimLeft();
	m_Name.TrimRight();
	if (m_Name.IsEmpty()) return false;

	if (m_Name.RevFind(_T('.')) < 0)
	{
		m_Name += _T(".pet");
	}

// p��prava cesty do c�lov�ho adres��e
	m_Path.Empty();
	n = m_Name.RevFind('\\');
	if (n >= 0)
	{
		m_Path = m_Name;
		m_Path.Delete(n + 1);
	}

// p��prava datov�ho bufferu
	MemFree(m_Data);
	m_Data = (BYTE*)MemGet(BUFSIZE);
	if (m_Data == NULL) return false;
	m_Read = 0;
	m_Num = 0;

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// z�pis v�stupn�ho bufferu (vrac� FALSE=chyba)

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
// z�pis bajtu do v�stupn�ho souboru (vrac� FALSE=chyba)

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
// z�pis dat do v�stupn�ho souboru (vrac� FALSE=chyba)

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
// z�pis textu do v�stupn�ho souboru (vrac� FALSE=chyba)

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
// exportov�n� jednoho prvku (bez potomk�) (vrac� FALSE=chyba)

bool ExportItem(PROGITEM* item)
{
// index deklara�n�ho jm�na
	int deklbuf = item->RefBlok;
	int deklinx = item->RefIndex;

// z bufferu struktur zm�na na intern� buffer
	if (deklbuf == BufStrID)
	{
		deklbuf = BufStr[deklinx].RefBlok;
		deklinx = BufStr[deklinx].RefIndex;
	}

// z bufferu t��d zm�na na intern� buffer
	if (deklbuf == BufClsID)
	{
		deklbuf = BufCls[deklinx].RefBlok;
		deklinx = BufCls[deklinx].RefIndex;
	}

// zji�t�n� deklara�n� polo�ky
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

// test, zda je deklara�n� jm�no ji� zn�m�
	if (bf->At(deklinx).IsEmpty())
	{

// deklara�n� jm�no pro intern� prvek
//		if (deklbuf == BufIntID)
//		{
//			m_NameInt[deklinx] = InitTabFunc[deklinx].Import;
//		}

// deklara�n� jm�no pro glob�ln� a lok�ln� buffer
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

			name = "�esk� �ESK�";

			WCHAR* buf = (WCHAR*)MemGet(1024);

			::MultiByteToWideChar(CodePage, 0, name, name.Length(), buf, 500);

			char* buf2 = (char*)MemGet(1024);

			::WideCharToMultiByte(1252, 0, buf, name.Length(), buf2, 500, NULL, NULL);
			
			bf->At(deklinx) = buf;
			MemFree(buf);
			MemFree(buf2);


		}

	}

// exportovan� jm�no
	WriteText(bf->At(deklinx));


	return true;
}


/////////////////////////////////////////////////////////////////////////////
// exportov�n� programu

void OnExport()
{
// dialog k zad�n� c�le
	if (!InitFiltr(true)) return;

// p��prava bufferu itern�ch jmen
	m_NameInt.DelAll();
	for (int i = BufInt.Max(); i > 0; i--)
	{
		m_NameInt.New();
	}

// p��prava bufferu glob�ln�ch jmen
	m_NameObj.DelAll();
	for (i = BufObj.Max(); i > 0; i--)
	{
		m_NameObj.New();
	}

// p��prava bufferu lok�ln�ch jmen
	m_NameLoc.DelAll();
	for (i = BufLoc.Max(); i > 0; i--)
	{
		m_NameLoc.New();
	}

// lok�ln� prom�nn�
	int index;

// vytvo�en� souboru
	m_Handle = ::CreateFile(m_Name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_Handle == INVALID_HANDLE_VALUE) goto CHYBA;

// nalezen� indexu hlavn� funkce
	index = BufLoc.SrcDat(BufEdiID, 0);
	if (index < 0) goto CHYBA;
	m_FncInx = BufObj.SrcDat(BufLocID, index);
	if (m_FncInx < 0) goto CHYBA;

// nalezen� indexu hlavn� plochy
	m_MapInx = BufObj.SrcDat(BufMapID, 0);
	if (m_MapInx < 0) goto CHYBA;

// nalezen� indexu hlavn� ikony
	m_IcoInx = BufObj.SrcDat(BufIcoID, 0);
	if (m_IcoInx < 0) goto CHYBA;

// nalezen� indexu sprajtu Petra
	m_PetInx = BufObj.SrcDat(BufSprID, 0);
	if (m_PetInx < 0) goto CHYBA;

// nalezen� indexu sprajtu Lucky
	m_LucInx = BufObj.SrcDat(BufSprID, 1);
	if (m_LucInx < 0) goto CHYBA;

// ulo�en� hlavn� skupiny
	m_GrpInx = BufObj[m_FncInx].Parent;
	if (m_GrpInx >= 0)
	{
		if (!ExportItem(&BufObj[m_GrpInx])) goto CHYBA;
	}





// cyklus p�es v�echny glob�ln� objekty
//	index = BufObj.First();













// vypr�zdn�n� z�pisov�ho bufferu
	if (!WriteFlush()) goto CHYBA;

// uzav�en� souboru
	::CloseHandle(m_Handle);
	m_Handle = INVALID_HANDLE_VALUE;
	return;

// chybov� hl�en�
CHYBA:
	if (m_Handle != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_Handle);
		m_Handle = INVALID_HANDLE_VALUE;
	}
	::MessageBox(MainFrame, "CHYBA", "CHYBA", 0);
}


/////////////////////////////////////////////////////////////////////////////
// importov�n� programu

void OnImport()
{
// dialog k zad�n� c�le
	if (!InitFiltr(false)) return;


}




}
*/
