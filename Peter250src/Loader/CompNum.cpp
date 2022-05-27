
#include "Main.h"

/***************************************************************************\
*																			*
*					Kompilace programu - ��seln� v�raz						*
*																			*
\***************************************************************************/

#pragma optimize("s", on)			// optimalizace na minim�ln� velikost


/////////////////////////////////////////////////////////////////////////////
// p�id�n� ��seln� konstanty do bufferu programu

void CompAddItemNum(double data)
{
// vytvo�en� polo�ky
	int index = CompAddItem(FNumConst);

// nastaven� dat
	ProgBuf[index].Double = data;
}


/////////////////////////////////////////////////////////////////////////////
// p�id�n� k�du kl�vesy do bufferu programu
 
void _fastcall CompAddItemNum(BYTE data)
{
// statick� prom�nn� pro p�evod
	static int tmp = 0;

// p�evod na ��slo INT
	*(BYTE*)&tmp = data;

// vytvo�en� polo�ky a nastaven� dat
	ProgBuf[CompAddItem(FNumConst)].Double = tmp;
}


/////////////////////////////////////////////////////////////////////////////
// tabulka ��dic�ch k�d� kl�ves (zajistit spojitost k�d� kl�ves IDF !!!!)

#define CompTabVKBeg IDF_F1			// prvn� k�d tabulky
#define CompTabVKEnd IDF_VK_SPACE	// posledn� k�d tabulky

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
// p�eklad ��seln�ho v�razu (vrac� true = operace OK)

bool _fastcall CompNum(int index)
{
// kontrola tabulky k�d� kl�ves
#pragma warning ( disable: 4127)				// hl�en� - konstantn� podm�nka
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
#pragma warning ( default: 4127)				// hl�en� - konstantn� podm�nka

// adresa zdrojov�ho prvku
	if ((DWORD)index >= (DWORD)BufEdiN) return false;
	PETPROG*	item = BufEdi + index;
	PETPROG2*	item2 = BufEdi2 + index;
	int refinx = item->RefIndex;
	PETPROG2*	prg2;

// kontrola, zda je polo�ka vypnuta
	if ((item->Param & (PETPROG_OFF | PETPROG_OFF_DEP)) != 0) return false;

// v�tven� podle funkce
	switch (item->Func + IDF)
	{
	case IDF_NUM:
		CompAddItemNum(LoadNum(item, item2));	// ��seln� konstanta
		return true;

	case IDF_0:
		CompAddItemNum((BYTE)0);				// ��slo 0
		return true;

	case IDF_1:
		CompAddItemNum((BYTE)1);				// ��slo 1
		return true;

	case IDF_2:
		CompAddItemNum((BYTE)2);				// ��slo 2
		return true;

	case IDF_3:
		CompAddItemNum((BYTE)3);				// ��slo 3
		return true;

	case IDF_4:
		CompAddItemNum((BYTE)4);				// ��slo 4
		return true;

	case IDF_5:
		CompAddItemNum((BYTE)5);				// ��slo 5
		return true;

	case IDF_6:
		CompAddItemNum((BYTE)6);				// ��slo 6
		return true;

	case IDF_7:
		CompAddItemNum((BYTE)7);				// ��slo 7
		return true;

	case IDF_8:
		CompAddItemNum((BYTE)8);				// ��slo 8
		return true;

	case IDF_9:
		CompAddItemNum((BYTE)9);				// ��slo 9
		return true;

	case IDF_ADD:
		return CompNumGrp(index, FAdd, FAdd);	// oper�tor ADD

	case IDF_SUB:
		return CompNumGrp(index, FSub, FSub1);	// oper�tor SUB

	case IDF_MUL:
		return CompNumGrp(index, FMul, FMul);	// oper�tor MUL

	case IDF_DIV:
		return CompNumGrp(index, FDiv, FDiv1);	// oper�tor DIV

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
		return CompFunc(index, IDF_REAL);		// funkce s n�vratem ��sla

	case IDF_SMER:
		CompAddItem(FGetSmer);					// Petr - sm�r
		return true;

	case IDF_POZICEX:
		CompAddItem(FGetPozX);					// Petr - pozice X
		return true;

	case IDF_POZICEY:
		CompAddItem(FGetPozY);					// Petr - pozice Y
		return true;

	case IDF_PETRA_SMER:
		CompAddItem(FGetSmer2);					// Petra - sm�r
		return true;

	case IDF_PETRA_POZICEX:
		CompAddItem(FGetPozX2);					// Petra - pozice X
		return true;

	case IDF_PETRA_POZICEY:
		CompAddItem(FGetPozY2);					// Petra - pozice Y
		return true;

	case IDF_MOD:
		return CompNumGrp(index, FMod, FMod);	// oper�tor MOD

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
		CompAddItemNum(pi);						// Ludolfovo ��slo
		return true;

	case IDF_INTEGER:
		return CompNumPar(index, FNumInt);		// cel� ��slo

	case IDF_FRAC:
		return CompNumPar(index, FFrac);		// desetinn� ��st ��sla

	case IDF_ABS:
		return CompNumPar(index, FAbs);			// absolutn� hodnota

	case IDF_SQRT:
		return CompNumPar(index, FSqrt);		// odmocnina

	case IDF_FACT:
		return CompNumPar(index, FFact);		// faktori�l

	case IDF_RAND:
		if (!CompNumPar(index, FRand))			// n�hodn� ��slo s n�sobkem
		{
			CompAddItem(FRand0);				// n�hodn� ��slo bez n�sobku
		}
		return true;

	case IDF_EUL:
		CompAddItemNum(eul);					// Eulerova konstanta
		return true;

	case IDF_DEGRAD:
		return CompNumPar(index, FDegRad);		// p�evod stup�� na radi�ny

	case IDF_RADDEG:
		return CompNumPar(index, FRadDeg);		// p�evod radi�n� na stupn�

	case IDF_LOG:
		return CompNumPar(index, FLog);			// dekadick� logaritmus

	case IDF_LN:
		return CompNumPar(index, FLn);			// p�irozen� logaritmus

	case IDF_PEXP:
		return CompNumPar(index, FExp);			// p�irozen� exponent

	case IDF_MOCN:
		return CompNumGrp(index, FMocn, FMocn);	// oper�tor MOCNINA

	case IDF_UHEL0:
		CompAddItemNum((BYTE)0);				// �hel 0 stup��
		return true;

	case IDF_UHEL45:
		CompAddItemNum(uhel45);					// �hel 45 stup��
		return true;

	case IDF_UHEL90:
		CompAddItemNum(uhel90);					// �hel 90 stup��
		return true;

	case IDF_UHEL135:
		CompAddItemNum(uhel135);				// �hel 135 stup��
		return true;

	case IDF_UHEL180:
		CompAddItemNum(uhel180);				// �hel 180 stup��
		return true;

	case IDF_UHEL225:
		CompAddItemNum(uhel225);				// �hel 225 stup��
		return true;

	case IDF_UHEL270:
		CompAddItemNum(uhel270);				// �hel 270 stup��
		return true;

	case IDF_UHEL315:
		CompAddItemNum(uhel315);				// �hel 315 stup��
		return true;

	case IDF_KLAV_VKEY:
		CompAddItem(FVKey);						// na�ten� kl�vesy bez �ek�n�
		return true;

	case IDF_MOUSE_X:
		CompAddItem(FMouseX);					// na�ten� sou�adnice X my�i
		return true;

	case IDF_MOUSE_Y:
		CompAddItem(FMouseY);					// na�ten� sou�adnice Y my�i
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
		CompAddItem(FGetSwcN1);					// na�ten� ��sla 1 - Petr
		return true;

	case IDF_PETRA_SWCN1:
		CompAddItem(FGetSwcN12);				// na�ten� ��sla 1 - Petra
		return true;

	case IDF_PETER_SWCN2:
		CompAddItem(FGetSwcN2);					// na�ten� ��sla 2 - Petr
		return true;

	case IDF_PETRA_SWCN2:
		CompAddItem(FGetSwcN22);				// na�ten� ��sla 2 - Petra
		return true;

	case IDF_PETER_SWCN3:
		CompAddItem(FGetSwcN3);					// na�ten� ��sla 3 - Petr
		return true;

	case IDF_PETRA_SWCN3:
		CompAddItem(FGetSwcN32);				// na�ten� ��sla 3 - Petra
		return true;

	case IDF_TEXT_LEN:
		return CompTxtPar(index, FTextLen);	// d�lka textu

	case IDF_TEXT_SCAN:
		return CompTxtPar(index, FTextScan);// p�evod textu na ��slo

	case IDF_TEXT_FIND:
		CompAddItem(FTextFind);				// vyhled�n� textu
		CompTxtSubPar(index, IDF_TEXT_FIND_SUB);
		CompTxtSubPar(index, IDF_TEXT_INPUT);
		return true;

	case IDF_ORD:
		return CompTxtPar(index, FTextOrd);	// abecedn� po�ad� znaku

	case IDF_COLOR:
		{									// barevn� konstanta
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
		CompAddItem(FColorLevel);			// nastaven� jasu barvy
		CompNumSubPar(index, IDF_GRAPH_COLOR, DEFCOLOR);
		CompNumSubPar(index, IDF_COLOR_LEVEL_N, 1);
		return true;

	case IDF_KLAV_VKEY_WAIT:
		CompAddItem(FVKeyWait);				// na�ten� kl�vesy s �ek�n�m
		return true;

	case IDF_VK_NOKEY:
		CompAddItemNum((BYTE)0);			// kl�vesa 0
		return true;

	case IDF_VK_WINR:
		CompAddItemNum((BYTE)VK_RWIN);		// kl�vesa WinR
		return true;

	case IDF_VK_SHIFTR:
		CompAddItemNum((BYTE)VK_RSHIFT);	// kl�vesa ShiftR
		return true;

	case IDF_VK_CTRLR:
		CompAddItemNum((BYTE)VK_RCONTROL);	// kl�vesa ControlR
		return true;

	case IDF_VK_ALTR:
		CompAddItemNum((BYTE)VK_RMENU);		// kl�vesa AltR
		return true;

	case IDF_TEXTLINE_NUM:
		return CompTxtPar(index, FTextLineNum);	// po�et ��dk� textu

	case IDF_MUSIC_POS:
		CompAddItem(FGetMusicPos);			// zji�t�n� pozice hudby
		return true;

	case IDF_MUSIC_LEN:
		CompAddItem(FMusicLen);				// zji�t�n� d�lky hudby
		return true;

	case IDF_COLOR_SYNTH:
		CompAddItem(FColorSynth);			// synt�za barvy
		CompNumSubPar(index, IDF_COLOR_SYNTH_RED, 0);
		CompNumSubPar(index, IDF_COLOR_SYNTH_GREEN, 0);
		CompNumSubPar(index, IDF_COLOR_SYNTH_BLUE, 0);
		CompNumSubPar(index, IDF_COLOR_SYNTH_ALPHA, 1);
		return true;

	case IDF_GET_POINT:
		CompAddItem(FGetPoint);				// zji�t�n� barvy bodu
		CompNumSubPar(index, IDF_GET_POINT_X, 0);
		CompNumSubPar(index, IDF_GET_POINT_Y, 0);
		return true;

	case IDF_SQR:
		return CompNumPar(index, FNumSqr);	// druh� mocnina

	case IDF_CD_LENGTH:
		CompAddItem(FCDGetLength);			// zji�t�n� d�lky CD
		return true;

	case IDF_CD_TRACKS:
		CompAddItem(FCDGetTracks);			// zji�t�n� po�tu stop CD
		return true;

	case IDF_CD_POS:
		CompAddItem(FCDGetPos);				// zji�t�n� aktivn� pozice CD
		return true;

	case IDF_CD_TRACK:
		CompAddItem(FCDGetTrack);			// zji�t�n� aktivn� stopy CD
		return true;

	case IDF_CD_TRACK_LENGTH:
		CompAddItem(FCDGetTrackLength);		// zji�t�n� d�lky stopy CD
		return true;

	case IDF_CD_TRACK_POS:
		CompAddItem(FCDGetTrackPos);		// zji�t�n� pozice stopy CD
		return true;

	case IDF_FILE_DISKSIZE:
		CompAddItem(FGetDiskSize);			// zji�t�n� celkov� velikosti disku
		return true;

	case IDF_FILE_DISKFREE:
		CompAddItem(FGetDiskFree);			// zji�t�n� voln�ho m�sta disku
		return true;

	case IDF_FILE_DISKSERIAL:
		CompAddItem(FGetDiskSerial);		// zji�t�n� s�riov�ho ��sla disku
		return true;

	case IDF_FILE_CREATE:
		CompAddItem(FGetFileCreate);		// zji�t�n� �asu vytvo�en� souboru
		return true;

	case IDF_FILE_OPEN:
		CompAddItem(FGetFileOpen);			// zji�t�n� �asu �ten� ze souboru
		return true;

	case IDF_FILE_MODI:
		CompAddItem(FGetFileModi);			// zji�t�n� �asu modifikace souboru
		return true;

	case IDF_FILE_BYTES:
		CompAddItem(FGetFileByteS);			// na�ten� ��sla bajt se znam�nkem
		return true;

	case IDF_FILE_BYTEU:
		CompAddItem(FGetFileByteU);			// na�ten� ��sla bajt bez znam�nka
		return true;

	case IDF_FILE_WORDS:
		CompAddItem(FGetFileWordS);			// na�ten� ��sla slovo se znam�nkem
		return true;

	case IDF_FILE_WORDU:
		CompAddItem(FGetFileWordU);			// na�ten� ��sla slovo bez znam�nka
		return true;

	case IDF_FILE_DWORDS:
		CompAddItem(FGetFileDWordS);		// na�ten� ��sla dvojslovo se znam�nkem
		return true;

	case IDF_FILE_DWORDU:
		CompAddItem(FGetFileDWordU);		// na�ten� ��sla dvojslovo bez znam�nka
		return true;

	case IDF_FILE_FLOAT:
		CompAddItem(FGetFileFloat);			// na�ten� ��sla FLOAT
		return true;

	case IDF_FILE_DOUBLE:
		CompAddItem(FGetFileDouble);		// na�ten� ��sla DOUBLE
		return true;

	case IDF_FILE_READOFF:
		CompAddItem(FGetFileReadOff);		// offset pro �ten� ze souboru
		return true;

	case IDF_FILE_WRITEOFF:
		CompAddItem(FGetFileWriteOff);		// offset pro z�pis do souboru
		return true;

	case IDF_DATETIME_CURRENT:
		CompAddItem(FGetTimeCurrent);		// na�ten� aktu�ln�ho �asu
		return true;

	case IDF_DATETIME_UTC:
		return CompNumPar(index, FTimeUtc);	// p�evod na sv�tov� �as

	case IDF_DATETIME_LOCAL:
		return CompNumPar(index, FTimeLocal); // p�evod na m�stn� �as

	case IDF_DATETIME_COMP:
		CompAddItem(FTimeComp);					// synt�za �asu
		CompNumSubPar(index, IDF_DATETIME_COMP_YEAR, 1601);
		CompNumSubPar(index, IDF_DATETIME_COMP_MONTH, 1);
		CompNumSubPar(index, IDF_DATETIME_COMP_DAY, 1);
		CompNumSubPar(index, IDF_DATETIME_COMP_HOUR, 0);
		CompNumSubPar(index, IDF_DATETIME_COMP_MINUTE, 0);
		CompNumSubPar(index, IDF_DATETIME_COMP_SECONDE, 0);
		CompNumSubPar(index, IDF_DATETIME_COMP_MSECONDE, 0);
		return true;

	case IDF_DATETIME_GET_YEAR:
		return CompNumPar(index, FGetTimeYear);	// na�ten� roku

	case IDF_DATETIME_GET_MONTH:
		return CompNumPar(index, FGetTimeMonth);// na�ten� m�s�ce

	case IDF_DATETIME_GET_DAY:
		return CompNumPar(index, FGetTimeDay);	// na�ten� dne

	case IDF_DATETIME_GET_WEEK:
		return CompNumPar(index, FGetTimeWeek);	// na�ten� dne v t�dnu

	case IDF_DATETIME_GET_HOUR:
		return CompNumPar(index, FGetTimeHour);	// na�ten� hodiny

	case IDF_DATETIME_GET_MINUTE:
		return CompNumPar(index, FGetTimeMin);	// na�ten� minuty

	case IDF_DATETIME_GET_SECONDE:
		return CompNumPar(index, FGetTimeSec);	// na�ten� sekundy

	case IDF_DATETIME_GET_MSECONDE:
		return CompNumPar(index, FGetTimeMSec);	// na�ten� milisekundy

	case IDF_FILE_SIZE:
		CompAddItem(FGetFileSize);				// zji�t�n� d�lky souboru
		return true;

	case IDF_FILE_TEXTN_LEN:
		CompAddItem(FGetFileTextNLen);			// d�lka textu s pevnou d�lkou
		return true;

	case IDF_FILE_DISK_TYPE:
		CompAddItem(FFileDiskType);				// na�ten� typu disku
		return true;

	case IDF_FILE_3BYTES:
		CompAddItem(FGetFile3ByteS);			// na�ten� ��sla 3 bajty se znam�nkem
		return true;

	case IDF_FILE_3BYTEU:
		CompAddItem(FGetFile3ByteU);			// na�ten� ��sla 3 bajty bez znam�nka
		return true;

	case IDF_DIALOG_BOX:
		CompAddItem(FNumDialogBox);				// dialog box s n�vratem hodnoty
		CompTxtSubPar(index, IDF_DIALOG_BOX_CAPTION);
		CompTxtSubPar(index, IDF_DIALOG_BOX_TEXT);
		CompNumSubPar(index, IDF_DIALOG_BOX_ICON, 0);
		CompNumSubPar(index, IDF_DIALOG_BOX_BUTTON, 0);
		CompNumSubPar(index, IDF_DIALOG_BOX_DEFAULT, 1);
		return true;

	case IDF_SOUND_BIT:
		return CompSndPar(index, FGetSoundBit);	// po�et bit� na vzorek

	case IDF_SOUND_FREQ:
		return CompSndPar(index, FGetSoundFreq);	// vzorkovac� kmito�et

	case IDF_SOUND_LENGTH:
		return CompSndPar(index, FGetSoundLength);	// d�lka zvuku

	case IDF_SOUND_DISTINGUISH:
		CompAddItem(FGetSoundDistinguish);			// zji�t�n� rozli�ovac�ho k�du zvuku
		return true;

	case IDF_MAP_WIDTH:
		if (!CompMapPar(index, FGetMapWidth))		// ���ka plochy
		{
			CompAddItem(FGetMapWidth0);
		}
		return true;

	case IDF_MAP_HEIGHT:
		if (!CompMapPar(index, FGetMapHeight))		// v��ka plochy
		{
			CompAddItem(FGetMapHeight0);
		}
		return true;

	case IDF_MAP_NUM1:
		CompAddItem(FGetMapNum1);			// ��slo 1 z plochy
		CompMapSubPar(index, IDF_MAP_INPUT);
		CompNumSubPar(index, IDF_MAP_X, 0);
		CompNumSubPar(index, IDF_MAP_Y, 0);
		return true;

	case IDF_MAP_NUM2:
		CompAddItem(FGetMapNum2);			// ��slo 2 z plochy
		CompMapSubPar(index, IDF_MAP_INPUT);
		CompNumSubPar(index, IDF_MAP_X, 0);
		CompNumSubPar(index, IDF_MAP_Y, 0);
		return true;

	case IDF_MAP_NUM3:
		CompAddItem(FGetMapNum3);			// ��slo 3 z plochy
		CompMapSubPar(index, IDF_MAP_INPUT);
		CompNumSubPar(index, IDF_MAP_X, 0);
		CompNumSubPar(index, IDF_MAP_Y, 0);
		return true;

	case IDF_SPRITE_WIN_X1:					// lev� okraj okna sprajt�
		CompAddItem(FGetSpriteWinX1);
		return true;

	case IDF_SPRITE_WIN_Y1:					// doln� okraj okna sprajt�
		CompAddItem(FGetSpriteWinY1);
		return true;

	case IDF_SPRITE_WIN_X2:					// prav� okraj okna sprajt�
		CompAddItem(FGetSpriteWinX2);
		return true;

	case IDF_SPRITE_WIN_Y2:					// horn� okraj okna sprajt�
		CompAddItem(FGetSpriteWinY2);
		return true;

	case IDF_RETURN_CODE:
		CompAddItem(FGetReturnCode);		// n�vratov� k�d
		return true;

	case IDF_EXEC_WAIT:
		return CompTxtPar(index, FGetExecWait);	// spu�t�n� programu s �ek�n�m

	case IDF_JOYSTICK_X:
		if (!CompNumPar(index, FJoystickX))	// sou�adnice X joysticku
		{
			CompAddItem(FJoystickX);
			CompAddItemNum((BYTE)0);
		}
		return true;

	case IDF_JOYSTICK_Y:
		if (!CompNumPar(index, FJoystickY))	// sou�adnice Y joysticku
		{
			CompAddItem(FJoystickY);
			CompAddItemNum((BYTE)0);
		}
		return true;

	case IDF_JOYSTICK_Z:
		if (!CompNumPar(index, FJoystickZ))	// sou�adnice Z joysticku
		{
			CompAddItem(FJoystickZ);
			CompAddItemNum((BYTE)0);
		}
		return true;

	case IDF_WINDOW_ID:
		CompAddItem(FGetWindowID);			// aktivn� prvek okna
		return true;

	case IDF_WINDOW_NEW:
		CompAddItem(FWindowNew);			// vytvo�en� okna
		return true;

	case IDF_BUTTON_NEW:
		CompAddItem(FButtonNew);			// vytvo�en� tla��tka
		return true;

	case IDF_BUTTON_ESC:
		CompAddItem(FGetButtonEsc);			// tla��tko pro Esc
		return true;

	case IDF_BUTTONCHECK_NEW:
		CompAddItem(FButtonCheckNew);		// vytvo�en� tla��tka
		return true;

	case IDF_BUTTONCHECK_NEW2:
		CompAddItem(FButtonCheckNew2);		// vytvo�en� tla��tkov�ho p�ep�na�e
		return true;

	case IDF_EDIT_NEW:
		CompAddItem(FEditNew);				// vytvo�en� edita�n�ho ��dku
		return true;

	case IDF_BUTTONRADIO_NEW:
		CompAddItem(FButtonRadioNew);		// vytvo�en� tla��tka
		return true;

	case IDF_BUTTONRADIO_NEW2:
		CompAddItem(FButtonRadioNew2);		// vytvo�en� tla��tka
		return true;

	case IDF_WINDOW_X:
		CompAddItem(FGetWindowX);			// sou�adnice X okna
		return true;

	case IDF_WINDOW_Y:
		CompAddItem(FGetWindowY);			// sou�adnice Y okna
		return true;

	case IDF_WINDOW_W:
		CompAddItem(FGetWindowW);			// ���ka okna
		return true;

	case IDF_WINDOW_H:
		CompAddItem(FGetWindowH);			// v��ka okna
		return true;

	case IDF_WINDOW_W_INT:
		CompAddItem(FGetWindowWInt);		// vnit�n� ���ka okna
		return true;

	case IDF_WINDOW_H_INT:
		CompAddItem(FGetWindowHInt);		// vnit�n� v��ka okna
		return true;

	case IDF_FONTHEIGHT:
		CompAddItem(FGetFontHeight);		// v��ka font�
		return true;

	case IDF_FONTWIDTH:
		CompAddItem(FGetFontWidth);			// ���ka font�
		return true;

	case IDF_FONTANGLE:
		CompAddItem(FGetFontAngle);			// �hel textu
		return true;

	case IDF_FONTCOL:
		CompAddItem(FGetFontCol);			// barva textu
		return true;

	case IDF_FONTBACK:
		CompAddItem(FGetFontBack);			// barva pozad�
		return true;

	case IDF_MEMO_NEW:
		CompAddItem(FMemoNew);				// vytvo�en� edita�n�ho pole
		return true;

	case IDF_RICHEDIT_NEW:
		CompAddItem(FRicheditNew);			// vytvo�en� editoru Richedit
		return true;

	case IDF_FOCUS:
		CompAddItem(FGetFocus);				// okno s fokusem
		return true;

	case IDF_DEFBUTTON_NEW:
		CompAddItem(FDefButtonNew);			// vytvo�en� implicitn�ho tla��tka
		return true;

	case IDF_BUTTONICON_NEW:
		CompAddItem(FButtonIconNew);		// vytvo�en� tla��tka s ikonou
		return true;

	case IDF_GROUPBOX_NEW:
		CompAddItem(FGroupBoxNew);			// vytvo�en� skupiny
		return true;

	case IDF_LISTBOX_NEW:
		CompAddItem(FListBoxNew);			// vytvo�en� seznamu
		return true;

	case IDF_TABS_NEW:
		CompAddItem(FTabsNew);				// vytvo�en� z�lo�ek
		return true;

	case IDF_WINTEXT_NEW:
		CompAddItem(FWinTextNew);			// vytvo�en� textu
		return true;

	case IDF_WINTEXT_NEW_CENTER:
		CompAddItem(FWinTextNewCenter);		// vytvo�en� textu vyst�ed�n�ho
		return true;

	case IDF_WINTEXT_NEW_RIGHT:
		CompAddItem(FWinTextNewRight);		// vytvo�en� textu zarovnan�ho vpravo
		return true;

	case IDF_COMBOBOX_NEW:
		CompAddItem(FComboBoxNew);			// vytvo�en� seznamu
		return true;

	case IDF_COMBOEDIT_NEW:
		CompAddItem(FComboEditNew);			// vytvo�en� seznamu
		return true;

	case IDF_LISTEDIT_NEW:
		CompAddItem(FListEditNew);			// vytvo�en� seznamu
		return true;

	case IDF_BUTTON3_NEW:
		CompAddItem(FButton3New);			// vytvo�en� tla��tka
		return true;

	case IDF_WINDOWRO_NEW:
		CompAddItem(FWindowRONew);			// vytvo�en� okna
		return true;

	case IDF_WINICON_NEW:
		CompAddItem(FWinIconNew);			// vytvo�en� ikony
		return true;

	case IDF_WINPIC_NEW:
		CompAddItem(FWinPicNew);			// vytvo�en� obr�zku
		return true;

	case IDF_2PI:
		CompAddItemNum(pi2);				// �hel 360 stup��
		return true;

	case IDF_BOD32:
		CompAddItemNum((double)1/32);		// vzd�lenost 1 bodu
		return true;

	case IDF_M1:
		CompAddItemNum((double)-1);			// �hel 360 stup��
		return true;

	case IDF_SOUND_POS:
		CompAddItem(FGetSoundPos);			// zji�t�n� pozice zvuku
		return true;

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_DLL_LOAD:						// na�ten� DLL knihovny
		return CompTxtPar(index, FDLLLoad);

	case IDF_DLL_IMPORT:					// importov�n� funkce DLL knihovny
		CompAddItem(FDLLImport);
		CompNumSubPar(index, IDF_DLL_IMPORT_LIB, 0);
		CompTxtSubPar(index, IDF_DLL_IMPORT_NAME);
		CompNumSubPar(index, IDF_DLL_IMPORT_PAR, 0);
		CompLogSubPar(index, IDF_DLL_IMPORT_C, false);
		return true;

	case IDF_DLL_EXEC:						// proveden� funkce DLL
		return CompNumPar(index, FDLLExecNum);

	case IDF_MEMORY_NEW:					// vytvo�en� pam�ov�ho bloku
		return CompNumPar(index, FMemoryNew);

	case IDF_MEMORY_RESIZE:					// zm�na velikosti pam�ov�ho bloku
		CompAddItem(FMemoryResize);
		CompNumSubPar(index, IDF_MEMORY_RESIZE_ADR, 0);
		CompNumSubPar(index, IDF_MEMORY_RESIZE_NEW, 0);
		return true;

	case IDF_MEMORY_READ:					// ukazatel �ten� z pam�ti
		CompAddItem(FGetMemoryRead);
		return true;

	case IDF_MEMORY_WRITE:					// ukazatel z�pisu do pam�ti
		CompAddItem(FGetMemoryWrite);
		return true;

	case IDF_MEMORY_COMPARE:				// porovn�n� dat
		return CompNumPar(index, FMemoryCompare);

	case IDF_MEMORY_BYTES:					// bajt se znam�nkem
		CompAddItem(FGetMemoryByteS);
		return true;

	case IDF_MEMORY_BYTEU:					// bajt bez znam�nka
		CompAddItem(FGetMemoryByteU);
		return true;

	case IDF_MEMORY_WORDS:					// slovo se znam�nkem
		CompAddItem(FGetMemoryWordS);
		return true;

	case IDF_MEMORY_WORDU:					// slovo bez znam�nka
		CompAddItem(FGetMemoryWordU);
		return true;

	case IDF_MEMORY_3BYTES:					// 3-bajt se znam�nkem
		CompAddItem(FGetMemory3ByteS);
		return true;

	case IDF_MEMORY_3BYTEU:					// 3-bajt bez znam�nka
		CompAddItem(FGetMemory3ByteU);
		return true;

	case IDF_MEMORY_DWORDS:					// dvouslovo se znam�nkem
		CompAddItem(FGetMemoryDWordS);
		return true;

	case IDF_MEMORY_DWORDU:					// dvouslovo bez znam�nka
		CompAddItem(FGetMemoryDWordU);
		return true;

	case IDF_MEMORY_FLOAT:					// re�ln� ��slo 4 bajty
		CompAddItem(FGetMemoryFloat);
		return true;

	case IDF_MEMORY_DOUBLE:					// re�ln� ��slo 8 bajt�
		CompAddItem(FGetMemoryDouble);
		return true;

	case IDF_MEMORY_TEXTN_LEN:				// d�lka textu s pevnou d�lkou
		CompAddItem(FGetMemoryTextNLen);
		return true;

	case IDF_WINDOW_HANDLE:					// handle okenn�ho prvku
		return CompNumPar(index, FWindowHandle);

	case IDF_MEMORY_3D:						// vytvo�en� objektu z bloku
		CompAddItem(FMemory3D);
		CompNumSubPar(index, IDF_MEMORY_3D_VN, 0);
		CompNumSubPar(index, IDF_MEMORY_3D_VERT, 0);
		CompNumSubPar(index, IDF_MEMORY_3D_NORM, 0);
		CompNumSubPar(index, IDF_MEMORY_3D_TEXT, 0);
		CompNumSubPar(index, IDF_MEMORY_3D_COL, 0);
		CompNumSubPar(index, IDF_MEMORY_3D_FN, 0);
		CompNumSubPar(index, IDF_MEMORY_3D_FACE, 0);
		return true;

	case IDF_INI_NUM:						// �ten� konfigura�n�ho ��sla
		if (!CompNumPar(index, FGetIniNum))
		{
			CompAddItem(FGetIniNum0);
		}
		return true;

	case IDF_REG_KEY:						// skupina kl��� registr�
		CompAddItem(FGetRegKey);
		return true;

	case IDF_REG_NUM:						// �ten� ��sla z registru
		if (!CompNumPar(index, FGetRegNum))
		{
			CompAddItem(FGetRegNum0);
		}
		return true;

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------


// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_GAME_N1:
		CompAddItem(FGetGameN1);			// na�ten� hern�ho ��sla 1
		return true;

	case IDF_GAME_N2:
		CompAddItem(FGetGameN2);			// na�ten� hern�ho ��sla 2
		return true;

	case IDF_GAME_N3:
		CompAddItem(FGetGameN3);			// na�ten� hern�ho ��sla 3
		return true;

	case IDF_GAME_N4:
		CompAddItem(FGetGameN4);			// na�ten� hern�ho ��sla 4
		return true;

	case IDF_GAME_N5:
		CompAddItem(FGetGameN5);			// na�ten� hern�ho ��sla 5
		return true;

	case IDF_GAME_N6:
		CompAddItem(FGetGameN6);			// na�ten� hern�ho ��sla 6
		return true;

	case IDF_GAME_N7:
		CompAddItem(FGetGameN7);			// na�ten� hern�ho ��sla 7
		return true;

	case IDF_GAME_N8:
		CompAddItem(FGetGameN8);			// na�ten� hern�ho ��sla 8
		return true;

	case IDF_PLAYER_MAX:
		CompAddItem(FGetPlayerMax);			// max. hr���
		return true;

	case IDF_PLAYER_INX:
		CompAddItem(FPlayerInx);			// index hr��e
		return true;

	case IDF_GAME_PACKET:
		CompAddItem(FGetGamePacket);		// p��jem paketu
		return true;

	case IDF_GAME_DATAN:
		CompAddItem(FGetGameDataN);			// na�ten� ��sla z paketu
		return true;

	case IDF_GAME_DATAB:
		CompAddItem(FGetGameDataB);			// na�ten� bajtu z paketu
		return true;

	case IDF_GRAPH_AREA_WIDTH:
		CompAddItem(FGetGraphAreaWidth);	// ���ka k zobrazen� centrovan�ho textu
		return true;

	case IDF_UDP_SEND_PORT:
		CompAddItem(FGetUDPSendPort);		// zji�t�n� vys�lac�ho portu UDP
		return true;

	case IDF_UDP_REC_SIZE:
		CompAddItem(FGetUDPRecSize);		// zji�t�n� velikosti p�ij�mac�ho bufferu UDP
		return true;

	case IDF_UDP_REC_PORT:
		CompAddItem(FGetUDPRecPort);		// zji�t�n� p�ijat�ho portu UDP
		return true;

	case IDF_UDP_MEM:
		CompAddItem(FGetUDPMem);			// p��jem datov�ho bloku UDP
		return true;

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------


	case IDF_PICTURE_WIDTH:
		return CompPicPar(index, FGetPicWidth);		// ���ka obr�zku

	case IDF_PICTURE_HEIGHT:
		return CompPicPar(index, FGetPicHeight);	// v��ka obr�zku

	case IDF_BIT_AND:
		return CompNumGrp(index, FBitAnd, FBitAnd);	// oper�tor BIT-AND

	case IDF_BIT_OR:
		return CompNumGrp(index, FBitOr, FBitOr);	// oper�tor BIT-OR

	case IDF_BIT_XOR:
		return CompNumGrp(index, FBitXor, FBitXor);	// oper�tor BIT-XOR

	case IDF_BIT_NOT:
		return CompNumPar(index, FBitNot);			// bitov� dopln�k

	case IDF_BIT_MAX:
		CompAddItemNum((double)(DWORD)0xffffffff);	// max. bitov� ��slo
		return true;

	case IDF_BIT_LEFT:
		return CompNumPar(index, FBitLeft);			// bitov� rotace vlevo

	case IDF_BIT_RIGHT:
		return CompNumPar(index, FBitRight);		// bitov� rotace vpravo

	case IDF_CODEPAGE:
		CompAddItem(FGetCodePage);					// zji�t�n� znakov� sady
		return true;

	case IDF_LANGUAGE:
		CompAddItem(FGetLanguage);					// zji�t�n� jazyku
		return true;

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_COM_OPEN:								// otev�en� portu
		return CompTxtPar(index, FComOpen);

	case IDF_COM_ID:
		CompAddItem(FGetComID);						// aktivn� port
		return true;

	case IDF_COM_BAUD:
		CompAddItem(FGetComBaud);					// zji�t�n� p�enosov� rychlosti portu
		return true;

	case IDF_COM_BITS:
		CompAddItem(FGetComBits);					// zji�t�n� bit� portu
		return true;

	case IDF_COM_PARIT:
		CompAddItem(FGetComParit);					// zji�t�n� parity
		return true;

	case IDF_COM_NUM:
		CompAddItem(FGetComNum);					// na�ten� ��sla
		return true;

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------

	case IDF_RICHMEMO_NEW:
		CompAddItem(FRichMemoNew);				// vytvo�en� edita�n�ho pole bez zalamov�n�
		return true;

	case IDF_PROGRESS_NEW:
		CompAddItem(FProgressNew);				// vytvo�en� indik�toru pr�b�hu
		return true;

	case IDF_HTRACKBAR_NEW:
		CompAddItem(FHTrackbarNew);				// vytvo�en� horizont�ln�ho t�hla
		return true;

	case IDF_VTRACKBAR_NEW:
		CompAddItem(FVTrackbarNew);				// vytvo�en� vertik�ln�ho t�hla
		return true;

	case IDF_HSCROLLBAR_NEW:
		CompAddItem(FHScrollbarNew);			// vytvo�en� horizont�ln�ho posuvn�ku
		return true;

	case IDF_VSCROLLBAR_NEW:
		CompAddItem(FVScrollbarNew);			// vytvo�en� vertik�ln�ho posuvn�ku
		return true;

	case IDF_WINDOW_POS:
		CompAddItem(FGetWindowPos);				// na�ten� pozice prvku
		return true;

	case IDF_WINDOW_POS2:
		CompAddItem(FGetWindowPos2);				// na�ten� pozice 2 prvku
		return true;

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_MIX_LINE_TYPE:						// zji�t�n� typu sign�lu
		CompAddItem(FGetLineType);
		return true;

	case IDF_MIX_CONTROL_TYPE:					// zji�t�n� typu ovl�dac�ho prvku
		CompAddItem(FGetCtrlType);
		return true;
	
	case IDF_MIX_CONTROL_VAL:					// zji�t�n� hodnoty ovl�dac�ho prvku
		CompAddItem(FGetCtrlVal);
		return true;

	case IDF_MIX_CHANNEL:						// zji�t�n� po�tu kan�l� prvku
		CompAddItem(FGetMixChannel);
		return true;

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------


// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_DIRECT3D_OCULLING:					// test nastaven� odstra�ov�n� ploch objektu Direct3D
		CompAddItem(FGetD3DOCulling);
		return true;

	case IDF_DIRECT3D_WALL:						// vytvo�en� zdi Direct3D
		CompAddItem(FD3DWall);
		return true;

	case IDF_DIRECT3D_CUBE:						// vytvo�en� krychle Direct3D
		CompAddItem(FD3DCube);
		return true;

	case IDF_DIRECT3D_SPHERE:					// vytvo�en� koule Direct3D
		CompAddItem(FD3DSphere);
		return true;

	case IDF_DIRECT3D_CYLINDER:					// vytvo�en� v�lce Direct3D
		CompAddItem(FD3DCylinder);
		return true;

	case IDF_DIRECT3D_CONE:						// vytvo�en� ku�ele Direct3D
		CompAddItem(FD3DCone);
		return true;

	case IDF_DIRECT3D_CONE2:					// vytvo�en� komol�ho ku�ele Direct3D
		return CompNumPar(index, FD3DCone2);

	case IDF_DIRECT3D_CLONE:					// klonov�n� objektu Direct3D
		return CompNumPar(index, FD3DClone);

	case IDF_DIRECT3D_TEXT:						// objekt Direct3D z textu
		return CompTxtPar(index, FD3DText);

	case IDF_DIRECT3D_FILE:						// objekt Direct3D ze souboru
		return CompTxtPar(index, FD3DFile);

	case IDF_DIRECT3D_OBJECT_ID:				// zji�t�n� ID aktivn�ho objektu Direct3D
		CompAddItem(FGetD3DObjectID);
		return true;

	case IDF_DIRECT3D_SCALEX:					// zji�t�n� m���tka ve sm�ru X
		CompAddItem(FGetD3DScaleX);
		return true;

	case IDF_DIRECT3D_SCALEY:					// zji�t�n� m���tka ve sm�ru Y
		CompAddItem(FGetD3DScaleY);
		return true;

	case IDF_DIRECT3D_SCALEZ:					// zji�t�n� m���tka ve sm�ru Z
		CompAddItem(FGetD3DScaleZ);
		return true;

	case IDF_DIRECT3D_ROTATEX:					// zji�t�n� rotace podle osy X
		CompAddItem(FGetD3DRotateX);
		return true;

	case IDF_DIRECT3D_ROTATEY:					// zji�t�n� rotace podle osy Y
		CompAddItem(FGetD3DRotateY);
		return true;

	case IDF_DIRECT3D_ROTATEZ:					// zji�t�n� rotace podle osy Z
		CompAddItem(FGetD3DRotateZ);
		return true;

	case IDF_DIRECT3D_TRANSX:					// zji�t�n� posunu ve sm�ru X
		CompAddItem(FGetD3DTransX);
		return true;

	case IDF_DIRECT3D_TRANSY:					// zji�t�n� posunu ve sm�ru Y
		CompAddItem(FGetD3DTransY);
		return true;

	case IDF_DIRECT3D_TRANSZ:					// zji�t�n� posunu ve sm�ru Z
		CompAddItem(FGetD3DTransZ);
		return true;

	case IDF_DIRECT3D_COLOR:					// zji�t�n� barvy objektu
		CompAddItem(FGetD3DColor);
		return true;

	case IDF_DIRECT3D_EMISSIVE:					// zji�t�n� barvy sv�tivosti
		CompAddItem(FGetD3DEmissive);
		return true;

	case IDF_DIRECT3D_POWER:					// zji�t�n� matnosti objektu
		CompAddItem(FGetD3DPower);
		return true;

	case IDF_DIRECT3D_SPECULAR:					// zji�t�n� barvy odlesku
		CompAddItem(FGetD3DSpecular);
		return true;

	case IDF_DIRECT3D_LIGHT:					// vytvo�en� sv�tla Direct3D
		CompAddItem(FD3DLight);
		return true;

	case IDF_DIRECT3D_LIGHT_COLOR:				// zji�t�n� barvy sv�tla Direct3D
		CompAddItem(FGetD3DLightColor);
		return true;

	case IDF_DIRECT3D_LIGHT_TYPE:				// zji�t�n� typu sv�tla Direct3D
		CompAddItem(FGetD3DLightType);
		return true;

	case IDF_DIRECT3D_LIGHT_UMBRA:				// zji�t�n� �hle st�nu sv�tla Direct3D
		CompAddItem(FGetD3DLightUmbra);
		return true;

	case IDF_DIRECT3D_LIGHT_PENUMBRA:			// zji�t�n� �hle polost�nu sv�tla Direct3D
		CompAddItem(FGetD3DLightPenumbra);
		return true;

	case IDF_DIRECT3D_LIGHT_RANGE:				// zji�t�n� dosahu sv�tla Direct3D
		CompAddItem(FGetD3DLightRange);
		return true;

	case IDF_DIRECT3D_LIGHT_CATTEN:				// zji�t�n� konstantn�ho �tlumu sv�tla Direct3D
		CompAddItem(FGetD3DLightCatten);
		return true;

	case IDF_DIRECT3D_LIGHT_LATTEN:				// zji�t�n� line�rn�ho �tlumu sv�tla Direct3D
		CompAddItem(FGetD3DLightLatten);
		return true;

	case IDF_DIRECT3D_LIGHT_QATTEN:				// zji�t�n� kvadratick�ho �tlumu sv�tla Direct3D
		CompAddItem(FGetD3DLightQatten);
		return true;

	case IDF_DIRECT3D_PROJECTION:				// typ projekce
		CompAddItem(FGetD3DProjection);
		return true;

	case IDF_DIRECT3D_SCENE_COLOR:				// zji�t�n� barvy pozad� sc�ny
		CompAddItem(FGetD3DSceneColor);
		return true;

	case IDF_DIRECT3D_FOG_COLOR:				// zji�t�n� barvy mlhy
		CompAddItem(FGetD3DFogColor);
		return true;

	case IDF_DIRECT3D_FOG_MODE:					// zji�t�n� m�du mlhy
		CompAddItem(FGetD3DFogMode);
		return true;

	case IDF_DIRECT3D_FOG_START:				// zji�t�n� za��tku mlhy
		CompAddItem(FGetD3DFogStart);
		return true;

	case IDF_DIRECT3D_FOG_END:					// zji�t�n� konce mlhy
		CompAddItem(FGetD3DFogEnd);
		return true;

	case IDF_DIRECT3D_FOG_DENSITY:				// zji�t�n� hustoty mlhy
		CompAddItem(FGetD3DFogDensity);
		return true;

	case IDF_DIRECT3D_VIEW_FRONT:				// zji�t�n� prom�tac� roviny
		CompAddItem(FGetD3DViewFront);
		return true;

	case IDF_DIRECT3D_VIEW_BACK:				// zji�t�n� vzd�lenosti dohledu
		CompAddItem(FGetD3DViewBack);
		return true;

//	case IDF_DIRECT3D_PICK:
//		CompAddItem(FD3DPick);							// vyhled�n� objektu
//		CompNumSubPar(index, IDF_DIRECT3D_PICK_X, 0x1000000);
//		CompNumSubPar(index, IDF_DIRECT3D_PICK_Y, 0x1000000);
//		return true;

	case IDF_DIRECT3D_2DOBJECT:
		return CompPicPar(index, FD3D2DObject);		// vytvo�en� 2D objektu

	case IDF_DIRECT3D_TERRAIN:
		return CompPicPar(index, FD3D2DTerrain);	// vytvo�en� ter�nu

	case IDF_DIRECT3D_LEVEL:
		CompAddItem(FD3DLevel);							// zji�t�n� �rovn� ter�nu
		CompNumSubPar(index, IDF_DIRECT3D_LEVEL_X, 0x1000000);
		CompNumSubPar(index, IDF_DIRECT3D_LEVEL_Y, 0x1000000);
		return true;

	case IDF_DIRECT3D_EMPTY:						// vytvo�en� pr�zdn�ho objektu Direct3D
		CompAddItem(FD3DEmpty);
		return true;

	case IDF_DIRECT3D_SPLITS:						// zji�t�n� slo�itosti objekt�
		CompAddItem(FGetD3DSplits);
		return true;

	case IDF_DIRECT3D_ORDER:						// zji�t�n� po�ad� rotac� objektu
		CompAddItem(FGetD3DOrder);
		return true;

	case IDF_DIRECT3D_DRIVER:						// zji�t�n� aktivn�ho ovlada�e
		CompAddItem(FGetD3DDriver);
		return true;

	case IDF_DIRECT3D_INTERFACE:					// zji�t�n� aktivn�ho rozhran�
		CompAddItem(FGetD3DInterface);
		return true;

	case IDF_DIRECT3D_ROTARY:						// vytvo�en� rota�n�ho objektu
		return CompTxtPar(index, FD3DRotary);

	case IDF_DIRECT3D_TERRAIN_MAP:
		return CompMapPar(index, FD3DTerrainMap);	// vytvo�en� ter�nu z plochy

	case IDF_DIRECT3D_SMOOTH:						// zji�t�n� stupn� vyhlazen� textur
		CompAddItem(FGetD3DSmooth);
		return true;

	case IDF_DIRECT3D_AMBIENT:						// zji�t�n� ambient sv�tla
		CompAddItem(FGetD3DAmbient);
		return true;

	case IDF_DIRECT3D_CIRCLE:						// vytvo�en� kruhu Direct3D
		CompAddItem(FD3DCircle);
		return true;

	case IDF_DIRECT3D_TRIANGLE:						// vytvo�en� troj�heln�ku Direct3D
		CompAddItem(FD3DTriangle);
		return true;

	case IDF_DIRECT3D_TORUS:						// vytvo�en� torusu Direct3D
		return CompNumPar(index, FD3DTorus);

	case IDF_DIRECT3D_2DSTATIC:
		return CompPicPar(index, FD3D2DStatic);		// vytvo�en� statick�ho 2D objektu

	case IDF_DIRECT3D_HEMISPHERE:					// vytvo�en� polokoule Direct3D
		CompAddItem(FD3DHemisphere);
		return true;

	case IDF_DIRECT3D_TRANSPARENT:					// zji�t�n� pr�hlednosti textury
		CompAddItem(FGetD3DTransparent);
		return true;

	case IDF_DIRECT3D_ALPHAREF:						// zji�t�n� referen�n� �rovn� alfa kan�lu
		CompAddItem(FGetD3DAlphaRef);
		return true;

	case IDF_DIRECT3D_AMBIREF:						// zji�t�n� odrazivosti rozpt�len�ho sv�tla
		CompAddItem(FGetD3DAmbiRef);
		return true;

	case IDF_DIRECT3D_ELAPSED:
		if (!CompNumPar(index, FD3DElapsed))		// uplynul� �as s n�sobkem
		{
			CompAddItem(FD3DElapsed0);				// uplynul� �as bez n�sobku
		}
		return true;

	case IDF_DIRECT3D_SPEED:						// m��en� rychlosti renderov�n�
		CompAddItem(FD3DSpeed);
		return true;

	case IDF_DIRECT3D_FREE:							// voln� videopam�
		CompAddItem(FD3DFree);
		return true;

	case IDF_DIRECT3D_PICTURE:
		return CompPicPar(index, FD3DPicture);		// vytvo�en� 2D obr�zku

	case IDF_DIRECT3D_LENSFLARE:
		return CompPicPar(index, FD3DLensFlare);	// vytvo�en� �o�kov�ho odlesku

	case IDF_DIRECT3D_STAGE:						// stupe� textur
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

	case IDF_DIRECT3D_ADDRESSU:						// horizont�ln� adresov�n�
		CompAddItem(FGetD3DAddressU);
		return true;

	case IDF_DIRECT3D_ADDRESSV:						// vertik�ln� adresov�n�
		CompAddItem(FGetD3DAddressV);
		return true;

//	case IDF_DIRECT3D_BORDER:						// barva okol� textur
//		CompAddItem(FGetD3DBorder);
//		return true;

	case IDF_DIRECT3D_BIAS:							// zjemn�n� vzd�len�ch textur
		CompAddItem(FGetD3DBias);
		return true;

	case IDF_DIRECT3D_TFACTOR:						// faktor textur
		CompAddItem(FGetD3DTFactor);
		return true;

	case IDF_DIRECT3D_MIPMAPS:						// po�et �rovn� mipmaps
		CompAddItem(FGetD3DMipMaps);
		return true;

	case IDF_DIRECT3D_ZGROUP:						// renderovac� skupina
		CompAddItem(FGetD3DZGroup);
		return true;

	case IDF_DIRECT3D_ZTEST:						// hloubkov� test
		CompAddItem(FGetD3DZTest);
		return true;

	case IDF_SHADOW_RANGE:							// dosah st�nu
		CompAddItem(FGetD3DShadowRange);
		return true;

	case IDF_DIRECT3D_LOD_OBJECT:					// objekt pro LOD
		CompAddItem(FGetD3DLODObject);
		return true;

	case IDF_DIRECT3D_LOD_DISTANCE:					// vzd�lenost pro LOD
		CompAddItem(FGetD3DLODDistance);
		return true;

	case IDF_DIRECT3D_MORPH_MODEL:					// zji�t�n� max. stupn� morfov�n�
		CompAddItem(FGetD3DMorphModel);
		return true;

	case IDF_DIRECT3D_MORPH_LEVEL:					// zji�t�n� stupn� morfov�n�
		CompAddItem(FGetD3DMorphLevel);
		return true;

	case IDF_DIRECT3D_MEMORY:						// pam�ov� blok z 3D prvku
		return CompNumPar(index, FD3DMemory);

	case IDF_DIRECT3D_CHILD:						// nalezen� potomka 3D objektu
		if (!CompNumPar(index, FD3DChild))			// zadan� potomek
		{
			CompAddItem(FD3DChild0);				// prvn� potomek
		}
		return true;

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------

	case IDF_POLAR_RADIUS:
		CompAddItem(FPolarRadius);					// v�po�et vzd�lenosti bodu
		CompNumSubPar(index, IDF_POLAR_X, 0);
		CompNumSubPar(index, IDF_POLAR_Y, 0);
		return true;

	case IDF_POLAR_ANGLE:
		CompAddItem(FPolarAngle);					// v�po�et �hlu bodu
		CompNumSubPar(index, IDF_POLAR_X, 0);
		CompNumSubPar(index, IDF_POLAR_Y, 0);
		return true;

	case IDF_PRECISE_TIMER:							// p�esn� ��ta� �asu
		CompAddItem(FPreciseTimer);
		return true;

	case IDF_TABLE_NEW:
		return CompTxtPar(index, FTableNew);		// vytvo�en� tabulky

	case IDF_TABLE_WIDTH:
		CompAddItem(FGetTableWidth);				// zji�t�n� ���ky sloupce tabulky
		return true;

	case IDF_TABLE_ALIGN:
		CompAddItem(FGetTableAlign);				// zji�t�n� zarovn�n� sloupce tabulky
		return true;

	default:

// p�eklad k�du kl�vesy
		{
			int fn = item->Func + IDF - CompTabVKBeg;
			if ((DWORD)fn < (DWORD)(CompTabVKEnd - CompTabVKBeg + 1))
			{
				CompAddItemNum(CompTabVK[fn]);	// k�d kl�vesy
				return true;
			}
		}
		return false;
	}
}


/////////////////////////////////////////////////////////////////////////////
// p�eklad p��kazu s ��seln�m parametrem

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
// p�eklad p��kazu s ��seln�m podparametrem (hledan�m podle identifikace)

void CompNumSubPar(int index, int idf, double def)
{
// korekce identifikace funkce
	idf -= IDF;

// ukazatel v�choz�ho prvku
	PETPROG*	item = BufEdi + index;
	PETPROG2*	item2 = BufEdi2 + index;

// kontrola, zda m� polo�ka n�jak� potomky
	if (item->Param & PETPROG_CHILDS)
	{
		int posun = 1;

// cyklus p�es v�echny potomky
		do {

// adresa dal��ho potomka
			index += posun;
			item += posun;
			item2 += posun;

// test, zda to je hledan� prvek - na�ten� prvku
			if ((item->Func == idf) &&
				(item->Param & PETPROG_CHILDS) && 
				CompNum(index + 1))
			{
				return;
			}

// posun pro p��t� prvek
			posun = item2->Items;

// dokud je dal�� potomek
		} while (item->Param & PETPROG_NEXT);
	}

// pou�ije se implicitn� hodnota
	CompAddItemNum(def);
}
