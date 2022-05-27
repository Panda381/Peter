
#include "Main.h"

/***************************************************************************\
*																			*
*							Kompilace programu								*
*																			*
\***************************************************************************/

#pragma optimize("s", on)			// optimalizace na minim�ln� velikost

// korekce lok�ln�ch prom�nn�ch (p�i vol�n� ve vstupn�ch parametrech)

int KorigNum = 0;
int KorigLog = 0;
int KorigIco = 0;
int KorigMap = 0;
int KorigMus = 0;
int KorigPic = 0;
int KorigSnd = 0;
int KorigSpr = 0;
int KorigTxt = 0;
int KorigLst = 0;


/////////////////////////////////////////////////////////////////////////////
// p�eklad p��kazu se skupinou (data = po�et parametr� - 1)

bool CompGroup(COMPCOMP comp, int index, PROCCOM func, PROCCOM func1)
{
// �schova indexu z�hlav�
	int zahlavi = CompAddItem(func, -1);

// ukazatel v�choz�ho prvku
	PETPROG*	item = BufEdi + index;
	PETPROG2*	item2 = BufEdi2 + index;

// buffer index� skupin
	CBufInt group;

// buffer index� k nastaven� skok�
	CBufInt jumps;

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

// vno�en� do skupin
			while ((item->Func == IDF_GROUP - IDF) && (item->Param & PETPROG_CHILDS))
			{
				group.Add(index);
				index++;
				item++;
				item2++;
			}

// na�ten� prvku
			int zahl2 = ProgNum;

			if (comp(index))
			{
				ProgBuf[zahlavi].Data++;

// nastaven� skok� v p�ede�l�ch prvc�ch (v�etn� posledn�ho)
				int n = jumps.Num();
				jumps.Add(zahl2);
				for (; n >= 0; n--)
				{
					zahl2 = jumps[n];
					ProgBuf[zahl2].Jump = ProgNum - zahl2;
				}
			}

// vyno�en� ze skupin
			while ((group.Num() > 0) && ((item->Param & PETPROG_NEXT) == 0))
			{
				index = group[group.Num()-1];
				group.Del(1);
				item = BufEdi + index;
				item2 = BufEdi2 + index;
			}

// posun pro p��t� prvek
			posun = item2->Items;

// dokud je dal�� potomek
		} while (item->Param & PETPROG_NEXT);
	}

// kontrola, zda byl alespo� n�jak� prvek
	if (ProgBuf[zahlavi].Data < 0)
	{
		if (func1 == FCommandExec1)
		{
			ProgBuf[zahlavi].ExecCom = FCommandExec0;
		}
		else
		{
			ProgNum = zahlavi;
			return false;
		}
	}

// korekce funkce pro 1 prvek
	if (ProgBuf[zahlavi].Data == 0)
	{
		ProgBuf[zahlavi].ExecCom = func1;
	}

	return true;
}


/////////////////////////////////////////////////////////////////////////////
// p�eklad vol�n� funkce (index=-1 hlavn� funkce, typ = ID prom�nn� v�sledku, IDF_FNC = nen�)

bool CompFunc(int index, int typ)
{
// index deklarace funkce
	int locinx = 0;								// index funkce v lok�ln�m bufferu
	int ediinx;									// index funkce v edita�n�m bufferu
	int i;										// pomocn� ��ta�
	int inx;									// pomocn� index

	if (index >= 0)
	{
		locinx = BufEdi[index].RefIndex;			// index funkce v lok�ln�m bufferu

// lok�ln� funkce
		if (BufEdi[index].RefBlok == BufLocID)		// je reference na lok�ln� buffer
		{
			if (((DWORD)locinx >= (DWORD)BufLocN) ||				// platn� lok�ln� funkce
				(BufLoc[locinx].DatBlok != BufEdiID)) return false;	// data v edita�n�m bufferu
			ediinx = BufLoc2[locinx].Data;							// index funkce v edita�n�m bufferu
			if (((DWORD)ediinx >= (DWORD)BufEdiN) ||				// platn� funkce v edita�n�m bufferu
				(typ != IDF_FNC)) return false;						// nesm� b�t po�adov�na n�vratov� hodnota
			CompAddItem(FFunc0, ediinx);							// lok�ln� funkce
			return true;
		}

// jinak mus� b�t reference na glob�ln� funkci
		if ((BufEdi[index].RefBlok != BufObjID) ||					// mus� b�t odkaz na glob�ln� buffer
			((DWORD)locinx >= (DWORD)BufObjN) ||					// platn� index glob�ln� deklarace
			(BufObj[locinx].DatBlok != BufLocID)) return false;		// odkaz na lok�ln� buffer

// lok�ln� definice
		locinx = BufObj2[locinx].Data;								// deklarace v lok�ln�m bufferu
	}

	if (((DWORD)locinx >= (DWORD)BufLocN) ||					// platn� lok�ln� definice
		(BufLoc[locinx].DatBlok != BufEdiID)) return false;		// odkaz na edita�n� buffer

// definice v edita�n�m bufferu
	ediinx = BufLoc2[locinx].Data;								// funkce v edita�n�m bufferu
	if ((DWORD)ediinx >= (DWORD)BufEdiN) return false;			// platn� funkce

// nalezen� v�stupn� prom�nn�
	int resinx = -1;
	if (typ != IDF_FNC)
	{
		if ((BufLoc[locinx].Param & PETPROG_CHILDS) == 0) return false;
		inx = locinx+1;
		for (;;)
		{
			if (BufLoc[inx].Func == IDF_OUT - IDF)
			{
				i = BufLoc2[inx].Items;
				for (; i > 0; i--)
				{
					if (BufLoc[inx].Func == typ - IDF)
					{
						resinx = BufLoc2[inx].Local;
						break;
					}

					inx++;
				}
				break;
			}
			if ((BufLoc[inx].Param & PETPROG_NEXT) == 0) return false;
			inx += BufLoc2[inx].Items;
		}

		if (resinx == -1) return false;
	}

// vol�n� funkce
	switch (typ)
	{
	case IDF_REAL:
		CompAddItem(FNumFunc, ediinx, resinx);
		break;

	case IDF_TEXT:
		CompAddItem(FTextFunc, ediinx, resinx);
		break;

	case IDF_BOOL:
		CompAddItem(FBoolFunc, ediinx, resinx);
		break;

	case IDF_ICON:
		CompAddItem(FIconFunc, ediinx, resinx);
		break;

	case IDF_MAP:
		CompAddItem(FMapFunc, ediinx, resinx);
		break;

	case IDF_PIC:
		CompAddItem(FPicFunc, ediinx, resinx);
		break;

	case IDF_SPRITE:
		CompAddItem(FSpriteFunc, ediinx, resinx);
		break;

	case IDF_SND:
		CompAddItem(FSoundFunc, ediinx, resinx);
		break;

	case IDF_MUS:
		CompAddItem(FMusicFunc, ediinx, resinx);
		break;

	case IDF_FNC:
		CompAddItem(FFunc, ediinx);
		break;

	default:
		return false;
	}

// ��ta�e prom�nn�ch
	int locreal = 0;
	int loctext = 0;
	int locbool = 0;
	int locicon = 0;
	int locmap = 0;
	int locpic = 0;
	int locsprite = 0;
	int locsound = 0;
	int locmusic = 0;
	int loclist = 0;

// p�eklad lok�ln�ch prom�nn�ch
	int zahlavi = CompAddItem(FCommandExec, -1);
	i = BufLoc2[locinx].Items;
	inx = locinx;
	for (; i > 0; i--)
	{
		int size = 1;
		int data = BufLoc2[inx].Data;
		int list = BufLoc2[inx].List;
		if (list >= 0) size = List[list].Size;

// nalezen� vstupn� prom�nn�
		int parinx = -1;
		if ((index >= 0) && (size == 1) && ((BufEdi[index].Param & PETPROG_CHILDS) != 0))
		{
			int j = index+1;
			for (;;)
			{
				if ((BufEdi[j].RefIndex == inx) &&
					(BufEdi2[j].Items > 1) &&
					(BufEdi[j].RefBlok == BufLocID))
				{
					parinx = j;
					break;
				}

				if ((BufEdi[j].Param & PETPROG_NEXT) == 0) break;
				j += BufEdi2[j].Items;
			}
		}

// vytvo�en� vstupn� prom�nn�
		switch (BufLoc[inx].Func)
		{
		case IDF_REAL - IDF:
			if ((parinx < 0) || (!CompNumPar(parinx, FNewReal)))
			{
				if (size == 1)
				{
					CompAddItem(FDupReal, data);
				}
				else
				{
					CompAddItem(FDupRealList, data, size);
				}
			}
			locreal += size;
			KorigNum += size;
			ProgBuf[zahlavi].Data++;
			break;

		case IDF_TEXT - IDF:
			if ((parinx < 0) || (!CompTxtPar(parinx, FNewText)))
			{
				if (size == 1)
				{
					CompAddItem(FDupText, data);
				}
				else
				{
					CompAddItem(FDupTextList, data, size);
				}
			}
			loctext += size;
			KorigTxt += size;
			ProgBuf[zahlavi].Data++;
			break;

		case IDF_BOOL - IDF:
			if ((parinx < 0) || (!CompLogPar(parinx, FNewBool)))
			{
				if (size == 1)
				{
					CompAddItem(FDupBool, data);
				}
				else
				{
					CompAddItem(FDupBoolList, data, size);
				}
			}
			locbool += size;
			KorigLog += size;
			ProgBuf[zahlavi].Data++;
			break;

		case IDF_ICON - IDF:
			if ((parinx < 0) || (!CompIcoPar(parinx, FNewIcon)))
			{
				if (size == 1)
				{
					CompAddItem(FDupIcon, data);
				}
				else
				{
					CompAddItem(FDupIconList, data, size);
				}
			}
			locicon += size;
			KorigIco += size;
			ProgBuf[zahlavi].Data++;
			break;

		case IDF_MAP - IDF:
			if ((parinx < 0) || (!CompMapPar(parinx, FNewMap)))
			{
				if (size == 1)
				{
					CompAddItem(FDupMap, data);
				}
				else
				{
					CompAddItem(FDupMapList, data, size);
				}
			}
			locmap += size;
			KorigMap += size;
			ProgBuf[zahlavi].Data++;
			break;

		case IDF_PIC - IDF:
			if ((parinx < 0) || (!CompPicPar(parinx, FNewPicture)))
			{
				if (size == 1)
				{
					CompAddItem(FDupPicture, data);
				}
				else
				{
					CompAddItem(FDupPictureList, data, size);
				}
			}
			locpic += size;
			KorigPic += size;
			ProgBuf[zahlavi].Data++;
			break;

		case IDF_SPRITE - IDF:
			if ((parinx < 0) || (!CompSprPar(parinx, FNewSprite)))
			{
				if (size == 1)
				{
					CompAddItem(FDupSprite, data);
				}
				else
				{
					CompAddItem(FDupSpriteList, data, size);
				}
			}
			locsprite += size;
			KorigSpr += size;
			ProgBuf[zahlavi].Data++;
			break;

		case IDF_SND - IDF:
			if ((parinx < 0) || (!CompSndPar(parinx, FNewSound)))
			{
				if (size == 1)
				{
					CompAddItem(FDupSound, data);
				}
				else
				{
					CompAddItem(FDupSoundList, data, size);
				}
			}
			locsound += size;
			KorigSnd += size;
			ProgBuf[zahlavi].Data++;
			break;

		case IDF_MUS - IDF:
			if ((parinx < 0) || (!CompMusPar(parinx, FNewMusic)))
			{
				if (size == 1)
				{
					CompAddItem(FDupMusic, data);
				}
				else
				{
					CompAddItem(FDupMusicList, data, size);
				}
			}
			locmusic += size;
			KorigMus += size;
			ProgBuf[zahlavi].Data++;
			break;

		case IDF_LIST - IDF:
			CompAddItem(FDupList, list);
			loclist += 1;
			KorigLst += 1;
			ProgBuf[zahlavi].Data++;
			break;
		}
		inx++;
	}

// uvoln�n� lok�ln�ch prom�nn�ch
	zahlavi = CompAddItem(FCommandExec, -1);
	if (locreal > 0)
	{
		CompAddItem(FDelReal, locreal);
		ProgBuf[zahlavi].Data++;
		KorigNum -= locreal;
	}

	if (loctext > 0)
	{
		CompAddItem(FDelText, loctext);
		ProgBuf[zahlavi].Data++;
		KorigTxt -= loctext;
	}

	if (locbool > 0)
	{
		CompAddItem(FDelBool, locbool);
		ProgBuf[zahlavi].Data++;
		KorigLog -= locbool;
	}

	if (locicon > 0)
	{
		CompAddItem(FDelIcon, locicon);
		ProgBuf[zahlavi].Data++;
		KorigIco -= locicon;
	}

	if (locmap > 0)
	{
		CompAddItem(FDelMap, locmap);
		ProgBuf[zahlavi].Data++;
		KorigMap -= locmap;
	}

	if (locpic > 0)
	{
		CompAddItem(FDelPicture, locpic);
		ProgBuf[zahlavi].Data++;
		KorigPic -= locpic;
	}

	if (locsprite > 0)
	{
		CompAddItem(FDelSprite, locsprite);
		ProgBuf[zahlavi].Data++;
		KorigSpr -= locsprite;
	}

	if (locsound > 0)
	{
		CompAddItem(FDelSound, locsound);
		ProgBuf[zahlavi].Data++;
		KorigSnd -= locsound;
	}

	if (locmusic > 0)
	{
		CompAddItem(FDelMusic, locmusic);
		ProgBuf[zahlavi].Data++;
		KorigMus -= locmusic;
	}

	if (loclist > 0)
	{
		CompAddItem(FDelList, loclist);
		ProgBuf[zahlavi].Data++;
		KorigLst -= loclist;
	}

	return true;
}


/////////////////////////////////////////////////////////////////////////////
// kompilace programu

void Comp()
{
// p�eklad v�ech funkc�
	int index = 0;
	while (index < BufEdiN)
	{
		BufEdi2[index].Funkce = ProgNum;
		CompComGrp(index, FCommandExec, FCommandExec1);
		index += BufEdi2[index].Items;
	}

// vol�n� hlavn� funkce
	ProgStart = ProgNum;
	if (!CompFunc(-1, IDF_FNC)) ProgStart = 0;

// p�emapov�n� index� funkc�
	EXECITEM* item = ProgBuf;
	for (int i = ProgNum; i > 0; i--)
	{
		if (
				(item->ExecCom == FFunc0) ||
				(item->ExecCom == FFunc) ||
				(item->ExecNum == FNumFunc) ||
				(item->ExecTxt == FTextFunc) ||
				(item->ExecLog == FBoolFunc) ||
				(item->ExecIco == FIconFunc) ||
				(item->ExecMap == FMapFunc) ||
				(item->ExecPic == FPicFunc) ||
				(item->ExecSpr == FSpriteFunc) ||
				(item->ExecSnd == FSoundFunc) ||
				(item->ExecMus == FMusicFunc)
			)
		{
			item->Data = BufEdi2[item->Data].Funkce;
		}
		item++;
	}
}

