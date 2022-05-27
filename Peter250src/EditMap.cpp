
#include "Main.h"

/***************************************************************************\
*																			*
*								Editor ploch								*
*																			*
\***************************************************************************/

namespace EditMap
{

/////////////////////////////////////////////////////////////////////////////
// parametry

// parametry editovan� plochy
int		Index = 0;						// editovan� plocha
int		Width;							// ���ka plochy v pol��k�ch
int		Height;							// v��ka plochy v pol��k�ch
int		MWidth;							// ���ka plochy v bodech
int		MHeight;						// v��ka plochy v bodech

// zobrazen� ��st plochy
int		TLeft;							// po��te�n� bod plochy vlevo
int		TBottom;						// po��te�n� bod plochy dole

// zobrazen� plochy na displeji (v bodech displeje)
int		DLeft;							// po��tek zobrazen� vlevo
int		DTop;							// po��tek zobrazen� naho�e
int		DWidth;							// zobrazen� ���ka ��sti plochy na displeji
int		DHeight;						// zobrazen� v��ka ��sti plochy na displeji

int		ILeft;							// index ikony vlevo
int		IRight;							// index ikony vpravo
int		ITop;							// index ikony naho�e
int		IBottom;						// index ikony dole

int		OLeft;							// zobrazen� ��st ikony vlevo
int		ORight;							// zobrazen� ��st ikony vpravo
int		OTop;							// zobrazen� ��st ikony naho�e
int		OBottom;						// zobrazen� ��st ikony dole

// nastaven� nov�ch rozm�r�
int		NewDimWidth;					// nov� ���ka
int		NewDimHeight;					// nov� v��ka
BOOL	NewDimFill = FALSE;				// prob�h� pln�n� text�

// edita�n� m�d
int		Mode = IDN_MAP;					// edita�n� m�d
BOOL	Rastr = TRUE;					// p��znak zobrazen� rastru
BOOL	EditMap = FALSE;				// edita�n� m�d
BOOL	FillMap = FALSE;				// m�d v�pln�
BOOL	MFilling = FALSE;				// ozna�ov�n� bloku pro v�pl�
int		FillX;							// po��te�n� ikona v�pln� X
int		FillY;							// po��te�n� ikona v�pln� Y

// obsluha ta�en�
BOOL	BegDrag = FALSE;				// p��znak zah�jen� ta�en�
BOOL	MDraging = FALSE;				// p��znak ta�en� ikony editoru plochy
int		BegX;							// sou�adnice my�i X p�ed zah�jen�m ta�en�
int		BegY;							// sou�adnice my�i Y p�ed zah�jen�m ta�en�
BOOL	CopyMode = FALSE;				// p��znak m�du kop�rov�n�
int		DragX = MOUSEINV;				// sou�adnice X ta�en� ikony (MOUSEINV = nen�)
int		DragY = MOUSEINV;				// sou�adnice Y ta�en� ikony (MOUSEINV = nen�)
HIMAGELIST DragImageList;				// obr�zek ta�en�
int		HotSpotX;						// bod dotyku X
int		HotSpotY;						// bod dotyku Y

// ikony p�ep�na��
CBufIcon	MapSwc;						// ikony p�ep�na��
CBufIcon	MapSwcNum;					// ikony ��sel

// editace ��seln� polo�ky
int		EditNumX;						// sou�adnice X editovan� ikony
int		EditNumY;						// sou�adnice Y editovan� ikony
BOOL	EditNumFill = FALSE;			// prob�h� pln�n� ��seln� polo�ky
#define	EDITNUMHEIGHT 16				// v��ka edita�n�ho pole

// ************************** obsluha volby editace *************************

/////////////////////////////////////////////////////////////////////////////
// inicializace p�i startu programu

void StartInit()
{
// na�ten� ikon p�ep�na��
	MapSwc.IconSize(12, 12);
	MapSwc.Load(IDN_MAPSWC, 10);
	ASSERT(MapSwc.Num() == 10);

// na�ten� ikon ��sel
	MapSwcNum.IconSize(8, 12);
	MapSwcNum.Load(IDN_MAPSWCNUM, 10);
	ASSERT(MapSwcNum.Num() == 10);
}


/////////////////////////////////////////////////////////////////////////////
// zah�jen� editace (mus� b�t platn� index!)

void BegEdit(int index)
{
// ukon�en� editace ��seln�ho parametru
	EndEditSwcNum();

// �schova indexu plochy
	Index = index;						// index ikony/obr�zku
	ASSERT(Map.IsValid(index));
	Map[index].CopyWrite();				// kopie p�ed z�pisem

// �schova parametr� plochy
	Width = Map[index].Width();			// ���ka plochy v pol��k�ch
	Height = Map[index].Height();		// v��ka plochy v pol��k�ch

	MDraging = FALSE;					// nen� ta�en�

// p�epo�et rozlo�en� a zobrazen� obr�zku
	Disp();

// aktualizace voleb bloku
	UpdateClipboard();
}

/////////////////////////////////////////////////////////////////////////////
// obsluha dialogu pro zad�n� rozm�r�

// nastaven� textu ���ky
void DimenDialSetWidth(HWND wnd)
{
	CText txt;
	txt.Int(NewDimWidth);
	NewDimFill = TRUE;
	txt.SetDialogText(wnd, IDC_SIRKA);
	NewDimFill = FALSE;
}

// na�ten� ���ky
void DimenDialGetWidth(HWND wnd)
{
	if (!NewDimFill)
	{
		CText txt;
		txt.GetDialogText(wnd, IDC_SIRKA);
		NewDimWidth = Int(txt);
		if (NewDimWidth < 1) NewDimWidth = Width;
		if (NewDimWidth > MAPMAXWIDTH) NewDimWidth = MAPMAXWIDTH;
	}
}

// nastaven� textu v��ky
void DimenDialSetHeight(HWND wnd)
{
	CText txt;
	txt.Int(NewDimHeight);
	NewDimFill = TRUE;
	txt.SetDialogText(wnd, IDC_VYSKA);
	NewDimFill = FALSE;
}

// na�ten� v��ky
void DimenDialGetHeight(HWND wnd)
{
	if (!NewDimFill)
	{
		CText txt;
		txt.GetDialogText(wnd, IDC_VYSKA);
		NewDimHeight = Int(txt);
		if (NewDimHeight < 1) NewDimHeight = Height;
		if (NewDimHeight > MAPMAXHEIGHT) NewDimHeight = MAPMAXHEIGHT;
	}
}

// callback funkce dialogu
BOOL CALLBACK DimenDialogProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
	case WM_INITDIALOG:
		{
			CenterWindow(hWnd, MainFrame);
			CText txt;

			txt.Load(IDN_MAPDIMEN);
			txt.SetWindowTextFont(hWnd);

			DialogCustomText(hWnd, IDN_SIRKA);
			DialogCustomText(hWnd, IDN_VYSKA);

			txt.Load(IDN_CANCEL0);
			txt.SetDialogTextFont(hWnd, IDCANCEL);

			DimenDialSetWidth(hWnd);
			DimenDialSetHeight(hWnd);
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
				DimenDialGetWidth(hWnd);
				DimenDialGetHeight(hWnd);
				::EndDialog(hWnd, LOWORD(wParam));
				return TRUE;
			}
			break;

		case EN_KILLFOCUS:
			if (LOWORD(wParam) == IDC_SIRKA)
			{
				DimenDialGetWidth(hWnd);
				DimenDialSetWidth(hWnd);
			}

			if (LOWORD(wParam) == IDC_VYSKA)
			{
				DimenDialGetHeight(hWnd);
				DimenDialSetHeight(hWnd);
			}
			break;
		}
		break;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� rozm�r� plochy

void OnDimen()
{
// p��prava v�choz�ch rozm�r�
	NewDimWidth = Width;
	NewDimHeight = Height;

// ukon�en� editace ��seln�ho parametru
	EndEditSwcNum();

// zad�n� nov�ch rozm�r� plochy
	if (::DialogBoxParam(
			hInstance,
			MAKEINTRESOURCE(IDN_MAPDIMEN),
			MainFrame,
			(DLGPROC)DimenDialogProc,
			(LPARAM)IDN_MAPDIMEN) != IDOK) return;

// kontrola, zda byly rozm�ry zm�n�ny
	if ((NewDimWidth == Width) && (NewDimHeight == Height))
	{
		return;
	}

// zapnut� kurzoru �ek�n�
	BeginWaitCursor();

// �schova plochy pro UNDO
	PushUndo();

// odpojen� referenc� plochy
	Map[Index].RefDec();

// nastaven� nov�ch rozm�r� obr�zku
	ASSERT(Map.IsValid(Index));
	Width = NewDimWidth;
	Height = NewDimHeight;
	Map[Index].Resize(Width, Height);

// napojen� referenc� plochy
	Map[Index].RefInc();

// aktualizace aktivn� polo�ky (zobrazen� referenc�)
	ProgAktItem();

// p�ekreslen� displeje
	Disp();

// p��znak modifikace
	SetModi();

// aktualizace voleb bloku
	UpdateClipboard();

// vypnut� kurzoru �ek�n�
	EndWaitCursor();
}



// ***************************** obsluha zobrazen� **************************

/////////////////////////////////////////////////////////////////////////////
// p�epo�et rozm�r�

void ReCalc()
{
// ���ka a v��ka plochy v bodech
	MWidth = Width * ICONWIDTH;
	MHeight = Height * ICONHEIGHT;

// p��prava zobrazen� v��ky (korekce, je-li horizont�ln� posuvn�k)
	DHeight = EditHeight - 2 - 1;				// v��ka k zobrazen�
	if (EditWidth - 4 - TRACKSIRKA < MWidth)
	{
		DHeight -= TRACKSIRKA;					// sn�en� v��ky o horizont�ln� posuvn�k
		if (DHeight < 1) DHeight = 1;			// minim�ln� ���ka
	}

// p��prava zobrazen� ���ky (korekce, je-li vertik�ln� posuvn�k)
	DWidth = EditWidth - 4 - 1;					//	���ka k zobrazen�
	if (DHeight < MHeight)
	{
		DWidth -= TRACKSIRKA;					// sn�en� ���ky o vertik�ln� posuvn�k
		if (DWidth < 1) DWidth = 1;				// minim�ln� ���ka
	}

// zobrazen� po��tek
	DLeft = EditX + 2 + (DWidth - MWidth)/2;
	if (DWidth < MWidth) DLeft = EditX + 2;
	DTop = EditY + 2 + (DHeight - MHeight)/2;
	if (DHeight < MHeight) DTop = EditY + 2;

// omezen� na skute�nou v��ku a ���ku
	if (DHeight > MHeight) DHeight = MHeight;	// omezen� na skute�n� rozm�ry
	if (DWidth > MWidth) DWidth = MWidth;		// omezen� na skute�n� rozm�ry

// omezen� po��tku p�i zobrazen� edita�n�ho pole
	if (EditMapNumWnd)
	{
		int i = (EditNumX+1)*ICONWIDTH - DWidth;
		if (TLeft < i) TLeft = i;
		i = EditNumX*ICONWIDTH;
		if (TLeft > i) TLeft = i;

		i = (EditNumY+1)*ICONHEIGHT-EDITNUMHEIGHT-1;
		if (TBottom > i) TBottom = i;
		i = (EditNumY+1)*ICONHEIGHT - DHeight;
		if (TBottom < i) TBottom = i;
	}

// omezen� po��te�n�ho bodu
	if (TLeft < 0) TLeft = 0;
	if (TLeft > MWidth - DWidth) TLeft = MWidth - DWidth;
	if (TBottom < 0) TBottom = 0;
	if (TBottom > MHeight - DHeight) TBottom = MHeight - DHeight;

// p�epo�et zobrazen�ho po��tku na ikony a body
	ILeft = TLeft / ICONWIDTH;					// index ikony vlevo
	OLeft = ICONWIDTH - (TLeft % ICONWIDTH);	// zobrazen� ��st ikony vlevo
	IBottom = TBottom / ICONHEIGHT;				// index ikony dole
	OBottom = ICONHEIGHT - (TBottom % ICONHEIGHT); // zobrazen� ��st ikony dole

// zobrazen� ��st ikon vpravo a naho�e
	ORight = ICONWIDTH - ((MWidth - TLeft - DWidth) % ICONWIDTH);
	OTop = ICONHEIGHT - ((MHeight - TBottom - DHeight) % ICONHEIGHT);

// index ikon vpravo a dole
	IRight = ILeft + (DWidth - OLeft + ICONWIDTH - 1) / ICONWIDTH;
	ITop = IBottom + (DHeight - OBottom + ICONHEIGHT - 1) / ICONHEIGHT;

// p��znak zapnut� posuvn�k�
	HScrollDisp = (DWidth < MWidth);
	VScrollDisp = (DHeight < MHeight);
}


/////////////////////////////////////////////////////////////////////////////
// zobrazen� cel�ho okna editoru

void Disp()
{
// p�epo�et zobrazen�
	ReCalc();

// nastaven� posuvn�k�
	SetScroll();

// otev�en� kontextu displeje
	HDC dc = ::GetDC(MainFrame);

// nastaven� vlastn�ch palet
	HPALETTE OldPal;					// �schova p�vodn�ch palet
	OldPal = ::SelectPalette(dc,		// v�b�r vlastn�ch palet
		StdPalette, FALSE);
	::RealizePalette(dc);				// realizace palet

// p��prava �t�tce k vymaz�n� podkladu
	HBRUSH brush = (HBRUSH)::GetStockObject(LTGRAY_BRUSH); // �t�tec k vymaz�n� plochy

// vymaz�n� plochy naho�e nad obr�zkem
	RECT rc;
	rc.left = EditX + 2;
	rc.right = EditX + EditWidth - 2;
	rc.top = EditY + 2;
	rc.bottom = DTop;
	if (rc.top < rc.bottom)
	{
		::FillRect(dc, &rc, brush);
	}

// vymaz�n� plochy dole pod obr�zkem (left a right je nastaveno)
	rc.top = DTop + DHeight;
	rc.bottom = EditY + EditHeight - 2;
	if (rc.top < rc.bottom)
	{
		::FillRect(dc, &rc, brush);
	}

// vymaz�n� plochy vlevo od obr�zku (left je nastaveno)
	rc.right = DLeft;
	rc.top = DTop;
	rc.bottom = DTop + DHeight;
	if (rc.left < rc.right)
	{
		::FillRect(dc, &rc, brush);
	}

// vymaz�n� plochy vpravo od obr�zku (top a bottom je nastaveno)
	rc.left = DLeft + DWidth;
	rc.right = EditX + EditWidth - 2;
	if (rc.left < rc.right)
	{
		::FillRect(dc, &rc, brush);
	}

// vykreslen� obr�zku
	DispAkt(dc);

// zru�en� �t�tce podkladu (i kdy� podle dokumentace ru�en� nen� nutn�)
	::DeleteObject(brush);

// n�vrat p�vodn�ch palet
	::SelectPalette(dc,OldPal,FALSE);

// uvoln�n� kontextu displeje
	::ReleaseDC(MainFrame, dc);
}


/////////////////////////////////////////////////////////////////////////////
// aktualiza�n� p�ekreslen� obr�zku (s p�epo�tem rozm�r�)

void ReDisp()
{
// p�epo�et zobrazen�
	ReCalc();

// nastaven� posuvn�k�
	SetScroll();

// otev�en� kontextu displeje
	HDC dc = ::GetDC(MainFrame);

// nastaven� vlastn�ch palet
	HPALETTE OldPal;					// �schova p�vodn�ch palet
	OldPal = ::SelectPalette(dc,		// v�b�r vlastn�ch palet
		StdPalette, FALSE);
	::RealizePalette(dc);				// realizace palet

// vykreslen� obr�zku
	DispAkt(dc);

// n�vrat p�vodn�ch palet
	::SelectPalette(dc,OldPal,FALSE);

// uvoln�n� kontextu displeje
	::ReleaseDC(MainFrame, dc);
}


/////////////////////////////////////////////////////////////////////////////
// aktualiza�n� p�ekreslen� obr�zku

void DispAkt(HDC dc)
{
// lok�ln� prom�nn�
	int i,j, k, l;
	int width = (IRight - ILeft + 1)*ICONWIDTH;
	int height = (ITop - IBottom + 1)*ICONHEIGHT;
	BYTE* dst;
	BYTE* src;
	MAPITEM* data = Map[Index].DataData();
	int cislic;

// vytvo�en� bufferu k zobrazen�
	BYTE* buf = (BYTE*)MemGet(width*height);

// zobrazen� ikon
	for (i = ILeft; i <= IRight; i++)
	{
		for (j = IBottom; j <= ITop; j++)
		{
			(Icon.Get(data[i + j*Width].Icon)).DeComp();
			src = (Icon.Get(data[i + j*Width].Icon)).DataData();
			dst = buf + (i-ILeft)*ICONWIDTH + (j-IBottom)*ICONHEIGHT*width;

			for (k = ICONHEIGHT; k > 0; k--)
			{
				MemCopy(dst, src, ICONWIDTH);
				dst += width;
				src += ICONWIDTH;
			}
			dst -= width * ICONHEIGHT;

// zobrazen� p�ep�na�e
			k = -1;
			switch (Mode)
			{
			case IDN_SWC5: k++;		// 4
			case IDN_SWC4: k++;		// 3
			case IDN_SWC3: k++;		// 2
			case IDN_SWC2: k++;		// 1
			case IDN_SWC1: k++;		// 0
				if (data[i + j*Width].Param & ((DWORD)MAP_FLAG1 << k))
				{
					k = 2*k;
				}
				else
				{
					k = 2*k +1;
				}

				dst += width * (ICONHEIGHT - 12);
				src = MapSwc[k].DataData();
				for (k = 12; k > 0; k--)
				{
					MemCopy(dst, src, 12);
					dst += width;
					src += 12;
				}
				break;

// zobrazen� ��seln� hodnoty
			case IDN_SWCN1:	k = ((data[i + j*Width].Param & MAP_VALUE1_MASK) >> MAP_VALUE1_ROT);
							goto DISPAKTSWCNUM;

			case IDN_SWCN2:	k = ((data[i + j*Width].Param & MAP_VALUE2_MASK) >> MAP_VALUE2_ROT);
							goto DISPAKTSWCNUM;

			case IDN_SWCN3:	k = ((data[i + j*Width].Param & MAP_VALUE3_MASK) >> MAP_VALUE3_ROT);

		DISPAKTSWCNUM:
				cislic = 1;				// po�et ��slic
				if (k >= 10) cislic++;
				if (k >= 100) cislic++;
				if (k >= 1000) cislic++;

				dst += width * (ICONHEIGHT - 12) + (cislic-1)*8;

				for (; cislic > 0; cislic--)
				{
					src = MapSwcNum[k % 10].DataData();

					for (l = 12; l > 0; l--)
					{
						MemCopy(dst, src, 8);
						dst += width;
						src += 8;
					}

					k /= 10;
					dst -= 12*width + 8;
				}
			}
		}
	}

// zobrazen� rastru
	if (Rastr)
	{
		BYTE col = BlueCol;

		dst = buf + ICONHEIGHT*width;
		for (i = ITop - IBottom; i > 0; i--)
		{
			MemFill(dst, width, col);
			dst += ICONHEIGHT*width;
		}

		for (i = IRight - ILeft; i > 0; i--)
		{
			dst = buf + ICONWIDTH*i - 1;
			for (j = height; j > 0; j--)
			{
				*dst = col;
				dst += width;
			}
		}
	}

// p�ednastaven� parametr� z�hlav� bitmapy
	StdBitmapInfo->bmiHeader.biWidth = width;
	StdBitmapInfo->bmiHeader.biHeight = DHeight;

// zobrazen�
	::StretchDIBits(dc, DLeft, DTop, DWidth, DHeight, ICONWIDTH - OLeft,
			0, DWidth, DHeight, buf + (ICONHEIGHT-OBottom)*width,
			StdBitmapInfo, DIB_RGB_COLORS, SRCCOPY);

// zru�en� bufferu
	MemFree(buf);
}


/////////////////////////////////////////////////////////////////////////////
// zapnut�/vypnut� rastru

void OnRastr()
{
	Rastr = !Rastr;
	ReDisp();
	UpdateMenu();
}


/////////////////////////////////////////////////////////////////////////////
// obsluha zobrazen� kurzoru my�i (TRUE=obslou�eno)

BOOL OnSetCursor(int x, int y)
{
// pole obr�zku
	if (((x >= DLeft) &&
		(x < DLeft + DWidth) &&
		(y >= DTop) &&
		(y < DTop + DHeight)))
	{
		if (EditMap)
		{
			CurAkt = CurPen;
		}
		else
		{
			if (FillMap)
			{
				CurAkt = CurFill;
			}
			else
			{
				if (MDraging)
				{
					if ((MouseX == MOUSEINV) || (MouseY == MOUSEINV))
					{
						if (CopyMode)
						{
							CurAkt = CurNoDrag;
						}
						else
						{
							CurAkt = CurDelete;
						}
					}
					else
					{
						if (CopyMode)
						{
							CurAkt = CurCopy;
						}
						else
						{
							CurAkt = CurMove;
						}
					}
				}
				else
				{
					CurAkt = CurRuka;
				}
			}
		}
		::SetCursor(CurAkt);
		return TRUE;
	}

// implicitn� obsluha
	return FALSE;
}


// ********************* obsluha posuvn�k� **********************************


/////////////////////////////////////////////////////////////////////////////
// p�esun posuvn�k� a edita�n�ho pole p�i zm�n� rozm�r� okna

HDWP MoveScroll(HDWP hdwp)
{
// lok�ln� prom�nn�
	int x, y, w, h;

// p�epo�et zobrazen�
	ReCalc();

// p�esun a zobrazen� horizont�ln�ho posuvn�ku
	x = EditX + 2;
	y = EditY + EditHeight - 2 - TRACKSIRKA;
	w = EditWidth - 4;
	h = TRACKSIRKA;
	if (VScrollDisp) w -= TRACKSIRKA;
	hdwp = ::DeferWindowPos(hdwp, HScroll, NULL, x, y, w, h,
			SWP_DRAWFRAME | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER |
			((HScrollDisp && !PreviewPic) ? SWP_SHOWWINDOW : SWP_HIDEWINDOW));

// p�esun a zobrazen� vertik�ln�ho posuvn�ku
	x = EditX + EditWidth - 2 - TRACKSIRKA;
	y = EditY + 2;
	w = TRACKSIRKA;
	h = EditHeight - 4;
	if (HScrollDisp) h -= TRACKSIRKA;
	hdwp = ::DeferWindowPos(hdwp, VScroll, NULL, x, y, w, h,
			SWP_DRAWFRAME | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER |
			((VScrollDisp && !PreviewPic) ? SWP_SHOWWINDOW : SWP_HIDEWINDOW));

// p�esun edita�n�ho pole
	if (EditMapNumWnd)
	{
		x = DLeft + (EditNumX - ILeft)*ICONWIDTH - (ICONWIDTH - OLeft);
		y = DTop + (ITop - EditNumY - 1)*ICONHEIGHT + OTop;
		w = ICONWIDTH;
		h = EDITNUMHEIGHT;
		if (w > DWidth) w = DWidth;
		if (h > DHeight) h = DHeight;
		hdwp = ::DeferWindowPos(hdwp, EditMapNumWnd, NULL, x, y, w, h, 
			SWP_NOOWNERZORDER | SWP_NOZORDER);
	}

// nastaven� posuvn�k�
	SetScroll();

// nov� hodnota HDWP
	return hdwp;
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace zobrazen� posuvn�k�

void SetScroll()
{
// lok�ln� prom�nn�
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);

// -------- horizont�ln� posuvn�k -------------

// na�ten� stavu horizont�ln�ho posuvn�ku
	si.fMask = SIF_ALL;
	::GetScrollInfo(HScroll, SB_CTL, &si);	// na�ten� stavu posuvn�ku
	si.fMask = 0;							// p�ednastaven� - nic se nem�n�

// minim�ln� pozice
	if (si.nMin != 0)
	{
		si.nMin = 0;
		si.fMask |= SIF_RANGE;
	}

// maxim�ln� pozice
	if (si.nMax != MWidth)
	{
		si.nMax = MWidth;
		si.fMask |= SIF_RANGE;
	}

// velikost str�nky
	if (si.nPage != (UINT)DWidth)
	{
		si.nPage = (UINT)DWidth;
		si.fMask |= SIF_PAGE;
	}

// pozice posuvn�ku
	if (si.nPos != TLeft)
	{
		si.nPos = TLeft;
		si.fMask |= SIF_POS;
	}

// nastaven� nov�ch parametr� posuvn�ku
	if (si.fMask)
	{
		::SetScrollInfo(HScroll, SB_CTL, &si, TRUE);
	}

// zobrazen� horizont�ln�ho posuvn�ku
	::ShowWindow(HScroll, (HScrollDisp && !PreviewPic) ? SW_SHOWNA : SW_HIDE);


// -------- vertik�ln� posuvn�k -------------

// na�ten� stavu vertik�ln�ho posuvn�ku
	si.fMask = SIF_ALL;
	::GetScrollInfo(VScroll, SB_CTL, &si);
	si.fMask = 0;							// p�ednastaven� - nic se nem�n�

// minim�ln� pozice
	if (si.nMin != 0)
	{
		si.nMin = 0;
		si.fMask |= SIF_RANGE;
	}

// maxim�ln� pozice
	if (si.nMax != MHeight)
	{
		si.nMax = MHeight;
		si.fMask |= SIF_RANGE;
	}

// velikost str�nky
	if (si.nPage != (UINT)DHeight)
	{
		si.nPage = (UINT)DHeight;
		si.fMask |= SIF_PAGE;
	}

// pozice posuvn�ku
	if (si.nPos != MHeight - DHeight - TBottom)
	{
		si.nPos = MHeight - DHeight - TBottom;
		si.fMask |= SIF_POS;
	}

// nastaven� nov�ch parametr� posuvn�ku
	if (si.fMask)
	{
		::SetScrollInfo(VScroll, SB_CTL, &si, TRUE);
	}

// zobrazen� vertik�ln�ho posuvn�ku
	::ShowWindow(VScroll, (VScrollDisp && !PreviewPic) ? SW_SHOWNA : SW_HIDE);
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace pozice edita�n�ho pole

void SetEditMapNum()
{
	if (EditMapNumWnd)
	{
		ReCalc();

		int x = DLeft + (EditNumX - ILeft)*ICONWIDTH - (ICONWIDTH - OLeft);
		int y = DTop + (ITop - EditNumY - 1)*ICONHEIGHT + OTop;
		int w = ICONWIDTH;
		int h = EDITNUMHEIGHT;
		if (w > DWidth) w = DWidth;
		if (h > DHeight) h = DHeight;
		::SetWindowPos(EditMapNumWnd, NULL, x, y, w, h, SWP_NOOWNERZORDER | SWP_NOZORDER);
	}
}


/////////////////////////////////////////////////////////////////////////////
// horizont�ln� posuvn�k

void OnHScroll(int code, int pos)
{
	switch (code)
	{
	case SB_LEFT:
		pos = 0;
		break;

	case SB_LINELEFT:
		pos -= DWidth/20+1;
		break;

	case SB_LINERIGHT:
		pos += DWidth/20+1;
		break;

	case SB_PAGELEFT:
		pos -= DWidth-16;
		break;

	case SB_PAGERIGHT:
		pos += DWidth-16;
		break;

	case SB_RIGHT:
		pos = MWidth - DWidth;
		break;
	}

	if (pos < 0) pos = 0;
	if (pos >= MWidth - DWidth)
		pos = MWidth - DWidth;

	if (pos != TLeft)
	{
		TLeft = pos;
		SetEditMapNum();
		Disp();
	}
}


/////////////////////////////////////////////////////////////////////////////
// vertik�ln� posuvn�k

void OnVScroll(int code, int pos)
{
	switch (code)
	{
	case SB_TOP:
		pos = 0;
		break;

	case SB_LINEUP:
		pos -= DHeight/20+1;
		break;

	case SB_LINEDOWN:
		pos += DHeight/20+1;
		break;

	case SB_PAGEUP:
		pos -= DHeight-16;
		break;

	case SB_PAGEDOWN:
		pos += DHeight-16;
		break;

	case SB_BOTTOM:
		pos = MHeight - DHeight;
		break;
	}

	if (pos < 0) pos = 0;
	if (pos >= MHeight - DHeight)
		pos = MHeight - DHeight;
	pos = MHeight - DHeight - pos;

	if (pos != TBottom)
	{
		TBottom = pos;
		SetEditMapNum();
		Disp();
	}
}


// **************************** obsluha editace *****************************

/////////////////////////////////////////////////////////////////////////////
// zah�jen� editace ��seln�ho parametru

void BegEditSwcNum(int x, int y)
{
// kontrola polo�ky
	if (((DWORD)x >= (DWORD)Width) || ((DWORD)y >= (DWORD)Height))
	{
		return;
	}

// ukon�en� editace ��seln�ho parametru
	EndEditSwcNum();

// �schova parametr�
	EditNumX = x;
	EditNumY = y;
	EditNumFill = TRUE;

// p��prava sou�adnic polo�ky
	int left = DLeft + (EditNumX - ILeft)*ICONWIDTH - (ICONWIDTH - OLeft);
	int top = DTop + (ITop - EditNumY - 1)*ICONHEIGHT + OTop;

// vytvo�en� edita�n�ho okna
	EditMapNumWnd = ::CreateWindowEx(
		0,								// roz���en� styl
		_T("EDIT"),						// t��da
		NULL,							// text
		WS_CHILD | ES_AUTOHSCROLL | ES_NUMBER,	// styl
		left,							// X
		top,							// Y
		ICONWIDTH,						// ���ka
		EDITNUMHEIGHT,					// v��ka
		MainFrame,						// rodi�
		NULL,							// ID
		hInstance,						// instance
		NULL);							// data

// p��prava ��sla
	int cislo = (Map[Index].DataData())[EditNumX + EditNumY*Width].Param;
	switch (Mode)
	{
	case IDN_SWCN1: cislo &= MAP_VALUE1_MASK;
					cislo >>= MAP_VALUE1_ROT;
					break;

	case IDN_SWCN2: cislo &= MAP_VALUE2_MASK;
					cislo >>= MAP_VALUE2_ROT;
					break;

	case IDN_SWCN3: cislo &= MAP_VALUE3_MASK;
					cislo >>= MAP_VALUE3_ROT;
					break;
	}

// nastaven� ��sla
	CText txt;
	txt.Int(cislo);
	txt.SetWindowText(EditMapNumWnd);

// zobrazen� okna
	::ShowWindow(EditMapNumWnd, SW_SHOW);
	::SetFocus(EditMapNumWnd);
	::SendMessage(EditMapNumWnd, EM_SETSEL, 0, (LPARAM)-1);
	EditNumFill = FALSE;

// aktualizace menu
	UpdateClipboard();
	Disp();
}


/////////////////////////////////////////////////////////////////////////////
// zm�na editovan�ho ��sla

void OnChangeEditSwcNum()
{
// test platnosti
	if (ProgMode && EditMapNumWnd && !EditNumFill && (EditMode == BufMapID))
	{

// na�ten� zadan�ho ��sla
		CText txt;
		txt.GetWindowText(EditMapNumWnd);
		int cislo = Int(txt);
		if (cislo < 0) cislo = 0;

// adresa polo�ky
		MAPITEM* item = Map[Index].DataData() + (EditNumX + EditNumY * Width);

// nastaven� ��sla
		int max = 0;
		DWORD mask = 0;
		int rot = 0;

		switch(Mode)
		{
		case IDN_SWCN1:
			max = MAP_VALUE1_MAX;
			mask = MAP_VALUE1_MASK;
			rot = MAP_VALUE1_ROT;
			break;

		case IDN_SWCN2:
			max = MAP_VALUE2_MAX;
			mask = MAP_VALUE2_MASK;
			rot = MAP_VALUE2_ROT;
			break;

		case IDN_SWCN3:
			max = MAP_VALUE3_MAX;
			mask = MAP_VALUE3_MASK;
			rot = MAP_VALUE3_ROT;
			break;
		}

		if (cislo > max)
		{
			cislo = max;
		}

		cislo <<= rot;

		if ((DWORD)cislo != (item->Param & mask))
		{
			item->Param &= ~mask;
			item->Param |= (DWORD)cislo;
			SetModi();
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// ukon�en� editace ��sla

void EndEditSwcNum()
{
	if (EditMapNumWnd)
	{
		EditNumFill = TRUE;
		::DestroyWindow(EditMapNumWnd);
		EditMapNumWnd = NULL;
		EditNumFill = FALSE;
		Disp();
		ProgUpdateUndoRedo();
	}
}


/////////////////////////////////////////////////////////////////////////////
// ukon�en� ta�en�

void EndDrag()
{
	BegDrag = FALSE;

	if (MDraging)
	{
		MDraging = FALSE;

// zastaven� ta�en�
		::ImageList_DragLeave(NULL);

// ukon�en� ta�en�
		::ImageList_EndDrag();

// zru�en� seznamu obr�zk� ta�en�
		::ImageList_Destroy(DragImageList);

// uvoln�ni zachyt�v�n� my�i
		::ReleaseCapture();

// nastaven� b�n�ho kurzoru my�i
		::SetCursor(CurArrow);
	}
}


/////////////////////////////////////////////////////////////////////////////
// zobrazen� v�b�ru v�pln�

void DispFill()
{
	if ((DragX == MOUSEINV) || (DragY == MOUSEINV)) return;

// p��prava DC displeje
	HDC dc = ::GetDC(MainFrame);

// v�b�r �t�tce
	HBRUSH oldBrush = (HBRUSH)::SelectObject(dc, HalfToneBrush);

// p��prava sou�adnic
	int x1 = (FillX - ILeft)*ICONWIDTH + DLeft + OLeft - ICONWIDTH;
	int y1 = (ITop - FillY)*ICONHEIGHT + DTop + OTop - ICONHEIGHT;
	int x2 = (DragX - FillX)*ICONWIDTH + x1;
	int y2 = (FillY - DragY)*ICONHEIGHT + y1;

	if (x1 > x2)
	{
		int n = x2;
		x2 = x1;
		x1 = n;
	}

	if (y1 > y2)
	{
		int n = y2;
		y2 = y1;
		y1 = n;
	}

	x2 += ICONWIDTH;
	y2 += ICONHEIGHT;

	if (x1 < DLeft) x1 = DLeft;
	if (y1 < DTop) y1 = DTop;
	if (x2 >= DLeft + DWidth) x2 = DLeft + DWidth;
	if (y2 >= DTop + DHeight) y2 = DTop + DHeight;

	::PatBlt(dc, x1, y1, x2 - x1, y2 - y1, PATINVERT);

// navr�cen� p�vodn�ho �t�tce
	::SelectObject(dc, oldBrush);

// uvoln�n� DC displeje
	::ReleaseDC(MainFrame, dc);
}


/////////////////////////////////////////////////////////////////////////////
// ukon�en� ta�en� v�pln�

void EndFill()
{
	DispFill();
	MFilling = FALSE;
	if ((DragX == MOUSEINV) || (DragY == MOUSEINV)) return;

	int x1 = FillX;
	int y1 = FillY;
	int x2 = DragX;
	int y2 = DragY;

	int n;

	if (x1 > x2)
	{
		n = x2;
		x2 = x1;
		x1 = n;
	}

	if (y1 > y2)
	{
		n = y2;
		y2 = y1;
		y1 = n;
	}

	if ((x1 == x2) && (y1 == y2)) return;

	PushUndo();

	MAPITEM* data = Map[Index].DataData();
	MAPITEM* srcdata = data + (FillX + FillY*Width);

	for (int x = x1; x <= x2; x++)
	{
		for (int y = y1; y <= y2; y++)
		{
			MAPITEM* dstdata = data + (x + y*Width);

//			Map[Index].
				RefIconDec(dstdata->Icon);
//			Map[Index].
				RefIconInc(srcdata->Icon);
			dstdata->Icon = srcdata->Icon;
			dstdata->Param = srcdata->Param;
		}
	}

	SetModi();
	ProgAktItem();
	Disp();
}


/////////////////////////////////////////////////////////////////////////////
// stisk tla��tka my�i (TRUE = obslou�eno)

BOOL OnButtonDown(UINT flags, int x, int y, BOOL right, BOOL dclick)
{
// ukon�en� editace ��seln�ho parametru
	EndEditSwcNum();

// ukon�en� ta�en�
	if (MFilling)
	{
		EndFill();
		return TRUE;
	}

	if (MDraging)
	{
		EndDrag();
		return TRUE;
	}

// kontrola, zda je platn� edita�n� pole
	if ((MouseX == MOUSEINV) || (MouseY == MOUSEINV))
	{
		return FALSE;
	}

// �schova sou�adnic p�ed zah�jen�m ta�en�
	BegX = x;
	BegY = y;
	DragX = MouseX;
	DragY = MouseY;

// v�pl� plochy
	if (FillMap)
	{
		FillX = MouseX;
		FillY = MouseY;
		MFilling = TRUE;
		DispFill();
		return TRUE;
	}

	BegDrag = TRUE;
	CopyMode = FALSE;

// �schova plochy pro UNDO
	if (EditMap && (Mode != IDN_MAP))
	{
		PushUndo();
	}

// adresa polo�ky
	MAPITEM* data = Map[Index].DataData() + (MouseX + MouseY*Width);

// index ikony
	int icon = data->Icon;

// zah�jen� editace
	int k = -1;

	if (EditMap)
	{
		switch (Mode)
		{
		case IDN_MAP:
			if (Icon.IsValid(icon))
			{
				CBufProg* buf = &BufObj;
				int inx = BufObj.SrcDat(BufIcoID, icon);
				if (inx < 0)
				{
					buf = &BufLoc;
					inx = BufLoc.SrcDat(BufIcoID, icon);
				}

				if (buf->IsValid(inx))
				{
					if (SetEditMode(BufIcoID, icon))
					{
						SetToolMode(BufIcoID);
						BufEdi.Nadpis(Text.GetTxt(buf->GetText(inx)));
						SetFocusEdit();
//						ProgUpdateInOut();
					}
				}
			}
			break;

		case IDN_SWC5:	k++;	// 4
		case IDN_SWC4:	k++;	// 3
		case IDN_SWC3:	k++;	// 2
		case IDN_SWC2:	k++;	// 1
		case IDN_SWC1:	k++;	// 0
			data->Param ^= ((DWORD)MAP_FLAG1 << k);
			SetModi();
			ReDisp();
			break;

		case IDN_SWCN1:
		case IDN_SWCN2:
		case IDN_SWCN3:
			BegEditSwcNum(MouseX, MouseY);
			break;
		}
	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// posun my�i

void OnMouseMove(UINT flags, int x, int y)
{
// korekce sou�adnic p�i ta�en� (na st�ed ikony)
	if (MDraging)
	{
		x -= HotSpotX;	x += ICONWIDTH/2;
		y -= HotSpotY;	y += ICONHEIGHT/2;
	}

	if (Draging)
	{
		x -= DragHotSpotX;	x += DragIconWidth/2;
		y -= DragHotSpotY;	y += DragIconHeight/2;
	}

// p��prava relativn�ch sou�adnic bodu v edita�n�m poli
	int sx = (x - DLeft - OLeft + ICONWIDTH)/ICONWIDTH + ILeft;
	int sy = ITop - (y - DTop - OTop + ICONHEIGHT)/ICONHEIGHT;

// sou�adnice my�i pro zobrazen�
	if ((x >= DLeft) && (y >= DTop) &&
		(x < DLeft + DWidth) && (y < DTop + DHeight))
	{
		MouseX = sx;
		MouseY = sy;
	}
	else
	{
		MouseX = MOUSEINV;
		MouseY = MOUSEINV;
	}

// ta�en� v�pln�
	if (MFilling)
	{
		MouseX = sx;
		MouseY = sy;
		if (x < DLeft) MouseX = ILeft;
		if (y < DTop) MouseY = ITop;
		if (x >= DLeft + DWidth) MouseX = IRight;
		if (y >= DTop + DHeight) MouseY = IBottom;

		if ((MouseX != DragX) || (MouseY != DragY))
		{
			DispFill();
			DragX = MouseX;
			DragY = MouseY;
			DispFill();
		}
		DispMouseXY();
		return;
	}

// zah�jen� ta�en�
	if ((flags & (MK_LBUTTON | MK_RBUTTON)) &&
		!MDraging &&
		!EditMap &&
		BegDrag &&
		(DragX != MOUSEINV) &&
		(DragY != MOUSEINV) &&
		(abs(x - BegX) + abs(y - BegY) >= 3))
	{
		MDraging = TRUE;

// bod styku pro ta�en�
		HotSpotX = BegX - (DLeft + (DragX*ICONWIDTH - TLeft));
		HotSpotY = BegY - (DTop + DHeight - ((DragY+1)*ICONHEIGHT - TBottom));

// vygenerov�n� ta�en� ikony
		CBufIcon dragicon;
		dragicon.Add(Icon.Get((Map[Index].DataData())[DragX + DragY*Width].Icon));
		DragImageList = dragicon.GenerList(TRUE, SI_BIG, NULL);

// zapnut� zobrazen� obr�zku ta�en� (bez uzamyk�n� okna)
		::ImageList_DragShowNolock(TRUE);

// zapnut� kurzoru pro ta�en�
		CopyMode = (flags & (MK_RBUTTON));
		if (CopyMode)
		{
			CurAkt = CurCopy;
		}
		else
		{
			CurAkt = CurMove;
		}
		::SetCursor(CurAkt);

// vytvo�en� nov�ho obr�zku ta�en�
		::ImageList_SetDragCursorImage(DragImageList, 0, 0, 0);

// zah�jen� operace ta�en�
		::ImageList_BeginDrag(DragImageList, 0, 0, 0);

// na�ten� sou�adnice kurzoru my�i
//		POINT pt;
//		::GetCursorPos(&pt);

// zah�jen� operace ta�en�, z�kaz aktualizace okna
		::ImageList_DragEnter(NULL, MouseScreen.x - HotSpotX, MouseScreen.y - HotSpotY);

// p�eta�en� obr�zku na aktu�ln� pozici my�i
		::ImageList_DragMove(MouseScreen.x - HotSpotX, MouseScreen.y - HotSpotY);

// zah�jen� zachyt�v�n� my�i
		::SetCapture(MainFrame);
	}
	else
	{

// ukon�en� ta�en�, pokud bylo tla��tko uvoln�no mimo oblast
		if (MDraging && ((flags & (MK_LBUTTON | MK_RBUTTON)) == 0))
		{
			EndDrag();
		}
		else
		{

// pokra�ov�n� v ta�en�
			if (MDraging)
			{

// na�ten� sou�adnice kurzoru my�i
//				POINT pt;
//				::GetCursorPos(&pt);

// p�eta�en� obr�zku na aktu�ln� pozici my�i
				::ImageList_DragMove(MouseScreen.x - HotSpotX, MouseScreen.y - HotSpotY);

// nastaven� kurzoru my�i
				HCURSOR curnew = CurAkt;

				if ((MouseX == MOUSEINV) || (MouseY == MOUSEINV))
				{
					if (CopyMode)
					{
						curnew = CurNoDrag;
					}
					else
					{
						curnew = CurDelete;
					}
				}
				else
				{
					if (CopyMode)
					{
						curnew = CurCopy;
					}
					else
					{
						curnew = CurMove;
					}
				}
						
				if (curnew != CurAkt)
				{
					CurAkt = curnew;
					::SetCursor(CurAkt);
				}
			}
		}
	}

// zobrazen� sou�adnic my�i
	DispMouseXY();
}

/////////////////////////////////////////////////////////////////////////////
// uvoln�n� tla��tka my�i

void OnButtonUp(UINT keys, BOOL right)
{
	if (MFilling)
	{
		EndFill();
		return;
	}

	if (MDraging)
	{
		MAPITEM* data = Map[Index].DataData();
		MAPITEM* olddata = data + (DragX + DragY*Width);
		BOOL modi = FALSE;

		if ((DragX != MouseX) || (DragY != MouseY))
		{
			int olditem = 0;
			DWORD oldparam = 0;

			if ((MouseX != MOUSEINV) && (MouseY != MOUSEINV))
			{
				MAPITEM* newdata = data + (MouseX + MouseY*Width);

				olditem = newdata->Icon;
				oldparam = newdata->Param;

				if ((newdata->Icon != olddata->Icon) ||
					(newdata->Param != olddata->Param))
				{
					PushUndo();
					data = Map[Index].DataData();
					olddata = data + (DragX + DragY*Width);
					newdata = data + (MouseX + MouseY*Width);

//					Map[Index].
						RefIconDec(olditem);
//					Map[Index].
						RefIconInc(olddata->Icon);

					newdata->Icon = olddata->Icon;
					newdata->Param = olddata->Param;
					modi = TRUE;
				}
			}

			if (!CopyMode)
			{
				if ((olddata->Icon != olditem) || 
					(olddata->Param != oldparam))
				{
					if (!modi)
					{
						PushUndo();
						data = Map[Index].DataData();
						olddata = data + (DragX + DragY*Width);
					}

//					Map[Index].
						RefIconDec(olddata->Icon);
//					Map[Index].
						RefIconInc(olditem);

					olddata->Icon = olditem;
					olddata->Param = oldparam;
					modi = TRUE;
				}
			}

			if (modi)
			{
				SetModi();
				ProgAktItem();
			}
		}

		EndDrag();
		Disp();
	}
	else

// navigace ikony (nen�-li ta�en� a nen�-li p�eru�en� druh�m tla��tkem)
	{
		if (!EditMap && BegDrag && (MouseX != MOUSEINV) && (MouseY != MOUSEINV))
		{
			int icon = (Map[Index].DataData() + (MouseX + MouseY*Width))->Icon;
			BufObj.Select(BufObj.SrcDat(BufIcoID, icon));
			BufLoc.Select(BufLoc.SrcDat(BufIcoID, icon));
		}
	}

	BegDrag = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// polo�en� ikony zvn�j�ku

void DragDrop(int icon)
{
// ukon�en� editace ��seln�ho parametru
	EndEditSwcNum();

	int inx = MouseX + MouseY*Width;

	if ((EditMode != BufMapID) ||
		Map.IsNotValid(Index) ||
		Icon.IsNotValid(icon) ||
		((DWORD)MouseX >= (DWORD)Width) ||
		((DWORD)MouseY >= (DWORD)Height) ||
		((Map[Index].DataData())[inx].Icon == icon))
	{
		return;
	}

	PushUndo();

//	Map[Index].
		RefIconDec((Map[Index].DataData())[inx].Icon);
//	Map[Index].
		RefIconInc(icon);

	(Map[Index].DataData())[inx].Icon = icon;

	SetModi();
	ProgAktItem();
	EndDrag();
	Disp();
}


/////////////////////////////////////////////////////////////////////////////
// vstup kl�vesy z kl�vesnice (TRUE=obslou�eno)

BOOL OnKeyDown(int key)
{
// obsluha edita�n�ho pole ��seln�ho parametru
	if (EditMapNumWnd)
	{
		switch (key)
		{
		case VK_RETURN:
		case VK_TAB:
		case VK_ESCAPE:
			EndEditSwcNum();
			return TRUE;

		case VK_LEFT:
			BegEditSwcNum(EditNumX-1, EditNumY);
			return TRUE;

		case VK_RIGHT:
			BegEditSwcNum(EditNumX+1, EditNumY);
			return TRUE;

		case VK_UP:
			BegEditSwcNum(EditNumX, EditNumY+1);
			return TRUE;

		case VK_DOWN:
			BegEditSwcNum(EditNumX, EditNumY-1);
			return TRUE;
		}
	}
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� edita�n�ho m�du

void SetMode(int mode)
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// ukon�en� editace ��seln�ho parametru
	EndEditSwcNum();

// kontrola, zda je m�d ji� nastaven
	if (Mode == mode) return;

// nastaven� m�du
	Mode = mode;

// p�ekreslen� displeje
	ReDisp();

// aktualizace voleb
	UpdateMenu();

// aktualizace voleb bloku
	UpdateClipboard();
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace voleb v�b�ru m�du

void UpdateMenu()
{
	SelectCombo(ToolEditMap, Mode, ToolBarEditMapTools);
	CheckCommand(IDN_RASTR, Rastr);
	CheckCommand(IDN_FILLMAP, FillMap);
	CheckCommand(IDN_EDITMAP, EditMap);
}


/////////////////////////////////////////////////////////////////////////////
// m�d v�pln�

void OnFillMap()
{
	if (!ProgMode) return;
	EndEditSwcNum();
	FillMap = !FillMap;
	EditMap = FALSE;
	UpdateMenu();
}


/////////////////////////////////////////////////////////////////////////////
// edita�n� m�d

void OnEditMap()
{
	if (!ProgMode) return;
	EndEditSwcNum();
	EditMap = !EditMap;
	FillMap = FALSE;
	UpdateMenu();
}


// ****************************** obsluha bloku ******************************

/////////////////////////////////////////////////////////////////////////////
// v�b�r v�eho

void SelectAll()
{
// v�b�r v�eho
	if (EditMapNumWnd)
	{
		::SendMessage(EditMapNumWnd, EM_SETSEL, 0, (LPARAM)-1);
	}

// aktualizace voleb bloku
	UpdateClipboard();
}

/////////////////////////////////////////////////////////////////////////////
// zru�en� bloku

void Delete()
{
// operace CLEAR
	if (EditMapNumWnd)
	{
		::SendMessage(EditMapNumWnd, WM_CLEAR, 0, 0);
	}

// aktualizace voleb bloku
	UpdateClipboard();
}

/////////////////////////////////////////////////////////////////////////////
// kopie bloku

void Copy()
{
// operace COPY
	if (EditMapNumWnd)
	{
		::SendMessage(EditMapNumWnd, WM_COPY, 0, 0);
	}

// aktualizace voleb bloku
	UpdateClipboard();
}

/////////////////////////////////////////////////////////////////////////////
// vyst��en� bloku

void Cut()
{
// operaceCUT
	if (EditMapNumWnd)
	{
		::SendMessage(EditMapNumWnd, WM_CUT, 0, 0);
	}

// aktualizace voleb bloku
	UpdateClipboard();
}

/////////////////////////////////////////////////////////////////////////////
// navr�cen� bloku

void Paste()
{
// operace PASTE
	if (EditMapNumWnd)
	{
		::SendMessage(EditMapNumWnd, WM_PASTE, 0, 0);
	}

// aktualizace voleb bloku
	UpdateClipboard();
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace voleb bloku

void UpdateClipboard()
{
	if ((FocusBuf == BufEdiID) && (EditMode == BufMapID))
	{
		if (EditMapNumWnd)
		{
			int start, end;
			::SendMessage(EditMapNumWnd, EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
			BOOL sel = (start != end);
			EnableCommand(IDN_CUT, sel);
			EnableCommand(IDN_COPY, sel);
			EnableCommand(IDN_PASTE, ::IsClipboardFormatAvailable(CF_TEXT));
			EnableCommand(IDN_CLEAR, sel);
			EnableCommand(IDN_ALL, ::GetWindowTextLength(EditMapNumWnd) > 0);
		}
		else
		{
			EnableCommand(IDN_CUT, FALSE);
			EnableCommand(IDN_COPY, FALSE);
			EnableCommand(IDN_PASTE, FALSE);
			EnableCommand(IDN_CLEAR, FALSE);
			EnableCommand(IDN_ALL, FALSE);
		}
	}
}


// ****************************** obsluha UNDO ******************************


/////////////////////////////////////////////////////////////////////////////
// �schova plochu pro UNDO

void PushUndo()
{
	Undo.AddMapSet(Index, Map[Index], Mode);
	Map[Index].CopyWrite();
	Undo.Stop();
}

}
