
#include "Main.h"

/***************************************************************************\
*																			*
*						Kompilace programu - ikona							*
*																			*
\***************************************************************************/

#pragma optimize("s", on)			// optimalizace na minim�ln� velikost


/////////////////////////////////////////////////////////////////////////////
// p�eklad v�razu s ikonou (vrac� true = operace OK)

bool _fastcall CompIco(int index)
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
	case IDF_PREDMET:
		CompAddItem(FPredmet);				// p�edm�t p�ed Pet��kem
		return true;

	case IDF_PREDMET_POZ:
		CompAddItem(FPredmetPoz);			// p�edm�t pod Pet��kem
		return true;

	case IDF_PETRA_PREDMET:
		CompAddItem(FPredmet2);				// p�edm�t p�ed Petrou
		return true;

	case IDF_PETRA_PREDMET_POZ:
		CompAddItem(FPredmet2Poz);			// p�edm�t pod Petrou
		return true;

	case IDF_OKRAJ:
		CompAddItem(FIconEmpty);			// pr�zdn� ikona
		return true;

	case IDF_FNC:
		return CompFunc(index, IDF_ICON);	// funkce s n�vratem ikony

	case IDF_MAP_ICON:
		CompAddItem(FGetMapIcon);			// ikona z plochy
		CompMapSubPar(index, IDF_MAP_INPUT);
		CompNumSubPar(index, IDF_MAP_X, 0);
		CompNumSubPar(index, IDF_MAP_Y, 0);
		return true;

	case IDF_NONE:
	case IDF_ICON:							// prom�nn� ikony
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			item = BufObj + refinx;
			item2 = BufObj2 + refinx;
			if (Icon.IsNotValid(item2->Data)) return false;

			if (item2->List < 0)
			{
				CompAddItem(FIconObj, item2->Data);
			}
			else
			{
				CompAddItem(FIconObjList, item2->Data, item2->List);
			}
			return true;
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			item = BufLoc + refinx;
			item2 = BufLoc2 + refinx;
			if (Icon.IsNotValid(item2->Data)) return false;

			if (item2->List < 0)
			{
				CompAddItem(FIconLoc, item2->Local + KorigIco);
			}
			else
			{
				CompAddItem(FIconLocList, item2->Local + KorigIco, item2->LocalList + KorigLst);
			}
			return true;
		}
		return false;
	
	case IDF_WINDOW_ICON:
		CompAddItem(FGetWindowIcon);		// ikona okna
		return true;

	case IDF_FILE_ICON:
		CompAddItem(FGetFileIcon);			// na�ten� ikony
		return true;

	case IDF_ICON_GET:
		CompAddItem(FIconGet);					// na�ten� ikony
		CompNumSubPar(index, IDF_PIC_BLOCK_X, 0);
		CompNumSubPar(index, IDF_PIC_BLOCK_Y, 0);
		return true;

	default:
		return false;
	}
}


/////////////////////////////////////////////////////////////////////////////
// p�eklad p��kazu s parametrem ikony

bool CompIcoPar(int index, PROCCOM func)
{
	return CompIcoPar(index, func, 0, -1);
}

bool CompIcoPar(int index, PROCCOM func, int data)
{
	return CompIcoPar(index, func, data, -1);
}

bool CompIcoPar(int index, PROCCOM func, int data, int list)
{
	int zahlavi = CompAddItem(func, data, list);

	if ((BufEdi2[index].Items == 1) || !CompIco(index+1))
	{
		ProgNum = zahlavi;
		return false;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// p�eklad p��kazu s podparametrem ikony (hledan�m podle identifikace)

void CompIcoSubPar(int index, int idf)
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
				CompIco(index + 1))
			{
				return;
			}

// posun pro p��t� prvek
			posun = item2->Items;

// dokud je dal�� potomek
		} while (item->Param & PETPROG_NEXT);
	}

// pou�ije se implicitn� hodnota - pr�zdn�
	CompAddItem(FIconEmpty);
}
