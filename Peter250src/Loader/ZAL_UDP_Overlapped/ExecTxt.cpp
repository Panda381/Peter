
#include "Main.h"

/***************************************************************************\
*																			*
*						Prov�d�n� programu - text							*
*																			*
\***************************************************************************/

#pragma optimize("t", on)			// optimalizace na maxim�ln� rychlost


/////////////////////////////////////////////////////////////////////////////
// na�ten� jm�na souboru

void _fastcall FFileName(CString& text)
{
	FText(text);
	text.TrimLeft();
	text.TrimRight();
}


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� textu stavov�ho ��dku

void _fastcall FGetStatus(CString& text) { text = StatusText; }


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� textu nadpisu okna

void _fastcall FGetCaption(CString& text) { text = CaptionText; }


/***************************************************************************\
*																			*
*									funkce									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// funkce s n�vratem textu

void _fastcall FTextFunc(CString& text)
{
// �schova indexu volan� funkce
	int data = ExecItem[-1].Data;

// �schova indexu prom�nn� s n�vratovou hodnotou
	int res = ExecItem[-1].List;

// inicializace lok�ln�ch prom�nn�ch
	FCommand();

// �schova ukazatele programu
	EXECITEM* oldexe = ExecItem;

// nov� adresa programu
	ExecItem = ProgBuf + data;

// kontrola hloubky vno�en�
	Hloubka--;
	if (Hloubka >= 0)
	{

// vyvol�n� funkce
		FCommand();
	}
	Hloubka++;

// n�vrat adresy programu
	ExecItem = oldexe;

// zru�en� po�adavku o p�eru�en�
	Break &= ~(BREAKFUNC | BREAKWHILE);

// n�vrat v�sledku operace
	text = Text[Text.Num() - res];

// zru�en� lok�ln�ch prom�nn�ch
	FCommand();
}


/***************************************************************************\
*																			*
*								prom�nn�									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// pr�zdn� text

void _fastcall FTextEmpty(CString& text) { text.Empty(); };


/////////////////////////////////////////////////////////////////////////////
// text CR/LF

TCHAR	Textcrlf[3] = {13,10,0};
CString TextCRLF(Textcrlf, 2);

void _fastcall FTextCRLF(CString& text) { text = TextCRLF; };


/////////////////////////////////////////////////////////////////////////////
// glob�ln� text (Data = index)

void _fastcall FTextObj(CString& text)
{
// index textu
	int inx = ExecItem[-1].Data;

// nastaven� textu
	text = Text[inx];
}


/////////////////////////////////////////////////////////////////////////////
// globaln� text v seznamu (Data = index, List = seznam)

void _fastcall FTextObjList(CString& text)
{
// index seznamu
	int list = ExecItem[-1].List;

// index textu, inkrementace indexu seznamu
	int inx = ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// nastaven� textu
	text = Text[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lok�ln� text (Data = index)

void _fastcall FTextLoc(CString& text)
{
// index textu
	int inx = Text.Num() - ExecItem[-1].Data;

// nastaven� textu
	text = Text[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lok�ln� text v seznamu (Data = index, List = seznam)

void _fastcall FTextLocList(CString& text)
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index textu, inkrementace indexu seznamu
	int inx = Text.Num() - ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// nastaven� textu
	text = Text[inx];
}


/***************************************************************************\
*																			*
*								konverze									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// p�evod ��sla na text

void _fastcall FTextFormat(CString& text) { text.Double(FNum()); };


/////////////////////////////////////////////////////////////////////////////
// p�evod ��sla na text - 2 ��slice

void _fastcall FTextFormat2(CString& text)
{
	text.Empty();
	int num = FIntR();
	if (num < 0) num = 0;
	if (num > 99) num = 99;
	text.Add2Dig((char)num);
}


/////////////////////////////////////////////////////////////////////////////
// p�evod ��sla na text - 8 ��slic HEX

void _fastcall FTextFormatH(CString& text) { text.Hex(FIntRN()); };


/////////////////////////////////////////////////////////////////////////////
// p�evod ��sla na znak

void _fastcall FTextChar(CString& text) { text = (TCHAR)FIntR(); }


/////////////////////////////////////////////////////////////////////////////
// p�evod na mal� p�smena

void _fastcall FTextLower(CString& text)
{
	FText(text);
	text.LowerCase();
}


/////////////////////////////////////////////////////////////////////////////
// p�evod na velk� p�smena

void _fastcall FTextUpper(CString& text)
{
	FText(text);
	text.UpperCase();
}


/***************************************************************************\
*																			*
*							obsluha �asu									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// p�evod data na evropsk� form�t

void _fastcall FTimeDateEur(CString& text)
{
// na�ten� �asu
	FNumSysTime();

// dek�dov�n� data
	text.Empty();
	text.Add2DigSpc((char)SystemTime.wDay);
	text += _T('.');
	text.Add2Dig((char)SystemTime.wMonth);
	text += _T('.');
	text.Add4Dig(SystemTime.wYear);
}


/////////////////////////////////////////////////////////////////////////////
// p�evod data na americk� form�t

void _fastcall FTimeDateUsa(CString& text)
{
// na�ten� �asu
	FNumSysTime();

// dek�dov�n� data
	text.Empty();
	text.Add2DigSpc((char)SystemTime.wMonth);
	text += _T('/');
	text.Add2Dig((char)SystemTime.wDay);
	text += _T('/');
	text.Add4Dig(SystemTime.wYear);
}


/////////////////////////////////////////////////////////////////////////////
// p�evod data na japonsk� form�t

void _fastcall FTimeDateJap(CString& text)
{
// na�ten� �asu
	FNumSysTime();

// dek�dov�n� data
	text.Empty();
	text.Add4Dig(SystemTime.wYear);
	text += _T('-');
	text.Add2Dig((char)SystemTime.wMonth);
	text += _T('-');
	text.Add2Dig((char)SystemTime.wDay);
}


/////////////////////////////////////////////////////////////////////////////
// p�evod �asu na form�t 24

void _fastcall FTimeTime24(CString& text)
{
// na�ten� �asu
	FNumSysTime();

// dek�dov�n� �asu
	text.Empty();
	text.Add2DigSpc((char)SystemTime.wHour);
	text += _T(':');
	text.Add2Dig((char)SystemTime.wMinute);
	text += _T(':');
	text.Add2Dig((char)SystemTime.wSecond);
}


/////////////////////////////////////////////////////////////////////////////
// p�evod �asu na form�t 12

void _fastcall FTimeTime12(CString& text)
{
// na�ten� �asu
	FNumSysTime();

// p��prava hodiny
	char h = (char)SystemTime.wHour;
	if (h >= 12) h -= 12;
	if (h == 0) h = 12;

// dek�dov�n� �asu
	text.Empty();
	text.Add2DigSpc(h);
	text += _T(':');
	text.Add2Dig((char)SystemTime.wMinute);
	text += _T(':');
	text.Add2Dig((char)SystemTime.wSecond);

// ozna�en� a/p
	if (SystemTime.wHour >= 12)
	{
		text += _T('p');
	}
	else
	{
		text += _T('a');
	}
}


/***************************************************************************\
*																			*
*							obsluha soubor�									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// na�ten� aktivn�ho adres��e

void _fastcall FGetFileAct(CString& text) { text.GetAktDir(); }


/////////////////////////////////////////////////////////////////////////////
// na�ten� jmenovky disku

void _fastcall FGetDiskLabel(CString& text)
{
// p��prava bufferu
	TCHAR buf[261];
	buf[0] = 0;

// na�ten� jm�na disku
	::GetVolumeInformation(NULL, buf, 260, NULL, NULL, NULL, NULL, 0);

// nastaven� do prom�nn�
	text = buf;
}


/////////////////////////////////////////////////////////////////////////////
// soubor pro �ten�

void _fastcall FGetFileRead(CString& text) { text = FileRead; }


/////////////////////////////////////////////////////////////////////////////
// soubor pro z�pis

void _fastcall FGetFileWrite(CString& text) { text = FileWrite; }


/////////////////////////////////////////////////////////////////////////////
// na�ten� textu zakon�en�ho nulou

void _fastcall FGetFileText0(CString& text)
{
// vypr�zdn�n� textu
	text.Empty();

// na�ten� textu v k�du UNICODE
	if (FileTextUnicode)
	{

// buffer k na�ten� ��sti textu
		WCHAR buf[256];
		int index = 0;

// cyklus a� po bajt 0
		WCHAR znak = FileReadWord();
		while (znak != 0)
		{
			buf[index] = znak;
			index++;
			if (index == 256)
			{
				text.Add(buf, index);
				index = 0;
			}
			znak = FileReadWord();
		}

// z�pis zbytku textu
		text.Add(buf, index);
	}

// na�ten� textu v k�du Windows
	else
	{

// buffer k na�ten� ��sti textu
		char buf[256];
		int index = 0;

// cyklus a� po bajt 0
		BYTE znak = FileReadByte();
		while (znak != 0)
		{
			buf[index] = (char)znak;
			index++;
			if (index == 256)
			{
				text.Add(buf, index);
				index = 0;
			}
			znak = FileReadByte();
		}

// z�pis zbytku textu
		text.Add(buf, index);
	}
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� textu zakon�en�ho CR

void _fastcall FGetFileTextCR(CString& text)
{
// vypr�zdn�n� textu
	text.Empty();

// na�ten� textu v k�du UNICODE
	if (FileTextUnicode)
	{

// buffer k na�ten� ��sti textu
		WCHAR buf[256];
		int index = 0;

// cyklus a� po bajt 0, CR nebo LF
		WCHAR znak = FileReadWord();
		while ((znak != 0) && (znak != 13) && (znak != 10))
		{
			buf[index] = znak;
			index++;
			if (index == 256)
			{
				text.Add(buf, 256);
				index = 0;
			}
			znak = FileReadWord();
		}

// z�pis zbytku textu
		text.Add(buf, index);

// pro znak CR nebo LF se ru�� p�rov� LF nebo CR
		if (znak == 13)
		{
			znak = FileReadWord();
			if (znak != 10) FileReadOff -= 2;
		}
		else
		{
			if (znak == 10)
			{
				znak = FileReadWord();
				if (znak != 13) FileReadOff -= 2;
			}
		}
	}

// na�ten� textu v k�du Windows
	else
	{

// buffer k na�ten� ��sti textu
		char buf[256];
		int index = 0;

// cyklus a� po bajt 0, CR nebo LF
		BYTE znak = FileReadByte();
		while ((znak != 0) && (znak != 13) && (znak != 10))
		{
			buf[index] = (char)znak;
			index++;
			if (index == 256)
			{
				text.Add(buf, 256);
				index = 0;
			}
			znak = FileReadByte();
		}

// z�pis zbytku textu
		text.Add(buf, index);

// pro znak CR nebo LF se ru�� p�rov� LF nebo CR
		if (znak == 13)
		{
			znak = FileReadByte();
			if (znak != 10) FileReadOff--;
		}
		else
		{
			if (znak == 10)
			{
				znak = FileReadByte();
				if (znak != 13) FileReadOff--;
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� textu s pevnou d�lkou

void _fastcall FGetFileTextN(CString& text)
{
// ukazatel indexu znaku
	int index = 0;

// na�ten� textu v k�du UNICODE
	if (FileTextUnicode)
	{

#ifdef _UNICODE

// nastaven� d�lky textu
		text.Length(FileTextNLen);

// na�ten� textu
		for (int i = FileTextNLen; i > 0; i--)
		{
			WCHAR znak = FileReadWord();
			if (znak == 0) znak = ' ';
			text[index] = znak;
			index++;
		}

#else

		WCHAR* buf = (WCHAR*)MemGet(2*FileTextNLen);

// na�ten� textu
		for (int i = FileTextNLen; i > 0; i--)
		{
			WCHAR znak = FileReadWord();
			if (znak == 0) znak = ' ';
			buf[index] = znak;
			index++;
		}
		text.Term();
		text.Init(buf, index);

		MemFree(buf);

#endif

	}

// na�ten� textu v k�du Windows
	else
	{

#ifdef _UNICODE

		char* buf = (char*)MemGet(FileTextNLen);

// na�ten� textu
		for (int i = FileTextNLen; i > 0; i--)
		{
			BYTE znak = FileReadByte();
			if (znak == 0) znak = ' ';
			buf[index] = znak;
			index++;
		}
		text.Term();
		text.Init(buf, index);

		MemFree(buf);

#else

// nastaven� d�lky textu
		text.Length(FileTextNLen);

// na�ten� textu
		for (int i = FileTextNLen; i > 0; i--)
		{
			BYTE znak = FileReadByte();
			if (znak == 0) znak = ' ';
			text[index] = znak;
			index++;
		}

#endif

	}

// odstran�n� mezer z konce textu
	while ((index > 0) && (text[index-1] == ' '))
	{
		index--;
	}
	text.Length(index);
}


/////////////////////////////////////////////////////////////////////////////
// seznam soubor�

void FileList(CString& text)
{
// p��prava masky soubor�
	CString maska = text;
	maska.TrimLeft();
	maska.TrimRight();
	if (maska.IsEmpty()) maska = _T("*.*");		// implicitn� maska
	text.Empty();								// vypr�zdn�n� seznamu soubor�
	CString maska0;								// jedna maska

// cyklus p�es v�echny masky
	do
	{

// p��prava jedn� masky
		int pos = maska.Find(';');
		if (pos < 0) pos = maska.Length();
		maska0 = maska.Left(pos);
		maska0.TrimLeft();
		maska0.TrimRight();
		if (maska0.IsNotEmpty())
		{

// zah�jen� vyhled�v�n� soubor�
			WIN32_FIND_DATA wfd;			// struktura pro hled�n� soubor�
			HANDLE handle = ::FindFirstFile(maska0, &wfd);

// vyhled�n� soubor�
			if (handle != INVALID_HANDLE_VALUE)
			{
				do
				{
					text += wfd.cFileName;
					text += 13;					// CR
					text += 10;					// LF

				} while (::FindNextFile(handle, &wfd));
				::FindClose(handle);
			}
		}

// vypu�t�n� zpracovan� masky
		maska = maska.Mid(pos+1);

// dokud je n�jak� dal�� maska
	} while (maska.IsNotEmpty());
}

void _fastcall FFileListAll(CString& text)
{
	text.Empty();
	FileList(text);
}

void _fastcall FFileList(CString& text)
{
	FText(text);
	FileList(text);
}

/////////////////////////////////////////////////////////////////////////////
// na�ten� alias pro jeden jazyk (vrac� FALSE=nen� alias, text se nezm�n�)

bool _fastcall ReadAlias(CString& text, int lan)
{
// p��prava jm�na skupiny
	InitAliasGroup(lan);

// na�ten� alias
	TCHAR buf[1024];
	buf[0] = 0;
	int len = ::GetPrivateProfileString(AliasGroup, AliasKey, EmptyString, buf, 1023, AliasName);

// inicializace textu
	if (len > 0)
	{
		text.Term();
		text.Init(buf, len);
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// na�ten� alias souboru

void _fastcall FGetAlias(CString& text)
{
// text implicitn� pr�zdn�
	text.Empty();

// test, zda je soubor zadan�
	if (FileRead.IsEmpty())
	{
		FileError = true;
		return;
	}

// p��prava aliases
	InitAlias(FileRead);

// implicitn� aliases
	text = AliasKey;
	int pos = text.RevFind(_T('.'));
	if (pos >= 0) text.Delete(pos);

// test souboru ALIASES.INI
	if ((int)::GetFileAttributes(AliasName) != -1)
	{
		if (!ReadAlias(text, Jazyk) &&
			!ReadAlias(text, JazykDef) &&
			!ReadAlias(text, JAZYKENG))
		{
			for (int i = 0; i < JAZYKNUM; i++)
			{
				if ((i != Jazyk) &&
					(i != JazykDef) &&
					(i != JAZYKENG))
				{
					if (ReadAlias(text, i)) break;
				}
			}
		}
	}
}


/***************************************************************************\
*																			*
*								obsluha kl�vesnice							*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// �ek�n� na znak z kl�vesnice

void _fastcall FKeyCharWait(CString& text)
{
// vypr�zdn�n� textu
	text.Empty();

// �ek�n� na vstup znaku z kl�vesnice (nebo na p�eru�en�)
	while ((Break == 0) && 
			(CharBufRead == CharBufWrite) && 
			(!ConsoleOn))
	{
#ifndef _MINI
		D3Render(true);
#endif // _MINI
		TimeWait(1);
	}

// kontrola, zda byla p�ijata kl�vesa (zda to nebylo jen p�eru�en�)
	if (CharBufRead != CharBufWrite)
	{

// na�ten� kl�vesy
		text = (TCHAR)CharBuf[CharBufRead];

// zv��en� �tec� adresy
		CharBufRead++;
		if (CharBufRead >= KEYBUFSIZE) CharBufRead = 0;
	}
}


/////////////////////////////////////////////////////////////////////////////
// vstup znaku z kl�vesnice

void _fastcall FKeyChar(CString& text)
{
// test, zda byla p�ijata kl�vesa
	if (CharBufRead != CharBufWrite)
	{

// na�ten� kl�vesy
		text = (TCHAR)CharBuf[CharBufRead];

// zv��en� �tec� adresy
		CharBufRead++;
		if (CharBufRead >= KEYBUFSIZE) CharBufRead = 0;
	}
	else
	{

// jinak vypr�zdn�n� textu
		text.Empty();
	}
}


/***************************************************************************\
*																			*
*							obsluha dialog�									*
*																			*
\***************************************************************************/

CString DialogTextNadpis;
CString DialogText1;
CString DialogText2;
CString DialogText3;
CString DialogTextPar;

/////////////////////////////////////////////////////////////////////////////
// obsluha dialogu zad�n� textu

BOOL CALLBACK DialogTextProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
	case WM_INITDIALOG:
		{
			CenterWindow(hWnd, Win[DialogParent].Wnd);
			DialogTextNadpis.SetWindowText(hWnd);
			DialogText1.SetDialogText(hWnd, IDC_TEXT1);
			DialogText2.SetDialogText(hWnd, IDC_TEXT2);
			DialogText3.SetDialogText(hWnd, IDC_TEXT3);
			DialogTextPar.SetDialogText(hWnd, IDC_EDIT);
			CString txt;
			txt = JazykInfo[Jazyk].CancelTxt;
			txt.SetDialogText(hWnd, IDCANCEL);
		}
		return TRUE;

	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
		case 0:
		case 1:
			switch (LOWORD(wParam))
			{
			case IDOK:
				DialogTextPar.GetDialogText(hWnd, IDC_EDIT);
				::EndDialog(hWnd, LOWORD(wParam));
				return TRUE;

			case IDCANCEL:
				DialogTextPar.Empty();
				::EndDialog(hWnd, LOWORD(wParam));
				return TRUE;
			}
			break;
		}
		break;

	}
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// dialog box s n�vratem textu

void _fastcall FDialogText(CString& text)
{
// na�ten� text�
	FText(DialogTextNadpis);
	FText(DialogText1);
	FText(DialogText2);
	FText(DialogText3);
	FText(DialogTextPar);

// vypnut� celoobrazovkov�ho m�du
//	SetFullScreen(false);

#ifndef _MINI
	D3Render(true);
#endif // _MINI

// p�ekreslen� displeje p�ed vyvol�n�m dialogu
	ReDisp();

// vyvol�n� dialogu
	IsOverlay = true;

	::DialogBoxParam(
			hInstance,
			MAKEINTRESOURCE(IDD_TEXT_PARAM),
			Win[DialogParent].Wnd,
			(DLGPROC)DialogTextProc,
			(LPARAM)0);

	IsOverlay = false;
	if (FullScreen) RePaint(&Win[0]);

// navr�cen� hodnota
	text = DialogTextPar;
}

/////////////////////////////////////////////////////////////////////////////
// dialog otev�en�/ulo�en� souboru

void _fastcall FDialogOpenSave(CString& text, bool save)
{
// na�ten� titulku okna
	CString title;
	FText(title);

// na�ten� jmen soubor�
	CString jmena;
	FText(jmena);

// na�ten� masky soubor�
	CString masky;
	FText(masky);

// na�ten� p�ednastaven�ho jm�na souboru
	CString name;
	FText(name);
	name.TrimLeft();
	name.TrimRight();
	name.PathName(0);

// vypnut� celoobrazovkov�ho m�du
//	SetFullScreen(false);

// p�ekreslen� displeje p�ed vyvol�n�m dialogu
	ReDisp();

// p��prava struktury k zad�n� jm�na souboru
	OPENFILENAME ofn;
	MemFill(&ofn, sizeof(ofn), 0);
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = Win[DialogParent].Wnd;

// v�choz� adres��
	CString aktdir;
	int namepos = name.RevFind(_T('\\'));					// nalezen� konce cesty
	if (namepos < 0)
	{
		namepos = name.Find(_T(':'));						// nalezen� alespo� disku
		if (namepos < 0)
		{
			aktdir.GetAktDir();								// pou�ije se aktu�ln� adres��
		}
		else
		{
			aktdir = name.Left(namepos + 1);				// ozna�en� disku
		}
	}
	else
	{
		if ((namepos == 0) || 
			((namepos == 2) && (name.Get(1) == _T(':'))))	// je to ROOT disku?
		{
			aktdir = name.Left(namepos + 1);				// adres�� bude s koncov�m lom�tkem
		}
		else
		{
			aktdir = name.Left(namepos);					// adres�� po koncov� lom�tko
		}
	}

	ofn.lpstrInitialDir = aktdir;							// v�choz� adres��

// odstran�n� cesty ze jm�na souboru
	name.Delete(0, namepos + 1);

// rozd�len� jm�na souboru na jm�no a p��ponu
	CString defext;
	int extpos = name.Find(_T('.'));
	if (extpos >= 0)
	{
		defext = name;
		name.Delete(extpos);
		defext.Delete(0, extpos + 1);
		if (defext.Find(_T('*')) >= 0) defext.Empty();
	}
	CString defext2;			// alternativn� implicitn� p��pona
	int filterindex2 = 0;		// alternativn� index filtru

// p��prava p��pony pro hled�n� v seznamu masek
	CString defextup;
	if (defext.IsNotEmpty())
	{
		ofn.lpstrDefExt = defext;

		defextup = _T('.') + defext + _T(';');
		defextup.UpperCase();
	}

// buffer pro masku soubor�
	TCHAR* maskbuf = NULL;
	int masksize = 0;

// po�et ��dk� zadan�ho seznamu masek
	int l = masky.LineNum();
	int l2 = jmena.LineNum();
	if (l < l2) l = l2;
	if (l <= 0) l = 1;

// na�ten� seznamu masek
	for (int n = 0; n < l;)
	{

// n�zev masky
		CString radek = jmena.GetLine(n);

// o�ez�n� odd�lova�� ze za��tku a konce n�zvu masky
		radek.TrimLeft();
		radek.TrimRight();

// implicitn� n�zev masky
		if (radek.IsEmpty())
		{
			radek = JazykInfo[Jazyk].AllFilesTxt;
			radek += _T(" (*.*)");
		}

// maska soubor�
		CString maska = masky.GetLine(n);
		n++;

// o�ez�n� odd�lova�� ze za��tku a konce masky
		maska.TrimLeft();
		maska.TrimRight();

// o�ez�n� st�edn�ku ze za��tku a konce masky
		while (maska.Get(0) == _T(';')) maska.Delete(0,1);
		while (maska.LastChar() == _T(';')) maska.Delete(maska.Length()-1, 1);

// vypu�t�n� mezer z masky
		for (;;)
		{
			int p = maska.Find(_T(' '));
			if (p < 0) break;
			maska.Delete(p, 1);
		}

// implicitn� maska
		if (maska.IsEmpty()) maska = _T("*.*");

// implicitn� p��pona
		if (defext2.IsEmpty())
		{
			defext2 = maska;
			defext2.Delete(0, defext2.Find(_T('.')) + 1);	// zru�en� ��sti p�ed a s te�kou
			int pos = defext2.Find(_T(';'));				// za��tek dal�� masky
			if (pos >= 0) defext2.Delete(pos);				// zru�en� dal�� masky

			if (defext2.Find(_T('*')) >= 0)
			{
				defext2.Empty();
			}
			else
			{
				filterindex2 = n;

				if (ofn.lpstrDefExt == NULL)				// p��pona nebyla zad�na
				{
					defext = defext2;
					ofn.lpstrDefExt = defext;

					defextup = _T('.') + defext + _T(';');
					defextup.UpperCase();
				}
			}
		}

// vyhled�n� p��pony
		if (defextup.IsNotEmpty())
		{
			CString maska2 = maska + _T(';');
			maska2.UpperCase();
			if (maska2.Find(defextup) >= 0)
			{
				ofn.nFilterIndex = n;
			}
		}

// zv��en� velikosti bufferu masek
		int newsize = masksize + (radek.Length() + maska.Length() + 2)*sizeof(TCHAR);
		maskbuf = (TCHAR*)MemSize(maskbuf, newsize + sizeof(TCHAR));

// p�id�n� n�zvu masky
		radek.Write(maskbuf + masksize/sizeof(TCHAR));
		masksize += (radek.Length() + 1)*sizeof(TCHAR);
		maskbuf[(masksize-1)/sizeof(TCHAR)] = 0;

// p�id�n� masky soubor�
		maska.Write(maskbuf + masksize/sizeof(TCHAR));
		masksize = newsize;
		maskbuf[(masksize-1)/sizeof(TCHAR)] = 0;
	}

// koncov� nula ozna�uj�c� konec seznamu masek
	maskbuf[masksize/sizeof(TCHAR)] = 0;

// buffer seznamu masek
	ofn.lpstrFilter = maskbuf;

// nen�-li nalezena odpov�daj�c� maska, nebude ani implicitn� p��pona
	if (ofn.nFilterIndex == 0)
	{
		if (defext2.IsNotEmpty())
		{
			ofn.lpstrDefExt = defext2;
			ofn.nFilterIndex = filterindex2;
		}
		else
		{
			ofn.lpstrDefExt = NULL;
		}
	}

// p�ednastaven� jm�no souboru
	n = name.Length() + 1;
	if (n < 1024) n = 1024;
	TCHAR* file = (TCHAR*)MemGet(n*sizeof(TCHAR));
	MemFill(file, n*sizeof(TCHAR), 0);
	name.Write(file);
	ofn.lpstrFile = file;
	ofn.nMaxFile = n;

// titulek okna
	if (title.IsEmpty())
	{
		ofn.lpstrTitle = NULL;
	}
	else
	{
		ofn.lpstrTitle = title;
	}

// p��znaky
	ofn.Flags = OFN_ENABLESIZING | OFN_EXPLORER | OFN_HIDEREADONLY | OFN_LONGNAMES |
			OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_SHAREAWARE;

// zad�n� jm�na souboru
	BOOL res;
	IsOverlay = true;
	if (save)
	{
		res = ::GetSaveFileName(&ofn);
	}
	else
	{
		res = ::GetOpenFileName(&ofn);
	}
	IsOverlay = false;
	if (FullScreen) RePaint(&Win[0]);

// na�ten� jm�na zadan�ho souboru (p�i chyb� nebo p�eru�en� bude pr�zdn� text)
	if (res)
	{
		text = file;
	}
	else
	{
		text.Empty();
	}

// zru�en� buffer�
	MemFree(file);
	MemFree(maskbuf);
}

/////////////////////////////////////////////////////////////////////////////
// dialog otev�en� souboru

void _fastcall FDialogOpen(CString& text)
{
	FDialogOpenSave(text, false);
}

/////////////////////////////////////////////////////////////////////////////
// dialog ulo�en� souboru

void _fastcall FDialogSave(CString& text)
{
	FDialogOpenSave(text, true);
}

/////////////////////////////////////////////////////////////////////////////
// dialog vyhled�n� textu

void _fastcall FDialogFind(CString& text)
{
	text.Empty();
}

/////////////////////////////////////////////////////////////////////////////
// dialog v�b�ru barvy

void _fastcall FDialogColor(CString& text)
{
	text.Empty();
}


/***************************************************************************\
*																			*
*								obsluha text�								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// lev� ��st textu

void _fastcall FTextLeft(CString& text)
{
	int znaku = FIntR();
	FText(text);
	text.Delete(znaku);
}


/////////////////////////////////////////////////////////////////////////////
// prav� ��st textu

void _fastcall FTextRight(CString& text)
{
	int pozice = FIntR();
	CString txt;
	FText(txt);
	text = txt.Right(txt.Length() - pozice);
}


/////////////////////////////////////////////////////////////////////////////
// st�edn� ��st textu

void _fastcall FTextMid(CString& text)
{
	int pozice = FIntR();
	int znaku = FIntR();
	CString txt;
	FText(txt);
	text = txt.Mid(pozice, znaku);
}


/////////////////////////////////////////////////////////////////////////////
// ��dek textu

void _fastcall FTextlineGet(CString& text)
{
	int radek = FIntR();

	CString txt;
	FText(txt);

	text = txt.GetLine(radek);
}

/////////////////////////////////////////////////////////////////////////////
// blok textu

void _fastcall FTextBlock(CString& text)
{
	int begline = FIntR();
	int begpos = FIntR();
	int endline = FIntR();
	int endpos = FIntR();

	CString txt;
	FText(txt);

	text = txt.GetBlock(begline, begpos, endline, endpos);
}

/////////////////////////////////////////////////////////////////////////////
// sou�et jednoho textu - ponech� se beze zm�ny

void _fastcall FTextAdd1(CString& text) { FText(text); }


/////////////////////////////////////////////////////////////////////////////
// sou�et text� (Data = po�et text� - 1)

void _fastcall FTextAdd(CString& text)
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data;

// pomocn� buffer textu
	text.Empty();
	CString text2;

// cyklus p�es v�echny texty
	for (; i >= 0; i--)
	{

// na�ten� dal��ho textu
		FText(text2);
		text += text2;
	}
}


/***************************************************************************\
*																			*
*									konzola									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// vstup textu z konzoly

void _fastcall FConsoleIn(CString& text) { ConsoleIn(text); }


/////////////////////////////////////////////////////////////////////////////
// p��kazov� ��dek

void _fastcall FGetCommandLine(CString& text) { text = CommandLine; }

void _fastcall FCommandPar(CString& text)
{
	text.Empty();

// index parametru
	int i = FIntR();
	int pos = 0;
	int beg = 0;
	int len = CommandLine.Length();

// nalezen� parametru
	for (; i >= 0; i--)
	{

// nalezen� za��tku parametru
		while (	(pos < len) && 
				(CommandLine[pos] <= _T(' ')) && 
				(CommandLine[pos] > 0)) 
		{
			pos++;
		}
		beg = pos;

// nalezen� konce parametru
		bool uvoz = false;
		while (	(pos < len) &&
				(	(CommandLine[pos] < 0) ||
					(CommandLine[pos] > _T(' ')) ||
					uvoz
				) )
		{
			if (CommandLine[pos] == _T('"')) uvoz = !uvoz;
			pos++;
		}
	}

// p�enesen� parametru
	text = CommandLine.Mid(beg, pos-beg);

// zru�en� uvozovek
	pos = 0;
	len = text.Length();
	while (pos < len)
	{
		if (text[pos] == _T('"'))
		{
			text.Delete(pos, 1);
			len--;
			if ((pos < len) && (text[pos] != _T('"'))) pos--;
		}
		pos++;
	}
}


/////////////////////////////////////////////////////////////////////////////
// jm�no programu

void _fastcall FExecName(CString& text) { text = ExeFileName; }


/***************************************************************************\
*																			*
*									dialogy									*
*																			*
\***************************************************************************/

CString InRichEditText;

DWORD CALLBACK InEditStreamCallback(DWORD cookie, BYTE* buf, long num, long* bytes)
{
	*bytes = num;

	InRichEditText.Add((TCHAR*)buf, num/sizeof(TCHAR));

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// text okna

void WindowTextValidate()
{
	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];

		if (!item->TextValid)
		{
			switch (item->Typ)
			{
			case WINITEM_LISTBOX:
				{
					item->Text.Empty();
					int i = ::SendMessage(item->Wnd, LB_GETCURSEL, 0, 0);
					if (i >= 0)
					{
						int len = ::SendMessage(item->Wnd, LB_GETTEXTLEN, i, 0);

						if (len > 0)
						{
							TCHAR* buf = (TCHAR*)MemGet((len+1)*sizeof(TCHAR));
							::SendMessage(item->Wnd, LB_GETTEXT, i, (LPARAM)buf);
							item->Text.Term();
							item->Text.Init(buf, len);
							MemFree(buf);
						}
					}
				}
				break;

			case WINITEM_COMBOBOX:
				{
					item->Text.Empty();
					int i = ::SendMessage(item->Wnd, CB_GETCURSEL, 0, 0);
					if (i >= 0)
					{
						int len = ::SendMessage(item->Wnd, CB_GETLBTEXTLEN, i, 0);

						if (len > 0)
						{
							TCHAR* buf = (TCHAR*)MemGet((len+1)*sizeof(TCHAR));
							::SendMessage(item->Wnd, CB_GETLBTEXT, i, (LPARAM)buf);
							item->Text.Term();
							item->Text.Init(buf, len);
							MemFree(buf);
						}
					}
				}
				break;

			case WINITEM_RICHEDIT:
			case WINITEM_RICHMEMO:
				{
					InRichEditText.Empty();
					EDITSTREAM stream;
					stream.pfnCallback = InEditStreamCallback;
					::SendMessage(item->Wnd, EM_STREAMOUT, (item->Typ == WINITEM_RICHEDIT) ? SF_RTF : SF_TEXT, (LPARAM)&stream);
					item->Text = InRichEditText;

				}
				break;

			case WINITEM_TABLE:
				{
					item->Text.Empty();

					int col = item->TabCol;
					int cols = item->TabCols;

					if ((DWORD)col < (DWORD)cols)
					{
						int row = item->TabRow;

						if ((DWORD)row < (DWORD)item->TabRows)
						{
							item->Text = item->TabData[row * cols + col];
						}
					}
				}
				break;

			default:
				item->Text.GetWindowText(item->Wnd);
			}

			item->TextValid = true;
		}
	}
}


void _fastcall FGetWindowText(CString& text)
{
	if (Win.IsValid(WindowID))
	{
		WindowTextValidate();
		text = Win[WindowID].Text;
	}
	else
	{
		text.Empty();
	}
}

/***************************************************************************\
*																			*
*									DirectPlay								*
*																			*
\***************************************************************************/

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

/////////////////////////////////////////////////////////////////////////////
// seznam spojen�

void _fastcall FConnectList(CString& text)
{
	DirectPlayConInit();
	text = DirectPlayConnects;
}

/////////////////////////////////////////////////////////////////////////////
// aktivn� spojen�

void _fastcall FGetConnectAct(CString& text)
{
	text = DirectPlayConnectsAkt;
}

/////////////////////////////////////////////////////////////////////////////
// seznam her

void _fastcall FGameList(CString& text)
{
	DirectPlayEnumGames();
	text = DirectPlayGames;
}


/////////////////////////////////////////////////////////////////////////////
// aktivn� hra

void _fastcall FGetGameAct(CString& text)
{
	text = DirectPlayGamesAkt;
}

/////////////////////////////////////////////////////////////////////////////
// seznam hr���

void _fastcall FPlayerList(CString& text)
{
	DirectPlayEnumPlayers();
	text = DirectPlayPlayers;
}


/////////////////////////////////////////////////////////////////////////////
// aktivn� hr��

void _fastcall FGetPlayerAct(CString& text)
{
	text = DirectPlayPlayersAkt;
}

/////////////////////////////////////////////////////////////////////////////
// na�ten� textu z paketu

void _fastcall FGetGameDataT(CString& text)
{
// nen� nic v bufferu
	if (DirectPlayDataInO >= DirectPlayDataInN)
	{
		text.Empty();
		return;
	}

// na�ten� textu z bufferu
	text = (char*)(DirectPlayDataIn + DirectPlayDataInO);

// posun ukazatel�
	DirectPlayDataInO += text.Length() + 1;
	if (DirectPlayDataInO > DirectPlayDataInN)
	{
		DirectPlayDataInO = DirectPlayDataInN;
	}
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� IP adresy po��ta�e

void _fastcall FGetGameIP(CString& text)
{
// test platnosti IP adresy
	if (HostIPValid <= 0)
	{
		HostIPValid = 3*18;

		// inicializace WSA knihovny
		if (WSAInitialize1())
		{

// p��prava bufferu k na�ten� lok�ln�ho jm�na
			char* buf = (char*)MemGet(512);

// na�ten� lok�ln�ho jm�na
			if (pWSAGetHostName(buf, 511) == 0)
			{
				struct hostent* hst = pWSAGetHostByName(buf);
				if (hst != NULL)
				{

// test, zda je platn� IP adresa
					char** list = hst->h_addr_list;
					if ((list != NULL) && (*list != NULL))
					{

// dek�dov�n� IP adresy
						char FAR* adr = pWSAInetNtoa(**(struct in_addr**)list);
						if (adr != NULL)
						{
							HostIP = adr;
						}
					}	
				}
			}

// uvoln�n� bufferu lok�ln�ho jm�na
			MemFree(buf);
		}
	}

// IP adresa po��ta�e
	text = HostIP;
}

/***************************************************************************\
*																			*
*										UDP									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� vys�lac� IP adresy UDP

void _fastcall FGetUDPSendIP(CString& text)
{
	text = "127.0.0.1";

	if (WSAInitialize1())
	{
		char FAR* adr = pWSAInetNtoa(UDPSendAddr.sin_addr);
		if (adr != NULL) text = adr;
	}
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� p�ijat� IP adresy UDP

void _fastcall FGetUDPRecIP(CString& text)
{
	text = "255.255.255.255";

	if (WSAInitialize1())
	{
		char FAR* adr = pWSAInetNtoa(UDPRecRecAddr.sin_addr);
		if (adr != NULL) text = adr;
	}
}

/////////////////////////////////////////////////////////////////////////////
// p��jem textu UDP

void _fastcall FGetUDPText(CString& text)
{
	int n = RecUDP();
#ifdef UDP_OVER
	CString t((LPCSTR)UDPRecBuf.buf, n);
	t.KorigNul();
	text = t;
	StartRecUDP();
#else
	text.Term();
	text.Init((LPCSTR)UDPRecBuf.buf, n);
	text.KorigNul();
#endif
}

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------



/////////////////////////////////////////////////////////////////////////////
// zji�t�n� u�ivatelsk�ho fontu

void _fastcall FGetUserFont(CString& text)
{
	text = UserFont;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� seznamu font�

int CALLBACK EnumFontFamProc(const LOGFONT* lpelf, const TEXTMETRIC* lpntm, DWORD FontType, LPARAM lParam)
{
	CString txt = lpelf->lfFaceName;
	txt += 13;
	txt += 10;
	FontList += txt;
	return TRUE;
}

void _fastcall FEnumFonts(CString& text)
{
	if (FontList.IsEmpty())
	{
		HDC dc = ::GetDC(MainFrame);
		::EnumFontFamilies(dc, NULL, EnumFontFamProc, 0);
		::ReleaseDC(MainFrame, dc);
	}
	text = FontList;
}


// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

/////////////////////////////////////////////////////////////////////////////
// na�ten� znaku z portu

void _fastcall FGetComText(CString& text)
{
	BYTE n = ComReceive();

//	if (n == 0)
//	{
//		text.Empty();
//	}
//	else
//	{
		switch (Coms[ComAkt].Dcb.ByteSize)
		{
		case 5:
		case 6:
			n |= 0x40;
		}

		text = (char)n;
//	}
}

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------



/***************************************************************************\
*																			*
*								obsluha mixeru								*
*																			*
\***************************************************************************/

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� mix�n�ch za��zen�

void _fastcall FGetMixDevice(CString& text)
{
	MixDevInit();
	text = MixDevT;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� seznamu c�lov�ch sign�l�

void _fastcall FGetMixLineDst(CString& text)
{
	MixDevInit();
	text = MixLineDT;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� seznamu zdrojov�ch sign�l�

void _fastcall FGetMixLineSrc(CString& text)
{
	MixDevInit();
	text = MixLineST;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� seznamu ovl�dac�ch prvk�

void _fastcall FGetMixControl(CString& text)
{
	MixDevInit();
	text = MixCtrlT;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� seznamu hodnot ovl�dac�ch prvk�

void _fastcall FGetMixCtrlList(CString& text)
{
	MixDevInit();
	text = MixValT;
}

/***************************************************************************\
*																			*
*									DDE										*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� seznamu server�

void _fastcall FGetDDEApp(CString& text)
{
// na�ten� seznamu aplikac� DDE
	DDEAppListLoad();

// sestaven� seznamu server� DDE
	text.Empty();

	for (int i = 0; i < DDEServNum; i++)
	{
		if (DDEServList[i].ServHandle != MainFrame)
		{
			text += DDEServList[i].ServName;
			text += TextCRLF;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� seznamu t�mat

void _fastcall FGetDDETopic(CString& text)
{
// p��prava polo�ky serveru
	int i = DDEServAkt;
	if (i < 0)
	{
		i = DDEAppAkt;
		if (i >= 0)
		{
			i = DDEAppList[i].ServFirst;
		}
	}

// nen� platn� server
	text.Empty();
	if (i < 0) return;

// sestaven� seznamu t�mat
	i = DDEServList[i].TopicFirst;

	while (i >= 0)
	{
		DDETOPITEM* topitem = DDETopList + i;
		text += topitem->TopicName;
		text += TextCRLF;

		i = topitem->TopicNext;
	}
}

/***************************************************************************\
*																			*
*									Direct3D								*
*																			*
\***************************************************************************/

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� seznamu videom�d�

void _fastcall FGet3DMode(CString& text)
{
// inicializace seznamu videom�d�
	InitVideoModes();

	text.Empty();

	CString txt2;
	CString txt3;

	for (int i = 0; i < NumVideoModes; i++)
	{
		int width = TabVideoModes[i].Width;
		int height = TabVideoModes[i].Height;
		int bits = TabVideoModes[i].Bits;

		txt2.Int(width);
		txt2 += _T('*');
		txt3.Int(height);
		txt2 += txt3;
		txt2 += _T('/');
		txt3.Int(bits);
		txt2 += txt3;
		if ((width == ScreenWidth0) &&
			(height == ScreenHeight0) &&
			(bits == ScreenBits))
		{
			txt2 += _T('<');
		}

		txt2 += _T(13);
		txt2 += _T(10);
		text += txt2;
	}
}

/***************************************************************************\
*																			*
*								obsluha DLL									*
*																			*
\***************************************************************************/

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

/////////////////////////////////////////////////////////////////////////////
// text zakon�en� nulou

void _fastcall FGetMemoryText0(CString& text)
{
	char* buf = (char*)DLLMemoryRead;
	text = buf;
	buf += text.Length() + 1;
	DLLMemoryRead = buf;
}

/////////////////////////////////////////////////////////////////////////////
// ukazatel na text zakon�en� nulou

void _fastcall FGetMemoryText0P(CString& text)
{
	char** buf = (char**)DLLMemoryRead;
	text = *buf;
	buf++;
	DLLMemoryRead = buf;
}

/////////////////////////////////////////////////////////////////////////////
// text s pevnou d�lkou

void _fastcall FGetMemoryTextN(CString& text)
{
	text.Term();
	char* buf = (char*)DLLMemoryRead;
	text.Init(buf, DLLMemoryTextNLen);
	buf += DLLMemoryTextNLen;
	DLLMemoryRead = buf;
}

/***************************************************************************\
*																			*
*								konfigurace									*
*																			*
\***************************************************************************/

#define MAXPROFILELEN 0x0FFF

/////////////////////////////////////////////////////////////////////////////
// na�ten� textu z INI (v textu je ulo�ena implicitn� hodnota)

void _fastcall GetIniProfileString(CString& text)
{
// jm�no sekce
	LPCTSTR section = NULL;
	if (INISection.IsNotEmpty()) section = INISection;

// jm�no hodnoty
	LPCTSTR key = NULL;
	if (INIKey.IsNotEmpty()) key = INIKey;

// p��prava implicitn�ho textu (varov�n� - ve Win95 m��e b�t buffer modifikov�n)
	text.TrimLeft();
	text.TrimRight();
	if (text.Length() > MAXPROFILELEN) text.Delete(MAXPROFILELEN);
	LPTSTR vzor = (LPTSTR)MemGet((text.Length() + 1) * sizeof(TCHAR));
	text.Write(vzor);
	vzor[text.Length()] = 0;

// p��prava c�lov�ho bufferu
	LPTSTR buf = (LPTSTR)MemGet((MAXPROFILELEN + 1) * sizeof(TCHAR));

// na�ten� polo�ky
	int len = ::GetPrivateProfileString(section, key, vzor, buf, MAXPROFILELEN, INIFile);

// ulo�en� seznamu
	if ((section == NULL) || (key == NULL))
	{
		text.Empty();
		LPCTSTR src = buf;

		while (*src != 0)
		{
			CString text2(src);
			text += text2;
			text += TextCRLF;
			src += text2.Length() + 1;
		}
	}

// ulo�en� polo�ky, nen�-li seznam
	else
	{
		text.Term();
		text.Init(buf, len);
		text.KorigNul();
	}

	MemFree(buf);
}

/////////////////////////////////////////////////////////////////////////////
// jm�no konfigura�n�ho souboru

void _fastcall FGetIniFile(CString& text)
{
	text = INIFile;
}

/////////////////////////////////////////////////////////////////////////////
// jm�no konfigura�n� sekce

void _fastcall FGetIniSection(CString& text)
{
	text = INISection;
}

/////////////////////////////////////////////////////////////////////////////
// jm�no konfigura�n�ho parametru

void _fastcall FGetIniValue(CString& text)
{
	text = INIKey;
}

/////////////////////////////////////////////////////////////////////////////
// �ten� konfigura�n�ho textu

void _fastcall FGetIniString(CString& text)
{
	FText(text);
	GetIniProfileString(text);
}

void _fastcall FGetIniString0(CString& text)
{
	text.Empty();
	GetIniProfileString(text);
}

/////////////////////////////////////////////////////////////////////////////
// kl�� registr�

void _fastcall FGetRegSubkey(CString& text)
{
	text = REGSubkey;
}

/////////////////////////////////////////////////////////////////////////////
// jm�no polo�ky registr�

void _fastcall FGetRegValue(CString& text)
{
	text = REGValue;
}

/////////////////////////////////////////////////////////////////////////////
// �ten� textu z registru

void _fastcall GetRegString(CString& text)
{
// otev�en� kl��e
	HKEY hkey;

	if (::RegOpenKeyEx(
		(HKEY) (REGKey | 0x80000000),
		REGSubkey,
		0,
		KEY_QUERY_VALUE,
		&hkey) == ERROR_SUCCESS)
	{

// na�ten� hodnoty kl��e
		int bufsize = (MAXPROFILELEN + 1) * sizeof(TCHAR);

		LPTSTR buf = (LPTSTR)MemGet((MAXPROFILELEN + 1) * sizeof(TCHAR));

		if (::RegQueryValueEx(
			hkey,
			REGValue,
			0,
			NULL,
			(BYTE*)buf,
			(DWORD*)&bufsize) == ERROR_SUCCESS)
		{
			bufsize /= sizeof(TCHAR);

			if (bufsize > 0)
			{
				text.Term();
				text.Init(buf, bufsize-1);
				text.KorigNul();
			}
		}

		MemFree(buf);

// uzav�en� kl��e
		::RegCloseKey(hkey);
	}
}

void _fastcall FGetRegString(CString& text)
{
	FText(text);
	GetRegString(text);
}

void _fastcall FGetRegString0(CString& text)
{
	text.Empty();
	GetRegString(text);
}

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------
