
/***************************************************************************\
*																			*
*						Provádìní programu - logický výraz					*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inline volání pøíkazu - logický výraz

inline bool FBool()
{
	ExecItem++;
	return ExecItem[-1].ExecLog();
}


// funkce
bool FBoolFunc();				// funkce s návratem logické hodnoty

// promìnné
bool FTrue();					// konstanta TRUE
bool FFalse();					// konstanta FALSE
bool FBoolObj();				// globální promìnná (Data = index)
bool FBoolObjList();			// globalní promìnná v seznamu (Data = index, List = seznam)
bool FBoolLoc();				// lokální promìnná (Data = index)
bool FBoolLocList();			// lokální promìnná v seznamu (Data = index, List = seznam)
bool FGetSpriteMove();			// pohyb globálního sprajtu (Data = index)
bool FGetSpriteMoveList();		// pohyb sprajtu v seznamu (Data = index, List = seznam)
bool FGetSpriteMoveLoc();		// pohyb lokálního sprajtu (Data = index)
bool FGetSpriteMoveLocList();	// pohyb lokálního sprajtu v seznamu (Data = index, List = seznam)
bool FGetSpriteVis();			// viditelnost globálního sprajtu (Data = index)
bool FGetSpriteVisList();		// viditelnost sprajtu v seznamu (Data = index, List = seznam)
bool FGetSpriteVisLoc();		// viditelnost lokálního sprajtu (Data = index)
bool FGetSpriteVisLocList();	// viditelnost lokálního sprajtu v seznamu (Data = index, List = seznam)

// matematické operátory
bool FEqu1();					// operátor EQU - pro 1 prvek (porovná èíslo s nulou)
bool FEqu();					// operátor EQU (Data = poèet prvkù - 1)
bool FNEqu1();					// operátor NEQU - pro 1 prvek (porovná èíslo s nulou)
bool FNEqu();					// operátor NEQU (Data = poèet prvkù - 1)
bool FEqGr1();					// operátor EQGR - pro 1 prvek (porovná èíslo s nulou)
bool FEqGr();					// operátor EQGR (Data = poèet prvkù - 1)
bool FEqLt1();					// operátor EQLT - pro 1 prvek (porovná èíslo s nulou)
bool FEqLt();					// operátor EQLT (Data = poèet prvkù - 1)
bool FGr1();					// operátor GR - pro 1 prvek (porovná èíslo s nulou)
bool FGr();						// operátor GR (Data = poèet prvkù - 1)
bool FLt1();					// operátor LT - pro 1 prvek (porovná èíslo s nulou)
bool FLt();						// operátor LT (Data = poèet prvkù - 1)

// logické operátory
bool FAnd1();					// operátor AND - pro 1 prvek (jenom vrátí hodnotu beze zmìny)
bool FAnd();					// operátor AND (Data = poèet prvkù - 1)
inline bool FOr1() { return FAnd1(); }	// operátor OR - pro 1 prvek (jenom vrátí hodnotu beze zmìny)
bool FOr();						// operátor OR (Data = poèet prvkù - 1)
inline bool FXor1() { return FAnd1(); }	// operátor XOR - pro 1 prvek (jenom vrátí hodnotu beze zmìny)
bool FXor();					// operátor XOR (Data = poèet prvkù - 1)
bool FNot();					// operátor NOT

// obsluha ikon
bool FCompEqIco1();				// porovnání jedné ikony, zda je prázdná
bool FCompEqIco();				// porovnání shodnosti ikon

bool FCompEqMap1();				// porovnání jedné plochy, zda je prázdná
bool FCompEqMap();				// porovnání shodnosti ploch
bool FCompEqPic1();				// porovnání jednoho obrázku, zda je prázdný
bool FCompEqPic();				// porovnání shodnosti obrázkù
bool FCompEqSpr1();				// porovnání jednoho sprajtu, zda je prázdný
bool FCompEqSpr();				// porovnání shodnosti sprajtù
bool FCompEqSnd1();				// porovnání jednoho zvuku, zda je prázdný
bool FCompEqSnd();				// porovnání shodnosti zvukù
bool FCompEqMus1();				// porovnání jedné hudby, zda je prázdná
bool FCompEqMus();				// porovnání shodnosti hudeb

// obsluha textù
bool FCompEqTxt1();				// porovnání jednoho textu, zda je prázdný
bool FCompEqTxt();				// porovnání textù na shodnost (Data = poèet textù - 1)
bool FCompGtTxt1();				// porovnání jednoho textu, zda je neprázdný
bool FCompGtTxt();				// porovnání textù na neshodnost (Data = poèet textù - 1)

// obsluha Petra a Petry
bool FGetVisible();				// naètení viditelnosti Petra
bool FGetVisible2();			// naètení viditelnosti Petry

bool FGetSwc1();				// pøepínaè 1 - Petr
bool FGetSwc2();				// pøepínaè 2 - Petr
bool FGetSwc3();				// pøepínaè 3 - Petr
bool FGetSwc4();				// pøepínaè 4 - Petr
bool FGetSwc5();				// pøepínaè 5 - Petr

bool FGetSwc12();				// pøepínaè 1 - Petra
bool FGetSwc22();				// pøepínaè 2 - Petra
bool FGetSwc32();				// pøepínaè 3 - Petra
bool FGetSwc42();				// pøepínaè 4 - Petra
bool FGetSwc52();				// pøepínaè 5 - Petra

// obsluha klávesnice
bool FKeyTest();				// test stisku klávesy
bool FStatusNumLock();			// test stavu Num Lock
bool FStatusCapsLock();			// test stavu Caps Lock
bool FStatusScrollLock();		// test stavu Scroll Lock
bool FStatusInsert();			// test stavu Insert

// obsluha myši
bool FMouseL();					// test stisku levého tlaèítka myši
bool FMouseR();					// test stisku pravého tlaèítka myši
bool FMouseLClick();			// test kliku levým tlaèítkem myši
bool FMouseRClick();			// test kliku pravým tlaèítkem myši
bool FMouseLDClick();			// test dvojkliku levým tlaèítkem myši
bool FMouseRDClick();			// test dvojkliku pravým tlaèítkem myši
bool FMouseRegion();			// test myši v oblasti
bool FMouseItem();				// test myši v dialogovém prvku

// obsluha zvuku
bool FSoundTest();				// test pøehrávání jakéhokoliv zvuku
bool FSoundTest1();				// test pøehrávání konkrétního zvuku
bool FGetSoundStereo();			// pøíznak stereo
bool FSoundTestPause();			// test zapauzování zvuku

// obsluha hudby
bool FMusicTest();				// test pøehrávání hudby
bool FMusicTestPause();			// test zapauzování hudby

// obsluha CD diskù
bool FCDGetPause();				// test pauzy CD
bool FCDTest();					// test pøehrávání CD
bool FCDGetEject();				// test vysunutí CD

// obsluha souborù
bool FBoolFileNew();			// vytvoøení souboru
bool FBoolDirNew();				// vytvoøení adresáøe
bool FBoolFileDelete();			// zrušení souboru/adresáøe
bool FFileTest();				// test existence disku/souboru/adresáøe
bool FBoolFileCopy();			// kopie souborù
bool FBoolFileMove();			// pøesun souborù
bool FGetFileRO();				// naètení atributu R/O souboru
bool FGetFileHID();				// naètení atributu HID souboru
bool FGetFileSYS();				// naètení atributu SYS souboru
bool FGetFileDIR();				// naètení atributu DIR souboru
bool FGetFileARC();				// naètení atributu ARC souboru
bool FGetFileTMP();				// naètení atributu TMP souboru
bool FGetFileCMP();				// naètení atributu CMP souboru
bool FGetFileOFF();				// naètení atributu OFF souboru
bool FGetFileError();			// poskytnutí pøíznaku chyby souborù
bool FGetFileTextUnicode();		// poskytnutí pøíznaku textu UNICODE
bool FGetFileLogic();			// naètení logické hodnoty

// obsluha plochy
bool FGetMapSwc1();				// naètení pøepínaèe 1 z plochy
bool FGetMapSwc2();				// naètení pøepínaèe 2 z plochy
bool FGetMapSwc3();				// naètení pøepínaèe 3 z plochy
bool FGetMapSwc4();				// naètení pøepínaèe 4 z plochy
bool FGetMapSwc5();				// naètení pøepínaèe 5 z plochy

// konzola
bool FGetConsoleOn();			// režim konzoly

// joystick
bool FJoystick1();				// tlaèítko 1
bool FJoystick2();				// tlaèítko 2
bool FJoystick3();				// tlaèítko 3
bool FJoystick4();				// tlaèítko 4

// dialogy
//bool FGetDialogOn();			// režim dialogu
bool FGetWindowVisible();		// viditelnost okna
bool FGetWindowDisable();		// zákaz okna
bool FButtonClick();			// kliknutí na tlaèítko
bool FGetCheckBoxOn();			// stav pøepínaèe
bool FGetButton3On();
bool FGetFontBold();			// zvýraznìné písmo
bool FGetFontFixed();			// písmo s pevnou rozteèí
bool FGetFontItalic();			// šikmé písmo
bool FGetFontUnder();			// podtržené písmo
bool FGetFontStrike();			// pøeškrtnuté písmo
bool FGetFontSerif();			// patièkové písmo
bool FGetFullScreen();			// celoobrazovkový režim
bool FGetWindowFrame();			// naètení pøepínaèe rámeèku okna
bool FGetWindowCaption();		// naètení pøepínaèe nadpisu okna
bool FGetWindowTop();			// naètení pøepínaèe okna vždy nahoøe
bool FGetWindowSize();			// naètení pøepínaèe okna mìnitelné velikosti
bool FGetWindowMayClose();		// požadavek o uzavøení okna
bool FGetWindowMaxim();			// zjištìní maximalizace okna
bool FGetWindowMinim();			// zjištìní minimalizace okna

// DirectPlay
bool FGetGameL1();				// zjištìní pøepínaèe hry 1
bool FGetGameL2();				// zjištìní pøepínaèe hry 2
bool FGetGameL3();				// zjištìní pøepínaèe hry 3
bool FGetGameL4();				// zjištìní pøepínaèe hry 4
bool FGetGameL5();				// zjištìní pøepínaèe hry 5
bool FGetGameL6();				// zjištìní pøepínaèe hry 6
bool FGetGameL7();				// zjištìní pøepínaèe hry 7
bool FGetGameL8();				// zjištìní pøepínaèe hry 8
bool FGameHost();				// zjištìní pøíznaku hostitele
bool FGetGameDataL();			// naètení pøepínaèe z paketu

// porty
bool FGetComStop();				// zjištìní stopbitù
bool FComIn();					// test pøijatého znaku
bool FGetComSend();				// test povolení vysílání
bool FGetComReceive();			// test povolení pøíjmu

bool FGetDialogGraph();			// zjištìní módu grafického pozadí

// Direct3D
bool FGetD3DWireframe();		// zjištìní nastavení vyplòování ploch
bool FGetD3DLighton();			// zjištìní nastavení osvìtlení
bool FGetD3DShades();			// zjištìní nastavení plynulého stínování
bool FGetD3DMinFilter();		// zjištìní filtrace zmenšených textur
bool FGetD3DMagFilter();		// zjištìní filtrace zvìtšených textur
bool FGetD3DMipFilter();		// zjištìní filtrace vzdálených textur
bool FGetD3DOWireframe();		// zjištìní nastavení vyplòování ploch objektu
bool FGetD3DOLighton();			// zjištìní nastavení osvìtlení objektu
bool FGetD3DOShades();			// zjištìní nastavení plynulého stínování objektu
bool FGetD3DOMinFilter();		// zjištìní filtrace zmenšených textur objektu
bool FGetD3DOMagFilter();		// zjištìní filtrace zvìtšených textur objektu
bool FGetD3DOMipFilter();		// zjištìní filtrace vzdálených textur objektu
bool FGetD3DVisible();			// zjištìní viditelnosti objektu Direct3D
bool FGetD3DUpper();			// tvoøit horní podstavu
bool FGetD3DLower();			// tvoøit dolní podstavu
bool FGetD3DZWrite();			// zjištìní hloubkového zápisu
bool FGetD3DZSort();			// zjištìní hloubkového tøídìní

bool FGetActive();				// test, zda to je aktivní aplikace

bool FPicPrint();				// tisk obrázku

// obsluha DLL
bool FGetMemoryBool();			// logická hodnota
