
#include "Main.h"

/***************************************************************************\
*																			*
*								Editor ��sel								*
*																			*
\***************************************************************************/

namespace EditNum
{

/////////////////////////////////////////////////////////////////////////////
// parametry

int		Index = -1;						// editovan� polo�ka
int		DispLeft;						// sou�adnice X edita�n�ho pole
int		DispTop;						// sou�adnice Y edita�n�ho pole
int		DispWidth;						// ���ka edita�n�ho pole
int		DispHeight;						// v��ka edita�n�ho pole

/////////////////////////////////////////////////////////////////////////////
// inicializace p�i startu programu

void StartInit()
{
// vytvo�en� edita�n�ho okna
	EditNumWnd = ::CreateWindowEx(
		WS_EX_CLIENTEDGE,				// roz���en� styl
		_T("EDIT"),						// t��da
		NULL,							// text
		WS_CHILD | ES_AUTOHSCROLL,		// styl
		300,							// X
		200,							// Y
		100,							// ���ka
		30,								// v��ka
		MainFrame,						// rodi�
		NULL,							// ID
		hInstance,						// instance
		NULL);							// data
}


/////////////////////////////////////////////////////////////////////////////
// zah�jen� editace (mus� b�t platn� index!)

void BegEdit(int index)
{
// index prvku
	Index = index;
	ASSERT(Real.IsValid(index));

// zobrazen� okna
	::ShowWindow(EditNumWnd, SW_SHOW);

// p��prava textu
	ReLoad();

// zobrazen�
	Disp();

// aktualizace voleb bloku
	UpdateClipboard();
}


/////////////////////////////////////////////////////////////////////////////
// p�esun edita�n�ho pole

HDWP MoveEdit(HDWP hdwp)
{
	DispWidth = 200;
	if (DispWidth > EditWidth-4) DispWidth = EditWidth-4;
	if (DispWidth < 1) DispWidth = 1;

	DispHeight = 25;
	if (DispHeight > EditHeight-4) DispHeight = EditHeight-4;
	if (DispHeight < 1) DispHeight = 1;

	DispLeft = EditX + (EditWidth - DispWidth)/2;
	DispTop = EditY + (EditHeight - DispHeight)/2;

	hdwp = ::DeferWindowPos(hdwp, EditNumWnd, NULL, 
			DispLeft, DispTop, DispWidth, DispHeight,
			SWP_DRAWFRAME | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	return hdwp;
}


/////////////////////////////////////////////////////////////////////////////
// zobrazen� podkladu kolem editoru

void Disp()
{
// otev�en� kontextu displeje
	HDC dc = ::GetDC(MainFrame);

// p��prava �t�tce k vymaz�n� podkladu
	HBRUSH brush = (HBRUSH)::GetStockObject(LTGRAY_BRUSH); // �t�tec k vymaz�n� plochy

// vymaz�n� plochy naho�e nad editorem
	RECT rc;
	rc.left = EditX + 2;
	rc.right = EditX + EditWidth - 2;
	rc.top = EditY + 2;
	rc.bottom = DispTop;
	if (rc.top < rc.bottom)
	{
		::FillRect(dc, &rc, brush);
	}

// vymaz�n� plochy dole pod editorem (left a right je nastaveno)
	rc.top = DispTop + DispHeight;
	rc.bottom = EditY + EditHeight - 2;
	if (rc.top < rc.bottom)
	{
		::FillRect(dc, &rc, brush);
	}

// vymaz�n� plochy vlevo od editoru (left je nastaveno)
	rc.right = DispLeft;
	rc.top = DispTop;
	rc.bottom = DispTop + DispHeight;
	if (rc.left < rc.right)
	{
		::FillRect(dc, &rc, brush);
	}

// vymaz�n� plochy vpravo od editoru (top a bottom je nastaveno)
	rc.left = DispLeft + DispWidth;
	rc.right = EditX + EditWidth - 2;
	if (rc.left < rc.right)
	{
		::FillRect(dc, &rc, brush);
	}

// zru�en� �t�tce podkladu (i kdy� podle dokumentace ru�en� nen� nutn�)
	::DeleteObject(brush);

// uvoln�n� kontextu displeje
	::ReleaseDC(MainFrame, dc);
}


/////////////////////////////////////////////////////////////////////////////
// vstup znaku z kl�vesnice (TRUE=zru�it)

BOOL OnChar(TCHAR znak)
{
	switch (znak)
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case '.':
	case ',':
	case 'E':
	case 'e':
	case '-':
	case '+':
		return FALSE;
	case 13:
	case 27:
		return TRUE;
	}

	if (znak < 32) return FALSE;

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// v�b�r v�eho

void SelectAll()
{
// v�b�r v�eho
	::SendMessage(EditNumWnd, EM_SETSEL, 0, (LPARAM)-1);

// aktualizace voleb bloku
	UpdateClipboard();
}

/////////////////////////////////////////////////////////////////////////////
// zru�en� bloku nebo znaku

void Delete()
{
// zji�t�n� ozna�en�ho �seku textu
	long int start, end;
	::SendMessage(EditNumWnd, EM_GETSEL, (WPARAM)&start, (WPARAM)&end);

// nen�-li nic ozna�eno, ru�� se znak za kurzorem (podhozen� kl�vesy DELETE)
	if (start == end)
	{
		::SendMessage(EditNumWnd, WM_KEYDOWN,	VK_DELETE, 0x01530001);
		::SendMessage(EditNumWnd, WM_KEYUP,		VK_DELETE, 0xC1530001);
	}
	else

// jinak zru�en� ozna�en�ho �seku
	{
		::SendMessage(EditNumWnd, WM_CLEAR, 0, 0);
	}

// aktualizace voleb bloku
	UpdateClipboard();
}

/////////////////////////////////////////////////////////////////////////////
// kopie bloku

void Copy()
{
// operace COPY
	::SendMessage(EditNumWnd, WM_COPY, 0, 0);

// aktualizace voleb bloku
	UpdateClipboard();
}

/////////////////////////////////////////////////////////////////////////////
// vyst��en� bloku

void Cut()
{
// operaceCUT
	::SendMessage(EditNumWnd, WM_CUT, 0, 0);

// aktualizace voleb bloku
	UpdateClipboard();
}

/////////////////////////////////////////////////////////////////////////////
// navr�cen� bloku

void Paste()
{
// operace PASTE
	::SendMessage(EditNumWnd, WM_PASTE, 0, 0);

// aktualizace voleb bloku
	UpdateClipboard();
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace voleb bloku

void UpdateClipboard()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

	if ((FocusBuf == BufEdiID) && (EditMode == BufNumID))
	{
		long int start, end;
		::SendMessage(EditNumWnd, EM_GETSEL, (WPARAM)&start, (WPARAM)&end);
		BOOL enable = (end != start);
		int len = ::GetWindowTextLength(EditNumWnd);

		EnableCommand(IDN_CUT, enable);
		EnableCommand(IDN_COPY, enable);
		EnableCommand(IDN_PASTE, ::IsClipboardFormatAvailable(CF_TEXT));
		EnableCommand(IDN_CLEAR, enable || (start < len));
		EnableCommand(IDN_ALL, len > 0);

		SetStatusText2(EmptyText);
	}
}


/////////////////////////////////////////////////////////////////////////////
// zm�na textu

void OnChange()
{
	if ((FocusBuf == BufEdiID) && (EditMode == BufNumID) && Real.IsValid(Index))
	{
		CText txt;
		txt.GetWindowText(EditNumWnd);
		double num = Double(txt);
		txt.Double(num);
		num = Double(txt);				// opakov�n� pro p��pad zad�n� vy��� p�esnosti
		if (Real[Index] != num)
		{
			Undo.AddRealSet(Index, Real[Index]);

			Real[Index] = num;
			SetModi();

			Undo.Stop();
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// znovuna�ten� ��sla

void ReLoad()
{
	CText txt;
	txt.Double(Real[Index]);
	txt.SetWindowText(EditNumWnd);
	SelectAll();
	Undo.Fixup();	
}

}
