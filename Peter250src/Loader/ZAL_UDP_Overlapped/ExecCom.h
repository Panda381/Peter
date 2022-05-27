
/***************************************************************************\
*																			*
*							Provádìní programu - pøíkaz						*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inline volání pøíkazu

inline void FCommand()
{
	ExecItem++;
	ExecItem[-1].ExecCom();
}

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

// nastavení transformací objektu
//void SetTransform(D3DFITEM* frame);

/////////////////////////////////////////////////////////////////////////////
// automatické mapování textury

void D3DTextMap(D3DFITEM* item);


#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------


// vìtvení CASE
void FCaseReal();				// CASE s èíslem
void FCaseBool();				// CASE s logickým výrazem
void FCaseIcon();				// CASE s ikonou
void FCaseSprite();				// CASE se spritem
void FCaseMap();				// CASE s plochou
void FCaseMusic();				// CASE s hudbou
void FCaseText();				// CASE s textem
void FCaseSound();				// CASE se zvukem
void FCasePicture();			// CASE s obrázkem

// funkce
void FFunc();					// funkce s lokálními promìnnými
void FFunc0();					// funkce bez lokálních promìnných
void FDupList();				// vytvoøení lokálního seznamu duplikací (Data = index vzoru)
void FDelList();				// zrušení lokálního seznamu (Data = poèet)
void FDupReal();				// vytvoøení lokální èíselné promìnné duplikací (Data = index vzoru)
void FDupRealList();			// vytvoøení lokální èíselné promìnné seznamu (Data = index vzoru, List = poèet)
void FNewReal();				// vytvoøení lokální èíselné promìnné s inicializací
void FDelReal();				// zrušení lokálních èíselných promìnných (Data = poèet)
void FDupBool();				// vytvoøení lokální logické promìnné duplikací (Data = index vzoru)
void FDupBoolList();			// vytvoøení lokální logické promìnné seznamu (Data = index vzoru, List = poèet)
void FNewBool();				// vytvoøení lokální logické promìnné s inicializací
void FDelBool();				// zrušení lokálních logických promìnných (Data = poèet)
void FDupIcon();				// vytvoøení lokální promìnné ikony duplikací (Data = index vzoru)
void FDupIconList();			// vytvoøení lokální promìnné ikony seznamu (Data = index vzoru, List = poèet)
void FNewIcon();				// vytvoøení lokální promìnné ikony s inicializací
void FDelIcon();				// zrušení lokálních promìnných ikon (Data = poèet)
void FDupSprite();				// vytvoøení lokální promìnné sprajtu duplikací (Data = index vzoru)
void FDupSpriteList();			// vytvoøení lokální promìnné sprajtu seznamu (Data = index vzoru, List = poèet)
void FNewSprite();				// vytvoøení lokální promìnné sprajtu s inicializací
void FDelSprite();				// zrušení lokálních promìnných sprajtù (Data = poèet)
void FDupMap();					// vytvoøení lokální promìnné plochy duplikací (Data = index vzoru)
void FDupMapList();				// vytvoøení lokální promìnné plochy seznamu (Data = index vzoru, List = poèet)
void FNewMap();					// vytvoøení lokální promìnné plochy s inicializací
void FDelMap();					// zrušení lokálních promìnných ploch (Data = poèet)
void FDupText();				// vytvoøení lokální promìnné textu duplikací (Data = index vzoru)
void FDupTextList();			// vytvoøení lokální promìnné textu seznamu (Data = index vzoru, List = poèet)
void FNewText();				// vytvoøení lokální promìnné textu s inicializací
void FDelText();				// zrušení lokálních promìnných textù (Data = poèet)
void FDupSound();				// vytvoøení lokální promìnné zvuku duplikací (Data = index vzoru)
void FDupSoundList();			// vytvoøení lokální promìnné zvuku seznamu (Data = index vzoru, List = poèet)
void FNewSound();				// vytvoøení lokální promìnné zvuku s inicializací
void FDelSound();				// zrušení lokálních promìnných zvukù (Data = poèet)
void FDupPicture();				// vytvoøení lokální promìnné obrázku duplikací (Data = index vzoru)
void FDupPictureList();			// vytvoøení lokální promìnné obrázku seznamu (Data = index vzoru, List = poèet)
void FNewPicture();				// vytvoøení lokální promìnné obrázku s inicializací
void FDelPicture();				// zrušení lokálních promìnných obrázkù (Data = poèet)
void FDupMusic();				// vytvoøení lokální promìnné hudby duplikací (Data = index vzoru)
void FDupMusicList();			// vytvoøení lokální promìnné hudby seznamu (Data = index vzoru, List = poèet)
void FNewMusic();				// vytvoøení lokální promìnné hudby s inicializací
void FDelMusic();				// zrušení lokálních promìnných hudby (Data = poèet)

// nastavení promìnných
void FSetList();				// nastavení indexu globálního seznamu
void FSetListLoc();				// nastavení indexu lokálního seznamu
void FSetListAuto();			// nastavení inkrementace globálního seznamu
void FSetListAutoLoc();			// nastavení inkrementace lokálního seznamu
void FSetIcon();				// nastavení globální ikony (Data = index)
void FSetIconList();			// nastavení ikony v seznamu (Data = index, List = seznam)
void FSetIconLoc();				// nastavení lokální ikony (Data = index)
void FSetIconLocList();			// nastavení lokální ikony v seznamu (Data = index, List = seznam)
void FSetLog();					// nastavení globální logické promìnné (Data = index)
void FSetLogList();				// nastavení logické promìnné v seznamu (Data = index, List = seznam)
void FSetLogLoc();				// nastavení lokální logické promìnné (Data = index)
void FSetLogLocList();			// nastavení lokální logické promìnné v seznamu (Data = index, List = seznam)
void FSetMap();					// nastavení globální plochy (Data = index)
void FSetMapList();				// nastavení plochy v seznamu (Data = index, List = seznam)
void FSetMapLoc();				// nastavení lokální plochy (Data = index)
void FSetMapLocList();			// nastavení lokální plochy v seznamu (Data = index, List = seznam)
void FSetNum();					// nastavení globálního èísla (Data = index)
void FSetNumList();				// nastavení èísla v seznamu (Data = index, List = seznam)
void FSetNumLoc();				// nastavení lokálního èísla (Data = index)
void FSetNumLocList();			// nastavení lokálního èísla v seznamu (Data = index, List = seznam)
void FSetPic();					// nastavení globálního obrázku (Data = index)
void FSetPicList();				// nastavení obrázku v seznamu (Data = index, List = seznam)
void FSetPicLoc();				// nastavení lokálního obrázku (Data = index)
void FSetPicLocList();			// nastavení lokálního obrázku v seznamu (Data = index, List = seznam)
void FSetSound();				// nastavení globálního zvuku (Data = index)
void FSetSoundList();			// nastavení zvuku v seznamu (Data = index, List = seznam)
void FSetSoundLoc();			// nastavení lokálního zvuku (Data = index)
void FSetSoundLocList();		// nastavení lokálního zvuku v seznamu (Data = index, List = seznam)
void FSetMusic();				// nastavení globální hudby (Data = index)
void FSetMusicList();			// nastavení hudby v seznamu (Data = index, List = seznam)
void FSetMusicLoc();			// nastavení lokální hudby (Data = index)
void FSetMusicLocList();		// nastavení lokální hudby v seznamu (Data = index, List = seznam)
void FSetText();				// nastavení globálního textu (Data = index)
void FSetTextList();			// nastavení textu v seznamu (Data = index, List = seznam)
void FSetTextLoc();				// nastavení lokálního textu (Data = index)
void FSetTextLocList();			// nastavení lokálního textu v seznamu (Data = index, List = seznam)
void FSetSprite();				// nastavení globálního sprajtu (Data = index)
void FSetSpriteList();			// nastavení sprajtu v seznamu (Data = index, List = seznam)
void FSetSpriteLoc();			// nastavení lokálního sprajtu (Data = index)
void FSetSpriteLocList();		// nastavení lokálního sprajtu v seznamu (Data = index, List = seznam)
void FSpriteX();				// nastavení souøadnice X globálního sprajtu (Data = index)
void FSpriteXList();			// nastavení souøadnice X sprajtu v seznamu (Data = index, List = seznam)
void FSpriteXLoc();				// nastavení souøadnice X lokálního sprajtu (Data = index)
void FSpriteXLocList();			// nastavení souøadnice X lokálního sprajtu v seznamu (Data = index, List = seznam)
void FSpriteY();				// nastavení souøadnice Y globálního sprajtu (Data = index)
void FSpriteYList();			// nastavení souøadnice Y sprajtu v seznamu (Data = index, List = seznam)
void FSpriteYLoc();				// nastavení souøadnice Y lokálního sprajtu (Data = index)
void FSpriteYLocList();			// nastavení souøadnice Y lokálního sprajtu v seznamu (Data = index, List = seznam)
void FSpriteSmer();				// nastavení smìru globálního sprajtu (Data = index)
void FSpriteSmerList();			// nastavení smìru sprajtu v seznamu (Data = index, List = seznam)
void FSpriteSmerLoc();			// nastavení smìru lokálního sprajtu (Data = index)
void FSpriteSmerLocList();		// nastavení smìru lokálního sprajtu v seznamu (Data = index, List = seznam)
void FSpriteFaze();				// nastavení fáze globálního sprajtu (Data = index)
void FSpriteFazeList();			// nastavení fáze sprajtu v seznamu (Data = index, List = seznam)
void FSpriteFazeLoc();			// nastavení fáze lokálního sprajtu (Data = index)
void FSpriteFazeLocList();		// nastavení fáze lokálního sprajtu v seznamu (Data = index, List = seznam)
void FSpriteStatus();			// nastavení fáze globálního sprajtu (Data = index)
void FSpriteStatusList();		// nastavení fáze sprajtu v seznamu (Data = index, List = seznam)
void FSpriteStatusLoc();		// nastavení fáze lokálního sprajtu (Data = index)
void FSpriteStatusLocList();	// nastavení fáze lokálního sprajtu v seznamu (Data = index, List = seznam)
void FSpriteVis();				// nastavení viditelnosti globálního sprajtu (Data = index)
void FSpriteVisList();			// nastavení viditelnosti sprajtu v seznamu (Data = index, List = seznam)
void FSpriteVisLoc();			// nastavení viditelnosti lokálního sprajtu (Data = index)
void FSpriteVisLocList();		// nastavení viditelnosti lokálního sprajtu v seznamu (Data = index, List = seznam)

// øídicí pøíkazy
void FIf();						// podmínka IF
void FWhile();					// cyklus WHILE
void FRepeat();					// cyklus REPEAT
void FBreak();					// pøerušení cyklu
void FReturn();					// pøerušení funkce
void FCommandExec0();			// prázdný pøíkaz
void FCommandExec1();			// 1 pøíkaz
void FCommandExec();			// posloupnost pøíkazù (Data = poèet pøíkazù - 1)
void FWait1();					// èekání - 1 impuls
void FWait();					// èekání
void FSetStatus();				// nastavení textu stavového øádku
void FSetCaption();				// nastavení textu nadpisu okna

// obsluha dialogù
void FDialogBox();				// dialog box

// obsluha souborù
void FFileNew();				// nový soubor
void FDirNew();					// nový adresáø
void FFileDelete();				// zrušení souboru/adresáøe
void FFileAct();				// nastavení aktivního adresáøe
void FDiskLabel();				// nastavení jmenovky disku
void FFileRead();				// soubor pro ètení
void FFileWrite();				// soubor pro zápis
void FFileCopy();				// kopie souborù
void FFileMove();				// pøesun souborù
void FFileRO();					// nastavení atributu R/O
void FFileHID();				// nastavení atributu HID
void FFileSYS();				// nastavení atributu SYS
void FFileARC();				// nastavení atributu ARC
void FFileCreate();				// nastavení èasu vytvoøení souboru
void FFileOpen();				// nastavení èasu ètení ze souboru
void FFileModi();				// nastavení èasu zápisu do souboru
void FFileSize();				// nastavení velikosti souboru
void FFileReadOff();			// obsluha souborù
void FFileWriteOff();			// nastavení offsetu pro zápis
void FFileError();				// nastavení pøíznaku chyby
void FFileTextNLen();			// nastavení délky textu s pevnou délkou
void FFileByteS();				// zápis bajtu se znaménkem
void FFileByteU();				// zápis bajtu bez znaménka
void FFileWordS();				// zápis slova se znaménkem
void FFileWordU();				// zápis slova bez znaménka
void FFile3ByteS();				// zápis 3 bajtù se znaménkem
void FFile3ByteU();				// zápis 3 bajtù bez znaménka
void FFileDWordS();				// zápis dvojslova se znaménkem
void FFileDWordU();				// zápis dvojslova bez znaménka
void FFileFloat();				// zápis èísla FLOAT
void FFileDouble();				// zápis èísla DOUBLE
void FFileLogic();				// zápis logické hodnoty
void FFileText0();				// zápis textu zakonèeného nulou
void FFileTextCR();				// zápis textu zakonèeného CR/LF
void FFileTextN();				// zápis textu s pevnou délkou
void FFileTextUnicode();		// nastavení pøíznaku textu UNICODE
void FFileIcon();				// zápis ikony
void FFilePicture();			// zápis obrázku
void FFileSound();				// zápis zvuku
void FFileMusic();				// zápis hudby
void FFileSprite();				// zápis sprajtu

// obsluha èasu
void FTimeCurrent();			// nastavení aktuálního èasu

// obsluha CD disku
void FCDPlay();					// pøehrátí CD 
void FCDStop();					// zastavení pøehrávání CD
void FCDPause();				// pauza CD
void FCDPauseSet();				// pauza CD
void FCDSetPos();				// nastavení pozice CD
void FCDSetTrack();				// nastavení stopy CD
void FCDSetTrackPos();			// nastavení pozice stopy CD
void FCDEject();				// vysunutí CD
void FCDEjectSet();				// vysunutí CD

// obsluha hudby
void FMusicPlay();				// pøehrátí hudby
void FMusicLoop();				// opakované pøehrávání hudby
void FMusicStop();				// zastavení pøehrávání hudby
void FMusicPause();				// pauza hudby se zmìnou
void FMusicPauseSet();			// pauza hudby s nastavením
void FSetMusicPos();			// nastavení pozice hudby

// obsluha Petra a Petry
//void PeterWait();				// Petr - èekání na dokonèení pohybu
//void Peter2Wait();				// Petra - èekání na dokonèení pohybu
void FKrok();					// Petr - krok vpøed
void FKrok2();					// Petra - krok vpøed
void FVlevo();					// Petr - vlevo
void FVlevo2();					// Petra - vlevo
void FVpravo();					// Petr - vpravo
void FVpravo2();				// Petra - vpravo
void FVzad();					// Petr - vzad
void FVzad2();					// Petra - vzad
void FVisible();				// Petr - viditelnost
void FVisible2();				// Petra - viditelnost
void FSetPredmet();				// nastavení pøedmìtu pøed Petrem
void FSetPredmet2();			// nastavení pøedmìtu pøed Petrou
void FSetPredmetPoz();			// nastavení pøedmìtu pod Petrem
void FSetPredmet2Poz();			// nastavení pøedmìtu pod Petrou
void FSetSmer();				// Petr - nastavení smìru
void FSetSmer2();				// Petra - nastavení smìru
void FSetPozX();				// Petr - nastavení souøadnice X
void FSetPozX2();				// Petra - nastavení souøadnice X
void FSetPozY();				// Petr - nastavení souøadnice Y
void FSetPozY2();				// Petra - nastavení souøadnice Y
void FSetSwcN1();				// Petr - nastavení èísla 1
void FSetSwcN2();				// Petr - nastavení èísla 2
void FSetSwcN3();				// Petr - nastavení èísla 3
void FSetSwcN12();				// Petr2 - nastavení èísla 1
void FSetSwcN22();				// Petr2 - nastavení èísla 2
void FSetSwcN32();				// Petr2 - nastavení èísla 3
void FSetSwc1();				// Petr - nastavení pøepínaèe 1
void FSetSwc2();				// Petr - nastavení pøepínaèe 2
void FSetSwc3();				// Petr - nastavení pøepínaèe 3
void FSetSwc4();				// Petr - nastavení pøepínaèe 4
void FSetSwc5();				// Petr - nastavení pøepínaèe 5
void FSetSwc12();				// Petra - nastavení pøepínaèe 1
void FSetSwc22();				// Petra - nastavení pøepínaèe 2
void FSetSwc32();				// Petra - nastavení pøepínaèe 3
void FSetSwc42();				// Petra - nastavení pøepínaèe 4
void FSetSwc52();				// Petra - nastavení pøepínaèe 5

// obsluha zvuku
void FSoundPlay();				// pøehrátí zvuku
void FSoundStop();				// zastavení všech zvukù
void FSoundStop1();				// zastavení konkrétního zvuku
void FSetSoundPos();			// nastavení pozice zvuku
void FSoundPause();				// pauza zvuku
void FSoundPauseSet();			// pauza zvuku
void FSetSoundDistinguish();	// nastavení rozlišovacího kódu

// obsluha grafiky
extern int PointWidth;			// nastavená šíøka grafického bodu
extern BYTE PointCol;			// nastavená barva grafického bodu
void _fastcall CirclePoint(int x, int y); // zobrazení bodu nebo kruhu
void FPoint();					// zobrazení bodu
void FLine();					// zobrazení èáry
void FBox();					// zobrazení obdélníku
void FBoxFill();				// zobrazení vyplnìného obdélníku
void FFill();					// výplò
void FCircle();					// kružnice
void FCircleFill();				// kruh
void FSphere();					// koule
void FTriangle();				// vyplnìný trojùhelník
void FGraphText();				// zobrazení textu
void FSetGraphAreaWidth();		// nastavení šíøky pole k zobrazení centrovaného textu
void FPicDisp();				// zobrazení obrázku
void FTransparent();			// zprùhlednìní výøezu

// obsluha klávesnice
void FComVKeyWait();			// èekání na klávesu
void FComCharWait();			// èekání na snak z klávesnice
void FKlavFlush();				// vyprázdnìní bufferu kláves
void FSetNumLock();				// nastavení pøepínaèe Num Lock
void FSetCapsLock();			// nastavení pøepínaèe Caps Lock
void FSetScrollLock();			// nastavení pøepínaèe Scroll Lock
void FSetInsertLock();			// nastavení pøepínaèe Insert

void FSpriteShiftX();			// hromadný posun sprajtù ve smìru X
void FSpriteShiftY();			// hromadný posun sprajtù ve smìru Y

// okno k zobrazení sprajtù
void FSpriteWinX1();
void FSpriteWinY1();
void FSpriteWinX2();
void FSpriteWinY2();

// konzola
void FConsoleOn();				// režim konzoly
void FConsoleOut();				// výstup textu na konzolu
void FConsoleErr();				// výstup chybového textu na konzolu

void FSetReturnCode();			// návratový kód

void FExec();					// spuštìní programu
void FExecWait();				// spuštìní programu

// dialog
//void FDialogOn();				// režim dialogu
//void FDialogOnSet();
void FWindowID();				// èíslo prvku
void FWindowDel();				// zrušení okna
void FWindowText();				// text okna
void FWindowX();				// pozice okna X
void FWindowY();				// pozice okna Y
void FWindowW();				// šíøka okna
void FWindowH();				// výška okna
void FWindowWInt();				// vnitøní šíøka okna
void FWindowHInt();				// vnitøní výška okna
void FWindowVisible();			// viditelnost okna
void FWindowVisibleSet();		// viditelnost okna
void FWindowDisable();			// zákaz okna
void FWindowDisableSet();		// zákaz okna
void FCheckBoxOn();				// pøepínaè
void FCheckBoxOnSet();			// pøepínaè
void FButton3On();
void FButton3OnSet();
void FFocus();					// okno s fokusem
void FFontBold();
void FFontBoldSet();
void FFontFixed();
void FFontFixedSet();
void FFontHeightSet();
void FFontItalic();
void FFontItalicSet();
void FFontUnder();
void FFontUnderSet();
void FFontStrike();
void FFontStrikeSet();
void FFontSerif();
void FFontSerifSet();
void FFontWidthSet();
void FFontAngleSet();
void FFontColSet();
void FFontBackSet();
void FListBoxAdd();				// pøidání položky do seznamu
void FListBoxClear();			// vyprázdnìní seznamu
void FWindowIcon();				// ikona okna
void FWindowPic();				// obrázek okna
void FSetButtonEsc();			// tlaèítko pro klávesu Esc
void FSetWindowPos();			// nastavení pozice v prvku okna
void FSetWindowPos2();			// nastavení pozice 2 v prvku okna
void FSetTableWidth();			// šíøka sloupce tabulky
void FSetTableAlign();			// zarovnání sloupce tabulky

void FSetWindowFrame();			// nastavení rámeèku okna
void FSetWindowCaption();		// nastavení nadpisu okna
void FSetWindowTop();			// nastavení okna vždy nahoøe
void FSetWindowSize();			// nastavení okna s mìnitelnou velikostí
void FSetWindowMayClose();		// nastavení povolení uzavøení okna
void FSetWindowMaxim();			// maximalizace okna
void FSetWindowMinim();			// minimalizace okna

void FFullScreen();				// celoobrazokový mód
void FFullScreenSet();			// celoobrazokový mód

void FMouseCursor();			// kurzor myši

void FSetMouseX();				// nastavení souøadnice X myši
void FSetMouseY();				// nastavení souøadnice Y myši

/////////////////////////////////////////////////////////////////////////////
// obsluha kurzoru myši (vrací TRUE = kurzor myši nastaven)

BOOL OnSetCursor(int x, int y);

// DirectPlay
void FSetConnectAct();			// nastavení aktivního spojení
void FSetGameAct();				// nastavení aktivní hry
void FSetPlayerAct();			// nastavení aktivního hráèe

void FSetGameN1();				// nastavení èíselné hodnoty hry 1
void FSetGameN2();				// nastavení èíselné hodnoty hry 2
void FSetGameN3();				// nastavení èíselné hodnoty hry 3
void FSetGameN4();				// nastavení èíselné hodnoty hry 4
void FSetGameN5();				// nastavení èíselné hodnoty hry 5
void FSetGameN6();				// nastavení èíselné hodnoty hry 6
void FSetGameN7();				// nastavení èíselné hodnoty hry 7
void FSetGameN8();				// nastavení èíselné hodnoty hry 8

void FSetGameL1();				// nastavení pøepínaèe hry 1
void FSetGameL2();				// nastavení pøepínaèe hry 2
void FSetGameL3();				// nastavení pøepínaèe hry 3
void FSetGameL4();				// nastavení pøepínaèe hry 4
void FSetGameL5();				// nastavení pøepínaèe hry 5
void FSetGameL6();				// nastavení pøepínaèe hry 6
void FSetGameL7();				// nastavení pøepínaèe hry 7
void FSetGameL8();				// nastavení pøepínaèe hry 8

void FSetPlayerMax();			// nastavení max. poètu hráèù
void FSetGamePacket();			// vyslání datového paketu
void FSetGamePacket0();			// vyslání datového paketu (všem)
void FSetGameDataN();			// vyslání èísla paketem
void FSetGameDataT();			// vyslání textu paketem (není pro UNICODE !!!)
void FSetGameDataL();			// vyslání pøepínaèe paketem
void FSetGameDataB();			// vyslání bajtu paketem

// UDP
void FSetUDPSendIP();			// nastavení vysílací IP adresy UDP
void FSetUDPSendPort();			// nastavení vysílacího portu UDP
void FSetUDPRecSize();			// nastavení velikosti pøijímacího bufferu UDP (0=zavøení pøíjmu)
void FSetUDPRecIP();			// nastavení pøijímací IP adresy UDP
void FSetUDPRecPort();			// nastavení pøijímacího portu UDP
void FSetUDPMem();				// odeslání binárního paketu UDP
void FSetUDPText();				// odeslání textového paketu UDP

void FSetCodePage();			// nastavení znakové sady
void FSetUserFont();			// nastavení uživatelského fontu
void FSetLanguage();			// nastavení jazyku

void FSetAlias();				// nastavení alias

// porty
void FSetComID();				// nastavení aktivního portu
void FComClose();				// uzavøení portu
void FSetComBaud();				// nastavení pøenosové rychlosti portu
void FSetComStop();				// nastavení stopbitù
void FSetComBits();				// nastavení poètu bitù
void FSetComParit();			// nastavení parity
void FSetComText();				// vyslání znaku
void FSetComNum();				// vyslání èísla
void FSetComSend();				// povolení vysílání
void FSetComReceive();			// povolení pøíjmu

// mixer
void FSetMixDevice();			// nastavení aktivního mixeru
void FSetMixLineDst();			// nastavení cílového signálu
void FSetMixLineSrc();			// nastavení zdrojového signálu
void FSetLineType();			// nastavení typu signálu
void FSetMixControl();			// nastavení ovládacího prvku
void FSetCtrlType();			// nastavení typu ovládacího prvku
void FSetCtrlVal();				// nastavení hodnoty ovládacího prvku
void FSetMixChannel();			// nastavení kanálu ovládacího prvku
void FSetMixCtrlList();			// výbìr hodnoty ovládacího prvku

// Direct3D
void FSet3DMode();				// nastavení videomódu Direct3D
void FDirect3DView();			// nastavení okna Direct3D

void FSetD3DWireframe();		// nastavení módu výplnì Direct3D
void FSetD3DLighton();			// zapnutí osvìtlení Direct3D
void FSetD3DShades();			// nastavení módu stínování Direct3D
void FD3DInvert();				// inverze objektu
void FSetD3DMinFilter();		// zapnutí filtrace pøi zmenšení
void FSetD3DMagFilter();		// zapnutí filtrace pøi zvìtšení
void FSetD3DMipFilter();		// zapnutí filtrace pøi vzdálení
void FSetD3DOWireframe();		// nastavení módu výplnì objektu Direct3D
void FSetD3DOLighton();			// zapnutí osvìtlení objektu Direct3D
void FSetD3DOShades();			// nastavení módu stínování objektu Direct3D
void FSetD3DOMinFilter();		// zapnutí filtrace pøi zmenšení textury objektu
void FSetD3DOMagFilter();		// zapnutí filtrace pøi zvìtšení textury objektu
void FSetD3DOMipFilter();		// zapnutí filtrace pøi vzdálení textury objektu
void FSetD3DOCulling();			// nastavení odstraòování ploch objektu

void FSetD3DVisible();			// nastavení viditelnosti objektu Direct3D
void FSetD3DObjectID();			// nastavení aktivního objektu Direct3D
void FD3DObjectDelete();		// zrušení objektu Direct3D

void FSetD3DScaleX();			// nastavení mìøítka ve smìru X
void FSetD3DScaleY();			// nastavení mìøítka ve smìru Y
void FSetD3DScaleZ();			// nastavení mìøítka ve smìru Z
void FSetD3DRotateX();			// nastavení rotace podle osy X
void FSetD3DRotateY();			// nastavení rotace podle osy Y
void FSetD3DRotateZ();			// nastavení rotace podle osy Z
void FSetD3DTransX();			// nastavení posunu ve smìru X
void FSetD3DTransY();			// nastavení posunu ve smìru Y
void FSetD3DTransZ();			// nastavení posunu ve smìru Z
void FSetD3DColor();			// nastavení barvy objektu
void FSetD3DAmbiRef();			// nastavení odrazu rozptýleného svìtla
void FSetD3DEmissive();			// nastavení barvy svítivosti
void FSetD3DPower();			// nastavení matnosti
void FSetD3DSpecular();			// nastavení barvy odlesku
void FSetD3DTexture();			// nastavení textury objektu Direct3D
void FD3DTextureFile();			// naètení textury objektu Direct3D ze souboru
void FD3DTextMap();				// mapování textury
void FSetD3DLightColor();		// nastavení barvy svìtla
void FSetD3DLightType();		// nastavení typu svìtla
void FSetD3DLightUmbra();		// nastavení úhlu stínu svìtla
void FSetD3DLightPenumbra();	// nastavení úhlu polostínu svìtla
void FSetD3DLightRange();		// nastavení dosahu svìtla
void FSetD3DLightCatten();		// nastavení konstantního útlumu svìtla
void FSetD3DLightLatten();		// nastavení lineárního útlumu svìtla
void FSetD3DLightQatten();		// nastavení kvadratického útlumu svìtla
void FSetD3DProjection();		// nastavení projekce
void FD3DLink();				// pøipojení jiného objektu
//void FD3DSave();				// uložení objektu do souboru
void FSetD3DSceneColor();		// nastavení barvy pozadí scény
void FSetD3DAmbient();			// nastavení ambient svìtla
void FSetD3DSceneTexture();		// nastavení textury pozadí scény
void FSetD3DFogColor();			// nastavení barvy mlhy
void FSetD3DFogMode();			// nastavení módu mlhy
void FSetD3DFogStart();			// nastavení zaèátku mlhy
void FSetD3DFogEnd();			// nastavení konce mlhy
void FSetD3DFogDensity();		// nastavení hustoty mlhy
void FSetD3DViewFront();		// nastavení promítací roviny
void FSetD3DViewBack();			// nastavení vzdálenosti dohledu
void FSetD3DUpper();			// vytváøet horní podstavu
void FSetD3DLower();			// vytváøet dolní podstavu
void FSetD3DSplits();			// nastavení složitosti objektù
void FSetD3DOrder();			// nastavení poøadí rotací objektu
void FSetD3DDriver();			// nastavení ovladaèe
void FSetD3DInterface();		// nastavení rozhraní
void FSetD3DMinFilter();		// zapnutí filtrace textur pøi zmenšení
//void FSetD3DPersp();			// nastavení korekce perspektivy
void FSetD3DSmooth();			// nastavení vyhlazení ditheringu textur
void FD3DNorm();				// normalizace objektu
void FSetD3DTransparent();		// nastavení prùhlednosti
void FSetD3DAlphaRef();			// nastavení alpfa referenèní úrovnì
void FSetD3DStage();			// nastavení aktivního stupnì textur
void FSetD3DColorOp();			// nastavení operace barev
void FSetD3DColorArg1();		// nastavení argumentu 1 barev
void FSetD3DColorArg2();		// nastavení argumentu 2 barev
void FSetD3DAlphaOp();			// nastavení operace alfa
void FSetD3DAlphaArg1();		// nastavení argumentu 1 barev
void FSetD3DAlphaArg2();		// nastavení argumentu 2 barev
void FSetD3DAddressU();			// nastavení horizontálního adresování
void FSetD3DAddressV();			// nastavení vertikálního adresování
//void FSetD3DBorder();			// nastavení barvy okolí textury
void FSetD3DBias();				// nastavení zjemnìní vzdálených textur
void FSetD3DTFactor();			// nastavení barvy faktoru textur
void FSetD3DMipMaps();			// poèet úrovní mipmaps
void FSetD3DZGroup();			// nastavení renderovací skupiny
void FSetD3DZSort();			// nastavení tøídìní
void FSetD3DZWrite();			// nastavení hloubkového zápisu
void FSetD3DZTest();			// nastavení hloubkového testu
void FSetD3DShadowRange();		// nastavení dosahu stínu
void FD3DNormals();				// generování normál
void FD3DNormals0();			// generování normál (implicitní parametr)
void FD3DRender();				// renderování bez zobrazení
void FSetD3DLODObject();		// nastavení objektu pro LOD
void FSetD3DLODDistance();		// nastavení vzdálenosti pro LOD
void FSetD3DMorphModel();		// nastavení vzoru morfování
void FSetD3DMorphLevel();		// nastavení stupnì morfování

// obsluha DDE
void FSetDDEApp();				// nastavení jména aplikace DDE

void FSetActive();				// nastavení aktivní aplikace
void FSetDialogGraph();			// nastavení grafického pozadí

void FPicPrint2();				// tisk obrázku

// obsluha DLL
void FDLLFree();				// uvolnìní DLL knihovny
void FDLLExecCom();				// provedení DLL funkce
void FMemoryFree();				// zrušení bloku pamìti
void FSetMemoryRead();			// ukazatel ètení z pamìti
void FSetMemoryWrite();			// ukazatel zápisu do pamìti
void FMemoryCopy();				// kopie dat
void FSetMemoryByteS();			// bajt se znaménkem
inline void FSetMemoryByteU() { FSetMemoryByteS(); } // bajt bez znaménka
void FSetMemoryWordS();			// slovo se znaménkem
inline void FSetMemoryWordU() { FSetMemoryWordS(); }	// slovo bez znaménka
void FSetMemory3ByteS();		// 3-bajt se znaménkem
inline void FSetMemory3ByteU() { FSetMemory3ByteS(); } // 3-bajt bez znaménka
void FSetMemoryDWordS();		// dvouslovo se znaménkem
void FSetMemoryDWordU();		// dvouslovo bez znaménka
void FSetMemoryFloat();			// reálné èíslo 4 bajty
void FSetMemoryDouble();		// reálné èíslo 8 bajtù
void FSetMemoryBool();			// logická hodnota
void FSetMemoryText0();			// text zakonèený nulou
void FSetMemoryText0P();		// ukazatel na text zakonèený nulou
void FSetMemoryTextN();			// text s pevnou délkou
void FSetMemoryTextNLen();		// délka textu s pevnou délkou
void FMemoryLoad();				// naètení bloku dat
void FMemorySave();				// zápis bloku dat

// konfigurace
void FSetIniFile();				// jméno konfiguraèního souboru
void FSetIniSection();			// jméno konfiguraèní sekce
void FSetIniValue();			// jméno konfiguraèního parametru
void FSetIniDel();				// zrušení konfiguraèního parametru
void FSetIniString();			// zápis konfiguraèního textu
void FSetIniNum();				// zápis konfiguraèního èísla
void FSetRegKey();				// skupina klíèù registrù
void FSetRegSubkey();			// klíè registrù
void FSetRegValue();			// jméno položky registru
void FSetRegDel();				// zrušení registru
void FSetRegString();			// zápis textu do registru
void FSetRegNum();				// zápis èísla do registru
