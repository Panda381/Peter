
/***************************************************************************\
*																			*
*						Prov�d�n� programu - logick� v�raz					*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inline vol�n� p��kazu - logick� v�raz

inline bool FBool()
{
	ExecItem++;
	return ExecItem[-1].ExecLog();
}


// funkce
bool FBoolFunc();				// funkce s n�vratem logick� hodnoty

// prom�nn�
bool FTrue();					// konstanta TRUE
bool FFalse();					// konstanta FALSE
bool FBoolObj();				// glob�ln� prom�nn� (Data = index)
bool FBoolObjList();			// globaln� prom�nn� v seznamu (Data = index, List = seznam)
bool FBoolLoc();				// lok�ln� prom�nn� (Data = index)
bool FBoolLocList();			// lok�ln� prom�nn� v seznamu (Data = index, List = seznam)
bool FGetSpriteMove();			// pohyb glob�ln�ho sprajtu (Data = index)
bool FGetSpriteMoveList();		// pohyb sprajtu v seznamu (Data = index, List = seznam)
bool FGetSpriteMoveLoc();		// pohyb lok�ln�ho sprajtu (Data = index)
bool FGetSpriteMoveLocList();	// pohyb lok�ln�ho sprajtu v seznamu (Data = index, List = seznam)
bool FGetSpriteVis();			// viditelnost glob�ln�ho sprajtu (Data = index)
bool FGetSpriteVisList();		// viditelnost sprajtu v seznamu (Data = index, List = seznam)
bool FGetSpriteVisLoc();		// viditelnost lok�ln�ho sprajtu (Data = index)
bool FGetSpriteVisLocList();	// viditelnost lok�ln�ho sprajtu v seznamu (Data = index, List = seznam)

// matematick� oper�tory
bool FEqu1();					// oper�tor EQU - pro 1 prvek (porovn� ��slo s nulou)
bool FEqu();					// oper�tor EQU (Data = po�et prvk� - 1)
bool FNEqu1();					// oper�tor NEQU - pro 1 prvek (porovn� ��slo s nulou)
bool FNEqu();					// oper�tor NEQU (Data = po�et prvk� - 1)
bool FEqGr1();					// oper�tor EQGR - pro 1 prvek (porovn� ��slo s nulou)
bool FEqGr();					// oper�tor EQGR (Data = po�et prvk� - 1)
bool FEqLt1();					// oper�tor EQLT - pro 1 prvek (porovn� ��slo s nulou)
bool FEqLt();					// oper�tor EQLT (Data = po�et prvk� - 1)
bool FGr1();					// oper�tor GR - pro 1 prvek (porovn� ��slo s nulou)
bool FGr();						// oper�tor GR (Data = po�et prvk� - 1)
bool FLt1();					// oper�tor LT - pro 1 prvek (porovn� ��slo s nulou)
bool FLt();						// oper�tor LT (Data = po�et prvk� - 1)

// logick� oper�tory
bool FAnd1();					// oper�tor AND - pro 1 prvek (jenom vr�t� hodnotu beze zm�ny)
bool FAnd();					// oper�tor AND (Data = po�et prvk� - 1)
inline bool FOr1() { return FAnd1(); }	// oper�tor OR - pro 1 prvek (jenom vr�t� hodnotu beze zm�ny)
bool FOr();						// oper�tor OR (Data = po�et prvk� - 1)
inline bool FXor1() { return FAnd1(); }	// oper�tor XOR - pro 1 prvek (jenom vr�t� hodnotu beze zm�ny)
bool FXor();					// oper�tor XOR (Data = po�et prvk� - 1)
bool FNot();					// oper�tor NOT

// obsluha ikon
bool FCompEqIco1();				// porovn�n� jedn� ikony, zda je pr�zdn�
bool FCompEqIco();				// porovn�n� shodnosti ikon

bool FCompEqMap1();				// porovn�n� jedn� plochy, zda je pr�zdn�
bool FCompEqMap();				// porovn�n� shodnosti ploch
bool FCompEqPic1();				// porovn�n� jednoho obr�zku, zda je pr�zdn�
bool FCompEqPic();				// porovn�n� shodnosti obr�zk�
bool FCompEqSpr1();				// porovn�n� jednoho sprajtu, zda je pr�zdn�
bool FCompEqSpr();				// porovn�n� shodnosti sprajt�
bool FCompEqSnd1();				// porovn�n� jednoho zvuku, zda je pr�zdn�
bool FCompEqSnd();				// porovn�n� shodnosti zvuk�
bool FCompEqMus1();				// porovn�n� jedn� hudby, zda je pr�zdn�
bool FCompEqMus();				// porovn�n� shodnosti hudeb

// obsluha text�
bool FCompEqTxt1();				// porovn�n� jednoho textu, zda je pr�zdn�
bool FCompEqTxt();				// porovn�n� text� na shodnost (Data = po�et text� - 1)
bool FCompGtTxt1();				// porovn�n� jednoho textu, zda je nepr�zdn�
bool FCompGtTxt();				// porovn�n� text� na neshodnost (Data = po�et text� - 1)

// obsluha Petra a Petry
bool FGetVisible();				// na�ten� viditelnosti Petra
bool FGetVisible2();			// na�ten� viditelnosti Petry

bool FGetSwc1();				// p�ep�na� 1 - Petr
bool FGetSwc2();				// p�ep�na� 2 - Petr
bool FGetSwc3();				// p�ep�na� 3 - Petr
bool FGetSwc4();				// p�ep�na� 4 - Petr
bool FGetSwc5();				// p�ep�na� 5 - Petr

bool FGetSwc12();				// p�ep�na� 1 - Petra
bool FGetSwc22();				// p�ep�na� 2 - Petra
bool FGetSwc32();				// p�ep�na� 3 - Petra
bool FGetSwc42();				// p�ep�na� 4 - Petra
bool FGetSwc52();				// p�ep�na� 5 - Petra

// obsluha kl�vesnice
bool FKeyTest();				// test stisku kl�vesy
bool FStatusNumLock();			// test stavu Num Lock
bool FStatusCapsLock();			// test stavu Caps Lock
bool FStatusScrollLock();		// test stavu Scroll Lock
bool FStatusInsert();			// test stavu Insert

// obsluha my�i
bool FMouseL();					// test stisku lev�ho tla��tka my�i
bool FMouseR();					// test stisku prav�ho tla��tka my�i
bool FMouseLClick();			// test kliku lev�m tla��tkem my�i
bool FMouseRClick();			// test kliku prav�m tla��tkem my�i
bool FMouseLDClick();			// test dvojkliku lev�m tla��tkem my�i
bool FMouseRDClick();			// test dvojkliku prav�m tla��tkem my�i
bool FMouseRegion();			// test my�i v oblasti
bool FMouseItem();				// test my�i v dialogov�m prvku

// obsluha zvuku
bool FSoundTest();				// test p�ehr�v�n� jak�hokoliv zvuku
bool FSoundTest1();				// test p�ehr�v�n� konkr�tn�ho zvuku
bool FGetSoundStereo();			// p��znak stereo
bool FSoundTestPause();			// test zapauzov�n� zvuku

// obsluha hudby
bool FMusicTest();				// test p�ehr�v�n� hudby
bool FMusicTestPause();			// test zapauzov�n� hudby

// obsluha CD disk�
bool FCDGetPause();				// test pauzy CD
bool FCDTest();					// test p�ehr�v�n� CD
bool FCDGetEject();				// test vysunut� CD

// obsluha soubor�
bool FBoolFileNew();			// vytvo�en� souboru
bool FBoolDirNew();				// vytvo�en� adres��e
bool FBoolFileDelete();			// zru�en� souboru/adres��e
bool FFileTest();				// test existence disku/souboru/adres��e
bool FBoolFileCopy();			// kopie soubor�
bool FBoolFileMove();			// p�esun soubor�
bool FGetFileRO();				// na�ten� atributu R/O souboru
bool FGetFileHID();				// na�ten� atributu HID souboru
bool FGetFileSYS();				// na�ten� atributu SYS souboru
bool FGetFileDIR();				// na�ten� atributu DIR souboru
bool FGetFileARC();				// na�ten� atributu ARC souboru
bool FGetFileTMP();				// na�ten� atributu TMP souboru
bool FGetFileCMP();				// na�ten� atributu CMP souboru
bool FGetFileOFF();				// na�ten� atributu OFF souboru
bool FGetFileError();			// poskytnut� p��znaku chyby soubor�
bool FGetFileTextUnicode();		// poskytnut� p��znaku textu UNICODE
bool FGetFileLogic();			// na�ten� logick� hodnoty

// obsluha plochy
bool FGetMapSwc1();				// na�ten� p�ep�na�e 1 z plochy
bool FGetMapSwc2();				// na�ten� p�ep�na�e 2 z plochy
bool FGetMapSwc3();				// na�ten� p�ep�na�e 3 z plochy
bool FGetMapSwc4();				// na�ten� p�ep�na�e 4 z plochy
bool FGetMapSwc5();				// na�ten� p�ep�na�e 5 z plochy

// konzola
bool FGetConsoleOn();			// re�im konzoly

// joystick
bool FJoystick1();				// tla��tko 1
bool FJoystick2();				// tla��tko 2
bool FJoystick3();				// tla��tko 3
bool FJoystick4();				// tla��tko 4

// dialogy
//bool FGetDialogOn();			// re�im dialogu
bool FGetWindowVisible();		// viditelnost okna
bool FGetWindowDisable();		// z�kaz okna
bool FButtonClick();			// kliknut� na tla��tko
bool FGetCheckBoxOn();			// stav p�ep�na�e
bool FGetButton3On();
bool FGetFontBold();			// zv�razn�n� p�smo
bool FGetFontFixed();			// p�smo s pevnou rozte��
bool FGetFontItalic();			// �ikm� p�smo
bool FGetFontUnder();			// podtr�en� p�smo
bool FGetFontStrike();			// p�e�krtnut� p�smo
bool FGetFontSerif();			// pati�kov� p�smo
bool FGetFullScreen();			// celoobrazovkov� re�im
bool FGetWindowFrame();			// na�ten� p�ep�na�e r�me�ku okna
bool FGetWindowCaption();		// na�ten� p�ep�na�e nadpisu okna
bool FGetWindowTop();			// na�ten� p�ep�na�e okna v�dy naho�e
bool FGetWindowSize();			// na�ten� p�ep�na�e okna m�niteln� velikosti
bool FGetWindowMayClose();		// po�adavek o uzav�en� okna
bool FGetWindowMaxim();			// zji�t�n� maximalizace okna
bool FGetWindowMinim();			// zji�t�n� minimalizace okna

// DirectPlay
bool FGetGameL1();				// zji�t�n� p�ep�na�e hry 1
bool FGetGameL2();				// zji�t�n� p�ep�na�e hry 2
bool FGetGameL3();				// zji�t�n� p�ep�na�e hry 3
bool FGetGameL4();				// zji�t�n� p�ep�na�e hry 4
bool FGetGameL5();				// zji�t�n� p�ep�na�e hry 5
bool FGetGameL6();				// zji�t�n� p�ep�na�e hry 6
bool FGetGameL7();				// zji�t�n� p�ep�na�e hry 7
bool FGetGameL8();				// zji�t�n� p�ep�na�e hry 8
bool FGameHost();				// zji�t�n� p��znaku hostitele
bool FGetGameDataL();			// na�ten� p�ep�na�e z paketu

// porty
bool FGetComStop();				// zji�t�n� stopbit�
bool FComIn();					// test p�ijat�ho znaku
bool FGetComSend();				// test povolen� vys�l�n�
bool FGetComReceive();			// test povolen� p��jmu

bool FGetDialogGraph();			// zji�t�n� m�du grafick�ho pozad�

// Direct3D
bool FGetD3DWireframe();		// zji�t�n� nastaven� vypl�ov�n� ploch
bool FGetD3DLighton();			// zji�t�n� nastaven� osv�tlen�
bool FGetD3DShades();			// zji�t�n� nastaven� plynul�ho st�nov�n�
bool FGetD3DMinFilter();		// zji�t�n� filtrace zmen�en�ch textur
bool FGetD3DMagFilter();		// zji�t�n� filtrace zv�t�en�ch textur
bool FGetD3DMipFilter();		// zji�t�n� filtrace vzd�len�ch textur
bool FGetD3DOWireframe();		// zji�t�n� nastaven� vypl�ov�n� ploch objektu
bool FGetD3DOLighton();			// zji�t�n� nastaven� osv�tlen� objektu
bool FGetD3DOShades();			// zji�t�n� nastaven� plynul�ho st�nov�n� objektu
bool FGetD3DOMinFilter();		// zji�t�n� filtrace zmen�en�ch textur objektu
bool FGetD3DOMagFilter();		// zji�t�n� filtrace zv�t�en�ch textur objektu
bool FGetD3DOMipFilter();		// zji�t�n� filtrace vzd�len�ch textur objektu
bool FGetD3DVisible();			// zji�t�n� viditelnosti objektu Direct3D
bool FGetD3DUpper();			// tvo�it horn� podstavu
bool FGetD3DLower();			// tvo�it doln� podstavu
bool FGetD3DZWrite();			// zji�t�n� hloubkov�ho z�pisu
bool FGetD3DZSort();			// zji�t�n� hloubkov�ho t��d�n�

bool FGetActive();				// test, zda to je aktivn� aplikace

bool FPicPrint();				// tisk obr�zku

// obsluha DLL
bool FGetMemoryBool();			// logick� hodnota
