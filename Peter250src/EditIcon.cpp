
#include "Main.h"

/***************************************************************************\
*																			*
*							Editor ikon a obr�zk�							*
*																			*
\***************************************************************************/

namespace EditIcon
{

/////////////////////////////////////////////////////////////////////////////
// parametry

// v�b�r barev
BYTE	ColLeft;						// barva pro lev� tla��tko
BYTE	ColRight;						// barva pro prav� tla��tko
BYTE	ColInd = 255;					// barva indik�toru barev (255 = nen�)

// parametry ikony/obr�zku
int		BufID = -1;						// editovan� buffer (-1 = nen�)
int		Index = -1;						// editovan� ikona v bufferu ikon/obr�zk� (-2 = sprajt)
int		Width = 32;						// ���ka obr�zku v bodech
int		Height = 32;					// v��ka obr�zku v bodech
BYTE*	Data = NULL;					// data obr�zku

// zadan� nov� rozm�ry obr�zku (v bodech)
int		NewDimWidth;					// ���ka obr�zku
int		NewDimHeight;					// v��ka obr�zku
BOOL	NewDimPoint = FALSE;			// rozm�ry zad�ny v bodech
BOOL	NewDimFill = FALSE;				// prob�h� programov� pln�n� dialogu

// zm�na velikosti obr�zku
int		ResizeOldWidth = 640;			// p�vodn� ���ka bloku
int		ResizeOldHeight = 480;			// p�vodn� v��ka bloku
int		ResizeWidth = 640;				// zadan� nov� ���ka
int		ResizeHeight = 480;				// zadan� nov� v��ka
double	ResizeWidthProc = 100;			// nov� ���ka zadan� v procentech
double	ResizeHeightProc = 100;			// nov� v��ka zadan� v procentech
BOOL	ResizeFill = FALSE;				// prob�h� programov� pln�n� dialogu
BOOL	ResizeAktWidth = TRUE;			// je aktivn� pole ���ky

enum {
	ResizeModeStep = 0,					// rozm�ry zad�ny v kroc�ch
	ResizeModePoint,					// rozm�ry zad�ny v bodech
	ResizeModeProc,						// rozm�ry zad�ny v procentech
};

int		ResizeMode = ResizeModeProc;	// zadan� typ rozm�r�
BOOL	ResizeModeProp = TRUE;			// zachovat pom�r stran
BOOL	ResizeModeInt = TRUE;			// prov�d�t interpolaci
BOOL	ResizeModeDith = TRUE;			// prov�d�t dithering

// zobrazen� ��st obr�zku (v bodech obr�zku, bez p�epo�tu na displej)
int		DispLeft = 0;					// po��tek zobrazen� ��sti obr�zku zleva
int		DispBottom = 0;					// po��tek zobrazen� ��sti obr�zku zdola
int		DispWidth = 32;					// ���ka zobrazen� ��sti obr�zku
int		DispHeight = 32;				// v��ka zobrazen� ��sti obr�zku
int		MidX = -1;						// nastaven� st�ed obr�zku X
int		MidY = -1;						// nastaven� st�ed obr�zku Y
BOOL	Rastr = TRUE;					// zobrazen� rastru

// aktualizace zobrazen� obr�zku
int		AktLeft;						// po��tek vlevo
int		AktTop;							// po��tek naho�e
int		AktRight;						// konec vpravo
int		AktBottom;						// konec dole

// zobrazen� obr�zku na displeji (v bodech displeje)
int		DLeft;							// po��tek zobrazen� vlevo
int		DTop;							// po��tek zobrazen� naho�e
int		DWidth;							// zobrazen� ���ka ��sti obr�zku na displeji
int		DHeight;						// zobrazen� v��ka ��sti obr�zku na displeji
int		Zoom = 1;						// aktu�ln� m���tko obr�zku/ikony
int		ZoomIcon = -1;					// m���tko ikony
int		ZoomPic = -1;					// m���tko obr�zku
#define MAXZOOM 64						// maxim�ln� m���tko (mocnina 2 !!!)

// pole v�b�ru barev
int		ColsX;							// po�et barev horizont�ln�
int		ColsXByte;						// ���ka obr�zku barev v bajtech
int		ColsY;							// po�et barev vertik�ln�
int		ColsLeft;						// po��tek pole barev X
int		ColsTop;						// po��tek pole barev Y
int		ColsWidth;						// zobrazen� ���ka pole barev
int		ColsHeight;						// zobrazen� v��ka pole barev
int		ColsIncX;						// p��rustek barev X
int		ColsIncY;						// p��rustek barev Y
#define	MINCOLSIR 6						// minim�ln� ���ka pol��ka k zobrazen� r�mu v�b�ru barvy

// volba fontu
int		FontWeight = FW_NORMAL;			// v�ha fontu (BOLD)
BOOL	FontItalic = FALSE;				// font typu ITALIC
int		FontSize = 32;					// velikost fontu
CText	FontName;						// jm�no fontu
int		FontCharSet = EASTEUROPE_CHARSET; //DEFAULT_CHARSET; // znakov� sada
CText	FontText;						// zadan� text

// obsluha kreslen�
int		Mode = IDN_PEN;					// edita�n� m�d (k�d IDN_xxx)
int		OldMode = IDN_PEN;				// uschovan� edita�n� m�d (k�d IDN_xxx)
int		PenWidth = 1;					// ���ka pera
int		PenWidthID = IDN_WIDTH1;		// ID volby pera
BOOL	Editing = FALSE;				// p��znak prob�haj�c� editov�n�
int		OldX = 0;						// p�vodn� sou�adnice kreslen� X
int		OldY = 0;						// p�vodn� sou�adnice kreslen� Y
int		TopX = 0;						// po��te�n� sou�adnice ta�en� X
int		TopY = 0;						// po��te�n� sou�adnice ta�en� Y
BYTE*	PushBuf = NULL;					// buffer k mezi�schov� obr�zku

// obsluha bloku
BOOL	Selecting = FALSE;				// p��znak ozna�en� bloku
BOOL	Moving = FALSE;					// p��znak p�esouv�n� bloku
BYTE*	BlokBuf = NULL;					// buffer s blokem
int		BlokX = 0;						// po��tek bloku X
int		BlokY = 0;						// po��tek bloku Y
int		BlokWidth = 32;					// ���ka bloku
int		BlokHeight = 32;				// v��ka bloku
BYTE	BlokBack;						// pr�hledn� barva bloku

// prvn� ozna�en� bloku
BOOL	CutMode = FALSE;				// v�choz� sou�adnice bloku
int		FirstBlokX = 0;					// v�choz� po��tek bloku X
int		FirstBlokY = 0;					// v�choz� po��tek bloku Y
int		FirstBlokWidth = 0;				// v�choz� ���ka bloku
int		FirstBlokHeight = 0;			// v�choz� v��ka bloku

// tabulka k�d� pro vykreslen� tla��tek p�i editaci bloku
int EditBlokTab[] = {
	ButtonYFlip,		IDN_YFLIP,
	ButtonXFlip,		IDN_XFLIP,
	ButtonLRot,			IDN_LROT,
	ButtonLRot67,		IDN_LROT67,
	ButtonLRot60,		IDN_LROT60,
	ButtonLRot45,		IDN_LROT45,
	ButtonLRot30,		IDN_LROT30,
	ButtonLRot22,		IDN_LROT22,
	ButtonRRot,			IDN_RROT,
	ButtonRRot67,		IDN_RROT67,
	ButtonRRot60,		IDN_RROT60,
	ButtonRRot45,		IDN_RROT45,
	ButtonRRot30,		IDN_RROT30,
	ButtonRRot22,		IDN_RROT22,
	ButtonMul2,			IDN_MUL2,
	ButtonDiv2,			IDN_DIV2,
	ButtonXCol,			IDN_XCOL,
	ButtonSCol,			IDN_SCOL,
	ButtonMulDiv,		IDN_MULDIV,
	-1
};


// ************************** obsluha volby editace *************************


/////////////////////////////////////////////////////////////////////////////
// inicializace p�i startu programu

void StartInit()
{
// p��prava barev
	ColLeft = WhiteCol;					// barva lev�ho tla��tka
	ColRight = BackCol;					// barva prav�ho tla��tka
	BlokBack = BackCol;					// pr�hledn� barva bloku

// p��prava rozm�r� obr�zku barev
	ColsX = ColCol+1;					// po�et barev horizont�ln�
	ColsXByte = (ColCol+1+3) & ~3;		// d�lka linky obr�zku barev v bajtech
	ColsY = ColLev;						// po�et barev vertik�ln�

// jm�no fontu, zadan� text
	FontName = _T("Times New Roman");	// jm�no fontu
}


/////////////////////////////////////////////////////////////////////////////
// zah�jen� editace (mus� b�t platn� index!) (-2 = ikona sprajtu)

void BegEdit(int bufID, int index)
{
// nulov�n� p�ep�na��
	Selecting = FALSE;
	Editing = FALSE;
	Moving = FALSE;

// editace obr�zk�
	if (bufID == BufPicID)
	{
		ASSERT ((index == -2) || Picture.IsValid(index));

		CPicture* pic;

		if (index == -2)
		{
			EditItemSprite = EditSprite::Index;
			EditItemPic = EditSprite::IndexPic;
			pic = &Sprite[EditItemSprite][EditItemPic];
		}
		else
		{
			pic = &(Picture[index]);	// adresa obr�zku
		}

		pic->CopyWrite();			// kopie p�ed z�pisem
		pic->DeComp();
		Width = pic->Width();		// ���ka obr�zku
		Height = pic->Height();		// v��ka obr�zku
		Data = pic->DataData();		// adresa dat obr�zku

		if (ZoomPic <= 0)
		{
			ZoomPic = (EditWidth-TRACKSIRKA-4)/Width;
			int z2 = (EditHeight*5/6-TRACKSIRKA-4)/Height;
			if (ZoomPic > z2) ZoomPic = z2;
			if (ZoomPic <= 0) ZoomPic = 1;
			if (ZoomPic > MAXZOOM) ZoomPic = MAXZOOM;
/*	
			z2 = 1;
			while ((2*z2 <= ZoomPic) && (z2 < MAXZOOM))
			{
				z2 *= 2;
			}
			ZoomPic = z2;
*/
		}

		Zoom = ZoomPic;
	}
	else

// editace ikon
	{
		ASSERT(Icon.IsValid(index));

		CIcon* editicon;

		editicon = &(Icon[index]);		// adresa ikony

		editicon->CopyWrite();			// kopie p�ed z�pisem
		editicon->DeComp();
		Width = ICONWIDTH;				// ���ka ikony
		Height = ICONHEIGHT;			// v��ka ikony
		Data = editicon->DataData();	// adresa dat ikony

		if (ZoomIcon <= 0)
		{
			ZoomIcon = (EditWidth-TRACKSIRKA-4)/ICONWIDTH;
			int z2 = (EditHeight*5/6-TRACKSIRKA-4)/ICONHEIGHT;
			if (ZoomIcon > z2) ZoomIcon = z2;
			if (ZoomIcon <= 0) ZoomIcon = 1;
			if (ZoomIcon > MAXZOOM) ZoomIcon = MAXZOOM;
/*	
			z2 = 1;
			while ((2*z2 <= ZoomIcon) && (z2 < MAXZOOM))
			{
				z2 *= 2;
			}
			ZoomIcon = z2;
*/
		}

		Zoom = ZoomIcon;
	}

// p��prava dat
	BufID = bufID;						// m�d editace ikon/obr�zku
	Index = index;						// index ikony/obr�zku
	PushBuf = (BYTE*)MemSize(PushBuf, Width * Height * sizeof(BYTE));

// st�ed zv�t�en�
	if ((DWORD)MidX >= (DWORD)Width) MidX = Width/2;
	if ((DWORD)MidY >= (DWORD)Height) MidY = Height/2;

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

	if (NewDimPoint)
	{
		txt.Int(NewDimWidth);
	}
	else
	{
		txt.Double((double)NewDimWidth/ICONWIDTH);
	}

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

		if (NewDimPoint)
		{
			NewDimWidth = Int(txt);
		}
		else
		{
			NewDimWidth = (int)(Double(txt)*ICONWIDTH + 0.5);
		}
	}
}

// nastaven� textu v��ky
void DimenDialSetHeight(HWND wnd)
{
	CText txt;

	if (NewDimPoint)
	{
		txt.Int(NewDimHeight);
	}
	else
	{
		txt.Double((double)NewDimHeight/ICONHEIGHT);
	}

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

		if (NewDimPoint)
		{
			NewDimHeight = Int(txt);
		}
		else
		{
			NewDimHeight = (int)(Double(txt)*ICONHEIGHT + 0.5);
		}
	}
}

// nastaven� p�ep�na�e volby jednotek
void DimenDialSetSwc(HWND wnd)
{
	NewDimFill = TRUE;
	DialogSetCheck(wnd, IDC_KROK, !NewDimPoint);
	DialogSetCheck(wnd, IDC_BOD, NewDimPoint);
	NewDimFill = FALSE;
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

			txt.Load(IDN_PICDIMEN);
			txt.SetWindowTextFont(hWnd);

			DialogCustomText(hWnd, IDN_SIRKA);
			DialogCustomText(hWnd, IDN_VYSKA);
			DialogCustomBox(hWnd, IDN_ROZMERY);
			DialogCustomRadio(hWnd, IDC_KROK);
			DialogCustomRadio(hWnd, IDC_BOD);

			txt.Load(IDN_CANCEL0);
			txt.SetDialogTextFont(hWnd, IDCANCEL);

			DimenDialSetWidth(hWnd);
			DimenDialSetHeight(hWnd);
			DimenDialSetSwc(hWnd);
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

			case IDC_KROK:
			case IDC_BOD:
				{
					BOOL old = NewDimPoint;

					DimenDialGetWidth(hWnd);
					DimenDialGetHeight(hWnd);

					NewDimPoint = (LOWORD(wParam) == IDC_BOD);
					DimenDialSetSwc(hWnd);

					if (old != NewDimPoint)
					{
						DimenDialSetWidth(hWnd);
						DimenDialSetHeight(hWnd);
					}
				}
				break;
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
// nastaven� rozm�r� obr�zku

void OnDimen()
{
// kontrola re�imu
	if (BufID != BufPicID) return;

// p��prava v�choz�ch rozm�r�
	NewDimWidth = Width;
	NewDimHeight = Height;

// zad�n� nov�ch rozm�r� obr�zku
	if (::DialogBoxParam(
			hInstance,
			MAKEINTRESOURCE(IDN_PICDIMEN),
			MainFrame,
			(DLGPROC)DimenDialogProc,
			(LPARAM)IDN_PICDIMEN) != IDOK) return;

// korekce zadan�ch �daj�
	if (NewDimWidth < 1) NewDimWidth = Width;
	if (NewDimWidth > 0x8000) NewDimWidth = 0x8000;
	if (NewDimHeight < 1) NewDimHeight = Height;
	if (NewDimHeight > 0x8000) NewDimHeight = 0x8000;

// kontrola, zda byly rozm�ry zm�n�ny
	if ((NewDimWidth == Width) && (NewDimHeight == Height))
	{
		return;
	}

// zapnut� kurzoru �ek�n�
	BeginWaitCursor();

// vypnut� ozna�en� bloku
	DispSelect();

// zru�en� m�du bloku
	if ((Mode == IDN_TEXT) || (Mode == IDN_SELECT))
	{
		SetMode(OldMode);
	}

// nulov�n� p�ep�na��
	Selecting = FALSE;
	Editing = FALSE;
	Moving = FALSE;

// nastaven� nov�ch rozm�r� obr�zku
	Width = NewDimWidth;
	Height = NewDimHeight;

	if (Index == -2)
	{
		Undo.AddSprSet(EditItemSprite, Sprite[EditItemSprite]);
		Sprite[EditItemSprite].CopyWrite();

		for (int ii = Sprite[EditItemSprite].Faze()*Sprite[EditItemSprite].Smer()-1; ii >= 0; ii--)
		{
//			Sprite[EditItemSprite][ii].CopyWrite();
			Sprite[EditItemSprite][ii].Resize(Width, Height);
		}
		Sprite[EditItemSprite][EditItemPic].DeComp();
		Data = Sprite[EditItemSprite][EditItemPic].DataData();

		Undo.Stop();
	}
	else
	{
		PushUndo();

		ASSERT(Picture.IsValid(Index));
		Picture[Index].Resize(Width, Height);
		Picture[Index].DeComp();
		Data = Picture[Index].DataData();
	}

	PushBuf = (BYTE*)MemSize(PushBuf, Width * Height * sizeof(BYTE));
	if ((DWORD)MidX >= (DWORD)Width) MidX = Width/2;
	if ((DWORD)MidY >= (DWORD)Height) MidY = Height/2;

// p�epo�et zobrazen�
	ReCalc();
	DispLeft = MidX - DispWidth/2;
	DispBottom = MidY - DispHeight/2;
	ReCalc();
	SetScroll();
	ProgOnSize();

// p�ekreslen� displeje
	Disp();

// p��znak modifikace
	SetModi();

// aktualizace voleb bloku
	UpdateClipboard();

// aktualizace zobrazen� strom�
	UpdateTree();

// vypnut� kurzoru �ek�n�
	EndWaitCursor();
}


// ***************************** obsluha zobrazen� **************************


/////////////////////////////////////////////////////////////////////////////
// p�epo�et rozm�r�

void ReCalc()
{
// v�po�et horizont�ln�ch parametr� pole pro v�b�r barev
	ColsIncX = (EditWidth - 4) / ColsX;		// p��rustek barev X
	if (ColsIncX < 1) ColsIncX = 1;			// mus� b�t p��rustek min. 1
	ColsWidth = ColsIncX * ColsX;				// zobrazen� ���ka pole barev
	ColsLeft = EditX + (EditWidth - ColsWidth)/2; // lev� okraj pole barev

// v�po�et vertik�ln�ch parametr� pole pro v�b�r barev
	ColsIncY = (EditHeight - 4) / 6 / ColsY;	// p��rustek barev Y
	if (ColsIncY < 1) ColsIncY = 1;			// mus� b�t p��rustek min. 1
	ColsHeight = ColsIncY * ColsY;				// zobrazen� ���ka pole barev
	ColsTop = EditY + EditHeight - 2 - ColsHeight; // horn� okraj pole barev

// p��prava zobrazen� v��ky (korekce, je-li horizont�ln� posuvn�k)
	DHeight = ColsTop - EditY - 2 - 1 - 4;		// v��ka k zobrazen�
	if (EditWidth - 4 - TRACKSIRKA < Width*Zoom)
	{
		DHeight -= TRACKSIRKA;				// sn�en� v��ky o horizont�ln� posuvn�k
	}
	if (DHeight < 1) DHeight = 1;			// minim�ln� ���ka

// p��prava zobrazen� ���ky (korekce, je-li vertik�ln� posuvn�k)
	DWidth = EditWidth - 4 - 1 - 4;			//	���ka k zobrazen�
	if (DHeight < Height*Zoom)
	{
		DWidth -= TRACKSIRKA;				// sn�en� ���ky o vertik�ln� posuvn�k
	}
	if (DWidth < 1) DWidth = 1;				// minim�ln� ���ka

// po�et zobrazen�ch bod�
	DispWidth = DWidth/Zoom;				// po�et zobrazen�ch bod� horizont�ln�
	if (DispWidth < 1) DispWidth = 1;		// minim�ln� po�et bod� horizont�ln�
	if (DispWidth > Width) DispWidth = Width; // omezen� na skute�nou ���ku
	DispHeight = DHeight/Zoom;			// po�et zobrazen�ch bod� vertik�ln�
	if (DispHeight < 1) DispHeight = 1;		// minim�ln� po�et bod� vertik�ln�
	if (DispHeight > Height) DispHeight = Height; // omezen� na skute�nou v��ku

// omezen� po��tku zobrazen�
	if (DispLeft < 0) DispLeft = 0;
	if (DispLeft > Width - DispWidth) 
		DispLeft = Width - DispWidth;
	if (DispBottom < 0) DispBottom = 0;
	if (DispBottom > Height - DispHeight)
		DispBottom = Height - DispHeight;

// up�esn�n� �daj� k zobrazen�
	DLeft = EditX + 2 + 2 + (DWidth - Zoom*DispWidth)/2;
//	if (DispWidth < Width) DLeft = EditX + 2;
	DTop = EditY + 2 + 2 + (DHeight - Zoom*DispHeight)/2;
//	if (DispHeight < Height) DTop = EditY + 2;
	DWidth = Zoom*DispWidth;
	DHeight = Zoom*DispHeight;

// p��znak zapnut� posuvn�k�
	HScrollDisp = (DispWidth < Width);
	VScrollDisp = (DispHeight < Height);
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

// vykreslen� obr�zku
	AktLeft = 0;						// po��tek vlevo
	AktRight = Width;					// konec vpravo
	AktTop = 0;							// po��tek naho�e
	AktBottom = Height;					// konec dole
	DispAkt(dc);				// p�ekreslen� obr�zku

// zobrazen� r�mu kolem obr�zku
	DispRam(dc);

// zobrazen� barev
	for (int col = StdColors-1; col >= 0; col--)
	{
		DispCol(dc, col);
	}

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
	DispAkt(dc);				// p�ekreslen� obr�zku

// zobrazen� r�mu kolem obr�zku
	DispRam(dc);

// n�vrat p�vodn�ch palet
	::SelectPalette(dc,OldPal,FALSE);

// uvoln�n� kontextu displeje
	::ReleaseDC(MainFrame, dc);
}


/////////////////////////////////////////////////////////////////////////////
// p�ekreslen� okraj� kolem obr�zku

void DispRam(HDC dc)
{
// rozm�ry k zobrazen� podkladu
	int dleft = DLeft;
	int dright = dleft + DWidth;
	int dtop = DTop;
	int dbottom = dtop + DHeight;

// test, zda je zapnuto zobrazen� rastru
	if (Rastr)
	{

// p��prava a v�b�r pera k zobrazen� okraje
		HPEN pen = ::CreatePen(PS_SOLID, 1, 0x0000ff);
		HPEN oldpen = (HPEN)::SelectObject(dc, pen);

// zobrazen� horn�ho okraje
		if ((Zoom == 1) && (DispBottom + DispHeight == Height) && (dtop > EditY + 2))
		{
			dtop--;
			::MoveToEx(dc, dleft, dtop, NULL);
			::LineTo(dc, dright, dtop);
		}

// zobrazen� prav�ho okraje
		if ((Zoom == 1) && (DispLeft + DispWidth == Width) && (dright < EditX + EditWidth - 2))
		{
			::MoveToEx(dc, dright, dtop, NULL);
			::LineTo(dc, dright, dbottom);
			dright++;
		}

// zobrazen� lev�ho okraje
		BOOL lleft = ((DispLeft == 0) && (dleft > EditX + 2));
		if (lleft)
		{
			dleft--;
			::MoveToEx(dc, dleft, dtop, NULL);
			::LineTo(dc, dleft, dbottom);
		}

// zobrazen� spodn�ho okraje
		BOOL lbottom = ((DispBottom == 0) && (dbottom < EditY + EditHeight - 2));
		if (lbottom)
		{
			::MoveToEx(dc, dleft, dbottom, NULL);
			::LineTo(dc, dright, dbottom);
			dbottom++;
		}

// n�vrat a zru�en� pera k zobrazen� okraje
		::SelectObject(dc, oldpen);
		::DeleteObject(pen);

// test, zda bude modr� nebo �ern� okraj
		if (Zoom > 1)
		{

// p��prava a v�b�r pera k zobrazen� modr�ho okraje
			pen = ::CreatePen(PS_SOLID, 1, 0xff0000);
			oldpen = (HPEN)::SelectObject(dc, pen);

// zobrazen� lev�ho okraje
			if (!lleft && (DispLeft % 32 == 0) && (dleft > EditX + 2))
			{
				lleft = TRUE;
				dleft--;
				::MoveToEx(dc, dleft, dtop, NULL);
				::LineTo(dc, dleft, dbottom);
			}

// zobrazen� doln�ho okraje
			if (!lbottom && (DispBottom % 32 == 0) && (dbottom < EditY + EditHeight - 2))
			{
				lbottom = TRUE;
				::MoveToEx(dc, dleft, dbottom, NULL);
				::LineTo(dc, dright, dbottom);
				dbottom++;
			}

// n�vrat a zru�en� pera k zobrazen� okraje
			::SelectObject(dc, oldpen);
			::DeleteObject(pen);

// test, zda bude �ern� okraj
			if (Zoom >= 8)
			{

// p��prava a v�b�r pera k zobrazen� �ern�ho okraje
				pen = ::CreatePen(PS_SOLID, 1, 0);
				oldpen = (HPEN)::SelectObject(dc, pen);

// zobrazen� lev�ho okraje
				if (!lleft && (dleft > EditX + 2))
				{
					dleft--;
					::MoveToEx(dc, dleft, dtop, NULL);
					::LineTo(dc, dleft, dbottom);
				}

// zobrazen� doln�ho okraje
				if (!lbottom && (dbottom < EditY + EditHeight - 2))
				{
					::MoveToEx(dc, dleft, dbottom, NULL);
					::LineTo(dc, dright, dbottom);
					dbottom++;
				}

// n�vrat a zru�en� pera k zobrazen� okraje
				::SelectObject(dc, oldpen);
				::DeleteObject(pen);
			}
		}
	}

// p��prava �t�tce k vymaz�n� podkladu
	HBRUSH brush = (HBRUSH)::GetStockObject(LTGRAY_BRUSH); // �t�tec k vymaz�n� plochy

// vymaz�n� plochy naho�e nad obr�zkem
	RECT rc;
	rc.left = EditX + 2;
	rc.right = EditX + EditWidth - 2;
	rc.top = EditY + 2;
	rc.bottom = dtop;
	if (rc.top < rc.bottom)
	{
		::FillRect(dc, &rc, brush);
	}

// vymaz�n� plochy dole pod obr�zkem (left a right je nastaveno)
	rc.top = dbottom;
	rc.bottom = ColsTop;
	if (rc.top < rc.bottom)
	{
		::FillRect(dc, &rc, brush);
	}

// vymaz�n� plochy vlevo od obr�zku (left je nastaveno)
	rc.right = dleft;
	rc.top = dtop;
	rc.bottom = dbottom;
	if (rc.left < rc.right)
	{
		::FillRect(dc, &rc, brush);
	}

// vymaz�n� plochy vpravo od obr�zku (top a bottom je nastaveno)
	rc.left = dright;
	rc.right = EditX + EditWidth - 2;
	if (rc.left < rc.right)
	{
		::FillRect(dc, &rc, brush);
	}

// vymaz�n� plochy vlevo od pole barev
	rc.left = EditX + 2;
	rc.right = ColsLeft;
	rc.top = ColsTop;
	rc.bottom = EditY + EditHeight - 2;
	if (rc.left < rc.right)
	{
		::FillRect(dc, &rc, brush);
	}

// vymaz�n� plochy vpravo od pole barev (top a bottom je nastaveno)
	rc.left = ColsLeft + ColsWidth;
	rc.right = EditX + EditWidth - 2;
	if (rc.left < rc.right)
	{
		::FillRect(dc, &rc, brush);
	}

// zru�en� �t�tce podkladu (i kdy� podle dokumentace ru�en� nen� nutn�)
	::DeleteObject(brush);
}


/////////////////////////////////////////////////////////////////////////////
// aktualiza�n� p�ekreslen� obr�zku

void DispAkt(HDC dc)
{
// omezen� hranic k zobrazen�
	if (AktLeft < DispLeft) AktLeft = DispLeft;
	if (AktTop < (Height - DispBottom - DispHeight))
		AktTop = Height - DispBottom - DispHeight;
	if (AktRight > (DispLeft + DispWidth))
		AktRight = DispLeft + DispWidth;
	if (AktBottom > Height - DispBottom)
		AktBottom = Height - DispBottom;

// p��prava zdrojov�ch sou�adnic k zobrazen�
	int srcLeft = AktLeft;
	int srcWidth = AktRight - srcLeft;

// !!!!!!!!!!!! dopl�kov� ochrana (hav�rie p�i UNDO po zm�n� velikosti obr�zk� sprajtu)
	if (srcWidth > Width) srcWidth = Width;

	int srcTop = AktTop;
	int srcHeight = AktBottom - srcTop;

// !!!!!!!!!!!! dopl�kov� ochrana (hav�rie p�i UNDO po zm�n� velikosti obr�zk� sprajtu)
	if (srcHeight > Height) srcHeight = Height;

// zneplatn�n� hranic k zobrazen�
	AktLeft = Width;
	AktRight = 0;
	AktTop = Height;
	AktBottom = 0;

// kontrola, zda je co zobrazit
	if ((srcWidth <= 0) || (srcHeight <= 0)) return;

// p��prava c�lov�ch sou�adnic k zobrazen�
	int dstLeft = DLeft + (srcLeft - DispLeft)*Zoom;
	int dstTop = DTop + (srcTop - (Height - DispBottom - DispHeight))*Zoom;
	int dstWidth = srcWidth*Zoom;
	int dstHeight = srcHeight*Zoom;
	int dstWidthByte = (dstWidth + 3) & ~3;

// adresa dat k zobrazen�
	if (BufID == BufPicID)
	{
		if (Index == -2)
		{
			Sprite[EditItemSprite][EditItemPic].DeComp();

// !!!!!!!!!!!! dopl�kov� ochrana (hav�rie p�i UNDO po zm�n� velikosti obr�zk� sprajtu)
			if ((Sprite[EditItemSprite][EditItemPic].Width() != Width) ||
				(Sprite[EditItemSprite][EditItemPic].Height() != Height)) return;

			Data = Sprite[EditItemSprite][EditItemPic].DataData();
		}
		else
		{
			Picture[Index].DeComp();

// !!!!!!!!!!!! dopl�kov� ochrana (hav�rie p�i UNDO po zm�n� velikosti obr�zk� sprajtu)
			if ((Picture[Index].Width() != Width) || (Picture[Index].Height() != Height)) return;

			Data = Picture[Index].DataData();
		}
	}
	else
	{
		Icon[Index].DeComp();
		Data = Icon[Index].DataData();
	}

	BYTE* data = Data + (Height - srcTop - srcHeight)*Width;

// vypnut� v�b�ru bloku
	DispSelect();

// p�ednastaven� parametr� z�hlav� bitmapy
	StdBitmapInfo->bmiHeader.biWidth = dstWidth;
	StdBitmapInfo->bmiHeader.biHeight = dstHeight;

// vytvo�en� bufferu k dek�dov�n� zobrazen�
	BYTE* buf;
	buf = (BYTE*)MemGet(dstWidthByte * dstHeight);
	BYTE* src = data + srcLeft;
	BYTE* dst = buf;
	BYTE col = BlackCol;

	dstWidthByte -= dstWidth;
	int i, i2, j;

// dek�dov�n� obr�zku
	if ((Zoom == 1) && (dstWidthByte == 0) && (Width == dstWidth))
	{
		MemCopy(dst, src, srcWidth * srcHeight);
	}
	else
	{
		for (i = srcHeight; i > 0; i--)
		{
			switch (Zoom)
			{
			case 1:
				MemCopy(dst, src, srcWidth);
				dst += dstWidthByte + dstWidth;
				break;

			case 2:
				for (i2 = 2; i2 > 0; i2--)
				{
					for (j = srcWidth; j > 0; j--)
					{
						MemFill(dst, 2, *src);
						src++;
						dst += 2;
					}
					dst += dstWidthByte;
					src -= srcWidth;
				}
				break;

			case 3:
				for (i2 = 3; i2 > 0; i2--)
				{
					for (j = srcWidth; j > 0; j--)
					{
						MemFill(dst, 3, *src);
						src++;
						dst += 3;
					}
					dst += dstWidthByte;
					src -= srcWidth;
				}
				break;

			case 4:
				for (i2 = 4; i2 > 0; i2--)
				{
					for (j = srcWidth; j > 0; j--)
					{
						MemFill(dst, 4, *src);
						src++;
						dst += 4;
					}
					dst += dstWidthByte;
					src -= srcWidth;
				}
				break;

			case 5:
				for (i2 = 5; i2 > 0; i2--)
				{
					for (j = srcWidth; j > 0; j--)
					{
						MemFill(dst, 5, *src);
						src++;
						dst += 5;
					}
					dst += dstWidthByte;
					src -= srcWidth;
				}
				break;

			case 6:
				for (i2 = 6; i2 > 0; i2--)
				{
					for (j = srcWidth; j > 0; j--)
					{
						MemFill(dst, 6, *src);
						src++;
						dst += 6;
					}
					dst += dstWidthByte;
					src -= srcWidth;
				}
				break;

			case 7:
				for (i2 = 7; i2 > 0; i2--)
				{
					for (j = srcWidth; j > 0; j--)
					{
						MemFill(dst, 7, *src);
						src++;
						dst += 7;
					}
					dst += dstWidthByte;
					src -= srcWidth;
				}
				break;

			case 8:
				i2 = 8;
				if (Rastr) i2--;

				for (; i2 > 0; i2--)
				{
					for (j = srcWidth; j > 0; j--)
					{
						MemFill(dst, 8, *src);
						src++;
						dst += 8;
						if (Rastr)
						{
							dst[-1] = col;
						}
					}
					dst += dstWidthByte;
					src -= srcWidth;
				}

				if (Rastr)
				{
					MemFill(dst, dstWidth, col);
					dst += dstWidth + dstWidthByte;
				}
				break;
/*
			case 16:
				i2 = 16;
				if (Rastr) i2--;

				for (; i2 > 0; i2--)
				{
					for (j = srcWidth; j > 0; j--)
					{
						MemFill(dst, 16, *src);
						src++;
						dst += 16;

						if(Rastr)
						{
							dst[-1] = col;
						}
					}
					dst += dstWidthByte;
					src -= srcWidth;
				}

				if (Rastr)
				{
					MemFill(dst, dstWidth, col);
					dst += dstWidth + dstWidthByte;
				}
				break;
*/
			default:
				i2 = Zoom;
				if (Rastr) i2--;

				for (; i2 > 0; i2--)
				{
					for (j = srcWidth; j > 0; j--)
					{
// !!!!!!!!!!!! zde nast�v� hav�rie p�i UNDO po zm�n� velikosti obr�zk� sprajtu (neplatn� adresa src, Zoom=13)
						MemFill(dst, Zoom, *src);
						src++;
						dst += Zoom;
						if (Rastr)
						{
							dst[-1] = col;
						}
					}
					dst += dstWidthByte;
					src -= srcWidth;
				}

				if (Rastr)
				{
					MemFill(dst, dstWidth, col);
					dst += dstWidth + dstWidthByte;
				}
			}
			src += Width;
		}
	}

// dek�dov�n� vertik�ln�ch hrub�ch �ar
	if (Rastr)
	{
		dstWidthByte += dstWidth;
		col = BlueCol;
		int dstwidth = dstWidth;

		for (i = (ICONWIDTH-1) - (srcLeft % ICONWIDTH); i < srcWidth; i += ICONWIDTH)
		{
			if ((i == srcWidth - 1) && (srcLeft + srcWidth == Width)) 
			{
				col = RedCol;
				dstwidth--;
			}

			dst = buf + (i+1)*Zoom - 1;
			for (j = dstHeight; j > 0; j--)
			{
				*dst = col;
				dst += dstWidthByte;
			}
		}

// dek�dov�n� horizont�ln�ch hrub�ch �ar
		col = BlueCol;

		i = ICONHEIGHT - (Height - srcTop - srcHeight) % ICONHEIGHT;
		for (; i <= srcHeight; i += ICONHEIGHT)
		{
			if ((i == srcHeight) && (srcTop == 0)) col = RedCol;

			dst = buf + (i*Zoom-1)*dstWidthByte;
			MemFill(dst, dstwidth, col);
		}
	}

// zobrazen� obr�zku z bufferu
	::StretchDIBits(dc, dstLeft, dstTop, dstWidth, dstHeight,
		0, 0, dstWidth, dstHeight, buf,
		StdBitmapInfo, DIB_RGB_COLORS, SRCCOPY);

// zru�en� bufferu k dek�dov�n� zobrazen�
	MemFree(buf);

// zapnut� v�b�ru bloku
	DispSelect();
}


/////////////////////////////////////////////////////////////////////////////
// vykreslen� pol��ka v�b�ru barvy (s vlastn�m kontextem za��zen�)

void DispCol(int col)
{
// otev�en� kontextu displeje
	HDC dc = ::GetDC(MainFrame);

// nastaven� vlastn�ch palet
	HPALETTE OldPal;					// �schova p�vodn�ch palet
	OldPal = ::SelectPalette(dc,		// v�b�r vlastn�ch palet
		StdPalette, FALSE);
	::RealizePalette(dc);				// realizace palet

// zobrazen� barvy
	DispCol(dc, col);

// n�vrat p�vodn�ch palet
	::SelectPalette(dc,OldPal,FALSE);

// uvoln�n� kontextu displeje
	::ReleaseDC(MainFrame, dc);
}


/////////////////////////////////////////////////////////////////////////////
// vykreslen� pol��ka v�b�ru barvy

void DispCol(HDC dc, int col)
{
// zablokov�n� nepovolen� barvy (nap�. -1)
	if ((DWORD)col >= (DWORD)StdColors) return;

// p�ednastaven� sou�adnic pro podkladovou barvu nebo barvu st�nu
	RECT rc;
	rc.left = ColsLeft;
	rc.top = ColsTop;
	if (col == ShadCol) rc.top += ColsHeight/2;
	rc.bottom = rc.top + ColsHeight/2;

// v�po�et sou�adnic pro jin� barvy ne� podkladov�
	if ((col != BackCol) && (col != ShadCol))
	{
		rc.top += ((col-ResCols) % ColLev) * ColsIncY;
		rc.left += ((col-ResCols) / ColLev + 1) * ColsIncX;
		rc.bottom = rc.top + ColsIncY;
	}
	rc.right = rc.left + ColsIncX;

// �schova sou�adnic r�mu
	RECT rc2;
	rc2 = rc;

// kontrola, zda se zobraz� r�m
	if ((ColsIncX >= MINCOLSIR) && (ColsIncY >= MINCOLSIR))
	{

// zobrazen� r�mu kolem pol��ka
		::DrawEdge(dc, &rc, EDGE_SUNKEN, BF_RECT);

// korekce ��mu pol��ka pro barvu
		rc.left += 2;
		rc.top += 2;
		rc.right -= 2;
		rc.bottom -= 2;
	}

// zobrazen� barvy pol��ka
	HBRUSH brush = ::CreateSolidBrush(PALETTEINDEX((WORD)col));
	::FillRect(dc, &rc, brush);
	::DeleteObject(brush);

// zobrazen� r�me�ku indik�toru aktivn� barvy
	if ((col == ColInd) && (rc.left < rc.right) && (rc.top < rc.bottom))
	{
		::DrawEdge(dc, &rc2, EDGE_RAISED, BF_RECT);
	}

// zobrazen� r�me�ku indik�toru lev�ho tla��tka
	if (col == ColLeft)
	{
		rc2.left += ColsIncX/6;
		rc2.top += ColsIncY/6;
		rc2.right -= ColsIncX/6;
		rc2.bottom -= ColsIncY/6;
		::DrawEdge(dc, &rc2, EDGE_BUMP, BF_RECT);
	}

// zobrazen� r�me�ku indik�toru prav�ho tla��tka
	if (col == ColRight)
	{
		rc2.left = rc.left + ColsIncX/4;
		rc2.top = rc.top + ColsIncY/4;
		rc2.right = rc.right - ColsIncX/4;
		rc2.bottom = rc.bottom - ColsIncY/4;
		::DrawEdge(dc, &rc2, EDGE_BUMP, BF_RECT);
	}
}


/////////////////////////////////////////////////////////////////////////////
// zv�t�en� m���tka

void ZoomIn()
{
	if (Zoom < MAXZOOM)
	{
		Zoom++;

		switch (BufID)
		{
		case BufIcoID:
			ZoomIcon = Zoom;
			break;

		case BufPicID:
			ZoomPic = Zoom;
			break;
		}

		ReCalc();
		DispLeft = MidX - DispWidth/2;
		DispBottom = MidY - DispHeight/2;
		ReCalc();
		SetScroll();
		ProgOnSize();
		Disp();
		UpdateZoomInOut();
	}
}

/////////////////////////////////////////////////////////////////////////////
// zmen�en� m���tka

void ZoomOut()
{
	if (Zoom > 1)
	{
		Zoom--;

		switch (BufID)
		{
		case BufIcoID:
			ZoomIcon = Zoom;
			break;

		case BufPicID:
			ZoomPic = Zoom;
			break;
		}

		ReCalc();
		DispLeft = MidX - DispWidth/2;
		DispBottom = MidY - DispHeight/2;
		ReCalc();
		SetScroll();
		ProgOnSize();
		Disp();
		UpdateZoomInOut();
	}
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace tla��tek pro zv�t�en�/zmen�en� m���tka

void UpdateZoomInOut()
{
	if (FocusBuf == BufEdiID)
	{
		EnableCommand(IDN_IN, Zoom < MAXZOOM);
		EnableCommand(IDN_OUT, Zoom > 1);
	}
}


/////////////////////////////////////////////////////////////////////////////
// p�ekreslen� ikon ve stromech

void UpdateTree()
{
// kontrola, zda je buffer ikon
	if ((BufID != BufIcoID) || (Index < 0)) return;

// aktualizace zobrazen� ikon
	for (int buf = 0; buf < PROGBUFNUM; buf++)
	{
		BufProg[buf].ReDispIcon(Index);
	}
}


/////////////////////////////////////////////////////////////////////////////
// zapnut�/vypnut� rastru

void OnRastr()
{
	Rastr = !Rastr;
	UpdateMenu();
	Disp();
}


// ********************* obsluha posuvn�k� **********************************


/////////////////////////////////////////////////////////////////////////////
// p�esun posuvn�k� p�i zm�n� rozm�r� okna

HDWP MoveScroll(HDWP hdwp)
{
// lok�ln� prom�nn�
	int x, y, w, h;

// p�epo�et zobrazen�
	ReCalc();

// p�esun a zobrazen� horizont�ln�ho posuvn�ku
	x = EditX + 2;
	y = ColsTop - TRACKSIRKA;
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
	h = ColsTop - EditY - 2;
	if (HScrollDisp) h -= TRACKSIRKA;
	hdwp = ::DeferWindowPos(hdwp, VScroll, NULL, x, y, w, h,
			SWP_DRAWFRAME | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER |
			((VScrollDisp && !PreviewPic) ? SWP_SHOWWINDOW : SWP_HIDEWINDOW));

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
	if (si.nMax != Width)
	{
		si.nMax = Width;
		si.fMask |= SIF_RANGE;
	}

// velikost str�nky
	if (si.nPage != (UINT)DispWidth)
	{
		si.nPage = (UINT)DispWidth;
		si.fMask |= SIF_PAGE;
	}

// pozice posuvn�ku
	if (si.nPos != DispLeft)
	{
		si.nPos = DispLeft;
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
	if (si.nMax != Height)
	{
		si.nMax = Height;
		si.fMask |= SIF_RANGE;
	}

// velikost str�nky
	if (si.nPage != (UINT)DispHeight)
	{
		si.nPage = (UINT)DispHeight;
		si.fMask |= SIF_PAGE;
	}

// pozice posuvn�ku
	if (si.nPos != Height - DispHeight - DispBottom)
	{
		si.nPos = Height - DispHeight - DispBottom;
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
// horizont�ln� posuvn�k

void OnHScroll(int code, int pos)
{
	switch (code)
	{
	case SB_LEFT:
		pos = 0;
		break;

	case SB_LINELEFT:
		pos -= DispWidth/20+1;
		break;

	case SB_LINERIGHT:
		pos += DispWidth/20+1;
		break;

	case SB_PAGELEFT:
		pos -= DispWidth-1;
		break;

	case SB_PAGERIGHT:
		pos += DispWidth-1;
		break;

	case SB_RIGHT:
		pos = Width - DispWidth;
		break;
	}

	if (pos < 0) pos = 0;
	if (pos >= Width - DispWidth)
		pos = Width - DispWidth;

	if (pos != DispLeft)
	{
		DispLeft = pos;
		MidX = DispLeft + DispWidth/2;
		MidY = DispBottom + DispHeight/2;
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
		pos -= DispHeight/20+1;
		break;

	case SB_LINEDOWN:
		pos += DispHeight/20+1;
		break;

	case SB_PAGEUP:
		pos -= DispHeight-1;
		break;

	case SB_PAGEDOWN:
		pos += DispHeight-1;
		break;

	case SB_BOTTOM:
		pos = Height - DispHeight;
		break;
	}

	if (pos < 0) pos = 0;
	if (pos >= Height - DispHeight)
		pos = Height - DispHeight;
	pos = Height - DispHeight - pos;

	if (pos != DispBottom)
	{
		DispBottom = pos;
		MidX = DispLeft + DispWidth/2;
		MidY = DispBottom + DispHeight/2;
		Disp();
	}
}


// **************************** obsluha editace *****************************


/////////////////////////////////////////////////////////////////////////////
// �schova obr�zku do pomocn�ho bufferu

void Push()
{
	if (BufID == BufPicID)
	{
		if (Index == -2)
		{
			Sprite[EditItemSprite][EditItemPic].DeComp();
			Data = Sprite[EditItemSprite][EditItemPic].DataData();
		}
		else
		{
			Picture[Index].DeComp();
			Data = Picture[Index].DataData();
		}
	}
	else
	{
		Icon[Index].DeComp();
		Data = Icon[Index].DataData();
	}

	PushBuf = (BYTE*)MemSize(PushBuf, Width * Height * sizeof(BYTE));
	MemCopy(PushBuf, Data, Width * Height);
}

/////////////////////////////////////////////////////////////////////////////
// n�vrat obr�zku z pomocn�ho bufferu

void Pop()
{
	if (BufID == BufPicID)
	{
		if (Index == -2)
		{
			Sprite[EditItemSprite][EditItemPic].DeComp();
			Data = Sprite[EditItemSprite][EditItemPic].DataData();
		}
		else
		{
			Picture[Index].DeComp();
			Data = Picture[Index].DataData();
		}
	}
	else
	{
		Icon[Index].DeComp();
		Data = Icon[Index].DataData();
	}


	MemCopy(Data, PushBuf, Width * Height);
	AktLeft = 0;
	AktTop = 0;
	AktRight = Width;
	AktBottom = Height;
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� bodu z bufferu (nekontroluje sou�adnice !)

inline BYTE _fastcall GetPush(int x, int y)
{
	return PushBuf[Width * (Height-1-y) + x];
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� bodu v bufferu (nekontroluje sou�adnice !)

inline void _fastcall SetPush(int x, int y, BYTE col)
{
	PushBuf[Width * (Height-1-y) + x] = col;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� indik�toru barvy

void SetColInd(BYTE col)
{
	BYTE oldcol = ColInd;

	if (col == oldcol) return;

	ColInd = col;

	DispCol(oldcol);
	DispCol(col);
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� bodu obr�zku (nekontroluje sou�adnice !)

inline BYTE _fastcall GetPoint(int x, int y)
{
	return Data[Width*(Height-1-y) + x];
}


/////////////////////////////////////////////////////////////////////////////
// obsluha zobrazen� kurzoru my�i (TRUE=obslou�eno)

BOOL OnSetCursor(int x, int y)
{
// pole volby barvy
	if ((x >= ColsLeft) &&
		(x < ColsLeft + ColsWidth) &&
		(y >= ColsTop) &&
		(y < ColsTop + ColsHeight) &&
		!Editing)
	{
		CurAkt = CurKapatko;
		::SetCursor(CurAkt);
		return TRUE;
	}

// pole obr�zku
	if (((x >= DLeft) &&
		(x < DLeft + DWidth) &&
		(y >= DTop) &&
		(y < DTop + DHeight)) ||
		Editing)
	{
		switch(Mode)
		{
		case IDN_LINE:		CurAkt = CurLine;
							break;

		case IDN_FILLBOX:	CurAkt = CurFillRect;
							break;

		case IDN_BOX:		CurAkt = CurRect;
							break;

		case IDN_FILLCIRCLE: CurAkt = CurFillRound;
							break;

		case IDN_CIRCLE:	CurAkt = CurRound;
							break;

		case IDN_FILLELLIPSE: CurAkt = CurFillElip;
							break;

		case IDN_ELLIPSE:	CurAkt = CurElip;
							break;

		case IDN_SPHERE:	CurAkt = CurKoule;
							break;

		case IDN_PIPE:		CurAkt = CurKapatko;
							break;

		case IDN_FILL:		CurAkt = CurFill;
							break;

		case IDN_SPRAY:		CurAkt = CurSpray;
							break;

		case IDN_TEXT:
		case IDN_SELECT:	
							if (Selecting &&
								TestBlok((x - DLeft)/Zoom + DispLeft,
										(y - DTop)/Zoom + (Height - DispHeight - DispBottom)) &&
								(!Editing || Moving))
							{
								if (Moving)
								{
									CurAkt = CurRuka;
								}
								else
								{
									CurAkt = CurSelectMove;
								}
							}
							else
							{
								CurAkt = CurSelect;
							}
							break;

		default:			if (PenWidth > 7)
							{
								CurAkt = CurPaint;
							}
							else
							{
								CurAkt = CurPen;
							}
		}
		::SetCursor(CurAkt);
		return TRUE;
	}

// implicitn� obsluha
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// stisk tla��tka my�i (TRUE = obslou�eno)

BOOL OnButtonDown(UINT flags, int x, int y, BOOL right, BOOL dclick)
{

	if (BufID == BufPicID)
	{
		if (Index == -2)
		{
			Sprite[EditItemSprite][EditItemPic].DeComp();
			Data = Sprite[EditItemSprite][EditItemPic].DataData();
		}
		else
		{
			Picture[Index].DeComp();
			Data = Picture[Index].DataData();
		}
	}
	else
	{
		Icon[Index].DeComp();
		Data = Icon[Index].DataData();
	}


// p��prava adresy prom�nn� barvy
	BYTE col;							// zvolen� aktivn� barva
	BYTE* colbut;						// prom�nn� s aktivn� barvou

	if (right)
	{
		colbut = &ColRight;				// barva pro prav� tla��tko
	}
	else
	{
		colbut = &ColLeft;				// barva pro lev� tla��tko
	}

	BYTE oldcol = *colbut;				// p�vodn� barva

// volba barvy
	if ((x >= ColsLeft) &&
		(x < ColsLeft + ColsWidth) &&
		(y >= ColsTop) &&
		(y < ColsTop + ColsHeight))
	{
		if (x < (ColsLeft + ColsIncX))
		{
			col = (BYTE)((y - ColsTop)/(ColsHeight/2));
		}
		else
		{
			col = (BYTE)((x - ColsLeft - ColsIncX)/ColsIncX * ColLev + (y - ColsTop)/ColsIncY + ResCols);
		}
		ASSERT(col < (BYTE)StdColors);
		if (col != oldcol)
		{
			*colbut = col;
			DispCol(oldcol);
			DispCol(col);

// zm�na pozad� ozna�en�ho bloku p�i zm�n� barvy prav�ho tla��tka
			if (right && (Mode == IDN_SELECT) && Selecting && CutMode)
			{
				MoveSelect();
				ReDisp();
			}

// p�ekreslen� textu p�i zm�n� barvy lev�ho tla��tka
			if ((Mode == IDN_TEXT) && !right)
			{
				ReDispText();				// zobrazen� textu
			}
		}

// v m�du kap�tka n�vrat p�ede�l�ho m�du
		if (Mode == IDN_PIPE)
		{
			SetMode(OldMode);
		}
		return TRUE;
	}

// p��prava relativn�ch sou�adnic bodu v edita�n�m poli
	int sx = (x - DLeft)/Zoom + DispLeft;
	int sy = (y - DTop)/Zoom + (Height - DispHeight - DispBottom);

// test, zda je edita�n� pole
	if ((x >= DLeft) &&
		(y >= DTop) &&
		(sx < DispLeft + DispWidth) &&
		(sy < Height - DispBottom) &&
		(Data))
	{

// v�tven� podle edita�n�ho m�du
		switch (Mode)
		{

// m�d pera, ��ry, obd�ln�ku, kru�nice a elipsy
		case IDN_PEN:
		case IDN_LINE:
		case IDN_BOX:
		case IDN_CIRCLE:
		case IDN_ELLIPSE:
			PushUndo();				// �schova obr�zku do UNDO bufferu
			Editing = TRUE;					// p��znak zah�jen� editace
			Push();					// �schova do pomocn�ho bufferu
			OldX = sx;						// �schova sou�adnce X
			OldY = sy;						// �schova sou�adnice Y
			TopX = sx;						// �schova sou�adnice X
			TopY = sy;						// �schova sou�adnice Y
			SetPaint(sx, sy, *colbut);		// vykreslen� v�choz�ho bodu
			SetModi();						// nastaven� p��znaku modifikace
			break;

// m�d vypln�n�ho obd�ln�ku, kruhu a ov�lu - neuplatn� se ���ka ��ry
		case IDN_FILLBOX:
		case IDN_FILLCIRCLE:
		case IDN_FILLELLIPSE:
		case IDN_SPHERE:
			PushUndo();				// �schova obr�zku do UNDO bufferu
			Editing = TRUE;					// p��znak zah�jen� editace
			Push();					// �schova do pomocn�ho bufferu
			OldX = sx;						// �schova sou�adnce X
			OldY = sy;						// �schova sou�adnice Y
			TopX = sx;						// �schova sou�adnice X
			TopY = sy;						// �schova sou�adnice Y
			SetPoint(sx, sy, *colbut);		// vykreslen� v�choz�ho bodu
			if (sx < AktLeft) AktLeft = sx;
			if (sx >= AktRight) AktRight = sx+1;
			if (sy < AktTop) AktTop = sy;
			if (sy >= AktBottom) AktBottom = sy+1;
			SetModi();						// nastaven� p��znaku modifikace
			break;

// m�d spreje
		case IDN_SPRAY:
			PushUndo();				// �schova obr�zku do UNDO bufferu
			Editing = TRUE;					// p��znak zah�jen� editace
			OldX = sx;						// �schova sou�adnice X
			OldY = sy;						// �schova sou�adnice Y
			SetSpray(sx, sy, *colbut);		// vykreslen� v�choz�ho bodu
			SetModi();						// nastaven� p��znaku modifikace
			break;

// m�d kap�tka
		case IDN_PIPE:
			col = GetPoint(sx, sy);
			oldcol = *colbut;
			if (col != oldcol)
			{
				*colbut = col;
				DispCol(oldcol);
				DispCol(col);
			}
			SetMode(OldMode);
			break;

// m�d v�pln�
		case IDN_FILL:
			col = GetPoint(sx, sy);
			if (col != *colbut)
			{
				PushUndo();
				SetFill(sx, sy, *colbut, col);
				SetModi();
				Disp();
				UpdateTree();
			}
			break;

// m�d v�b�ru a textu
		case IDN_TEXT:
		case IDN_SELECT:

// prav� tla��tko vypne ozna�en� bloku (jinak pokra�uje stejn� jako lev�)
			if (right && Selecting)
			{
				DispSelect();				// vypnut� zobrazen� v�b�ru bloku
				Moving = FALSE;				// konec posunu bloku
				Selecting = FALSE;			// zru�en� p��znaku v�b�ru bloku
				Editing = FALSE;			// zru�en� p��znaku editace
				if (Mode == IDN_TEXT)
				{
					SetMode(OldMode);
					return TRUE;
				}
				break;
			}

// zah�jen� p�esunu bloku, je-li kurzor uvnit� bloku
			if (Selecting)					// je ji� blok ozna�en?
			{
				if (TestBlok(sx, sy))		// je kurzor v bloku?
				{
					Editing = TRUE;			// p��znak zah�jen� editace
					Moving = TRUE;			// p��znak p�esunu bloku
					OldX = sx;				// �schova sou�adnice X
					OldY = sy;				// �schova sou�adnice Y
					::SetCursor(CurRuka);	// zapnut� kurzoru pro p�esun
					SetModi();				// p��znak modifikace programu
				}

// vn� bloku se blok vypne (a zah�j� se nov� ozna�ov�n�)
				else
				{
					DispSelect();			// vypnut� ozna�en� bloku
					Selecting = FALSE;		// zru�en� p��znaku v�b�ru bloku

					if (Mode == IDN_TEXT)
					{
						SetMode(OldMode);
						return TRUE;
					}
				}
			}

// zah�jen� ozna�ov�n� bloku
			if (!Selecting)			// nen� blok ozna�en?
			{
				PushUndo();			// �schova do UNDO bufferu
				Editing = TRUE;				// p��znak zah�jen� editace
				Push();				// �schova do pomocn�ho bufferu
				Selecting = TRUE;			// p��znak ozna�en� bloku
				Moving = FALSE;				// nen� p�esun bloku
				OldX = sx;					// �schova sou�adnice X
				OldY = sy;					// �schova sou�adnice Y
				TopX = sx;					// po��te�n� sou�adnice X ozna�en�ho bloku
				TopY = sy;					// po��te�n� sou�adnice Y ozna�en�ho bloku
				BlokX = sx;					// po��tek bloku X
				BlokY = sy;					// po��tek bloku Y
				BlokWidth = 1;				// ���ka bloku
				BlokHeight = 1;				// v��ka bloku
				BlokBuf = (BYTE*)MemSize(BlokBuf, 1*sizeof(BYTE));			// buffer pro uchov�n� 1 bodu
				SetBlok(0, 0, GetPoint(sx, sy)); // �schova prvn�ho bodu
				CutMode = !right;			// p��znak, zda jsou �daje k vymaz�n�
				FirstBlokX = sx;			// v�choz� po��tek bloku X
				FirstBlokY = sy;			// v�choz� po��tek bloku Y
				FirstBlokWidth = 1;			// v�choz� ���ka bloku
				FirstBlokHeight = 1;		// v�choz� v��ka bloku
				BlokBack = ColRight;		// pr�hledn� barva bloku
				DispSelect();				// zapnut� ozna�en� bloku
			}
			break;
		}

// p�ekreslen� displeje
		ReDisp();

// aktualizace voleb bloku
		UpdateClipboard();

		return TRUE;
	}

	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// posun my�i

void OnMouseMove(UINT flags, int x, int y)
{
	if (BufID == BufPicID)
	{
		if (Index == -2)
		{
			Sprite[EditItemSprite][EditItemPic].DeComp();
			Data = Sprite[EditItemSprite][EditItemPic].DataData();
		}
		else
		{
			Picture[Index].DeComp();
			Data = Picture[Index].DataData();
		}
	}
	else
	{
		Icon[Index].DeComp();
		Data = Icon[Index].DataData();
	}


// p��prava relativn�ch sou�adnic bodu v edita�n�m poli
	int sx = (x - DLeft)/Zoom + DispLeft;
	int sy = (y - DTop)/Zoom + (Height - DispHeight - DispBottom);

// p�i editaci se sou�adnice omez� na edita�n� pole
	if (Editing)
	{
		if (x < DLeft)
		{
			x = DLeft;
			sx = DispLeft;
		}

		if (y < DTop)
		{
			y = DTop;
			sy = Height - DispHeight - DispBottom;
		}

		if ((DWORD)sx >= (DWORD)Width)
		{
			sx = Width - 1;
		}

		if ((DWORD)sy >= (DWORD)Height)
		{
			sy = Height - 1;
		}
	}

// sou�adnice my�i pro zobrazen�
	MouseX = sx;
	MouseY = Height - 1 - sy;

// test, zda je edita�n� pole
	if ((x >= DLeft) &&
		(y >= DTop) &&
		(sx < DispLeft + DispWidth) &&
		(sy < Height - DispBottom) &&
		(Data))
	{

// test, zda je m�d editace a je stisknuto n�kter� tla��tko
		if ((flags & (MK_LBUTTON | MK_RBUTTON)) && Editing)
		{

// p��prava barvy ke kreslen�
			BYTE col;
			if (flags & MK_LBUTTON)
			{
				col = ColLeft;
			}
			else
			{
				col = ColRight;
			}

// v�tven� podle edita�n�ho m�du
			if ((OldX != sx) || (OldY != sy))
			{
				switch (Mode)
				{

// kreslen� perem
				case IDN_PEN:
					SetLinePaint(OldX, OldY, sx, sy, col);
					break;

// kreslen� ��ry
				case IDN_LINE:
					Pop();
					SetLinePaint(TopX, TopY, sx, sy, col);
					break;

// kreslen� sprejem
				case IDN_SPRAY:
					SetSpray(sx, sy, col);
					break;

// kreslen� obd�ln�ku
				case IDN_BOX:
					Pop();
					SetRect(TopX, TopY, sx, sy, col);
					break;

// kreslen� obd�ln�ku s v�pln�
				case IDN_FILLBOX:
					Pop();
					SetRectFill(TopX, TopY, sx, sy, col);
					break;

// kreslen� kru�nice
				case IDN_CIRCLE:
					Pop();
					SetRound(TopX, TopY, sx, sy, col);
					break;

// kreslen� kruhu
				case IDN_FILLCIRCLE:
					Pop();
					SetRoundFill(TopX, TopY, sx, sy, col);
					break;

// kreslen� elipsy
				case IDN_ELLIPSE:
					Pop();
					SetElip(TopX, TopY, sx, sy, col);
					break;

// kreslen� ov�lu
				case IDN_FILLELLIPSE:
					Pop();
					SetElipFill(TopX, TopY, sx, sy, col);
					break;

// kreslen� koule
				case IDN_SPHERE:
					Pop();
					SetKoule(TopX, TopY, sx, sy, col);
					break;

// v�b�r bloku
				case IDN_TEXT:
				case IDN_SELECT:
					if (Selecting)		// je blok ozna�en?
					{
						DispSelect();

						if (Moving)
						{
							BlokX += sx - OldX;
							BlokY += sy - OldY;
							MoveSelect();
							DispSelect();
							SetModi();
						}
						else
						{
							if (sx >= TopX)
							{
								BlokX = TopX;
								BlokWidth = sx - TopX + 1;
							}
							else
							{
								BlokX = sx;
								BlokWidth = TopX - sx + 1;
							}

							if (sy >= TopY)
							{
								BlokY = TopY;
								BlokHeight = sy - TopY + 1;
							}
							else
							{
								BlokY = sy;
								BlokHeight = TopY - sy + 1;
							}

							DispSelect();
			
							BlokBuf = (BYTE*)MemSize(BlokBuf, BlokWidth*BlokHeight * sizeof(BYTE));

							for (int bloki = 0; bloki < BlokWidth; bloki++)
							{
								for (int blokj = 0; blokj < BlokHeight; blokj++)
								{
									SetBlok(bloki, blokj, GetPoint(BlokX + bloki, BlokY + blokj));
								}
							}

							BlokBack = ColRight;
							FirstBlokX = BlokX;
							FirstBlokY = BlokY;
							FirstBlokWidth = BlokWidth;
							FirstBlokHeight = BlokHeight;
						}
					}
				}
			}

// �schova sou�adnic
			OldX = sx;
			OldY = sy;

// vypnut� indikace aktu�ln� barvy
			SetColInd(255);
		}

// nen� m�d editace - zobrazen� barvy pod kurzorem
		else
		{
			if (Editing)
			{
				UpdateTree();
			}
			
			Editing = FALSE;
			Moving = FALSE;
			SetColInd(GetPoint(sx, sy));
		}
	}

// kurzor my�i mimo edita�n� pole - vypnut� indikace barvy
	else
	{
		SetColInd(255);
		MouseX = MOUSEINV;
		MouseY = MOUSEINV;
	}

// p�ekreslen� displeje
	ReDisp();

// zobrazen� sou�adnic my�i
	if (((DWORD)MouseX >= (DWORD)Width) || ((DWORD)MouseY >= (DWORD)Height))
	{
		MouseX = MOUSEINV;
		MouseY = MOUSEINV;
	}
	DispMouseXY();
}


/////////////////////////////////////////////////////////////////////////////
// uvoln�n� tla��tka my�i

void OnButtonUp(UINT keys, BOOL right)
{
	if (Editing)
	{
		UpdateTree();
	}

// v m�du p�esunu bloku navr�cen� kurzoru
	if (Moving)
	{
		::SetCursor(CurSelectMove);
	}
	Editing = FALSE;
	Moving = FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// vstup znaku z kl�vesnice (TRUE=obslou�eno)

BOOL OnChar(TCHAR znak)
{
	switch (znak)
	{
	case 8:
	case 127:
		if (Mode == IDN_TEXT)
		{
			if (FontText.IsNotEmpty())
			{
				FontText.Delete(FontText.Length() - 1);
				ReDispText();
			}
			return TRUE;
		}
		break;

	default:
		if (Mode == IDN_TEXT)
		{
			if ((DWORD)znak >= (DWORD)32)
			{
				FontText.Add(znak);
				ReDispText();
			}
			return TRUE;
		}
	}

	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// vstup kl�vesy z kl�vesnice (TRUE=obslou�eno)

BOOL OnKeyDown(int key)
{
	switch (key)
	{
// DELETE - zru�en� znaku
	case VK_DELETE:
		if (Mode == IDN_TEXT)
		{
			if (FontText.IsNotEmpty())
			{
				FontText.Delete(FontText.Length()-1);
				ReDispText();
			}
			return TRUE;
		}
		break;

// ENTER - ukon�en� zad�n� textu
	case VK_RETURN:
		if (Mode == IDN_TEXT)
		{
			SetMode(OldMode);
			return TRUE;
		}

		if (Mode == IDN_SELECT)
		{
			DispSelect();
			Selecting = FALSE;
			return TRUE;
		}
		break;

// ESC - zru�en� zad�n� textu
	case VK_ESCAPE:
		if (Mode == IDN_TEXT)
		{
			Delete();
			SetMode(OldMode);
			return TRUE;
		}

		if ((Mode == IDN_SELECT) && Selecting)
		{
			if (CutMode)
			{
				BlokX = FirstBlokX;
				BlokY = FirstBlokY;
				MoveSelect();
				DispSelect();
				Selecting = FALSE;
				Disp();
			}
			else
			{
				Delete();
			}
			return TRUE;
		}
		break;
	}
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� edita�n�ho m�du

void SetMode(int mode)
{
// kontrola, zda je re�im editace
	if (!ProgMode ||
		((EditMode != BufIcoID) && (EditMode != BufPicID)))
		return;

// volba fontu p�i zapnut� m�du textu
	if(mode == IDN_TEXT)
	{
		FontText.Empty();

// p��prava parametr� fontu
		LOGFONT	lf;										// parametry fontu
		lf.lfHeight = FontSize;							// velikost fontu
		lf.lfWidth = 0;									// ���ka fontu
		lf.lfEscapement = 0;							// �nikov� vektor
		lf.lfOrientation = 0;							// orientace
		lf.lfWeight = FontWeight;						// v�ha fontu (BOLD)
		lf.lfItalic = (BYTE)FontItalic;					// kurz�va
		lf.lfCharSet = (BYTE)FontCharSet;				// znakov� sada
		lf.lfOutPrecision = OUT_DEFAULT_PRECIS;			// p�esnost v�stupu
		lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;			// p�esnost oblasti
		lf.lfQuality = DEFAULT_QUALITY;					// kvalita fontu
		lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE; // rozte� font�

		int txtlen = FontName.Length();						// d�lka fontu
		if (txtlen > (LF_FACESIZE-2)) txtlen = (LF_FACESIZE-2);	// omezen� d�lky jm�na fontu
		MemCopy(lf.lfFaceName, (LPCTSTR)FontName, txtlen); // p�enesen� jm�na fontu
		lf.lfFaceName[txtlen] = 0;						// ozna�en� konce textu

// p��prava parametr� dialogu
		CHOOSEFONT cf;
		cf.lStructSize = sizeof(LOGFONT);
		cf.hwndOwner = MainFrame;
		cf.lpLogFont = &lf;
		cf.iPointSize = FontSize;
		cf.Flags = CF_NOVERTFONTS | CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;
		cf.hInstance = hInstance;
		cf.nFontType = 0;
		if (FontWeight > ((FW_NORMAL + FW_BOLD)/2)) { cf.nFontType |= BOLD_FONTTYPE; }
		if (FontItalic) { cf.nFontType |= ITALIC_FONTTYPE; }

// volba fontu (p�i p�eru�en� se nic nestane)
		if (!::ChooseFont(&cf))
		{
			return;
		}

// na�ten� zvolen�ch parametr� fontu
		FontWeight = lf.lfWeight;					// v�ha fontu
		FontItalic = lf.lfItalic;					// p��znak ITALIC
		FontName = lf.lfFaceName;					// jm�no fontu
		FontSize = lf.lfHeight;
		FontCharSet = lf.lfCharSet;

// aktualizace fontu, je-li ji� m�d textu
		if (Mode == IDN_TEXT)
		{
			ReDispText();				// zobrazen� textu
			return;
		}
	}

// nastaven� ���ky pera
	switch (mode)
	{
	case IDN_WIDTH1: 
		PenWidth = 1;
		PenWidthID = mode;
		UpdateMenu();
		return;

	case IDN_WIDTH2: 
		PenWidth = 2;
		PenWidthID = mode;
		UpdateMenu();
		return;

	case IDN_WIDTH3: 
		PenWidth = 5;
		PenWidthID = mode;
		UpdateMenu();
		return;

	case IDN_WIDTH4: 
		PenWidth = 9;
		PenWidthID = mode;
		UpdateMenu();
		return;

	case IDN_WIDTH5: 
		PenWidth = 21;
		PenWidthID = mode;
		UpdateMenu();
		return;
	}

// edita�n� p��kazy
	if (mode == IDN_EDITBLOK)
	{
		EditBlok(-1);
		return;
	}

// vypnut� ozna�en� bloku
	DispSelect();
	Selecting = FALSE;

// kontrola, zda je zm�na
	if (mode == Mode) return;

// �schova star�ho m�du
	if (((mode == IDN_PIPE) ||
		(mode == IDN_TEXT) ||
		(mode == IDN_SELECT)) &&
		(Mode != IDN_PIPE) &&
		(Mode != IDN_TEXT) &&
		(Mode != IDN_SELECT))
	{
		OldMode = Mode;
	}

// �schova nov�ho m�du
	Mode = mode;

// v m�du textu zapnut� bloku
	if (mode == IDN_TEXT)
	{
		PushUndo();					// �schova obr�zku do undo bufferu
		Push();						// �schova do pomocn�ho bufferu
		Selecting = TRUE;			// p��znak ozna�en� bloku
		BlokWidth = 10;				// ���ka bloku
		BlokHeight = abs(FontSize)+5; // v��ka bloku
		CutMode = FALSE;			// nejsou �daje k vymaz�n�
		BlokBack = ColRight;		// pr�hledn� barva bloku
		DispSelect();				// zapnut� ozna�en� bloku

		BlokBuf = (BYTE*)MemSize(BlokBuf, BlokWidth * BlokHeight * sizeof(BYTE));	// buffer pro blok

		for (int x = 0; x < BlokWidth; x++)
		{
			for (int y = 0; y < BlokHeight; y++)
			{
				SetBlok(x, y, BlokBack);
			}
		}
		ReDispText();				// zobrazen� textu
	}

// aktualizace volby m�du
	UpdateMenu();

// aktualizace voleb bloku
	UpdateClipboard();
}

/////////////////////////////////////////////////////////////////////////////
// editace bloku

// callback funkce dialogu
BOOL CALLBACK BlokDialogProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
	case WM_INITDIALOG:
		{
			CenterWindow(hWnd, MainFrame);
			CText txt;

			txt.Load(IDN_PICBLOK);
			txt.SetWindowTextFont(hWnd);

			DialogCustomBox(hWnd, IDN_PREVRACENI);
			DialogCustomText(hWnd, IDN_YFLIP_TXT);
			DialogCustomText(hWnd, IDN_XFLIP_TXT);
			DialogCustomBox(hWnd, IDN_OTOCENI);
			DialogCustomText(hWnd, IDN_LROT_TXT);
			DialogCustomText(hWnd, IDN_RROT_TXT);
			DialogCustomText(hWnd, IDN_LROT67_TXT);
			DialogCustomText(hWnd, IDN_RROT67_TXT);
			DialogCustomText(hWnd, IDN_LROT60_TXT);
			DialogCustomText(hWnd, IDN_RROT60_TXT);
			DialogCustomText(hWnd, IDN_LROT45_TXT);
			DialogCustomText(hWnd, IDN_RROT45_TXT);
			DialogCustomText(hWnd, IDN_LROT30_TXT);
			DialogCustomText(hWnd, IDN_RROT30_TXT);
			DialogCustomText(hWnd, IDN_LROT22_TXT);
			DialogCustomText(hWnd, IDN_RROT22_TXT);
			DialogCustomBox(hWnd, IDN_ZVETSENI);
			DialogCustomText(hWnd, IDN_MUL2_TXT);
			DialogCustomText(hWnd, IDN_DIV2_TXT);
			DialogCustomText(hWnd, IDN_MULDIV_TXT);
			DialogCustomBox(hWnd, IDN_BARVY);
			DialogCustomText(hWnd, IDN_XCOL_TXT);
			DialogCustomText(hWnd, IDN_SCOL_TXT);

			txt.Load(IDN_CANCEL0);
			txt.SetDialogTextFont(hWnd, IDCANCEL);

			for (int* tab = EditBlokTab; *tab >= 0; tab += 2)
			{
				::SetWindowPos(::GetDlgItem(hWnd, tab[1]), NULL, 0, 0, 33,29, 
					SWP_NOCOPYBITS | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
			}
		}
		return TRUE;

	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
		case 0:
		case 1:
			switch (LOWORD(wParam))
			{
			case IDN_YFLIP:
			case IDN_XFLIP:
			case IDN_LROT:
			case IDN_LROT67:
			case IDN_LROT60:
			case IDN_LROT45:
			case IDN_LROT30:
			case IDN_LROT22:
			case IDN_RROT:
			case IDN_RROT67:
			case IDN_RROT60:
			case IDN_RROT45:
			case IDN_RROT30:
			case IDN_RROT22:
			case IDN_MUL2:
			case IDN_DIV2:
			case IDN_MULDIV:
			case IDN_XCOL:
			case IDN_SCOL:
			case IDOK:
			case IDCANCEL:
				::EndDialog(hWnd, LOWORD(wParam));
				return TRUE;
			}
			break;
		}
		break;

	case WM_DRAWITEM:
		{
			DRAWITEMSTRUCT* di = (DRAWITEMSTRUCT*)lParam;
			int id = wParam;

			if ((id == IDCANCEL) || (di->CtlType == ODT_STATIC) ||
				(::GetWindowLong(di->hwndItem, GWL_STYLE) & BS_BOTTOM))
			{
				if (DialogDraw(hWnd, lParam)) return TRUE;
			}


			DWORD state = di->itemState;
			HDC dc = di->hDC;
			RECT* rc = &di->rcItem;
			int* tab = EditBlokTab;

			int i;
			for (i = 0; tab[2*i] >= 0; i++)
			{
				if (id == tab[2*i+1])
				{
					break;
				}
			}
			i = tab[2*i];

			HDC dc2 = ::CreateCompatibleDC(dc);
			::SelectObject(dc2, ToolBarBmp);

			int x = (rc->left + rc->right)/2-8;
			int y = (rc->top + rc->bottom)/2-8;

			if (state & ODS_SELECTED)
			{
				::DrawFrameControl(dc, rc, DFC_BUTTON, DFCS_BUTTONPUSH | DFCS_PUSHED);
				if (i >= 0)
				{
					::BitBlt(dc, x+1, y+1, 16, 16, dc2, 16*i, 0, SRCCOPY);
				}
			}
			else
			{
				::DrawFrameControl(dc, rc, DFC_BUTTON, DFCS_BUTTONPUSH);
				if (i >= 0)
				{
					::BitBlt(dc, x, y, 16, 16, dc2, 16*i, 0, SRCCOPY);
				}
			}
			::DeleteDC(dc2);

			if (state & ODS_FOCUS)
			{
				::SendMessage(hWnd, DM_SETDEFID, di->CtlID, 0);

				rc->left += 3;
				rc->top += 3;
				rc->right -= 4;
				rc->bottom -= 4;
				::DrawFocusRect(dc, rc);
			}
		}
		return TRUE;



	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// zm�na velikosti bloku

// aktualizace z�visl�ch parametr�
void ResizeAktText()
{
	if (ResizeWidth < 1) ResizeWidth = 1;
	if (ResizeWidth > 0x8000) ResizeWidth = 0x8000;

	if (ResizeHeight < 1) ResizeHeight = 1;
	if (ResizeHeight > 0x8000) ResizeHeight = 0x8000;

	if (ResizeModeProp)
	{
		if (ResizeAktWidth)
		{
			ResizeHeight = Round((double)ResizeWidth/ResizeOldWidth*ResizeOldHeight);
		}
		else
		{
			ResizeWidth = Round((double)ResizeHeight/ResizeOldHeight*ResizeOldWidth);
		}
	}
}

// nastaven� rozm�r� (all = v�etn� aktivn�ch)
void ResizeSetText(HWND wnd, BOOL all)
{
	ResizeFill = TRUE;

	CText txt;

	BOOL widthall = (all || (ResizeModeProp && !ResizeAktWidth));
	BOOL heightall = (all || (ResizeModeProp && ResizeAktWidth));

	if (widthall || (ResizeMode != ResizeModeStep))
	{
		txt.Double((double)Round((double)ResizeWidth/ICONWIDTH*100)/100);
		txt.SetDialogText(wnd, IDN_PICRESIZE_STEPX);
	}

	if (heightall || (ResizeMode != ResizeModeStep))
	{
		txt.Double((double)Round((double)ResizeHeight/ICONHEIGHT*100)/100);
		txt.SetDialogText(wnd, IDN_PICRESIZE_STEPY);
	}

	if (widthall || (ResizeMode != ResizeModePoint))
	{
		txt.Int(ResizeWidth);
		txt.SetDialogText(wnd, IDN_PICRESIZE_POINTX);
	}

	if (heightall || (ResizeMode != ResizeModePoint))
	{
		txt.Int(ResizeHeight);
		txt.SetDialogText(wnd, IDN_PICRESIZE_POINTY);
	}

	if (widthall || (ResizeMode != ResizeModeProc))
	{
		if ((ResizeMode == ResizeModeProc) && ResizeModeProp && !ResizeAktWidth)
		{
			txt.Double(ResizeHeightProc);
		}
		else
		{
			if ((ResizeMode == ResizeModeProc) && widthall)
			{
				txt.Double(ResizeWidthProc);
			}
			else
			{
				txt.Double((double)Round((double)ResizeWidth/ResizeOldWidth*10000)/100);
			}
		}
		txt.SetDialogText(wnd, IDN_PICRESIZE_PROCX);
	}

	if (heightall || (ResizeMode != ResizeModeProc))
	{
		if ((ResizeMode == ResizeModeProc) && ResizeModeProp && ResizeAktWidth)
		{
			txt.Double(ResizeWidthProc);
		}
		else
		{
			if ((ResizeMode == ResizeModeProc) && heightall)
			{
				txt.Double(ResizeHeightProc);
			}
			else
			{
				txt.Double((double)Round((double)ResizeHeight/ResizeOldHeight*10000)/100);
			}
		}
		txt.SetDialogText(wnd, IDN_PICRESIZE_PROCY);
	}

	ResizeFill = FALSE;
}

// na�ten� zadan�ch rozm�r�
void ResizeGetText(HWND wnd)
{
	if (!ResizeFill)
	{
		CText txt;

		switch(ResizeMode)
		{
		case ResizeModeStep:
			if (!ResizeModeProp || ResizeAktWidth)
			{
				txt.GetDialogText(wnd, IDN_PICRESIZE_STEPX);
				ResizeWidth = Round(Double(txt)*ICONWIDTH);
			}

			if (!ResizeModeProp || !ResizeAktWidth)
			{
				txt.GetDialogText(wnd, IDN_PICRESIZE_STEPY);
				ResizeHeight = Round(Double(txt)*ICONHEIGHT);
			}
			break;

		case ResizeModePoint:
			if (!ResizeModeProp || ResizeAktWidth)
			{
				txt.GetDialogText(wnd, IDN_PICRESIZE_POINTX);
				ResizeWidth = Int(txt);
			}

			if (!ResizeModeProp || !ResizeAktWidth)
			{
				txt.GetDialogText(wnd, IDN_PICRESIZE_POINTY);
				ResizeHeight = Int(txt);
			}
			break;

		case ResizeModeProc:
			if (!ResizeModeProp || ResizeAktWidth)
			{
				txt.GetDialogText(wnd, IDN_PICRESIZE_PROCX);
				ResizeWidthProc = Double(txt);
				if (ResizeModeProp) ResizeHeightProc = ResizeWidthProc;
				ResizeWidth = Round(ResizeWidthProc/100*ResizeOldWidth);
			}

			if (!ResizeModeProp || !ResizeAktWidth)
			{
				txt.GetDialogText(wnd, IDN_PICRESIZE_PROCY);
				ResizeHeightProc = Double(txt);
				if (ResizeModeProp) ResizeWidthProc = ResizeHeightProc;
				ResizeHeight = Round(ResizeHeightProc/100*ResizeOldHeight);
			}
			break;
		}

		ResizeAktText();
	}
}

// nastaven� p�ep�na��
void ResizeSetSwc(HWND wnd)
{
	ResizeFill = TRUE;

	DialogSetCheck(wnd, IDN_PICRESIZE_STEP, (ResizeMode == ResizeModeStep));
	DialogSetCheck(wnd, IDN_PICRESIZE_POINT, (ResizeMode == ResizeModePoint));
	DialogSetCheck(wnd, IDN_PICRESIZE_PROC, (ResizeMode == ResizeModeProc));

	DialogSetCheck(wnd, IDN_PICRESIZE_PROP, ResizeModeProp);
	DialogSetCheck(wnd, IDN_PICRESIZE_INT, ResizeModeInt);
	DialogSetCheck(wnd, IDN_PICRESIZE_DITH, ResizeModeDith);

	ResizeFill = FALSE;
}

// aktualizace fokusu
void ResizeAktFocus(HWND wnd, BOOL aktwidth)
{
	int focus;

	switch(ResizeMode)
	{
	case ResizeModeStep:
		focus = IDN_PICRESIZE_STEPX;
		if (!aktwidth) focus = IDN_PICRESIZE_STEPY;
		break;

	case ResizeModePoint:
		focus = IDN_PICRESIZE_POINTX;
		if (!aktwidth) focus = IDN_PICRESIZE_POINTY;
		break;

	default:
		focus = IDN_PICRESIZE_PROCX;
		if (!aktwidth) focus = IDN_PICRESIZE_PROCY;
	}
	::SetFocus(::GetDlgItem(wnd, focus));
}


// callback funkce dialogu
BOOL CALLBACK ResizeDialogProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
	case WM_INITDIALOG:
		{
			CenterWindow(hWnd, MainFrame);

			CText txt;

			txt.Load(IDN_PICRESIZE);
			txt.SetWindowTextFont(hWnd);

			DialogCustomText(hWnd, IDN_SOUCASNE_ROZMERY);
			DialogCustomText(hWnd, IDN_KROKU);
			DialogCustomText(hWnd, IDN_BODU);
			DialogCustomBox(hWnd, IDN_NOVE_ROZMERY);
			DialogCustomText(hWnd, IDN_SIRKA2);
			DialogCustomText(hWnd, IDN_VYSKA2);
			DialogCustomRadio(hWnd, IDN_PICRESIZE_STEP);
			DialogCustomRadio(hWnd, IDN_PICRESIZE_POINT);
			DialogCustomRadio(hWnd, IDN_PICRESIZE_PROC);
			DialogCustomCheck(hWnd, IDN_PICRESIZE_PROP);
			DialogCustomCheck(hWnd, IDN_PICRESIZE_INT);
			DialogCustomCheck(hWnd, IDN_PICRESIZE_DITH);

			txt.Load(IDN_CANCEL0);
			txt.SetDialogTextFont(hWnd, IDCANCEL);

			ResizeSetSwc(hWnd);

			txt.Int(ResizeOldWidth);
			txt += _T(" x ");
			txt.AddInt(ResizeOldHeight);
			txt.SetDialogTextFont(hWnd, IDN_PICRESIZE_DIMPOINT);

			txt.Double((double)Round((double)ResizeOldWidth/ICONWIDTH*100)/100);
			txt += _T(" x ");
			txt.AddDouble((double)Round((double)ResizeOldHeight/ICONHEIGHT*100)/100);
			txt.SetDialogTextFont(hWnd, IDN_PICRESIZE_DIMSTEP);

			if (ResizeMode == ResizeModeProc)
			{
				ResizeWidth = Round(ResizeWidthProc/100*ResizeOldWidth);
				ResizeHeight = Round(ResizeHeightProc/100*ResizeOldHeight);
			}
			else
			{
				ResizeWidthProc = (double)Round((double)ResizeWidth/ResizeOldWidth*10000)/100;
				ResizeHeightProc = (double)Round((double)ResizeHeight/ResizeOldHeight*10000)/100;
			}

			ResizeAktText();
			ResizeSetSwc(hWnd);
			ResizeSetText(hWnd, TRUE);
			ResizeAktFocus(hWnd, ResizeAktWidth);
		}
		return FALSE;

	case WM_DRAWITEM:				// 0x02B
		if (DialogDraw(hWnd, lParam)) return TRUE;
		return FALSE;

	case WM_SETFOCUS:
	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
		case 0:
		case 1:
			switch (LOWORD(wParam))
			{
			case IDOK:
			case IDCANCEL:
				::EndDialog(hWnd, LOWORD(wParam));
				return TRUE;

			case IDN_PICRESIZE_STEP:
				ResizeMode = ResizeModeStep;
				goto picresize;

			case IDN_PICRESIZE_POINT:
				ResizeMode = ResizeModePoint;
				goto picresize;

			case IDN_PICRESIZE_PROC:
				ResizeMode = ResizeModeProc;
				goto picresize;

			case IDN_PICRESIZE_PROP:
				ResizeModeProp = !ResizeModeProp;
				goto picresize;

			case IDN_PICRESIZE_INT:
				ResizeModeInt = !ResizeModeInt;
				goto picresize;

			case IDN_PICRESIZE_DITH:
				ResizeModeDith = !ResizeModeDith;

picresize:
				if (!ResizeFill)
				{
					ResizeAktText();
					ResizeSetSwc(hWnd);
					ResizeSetText(hWnd, TRUE);
				}
				break;
			}
			break;

		case EN_CHANGE:
			if (!ResizeFill)
			{
				switch (LOWORD(wParam))
				{
				case IDN_PICRESIZE_STEPX:
					ResizeMode = ResizeModeStep;
					ResizeAktWidth = TRUE;
					break;

				case IDN_PICRESIZE_STEPY:
					ResizeMode = ResizeModeStep;
					ResizeAktWidth = FALSE;
					break;

				case IDN_PICRESIZE_POINTX:
					ResizeMode = ResizeModePoint;
					ResizeAktWidth = TRUE;
					break;

				case IDN_PICRESIZE_POINTY:
					ResizeMode = ResizeModePoint;
					ResizeAktWidth = FALSE;
					break;

				case IDN_PICRESIZE_PROCX:
					ResizeMode = ResizeModeProc;
					ResizeAktWidth = TRUE;
					break;

				case IDN_PICRESIZE_PROCY:
					ResizeMode = ResizeModeProc;
					ResizeAktWidth = FALSE;
					break;
				}

				ResizeSetSwc(hWnd);
				ResizeGetText(hWnd);

				switch (LOWORD(wParam))
				{
				case IDN_PICRESIZE_STEPX:
				case IDN_PICRESIZE_POINTX:
					ResizeWidthProc = (double)Round((double)ResizeWidth/ResizeOldWidth*10000)/100;
					if (ResizeModeProp)
					{
						ResizeHeightProc = (double)Round((double)ResizeHeight/ResizeOldHeight*10000)/100;
					}
					break;

				case IDN_PICRESIZE_STEPY:
				case IDN_PICRESIZE_POINTY:
					ResizeHeightProc = (double)Round((double)ResizeHeight/ResizeOldHeight*10000)/100;
					if (ResizeModeProp)
					{
						ResizeWidthProc = (double)Round((double)ResizeWidth/ResizeOldWidth*10000)/100;
					}
					break;
				}

				ResizeSetText(hWnd, FALSE);
			}
			break;
		}
		break;
	}
	return FALSE;
}


// zm�na velikosti obr�zku nebo bloku
void EditPicResize(int width, int height, BOOL inter, BOOL dith)
{
// omezen� zadan�ch �daj�
	if (width < 1) width = 1;
	if (width > 0x8000) width = 0x8000;
	if (height < 1) height = 1;
	if (height > 0x8000) height = 0x8000;

// kontrola, zda se rozm�ry m�n�
	if ((width == ResizeOldWidth) && (height == ResizeOldHeight)) return;

// zapnut� kurzoru �ek�n�
	BeginWaitCursor();

// zm�na velikosti obr�zku, nen�-li ozna�en blok
	if (!Selecting && (BufID == BufPicID))
	{
		if (Index == -2)
		{
			Undo.AddSprSet(EditItemSprite, Sprite[EditItemSprite]);
			Sprite[EditItemSprite].CopyWrite();

			for (int ii = Sprite[EditItemSprite].Faze()*Sprite[EditItemSprite].Smer()-1; ii >= 0; ii--)
			{
				Sprite[EditItemSprite][ii].Zoom(width, height, inter, dith);
			}

			Sprite[EditItemSprite][EditItemPic].DeComp();
			Data = Sprite[EditItemSprite][EditItemPic].DataData();
			Width = Sprite[EditItemSprite][EditItemPic].Width();
			Height = Sprite[EditItemSprite][EditItemPic].Height();

			Undo.Stop();
		}
		else
		{
			PushUndo();

			Picture[Index].Zoom(width, height, inter, dith);
			Picture[Index].DeComp();
			Data = Picture[Index].DataData();
			Width = Picture[Index].Width();
			Height = Picture[Index].Height();
		}

		PushBuf = (BYTE*)MemSize(PushBuf, Width * Height * sizeof(BYTE));
		if ((DWORD)MidX >= (DWORD)Width) MidX = Width/2;
		if ((DWORD)MidY >= (DWORD)Height) MidY = Height/2;

// p�epo�et zobrazen�
		ReCalc();
		DispLeft = MidX - DispWidth/2;
		DispBottom = MidY - DispHeight/2;
		ReCalc();
		SetScroll();
		ProgOnSize();
	}
	else
	{

// �schova UNDO
		PushUndo();

// p��prava bloku, nen�-li ozna�en
		if (!Selecting) SelectAll();

// zm�na velikosti bloku
		CPicture pic;

		if (pic.New(BlokWidth, BlokHeight) &&
			pic.CopyData(BlokBuf) &&
			pic.Zoom(width, height, inter, dith) &&
			pic.DeComp() &&
			((BlokBuf = (BYTE*)MemSize(BlokBuf, pic.Size())) != NULL))
		{
			BlokWidth = pic.Width();
			BlokHeight = pic.Height();
			MemCopy(BlokBuf, pic.DataData(), pic.Size());
		}

// korekce po��tku k zobrazen� bloku
		if ((BlokX > (DispLeft+DispWidth-2)) ||
			(BlokY > (Height-DispBottom-2)) ||
			((BlokX + BlokWidth) < (DispLeft+2)) ||
			((BlokY + BlokHeight) < (Height - DispBottom - DispHeight + 2)))
		{
			BlokX = DispLeft + DispWidth/2 - BlokWidth/2;
			BlokY = Height - DispBottom - DispHeight/2 - BlokHeight/2;
		}

// polo�en� bloku
		MoveSelect();
		DispSelect();
	}

// p�ekreslen� displeje
	Disp();

// p��znak modifikace souboru
	SetModi();

// aktualizace voleb pro blok
	UpdateClipboard();

// aktualizace zobrazen� strom�
	UpdateTree();

// vypnut� kurzoru �ek�n�
	EndWaitCursor();
}


// proveden� editace bloku (-1 = zat�m neur�eno)
void EditBlok(int mode)
{
// kontrola, zda je re�im editace
	if (!ProgMode ||
		((EditMode != BufIcoID) && (EditMode != BufPicID)))
		return;

// p��prava buffer�
	if (BufID == BufPicID)
	{
		if (Index == -2)
		{
			Sprite[EditItemSprite][EditItemPic].DeComp();
			Data = Sprite[EditItemSprite][EditItemPic].DataData();
		}
		else
		{
			Picture[Index].DeComp();
			Data = Picture[Index].DataData();
		}
	}
	else
	{
		Icon[Index].DeComp();
		Data = Icon[Index].DataData();
	}

// zad�n� operace
	if (mode < 0)
	{
		mode = ::DialogBoxParam(
				hInstance,
				MAKEINTRESOURCE(IDN_PICBLOK),
				MainFrame,
				(DLGPROC)BlokDialogProc,
				(LPARAM)IDN_PICBLOK);
	}
	
	if (mode == IDCANCEL) return;

// �schova velikosti pro zm�nu velikosti bloku
	ResizeOldWidth = Width;
	ResizeOldHeight = Height;

	if (Selecting)
	{
		ResizeOldWidth = BlokWidth;
		ResizeOldHeight = BlokHeight;
	}

// dvojn�sobn� velikost
	if (mode == IDN_MUL2)
	{
		EditPicResize(ResizeOldWidth*2, ResizeOldHeight*2, FALSE, FALSE);
		return;
	}

// polovi�n� velikost
	if (mode == IDN_DIV2)
	{
		EditPicResize(ResizeOldWidth/2, ResizeOldHeight/2, TRUE, Dither);
		return;
	}

// zm�na velikosti bloku
	if (mode == IDN_MULDIV)
	{
		if (::DialogBoxParam(
			hInstance,
			MAKEINTRESOURCE(IDN_PICRESIZE),
			MainFrame,
			(DLGPROC)ResizeDialogProc,
			(LPARAM)IDN_PICRESIZE) == IDOK)
		{
			EditPicResize(ResizeWidth, ResizeHeight, ResizeModeInt, ResizeModeDith);
		}
		return;
	}

// zapnut� kurzoru �ek�n�
	BeginWaitCursor();

// �schova do UNDO
	PushUndo();

// lok�ln� prom�nn�
	int x, y;							// ukazatel sou�adnic
	BYTE* blok2;						// pomocn� buffer bloku
	int oldwidth;						// star� ���ka bloku
	int oldheight;						// star� v��ka bloku
	double uhel = 0;					// �hel pro rotaci
	double uhel2;						// pomocn� �hel pro rotaci
	double delka2;						// pomocn� d�lka pro rotaci
	int dx, dy;							// vzd�lenost od st�edu
	BYTE col;							// mezi�schova barvy
	int xr, yr;							// pomocn� registry
	BOOL oldsel = Selecting;			// �schova p��znaku ozna�en� bloku

// p��prava bloku, nen�-li ozna�en
	if (!Selecting)
	{
		SelectAll();
	}

// �schova ���ky a v��ky bloku
	oldwidth = BlokWidth;
	oldheight = BlokHeight;

// nastaven� nov�ch rozm�r� bloku
	switch (mode)
	{
	case IDN_LROT:
	case IDN_RROT:
		BlokWidth = oldheight;
		BlokHeight = oldwidth;
		break;

// �hel rotace
	case IDN_LROT67:
		uhel = (double)uhel67;
		goto ROTUHEL;

	case IDN_LROT60:
		uhel = (double)uhel60;
		goto ROTUHEL;

	case IDN_LROT45:
		uhel = (double)uhel45;
		goto ROTUHEL;

	case IDN_LROT30:
		uhel = (double)uhel30;
		goto ROTUHEL;

	case IDN_LROT22:
		uhel = (double)uhel22;
		goto ROTUHEL;

	case IDN_RROT67:
		uhel = (double)-uhel67;
		goto ROTUHEL;

	case IDN_RROT60:
		uhel = (double)-uhel60;
		goto ROTUHEL;

	case IDN_RROT45:
		uhel = (double)-uhel45;
		goto ROTUHEL;

	case IDN_RROT30:
		uhel = (double)-uhel30;
		goto ROTUHEL;

	case IDN_RROT22:
		uhel = (double)-uhel22;
ROTUHEL:
		uhel2 = atan2((double)oldheight, (double)oldwidth);
		delka2 = sqrt((double)(BlokWidth*BlokWidth + BlokHeight*BlokHeight));
		BlokHeight = (int)(delka2*sin(fabs(uhel)+uhel2)+0.9);
		BlokWidth = (int)(delka2*cos(fabs(uhel)-uhel2)+0.9);
		break;
	}

// korekce po��tku bloku
	BlokX += oldwidth/2 - BlokWidth/2;
	BlokY += oldheight/2 - BlokHeight/2;
	if (BlokX > (DispLeft + DispWidth - 5)) BlokX = DispLeft + DispWidth - 5;
	if (BlokY > (Height - DispBottom - 5)) BlokY = Height - DispBottom - 5;
	if ((BlokX + BlokWidth) < (DispLeft + 5)) BlokX = DispLeft + 5 - BlokWidth;
	if ((BlokY + BlokHeight) < (Height - DispBottom - DispHeight + 5)) 
		BlokY = Height - DispBottom - DispHeight + 5 - BlokHeight;

// �schova bloku a vytvo�en� jin�ho
	blok2 = BlokBuf;						// aktu�ln� blok
	BlokBuf = (BYTE*)MemGet(BlokWidth*BlokHeight);	// vytvo�en� nov�ho bloku

// cyklus p�es v�echny body
	for (x = 0; x < BlokWidth; x++)
	{
		for (y = 0; y < BlokHeight; y++)
		{
			switch (mode)
			{

// horizont�ln� p�evr�cen�
			case IDN_YFLIP:
				SetBlok(x,y,blok2[BlokWidth - 1 - x + BlokWidth*(BlokHeight-1-y)]);
				break;

// vertik�ln� p�evr�cen�
			case IDN_XFLIP:
				SetBlok(x,y,blok2[x + BlokWidth*y]);
				break;

// oto�en� vpravo
			case IDN_RROT:
				SetBlok(x,y,blok2[y + BlokHeight*(BlokWidth - 1 - (BlokWidth - 1 - x))]);
				break;

// oto�en� vlevo
			case IDN_LROT:
				SetBlok(x,y,blok2[BlokHeight - 1 - y + BlokHeight*(BlokWidth - 1 - x)]);
				break;

// z�m�na barev
			case IDN_XCOL:
				col = blok2[x + BlokWidth*(BlokHeight - 1 - y)];
				if (col == ColLeft)
				{
					SetBlok(x,y,ColRight);
				}
				else
				{
					if (col == ColRight)
					{
						SetBlok(x,y,ColLeft);
					}
					else
					{
						SetBlok(x, y, col);
					}
				}
				break;

// nahrazen� barvy
			case IDN_SCOL:
				col = blok2[x + BlokWidth*(BlokHeight - 1 - y)];
				if (col == ColLeft)
				{
					SetBlok(x,y,ColRight);
				}
				else
				{
					SetBlok(x, y, col);
				}
				break;

// oto�en�
			case IDN_LROT67:
			case IDN_LROT60:
			case IDN_LROT45:
			case IDN_LROT30:
			case IDN_LROT22:
			case IDN_RROT67:
			case IDN_RROT60:
			case IDN_RROT45:
			case IDN_RROT30:
			case IDN_RROT22:
				dx = x - BlokWidth/2;
				dy = y - BlokHeight/2;

				uhel2 = atan2((double)dy, (double)dx) + uhel;
				delka2 = sqrt((double)(dx*dx + dy*dy));
				xr = (int)(delka2*cos(uhel2)+0.5+oldwidth)-(oldwidth+1)/2;
				yr = (int)(delka2*sin(uhel2)+0.5+oldheight)-(oldheight+1)/2;

				if ((xr >= 0) && (yr >= 0) && (xr < oldwidth) && (yr < oldheight))
				{
					SetBlok(x,y,blok2[xr + oldwidth*(oldheight-1-yr)]);
				}
				else
				{
					SetBlok(x, y, BlokBack);
				}
				break;
			}
		}
	}

// zru�en� pomocn�ho bloku
	MemFree(blok2);

// p�ekreslen� bloku (pro obr�zek zm�na rozm�r�)
	DispSelect();
	if (oldsel || (Index == -2))
	{
		MoveSelect();
		DispSelect();
	}
	else
	{
		if (BufID == BufPicID)
		{
			ASSERT((Index == -2) || Picture.IsValid(Index));

			Height = BlokHeight;
			Width = BlokWidth;
			PushBuf = (BYTE*)MemSize(PushBuf, Width * Height * sizeof(BYTE));
			if ((DWORD)MidX >= (DWORD)Width) MidX = Width/2;
			if ((DWORD)MidY >= (DWORD)Height) MidY = Height/2;

			CPicture* pic;

			if (Index == -2)
			{
				EditItemSprite = EditSprite::Index;
				EditItemPic = EditSprite::IndexPic;
				pic = &Sprite[EditItemSprite][EditItemPic];
			}
			else
			{
				pic = &(Picture[Index]);	// adresa obr�zku
			}

			pic->New(Width, Height);
			Data = pic->DataData();

			BlokX = 0;
			BlokY = 0;
			FirstBlokX = 0;
			FirstBlokY = 0;
			FirstBlokWidth = Width;
			FirstBlokHeight = Height;
		}
		
		MoveSelect();
		Selecting = FALSE;
		SetMode(OldMode);
	}

// p�ekreslen� displeje
	Disp();

// p��znak modifikace souboru
	SetModi();

// aktualizace voleb pro blok
	UpdateClipboard();

// aktualizace zobrazen� strom�
	UpdateTree();

// vypnut� kurzoru �ek�n�
	EndWaitCursor();
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace voleb v�b�ru m�du

void UpdateMenu()
{
	SelectCombo(ToolEditIcon, Mode, ToolBarEditPicTools);
	SelectCombo(ToolEditIconWidth, PenWidthID, ToolBarEditPicWidths);
	if (FocusBuf == BufEdiID)
	{
		CheckCommand(IDN_RASTR, Rastr);
	}
}


// ************************** obsluha kreslen� ******************************


/////////////////////////////////////////////////////////////////////////////
// vykreslen� bodu
 
void _fastcall SetPoint(int x, int y, BYTE col)
{
// kontrola sou�adnic
	if ((x < 0) || (y < 0) ||
		(x >= Width) || (y >= Height))
		return;

// adresa bajtu dat
	BYTE* data = &Data[Width*(Height-1-y) + x];	// adresa za��tku dat obr�zku
	*data = col;
}


/////////////////////////////////////////////////////////////////////////////
// vykreslen� bodu �t�tce

void SetPaint(int x, int y, BYTE col)
{
	int i;

	switch (PenWidth)
	{
// *
	case 1:
		SetPoint(x  , y  , col);
		if (x < AktLeft) AktLeft = x;
		if (x >= AktRight) AktRight = x+1;
		if (y < AktTop) AktTop = y;
		if (y >= AktBottom) AktBottom = y+1;
		break;

// **
// **
	case 2:
		SetPoint(x-1, y-1, col);
		SetPoint(x  , y-1, col);

		SetPoint(x-1, y  , col);
		SetPoint(x  , y  , col);

		if ((x-1) < AktLeft) AktLeft = x-1;
		if (x >= AktRight) AktRight = x+1;
		if ((y-1) < AktTop) AktTop = y-1;
		if (y >= AktBottom) AktBottom = y+1;
		break;

// ***
// ***
// ***
	case 3:
		SetPoint(x-1, y-1, col);
		SetPoint(x  , y-1, col);
		SetPoint(x+1, y-1, col);

		SetPoint(x-1, y  , col);
		SetPoint(x  , y  , col);
		SetPoint(x+1, y  , col);

		SetPoint(x-1, y+1, col);
		SetPoint(x  , y+1, col);
		SetPoint(x+1, y+1, col);

		if ((x-1) < AktLeft) AktLeft = x-1;
		if ((x+1) >= AktRight) AktRight = x+2;
		if ((y-1) < AktTop) AktTop = y-1;
		if ((y+1) >= AktBottom) AktBottom = y+2;
		break;

//  ***
// *****
// *****
// *****
//  ***
	case 5:
		SetPoint(x-1, y-2, col);
		SetPoint(x  , y-2, col);
		SetPoint(x+1, y-2, col);

		SetPoint(x-2, y-1, col);
		SetPoint(x-1, y-1, col);
		SetPoint(x  , y-1, col);
		SetPoint(x+1, y-1, col);
		SetPoint(x+2, y-1, col);

		SetPoint(x-2, y  , col);
		SetPoint(x-1, y  , col);
		SetPoint(x  , y  , col);
		SetPoint(x+1, y  , col);
		SetPoint(x+2, y  , col);

		SetPoint(x-2, y+1, col);
		SetPoint(x-1, y+1, col);
		SetPoint(x  , y+1, col);
		SetPoint(x+1, y+1, col);
		SetPoint(x+2, y+1, col);

		SetPoint(x-1, y+2, col);
		SetPoint(x  , y+2, col);
		SetPoint(x+1, y+2, col);

		if ((x-2) < AktLeft) AktLeft = x-2;
		if ((x+2) >= AktRight) AktRight = x+3;
		if ((y-2) < AktTop) AktTop = y-2;
		if ((y+2) >= AktBottom) AktBottom = y+3;
		break;

//   *****  
//  ******* 
// *********
// *********
// *********
// *********
// *********
//  ******* 
//   *****  
	case 9:
		SetPoint(x-2, y-4, col);
		SetPoint(x-1, y-4, col);
		SetPoint(x  , y-4, col);
		SetPoint(x+1, y-4, col);
		SetPoint(x+2, y-4, col);

		SetPoint(x-3, y-3, col);
		SetPoint(x-2, y-3, col);
		SetPoint(x-1, y-3, col);
		SetPoint(x  , y-3, col);
		SetPoint(x+1, y-3, col);
		SetPoint(x+2, y-3, col);
		SetPoint(x+3, y-3, col);

		for (i = y-2; i <= y+2; i++)
		{
			SetPoint(x-4, i, col);
			SetPoint(x-3, i, col);
			SetPoint(x-2, i, col);
			SetPoint(x-1, i, col);
			SetPoint(x  , i, col);
			SetPoint(x+1, i, col);
			SetPoint(x+2, i, col);
			SetPoint(x+3, i, col);
			SetPoint(x+4, i, col);
		}

		SetPoint(x-3, y+3, col);
		SetPoint(x-2, y+3, col);
		SetPoint(x-1, y+3, col);
		SetPoint(x  , y+3, col);
		SetPoint(x+1, y+3, col);
		SetPoint(x+2, y+3, col);
		SetPoint(x+3, y+3, col);

		SetPoint(x-2, y+4, col);
		SetPoint(x-1, y+4, col);
		SetPoint(x  , y+4, col);
		SetPoint(x+1, y+4, col);
		SetPoint(x+2, y+4, col);

		if ((x-4) < AktLeft) AktLeft = x-4;
		if ((x+4) >= AktRight) AktRight = x+5;
		if ((y-4) < AktTop) AktTop = y-4;
		if ((y+4) >= AktBottom) AktBottom = y+5;
		break;

//        *******          10
//      ***********        9
//     *************       8
//    ***************      7
//   *****************     6
//  *******************    5
//  *******************    4
// *********************   3
// *********************   2
// *********************   1
// *********************   0
// *********************  -1
// *********************  -2
// *********************  -3
//  *******************   -4
//  *******************   -5
//   *****************    -6
//    ***************     -7
//     *************      -8
//      ***********       -9
//        *******         -10
	case 21:
		SetPoint(x- 3, y-10, col);
		SetPoint(x- 2, y-10, col);
		SetPoint(x- 1, y-10, col);
		SetPoint(x   , y-10, col);
		SetPoint(x+ 1, y-10, col);
		SetPoint(x+ 2, y-10, col);
		SetPoint(x+ 3, y-10, col);

		SetPoint(x- 5, y- 9, col);
		SetPoint(x- 4, y- 9, col);
		SetPoint(x- 3, y- 9, col);
		SetPoint(x- 2, y- 9, col);
		SetPoint(x- 1, y- 9, col);
		SetPoint(x   , y- 9, col);
		SetPoint(x+ 1, y- 9, col);
		SetPoint(x+ 2, y- 9, col);
		SetPoint(x+ 3, y- 9, col);
		SetPoint(x+ 4, y- 9, col);
		SetPoint(x+ 5, y- 9, col);

		SetPoint(x- 6, y- 8, col);
		SetPoint(x- 5, y- 8, col);
		SetPoint(x- 4, y- 8, col);
		SetPoint(x- 3, y- 8, col);
		SetPoint(x- 2, y- 8, col);
		SetPoint(x- 1, y- 8, col);
		SetPoint(x   , y- 8, col);
		SetPoint(x+ 1, y- 8, col);
		SetPoint(x+ 2, y- 8, col);
		SetPoint(x+ 3, y- 8, col);
		SetPoint(x+ 4, y- 8, col);
		SetPoint(x+ 5, y- 8, col);
		SetPoint(x+ 6, y- 8, col);

		SetPoint(x- 7, y- 7, col);
		SetPoint(x- 6, y- 7, col);
		SetPoint(x- 5, y- 7, col);
		SetPoint(x- 4, y- 7, col);
		SetPoint(x- 3, y- 7, col);
		SetPoint(x- 2, y- 7, col);
		SetPoint(x- 1, y- 7, col);
		SetPoint(x   , y- 7, col);
		SetPoint(x+ 1, y- 7, col);
		SetPoint(x+ 2, y- 7, col);
		SetPoint(x+ 3, y- 7, col);
		SetPoint(x+ 4, y- 7, col);
		SetPoint(x+ 5, y- 7, col);
		SetPoint(x+ 6, y- 7, col);
		SetPoint(x+ 7, y- 7, col);

		SetPoint(x- 8, y- 6, col);
		SetPoint(x- 7, y- 6, col);
		SetPoint(x- 6, y- 6, col);
		SetPoint(x- 5, y- 6, col);
		SetPoint(x- 4, y- 6, col);
		SetPoint(x- 3, y- 6, col);
		SetPoint(x- 2, y- 6, col);
		SetPoint(x- 1, y- 6, col);
		SetPoint(x   , y- 6, col);
		SetPoint(x+ 1, y- 6, col);
		SetPoint(x+ 2, y- 6, col);
		SetPoint(x+ 3, y- 6, col);
		SetPoint(x+ 4, y- 6, col);
		SetPoint(x+ 5, y- 6, col);
		SetPoint(x+ 6, y- 6, col);
		SetPoint(x+ 7, y- 6, col);
		SetPoint(x+ 8, y- 6, col);

		for (i = y-5; i <= y-4; i++)
		{
			SetPoint(x- 9, i, col);
			SetPoint(x- 8, i, col);
			SetPoint(x- 7, i, col);
			SetPoint(x- 6, i, col);
			SetPoint(x- 5, i, col);
			SetPoint(x- 4, i, col);
			SetPoint(x- 3, i, col);
			SetPoint(x- 2, i, col);
			SetPoint(x- 1, i, col);
			SetPoint(x   , i, col);
			SetPoint(x+ 1, i, col);
			SetPoint(x+ 2, i, col);
			SetPoint(x+ 3, i, col);
			SetPoint(x+ 4, i, col);
			SetPoint(x+ 5, i, col);
			SetPoint(x+ 6, i, col);
			SetPoint(x+ 7, i, col);
			SetPoint(x+ 8, i, col);
			SetPoint(x+ 9, i, col);
		}

		for (i = y-3; i <= y+3; i++)
		{
			SetPoint(x-10, i, col);
			SetPoint(x- 9, i, col);
			SetPoint(x- 8, i, col);
			SetPoint(x- 7, i, col);
			SetPoint(x- 6, i, col);
			SetPoint(x- 5, i, col);
			SetPoint(x- 4, i, col);
			SetPoint(x- 3, i, col);
			SetPoint(x- 2, i, col);
			SetPoint(x- 1, i, col);
			SetPoint(x   , i, col);
			SetPoint(x+ 1, i, col);
			SetPoint(x+ 2, i, col);
			SetPoint(x+ 3, i, col);
			SetPoint(x+ 4, i, col);
			SetPoint(x+ 5, i, col);
			SetPoint(x+ 6, i, col);
			SetPoint(x+ 7, i, col);
			SetPoint(x+ 8, i, col);
			SetPoint(x+ 9, i, col);
			SetPoint(x+10, i, col);
		}

		for (i = y+4; i <= y+5; i++)
		{
			SetPoint(x- 9, i, col);
			SetPoint(x- 8, i, col);
			SetPoint(x- 7, i, col);
			SetPoint(x- 6, i, col);
			SetPoint(x- 5, i, col);
			SetPoint(x- 4, i, col);
			SetPoint(x- 3, i, col);
			SetPoint(x- 2, i, col);
			SetPoint(x- 1, i, col);
			SetPoint(x   , i, col);
			SetPoint(x+ 1, i, col);
			SetPoint(x+ 2, i, col);
			SetPoint(x+ 3, i, col);
			SetPoint(x+ 4, i, col);
			SetPoint(x+ 5, i, col);
			SetPoint(x+ 6, i, col);
			SetPoint(x+ 7, i, col);
			SetPoint(x+ 8, i, col);
			SetPoint(x+ 9, i, col);
		}

		SetPoint(x- 8, y+ 6, col);
		SetPoint(x- 7, y+ 6, col);
		SetPoint(x- 6, y+ 6, col);
		SetPoint(x- 5, y+ 6, col);
		SetPoint(x- 4, y+ 6, col);
		SetPoint(x- 3, y+ 6, col);
		SetPoint(x- 2, y+ 6, col);
		SetPoint(x- 1, y+ 6, col);
		SetPoint(x   , y+ 6, col);
		SetPoint(x+ 1, y+ 6, col);
		SetPoint(x+ 2, y+ 6, col);
		SetPoint(x+ 3, y+ 6, col);
		SetPoint(x+ 4, y+ 6, col);
		SetPoint(x+ 5, y+ 6, col);
		SetPoint(x+ 6, y+ 6, col);
		SetPoint(x+ 7, y+ 6, col);
		SetPoint(x+ 8, y+ 6, col);

		SetPoint(x- 7, y+ 7, col);
		SetPoint(x- 6, y+ 7, col);
		SetPoint(x- 5, y+ 7, col);
		SetPoint(x- 4, y+ 7, col);
		SetPoint(x- 3, y+ 7, col);
		SetPoint(x- 2, y+ 7, col);
		SetPoint(x- 1, y+ 7, col);
		SetPoint(x   , y+ 7, col);
		SetPoint(x+ 1, y+ 7, col);
		SetPoint(x+ 2, y+ 7, col);
		SetPoint(x+ 3, y+ 7, col);
		SetPoint(x+ 4, y+ 7, col);
		SetPoint(x+ 5, y+ 7, col);
		SetPoint(x+ 6, y+ 7, col);
		SetPoint(x+ 7, y+ 7, col);

		SetPoint(x- 6, y+ 8, col);
		SetPoint(x- 5, y+ 8, col);
		SetPoint(x- 4, y+ 8, col);
		SetPoint(x- 3, y+ 8, col);
		SetPoint(x- 2, y+ 8, col);
		SetPoint(x- 1, y+ 8, col);
		SetPoint(x   , y+ 8, col);
		SetPoint(x+ 1, y+ 8, col);
		SetPoint(x+ 2, y+ 8, col);
		SetPoint(x+ 3, y+ 8, col);
		SetPoint(x+ 4, y+ 8, col);
		SetPoint(x+ 5, y+ 8, col);
		SetPoint(x+ 6, y+ 8, col);

		SetPoint(x- 5, y+ 9, col);
		SetPoint(x- 4, y+ 9, col);
		SetPoint(x- 3, y+ 9, col);
		SetPoint(x- 2, y+ 9, col);
		SetPoint(x- 1, y+ 9, col);
		SetPoint(x   , y+ 9, col);
		SetPoint(x+ 1, y+ 9, col);
		SetPoint(x+ 2, y+ 9, col);
		SetPoint(x+ 3, y+ 9, col);
		SetPoint(x+ 4, y+ 9, col);
		SetPoint(x+ 5, y+ 9, col);

		SetPoint(x- 3, y+10, col);
		SetPoint(x- 2, y+10, col);
		SetPoint(x- 1, y+10, col);
		SetPoint(x   , y+10, col);
		SetPoint(x+ 1, y+10, col);
		SetPoint(x+ 2, y+10, col);
		SetPoint(x+ 3, y+10, col);

		if ((x-10) < AktLeft) AktLeft = x-10;
		if ((x+10) >= AktRight) AktRight = x+11;
		if ((y-10) < AktTop) AktTop = y-10;
		if ((y+10) >= AktBottom) AktBottom = y+11;
		break;

	default:
		ASSERTERROR;
//		int x1 = x - (m_PenWidth+1)/2;
//		int y1 = y - (m_PenWidth+1)/2;
//		int x2 = x + m_PenWidth/2;
//		int y2 = y + (m_PenWidth+1)/2;

//		SetRoundFill(x1, y, x2, y, col);

//		if (x1 < m_AktLeft) m_AktLeft = x1;
//		if (x2 >= m_AktRight) m_AktRight = x2+1;
//		if (y1 < m_AktTop) m_AktTop = y1;
//		if (y2 >= m_AktBottom) m_AktBottom = y2+1;
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� bodu spreje

void SetSpray(int x, int y, BYTE col)
{
	int m;
	switch (PenWidth)
	{
	case 1:
		m = 4;
		break;
	case 2:
	case 3:
		m = 8;
		break;
	case 4:
	case 5:
	case 6:
		m = 16;
		break;
	case 7:
	case 8:
	case 9:
	case 10:
		m = 32;
		break;
	default:
		m = 64;
	}

	int old = PenWidth;
	PenWidth = 1;

	for (int i = (m-3)*2; i > 0; i--)
	{
		if ((rand() & 0x7f) < 10)
		{
			SetPaint(x + (rand() & (m-1)) - (m/2-1), y + (rand() & (m/2-1)) - (m/4-1), col);
		}

		if ((rand() & 0x7f) < 10)
		{
			SetPaint(x + (rand() & (m/2-1)) - (m/4-1), y + (rand() & (m-1)) - (m/2-1), col);
		}

		if ((rand() & 0x7f) < 10)
		{
			SetPaint(x + (rand() & (m-1)) - (m/2-1), y + (rand() & (m-1)) - (m/2-1), col);
		}
	}
	PenWidth = old;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� ��ry �t�tcem

void SetLinePaint(int x1, int y1, int x2, int y2, BYTE col)
{
// lok�ln� prom�nn�
	int i;						// pracovn� ��ta�
	int dx;						// rozd�l sou�adnic X
	int dy;						// rozd�l sou�adnic Y

// oprava sou�adnic, aby ��ra �la v�dy zleva doprava (tj. x1 vlevo, x2 vpravo)
	if (x2 < x1)
	{
		i = x2; x2 = x1; x1 = i;
		i = y2; y2 = y1; y1 = i;
	}

// rozd�l sou�adnic X
	dx = x2 - x1;

// sm�r nahoru (tj. y1 dole, y2 naho�e)
	if (y1 >= y2)
	{

// rozd�l sou�adnic Y
		dy = y1 - y2;

// strm� nahoru
		if (dx <= dy)
		{
			for (i = 0; i < dy; i++)
			{
				SetPaint(x1 + (dx*i + dy/2)/dy, y1, col);
				y1--;
			}
		}

// m�rn� nahoru
		else
		{
			for (i = 0; i < dx; i++)
			{
				SetPaint(x1, y1 - (dy*i + dx/2)/dx, col);
				x1++;
			}
		}
	}

// sm�r dol� (tj. y1 naho�e, y2 dole)
	else
	{

// rozd�l sou�adnic Y
		dy = y2 - y1;

// strom� dol�
		if (dx <= dy)
		{
			for (i = 0; i < dy; i++)
			{
				SetPaint(x1 + (dx*i + dy/2)/dy, y1, col);
				y1++;
			}
		}

// m�rn� dol�
		else
		{
			for (i = 0; i < dx; i++)
			{
				SetPaint(x1, y1 + (dy*i + dx/2)/dx, col);
				x1++;
			}
		}
	}

// vykreslen� koncov�ho bodu (ned�lat v cyklu pro p��pad dx,dy = 0)
	SetPaint(x2, y2, col);
}


/////////////////////////////////////////////////////////////////////////////
// vykreslen� obd�ln�ku

void SetRect(int x1, int y1, int x2, int y2, BYTE col)
{
	SetLinePaint(x1, y1, x2, y1, col);
	SetLinePaint(x1, y1, x1, y2, col);
	SetLinePaint(x1, y2, x2, y2, col);
	SetLinePaint(x2, y1, x2, y2, col);
}


/////////////////////////////////////////////////////////////////////////////
// vykreslen� obd�ln�ku s v�pln�

void SetRectFill(int x1, int y1, int x2, int y2, BYTE col)
{
	int width;					// ���ka linky
	int height;					// po�et linek
	BYTE* dst;					// ukl�dac� adresa

// oprava po�ad� bod� X, aby byl X1 p�ed X2
	if (x1 > x2)
	{
		width = x1;
		x1 = x2;
		x2 = width;
	}

// oprava po�ad� bod� Y, aby byl Y1 p�ed Y2
	if (y1 > y2)
	{
		height = y1;
		y1 = y2;
		y2 = height;
	}

// omezen� krajn�ch bod�
	if (x1 < 0) x1 = 0;
	if (x2 >= Width) x2 = Width - 1;
	width = x2 - x1 + 1;
	if (width <= 0) return;
	if (y1 < 0) y1 = 0;
	if (y2 >= Height) y2 = Height - 1;
	height = y2 - y1 + 1;

// aktualizace displeje
	if (x1 < AktLeft) AktLeft = x1;
	if (x2 >= AktRight) AktRight = x2+1;
	if (y1 < AktTop) AktTop = y1;
	if (y2 >= AktBottom) AktBottom = y2+1;

// p��prava ukl�dac� adresy dat
	dst = Data + (Height - 1 - y2)*Width + x1;
		
// vykreslen� obd�ln�ku
	for (; height > 0; height--)
	{
		MemFill(dst, width, col);
		dst += Width;
	}
}


/////////////////////////////////////////////////////////////////////////////
// vykreslen� kru�nice

void SetRound(int x1, int y1, int x2, int y2, BYTE col)
{
// lok�ln� prom�nn�
	int sx, sy;							// st�ed kru�nice
	int dx,dy;							// vzd�lenost bodu od st�edu
	double polomer;						// polom�r
	double polomer2;					// kvadr�t polom�ru
	double width = PenWidth - 0.5;		// ���ka pera

// st�ed kru�nice
	sx = (x1 + x2)/2;					// sou�adnice st�edu X
	sy = (y1 + y2)/2;					// sou�adnice st�edu Y

// po��te�n� polom�r kru�nice (nejmen��)
	int pol = (int)(sqrt((double)((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1))) + 0.8);
	polomer = (double)(pol/2) - width/2;
	pol = pol % 2;

// cyklus p�es v�echny pr�m�ry
	for (; width > 0; width -= 0.51)
	{
		polomer2 = polomer*polomer;

// cyklus p�es v�echny body
		for (dx = (int)(polomer * 0.8 + 0.8); dx >= 0; dx--)
		{
			dy = (int)(sqrt(polomer2 - (double)dx*dx) + 0.7);

			SetPoint(sx - dx		, sy + dy + pol	, col);
			SetPoint(sx + dx + pol	, sy + dy + pol	, col);
			SetPoint(sx - dx		, sy - dy		, col);
			SetPoint(sx + dx + pol	, sy - dy		, col);

			SetPoint(sx - dy		, sy + dx + pol	, col);
			SetPoint(sx + dy + pol	, sy + dx + pol	, col);
			SetPoint(sx - dy		, sy - dx		, col);
			SetPoint(sx + dy + pol	, sy - dx		, col);
		}
		polomer += 0.52;
	}
}


/////////////////////////////////////////////////////////////////////////////
// kreslen� kruhu

void SetRoundFill(int x1, int y1, int x2, int y2, BYTE col)
{
// lok�ln� prom�nn�
	int x,y;							// ukazatele bodu
	int d;								// pr�m�r kruhu
	double sx, sy;						// st�ed kruhu
	double dx,dy;						// vzd�lenost bodu od st�edu

// oprava po�ad� sou�adnic X
	if (x1 > x2)
	{
		x = x1;
		x1 = x2;
		x2 = x;
	}

// oprava po�ad� sou�adnic Y
	if (y1 > y2)
	{
		y = y1;
		y1 = y2;
		y2 = y;
	}

// pr�m�r kruhu (v�etn� koncov�ch bod�)
	d = (int)(sqrt((double)((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1)))+0.4) + 1;

// st�ed kruhu (zat�m zaokrouhlen� na cel� body)
	sx = (int)((x1 + x2) / 2);		// sou�adnice st�edu X
	sy = (int)((y1 + y2) / 2);		// sou�adnice st�edu Y

// korekce st�edu pro sud� pr�m�r
	if ((d & 1) == 0)
	{
		sx += 0.5;
		sy += 0.5;
	}

// p��prava minim�ln�ch a maxim�ln�ch sou�adnic
	x1 = (int)sx - d/2 - 2;
	x2 = (int)sx + d/2 + 2;
	y1 = (int)sy - d/2 - 2;
	y2 = (int)sy + d/2 + 2;

// vykreslen� kruhu
	for (x = x1; x <= x2; x++)			// cyklus p�es v�echny pozice na ��dku
	{
		for (y = y1; y <= y2; y++)		// cyklus p�es v�echny ��dky
		{
			dx = fabs((double)x - sx)*2;		// vzd�lenost od st�edu ve sm�ru X
			dy = fabs((double)y - sy)*2;		// vzd�lenost od st�edu ve sm�ru Y

			if (dx*dx + dy*dy <= (double)d*d)	// le�� bod uvnit� kruhu ?
			{
				SetPoint(x,y,col);		// nastaven� bodu kruhu
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// kreslen� elipsy

void SetElip(int x1, int y1, int x2, int y2, BYTE col)
{
// lok�ln� prom�nn�
	int sx, sy;							// st�ed elipsy
	int dx,dy;							// vzd�lenost bodu od st�edu
	double rx, ry;						// polom�r ve sm�ru X a Y

// st�ed elipsy
	sx = (x1 + x2)/2;					// sou�adnice st�edu X
	sy = (y1 + y2)/2;					// sou�adnice st�edu Y

// polom�r elipsy
	rx = (double)(abs(x2 - x1) / 2);
	ry = (double)(abs(y2 - y1) / 2);
	if (rx < 0.01) rx = 0.01;
	if (ry < 0.01) ry = 0.01;

	int kx = abs(x2 - x1) % 2;
	int ky = abs(y2 - y1) % 2;

// cyklus p�es v�echny horizont�ln� body
	for (dx = (int)rx; dx >= 0; dx--)
	{
		dy = (int)(sqrt(1 - (double)dx*dx/rx/rx)*ry + 0.5);

		SetPaint(sx - dx		, sy + dy + ky	, col);
		SetPaint(sx + dx + kx	, sy + dy + ky	, col);
		SetPaint(sx - dx		, sy - dy		, col);
		SetPaint(sx + dx + kx	, sy - dy		, col);
	}

// cyklus p�es v�echny vertik�ln� body
	for (dy = (int)ry; dy >= 0; dy--)
	{
		dx = (int)(sqrt(1 - (double)dy*dy/ry/ry)*rx + 0.5);

		SetPaint(sx - dx		, sy + dy + ky	, col);
		SetPaint(sx + dx + kx	, sy + dy + ky	, col);
		SetPaint(sx - dx		, sy - dy		, col);
		SetPaint(sx + dx + kx	, sy - dy		, col);
	}
}


/////////////////////////////////////////////////////////////////////////////
// kreslen� ov�lu

void SetElipFill(int x1, int y1, int x2, int y2, BYTE col)
{
// lok�ln� prom�nn�
	int x,y;							// ukazatele bodu
	double sx, sy;						// st�ed elipsy
	double dx,dy;						// vzd�lenost bodu od st�edu
	double rx, ry;						// polom�r ve sm�ru X a Y

// oprava po�ad� sou�adnic X
	if (x1 > x2)
	{
		x = x1;
		x1 = x2;
		x2 = x;
	}

// oprava po�ad� sou�adnic Y
	if (y1 > y2)
	{
		y = y1;
		y1 = y2;
		y2 = y;
	}

// polom�r elipsy
	rx = (double)(x2 + 1 - x1) / 2;
	ry = (double)(y2 + 1 - y1) / 2;
	rx *= rx;
	ry *= ry;

// st�ed elipsy
	sx = (double)(x1 + x2) / 2;			// sou�adnice st�edu X
	sy = (double)(y1 + y2) / 2;			// sou�adnice st�edu Y

// vykreslen� elipsy
	for (x = x1; x <= x2; x++)
	{
		for (y = y1; y <= y2; y++)
		{
			dx = fabs((double)x - sx);			// vzd�lenost X od st�edu
			dy = fabs((double)y - sy);			// vzd�lenost Y od st�edu

			if ((dy*dy/ry) <= (1 - dx*dx/rx))
			{
				SetPoint(x,y,col);
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// kreslen� koule

void SetKoule(int x1, int y1, int x2, int y2, BYTE col0)
{
// lok�ln� prom�nn�
	int i;								// pracovn� ��ta�
	int x,y;							// ukazatele bodu
	int d;								// pr�m�r koule
	double sx, sy;						// st�ed koule
	double dx,dy;						// vzd�lenost bodu od st�edu
	int dxcol2, dycol2;					// kvadr�ty vzd�lenosti pro barvu
	int sxcol, sycol;					// st�ed pro barvu

	BYTE col[20];						// buffer palet

// p��prava po�tu barev
	int cols;							// po�et barev
	if (col0 >= StdColors - 2*ColLev)
	{
		cols = StdColors - col0;
	}
	else
	{
		cols = (col0 - ResCols + ColLev) / ColLev * ColLev + ResCols - col0 + 1;
	}

// p��prava bufferu barev
	for (i = 0; i < cols-1; i++)
	{
		col[i] = (BYTE)(col0+i);
	}
	col[cols-1] = (BYTE)(StdColors - 1);

// oprava po�ad� sou�adnic X
	if (x1 > x2)
	{
		x = x1;
		x1 = x2;
		x2 = x;
	}

// oprava po�ad� sou�adnic Y
	if (y1 > y2)
	{
		y = y1;
		y1 = y2;
		y2 = y;
	}

// pr�m�r koule (v�etn� koncov�ch bod�)
	d = (int)(sqrt((double)((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1)))+0.4) + 1;

// st�ed koule (zat�m zaokrouhlen� na cel� body)
	sx = (int)((x1 + x2) / 2);		// sou�adnice st�edu X
	sy = (int)((y1 + y2) / 2);		// sou�adnice st�edu Y

// st�ed pro barvu
	sxcol = (int)(sx - d/4);
	sycol = (int)(sy - d/4);

// korekce st�edu pro sud� pr�m�r
	if ((d & 1) == 0)
	{
		sx += 0.5;
		sy += 0.5;
	}

// p��prava minim�ln�ch a maxim�ln�ch sou�adnic
	x1 = (int)sx - d/2 - 2;
	x2 = (int)sx + d/2 + 2;
	y1 = (int)sy - d/2 - 2;
	y2 = (int)sy + d/2 + 2;

// vykreslen� koule
	for (x = x1; x <= x2; x++)			// cyklus p�es v�echny pozice na ��dku
	{
		for (y = y1; y <= y2; y++)		// cyklus p�es v�echny ��dky
		{
			dx = fabs((double)x - sx)*2;		// vzd�lenost od st�edu ve sm�ru X
			dy = fabs((double)y - sy)*2;		// vzd�lenost od st�edu ve sm�ru Y

			dxcol2 = abs(x - sxcol) * 2;
			dxcol2 *= dxcol2;
			dycol2 = abs(y - sycol) * 2;
			dycol2 *= dycol2;

			if (dx*dx + dy*dy <= (double)d*d)	// le�� bod uvnit� kruhu ?
			{
				i = (int)(cols * 2.2 * sqrt((double)(dxcol2 + dycol2)) /d);

				switch(i & 3)
				{
				case 0:
					i = i/4;
					break;

				case 1:
					i = i/4 + (x & y &1);
					break;

				case 2:
					i = i/4 + ((x + y) & 1);
					break;

				default:
					i = i/4 + ((x | y) & 1);
				}

				if (i >= cols) i = cols - 1;

				SetPoint(x,y,col[i]);		// nastaven� bodu kruhu
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// v�pl� (col = nov� barva, fil = podklad)

void SetFill(int x, int y, BYTE col, BYTE fil)
{
	int min = x;
	int max = x;
	int i;

	SetPoint(x, y, col);

// v�pl� minim�ln�m sm�rem
	while ((min > 0) && (GetPoint(min-1,y) == fil))
	{
		min--;
		SetPoint(min, y, col);
	}

// v�pl� maxim�ln�m sm�rem
	while ((max < Width - 1) && (GetPoint(max+1,y) == fil))
	{
		max++;
		SetPoint(max, y, col);
	}

// v�pl� sm�rem nahoru
	if (y > 0)
	{
		y--;
		for (i = min; i <= max; i++)
		{
			if (GetPoint(i, y) == fil)
				SetFill(i,y,col,fil);
		}
		y++;
	}

// v�pl� sm�rem dol�
	if (y < Height - 1)
	{
		y++;
		for (i = min; i <= max; i++)
		{
			if (GetPoint(i, y) == fil)
				SetFill(i,y,col,fil);
		}
		y--;
	}
}


/////////////////////////////////////////////////////////////////////////////
// zobrazen� textu

void ReDispText()
{
// vypnut� ozna�en� bloku
	DispSelect();

// p��prava barvy
	BYTE col = ColLeft;							// barva p�sma
	BlokBack = 0;								// barva pozad�

// vytvo�en� bufferu MONO bitmapy
	int width0 = (Width + 31) & ~0x1f;			// zarovnan� ���ka (vy�aduje to GetDIBits)
	int widthbyte = width0/8;
	BYTE* buf = (BYTE*)MemGet(widthbyte * Height);
	MemFill(buf, widthbyte * Height, -1);

// vytvo�en� mono bitmapy
	HBITMAP bmp = ::CreateBitmap(width0, Height, 1, 1, buf);
	ASSERT(bmp);

// otev�en� DC displeje
	HDC dc = ::GetDC(MainFrame);
	ASSERT(dc);
	if (dc && bmp)
	{

// vytvo�en� kompatibiln�ho DC
		HDC dc2 = ::CreateCompatibleDC(dc);
		ASSERT(dc2);
		if (dc2)
		{

// v�b�r bitmapy do DC
			HBITMAP oldbmp = (HBITMAP)::SelectObject(dc2, bmp);

// vytvo�en� fontu
			HFONT font = ::CreateFont(FontSize, 0, 0, 0, 
				FontWeight, FontItalic, FALSE, FALSE,
				FontCharSet, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, FontName);

// v�b�r fontu
			HFONT oldfont = (HFONT)::SelectObject(dc2, font);

// v�pis textu
			::TextOut(dc2, 0, 0, FontText, FontText.Length());

// navr�cen� p�vodn�ho fontu
			::SelectObject(dc2, oldfont);

// zru�en� vytvo�en�ho fontu
			::DeleteObject(font);

// navr�cen� p�vodn� bitmapy
			::SelectObject(dc2, oldbmp);

// na�ten� dat zp�t do bufferu
			struct {BITMAPINFOHEADER bmiHeader; RGBQUAD bmiColors[2]; } bmpinfo;
			bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmpinfo.bmiHeader.biWidth = width0;
			bmpinfo.bmiHeader.biHeight = Height;
			bmpinfo.bmiHeader.biPlanes = 1;
			bmpinfo.bmiHeader.biBitCount = 1;
			bmpinfo.bmiHeader.biCompression = BI_RGB;
			bmpinfo.bmiHeader.biSizeImage = 0;
			bmpinfo.bmiHeader.biXPelsPerMeter = 0;
			bmpinfo.bmiHeader.biYPelsPerMeter = 0;
			bmpinfo.bmiHeader.biClrUsed = 0;
			bmpinfo.bmiHeader.biClrImportant = 0;
			::GetDIBits(dc2, bmp, 0, Height, buf, (BITMAPINFO*) &bmpinfo, DIB_RGB_COLORS);

// zji�t�n� v��ky textu
			BYTE* dst;
			int height = Height;
			int width = (Width + 7)/8 - 1;
			for (; height > (abs(FontSize)+4); height--)
			{
				dst = &buf[(Height - height) * widthbyte];
				if ((*dst != 0xff) || !MemCompare(dst, dst+1, width))
					break;
			}

			BlokHeight = height;

			char* src = (char*)&buf[widthbyte*(Height - height)];
			BlokWidth = Width;
			BlokBuf = (BYTE*)MemSize(BlokBuf, BlokWidth*BlokHeight * sizeof(BYTE));
			MemFill(BlokBuf, BlokWidth*BlokHeight, 0);

// p�enesen� dat do obr�zku
			dst = BlokBuf;
			char data;

			for (int i = BlokHeight; i > 0; i--)
			{
				int k = 8;
				char* src2 = src;
				data = *src2;
				src2++;

				for (int j = BlokWidth; j > 0; j--)
				{
					if (data >= 0)
					{
						*dst = col;
					}
					dst++;
					data <<= 1;

					k--;
					if (k == 0)
					{
						data = *src2;
						src2++;
						k = 8;
					}
				}

				src += widthbyte;
			}

// zru�en� pracovn�ho DC
			::DeleteDC(dc2);
		}

// uvoln�n� DC
		::ReleaseDC(MainFrame, dc);
	}

// zru�en� bitmapy
	::DeleteObject(bmp);

// zru�en� bufferu
	buf = (BYTE*)MemSize(buf, 0);

// korekce po��tku zobrazen� bloku
	if (BlokX > (DispLeft + DispWidth - 5)) BlokX = DispLeft + DispWidth - 5;
	if (BlokY > (Height - DispBottom - 5)) BlokY = Height - DispBottom - 5;
	if ((BlokX + BlokWidth) < (DispLeft + 5)) BlokX = DispLeft + 5 - BlokWidth;
	if ((BlokY + BlokHeight) < (Height - DispBottom - DispHeight + 5)) 
		BlokY = Height - DispBottom - DispHeight + 5 - BlokHeight;

// p�ekreslen� bloku
	MoveSelect();

// zapnut� ozna�en� bloku
	DispSelect();

// p�ekreslen� obrazovky
	Disp();
}


// ************************* obsluha bloku **********************************


/////////////////////////////////////////////////////////////////////////////
// na�ten� bodu z bloku (nekontroluje sou�adnice !)

inline BYTE _fastcall GetBlok(int x, int y)
{
	return BlokBuf[BlokWidth * (BlokHeight-1-y) + x];
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� bodu v bloku (nekontroluje sou�adnice !)

inline void _fastcall SetBlok(int x, int y, BYTE col)
{
	BlokBuf[BlokWidth * (BlokHeight-1-y) + x] = col;
}


/////////////////////////////////////////////////////////////////////////////
// zapnut�/vypnut� v�b�ru bloku

void DispSelect()
{
// kontrola, zda je blok zobrazen
	if (!Selecting) return;

// p��prava DC displeje
	HDC dc = ::GetDC(MainFrame);

// omezuj�c� oblast
	RECT clip;

	clip.left = DLeft - 1;
	if (clip.left < EditX) clip.left = EditX;

	clip.top = DTop;
	if (clip.top < EditY) clip.top = EditY;

	clip.right = DLeft + DWidth;
	if (clip.right >= EditX + EditWidth) clip.right = EditX + EditWidth;

	clip.bottom = DTop + DHeight + 1;
	if (clip.bottom >= EditY + EditHeight) clip.bottom = EditY + EditHeight;

// v�b�r �t�tce
	HBRUSH oldBrush = (HBRUSH)::SelectObject(dc, HalfToneBrush);

// ���ka ��ry
	int s = Zoom/2;
	if (s == 0) s++;

// p��prava sou�adnic obd�ln�ku
	int x1 = BlokX - DispLeft;
	int x2 = x1 + BlokWidth;
	int y1 = BlokY - (Height - DispBottom - DispHeight);
	int y2 = y1 + BlokHeight;

// v�po�et sou�adnic v bodech
	x1 = x1*Zoom + DLeft - 1;
	x2 = x2*Zoom + DLeft;
	y1 = y1*Zoom + DTop;
	y2 = y2*Zoom + DTop + 1;

// omezen� sou�adnic
	int x12 = x1; 
	if (x12 < clip.left) x12 = clip.left;

	int y12 = y1;
	if (y12 < clip.top) y12 = clip.top - s;

	int x22 = x2;
	if (x22 > clip.right) x22 = clip.right;

	int y22 = y2;
	if (y22 > clip.bottom) y22 = clip.bottom + s;

// kontrola sou�adnic
	if ((x12 < x22) && (y12 < y22))
	{

// horn� linka
		if (y1 >= clip.top)
		{
			::PatBlt(dc, x12, y12, x22-x12, s, PATINVERT);
		}

// spodn� linka
		if (y2 <= clip.bottom)
		{
			::PatBlt(dc, x12, y22-s, x22-x12, s, PATINVERT);
		}

// lev� linka
		if (x1 >= clip.left)
		{
			::PatBlt(dc, x12, y12+s, s, y22-y12-2*s, PATINVERT);
		}

// prav� linka
		if (x2 <= clip.right)
		{
			::PatBlt(dc, x22-s, y12+s, s, y22-y12-2*s, PATINVERT);
		}
	}

// navr�cen� p�vodn�ho �t�tce
	::SelectObject(dc, oldBrush);

// uvoln�n� DC displeje
	::ReleaseDC(MainFrame, dc);
}


/////////////////////////////////////////////////////////////////////////////
// zobrazen� bloku p�i p�esunu

void MoveSelect()
{
// lok�ln� prom�nn�
	int x;
	int y;
	int width;
	int height;
	int x2;
	int y2;
	int i;
	BYTE col;
	BYTE back = BlokBack;

// n�vrat obsahu obr�zku
	Pop();

// sou�adnice k vymaz�n� pozad�
	x = FirstBlokX;
	y = FirstBlokY;
	width = FirstBlokWidth;
	height = FirstBlokHeight;

// test, zda se zobraz� podklad pod blokem
	if ((CutMode) && 
		(x < Width) && 
		(y < Height) && 
		(width > 0) && 
		(height > 0) &&
		((x + width) > 0) &&
		((y + height) > 0))
	{

// minim�ln� po��tek X
		if (x < 0)
		{
			width += x;
			x = 0;
		}

// minim�ln� po��tek Y
		if (y < 0)
		{
			height += y;
			y = 0;
		}

// maxim�ln� ���ka
		if ((x + width) > Width)
		{
			width = Width - x;
		}

// maxim�ln� v��ka
		if ((y + height) > Height)
		{
			height = Height - y;
		}

// vymaz�n� podkladu pod blokem
		for (; height > 0; height--)
		{
			MemFill(&Data[(Height - 1 - y)*Width + x], width, ColRight);
			y++;
		}
	}

// sou�adnice k zobrazen� bloku
	x = BlokX;
	y = BlokY;
	width = BlokWidth;
	height = BlokHeight;
	x2 = 0;
	y2 = 0;

// test, zda se zobraz� blok
	if ((x < Width) && 
		(y < Height) && 
		(width > 0) && 
		(height > 0) &&
		((x + width) > 0) &&
		((y + height) > 0))
	{

// minim�ln� po��tek X
		if (x < 0)
		{
			width += x;
			x2 = -x;
			x = 0;
		}

// minim�ln� po��tek Y
		if (y < 0)
		{
			height += y;
			y2 = -y;
			y = 0;
		}

// maxim�ln� ���ka
		if ((x + width) > Width)
		{
			width = Width - x;
		}

// maxim�ln� v��ka
		if ((y + height) > Height)
		{
			height = Height - y;
		}

// zobrazen� bloku
		for (; height > 0; height--)
		{
			for (i = 0; i < width; i++)
			{
				col = GetBlok(x2+i, y2);
				if (col != back)
				{
					Data[(Height-1-y)*Width+x+i] = col;
				}
			}
			y++;
			y2++;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// test bodu, zda je uvnit� vybran�ho bloku

BOOL TestBlok(int x, int y)
{
	return (
				(x >= BlokX) &&
				(x < BlokX + BlokWidth) &&
				(y >= BlokY) &&
				(y < BlokY + BlokHeight)
			);
}


/////////////////////////////////////////////////////////////////////////////
// zru�en� bloku

void Delete()
{
	if (Selecting)
	{
		DispSelect();			// vypnut� zobrazen� v�b�ru bloku
		BlokWidth = 0;			// neplatn� ���ka bloku
		BlokHeight = 0;			// neplatn� v��ka bloku
		MoveSelect();			// p�ekreslen� ikony
		Moving = FALSE;			// konec posunu bloku
		Selecting = FALSE;		// zru�en� p��znaku v�b�ru bloku
		Editing = FALSE;		// zru�en� p��znaku editace
		Disp();				// p�ekreslen� displeje
		SetModi();					// nastaven� p��znaku modifikace
		UpdateTree();			// p�ekreslen� ikon ve stromech
	}

// aktualizace voleb bloku
	UpdateClipboard();
}


/////////////////////////////////////////////////////////////////////////////
// otisk bloku

void OnOtisk()
{
// kontrola, zda je re�im editace
	if (ProgMode &&
		((EditMode == BufIcoID) || (EditMode == BufPicID)) &&
		Selecting)
	{
		PushUndo();
		Push();				// �schova do pomocn�ho bufferu
		CutMode = FALSE;

// p��znak modifikace souboru
		SetModi();									// nastaven� p��znaku modifikace

// aktualizace zobrazen� strom�
		UpdateTree();
	}
}


/////////////////////////////////////////////////////////////////////////////
// kopie bloku do schr�nky (FALSE = chyba)

BOOL Copy()
{
	if (BufID == BufPicID)
	{
		if (Index == -2)
		{
			Sprite[EditItemSprite][EditItemPic].DeComp();
			Data = Sprite[EditItemSprite][EditItemPic].DataData();
		}
		else
		{
			Picture[Index].DeComp();
			Data = Picture[Index].DataData();
		}
	}
	else
	{
		Icon[Index].DeComp();
		Data = Icon[Index].DataData();
	}


// lok�ln� prom�nn�
	BYTE*		src;			// adresa zdrojov�ch dat
	int			width;			// ���ka bitmapy v bodech
	int			height;			// v��ka bitmapy
	int			dstinc;			// p��rustek c�lov� adresy linky
	int			i;				// pracovn� ��ta�
	int			delka;			// d�lka dat v bufferu celkem
	HGLOBAL		global;			// glob�ln� buffer s daty
	BITMAPINFO* bitmap;			// ukazatel na data v glob�ln�m bufferu

// otev�en� schr�nky
	if (!::OpenClipboard(MainFrame))
		return FALSE;

// p��prava ukazatel� dat
	if (Selecting)
	{
		width = BlokWidth;		// ���ka bloku
		height = BlokHeight;	// v��ka bloku
		src = BlokBuf;			// data jsou v bufferu bloku
	}
	else
	{
		width = Width;
		height = Height;
		src = Data;
	}

// vypr�zdn�n� schr�nky
	if (!::EmptyClipboard())
	{
		::CloseClipboard();
		return FALSE;
	}

// p��prava velikosti dat
	dstinc = (width+3) & ~3; // p��rustek c�lov� adresy linky
	delka = dstinc * height + sizeof(BITMAPINFO) + sizeof(RGBQUAD)*255;

// vytvo�en� glob�ln�ho bufferu pro data
	global = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, delka);
	if (global == NULL)
	{
		::CloseClipboard();
		return FALSE;
	}

// uzamknut� bufferu
	bitmap = (BITMAPINFO*) ::GlobalLock(global);

// kopie z�hlav� bitmapy s paletami do bufferu
	MemCopy(bitmap, StdBitmapInfo, sizeof(BITMAPINFO) + sizeof(RGBQUAD)*255);

// kopie dat obr�zku
	for (i = 0; i < height; i++)
	{
		MemCopy(&((BYTE*)bitmap)[sizeof(BITMAPINFO) + sizeof(RGBQUAD)*255 
			+ i*dstinc], &src[i*width], width);
	}

// nastaven� parametr� obr�zku
	bitmap->bmiHeader.biWidth = width;
	bitmap->bmiHeader.biHeight = height;

// odemknut� bufferu
	::GlobalUnlock(global);

// ulo�en� dat do schr�nky
	::SetClipboardData(CF_DIB, global);

// uzav�en� schr�nky
	::CloseClipboard();

// aktualizace voleb bloku
	UpdateClipboard();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// vyst�ihnut� bloku do schr�nky

void Cut()
{
	if (Selecting && Copy())
	{
		Delete();
	}

// aktualizace voleb bloku
	UpdateClipboard();
}

/////////////////////////////////////////////////////////////////////////////
// n�vrat bloku ze schr�nky

void Paste()
{
// lok�ln� prom�nn�
	HGLOBAL		global;			// glob�ln� buffer s daty
	BITMAPINFO* bitmap;			// ukazatel na data v glob�ln�m bufferu
	int			width;			// ���ka bitmapy v bodech
	int			height;			// v��ka bitmapy
	int			palet;			// po�et palet v bitmap�
	int			srcinc;			// p��rustek zdrojov� adresy linky
	int			i, j;			// pracovn� ��ta�e
	BYTE*		src;			// ukazatel zdrojov� adresy
	BYTE*		dst;			// ukazatel c�lov� adresy
	WORD		srcdat;			// zdrojov� data 16 bit�
	BYTE		r,g,b;			// slo�ky barev

// otev�en� schr�nky
	if (!::OpenClipboard(MainFrame))
		return;

// na�ten� dat schr�nky
	global = ::GetClipboardData(CF_DIB);
	if (global == NULL)
	{
		::CloseClipboard();
		return;
	}
	
// uzamknut� bufferu
	bitmap = (BITMAPINFO*) ::GlobalLock(global);

// kontrola form�tu bitmapy
	if	(TestBMP(bitmap) && (bitmap->bmiHeader.biCompression == BI_RGB))
	{

// zapnut� m�du bloku
		SetMode(IDN_SELECT);

// vypnut� ozna�en� bloku
		if (Selecting)
		{
			DispSelect();			// vypnut� zobrazen� v�b�ru bloku
			Moving = FALSE;		// konec posunu bloku
			Selecting = FALSE;	// zru�en� p��znaku v�b�ru bloku
			Editing = FALSE;		// zru�en� p��znaku editace
		}

// �schova ikony do bufferu
		PushUndo();			// �schova ikony do undo bufferu
		Push();				// �schova ikony do bufferu

// p��prava parametr� bitmapy
		width = bitmap->bmiHeader.biWidth;	// ���ka
		height = bitmap->bmiHeader.biHeight; // v��ka

// vytvo�en� bufferu k ulo�en� dat
		BlokBuf = (BYTE*)MemSize(BlokBuf, width*height * sizeof(BYTE));				// vytvo�en� bufferu pro blok
		dst = BlokBuf;							// ukazatel c�lov� adresy
		src = (BYTE*)&bitmap->bmiColors[0];		// za��tek zdrojov�ch dat

// p��prava bufferu odchylky pro dithering
		int* odch = NULL;
		if (Dither)
		{
			odch = (int*)MemGet((3*width + 6) * sizeof(int));
			MemFill(odch, (3*width + 6) * sizeof(int), 0);
		}

// rozli�en� podle po�tu bod�
		switch (bitmap->bmiHeader.biBitCount)
		{

// 1 bit
		case 1:
			srcinc = ((width+7)/8 + 3) & ~3;	// p��rustek zdrojov� adresy

// p��prava konverzn� tabulky palet
			GenKonvPal(bitmap);					// p��prava konverzn� tabulky

// p��prava po�tu palet
			palet = bitmap->bmiHeader.biClrUsed; // po�et palet v tabulce
			if (palet == 0) palet = 2;			// pou��v� se maxim�ln� po�et palet
			src += sizeof(RGBQUAD) * palet;		// korekce zdrojov� adresy

// kopie dat do bufferu bloku
			for (i = height-1; i >= 0; i--)		// cyklus p�es v�echny linky
			{
				for (j = 0; j < width; j++)
				{
					*dst = KonvPal[(src[j/8] >> (7 - (j & 7))) & 1];
					dst++;
				}
				src += srcinc;					// zv��en� zdrojov� adresy
			}
			break;

// 4 bity
		case 4:
			srcinc = ((width+1)/2 + 3) & ~3;	// p��rustek zdrojov� adresy

// p��prava konverzn� tabulky palet
			GenKonvPal(bitmap);					// p��prava konverzn� tabulky

// p��prava po�tu palet
			palet = bitmap->bmiHeader.biClrUsed; // po�et palet v tabulce
			if (palet == 0) palet = 16;			// pou��v� se maxim�ln� po�et palet
			src += sizeof(RGBQUAD) * palet;		// korekce zdrojov� adresy

// kopie dat do bufferu bloku
			for (i = height-1; i >= 0; i--)		// cyklus p�es v�echny linky
			{
				for (j = 0; j < width; j++)
				{
					if (j & 1)
					{
						*dst = KonvPal[src[j/2] & 0xF];
					}
					else
					{
						*dst = KonvPal[src[j/2] >> 4];
					}
					dst++;
				}
				src += srcinc;					// zv��en� zdrojov� adresy
			}
			break;

// 8 bit�
		case 8:
			srcinc = (width + 3) & ~3;			// p��rustek zdrojov� adresy

// p��prava konverzn� tabulky palet
			GenKonvPal(bitmap);					// p��prava konverzn� tabulky

// p��prava po�tu palet
			palet = bitmap->bmiHeader.biClrUsed; // po�et palet v tabulce
			if (palet == 0) palet = 256;		// pou��v� se maxim�ln� po�et palet
			src += sizeof(RGBQUAD) * palet;		// korekce zdrojov� adresy

// kopie dat do bufferu bloku
			for (i = height-1; i >= 0; i--)		// cyklus p�es v�echny linky
			{
				if (Dither)
				{
					int* odch0 = odch + 3;			// ukazatel v bufferu odchylek

					for (j = width; j > 0; j--)		// cyklus p�es v�echny body
					{
					// bod k z�pisu
						BYTE col = *src;
						src++;
						RGBQUAD* rgb = bitmap->bmiColors + col;

					// pozad�
						if ((*(int*)rgb & 0xffffff) == (BACKCOLOR_BLUE | (BACKCOLOR_GREEN*256) | (BACKCOLOR_RED*256*256)))
						{
							col = BackCol;

							*odch0 = 0;
							odch0++;
							*odch0 = 0;
							odch0++;
							*odch0 = 0;
							odch0++;
						}
						else
						{	

				// st�n
						  if ((*(int*)rgb & 0xffffff) == (SHADCOLOR_BLUE | (SHADCOLOR_GREEN*256) | (SHADCOLOR_RED*256*256)))
						  {
							col = ShadCol;

							*odch0 = 0;
							odch0++;
							*odch0 = 0;
							odch0++;
							*odch0 = 0;
							odch0++;
						  }
						  else
						  {	

					// po�adovan� barva
							b = rgb->rgbBlue;			// modr� slo�ka
							g = rgb->rgbGreen;			// zelen� slo�ka
							r = rgb->rgbRed;			// �erven� slo�ka

					// zkorigovan� barva
							int b2 = b - (odch0[-3] + odch0[0] + odch0[3])*5/8;		// modr� slo�ka
							int g2 = g - (odch0[-2] + odch0[1] + odch0[4])*5/8;		// zelen� slo�ka
							int r2 = r - (odch0[-1] + odch0[2] + odch0[5])*5/8;		// �erven� slo�ka

					// omezen� p�ete�en� barvy
							if (b2 < 0) b2 = 0;
							if (b2 > 255) b2 = 255;
							if (g2 < 0) g2 = 0;
							if (g2 > 255) g2 = 255;
							if (r2 < 0) r2 = 0;
							if (r2 > 255) r2 = 255;

					// import barvy
							col = PalImport((BYTE)r2, (BYTE)g2, (BYTE)b2);

					// odchylka aktu�ln�ho bodu od po�adovan� barvy
							rgb = StdBitmapInfo->bmiColors + col;
							*odch0 = rgb->rgbBlue - b;
							odch0++;

							*odch0 = rgb->rgbGreen - g;
							odch0++;

							*odch0 = rgb->rgbRed - r;
							odch0++;
						  }
						}

					// ulo�en� bodu
						*dst = col;
						dst++;						// zv��en� c�lov� adresy
					}
					src -= width;
				}
				else
				{
					KonvCopy(dst, src, width);
					dst += width;					// zv��en� c�lov� adresy
				}
				src += srcinc;					// zv��en� zdrojov� adresy
			}
			break;

// 16 bit�
		case 16:
			srcinc = ((2*width + 3) & ~3) - 2*width;

			for (i = height; i > 0; i--)	// cyklus p�es v�echny linky
			{
				if (Dither)
				{
					int* odch0 = odch + 3;			// ukazatel v bufferu odchylek

					for (j = width; j > 0; j--)		// cyklus p�es v�echny body na lince
					{

					// po�adovan� barva
						srcdat = *(WORD*)src;		// data jednoho bodu
						b = (BYTE)(srcdat & 0x1F);	// modr� slo�ka
						b = (BYTE)(b*8 + b/4);
						srcdat >>= 5;				// zru�en� bit� modr� slo�ky
						g = (BYTE)(srcdat & 0x1F);	// zelen� slo�ka
						g = (BYTE)(g*8 + g/4);
						srcdat >>= 5;				// zru�en� bit� zelen� slo�ky
						r = (BYTE)(srcdat & 0x1F);	// �erven� slo�ka
						r = (BYTE)(r*8 + r/4);
						src++;						// zv��en� zdrojov� adresy
						src++;						// zv��en� zdrojov� adresy

					// zkorigovan� barva
						int b2 = b - (odch0[-3] + odch0[0] + odch0[3])*5/8;		// modr� slo�ka
						int g2 = g - (odch0[-2] + odch0[1] + odch0[4])*5/8;		// zelen� slo�ka
						int r2 = r - (odch0[-1] + odch0[2] + odch0[5])*5/8;		// �erven� slo�ka

					// omezen� p�ete�en� barvy
						if (b2 < 0) b2 = 0;
						if (b2 > 255) b2 = 255;
						if (g2 < 0) g2 = 0;
						if (g2 > 255) g2 = 255;
						if (r2 < 0) r2 = 0;
						if (r2 > 255) r2 = 255;

					// import barvy
						BYTE col = PalImport((BYTE)r2, (BYTE)g2, (BYTE)b2);

					// odchylka aktu�ln�ho bodu od po�adovan� barvy
						RGBQUAD* rgb = StdBitmapInfo->bmiColors + col;
						*odch0 = rgb->rgbBlue - b;
						odch0++;

						*odch0 = rgb->rgbGreen - g;
						odch0++;

						*odch0 = rgb->rgbRed - r;
						odch0++;

					// ulo�en� bodu
						*dst = col;	// import barvy do vlastn�ch palet
						dst++;						// zv��en� c�lov� adresy
					}
				}
				else
				{
					for (j = width; j > 0; j--)		// cyklus p�es v�echny body
					{
						srcdat = *(WORD*)src;		// data jednoho bodu
						b = (BYTE)(srcdat & 0x1F);	// modr� slo�ka
						b = (BYTE)(b*8 + b/4);
						srcdat >>= 5;				// zru�en� bit� modr� slo�ky
						g = (BYTE)(srcdat & 0x1F);	// zelen� slo�ka
						g = (BYTE)(g*8 + g/4);
						srcdat >>= 5;				// zru�en� bit� zelen� slo�ky
						r = (BYTE)(srcdat & 0x1F);	// �erven� slo�ka
						r = (BYTE)(r*8 + r/4);
						*dst = PalImport(r, g, b);	// import barvy do vlastn�ch palet
						src++;						// zv��en� zdrojov� adresy
						src++;
						dst++;						// zv��en� c�lov� adresy
					}
				}
				src += srcinc;
			}
			break;

// 24 bit�
		case 24:
			srcinc = ((3*width + 3) & ~3) - 3*width;
			for (i = height; i > 0; i--)	// cyklus p�es v�echny linky
			{
				if (Dither)
				{
					int* odch0 = odch + 3;			// ukazatel v bufferu odchylek

					for (j = width; j > 0; j--)		// cyklus p�es v�echny body na lince
					{

					// pozad�
						if ((*(int*)src & 0xffffff) == (BACKCOLOR_BLUE | (BACKCOLOR_GREEN*256) | (BACKCOLOR_RED*256*256)))
						{
							*dst = BackCol;
							src += 3;

							*odch0 = 0;
							odch0++;
							*odch0 = 0;
							odch0++;
							*odch0 = 0;
							odch0++;
						}
						else
						{	

				// st�n
						  if ((*(int*)src & 0xffffff) == (SHADCOLOR_BLUE | (SHADCOLOR_GREEN*256) | (SHADCOLOR_RED*256*256)))
						  {
							*dst = ShadCol;
							src += 3;

							*odch0 = 0;
							odch0++;
							*odch0 = 0;
							odch0++;
							*odch0 = 0;
							odch0++;
						  }
						  else
						  {	

					// po�adovan� barva
							b = *src;					// modr� slo�ka
							src++;						// zv��en� zdrojov� adresy
							g = *src;					// zelen� slo�ka
							src++;						// zv��en� zdrojov� adresy
							r = *src;					// �erven� slo�ka
							src++;						// zv��en� zdrojov� adresy

					// zkorigovan� barva
							int b2 = b - (odch0[-3] + odch0[0] + odch0[3])*5/8;		// modr� slo�ka
							int g2 = g - (odch0[-2] + odch0[1] + odch0[4])*5/8;		// zelen� slo�ka
							int r2 = r - (odch0[-1] + odch0[2] + odch0[5])*5/8;		// �erven� slo�ka

					// omezen� p�ete�en� barvy
							if (b2 < 0) b2 = 0;
							if (b2 > 255) b2 = 255;
							if (g2 < 0) g2 = 0;
							if (g2 > 255) g2 = 255;
							if (r2 < 0) r2 = 0;
							if (r2 > 255) r2 = 255;

					// import barvy
							BYTE col = PalImport((BYTE)r2, (BYTE)g2, (BYTE)b2);

					// odchylka aktu�ln�ho bodu od po�adovan� barvy
							RGBQUAD* rgb = StdBitmapInfo->bmiColors + col;
							*odch0 = rgb->rgbBlue - b;
							odch0++;

							*odch0 = rgb->rgbGreen - g;
							odch0++;

							*odch0 = rgb->rgbRed - r;
							odch0++;

					// ulo�en� bodu
							*dst = col;	// import barvy do vlastn�ch palet
						  }
						}
						dst++;						// zv��en� c�lov� adresy
					}
				}
				else
				{
					for (j = width; j > 0; j--)		// cyklus p�es v�echny body
					{
						if ((*(int*)src & 0xffffff) == (BACKCOLOR_BLUE | (BACKCOLOR_GREEN*256) | (BACKCOLOR_RED*256*256)))
						{
							*dst = BackCol;
							src += 3;
						}
						else
						{
				// st�n
						  if ((*(int*)src & 0xffffff) == (SHADCOLOR_BLUE | (SHADCOLOR_GREEN*256) | (SHADCOLOR_RED*256*256)))
						  {
							*dst = ShadCol;
							src += 3;
						  }
						  else
						  {	
							b = *src;					// modr� slo�ka
							src++;						// zv��en� zdrojov� adresy
							g = *src;					// zelen� slo�ka
							src++;						// zv��en� zdrojov� adresy
							r = *src;					// �erven� slo�ka
							src++;						// zv��en� zdrojov� adresy
							*dst = PalImport(r, g, b);	// import barvy do vlastn�ch palet
						  }
						}
						dst++;						// zv��en� c�lov� adresy
					}
				}
				src += srcinc;
			}
			break;

// 32 bit�
		case 32:
			for (i = height; i > 0; i--)		// cyklus p�es v�echny linky
			{
				if (Dither)
				{
					int* odch0 = odch + 3;			// ukazatel v bufferu odchylek

					for (j = width; j > 0; j--)		// cyklus p�es v�echny body na lince
					{

					// pozad�
						if ((*(int*)src & 0xffffff) == (BACKCOLOR_BLUE | (BACKCOLOR_GREEN*256) | (BACKCOLOR_RED*256*256)))
						{
							*dst = BackCol;
							src += 4;

							*odch0 = 0;
							odch0++;
							*odch0 = 0;
							odch0++;
							*odch0 = 0;
							odch0++;
						}
						else
						{
							
				// st�n
						  if ((*(int*)src & 0xffffff) == (SHADCOLOR_BLUE | (SHADCOLOR_GREEN*256) | (SHADCOLOR_RED*256*256)))
						  {
							*dst = ShadCol;
							src += 4;

							*odch0 = 0;
							odch0++;
							*odch0 = 0;
							odch0++;
							*odch0 = 0;
							odch0++;
						  }
						  else
						  {	

					// po�adovan� barva
							b = *src;					// modr� slo�ka
							src++;						// zv��en� zdrojov� adresy
							g = *src;					// zelen� slo�ka
							src++;						// zv��en� zdrojov� adresy
							r = *src;					// �erven� slo�ka
							src++;						// zv��en� zdrojov� adresy
							src++;						// zv��en� zdrojov� adresy

					// zkorigovan� barva
							int b2 = b - (odch0[-3] + odch0[0] + odch0[3])*5/8;		// modr� slo�ka
							int g2 = g - (odch0[-2] + odch0[1] + odch0[4])*5/8;		// zelen� slo�ka
							int r2 = r - (odch0[-1] + odch0[2] + odch0[5])*5/8;		// �erven� slo�ka

					// omezen� p�ete�en� barvy
							if (b2 < 0) b2 = 0;
							if (b2 > 255) b2 = 255;
							if (g2 < 0) g2 = 0;
							if (g2 > 255) g2 = 255;
							if (r2 < 0) r2 = 0;
							if (r2 > 255) r2 = 255;

					// import barvy
							BYTE col = PalImport((BYTE)r2, (BYTE)g2, (BYTE)b2);

					// odchylka aktu�ln�ho bodu od po�adovan� barvy
							RGBQUAD* rgb = StdBitmapInfo->bmiColors + col;
							*odch0 = rgb->rgbBlue - b;
							odch0++;

							*odch0 = rgb->rgbGreen - g;
							odch0++;

							*odch0 = rgb->rgbRed - r;
							odch0++;

					// ulo�en� bodu
							*dst = col;	// import barvy do vlastn�ch palet
						  }
						}
						dst++;						// zv��en� c�lov� adresy
					}
				}
				else
				{
					for (j = width; j > 0; j--)		// cyklus p�es v�echny body
					{
						if ((*(int*)src & 0xffffff) == (BACKCOLOR_BLUE | (BACKCOLOR_GREEN*256) | (BACKCOLOR_RED*256*256)))
						{
							*dst = BackCol;
							src += 4;
						}
						else
						{
				// st�n
						  if ((*(int*)src & 0xffffff) == (SHADCOLOR_BLUE | (SHADCOLOR_GREEN*256) | (SHADCOLOR_RED*256*256)))
						  {
							*dst = ShadCol;
							src += 4;
						  }
						  else
						  {	
							b = *src;					// modr� slo�ka
							src++;						// zv��en� zdrojov� adresy
							g = *src;					// zelen� slo�ka
							src++;						// zv��en� zdrojov� adresy
							r = *src;					// �erven� slo�ka
							src++;						// zv��en� zdrojov� adresy
							src++;						// zv��en� zdrojov� adresy
							*dst = PalImport(r, g, b);	// import barvy do vlastn�ch palet
						  }
						}
						dst++;						// zv��en� c�lov� adresy
					}
				}
			}
			break;
		}

// korekce po��tku k zobrazen� bloku
		if ((BlokX > (DispLeft+DispWidth-5)) ||
			(BlokY > (Height-DispBottom-5)) ||
			((BlokX + BlokWidth) < (DispLeft+5)) ||
			((BlokY + BlokHeight) < (Height - DispBottom - DispHeight + 5)))
		{
			BlokX = DispLeft + DispWidth/2 - width/2;
			BlokY = Height - DispBottom - DispHeight/2 - height/2;
		}
		BlokWidth = width;
		BlokHeight = height;

// zah�jen� ozna�ov�n� bloku
		BlokBack = ColRight;// pr�hledn� barva bloku
		CutMode = FALSE;		// nen� vymaz�n� bloku
		Selecting = TRUE;		// je m�d v�b�ru bloku
		Mode = IDN_SELECT; // zapnut� m�du editace
		MoveSelect();			// p�ekreslen� ikony
		DispSelect();			// vypnut� zobrazen� v�b�ru bloku
		Disp();				// p�ekreslen� displeje

// uvoln�n� bufferu odchylky pro dithering
		MemFree(odch);
	}

// odemknut� bufferu
	::GlobalUnlock(global);

// uzav�en� schr�nky
	::CloseClipboard();

// p��znak modifikace souboru
	SetModi();									// nastaven� p��znaku modifikace

// aktualizace zobrazen� strom�
	UpdateTree();

// aktualizace voleb bloku
	UpdateClipboard();
}


/////////////////////////////////////////////////////////////////////////////
// ozna�en� cel�ho bloku

void SelectAll()
{
// zapnut� m�du bloku
	SetMode(IDN_SELECT);

// nastaven� �daj� pro ozna�en� v�eho
	PushUndo();					// �schova obr�zku do undo bufferu
	Push();						// �schova do pomocn�ho bufferu
	Selecting = TRUE;			// p��znak ozna�en� bloku
	BlokX = 0;					// po��tek bloku X
	BlokY = 0;					// po��tek bloku Y
	BlokWidth = Width;			// ���ka bloku
	BlokHeight = Height;		// v��ka bloku
	CutMode = TRUE;				// jsou �daje k vymaz�n�
	FirstBlokX = 0;				// v�choz� po��tek bloku X
	FirstBlokY = 0;				// v�choz� po��tek bloku Y
	FirstBlokWidth = Width;		// v�choz� ���ka bloku
	FirstBlokHeight = Height;	// v�choz� v��ka bloku
	BlokBack = ColRight;		// pr�hledn� barva bloku

// zobrazen� ozna�en� bloku
	DispSelect();				// zapnut� ozna�en� bloku

// �schova dat do bufferu
	BlokBuf = (BYTE*)MemSize(BlokBuf, Width * Height * sizeof(BYTE));	// buffer pro blok

	for (int x = 0; x < Width; x++)
	{
		for (int y = 0; y < Height; y++)
		{
			SetBlok(x, y, GetPoint(x, y));
		}
	}

// aktualizace voleb bloku
	UpdateClipboard();
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace voleb bloku

void UpdateClipboard()
{
	if (FocusBuf == BufEdiID)
	{
		EnableCommand(IDN_CUT, Selecting);
		EnableCommand(IDN_COPY, TRUE);
		EnableCommand(IDN_PASTE, ::IsClipboardFormatAvailable(CF_DIB));
		EnableCommand(IDN_CLEAR, Selecting);
		EnableCommand(IDN_ALL, TRUE);

		EnableCommand(IDN_OTISK, Selecting);
	}
}


// ****************************** obsluha UNDO ******************************


/////////////////////////////////////////////////////////////////////////////
// ulo�en� obr�zku do undo bufferu

void PushUndo()
{
	if (BufID == BufPicID)
	{
		if (Index == -2)
		{
			Undo.AddSprPicSet(EditItemSprite, EditItemPic, Sprite[EditItemSprite][EditItemPic]);
			Sprite[EditItemSprite].CopyWrite();
			Sprite[EditItemSprite][EditItemPic].CopyWrite();
			Sprite[EditItemSprite][EditItemPic].DeComp();
			Data = Sprite[EditItemSprite][EditItemPic].DataData();
		}
		else
		{
			Undo.AddPicSet(Index, Picture[Index]);
			Picture[Index].CopyWrite();
			Picture[Index].DeComp();
			Data = Picture[Index].DataData();
		}
	}
	else
	{
		Undo.AddIconSet(Index, Icon[Index]);
		Icon[Index].CopyWrite();
		Icon[Index].DeComp();
		Data = Icon[Index].DataData();
	}
	Undo.Stop();
}

}
