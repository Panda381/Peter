
#include "Main.h"

/***************************************************************************\
*																			*
*									Výbìr barvy								*
*																			*
\***************************************************************************/

namespace ProgCol
{

BYTE	ColInd = WhiteCol;				// vybraná barva

int		ColsLeft = 0;					// poèátek pole barev X
int		ColsWidth = 200;				// zobrazená šíøka pole barev
int		ColsHeight = 150;				// zobrazená výška pole barev
int		ColsIncX = 10;					// pøírustek barev X
int		ColsIncY = 5;					// pøírustek barev Y


/////////////////////////////////////////////////////////////////////////////
// pøepoèet velikosti

void OnSize(HWND hWnd)
{
	RECT rc;
	::GetClientRect(hWnd, &rc);

// výpoèet horizontálních parametrù pole pro výbìr barev
	ColsIncX = (rc.right - 2) / ColCol;		// pøírustek barev X
	ColsWidth = ColsIncX * ColCol;			// zobrazená šíøka pole barev
	ColsLeft = (rc.right - ColsWidth)/2;	// levý okraj pole barev

// výpoèet vertikálních parametrù pole pro výbìr barev
	ColsIncY = (rc.bottom - 36) / ColLev;	// pøírustek barev Y
	ColsHeight = ColsIncY * ColLev;			// zobrazená šíøka pole barev
}
			

/////////////////////////////////////////////////////////////////////////////
// obsluha dialogu hledání textu

BOOL CALLBACK ProgColDialogProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
	case WM_INITDIALOG:
		{
			OnSize(hWnd);
			CenterWindow(hWnd, MainFrame);

			CText txt;

			txt.Load(IDN_SELECT_COLOR);
			txt.SetWindowTextFont(hWnd);

			txt.Load(IDN_CANCEL0);
			txt.SetDialogTextFont(hWnd, IDCANCEL);

		}
		return TRUE;

	case WM_PAINT:
		{
			HDC dc = ::GetDC(hWnd);
			HPALETTE OldPal = ::SelectPalette(dc, StdPalette, FALSE);
			::RealizePalette(dc);

			for (int col = StdColors-1; col >= ResCols; col--)
			{

// souøadnice pro barvu
				RECT rc;
				rc.left = ColsLeft + ((col-ResCols)/ColLev) * ColsIncX;
				rc.top = ((col-ResCols) % ColLev) * ColsIncY;
				rc.right = rc.left + ColsIncX;
				rc.bottom = rc.top + ColsIncY;

// zobrazení rámeèku indikátoru aktivní barvy
				if (col == ColInd)
				{
					::DrawEdge(dc, &rc, EDGE_RAISED, BF_RECT);
				}
				else

// zobrazení rámu kolem políèka
				{
					::DrawEdge(dc, &rc, EDGE_SUNKEN, BF_RECT);
				}

// korekce øámu políèka pro barvu
				rc.left += 2;
				rc.top += 2;
				rc.right -= 2;
				rc.bottom -= 2;

// zobrazení barvy políèka
				HBRUSH brush = ::CreateSolidBrush(PALETTEINDEX((WORD)col));
				::FillRect(dc, &rc, brush);
				::DeleteObject(brush);
			}

			::SelectPalette(dc, OldPal, FALSE);
			::ReleaseDC(hWnd, dc);
		}
		return TRUE;
		
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		{
			int xm = LOWORD(lParam) - ColsLeft;
			int ym = HIWORD(lParam);
			if ((xm >= 0) && (xm < ColsWidth) &&
				(ym >= 0) && (ym < ColsHeight))
			{
				ColInd = (BYTE)(ym/ColsIncY + xm/ColsIncX*ColLev + ResCols);
				::EndDialog(hWnd, IDOK);
				return TRUE;
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
			if (LOWORD(wParam) == IDCANCEL)
			{
				::EndDialog(hWnd, IDCANCEL);
				return TRUE;
			}
			break;
		}
		break;

	}
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// výbìr barvy

void SelectColor(int index)
{
// index ikony
	int icon = BufEdi.GetIcon(index);

// pøíprava aktuální barvy
	BYTE oldcol = Icon[icon][ICONWIDTH/2 + ICONHEIGHT/2*ICONWIDTH];
	ColInd = oldcol;

// zadání barvy
	if (::DialogBoxParam(
				hInstance,
				MAKEINTRESOURCE(IDN_SELECT_COLOR),
				MainFrame,
				(DLGPROC)ProgColDialogProc,
				(LPARAM)IDN_SELECT_COLOR) != IDOK) return;

	if (oldcol == ColInd) return;

// pøivlastnìní ikony
	PROGITEM* item = &BufEdi[index];

	if (item->Icon < 0)
	{
		Undo.AddProgIcon(BufEdiID, index, -1);
		icon = Icon.Dup(icon);
		item->Icon = icon;
	}

// výplò
	Undo.AddIconSet(icon, Icon[icon]);
	Icon[icon].CopyWrite();
	BYTE* dst = Icon[icon].DataData() + 6*ICONWIDTH + 6;

	for (int i = 20; i > 0; i--)
	{
		MemFill(dst, 20, ColInd);
		dst += ICONWIDTH;
	}

	BufEdi.ReDispIcon(icon);
	Undo.Stop();
	ProgAktItem();
	SetModi();
}


}


/////////////////////////////////////////////////////////////////////////////
// menu nastavení fontu

CFont	SetupFont;							// pracovní parametry fontu
HFONT	SetupVzorFont = NULL;				// font použitý ve vzorku

BOOL	NewSetupFill = FALSE;				// probíhá programové plnìní dialogu nastavení

CBufText FontList;							// seznam fontù

BOOL	OldScroll = FALSE;					// pùvodní scrolování
BOOL	OldScrollValid = FALSE;				// pøíznak platnosti údaje scrolování

// zjištìní seznamu fontù
int CALLBACK EnumFontFamProc(const LOGFONT* lpelf, const TEXTMETRIC* lpntm, DWORD FontType, LPARAM lParam)
{
	CText txt = lpelf->lfFaceName;

	CText chset;

	switch (lpelf->lfCharSet)
	{
	case BALTIC_CHARSET:
		chset = _T(" Baltic");
		break;

	case EASTEUROPE_CHARSET:
		chset = _T(" CE");
		break;

	case GREEK_CHARSET:
		chset = _T(" Greek");
		break;

	case RUSSIAN_CHARSET:
		chset = _T(" Cyr");
		break;

	case TURKISH_CHARSET :
		chset = _T(" Tur");
		break;
	}

	if (chset.IsNotEmpty() && (txt.Right(chset.Length()) == chset))
	{
		txt.Delete(txt.Length() - chset.Length());
	}

	for (int i = 0; i < FontList.Max(); i++)
	{
		if (FontList.IsValid(i) && (FontList[i] == txt)) return TRUE;
	}

	FontList.Add(txt);
	return TRUE;
}

void SetupSetSwc(HWND wnd)
{
	NewSetupFill = TRUE;

	HWND foc = ::GetFocus();
	CText txt;
	HWND w;
	BOOL enable;

	BOOL serif = SetupFont.Serif;
	if (SetupFont.Standard) serif = FALSE;

	DialogSetCheck(wnd, IDN_SETUP_STD, SetupFont.Standard);
	DialogSetCheck(wnd, IDN_SETUP_BOLD, SetupFont.Bold);
	DialogSetCheck(wnd, IDN_SETUP_ITALIC, SetupFont.Italic);
	DialogSetCheck(wnd, IDN_SETUP_UNDERLINE, SetupFont.Underline);
	DialogSetCheck(wnd, IDN_SETUP_STRIKEOUT, SetupFont.Strikeout);
	DialogSetCheck(wnd, IDN_SETUP_NORMAL, (!serif && !SetupFont.Fixed));
	DialogSetCheck(wnd, IDN_SETUP_SERIF, (serif && !SetupFont.Fixed));
	DialogSetCheck(wnd, IDN_SETUP_COURIER, (!serif && SetupFont.Fixed));
	DialogSetCheck(wnd, IDN_SETUP_USER, (serif && SetupFont.Fixed));

	enable = !SetupFont.Standard;
	::EnableWindow(::GetDlgItem(wnd, IDN_SETUP_ITALIC), enable);
	::EnableWindow(::GetDlgItem(wnd, IDN_SETUP_UNDERLINE), enable);
	::EnableWindow(::GetDlgItem(wnd, IDN_SETUP_STRIKEOUT), enable);
	::EnableWindow(::GetDlgItem(wnd, IDN_SETUP_SERIF), enable);
	::EnableWindow(::GetDlgItem(wnd, IDN_SETUP_USER), enable);

// nastavení výšky fontu v editoru
	w = ::GetDlgItem(wnd, IDN_SETUP_HEIGHT2);
	::EnableWindow(w, enable);

	txt.Int(SetupFont.Height);
	if (w != foc)
	{
		::SendMessage(w, CB_SELECTSTRING, (WPARAM)-1, (LPARAM)(LPCTSTR)txt);
		txt.SetWindowText(w);
	}

// nastavení šíøky fontu v editoru
	w = ::GetDlgItem(wnd, IDN_SETUP_WIDTH2);
	::EnableWindow(w, enable);

	txt.Double(SetupFont.Width*100);

	if (w != foc)
	{
		::SendMessage(w, CB_SELECTSTRING, (WPARAM)-1, (LPARAM)(LPCTSTR)txt);
		txt.SetWindowText(w);
	}

// povolení okna typu fontu
	enable = !SetupFont.Standard && SetupFont.Serif && SetupFont.Fixed;
	w = ::GetDlgItem(wnd, IDN_SETUP_TYP2);
	::EnableWindow(w, enable);
	::InvalidateRect(w, NULL, TRUE);

// výbìr aktivního typu fontu
	if (w != foc)
	{
		::SendMessage(w, LB_SELECTSTRING, (WPARAM)-1, (LPARAM)(LPCTSTR)SetupFont.User);
	}

// konec plnìní
	NewSetupFill = FALSE;
}

void SetupSetFont(HWND wnd)
{
	HWND vzor = ::GetDlgItem(wnd, IDN_SETUP_VZOR2);
	HFONT oldfont = SetupVzorFont;

	SetupVzorFont = GetFont(&SetupFont);

	::SendMessage(vzor, WM_SETFONT, (WPARAM)SetupVzorFont, TRUE);
	DelFont(oldfont);
}

int heights[] = 
{
	0,
	4,
	5,
	6,
	7,
	8,
	9,
	10,
	11,
	12,
	13,
	14,
	15,
	16,
	17,
	18,
	19,
	20,
	21,
	22,
	23,
	24,
	25,
	26,
	27,
	28,
	29,
	30,
	31,
	32,
	33,
	34,
	35,
	36,
	38,
	40,
	42,
	44,
	46,
	48,
	50,
	52,
	54,
	56,
	58,
	60,
	62,
	64,
	66,
	68,
	70,
	72,
	76,
	80,
	84,
	88,
	92,
	96,
	100,
	110,
	120,
	130,
	140,
	150,
	160,
	180,
	200,
	-1
};

int widths[] = 
{
	0,
	30,
	40,
	50,
	60,
	70,
	80,
	85,
	90,
	95,
	100,
	105,
	110,
	115,
	120,
	130,
	140,
	150,
	160,
	180,
	200,
	250,
	300,
	350,
	400,
	500,
	-1
};

BOOL CALLBACK ProgSetupDialogProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
	case WM_INITDIALOG:
		{
			CenterWindow(hWnd, MainFrame);

			CText txt;

// zjištìní fontù
			if (FontList.Num() == 0)
			{
				HDC dc = ::GetDC(MainFrame);
				::EnumFontFamilies(dc, NULL, EnumFontFamProc, 0);
				::ReleaseDC(MainFrame, dc);
			}

// naplnìní seznamu fontù
			HWND lst = ::GetDlgItem(hWnd, IDN_SETUP_TYP2);
			::SendMessage(lst, LB_RESETCONTENT, 0, 0);

			for (int i = 0; i < FontList.Max(); i++)
			{
				if (FontList.IsValid(i))
				{
					::SendMessage(lst, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)FontList[i]);
				}
			}

// naplnìní seznamu velikostí
			lst = ::GetDlgItem(hWnd, IDN_SETUP_HEIGHT2);
			::SendMessage(lst, CB_RESETCONTENT, 0, 0);

			int* hgh = heights;
			while(*hgh >= 0)
			{
				txt.Int(*hgh);
				::SendMessage(lst, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)txt);
				hgh++;
			}

// naplnìní seznamu šíøek
			lst = ::GetDlgItem(hWnd, IDN_SETUP_WIDTH2);
			::SendMessage(lst, CB_RESETCONTENT, 0, 0);

			int* wdh = widths;
			while(*wdh >= 0)
			{
				txt.Int(*wdh);
				::SendMessage(lst, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)txt);
				wdh++;
			}

// nadpis okna
			txt.Load(IDN_SETUP);
			txt.SetWindowTextFont(hWnd);

// textu
			DialogCustomText(hWnd, IDN_SETUP_VZOR);
			SetupVzor.SetDialogText(hWnd, IDN_SETUP_VZOR2);
			DialogCustomCheck(hWnd, IDN_SETUP_STD);
			DialogCustomCheck(hWnd, IDN_SETUP_BOLD);
			DialogCustomCheck(hWnd, IDN_SETUP_ITALIC);
			DialogCustomCheck(hWnd, IDN_SETUP_UNDERLINE);
			DialogCustomCheck(hWnd, IDN_SETUP_STRIKEOUT);
			DialogCustomRadio(hWnd, IDN_SETUP_NORMAL);
			DialogCustomRadio(hWnd, IDN_SETUP_SERIF);
			DialogCustomRadio(hWnd, IDN_SETUP_COURIER);
			DialogCustomBox(hWnd, IDN_SETUP_RAM1);
			DialogCustomBox(hWnd, IDN_SETUP_RAM2);
			DialogCustomRadio(hWnd, IDN_SETUP_USER);

			DialogCustomText(hWnd, IDN_SETUP_TYP);
			DialogCustomText(hWnd, IDN_SETUP_HEIGHT);
			DialogCustomText(hWnd, IDN_SETUP_WIDTH);

// tlaèítko Storno
			txt.Load(IDN_CANCEL0);
			txt.SetDialogTextFont(hWnd, IDCANCEL);

			SetupSetSwc(hWnd);
			SetupSetFont(hWnd);
		}
		return TRUE;

	case WM_DRAWITEM:				// 0x02B
		if (DialogDraw(hWnd, lParam)) return TRUE;
		return FALSE;

	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
		case BN_CLICKED:
		case LBN_SELCHANGE:			// CBN_SELCHANGE
		case CBN_EDITCHANGE:
		case EN_CHANGE:
			switch (LOWORD(wParam))
			{
			case IDOK:
			case IDCANCEL:
				{
					::EndDialog(hWnd, LOWORD(wParam));
					DelFont(SetupVzorFont);

					CText setupvzor;
					setupvzor.GetDialogText(hWnd, IDN_SETUP_VZOR2);

					if (setupvzor == _T("multi"))
					{
						if (LOWORD(wParam) == IDOK)
						{
							MultiJazyk = !MultiJazyk;

							MenuAkt = NULL;
							if (ProgMode)
							{
								ProgSetMenu();
							}
							else
							{
								SelectSetMenu();
							}
						}
						setupvzor.Empty();
					}


					if (setupvzor.IsNotEmpty())
					{
						SetupVzor = setupvzor;
					}

					SetupVzorFont = NULL;
					return TRUE;
				}

			case IDN_SETUP_NORMAL:
				SetupFont.Fixed = FALSE;
				SetupFont.Serif = FALSE;
				goto setdlg;

			case IDN_SETUP_SERIF:
				SetupFont.Fixed = FALSE;
				SetupFont.Serif = TRUE;
				goto setdlg;

			case IDN_SETUP_COURIER:
				SetupFont.Fixed = TRUE;
				SetupFont.Serif = FALSE;
				goto setdlg;

			case IDN_SETUP_USER:
				SetupFont.Fixed = TRUE;
				SetupFont.Serif = TRUE;
				goto setdlg;

			case IDN_SETUP_STD:
				SetupFont.Standard = !SetupFont.Standard;
				goto setdlg;

			case IDN_SETUP_BOLD:
				SetupFont.Bold = !SetupFont.Bold;
				goto setdlg;

			case IDN_SETUP_ITALIC:
				SetupFont.Italic = !SetupFont.Italic;
				goto setdlg;

			case IDN_SETUP_UNDERLINE:
				SetupFont.Underline = !SetupFont.Underline;
				goto setdlg;

			case IDN_SETUP_STRIKEOUT:
				SetupFont.Strikeout = !SetupFont.Strikeout;

setdlg:
				SetupSetSwc(hWnd);
				SetupSetFont(hWnd);
				break;

			case IDN_SETUP_TYP2:
				if (!NewSetupFill && !SetupFont.Standard && SetupFont.Serif && SetupFont.Fixed)
				{
					HWND w = ::GetDlgItem(hWnd, IDN_SETUP_TYP2);
					int i = ::SendMessage(w, LB_GETCURSEL, 0, 0);

					if (i >= 0)
					{
						int len = ::SendMessage(w, LB_GETTEXTLEN, i, 0);

						if (len > 0)
						{
							TCHAR* buf = (TCHAR*)MemGet((len+1)*sizeof(TCHAR));
							::SendMessage(w, LB_GETTEXT, i, (LPARAM)buf);
							SetupFont.User.Term();
							SetupFont.User.Init(buf, len);
							MemFree(buf);
							SetupSetFont(hWnd);
						}
					}
				}
				break;

			case IDN_SETUP_HEIGHT2:
				if (!NewSetupFill && !SetupFont.Standard)
				{
					HWND w = ::GetDlgItem(hWnd, IDN_SETUP_HEIGHT2);
					CText txt;

					if (HIWORD(wParam) == CBN_EDITCHANGE)
					{
						txt.GetWindowText(w);
					}
					else
					{
						int inx = ::SendMessage(w, CB_GETCURSEL, 0, 0);
						if (inx != CB_ERR)
						{
							int len = ::SendMessage(w, CB_GETLBTEXTLEN, inx, 0);
							if ((len != CB_ERR) && (len > 0))
							{
								TCHAR* buf = (TCHAR*)MemGet((len+1)*sizeof(TCHAR));
								buf[0] = 0;
								::SendMessage(w, CB_GETLBTEXT, inx, (LPARAM)buf);
								buf[len] = 0;
								txt = buf;
								MemFree(buf);
							}
						}
					}

					SetupFont.Height = Int(txt);

					SetupSetFont(hWnd);
				}
				break;

			case IDN_SETUP_WIDTH2:
				if (!NewSetupFill && !SetupFont.Standard)
				{
					HWND w = ::GetDlgItem(hWnd, IDN_SETUP_WIDTH2);
					CText txt;

					if (HIWORD(wParam) == CBN_EDITCHANGE)
					{
						txt.GetWindowText(w);
					}
					else
					{
						int inx = ::SendMessage(w, CB_GETCURSEL, 0, 0);
						if (inx != CB_ERR)
						{
							int len = ::SendMessage(w, CB_GETLBTEXTLEN, inx, 0);
							if ((len != CB_ERR) && (len > 0))
							{
								TCHAR* buf = (TCHAR*)MemGet((len+1)*sizeof(TCHAR));
								buf[0] = 0;
								::SendMessage(w, CB_GETLBTEXT, inx, (LPARAM)buf);
								buf[len] = 0;
								txt = buf;
								MemFree(buf);
							}
						}
					}

					SetupFont.Width = Double(txt)/100;

					SetupSetFont(hWnd);
				}
				break;
			}
			break;
		}
		break;

	case WM_CTLCOLORLISTBOX:
		{
			HDC dc = (HDC)wParam;
			HWND wnd = (HWND)lParam;
			HWND lst = ::GetDlgItem(hWnd, IDN_SETUP_TYP2);
			
			if (!SetupFont.Standard && ((SetupFont.Serif && SetupFont.Fixed) || (wnd != lst)))
			{
				::SetBkColor(dc, 0xffffff);
				return (LRESULT)::GetStockObject(WHITE_BRUSH);
			}
			else
			{
				::SetBkColor(dc, 0xc0c0c0);
				return (LRESULT)::GetStockObject(LTGRAY_BRUSH);
			}
		}
		break;

	case WM_ACTIVATE:
		if (OldScrollValid)
		{
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				::SystemParametersInfo(SPI_SETLISTBOXSMOOTHSCROLLING, 0, (void*)OldScroll, 0);
			}
			else
			{
				::SystemParametersInfo(SPI_SETLISTBOXSMOOTHSCROLLING, 0, (void*)FALSE, 0);
			}
		}
		return 0;
	}
	return FALSE;
}

void ProgOnSetup(int id)
{
// pøíprava pøepínaèù
	switch (id)
	{
	case IDN_SETUP1:
		SetupFont = SelectFont;
		break;

	case IDN_SETUP2:
		SetupFont = TreeFont;
		break;

	case IDN_SETUP3:
		SetupFont = TextFont;
		break;

	case IDN_SETUP4:
		SetupFont = MenuFont;
		break;

	case IDN_SETUP5:
		SetupFont = StatusFont;
		break;

	default:
		return;
	}

// úschova skrolování
	OldScrollValid = ::SystemParametersInfo(SPI_GETLISTBOXSMOOTHSCROLLING, 0, &OldScroll, 0);
	if (OldScrollValid)
	{
		::SystemParametersInfo(SPI_SETLISTBOXSMOOTHSCROLLING, 0, (void*)FALSE, 0);
	}

// zadání písma
	int res = ::DialogBoxParam(
				hInstance,
				MAKEINTRESOURCE(IDN_SETUP),
				MainFrame,
				(DLGPROC)ProgSetupDialogProc,
				(LPARAM)IDN_SETUP);

// navrácení skrolování
	if (OldScrollValid)
	{
		::SystemParametersInfo(SPI_SETLISTBOXSMOOTHSCROLLING, 0, (void*)OldScroll, 0);
	}
	if (res != IDOK) return;

// uložení a aktualizace fontù
	switch (id)
	{
	case IDN_SETUP1:
		SelectFont = SetupFont;
		if (SelectMode)
		{
			SelectSetMenu();
		}
		break;

	case IDN_SETUP2:
		TreeFont = SetupFont;
		if (ProgMode)
		{
			BufCls.AktFont();
			BufObj.AktFont();
			BufLoc.AktFont();
			BufStr.AktFont();
			BufEdi.AktFont();
		}
		break;

	case IDN_SETUP3:
		TextFont = SetupFont;
		EditText::StartReInit();
		break;

	case IDN_SETUP4:
		MenuFont = SetupFont;
		MenuAkt = NULL;
		if (ProgMode)
		{
			ProgSetMenu();
		}
		else
		{
			SelectSetMenu();
		}
		break;

	case IDN_SETUP5:
		StatusFont = SetupFont;
		SetStatusFont();
		break;
	}
}
