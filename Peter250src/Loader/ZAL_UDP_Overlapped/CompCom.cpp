
#include "Main.h"

/***************************************************************************\
*																			*
*						Kompilace programu - pøíkaz							*
*																			*
\***************************************************************************/

#pragma optimize("s", on)			// optimalizace na minimální velikost


/////////////////////////////////////////////////////////////////////////////
// pøidání jednoho prvku do bufferu programu (vrací index prvku)

int CompAddItem(PROCCOM func) { return CompAddItem(func, 0, -1); };

int CompAddItem(PROCCOM func, int data) { return CompAddItem(func, data, -1); };

int CompAddItem(PROCCOM func, int data, int list)
{
// zajištìní velikosti bufferu
	if (ProgNum >= ProgMax)				// bude pøeteèení bufferu ?
	{
		ProgMax += 256;					// zvýšení èítaèe velikosti bufferu
		MemBuf(ProgBuf, ProgMax);		// zvýšení velikosti bufferu
	}

// adresa vkládané  položky programu
	EXECITEM* item = ProgBuf + ProgNum;	// adresa položky programu
	ProgNum++;							// zvýšení èítaèe položek

// inicializace prvku programu
	item->ExecCom = func;				// volaná funkce položky
	item->Data = data;					// data položky
	item->List = list;					// seznam
	item->Jump = 0;						// poèet prvkù k pøeskoèení

	return ProgNum-1;					// vrací index prvku
}


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu CASE

bool CompCase(int index)
{
// adresa prvku
	PETPROG*	item = BufEdi + index;
	PETPROG2*	item2 = BufEdi2 + index;

// kontrola, zda má položka nìjaké potomky
	if ((item->Param & PETPROG_CHILDS) == 0) return false;

// uložení záhlaví pøíkazu (zatím verze s èíslem)
	int zahlavi = CompAddItem(FCaseReal, 0);

// typ dat (-1 = nic)
	int bufID = -1;					// ID typu dat

// --------------- naètení výrazù k vìtvení ----------------------

// nalezení vìtve s testovaným výrazem
	int posun = 1;
	int inx = index;
	do {

// adresa dalšího potomka
		inx += posun;
		item += posun;
		item2 += posun;

// test, zda to je hledaný prvek
		if ((item->Func == IDF_CASE_SUM - IDF) &&
			(item->Param & PETPROG_CHILDS))
		{

// interpretace jako èíselný výraz
			if (CompNum(inx+1))
			{
				bufID = BufNumID;		// typ dat èíslo
//				ProgBuf[zahlavi].ExecCom = FCaseReal;
				break;
			}

// interpretace jako logický výraz
			if (CompLog(inx+1))
			{
				bufID = BufLogID;		// typ dat BOOL
				ProgBuf[zahlavi].ExecCom = FCaseBool;
				break;
			}

// interpretace jako ikona
			if (CompIco(inx+1))
			{
				bufID = BufIcoID;		// typ dat ikona
				ProgBuf[zahlavi].ExecCom = FCaseIcon;
				break;
			}

// interpretace jako plocha
			if (CompMap(inx+1))
			{
				bufID = BufMapID;		// typ dat plocha
				ProgBuf[zahlavi].ExecCom = FCaseMap;
				break;
			}

// interpretace jako sprajt
			if (CompSpr(inx+1))
			{
				bufID = BufSprID;		// typ dat sprajt
				ProgBuf[zahlavi].ExecCom = FCaseSprite;
				break;
			}

// interpretace jako hudba
			if (CompMus(inx+1))
			{
				bufID = BufMusID;		// typ dat hudba
				ProgBuf[zahlavi].ExecCom = FCaseMusic;
				break;
			}

// interpretace jako obrázek
			if (CompPic(inx+1))
			{
				bufID = BufPicID;		// typ dat obrázek
				ProgBuf[zahlavi].ExecCom = FCasePicture;
				break;
			}

// interpretace jako zvuk
			if (CompSnd(inx+1))
			{
				bufID = BufSndID;		// typ dat zvuk
				ProgBuf[zahlavi].ExecCom = FCaseSound;
				break;
			}

// interpretace jako text
			if (CompTxt(inx+1))
			{
				bufID = BufTxtID;		// typ dat text
				ProgBuf[zahlavi].ExecCom = FCaseText;
				break;
			}
		}

// posun pro pøíští prvek
		posun = item2->Items;

// dokud je další potomek
	} while (item->Param & PETPROG_NEXT);

// nenalezeno
	if (bufID < 0)
	{
		ProgNum = zahlavi;
		return false;
	}

// ------------------- naètení testù vìtví ------------------

// buffer indexù vìtví
	CBufInt bufon;

// nalezení vìtve case
	posun = 1;
	inx = index;
	item = BufEdi + inx;
	item2 = BufEdi2 + inx;
	int caseinx = 0;
	do {

// adresa dalšího potomka
		inx += posun;
		item += posun;
		item2 += posun;

// test, zda to je hledaný prvek
		if ((item->Func == IDF_CASE_ON - IDF) &&
			(item->Param & PETPROG_CHILDS))
		{

// èítaè obsluhy vìtví
			caseinx--;

// vyhledání testù vìtve
			int ii = inx+1;
			PETPROG* prg = item+1;
			PETPROG2* prg2 = item2+1;
			for (;;)
			{
				if ((prg->Func == IDF_CASE_EQU - IDF) &&
					(prg->Param & PETPROG_CHILDS))
				{
					int hl = ProgNum;
					ii++;

					switch (bufID)
					{
					case BufNumID:
						if (CompNum(ii)) goto CASEEQU;
						break;

					case BufLogID:
						if (CompLog(ii)) goto CASEEQU;
						break;

					case BufIcoID:
						if (CompIco(ii)) goto CASEEQU;
						break;

					case BufSprID:
						if (CompSpr(ii)) goto CASEEQU;
						break;

					case BufMapID:
						if (CompMap(ii)) goto CASEEQU;
						break;

					case BufTxtID:
						if (CompTxt(ii)) goto CASEEQU;
						break;

					case BufSndID:
						if (CompSnd(ii)) goto CASEEQU;
						break;

					case BufPicID:
						if (CompPic(ii)) goto CASEEQU;
						break;

					case BufMusID:
						if (CompMus(ii))
CASEEQU:				{
							ProgBuf[zahlavi].Data++;
							bufon.Add(hl);
							ProgBuf[hl].Jump = caseinx;
						}
						break;

					}
					ii--;
				}

				if ((prg->Param & PETPROG_NEXT) == 0) break;
				ii += prg2->Items;
				prg += prg2->Items;
				prg2 += prg2->Items;
			}
		}

// posun pro pøíští prvek
		posun = item2->Items;

// dokud je další potomek
	} while (item->Param & PETPROG_NEXT);


// -------------- naètení pøíkazu DEFAULT ----------------

// buffer indexù skokù
	CBufInt bufjump;
	bufjump.Add(ProgNum);
	CompComSubPar(index, IDF_CASE_DEF);


// --------------------- naètení obsluh vìtví ----------------

// nalezení vìtve case
	posun = 1;
	inx = index;
	item = BufEdi + inx;
	item2 = BufEdi2 + inx;
	caseinx = 0;
	do {

// adresa dalšího potomka
		inx += posun;
		item += posun;
		item2 += posun;

// test, zda to je hledaný prvek
		if ((item->Func == IDF_CASE_ON - IDF) &&
			(item->Param & PETPROG_CHILDS))
		{

// èítaè obsluhy vìtví
			caseinx--;
			int hl = ProgNum;

// naètení pøíkazù
			if (!CompComGrp(inx, FCommandExec, FCommandExec1))
			{
				CompAddItem(FCommandExec0);
			}

// úschova indexu skoku
			bufjump.Add(hl);

// pøemapování podmínek CASE
			for (int j = bufon.Num()-1; j >= 0; j--)
			{
				if (ProgBuf[bufon[j]].Jump == caseinx)
				{
					ProgBuf[bufon[j]].Jump = hl - bufon[j];
				}
			}	 
		}

// posun pro pøíští prvek
		posun = item2->Items;

// dokud je další potomek
	} while (item->Param & PETPROG_NEXT);


// ----------------- pøemapování skokù obsluh -----------

	for (int j = bufjump.Num()-1; j >= 0; j--)
	{
		ProgBuf[bufjump[j]].Jump = ProgNum - bufjump[j];
	}

	return true;
}


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu (vrací true = operace OK)

bool _fastcall CompCom(int index)
{
// adresa zdrojového prvku
	if ((DWORD)index >= (DWORD)BufEdiN) return false;
	PETPROG*	item = BufEdi + index;
//	PETPROG2*	item2 = BufEdi2 + index;
	int refinx = item->RefIndex;
	PETPROG2*	prg2;

// kontrola, zda je položka vypnuta
	if ((item->Param & (PETPROG_OFF | PETPROG_OFF_DEP)) != 0) return false;

// vìtvení podle funkce
	switch (item->Func + IDF)
	{
	case IDF_IF:
		CompAddItem(FIf);				// podmínka IF
		CompLogSubPar(index, IDF_IF_COND, true);
		CompComSubPar(index, IDF_IF_YES);
		CompComSubPar(index, IDF_IF_NO);
		return true;

	case IDF_WHILE:
		CompAddItem(FWhile);			// cyklus While
		CompLogSubPar(index, IDF_WHILE_COND, false);
		CompComSubPar(index, IDF_WHILE_BODY);
		return true;

	case IDF_WHILE_BREAK:
		CompAddItem(FBreak);			// pøerušení cyklu Break
		return true;
	
	case IDF_CASE:
		return CompCase(index);			// pøeklad pøíkazu CASE

	case IDF_REAL:						// èíselná promìnná
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			prg2 = BufObj2 + refinx;
			if (Real.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompNumPar(index, FSetNum, prg2->Data);
			}
			else
			{
				return CompNumPar(index, FSetNumList, prg2->Data, prg2->List);
			}
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			prg2 = BufLoc2 + refinx;
			if (Real.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompNumPar(index, FSetNumLoc, prg2->Local);
			}
			else
			{
				return CompNumPar(index, FSetNumLocList, prg2->Local, prg2->LocalList);
			}
		}
		return false;

	case IDF_ICON:							// promìnná ikony
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			prg2 = BufObj2 + refinx;
			if (Icon.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompIcoPar(index, FSetIcon, prg2->Data);
			}
			else
			{
				return CompIcoPar(index, FSetIconList, prg2->Data, prg2->List);
			}
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			prg2 = BufLoc2 + refinx;
			if (Icon.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompIcoPar(index, FSetIconLoc, prg2->Local);
			}
			else
			{
				return CompIcoPar(index, FSetIconLocList, prg2->Local, prg2->LocalList);
			}
		}
		return false;

	case IDF_SND:							// promìnná zvuku
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			prg2 = BufObj2 + refinx;
			if (Sound.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompSndPar(index, FSetSound, prg2->Data);
			}
			else
			{
				return CompSndPar(index, FSetSoundList, prg2->Data, prg2->List);
			}
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			prg2 = BufLoc2 + refinx;
			if (Sound.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompSndPar(index, FSetSoundLoc, prg2->Local);
			}
			else
			{
				return CompSndPar(index, FSetSoundLocList, prg2->Local, prg2->LocalList);
			}
		}
		return false;

	case IDF_MUS:							// promìnná hudby
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			prg2 = BufObj2 + refinx;
			if (Music.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompMusPar(index, FSetMusic, prg2->Data);
			}
			else
			{
				return CompMusPar(index, FSetMusicList, prg2->Data, prg2->List);
			}
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			prg2 = BufLoc2 + refinx;
			if (Music.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompMusPar(index, FSetMusicLoc, prg2->Local);
			}
			else
			{
				return CompMusPar(index, FSetMusicLocList, prg2->Local, prg2->LocalList);
			}
		}
		return false;

	case IDF_FNC:
		return CompFunc(index, IDF_FNC);	// funkce bez návratu

	case IDF_MAP:							// promìnná plochy
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			prg2 = BufObj2 + refinx;
			if (Map.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompMapPar(index, FSetMap, prg2->Data);
			}
			else
			{
				return CompMapPar(index, FSetMapList, prg2->Data, prg2->List);
			}
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			prg2 = BufLoc2 + refinx;
			if (Map.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompMapPar(index, FSetMapLoc, prg2->Local);
			}
			else
			{
				return CompMapPar(index, FSetMapLocList, prg2->Local, prg2->LocalList);
			}
		}
		return false;

	case IDF_SPRITE:							// promìnná sprajtu
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			prg2 = BufObj2 + refinx;
			if (Sprite.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompSprPar(index, FSetSprite, prg2->Data);
			}
			else
			{
				return CompSprPar(index, FSetSpriteList, prg2->Data, prg2->List);
			}
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			prg2 = BufLoc2 + refinx;
			if (Sprite.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompSprPar(index, FSetSpriteLoc, prg2->Local);
			}
			else
			{
				return CompSprPar(index, FSetSpriteLocList, prg2->Local, prg2->LocalList);
			}
		}
		return false;

	case IDF_TEXT:							// promìnná textu
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			prg2 = BufObj2 + refinx;
			if (Text.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompTxtPar(index, FSetText, prg2->Data);
			}
			else
			{
				return CompTxtPar(index, FSetTextList, prg2->Data, prg2->List);
			}
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			prg2 = BufLoc2 + refinx;
			if (Text.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompTxtPar(index, FSetTextLoc, prg2->Local);
			}
			else
			{
				return CompTxtPar(index, FSetTextLocList, prg2->Local, prg2->LocalList);
			}
		}
		return false;

	case IDF_KROK:
		CompAddItem(FKrok);				// Petøík - krok vpøed
		return true;

	case IDF_VLEVO:
		CompAddItem(FVlevo);			// Petøík - vlevo
		return true;

	case IDF_VPRAVO:
		CompAddItem(FVpravo);			// Petøík - vpravo
		return true;

	case IDF_VZAD:
		CompAddItem(FVzad);				// Petøík - vzad
		return true;

	case IDF_BOOL:						// logická promìnná
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			prg2 = BufObj2 + refinx;
			if (Bool.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompLogPar(index, FSetLog, prg2->Data);
			}
			else
			{
				return CompLogPar(index, FSetLogList, prg2->Data, prg2->List);
			}
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			prg2 = BufLoc2 + refinx;
			if (Bool.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompLogPar(index, FSetLogLoc, prg2->Local);
			}
			else
			{
				return CompLogPar(index, FSetLogLocList, prg2->Local, prg2->LocalList);
			}
		}
		return false;

	case IDF_PETRA_KROK:
		CompAddItem(FKrok2);			// Petra - krok vpøed
		return true;

	case IDF_PETRA_VLEVO:
		CompAddItem(FVlevo2);			// Petra - vlevo
		return true;

	case IDF_PETRA_VPRAVO:
		CompAddItem(FVpravo2);			// Petra - vpravo
		return true;

	case IDF_PETRA_VZAD:
		CompAddItem(FVzad2);			// Petra - vzad
		return true;

	case IDF_RYCHLE:
		return CompLogPar(index, FVisible);	// Petr - viditelnost

	case IDF_EXIT:
		CompAddItem(FReturn);			// návrat z funkce
		return true;

	case IDF_PETRA_RYCHLE:
		return CompLogPar(index, FVisible2); // Petra - viditelnost

	case IDF_PREDMET:
		return CompIcoPar(index, FSetPredmet); // nastavení pøedmìtu pøed Petrem

	case IDF_SMER:
		return CompNumPar(index, FSetSmer);	// Petr - nastavení smìru

	case IDF_POZICEX:
		return CompNumPar(index, FSetPozX);	// Petr - nastavení pozice X

	case IDF_POZICEY:
		return CompNumPar(index, FSetPozY);	// Petr - nastavení pozice Y

	case IDF_PETRA_PREDMET:
		return CompIcoPar(index, FSetPredmet2); // nastavení pøedmìtu pøed Petrou

	case IDF_PETRA_SMER:
		return CompNumPar(index, FSetSmer2); // Petra - nastavení smìru

	case IDF_PETRA_POZICEX:
		return CompNumPar(index, FSetPozX2); // Petra - nastavení pozice X

	case IDF_PETRA_POZICEY:
		return CompNumPar(index, FSetPozY2); // Petra - nastavení pozice Y

	case IDF_WAIT:
		if (!CompNumPar(index, FWait))		// èekání
		{
			CompAddItem(FWait1);
		}
		return true;

	case IDF_PIC:							// promìnná obrázku
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			prg2 = BufObj2 + refinx;
			if (Picture.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompPicPar(index, FSetPic, prg2->Data);
			}
			else
			{
				return CompPicPar(index, FSetPicList, prg2->Data, prg2->List);
			}
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			prg2 = BufLoc2 + refinx;
			if (Picture.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompPicPar(index, FSetPicLoc, prg2->Local);
			}
			else
			{
				return CompPicPar(index, FSetPicLocList, prg2->Local, prg2->LocalList);
			}
		}
		return false;

	case IDF_REPEAT:
		CompAddItem(FRepeat);			// cyklus Repeat
		CompNumSubPar(index, IDF_REPEAT_N, 0);
		CompComSubPar(index, IDF_WHILE_BODY);
		return true;

	case IDF_MOUSE_X:
		return CompNumPar(index, FSetMouseX);	// nastavení myši X

	case IDF_MOUSE_Y:
		return CompNumPar(index, FSetMouseY);	// nastavení myši Y

	case IDF_SPRITE_X:
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			prg2 = BufObj2 + refinx;
			if (Sprite.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompNumPar(index, FSpriteX, prg2->Data);
			}
			else
			{
				return CompNumPar(index, FSpriteXList, prg2->Data, prg2->List);
			}
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			prg2 = BufLoc2 + refinx;
			if (Sprite.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompNumPar(index, FSpriteXLoc, prg2->Local);
			}
			else
			{
				return CompNumPar(index, FSpriteXLocList, prg2->Local, prg2->LocalList);
			}
		}
		return false;

	case IDF_SPRITE_Y:
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			prg2 = BufObj2 + refinx;
			if (Sprite.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompNumPar(index, FSpriteY, prg2->Data);
			}
			else
			{
				return CompNumPar(index, FSpriteYList, prg2->Data, prg2->List);
			}
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			prg2 = BufLoc2 + refinx;
			if (Sprite.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompNumPar(index, FSpriteYLoc, prg2->Local);
			}
			else
			{
				return CompNumPar(index, FSpriteYLocList, prg2->Local, prg2->LocalList);
			}
		}
		return false;

	case IDF_SPRITE_SMER:
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			prg2 = BufObj2 + refinx;
			if (Sprite.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompNumPar(index, FSpriteSmer, prg2->Data);
			}
			else
			{
				return CompNumPar(index, FSpriteSmerList, prg2->Data, prg2->List);
			}
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			prg2 = BufLoc2 + refinx;
			if (Sprite.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompNumPar(index, FSpriteSmerLoc, prg2->Local);
			}
			else
			{
				return CompNumPar(index, FSpriteSmerLocList, prg2->Local, prg2->LocalList);
			}
		}
		return false;

	case IDF_SPRITE_FAZE:
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			prg2 = BufObj2 + refinx;
			if (Sprite.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompNumPar(index, FSpriteFaze, prg2->Data);
			}
			else
			{
				return CompNumPar(index, FSpriteFazeList, prg2->Data, prg2->List);
			}
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			prg2 = BufLoc2 + refinx;
			if (Sprite.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompNumPar(index, FSpriteFazeLoc, prg2->Local);
			}
			else
			{
				return CompNumPar(index, FSpriteFazeLocList, prg2->Local, prg2->LocalList);
			}
		}
		return false;

	case IDF_SPRITE_STATUS:
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			prg2 = BufObj2 + refinx;
			if (Sprite.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompNumPar(index, FSpriteStatus, prg2->Data);
			}
			else
			{
				return CompNumPar(index, FSpriteStatusList, prg2->Data, prg2->List);
			}
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			prg2 = BufLoc2 + refinx;
			if (Sprite.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompNumPar(index, FSpriteStatusLoc, prg2->Local);
			}
			else
			{
				return CompNumPar(index, FSpriteStatusLocList, prg2->Local, prg2->LocalList);
			}
		}
		return false;

	case IDF_SPRITE_VISIBLE:
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			prg2 = BufObj2 + refinx;
			if (Sprite.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompLogPar(index, FSpriteVis, prg2->Data);
			}
			else
			{
				return CompLogPar(index, FSpriteVisList, prg2->Data, prg2->List);
			}
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			prg2 = BufLoc2 + refinx;
			if (Sprite.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				return CompLogPar(index, FSpriteVisLoc, prg2->Local);
			}
			else
			{
				return CompLogPar(index, FSpriteVisLocList, prg2->Local, prg2->LocalList);
			}
		}
		return false;

	case IDF_LIST_INDEX:
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			prg2 = BufObj2 + refinx;
			if (List.IsNotValid(prg2->List)) return false;
			return CompNumPar(index, FSetList, 0, prg2->List);
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			prg2 = BufLoc2 + refinx;
			if (List.IsNotValid(prg2->List)) return false;
			return CompNumPar(index, FSetListLoc, 0, prg2->LocalList);
		}
		return false;

	case IDF_LIST_AUTO:
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			prg2 = BufObj2 + refinx;
			if (List.IsNotValid(prg2->List)) return false;
			return CompNumPar(index, FSetListAuto, 0, prg2->List);
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			prg2 = BufLoc2 + refinx;
			if (List.IsNotValid(prg2->List)) return false;
			return CompNumPar(index, FSetListAutoLoc, 0, prg2->LocalList);
		}
		return false;

	case IDF_PETER_SWCN1:
		return CompNumPar(index, FSetSwcN1);	// Petr - nastavení èísla 1

	case IDF_PETRA_SWCN1:
		return CompNumPar(index, FSetSwcN12);	// Petra - nastavení èísla 1

	case IDF_PETER_SWCN2:
		return CompNumPar(index, FSetSwcN2);	// Petr - nastavení èísla 2

	case IDF_PETRA_SWCN2:
		return CompNumPar(index, FSetSwcN22);	// Petra - nastavení èísla 2

	case IDF_PETER_SWCN3:
		return CompNumPar(index, FSetSwcN3);	// Petr - nastavení èísla 3

	case IDF_PETRA_SWCN3:
		return CompNumPar(index, FSetSwcN32);	// Petra - nastavení èísla 3

	case IDF_PETER_SWC1:
		return CompLogPar(index, FSetSwc1);		// Petr - nastavení pøepínaèe 1

	case IDF_PETRA_SWC1:
		return CompLogPar(index, FSetSwc12);	// Petra - nastavení pøepínaèe 1

	case IDF_PETER_SWC2:
		return CompLogPar(index, FSetSwc2);		// Petr - nastavení pøepínaèe 2

	case IDF_PETRA_SWC2:
		return CompLogPar(index, FSetSwc22);	// Petra - nastavení pøepínaèe 2

	case IDF_PETER_SWC3:
		return CompLogPar(index, FSetSwc3);		// Petr - nastavení pøepínaèe 3

	case IDF_PETRA_SWC3:
		return CompLogPar(index, FSetSwc32);	// Petra - nastavení pøepínaèe 3

	case IDF_PETER_SWC4:
		return CompLogPar(index, FSetSwc4);		// Petr - nastavení pøepínaèe 4

	case IDF_PETRA_SWC4:
		return CompLogPar(index, FSetSwc42);	// Petra - nastavení pøepínaèe 4

	case IDF_PETER_SWC5:
		return CompLogPar(index, FSetSwc5);		// Petr - nastavení pøepínaèe 5

	case IDF_PETRA_SWC5:
		return CompLogPar(index, FSetSwc52);	// Petra - nastavení pøepínaèe 5

	case IDF_STATUS_LINE:
		return CompTxtPar(index, FSetStatus);	// nastavení stavového øádku

	case IDF_CAPTION:
		return CompTxtPar(index, FSetCaption);	// nastavení nadpisu

	case IDF_SOUND_PLAY:
		CompAddItem(FSoundPlay);				// pøehrátí zvuku
		CompSndSubPar(index, IDF_SOUND_PLAY_SOUND);
		CompNumSubPar(index, IDF_SOUND_PLAY_VOL, 1);
		CompNumSubPar(index, IDF_SOUND_PLAY_BAL, 0);
		CompNumSubPar(index, IDF_SOUND_PLAY_SPEED, 1);
		CompNumSubPar(index, IDF_SOUND_PLAY_LOOP, 1);
		CompLogSubPar(index, IDF_SOUND_PLAY_STOP, true);
		return true;

	case IDF_SOUND_STOP:
		if (!CompSndPar(index, FSoundStop1))	// zastavení zvuku
		{
			CompAddItem(FSoundStop);			// zastavení zvuku
		}
		return true;

	case IDF_SOUND_DISTINGUISH:
		return CompNumPar(index, FSetSoundDistinguish);// nastavení rozlišovacího kódu zvuku

	case IDF_POINT:
		CompAddItem(FPoint);					// zobrazení bodu
		CompNumSubPar(index, IDF_GRAPH_COLOR, DEFCOLOR);
		CompNumSubPar(index, IDF_GRAPH_WIDTH, 1);
		CompNumSubPar(index, IDF_POINT_X, 0);
		CompNumSubPar(index, IDF_POINT_Y, 0);
		return true;

	case IDF_LINE:
		CompAddItem(FLine);						// zobrazení èáry
		CompNumSubPar(index, IDF_GRAPH_COLOR, DEFCOLOR);
		CompNumSubPar(index, IDF_GRAPH_WIDTH, 1);
		CompNumSubPar(index, IDF_LINE_X1, 0);
		CompNumSubPar(index, IDF_LINE_Y1, 0);
		CompNumSubPar(index, IDF_LINE_X2, 1);
		CompNumSubPar(index, IDF_LINE_Y2, 1);
		return true;

	case IDF_BOX:
		CompAddItem(FBox);						// zobrazení obdélníku
		CompNumSubPar(index, IDF_GRAPH_COLOR, DEFCOLOR);
		CompNumSubPar(index, IDF_GRAPH_WIDTH, 1);
		CompNumSubPar(index, IDF_BOX_X1, 0);
		CompNumSubPar(index, IDF_BOX_Y1, 0);
		CompNumSubPar(index, IDF_BOX_X2, 1);
		CompNumSubPar(index, IDF_BOX_Y2, 1);
		return true;

	case IDF_BOX_FILL:
		CompAddItem(FBoxFill);					// zobrazení vyplnìného obdélníku
		CompNumSubPar(index, IDF_GRAPH_COLOR, DEFCOLOR);
		CompNumSubPar(index, IDF_BOX_FILL_X1, 0);
		CompNumSubPar(index, IDF_BOX_FILL_Y1, 0);
		CompNumSubPar(index, IDF_BOX_FILL_X2, 2000000);
		CompNumSubPar(index, IDF_BOX_FILL_Y2, 2000000);
		return true;

	case IDF_FILL:
		CompAddItem(FFill);					// výplò
		CompNumSubPar(index, IDF_GRAPH_COLOR, DEFCOLOR);
		CompNumSubPar(index, IDF_FILL_X, 0);
		CompNumSubPar(index, IDF_FILL_Y, 0);
		return true;

	case IDF_CIRCLE:
		CompAddItem(FCircle);				// kružnice
		CompNumSubPar(index, IDF_GRAPH_COLOR, DEFCOLOR);
		CompNumSubPar(index, IDF_GRAPH_WIDTH, 1);
		CompNumSubPar(index, IDF_CIRCLE_X, 1);
		CompNumSubPar(index, IDF_CIRCLE_Y, 1);
		CompNumSubPar(index, IDF_CIRCLE_R, 1);
		return true;

	case IDF_CIRCLE_FILL:
		CompAddItem(FCircleFill);			// kruh
		CompNumSubPar(index, IDF_GRAPH_COLOR, DEFCOLOR);
		CompNumSubPar(index, IDF_CIRCLE_FILL_X, 1);
		CompNumSubPar(index, IDF_CIRCLE_FILL_Y, 1);
		CompNumSubPar(index, IDF_CIRCLE_FILL_R, 1);
		return true;

	case IDF_TRIANGLE:
		CompAddItem(FTriangle);			// trojúhelník
		CompNumSubPar(index, IDF_GRAPH_COLOR, DEFCOLOR);
		CompNumSubPar(index, IDF_TRIANGLE_X1, 0);
		CompNumSubPar(index, IDF_TRIANGLE_Y1, 0);
		CompNumSubPar(index, IDF_TRIANGLE_X2, 1);
		CompNumSubPar(index, IDF_TRIANGLE_Y2, 0);
		CompNumSubPar(index, IDF_TRIANGLE_X3, 0);
		CompNumSubPar(index, IDF_TRIANGLE_Y3, 1);
		return true;

	case IDF_SPHERE:
		CompAddItem(FSphere);			// koule
		CompNumSubPar(index, IDF_GRAPH_COLOR, DEFCOLOR);
		CompNumSubPar(index, IDF_SPHERE_X, 1);
		CompNumSubPar(index, IDF_SPHERE_Y, 1);
		CompNumSubPar(index, IDF_SPHERE_R, 1);
		return true;

	case IDF_KLAV_VKEY_WAIT:
		CompAddItem(FComVKeyWait);		// vtup klávesy s èekáním
		return true;

	case IDF_KLAV_CHAR_WAIT:
		CompAddItem(FComCharWait);		// vstup znaku s èekáním
		return true;

	case IDF_STATUS_NUMLOCK:
		return CompLogPar(index, FSetNumLock);	// nastavení pøepínaèen Num Lock

	case IDF_STATUS_CAPSLOCK:
		return CompLogPar(index, FSetCapsLock);	// nastavení pøepínaèen Caps Lock

	case IDF_STATUS_SCROLLLOCK:
		return CompLogPar(index, FSetScrollLock);	// nastavení pøepínaèen Scroll Lock

	case IDF_STATUS_INSERT:
		return CompLogPar(index, FSetInsertLock);	// nastavení pøepínaèen Insert

	case IDF_GRAPH_TEXT:
		CompAddItem(FGraphText);					// zobrazení textu
		CompTxtSubPar(index, IDF_GRAPH_TEXT_TEXT);
		CompNumSubPar(index, IDF_GRAPH_COLOR, DEFCOLOR);
		CompNumSubPar(index, IDF_GRAPH_TEXT_X, 0);
		CompNumSubPar(index, IDF_GRAPH_TEXT_Y, 0);
		CompNumSubPar(index, IDF_GRAPH_TEXT_ANGLE, 0);
		CompNumSubPar(index, IDF_GRAPH_TEXT_H, 0);
		CompNumSubPar(index, IDF_GRAPH_TEXT_W, 0);
		CompLogSubPar(index, IDF_GRAPH_TEXT_BOLD, false);
		CompLogSubPar(index, IDF_GRAPH_TEXT_ITALIC, false);
		CompLogSubPar(index, IDF_GRAPH_TEXT_UNDERLINE, false);
		CompLogSubPar(index, IDF_GRAPH_TEXT_STRIKEOUT, false);
		CompLogSubPar(index, IDF_GRAPH_TEXT_SERIF, false);
		CompLogSubPar(index, IDF_GRAPH_TEXT_COURIER, false);
		return true;

	case IDF_PREDMET_POZ:
		return CompIcoPar(index, FSetPredmetPoz); // nastavení pøedmìtu pod Petrem

	case IDF_PETRA_PREDMET_POZ:
		return CompIcoPar(index, FSetPredmet2Poz); // nastavení pøedmìtu pod Petrou

	case IDF_PIC_DISP:
		CompAddItem(FPicDisp);					// zobrazení obrázku
		CompPicSubPar(index, IDF_PIC_BACK);
		CompNumSubPar(index, IDF_PIC_X, 0);
		CompNumSubPar(index, IDF_PIC_Y, 0);
		CompNumSubPar(index, IDF_PIC_BLOCK_X, 0);
		CompNumSubPar(index, IDF_PIC_BLOCK_Y, 0);
		CompNumSubPar(index, IDF_PIC_BLOCK_W, 2000000);
		CompNumSubPar(index, IDF_PIC_BLOCK_H, 2000000);
		return true;

	case IDF_TRANSPARENT:
		CompAddItem(FTransparent);					// zprùhlednìní okna
		return true;

	case IDF_MOUSE_CURSOR:
		CompAddItem(FMouseCursor);				// kurzor myši
		CompIcoSubPar(index, IDF_MOUSE_CURSOR_PIC);
		CompNumSubPar(index, IDF_MOUSE_CURSOR_X, 0);
		CompNumSubPar(index, IDF_MOUSE_CURSOR_Y, 0);
		CompNumSubPar(index, IDF_MOUSE_CURSOR_W, 2000000);
		CompNumSubPar(index, IDF_MOUSE_CURSOR_H, 2000000);
		return true;

	case IDF_MUSIC_PLAY:
		return CompMusPar(index, FMusicPlay);	// pøehrátí hudby

	case IDF_MUSIC_LOOP:
		return CompMusPar(index, FMusicLoop);	// opakované pøehrávání hudby

	case IDF_MUSIC_STOP:
		CompAddItem(FMusicStop);				// zastavení hudby
		return true;

	case IDF_MUSIC_PAUSE:
		if (!CompLogPar(index, FMusicPauseSet))
		{
			CompAddItem(FMusicPause);				// pozastavení hudby
		}
		return true;

	case IDF_MUSIC_POS:
		return CompNumPar(index, FSetMusicPos);	// nastavení pozice hudby

	case IDF_CD_PLAY:
		if (!CompNumPar(index, FCDPlay))
		{
			CompAddItem(FCDPlay);				// pøehrávání CD
			CompAddItemNum((BYTE)1);			// implicitní stopa 1	
		}
		return true;

	case IDF_CD_STOP:
		CompAddItem(FCDStop);					// zastavení CD
		return true;

	case IDF_CD_PAUSE:
		if (!CompLogPar(index, FCDPauseSet))
		{
			CompAddItem(FCDPause);				// pozastavení CD
		}
		return true;

	case IDF_CD_EJECT:
		if (!CompLogPar(index, FCDEjectSet))
		{
			CompAddItem(FCDEject);				// zasunutí CD
		}
		return true;

	case IDF_CD_POS:
		return CompNumPar(index, FCDSetPos);	// nastavení pozice CD

	case IDF_CD_TRACK:
		return CompNumPar(index, FCDSetTrack);	// nastavení stopy CD

	case IDF_CD_TRACK_POS:
		return CompNumPar(index, FCDSetTrackPos);// nastavení pozice stopy CD

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_DLL_FREE:						// uvolnìní DLL knihovny
		return CompNumPar(index, FDLLFree);

	case IDF_DLL_EXEC:						// provedení funkce DLL
		return CompNumPar(index, FDLLExecCom);

	case IDF_MEMORY_FREE:					// zrušení pamìového bloku
		return CompNumPar(index, FMemoryFree);

	case IDF_MEMORY_READ:					// ukazatel ètení z pamìti
		return CompNumPar(index, FSetMemoryRead);

	case IDF_MEMORY_WRITE:					// ukazatel zápisu do pamìti
		return CompNumPar(index, FSetMemoryWrite);

	case IDF_MEMORY_COPY:					// kopie dat
		return CompNumPar(index, FMemoryCopy);

	case IDF_MEMORY_BYTES:					// bajt se znaménkem
		return CompNumPar(index, FSetMemoryByteS);

	case IDF_MEMORY_BYTEU:					// bajt bez znaménka
		return CompNumPar(index, FSetMemoryByteU);

	case IDF_MEMORY_WORDS:					// slovo se znaménkem
		return CompNumPar(index, FSetMemoryWordS);

	case IDF_MEMORY_WORDU:					// slovo bez znaménka
		return CompNumPar(index, FSetMemoryWordU);

	case IDF_MEMORY_3BYTES:					// 3-bajt se znaménkem
		return CompNumPar(index, FSetMemory3ByteS);

	case IDF_MEMORY_3BYTEU:					// 3-bajt bez znaménka
		return CompNumPar(index, FSetMemory3ByteU);

	case IDF_MEMORY_DWORDS:					// dvouslovo se znaménkem
		return CompNumPar(index, FSetMemoryDWordS);

	case IDF_MEMORY_DWORDU:					// dvouslovo bez znaménka
		return CompNumPar(index, FSetMemoryDWordU);

	case IDF_MEMORY_FLOAT:					// reálné èíslo 4 bajty
		return CompNumPar(index, FSetMemoryFloat);

	case IDF_MEMORY_DOUBLE:					// reálné èíslo 8 bajtù
		return CompNumPar(index, FSetMemoryDouble);

	case IDF_MEMORY_BOOL:					// logická hodnota
		return CompLogPar(index, FSetMemoryBool);

	case IDF_MEMORY_TEXT0:					// text zakonèený nulou
		return CompTxtPar(index, FSetMemoryText0);

	case IDF_MEMORY_TEXT0P:					// ukazatel na text zakonèený nulou
		return CompTxtPar(index, FSetMemoryText0P);

	case IDF_MEMORY_TEXTN:					// text s pevnou délkou
		return CompTxtPar(index, FSetMemoryTextN);

	case IDF_MEMORY_TEXTN_LEN:				// délka textu s pevnou délkou
		return CompNumPar(index, FSetMemoryTextNLen);

	case IDF_MEMORY_LOAD:					// naètení bloku dat
		return CompNumPar(index, FMemoryLoad);

	case IDF_MEMORY_SAVE:					// zápis bloku dat
		return CompNumPar(index, FMemorySave);

	case IDF_INI_FILE:						// jméno konfiguraèního souboru
		return CompTxtPar(index, FSetIniFile);

	case IDF_INI_SECTION:					// jméno konfiguraèní sekce
		return CompTxtPar(index, FSetIniSection);

	case IDF_INI_VALUE:						// jméno konfiguraèního parametru
		return CompTxtPar(index, FSetIniValue);

	case IDF_INI_STRING:					// zápis konfiguraèního textu
		if (!CompTxtPar(index, FSetIniString))
		{
			CompAddItem(FSetIniDel);
		}
		return true;

	case IDF_INI_NUM:						// zápis konfiguraèního èísla
		if (!CompNumPar(index, FSetIniNum))
		{
			CompAddItem(FSetIniDel);
		}
		return true;

	case IDF_REG_KEY:						// skupina klíèù registrù
		return CompNumPar(index, FSetRegKey);

	case IDF_REG_SUBKEY:					// klíè registrù
		return CompTxtPar(index, FSetRegSubkey);

	case IDF_REG_VALUE:						// jméno položky registru
		return CompTxtPar(index, FSetRegValue);

	case IDF_REG_STRING:					// zápis textu do registru
		if (!CompTxtPar(index, FSetRegString))
		{
			CompAddItem(FSetRegDel);
		}
		return true;

	case IDF_REG_NUM:						// zápis èísla do registru
		if (!CompNumPar(index, FSetRegNum))
		{
			CompAddItem(FSetRegDel);
		}
		return true;

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------

	case IDF_FILE_FILENEW:
		return CompTxtPar(index, FFileNew);		// nový soubor

	case IDF_FILE_DIRNEW:
		return CompTxtPar(index, FDirNew);		// nový adresáø

	case IDF_FILE_DELETE:
		return CompTxtPar(index, FFileDelete);	// zrušení souboru/adresáøe

	case IDF_FILE_ACT:
		return CompTxtPar(index, FFileAct);		// nastavení aktivního adresáøe

	case IDF_FILE_DISKLABEL:
		return CompTxtPar(index, FDiskLabel);	// nastavení jmenovky disku

	case IDF_FILE_READ:
		return CompTxtPar(index, FFileRead);	// jméno souboru pro ètení

	case IDF_FILE_WRITE:
		return CompTxtPar(index, FFileWrite);	// jméno souboru pro zápis

	case IDF_FILE_COPY:
		CompAddItem(FFileCopy);					// kopie souboru
		return true;

	case IDF_FILE_MOVE:
		CompAddItem(FFileMove);					// pøesun souboru
		return true;

	case IDF_FILE_RO:
		return CompLogPar(index, FFileRO);		// nastavení atributu RO

	case IDF_FILE_HID:
		return CompLogPar(index, FFileHID);		// nastavení atributu HID

	case IDF_FILE_SYS:
		return CompLogPar(index, FFileSYS);		// nastavení atributu SYS

	case IDF_FILE_ARC:
		return CompLogPar(index, FFileARC);		// nastavení atributu ARC

	case IDF_FILE_CREATE:
		return CompNumPar(index, FFileCreate);	// nastavení èasu vytvoøení souboru

	case IDF_FILE_OPEN:
		return CompNumPar(index, FFileOpen);	// nastavení èasu posledního ètení ze souboru

	case IDF_FILE_MODI:
		return CompNumPar(index, FFileModi);	// nastavení èasu posledního zápisu do souboru

	case IDF_FILE_BYTES:
		return CompNumPar(index, FFileByteS);	// zápis bajtu se znaménkem

	case IDF_FILE_BYTEU:
		return CompNumPar(index, FFileByteU);	// zápis bajtu bez znaménka

	case IDF_FILE_WORDS:
		return CompNumPar(index, FFileWordS);	// zápis slova se znaménkem

	case IDF_FILE_WORDU:
		return CompNumPar(index, FFileWordU);	// zápis slova bez znaménka

	case IDF_FILE_DWORDS:
		return CompNumPar(index, FFileDWordS);	// zápis dvojslova se znaménkem

	case IDF_FILE_DWORDU:
		return CompNumPar(index, FFileDWordU);	// zápis dvojslova bez znaménka

	case IDF_FILE_FLOAT:
		return CompNumPar(index, FFileFloat);	// zápis èísla FLOAT

	case IDF_FILE_DOUBLE:
		return CompNumPar(index, FFileDouble);	// zápis èísla DOUBLE

	case IDF_FILE_LOGIC:
		return CompLogPar(index, FFileLogic);	// zápis logické hodnoty

	case IDF_FILE_TEXT0:
		return CompTxtPar(index, FFileText0);	// zápis textu zakonèeného nulou

	case IDF_FILE_TEXTCR:
		return CompTxtPar(index, FFileTextCR);	// zápis textu zakonèeného CR

	case IDF_FILE_TEXTN:
		return CompTxtPar(index, FFileTextN);	// zápis textu s pevnou délkou

	case IDF_FILE_TEXT_UNICODE:					// text v kódu UNICODE
		return CompLogPar(index, FFileTextUnicode);

	case IDF_FILE_READOFF:
		return CompNumPar(index, FFileReadOff);	// offset pro ètení

	case IDF_FILE_WRITEOFF:
		return CompNumPar(index, FFileWriteOff);// offset pro zápis

	case IDF_DATETIME_CURRENT:
		return CompNumPar(index, FTimeCurrent);	// nastavení aktuálního data a èasu

	case IDF_FILE_SIZE:
		return CompNumPar(index, FFileSize);	// nastavení velikosti souboru

	case IDF_FILE_ERROR:
		return CompLogPar(index, FFileError);	// pøíznak chyby souborù

	case IDF_FILE_TEXTN_LEN:
		return CompNumPar(index, FFileTextNLen);// délka textu s pevnou délkou

	case IDF_FILE_3BYTES:
		return CompNumPar(index, FFile3ByteS);	// zápis 3 bajtù se znaménkem

	case IDF_FILE_3BYTEU:
		return CompNumPar(index, FFile3ByteU);	// zápis 3 bajtù bez znaménka

	case IDF_FILE_ICON:
		return CompIcoPar(index, FFileIcon);	// zápis ikony

	case IDF_FILE_PICTURE:
		return CompPicPar(index, FFilePicture);	// zápis obrázku

	case IDF_FILE_SOUND:
		return CompSndPar(index, FFileSound);	// zápis zvuku

	case IDF_FILE_MUSIC:
		return CompMusPar(index, FFileMusic);	// zápis hudby

	case IDF_FILE_SPRITE:
		return CompSprPar(index, FFileSprite);	// zápis sprajtu

	case IDF_DIALOG_BOX:
		CompAddItem(FDialogBox);				// dialog box
		CompTxtSubPar(index, IDF_DIALOG_BOX_CAPTION);
		CompTxtSubPar(index, IDF_DIALOG_BOX_TEXT);
		CompNumSubPar(index, IDF_DIALOG_BOX_ICON, 0);
		CompNumSubPar(index, IDF_DIALOG_BOX_BUTTON, 0);
		CompNumSubPar(index, IDF_DIALOG_BOX_DEFAULT, 1);
		return true;

	case IDF_SPRITE_SHIFT_X:
		return CompNumPar(index, FSpriteShiftX); // posun sprajtù ve smìru X

	case IDF_SPRITE_SHIFT_Y:
		return CompNumPar(index, FSpriteShiftY); // posun sprajtù ve smìru Y

	case IDF_SPRITE_WIN_X1:
		return CompNumPar(index, FSpriteWinX1);	// levý okraj okna sprajtù

	case IDF_SPRITE_WIN_Y1:
		return CompNumPar(index, FSpriteWinY1);	// dolní okraj okna sprajtù

	case IDF_SPRITE_WIN_X2:
		return CompNumPar(index, FSpriteWinX2);	// pravý okraj okna sprajtù

	case IDF_SPRITE_WIN_Y2:
		return CompNumPar(index, FSpriteWinY2);	// horní okraj okna sprajtù

	case IDF_CONSOLE_ON:
//		if (!CompLogPar(index, FConsoleOnSet))	// režim konzoly
//		{
			CompAddItem(FConsoleOn);
//		}
		return true;

	case IDF_CONSOLE_OUT:
		return CompTxtPar(index, FConsoleOut);	// výstup textu na konzolu

	case IDF_CONSOLE_ERR:
		return CompTxtPar(index, FConsoleErr);	// výstup chybového textu na konzolu

	case IDF_RETURN_CODE:
		return CompNumPar(index, FSetReturnCode); // návratový kód
		 
	case IDF_EXEC:
		return CompTxtPar(index, FExec);		// spuštìní programu

	case IDF_EXEC_WAIT:
		return CompTxtPar(index, FExecWait);	// spuštìní programu s èekáním

//	case IDF_DIALOG_ON:
//		if (!CompLogPar(index, FDialogOnSet))	// režim dialogu
//		{
//			CompAddItem(FDialogOn);
//		}
//		return true;

	case IDF_WINDOW_ID:
		return CompNumPar(index, FWindowID);	// aktivní prvek

	case IDF_WINDOW_DEL:
		return CompNumPar(index, FWindowDel);	// zrušení okna

	case IDF_WINDOW_TEXT:
		return CompTxtPar(index, FWindowText);	// text okna

	case IDF_WINDOW_X:
		return CompNumPar(index, FWindowX);		// poèátek okna X

	case IDF_WINDOW_Y:
		return CompNumPar(index, FWindowY);		// poèátek okna Y

	case IDF_WINDOW_W:
		return CompNumPar(index, FWindowW);		// šíøka okna

	case IDF_WINDOW_H:
		return CompNumPar(index, FWindowH);		// výška okna

	case IDF_WINDOW_W_INT:
		return CompNumPar(index, FWindowWInt);	// vnitøní šíøka okna

	case IDF_WINDOW_H_INT:
		return CompNumPar(index, FWindowHInt);	// vnitøní výška okna

	case IDF_WINDOW_VISIBLE:
		if (!CompLogPar(index, FWindowVisibleSet)) // viditelnost okna
		{
			CompAddItem(FWindowVisible);
		}
		return true;

	case IDF_WINDOW_DISABLE:
		if (!CompLogPar(index, FWindowDisableSet)) // zákaz okna
		{
			CompAddItem(FWindowDisable);
		}
		return true;

	case IDF_CHECKBOX_ON:
		if (!CompLogPar(index, FCheckBoxOnSet))	// pøepínaè
		{
			CompAddItem(FCheckBoxOn);
		}
		return true;

	case IDF_FOCUS:
		return CompNumPar(index, FFocus);	// okno s fokusem

	case IDF_FONTBOLD:
		if (!CompLogPar(index, FFontBoldSet)) // zvýraznìný text
		{
			CompAddItem(FFontBold);
		}
		return true;

	case IDF_FONTFIXED:
		if (!CompLogPar(index, FFontFixedSet)) // text s pevnou rozteèí
		{
			CompAddItem(FFontFixed);
		}
		return true;

	case IDF_FONTHEIGHT:
		return CompNumPar(index, FFontHeightSet); // nastavení výšky znakù

	case IDF_FONTITALIC:
		if (!CompLogPar(index, FFontItalicSet)) // šikmý text
		{
			CompAddItem(FFontItalic);
		}
		return true;

	case IDF_FONTUNDER:
		if (!CompLogPar(index, FFontUnderSet)) // potržený text
		{
			CompAddItem(FFontUnder);
		}
		return true;

	case IDF_FONTSTRIKE:
		if (!CompLogPar(index, FFontStrikeSet)) // pøeškrnutý text
		{
			CompAddItem(FFontStrike);
		}
		return true;

	case IDF_FONTSERIF:
		if (!CompLogPar(index, FFontSerifSet)) // text s patièkami
		{
			CompAddItem(FFontSerif);
		}
		return true;

	case IDF_FONTWIDTH:
		return CompNumPar(index, FFontWidthSet); // nastavení šíøky znakù

	case IDF_FONTANGLE:
		return CompNumPar(index, FFontAngleSet); // nastavení šíøky znakù

	case IDF_FONTCOL:
		return CompNumPar(index, FFontColSet); // nastavení barvy textu

	case IDF_FONTBACK:
		return CompNumPar(index, FFontBackSet); // nastavení barvy pozadí

	case IDF_LISTBOX_ADD:
		return CompTxtPar(index, FListBoxAdd);	// pøidání položky do seznamu

	case IDF_LISTBOX_CLEAR:
		CompAddItem(FListBoxClear);			// vyprázdnìní seznamu
		return true;

	case IDF_BUTTON3_ON:
		if (!CompLogPar(index, FButton3OnSet))	// pøepínaè
		{
			CompAddItem(FButton3On);
		}
		return true;

	case IDF_WINDOW_ICON:
		return CompIcoPar(index, FWindowIcon);

	case IDF_WINDOW_PIC:
		return CompPicPar(index, FWindowPic);

	case IDF_FULLSCREEN:
		if (!CompLogPar(index, FFullScreenSet))	// pøepínaè
		{
			CompAddItem(FFullScreen);
		}
		return true;

	case IDF_KLAV_FLUSH:
		CompAddItem(FKlavFlush);			// vyprázdnìní bufferu kláves
		return true;

	case IDF_SOUND_POS:
		return CompNumPar(index, FSetSoundPos);	// nastavení pozice zvuku

	case IDF_SOUND_PAUSE:
		if (!CompLogPar(index, FSoundPauseSet))
		{
			CompAddItem(FSoundPause);			// pozastavení zvuku
		}
		return true;

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_CONNECT_ACT:
		return CompTxtPar(index, FSetConnectAct);	// nastavení aktivního spojení

	case IDF_GAME_ACT:
		return CompTxtPar(index, FSetGameAct);		// nastavení aktivní hry

	case IDF_PLAYER_ACT:
		return CompTxtPar(index, FSetPlayerAct);	// nastavení aktivního hráèe

	case IDF_GAME_N1:
		return CompNumPar(index, FSetGameN1);		// nastavení herního èísla 1

	case IDF_GAME_N2:
		return CompNumPar(index, FSetGameN2);		// nastavení herního èísla 2

	case IDF_GAME_N3:
		return CompNumPar(index, FSetGameN3);		// nastavení herního èísla 3

	case IDF_GAME_N4:
		return CompNumPar(index, FSetGameN4);		// nastavení herního èísla 4

	case IDF_GAME_N5:
		return CompNumPar(index, FSetGameN5);		// nastavení herního èísla 5

	case IDF_GAME_N6:
		return CompNumPar(index, FSetGameN6);		// nastavení herního èísla 6

	case IDF_GAME_N7:
		return CompNumPar(index, FSetGameN7);		// nastavení herního èísla 7

	case IDF_GAME_N8:
		return CompNumPar(index, FSetGameN8);		// nastavení herního èísla 8

	case IDF_GAME_L1:
		return CompLogPar(index, FSetGameL1);		// nastavení herního pøepínaèe 1

	case IDF_GAME_L2:
		return CompLogPar(index, FSetGameL2);		// nastavení herního pøepínaèe 2

	case IDF_GAME_L3:
		return CompLogPar(index, FSetGameL3);		// nastavení herního pøepínaèe 3

	case IDF_GAME_L4:
		return CompLogPar(index, FSetGameL4);		// nastavení herního pøepínaèe 4

	case IDF_GAME_L5:
		return CompLogPar(index, FSetGameL5);		// nastavení herního pøepínaèe 5

	case IDF_GAME_L6:
		return CompLogPar(index, FSetGameL6);		// nastavení herního pøepínaèe 6

	case IDF_GAME_L7:
		return CompLogPar(index, FSetGameL7);		// nastavení herního pøepínaèe 7

	case IDF_GAME_L8:
		return CompLogPar(index, FSetGameL8);		// nastavení herního pøepínaèe 8

	case IDF_PLAYER_MAX:
		return CompNumPar(index, FSetPlayerMax);	// max. hráèù

	case IDF_GAME_PACKET:
		if (!CompNumPar(index, FSetGamePacket))		// vyslání paketu
		{
			CompAddItem(FSetGamePacket0);
		}
		return true;

	case IDF_GAME_DATAN:
		return CompNumPar(index, FSetGameDataN);	// vyslání èísla paketem

	case IDF_GAME_DATAT:
		return CompTxtPar(index, FSetGameDataT);	// vyslání textu paketem

	case IDF_GAME_DATAL:
		return CompLogPar(index, FSetGameDataL);	// vyslání pøepínaèe paketem

	case IDF_GAME_DATAB:
		return CompNumPar(index, FSetGameDataB);	// vyslání bajtu paketem

	case IDF_UDP_SEND_IP:
		return CompTxtPar(index, FSetUDPSendIP);	// nastavení vysílací IP adresy UDP

	case IDF_UDP_SEND_PORT:
		return CompNumPar(index, FSetUDPSendPort);	// nastavení vysílacího portu UDP

	case IDF_UDP_REC_SIZE:
		return CompNumPar(index, FSetUDPRecSize);	// nastavení velikosti pøijímacího bufferu UDP

	case IDF_UDP_REC_IP:
		return CompTxtPar(index, FSetUDPRecIP);		// nastavení pøijímací IP adresy UDP

	case IDF_UDP_REC_PORT:
		return CompNumPar(index, FSetUDPRecPort);	// nastavení pøijímacího portu UDP

	case IDF_UDP_MEM:
		return CompNumPar(index, FSetUDPMem);		// odeslání binárního paketu UDP

	case IDF_UDP_TEXT:
		return CompTxtPar(index, FSetUDPText);		// odeslání textového paketu UDP

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------


	case IDF_BUTTON_ESC:
		return CompNumPar(index, FSetButtonEsc);	// tlaèítko pro Esc

	case IDF_CODEPAGE:
		return CompNumPar(index, FSetCodePage);		// nastavení znakové sady

	case IDF_USERFONT:
		return CompTxtPar(index, FSetUserFont);		// nastavení uživatelského fontu

	case IDF_LANGUAGE:
		return CompNumPar(index, FSetLanguage);		// nastavení jazyku

	case IDF_WINDOW_FRAME:
		return CompLogPar(index, FSetWindowFrame);	// nastavení rámeèku okna

	case IDF_WINDOW_CAPTION:
		return CompLogPar(index, FSetWindowCaption); // nastavení nadpisu okna

	case IDF_WINDOW_TOP:
		return CompLogPar(index, FSetWindowTop);	// nastavení okna vždy nahoøe

	case IDF_WINDOW_RESIZABLE:
		return CompLogPar(index, FSetWindowSize);	// nastavení okna s mìnitelnou velikostí

	case IDF_WINDOW_MAY_CLOSE:
		return CompLogPar(index, FSetWindowMayClose);	// nastavení povolení uzavøení okna

	case IDF_WINDOW_MAXIM:
		return CompLogPar(index, FSetWindowMaxim);		// nastavení maximalizace okna

	case IDF_WINDOW_MINIM:
		return CompLogPar(index, FSetWindowMinim);		// nastavení minimalizace okna

	case IDF_WINDOW_POS:
		return CompNumPar(index, FSetWindowPos);	// nastavení ukazatele v prvku

	case IDF_WINDOW_POS2:
		return CompNumPar(index, FSetWindowPos2);	// nastavení ukazatele 2 v prvku

	case IDF_ALIAS:
		return CompTxtPar(index, FSetAlias);		// nastavení ALIAS souboru

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_COM_ID:
		return CompNumPar(index, FSetComID);		// aktivní port

	case IDF_COM_CLOSE:
		return CompNumPar(index, FComClose);		// uzavøení portu

	case IDF_COM_BAUD:
		return CompNumPar(index, FSetComBaud);		// nastavení pøenosové rychlosti portu

	case IDF_COM_STOP:
		return CompLogPar(index, FSetComStop);		// nastavení stopbitù portu

	case IDF_COM_BITS:
		return CompNumPar(index, FSetComBits);		// nastavení poètu bitù portu

	case IDF_COM_PARIT:
		return CompNumPar(index, FSetComParit);		// nastavení parity

	case IDF_COM_TEXT:
		return CompTxtPar(index, FSetComText);		// vyslání textu na port

	case IDF_COM_NUM:
		return CompNumPar(index, FSetComNum);		// vyslání èísla na port

	case IDF_COM_SEND:
		return CompLogPar(index, FSetComSend);		// povolení vysílání

	case IDF_COM_RECEIVE:
		return CompLogPar(index, FSetComReceive);	// povolení pøíjmu

	case IDF_DDE_APP:
		return CompTxtPar(index, FSetDDEApp);		// nastavení jména plikace DDE

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------

	case IDF_GRAPH_AREA_WIDTH:
		return CompNumPar(index, FSetGraphAreaWidth);	// šíøka k zobrazení centrovaného textu

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_MIX_DEVICE:
		return CompTxtPar(index, FSetMixDevice);	// nastavení mixážního zaøízení

	case IDF_MIX_LINE_DST:
		return CompTxtPar(index, FSetMixLineDst);	// nastavení cílového signálu

	case IDF_MIX_LINE_SRC:
		return CompTxtPar(index, FSetMixLineSrc);	// nastavení zdrojového signálu

	case IDF_MIX_LINE_TYPE:							// nastavení typu signálu
		return CompNumPar(index, FSetLineType);

	case IDF_MIX_CONTROL:
		return CompTxtPar(index, FSetMixControl);	// nastavení ovládacího prvku

	case IDF_MIX_CONTROL_TYPE:						// nastavení typu ovládacího prvku
		return CompNumPar(index, FSetCtrlType);

	case IDF_MIX_CONTROL_VAL:						// nastavení hodnoty ovládacího prvku
		return CompNumPar(index, FSetCtrlVal);

	case IDF_MIX_CHANNEL:							// nastavení kanálu ovládacího prvku
		return CompNumPar(index, FSetMixChannel);

	case IDF_MIX_CONTROL_LIST:						// výbìr hodnoty ovládacího prvku
		return CompTxtPar(index, FSetMixCtrlList);

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------


	case IDF_DIALOG_GRAPHIC:
		return CompLogPar(index, FSetDialogGraph);	// nadstavení grafického pozadí

	case IDF_DIRECT3D_MODE_LIST:
		return CompTxtPar(index, FSet3DMode);		// nastavení videomódu

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_DIRECT3D_VIEW:
		CompAddItem(FDirect3DView);					// nastavení okna Direct3D
		CompNumSubPar(index, IDF_DIRECT3D_VIEW_X, -100);
		CompNumSubPar(index, IDF_DIRECT3D_VIEW_Y, -100);
		CompNumSubPar(index, IDF_DIRECT3D_VIEW_W, 2000000);
		CompNumSubPar(index, IDF_DIRECT3D_VIEW_H, 2000000);
		return true;

	case IDF_DIRECT3D_WIREFRAME:
		return CompLogPar(index, FSetD3DWireframe);	// zapnutí výplnì 3D

	case IDF_DIRECT3D_LIGHTON:
		return CompLogPar(index, FSetD3DLighton);	// zapnuto osvìtlení

	case IDF_DIRECT3D_SHADES:
		return CompLogPar(index, FSetD3DShades);	// nastavení módu stínování 3D

	case IDF_DIRECT3D_INVERT:
		CompAddItem(FD3DInvert);					// inverze objektu
		return true;

	case IDF_DIRECT3D_OCULLING:
		return CompNumPar(index, FSetD3DOCulling);	// nastavení odstraòování ploch objektu Direct 3D

	case IDF_DIRECT3D_MINFILTER:
		return CompLogPar(index, FSetD3DMinFilter);		// zapnutí filtrace pøi zmenšení

	case IDF_DIRECT3D_MAGFILTER:
		return CompLogPar(index, FSetD3DMagFilter);		// zapnutí filtrace pøi zvìtšení

	case IDF_DIRECT3D_MIPFILTER:
		return CompLogPar(index, FSetD3DMipFilter);		// zapnutí filtrace pøi vzdálení

	case IDF_DIRECT3D_OWIREFRAME:
		return CompLogPar(index, FSetD3DOWireframe);	// zapnutí výplnì objektu 3D

	case IDF_DIRECT3D_OLIGHTON:
		return CompLogPar(index, FSetD3DOLighton);	// zapnuto osvìtlení objektu

	case IDF_DIRECT3D_OSHADES:
		return CompLogPar(index, FSetD3DOShades);	// nastavení módu stínování objektu 3D

	case IDF_DIRECT3D_OMINFILTER:
		return CompLogPar(index, FSetD3DOMinFilter);		// zapnutí filtrace pøi zmenšení textury objektu

	case IDF_DIRECT3D_OMAGFILTER:
		return CompLogPar(index, FSetD3DOMagFilter);		// zapnutí filtrace pøi zvìtšení textury objektu

	case IDF_DIRECT3D_OMIPFILTER:
		return CompLogPar(index, FSetD3DOMipFilter);		// zapnutí filtrace pøi vzdálení textury objektu

	case IDF_DIRECT3D_VISIBLE:						// nastavení viditelnosti objektu Direct 3D
		return CompLogPar(index, FSetD3DVisible);

	case IDF_DIRECT3D_LINK:							// pøipojení jiného objektu/objektù
		return CompNumGrp(index, FD3DLink, FD3DLink);

//	case IDF_DIRECT3D_SAVE:							// uložení objektu do souboru
//		return CompTxtPar(index, FD3DSave);

	case IDF_DIRECT3D_OBJECT_ID:
		return CompNumPar(index, FSetD3DObjectID);	// nastavení aktivního objektu Direct3D

	case IDF_DIRECT3D_OBJECT_DELETE:
		return CompNumGrp(index, FD3DObjectDelete, FD3DObjectDelete); // zrušení objektu/objektù Direct3D

	case IDF_DIRECT3D_SCALEX:
		return CompNumPar(index, FSetD3DScaleX);	// nastavení mìøítka ve smìru X

	case IDF_DIRECT3D_SCALEY:
		return CompNumPar(index, FSetD3DScaleY);	// nastavení mìøítka ve smìru Y

	case IDF_DIRECT3D_SCALEZ:
		return CompNumPar(index, FSetD3DScaleZ);	// nastavení mìøítka ve smìru Z

	case IDF_DIRECT3D_ROTATEX:
		return CompNumPar(index, FSetD3DRotateX);	// nastavení rotace podle osy X

	case IDF_DIRECT3D_ROTATEY:
		return CompNumPar(index, FSetD3DRotateY);	// nastavení rotace podle osy Y

	case IDF_DIRECT3D_ROTATEZ:
		return CompNumPar(index, FSetD3DRotateZ);	// nastavení rotace podle osy Z

	case IDF_DIRECT3D_TRANSX:
		return CompNumPar(index, FSetD3DTransX);	// nastavení posunu ve smìru X

	case IDF_DIRECT3D_TRANSY:
		return CompNumPar(index, FSetD3DTransY);	// nastavení posunu ve smìru Y

	case IDF_DIRECT3D_TRANSZ:
		return CompNumPar(index, FSetD3DTransZ);	// nastavení posunu ve smìru Z

	case IDF_DIRECT3D_COLOR:
		return CompNumPar(index, FSetD3DColor);		// nastavení barvy objektu

	case IDF_DIRECT3D_EMISSIVE:
		return CompNumPar(index, FSetD3DEmissive);	// nastavení barvy svítivosti

	case IDF_DIRECT3D_POWER:
		return CompNumPar(index, FSetD3DPower);		// nastavení matnosti

	case IDF_DIRECT3D_SPECULAR:
		return CompNumPar(index, FSetD3DSpecular);	// nastavení barvy odlesku

	case IDF_DIRECT3D_TEXTURE:						// nastavení textury objektu Direct3D
		return CompPicPar(index, FSetD3DTexture);

	case IDF_DIRECT3D_TEXTURE_FILE:					// naètení textury objektu Direct3D ze souboru
		CompAddItem(FD3DTextureFile);
		return true;

	case IDF_DIRECT3D_SMOOTH:						// nastavení vyhlazení textur
		return CompNumPar(index, FSetD3DSmooth);

	case IDF_DIRECT3D_TEXTMAP:						// mapování textury
		CompAddItem(FD3DTextMap);
		CompNumSubPar(index, IDF_DIRECT3D_TEXTMAP_METHOD, 0);
		CompNumSubPar(index, IDF_DIRECT3D_TEXTMAP_TYPE, 0);
		CompNumSubPar(index, IDF_DIRECT3D_TEXTMAP_OX, 0);
		CompNumSubPar(index, IDF_DIRECT3D_TEXTMAP_OY, 0);
		CompNumSubPar(index, IDF_DIRECT3D_TEXTMAP_OZ, 0);
		CompNumSubPar(index, IDF_DIRECT3D_TEXTMAP_RZ, 0);
		CompNumSubPar(index, IDF_DIRECT3D_TEXTMAP_RX, 0);
		CompNumSubPar(index, IDF_DIRECT3D_TEXTMAP_RY, 0);
		CompNumSubPar(index, IDF_DIRECT3D_TEXTMAP_OU, -0.5);
		CompNumSubPar(index, IDF_DIRECT3D_TEXTMAP_OV, -0.5);
		CompNumSubPar(index, IDF_DIRECT3D_TEXTMAP_SU, 1);
		CompNumSubPar(index, IDF_DIRECT3D_TEXTMAP_SV, 1);
		return true;

	case IDF_DIRECT3D_TRANSPARENT:
		return CompNumPar(index, FSetD3DTransparent);	// nastavení prùhlednosti

	case IDF_DIRECT3D_ALPHAREF:
		return CompNumPar(index, FSetD3DAlphaRef);		// nastavení referenèní úrovnì alfa kanálu

	case IDF_DIRECT3D_LIGHT_COLOR:
		return CompNumPar(index, FSetD3DLightColor);	// nastavení barvy svìtla Direct3D

	case IDF_DIRECT3D_LIGHT_TYPE:
		return CompNumPar(index, FSetD3DLightType);		// nastavení typu svìtla Direct3D

	case IDF_DIRECT3D_LIGHT_UMBRA:
		return CompNumPar(index, FSetD3DLightUmbra);	// nastavení úhlu stínu svìtla Direct3D

	case IDF_DIRECT3D_LIGHT_PENUMBRA:
		return CompNumPar(index, FSetD3DLightPenumbra);	// nastavení úhlu polostínu svìtla Direct3D

	case IDF_DIRECT3D_LIGHT_RANGE:
		return CompNumPar(index, FSetD3DLightRange);	// nastavení dosahu svìtla Direct3D

	case IDF_DIRECT3D_LIGHT_CATTEN:
		return CompNumPar(index, FSetD3DLightCatten);	// nastavení konstantního útlumu svìtla Direct3D

	case IDF_DIRECT3D_LIGHT_LATTEN:
		return CompNumPar(index, FSetD3DLightLatten);	// nastavení lineárního útlumu svìtla Direct3D

	case IDF_DIRECT3D_LIGHT_QATTEN:
		return CompNumPar(index, FSetD3DLightQatten);	// nastavení kvadratického útlumu svìtla Direct3D

	case IDF_DIRECT3D_PROJECTION:						// typ projekce
		return CompNumPar(index, FSetD3DProjection);

	case IDF_DIRECT3D_SCENE_COLOR:
		return CompNumPar(index, FSetD3DSceneColor);	// nastavení barvy pozadí scény

	case IDF_DIRECT3D_SCENE_TEXTURE:
		return CompPicPar(index, FSetD3DSceneTexture);	// nastavení textury pozadí scény

	case IDF_DIRECT3D_FOG_COLOR:
		return CompNumPar(index, FSetD3DFogColor);		// nastavení barvy mlhy

	case IDF_DIRECT3D_FOG_MODE:
		return CompNumPar(index, FSetD3DFogMode);		// nastavení módu mlhy

	case IDF_DIRECT3D_FOG_START:
		return CompNumPar(index, FSetD3DFogStart);		// nastavení zaèátku mlhy

	case IDF_DIRECT3D_FOG_END:
		return CompNumPar(index, FSetD3DFogEnd);		// nastavení konce mlhy

	case IDF_DIRECT3D_FOG_DENSITY:
		return CompNumPar(index, FSetD3DFogDensity);	// nastavení hustoty mlhy

	case IDF_DIRECT3D_VIEW_FRONT:
		return CompNumPar(index, FSetD3DViewFront);		// nastavení promítací roviny

	case IDF_DIRECT3D_VIEW_BACK:
		return CompNumPar(index, FSetD3DViewBack);		// nastavení vzdálenosti dohledu

	case IDF_DIRECT3D_UPPER:
		return CompLogPar(index, FSetD3DUpper);			// vytváøet horní podstavu

	case IDF_DIRECT3D_LOWER:
		return CompLogPar(index, FSetD3DLower);			// vytváøet dolní podstavu

	case IDF_DIRECT3D_SPLITS:
		return CompNumPar(index, FSetD3DSplits);		// nastavení složitosti objektu

	case IDF_DIRECT3D_ORDER:
		return CompNumPar(index, FSetD3DOrder);			// nastavení poøadí rotací

	case IDF_DIRECT3D_DRIVER:
		return CompNumPar(index, FSetD3DDriver);		// nastavení ovladaèe

	case IDF_DIRECT3D_INTERFACE:
		return CompNumPar(index, FSetD3DInterface);		// nastavení rozhraní

	case IDF_DIRECT3D_AMBIENT:							// nastavení ambient svìtla
		return CompNumPar(index, FSetD3DAmbient);

	case IDF_DIRECT3D_NORM:
		CompAddItem(FD3DNorm);							// normalizace objektu
		return true;

	case IDF_DIRECT3D_AMBIREF:
		return CompNumPar(index, FSetD3DAmbiRef);		// nastavení odrazivosti rozptýleného svìtla

	case IDF_DIRECT3D_STAGE:							// stupeò textur
		return CompNumPar(index, FSetD3DStage);

	case IDF_DIRECT3D_COLOROP:							// operace barev
		return CompNumPar(index, FSetD3DColorOp);

	case IDF_DIRECT3D_COLORARG1:						// argument 1 barev
		return CompNumPar(index, FSetD3DColorArg1);

	case IDF_DIRECT3D_COLORARG2:						// argument 2 barev
		return CompNumPar(index, FSetD3DColorArg2);

	case IDF_DIRECT3D_ALPHAOP:							// operace alfa
		return CompNumPar(index, FSetD3DAlphaOp);

	case IDF_DIRECT3D_ALPHAARG1:						// argument 1 alfa
		return CompNumPar(index, FSetD3DAlphaArg1);

	case IDF_DIRECT3D_ALPHAARG2:						// argument 2 alfa
		return CompNumPar(index, FSetD3DAlphaArg2);

	case IDF_DIRECT3D_ADDRESSU:							// horizontální adresování
		return CompNumPar(index, FSetD3DAddressU);

	case IDF_DIRECT3D_ADDRESSV:							// vertikální adresování
		return CompNumPar(index, FSetD3DAddressV);

//	case IDF_DIRECT3D_BORDER:							// barva okolí textur
//		return CompNumPar(index, FSetD3DBorder);

	case IDF_DIRECT3D_BIAS:								// zjemnìní vzdálených textur
		return CompNumPar(index, FSetD3DBias);

	case IDF_DIRECT3D_TFACTOR:							// faktor textur
		return CompNumPar(index, FSetD3DTFactor);

	case IDF_DIRECT3D_MIPMAPS:							// poèet mipmaps
		return CompNumPar(index, FSetD3DMipMaps);

	case IDF_DIRECT3D_ZGROUP:							// renderovací skupina
		return CompNumPar(index, FSetD3DZGroup);

	case IDF_DIRECT3D_ZSORT:							// tøídìní skupiny
		return CompLogPar(index, FSetD3DZSort);

	case IDF_DIRECT3D_ZWRITE:							// zápis hloubkové informace
		return CompLogPar(index, FSetD3DZWrite);

	case IDF_DIRECT3D_ZTEST:							// hloubkový test
		return CompNumPar(index, FSetD3DZTest);

	case IDF_DIRECT3D_NORMALS:							// generování normál
		if (!CompNumPar(index, FD3DNormals))
		{
			CompAddItem(FD3DNormals0);
		}
		return true;

	case IDF_DIRECT3D_RENDER:							// renderování bez zobrazení
		CompAddItem(FD3DRender);
		return true;

	case IDF_SHADOW_RANGE:								// dosah stínu
		return CompNumPar(index, FSetD3DShadowRange);

	case IDF_DIRECT3D_LOD_OBJECT:						// objekt pro LOD
		return CompNumPar(index, FSetD3DLODObject);

	case IDF_DIRECT3D_LOD_DISTANCE:						// vzdálenost pro LOD
		return CompNumPar(index, FSetD3DLODDistance);

	case IDF_DIRECT3D_MORPH_MODEL:						// pøidání vzoru morfování
		return CompNumPar(index, FSetD3DMorphModel);

	case IDF_DIRECT3D_MORPH_LEVEL:						// nastavení stupnì morfování
		return CompNumPar(index, FSetD3DMorphLevel);

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------
	 

	case IDF_ACTIVE:									// nastavení aktivní aplikace
		return CompLogPar(index, FSetActive);

	case IDF_TABLE_WIDTH:								// nastavení šíøky sloupce tabulky
		return CompNumPar(index, FSetTableWidth);

	case IDF_TABLE_ALIGN:								// nastavení zarovnání sloupce tabulky
		return CompNumPar(index, FSetTableAlign);

	case IDF_PRINT_PICTURE:
		return CompPicPar(index, FPicPrint2);			// tisk obrázku

	default:
		return false;
	}
}


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu s podparametrem pøíkazù (hledaným podle identifikace), Jump = pøeskoèení skupiny

void CompComSubPar(int index, int idf)
{
// korekce identifikace funkce
	idf -= IDF;

// ukazatel výchozího prvku
	PETPROG*	item = BufEdi + index;
	PETPROG2*	item2 = BufEdi2 + index;
	int zahlavi = ProgNum;

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
				CompComGrp(index, FCommandExec, FCommandExec1))
			{
				ProgBuf[zahlavi].Jump = ProgNum - zahlavi;
				return;
			}

// posun pro pøíští prvek
			posun = item2->Items;

// dokud je další potomek
		} while (item->Param & PETPROG_NEXT);
	}

// použije se implicitní hodnota - prázdný pøíkaz
	CompAddItem(FCommandExec0);
	ProgBuf[zahlavi].Jump = ProgNum - zahlavi;
}
