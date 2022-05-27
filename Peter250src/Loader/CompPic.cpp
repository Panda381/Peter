
#include "Main.h"

/***************************************************************************\
*																			*
*						Kompilace programu - obr�zek						*
*																			*
\***************************************************************************/

#pragma optimize("s", on)			// optimalizace na minim�ln� velikost


/////////////////////////////////////////////////////////////////////////////
// p�eklad v�razu s obr�zkem (vrac� true = operace OK)

bool _fastcall CompPic(int index)
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
		return CompFunc(index, IDF_PIC);	// funkce s n�vratem obr�zku

	case IDF_PIC_GET:
		CompAddItem(FPicGet);					// na�ten� obr�zku
		CompNumSubPar(index, IDF_PIC_BLOCK_X, 0);
		CompNumSubPar(index, IDF_PIC_BLOCK_Y, 0);
		CompNumSubPar(index, IDF_PIC_BLOCK_W, 2000000);
		CompNumSubPar(index, IDF_PIC_BLOCK_H, 2000000);
		return true;

	case IDF_FILE_PICTURE:
		CompAddItem(FGetFilePicture);			// na�ten� obr�zku
		return true;

	case IDF_PICTURE_XFLIP:
		return CompPicPar(index, FPicXFlip);	// horizont�ln� p�evr�cen�

	case IDF_PICTURE_YFLIP:
		return CompPicPar(index, FPicYFlip);	// vertik�ln� p�evr�cen�

	case IDF_PICTURE_ZOOM:
		CompAddItem(FPicZoom);					// zm�na velikosti obr�zku
		CompPicSubPar(index, IDF_PICTURE_ZOOM_PIC);
		CompNumSubPar(index, IDF_PICTURE_ZOOM_WIDTH, 2000000);
		CompNumSubPar(index, IDF_PICTURE_ZOOM_HEIGHT, 2000000);
		CompLogSubPar(index, IDF_PICTURE_ZOOM_INT, FALSE);
		CompLogSubPar(index, IDF_PICTURE_ZOOM_DITH, TRUE);
		return true;

	case IDF_PICTURE_LEVEL:
		CompAddItem(FPicLevel);					// zm�na jasu obr�zku
		CompPicSubPar(index, IDF_PICTURE_LEVEL_PIC);
		CompNumSubPar(index, IDF_PICTURE_LEVEL_N, 1);
		CompLogSubPar(index, IDF_PICTURE_ZOOM_DITH, FALSE);
		return true;

	case IDF_PICTURE_ROTATE:
		CompAddItem(FPicRotate);					// oto�en� obr�zku
		CompPicSubPar(index, IDF_PICTURE_ROTATE_PIC);
		CompNumSubPar(index, IDF_PICTURE_ROTATE_ANGLE, 0);
		CompLogSubPar(index, IDF_PICTURE_ZOOM_INT, FALSE);
		CompLogSubPar(index, IDF_PICTURE_ZOOM_DITH, TRUE);
		return true;

	case IDF_WINDOW_PIC:
		CompAddItem(FGetWindowPic);		// obr�zek okna
		return true;

	case IDF_MAP_PICTURE:
		return CompMapPar(index, FMapPicture);	// konverze plochy na obr�zek

	case IDF_ICON_PICTURE:
		return CompIcoPar(index, FIconPicture);	// konverze ikony na obr�zek

	case IDF_PICTURE_MASK:
		CompAddItem(FPicMask);					// maskov�n� obr�zku
		CompPicSubPar(index, IDF_PICTURE_MASK_PIC);
		CompPicSubPar(index, IDF_PICTURE_MASK_MASK);
		return true;

	case IDF_PICTURE_XCOL:
		CompAddItem(FPicXCol);					// z�m�na barev obr�zku
		CompPicSubPar(index, IDF_PICTURE_MASK_PIC);
		CompNumSubPar(index, IDF_PICTURE_XCOL1, 0xFFFFFF);
		CompNumSubPar(index, IDF_PICTURE_XCOL2, 0);
		return true;

	case IDF_PICTURE_SCOL:
		CompAddItem(FPicSCol);					// n�hrada barvy obr�zku
		CompPicSubPar(index, IDF_PICTURE_MASK_PIC);
		CompNumSubPar(index, IDF_PICTURE_SCOL1, 0xFFFFFF);
		CompNumSubPar(index, IDF_PICTURE_SCOL2, 0);
		return true;

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_DIRECT3D_TEXTURE:					// zji�t�n� textury obr�zku Direct3D
		CompAddItem(FGetD3DTexture);
		return true;

	case IDF_DIRECT3D_SCENE_TEXTURE:			// zji�t�n� textury pozad� sc�ny
		CompAddItem(FGetD3DSceneTexture);
		return true;

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------

	default:
		return false;
	}
}


/////////////////////////////////////////////////////////////////////////////
// p�eklad p��kazu s parametrem obr�zku

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
// p�eklad p��kazu s podparametrem obr�zku (hledan�m podle identifikace)

void CompPicSubPar(int index, int idf)
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
				CompPic(index + 1))
			{
				return;
			}

// posun pro p��t� prvek
			posun = item2->Items;

// dokud je dal�� potomek
		} while (item->Param & PETPROG_NEXT);
	}

// pou�ije se implicitn� hodnota - pr�zdn� text
	CompAddItem(FPicEmpty);
}
