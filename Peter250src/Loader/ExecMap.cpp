
#include "Main.h"

/***************************************************************************\
*																			*
*						Prov�d�n� programu - plocha							*
*																			*
\***************************************************************************/

#pragma optimize("t", on)			// optimalizace na maxim�ln� rychlost


/***************************************************************************\
*																			*
*									funkce									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// funkce s n�vratem plochy

void _fastcall FMapFunc(CMap& map)
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
	map = Map[Map.Num() - res];

// zru�en� lok�ln�ch prom�nn�ch
	FCommand();
}


/***************************************************************************\
*																			*
*								prom�nn�									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// pr�zdn� plocha

void _fastcall FMapEmpty(CMap& map) { map.Empty(); };


/////////////////////////////////////////////////////////////////////////////
// hlavn� plocha

void _fastcall FMapMain(CMap& map) { map = Map[0]; };


/////////////////////////////////////////////////////////////////////////////
// glob�ln� plocha (Data = index)

void _fastcall FMapObj(CMap& map)
{
// index plochy
	int inx = ExecItem[-1].Data;

// nastaven� plochy
	map = Map[inx];
}


/////////////////////////////////////////////////////////////////////////////
// globaln� plocha v seznamu (Data = index, List = seznam)

void _fastcall FMapObjList(CMap& map)
{
// index seznamu
	int list = ExecItem[-1].List;

// index plochy, inkrementace indexu seznamu
	int inx = ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// nastaven� plochy
	map = Map[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lok�ln� plocha (Data = index)

void _fastcall FMapLoc(CMap& map)
{
// index plochy
	int inx = Map.Num() - ExecItem[-1].Data;

// nastaven� plochy
	map = Map[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lok�ln� plocha v seznamu (Data = index, List = seznam)

void _fastcall FMapLocList(CMap& map)
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index plochy, inkrementace indexu seznamu
	int inx = Map.Num() - ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// nastaven� plochy
	map = Map[inx];
}


/////////////////////////////////////////////////////////////////////////////
// kopie v��ezu plochy

void _fastcall FMapCopy(CMap& map)
{
// na�ten� vstupn� plochy
	FMap(map);

// c�lov� sou�adnice
	int destx = FIntR();
	int desty = FIntR();

// zdrojov� plocha
	CMap srcmap;
	FMap(srcmap);

// zdrojov� sou�adnice a rozm�ry
	int srcx = FIntR();
	int srcy = FIntR();
	int width = FIntR();
	int height = FIntR();

// omezen� c�lov�ch sou�adnic
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

// omezen� zdrojov�ch sou�adnic
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

// p��prava c�lov� plochy pro z�pis
	map.CopyWrite();

// p�enesen� dat
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
// nov� plocha

void _fastcall FMapNew(CMap& map)
{
// rozm�ry plochy
	int width = Round0(FNum());
	int height = Round0(FNum());

	if (width <= 0) width = Map[0].Width();
	if (height <= 0) height = Map[0].Height();

	map.New(width, height);
	map.Clear();
}
