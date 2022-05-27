
#include "Main.h"

/***************************************************************************\
*																			*
*						Kompilace programu - text							*
*																			*
\***************************************************************************/

#pragma optimize("s", on)			// optimalizace na minim�ln� velikost


/////////////////////////////////////////////////////////////////////////////
// p�eklad textov�ho v�razu (vrac� true = operace OK)

bool _fastcall CompTxt(int index)
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
	case IDF_CAPTION:
		CompAddItem(FGetCaption);		// na�ten� nadpisu okna
		return true;

	case IDF_STATUS_LINE:
		CompAddItem(FGetStatus);		// na�ten� textu stavov�ho ��dku
		return true;

	case IDF_TEXT_CONST:
		CompAddItem(FTextObj, Text.Dup(item->Name)); // textov� konstanta
		return true;

//	case IDF_TEXT_LINE:
//		{
//			int i = Text.Dup(item->Name);
//			Text[i].Add(Textcrlf, 2);
//			CompAddItem(FTextObj, i);	 // textov� konstanta
//		}
//		return true;
		
	case IDF_TEXT_CRLF:
		CompAddItem(FTextCRLF);				// text CR/LF
		return true;

	case IDF_FNC:
		return CompFunc(index, IDF_TEXT);	// funkce s n�vratem textu

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
		CompAddItem(FTextLeft);				// lev� ��st textu
		CompNumSubPar(index, IDF_TEXT_LEFT_LEN, 1);
		CompTxtSubPar(index, IDF_TEXT_INPUT);
		return true;

	case IDF_TEXT_RIGHT:
		CompAddItem(FTextRight);				// prav� ��st textu
		CompNumSubPar(index, IDF_TEXT_RIGHT_POS, 0);
		CompTxtSubPar(index, IDF_TEXT_INPUT);
		return true;

	case IDF_TEXT_MID:
		CompAddItem(FTextMid);				// st�edn� ��st textu
		CompNumSubPar(index, IDF_TEXT_MID_POS, 0);
		CompNumSubPar(index, IDF_TEXT_MID_LEN, 1);
		CompTxtSubPar(index, IDF_TEXT_INPUT);
		return true;

	case IDF_TEXTLINE_GET:
		CompAddItem(FTextlineGet);			// ��dek textu
		CompNumSubPar(index, IDF_TEXTLINE_INDEX, 0);
		CompTxtSubPar(index, IDF_TEXT_INPUT);
		return true;

	case IDF_TEXT_ADD:
		return CompTxtGrp(index, FTextAdd, FTextAdd1);	// sou�et text�

	case IDF_DIALOG_TEXT:
		CompAddItem(FDialogText);				// textov� dialog
		CompTxtSubPar(index, IDF_DIALOG_BOX_CAPTION);
		CompTxtSubPar(index, IDF_DIALOG_TEXT1);
		CompTxtSubPar(index, IDF_DIALOG_TEXT2);
		CompTxtSubPar(index, IDF_DIALOG_TEXT3);
		CompTxtSubPar(index, IDF_DIALOG_TEXT_PAR);
		return true;

	case IDF_DIALOG_OPEN:
		CompAddItem(FDialogOpen);				// dialog otev�en� souboru
		CompTxtSubPar(index, IDF_DIALOG_BOX_CAPTION);
		CompTxtSubPar(index, IDF_DIALOG_FILES);
		CompTxtSubPar(index, IDF_DIALOG_EXT);
		CompTxtSubPar(index, IDF_DIALOG_NAME);
		return true;

	case IDF_DIALOG_SAVE:
		CompAddItem(FDialogSave);				// dialog ulo�en� souboru
		CompTxtSubPar(index, IDF_DIALOG_BOX_CAPTION);
		CompTxtSubPar(index, IDF_DIALOG_FILES);
		CompTxtSubPar(index, IDF_DIALOG_EXT);
		CompTxtSubPar(index, IDF_DIALOG_NAME);
		return true;

	case IDF_DIALOG_FIND:
		CompAddItem(FDialogFind);				// dialog nalezen� textu
		return true;

	case IDF_DIALOG_COLOR:
		CompAddItem(FDialogColor);				// dialog v�b�ru barvy
		return true;

	case IDF_TEXT_FORMAT:
		return CompNumPar(index, FTextFormat); // p�evod ��sla na text

	case IDF_TEXT_FORMAT2:
		return CompNumPar(index, FTextFormat2); // p�evod ��sla na text 2 ��slice

	case IDF_TEXT_FORMATH:
		return CompNumPar(index, FTextFormatH); // p�evod ��sla na HEX text

	case IDF_TEXT_LOWER:
		return CompTxtPar(index, FTextLower); // p�evod na mal� p�smena

	case IDF_TEXT_UPPER:
		return CompTxtPar(index, FTextUpper); // p�evod na velk� p�smena

	case IDF_CHAR:
		return CompNumPar(index, FTextChar); // p�evod ��sla na znak

	case IDF_KLAV_CHAR_WAIT:
		CompAddItem(FKeyCharWait);		// na�ten� znaku z kl�vesnice
		return true;

	case IDF_KLAV_CHAR:
		CompAddItem(FKeyChar);			// vstup znaku z kl�vesnice
		return true;

	case IDF_FILE_ACT:
		CompAddItem(FGetFileAct);		// aktivn� adres��
		return true;

	case IDF_FILE_DISKLABEL:
		CompAddItem(FGetDiskLabel);		// jm�no disku
		return true;

	case IDF_FILE_READ:
		CompAddItem(FGetFileRead);		// jm�no souboru pro �ten�
		return true;

	case IDF_FILE_WRITE:
		CompAddItem(FGetFileWrite);		// jm�no souboru pro z�pisu
		return true;

	case IDF_FILE_TEXT0:
		CompAddItem(FGetFileText0);		// na�ten� textu zakon�en�ho nulou
		return true;

	case IDF_FILE_TEXTCR:
		CompAddItem(FGetFileTextCR);	// na�ten� textu zakon�en�ho s CR
		return true;

	case IDF_FILE_TEXTN:
		CompAddItem(FGetFileTextN);		// na�ten� textu s pevnou d�lkou
		return true;

	case IDF_DATETIME_DATEEUR:
		return CompNumPar(index, FTimeDateEur);	// p�evod data na evropsk� form�t

	case IDF_DATETIME_DATEUSA:
		return CompNumPar(index, FTimeDateUsa);	// p�evod data na americk� form�t

	case IDF_DATETIME_DATEJAP:
		return CompNumPar(index, FTimeDateJap);	// p�evod data na japonsk� form�t

	case IDF_DATETIME_TIME24:
		return CompNumPar(index, FTimeTime24);	// p�evod �asu na form�t 24

	case IDF_DATETIME_TIME12:
		return CompNumPar(index, FTimeTime12);	// p�evod �asu na form�t 12

	case IDF_CONSOLE_IN:
		CompAddItem(FConsoleIn);				// vstup textu z konzoly
		return true;

	case IDF_COMMAND_LINE:
		CompAddItem(FGetCommandLine);			// p��kazov� ��dek
		return true;

	case IDF_COMMAND_PAR:
		return CompNumPar(index, FCommandPar);	// parametr programu

	case IDF_EXEC_NAME:
		CompAddItem(FExecName);					// jm�no programu
		return true;

	case IDF_WINDOW_TEXT:
		CompAddItem(FGetWindowText);			// text okna
		return true;

	case IDF_FILE_LIST:
		if (!CompTxtPar(index, FFileList))		// seznam soubor�
		{
			CompAddItem(FFileListAll);
		}
		return true;


// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_CONNECT_LIST:
		CompAddItem(FConnectList);				// seznam spojen� DirectPlay
		return true;

	case IDF_CONNECT_ACT:
		CompAddItem(FGetConnectAct);			// aktivn� spojen� DirectPlay
		return true;

	case IDF_GAME_LIST:
		CompAddItem(FGameList);					// seznam her DirectPlay
		return true;

	case IDF_GAME_ACT:
		CompAddItem(FGetGameAct);				// aktivn� hra DirectPlay
		return true;

	case IDF_PLAYER_LIST:
		CompAddItem(FPlayerList);				// seznam hr��� DirectPlay
		return true;

	case IDF_PLAYER_ACT:
		CompAddItem(FGetPlayerAct);				// aktivn� hr�� DirectPlay
		return true;

	case IDF_GAME_DATAT:
		CompAddItem(FGetGameDataT);				// na�ten� textu z paketu
		return true;

	case IDF_GAME_IP:
		CompAddItem(FGetGameIP);				// zji�t�n� IP po��ta�e
		return true;

	case IDF_DDE_APP:
		CompAddItem(FGetDDEApp);				// zji�t�n� seznamu aplikac�
		return true;

	case IDF_DDE_TOPIC:
		CompAddItem(FGetDDETopic);				// zji�t�n� seznamu t�mat
		return true;

	case IDF_UDP_SEND_IP:
		CompAddItem(FGetUDPSendIP);				// zji�t�n� vys�lac� IP adresy UDP
		return true;

	case IDF_UDP_REC_IP:
		CompAddItem(FGetUDPRecIP);				// zji�t�n� p�ijat� IP adresy UDP
		return true;

	case IDF_UDP_TEXT:
		CompAddItem(FGetUDPText);				// p��jem textu UDP
		return true;

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------

	case IDF_USERFONT:
		CompAddItem(FGetUserFont);				// zji�t�n� u�ivatelsk�ho fontu
		return true;

	case IDF_ENUMFONTS:
		CompAddItem(FEnumFonts);				// zji�t�n� seznamu font�
		return true;

	case IDF_ALIAS:
		CompAddItem(FGetAlias);					// na�ten� alias souboru
		return true;

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_MEMORY_TEXT0:							// text zakon�en� nulou
		CompAddItem(FGetMemoryText0);
		return true;

	case IDF_MEMORY_TEXT0P:							// ukazatel na text zakon�en� nulou
		CompAddItem(FGetMemoryText0P);
		return true;

	case IDF_MEMORY_TEXTN:							// text s pevnou d�lkou
		CompAddItem(FGetMemoryTextN);
		return true;

	case IDF_INI_FILE:								// jm�no konfigura�n�ho souboru
		CompAddItem(FGetIniFile);
		return true;

	case IDF_INI_SECTION:							// jm�no konfigura�n� sekce
		CompAddItem(FGetIniSection);
		return true;

	case IDF_INI_VALUE:								// jm�no konfigura�n�ho parametru
		CompAddItem(FGetIniValue);
		return true;

	case IDF_INI_STRING:							// �ten� konfigura�n�ho textu
		if (!CompTxtPar(index, FGetIniString))
		{
			CompAddItem(FGetIniString0);
		}
		return true;

	case IDF_REG_SUBKEY:							// kl�� registr�
		CompAddItem(FGetRegSubkey);
		return true;

	case IDF_REG_VALUE:								// jm�no polo�ky registru
		CompAddItem(FGetRegValue);
		return true;

	case IDF_REG_STRING:							// �ten� textu z registru
		if (!CompTxtPar(index, FGetRegString))
		{
			CompAddItem(FGetRegString0);
		}
		return true;

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------


// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_COM_TEXT:
		CompAddItem(FGetComText);				// na�ten� znaku z portu
		return true;

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_MIX_DEVICE:
		CompAddItem(FGetMixDevice);				// zji�t�n� seznamu mix�n�ch za��zen�
		return true;

	case IDF_MIX_LINE_DST:
		CompAddItem(FGetMixLineDst);			// zji�t�n� seznamu c�lov�ch sign�l�
		return true;

	case IDF_MIX_LINE_SRC:
		CompAddItem(FGetMixLineSrc);			// zji�t�n� seznamu zdrojov�ch sign�l�
		return true;

	case IDF_MIX_CONTROL:
		CompAddItem(FGetMixControl);			// zji�t�n� seznamu ovl�dac�ch prvk�
		return true;

	case IDF_MIX_CONTROL_LIST:
		CompAddItem(FGetMixCtrlList);			// zji�t�n� seznamu hodnot ovl�dac�ch prvk�
		return true;
		
#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------

	case IDF_DIRECT3D_MODE_LIST:
		CompAddItem(FGet3DMode);				// zji�t�n� seznamu videom�d�
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
// p�eklad p��kazu s textov�m parametrem

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
// p�eklad p��kazu s textov�m podparametrem (hledan�m podle identifikace)

void CompTxtSubPar(int index, int idf)
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
				CompTxt(index + 1))
			{
				return;
			}

// posun pro p��t� prvek
			posun = item2->Items;

// dokud je dal�� potomek
		} while (item->Param & PETPROG_NEXT);
	}

// pou�ije se implicitn� hodnota - pr�zdn� text
	CompAddItem(FTextEmpty);
}
