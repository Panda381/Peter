
/***************************************************************************\
*																			*
*							Obsluha editace souboru							*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// polo�ka inicializa�n� tabulky funkc�

typedef struct INITFUNC_ {
	short	SrcMask;		// maska zdrojov�ch vlastnost�
	short	DstMask;		// maska c�lov�ch vlastnost�
	short	Param;			// parametry
	char*	Import;			// text pro import funkce
} INITFUNC;

// inicializa�n� tabulka funkc�
extern	const INITFUNC InitTabFunc[];

/////////////////////////////////////////////////////////////////////////////
// edita�n� pole

extern	int			FocusBuf;						// okno s fokusem (= ��slo bufferu, BufEdiID = edita�n� pole)

extern	int			EditMode;						// aktivn� edita�n� pole (= ��slo bufferu)
extern	int			EditBuf;						// editovan� buffer
extern	int			EditItem;						// editovan� polo�ka v bufferu
extern	int			EditItemSprite;					// sprajt s editovanou ikonou (�schova pro historii)
extern	int			EditItemPic;					// editovan� obr�zek sprajtu (�schova pro historii)

extern	int			EditX;							// sou�adnice X po��tku edita�n�ho pole
extern	int			EditY;							// sou�adnice Y po��tku edita�n�ho pole
extern	int			EditWidth;						// ���ka edita�n�ho pole
extern	int			EditHeight;						// v��ka edita�n�ho pole

extern	int			ToolMode;						// aktivn� edita�n� m�d Toolbaru (=��slo bufferu)

extern	UINT		FocusTimer;						// �asova� pro nastaven� fokusu
extern	int			FocusTimerN;					// ��ta� pro nastaven� fokusu

/////////////////////////////////////////////////////////////////////////////
// hl�en� o zm�n� adres��e

extern	HANDLE		FunctionDirChange;				// hl�en� o zm�n� adres��e - funkce
extern	HANDLE		NumberDirChange;				// hl�en� o zm�n� adres��e - ��sla
extern	HANDLE		TextDirChange;					// hl�en� o zm�n� adres��e - text
extern	HANDLE		BoolDirChange;					// hl�en� o zm�n� adres��e - logick� hodnoty
extern	HANDLE		IconDirChange;					// hl�en� o zm�n� adres��e - ikony
extern	HANDLE		MapDirChange;					// hl�en� o zm�n� adres��e - plochy
extern	HANDLE		PictureDirChange;				// hl�en� o zm�n� adres��e - obr�zky
extern	HANDLE		SpriteDirChange;				// hl�en� o zm�n� adres��e - sprajty
extern	HANDLE		SoundDirChange;					// hl�en� o zm�n� adres��e - zvuk
extern	HANDLE		MusicDirChange;					// hl�en� o zm�n� adres��e - hudba

extern	HANDLE		FunctionDirChange2;				// hl�en� o zm�n� adres��e - alternativn� funkce
extern	HANDLE		NumberDirChange2;				// hl�en� o zm�n� adres��e - alternativn� ��sla
extern	HANDLE		TextDirChange2;					// hl�en� o zm�n� adres��e - alternativn� text
extern	HANDLE		BoolDirChange2;					// hl�en� o zm�n� adres��e - alternativn� logick� hodnoty
extern	HANDLE		IconDirChange2;					// hl�en� o zm�n� adres��e - alternativn� ikony
extern	HANDLE		MapDirChange2;					// hl�en� o zm�n� adres��e - alternativn� plochy
extern	HANDLE		PictureDirChange2;				// hl�en� o zm�n� adres��e - alternativn� obr�zky
extern	HANDLE		SpriteDirChange2;				// hl�en� o zm�n� adres��e - alternativn� sprajty
extern	HANDLE		SoundDirChange2;				// hl�en� o zm�n� adres��e - alternativn� zvuk
extern	HANDLE		MusicDirChange2;				// hl�en� o zm�n� adres��e - alternativn� hudba

extern	int			ProgDirChangeIgnore;			// ��ta� pro ignorov�n� hl�en� zm�n po vlastn� zm�n�

extern	int			ProgStatusWidth;				// ���ka druh�ho pole programov�ho ��dku

//#define MOUSEINV -100000							// neplatn� sou�adnice my�i
//extern	int			mX;								// sou�adnice X my�i
//extern	int			mY;								// sou�adnice Y my�i

extern	BOOL		Modi;							// p��znak modifikace souboru

/////////////////////////////////////////////////////////////////////////////
// obsluha kurzoru my�i

extern	int		MouseX;								// sou�adnice X my�i
extern	int		MouseY;								// sou�adnice Y my�i


/////////////////////////////////////////////////////////////////////////////
// obsluha editace ��sla

extern	HWND	EditNumWnd;						// edita�n� okno ��sla

/////////////////////////////////////////////////////////////////////////////
// editace ��seln� polo�ky plochy

extern	HWND	EditMapNumWnd;					// edita�n� okno plochy (NULL=nen�)

/////////////////////////////////////////////////////////////////////////////
// obsluha editace logick�ch hodnot

extern	HWND	EditLogWnd;						// edita�n� okno logick�ch hodnot

/////////////////////////////////////////////////////////////////////////////
// obsluha editace textu

extern	HWND	EditTextWnd;					// edita�n� okno textu

/////////////////////////////////////////////////////////////////////////////
// posuvn�ky pro editory

extern	HWND	HScroll;						// horizont�ln� posuvn�k
extern	HWND	VScroll;						// vertik�ln� posuvn�k
extern	BOOL	HScrollDisp;					// je horizont�ln� posuvn�k
extern	BOOL	VScrollDisp;					// je vertik�ln� posuvn�k
#define	TRACKSIRKA 16							// ���ka posuvn�ku

/////////////////////////////////////////////////////////////////////////////
// glob�ln� a lok�ln� prom�nn�

extern	BOOL		ProgMode;						// p��znak m�du editace
//extern	HWND		EditName;						// editor jm�na prvku
//extern	HWND		EditFind;						// editor textu k hled�n�
extern	int			ToolBarProgN;					// po�et z�kladn�ch prvk� panelu n�stroj�

extern	HWND		TreeCls;						// okno t��d
extern	HWND		TreeObj;						// okno glob�ln�ch objekt�
extern	HWND		TreeLoc;						// okno lok�ln�ch objekt�
extern	HWND		TreeEdi;						// okno editoru
extern	HWND		TreeStr;						// okno struktur

extern	HIMAGELIST	ProgImageList;					// seznam obr�zk�
extern	HIMAGELIST	ProgImageListMedium;			// seznam st�edn�ch obr�zk�
extern	HIMAGELIST	ProgImageListSmall;				// seznam mal�ch obr�zk�
extern	HIMAGELIST	ProgStateList;					// seznam stavov�ch ikon
extern	HIMAGELIST	ProgStateListMedium;			// seznam st�edn�ch stavov�ch ikon
extern	HIMAGELIST	ProgStateListSmall;				// seznam mal�ch stavov�ch ikon


#define CLASSFNCINDEX		0		// index FNC v bufferu t��d
#define CLASSICONINDEX		4		// index ICON v bufferu t��d
#define CLASSMAPINDEX		5		// index MAP v bufferu t��d
#define CLASSPICINDEX		6		// index BACK v bufferu t��d
#define CLASSSPRITEINDEX	9		// index SPRITE v bufferu t��d

/////////////////////////////////////////////////////////////////////////////
// obsluha editace jm�na prvku

extern	BOOL		EditName;						// p��znak prob�haj�c� editace

/////////////////////////////////////////////////////////////////////////////
// inicializa�n� buffery (p�ipravuj� se jen jednou p�i startu programu)
//
// P�i pou��v�n� buffer� se po��t� s t�m, �e polo�ky jsou ulo�eny po �ad�
// od za��tku bufferu, po�et polo�ek odpov�d� po�tu (a po�ad�) funkc�.

extern	const int	FncNum;							// po�et funkc�
extern	CBufIcon	IconFnc;						// ikony funkc�

extern	BOOL		PreviewPic;						// prob�h� preview obr�zku

/////////////////////////////////////////////////////////////////////////////
// buffery

extern	CBufProg	BufProg[PROGBUFNUM];			// buffery programu
extern	CBufReal	Real;							// buffer re�ln�ch ��sel
extern	CBufMultiText	Text;							// buffer text�
extern	CBufBool	Bool;							// buffer logick�ch prom�nn�ch
extern	CBufIcon	Icon;							// buffer ikon
extern	CBufIcon	IconState;						// buffer stavov�ch ikon
extern	CBufMap		Map;							// buffer ploch
extern	CBufPic		Picture;						// buffer obr�zk�
extern	CBufSprite	Sprite;							// buffer sprajt�
extern	CBufSound	Sound;							// buffer zvuk�
extern	CBufMusic	Music;							// buffer hudby

extern	CSprite		Sprite0;						// sprajt 0 (Petr)
extern	CSprite		Sprite1;						// sprajt 1 (Petra)
extern	CBufIcon	Icon1;							// buffer s 1 ikonou k zobrazen�

//#define		ICONNUM 31								// velikost bufferu ikon (pro TRUECOLOR max. 63!)
//extern	CBufIcon	Icon16;							// buffer ikon s velikost� 16x16
//extern	CBufIndex	Icon16List;						// buffer index� ikon s velikost� 16x16
//extern	int			Icon16Next;						// p��t� ikona pro z�pis
//extern	CBufIcon	Icon32;							// buffer ikon s velikost� 32x32
//extern	CBufIndex	Icon32List;						// buffer index� ikon s velikost� 32x32
//extern	int			Icon32Next;						// p��t� ikona pro z�pis

#define	BufInt		BufProg[BufIntID]				// intern� buffer
#define	BufCls		BufProg[BufClsID]				// buffer t��d
#define	BufObj		BufProg[BufObjID]				// glob�ln� buffer
#define BufLoc		BufProg[BufLocID]				// lok�ln� buffer
#define	BufEdi		BufProg[BufEdiID]				// edita�n� buffer
#define	BufStr		BufProg[BufStrID]				// buffer struktur

extern	CBufUndo	Undo;							// buffer undo a redo

extern	int			GroupIndex;						// index do bufferu BufStr na IDF_GROUP

/////////////////////////////////////////////////////////////////////////////
// combobox panely

extern	HWND		ToolEditIcon;					// panel n�stroj� editoru ikon
extern	HWND		ToolEditIconWidth;				// panel n�stroj� volby ���ky
extern	HWND		ToolEditMap;					// panel n�stroj� editoru ploch

/////////////////////////////////////////////////////////////////////////////
// tabulky p��kaz� v COMBO boxu

extern	int ToolBarEditPicTools[];					// n�stroje EDITICON
extern	int ToolBarEditPicWidths[];					// ���ky EDITICON
extern	int ToolBarEditMapTools[];					// n�stroje EDITMAP


/////////////////////////////////////////////////////////////////////////////
// nastaven� menu programu

void ProgSetMenu();

/////////////////////////////////////////////////////////////////////////////
// aktualizace po zm�n� jazyku

void ProgReloadJazyk();


/////////////////////////////////////////////////////////////////////////////
// prvn� inicializace p�i startu programu (hlavn� okno i SELECT je ji� vytvo�eno)

void ProgStartInit();


/////////////////////////////////////////////////////////////////////////////
// zah�jen� re�imu editace souboru

void ProgInit();


/////////////////////////////////////////////////////////////////////////////
// ukon�en� re�imu editace souboru

void ProgTerm();


/////////////////////////////////////////////////////////////////////////////
// zah�jen� sledov�n� zm�n adres��e

void BeginDirChange(HANDLE* dir1, HANDLE* dir2, CText& path1, CText& path2);


/////////////////////////////////////////////////////////////////////////////
// update voleb pro ZOOM

void ProgUpdateInOut();


/////////////////////////////////////////////////////////////////////////////
// nastaven� edita�n�ho m�du Toolbar (podle ID bufferu, -1 = vypnout)

void SetToolMode(int bufID);


/////////////////////////////////////////////////////////////////////////////
// nastaven� edita�n�ho m�du ed�ta�n�ho panelu (TRUE=OK)

BOOL SetEditMode(int bufID, int index);


/////////////////////////////////////////////////////////////////////////////
// nastaven� fokusu na okno editace

void ProgSetFocus();

/////////////////////////////////////////////////////////////////////////////
// update voleb bloku

void ProgUpdateClipboard();


/////////////////////////////////////////////////////////////////////////////
// update UNDO a REDO

void ProgUpdateUndoRedo();


/////////////////////////////////////////////////////////////////////////////
// p�ekreslen� okna

void UpdateWnd(HWND wnd);


/////////////////////////////////////////////////////////////////////////////
// vykreslen� nadpis� oken

void ProgDispNadpis();


/////////////////////////////////////////////////////////////////////////////
// aktualizace zobrazen� jm�na z�visl�ch polo�ek

void AktNameTree(int bufID, CText& txt, int refBlok, int refIndex);


/////////////////////////////////////////////////////////////////////////////
// p�ekreslen� strom�

void UpdateAllTree();


/////////////////////////////////////////////////////////////////////////////
// nastaven� fokusu na okno

void ProgOnSetFocus(HWND hWnd);


/////////////////////////////////////////////////////////////////////////////
// ztr�ta fokusu z okna

//void ProgOnKillFocus(HWND hWnd);


/////////////////////////////////////////////////////////////////////////////
// zm�na velikosti okna

void ProgOnSize();


/////////////////////////////////////////////////////////////////////////////
// vykreslen� nadpis� oken

void ProgDispNadpis();


/////////////////////////////////////////////////////////////////////////////
// vykreslen� okna

void ProgOnPaint();


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� informace o ikon� k zobrazen�

void ProgOnGetDispInfo(HWND hWnd, TV_DISPINFO* tvd);


/////////////////////////////////////////////////////////////////////////////
// zobrazen� polo�ky (nap�. po hled�n�)

void DispItem(int bufID, int index);


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� bufferu programu z handle okna (NULL = nen�)

CBufProg* BufProgFromHandle(HWND hWnd);


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� ID bufferu programu z handle okna (-1 = nen�)

int BufProgIDFromHandle(HWND hWnd);


/////////////////////////////////////////////////////////////////////////////
// zah�jen� rozvinut� polo�ky (vrac� TRUE=z�kaz zm�ny)

BOOL ProgOnExpanding(HWND hWnd, TV_ITEM* tvi, BOOL expand);


/////////////////////////////////////////////////////////////////////////////
// zm�na rozvinut� polo�ky

void ProgOnExpand(HWND hWnd, TV_ITEM* tvi, BOOL expand);


/////////////////////////////////////////////////////////////////////////////
// zm�na vybran� polo�ky

void ProgOnSelChanged(HWND hWnd, NM_TREEVIEW* tv);


/////////////////////////////////////////////////////////////////////////////
// posun my�i

void ProgOnMouseMove(UINT flags, int x, int y);


/////////////////////////////////////////////////////////////////////////////
// uvoln�n� tla��tka my�i

void ProgOnButtonUp(UINT keys, BOOL right);


/////////////////////////////////////////////////////////////////////////////
// stisk tla��tka my�i (vrac� TRUE=obslou�eno)

BOOL ProgOnButtonDown(UINT flags, int x, int y, BOOL right, BOOL dclick);


/////////////////////////////////////////////////////////////////////////////
// zv�t�en� m���tka zobrazen�

void ProgOnZoomIn();


/////////////////////////////////////////////////////////////////////////////
// zmen�en� m���tka zobrazen�

void ProgOnZoomOut();


//void ProgOnReIcon();


/////////////////////////////////////////////////////////////////////////////
// dvojklik my�� - editace prvku

void ProgOnDblClk(HWND hWnd);


/////////////////////////////////////////////////////////////////////////////
// Enter - editace prvku

void ProgOnEnter(HWND hWnd);


/////////////////////////////////////////////////////////////////////////////
// obsluha modifikace

// zapnut� modifikace
void SetModi();

// vypnut� modifikace
void ResModi();

// podm�n�n� ulo�en� p�i modifikaci (TRUE=pokra�ovat)
BOOL TestModi();


/////////////////////////////////////////////////////////////////////////////
// obsluha zobrazen� kurzoru my�i (vrac� TRUE=nastaveno)

BOOL ProgOnSetCursor();


/////////////////////////////////////////////////////////////////////////////
// p�epnut� stavu LOCK prvku pod kurzorem

void ProgOnLock();

/////////////////////////////////////////////////////////////////////////////
// p�epnut� stavu OFF prvku pod kurzorem

void ProgOnOff();

/////////////////////////////////////////////////////////////////////////////
// p�epnut� stavu DEF_NAME prvku pod kurzorem

void ProgOnDefName();

/////////////////////////////////////////////////////////////////////////////
// p�epnut� stavu DEF_ICON prvku pod kurzorem

void ProgOnDefIcon();

/////////////////////////////////////////////////////////////////////////////
// aktualizace p�ep�na�� pro vybranou polo�ku

void ProgAktItem();

/////////////////////////////////////////////////////////////////////////////
// aktualizace p��znak� LOCK a OFF buffer�

void UpdateLock();

/////////////////////////////////////////////////////////////////////////////
// zah�jen� editace ikony prvku

void ProgOnEditIcon();

/////////////////////////////////////////////////////////////////////////////
// zah�jen�/ukon�en� editace jm�na prvku

void ProgOnEditName();

/////////////////////////////////////////////////////////////////////////////
// zah�jen� editace jm�na prvku (vrac� TRUE=p�eru�it editaci)

BOOL ProgOnBeginLabelEdit(HWND hWnd, HTREEITEM hItem);


/////////////////////////////////////////////////////////////////////////////
// ukon�en� editace jm�na prvku

void ProgOnEndLabelEdit(HWND hWnd, TV_ITEM* tvi);


/////////////////////////////////////////////////////////////////////////////
// obsluha kl�vesy (vrac� TRUE=zru�en� kl�vesy)

BOOL ProgOnKeyDown(HWND hWnd, int key, int data);


/////////////////////////////////////////////////////////////////////////////
// n�sleduj�c� deklarace

void ProgOnRefNext();


/////////////////////////////////////////////////////////////////////////////
// p�ede�l� deklarace

void ProgOnRefPrev();


/////////////////////////////////////////////////////////////////////////////
// definice deklarace

void ProgOnRefDef();

/////////////////////////////////////////////////////////////////////////////
// hled�n� textu

void ProgOnFind();
void ProgOnFindNext();
void ProgOnFindPrev();


/////////////////////////////////////////////////////////////////////////////
// horizont�ln� posuvn�k

void ProgOnHScroll(int code, int pos);

/////////////////////////////////////////////////////////////////////////////
// vertik�ln� posuvn�k

void ProgOnVScroll(int code, int pos);

/////////////////////////////////////////////////////////////////////////////
// obsluha �asova�e (TRUE=obslou�eno)

BOOL ProgOnTimer(UINT timerID);

/////////////////////////////////////////////////////////////////////////////
// vstup znaku z kl�vesnice (TRUE=obslou�eno)

BOOL ProgOnChar(HWND hWnd, TCHAR znak);

/////////////////////////////////////////////////////////////////////////////
// v�b�r v�eho

void ProgOnAll();

/////////////////////////////////////////////////////////////////////////////
// zru�en� bloku

void ProgOnDelete();

/////////////////////////////////////////////////////////////////////////////
// kopie bloku

void ProgOnCopy();

/////////////////////////////////////////////////////////////////////////////
// vyst��en� bloku

void ProgOnCut();

/////////////////////////////////////////////////////////////////////////////
// vlo�en� bloku

void ProgOnPaste();

/////////////////////////////////////////////////////////////////////////////
// UNDO

void ProgOnUndo();

/////////////////////////////////////////////////////////////////////////////
// REDO

void ProgOnRedo();

/////////////////////////////////////////////////////////////////////////////
// nastaven� fokusu na okno editoru s opo�d�n�m

void SetFocusEdit();
void SetFocusEditMode(int buf, int inx);

/////////////////////////////////////////////////////////////////////////////
// zobrazen� sou�adnic grafick�ho editoru

void DispMouseXY();

/////////////////////////////////////////////////////////////////////////////
// vykreslen� prvku (vrac� TRUE=obslou�eno)

BOOL ProgOnDrawItem(DRAWITEMSTRUCT* di);

/////////////////////////////////////////////////////////////////////////////
// nastaven� rozm�r� obr�zku/plochy

void ProgOnDimen();

/////////////////////////////////////////////////////////////////////////////
// zapnut�/vypnut� rastru

void ProgOnRastr();

/////////////////////////////////////////////////////////////////////////////
// start p�ehr�v�n�

void ProgOnPlay();

/////////////////////////////////////////////////////////////////////////////
// pozastaven� nahr�v�n�, p�ehr�v�n�

void ProgOnPause();

/////////////////////////////////////////////////////////////////////////////
// zastaven� nahr�v�n� i p�ehr�v�n�

void ProgOnStop();

/////////////////////////////////////////////////////////////////////////////
// p�epnut� p��znaku opakov�n�

void ProgOnLoop();

/////////////////////////////////////////////////////////////////////////////
// start nahr�v�n�

void ProgOnRecord();

/////////////////////////////////////////////////////////////////////////////
// p�ev�jen� zp�t

void ProgOnRew();

/////////////////////////////////////////////////////////////////////////////
// p�ev�jen� vp�ed

void ProgOnFrw();

/////////////////////////////////////////////////////////////////////////////
// start programu

void ProgOnStart();

/////////////////////////////////////////////////////////////////////////////
// zapnut� re�imu preview obr�zku

void PreviewPicOn();

/////////////////////////////////////////////////////////////////////////////
// vypnut� re�imu preview obr�zku

void PreviewPicOff();

/////////////////////////////////////////////////////////////////////////////
// zastave�� preview, zah�jen� nov�ho ��t�n� �asu

void PreviewStop();

/////////////////////////////////////////////////////////////////////////////
// zah�jen� preview p�ehr�v�n� souboru pod kurzorem

void PreviewStart();

/////////////////////////////////////////////////////////////////////////////
// nulov�n� p��znak� na�ten� obr�zk� pro preview

void PreviewPicNul();
