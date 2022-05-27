
#include "Main.h"

/***************************************************************************\
*																			*
*						Kompilace programu - obrázek						*
*																			*
\***************************************************************************/

#pragma optimize("s", on)			// optimalizace na minimální velikost


/////////////////////////////////////////////////////////////////////////////
// pøeklad výrazu s obrázkem (vrací true = operace OK)

bool _fastcall CompPic(int index)
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
	case IDF_PIC:
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			item = BufObj + refinx;
			item2 = BufObj2 + refinx;
			if (Picture.IsNotValid(item2->Data)) return false;

			if (item2->List < 0)
			{
				CompAddItem(FPicObj, item2->Data);
			}
			else
			{
				CompAddItem(FPicObjList, item2->Data, item2->List);
			}
			return true;
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			item = BufLoc + refinx;
			item2 = BufLoc2 + refinx;
			if (Picture.IsNotValid(item2->Data)) return false;

			if (item2->List < 0)
			{
				CompAddItem(FPicLoc, item2->Local + KorigPic);
			}
			else
			{
				CompAddItem(FPicLocList, item2->Local + KorigPic, item2->LocalList + KorigLst);
			}
			return true;
		}
		return false;

	case IDF_FNC:
		return CompFunc(index, IDF_PIC);	// funkce s návratem obrázku

	case IDF_PIC_GET:
		CompAddItem(FPicGet);					// naètení obrázku
		CompNumSubPar(index, IDF_PIC_BLOCK_X, 0);
		CompNumSubPar(index, IDF_PIC_BLOCK_Y, 0);
		CompNumSubPar(index, IDF_PIC_BLOCK_W, 2000000);
		CompNumSubPar(index, IDF_PIC_BLOCK_H, 2000000);
		return true;

	case IDF_FILE_PICTURE:
		CompAddItem(FGetFilePicture);			// naètení obrázku
		return true;

	case IDF_PICTURE_XFLIP:
		return CompPicPar(index, FPicXFlip);	// horizontální pøevrácení

	case IDF_PICTURE_YFLIP:
		return CompPicPar(index, FPicYFlip);	// vertikální pøevrácení

	case IDF_PICTURE_ZOOM:
		CompAddItem(FPicZoom);					// zmìna velikosti obrázku
		CompPicSubPar(index, IDF_PICTURE_ZOOM_PIC);
		CompNumSubPar(index, IDF_PICTURE_ZOOM_WIDTH, 2000000);
		CompNumSubPar(index, IDF_PICTURE_ZOOM_HEIGHT, 2000000);
		CompLogSubPar(index, IDF_PICTURE_ZOOM_INT, FALSE);
		CompLogSubPar(index, IDF_PICTURE_ZOOM_DITH, TRUE);
		return true;

	case IDF_PICTURE_LEVEL:
		CompAddItem(FPicLevel);					// zmìna jasu obrázku
		CompPicSubPar(index, IDF_PICTURE_LEVEL_PIC);
		CompNumSubPar(index, IDF_PICTURE_LEVEL_N, 1);
		CompLogSubPar(index, IDF_PICTURE_ZOOM_DITH, FALSE);
		return true;

	case IDF_PICTURE_ROTATE:
		CompAddItem(FPicRotate);					// otoèení obrázku
		CompPicSubPar(index, IDF_PICTURE_ROTATE_PIC);
		CompNumSubPar(index, IDF_PICTURE_ROTATE_ANGLE, 0);
		CompLogSubPar(index, IDF_PICTURE_ZOOM_INT, FALSE);
		CompLogSubPar(index, IDF_PICTURE_ZOOM_DITH, TRUE);
		return true;

	case IDF_WINDOW_PIC:
		CompAddItem(FGetWindowPic);		// obrázek okna
		return true;

	case IDF_MAP_PICTURE:
		return CompMapPar(index, FMapPicture);	// konverze plochy na obrázek

	case IDF_ICON_PICTURE:
		return CompIcoPar(index, FIconPicture);	// konverze ikony na obrázek

	case IDF_PICTURE_MASK:
		CompAddItem(FPicMask);					// maskování obrázku
		CompPicSubPar(index, IDF_PICTURE_MASK_PIC);
		CompPicSubPar(index, IDF_PICTURE_MASK_MASK);
		return true;

	case IDF_PICTURE_XCOL:
		CompAddItem(FPicXCol);					// zámìna barev obrázku
		CompPicSubPar(index, IDF_PICTURE_MASK_PIC);
		CompNumSubPar(index, IDF_PICTURE_XCOL1, 0xFFFFFF);
		CompNumSubPar(index, IDF_PICTURE_XCOL2, 0);
		return true;

	case IDF_PICTURE_SCOL:
		CompAddItem(FPicSCol);					// náhrada barvy obrázku
		CompPicSubPar(index, IDF_PICTURE_MASK_PIC);
		CompNumSubPar(index, IDF_PICTURE_SCOL1, 0xFFFFFF);
		CompNumSubPar(index, IDF_PICTURE_SCOL2, 0);
		return true;

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_DIRECT3D_TEXTURE:					// zjištìní textury obrázku Direct3D
		CompAddItem(FGetD3DTexture);
		return true;

	case IDF_DIRECT3D_SCENE_TEXTURE:			// zjištìní textury pozadí scény
		CompAddItem(FGetD3DSceneTexture);
		return true;

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------

	default:
		return false;
	}
}


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu s parametrem obrázku

bool CompPicPar(int index, PROCCOM func)
{
	return CompPicPar(index, func, 0, -1);
}

bool CompPicPar(int index, PROCCOM func, int data)
{
	return CompPicPar(index, func, data, -1);
}

bool CompPicPar(int index, PROCCOM func, int data, int list)
{
	int zahlavi = CompAddItem(func, data, list);

	if ((BufEdi2[index].Items == 1) || !CompPic(index+1))
	{
		ProgNum = zahlavi;
		return false;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu s podparametrem obrázku (hledaným podle identifikace)

void CompPicSubPar(int index, int idf)
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
				CompPic(index + 1))
			{
				return;
			}

// posun pro pøíští prvek
			posun = item2->Items;

// dokud je další potomek
		} while (item->Param & PETPROG_NEXT);
	}

// použije se implicitní hodnota - prázdný text
	CompAddItem(FPicEmpty);
}
