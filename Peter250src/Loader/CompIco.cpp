
#include "Main.h"

/***************************************************************************\
*																			*
*						Kompilace programu - ikona							*
*																			*
\***************************************************************************/

#pragma optimize("s", on)			// optimalizace na minimální velikost


/////////////////////////////////////////////////////////////////////////////
// pøeklad výrazu s ikonou (vrací true = operace OK)

bool _fastcall CompIco(int index)
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
	case IDF_PREDMET:
		CompAddItem(FPredmet);				// pøedmìt pøed Petøíkem
		return true;

	case IDF_PREDMET_POZ:
		CompAddItem(FPredmetPoz);			// pøedmìt pod Petøíkem
		return true;

	case IDF_PETRA_PREDMET:
		CompAddItem(FPredmet2);				// pøedmìt pøed Petrou
		return true;

	case IDF_PETRA_PREDMET_POZ:
		CompAddItem(FPredmet2Poz);			// pøedmìt pod Petrou
		return true;

	case IDF_OKRAJ:
		CompAddItem(FIconEmpty);			// prázdná ikona
		return true;

	case IDF_FNC:
		return CompFunc(index, IDF_ICON);	// funkce s návratem ikony

	case IDF_MAP_ICON:
		CompAddItem(FGetMapIcon);			// ikona z plochy
		CompMapSubPar(index, IDF_MAP_INPUT);
		CompNumSubPar(index, IDF_MAP_X, 0);
		CompNumSubPar(index, IDF_MAP_Y, 0);
		return true;

	case IDF_NONE:
	case IDF_ICON:							// promìnná ikony
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
		CompAddItem(FGetFileIcon);			// naètení ikony
		return true;

	case IDF_ICON_GET:
		CompAddItem(FIconGet);					// naètení ikony
		CompNumSubPar(index, IDF_PIC_BLOCK_X, 0);
		CompNumSubPar(index, IDF_PIC_BLOCK_Y, 0);
		return true;

	default:
		return false;
	}
}


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu s parametrem ikony

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
// pøeklad pøíkazu s podparametrem ikony (hledaným podle identifikace)

void CompIcoSubPar(int index, int idf)
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
				CompIco(index + 1))
			{
				return;
			}

// posun pro pøíští prvek
			posun = item2->Items;

// dokud je další potomek
		} while (item->Param & PETPROG_NEXT);
	}

// použije se implicitní hodnota - prázdná
	CompAddItem(FIconEmpty);
}
