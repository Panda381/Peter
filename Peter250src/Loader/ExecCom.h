
/***************************************************************************\
*																			*
*							Prov�d�n� programu - p��kaz						*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inline vol�n� p��kazu

inline void FCommand()
{
	ExecItem++;
	ExecItem[-1].ExecCom();
}

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

// nastaven� transformac� objektu
//void SetTransform(D3DFITEM* frame);

/////////////////////////////////////////////////////////////////////////////
// automatick� mapov�n� textury

void D3DTextMap(D3DFITEM* item);


#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------


// v�tven� CASE
void FCaseReal();				// CASE s ��slem
void FCaseBool();				// CASE s logick�m v�razem
void FCaseIcon();				// CASE s ikonou
void FCaseSprite();				// CASE se spritem
void FCaseMap();				// CASE s plochou
void FCaseMusic();				// CASE s hudbou
void FCaseText();				// CASE s textem
void FCaseSound();				// CASE se zvukem
void FCasePicture();			// CASE s obr�zkem

// funkce
void FFunc();					// funkce s lok�ln�mi prom�nn�mi
void FFunc0();					// funkce bez lok�ln�ch prom�nn�ch
void FDupList();				// vytvo�en� lok�ln�ho seznamu duplikac� (Data = index vzoru)
void FDelList();				// zru�en� lok�ln�ho seznamu (Data = po�et)
void FDupReal();				// vytvo�en� lok�ln� ��seln� prom�nn� duplikac� (Data = index vzoru)
void FDupRealList();			// vytvo�en� lok�ln� ��seln� prom�nn� seznamu (Data = index vzoru, List = po�et)
void FNewReal();				// vytvo�en� lok�ln� ��seln� prom�nn� s inicializac�
void FDelReal();				// zru�en� lok�ln�ch ��seln�ch prom�nn�ch (Data = po�et)
void FDupBool();				// vytvo�en� lok�ln� logick� prom�nn� duplikac� (Data = index vzoru)
void FDupBoolList();			// vytvo�en� lok�ln� logick� prom�nn� seznamu (Data = index vzoru, List = po�et)
void FNewBool();				// vytvo�en� lok�ln� logick� prom�nn� s inicializac�
void FDelBool();				// zru�en� lok�ln�ch logick�ch prom�nn�ch (Data = po�et)
void FDupIcon();				// vytvo�en� lok�ln� prom�nn� ikony duplikac� (Data = index vzoru)
void FDupIconList();			// vytvo�en� lok�ln� prom�nn� ikony seznamu (Data = index vzoru, List = po�et)
void FNewIcon();				// vytvo�en� lok�ln� prom�nn� ikony s inicializac�
void FDelIcon();				// zru�en� lok�ln�ch prom�nn�ch ikon (Data = po�et)
void FDupSprite();				// vytvo�en� lok�ln� prom�nn� sprajtu duplikac� (Data = index vzoru)
void FDupSpriteList();			// vytvo�en� lok�ln� prom�nn� sprajtu seznamu (Data = index vzoru, List = po�et)
void FNewSprite();				// vytvo�en� lok�ln� prom�nn� sprajtu s inicializac�
void FDelSprite();				// zru�en� lok�ln�ch prom�nn�ch sprajt� (Data = po�et)
void FDupMap();					// vytvo�en� lok�ln� prom�nn� plochy duplikac� (Data = index vzoru)
void FDupMapList();				// vytvo�en� lok�ln� prom�nn� plochy seznamu (Data = index vzoru, List = po�et)
void FNewMap();					// vytvo�en� lok�ln� prom�nn� plochy s inicializac�
void FDelMap();					// zru�en� lok�ln�ch prom�nn�ch ploch (Data = po�et)
void FDupText();				// vytvo�en� lok�ln� prom�nn� textu duplikac� (Data = index vzoru)
void FDupTextList();			// vytvo�en� lok�ln� prom�nn� textu seznamu (Data = index vzoru, List = po�et)
void FNewText();				// vytvo�en� lok�ln� prom�nn� textu s inicializac�
void FDelText();				// zru�en� lok�ln�ch prom�nn�ch text� (Data = po�et)
void FDupSound();				// vytvo�en� lok�ln� prom�nn� zvuku duplikac� (Data = index vzoru)
void FDupSoundList();			// vytvo�en� lok�ln� prom�nn� zvuku seznamu (Data = index vzoru, List = po�et)
void FNewSound();				// vytvo�en� lok�ln� prom�nn� zvuku s inicializac�
void FDelSound();				// zru�en� lok�ln�ch prom�nn�ch zvuk� (Data = po�et)
void FDupPicture();				// vytvo�en� lok�ln� prom�nn� obr�zku duplikac� (Data = index vzoru)
void FDupPictureList();			// vytvo�en� lok�ln� prom�nn� obr�zku seznamu (Data = index vzoru, List = po�et)
void FNewPicture();				// vytvo�en� lok�ln� prom�nn� obr�zku s inicializac�
void FDelPicture();				// zru�en� lok�ln�ch prom�nn�ch obr�zk� (Data = po�et)
void FDupMusic();				// vytvo�en� lok�ln� prom�nn� hudby duplikac� (Data = index vzoru)
void FDupMusicList();			// vytvo�en� lok�ln� prom�nn� hudby seznamu (Data = index vzoru, List = po�et)
void FNewMusic();				// vytvo�en� lok�ln� prom�nn� hudby s inicializac�
void FDelMusic();				// zru�en� lok�ln�ch prom�nn�ch hudby (Data = po�et)

// nastaven� prom�nn�ch
void FSetList();				// nastaven� indexu glob�ln�ho seznamu
void FSetListLoc();				// nastaven� indexu lok�ln�ho seznamu
void FSetListAuto();			// nastaven� inkrementace glob�ln�ho seznamu
void FSetListAutoLoc();			// nastaven� inkrementace lok�ln�ho seznamu
void FSetIcon();				// nastaven� glob�ln� ikony (Data = index)
void FSetIconList();			// nastaven� ikony v seznamu (Data = index, List = seznam)
void FSetIconLoc();				// nastaven� lok�ln� ikony (Data = index)
void FSetIconLocList();			// nastaven� lok�ln� ikony v seznamu (Data = index, List = seznam)
void FSetLog();					// nastaven� glob�ln� logick� prom�nn� (Data = index)
void FSetLogList();				// nastaven� logick� prom�nn� v seznamu (Data = index, List = seznam)
void FSetLogLoc();				// nastaven� lok�ln� logick� prom�nn� (Data = index)
void FSetLogLocList();			// nastaven� lok�ln� logick� prom�nn� v seznamu (Data = index, List = seznam)
void FSetMap();					// nastaven� glob�ln� plochy (Data = index)
void FSetMapList();				// nastaven� plochy v seznamu (Data = index, List = seznam)
void FSetMapLoc();				// nastaven� lok�ln� plochy (Data = index)
void FSetMapLocList();			// nastaven� lok�ln� plochy v seznamu (Data = index, List = seznam)
void FSetNum();					// nastaven� glob�ln�ho ��sla (Data = index)
void FSetNumList();				// nastaven� ��sla v seznamu (Data = index, List = seznam)
void FSetNumLoc();				// nastaven� lok�ln�ho ��sla (Data = index)
void FSetNumLocList();			// nastaven� lok�ln�ho ��sla v seznamu (Data = index, List = seznam)
void FSetPic();					// nastaven� glob�ln�ho obr�zku (Data = index)
void FSetPicList();				// nastaven� obr�zku v seznamu (Data = index, List = seznam)
void FSetPicLoc();				// nastaven� lok�ln�ho obr�zku (Data = index)
void FSetPicLocList();			// nastaven� lok�ln�ho obr�zku v seznamu (Data = index, List = seznam)
void FSetSound();				// nastaven� glob�ln�ho zvuku (Data = index)
void FSetSoundList();			// nastaven� zvuku v seznamu (Data = index, List = seznam)
void FSetSoundLoc();			// nastaven� lok�ln�ho zvuku (Data = index)
void FSetSoundLocList();		// nastaven� lok�ln�ho zvuku v seznamu (Data = index, List = seznam)
void FSetMusic();				// nastaven� glob�ln� hudby (Data = index)
void FSetMusicList();			// nastaven� hudby v seznamu (Data = index, List = seznam)
void FSetMusicLoc();			// nastaven� lok�ln� hudby (Data = index)
void FSetMusicLocList();		// nastaven� lok�ln� hudby v seznamu (Data = index, List = seznam)
void FSetText();				// nastaven� glob�ln�ho textu (Data = index)
void FSetTextList();			// nastaven� textu v seznamu (Data = index, List = seznam)
void FSetTextLoc();				// nastaven� lok�ln�ho textu (Data = index)
void FSetTextLocList();			// nastaven� lok�ln�ho textu v seznamu (Data = index, List = seznam)
void FSetSprite();				// nastaven� glob�ln�ho sprajtu (Data = index)
void FSetSpriteList();			// nastaven� sprajtu v seznamu (Data = index, List = seznam)
void FSetSpriteLoc();			// nastaven� lok�ln�ho sprajtu (Data = index)
void FSetSpriteLocList();		// nastaven� lok�ln�ho sprajtu v seznamu (Data = index, List = seznam)
void FSpriteX();				// nastaven� sou�adnice X glob�ln�ho sprajtu (Data = index)
void FSpriteXList();			// nastaven� sou�adnice X sprajtu v seznamu (Data = index, List = seznam)
void FSpriteXLoc();				// nastaven� sou�adnice X lok�ln�ho sprajtu (Data = index)
void FSpriteXLocList();			// nastaven� sou�adnice X lok�ln�ho sprajtu v seznamu (Data = index, List = seznam)
void FSpriteY();				// nastaven� sou�adnice Y glob�ln�ho sprajtu (Data = index)
void FSpriteYList();			// nastaven� sou�adnice Y sprajtu v seznamu (Data = index, List = seznam)
void FSpriteYLoc();				// nastaven� sou�adnice Y lok�ln�ho sprajtu (Data = index)
void FSpriteYLocList();			// nastaven� sou�adnice Y lok�ln�ho sprajtu v seznamu (Data = index, List = seznam)
void FSpriteSmer();				// nastaven� sm�ru glob�ln�ho sprajtu (Data = index)
void FSpriteSmerList();			// nastaven� sm�ru sprajtu v seznamu (Data = index, List = seznam)
void FSpriteSmerLoc();			// nastaven� sm�ru lok�ln�ho sprajtu (Data = index)
void FSpriteSmerLocList();		// nastaven� sm�ru lok�ln�ho sprajtu v seznamu (Data = index, List = seznam)
void FSpriteFaze();				// nastaven� f�ze glob�ln�ho sprajtu (Data = index)
void FSpriteFazeList();			// nastaven� f�ze sprajtu v seznamu (Data = index, List = seznam)
void FSpriteFazeLoc();			// nastaven� f�ze lok�ln�ho sprajtu (Data = index)
void FSpriteFazeLocList();		// nastaven� f�ze lok�ln�ho sprajtu v seznamu (Data = index, List = seznam)
void FSpriteStatus();			// nastaven� f�ze glob�ln�ho sprajtu (Data = index)
void FSpriteStatusList();		// nastaven� f�ze sprajtu v seznamu (Data = index, List = seznam)
void FSpriteStatusLoc();		// nastaven� f�ze lok�ln�ho sprajtu (Data = index)
void FSpriteStatusLocList();	// nastaven� f�ze lok�ln�ho sprajtu v seznamu (Data = index, List = seznam)
void FSpriteVis();				// nastaven� viditelnosti glob�ln�ho sprajtu (Data = index)
void FSpriteVisList();			// nastaven� viditelnosti sprajtu v seznamu (Data = index, List = seznam)
void FSpriteVisLoc();			// nastaven� viditelnosti lok�ln�ho sprajtu (Data = index)
void FSpriteVisLocList();		// nastaven� viditelnosti lok�ln�ho sprajtu v seznamu (Data = index, List = seznam)

// ��dic� p��kazy
void FIf();						// podm�nka IF
void FWhile();					// cyklus WHILE
void FRepeat();					// cyklus REPEAT
void FBreak();					// p�eru�en� cyklu
void FReturn();					// p�eru�en� funkce
void FCommandExec0();			// pr�zdn� p��kaz
void FCommandExec1();			// 1 p��kaz
void FCommandExec();			// posloupnost p��kaz� (Data = po�et p��kaz� - 1)
void FWait1();					// �ek�n� - 1 impuls
void FWait();					// �ek�n�
void FSetStatus();				// nastaven� textu stavov�ho ��dku
void FSetCaption();				// nastaven� textu nadpisu okna

// obsluha dialog�
void FDialogBox();				// dialog box

// obsluha soubor�
void FFileNew();				// nov� soubor
void FDirNew();					// nov� adres��
void FFileDelete();				// zru�en� souboru/adres��e
void FFileAct();				// nastaven� aktivn�ho adres��e
void FDiskLabel();				// nastaven� jmenovky disku
void FFileRead();				// soubor pro �ten�
void FFileWrite();				// soubor pro z�pis
void FFileCopy();				// kopie soubor�
void FFileMove();				// p�esun soubor�
void FFileRO();					// nastaven� atributu R/O
void FFileHID();				// nastaven� atributu HID
void FFileSYS();				// nastaven� atributu SYS
void FFileARC();				// nastaven� atributu ARC
void FFileCreate();				// nastaven� �asu vytvo�en� souboru
void FFileOpen();				// nastaven� �asu �ten� ze souboru
void FFileModi();				// nastaven� �asu z�pisu do souboru
void FFileSize();				// nastaven� velikosti souboru
void FFileReadOff();			// obsluha soubor�
void FFileWriteOff();			// nastaven� offsetu pro z�pis
void FFileError();				// nastaven� p��znaku chyby
void FFileTextNLen();			// nastaven� d�lky textu s pevnou d�lkou
void FFileByteS();				// z�pis bajtu se znam�nkem
void FFileByteU();				// z�pis bajtu bez znam�nka
void FFileWordS();				// z�pis slova se znam�nkem
void FFileWordU();				// z�pis slova bez znam�nka
void FFile3ByteS();				// z�pis 3 bajt� se znam�nkem
void FFile3ByteU();				// z�pis 3 bajt� bez znam�nka
void FFileDWordS();				// z�pis dvojslova se znam�nkem
void FFileDWordU();				// z�pis dvojslova bez znam�nka
void FFileFloat();				// z�pis ��sla FLOAT
void FFileDouble();				// z�pis ��sla DOUBLE
void FFileLogic();				// z�pis logick� hodnoty
void FFileText0();				// z�pis textu zakon�en�ho nulou
void FFileTextCR();				// z�pis textu zakon�en�ho CR/LF
void FFileTextN();				// z�pis textu s pevnou d�lkou
void FFileTextUnicode();		// nastaven� p��znaku textu UNICODE
void FFileIcon();				// z�pis ikony
void FFilePicture();			// z�pis obr�zku
void FFileSound();				// z�pis zvuku
void FFileMusic();				// z�pis hudby
void FFileSprite();				// z�pis sprajtu

// obsluha �asu
void FTimeCurrent();			// nastaven� aktu�ln�ho �asu

// obsluha CD disku
void FCDPlay();					// p�ehr�t� CD 
void FCDStop();					// zastaven� p�ehr�v�n� CD
void FCDPause();				// pauza CD
void FCDPauseSet();				// pauza CD
void FCDSetPos();				// nastaven� pozice CD
void FCDSetTrack();				// nastaven� stopy CD
void FCDSetTrackPos();			// nastaven� pozice stopy CD
void FCDEject();				// vysunut� CD
void FCDEjectSet();				// vysunut� CD

// obsluha hudby
void FMusicPlay();				// p�ehr�t� hudby
void FMusicLoop();				// opakovan� p�ehr�v�n� hudby
void FMusicStop();				// zastaven� p�ehr�v�n� hudby
void FMusicPause();				// pauza hudby se zm�nou
void FMusicPauseSet();			// pauza hudby s nastaven�m
void FSetMusicPos();			// nastaven� pozice hudby

// obsluha Petra a Petry
//void PeterWait();				// Petr - �ek�n� na dokon�en� pohybu
//void Peter2Wait();				// Petra - �ek�n� na dokon�en� pohybu
void FKrok();					// Petr - krok vp�ed
void FKrok2();					// Petra - krok vp�ed
void FVlevo();					// Petr - vlevo
void FVlevo2();					// Petra - vlevo
void FVpravo();					// Petr - vpravo
void FVpravo2();				// Petra - vpravo
void FVzad();					// Petr - vzad
void FVzad2();					// Petra - vzad
void FVisible();				// Petr - viditelnost
void FVisible2();				// Petra - viditelnost
void FSetPredmet();				// nastaven� p�edm�tu p�ed Petrem
void FSetPredmet2();			// nastaven� p�edm�tu p�ed Petrou
void FSetPredmetPoz();			// nastaven� p�edm�tu pod Petrem
void FSetPredmet2Poz();			// nastaven� p�edm�tu pod Petrou
void FSetSmer();				// Petr - nastaven� sm�ru
void FSetSmer2();				// Petra - nastaven� sm�ru
void FSetPozX();				// Petr - nastaven� sou�adnice X
void FSetPozX2();				// Petra - nastaven� sou�adnice X
void FSetPozY();				// Petr - nastaven� sou�adnice Y
void FSetPozY2();				// Petra - nastaven� sou�adnice Y
void FSetSwcN1();				// Petr - nastaven� ��sla 1
void FSetSwcN2();				// Petr - nastaven� ��sla 2
void FSetSwcN3();				// Petr - nastaven� ��sla 3
void FSetSwcN12();				// Petr2 - nastaven� ��sla 1
void FSetSwcN22();				// Petr2 - nastaven� ��sla 2
void FSetSwcN32();				// Petr2 - nastaven� ��sla 3
void FSetSwc1();				// Petr - nastaven� p�ep�na�e 1
void FSetSwc2();				// Petr - nastaven� p�ep�na�e 2
void FSetSwc3();				// Petr - nastaven� p�ep�na�e 3
void FSetSwc4();				// Petr - nastaven� p�ep�na�e 4
void FSetSwc5();				// Petr - nastaven� p�ep�na�e 5
void FSetSwc12();				// Petra - nastaven� p�ep�na�e 1
void FSetSwc22();				// Petra - nastaven� p�ep�na�e 2
void FSetSwc32();				// Petra - nastaven� p�ep�na�e 3
void FSetSwc42();				// Petra - nastaven� p�ep�na�e 4
void FSetSwc52();				// Petra - nastaven� p�ep�na�e 5

// obsluha zvuku
void FSoundPlay();				// p�ehr�t� zvuku
void FSoundStop();				// zastaven� v�ech zvuk�
void FSoundStop1();				// zastaven� konkr�tn�ho zvuku
void FSetSoundPos();			// nastaven� pozice zvuku
void FSoundPause();				// pauza zvuku
void FSoundPauseSet();			// pauza zvuku
void FSetSoundDistinguish();	// nastaven� rozli�ovac�ho k�du

// obsluha grafiky
extern int PointWidth;			// nastaven� ���ka grafick�ho bodu
extern BYTE PointCol;			// nastaven� barva grafick�ho bodu
void _fastcall CirclePoint(int x, int y); // zobrazen� bodu nebo kruhu
void FPoint();					// zobrazen� bodu
void FLine();					// zobrazen� ��ry
void FBox();					// zobrazen� obd�ln�ku
void FBoxFill();				// zobrazen� vypln�n�ho obd�ln�ku
void FFill();					// v�pl�
void FCircle();					// kru�nice
void FCircleFill();				// kruh
void FSphere();					// koule
void FTriangle();				// vypln�n� troj�heln�k
void FGraphText();				// zobrazen� textu
void FSetGraphAreaWidth();		// nastaven� ���ky pole k zobrazen� centrovan�ho textu
void FPicDisp();				// zobrazen� obr�zku
void FTransparent();			// zpr�hledn�n� v��ezu

// obsluha kl�vesnice
void FComVKeyWait();			// �ek�n� na kl�vesu
void FComCharWait();			// �ek�n� na snak z kl�vesnice
void FKlavFlush();				// vypr�zdn�n� bufferu kl�ves
void FSetNumLock();				// nastaven� p�ep�na�e Num Lock
void FSetCapsLock();			// nastaven� p�ep�na�e Caps Lock
void FSetScrollLock();			// nastaven� p�ep�na�e Scroll Lock
void FSetInsertLock();			// nastaven� p�ep�na�e Insert

void FSpriteShiftX();			// hromadn� posun sprajt� ve sm�ru X
void FSpriteShiftY();			// hromadn� posun sprajt� ve sm�ru Y

// okno k zobrazen� sprajt�
void FSpriteWinX1();
void FSpriteWinY1();
void FSpriteWinX2();
void FSpriteWinY2();

// konzola
void FConsoleOn();				// re�im konzoly
void FConsoleOut();				// v�stup textu na konzolu
void FConsoleErr();				// v�stup chybov�ho textu na konzolu

void FSetReturnCode();			// n�vratov� k�d

void FExec();					// spu�t�n� programu
void FExecWait();				// spu�t�n� programu

// dialog
//void FDialogOn();				// re�im dialogu
//void FDialogOnSet();
void FWindowID();				// ��slo prvku
void FWindowDel();				// zru�en� okna
void FWindowText();				// text okna
void FWindowX();				// pozice okna X
void FWindowY();				// pozice okna Y
void FWindowW();				// ���ka okna
void FWindowH();				// v��ka okna
void FWindowWInt();				// vnit�n� ���ka okna
void FWindowHInt();				// vnit�n� v��ka okna
void FWindowVisible();			// viditelnost okna
void FWindowVisibleSet();		// viditelnost okna
void FWindowDisable();			// z�kaz okna
void FWindowDisableSet();		// z�kaz okna
void FCheckBoxOn();				// p�ep�na�
void FCheckBoxOnSet();			// p�ep�na�
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
void FListBoxAdd();				// p�id�n� polo�ky do seznamu
void FListBoxClear();			// vypr�zdn�n� seznamu
void FWindowIcon();				// ikona okna
void FWindowPic();				// obr�zek okna
void FSetButtonEsc();			// tla��tko pro kl�vesu Esc
void FSetWindowPos();			// nastaven� pozice v prvku okna
void FSetWindowPos2();			// nastaven� pozice 2 v prvku okna
void FSetTableWidth();			// ���ka sloupce tabulky
void FSetTableAlign();			// zarovn�n� sloupce tabulky

void FSetWindowFrame();			// nastaven� r�me�ku okna
void FSetWindowCaption();		// nastaven� nadpisu okna
void FSetWindowTop();			// nastaven� okna v�dy naho�e
void FSetWindowSize();			// nastaven� okna s m�nitelnou velikost�
void FSetWindowMayClose();		// nastaven� povolen� uzav�en� okna
void FSetWindowMaxim();			// maximalizace okna
void FSetWindowMinim();			// minimalizace okna

void FFullScreen();				// celoobrazokov� m�d
void FFullScreenSet();			// celoobrazokov� m�d

void FMouseCursor();			// kurzor my�i

void FSetMouseX();				// nastaven� sou�adnice X my�i
void FSetMouseY();				// nastaven� sou�adnice Y my�i

/////////////////////////////////////////////////////////////////////////////
// obsluha kurzoru my�i (vrac� TRUE = kurzor my�i nastaven)

BOOL OnSetCursor(int x, int y);

// DirectPlay
void FSetConnectAct();			// nastaven� aktivn�ho spojen�
void FSetGameAct();				// nastaven� aktivn� hry
void FSetPlayerAct();			// nastaven� aktivn�ho hr��e

void FSetGameN1();				// nastaven� ��seln� hodnoty hry 1
void FSetGameN2();				// nastaven� ��seln� hodnoty hry 2
void FSetGameN3();				// nastaven� ��seln� hodnoty hry 3
void FSetGameN4();				// nastaven� ��seln� hodnoty hry 4
void FSetGameN5();				// nastaven� ��seln� hodnoty hry 5
void FSetGameN6();				// nastaven� ��seln� hodnoty hry 6
void FSetGameN7();				// nastaven� ��seln� hodnoty hry 7
void FSetGameN8();				// nastaven� ��seln� hodnoty hry 8

void FSetGameL1();				// nastaven� p�ep�na�e hry 1
void FSetGameL2();				// nastaven� p�ep�na�e hry 2
void FSetGameL3();				// nastaven� p�ep�na�e hry 3
void FSetGameL4();				// nastaven� p�ep�na�e hry 4
void FSetGameL5();				// nastaven� p�ep�na�e hry 5
void FSetGameL6();				// nastaven� p�ep�na�e hry 6
void FSetGameL7();				// nastaven� p�ep�na�e hry 7
void FSetGameL8();				// nastaven� p�ep�na�e hry 8

void FSetPlayerMax();			// nastaven� max. po�tu hr���
void FSetGamePacket();			// vysl�n� datov�ho paketu
void FSetGamePacket0();			// vysl�n� datov�ho paketu (v�em)
void FSetGameDataN();			// vysl�n� ��sla paketem
void FSetGameDataT();			// vysl�n� textu paketem (nen� pro UNICODE !!!)
void FSetGameDataL();			// vysl�n� p�ep�na�e paketem
void FSetGameDataB();			// vysl�n� bajtu paketem

// UDP
void FSetUDPSendIP();			// nastaven� vys�lac� IP adresy UDP
void FSetUDPSendPort();			// nastaven� vys�lac�ho portu UDP
void FSetUDPRecSize();			// nastaven� velikosti p�ij�mac�ho bufferu UDP (0=zav�en� p��jmu)
void FSetUDPRecIP();			// nastaven� p�ij�mac� IP adresy UDP
void FSetUDPRecPort();			// nastaven� p�ij�mac�ho portu UDP
void FSetUDPMem();				// odesl�n� bin�rn�ho paketu UDP
void FSetUDPText();				// odesl�n� textov�ho paketu UDP

void FSetCodePage();			// nastaven� znakov� sady
void FSetUserFont();			// nastaven� u�ivatelsk�ho fontu
void FSetLanguage();			// nastaven� jazyku

void FSetAlias();				// nastaven� alias

// porty
void FSetComID();				// nastaven� aktivn�ho portu
void FComClose();				// uzav�en� portu
void FSetComBaud();				// nastaven� p�enosov� rychlosti portu
void FSetComStop();				// nastaven� stopbit�
void FSetComBits();				// nastaven� po�tu bit�
void FSetComParit();			// nastaven� parity
void FSetComText();				// vysl�n� znaku
void FSetComNum();				// vysl�n� ��sla
void FSetComSend();				// povolen� vys�l�n�
void FSetComReceive();			// povolen� p��jmu

// mixer
void FSetMixDevice();			// nastaven� aktivn�ho mixeru
void FSetMixLineDst();			// nastaven� c�lov�ho sign�lu
void FSetMixLineSrc();			// nastaven� zdrojov�ho sign�lu
void FSetLineType();			// nastaven� typu sign�lu
void FSetMixControl();			// nastaven� ovl�dac�ho prvku
void FSetCtrlType();			// nastaven� typu ovl�dac�ho prvku
void FSetCtrlVal();				// nastaven� hodnoty ovl�dac�ho prvku
void FSetMixChannel();			// nastaven� kan�lu ovl�dac�ho prvku
void FSetMixCtrlList();			// v�b�r hodnoty ovl�dac�ho prvku

// Direct3D
void FSet3DMode();				// nastaven� videom�du Direct3D
void FDirect3DView();			// nastaven� okna Direct3D

void FSetD3DWireframe();		// nastaven� m�du v�pln� Direct3D
void FSetD3DLighton();			// zapnut� osv�tlen� Direct3D
void FSetD3DShades();			// nastaven� m�du st�nov�n� Direct3D
void FD3DInvert();				// inverze objektu
void FSetD3DMinFilter();		// zapnut� filtrace p�i zmen�en�
void FSetD3DMagFilter();		// zapnut� filtrace p�i zv�t�en�
void FSetD3DMipFilter();		// zapnut� filtrace p�i vzd�len�
void FSetD3DOWireframe();		// nastaven� m�du v�pln� objektu Direct3D
void FSetD3DOLighton();			// zapnut� osv�tlen� objektu Direct3D
void FSetD3DOShades();			// nastaven� m�du st�nov�n� objektu Direct3D
void FSetD3DOMinFilter();		// zapnut� filtrace p�i zmen�en� textury objektu
void FSetD3DOMagFilter();		// zapnut� filtrace p�i zv�t�en� textury objektu
void FSetD3DOMipFilter();		// zapnut� filtrace p�i vzd�len� textury objektu
void FSetD3DOCulling();			// nastaven� odstra�ov�n� ploch objektu

void FSetD3DVisible();			// nastaven� viditelnosti objektu Direct3D
void FSetD3DObjectID();			// nastaven� aktivn�ho objektu Direct3D
void FD3DObjectDelete();		// zru�en� objektu Direct3D

void FSetD3DScaleX();			// nastaven� m���tka ve sm�ru X
void FSetD3DScaleY();			// nastaven� m���tka ve sm�ru Y
void FSetD3DScaleZ();			// nastaven� m���tka ve sm�ru Z
void FSetD3DRotateX();			// nastaven� rotace podle osy X
void FSetD3DRotateY();			// nastaven� rotace podle osy Y
void FSetD3DRotateZ();			// nastaven� rotace podle osy Z
void FSetD3DTransX();			// nastaven� posunu ve sm�ru X
void FSetD3DTransY();			// nastaven� posunu ve sm�ru Y
void FSetD3DTransZ();			// nastaven� posunu ve sm�ru Z
void FSetD3DColor();			// nastaven� barvy objektu
void FSetD3DAmbiRef();			// nastaven� odrazu rozpt�len�ho sv�tla
void FSetD3DEmissive();			// nastaven� barvy sv�tivosti
void FSetD3DPower();			// nastaven� matnosti
void FSetD3DSpecular();			// nastaven� barvy odlesku
void FSetD3DTexture();			// nastaven� textury objektu Direct3D
void FD3DTextureFile();			// na�ten� textury objektu Direct3D ze souboru
void FD3DTextMap();				// mapov�n� textury
void FSetD3DLightColor();		// nastaven� barvy sv�tla
void FSetD3DLightType();		// nastaven� typu sv�tla
void FSetD3DLightUmbra();		// nastaven� �hlu st�nu sv�tla
void FSetD3DLightPenumbra();	// nastaven� �hlu polost�nu sv�tla
void FSetD3DLightRange();		// nastaven� dosahu sv�tla
void FSetD3DLightCatten();		// nastaven� konstantn�ho �tlumu sv�tla
void FSetD3DLightLatten();		// nastaven� line�rn�ho �tlumu sv�tla
void FSetD3DLightQatten();		// nastaven� kvadratick�ho �tlumu sv�tla
void FSetD3DProjection();		// nastaven� projekce
void FD3DLink();				// p�ipojen� jin�ho objektu
//void FD3DSave();				// ulo�en� objektu do souboru
void FSetD3DSceneColor();		// nastaven� barvy pozad� sc�ny
void FSetD3DAmbient();			// nastaven� ambient sv�tla
void FSetD3DSceneTexture();		// nastaven� textury pozad� sc�ny
void FSetD3DFogColor();			// nastaven� barvy mlhy
void FSetD3DFogMode();			// nastaven� m�du mlhy
void FSetD3DFogStart();			// nastaven� za��tku mlhy
void FSetD3DFogEnd();			// nastaven� konce mlhy
void FSetD3DFogDensity();		// nastaven� hustoty mlhy
void FSetD3DViewFront();		// nastaven� prom�tac� roviny
void FSetD3DViewBack();			// nastaven� vzd�lenosti dohledu
void FSetD3DUpper();			// vytv��et horn� podstavu
void FSetD3DLower();			// vytv��et doln� podstavu
void FSetD3DSplits();			// nastaven� slo�itosti objekt�
void FSetD3DOrder();			// nastaven� po�ad� rotac� objektu
void FSetD3DDriver();			// nastaven� ovlada�e
void FSetD3DInterface();		// nastaven� rozhran�
void FSetD3DMinFilter();		// zapnut� filtrace textur p�i zmen�en�
//void FSetD3DPersp();			// nastaven� korekce perspektivy
void FSetD3DSmooth();			// nastaven� vyhlazen� ditheringu textur
void FD3DNorm();				// normalizace objektu
void FSetD3DTransparent();		// nastaven� pr�hlednosti
void FSetD3DAlphaRef();			// nastaven� alpfa referen�n� �rovn�
void FSetD3DStage();			// nastaven� aktivn�ho stupn� textur
void FSetD3DColorOp();			// nastaven� operace barev
void FSetD3DColorArg1();		// nastaven� argumentu 1 barev
void FSetD3DColorArg2();		// nastaven� argumentu 2 barev
void FSetD3DAlphaOp();			// nastaven� operace alfa
void FSetD3DAlphaArg1();		// nastaven� argumentu 1 barev
void FSetD3DAlphaArg2();		// nastaven� argumentu 2 barev
void FSetD3DAddressU();			// nastaven� horizont�ln�ho adresov�n�
void FSetD3DAddressV();			// nastaven� vertik�ln�ho adresov�n�
//void FSetD3DBorder();			// nastaven� barvy okol� textury
void FSetD3DBias();				// nastaven� zjemn�n� vzd�len�ch textur
void FSetD3DTFactor();			// nastaven� barvy faktoru textur
void FSetD3DMipMaps();			// po�et �rovn� mipmaps
void FSetD3DZGroup();			// nastaven� renderovac� skupiny
void FSetD3DZSort();			// nastaven� t��d�n�
void FSetD3DZWrite();			// nastaven� hloubkov�ho z�pisu
void FSetD3DZTest();			// nastaven� hloubkov�ho testu
void FSetD3DShadowRange();		// nastaven� dosahu st�nu
void FD3DNormals();				// generov�n� norm�l
void FD3DNormals0();			// generov�n� norm�l (implicitn� parametr)
void FD3DRender();				// renderov�n� bez zobrazen�
void FSetD3DLODObject();		// nastaven� objektu pro LOD
void FSetD3DLODDistance();		// nastaven� vzd�lenosti pro LOD
void FSetD3DMorphModel();		// nastaven� vzoru morfov�n�
void FSetD3DMorphLevel();		// nastaven� stupn� morfov�n�

// obsluha DDE
void FSetDDEApp();				// nastaven� jm�na aplikace DDE

void FSetActive();				// nastaven� aktivn� aplikace
void FSetDialogGraph();			// nastaven� grafick�ho pozad�

void FPicPrint2();				// tisk obr�zku

// obsluha DLL
void FDLLFree();				// uvoln�n� DLL knihovny
void FDLLExecCom();				// proveden� DLL funkce
void FMemoryFree();				// zru�en� bloku pam�ti
void FSetMemoryRead();			// ukazatel �ten� z pam�ti
void FSetMemoryWrite();			// ukazatel z�pisu do pam�ti
void FMemoryCopy();				// kopie dat
void FSetMemoryByteS();			// bajt se znam�nkem
inline void FSetMemoryByteU() { FSetMemoryByteS(); } // bajt bez znam�nka
void FSetMemoryWordS();			// slovo se znam�nkem
inline void FSetMemoryWordU() { FSetMemoryWordS(); }	// slovo bez znam�nka
void FSetMemory3ByteS();		// 3-bajt se znam�nkem
inline void FSetMemory3ByteU() { FSetMemory3ByteS(); } // 3-bajt bez znam�nka
void FSetMemoryDWordS();		// dvouslovo se znam�nkem
void FSetMemoryDWordU();		// dvouslovo bez znam�nka
void FSetMemoryFloat();			// re�ln� ��slo 4 bajty
void FSetMemoryDouble();		// re�ln� ��slo 8 bajt�
void FSetMemoryBool();			// logick� hodnota
void FSetMemoryText0();			// text zakon�en� nulou
void FSetMemoryText0P();		// ukazatel na text zakon�en� nulou
void FSetMemoryTextN();			// text s pevnou d�lkou
void FSetMemoryTextNLen();		// d�lka textu s pevnou d�lkou
void FMemoryLoad();				// na�ten� bloku dat
void FMemorySave();				// z�pis bloku dat

// konfigurace
void FSetIniFile();				// jm�no konfigura�n�ho souboru
void FSetIniSection();			// jm�no konfigura�n� sekce
void FSetIniValue();			// jm�no konfigura�n�ho parametru
void FSetIniDel();				// zru�en� konfigura�n�ho parametru
void FSetIniString();			// z�pis konfigura�n�ho textu
void FSetIniNum();				// z�pis konfigura�n�ho ��sla
void FSetRegKey();				// skupina kl��� registr�
void FSetRegSubkey();			// kl�� registr�
void FSetRegValue();			// jm�no polo�ky registru
void FSetRegDel();				// zru�en� registru
void FSetRegString();			// z�pis textu do registru
void FSetRegNum();				// z�pis ��sla do registru
