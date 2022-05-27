
/***************************************************************************\
*																			*
*						Prov�d�n� programu - ��seln� v�raz					*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inline vol�n� p��kazu - ��slo

inline double FNum()
{
	ExecItem++;
	return ExecItem[-1].ExecNum();
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��seln�ho parametru p�eveden�ho na int

//int FInt();

/////////////////////////////////////////////////////////////////////////////
// na�ten� �hlu s normalizac� rozsahu 0 a� 2pi

double FAngle();


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��seln�ho parametru p�eveden�ho na int se zaokrouhlen�m

int FIntR();


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��seln�ho parametru p�eveden�ho na DWORD se zaokrouhlen�m

DWORD FIntRN();

/////////////////////////////////////////////////////////////////////////////
// na�ten� ��seln�ho parametru p�eveden�ho na INT64 se zaokrouhlen�m

hyper FIntHN();

/////////////////////////////////////////////////////////////////////////////
// na�ten� ��seln�ho parametru p�eveden�ho na grafickou sou�adnici X

int FIntX();


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��seln�ho parametru p�eveden�ho na grafickou sou�adnici Y

inline int FIntY() { return FIntX(); };


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��seln�ho parametru vyn�soben�ho * 1000 (�as v milisekund�ch)

int FInt1000();


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��seln�ho parametru vyn�soben�ho * 2 (pr�m�r kruhu)

int FIntX2();


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��seln�ho parametru vyn�soben�ho * 18.20648 (�as v �asov�ch impulsech)

int FInt18();


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��seln�ho parametru s vyn�soben�m * 255

BYTE FInt255();


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��seln�ho parametru p�eveden�ho na souborov� �as

__int64 FNumFileTime();


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��seln�ho parametru p�eveden�ho na syst�mov� �as

#define FILETIMEKONV 10000000		// p�evod na souborov� �as

extern	SYSTEMTIME	SystemTime;			// pomocn� prom�nn� pro obsluhu �asu
void FNumSysTime();


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��seln�ho parametru p�eveden�ho na sv�tov� souborov� �as

__int64 FNumFileTimeUTC();


/////////////////////////////////////////////////////////////////////////////
// na�ten� barvy

int FNumCol();


/////////////////////////////////////////////////////////////////////////////
// na�ten� sou�adnic Petra/Petry (ECX = adresa x, EDX = adresa y)

bool _fastcall PetrXY(int& x, int& y);
bool _fastcall Petr2XY(int& x, int& y);


/////////////////////////////////////////////////////////////////////////////
// na�ten� pozice Petra/Petry (NULL=neplatn�)

MAPITEM* PetrXYMap();
MAPITEM* Petr2XYMap();


/////////////////////////////////////////////////////////////////////////////
// na�ten� p��t� sou�adnice Petra/Petry (ECX = adresa x, EDX = adresa y)

bool _fastcall PetrXYNext(int& x, int& y);
bool _fastcall Petr2XYNext(int& x, int& y);


// funkce
double FNumFunc();					// funkce s n�vratem ��seln� hodnoty

// prom�nn�
double FNumConst();					// ��seln� konstanta
double FNumObj();					// glob�ln� prom�nn� (Data = index)
double FNumObjList();				// globaln� prom�nn� v seznamu (Data = index, List = seznam)
double FNumLoc();					// lok�ln� prom�nn� (Data = index)
double FNumLocList();				// lok�ln� prom�nn� v seznamu (Data = index, List = seznam)
double FGetList();					// index glob�ln�ho seznamu
double FGetListLoc();				// index lok�ln�ho seznamu
double FGetListAuto();				// inkrementace glob�ln�ho seznamu
double FGetListAutoLoc();			// inkrementace lok�ln�ho seznamu
double FGetSpriteX();				// sou�adnice X glob�ln�ho sprajtu (Data = index)
double FGetSpriteXList();			// sou�adnice X sprajtu v seznamu (Data = index, List = seznam)
double FGetSpriteXLoc();			// sou�adnice X lok�ln�ho sprajtu (Data = index)
double FGetSpriteXLocList();		// sou�adnice X lok�ln�ho sprajtu v seznamu (Data = index, List = seznam)
double FGetSpriteY();				// sou�adnice Y glob�ln�ho sprajtu (Data = index)
double FGetSpriteYList();			// sou�adnice Y sprajtu v seznamu (Data = index, List = seznam)
double FGetSpriteYLoc();			// sou�adnice Y lok�ln�ho sprajtu (Data = index)
double FGetSpriteYLocList();		// sou�adnice Y lok�ln�ho sprajtu v seznamu (Data = index, List = seznam)
double FGetSpriteSmer();			// sm�r glob�ln�ho sprajtu (Data = index)
double FGetSpriteSmerList();		// sm�r sprajtu v seznamu (Data = index, List = seznam)
double FGetSpriteSmerLoc();			// sm�r lok�ln�ho sprajtu (Data = index)
double FGetSpriteSmerLocList();		// sm�r lok�ln�ho sprajtu v seznamu (Data = index, List = seznam)
double FGetSpriteFaze();			// f�ze glob�ln�ho sprajtu (Data = index)
double FGetSpriteFazeList();		// f�ze sprajtu v seznamu (Data = index, List = seznam)
double FGetSpriteFazeLoc();			// f�ze lok�ln�ho sprajtu (Data = index)
double FGetSpriteFazeLocList();		// f�ze lok�ln�ho sprajtu v seznamu (Data = index, List = seznam)
double FGetSpriteStatus();			// stav glob�ln�ho sprajtu (Data = index)
double FGetSpriteStatusList();		// stav sprajtu v seznamu (Data = index, List = seznam)
double FGetSpriteStatusLoc();		// stav lok�ln�ho sprajtu (Data = index)
double FGetSpriteStatusLocList();	// stav lok�ln�ho sprajtu v seznamu (Data = index, List = seznam)

// obsluha Petra a Petry
double FGetSmer();					// Petr - sm�r
double FGetSmer2();					// Petra - sm�r
double FGetPozX();					// Petr - pozice X
double FGetPozX2();					// Petra - pozice X
double FGetPozY();					// Petr - pozice Y
double FGetPozY2();					// Petra - pozice Y
double FGetSwcN1();					// na�ten� ��sla 1 - Petr
double FGetSwcN12();				// na�ten� ��sla 1 - Petra
double FGetSwcN2();					// na�ten� ��sla 2 - Petr
double FGetSwcN22();				// na�ten� ��sla 2 - Petra
double FGetSwcN3();					// na�ten� ��sla 3 - Petr
double FGetSwcN32();				// na�ten� ��sla 3 - Petra

// v�po�ty
double FInc();						// inkrementace
double FDec();						// dekrementace
double FSin();						// sinus
double FCos();						// cosinus
double FTg();						// tangens
double FCoTg();						// cotangens
double FArcSin();					// arcsinus
double FArcCos();					// arccosinus
double FArcTg();					// arctangens
double FArcCoTg();					// arccotangens
double FNumInt();					// cel� ��slo
double FFrac();						// desetinn� ��st ��sla
double FAbs();						// absolutn� hodnota
double FSqrt();						// odmocnina
double FFact();						// faktori�l
double FRand();						// n�hodn� ��slo s n�sobkem
double FRand0();					// n�hodn� ��slo bez n�sobku
double FDegRad();					// p�evod stup�� na radi�ny
double FRadDeg();					// p�evod radi�n� na stupn�
double FLog();						// dekadick� logaritmus
double FLn();						// p�irozen� logaritmus
double FExp();						// p�irozen� exponent
double FAdd();						// oper�tor ADD
double FSub1();						// oper�tor SUB pro 1 prvek
double FSub();						// oper�tor SUB
double FMul();						// oper�tor MUL
double FDiv1();						// oper�tor DIV pro 1 prvek
double FDiv();						// oper�tor DIV
double FMod();						// oper�tor MOD
double FMocn();						// oper�tor MOCNINA
double FNumSqr();					// druh� mocnina

// obsluha grafiky
double FColorLevel();				// nastaven� jasu barvy
double FColorSynth();				// synt�za barvy
double FGetPoint();					// zji�t�n� barvy bodu
double FGetPicWidth();				// zji�t�n� ���ky obr�zku
double FGetPicHeight();				// zji�t�n� v��ky obr�zku
double FGetGraphAreaWidth();		// zji�t�n� ���ky pole k zobrazen� centrovan�ho textu

// obsluha �asu
double FGetTimeCurrent();			// na�ten� aktu�ln�ho �asu
double FTimeUtc();					// p�evod na sv�tov� �as
double FTimeLocal();				// p�evod na m�stn� �as
double FTimeComp();					// synt�za �asu
double FGetTimeYear();				// na�ten� roku z �asu
double FGetTimeMonth();				// na�ten� m�s�ce z �asu
double FGetTimeDay();				// na�ten� dne z �asu
double FGetTimeWeek();				// na�ten� dne v t�dnu z �asu
double FGetTimeHour();				// na�ten� hodiny z �asu
double FGetTimeMin();				// na�ten� minuty z �asu
double FGetTimeSec();				// na�ten� sekundy z �asu
double FGetTimeMSec();				// na�ten� milisekundy z �asu

// obsluha my�i
double FMouseX();					// na�ten� sou�adnice X my�i
double FMouseY();					// na�ten� sou�adnice Y my�i

// obsluha dialog�
double FNumDialogBox();				// dialog box s n�vratem hodnoty

// obsluha souboru
double FGetDiskSize();				// zji�t�n� velikosti disku
double FGetDiskFree();				// zji�t�n� voln�ho m�sta disku
double FGetDiskSerial();			// na�ten� s�riov�ho ��sla disku
double FGetFileCreate();			// �as vytvo�en� souboru
double FGetFileOpen();				// �as �ten� ze souboru
double FGetFileModi();				// �as z�pisu do souboru
double FGetFileReadOff();			// offset pro �ten� ze souboru
double FGetFileWriteOff();			// offset pro z�pis do souboru
double FGetFileSize();				// zji�t�n� velikosti souboru
double FGetFileTextNLen();			// d�lka textu s pevnou d�lkou
double FFileDiskType();				// na�ten� typu disku
double FGetFileByteS();				// na�ten� ��sla ze souboru bajt se znam�nkem
double FGetFileByteU();				// na�ten� ��sla ze souboru bajt bez znam�nka
double FGetFileWordS();				// na�ten� ��sla ze souboru slovo se znam�nkem
double FGetFileWordU();				// na�ten� ��sla ze souboru slovo bez znam�nka
double FGetFile3ByteS();			// na�ten� ��sla ze souboru 3 bajty se znam�nkem
double FGetFile3ByteU();			// na�ten� ��sla ze souboru 3 bajty bez znam�nka
double FGetFileDWordS();			// na�ten� ��sla ze souboru dvojslovo se znam�nkem
double FGetFileDWordU();			// na�ten� ��sla ze souboru dvojslovo bez znam�nka
double FGetFileFloat();				// na�ten� ��sla ze souboru FLOAT
double FGetFileDouble();			// na�ten� ��sla ze souboru DOUBLE

// obsluha CD disku
double FCDGetLength();				// zji�t�n� d�lky CD
double FCDGetTracks();				// zji�t�n� po�tu stop CD
double FCDGetPos();					// zji�t�n� pozice CD
double FCDGetTrack();				// zji�t�n� aktivn� stopy CD
double FCDGetTrackLength();			// zji�t�n� d�lky aktivn� stopy
double FCDGetTrackPos();			// zji�t�n� pozice aktivn� stopy CD

// obsluha hudby
double FMusicLen();					// d�lka p�ehr�van� hudby
double FGetMusicPos();				// pozice hudby

// obsluha zvuku
double FGetSoundBit();				// po�et bit� na vzorek
double FGetSoundFreq();				// vzorkovac� kmito�et
double FGetSoundLength();			// d�lka zvuku
double FGetSoundPos();				// pozice zvuku
double FGetSoundDistinguish();		// rozli�ovac� k�d

// obsluha text�
double FTextLen();					// d�lka textu
double FTextScan();					// p�evod textu na ��slo
double FTextFind();					// nalezen� textu
double FTextOrd();					// abecedn� po�ad� znaku
double FTextLineNum();				// zji�t�n� po�tu ��dk� textu

// obsluha kl�vesnice
double FVKey();						// na�ten� kl�vesy bez �ek�n�
double FVKeyWait();					// na�ten� kl�vesy s �ek�n�m

// plocha
double FGetMapWidth();				// ���ka plochy
double FGetMapWidth0();				// ���ka hlavn� plochy
double FGetMapHeight();				// v��ka plochy
double FGetMapHeight0();			// v��ka hlavn� plochy
double FGetMapNum1();				// na�ten� ��sla 1 z plochy
double FGetMapNum2();				// na�ten� ��sla 2 z plochy
double FGetMapNum3();				// na�ten� ��sla 3 z plochy

// na�ten� okna k zobrazen� sprajt�
double FGetSpriteWinX1();
double FGetSpriteWinY1();
double FGetSpriteWinX2();
double FGetSpriteWinY2();

double FGetReturnCode();			// n�vratov� k�d

double FGetExecWait();				// spu�t�n� programu

// joystick
double FJoystickX();				// pozice X
double FJoystickY();				// pozice Y
double FJoystickZ();				// pozice Z

// dialogy
double FGetWindowID();				// ID aktivn�ho prvku
double FWindowNew();				// vytvo�en� okna
double FWindowRONew();				// vytvo�en� okna
double FButtonNew();				// vytvo�en� tla��tka
double FGetButtonEsc();				// tla��tko Esc
double FDefButtonNew();				// vytvo�en� tla��tka
double FButtonIconNew();			// vytvo�en� ikonov�ho tla��tka
double FButton3New();				// vytvo�en� tla��tka
double FButtonCheckNew();			// vytvo�en� tla��tka
double FButtonCheckNew2();			// vytvo�en� p�ep�nac�ho tla��tka
double FEditNew();					// vytvo�en� edita�n�ho ��dku
double FButtonRadioNew();			// vytvo�en� tla��tka
double FButtonRadioNew2();			// vytvo�en� tla��tka
double FMemoNew();					// vytvo�en� edita�n�ho pole
double FRicheditNew();				// vytvo�en� editoru Richedit
double FRichMemoNew();				// vytvo�en� edita�n�ho pole se zalamov�n�m
double FGroupBoxNew();				// vytvo�en� skupiny
double FListBoxNew();				// vytvo�en� seznamu
double FTabsNew();					// vytvo�en� z�lo�ek
double FWinTextNew();				// vytvo�en� textu
double FWinTextNewCenter();			// vytvo�en� textu vyst�ed�n�ho
double FWinTextNewRight();			// vytvo�en� textu zarovnan�ho vpravo
double FListEditNew();				// vytvo�en� seznamu
double FComboBoxNew();				// vytvo�en� seznamu
double FComboEditNew();				// vytvo�en� seznamu
double FTableNew();					// vytvo�en� tabulky
double FGetTableWidth();			// zji�t�n� ���ky sloupce tabulky
double FGetTableAlign();			// zji�t�n� zarovn�n� sloupce tabulky
double FGetWindowX();				// po��tek okna vlevo
double FGetWindowY();				// po��tek okna dole
double FGetWindowW();				// ���ka okna
double FGetWindowH();				// v��ka okna
double FGetWindowWInt();			// vnit�n� ���ka okna
double FGetWindowHInt();			// vnit�n� v��ka okna
double FGetFontHeight();			// v��ka font�
double FGetFontWidth();				// ���ka font�
double FGetFontAngle();				// �hel textu
double FGetFontCol();				// barva textu
double FGetFontBack();				// barva pozad�
double FGetFocus();					// okno s fokusem
double FWinIconNew();				// vytvo�en� ikony
double FWinPicNew();				// vytvo�en� obr�zku
double FProgressNew();				// indik�tor
double FHTrackbarNew();				// t�hlo horizont�ln�
double FVTrackbarNew();				// t�hlo vertik�ln�
double FHScrollbarNew();			// horizont�ln� posuvn�k
double FVScrollbarNew();			// vertik�ln� posuvn�k
double FGetWindowPos();				// pozice v prvku
double FGetWindowPos2();			// pozice 2 v prvku

// DirectPlay
double FGetGameN1();				// zji�t�n� ��seln� hodnoty hry 1
double FGetGameN2();				// zji�t�n� ��seln� hodnoty hry 2
double FGetGameN3();				// zji�t�n� ��seln� hodnoty hry 3
double FGetGameN4();				// zji�t�n� ��seln� hodnoty hry 4
double FGetGameN5();				// zji�t�n� ��seln� hodnoty hry 5
double FGetGameN6();				// zji�t�n� ��seln� hodnoty hry 6
double FGetGameN7();				// zji�t�n� ��seln� hodnoty hry 7
double FGetGameN8();				// zji�t�n� ��seln� hodnoty hry 8
double FGetPlayerMax();				// zji�t�n� max. po�tu hr���
double FPlayerInx();				// zji�t�n� indexu hr��e
double FGetGamePacket();			// p��jem paketu
double FGetGameDataN();				// na�ten� ��sla z paketu
double FGetGameDataB();				// na�ten� bajtu z paketu

// UDP
double FGetUDPSendPort();			// zji�t�n� vys�lac�ho portu UDP
double FGetUDPRecSize();			// zji�t�n� velikosti p�ij�mac�ho bufferu UDP
double FGetUDPRecPort();			// zji�t�n� p�ij�mac�ho portu UDP
double FGetUDPMem();				// p��jem datov�ho bloku UDP

// bitov� operace
double FBitAnd();					// oper�tor BIT-AND (Data = po�et prvk� - 1)
double FBitOr();					// oper�tor BIT-OR (Data = po�et prvk� - 1)
double FBitXor();					// oper�tor BIT-XOR (Data = po�et prvk� - 1)
double FBitNot();					// bitov� dopln�k
double FBitLeft();					// rotace vlevo
double FBitRight();					// rotace vpravo


double FGetCodePage();				// zji�t�n� znakov� sady
double FGetLanguage();				// zji�t�n� jazyku

// porty
double FComOpen();					// otev�en� portu
double FGetComID();					// aktivn� port
double FGetComBaud();				// zji�t�n� p�enosov� rychlosti portu
double FGetComBits();				// zji�t�n� po�tu bit� portu
double FGetComParit();				// zji�t�n� parity
double FGetComNum();				// na�ten� bajtu

// mixer
double FGetLineType();				// zji�t�n� typu sign�lu
double FGetCtrlType();				// zji�t�n� typu ovl�dac�ho prvku
double FGetCtrlVal();				// zji�t�n� hodnoty ovl�dac�ho prvku
double FGetMixChannel();			// zji�t�n� po�tu kan�l� prvku

// Direct3D
double FGetD3DOCulling();			// zji�t�n� m�du odstra�ov�ni ploch objektu Direct3D
double FD3DTriangle();				// vytvo�en� troj�hlen�ku
double FD3DWall();					// vytvo�en� zdi
double FD3DCircle();				// vytvo�en� kruhu
double FD3DCube();					// vytvo�en� krychle
double FD3DSphere();				// vytvo�en� koule
double FD3DHemisphere();			// vytvo�en� polokoule
double FD3DTorus();					// vytvo�en� torusu
double FD3DCylinder();				// vytvo�en� v�lce
double FD3DCone();					// vytvo�en� ku�ele
double FD3DCone2();					// vytvo�en� komol�ho ku�ele
double FD3DClone();					// klonov�n�
double FD3DText();					// na�ten� objektu z textu
double FD3DFile();					// na�ten� objektu ze souboru
double FGetD3DObjectID();			// zji�t�n� ID aktivn�ho objektu
double FD3DLight();					// vytvo�en� sv�tla

double FGetD3DScaleX();				// zji�t�n� m���tka ve sm�ru X
double FGetD3DScaleY();				// zji�t�n� m���tka ve sm�ru Y
double FGetD3DScaleZ();				// zji�t�n� m���tka ve sm�ru Z
double FGetD3DRotateX();			// zji�t�n� rotace podle osy X
double FGetD3DRotateY();			// zji�t�n� rotace podle osy Y
double FGetD3DRotateZ();			// zji�t�n� rotace podle osy Z
double FGetD3DTransX();				// zji�t�n� posunu ve sm�ru X
double FGetD3DTransY();				// zji�t�n� posunu ve sm�ru Y
double FGetD3DTransZ();				// zji�t�n� posunu ve sm�ru Z
double FGetD3DColor();				// zji�t�n� barvy objektu
double FGetD3DEmissive();			// zji�t�n� barvy sv�tivosti
double FGetD3DAmbiRef();			// zji�t�n� odrazu rozpt�len�ho sv�tla
double FGetD3DPower();				// zji�t�n� matnosti
double FGetD3DSpecular();			// zji�t�n� barvy odlesku
double FGetD3DLightColor();			// zji�t�n� barvy sv�tla
double FGetD3DLightType();			// zji�t�n� typu sv�tla
double FGetD3DLightUmbra();			// zji�t�n� �hlu st�nu sv�tla
double FGetD3DLightPenumbra();		// zji�t�n� �hlu polost�nu sv�tla
double FGetD3DLightRange();			// zji�t�n� dosahu sv�tla
double FGetD3DLightCatten();		// zji�t�n� konstant�ho �tlumu sv�tla
double FGetD3DLightLatten();		// zji�t�n� line�rn�ho �tlumu sv�tla
double FGetD3DLightQatten();		// zji�t�n� line�rn�ho �tlumu sv�tla
double FGetD3DProjection();			// zji�t�n� projekce
double FGetD3DSceneColor();			// zji�t�n� barvy pozad� sc�ny
double FGetD3DAmbient();			// zji�t�n� ambient sv�tla
double FGetD3DFogColor();			// zji�t�n� barvy mlhy
double FGetD3DFogMode();			// zji�t�n� m�dy mlhy
double FGetD3DFogStart();			// zji�t�n� za��tku mlhy
double FGetD3DFogEnd();				// zji�t�n� konce mlhy
double FGetD3DFogDensity();			// zji�t�n� hustotu mlhy
double FGetD3DViewFront();			// zji�t�n� projek�n� roviny
double FGetD3DViewBack();			// zji�t�n� vzd�lenosti dohledu
//double FD3DPick();					// vyhled�n� objektu
double FD3D2DObject();				// vytvo�en� 2D objektu
double FD3D2DTerrain();				// vytvo�en� ter�nu
double FD3DLevel();					// zji�t�n� �rovn� ter�nu
double FD3DEmpty();					// vytvo�en� pr�zdn�ho objektu
double FGetD3DSplits();				// zji�t�n� slo�itosti objekt�
double FGetD3DOrder();				// zji�t�n� po�ad� rotac� objekt�
double FGetD3DDriver();				// zji�t�n� ovlada�e
double FGetD3DInterface();			// zji�t�n� rozhran�
double FD3DRotary();				// vytvo�en� rota�n�ho objektu
double FD3DTerrainMap();			// vytvo�en� ter�nu z plochy
double FGetD3DSmooth();				// zji�t�n� vyhlazen� textur
double FD3D2DStatic();				// vytvo�en� statick�ho 2D objektu
double FGetD3DTransparent();		// zji�t�n� pr�hlednosti textury objektu Direct3D
double FGetD3DAlphaRef();			// zji�t�n� alfa �rovn� Direct3D
double FD3DElapsed0();				// uplynul� p�esn� �as 
double FD3DElapsed();				// uplynul� p�esn� �as 
double FD3DSpeed();					// rychlost renderov�n�
double FD3DFree();					// voln� videopam�
double FD3DPicture();				// vytvo�en� 2D obr�zku
double FD3DLensFlare();				// vytvo�en� �o�kov�ho odlesku
double FGetD3DStage();				// zji�t�n� po�tu stup�� textur
double FGetD3DColorOp();			// zji�t�n� operace barev
double FGetD3DColorArg1();			// zji�t�n� argumentu 1 barev
double FGetD3DColorArg2();			// zji�t�n� argumentu 2 barev
double FGetD3DAlphaOp();			// zji�t�n� operace alfa
double FGetD3DAlphaArg1();			// zji�t�n� argumentu 1 alfa
double FGetD3DAlphaArg2();			// zji�t�n� argumentu 2 alfa
double FGetD3DAddressU();			// zji�t�n� horizont�ln�ho adresov�n�
double FGetD3DAddressV();			// zji�t�n� vertik�ln�ho adresov�n�
//double FGetD3DBorder();				// zji�t�n� barvy okol� textury
double FGetD3DBias();				// zji�t�n� zjemn�n� vzd�len�ch textur
double FGetD3DMipMaps();			// po�et �rovn� mipmaps

double FPolarRadius();				// v�po�et vzd�lenosti bodu
double FPolarAngle();				// v�po�et �hlu bodu
double FPreciseTimer();				// p�esn� ��ta� �asu
double FGetD3DTFactor();			// zji�t�n� barvy faktoru textur
double FGetD3DZGroup();				// zji�t�n� renderovac� skupiny
double FGetD3DZTest();				// zji�t�n� hloubkov�ho testu
double FGetD3DShadowRange();		// zji�t�n� dosahu st�nu
double FGetD3DLODObject();			// zji�t�n� objektu pro LOD
double FGetD3DLODDistance();		// zji�t�n� vzd�lenosti pro LOD
double FGetD3DMorphModel();			// zji�t�n� max. stupn� morfov�n�
double FGetD3DMorphLevel();			// zji�t�n� stupn� morfov�n�
double FD3DChild0();				// poskytnut� potomka 3D objektu
double FD3DChild();					// poskytnut� dal��ho potomka 3D objektu

// tabulky pro tvorbu zdi 3D, pou��vaj� se i p�i zobrazen� 2D objektu v OpenGL
extern D3DVECTOR wall_vertices[4];
extern float wall_text[2*4];

// obsluha DLL
double FDLLLoad();					// na�ten� DLL knihovny
double FDLLImport();				// importov�n� DLL funkce
double FDLLExecNum();				// proveden� DLL funkce
double FMemoryNew();				// vytvo�en� bloku pam�ti
double FMemoryResize();				// zm�na velikosti bloku pam�ti
double FGetMemoryRead();			// ukazatel �ten� z pam�ti
double FGetMemoryWrite();			// ukazatel z�pisu do pam�ti
double FMemoryCompare();			// porovn�n� dat
double FGetMemoryByteS();			// bajt se znam�nkem
double FGetMemoryByteU();			// bajt bez znam�nka
double FGetMemoryWordS();			// slovo se znam�nkem
double FGetMemoryWordU();			// slovo bez znam�nka
double FGetMemory3ByteS();			// 3-bajt se znam�nkem
double FGetMemory3ByteU();			// 3-bajt bez znam�nka
double FGetMemoryDWordS();			// dvouslovo se znam�nkem
double FGetMemoryDWordU();			// dvouslovo bez znam�nka
double FGetMemoryFloat();			// re�ln� ��slo 4 bajty
double FGetMemoryDouble();			// re�ln� ��slo 8 bajt�
double FGetMemoryTextNLen();		// d�lka textu s pevnou d�lkou
double FWindowHandle();				// handle okenn�ho prvku
double FMemory3D();					// vytvo�en� 3D objektu z pam�ti
double FD3DMemory();				// vytvo�en� pam�ov�ho bloku z 3D objektu

// konfigurace
double FGetIniNum();				// �ten� konfigura�n�ho ��sla
double FGetIniNum0();				// �ten� konfigura�n�ho ��sla
double FGetRegKey();				// skupina kl��� registru
double FGetRegNum();				// �ten� ��sla z registru
double FGetRegNum0();				// �ten� ��sla z registru
