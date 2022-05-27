
#include "Main.h"

/***************************************************************************\
*																			*
*							Kompilace programu - hudba						*
*																			*
\***************************************************************************/

#pragma optimize("s", on)			// optimalizace na minim�ln� velikost


/////////////////////////////////////////////////////////////////////////////
// p�eklad v�razu s hudbou (vrac� true = operace OK)

bool _fastcall CompMus(int index)
{
// adresa zdrojov�ho prvku
	if ((DWORD)index >= (DWORD)BufEdiN) return false;
	PETPROG*	item = BufEdi + index;
	PETPROG2*	item2 = BufEdi2 + index;
	int refinx = item->RefIndex;

// kontrola, zda je polo�ka vypnuta
	if ((item->Param & (PETPROG_OFF | PETPROG_OFF_DEP)) != 0) return false;

// v�tven� podle funkce
	switch (item->Func + IDF)
	{
	case IDF_FNC:
		return CompFunc(index, IDF_MUS);	// funkce s n�vratem hudby

	case IDF_MUS:
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			item = BufObj + refinx;
			item2 = BufObj2 + refinx;
			if (Music.IsNotValid(item2->Data)) return false;

			if (item2->List < 0)
			{
				CompAddItem(FMusicObj, item2->Data);
			}
			else
			{
				CompAddItem(FMusicObjList, item2->Data, item2->List);
			}
			return true;
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			item = BufLoc + refinx;
			item2 = BufLoc2 + refinx;
			if (Music.IsNotValid(item2->Data)) return false;

			if (item2->List < 0)
			{
				CompAddItem(FMusicLoc, item2->Local + KorigMus);
			}
			else
			{
				CompAddItem(FMusicLocList, item2->Local + KorigMus, item2->LocalList + KorigLst);
			}
			return true;
		}
		return false;

	case IDF_FILE_MUSIC:
		CompAddItem(FGetFileMusic);			// na�ten� hudby
		return true;

	default:
		return false;
	}
}


/////////////////////////////////////////////////////////////////////////////
// p�eklad p��kazu s parametrem hudby

bool CompMusPar(int index, PROCCOM func)
{
	return CompMusPar(index, func, 0, -1);
}

bool CompMusPar(int index, PROCCOM func, int data)
{
	return CompMusPar(index, func, data, -1);
}

bool CompMusPar(int index, PROCCOM func, int data, int list)
{
	int zahlavi = CompAddItem(func, data, list);

	if ((BufEdi2[index].Items == 1) || !CompMus(index+1))
	{
		ProgNum = zahlavi;
		return false;
	}
	return true;
}
