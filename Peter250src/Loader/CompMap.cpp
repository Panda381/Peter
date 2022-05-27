
#include "Main.h"

/***************************************************************************\
*																			*
*						Kompilace programu - plocha							*
*																			*
\***************************************************************************/

#pragma optimize("s", on)			// optimalizace na minim�ln� velikost


/////////////////////////////////////////////////////////////////////////////
// p�eklad v�razu s plochou (vrac� true = operace OK)

bool _fastcall CompMap(int index)
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
		return CompFunc(index, IDF_MAP);	// funkce s n�vratem plochy

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
		CompAddItem(FMapNew);				// nov� plocha
		CompNumSubPar(index, IDF_MAP_NEW_W, 0);
		CompNumSubPar(index, IDF_MAP_NEW_H, 0);
		return true;

	default:
		return false;
	}
}


/////////////////////////////////////////////////////////////////////////////
// p�eklad p��kazu s parametrem plochy

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
// p�eklad p��kazu s podparametrem plochy (hledan�m podle identifikace)

void CompMapSubPar(int index, int idf)
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
				CompMap(index + 1))
			{
				return;
			}

// posun pro p��t� prvek
			posun = item2->Items;

// dokud je dal�� potomek
		} while (item->Param & PETPROG_NEXT);
	}

// pou�ije se implicitn� hodnota - hlavn� plocha
	CompAddItem(FMapMain);
}
