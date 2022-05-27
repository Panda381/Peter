
/***************************************************************************\
*																			*
*							Obsluha editace souboru							*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// položka inicializaèní tabulky funkcí

typedef struct INITFUNC_ {
	short	SrcMask;		// maska zdrojových vlastností
	short	DstMask;		// maska cílových vlastností
	short	Param;			// parametry
	char*	Import;			// text pro import funkce
} INITFUNC;

// inicializaèní tabulka funkcí
extern	const INITFUNC InitTabFunc[];

/////////////////////////////////////////////////////////////////////////////
// editaèní pole

extern	int			FocusBuf;						// okno s fokusem (= èíslo bufferu, BufEdiID = editaèní pole)

extern	int			EditMode;						// aktivní editaèní pole (= èíslo bufferu)
extern	int			EditBuf;						// editovaný buffer
extern	int			EditItem;						// editovaná položka v bufferu
extern	int			EditItemSprite;					// sprajt s editovanou ikonou (úschova pro historii)
extern	int			EditItemPic;					// editovaný obrázek sprajtu (úschova pro historii)

extern	int			EditX;							// souøadnice X poèátku editaèního pole
extern	int			EditY;							// souøadnice Y poèátku editaèního pole
extern	int			EditWidth;						// šíøka editaèního pole
extern	int			EditHeight;						// výška editaèního pole

extern	int			ToolMode;						// aktivní editaèní mód Toolbaru (=èíslo bufferu)

extern	UINT		FocusTimer;						// èasovaè pro nastavení fokusu
extern	int			FocusTimerN;					// èítaè pro nastavení fokusu

/////////////////////////////////////////////////////////////////////////////
// hlášení o zmìnì adresáøe

extern	HANDLE		FunctionDirChange;				// hlášení o zmìnì adresáøe - funkce
extern	HANDLE		NumberDirChange;				// hlášení o zmìnì adresáøe - èísla
extern	HANDLE		TextDirChange;					// hlášení o zmìnì adresáøe - text
extern	HANDLE		BoolDirChange;					// hlášení o zmìnì adresáøe - logické hodnoty
extern	HANDLE		IconDirChange;					// hlášení o zmìnì adresáøe - ikony
extern	HANDLE		MapDirChange;					// hlášení o zmìnì adresáøe - plochy
extern	HANDLE		PictureDirChange;				// hlášení o zmìnì adresáøe - obrázky
extern	HANDLE		SpriteDirChange;				// hlášení o zmìnì adresáøe - sprajty
extern	HANDLE		SoundDirChange;					// hlášení o zmìnì adresáøe - zvuk
extern	HANDLE		MusicDirChange;					// hlášení o zmìnì adresáøe - hudba

extern	HANDLE		FunctionDirChange2;				// hlášení o zmìnì adresáøe - alternativní funkce
extern	HANDLE		NumberDirChange2;				// hlášení o zmìnì adresáøe - alternativní èísla
extern	HANDLE		TextDirChange2;					// hlášení o zmìnì adresáøe - alternativní text
extern	HANDLE		BoolDirChange2;					// hlášení o zmìnì adresáøe - alternativní logické hodnoty
extern	HANDLE		IconDirChange2;					// hlášení o zmìnì adresáøe - alternativní ikony
extern	HANDLE		MapDirChange2;					// hlášení o zmìnì adresáøe - alternativní plochy
extern	HANDLE		PictureDirChange2;				// hlášení o zmìnì adresáøe - alternativní obrázky
extern	HANDLE		SpriteDirChange2;				// hlášení o zmìnì adresáøe - alternativní sprajty
extern	HANDLE		SoundDirChange2;				// hlášení o zmìnì adresáøe - alternativní zvuk
extern	HANDLE		MusicDirChange2;				// hlášení o zmìnì adresáøe - alternativní hudba

extern	int			ProgDirChangeIgnore;			// èítaè pro ignorování hlášení zmìn po vlastní zmìnì

extern	int			ProgStatusWidth;				// šíøka druhého pole programového øádku

//#define MOUSEINV -100000							// neplatná souøadnice myši
//extern	int			mX;								// souøadnice X myši
//extern	int			mY;								// souøadnice Y myši

extern	BOOL		Modi;							// pøíznak modifikace souboru

/////////////////////////////////////////////////////////////////////////////
// obsluha kurzoru myši

extern	int		MouseX;								// souøadnice X myši
extern	int		MouseY;								// souøadnice Y myši


/////////////////////////////////////////////////////////////////////////////
// obsluha editace èísla

extern	HWND	EditNumWnd;						// editaèní okno èísla

/////////////////////////////////////////////////////////////////////////////
// editace èíselné položky plochy

extern	HWND	EditMapNumWnd;					// editaèní okno plochy (NULL=není)

/////////////////////////////////////////////////////////////////////////////
// obsluha editace logických hodnot

extern	HWND	EditLogWnd;						// editaèní okno logických hodnot

/////////////////////////////////////////////////////////////////////////////
// obsluha editace textu

extern	HWND	EditTextWnd;					// editaèní okno textu

/////////////////////////////////////////////////////////////////////////////
// posuvníky pro editory

extern	HWND	HScroll;						// horizontální posuvník
extern	HWND	VScroll;						// vertikální posuvník
extern	BOOL	HScrollDisp;					// je horizontální posuvník
extern	BOOL	VScrollDisp;					// je vertikální posuvník
#define	TRACKSIRKA 16							// šíøka posuvníku

/////////////////////////////////////////////////////////////////////////////
// globální a lokální promìnné

extern	BOOL		ProgMode;						// pøíznak módu editace
//extern	HWND		EditName;						// editor jména prvku
//extern	HWND		EditFind;						// editor textu k hledání
extern	int			ToolBarProgN;					// poèet základních prvkù panelu nástrojù

extern	HWND		TreeCls;						// okno tøíd
extern	HWND		TreeObj;						// okno globálních objektù
extern	HWND		TreeLoc;						// okno lokálních objektù
extern	HWND		TreeEdi;						// okno editoru
extern	HWND		TreeStr;						// okno struktur

extern	HIMAGELIST	ProgImageList;					// seznam obrázkù
extern	HIMAGELIST	ProgImageListMedium;			// seznam støedních obrázkù
extern	HIMAGELIST	ProgImageListSmall;				// seznam malých obrázkù
extern	HIMAGELIST	ProgStateList;					// seznam stavových ikon
extern	HIMAGELIST	ProgStateListMedium;			// seznam støedních stavových ikon
extern	HIMAGELIST	ProgStateListSmall;				// seznam malých stavových ikon


#define CLASSFNCINDEX		0		// index FNC v bufferu tøíd
#define CLASSICONINDEX		4		// index ICON v bufferu tøíd
#define CLASSMAPINDEX		5		// index MAP v bufferu tøíd
#define CLASSPICINDEX		6		// index BACK v bufferu tøíd
#define CLASSSPRITEINDEX	9		// index SPRITE v bufferu tøíd

/////////////////////////////////////////////////////////////////////////////
// obsluha editace jména prvku

extern	BOOL		EditName;						// pøíznak probíhající editace

/////////////////////////////////////////////////////////////////////////////
// inicializaèní buffery (pøipravují se jen jednou pøi startu programu)
//
// Pøi používání bufferù se poèítá s tím, že položky jsou uloženy po øadì
// od zaèátku bufferu, poèet položek odpovídá poètu (a poøadí) funkcí.

extern	const int	FncNum;							// poèet funkcí
extern	CBufIcon	IconFnc;						// ikony funkcí

extern	BOOL		PreviewPic;						// probíhá preview obrázku

/////////////////////////////////////////////////////////////////////////////
// buffery

extern	CBufProg	BufProg[PROGBUFNUM];			// buffery programu
extern	CBufReal	Real;							// buffer reálných èísel
extern	CBufMultiText	Text;							// buffer textù
extern	CBufBool	Bool;							// buffer logických promìnných
extern	CBufIcon	Icon;							// buffer ikon
extern	CBufIcon	IconState;						// buffer stavových ikon
extern	CBufMap		Map;							// buffer ploch
extern	CBufPic		Picture;						// buffer obrázkù
extern	CBufSprite	Sprite;							// buffer sprajtù
extern	CBufSound	Sound;							// buffer zvukù
extern	CBufMusic	Music;							// buffer hudby

extern	CSprite		Sprite0;						// sprajt 0 (Petr)
extern	CSprite		Sprite1;						// sprajt 1 (Petra)
extern	CBufIcon	Icon1;							// buffer s 1 ikonou k zobrazení

//#define		ICONNUM 31								// velikost bufferu ikon (pro TRUECOLOR max. 63!)
//extern	CBufIcon	Icon16;							// buffer ikon s velikostí 16x16
//extern	CBufIndex	Icon16List;						// buffer indexù ikon s velikostí 16x16
//extern	int			Icon16Next;						// pøíští ikona pro zápis
//extern	CBufIcon	Icon32;							// buffer ikon s velikostí 32x32
//extern	CBufIndex	Icon32List;						// buffer indexù ikon s velikostí 32x32
//extern	int			Icon32Next;						// pøíští ikona pro zápis

#define	BufInt		BufProg[BufIntID]				// interní buffer
#define	BufCls		BufProg[BufClsID]				// buffer tøíd
#define	BufObj		BufProg[BufObjID]				// globální buffer
#define BufLoc		BufProg[BufLocID]				// lokální buffer
#define	BufEdi		BufProg[BufEdiID]				// editaèní buffer
#define	BufStr		BufProg[BufStrID]				// buffer struktur

extern	CBufUndo	Undo;							// buffer undo a redo

extern	int			GroupIndex;						// index do bufferu BufStr na IDF_GROUP

/////////////////////////////////////////////////////////////////////////////
// combobox panely

extern	HWND		ToolEditIcon;					// panel nástrojù editoru ikon
extern	HWND		ToolEditIconWidth;				// panel nástrojù volby šíøky
extern	HWND		ToolEditMap;					// panel nástrojù editoru ploch

/////////////////////////////////////////////////////////////////////////////
// tabulky pøíkazù v COMBO boxu

extern	int ToolBarEditPicTools[];					// nástroje EDITICON
extern	int ToolBarEditPicWidths[];					// šíøky EDITICON
extern	int ToolBarEditMapTools[];					// nástroje EDITMAP


/////////////////////////////////////////////////////////////////////////////
// nastavení menu programu

void ProgSetMenu();

/////////////////////////////////////////////////////////////////////////////
// aktualizace po zmìnì jazyku

void ProgReloadJazyk();


/////////////////////////////////////////////////////////////////////////////
// první inicializace pøi startu programu (hlavní okno i SELECT je již vytvoøeno)

void ProgStartInit();


/////////////////////////////////////////////////////////////////////////////
// zahájení režimu editace souboru

void ProgInit();


/////////////////////////////////////////////////////////////////////////////
// ukonèení režimu editace souboru

void ProgTerm();


/////////////////////////////////////////////////////////////////////////////
// zahájení sledování zmìn adresáøe

void BeginDirChange(HANDLE* dir1, HANDLE* dir2, CText& path1, CText& path2);


/////////////////////////////////////////////////////////////////////////////
// update voleb pro ZOOM

void ProgUpdateInOut();


/////////////////////////////////////////////////////////////////////////////
// nastavení editaèního módu Toolbar (podle ID bufferu, -1 = vypnout)

void SetToolMode(int bufID);


/////////////////////////////////////////////////////////////////////////////
// nastavení editaèního módu edítaèního panelu (TRUE=OK)

BOOL SetEditMode(int bufID, int index);


/////////////////////////////////////////////////////////////////////////////
// nastavení fokusu na okno editace

void ProgSetFocus();

/////////////////////////////////////////////////////////////////////////////
// update voleb bloku

void ProgUpdateClipboard();


/////////////////////////////////////////////////////////////////////////////
// update UNDO a REDO

void ProgUpdateUndoRedo();


/////////////////////////////////////////////////////////////////////////////
// pøekreslení okna

void UpdateWnd(HWND wnd);


/////////////////////////////////////////////////////////////////////////////
// vykreslení nadpisù oken

void ProgDispNadpis();


/////////////////////////////////////////////////////////////////////////////
// aktualizace zobrazení jména závislých položek

void AktNameTree(int bufID, CText& txt, int refBlok, int refIndex);


/////////////////////////////////////////////////////////////////////////////
// pøekreslení stromù

void UpdateAllTree();


/////////////////////////////////////////////////////////////////////////////
// nastavení fokusu na okno

void ProgOnSetFocus(HWND hWnd);


/////////////////////////////////////////////////////////////////////////////
// ztráta fokusu z okna

//void ProgOnKillFocus(HWND hWnd);


/////////////////////////////////////////////////////////////////////////////
// zmìna velikosti okna

void ProgOnSize();


/////////////////////////////////////////////////////////////////////////////
// vykreslení nadpisù oken

void ProgDispNadpis();


/////////////////////////////////////////////////////////////////////////////
// vykreslení okna

void ProgOnPaint();


/////////////////////////////////////////////////////////////////////////////
// zjištìní informace o ikonì k zobrazení

void ProgOnGetDispInfo(HWND hWnd, TV_DISPINFO* tvd);


/////////////////////////////////////////////////////////////////////////////
// zobrazení položky (napø. po hledání)

void DispItem(int bufID, int index);


/////////////////////////////////////////////////////////////////////////////
// zjištìní bufferu programu z handle okna (NULL = není)

CBufProg* BufProgFromHandle(HWND hWnd);


/////////////////////////////////////////////////////////////////////////////
// zjištìní ID bufferu programu z handle okna (-1 = není)

int BufProgIDFromHandle(HWND hWnd);


/////////////////////////////////////////////////////////////////////////////
// zahájení rozvinutí položky (vrací TRUE=zákaz zmìny)

BOOL ProgOnExpanding(HWND hWnd, TV_ITEM* tvi, BOOL expand);


/////////////////////////////////////////////////////////////////////////////
// zmìna rozvinutí položky

void ProgOnExpand(HWND hWnd, TV_ITEM* tvi, BOOL expand);


/////////////////////////////////////////////////////////////////////////////
// zmìna vybrané položky

void ProgOnSelChanged(HWND hWnd, NM_TREEVIEW* tv);


/////////////////////////////////////////////////////////////////////////////
// posun myši

void ProgOnMouseMove(UINT flags, int x, int y);


/////////////////////////////////////////////////////////////////////////////
// uvolnìní tlaèítka myši

void ProgOnButtonUp(UINT keys, BOOL right);


/////////////////////////////////////////////////////////////////////////////
// stisk tlaèítka myši (vrací TRUE=obslouženo)

BOOL ProgOnButtonDown(UINT flags, int x, int y, BOOL right, BOOL dclick);


/////////////////////////////////////////////////////////////////////////////
// zvìtšení mìøítka zobrazení

void ProgOnZoomIn();


/////////////////////////////////////////////////////////////////////////////
// zmenšení mìøítka zobrazení

void ProgOnZoomOut();


//void ProgOnReIcon();


/////////////////////////////////////////////////////////////////////////////
// dvojklik myší - editace prvku

void ProgOnDblClk(HWND hWnd);


/////////////////////////////////////////////////////////////////////////////
// Enter - editace prvku

void ProgOnEnter(HWND hWnd);


/////////////////////////////////////////////////////////////////////////////
// obsluha modifikace

// zapnutí modifikace
void SetModi();

// vypnutí modifikace
void ResModi();

// podmínìné uložení pøi modifikaci (TRUE=pokraèovat)
BOOL TestModi();


/////////////////////////////////////////////////////////////////////////////
// obsluha zobrazení kurzoru myši (vrací TRUE=nastaveno)

BOOL ProgOnSetCursor();


/////////////////////////////////////////////////////////////////////////////
// pøepnutí stavu LOCK prvku pod kurzorem

void ProgOnLock();

/////////////////////////////////////////////////////////////////////////////
// pøepnutí stavu OFF prvku pod kurzorem

void ProgOnOff();

/////////////////////////////////////////////////////////////////////////////
// pøepnutí stavu DEF_NAME prvku pod kurzorem

void ProgOnDefName();

/////////////////////////////////////////////////////////////////////////////
// pøepnutí stavu DEF_ICON prvku pod kurzorem

void ProgOnDefIcon();

/////////////////////////////////////////////////////////////////////////////
// aktualizace pøepínaèù pro vybranou položku

void ProgAktItem();

/////////////////////////////////////////////////////////////////////////////
// aktualizace pøíznakù LOCK a OFF bufferù

void UpdateLock();

/////////////////////////////////////////////////////////////////////////////
// zahájení editace ikony prvku

void ProgOnEditIcon();

/////////////////////////////////////////////////////////////////////////////
// zahájení/ukonèení editace jména prvku

void ProgOnEditName();

/////////////////////////////////////////////////////////////////////////////
// zahájení editace jména prvku (vrací TRUE=pøerušit editaci)

BOOL ProgOnBeginLabelEdit(HWND hWnd, HTREEITEM hItem);


/////////////////////////////////////////////////////////////////////////////
// ukonèení editace jména prvku

void ProgOnEndLabelEdit(HWND hWnd, TV_ITEM* tvi);


/////////////////////////////////////////////////////////////////////////////
// obsluha klávesy (vrací TRUE=zrušení klávesy)

BOOL ProgOnKeyDown(HWND hWnd, int key, int data);


/////////////////////////////////////////////////////////////////////////////
// následující deklarace

void ProgOnRefNext();


/////////////////////////////////////////////////////////////////////////////
// pøedešlá deklarace

void ProgOnRefPrev();


/////////////////////////////////////////////////////////////////////////////
// definice deklarace

void ProgOnRefDef();

/////////////////////////////////////////////////////////////////////////////
// hledání textu

void ProgOnFind();
void ProgOnFindNext();
void ProgOnFindPrev();


/////////////////////////////////////////////////////////////////////////////
// horizontální posuvník

void ProgOnHScroll(int code, int pos);

/////////////////////////////////////////////////////////////////////////////
// vertikální posuvník

void ProgOnVScroll(int code, int pos);

/////////////////////////////////////////////////////////////////////////////
// obsluha èasovaèe (TRUE=obslouženo)

BOOL ProgOnTimer(UINT timerID);

/////////////////////////////////////////////////////////////////////////////
// vstup znaku z klávesnice (TRUE=obslouženo)

BOOL ProgOnChar(HWND hWnd, TCHAR znak);

/////////////////////////////////////////////////////////////////////////////
// výbìr všeho

void ProgOnAll();

/////////////////////////////////////////////////////////////////////////////
// zrušení bloku

void ProgOnDelete();

/////////////////////////////////////////////////////////////////////////////
// kopie bloku

void ProgOnCopy();

/////////////////////////////////////////////////////////////////////////////
// vystøížení bloku

void ProgOnCut();

/////////////////////////////////////////////////////////////////////////////
// vložení bloku

void ProgOnPaste();

/////////////////////////////////////////////////////////////////////////////
// UNDO

void ProgOnUndo();

/////////////////////////////////////////////////////////////////////////////
// REDO

void ProgOnRedo();

/////////////////////////////////////////////////////////////////////////////
// nastavení fokusu na okno editoru s opoždìním

void SetFocusEdit();
void SetFocusEditMode(int buf, int inx);

/////////////////////////////////////////////////////////////////////////////
// zobrazení souøadnic grafického editoru

void DispMouseXY();

/////////////////////////////////////////////////////////////////////////////
// vykreslení prvku (vrací TRUE=obslouženo)

BOOL ProgOnDrawItem(DRAWITEMSTRUCT* di);

/////////////////////////////////////////////////////////////////////////////
// nastavení rozmìrù obrázku/plochy

void ProgOnDimen();

/////////////////////////////////////////////////////////////////////////////
// zapnutí/vypnutí rastru

void ProgOnRastr();

/////////////////////////////////////////////////////////////////////////////
// start pøehrávání

void ProgOnPlay();

/////////////////////////////////////////////////////////////////////////////
// pozastavení nahrávání, pøehrávání

void ProgOnPause();

/////////////////////////////////////////////////////////////////////////////
// zastavení nahrávání i pøehrávání

void ProgOnStop();

/////////////////////////////////////////////////////////////////////////////
// pøepnutí pøíznaku opakování

void ProgOnLoop();

/////////////////////////////////////////////////////////////////////////////
// start nahrávání

void ProgOnRecord();

/////////////////////////////////////////////////////////////////////////////
// pøevíjení zpìt

void ProgOnRew();

/////////////////////////////////////////////////////////////////////////////
// pøevíjení vpøed

void ProgOnFrw();

/////////////////////////////////////////////////////////////////////////////
// start programu

void ProgOnStart();

/////////////////////////////////////////////////////////////////////////////
// zapnutí režimu preview obrázku

void PreviewPicOn();

/////////////////////////////////////////////////////////////////////////////
// vypnutí režimu preview obrázku

void PreviewPicOff();

/////////////////////////////////////////////////////////////////////////////
// zastaveòí preview, zahájení nového èítání èasu

void PreviewStop();

/////////////////////////////////////////////////////////////////////////////
// zahájení preview pøehrávání souboru pod kurzorem

void PreviewStart();

/////////////////////////////////////////////////////////////////////////////
// nulování pøíznakù naètení obrázkù pro preview

void PreviewPicNul();
