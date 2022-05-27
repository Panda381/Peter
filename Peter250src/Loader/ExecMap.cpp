
#include "Main.h"

/***************************************************************************\
*																			*
*						Provádìní programu - plocha							*
*																			*
\***************************************************************************/

#pragma optimize("t", on)			// optimalizace na maximální rychlost


/***************************************************************************\
*																			*
*									funkce									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// funkce s návratem plochy

void _fastcall FMapFunc(CMap& map)
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
	map = Map[Map.Num() - res];

// zrušení lokálních promìnných
	FCommand();
}


/***************************************************************************\
*																			*
*								promìnné									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// prázdná plocha

void _fastcall FMapEmpty(CMap& map) { map.Empty(); };


/////////////////////////////////////////////////////////////////////////////
// hlavní plocha

void _fastcall FMapMain(CMap& map) { map = Map[0]; };


/////////////////////////////////////////////////////////////////////////////
// globální plocha (Data = index)

void _fastcall FMapObj(CMap& map)
{
// index plochy
	int inx = ExecItem[-1].Data;

// nastavení plochy
	map = Map[inx];
}


/////////////////////////////////////////////////////////////////////////////
// globalní plocha v seznamu (Data = index, List = seznam)

void _fastcall FMapObjList(CMap& map)
{
// index seznamu
	int list = ExecItem[-1].List;

// index plochy, inkrementace indexu seznamu
	int inx = ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// nastavení plochy
	map = Map[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lokální plocha (Data = index)

void _fastcall FMapLoc(CMap& map)
{
// index plochy
	int inx = Map.Num() - ExecItem[-1].Data;

// nastavení plochy
	map = Map[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lokální plocha v seznamu (Data = index, List = seznam)

void _fastcall FMapLocList(CMap& map)
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index plochy, inkrementace indexu seznamu
	int inx = Map.Num() - ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// nastavení plochy
	map = Map[inx];
}


/////////////////////////////////////////////////////////////////////////////
// kopie výøezu plochy

void _fastcall FMapCopy(CMap& map)
{
// naètení vstupní plochy
	FMap(map);

// cílové souøadnice
	int destx = FIntR();
	int desty = FIntR();

// zdrojová plocha
	CMap srcmap;
	FMap(srcmap);

// zdrojové souøadnice a rozmìry
	int srcx = FIntR();
	int srcy = FIntR();
	int width = FIntR();
	int height = FIntR();

// omezení cílových souøadnic
	if ((destx >= map.Width()) || (desty >= map.Height())) return;

	if (destx < 0) 
	{
		srcx -= destx;
		width += destx;
		destx = 0;
	}
	if (destx + width > map.Width()) width = map.Width() - destx;

	if (desty < 0)
	{
		srcy -= desty;
		height += desty;
		desty = 0;
	}
	if (desty + height > map.Height()) height = map.Height() - desty;

// omezení zdrojových souøadnic
	if ((srcx >= srcmap.Width()) || (srcy >= srcmap.Height())) return;

	if (srcx < 0)
	{
		destx -= srcx;
		width += srcx;
		srcx = 0;
	}
	if (srcx + width > srcmap.Width()) width = srcmap.Width() - srcx;

	if (srcy < 0)
	{
		desty -= srcy;
		height += srcy;
		srcy = 0;
	}

// kontrola, zda je co zobrazit
	if ((destx >= map.Width()) || (desty >= map.Height())) return;
	if ((width <= 0) || (height <= 0)) return;

// pøíprava cílové plochy pro zápis
	map.CopyWrite();

// pøenesení dat
	MAPITEM* dstitem = &map.At(destx, desty);
	MAPITEM* srcitem = &srcmap.At(srcx, srcy);
	int dstinc = map.Width() - width;
	int srcinc = srcmap.Width() - width;

	for (; height > 0; height--)
	{

		for (int i = width; i > 0; i--)
		{
			dstitem->Param = srcitem->Param;
			dstitem->Icon = srcitem->Icon;

			dstitem++;
			srcitem++;
		}

		dstitem += dstinc;
		srcitem += srcinc;
	}
}

/////////////////////////////////////////////////////////////////////////////
// nová plocha

void _fastcall FMapNew(CMap& map)
{
// rozmìry plochy
	int width = Round0(FNum());
	int height = Round0(FNum());

	if (width <= 0) width = Map[0].Width();
	if (height <= 0) height = Map[0].Height();

	map.New(width, height);
	map.Clear();
}
