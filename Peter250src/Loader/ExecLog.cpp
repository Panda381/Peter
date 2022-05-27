
#include "Main.h"

/***************************************************************************\
*																			*
*						Prov�d�n� programu - logick� v�raz					*
*																			*
\***************************************************************************/

#pragma optimize("t", on)			// optimalizace na maxim�ln� rychlost


/***************************************************************************\
*																			*
*									funkce									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// funkce s n�vratem logick� hodnoty

bool FBoolFunc()
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
	bool result = Bool[Bool.Num() - res];

// zru�en� lok�ln�ch prom�nn�ch
	FCommand();

	return result;
}


/***************************************************************************\
*																			*
*								prom�nn�									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// konstanta TRUE a FALSE

bool FTrue() { return true; };
bool FFalse() { return false; };


/////////////////////////////////////////////////////////////////////////////
// glob�ln� prom�nn� (Data = index)

bool FBoolObj()
{
// index
	int inx = ExecItem[-1].Data;

// data
	return Bool[inx];
}


/////////////////////////////////////////////////////////////////////////////
// globaln� prom�nn� v seznamu (Data = index, List = seznam)

bool FBoolObjList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index prom�nn�, inkrementace indexu seznamu
	int inx = ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// data
	return Bool[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lok�ln� prom�nn� (Data = index)

bool FBoolLoc()
{
// index prom�nn�
	int inx = Bool.Num() - ExecItem[-1].Data;

// data
	return Bool[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lok�ln� prom�nn� v seznamu (Data = index, List = seznam)

bool FBoolLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index prom�nn�, inkrementace indexu seznamu
	int inx = Bool.Num() - ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// data
	return Bool[inx];
}


/////////////////////////////////////////////////////////////////////////////
// pohyb glob�ln�ho sprajtu (Data = index)

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
// pohyb lok�ln�ho sprajtu (Data = index)

bool FGetSpriteMoveLoc()
{
// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;

// pohyb sprajtu
	return (Sprite[inx].KrokCit() > 0);
}


/////////////////////////////////////////////////////////////////////////////
// pohyb lok�ln�ho sprajtu v seznamu (Data = index, List = seznam)

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
// viditelnost glob�ln�ho sprajtu (Data = index)

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
// viditelnost lok�ln�ho sprajtu (Data = index)

bool FGetSpriteVisLoc()
{
// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;

// viditelnost sprajtu
	return Sprite[inx].Visible();
}


/////////////////////////////////////////////////////////////////////////////
// viditelnost lok�ln�ho sprajtu v seznamu (Data = index, List = seznam)

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
*							matematick� oper�tory							*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// oper�tor EQU - pro 1 prvek (porovn� ��slo s nulou)

bool FEqu1() { return FNum() == 0; };


/////////////////////////////////////////////////////////////////////////////
// oper�tor EQU (Data = po�et prvk� - 1)

bool FEqu()
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data;

// na�ten� prvn�ho ��sla
	double num = FNum();

// cyklus p�es dal�� ��sla
	for (; i > 0; i--)
	{

// porovn�n� dal��ho ��sla
		if (FNum() != num)
		{

// nevyhovuje - zrychlen� ukon�en� operace
			if (i > 1) ExecItem += ExecItem->Jump;
			return false;
		}
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// oper�tor NEQU - pro 1 prvek (porovn� ��slo s nulou)

bool FNEqu1() { return FNum() != 0; };


/////////////////////////////////////////////////////////////////////////////
// oper�tor NEQU (Data = po�et prvk� - 1)

bool FNEqu()
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data;

// na�ten� prvn�ho ��sla
	double num1 = FNum();
	double num2;

// cyklus p�es dal�� ��sla
	for (; i > 0; i--)
	{

// na�ten� dal��ho ��sla
		num2 = FNum();

// porovn�n� dal��ho ��sla
		if (num2 == num1)
		{

// nevyhovuje - zrychlen� ukon�en� operace
			if (i > 1) ExecItem += ExecItem->Jump;
			return false;
		}

// �schova ��sla pro p��t� operaci
		num1 = num2;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// oper�tor EQGR - pro 1 prvek (porovn� ��slo s nulou)

bool FEqGr1() { return FNum() >= 0; };


/////////////////////////////////////////////////////////////////////////////
// oper�tor EQGR (Data = po�et prvk� - 1)

bool FEqGr()
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data;

// na�ten� prvn�ho ��sla
	double num1 = FNum();
	double num2;

// cyklus p�es dal�� ��sla
	for (; i > 0; i--)
	{

// na�ten� dal��ho ��sla
		num2 = FNum();

// porovn�n� dal��ho ��sla
		if (num1 < num2)
		{

// nevyhovuje - zrychlen� ukon�en� operace
			if (i > 1) ExecItem += ExecItem->Jump;
			return false;
		}

// �schova ��sla pro p��t� operaci
		num1 = num2;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// oper�tor EQLT - pro 1 prvek (porovn� ��slo s nulou)

bool FEqLt1() { return FNum() <= 0; };


/////////////////////////////////////////////////////////////////////////////
// oper�tor EQLT (Data = po�et prvk� - 1)

bool FEqLt()
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data;

// na�ten� prvn�ho ��sla
	double num1 = FNum();
	double num2;

// cyklus p�es dal�� ��sla
	for (; i > 0; i--)
	{

// na�ten� dal��ho ��sla
		num2 = FNum();

// porovn�n� dal��ho ��sla
		if (num1 > num2)
		{

// nevyhovuje - zrychlen� ukon�en� operace
			if (i > 1) ExecItem += ExecItem->Jump;
			return false;
		}

// �schova ��sla pro p��t� operaci
		num1 = num2;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// oper�tor GR - pro 1 prvek (porovn� ��slo s nulou)

bool FGr1() { return FNum() > 0; };


/////////////////////////////////////////////////////////////////////////////
// oper�tor GR (Data = po�et prvk� - 1)

bool FGr()
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data;

// na�ten� prvn�ho ��sla
	double num1 = FNum();
	double num2;

// cyklus p�es dal�� ��sla
	for (; i > 0; i--)
	{

// na�ten� dal��ho ��sla
		num2 = FNum();

// porovn�n� dal��ho ��sla
		if (num1 <= num2)
		{

// nevyhovuje - zrychlen� ukon�en� operace
			if (i > 1) ExecItem += ExecItem->Jump;
			return false;
		}

// �schova ��sla pro p��t� operaci
		num1 = num2;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// oper�tor LT - pro 1 prvek (porovn� ��slo s nulou)

bool FLt1() { return FNum() < 0; };


/////////////////////////////////////////////////////////////////////////////
// oper�tor LT (Data = po�et prvk� - 1)

bool FLt()
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data;

// na�ten� prvn�ho ��sla
	double num1 = FNum();
	double num2;

// cyklus p�es dal�� ��sla
	for (; i > 0; i--)
	{

// na�ten� dal��ho ��sla
		num2 = FNum();

// porovn�n� dal��ho ��sla
		if (num1 >= num2)
		{

// nevyhovuje - zrychlen� ukon�en� operace
			if (i > 1) ExecItem += ExecItem->Jump;
			return false;
		}

// �schova ��sla pro p��t� operaci
		num1 = num2;
	}
	return true;
}


/***************************************************************************\
*																			*
*								logick� oper�tory							*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// oper�tor AND - pro 1 prvek (jenom vr�t� hodnotu beze zm�ny)

bool FAnd1() { return FBool(); }


/////////////////////////////////////////////////////////////////////////////
// oper�tor AND (Data = po�et prvk� - 1)

bool FAnd()
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data;

// cyklus p�es v�echny prvky
	for (; i >= 0; i--)
	{

// kontrola prvku
		if (!FBool())
		{

// nevyhovuje - zrychlen� ukon�en� operace
			if (i > 0) ExecItem += ExecItem->Jump;
			return false;
		}
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// oper�tor OR (Data = po�et prvk� - 1)

bool FOr()
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data;

// cyklus p�es v�echny prvky
	for (; i >= 0; i--)
	{

// kontrola prvku
		if (FBool())
		{

// vyhovuje - zrychlen� ukon�en� operace
			if (i > 0) ExecItem += ExecItem->Jump;
			return true;
		}
	}
	return false;
}


/////////////////////////////////////////////////////////////////////////////
// oper�tor XOR (Data = po�et prvk� - 1)

bool FXor()
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data;

// st�ada� operace
	bool result = false;

// cyklus p�es v�echny prvky
	for (; i >= 0; i--)
	{

// na�ten� prvku
		result ^= FBool();
	}
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// oper�tor NOT

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
// porovn�n� jedn� ikony, zda je pr�zdn�

bool FCompEqIco1()
{
	CIcon icon;
	FIcon(icon);
	return (icon.IsEmpty() != FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// porovn�n� shodnosti ikon (Data = po�et prvk� - 1)

bool FCompEqIco()
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data;

// na�ten� prvn� ikony
	CIcon icon1;
	FIcon(icon1);
	CIcon icon2;

// cyklus p�es dal�� ikony
	for (; i > 0; i--)
	{

// na�ten� dal�� ikony
		FIcon(icon2);

// porovn�n� ikon
		if (icon1 != icon2)
		{

// nevyhovuje - zrychlen� ukon�en� operace
			if (i > 1) ExecItem += ExecItem->Jump;
			return false;
		}

// ikona 2 se uschov� pro p��t� pou�it�
//		icon1 = icon2;
	}
	return true;
}







/////////////////////////////////////////////////////////////////////////////
// porovn�n� jedn� plochy, zda je pr�zdn�

bool FCompEqMap1()
{
	CMap map;
	FMap(map);
	return (map.IsEmpty() != FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// porovn�n� shodnosti ploch (Data = po�et prvk� - 1)

bool FCompEqMap()
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data;

// na�ten� prvn� plochy
	CMap map1;
	FMap(map1);
	CMap map2;

// cyklus p�es dal�� plochy
	for (; i > 0; i--)
	{

// na�ten� dal�� plochy
		FMap(map2);

// porovn�n� ploch
		if (map1 != map2)
		{

// nevyhovuje - zrychlen� ukon�en� operace
			if (i > 1) ExecItem += ExecItem->Jump;
			return false;
		}

// plocha 2 se uschov� pro p��t� pou�it�
//		map1 = map2;
	}
	return true;
}



/////////////////////////////////////////////////////////////////////////////
// porovn�n� jednoho obr�zku, zda je pr�zdn�

bool FCompEqPic1()
{
	CPicture pic;
	FPicture(pic);
	return (pic.IsEmpty() != FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// porovn�n� shodnosti obr�zk� (Data = po�et prvk� - 1)

bool FCompEqPic()
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data;

// na�ten� prvn�ho obr�zku
	CPicture pic1;
	FPicture(pic1);
	CPicture pic2;

// cyklus p�es dal�� obr�zky
	for (; i > 0; i--)
	{

// na�ten� dal��ho obr�zku
		FPicture(pic2);

// porovn�n� obr�zk�
		if (pic1 != pic2)
		{

// nevyhovuje - zrychlen� ukon�en� operace
			if (i > 1) ExecItem += ExecItem->Jump;
			return false;
		}

// obr�zek 2 se uschov� pro p��t� pou�it�
//		pic1 = pic2;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// porovn�n� jednoho sprajtu, zda je pr�zdn�

bool FCompEqSpr1()
{
	CSprite spr;
	FSprite(spr);
	return (spr.IsEmpty() != FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// porovn�n� shodnosti sprajt� (Data = po�et prvk� - 1)

bool FCompEqSpr()
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data;

// na�ten� prvn�ho sprajtu
	CSprite spr1;
	FSprite(spr1);
	CSprite spr2;

// cyklus p�es dal�� sprajty
	for (; i > 0; i--)
	{

// na�ten� dal��ho sprajtu
		FSprite(spr2);

// porovn�n� sprajt�
		if (spr1 != spr2)
		{

// nevyhovuje - zrychlen� ukon�en� operace
			if (i > 1) ExecItem += ExecItem->Jump;
			return false;
		}

// sprajt 2 se uschov� pro p��t� pou�it�
//		spr1 = spr2;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// porovn�n� jednoho zvuku, zda je pr�zdn�

bool FCompEqSnd1()
{
	CSound snd;
	FSound(snd);
	return (snd.IsEmpty() != FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// porovn�n� shodnosti zvuk� (Data = po�et prvk� - 1)

bool FCompEqSnd()
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data;

// na�ten� prvn�ho zvuku
	CSound snd1;
	FSound(snd1);
	CSound snd2;

// cyklus p�es dal�� zvuky
	for (; i > 0; i--)
	{

// na�ten� dal��ho zvuku
		FSound(snd2);

// porovn�n� zvuk�
		if (snd1 != snd2)
		{

// nevyhovuje - zrychlen� ukon�en� operace
			if (i > 1) ExecItem += ExecItem->Jump;
			return false;
		}

// zvuk 2 se uschov� pro p��t� pou�it�
//		snd1 = snd2;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// porovn�n� jedn� hudby, zda je pr�zdn�

bool FCompEqMus1()
{
	CMusic mus;
	FMusic(mus);
	return (mus.IsEmpty() != FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// porovn�n� shodnosti hudeb (Data = po�et prvk� - 1)

bool FCompEqMus()
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data;

// na�ten� prvn� hudby
	CMusic mus1;
	FMusic(mus1);
	CMusic mus2;

// cyklus p�es dal�� hudby
	for (; i > 0; i--)
	{

// na�ten� dal�� hudby
		FMusic(mus2);

// porovn�n� hudeb
		if (mus1 != mus2)
		{

// nevyhovuje - zrychlen� ukon�en� operace
			if (i > 1) ExecItem += ExecItem->Jump;
			return false;
		}

// hudba 2 se uschov� pro p��t� pou�it�
//		mus1 = mus2;
	}
	return true;
}


/***************************************************************************\
*																			*
*								obsluha text�								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// porovn�n� jednoho textu, zda je pr�zdn�

bool FCompEqTxt1()
{
	CString txt;
	FText(txt);
	return (txt.IsEmpty() != FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// porovn�n� text� na shodnost (Data = po�et text� - 1)

bool FCompEqTxt()
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data;

// na�ten� prvn�ho textu
	CString txt1;
	FText(txt1);
	CString txt2;

// cyklus p�es dal�� texty
	for (; i > 0; i--)
	{

// na�ten� dal��ho textu
		FText(txt2);

// porovn�n� text�
		if (txt1 != txt2)
		{

// nevyhovuje - zrychlen� ukon�en� operace
			if (i > 1) ExecItem += ExecItem->Jump;
			return false;
		}

// text 2 se uschov� pro p��t� pou�it�
//		txt1 = txt2;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// porovn�n� jednoho textu, zda je nepr�zdn�

bool FCompGtTxt1()
{
	CString txt;
	FText(txt);
	return (txt.IsNotEmpty() != FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// porovn�n� text� na neshodnost (Data = po�et text� - 1)

bool FCompGtTxt()
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data;

// na�ten� prvn�ho textu
	CString txt1;
	FText(txt1);
	CString txt2;

// cyklus p�es dal�� texty
	for (; i > 0; i--)
	{

// na�ten� dal��ho textu
		FText(txt2);

// porovn�n� text�
		if (txt1 <= txt2)
		{

// nevyhovuje - zrychlen� ukon�en� operace
			if (i > 1) ExecItem += ExecItem->Jump;
			return false;
		}

// text 2 se uschov� pro p��t� pou�it�
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
// na�ten� viditelnosti Petra a Petry

bool FGetVisible() { return Sprite[0].Visible(); }
bool FGetVisible2() { return Sprite[1].Visible(); }


/////////////////////////////////////////////////////////////////////////////
// na�ten� p�ep�na�e Petra

bool _fastcall FGetSwc(DWORD mask)
{
// na�ten� pozice Petra
	MAPITEM* item = PetrXYMap();

// zji�t�n� p�ep�na�e, je-li platn� pozice
	if (item != NULL)
	{
		return ((item->Param & mask) != 0);
	}
	return false;
}

bool FGetSwc1() { return FGetSwc(MAP_FLAG1); };		// p�ep�na� 1 - Petr
bool FGetSwc2() { return FGetSwc(MAP_FLAG2); };		// p�ep�na� 2 - Petr
bool FGetSwc3() { return FGetSwc(MAP_FLAG3); };		// p�ep�na� 3 - Petr
bool FGetSwc4() { return FGetSwc(MAP_FLAG4); };		// p�ep�na� 4 - Petr
bool FGetSwc5() { return FGetSwc(MAP_FLAG5); };		// p�ep�na� 5 - Petr

/////////////////////////////////////////////////////////////////////////////
// na�ten� p�ep�na�e Petry

bool _fastcall FGet2Swc(DWORD mask)
{
// na�ten� pozice Petry
	MAPITEM* item = Petr2XYMap();

// zji�t�n� p�ep�na�e, je-li platn� pozice
	if (item != NULL)
	{
		return ((item->Param & mask) != 0);
	}
	return false;
}

bool FGetSwc12() { return FGet2Swc(MAP_FLAG1); };	// p�ep�na� 1 - Petra
bool FGetSwc22() { return FGet2Swc(MAP_FLAG2); };	// p�ep�na� 2 - Petra
bool FGetSwc32() { return FGet2Swc(MAP_FLAG3); };	// p�ep�na� 3 - Petra
bool FGetSwc42() { return FGet2Swc(MAP_FLAG4); };	// p�ep�na� 4 - Petra
bool FGetSwc52() { return FGet2Swc(MAP_FLAG5); };	// p�ep�na� 5 - Petra


/***************************************************************************\
*																			*
*							obsluha kl�vesnice								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// test stisku kl�vesy

bool FKeyTest()
{
// na�ten� k�du kl�vesy
	int klav = FIntR();

// k�d kl�vesy Pause
	if (klav == VK_PAUSE) return Pause;

// kontrola platnosti k�du kl�vesy
	if ((DWORD)klav >= (DWORD)KEYMAPSIZE) return false;

// test ��dn� kl�vesy
	if (klav == 0) return ((KeyMapNum == 0) && !Pause);

// test kl�vesy
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
*							obsluha my�i									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// test stisku lev�ho tla��tka my�i

bool FMouseL() { return LMouseDown; }


/////////////////////////////////////////////////////////////////////////////
// test stisku prav�ho tla��tka my�i

bool FMouseR() { return RMouseDown; }


/////////////////////////////////////////////////////////////////////////////
// test kliku lev�m tla��tkem my�i

bool FMouseLClick()
{
	bool result = LMouseClick;
	LMouseClick = false;
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// test kliku prav�m tla��tkem my�i

bool FMouseRClick()
{
	bool result = RMouseClick;
	RMouseClick = false;
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// test dvojkliku lev�m tla��tkem my�i

bool FMouseLDClick()
{
	bool result = LMouseDClick;
	LMouseDClick = false;
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// test dvojkliku prav�m tla��tkem my�i

bool FMouseRDClick()
{
	bool result = RMouseDClick;
	RMouseDClick = false;
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// test my�i v oblasti

bool FMouseRegion()
{
// na�ten� rozm�r� regionu
	double x1 = FNum();				// po��tek X
	double y1 = FNum();				// po��tek Y
	double x2 = FNum() + x1;		// konec X
	double y2 = FNum() + y1;		// konec Y

// porovn�n� oblasti
	return ((MouseX >= x1) && (MouseX < x2) && (MouseY >= y1) && (MouseY < y2));
}

/////////////////////////////////////////////////////////////////////////////
// test my�i v dialogov�m prvku

bool FMouseItem()
{
// na�ten� ��sla prvku
	int id = FIntR();

// test, zda je platn� prvek okna
	if (Win.IsValid(id))
	{
		WINITEM* item = &Win[id];

// p��prava v��ky rodi�e
		int height;
		if ((item->Typ == WINITEM_WINDOW) || (item->Typ == WINITEM_WINDOWRO))
		{
			height = ScreenHeight0;
		}
		else
		{
			height = Win[item->Parent].ClientHeight;
		}

// p��prava rozm�r� regionu
		double x1 = (double)item->Left/ICONWIDTH;							// po��tek X
		double y1 = (double)(height - item->Top - item->Height)/ICONHEIGHT;	// po��tek Y
		double x2 = (double)item->Width/ICONWIDTH + x1;						// konec X
		double y2 = (double)item->Height/ICONHEIGHT + y1;					// konec Y

// porovn�n� oblasti
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
// test p�ehr�v�n� jak�hokoliv zvuku

bool FSoundTest()
{
	return ((WaveOut != NULL) || (SoundKanalu > 0));
}


/////////////////////////////////////////////////////////////////////////////
// test p�ehr�v�n� konkr�tn�ho zvuku

bool FSoundTest1()
{
// na�ten� testovan�ho zvuku
	CSound sound;
	FSound(sound);

// nestandardn� form�t
	if (!WaveFormatPCM)
	{
		return ((WaveOut != NULL) && (sound == Sound2));
	}

// nalezen� zvuku v bufferu
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
// p��znak stereo

bool FGetSoundStereo()
{
	CSound snd;
	FSound(snd);
	return (snd.Channels() == 2);
}


/////////////////////////////////////////////////////////////////////////////
// test zapauzov�n� zvuku

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
// test p�ehr�v�n� hudby

bool FMusicTest()
{
	return MusicPlaing;
}


/////////////////////////////////////////////////////////////////////////////
// test zapauzov�n� hudby

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
// aktualizace �daj� o CD
	CDAkt();

// p��znak pauzy CD
	return CDPausing;
}


/////////////////////////////////////////////////////////////////////////////
// test p�ehr�v�n� CD

bool FCDTest()
{
// aktualizace �daj� o CD
	CDAkt();

// p��znak p�ehr�v�n� CD
	return CDPlaing;
}


/////////////////////////////////////////////////////////////////////////////
// test vysunut� CD

bool FCDGetEject()
{
// aktualizace �daj� o CD
	CDAkt();

// p��znak vysunut� CD
	return !CDDiskValid;
}


/***************************************************************************\
*																			*
*							obsluha soubor�									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� souboru

bool FBoolFileNew()
{
// uzav�en� soubor�
	FileClose();

// na�ten� jm�na adres��e
	CString jmeno;
	FFileName(jmeno);

// jm�no mus� b�t zad�no a nesm� kon�it znakem "\"
	if (jmeno.IsNotEmpty() && (jmeno[jmeno.Length()-1] != '\\'))
	{

// prvn� pokus o vytvo�en� souboru
		HANDLE file = ::CreateFile(jmeno, GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL);

		if (file != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(file);
			return true;
		}

// kontrola, zda soubor ji� existuje
		DWORD atr = ::GetFileAttributes(jmeno);
		if ((int)atr != -1)
		{
			if ((atr & FILE_ATTRIBUTE_DIRECTORY) == 0) return true;
		}
		else
		{

// vytvo�en� cesty k souboru
			int prev = jmeno.RevFind('\\');
			if (prev > 0)
			{
				if (CreatePath(jmeno.Left(prev)))
				{

// druh� pokus o vytvo�en� souboru
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
// vytvo�en� adres��e

bool FBoolDirNew()
{
// uzav�en� soubor�
	FileClose();

// na�ten� jm�na adres��e
	CString jmeno;
	FFileName(jmeno);

// vytvo�en� adres��e
	if (jmeno.IsEmpty() || !CreatePath(jmeno))
	{
		FileError = true;
		return false;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// zru�en� souboru/adres��e

bool FBoolFileDelete()
{
// uzav�en� soubor�
	FileClose();

// na�ten� jm�na souboru/adres��e
	CString jmeno;
	FFileName(jmeno);

// zru�en� souboru nebo adres��e
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
// test existence disku/souboru/adres��e

bool FFileTest()
{
// na�ten� jm�na disku/adres��e/souboru
	CString jmeno;
	FFileName(jmeno);

// d�lka jm�na
	int delka = jmeno.Length();
	if (delka <= 0) return false;

// kontroluje se jenom disk, p��p. root
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

// kontroluje se adres��/soubor
	WIN32_FIND_DATA wfd;
	HANDLE find = ::FindFirstFile(jmeno, &wfd);
	if (find == INVALID_HANDLE_VALUE) return false;
	::FindClose(find);
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// kopie soubor�

bool FBoolFileCopy()
{
// uzav�en� otev�en�ch soubor�
	FileClose();

// jm�na obou soubor� mus� b�t platn�
	if (FileRead.IsNotEmpty() && FileWrite.IsNotEmpty())
	{

// nulov�n� ukazatel� v souborech (kdyby se zm�nily)
		FileReadOff = 0;
		FileWriteOff = 0;

// proveden� kopie soubor�
		if (::CopyFile(FileRead, FileWrite, TRUE)) return true;
	}

// chyba operace
	FileError = true;
	return false;
}


/////////////////////////////////////////////////////////////////////////////
// p�esun soubor�

bool FBoolFileMove()
{
// uzav�en� otev�en�ch soubor�
	FileClose();

// jm�na obou soubor� mus� b�t platn�
	if (FileRead.IsNotEmpty() && FileWrite.IsNotEmpty())
	{

// nulov�n� ukazatel� v souborech (kdyby se zm�nily)
		FileReadOff = 0;
		FileWriteOff = 0;

// proveden� p�esunu soubor�
		if (::MoveFile(FileRead, FileWrite)) return true;
	}

// chyba operace
	FileError = true;
	return false;
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� atributu souboru

bool _fastcall FGetFileAttrib(DWORD attrib)
{
// uzav�en� otev�en�ch soubor�
	FileClose();

// mus� b�t n�jak� jm�no souboru
	if (FileRead.IsNotEmpty())
	{

// na�ten� atribut� souboru
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
// na�ten� logick� hodnoty

bool FGetFileLogic() { return (FileReadByte() != 0); }


/////////////////////////////////////////////////////////////////////////////
// poskytnut� p��znaku chyby soubor�

bool FGetFileError() { return FileError; }


/////////////////////////////////////////////////////////////////////////////
// poskytnut� p��znaku textu UNICODE

bool FGetFileTextUnicode() { return FileTextUnicode; }


/***************************************************************************\
*																			*
*								obsluha plochy								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// na�ten� p�ep�na�e z plochy

bool _fastcall FGetMapSwc(DWORD mask)
{
// na�ten� plochy
	CMap map;
	FMap(map);

// na�ten� sou�adnic
	int x = FIntR();
	int y = FIntR();

// p�ep�na� na pozici
	if (((DWORD)x < (DWORD)map.Width()) && ((DWORD)y < (DWORD)map.Height()))
	{
		return ((map.At(x, y).Param & mask) != 0);
	}
	return false;
}

bool FGetMapSwc1() { return FGetMapSwc(MAP_FLAG1); };		// p�ep�na� 1
bool FGetMapSwc2() { return FGetMapSwc(MAP_FLAG2); };		// p�ep�na� 2
bool FGetMapSwc3() { return FGetMapSwc(MAP_FLAG3); };		// p�ep�na� 3
bool FGetMapSwc4() { return FGetMapSwc(MAP_FLAG4); };		// p�ep�na� 4
bool FGetMapSwc5() { return FGetMapSwc(MAP_FLAG5); };		// p�ep�na� 5


/***************************************************************************\
*																			*
*									konzola									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// re�im konzoly

bool FGetConsoleOn() { return ConsoleOn; }


/***************************************************************************\
*																			*
*									joystick								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// tla��tko 1

bool FJoystick1()
{
	int inx = FIntR();
	if ((DWORD)inx >= (DWORD)MAXJOYSTICK) return false;
	AktJoystick(inx);
	return Joystick1[inx];
}

/////////////////////////////////////////////////////////////////////////////
// tla��tko 2

bool FJoystick2()
{
	int inx = FIntR();
	if ((DWORD)inx >= (DWORD)MAXJOYSTICK) return false;
	AktJoystick(inx);
	return Joystick2[inx];
}

/////////////////////////////////////////////////////////////////////////////
// tla��tko 3

bool FJoystick3()
{
	int inx = FIntR();
	if ((DWORD)inx >= (DWORD)MAXJOYSTICK) return false;
	AktJoystick(inx);
	return Joystick3[inx];
}

/////////////////////////////////////////////////////////////////////////////
// tla��tko 4

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
// z�kaz okna

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
// kliknut� na tla��tko

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
// stav p�ep�na�e

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
// zv�razn�n� p�smo

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
// p�smo s pevnou rozte��

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
// �ikm� p�smo

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
// podtr�en� p�smo

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
// p�e�krtnut� p�smo

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
// pati�kov� p�smo

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
// celoobrazovkov� re�im

bool FGetFullScreen()
{
	return FullScreen;
}

/////////////////////////////////////////////////////////////////////////////
// na�ten� p�ep�na�e r�me�ku okna

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
// na�ten� p�ep�na�e nadpisu okna

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
// na�ten� p�ep�na�e okna v�dy naho�e

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
// na�ten� p�ep�na�e okna m�niteln� velikosti

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
// po�adavek o uzav�en� okna

bool FGetWindowMayClose()
{
	bool reqclose = ReqClose;
	ReqClose = false;
	return reqclose;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� maximalizace okna

bool FGetWindowMaxim()
{
	return (::IsZoomed(Win[DialogParent].Wnd) != 0);
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� minimalizace okna

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
// zji�t�n� p�ep�na�e hry 1

bool FGetGameL1()
{
	return (((WORD*)DirectPlayGamesParam)[0] >= 0x8000);
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� p�ep�na�e hry 2

bool FGetGameL2()
{
	return (((WORD*)DirectPlayGamesParam)[1] >= 0x8000);
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� p�ep�na�e hry 3

bool FGetGameL3()
{
	return (((WORD*)DirectPlayGamesParam)[2] >= 0x8000);
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� p�ep�na�e hry 4

bool FGetGameL4()
{
	return (((WORD*)DirectPlayGamesParam)[3] >= 0x8000);
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� p�ep�na�e hry 5

bool FGetGameL5()
{
	return (((WORD*)DirectPlayGamesParam)[4] >= 0x8000);
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� p�ep�na�e hry 6

bool FGetGameL6()
{
	return (((WORD*)DirectPlayGamesParam)[5] >= 0x8000);
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� p�ep�na�e hry 7

bool FGetGameL7()
{
	return (((WORD*)DirectPlayGamesParam)[6] >= 0x8000);
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� p�ep�na�e hry 8

bool FGetGameL8()
{
	return (((WORD*)DirectPlayGamesParam)[7] >= 0x8000);
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� p��znaku hostitele

bool FGameHost()
{
	return DirectPlayGamesHost;
}

/////////////////////////////////////////////////////////////////////////////
// na�ten� p�ep�na�e z paketu

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
// zji�t�n� stopbit�

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
// test p�ijat�ho znaku

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
// test povolen� vys�l�n�, prot�j�ek je p�ipraven p�ij�mat data (sign�l CTS)

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
// test povolen� p��jmu, data vys�lan� od prot�j�ku jsou platn� (sign�l DSR)

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
// --------------------- konec vypnut� pro MINI verzi -------------------

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� m�du grafick�ho pozad�

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
// zji�t�n� nastaven� vypl�ov�n� ploch

bool FGetD3DWireframe()
{
	return D3DWireframeGlobal;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� nastaven� osv�tlen�

bool FGetD3DLighton()
{
	return D3DLightOnGlobal;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� nastaven� plynul�ho st�nov�n�

bool FGetD3DShades()
{
	return D3DShadesGlobal;
}
		   
/////////////////////////////////////////////////////////////////////////////
// zji�t�n� filtrace zmen�en�ch textur

bool FGetD3DMinFilter()
{
	return D3DMinFilterGlobal;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� filtrace zv�t�en�ch textur

bool FGetD3DMagFilter()
{
	return D3DMagFilterGlobal;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� filtrace vzd�len�ch textur

bool FGetD3DMipFilter()
{
	return D3DMipFilterGlobal;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� nastaven� vypl�ov�n� ploch objektu

bool FGetD3DOWireframe()
{
	if (IsValidID())
	{
		return GetID()->Wireframe;
	}
	return FGetD3DWireframe();
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� nastaven� osv�tlen� objektu

bool FGetD3DOLighton()
{
	if (IsValidID())
	{
		return GetID()->Lighton;
	}
	return FGetD3DLighton();
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� nastaven� plynul�ho st�nov�n� objektu

bool FGetD3DOShades()
{
	if (IsValidID())
	{
		return GetID()->Shades;
	}
	return FGetD3DShades();
}
		   
/////////////////////////////////////////////////////////////////////////////
// zji�t�n� filtrace zmen�en�ch textur objektu

bool FGetD3DOMinFilter()
{
	if (IsValidID())
	{
		return GetID()->MinFilter[D3DAktStage];
	}
	return FGetD3DMinFilter();
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� filtrace zv�t�en�ch textur objektu

bool FGetD3DOMagFilter()
{
	if (IsValidID())
	{
		return GetID()->MagFilter[D3DAktStage];
	}
	return FGetD3DMagFilter();
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� filtrace vzd�len�ch textur objektu

bool FGetD3DOMipFilter()
{
	if (IsValidID())
	{
		return GetID()->MipFilter[D3DAktStage];
	}
	return FGetD3DMipFilter();
}

/////////////////////////////////////////////////////////////////////////////
// tvo�it horn� podstavu

bool FGetD3DUpper()
{
	return D3DCreateUpper;
}

/////////////////////////////////////////////////////////////////////////////
// tvo�it doln� podstavu

bool FGetD3DLower()
{
	return D3DCreateLower;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� viditelnosti objektu Direct3D

bool FGetD3DVisible()
{
	if (IsValidID())
	{
		return GetID()->Visible;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� hloubkov�ho z�pisu

bool FGetD3DZWrite()
{
	if (IsValidID())
	{
		return (GetID()->ZWrite != FALSE);
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� hloubkov�ho t��d�n�

bool FGetD3DZSort()
{
	if (IsValidID())
	{
		return (RendSort[GetID()->RendGroup] != FALSE);
	}
	return false;
}

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------


/////////////////////////////////////////////////////////////////////////////
// test, zda to je aktivn� aplikace

bool FGetActive()
{
	return AppActive;
}

/////////////////////////////////////////////////////////////////////////////
// tisk obr�zku

bool FPicPrint() 
{ 
// na�ten� obr�zku k tisku
	CPicture pic;
	FPicture(pic);
	if ((pic.Width() == 0) || (pic.Height() == 0)) return false;

// nastaven� tisk�rny
	PRINTDLG pd;
	MemFill(&pd, sizeof(PRINTDLG), 0);
	pd.lStructSize = sizeof(PRINTDLG);
	pd.Flags = PD_RETURNDC | PD_PRINTSETUP;
	pd.hwndOwner = Win[DialogParent].Wnd;

	BOOL res = ::PrintDlg(&pd);
	if (pd.hDevMode) ::GlobalFree (pd.hDevMode);
	if (pd.hDevNames) :: GlobalFree (pd.hDevNames);
	if (!res || (pd.hDC == NULL)) return false;

// zah�jen� tisku
	DOCINFO di;
	MemFill(&di, sizeof(DOCINFO), 0);
	di.cbSize = sizeof(DOCINFO);
	di.lpszDocName = Win[DialogParent].Text;
	::StartDoc(pd.hDC, &di);
	::StartPage(pd.hDC);

// tisk obr�zku
	pic.DeComp();

	StdBitmapInfo->bmiHeader.biWidth = pic.Width();
	StdBitmapInfo->bmiHeader.biHeight = pic.Height();

	int horzres = ::GetDeviceCaps(pd.hDC, HORZRES);	// ���ka pap�ru v tiskov�ch bodech
	int horzres0 = Round(horzres * 0.95);			// u�ite�n� ���ka pap�ru v bodech
	int vertres = ::GetDeviceCaps(pd.hDC, VERTRES); // v��ka pap�ru v tiskov�ch bodech
	int vertres0 = Round(vertres * 0.95);			// u�ite�n� v��ka pap�ru v bodech

	int scale = 1;									// m���tko * 4

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
		left, top, width2, height2,					// c�lov� parametry
		0, 0, pic.Width(), pic.Height(),			// zdrojov� parametry
		pic.DataData(),								// data obr�zku
		StdBitmapInfo, DIB_RGB_COLORS, SRCCOPY);

// ukon�en� tisku
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
// logick� hodnota

bool FGetMemoryBool()
{
	BOOL* buf = (BOOL*)DLLMemoryRead;
	bool data = (*buf != FALSE);
	buf++;
	DLLMemoryRead = buf;
	return data;
}


#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------
