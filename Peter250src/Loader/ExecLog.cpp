
#include "Main.h"

/***************************************************************************\
*																			*
*						Provádìní programu - logický výraz					*
*																			*
\***************************************************************************/

#pragma optimize("t", on)			// optimalizace na maximální rychlost


/***************************************************************************\
*																			*
*									funkce									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// funkce s návratem logické hodnoty

bool FBoolFunc()
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
	bool result = Bool[Bool.Num() - res];

// zrušení lokálních promìnných
	FCommand();

	return result;
}


/***************************************************************************\
*																			*
*								promìnné									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// konstanta TRUE a FALSE

bool FTrue() { return true; };
bool FFalse() { return false; };


/////////////////////////////////////////////////////////////////////////////
// globální promìnná (Data = index)

bool FBoolObj()
{
// index
	int inx = ExecItem[-1].Data;

// data
	return Bool[inx];
}


/////////////////////////////////////////////////////////////////////////////
// globalní promìnná v seznamu (Data = index, List = seznam)

bool FBoolObjList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index promìnné, inkrementace indexu seznamu
	int inx = ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// data
	return Bool[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lokální promìnná (Data = index)

bool FBoolLoc()
{
// index promìnné
	int inx = Bool.Num() - ExecItem[-1].Data;

// data
	return Bool[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lokální promìnná v seznamu (Data = index, List = seznam)

bool FBoolLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index promìnné, inkrementace indexu seznamu
	int inx = Bool.Num() - ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// data
	return Bool[inx];
}


/////////////////////////////////////////////////////////////////////////////
// pohyb globálního sprajtu (Data = index)

bool FGetSpriteMove()
{
// index sprajtu
	int inx = ExecItem[-1].Data;

// pohyb sprajtu
	return (Sprite[inx].KrokCit() > 0);
}


/////////////////////////////////////////////////////////////////////////////
// pohyb sprajtu v seznamu (Data = index, List = seznam)

bool FGetSpriteMoveList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index sprajtu, inkrementace indexu seznamu
	int inx = ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// pohyb sprajtu
	return (Sprite[inx].KrokCit() > 0);
}


/////////////////////////////////////////////////////////////////////////////
// pohyb lokálního sprajtu (Data = index)

bool FGetSpriteMoveLoc()
{
// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;

// pohyb sprajtu
	return (Sprite[inx].KrokCit() > 0);
}


/////////////////////////////////////////////////////////////////////////////
// pohyb lokálního sprajtu v seznamu (Data = index, List = seznam)

bool FGetSpriteMoveLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index sprajtu, inkrementace indexu seznamu
	int inx = Sprite.Num() - ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// pohyb sprajtu
	return (Sprite[inx].KrokCit() > 0);
}


/////////////////////////////////////////////////////////////////////////////
// viditelnost globálního sprajtu (Data = index)

bool FGetSpriteVis()
{
// index sprajtu
	int inx = ExecItem[-1].Data;

// viditelnost sprajtu
	return Sprite[inx].Visible();
}


/////////////////////////////////////////////////////////////////////////////
// viditelnost sprajtu v seznamu (Data = index, List = seznam)

bool FGetSpriteVisList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index sprajtu, inkrementace indexu seznamu
	int inx = ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// viditelnost sprajtu
	return Sprite[inx].Visible();
}


/////////////////////////////////////////////////////////////////////////////
// viditelnost lokálního sprajtu (Data = index)

bool FGetSpriteVisLoc()
{
// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;

// viditelnost sprajtu
	return Sprite[inx].Visible();
}


/////////////////////////////////////////////////////////////////////////////
// viditelnost lokálního sprajtu v seznamu (Data = index, List = seznam)

bool FGetSpriteVisLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index sprajtu, inkrementace indexu seznamu
	int inx = Sprite.Num() - ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// viditelnost sprajtu
	return Sprite[inx].Visible();
}


/***************************************************************************\
*																			*
*							matematické operátory							*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// operátor EQU - pro 1 prvek (porovná èíslo s nulou)

bool FEqu1() { return FNum() == 0; };


/////////////////////////////////////////////////////////////////////////////
// operátor EQU (Data = poèet prvkù - 1)

bool FEqu()
{
// pøíprava poètu prvkù
	int i = ExecItem[-1].Data;

// naètení prvního èísla
	double num = FNum();

// cyklus pøes další èísla
	for (; i > 0; i--)
	{

// porovnání dalšího èísla
		if (FNum() != num)
		{

// nevyhovuje - zrychlené ukonèení operace
			if (i > 1) ExecItem += ExecItem->Jump;
			return false;
		}
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// operátor NEQU - pro 1 prvek (porovná èíslo s nulou)

bool FNEqu1() { return FNum() != 0; };


/////////////////////////////////////////////////////////////////////////////
// operátor NEQU (Data = poèet prvkù - 1)

bool FNEqu()
{
// pøíprava poètu prvkù
	int i = ExecItem[-1].Data;

// naètení prvního èísla
	double num1 = FNum();
	double num2;

// cyklus pøes další èísla
	for (; i > 0; i--)
	{

// naètení dalšího èísla
		num2 = FNum();

// porovnání dalšího èísla
		if (num2 == num1)
		{

// nevyhovuje - zrychlené ukonèení operace
			if (i > 1) ExecItem += ExecItem->Jump;
			return false;
		}

// úschova èísla pro pøíští operaci
		num1 = num2;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// operátor EQGR - pro 1 prvek (porovná èíslo s nulou)

bool FEqGr1() { return FNum() >= 0; };


/////////////////////////////////////////////////////////////////////////////
// operátor EQGR (Data = poèet prvkù - 1)

bool FEqGr()
{
// pøíprava poètu prvkù
	int i = ExecItem[-1].Data;

// naètení prvního èísla
	double num1 = FNum();
	double num2;

// cyklus pøes další èísla
	for (; i > 0; i--)
	{

// naètení dalšího èísla
		num2 = FNum();

// porovnání dalšího èísla
		if (num1 < num2)
		{

// nevyhovuje - zrychlené ukonèení operace
			if (i > 1) ExecItem += ExecItem->Jump;
			return false;
		}

// úschova èísla pro pøíští operaci
		num1 = num2;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// operátor EQLT - pro 1 prvek (porovná èíslo s nulou)

bool FEqLt1() { return FNum() <= 0; };


/////////////////////////////////////////////////////////////////////////////
// operátor EQLT (Data = poèet prvkù - 1)

bool FEqLt()
{
// pøíprava poètu prvkù
	int i = ExecItem[-1].Data;

// naètení prvního èísla
	double num1 = FNum();
	double num2;

// cyklus pøes další èísla
	for (; i > 0; i--)
	{

// naètení dalšího èísla
		num2 = FNum();

// porovnání dalšího èísla
		if (num1 > num2)
		{

// nevyhovuje - zrychlené ukonèení operace
			if (i > 1) ExecItem += ExecItem->Jump;
			return false;
		}

// úschova èísla pro pøíští operaci
		num1 = num2;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// operátor GR - pro 1 prvek (porovná èíslo s nulou)

bool FGr1() { return FNum() > 0; };


/////////////////////////////////////////////////////////////////////////////
// operátor GR (Data = poèet prvkù - 1)

bool FGr()
{
// pøíprava poètu prvkù
	int i = ExecItem[-1].Data;

// naètení prvního èísla
	double num1 = FNum();
	double num2;

// cyklus pøes další èísla
	for (; i > 0; i--)
	{

// naètení dalšího èísla
		num2 = FNum();

// porovnání dalšího èísla
		if (num1 <= num2)
		{

// nevyhovuje - zrychlené ukonèení operace
			if (i > 1) ExecItem += ExecItem->Jump;
			return false;
		}

// úschova èísla pro pøíští operaci
		num1 = num2;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// operátor LT - pro 1 prvek (porovná èíslo s nulou)

bool FLt1() { return FNum() < 0; };


/////////////////////////////////////////////////////////////////////////////
// operátor LT (Data = poèet prvkù - 1)

bool FLt()
{
// pøíprava poètu prvkù
	int i = ExecItem[-1].Data;

// naètení prvního èísla
	double num1 = FNum();
	double num2;

// cyklus pøes další èísla
	for (; i > 0; i--)
	{

// naètení dalšího èísla
		num2 = FNum();

// porovnání dalšího èísla
		if (num1 >= num2)
		{

// nevyhovuje - zrychlené ukonèení operace
			if (i > 1) ExecItem += ExecItem->Jump;
			return false;
		}

// úschova èísla pro pøíští operaci
		num1 = num2;
	}
	return true;
}


/***************************************************************************\
*																			*
*								logické operátory							*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// operátor AND - pro 1 prvek (jenom vrátí hodnotu beze zmìny)

bool FAnd1() { return FBool(); }


/////////////////////////////////////////////////////////////////////////////
// operátor AND (Data = poèet prvkù - 1)

bool FAnd()
{
// pøíprava poètu prvkù
	int i = ExecItem[-1].Data;

// cyklus pøes všechny prvky
	for (; i >= 0; i--)
	{

// kontrola prvku
		if (!FBool())
		{

// nevyhovuje - zrychlené ukonèení operace
			if (i > 0) ExecItem += ExecItem->Jump;
			return false;
		}
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// operátor OR (Data = poèet prvkù - 1)

bool FOr()
{
// pøíprava poètu prvkù
	int i = ExecItem[-1].Data;

// cyklus pøes všechny prvky
	for (; i >= 0; i--)
	{

// kontrola prvku
		if (FBool())
		{

// vyhovuje - zrychlené ukonèení operace
			if (i > 0) ExecItem += ExecItem->Jump;
			return true;
		}
	}
	return false;
}


/////////////////////////////////////////////////////////////////////////////
// operátor XOR (Data = poèet prvkù - 1)

bool FXor()
{
// pøíprava poètu prvkù
	int i = ExecItem[-1].Data;

// støadaè operace
	bool result = false;

// cyklus pøes všechny prvky
	for (; i >= 0; i--)
	{

// naètení prvku
		result ^= FBool();
	}
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// operátor NOT

bool FNot() 
{
	bool result = FBool();
	result ^= true ^ false;
	return result;
}


/***************************************************************************\
*																			*
*								obsluha ikon								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// porovnání jedné ikony, zda je prázdná

bool FCompEqIco1()
{
	CIcon icon;
	FIcon(icon);
	return (icon.IsEmpty() != FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// porovnání shodnosti ikon (Data = poèet prvkù - 1)

bool FCompEqIco()
{
// pøíprava poètu prvkù
	int i = ExecItem[-1].Data;

// naètení první ikony
	CIcon icon1;
	FIcon(icon1);
	CIcon icon2;

// cyklus pøes další ikony
	for (; i > 0; i--)
	{

// naètení další ikony
		FIcon(icon2);

// porovnání ikon
		if (icon1 != icon2)
		{

// nevyhovuje - zrychlené ukonèení operace
			if (i > 1) ExecItem += ExecItem->Jump;
			return false;
		}

// ikona 2 se uschová pro pøíští použití
//		icon1 = icon2;
	}
	return true;
}







/////////////////////////////////////////////////////////////////////////////
// porovnání jedné plochy, zda je prázdná

bool FCompEqMap1()
{
	CMap map;
	FMap(map);
	return (map.IsEmpty() != FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// porovnání shodnosti ploch (Data = poèet prvkù - 1)

bool FCompEqMap()
{
// pøíprava poètu prvkù
	int i = ExecItem[-1].Data;

// naètení první plochy
	CMap map1;
	FMap(map1);
	CMap map2;

// cyklus pøes další plochy
	for (; i > 0; i--)
	{

// naètení další plochy
		FMap(map2);

// porovnání ploch
		if (map1 != map2)
		{

// nevyhovuje - zrychlené ukonèení operace
			if (i > 1) ExecItem += ExecItem->Jump;
			return false;
		}

// plocha 2 se uschová pro pøíští použití
//		map1 = map2;
	}
	return true;
}



/////////////////////////////////////////////////////////////////////////////
// porovnání jednoho obrázku, zda je prázdný

bool FCompEqPic1()
{
	CPicture pic;
	FPicture(pic);
	return (pic.IsEmpty() != FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// porovnání shodnosti obrázkù (Data = poèet prvkù - 1)

bool FCompEqPic()
{
// pøíprava poètu prvkù
	int i = ExecItem[-1].Data;

// naètení prvního obrázku
	CPicture pic1;
	FPicture(pic1);
	CPicture pic2;

// cyklus pøes další obrázky
	for (; i > 0; i--)
	{

// naètení dalšího obrázku
		FPicture(pic2);

// porovnání obrázkù
		if (pic1 != pic2)
		{

// nevyhovuje - zrychlené ukonèení operace
			if (i > 1) ExecItem += ExecItem->Jump;
			return false;
		}

// obrázek 2 se uschová pro pøíští použití
//		pic1 = pic2;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// porovnání jednoho sprajtu, zda je prázdný

bool FCompEqSpr1()
{
	CSprite spr;
	FSprite(spr);
	return (spr.IsEmpty() != FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// porovnání shodnosti sprajtù (Data = poèet prvkù - 1)

bool FCompEqSpr()
{
// pøíprava poètu prvkù
	int i = ExecItem[-1].Data;

// naètení prvního sprajtu
	CSprite spr1;
	FSprite(spr1);
	CSprite spr2;

// cyklus pøes další sprajty
	for (; i > 0; i--)
	{

// naètení dalšího sprajtu
		FSprite(spr2);

// porovnání sprajtù
		if (spr1 != spr2)
		{

// nevyhovuje - zrychlené ukonèení operace
			if (i > 1) ExecItem += ExecItem->Jump;
			return false;
		}

// sprajt 2 se uschová pro pøíští použití
//		spr1 = spr2;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// porovnání jednoho zvuku, zda je prázdný

bool FCompEqSnd1()
{
	CSound snd;
	FSound(snd);
	return (snd.IsEmpty() != FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// porovnání shodnosti zvukù (Data = poèet prvkù - 1)

bool FCompEqSnd()
{
// pøíprava poètu prvkù
	int i = ExecItem[-1].Data;

// naètení prvního zvuku
	CSound snd1;
	FSound(snd1);
	CSound snd2;

// cyklus pøes další zvuky
	for (; i > 0; i--)
	{

// naètení dalšího zvuku
		FSound(snd2);

// porovnání zvukù
		if (snd1 != snd2)
		{

// nevyhovuje - zrychlené ukonèení operace
			if (i > 1) ExecItem += ExecItem->Jump;
			return false;
		}

// zvuk 2 se uschová pro pøíští použití
//		snd1 = snd2;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// porovnání jedné hudby, zda je prázdná

bool FCompEqMus1()
{
	CMusic mus;
	FMusic(mus);
	return (mus.IsEmpty() != FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// porovnání shodnosti hudeb (Data = poèet prvkù - 1)

bool FCompEqMus()
{
// pøíprava poètu prvkù
	int i = ExecItem[-1].Data;

// naètení první hudby
	CMusic mus1;
	FMusic(mus1);
	CMusic mus2;

// cyklus pøes další hudby
	for (; i > 0; i--)
	{

// naètení další hudby
		FMusic(mus2);

// porovnání hudeb
		if (mus1 != mus2)
		{

// nevyhovuje - zrychlené ukonèení operace
			if (i > 1) ExecItem += ExecItem->Jump;
			return false;
		}

// hudba 2 se uschová pro pøíští použití
//		mus1 = mus2;
	}
	return true;
}


/***************************************************************************\
*																			*
*								obsluha textù								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// porovnání jednoho textu, zda je prázdný

bool FCompEqTxt1()
{
	CString txt;
	FText(txt);
	return (txt.IsEmpty() != FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// porovnání textù na shodnost (Data = poèet textù - 1)

bool FCompEqTxt()
{
// pøíprava poètu prvkù
	int i = ExecItem[-1].Data;

// naètení prvního textu
	CString txt1;
	FText(txt1);
	CString txt2;

// cyklus pøes další texty
	for (; i > 0; i--)
	{

// naètení dalšího textu
		FText(txt2);

// porovnání textù
		if (txt1 != txt2)
		{

// nevyhovuje - zrychlené ukonèení operace
			if (i > 1) ExecItem += ExecItem->Jump;
			return false;
		}

// text 2 se uschová pro pøíští použití
//		txt1 = txt2;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// porovnání jednoho textu, zda je neprázdný

bool FCompGtTxt1()
{
	CString txt;
	FText(txt);
	return (txt.IsNotEmpty() != FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// porovnání textù na neshodnost (Data = poèet textù - 1)

bool FCompGtTxt()
{
// pøíprava poètu prvkù
	int i = ExecItem[-1].Data;

// naètení prvního textu
	CString txt1;
	FText(txt1);
	CString txt2;

// cyklus pøes další texty
	for (; i > 0; i--)
	{

// naètení dalšího textu
		FText(txt2);

// porovnání textù
		if (txt1 <= txt2)
		{

// nevyhovuje - zrychlené ukonèení operace
			if (i > 1) ExecItem += ExecItem->Jump;
			return false;
		}

// text 2 se uschová pro pøíští použití
		txt1 = txt2;
	}
	return true;
}


/***************************************************************************\
*																			*
*							obsluha Petra a Petry							*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// naètení viditelnosti Petra a Petry

bool FGetVisible() { return Sprite[0].Visible(); }
bool FGetVisible2() { return Sprite[1].Visible(); }


/////////////////////////////////////////////////////////////////////////////
// naètení pøepínaèe Petra

bool _fastcall FGetSwc(DWORD mask)
{
// naètení pozice Petra
	MAPITEM* item = PetrXYMap();

// zjištìní pøepínaèe, je-li platná pozice
	if (item != NULL)
	{
		return ((item->Param & mask) != 0);
	}
	return false;
}

bool FGetSwc1() { return FGetSwc(MAP_FLAG1); };		// pøepínaè 1 - Petr
bool FGetSwc2() { return FGetSwc(MAP_FLAG2); };		// pøepínaè 2 - Petr
bool FGetSwc3() { return FGetSwc(MAP_FLAG3); };		// pøepínaè 3 - Petr
bool FGetSwc4() { return FGetSwc(MAP_FLAG4); };		// pøepínaè 4 - Petr
bool FGetSwc5() { return FGetSwc(MAP_FLAG5); };		// pøepínaè 5 - Petr

/////////////////////////////////////////////////////////////////////////////
// naètení pøepínaèe Petry

bool _fastcall FGet2Swc(DWORD mask)
{
// naètení pozice Petry
	MAPITEM* item = Petr2XYMap();

// zjištìní pøepínaèe, je-li platná pozice
	if (item != NULL)
	{
		return ((item->Param & mask) != 0);
	}
	return false;
}

bool FGetSwc12() { return FGet2Swc(MAP_FLAG1); };	// pøepínaè 1 - Petra
bool FGetSwc22() { return FGet2Swc(MAP_FLAG2); };	// pøepínaè 2 - Petra
bool FGetSwc32() { return FGet2Swc(MAP_FLAG3); };	// pøepínaè 3 - Petra
bool FGetSwc42() { return FGet2Swc(MAP_FLAG4); };	// pøepínaè 4 - Petra
bool FGetSwc52() { return FGet2Swc(MAP_FLAG5); };	// pøepínaè 5 - Petra


/***************************************************************************\
*																			*
*							obsluha klávesnice								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// test stisku klávesy

bool FKeyTest()
{
// naètení kódu klávesy
	int klav = FIntR();

// kód klávesy Pause
	if (klav == VK_PAUSE) return Pause;

// kontrola platnosti kódu klávesy
	if ((DWORD)klav >= (DWORD)KEYMAPSIZE) return false;

// test žádné klávesy
	if (klav == 0) return ((KeyMapNum == 0) && !Pause);

// test klávesy
	return KeyMap[klav];
}


/////////////////////////////////////////////////////////////////////////////
// test stavu Num Lock

bool FStatusNumLock() { return NumLock; }


/////////////////////////////////////////////////////////////////////////////
// test stavu Caps Lock

bool FStatusCapsLock() { return CapsLock; }


/////////////////////////////////////////////////////////////////////////////
// test stavu Scroll Lock

bool FStatusScrollLock() { return ScrollLock; }


/////////////////////////////////////////////////////////////////////////////
// test stavu Insert

bool FStatusInsert() { return InsertLock; }


/***************************************************************************\
*																			*
*							obsluha myši									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// test stisku levého tlaèítka myši

bool FMouseL() { return LMouseDown; }


/////////////////////////////////////////////////////////////////////////////
// test stisku pravého tlaèítka myši

bool FMouseR() { return RMouseDown; }


/////////////////////////////////////////////////////////////////////////////
// test kliku levým tlaèítkem myši

bool FMouseLClick()
{
	bool result = LMouseClick;
	LMouseClick = false;
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// test kliku pravým tlaèítkem myši

bool FMouseRClick()
{
	bool result = RMouseClick;
	RMouseClick = false;
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// test dvojkliku levým tlaèítkem myši

bool FMouseLDClick()
{
	bool result = LMouseDClick;
	LMouseDClick = false;
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// test dvojkliku pravým tlaèítkem myši

bool FMouseRDClick()
{
	bool result = RMouseDClick;
	RMouseDClick = false;
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// test myši v oblasti

bool FMouseRegion()
{
// naètení rozmìrù regionu
	double x1 = FNum();				// poèátek X
	double y1 = FNum();				// poèátek Y
	double x2 = FNum() + x1;		// konec X
	double y2 = FNum() + y1;		// konec Y

// porovnání oblasti
	return ((MouseX >= x1) && (MouseX < x2) && (MouseY >= y1) && (MouseY < y2));
}

/////////////////////////////////////////////////////////////////////////////
// test myši v dialogovém prvku

bool FMouseItem()
{
// naètení èísla prvku
	int id = FIntR();

// test, zda je platný prvek okna
	if (Win.IsValid(id))
	{
		WINITEM* item = &Win[id];

// pøíprava výšky rodièe
		int height;
		if ((item->Typ == WINITEM_WINDOW) || (item->Typ == WINITEM_WINDOWRO))
		{
			height = ScreenHeight0;
		}
		else
		{
			height = Win[item->Parent].ClientHeight;
		}

// pøíprava rozmìrù regionu
		double x1 = (double)item->Left/ICONWIDTH;							// poèátek X
		double y1 = (double)(height - item->Top - item->Height)/ICONHEIGHT;	// poèátek Y
		double x2 = (double)item->Width/ICONWIDTH + x1;						// konec X
		double y2 = (double)item->Height/ICONHEIGHT + y1;					// konec Y

// porovnání oblasti
		return ((MouseX >= x1) && (MouseX < x2) && (MouseY >= y1) && (MouseY < y2));
	}
	else
	{
		return (id < 0);
	}
}

/***************************************************************************\
*																			*
*							obsluha zvuku									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// test pøehrávání jakéhokoliv zvuku

bool FSoundTest()
{
	return ((WaveOut != NULL) || (SoundKanalu > 0));
}


/////////////////////////////////////////////////////////////////////////////
// test pøehrávání konkrétního zvuku

bool FSoundTest1()
{
// naètení testovaného zvuku
	CSound sound;
	FSound(sound);

// nestandardní formát
	if (!WaveFormatPCM)
	{
		return ((WaveOut != NULL) && (sound == Sound2));
	}

// nalezení zvuku v bufferu
	for (int i = 0; i < MAXSOUNDKANALU; i++)
	{
		if ((SoundChan[i].Num > 0) && (SoundChan[i].Sound == sound) && (SoundChan[i].SoundID == SoundID))
		{
			return true;
		}
	}
	return false;
}


/////////////////////////////////////////////////////////////////////////////
// pøíznak stereo

bool FGetSoundStereo()
{
	CSound snd;
	FSound(snd);
	return (snd.Channels() == 2);
}


/////////////////////////////////////////////////////////////////////////////
// test zapauzování zvuku

bool FSoundTestPause()
{
	return SoundPausing;
}


/***************************************************************************\
*																			*
*							obsluha hudby									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// test pøehrávání hudby

bool FMusicTest()
{
	return MusicPlaing;
}


/////////////////////////////////////////////////////////////////////////////
// test zapauzování hudby

bool FMusicTestPause()
{
	return MusicPausing;
}


/***************************************************************************\
*																			*
*							obsluha CD disku								*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// test pauzy CD

bool FCDGetPause()
{
// aktualizace údajù o CD
	CDAkt();

// pøíznak pauzy CD
	return CDPausing;
}


/////////////////////////////////////////////////////////////////////////////
// test pøehrávání CD

bool FCDTest()
{
// aktualizace údajù o CD
	CDAkt();

// pøíznak pøehrávání CD
	return CDPlaing;
}


/////////////////////////////////////////////////////////////////////////////
// test vysunutí CD

bool FCDGetEject()
{
// aktualizace údajù o CD
	CDAkt();

// pøíznak vysunutí CD
	return !CDDiskValid;
}


/***************************************************************************\
*																			*
*							obsluha souborù									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// vytvoøení souboru

bool FBoolFileNew()
{
// uzavøení souborù
	FileClose();

// naètení jména adresáøe
	CString jmeno;
	FFileName(jmeno);

// jméno musí být zadáno a nesmí konèit znakem "\"
	if (jmeno.IsNotEmpty() && (jmeno[jmeno.Length()-1] != '\\'))
	{

// první pokus o vytvoøení souboru
		HANDLE file = ::CreateFile(jmeno, GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL);

		if (file != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(file);
			return true;
		}

// kontrola, zda soubor již existuje
		DWORD atr = ::GetFileAttributes(jmeno);
		if ((int)atr != -1)
		{
			if ((atr & FILE_ATTRIBUTE_DIRECTORY) == 0) return true;
		}
		else
		{

// vytvoøení cesty k souboru
			int prev = jmeno.RevFind('\\');
			if (prev > 0)
			{
				if (CreatePath(jmeno.Left(prev)))
				{

// druhý pokus o vytvoøení souboru
					file = ::CreateFile(jmeno, GENERIC_READ | GENERIC_WRITE,
						FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
						FILE_ATTRIBUTE_NORMAL, NULL);

					if (file != INVALID_HANDLE_VALUE)
					{
						::CloseHandle(file);
						return true;
					}
				}
			}
		}
	}

// chyba operace
	FileError = true;
	return false;
}


/////////////////////////////////////////////////////////////////////////////
// vytvoøení adresáøe

bool FBoolDirNew()
{
// uzavøení souborù
	FileClose();

// naètení jména adresáøe
	CString jmeno;
	FFileName(jmeno);

// vytvoøení adresáøe
	if (jmeno.IsEmpty() || !CreatePath(jmeno))
	{
		FileError = true;
		return false;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// zrušení souboru/adresáøe

bool FBoolFileDelete()
{
// uzavøení souborù
	FileClose();

// naètení jména souboru/adresáøe
	CString jmeno;
	FFileName(jmeno);

// zrušení souboru nebo adresáøe
	if (jmeno.IsEmpty() ||
		(!::DeleteFile(jmeno) &&
		!::RemoveDirectory(jmeno)))
	{
		
		FileError = true;
		return false;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// test existence disku/souboru/adresáøe

bool FFileTest()
{
// naètení jména disku/adresáøe/souboru
	CString jmeno;
	FFileName(jmeno);

// délka jména
	int delka = jmeno.Length();
	if (delka <= 0) return false;

// kontroluje se jenom disk, pøíp. root
	if (((delka == 2) && (jmeno[1] == ':')) ||
		((delka == 3) && (jmeno[1] == ':') && (jmeno[2] == '\\')))
	{
		TCHAR disk = jmeno[0];
		if ((disk >= 'a') && (disk < 'a'+32)) disk = (TCHAR)(disk + 'A' - 'a');
		if ((disk < 'A') || (disk >= 'A'+32)) return false;

		disk -= 'A';
		if ((::GetLogicalDrives() & (1 << disk)) == 0) return false;

		if (delka < 3) return true;

		DWORD comp;
		DWORD syst;
		TCHAR name[20];

		return (::GetVolumeInformation(jmeno, NULL, 0, NULL,
			&comp, &syst, name, 20) != FALSE);
	}

// kontroluje se adresáø/soubor
	WIN32_FIND_DATA wfd;
	HANDLE find = ::FindFirstFile(jmeno, &wfd);
	if (find == INVALID_HANDLE_VALUE) return false;
	::FindClose(find);
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// kopie souborù

bool FBoolFileCopy()
{
// uzavøení otevøených souborù
	FileClose();

// jména obou souborù musí být platná
	if (FileRead.IsNotEmpty() && FileWrite.IsNotEmpty())
	{

// nulování ukazatelù v souborech (kdyby se zmìnily)
		FileReadOff = 0;
		FileWriteOff = 0;

// provedení kopie souborù
		if (::CopyFile(FileRead, FileWrite, TRUE)) return true;
	}

// chyba operace
	FileError = true;
	return false;
}


/////////////////////////////////////////////////////////////////////////////
// pøesun souborù

bool FBoolFileMove()
{
// uzavøení otevøených souborù
	FileClose();

// jména obou souborù musí být platná
	if (FileRead.IsNotEmpty() && FileWrite.IsNotEmpty())
	{

// nulování ukazatelù v souborech (kdyby se zmìnily)
		FileReadOff = 0;
		FileWriteOff = 0;

// provedení pøesunu souborù
		if (::MoveFile(FileRead, FileWrite)) return true;
	}

// chyba operace
	FileError = true;
	return false;
}


/////////////////////////////////////////////////////////////////////////////
// naètení atributu souboru

bool _fastcall FGetFileAttrib(DWORD attrib)
{
// uzavøení otevøených souborù
	FileClose();

// musí být nìjaké jméno souboru
	if (FileRead.IsNotEmpty())
	{

// naètení atributù souboru
		DWORD atr = ::GetFileAttributes(FileRead);
		if ((int)atr != -1) return ((atr & attrib) != 0);
	}

// chyba operace
	FileError = true;
	return false;
}

bool FGetFileRO()	{ return FGetFileAttrib(FILE_ATTRIBUTE_READONLY); }
bool FGetFileHID()	{ return FGetFileAttrib(FILE_ATTRIBUTE_HIDDEN); }
bool FGetFileSYS()	{ return FGetFileAttrib(FILE_ATTRIBUTE_SYSTEM); }
bool FGetFileDIR()	{ return FGetFileAttrib(FILE_ATTRIBUTE_DIRECTORY); }
bool FGetFileARC()	{ return FGetFileAttrib(FILE_ATTRIBUTE_ARCHIVE); }
bool FGetFileTMP()	{ return FGetFileAttrib(FILE_ATTRIBUTE_TEMPORARY); }
bool FGetFileCMP()	{ return FGetFileAttrib(FILE_ATTRIBUTE_COMPRESSED); }
bool FGetFileOFF()	{ return FGetFileAttrib(FILE_ATTRIBUTE_OFFLINE); }


/////////////////////////////////////////////////////////////////////////////
// naètení logické hodnoty

bool FGetFileLogic() { return (FileReadByte() != 0); }


/////////////////////////////////////////////////////////////////////////////
// poskytnutí pøíznaku chyby souborù

bool FGetFileError() { return FileError; }


/////////////////////////////////////////////////////////////////////////////
// poskytnutí pøíznaku textu UNICODE

bool FGetFileTextUnicode() { return FileTextUnicode; }


/***************************************************************************\
*																			*
*								obsluha plochy								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// naètení pøepínaèe z plochy

bool _fastcall FGetMapSwc(DWORD mask)
{
// naètení plochy
	CMap map;
	FMap(map);

// naètení souøadnic
	int x = FIntR();
	int y = FIntR();

// pøepínaè na pozici
	if (((DWORD)x < (DWORD)map.Width()) && ((DWORD)y < (DWORD)map.Height()))
	{
		return ((map.At(x, y).Param & mask) != 0);
	}
	return false;
}

bool FGetMapSwc1() { return FGetMapSwc(MAP_FLAG1); };		// pøepínaè 1
bool FGetMapSwc2() { return FGetMapSwc(MAP_FLAG2); };		// pøepínaè 2
bool FGetMapSwc3() { return FGetMapSwc(MAP_FLAG3); };		// pøepínaè 3
bool FGetMapSwc4() { return FGetMapSwc(MAP_FLAG4); };		// pøepínaè 4
bool FGetMapSwc5() { return FGetMapSwc(MAP_FLAG5); };		// pøepínaè 5


/***************************************************************************\
*																			*
*									konzola									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// režim konzoly

bool FGetConsoleOn() { return ConsoleOn; }


/***************************************************************************\
*																			*
*									joystick								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// tlaèítko 1

bool FJoystick1()
{
	int inx = FIntR();
	if ((DWORD)inx >= (DWORD)MAXJOYSTICK) return false;
	AktJoystick(inx);
	return Joystick1[inx];
}

/////////////////////////////////////////////////////////////////////////////
// tlaèítko 2

bool FJoystick2()
{
	int inx = FIntR();
	if ((DWORD)inx >= (DWORD)MAXJOYSTICK) return false;
	AktJoystick(inx);
	return Joystick2[inx];
}

/////////////////////////////////////////////////////////////////////////////
// tlaèítko 3

bool FJoystick3()
{
	int inx = FIntR();
	if ((DWORD)inx >= (DWORD)MAXJOYSTICK) return false;
	AktJoystick(inx);
	return Joystick3[inx];
}

/////////////////////////////////////////////////////////////////////////////
// tlaèítko 4

bool FJoystick4()
{
	int inx = FIntR();
	if ((DWORD)inx >= (DWORD)MAXJOYSTICK) return false;
	AktJoystick(inx);
	return Joystick4[inx];
}


/***************************************************************************\
*																			*
*									dialogy									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// viditelnost okna

bool FGetWindowVisible()
{
	if (Win.IsValid(WindowID))
	{
		return Win[WindowID].Show;
	}
	else
	{
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////
// zákaz okna

bool FGetWindowDisable()
{
	if (Win.IsValid(WindowID))
	{
		return Win[WindowID].Disable;
	}
	else
	{
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////
// kliknutí na tlaèítko

bool FButtonClick()
{
	if (Win.IsValid(WindowID))
	{
		bool click = Win[WindowID].Click;
		Win[WindowID].Click = false;
		return click;
	}
	else
	{
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////
// stav pøepínaèe

bool FGetCheckBoxOn()
{
	if (Win.IsValid(WindowID))
	{
		return Win[WindowID].Check;
	}
	else
	{
		return false;
	}
}

bool FGetButton3On()
{
	if (Win.IsValid(WindowID))
	{
		return Win[WindowID].Check3;
	}
	else
	{
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////
// zvýraznìné písmo

bool FGetFontBold()
{
	if (Win.IsValid(WindowID))
	{
		return Win[WindowID].Bold;
	}
	else
	{
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////
// písmo s pevnou rozteèí

bool FGetFontFixed()
{
	if (Win.IsValid(WindowID))
	{
		return Win[WindowID].Fixed;
	}
	else
	{
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////
// šikmé písmo

bool FGetFontItalic()
{
	if (Win.IsValid(WindowID))
	{
		return Win[WindowID].Italic;
	}
	else
	{
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////
// podtržené písmo

bool FGetFontUnder()
{
	if (Win.IsValid(WindowID))
	{
		return Win[WindowID].Underline;
	}
	else
	{
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////
// pøeškrtnuté písmo

bool FGetFontStrike()
{
	if (Win.IsValid(WindowID))
	{
		return Win[WindowID].Strikeout;
	}
	else
	{
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////
// patièkové písmo

bool FGetFontSerif()
{
	if (Win.IsValid(WindowID))
	{
		return Win[WindowID].Serif;
	}
	else
	{
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////
// celoobrazovkový režim

bool FGetFullScreen()
{
	return FullScreen;
}

/////////////////////////////////////////////////////////////////////////////
// naètení pøepínaèe rámeèku okna

bool FGetWindowFrame()
{
	if (Win.IsValid(WindowID))
	{
		return Win[WindowID].HasBorder;
	}
	else
	{
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////
// naètení pøepínaèe nadpisu okna

bool FGetWindowCaption()
{
	if (Win.IsValid(WindowID))
	{
		return Win[WindowID].HasCaption;
	}
	else
	{
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////
// naètení pøepínaèe okna vždy nahoøe

bool FGetWindowTop()
{
	if (Win.IsValid(WindowID))
	{
		return Win[WindowID].AlwaysTop;
	}
	else
	{
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////
// naètení pøepínaèe okna mìnitelné velikosti

bool FGetWindowSize()
{
	if (Win.IsValid(WindowID))
	{
		return Win[WindowID].HasSize;
	}
	else
	{
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////
// požadavek o uzavøení okna

bool FGetWindowMayClose()
{
	bool reqclose = ReqClose;
	ReqClose = false;
	return reqclose;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní maximalizace okna

bool FGetWindowMaxim()
{
	return (::IsZoomed(Win[DialogParent].Wnd) != 0);
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní minimalizace okna

bool FGetWindowMinim()
{
	return (::IsIconic(Win[DialogParent].Wnd) != 0);
}

/***************************************************************************\
*																			*
*									DirectPlay								*
*																			*
\***************************************************************************/

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

/////////////////////////////////////////////////////////////////////////////
// zjištìní pøepínaèe hry 1

bool FGetGameL1()
{
	return (((WORD*)DirectPlayGamesParam)[0] >= 0x8000);
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní pøepínaèe hry 2

bool FGetGameL2()
{
	return (((WORD*)DirectPlayGamesParam)[1] >= 0x8000);
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní pøepínaèe hry 3

bool FGetGameL3()
{
	return (((WORD*)DirectPlayGamesParam)[2] >= 0x8000);
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní pøepínaèe hry 4

bool FGetGameL4()
{
	return (((WORD*)DirectPlayGamesParam)[3] >= 0x8000);
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní pøepínaèe hry 5

bool FGetGameL5()
{
	return (((WORD*)DirectPlayGamesParam)[4] >= 0x8000);
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní pøepínaèe hry 6

bool FGetGameL6()
{
	return (((WORD*)DirectPlayGamesParam)[5] >= 0x8000);
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní pøepínaèe hry 7

bool FGetGameL7()
{
	return (((WORD*)DirectPlayGamesParam)[6] >= 0x8000);
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní pøepínaèe hry 8

bool FGetGameL8()
{
	return (((WORD*)DirectPlayGamesParam)[7] >= 0x8000);
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní pøíznaku hostitele

bool FGameHost()
{
	return DirectPlayGamesHost;
}

/////////////////////////////////////////////////////////////////////////////
// naètení pøepínaèe z paketu

bool FGetGameDataL()
{
	BYTE log;
	DirectReceiveData(&log, 1);
	return (log != 0);
}

/***************************************************************************\
*																			*
*									Porty									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// zjištìní stopbitù

bool FGetComStop()
{
	COMITEM* item = Coms + ComAkt;
	if (item->File != INVALID_HANDLE_VALUE)
	{
		return (item->Dcb.StopBits != ONESTOPBIT);
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// test pøijatého znaku

bool FComIn()
{
	COMITEM* item = Coms + ComAkt;
	HANDLE file = item->File;

	if (file != INVALID_HANDLE_VALUE)
	{
		COMSTAT cs;
		DWORD flags;
		if (::ClearCommError(file, &flags, &cs))
		{
			if (cs.cbInQue > 0) return true;
		}
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// test povolení vysílání, protìjšek je pøipraven pøijímat data (signál CTS)

bool FGetComSend()
{
	COMITEM* item = Coms + ComAkt;
	HANDLE file = item->File;

	if (file != INVALID_HANDLE_VALUE)
	{
		DWORD flags;
		if (::GetCommModemStatus(file, &flags))
		{
			if (flags & MS_CTS_ON) return true;
		}
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// test povolení pøíjmu, data vysílaná od protìjšku jsou platná (signál DSR)

bool FGetComReceive()
{
	COMITEM* item = Coms + ComAkt;
	HANDLE file = item->File;

	if (file != INVALID_HANDLE_VALUE)
	{
		DWORD flags;
		if (::GetCommModemStatus(file, &flags))
		{
			if (flags & MS_DSR_ON) return true;
		}
	}
	return false;
}

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------

/////////////////////////////////////////////////////////////////////////////
// zjištìní módu grafického pozadí

bool FGetDialogGraph()
{
	return DialogGraph;
}

/***************************************************************************\
*																			*
*									Direct3D								*
*																			*
\***************************************************************************/

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

/////////////////////////////////////////////////////////////////////////////
// zjištìní nastavení vyplòování ploch

bool FGetD3DWireframe()
{
	return D3DWireframeGlobal;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní nastavení osvìtlení

bool FGetD3DLighton()
{
	return D3DLightOnGlobal;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní nastavení plynulého stínování

bool FGetD3DShades()
{
	return D3DShadesGlobal;
}
		   
/////////////////////////////////////////////////////////////////////////////
// zjištìní filtrace zmenšených textur

bool FGetD3DMinFilter()
{
	return D3DMinFilterGlobal;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní filtrace zvìtšených textur

bool FGetD3DMagFilter()
{
	return D3DMagFilterGlobal;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní filtrace vzdálených textur

bool FGetD3DMipFilter()
{
	return D3DMipFilterGlobal;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní nastavení vyplòování ploch objektu

bool FGetD3DOWireframe()
{
	if (IsValidID())
	{
		return GetID()->Wireframe;
	}
	return FGetD3DWireframe();
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní nastavení osvìtlení objektu

bool FGetD3DOLighton()
{
	if (IsValidID())
	{
		return GetID()->Lighton;
	}
	return FGetD3DLighton();
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní nastavení plynulého stínování objektu

bool FGetD3DOShades()
{
	if (IsValidID())
	{
		return GetID()->Shades;
	}
	return FGetD3DShades();
}
		   
/////////////////////////////////////////////////////////////////////////////
// zjištìní filtrace zmenšených textur objektu

bool FGetD3DOMinFilter()
{
	if (IsValidID())
	{
		return GetID()->MinFilter[D3DAktStage];
	}
	return FGetD3DMinFilter();
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní filtrace zvìtšených textur objektu

bool FGetD3DOMagFilter()
{
	if (IsValidID())
	{
		return GetID()->MagFilter[D3DAktStage];
	}
	return FGetD3DMagFilter();
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní filtrace vzdálených textur objektu

bool FGetD3DOMipFilter()
{
	if (IsValidID())
	{
		return GetID()->MipFilter[D3DAktStage];
	}
	return FGetD3DMipFilter();
}

/////////////////////////////////////////////////////////////////////////////
// tvoøit horní podstavu

bool FGetD3DUpper()
{
	return D3DCreateUpper;
}

/////////////////////////////////////////////////////////////////////////////
// tvoøit dolní podstavu

bool FGetD3DLower()
{
	return D3DCreateLower;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní viditelnosti objektu Direct3D

bool FGetD3DVisible()
{
	if (IsValidID())
	{
		return GetID()->Visible;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní hloubkového zápisu

bool FGetD3DZWrite()
{
	if (IsValidID())
	{
		return (GetID()->ZWrite != FALSE);
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní hloubkového tøídìní

bool FGetD3DZSort()
{
	if (IsValidID())
	{
		return (RendSort[GetID()->RendGroup] != FALSE);
	}
	return false;
}

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------


/////////////////////////////////////////////////////////////////////////////
// test, zda to je aktivní aplikace

bool FGetActive()
{
	return AppActive;
}

/////////////////////////////////////////////////////////////////////////////
// tisk obrázku

bool FPicPrint() 
{ 
// naètení obrázku k tisku
	CPicture pic;
	FPicture(pic);
	if ((pic.Width() == 0) || (pic.Height() == 0)) return false;

// nastavení tiskárny
	PRINTDLG pd;
	MemFill(&pd, sizeof(PRINTDLG), 0);
	pd.lStructSize = sizeof(PRINTDLG);
	pd.Flags = PD_RETURNDC | PD_PRINTSETUP;
	pd.hwndOwner = Win[DialogParent].Wnd;

	BOOL res = ::PrintDlg(&pd);
	if (pd.hDevMode) ::GlobalFree (pd.hDevMode);
	if (pd.hDevNames) :: GlobalFree (pd.hDevNames);
	if (!res || (pd.hDC == NULL)) return false;

// zahájení tisku
	DOCINFO di;
	MemFill(&di, sizeof(DOCINFO), 0);
	di.cbSize = sizeof(DOCINFO);
	di.lpszDocName = Win[DialogParent].Text;
	::StartDoc(pd.hDC, &di);
	::StartPage(pd.hDC);

// tisk obrázku
	pic.DeComp();

	StdBitmapInfo->bmiHeader.biWidth = pic.Width();
	StdBitmapInfo->bmiHeader.biHeight = pic.Height();

	int horzres = ::GetDeviceCaps(pd.hDC, HORZRES);	// šíøka papíru v tiskových bodech
	int horzres0 = Round(horzres * 0.95);			// užiteèná šíøka papíru v bodech
	int vertres = ::GetDeviceCaps(pd.hDC, VERTRES); // výška papíru v tiskových bodech
	int vertres0 = Round(vertres * 0.95);			// užiteèná výška papíru v bodech

	int scale = 1;									// mìøítko * 4

	while ((((pic.Width() * (scale+1) + 2)/4) <= horzres0) &&
		(((pic.Height() * (scale+1) + 2)/4) <= vertres0))
	{
		scale++;
	}

	int width2 = (pic.Width() * scale + 2) / 4;
	int height2 = (pic.Height() * scale + 2) / 4;

	int left = (horzres - width2)/2;
	if (left < 0) left = 0;

	int top = 0;
	if (vertres < horzres)
	{
		top = (vertres - height2)/2;
		if (top < 0) top = 0;
	}

	::StretchDIBits(pd.hDC,
		left, top, width2, height2,					// cílové parametry
		0, 0, pic.Width(), pic.Height(),			// zdrojové parametry
		pic.DataData(),								// data obrázku
		StdBitmapInfo, DIB_RGB_COLORS, SRCCOPY);

// ukonèení tisku
	::EndPage(pd.hDC);
	::EndDoc(pd.hDC);
	::DeleteDC(pd.hDC);

	return true;
}


/***************************************************************************\
*																			*
*								obsluha DLL									*
*																			*
\***************************************************************************/

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

/////////////////////////////////////////////////////////////////////////////
// logická hodnota

bool FGetMemoryBool()
{
	BOOL* buf = (BOOL*)DLLMemoryRead;
	bool data = (*buf != FALSE);
	buf++;
	DLLMemoryRead = buf;
	return data;
}


#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------
