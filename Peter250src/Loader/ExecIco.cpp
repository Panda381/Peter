
#include "Main.h"

/***************************************************************************\
*																			*
*						Provádìní programu - ikona							*
*																			*
\***************************************************************************/

#pragma optimize("t", on)			// optimalizace na maximální rychlost


/////////////////////////////////////////////////////////////////////////////
// naètení ikony z plochy

void _fastcall FIconGet(CIcon& icon)
{
// vyprázdnìní ikony
	icon.Empty();

// naètení parametrù výøezu
	int x = FIntX();				// poèátek výøezu X
	int y = FIntY();				// poèátek výøezu Y
	int width = ICONWIDTH;			// šíøka výøezu
	int height = ICONHEIGHT;		// výška výøezu

// omezení parametrù
	if (x < 0) { width += x; x = 0; }
	if (x > Width) x = Width;
	if (width < 0) width = 0;
	if (width > Width) width = Width;
	if (x + width > Width) width = Width - x;

	if (y < 0) { height += y; y = 0; }
	if (y > Height) y = Height;
	if (height < 0) height = 0;
	if (height > Height) height = Height;
	if (y + height > Height) height = Height - y;

// kontrola platnosti parametrù
	if ((width <= 0) || (height <= 0)) return;

// vytvoøení ikony
	icon.New();
	if ((width < ICONWIDTH) || (height < ICONHEIGHT))
	{
		MemFill(icon.DataData(), ICONSIZE, BackCol);
	}

// pøenesení dat obrázku
	BYTE* dst = icon.DataData() + (ICONWIDTH - width)/2 + (ICONHEIGHT - height)/2*ICONWIDTH;
	BYTE* src = BackBuf + x + y*WidthByte;
	for (; height > 0; height--)
	{
		MemCopy(dst, src, width);
		dst += ICONWIDTH;
		src += WidthByte;
	}
}


/***************************************************************************\
*																			*
*									funkce									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// funkce s návratem ikony

void _fastcall FIconFunc(CIcon& icon)
{
// úschova indexu volané funkce
	int data = ExecItem[-1].Data;

// úschova indexu promìnné s návratovou hodnotou
	int res = ExecItem[-1].List;

// inicializace lokálních promìnných
	FCommand();

// úschova ukazatele programu
	EXECITEM* oldexe = ExecItem;

// nová adresa programu
	ExecItem = ProgBuf + data;

// kontrola hloubky vnoøení
	Hloubka--;
	if (Hloubka >= 0)
	{

// vyvolání funkce
		FCommand();
	}
	Hloubka++;

// návrat adresy programu
	ExecItem = oldexe;

// zrušení požadavku o pøerušení
	Break &= ~(BREAKFUNC | BREAKWHILE);

// návrat výsledku operace
	icon = Icon[Icon.Num() - res];

// zrušení lokálních promìnných
	FCommand();
}


/***************************************************************************\
*																			*
*								promìnné									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// prázdná ikona

void _fastcall FIconEmpty(CIcon& icon) { icon.Empty(); };


/////////////////////////////////////////////////////////////////////////////
// globální ikona (Data = index)

void _fastcall FIconObj(CIcon& icon)
{
// index ikony
	int inx = ExecItem[-1].Data;

// nastavení ikony
	icon = Icon[inx];
}


/////////////////////////////////////////////////////////////////////////////
// globalní ikona v seznamu (Data = index, List = seznam)

void _fastcall FIconObjList(CIcon& icon)
{
// index seznamu
	int list = ExecItem[-1].List;

// index ikony, inkrementace indexu seznamu
	int inx = ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// nastavení ikony
	icon = Icon[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lokální ikona (Data = index)

void _fastcall FIconLoc(CIcon& icon)
{
// index ikony
	int inx = Icon.Num() - ExecItem[-1].Data;

// nastavení ikony
	icon = Icon[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lokální ikona v seznamu (Data = index, List = seznam)

void _fastcall FIconLocList(CIcon& icon)
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index ikony, inkrementace indexu seznamu
	int inx = Icon.Num() - ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// nastavení ikony
	icon = Icon[inx];
}


/***************************************************************************\
*																			*
*							obsluha Petra a Petry							*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// naètení pøedmìtu pøed Petrem

void _fastcall FPredmet(CIcon& icon)
{
	int x;
	int y;
	if (PetrXYNext(x, y))
	{
		icon = Map[0].At(x, y).Icon;
	}
	else
	{
		icon.Empty();
	}
}


/////////////////////////////////////////////////////////////////////////////
// naètení pøedmìtu pøed Petrou

void _fastcall FPredmet2(CIcon& icon)
{
	int x;
	int y;
	if (Petr2XYNext(x, y))
	{
		icon = Map[0].At(x, y).Icon;
	}
	else
	{
		icon.Empty();
	}
}


/////////////////////////////////////////////////////////////////////////////
// naètení pøedmìtu pod Petrem

void _fastcall FPredmetPoz(CIcon& icon)
{
	int x;
	int y;
	if (PetrXY(x, y))
	{
		icon = Map[0].At(x, y).Icon;
	}
	else
	{
		icon.Empty();
	}
}


/////////////////////////////////////////////////////////////////////////////
// naètení pøedmìtu pod Petrou

void _fastcall FPredmet2Poz(CIcon& icon)
{
	int x;
	int y;
	if (Petr2XY(x, y))
	{
		icon = Map[0].At(x, y).Icon;
	}
	else
	{
		icon.Empty();
	}
}


/////////////////////////////////////////////////////////////////////////////
// naètení pøedmìtu z plochy

void _fastcall FGetMapIcon(CIcon& icon)
{
// naètení plochy
	CMap map;
	FMap(map);

// naètení souøadnic
	int x = FIntR();
	int y = FIntR();

// ikona na pozici
	if (((DWORD)x < (DWORD)map.Width()) && ((DWORD)y < (DWORD)map.Height()))
	{
		icon = map.At(x, y).Icon;
	}
	else
	{
		icon.Empty();
	}
}


/////////////////////////////////////////////////////////////////////////////
// naètení ikony okna

void _fastcall FGetWindowIcon(CIcon& icon)
{
	if (Win.IsValid(WindowID))
	{
		icon = Win[WindowID].Icon;
	}
	else
	{
		icon.Empty();
	}
}

/////////////////////////////////////////////////////////////////////////////
// naètení ikony ze souboru

void _fastcall FGetFileIcon(CIcon& icon)
{
	icon.Empty();
	icon.LoadFile();
}
