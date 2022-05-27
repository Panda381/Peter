
#include "Main.h"

/***************************************************************************\
*																			*
*						Prov�d�n� programu - sprajt							*
*																			*
\***************************************************************************/

#pragma optimize("t", on)			// optimalizace na maxim�ln� rychlost


/***************************************************************************\
*																			*
*									funkce									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// funkce s n�vratem sprajtu

void _fastcall FSpriteFunc(CSprite& sprite)
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
	sprite = Sprite[Sprite.Num() - res];

// zru�en� lok�ln�ch prom�nn�ch
	FCommand();
}


/***************************************************************************\
*																			*
*								prom�nn�									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// pr�zdn� sprajt

void _fastcall FSpriteEmpty(CSprite& sprite) { sprite.Empty(); };


/////////////////////////////////////////////////////////////////////////////
// glob�ln� sprajt (Data = index)

void _fastcall FSpriteObj(CSprite& sprite)
{
// index sprajtu
	int inx = ExecItem[-1].Data;

// nastaven� sprajtu
	sprite = Sprite[inx];
}


/////////////////////////////////////////////////////////////////////////////
// globaln� sprajt v seznamu (Data = index, List = seznam)

void _fastcall FSpriteObjList(CSprite& sprite)
{
// index seznamu
	int list = ExecItem[-1].List;

// index sprajtu, inkrementace indexu seznamu
	int inx = ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// nastaven� sprajtu
	sprite = Sprite[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lok�ln� sprajt (Data = index)

void _fastcall FSpriteLoc(CSprite& sprite)
{
// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;

// nastaven� sprajtu
	sprite = Sprite[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lok�ln� sprajt v seznamu (Data = index, List = seznam)

void _fastcall FSpriteLocList(CSprite& sprite)
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index sprajtu, inkrementace indexu seznamu
	int inx = Sprite.Num() - ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// nastaven� sprajtu
	sprite = Sprite[inx];
}

/////////////////////////////////////////////////////////////////////////////
// na�ten� sprajtu ze souboru

void _fastcall FGetFileSprite(CSprite& sprite)
{
	sprite.Empty();
	sprite.LoadFile();
}
