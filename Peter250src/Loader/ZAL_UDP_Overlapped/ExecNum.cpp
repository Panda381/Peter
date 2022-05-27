
#include "Main.h"

/***************************************************************************\
*																			*
*					Provádìní programu - èíselný výraz						*
*																			*
\***************************************************************************/

#pragma optimize("t", on)			// optimalizace na maximální rychlost

SYSTEMTIME	SystemTime;				// pomocná promìnná pro obsluhu èasu

/////////////////////////////////////////////////////////////////////////////
// naètení èíselného parametru pøevedeného na int - seøíznuto smìrem k nule

//int FInt() { return (int)FNum(); }


/////////////////////////////////////////////////////////////////////////////
// naètení úhlu s normalizací rozsahu 0 až 2pi

#pragma warning ( disable: 4725)	// hlášení - problémová instrukce

double FAngle()
{

// naètení úhlu
	double num = FNum();

// bude normalizace pro kladný úhel > 2pi
	if (num >= pi2)
	{

// normalizace velkých kladných úhlù
		if (num >= pi8)
		{
			double num2 = pi2;

#ifdef _M_IX86

			_asm {
				fld			num2		// naètení dìlitele -> ST0
				fld			num			// naètení dìlence -> ST0, dìlitel -> ST1
X1:				fprem					// zbytek -> ST0, v ST1 zùstává dìlitel
				wait					// synchronizace
				fnstsw		ax			// naètení stavového slova
				wait					// synchronizace
				sahf					// nastavení stavových pøíznakù
				jp			X1			// operace neúplná - pokraèování
				fstp		num			// výsledek operace
				fstp		st(0)		// zrušení ST0 (dìlitel)
			}

#else

			num = fmod(num, num2);

#endif

#define MULT 360						// tj. 5*3*3*2*2*2 (asi tak možné násobky)
#define EPS 0.01						// asi tak pro rozeznání násobku

// zarovnání nepøesností, pokud se jedná o nìjaký podíl úhlu
			double num3 = num / pi * MULT;
			int m = Round(num3);		// to jsou možné násobky
			double num4 = num3 - m;		// odchylka od násobkù
			if ((num4 <= EPS) && (num4 >= -EPS))
			{
				num = m * pi / MULT;	// zarovnání výsledku

				if (num >= pi2) num += pi2m;
			}
		}

// jinak normalizace malých kladných úhlù
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

// bude normalizace pro záporný úhel < 0
		if (num < 0)
		{

// normalizace velkých záporných úhlù
			if (num <= pi8m)
			{
				num = -num;
				double num2 = pi2;

#ifdef _M_IX86

				_asm {
					fld			num2		// naètení dìlitele -> ST0
					fld			num			// naètení dìlence -> ST0, dìlitel -> ST1
X2:					fprem					// zbytek -> ST0, v ST1 zùstává dìlitel
					wait					// synchronizace
					fnstsw		ax			// naètení stavového slova
					wait					// synchronizace
					sahf					// nastavení stavových pøíznakù
					jp			X2			// operace neúplná - pokraèování
					fstp		num			// výsledek operace
					fstp		st(0)		// zrušení ST0 (dìlitel)
				}

#else

				num = fmod(num, num2);

#endif

				num = pi2 - num;
				if (num >= pi2) num += pi2m;

// zarovnání nepøesností, pokud se jedná o nìjaký podíl úhlu
				double num3 = num / pi * MULT;
				int m = Round(num3);		// to jsou možné násobky
				double num4 = num3 - m;		// odchylka od násobkù
				if ((num4 <= EPS) && (num4 >= -EPS))
				{
					num = m * pi / MULT;	// zarovnání výsledku

					if (num >= pi2) num += pi2m;
				}
			}

// jinak normalizace malých záporných úhlù
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

#pragma warning ( default: 4725)	// hlášení - problémová instrukce


/////////////////////////////////////////////////////////////////////////////
// naètení èíselného parametru pøevedeného na int se zaokrouhlením

int FIntR()
{
	int result;						// výsledek operace
	double num;						// naètené èíslo

	num = FNum();					// naètení èísla

#ifdef _M_IX86

	_asm {
		fld			num				// naètení èísla do koprocesoru
		fistp		result			// pøevod èísla na celé èíslo
	}

#else

	result = Round(num);			// zaokrouhlení èísla

#endif

	return result;					// výsledek operace
}


/////////////////////////////////////////////////////////////////////////////
// naètení èíselného parametru pøevedeného na DWORD se zaokrouhlením

DWORD FIntRN()
{
	__int64 result;					// výsledek operace
	double num;						// naètené èíslo

	num = FNum();					// naètení èísla

#ifdef _M_IX86

	_asm {
		fld			num				// naètení èísla do koprocesoru
		fistp		result			// pøevod èísla na celé èíslo
	}

#else

	if (num >= 0x80000000)
	{
		result = Round(num - 0x80000000) + 0x80000000;
	}
	else
	{
		result = Round(num);			// zaokrouhlení èísla
	}

#endif

	return (DWORD)result;				// výsledek operace
}

/////////////////////////////////////////////////////////////////////////////
// naètení èíselného parametru pøevedeného na INT64 se zaokrouhlením

hyper FIntHN()
{
	hyper result;					// výsledek operace
	double num;						// naètené èíslo

	num = FNum();					// naètení èísla

#ifdef _M_IX86

	_asm {
		fld			num				// naètení èísla do koprocesoru
		fistp		result			// pøevod èísla na celé èíslo
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
			result = Round(num);			// zaokrouhlení èísla
		}
	}

#endif

	return result;				// výsledek operace
}

/////////////////////////////////////////////////////////////////////////////
// naètení èíselného parametru pøevedeného na grafickou souøadnici X

int FIntX()
{
	int result;						// výsledek operace
	double num;						// naètené èíslo

	num = FNum() * ICONWIDTH;		// naètení èísla

#ifdef _M_IX86

	_asm {
		fld			num				// naètení èísla do koprocesoru
		fistp		result			// pøevod èísla na celé èíslo
	}

#else

	result = Round(num);			// zaokrouhlení èísla

#endif

	return result;					// výsledek operace
}


/////////////////////////////////////////////////////////////////////////////
// naètení èíselného parametru vynásobeného * 1000 (èas v milisekundách)

int FInt1000()
{
	int result;						// výsledek operace
	double num;						// naètené èíslo

	num = FNum() * 1000;			// naètení èísla

#ifdef _M_IX86

	_asm {
		fld			num				// naètení èísla do koprocesoru
		fistp		result			// pøevod èísla na celé èíslo
	}

#else

	result = Round(num);			// zaokrouhlení èísla

#endif

	return result;					// výsledek operace
}


/////////////////////////////////////////////////////////////////////////////
// naètení èíselného parametru vynásobeného * ICONWIDTH * 2 (prùmìr kruhu)

int FIntX2()
{
	int result;						// výsledek operace
	double num;						// naètené èíslo

	num = FNum() * ICONWIDTH * 2;				// naètení èísla

#ifdef _M_IX86

	_asm {
		fld			num				// naètení èísla do koprocesoru
		fistp		result			// pøevod èísla na celé èíslo
	}

#else

	result = Round(num);			// zaokrouhlení èísla

#endif

	return result;					// výsledek operace
}


/////////////////////////////////////////////////////////////////////////////
// naètení èíselného parametru vynásobeného * 18.20648 (èas v èasových impulsech)

int FInt18()
{
	int result;						// výsledek operace
	double num;						// naètené èíslo

	num = FNum() * 18.20648;		// naètení èísla

#ifdef _M_IX86

	_asm {
		fld			num				// naètení èísla do koprocesoru
		fistp		result			// pøevod èísla na celé èíslo
	}

#else

	result = Round(num);			// zaokrouhlení èísla

#endif

	return result;					// výsledek operace
}


/////////////////////////////////////////////////////////////////////////////
// naètení èíselného parametru s vynásobením * 255 a omezením 0 až 255

BYTE FInt255()
{
	int result;						// výsledek operace
	double num;						// naètené èíslo
	num = FNum() * 255;				// naètení èísla

#ifdef _M_IX86

	_asm {
		fld			num				// naètení èísla do koprocesoru
		fistp		result			// pøevod èísla na celé èíslo
	}

#else

	result = Round(num);			// naètení èísla se zaokrouhlením

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

	return (BYTE)result;			// výsledek operace
}


/////////////////////////////////////////////////////////////////////////////
// naètení èíselného parametru pøevedeného na souborový èas

__int64 FNumFileTime()
{
	__int64 cas;					// pomocná promìnná k naètení èasu
	double num;						// naètené èíslo
	num = FNum() * FILETIMEKONV;	// naètení èísla

#ifdef _M_IX86

	_asm {
		fld			num				// naètení èísla do koprocesoru
		fistp		cas				// pøevod èísla na celé èíslo
	}

#else

	cas = (__int64)(num + 0.5);

#endif

	if (((char*)&cas)[7] < 0) return 0;

	return cas;
}


/////////////////////////////////////////////////////////////////////////////
// naètení èíselného parametru pøevedeného na systémový èas

void FNumSysTime()
{
	__int64 cas = FNumFileTime();	// naètení souborového èasu
	::FileTimeToSystemTime((FILETIME*)&cas, &SystemTime);
}


/////////////////////////////////////////////////////////////////////////////
// naètení èíselného parametru pøevedeného na svìtový souborový èas

__int64 FNumFileTimeUTC()
{
// naètení lokálního èasu
	__int64 cas = FNumFileTime();	// naètení lokálního èasu

// pøevod na svìtový èas
	__int64 cas2;
	::LocalFileTimeToFileTime((FILETIME*)&cas, (FILETIME*)&cas2);

	return cas2;
}


/////////////////////////////////////////////////////////////////////////////
// naètení barvy

int FNumCol()
{
/*
	DWORD barva;					// naètená barva
	double num;						// naètené èíslo
	num = FNum();					// naètení èísla

#ifdef _M_IX86

	_asm {
		fld			num				// naètení èísla do koprocesoru
		fistp		barva			// pøevod èísla na celé èíslo
	}

#else

	barva = Round(num);				// zaokrouhlení èísla

#endif
*/
	return PalImport(FIntRN());		// výsledek operace
}


/////////////////////////////////////////////////////////////////////////////
// naètení souøadnic Petra (ECX = adresa x, EDX = adresa y)

bool _fastcall PetrXY(int& x, int& y)
{
#ifdef _M_IX86

	__asm {
		mov			eax,[Sprite.m_Data]								// adresa dat sprajtu
		fld			qword ptr [eax + 0*TYPE CSprite]CSprite.m_CilX  // naètení souøadnice X
		fistp		dword ptr [ecx]									// pøevod X na celé èíslo
		fld			qword ptr [eax + 0*TYPE CSprite]CSprite.m_CilY  // naètení souøadnice Y
		fistp		dword ptr [edx]									// pøevod Y na celé èíslo
	}

#else

	x = Round(Sprite[0].CilX());
	y = Round(Sprite[0].CilY());

#endif

// kontrola platnosti souøadnice
	return (((DWORD)x < (DWORD)WidthN) && ((DWORD)y < (DWORD)HeightN));
}


/////////////////////////////////////////////////////////////////////////////
// naètení souøadnic Petry (ECX = adresa x, EDX = adresa y)

bool _fastcall Petr2XY(int& x, int& y)
{
#ifdef _M_IX86

	__asm {
		mov			eax,[Sprite.m_Data]								// adresa dat sprajtu
		fld			qword ptr [eax + 1*TYPE CSprite]CSprite.m_CilX  // naètení souøadnice X
		fistp		dword ptr [ecx]									// pøevod X na celé èíslo
		fld			qword ptr [eax + 1*TYPE CSprite]CSprite.m_CilY  // naètení souøadnice Y
		fistp		dword ptr [edx]									// pøevod Y na celé èíslo
	}

#else

	x = Round(Sprite[1].CilX());
	y = Round(Sprite[1].CilY());

#endif

// kontrola platnosti souøadnice
	return (((DWORD)x < (DWORD)WidthN) && ((DWORD)y < (DWORD)HeightN));
}


/////////////////////////////////////////////////////////////////////////////
// naètení pozice Petra (NULL=neplatná)

MAPITEM* PetrXYMap()
{
	int x;			// souøadnice X Petra
	int y;			// souøadnice Y Petra

#ifdef _M_IX86

	__asm {
		mov			eax,[Sprite.m_Data]								// adresa dat sprajtu
		fld			qword ptr [eax + 0*TYPE CSprite]CSprite.m_CilX  // naètení souøadnice X
		fistp		dword ptr x										// pøevod X na celé èíslo
		fld			qword ptr [eax + 0*TYPE CSprite]CSprite.m_CilY  // naètení souøadnice Y
		fistp		dword ptr y										// pøevod Y na celé èíslo
	}

#else

	x = Round(Sprite[0].CilX());
	y = Round(Sprite[0].CilY());

#endif

// kontrola platnosti souøadnice
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
// naètení pozice Petry (NULL=neplatná)

MAPITEM* Petr2XYMap()
{
	int x;			// souøadnice X Petra
	int y;			// souøadnice Y Petra

#ifdef _M_IX86

	__asm {
		mov			eax,[Sprite.m_Data]								// adresa dat sprajtu
		fld			qword ptr [eax + 1*TYPE CSprite]CSprite.m_CilX  // naètení souøadnice X
		fistp		dword ptr x										// pøevod X na celé èíslo
		fld			qword ptr [eax + 1*TYPE CSprite]CSprite.m_CilY  // naètení souøadnice Y
		fistp		dword ptr y										// pøevod Y na celé èíslo
	}

#else

	x = Round(Sprite[1].CilX());
	y = Round(Sprite[1].CilY());

#endif

// kontrola platnosti souøadnice
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
// naètení pøíští souøadnice Petra (ECX = adresa x, EDX = adresa y)

bool _fastcall PetrXYNext(int& x, int& y)
{
#ifdef _M_IX86

	__asm {
		mov			eax,[Sprite.m_Data]								// adresa dat sprajtu
		fld			qword ptr [eax + 0*TYPE CSprite]CSprite.m_CilX  // naètení souøadnice X
		fistp		dword ptr [ecx]									// pøevod X na celé èíslo
		fld			qword ptr [eax + 0*TYPE CSprite]CSprite.m_CilY  // naètení souøadnice Y
		fistp		dword ptr [edx]									// pøevod Y na celé èíslo
	}

#else

	x = Round(Sprite[0].CilX());
	y = Round(Sprite[0].CilY());

#endif

	switch (16 * Sprite[0].SmerN() / Sprite[0].Smer())
	{
	case 1:					// vpravo mírnì vzhùru
	case 2:					// vpravo vzhùru
	case 3:					// mírnì vpravo vzhùru
		x++;

	case 4:					// vzhùru
		y++;
		break;

	case 5:					// mírnì vlevo vzhùru
	case 6:					// vlevo vzhùru
	case 7:					// vlevo mírnì vzhùru
		y++;

	case 8:					// vlevo
		x--;
		break;

	case 9:					// vlevo mírnì dolù
	case 10:				// vlevo dolù
	case 11:				// mírnì vlevo dolù
		x--;

	case 12:				// dolù
		y--;
		break;

	case 13:				// mírnì vpravo dolù
	case 14:				// vpravo dolù
	case 15:				// vpravo mírnì dolù
		y--;

	case 0:					// vpravo
		x++;
		break;
	}

// kontrola platnosti souøadnice
	return (((DWORD)x < (DWORD)WidthN) && ((DWORD)y < (DWORD)HeightN));
}


/////////////////////////////////////////////////////////////////////////////
// naètení pøíští souøadnice Petry (ECX = adresa x, EDX = adresa y)

bool _fastcall Petr2XYNext(int& x, int& y)
{
#ifdef _M_IX86

	__asm {
		mov			eax,[Sprite.m_Data]								// adresa dat sprajtu
		fld			qword ptr [eax + 1*TYPE CSprite]CSprite.m_CilX  // naètení souøadnice X
		fistp		dword ptr [ecx]									// pøevod X na celé èíslo
		fld			qword ptr [eax + 1*TYPE CSprite]CSprite.m_CilY  // naètení souøadnice Y
		fistp		dword ptr [edx]									// pøevod Y na celé èíslo
	}

#else

	x = Round(Sprite[1].CilX());
	y = Round(Sprite[1].CilY());

#endif

	switch (16 * Sprite[1].SmerN() / Sprite[1].Smer())
	{
	case 1:					// vpravo mírnì vzhùru
	case 2:					// vpravo vzhùru
	case 3:					// mírnì vpravo vzhùru
		x++;

	case 4:					// vzhùru
		y++;
		break;

	case 5:					// mírnì vlevo vzhùru
	case 6:					// vlevo vzhùru
	case 7:					// vlevo mírnì vzhùru
		y++;

	case 8:					// vlevo
		x--;
		break;

	case 9:					// vlevo mírnì dolù
	case 10:				// vlevo dolù
	case 11:				// mírnì vlevo dolù
		x--;

	case 12:				// dolù
		y--;
		break;

	case 13:				// mírnì vpravo dolù
	case 14:				// vpravo dolù
	case 15:				// vpravo mírnì dolù
		y--;

	case 0:					// vpravo
		x++;
		break;
	}

// kontrola platnosti souøadnice
	return (((DWORD)x < (DWORD)WidthN) && ((DWORD)y < (DWORD)HeightN));
}


/***************************************************************************\
*																			*
*									funkce									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// funkce s návratem èíselné hodnoty

double FNumFunc()
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
	double result = Real[Real.Num() - res];

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
// èíselná konstanta

double FNumConst() { return ExecItem[-1].Double; }


/////////////////////////////////////////////////////////////////////////////
// globální promìnná (Data = index)

double FNumObj()
{
// index
	int inx = ExecItem[-1].Data;

// data
	return Real[inx];
}


/////////////////////////////////////////////////////////////////////////////
// globalní promìnná v seznamu (Data = index, List = seznam)

double FNumObjList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index promìnné, inkrementace indexu seznamu
	int inx = ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// data
	return Real[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lokální promìnná (Data = index)

double FNumLoc()
{
// index promìnné
	int inx = Real.Num() - ExecItem[-1].Data;

// data
	return Real[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lokální promìnná v seznamu (Data = index, List = seznam)

double FNumLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index promìnné, inkrementace indexu seznamu
	int inx = Real.Num() - ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// data
	return Real[inx];
}


/////////////////////////////////////////////////////////////////////////////
// index globálního seznamu

double FGetList()
{
// index seznamu
	int list = ExecItem[-1].List;

// hodnota indexu
	return List[list].Index;
}


/////////////////////////////////////////////////////////////////////////////
// index lokálního seznamu

double FGetListLoc()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// hodnota indexu
	return List[list].Index;
}


/////////////////////////////////////////////////////////////////////////////
// inkrementace globálního seznamu

double FGetListAuto()
{
// index seznamu
	int list = ExecItem[-1].List;

// nastavení nové hodnoty inkrementace
	return List[list].Auto;
}


/////////////////////////////////////////////////////////////////////////////
// inkrementace lokálního seznamu

double FGetListAutoLoc()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// nastavení nové hodnoty inkrementace
	return List[list].Auto;
}


/////////////////////////////////////////////////////////////////////////////
// souøadnice X globálního sprajtu (Data = index)

double FGetSpriteX()
{
// index sprajtu
	int inx = ExecItem[-1].Data;

// souøadnice X sprajtu
	return Sprite[inx].AktX();
}


/////////////////////////////////////////////////////////////////////////////
// souøadnice X sprajtu v seznamu (Data = index, List = seznam)

double FGetSpriteXList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index sprajtu, inkrementace indexu seznamu
	int inx = ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// souøadnice X sprajtu
	return Sprite[inx].AktX();
}


/////////////////////////////////////////////////////////////////////////////
// souøadnice X lokálního sprajtu (Data = index)

double FGetSpriteXLoc()
{
// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;

// souøadnice X sprajtu
	return Sprite[inx].AktX();
}


/////////////////////////////////////////////////////////////////////////////
// souøadnice X lokálního sprajtu v seznamu (Data = index, List = seznam)

double FGetSpriteXLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index sprajtu, inkrementace indexu seznamu
	int inx = Sprite.Num() - ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// souøadnice X sprajtu
	return Sprite[inx].AktX();
}


/////////////////////////////////////////////////////////////////////////////
// souøadnice Y globálního sprajtu (Data = index)

double FGetSpriteY()
{
// index sprajtu
	int inx = ExecItem[-1].Data;

// souøadnice Y sprajtu
	return Sprite[inx].AktY();
}


/////////////////////////////////////////////////////////////////////////////
// souøadnice Y sprajtu v seznamu (Data = index, List = seznam)

double FGetSpriteYList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index sprajtu, inkrementace indexu seznamu
	int inx = ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// souøadnice Y sprajtu
	return Sprite[inx].AktY();
}


/////////////////////////////////////////////////////////////////////////////
// souøadnice Y lokálního sprajtu (Data = index)

double FGetSpriteYLoc()
{
// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;

// souøadnice Y sprajtu
	return Sprite[inx].AktY();
}


/////////////////////////////////////////////////////////////////////////////
// souøadnice Y lokálního sprajtu v seznamu (Data = index, List = seznam)

double FGetSpriteYLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index sprajtu, inkrementace indexu seznamu
	int inx = Sprite.Num() - ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// souøadnice Y sprajtu
	return Sprite[inx].AktY();
}


/////////////////////////////////////////////////////////////////////////////
// smìr globálního sprajtu (Data = index)

double FGetSpriteSmer()
{
// index sprajtu
	int inx = ExecItem[-1].Data;

// smìr sprajtu
	return Sprite[inx].SmerR();
}


/////////////////////////////////////////////////////////////////////////////
// smìr sprajtu v seznamu (Data = index, List = seznam)

double FGetSpriteSmerList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index sprajtu, inkrementace indexu seznamu
	int inx = ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// smìr sprajtu
	return Sprite[inx].SmerR();
}


/////////////////////////////////////////////////////////////////////////////
// smìr lokálního sprajtu (Data = index)

double FGetSpriteSmerLoc()
{
// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;

// smìr sprajtu
	return Sprite[inx].SmerR();
}


/////////////////////////////////////////////////////////////////////////////
// smìr lokálního sprajtu v seznamu (Data = index, List = seznam)

double FGetSpriteSmerLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index sprajtu, inkrementace indexu seznamu
	int inx = Sprite.Num() - ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// smìr sprajtu
	return Sprite[inx].SmerR();
}


/////////////////////////////////////////////////////////////////////////////
// fáze globálního sprajtu (Data = index)

double FGetSpriteFaze()
{
// index sprajtu
	int inx = ExecItem[-1].Data;

// fáze sprajtu
	return SpriteNextFaze(inx);
}


/////////////////////////////////////////////////////////////////////////////
// fáze sprajtu v seznamu (Data = index, List = seznam)

double FGetSpriteFazeList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index sprajtu, inkrementace indexu seznamu
	int inx = ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// fáze sprajtu
	return SpriteNextFaze(inx);
}


/////////////////////////////////////////////////////////////////////////////
// fáze lokálního sprajtu (Data = index)

double FGetSpriteFazeLoc()
{
// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;

// fáze sprajtu
	return SpriteNextFaze(inx);
}


/////////////////////////////////////////////////////////////////////////////
// fáze lokálního sprajtu v seznamu (Data = index, List = seznam)

double FGetSpriteFazeLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index sprajtu, inkrementace indexu seznamu
	int inx = Sprite.Num() - ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// fáze sprajtu
	return SpriteNextFaze(inx);
}


/////////////////////////////////////////////////////////////////////////////
// stav globálního sprajtu (Data = index)

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
// stav lokálního sprajtu (Data = index)

double FGetSpriteStatusLoc()
{
// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;

// stav sprajtu
	return Sprite[inx].Status();
}


/////////////////////////////////////////////////////////////////////////////
// stav lokálního sprajtu v seznamu (Data = index, List = seznam)

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
// Petr - smìr

double FGetSmer() { return Sprite[0].SmerR(); }


/////////////////////////////////////////////////////////////////////////////
// Petra - smìr

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
// naètení èísla - Petr

double _fastcall FGetSwcN(int mask, char rot)
{
// naètení pozice Petra
	MAPITEM* item = PetrXYMap();

// naètení èísla, je-li platná pozice
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
// naètení èísla - Petra

double _fastcall FGet2SwcN(int mask, char rot)
{
// naètení pozice Petry
	MAPITEM* item = Petr2XYMap();

// naètení èísla, je-li platná pozice
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
*								výpoèty										*
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
// celé èíslo

double FNumInt() { return (int)FNum(); };


/////////////////////////////////////////////////////////////////////////////
// desetinná èást èísla

double FFrac()
{
	double num = FNum();
	return num - (int)num;
}


/////////////////////////////////////////////////////////////////////////////
// absolutní hodnota

double FAbs() { return fabs(FNum()); };


/////////////////////////////////////////////////////////////////////////////
// odmocnina

double FSqrt() { return sqrt(FNum()); };


/////////////////////////////////////////////////////////////////////////////
// faktoriál

double FFact()
{
// naètení èísla pro výpoèet faktoriálu
	int citac = FIntR();

// kladná hodnota v povoleném rozsahu
	if ((DWORD)citac <= MAXFACT+1) return FactTab[citac];

// pøeteèení kladným smìrem
	if (citac > 0) return FactTab[MAXFACT+1];

// absolutní hodnota èísla
	citac = -citac;

// pøeteèení záporným smìrem
	if (citac > MAXFACT) return -FactTab[MAXFACT+1];

// hodnota v záporném smìru (lichá èísla jsou záporná)
	if (citac & 1) return -FactTab[citac];
	return FactTab[citac];
}


/////////////////////////////////////////////////////////////////////////////
// náhodné èíslo s násobkem

double FRand() { return FNum() * Rand(); };


/////////////////////////////////////////////////////////////////////////////
// náhodné èíslo bez násobku

double FRand0() { return Rand(); };


/////////////////////////////////////////////////////////////////////////////
// pøevod stupòù na radiány

double FDegRad() { return FNum() * degrad; };


/////////////////////////////////////////////////////////////////////////////
// pøevod radiánù na stupnì

double FRadDeg() { return FNum() * raddeg; };


/////////////////////////////////////////////////////////////////////////////
// dekadický logaritmus

double FLog() { return log10(FNum()); }


/////////////////////////////////////////////////////////////////////////////
// pøirozený logaritmus

double FLn() { return log(FNum()); }


/////////////////////////////////////////////////////////////////////////////
// pøirozený exponent

double FExp() { return exp(FNum()); }


/////////////////////////////////////////////////////////////////////////////
// operátor ADD (Data = poèet prvkù - 1)

double FAdd()
{
// pøíprava poètu prvkù
	int i = ExecItem[-1].Data;

// naètení prvního èísla
	double num = FNum();

// cyklus pøes další èísla
	for (; i > 0; i--)
	{

// operace s dalším èíslem
		num += FNum();
	}
	return num;
}


/////////////////////////////////////////////////////////////////////////////
// operátor SUB pro 1 prvek

double FSub1() { return -FNum(); };


/////////////////////////////////////////////////////////////////////////////
// operátor SUB (Data = poèet prvkù - 1)

double FSub()
{
// pøíprava poètu prvkù
	int i = ExecItem[-1].Data;

// naètení prvního èísla
	double num = FNum();

// cyklus pøes další èísla
	for (; i > 0; i--)
	{

// operace s dalším èíslem (tento zápis je efektivnìjší pro pøeklad)
		num = -(FNum() - num);
	}
	return num;
}


/////////////////////////////////////////////////////////////////////////////
// operátor MUL (Data = poèet prvkù - 1)

double FMul()
{
// pøíprava poètu prvkù
	int i = ExecItem[-1].Data;

// naètení prvního èísla
	double num = FNum();

// cyklus pøes další èísla
	for (; i > 0; i--)
	{

// operace s dalším èíslem
		num *= FNum();
	}
	return num;
}

/////////////////////////////////////////////////////////////////////////////
// operátor DIV pro 1 prvek

double FDiv1() { return 1/FNum(); };


/////////////////////////////////////////////////////////////////////////////
// operátor DIV (Data = poèet prvkù - 1)

double FDiv()
{
// pøíprava poètu prvkù
	int i = ExecItem[-1].Data;

// naètení prvního èísla
	double num = FNum();

// cyklus pøes další èísla
	for (; i > 0; i--)
	{

// naètení dalšího èísla
		double num2 = FNum();

// operace s dalším èíslem (v pøípadì 0 / 0 bude výsledek 1)
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

#pragma warning ( disable: 4725)	// hlášení - problémová instrukce

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
		fld			num2			// naètení dìlitele -> ST0
		fld			num1			// naètení dìlence -> ST0, dìlitel -> ST1
X1:		fprem						// zbytek -> ST0, v ST1 zùstává dìlitel
		wait						// synchronizace
		fnstsw		ax				// naètení stavového slova
		wait						// synchronizace
		sahf						// nastavení stavových pøíznakù
		jp			X1				// operace neúplná - pokraèování
		fstp		num				// výsledek operace
		fstp		st(0)			// zrušení ST0 (dìlitel)
	}

#else

		num = fmod(num1, num2);

#endif

		return num;
}

#pragma warning ( default: 4725)	// hlášení - problémová instrukce

/////////////////////////////////////////////////////////////////////////////
// operátor MOD (Data = poèet prvkù - 1)

double FMod()
{
// pøíprava poètu prvkù
	int i = ExecItem[-1].Data;

// naètení prvního èísla
	double num = FNum();

// cyklus pøes další èísla
	for (; i > 0; i--)
	{

// operace s dalším èíslem
		double num2 = FNum();			// naètení dalšího èísla
		num = NumMod(num, num2);
	}
	return num;
}

/////////////////////////////////////////////////////////////////////////////
// operátor MOCNINA (Data = poèet prvkù - 1)

double FMocn()
{
// pøíprava poètu prvkù
	int i = ExecItem[-1].Data;

// naètení prvního èísla
	double num = FNum();

// cyklus pøes další èísla
	for (; i > 0; i--)
	{

// umocnìní dalším èíslem
		num = pow(num, FNum());
	}
	return num;
}


/////////////////////////////////////////////////////////////////////////////
// druhá mocnina

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
// nastavení jasu barvy

double FColorLevel()
{
// naètení barvy, pøevod na základní jas barvy
	int col = FNumCol();
	if ((col == BackCol) || (col == ShadCol)) col = BlackCol;
	col = (col - ResCols) / ColLev * ColLev + ResCols;

// pøíprava poètu odstínù pro barvu
	int cols = ColLev + 1;				// poèet odstínù pro bìžné barvy
	if (col >= StdColors - 2*ColLev)	// je to bílá nebo šedá?
	{
		col = StdColors - 2*ColLev;		// korekce na bílou barvu
		cols = 2*ColLev;				// poèet odstínù pro bílou barvu
	}

// naètení jasu barvy
	double level = FNum()*(cols-1);

// pøevod na index jasu barvy
	int jas;
	
#ifdef _M_IX86

	_asm {
		fld			level				// naètení èísla do koprocesoru
		fistp		jas					// pøevod èísla na celé èíslo
	}

#else

	jas = Round(level);					// zaokrouhlení èísla

#endif
	
// omezení indexu jasu barvy
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

// pro jas 0 se použije èerná barva
	if (jas == 0) return 0;

// výsledná barva
	return PalExport(col + cols - 1 - jas);
}


/////////////////////////////////////////////////////////////////////////////
// syntéza barvy

double FColorSynth()
{
	DWORD color;
	((BYTE*)&color)[0] = FInt255();		// èervená
	((BYTE*)&color)[1] = FInt255();		// zelená
	((BYTE*)&color)[2] = FInt255();		// modrá
	((BYTE*)&color)[3] = (BYTE)(FInt255() ^ 0xff);// alfa
	return color;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní barvy bodu

double FGetPoint()
{
// naètení souøadnic bodu
	int x = FIntX();
	int y = FIntY();

// implicitní barva - pozadí
	int col = BackCol;

// kontrola platnosty souøadnice bodu
	if (((DWORD)x < (DWORD)Width) && ((DWORD)y < (DWORD)Height))
	{

// naètení bodu
		col = BackBuf[x + y*WidthByte];
	}

// výsledná barva
	return PalExport(col);
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní šíøky obrázku

double FGetPicWidth()
{
	CPicture pic;
	FPicture(pic);
	double n = (double)pic.Width()/ICONWIDTH;
	return n;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní výšky obrázku

double FGetPicHeight()
{
	CPicture pic;
	FPicture(pic);
	double n = (double)pic.Height()/ICONHEIGHT;
	return n;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní šíøky pole k zobrazení centrovaného textu

double FGetGraphAreaWidth()
{
	double n = (double)GraphAreaWidth/ICONWIDTH;
	return n;
}


/***************************************************************************\
*																			*
*							obsluha èasu									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// naètení aktuálního èasu

double FGetTimeCurrent()
{
// naètení aktuálního èasu
	SYSTEMTIME systime;
	::GetLocalTime(&systime);

// pøevod na souborový èas
	__int64 cas;
	::SystemTimeToFileTime(&systime, (FILETIME*)&cas);

// návrat výsledku
	return (double)cas / FILETIMEKONV;
}


/////////////////////////////////////////////////////////////////////////////
// pøevod na zvìtový èas

double FTimeUtc()
{
// naètení èasu s pøevodem na svìtový èas
	__int64 cas = FNumFileTimeUTC();

// návrat výsledku
	return (double)cas / FILETIMEKONV;
}


/////////////////////////////////////////////////////////////////////////////
// pøevod na místní èas

double FTimeLocal()
{
// naètení èasu k pøevodu
	__int64 cas = FNumFileTime();

// pøevod èísla
	__int64 cas2;
	::FileTimeToLocalFileTime((FILETIME*)&cas, (FILETIME*)&cas2);

// návrat výsledku
	return (double)cas2 / FILETIMEKONV;
}


/////////////////////////////////////////////////////////////////////////////
// syntéza èasu

double FTimeComp()
{
// naètení složek èasu
	SYSTEMTIME systime;

	int i = FIntR();
	if (i < 1601) i = 1601;
	if (i > 9999) i = 9999;		// system povoluje až 30000, ale kvùli zobrazení radìji jen 4 èíslice
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

// provedení konverze
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
// naètení roku z èasu

double FGetTimeYear() { FNumSysTime(); return SystemTime.wYear; }


/////////////////////////////////////////////////////////////////////////////
// naètení mìsíce z èasu

double FGetTimeMonth() { FNumSysTime(); return SystemTime.wMonth; }


/////////////////////////////////////////////////////////////////////////////
// naètení dne z èasu

double FGetTimeDay() { FNumSysTime(); return SystemTime.wDay; }


/////////////////////////////////////////////////////////////////////////////
// naètení dne v týdnu z èasu

double FGetTimeWeek() { FNumSysTime(); return SystemTime.wDayOfWeek; }


/////////////////////////////////////////////////////////////////////////////
// naètení hodiny z èasu

double FGetTimeHour() { FNumSysTime(); return SystemTime.wHour; }


/////////////////////////////////////////////////////////////////////////////
// naètení minuty z èasu

double FGetTimeMin() { FNumSysTime(); return SystemTime.wMinute; }


/////////////////////////////////////////////////////////////////////////////
// naètení sekundy z èasu

double FGetTimeSec() { FNumSysTime(); return SystemTime.wSecond; }


/////////////////////////////////////////////////////////////////////////////
// naètení milisekundy z èasu

double FGetTimeMSec() { FNumSysTime(); return SystemTime.wMilliseconds; }


/***************************************************************************\
*																			*
*							obsluha myši									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// naètení souøadnice X myši

double FMouseX() { return MouseX; }

/////////////////////////////////////////////////////////////////////////////
// naètení souøadnice Y myši

double FMouseY() { return MouseY; }


/***************************************************************************\
*																			*
*							obsluha dialogù									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// dialog box s návratem hodnoty

double FNumDialogBox()
{
// naètení textu nadpisu okna
	CString nadpis;
	FText(nadpis);

// naètení textu hlášení
	CString text;
	FText(text);

// èíslo ikony
	int icon = FIntR();

// typ tlaèítek
	int but = FIntR();

// implicitní tlaèítko
	int def = FIntR();

// pro nadpis se použije první øádek textu
	int crlf = nadpis.Find(13);
	if (crlf >= 0) nadpis.Delete(crlf);

// korekce povolených hodnot
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

// vypnutí celoobrazovkového módu
//	SetFullScreen(false);

#ifndef _MINI
	D3Render(true);
#endif // _MINI

// pøekreslení displeje pøed vyvoláním dialogu
	ReDisp();

// vyvolání dialogu
	IsOverlay = true;

	int result = ::MessageBox(Win[DialogParent].Wnd, text, nadpis,
		icon * 0x10 + but + (def-1)*0x100);

	IsOverlay = false;
	if (FullScreen) RePaint(&Win[0]);

	return result;
}


/***************************************************************************\
*																			*
*							obsluha souborù									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// zjištìní velikosti disku

double FGetDiskSize()
{	
// naètení informací o disku
	GetDiskSpace();

// velikost disku
	return (double)DiskSize;
}


/////////////////////////////////////////////////////////////////////////////
// zjištìní volného místa disku

double FGetDiskFree()
{	
// naètení informací o disku
	GetDiskSpace();

// volné místo disku
	return (double)DiskFree;
}


/////////////////////////////////////////////////////////////////////////////
// naètení sériového èísla disku

double FGetDiskSerial()
{
// promìnná k naètení èísla
	__int64 serial = 0;

// naètení sériového èísla disku
	::GetVolumeInformation(NULL, NULL, 0, (DWORD*)&serial, NULL, NULL, NULL, 0);

// naètený údaj (pøi chybì nebo když èíslo není zùstane = 0)
	return (double)serial;
}


/////////////////////////////////////////////////////////////////////////////
// naètení informací o vstupním souboru (false = chyba)

bool FGetFileInfo(WIN32_FIND_DATA* wfd)
{
// uzavøení souborù
	FileClose();

// kontrola zadání jména
	if (FileRead.IsNotEmpty())
	{

// nalezení souboru
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
// èas vytvoøení souboru

double FGetFileCreate()
{
// naètení informací o souboru
	WIN32_FIND_DATA wfd;
	if (FGetFileInfo(&wfd))
	{

// pøevod èasu na lokální èas
		__int64 cas;
		if (::FileTimeToLocalFileTime(&wfd.ftCreationTime, (FILETIME*)&cas))
		{

// návrat výsledku
			return (double)cas / FILETIMEKONV;
		}
	}

// chyba
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// èas ètení ze souboru

double FGetFileOpen()
{
// naètení informací o souboru
	WIN32_FIND_DATA wfd;
	if (FGetFileInfo(&wfd))
	{

// pøevod èasu na lokální èas
		__int64 cas;
		if (::FileTimeToLocalFileTime(&wfd.ftLastAccessTime, (FILETIME*)&cas))
		{

// návrat výsledku
			return (double)cas / FILETIMEKONV;
		}
	}

// chyba
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// èas zápisu do souboru

double FGetFileModi()
{
// naètení informací o souboru
	WIN32_FIND_DATA wfd;
	if (FGetFileInfo(&wfd))
	{

// pøevod èasu na lokální èas
		__int64 cas;
		if (::FileTimeToLocalFileTime(&wfd.ftLastWriteTime, (FILETIME*)&cas))
		{

// návrat výsledku
			return (double)cas / FILETIMEKONV;
		}
	}

// chyba
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// offset pro ètení ze souboru

double FGetFileReadOff() { return FileReadOff; };


/////////////////////////////////////////////////////////////////////////////
// offset pro zápis do souboru

double FGetFileWriteOff() { return FileWriteOff; };


/////////////////////////////////////////////////////////////////////////////
// zjištìní velikosti souboru

double FGetFileSize()
{
// lokální promìnné
	int size = 0;

// vyprázdìní zápisového bufferu a otevøení ètecího souboru
	if (FileWriteFlush() && FileReadOpen())
	{
// zjištìní velikosti souboru
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

// zjištìná velikost
	return size;
}


/////////////////////////////////////////////////////////////////////////////
// délka textu s pevnou délkou

double FGetFileTextNLen() { return FileTextNLen; }


/////////////////////////////////////////////////////////////////////////////
// naètení typu disku

double FFileDiskType() { return ::GetDriveType(NULL); }


/////////////////////////////////////////////////////////////////////////////
// naètení èísla ze souboru bajt se znaménkem

double FGetFileByteS() { return (char)FileReadByte(); }


/////////////////////////////////////////////////////////////////////////////
// naètení èísla ze souboru bajt bez znaménka

double FGetFileByteU() { return (BYTE)FileReadByte(); }


/////////////////////////////////////////////////////////////////////////////
// naètení èísla ze souboru slovo se znaménkem

double FGetFileWordS()
{ 
	short cislo;
	((BYTE*)&cislo)[0] = FileReadByte();
	((BYTE*)&cislo)[1] = FileReadByte();
	return cislo;
}


/////////////////////////////////////////////////////////////////////////////
// naètení èísla ze souboru slovo bez znaménka

double FGetFileWordU()
{ 
	long cislo = 0;
	((BYTE*)&cislo)[0] = FileReadByte();
	((BYTE*)&cislo)[1] = FileReadByte();
	return cislo;
}


/////////////////////////////////////////////////////////////////////////////
// naètení èísla ze souboru 3 bajty se znaménkem

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
// naètení èísla ze souboru 3 bajty bez znaménka

double FGetFile3ByteU()
{ 
	long cislo = 0;
	((BYTE*)&cislo)[0] = FileReadByte();
	((BYTE*)&cislo)[1] = FileReadByte();
	((BYTE*)&cislo)[2] = FileReadByte();
	return cislo;
}


/////////////////////////////////////////////////////////////////////////////
// naètení èísla ze souboru dvojslovo se znaménkem

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
// naètení èísla ze souboru dvojslovo bez znaménka

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
// naètení èísla ze souboru FLOAT

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
// naètení èísla ze souboru DOUBLE

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
// zjištìní délky CD

double FCDGetLength()
{
// aktualizace údajù o CD
	CDAkt();

// délka CD
	return (double)CDLength / 1000;
}


/////////////////////////////////////////////////////////////////////////////
// zjištìní poètu stop CD

double FCDGetTracks()
{
// aktualizace údajù o CD
	CDAkt();

// poèet stop CD
	return CDTracks;
}


/////////////////////////////////////////////////////////////////////////////
// zjištìní pozice CD

double FCDGetPos()
{
// aktualizace údajù o CD
	CDAkt();

// pozice CD
	return (double)CDPos / 1000;
}


/////////////////////////////////////////////////////////////////////////////
// zjištìní aktivní stopy CD

double FCDGetTrack()
{
// aktualizace údajù o CD
	CDAkt();

// aktivní stopa
	return CDTrack;
}


/////////////////////////////////////////////////////////////////////////////
// zjištìní délky aktivní stopy

double FCDGetTrackLength()
{
// aktualizace údajù o CD
	CDAkt();

// délka stopy CD
	return (double)CDTrackLength / 1000;
}


/////////////////////////////////////////////////////////////////////////////
// zjištìní pozice aktivní stopy CD

double FCDGetTrackPos()
{
// aktualizace údajù o CD
	CDAkt();

// pozice aktivní stopy CD
	return (double)CDTrackPos / 1000;
}


/***************************************************************************\
*																			*
*								obsluha hudby								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// ïélka pøehrávané hudby

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
// poèet bitù na vzorek

double FGetSoundBit()
{
	CSound snd;
	FSound(snd);
	return (double)snd.Bits();
}


/////////////////////////////////////////////////////////////////////////////
// vzorkovací kmitoèet

double FGetSoundFreq()
{
	CSound snd;
	FSound(snd);
	return (double)snd.Samples();
}

/////////////////////////////////////////////////////////////////////////////
// délka zvuku

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
// rozlišovací kód

double FGetSoundDistinguish()
{
	return (double)(WORD)SoundID;
}


/***************************************************************************\
*																			*
*								obsluha textù								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// délka textu

double FTextLen()
{
// délka textu
	int len;

// následující úsek ponechat v závorkách, aby se vyvolal
// destruktor textu døíve než se èíslo uloží do koprocesoru:
	{
// naètení textu
		CString text;
		FText(text);

// délka textu
		len = text.Length();
	}
	return len;
}


/////////////////////////////////////////////////////////////////////////////
// pøevod textu na èíslo

double FTextScan()
{
// adresa textu
	LPCTSTR txt;

// následující úsek ponechat v závorkách, aby se vyvolal
// destruktor textu døíve než se èíslo uloží do koprocesoru:
	{
// naètení textu
		CString text;
		FText(text);
		txt = text;
	}

// pøevod textu na èíslo
	return Double(txt);
}


/////////////////////////////////////////////////////////////////////////////
// nalezení textu

double FTextFind()
{
// hledaný text
	CString subtext;
	FText(subtext);

// prohledávaný text
	CString text;
	FText(text);

// nalezení textu
	int pos = text.Find(subtext);
	return pos;
}


/////////////////////////////////////////////////////////////////////////////
// abecední poøadí znaku

double FTextOrd()
{
// pomocný registr
	int result;

// následující úsek ponechat v závorkách, aby se vyvolal
// destruktor textu døíve než se èíslo uloží do koprocesoru:
	{

// naètení textu (znaku)
		CString text;
		FText(text);

// kódu znaku (není-li žádný znak, vrátí se koncový bajt 0)
#ifdef _UNICODE
		result = (WORD)text.DataData()[0];
#else
		result = (BYTE)text.DataData()[0];
#endif
	}

// návrat výsledku
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// zjištìní poètu øádkù textu

double FTextLineNum()
{
// naètení textu
	CString text;
	FText(text);
	return text.LineNum();
}


/***************************************************************************\
*																			*
*								obsluha klávesnice							*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// naètení klávesy bez èekání

double FVKey()
{
// pøíprava výsledku operace
	int result = 0;

// test, zda byla pøijata klávesa
	if (KeyBufRead != KeyBufWrite)
	{

// naètení klávesy z bufferu
		result = KeyBuf[KeyBufRead];

// zvýšení ètecí adresy
		KeyBufRead++;
		if (KeyBufRead >= KEYBUFSIZE) KeyBufRead = 0;
	}

	return result;
}


/////////////////////////////////////////////////////////////////////////////
// naètení klávesy s èekáním

double FVKeyWait()
{
// pøíprava výsledku operace
	int result = 0;

// èekání na vstup znaku z klávesnice (nebo na pøerušení)
	while ((Break == 0) && 
			(KeyBufRead == KeyBufWrite) && 
			(!ConsoleOn))
	{
#ifndef _MINI
		D3Render(true);
#endif // _MINI
		TimeWait(1);
	}

// kontrola, zda byla pøijata klávesa (zda to nebylo jen pøerušení)
	if (KeyBufRead != KeyBufWrite)
	{

// naètení klávesy z bufferu
		result = KeyBuf[KeyBufRead];

// zvýšení ètecí adresy
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
// šíøka plochy

double FGetMapWidth()
{
// naètení plochy
	CMap map;
	FMap(map);

// šíøka plochy
	return map.Width();
}

double FGetMapWidth0() { return Map[0].Width(); }


/////////////////////////////////////////////////////////////////////////////
// výška plochy

double FGetMapHeight()
{
// naètení plochy
	CMap map;
	FMap(map);

// výška plochy
	return map.Height();
}

double FGetMapHeight0() { return Map[0].Height(); }


/////////////////////////////////////////////////////////////////////////////
// naètení èísla z plochy

double _fastcall FGetMapNum(int mask, char rot)
{
// naètení plochy
	CMap map;
	FMap(map);

// naètení souøadnic
	int x = FIntR();
	int y = FIntR();

// èíslo na pozici
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
// naètení okna k zobrazení sprajtù

double FGetSpriteWinX1() { return (double)SpriteWinX1 / ICONWIDTH; }
double FGetSpriteWinY1() { return (double)SpriteWinY1 / ICONHEIGHT; }
double FGetSpriteWinX2() { return (double)SpriteWinX2 / ICONWIDTH; }
double FGetSpriteWinY2() { return (double)SpriteWinY2 / ICONHEIGHT; }


/////////////////////////////////////////////////////////////////////////////
// návratový kód

double FGetReturnCode() { return ReturnCode; }


/////////////////////////////////////////////////////////////////////////////
// spuštìní programu

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
// ID aktivního prvku

double FGetWindowID() { return WindowID; }


/////////////////////////////////////////////////////////////////////////////
// vytvoøení okna

double FWindowNew()
{
	WindowNew(WINITEM_WINDOW, WS_SYSMENU); 
	return WindowID;
}


/////////////////////////////////////////////////////////////////////////////
// vytvoøení okna

double FWindowRONew()
{
	WindowNew(WINITEM_WINDOWRO, 0); 
	return WindowID;
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení tlaèítka

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
// indikátor

CString	ProgressClass(PROGRESS_CLASS);

double FProgressNew()
{
	::InitCommonControls();

	WindowItemNew(WINITEM_PROGRESS, ProgressClass, 202, 16, 0, 0);
	return WindowID;
}

/////////////////////////////////////////////////////////////////////////////
// táhlo

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
// posuvník

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
			// není-li nic oznaèeno, vrací LB_ERR = -1
			return (double)(int)::SendMessage(item->Wnd, LB_GETCURSEL, 0, 0);

		case WINITEM_TABS:
			// není-li nic oznaèeno, vrací -1
			return (double)(int)::SendMessage(item->Wnd, TCM_GETCURSEL, 0, 0);

		case WINITEM_LISTEDIT:
		case WINITEM_COMBOBOX:
		case WINITEM_COMBOEDIT:
			// není-li nic oznaèeno, vrací CB_ERR = -1
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
// editaèní pole

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
// !!!!!!!!!!!!!!!!! Novìjší verze RichEdit pøi naètení textu vymaže UNDO buffer
// !!!!!!!!!!!!!!!!! u novìjší verze RichEdit nefunguje hledání textu
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
// naètení seznamu titulkù
	CString text;
	FText(text);

	int cols = text.LineNum();
	if (cols <= 0) cols = 1;

// pøíprava jednotlivých titulkù
	CBufText coltit;
	bool head = false;
	for (int i = 0; i < cols; i++)
	{
		coltit.New();
		coltit[i] = text.GetLine(i);
		if (coltit[i].IsNotEmpty()) head = true;
	}

// vytvoøení tabulky
	::InitCommonControls();

	WindowItemNew(WINITEM_TABLE, ListviewClass, 32*8, 128, LVS_REPORT | LVS_SINGLESEL | LVS_OWNERDRAWFIXED | LVS_SHOWSELALWAYS
		| WS_GROUP | WS_TABSTOP | (head ? LVS_NOSORTHEADER : LVS_NOCOLUMNHEADER), WS_EX_STATICEDGE);

	WINITEM* item = &Win[WindowID];
	item->HasCaption = true;

// vytvoøení sloupeèkù
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

// vytvoøení prvního øádku
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
// rozmìry okna

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
// výška fontù

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
// šíøka fontù

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
// úhel textu

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
// barva pozadí

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
// zjištìní èíselné hodnoty hry 1

double FGetGameN1()
{
	int n = (int)(((WORD*)DirectPlayGamesParam)[0] & 0x7fff) - 0x4000;
	return n;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní èíselné hodnoty hry 2

double FGetGameN2()
{
	int n = (int)(((WORD*)DirectPlayGamesParam)[1] & 0x7fff) - 0x4000;
	return n;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní èíselné hodnoty hry 3

double FGetGameN3()
{
	int n = (int)(((WORD*)DirectPlayGamesParam)[2] & 0x7fff) - 0x4000;
	return n;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní èíselné hodnoty hry 4

double FGetGameN4()
{
	int n = (int)(((WORD*)DirectPlayGamesParam)[3] & 0x7fff) - 0x4000;
	return n;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní èíselné hodnoty hry 5

double FGetGameN5()
{
	int n = (int)(((WORD*)DirectPlayGamesParam)[4] & 0x7fff) - 0x4000;
	return n;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní èíselné hodnoty hry 6

double FGetGameN6()
{
	int n = (int)(((WORD*)DirectPlayGamesParam)[5] & 0x7fff) - 0x4000;
	return n;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní èíselné hodnoty hry 7

double FGetGameN7()
{
	int n = (int)(((WORD*)DirectPlayGamesParam)[6] & 0x7fff) - 0x4000;
	return n;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní èíselné hodnoty hry 8

double FGetGameN8()
{
	int n = (int)(((WORD*)DirectPlayGamesParam)[7] & 0x7fff) - 0x4000;
	return n;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní max. poètu hráèù

double FGetPlayerMax()
{
	return DirectPlayPlayersMax;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní indexu hráèe

double FPlayerInx()
{
	return DirectPlayPlayersAktN;
}

/////////////////////////////////////////////////////////////////////////////
// pøíjem paketu

double FGetGamePacket()
{
	return DirectReceive();
}

/////////////////////////////////////////////////////////////////////////////
// naètení èísla z paketu

double FGetGameDataN()
{
	double num;
	DirectReceiveData((BYTE*)&num, 8);
	return num;
}

/////////////////////////////////////////////////////////////////////////////
// naètení bajtu z paketu

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
// zjištìní vysílacího portu UDP

double FGetUDPSendPort()
{
	int n = UDPSendAddr.sin_port;
	return ((n & 0xff) << 8) | (n >> 8);
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní velikosti pøijímacího bufferu UDP

double FGetUDPRecSize()
{
	return UDPRecBuf.len;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní pøijímacího portu UDP

double FGetUDPRecPort()
{
	int n = UDPRecAddr.sin_port;
	return ((n & 0xff) << 8) | (n >> 8);
}

/////////////////////////////////////////////////////////////////////////////
// pøíjem datového bloku UDP

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
// --------------------- konec vypnutí pro MINI verzi -------------------

/***************************************************************************\
*																			*
*							Bitové operace									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// operátor BIT-AND (Data = poèet prvkù - 1)

double FBitAnd()
{
// pøíprava poètu prvkù
	int i = ExecItem[-1].Data;

// naètení prvního èísla
	DWORD num = FIntRN();

// cyklus pøes další èísla
	for (; i > 0; i--)
	{

// operace s dalším èíslem
		num &= FIntRN();
	}
	return num;
}

/////////////////////////////////////////////////////////////////////////////
// operátor BIT-OR (Data = poèet prvkù - 1)

double FBitOr()
{
// pøíprava poètu prvkù
	int i = ExecItem[-1].Data;

// naètení prvního èísla
	DWORD num = FIntRN();

// cyklus pøes další èísla
	for (; i > 0; i--)
	{

// operace s dalším èíslem
		num |= FIntRN();
	}
	return num;
}

/////////////////////////////////////////////////////////////////////////////
// operátor BIT-XOR (Data = poèet prvkù - 1)

double FBitXor()
{
// pøíprava poètu prvkù
	int i = ExecItem[-1].Data;

// naètení prvního èísla
	DWORD num = FIntRN();

// cyklus pøes další èísla
	for (; i > 0; i--)
	{

// operace s dalším èíslem
		num ^= FIntRN();
	}
	return num;
}

/////////////////////////////////////////////////////////////////////////////
// bitový doplnìk

double FBitNot()
{ 
// naètení èísla
	DWORD num = FIntRN();
	num = ~num;
	return num;
};

/////////////////////////////////////////////////////////////////////////////
// rotace vlevo

double FBitLeft()
{ 
// naètení èísla
	DWORD num = FIntRN();

	num = (num << 1) | (num >> 31);

	return num;
};


/////////////////////////////////////////////////////////////////////////////
// rotace vpravo

double FBitRight()
{ 
// naètení èísla
	DWORD num = FIntRN();

	num = (num >> 1) | (num << 31);

	return num;
};



/////////////////////////////////////////////////////////////////////////////
// zjištìní znakové sady

double FGetCodePage() { return CodePage; }


/////////////////////////////////////////////////////////////////////////////
// zjištìní jazyku

double FGetLanguage() { return LangID; }


/***************************************************************************\
*																			*
*									Porty									*
*																			*
\***************************************************************************/

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

/////////////////////////////////////////////////////////////////////////////
// otevøení portu

double FComOpen()
{
// naètení textu
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

// nalezení volné položky
	for (id = 0; id < COMSMAX; id++)
	{
		if (Coms[id].File == INVALID_HANDLE_VALUE) break;
	}
	if (id == COMSMAX) return -1;

	COMITEM* item = Coms + id;

// otevøení zaøízení
	HANDLE file = ::CreateFile(
		text,									// jméno zaøízení
		GENERIC_READ | GENERIC_WRITE,			// požadovaný pøístup
		0,										// sdílení není povoleno
		NULL,									// nelze sdílet s podprocesy
		OPEN_EXISTING,							// zaøízení musí existovat
		FILE_ATTRIBUTE_NORMAL,					// atributy normální
		NULL);									// není šablona
	if (file == INVALID_HANDLE_VALUE) return -1;
	item->File = file;

// inicializace bufferù (pro pøijímací buffer > 1024 dìlá chyby v datech nad 1024 !!!!)
	::SetupComm(file, 1010, 64);

// hlášení událostí se nebude používat
	::SetCommMask(file, 0);

// vyprázdnìní bufferù
	::PurgeComm(file, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

// nastavení time-out
	COMMTIMEOUTS to;
	to.ReadIntervalTimeout = 100;
	to.ReadTotalTimeoutMultiplier = 100;
	to.ReadTotalTimeoutConstant = 0;
	to.WriteTotalTimeoutMultiplier = 100;
	to.WriteTotalTimeoutConstant = 0;
	::SetCommTimeouts(file, &to);

// nastavení jako aktivní port
	ComAkt = id;

// inicializace parametrù portu
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

// aktualizace parametrù, pokud byla zmìna
	if (!MemCompare(&olddcb, dcb, sizeof(DCB)))
	{
		SetComDCB();
	}

	return id;
}

/////////////////////////////////////////////////////////////////////////////
// aktivní port

double FGetComID()
{
	return ComAkt;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní pøenosové rychlosti portu

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
// zjištìní poètu bitù portu

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
// zjištìní parity

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
// naètení bajtu

double FGetComNum()
{
	return (double)ComReceive();
}

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------


/***************************************************************************\
*																			*
*								obsluha mixeru								*
*																			*
\***************************************************************************/

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

/////////////////////////////////////////////////////////////////////////////
// zjištìní typu signálu

double FGetLineType()
{
// inicializace mixeru
	if (!MixDevInit() || ((DWORD)MixLineA >= (DWORD)MixLineN)) return -1;

// zjištìní typu signálu
	return MixLine[MixLineA].Type;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní typu ovládacího prvku

double FGetCtrlType()
{
// inicializace mixeru
	if (!MixDevInit() || ((DWORD)MixCtrlA >= (DWORD)MixCtrlN)) return -1;

// zjištìní typu ovládacího prvku
	return MixCtrl[MixCtrlA].Type;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní hodnoty ovládacího prvku

double FGetCtrlVal()
{
	return GetMixCtrlVal();
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní poètu kanálù prvku

double FGetMixChannel()
{
// inicializace mixeru
	if (!MixDevInit() || ((DWORD)MixCtrlA >= (DWORD)MixCtrlN)) return 0;

// zjištìní poètu kanálù
	return MixCtrl[MixCtrlA].Channels;
}

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------

/***************************************************************************\
*																			*
*								obsluha Direct3D							*
*																			*
\***************************************************************************/

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

/////////////////////////////////////////////////////////////////////////////
// zjištìní módu odstraòováni ploch objektu Direct3D

double FGetD3DOCulling()
{
	if (IsValidID())
	{
		return GetID()->Culling;
	}
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení zdi (je voláno i pro vytvoøení 2D objektu)

// tabulka se používá i pøi zobrazení textury pozadí scény v OpenGL
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

// tabulka se používá i pøi zobrazení textury pozadí scény v OpenGL
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
// vytvoøení statického 2D objektu

D3DVECTOR static2D_vertices[16] = {
// 1. stìna
	-0.5,  0.5,    0,		// 0
     0.5,  0.5,    0,		// 1
    -0.5, -0.5,    0,		// 2
     0.5, -0.5,    0,		// 3

// 2. stìna
	   0,  0.5, -0.5,		// 4
       0,  0.5,  0.5,		// 5
       0, -0.5, -0.5,		// 6
       0, -0.5,  0.5,		// 7

// 3. stìna
	 0.5,  0.5,    0,		// 8
    -0.5,  0.5,    0,		// 9
     0.5, -0.5,    0,		// 10
    -0.5, -0.5,    0,		// 11

// 4. stìna
	   0,  0.5,  0.5,		// 12
       0,  0.5, -0.5,		// 13
       0, -0.5,  0.5,		// 14
       0, -0.5, -0.5		// 15
};

int static2D_faces[24] = {
// 1. stìna
	0, 1, 2,
	2, 1, 3,

// 2. stìna
	4, 5, 6,
	6, 5, 7,

// 3. stìna
	8, 9, 10,
	10, 9, 11,

// 4. stìna
    12, 13, 14,
	14, 13, 15
};

D3DVECTOR static2D_normals[16] = {
// 1. stìna
	0, 0, -1,
	0, 0, -1,
	0, 0, -1,
	0, 0, -1,

// 2. stìna
	1, 0,  0,
	1, 0,  0,
	1, 0,  0,
	1, 0,  0,

// 3. stìna
	0, 0,  1,
	0, 0,  1,
	0, 0,  1,
	0, 0,  1,

// 4. stìna
	-1, 0, 0,
	-1, 0, 0,
	-1, 0, 0,
	-1, 0, 0
};

float static2D_text[2*16] = {
// 1. stìna
	0, 0,
	1, 0,
	0, 1,
	1, 1,

// 2. stìna
	0, 0,
	1, 0,
	0, 1,
	1, 1,

// 3. stìna
	0, 0,
	1, 0,
	0, 1,
	1, 1,

// 4. stìna
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
// vytvoøení trojúhelníku

D3DVECTOR triangle_vertices[3] = {
	-0.5, -0.5, 0,			// 0 (vlevo dole)
     0,    0.5, 0,			// 1 (uprostøed nahoøe)
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
// vytvoøení krychle

//    3----7
//    |\    \ 
//    | 2----6
//    1 |  5 |
//     \|    |
//      0----4

D3DVECTOR cube_vertices[24] = {

// horní stìna (musí být jako první stìna!)
    -0.5,  0.5,  0.5,	// ( 0) 3 horní stìny
     0.5,  0.5,  0.5,	// ( 1) 7 horní stìny
    -0.5,  0.5, -0.5,	// ( 2) 2 horní stìny
     0.5,  0.5, -0.5,	// ( 3) 6 horní stìny

// pøední stìna
    -0.5,  0.5, -0.5,	// ( 4) 2 pøední stìny
     0.5,  0.5, -0.5,	// ( 5) 6 pøední stìny
	-0.5, -0.5, -0.5,	// ( 6) 0 pøední stìny
     0.5, -0.5, -0.5,	// ( 7) 4 pøední stìny

// zadní stìna
     0.5,  0.5,  0.5,	// ( 8) 7 zadní stìny
    -0.5,  0.5,  0.5,	// ( 9) 3 zadní stìny
     0.5, -0.5,  0.5,	// (10) 5 zadní stìny
    -0.5, -0.5,  0.5,	// (11) 1 zadní stìny

// levá stìna
    -0.5,  0.5,  0.5,	// (12) 3 levé stìny
    -0.5,  0.5, -0.5,	// (13) 2 levé stìny
    -0.5, -0.5,  0.5,	// (14) 1 levé stìny
	-0.5, -0.5, -0.5,	// (15) 0 levé stìny

// pravá stìna
     0.5,  0.5, -0.5,	// (16) 6 pravé stìny
     0.5,  0.5,  0.5,	// (17) 7 pravé stìny
     0.5, -0.5, -0.5,	// (18) 4 pravé stìny
     0.5, -0.5,  0.5,	// (19) 5 pravé stìny

// dolní stìna (musí být jako poslední stìna!)
	-0.5, -0.5, -0.5,	// (20) 0 dolní stìny
     0.5, -0.5, -0.5,	// (21) 4 dolní stìny
    -0.5, -0.5,  0.5,	// (22) 1 dolní stìny
     0.5, -0.5,  0.5,	// (23) 5 dolní stìny
};

//    3----7
//    |\    \ 
//    | 2----6
//    1 |  5 |
//     \|    |
//      0----4

int cube_faces[36] = {
	0,	1,	2,			// horní stìna (3-7-2 a 2-7-6)
	2,	1,	3,

	4,	5,	6,			// pøední stìna (2-6-0 a 0-6-4)
	6,  5,	7,

	8,  9,  10,			// zadní stìna (7-3-5 a 5-3-1)
	10, 9,  11,

	12, 13, 14,			// levá stìna (3-2-1 a 1-2-0)
	14, 13, 15,

	16, 17, 18,			// pravá stìna (6-7-4 a 4-7-5)
	18, 17, 19,

	20, 21, 22,			// dolní stìna (0-4-1 a 1-4-5)
	22, 21, 23,
};

//    3----7
//    |\    \ 
//    | 2----6
//    1 |  5 |
//     \|    |
//      0----4

D3DVECTOR cube_normals[24] = {
	 0,  1,  0,		// 3 horní stìny
	 0,  1,  0,		// 7 horní stìny
	 0,  1,  0,		// 2 horní stìny
	 0,  1,  0,		// 6 horní stìny

	 0,  0, -1,		// 2 pøední stìny
	 0,  0, -1,		// 6 pøední stìny
	 0,  0, -1,		// 0 pøední stìny
	 0,  0, -1,		// 4 pøední stìny

	 0,  0,  1,		// 7 zadní stìny
	 0,  0,  1,		// 3 zadní stìny
	 0,  0,  1,		// 5 zadní stìny
	 0,  0,  1,		// 1 zadní stìny

	-1,  0,  0,		// 3 levé stìny
	-1,  0,  0,		// 2 levé stìny
	-1,  0,  0,		// 1 levé stìny
	-1,  0,  0,		// 0 levé stìny

	 1,  0,  0,		// 6 pravé stìny
	 1,  0,  0,		// 7 pravé stìny
	 1,  0,  0,		// 4 pravé stìny
	 1,  0,  0,		// 5 pravé stìny

	 0, -1,  0,		// 0 dolní stìny
	 0, -1,  0,		// 4 dolní stìny
	 0, -1,  0,		// 1 dolní stìny
	 0, -1,  0,		// 5 dolní stìny
};

// Mapování èástí textury:
//     zadní     levá      dolní
//     pøední    pravá     horní

float cube_text[2*24] = {
	(float)2/3,		0.5,			// 3 horní stìny
	1,				0.5,			// 7 horní stìny
	(float)2/3,		1,				// 2 horní stìny
	1,				1,				// 6 horní stìny

	0,				0.5,			// 2 pøední stìny
	(float)1/3,		0.5,			// 6 pøední stìny
	0,				1,				// 0 pøední stìny
	(float)1/3,		1,				// 4 pøední stìny

	0,				0,				// 7 zadní stìny
	(float)1/3,		0,				// 3 zadní stìny
	0,				0.5,			// 5 zadní stìny
	(float)1/3,		0.5,			// 1 zadní stìny

	(float)1/3,		0,				// 3 levé stìny
	(float)2/3,		0,				// 2 levé stìny
	(float)1/3,		0.5,			// 1 levé stìny
	(float)2/3,		0.5,			// 0 levé stìny

	(float)1/3,		0.5,			// 6 pravé stìny
	(float)2/3,		0.5,			// 7 pravé stìny
	(float)1/3,		1,				// 4 pravé stìny
	(float)2/3,		1,				// 5 pravé stìny

	(float)2/3,		0,				// 0 dolní stìny
	1,				0,				// 4 dolní stìny
	(float)2/3,		0.5,			// 1 dolní stìny
	1,				0.5,			// 5 dolní stìny
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
// generátor rotaèních objektù - shora dolù (závìrem zruší globální buffery)

// popisovaè øady
typedef struct GENROWITEM_
{
	double	dy;					// rozdíl souøadnic Y k další øadì
	double	dr;					// rozdíl polomìrù R k další øadì
	double	length;				// délka spojnice k další øadì
	double	angle;				// úhel spojnice k další øadì
	double	norm1;				// úhel normály - první øada
	double	norm2;				// úhel normály - druhá øada
	int		vn;					// poèet vrcholù v jedné øadì (bez zdvojnásobení)
	bool	r0;					// pøíznak nulového polomìru
	bool	smooth;				// pøíznak zjemnìní normály - spoleèná normála pro obì strany
	bool	close;				// objekt je uzavøený (konec a poèátek jsou totožné)
	bool	prev;				// pøíznak, že jsou trojúhelníky zpìt (nahoru)
	bool	next;				// pøíznak, že jsou trojúhelníky vpøed (dolù)
	bool	use2;				// pøíznak použití 2 øad vrcholù
} GENROWITEM;

int			genRows;			// poèet øad objektu
double*		genY = NULL;		// buffer souøadnic Y (shora dolù)
double*		genR = NULL;		// buffer polomìrù øad

// parametr sm = zjemòovat vždy (=koule, toroid)

void GenRot(bool sm)
{

// pro malý poèet øad se vytvoøí náhradní prázdná skupina
	if (genRows <= 1)
	{
		MemFree(genY);
		MemFree(genR);

		D3D_ID = D3DF_New();
		return;
	}

// pøíprava promìnných
	int level = D3DLevel;			// poèet dìlení (polovina)
	int level2 = level*2;			// poèet dìlení na celý obvod
	double alfa = pi / level;		// úhel na jeden díl
	double alfa2 = alfa/2;			// polovièní úhel na jeden díl
	int i,j;

// pøíprava bufferu popisovaèe øad
	GENROWITEM* gr = (GENROWITEM*)MemGet(genRows * sizeof(GENROWITEM));
	GENROWITEM* item = gr;									// ukazatel položek popisovaèe øady

	double delka = 0;										// celková délka spojnic
	int vN = 0;												// poèet vrcholù celkem
	int fN = 0;												// poèet plošek celkem

// cyklus pøes všechny øady
	bool firstrow = true;									// pøíznak, že to je první øada

	for (i = 0; i < genRows; i++)
	{

// pøíznak poslední øady
		bool lastrow = (i == (genRows - 1));

// pøíznak nulového polomìru
		bool r00 = (genR[i] == 0);
		item->r0 = r00;

// rozdíl souøadnic Y a polomìrù R k další øadì
		double dy0 = 0;
		double dr0 = 0;

		if (!lastrow)
		{
			dy0 = genY[i + 1] - genY[i];
			dr0 = genR[i + 1] - genR[i];
		}

		item->dy = dy0;
		item->dr = dr0;

// délka spojnice k další øadì
		double d0 = sqrt(dy0*dy0 + dr0*dr0);
		item->length = d0;
		delka += d0;
		
// úhel spojnice k další øadì
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
				 
// rozdíl úhlu s pøedešlou øadou (pro první a poslední øadu se poèítá úhel sám se sebou)
// (první a poslední øada mùže splývat, je-li uzavøený objekt - napø. torus)
// (pro zjednodušení pøedpokládáme, že uzavøený objekt má konce vždy zjemnìné,
//  protože na zaèátku neznáme smìrník konce)
		bool close = false;					// pøíznak uzavøeného objektu

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

// porovnání úhlu, zda bude zjemnìní (zvláštní korekce se provádí pro kouli se složitostí 2)
		bool smooth0;
		if (level == 2)
		{
			if ((genRows == 3) &&
				(genR[0] == 0) && 
				(genR[2] == 0))
			{
				smooth0 = true;				// zvláštní pøípad - koule se složitostí 2 (úhel 90 stupòù)
			}
			else
			{
				smooth0 = (da < 1.4);		// tj. úhel asi 80 stupòù
			}
		}
		else
		{
			if (level == 3)
			{
				smooth0 = (da < 1.1);		// tj. úhel asi 64 stupòù, aby vyhovìla koule se složitostí 3
			}
			else
			{
				smooth0 = (da < 0.8);		// tj. úhel asi 45.8 stupòù, aby vyhovìla koule se složitostí 4
			}
		}
		if (close || sm) smooth0 = true;		// pro uzavøený objekt (nebo nucené zjemnìní) zjemníme vždy

		item->smooth = smooth0;
		item->close = close;

// pøíznak, že jsou trojúhelníky zpìt (nahoru)
		bool prev0 = (!r00 && !firstrow);
		item->prev = prev0;
		if (prev0) fN += level2;

// pøíznak, že jsou trojúhelníky vpøed (dolù)
		bool next0 = (!r00 && !lastrow);
		item->next = next0;
		if (next0) fN += level2;

// pøíznak použití 2 øad vrcholù
		bool use20 = (prev0 && next0 && !smooth0);
		item->use2 = use20;

// poèet vrcholù v jedné øadì
		int vn00 = level2;
		if (!r00) vn00++;
		item->vn = vn00;
		if (use20) vn00 = vn00 * 2;
		vN += vn00;

// výpoèet úhlu normál
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

// pøíprava pro další øadu
		firstrow = false;
		item++;
	}

// kontrola platnosti bodù - vytvoøí se náhradní prázdná skupina
	if (delka == 0)
	{
		MemFree(gr);
		MemFree(genY);
		MemFree(genR);

		D3D_ID = D3DF_New();
		return;
	}

// vytvoøení výstupních bufferù
	D3DVECTOR* v = (D3DVECTOR*)MemGet(vN * sizeof(D3DVECTOR)); // buffer vektorù vrcholù
	D3DVECTOR* n = (D3DVECTOR*)MemGet(vN * sizeof(D3DVECTOR)); // buffer vektorù normál
	float* t = (float*)MemGet(vN * 2 * sizeof(float));		// buffer souøadnic textur
	int* f = (int*)MemGet(fN * 3 * sizeof(int));			// buffer plošek

// promìnné ke generování
	double even = false;									// pøíznak sudé øady

	D3DVECTOR* vp = v;										// ukládací adresa vektorù vrcholù
	D3DVECTOR* np = n;										// ukládaví adresa vektorù normál
	float* tp = t;											// ukládací adresa souøadnic textur
	int* fp = f;											// ukládací adresa plošek

	int inx = 0;											// ukazatel indexu bodu
	double del = 0;											// ukazatel souøadnice textury
	delka = 1/delka;										// délka jako násobitel (pro zrychlení výpoètu)

// cyklus pøes všechny øady bodù
	item = gr;												// ukazatel položek popisovaèe øady

	for (i = 0; i < genRows; i++)
	{

// pøíprava promìnných
		double r = genR[i];									// polomìr øady
		double y = genY[i];									// souøadnice Y øady

		int vn = item->vn;									// poèet vrcholù v øadì

		bool use2 = item->use2;								// vytvoøí se 2 øady vrcholù

		float nn1 = (float)sin(item->norm1);				// normála první øady vrcholù (složka y)
		float nn2 = nn1;
		double rr1 = cos(item->norm1);
		double rr2 = rr1;

		if (use2)
		{
			nn2 = (float)sin(item->norm2);					// normála druhé øady vrcholù (složka y)
			rr2 = cos(item->norm2);
		}

		double gama = 0;									// ukazatel smìru otoèení bodu
		if (even)
		{
			gama = alfa2;									// pro sudou øadu odsazení
		}
		else
		{
			if (item->r0) gama = alfa;						// první bod se nevytváøí
		}

// vygenerování vrcholù
		for (j = vn; j > 0; j--)
		{
			double cosgama = cos(gama);
			double singama = sin(gama);

// souøadnice vrcholu
			vp->x = (float)(-r*singama);
			vp->y = (float)y;
			vp->z = (float)(r*cosgama);

// normála vrcholu
			np->x = (float)(-rr1*singama);
			np->y = nn1;
			np->z = (float)(rr1*cosgama);

// textura vrcholu
			*tp = (float)(gama/pi2);
			tp++;
			*tp = (float)del;

// vytvoøení druhého vrcholu
			if (use2)
			{
				vp[vn] = vp[0];

				np[vn].x = (float)(-rr2*singama);
				np[vn].y = nn2;
				np[vn].z = (float)(rr2*cosgama);

				tp[2*vn-1] = tp[-1];
				tp[2*vn] = tp[0];
			}

// zvýšení ukazatelù
			vp++;
			np++;
			tp++;
			gama += alfa;
		}

// pøeskoèení druhé øady vrcholù
		if (use2)
		{
			vp += vn;
			np += vn;
			tp += 2*vn;
		}

// vygenerování horní øady trojúhelníkù
		if (item->prev)
		{
			int inx2 = inx;

			for (j = 0; j < level2; j++)
			{

			// levý bod
				*fp = inx2;
				fp++;

			// horní bod
				int k = inx2 - item[-1].vn;
				if (even && !item[-1].r0) k = k + 1;
				*fp = k;
				fp++;

			// pravý bod
				*fp = inx2 + 1;
				fp++;

				inx2++;
			}
		}


// vygenerování dolní øady trojúhelníkù
		if (item->next)
		{
			int inx2 = inx;
			if (use2) inx2 = inx + vn;

			for (j = 0; j < level2; j++)
			{
			// levý bod
				*fp = inx2;
				fp++;

			// pravý bod
				*fp = inx2 + 1;
				fp++;

			// dolní bod
				int k = inx2 + vn;
				if (even && !item[1].r0) k = k + 1;
				*fp = k;
				fp++;

				inx2++;
			}
		}


// zvýšení ukazatele délky (pro textury)
		del += item->length*delka;

// zvýšení ukazatele indexu vrcholu
		inx = inx + vn;							// zvýšení ukazatele indexu položek
		if (use2) inx = inx + vn;

// pøíprava pro další øadu
		even = !even;							// zmìna pøíznaku sudé øady
		item++;
	}

	ASSERT(inx == vN);
	ASSERT(fp == (f + 3*fN));

// vytvoøení objektu
	int index = D3DF_CreateMesh(vN, v, fN, f);
	D3DF_SetNormal(index, n);
	D3DF_SetTextUV(index, t, 0);
	D3D_ID = index;
	D3DF_SetAddress(index, D3DTADDRESS_WRAP, D3DTADDRESS_CLAMP, 0);

// korekce pro pravoruký souøadný systém
	D3DF_KorRightHand();

// zrušení bufferù
	MemFree(v);
	MemFree(n);
	MemFree(f);
	MemFree(t);
	MemFree(gr);
	MemFree(genY);
	MemFree(genR);
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení kruhu

double FD3DCircle()
{
	int rows = (D3DLevel/16) + 2;		// poèet pásù

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

// pøevrácení kruhu, aby smìøoval "-z"
// (zde se pøedpokládá, že buffery ještì nebyly vytvoøeny!!!)
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
// vytvoøení koule

double FD3DSphere()
{
	int rows = D3DLevel + 1;		// poèet øad bodù vnitøního pásu
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
// vytvoøení polokoule

double FD3DHemisphere()
{
	int rows = (D3DLevel+1)/2 + 1;		// poèet øad bodù vnitøního pásu
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
// vytvoøení torusu

double FD3DTorus()
{
	double r = FNum()/2;			// menší polomìr

	if (r < 0.0001)				// minimální polomìr
	{
		return FD3DEmpty();			// vytvoøí se jen skupina
	}

	int rows = 2*D3DLevel + 1;			// poèet generovaných bodù
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
// vytvoøení válce

double FD3DCylinder()
{
	int rows = D3DLevel/8 + 2;			// poèet øad bodù vnitøního pásu
	int extra = (D3DLevel/20);			// pøídavné øádky nahoøe a dole

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
// vytvoøení kužele

double FD3DCone()
{
	int rows = D3DLevel/8 + 2;			// poèet øad bodù vnitøního pásu
	int extra = (D3DLevel/20);			// pøídavné øádky dole

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
// vytvoøení komolého kužele

double FD3DCone2()
{
	double r = FNum()/2;			// menší polomìr

	if (r < 0.001)					// minimální polomìr
	{
		return FD3DCone();
	}

	int rows = D3DLevel/8 + 2;			// poèet øad bodù vnitøního pásu
	int extra1 = (Round(2*r*D3DLevel)/20);	// pøídavné øádky nahoøe
	int extra2 = (D3DLevel/20);			// pøídavné øádky dole

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
// vytvoøení svìtla

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
// klonování

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
// naètení objektu ze souboru

double FD3DFile()
{
// naètení jména souboru
	CString text;
	FText(text);

// úprava jména souboru
	text.TrimLeft();
	text.TrimRight();
	text.PathName(0);

	if (text.IsEmpty())
	{
READERROR:
		FileError = true;
		return FD3DEmpty();
	}

// otevøení souboru
	HANDLE file = ::CreateFile(text, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (FileReadHandle == INVALID_HANDLE_VALUE) goto READERROR;

// zjištìní velikosti souboru
	int size = (int)::GetFileSize(file, NULL);

	if (size <= 10)
	{	
READERROR2:
		::CloseHandle(file);
		goto READERROR;
	}

// vytvoøení datového bufferu
	BYTE* data = (BYTE*)MemGet(size);

// naètení souboru
	DWORD read = 0;

	if ((::ReadFile(file, (void*)data, size, &read, NULL) == FALSE) ||
		((int)read != size))
	{
		MemFree(data);
		goto READERROR2;
	}
	::CloseHandle(file);

// pøíprava cesty do adresáøe souboru
	text.Delete(text.RevFind('\\'));

// vytvoøení objektu
	ReadMesh(data, size, text);

// zrušení datového bufferu
	MemFree(data);

	return D3D_ID;
}

/////////////////////////////////////////////////////////////////////////////
// naètení objektu z textu

double FD3DText()
{
	CString text;
	FText(text);

	CString path;
	ReadMesh((BYTE*)(LPCSTR)text, text.Length(), path);

	return D3D_ID;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní ID aktivního objektu

double FGetD3DObjectID()
{
	return D3D_ID;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní poøadí rotací objektù

double FGetD3DOrder()
{
	if (IsValidID()) return GetID()->Order;
	return D3DFORDER_XYZ;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní mìøítka ve smìru X

double FGetD3DScaleX()
{
	if (IsValidID()) return GetID()->ScaleX;
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní mìøítka ve smìru Y

double FGetD3DScaleY()
{
	if (IsValidID()) return GetID()->ScaleY;
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní mìøítka ve smìru Z

double FGetD3DScaleZ()
{
	if (IsValidID()) return GetID()->ScaleZ;
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní rotace podle osy X

double FGetD3DRotateX()
{
	if (IsValidID()) return GetID()->RotateX;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní rotace podle osy Y

double FGetD3DRotateY()
{
	if (IsValidID()) return GetID()->RotateY;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní rotace podle osy Z

double FGetD3DRotateZ()
{
	if (IsValidID()) return GetID()->RotateZ;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní posunu ve smìru X

double FGetD3DTransX()
{
	if (IsValidID()) return GetID()->TransX;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní posunu ve smìru Y

double FGetD3DTransY()
{
	if (IsValidID()) return GetID()->TransY;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní posunu ve smìru Z

double FGetD3DTransZ()
{
	if (IsValidID()) return GetID()->TransZ;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní barvy objektu (-1=vnitøní objektu, -2=z rodièe, jinak urèená)

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
// zjištìní barvy svítivosti

double FGetD3DEmissive()
{
	if (IsValidID())
	{
		return GetID()->Emissive;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní odrazu rozptýleného svìtla

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
// zjištìní matnosti

double FGetD3DPower()
{
	if (IsValidID())
	{
		return GetID()->Power;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní barvy odlesku

double FGetD3DSpecular()
{
	if (IsValidID())
	{
		return GetID()->Specular;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní barvy svìtla

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
// zjištìní typu svìtla

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
// zjištìní úhlu svìtelného kužele

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
// zjištìní úhlu polostínu svìtla

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
// zjištìní dosahu svìtla

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
// zjištìní intenzity svìtla

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
// zjištìní lineárního útlumu svìtla

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
// zjištìní kvadratického útlumu svìtla

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
// zjištìní projekce

double FGetD3DProjection()
{
	return D3DProjection;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní barvy pozadí scény

double FGetD3DSceneColor()
{
	if (D3DBackCol == MAXDWORD) return -1;
	if (D3DBackCol == (MAXDWORD-1)) return -2;
	return D3DBackCol;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní ambient svìtla

double FGetD3DAmbient()
{	
	return D3DAmbientCol;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní barvy mlhy

double FGetD3DFogColor()
{
	if (FogColor == MAXDWORD) return -1;
	return FogColor;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní módy mlhy

double FGetD3DFogMode()
{
	int n = FogType;
	if (FogKorig) n += 4;
	return n;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní zaèátku mlhy

double FGetD3DFogStart()
{
	return FogStart;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní konce mlhy

double FGetD3DFogEnd()
{
	return FogEnd;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní hustotu mlhy

double FGetD3DFogDensity()
{
	return FogDens;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní projekèní roviny

double FGetD3DViewFront()
{
	return D3DFrontClip;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní vzdálenosti dohledu

double FGetD3DViewBack()
{
	return D3DBackClip;
}

/////////////////////////////////////////////////////////////////////////////
// vyhledání objektu

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
// vytvoøení 2D objektu

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
// vytvoøení prázdného objektu

double FD3DEmpty()
{
	D3D_ID = D3DF_New();
	return D3D_ID;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní složitosti objektù

double FGetD3DSplits()
{
	return D3DLevel;
}


/////////////////////////////////////////////////////////////////////////////
// zjištìní ovladaèe

double FGetD3DDriver()
{
	return D3DDevAkt;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní rozhraní

double FGetD3DInterface()
{
	return D3DIntAkt;
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení rotaèního objektu

double FD3DRotary()
{
// naètení textu definice
	CString text;
	FText(text);

#define MAX_ROWS 10000

// naètení definice bodù
	int lin = 0;					// ukazatel øádku textu
	CString line;					// jeden øádek textu
	genRows = 0;					// èítaè øádkù s body
	genY = (double*)MemGet(MAX_ROWS * sizeof(double));	// buffer souøadnice Y
	genR = (double*)MemGet(MAX_ROWS * sizeof(double));	// buffer polomìru bodu

	for (int i = text.LineNum(); i > 0; i--)
	{

// naètení jednoho øádku textu
		line = text.GetLine(lin);		// jeden øádek z textu
		lin++;

		line.TrimLeft();
		line.TrimRight();
		if (line.IsNotEmpty())
		{

// naètení souøadnice Y
			genY[genRows] = Double(line);

// naètení polomìru bodu
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

// zvýšení èítaèe øádkù bodù
			genRows++;
			if (genRows == MAX_ROWS) break;
		}
	}

// vytvoøení objektu
	GenRot(false);

	return D3D_ID;
}

/////////////////////////////////////////////////////////////////////////////
// generátor terénu (zruší globální buffery)

//#define MAXVERT 0xf000 // MaxVertexNum

int		terWidth;				// šíøka terénu (políèek)
int		terHeight;				// výška terénu (políèek)
float*	terMap;					// mapa terénu (width*height hodnot 0 až 1),
								// poøadí zleva doprava a zdola nahoru,
								// (levý sloupec a poslední øada se zduplikují)

void GenTerrain()
{
	int i,j;

// omezení velikosti terénu
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

// vytvoøení bufferu výškové mapy (s duplikací okrajù)
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

// vytvoøení bufferu vektorù vrcholù (od dolní øady nahoru, zleva doprava, každé políèko má 4 vrcholy)
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

		// levý dolní roh
			v->x = (float)x;
			v->y = ter[0];
			v->z = (float)z;
			v++;

		// levý horní roh
			v->x = (float)x;
			v->y = ter[terWidth+1];
			v->z = (float)z2;
			v++;

			x += dx;

		// pravý horní roh
			v->x = (float)x;
			v->y = ter[terWidth+2];
			v->z = (float)z2;
			v++;

		// pravý dolní roh
			v->x = (float)x;
			v->y = ter[1];
			v->z = (float)z;
			v++;

			ter++;
		}

		ter++;
		z = z2;
	}

// vytvoøení bufferu normál (stejná organizace jako vektor vrcholù)
	D3DVECTOR* norm = (D3DVECTOR*)MemGet(vertN * sizeof(D3DVECTOR));
	D3DVECTOR* n = norm;
	v = vert;

// Normála je násobek vektorù: A x B = {Ay*Bz-Az*By,Az*Bx-Ax*Bz,Ax*By-Ay*Bx)
// pro A={dx,dxn,0} a B={0,dzn,dz} je N={dz*dxn,-dz*dx,dzn*dx}
// Vektory nejsou normalizované - provede se pøi jejich uložení.

	float dy = (float)(dz * dx);

	for (i = terHeight; i > 0; i--)
	{
		for (j = terWidth; j > 0; j--)
		{
			double dx1 = (v[0].y - v[3].y);		// rozdíl dole
			double dx2 = (v[1].y - v[2].y);		// rozdíl nahoøe

			double dz1 = (v[0].y - v[1].y);		// rozdíl vlevo
			double dz2 = (v[3].y - v[2].y);		// rozdíl vpravo

		// levý dolní roh
			n->x = (float)(dz*dx1);
			n->y = dy;
			n->z = (float)(dz1*dx);
			n++;

		// levý horní roh
			n->x = (float)(dz*dx2);
			n->y = dy;
			n->z = (float)(dz1*dx);
			n++;

		// pravý horní roh
			n->x = (float)(dz*dx2);
			n->y = dy;
			n->z = (float)(dz2*dx);
			n++;

		// pravý dolní roh
			n->x = (float)(dz*dx1);
			n->y = dy;
			n->z = (float)(dz2*dx);
			n++;

			v += 4;
		}
	}

// vyhlazení normál
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

// vytvoøení bufferu implicitního mapování textury
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
		// levý dolní roh
			*t = u;
			t++;
			*t = v1;
			t++;

		// levý horní roh
			*t = u;
			t++;
			*t = v2;
			t++;

			u += 0.25;

		// pravý horní roh
			*t = u;
			t++;
			*t = v2;
			t++;

		// pravý dolní roh
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

// vytvoøení plošek (zleva doprava, zdola nahoru)
// orientace rohù políèka:
//		1    2
//      0    3

	int faceN = terHeight*terWidth*2;
	int* face = (int*)MemGet(faceN * 3 *sizeof(int));

	int* f = face;
	int inx = 0;

	for (i = terHeight*terWidth; i > 0; i--)
	{
	// 1. trojúhleník (vrcholy 0-1-2)
		*f = inx;
		f++;
		*f = inx + 1;
		f++;
		*f = inx + 2;
		f++;

	// 2. trojúhelník (vrcholy 0-2-3)
		*f = inx;
		f++;
		*f = inx + 2;
		f++;
		*f = inx + 3;
		f++;

		inx += 4;
	}

// vytvoøení objektu
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

// zrušení bufferù
	MemFree(vert);
	MemFree(face);
	MemFree(norm);
	MemFree(text);
}

/////////////////////////////////////////////////////////////////////////////
// popisovaè souseda ikony

typedef struct _ICONNEIGH
{
	int		icon;			// index ikony
	int		smer;			// offset souseda (0 až 7)
	int		neigh;			// index souseda
	int		counter;		// èítaè výskytu
} ICONNEIGH;

// pomocná tabulka smìrù X
const int smerX[8] =
{
	1,						// 0: doprava
	1,						// 1: doprava o øádek zpìt (dolù)
	0,						// 2: o øádek zpìt
	-1,						// 3: doleva o øádek zpìt (dolù)
	-1,						// 4: doleva
	-1,						// 5: doleva o øádek vpøed (nahoru)
	0,						// 6: o øádek vpøed
	1						// 7: doprava o øádek vpøed (nahoru)
};

// pomocná tabulka smìrù Y
const int smerY[8] =
{
	0,						// 0: doprava
	-1,						// 1: doprava o øádek zpìt (dolù)
	-1,						// 2: o øádek zpìt
	-1,						// 3: doleva o øádek zpìt (dolù)
	0,						// 4: doleva
	1,						// 5: doleva o øádek vpøed (nahoru)
	1,						// 6: o øádek vpøed
	1						// 7: doprava o øádek vpøed (nahoru)
};

#define ICONBORD	16							// šíøka pøídavného pásu kolem ikon (v bodech)
#define ICONSIZEB	(ICONWIDTH + 2*ICONBORD)	// šíøka a výška ikony s pøídavným pásem

/////////////////////////////////////////////////////////////////////////////
// vytvoøení terénu z plochy

double FD3DTerrainMap()
{

// naètení plochy
	CMap map;
	FMap(map);

// pøíprava parametrù plochy
	terWidth = map.Width();
	terHeight = map.Height();
	int size = terWidth * terHeight;
	int i,j;

// pøíprava bufferu pro zmapování ikon a výšek
	CIcon* ico = (CIcon*)MemGet(size * sizeof(CIcon));
	int* icomap = (int*)MemGet(size * sizeof(int));
	int icons = 0;

// zmapování ikon
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

// vyhledání èetností sousedù ikon
	ICONNEIGH* iconneigh = (ICONNEIGH*)MemGet(8 * size * sizeof(ICONNEIGH));
	int neighN = 0;

	int x = 0;
	int y = 0;

	for (i = 0; i < size; i++)
	{
		int icoinx = icomap[i];			// støedová ikona
		
		for (j = 0; j < 8; j++)			// cyklus pøes všechny sousedy
		{
			int x2 = x + smerX[j];		// souøadnice X souseda
			if (x2 < 0) x2 = terWidth - 1;
			if (x2 >= terWidth) x2 = 0;

			int y2 = y + smerY[j];		// souøadnice Y souseda
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

// vyhledání nejèastìjších sousedù (tabulka je po 8 položkách indexù sousedù)
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

			ASSERT(count > 0);		// (to by byla chyba v této proceduøe)
			neighP++;
		}
	}

// pøíprava textury
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

// dekódování ikon textury (od horní øady dolù)
	int radek = 1;
	int pozice = 0;

	for (i = 0; i < icons; i++)
	{
		BYTE* dst0 = picdata + (icopicWH - radek*ICONSIZEB + ICONBORD)*icopicWH + pozice*ICONSIZEB + ICONBORD;

		ico[i].DeComp();
		BYTE* src = ico[i].DataData();

	// pøenesení ikony
		BYTE* dst = dst0; 
		for (j = ICONHEIGHT; j > 0; j--)
		{
			MemCopy(dst, src, ICONWIDTH);
			src += ICONWIDTH;
			dst += icopicWH;
		}

	// pøenesení sousedù
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

		// zpìt (dolù)
			if (smerY[j] < 0)
			{
				src += ((ICONHEIGHT-ICONBORD)*ICONWIDTH);
				dst -= (ICONBORD*icopicWH);
				height = ICONBORD;
			}

		// vpøed (nahoru)
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

	// zvýšení ukazatele pozice
		pozice++;
		if (pozice == icoWH)
		{
			pozice = 0;
			radek++;
		}
	}

// vytvoøení bufferu výškové mapy (od dolní øady nahoru)
	float* ter = (float*)MemGet(size*sizeof(float));
	terMap = ter;

	for (i = 0; i < size; i++)
	{
		*ter = (float)((double)((map[i].Param & MAP_VALUE1_MASK) >> MAP_VALUE1_ROT)/1000);
		ter++;
	}

// zrušení bufferù
	for (i = 0; i < icons; i++)
	{
		ico[i].Term();
	}

	MemFree(ico);
	MemFree(neighTab);
	MemFree(iconneigh);

// vytvoøení objektu
	int oldwidth = terWidth;

	GenTerrain();

	int newwidth = terWidth;
	int newheight = terHeight;
	int newsize = newwidth * newheight;
	size = oldwidth * newheight;
	oldwidth -= newwidth;

// vytvoøení bufferu mapování textur
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

		// vlevo nahoøe
		*tp = u;
		tp++;
		*tp = v;
		tp++;

		// vpravo nahoøe
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

// nastavení mapování textur
	D3DF_SetTextUV(D3D_ID, t, 0);

// zrušení bufferù
	MemFree(t);
	MemFree(icomap);

// nastavení opakování textury
	D3DF_SetAddress(D3D_ID, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, 0);

// nastavení textury
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
// vytvoøení terénu z obrázku

double FD3D2DTerrain()
{

// naètení obrázku s terénem
	CPicture pic;
	FPicture(pic);

// pøíprava parametrù obrázku
	pic.DeComp();
	int width = pic.Width();
	int height = pic.Height();

#define OKRAJ	4			// pøídavný okraj

// pracovní obrázek s vìtšími rozmìry
	int korig = OKRAJ; //Round(OKRAJ*D3DSmooth);		 	// pøidané body
	int widthnew = width + 2*korig;
	int heightnew = height + 2*korig;
	CPicture pic2(widthnew, heightnew);

// kopie pùvodního obrázku (s duplikací okrajù)
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

// konverze obrázku na zmenšenou velikost
	int widthnew2 = Round(widthnew / D3DSmooth); // / D3DSmooth);
	if (widthnew2 < 2) widthnew = 2;
//	if (widthnew2 < ((2*OKRAJ+2)/2)) widthnew2 = ((2*OKRAJ+2)/2);
	int heightnew2 = Round(heightnew / D3DSmooth); // / D3DSmooth);
	if (heightnew2 < 2) heightnew2 = 2;
//	if (heightnew2 < ((2*OKRAJ+2)/2)) heightnew2 = ((2*OKRAJ+2)/2);

	buf = ZoomTrueColor(buf, widthnew, heightnew, widthnew2, heightnew2);

// konverze obrázku na správnou velikost
//	widthnew = Round(widthnew / D3DSmooth);
//	if (widthnew < (2*OKRAJ+2)) widthnew = (2*OKRAJ+2);
//	heightnew = Round(heightnew / D3DSmooth);
//	if (heightnew < (2*OKRAJ+2)) heightnew = (2*OKRAJ+2);

	buf = ZoomTrueColor(buf, widthnew2, heightnew2, widthnew, heightnew);

//	width = widthnew - 2*OKRAJ;
//	height = heightnew - 2*OKRAJ;

// konverze obrázku na výškové body
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

// zrušení bufferu
	MemFree(buf);

// vytvoøení objektu
	GenTerrain();

// nastavení opakování textury
	D3DF_SetAddress(D3D_ID, D3DTADDRESS_WRAP, D3DTADDRESS_WRAP, 0);

	return D3D_ID;
}


/////////////////////////////////////////////////////////////////////////////
// zjištìní úrovnì terénu

double FD3DLevel()
{

// naètení horizontální souøadnice
	double x = FNum();
	if (x == 0x1000000)
	{
		x = D3DF_Get(1)->TransX;
	}

// naètení vertikální souøadnice
	double y = FNum();
	if (y == 0x1000000)
	{
		y = D3DF_Get(1)->TransZ;
	}

// aktuální terén
	double h = 0;

	if (D3DF_IsValid(TerenID))
	{
		D3DFITEM* frame = D3DF_Get(TerenID);
		
// test, zda to je terén
		if (frame->IsTerrain &&
			(frame->TerMap != NULL))
		{

// odeètení souøadnice X a Z
			x -= frame->TransX;
			y -= frame->TransZ;

// korekce mìøítka X a Z
			x /= frame->ScaleX;
			y /= frame->ScaleZ;

// modulování souøadnic
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

// pøepoèet na bodové souøadnice
			x = x * frame->TerWidth;
			y = y * frame->TerHeight;

// souøadnice poèáteèního bodu
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

// zjištìní 4 bodù
			int ww = frame->TerWidth + 1;

			float* t = frame->TerMap + y0*ww + x0;
			float n1 = t[0];	// 1: vlevo dole
			float n2 = t[1];	// 2: vpravo dole
			float n3 = t[ww];	// 3: vlevo nahoøe
			float n4 = t[ww+1];	// 4: vpravo nahoøe

// výpoèet výšky (trojúhelník 1-4-2 a 1-3-4)
			if (dx > dy)
			{
				h = n1 + (n2 - n1)*dx + (n4 - n2)*dy;
			}
			else
			{
				h = n1 + (n3 - n1)*dy + (n4 - n3)*dx;
			}

// vynásobení mìøítkem Y
			h *= frame->ScaleY;

// pøiètení souøadnice Y
			h += frame->TransY;
		}
	}
	return h;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní vyhlazení textur

double FGetD3DSmooth()
{
	return D3DSmooth;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní prùhlednosti objektu Direct3D

double FGetD3DTransparent()
{
	if (IsValidID())
	{
		return (double)(GetID()->SrcDstBlend);
	}
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní alfa úrovnì Direct3D

double FGetD3DAlphaRef()
{
	if (IsValidID())
	{
		return (GetID()->AlphaRef);
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// uplynulý pøesný èas 

double FD3DElapsed0()
{
	return D3DElapsedTime;
}

/////////////////////////////////////////////////////////////////////////////
// uplynulý pøesný èas 

double FD3DElapsed()
{	  
	double n = FNum();
	return (D3DElapsedTime*n);
}

/////////////////////////////////////////////////////////////////////////////
// rychlost renderování

double FD3DSpeed()
{
	return D3DAverageFreqI;
}

/////////////////////////////////////////////////////////////////////////////
// volná videopamì

double FD3DFree()
{
	FPUFloat();
	double n = pD3Free();
	FPUDouble();

	return n;
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení 2D obrázku

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

// pøipojení ke kameøe
	D3DF_Link(1, D3D_ID);

	return D3D_ID;
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení èoèkového odlesku

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
// zjištìní poètu stupòù textur

double FGetD3DStage()
{
	return (double)MaxTextureSimult;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní operace barev

double FGetD3DColorOp()
{
	if (IsValidID())
	{
		return GetID()->ColorOp[D3DAktStage];
	}
	return D3DTOP_DISABLE;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní argumentu 1 barev

double FGetD3DColorArg1()
{
	if (IsValidID())
	{
		return GetID()->ColorArg1[D3DAktStage];
	}
	return D3DTA_TEXTURE;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní argumentu 2 barev

double FGetD3DColorArg2()
{
	if (IsValidID())
	{
		return GetID()->ColorArg2[D3DAktStage];
	}
	return D3DTA_CURRENT;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní operace alfa

double FGetD3DAlphaOp()
{
	if (IsValidID())
	{
		return GetID()->AlphaOp[D3DAktStage];
	}
	return D3DTOP_DISABLE;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní argumentu 1 alfa

double FGetD3DAlphaArg1()
{
	if (IsValidID())
	{
		return GetID()->AlphaArg1[D3DAktStage];
	}
	return D3DTA_TEXTURE;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní argumentu 2 alfa

double FGetD3DAlphaArg2()
{
	if (IsValidID())
	{
		return GetID()->AlphaArg2[D3DAktStage];
	}
	return D3DTA_CURRENT;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní horizontálního adresování

double FGetD3DAddressU()
{
	if (IsValidID())
	{
		return GetID()->AddressU[D3DAktStage];
	}
	return D3DTADDRESS_WRAP;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní vertikálního adresování

double FGetD3DAddressV()
{
	if (IsValidID())
	{
		return GetID()->AddressV[D3DAktStage];
	}
	return D3DTADDRESS_WRAP;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní barvy okolí textury
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
// zjištìní zjemnìní vzdálených textur

double FGetD3DBias()
{
	if (IsValidID())
	{
		return GetID()->Bias[D3DAktStage];
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní barvy faktoru textur

double FGetD3DTFactor()
{
	if (IsValidID())
	{
		return GetID()->TFactor;
	}
	return 0xffffff;
}

/////////////////////////////////////////////////////////////////////////////
// poèet úrovní mipmaps

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
// zjištìní renderovací skupiny

double FGetD3DZGroup()
{
	if (IsValidID())
	{
		return (double)(GetID()->RendGroup);
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní hloubkového testu

double FGetD3DZTest()
{
	if (IsValidID())
	{
		return (double)(GetID()->ZTest);
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní dosahu stínu

double FGetD3DShadowRange()
{
	if (IsValidID())
	{
		return (double)(GetID()->ShadowRange);
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní objektu pro LOD

double FGetD3DLODObject()
{
	if (IsValidID())
	{
		return (double)(GetID()->LODNext);
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní vzdálenosti pro LOD

double FGetD3DLODDistance()
{
	if (IsValidID())
	{
		return (GetID()->LODMax);
	}
	return 1e15;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní max. stupnì morfování

double FGetD3DMorphModel()
{
	if (IsValidID())
	{
		return (GetID()->MorphMax);
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní stupnì morfování

double FGetD3DMorphLevel()
{
	if (IsValidID())
	{
		return (GetID()->MorphStage);
	}
	return 0;
}


#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------


/////////////////////////////////////////////////////////////////////////////
// výpoèet vzdálenosti bodu

double FPolarRadius()
{
	double x = FNum();
	double y = FNum();

	double n = sqrt(x*x + y*y);

	return n;
}

/////////////////////////////////////////////////////////////////////////////
// výpoèet úhlu bodu

double FPolarAngle()
{
	double x = FNum();
	double y = FNum();

	double n = atan2(y, x);
	if (n < 0) n = pi2 + n;

	return n;
}

/////////////////////////////////////////////////////////////////////////////
// pøesný èítaè èasu

bool PrecTimerInit = false;			// probìhla inicializace pøesného èasu

// pøesný èítaè èasu
bool IsPerformanceTimer = false;	// je pøesný èítaè èasu
double PerfFrequency = 0;			// frekvence pøesného èítaèe èasu
__int64 PerfCounter = 0;			// naètená hodnota pøesného èítaèe èasu

// multimediální èítaè
double OldPrecTimer = 0;			// stará uschovaná hodnota èítaèe
double AddPrecTimer = 0;			// pøidaná hodnota k èítaèi (pøeteèení po 49 dnech)

double FPreciseTimer()
{
// zjištìní, zda je pøesný mìøiè èasu
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

// obsluha pomocí èítaèe pøesného èasu
	if (IsPerformanceTimer)
	{
		if (::QueryPerformanceCounter((LARGE_INTEGER*)&PerfCounter))
		{
			return (double)PerfCounter / PerfFrequency;
		}
	}

// naètení multimediálního èítaèe
	double time = (double)(DWORD)::timeGetTime() * 0.001;
	double newtime;

// korekce pøi pøeteèení mìøící periody (49.71 dnù)
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
// naètení DLL knihovny

double FDLLLoad()
{
// naètení jména knihovny
	CString jmeno;
	FText(jmeno);

// naètení knihovny
	HINSTANCE inst = ::LoadLibrary(jmeno);
	
	return (double)(int)inst;
}


/////////////////////////////////////////////////////////////////////////////
// importování DLL funkce

double FDLLImport()
{
// naètení handle knihovny
	int module = FIntR();

// naètení jména funkce
	CString jmeno;
	FText(jmeno);

// naètení poètu parametrù
	int param = FIntR();

// naètení pøíznaku volací konvence C
	bool conv = FBool();

// kontrola zadání parametrù
	if ((module == 0) || jmeno.IsEmpty() || (param < 0) || (param > 0xffff)) return 0;

// importování funkce
	FARPROC proc = ::GetProcAddress((HMODULE)module, (LPCSTR)jmeno);

// pøi neúspìchu doplnìní o písmeno "A"
	if (proc == NULL)
	{
		jmeno += 'A';
		proc = ::GetProcAddress((HMODULE)module, (LPCSTR)jmeno);
	}

// pøi neúspìchu druhý pokus - vyhledání èíslem
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

// vytvoøení bloku parametrù
	int* blok = (int*)MemGet(param*4 + 8);

// inicializace bloku parametrù
	blok[0] = (int)proc;
	blok[1] = param;
	if (conv) blok[1] |= 0x10000;
	MemFill(blok + 2, param * 4, 0);

	return (double)(int)blok;
}

/////////////////////////////////////////////////////////////////////////////
// provedení DLL funkce

double FDLLExecNum()
{
// naètení bloku parametrù funkce
	int* blok = (int*)FIntR();

	int	retcode = 0;

#ifndef _M_IX86
	nepodporovaný pøeklad!!!!	
#endif

_asm {
		push	ecx

		mov		esi,[blok]			// bloku parametrù
		or		esi,esi				// jsou platná data?
		jz		pasconv				// není platný blok dat
		lods	dword ptr [esi]		// adresa funkce

		xor		ecx,ecx
		mov		cx,[esi]			// poèet parametrù
		shl		ecx,2				// délka dat v bajtech
		sub		esp,ecx				// vytvoøení místa pro parametry
		mov		edi,esp
		mov		cx,[esi]			// poèet parametrù
		add		esi,4				// zaèátek parametrù v bloku
		rep		movs dword ptr [edi], dword ptr [esi]

		call	eax					// volání funkce
		mov		[retcode],eax		// návratový kód

		mov		esi,[blok]			// blok parametrù
		test	byte ptr [esi+6],1	// je volací konvence C ?
		jz		pasconv				// není C, je Pascal

		xor		eax,eax
		mov		ax,[esi+4]			// EAX <- poèet parametrù
		shl		eax,2				// poèet bajtù
		add		esp,eax				// návrat ukazatele zásobníku

pasconv:
		pop		ecx
	}

	return retcode;
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení bloku pamìti

double FMemoryNew()
{
// naètení velikosti bloku
	int size = FIntR();

// vytvoøení pamìového bloku
	void* blok = MemGet(size);

	return (double)(int)blok;
}

/////////////////////////////////////////////////////////////////////////////
// zmìna velikosti bloku pamìti

double FMemoryResize()
{
// naètení staré adresy bloku
	void* blok = (void*)FIntR();

// naètení požadované velikosti bloku
	int size = FIntR();

// zmìna velikosti bloku
	blok = MemSize(blok, size);

	return (double)(int)blok;
}

/////////////////////////////////////////////////////////////////////////////
// ukazatel ètení z pamìti

double FGetMemoryRead()
{
	return (double)(int)DLLMemoryRead;
}

/////////////////////////////////////////////////////////////////////////////
// ukazatel zápisu do pamìti

double FGetMemoryWrite()
{
	return (double)(int)DLLMemoryWrite;
}

/////////////////////////////////////////////////////////////////////////////
// porovnání dat

double FMemoryCompare()
{
// naètení délky dat
	int len = FIntR();

// adresa dat
	char* buf1 = (char*)DLLMemoryRead;
	char* buf2 = (char*)DLLMemoryWrite;

// porovnání dat
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
// bajt se znaménkem

double FGetMemoryByteS()
{
	char* buf = (char*)DLLMemoryRead;
	long data = (long)*buf;
	buf++;
	DLLMemoryRead = buf;
	return data;
}

/////////////////////////////////////////////////////////////////////////////
// bajt bez znaménka

double FGetMemoryByteU()
{
	BYTE* buf = (BYTE*)DLLMemoryRead;
	long data = (long)*buf;
	buf++;
	DLLMemoryRead = buf;
	return data;
}

/////////////////////////////////////////////////////////////////////////////
// slovo se znaménkem

double FGetMemoryWordS()
{
	short* buf = (short*)DLLMemoryRead;
	long data = (long)*buf;
	buf++;
	DLLMemoryRead = buf;
	return data;
}

/////////////////////////////////////////////////////////////////////////////
// slovo bez znaménka

double FGetMemoryWordU()
{
	WORD* buf = (WORD*)DLLMemoryRead;
	long data = (long)*buf;
	buf++;
	DLLMemoryRead = buf;
	return data;
}

/////////////////////////////////////////////////////////////////////////////
// 3-bajt se znaménkem

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
// 3-bajt bez znaménka

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
// dvouslovo se znaménkem

double FGetMemoryDWordS()
{
	long* buf = (long*)DLLMemoryRead;
	long data = *buf;
	buf++;
	DLLMemoryRead = buf;
	return data;
}

/////////////////////////////////////////////////////////////////////////////
// dvouslovo bez znaménka

double FGetMemoryDWordU()
{
	DWORD* buf = (DWORD*)DLLMemoryRead;
	DWORD data = *buf;
	buf++;
	DLLMemoryRead = buf;
	return data;
}

/////////////////////////////////////////////////////////////////////////////
// reálné èíslo 4 bajty

double FGetMemoryFloat()
{
	float* buf = (float*)DLLMemoryRead;
	float data = *buf;
	buf++;
	DLLMemoryRead = buf;
	return data;
}

/////////////////////////////////////////////////////////////////////////////
// reálné èíslo 8 bajtù

double FGetMemoryDouble()
{
	double* buf = (double*)DLLMemoryRead;
	double data = *buf;
	buf++;
	DLLMemoryRead = buf;
	return data;
}

/////////////////////////////////////////////////////////////////////////////
// délka textu s pevnou délkou

double FGetMemoryTextNLen()
{
	return DLLMemoryTextNLen;
}


/////////////////////////////////////////////////////////////////////////////
// handle okenního prvku

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
// vytvoøení 3D objektu z pamìti

double FMemory3D()
{
// naètení parametrù
	int vN = FIntR();						// poèet vrcholù
	D3DVECTOR* vert = (D3DVECTOR*)FIntR();	// buffer vrcholù
	D3DVECTOR* norm = (D3DVECTOR*)FIntR();	// buffer normál
	float* text = (float*)FIntR();			// buffer textur
	D3DCOLORVALUE* col = (D3DCOLORVALUE*)FIntR(); // buffer barev
	int fN = FIntR();						// poèet plošek
	int* face = (int*)FIntR();				// buffer plošek

// omezení indexù plošek
	for (int i = fN*3 - 1; i >= 0; i--)
	{
		if (face[i] >= vN) face[i] = 0;
	}

// vytvoøení objektu
	int inx = D3DF_CreateMesh(vN, vert, fN, face);

// nastavení normál
	if (norm != NULL)
	{
		D3DF_SetNormal(inx, norm);
	}
	else
	{
		D3DF_AutoNormals(inx, 0.5);
	}

// nastavení mapování textur
	if (text != NULL)
	{
		D3DF_SetTextUV(inx, text, 0);
	}

// nastavení barev
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
// vytvoøení pamìového bloku z 3D objektu

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
// poskytnutí potomka 3D objektu

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

// nalezení prvního potomka
	if (inx == -1)
	{
		return FD3DChild0();
	}

// nalezení rodièe
	if (inx == -2)
	{
		if (IsValidID())
		{
			return (double)(GetID()->Parent);
		}
		return -1;
	}

// nalezení dalšího potomka
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
// ètení konfiguraèního èísla

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
// skupina klíèù registru

double FGetRegKey()
{
	return REGKey;
}

/////////////////////////////////////////////////////////////////////////////
// ètení èísla z registru

int GetRegNum(int cislo)
{
// otevøení klíèe
	HKEY hkey;

	if (::RegOpenKeyEx(
		(HKEY) (REGKey | 0x80000000),
		REGSubkey,
		0,
		KEY_QUERY_VALUE,
		&hkey) == ERROR_SUCCESS)
	{

// naètení hodnoty klíèe
		DWORD bufsize = 4;

		::RegQueryValueEx(
			hkey,
			REGValue,
			0,
			NULL,
			(BYTE*)&cislo,
			&bufsize);

// uzavøení klíèe
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
// --------------------- konec vypnutí pro MINI verzi -------------------
