
#include "Main.h"

/***************************************************************************\
*																			*
*						Kompilace programu - zvuk							*
*																			*
\***************************************************************************/

#pragma optimize("s", on)			// optimalizace na minimální velikost


/////////////////////////////////////////////////////////////////////////////
// pøeklad výrazu se zvukem (vrací true = operace OK)

bool _fastcall CompSnd(int index)
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
	case IDF_SND:
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			item = BufObj + refinx;
			item2 = BufObj2 + refinx;
			if (Sound.IsNotValid(item2->Data)) return false;

			if (item2->List < 0)
			{
				CompAddItem(FSoundObj, item2->Data);
			}
			else
			{
				CompAddItem(FSoundObjList, item2->Data, item2->List);
			}
			return true;
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			item = BufLoc + refinx;
			item2 = BufLoc2 + refinx;
			if (Sound.IsNotValid(item2->Data)) return false;

			if (item2->List < 0)
			{
				CompAddItem(FSoundLoc, item2->Local + KorigSnd);
			}
			else
			{
				CompAddItem(FSoundLocList, item2->Local + KorigSnd, item2->LocalList + KorigLst);
			}
			return true;
		}
		return false;

	case IDF_FNC:
		return CompFunc(index, IDF_SND);	// funkce s návratem zvuku

	case IDF_SOUND_CONV_8BIT:
		return CompSndPar(index, FSoundConv8Bit);	// konverze na 8 bitù

	case IDF_SOUND_CONV_16BIT:
		return CompSndPar(index, FSoundConv16Bit);	// konverze na 16 bitù

	case IDF_SOUND_CONV_STEREO:
		return CompSndPar(index, FSoundConvStereo);	// konverze na stereo

	case IDF_SOUND_CONV_MONO:
		return CompSndPar(index, FSoundConvMono);	// konverze na mono

	case IDF_SOUND_CONV_11025:
		return CompSndPar(index, FSoundConv11025);	// konverze na frekvenci 11025

	case IDF_SOUND_CONV_22050:
		return CompSndPar(index, FSoundConv22050);	// konverze na frekvenci 22050

	case IDF_SOUND_CONV_44100:
		return CompSndPar(index, FSoundConv44100);	// konverze na frekvenci 44100

	case IDF_SOUND_ADD:
		return CompSndGrp(index, FSoundAdd, FSoundAdd1);	// souèet zvukù

	case IDF_SOUND_SPEED:							// konverze rychlosti zvuku
		CompAddItem(FSoundSpeed);
		CompSndSubPar(index, IDF_SOUND_INPUT);
		CompNumSubPar(index, IDF_SOUND_SPEED_K, 1);
		return true;

	case IDF_TONGEN:								// tónový generátor
		CompAddItem(FTonGen);
		CompNumSubPar(index, IDF_TONGEN_F, 1000);
		CompNumSubPar(index, IDF_TONGEN_T, 1);
		return true;

	case IDF_FILE_SOUND:
		CompAddItem(FGetFileSound);			// naètení zvuku
		return true;

	case IDF_SOUND_CONV_PCM:
		return CompSndPar(index, FSoundConvPCM);	// dekomprimace zvuku

	default:
		return false;
	}
}


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu s parametrem zvuku

bool CompSndPar(int index, PROCCOM func)
{
	return CompSndPar(index, func, 0, -1);
}

bool CompSndPar(int index, PROCCOM func, int data)
{
	return CompSndPar(index, func, data, -1);
}

bool CompSndPar(int index, PROCCOM func, int data, int list)
{
	int zahlavi = CompAddItem(func, data, list);

	if ((BufEdi2[index].Items == 1) || !CompSnd(index+1))
	{
		ProgNum = zahlavi;
		return false;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu s podparametrem zvuku (hledaným podle identifikace)

void CompSndSubPar(int index, int idf)
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
				CompSnd(index + 1))
			{
				return;
			}

// posun pro pøíští prvek
			posun = item2->Items;

// dokud je další potomek
		} while (item->Param & PETPROG_NEXT);
	}

// použije se implicitní hodnota - prázdný zvuk
	CompAddItem(FSoundEmpty);
}
