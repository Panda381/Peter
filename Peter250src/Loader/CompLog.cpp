
#include "Main.h"

/***************************************************************************\
*																			*
*					Kompilace programu - logický výraz						*
*																			*
\***************************************************************************/

#pragma optimize("s", on)			// optimalizace na minimální velikost


/////////////////////////////////////////////////////////////////////////////
// pøeklad logického výrazu (vrací true = operace OK)

bool _fastcall CompLog(int index)
{
// adresa zdrojového prvku
	if ((DWORD)index >= (DWORD)BufEdiN) return false;
	PETPROG*	item = BufEdi + index;
	PETPROG2*	item2 = BufEdi2 + index;
	int refinx = item->RefIndex;
	PETPROG2*	prg2;

// kontrola, zda je položka vypnuta
	if ((item->Param & (PETPROG_OFF | PETPROG_OFF_DEP)) != 0) return false;

// vìtvení podle funkce
	switch (item->Func + IDF)
	{
	case IDF_EQU:
		return CompNumGrp(index, FEqu, FEqu1);		// operátor EQU

	case IDF_NEQU:
		return CompNumGrp(index, FNEqu, FNEqu1);	// operátor NEQU

	case IDF_EQGR:
		return CompNumGrp(index, FEqGr, FEqGr1);	// operátor EQGR

	case IDF_EQLT:
		return CompNumGrp(index, FEqLt, FEqLt1);	// operátor EQLT

	case IDF_GR:
		return CompNumGrp(index, FGr, FGr1);		// operátor GR

	case IDF_LT:
		return CompNumGrp(index, FLt, FLt1);		// operátor LT

	case IDF_AND:
		return CompLogGrp(index, FAnd, FAnd1);		// operátor AND

	case IDF_OR:
		return CompLogGrp(index, FOr, FOr1);		// operátor OR

	case IDF_XOR:
		return CompLogGrp(index, FXor, FXor1);		// operátor XOR

	case IDF_NOT:
		return CompLogPar(index, FNot);				// operátor NOT

	case IDF_FNC:
		return CompFunc(index, IDF_BOOL);			// funkce s návratem logické hodnoty

	case IDF_BOOL:									// logická promìnná
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			item = BufObj + refinx;
			item2 = BufObj2 + refinx;
			if (Bool.IsNotValid(item2->Data)) return false;

			if (item2->List < 0)
			{
				CompAddItem(FBoolObj, item2->Data);
			}
			else
			{
				CompAddItem(FBoolObjList, item2->Data, item2->List);
			}
			return true;
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			item = BufLoc + refinx;
			item2 = BufLoc2 + refinx;
			if (Bool.IsNotValid(item2->Data)) return false;

			if (item2->List < 0)
			{
				CompAddItem(FBoolLoc, item2->Local + KorigLog);
			}
			else
			{
				CompAddItem(FBoolLocList, item2->Local + KorigLog, item2->LocalList + KorigLst);
			}
			return true;
		}
		return false;

	case IDF_TRUE:
		CompAddItem(FTrue);							// konstanta TRUE
		return true;

	case IDF_FALSE:
		CompAddItem(FFalse);						// konstanta FALSE
		return true;

	case IDF_RYCHLE:
		CompAddItem(FGetVisible);					// viditelnost Petra
		return true;

	case IDF_PETRA_RYCHLE:
		CompAddItem(FGetVisible2);					// viditelnost Petry
		return true;

	case IDF_KLAV_TEST:
		return CompNumPar(index, FKeyTest);			// test stisku klávesy

	case IDF_MOUSE_L:
		CompAddItem(FMouseL);						// test stisku levého tlaèítka myši
		return true;

	case IDF_MOUSE_R:
		CompAddItem(FMouseR);						// test stisku pravého tlaèítka myši
		return true;

	case IDF_EQU_TEXT:
		return CompTxtGrp(index, FCompEqTxt, FCompEqTxt1);	// shodnost textù

	case IDF_EQU_ICO:
		return CompIcoGrp(index, FCompEqIco, FCompEqIco1);	// shodnost pøedmìtù

	case IDF_SPRITE_MOVE:
		if (item->RefBlok == BufObjID)
		{
			if ((DWORD)refinx >= (DWORD)BufObjN) return false;
			prg2 = BufObj2 + refinx;
			if (Sprite.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				CompAddItem(FGetSpriteMove, prg2->Data);
			}
			else
			{
				CompAddItem(FGetSpriteMoveList, prg2->Data, prg2->List);
			}
			return true;
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			prg2 = BufLoc2 + refinx;
			if (Sprite.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				CompAddItem(FGetSpriteMoveLoc, prg2->Local + KorigSpr);
			}
			else
			{
				CompAddItem(FGetSpriteMoveLocList, prg2->Local + KorigSpr, prg2->LocalList + KorigLst);
			}
			return true;
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
				CompAddItem(FGetSpriteVis, prg2->Data);
			}
			else
			{
				CompAddItem(FGetSpriteVisList, prg2->Data, prg2->List);
			}
			return true;
		}

		if (item->RefBlok == BufLocID)
		{
			if ((DWORD)refinx >= (DWORD)BufLocN) return false;
			prg2 = BufLoc2 + refinx;
			if (Sprite.IsNotValid(prg2->Data)) return false;

			if (prg2->List < 0)
			{
				CompAddItem(FGetSpriteVisLoc, prg2->Local + KorigSpr);
			}
			else
			{
				CompAddItem(FGetSpriteVisLocList, prg2->Local + KorigSpr, prg2->LocalList + KorigLst);
			}
			return true;
		}
		return false;

	case IDF_PETER_SWC1:
		CompAddItem(FGetSwc1);						// naètení pøepínaèe 1 - Petr
		return true;

	case IDF_PETRA_SWC1:
		CompAddItem(FGetSwc12);						// naètení pøepínaèe 1 - Petra
		return true;

	case IDF_PETER_SWC2:
		CompAddItem(FGetSwc2);						// naètení pøepínaèe 2 - Petr
		return true;

	case IDF_PETRA_SWC2:
		CompAddItem(FGetSwc22);						// naètení pøepínaèe 2 - Petra
		return true;

	case IDF_PETER_SWC3:
		CompAddItem(FGetSwc3);						// naètení pøepínaèe 3 - Petr
		return true;

	case IDF_PETRA_SWC3:
		CompAddItem(FGetSwc32);						// naètení pøepínaèe 3 - Petra
		return true;

	case IDF_PETER_SWC4:
		CompAddItem(FGetSwc4);						// naètení pøepínaèe 4 - Petr
		return true;

	case IDF_PETRA_SWC4:
		CompAddItem(FGetSwc42);						// naètení pøepínaèe 4 - Petra
		return true;

	case IDF_PETER_SWC5:
		CompAddItem(FGetSwc5);						// naètení pøepínaèe 5 - Petr
		return true;

	case IDF_PETRA_SWC5:
		CompAddItem(FGetSwc52);						// naètení pøepínaèe 5 - Petra
		return true;

	case IDF_TEXT_GT:
		return CompTxtGrp(index, FCompGtTxt, FCompGtTxt1);	// abecední porovnání textù

	case IDF_STATUS_NUMLOCK:
		CompAddItem(FStatusNumLock);				// naètení stavu Num Lock
		return true;

	case IDF_STATUS_CAPSLOCK:
		CompAddItem(FStatusCapsLock);				// naètení stavu Caps Lock
		return true;

	case IDF_STATUS_SCROLLLOCK:
		CompAddItem(FStatusScrollLock);				// naètení stavu Scroll Lock
		return true;

	case IDF_STATUS_INSERT:
		CompAddItem(FStatusInsert);					// naètení stavu Insert
		return true;

	case IDF_MOUSE_LCLICK:
		CompAddItem(FMouseLClick);					// test kliku levým tlaèítkem myši
		return true;

	case IDF_MOUSE_RCLICK:
		CompAddItem(FMouseRClick);					// test kliku pravým tlaèítkem myši
		return true;

	case IDF_MOUSE_LDCLICK:
		CompAddItem(FMouseLDClick);					// test dvojkliku levým tlaèítkem myši
		return true;

	case IDF_MOUSE_RDCLICK:
		CompAddItem(FMouseRDClick);					// test dvojkliku pravým tlaèítkem myši
		return true;

	case IDF_MOUSE_REGION:
		CompAddItem(FMouseRegion);					// test oblasti myši
		CompNumSubPar(index, IDF_MOUSE_REGION_X, 0);
		CompNumSubPar(index, IDF_MOUSE_REGION_Y, 0);
		CompNumSubPar(index, IDF_MOUSE_REGION_W, 2000000);
		CompNumSubPar(index, IDF_MOUSE_REGION_H, 2000000);
		return true;

	case IDF_MOUSE_ITEM:
		return CompNumPar(index, FMouseItem);			// test myši v prvku

	case IDF_SOUND_TEST:
		if (!CompSndPar(index, FSoundTest1))		// test pøehrávání zvuku
		{
			CompAddItem(FSoundTest);
		}
		return true;

	case IDF_MUSIC_TEST:
		CompAddItem(FMusicTest);					// test pøehrávání hudby
		return true;

	case IDF_MUSIC_PAUSE:
		CompAddItem(FMusicTestPause);				// test pauzy hudby
		return true;

	case IDF_CD_PAUSE:
		CompAddItem(FCDGetPause);					// test pauzy CD
		return true;

	case IDF_CD_EJECT:
		CompAddItem(FCDGetEject);					// test zasunutí CD
		return true;

	case IDF_CD_TEST:
		CompAddItem(FCDTest);						// test pøehrávání CD
		return true;

	case IDF_FILE_FILENEW:
		return CompTxtPar(index, FBoolFileNew);		// vytvoøení souboru

	case IDF_FILE_DIRNEW:
		return CompTxtPar(index, FBoolDirNew);		// vytvoøení adresáøe

	case IDF_FILE_TEXT_UNICODE:						// text v kódu UNICODE
		CompAddItem(FGetFileTextUnicode);
		return true;

	case IDF_FILE_LOGIC:
		CompAddItem(FGetFileLogic);					// naètení logické hodnoty
		return true;

	case IDF_FILE_DELETE:
		return CompTxtPar(index, FBoolFileDelete);	// zrušení souboru

	case IDF_FILE_TEST:
		return CompTxtPar(index, FFileTest);		// test platnosti disku/souboru/adresáøe

	case IDF_FILE_COPY:
		CompAddItem(FBoolFileCopy);					// kopie souboru
		return true;

	case IDF_FILE_MOVE:
		CompAddItem(FBoolFileMove);					// pøesun souboru
		return true;

	case IDF_FILE_RO:
		CompAddItem(FGetFileRO);					// atribut R/O
		return true;

	case IDF_FILE_HID:
		CompAddItem(FGetFileHID);					// atribut HID
		return true;

	case IDF_FILE_SYS:
		CompAddItem(FGetFileSYS);					// atribut SYS
		return true;

	case IDF_FILE_DIR:
		CompAddItem(FGetFileDIR);					// atribut DIR
		return true;

	case IDF_FILE_ARC:
		CompAddItem(FGetFileARC);					// atribut ARC
		return true;

	case IDF_FILE_TMP:
		CompAddItem(FGetFileTMP);					// atribut TMP
		return true;

	case IDF_FILE_CMP:
		CompAddItem(FGetFileCMP);					// atribut CMP
		return true;

	case IDF_FILE_OFF:
		CompAddItem(FGetFileOFF);					// atribut OFF
		return true;

	case IDF_FILE_ERROR:
		CompAddItem(FGetFileError);					// pøíznak chyby souborù
		return true;

	case IDF_SOUND_STEREO:
		return CompSndPar(index, FGetSoundStereo);	// pøíznak stereo zvuku

	case IDF_MAP_SWC1:
		CompAddItem(FGetMapSwc1);					// pøepínaè 1 z plochy
		CompMapSubPar(index, IDF_MAP_INPUT);
		CompNumSubPar(index, IDF_MAP_X, 0);
		CompNumSubPar(index, IDF_MAP_Y, 0);
		return true;

	case IDF_MAP_SWC2:
		CompAddItem(FGetMapSwc2);					// pøepínaè 2 z plochy
		CompMapSubPar(index, IDF_MAP_INPUT);
		CompNumSubPar(index, IDF_MAP_X, 0);
		CompNumSubPar(index, IDF_MAP_Y, 0);
		return true;

	case IDF_MAP_SWC3:
		CompAddItem(FGetMapSwc3);					// pøepínaè 3 z plochy
		CompMapSubPar(index, IDF_MAP_INPUT);
		CompNumSubPar(index, IDF_MAP_X, 0);
		CompNumSubPar(index, IDF_MAP_Y, 0);
		return true;

	case IDF_MAP_SWC4:
		CompAddItem(FGetMapSwc4);					// pøepínaè 4 z plochy
		CompMapSubPar(index, IDF_MAP_INPUT);
		CompNumSubPar(index, IDF_MAP_X, 0);
		CompNumSubPar(index, IDF_MAP_Y, 0);
		return true;

	case IDF_MAP_SWC5:
		CompAddItem(FGetMapSwc5);					// pøepínaè 5 z plochy
		CompMapSubPar(index, IDF_MAP_INPUT);
		CompNumSubPar(index, IDF_MAP_X, 0);
		CompNumSubPar(index, IDF_MAP_Y, 0);
		return true;

	case IDF_CONSOLE_ON:
		CompAddItem(FGetConsoleOn);					// režim konzoly
		return true;

	case IDF_JOYSTICK_1:
		if (!CompNumPar(index, FJoystick1))			// tlaèítko 1 joysticku
		{
			CompAddItem(FJoystick1);
			CompAddItemNum((BYTE)0);
		}
		return true;

	case IDF_JOYSTICK_2:
		if (!CompNumPar(index, FJoystick2))			// tlaèítko 2 joysticku
		{
			CompAddItem(FJoystick2);
			CompAddItemNum((BYTE)0);
		}
		return true;

	case IDF_JOYSTICK_3:
		if (!CompNumPar(index, FJoystick3))			// tlaèítko 3 joysticku
		{
			CompAddItem(FJoystick3);
			CompAddItemNum((BYTE)0);
		}
		return true;

	case IDF_JOYSTICK_4:
		if (!CompNumPar(index, FJoystick4))			// tlaèítko 4 joysticku
		{
			CompAddItem(FJoystick4);
			CompAddItemNum((BYTE)0);
		}
		return true;

//	case IDF_DIALOG_ON:
//		CompAddItem(FGetDialogOn);					// režim dialogu
//		return true;

	case IDF_WINDOW_VISIBLE:
		CompAddItem(FGetWindowVisible);				// viditelnost okna
		return true;

	case IDF_WINDOW_DISABLE:
		CompAddItem(FGetWindowDisable);				// zákaz okna
		return true;

	case IDF_BUTTON_CLICK:
		CompAddItem(FButtonClick);					// klik na tlaèítko
		return true;

	case IDF_CHECKBOX_ON:
		CompAddItem(FGetCheckBoxOn);				// zapnutí pøepínaèe
		return true;

	case IDF_FONTBOLD:
		CompAddItem(FGetFontBold);					// zvýraznìný text
		return true;

	case IDF_FONTFIXED:
		CompAddItem(FGetFontFixed);					// text s pevnou rozteèí
		return true;

	case IDF_FONTITALIC:
		CompAddItem(FGetFontItalic);				// šikmý text
		return true;

	case IDF_FONTUNDER:
		CompAddItem(FGetFontUnder);					// podtržený text
		return true;

	case IDF_FONTSTRIKE:
		CompAddItem(FGetFontStrike);				// pøeškrtnutý text
		return true;

	case IDF_FONTSERIF:
		CompAddItem(FGetFontSerif);					// patièkový text
		return true;

	case IDF_BUTTON3_ON:
		CompAddItem(FGetButton3On);					// 3. stav pøepínaèe
		return true;

	case IDF_FULLSCREEN:
		CompAddItem(FGetFullScreen);				// celoobrazovkový režim
		return true;

	case IDF_SOUND_PAUSE:
		CompAddItem(FSoundTestPause);				// test pauzy zvuku
		return true;

	case IDF_EQU_MAP:
		return CompMapGrp(index, FCompEqIco, FCompEqIco1);	// shodnost ploch

	case IDF_EQU_PIC:
		return CompPicGrp(index, FCompEqPic, FCompEqPic1);	// shodnost obrázkù

	case IDF_EQU_SPR:
		return CompSprGrp(index, FCompEqSpr, FCompEqSpr1);	// shodnost sprajtù

	case IDF_EQU_SND:
		return CompSndGrp(index, FCompEqSnd, FCompEqSnd1);	// shodnost zvukù

	case IDF_EQU_MUS:
		return CompMusGrp(index, FCompEqMus, FCompEqMus1);	// shodnost hudby

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_GAME_L1:
		CompAddItem(FGetGameL1);					// zjištìní herního pøepínaèe 1
		return true;

	case IDF_GAME_L2:
		CompAddItem(FGetGameL2);					// zjištìní herního pøepínaèe 2
		return true;

	case IDF_GAME_L3:
		CompAddItem(FGetGameL3);					// zjištìní herního pøepínaèe 3
		return true;

	case IDF_GAME_L4:
		CompAddItem(FGetGameL4);					// zjištìní herního pøepínaèe 4
		return true;

	case IDF_GAME_L5:
		CompAddItem(FGetGameL5);					// zjištìní herního pøepínaèe 5
		return true;

	case IDF_GAME_L6:
		CompAddItem(FGetGameL6);					// zjištìní herního pøepínaèe 6
		return true;

	case IDF_GAME_L7:
		CompAddItem(FGetGameL7);					// zjištìní herního pøepínaèe 7
		return true;

	case IDF_GAME_L8:
		CompAddItem(FGetGameL8);					// zjištìní herního pøepínaèe 8
		return true;

	case IDF_GAME_HOST:
		CompAddItem(FGameHost);						// zjištìní pøíznaku hostitele
		return true;

	case IDF_GAME_DATAL:
		CompAddItem(FGetGameDataL);					// naètení pøepínaèe z paketu
		return true;

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------

	case IDF_WINDOW_FRAME:
		CompAddItem(FGetWindowFrame);				// naètení pøepínaèe rámeèku okna
		return true;

	case IDF_WINDOW_CAPTION:
		CompAddItem(FGetWindowCaption);				// naètení pøepínaèe nadpisu okna
		return true;

	case IDF_WINDOW_TOP:
		CompAddItem(FGetWindowTop);					// naètení pøíznaku okna vždy nahoøe
		return true;

	case IDF_WINDOW_RESIZABLE:
		CompAddItem(FGetWindowSize);				// naètení pøíznaku okna s mìnitelnou velikostí
		return true;

	case IDF_WINDOW_MAY_CLOSE:
		CompAddItem(FGetWindowMayClose);			// požadavek o uzavøení okna
		return true;

	case IDF_WINDOW_MAXIM:
		CompAddItem(FGetWindowMaxim);				// zjištìní maximalizace okna
		return true;

	case IDF_WINDOW_MINIM:
		CompAddItem(FGetWindowMinim);				// zjištìní minimalizace okna
		return true;

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_COM_STOP:
		CompAddItem(FGetComStop);					// zjištìní stopbitù portu
		return true;

	case IDF_COM_IN:
		CompAddItem(FComIn);						// test pøijatého znaku z portu
		return true;

	case IDF_COM_SEND:
		CompAddItem(FGetComSend);					// test povolení vysílání
		return true;

	case IDF_COM_RECEIVE:
		CompAddItem(FGetComReceive);				// test povolení pøíjmu
		return true;

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------

	case IDF_DIALOG_GRAPHIC:
		CompAddItem(FGetDialogGraph);				// test grafického pozadí
		return true;

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_MEMORY_BOOL:							// logická hodnota
		CompAddItem(FGetMemoryBool);
		return true;

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------


// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_DIRECT3D_WIREFRAME:					// nastavení módu výplnì 3D
		CompAddItem(FGetD3DWireframe);
		return true;

	case IDF_DIRECT3D_LIGHTON:						// nastavení módu osvìtlení 3D
		CompAddItem(FGetD3DLighton);
		return true;

	case IDF_DIRECT3D_SHADES:						// nastavení módu stínování 3D
		CompAddItem(FGetD3DShades);
		return true;

	case IDF_DIRECT3D_UPPER:						// vytváøet horní podstavu
		CompAddItem(FGetD3DUpper);
		return true;

	case IDF_DIRECT3D_LOWER:						// vytváøet dolní podstavu
		CompAddItem(FGetD3DLower);
		return true;

	case IDF_DIRECT3D_MINFILTER:					// zapnutí filtru pøi zmenšení
		CompAddItem(FGetD3DMinFilter);
		return true;

	case IDF_DIRECT3D_MAGFILTER:					// zapnutí filtru pøi zvìtšení
		CompAddItem(FGetD3DMagFilter);
		return true;

	case IDF_DIRECT3D_MIPFILTER:					// zapnutí filtru pøi vzdálení
		CompAddItem(FGetD3DMipFilter);
		return true;

	case IDF_DIRECT3D_OWIREFRAME:					// nastavení módu výplnì objektu 3D
		CompAddItem(FGetD3DOWireframe);
		return true;

	case IDF_DIRECT3D_OLIGHTON:						// nastavení módu osvìtlení objektu 3D
		CompAddItem(FGetD3DOLighton);
		return true;

	case IDF_DIRECT3D_OSHADES:						// nastavení módu stínování objektu 3D
		CompAddItem(FGetD3DOShades);
		return true;

	case IDF_DIRECT3D_OMINFILTER:					// zapnutí filtru pøi zmenšení textury objektu
		CompAddItem(FGetD3DOMinFilter);
		return true;

	case IDF_DIRECT3D_OMAGFILTER:					// zapnutí filtru pøi zvìtšení textury objektu
		CompAddItem(FGetD3DOMagFilter);
		return true;

	case IDF_DIRECT3D_OMIPFILTER:					// zapnutí filtru pøi vzdálení textury objektu
		CompAddItem(FGetD3DOMipFilter);
		return true;

	case IDF_DIRECT3D_VISIBLE:						// test viditelnosti objektu Direct3D
		CompAddItem(FGetD3DVisible);
		return true;

	case IDF_DIRECT3D_ZSORT:						// hloubkové tøídìní
		CompAddItem(FGetD3DZSort);
		return true;

	case IDF_DIRECT3D_ZWRITE:						// hloubkový zápis
		CompAddItem(FGetD3DZWrite);
		return true;

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------

	case IDF_ACTIVE:								// test, zda to je aktivní aplikace
		CompAddItem(FGetActive);
		return true;

	case IDF_PRINT_PICTURE:
		return CompPicPar(index, FPicPrint);		// tisk obrázku

	default:
		return false;
	}
}


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu s logickým parametrem

bool CompLogPar(int index, PROCCOM func)
{
	return CompLogPar(index, func, 0, -1);
}

bool CompLogPar(int index, PROCCOM func, int data)
{
	return CompLogPar(index, func, data, -1);
}

bool CompLogPar(int index, PROCCOM func, int data, int list)
{
	int zahlavi = CompAddItem(func, data, list);

	if ((BufEdi2[index].Items == 1) || !CompLog(index+1))
	{
		ProgNum = zahlavi;
		return false;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu s logickým podparametrem (hledaným podle identifikace)

void CompLogSubPar(int index, int idf, bool def)
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
				CompLog(index + 1))
			{
				return;
			}

// posun pro pøíští prvek
			posun = item2->Items;

// dokud je další potomek
		} while (item->Param & PETPROG_NEXT);
	}

// použije se implicitní hodnota
	if (def)
	{
		CompAddItem(FTrue);							// konstanta TRUE
	}
	else
	{
		CompAddItem(FFalse);						// konstanta FALSE
	}
}
