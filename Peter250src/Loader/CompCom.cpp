
#include "Main.h"

/***************************************************************************\
*																			*
*						Kompilace programu - p��kaz							*
*																			*
\***************************************************************************/

#pragma optimize("s", on)			// optimalizace na minim�ln� velikost


/////////////////////////////////////////////////////////////////////////////
// p�id�n� jednoho prvku do bufferu programu (vrac� index prvku)

int CompAddItem(PROCCOM func) { return CompAddItem(func, 0, -1); };

int CompAddItem(PROCCOM func, int data) { return CompAddItem(func, data, -1); };

int CompAddItem(PROCCOM func, int data, int list)
{
// zaji�t�n� velikosti bufferu
	if (ProgNum >= ProgMax)				// bude p�ete�en� bufferu ?
	{
		ProgMax += 256;					// zv��en� ��ta�e velikosti bufferu
		MemBuf(ProgBuf, ProgMax);		// zv��en� velikosti bufferu
	}

// adresa vkl�dan�  polo�ky programu
	EXECITEM* item = ProgBuf + ProgNum;	// adresa polo�ky programu
	ProgNum++;							// zv��en� ��ta�e polo�ek

// inicializace prvku programu
	item->ExecCom = func;				// volan� funkce polo�ky
	item->Data = data;					// data polo�ky
	item->List = list;					// seznam
	item->Jump = 0;						// po�et prvk� k p�esko�en�

	return ProgNum-1;					// vrac� index prvku
}


/////////////////////////////////////////////////////////////////////////////
// p�eklad p��kazu CASE

bool CompCase(int index)
{
// adresa prvku
	PETPROG*	item = BufEdi + index;
	PETPROG2*	item2 = BufEdi2 + index;

// kontrola, zda m� polo�ka n�jak� potomky
	if ((item->Param & PETPROG_CHILDS) == 0) return false;

// ulo�en� z�hlav� p��kazu (zat�m verze s ��slem)
	int zahlavi = CompAddItem(FCaseReal, 0);

// typ dat (-1 = nic)
	int bufID = -1;					// ID typu dat

// --------------- na�ten� v�raz� k v�tven� ----------------------

// nalezen� v�tve s testovan�m v�razem
	int posun = 1;
	int inx = index;
	do {

// adresa dal��ho potomka
		inx += posun;
		item += posun;
		item2 += posun;

// test, zda to je hledan� prvek
		if ((item->Func == IDF_CASE_SUM - IDF) &&
			(item->Param & PETPROG_CHILDS))
		{

// interpretace jako ��seln� v�raz
			if (CompNum(inx+1))
			{
				bufID = BufNumID;		// typ dat ��slo
//				ProgBuf[zahlavi].ExecCom = FCaseReal;
				break;
			}

// interpretace jako logick� v�raz
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

// interpretace jako obr�zek
			if (CompPic(inx+1))
			{
				bufID = BufPicID;		// typ dat obr�zek
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

// posun pro p��t� prvek
		posun = item2->Items;

// dokud je dal�� potomek
	} while (item->Param & PETPROG_NEXT);

// nenalezeno
	if (bufID < 0)
	{
		ProgNum = zahlavi;
		return false;
	}

// ------------------- na�ten� test� v�tv� ------------------

// buffer index� v�tv�
	CBufInt bufon;

// nalezen� v�tve case
	posun = 1;
	inx = index;
	item = BufEdi + inx;
	item2 = BufEdi2 + inx;
	int caseinx = 0;
	do {

// adresa dal��ho potomka
		inx += posun;
		item += posun;
		item2 += posun;

// test, zda to je hledan� prvek
		if ((item->Func == IDF_CASE_ON - IDF) &&
			(item->Param & PETPROG_CHILDS))
		{

// ��ta� obsluhy v�tv�
			caseinx--;

// vyhled�n� test� v�tve
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

// posun pro p��t� prvek
		posun = item2->Items;

// dokud je dal�� potomek
	} while (item->Param & PETPROG_NEXT);


// -------------- na�ten� p��kazu DEFAULT ----------------

// buffer index� skok�
	CBufInt bufjump;
	bufjump.Add(ProgNum);
	CompComSubPar(index, IDF_CASE_DEF);


// --------------------- na�ten� obsluh v�tv� ----------------

// nalezen� v�tve case
	posun = 1;
	inx = index;
	item = BufEdi + inx;
	item2 = BufEdi2 + inx;
	caseinx = 0;
	do {

// adresa dal��ho potomka
		inx += posun;
		item += posun;
		item2 += posun;

// test, zda to je hledan� prvek
		if ((item->Func == IDF_CASE_ON - IDF) &&
			(item->Param & PETPROG_CHILDS))
		{

// ��ta� obsluhy v�tv�
			caseinx--;
			int hl = ProgNum;

// na�ten� p��kaz�
			if (!CompComGrp(inx, FCommandExec, FCommandExec1))
			{
				CompAddItem(FCommandExec0);
			}

// �schova indexu skoku
			bufjump.Add(hl);

// p�emapov�n� podm�nek CASE
			for (int j = bufon.Num()-1; j >= 0; j--)
			{
				if (ProgBuf[bufon[j]].Jump == caseinx)
				{
					ProgBuf[bufon[j]].Jump = hl - bufon[j];
				}
			}	 
		}

// posun pro p��t� prvek
		posun = item2->Items;

// dokud je dal�� potomek
	} while (item->Param & PETPROG_NEXT);


// ----------------- p�emapov�n� skok� obsluh -----------

	for (int j = bufjump.Num()-1; j >= 0; j--)
	{
		ProgBuf[bufjump[j]].Jump = ProgNum - bufjump[j];
	}

	return true;
}


/////////////////////////////////////////////////////////////////////////////
// p�eklad p��kazu (vrac� true = operace OK)

bool _fastcall CompCom(int index)
{
// adresa zdrojov�ho prvku
	if ((DWORD)index >= (DWORD)BufEdiN) return false;
	PETPROG*	item = BufEdi + index;
//	PETPROG2*	item2 = BufEdi2 + index;
	int refinx = item->RefIndex;
	PETPROG2*	prg2;

// kontrola, zda je polo�ka vypnuta
	if ((item->Param & (PETPROG_OFF | PETPROG_OFF_DEP)) != 0) return false;

// v�tven� podle funkce
	switch (item->Func + IDF)
	{
	case IDF_IF:
		CompAddItem(FIf);				// podm�nka IF
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
		CompAddItem(FBreak);			// p�eru�en� cyklu Break
		return true;
	
	case IDF_CASE:
		return CompCase(index);			// p�eklad p��kazu CASE

	case IDF_REAL:						// ��seln� prom�nn�
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

	case IDF_ICON:							// prom�nn� ikony
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

	case IDF_SND:							// prom�nn� zvuku
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

	case IDF_MUS:							// prom�nn� hudby
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
		return CompFunc(index, IDF_FNC);	// funkce bez n�vratu

	case IDF_MAP:							// prom�nn� plochy
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

	case IDF_SPRITE:							// prom�nn� sprajtu
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

	case IDF_TEXT:							// prom�nn� textu
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
		CompAddItem(FKrok);				// Pet��k - krok vp�ed
		return true;

	case IDF_VLEVO:
		CompAddItem(FVlevo);			// Pet��k - vlevo
		return true;

	case IDF_VPRAVO:
		CompAddItem(FVpravo);			// Pet��k - vpravo
		return true;

	case IDF_VZAD:
		CompAddItem(FVzad);				// Pet��k - vzad
		return true;

	case IDF_BOOL:						// logick� prom�nn�
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
		CompAddItem(FKrok2);			// Petra - krok vp�ed
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
		CompAddItem(FReturn);			// n�vrat z funkce
		return true;

	case IDF_PETRA_RYCHLE:
		return CompLogPar(index, FVisible2); // Petra - viditelnost

	case IDF_PREDMET:
		return CompIcoPar(index, FSetPredmet); // nastaven� p�edm�tu p�ed Petrem

	case IDF_SMER:
		return CompNumPar(index, FSetSmer);	// Petr - nastaven� sm�ru

	case IDF_POZICEX:
		return CompNumPar(index, FSetPozX);	// Petr - nastaven� pozice X

	case IDF_POZICEY:
		return CompNumPar(index, FSetPozY);	// Petr - nastaven� pozice Y

	case IDF_PETRA_PREDMET:
		return CompIcoPar(index, FSetPredmet2); // nastaven� p�edm�tu p�ed Petrou

	case IDF_PETRA_SMER:
		return CompNumPar(index, FSetSmer2); // Petra - nastaven� sm�ru

	case IDF_PETRA_POZICEX:
		return CompNumPar(index, FSetPozX2); // Petra - nastaven� pozice X

	case IDF_PETRA_POZICEY:
		return CompNumPar(index, FSetPozY2); // Petra - nastaven� pozice Y

	case IDF_WAIT:
		if (!CompNumPar(index, FWait))		// �ek�n�
		{
			CompAddItem(FWait1);
		}
		return true;

	case IDF_PIC:							// prom�nn� obr�zku
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
		return CompNumPar(index, FSetMouseX);	// nastaven� my�i X

	case IDF_MOUSE_Y:
		return CompNumPar(index, FSetMouseY);	// nastaven� my�i Y

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
		return CompNumPar(index, FSetSwcN1);	// Petr - nastaven� ��sla 1

	case IDF_PETRA_SWCN1:
		return CompNumPar(index, FSetSwcN12);	// Petra - nastaven� ��sla 1

	case IDF_PETER_SWCN2:
		return CompNumPar(index, FSetSwcN2);	// Petr - nastaven� ��sla 2

	case IDF_PETRA_SWCN2:
		return CompNumPar(index, FSetSwcN22);	// Petra - nastaven� ��sla 2

	case IDF_PETER_SWCN3:
		return CompNumPar(index, FSetSwcN3);	// Petr - nastaven� ��sla 3

	case IDF_PETRA_SWCN3:
		return CompNumPar(index, FSetSwcN32);	// Petra - nastaven� ��sla 3

	case IDF_PETER_SWC1:
		return CompLogPar(index, FSetSwc1);		// Petr - nastaven� p�ep�na�e 1

	case IDF_PETRA_SWC1:
		return CompLogPar(index, FSetSwc12);	// Petra - nastaven� p�ep�na�e 1

	case IDF_PETER_SWC2:
		return CompLogPar(index, FSetSwc2);		// Petr - nastaven� p�ep�na�e 2

	case IDF_PETRA_SWC2:
		return CompLogPar(index, FSetSwc22);	// Petra - nastaven� p�ep�na�e 2

	case IDF_PETER_SWC3:
		return CompLogPar(index, FSetSwc3);		// Petr - nastaven� p�ep�na�e 3

	case IDF_PETRA_SWC3:
		return CompLogPar(index, FSetSwc32);	// Petra - nastaven� p�ep�na�e 3

	case IDF_PETER_SWC4:
		return CompLogPar(index, FSetSwc4);		// Petr - nastaven� p�ep�na�e 4

	case IDF_PETRA_SWC4:
		return CompLogPar(index, FSetSwc42);	// Petra - nastaven� p�ep�na�e 4

	case IDF_PETER_SWC5:
		return CompLogPar(index, FSetSwc5);		// Petr - nastaven� p�ep�na�e 5

	case IDF_PETRA_SWC5:
		return CompLogPar(index, FSetSwc52);	// Petra - nastaven� p�ep�na�e 5

	case IDF_STATUS_LINE:
		return CompTxtPar(index, FSetStatus);	// nastaven� stavov�ho ��dku

	case IDF_CAPTION:
		return CompTxtPar(index, FSetCaption);	// nastaven� nadpisu

	case IDF_SOUND_PLAY:
		CompAddItem(FSoundPlay);				// p�ehr�t� zvuku
		CompSndSubPar(index, IDF_SOUND_PLAY_SOUND);
		CompNumSubPar(index, IDF_SOUND_PLAY_VOL, 1);
		CompNumSubPar(index, IDF_SOUND_PLAY_BAL, 0);
		CompNumSubPar(index, IDF_SOUND_PLAY_SPEED, 1);
		CompNumSubPar(index, IDF_SOUND_PLAY_LOOP, 1);
		CompLogSubPar(index, IDF_SOUND_PLAY_STOP, true);
		return true;

	case IDF_SOUND_STOP:
		if (!CompSndPar(index, FSoundStop1))	// zastaven� zvuku
		{
			CompAddItem(FSoundStop);			// zastaven� zvuku
		}
		return true;

	case IDF_SOUND_DISTINGUISH:
		return CompNumPar(index, FSetSoundDistinguish);// nastaven� rozli�ovac�ho k�du zvuku

	case IDF_POINT:
		CompAddItem(FPoint);					// zobrazen� bodu
		CompNumSubPar(index, IDF_GRAPH_COLOR, DEFCOLOR);
		CompNumSubPar(index, IDF_GRAPH_WIDTH, 1);
		CompNumSubPar(index, IDF_POINT_X, 0);
		CompNumSubPar(index, IDF_POINT_Y, 0);
		return true;

	case IDF_LINE:
		CompAddItem(FLine);						// zobrazen� ��ry
		CompNumSubPar(index, IDF_GRAPH_COLOR, DEFCOLOR);
		CompNumSubPar(index, IDF_GRAPH_WIDTH, 1);
		CompNumSubPar(index, IDF_LINE_X1, 0);
		CompNumSubPar(index, IDF_LINE_Y1, 0);
		CompNumSubPar(index, IDF_LINE_X2, 1);
		CompNumSubPar(index, IDF_LINE_Y2, 1);
		return true;

	case IDF_BOX:
		CompAddItem(FBox);						// zobrazen� obd�ln�ku
		CompNumSubPar(index, IDF_GRAPH_COLOR, DEFCOLOR);
		CompNumSubPar(index, IDF_GRAPH_WIDTH, 1);
		CompNumSubPar(index, IDF_BOX_X1, 0);
		CompNumSubPar(index, IDF_BOX_Y1, 0);
		CompNumSubPar(index, IDF_BOX_X2, 1);
		CompNumSubPar(index, IDF_BOX_Y2, 1);
		return true;

	case IDF_BOX_FILL:
		CompAddItem(FBoxFill);					// zobrazen� vypln�n�ho obd�ln�ku
		CompNumSubPar(index, IDF_GRAPH_COLOR, DEFCOLOR);
		CompNumSubPar(index, IDF_BOX_FILL_X1, 0);
		CompNumSubPar(index, IDF_BOX_FILL_Y1, 0);
		CompNumSubPar(index, IDF_BOX_FILL_X2, 2000000);
		CompNumSubPar(index, IDF_BOX_FILL_Y2, 2000000);
		return true;

	case IDF_FILL:
		CompAddItem(FFill);					// v�pl�
		CompNumSubPar(index, IDF_GRAPH_COLOR, DEFCOLOR);
		CompNumSubPar(index, IDF_FILL_X, 0);
		CompNumSubPar(index, IDF_FILL_Y, 0);
		return true;

	case IDF_CIRCLE:
		CompAddItem(FCircle);				// kru�nice
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
		CompAddItem(FTriangle);			// troj�heln�k
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
		CompAddItem(FComVKeyWait);		// vtup kl�vesy s �ek�n�m
		return true;

	case IDF_KLAV_CHAR_WAIT:
		CompAddItem(FComCharWait);		// vstup znaku s �ek�n�m
		return true;

	case IDF_STATUS_NUMLOCK:
		return CompLogPar(index, FSetNumLock);	// nastaven� p�ep�na�en Num Lock

	case IDF_STATUS_CAPSLOCK:
		return CompLogPar(index, FSetCapsLock);	// nastaven� p�ep�na�en Caps Lock

	case IDF_STATUS_SCROLLLOCK:
		return CompLogPar(index, FSetScrollLock);	// nastaven� p�ep�na�en Scroll Lock

	case IDF_STATUS_INSERT:
		return CompLogPar(index, FSetInsertLock);	// nastaven� p�ep�na�en Insert

	case IDF_GRAPH_TEXT:
		CompAddItem(FGraphText);					// zobrazen� textu
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
		return CompIcoPar(index, FSetPredmetPoz); // nastaven� p�edm�tu pod Petrem

	case IDF_PETRA_PREDMET_POZ:
		return CompIcoPar(index, FSetPredmet2Poz); // nastaven� p�edm�tu pod Petrou

	case IDF_PIC_DISP:
		CompAddItem(FPicDisp);					// zobrazen� obr�zku
		CompPicSubPar(index, IDF_PIC_BACK);
		CompNumSubPar(index, IDF_PIC_X, 0);
		CompNumSubPar(index, IDF_PIC_Y, 0);
		CompNumSubPar(index, IDF_PIC_BLOCK_X, 0);
		CompNumSubPar(index, IDF_PIC_BLOCK_Y, 0);
		CompNumSubPar(index, IDF_PIC_BLOCK_W, 2000000);
		CompNumSubPar(index, IDF_PIC_BLOCK_H, 2000000);
		return true;

	case IDF_TRANSPARENT:
		CompAddItem(FTransparent);					// zpr�hledn�n� okna
		return true;

	case IDF_MOUSE_CURSOR:
		CompAddItem(FMouseCursor);				// kurzor my�i
		CompIcoSubPar(index, IDF_MOUSE_CURSOR_PIC);
		CompNumSubPar(index, IDF_MOUSE_CURSOR_X, 0);
		CompNumSubPar(index, IDF_MOUSE_CURSOR_Y, 0);
		CompNumSubPar(index, IDF_MOUSE_CURSOR_W, 2000000);
		CompNumSubPar(index, IDF_MOUSE_CURSOR_H, 2000000);
		return true;

	case IDF_MUSIC_PLAY:
		return CompMusPar(index, FMusicPlay);	// p�ehr�t� hudby

	case IDF_MUSIC_LOOP:
		return CompMusPar(index, FMusicLoop);	// opakovan� p�ehr�v�n� hudby

	case IDF_MUSIC_STOP:
		CompAddItem(FMusicStop);				// zastaven� hudby
		return true;

	case IDF_MUSIC_PAUSE:
		if (!CompLogPar(index, FMusicPauseSet))
		{
			CompAddItem(FMusicPause);				// pozastaven� hudby
		}
		return true;

	case IDF_MUSIC_POS:
		return CompNumPar(index, FSetMusicPos);	// nastaven� pozice hudby

	case IDF_CD_PLAY:
		if (!CompNumPar(index, FCDPlay))
		{
			CompAddItem(FCDPlay);				// p�ehr�v�n� CD
			CompAddItemNum((BYTE)1);			// implicitn� stopa 1	
		}
		return true;

	case IDF_CD_STOP:
		CompAddItem(FCDStop);					// zastaven� CD
		return true;

	case IDF_CD_PAUSE:
		if (!CompLogPar(index, FCDPauseSet))
		{
			CompAddItem(FCDPause);				// pozastaven� CD
		}
		return true;

	case IDF_CD_EJECT:
		if (!CompLogPar(index, FCDEjectSet))
		{
			CompAddItem(FCDEject);				// zasunut� CD
		}
		return true;

	case IDF_CD_POS:
		return CompNumPar(index, FCDSetPos);	// nastaven� pozice CD

	case IDF_CD_TRACK:
		return CompNumPar(index, FCDSetTrack);	// nastaven� stopy CD

	case IDF_CD_TRACK_POS:
		return CompNumPar(index, FCDSetTrackPos);// nastaven� pozice stopy CD

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_DLL_FREE:						// uvoln�n� DLL knihovny
		return CompNumPar(index, FDLLFree);

	case IDF_DLL_EXEC:						// proveden� funkce DLL
		return CompNumPar(index, FDLLExecCom);

	case IDF_MEMORY_FREE:					// zru�en� pam�ov�ho bloku
		return CompNumPar(index, FMemoryFree);

	case IDF_MEMORY_READ:					// ukazatel �ten� z pam�ti
		return CompNumPar(index, FSetMemoryRead);

	case IDF_MEMORY_WRITE:					// ukazatel z�pisu do pam�ti
		return CompNumPar(index, FSetMemoryWrite);

	case IDF_MEMORY_COPY:					// kopie dat
		return CompNumPar(index, FMemoryCopy);

	case IDF_MEMORY_BYTES:					// bajt se znam�nkem
		return CompNumPar(index, FSetMemoryByteS);

	case IDF_MEMORY_BYTEU:					// bajt bez znam�nka
		return CompNumPar(index, FSetMemoryByteU);

	case IDF_MEMORY_WORDS:					// slovo se znam�nkem
		return CompNumPar(index, FSetMemoryWordS);

	case IDF_MEMORY_WORDU:					// slovo bez znam�nka
		return CompNumPar(index, FSetMemoryWordU);

	case IDF_MEMORY_3BYTES:					// 3-bajt se znam�nkem
		return CompNumPar(index, FSetMemory3ByteS);

	case IDF_MEMORY_3BYTEU:					// 3-bajt bez znam�nka
		return CompNumPar(index, FSetMemory3ByteU);

	case IDF_MEMORY_DWORDS:					// dvouslovo se znam�nkem
		return CompNumPar(index, FSetMemoryDWordS);

	case IDF_MEMORY_DWORDU:					// dvouslovo bez znam�nka
		return CompNumPar(index, FSetMemoryDWordU);

	case IDF_MEMORY_FLOAT:					// re�ln� ��slo 4 bajty
		return CompNumPar(index, FSetMemoryFloat);

	case IDF_MEMORY_DOUBLE:					// re�ln� ��slo 8 bajt�
		return CompNumPar(index, FSetMemoryDouble);

	case IDF_MEMORY_BOOL:					// logick� hodnota
		return CompLogPar(index, FSetMemoryBool);

	case IDF_MEMORY_TEXT0:					// text zakon�en� nulou
		return CompTxtPar(index, FSetMemoryText0);

	case IDF_MEMORY_TEXT0P:					// ukazatel na text zakon�en� nulou
		return CompTxtPar(index, FSetMemoryText0P);

	case IDF_MEMORY_TEXTN:					// text s pevnou d�lkou
		return CompTxtPar(index, FSetMemoryTextN);

	case IDF_MEMORY_TEXTN_LEN:				// d�lka textu s pevnou d�lkou
		return CompNumPar(index, FSetMemoryTextNLen);

	case IDF_MEMORY_LOAD:					// na�ten� bloku dat
		return CompNumPar(index, FMemoryLoad);

	case IDF_MEMORY_SAVE:					// z�pis bloku dat
		return CompNumPar(index, FMemorySave);

	case IDF_INI_FILE:						// jm�no konfigura�n�ho souboru
		return CompTxtPar(index, FSetIniFile);

	case IDF_INI_SECTION:					// jm�no konfigura�n� sekce
		return CompTxtPar(index, FSetIniSection);

	case IDF_INI_VALUE:						// jm�no konfigura�n�ho parametru
		return CompTxtPar(index, FSetIniValue);

	case IDF_INI_STRING:					// z�pis konfigura�n�ho textu
		if (!CompTxtPar(index, FSetIniString))
		{
			CompAddItem(FSetIniDel);
		}
		return true;

	case IDF_INI_NUM:						// z�pis konfigura�n�ho ��sla
		if (!CompNumPar(index, FSetIniNum))
		{
			CompAddItem(FSetIniDel);
		}
		return true;

	case IDF_REG_KEY:						// skupina kl��� registr�
		return CompNumPar(index, FSetRegKey);

	case IDF_REG_SUBKEY:					// kl�� registr�
		return CompTxtPar(index, FSetRegSubkey);

	case IDF_REG_VALUE:						// jm�no polo�ky registru
		return CompTxtPar(index, FSetRegValue);

	case IDF_REG_STRING:					// z�pis textu do registru
		if (!CompTxtPar(index, FSetRegString))
		{
			CompAddItem(FSetRegDel);
		}
		return true;

	case IDF_REG_NUM:						// z�pis ��sla do registru
		if (!CompNumPar(index, FSetRegNum))
		{
			CompAddItem(FSetRegDel);
		}
		return true;

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------

	case IDF_FILE_FILENEW:
		return CompTxtPar(index, FFileNew);		// nov� soubor

	case IDF_FILE_DIRNEW:
		return CompTxtPar(index, FDirNew);		// nov� adres��

	case IDF_FILE_DELETE:
		return CompTxtPar(index, FFileDelete);	// zru�en� souboru/adres��e

	case IDF_FILE_ACT:
		return CompTxtPar(index, FFileAct);		// nastaven� aktivn�ho adres��e

	case IDF_FILE_DISKLABEL:
		return CompTxtPar(index, FDiskLabel);	// nastaven� jmenovky disku

	case IDF_FILE_READ:
		return CompTxtPar(index, FFileRead);	// jm�no souboru pro �ten�

	case IDF_FILE_WRITE:
		return CompTxtPar(index, FFileWrite);	// jm�no souboru pro z�pis

	case IDF_FILE_COPY:
		CompAddItem(FFileCopy);					// kopie souboru
		return true;

	case IDF_FILE_MOVE:
		CompAddItem(FFileMove);					// p�esun souboru
		return true;

	case IDF_FILE_RO:
		return CompLogPar(index, FFileRO);		// nastaven� atributu RO

	case IDF_FILE_HID:
		return CompLogPar(index, FFileHID);		// nastaven� atributu HID

	case IDF_FILE_SYS:
		return CompLogPar(index, FFileSYS);		// nastaven� atributu SYS

	case IDF_FILE_ARC:
		return CompLogPar(index, FFileARC);		// nastaven� atributu ARC

	case IDF_FILE_CREATE:
		return CompNumPar(index, FFileCreate);	// nastaven� �asu vytvo�en� souboru

	case IDF_FILE_OPEN:
		return CompNumPar(index, FFileOpen);	// nastaven� �asu posledn�ho �ten� ze souboru

	case IDF_FILE_MODI:
		return CompNumPar(index, FFileModi);	// nastaven� �asu posledn�ho z�pisu do souboru

	case IDF_FILE_BYTES:
		return CompNumPar(index, FFileByteS);	// z�pis bajtu se znam�nkem

	case IDF_FILE_BYTEU:
		return CompNumPar(index, FFileByteU);	// z�pis bajtu bez znam�nka

	case IDF_FILE_WORDS:
		return CompNumPar(index, FFileWordS);	// z�pis slova se znam�nkem

	case IDF_FILE_WORDU:
		return CompNumPar(index, FFileWordU);	// z�pis slova bez znam�nka

	case IDF_FILE_DWORDS:
		return CompNumPar(index, FFileDWordS);	// z�pis dvojslova se znam�nkem

	case IDF_FILE_DWORDU:
		return CompNumPar(index, FFileDWordU);	// z�pis dvojslova bez znam�nka

	case IDF_FILE_FLOAT:
		return CompNumPar(index, FFileFloat);	// z�pis ��sla FLOAT

	case IDF_FILE_DOUBLE:
		return CompNumPar(index, FFileDouble);	// z�pis ��sla DOUBLE

	case IDF_FILE_LOGIC:
		return CompLogPar(index, FFileLogic);	// z�pis logick� hodnoty

	case IDF_FILE_TEXT0:
		return CompTxtPar(index, FFileText0);	// z�pis textu zakon�en�ho nulou

	case IDF_FILE_TEXTCR:
		return CompTxtPar(index, FFileTextCR);	// z�pis textu zakon�en�ho CR

	case IDF_FILE_TEXTN:
		return CompTxtPar(index, FFileTextN);	// z�pis textu s pevnou d�lkou

	case IDF_FILE_TEXT_UNICODE:					// text v k�du UNICODE
		return CompLogPar(index, FFileTextUnicode);

	case IDF_FILE_READOFF:
		return CompNumPar(index, FFileReadOff);	// offset pro �ten�

	case IDF_FILE_WRITEOFF:
		return CompNumPar(index, FFileWriteOff);// offset pro z�pis

	case IDF_DATETIME_CURRENT:
		return CompNumPar(index, FTimeCurrent);	// nastaven� aktu�ln�ho data a �asu

	case IDF_FILE_SIZE:
		return CompNumPar(index, FFileSize);	// nastaven� velikosti souboru

	case IDF_FILE_ERROR:
		return CompLogPar(index, FFileError);	// p��znak chyby soubor�

	case IDF_FILE_TEXTN_LEN:
		return CompNumPar(index, FFileTextNLen);// d�lka textu s pevnou d�lkou

	case IDF_FILE_3BYTES:
		return CompNumPar(index, FFile3ByteS);	// z�pis 3 bajt� se znam�nkem

	case IDF_FILE_3BYTEU:
		return CompNumPar(index, FFile3ByteU);	// z�pis 3 bajt� bez znam�nka

	case IDF_FILE_ICON:
		return CompIcoPar(index, FFileIcon);	// z�pis ikony

	case IDF_FILE_PICTURE:
		return CompPicPar(index, FFilePicture);	// z�pis obr�zku

	case IDF_FILE_SOUND:
		return CompSndPar(index, FFileSound);	// z�pis zvuku

	case IDF_FILE_MUSIC:
		return CompMusPar(index, FFileMusic);	// z�pis hudby

	case IDF_FILE_SPRITE:
		return CompSprPar(index, FFileSprite);	// z�pis sprajtu

	case IDF_DIALOG_BOX:
		CompAddItem(FDialogBox);				// dialog box
		CompTxtSubPar(index, IDF_DIALOG_BOX_CAPTION);
		CompTxtSubPar(index, IDF_DIALOG_BOX_TEXT);
		CompNumSubPar(index, IDF_DIALOG_BOX_ICON, 0);
		CompNumSubPar(index, IDF_DIALOG_BOX_BUTTON, 0);
		CompNumSubPar(index, IDF_DIALOG_BOX_DEFAULT, 1);
		return true;

	case IDF_SPRITE_SHIFT_X:
		return CompNumPar(index, FSpriteShiftX); // posun sprajt� ve sm�ru X

	case IDF_SPRITE_SHIFT_Y:
		return CompNumPar(index, FSpriteShiftY); // posun sprajt� ve sm�ru Y

	case IDF_SPRITE_WIN_X1:
		return CompNumPar(index, FSpriteWinX1);	// lev� okraj okna sprajt�

	case IDF_SPRITE_WIN_Y1:
		return CompNumPar(index, FSpriteWinY1);	// doln� okraj okna sprajt�

	case IDF_SPRITE_WIN_X2:
		return CompNumPar(index, FSpriteWinX2);	// prav� okraj okna sprajt�

	case IDF_SPRITE_WIN_Y2:
		return CompNumPar(index, FSpriteWinY2);	// horn� okraj okna sprajt�

	case IDF_CONSOLE_ON:
//		if (!CompLogPar(index, FConsoleOnSet))	// re�im konzoly
//		{
			CompAddItem(FConsoleOn);
//		}
		return true;

	case IDF_CONSOLE_OUT:
		return CompTxtPar(index, FConsoleOut);	// v�stup textu na konzolu

	case IDF_CONSOLE_ERR:
		return CompTxtPar(index, FConsoleErr);	// v�stup chybov�ho textu na konzolu

	case IDF_RETURN_CODE:
		return CompNumPar(index, FSetReturnCode); // n�vratov� k�d
		 
	case IDF_EXEC:
		return CompTxtPar(index, FExec);		// spu�t�n� programu

	case IDF_EXEC_WAIT:
		return CompTxtPar(index, FExecWait);	// spu�t�n� programu s �ek�n�m

//	case IDF_DIALOG_ON:
//		if (!CompLogPar(index, FDialogOnSet))	// re�im dialogu
//		{
//			CompAddItem(FDialogOn);
//		}
//		return true;

	case IDF_WINDOW_ID:
		return CompNumPar(index, FWindowID);	// aktivn� prvek

	case IDF_WINDOW_DEL:
		return CompNumPar(index, FWindowDel);	// zru�en� okna

	case IDF_WINDOW_TEXT:
		return CompTxtPar(index, FWindowText);	// text okna

	case IDF_WINDOW_X:
		return CompNumPar(index, FWindowX);		// po��tek okna X

	case IDF_WINDOW_Y:
		return CompNumPar(index, FWindowY);		// po��tek okna Y

	case IDF_WINDOW_W:
		return CompNumPar(index, FWindowW);		// ���ka okna

	case IDF_WINDOW_H:
		return CompNumPar(index, FWindowH);		// v��ka okna

	case IDF_WINDOW_W_INT:
		return CompNumPar(index, FWindowWInt);	// vnit�n� ���ka okna

	case IDF_WINDOW_H_INT:
		return CompNumPar(index, FWindowHInt);	// vnit�n� v��ka okna

	case IDF_WINDOW_VISIBLE:
		if (!CompLogPar(index, FWindowVisibleSet)) // viditelnost okna
		{
			CompAddItem(FWindowVisible);
		}
		return true;

	case IDF_WINDOW_DISABLE:
		if (!CompLogPar(index, FWindowDisableSet)) // z�kaz okna
		{
			CompAddItem(FWindowDisable);
		}
		return true;

	case IDF_CHECKBOX_ON:
		if (!CompLogPar(index, FCheckBoxOnSet))	// p�ep�na�
		{
			CompAddItem(FCheckBoxOn);
		}
		return true;

	case IDF_FOCUS:
		return CompNumPar(index, FFocus);	// okno s fokusem

	case IDF_FONTBOLD:
		if (!CompLogPar(index, FFontBoldSet)) // zv�razn�n� text
		{
			CompAddItem(FFontBold);
		}
		return true;

	case IDF_FONTFIXED:
		if (!CompLogPar(index, FFontFixedSet)) // text s pevnou rozte��
		{
			CompAddItem(FFontFixed);
		}
		return true;

	case IDF_FONTHEIGHT:
		return CompNumPar(index, FFontHeightSet); // nastaven� v��ky znak�

	case IDF_FONTITALIC:
		if (!CompLogPar(index, FFontItalicSet)) // �ikm� text
		{
			CompAddItem(FFontItalic);
		}
		return true;

	case IDF_FONTUNDER:
		if (!CompLogPar(index, FFontUnderSet)) // potr�en� text
		{
			CompAddItem(FFontUnder);
		}
		return true;

	case IDF_FONTSTRIKE:
		if (!CompLogPar(index, FFontStrikeSet)) // p�e�krnut� text
		{
			CompAddItem(FFontStrike);
		}
		return true;

	case IDF_FONTSERIF:
		if (!CompLogPar(index, FFontSerifSet)) // text s pati�kami
		{
			CompAddItem(FFontSerif);
		}
		return true;

	case IDF_FONTWIDTH:
		return CompNumPar(index, FFontWidthSet); // nastaven� ���ky znak�

	case IDF_FONTANGLE:
		return CompNumPar(index, FFontAngleSet); // nastaven� ���ky znak�

	case IDF_FONTCOL:
		return CompNumPar(index, FFontColSet); // nastaven� barvy textu

	case IDF_FONTBACK:
		return CompNumPar(index, FFontBackSet); // nastaven� barvy pozad�

	case IDF_LISTBOX_ADD:
		return CompTxtPar(index, FListBoxAdd);	// p�id�n� polo�ky do seznamu

	case IDF_LISTBOX_CLEAR:
		CompAddItem(FListBoxClear);			// vypr�zdn�n� seznamu
		return true;

	case IDF_BUTTON3_ON:
		if (!CompLogPar(index, FButton3OnSet))	// p�ep�na�
		{
			CompAddItem(FButton3On);
		}
		return true;

	case IDF_WINDOW_ICON:
		return CompIcoPar(index, FWindowIcon);

	case IDF_WINDOW_PIC:
		return CompPicPar(index, FWindowPic);

	case IDF_FULLSCREEN:
		if (!CompLogPar(index, FFullScreenSet))	// p�ep�na�
		{
			CompAddItem(FFullScreen);
		}
		return true;

	case IDF_KLAV_FLUSH:
		CompAddItem(FKlavFlush);			// vypr�zdn�n� bufferu kl�ves
		return true;

	case IDF_SOUND_POS:
		return CompNumPar(index, FSetSoundPos);	// nastaven� pozice zvuku

	case IDF_SOUND_PAUSE:
		if (!CompLogPar(index, FSoundPauseSet))
		{
			CompAddItem(FSoundPause);			// pozastaven� zvuku
		}
		return true;

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_CONNECT_ACT:
		return CompTxtPar(index, FSetConnectAct);	// nastaven� aktivn�ho spojen�

	case IDF_GAME_ACT:
		return CompTxtPar(index, FSetGameAct);		// nastaven� aktivn� hry

	case IDF_PLAYER_ACT:
		return CompTxtPar(index, FSetPlayerAct);	// nastaven� aktivn�ho hr��e

	case IDF_GAME_N1:
		return CompNumPar(index, FSetGameN1);		// nastaven� hern�ho ��sla 1

	case IDF_GAME_N2:
		return CompNumPar(index, FSetGameN2);		// nastaven� hern�ho ��sla 2

	case IDF_GAME_N3:
		return CompNumPar(index, FSetGameN3);		// nastaven� hern�ho ��sla 3

	case IDF_GAME_N4:
		return CompNumPar(index, FSetGameN4);		// nastaven� hern�ho ��sla 4

	case IDF_GAME_N5:
		return CompNumPar(index, FSetGameN5);		// nastaven� hern�ho ��sla 5

	case IDF_GAME_N6:
		return CompNumPar(index, FSetGameN6);		// nastaven� hern�ho ��sla 6

	case IDF_GAME_N7:
		return CompNumPar(index, FSetGameN7);		// nastaven� hern�ho ��sla 7

	case IDF_GAME_N8:
		return CompNumPar(index, FSetGameN8);		// nastaven� hern�ho ��sla 8

	case IDF_GAME_L1:
		return CompLogPar(index, FSetGameL1);		// nastaven� hern�ho p�ep�na�e 1

	case IDF_GAME_L2:
		return CompLogPar(index, FSetGameL2);		// nastaven� hern�ho p�ep�na�e 2

	case IDF_GAME_L3:
		return CompLogPar(index, FSetGameL3);		// nastaven� hern�ho p�ep�na�e 3

	case IDF_GAME_L4:
		return CompLogPar(index, FSetGameL4);		// nastaven� hern�ho p�ep�na�e 4

	case IDF_GAME_L5:
		return CompLogPar(index, FSetGameL5);		// nastaven� hern�ho p�ep�na�e 5

	case IDF_GAME_L6:
		return CompLogPar(index, FSetGameL6);		// nastaven� hern�ho p�ep�na�e 6

	case IDF_GAME_L7:
		return CompLogPar(index, FSetGameL7);		// nastaven� hern�ho p�ep�na�e 7

	case IDF_GAME_L8:
		return CompLogPar(index, FSetGameL8);		// nastaven� hern�ho p�ep�na�e 8

	case IDF_PLAYER_MAX:
		return CompNumPar(index, FSetPlayerMax);	// max. hr���

	case IDF_GAME_PACKET:
		if (!CompNumPar(index, FSetGamePacket))		// vysl�n� paketu
		{
			CompAddItem(FSetGamePacket0);
		}
		return true;

	case IDF_GAME_DATAN:
		return CompNumPar(index, FSetGameDataN);	// vysl�n� ��sla paketem

	case IDF_GAME_DATAT:
		return CompTxtPar(index, FSetGameDataT);	// vysl�n� textu paketem

	case IDF_GAME_DATAL:
		return CompLogPar(index, FSetGameDataL);	// vysl�n� p�ep�na�e paketem

	case IDF_GAME_DATAB:
		return CompNumPar(index, FSetGameDataB);	// vysl�n� bajtu paketem

	case IDF_UDP_SEND_IP:
		return CompTxtPar(index, FSetUDPSendIP);	// nastaven� vys�lac� IP adresy UDP

	case IDF_UDP_SEND_PORT:
		return CompNumPar(index, FSetUDPSendPort);	// nastaven� vys�lac�ho portu UDP

	case IDF_UDP_REC_SIZE:
		return CompNumPar(index, FSetUDPRecSize);	// nastaven� velikosti p�ij�mac�ho bufferu UDP

	case IDF_UDP_REC_IP:
		return CompTxtPar(index, FSetUDPRecIP);		// nastaven� p�ij�mac� IP adresy UDP

	case IDF_UDP_REC_PORT:
		return CompNumPar(index, FSetUDPRecPort);	// nastaven� p�ij�mac�ho portu UDP

	case IDF_UDP_MEM:
		return CompNumPar(index, FSetUDPMem);		// odesl�n� bin�rn�ho paketu UDP

	case IDF_UDP_TEXT:
		return CompTxtPar(index, FSetUDPText);		// odesl�n� textov�ho paketu UDP

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------


	case IDF_BUTTON_ESC:
		return CompNumPar(index, FSetButtonEsc);	// tla��tko pro Esc

	case IDF_CODEPAGE:
		return CompNumPar(index, FSetCodePage);		// nastaven� znakov� sady

	case IDF_USERFONT:
		return CompTxtPar(index, FSetUserFont);		// nastaven� u�ivatelsk�ho fontu

	case IDF_LANGUAGE:
		return CompNumPar(index, FSetLanguage);		// nastaven� jazyku

	case IDF_WINDOW_FRAME:
		return CompLogPar(index, FSetWindowFrame);	// nastaven� r�me�ku okna

	case IDF_WINDOW_CAPTION:
		return CompLogPar(index, FSetWindowCaption); // nastaven� nadpisu okna

	case IDF_WINDOW_TOP:
		return CompLogPar(index, FSetWindowTop);	// nastaven� okna v�dy naho�e

	case IDF_WINDOW_RESIZABLE:
		return CompLogPar(index, FSetWindowSize);	// nastaven� okna s m�nitelnou velikost�

	case IDF_WINDOW_MAY_CLOSE:
		return CompLogPar(index, FSetWindowMayClose);	// nastaven� povolen� uzav�en� okna

	case IDF_WINDOW_MAXIM:
		return CompLogPar(index, FSetWindowMaxim);		// nastaven� maximalizace okna

	case IDF_WINDOW_MINIM:
		return CompLogPar(index, FSetWindowMinim);		// nastaven� minimalizace okna

	case IDF_WINDOW_POS:
		return CompNumPar(index, FSetWindowPos);	// nastaven� ukazatele v prvku

	case IDF_WINDOW_POS2:
		return CompNumPar(index, FSetWindowPos2);	// nastaven� ukazatele 2 v prvku

	case IDF_ALIAS:
		return CompTxtPar(index, FSetAlias);		// nastaven� ALIAS souboru

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_COM_ID:
		return CompNumPar(index, FSetComID);		// aktivn� port

	case IDF_COM_CLOSE:
		return CompNumPar(index, FComClose);		// uzav�en� portu

	case IDF_COM_BAUD:
		return CompNumPar(index, FSetComBaud);		// nastaven� p�enosov� rychlosti portu

	case IDF_COM_STOP:
		return CompLogPar(index, FSetComStop);		// nastaven� stopbit� portu

	case IDF_COM_BITS:
		return CompNumPar(index, FSetComBits);		// nastaven� po�tu bit� portu

	case IDF_COM_PARIT:
		return CompNumPar(index, FSetComParit);		// nastaven� parity

	case IDF_COM_TEXT:
		return CompTxtPar(index, FSetComText);		// vysl�n� textu na port

	case IDF_COM_NUM:
		return CompNumPar(index, FSetComNum);		// vysl�n� ��sla na port

	case IDF_COM_SEND:
		return CompLogPar(index, FSetComSend);		// povolen� vys�l�n�

	case IDF_COM_RECEIVE:
		return CompLogPar(index, FSetComReceive);	// povolen� p��jmu

	case IDF_DDE_APP:
		return CompTxtPar(index, FSetDDEApp);		// nastaven� jm�na plikace DDE

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------

	case IDF_GRAPH_AREA_WIDTH:
		return CompNumPar(index, FSetGraphAreaWidth);	// ���ka k zobrazen� centrovan�ho textu

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_MIX_DEVICE:
		return CompTxtPar(index, FSetMixDevice);	// nastaven� mix�n�ho za��zen�

	case IDF_MIX_LINE_DST:
		return CompTxtPar(index, FSetMixLineDst);	// nastaven� c�lov�ho sign�lu

	case IDF_MIX_LINE_SRC:
		return CompTxtPar(index, FSetMixLineSrc);	// nastaven� zdrojov�ho sign�lu

	case IDF_MIX_LINE_TYPE:							// nastaven� typu sign�lu
		return CompNumPar(index, FSetLineType);

	case IDF_MIX_CONTROL:
		return CompTxtPar(index, FSetMixControl);	// nastaven� ovl�dac�ho prvku

	case IDF_MIX_CONTROL_TYPE:						// nastaven� typu ovl�dac�ho prvku
		return CompNumPar(index, FSetCtrlType);

	case IDF_MIX_CONTROL_VAL:						// nastaven� hodnoty ovl�dac�ho prvku
		return CompNumPar(index, FSetCtrlVal);

	case IDF_MIX_CHANNEL:							// nastaven� kan�lu ovl�dac�ho prvku
		return CompNumPar(index, FSetMixChannel);

	case IDF_MIX_CONTROL_LIST:						// v�b�r hodnoty ovl�dac�ho prvku
		return CompTxtPar(index, FSetMixCtrlList);

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------


	case IDF_DIALOG_GRAPHIC:
		return CompLogPar(index, FSetDialogGraph);	// nadstaven� grafick�ho pozad�

	case IDF_DIRECT3D_MODE_LIST:
		return CompTxtPar(index, FSet3DMode);		// nastaven� videom�du

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_DIRECT3D_VIEW:
		CompAddItem(FDirect3DView);					// nastaven� okna Direct3D
		CompNumSubPar(index, IDF_DIRECT3D_VIEW_X, -100);
		CompNumSubPar(index, IDF_DIRECT3D_VIEW_Y, -100);
		CompNumSubPar(index, IDF_DIRECT3D_VIEW_W, 2000000);
		CompNumSubPar(index, IDF_DIRECT3D_VIEW_H, 2000000);
		return true;

	case IDF_DIRECT3D_WIREFRAME:
		return CompLogPar(index, FSetD3DWireframe);	// zapnut� v�pln� 3D

	case IDF_DIRECT3D_LIGHTON:
		return CompLogPar(index, FSetD3DLighton);	// zapnuto osv�tlen�

	case IDF_DIRECT3D_SHADES:
		return CompLogPar(index, FSetD3DShades);	// nastaven� m�du st�nov�n� 3D

	case IDF_DIRECT3D_INVERT:
		CompAddItem(FD3DInvert);					// inverze objektu
		return true;

	case IDF_DIRECT3D_OCULLING:
		return CompNumPar(index, FSetD3DOCulling);	// nastaven� odstra�ov�n� ploch objektu Direct 3D

	case IDF_DIRECT3D_MINFILTER:
		return CompLogPar(index, FSetD3DMinFilter);		// zapnut� filtrace p�i zmen�en�

	case IDF_DIRECT3D_MAGFILTER:
		return CompLogPar(index, FSetD3DMagFilter);		// zapnut� filtrace p�i zv�t�en�

	case IDF_DIRECT3D_MIPFILTER:
		return CompLogPar(index, FSetD3DMipFilter);		// zapnut� filtrace p�i vzd�len�

	case IDF_DIRECT3D_OWIREFRAME:
		return CompLogPar(index, FSetD3DOWireframe);	// zapnut� v�pln� objektu 3D

	case IDF_DIRECT3D_OLIGHTON:
		return CompLogPar(index, FSetD3DOLighton);	// zapnuto osv�tlen� objektu

	case IDF_DIRECT3D_OSHADES:
		return CompLogPar(index, FSetD3DOShades);	// nastaven� m�du st�nov�n� objektu 3D

	case IDF_DIRECT3D_OMINFILTER:
		return CompLogPar(index, FSetD3DOMinFilter);		// zapnut� filtrace p�i zmen�en� textury objektu

	case IDF_DIRECT3D_OMAGFILTER:
		return CompLogPar(index, FSetD3DOMagFilter);		// zapnut� filtrace p�i zv�t�en� textury objektu

	case IDF_DIRECT3D_OMIPFILTER:
		return CompLogPar(index, FSetD3DOMipFilter);		// zapnut� filtrace p�i vzd�len� textury objektu

	case IDF_DIRECT3D_VISIBLE:						// nastaven� viditelnosti objektu Direct 3D
		return CompLogPar(index, FSetD3DVisible);

	case IDF_DIRECT3D_LINK:							// p�ipojen� jin�ho objektu/objekt�
		return CompNumGrp(index, FD3DLink, FD3DLink);

//	case IDF_DIRECT3D_SAVE:							// ulo�en� objektu do souboru
//		return CompTxtPar(index, FD3DSave);

	case IDF_DIRECT3D_OBJECT_ID:
		return CompNumPar(index, FSetD3DObjectID);	// nastaven� aktivn�ho objektu Direct3D

	case IDF_DIRECT3D_OBJECT_DELETE:
		return CompNumGrp(index, FD3DObjectDelete, FD3DObjectDelete); // zru�en� objektu/objekt� Direct3D

	case IDF_DIRECT3D_SCALEX:
		return CompNumPar(index, FSetD3DScaleX);	// nastaven� m���tka ve sm�ru X

	case IDF_DIRECT3D_SCALEY:
		return CompNumPar(index, FSetD3DScaleY);	// nastaven� m���tka ve sm�ru Y

	case IDF_DIRECT3D_SCALEZ:
		return CompNumPar(index, FSetD3DScaleZ);	// nastaven� m���tka ve sm�ru Z

	case IDF_DIRECT3D_ROTATEX:
		return CompNumPar(index, FSetD3DRotateX);	// nastaven� rotace podle osy X

	case IDF_DIRECT3D_ROTATEY:
		return CompNumPar(index, FSetD3DRotateY);	// nastaven� rotace podle osy Y

	case IDF_DIRECT3D_ROTATEZ:
		return CompNumPar(index, FSetD3DRotateZ);	// nastaven� rotace podle osy Z

	case IDF_DIRECT3D_TRANSX:
		return CompNumPar(index, FSetD3DTransX);	// nastaven� posunu ve sm�ru X

	case IDF_DIRECT3D_TRANSY:
		return CompNumPar(index, FSetD3DTransY);	// nastaven� posunu ve sm�ru Y

	case IDF_DIRECT3D_TRANSZ:
		return CompNumPar(index, FSetD3DTransZ);	// nastaven� posunu ve sm�ru Z

	case IDF_DIRECT3D_COLOR:
		return CompNumPar(index, FSetD3DColor);		// nastaven� barvy objektu

	case IDF_DIRECT3D_EMISSIVE:
		return CompNumPar(index, FSetD3DEmissive);	// nastaven� barvy sv�tivosti

	case IDF_DIRECT3D_POWER:
		return CompNumPar(index, FSetD3DPower);		// nastaven� matnosti

	case IDF_DIRECT3D_SPECULAR:
		return CompNumPar(index, FSetD3DSpecular);	// nastaven� barvy odlesku

	case IDF_DIRECT3D_TEXTURE:						// nastaven� textury objektu Direct3D
		return CompPicPar(index, FSetD3DTexture);

	case IDF_DIRECT3D_TEXTURE_FILE:					// na�ten� textury objektu Direct3D ze souboru
		CompAddItem(FD3DTextureFile);
		return true;

	case IDF_DIRECT3D_SMOOTH:						// nastaven� vyhlazen� textur
		return CompNumPar(index, FSetD3DSmooth);

	case IDF_DIRECT3D_TEXTMAP:						// mapov�n� textury
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
		return CompNumPar(index, FSetD3DTransparent);	// nastaven� pr�hlednosti

	case IDF_DIRECT3D_ALPHAREF:
		return CompNumPar(index, FSetD3DAlphaRef);		// nastaven� referen�n� �rovn� alfa kan�lu

	case IDF_DIRECT3D_LIGHT_COLOR:
		return CompNumPar(index, FSetD3DLightColor);	// nastaven� barvy sv�tla Direct3D

	case IDF_DIRECT3D_LIGHT_TYPE:
		return CompNumPar(index, FSetD3DLightType);		// nastaven� typu sv�tla Direct3D

	case IDF_DIRECT3D_LIGHT_UMBRA:
		return CompNumPar(index, FSetD3DLightUmbra);	// nastaven� �hlu st�nu sv�tla Direct3D

	case IDF_DIRECT3D_LIGHT_PENUMBRA:
		return CompNumPar(index, FSetD3DLightPenumbra);	// nastaven� �hlu polost�nu sv�tla Direct3D

	case IDF_DIRECT3D_LIGHT_RANGE:
		return CompNumPar(index, FSetD3DLightRange);	// nastaven� dosahu sv�tla Direct3D

	case IDF_DIRECT3D_LIGHT_CATTEN:
		return CompNumPar(index, FSetD3DLightCatten);	// nastaven� konstantn�ho �tlumu sv�tla Direct3D

	case IDF_DIRECT3D_LIGHT_LATTEN:
		return CompNumPar(index, FSetD3DLightLatten);	// nastaven� line�rn�ho �tlumu sv�tla Direct3D

	case IDF_DIRECT3D_LIGHT_QATTEN:
		return CompNumPar(index, FSetD3DLightQatten);	// nastaven� kvadratick�ho �tlumu sv�tla Direct3D

	case IDF_DIRECT3D_PROJECTION:						// typ projekce
		return CompNumPar(index, FSetD3DProjection);

	case IDF_DIRECT3D_SCENE_COLOR:
		return CompNumPar(index, FSetD3DSceneColor);	// nastaven� barvy pozad� sc�ny

	case IDF_DIRECT3D_SCENE_TEXTURE:
		return CompPicPar(index, FSetD3DSceneTexture);	// nastaven� textury pozad� sc�ny

	case IDF_DIRECT3D_FOG_COLOR:
		return CompNumPar(index, FSetD3DFogColor);		// nastaven� barvy mlhy

	case IDF_DIRECT3D_FOG_MODE:
		return CompNumPar(index, FSetD3DFogMode);		// nastaven� m�du mlhy

	case IDF_DIRECT3D_FOG_START:
		return CompNumPar(index, FSetD3DFogStart);		// nastaven� za��tku mlhy

	case IDF_DIRECT3D_FOG_END:
		return CompNumPar(index, FSetD3DFogEnd);		// nastaven� konce mlhy

	case IDF_DIRECT3D_FOG_DENSITY:
		return CompNumPar(index, FSetD3DFogDensity);	// nastaven� hustoty mlhy

	case IDF_DIRECT3D_VIEW_FRONT:
		return CompNumPar(index, FSetD3DViewFront);		// nastaven� prom�tac� roviny

	case IDF_DIRECT3D_VIEW_BACK:
		return CompNumPar(index, FSetD3DViewBack);		// nastaven� vzd�lenosti dohledu

	case IDF_DIRECT3D_UPPER:
		return CompLogPar(index, FSetD3DUpper);			// vytv��et horn� podstavu

	case IDF_DIRECT3D_LOWER:
		return CompLogPar(index, FSetD3DLower);			// vytv��et doln� podstavu

	case IDF_DIRECT3D_SPLITS:
		return CompNumPar(index, FSetD3DSplits);		// nastaven� slo�itosti objektu

	case IDF_DIRECT3D_ORDER:
		return CompNumPar(index, FSetD3DOrder);			// nastaven� po�ad� rotac�

	case IDF_DIRECT3D_DRIVER:
		return CompNumPar(index, FSetD3DDriver);		// nastaven� ovlada�e

	case IDF_DIRECT3D_INTERFACE:
		return CompNumPar(index, FSetD3DInterface);		// nastaven� rozhran�

	case IDF_DIRECT3D_AMBIENT:							// nastaven� ambient sv�tla
		return CompNumPar(index, FSetD3DAmbient);

	case IDF_DIRECT3D_NORM:
		CompAddItem(FD3DNorm);							// normalizace objektu
		return true;

	case IDF_DIRECT3D_AMBIREF:
		return CompNumPar(index, FSetD3DAmbiRef);		// nastaven� odrazivosti rozpt�len�ho sv�tla

	case IDF_DIRECT3D_STAGE:							// stupe� textur
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

	case IDF_DIRECT3D_ADDRESSU:							// horizont�ln� adresov�n�
		return CompNumPar(index, FSetD3DAddressU);

	case IDF_DIRECT3D_ADDRESSV:							// vertik�ln� adresov�n�
		return CompNumPar(index, FSetD3DAddressV);

//	case IDF_DIRECT3D_BORDER:							// barva okol� textur
//		return CompNumPar(index, FSetD3DBorder);

	case IDF_DIRECT3D_BIAS:								// zjemn�n� vzd�len�ch textur
		return CompNumPar(index, FSetD3DBias);

	case IDF_DIRECT3D_TFACTOR:							// faktor textur
		return CompNumPar(index, FSetD3DTFactor);

	case IDF_DIRECT3D_MIPMAPS:							// po�et mipmaps
		return CompNumPar(index, FSetD3DMipMaps);

	case IDF_DIRECT3D_ZGROUP:							// renderovac� skupina
		return CompNumPar(index, FSetD3DZGroup);

	case IDF_DIRECT3D_ZSORT:							// t��d�n� skupiny
		return CompLogPar(index, FSetD3DZSort);

	case IDF_DIRECT3D_ZWRITE:							// z�pis hloubkov� informace
		return CompLogPar(index, FSetD3DZWrite);

	case IDF_DIRECT3D_ZTEST:							// hloubkov� test
		return CompNumPar(index, FSetD3DZTest);

	case IDF_DIRECT3D_NORMALS:							// generov�n� norm�l
		if (!CompNumPar(index, FD3DNormals))
		{
			CompAddItem(FD3DNormals0);
		}
		return true;

	case IDF_DIRECT3D_RENDER:							// renderov�n� bez zobrazen�
		CompAddItem(FD3DRender);
		return true;

	case IDF_SHADOW_RANGE:								// dosah st�nu
		return CompNumPar(index, FSetD3DShadowRange);

	case IDF_DIRECT3D_LOD_OBJECT:						// objekt pro LOD
		return CompNumPar(index, FSetD3DLODObject);

	case IDF_DIRECT3D_LOD_DISTANCE:						// vzd�lenost pro LOD
		return CompNumPar(index, FSetD3DLODDistance);

	case IDF_DIRECT3D_MORPH_MODEL:						// p�id�n� vzoru morfov�n�
		return CompNumPar(index, FSetD3DMorphModel);

	case IDF_DIRECT3D_MORPH_LEVEL:						// nastaven� stupn� morfov�n�
		return CompNumPar(index, FSetD3DMorphLevel);

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------
	 

	case IDF_ACTIVE:									// nastaven� aktivn� aplikace
		return CompLogPar(index, FSetActive);

	case IDF_TABLE_WIDTH:								// nastaven� ���ky sloupce tabulky
		return CompNumPar(index, FSetTableWidth);

	case IDF_TABLE_ALIGN:								// nastaven� zarovn�n� sloupce tabulky
		return CompNumPar(index, FSetTableAlign);

	case IDF_PRINT_PICTURE:
		return CompPicPar(index, FPicPrint2);			// tisk obr�zku

	default:
		return false;
	}
}


/////////////////////////////////////////////////////////////////////////////
// p�eklad p��kazu s podparametrem p��kaz� (hledan�m podle identifikace), Jump = p�esko�en� skupiny

void CompComSubPar(int index, int idf)
{
// korekce identifikace funkce
	idf -= IDF;

// ukazatel v�choz�ho prvku
	PETPROG*	item = BufEdi + index;
	PETPROG2*	item2 = BufEdi2 + index;
	int zahlavi = ProgNum;

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
				CompComGrp(index, FCommandExec, FCommandExec1))
			{
				ProgBuf[zahlavi].Jump = ProgNum - zahlavi;
				return;
			}

// posun pro p��t� prvek
			posun = item2->Items;

// dokud je dal�� potomek
		} while (item->Param & PETPROG_NEXT);
	}

// pou�ije se implicitn� hodnota - pr�zdn� p��kaz
	CompAddItem(FCommandExec0);
	ProgBuf[zahlavi].Jump = ProgNum - zahlavi;
}
