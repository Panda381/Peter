
#include "Main.h"

/***************************************************************************\
*																			*
*					Prov�d�n� programu - ��seln� v�raz						*
*																			*
\***************************************************************************/

#pragma optimize("t", on)			// optimalizace na maxim�ln� rychlost

SYSTEMTIME	SystemTime;				// pomocn� prom�nn� pro obsluhu �asu

/////////////////////////////////////////////////////////////////////////////
// na�ten� ��seln�ho parametru p�eveden�ho na int - se��znuto sm�rem k nule

//int FInt() { return (int)FNum(); }


/////////////////////////////////////////////////////////////////////////////
// na�ten� �hlu s normalizac� rozsahu 0 a� 2pi

#pragma warning ( disable: 4725)	// hl�en� - probl�mov� instrukce

double FAngle()
{

// na�ten� �hlu
	double num = FNum();

// bude normalizace pro kladn� �hel > 2pi
	if (num >= pi2)
	{

// normalizace velk�ch kladn�ch �hl�
		if (num >= pi8)
		{
			double num2 = pi2;

#ifdef _M_IX86

			_asm {
				fld			num2		// na�ten� d�litele -> ST0
				fld			num			// na�ten� d�lence -> ST0, d�litel -> ST1
X1:				fprem					// zbytek -> ST0, v ST1 z�st�v� d�litel
				wait					// synchronizace
				fnstsw		ax			// na�ten� stavov�ho slova
				wait					// synchronizace
				sahf					// nastaven� stavov�ch p��znak�
				jp			X1			// operace ne�pln� - pokra�ov�n�
				fstp		num			// v�sledek operace
				fstp		st(0)		// zru�en� ST0 (d�litel)
			}

#else

			num = fmod(num, num2);

#endif

#define MULT 360						// tj. 5*3*3*2*2*2 (asi tak mo�n� n�sobky)
#define EPS 0.01						// asi tak pro rozezn�n� n�sobku

// zarovn�n� nep�esnost�, pokud se jedn� o n�jak� pod�l �hlu
			double num3 = num / pi * MULT;
			int m = Round(num3);		// to jsou mo�n� n�sobky
			double num4 = num3 - m;		// odchylka od n�sobk�
			if ((num4 <= EPS) && (num4 >= -EPS))
			{
				num = m * pi / MULT;	// zarovn�n� v�sledku

				if (num >= pi2) num += pi2m;
			}
		}

// jinak normalizace mal�ch kladn�ch �hl�
		else
		{
			do 
			{
				num += pi2m;
			} while (num >= pi2);
		}
	}
	else
	{

// bude normalizace pro z�porn� �hel < 0
		if (num < 0)
		{

// normalizace velk�ch z�porn�ch �hl�
			if (num <= pi8m)
			{
				num = -num;
				double num2 = pi2;

#ifdef _M_IX86

				_asm {
					fld			num2		// na�ten� d�litele -> ST0
					fld			num			// na�ten� d�lence -> ST0, d�litel -> ST1
X2:					fprem					// zbytek -> ST0, v ST1 z�st�v� d�litel
					wait					// synchronizace
					fnstsw		ax			// na�ten� stavov�ho slova
					wait					// synchronizace
					sahf					// nastaven� stavov�ch p��znak�
					jp			X2			// operace ne�pln� - pokra�ov�n�
					fstp		num			// v�sledek operace
					fstp		st(0)		// zru�en� ST0 (d�litel)
				}

#else

				num = fmod(num, num2);

#endif

				num = pi2 - num;
				if (num >= pi2) num += pi2m;

// zarovn�n� nep�esnost�, pokud se jedn� o n�jak� pod�l �hlu
				double num3 = num / pi * MULT;
				int m = Round(num3);		// to jsou mo�n� n�sobky
				double num4 = num3 - m;		// odchylka od n�sobk�
				if ((num4 <= EPS) && (num4 >= -EPS))
				{
					num = m * pi / MULT;	// zarovn�n� v�sledku

					if (num >= pi2) num += pi2m;
				}
			}

// jinak normalizace mal�ch z�porn�ch �hl�
			else
			{
				do 
				{
					num += pi2;
				} while (num < 0);
			}
		}
	}

	return num;
}

#pragma warning ( default: 4725)	// hl�en� - probl�mov� instrukce


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��seln�ho parametru p�eveden�ho na int se zaokrouhlen�m

int FIntR()
{
	int result;						// v�sledek operace
	double num;						// na�ten� ��slo

	num = FNum();					// na�ten� ��sla

#ifdef _M_IX86

	_asm {
		fld			num				// na�ten� ��sla do koprocesoru
		fistp		result			// p�evod ��sla na cel� ��slo
	}

#else

	result = Round(num);			// zaokrouhlen� ��sla

#endif

	return result;					// v�sledek operace
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��seln�ho parametru p�eveden�ho na DWORD se zaokrouhlen�m

DWORD FIntRN()
{
	__int64 result;					// v�sledek operace
	double num;						// na�ten� ��slo

	num = FNum();					// na�ten� ��sla

#ifdef _M_IX86

	_asm {
		fld			num				// na�ten� ��sla do koprocesoru
		fistp		result			// p�evod ��sla na cel� ��slo
	}

#else

	if (num >= 0x80000000)
	{
		result = Round(num - 0x80000000) + 0x80000000;
	}
	else
	{
		result = Round(num);			// zaokrouhlen� ��sla
	}

#endif

	return (DWORD)result;				// v�sledek operace
}

/////////////////////////////////////////////////////////////////////////////
// na�ten� ��seln�ho parametru p�eveden�ho na INT64 se zaokrouhlen�m

hyper FIntHN()
{
	hyper result;					// v�sledek operace
	double num;						// na�ten� ��slo

	num = FNum();					// na�ten� ��sla

#ifdef _M_IX86

	_asm {
		fld			num				// na�ten� ��sla do koprocesoru
		fistp		result			// p�evod ��sla na cel� ��slo
	}

#else

	if (num >= 0x80000000)
	{
		result = Round(num - 0x80000000) + 0x80000000;
	}
	else
	{
		if (num < 0)
		{
			result = -Round(-num);
		}
		else
		{
			result = Round(num);			// zaokrouhlen� ��sla
		}
	}

#endif

	return result;				// v�sledek operace
}

/////////////////////////////////////////////////////////////////////////////
// na�ten� ��seln�ho parametru p�eveden�ho na grafickou sou�adnici X

int FIntX()
{
	int result;						// v�sledek operace
	double num;						// na�ten� ��slo

	num = FNum() * ICONWIDTH;		// na�ten� ��sla

#ifdef _M_IX86

	_asm {
		fld			num				// na�ten� ��sla do koprocesoru
		fistp		result			// p�evod ��sla na cel� ��slo
	}

#else

	result = Round(num);			// zaokrouhlen� ��sla

#endif

	return result;					// v�sledek operace
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��seln�ho parametru vyn�soben�ho * 1000 (�as v milisekund�ch)

int FInt1000()
{
	int result;						// v�sledek operace
	double num;						// na�ten� ��slo

	num = FNum() * 1000;			// na�ten� ��sla

#ifdef _M_IX86

	_asm {
		fld			num				// na�ten� ��sla do koprocesoru
		fistp		result			// p�evod ��sla na cel� ��slo
	}

#else

	result = Round(num);			// zaokrouhlen� ��sla

#endif

	return result;					// v�sledek operace
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��seln�ho parametru vyn�soben�ho * ICONWIDTH * 2 (pr�m�r kruhu)

int FIntX2()
{
	int result;						// v�sledek operace
	double num;						// na�ten� ��slo

	num = FNum() * ICONWIDTH * 2;				// na�ten� ��sla

#ifdef _M_IX86

	_asm {
		fld			num				// na�ten� ��sla do koprocesoru
		fistp		result			// p�evod ��sla na cel� ��slo
	}

#else

	result = Round(num);			// zaokrouhlen� ��sla

#endif

	return result;					// v�sledek operace
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��seln�ho parametru vyn�soben�ho * 18.20648 (�as v �asov�ch impulsech)

int FInt18()
{
	int result;						// v�sledek operace
	double num;						// na�ten� ��slo

	num = FNum() * 18.20648;		// na�ten� ��sla

#ifdef _M_IX86

	_asm {
		fld			num				// na�ten� ��sla do koprocesoru
		fistp		result			// p�evod ��sla na cel� ��slo
	}

#else

	result = Round(num);			// zaokrouhlen� ��sla

#endif

	return result;					// v�sledek operace
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��seln�ho parametru s vyn�soben�m * 255 a omezen�m 0 a� 255

BYTE FInt255()
{
	int result;						// v�sledek operace
	double num;						// na�ten� ��slo
	num = FNum() * 255;				// na�ten� ��sla

#ifdef _M_IX86

	_asm {
		fld			num				// na�ten� ��sla do koprocesoru
		fistp		result			// p�evod ��sla na cel� ��slo
	}

#else

	result = Round(num);			// na�ten� ��sla se zaokrouhlen�m

#endif

	if ((DWORD)result > 255)
	{
		if (result < 0)
		{
			result = 0;
		}
		else
		{
			result = -1;
		}
	}

	return (BYTE)result;			// v�sledek operace
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��seln�ho parametru p�eveden�ho na souborov� �as

__int64 FNumFileTime()
{
	__int64 cas;					// pomocn� prom�nn� k na�ten� �asu
	double num;						// na�ten� ��slo
	num = FNum() * FILETIMEKONV;	// na�ten� ��sla

#ifdef _M_IX86

	_asm {
		fld			num				// na�ten� ��sla do koprocesoru
		fistp		cas				// p�evod ��sla na cel� ��slo
	}

#else

	cas = (__int64)(num + 0.5);

#endif

	if (((char*)&cas)[7] < 0) return 0;

	return cas;
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��seln�ho parametru p�eveden�ho na syst�mov� �as

void FNumSysTime()
{
	__int64 cas = FNumFileTime();	// na�ten� souborov�ho �asu
	::FileTimeToSystemTime((FILETIME*)&cas, &SystemTime);
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��seln�ho parametru p�eveden�ho na sv�tov� souborov� �as

__int64 FNumFileTimeUTC()
{
// na�ten� lok�ln�ho �asu
	__int64 cas = FNumFileTime();	// na�ten� lok�ln�ho �asu

// p�evod na sv�tov� �as
	__int64 cas2;
	::LocalFileTimeToFileTime((FILETIME*)&cas, (FILETIME*)&cas2);

	return cas2;
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� barvy

int FNumCol()
{
/*
	DWORD barva;					// na�ten� barva
	double num;						// na�ten� ��slo
	num = FNum();					// na�ten� ��sla

#ifdef _M_IX86

	_asm {
		fld			num				// na�ten� ��sla do koprocesoru
		fistp		barva			// p�evod ��sla na cel� ��slo
	}

#else

	barva = Round(num);				// zaokrouhlen� ��sla

#endif
*/
	return PalImport(FIntRN());		// v�sledek operace
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� sou�adnic Petra (ECX = adresa x, EDX = adresa y)

bool _fastcall PetrXY(int& x, int& y)
{
#ifdef _M_IX86

	__asm {
		mov			eax,[Sprite.m_Data]								// adresa dat sprajtu
		fld			qword ptr [eax + 0*TYPE CSprite]CSprite.m_CilX  // na�ten� sou�adnice X
		fistp		dword ptr [ecx]									// p�evod X na cel� ��slo
		fld			qword ptr [eax + 0*TYPE CSprite]CSprite.m_CilY  // na�ten� sou�adnice Y
		fistp		dword ptr [edx]									// p�evod Y na cel� ��slo
	}

#else

	x = Round(Sprite[0].CilX());
	y = Round(Sprite[0].CilY());

#endif

// kontrola platnosti sou�adnice
	return (((DWORD)x < (DWORD)WidthN) && ((DWORD)y < (DWORD)HeightN));
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� sou�adnic Petry (ECX = adresa x, EDX = adresa y)

bool _fastcall Petr2XY(int& x, int& y)
{
#ifdef _M_IX86

	__asm {
		mov			eax,[Sprite.m_Data]								// adresa dat sprajtu
		fld			qword ptr [eax + 1*TYPE CSprite]CSprite.m_CilX  // na�ten� sou�adnice X
		fistp		dword ptr [ecx]									// p�evod X na cel� ��slo
		fld			qword ptr [eax + 1*TYPE CSprite]CSprite.m_CilY  // na�ten� sou�adnice Y
		fistp		dword ptr [edx]									// p�evod Y na cel� ��slo
	}

#else

	x = Round(Sprite[1].CilX());
	y = Round(Sprite[1].CilY());

#endif

// kontrola platnosti sou�adnice
	return (((DWORD)x < (DWORD)WidthN) && ((DWORD)y < (DWORD)HeightN));
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� pozice Petra (NULL=neplatn�)

MAPITEM* PetrXYMap()
{
	int x;			// sou�adnice X Petra
	int y;			// sou�adnice Y Petra

#ifdef _M_IX86

	__asm {
		mov			eax,[Sprite.m_Data]								// adresa dat sprajtu
		fld			qword ptr [eax + 0*TYPE CSprite]CSprite.m_CilX  // na�ten� sou�adnice X
		fistp		dword ptr x										// p�evod X na cel� ��slo
		fld			qword ptr [eax + 0*TYPE CSprite]CSprite.m_CilY  // na�ten� sou�adnice Y
		fistp		dword ptr y										// p�evod Y na cel� ��slo
	}

#else

	x = Round(Sprite[0].CilX());
	y = Round(Sprite[0].CilY());

#endif

// kontrola platnosti sou�adnice
	if (((DWORD)x < (DWORD)WidthN) && ((DWORD)y < (DWORD)HeightN))
	{
		return &Map[0].At(x, y);
	}
	else
	{
		return NULL;
	}
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� pozice Petry (NULL=neplatn�)

MAPITEM* Petr2XYMap()
{
	int x;			// sou�adnice X Petra
	int y;			// sou�adnice Y Petra

#ifdef _M_IX86

	__asm {
		mov			eax,[Sprite.m_Data]								// adresa dat sprajtu
		fld			qword ptr [eax + 1*TYPE CSprite]CSprite.m_CilX  // na�ten� sou�adnice X
		fistp		dword ptr x										// p�evod X na cel� ��slo
		fld			qword ptr [eax + 1*TYPE CSprite]CSprite.m_CilY  // na�ten� sou�adnice Y
		fistp		dword ptr y										// p�evod Y na cel� ��slo
	}

#else

	x = Round(Sprite[1].CilX());
	y = Round(Sprite[1].CilY());

#endif

// kontrola platnosti sou�adnice
	if (((DWORD)x < (DWORD)WidthN) && ((DWORD)y < (DWORD)HeightN))
	{
		return &Map[0].At(x, y);
	}
	else
	{
		return NULL;
	}
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� p��t� sou�adnice Petra (ECX = adresa x, EDX = adresa y)

bool _fastcall PetrXYNext(int& x, int& y)
{
#ifdef _M_IX86

	__asm {
		mov			eax,[Sprite.m_Data]								// adresa dat sprajtu
		fld			qword ptr [eax + 0*TYPE CSprite]CSprite.m_CilX  // na�ten� sou�adnice X
		fistp		dword ptr [ecx]									// p�evod X na cel� ��slo
		fld			qword ptr [eax + 0*TYPE CSprite]CSprite.m_CilY  // na�ten� sou�adnice Y
		fistp		dword ptr [edx]									// p�evod Y na cel� ��slo
	}

#else

	x = Round(Sprite[0].CilX());
	y = Round(Sprite[0].CilY());

#endif

	switch (16 * Sprite[0].SmerN() / Sprite[0].Smer())
	{
	case 1:					// vpravo m�rn� vzh�ru
	case 2:					// vpravo vzh�ru
	case 3:					// m�rn� vpravo vzh�ru
		x++;

	case 4:					// vzh�ru
		y++;
		break;

	case 5:					// m�rn� vlevo vzh�ru
	case 6:					// vlevo vzh�ru
	case 7:					// vlevo m�rn� vzh�ru
		y++;

	case 8:					// vlevo
		x--;
		break;

	case 9:					// vlevo m�rn� dol�
	case 10:				// vlevo dol�
	case 11:				// m�rn� vlevo dol�
		x--;

	case 12:				// dol�
		y--;
		break;

	case 13:				// m�rn� vpravo dol�
	case 14:				// vpravo dol�
	case 15:				// vpravo m�rn� dol�
		y--;

	case 0:					// vpravo
		x++;
		break;
	}

// kontrola platnosti sou�adnice
	return (((DWORD)x < (DWORD)WidthN) && ((DWORD)y < (DWORD)HeightN));
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� p��t� sou�adnice Petry (ECX = adresa x, EDX = adresa y)

bool _fastcall Petr2XYNext(int& x, int& y)
{
#ifdef _M_IX86

	__asm {
		mov			eax,[Sprite.m_Data]								// adresa dat sprajtu
		fld			qword ptr [eax + 1*TYPE CSprite]CSprite.m_CilX  // na�ten� sou�adnice X
		fistp		dword ptr [ecx]									// p�evod X na cel� ��slo
		fld			qword ptr [eax + 1*TYPE CSprite]CSprite.m_CilY  // na�ten� sou�adnice Y
		fistp		dword ptr [edx]									// p�evod Y na cel� ��slo
	}

#else

	x = Round(Sprite[1].CilX());
	y = Round(Sprite[1].CilY());

#endif

	switch (16 * Sprite[1].SmerN() / Sprite[1].Smer())
	{
	case 1:					// vpravo m�rn� vzh�ru
	case 2:					// vpravo vzh�ru
	case 3:					// m�rn� vpravo vzh�ru
		x++;

	case 4:					// vzh�ru
		y++;
		break;

	case 5:					// m�rn� vlevo vzh�ru
	case 6:					// vlevo vzh�ru
	case 7:					// vlevo m�rn� vzh�ru
		y++;

	case 8:					// vlevo
		x--;
		break;

	case 9:					// vlevo m�rn� dol�
	case 10:				// vlevo dol�
	case 11:				// m�rn� vlevo dol�
		x--;

	case 12:				// dol�
		y--;
		break;

	case 13:				// m�rn� vpravo dol�
	case 14:				// vpravo dol�
	case 15:				// vpravo m�rn� dol�
		y--;

	case 0:					// vpravo
		x++;
		break;
	}

// kontrola platnosti sou�adnice
	return (((DWORD)x < (DWORD)WidthN) && ((DWORD)y < (DWORD)HeightN));
}


/***************************************************************************\
*																			*
*									funkce									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// funkce s n�vratem ��seln� hodnoty

double FNumFunc()
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
	double result = Real[Real.Num() - res];

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
// ��seln� konstanta

double FNumConst() { return ExecItem[-1].Double; }


/////////////////////////////////////////////////////////////////////////////
// glob�ln� prom�nn� (Data = index)

double FNumObj()
{
// index
	int inx = ExecItem[-1].Data;

// data
	return Real[inx];
}


/////////////////////////////////////////////////////////////////////////////
// globaln� prom�nn� v seznamu (Data = index, List = seznam)

double FNumObjList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index prom�nn�, inkrementace indexu seznamu
	int inx = ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// data
	return Real[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lok�ln� prom�nn� (Data = index)

double FNumLoc()
{
// index prom�nn�
	int inx = Real.Num() - ExecItem[-1].Data;

// data
	return Real[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lok�ln� prom�nn� v seznamu (Data = index, List = seznam)

double FNumLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index prom�nn�, inkrementace indexu seznamu
	int inx = Real.Num() - ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// data
	return Real[inx];
}


/////////////////////////////////////////////////////////////////////////////
// index glob�ln�ho seznamu

double FGetList()
{
// index seznamu
	int list = ExecItem[-1].List;

// hodnota indexu
	return List[list].Index;
}


/////////////////////////////////////////////////////////////////////////////
// index lok�ln�ho seznamu

double FGetListLoc()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// hodnota indexu
	return List[list].Index;
}


/////////////////////////////////////////////////////////////////////////////
// inkrementace glob�ln�ho seznamu

double FGetListAuto()
{
// index seznamu
	int list = ExecItem[-1].List;

// nastaven� nov� hodnoty inkrementace
	return List[list].Auto;
}


/////////////////////////////////////////////////////////////////////////////
// inkrementace lok�ln�ho seznamu

double FGetListAutoLoc()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// nastaven� nov� hodnoty inkrementace
	return List[list].Auto;
}


/////////////////////////////////////////////////////////////////////////////
// sou�adnice X glob�ln�ho sprajtu (Data = index)

double FGetSpriteX()
{
// index sprajtu
	int inx = ExecItem[-1].Data;

// sou�adnice X sprajtu
	return Sprite[inx].AktX();
}


/////////////////////////////////////////////////////////////////////////////
// sou�adnice X sprajtu v seznamu (Data = index, List = seznam)

double FGetSpriteXList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index sprajtu, inkrementace indexu seznamu
	int inx = ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// sou�adnice X sprajtu
	return Sprite[inx].AktX();
}


/////////////////////////////////////////////////////////////////////////////
// sou�adnice X lok�ln�ho sprajtu (Data = index)

double FGetSpriteXLoc()
{
// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;

// sou�adnice X sprajtu
	return Sprite[inx].AktX();
}


/////////////////////////////////////////////////////////////////////////////
// sou�adnice X lok�ln�ho sprajtu v seznamu (Data = index, List = seznam)

double FGetSpriteXLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index sprajtu, inkrementace indexu seznamu
	int inx = Sprite.Num() - ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// sou�adnice X sprajtu
	return Sprite[inx].AktX();
}


/////////////////////////////////////////////////////////////////////////////
// sou�adnice Y glob�ln�ho sprajtu (Data = index)

double FGetSpriteY()
{
// index sprajtu
	int inx = ExecItem[-1].Data;

// sou�adnice Y sprajtu
	return Sprite[inx].AktY();
}


/////////////////////////////////////////////////////////////////////////////
// sou�adnice Y sprajtu v seznamu (Data = index, List = seznam)

double FGetSpriteYList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index sprajtu, inkrementace indexu seznamu
	int inx = ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// sou�adnice Y sprajtu
	return Sprite[inx].AktY();
}


/////////////////////////////////////////////////////////////////////////////
// sou�adnice Y lok�ln�ho sprajtu (Data = index)

double FGetSpriteYLoc()
{
// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;

// sou�adnice Y sprajtu
	return Sprite[inx].AktY();
}


/////////////////////////////////////////////////////////////////////////////
// sou�adnice Y lok�ln�ho sprajtu v seznamu (Data = index, List = seznam)

double FGetSpriteYLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index sprajtu, inkrementace indexu seznamu
	int inx = Sprite.Num() - ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// sou�adnice Y sprajtu
	return Sprite[inx].AktY();
}


/////////////////////////////////////////////////////////////////////////////
// sm�r glob�ln�ho sprajtu (Data = index)

double FGetSpriteSmer()
{
// index sprajtu
	int inx = ExecItem[-1].Data;

// sm�r sprajtu
	return Sprite[inx].SmerR();
}


/////////////////////////////////////////////////////////////////////////////
// sm�r sprajtu v seznamu (Data = index, List = seznam)

double FGetSpriteSmerList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index sprajtu, inkrementace indexu seznamu
	int inx = ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// sm�r sprajtu
	return Sprite[inx].SmerR();
}


/////////////////////////////////////////////////////////////////////////////
// sm�r lok�ln�ho sprajtu (Data = index)

double FGetSpriteSmerLoc()
{
// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;

// sm�r sprajtu
	return Sprite[inx].SmerR();
}


/////////////////////////////////////////////////////////////////////////////
// sm�r lok�ln�ho sprajtu v seznamu (Data = index, List = seznam)

double FGetSpriteSmerLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index sprajtu, inkrementace indexu seznamu
	int inx = Sprite.Num() - ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// sm�r sprajtu
	return Sprite[inx].SmerR();
}


/////////////////////////////////////////////////////////////////////////////
// f�ze glob�ln�ho sprajtu (Data = index)

double FGetSpriteFaze()
{
// index sprajtu
	int inx = ExecItem[-1].Data;

// f�ze sprajtu
	return SpriteNextFaze(inx);
}


/////////////////////////////////////////////////////////////////////////////
// f�ze sprajtu v seznamu (Data = index, List = seznam)

double FGetSpriteFazeList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index sprajtu, inkrementace indexu seznamu
	int inx = ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// f�ze sprajtu
	return SpriteNextFaze(inx);
}


/////////////////////////////////////////////////////////////////////////////
// f�ze lok�ln�ho sprajtu (Data = index)

double FGetSpriteFazeLoc()
{
// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;

// f�ze sprajtu
	return SpriteNextFaze(inx);
}


/////////////////////////////////////////////////////////////////////////////
// f�ze lok�ln�ho sprajtu v seznamu (Data = index, List = seznam)

double FGetSpriteFazeLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index sprajtu, inkrementace indexu seznamu
	int inx = Sprite.Num() - ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// f�ze sprajtu
	return SpriteNextFaze(inx);
}


/////////////////////////////////////////////////////////////////////////////
// stav glob�ln�ho sprajtu (Data = index)

double FGetSpriteStatus()
{
// index sprajtu
	int inx = ExecItem[-1].Data;

// stav sprajtu
	return Sprite[inx].Status();
}


/////////////////////////////////////////////////////////////////////////////
// stav sprajtu v seznamu (Data = index, List = seznam)

double FGetSpriteStatusList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index sprajtu, inkrementace indexu seznamu
	int inx = ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// stav sprajtu
	return Sprite[inx].Status();
}


/////////////////////////////////////////////////////////////////////////////
// stav lok�ln�ho sprajtu (Data = index)

double FGetSpriteStatusLoc()
{
// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;

// stav sprajtu
	return Sprite[inx].Status();
}


/////////////////////////////////////////////////////////////////////////////
// stav lok�ln�ho sprajtu v seznamu (Data = index, List = seznam)

double FGetSpriteStatusLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index sprajtu, inkrementace indexu seznamu
	int inx = Sprite.Num() - ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// stav sprajtu
	return Sprite[inx].Status();
}


/***************************************************************************\
*																			*
*							obsluha Petra a Petry							*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// Petr - sm�r

double FGetSmer() { return Sprite[0].SmerR(); }


/////////////////////////////////////////////////////////////////////////////
// Petra - sm�r

double FGetSmer2() { return Sprite[1].SmerR(); }


/////////////////////////////////////////////////////////////////////////////
// Petr - pozice X

double FGetPozX() { return Sprite[0].AktX(); }


/////////////////////////////////////////////////////////////////////////////
// Petra - pozice X

double FGetPozX2() { return Sprite[1].AktX(); }


/////////////////////////////////////////////////////////////////////////////
// Petr - pozice Y

double FGetPozY() { return Sprite[0].AktY(); }


/////////////////////////////////////////////////////////////////////////////
// Petra - pozice Y

double FGetPozY2() { return Sprite[1].AktY(); }


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��sla - Petr

double _fastcall FGetSwcN(int mask, char rot)
{
// na�ten� pozice Petra
	MAPITEM* item = PetrXYMap();

// na�ten� ��sla, je-li platn� pozice
	if (item != NULL)
	{
		return (((int)item->Param & mask) >> rot);
	}
	return 0;
}

double FGetSwcN1() { return FGetSwcN(MAP_VALUE1_MASK, MAP_VALUE1_ROT); };
double FGetSwcN2() { return FGetSwcN(MAP_VALUE2_MASK, MAP_VALUE2_ROT); };
double FGetSwcN3() { return FGetSwcN(MAP_VALUE3_MASK, MAP_VALUE3_ROT); };


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��sla - Petra

double _fastcall FGet2SwcN(int mask, char rot)
{
// na�ten� pozice Petry
	MAPITEM* item = Petr2XYMap();

// na�ten� ��sla, je-li platn� pozice
	if (item != NULL)
	{
		return (((int)item->Param & mask) >> rot);
	}
	return 0;
}

double FGetSwcN12() { return FGet2SwcN(MAP_VALUE1_MASK, MAP_VALUE1_ROT); };
double FGetSwcN22() { return FGet2SwcN(MAP_VALUE2_MASK, MAP_VALUE2_ROT); };
double FGetSwcN32() { return FGet2SwcN(MAP_VALUE3_MASK, MAP_VALUE3_ROT); };


/***************************************************************************\
*																			*
*								v�po�ty										*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// inkrementace

double FInc() { return FNum() + 1; };


/////////////////////////////////////////////////////////////////////////////
// dekrementace

double FDec() { return FNum() - 1; };


/////////////////////////////////////////////////////////////////////////////
// sinus

double FSin() { return sin(FNum()); };


/////////////////////////////////////////////////////////////////////////////
// cosinus

double FCos() { return cos(FNum()); };


/////////////////////////////////////////////////////////////////////////////
// tangens

double FTg() { return tan(FNum()); };


/////////////////////////////////////////////////////////////////////////////
// cotangens

double FCoTg() { return 1/tan(FNum()); };


/////////////////////////////////////////////////////////////////////////////
// arcsinus

double FArcSin() { return asin(FNum()); }


/////////////////////////////////////////////////////////////////////////////
// arccosinus

double FArcCos() { return acos(FNum()); };


/////////////////////////////////////////////////////////////////////////////
// arctangens

double FArcTg() { return atan(FNum()); };


/////////////////////////////////////////////////////////////////////////////
// arccotangens

double FArcCoTg() { return atan(1/FNum()); };


/////////////////////////////////////////////////////////////////////////////
// cel� ��slo

double FNumInt() { return (int)FNum(); };


/////////////////////////////////////////////////////////////////////////////
// desetinn� ��st ��sla

double FFrac()
{
	double num = FNum();
	return num - (int)num;
}


/////////////////////////////////////////////////////////////////////////////
// absolutn� hodnota

double FAbs() { return fabs(FNum()); };


/////////////////////////////////////////////////////////////////////////////
// odmocnina

double FSqrt() { return sqrt(FNum()); };


/////////////////////////////////////////////////////////////////////////////
// faktori�l

double FFact()
{
// na�ten� ��sla pro v�po�et faktori�lu
	int citac = FIntR();

// kladn� hodnota v povolen�m rozsahu
	if ((DWORD)citac <= MAXFACT+1) return FactTab[citac];

// p�ete�en� kladn�m sm�rem
	if (citac > 0) return FactTab[MAXFACT+1];

// absolutn� hodnota ��sla
	citac = -citac;

// p�ete�en� z�porn�m sm�rem
	if (citac > MAXFACT) return -FactTab[MAXFACT+1];

// hodnota v z�porn�m sm�ru (lich� ��sla jsou z�porn�)
	if (citac & 1) return -FactTab[citac];
	return FactTab[citac];
}


/////////////////////////////////////////////////////////////////////////////
// n�hodn� ��slo s n�sobkem

double FRand() { return FNum() * Rand(); };


/////////////////////////////////////////////////////////////////////////////
// n�hodn� ��slo bez n�sobku

double FRand0() { return Rand(); };


/////////////////////////////////////////////////////////////////////////////
// p�evod stup�� na radi�ny

double FDegRad() { return FNum() * degrad; };


/////////////////////////////////////////////////////////////////////////////
// p�evod radi�n� na stupn�

double FRadDeg() { return FNum() * raddeg; };


/////////////////////////////////////////////////////////////////////////////
// dekadick� logaritmus

double FLog() { return log10(FNum()); }


/////////////////////////////////////////////////////////////////////////////
// p�irozen� logaritmus

double FLn() { return log(FNum()); }


/////////////////////////////////////////////////////////////////////////////
// p�irozen� exponent

double FExp() { return exp(FNum()); }


/////////////////////////////////////////////////////////////////////////////
// oper�tor ADD (Data = po�et prvk� - 1)

double FAdd()
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data;

// na�ten� prvn�ho ��sla
	double num = FNum();

// cyklus p�es dal�� ��sla
	for (; i > 0; i--)
	{

// operace s dal��m ��slem
		num += FNum();
	}
	return num;
}


/////////////////////////////////////////////////////////////////////////////
// oper�tor SUB pro 1 prvek

double FSub1() { return -FNum(); };


/////////////////////////////////////////////////////////////////////////////
// oper�tor SUB (Data = po�et prvk� - 1)

double FSub()
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data;

// na�ten� prvn�ho ��sla
	double num = FNum();

// cyklus p�es dal�� ��sla
	for (; i > 0; i--)
	{

// operace s dal��m ��slem (tento z�pis je efektivn�j�� pro p�eklad)
		num = -(FNum() - num);
	}
	return num;
}


/////////////////////////////////////////////////////////////////////////////
// oper�tor MUL (Data = po�et prvk� - 1)

double FMul()
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data;

// na�ten� prvn�ho ��sla
	double num = FNum();

// cyklus p�es dal�� ��sla
	for (; i > 0; i--)
	{

// operace s dal��m ��slem
		num *= FNum();
	}
	return num;
}

/////////////////////////////////////////////////////////////////////////////
// oper�tor DIV pro 1 prvek

double FDiv1() { return 1/FNum(); };


/////////////////////////////////////////////////////////////////////////////
// oper�tor DIV (Data = po�et prvk� - 1)

double FDiv()
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data;

// na�ten� prvn�ho ��sla
	double num = FNum();

// cyklus p�es dal�� ��sla
	for (; i > 0; i--)
	{

// na�ten� dal��ho ��sla
		double num2 = FNum();

// operace s dal��m ��slem (v p��pad� 0 / 0 bude v�sledek 1)
		if (num == num2)
		{
			num = 1;
		}
		else
		{
			num /= num2;
		}
	}
	return num;
}

/////////////////////////////////////////////////////////////////////////////
// operace MOD

#pragma warning ( disable: 4725)	// hl�en� - probl�mov� instrukce

double NumMod(double num1, double num2)
{
	if (num2 <= 0)
	{
		if (num2 == 0) return 0;
		return -NumMod(-num1, -num2);
	}

	double num;

	if (num1 < 0)
	{
		num = num2 - NumMod(-num1, num2);
		if (num == num2) num = 0;
		return num;
	}

#ifdef _M_IX86

	_asm {
		fld			num2			// na�ten� d�litele -> ST0
		fld			num1			// na�ten� d�lence -> ST0, d�litel -> ST1
X1:		fprem						// zbytek -> ST0, v ST1 z�st�v� d�litel
		wait						// synchronizace
		fnstsw		ax				// na�ten� stavov�ho slova
		wait						// synchronizace
		sahf						// nastaven� stavov�ch p��znak�
		jp			X1				// operace ne�pln� - pokra�ov�n�
		fstp		num				// v�sledek operace
		fstp		st(0)			// zru�en� ST0 (d�litel)
	}

#else

		num = fmod(num1, num2);

#endif

		return num;
}

#pragma warning ( default: 4725)	// hl�en� - probl�mov� instrukce

/////////////////////////////////////////////////////////////////////////////
// oper�tor MOD (Data = po�et prvk� - 1)

double FMod()
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data;

// na�ten� prvn�ho ��sla
	double num = FNum();

// cyklus p�es dal�� ��sla
	for (; i > 0; i--)
	{

// operace s dal��m ��slem
		double num2 = FNum();			// na�ten� dal��ho ��sla
		num = NumMod(num, num2);
	}
	return num;
}

/////////////////////////////////////////////////////////////////////////////
// oper�tor MOCNINA (Data = po�et prvk� - 1)

double FMocn()
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data;

// na�ten� prvn�ho ��sla
	double num = FNum();

// cyklus p�es dal�� ��sla
	for (; i > 0; i--)
	{

// umocn�n� dal��m ��slem
		num = pow(num, FNum());
	}
	return num;
}


/////////////////////////////////////////////////////////////////////////////
// druh� mocnina

double FNumSqr()
{
	double cislo = FNum();
	return cislo * cislo;
}


/***************************************************************************\
*																			*
*								obsluha grafiky								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// nastaven� jasu barvy

double FColorLevel()
{
// na�ten� barvy, p�evod na z�kladn� jas barvy
	int col = FNumCol();
	if ((col == BackCol) || (col == ShadCol)) col = BlackCol;
	col = (col - ResCols) / ColLev * ColLev + ResCols;

// p��prava po�tu odst�n� pro barvu
	int cols = ColLev + 1;				// po�et odst�n� pro b�n� barvy
	if (col >= StdColors - 2*ColLev)	// je to b�l� nebo �ed�?
	{
		col = StdColors - 2*ColLev;		// korekce na b�lou barvu
		cols = 2*ColLev;				// po�et odst�n� pro b�lou barvu
	}

// na�ten� jasu barvy
	double level = FNum()*(cols-1);

// p�evod na index jasu barvy
	int jas;
	
#ifdef _M_IX86

	_asm {
		fld			level				// na�ten� ��sla do koprocesoru
		fistp		jas					// p�evod ��sla na cel� ��slo
	}

#else

	jas = Round(level);					// zaokrouhlen� ��sla

#endif
	
// omezen� indexu jasu barvy
	if ((DWORD)jas >= (DWORD)cols)
	{
		if (jas < 0)
		{
			jas = 0;
		}
		else
		{
			jas = cols-1;
		}
	}

// pro jas 0 se pou�ije �ern� barva
	if (jas == 0) return 0;

// v�sledn� barva
	return PalExport(col + cols - 1 - jas);
}


/////////////////////////////////////////////////////////////////////////////
// synt�za barvy

double FColorSynth()
{
	DWORD color;
	((BYTE*)&color)[0] = FInt255();		// �erven�
	((BYTE*)&color)[1] = FInt255();		// zelen�
	((BYTE*)&color)[2] = FInt255();		// modr�
	((BYTE*)&color)[3] = (BYTE)(FInt255() ^ 0xff);// alfa
	return color;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� barvy bodu

double FGetPoint()
{
// na�ten� sou�adnic bodu
	int x = FIntX();
	int y = FIntY();

// implicitn� barva - pozad�
	int col = BackCol;

// kontrola platnosty sou�adnice bodu
	if (((DWORD)x < (DWORD)Width) && ((DWORD)y < (DWORD)Height))
	{

// na�ten� bodu
		col = BackBuf[x + y*WidthByte];
	}

// v�sledn� barva
	return PalExport(col);
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� ���ky obr�zku

double FGetPicWidth()
{
	CPicture pic;
	FPicture(pic);
	double n = (double)pic.Width()/ICONWIDTH;
	return n;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� v��ky obr�zku

double FGetPicHeight()
{
	CPicture pic;
	FPicture(pic);
	double n = (double)pic.Height()/ICONHEIGHT;
	return n;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� ���ky pole k zobrazen� centrovan�ho textu

double FGetGraphAreaWidth()
{
	double n = (double)GraphAreaWidth/ICONWIDTH;
	return n;
}


/***************************************************************************\
*																			*
*							obsluha �asu									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// na�ten� aktu�ln�ho �asu

double FGetTimeCurrent()
{
// na�ten� aktu�ln�ho �asu
	SYSTEMTIME systime;
	::GetLocalTime(&systime);

// p�evod na souborov� �as
	__int64 cas;
	::SystemTimeToFileTime(&systime, (FILETIME*)&cas);

// n�vrat v�sledku
	return (double)cas / FILETIMEKONV;
}


/////////////////////////////////////////////////////////////////////////////
// p�evod na zv�tov� �as

double FTimeUtc()
{
// na�ten� �asu s p�evodem na sv�tov� �as
	__int64 cas = FNumFileTimeUTC();

// n�vrat v�sledku
	return (double)cas / FILETIMEKONV;
}


/////////////////////////////////////////////////////////////////////////////
// p�evod na m�stn� �as

double FTimeLocal()
{
// na�ten� �asu k p�evodu
	__int64 cas = FNumFileTime();

// p�evod ��sla
	__int64 cas2;
	::FileTimeToLocalFileTime((FILETIME*)&cas, (FILETIME*)&cas2);

// n�vrat v�sledku
	return (double)cas2 / FILETIMEKONV;
}


/////////////////////////////////////////////////////////////////////////////
// synt�za �asu

double FTimeComp()
{
// na�ten� slo�ek �asu
	SYSTEMTIME systime;

	int i = FIntR();
	if (i < 1601) i = 1601;
	if (i > 9999) i = 9999;		// system povoluje a� 30000, ale kv�li zobrazen� rad�ji jen 4 ��slice
	systime.wYear = (WORD)i;

	i = FIntR();
	if (i < 1) i = 1;
	if (i > 12) i = 12;
	systime.wMonth = (WORD)i;

	i = FIntR();
	if (i < 1) i = 1;
	if (i > 31) i = 31;
	systime.wDay = (WORD)i;

	i = FIntR();
	if ((DWORD)i > (DWORD)23)
	{
		if (i < 0)
		{
			i = 0;
		}
		else
		{
			i = 23;
		}
	}
	systime.wHour = (WORD)i;

	i = FIntR();
	if ((DWORD)i > (DWORD)59)
	{
		if (i < 0)
		{
			i = 0;
		}
		else
		{
			i = 59;
		}
	}
	systime.wMinute = (WORD)i;

	i = FIntR();
	if ((DWORD)i > (DWORD)59)
	{
		if (i < 0)
		{
			i = 0;
		}
		else
		{
			i = 59;
		}
	}
	systime.wSecond = (WORD)i;

	i = FIntR();
	if ((DWORD)i > (DWORD)999)
	{
		if (i < 0)
		{
			i = 0;
		}
		else
		{
			i = 999;
		}
	}
	systime.wMilliseconds = (WORD)i;

// proveden� konverze
	__int64 cas;
	if (!::SystemTimeToFileTime(&systime, (FILETIME*)&cas))
	{
		if (systime.wDay != 31)
		{
			return 0;
		}

		systime.wDay = 30;

		if (!::SystemTimeToFileTime(&systime, (FILETIME*)&cas))
		{
			systime.wDay = 29;

			if (!::SystemTimeToFileTime(&systime, (FILETIME*)&cas))
			{
				systime.wDay = 28;

				if (!::SystemTimeToFileTime(&systime, (FILETIME*)&cas))
				{
					return 0;
				}
			}
		}
	}
	return (double)cas / FILETIMEKONV;
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� roku z �asu

double FGetTimeYear() { FNumSysTime(); return SystemTime.wYear; }


/////////////////////////////////////////////////////////////////////////////
// na�ten� m�s�ce z �asu

double FGetTimeMonth() { FNumSysTime(); return SystemTime.wMonth; }


/////////////////////////////////////////////////////////////////////////////
// na�ten� dne z �asu

double FGetTimeDay() { FNumSysTime(); return SystemTime.wDay; }


/////////////////////////////////////////////////////////////////////////////
// na�ten� dne v t�dnu z �asu

double FGetTimeWeek() { FNumSysTime(); return SystemTime.wDayOfWeek; }


/////////////////////////////////////////////////////////////////////////////
// na�ten� hodiny z �asu

double FGetTimeHour() { FNumSysTime(); return SystemTime.wHour; }


/////////////////////////////////////////////////////////////////////////////
// na�ten� minuty z �asu

double FGetTimeMin() { FNumSysTime(); return SystemTime.wMinute; }


/////////////////////////////////////////////////////////////////////////////
// na�ten� sekundy z �asu

double FGetTimeSec() { FNumSysTime(); return SystemTime.wSecond; }


/////////////////////////////////////////////////////////////////////////////
// na�ten� milisekundy z �asu

double FGetTimeMSec() { FNumSysTime(); return SystemTime.wMilliseconds; }


/***************************************************************************\
*																			*
*							obsluha my�i									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// na�ten� sou�adnice X my�i

double FMouseX() { return MouseX; }

/////////////////////////////////////////////////////////////////////////////
// na�ten� sou�adnice Y my�i

double FMouseY() { return MouseY; }


/***************************************************************************\
*																			*
*							obsluha dialog�									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// dialog box s n�vratem hodnoty

double FNumDialogBox()
{
// na�ten� textu nadpisu okna
	CString nadpis;
	FText(nadpis);

// na�ten� textu hl�en�
	CString text;
	FText(text);

// ��slo ikony
	int icon = FIntR();

// typ tla��tek
	int but = FIntR();

// implicitn� tla��tko
	int def = FIntR();

// pro nadpis se pou�ije prvn� ��dek textu
	int crlf = nadpis.Find(13);
	if (crlf >= 0) nadpis.Delete(crlf);

// korekce povolen�ch hodnot
	if ((DWORD)icon > 4) icon = 0;
	if ((DWORD)but > 5) but = 0;
	if (def < 1) def = 1;

	int buts;

	switch (but)
	{
	case 0: buts = 1;
			break;

	case 2:
	case 3:	buts = 3;
			break;

	default: buts = 2;
	}

	if (def > buts) def = buts;

// vypnut� celoobrazovkov�ho m�du
//	SetFullScreen(false);

#ifndef _MINI
	D3Render(true);
#endif // _MINI

// p�ekreslen� displeje p�ed vyvol�n�m dialogu
	ReDisp();

// vyvol�n� dialogu
	IsOverlay = true;

	int result = ::MessageBox(Win[DialogParent].Wnd, text, nadpis,
		icon * 0x10 + but + (def-1)*0x100);

	IsOverlay = false;
	if (FullScreen) RePaint(&Win[0]);

	return result;
}


/***************************************************************************\
*																			*
*							obsluha soubor�									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� velikosti disku

double FGetDiskSize()
{	
// na�ten� informac� o disku
	GetDiskSpace();

// velikost disku
	return (double)DiskSize;
}


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� voln�ho m�sta disku

double FGetDiskFree()
{	
// na�ten� informac� o disku
	GetDiskSpace();

// voln� m�sto disku
	return (double)DiskFree;
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� s�riov�ho ��sla disku

double FGetDiskSerial()
{
// prom�nn� k na�ten� ��sla
	__int64 serial = 0;

// na�ten� s�riov�ho ��sla disku
	::GetVolumeInformation(NULL, NULL, 0, (DWORD*)&serial, NULL, NULL, NULL, 0);

// na�ten� �daj (p�i chyb� nebo kdy� ��slo nen� z�stane = 0)
	return (double)serial;
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� informac� o vstupn�m souboru (false = chyba)

bool FGetFileInfo(WIN32_FIND_DATA* wfd)
{
// uzav�en� soubor�
	FileClose();

// kontrola zad�n� jm�na
	if (FileRead.IsNotEmpty())
	{

// nalezen� souboru
		HANDLE find = ::FindFirstFile(FileRead, wfd);
		if (find != INVALID_HANDLE_VALUE)
		{

// operace OK
			::FindClose(find);
			return true;
		}
	}

// chyba
	FileError = true;
	return false;
}


/////////////////////////////////////////////////////////////////////////////
// �as vytvo�en� souboru

double FGetFileCreate()
{
// na�ten� informac� o souboru
	WIN32_FIND_DATA wfd;
	if (FGetFileInfo(&wfd))
	{

// p�evod �asu na lok�ln� �as
		__int64 cas;
		if (::FileTimeToLocalFileTime(&wfd.ftCreationTime, (FILETIME*)&cas))
		{

// n�vrat v�sledku
			return (double)cas / FILETIMEKONV;
		}
	}

// chyba
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// �as �ten� ze souboru

double FGetFileOpen()
{
// na�ten� informac� o souboru
	WIN32_FIND_DATA wfd;
	if (FGetFileInfo(&wfd))
	{

// p�evod �asu na lok�ln� �as
		__int64 cas;
		if (::FileTimeToLocalFileTime(&wfd.ftLastAccessTime, (FILETIME*)&cas))
		{

// n�vrat v�sledku
			return (double)cas / FILETIMEKONV;
		}
	}

// chyba
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// �as z�pisu do souboru

double FGetFileModi()
{
// na�ten� informac� o souboru
	WIN32_FIND_DATA wfd;
	if (FGetFileInfo(&wfd))
	{

// p�evod �asu na lok�ln� �as
		__int64 cas;
		if (::FileTimeToLocalFileTime(&wfd.ftLastWriteTime, (FILETIME*)&cas))
		{

// n�vrat v�sledku
			return (double)cas / FILETIMEKONV;
		}
	}

// chyba
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// offset pro �ten� ze souboru

double FGetFileReadOff() { return FileReadOff; };


/////////////////////////////////////////////////////////////////////////////
// offset pro z�pis do souboru

double FGetFileWriteOff() { return FileWriteOff; };


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� velikosti souboru

double FGetFileSize()
{
// lok�ln� prom�nn�
	int size = 0;

// vypr�zd�n� z�pisov�ho bufferu a otev�en� �tec�ho souboru
	if (FileWriteFlush() && FileReadOpen())
	{
// zji�t�n� velikosti souboru
		int res = ::GetFileSize(FileReadHandle, NULL);

// chyba
		if (res == -1)
		{
			FileError = true;
		}
		else
		{
			size = res;
		}
	}

// zji�t�n� velikost
	return size;
}


/////////////////////////////////////////////////////////////////////////////
// d�lka textu s pevnou d�lkou

double FGetFileTextNLen() { return FileTextNLen; }


/////////////////////////////////////////////////////////////////////////////
// na�ten� typu disku

double FFileDiskType() { return ::GetDriveType(NULL); }


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��sla ze souboru bajt se znam�nkem

double FGetFileByteS() { return (char)FileReadByte(); }


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��sla ze souboru bajt bez znam�nka

double FGetFileByteU() { return (BYTE)FileReadByte(); }


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��sla ze souboru slovo se znam�nkem

double FGetFileWordS()
{ 
	short cislo;
	((BYTE*)&cislo)[0] = FileReadByte();
	((BYTE*)&cislo)[1] = FileReadByte();
	return cislo;
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��sla ze souboru slovo bez znam�nka

double FGetFileWordU()
{ 
	long cislo = 0;
	((BYTE*)&cislo)[0] = FileReadByte();
	((BYTE*)&cislo)[1] = FileReadByte();
	return cislo;
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��sla ze souboru 3 bajty se znam�nkem

double FGetFile3ByteS()
{ 
	long cislo = 0;
	((BYTE*)&cislo)[0] = FileReadByte();
	((BYTE*)&cislo)[1] = FileReadByte();
	((BYTE*)&cislo)[2] = FileReadByte();

	if (((char*)&cislo)[2] < 0)
	{
		((BYTE*)&cislo)[3] = 0xff;
	}

	return cislo;
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��sla ze souboru 3 bajty bez znam�nka

double FGetFile3ByteU()
{ 
	long cislo = 0;
	((BYTE*)&cislo)[0] = FileReadByte();
	((BYTE*)&cislo)[1] = FileReadByte();
	((BYTE*)&cislo)[2] = FileReadByte();
	return cislo;
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��sla ze souboru dvojslovo se znam�nkem

double FGetFileDWordS()
{ 
	long cislo;
	((BYTE*)&cislo)[0] = FileReadByte();
	((BYTE*)&cislo)[1] = FileReadByte();
	((BYTE*)&cislo)[2] = FileReadByte();
	((BYTE*)&cislo)[3] = FileReadByte();
	return cislo;
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��sla ze souboru dvojslovo bez znam�nka

double FGetFileDWordU()
{ 
	__int64 cislo;
	((BYTE*)&cislo)[0] = FileReadByte();
	((BYTE*)&cislo)[1] = FileReadByte();
	((BYTE*)&cislo)[2] = FileReadByte();
	((BYTE*)&cislo)[3] = FileReadByte();
	((long*)&cislo)[1] = 0;
	return (double)cislo;
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��sla ze souboru FLOAT

double FGetFileFloat()
{
	float cislo;
	((BYTE*)&cislo)[0] = FileReadByte();
	((BYTE*)&cislo)[1] = FileReadByte();
	((BYTE*)&cislo)[2] = FileReadByte();
	((BYTE*)&cislo)[3] = FileReadByte();
	return cislo;
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��sla ze souboru DOUBLE

double FGetFileDouble()
{
	double cislo;
	((BYTE*)&cislo)[0] = FileReadByte();
	((BYTE*)&cislo)[1] = FileReadByte();
	((BYTE*)&cislo)[2] = FileReadByte();
	((BYTE*)&cislo)[3] = FileReadByte();
	((BYTE*)&cislo)[4] = FileReadByte();
	((BYTE*)&cislo)[5] = FileReadByte();
	((BYTE*)&cislo)[6] = FileReadByte();
	((BYTE*)&cislo)[7] = FileReadByte();
	return cislo;
}


/***************************************************************************\
*																			*
*							obsluha CD disku								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� d�lky CD

double FCDGetLength()
{
// aktualizace �daj� o CD
	CDAkt();

// d�lka CD
	return (double)CDLength / 1000;
}


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� po�tu stop CD

double FCDGetTracks()
{
// aktualizace �daj� o CD
	CDAkt();

// po�et stop CD
	return CDTracks;
}


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� pozice CD

double FCDGetPos()
{
// aktualizace �daj� o CD
	CDAkt();

// pozice CD
	return (double)CDPos / 1000;
}


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� aktivn� stopy CD

double FCDGetTrack()
{
// aktualizace �daj� o CD
	CDAkt();

// aktivn� stopa
	return CDTrack;
}


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� d�lky aktivn� stopy

double FCDGetTrackLength()
{
// aktualizace �daj� o CD
	CDAkt();

// d�lka stopy CD
	return (double)CDTrackLength / 1000;
}


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� pozice aktivn� stopy CD

double FCDGetTrackPos()
{
// aktualizace �daj� o CD
	CDAkt();

// pozice aktivn� stopy CD
	return (double)CDTrackPos / 1000;
}


/***************************************************************************\
*																			*
*								obsluha hudby								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// ��lka p�ehr�van� hudby

double FMusicLen() { return (double)MusicDelka / 1000; }


/////////////////////////////////////////////////////////////////////////////
// pozice hudby

double FGetMusicPos() { return (double)MusicAktPos / 1000; }


/***************************************************************************\
*																			*
*								obsluha zvuku								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// po�et bit� na vzorek

double FGetSoundBit()
{
	CSound snd;
	FSound(snd);
	return (double)snd.Bits();
}


/////////////////////////////////////////////////////////////////////////////
// vzorkovac� kmito�et

double FGetSoundFreq()
{
	CSound snd;
	FSound(snd);
	return (double)snd.Samples();
}

/////////////////////////////////////////////////////////////////////////////
// d�lka zvuku

double FGetSoundLength()
{
	CSound snd;
	FSound(snd);

	int bytespersec = snd.Samples() * snd.Channels() * (snd.Bits()/8);
	if (bytespersec == 0)
	{
		bytespersec = snd.Samples() * snd.Channels() / 2;
		if ((snd.Format() >= 0x50) && (snd.Format() < 0x60))
		{
			WAVEFORMATEX* wfx = (WAVEFORMATEX*)(snd.DataData());
			bytespersec = wfx->nAvgBytesPerSec;
		}
	}
	if (bytespersec <= 0) bytespersec = 1;

	return (double)(snd.Size() - snd.SizeHead()) / bytespersec;
}


/////////////////////////////////////////////////////////////////////////////
// pozice zvuku

double FGetSoundPos() { return GetSoundPos(); }


/////////////////////////////////////////////////////////////////////////////
// rozli�ovac� k�d

double FGetSoundDistinguish()
{
	return (double)(WORD)SoundID;
}


/***************************************************************************\
*																			*
*								obsluha text�								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// d�lka textu

double FTextLen()
{
// d�lka textu
	int len;

// n�sleduj�c� �sek ponechat v z�vork�ch, aby se vyvolal
// destruktor textu d��ve ne� se ��slo ulo�� do koprocesoru:
	{
// na�ten� textu
		CString text;
		FText(text);

// d�lka textu
		len = text.Length();
	}
	return len;
}


/////////////////////////////////////////////////////////////////////////////
// p�evod textu na ��slo

double FTextScan()
{
// adresa textu
	LPCTSTR txt;

// n�sleduj�c� �sek ponechat v z�vork�ch, aby se vyvolal
// destruktor textu d��ve ne� se ��slo ulo�� do koprocesoru:
	{
// na�ten� textu
		CString text;
		FText(text);
		txt = text;
	}

// p�evod textu na ��slo
	return Double(txt);
}


/////////////////////////////////////////////////////////////////////////////
// nalezen� textu

double FTextFind()
{
// hledan� text
	CString subtext;
	FText(subtext);

// prohled�van� text
	CString text;
	FText(text);

// nalezen� textu
	int pos = text.Find(subtext);
	return pos;
}


/////////////////////////////////////////////////////////////////////////////
// abecedn� po�ad� znaku

double FTextOrd()
{
// pomocn� registr
	int result;

// n�sleduj�c� �sek ponechat v z�vork�ch, aby se vyvolal
// destruktor textu d��ve ne� se ��slo ulo�� do koprocesoru:
	{

// na�ten� textu (znaku)
		CString text;
		FText(text);

// k�du znaku (nen�-li ��dn� znak, vr�t� se koncov� bajt 0)
#ifdef _UNICODE
		result = (WORD)text.DataData()[0];
#else
		result = (BYTE)text.DataData()[0];
#endif
	}

// n�vrat v�sledku
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� po�tu ��dk� textu

double FTextLineNum()
{
// na�ten� textu
	CString text;
	FText(text);
	return text.LineNum();
}


/***************************************************************************\
*																			*
*								obsluha kl�vesnice							*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// na�ten� kl�vesy bez �ek�n�

double FVKey()
{
// p��prava v�sledku operace
	int result = 0;

// test, zda byla p�ijata kl�vesa
	if (KeyBufRead != KeyBufWrite)
	{

// na�ten� kl�vesy z bufferu
		result = KeyBuf[KeyBufRead];

// zv��en� �tec� adresy
		KeyBufRead++;
		if (KeyBufRead >= KEYBUFSIZE) KeyBufRead = 0;
	}

	return result;
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� kl�vesy s �ek�n�m

double FVKeyWait()
{
// p��prava v�sledku operace
	int result = 0;

// �ek�n� na vstup znaku z kl�vesnice (nebo na p�eru�en�)
	while ((Break == 0) && 
			(KeyBufRead == KeyBufWrite) && 
			(!ConsoleOn))
	{
#ifndef _MINI
		D3Render(true);
#endif // _MINI
		TimeWait(1);
	}

// kontrola, zda byla p�ijata kl�vesa (zda to nebylo jen p�eru�en�)
	if (KeyBufRead != KeyBufWrite)
	{

// na�ten� kl�vesy z bufferu
		result = KeyBuf[KeyBufRead];

// zv��en� �tec� adresy
		KeyBufRead++;
		if (KeyBufRead >= KEYBUFSIZE) KeyBufRead = 0;
	}

	return result;
}


/***************************************************************************\
*																			*
*									plocha									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// ���ka plochy

double FGetMapWidth()
{
// na�ten� plochy
	CMap map;
	FMap(map);

// ���ka plochy
	return map.Width();
}

double FGetMapWidth0() { return Map[0].Width(); }


/////////////////////////////////////////////////////////////////////////////
// v��ka plochy

double FGetMapHeight()
{
// na�ten� plochy
	CMap map;
	FMap(map);

// v��ka plochy
	return map.Height();
}

double FGetMapHeight0() { return Map[0].Height(); }


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��sla z plochy

double _fastcall FGetMapNum(int mask, char rot)
{
// na�ten� plochy
	CMap map;
	FMap(map);

// na�ten� sou�adnic
	int x = FIntR();
	int y = FIntR();

// ��slo na pozici
	if (((DWORD)x < (DWORD)map.Width()) && ((DWORD)y < (DWORD)map.Height()))
	{
		return (((int)map.At(x, y).Param & mask) >> rot);
	}
	return 0;
}

double FGetMapNum1() { return FGetMapNum(MAP_VALUE1_MASK, MAP_VALUE1_ROT); };
double FGetMapNum2() { return FGetMapNum(MAP_VALUE2_MASK, MAP_VALUE2_ROT); };
double FGetMapNum3() { return FGetMapNum(MAP_VALUE3_MASK, MAP_VALUE3_ROT); };


/////////////////////////////////////////////////////////////////////////////
// na�ten� okna k zobrazen� sprajt�

double FGetSpriteWinX1() { return (double)SpriteWinX1 / ICONWIDTH; }
double FGetSpriteWinY1() { return (double)SpriteWinY1 / ICONHEIGHT; }
double FGetSpriteWinX2() { return (double)SpriteWinX2 / ICONWIDTH; }
double FGetSpriteWinY2() { return (double)SpriteWinY2 / ICONHEIGHT; }


/////////////////////////////////////////////////////////////////////////////
// n�vratov� k�d

double FGetReturnCode() { return ReturnCode; }


/////////////////////////////////////////////////////////////////////////////
// spu�t�n� programu

double FGetExecWait()
{
	CString text;
	FText(text);
	DWORD result = Exec(text, TRUE);
	SetConsoleText();
	return result;
}

/***************************************************************************\
*																			*
*									joystick								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// pozice X

double FJoystickX()
{
	int inx = FIntR();
	if ((DWORD)inx >= (DWORD)MAXJOYSTICK) return 0;
	AktJoystick(inx);
	return JoystickX[inx];
}


/////////////////////////////////////////////////////////////////////////////
// pozice Y

double FJoystickY()
{
	int inx = FIntR();
	if ((DWORD)inx >= (DWORD)MAXJOYSTICK) return 0;
	AktJoystick(inx);
	return JoystickY[inx];
}


/////////////////////////////////////////////////////////////////////////////
// pozice Z

double FJoystickZ()
{
	int inx = FIntR();
	if ((DWORD)inx >= (DWORD)MAXJOYSTICK) return 0;
	AktJoystick(inx);
	return JoystickZ[inx];
}


/***************************************************************************\
*																			*
*									dialogy									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// ID aktivn�ho prvku

double FGetWindowID() { return WindowID; }


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� okna

double FWindowNew()
{
	WindowNew(WINITEM_WINDOW, WS_SYSMENU); 
	return WindowID;
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� okna

double FWindowRONew()
{
	WindowNew(WINITEM_WINDOWRO, 0); 
	return WindowID;
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� tla��tka

double FButtonNew()
{
	WindowItemNew(WINITEM_BUTTON, ButtonClassName, 10*8, 24, BS_PUSHBUTTON | BS_NOTIFY | WS_GROUP | WS_TABSTOP, 0);
	return WindowID;
}

double FGetButtonEsc()
{
	return ButtonEsc;
}

double FDefButtonNew()
{
	WindowItemNew(WINITEM_DEFBUTTON, ButtonClassName, 10*8, 24, BS_DEFPUSHBUTTON | BS_NOTIFY | WS_GROUP | WS_TABSTOP, 0);
	return WindowID;
}

double FButtonIconNew()
{
	WindowItemNew(WINITEM_BUTTONICON, ButtonClassName, 40, 40, BS_3STATE | BS_NOTIFY | BS_PUSHLIKE | WS_GROUP | WS_TABSTOP | BS_ICON, 0);
	return WindowID;
}

double FButtonCheckNew()
{
	WindowItemNew(WINITEM_BUTTONCHECK, ButtonClassName, 16*8, 16, BS_AUTOCHECKBOX | BS_NOTIFY | WS_GROUP | WS_TABSTOP, 0);
	return WindowID;
}

double FButtonCheckNew2()
{
	WindowItemNew(WINITEM_BUTTONCHECK2, ButtonClassName, 10*8, 24, BS_3STATE | BS_NOTIFY | BS_PUSHLIKE | WS_GROUP | WS_TABSTOP, 0);
	return WindowID;
}

double FButton3New()
{
	WindowItemNew(WINITEM_BUTTON3, ButtonClassName, 16*8, 16, BS_AUTO3STATE | BS_NOTIFY | WS_GROUP | WS_TABSTOP, 0);
	return WindowID;
}

double FButtonRadioNew()
{
	WindowItemNew(WINITEM_BUTTONRADIO, ButtonClassName, 16*8, 16, BS_AUTORADIOBUTTON | BS_NOTIFY, 0);
	return WindowID;
}

double FButtonRadioNew2()
{
	WindowItemNew(WINITEM_BUTTONRADIO2, ButtonClassName, 16*8, 16, BS_AUTORADIOBUTTON | BS_NOTIFY | WS_GROUP | WS_TABSTOP, 0);
	return WindowID;
}

double FGroupBoxNew()
{
	WindowItemNew(WINITEM_GROUP, ButtonClassName, 24*8, 12*8, BS_GROUPBOX, WS_EX_TRANSPARENT);
	return WindowID;
}

/////////////////////////////////////////////////////////////////////////////
// indik�tor

CString	ProgressClass(PROGRESS_CLASS);

double FProgressNew()
{
	::InitCommonControls();

	WindowItemNew(WINITEM_PROGRESS, ProgressClass, 202, 16, 0, 0);
	return WindowID;
}

/////////////////////////////////////////////////////////////////////////////
// t�hlo

CString	TrackbarClass(TRACKBAR_CLASS);

double FHTrackbarNew()
{
	::InitCommonControls();

	WindowItemNew(WINITEM_HTRACKBAR, TrackbarClass, 16*8, 34, TBS_HORZ | TBS_TOP | TBS_AUTOTICKS | WS_GROUP | WS_TABSTOP, 0);
	return WindowID;
}

double FVTrackbarNew()
{
	::InitCommonControls();

	WindowItemNew(WINITEM_VTRACKBAR, TrackbarClass, 42, 16*8, TBS_VERT | TBS_BOTH | TBS_AUTOTICKS | WS_GROUP | WS_TABSTOP, 0);
	return WindowID;
}

/////////////////////////////////////////////////////////////////////////////
// posuvn�k

CString	ScrollbarClass("SCROLLBAR");

double FHScrollbarNew()
{
	WindowItemNew(WINITEM_HSCROLLBAR, ScrollbarClass, 32*8, 18, SBS_HORZ | WS_GROUP | WS_TABSTOP, 0);
	return WindowID;
}

double FVScrollbarNew()
{
	WindowItemNew(WINITEM_VSCROLLBAR, ScrollbarClass, 18, 32*8, SBS_VERT | WS_GROUP | WS_TABSTOP, 0);
	return WindowID;
}

/////////////////////////////////////////////////////////////////////////////
// pozice v prvku

double FGetWindowPos()
{
	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];

		switch (item->Typ)
		{
		case WINITEM_HTRACKBAR:
			return ((double)::SendMessage(item->Wnd, TBM_GETPOS, 0, 0) / 100);

		case WINITEM_VTRACKBAR:
			return ((double)(100 - ::SendMessage(item->Wnd, TBM_GETPOS, 0, 0)) / 100);

		case WINITEM_PROGRESS:
			return ((double)::SendMessage(item->Wnd, PBM_GETPOS, 0, 0) / 100);

		case WINITEM_LISTBOX:
			// nen�-li nic ozna�eno, vrac� LB_ERR = -1
			return (double)(int)::SendMessage(item->Wnd, LB_GETCURSEL, 0, 0);

		case WINITEM_TABS:
			// nen�-li nic ozna�eno, vrac� -1
			return (double)(int)::SendMessage(item->Wnd, TCM_GETCURSEL, 0, 0);

		case WINITEM_LISTEDIT:
		case WINITEM_COMBOBOX:
		case WINITEM_COMBOEDIT:
			// nen�-li nic ozna�eno, vrac� CB_ERR = -1
			return (double)(int)::SendMessage(item->Wnd, CB_GETCURSEL, 0, 0);

		case WINITEM_EDIT:
			{
				DWORD pos = 0;
				::SendMessage(item->Wnd, EM_GETSEL, (LPARAM)&pos, NULL);
				return (double)pos;
			}

		case WINITEM_MEMO:
			{
				int sel;
				::SendMessage(item->Wnd, EM_GETSEL, (WPARAM)&sel, 0);
				int radek = ::SendMessage(item->Wnd, EM_LINEFROMCHAR, sel, 0);
				return (double)radek;
			}

		case WINITEM_RICHEDIT:
		case WINITEM_RICHMEMO:
			{
				CHARRANGE sel;
				::SendMessage(item->Wnd, EM_GETSEL, 0, (LPARAM)&sel);
				int radek = ::SendMessage(item->Wnd, EM_LINEFROMCHAR, sel.cpMin, 0);
				return (double)radek;
			}

		case WINITEM_TABLE:
			return (double)item->TabRowF;

		case WINITEM_HSCROLLBAR:
		case WINITEM_VSCROLLBAR:
			return item->ScrollPos;
		}
		return 0;
	}
	else
	{
		return 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
// pozice 2 v prvku

double FGetWindowPos2()
{
	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];

		switch (item->Typ)
		{
		case WINITEM_MEMO:
			{
				int sel;
				::SendMessage(item->Wnd, EM_GETSEL, (WPARAM)&sel, 0);
				int radek = ::SendMessage(item->Wnd, EM_LINEFROMCHAR, sel, 0);
				int pozice = sel - ::SendMessage(item->Wnd, EM_LINEINDEX, radek, 0);
				return (double)pozice;
			}

		case WINITEM_RICHEDIT:
		case WINITEM_RICHMEMO:
			{
				CHARRANGE sel;
				::SendMessage(item->Wnd, EM_EXGETSEL, 0, (LPARAM)&sel);
				int radek = ::SendMessage(item->Wnd, EM_LINEFROMCHAR, sel.cpMin, 0);
				int pozice = sel.cpMin - ::SendMessage(item->Wnd, EM_LINEINDEX, radek, 0);
				return (double)pozice;
			}

		case WINITEM_TABLE:
			return (double)item->TabCol;

		case WINITEM_HSCROLLBAR:
		case WINITEM_VSCROLLBAR:
			return item->ScrollPage;
		}
		return 0;
	}
	else
	{
		return 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
// edita�n� pole

double FEditNew()
{
	WindowItemNew(WINITEM_EDIT, EditClassName, 16*8, 21, ES_AUTOHSCROLL | WS_GROUP | WS_TABSTOP, WS_EX_CLIENTEDGE);
	return WindowID;
}

double FMemoNew()
{
	WindowItemNew(WINITEM_MEMO, EditClassName, 20*8, 73, ES_AUTOVSCROLL | ES_MULTILINE | WS_GROUP | WS_TABSTOP
			| ES_WANTRETURN | WS_VSCROLL, WS_EX_CLIENTEDGE);
	return WindowID;
}


HINSTANCE RichEditLib = NULL;

double RichNew(int typ, int par)
{
// !!!!!!!!!!!!!!!!! Nov�j�� verze RichEdit p�i na�ten� textu vyma�e UNDO buffer
// !!!!!!!!!!!!!!!!! u nov�j�� verze RichEdit nefunguje hled�n� textu
	if (RichEditLib == NULL)
	{
//		RichEditLib = ::LoadLibrary(_T("RICHED20.DLL"));
//		if (RichEditLib != NULL)
//		{
//			RichEditClassName = RICHEDIT_CLASS;
//		}
//		else
		{
			RichEditLib = ::LoadLibrary(_T("RICHED32.DLL"));
		}
	}		
	WindowItemNew(typ, RichEditClassName, 20*8, 73, par, WS_EX_CLIENTEDGE);

	return WindowID;
}

double FRicheditNew()
{
	return RichNew(WINITEM_RICHEDIT, ES_AUTOVSCROLL | ES_MULTILINE | WS_GROUP | WS_TABSTOP
			| ES_WANTRETURN | WS_VSCROLL);
}


double FRichMemoNew()
{
	return RichNew(WINITEM_RICHMEMO, ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_MULTILINE | WS_GROUP | WS_TABSTOP
			| ES_WANTRETURN | WS_VSCROLL | WS_HSCROLL);
}

/////////////////////////////////////////////////////////////////////////////
// seznam

double FListBoxNew()
{
	WindowItemNew(WINITEM_LISTBOX, ListBoxClassName, 16*8, 24*8, LBS_NOINTEGRALHEIGHT | WS_VSCROLL | WS_GROUP | WS_TABSTOP
			| LBS_USETABSTOPS | LBS_NOTIFY, WS_EX_CLIENTEDGE);
	return WindowID;
}

double FListEditNew()
{
	WindowItemNew(WINITEM_LISTEDIT, ComboBoxClassName, 16*8, 24*8, CBS_AUTOHSCROLL | CBS_NOINTEGRALHEIGHT | WS_VSCROLL | WS_GROUP | WS_TABSTOP
			| CBS_SIMPLE, 0);
	return WindowID;
}

double FComboBoxNew()
{
	WindowItemNew(WINITEM_COMBOBOX, ComboBoxClassName, 16*8, 24*8, CBS_AUTOHSCROLL | CBS_NOINTEGRALHEIGHT | WS_VSCROLL | WS_GROUP | WS_TABSTOP
			| CBS_DROPDOWNLIST, 0);
	return WindowID;
}

double FComboEditNew()
{
	WindowItemNew(WINITEM_COMBOEDIT, ComboBoxClassName, 16*8, 24*8, CBS_AUTOHSCROLL | CBS_NOINTEGRALHEIGHT | WS_VSCROLL | WS_GROUP | WS_TABSTOP
			| CBS_DROPDOWN, 0);
	return WindowID;
}

double FTabsNew()
{
	::InitCommonControls();

	WindowItemNew(WINITEM_TABS, TabsClassName, 32*8, 24*8, WS_CLIPSIBLINGS | WS_GROUP | WS_TABSTOP, 0);
	return WindowID;
}

/////////////////////////////////////////////////////////////////////////////
// tabulka

CString	ListviewClass(WC_LISTVIEW);

double FTableNew()
{
// na�ten� seznamu titulk�
	CString text;
	FText(text);

	int cols = text.LineNum();
	if (cols <= 0) cols = 1;

// p��prava jednotliv�ch titulk�
	CBufText coltit;
	bool head = false;
	for (int i = 0; i < cols; i++)
	{
		coltit.New();
		coltit[i] = text.GetLine(i);
		if (coltit[i].IsNotEmpty()) head = true;
	}

// vytvo�en� tabulky
	::InitCommonControls();

	WindowItemNew(WINITEM_TABLE, ListviewClass, 32*8, 128, LVS_REPORT | LVS_SINGLESEL | LVS_OWNERDRAWFIXED | LVS_SHOWSELALWAYS
		| WS_GROUP | WS_TABSTOP | (head ? LVS_NOSORTHEADER : LVS_NOCOLUMNHEADER), WS_EX_STATICEDGE);

	WINITEM* item = &Win[WindowID];
	item->HasCaption = true;

// vytvo�en� sloupe�k�
	HWND wnd = item->Wnd;
	item->TabAlign = (BYTE*)MemGet(cols*sizeof(BYTE));

	BYTE fm;

	for (i = 0; i < cols; i++)
	{
		item->TabCols++;

		LVCOLUMN col;
		MemFill(&col, sizeof(col), 0);
		col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		fm = (BYTE)((i == 0) ? LVCFMT_LEFT : LVCFMT_CENTER);
		col.fmt = fm;
		item->TabAlign[i] = 2;
		col.cx = (16*8);
		col.pszText = (LPTSTR)(LPCTSTR)coltit[i];
		col.iSubItem = i;

		::SendMessage(wnd, LVM_INSERTCOLUMN, i, (LPARAM)&col);
	}

// vytvo�en� prvn�ho ��dku
	i = 32;
	while (i < cols) i *= 2;

	item->TabData = (CString*)MemGet(i*sizeof(CString));
	item->TabDataMax = i;

	for (i = 0; i < cols; i++)
	{
		item->TabData[i].Init();
	}

	item->TabRows = 1;

	LVITEM lvi;
	MemFill(&lvi, sizeof(lvi), 0);
	lvi.mask = LVIF_TEXT | LVIF_NORECOMPUTE;
	lvi.pszText = LPSTR_TEXTCALLBACK;

	::SendMessage(wnd, LVM_INSERTITEM, 0, (LPARAM)&lvi);

	return WindowID;
}

double FGetTableWidth()
{
	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];
		int col = item->TabCol;

		if ((DWORD)col < (DWORD)item->TabCols)
		{
			if (item->Typ == WINITEM_TABLE)
			{
				LVCOLUMN lvc;
				lvc.mask =  LVCF_WIDTH;
				lvc.cx = 0;
				lvc.iSubItem = col;
				::SendMessage(item->Wnd, LVM_GETCOLUMN, col, (LPARAM)&lvc);

				return ((double)lvc.cx / ICONWIDTH);
			}
		}
	}

	return 0;
}

double FGetTableAlign()
{
	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];
		int col = item->TabCol;

		if ((DWORD)col < (DWORD)item->TabCols)
		{
			if (item->Typ == WINITEM_TABLE)
			{
				return item->TabAlign[col];
			}
		}
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// text

double FWinTextNew()
{
	WindowItemNew(WINITEM_WINTEXT, StaticClassName, 16*8, 16, SS_LEFT | SS_NOTIFY | SS_NOPREFIX, 0);
	return WindowID;
}

double FWinTextNewCenter()
{
	WindowItemNew(WINITEM_WINTEXT, StaticClassName, 16*8, 16, SS_CENTER | SS_NOTIFY | SS_NOPREFIX, 0);
	return WindowID;
}

double FWinTextNewRight()
{
	WindowItemNew(WINITEM_WINTEXT, StaticClassName, 16*8, 16, SS_RIGHT | SS_NOTIFY | SS_NOPREFIX, 0);
	return WindowID;
}

double FWinIconNew()
{
	WindowItemNew(WINITEM_WINICON, StaticClassName, 32, 32, SS_CENTERIMAGE | SS_ICON | SS_NOTIFY | SS_REALSIZEIMAGE, 0);
	return WindowID;
}

double FWinPicNew()
{
	WindowItemNew(WINITEM_WINPIC, StaticClassName, 32, 32, SS_NOTIFY | SS_OWNERDRAW, 0);
	return WindowID;
}


/////////////////////////////////////////////////////////////////////////////
// rozm�ry okna

double FGetWindowX()
{
	if (Win.IsValid(WindowID))
	{
		return (double)Win[WindowID].Left/ICONWIDTH;
	}
	else
	{
		return 0;
	}
}

double FGetWindowY()
{
	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];
		int height;
		if ((item->Typ == WINITEM_WINDOW) || (item->Typ == WINITEM_WINDOWRO))
		{
			height = ScreenHeight0;
		}
		else
		{
			height = Win[item->Parent].ClientHeight;
		}
		return (double)(height - item->Top - item->Height)/ICONHEIGHT;
	}
	else
	{
		return 0;
	}
}


double FGetWindowW()
{
	if (Win.IsValid(WindowID))
	{
		return (double)Win[WindowID].Width/ICONWIDTH;
	}
	else
	{
		return (double)ScreenWidth0/ICONWIDTH;
	}
}

double FGetWindowWInt()
{
	if (Win.IsValid(WindowID))
	{
		return (double)Win[WindowID].ClientWidth/ICONWIDTH;
	}
	else
	{
		return (double)ScreenWidth/ICONWIDTH;
	}
}

double FGetWindowH()
{
	if (Win.IsValid(WindowID))
	{
		return (double)Win[WindowID].Height/ICONHEIGHT;
	}
	else
	{
		return (double)ScreenHeight0/ICONHEIGHT;
	}
}

double FGetWindowHInt()
{
	if (Win.IsValid(WindowID))
	{
		return (double)Win[WindowID].ClientHeight/ICONHEIGHT;
	}
	else
	{
		return (double)ScreenHeight/ICONHEIGHT;
	}
}


/////////////////////////////////////////////////////////////////////////////
// v��ka font�

double FGetFontHeight()
{
	if (Win.IsValid(WindowID))
	{
		return Win[WindowID].FontHeight;
	}
	else
	{
		return 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
// ���ka font�

double FGetFontWidth()
{
	if (Win.IsValid(WindowID))
	{
		return Win[WindowID].FontWidth;
	}
	else
	{
		return 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
// �hel textu

double FGetFontAngle()
{
	if (Win.IsValid(WindowID))
	{
		return Win[WindowID].Angle;
	}
	else
	{
		return 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
// barva textu

double FGetFontCol()
{
	if (Win.IsValid(WindowID))
	{
		return Win[WindowID].FontCol;
	}
	else
	{
		return StdColorBtnText;
	}
}

/////////////////////////////////////////////////////////////////////////////
// barva pozad�

double FGetFontBack()
{
	if (Win.IsValid(WindowID))
	{
		return Win[WindowID].FontBack;
	}
	else
	{
		return StdColorBtnFace;
	}
}

/////////////////////////////////////////////////////////////////////////////
// okno s fokusem

double FGetFocus() { return WindowFocus; };


/***************************************************************************\
*																			*
*									DirectPlay								*
*																			*
\***************************************************************************/

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� ��seln� hodnoty hry 1

double FGetGameN1()
{
	int n = (int)(((WORD*)DirectPlayGamesParam)[0] & 0x7fff) - 0x4000;
	return n;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� ��seln� hodnoty hry 2

double FGetGameN2()
{
	int n = (int)(((WORD*)DirectPlayGamesParam)[1] & 0x7fff) - 0x4000;
	return n;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� ��seln� hodnoty hry 3

double FGetGameN3()
{
	int n = (int)(((WORD*)DirectPlayGamesParam)[2] & 0x7fff) - 0x4000;
	return n;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� ��seln� hodnoty hry 4

double FGetGameN4()
{
	int n = (int)(((WORD*)DirectPlayGamesParam)[3] & 0x7fff) - 0x4000;
	return n;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� ��seln� hodnoty hry 5

double FGetGameN5()
{
	int n = (int)(((WORD*)DirectPlayGamesParam)[4] & 0x7fff) - 0x4000;
	return n;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� ��seln� hodnoty hry 6

double FGetGameN6()
{
	int n = (int)(((WORD*)DirectPlayGamesParam)[5] & 0x7fff) - 0x4000;
	return n;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� ��seln� hodnoty hry 7

double FGetGameN7()
{
	int n = (int)(((WORD*)DirectPlayGamesParam)[6] & 0x7fff) - 0x4000;
	return n;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� ��seln� hodnoty hry 8

double FGetGameN8()
{
	int n = (int)(((WORD*)DirectPlayGamesParam)[7] & 0x7fff) - 0x4000;
	return n;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� max. po�tu hr���

double FGetPlayerMax()
{
	return DirectPlayPlayersMax;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� indexu hr��e

double FPlayerInx()
{
	return DirectPlayPlayersAktN;
}

/////////////////////////////////////////////////////////////////////////////
// p��jem paketu

double FGetGamePacket()
{
	return DirectReceive();
}

/////////////////////////////////////////////////////////////////////////////
// na�ten� ��sla z paketu

double FGetGameDataN()
{
	double num;
	DirectReceiveData((BYTE*)&num, 8);
	return num;
}

/////////////////////////////////////////////////////////////////////////////
// na�ten� bajtu z paketu

double FGetGameDataB()
{
	BYTE num;
	DirectReceiveData((BYTE*)&num, 1);
	return (double)(int)(BYTE)num;
}

/***************************************************************************\
*																			*
*										UDP									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� vys�lac�ho portu UDP

double FGetUDPSendPort()
{
	int n = UDPSendAddr.sin_port;
	return ((n & 0xff) << 8) | (n >> 8);
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� velikosti p�ij�mac�ho bufferu UDP

double FGetUDPRecSize()
{
	return UDPRecBuf.len;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� p�ij�mac�ho portu UDP

double FGetUDPRecPort()
{
	int n = UDPRecAddr.sin_port;
	return ((n & 0xff) << 8) | (n >> 8);
}

/////////////////////////////////////////////////////////////////////////////
// p��jem datov�ho bloku UDP

double FGetUDPMem()
{
	int n = RecUDP();
	MemCopy(DLLMemoryWrite, UDPRecBuf.buf, n);
	DLLMemoryWrite = (BYTE*)DLLMemoryWrite + n;
#ifdef UDP_OVER
	StartRecUDP();
#endif
	return n;
}

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------

/***************************************************************************\
*																			*
*							Bitov� operace									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// oper�tor BIT-AND (Data = po�et prvk� - 1)

double FBitAnd()
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data;

// na�ten� prvn�ho ��sla
	DWORD num = FIntRN();

// cyklus p�es dal�� ��sla
	for (; i > 0; i--)
	{

// operace s dal��m ��slem
		num &= FIntRN();
	}
	return num;
}

/////////////////////////////////////////////////////////////////////////////
// oper�tor BIT-OR (Data = po�et prvk� - 1)

double FBitOr()
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data;

// na�ten� prvn�ho ��sla
	DWORD num = FIntRN();

// cyklus p�es dal�� ��sla
	for (; i > 0; i--)
	{

// operace s dal��m ��slem
		num |= FIntRN();
	}
	return num;
}

/////////////////////////////////////////////////////////////////////////////
// oper�tor BIT-XOR (Data = po�et prvk� - 1)

double FBitXor()
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data;

// na�ten� prvn�ho ��sla
	DWORD num = FIntRN();

// cyklus p�es dal�� ��sla
	for (; i > 0; i--)
	{

// operace s dal��m ��slem
		num ^= FIntRN();
	}
	return num;
}

/////////////////////////////////////////////////////////////////////////////
// bitov� dopln�k

double FBitNot()
{ 
// na�ten� ��sla
	DWORD num = FIntRN();
	num = ~num;
	return num;
};

/////////////////////////////////////////////////////////////////////////////
// rotace vlevo

double FBitLeft()
{ 
// na�ten� ��sla
	DWORD num = FIntRN();

	num = (num << 1) | (num >> 31);

	return num;
};


/////////////////////////////////////////////////////////////////////////////
// rotace vpravo

double FBitRight()
{ 
// na�ten� ��sla
	DWORD num = FIntRN();

	num = (num >> 1) | (num << 31);

	return num;
};



/////////////////////////////////////////////////////////////////////////////
// zji�t�n� znakov� sady

double FGetCodePage() { return CodePage; }


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� jazyku

double FGetLanguage() { return LangID; }


/***************************************************************************\
*																			*
*									Porty									*
*																			*
\***************************************************************************/

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

/////////////////////////////////////////////////////////////////////////////
// otev�en� portu

double FComOpen()
{
// na�ten� textu
	CString text;
	FText(text);

// korekce textu
	int id;
	text.UpperCase();
	text.TrimLeft();
	text.TrimRight();
	while ((id = text.Find(_T(' '))) >= 0)
	{
		text.Delete(id, 1);
	}

// nalezen� voln� polo�ky
	for (id = 0; id < COMSMAX; id++)
	{
		if (Coms[id].File == INVALID_HANDLE_VALUE) break;
	}
	if (id == COMSMAX) return -1;

	COMITEM* item = Coms + id;

// otev�en� za��zen�
	HANDLE file = ::CreateFile(
		text,									// jm�no za��zen�
		GENERIC_READ | GENERIC_WRITE,			// po�adovan� p��stup
		0,										// sd�len� nen� povoleno
		NULL,									// nelze sd�let s podprocesy
		OPEN_EXISTING,							// za��zen� mus� existovat
		FILE_ATTRIBUTE_NORMAL,					// atributy norm�ln�
		NULL);									// nen� �ablona
	if (file == INVALID_HANDLE_VALUE) return -1;
	item->File = file;

// inicializace buffer� (pro p�ij�mac� buffer > 1024 d�l� chyby v datech nad 1024 !!!!)
	::SetupComm(file, 1010, 64);

// hl�en� ud�lost� se nebude pou��vat
	::SetCommMask(file, 0);

// vypr�zdn�n� buffer�
	::PurgeComm(file, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

// nastaven� time-out
	COMMTIMEOUTS to;
	to.ReadIntervalTimeout = 100;
	to.ReadTotalTimeoutMultiplier = 100;
	to.ReadTotalTimeoutConstant = 0;
	to.WriteTotalTimeoutMultiplier = 100;
	to.WriteTotalTimeoutConstant = 0;
	::SetCommTimeouts(file, &to);

// nastaven� jako aktivn� port
	ComAkt = id;

// inicializace parametr� portu
	GetComDCB();

	DCB olddcb;
	DCB* dcb = &item->Dcb;

	MemCopy(&olddcb, dcb, sizeof(DCB));

	dcb->fBinary = TRUE;
	dcb->fOutxCtsFlow = FALSE;
	dcb->fOutxDsrFlow = FALSE;
	dcb->fDtrControl = DTR_CONTROL_DISABLE;
	dcb->fDsrSensitivity = FALSE;
	dcb->fOutX = FALSE;
	dcb->fInX = FALSE;
	dcb->fNull = FALSE;
	dcb->fRtsControl = RTS_CONTROL_DISABLE;
	dcb->fAbortOnError = FALSE;
	dcb->ErrorChar = (char)(BYTE)0xff;
	dcb->fErrorChar = TRUE;
	dcb->EofChar = 0;

// aktualizace parametr�, pokud byla zm�na
	if (!MemCompare(&olddcb, dcb, sizeof(DCB)))
	{
		SetComDCB();
	}

	return id;
}

/////////////////////////////////////////////////////////////////////////////
// aktivn� port

double FGetComID()
{
	return ComAkt;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� p�enosov� rychlosti portu

double FGetComBaud()
{
	COMITEM* item = Coms + ComAkt;
	if (item->File != INVALID_HANDLE_VALUE)
	{
		return item->Dcb.BaudRate;
	}
	return 1200;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� po�tu bit� portu

double FGetComBits()
{
	COMITEM* item = Coms + ComAkt;
	if (item->File != INVALID_HANDLE_VALUE)
	{
		return item->Dcb.ByteSize;
	}
	return 8;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� parity

double FGetComParit()
{
	COMITEM* item = Coms + ComAkt;
	if (item->File != INVALID_HANDLE_VALUE)
	{
		DCB* dcb = &item->Dcb;

		if (!dcb->fParity) return 0;
		return dcb->Parity;
	}
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� bajtu

double FGetComNum()
{
	return (double)ComReceive();
}

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------


/***************************************************************************\
*																			*
*								obsluha mixeru								*
*																			*
\***************************************************************************/

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� typu sign�lu

double FGetLineType()
{
// inicializace mixeru
	if (!MixDevInit() || ((DWORD)MixLineA >= (DWORD)MixLineN)) return -1;

// zji�t�n� typu sign�lu
	return MixLine[MixLineA].Type;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� typu ovl�dac�ho prvku

double FGetCtrlType()
{
// inicializace mixeru
	if (!MixDevInit() || ((DWORD)MixCtrlA >= (DWORD)MixCtrlN)) return -1;

// zji�t�n� typu ovl�dac�ho prvku
	return MixCtrl[MixCtrlA].Type;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� hodnoty ovl�dac�ho prvku

double FGetCtrlVal()
{
	return GetMixCtrlVal();
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� po�tu kan�l� prvku

double FGetMixChannel()
{
// inicializace mixeru
	if (!MixDevInit() || ((DWORD)MixCtrlA >= (DWORD)MixCtrlN)) return 0;

// zji�t�n� po�tu kan�l�
	return MixCtrl[MixCtrlA].Channels;
}

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------

/***************************************************************************\
*																			*
*								obsluha Direct3D							*
*																			*
\***************************************************************************/

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� m�du odstra�ov�ni ploch objektu Direct3D

double FGetD3DOCulling()
{
	if (IsValidID())
	{
		return GetID()->Culling;
	}
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� zdi (je vol�no i pro vytvo�en� 2D objektu)

// tabulka se pou��v� i p�i zobrazen� textury pozad� sc�ny v OpenGL
D3DVECTOR wall_vertices[4] = {
	-0.5,  0.5, 0,			// 0      0-----1
     0.5,  0.5, 0,			// 1	  |	  /	|
    -0.5, -0.5, 0,			// 2	  |	/	|
     0.5, -0.5, 0			// 3	  2-----3
};

int wall_faces[6] = {
	0, 1, 2,
	2, 1, 3
};

D3DVECTOR wall_normals[4] = {
	0, 0, -1,
	0, 0, -1,
	0, 0, -1,
	0, 0, -1
};

// tabulka se pou��v� i p�i zobrazen� textury pozad� sc�ny v OpenGL
float wall_text[2*4] = {
	0, 0,	// 0
	1, 0,	// 1
	0, 1,	// 2
	1, 1	// 3
};

double FD3DWall()
{
	int inx = D3DF_CreateMesh(4, wall_vertices, 2, wall_faces);
	D3DF_SetNormal(inx, wall_normals);
	D3DF_SetTextUV(inx, wall_text, 0);
	D3D_ID = inx;
	D3DF_SetAddress(inx, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, 0);
	D3DF_KorRightHand();

	return D3D_ID;
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� statick�ho 2D objektu

D3DVECTOR static2D_vertices[16] = {
// 1. st�na
	-0.5,  0.5,    0,		// 0
     0.5,  0.5,    0,		// 1
    -0.5, -0.5,    0,		// 2
     0.5, -0.5,    0,		// 3

// 2. st�na
	   0,  0.5, -0.5,		// 4
       0,  0.5,  0.5,		// 5
       0, -0.5, -0.5,		// 6
       0, -0.5,  0.5,		// 7

// 3. st�na
	 0.5,  0.5,    0,		// 8
    -0.5,  0.5,    0,		// 9
     0.5, -0.5,    0,		// 10
    -0.5, -0.5,    0,		// 11

// 4. st�na
	   0,  0.5,  0.5,		// 12
       0,  0.5, -0.5,		// 13
       0, -0.5,  0.5,		// 14
       0, -0.5, -0.5		// 15
};

int static2D_faces[24] = {
// 1. st�na
	0, 1, 2,
	2, 1, 3,

// 2. st�na
	4, 5, 6,
	6, 5, 7,

// 3. st�na
	8, 9, 10,
	10, 9, 11,

// 4. st�na
    12, 13, 14,
	14, 13, 15
};

D3DVECTOR static2D_normals[16] = {
// 1. st�na
	0, 0, -1,
	0, 0, -1,
	0, 0, -1,
	0, 0, -1,

// 2. st�na
	1, 0,  0,
	1, 0,  0,
	1, 0,  0,
	1, 0,  0,

// 3. st�na
	0, 0,  1,
	0, 0,  1,
	0, 0,  1,
	0, 0,  1,

// 4. st�na
	-1, 0, 0,
	-1, 0, 0,
	-1, 0, 0,
	-1, 0, 0
};

float static2D_text[2*16] = {
// 1. st�na
	0, 0,
	1, 0,
	0, 1,
	1, 1,

// 2. st�na
	0, 0,
	1, 0,
	0, 1,
	1, 1,

// 3. st�na
	0, 0,
	1, 0,
	0, 1,
	1, 1,

// 4. st�na
	0, 0,
	1, 0,
	0, 1,
	1, 1,
};

double FD3D2DStatic()
{
	CPicture pic;
	FPicture(pic);

	int inx = D3DF_CreateMesh(16, static2D_vertices, 8, static2D_faces);
	D3DF_SetNormal(inx, static2D_normals);
	D3DF_SetTextUV(inx, static2D_text, 0);
	D3D_ID = inx;
	D3DF_SetAddress(inx, D3DTADDRESS_CLAMP, D3DTADDRESS_CLAMP, 0);
	D3DF_KorRightHand();

	D3DFITEM* item = GetID();
	item->MatSource = MATSOURCE_FRAME;
//	item->Lighton = false;

	if (pic.IsNotEmpty())
	{
		item->Texture[0] = D3DT_Add(pic);
//		item->AktMat = true;
	}

	item->AlphaRef = 0.5;
	item->AlphaRef2 = 0x80;
	item->IsAlphaRef = true;

	return D3D_ID;
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� troj�heln�ku

D3DVECTOR triangle_vertices[3] = {
	-0.5, -0.5, 0,			// 0 (vlevo dole)
     0,    0.5, 0,			// 1 (uprost�ed naho�e)
     0.5, -0.5, 0			// 2 (vpravo dole)
};

int triangle_faces[3] = {
	0, 1, 2
};

D3DVECTOR triangle_normals[3] = {
	0, 0, -1,
	0, 0, -1,
	0, 0, -1
};

float triangle_text[2*3] = {
	0,   1,	// 0
	0.5, 0,	// 1
	1,   1 	// 2
};

double FD3DTriangle()
{
	int inx = D3DF_CreateMesh(3, triangle_vertices, 1, triangle_faces);
	D3DF_SetNormal(inx, triangle_normals);
	D3DF_SetTextUV(inx, triangle_text, 0);
	D3D_ID = inx;
	D3DF_SetAddress(inx, D3DTADDRESS_CLAMP, D3DTADDRESS_CLAMP, 0);
	D3DF_KorRightHand();
	return D3D_ID;
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� krychle

//    3----7
//    |\    \ 
//    | 2----6
//    1 |  5 |
//     \|    |
//      0----4

D3DVECTOR cube_vertices[24] = {

// horn� st�na (mus� b�t jako prvn� st�na!)
    -0.5,  0.5,  0.5,	// ( 0) 3 horn� st�ny
     0.5,  0.5,  0.5,	// ( 1) 7 horn� st�ny
    -0.5,  0.5, -0.5,	// ( 2) 2 horn� st�ny
     0.5,  0.5, -0.5,	// ( 3) 6 horn� st�ny

// p�edn� st�na
    -0.5,  0.5, -0.5,	// ( 4) 2 p�edn� st�ny
     0.5,  0.5, -0.5,	// ( 5) 6 p�edn� st�ny
	-0.5, -0.5, -0.5,	// ( 6) 0 p�edn� st�ny
     0.5, -0.5, -0.5,	// ( 7) 4 p�edn� st�ny

// zadn� st�na
     0.5,  0.5,  0.5,	// ( 8) 7 zadn� st�ny
    -0.5,  0.5,  0.5,	// ( 9) 3 zadn� st�ny
     0.5, -0.5,  0.5,	// (10) 5 zadn� st�ny
    -0.5, -0.5,  0.5,	// (11) 1 zadn� st�ny

// lev� st�na
    -0.5,  0.5,  0.5,	// (12) 3 lev� st�ny
    -0.5,  0.5, -0.5,	// (13) 2 lev� st�ny
    -0.5, -0.5,  0.5,	// (14) 1 lev� st�ny
	-0.5, -0.5, -0.5,	// (15) 0 lev� st�ny

// prav� st�na
     0.5,  0.5, -0.5,	// (16) 6 prav� st�ny
     0.5,  0.5,  0.5,	// (17) 7 prav� st�ny
     0.5, -0.5, -0.5,	// (18) 4 prav� st�ny
     0.5, -0.5,  0.5,	// (19) 5 prav� st�ny

// doln� st�na (mus� b�t jako posledn� st�na!)
	-0.5, -0.5, -0.5,	// (20) 0 doln� st�ny
     0.5, -0.5, -0.5,	// (21) 4 doln� st�ny
    -0.5, -0.5,  0.5,	// (22) 1 doln� st�ny
     0.5, -0.5,  0.5,	// (23) 5 doln� st�ny
};

//    3----7
//    |\    \ 
//    | 2----6
//    1 |  5 |
//     \|    |
//      0----4

int cube_faces[36] = {
	0,	1,	2,			// horn� st�na (3-7-2 a 2-7-6)
	2,	1,	3,

	4,	5,	6,			// p�edn� st�na (2-6-0 a 0-6-4)
	6,  5,	7,

	8,  9,  10,			// zadn� st�na (7-3-5 a 5-3-1)
	10, 9,  11,

	12, 13, 14,			// lev� st�na (3-2-1 a 1-2-0)
	14, 13, 15,

	16, 17, 18,			// prav� st�na (6-7-4 a 4-7-5)
	18, 17, 19,

	20, 21, 22,			// doln� st�na (0-4-1 a 1-4-5)
	22, 21, 23,
};

//    3----7
//    |\    \ 
//    | 2----6
//    1 |  5 |
//     \|    |
//      0----4

D3DVECTOR cube_normals[24] = {
	 0,  1,  0,		// 3 horn� st�ny
	 0,  1,  0,		// 7 horn� st�ny
	 0,  1,  0,		// 2 horn� st�ny
	 0,  1,  0,		// 6 horn� st�ny

	 0,  0, -1,		// 2 p�edn� st�ny
	 0,  0, -1,		// 6 p�edn� st�ny
	 0,  0, -1,		// 0 p�edn� st�ny
	 0,  0, -1,		// 4 p�edn� st�ny

	 0,  0,  1,		// 7 zadn� st�ny
	 0,  0,  1,		// 3 zadn� st�ny
	 0,  0,  1,		// 5 zadn� st�ny
	 0,  0,  1,		// 1 zadn� st�ny

	-1,  0,  0,		// 3 lev� st�ny
	-1,  0,  0,		// 2 lev� st�ny
	-1,  0,  0,		// 1 lev� st�ny
	-1,  0,  0,		// 0 lev� st�ny

	 1,  0,  0,		// 6 prav� st�ny
	 1,  0,  0,		// 7 prav� st�ny
	 1,  0,  0,		// 4 prav� st�ny
	 1,  0,  0,		// 5 prav� st�ny

	 0, -1,  0,		// 0 doln� st�ny
	 0, -1,  0,		// 4 doln� st�ny
	 0, -1,  0,		// 1 doln� st�ny
	 0, -1,  0,		// 5 doln� st�ny
};

// Mapov�n� ��st� textury:
//     zadn�     lev�      doln�
//     p�edn�    prav�     horn�

float cube_text[2*24] = {
	(float)2/3,		0.5,			// 3 horn� st�ny
	1,				0.5,			// 7 horn� st�ny
	(float)2/3,		1,				// 2 horn� st�ny
	1,				1,				// 6 horn� st�ny

	0,				0.5,			// 2 p�edn� st�ny
	(float)1/3,		0.5,			// 6 p�edn� st�ny
	0,				1,				// 0 p�edn� st�ny
	(float)1/3,		1,				// 4 p�edn� st�ny

	0,				0,				// 7 zadn� st�ny
	(float)1/3,		0,				// 3 zadn� st�ny
	0,				0.5,			// 5 zadn� st�ny
	(float)1/3,		0.5,			// 1 zadn� st�ny

	(float)1/3,		0,				// 3 lev� st�ny
	(float)2/3,		0,				// 2 lev� st�ny
	(float)1/3,		0.5,			// 1 lev� st�ny
	(float)2/3,		0.5,			// 0 lev� st�ny

	(float)1/3,		0.5,			// 6 prav� st�ny
	(float)2/3,		0.5,			// 7 prav� st�ny
	(float)1/3,		1,				// 4 prav� st�ny
	(float)2/3,		1,				// 5 prav� st�ny

	(float)2/3,		0,				// 0 doln� st�ny
	1,				0,				// 4 doln� st�ny
	(float)2/3,		0.5,			// 1 doln� st�ny
	1,				0.5,			// 5 doln� st�ny
};


double FD3DCube()
{
	D3DVECTOR* v = cube_vertices;
	D3DVECTOR* n = cube_normals;
	float* t = cube_text;

	int vn = 24;
	int fn = 12;

	if (!D3DCreateUpper)
	{
		v += 4;
		n += 4;
		t += 8;
		vn -= 4;
		fn -= 2;
	}

	if (!D3DCreateLower)
	{
		vn -= 4;
		fn -= 2;
	}

	int inx = D3DF_CreateMesh(vn, v, fn, cube_faces);
	D3DF_SetNormal(inx, n);
	D3DF_SetTextUV(inx, t, 0);
	D3D_ID = inx;
	D3DF_SetAddress(inx, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, 0);
	D3DF_KorRightHand();
	return D3D_ID;
}

/////////////////////////////////////////////////////////////////////////////
// gener�tor rota�n�ch objekt� - shora dol� (z�v�rem zru�� glob�ln� buffery)

// popisova� �ady
typedef struct GENROWITEM_
{
	double	dy;					// rozd�l sou�adnic Y k dal�� �ad�
	double	dr;					// rozd�l polom�r� R k dal�� �ad�
	double	length;				// d�lka spojnice k dal�� �ad�
	double	angle;				// �hel spojnice k dal�� �ad�
	double	norm1;				// �hel norm�ly - prvn� �ada
	double	norm2;				// �hel norm�ly - druh� �ada
	int		vn;					// po�et vrchol� v jedn� �ad� (bez zdvojn�soben�)
	bool	r0;					// p��znak nulov�ho polom�ru
	bool	smooth;				// p��znak zjemn�n� norm�ly - spole�n� norm�la pro ob� strany
	bool	close;				// objekt je uzav�en� (konec a po��tek jsou toto�n�)
	bool	prev;				// p��znak, �e jsou troj�heln�ky zp�t (nahoru)
	bool	next;				// p��znak, �e jsou troj�heln�ky vp�ed (dol�)
	bool	use2;				// p��znak pou�it� 2 �ad vrchol�
} GENROWITEM;

int			genRows;			// po�et �ad objektu
double*		genY = NULL;		// buffer sou�adnic Y (shora dol�)
double*		genR = NULL;		// buffer polom�r� �ad

// parametr sm = zjem�ovat v�dy (=koule, toroid)

void GenRot(bool sm)
{

// pro mal� po�et �ad se vytvo�� n�hradn� pr�zdn� skupina
	if (genRows <= 1)
	{
		MemFree(genY);
		MemFree(genR);

		D3D_ID = D3DF_New();
		return;
	}

// p��prava prom�nn�ch
	int level = D3DLevel;			// po�et d�len� (polovina)
	int level2 = level*2;			// po�et d�len� na cel� obvod
	double alfa = pi / level;		// �hel na jeden d�l
	double alfa2 = alfa/2;			// polovi�n� �hel na jeden d�l
	int i,j;

// p��prava bufferu popisova�e �ad
	GENROWITEM* gr = (GENROWITEM*)MemGet(genRows * sizeof(GENROWITEM));
	GENROWITEM* item = gr;									// ukazatel polo�ek popisova�e �ady

	double delka = 0;										// celkov� d�lka spojnic
	int vN = 0;												// po�et vrchol� celkem
	int fN = 0;												// po�et plo�ek celkem

// cyklus p�es v�echny �ady
	bool firstrow = true;									// p��znak, �e to je prvn� �ada

	for (i = 0; i < genRows; i++)
	{

// p��znak posledn� �ady
		bool lastrow = (i == (genRows - 1));

// p��znak nulov�ho polom�ru
		bool r00 = (genR[i] == 0);
		item->r0 = r00;

// rozd�l sou�adnic Y a polom�r� R k dal�� �ad�
		double dy0 = 0;
		double dr0 = 0;

		if (!lastrow)
		{
			dy0 = genY[i + 1] - genY[i];
			dr0 = genR[i + 1] - genR[i];
		}

		item->dy = dy0;
		item->dr = dr0;

// d�lka spojnice k dal�� �ad�
		double d0 = sqrt(dy0*dy0 + dr0*dr0);
		item->length = d0;
		delka += d0;
		
// �hel spojnice k dal�� �ad�
		double a0 = 0;
		if (d0 != 0)
		{
			a0 = atan2(dy0, dr0);
		}
		else
		{
			if (!firstrow)
			{
				a0 = item[-1].angle;
			}
		}
		item->angle = a0;
				 
// rozd�l �hlu s p�ede�lou �adou (pro prvn� a posledn� �adu se po��t� �hel s�m se sebou)
// (prvn� a posledn� �ada m��e spl�vat, je-li uzav�en� objekt - nap�. torus)
// (pro zjednodu�en� p�edpokl�d�me, �e uzav�en� objekt m� konce v�dy zjemn�n�,
//  proto�e na za��tku nezn�me sm�rn�k konce)
		bool close = false;					// p��znak uzav�en�ho objektu

		double da = uhel90;

		if (firstrow || lastrow)
		{
			if ((fabs(genR[0] - genR[genRows - 1]) +
				fabs(genY[0] - genY[genRows - 1])) < 0.0001)
			{
				close = true;
			}
			else
			{
				if (r00)
				{
					da = fabs(2*a0);
				}
			}
		}
		else
		{
			da = fabs(a0 - item[-1].angle);
		}

		if (da > pi) da = pi2 - da;

// porovn�n� �hlu, zda bude zjemn�n� (zvl�tn� korekce se prov�d� pro kouli se slo�itost� 2)
		bool smooth0;
		if (level == 2)
		{
			if ((genRows == 3) &&
				(genR[0] == 0) && 
				(genR[2] == 0))
			{
				smooth0 = true;				// zvl�tn� p��pad - koule se slo�itost� 2 (�hel 90 stup��)
			}
			else
			{
				smooth0 = (da < 1.4);		// tj. �hel asi 80 stup��
			}
		}
		else
		{
			if (level == 3)
			{
				smooth0 = (da < 1.1);		// tj. �hel asi 64 stup��, aby vyhov�la koule se slo�itost� 3
			}
			else
			{
				smooth0 = (da < 0.8);		// tj. �hel asi 45.8 stup��, aby vyhov�la koule se slo�itost� 4
			}
		}
		if (close || sm) smooth0 = true;		// pro uzav�en� objekt (nebo nucen� zjemn�n�) zjemn�me v�dy

		item->smooth = smooth0;
		item->close = close;

// p��znak, �e jsou troj�heln�ky zp�t (nahoru)
		bool prev0 = (!r00 && !firstrow);
		item->prev = prev0;
		if (prev0) fN += level2;

// p��znak, �e jsou troj�heln�ky vp�ed (dol�)
		bool next0 = (!r00 && !lastrow);
		item->next = next0;
		if (next0) fN += level2;

// p��znak pou�it� 2 �ad vrchol�
		bool use20 = (prev0 && next0 && !smooth0);
		item->use2 = use20;

// po�et vrchol� v jedn� �ad�
		int vn00 = level2;
		if (!r00) vn00++;
		item->vn = vn00;
		if (use20) vn00 = vn00 * 2;
		vN += vn00;

// v�po�et �hlu norm�l
		double n2 = a0 + uhel90;
		double n1 = n2;

		if (firstrow)
		{
			if (smooth0)
			{
				n1 = uhel90;

				if (close)
				{
					if ((n2 > -uhel45) && (n2 < uhel45))
					{
						n1 = 0;
					}
					else
					{
						if ((n2 >= uhel45) && (n2 < uhel135))
						{
							n1 = uhel90;
						}
						else
						{
							if ((n2 <= -uhel45) || (n2 >= uhel225))
							{
								n1 = uhel270;
							}
							else
							{
								n1 = uhel180;
							}
						}
					}

					if (sm) n1 = uhel180;	// torus
				}
				n2 = n1;
			}
		}
		else
		{
			if (lastrow)
			{
				if (smooth0)
				{
					n1 = -uhel90;

					if (close)
					{
						if ((n2 > -uhel45) && (n2 < uhel45))
						{
							n1 = 0;
						}
						else
						{
							if ((n2 >= uhel45) && (n2 < uhel135))
							{
								n1 = uhel90;
							}
							else
							{
								if ((n2 <= -uhel45) || (n2 >= uhel225))
								{
									n1 = uhel270;
								}
								else
								{
									n1 = uhel180;
								}
							}
						}

						if (sm) n1 = uhel180;	// torus
					}
					n2 = n1;
				}
			}
			else
			{
				n1 = item[-1].angle + uhel90;
			}
		}

		if (n1 > pi) n1 -= pi2;
		if (n1 < -pi) n1 += pi2;
		if (n2 > pi) n2 -= pi2;
		if (n2 < -pi) n2 += pi2;

		if (smooth0)
		{
			n1 = (n1 + n2)/2;
			n2 = n1;
		}

		item->norm1 = n1;
		item->norm2 = n2;

// p��prava pro dal�� �adu
		firstrow = false;
		item++;
	}

// kontrola platnosti bod� - vytvo�� se n�hradn� pr�zdn� skupina
	if (delka == 0)
	{
		MemFree(gr);
		MemFree(genY);
		MemFree(genR);

		D3D_ID = D3DF_New();
		return;
	}

// vytvo�en� v�stupn�ch buffer�
	D3DVECTOR* v = (D3DVECTOR*)MemGet(vN * sizeof(D3DVECTOR)); // buffer vektor� vrchol�
	D3DVECTOR* n = (D3DVECTOR*)MemGet(vN * sizeof(D3DVECTOR)); // buffer vektor� norm�l
	float* t = (float*)MemGet(vN * 2 * sizeof(float));		// buffer sou�adnic textur
	int* f = (int*)MemGet(fN * 3 * sizeof(int));			// buffer plo�ek

// prom�nn� ke generov�n�
	double even = false;									// p��znak sud� �ady

	D3DVECTOR* vp = v;										// ukl�dac� adresa vektor� vrchol�
	D3DVECTOR* np = n;										// ukl�dav� adresa vektor� norm�l
	float* tp = t;											// ukl�dac� adresa sou�adnic textur
	int* fp = f;											// ukl�dac� adresa plo�ek

	int inx = 0;											// ukazatel indexu bodu
	double del = 0;											// ukazatel sou�adnice textury
	delka = 1/delka;										// d�lka jako n�sobitel (pro zrychlen� v�po�tu)

// cyklus p�es v�echny �ady bod�
	item = gr;												// ukazatel polo�ek popisova�e �ady

	for (i = 0; i < genRows; i++)
	{

// p��prava prom�nn�ch
		double r = genR[i];									// polom�r �ady
		double y = genY[i];									// sou�adnice Y �ady

		int vn = item->vn;									// po�et vrchol� v �ad�

		bool use2 = item->use2;								// vytvo�� se 2 �ady vrchol�

		float nn1 = (float)sin(item->norm1);				// norm�la prvn� �ady vrchol� (slo�ka y)
		float nn2 = nn1;
		double rr1 = cos(item->norm1);
		double rr2 = rr1;

		if (use2)
		{
			nn2 = (float)sin(item->norm2);					// norm�la druh� �ady vrchol� (slo�ka y)
			rr2 = cos(item->norm2);
		}

		double gama = 0;									// ukazatel sm�ru oto�en� bodu
		if (even)
		{
			gama = alfa2;									// pro sudou �adu odsazen�
		}
		else
		{
			if (item->r0) gama = alfa;						// prvn� bod se nevytv���
		}

// vygenerov�n� vrchol�
		for (j = vn; j > 0; j--)
		{
			double cosgama = cos(gama);
			double singama = sin(gama);

// sou�adnice vrcholu
			vp->x = (float)(-r*singama);
			vp->y = (float)y;
			vp->z = (float)(r*cosgama);

// norm�la vrcholu
			np->x = (float)(-rr1*singama);
			np->y = nn1;
			np->z = (float)(rr1*cosgama);

// textura vrcholu
			*tp = (float)(gama/pi2);
			tp++;
			*tp = (float)del;

// vytvo�en� druh�ho vrcholu
			if (use2)
			{
				vp[vn] = vp[0];

				np[vn].x = (float)(-rr2*singama);
				np[vn].y = nn2;
				np[vn].z = (float)(rr2*cosgama);

				tp[2*vn-1] = tp[-1];
				tp[2*vn] = tp[0];
			}

// zv��en� ukazatel�
			vp++;
			np++;
			tp++;
			gama += alfa;
		}

// p�esko�en� druh� �ady vrchol�
		if (use2)
		{
			vp += vn;
			np += vn;
			tp += 2*vn;
		}

// vygenerov�n� horn� �ady troj�heln�k�
		if (item->prev)
		{
			int inx2 = inx;

			for (j = 0; j < level2; j++)
			{

			// lev� bod
				*fp = inx2;
				fp++;

			// horn� bod
				int k = inx2 - item[-1].vn;
				if (even && !item[-1].r0) k = k + 1;
				*fp = k;
				fp++;

			// prav� bod
				*fp = inx2 + 1;
				fp++;

				inx2++;
			}
		}


// vygenerov�n� doln� �ady troj�heln�k�
		if (item->next)
		{
			int inx2 = inx;
			if (use2) inx2 = inx + vn;

			for (j = 0; j < level2; j++)
			{
			// lev� bod
				*fp = inx2;
				fp++;

			// prav� bod
				*fp = inx2 + 1;
				fp++;

			// doln� bod
				int k = inx2 + vn;
				if (even && !item[1].r0) k = k + 1;
				*fp = k;
				fp++;

				inx2++;
			}
		}


// zv��en� ukazatele d�lky (pro textury)
		del += item->length*delka;

// zv��en� ukazatele indexu vrcholu
		inx = inx + vn;							// zv��en� ukazatele indexu polo�ek
		if (use2) inx = inx + vn;

// p��prava pro dal�� �adu
		even = !even;							// zm�na p��znaku sud� �ady
		item++;
	}

	ASSERT(inx == vN);
	ASSERT(fp == (f + 3*fN));

// vytvo�en� objektu
	int index = D3DF_CreateMesh(vN, v, fN, f);
	D3DF_SetNormal(index, n);
	D3DF_SetTextUV(index, t, 0);
	D3D_ID = index;
	D3DF_SetAddress(index, D3DTADDRESS_WRAP, D3DTADDRESS_CLAMP, 0);

// korekce pro pravoruk� sou�adn� syst�m
	D3DF_KorRightHand();

// zru�en� buffer�
	MemFree(v);
	MemFree(n);
	MemFree(f);
	MemFree(t);
	MemFree(gr);
	MemFree(genY);
	MemFree(genR);
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� kruhu

double FD3DCircle()
{
	int rows = (D3DLevel/16) + 2;		// po�et p�s�

	genRows = rows;

	genY = (double*)MemGet(rows * sizeof(double));
	genR = (double*)MemGet(rows * sizeof(double));

	double* geny = genY;
	double* genr = genR;

	double r = 0;
	double dr = 0.5/(rows - 1);

	for (rows--; rows > 0; rows--)
	{
		*geny = 0;
		*genr = r;
		r += dr;

		geny++;
		genr++;
	}

	*geny = 0;
	*genr = 0.5;

	GenRot(false);

// p�evr�cen� kruhu, aby sm��oval "-z"
// (zde se p�edpokl�d�, �e buffery je�t� nebyly vytvo�eny!!!)
	D3DFITEM* item = GetID();

	ASSERT(item->VertNum > 0);
	ASSERT(item->Vertex != NULL);
	ASSERT(item->Normal != NULL);
	ASSERT(item->TextUV != NULL);
	ASSERT(item->Data1 == NULL);
	ASSERT(item->Data2 == NULL);

	D3DF_SetAddress(D3D_ID, D3DTADDRESS_CLAMP, D3DTADDRESS_CLAMP, 0);

	D3DVECTOR* v = item->Vertex;
	D3DVECTOR* n = item->Normal;
	float* t = item->TextUV[0];

	for (int i = item->VertNum; i > 0; i--)
	{
		float z = v->z;
		v->y = v->z;
		v->z = 0;

		*t = (float)(v->x + 0.5);
		t++;
		*t = (float)(0.5 - z);
		t++;

		v++;

		n->x = 0;
		n->y = 0;
		n->z = -1;
		n++;
	}

// aktualizovat sektory
	item->AktSector = TRUE;

	return D3D_ID;
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� koule

double FD3DSphere()
{
	int rows = D3DLevel + 1;		// po�et �ad bod� vnit�n�ho p�su
	genRows = rows;

	genY = (double*)MemGet(genRows * sizeof(double));
	genR = (double*)MemGet(genRows * sizeof(double));

	double* geny = genY;
	double* genr = genR;

	*geny = 0.5;	geny++;
	*genr = 0;		genr++;

	double a = uhel90;
	double da = pi/D3DLevel;

	for (rows -= 2; rows > 0; rows--)
	{
		a -= da;
		*geny = 0.5*sin(a);		geny++;
		*genr = 0.5*cos(a);		genr++;
	}

	*geny = -0.5;
	*genr = 0;

	GenRot(true);

	return D3D_ID;
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� polokoule

double FD3DHemisphere()
{
	int rows = (D3DLevel+1)/2 + 1;		// po�et �ad bod� vnit�n�ho p�su
	if (D3DCreateLower)
	{
		genRows = rows + 1;
	}
	else
	{
		genRows = rows;
	}

	genY = (double*)MemGet((rows + 1) * sizeof(double));
	genR = (double*)MemGet((rows + 1) * sizeof(double));

	double* geny = genY;
	double* genr = genR;

	*geny = 0.5;	geny++;
	*genr = 0;		genr++;

	double a = uhel90;
	rows--;
	double da = a/rows;

	for (; rows > 0; rows--)
	{
		a -= da;
		*geny = 0.5*sin(a);		geny++;
		*genr = 0.5*cos(a);		genr++;
	}

	*geny = 0;
	*genr = 0;

	GenRot(false);

	return D3D_ID;
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� torusu

double FD3DTorus()
{
	double r = FNum()/2;			// men�� polom�r

	if (r < 0.0001)				// minim�ln� polom�r
	{
		return FD3DEmpty();			// vytvo�� se jen skupina
	}

	int rows = 2*D3DLevel + 1;			// po�et generovan�ch bod�
	genRows = rows;

	genY = (double*)MemGet(genRows * sizeof(double));
	genR = (double*)MemGet(genRows * sizeof(double));

	double* geny = genY;
	double* genr = genR;

	double a = uhel180;
	double da = pi/D3DLevel;

	*geny = 0;			geny++;
	*genr = 0.5 - r;	genr++;

	for (rows -= 2; rows > 0; rows--)
	{
		a -= da;

		*geny = r*sin(a);			geny++;
		*genr = 0.5 + r*cos(a);		genr++;
	}

	*geny = 0;
	*genr = 0.5 - r;

	int oldlevel = D3DLevel;

	int ii = Round(D3DLevel/r/10);
	if (ii > (2*D3DLevel)) ii = (2*D3DLevel);
	D3DLevel += ii;
	GenRot(true);
	D3DLevel = oldlevel;

	return D3D_ID;
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� v�lce

double FD3DCylinder()
{
	int rows = D3DLevel/8 + 2;			// po�et �ad bod� vnit�n�ho p�su
	int extra = (D3DLevel/20);			// p��davn� ��dky naho�e a dole

	genRows = rows;

	if (D3DCreateUpper)
	{
		genRows++;
		genRows += extra;
	}

	if (D3DCreateLower)
	{
		genRows++;
		genRows += extra;
	}

	genY = (double*)MemGet((rows + 2 + 2*extra) * sizeof(double));
	genR = (double*)MemGet((rows + 2 + 2*extra) * sizeof(double));

	double* geny = genY;
	double* genr = genR;

	double dd = (double)0.5/(extra+1);

	if (D3DCreateUpper)
	{
		double d = 0;

		for (int i = extra + 1; i > 0; i--)
		{
			*geny = 0.5;	geny++;
			*genr = d;		genr++;
			d += dd;
		}
	}

	double h = 0.5;
	double dh = (double)1/(rows-1);

	for (rows--; rows > 0; rows--)
	{
		*geny = h;		geny++;
		*genr = 0.5;	genr++;
		h -= dh;
	}

	*geny = -0.5;	geny++;
	*genr = 0.5;	genr++;

	if (D3DCreateLower)
	{
		double d = 0.5;

		for (int i = extra; i > 0; i--)
		{
			d -= dd;
			*geny = -0.5;	geny++;
			*genr = d;		genr++;
		}

		*geny = -0.5;
		*genr = 0;
	}

	GenRot(false);

	return D3D_ID;
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� ku�ele

double FD3DCone()
{
	int rows = D3DLevel/8 + 2;			// po�et �ad bod� vnit�n�ho p�su
	int extra = (D3DLevel/20);			// p��davn� ��dky dole

	genRows = rows;

	if (D3DCreateLower)
	{
		genRows++;
		genRows += extra;
	}

	genY = (double*)MemGet((rows + 1 + extra) * sizeof(double));
	genR = (double*)MemGet((rows + 1 + extra) * sizeof(double));

	double* geny = genY;
	double* genr = genR;

	double r = 0;
	double y = 0.5;
	double dr = 0.5/(rows-1);
	double dy = (double)1/(rows-1);

	for (rows--; rows > 0; rows--)
	{
		*geny = y;		geny++;
		*genr = r;		genr++;
		r += dr;
		y -= dy;
	}

	*geny = -0.5;	geny++;
	*genr = 0.5;	genr++;

	if (D3DCreateLower)
	{
		r = 0.5;
		dr = 0.5/(extra+1); 

		for (int i = extra; i > 0; i--)
		{
			r -= dr;
			*geny = -0.5;	geny++;
			*genr = r;		genr++;
		}

		*geny = -0.5;
		*genr = 0;
	}

	GenRot(false);

 	return D3D_ID;
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� komol�ho ku�ele

double FD3DCone2()
{
	double r = FNum()/2;			// men�� polom�r

	if (r < 0.001)					// minim�ln� polom�r
	{
		return FD3DCone();
	}

	int rows = D3DLevel/8 + 2;			// po�et �ad bod� vnit�n�ho p�su
	int extra1 = (Round(2*r*D3DLevel)/20);	// p��davn� ��dky naho�e
	int extra2 = (D3DLevel/20);			// p��davn� ��dky dole

	genRows = rows;

	if (D3DCreateUpper)
	{
		genRows++;
		genRows += extra1;
	}

	if (D3DCreateLower)
	{
		genRows++;
		genRows += extra2;
	}

	genY = (double*)MemGet((rows + 2 + extra1 + extra2) * sizeof(double));
	genR = (double*)MemGet((rows + 2 + extra1 + extra2) * sizeof(double));

	double* geny = genY;
	double* genr = genR;

	double dd = r/(extra1+1);

	if (D3DCreateUpper)
	{
		double d = 0;

		for (int i = extra1 + 1; i > 0; i--)
		{
			*geny = 0.5;	geny++;
			*genr = d;		genr++;
			d += dd;
		}
	}

	double y = 0.5;
	double dr = (0.5-r)/(rows-1);
	double dy = (double)1/(rows-1);

	for (rows--; rows > 0; rows--)
	{
		*geny = y;		geny++;
		*genr = r;		genr++;
		r += dr;
		y -= dy;
	}

	*geny = -0.5;	geny++;
	*genr = 0.5;	genr++;

	if (D3DCreateLower)
	{
		r = 0.5;
		dr = 0.5/(extra2+1); 

		for (int i = extra2; i > 0; i--)
		{
			r -= dr;
			*geny = -0.5;	geny++;
			*genr = r;		genr++;
		}

		*geny = -0.5;
		*genr = 0;
	}

	GenRot(false);

 	return D3D_ID;
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� sv�tla

double FD3DLight()
{		
	D3D_ID = D3DF_New();
	D3DFITEM* item = GetID();
	item->Type = D3DFTYPE_LIGHT;
	item->Light = D3DL_New();
	D3DL_Get(item->Light)->Frame = D3D_ID;

	return D3D_ID;
}

/////////////////////////////////////////////////////////////////////////////
// klonov�n�

double FD3DClone()
{
	int inx = FIntR();

	D3D_ID = D3DF_Copy(inx);

	if (GetID()->IsTerrain)
	{
		TerenID = D3D_ID;
	}

	return D3D_ID;
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� objektu ze souboru

double FD3DFile()
{
// na�ten� jm�na souboru
	CString text;
	FText(text);

// �prava jm�na souboru
	text.TrimLeft();
	text.TrimRight();
	text.PathName(0);

	if (text.IsEmpty())
	{
READERROR:
		FileError = true;
		return FD3DEmpty();
	}

// otev�en� souboru
	HANDLE file = ::CreateFile(text, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (FileReadHandle == INVALID_HANDLE_VALUE) goto READERROR;

// zji�t�n� velikosti souboru
	int size = (int)::GetFileSize(file, NULL);

	if (size <= 10)
	{	
READERROR2:
		::CloseHandle(file);
		goto READERROR;
	}

// vytvo�en� datov�ho bufferu
	BYTE* data = (BYTE*)MemGet(size);

// na�ten� souboru
	DWORD read = 0;

	if ((::ReadFile(file, (void*)data, size, &read, NULL) == FALSE) ||
		((int)read != size))
	{
		MemFree(data);
		goto READERROR2;
	}
	::CloseHandle(file);

// p��prava cesty do adres��e souboru
	text.Delete(text.RevFind('\\'));

// vytvo�en� objektu
	ReadMesh(data, size, text);

// zru�en� datov�ho bufferu
	MemFree(data);

	return D3D_ID;
}

/////////////////////////////////////////////////////////////////////////////
// na�ten� objektu z textu

double FD3DText()
{
	CString text;
	FText(text);

	CString path;
	ReadMesh((BYTE*)(LPCSTR)text, text.Length(), path);

	return D3D_ID;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� ID aktivn�ho objektu

double FGetD3DObjectID()
{
	return D3D_ID;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� po�ad� rotac� objekt�

double FGetD3DOrder()
{
	if (IsValidID()) return GetID()->Order;
	return D3DFORDER_XYZ;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� m���tka ve sm�ru X

double FGetD3DScaleX()
{
	if (IsValidID()) return GetID()->ScaleX;
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� m���tka ve sm�ru Y

double FGetD3DScaleY()
{
	if (IsValidID()) return GetID()->ScaleY;
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� m���tka ve sm�ru Z

double FGetD3DScaleZ()
{
	if (IsValidID()) return GetID()->ScaleZ;
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� rotace podle osy X

double FGetD3DRotateX()
{
	if (IsValidID()) return GetID()->RotateX;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� rotace podle osy Y

double FGetD3DRotateY()
{
	if (IsValidID()) return GetID()->RotateY;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� rotace podle osy Z

double FGetD3DRotateZ()
{
	if (IsValidID()) return GetID()->RotateZ;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� posunu ve sm�ru X

double FGetD3DTransX()
{
	if (IsValidID()) return GetID()->TransX;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� posunu ve sm�ru Y

double FGetD3DTransY()
{
	if (IsValidID()) return GetID()->TransY;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� posunu ve sm�ru Z

double FGetD3DTransZ()
{
	if (IsValidID()) return GetID()->TransZ;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� barvy objektu (-1=vnit�n� objektu, -2=z rodi�e, jinak ur�en�)

double FGetD3DColor()
{
	if (IsValidID())
	{
		D3DFITEM* item = GetID();
		if (item->MatSource == MATSOURCE_PARENT) return -2;
		if (item->MatSource == MATSOURCE_OBJECT) return -1;
		return item->Diffuse;
	}

	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� barvy sv�tivosti

double FGetD3DEmissive()
{
	if (IsValidID())
	{
		return GetID()->Emissive;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� odrazu rozpt�len�ho sv�tla

double FGetD3DAmbiRef()
{
	if (IsValidID())
	{
		D3DFITEM* item = GetID();
		if (item->Ambient == MAXDWORD) return -1;
		return item->Ambient;
	}

	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� matnosti

double FGetD3DPower()
{
	if (IsValidID())
	{
		return GetID()->Power;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� barvy odlesku

double FGetD3DSpecular()
{
	if (IsValidID())
	{
		return GetID()->Specular;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� barvy sv�tla

double FGetD3DLightColor()
{
	if (IsValidID())
	{
		D3DFITEM* item = GetID();

		if (item->Type == D3DFTYPE_LIGHT)
		{
			return D3DL_Get(item->Light)->Diffuse;
		}
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� typu sv�tla

double FGetD3DLightType()
{
	if (IsValidID())
	{
		D3DFITEM* item = GetID();

		if (item->Type == D3DFTYPE_LIGHT)
		{
			return (double)(int)(D3DL_Get(item->Light)->Light.Type);
		}
	}

	return (double)(int)D3DLIGHT_DIRECTIONAL;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� �hlu sv�teln�ho ku�ele

double FGetD3DLightUmbra()
{
	if (IsValidID())
	{
		D3DFITEM* item = GetID();

		if (item->Type == D3DFTYPE_LIGHT)
		{
			return D3DL_Get(item->Light)->Theta;
		}
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� �hlu polost�nu sv�tla

double FGetD3DLightPenumbra()
{
	if (IsValidID())
	{
		D3DFITEM* item = GetID();

		if (item->Type == D3DFTYPE_LIGHT)
		{
			return D3DL_Get(item->Light)->Phi;
		}
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� dosahu sv�tla

double FGetD3DLightRange()
{
	if (IsValidID())
	{
		D3DFITEM* item = GetID();

		if (item->Type == D3DFTYPE_LIGHT)
		{
			return D3DL_Get(item->Light)->Range;
		}
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� intenzity sv�tla

double FGetD3DLightCatten()
{
	if (IsValidID())
	{
		D3DFITEM* item = GetID();

		if (item->Type == D3DFTYPE_LIGHT)
		{
			return D3DL_Get(item->Light)->Intens;
		}
	}

	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� line�rn�ho �tlumu sv�tla

double FGetD3DLightLatten()
{
	if (IsValidID())
	{
		D3DFITEM* item = GetID();

		if (item->Type == D3DFTYPE_LIGHT)
		{
			return D3DL_Get(item->Light)->Attenuation1;
		}
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� kvadratick�ho �tlumu sv�tla

double FGetD3DLightQatten()
{
	if (IsValidID())
	{
		D3DFITEM* item = GetID();

		if (item->Type == D3DFTYPE_LIGHT)
		{
			return D3DL_Get(item->Light)->Attenuation2;
		}
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� projekce

double FGetD3DProjection()
{
	return D3DProjection;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� barvy pozad� sc�ny

double FGetD3DSceneColor()
{
	if (D3DBackCol == MAXDWORD) return -1;
	if (D3DBackCol == (MAXDWORD-1)) return -2;
	return D3DBackCol;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� ambient sv�tla

double FGetD3DAmbient()
{	
	return D3DAmbientCol;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� barvy mlhy

double FGetD3DFogColor()
{
	if (FogColor == MAXDWORD) return -1;
	return FogColor;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� m�dy mlhy

double FGetD3DFogMode()
{
	int n = FogType;
	if (FogKorig) n += 4;
	return n;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� za��tku mlhy

double FGetD3DFogStart()
{
	return FogStart;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� konce mlhy

double FGetD3DFogEnd()
{
	return FogEnd;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� hustotu mlhy

double FGetD3DFogDensity()
{
	return FogDens;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� projek�n� roviny

double FGetD3DViewFront()
{
	return D3DFrontClip;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� vzd�lenosti dohledu

double FGetD3DViewBack()
{
	return D3DBackClip;
}

/////////////////////////////////////////////////////////////////////////////
// vyhled�n� objektu

//double FD3DPick()
//{		
	/*
	int x = FIntX();
	if (x == (0x1000000*ICONWIDTH))
	{
		x = D3DW/2;
	}
	else
	{
		x -= D3DX;
	}

	int y = FIntY();
	if (y == (0x1000000*ICONHEIGHT))
	{
		y = D3DH/2;
	}
	else
	{
		y = Height - y - D3DY;
	}

	if (D3View != NULL)
	{
		LPDIRECT3DRMPICKEDARRAY list = NULL;

		D3View->Pick(x, y, &list);

		if (list != NULL)
		{
			int size = list->GetSize();
			int inx = 0;

			for (; size > 0; size--)
			{
				LPDIRECT3DRMVISUAL visual = NULL;
				LPDIRECT3DRMFRAMEARRAY frames = NULL;
				D3DRMPICKDESC desc;

				list->GetPick(inx, &visual, &frames, &desc);

				if (frames != NULL)
				{
					int size2 = frames->GetSize();
					int inx2 = size2 - 1;

					for (; size2 > 0; size2--)
					{
						LPDIRECT3DRMFRAME frame = NULL;
  
						frames->GetElement(inx2, &frame);

						if (frame != NULL)
						{
							int i = frame->GetAppData();
							if ((i != 1) && D3DF.IsValid(i) && (D3DF[i].Light == NULL))
							{
								frame->Release();
								frames->Release();
								list->Release();
								return i;
							}

							frame->Release();
						}

						inx2--;
					}

					frames->Release();
				}

				inx++;
			}

			list->Release();
		}
	}
	*/

//	return -1;
//}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� 2D objektu

double FD3D2DObject()
{
	CPicture pic;
	FPicture(pic);

	int inx = D3DF_CreateMesh(4, wall_vertices, 2, wall_faces);
	D3DF_SetNormal(inx, wall_normals);
	D3DF_SetTextUV(inx, wall_text, 0);
	D3D_ID = inx;

	D3DFITEM* item = GetID();
	item->Type = D3DFTYPE_DECAL;
	item->MatSource = MATSOURCE_FRAME;
	D3DF_SetAddress(inx, D3DTADDRESS_CLAMP, D3DTADDRESS_CLAMP, 0);
	D3DF_KorRightHand();

	if (pic.IsNotEmpty())
	{
		item->Texture[0] = D3DT_Add(pic);
//		item->AktMat = true;
	}

	item->AlphaRef = 0.5;
	item->AlphaRef2 = 0x80;
	item->IsAlphaRef = true;

	return D3D_ID;
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� pr�zdn�ho objektu

double FD3DEmpty()
{
	D3D_ID = D3DF_New();
	return D3D_ID;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� slo�itosti objekt�

double FGetD3DSplits()
{
	return D3DLevel;
}


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� ovlada�e

double FGetD3DDriver()
{
	return D3DDevAkt;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� rozhran�

double FGetD3DInterface()
{
	return D3DIntAkt;
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� rota�n�ho objektu

double FD3DRotary()
{
// na�ten� textu definice
	CString text;
	FText(text);

#define MAX_ROWS 10000

// na�ten� definice bod�
	int lin = 0;					// ukazatel ��dku textu
	CString line;					// jeden ��dek textu
	genRows = 0;					// ��ta� ��dk� s body
	genY = (double*)MemGet(MAX_ROWS * sizeof(double));	// buffer sou�adnice Y
	genR = (double*)MemGet(MAX_ROWS * sizeof(double));	// buffer polom�ru bodu

	for (int i = text.LineNum(); i > 0; i--)
	{

// na�ten� jednoho ��dku textu
		line = text.GetLine(lin);		// jeden ��dek z textu
		lin++;

		line.TrimLeft();
		line.TrimRight();
		if (line.IsNotEmpty())
		{

// na�ten� sou�adnice Y
			genY[genRows] = Double(line);

// na�ten� polom�ru bodu
			int pos = line.Find(' ');
			if (pos < 0) pos = line.Find(';');
			if (pos < 0) pos = line.Find(9);

			genR[genRows] = 0;

			if (pos >= 0)
			{
				line.Delete(0, pos + 1);
				line.TrimLeft();
				line.TrimRight();

				if (line.IsNotEmpty())
				{
					genR[genRows] = Double(line);
				}
			}

// zv��en� ��ta�e ��dk� bod�
			genRows++;
			if (genRows == MAX_ROWS) break;
		}
	}

// vytvo�en� objektu
	GenRot(false);

	return D3D_ID;
}

/////////////////////////////////////////////////////////////////////////////
// gener�tor ter�nu (zru�� glob�ln� buffery)

//#define MAXVERT 0xf000 // MaxVertexNum

int		terWidth;				// ���ka ter�nu (pol��ek)
int		terHeight;				// v��ka ter�nu (pol��ek)
float*	terMap;					// mapa ter�nu (width*height hodnot 0 a� 1),
								// po�ad� zleva doprava a zdola nahoru,
								// (lev� sloupec a posledn� �ada se zduplikuj�)

void GenTerrain()
{
	int i,j;

// omezen� velikosti ter�nu
	i = terWidth * terHeight * 4;
	int korwidth = terWidth;
	int korheight = terHeight;

//	if (i > MAXVERT)
//	{
//		korwidth = RoundM(sqrt(MAXVERT/4));
//		if (korwidth > terWidth) korwidth = terWidth;
//		korheight = MAXVERT/4/korwidth;
//		ASSERT((korwidth * korheight * 4) <= MAXVERT);
//	}

// vytvo�en� bufferu v��kov� mapy (s duplikac� okraj�)
	float* termap = (float*)MemGet((korwidth+1)*(korheight+1)*sizeof(float));
	float* ter = termap + korwidth + 1;
	float* tersrc = terMap;

	for (i = korheight; i > 0; i--)
	{
		MemCopy(ter, tersrc, korwidth*sizeof(float));
		ter += (korwidth+1);
		ter[-1] = tersrc[0];
		tersrc += terWidth;
	}
	MemCopy(termap, ter - korwidth - 1, (korwidth+1)*sizeof(float));

	terWidth = korwidth;
	terHeight = korheight;
	MemFree(terMap);

// vytvo�en� bufferu vektor� vrchol� (od doln� �ady nahoru, zleva doprava, ka�d� pol��ko m� 4 vrcholy)
	int vertN = terWidth*terHeight*4;
	D3DVECTOR* vert = (D3DVECTOR*)MemGet(vertN*sizeof(D3DVECTOR));
	D3DVECTOR* v = vert;
	ter = termap;

	double z = -0.5;
	double dz = 1/(double)terHeight;
	double dx = 1/(double)terWidth;

	for (i = terHeight; i > 0; i--)
	{
		double x = -0.5;
		double z2 = z + dz;

		for (j = terWidth; j > 0; j--)
		{

		// lev� doln� roh
			v->x = (float)x;
			v->y = ter[0];
			v->z = (float)z;
			v++;

		// lev� horn� roh
			v->x = (float)x;
			v->y = ter[terWidth+1];
			v->z = (float)z2;
			v++;

			x += dx;

		// prav� horn� roh
			v->x = (float)x;
			v->y = ter[terWidth+2];
			v->z = (float)z2;
			v++;

		// prav� doln� roh
			v->x = (float)x;
			v->y = ter[1];
			v->z = (float)z;
			v++;

			ter++;
		}

		ter++;
		z = z2;
	}

// vytvo�en� bufferu norm�l (stejn� organizace jako vektor vrchol�)
	D3DVECTOR* norm = (D3DVECTOR*)MemGet(vertN * sizeof(D3DVECTOR));
	D3DVECTOR* n = norm;
	v = vert;

// Norm�la je n�sobek vektor�: A x B = {Ay*Bz-Az*By,Az*Bx-Ax*Bz,Ax*By-Ay*Bx)
// pro A={dx,dxn,0} a B={0,dzn,dz} je N={dz*dxn,-dz*dx,dzn*dx}
// Vektory nejsou normalizovan� - provede se p�i jejich ulo�en�.

	float dy = (float)(dz * dx);

	for (i = terHeight; i > 0; i--)
	{
		for (j = terWidth; j > 0; j--)
		{
			double dx1 = (v[0].y - v[3].y);		// rozd�l dole
			double dx2 = (v[1].y - v[2].y);		// rozd�l naho�e

			double dz1 = (v[0].y - v[1].y);		// rozd�l vlevo
			double dz2 = (v[3].y - v[2].y);		// rozd�l vpravo

		// lev� doln� roh
			n->x = (float)(dz*dx1);
			n->y = dy;
			n->z = (float)(dz1*dx);
			n++;

		// lev� horn� roh
			n->x = (float)(dz*dx2);
			n->y = dy;
			n->z = (float)(dz1*dx);
			n++;

		// prav� horn� roh
			n->x = (float)(dz*dx2);
			n->y = dy;
			n->z = (float)(dz2*dx);
			n++;

		// prav� doln� roh
			n->x = (float)(dz*dx1);
			n->y = dy;
			n->z = (float)(dz2*dx);
			n++;

			v += 4;
		}
	}

// vyhlazen� norm�l
	n = norm;

	int down = (terHeight-1)*terWidth*4;

	for (i = terHeight; i > 0; i--)
	{
		int left = 4*(terWidth-1);

		for (j = terWidth; j > 0; j--)
		{
			float x = (float)((n[0].x + n[left+3].x)/2);
			n[0].x = x;
			n[left+3].x = x;
			n[down+1].x = x;
			n[left+down+2].x = x;

			float z = (float)((n[0].z + n[down+1].z)/2);
			n[0].z = z;
			n[left+3].z = z;
			n[down+1].z = z;
			n[left+down+2].z = z;

			n += 4;

			left = -4;
		}

		down = -4*terWidth;
	}	 

// vytvo�en� bufferu implicitn�ho mapov�n� textury
	float* text = (float*)MemGet(vertN * 2 * sizeof(float));
	float* t = text;

	float v1 = 1;
	int vi = 4;

	for (i = terHeight; i > 0; i--)
	{
		float v2 = (float)(v1 - 0.25);
		if (vi == 1) v2 = 0;

		float u = 0;
		int ui = 4;

		for (j = terWidth; j > 0; j--)
		{
		// lev� doln� roh
			*t = u;
			t++;
			*t = v1;
			t++;

		// lev� horn� roh
			*t = u;
			t++;
			*t = v2;
			t++;

			u += 0.25;

		// prav� horn� roh
			*t = u;
			t++;
			*t = v2;
			t++;

		// prav� doln� roh
			*t = u;
			t++;
			*t = v1;
			t++;

			ui--;
			if (ui == 0)
			{
				u = 0;
				ui = 4;
			}
		}

		v1 = v2;
		vi--;
		if (vi == 0)
		{
			v1 = 1;
			vi = 4;
		}
	}

// vytvo�en� plo�ek (zleva doprava, zdola nahoru)
// orientace roh� pol��ka:
//		1    2
//      0    3

	int faceN = terHeight*terWidth*2;
	int* face = (int*)MemGet(faceN * 3 *sizeof(int));

	int* f = face;
	int inx = 0;

	for (i = terHeight*terWidth; i > 0; i--)
	{
	// 1. troj�hlen�k (vrcholy 0-1-2)
		*f = inx;
		f++;
		*f = inx + 1;
		f++;
		*f = inx + 2;
		f++;

	// 2. troj�heln�k (vrcholy 0-2-3)
		*f = inx;
		f++;
		*f = inx + 2;
		f++;
		*f = inx + 3;
		f++;

		inx += 4;
	}

// vytvo�en� objektu
	int index = D3DF_CreateMesh(vertN, vert, faceN, face);
	D3DF_SetNormal(index, norm);
	D3DF_SetTextUV(index, text, 0);
	D3D_ID = index;
	TerenID = index;
	D3DFITEM* item = GetID();
	item->IsTerrain = true;
	item->TerWidth = terWidth;
	item->TerHeight = terHeight;
	item->TerMap = termap;
	D3DF_SetAddress(D3D_ID, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, 0);
	D3DF_KorRightHand();

// zru�en� buffer�
	MemFree(vert);
	MemFree(face);
	MemFree(norm);
	MemFree(text);
}

/////////////////////////////////////////////////////////////////////////////
// popisova� souseda ikony

typedef struct _ICONNEIGH
{
	int		icon;			// index ikony
	int		smer;			// offset souseda (0 a� 7)
	int		neigh;			// index souseda
	int		counter;		// ��ta� v�skytu
} ICONNEIGH;

// pomocn� tabulka sm�r� X
const int smerX[8] =
{
	1,						// 0: doprava
	1,						// 1: doprava o ��dek zp�t (dol�)
	0,						// 2: o ��dek zp�t
	-1,						// 3: doleva o ��dek zp�t (dol�)
	-1,						// 4: doleva
	-1,						// 5: doleva o ��dek vp�ed (nahoru)
	0,						// 6: o ��dek vp�ed
	1						// 7: doprava o ��dek vp�ed (nahoru)
};

// pomocn� tabulka sm�r� Y
const int smerY[8] =
{
	0,						// 0: doprava
	-1,						// 1: doprava o ��dek zp�t (dol�)
	-1,						// 2: o ��dek zp�t
	-1,						// 3: doleva o ��dek zp�t (dol�)
	0,						// 4: doleva
	1,						// 5: doleva o ��dek vp�ed (nahoru)
	1,						// 6: o ��dek vp�ed
	1						// 7: doprava o ��dek vp�ed (nahoru)
};

#define ICONBORD	16							// ���ka p��davn�ho p�su kolem ikon (v bodech)
#define ICONSIZEB	(ICONWIDTH + 2*ICONBORD)	// ���ka a v��ka ikony s p��davn�m p�sem

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� ter�nu z plochy

double FD3DTerrainMap()
{

// na�ten� plochy
	CMap map;
	FMap(map);

// p��prava parametr� plochy
	terWidth = map.Width();
	terHeight = map.Height();
	int size = terWidth * terHeight;
	int i,j;

// p��prava bufferu pro zmapov�n� ikon a v��ek
	CIcon* ico = (CIcon*)MemGet(size * sizeof(CIcon));
	int* icomap = (int*)MemGet(size * sizeof(int));
	int icons = 0;

// zmapov�n� ikon
	CIcon ico0;

	for (i = 0; i < size; i++)
	{
		ico0 = map[i].Icon;

		for (j = 0; j < icons; j++)
		{
			if (ico0 == ico[j])
			{
				break;
			}
		}

		if (j == icons)
		{
			ico[j].Init(ico0.Data());
			ico[j].DeComp();
			icons++;
		}
		icomap[i] = j;
	}

// vyhled�n� �etnost� soused� ikon
	ICONNEIGH* iconneigh = (ICONNEIGH*)MemGet(8 * size * sizeof(ICONNEIGH));
	int neighN = 0;

	int x = 0;
	int y = 0;

	for (i = 0; i < size; i++)
	{
		int icoinx = icomap[i];			// st�edov� ikona
		
		for (j = 0; j < 8; j++)			// cyklus p�es v�echny sousedy
		{
			int x2 = x + smerX[j];		// sou�adnice X souseda
			if (x2 < 0) x2 = terWidth - 1;
			if (x2 >= terWidth) x2 = 0;

			int y2 = y + smerY[j];		// sou�adnice Y souseda
			if (y2 < 0) y2 = terHeight - 1;
			if (y2 >= terHeight) y2 = 0;

			int neighinx = icomap[x2 + y2*terWidth]; // soused

			ICONNEIGH* item = iconneigh;

			for (int k = 0; k < neighN; k++)
			{
				if ((item->icon == icoinx) &&
					(item->smer == j) &&
					(item->neigh == neighinx))
				{
					item->counter++;
					break;
				}		  
				item++;
			}

			if (k == neighN)
			{
				item->icon = icoinx;
				item->smer = j;
				item->neigh = neighinx;
				item->counter = 1;
				neighN++;
			}
		}

		x++;
		if (x == terWidth)
		{
			x = 0;
			y++;
		}
	}

// vyhled�n� nej�ast�j��ch soused� (tabulka je po 8 polo�k�ch index� soused�)
	int* neighTab = (int*)MemGet(icons * 8 * sizeof(int));
	int* neighP = neighTab;

	for (i = 0; i < icons; i++)
	{
		for (j = 0; j < 8; j++)
		{
			int count = 0;

			ICONNEIGH* item = iconneigh;

			for (int k = 0; k < neighN; k++)
			{
				if ((item->icon == i) &&
					(item->smer == j) &&
					(item->counter > count))
				{
					count = item->counter;
					*neighP = item->neigh;
				}	
				item++;
			}

			ASSERT(count > 0);		// (to by byla chyba v t�to procedu�e)
			neighP++;
		}
	}

// p��prava textury
	int icopicWH = 32;
	int icoWH;

	do
	{
		icopicWH = (icopicWH << 1);
		icoWH = icopicWH/ICONSIZEB;

	} while (icoWH*icoWH < icons);

	CPicture pic(icopicWH, icopicWH);
	BYTE* picdata = pic.DataData();
	MemFill(picdata, icopicWH * icopicWH, BlackCol);

// dek�dov�n� ikon textury (od horn� �ady dol�)
	int radek = 1;
	int pozice = 0;

	for (i = 0; i < icons; i++)
	{
		BYTE* dst0 = picdata + (icopicWH - radek*ICONSIZEB + ICONBORD)*icopicWH + pozice*ICONSIZEB + ICONBORD;

		ico[i].DeComp();
		BYTE* src = ico[i].DataData();

	// p�enesen� ikony
		BYTE* dst = dst0; 
		for (j = ICONHEIGHT; j > 0; j--)
		{
			MemCopy(dst, src, ICONWIDTH);
			src += ICONWIDTH;
			dst += icopicWH;
		}

	// p�enesen� soused�
		for (j = 0; j < 8; j++)
		{
			src = ico[neighTab[8*i + j]].DataData();
			int width = ICONWIDTH;
			int height = ICONHEIGHT;
			dst = dst0;

		// vlevo
			if (smerX[j] < 0)
			{
				src += (ICONWIDTH-ICONBORD);
				dst -= ICONBORD;
				width = ICONBORD;
			}

		// vpravo
			if (smerX[j] > 0)
			{
				dst += ICONWIDTH;
				width = ICONBORD;
			}

		// zp�t (dol�)
			if (smerY[j] < 0)
			{
				src += ((ICONHEIGHT-ICONBORD)*ICONWIDTH);
				dst -= (ICONBORD*icopicWH);
				height = ICONBORD;
			}

		// vp�ed (nahoru)
			if (smerY[j] > 0)
			{
				height = ICONBORD;
				dst += (ICONHEIGHT*icopicWH);
			}

			for (int k = height; k > 0; k--)
			{
				MemCopy(dst, src, width);
				src += ICONWIDTH;
				dst += icopicWH;
			}
		}

	// zv��en� ukazatele pozice
		pozice++;
		if (pozice == icoWH)
		{
			pozice = 0;
			radek++;
		}
	}

// vytvo�en� bufferu v��kov� mapy (od doln� �ady nahoru)
	float* ter = (float*)MemGet(size*sizeof(float));
	terMap = ter;

	for (i = 0; i < size; i++)
	{
		*ter = (float)((double)((map[i].Param & MAP_VALUE1_MASK) >> MAP_VALUE1_ROT)/1000);
		ter++;
	}

// zru�en� buffer�
	for (i = 0; i < icons; i++)
	{
		ico[i].Term();
	}

	MemFree(ico);
	MemFree(neighTab);
	MemFree(iconneigh);

// vytvo�en� objektu
	int oldwidth = terWidth;

	GenTerrain();

	int newwidth = terWidth;
	int newheight = terHeight;
	int newsize = newwidth * newheight;
	size = oldwidth * newheight;
	oldwidth -= newwidth;

// vytvo�en� bufferu mapov�n� textur
	float* t = (float*)MemGet(newsize*2*4*sizeof(float));
	float* tp = t;

	int xx = newwidth;

	for (i = 0; i < size; i++)
	{
		j = icomap[i];

		float u = (float)((double)((j % icoWH)*ICONSIZEB + ICONBORD)/icopicWH);
		float u2 = (float)((double)((j % icoWH + 1)*ICONSIZEB - ICONBORD)/icopicWH);

		float v = (float)((double)((j / icoWH)*ICONSIZEB + ICONBORD)/icopicWH);
		float v2 = (float)((double)((j / icoWH + 1)*ICONSIZEB - ICONBORD)/icopicWH);

		// vlevo dole
		*tp = u;
		tp++;
		*tp = v2;
		tp++;

		// vlevo naho�e
		*tp = u;
		tp++;
		*tp = v;
		tp++;

		// vpravo naho�e
		*tp = u2;
		tp++;
		*tp = v;
		tp++;

		// vpravo dole
		*tp = u2;
		tp++;
		*tp = v2;
		tp++;

		xx--;
		if (xx == 0)
		{
			xx = newwidth;
			i += oldwidth;
		}
	}

// nastaven� mapov�n� textur
	D3DF_SetTextUV(D3D_ID, t, 0);

// zru�en� buffer�
	MemFree(t);
	MemFree(icomap);

// nastaven� opakov�n� textury
	D3DF_SetAddress(D3D_ID, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, 0);

// nastaven� textury
	D3DFITEM* item = GetID();
	item->MatSource = MATSOURCE_FRAME;

	if (pic.IsNotEmpty())
	{
		item->Texture[0] = D3DT_Add(pic);
		D3DT_Get(item->Texture[0])->MipMaps = 4;
//		item->AktMat = true;
	}

	return D3D_ID;
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� ter�nu z obr�zku

double FD3D2DTerrain()
{

// na�ten� obr�zku s ter�nem
	CPicture pic;
	FPicture(pic);

// p��prava parametr� obr�zku
	pic.DeComp();
	int width = pic.Width();
	int height = pic.Height();

#define OKRAJ	4			// p��davn� okraj

// pracovn� obr�zek s v�t��mi rozm�ry
	int korig = OKRAJ; //Round(OKRAJ*D3DSmooth);		 	// p�idan� body
	int widthnew = width + 2*korig;
	int heightnew = height + 2*korig;
	CPicture pic2(widthnew, heightnew);

// kopie p�vodn�ho obr�zku (s duplikac� okraj�)
	BYTE* dst = pic2.DataData() + korig*widthnew;
	BYTE* src = pic.DataData();
	int i;

	for (i = height; i > 0; i--)
	{
		MemCopy(dst, src + width - korig, korig);
		dst += korig;
		MemCopy(dst, src, width);
		dst += width;
		MemCopy(dst, src, korig);
		dst += korig;
		src += width;
	}

	dst = pic2.DataData();
	MemCopy(dst, dst + height*widthnew, korig*widthnew);
	MemCopy(dst + (korig+height)*widthnew, dst + korig*widthnew, korig*widthnew);

// konverze dat na TrueColor
	BYTE* buf = pic2.ExportTrueAlphaBeta();

// konverze obr�zku na zmen�enou velikost
	int widthnew2 = Round(widthnew / D3DSmooth); // / D3DSmooth);
	if (widthnew2 < 2) widthnew = 2;
//	if (widthnew2 < ((2*OKRAJ+2)/2)) widthnew2 = ((2*OKRAJ+2)/2);
	int heightnew2 = Round(heightnew / D3DSmooth); // / D3DSmooth);
	if (heightnew2 < 2) heightnew2 = 2;
//	if (heightnew2 < ((2*OKRAJ+2)/2)) heightnew2 = ((2*OKRAJ+2)/2);

	buf = ZoomTrueColor(buf, widthnew, heightnew, widthnew2, heightnew2);

// konverze obr�zku na spr�vnou velikost
//	widthnew = Round(widthnew / D3DSmooth);
//	if (widthnew < (2*OKRAJ+2)) widthnew = (2*OKRAJ+2);
//	heightnew = Round(heightnew / D3DSmooth);
//	if (heightnew < (2*OKRAJ+2)) heightnew = (2*OKRAJ+2);

	buf = ZoomTrueColor(buf, widthnew2, heightnew2, widthnew, heightnew);

//	width = widthnew - 2*OKRAJ;
//	height = heightnew - 2*OKRAJ;

// konverze obr�zku na v��kov� body
	float* ter = (float*)MemGet(width*height*sizeof(float));
	terMap = ter;

	src = buf + 5*OKRAJ*(widthnew+1);

	for (i = height; i > 0; i--)
	{
		for (int j = width; j > 0; j--)
		{
			*ter = (float)((double)(src[0] + src[1] + src[2])/(3*255));
			ter++;
			src += 5;
		}

		src += 5*2*OKRAJ;
	}

	terWidth = width;
	terHeight = height;

// zru�en� bufferu
	MemFree(buf);

// vytvo�en� objektu
	GenTerrain();

// nastaven� opakov�n� textury
	D3DF_SetAddress(D3D_ID, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, 0);

	return D3D_ID;
}


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� �rovn� ter�nu

double FD3DLevel()
{

// na�ten� horizont�ln� sou�adnice
	double x = FNum();
	if (x == 0x1000000)
	{
		x = D3DF_Get(1)->TransX;
	}

// na�ten� vertik�ln� sou�adnice
	double y = FNum();
	if (y == 0x1000000)
	{
		y = D3DF_Get(1)->TransZ;
	}

// aktu�ln� ter�n
	double h = 0;

	if (D3DF_IsValid(TerenID))
	{
		D3DFITEM* frame = D3DF_Get(TerenID);
		
// test, zda to je ter�n
		if (frame->IsTerrain &&
			(frame->TerMap != NULL))
		{

// ode�ten� sou�adnice X a Z
			x -= frame->TransX;
			y -= frame->TransZ;

// korekce m���tka X a Z
			x /= frame->ScaleX;
			y /= frame->ScaleZ;

// modulov�n� sou�adnic
			x = x + 0.5;
			y = y + 0.5;

			if (x < 0)
			{
				x = 1 + (x + Round0(-x));
			}
			else
			{
				if (x > 1) x -= RoundM(x);
			}
			ASSERT(x >= 0);
			ASSERT(x <= 1);
			if (x < 0) x = 0;

			if (y < 0)
			{
				y = 1 + (y + Round0(-y));
			}
			else
			{
				if (y > 1) y -= RoundM(y);
			}
			ASSERT(y >= 0);
			ASSERT(y <= 1);
			if (y < 0) y = 0;

// p�epo�et na bodov� sou�adnice
			x = x * frame->TerWidth;
			y = y * frame->TerHeight;

// sou�adnice po��te�n�ho bodu
			int x0 = Round0(x);
			double dx = x - x0;
			if (x0 >= frame->TerWidth)
			{
				x0 = frame->TerWidth - 1;
				dx = 1;
			}

			int y0 = Round0(y);
			double dy = y - y0;
			if (y0 >= frame->TerHeight)
			{
				y0 = frame->TerHeight - 1;
				dy = 1;
			}

// zji�t�n� 4 bod�
			int ww = frame->TerWidth + 1;

			float* t = frame->TerMap + y0*ww + x0;
			float n1 = t[0];	// 1: vlevo dole
			float n2 = t[1];	// 2: vpravo dole
			float n3 = t[ww];	// 3: vlevo naho�e
			float n4 = t[ww+1];	// 4: vpravo naho�e

// v�po�et v��ky (troj�heln�k 1-4-2 a 1-3-4)
			if (dx > dy)
			{
				h = n1 + (n2 - n1)*dx + (n4 - n2)*dy;
			}
			else
			{
				h = n1 + (n3 - n1)*dy + (n4 - n3)*dx;
			}

// vyn�soben� m���tkem Y
			h *= frame->ScaleY;

// p�i�ten� sou�adnice Y
			h += frame->TransY;
		}
	}
	return h;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� vyhlazen� textur

double FGetD3DSmooth()
{
	return D3DSmooth;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� pr�hlednosti objektu Direct3D

double FGetD3DTransparent()
{
	if (IsValidID())
	{
		return (double)(GetID()->SrcDstBlend);
	}
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� alfa �rovn� Direct3D

double FGetD3DAlphaRef()
{
	if (IsValidID())
	{
		return (GetID()->AlphaRef);
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// uplynul� p�esn� �as 

double FD3DElapsed0()
{
	return D3DElapsedTime;
}

/////////////////////////////////////////////////////////////////////////////
// uplynul� p�esn� �as 

double FD3DElapsed()
{	  
	double n = FNum();
	return (D3DElapsedTime*n);
}

/////////////////////////////////////////////////////////////////////////////
// rychlost renderov�n�

double FD3DSpeed()
{
	return D3DAverageFreqI;
}

/////////////////////////////////////////////////////////////////////////////
// voln� videopam�

double FD3DFree()
{
	FPUFloat();
	double n = pD3Free();
	FPUDouble();

	return n;
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� 2D obr�zku

double FD3DPicture()
{
	CPicture pic;
	FPicture(pic);

	int inx = D3DF_CreateMesh(4, wall_vertices, 2, wall_faces);
	D3DF_SetNormal(inx, wall_normals);
	D3DF_SetTextUV(inx, wall_text, 0);
	D3D_ID = inx;

	D3DFITEM* item = GetID();
	item->Type = D3DFTYPE_PICTURE;
	item->MatSource = MATSOURCE_FRAME;
	D3DF_SetAddress(inx, D3DTADDRESS_CLAMP, D3DTADDRESS_CLAMP, 0);
	D3DF_KorRightHand();

	if (pic.IsNotEmpty())
	{
		item->Texture[0] = D3DT_Add(pic);
	}

	if ((D3DWidth != 0) && (D3DHeight != 0))
	{
		item->ScaleX = (double)pic.Width()/D3DWidth/D3DSmooth;
		item->ScaleY = (double)pic.Height()/D3DHeight/D3DSmooth;
	}

	item->Lighton = false;
	item->Shades = false;
	item->MagFilter[0] = false;
	item->MinFilter[0] = false;
	item->MipFilter[0] = false;

	item->AlphaRef = 0.5;
	item->AlphaRef2 = 0x80;
	item->IsAlphaRef = true;

	item->RendGroup = RENDPIC;
	item->ZTest = D3DCMP_ALWAYS;
	item->ZWrite = FALSE;

// p�ipojen� ke kame�e
	D3DF_Link(1, D3D_ID);

	return D3D_ID;
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� �o�kov�ho odlesku

double FD3DLensFlare()
{
	CPicture pic;
	FPicture(pic);

	int inx = D3DF_CreateMesh(4, wall_vertices, 2, wall_faces);
	D3DF_SetNormal(inx, wall_normals);
	D3DF_SetTextUV(inx, wall_text, 0);
	D3D_ID = inx;

	D3DFITEM* item = GetID();
	item->Type = D3DFTYPE_LENSFLARE;
	item->MatSource = MATSOURCE_FRAME;
	D3DF_SetAddress(inx, D3DTADDRESS_CLAMP, D3DTADDRESS_CLAMP, 0);
	D3DF_KorRightHand();

	if (pic.IsNotEmpty())
	{
		item->Texture[0] = D3DT_Add(pic);
	}

	if ((D3DWidth != 0) && (D3DHeight != 0))
	{
		item->ScaleX = (double)pic.Width()/D3DWidth/D3DSmooth;
		item->ScaleY = (double)pic.Height()/D3DHeight/D3DSmooth;
	}

	item->Lighton = false;
	item->Shades = false;
	item->MagFilter[0] = true;
	item->MinFilter[0] = true;
	item->MipFilter[0] = true;

	item->AlphaRef = 0;
	item->AlphaRef2 = 0;
	item->IsAlphaRef = false;

	item->IsBlend = true;
	item->BlendRend = true;
	item->SrcBlend = BLEND_ONE;
	item->DstBlend = BLEND_ONE;
	item->SrcDstBlend = 11;

	item->RendGroup = RENDPIC;
	item->ZTest = D3DCMP_ALWAYS;
	item->ZWrite = FALSE;

	return D3D_ID;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� po�tu stup�� textur

double FGetD3DStage()
{
	return (double)MaxTextureSimult;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� operace barev

double FGetD3DColorOp()
{
	if (IsValidID())
	{
		return GetID()->ColorOp[D3DAktStage];
	}
	return D3DTOP_DISABLE;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� argumentu 1 barev

double FGetD3DColorArg1()
{
	if (IsValidID())
	{
		return GetID()->ColorArg1[D3DAktStage];
	}
	return D3DTA_TEXTURE;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� argumentu 2 barev

double FGetD3DColorArg2()
{
	if (IsValidID())
	{
		return GetID()->ColorArg2[D3DAktStage];
	}
	return D3DTA_CURRENT;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� operace alfa

double FGetD3DAlphaOp()
{
	if (IsValidID())
	{
		return GetID()->AlphaOp[D3DAktStage];
	}
	return D3DTOP_DISABLE;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� argumentu 1 alfa

double FGetD3DAlphaArg1()
{
	if (IsValidID())
	{
		return GetID()->AlphaArg1[D3DAktStage];
	}
	return D3DTA_TEXTURE;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� argumentu 2 alfa

double FGetD3DAlphaArg2()
{
	if (IsValidID())
	{
		return GetID()->AlphaArg2[D3DAktStage];
	}
	return D3DTA_CURRENT;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� horizont�ln�ho adresov�n�

double FGetD3DAddressU()
{
	if (IsValidID())
	{
		return GetID()->AddressU[D3DAktStage];
	}
	return D3DTADDRESS_WRAP;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� vertik�ln�ho adresov�n�

double FGetD3DAddressV()
{
	if (IsValidID())
	{
		return GetID()->AddressV[D3DAktStage];
	}
	return D3DTADDRESS_WRAP;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� barvy okol� textury
/*
double FGetD3DBorder()
{
	if (IsValidID())
	{
		return GetID()->Border[D3DAktStage];
	}
	return 0;
}
*/
/////////////////////////////////////////////////////////////////////////////
// zji�t�n� zjemn�n� vzd�len�ch textur

double FGetD3DBias()
{
	if (IsValidID())
	{
		return GetID()->Bias[D3DAktStage];
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� barvy faktoru textur

double FGetD3DTFactor()
{
	if (IsValidID())
	{
		return GetID()->TFactor;
	}
	return 0xffffff;
}

/////////////////////////////////////////////////////////////////////////////
// po�et �rovn� mipmaps

double FGetD3DMipMaps()
{
	if (IsValidID())
	{
		D3DFITEM* item = GetID();

		int text = item->Texture[D3DAktStage];

		if (D3DT_IsValid(text))
		{
			return D3DT_Get(text)->MipMaps;
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� renderovac� skupiny

double FGetD3DZGroup()
{
	if (IsValidID())
	{
		return (double)(GetID()->RendGroup);
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� hloubkov�ho testu

double FGetD3DZTest()
{
	if (IsValidID())
	{
		return (double)(GetID()->ZTest);
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� dosahu st�nu

double FGetD3DShadowRange()
{
	if (IsValidID())
	{
		return (double)(GetID()->ShadowRange);
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� objektu pro LOD

double FGetD3DLODObject()
{
	if (IsValidID())
	{
		return (double)(GetID()->LODNext);
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� vzd�lenosti pro LOD

double FGetD3DLODDistance()
{
	if (IsValidID())
	{
		return (GetID()->LODMax);
	}
	return 1e15;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� max. stupn� morfov�n�

double FGetD3DMorphModel()
{
	if (IsValidID())
	{
		return (GetID()->MorphMax);
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� stupn� morfov�n�

double FGetD3DMorphLevel()
{
	if (IsValidID())
	{
		return (GetID()->MorphStage);
	}
	return 0;
}


#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------


/////////////////////////////////////////////////////////////////////////////
// v�po�et vzd�lenosti bodu

double FPolarRadius()
{
	double x = FNum();
	double y = FNum();

	double n = sqrt(x*x + y*y);

	return n;
}

/////////////////////////////////////////////////////////////////////////////
// v�po�et �hlu bodu

double FPolarAngle()
{
	double x = FNum();
	double y = FNum();

	double n = atan2(y, x);
	if (n < 0) n = pi2 + n;

	return n;
}

/////////////////////////////////////////////////////////////////////////////
// p�esn� ��ta� �asu

bool PrecTimerInit = false;			// prob�hla inicializace p�esn�ho �asu

// p�esn� ��ta� �asu
bool IsPerformanceTimer = false;	// je p�esn� ��ta� �asu
double PerfFrequency = 0;			// frekvence p�esn�ho ��ta�e �asu
__int64 PerfCounter = 0;			// na�ten� hodnota p�esn�ho ��ta�e �asu

// multimedi�ln� ��ta�
double OldPrecTimer = 0;			// star� uschovan� hodnota ��ta�e
double AddPrecTimer = 0;			// p�idan� hodnota k ��ta�i (p�ete�en� po 49 dnech)

double FPreciseTimer()
{
// zji�t�n�, zda je p�esn� m��i� �asu
	if (!PrecTimerInit)
	{
		PrecTimerInit = true;

		if (::QueryPerformanceFrequency((LARGE_INTEGER*)&PerfCounter) &&
			(PerfCounter > (__int64)1000))
		{
			PerfFrequency = (double)PerfCounter;

			IsPerformanceTimer = true;
		}
	}

// obsluha pomoc� ��ta�e p�esn�ho �asu
	if (IsPerformanceTimer)
	{
		if (::QueryPerformanceCounter((LARGE_INTEGER*)&PerfCounter))
		{
			return (double)PerfCounter / PerfFrequency;
		}
	}

// na�ten� multimedi�ln�ho ��ta�e
	double time = (double)(DWORD)::timeGetTime() * 0.001;
	double newtime;

// korekce p�i p�ete�en� m���c� periody (49.71 dn�)
	for (;;)
	{
		newtime = time + AddPrecTimer;
		if (newtime >= OldPrecTimer) break;
		AddPrecTimer = AddPrecTimer + (double)4294967296;
	}
	OldPrecTimer = newtime;

	return newtime;
}

/***************************************************************************\
*																			*
*								obsluha DLL									*
*																			*
\***************************************************************************/

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

/////////////////////////////////////////////////////////////////////////////
// na�ten� DLL knihovny

double FDLLLoad()
{
// na�ten� jm�na knihovny
	CString jmeno;
	FText(jmeno);

// na�ten� knihovny
	HINSTANCE inst = ::LoadLibrary(jmeno);
	
	return (double)(int)inst;
}


/////////////////////////////////////////////////////////////////////////////
// importov�n� DLL funkce

double FDLLImport()
{
// na�ten� handle knihovny
	int module = FIntR();

// na�ten� jm�na funkce
	CString jmeno;
	FText(jmeno);

// na�ten� po�tu parametr�
	int param = FIntR();

// na�ten� p��znaku volac� konvence C
	bool conv = FBool();

// kontrola zad�n� parametr�
	if ((module == 0) || jmeno.IsEmpty() || (param < 0) || (param > 0xffff)) return 0;

// importov�n� funkce
	FARPROC proc = ::GetProcAddress((HMODULE)module, (LPCSTR)jmeno);

// p�i ne�sp�chu dopln�n� o p�smeno "A"
	if (proc == NULL)
	{
		jmeno += 'A';
		proc = ::GetProcAddress((HMODULE)module, (LPCSTR)jmeno);
	}

// p�i ne�sp�chu druh� pokus - vyhled�n� ��slem
/*
	if (proc == NULL)
	{
		if ((jmeno.Length() > 1) && (jmeno[0] == '#') &&
			((jmeno[1] == '$') ||
			((jmeno[1] >= '0') && (jmeno[1] <= '9'))))
		{
			int n = Int((LPCTSTR)(CString)jmeno.Mid(1));

			proc = ::GetProcAddress((HMODULE)module, (LPCSTR)n);
		}
	}
*/
// kontrola platnosti funkce
	if (proc == NULL) return 0;

// vytvo�en� bloku parametr�
	int* blok = (int*)MemGet(param*4 + 8);

// inicializace bloku parametr�
	blok[0] = (int)proc;
	blok[1] = param;
	if (conv) blok[1] |= 0x10000;
	MemFill(blok + 2, param * 4, 0);

	return (double)(int)blok;
}

/////////////////////////////////////////////////////////////////////////////
// proveden� DLL funkce

double FDLLExecNum()
{
// na�ten� bloku parametr� funkce
	int* blok = (int*)FIntR();

	int	retcode = 0;

#ifndef _M_IX86
	nepodporovan� p�eklad!!!!	
#endif

_asm {
		push	ecx

		mov		esi,[blok]			// bloku parametr�
		or		esi,esi				// jsou platn� data?
		jz		pasconv				// nen� platn� blok dat
		lods	dword ptr [esi]		// adresa funkce

		xor		ecx,ecx
		mov		cx,[esi]			// po�et parametr�
		shl		ecx,2				// d�lka dat v bajtech
		sub		esp,ecx				// vytvo�en� m�sta pro parametry
		mov		edi,esp
		mov		cx,[esi]			// po�et parametr�
		add		esi,4				// za��tek parametr� v bloku
		rep		movs dword ptr [edi], dword ptr [esi]

		call	eax					// vol�n� funkce
		mov		[retcode],eax		// n�vratov� k�d

		mov		esi,[blok]			// blok parametr�
		test	byte ptr [esi+6],1	// je volac� konvence C ?
		jz		pasconv				// nen� C, je Pascal

		xor		eax,eax
		mov		ax,[esi+4]			// EAX <- po�et parametr�
		shl		eax,2				// po�et bajt�
		add		esp,eax				// n�vrat ukazatele z�sobn�ku

pasconv:
		pop		ecx
	}

	return retcode;
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� bloku pam�ti

double FMemoryNew()
{
// na�ten� velikosti bloku
	int size = FIntR();

// vytvo�en� pam�ov�ho bloku
	void* blok = MemGet(size);

	return (double)(int)blok;
}

/////////////////////////////////////////////////////////////////////////////
// zm�na velikosti bloku pam�ti

double FMemoryResize()
{
// na�ten� star� adresy bloku
	void* blok = (void*)FIntR();

// na�ten� po�adovan� velikosti bloku
	int size = FIntR();

// zm�na velikosti bloku
	blok = MemSize(blok, size);

	return (double)(int)blok;
}

/////////////////////////////////////////////////////////////////////////////
// ukazatel �ten� z pam�ti

double FGetMemoryRead()
{
	return (double)(int)DLLMemoryRead;
}

/////////////////////////////////////////////////////////////////////////////
// ukazatel z�pisu do pam�ti

double FGetMemoryWrite()
{
	return (double)(int)DLLMemoryWrite;
}

/////////////////////////////////////////////////////////////////////////////
// porovn�n� dat

double FMemoryCompare()
{
// na�ten� d�lky dat
	int len = FIntR();

// adresa dat
	char* buf1 = (char*)DLLMemoryRead;
	char* buf2 = (char*)DLLMemoryWrite;

// porovn�n� dat
	for (; ((len > 0) && (*buf1 == *buf2)); len--)
	{
		buf1++;
		buf2++;
	}

	DLLMemoryRead = buf1;
	DLLMemoryWrite = buf2;

	return (double)len;
}

/////////////////////////////////////////////////////////////////////////////
// bajt se znam�nkem

double FGetMemoryByteS()
{
	char* buf = (char*)DLLMemoryRead;
	long data = (long)*buf;
	buf++;
	DLLMemoryRead = buf;
	return data;
}

/////////////////////////////////////////////////////////////////////////////
// bajt bez znam�nka

double FGetMemoryByteU()
{
	BYTE* buf = (BYTE*)DLLMemoryRead;
	long data = (long)*buf;
	buf++;
	DLLMemoryRead = buf;
	return data;
}

/////////////////////////////////////////////////////////////////////////////
// slovo se znam�nkem

double FGetMemoryWordS()
{
	short* buf = (short*)DLLMemoryRead;
	long data = (long)*buf;
	buf++;
	DLLMemoryRead = buf;
	return data;
}

/////////////////////////////////////////////////////////////////////////////
// slovo bez znam�nka

double FGetMemoryWordU()
{
	WORD* buf = (WORD*)DLLMemoryRead;
	long data = (long)*buf;
	buf++;
	DLLMemoryRead = buf;
	return data;
}

/////////////////////////////////////////////////////////////////////////////
// 3-bajt se znam�nkem

double FGetMemory3ByteS()
{
	char* buf = (char*)DLLMemoryRead;
	long data;
	*(WORD*)&data = *(WORD*)buf;
	((short*)&data)[1] = (short)buf[2];
	buf += 3;
	DLLMemoryRead = buf;
	return data;
}

/////////////////////////////////////////////////////////////////////////////
// 3-bajt bez znam�nka

double FGetMemory3ByteU()
{
	BYTE* buf = (BYTE*)DLLMemoryRead;
	long data;
	*(WORD*)&data = *(WORD*)buf;
	((WORD*)&data)[1] = (WORD)buf[2];
	buf += 3;
	DLLMemoryRead = buf;
	return data;
}

/////////////////////////////////////////////////////////////////////////////
// dvouslovo se znam�nkem

double FGetMemoryDWordS()
{
	long* buf = (long*)DLLMemoryRead;
	long data = *buf;
	buf++;
	DLLMemoryRead = buf;
	return data;
}

/////////////////////////////////////////////////////////////////////////////
// dvouslovo bez znam�nka

double FGetMemoryDWordU()
{
	DWORD* buf = (DWORD*)DLLMemoryRead;
	DWORD data = *buf;
	buf++;
	DLLMemoryRead = buf;
	return data;
}

/////////////////////////////////////////////////////////////////////////////
// re�ln� ��slo 4 bajty

double FGetMemoryFloat()
{
	float* buf = (float*)DLLMemoryRead;
	float data = *buf;
	buf++;
	DLLMemoryRead = buf;
	return data;
}

/////////////////////////////////////////////////////////////////////////////
// re�ln� ��slo 8 bajt�

double FGetMemoryDouble()
{
	double* buf = (double*)DLLMemoryRead;
	double data = *buf;
	buf++;
	DLLMemoryRead = buf;
	return data;
}

/////////////////////////////////////////////////////////////////////////////
// d�lka textu s pevnou d�lkou

double FGetMemoryTextNLen()
{
	return DLLMemoryTextNLen;
}


/////////////////////////////////////////////////////////////////////////////
// handle okenn�ho prvku

double FWindowHandle()
{
	int prvek = FIntR();

	if (Win.IsValid(prvek))
	{
		return (int)(Win[prvek].Wnd);
	}
	else
	{
		return 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� 3D objektu z pam�ti

double FMemory3D()
{
// na�ten� parametr�
	int vN = FIntR();						// po�et vrchol�
	D3DVECTOR* vert = (D3DVECTOR*)FIntR();	// buffer vrchol�
	D3DVECTOR* norm = (D3DVECTOR*)FIntR();	// buffer norm�l
	float* text = (float*)FIntR();			// buffer textur
	D3DCOLORVALUE* col = (D3DCOLORVALUE*)FIntR(); // buffer barev
	int fN = FIntR();						// po�et plo�ek
	int* face = (int*)FIntR();				// buffer plo�ek

// omezen� index� plo�ek
	for (int i = fN*3 - 1; i >= 0; i--)
	{
		if (face[i] >= vN) face[i] = 0;
	}

// vytvo�en� objektu
	int inx = D3DF_CreateMesh(vN, vert, fN, face);

// nastaven� norm�l
	if (norm != NULL)
	{
		D3DF_SetNormal(inx, norm);
	}
	else
	{
		D3DF_AutoNormals(inx, 0.5);
	}

// nastaven� mapov�n� textur
	if (text != NULL)
	{
		D3DF_SetTextUV(inx, text, 0);
	}

// nastaven� barev
	if (col != NULL)
	{
		D3DF_SetVertCol(inx, col);
	}

	D3D_ID = inx;
	D3DF_SetAddress(inx, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, 0);
	D3DF_KorRightHand();

	return D3D_ID;
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� pam�ov�ho bloku z 3D objektu

double FD3DMemory()
{
	int inx = FIntR();

	if (D3DF_IsValid(inx))
	{
		D3DFITEM* item = D3DF_Get(inx);

		int* mem = (int*)MemGet(8*sizeof(int));

		mem[0] = 0;

		int vN = item->VertNum;
		mem[1] = vN;

		D3DVECTOR* v = (D3DVECTOR*)MemGet(vN * sizeof(D3DVECTOR));
		MemCopy(v, item->Vertex, vN * sizeof(D3DVECTOR));
		mem[2] = (int)v;

		D3DVECTOR* n = (D3DVECTOR*)MemGet(vN * sizeof(D3DVECTOR));
		MemCopy(n, item->Normal, vN * sizeof(D3DVECTOR));
		mem[3] = (int)n;

		float* tex = NULL;
		if (item->TextUV[0] != NULL)
		{
			tex = (float*)MemGet(vN * 2 * sizeof(float));
			MemCopy(tex, item->TextUV[0], vN * 2 * sizeof(float));
		}
		mem[4] = (int)tex;

		D3DCOLORVALUE* col = NULL;
		if (item->VertColF != NULL)
		{
			col = (D3DCOLORVALUE*)MemGet(vN * sizeof(D3DCOLORVALUE));
			MemCopy(col, item->VertColF, vN * sizeof(D3DCOLORVALUE));
		}
		mem[5] = (int)col;

		int fN = item->FaceNum;
		mem[6] = fN;

		int* face = (int*)MemGet(fN * 3 * sizeof(int));
		MemCopy(face, item->Face, fN * 3 * sizeof(int));
		mem[7] = (int)face;

		return (int)mem;
	}
	else
	{
		return 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
// poskytnut� potomka 3D objektu

double FD3DChild0()
{
	if (IsValidID())
	{
		return (double)(GetID()->Child);
	}
	return -1;
}

double FD3DChild()
{
	int inx = FIntR();

// nalezen� prvn�ho potomka
	if (inx == -1)
	{
		return FD3DChild0();
	}

// nalezen� rodi�e
	if (inx == -2)
	{
		if (IsValidID())
		{
			return (double)(GetID()->Parent);
		}
		return -1;
	}

// nalezen� dal��ho potomka
	if (D3DF_IsValid(inx))
	{
		D3DFITEM* item = D3DF_Get(inx);
		return item->Next;
	}
	return -1;
}


/***************************************************************************\
*																			*
*								konfigurace									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// �ten� konfigura�n�ho ��sla

double FGetIniNum()
{
	int cislo = FIntR();
	CString text;
	text.Int(cislo);
	GetIniProfileString(text);
	return (double)Int((LPCTSTR)text);
}

double FGetIniNum0()
{
	CString text('0');
	GetIniProfileString(text);
	return (double)Int((LPCTSTR)text);
}

/////////////////////////////////////////////////////////////////////////////
// skupina kl��� registru

double FGetRegKey()
{
	return REGKey;
}

/////////////////////////////////////////////////////////////////////////////
// �ten� ��sla z registru

int GetRegNum(int cislo)
{
// otev�en� kl��e
	HKEY hkey;

	if (::RegOpenKeyEx(
		(HKEY) (REGKey | 0x80000000),
		REGSubkey,
		0,
		KEY_QUERY_VALUE,
		&hkey) == ERROR_SUCCESS)
	{

// na�ten� hodnoty kl��e
		DWORD bufsize = 4;

		::RegQueryValueEx(
			hkey,
			REGValue,
			0,
			NULL,
			(BYTE*)&cislo,
			&bufsize);

// uzav�en� kl��e
		::RegCloseKey(hkey);
	}

	return cislo;
}

double FGetRegNum()
{
	int cislo = FIntR();
	return GetRegNum(cislo);
}

double FGetRegNum0()
{
	return GetRegNum(0);
}

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------
