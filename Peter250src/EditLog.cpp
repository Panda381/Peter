
#include "Main.h"

/***************************************************************************\
*																			*
*								Editor logick�ch hodnot						*
*																			*
\***************************************************************************/

namespace EditLog
{

/////////////////////////////////////////////////////////////////////////////
// parametry

int		Index = 0;						// editovan� polo�ka
int		DispLeft;						// sou�adnice X edita�n�ho pole
int		DispTop;						// sou�adnice Y edita�n�ho pole
int		DispWidth;						// ���ka edita�n�ho pole
int		DispHeight;						// v��ka edita�n�ho pole

/////////////////////////////////////////////////////////////////////////////
// inicializace p�i startu programu

void StartInit()
{
	CText txt;
	txt.Load(IDN_ZAPNUTO);

// vytvo�en� okna p�ep�na�e
	EditLogWnd = ::CreateWindowEx(
		0,								// roz���en� styl
		_T("BUTTON"),					// t��da
		txt,							// text
		WS_CHILD | BS_CHECKBOX,			// styl
		300,							// X
		200,							// Y
		150,							// ���ka
		30,								// v��ka
		MainFrame,						// rodi�
		(HMENU)EDITLOGID,				// ID
		hInstance,						// instance
		NULL);							// data
}


/////////////////////////////////////////////////////////////////////////////
// zah�jen� editace (mus� b�t platn� index!)

void BegEdit(int index)
{
// index prvku
	Index = index;
	ASSERT(Bool.IsValid(index));

// zobrazen� okna
	::ShowWindow(EditLogWnd, SW_SHOW);

// nastaven� stavu tla��tka
	::SendMessage(EditLogWnd, BM_SETCHECK, Bool[index] ? BST_CHECKED : BST_UNCHECKED, 0);

// zobrazen�
	Disp();
}


/////////////////////////////////////////////////////////////////////////////
// p�esun edita�n�ho pole

HDWP MoveEdit(HDWP hdwp)
{
	DispWidth = 150;
	if (DispWidth > EditWidth-4) DispWidth = EditWidth-4;
	if (DispWidth < 1) DispWidth = 1;

	DispHeight = 18 + 8;
	if (DispHeight > EditHeight-4) DispHeight = EditHeight-4;
	if (DispHeight < 1) DispHeight = 1;

	DispLeft = EditX + (EditWidth - DispWidth)/2;
	DispTop = EditY + (EditHeight - DispHeight)/2;

	hdwp = ::DeferWindowPos(hdwp, EditLogWnd, NULL, 
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
// p�epnut� p�ep�na�e

void Switch()
{
	if (ProgMode && (FocusBuf == BufEdiID) && (EditMode == BufLogID))
	{
		Undo.AddBoolSet(Index, Bool[Index]);
		Bool[Index] = !Bool[Index];
		SetModi();
		::SendMessage(EditLogWnd, BM_SETCHECK, Bool[Index] ? BST_CHECKED : BST_UNCHECKED, 0);
		Undo.Stop();
	}
}


}
