
#include "Main.h"

/***************************************************************************\
*																			*
*						Provádìní programu - sprajt							*
*																			*
\***************************************************************************/

#pragma optimize("t", on)			// optimalizace na maximální rychlost


/***************************************************************************\
*																			*
*									funkce									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// funkce s návratem sprajtu

void _fastcall FSpriteFunc(CSprite& sprite)
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
	sprite = Sprite[Sprite.Num() - res];

// zrušení lokálních promìnných
	FCommand();
}


/***************************************************************************\
*																			*
*								promìnné									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// prázdný sprajt

void _fastcall FSpriteEmpty(CSprite& sprite) { sprite.Empty(); };


/////////////////////////////////////////////////////////////////////////////
// globální sprajt (Data = index)

void _fastcall FSpriteObj(CSprite& sprite)
{
// index sprajtu
	int inx = ExecItem[-1].Data;

// nastavení sprajtu
	sprite = Sprite[inx];
}


/////////////////////////////////////////////////////////////////////////////
// globalní sprajt v seznamu (Data = index, List = seznam)

void _fastcall FSpriteObjList(CSprite& sprite)
{
// index seznamu
	int list = ExecItem[-1].List;

// index sprajtu, inkrementace indexu seznamu
	int inx = ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// nastavení sprajtu
	sprite = Sprite[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lokální sprajt (Data = index)

void _fastcall FSpriteLoc(CSprite& sprite)
{
// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;

// nastavení sprajtu
	sprite = Sprite[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lokální sprajt v seznamu (Data = index, List = seznam)

void _fastcall FSpriteLocList(CSprite& sprite)
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index sprajtu, inkrementace indexu seznamu
	int inx = Sprite.Num() - ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// nastavení sprajtu
	sprite = Sprite[inx];
}

/////////////////////////////////////////////////////////////////////////////
// naètení sprajtu ze souboru

void _fastcall FGetFileSprite(CSprite& sprite)
{
	sprite.Empty();
	sprite.LoadFile();
}
