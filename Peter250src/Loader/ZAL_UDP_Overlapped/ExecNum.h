
/***************************************************************************\
*																			*
*						Provádìní programu - èíselnı vıraz					*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inline volání pøíkazu - èíslo

inline double FNum()
{
	ExecItem++;
	return ExecItem[-1].ExecNum();
}


/////////////////////////////////////////////////////////////////////////////
// naètení èíselného parametru pøevedeného na int

//int FInt();

/////////////////////////////////////////////////////////////////////////////
// naètení úhlu s normalizací rozsahu 0 a 2pi

double FAngle();


/////////////////////////////////////////////////////////////////////////////
// naètení èíselného parametru pøevedeného na int se zaokrouhlením

int FIntR();


/////////////////////////////////////////////////////////////////////////////
// naètení èíselného parametru pøevedeného na DWORD se zaokrouhlením

DWORD FIntRN();

/////////////////////////////////////////////////////////////////////////////
// naètení èíselného parametru pøevedeného na INT64 se zaokrouhlením

hyper FIntHN();

/////////////////////////////////////////////////////////////////////////////
// naètení èíselného parametru pøevedeného na grafickou souøadnici X

int FIntX();


/////////////////////////////////////////////////////////////////////////////
// naètení èíselného parametru pøevedeného na grafickou souøadnici Y

inline int FIntY() { return FIntX(); };


/////////////////////////////////////////////////////////////////////////////
// naètení èíselného parametru vynásobeného * 1000 (èas v milisekundách)

int FInt1000();


/////////////////////////////////////////////////////////////////////////////
// naètení èíselného parametru vynásobeného * 2 (prùmìr kruhu)

int FIntX2();


/////////////////////////////////////////////////////////////////////////////
// naètení èíselného parametru vynásobeného * 18.20648 (èas v èasovıch impulsech)

int FInt18();


/////////////////////////////////////////////////////////////////////////////
// naètení èíselného parametru s vynásobením * 255

BYTE FInt255();


/////////////////////////////////////////////////////////////////////////////
// naètení èíselného parametru pøevedeného na souborovı èas

__int64 FNumFileTime();


/////////////////////////////////////////////////////////////////////////////
// naètení èíselného parametru pøevedeného na systémovı èas

#define FILETIMEKONV 10000000		// pøevod na souborovı èas

extern	SYSTEMTIME	SystemTime;			// pomocná promìnná pro obsluhu èasu
void FNumSysTime();


/////////////////////////////////////////////////////////////////////////////
// naètení èíselného parametru pøevedeného na svìtovı souborovı èas

__int64 FNumFileTimeUTC();


/////////////////////////////////////////////////////////////////////////////
// naètení barvy

int FNumCol();


/////////////////////////////////////////////////////////////////////////////
// naètení souøadnic Petra/Petry (ECX = adresa x, EDX = adresa y)

bool _fastcall PetrXY(int& x, int& y);
bool _fastcall Petr2XY(int& x, int& y);


/////////////////////////////////////////////////////////////////////////////
// naètení pozice Petra/Petry (NULL=neplatná)

MAPITEM* PetrXYMap();
MAPITEM* Petr2XYMap();


/////////////////////////////////////////////////////////////////////////////
// naètení pøíští souøadnice Petra/Petry (ECX = adresa x, EDX = adresa y)

bool _fastcall PetrXYNext(int& x, int& y);
bool _fastcall Petr2XYNext(int& x, int& y);


// funkce
double FNumFunc();					// funkce s návratem èíselné hodnoty

// promìnné
double FNumConst();					// èíselná konstanta
double FNumObj();					// globální promìnná (Data = index)
double FNumObjList();				// globalní promìnná v seznamu (Data = index, List = seznam)
double FNumLoc();					// lokální promìnná (Data = index)
double FNumLocList();				// lokální promìnná v seznamu (Data = index, List = seznam)
double FGetList();					// index globálního seznamu
double FGetListLoc();				// index lokálního seznamu
double FGetListAuto();				// inkrementace globálního seznamu
double FGetListAutoLoc();			// inkrementace lokálního seznamu
double FGetSpriteX();				// souøadnice X globálního sprajtu (Data = index)
double FGetSpriteXList();			// souøadnice X sprajtu v seznamu (Data = index, List = seznam)
double FGetSpriteXLoc();			// souøadnice X lokálního sprajtu (Data = index)
double FGetSpriteXLocList();		// souøadnice X lokálního sprajtu v seznamu (Data = index, List = seznam)
double FGetSpriteY();				// souøadnice Y globálního sprajtu (Data = index)
double FGetSpriteYList();			// souøadnice Y sprajtu v seznamu (Data = index, List = seznam)
double FGetSpriteYLoc();			// souøadnice Y lokálního sprajtu (Data = index)
double FGetSpriteYLocList();		// souøadnice Y lokálního sprajtu v seznamu (Data = index, List = seznam)
double FGetSpriteSmer();			// smìr globálního sprajtu (Data = index)
double FGetSpriteSmerList();		// smìr sprajtu v seznamu (Data = index, List = seznam)
double FGetSpriteSmerLoc();			// smìr lokálního sprajtu (Data = index)
double FGetSpriteSmerLocList();		// smìr lokálního sprajtu v seznamu (Data = index, List = seznam)
double FGetSpriteFaze();			// fáze globálního sprajtu (Data = index)
double FGetSpriteFazeList();		// fáze sprajtu v seznamu (Data = index, List = seznam)
double FGetSpriteFazeLoc();			// fáze lokálního sprajtu (Data = index)
double FGetSpriteFazeLocList();		// fáze lokálního sprajtu v seznamu (Data = index, List = seznam)
double FGetSpriteStatus();			// stav globálního sprajtu (Data = index)
double FGetSpriteStatusList();		// stav sprajtu v seznamu (Data = index, List = seznam)
double FGetSpriteStatusLoc();		// stav lokálního sprajtu (Data = index)
double FGetSpriteStatusLocList();	// stav lokálního sprajtu v seznamu (Data = index, List = seznam)

// obsluha Petra a Petry
double FGetSmer();					// Petr - smìr
double FGetSmer2();					// Petra - smìr
double FGetPozX();					// Petr - pozice X
double FGetPozX2();					// Petra - pozice X
double FGetPozY();					// Petr - pozice Y
double FGetPozY2();					// Petra - pozice Y
double FGetSwcN1();					// naètení èísla 1 - Petr
double FGetSwcN12();				// naètení èísla 1 - Petra
double FGetSwcN2();					// naètení èísla 2 - Petr
double FGetSwcN22();				// naètení èísla 2 - Petra
double FGetSwcN3();					// naètení èísla 3 - Petr
double FGetSwcN32();				// naètení èísla 3 - Petra

// vıpoèty
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
double FNumInt();					// celé èíslo
double FFrac();						// desetinná èást èísla
double FAbs();						// absolutní hodnota
double FSqrt();						// odmocnina
double FFact();						// faktoriál
double FRand();						// náhodné èíslo s násobkem
double FRand0();					// náhodné èíslo bez násobku
double FDegRad();					// pøevod stupòù na radiány
double FRadDeg();					// pøevod radiánù na stupnì
double FLog();						// dekadickı logaritmus
double FLn();						// pøirozenı logaritmus
double FExp();						// pøirozenı exponent
double FAdd();						// operátor ADD
double FSub1();						// operátor SUB pro 1 prvek
double FSub();						// operátor SUB
double FMul();						// operátor MUL
double FDiv1();						// operátor DIV pro 1 prvek
double FDiv();						// operátor DIV
double FMod();						// operátor MOD
double FMocn();						// operátor MOCNINA
double FNumSqr();					// druhá mocnina

// obsluha grafiky
double FColorLevel();				// nastavení jasu barvy
double FColorSynth();				// syntéza barvy
double FGetPoint();					// zjištìní barvy bodu
double FGetPicWidth();				// zjištìní šíøky obrázku
double FGetPicHeight();				// zjištìní vıšky obrázku
double FGetGraphAreaWidth();		// zjištìní šíøky pole k zobrazení centrovaného textu

// obsluha èasu
double FGetTimeCurrent();			// naètení aktuálního èasu
double FTimeUtc();					// pøevod na svìtovı èas
double FTimeLocal();				// pøevod na místní èas
double FTimeComp();					// syntéza èasu
double FGetTimeYear();				// naètení roku z èasu
double FGetTimeMonth();				// naètení mìsíce z èasu
double FGetTimeDay();				// naètení dne z èasu
double FGetTimeWeek();				// naètení dne v tıdnu z èasu
double FGetTimeHour();				// naètení hodiny z èasu
double FGetTimeMin();				// naètení minuty z èasu
double FGetTimeSec();				// naètení sekundy z èasu
double FGetTimeMSec();				// naètení milisekundy z èasu

// obsluha myši
double FMouseX();					// naètení souøadnice X myši
double FMouseY();					// naètení souøadnice Y myši

// obsluha dialogù
double FNumDialogBox();				// dialog box s návratem hodnoty

// obsluha souboru
double FGetDiskSize();				// zjištìní velikosti disku
double FGetDiskFree();				// zjištìní volného místa disku
double FGetDiskSerial();			// naètení sériového èísla disku
double FGetFileCreate();			// èas vytvoøení souboru
double FGetFileOpen();				// èas ètení ze souboru
double FGetFileModi();				// èas zápisu do souboru
double FGetFileReadOff();			// offset pro ètení ze souboru
double FGetFileWriteOff();			// offset pro zápis do souboru
double FGetFileSize();				// zjištìní velikosti souboru
double FGetFileTextNLen();			// délka textu s pevnou délkou
double FFileDiskType();				// naètení typu disku
double FGetFileByteS();				// naètení èísla ze souboru bajt se znaménkem
double FGetFileByteU();				// naètení èísla ze souboru bajt bez znaménka
double FGetFileWordS();				// naètení èísla ze souboru slovo se znaménkem
double FGetFileWordU();				// naètení èísla ze souboru slovo bez znaménka
double FGetFile3ByteS();			// naètení èísla ze souboru 3 bajty se znaménkem
double FGetFile3ByteU();			// naètení èísla ze souboru 3 bajty bez znaménka
double FGetFileDWordS();			// naètení èísla ze souboru dvojslovo se znaménkem
double FGetFileDWordU();			// naètení èísla ze souboru dvojslovo bez znaménka
double FGetFileFloat();				// naètení èísla ze souboru FLOAT
double FGetFileDouble();			// naètení èísla ze souboru DOUBLE

// obsluha CD disku
double FCDGetLength();				// zjištìní délky CD
double FCDGetTracks();				// zjištìní poètu stop CD
double FCDGetPos();					// zjištìní pozice CD
double FCDGetTrack();				// zjištìní aktivní stopy CD
double FCDGetTrackLength();			// zjištìní délky aktivní stopy
double FCDGetTrackPos();			// zjištìní pozice aktivní stopy CD

// obsluha hudby
double FMusicLen();					// délka pøehrávané hudby
double FGetMusicPos();				// pozice hudby

// obsluha zvuku
double FGetSoundBit();				// poèet bitù na vzorek
double FGetSoundFreq();				// vzorkovací kmitoèet
double FGetSoundLength();			// délka zvuku
double FGetSoundPos();				// pozice zvuku
double FGetSoundDistinguish();		// rozlišovací kód

// obsluha textù
double FTextLen();					// délka textu
double FTextScan();					// pøevod textu na èíslo
double FTextFind();					// nalezení textu
double FTextOrd();					// abecední poøadí znaku
double FTextLineNum();				// zjištìní poètu øádkù textu

// obsluha klávesnice
double FVKey();						// naètení klávesy bez èekání
double FVKeyWait();					// naètení klávesy s èekáním

// plocha
double FGetMapWidth();				// šíøka plochy
double FGetMapWidth0();				// šíøka hlavní plochy
double FGetMapHeight();				// vıška plochy
double FGetMapHeight0();			// vıška hlavní plochy
double FGetMapNum1();				// naètení èísla 1 z plochy
double FGetMapNum2();				// naètení èísla 2 z plochy
double FGetMapNum3();				// naètení èísla 3 z plochy

// naètení okna k zobrazení sprajtù
double FGetSpriteWinX1();
double FGetSpriteWinY1();
double FGetSpriteWinX2();
double FGetSpriteWinY2();

double FGetReturnCode();			// návratovı kód

double FGetExecWait();				// spuštìní programu

// joystick
double FJoystickX();				// pozice X
double FJoystickY();				// pozice Y
double FJoystickZ();				// pozice Z

// dialogy
double FGetWindowID();				// ID aktivního prvku
double FWindowNew();				// vytvoøení okna
double FWindowRONew();				// vytvoøení okna
double FButtonNew();				// vytvoøení tlaèítka
double FGetButtonEsc();				// tlaèítko Esc
double FDefButtonNew();				// vytvoøení tlaèítka
double FButtonIconNew();			// vytvoøení ikonového tlaèítka
double FButton3New();				// vytvoøení tlaèítka
double FButtonCheckNew();			// vytvoøení tlaèítka
double FButtonCheckNew2();			// vytvoøení pøepínacího tlaèítka
double FEditNew();					// vytvoøení editaèního øádku
double FButtonRadioNew();			// vytvoøení tlaèítka
double FButtonRadioNew2();			// vytvoøení tlaèítka
double FMemoNew();					// vytvoøení editaèního pole
double FRicheditNew();				// vytvoøení editoru Richedit
double FRichMemoNew();				// vytvoøení editaèního pole se zalamováním
double FGroupBoxNew();				// vytvoøení skupiny
double FListBoxNew();				// vytvoøení seznamu
double FTabsNew();					// vytvoøení záloek
double FWinTextNew();				// vytvoøení textu
double FWinTextNewCenter();			// vytvoøení textu vystøedìného
double FWinTextNewRight();			// vytvoøení textu zarovnaného vpravo
double FListEditNew();				// vytvoøení seznamu
double FComboBoxNew();				// vytvoøení seznamu
double FComboEditNew();				// vytvoøení seznamu
double FTableNew();					// vytvoøení tabulky
double FGetTableWidth();			// zjištìní šíøky sloupce tabulky
double FGetTableAlign();			// zjištìní zarovnání sloupce tabulky
double FGetWindowX();				// poèátek okna vlevo
double FGetWindowY();				// poèátek okna dole
double FGetWindowW();				// šíøka okna
double FGetWindowH();				// vıška okna
double FGetWindowWInt();			// vnitøní šíøka okna
double FGetWindowHInt();			// vnitøní vıška okna
double FGetFontHeight();			// vıška fontù
double FGetFontWidth();				// šíøka fontù
double FGetFontAngle();				// úhel textu
double FGetFontCol();				// barva textu
double FGetFontBack();				// barva pozadí
double FGetFocus();					// okno s fokusem
double FWinIconNew();				// vytvoøení ikony
double FWinPicNew();				// vytvoøení obrázku
double FProgressNew();				// indikátor
double FHTrackbarNew();				// táhlo horizontálnì
double FVTrackbarNew();				// táhlo vertikálnì
double FHScrollbarNew();			// horizontální posuvník
double FVScrollbarNew();			// vertikální posuvník
double FGetWindowPos();				// pozice v prvku
double FGetWindowPos2();			// pozice 2 v prvku

// DirectPlay
double FGetGameN1();				// zjištìní èíselné hodnoty hry 1
double FGetGameN2();				// zjištìní èíselné hodnoty hry 2
double FGetGameN3();				// zjištìní èíselné hodnoty hry 3
double FGetGameN4();				// zjištìní èíselné hodnoty hry 4
double FGetGameN5();				// zjištìní èíselné hodnoty hry 5
double FGetGameN6();				// zjištìní èíselné hodnoty hry 6
double FGetGameN7();				// zjištìní èíselné hodnoty hry 7
double FGetGameN8();				// zjištìní èíselné hodnoty hry 8
double FGetPlayerMax();				// zjištìní max. poètu hráèù
double FPlayerInx();				// zjištìní indexu hráèe
double FGetGamePacket();			// pøíjem paketu
double FGetGameDataN();				// naètení èísla z paketu
double FGetGameDataB();				// naètení bajtu z paketu

// UDP
double FGetUDPSendPort();			// zjištìní vysílacího portu UDP
double FGetUDPRecSize();			// zjištìní velikosti pøijímacího bufferu UDP
double FGetUDPRecPort();			// zjištìní pøijímacího portu UDP
double FGetUDPMem();				// pøíjem datového bloku UDP

// bitové operace
double FBitAnd();					// operátor BIT-AND (Data = poèet prvkù - 1)
double FBitOr();					// operátor BIT-OR (Data = poèet prvkù - 1)
double FBitXor();					// operátor BIT-XOR (Data = poèet prvkù - 1)
double FBitNot();					// bitovı doplnìk
double FBitLeft();					// rotace vlevo
double FBitRight();					// rotace vpravo


double FGetCodePage();				// zjištìní znakové sady
double FGetLanguage();				// zjištìní jazyku

// porty
double FComOpen();					// otevøení portu
double FGetComID();					// aktivní port
double FGetComBaud();				// zjištìní pøenosové rychlosti portu
double FGetComBits();				// zjištìní poètu bitù portu
double FGetComParit();				// zjištìní parity
double FGetComNum();				// naètení bajtu

// mixer
double FGetLineType();				// zjištìní typu signálu
double FGetCtrlType();				// zjištìní typu ovládacího prvku
double FGetCtrlVal();				// zjištìní hodnoty ovládacího prvku
double FGetMixChannel();			// zjištìní poètu kanálù prvku

// Direct3D
double FGetD3DOCulling();			// zjištìní módu odstraòováni ploch objektu Direct3D
double FD3DTriangle();				// vytvoøení trojúhleníku
double FD3DWall();					// vytvoøení zdi
double FD3DCircle();				// vytvoøení kruhu
double FD3DCube();					// vytvoøení krychle
double FD3DSphere();				// vytvoøení koule
double FD3DHemisphere();			// vytvoøení polokoule
double FD3DTorus();					// vytvoøení torusu
double FD3DCylinder();				// vytvoøení válce
double FD3DCone();					// vytvoøení kuele
double FD3DCone2();					// vytvoøení komolého kuele
double FD3DClone();					// klonování
double FD3DText();					// naètení objektu z textu
double FD3DFile();					// naètení objektu ze souboru
double FGetD3DObjectID();			// zjištìní ID aktivního objektu
double FD3DLight();					// vytvoøení svìtla

double FGetD3DScaleX();				// zjištìní mìøítka ve smìru X
double FGetD3DScaleY();				// zjištìní mìøítka ve smìru Y
double FGetD3DScaleZ();				// zjištìní mìøítka ve smìru Z
double FGetD3DRotateX();			// zjištìní rotace podle osy X
double FGetD3DRotateY();			// zjištìní rotace podle osy Y
double FGetD3DRotateZ();			// zjištìní rotace podle osy Z
double FGetD3DTransX();				// zjištìní posunu ve smìru X
double FGetD3DTransY();				// zjištìní posunu ve smìru Y
double FGetD3DTransZ();				// zjištìní posunu ve smìru Z
double FGetD3DColor();				// zjištìní barvy objektu
double FGetD3DEmissive();			// zjištìní barvy svítivosti
double FGetD3DAmbiRef();			// zjištìní odrazu rozptıleného svìtla
double FGetD3DPower();				// zjištìní matnosti
double FGetD3DSpecular();			// zjištìní barvy odlesku
double FGetD3DLightColor();			// zjištìní barvy svìtla
double FGetD3DLightType();			// zjištìní typu svìtla
double FGetD3DLightUmbra();			// zjištìní úhlu stínu svìtla
double FGetD3DLightPenumbra();		// zjištìní úhlu polostínu svìtla
double FGetD3DLightRange();			// zjištìní dosahu svìtla
double FGetD3DLightCatten();		// zjištìní konstantího útlumu svìtla
double FGetD3DLightLatten();		// zjištìní lineárního útlumu svìtla
double FGetD3DLightQatten();		// zjištìní lineárního útlumu svìtla
double FGetD3DProjection();			// zjištìní projekce
double FGetD3DSceneColor();			// zjištìní barvy pozadí scény
double FGetD3DAmbient();			// zjištìní ambient svìtla
double FGetD3DFogColor();			// zjištìní barvy mlhy
double FGetD3DFogMode();			// zjištìní módy mlhy
double FGetD3DFogStart();			// zjištìní zaèátku mlhy
double FGetD3DFogEnd();				// zjištìní konce mlhy
double FGetD3DFogDensity();			// zjištìní hustotu mlhy
double FGetD3DViewFront();			// zjištìní projekèní roviny
double FGetD3DViewBack();			// zjištìní vzdálenosti dohledu
//double FD3DPick();					// vyhledání objektu
double FD3D2DObject();				// vytvoøení 2D objektu
double FD3D2DTerrain();				// vytvoøení terénu
double FD3DLevel();					// zjištìní úrovnì terénu
double FD3DEmpty();					// vytvoøení prázdného objektu
double FGetD3DSplits();				// zjištìní sloitosti objektù
double FGetD3DOrder();				// zjištìní poøadí rotací objektù
double FGetD3DDriver();				// zjištìní ovladaèe
double FGetD3DInterface();			// zjištìní rozhraní
double FD3DRotary();				// vytvoøení rotaèního objektu
double FD3DTerrainMap();			// vytvoøení terénu z plochy
double FGetD3DSmooth();				// zjištìní vyhlazení textur
double FD3D2DStatic();				// vytvoøení statického 2D objektu
double FGetD3DTransparent();		// zjištìní prùhlednosti textury objektu Direct3D
double FGetD3DAlphaRef();			// zjištìní alfa úrovnì Direct3D
double FD3DElapsed0();				// uplynulı pøesnı èas 
double FD3DElapsed();				// uplynulı pøesnı èas 
double FD3DSpeed();					// rychlost renderování
double FD3DFree();					// volná videopamì
double FD3DPicture();				// vytvoøení 2D obrázku
double FD3DLensFlare();				// vytvoøení èoèkového odlesku
double FGetD3DStage();				// zjištìní poètu stupòù textur
double FGetD3DColorOp();			// zjištìní operace barev
double FGetD3DColorArg1();			// zjištìní argumentu 1 barev
double FGetD3DColorArg2();			// zjištìní argumentu 2 barev
double FGetD3DAlphaOp();			// zjištìní operace alfa
double FGetD3DAlphaArg1();			// zjištìní argumentu 1 alfa
double FGetD3DAlphaArg2();			// zjištìní argumentu 2 alfa
double FGetD3DAddressU();			// zjištìní horizontálního adresování
double FGetD3DAddressV();			// zjištìní vertikálního adresování
//double FGetD3DBorder();				// zjištìní barvy okolí textury
double FGetD3DBias();				// zjištìní zjemnìní vzdálenıch textur
double FGetD3DMipMaps();			// poèet úrovní mipmaps

double FPolarRadius();				// vıpoèet vzdálenosti bodu
double FPolarAngle();				// vıpoèet úhlu bodu
double FPreciseTimer();				// pøesnı èítaè èasu
double FGetD3DTFactor();			// zjištìní barvy faktoru textur
double FGetD3DZGroup();				// zjištìní renderovací skupiny
double FGetD3DZTest();				// zjištìní hloubkového testu
double FGetD3DShadowRange();		// zjištìní dosahu stínu
double FGetD3DLODObject();			// zjištìní objektu pro LOD
double FGetD3DLODDistance();		// zjištìní vzdálenosti pro LOD
double FGetD3DMorphModel();			// zjištìní max. stupnì morfování
double FGetD3DMorphLevel();			// zjištìní stupnì morfování
double FD3DChild0();				// poskytnutí potomka 3D objektu
double FD3DChild();					// poskytnutí dalšího potomka 3D objektu

// tabulky pro tvorbu zdi 3D, pouívají se i pøi zobrazení 2D objektu v OpenGL
extern D3DVECTOR wall_vertices[4];
extern float wall_text[2*4];

// obsluha DLL
double FDLLLoad();					// naètení DLL knihovny
double FDLLImport();				// importování DLL funkce
double FDLLExecNum();				// provedení DLL funkce
double FMemoryNew();				// vytvoøení bloku pamìti
double FMemoryResize();				// zmìna velikosti bloku pamìti
double FGetMemoryRead();			// ukazatel ètení z pamìti
double FGetMemoryWrite();			// ukazatel zápisu do pamìti
double FMemoryCompare();			// porovnání dat
double FGetMemoryByteS();			// bajt se znaménkem
double FGetMemoryByteU();			// bajt bez znaménka
double FGetMemoryWordS();			// slovo se znaménkem
double FGetMemoryWordU();			// slovo bez znaménka
double FGetMemory3ByteS();			// 3-bajt se znaménkem
double FGetMemory3ByteU();			// 3-bajt bez znaménka
double FGetMemoryDWordS();			// dvouslovo se znaménkem
double FGetMemoryDWordU();			// dvouslovo bez znaménka
double FGetMemoryFloat();			// reálné èíslo 4 bajty
double FGetMemoryDouble();			// reálné èíslo 8 bajtù
double FGetMemoryTextNLen();		// délka textu s pevnou délkou
double FWindowHandle();				// handle okenního prvku
double FMemory3D();					// vytvoøení 3D objektu z pamìti
double FD3DMemory();				// vytvoøení pamìového bloku z 3D objektu

// konfigurace
double FGetIniNum();				// ètení konfiguraèního èísla
double FGetIniNum0();				// ètení konfiguraèního èísla
double FGetRegKey();				// skupina klíèù registru
double FGetRegNum();				// ètení èísla z registru
double FGetRegNum0();				// ètení èísla z registru
