
#include "Main.h"

/***************************************************************************\
*																			*
*						Kompilace programu - text							*
*																			*
\***************************************************************************/

#pragma optimize("s", on)			// optimalizace na minimální velikost


/////////////////////////////////////////////////////////////////////////////
// pøeklad textového výrazu (vrací true = operace OK)

bool _fastcall CompTxt(int index)
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
	case IDF_CAPTION:
		CompAddItem(FGetCaption);		// naètení nadpisu okna
		return true;

	case IDF_STATUS_LINE:
		CompAddItem(FGetStatus);		// naètení textu stavového øádku
		return true;

	case IDF_TEXT_CONST:
		CompAddItem(FTextObj, Text.Dup(item->Name)); // textová konstanta
		return true;

//	case IDF_TEXT_LINE:
//		{
//			int i = Text.Dup(item->Name);
//			Text[i].Add(Textcrlf, 2);
//			CompAddItem(FTextObj, i);	 // textová konstanta
//		}
//		return true;
		
	case IDF_TEXT_CRLF:
		CompAddItem(FTextCRLF);				// text CR/LF
		return true;

	case IDF_FNC:
		return CompFunc(index, IDF_TEXT);	// funkce s návratem textu

	case IDF_TEXT:
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			item = BufObj + refinx;
			item2 = BufObj2 + refinx;
			if (Text.IsNotValid(item2->Data)) return false;

			if (item2->List < 0)
			{
				CompAddItem(FTextObj, item2->Data);
			}
			else
			{
				CompAddItem(FTextObjList, item2->Data, item2->List);
			}
			return true;
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			item = BufLoc + refinx;
			item2 = BufLoc2 + refinx;
			if (Text.IsNotValid(item2->Data)) return false;

			if (item2->List < 0)
			{
				CompAddItem(FTextLoc, item2->Local + KorigTxt);
			}
			else
			{
				CompAddItem(FTextLocList, item2->Local + KorigTxt, item2->LocalList + KorigLst);
			}
			return true;
		}
		return false;

	case IDF_TEXT_LEFT:
		CompAddItem(FTextLeft);				// levá èást textu
		CompNumSubPar(index, IDF_TEXT_LEFT_LEN, 1);
		CompTxtSubPar(index, IDF_TEXT_INPUT);
		return true;

	case IDF_TEXT_RIGHT:
		CompAddItem(FTextRight);				// pravá èást textu
		CompNumSubPar(index, IDF_TEXT_RIGHT_POS, 0);
		CompTxtSubPar(index, IDF_TEXT_INPUT);
		return true;

	case IDF_TEXT_MID:
		CompAddItem(FTextMid);				// støední èást textu
		CompNumSubPar(index, IDF_TEXT_MID_POS, 0);
		CompNumSubPar(index, IDF_TEXT_MID_LEN, 1);
		CompTxtSubPar(index, IDF_TEXT_INPUT);
		return true;

	case IDF_TEXTLINE_GET:
		CompAddItem(FTextlineGet);			// øádek textu
		CompNumSubPar(index, IDF_TEXTLINE_INDEX, 0);
		CompTxtSubPar(index, IDF_TEXT_INPUT);
		return true;

	case IDF_TEXT_ADD:
		return CompTxtGrp(index, FTextAdd, FTextAdd1);	// souèet textù

	case IDF_DIALOG_TEXT:
		CompAddItem(FDialogText);				// textový dialog
		CompTxtSubPar(index, IDF_DIALOG_BOX_CAPTION);
		CompTxtSubPar(index, IDF_DIALOG_TEXT1);
		CompTxtSubPar(index, IDF_DIALOG_TEXT2);
		CompTxtSubPar(index, IDF_DIALOG_TEXT3);
		CompTxtSubPar(index, IDF_DIALOG_TEXT_PAR);
		return true;

	case IDF_DIALOG_OPEN:
		CompAddItem(FDialogOpen);				// dialog otevøení souboru
		CompTxtSubPar(index, IDF_DIALOG_BOX_CAPTION);
		CompTxtSubPar(index, IDF_DIALOG_FILES);
		CompTxtSubPar(index, IDF_DIALOG_EXT);
		CompTxtSubPar(index, IDF_DIALOG_NAME);
		return true;

	case IDF_DIALOG_SAVE:
		CompAddItem(FDialogSave);				// dialog uložení souboru
		CompTxtSubPar(index, IDF_DIALOG_BOX_CAPTION);
		CompTxtSubPar(index, IDF_DIALOG_FILES);
		CompTxtSubPar(index, IDF_DIALOG_EXT);
		CompTxtSubPar(index, IDF_DIALOG_NAME);
		return true;

	case IDF_DIALOG_FIND:
		CompAddItem(FDialogFind);				// dialog nalezení textu
		return true;

	case IDF_DIALOG_COLOR:
		CompAddItem(FDialogColor);				// dialog výbìru barvy
		return true;

	case IDF_TEXT_FORMAT:
		return CompNumPar(index, FTextFormat); // pøevod èísla na text

	case IDF_TEXT_FORMAT2:
		return CompNumPar(index, FTextFormat2); // pøevod èísla na text 2 èíslice

	case IDF_TEXT_FORMATH:
		return CompNumPar(index, FTextFormatH); // pøevod èísla na HEX text

	case IDF_TEXT_LOWER:
		return CompTxtPar(index, FTextLower); // pøevod na malá písmena

	case IDF_TEXT_UPPER:
		return CompTxtPar(index, FTextUpper); // pøevod na velká písmena

	case IDF_CHAR:
		return CompNumPar(index, FTextChar); // pøevod èísla na znak

	case IDF_KLAV_CHAR_WAIT:
		CompAddItem(FKeyCharWait);		// naètení znaku z klávesnice
		return true;

	case IDF_KLAV_CHAR:
		CompAddItem(FKeyChar);			// vstup znaku z klávesnice
		return true;

	case IDF_FILE_ACT:
		CompAddItem(FGetFileAct);		// aktivní adresáø
		return true;

	case IDF_FILE_DISKLABEL:
		CompAddItem(FGetDiskLabel);		// jméno disku
		return true;

	case IDF_FILE_READ:
		CompAddItem(FGetFileRead);		// jméno souboru pro ètení
		return true;

	case IDF_FILE_WRITE:
		CompAddItem(FGetFileWrite);		// jméno souboru pro zápisu
		return true;

	case IDF_FILE_TEXT0:
		CompAddItem(FGetFileText0);		// naètení textu zakonèeného nulou
		return true;

	case IDF_FILE_TEXTCR:
		CompAddItem(FGetFileTextCR);	// naètení textu zakonèeného s CR
		return true;

	case IDF_FILE_TEXTN:
		CompAddItem(FGetFileTextN);		// naètení textu s pevnou délkou
		return true;

	case IDF_DATETIME_DATEEUR:
		return CompNumPar(index, FTimeDateEur);	// pøevod data na evropský formát

	case IDF_DATETIME_DATEUSA:
		return CompNumPar(index, FTimeDateUsa);	// pøevod data na americký formát

	case IDF_DATETIME_DATEJAP:
		return CompNumPar(index, FTimeDateJap);	// pøevod data na japonský formát

	case IDF_DATETIME_TIME24:
		return CompNumPar(index, FTimeTime24);	// pøevod èasu na formát 24

	case IDF_DATETIME_TIME12:
		return CompNumPar(index, FTimeTime12);	// pøevod èasu na formát 12

	case IDF_CONSOLE_IN:
		CompAddItem(FConsoleIn);				// vstup textu z konzoly
		return true;

	case IDF_COMMAND_LINE:
		CompAddItem(FGetCommandLine);			// pøíkazový øádek
		return true;

	case IDF_COMMAND_PAR:
		return CompNumPar(index, FCommandPar);	// parametr programu

	case IDF_EXEC_NAME:
		CompAddItem(FExecName);					// jméno programu
		return true;

	case IDF_WINDOW_TEXT:
		CompAddItem(FGetWindowText);			// text okna
		return true;

	case IDF_FILE_LIST:
		if (!CompTxtPar(index, FFileList))		// seznam souborù
		{
			CompAddItem(FFileListAll);
		}
		return true;


// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_CONNECT_LIST:
		CompAddItem(FConnectList);				// seznam spojení DirectPlay
		return true;

	case IDF_CONNECT_ACT:
		CompAddItem(FGetConnectAct);			// aktivní spojení DirectPlay
		return true;

	case IDF_GAME_LIST:
		CompAddItem(FGameList);					// seznam her DirectPlay
		return true;

	case IDF_GAME_ACT:
		CompAddItem(FGetGameAct);				// aktivní hra DirectPlay
		return true;

	case IDF_PLAYER_LIST:
		CompAddItem(FPlayerList);				// seznam hráèù DirectPlay
		return true;

	case IDF_PLAYER_ACT:
		CompAddItem(FGetPlayerAct);				// aktivní hráè DirectPlay
		return true;

	case IDF_GAME_DATAT:
		CompAddItem(FGetGameDataT);				// naètení textu z paketu
		return true;

	case IDF_GAME_IP:
		CompAddItem(FGetGameIP);				// zjištìní IP poèítaèe
		return true;

	case IDF_DDE_APP:
		CompAddItem(FGetDDEApp);				// zjištìní seznamu aplikací
		return true;

	case IDF_DDE_TOPIC:
		CompAddItem(FGetDDETopic);				// zjištìní seznamu témat
		return true;

	case IDF_UDP_SEND_IP:
		CompAddItem(FGetUDPSendIP);				// zjištìní vysílací IP adresy UDP
		return true;

	case IDF_UDP_REC_IP:
		CompAddItem(FGetUDPRecIP);				// zjištìní pøijaté IP adresy UDP
		return true;

	case IDF_UDP_TEXT:
		CompAddItem(FGetUDPText);				// pøíjem textu UDP
		return true;

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------

	case IDF_USERFONT:
		CompAddItem(FGetUserFont);				// zjištìní uživatelského fontu
		return true;

	case IDF_ENUMFONTS:
		CompAddItem(FEnumFonts);				// zjištìní seznamu fontù
		return true;

	case IDF_ALIAS:
		CompAddItem(FGetAlias);					// naètení alias souboru
		return true;

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_MEMORY_TEXT0:							// text zakonèený nulou
		CompAddItem(FGetMemoryText0);
		return true;

	case IDF_MEMORY_TEXT0P:							// ukazatel na text zakonèený nulou
		CompAddItem(FGetMemoryText0P);
		return true;

	case IDF_MEMORY_TEXTN:							// text s pevnou délkou
		CompAddItem(FGetMemoryTextN);
		return true;

	case IDF_INI_FILE:								// jméno konfiguraèního souboru
		CompAddItem(FGetIniFile);
		return true;

	case IDF_INI_SECTION:							// jméno konfiguraèní sekce
		CompAddItem(FGetIniSection);
		return true;

	case IDF_INI_VALUE:								// jméno konfiguraèního parametru
		CompAddItem(FGetIniValue);
		return true;

	case IDF_INI_STRING:							// ètení konfiguraèního textu
		if (!CompTxtPar(index, FGetIniString))
		{
			CompAddItem(FGetIniString0);
		}
		return true;

	case IDF_REG_SUBKEY:							// klíè registrù
		CompAddItem(FGetRegSubkey);
		return true;

	case IDF_REG_VALUE:								// jméno položky registru
		CompAddItem(FGetRegValue);
		return true;

	case IDF_REG_STRING:							// ètení textu z registru
		if (!CompTxtPar(index, FGetRegString))
		{
			CompAddItem(FGetRegString0);
		}
		return true;

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------


// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_COM_TEXT:
		CompAddItem(FGetComText);				// naètení znaku z portu
		return true;

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_MIX_DEVICE:
		CompAddItem(FGetMixDevice);				// zjištìní seznamu mixážních zaøízení
		return true;

	case IDF_MIX_LINE_DST:
		CompAddItem(FGetMixLineDst);			// zjištìní seznamu cílových signálù
		return true;

	case IDF_MIX_LINE_SRC:
		CompAddItem(FGetMixLineSrc);			// zjištìní seznamu zdrojových signálù
		return true;

	case IDF_MIX_CONTROL:
		CompAddItem(FGetMixControl);			// zjištìní seznamu ovládacích prvkù
		return true;

	case IDF_MIX_CONTROL_LIST:
		CompAddItem(FGetMixCtrlList);			// zjištìní seznamu hodnot ovládacích prvkù
		return true;
		
#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------

	case IDF_DIRECT3D_MODE_LIST:
		CompAddItem(FGet3DMode);				// zjištìní seznamu videomódù
		return true;

	case IDF_TEXT_BLOCK:						// blok textu
		CompAddItem(FTextBlock);
		CompNumSubPar(index, IDF_TEXT_BLOCK_BEGLINE, 0);
		CompNumSubPar(index, IDF_TEXT_BLOCK_BEGPOS, 0);
		CompNumSubPar(index, IDF_TEXT_BLOCK_ENDLINE, -1);
		CompNumSubPar(index, IDF_TEXT_BLOCK_ENDPOS, -1);
		CompTxtSubPar(index, IDF_TEXT_INPUT);
		return true;

	default:
		return false;
	}
}


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu s textovým parametrem

bool CompTxtPar(int index, PROCCOM func)
{
	return CompTxtPar(index, func, 0, -1);
}

bool CompTxtPar(int index, PROCCOM func, int data)
{
	return CompTxtPar(index, func, data, -1);
}

bool CompTxtPar(int index, PROCCOM func, int data, int list)
{
	int zahlavi = CompAddItem(func, data, list);

	if ((BufEdi2[index].Items == 1) || !CompTxt(index+1))
	{
		ProgNum = zahlavi;
		return false;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu s textovým podparametrem (hledaným podle identifikace)

void CompTxtSubPar(int index, int idf)
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
				CompTxt(index + 1))
			{
				return;
			}

// posun pro pøíští prvek
			posun = item2->Items;

// dokud je další potomek
		} while (item->Param & PETPROG_NEXT);
	}

// použije se implicitní hodnota - prázdný text
	CompAddItem(FTextEmpty);
}
