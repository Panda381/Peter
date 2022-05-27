
#include "Main.h"

/***************************************************************************\
*																			*
*							Kompilace programu								*
*																			*
\***************************************************************************/

#pragma optimize("s", on)			// optimalizace na minimální velikost

// korekce lokálních promìnných (pøi volání ve vstupních parametrech)

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
// pøeklad pøíkazu se skupinou (data = poèet parametrù - 1)

bool CompGroup(COMPCOMP comp, int index, PROCCOM func, PROCCOM func1)
{
// úschova indexu záhlaví
	int zahlavi = CompAddItem(func, -1);

// ukazatel výchozího prvku
	PETPROG*	item = BufEdi + index;
	PETPROG2*	item2 = BufEdi2 + index;

// buffer indexù skupin
	CBufInt group;

// buffer indexù k nastavení skokù
	CBufInt jumps;

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

// vnoøení do skupin
			while ((item->Func == IDF_GROUP - IDF) && (item->Param & PETPROG_CHILDS))
			{
				group.Add(index);
				index++;
				item++;
				item2++;
			}

// naètení prvku
			int zahl2 = ProgNum;

			if (comp(index))
			{
				ProgBuf[zahlavi].Data++;

// nastavení skokù v pøedešlých prvcích (vèetnì posledního)
				int n = jumps.Num();
				jumps.Add(zahl2);
				for (; n >= 0; n--)
				{
					zahl2 = jumps[n];
					ProgBuf[zahl2].Jump = ProgNum - zahl2;
				}
			}

// vynoøení ze skupin
			while ((group.Num() > 0) && ((item->Param & PETPROG_NEXT) == 0))
			{
				index = group[group.Num()-1];
				group.Del(1);
				item = BufEdi + index;
				item2 = BufEdi2 + index;
			}

// posun pro pøíští prvek
			posun = item2->Items;

// dokud je další potomek
		} while (item->Param & PETPROG_NEXT);
	}

// kontrola, zda byl alespoò nìjaký prvek
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
// pøeklad volání funkce (index=-1 hlavní funkce, typ = ID promìnné výsledku, IDF_FNC = není)

bool CompFunc(int index, int typ)
{
// index deklarace funkce
	int locinx = 0;								// index funkce v lokálním bufferu
	int ediinx;									// index funkce v editaèním bufferu
	int i;										// pomocný èítaè
	int inx;									// pomocný index

	if (index >= 0)
	{
		locinx = BufEdi[index].RefIndex;			// index funkce v lokálním bufferu

// lokální funkce
		if (BufEdi[index].RefBlok == BufLocID)		// je reference na lokální buffer
		{
			if (((DWORD)locinx >= (DWORD)BufLocN) ||				// platná lokální funkce
				(BufLoc[locinx].DatBlok != BufEdiID)) return false;	// data v editaèním bufferu
			ediinx = BufLoc2[locinx].Data;							// index funkce v editaèním bufferu
			if (((DWORD)ediinx >= (DWORD)BufEdiN) ||				// platná funkce v editaèním bufferu
				(typ != IDF_FNC)) return false;						// nesmí být požadována návratová hodnota
			CompAddItem(FFunc0, ediinx);							// lokální funkce
			return true;
		}

// jinak musí být reference na globální funkci
		if ((BufEdi[index].RefBlok != BufObjID) ||					// musí být odkaz na globální buffer
			((DWORD)locinx >= (DWORD)BufObjN) ||					// platný index globální deklarace
			(BufObj[locinx].DatBlok != BufLocID)) return false;		// odkaz na lokální buffer

// lokální definice
		locinx = BufObj2[locinx].Data;								// deklarace v lokálním bufferu
	}

	if (((DWORD)locinx >= (DWORD)BufLocN) ||					// platná lokální definice
		(BufLoc[locinx].DatBlok != BufEdiID)) return false;		// odkaz na editaèní buffer

// definice v editaèním bufferu
	ediinx = BufLoc2[locinx].Data;								// funkce v editaèním bufferu
	if ((DWORD)ediinx >= (DWORD)BufEdiN) return false;			// platná funkce

// nalezení výstupní promìnné
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

// volání funkce
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

// èítaèe promìnných
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

// pøeklad lokálních promìnných
	int zahlavi = CompAddItem(FCommandExec, -1);
	i = BufLoc2[locinx].Items;
	inx = locinx;
	for (; i > 0; i--)
	{
		int size = 1;
		int data = BufLoc2[inx].Data;
		int list = BufLoc2[inx].List;
		if (list >= 0) size = List[list].Size;

// nalezení vstupní promìnné
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

// vytvoøení vstupní promìnné
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

// uvolnìní lokálních promìnných
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
// pøeklad všech funkcí
	int index = 0;
	while (index < BufEdiN)
	{
		BufEdi2[index].Funkce = ProgNum;
		CompComGrp(index, FCommandExec, FCommandExec1);
		index += BufEdi2[index].Items;
	}

// volání hlavní funkce
	ProgStart = ProgNum;
	if (!CompFunc(-1, IDF_FNC)) ProgStart = 0;

// pøemapování indexù funkcí
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

