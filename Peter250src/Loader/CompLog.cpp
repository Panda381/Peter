
#include "Main.h"

/***************************************************************************\
*																			*
*					Kompilace programu - logick� v�raz						*
*																			*
\***************************************************************************/

#pragma optimize("s", on)			// optimalizace na minim�ln� velikost


/////////////////////////////////////////////////////////////////////////////
// p�eklad logick�ho v�razu (vrac� true = operace OK)

bool _fastcall CompLog(int index)
{
// adresa zdrojov�ho prvku
	if ((DWORD)index >= (DWORD)BufEdiN) return false;
	PETPROG*	item = BufEdi + index;
	PETPROG2*	item2 = BufEdi2 + index;
	int refinx = item->RefIndex;
	PETPROG2*	prg2;

// kontrola, zda je polo�ka vypnuta
	if ((item->Param & (PETPROG_OFF | PETPROG_OFF_DEP)) != 0) return false;

// v�tven� podle funkce
	switch (item->Func + IDF)
	{
	case IDF_EQU:
		return CompNumGrp(index, FEqu, FEqu1);		// oper�tor EQU

	case IDF_NEQU:
		return CompNumGrp(index, FNEqu, FNEqu1);	// oper�tor NEQU

	case IDF_EQGR:
		return CompNumGrp(index, FEqGr, FEqGr1);	// oper�tor EQGR

	case IDF_EQLT:
		return CompNumGrp(index, FEqLt, FEqLt1);	// oper�tor EQLT

	case IDF_GR:
		return CompNumGrp(index, FGr, FGr1);		// oper�tor GR

	case IDF_LT:
		return CompNumGrp(index, FLt, FLt1);		// oper�tor LT

	case IDF_AND:
		return CompLogGrp(index, FAnd, FAnd1);		// oper�tor AND

	case IDF_OR:
		return CompLogGrp(index, FOr, FOr1);		// oper�tor OR

	case IDF_XOR:
		return CompLogGrp(index, FXor, FXor1);		// oper�tor XOR

	case IDF_NOT:
		return CompLogPar(index, FNot);				// oper�tor NOT

	case IDF_FNC:
		return CompFunc(index, IDF_BOOL);			// funkce s n�vratem logick� hodnoty

	case IDF_BOOL:									// logick� prom�nn�
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
		return CompNumPar(index, FKeyTest);			// test stisku kl�vesy

	case IDF_MOUSE_L:
		CompAddItem(FMouseL);						// test stisku lev�ho tla��tka my�i
		return true;

	case IDF_MOUSE_R:
		CompAddItem(FMouseR);						// test stisku prav�ho tla��tka my�i
		return true;

	case IDF_EQU_TEXT:
		return CompTxtGrp(index, FCompEqTxt, FCompEqTxt1);	// shodnost text�

	case IDF_EQU_ICO:
		return CompIcoGrp(index, FCompEqIco, FCompEqIco1);	// shodnost p�edm�t�

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
		CompAddItem(FGetSwc1);						// na�ten� p�ep�na�e 1 - Petr
		return true;

	case IDF_PETRA_SWC1:
		CompAddItem(FGetSwc12);						// na�ten� p�ep�na�e 1 - Petra
		return true;

	case IDF_PETER_SWC2:
		CompAddItem(FGetSwc2);						// na�ten� p�ep�na�e 2 - Petr
		return true;

	case IDF_PETRA_SWC2:
		CompAddItem(FGetSwc22);						// na�ten� p�ep�na�e 2 - Petra
		return true;

	case IDF_PETER_SWC3:
		CompAddItem(FGetSwc3);						// na�ten� p�ep�na�e 3 - Petr
		return true;

	case IDF_PETRA_SWC3:
		CompAddItem(FGetSwc32);						// na�ten� p�ep�na�e 3 - Petra
		return true;

	case IDF_PETER_SWC4:
		CompAddItem(FGetSwc4);						// na�ten� p�ep�na�e 4 - Petr
		return true;

	case IDF_PETRA_SWC4:
		CompAddItem(FGetSwc42);						// na�ten� p�ep�na�e 4 - Petra
		return true;

	case IDF_PETER_SWC5:
		CompAddItem(FGetSwc5);						// na�ten� p�ep�na�e 5 - Petr
		return true;

	case IDF_PETRA_SWC5:
		CompAddItem(FGetSwc52);						// na�ten� p�ep�na�e 5 - Petra
		return true;

	case IDF_TEXT_GT:
		return CompTxtGrp(index, FCompGtTxt, FCompGtTxt1);	// abecedn� porovn�n� text�

	case IDF_STATUS_NUMLOCK:
		CompAddItem(FStatusNumLock);				// na�ten� stavu Num Lock
		return true;

	case IDF_STATUS_CAPSLOCK:
		CompAddItem(FStatusCapsLock);				// na�ten� stavu Caps Lock
		return true;

	case IDF_STATUS_SCROLLLOCK:
		CompAddItem(FStatusScrollLock);				// na�ten� stavu Scroll Lock
		return true;

	case IDF_STATUS_INSERT:
		CompAddItem(FStatusInsert);					// na�ten� stavu Insert
		return true;

	case IDF_MOUSE_LCLICK:
		CompAddItem(FMouseLClick);					// test kliku lev�m tla��tkem my�i
		return true;

	case IDF_MOUSE_RCLICK:
		CompAddItem(FMouseRClick);					// test kliku prav�m tla��tkem my�i
		return true;

	case IDF_MOUSE_LDCLICK:
		CompAddItem(FMouseLDClick);					// test dvojkliku lev�m tla��tkem my�i
		return true;

	case IDF_MOUSE_RDCLICK:
		CompAddItem(FMouseRDClick);					// test dvojkliku prav�m tla��tkem my�i
		return true;

	case IDF_MOUSE_REGION:
		CompAddItem(FMouseRegion);					// test oblasti my�i
		CompNumSubPar(index, IDF_MOUSE_REGION_X, 0);
		CompNumSubPar(index, IDF_MOUSE_REGION_Y, 0);
		CompNumSubPar(index, IDF_MOUSE_REGION_W, 2000000);
		CompNumSubPar(index, IDF_MOUSE_REGION_H, 2000000);
		return true;

	case IDF_MOUSE_ITEM:
		return CompNumPar(index, FMouseItem);			// test my�i v prvku

	case IDF_SOUND_TEST:
		if (!CompSndPar(index, FSoundTest1))		// test p�ehr�v�n� zvuku
		{
			CompAddItem(FSoundTest);
		}
		return true;

	case IDF_MUSIC_TEST:
		CompAddItem(FMusicTest);					// test p�ehr�v�n� hudby
		return true;

	case IDF_MUSIC_PAUSE:
		CompAddItem(FMusicTestPause);				// test pauzy hudby
		return true;

	case IDF_CD_PAUSE:
		CompAddItem(FCDGetPause);					// test pauzy CD
		return true;

	case IDF_CD_EJECT:
		CompAddItem(FCDGetEject);					// test zasunut� CD
		return true;

	case IDF_CD_TEST:
		CompAddItem(FCDTest);						// test p�ehr�v�n� CD
		return true;

	case IDF_FILE_FILENEW:
		return CompTxtPar(index, FBoolFileNew);		// vytvo�en� souboru

	case IDF_FILE_DIRNEW:
		return CompTxtPar(index, FBoolDirNew);		// vytvo�en� adres��e

	case IDF_FILE_TEXT_UNICODE:						// text v k�du UNICODE
		CompAddItem(FGetFileTextUnicode);
		return true;

	case IDF_FILE_LOGIC:
		CompAddItem(FGetFileLogic);					// na�ten� logick� hodnoty
		return true;

	case IDF_FILE_DELETE:
		return CompTxtPar(index, FBoolFileDelete);	// zru�en� souboru

	case IDF_FILE_TEST:
		return CompTxtPar(index, FFileTest);		// test platnosti disku/souboru/adres��e

	case IDF_FILE_COPY:
		CompAddItem(FBoolFileCopy);					// kopie souboru
		return true;

	case IDF_FILE_MOVE:
		CompAddItem(FBoolFileMove);					// p�esun souboru
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
		CompAddItem(FGetFileError);					// p��znak chyby soubor�
		return true;

	case IDF_SOUND_STEREO:
		return CompSndPar(index, FGetSoundStereo);	// p��znak stereo zvuku

	case IDF_MAP_SWC1:
		CompAddItem(FGetMapSwc1);					// p�ep�na� 1 z plochy
		CompMapSubPar(index, IDF_MAP_INPUT);
		CompNumSubPar(index, IDF_MAP_X, 0);
		CompNumSubPar(index, IDF_MAP_Y, 0);
		return true;

	case IDF_MAP_SWC2:
		CompAddItem(FGetMapSwc2);					// p�ep�na� 2 z plochy
		CompMapSubPar(index, IDF_MAP_INPUT);
		CompNumSubPar(index, IDF_MAP_X, 0);
		CompNumSubPar(index, IDF_MAP_Y, 0);
		return true;

	case IDF_MAP_SWC3:
		CompAddItem(FGetMapSwc3);					// p�ep�na� 3 z plochy
		CompMapSubPar(index, IDF_MAP_INPUT);
		CompNumSubPar(index, IDF_MAP_X, 0);
		CompNumSubPar(index, IDF_MAP_Y, 0);
		return true;

	case IDF_MAP_SWC4:
		CompAddItem(FGetMapSwc4);					// p�ep�na� 4 z plochy
		CompMapSubPar(index, IDF_MAP_INPUT);
		CompNumSubPar(index, IDF_MAP_X, 0);
		CompNumSubPar(index, IDF_MAP_Y, 0);
		return true;

	case IDF_MAP_SWC5:
		CompAddItem(FGetMapSwc5);					// p�ep�na� 5 z plochy
		CompMapSubPar(index, IDF_MAP_INPUT);
		CompNumSubPar(index, IDF_MAP_X, 0);
		CompNumSubPar(index, IDF_MAP_Y, 0);
		return true;

	case IDF_CONSOLE_ON:
		CompAddItem(FGetConsoleOn);					// re�im konzoly
		return true;

	case IDF_JOYSTICK_1:
		if (!CompNumPar(index, FJoystick1))			// tla��tko 1 joysticku
		{
			CompAddItem(FJoystick1);
			CompAddItemNum((BYTE)0);
		}
		return true;

	case IDF_JOYSTICK_2:
		if (!CompNumPar(index, FJoystick2))			// tla��tko 2 joysticku
		{
			CompAddItem(FJoystick2);
			CompAddItemNum((BYTE)0);
		}
		return true;

	case IDF_JOYSTICK_3:
		if (!CompNumPar(index, FJoystick3))			// tla��tko 3 joysticku
		{
			CompAddItem(FJoystick3);
			CompAddItemNum((BYTE)0);
		}
		return true;

	case IDF_JOYSTICK_4:
		if (!CompNumPar(index, FJoystick4))			// tla��tko 4 joysticku
		{
			CompAddItem(FJoystick4);
			CompAddItemNum((BYTE)0);
		}
		return true;

//	case IDF_DIALOG_ON:
//		CompAddItem(FGetDialogOn);					// re�im dialogu
//		return true;

	case IDF_WINDOW_VISIBLE:
		CompAddItem(FGetWindowVisible);				// viditelnost okna
		return true;

	case IDF_WINDOW_DISABLE:
		CompAddItem(FGetWindowDisable);				// z�kaz okna
		return true;

	case IDF_BUTTON_CLICK:
		CompAddItem(FButtonClick);					// klik na tla��tko
		return true;

	case IDF_CHECKBOX_ON:
		CompAddItem(FGetCheckBoxOn);				// zapnut� p�ep�na�e
		return true;

	case IDF_FONTBOLD:
		CompAddItem(FGetFontBold);					// zv�razn�n� text
		return true;

	case IDF_FONTFIXED:
		CompAddItem(FGetFontFixed);					// text s pevnou rozte��
		return true;

	case IDF_FONTITALIC:
		CompAddItem(FGetFontItalic);				// �ikm� text
		return true;

	case IDF_FONTUNDER:
		CompAddItem(FGetFontUnder);					// podtr�en� text
		return true;

	case IDF_FONTSTRIKE:
		CompAddItem(FGetFontStrike);				// p�e�krtnut� text
		return true;

	case IDF_FONTSERIF:
		CompAddItem(FGetFontSerif);					// pati�kov� text
		return true;

	case IDF_BUTTON3_ON:
		CompAddItem(FGetButton3On);					// 3. stav p�ep�na�e
		return true;

	case IDF_FULLSCREEN:
		CompAddItem(FGetFullScreen);				// celoobrazovkov� re�im
		return true;

	case IDF_SOUND_PAUSE:
		CompAddItem(FSoundTestPause);				// test pauzy zvuku
		return true;

	case IDF_EQU_MAP:
		return CompMapGrp(index, FCompEqIco, FCompEqIco1);	// shodnost ploch

	case IDF_EQU_PIC:
		return CompPicGrp(index, FCompEqPic, FCompEqPic1);	// shodnost obr�zk�

	case IDF_EQU_SPR:
		return CompSprGrp(index, FCompEqSpr, FCompEqSpr1);	// shodnost sprajt�

	case IDF_EQU_SND:
		return CompSndGrp(index, FCompEqSnd, FCompEqSnd1);	// shodnost zvuk�

	case IDF_EQU_MUS:
		return CompMusGrp(index, FCompEqMus, FCompEqMus1);	// shodnost hudby

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_GAME_L1:
		CompAddItem(FGetGameL1);					// zji�t�n� hern�ho p�ep�na�e 1
		return true;

	case IDF_GAME_L2:
		CompAddItem(FGetGameL2);					// zji�t�n� hern�ho p�ep�na�e 2
		return true;

	case IDF_GAME_L3:
		CompAddItem(FGetGameL3);					// zji�t�n� hern�ho p�ep�na�e 3
		return true;

	case IDF_GAME_L4:
		CompAddItem(FGetGameL4);					// zji�t�n� hern�ho p�ep�na�e 4
		return true;

	case IDF_GAME_L5:
		CompAddItem(FGetGameL5);					// zji�t�n� hern�ho p�ep�na�e 5
		return true;

	case IDF_GAME_L6:
		CompAddItem(FGetGameL6);					// zji�t�n� hern�ho p�ep�na�e 6
		return true;

	case IDF_GAME_L7:
		CompAddItem(FGetGameL7);					// zji�t�n� hern�ho p�ep�na�e 7
		return true;

	case IDF_GAME_L8:
		CompAddItem(FGetGameL8);					// zji�t�n� hern�ho p�ep�na�e 8
		return true;

	case IDF_GAME_HOST:
		CompAddItem(FGameHost);						// zji�t�n� p��znaku hostitele
		return true;

	case IDF_GAME_DATAL:
		CompAddItem(FGetGameDataL);					// na�ten� p�ep�na�e z paketu
		return true;

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------

	case IDF_WINDOW_FRAME:
		CompAddItem(FGetWindowFrame);				// na�ten� p�ep�na�e r�me�ku okna
		return true;

	case IDF_WINDOW_CAPTION:
		CompAddItem(FGetWindowCaption);				// na�ten� p�ep�na�e nadpisu okna
		return true;

	case IDF_WINDOW_TOP:
		CompAddItem(FGetWindowTop);					// na�ten� p��znaku okna v�dy naho�e
		return true;

	case IDF_WINDOW_RESIZABLE:
		CompAddItem(FGetWindowSize);				// na�ten� p��znaku okna s m�nitelnou velikost�
		return true;

	case IDF_WINDOW_MAY_CLOSE:
		CompAddItem(FGetWindowMayClose);			// po�adavek o uzav�en� okna
		return true;

	case IDF_WINDOW_MAXIM:
		CompAddItem(FGetWindowMaxim);				// zji�t�n� maximalizace okna
		return true;

	case IDF_WINDOW_MINIM:
		CompAddItem(FGetWindowMinim);				// zji�t�n� minimalizace okna
		return true;

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_COM_STOP:
		CompAddItem(FGetComStop);					// zji�t�n� stopbit� portu
		return true;

	case IDF_COM_IN:
		CompAddItem(FComIn);						// test p�ijat�ho znaku z portu
		return true;

	case IDF_COM_SEND:
		CompAddItem(FGetComSend);					// test povolen� vys�l�n�
		return true;

	case IDF_COM_RECEIVE:
		CompAddItem(FGetComReceive);				// test povolen� p��jmu
		return true;

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------

	case IDF_DIALOG_GRAPHIC:
		CompAddItem(FGetDialogGraph);				// test grafick�ho pozad�
		return true;

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_MEMORY_BOOL:							// logick� hodnota
		CompAddItem(FGetMemoryBool);
		return true;

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------


// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	case IDF_DIRECT3D_WIREFRAME:					// nastaven� m�du v�pln� 3D
		CompAddItem(FGetD3DWireframe);
		return true;

	case IDF_DIRECT3D_LIGHTON:						// nastaven� m�du osv�tlen� 3D
		CompAddItem(FGetD3DLighton);
		return true;

	case IDF_DIRECT3D_SHADES:						// nastaven� m�du st�nov�n� 3D
		CompAddItem(FGetD3DShades);
		return true;

	case IDF_DIRECT3D_UPPER:						// vytv��et horn� podstavu
		CompAddItem(FGetD3DUpper);
		return true;

	case IDF_DIRECT3D_LOWER:						// vytv��et doln� podstavu
		CompAddItem(FGetD3DLower);
		return true;

	case IDF_DIRECT3D_MINFILTER:					// zapnut� filtru p�i zmen�en�
		CompAddItem(FGetD3DMinFilter);
		return true;

	case IDF_DIRECT3D_MAGFILTER:					// zapnut� filtru p�i zv�t�en�
		CompAddItem(FGetD3DMagFilter);
		return true;

	case IDF_DIRECT3D_MIPFILTER:					// zapnut� filtru p�i vzd�len�
		CompAddItem(FGetD3DMipFilter);
		return true;

	case IDF_DIRECT3D_OWIREFRAME:					// nastaven� m�du v�pln� objektu 3D
		CompAddItem(FGetD3DOWireframe);
		return true;

	case IDF_DIRECT3D_OLIGHTON:						// nastaven� m�du osv�tlen� objektu 3D
		CompAddItem(FGetD3DOLighton);
		return true;

	case IDF_DIRECT3D_OSHADES:						// nastaven� m�du st�nov�n� objektu 3D
		CompAddItem(FGetD3DOShades);
		return true;

	case IDF_DIRECT3D_OMINFILTER:					// zapnut� filtru p�i zmen�en� textury objektu
		CompAddItem(FGetD3DOMinFilter);
		return true;

	case IDF_DIRECT3D_OMAGFILTER:					// zapnut� filtru p�i zv�t�en� textury objektu
		CompAddItem(FGetD3DOMagFilter);
		return true;

	case IDF_DIRECT3D_OMIPFILTER:					// zapnut� filtru p�i vzd�len� textury objektu
		CompAddItem(FGetD3DOMipFilter);
		return true;

	case IDF_DIRECT3D_VISIBLE:						// test viditelnosti objektu Direct3D
		CompAddItem(FGetD3DVisible);
		return true;

	case IDF_DIRECT3D_ZSORT:						// hloubkov� t��d�n�
		CompAddItem(FGetD3DZSort);
		return true;

	case IDF_DIRECT3D_ZWRITE:						// hloubkov� z�pis
		CompAddItem(FGetD3DZWrite);
		return true;

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------

	case IDF_ACTIVE:								// test, zda to je aktivn� aplikace
		CompAddItem(FGetActive);
		return true;

	case IDF_PRINT_PICTURE:
		return CompPicPar(index, FPicPrint);		// tisk obr�zku

	default:
		return false;
	}
}


/////////////////////////////////////////////////////////////////////////////
// p�eklad p��kazu s logick�m parametrem

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
// p�eklad p��kazu s logick�m podparametrem (hledan�m podle identifikace)

void CompLogSubPar(int index, int idf, bool def)
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
				CompLog(index + 1))
			{
				return;
			}

// posun pro p��t� prvek
			posun = item2->Items;

// dokud je dal�� potomek
		} while (item->Param & PETPROG_NEXT);
	}

// pou�ije se implicitn� hodnota
	if (def)
	{
		CompAddItem(FTrue);							// konstanta TRUE
	}
	else
	{
		CompAddItem(FFalse);						// konstanta FALSE
	}
}
