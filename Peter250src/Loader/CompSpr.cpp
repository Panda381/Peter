
#include "Main.h"

/***************************************************************************\
*																			*
*						Kompilace programu - sprajt							*
*																			*
\***************************************************************************/

#pragma optimize("s", on)			// optimalizace na minimální velikost


/////////////////////////////////////////////////////////////////////////////
// pøeklad výrazu se sprajtem (vrací true = operace OK)

bool _fastcall CompSpr(int index)
{
// adresa zdrojového prvku
	if ((DWORD)index >= (DWORD)BufEdiN) return false;
	PETPROG*	item = BufEdi + index;
	PETPROG2*	item2 = BufEdi2 + index;
	int refinx = item->RefIndex;

// kontrola, zda je položka vypnuta
	if ((item->Param & (PETPROG_OFF | PETPROG_OFF_DEP)) != 0) return false;

// vìtvení podle funkce
	switch (item->Func + IDF)
	{
	case IDF_SPRITE:
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			item = BufObj + refinx;
			item2 = BufObj2 + refinx;
			if (Sprite.IsNotValid(item2->Data)) return false;

			if (item2->List < 0)
			{
				CompAddItem(FSpriteObj, item2->Data);
			}
			else
			{
				CompAddItem(FSpriteObjList, item2->Data, item2->List);
			}
			return true;
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			item = BufLoc + refinx;
			item2 = BufLoc2 + refinx;
			if (Sprite.IsNotValid(item2->Data)) return false;

			if (item2->List < 0)
			{
				CompAddItem(FSpriteLoc, item2->Local + KorigSpr);
			}
			else
			{
				CompAddItem(FSpriteLocList, item2->Local + KorigSpr, item2->LocalList + KorigLst);
			}
			return true;
		}
		return false;

	case IDF_FNC:
		return CompFunc(index, IDF_SPRITE);	// funkce s návratem sprajtu

	case IDF_FILE_SPRITE:
		CompAddItem(FGetFileSprite);			// naètení sprajtu
		return true;

	default:
		return false;
	}
}


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu s parametrem sprajtu

bool CompSprPar(int index, PROCCOM func)
{
	return CompSprPar(index, func, 0, -1);
}

bool CompSprPar(int index, PROCCOM func, int data)
{
	return CompSprPar(index, func, data, -1);
}

bool CompSprPar(int index, PROCCOM func, int data, int list)
{
	int zahlavi = CompAddItem(func, data, list);

	if ((BufEdi2[index].Items == 1) || !CompSpr(index+1))
	{
		ProgNum = zahlavi;
		return false;
	}
	return true;
}
