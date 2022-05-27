
#include "Main.h"

/***************************************************************************\
*																			*
*						Kompilace programu - plocha							*
*																			*
\***************************************************************************/

#pragma optimize("s", on)			// optimalizace na minimální velikost


/////////////////////////////////////////////////////////////////////////////
// pøeklad výrazu s plochou (vrací true = operace OK)

bool _fastcall CompMap(int index)
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
	case IDF_MAP:
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			item = BufObj + refinx;
			item2 = BufObj2 + refinx;
			if (Map.IsNotValid(item2->Data)) return false;

			if (item2->List < 0)
			{
				CompAddItem(FMapObj, item2->Data);
			}
			else
			{
				CompAddItem(FMapObjList, item2->Data, item2->List);
			}
			return true;
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			item = BufLoc + refinx;
			item2 = BufLoc2 + refinx;
			if (Map.IsNotValid(item2->Data)) return false;

			if (item2->List < 0)
			{
				CompAddItem(FMapLoc, item2->Local + KorigMap);
			}
			else
			{
				CompAddItem(FMapLocList, item2->Local + KorigMap, item2->LocalList + KorigLst);
			}
			return true;
		}
		return false;

	case IDF_FNC:
		return CompFunc(index, IDF_MAP);	// funkce s návratem plochy

	case IDF_MAP_COPY:
		CompAddItem(FMapCopy);				// kopie plochy
		CompMapSubPar(index, IDF_MAP_INPUT);
		CompNumSubPar(index, IDF_MAP_X, 0);
		CompNumSubPar(index, IDF_MAP_Y, 0);
		CompMapSubPar(index, IDF_MAP_COPY_INPUT);
		CompNumSubPar(index, IDF_MAP_COPY_X, 0);
		CompNumSubPar(index, IDF_MAP_COPY_Y, 0);
		CompNumSubPar(index, IDF_MAP_COPY_W, 2000000);
		CompNumSubPar(index, IDF_MAP_COPY_H, 2000000);
		return true;

	case IDF_MAP_NEW:
		CompAddItem(FMapNew);				// nová plocha
		CompNumSubPar(index, IDF_MAP_NEW_W, 0);
		CompNumSubPar(index, IDF_MAP_NEW_H, 0);
		return true;

	default:
		return false;
	}
}


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu s parametrem plochy

bool CompMapPar(int index, PROCCOM func)
{
	return CompMapPar(index, func, 0, -1);
}

bool CompMapPar(int index, PROCCOM func, int data)
{
	return CompMapPar(index, func, data, -1);
}

bool CompMapPar(int index, PROCCOM func, int data, int list)
{
	int zahlavi = CompAddItem(func, data, list);

	if ((BufEdi2[index].Items == 1) || !CompMap(index+1))
	{
		ProgNum = zahlavi;
		return false;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu s podparametrem plochy (hledaným podle identifikace)

void CompMapSubPar(int index, int idf)
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
				CompMap(index + 1))
			{
				return;
			}

// posun pro pøíští prvek
			posun = item2->Items;

// dokud je další potomek
		} while (item->Param & PETPROG_NEXT);
	}

// použije se implicitní hodnota - hlavní plocha
	CompAddItem(FMapMain);
}
