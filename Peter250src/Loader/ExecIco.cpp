
#include "Main.h"

/***************************************************************************\
*																			*
*						Prov�d�n� programu - ikona							*
*																			*
\***************************************************************************/

#pragma optimize("t", on)			// optimalizace na maxim�ln� rychlost


/////////////////////////////////////////////////////////////////////////////
// na�ten� ikony z plochy

void _fastcall FIconGet(CIcon& icon)
{
// vypr�zdn�n� ikony
	icon.Empty();

// na�ten� parametr� v��ezu
	int x = FIntX();				// po��tek v��ezu X
	int y = FIntY();				// po��tek v��ezu Y
	int width = ICONWIDTH;			// ���ka v��ezu
	int height = ICONHEIGHT;		// v��ka v��ezu

// omezen� parametr�
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

// kontrola platnosti parametr�
	if ((width <= 0) || (height <= 0)) return;

// vytvo�en� ikony
	icon.New();
	if ((width < ICONWIDTH) || (height < ICONHEIGHT))
	{
		MemFill(icon.DataData(), ICONSIZE, BackCol);
	}

// p�enesen� dat obr�zku
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
// funkce s n�vratem ikony

void _fastcall FIconFunc(CIcon& icon)
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
	icon = Icon[Icon.Num() - res];

// zru�en� lok�ln�ch prom�nn�ch
	FCommand();
}


/***************************************************************************\
*																			*
*								prom�nn�									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// pr�zdn� ikona

void _fastcall FIconEmpty(CIcon& icon) { icon.Empty(); };


/////////////////////////////////////////////////////////////////////////////
// glob�ln� ikona (Data = index)

void _fastcall FIconObj(CIcon& icon)
{
// index ikony
	int inx = ExecItem[-1].Data;

// nastaven� ikony
	icon = Icon[inx];
}


/////////////////////////////////////////////////////////////////////////////
// globaln� ikona v seznamu (Data = index, List = seznam)

void _fastcall FIconObjList(CIcon& icon)
{
// index seznamu
	int list = ExecItem[-1].List;

// index ikony, inkrementace indexu seznamu
	int inx = ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// nastaven� ikony
	icon = Icon[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lok�ln� ikona (Data = index)

void _fastcall FIconLoc(CIcon& icon)
{
// index ikony
	int inx = Icon.Num() - ExecItem[-1].Data;

// nastaven� ikony
	icon = Icon[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lok�ln� ikona v seznamu (Data = index, List = seznam)

void _fastcall FIconLocList(CIcon& icon)
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index ikony, inkrementace indexu seznamu
	int inx = Icon.Num() - ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// nastaven� ikony
	icon = Icon[inx];
}


/***************************************************************************\
*																			*
*							obsluha Petra a Petry							*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// na�ten� p�edm�tu p�ed Petrem

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
// na�ten� p�edm�tu p�ed Petrou

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
// na�ten� p�edm�tu pod Petrem

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
// na�ten� p�edm�tu pod Petrou

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
// na�ten� p�edm�tu z plochy

void _fastcall FGetMapIcon(CIcon& icon)
{
// na�ten� plochy
	CMap map;
	FMap(map);

// na�ten� sou�adnic
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
// na�ten� ikony okna

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
// na�ten� ikony ze souboru

void _fastcall FGetFileIcon(CIcon& icon)
{
	icon.Empty();
	icon.LoadFile();
}
