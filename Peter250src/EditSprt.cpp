
#include "Main.h"

/***************************************************************************\
*																			*
*								Editor sprajt�								*
*																			*
\***************************************************************************/

namespace EditSprite
{

/////////////////////////////////////////////////////////////////////////////
// parametry

// parametry editovan�ho sprajtu
int		Index = 0;						// editovan� sprajt
int		IndexPic = 0;					// index editovan�ho obr�zku
int		Width;							// ���ka plochy v pol��k�ch (bez okraj�)
int		Height;							// v��ka plochy v pol��k�ch (bez okraj�)
int		PicWidth;						// ���ka obr�zku v bodech
int		PicHeight;						// v��ka obr�zku v bodech
int		KorWidth;						// zkorigovan� ���ka obr�zku (minim�ln� ICONWIDTH)
int		KorHeight;						// zkorigovan� v��ka obr�zku (minim�ln� ICONHEIGHT)
int		MWidth;							// ���ka plochy v bodech + okraj vlevo
int		MHeight;						// v��ka plochy v bodech + okraj naho�e

// zobrazen� ��st plochy
int		TLeft;							// po��te�n� bod plochy vlevo
int		TTop;							// po��te�n� bod plochy naho�e

// zobrazen� plochy na displeji v�etn� z�hlav� (v bodech displeje)
int		DLeft;							// po��tek zobrazen� vlevo
int		DTop;							// po��tek zobrazen� naho�e
int		DWidth;							// zobrazen� ���ka ��sti plochy na displeji
int		DHeight;						// zobrazen� v��ka ��sti plochy na displeji

int		ILeft;							// index ikony vlevo (0 = lev� okraj)
int		IRight;							// index ikony vpravo (0 = lev� okraj)
int		ITop;							// index ikony naho�e (0 = horn� okraj)
int		IBottom;						// index ikony dole (0 = horn� okraj)

int		OLeft;							// zobrazen� ��st ikony vlevo
int		ORight;							// zobrazen� ��st ikony vpravo
int		OTop;							// zobrazen� ��st ikony naho�e
int		OBottom;						// zobrazen� ��st ikony dole

// nastaven� nov�ch rozm�r� a vlastnost�
int		NewDelay;						// nov� prodleva mezi f�zemi
double	NewStep;						// nov� po�et f�z� na krok
int		NewLevel;						// nov� hladina k zobrazen�
int		NewKlid;						// nov� po�et f�z� pro klid
int		NewWidth;						// nov� ���ka obr�zku
int		NewHeight;						// nov� v��ka obr�zku
int		NewPohyb;						// nov� po�et f�z� pro pohyb
int		NewSmer;						// nov� po�et sm�r�
BOOL	NewFill = FALSE;				// prob�h� pln�n� text�

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

CBufIcon	MapSwcNum;					// ikony ��sel
CBufIcon	SpriteIcon;					// ikony sprajt�

// testovac� m�d
BOOL	TestMode = FALSE;				// je testovac� m�d
int		TestLeft = 0;					// lev� okraj pro test
int		TestTop = 0;					// horn� okraj pro test
#define	TestWidthN 15					// po�et ikon horizont�ln�
#define	TestHeightN 11					// po�et ikon vertik�ln�
#define TestWidth (TestWidthN*ICONWIDTH)	// ���ka pro test
#define	TestHeight (TestHeightN*ICONHEIGHT)	// v��ka pro test
UINT	TestTimer = 0;					// �asova� pro test
BYTE*	TestBuff = NULL;				// buffer s v�stupn�m obr�zkem
#define	TestTimerID 1434l				// ID �asova�e

int		TestKlid;						// po�et klidov�ch f�z�
int		TestSmeru;						// po�et sm�r�
int		TestFazi;						// po�et f�z�
int		TestDelay;						// prodleva mezi f�zemi (v impulsech)
double	TestKroku;						// po�et krok� na jednotku

int		TestFaze = 0;					// aktu�ln� f�ze sprajtu
int		TestSmer = 0;					// aktu�ln� sm�r sprajtu
int		TestNext = 0;					// ��ta� do p��t�ho kroku

int		TestPicWidth;					// ���ka obr�zku sprajtu
int		TestPicHeight;					// v��ka obr�zku sprajtu
double	TestX;							// aktu�ln� sou�adnice X
double	TestY;							// aktu�ln� sou�adnice Y
double	TestEndX;						// c�lov� sou�adnice X
double	TestEndY;						// c�lov� sou�adnice Y

// ************************** obsluha volby editace *************************

/////////////////////////////////////////////////////////////////////////////
// inicializace p�i startu programu

void StartInit()
{
// na�ten� ikon ��sel
	MapSwcNum.IconSize(8, 12);
	MapSwcNum.Load(IDN_MAPSWCNUM, 10);
	ASSERT(MapSwcNum.Num() == 10);

// na�ten� pomocn�ch ikon sprajt� (z�hlav�)
	SpriteIcon.Load(IDN_SPRITE_ICON, 26);
	ASSERT(SpriteIcon.Num() == 26);

// nastaven� obr�zk� pro sprajty 0 a 1 (Petr a Petra) (maj� velikost ICONSIZE)
	CPicture* pic = Sprite0.DataData();
	int j = 2;
	for (int i = 0; i < 8; i++)
	{
		SpriteIcon[j  ].DeComp();
		SpriteIcon[j+1].DeComp();
		SpriteIcon[j+2].DeComp();
		pic->CopyData(SpriteIcon[j  ].DataData()); pic++;
		pic->CopyData(SpriteIcon[j+1].DataData()); pic++;
		pic->CopyData(SpriteIcon[j  ].DataData()); pic++;
		pic->CopyData(SpriteIcon[j+2].DataData()); pic++;
		pic->CopyData(SpriteIcon[j  ].DataData()); pic++;

		if (i == 3) pic = Sprite1.DataData();

		j += 3;
	}
}

/////////////////////////////////////////////////////////////////////////////
// zah�jen� editace (mus� b�t platn� index!)

void BegEdit(int index)
{
	EndDrag();

// �schova indexu sprajtu
	Index = index;						// index sprajtu
	ASSERT(Sprite.IsValid(index));
	Sprite[index].CopyWrite();			// kopie p�ed z�pisem

// �schova parametr� sprajtu
	Width = Sprite[index].Faze();		// ���ka plochy v pol��k�ch
	Height = Sprite[index].Smer();		// v��ka plochy v pol��k�ch
	PicWidth = Sprite[index].Width();	// ���ka obr�zku sprajtu
	KorWidth = PicWidth;				// zkorigovan� ���ka obr�zku sprajtu
	if (KorWidth < ICONWIDTH) KorWidth = ICONWIDTH;
	PicHeight = Sprite[index].Height();	// v��ka obr�zku sprajtu
	KorHeight = PicHeight;				// zkorigovan� v��ka sprajtu
	if (KorHeight < ICONHEIGHT) KorHeight = ICONHEIGHT;

// p�epo�et rozlo�en� a zobrazen� obr�zku
	Disp();
}

/////////////////////////////////////////////////////////////////////////////
// obsluha dialogu pro zad�n� vlastnost�

// nastaven� prodlevy
void DimenSetDelay(HWND wnd)
{
	CText txt;
	txt.Int(NewDelay);
	NewFill = TRUE;
	txt.SetDialogText(wnd, IDC_DELAY);
	NewFill = FALSE;
}

// na�ten� prodlevy
void DimenGetDelay(HWND wnd)
{
	if (!NewFill)
	{
		CText txt;
		txt.GetDialogText(wnd, IDC_DELAY);
		NewDelay = Int(txt);
		if (NewDelay < 55) NewDelay = 55;
		if (NewDelay > 4000000) NewDelay = 4000000;
		NewDelay = (NewDelay + 22) / 55 * 55;
		if (NewDelay > SPRITEMAXDELAY) NewDelay = SPRITEMAXDELAY;
	}
}

// nastaven� f�z� na krok
void DimenSetStep(HWND wnd)
{
	CText txt;
	txt.Double(NewStep);
	NewFill = TRUE;
	txt.SetDialogText(wnd, IDC_STEP);
	NewFill = FALSE;
}

// na�ten� f�z� na krok
void DimenGetStep(HWND wnd)
{
	if (!NewFill)
	{
		CText txt;
		txt.GetDialogText(wnd, IDC_STEP);
		NewStep = Double(txt);
		if (NewStep < 0)
		{
			NewStep = 0;
		}
		if (NewStep > SPRITEMAXKROKU)
		{
			NewStep = SPRITEMAXKROKU;
		}
	}
}

// nastaven� hladiny
void DimenSetLevel(HWND wnd)
{
	CText txt;
	txt.Int(NewLevel);
	NewFill = TRUE;
	txt.SetDialogText(wnd, IDC_LEVEL);
	NewFill = FALSE;
}

// na�ten� hladiny
void DimenGetLevel(HWND wnd)
{
	if (!NewFill)
	{
		CText txt;
		txt.GetDialogText(wnd, IDC_LEVEL);
		NewLevel = Int(txt);
	}
}

// nastaven� klidov�ch f�z�
void DimenSetKlid(HWND wnd)
{
	CText txt;
	txt.Int(NewKlid);
	NewFill = TRUE;
	txt.SetDialogText(wnd, IDC_KLID);
	NewFill = FALSE;
}

// na�ten� klidov�ch f�z�
void DimenGetKlid(HWND wnd)
{
	if (!NewFill)
	{
		CText txt;
		txt.GetDialogText(wnd, IDC_KLID);
		NewKlid = Int(txt);
		if (NewKlid < 1) NewKlid = 1;
		if (NewKlid > SPRITEMAXKLID) NewKlid = SPRITEMAXKLID;
	}
}

// nastaven� pohybov�ch f�z�
void DimenSetPohyb(HWND wnd)
{
	CText txt;
	txt.Int(NewPohyb);
	NewFill = TRUE;
	txt.SetDialogText(wnd, IDC_POHYB);
	NewFill = FALSE;
}

// na�ten� pohybov�ch f�z�
void DimenGetPohyb(HWND wnd)
{
	if (!NewFill)
	{
		CText txt;
		txt.GetDialogText(wnd, IDC_POHYB);
		NewPohyb = Int(txt);
		if (NewPohyb < 0) NewPohyb = 0;
		if (NewPohyb > SPRITEMAXPOHYB) NewPohyb = SPRITEMAXPOHYB;
	}
}

// nastaven� sm�r�
void DimenSetSmer(HWND wnd)
{
	CText txt;
	txt.Int(NewSmer);
	NewFill = TRUE;
	txt.SetDialogText(wnd, IDC_SMER);
	NewFill = FALSE;
}

// na�ten� sm�r�
void DimenGetSmer(HWND wnd)
{
	if (!NewFill)
	{
		CText txt;
		txt.GetDialogText(wnd, IDC_SMER);
		NewSmer = Int(txt);
		if (NewSmer < 1) NewSmer = 1;
		if (NewSmer > SPRITEMAXSMER) NewSmer = SPRITEMAXSMER;
	}
}

// nastaven� ���ky
void DimenSetWidth(HWND wnd)
{
	CText txt;
	txt.Double((double)NewWidth/ICONWIDTH);
	NewFill = TRUE;
	txt.SetDialogText(wnd, IDC_WIDTH);
	NewFill = FALSE;
}

// na�ten� ���ky
void DimenGetWidth(HWND wnd)
{
	if (!NewFill)
	{
		CText txt;
		txt.GetDialogText(wnd, IDC_WIDTH);
		NewWidth = (int)(Double(txt)*ICONWIDTH + 0.5);
		if (NewWidth <= 0) NewWidth = 1;
		if (NewWidth > 0x1000) NewWidth = 0x1000;
	}
}

// nastaven� v��ky
void DimenSetHeight(HWND wnd)
{
	CText txt;
	txt.Double((double)NewHeight/ICONHEIGHT);
	NewFill = TRUE;
	txt.SetDialogText(wnd, IDC_HEIGHT);
	NewFill = FALSE;
}

// na�ten� v��ky
void DimenGetHeight(HWND wnd)
{
	if (!NewFill)
	{
		CText txt;
		txt.GetDialogText(wnd, IDC_HEIGHT);
		NewHeight = (int)(Double(txt)*ICONHEIGHT + 0.5);
		if (NewHeight <= 0) NewHeight = 1;
		if (NewHeight > 0x1000) NewHeight = 0x1000;
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

			txt.Load(IDN_SPRITEDIMEN);
			txt.SetWindowTextFont(hWnd);

			DialogCustomText(hWnd, IDN_PRODLEVA_MEZI_FAZEMI);
			DialogCustomText(hWnd, IDN_MILISEKUND);
			DialogCustomText(hWnd, IDN_FAZI_NA_KROK);
			DialogCustomText(hWnd, IDN_HLADINA_K_ZOBRAZENI);
			DialogCustomText(hWnd, IDN_FAZI_PRO_KLID);
			DialogCustomText(hWnd, IDN_FAZI_PRO_POHYB);
			DialogCustomText(hWnd, IDN_SMERU);
			DialogCustomText(hWnd, IDN_SIRKA_OBRAZKU);
			DialogCustomText(hWnd, IDN_VYSKA_OBRAZKU);
			DialogCustomText(hWnd, IDN_KROKU);
			DialogCustomText(hWnd, IDN_BODU);

			txt.Load(IDN_CANCEL0);
			txt.SetDialogTextFont(hWnd, IDCANCEL);

			DimenSetDelay(hWnd);
			DimenSetStep(hWnd);
			DimenSetLevel(hWnd);
			DimenSetKlid(hWnd);
			DimenSetPohyb(hWnd);
			DimenSetSmer(hWnd);
			DimenSetWidth(hWnd);
			DimenSetHeight(hWnd);

			DialogCustomBox(hWnd, IDN_ANIMACE);
			DialogCustomBox(hWnd, IDN_POCET_OBRAZKU);
			DialogCustomBox(hWnd, IDN_ROZMER_OBRAZKU);
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
				DimenGetDelay(hWnd);
				DimenGetStep(hWnd);
				DimenGetLevel(hWnd);
				DimenGetKlid(hWnd);
				DimenGetPohyb(hWnd);
				DimenGetSmer(hWnd);
				DimenGetWidth(hWnd);
				DimenGetHeight(hWnd);
				::EndDialog(hWnd, LOWORD(wParam));
				return TRUE;
			}
			break;

		case EN_KILLFOCUS:
			switch (LOWORD(wParam))
			{
			case IDC_DELAY:
				DimenGetDelay(hWnd);
				DimenSetDelay(hWnd);
				break;

			case IDC_STEP:
				DimenGetStep(hWnd);
				DimenSetStep(hWnd);
				break;

			case IDC_LEVEL:
				DimenGetLevel(hWnd);
				DimenSetLevel(hWnd);
				break;

			case IDC_KLID:
				DimenGetKlid(hWnd);
				DimenSetKlid(hWnd);
				break;

			case IDC_POHYB:
				DimenGetPohyb(hWnd);
				DimenSetPohyb(hWnd);
				break;

			case IDC_SMER:
				DimenGetSmer(hWnd);
				DimenSetSmer(hWnd);
				break;

			case IDC_WIDTH:
				DimenGetWidth(hWnd);
				DimenSetWidth(hWnd);
				break;

			case IDC_HEIGHT:
				DimenGetHeight(hWnd);
				DimenSetHeight(hWnd);
				break;
			}
		}
		break;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� parametr� sprajtu

void OnDimen()
{
// p��prava v�choz�ch rozm�r�
	NewDelay = Sprite[Index].Delay();
	NewStep = Sprite[Index].Kroku();
	NewLevel = Sprite[Index].Level();
	NewKlid = Sprite[Index].Klid();
	NewPohyb = Width - NewKlid;
	NewSmer = Height;
	NewWidth = PicWidth;
	NewHeight = PicHeight;

// zad�n� nov�ch rozm�r� sprajtu
	if (::DialogBoxParam(
			hInstance,
			MAKEINTRESOURCE(IDN_SPRITEDIMEN),
			MainFrame,
			(DLGPROC)DimenDialogProc,
			(LPARAM)IDN_SPRITEDIMEN) != IDOK) return;

// kontrola, zda byly parametry zm�n�ny
	if ((NewDelay == Sprite[Index].Delay()) && 
		(NewStep == Sprite[Index].Kroku()) &&
		(NewLevel == Sprite[Index].Level()) &&
		(NewKlid == Sprite[Index].Klid()) &&
		(NewPohyb == Width - NewKlid) &&
		(NewWidth == PicWidth) &&
		(NewHeight == PicHeight) &&
		(NewSmer == Height))
	{
		return;
	}

// zapnut� kurzoru �ek�n�
	BeginWaitCursor();

// �schova pro UNDO
	PushUndo();

// nastaven� nov�ch parametr� sprajtu
	ASSERT(Sprite.IsValid(Index));
	if (((NewKlid + NewPohyb) < Width) || (NewSmer < Height))
	{
		HistEdit::Del(BufPicID, -2, Index, -1);
	}
	Sprite[Index].ReSize(NewKlid, NewPohyb, NewSmer);
	Sprite[Index].Delay(NewDelay);
	Sprite[Index].Kroku(NewStep);
	Sprite[Index].Level(NewLevel);

	Width = Sprite[Index].Faze();		// ���ka plochy v pol��k�ch
	Height = Sprite[Index].Smer();		// v��ka plochy v pol��k�ch

	for (int i = Width*Height-1; i >= 0; i--)
	{
		Sprite[Index][i].CopyWrite();
		Sprite[Index][i].Resize(NewWidth, NewHeight);
	}

	PicWidth = NewWidth;				// ���ka obr�zku sprajtu
	KorWidth = PicWidth;				// zkorigovan� ���ka obr�zku sprajtu
	if (KorWidth < ICONWIDTH) KorWidth = ICONWIDTH;
	PicHeight = NewHeight;				// v��ka obr�zku sprajtu
	KorHeight = PicHeight;				// zkorigovan� v��ka sprajtu
	if (KorHeight < ICONHEIGHT) KorHeight = ICONHEIGHT;

// p�ekreslen� displeje (a p�epo�et parametr�)
	Disp();

// p��znak modifikace
	SetModi();

// vypnut� kurzoru �ek�n�
	EndWaitCursor();
}



// ***************************** obsluha zobrazen� **************************

/////////////////////////////////////////////////////////////////////////////
// p�epo�et rozm�r�

void ReCalc()
{
// ���ka a v��ka plochy v bodech (v�etn� z�hlav�)
	MWidth = Width*KorWidth + ICONWIDTH;
	MHeight = Height*KorHeight + ICONHEIGHT;

// p��prava zobrazen� v��ky (korekce, je-li horizont�ln� posuvn�k)
	DHeight = EditHeight - 4;					// v��ka k zobrazen�
	if (EditWidth - 4 - TRACKSIRKA < MWidth)
	{
		DHeight -= TRACKSIRKA;					// sn�en� v��ky o horizont�ln� posuvn�k
		if (DHeight < 1) DHeight = 1;			// minim�ln� ���ka
	}

// p��prava zobrazen� ���ky (korekce, je-li vertik�ln� posuvn�k)
	DWidth = EditWidth - 4;						//	���ka k zobrazen�
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

// omezen� po��te�n�ho bodu
	if (TLeft < 0) TLeft = 0;
	if (TLeft > MWidth - DWidth) TLeft = MWidth - DWidth;
	if (TTop < 0) TTop = 0;
	if (TTop > MHeight - DHeight) TTop = MHeight - DHeight;

// p�epo�et zobrazen�ho po��tku na ikony a body
	if (TLeft < ICONWIDTH)
	{
		ILeft = 0;
		OLeft = ICONWIDTH - TLeft;				// zobrazen� ��st ikony vlevo
	}
	else
	{
		ILeft = (TLeft-ICONWIDTH) / KorWidth + 1;	// index ikony vlevo
		OLeft = KorWidth - ((TLeft-ICONWIDTH) % KorWidth);	// zobrazen� ��st ikony vlevo
	}

	if (TTop < ICONHEIGHT)
	{
		ITop = 0;
		OTop = ICONHEIGHT - TTop;
	}
	else
	{
		ITop = (TTop-ICONHEIGHT) / KorHeight + 1;	// index ikony naho�e
		OTop = KorHeight - ((TTop-ICONHEIGHT) % KorHeight);	// zobrazen� ��st ikony naho�e
	}

// zobrazen� ��st ikon vpravo a dole
	ORight = KorWidth - ((MWidth - TLeft - DWidth) % KorWidth);
	OBottom = KorHeight - ((MHeight - TTop - DHeight) % KorHeight);

// index ikon vpravo a dole
	IRight = ILeft + (DWidth - OLeft + KorWidth - 1) / KorWidth;
	IBottom = ITop + (DHeight - OTop + KorHeight - 1) / KorHeight;

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

// vykreslen� obr�zku
	DispAkt(dc);

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
	int width = ((IRight - ILeft + 1)*KorWidth + 7) & ~7;
	int height = (IBottom - ITop + 1)*KorHeight;
	BYTE* dst;
	BYTE* src;
	BYTE col;
	MapSwcNum[0].DeComp();
	BYTE numcol = (MapSwcNum[0].DataData())[0];		// barva podkladu ��slic
	int cislo;										// ��slo f�ze v z�hlav�
	int cislic;										// po�et ��slic ��sla v z�hlav�
	double uhel;									// sm�r �hlu
	double uhelsin;									// sinus �hlu
	double uhelcos;									// cosinus �hlu
	CPicture* data = Sprite[Index].DataData();

// vytvo�en� bufferu k zobrazen�
	BYTE* buf = (BYTE*)MemGet(width*height);

// zobrazen� ikon
	for (i = ILeft; i <= IRight; i++)
	{
		for (j = ITop; j <= IBottom; j++)
		{

// c�lov� adresa k ulo�en� ikony
			dst = buf + (i-ILeft)*KorWidth + (IBottom-j)*KorHeight*width;
			if ((i > 0) && (ILeft == 0)) dst += ICONWIDTH - KorWidth;

// je to horn� ��dek (z�hlav� s ��slem f�ze)?
			if (j == 0)
			{

// dek�dov�n� rohu z�hlav�
				if (i == 0)
				{
					SpriteIcon[1].DeComp();
					src = SpriteIcon[1].DataData();

					for (k = ICONHEIGHT; k > 0; k--)
					{
						MemCopy(dst, src, ICONWIDTH);
						dst += width;
						src += ICONWIDTH;
					}
				}

// jinak to bude ��slo f�ze
				else
				{

// vymaz�n� ikony pro ��slo f�ze
					col = OrangeCol;

					for (k = ICONHEIGHT; k > 0; k--)
					{
						MemFill(dst, KorWidth, col);
						dst += width;
					}
					dst -= width*(ICONHEIGHT/2 + 6);

// p��prava barvy a ��sla f�ze (s rozli�en�m f�ze pro klid)
					col = RedCol;
					cislo = i - 1;
					if (cislo >= Sprite[Index].Klid())
					{
						cislo -= Sprite[Index].Klid();
						col = BlackCol;
					}

// p��prava po�tu ��slic a ukl�daci adresy
					cislic = 1;
					if (cislo >= 10) cislic++;
					if (cislo >= 100) cislic++;
					if (cislo >= 1000) cislic++;
					if (cislo >= 10000) cislo = 9999;
					dst += KorWidth/2 - 8 + cislic*4;

// dek�dov�n� znak� ��slic
					for (; cislic > 0; cislic--)
					{
						MapSwcNum[cislo % 10].DeComp();
						src = MapSwcNum[cislo % 10].DataData();

						for (k = 12; k > 0; k--)
						{
							for (l = 8; l > 0; l--)
							{
								if (*src != numcol)
								{
									*dst = col;
								}
								src++;
								dst++;
							}
							dst += width - 8;
						}
						cislo /= 10;
						dst -= 12*width + 8;
					}
				}
			}

// nen� horn� ��dek
			else
			{

// je to z�hlav� vlevo (sm�r)?
				if (i == 0)
				{

// vymaz�n� ikony pro sm�r
					col = OrangeCol;

					for (k = KorHeight; k > 0; k--)
					{
						MemFill(dst, ICONWIDTH, col);
						dst += width;
					}
					dst -= width*((KorHeight+ICONHEIGHT)/2);

// dek�dov�n� podkladu z�hlav� pro sm�r
					SpriteIcon[0].DeComp();
					src = SpriteIcon[0].DataData();

					for (k = ICONHEIGHT; k > 0; k--)
					{
						MemCopy(dst, src, ICONWIDTH);
						dst += width;
						src += ICONWIDTH;
					}
					dst -= width*(ICONHEIGHT/2) - ICONWIDTH/2;

// p��prava �hlu sm�ru
					uhel = ((double)pi2 / Height) * (j - 1);
					uhelsin = sin(uhel);
					uhelcos = cos(uhel);

// dek�dov�n� sm�ru
					col = BlueCol;
					for (k = ICONWIDTH/2 - 3; k >= 0; k--)
					{
						l = (int)(uhelcos*k) + width*(int)(uhelsin*k);

						dst[l] = col;
						dst[l - 1] = col;
						dst[l - width] = col;
						dst[l - 1 - width] = col;
					}
				}

// jinak jsou platn� data
				else
				{

// vymaz�n� ikony pro obr�zek
					col = BackCol;

					for (k = KorHeight; k > 0; k--)
					{
						MemFill(dst, KorWidth, col);
						dst += width;
					}
					dst -= width*((KorHeight+PicHeight)/2) - (KorWidth-PicWidth)/2;

// dek�dov�n� platn� ikony
					data[(i-1) + (j-1)*Width].DeComp();
					src = data[(i-1) + (j-1)*Width].DataData();
	
					for (k = PicHeight; k > 0; k--)
					{
						MemCopy(dst, src, PicWidth);
						dst += width;
						src += PicWidth;
					}
				}
			}
		}
	}

// zobrazen� horizont�ln�ch �ar rastru
	col = BlackCol;
	dst = buf;
	for (i = IBottom - ITop; i >= 0; i--)
	{
		MemFill(dst, width, col);
		dst += KorHeight*width;
	}

	if (ITop == 0)
	{
		dst += (ICONHEIGHT-KorHeight-1)*width;
		MemFill(dst, width, col);
	}

// zobrazen� vertik�ln�ch �ar rastru
	for (i = IRight - ILeft + 1; i > 0; i--)
	{
		dst = buf + KorWidth*i - 1;
		if (ILeft == 0) dst += ICONWIDTH-KorWidth;

		if (i + ILeft - 1 == Sprite[Index].Klid())
		{
			col = RedCol;
		}

		for (j = height; j > 0; j--)
		{
			*dst = col;
			dst += width;
		}

		col = BlackCol;
	}

	if (ILeft == 0)
	{
		dst = buf;

		for (j = height; j > 0; j--)
		{
			*dst = col;
			dst += width;
		}
	}

// p�ednastaven� parametr� z�hlav� bitmapy
	StdBitmapInfo->bmiHeader.biWidth = width;
	StdBitmapInfo->bmiHeader.biHeight = DHeight;

// zobrazen�
	::StretchDIBits(dc, DLeft, DTop, DWidth, DHeight, 
		(ILeft == 0) ? (ICONWIDTH - OLeft) : (KorWidth - OLeft),
			0, DWidth, DHeight, buf + (KorHeight-OBottom)*width,
			StdBitmapInfo, DIB_RGB_COLORS, SRCCOPY);

// zru�en� bufferu
	MemFree(buf);
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
	if (si.nPos != TTop)
	{
		si.nPos = TTop;
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

	if (pos != TTop)
	{
		TTop = pos;
		Disp();
	}
}


// **************************** obsluha editace *****************************


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
// stisk tla��tka my�i (TRUE = obslou�eno)

BOOL OnButtonDown(UINT flags, int x, int y, BOOL right, BOOL dclick)
{
// ukon�en� ta�en�
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
	BegDrag = TRUE;
	CopyMode = FALSE;
	DragX = MouseX;
	DragY = MouseY;

// zah�jen� editace p�i dvojkliku
	if (dclick)
	{
		IndexPic = MouseX + MouseY * Width;
		Sprite[Index][IndexPic].CopyWrite();
		if (SetEditMode(BufPicID, -2))
		{
			ProgSetMenu();
			SetToolMode(BufPicID);
			SetFocusEdit();
			EditIcon::UpdateMenu();
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
		x -= HotSpotX;	x += KorWidth/2;
		y -= HotSpotY;	y += KorHeight/2;
	}

	if (Draging)
	{
		x -= DragHotSpotX;	x += DragIconWidth/2;
		y -= DragHotSpotY;	y += DragIconHeight/2;
	}

// p��prava relativn�ch sou�adnic bodu v edita�n�m poli
	int	sx = (x - DLeft - OLeft + KorWidth)/KorWidth + ILeft - 1;
	int sy = (y - DTop - OTop + KorHeight)/KorHeight + ITop - 1;

// sou�adnice my�i pro zobrazen�
	if ((x >= DLeft) && (y >= DTop) && 
		((DWORD)sx < (DWORD)Width) && ((DWORD)sy < (DWORD)Height) &&
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

// zah�jen� ta�en�
	if ((flags & (MK_LBUTTON | MK_RBUTTON)) &&
		!MDraging &&
		BegDrag &&
		(DragX != MOUSEINV) &&
		(DragY != MOUSEINV) &&
		(abs(x - BegX) + abs(y - BegY) >= 5))
	{
		MDraging = TRUE;

// bod styku pro ta�en�
		HotSpotX = BegX - DLeft - OLeft - (DragX-ILeft)*KorWidth;
		HotSpotY = BegY - DTop - OTop - (DragY-ITop)*KorHeight;

// vygenerov�n� ta�en� ikony
		DragImageList = (Sprite[Index].DataData())[DragX + DragY*Width].GenerDrag(EmptyText);

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
	if (MDraging)
	{
		if ((DragX != MouseX) || (DragY != MouseY))
		{
			PushUndo();

			CPicture* data = Sprite[Index].DataData();
			CPicture* olddata = data + (DragX + DragY*Width);
//			CPicture olditem(PicWidth, PicHeight);
			CPicture olditem;
			olditem.New(PicWidth, PicHeight);
			olditem.Clear();
	
			if ((MouseX != MOUSEINV) && (MouseY != MOUSEINV))
			{
				CPicture* newdata = data + (MouseX + MouseY*Width);

				olditem = *newdata;

				*newdata = *olddata;
			}

			if (!CopyMode)
			{
				*olddata = olditem;
			}

			SetModi();
		}

		EndDrag();
		Disp();
	}

	BegDrag = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// polo�en� obr�zku zvn�j�ku

void DragDrop(int pic)
{
// ukon�en� editace ��seln�ho parametru
	if ((EditMode != BufSprID) ||
		Sprite.IsNotValid(Index) ||
		Picture.IsNotValid(pic) ||
		((DWORD)MouseX >= (DWORD)Width) ||
		((DWORD)MouseY >= (DWORD)Height))
	{
		return;
	}

// �schova pro UNDO
	PushUndo();

	(Sprite[Index].DataData())[MouseX + MouseY*Width] = Picture[pic];
	(Sprite[Index].DataData())[MouseX + MouseY*Width].Resize(PicWidth, PicHeight);

	SetModi();
	EndDrag();
	Disp();
}


/////////////////////////////////////////////////////////////////////////////
// test

void TestPaint(HWND hWnd)
{
	int i, j;

// vymaz�n� bufferu ikonami pozad�
	BYTE* src;
	ASSERT(IconFnc.IsValid(0));
	if (Icon.IsValid(0))
	{
		Icon[0].DeComp();
		src = Icon[0].DataData();
	}
	else
	{
		IconFnc[0].DeComp();
		src = IconFnc[0].DataData();
	}
	BYTE* dst = TestBuff;

	for (i = ICONHEIGHT; i > 0; i--)
	{
		MemCopy(dst, src, ICONWIDTH);
		src += ICONWIDTH;
		MemCopy(dst + ICONWIDTH, dst, TestWidth - ICONWIDTH);
		dst += TestWidth;
	}

	MemCopy(dst, TestBuff, TestWidth * (TestHeight - ICONHEIGHT));

// p��prava f�ze sprajtu
	int faze = TestFaze;

	if (((TestX != TestEndX) || (TestY != TestEndY)) && 
		(TestFazi > TestKlid))
	{
		faze += TestKlid;
		if (faze >= TestFazi)
		{
			faze = TestKlid;
			TestFaze = 0;
		}
	}
	else
	{
		if (faze >= TestKlid)
		{
			faze = 0;
			TestFaze = 0;
		}
	}

// sou�adnice a rozm�ry k zobrazen� sprajtu
	int x = Round(TestX);
	int y = Round(TestY);
	int width = TestPicWidth;
	int height = TestPicHeight;

// zdrojov� adresa sprajtu
	Sprite[Index].At(faze, TestSmer).DeComp();
	src = Sprite[Index].At(faze, TestSmer).DataData();

// omezen� sou�adnic a rozm�r�
	if (x < 0)
	{
		width += x;
		src -= x;
		x = 0;
	}

	if (y < 0)
	{
		height += y;
		src -= y*TestPicWidth;
		y = 0;
	}

	if (x + width > TestWidth) width = TestWidth - x;

	if (y + height > TestHeight) height = TestHeight - y;

// c�lov� adresa k dek�dov�n� sprajtu
	dst = TestBuff + x + y*TestWidth;

// dek�dov�n� sprajtu
	if ((width > 0) && (height > 0))
	{
		for (i = height; i > 0; i--)
		{
			for (j = width; j > 0; j--)
			{
				if (*src != BackCol)
				{
					if (*src == ShadCol)
					{
						int c = *dst + 1;
						if ((c < ResCols) || (((c - ResCols) % ColLev) == 0))
						{
							c = BlackCol;
						}
						*dst = (BYTE)c;
					}
					else
					{
						*dst = *src;
					}
				}
				dst++;
				src++;
			}
			dst += TestWidth - width;
			src += TestPicWidth - width;
		}
	}

// otev�en� DC
	HDC dc = ::GetDC(hWnd);

// nastaven� vlastn�ch palet
	HPALETTE OldPal;					// �schova p�vodn�ch palet
	OldPal = ::SelectPalette(dc,		// v�b�r vlastn�ch palet
		StdPalette, FALSE);
	::RealizePalette(dc);				// realizace palet

// p�ednastaven� parametr� z�hlav� bitmapy
	StdBitmapInfo->bmiHeader.biWidth = TestWidth;
	StdBitmapInfo->bmiHeader.biHeight = TestHeight;

// zobrazen� obr�zku
	::StretchDIBits(dc, TestLeft, TestTop, TestWidth, TestHeight, 0,
			0, TestWidth, TestHeight, TestBuff,
			StdBitmapInfo, DIB_RGB_COLORS, SRCCOPY);

// n�vrat p�vodn�ch palet
	::SelectPalette(dc,OldPal,FALSE);

// uvoln�n� DC
	::ReleaseDC(hWnd, dc);

// posun sou�adnic
	TestNext--;
	if (TestNext <= 0)
	{
		TestFaze++;
		TestNext = TestDelay;

		if ((TestEndX != TestX) || (TestEndY != TestY))
		{
			double dx = TestEndX - TestX;
			double dy = TestEndY - TestY;
			double uhel = atan2(dy, dx) + pi2;
			double delka = sqrt(dx*dx +	dy*dy);
			int kroku = Round(delka * TestKroku / ICONWIDTH);
			if (kroku < 1) kroku = 1;
			double delka1 = delka / kroku;
			if (delka < delka1 + 0.01)
			{
				TestX = TestEndX;
				TestY = TestEndY;
			}
			else
			{
				TestX += cos(uhel) * delka1;
				TestY += sin(uhel) * delka1;
			}
		}
	}
}


// callback funkce dialogu
BOOL CALLBACK SpriteDialogProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
	case WM_INITDIALOG:
		{
			::MoveWindow(hWnd, 0, 0, TestWidth + 11, TestHeight + 70, FALSE);
			CenterWindow(hWnd, MainFrame);

			CText txt;

			txt.Load(IDN_SPRITETEST);
			txt.SetWindowTextFont(hWnd);

			txt.Load(IDN_CANCEL0);
			txt.SetDialogTextFont(hWnd, IDCANCEL);

			RECT rcClient;
			::GetClientRect(hWnd, &rcClient);
			TestLeft = (rcClient.right - TestWidth)/2;
			TestTop = 3;

			HWND item = ::GetDlgItem(hWnd, IDCANCEL);
			if (item)
			{
				RECT rcItem;
				::GetWindowRect(item, &rcItem);

				int itemWidth = rcItem.right - rcItem.left;
				int itemHeight = rcItem.bottom - rcItem.top;

				::MoveWindow(item, (rcClient.right - itemWidth)/2,
					rcClient.bottom - itemHeight - 8,
					itemWidth, itemHeight, TRUE);
			}

			TestTimer = ::SetTimer(hWnd, TestTimerID, TimerConst, NULL);
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
			case IDCANCEL:
				::KillTimer(hWnd, TestTimer);
				::EndDialog(hWnd, LOWORD(wParam));
				return TRUE;
			}
			break;
		}
		break;

	case WM_RBUTTONDBLCLK:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
		{
			TestEndX = (int)LOWORD(lParam) - TestLeft - TestPicWidth/2;
			TestEndY = TestHeight - (int)HIWORD(lParam) + TestTop - TestPicHeight/2;

			if ((TestEndX != TestX) || (TestEndY != TestY))
			{
				double uhel = atan2(TestEndY - TestY, TestEndX - TestX) + pi2;
				TestSmer = Round(uhel * TestSmeru / pi2);
				while (TestSmer >= TestSmeru) TestSmer -= TestSmeru;
				while (TestSmer < 0) TestSmer += TestSmeru;
				TestNext = 1;
			}
		}
		break;

	case WM_TIMER:
		TestPaint(hWnd);
		break;
	}
	return FALSE;
}

void OnTest()
{
// kontrola, zda je re�im editace
	if (!ProgMode ||
		(EditMode != BufSprID) ||
		Sprite.IsNotValid(Index) ||
		TestMode)
	{
		return;
	}

// vytvo�en� bufferu
	TestBuff = (BYTE*) MemGet(TestWidth * TestHeight);

// zah�jen� testovac�ho m�du
	TestPicWidth = Sprite[Index].Width();
	TestPicHeight = Sprite[Index].Height();
	TestX = (TestWidth - TestPicWidth)/2;
	TestEndX = TestX;
	TestY = (TestHeight - TestPicHeight)/2;
	TestEndY = TestY;
	TestFaze = 0;
	TestSmer = 0;
	TestFazi = Sprite[Index].Faze();
	TestKlid = Sprite[Index].Klid();
	TestSmeru = Sprite[Index].Smer();
	TestDelay = (Sprite[Index].Delay() + 22)/55;
	if (TestDelay < 1) TestDelay = 1;
	TestKroku = Sprite[Index].Kroku();

	TestMode = TRUE;

// test sprajtu
	::DialogBoxParam(
			hInstance,
			MAKEINTRESOURCE(IDN_SPRITETEST),
			MainFrame,
			(DLGPROC)SpriteDialogProc,
			(LPARAM)IDN_SPRITETEST);

// ukon�en� testovac�ho m�du
	TestMode = FALSE;

// zru�en� bufferu
	MemFree(TestBuff);
	TestBuff = NULL;
}

// ****************************** obsluha UNDO ******************************


/////////////////////////////////////////////////////////////////////////////
// �schova plochu pro UNDO

void PushUndo()
{
	Undo.AddSprSet(Index, Sprite[Index]);
	Sprite[Index].CopyWrite();
	Undo.Stop();
}


}
