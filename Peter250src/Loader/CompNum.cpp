
#include "Main.h"

/***************************************************************************\
*																			*
*					Kompilace programu - èíselný výraz						*
*																			*
\***************************************************************************/

#pragma optimize("s", on)			// optimalizace na minimální velikost


/////////////////////////////////////////////////////////////////////////////
// pøidání èíselné konstanty do bufferu programu

void CompAddItemNum(double data)
{
// vytvoøení položky
	int index = CompAddItem(FNumConst);

// nastavení dat
	ProgBuf[index].Double = data;
}


/////////////////////////////////////////////////////////////////////////////
// pøidání kódu klávesy do bufferu programu
 
void _fastcall CompAddItemNum(BYTE data)
{
// statická promìnná pro pøevod
	static int tmp = 0;

// pøevod na èíslo INT
	*(BYTE*)&tmp = data;

// vytvoøení položky a nastavení dat
	ProgBuf[CompAddItem(FNumConst)].Double = tmp;
}


/////////////////////////////////////////////////////////////////////////////
// tabulka øídicích kódù kláves (zajistit spojitost kódù kláves IDF !!!!)

#define CompTabVKBeg IDF_F1			// první kód tabulky
#define CompTabVKEnd IDF_VK_SPACE	// poslední kód tabulky

BYTE CompTabVK[] = {
	VK_F1,			// IDF_F1
	VK_F2,			// IDF_F2
	VK_F3,			// IDF_F3
	VK_F4,			// IDF_F4
	VK_F5,			// IDF_F5
	VK_F6,			// IDF_F6
	VK_F7,			// IDF_F7
	VK_F8,			// IDF_F8
	VK_F9,			// IDF_F9
	VK_F10,			// IDF_F10
	VK_F11,			// IDF_F11
	VK_F12,			// IDF_F12
	'A',			// IDF_A
	'B',			// IDF_B
	'C',			// IDF_C
	'D',			// IDF_D
	'E',			// IDF_E
	'F',			// IDF_F
	'G',			// IDF_G
	'H',			// IDF_H
	'I',			// IDF_I
	'J',			// IDF_J
	'K',			// IDF_K
	'L',			// IDF_L
	'M',			// IDF_M
	'N',			// IDF_N
	'O',			// IDF_O
	'P',			// IDF_P
	'Q',			// IDF_Q
	'R',			// IDF_R
	'S',			// IDF_S
	'T',			// IDF_T
	'U',			// IDF_U
	'V',			// IDF_V
	'W',			// IDF_W
	'X',			// IDF_X
	'Y',			// IDF_Y
	'Z',			// IDF_Z
	VK_NUMPAD0,		// IDF_VK_NUM0
	VK_NUMPAD1,		// IDF_VK_NUM1
	VK_NUMPAD2,		// IDF_VK_NUM2
	VK_NUMPAD3,		// IDF_VK_NUM3
	VK_NUMPAD4,		// IDF_VK_NUM4
	VK_NUMPAD5,		// IDF_VK_NUM5
	VK_NUMPAD6,		// IDF_VK_NUM6
	VK_NUMPAD7,		// IDF_VK_NUM7
	VK_NUMPAD8,		// IDF_VK_NUM8
	VK_NUMPAD9,		// IDF_VK_NUM9
	VK_DECIMAL,		// IDF_VK_NUMTECKA
	VK_ADD,			// IDF_VK_NUMPLUS
	VK_SUBTRACT,	// IDF_VK_NUMMINUS
	VK_MULTIPLY,	// IDF_VK_NUMKRAT
	VK_DIVIDE,		// IDF_VK_NUMDELENO
	VK_RETURN_NUM,	// IDF_VK_NUMENTER
	VK_NUMLOCK,		// IDF_VK_NUMLOCK
	'1',			// IDF_VK_1
	'2',			// IDF_VK_2
	'3',			// IDF_VK_3
	'4',			// IDF_VK_4
	'5',			// IDF_VK_5
	'6',			// IDF_VK_6
	'7',			// IDF_VK_7
	'8',			// IDF_VK_8
	'9',			// IDF_VK_9
	'0',			// IDF_VK_0
	192,			// IDF_VK_LCARKA
	189,			// IDF_VK_POMLCKA
	187,			// IDF_VK_ROVNITKO
	220,			// IDF_VK_ZLOMITKO
	219,			// IDF_VK_LLOMENA
	221,			// IDF_VK_RLOMENA
	186,			// IDF_VK_STREDNIK
	222,			// IDF_VK_RCARKA
	188,			// IDF_VK_CARKA
	190,			// IDF_VK_TECKA
	191,			// IDF_VK_LOMITKO
	VK_ESCAPE,		// IDF_VK_ESC
	VK_TAB,			// IDF_VK_TAB
	VK_CAPITAL,		// IDF_VK_CAPSLOCK
	VK_LWIN,		// IDF_VK_WIN
	VK_BACK,		// IDF_VK_BS
	VK_RETURN,		// IDF_VK_ENTER
	VK_APPS,		// IDF_VK_MENU
	VK_LEFT,		// IDF_VK_LEFT
	VK_RIGHT,		// IDF_VK_RIGHT
	VK_UP,			// IDF_VK_UP
	VK_DOWN,		// IDF_VK_DOWN
	VK_INSERT,		// IDF_VK_INSERT
	VK_DELETE,		// IDF_VK_DELETE
	VK_HOME,		// IDF_VK_HOME
	VK_END,			// IDF_VK_END
	VK_PRIOR,		// IDF_VK_PAGEUP
	VK_NEXT,		// IDF_VK_PAGEDOWN
	VK_PRINT,		// IDF_VK_PRINT
	VK_SCROLL,		// IDF_VK_SCROLL
	VK_PAUSE,		// IDF_VK_PAUSE
	VK_SHIFT,		// IDF_VK_SHIFT
	VK_CONTROL,		// IDF_VK_CTRL
	VK_MENU,		// IDF_VK_ALT
	VK_SPACE,		// IDF_VK_SPACE
};


/////////////////////////////////////////////////////////////////////////////
// pøeklad èíselného výrazu (vrací true = operace OK)

bool _fastcall CompNum(int index)
{
// kontrola tabulky kódù kláves
#pragma warning ( disable: 4127)				// hlášení - konstantní podmínka
	ASSERT(CompTabVKEnd - CompTabVKBeg + 1 == sizeof(CompTabVK));
	ASSERT(	(IDF_F1				+ 1 == IDF_F2			) && 
			(IDF_F2				+ 1 == IDF_F3			) &&
			(IDF_F3				+ 1 == IDF_F4			) &&
			(IDF_F4				+ 1 == IDF_F5			) &&
			(IDF_F5				+ 1 == IDF_F6			) &&
			(IDF_F6				+ 1 == IDF_F7			) &&
			(IDF_F7				+ 1 == IDF_F8			) &&
			(IDF_F8				+ 1 == IDF_F9			) &&
			(IDF_F9				+ 1 == IDF_F10			) &&
			(IDF_F10			+ 1 == IDF_F11			) &&
			(IDF_F11			+ 1 == IDF_F12			) &&
			(IDF_F12			+ 1 == IDF_A			) &&
			(IDF_A				+ 1 == IDF_B			) &&
			(IDF_B				+ 1 == IDF_C			) &&
			(IDF_C				+ 1 == IDF_D			) &&
			(IDF_D				+ 1 == IDF_E			) &&
			(IDF_E				+ 1 == IDF_F			) &&
			(IDF_F				+ 1 == IDF_G			) &&
			(IDF_G				+ 1 == IDF_H			) &&
			(IDF_H				+ 1 == IDF_I			) &&
			(IDF_I				+ 1 == IDF_J			) &&
			(IDF_J				+ 1 == IDF_K			) &&
			(IDF_K				+ 1 == IDF_L			) &&
			(IDF_L				+ 1 == IDF_M			) &&
			(IDF_M				+ 1 == IDF_N			) &&
			(IDF_N				+ 1 == IDF_O			) &&
			(IDF_O				+ 1 == IDF_P			) &&
			(IDF_P				+ 1 == IDF_Q			) &&
			(IDF_Q				+ 1 == IDF_R			) &&
			(IDF_R				+ 1 == IDF_S			) &&
			(IDF_S				+ 1 == IDF_T			) &&
			(IDF_T				+ 1 == IDF_U			) &&
			(IDF_U				+ 1 == IDF_V			) &&
			(IDF_V				+ 1 == IDF_W			) &&
			(IDF_W				+ 1 == IDF_X			) &&
			(IDF_X				+ 1 == IDF_Y			) &&
			(IDF_Y				+ 1 == IDF_Z			) &&
			(IDF_Z				+ 1 == IDF_VK_NUM0		) &&
			(IDF_VK_NUM0		+ 1 == IDF_VK_NUM1		) &&
			(IDF_VK_NUM1		+ 1 == IDF_VK_NUM2		) &&
			(IDF_VK_NUM2		+ 1 == IDF_VK_NUM3		) &&
			(IDF_VK_NUM3		+ 1 == IDF_VK_NUM4		) &&
			(IDF_VK_NUM4		+ 1 == IDF_VK_NUM5		) &&
			(IDF_VK_NUM5		+ 1 == IDF_VK_NUM6		) &&
			(IDF_VK_NUM6		+ 1 == IDF_VK_NUM7		) &&
			(IDF_VK_NUM7		+ 1 == IDF_VK_NUM8		) &&
			(IDF_VK_NUM8		+ 1 == IDF_VK_NUM9		) &&
			(IDF_VK_NUM9		+ 1 == IDF_VK_NUMTECKA	) &&
			(IDF_VK_NUMTECKA	+ 1 == IDF_VK_NUMPLUS	) &&
			(IDF_VK_NUMPLUS		+ 1 == IDF_VK_NUMMINUS	) &&
			(IDF_VK_NUMMINUS	+ 1 == IDF_VK_NUMKRAT	) &&
			(IDF_VK_NUMKRAT		+ 1 == IDF_VK_NUMDELENO	) &&
			(IDF_VK_NUMDELENO	+ 1 == IDF_VK_NUMENTER	) &&
			(IDF_VK_NUMENTER	+ 1 == IDF_VK_NUMLOCK	) &&
			(IDF_VK_NUMLOCK		+ 1 == IDF_VK_1			) &&
			(IDF_VK_1			+ 1 == IDF_VK_2			) &&
			(IDF_VK_2			+ 1 == IDF_VK_3			) &&
			(IDF_VK_3			+ 1 == IDF_VK_4			) &&
			(IDF_VK_4			+ 1 == IDF_VK_5			) &&
			(IDF_VK_5			+ 1 == IDF_VK_6			) &&
			(IDF_VK_6			+ 1 == IDF_VK_7			) &&
			(IDF_VK_7			+ 1 == IDF_VK_8			) &&
			(IDF_VK_8			+ 1 == IDF_VK_9			) &&
			(IDF_VK_9			+ 1 == IDF_VK_0			) &&
			(IDF_VK_0			+ 1 == IDF_VK_LCARKA	) &&
			(IDF_VK_LCARKA		+ 1 == IDF_VK_POMLCKA	) &&
			(IDF_VK_POMLCKA		+ 1 == IDF_VK_ROVNITKO	) &&
			(IDF_VK_ROVNITKO	+ 1 == IDF_VK_ZLOMITKO	) &&
			(IDF_VK_ZLOMITKO	+ 1 == IDF_VK_LLOMENA	) &&
			(IDF_VK_LLOMENA		+ 1 == IDF_VK_RLOMENA	) &&
			(IDF_VK_RLOMENA		+ 1 == IDF_VK_STREDNIK	) &&
			(IDF_VK_STREDNIK	+ 1 == IDF_VK_RCARKA	) &&
			(IDF_VK_RCARKA		+ 1 == IDF_VK_CARKA		) &&
			(IDF_VK_CARKA		+ 1 == IDF_VK_TECKA		) &&
			(IDF_VK_TECKA		+ 1 == IDF_VK_LOMITKO	) &&
			(IDF_VK_LOMITKO		+ 1 == IDF_VK_ESC		) &&
			(IDF_VK_ESC			+ 1 == IDF_VK_TAB		) &&
			(IDF_VK_TAB			+ 1 == IDF_VK_CAPSLOCK	) &&
			(IDF_VK_CAPSLOCK	+ 1 == IDF_VK_WIN		) &&
			(IDF_VK_WIN			+ 1 == IDF_VK_BS		) &&
			(IDF_VK_BS			+ 1 == IDF_VK_ENTER		) &&
			(IDF_VK_ENTER		+ 1 == IDF_VK_MENU		) &&
			(IDF_VK_MENU		+ 1 == IDF_VK_LEFT		) &&
			(IDF_VK_LEFT		+ 1 == IDF_VK_RIGHT		) &&
			(IDF_VK_RIGHT		+ 1 == IDF_VK_UP		) &&
			(IDF_VK_UP			+ 1 == IDF_VK_DOWN		) &&
			(IDF_VK_DOWN		+ 1 == IDF_VK_INSERT	) &&
			(IDF_VK_INSERT		+ 1 == IDF_VK_DELETE	) &&
			(IDF_VK_DELETE		+ 1 == IDF_VK_HOME		) &&
			(IDF_VK_HOME		+ 1 == IDF_VK_END		) &&
			(IDF_VK_END			+ 1 == IDF_VK_PAGEUP	) &&
			(IDF_VK_PAGEUP		+ 1 == IDF_VK_PAGEDOWN	) &&
			(IDF_VK_PAGEDOWN	+ 1 == IDF_VK_PRINT		) &&
			(IDF_VK_PRINT		+ 1 == IDF_VK_SCROLL	) &&
			(IDF_VK_SCROLL		+ 1 == IDF_VK_PAUSE		) &&
			(IDF_VK_PAUSE		+ 1 == IDF_VK_SHIFT		) &&
			(IDF_VK_SHIFT		+ 1 == IDF_VK_CTRL		) &&
			(IDF_VK_CTRL		+ 1 == IDF_VK_ALT		) &&
			(IDF_VK_ALT			+ 1 == IDF_VK_SPACE		));
#pragma warning ( default: 4127)				// hlášení - konstantní podmínka

// adresa zdrojového prvku
	if ((DWORD)index >= (DWORD)BufEdiN) return false;
	PETPROG*	item = BufEdi + index;
	PETPROG2*	item2 = BufEdi2 + index;
	int refinx = item->RefIndex;
	PETPROG2*	prg2;

// kontrola, zda je položka vypnuta
	if ((item->Param & (PETPROG_OFF | PETPROG_OFF_DEP)) != 0) return false;

// vìtvení podle funkce
	switch (item->Func + IDF)
	{
	case IDF_NUM:
		CompAddItemNum(LoadNum(item, item2));	// èíselná konstanta
		return true;

	case IDF_0:
		CompAddItemNum((BYTE)0);				// èíslo 0
		return true;

	case IDF_1:
		CompAddItemNum((BYTE)1);				// èíslo 1
		return true;

	case IDF_2:
		CompAddItemNum((BYTE)2);				// èíslo 2
		return true;

	case IDF_3:
		CompAddItemNum((BYTE)3);				// èíslo 3
		return true;

	case IDF_4:
		CompAddItemNum((BYTE)4);				// èíslo 4
		return true;

	case IDF_5:
		CompAddItemNum((BYTE)5);				// èíslo 5
		return true;

	case IDF_6:
		CompAddItemNum((BYTE)6);				// èíslo 6
		return true;

	case IDF_7:
		CompAddItemNum((BYTE)7);				// èíslo 7
		return true;

	case IDF_8:
		CompAddItemNum((BYTE)8);				// èíslo 8
		return true;

	case IDF_9:
		CompAddItemNum((BYTE)9);				// èíslo 9
		return true;

	case IDF_ADD:
		return CompNumGrp(index, FAdd, FAdd);	// operátor ADD

	case IDF_SUB:
		return CompNumGrp(index, FSub, FSub1);	// operátor SUB

	case IDF_MUL:
		return CompNumGrp(index, FMul, FMul);	// operátor MUL

	case IDF_DIV:
		return CompNumGrp(index, FDiv, FDiv1);	// operátor DIV

	case IDF_REAL:
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			item = BufObj + refinx;
			item2 = BufObj2 + refinx;
			if (Real.IsNotValid(item2->Data)) return false;

			if (item2->List < 0)
			{
				CompAddItem(FNumObj, item2->Data);
			}
			else
			{
				CompAddItem(FNumObjList, item2->Data, item2->List);
			}
			return true;
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			item = BufLoc + refinx;
			item2 = BufLoc2 + refinx;
			if (Real.IsNotValid(item2->Data)) return false;

			if (item2->List < 0)
			{
				CompAddItem(FNumLoc, item2->Local + KorigNum);
			}
			else
			{
				CompAddItem(FNumLocList, item2->Local + KorigNum, item2->LocalList + KorigLst);
			}
			return true;
		}
		return false;

	case IDF_FNC:
		return CompFunc(index, IDF_REAL);		// funkce s návratem èísla

	case IDF_SMER:
		CompAddItem(FGetSmer);					// Petr - smìr
		return true;

	case IDF_POZICEX:
		CompAddItem(FGetPozX);					// Petr - pozice X
		return true;

	case IDF_POZICEY:
		CompAddItem(FGetPozY);					// Petr - pozice Y
		return true;

	case IDF_PETRA_SMER:
		CompAddItem(FGetSmer2);					// Petra - smìr
		return true;

	case IDF_PETRA_POZICEX:
		CompAddItem(FGetPozX2);					// Petra - pozice X
		return true;

	case IDF_PETRA_POZICEY:
		CompAddItem(FGetPozY2);					// Petra - pozice Y
		return true;

	case IDF_MOD:
		return CompNumGrp(index, FMod, FMod);	// operátor MOD

	case IDF_INC:
		return CompNumPar(index, FInc);			// inkrementace

	case IDF_DEC:
		return CompNumPar(index, FDec);			// dekrementace

	case IDF_SIN:
		return CompNumPar(index, FSin);			// sinus

	case IDF_COS:
		return CompNumPar(index, FCos);			// cosinus

	case IDF_TG:
		return CompNumPar(index, FTg);			// tangens

	case IDF_COTG:
		return CompNumPar(index, FCoTg);		// cotangens

	case IDF_ARCSIN:
		return CompNumPar(index, FArcSin);		// arcsinus

	case IDF_ARCCOS:
		return CompNumPar(index, FArcCos);		// arccosinus

	case IDF_ARCTG:
		return CompNumPar(index, FArcTg);		// arctangens

	case IDF_ARCCOTG:
		return CompNumPar(index, FArcCoTg);		// arccotangens

	case IDF_PI:
		CompAddItemNum(pi);						// Ludolfovo èíslo
		return true;

	case IDF_INTEGER:
		return CompNumPar(index, FNumInt);		// celé èíslo

	case IDF_FRAC:
		return CompNumPar(index, FFrac);		// desetinná èást èísla

	case IDF_ABS:
		return CompNumPar(index, FAbs);			// absolutní hodnota

	case IDF_SQRT:
		return CompNumPar(index, FSqrt);		// odmocnina

	case IDF_FACT:
		return CompNumPar(index, FFact);		// faktoriál

	case IDF_RAND:
		if (!CompNumPar(index, FRand))			// náhodné èíslo s násobkem
		{
			CompAddItem(FRand0);				// náhodné èíslo bez násobku
		}
		return true;

	case IDF_EUL:
		CompAddItemNum(eul);					// Eulerova konstanta
		return true;

	case IDF_DEGRAD:
		return CompNumPar(index, FDegRad);		// pøevod stupòù na radiány

	case IDF_RADDEG:
		return CompNumPar(index, FRadDeg);		// pøevod radiánù na stupnì

	case IDF_LOG:
		return CompNumPar(index, FLog);			// dekadický logaritmus

	case IDF_LN:
		return CompNumPar(index, FLn);			// pøirozený logaritmus

	case IDF_PEXP:
		return CompNumPar(index, FExp);			// pøirozený exponent

	case IDF_MOCN:
		return CompNumGrp(index, FMocn, FMocn);	// operátor MOCNINA

	case IDF_UHEL0:
		CompAddItemNum((BYTE)0);				// úhel 0 stupòù
		return true;

	case IDF_UHEL45:
		CompAddItemNum(uhel45);					// úhel 45 stupòù
		return true;

	case IDF_UHEL90:
		CompAddItemNum(uhel90);					// úhel 90 stupòù
		return true;

	case IDF_UHEL135:
		CompAddItemNum(uhel135);				// úhel 135 stupòù
		return true;

	case IDF_UHEL180:
		CompAddItemNum(uhel180);				// úhel 180 stupòù
		return true;

	case IDF_UHEL225:
		CompAddItemNum(uhel225);				// úhel 225 stupòù
		return true;

	case IDF_UHEL270:
		CompAddItemNum(uhel270);				// úhel 270 stupòù
		return true;

	case IDF_UHEL315:
		CompAddItemNum(uhel315);				// úhel 315 stupòù
		return true;

	case IDF_KLAV_VKEY:
		CompAddItem(FVKey);						// naètení klávesy bez èekání
		return true;

	case IDF_MOUSE_X:
		CompAddItem(FMouseX);					// naètení souøadnice X myši
		return true;

	case IDF_MOUSE_Y:
		CompAddItem(FMouseY);					// naètení souøadnice Y myši
		return true;

	case IDF_SPRITE_X:
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			prg2 = BufObj2 + refinx;
			if (Sprite.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				CompAddItem(FGetSpriteX, prg2->Data);
			}
			else
			{
				CompAddItem(FGetSpriteXList, prg2->Data, prg2->List);
			}
			return true;
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			prg2 = BufLoc2 + refinx;
			if (Sprite.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				CompAddItem(FGetSpriteXLoc, prg2->Local + KorigSpr);
			}
			else
			{
				CompAddItem(FGetSpriteXLocList, prg2->Local + KorigSpr, prg2->LocalList + KorigLst);
			}
			return true;
		}
		return false;

	case IDF_SPRITE_Y:
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			prg2 = BufObj2 + refinx;
			if (Sprite.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				CompAddItem(FGetSpriteY, prg2->Data);
			}
			else
			{
				CompAddItem(FGetSpriteYList, prg2->Data, prg2->List);
			}
			return true;
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			prg2 = BufLoc2 + refinx;
			if (Sprite.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				CompAddItem(FGetSpriteYLoc, prg2->Local + KorigSpr);
			}
			else
			{
				CompAddItem(FGetSpriteYLocList, prg2->Local + KorigSpr, prg2->LocalList + KorigLst);
			}
			return true;
		}
		return false;

	case IDF_SPRITE_SMER:
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			prg2 = BufObj2 + refinx;
			if (Sprite.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				CompAddItem(FGetSpriteSmer, prg2->Data);
			}
			else
			{
				CompAddItem(FGetSpriteSmerList, prg2->Data, prg2->List);
			}
			return true;
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			prg2 = BufLoc2 + refinx;
			if (Sprite.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				CompAddItem(FGetSpriteSmerLoc, prg2->Local + KorigSpr);
			}
			else
			{
				CompAddItem(FGetSpriteSmerLocList, prg2->Local + KorigSpr, prg2->LocalList + KorigLst);
			}
			return true;
		}
		return false;

	case IDF_SPRITE_FAZE:
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			prg2 = BufObj2 + refinx;
			if (Sprite.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				CompAddItem(FGetSpriteFaze, prg2->Data);
			}
			else
			{
				CompAddItem(FGetSpriteFazeList, prg2->Data, prg2->List);
			}
			return true;
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			prg2 = BufLoc2 + refinx;
			if (Sprite.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				CompAddItem(FGetSpriteFazeLoc, prg2->Local + KorigSpr);
			}
			else
			{
				CompAddItem(FGetSpriteFazeLocList, prg2->Local + KorigSpr, prg2->LocalList + KorigLst);
			}
			return true;
		}
		return false;

	case IDF_SPRITE_STATUS:
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			prg2 = BufObj2 + refinx;
			if (Sprite.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				CompAddItem(FGetSpriteStatus, prg2->Data);
			}
			else
			{
				CompAddItem(FGetSpriteStatusList, prg2->Data, prg2->List);
			}
			return true;
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			prg2 = BufLoc2 + refinx;
			if (Sprite.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				CompAddItem(FGetSpriteStatusLoc, prg2->Local + KorigSpr);
			}
			else
			{
				CompAddItem(FGetSpriteStatusLocList, prg2->Local + KorigSpr, prg2->LocalList + KorigLst);
			}
			return true;
		}
		return false;

	case IDF_LIST_SIZE:
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			prg2 = BufObj2 + refinx;
			if (List.IsNotValid(prg2->List)) return false;
			CompAddItemNum((double)List[prg2->List].Size);
			return true;
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			prg2 = BufLoc2 + refinx;
			if (List.IsNotValid(prg2->List)) return false;
			CompAddItemNum((double)List[prg2->List].Size);
			return true;
		}
		return false;

	case IDF_LIST_INDEX:
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			prg2 = BufObj2 + refinx;
			if (List.IsNotValid(prg2->List)) return false;
			CompAddItem(FGetList, 0, prg2->List);
			return true;
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			prg2 = BufLoc2 + refinx;
			if (List.IsNotValid(prg2->List)) return false;
			CompAddItem(FGetListLoc, 0, prg2->LocalList);
			return true;
		}
		return false;

	case IDF_LIST_AUTO:
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			prg2 = BufObj2 + refinx;
			if (List.IsNotValid(prg2->List)) return false;
			CompAddItem(FGetListAuto, 0, prg2->List);
			return true;
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			prg2 = BufLoc2 + refinx;
			if (List.IsNotValid(prg2->List)) return false;
			CompAddItem(FGetListAutoLoc, 0, prg2->LocalList);
			return true;
		}
		return false;

	case IDF_PETER_SWCN1:
		CompAddItem(FGetSwcN1);					// naètení èísla 1 - Petr
		return true;

	case IDF_PETRA_SWCN1:
		CompAddItem(FGetSwcN12);				// naètení èísla 1 - Petra
		return true;

	case IDF_PETER_SWCN2:
		CompAddItem(FGetSwcN2);					// naètení èísla 2 - Petr
		return true;

	case IDF_PETRA_SWCN2:
		CompAddItem(FGetSwcN22);				// naètení èísla 2 - Petra
		return true;

	case IDF_PETER_SWCN3:
		CompAddItem(FGetSwcN3);					// naètení èísla 3 - Petr
		return true;

	case IDF_PETRA_SWCN3:
		CompAddItem(FGetSwcN32);				// naètení èísla 3 - Petra
		return true;

	case IDF_TEXT_LEN:
		return CompTxtPar(index, FTextLen);	// délka textu

	case IDF_TEXT_SCAN:
		return CompTxtPar(index, FTextScan);// pøevod textu na èíslo

	case IDF_TEXT_FIND:
		CompAddItem(FTextFind);				// vyhledání textu
		CompTxtSubPar(index, IDF_TEXT_FIND_SUB);
		CompTxtSubPar(index, IDF_TEXT_INPUT);
		return true;

	case IDF_ORD:
		return CompTxtPar(index, FTextOrd);	// abecední poøadí znaku

	case IDF_COLOR:
		{									// barevná konstanta
			int col = WhiteCol;

			if (Icon.IsValid(item->Icon))
			{
				Icon[item->Icon].DeComp();
				col = Icon[item->Icon].DataData()
						[ICONWIDTH*(ICONHEIGHT/2) + ICONWIDTH/2];
			}
			CompAddItemNum((double)PalExport(col));
		}
		return true;

	case IDF_COLOR_LEVEL:
		CompAddItem(FColorLevel);			// nastavení jasu barvy
		CompNumSubPar(index, IDF_GRAPH_COLOR, DEFCOLOR);
		CompNumSubPar(index, IDF_COLOR_LEVEL_N, 1);
		return true;

	case IDF_KLAV_VKEY_WAIT:
		CompAddItem(FVKeyWait);				// naètení klávesy s èekáním
		return true;

	case IDF_VK_NOKEY:
		CompAddItemNum((BYTE)0);			// klávesa 0
		return true;

	case IDF_VK_WINR:
		CompAddItemNum((BYTE)VK_RWIN);		// klávesa WinR
		return true;

	case IDF_VK_SHIFTR:
		CompAddItemNum((BYTE)VK_RSHIFT);	// klávesa ShiftR
		return true;

	case IDF_VK_CTRLR:
		CompAddItemNum((BYTE)VK_RCONTROL);	// klávesa ControlR
		return true;

	case IDF_VK_ALTR:
		CompAddItemNum((BYTE)VK_RMENU);		// klávesa AltR
		return true;

	case IDF_TEXTLINE_NUM:
		return CompTxtPar(index, FTextLineNum);	// poèet øádkù textu

	case IDF_MUSIC_POS:
		CompAddItem(FGetMusicPos);			// zjištìní pozice hudby
		return true;

	case IDF_MUSIC_LEN:
		CompAddItem(FMusicLen);				// zjištìní délky hudby
		return true;

	case IDF_COLOR_SYNTH:
		CompAddItem(FColorSynth);			// syntéza barvy
		CompNumSubPar(index, IDF_COLOR_SYNTH_RED, 0);
		CompNumSubPar(index, IDF_COLOR_SYNTH_GREEN, 0);
		CompNumSubPar(index, IDF_COLOR_SYNTH_BLUE, 0);
		CompNumSubPar(index, IDF_COLOR_SYNTH_ALPHA, 1);
		return true;

	case IDF_GET_POINT:
		CompAddItem(FGetPoint);				// zjištìní barvy bodu
		CompNumSubPar(index, IDF_GET_POINT_X, 0);
		CompNumSubPar(index, IDF_GET_POINT_Y, 0);
		return true;

	case IDF_SQR:
		return CompNumPar(index, FNumSqr);	// druhá mocnina

	case IDF_CD_LENGTH:
		CompAddItem(FCDGetLength);			// zjištìní délky CD
		return true;

	case IDF_CD_TRACKS:
		CompAddItem(FCDGetTracks);			// zjištìní poètu stop CD
		return true;

	case IDF_CD_POS:
		CompAddItem(FCDGetPos);				// zjištìní aktivní pozice CD
		return true;

	case IDF_CD_TRACK:
		CompAddItem(FCDGetTrack);			// zjištìní aktivní stopy CD
		return true;

	case IDF_CD_TRACK_LENGTH:
		CompAddItem(FCDGetTrackLength);		// zjištìní délky stopy CD
		return true;

	case IDF_CD_TRACK_POS:
		CompAddItem(FCDGetTrackPos);		// zjištìní pozice stopy CD
		return true;

	case IDF_FILE_DISKSIZE:
		CompAddItem(FGetDiskSize);			// zjištìní celkové velikosti disku
		return true;

	case IDF_FILE_DISKFREE:
		CompAddItem(FGetDiskFree);			// zjištìní volného místa disku
		return true;

	case IDF_FILE_DISKSERIAL:
		CompAddItem(FGetDiskSerial);		// zjištìní sériového èísla disku
		return true;

	case IDF_FILE_CREATE:
		CompAddItem(FGetFileCreate);		// zjištìní èasu vytvoøení souboru
		return true;

	case IDF_FILE_OPEN:
		CompAddItem(FGetFileOpen);			// zjištìní èasu ètení ze souboru
		return true;

	case IDF_FILE_MODI:
		CompAddItem(FGetFileModi);			// zjištìní èasu modifikace souboru
		return true;

	case IDF_FILE_BYTES:
		CompAddItem(FGetFileByteS);			// naètení èísla bajt se znaménkem
		return true;

	case IDF_FILE_BYTEU:
		CompAddItem(FGetFileByteU);			// naètení èísla bajt bez znaménka
		return true;

	case IDF_FILE_WORDS:
		CompAddItem(FGetFileWordS);			// naètení èísla slovo se znaménkem
		return true;

	case IDF_FILE_WORDU:
		CompAddItem(FGetFileWordU);			// naètení èísla slovo bez znaménka
		return true;

	case IDF_FILE_DWORDS:
		CompAddItem(FGetFileDWordS);		// naètení èísla dvojslovo se znaménkem
		return true;

	case IDF_FILE_DWORDU:
		CompAddItem(FGetFileDWordU);		// naètení èísla dvojslovo bez znaménka
		return true;

	case IDF_FILE_FLOAT:
		CompAddItem(FGetFileFloat);			// naètení èísla FLOAT
		return true;

	case IDF_FILE_DOUBLE:
		CompAddItem(FGetFileDouble);		// naètení èísla DOUBLE
		return true;

	case IDF_FILE_READOFF:
		CompAddItem(FGetFileReadOff);		// offset pro ètení ze souboru
		return true;

	case IDF_FILE_WRITEOFF:
		CompAddItem(FGetFileWriteOff);		// offset pro zápis do souboru
		return true;

	case IDF_DATETIME_CURRENT:
		CompAddItem(FGetTimeCurrent);		// naètení aktuálního èasu
		return true;

	case IDF_DATETIME_UTC:
		return CompNumPar(index, FTimeUtc);	// pøevod na svìtový èas

	case IDF_DATETIME_LOCAL:
		return CompNumPar(index, FTimeLocal); // pøevod na místní èas

	case IDF_DATETIME_COMP:
		CompAddItem(FTimeComp);					// syntéza èasu
		CompNumSubPar(index, IDF_DATETIME_COMP_YEAR, 1601);
		CompNumSubPar(index, IDF_DATETIME_COMP_MONTH, 1);
		CompNumSubPar(index, IDF_DATETIME_COMP_DAY, 1);
		CompNumSubPar(index, IDF_DATETIME_COMP_HOUR, 0);
		CompNumSubPar(index, IDF_DATETIME_COMP_MINUTE, 0);
		CompNumSubPar(index, IDF_DATETIME_COMP_SECONDE, 0);
		CompNumSubPar(index, IDF_DATETIME_COMP_MSECONDE, 0);
		return true;

	case IDF_DATETIME_GET_YEAR:
		return CompNumPar(index, FGetTimeYear);	// naètení roku

	case IDF_DATETIME_GET_MONTH:
		return CompNumPar(index, FGetTimeMonth);// naètení mìsíce

	case IDF_DATETIME_GET_DAY:
		return CompNumPar(index, FGetTimeDay);	// naètení dne

	case IDF_DATETIME_GET_WEEK:
		return CompNumPar(index, FGetTimeWeek);	// naètení dne v týdnu

	case IDF_DATETIME_GET_HOUR:
		return CompNumPar(index, FGetTimeHour);	// naètení hodiny

	case IDF_DATETIME_GET_MINUTE:
		return CompNumPar(index, FGetTimeMin);	// naètení minuty

	case IDF_DATETIME_GET_SECONDE:
		return CompNumPar(index, FGetTimeSec);	// naètení sekundy

	case IDF_DATETIME_GET_MSECONDE:
		return CompNumPar(index, FGetTimeMSec);	// naètení milisekundy

	case IDF_FILE_SIZE:
		CompAddItem(FGetFileSize);				// zjištìní délky souboru
		return true;

	case IDF_FILE_TEXTN_LEN:
		CompAddItem(FGetFileTextNLen);			// délka textu s pevnou délkou
		return true;

	case IDF_FILE_DISK_TYPE:
		CompAddItem(FFileDiskType);				// naètení typu disku
		return true;

	case IDF_FILE_3BYTES:
		CompAddItem(FGetFile3ByteS);			// naètení èísla 3 bajty se znaménkem
		return true;

	case IDF_FILE_3BYTEU:
		CompAddItem(FGetFile3ByteU);			// naètení èísla 3 bajty bez znaménka
		return true;

	case IDF_DIALOG_BOX:
		CompAddItem(FNumDialogBox);				// dialog box s návratem hodnoty
		CompTxtSubPar(index, IDF_DIALOG_BOX_CAPTION);
		CompTxtSubPar(index, IDF_DIALOG_BOX_TEXT);
		CompNumSubPar(index, IDF_DIALOG_BOX_ICON, 0);
		CompNumSubPar(index, IDF_DIALOG_BOX_BUTTON, 0);
		CompNumSubPar(index, IDF_DIALOG_BOX_DEFAULT, 1);
		return true;

	case IDF_SOUND_BIT:
		return CompSndPar(index, FGetSoundBit);	// poèet bitù na vzorek

	case IDF_SOUND_FREQ:
		return CompSndPar(index, FGetSoundFreq);	// vzorkovací kmitoèet

	case IDF_SOUND_LENGTH:
		return CompSndPar(index, FGetSoundLength);	// délka zvuku

	case IDF_SOUND_DISTINGUISH:
		CompAddItem(FGetSoundDistinguish);			// zjištìní rozlišovacího kódu zvuku
		return true;

	case IDF_MAP_WIDTH:
		if (!CompMapPar(index, FGetMapWidth))		// šíøka plochy
		{
			CompAddItem(FGetMapWidth0);
		}
		return true;

	case IDF_MAP_HEIGHT:
		if (!CompMapPar(index, FGetMapHeight))		// výška plochy
		{
			CompAddItem(FGetMapHeight0);
		}
		return true;

	case IDF_MAP_NUM1:
		CompAddItem(FGetMapNum1);			// èíslo 1 z plochy
		CompMapSubPar(index, IDF_MAP_INPUT);
		CompNumSubPar(index, IDF_MAP_X, 0);
		CompNumSubPar(index, IDF_MAP_Y, 0);
		return true;

	case IDF_MAP_NUM2:
		CompAddItem(FGetMapNum2);			// èíslo 2 z plochy
		CompMapSubPar(index, IDF_MAP_INPUT);
		CompNumSubPar(index, IDF_MAP_X, 0);
		CompNumSubPar(index, IDF_MAP_Y, 0);
		return true;

	case IDF_MAP_NUM3:
		CompAddItem(FGetMapNum3);			// èíslo 3 z plochy
		CompMapSubPar(index, IDF_MAP_INPUT);
		CompNumSubPar(index, IDF_MAP_X, 0);
		CompNumSubPar(index, IDF_MAP_Y, 0);
		return true;

	case IDF_SPRITE_WIN_X1:					// levý okraj okna sprajtù
		CompAddItem(FGetSpriteWinX1);
		return true;

	case IDF_SPRITE_WIN_Y1:					// dolní okraj okna sprajtù
		CompAddItem(FGetSpriteWinY1);
		return true;

	case IDF_SPRITE_WIN_X2:					// pravý okraj okna sprajtù
		CompAddItem(FGetSpriteWinX2);
		return true;

	case IDF_SPRITE_WIN_Y2:					// horní okraj okna sprajtù
		CompAddItem(FGetSpriteWinY2);
		return true;

	case IDF_RETURN_CODE:
		CompAddItem(FGetReturnCode);		// návratový kód
		return true;

	case IDF_EXEC_WAIT:
		return CompTxtPar(index, FGetExecWait);	// spuštìní programu s èekáním

	case IDF_JOYSTICK_X:
		if (!CompNumPar(index, FJoystickX))	// souøadnice X joysticku
		{
			CompAddItem(FJoystickX);
			CompAddItemNum((BYTE)0);
		}
		return true;

	case IDF_JOYSTICK_Y:
		if (!CompNumPar(index, FJoystickY))	// souøadnice Y joysticku
		{
			CompAddItem(FJoystickY);
			CompAddItemNum((BYTE)0);
		}
		return true;

	case IDF_JOYSTICK_Z:
		if (!CompNumPar(index, FJoystickZ))	// souøadnice Z joysticku
		{
			CompAddItem(FJoystickZ);
			CompAddItemNum((BYTE)0);
		}
		return true;

	case IDF_WINDOW_ID:
		CompAddItem(FGetWindowID);			// aktivní prvek okna
		return true;

	case IDF_WINDOW_NEW:
		CompAddItem(FWindowNew);			// vytvoøení okna
		return true;

	case IDF_BUTTON_NEW:
		CompAddItem(FButtonNew);			// vytvoøení tlaèítka
		return true;

	case IDF_BUTTON_ESC:
		CompAddItem(FGetButtonEsc);			// tlaèítko pro Esc
		return true;

	case IDF_BUTTONCHECK_NEW:
		CompAddItem(FButtonCheckNew);		// vytvoøení tlaèítka
		return true;

	case IDF_BUTTONCHECK_NEW2:
		CompAddItem(FButtonCheckNew2);		// vytvoøení tlaèítkového pøepínaèe
		return true;

	case IDF_EDIT_NEW:
		CompAddItem(FEditNew);				// vytvoøení editaèního øádku
		return true;

	case IDF_BUTTONRADIO_NEW:
		CompAddItem(FButtonRadioNew);		// vytvoøení tlaèítka
		return true;

	case IDF_BUTTONRADIO_NEW2:
		CompAddItem(FButtonRadioNew2);		// vytvoøení tlaèítka
		return true;

	case IDF_WINDOW_X:
		CompAddItem(FGetWindowX);			// souøadnice X okna
		return true;

	case IDF_WINDOW_Y:
		CompAddItem(FGetWindowY);			// souøadnice Y okna
		return true;

	case IDF_WINDOW_W:
		CompAddItem(FGetWindowW);			// šíøka okna
		return true;

	case IDF_WINDOW_H:
		CompAddItem(FGetWindowH);			// výška okna
		return true;

	case IDF_WINDOW_W_INT:
		CompAddItem(FGetWindowWInt);		// vnitøní šíøka okna
		return true;

	case IDF_WINDOW_H_INT:
		CompAddItem(FGetWindowHInt);		// vnitøní výška okna
		return true;

	case IDF_FONTHEIGHT:
		CompAddItem(FGetFontHeight);		// výška fontù
		return true;

	case IDF_FONTWIDTH:
		CompAddItem(FGetFontWidth);			// šíøka fontù
		return true;

	case IDF_FONTANGLE:
		CompAddItem(FGetFontAngle);			// úhel textu
		return true;

	case IDF_FONTCOL:
		CompAddItem(FGetFontCol);			// barva textu
		return true;

	case IDF_FONTBACK:
		CompAddItem(FGetFontBack);			// barva pozadí
		return true;

	case IDF_MEMO_NEW:
		CompAddItem(FMemoNew);				// vytvoøení editaèního pole
		return true;

	case IDF_RICHEDIT_NEW:
		CompAddItem(FRicheditNew);			// vytvoøení editoru Richedit
		return true;

	case IDF_FOCUS:
		CompAddItem(FGetFocus);				// okno s fokusem
		return true;

	case IDF_DEFBUTTON_NEW:
		CompAddItem(FDefButtonNew);			// vytvoøení implicitního tlaèítka
		return true;

	case IDF_BUTTONICON_NEW:
		CompAddItem(FButtonIconNew);		// vytvoøení tlaèítka s ikonou
		return true;

	case IDF_GROUPBOX_NEW:
		CompAddItem(FGroupBoxNew);			// vytvoøení skupiny
		return true;

	case IDF_LISTBOX_NEW:
		CompAddItem(FListBoxNew);			// vytvoøení seznamu
		return true;

	case IDF_TABS_NEW:
		CompAddItem(FTabsNew);				// vytvoøení záložek
		return true;

	case IDF_WINTEXT_NEW:
		CompAddItem(FWinTextNew);			// vytvoøení textu
		return true;

	case IDF_WINTEXT_NEW_CENTER:
		CompAddItem(FWinTextNewCenter);		// vytvoøení textu vystøedìného
		return true;

	case IDF_WINTEXT_NEW_RIGHT:
		CompAddItem(FWinTextNewRight);		// vytvoøení textu zarovnaného vpravo
		return true;

	case IDF_COMBOBOX_NEW:
		CompAddItem(FComboBoxNew);			// vytvoøení seznamu
		return true;

	case IDF_COMBOEDIT_NEW:
		CompAddItem(FComboEditNew);			// vytvoøení seznamu
		return true;

	case IDF_LISTEDIT_NEW:
		CompAddItem(FListEditNew);			// vytvoøení seznamu
		return true;

	case IDF_BUTTON3_NEW:
		CompAddItem(FButton3New);			// vytvoøení tlaèítka
		return true;

	case IDF_WINDOWRO_NEW:
		CompAddItem(FWindowRONew);			// vytvoøení okna
		return true;

	case IDF_WINICON_NEW:
		CompAddItem(FWinIconNew);			// vytvoøení ikony
		return true;

	case IDF_WINPIC_NEW:
		CompAddItem(FWinPicNew);			// vytvoøení obrázku
		return true;

	case IDF_2PI:
		CompAddItemNum(pi2);				// úhel 360 stupòù
		return true;

	case IDF_BOD32:
		CompAddItemNum((double)1/32);		// vzdálenost 1 bodu
		return true;

	case IDF_M1:
		CompAddItemNum((double)-1);			// úhel 360 stupòù
		return true;

	case IDF_SOUND_POS:
		CompAddItem(FGetSoundPos);			// zjištìní pozice zvuku
		return true;

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_DLL_LOAD:						// naètení DLL knihovny
		return CompTxtPar(index, FDLLLoad);

	case IDF_DLL_IMPORT:					// importování funkce DLL knihovny
		CompAddItem(FDLLImport);
		CompNumSubPar(index, IDF_DLL_IMPORT_LIB, 0);
		CompTxtSubPar(index, IDF_DLL_IMPORT_NAME);
		CompNumSubPar(index, IDF_DLL_IMPORT_PAR, 0);
		CompLogSubPar(index, IDF_DLL_IMPORT_C, false);
		return true;

	case IDF_DLL_EXEC:						// provedení funkce DLL
		return CompNumPar(index, FDLLExecNum);

	case IDF_MEMORY_NEW:					// vytvoøení pamìového bloku
		return CompNumPar(index, FMemoryNew);

	case IDF_MEMORY_RESIZE:					// zmìna velikosti pamìového bloku
		CompAddItem(FMemoryResize);
		CompNumSubPar(index, IDF_MEMORY_RESIZE_ADR, 0);
		CompNumSubPar(index, IDF_MEMORY_RESIZE_NEW, 0);
		return true;

	case IDF_MEMORY_READ:					// ukazatel ètení z pamìti
		CompAddItem(FGetMemoryRead);
		return true;

	case IDF_MEMORY_WRITE:					// ukazatel zápisu do pamìti
		CompAddItem(FGetMemoryWrite);
		return true;

	case IDF_MEMORY_COMPARE:				// porovnání dat
		return CompNumPar(index, FMemoryCompare);

	case IDF_MEMORY_BYTES:					// bajt se znaménkem
		CompAddItem(FGetMemoryByteS);
		return true;

	case IDF_MEMORY_BYTEU:					// bajt bez znaménka
		CompAddItem(FGetMemoryByteU);
		return true;

	case IDF_MEMORY_WORDS:					// slovo se znaménkem
		CompAddItem(FGetMemoryWordS);
		return true;

	case IDF_MEMORY_WORDU:					// slovo bez znaménka
		CompAddItem(FGetMemoryWordU);
		return true;

	case IDF_MEMORY_3BYTES:					// 3-bajt se znaménkem
		CompAddItem(FGetMemory3ByteS);
		return true;

	case IDF_MEMORY_3BYTEU:					// 3-bajt bez znaménka
		CompAddItem(FGetMemory3ByteU);
		return true;

	case IDF_MEMORY_DWORDS:					// dvouslovo se znaménkem
		CompAddItem(FGetMemoryDWordS);
		return true;

	case IDF_MEMORY_DWORDU:					// dvouslovo bez znaménka
		CompAddItem(FGetMemoryDWordU);
		return true;

	case IDF_MEMORY_FLOAT:					// reálné èíslo 4 bajty
		CompAddItem(FGetMemoryFloat);
		return true;

	case IDF_MEMORY_DOUBLE:					// reálné èíslo 8 bajtù
		CompAddItem(FGetMemoryDouble);
		return true;

	case IDF_MEMORY_TEXTN_LEN:				// délka textu s pevnou délkou
		CompAddItem(FGetMemoryTextNLen);
		return true;

	case IDF_WINDOW_HANDLE:					// handle okenního prvku
		return CompNumPar(index, FWindowHandle);

	case IDF_MEMORY_3D:						// vytvoøení objektu z bloku
		CompAddItem(FMemory3D);
		CompNumSubPar(index, IDF_MEMORY_3D_VN, 0);
		CompNumSubPar(index, IDF_MEMORY_3D_VERT, 0);
		CompNumSubPar(index, IDF_MEMORY_3D_NORM, 0);
		CompNumSubPar(index, IDF_MEMORY_3D_TEXT, 0);
		CompNumSubPar(index, IDF_MEMORY_3D_COL, 0);
		CompNumSubPar(index, IDF_MEMORY_3D_FN, 0);
		CompNumSubPar(index, IDF_MEMORY_3D_FACE, 0);
		return true;

	case IDF_INI_NUM:						// ètení konfiguraèního èísla
		if (!CompNumPar(index, FGetIniNum))
		{
			CompAddItem(FGetIniNum0);
		}
		return true;

	case IDF_REG_KEY:						// skupina klíèù registrù
		CompAddItem(FGetRegKey);
		return true;

	case IDF_REG_NUM:						// ètení èísla z registru
		if (!CompNumPar(index, FGetRegNum))
		{
			CompAddItem(FGetRegNum0);
		}
		return true;

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------


// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_GAME_N1:
		CompAddItem(FGetGameN1);			// naètení herního èísla 1
		return true;

	case IDF_GAME_N2:
		CompAddItem(FGetGameN2);			// naètení herního èísla 2
		return true;

	case IDF_GAME_N3:
		CompAddItem(FGetGameN3);			// naètení herního èísla 3
		return true;

	case IDF_GAME_N4:
		CompAddItem(FGetGameN4);			// naètení herního èísla 4
		return true;

	case IDF_GAME_N5:
		CompAddItem(FGetGameN5);			// naètení herního èísla 5
		return true;

	case IDF_GAME_N6:
		CompAddItem(FGetGameN6);			// naètení herního èísla 6
		return true;

	case IDF_GAME_N7:
		CompAddItem(FGetGameN7);			// naètení herního èísla 7
		return true;

	case IDF_GAME_N8:
		CompAddItem(FGetGameN8);			// naètení herního èísla 8
		return true;

	case IDF_PLAYER_MAX:
		CompAddItem(FGetPlayerMax);			// max. hráèù
		return true;

	case IDF_PLAYER_INX:
		CompAddItem(FPlayerInx);			// index hráèe
		return true;

	case IDF_GAME_PACKET:
		CompAddItem(FGetGamePacket);		// pøíjem paketu
		return true;

	case IDF_GAME_DATAN:
		CompAddItem(FGetGameDataN);			// naètení èísla z paketu
		return true;

	case IDF_GAME_DATAB:
		CompAddItem(FGetGameDataB);			// naètení bajtu z paketu
		return true;

	case IDF_GRAPH_AREA_WIDTH:
		CompAddItem(FGetGraphAreaWidth);	// šíøka k zobrazení centrovaného textu
		return true;

	case IDF_UDP_SEND_PORT:
		CompAddItem(FGetUDPSendPort);		// zjištìní vysílacího portu UDP
		return true;

	case IDF_UDP_REC_SIZE:
		CompAddItem(FGetUDPRecSize);		// zjištìní velikosti pøijímacího bufferu UDP
		return true;

	case IDF_UDP_REC_PORT:
		CompAddItem(FGetUDPRecPort);		// zjištìní pøijatého portu UDP
		return true;

	case IDF_UDP_MEM:
		CompAddItem(FGetUDPMem);			// pøíjem datového bloku UDP
		return true;

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------


	case IDF_PICTURE_WIDTH:
		return CompPicPar(index, FGetPicWidth);		// šíøka obrázku

	case IDF_PICTURE_HEIGHT:
		return CompPicPar(index, FGetPicHeight);	// výška obrázku

	case IDF_BIT_AND:
		return CompNumGrp(index, FBitAnd, FBitAnd);	// operátor BIT-AND

	case IDF_BIT_OR:
		return CompNumGrp(index, FBitOr, FBitOr);	// operátor BIT-OR

	case IDF_BIT_XOR:
		return CompNumGrp(index, FBitXor, FBitXor);	// operátor BIT-XOR

	case IDF_BIT_NOT:
		return CompNumPar(index, FBitNot);			// bitový doplnìk

	case IDF_BIT_MAX:
		CompAddItemNum((double)(DWORD)0xffffffff);	// max. bitové èíslo
		return true;

	case IDF_BIT_LEFT:
		return CompNumPar(index, FBitLeft);			// bitová rotace vlevo

	case IDF_BIT_RIGHT:
		return CompNumPar(index, FBitRight);		// bitová rotace vpravo

	case IDF_CODEPAGE:
		CompAddItem(FGetCodePage);					// zjištìní znakové sady
		return true;

	case IDF_LANGUAGE:
		CompAddItem(FGetLanguage);					// zjištìní jazyku
		return true;

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_COM_OPEN:								// otevøení portu
		return CompTxtPar(index, FComOpen);

	case IDF_COM_ID:
		CompAddItem(FGetComID);						// aktivní port
		return true;

	case IDF_COM_BAUD:
		CompAddItem(FGetComBaud);					// zjištìní pøenosové rychlosti portu
		return true;

	case IDF_COM_BITS:
		CompAddItem(FGetComBits);					// zjištìní bitù portu
		return true;

	case IDF_COM_PARIT:
		CompAddItem(FGetComParit);					// zjištìní parity
		return true;

	case IDF_COM_NUM:
		CompAddItem(FGetComNum);					// naètení èísla
		return true;

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------

	case IDF_RICHMEMO_NEW:
		CompAddItem(FRichMemoNew);				// vytvoøení editaèního pole bez zalamování
		return true;

	case IDF_PROGRESS_NEW:
		CompAddItem(FProgressNew);				// vytvoøení indikátoru prùbìhu
		return true;

	case IDF_HTRACKBAR_NEW:
		CompAddItem(FHTrackbarNew);				// vytvoøení horizontálního táhla
		return true;

	case IDF_VTRACKBAR_NEW:
		CompAddItem(FVTrackbarNew);				// vytvoøení vertikálního táhla
		return true;

	case IDF_HSCROLLBAR_NEW:
		CompAddItem(FHScrollbarNew);			// vytvoøení horizontálního posuvníku
		return true;

	case IDF_VSCROLLBAR_NEW:
		CompAddItem(FVScrollbarNew);			// vytvoøení vertikálního posuvníku
		return true;

	case IDF_WINDOW_POS:
		CompAddItem(FGetWindowPos);				// naètení pozice prvku
		return true;

	case IDF_WINDOW_POS2:
		CompAddItem(FGetWindowPos2);				// naètení pozice 2 prvku
		return true;

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_MIX_LINE_TYPE:						// zjištìní typu signálu
		CompAddItem(FGetLineType);
		return true;

	case IDF_MIX_CONTROL_TYPE:					// zjištìní typu ovládacího prvku
		CompAddItem(FGetCtrlType);
		return true;
	
	case IDF_MIX_CONTROL_VAL:					// zjištìní hodnoty ovládacího prvku
		CompAddItem(FGetCtrlVal);
		return true;

	case IDF_MIX_CHANNEL:						// zjištìní poètu kanálù prvku
		CompAddItem(FGetMixChannel);
		return true;

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------


// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_DIRECT3D_OCULLING:					// test nastavení odstraòování ploch objektu Direct3D
		CompAddItem(FGetD3DOCulling);
		return true;

	case IDF_DIRECT3D_WALL:						// vytvoøení zdi Direct3D
		CompAddItem(FD3DWall);
		return true;

	case IDF_DIRECT3D_CUBE:						// vytvoøení krychle Direct3D
		CompAddItem(FD3DCube);
		return true;

	case IDF_DIRECT3D_SPHERE:					// vytvoøení koule Direct3D
		CompAddItem(FD3DSphere);
		return true;

	case IDF_DIRECT3D_CYLINDER:					// vytvoøení válce Direct3D
		CompAddItem(FD3DCylinder);
		return true;

	case IDF_DIRECT3D_CONE:						// vytvoøení kužele Direct3D
		CompAddItem(FD3DCone);
		return true;

	case IDF_DIRECT3D_CONE2:					// vytvoøení komolého kužele Direct3D
		return CompNumPar(index, FD3DCone2);

	case IDF_DIRECT3D_CLONE:					// klonování objektu Direct3D
		return CompNumPar(index, FD3DClone);

	case IDF_DIRECT3D_TEXT:						// objekt Direct3D z textu
		return CompTxtPar(index, FD3DText);

	case IDF_DIRECT3D_FILE:						// objekt Direct3D ze souboru
		return CompTxtPar(index, FD3DFile);

	case IDF_DIRECT3D_OBJECT_ID:				// zjištìní ID aktivního objektu Direct3D
		CompAddItem(FGetD3DObjectID);
		return true;

	case IDF_DIRECT3D_SCALEX:					// zjištìní mìøítka ve smìru X
		CompAddItem(FGetD3DScaleX);
		return true;

	case IDF_DIRECT3D_SCALEY:					// zjištìní mìøítka ve smìru Y
		CompAddItem(FGetD3DScaleY);
		return true;

	case IDF_DIRECT3D_SCALEZ:					// zjištìní mìøítka ve smìru Z
		CompAddItem(FGetD3DScaleZ);
		return true;

	case IDF_DIRECT3D_ROTATEX:					// zjištìní rotace podle osy X
		CompAddItem(FGetD3DRotateX);
		return true;

	case IDF_DIRECT3D_ROTATEY:					// zjištìní rotace podle osy Y
		CompAddItem(FGetD3DRotateY);
		return true;

	case IDF_DIRECT3D_ROTATEZ:					// zjištìní rotace podle osy Z
		CompAddItem(FGetD3DRotateZ);
		return true;

	case IDF_DIRECT3D_TRANSX:					// zjištìní posunu ve smìru X
		CompAddItem(FGetD3DTransX);
		return true;

	case IDF_DIRECT3D_TRANSY:					// zjištìní posunu ve smìru Y
		CompAddItem(FGetD3DTransY);
		return true;

	case IDF_DIRECT3D_TRANSZ:					// zjištìní posunu ve smìru Z
		CompAddItem(FGetD3DTransZ);
		return true;

	case IDF_DIRECT3D_COLOR:					// zjištìní barvy objektu
		CompAddItem(FGetD3DColor);
		return true;

	case IDF_DIRECT3D_EMISSIVE:					// zjištìní barvy svítivosti
		CompAddItem(FGetD3DEmissive);
		return true;

	case IDF_DIRECT3D_POWER:					// zjištìní matnosti objektu
		CompAddItem(FGetD3DPower);
		return true;

	case IDF_DIRECT3D_SPECULAR:					// zjištìní barvy odlesku
		CompAddItem(FGetD3DSpecular);
		return true;

	case IDF_DIRECT3D_LIGHT:					// vytvoøení svìtla Direct3D
		CompAddItem(FD3DLight);
		return true;

	case IDF_DIRECT3D_LIGHT_COLOR:				// zjištìní barvy svìtla Direct3D
		CompAddItem(FGetD3DLightColor);
		return true;

	case IDF_DIRECT3D_LIGHT_TYPE:				// zjištìní typu svìtla Direct3D
		CompAddItem(FGetD3DLightType);
		return true;

	case IDF_DIRECT3D_LIGHT_UMBRA:				// zjištìní úhle stínu svìtla Direct3D
		CompAddItem(FGetD3DLightUmbra);
		return true;

	case IDF_DIRECT3D_LIGHT_PENUMBRA:			// zjištìní úhle polostínu svìtla Direct3D
		CompAddItem(FGetD3DLightPenumbra);
		return true;

	case IDF_DIRECT3D_LIGHT_RANGE:				// zjištìní dosahu svìtla Direct3D
		CompAddItem(FGetD3DLightRange);
		return true;

	case IDF_DIRECT3D_LIGHT_CATTEN:				// zjištìní konstantního útlumu svìtla Direct3D
		CompAddItem(FGetD3DLightCatten);
		return true;

	case IDF_DIRECT3D_LIGHT_LATTEN:				// zjištìní lineárního útlumu svìtla Direct3D
		CompAddItem(FGetD3DLightLatten);
		return true;

	case IDF_DIRECT3D_LIGHT_QATTEN:				// zjištìní kvadratického útlumu svìtla Direct3D
		CompAddItem(FGetD3DLightQatten);
		return true;

	case IDF_DIRECT3D_PROJECTION:				// typ projekce
		CompAddItem(FGetD3DProjection);
		return true;

	case IDF_DIRECT3D_SCENE_COLOR:				// zjištìní barvy pozadí scény
		CompAddItem(FGetD3DSceneColor);
		return true;

	case IDF_DIRECT3D_FOG_COLOR:				// zjištìní barvy mlhy
		CompAddItem(FGetD3DFogColor);
		return true;

	case IDF_DIRECT3D_FOG_MODE:					// zjištìní módu mlhy
		CompAddItem(FGetD3DFogMode);
		return true;

	case IDF_DIRECT3D_FOG_START:				// zjištìní zaèátku mlhy
		CompAddItem(FGetD3DFogStart);
		return true;

	case IDF_DIRECT3D_FOG_END:					// zjištìní konce mlhy
		CompAddItem(FGetD3DFogEnd);
		return true;

	case IDF_DIRECT3D_FOG_DENSITY:				// zjištìní hustoty mlhy
		CompAddItem(FGetD3DFogDensity);
		return true;

	case IDF_DIRECT3D_VIEW_FRONT:				// zjištìní promítací roviny
		CompAddItem(FGetD3DViewFront);
		return true;

	case IDF_DIRECT3D_VIEW_BACK:				// zjištìní vzdálenosti dohledu
		CompAddItem(FGetD3DViewBack);
		return true;

//	case IDF_DIRECT3D_PICK:
//		CompAddItem(FD3DPick);							// vyhledání objektu
//		CompNumSubPar(index, IDF_DIRECT3D_PICK_X, 0x1000000);
//		CompNumSubPar(index, IDF_DIRECT3D_PICK_Y, 0x1000000);
//		return true;

	case IDF_DIRECT3D_2DOBJECT:
		return CompPicPar(index, FD3D2DObject);		// vytvoøení 2D objektu

	case IDF_DIRECT3D_TERRAIN:
		return CompPicPar(index, FD3D2DTerrain);	// vytvoøení terénu

	case IDF_DIRECT3D_LEVEL:
		CompAddItem(FD3DLevel);							// zjištìní úrovnì terénu
		CompNumSubPar(index, IDF_DIRECT3D_LEVEL_X, 0x1000000);
		CompNumSubPar(index, IDF_DIRECT3D_LEVEL_Y, 0x1000000);
		return true;

	case IDF_DIRECT3D_EMPTY:						// vytvoøení prázdného objektu Direct3D
		CompAddItem(FD3DEmpty);
		return true;

	case IDF_DIRECT3D_SPLITS:						// zjištìní složitosti objektù
		CompAddItem(FGetD3DSplits);
		return true;

	case IDF_DIRECT3D_ORDER:						// zjištìní poøadí rotací objektu
		CompAddItem(FGetD3DOrder);
		return true;

	case IDF_DIRECT3D_DRIVER:						// zjištìní aktivního ovladaèe
		CompAddItem(FGetD3DDriver);
		return true;

	case IDF_DIRECT3D_INTERFACE:					// zjištìní aktivního rozhraní
		CompAddItem(FGetD3DInterface);
		return true;

	case IDF_DIRECT3D_ROTARY:						// vytvoøení rotaèního objektu
		return CompTxtPar(index, FD3DRotary);

	case IDF_DIRECT3D_TERRAIN_MAP:
		return CompMapPar(index, FD3DTerrainMap);	// vytvoøení terénu z plochy

	case IDF_DIRECT3D_SMOOTH:						// zjištìní stupnì vyhlazení textur
		CompAddItem(FGetD3DSmooth);
		return true;

	case IDF_DIRECT3D_AMBIENT:						// zjištìní ambient svìtla
		CompAddItem(FGetD3DAmbient);
		return true;

	case IDF_DIRECT3D_CIRCLE:						// vytvoøení kruhu Direct3D
		CompAddItem(FD3DCircle);
		return true;

	case IDF_DIRECT3D_TRIANGLE:						// vytvoøení trojúhelníku Direct3D
		CompAddItem(FD3DTriangle);
		return true;

	case IDF_DIRECT3D_TORUS:						// vytvoøení torusu Direct3D
		return CompNumPar(index, FD3DTorus);

	case IDF_DIRECT3D_2DSTATIC:
		return CompPicPar(index, FD3D2DStatic);		// vytvoøení statického 2D objektu

	case IDF_DIRECT3D_HEMISPHERE:					// vytvoøení polokoule Direct3D
		CompAddItem(FD3DHemisphere);
		return true;

	case IDF_DIRECT3D_TRANSPARENT:					// zjištìní prùhlednosti textury
		CompAddItem(FGetD3DTransparent);
		return true;

	case IDF_DIRECT3D_ALPHAREF:						// zjištìní referenèní úrovnì alfa kanálu
		CompAddItem(FGetD3DAlphaRef);
		return true;

	case IDF_DIRECT3D_AMBIREF:						// zjištìní odrazivosti rozptýleného svìtla
		CompAddItem(FGetD3DAmbiRef);
		return true;

	case IDF_DIRECT3D_ELAPSED:
		if (!CompNumPar(index, FD3DElapsed))		// uplynulý èas s násobkem
		{
			CompAddItem(FD3DElapsed0);				// uplynulý èas bez násobku
		}
		return true;

	case IDF_DIRECT3D_SPEED:						// mìøení rychlosti renderování
		CompAddItem(FD3DSpeed);
		return true;

	case IDF_DIRECT3D_FREE:							// volná videopamì
		CompAddItem(FD3DFree);
		return true;

	case IDF_DIRECT3D_PICTURE:
		return CompPicPar(index, FD3DPicture);		// vytvoøení 2D obrázku

	case IDF_DIRECT3D_LENSFLARE:
		return CompPicPar(index, FD3DLensFlare);	// vytvoøení èoèkového odlesku

	case IDF_DIRECT3D_STAGE:						// stupeò textur
		CompAddItem(FGetD3DStage);
		return true;

	case IDF_DIRECT3D_COLOROP:						// operace barev
		CompAddItem(FGetD3DColorOp);
		return true;

	case IDF_DIRECT3D_COLORARG1:					// argument 1 barev
		CompAddItem(FGetD3DColorArg1);
		return true;

	case IDF_DIRECT3D_COLORARG2:					// argument 2 barev
		CompAddItem(FGetD3DColorArg2);
		return true;

	case IDF_DIRECT3D_ALPHAOP:						// operace alfa
		CompAddItem(FGetD3DAlphaOp);
		return true;

	case IDF_DIRECT3D_ALPHAARG1:					// argument 1 alfa
		CompAddItem(FGetD3DAlphaArg1);
		return true;

	case IDF_DIRECT3D_ALPHAARG2:					// argument 2 alfa
		CompAddItem(FGetD3DAlphaArg2);
		return true;

	case IDF_DIRECT3D_ADDRESSU:						// horizontální adresování
		CompAddItem(FGetD3DAddressU);
		return true;

	case IDF_DIRECT3D_ADDRESSV:						// vertikální adresování
		CompAddItem(FGetD3DAddressV);
		return true;

//	case IDF_DIRECT3D_BORDER:						// barva okolí textur
//		CompAddItem(FGetD3DBorder);
//		return true;

	case IDF_DIRECT3D_BIAS:							// zjemnìní vzdálených textur
		CompAddItem(FGetD3DBias);
		return true;

	case IDF_DIRECT3D_TFACTOR:						// faktor textur
		CompAddItem(FGetD3DTFactor);
		return true;

	case IDF_DIRECT3D_MIPMAPS:						// poèet úrovní mipmaps
		CompAddItem(FGetD3DMipMaps);
		return true;

	case IDF_DIRECT3D_ZGROUP:						// renderovací skupina
		CompAddItem(FGetD3DZGroup);
		return true;

	case IDF_DIRECT3D_ZTEST:						// hloubkový test
		CompAddItem(FGetD3DZTest);
		return true;

	case IDF_SHADOW_RANGE:							// dosah stínu
		CompAddItem(FGetD3DShadowRange);
		return true;

	case IDF_DIRECT3D_LOD_OBJECT:					// objekt pro LOD
		CompAddItem(FGetD3DLODObject);
		return true;

	case IDF_DIRECT3D_LOD_DISTANCE:					// vzdálenost pro LOD
		CompAddItem(FGetD3DLODDistance);
		return true;

	case IDF_DIRECT3D_MORPH_MODEL:					// zjištìní max. stupnì morfování
		CompAddItem(FGetD3DMorphModel);
		return true;

	case IDF_DIRECT3D_MORPH_LEVEL:					// zjištìní stupnì morfování
		CompAddItem(FGetD3DMorphLevel);
		return true;

	case IDF_DIRECT3D_MEMORY:						// pamìový blok z 3D prvku
		return CompNumPar(index, FD3DMemory);

	case IDF_DIRECT3D_CHILD:						// nalezení potomka 3D objektu
		if (!CompNumPar(index, FD3DChild))			// zadaný potomek
		{
			CompAddItem(FD3DChild0);				// první potomek
		}
		return true;

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------

	case IDF_POLAR_RADIUS:
		CompAddItem(FPolarRadius);					// výpoèet vzdálenosti bodu
		CompNumSubPar(index, IDF_POLAR_X, 0);
		CompNumSubPar(index, IDF_POLAR_Y, 0);
		return true;

	case IDF_POLAR_ANGLE:
		CompAddItem(FPolarAngle);					// výpoèet úhlu bodu
		CompNumSubPar(index, IDF_POLAR_X, 0);
		CompNumSubPar(index, IDF_POLAR_Y, 0);
		return true;

	case IDF_PRECISE_TIMER:							// pøesný èítaè èasu
		CompAddItem(FPreciseTimer);
		return true;

	case IDF_TABLE_NEW:
		return CompTxtPar(index, FTableNew);		// vytvoøení tabulky

	case IDF_TABLE_WIDTH:
		CompAddItem(FGetTableWidth);				// zjištìní šíøky sloupce tabulky
		return true;

	case IDF_TABLE_ALIGN:
		CompAddItem(FGetTableAlign);				// zjištìní zarovnání sloupce tabulky
		return true;

	default:

// pøeklad kódu klávesy
		{
			int fn = item->Func + IDF - CompTabVKBeg;
			if ((DWORD)fn < (DWORD)(CompTabVKEnd - CompTabVKBeg + 1))
			{
				CompAddItemNum(CompTabVK[fn]);	// kód klávesy
				return true;
			}
		}
		return false;
	}
}


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu s èíselným parametrem

bool CompNumPar(int index, PROCCOM func)
{
	return CompNumPar(index, func, 0, -1);
}

bool CompNumPar(int index, PROCCOM func, int data)
{
	return CompNumPar(index, func, data, -1);
}

bool CompNumPar(int index, PROCCOM func, int data, int list)
{
	int zahlavi = CompAddItem(func, data, list);

	if ((BufEdi2[index].Items == 1) || !CompNum(index+1))
	{
		ProgNum = zahlavi;
		return false;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu s èíselným podparametrem (hledaným podle identifikace)

void CompNumSubPar(int index, int idf, double def)
{
// korekce identifikace funkce
	idf -= IDF;

// ukazatel výchozího prvku
	PETPROG*	item = BufEdi + index;
	PETPROG2*	item2 = BufEdi2 + index;

// kontrola, zda má položka nìjaké potomky
	if (item->Param & PETPROG_CHILDS)
	{
		int posun = 1;

// cyklus pøes všechny potomky
		do {

// adresa dalšího potomka
			index += posun;
			item += posun;
			item2 += posun;

// test, zda to je hledaný prvek - naètení prvku
			if ((item->Func == idf) &&
				(item->Param & PETPROG_CHILDS) && 
				CompNum(index + 1))
			{
				return;
			}

// posun pro pøíští prvek
			posun = item2->Items;

// dokud je další potomek
		} while (item->Param & PETPROG_NEXT);
	}

// použije se implicitní hodnota
	CompAddItemNum(def);
}
