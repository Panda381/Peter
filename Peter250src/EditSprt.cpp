
#include "Main.h"

/***************************************************************************\
*																			*
*								Editor sprajtù								*
*																			*
\***************************************************************************/

namespace EditSprite
{

/////////////////////////////////////////////////////////////////////////////
// parametry

// parametry editovaného sprajtu
int		Index = 0;						// editovaný sprajt
int		IndexPic = 0;					// index editovaného obrázku
int		Width;							// šíøka plochy v políèkách (bez okrajù)
int		Height;							// výška plochy v políèkách (bez okrajù)
int		PicWidth;						// šíøka obrázku v bodech
int		PicHeight;						// výška obrázku v bodech
int		KorWidth;						// zkorigovaná šíøka obrázku (minimálnì ICONWIDTH)
int		KorHeight;						// zkorigovaná výška obrázku (minimálnì ICONHEIGHT)
int		MWidth;							// šíøka plochy v bodech + okraj vlevo
int		MHeight;						// výška plochy v bodech + okraj nahoøe

// zobrazená èást plochy
int		TLeft;							// poèáteèní bod plochy vlevo
int		TTop;							// poèáteèní bod plochy nahoøe

// zobrazení plochy na displeji vèetnì záhlaví (v bodech displeje)
int		DLeft;							// poèátek zobrazení vlevo
int		DTop;							// poèátek zobrazení nahoøe
int		DWidth;							// zobrazená šíøka èásti plochy na displeji
int		DHeight;						// zobrazená výška èásti plochy na displeji

int		ILeft;							// index ikony vlevo (0 = levý okraj)
int		IRight;							// index ikony vpravo (0 = levý okraj)
int		ITop;							// index ikony nahoøe (0 = horní okraj)
int		IBottom;						// index ikony dole (0 = horní okraj)

int		OLeft;							// zobrazená èást ikony vlevo
int		ORight;							// zobrazená èást ikony vpravo
int		OTop;							// zobrazená èást ikony nahoøe
int		OBottom;						// zobrazená èást ikony dole

// nastavení nových rozmìrù a vlastností
int		NewDelay;						// nová prodleva mezi fázemi
double	NewStep;						// nový poèet fází na krok
int		NewLevel;						// nová hladina k zobrazení
int		NewKlid;						// nový poèet fází pro klid
int		NewWidth;						// nová šíøka obrázku
int		NewHeight;						// nová výška obrázku
int		NewPohyb;						// nový poèet fází pro pohyb
int		NewSmer;						// nový poèet smìrù
BOOL	NewFill = FALSE;				// probíhá plnìní textù

// obsluha tažení
BOOL	BegDrag = FALSE;				// pøíznak zahájení tažení
BOOL	MDraging = FALSE;				// pøíznak tažení ikony editoru plochy
int		BegX;							// souøadnice myši X pøed zahájením tažení
int		BegY;							// souøadnice myši Y pøed zahájením tažení
BOOL	CopyMode = FALSE;				// pøíznak módu kopírování
int		DragX = MOUSEINV;				// souøadnice X tažené ikony (MOUSEINV = není)
int		DragY = MOUSEINV;				// souøadnice Y tažené ikony (MOUSEINV = není)
HIMAGELIST DragImageList;				// obrázek tažení
int		HotSpotX;						// bod dotyku X
int		HotSpotY;						// bod dotyku Y

CBufIcon	MapSwcNum;					// ikony èísel
CBufIcon	SpriteIcon;					// ikony sprajtù

// testovací mód
BOOL	TestMode = FALSE;				// je testovací mód
int		TestLeft = 0;					// levý okraj pro test
int		TestTop = 0;					// horní okraj pro test
#define	TestWidthN 15					// poèet ikon horizontálnì
#define	TestHeightN 11					// poèet ikon vertikálnì
#define TestWidth (TestWidthN*ICONWIDTH)	// šíøka pro test
#define	TestHeight (TestHeightN*ICONHEIGHT)	// výška pro test
UINT	TestTimer = 0;					// èasovaè pro test
BYTE*	TestBuff = NULL;				// buffer s výstupním obrázkem
#define	TestTimerID 1434l				// ID èasovaèe

int		TestKlid;						// poèet klidových fází
int		TestSmeru;						// poèet smìrù
int		TestFazi;						// poèet fází
int		TestDelay;						// prodleva mezi fázemi (v impulsech)
double	TestKroku;						// poèet krokù na jednotku

int		TestFaze = 0;					// aktuální fáze sprajtu
int		TestSmer = 0;					// aktuální smìr sprajtu
int		TestNext = 0;					// èítaè do pøíštího kroku

int		TestPicWidth;					// šíøka obrázku sprajtu
int		TestPicHeight;					// výška obrázku sprajtu
double	TestX;							// aktuální souøadnice X
double	TestY;							// aktuální souøadnice Y
double	TestEndX;						// cílová souøadnice X
double	TestEndY;						// cílová souøadnice Y

// ************************** obsluha volby editace *************************

/////////////////////////////////////////////////////////////////////////////
// inicializace pøi startu programu

void StartInit()
{
// naètení ikon èísel
	MapSwcNum.IconSize(8, 12);
	MapSwcNum.Load(IDN_MAPSWCNUM, 10);
	ASSERT(MapSwcNum.Num() == 10);

// naètení pomocných ikon sprajtù (záhlaví)
	SpriteIcon.Load(IDN_SPRITE_ICON, 26);
	ASSERT(SpriteIcon.Num() == 26);

// nastavení obrázkù pro sprajty 0 a 1 (Petr a Petra) (mají velikost ICONSIZE)
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
// zahájení editace (musí být platný index!)

void BegEdit(int index)
{
	EndDrag();

// úschova indexu sprajtu
	Index = index;						// index sprajtu
	ASSERT(Sprite.IsValid(index));
	Sprite[index].CopyWrite();			// kopie pøed zápisem

// úschova parametrù sprajtu
	Width = Sprite[index].Faze();		// šíøka plochy v políèkách
	Height = Sprite[index].Smer();		// výška plochy v políèkách
	PicWidth = Sprite[index].Width();	// šíøka obrázku sprajtu
	KorWidth = PicWidth;				// zkorigovaná šíøka obrázku sprajtu
	if (KorWidth < ICONWIDTH) KorWidth = ICONWIDTH;
	PicHeight = Sprite[index].Height();	// výška obrázku sprajtu
	KorHeight = PicHeight;				// zkorigovaná výška sprajtu
	if (KorHeight < ICONHEIGHT) KorHeight = ICONHEIGHT;

// pøepoèet rozložení a zobrazení obrázku
	Disp();
}

/////////////////////////////////////////////////////////////////////////////
// obsluha dialogu pro zadání vlastností

// nastavení prodlevy
void DimenSetDelay(HWND wnd)
{
	CText txt;
	txt.Int(NewDelay);
	NewFill = TRUE;
	txt.SetDialogText(wnd, IDC_DELAY);
	NewFill = FALSE;
}

// naètení prodlevy
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

// nastavení fází na krok
void DimenSetStep(HWND wnd)
{
	CText txt;
	txt.Double(NewStep);
	NewFill = TRUE;
	txt.SetDialogText(wnd, IDC_STEP);
	NewFill = FALSE;
}

// naètení fází na krok
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

// nastavení hladiny
void DimenSetLevel(HWND wnd)
{
	CText txt;
	txt.Int(NewLevel);
	NewFill = TRUE;
	txt.SetDialogText(wnd, IDC_LEVEL);
	NewFill = FALSE;
}

// naètení hladiny
void DimenGetLevel(HWND wnd)
{
	if (!NewFill)
	{
		CText txt;
		txt.GetDialogText(wnd, IDC_LEVEL);
		NewLevel = Int(txt);
	}
}

// nastavení klidových fází
void DimenSetKlid(HWND wnd)
{
	CText txt;
	txt.Int(NewKlid);
	NewFill = TRUE;
	txt.SetDialogText(wnd, IDC_KLID);
	NewFill = FALSE;
}

// naètení klidových fází
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

// nastavení pohybových fází
void DimenSetPohyb(HWND wnd)
{
	CText txt;
	txt.Int(NewPohyb);
	NewFill = TRUE;
	txt.SetDialogText(wnd, IDC_POHYB);
	NewFill = FALSE;
}

// naètení pohybových fází
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

// nastavení smìrù
void DimenSetSmer(HWND wnd)
{
	CText txt;
	txt.Int(NewSmer);
	NewFill = TRUE;
	txt.SetDialogText(wnd, IDC_SMER);
	NewFill = FALSE;
}

// naètení smìrù
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

// nastavení šíøky
void DimenSetWidth(HWND wnd)
{
	CText txt;
	txt.Double((double)NewWidth/ICONWIDTH);
	NewFill = TRUE;
	txt.SetDialogText(wnd, IDC_WIDTH);
	NewFill = FALSE;
}

// naètení šíøky
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

// nastavení výšky
void DimenSetHeight(HWND wnd)
{
	CText txt;
	txt.Double((double)NewHeight/ICONHEIGHT);
	NewFill = TRUE;
	txt.SetDialogText(wnd, IDC_HEIGHT);
	NewFill = FALSE;
}

// naètení výšky
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
// nastavení parametrù sprajtu

void OnDimen()
{
// pøíprava výchozích rozmìrù
	NewDelay = Sprite[Index].Delay();
	NewStep = Sprite[Index].Kroku();
	NewLevel = Sprite[Index].Level();
	NewKlid = Sprite[Index].Klid();
	NewPohyb = Width - NewKlid;
	NewSmer = Height;
	NewWidth = PicWidth;
	NewHeight = PicHeight;

// zadání nových rozmìrù sprajtu
	if (::DialogBoxParam(
			hInstance,
			MAKEINTRESOURCE(IDN_SPRITEDIMEN),
			MainFrame,
			(DLGPROC)DimenDialogProc,
			(LPARAM)IDN_SPRITEDIMEN) != IDOK) return;

// kontrola, zda byly parametry zmìnìny
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

// zapnutí kurzoru èekání
	BeginWaitCursor();

// úschova pro UNDO
	PushUndo();

// nastavení nových parametrù sprajtu
	ASSERT(Sprite.IsValid(Index));
	if (((NewKlid + NewPohyb) < Width) || (NewSmer < Height))
	{
		HistEdit::Del(BufPicID, -2, Index, -1);
	}
	Sprite[Index].ReSize(NewKlid, NewPohyb, NewSmer);
	Sprite[Index].Delay(NewDelay);
	Sprite[Index].Kroku(NewStep);
	Sprite[Index].Level(NewLevel);

	Width = Sprite[Index].Faze();		// šíøka plochy v políèkách
	Height = Sprite[Index].Smer();		// výška plochy v políèkách

	for (int i = Width*Height-1; i >= 0; i--)
	{
		Sprite[Index][i].CopyWrite();
		Sprite[Index][i].Resize(NewWidth, NewHeight);
	}

	PicWidth = NewWidth;				// šíøka obrázku sprajtu
	KorWidth = PicWidth;				// zkorigovaná šíøka obrázku sprajtu
	if (KorWidth < ICONWIDTH) KorWidth = ICONWIDTH;
	PicHeight = NewHeight;				// výška obrázku sprajtu
	KorHeight = PicHeight;				// zkorigovaná výška sprajtu
	if (KorHeight < ICONHEIGHT) KorHeight = ICONHEIGHT;

// pøekreslení displeje (a pøepoèet parametrù)
	Disp();

// pøíznak modifikace
	SetModi();

// vypnutí kurzoru èekání
	EndWaitCursor();
}



// ***************************** obsluha zobrazení **************************

/////////////////////////////////////////////////////////////////////////////
// pøepoèet rozmìrù

void ReCalc()
{
// šíøka a výška plochy v bodech (vèetnì záhlaví)
	MWidth = Width*KorWidth + ICONWIDTH;
	MHeight = Height*KorHeight + ICONHEIGHT;

// pøíprava zobrazené výšky (korekce, je-li horizontální posuvník)
	DHeight = EditHeight - 4;					// výška k zobrazení
	if (EditWidth - 4 - TRACKSIRKA < MWidth)
	{
		DHeight -= TRACKSIRKA;					// snížený výšky o horizontální posuvník
		if (DHeight < 1) DHeight = 1;			// minimální šíøka
	}

// pøíprava zobrazené šíøky (korekce, je-li vertikální posuvník)
	DWidth = EditWidth - 4;						//	šíøka k zobrazení
	if (DHeight < MHeight)
	{
		DWidth -= TRACKSIRKA;					// snížení šíøky o vertikální posuvník
		if (DWidth < 1) DWidth = 1;				// minimální šíøka
	}

// zobrazený poèátek
	DLeft = EditX + 2 + (DWidth - MWidth)/2;
	if (DWidth < MWidth) DLeft = EditX + 2;
	DTop = EditY + 2 + (DHeight - MHeight)/2;
	if (DHeight < MHeight) DTop = EditY + 2;

// omezení na skuteènou výšku a šíøku
	if (DHeight > MHeight) DHeight = MHeight;	// omezení na skuteèné rozmìry
	if (DWidth > MWidth) DWidth = MWidth;		// omezení na skuteèné rozmìry

// omezení poèáteèního bodu
	if (TLeft < 0) TLeft = 0;
	if (TLeft > MWidth - DWidth) TLeft = MWidth - DWidth;
	if (TTop < 0) TTop = 0;
	if (TTop > MHeight - DHeight) TTop = MHeight - DHeight;

// pøepoèet zobrazeného poèátku na ikony a body
	if (TLeft < ICONWIDTH)
	{
		ILeft = 0;
		OLeft = ICONWIDTH - TLeft;				// zobrazená èást ikony vlevo
	}
	else
	{
		ILeft = (TLeft-ICONWIDTH) / KorWidth + 1;	// index ikony vlevo
		OLeft = KorWidth - ((TLeft-ICONWIDTH) % KorWidth);	// zobrazená èást ikony vlevo
	}

	if (TTop < ICONHEIGHT)
	{
		ITop = 0;
		OTop = ICONHEIGHT - TTop;
	}
	else
	{
		ITop = (TTop-ICONHEIGHT) / KorHeight + 1;	// index ikony nahoøe
		OTop = KorHeight - ((TTop-ICONHEIGHT) % KorHeight);	// zobrazená èást ikony nahoøe
	}

// zobrazená èást ikon vpravo a dole
	ORight = KorWidth - ((MWidth - TLeft - DWidth) % KorWidth);
	OBottom = KorHeight - ((MHeight - TTop - DHeight) % KorHeight);

// index ikon vpravo a dole
	IRight = ILeft + (DWidth - OLeft + KorWidth - 1) / KorWidth;
	IBottom = ITop + (DHeight - OTop + KorHeight - 1) / KorHeight;

// pøíznak zapnutí posuvníkù
	HScrollDisp = (DWidth < MWidth);
	VScrollDisp = (DHeight < MHeight);
}


/////////////////////////////////////////////////////////////////////////////
// zobrazení celého okna editoru

void Disp()
{
// pøepoèet zobrazení
	ReCalc();

// nastavení posuvníkù
	SetScroll();

// otevøení kontextu displeje
	HDC dc = ::GetDC(MainFrame);

// nastavení vlastních palet
	HPALETTE OldPal;					// úschova pùvodních palet
	OldPal = ::SelectPalette(dc,		// výbìr vlastních palet
		StdPalette, FALSE);
	::RealizePalette(dc);				// realizace palet

// vykreslení obrázku
	DispAkt(dc);

// pøíprava štìtce k vymazání podkladu
	HBRUSH brush = (HBRUSH)::GetStockObject(LTGRAY_BRUSH); // štìtec k vymazání plochy

// vymazání plochy nahoøe nad obrázkem
	RECT rc;
	rc.left = EditX + 2;
	rc.right = EditX + EditWidth - 2;
	rc.top = EditY + 2;
	rc.bottom = DTop;
	if (rc.top < rc.bottom)
	{
		::FillRect(dc, &rc, brush);
	}

// vymazání plochy dole pod obrázkem (left a right je nastaveno)
	rc.top = DTop + DHeight;
	rc.bottom = EditY + EditHeight - 2;
	if (rc.top < rc.bottom)
	{
		::FillRect(dc, &rc, brush);
	}

// vymazání plochy vlevo od obrázku (left je nastaveno)
	rc.right = DLeft;
	rc.top = DTop;
	rc.bottom = DTop + DHeight;
	if (rc.left < rc.right)
	{
		::FillRect(dc, &rc, brush);
	}

// vymazání plochy vpravo od obrázku (top a bottom je nastaveno)
	rc.left = DLeft + DWidth;
	rc.right = EditX + EditWidth - 2;
	if (rc.left < rc.right)
	{
		::FillRect(dc, &rc, brush);
	}

// zrušení štìtce podkladu (i když podle dokumentace rušení není nutné)
	::DeleteObject(brush);

// návrat pùvodních palet
	::SelectPalette(dc,OldPal,FALSE);

// uvolnìní kontextu displeje
	::ReleaseDC(MainFrame, dc);
}


/////////////////////////////////////////////////////////////////////////////
// aktualizaèní pøekreslení obrázku (s pøepoètem rozmìrù)

void ReDisp()
{
// pøepoèet zobrazení
	ReCalc();

// nastavení posuvníkù
	SetScroll();

// otevøení kontextu displeje
	HDC dc = ::GetDC(MainFrame);

// nastavení vlastních palet
	HPALETTE OldPal;					// úschova pùvodních palet
	OldPal = ::SelectPalette(dc,		// výbìr vlastních palet
		StdPalette, FALSE);
	::RealizePalette(dc);				// realizace palet

// vykreslení obrázku
	DispAkt(dc);

// návrat pùvodních palet
	::SelectPalette(dc,OldPal,FALSE);

// uvolnìní kontextu displeje
	::ReleaseDC(MainFrame, dc);
}


/////////////////////////////////////////////////////////////////////////////
// aktualizaèní pøekreslení obrázku

void DispAkt(HDC dc)
{
// lokální promìnné
	int i,j, k, l;
	int width = ((IRight - ILeft + 1)*KorWidth + 7) & ~7;
	int height = (IBottom - ITop + 1)*KorHeight;
	BYTE* dst;
	BYTE* src;
	BYTE col;
	MapSwcNum[0].DeComp();
	BYTE numcol = (MapSwcNum[0].DataData())[0];		// barva podkladu èíslic
	int cislo;										// èíslo fáze v záhlaví
	int cislic;										// poèet èíslic èísla v záhlaví
	double uhel;									// smìr úhlu
	double uhelsin;									// sinus úhlu
	double uhelcos;									// cosinus úhlu
	CPicture* data = Sprite[Index].DataData();

// vytvoøení bufferu k zobrazení
	BYTE* buf = (BYTE*)MemGet(width*height);

// zobrazení ikon
	for (i = ILeft; i <= IRight; i++)
	{
		for (j = ITop; j <= IBottom; j++)
		{

// cílová adresa k uložení ikony
			dst = buf + (i-ILeft)*KorWidth + (IBottom-j)*KorHeight*width;
			if ((i > 0) && (ILeft == 0)) dst += ICONWIDTH - KorWidth;

// je to horní øádek (záhlaví s èíslem fáze)?
			if (j == 0)
			{

// dekódování rohu záhlaví
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

// jinak to bude èíslo fáze
				else
				{

// vymazání ikony pro èíslo fáze
					col = OrangeCol;

					for (k = ICONHEIGHT; k > 0; k--)
					{
						MemFill(dst, KorWidth, col);
						dst += width;
					}
					dst -= width*(ICONHEIGHT/2 + 6);

// pøíprava barvy a èísla fáze (s rozlišením fáze pro klid)
					col = RedCol;
					cislo = i - 1;
					if (cislo >= Sprite[Index].Klid())
					{
						cislo -= Sprite[Index].Klid();
						col = BlackCol;
					}

// pøíprava poètu èíslic a ukládaci adresy
					cislic = 1;
					if (cislo >= 10) cislic++;
					if (cislo >= 100) cislic++;
					if (cislo >= 1000) cislic++;
					if (cislo >= 10000) cislo = 9999;
					dst += KorWidth/2 - 8 + cislic*4;

// dekódování znakù èíslic
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

// není horní øádek
			else
			{

// je to záhlaví vlevo (smìr)?
				if (i == 0)
				{

// vymazání ikony pro smìr
					col = OrangeCol;

					for (k = KorHeight; k > 0; k--)
					{
						MemFill(dst, ICONWIDTH, col);
						dst += width;
					}
					dst -= width*((KorHeight+ICONHEIGHT)/2);

// dekódování podkladu záhlaví pro smìr
					SpriteIcon[0].DeComp();
					src = SpriteIcon[0].DataData();

					for (k = ICONHEIGHT; k > 0; k--)
					{
						MemCopy(dst, src, ICONWIDTH);
						dst += width;
						src += ICONWIDTH;
					}
					dst -= width*(ICONHEIGHT/2) - ICONWIDTH/2;

// pøíprava úhlu smìru
					uhel = ((double)pi2 / Height) * (j - 1);
					uhelsin = sin(uhel);
					uhelcos = cos(uhel);

// dekódování smìru
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

// jinak jsou platná data
				else
				{

// vymazání ikony pro obrázek
					col = BackCol;

					for (k = KorHeight; k > 0; k--)
					{
						MemFill(dst, KorWidth, col);
						dst += width;
					}
					dst -= width*((KorHeight+PicHeight)/2) - (KorWidth-PicWidth)/2;

// dekódování platné ikony
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

// zobrazení horizontálních èar rastru
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

// zobrazení vertikálních èar rastru
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

// pøednastavení parametrù záhlaví bitmapy
	StdBitmapInfo->bmiHeader.biWidth = width;
	StdBitmapInfo->bmiHeader.biHeight = DHeight;

// zobrazení
	::StretchDIBits(dc, DLeft, DTop, DWidth, DHeight, 
		(ILeft == 0) ? (ICONWIDTH - OLeft) : (KorWidth - OLeft),
			0, DWidth, DHeight, buf + (KorHeight-OBottom)*width,
			StdBitmapInfo, DIB_RGB_COLORS, SRCCOPY);

// zrušení bufferu
	MemFree(buf);
}


// ********************* obsluha posuvníkù **********************************


/////////////////////////////////////////////////////////////////////////////
// pøesun posuvníkù pøi zmìnì rozmìrù okna

HDWP MoveScroll(HDWP hdwp)
{
// lokální promìnné
	int x, y, w, h;

// pøepoèet zobrazení
	ReCalc();

// pøesun a zobrazení horizontálního posuvníku
	x = EditX + 2;
	y = EditY + EditHeight - 2 - TRACKSIRKA;
	w = EditWidth - 4;
	h = TRACKSIRKA;
	if (VScrollDisp) w -= TRACKSIRKA;
	hdwp = ::DeferWindowPos(hdwp, HScroll, NULL, x, y, w, h,
			SWP_DRAWFRAME | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER |
			((HScrollDisp && !PreviewPic) ? SWP_SHOWWINDOW : SWP_HIDEWINDOW));

// pøesun a zobrazení vertikálního posuvníku
	x = EditX + EditWidth - 2 - TRACKSIRKA;
	y = EditY + 2;
	w = TRACKSIRKA;
	h = EditHeight - 4;
	if (HScrollDisp) h -= TRACKSIRKA;
	hdwp = ::DeferWindowPos(hdwp, VScroll, NULL, x, y, w, h,
			SWP_DRAWFRAME | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER |
			((VScrollDisp && !PreviewPic) ? SWP_SHOWWINDOW : SWP_HIDEWINDOW));

// nastavení posuvníkù
	SetScroll();

// nová hodnota HDWP
	return hdwp;
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace zobrazení posuvníkù

void SetScroll()
{
// lokální promìnné
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);

// -------- horizontální posuvník -------------

// naètení stavu horizontálního posuvníku
	si.fMask = SIF_ALL;
	::GetScrollInfo(HScroll, SB_CTL, &si);	// naètení stavu posuvníku
	si.fMask = 0;							// pøednastavení - nic se nemìní

// minimální pozice
	if (si.nMin != 0)
	{
		si.nMin = 0;
		si.fMask |= SIF_RANGE;
	}

// maximální pozice
	if (si.nMax != MWidth)
	{
		si.nMax = MWidth;
		si.fMask |= SIF_RANGE;
	}

// velikost stránky
	if (si.nPage != (UINT)DWidth)
	{
		si.nPage = (UINT)DWidth;
		si.fMask |= SIF_PAGE;
	}

// pozice posuvníku
	if (si.nPos != TLeft)
	{
		si.nPos = TLeft;
		si.fMask |= SIF_POS;
	}

// nastavení nových parametrù posuvníku
	if (si.fMask)
	{
		::SetScrollInfo(HScroll, SB_CTL, &si, TRUE);
	}

// zobrazení horizontálního posuvníku
	::ShowWindow(HScroll, (HScrollDisp && !PreviewPic) ? SW_SHOWNA : SW_HIDE);


// -------- vertikální posuvník -------------

// naètení stavu vertikálního posuvníku
	si.fMask = SIF_ALL;
	::GetScrollInfo(VScroll, SB_CTL, &si);
	si.fMask = 0;							// pøednastavení - nic se nemìní

// minimální pozice
	if (si.nMin != 0)
	{
		si.nMin = 0;
		si.fMask |= SIF_RANGE;
	}

// maximální pozice
	if (si.nMax != MHeight)
	{
		si.nMax = MHeight;
		si.fMask |= SIF_RANGE;
	}

// velikost stránky
	if (si.nPage != (UINT)DHeight)
	{
		si.nPage = (UINT)DHeight;
		si.fMask |= SIF_PAGE;
	}

// pozice posuvníku
	if (si.nPos != TTop)
	{
		si.nPos = TTop;
		si.fMask |= SIF_POS;
	}

// nastavení nových parametrù posuvníku
	if (si.fMask)
	{
		::SetScrollInfo(VScroll, SB_CTL, &si, TRUE);
	}

// zobrazení vertikálního posuvníku
	::ShowWindow(VScroll, (VScrollDisp && !PreviewPic) ? SW_SHOWNA : SW_HIDE);
}

/////////////////////////////////////////////////////////////////////////////
// horizontální posuvník

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
// vertikální posuvník

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
// ukonèení tažení

void EndDrag()
{
	BegDrag = FALSE;

	if (MDraging)
	{
		MDraging = FALSE;

// zastavení tažení
		::ImageList_DragLeave(NULL);

// ukonèení tažení
		::ImageList_EndDrag();

// zrušení seznamu obrázkù tažení
		::ImageList_Destroy(DragImageList);

// uvolnìni zachytávání myši
		::ReleaseCapture();

// nastavení bìžného kurzoru myši
		::SetCursor(CurArrow);
	}
}


/////////////////////////////////////////////////////////////////////////////
// stisk tlaèítka myši (TRUE = obslouženo)

BOOL OnButtonDown(UINT flags, int x, int y, BOOL right, BOOL dclick)
{
// ukonèení tažení
	if (MDraging)
	{
		EndDrag();
		return TRUE;
	}

// kontrola, zda je platné editaèní pole
	if ((MouseX == MOUSEINV) || (MouseY == MOUSEINV))
	{
		return FALSE;
	}

// úschova souøadnic pøed zahájením tažení
	BegX = x;
	BegY = y;
	BegDrag = TRUE;
	CopyMode = FALSE;
	DragX = MouseX;
	DragY = MouseY;

// zahájení editace pøi dvojkliku
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
// posun myši

void OnMouseMove(UINT flags, int x, int y)
{
// korekce souøadnic pøi tažení (na støed ikony)
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

// pøíprava relativních souøadnic bodu v editaèním poli
	int	sx = (x - DLeft - OLeft + KorWidth)/KorWidth + ILeft - 1;
	int sy = (y - DTop - OTop + KorHeight)/KorHeight + ITop - 1;

// souøadnice myši pro zobrazení
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

// zahájení tažení
	if ((flags & (MK_LBUTTON | MK_RBUTTON)) &&
		!MDraging &&
		BegDrag &&
		(DragX != MOUSEINV) &&
		(DragY != MOUSEINV) &&
		(abs(x - BegX) + abs(y - BegY) >= 5))
	{
		MDraging = TRUE;

// bod styku pro tažení
		HotSpotX = BegX - DLeft - OLeft - (DragX-ILeft)*KorWidth;
		HotSpotY = BegY - DTop - OTop - (DragY-ITop)*KorHeight;

// vygenerování tažené ikony
		DragImageList = (Sprite[Index].DataData())[DragX + DragY*Width].GenerDrag(EmptyText);

// zapnutí zobrazení obrázku tažení (bez uzamykání okna)
		::ImageList_DragShowNolock(TRUE);

// zapnutí kurzoru pro tažení
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

// vytvoøení nového obrázku tažení
		::ImageList_SetDragCursorImage(DragImageList, 0, 0, 0);

// zahájení operace tažení
		::ImageList_BeginDrag(DragImageList, 0, 0, 0);

// zahájení operace tažení, zákaz aktualizace okna
		::ImageList_DragEnter(NULL, MouseScreen.x - HotSpotX, MouseScreen.y - HotSpotY);

// pøetažení obrázku na aktuální pozici myši
		::ImageList_DragMove(MouseScreen.x - HotSpotX, MouseScreen.y - HotSpotY);

// zahájení zachytávání myši
		::SetCapture(MainFrame);
	}
	else
	{

// ukonèení tažení, pokud bylo tlaèítko uvolnìno mimo oblast
		if (MDraging && ((flags & (MK_LBUTTON | MK_RBUTTON)) == 0))
		{
			EndDrag();
		}
		else
		{

// pokraèování v tažení
			if (MDraging)
			{

// pøetažení obrázku na aktuální pozici myši
				::ImageList_DragMove(MouseScreen.x - HotSpotX, MouseScreen.y - HotSpotY);

// nastavení kurzoru myši
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

// zobrazení souøadnic myši
	DispMouseXY();
}

/////////////////////////////////////////////////////////////////////////////
// uvolnìní tlaèítka myši

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
// položení obrázku zvnìjšku

void DragDrop(int pic)
{
// ukonèení editace èíselného parametru
	if ((EditMode != BufSprID) ||
		Sprite.IsNotValid(Index) ||
		Picture.IsNotValid(pic) ||
		((DWORD)MouseX >= (DWORD)Width) ||
		((DWORD)MouseY >= (DWORD)Height))
	{
		return;
	}

// úschova pro UNDO
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

// vymazání bufferu ikonami pozadí
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

// pøíprava fáze sprajtu
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

// souøadnice a rozmìry k zobrazení sprajtu
	int x = Round(TestX);
	int y = Round(TestY);
	int width = TestPicWidth;
	int height = TestPicHeight;

// zdrojová adresa sprajtu
	Sprite[Index].At(faze, TestSmer).DeComp();
	src = Sprite[Index].At(faze, TestSmer).DataData();

// omezení souøadnic a rozmìrù
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

// cílová adresa k dekódování sprajtu
	dst = TestBuff + x + y*TestWidth;

// dekódování sprajtu
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

// otevøení DC
	HDC dc = ::GetDC(hWnd);

// nastavení vlastních palet
	HPALETTE OldPal;					// úschova pùvodních palet
	OldPal = ::SelectPalette(dc,		// výbìr vlastních palet
		StdPalette, FALSE);
	::RealizePalette(dc);				// realizace palet

// pøednastavení parametrù záhlaví bitmapy
	StdBitmapInfo->bmiHeader.biWidth = TestWidth;
	StdBitmapInfo->bmiHeader.biHeight = TestHeight;

// zobrazení obrázku
	::StretchDIBits(dc, TestLeft, TestTop, TestWidth, TestHeight, 0,
			0, TestWidth, TestHeight, TestBuff,
			StdBitmapInfo, DIB_RGB_COLORS, SRCCOPY);

// návrat pùvodních palet
	::SelectPalette(dc,OldPal,FALSE);

// uvolnìní DC
	::ReleaseDC(hWnd, dc);

// posun souøadnic
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
// kontrola, zda je režim editace
	if (!ProgMode ||
		(EditMode != BufSprID) ||
		Sprite.IsNotValid(Index) ||
		TestMode)
	{
		return;
	}

// vytvoøení bufferu
	TestBuff = (BYTE*) MemGet(TestWidth * TestHeight);

// zahájení testovacího módu
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

// ukonèení testovacího módu
	TestMode = FALSE;

// zrušení bufferu
	MemFree(TestBuff);
	TestBuff = NULL;
}

// ****************************** obsluha UNDO ******************************


/////////////////////////////////////////////////////////////////////////////
// úschova plochu pro UNDO

void PushUndo()
{
	Undo.AddSprSet(Index, Sprite[Index]);
	Sprite[Index].CopyWrite();
	Undo.Stop();
}


}
