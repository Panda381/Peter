
#include "Main.h"

/***************************************************************************\
*																			*
*								Editor textu								*
*																			*
\***************************************************************************/

namespace EditText
{

/////////////////////////////////////////////////////////////////////////////
// parametry

int		Index = -1;						// editovan� polo�ka
BOOL	Filling = FALSE;				// pln�n� dat
CText	TextRadek;						// text "��dek:"
CText	TextPozice;						// text "pozice:"

HFONT	TextFont0 = NULL;				// nastaven� font

/////////////////////////////////////////////////////////////////////////////
// nastaven� textu editoru

int RichEditData = 0;
CText RichEditText;

DWORD CALLBACK EditStreamCallback(DWORD cookie, BYTE* buf, long num, long* bytes)
{
	int n = RichEditText.Length()*sizeof(TCHAR) - RichEditData;
	if (n > num) n = num;
	if (n < 0) n = 0;
	*bytes = n;
	MemCopy(buf, RichEditText.DataData() + RichEditData/sizeof(TCHAR), n);
	RichEditData += n;

	return 0;
}

void SetEditText(CText text)
{
// vypnut� p�ekreslov�n� okna
	::SendMessage(EditTextWnd, WM_SETREDRAW, FALSE, 0);

// p��prava textov�ho proudu
	RichEditText = text;
	RichEditData = 0;
	EDITSTREAM stream;
	stream.pfnCallback = EditStreamCallback;

// odesl�n� textov�ho proudu
	::SendMessage(EditTextWnd, EM_STREAMIN, SF_TEXT, (LPARAM)&stream);

// obnoven� p�ekreslov�n� okna
	::SendMessage(EditTextWnd, WM_SETREDRAW, TRUE, 0);
	::InvalidateRect(EditTextWnd, NULL, TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// na�ten� textu editoru

DWORD CALLBACK InEditStreamCallback(DWORD cookie, BYTE* buf, long num, long* bytes)
{
	*bytes = num;

	CText txt((TCHAR*)buf, num/sizeof(TCHAR));
	RichEditText.Add(txt);

	return 0;
}

CText GetEditText()
{
	RichEditText.Empty();
	EDITSTREAM stream;
	stream.pfnCallback = InEditStreamCallback;
	::SendMessage(EditTextWnd, EM_STREAMOUT, SF_TEXT, (LPARAM)&stream);
	return RichEditText;
}

/////////////////////////////////////////////////////////////////////////////
// opakovan� inicializace po zm�n� jazyku

void StartReInit()
{
// inicializace statick�ch prom�nn�ch
	TextRadek.Load(IDT_RADEK);
	TextRadek.Add(' ');

	TextPozice = _T(", ");
	CText txt;
	txt.Load(IDT_POZICE);
	TextPozice.Add(txt);
	TextPozice.Add(' ');

// nastaven� fontu
	HFONT oldfont = TextFont0;

	TextFont0 = GetFont(&TextFont);
	::SendMessage(EditTextWnd, WM_SETFONT, (WPARAM)TextFont0, TRUE);

	::InvalidateRect(EditTextWnd, NULL, TRUE);

	DelFont(oldfont);
}

/////////////////////////////////////////////////////////////////////////////
// inicializace p�i startu programu

HINSTANCE RichEditLib = NULL;

void StartInit()
{
	if (RichEditLib == NULL)
	{
		RichEditLib = ::LoadLibrary(_T("RICHED32.DLL"));
	}		

	EditTextWnd = ::CreateWindowEx(
		WS_EX_CLIENTEDGE,				// roz���en� styl
		_T("RichEdit"),					// t��da
		NULL,							// text
		WS_CHILD | ES_AUTOVSCROLL 
			| ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL, // styl
		300,							// X
		200,							// Y
		100,							// ���ka
		30,								// v��ka
		MainFrame,						// rodi�
		NULL,							// ID
		hInstance,						// instance
		NULL);							// data

	::SendMessage(EditTextWnd, EM_SETTEXTMODE, TM_PLAINTEXT, 0);

	PARAFORMAT pf;
	pf.cbSize = sizeof(PARAFORMAT);
	pf.dwMask = PFM_TABSTOPS;
	pf.cTabCount = MAX_TAB_STOPS;
	for (int i = 0; i < MAX_TAB_STOPS; i++)
	{
		pf.rgxTabs[i] = i*960;
	}
	::SendMessage(EditTextWnd, EM_SETPARAFORMAT, 0, (LPARAM)&pf);

	::SendMessage(EditTextWnd, EM_SETTEXTMODE, TM_PLAINTEXT, 0);

	::SendMessage(EditTextWnd, EM_EXLIMITTEXT, 0, (LPARAM)0x10000000);
	::SendMessage(EditTextWnd, EM_SETEVENTMASK, 0, (LPARAM)(ENM_CHANGE | ENM_SELCHANGE));

	::SendMessage(EditTextWnd, EM_FMTLINES, FALSE, 0);

// inicializace statick�ch prom�nn�ch
	StartReInit();
}

/////////////////////////////////////////////////////////////////////////////
// zah�jen� editace (mus� b�t platn� index!)

void BegEdit(int index)
{
// index prvku
	Index = index;
	ASSERT(Text.IsValid(index));

// zobrazen� okna
	::ShowWindow(EditTextWnd, SW_SHOW);

// nastaven� textu
	Filling = TRUE;
	SetEditText(Text.GetTxt(index));
	Filling = FALSE;

// aktualizace voleb bloku
	UpdateClipboard();
}


/////////////////////////////////////////////////////////////////////////////
// p�esun edita�n�ho pole

HDWP MoveEdit(HDWP hdwp)
{
	hdwp = ::DeferWindowPos(hdwp, EditTextWnd, NULL, 
			EditX, EditY, EditWidth, EditHeight,
			SWP_DRAWFRAME | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	return hdwp;
}


/////////////////////////////////////////////////////////////////////////////
// v�b�r v�eho

void SelectAll()
{
// v�b�r v�eho
	::SendMessage(EditTextWnd, EM_SETSEL, 0, (LPARAM)-1);

// aktualizace voleb bloku
	UpdateClipboard();
}

/////////////////////////////////////////////////////////////////////////////
// zru�en� bloku nebo znaku

void Delete()
{
// zji�t�n� ozna�en�ho �seku textu
	CHARRANGE sel;
	::SendMessage(EditTextWnd, EM_EXGETSEL, 0, (LPARAM)&sel);

// nen�-li nic ozna�eno, ru�� se znak za kurzorem (podhozen� kl�vesy DELETE)
	if (sel.cpMin == sel.cpMax)
	{
		::SendMessage(EditTextWnd, WM_KEYDOWN,	VK_DELETE, 0x01530001);
		::SendMessage(EditTextWnd, WM_KEYUP,	VK_DELETE, 0xC1530001);
	}
	else

// jinak zru�en� ozna�en�ho �seku
	{
		::SendMessage(EditTextWnd, WM_CLEAR, 0, 0);
	}

// aktualizace voleb bloku
	UpdateClipboard();
}

/////////////////////////////////////////////////////////////////////////////
// kopie bloku

void Copy()
{
// operace COPY
	::SendMessage(EditTextWnd, WM_COPY, 0, 0);

// aktualizace voleb bloku
	UpdateClipboard();
}

/////////////////////////////////////////////////////////////////////////////
// vyst��en� bloku

void Cut()
{
// operaceCUT
	::SendMessage(EditTextWnd, WM_CUT, 0, 0);

// aktualizace voleb bloku
	UpdateClipboard();
}

/////////////////////////////////////////////////////////////////////////////
// navr�cen� bloku

void Paste()
{
// operace PASTE
	::SendMessage(EditTextWnd, WM_PASTE, 0, 0);

// aktualizace voleb bloku
	UpdateClipboard();
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace voleb bloku a pozice kurzoru

void UpdateClipboard()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

	if ((FocusBuf == BufEdiID) && (EditMode == BufTxtID))
	{
		CHARRANGE sel;
		::SendMessage(EditTextWnd, EM_EXGETSEL, 0, (LPARAM)&sel);

		BOOL enable = (sel.cpMax != sel.cpMin);
		int len = ::GetWindowTextLength(EditTextWnd);

		EnableCommand(IDN_CUT, enable);
		EnableCommand(IDN_COPY, enable);
		EnableCommand(IDN_PASTE, ::IsClipboardFormatAvailable(CF_TEXT));
		EnableCommand(IDN_CLEAR, enable || (sel.cpMin < len));
		EnableCommand(IDN_ALL, len > 0);

		CText txt(TextRadek);
		CText txt2;

		int radek = ::SendMessage(EditTextWnd, EM_LINEFROMCHAR, sel.cpMin, 0);
		txt2.Int(radek+1);
		txt.Add(txt2);

		txt.Add(TextPozice);
		int pozice = sel.cpMin - ::SendMessage(EditTextWnd, EM_LINEINDEX, radek, 0);
		txt2.Int(pozice+1);
		txt.Add(txt2);

		SetStatusText2(txt);
	}
}


/////////////////////////////////////////////////////////////////////////////
// zm�na textu

void OnChange()
{
	if ((FocusBuf == BufEdiID) && 
		(EditMode == BufTxtID) && 
		Text.IsValid(Index) &&
		!Filling)
	{
		Undo.AddTextSet(Index, Text[Index]);
		CText txt;
		txt = GetEditText();
		Text.SetTxt(Index, txt);
		Undo.Stop();
		SetModi();
	}
}


}
