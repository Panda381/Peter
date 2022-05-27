
#include "Main.h"

/***************************************************************************\
*																			*
*							Obsluha editace souboru							*
*																			*
\***************************************************************************/

#include	"ProgInit.inc"							// inicializa�n� tabulka funkc�

/////////////////////////////////////////////////////////////////////////////
// glob�ln� a lok�ln� prom�nn�

BOOL		ProgMode = FALSE;						// p��znak m�du editace
int			ToolBarProgN;							// po�et z�kladn�ch prvk� panelu n�stroj�

HWND		TreeCls = NULL;							// okno t��d
HWND		TreeObj = NULL;							// okno glob�ln�ch objekt�
HWND		TreeLoc = NULL;							// okno lok�ln�ch objekt�
HWND		TreeEdi = NULL;							// okno editoru
HWND		TreeStr = NULL;							// okno struktur

#define		NADPISHEIGHT 22							// v��ka okna nadpisu
int			ProgStatusWidth = 150;					// ���ka druh�ho pole programov�ho ��dku

HIMAGELIST	ProgImageList = NULL;					// seznam obr�zk�
HIMAGELIST	ProgImageListMedium = NULL;				// seznam st�edn�ch obr�zk�
HIMAGELIST	ProgImageListSmall = NULL;				// seznam mal�ch obr�zk�
HIMAGELIST	ProgStateList = NULL;					// seznam stavov�ch ikon
HIMAGELIST	ProgStateListMedium = NULL;				// seznam st�edn�ch stavov�ch ikon
HIMAGELIST	ProgStateListSmall = NULL;				// seznam mal�ch stavov�ch ikon

BOOL		Modi = FALSE;							// p��znak modifikace souboru

//BOOL		NoSelAkt = FALSE;						// neaktualizovat zm�nu v�b�ru

/////////////////////////////////////////////////////////////////////////////
// obsluha editace jm�na prvku

BOOL		EditName = FALSE;						// p��znak prob�haj�c� editace
HWND		EditNameWnd = NULL;						// edita�n� okno

/////////////////////////////////////////////////////////////////////////////
// obsluha editace ��sla

HWND		EditNumWnd = NULL;						// edita�n� okno ��sla

/////////////////////////////////////////////////////////////////////////////
// editace ��seln� polo�ky plochy

HWND		EditMapNumWnd = NULL;					// edita�n� okno plochy (NULL=nen�)

/////////////////////////////////////////////////////////////////////////////
// obsluha editace logick�ch hodnot

HWND		EditLogWnd = NULL;						// edita�n� okno logick�ch hodnot

/////////////////////////////////////////////////////////////////////////////
// obsluha editace textu

HWND		EditTextWnd = NULL;						// edita�n� okno textu

/////////////////////////////////////////////////////////////////////////////
// posuvn�ky pro editory

HWND	HScroll = NULL;								// horizont�ln� posuvn�k
HWND	VScroll = NULL;								// vertik�ln� posuvn�k
BOOL	HScrollDisp = FALSE;						// je horizont�ln� posuvn�k
BOOL	VScrollDisp = FALSE;						// je vertik�ln� posuvn�k

/////////////////////////////////////////////////////////////////////////////
// p�ed�ly oken

BOOL		Tracking = FALSE;						// je ta�en� p�ed�l�
BOOL		VTracking = FALSE;						// je ta�en vertik�ln� p�ed�l
BOOL		LTracking = FALSE;						// je ta�en lev� p�ed�l

#define	TRACKOFF	3								// odchylka sou�adnice pro ta�en� p�ed�lu

int			TrackX1;								// lev� vertik�ln� p�ed�l
int			TrackY1;								// lev� horizont�ln� p�ed�l
int			TrackX2;								// prav� vertik�ln� p�ed�l
int			TrackY2;								// prav� horizont�ln� p�ed�l

/////////////////////////////////////////////////////////////////////////////
// edita�n� pole

int			FocusBuf = BufObjID;					// okno s fokusem (= ��slo bufferu, BufEdiID = edita�n� pole)
int			OldFocusBuf = -1;						// star� okno s fokusem (kv�li p�ekreslen� star�ho kurzoru)

int			EditMode = -1;							// aktivn� edita�n� pole (= ��slo bufferu)
int			EditBuf = BufEdiID;						// editovan� buffer
int			EditItem = 0;							// editovan� polo�ka v bufferu
int			EditItemSprite = 0;						// sprajt s editovanou ikonou (�schova pro historii)
int			EditItemPic = 0;						// editovan� obr�zek sprajtu (�schova pro historii)

int			EditX = 0;								// sou�adnice X po��tku edita�n�ho pole
int			EditY = 0;								// sou�adnice Y po��tku edita�n�ho pole
int			EditWidth = 0;							// ���ka edita�n�ho pole
int			EditHeight = 0;							// v��ka edita�n�ho pole

int			ToolMode = -1;							// aktivn� edita�n� m�d Toolbaru (=��slo bufferu)


/////////////////////////////////////////////////////////////////////////////
// hl�en� o zm�n� adres��e

HANDLE		FunctionDirChange	= INVALID_HANDLE_VALUE;	// hl�en� o zm�n� adres��e - funkce
HANDLE		NumberDirChange		= INVALID_HANDLE_VALUE;	// hl�en� o zm�n� adres��e - ��sla
HANDLE		TextDirChange		= INVALID_HANDLE_VALUE;	// hl�en� o zm�n� adres��e - text
HANDLE		BoolDirChange		= INVALID_HANDLE_VALUE;	// hl�en� o zm�n� adres��e - logick� hodnoty
HANDLE		IconDirChange		= INVALID_HANDLE_VALUE;	// hl�en� o zm�n� adres��e - ikony
HANDLE		MapDirChange		= INVALID_HANDLE_VALUE;	// hl�en� o zm�n� adres��e - plochy
HANDLE		PictureDirChange	= INVALID_HANDLE_VALUE;	// hl�en� o zm�n� adres��e - obr�zky
HANDLE		SpriteDirChange		= INVALID_HANDLE_VALUE;	// hl�en� o zm�n� adres��e - sprajty
HANDLE		SoundDirChange		= INVALID_HANDLE_VALUE;	// hl�en� o zm�n� adres��e - zvuk
HANDLE		MusicDirChange		= INVALID_HANDLE_VALUE;	// hl�en� o zm�n� adres��e - hudba

HANDLE		FunctionDirChange2	= INVALID_HANDLE_VALUE;	// hl�en� o zm�n� adres��e - alternativn� funkce
HANDLE		NumberDirChange2	= INVALID_HANDLE_VALUE;	// hl�en� o zm�n� adres��e - alternativn� ��sla
HANDLE		TextDirChange2		= INVALID_HANDLE_VALUE;	// hl�en� o zm�n� adres��e - alternativn� text
HANDLE		BoolDirChange2		= INVALID_HANDLE_VALUE;	// hl�en� o zm�n� adres��e - alternativn� logick� hodnoty
HANDLE		IconDirChange2		= INVALID_HANDLE_VALUE;	// hl�en� o zm�n� adres��e - alternativn� ikony
HANDLE		MapDirChange2		= INVALID_HANDLE_VALUE;	// hl�en� o zm�n� adres��e - alternativn� plochy
HANDLE		PictureDirChange2	= INVALID_HANDLE_VALUE;	// hl�en� o zm�n� adres��e - alternativn� obr�zky
HANDLE		SpriteDirChange2	= INVALID_HANDLE_VALUE;	// hl�en� o zm�n� adres��e - alternativn� sprajty
HANDLE		SoundDirChange2		= INVALID_HANDLE_VALUE;	// hl�en� o zm�n� adres��e - alternativn� zvuk
HANDLE		MusicDirChange2		= INVALID_HANDLE_VALUE;	// hl�en� o zm�n� adres��e - alternativn� hudba

#define		ProgDirChangeTimerID 19118			// ID �asova�e hl�en� zm�ny adres��e
UINT		ProgDirChangeTimer = 0;				// �asova� pro hl�en� zm�n adres��e
int			ProgDirChangeIgnore = 0;			// ��ta� pro ignorov�n� hl�en� zm�n po vlastn� zm�n�

/////////////////////////////////////////////////////////////////////////////
// p�echodn� editovan� prvek v bufferu t��d

//CText		EditClassName;							// jm�no editovan�ho souboru
//BOOL		EditClassModi = FALSE;					// p��znak nutnosti ulo�en� dat
//int			EditClassBuf = -1;						// editovan� buffer bufferu t��d
//int			EditClassItem = -1;						// editovan� polo�ka v bufferu
//int			EditClassSprite = -1;					// sprajt s editovanou ikonou
//int			EditClassIcon = -1;						// editovan� ikona sprajtu

/////////////////////////////////////////////////////////////////////////////
// obsluha kurzoru my�i

#define MOUSEINV -100000				// neplatn� sou�adnice my�i
int		MouseX = MOUSEINV;				// sou�adnice X my�i
int		MouseY = MOUSEINV;				// sou�adnice Y my�i


/////////////////////////////////////////////////////////////////////////////
// opo�d�n� nastaven� fokusu

#define		FocusTimerID 19128						// ID �asova�e pro nastaven� fokusu
UINT		FocusTimer = 0;							// �asova� pro nastaven� fokusu
int			FocusTimerN = 0;						// ��ta� pro nastaven� fokusu
int			FocusNextBuf = BufEdiID;				// buffer k nastaven� jako aktivn�

int			FocusEditModeBuf = -1;					// buffer s editac� (-1 = nen�)
int			FocusEditModeInx = -1;					// polo�ka s editac�

/////////////////////////////////////////////////////////////////////////////
// obsluha preview

#define		PreviewTimerID 19135					// ID �asova�e pro preview
UINT		PreviewTimer = 0;						// �asova� pro preview
int			PreviewTimerN = -1;						// ��ta� pro preview

BOOL		PreviewWav = FALSE;						// prob�h� preview WAV
MCIDEVICEID	PreviewMid = 0;							// ID za��zen� pro MID
BOOL		PreviewPic = FALSE;						// prob�h� preview obr�zku

#define		PreviewPicSize 3						// po�et polo�ek pro cachov�n� obr�zk�
#define		PreviewPicAkt 0							// index aktu�ln�ho obr�zku
#define		PreviewPicNxt 1							// index n�sleduj�c�ho obr�zku
#define		PreviewPicPre 2							// index p�ede�l�ho obr�zku

CPicture	PreviewPicPic[PreviewPicSize];			// obr�zky pro cachov�n�
CText		PreviewPicName[PreviewPicSize];			// jm�na obr�zk� pro cachov�n� (pr�zdn� = nen�)
BOOL		PreviewPicLoad[PreviewPicSize];			// p��znaky na��t�n� obr�zk� (TRUE = byl na��t�n)

BOOL		PreviewPicVpred = TRUE;					// p�ednost obr�zky sm�rem vp�ed

TCHAR		PreviewTempName[MAX_PATH + 101];		// jm�no p�echodn�ho souboru p�i p�ehr�v�n� MP3
BOOL		IsPreviewTempName = FALSE;				// p��znak vytvo�en� p�echodn�ho souboru MP3


/////////////////////////////////////////////////////////////////////////////
// hled�n� textu

HWND		FindDialog = NULL;						// okno hled�n�
HWND		FindStringEdit = NULL;					// handle editoru textu k hled�n�

/////////////////////////////////////////////////////////////////////////////
// combobox panely

HWND		ToolEditIcon = NULL;					// panel n�stroj� editoru ikon
HWND		ToolEditIconWidth = NULL;				// panel n�stroj� volby ���ky
HWND		ToolEditMap = NULL;						// panel n�stroj� editoru ploch

/////////////////////////////////////////////////////////////////////////////
// inicializa�n� buffery (p�ipravuj� se jen jednou p�i startu programu)
//
// P�i pou��v�n� buffer� se po��t� s t�m, �e polo�ky jsou ulo�eny po �ad�
// od za��tku bufferu, po�et polo�ek odpov�d� po�tu (a po�ad�) funkc�.

const int FncNum = sizeof(InitTabFunc) / sizeof(InitTabFunc[0]);	// po�et funkc�
CBufMultiText	TextFnc;							// texty funkc�
CBufIcon	IconFnc;								// ikony funkc�

/////////////////////////////////////////////////////////////////////////////
// buffery

CBufProg	BufProg[PROGBUFNUM];					// (0 a� 5) buffery programu (0=intern�)
CBufReal	Real;									// buffer re�ln�ch ��sel
CBufMultiText	Text;								// buffer text�
CBufBool	Bool;									// buffer logick�ch prom�nn�ch
CBufIcon	Icon;									// buffer ikon
CBufIcon	IconState;								// buffer stavov�ch ikon
CBufMap		Map;									// buffer ploch
CBufPic		Picture;								// buffer obr�zk�
CBufSprite	Sprite;									// buffer sprajt�
CBufSound	Sound;									// buffer zvuk�
CBufMusic	Music;									// buffer hudby

CSprite		Sprite0;								// sprajt 0 (Petr)
CSprite		Sprite1;								// sprajt 1 (Petra)
CBufIcon	Icon1;									// buffer s 1 ikonou k zobrazen�

//CBufIcon	Icon16;									// buffer ikon s velikost� 16x16
//CBufIndex	Icon16List;								// buffer index� ikon s velikost� 16x16
//int			Icon16Next = 0;							// p��t� ikona pro z�pis
//CBufIcon	Icon32;									// buffer ikon s velikost� 32x32
//CBufIndex	Icon32List;								// buffer index� ikon s velikost� 32x32
//int			Icon32Next = 0;							// p��t� ikona pro z�pis

CBufUndo	Undo;									// buffer undo a redo

int			GroupIndex = -1;						// index do bufferu BufStr na IDF_GROUP

/////////////////////////////////////////////////////////////////////////////
// definice z�kladn�ch tla��tek pro toolbar Program

TBBUTTON ToolBarProgram[] = {
	{	ButtonStart,		IDN_START,			TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	ButtonClose,		IDN_CLOSE,			TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	ButtonSave,			IDN_SAVE,			TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	0,					0,					TBSTATE_ENABLED, TBSTYLE_SEP,		0, 0},
	{	ButtonUndo,			IDN_UNDO,			TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	ButtonRedo,			IDN_REDO,			TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	ButtonCut,			IDN_CUT,			TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	ButtonCopy,			IDN_COPY,			TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	ButtonPaste,		IDN_PASTE,			TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	ButtonClear,		IDN_CLEAR,			TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	ButtonAll,			IDN_ALL,			TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	0,					0,					TBSTATE_ENABLED, TBSTYLE_SEP,		0, 0},
	{	ButtonIn,			IDN_IN,				TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	ButtonOut,			IDN_OUT,			TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	ButtonHistPrev,		IDN_HIST_PREV,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	ButtonHistNext,		IDN_HIST_NEXT,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	0,					0,					TBSTATE_ENABLED, TBSTYLE_SEP,		0, 0},
};

#define ToolBarProgramNum (sizeof(ToolBarProgram)/sizeof(TBBUTTON))

// editor programu
TBBUTTON ToolBarEditProg[] = {
	{	ButtonLock,			IDN_LOCK,			TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	ButtonOff,			IDN_OFF,			TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	ButtonEditName,		IDN_EDIT_NAME,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	ButtonDefName,		IDN_DEF_NAME,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	ButtonEditIcon,		IDN_EDIT_ICON,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	ButtonDefIcon,		IDN_DEF_ICON,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	0,					0,					TBSTATE_ENABLED, TBSTYLE_SEP,		0, 0},
	{	ButtonRefDef,		IDN_REF_DEF,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	ButtonRefPrev,		IDN_REF_PREV,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	ButtonRefNext,		IDN_REF_NEXT,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	0,					0,					TBSTATE_ENABLED, TBSTYLE_SEP,		0, 0},
	{	ButtonFind,			IDN_FIND,			TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	ButtonFindPrev,		IDN_FIND_PREV,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	ButtonFindNext,		IDN_FIND_NEXT,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
};

#define ToolBarEditProgNum (sizeof(ToolBarEditProg)/sizeof(TBBUTTON))

// editor ikon
TBBUTTON ToolBarEditIcon[] = {
	{	ButtonRastr,		IDN_RASTR,			TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK,	0, 0},
	{	ButtonEditBlok,		IDN_EDITBLOK,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	ButtonOtisk,		IDN_OTISK,			TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
};

#define ToolBarEditIconNum (sizeof(ToolBarEditIcon)/sizeof(TBBUTTON))

// editor plochy
TBBUTTON ToolBarEditMap[] = {
	{	ButtonDimen,		IDN_DIMEN,			TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	ButtonEditMap,		IDN_EDITMAP,		TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK,	0, 0},
	{	ButtonFillMap,		IDN_FILLMAP,		TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK,	0, 0},
	{	ButtonRastr,		IDN_RASTR,			TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK,	0, 0},
//	{	ButtonSelect,		IDN_SELECT,			TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK,	0, 0},
};

#define ToolBarEditMapNum (sizeof(ToolBarEditMap)/sizeof(TBBUTTON))

// editor sprajtu
TBBUTTON ToolBarEditSprite[] = {
	{	ButtonSpritePar,	IDN_SPRITEPAR,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	ButtonTest,			IDN_TEST,			TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
};

#define ToolBarEditSpriteNum (sizeof(ToolBarEditSprite)/sizeof(TBBUTTON))

// editor obr�zk�
TBBUTTON ToolBarEditPic[] = {
	{	ButtonDimen,		IDN_DIMEN,			TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	ButtonRastr,		IDN_RASTR,			TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK,	0, 0},
	{	ButtonEditBlok,		IDN_EDITBLOK,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
	{	ButtonOtisk,		IDN_OTISK,			TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0},
};

#define ToolBarEditPicNum (sizeof(ToolBarEditPic)/sizeof(TBBUTTON))

// editor zvuku
TBBUTTON ToolBarEditSound[] = {
	{	ButtonPlay,			IDN_PLAY,			TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK,	0, 0},
	{	ButtonPause,		IDN_PAUSE,			TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK,	0, 0},
	{	ButtonStop,			IDN_STOP,			TBSTATE_ENABLED, TBSTYLE_BUTTON,					0, 0},
	{	ButtonLoop,			IDN_LOOP,			TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK,	0, 0},
	{	ButtonRecord,		IDN_RECORD,			TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK,	0, 0},
};

#define ToolBarEditSoundNum (sizeof(ToolBarEditSound)/sizeof(TBBUTTON))

// editor hudby
TBBUTTON ToolBarEditMusic[] = {
	{	ButtonPlay,			IDN_PLAY,			TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK,	0, 0},
	{	ButtonPause,		IDN_PAUSE,			TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK,	0, 0},
	{	ButtonStop,			IDN_STOP,			TBSTATE_ENABLED, TBSTYLE_BUTTON,					0, 0},
	{	ButtonLoop,			IDN_LOOP,			TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK,	0, 0},
	{	ButtonRew,			IDN_REW,			TBSTATE_ENABLED, TBSTYLE_BUTTON,					0, 0},
	{	ButtonFrw,			IDN_FRW,			TBSTATE_ENABLED, TBSTYLE_BUTTON,					0, 0},
};

#define ToolBarEditMusicNum (sizeof(ToolBarEditMusic)/sizeof(TBBUTTON))

// volba n�stroje pro kreslen�
int ToolBarEditPicTools[] = {
	ButtonPen,			IDN_PEN,
	ButtonLine,			IDN_LINE,
	ButtonBox,			IDN_BOX,
	ButtonFillBox,		IDN_FILLBOX,
	ButtonCircle,		IDN_CIRCLE,
	ButtonFillCircle,	IDN_FILLCIRCLE,
	ButtonEllipse,		IDN_ELLIPSE,
	ButtonFillEllipse,	IDN_FILLELLIPSE,
	ButtonSphere,		IDN_SPHERE,
	ButtonSelect,		IDN_SELECT,
	ButtonSpray,		IDN_SPRAY,
	ButtonPipe,			IDN_PIPE,
	ButtonFill,			IDN_FILL,
	ButtonText,			IDN_TEXT,
	-1
};

// volba ���ky pera pro kreslen�
int ToolBarEditPicWidths[] = {
	ButtonWidth1,		IDN_WIDTH1,
	ButtonWidth2,		IDN_WIDTH2,
	ButtonWidth3,		IDN_WIDTH3,
	ButtonWidth4,		IDN_WIDTH4,
	ButtonWidth5,		IDN_WIDTH5,
	-1
};

// volba m�du pro editor plochy
int ToolBarEditMapTools[] = {
	ButtonMap,			IDN_MAP,
	ButtonSwc1,			IDN_SWC1,
	ButtonSwc2,			IDN_SWC2,
	ButtonSwc3,			IDN_SWC3,
	ButtonSwc4,			IDN_SWC4,
	ButtonSwc5,			IDN_SWC5,
	ButtonSwcN1,		IDN_SWCN1,
	ButtonSwcN2,		IDN_SWCN2,
	ButtonSwcN3,		IDN_SWCN3,
	-1
};

/////////////////////////////////////////////////////////////////////////////
// definice menu

// program
MENUITEM* MenuPrg[] =
{
	MenuSoubor2,
	MenuEditace,
	MenuZobraz,
	MenuProgram,
	MenuHlp,
	NULL
};

// ��slo, p�ep�na�, text
MENUITEM* MenuNum[] =
{
	MenuSoubor2,
	MenuEditace,
	MenuZobraz,
	MenuHlp,
	NULL
};
#define MenuTxt MenuNum
#define MenuLog MenuNum

// ikona
MENUITEM* MenuIco[] =
{
	MenuSoubor2,
	MenuEditace,
	MenuZobraz,
	MenuIkona,
	MenuNastroje,
	MenuTloustka,
	MenuHlp,
	NULL
};

// obr�zek
MENUITEM* MenuPic[] =
{
	MenuSoubor2,
	MenuEditace,
	MenuZobraz,
	MenuObrazek,
	MenuNastroje,
	MenuTloustka,
	MenuHlp,
	NULL
};

// plocha
MENUITEM* MenuMap[] =
{
	MenuSoubor2,
	MenuEditace,
	MenuZobraz,
	MenuPlocha,
	MenuHlp,
	NULL
};

// sprajt
MENUITEM* MenuSpr[] =
{
	MenuSoubor2,
	MenuEditace,
	MenuZobraz,
	MenuSprajt,
	MenuHlp,
	NULL
};

// zvuk
MENUITEM* MenuSnd[] =
{
	MenuSoubor2,
	MenuEditace,
	MenuZobraz,
	MenuSound,
	MenuHlp,
	NULL
};

// hudba
MENUITEM* MenuMus[] =
{
	MenuSoubor2,
	MenuEditace,
	MenuZobraz,
	MenuMusic,
	MenuHlp,
	NULL
};

/////////////////////////////////////////////////////////////////////////////
// nastaven� menu programu a font� (lze i opakovan�)

void ProgSetMenu()
{
	MENUITEM** oldmenu = MenuAkt;

	BufCls.AktFont();
	BufObj.AktFont();
	BufLoc.AktFont();
	BufStr.AktFont();
	BufEdi.AktFont();

	if (FocusBuf != BufEdiID)
	{
		SetNewMenu(MenuPrg);
	}
	else
	{
		switch (EditMode)
		{
		case BufEdiID:
			SetNewMenu(MenuPrg);
			break;

		case BufNumID:
			SetNewMenu(MenuNum);
			break;

		case BufTxtID:
			SetNewMenu(MenuTxt);
			break;

		case BufLogID:
			SetNewMenu(MenuLog);
			break;

		case BufIcoID:
			SetNewMenu(MenuIco);
			break;

		case BufPicID:
			SetNewMenu(MenuPic);
			break;

		case BufMapID:
			SetNewMenu(MenuMap);
			break;

		case BufSprID:
			SetNewMenu(MenuSpr);
			break;

		case BufSndID:
			SetNewMenu(MenuSnd);
			break;

		case BufMusID:
			SetNewMenu(MenuMus);
			break;
		}
	}

	if (oldmenu != MenuAkt)
	{
		ProgUpdateInOut();
		ProgUpdateUndoRedo();
		ProgUpdateClipboard();
		ProgAktItem();
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace po zm�n� jazyku

void ProgReloadBuf(CBufProg* buf)
{
	int disp = buf->Disp();
	int sel = buf->Select();

	buf->Disp(-2);

	buf->Disp(disp);
	buf->Select(sel);
}

void ProgReloadJazyk()
{
// na�ten� nadpis� oken
	CText text;
	text.Load(IDN_NADPISOBJ);
	BufObj.Nadpis(text);
	text.Load(IDN_NADPISLOC);
	BufLoc.Nadpis(text);
	text.Load(IDN_NADPISSTR);
	BufStr.Nadpis(text);
	text.Load(IDN_NADPISCLS);
	BufCls.Nadpis(text);

// aktualizace oken
	if (ProgMode)
	{
		ProgReloadBuf(&BufStr);
		ProgReloadBuf(&BufCls);
		ProgReloadBuf(&BufObj);
		ProgReloadBuf(&BufLoc);
		ProgReloadBuf(&BufEdi);

		if (EditMode == BufTxtID)
		{
			EditText::BegEdit(EditText::Index);
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// prvn� inicializace p�i startu programu (hlavn� okno i SELECT je ji� vytvo�eno)

void ProgStartInit()
{
// inicializace buffer� programu
	int i;
	for (i = 0; i < PROGBUFNUM; i++)
	{
		BufProg[i].BufID(i);
	}

// na�ten� nadpis� oken
	CText text;
	text.Load(IDN_NADPISOBJ);
	BufObj.Nadpis(text);
	text.Load(IDN_NADPISLOC);
	BufLoc.Nadpis(text);
	text.Load(IDN_NADPISSTR);
	BufStr.Nadpis(text);
	text.Load(IDN_NADPISCLS);
	BufCls.Nadpis(text);

// inicializace statick�ch objekt�
	Sprite0.New(5, 4);				// sprajt 0 - Petr
	Sprite1.New(5, 4);				// sprajt 1 - Petra
	Sprite1.Level(900);

// kontrola tabulky jmen importu (zda jsou jm�na jedine�n�)
#ifdef	_DEBUG
	{
		int i, j, n;
		char* txt;

		for (i = 0; i < FncNum-1; i++)
		{
			txt = InitTabFunc[i].Import;
			n = ::lstrlenA(txt) + 1;

			for (j = i+1; j < FncNum; j++)
			{
				ASSERT(!MemCompare(txt, InitTabFunc[j].Import, n));
			}
		}
	}
#endif

// na�ten� text� funkc�
	int index;
	for (i = IDF; i < IDF + FncNum; i++)		// cyklus p�es v�echny ID
	{
		index = TextFnc.Load(i);				// na�ten� textu s resource
		ASSERT(index == (i - IDF));				// ukl�d�n� mus� b�t spojit�

#ifdef _DEBUG
		for (int j = 0; j < JAZYKNUM; j++)
		{
			ASSERT(TextFnc[index].MultiText(j).IsNotEmpty());	// mus� to b�t platn� text
		}
#endif
	}

#ifdef _DEBUG
	ASSERT(TextFnc.Num() == FncNum);			// po�et text� te� mus� p�esn� souhlasit
#endif

// na�ten� ikon funkc�
	IconFnc.Load(IDN_DEF_ICON, FncNum);			// na�ten� ikon z resource
	ASSERT(IconFnc.Num() == FncNum);			// mus� p�esn� souhlasit po�et

// vygenerov�n� ikon pro HELP (odpozn�mkovat a spustit v�dy po p�id�n� dal��ch funkc�, p�ed dopl�ov�n�m HELPu)
/*
	StdBitmapInfo->bmiColors[BackCol].rgbRed = 255;
	StdBitmapInfo->bmiColors[BackCol].rgbGreen = 255;
	StdBitmapInfo->bmiColors[BackCol].rgbBlue = 230;
	CPicture ppp;
	for (i = IDF; i < IDF+FncNum; i++)
	{
		ppp.New(32, 32);
		IconFnc[i-IDF].DeComp();
		ppp.CopyData(IconFnc[i-IDF].DataData());
		CText n;
		n.Int(i);
		ppp.SaveFile("HLP\\IDF\\IDF" + n + ".BMP");
	}
*/

// na�ten� stavov�ch ikon
	IconState.IconSize(12, 32);					// nastaveni velikosti ikon
	IconState.Load(IDN_STATE_ICON, 4);			// na�ten� stavov�ch ikon
	ASSERT(IconState.Num() == 4);

// inicializace bufferu seznamu ikon
//	Icon16.DelAll();
//	Icon32.DelAll();
//	Icon16List.NumClear(ICONNUM);
//	Icon32List.NumClear(ICONNUM);
//	for (i = 0; i < ICONNUM; i++)
//	{
//		Icon16.New();
//		Icon32.New();
//	}

// inicializace bufferu intern�ch funkc�
	PROGITEM* item;
	for (i = IDF; i < IDF + FncNum; i++)
	{
		index = BufInt.New();						// vytvo�en� nov� polo�ky
		ASSERT(index == (i - IDF));					// ukl�d�n� mus� b�t spojit�
		item = &BufInt[index];						// adresa nov� polo�ky
		item->Func = i;								// identifika�n� ��slo funkce
		item->SrcMask = InitTabFunc[index].SrcMask;	// zdrojov� vlastnosti
		item->DstMask = InitTabFunc[index].DstMask;	// c�lov� vlastnosti
		item->Param = InitTabFunc[index].Param;		// parametry
		item->Icon = index;							// index ikony
		item->Name = index;							// index jm�na
	}
	ASSERT(BufInt.Num() == FncNum);					// po�et funkc� mus� souhlasit

// inicializace bufferu struktur
	BufStr.InitTab(InitTabStr, sizeof(InitTabStr)/sizeof(InitTabStr[0]));
	ASSERT(BufStr.Num() == sizeof(InitTabStr)/sizeof(InitTabStr[0]));

// nalezen� indexu skupiny
	for (int inx = BufStr.Max()-1; inx >= 0; inx--)
	{
		if ((BufStr.IsValid(inx)) &&
			(BufStr[inx].Func == IDF_GROUP))
		{
			GroupIndex = inx;
			break;
		}
	};

// inicializace editor�
	EditIcon::StartInit();
	EditNum::StartInit();
	EditLog::StartInit();
	EditText::StartInit();
	EditMap::StartInit();
	EditSound::StartInit();
	EditMusic::StartInit();
	EditSprite::StartInit();
	ProgClip::StartInit();
	ProgLib::StartInit();
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� okna stromu

HWND CreateTree(int x, int y, int width, int height, int id)
{
	HWND wnd = ::CreateWindowEx(
		WS_EX_CLIENTEDGE,
		WC_TREEVIEW,
		NULL,
		WS_CHILD | WS_TABSTOP | TVS_HASLINES | TVS_HASBUTTONS 
			| TVS_LINESATROOT | TVS_EDITLABELS | TVS_SHOWSELALWAYS,
		x,
		y,
		width,
		height,
		MainFrame,
		(HMENU)id,
		hInstance,
		NULL);

	ASSERT(wnd != NULL);
	return wnd;
}


/////////////////////////////////////////////////////////////////////////////
// zah�jen� re�imu editace souboru

void ProgInit()
{
// kontrola, zda nen� re�im editace souboru
	if (ProgMode) return;

	Modi = FALSE;
	EditName = FALSE;
	EditNameWnd = NULL;
	EditMapNumWnd = NULL;

	EditMode = BufEdiID;						// editace zat�m nen� platn�
	EditBuf = BufEdiID;
	EditItem = 0;

// nastaven� tla��tek pro re�im programov�n�
	ToolBarProgN = ToolBarProgramNum;
	ToolBarAdd(ToolBarProgram, ToolBarProgramNum); // p�id�n� tla��tek

// nastaven� nov�ho menu
	ProgSetMenu();
	EnableCommand(IDN_START, TRUE);

// inicializace buffer�
	Icon = IconFnc;
	Text = TextFnc;

//	Icon16List.NumClear(ICONNUM);
//	Icon32List.NumClear(ICONNUM);

// buffer intern�ch funkc� mus� z�stat st�le stejn� a nezm�n�n
	ASSERT(BufInt.Num() == FncNum);
	ASSERT(Icon.Num() == FncNum);
	ASSERT(Text.Num() == FncNum);
	ASSERT(IconState.Num() == 4);

// buffer struktur mus� tak� z�stat nezm�n�n
	ASSERT(BufStr.Num() == sizeof(InitTabStr)/sizeof(InitTabStr[0]));

// vynulov�n� ��ta�� referenc� v bufferu struktur
	int inx;
	for (inx = BufStr.Max()-1; inx >= 0; inx--)
	{
		if (BufStr.IsValid(inx))
		{
			BufStr[inx].Refer = 0;
		}
	}

// inicializace bufferu t��d
	BufCls.InitTab(InitTabCls, sizeof(InitTabCls)/sizeof(InitTabCls[0]));

// na�ten� programu
	ProgFile::Load();

// kontrola programu
	ProgramCheck();

// aktualizace LOCK a OFF prvk� v buferech
	UpdateLock();

// vytvo�en� seznamu obr�zk�
	ProgImageList = ImageList_Create(32, 32, ILC_COLORDDB | ILC_MASK,1,1);
	ASSERT(ProgImageList);
	ProgImageListMedium = ImageList_Create(24, 24, ILC_COLORDDB | ILC_MASK,1,1);
	ASSERT(ProgImageListMedium);
	ProgImageListSmall = ImageList_Create(16, 16, ILC_COLORDDB | ILC_MASK,1,1);
	ASSERT(ProgImageListSmall);
	ProgStateList = IconState.GenerList(FALSE, SI_BIG);
	ASSERT(ProgStateList);
	ProgStateListMedium = IconState.GenerList(FALSE, SI_MEDIUM);
	ASSERT(ProgStateListMedium);
	ProgStateListSmall = IconState.GenerList(FALSE, SI_SMALL);
	ASSERT(ProgStateListSmall);

// vytvo�en� oken strom�
	TrackX1 = ClientRect.left + ClientWidth/5;
	TrackX2 = ClientRect.left + ClientWidth/5*4;
	TrackY1 = ClientRect.top + ClientHeight/3*2;
	TrackY2 = ClientRect.top + ClientHeight/3*2;

	EditX = TrackX1;
	EditY = ClientRect.top + NADPISHEIGHT;
	EditWidth = TrackX2 - TrackX1;
	EditHeight = ClientHeight;

	TreeObj = CreateTree(ClientRect.left, ClientRect.top + NADPISHEIGHT, TrackX1 - ClientRect.left, TrackY1 - ClientRect.top - NADPISHEIGHT, IDN_TREE_OBJ);
	TreeLoc = CreateTree(ClientRect.left, TrackY1 + NADPISHEIGHT, TrackX1 - ClientRect.left, ClientRect.bottom - TrackY1 - NADPISHEIGHT, IDN_TREE_LOC);
	TreeEdi = CreateTree(EditX, EditY, EditWidth, EditHeight - NADPISHEIGHT, IDN_TREE_EDI);
	TreeStr = CreateTree(TrackX2, ClientRect.top + NADPISHEIGHT, ClientRect.right - TrackX2, TrackY2 - ClientRect.top - NADPISHEIGHT, IDN_TREE_STR);
	TreeCls = CreateTree(TrackX2, TrackY2 + NADPISHEIGHT, ClientRect.right - TrackX2, ClientRect.bottom - TrackY2 - NADPISHEIGHT, IDN_TREE_CLS);

// horizont�ln� posuvn�k
	HScroll = ::CreateWindow(_T("SCROLLBAR"), NULL, WS_CHILD | SBS_HORZ,
		0, 0, TRACKSIRKA, TRACKSIRKA, MainFrame, NULL, hInstance, 0);

// vertik�ln� posuvn�k
	VScroll = ::CreateWindow(_T("SCROLLBAR"), NULL, WS_CHILD | SBS_VERT,
		0, 0, TRACKSIRKA, TRACKSIRKA, MainFrame, NULL, hInstance, 0);

// napojen� strom� na buffery
	BufObj.Tree(TreeObj);
	BufLoc.Tree(TreeLoc);
	BufEdi.Tree(TreeEdi);
	BufCls.Tree(TreeCls);
	BufStr.Tree(TreeStr);

// nastaven� ikon pro stromy
	BufObj.IconList(BufZoom[BufObjID]);
	BufLoc.IconList(BufZoom[BufLocID]);
	BufEdi.IconList(BufZoom[BufEdiID]);
	BufStr.IconList(BufZoom[BufStrID]);
	BufCls.IconList(BufZoom[BufClsID]);

// nastaven� font� pro stromy
	BufCls.AktFont();
	BufObj.AktFont();
	BufLoc.AktFont();
	BufStr.AktFont();
	BufEdi.AktFont();

// zobrazen� strom�
	BufObj.Disp(-1);
	BufLoc.Disp(0);
	BufEdi.Disp(0);
	BufStr.Disp(-1);
	BufCls.Disp(-1);

// zapnut� m�du programov�n�
	ProgMode = TRUE;

// zobrazen� oken strom�
	ProgOnSize();
	::ShowWindow(TreeObj, SW_SHOW);
	::ShowWindow(TreeLoc, SW_SHOW);
	::ShowWindow(TreeEdi, SW_SHOW);
	::ShowWindow(TreeStr, SW_SHOW);
	::ShowWindow(TreeCls, SW_SHOW);

// vypr�zdn�n� bufferu historie editace
	HistEdit::DelAll();
	HistEdit::Update();

// nastaven� fokusu na okno objekt�
	EditBuf = -1;
	SetEditMode(BufEdiID, 0);						// zapnut� edita�n�ho pole programu

// nastaven� fokusu na okno objekt� s opo�d�n�m (chyba v obsluze stromu v COMMCTL32.DLL)
// !!!!!!!! ProgSetFocus() havaruje (adresa 56AE90 se zneplatn� - polo�ka stromu struktur) !!!!!!!!
	FocusBuf = BufObjID;
	FocusNextBuf = BufObjID;
	FocusEditModeBuf = -1;
	FocusTimerN = 2;
	FocusTimer = ::SetTimer(MainFrame, FocusTimerID, TimerConst, NULL);
//	ProgSetFocus();

// nadpis edita�n�ho okna
	BufEdi.Nadpis(Text.GetTxt(BufObj.GetText(BufObj.SrcDat(BufLocID, 0))));

// zah�jen� hl�en� zm�n adres���
	BeginDirChange(&FunctionDirChange, &FunctionDirChange2, FunctionPath, FunctionPath2);
	BeginDirChange(&NumberDirChange, &NumberDirChange2, NumberPath, NumberPath2);
	BeginDirChange(&TextDirChange, &TextDirChange2, TextPath, TextPath2);
	BeginDirChange(&BoolDirChange, &BoolDirChange2, BoolPath, BoolPath2);
	BeginDirChange(&IconDirChange, &IconDirChange2, IconPath, IconPath2);
	BeginDirChange(&MapDirChange, &MapDirChange2, MapPath, MapPath2);
	BeginDirChange(&PictureDirChange, &PictureDirChange2, PicturePath, PicturePath2);
	BeginDirChange(&SpriteDirChange, &SpriteDirChange2, SpritePath, SpritePath2);
	BeginDirChange(&SoundDirChange, &SoundDirChange2, SoundPath, SoundPath2);
	BeginDirChange(&MusicDirChange, &MusicDirChange2, MusicPath, MusicPath2);

	ProgDirChangeTimer = ::SetTimer(MainFrame, ProgDirChangeTimerID, 2000, NULL);
	ProgDirChangeIgnore = 1;

// zah�jen� registrace UNDO
	BufCls.UndoOn();
	BufObj.UndoOn();
	BufLoc.UndoOn();
	BufEdi.UndoOn();
	BufStr.UndoOn();

	Real.UndoOn();
	Text.UndoOn();
	Bool.UndoOn();
	Icon.UndoOn();
	Map.UndoOn();
	Picture.UndoOn();
	Sprite.UndoOn();
	Sound.UndoOn();
	Music.UndoOn();

// zah�jen� ��t�n� preview
	PreviewStop();

// aktualizace p�ep�na�� pro vybranou polo�ku
	ProgAktItem();
}


/////////////////////////////////////////////////////////////////////////////
// zah�jen� sledov�n� zm�n adres��e

void BeginDirChange(HANDLE* dir1, HANDLE* dir2, CText& path1, CText& path2)
{
	if (*dir1 == INVALID_HANDLE_VALUE)
	{
		*dir1 = ::FindFirstChangeNotification(
			path1,							// cesta k adres��i
			TRUE,							// hl�sit podadred��e
			FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_ATTRIBUTES);
	}

	if ((*dir2 == INVALID_HANDLE_VALUE) && path2.IsNotEmpty())
	{
		*dir2 = ::FindFirstChangeNotification(
			path2,							// cesta k adres��i
			TRUE,							// hl�sit podadred��e
			FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_ATTRIBUTES);
	}
}


/////////////////////////////////////////////////////////////////////////////
// ukon�en� re�imu editace souboru

void ProgTerm()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;
	ProgMode = FALSE;

// ukon�en� preview
	PreviewStop();

	if (PreviewTimer)
	{
		::KillTimer(MainFrame, PreviewTimer);
		PreviewTimer = 0;
	}
	PreviewTimerN = -1;
	PreviewPicOff();

// ukon�en� registrace UNDO
	BufCls.UndoOff();
	BufObj.UndoOff();
	BufLoc.UndoOff();
	BufEdi.UndoOff();
	BufStr.UndoOff();

	Real.UndoOff();
	Text.UndoOff();
	Bool.UndoOff();
	Icon.UndoOff();
	Map.UndoOff();
	Picture.UndoOff();
	Sprite.UndoOff();
	Sound.UndoOff();
	Music.UndoOff();

// zru�en� hl�en� o zm�n� adres��e
	if (ProgDirChangeTimer)
	{
		::KillTimer(MainFrame, ProgDirChangeTimer);
		ProgDirChangeTimer = 0;
	}

	if (FunctionDirChange != INVALID_HANDLE_VALUE)
	{
		::FindCloseChangeNotification(FunctionDirChange);
		FunctionDirChange = INVALID_HANDLE_VALUE;
	}

	if (NumberDirChange != INVALID_HANDLE_VALUE)
	{
		::FindCloseChangeNotification(NumberDirChange);
		NumberDirChange = INVALID_HANDLE_VALUE;
	}

	if (TextDirChange != INVALID_HANDLE_VALUE)
	{
		::FindCloseChangeNotification(TextDirChange);
		TextDirChange = INVALID_HANDLE_VALUE;
	}

	if (BoolDirChange != INVALID_HANDLE_VALUE)
	{
		::FindCloseChangeNotification(BoolDirChange);
		BoolDirChange = INVALID_HANDLE_VALUE;
	}

	if (IconDirChange != INVALID_HANDLE_VALUE)
	{
		::FindCloseChangeNotification(IconDirChange);
		IconDirChange = INVALID_HANDLE_VALUE;
	}

	if (MapDirChange != INVALID_HANDLE_VALUE)
	{
		::FindCloseChangeNotification(MapDirChange);
		MapDirChange = INVALID_HANDLE_VALUE;
	}

	if (PictureDirChange != INVALID_HANDLE_VALUE)
	{
		::FindCloseChangeNotification(PictureDirChange);
		PictureDirChange = INVALID_HANDLE_VALUE;
	}

	if (SpriteDirChange != INVALID_HANDLE_VALUE)
	{
		::FindCloseChangeNotification(SpriteDirChange);
		SpriteDirChange = INVALID_HANDLE_VALUE;
	}

	if (SoundDirChange != INVALID_HANDLE_VALUE)
	{
		::FindCloseChangeNotification(SoundDirChange);
		SoundDirChange = INVALID_HANDLE_VALUE;
	}

	if (MusicDirChange != INVALID_HANDLE_VALUE)
	{
		::FindCloseChangeNotification(MusicDirChange);
		MusicDirChange = INVALID_HANDLE_VALUE;
	}



	if (FunctionDirChange2 != INVALID_HANDLE_VALUE)
	{
		::FindCloseChangeNotification(FunctionDirChange2);
		FunctionDirChange2 = INVALID_HANDLE_VALUE;
	}

	if (NumberDirChange2 != INVALID_HANDLE_VALUE)
	{
		::FindCloseChangeNotification(NumberDirChange2);
		NumberDirChange2 = INVALID_HANDLE_VALUE;
	}

	if (TextDirChange2 != INVALID_HANDLE_VALUE)
	{
		::FindCloseChangeNotification(TextDirChange2);
		TextDirChange2 = INVALID_HANDLE_VALUE;
	}

	if (BoolDirChange2 != INVALID_HANDLE_VALUE)
	{
		::FindCloseChangeNotification(BoolDirChange2);
		BoolDirChange2 = INVALID_HANDLE_VALUE;
	}

	if (IconDirChange2 != INVALID_HANDLE_VALUE)
	{
		::FindCloseChangeNotification(IconDirChange2);
		IconDirChange2 = INVALID_HANDLE_VALUE;
	}

	if (MapDirChange2 != INVALID_HANDLE_VALUE)
	{
		::FindCloseChangeNotification(MapDirChange2);
		MapDirChange2 = INVALID_HANDLE_VALUE;
	}

	if (PictureDirChange2 != INVALID_HANDLE_VALUE)
	{
		::FindCloseChangeNotification(PictureDirChange2);
		PictureDirChange2 = INVALID_HANDLE_VALUE;
	}

	if (SpriteDirChange2 != INVALID_HANDLE_VALUE)
	{
		::FindCloseChangeNotification(SpriteDirChange2);
		SpriteDirChange2 = INVALID_HANDLE_VALUE;
	}

	if (SoundDirChange2 != INVALID_HANDLE_VALUE)
	{
		::FindCloseChangeNotification(SoundDirChange2);
		SoundDirChange2 = INVALID_HANDLE_VALUE;
	}

	if (MusicDirChange2 != INVALID_HANDLE_VALUE)
	{
		::FindCloseChangeNotification(MusicDirChange2);
		MusicDirChange2 = INVALID_HANDLE_VALUE;
	}


// vypnut� automatick�ho focusu
	FocusTimerN = 0;
	FocusEditModeBuf = -1;
	if (FocusTimer)
	{
		::KillTimer(MainFrame, FocusTimer);
		FocusTimer = 0;
	}

// vypnut� m�du editace
	SetToolMode(-1);

// skryt� okna struktur
//	::ShowWindow(TreeStr, SW_HIDE);

// zru�en� okna glob�ln�ch objekt�
	BufObj.Disp(-2);
	BufObj.Tree(NULL);
	::DestroyWindow(TreeObj);
	TreeObj = NULL;

// zru�en� okna lok�ln�ch objekt�
	BufLoc.Disp(-2);
	BufLoc.Tree(NULL);
	::DestroyWindow(TreeLoc);
	TreeLoc = NULL;

// zru�en� okna editoru
	BufEdi.Disp(-2);
	BufEdi.Tree(NULL);
	::DestroyWindow(TreeEdi);
	TreeEdi = NULL;

// zru�en� okna struktur
	BufStr.Disp(-2);
	BufStr.Tree(NULL);
	::DestroyWindow(TreeStr);
	TreeStr = NULL;
	ASSERT(BufStr.Num() == sizeof(InitTabStr)/sizeof(InitTabStr[0]));

// zru�en� okna t��d
	BufCls.Disp(-2);
	BufCls.Tree(NULL);
	::DestroyWindow(TreeCls);
	TreeCls = NULL;

// zru�en� posuvn�ch li�t
	::DestroyWindow(HScroll);
	HScroll = NULL;
	::DestroyWindow(VScroll);
	VScroll = NULL;

	if (ToolEditIcon)
	{
		::DestroyWindow(ToolEditIcon);
		ToolEditIcon = NULL;
	}

	if (ToolEditIconWidth)
	{
		::DestroyWindow(ToolEditIconWidth);
		ToolEditIconWidth = NULL;
	}

	if (ToolEditMap)
	{
		::DestroyWindow(ToolEditMap);
		ToolEditMap = NULL;
	}

// skryt� okna editoru ��sla
	::ShowWindow(EditNumWnd, SW_HIDE);

// skryt� okna editoru logick�ch hodnot
	::ShowWindow(EditLogWnd, SW_HIDE);

// skryt� okna editoru textu
	::ShowWindow(EditTextWnd, SW_HIDE);

// skryt� okna editoru zvuku
	EditSound::OnStop();
	::ShowWindow(EditSound::DispWnd, SW_HIDE);

// skryt� okna editoru hudby
	EditMusic::OnStop();
	::ShowWindow(EditMusic::DispWnd, SW_HIDE);

// zru�en� okna editoru plochy
	EditMap::EndEditSwcNum();

// zru�en� seznamu obr�zk�
	::ImageList_Destroy(ProgImageList);
	ProgImageList = NULL;
	::ImageList_Destroy(ProgImageListMedium);
	ProgImageListMedium = NULL;
	::ImageList_Destroy(ProgImageListSmall);
	ProgImageListSmall = NULL;
	::ImageList_Destroy(ProgStateList);
	ProgStateList = NULL;
	::ImageList_Destroy(ProgStateListMedium);
	ProgStateListMedium = NULL;
	::ImageList_Destroy(ProgStateListSmall);
	ProgStateListSmall = NULL;

// vypr�zdn�n� bufferu historie editace (p�es zru�en�m buffer�!)
	HistEdit::DelAll();
	HistEdit::Update();

// vypr�zdn�n� buffer� programu (jen pracovn�ch)
	ASSERT(BufInt.Num() == FncNum);
	BufObj.DelAll();
	BufLoc.DelAll();
	BufEdi.DelAll();
	BufCls.DelAll();

// vypr�zdn�n� pracovn�ch buffer�
	Real.DelAll();								// buffer re�ln�ch ��sel
	Text.DelAll();								// buffer text�
	Bool.DelAll();								// buffer logick�ch prom�nn�ch
	Icon.DelAll();								// buffer ikon
	Map.DelAll();								// buffer ploch
	Sprite.DelAll();							// buffer sprajt�
	Sound.DelAll();								// buffer zvuk�
	Picture.DelAll();							// buffer obr�zk�
	Music.DelAll();								// buffer hudby

	Undo.DelAll();								// buffer undo a redo

// zru�en� v�ech tla��tek v panelu n�stroj�
	ToolBarClear(0);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� fokusu na aktivn� okno

void ProgSetFocus()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// na�ten� okna s fokusem
	HWND wndOld = ::GetFocus();

// vypnut� automatick�ho focusu
	FocusTimerN = 0;
	FocusEditModeBuf = -1;
	if (FocusTimer)
	{
		::KillTimer(MainFrame, FocusTimer);
		FocusTimer = 0;
	}

// p��prava nov�ho okna
	HWND wndNew = MainFrame;

	if ((FocusBuf != BufEdiID) || (EditMode == BufEdiID))
	{
		wndNew = BufProg[FocusBuf].Tree();
	}

	if ((FocusBuf == BufEdiID) && (EditMode == BufNumID))
	{
		wndNew = EditNumWnd;
	}

	if ((FocusBuf == BufEdiID) && (EditMode == BufLogID))
	{
		wndNew = EditLogWnd;
	}

	if ((FocusBuf == BufEdiID) && (EditMode == BufTxtID))
	{
		wndNew = EditTextWnd;
	}

	if ((FocusBuf == BufEdiID) && (EditMode == BufMapID) && EditMapNumWnd)
	{
		wndNew = EditMapNumWnd;
	}

// nastaven� fokusu na nov� okno
	if (wndNew != wndOld)
	{
//		NoSelAkt = TRUE;
		::SetFocus(wndNew);
		ProgOnSetFocus(wndNew);
//		NoSelAkt = FALSE;

// zah�jen� ��t�n� preview
		PreviewStop();
	}
}


/////////////////////////////////////////////////////////////////////////////
// update voleb bloku

void ProgUpdateClipboard()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// aktualizace voleb p�i editaci jm�na
	if (EditName)
	{
		long int start, end;
		::SendMessage(EditNameWnd, EM_GETSEL, (WPARAM)&start, (WPARAM)&end);
		BOOL enable = (end != start);
		int len = ::GetWindowTextLength(EditNameWnd);

		EnableCommand(IDN_CUT, enable);
		EnableCommand(IDN_COPY, enable);
		EnableCommand(IDN_PASTE, ::IsClipboardFormatAvailable(CF_TEXT));
		EnableCommand(IDN_CLEAR, enable || (start < len));
		EnableCommand(IDN_ALL, len > 0);
		return;
	}

// nastaven� panelu n�stroj�
	if (FocusBuf == BufEdiID)
	{
		switch(EditMode)
		{
		case BufEdiID:
			ProgClip::UpdateClipboard();
			break;

		case BufNumID:
			EditNum::UpdateClipboard();
			break;

		case BufTxtID:
			EditText::UpdateClipboard();
			break;

		case BufLogID:
			SetStatusText2(EmptyText);
			EnableCommand(IDN_CUT, FALSE);
			EnableCommand(IDN_COPY, FALSE);
			EnableCommand(IDN_PASTE, FALSE);
			EnableCommand(IDN_CLEAR, FALSE);
			EnableCommand(IDN_ALL, FALSE);
			break;

		case BufIcoID:
		case BufPicID:
			EditIcon::UpdateClipboard();
			break;

		case BufMapID:
			EditMap::UpdateClipboard();
			break;

		case BufSprID:
			EnableCommand(IDN_CUT, FALSE);
			EnableCommand(IDN_COPY, FALSE);
			EnableCommand(IDN_PASTE, FALSE);
			EnableCommand(IDN_CLEAR, FALSE);
			EnableCommand(IDN_ALL, FALSE);
			break;

		case BufSndID:
			EditSound::UpdateClipboard();
			break;

		case BufMusID:
			EditMusic::UpdateClipboard();
			break;

		default:
			ASSERTERROR;
		}
	}
	else
	{
		ProgClip::UpdateClipboard();
	}
}


/////////////////////////////////////////////////////////////////////////////
// update UNDO a REDO

void ProgUpdateUndoRedo()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// aktualizace voleb p�i editaci jm�na
	if (EditName)
	{
		BOOL enable = ::SendMessage(EditNameWnd, EM_CANUNDO, 0, 0);
		EnableCommand(IDN_UNDO, enable);
		EnableCommand(IDN_REDO, enable);
		return;
	}

// aktualizace voleb p�i editaci ��sla plochy
	if (EditMapNumWnd)
	{
		BOOL enable = ::SendMessage(EditMapNumWnd, EM_CANUNDO, 0, 0);
		EnableCommand(IDN_UNDO, enable);
		EnableCommand(IDN_REDO, enable);
		return;
	}

// nastaven� panelu n�stroj�
	Undo.UpdateUndoRedo();
}


/////////////////////////////////////////////////////////////////////////////
// update menu

void ProgUpdateMenu()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// nastaven� menu
	ProgSetMenu();

// nastaven� panelu n�stroj�
	if (FocusBuf == BufEdiID)
	{
		switch(EditMode)
		{
		case BufIcoID:
		case BufPicID:
			EditIcon::UpdateMenu();
			break;

		case BufSprID:
			EditSound::UpdateMenu();
			break;

		case BufSndID:
			EditSound::UpdateMenu();
			break;

		case BufMusID:
			EditMusic::UpdateMenu();
			break;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// update voleb pro ZOOM

void ProgUpdateInOut()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// aktualizace voleb p�i editaci jm�na
	if (EditName)
	{
		EnableCommand(IDN_IN, FALSE);
		EnableCommand(IDN_OUT, FALSE);
		return;
	}

// nastaven� panelu n�stroj�
	if (FocusBuf == BufEdiID)
	{
		switch(EditMode)
		{
		case BufEdiID:
			EnableCommand(IDN_IN, BufZoom[BufEdiID] != SI_BIG);
			EnableCommand(IDN_OUT, BufZoom[BufEdiID] != SI_SMALL);
			break;

		case BufIcoID:
		case BufPicID:
			EditIcon::UpdateZoomInOut();
			break;

		default:				
			EnableCommand(IDN_IN, FALSE);
			EnableCommand(IDN_OUT, FALSE);
		}
	}
	else
	{
		EnableCommand(IDN_IN, BufZoom[FocusBuf] != SI_BIG);
		EnableCommand(IDN_OUT, BufZoom[FocusBuf] != SI_SMALL);
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� fokusu na okno

void ProgOnSetFocus(HWND hWnd)
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// v�b�r textu v editoru ��sla
	if (hWnd == EditNumWnd)
	{
		::SendMessage(EditNumWnd, EM_SETSEL, 0, (LPARAM)-1);
	}

// ukon�en� editace ��seln�ho parametru plochy
	if (hWnd != EditMapNumWnd)
	{
		EditMap::EndEditSwcNum();
	}

// rozli�en� zdrojov�ho okna
	int bufID = BufProgIDFromHandle(hWnd);
	if (bufID < 0) return;
	FocusBuf = bufID;

// p�ekreslen� star�ho i nov�ho okna
	TimeRepaint(FocusBuf);
	TimeRepaint(OldFocusBuf);
	OldFocusBuf = FocusBuf;

// aktivace okna stromu editoru
//	if ((hWnd == TreeEdi) && (EditMode != BufEdiID))
//	{
//		SetEditMode(BufEdiID, 0);
//	}

// nastaven� panelu n�stroj�
	if (bufID == BufEdiID)
	{
		SetToolMode(EditMode);

		switch(EditMode)
		{
		case BufSndID:
		case BufMusID:
		case BufLogID:
			SetStatusText2(EmptyText);
			break;
		}
	}
	else
	{
		SetToolMode(bufID);
	}

	ProgUpdateClipboard();
	ProgUpdateUndoRedo();
	ProgUpdateInOut();
	ProgUpdateMenu();

// aktualizace p�ep�na�� pro vybranou polo�ku
	ProgDispNadpis();
	ProgAktItem();

// zah�jen� ��t�n� preview
	PreviewStop();
}


/////////////////////////////////////////////////////////////////////////////
// zm�na velikosti okna

void ProgOnSize()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// posun panelu n�stroj�
	ToolBarResize();

// zah�jen� p�esouv�n� oken
	HDWP hdwp = ::BeginDeferWindowPos(30);

// na�ten� v��ky stavov� li�ty (i kdy� to nen� pot�eba)
	RECT rcStat;
	::GetWindowRect(StatusBar, &rcStat);

// posun stavov�ho okna
	hdwp = ::DeferWindowPos(hdwp, StatusBar, NULL, ClientRect.left, ClientRect.bottom, ClientWidth, rcStat.bottom - rcStat.top,
			SWP_DRAWFRAME | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER);

// inicializace klientsk� oblasti
	InitClientRect();

// aktualizace p�ed�l� stavov�ho okna
	SetStatusWidth(ProgStatusWidth);

// p��prava p�ed�l�
	TrackX1 = ClientRect.left + (ClientWidth*LeftWidth + Scale/2)/Scale;
	if (TrackX1 > (ClientRect.right - 8)) TrackX1 = ClientRect.right - 8;
	if (TrackX1 < ClientRect.left + 4) TrackX1 = ClientRect.left + 4;

	TrackX2 = ClientRect.right - (ClientWidth*RightWidth + Scale/2)/Scale;
	if (TrackX2 > (ClientRect.right - 4)) TrackX2 = ClientRect.right - 4;
	if (TrackX2 < ClientRect.left + 8) TrackX2 = ClientRect.left + 8;

	TrackY1 = ClientRect.top + (ClientHeight*LeftHeight + Scale/2)/Scale;
	if (TrackY1 > (ClientRect.bottom - NADPISHEIGHT - 4)) TrackY1 = ClientRect.bottom - NADPISHEIGHT - 4;
	if (TrackY1 < ClientRect.top + NADPISHEIGHT + 4) TrackY1 = ClientRect.top + NADPISHEIGHT + 4;

	TrackY2 = ClientRect.top + (ClientHeight*RightHeight + Scale/2)/Scale;
	if (TrackY2 > (ClientRect.bottom - NADPISHEIGHT - 4)) TrackY2 = ClientRect.bottom - NADPISHEIGHT - 4;
	if (TrackY2 < ClientRect.top + NADPISHEIGHT + 4) TrackY2 = ClientRect.top + NADPISHEIGHT + 4;

// rozm�ry edita�n�ho pole
	EditX = TrackX1;
	EditY = ClientRect.top + NADPISHEIGHT;
	EditWidth = TrackX2 - TrackX1;
	EditHeight = ClientHeight - NADPISHEIGHT;

// zm�na velikosti okna glob�ln�ch objekt�
	hdwp = ::DeferWindowPos(hdwp, TreeObj, NULL, ClientRect.left, ClientRect.top + NADPISHEIGHT, TrackX1 - ClientRect.left, TrackY1 - ClientRect.top - NADPISHEIGHT,
			SWP_DRAWFRAME | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER);

// zm�na velikosti okna lok�ln�ch objekt�
	hdwp = ::DeferWindowPos(hdwp, TreeLoc, NULL, ClientRect.left, TrackY1 + NADPISHEIGHT, TrackX1 - ClientRect.left, ClientRect.bottom - TrackY1 - NADPISHEIGHT,
			SWP_DRAWFRAME | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER);

// zm�na velikosti okna editoru (i kdy� nen� zobrazeno)
	hdwp = ::DeferWindowPos(hdwp, TreeEdi, NULL, EditX, EditY, EditWidth, EditHeight,
			SWP_DRAWFRAME | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER);

// zm�na velikosti okna struktur
	hdwp = ::DeferWindowPos(hdwp, TreeStr, NULL, TrackX2, ClientRect.top + NADPISHEIGHT, ClientRect.right - TrackX2, TrackY2 - ClientRect.top - NADPISHEIGHT,
			SWP_DRAWFRAME | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER);

// zm�na velikosti okna t��d
	hdwp = ::DeferWindowPos(hdwp, TreeCls, NULL, TrackX2, TrackY2 + NADPISHEIGHT, ClientRect.right - TrackX2, ClientRect.bottom - TrackY2 - NADPISHEIGHT,
			SWP_DRAWFRAME | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER);

// nastaven� posuvn�k�
	switch (EditMode)
	{
	case BufMapID:
		hdwp = EditMap::MoveScroll(hdwp);
		break;

	case BufSprID:
		hdwp = EditSprite::MoveScroll(hdwp);
		break;

	case BufIcoID:
	case BufPicID:
		hdwp = EditIcon::MoveScroll(hdwp);
		break;
	
	default:;
		hdwp = ::DeferWindowPos(hdwp, HScroll, NULL, 0, 0, TRACKSIRKA, TRACKSIRKA,
			SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER);
		hdwp = ::DeferWindowPos(hdwp, VScroll, NULL, 0, 0, TRACKSIRKA, TRACKSIRKA,
			SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER);
	}

// posun edita�n�ho pole editoru ��sel
	hdwp = EditNum::MoveEdit(hdwp);	

// posun edita�n�ho pole editoru logick�ch hodnot
	hdwp = EditLog::MoveEdit(hdwp);

// posun edita�n�ho pole editoru textu
	hdwp = EditText::MoveEdit(hdwp);	

// posun displeje editoru zvuku
	hdwp = EditSound::MoveDisp(hdwp);

// posun displeje editoru hudby
	hdwp = EditMusic::MoveDisp(hdwp);

// konec p�esouv�n� oken
	::EndDeferWindowPos(hdwp);

// p�ekreslen� okna
	::UpdateWindow(MainFrame);
}


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� bufferu programu z handle okna (NULL = nen�)

CBufProg* BufProgFromHandle(HWND hWnd)
{
	if ((hWnd == TreeEdi) || 
		(hWnd == MainFrame) || 
		(hWnd == EditNumWnd) ||
		(hWnd == EditLogWnd) ||
		(hWnd == EditTextWnd) ||
		(hWnd == EditMapNumWnd) ||
		(hWnd == ToolEditIcon) || 
		(hWnd == ToolEditIconWidth) ||
		(hWnd == ToolEditMap) || 
		(hWnd == HScroll) ||
		(hWnd == VScroll)
		)
	{
		return &BufEdi;
	}

	if (hWnd == TreeStr)
	{
		return &BufStr;
	}

	if (hWnd == TreeObj)
	{
		return &BufObj;
	}

	if (hWnd == TreeLoc)
	{
		return &BufLoc;
	}

	if (hWnd == TreeCls)
	{
		return &BufCls;
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� ID bufferu programu z handle okna (-1 = nen�)

int BufProgIDFromHandle(HWND hWnd)
{
	if ((hWnd == TreeEdi) || 
		(hWnd == MainFrame) || 
		(hWnd == EditNumWnd) ||
		(hWnd == EditLogWnd) ||
		(hWnd == EditTextWnd) ||
		(hWnd == EditMapNumWnd) ||
		(hWnd == ToolEditIcon) || 
		(hWnd == ToolEditIconWidth) ||
		(hWnd == ToolEditMap) || 
		(hWnd == HScroll) ||
		(hWnd == VScroll)
		)
	{
		return BufEdiID;
	}

	if (hWnd == TreeStr)
	{
		return BufStrID;
	}

	if (hWnd == TreeObj)
	{
		return BufObjID;
	}

	if (hWnd == TreeLoc)
	{
		return BufLocID;
	}

	if (hWnd == TreeCls)
	{
		return BufClsID;
	}

	return -1;
}


/////////////////////////////////////////////////////////////////////////////
// zah�jen� rozvinut� polo�ky (vrac� TRUE=z�kaz zm�ny)
// (pozor - vyvol� se po dvojkliku v edita�n�m okn� u zru�en� polo�ky,
//  bez z�kazu rozvinut� vznikne chyba pam�ti! Bez t�to obsluhy havaruje!)

BOOL ProgOnExpanding(HWND hWnd, TV_ITEM* tvi, BOOL expand)
{
// kontrola, zda je re�im editace
	if (!ProgMode) return FALSE;

// rozli�en� zdrojov�ho okna
	CBufProg* buf = BufProgFromHandle(hWnd);
	if (buf == NULL) return TRUE;

// nalezen� polo�ky
	int index = buf->Find(tvi->hItem);
	if (index < 0) return TRUE;

// obsluha okna t��d
	if (buf->BufID() == BufClsID)
	{
		if ((buf->At(index).Func != IDF_GROUP) &&
			(buf->At(index).Parent >= 0))
		{
			return TRUE;
		}

		if (!expand) return FALSE;

		if ((buf->At(index).Child < 0) || (buf->At(index).Param & PR_CD2))
		{
			ProgLib::Load(index);
		}
		return (buf->At(index).Child < 0);
	}

// polo�ky bez dat lze rozvinout kdekoliv
	if ((buf->At(index).DatBlok < 0) && 
		(buf->At(index).RefBlok != BufObjID) &&
		(buf->At(index).RefBlok != BufLocID))
	{
		return FALSE;
	}

// p�i dvojkliku je nutno zkontrolovat m�sto kliknut�
	if (LMouseDClick || RMouseDClick)
	{
		TV_HITTESTINFO tvh;
		tvh.pt = MouseScreen;
		::ScreenToClient(buf->Tree(), &tvh.pt);
		::SendMessage(buf->Tree(), TVM_HITTEST, 0, (LPARAM)&tvh);
//		ASSERT(tvh.hItem == tvi->hItem);
		return ((tvh.flags & TVHT_ONITEMBUTTON) == 0);
	}
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// zm�na rozvinut� polo�ky

void ProgOnExpand(HWND hWnd, TV_ITEM* tvi, BOOL expand)
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// rozli�en� zdrojov�ho okna
	CBufProg* buf = BufProgFromHandle(hWnd);
	if (buf == NULL) return;

// nalezen� polo�ky
	int index = buf->Find(tvi->hItem);
	if (index < 0) return;

// nastaven� nov�ho p��znaku
	if (expand)
	{
		(*buf)[index].Param |= PR_EXP;
	}
	else
	{
		(*buf)[index].Param &= ~PR_EXP;
	}

	TimeRepaint(buf->BufID());
};


/////////////////////////////////////////////////////////////////////////////
// zm�na vybran� polo�ky

void ProgOnSelChanged(HWND hWnd, NM_TREEVIEW* tv)
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// aktualizace p�ep�na�� pro vybranou polo�ku
	ProgAktItem();

// zah�jen� ��t�n� preview
	PreviewStop();
}

/////////////////////////////////////////////////////////////////////////////
// zv�t�en� m���tka zobrazen�

void ProgOnZoomIn()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// nastaven� ikon pro strom
	if ((FocusBuf != BufEdiID) || (EditMode == BufEdiID))
	{
		SMALLICON zoom;
		switch (BufZoom[FocusBuf])
		{
		case SI_BIG:	return;
		case SI_MEDIUM:	zoom = SI_BIG; break;
		default: zoom = SI_MEDIUM;
		}
		BufZoom[FocusBuf] = zoom;
		BufProg[FocusBuf].IconList(zoom);
		EnableCommand(IDN_IN, (zoom == SI_MEDIUM));
		EnableCommand(IDN_OUT, TRUE);
		return;
	}

// nastaven� pro edita�n� pole
	if (FocusBuf == BufEdiID)
	{
		switch (EditMode)
		{
		case BufIcoID:
		case BufPicID:
			EditIcon::ZoomIn();
			return;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// zmen�en� m���tka zobrazen�

void ProgOnZoomOut()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// nastaven� ikon pro strom
	if ((FocusBuf != BufEdiID) || (EditMode == BufEdiID))
	{
		SMALLICON zoom;
		switch (BufZoom[FocusBuf])
		{
		case SI_SMALL:	return;
		case SI_MEDIUM:	zoom = SI_SMALL; break;
		default: zoom = SI_MEDIUM;
		}
		BufZoom[FocusBuf] = zoom;
		BufProg[FocusBuf].IconList(zoom);
		EnableCommand(IDN_IN, TRUE);
		EnableCommand(IDN_OUT, (zoom == SI_MEDIUM));
		return;
	}

// nastaven� pro edita�n� pole
	if (FocusBuf == BufEdiID)
	{
		switch (EditMode)
		{
		case BufIcoID:
		case BufPicID:
			EditIcon::ZoomOut();
			return;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� edita�n�ho m�du Toolbaru (podle ID bufferu, -1 = vypnout)

void SetToolMode(int bufID)
{
// korekce bufferu
	if ((DWORD)bufID < (DWORD)PROGBUFNUM)
	{
		bufID = BufEdiID;
	}

// kontrola, zda je m�d ji� nastaven
	if (bufID == ToolMode) return;

	if (ToolEditIcon)
	{
		::DestroyWindow(ToolEditIcon);
		ToolEditIcon = NULL;
	}

	if (ToolEditIconWidth)
	{
		::DestroyWindow(ToolEditIconWidth);
		ToolEditIconWidth = NULL;
	}

	if (ToolEditMap)
	{
		::DestroyWindow(ToolEditMap);
		ToolEditMap = NULL;
	}

// aktualizace menu
	ProgSetMenu();

// nulov�n� panelu n�stroj�
	ToolBarClear(ToolBarProgramNum);

// nastaven� panelu n�stroj� podle edita�n�ho m�du
	switch (bufID)
	{
	case BufEdiID:	ToolBarAdd(ToolBarEditProg, ToolBarEditProgNum);
					break;

	case BufNumID:	
					break;

	case BufTxtID:	
					break;

	case BufLogID:	
					break;

	case BufIcoID:	ToolBarAdd(ToolBarEditIcon, ToolBarEditIconNum);

					if (ToolEditIcon == NULL)
					{
						ToolEditIcon = CreateToolCombo(IDD_PEN, ToolBarEditPicTools, 0);
					}

					if (ToolEditIconWidth == NULL)
					{
						ToolEditIconWidth = CreateToolCombo(IDD_WIDTH, ToolBarEditPicWidths, 1);
					}

					EditIcon::UpdateMenu();
					break;

	case BufMapID:	ToolBarAdd(ToolBarEditMap, ToolBarEditMapNum);

					if (ToolEditMap == NULL)
					{
						ToolEditMap = CreateToolCombo(IDD_MAP, ToolBarEditMapTools, 0);
					}

					EditMap::UpdateMenu();
					break;

	case BufSprID:	ToolBarAdd(ToolBarEditSprite, ToolBarEditSpriteNum);
//					EditSprite::UpdateMenu();
					break;

	case BufSndID:	ToolBarAdd(ToolBarEditSound, ToolBarEditSoundNum);
					EditSound::UpdateMenu();
					break;

	case BufPicID:	ToolBarAdd(ToolBarEditPic, ToolBarEditPicNum);

					if (ToolEditIcon == NULL)
					{
						ToolEditIcon = CreateToolCombo(IDD_PEN, ToolBarEditPicTools, 0);
					}

					if (ToolEditIconWidth == NULL)
					{
						ToolEditIconWidth = CreateToolCombo(IDD_WIDTH, ToolBarEditPicWidths, 1);
					}

					EditIcon::UpdateMenu();
					break;

	case BufMusID:	ToolBarAdd(ToolBarEditMusic, ToolBarEditMusicNum);
					EditMusic::UpdateMenu();
					break;
	}

	ToolMode = bufID;

// aktualizace zobrazen� panelu n�stroj�
	ProgOnSize();
}


/////////////////////////////////////////////////////////////////////////////
// vypnut� edita�n�ch oken (-1 = v�e)

void HideEditWnd(int bufID)
{
// skryt� editoru programu
	if (bufID != BufEdiID)
	{
		::ShowWindow(TreeEdi, SW_HIDE);
	}

// skryt� editoru ��sel
	if (bufID != BufNumID)
	{
		::ShowWindow(EditNumWnd, SW_HIDE);
	}

// skryt� editoru logick�ch hodnot
	if (bufID != BufLogID)
	{
		::ShowWindow(EditLogWnd, SW_HIDE);
	}

// skryt� editoru textu
	if (bufID != BufTxtID)
	{
		::ShowWindow(EditTextWnd, SW_HIDE);
	}

// skryt� okna editoru zvuku
	if (bufID != BufSndID)
	{
		::ShowWindow(EditSound::DispWnd, SW_HIDE);
	}

// skryt� okna editoru hudby
	if (bufID != BufMusID)
	{
		::ShowWindow(EditMusic::DispWnd, SW_HIDE);
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� edita�n�ho m�du ed�ta�n�ho panelu (TRUE=OK) (BufPicID a -2 = editace obr�zku sprajtu)

BOOL SetEditMode(int bufID, int index)
{
// p��prava indexu dat pro hled�n� v buferech
	int datblok = bufID;
	int datindex = index;
	if ((bufID == BufPicID) && (datindex == -2))
	{
		datblok = BufSprID;
		datindex = EditSprite::Index;
	}

// nalezen� dat v glob�ln�m bufferu
	int editinx = BufObj.SrcDat(datblok, datindex);
	if (editinx >= 0)
	{
		BufObj.Def(editinx);
		BufLoc.Def(-1);
		BufEdi.Nadpis(Text.GetTxt(BufObj.GetText(editinx)));
		if (datblok == BufLocID)
		{
			BufLoc.Disp(datindex);
		}
	}
	else

// nalezen� dat v lok�ln�m bufferu
	{
		editinx = BufLoc.SrcDat(datblok, datindex);
		if (editinx >= 0)
		{
			BufLoc.Def(editinx);
			BufEdi.Nadpis(Text.GetTxt(BufLoc.GetText(editinx)));
			while (BufLoc[editinx].Parent >= 0)
			{
				editinx = BufLoc[editinx].Parent;
			}
			BufLoc.Disp(editinx);
			BufObj.Def(BufObj.SrcDat(BufLocID, editinx));
		}
		else
		{
			if ((bufID != BufIcoID) || (Icon.IsNotValid(index)))
			{
				return FALSE;
			}

			if (bufID == BufIcoID)
			{
				for (int i = BufEdi.Max()-1; i >= 0; i--)
				{
					if ((BufEdi.IsValid(i)) && 
						(BufEdi[i].Icon == index) &&
						(BufEdi[i].Func == IDF_COLOR))
					{
						bufID = BufEdiID;
						while (BufEdi[i].Parent >= 0) i = BufEdi[i].Parent;
						index = i;
						break;
					}
				}
			}
		}
	}

// korekce bufferu
	int bufID0 = bufID;
	if ((DWORD)bufID < (DWORD)PROGBUFNUM)
	{
		bufID = BufEdiID;
	}

// nastaven� panelu n�stroj�
	if (FocusBuf == BufEdiID)
	{
		SetToolMode(bufID);
	}

// vypnut� posuvn�k�
	if (bufID != EditMode)
	{
		HScrollDisp = FALSE;
		VScrollDisp = FALSE;
		::ShowWindow(HScroll, SW_HIDE);
		::ShowWindow(VScroll, SW_HIDE);
	}

// vypnut� editac�
	EditSound::OnStop();
	EditMusic::OnStop();
	EditMap::EndEditSwcNum();

// zafixov�n� zm�n UNDO
	Undo.Fixup();

// nastaven� edita�n�ho pole podle edita�n�ho m�du
	switch (bufID0)
	{
	case BufLocID:	if (BufLoc.IsValid(index) && (BufLoc[index].DatBlok == BufEdiID))
					{
						HistEdit::Push();
						EditMode = bufID;
						EditBuf = bufID0;
						EditItem = index;
						index = BufLoc[index].DatIndex;
						::ShowWindow(TreeEdi, SW_SHOW);
						BufEdi.Disp(index);
						HideEditWnd(bufID);
						ProgUpdateInOut();
						ProgOnSize();
						return TRUE;
					}
					break;

	case BufEdiID:	if (BufEdi.IsValid(index))
					{
						HistEdit::Push();
						EditMode = bufID;
						EditBuf = bufID0;
						EditItem = index;
						::ShowWindow(TreeEdi, SW_SHOW);
						BufEdi.Disp(index);
						HideEditWnd(bufID);
						ProgUpdateInOut();
						ProgOnSize();
						return TRUE;
					}
					break;

	case BufNumID:	if (Real.IsValid(index))
					{
						HistEdit::Push();
						EditMode = bufID;
						EditBuf = bufID0;
						EditItem = index;
						EditNum::BegEdit(index);
						HideEditWnd(bufID);
						ProgUpdateInOut();
						ProgOnSize();
						return TRUE;
					}
					break;

	case BufTxtID:	if (Text.IsValid(index))
					{
						HistEdit::Push();
						EditMode = bufID;
						EditBuf = bufID0;
						EditItem = index;
						EditText::BegEdit(index);
						HideEditWnd(bufID);
						ProgUpdateInOut();
						ProgOnSize();
						return TRUE;
					}
					break;

	case BufLogID:	if (Bool.IsValid(index))
					{
						HistEdit::Push();
						EditMode = bufID;
						EditBuf = bufID0;
						EditItem = index;
						EditLog::BegEdit(index);
						HideEditWnd(bufID);
						ProgUpdateInOut();
						ProgOnSize();
						return TRUE;
					}
					break;

	case BufIcoID:	if (Icon.IsValid(index))
					{
						HistEdit::Push();
						EditMode = bufID;
						EditBuf = bufID0;
						EditItem = index;
						EditIcon::BegEdit(BufIcoID, index);
						HideEditWnd(bufID);
						ProgUpdateInOut();
						ProgOnSize();
						return TRUE;
					}
					break;

	case BufMapID:	if (Map.IsValid(index))
					{
						HistEdit::Push();
						EditMode = bufID;
						EditBuf = bufID0;
						EditItem = index;
						EditMap::BegEdit(index);
						HideEditWnd(bufID);
						ProgUpdateInOut();
						ProgOnSize();
						return TRUE;
					}
					break;

	case BufPicID:	if ((index == -2) || Picture.IsValid(index))
					{
						HistEdit::Push();
						EditMode = bufID;
						EditBuf = bufID0;
						EditItem = index;
						EditIcon::BegEdit(BufPicID, index);
						HideEditWnd(bufID);
						ProgUpdateInOut();
						ProgOnSize();
						return TRUE;
					}
					break;

	case BufSprID:	if (Sprite.IsValid(index))
					{
						HistEdit::Push();
						EditMode = bufID;
						EditBuf = bufID0;
						EditItem = index;
						EditSprite::BegEdit(index);
						HideEditWnd(bufID);
						ProgUpdateInOut();
						ProgOnSize();
						return TRUE;
					}
					break;

	case BufSndID:	if (Sound.IsValid(index))
					{
						HistEdit::Push();
						EditMode = bufID;
						EditBuf = bufID0;
						EditItem = index;
						EditSound::BegEdit(index);
						HideEditWnd(bufID);
						ProgUpdateInOut();
						ProgOnSize();
						return TRUE;
					}
					break;

	case BufMusID:	if (Music.IsValid(index))
					{
						HistEdit::Push();
						EditMode = bufID;
						EditBuf = bufID0;
						EditItem = index;
						EditMusic::BegEdit(index);
						HideEditWnd(bufID);
						ProgUpdateInOut();
						ProgOnSize();
						return TRUE;
					}
					break;
	}
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// vykreslen� pr�zdn�ho edita�n�ho pole

void ProgDispEmpty()
{
// otev�en� kontextu displeje
	HDC dc = ::GetDC(MainFrame);

// oblast okna
	RECT rc;
	rc.left = EditX + 2;
	rc.top = EditY + 2;
	rc.right = EditX + EditWidth - 2;
	rc.bottom = EditY + EditHeight - 2;

// vypln�n� okna
	::FillRect(dc, &rc, (HBRUSH)::GetStockObject(LTGRAY_BRUSH));

// uvoln�n� kontextu displeje
	::ReleaseDC(MainFrame, dc);
}


/////////////////////////////////////////////////////////////////////////////
// vykreslen� r�mu kolem edita�n�ho pole

void ProgDispFrame()
{
// otev�en� kontextu displeje
	HDC dc = ::GetDC(MainFrame);

// oblast okna
	RECT rc;
	rc.left = EditX;
	rc.top = EditY;
	rc.right = EditX + EditWidth;
	rc.bottom = EditY + EditHeight;

// vykreslen� r�me�ku
	::DrawEdge(dc, &rc, EDGE_SUNKEN, BF_RECT);

// uvoln�n� kontextu displeje
	::ReleaseDC(MainFrame, dc);
}


/////////////////////////////////////////////////////////////////////////////
// p��prava sou�adnic nadpisu okna

void ProgNadpisRect(RECT* rc, int bufID)
{
	switch (bufID)
	{
	case BufObjID:
		rc->left = ClientRect.left;
		rc->top = ClientRect.top;
		rc->right = TrackX1;
		break;

	case BufLocID:
		rc->left = ClientRect.left;
		rc->top = TrackY1;
		rc->right = TrackX1;
		break;

	case BufStrID:
		rc->left = TrackX2;
		rc->top = ClientRect.top;
		rc->right = ClientRect.right;
		break;

	case BufClsID:
		rc->left = TrackX2;
		rc->top = TrackY2;
		rc->right = ClientRect.right;
		break;

	default:
		rc->left = TrackX1;
		rc->top = EditY - NADPISHEIGHT;
		rc->right = TrackX2;
	}
	rc->bottom = rc->top + NADPISHEIGHT;
}

/////////////////////////////////////////////////////////////////////////////
// vykreslen� nadpis� oken

void ProgDispNadpis()
{
// otev�en� kontextu displeje
	HDC dc = ::GetDC(MainFrame);

// nastaven� nov�ho fontu
	CFont fnt;
	fnt.Bold = TRUE;
	HFONT newfont = GetFont(&fnt);
	HFONT oldfont = (HFONT)::SelectObject(dc, newfont);

// cyklus p�es v�echny buffery
	for (int bufID = 0; bufID < PROGBUFNUM; bufID++)
	{
		if (bufID != BufIntID)
		{

// adresa bufferu
			CBufProg* buf = BufProg + bufID;
			
// kontrola, zda je okno zobrazeno
			if (buf->Tree() != NULL)
			{
// text k zobrazen�
				CText txt = buf->Nadpis();
				if ((bufID == BufEdiID) && PreviewPic && PreviewPicName[PreviewPicAkt].IsNotEmpty())
				{
					int index = BufCls.Select();
					if (BufCls.IsValid(index))
					{
						txt = Text.GetTxt(BufCls.GetText(index));
					}
				}

// p��prava sou�adnic nadpisu okna
				RECT rc;
				ProgNadpisRect(&rc, bufID);

// zobrazen� r�me�ku kolem nadpisu
				::DrawEdge(dc, &rc, EDGE_BUMP, BF_RECT);

// posun sou�adnic na vnit�ek okna
				rc.left += 2;
				rc.top += 2;
				rc.right -= 2;
				rc.bottom -= 2;

// p��prava barvy podkladu a barvy p�sma
				COLORREF bcol;					// barva podkladu
				COLORREF tcol;					// barva p�sma
				if (bufID == FocusBuf)
				{
					bcol = ::GetSysColor(COLOR_ACTIVECAPTION);
					tcol = ::GetSysColor(COLOR_CAPTIONTEXT);
				}
				else
				{
					bcol = ::GetSysColor(COLOR_INACTIVECAPTION);
					tcol = ::GetSysColor(COLOR_INACTIVECAPTIONTEXT);
				}

// p��prava m�du k zobrazen� textu
				::SetBkColor(dc, bcol);
				::SetBkMode(dc, OPAQUE);
				::SetTextColor(dc, tcol);

// zji�t�n� d�lky textu
				RECT rc2 = rc;
				::DrawText(dc, txt, txt.Length(), &rc2, DT_CALCRECT);

// nastaven� zarovn�v�n� textu
				int left;
				if ((rc2.right - rc2.left) <= (rc.right - rc.left))
				{
					left = (rc.left + rc.right)/2;
					::SetTextAlign(dc, TA_CENTER | TA_BASELINE);
				}
				else
				{
					left = rc.left;
					::SetTextAlign(dc, TA_LEFT | TA_BASELINE);
				}

// zobrazen� textu s podkladem
				::ExtTextOut(dc, left, (rc.top + rc.bottom)/2 + 4, 
						ETO_CLIPPED | ETO_OPAQUE, &rc, 
						txt, txt.Length(), NULL);
			}
		}
	}

// n�vrat p�vodn�ho fontu
	::SelectObject(dc, oldfont);
	DelFont(newfont);

// uvoln�n� kontextu displeje
	::ReleaseDC(MainFrame, dc);
}


/////////////////////////////////////////////////////////////////////////////
// vykreslen� okna

void ProgOnPaint()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// obsluha re�imu preview obr�zku
	if (PreviewPic)
	{
// zobrazen� neplatn�ho obr�zku
		if (PreviewPicPic[PreviewPicAkt].IsEmpty())
		{
			ProgDispEmpty();
		}
		else
		{
// parametry plochy
			int widthc = EditWidth - 4;
			int heightc = EditHeight - 4;
			int leftc = EditX + 2;
			int topc = EditY + 2;

// parametry obr�zku
			int width = PreviewPicPic[PreviewPicAkt].Width();
			int height = PreviewPicPic[PreviewPicAkt].Height();

			int widtho = width;
			if (widtho > widthc) widtho = widthc;
			int heighto = height;
			if (heighto > heightc) heighto = heightc;

			int lefto = (widthc - widtho)/2 + leftc;
			int topo = (heightc - heighto)/2 + topc;

			int widthbyte = (widtho + 3) & ~3;

// otev�en� kontextu displeje
			HDC dc = ::GetDC(MainFrame);

// nastaven� vlastn�ch palet
			HPALETTE OldPal;					// �schova p�vodn�ch palet
			OldPal = ::SelectPalette(dc,		// v�b�r vlastn�ch palet
				StdPalette, FALSE);
			::RealizePalette(dc);				// realizace palet

// buffer k dek�dov�n� obr�zku
			BYTE* buf = (BYTE*)MemGet(widthbyte * heighto);
			if (buf)
			{
				BYTE* dst = buf;
				PreviewPicPic[PreviewPicAkt].DeComp();
				BYTE* src = PreviewPicPic[PreviewPicAkt].DataData() + (height - heighto)/2*width + (width - widtho)/2;

// dek�dov�n� obr�zku
				for (int i = heighto; i > 0; i--)
				{
					MemCopy(dst, src, widtho);
					dst += widthbyte;
					src += width;
				}

// p�ednastaven� parametr� z�hlav� bitmapy
				StdBitmapInfo->bmiHeader.biWidth = widtho;
				StdBitmapInfo->bmiHeader.biHeight = heighto;

// zobrazen� obr�zku z bufferu
				::StretchDIBits(dc, lefto, topo, widtho, heighto,
					0, 0, widtho, heighto, buf,
					StdBitmapInfo, DIB_RGB_COLORS, SRCCOPY);

// zru�en� bufferu k dek�dov�n� obr�zku
				MemFree(buf);
			}

// p��prava �t�tce k vymaz�n� podkladu
			HBRUSH brush = (HBRUSH)::GetStockObject(LTGRAY_BRUSH); // �t�tec k vymaz�n� plochy

// vymaz�n� plochy naho�e nad obr�zkem
			RECT rc;
			rc.left = leftc;
			rc.right = leftc + widthc;
			rc.top = topc;
			rc.bottom = topo;
			if (rc.top < rc.bottom)
			{
				::FillRect(dc, &rc, brush);
			}

// vymaz�n� plochy dole pod obr�zkem (left a right je nastaveno)
			rc.top = topo + heighto;
			rc.bottom = topc + heightc;
			if (rc.top < rc.bottom)
			{
				::FillRect(dc, &rc, brush);
			}

// vymaz�n� plochy vlevo od obr�zku (left je nastaveno)
			rc.right = lefto;
			rc.top = topo;
			rc.bottom = topo + heighto;
			if (rc.left < rc.right)
			{
				::FillRect(dc, &rc, brush);
			}

// vymaz�n� plochy vpravo od obr�zku (top a bottom je nastaveno)
			rc.left = lefto + widtho;
			rc.right = leftc + widthc;
			if (rc.left < rc.right)
			{
				::FillRect(dc, &rc, brush);
			}

// zru�en� �t�tce podkladu (i kdy� podle dokumentace ru�en� nen� nutn�)
			::DeleteObject(brush);

// n�vrat p�vodn�ch palet
			::SelectPalette(dc,OldPal,FALSE);

// uvoln�n� kontextu displeje
			::ReleaseDC(MainFrame, dc);
		}

// zobrazen� r�mu kolem plochy
		ProgDispFrame();

// zobrazen� nadpis� oken
		ProgDispNadpis();
		return;
	}

// rozli�en� podle m�du
	switch (EditMode)
	{
	case BufEdiID:
					break;

	case BufTxtID:
					break;

	case BufLogID:	EditLog::Disp();
					ProgDispFrame();
					break;

	case BufNumID:	EditNum::Disp();
					ProgDispFrame();
					break;

	case BufMapID:	EditMap::Disp();
					ProgDispFrame();
					break;

	case BufSprID:	EditSprite::Disp();
					ProgDispFrame();
					break;

	case BufSndID:	EditSound::Disp();
					ProgDispFrame();
					break;

	case BufMusID:	EditMusic::Disp();
					ProgDispFrame();
					break;

	case BufIcoID:
	case BufPicID:	EditIcon::Disp();
					ProgDispFrame();
					break;


	default:	ProgDispEmpty();
				ProgDispFrame();
	}

// zobrazen� nadpis� oken
	ProgDispNadpis();
}


/////////////////////////////////////////////////////////////////////////////
// dvojklik my�� - editace prvku

void ProgOnDblClk(HWND hWnd)
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// rozli�en� zdrojov�ho okna
	int bufID = BufProgIDFromHandle(hWnd);
	if (bufID < 0) return;
	CBufProg* buf = &BufProg[bufID];

// zji�t�n� aktivn� polo�ky
	TV_HITTESTINFO hti;					// struktura pro nalezen� polo�ky
	hti.pt = MouseScreen;
	::ScreenToClient(buf->Tree(), &hti.pt);
	int index = buf->Find((HTREEITEM) ::SendMessage(buf->Tree(), TVM_HITTEST, 0, (LPARAM)&hti));
	if (index < 0) index = buf->Select();
	if (index < 0) return;
	PROGITEM* item = &(*buf)[index];

// kontrola, zda nen� klik na rozbalovac� k��ek
	RECT rc;
	*(HTREEITEM*)(&rc) = item->HTree;
	if (::SendMessage(buf->Tree(), TVM_GETITEMRECT, TRUE, (LPARAM)&rc))
	{
		int pozx = rc.left - buf->IconWidth() - 3;
		if ((hti.pt.x < pozx) && (hti.pt.x > pozx - buf->IconWidth() - 4)) return;
	}

// edita�n� buffer
	if (bufID == BufEdiID)
	{

// volba barvy
		if (item->Func == IDF_COLOR)
		{
			ProgCol::SelectColor(index);
			return;
		}

// p�esm�rov�n� z editoru do okna buffer�
		bufID = item->RefBlok;
		index = item->RefIndex;
		if (((bufID != BufObjID) && (bufID != BufLocID)) || (index < 0)) return;
		buf = &BufProg[bufID];
		ASSERT(buf->IsValid(index));
		item = &(*buf)[index];
	}

// nastaven� edita�n�ho m�du
	item->Param &= ~PR_NEW;				// zru�en� p��znaku nov� polo�ky

	if ((item->DatBlok == BufEdiID) || (item->DatBlok == BufLocID))
	{
		SetFocusEditMode(item->DatBlok, item->DatIndex);
	}
	else
	{
		if (!SetEditMode(item->DatBlok, item->DatIndex)) return;
		SetFocusEdit();
	}
}


/////////////////////////////////////////////////////////////////////////////
// Enter - editace prvku

void ProgOnEnter(HWND hWnd)
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// rozli�en� zdrojov�ho okna
	int bufID = BufProgIDFromHandle(hWnd);
	if (bufID < 0) return;
	CBufProg* buf = &BufProg[bufID];

// zji�t�n� aktivn� polo�ky
	int index = buf->Select();
	if (index < 0) return;
	PROGITEM* item = &(*buf)[index];

	if (bufID == BufEdiID)
	{

// volba barvy
		if (item->Func == IDF_COLOR)
		{
			ProgCol::SelectColor(index);
			return;
		}

// p�esm�rov�n� z editoru do okna buffer�
		bufID = item->RefBlok;
		index = item->RefIndex;
		if (((bufID != BufObjID) && (bufID != BufLocID)) || (index < 0)) return;
		buf = &BufProg[bufID];
		ASSERT(buf->IsValid(index));
		item = &(*buf)[index];
	}

// zah�jen� preview v okn� t��d
	if (FocusBuf == BufClsID) PreviewStop();

// nastaven� edita�n�ho m�du
	item->Param &= ~PR_NEW;				// zru�en� p��znaku nov� polo�ky
	if (!SetEditMode(item->DatBlok, item->DatIndex)) return;
	ProgOnSize();						// zapnut� posuvn�k�

// zah�jen� p�ehr�v�n� WAV a MID
	if ((FocusBuf == BufObjID) || (FocusBuf == BufLocID))
	{
		ProgOnPlay();
	}
}


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� informace o ikon� k zobrazen�

void ProgOnGetDispInfo(HWND hWnd, TV_DISPINFO* tvd)
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// kontrola, zda jsou vytvo�eny platn� seznamy obr�zk�
	if ((ProgImageList == NULL) || (ProgImageListMedium == NULL) || (ProgImageListSmall == NULL)) return;

// rozli�en� zdrojov�ho okna
	CBufProg* buf = BufProgFromHandle(hWnd);
	if (buf == NULL) return;

// nalezen� polo�ky
	int index = buf->Find(tvd->item.hItem);
	if (index < 0) return;

// na�ten� ikony
	int icon = buf->GetIcon(index);
	if (icon < 0) icon = 0;
	ASSERT(Icon.IsValid(icon));
	Icon1.DelAll();
	Icon1.Add(Icon.Get(icon));

// vygenerov�n� nov�ho seznamu ikon
	if (buf->IconHeight() > (32+24)/2)
	{
		::ImageList_Remove(ProgImageList, -1);
		Icon1.GenerList(FALSE, SI_BIG, ProgImageList);
/*
		for (int i = ICONNUM-1; i >= 0; i--)
		{
			if (Icon32List[i] == icon)
			{
				tvd->item.iImage = i;
				tvd->item.iSelectedImage = i;
				return;
			}
		}

		Icon32[Icon32Next] = Icon.Get(icon);
		Icon32List[Icon32Next] = icon;
		tvd->item.iImage = Icon32Next;
		tvd->item.iSelectedImage = Icon32Next;
		Icon32Next++;
		if (Icon32Next >= ICONNUM) Icon32Next = 0;
		::ImageList_Remove(ProgImageList, -1);
		Icon32.GenerList(FALSE, FALSE, ProgImageList);
		return;
*/
	}
	else
	{
		if (buf->IconHeight() > (24+16)/2)
		{
			::ImageList_Remove(ProgImageListMedium, -1);
			Icon1.GenerList(FALSE, SI_MEDIUM, ProgImageListMedium);
	/*
			for (int i = ICONNUM-1; i >= 0; i--)
			{
				if (Icon32List[i] == icon)
				{
					tvd->item.iImage = i;
					tvd->item.iSelectedImage = i;
					return;
				}
			}

			Icon32[Icon32Next] = Icon.Get(icon);
			Icon32List[Icon32Next] = icon;
			tvd->item.iImage = Icon32Next;
			tvd->item.iSelectedImage = Icon32Next;
			Icon32Next++;
			if (Icon32Next >= ICONNUM) Icon32Next = 0;
			::ImageList_Remove(ProgImageList, -1);
			Icon32.GenerList(FALSE, FALSE, ProgImageList);
			return;
	*/
		}
		else
		{
			::ImageList_Remove(ProgImageListSmall, -1);
			Icon1.GenerList(FALSE, SI_SMALL, ProgImageListSmall);
	/*
			for (int i = ICONNUM-1; i >= 0; i--)
			{
				if (Icon16List[i] == icon)
				{
					tvd->item.iImage = i;
					tvd->item.iSelectedImage = i;
					return;
				}
			}

			Icon16[Icon16Next] = Icon.Get(icon);
			Icon16List[Icon16Next] = icon;
			tvd->item.iImage = Icon16Next;
			tvd->item.iSelectedImage = Icon16Next;
			Icon16Next++;
			if (Icon16Next >= ICONNUM) Icon16Next = 0;
			::ImageList_Remove(ProgImageListSmall, -1);
			Icon16.GenerList(FALSE, TRUE, ProgImageListSmall);
			return;
	*/
		}
	}

// nastaven� ikony k zobrazen�
	tvd->item.iImage = 0;
	tvd->item.iSelectedImage = 0;
}


/////////////////////////////////////////////////////////////////////////////
// obsluha modifikace

// Pozor na p�ekryv obr�zk� tla��tek - p�i ulo�en� kl�vesou Ctrl+S se obr�zky 
// p�ekresl� bez vymaz�n� podkladu, �ed� barva p�itom z�stane nep�ekreslena.

// zapnut� modifikace
void SetModi()
{
	if (!Modi)
	{
		Modi = TRUE;
		ToolBarBitmap(IDN_SAVE, ButtonSaveModi);
	}
}

// vypnut� modifikace
void ResModi()
{
	if (Modi)
	{
		Modi = FALSE;
		ToolBarBitmap(IDN_SAVE, ButtonSave);
	}
}


// podm�n�n� ulo�en� p�i modifikaci (TRUE=pokra�ovat)
BOOL TestModi()
{
	if (Modi)
	{
		CText text;
		CText nadpis;
		text.Load(IDN_FILEMODI);
		nadpis.Load(IDN_FILEMODICAP);
/*
		MSGBOXPARAMS par;
		par.cbSize = sizeof(MSGBOXPARAMS);
		par.hwndOwner = MainFrame;
		par.hInstance = hInstance;
		par.lpszText = text;
		par.lpszCaption = nadpis;
		par.dwStyle = MB_USERICON | MB_YESNOCANCEL | MB_APPLMODAL | MB_SETFOREGROUND;
		par.lpszIcon = MAKEINTRESOURCE(IDN_MAINFRAME);
		par.dwContextHelpId = 0;
		par.lpfnMsgBoxCallback = NULL;
		par.dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT);

		int volba = ::MessageBoxIndirect(&par);
*/
		int volba = ::MessageBox(MainFrame, text, nadpis,
				MB_YESNOCANCEL | MB_ICONQUESTION);

		if (volba == IDCANCEL)
			return FALSE;

		if (volba == IDNO)
		{
			return TRUE;
		}

		if (volba == IDYES)
		{
			ProgFile::SaveMaxi();
			if (!Modi) 
			{
				return TRUE;
			}
		}
		return FALSE;			// pro CANCEL nebo n�co jin�ho
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// test kurzoru my�i na p�ed�lu (v klientsk�ch sou�adnic�ch)

BOOL TestTrack(int x, int y, BOOL vert, BOOL left)
{
// �schova typu p�ed�lu
	VTracking = vert;
	LTracking = left;

// p��prava sou�adnic ta�en�ho obd�ln�ku
	RECT TrackRect;

	if (vert)
	{
		TrackRect.top = ClientRect.top;
		TrackRect.bottom = ClientRect.bottom;

		if (left)
		{
			TrackRect.left = TrackX1 - TRACKOFF;
			TrackRect.right = TrackX1 + TRACKOFF;
		}
		else
		{
			TrackRect.left = TrackX2 - TRACKOFF;
			TrackRect.right = TrackX2 + TRACKOFF;
		}
	}
	else
	{
		if (left)
		{
			TrackRect.top = TrackY1 - TRACKOFF;
			TrackRect.bottom = TrackY1 + TRACKOFF;
			TrackRect.left = ClientRect.left;
			TrackRect.right = TrackX1;
		}
		else
		{
			TrackRect.top = TrackY2 - TRACKOFF;
			TrackRect.bottom = TrackY2 + TRACKOFF;
			TrackRect.left = TrackX2;
			TrackRect.right = ClientRect.right;
		}
	}

// test, zda je bod nad p�ed�lem
	return ((x >= TrackRect.left) &&
			(x < TrackRect.right) &&
			(y >= TrackRect.top) &&
			(y < TrackRect.bottom));
}


/////////////////////////////////////////////////////////////////////////////
// obsluha zobrazen� kurzoru my�i (vrac� TRUE=nastaveno)

BOOL ProgOnSetCursor()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return FALSE;

// ta�en� m� vlastn� obsluhu
	if (Draging) return FALSE;

// nastaven� kurzoru my�i, je-li ta�en� p�ed�lu
	if (Tracking)
	{
		if (VTracking)
		{
			CurAkt = CurSplitH;		// ta�en� vertik�ln�ho p�ed�lu
		}
		else
		{
			CurAkt = CurSplitV;		// ta�en� horizont�ln�ho p�ed�lu
		}

		::SetCursor(CurAkt);		// nastaven� kurzoru
		return TRUE;
	}

// p��prava ke zji�t�n�, zda je p�ed�l k ta�en�
//	::GetCursorPos(&MouseScreen);	// na�ten� sou�adnic my�i
//	MouseMain = MouseScreen;
//	::ScreenToClient(MainFrame, &MouseMain); // p�evod na klientsk� sou�adnice

	POINT pt = MouseMain;

// test vertik�ln�ch p�ed�l�
	if (TestTrack(pt.x, pt.y, TRUE, TRUE) ||
		TestTrack(pt.x, pt.y, TRUE, FALSE))
	{
		CurAkt = CurSplitH;		// ta�en� vertik�ln�ho p�ed�lu
		::SetCursor(CurAkt);	// nastaven� kurzoru
		return TRUE;
	}

// test horizont�ln�ch p�ed�l�
	if (TestTrack(pt.x, pt.y, FALSE, TRUE) ||
		TestTrack(pt.x, pt.y, FALSE, FALSE))
	{
		CurAkt = CurSplitV;		// ta�en� horizont�ln�ho p�ed�lu
		::SetCursor(CurAkt);	// nastaven� kurzoru
		return TRUE;
	}

// obsluha zobrazen� kurzoru v editoru
	if ((pt.x >= EditX) && 
		(pt.x < EditX + EditWidth) &&
		(pt.y >= EditY) &&
		(pt.y < EditY + EditHeight) &&
		!PreviewPic)
	{
		switch (EditMode)
		{
		case BufMapID:
			if (EditMap::OnSetCursor(pt.x, pt.y))
				return TRUE;
			break;

		case BufIcoID:
		case BufPicID:
			if (EditIcon::OnSetCursor(pt.x, pt.y))
				return TRUE;
			break;
		}
	}

// jinak nen� obsluha
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// zah�jen� ta�en� p�ed�lu (TRUE=obslou�eno)

BOOL BegTrack(int x, int y, BOOL vert, BOOL left)
{
// test, zda je p�ed�l
	if (TestTrack(x, y, vert, left))
	{

// p��znak zah�jen� ta�en�
		Tracking = TRUE;				// p��znak obsluhy ta�en�

// zah�jen� zachyt�v�n� my�i
		::SetCapture(MainFrame);
		return TRUE;
	}
	
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// ukon�en� ta�en� p�ed�lu

void EndTrack()
{
// test, zda prob�h� ta�en� p�ed�lu
	if (Tracking)
	{

// uvoln�n� zachyt�v�n� my�i
		::ReleaseCapture();			// ukon�en� zachyt�v�n� my�i

// vypnut� p��znaku ta�en�
		Tracking = FALSE;			// vypnut� p��znaku ta�en�
	}
}


/////////////////////////////////////////////////////////////////////////////
// stisk tla��tka my�i (TRUE = obslou�eno)

BOOL ProgOnButtonDown(UINT flags, int x, int y, BOOL right, BOOL dclick)
{
// kontrola, zda je re�im editace
	if (!ProgMode) return FALSE;

// kontrola, zda nen� ta�en�
	if (!Draging && !Tracking)
	{

// p�epnut� aktivn�ho panelu
		RECT rc;
		POINT pt;
		pt.x = x;
		pt.y = y;

		for (int bufID = 0; bufID < PROGBUFNUM; bufID++)
		{
			if ((bufID != BufIntID) && (bufID != FocusBuf))
			{
				ProgNadpisRect(&rc, bufID);
				switch (bufID)
				{
				case BufLocID:
					rc.top += TRACKOFF;
				case BufObjID:
					rc.right -= TRACKOFF;
					break;

				case BufEdiID:
					rc.right -= TRACKOFF;
					rc.bottom = EditY + EditHeight;
					rc.left += TRACKOFF;
					break;

				case BufClsID:
					rc.top += TRACKOFF;
				default:
					rc.left += TRACKOFF;
				}

				if (PtInRect(&rc, pt))
				{
					FocusBuf = bufID;

					if (PreviewPic)
					{
						ProgSetFocus();
						return TRUE;
					}

					ProgSetFocus();
				}
			}
		}

// zah�jen� ta�en� p�ed�lu
		if (BegTrack(x, y, TRUE, TRUE) ||
			BegTrack(x, y, TRUE, FALSE) ||
			BegTrack(x, y, FALSE, TRUE) ||
			BegTrack(x, y, FALSE, FALSE))
		{
			return TRUE;
		}
	}

// p�eru�en� ta�en�
	BreakDrag();
	EndTrack();

// obsluha edita�n�ho pole
	if ((x >= EditX) &&
		(x < EditX + EditWidth) &&
		(y >= EditY) &&
		(y < EditY + EditHeight) &&
		!PreviewPic)
	{
		switch(EditMode)
		{
		case BufMapID:
			if (EditMap::OnButtonDown(flags, x, y, right, dclick)) return TRUE;
			break;

		case BufSprID:
			if (EditSprite::OnButtonDown(flags, x, y, right, dclick)) return TRUE;
			break;

		case BufPicID:
		case BufIcoID:
			if (EditIcon::OnButtonDown(flags, x, y, right, dclick)) return TRUE;
			break;
		}
	}


	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// posun my�i p�i ta�en�

void ProgOnMouseMove(UINT flags, int x, int y)
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// p�eru�en� ta�en�
	if ((flags & (MK_LBUTTON | MK_RBUTTON)) == 0)
	{
		BreakDrag();
		EndTrack();
	}

// obsluha ta�en�
	if (Draging)
	{
		ProgOnMoveDrag(flags, x, y);
		return;
	}

	if ((EditMode == BufMapID) && (EditMap::MDraging))
	{
		EditMap::OnMouseMove(flags, x, y);
		return;
	}

	if ((EditMode == BufSprID) && (EditSprite::MDraging))
	{
		EditSprite::OnMouseMove(flags, x, y);
		return;
	}

// obsluha ta�en� p�ed�lu
	if (Tracking)
	{

// vertik�ln� p�ed�l
		if (VTracking)
		{

// p�vodn� sou�adnice X
			int oldx;

			if (LTracking)
			{
				oldx = TrackX1;
			}
			else
			{
				oldx = TrackX2;
			}

// nov� sou�adnice X
			int newx = x;
			if (newx < ClientRect.left + 4) newx = ClientRect.left + 4;
			if (newx > ClientRect.right - 4) newx = ClientRect.right - 4;

			if (LTracking)
			{
				if (newx > TrackX2 - 2*TRACKOFF) newx = TrackX2 - 2*TRACKOFF;
			}
			else
			{
				if (newx < TrackX1 + 2*TRACKOFF) newx = TrackX1 + 2*TRACKOFF;
			}

// kontrola, zda se sou�adnice m�n�
			if (newx != oldx)
			{

// nastaven� nov� sou�adnice
				if (LTracking)
				{
					TrackX1 = newx;
					LeftWidth = ((newx - ClientRect.left)*Scale + ClientWidth/2)/ClientWidth;
				}
				else
				{
					TrackX2 = newx;
					RightWidth = Scale - ((newx - ClientRect.left)*Scale + ClientWidth/2)/ClientWidth;
				}

// aktualizace zobrazen� oken
				ProgOnSize();
				::UpdateWindow(MainFrame);
			}
		}

// horizont�ln� p�ed�l
		else
		{

// p�vodn� sou�adnice Y
			int oldy;

			if (LTracking)
			{
				oldy = TrackY1;
			}
			else
			{
				oldy = TrackY2;
			}

// nov� sou�adnice Y 
			int newy = y;
			if (newy < ClientRect.top + NADPISHEIGHT + 4) newy = ClientRect.top + NADPISHEIGHT + 4;
			if (newy > ClientRect.bottom - NADPISHEIGHT - 4) newy = ClientRect.bottom - NADPISHEIGHT - 4;

// kontrola, zda se sou�adnice m�n�
			if (newy != oldy)
			{

// nastaven� nov� sou�adnice
				if (LTracking)
				{
					TrackY1 = newy;
					LeftHeight = ((newy - ClientRect.top)*Scale + ClientHeight/2)/ClientHeight;
				}
				else
				{
					TrackY2 = newy;
					RightHeight = ((newy - ClientRect.top)*Scale + ClientHeight/2)/ClientHeight;
				}

// aktualizace zobrazen� oken
				ProgOnSize();
				::UpdateWindow(MainFrame);
			}
		}
	}


// obsluha edita�n�ho pole
	if ((x >= EditX) &&
		(x < EditX + EditWidth) &&
		(y >= EditY) &&
		(y < EditY + EditHeight) &&
		!PreviewPic)
	{	
		switch(EditMode)
		{
		case BufMapID:
			EditMap::OnMouseMove(flags, x, y);
			break;

		case BufSprID:
			EditSprite::OnMouseMove(flags, x, y);
			break;

		case BufPicID:
		case BufIcoID:
			EditIcon::OnMouseMove(flags, x, y);
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// uvoln�n� tla��tka my�i

void ProgOnButtonUp(UINT keys, BOOL right)
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;
	
// ukon�en� ta�en�
	ProgOnEndDrag();
	EndTrack();

	if (EditName)
	{
		ProgUpdateClipboard();
		return;
	}

// obsluha edita�n�ho pole
	switch(EditMode)
	{
	case BufNumID:
		EditNum::UpdateClipboard();
		break;

	case BufMapID:
		EditMap::OnButtonUp(keys, right);
		EditMap::UpdateClipboard();
		break;

	case BufSprID:
		EditSprite::OnButtonUp(keys, right);
		break;

	case BufTxtID:
		EditText::UpdateClipboard();
		break;

	case BufPicID:
	case BufIcoID:
		EditIcon::OnButtonUp(keys, right);
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// p�epnut� stavu LOCK prvku pod kurzorem

void ProgOnLock()
{

// kontrola, zda je re�im editace
	if (!ProgMode) return;

// ukon�en� preview
	PreviewStop();

// test, zda je okno stromu
	if ((FocusBuf != BufEdiID) ||
		(EditMode == BufEdiID))
	{

// nalezen� aktivn�ho prvku
		CBufProg* buf = BufProg + FocusBuf;
		int index = buf->Select();
		if (index >= 0)
		{

// adresa polo�ky
			PROGITEM* item = &((*buf)[index]);

// prvek nesm� b�t z�visle uzamknut�
			if ((item->Param & PR_LOCK_DEP) || (item->Param & PR_CD & PR_CD2)) return;

// p�epnut� stavu pro okno t��d
			if ((FocusBuf == BufClsID) &&
				(item->Parent >= 0))
			{
				ProgLib::InitName(index);

				int atr = (int) :: GetFileAttributes(ProgLib::m_Name);
				if (atr == -1) return;
				atr &= ~(FILE_ATTRIBUTE_READONLY + FILE_ATTRIBUTE_DIRECTORY);
				if ((item->Param & PR_LOCK) == 0)
				{
					atr |= FILE_ATTRIBUTE_READONLY;
				}
				if (!::SetFileAttributes(ProgLib::m_Name, atr)) return;

				if (ProgLib::m_IconName.IsNotEmpty())
				{
					int atr = (int) :: GetFileAttributes(ProgLib::m_IconName);
					if (atr != -1)
					{
						atr &= ~(FILE_ATTRIBUTE_READONLY + FILE_ATTRIBUTE_DIRECTORY);
						if ((item->Param & PR_LOCK) == 0)
						{
							atr |= FILE_ATTRIBUTE_READONLY;
						}
						::SetFileAttributes(ProgLib::m_IconName, atr);
					}
				}

				ProgDirChangeIgnore = 2;
			}

// zm�na p��znaku uzam�en�
			item->Param ^= PR_LOCK;

// aktualizace zobrazen� prvku ve stromu
			TV_ITEM tvi;
			tvi.mask = TVIF_STATE;
			tvi.hItem = item->HTree;
			tvi.stateMask = TVIS_BOLD;
			tvi.state = (item->Param & (PR_LOCK | PR_LOCK_DEP)) ? TVIS_BOLD : 0;
			::SendMessage(buf->Tree(), TVM_SETITEM, 0, (LPARAM)&tvi);

// �schova UNDO
			Undo.AddProgLock(FocusBuf, index);
			Undo.Stop();

// aktualizace prvk� v buferech
			UpdateLock();

// aktualizace p�ep�na�� pro vybranou polo�ku
			ProgAktItem();

// nastaven� p��znaku modifikace
			SetModi();

			if (FocusBuf == BufClsID)
			{
				Undo.DelAll();
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// p�epnut� stavu OFF prvku pod kurzorem

void ProgOnOff()
{

// kontrola, zda je re�im editace
	if (!ProgMode) return;

// ukon�en� preview
	PreviewStop();

// test, zda je okno stromu
	if ((FocusBuf != BufEdiID) ||
		(EditMode == BufEdiID))
	{

// nalezen� aktivn�ho prvku
		CBufProg* buf = BufProg + FocusBuf;
		int index = buf->Select();
		if (index >= 0)
		{

// adresa polo�ky
			PROGITEM* item = &((*buf)[index]);

// prvek nesm� b�t uzamknut�
			if (item->Param & (PR_LOCK | PR_LOCK_DEP)) return;

// p�epnut� stavu pro okno t��d
			if ((FocusBuf == BufClsID) &&
				(item->Parent >= 0))
			{
				ProgLib::InitName(index);

				int atr = (int) :: GetFileAttributes(ProgLib::m_Name);
				if (atr == -1) return;
				atr &= ~(FILE_ATTRIBUTE_HIDDEN + FILE_ATTRIBUTE_DIRECTORY);
				if ((item->Param & PR_OFF) == 0)
				{
					atr |= FILE_ATTRIBUTE_HIDDEN;
				}
				if (!::SetFileAttributes(ProgLib::m_Name, atr)) return;

				if (ProgLib::m_IconName.IsNotEmpty())
				{
					int atr = (int) :: GetFileAttributes(ProgLib::m_IconName);
					if (atr != -1)
					{
						atr &= ~(FILE_ATTRIBUTE_HIDDEN + FILE_ATTRIBUTE_DIRECTORY);
						if ((item->Param & PR_OFF) == 0)
						{
							atr |= FILE_ATTRIBUTE_HIDDEN;
						}
						::SetFileAttributes(ProgLib::m_IconName, atr);
					}
				}

				ProgDirChangeIgnore = 2;
			}

// zm�na p��znaku vypnut�
			item->Param ^= PR_OFF;

// aktualizace zobrazen� prvku ve stromu
			TV_ITEM tvi;
			tvi.mask = TVIF_STATE;
			tvi.hItem = item->HTree;
			tvi.stateMask = TVIS_CUT;
			tvi.state = (item->Param & (PR_OFF | PR_OFF_DEP)) ? TVIS_CUT : 0;
			::SendMessage(buf->Tree(), TVM_SETITEM, 0, (LPARAM)&tvi);

// �schova UNDO
			Undo.AddProgOff(FocusBuf, index);
			Undo.Stop();

// aktualizace prvk� v buferech
			UpdateLock();

// aktualizace p�ep�na�� pro vybranou polo�ku
			ProgAktItem();

// nastaven� p��znaku modifikace
			SetModi();

			if (FocusBuf == BufClsID)
			{
				Undo.DelAll();
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace zobrazen� jm�na z�visl�ch polo�ek

void AktNameTree(int bufID, CText& txt, int refBlok, int refIndex)
{

// kontrola platnosti z�vislosti
	switch (refBlok)
	{
	case BufStrID:
		switch (bufID)
		{
		case BufObjID:
		case BufLocID:
		case BufClsID:
		case BufEdiID:
			break;
		default:
			return;
		}
		break;

	case BufClsID:
		switch (bufID)
		{
		case BufObjID:
		case BufLocID:
			break;
		default:
			return;
		}
		break;

	case BufLocID:
	case BufObjID:
		switch (bufID)
		{
		case BufEdiID:
			break;
		default:
			return;
		}
		break;

	default:
		return;
	}

// lok�ln� prom�nn�
	CBufProg* buf = BufProg + bufID;
	int index = buf->Max() - 1;
	PROGITEM* item = buf->Data() + index;
	TV_ITEM tvi;

// cyklus p�es v�echny polo�ky
	for (; index >= 0; index--)
	{

// test, zda je polo�ka platn�, zda referuje na hledanou polo�ku a zda m� implicitn� text
		if ((buf->IsValid(index)) &&
			(item->RefIndex == refIndex) &&
			(item->RefBlok == refBlok) &&
			(item->Name < 0))
		{

// oprava textu polo�ky ve stromu
			if (item->HTree)
			{
				tvi.hItem = item->HTree;
				tvi.mask = TVIF_TEXT;
				tvi.pszText = (LPTSTR)(LPCTSTR)txt;
				::SendMessage(buf->Tree(), TVM_SETITEM, 0, (LPARAM)&tvi);
			}

// aktualizace zobrazen� v z�visl�ch bufferech
			AktNameTree(BufObjID, txt, bufID, index);
			AktNameTree(BufLocID, txt, bufID, index);
			AktNameTree(BufEdiID, txt, bufID, index);
			AktNameTree(BufClsID, txt, bufID, index);
		}

// posun adresy polo�ky
		item--;
	}
}


/////////////////////////////////////////////////////////////////////////////
// p�epnut� stavu DEF_NAME prvku pod kurzorem

void ProgOnDefName()
{

// kontrola, zda je re�im editace
	if (!ProgMode) return;

// ukon�en� preview
	PreviewStop();

// test, zda je okno stromu
	if ((FocusBuf != BufEdiID) ||
		(EditMode == BufEdiID))
	{

// nalezen� aktivn�ho prvku
		CBufProg* buf = BufProg + FocusBuf;
		int index = buf->Select();
		if (index >= 0)
		{

// adresa prvku
			PROGITEM* item = &((*buf)[index]);

// prvek nesm� b�t uzamknut�
			if (item->Param & (PR_LOCK | PR_LOCK_DEP)) return;

// nelze p�epnout u potomka v okn� t��d
			if ((FocusBuf == BufClsID) && (item->Parent >= 0)) return;

// duplikace textu, je-li implicitn�
			if (item->Name < 0)
			{
				CMultiText txt;
				txt.MultiText(Text.GetTxt(buf->GetText(index)));
				item->Name = Text.Add(txt);
				Undo.AddProgName(FocusBuf, index, -1);
			}

// jinak zru�en� p�vodn�ho textu
			else
			{
				Undo.AddProgName(FocusBuf, index, item->Name);

				Text.Del(item->Name);
				item->Name = -1;
				int oldinx = buf->GetText(index);
				CMultiText txt;
				txt.MultiText(Text.GetTxt(oldinx));

// kontrola, zda je to jm�no objektu
				if (((FocusBuf == BufObjID) || (FocusBuf == BufLocID))
					&& (buf->TestObj(item->Func)))
				{

// p��prava po��te�n� a koncov� polo�ky k hled�n� v lok�ln�m bufferu
					int frst = -1;
					int last = -1;
					if (FocusBuf == BufLocID)
					{
						frst = index;
						while (buf->At(frst).Parent >= 0)
						{
							frst = buf->At(frst).Parent;
						}
						last = buf->At(frst).Next;
					}

// kontrola, zda je jm�no jedine�n�
					if ((BufLoc.FindObj(txt, index, frst, last) >= 0) ||
						(BufObj.FindObj(txt, index, -1, -1) >= 0))
					{

// nalezen� jedine�n�ho jm�na
						int n = 0;
						do
						{
							n++;
							txt.SetNumObj(n);
						} while ((BufLoc.FindObj(txt, index, frst, last) >= 0) ||
								(BufObj.FindObj(txt, index, -1, -1) >= 0));

// nastaven� nov�ho jm�na polo�ky
						item->Name = Text.Add(txt);
					}
				}

// nastaven� textu polo�ky ve stromu
				if (item->HTree)
				{
					TV_ITEM tvi;
					tvi.mask = TVIF_TEXT;
					tvi.hItem = item->HTree;
					tvi.pszText = (LPTSTR)(LPCTSTR)txt.MultiText();
					::SendMessage(buf->Tree(), TVM_SETITEM, 0, (LPARAM)&tvi);
				}

// aktualizace text� v z�visl�ch bufferech
				if (item->Refer)
				{
					CText txt2(txt.MultiText());
					AktNameTree(BufObjID, txt2, buf->BufID(), index);
					AktNameTree(BufLocID, txt2, buf->BufID(), index);
					AktNameTree(BufEdiID, txt2, buf->BufID(), index);
					AktNameTree(BufClsID, txt2, buf->BufID(), index);
				}
			}

// ukon�en� z�znamu UNDO
			Undo.Stop();

// aktualizace p�ep�na�� pro vybranou polo�ku
			ProgAktItem();

// nastaven� p��znaku modifikace
			SetModi();
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// p�ekreslen� okna

void UpdateWnd(HWND wnd)
{
	if (wnd)
	{
		::InvalidateRect(wnd, NULL, FALSE);
		::UpdateWindow(wnd);
	}
}


/////////////////////////////////////////////////////////////////////////////
// p�ekreslen� strom�

void UpdateAllTree()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// aktualizace oken
	UpdateWnd(TreeObj);
	UpdateWnd(TreeLoc);
	UpdateWnd(TreeEdi);
	UpdateWnd(TreeStr);
	UpdateWnd(TreeCls);
}

/////////////////////////////////////////////////////////////////////////////
// p�epnut� stavu DEF_ICON prvku pod kurzorem

void ProgOnDefIcon()
{

// kontrola, zda je re�im editace
	if (!ProgMode) return;

// ukon�en� preview
	PreviewStop();

// test, zda je okno stromu
	if ((FocusBuf != BufEdiID) ||
		(EditMode == BufEdiID))
	{

// nalezen� aktivn�ho prvku
		CBufProg* buf = BufProg + FocusBuf;
		int index = buf->Select();
		if (index >= 0)
		{

// adresa prvku
			PROGITEM* item = &((*buf)[index]);

// prvek nesm� b�t uzamknut� a nesm� to b�t okno t��d ani struktur
			if ((item->Param & (PR_LOCK | PR_LOCK_DEP)) || 
					(FocusBuf == BufClsID) || (FocusBuf == BufStrID)) return;

// duplikace ikony, je-li implicitn�
			if (item->Icon < 0)
			{
				item->Icon = Icon.Dup(buf->GetIcon(index));
				Undo.AddProgIcon(FocusBuf, index, -1);
			}

// jinak zru�en� p�vodn� ikony
			else
			{
				if (item->Icon != 0)
				{
					if (!HistEdit::Del(BufIcoID, item->Icon, -1, -1)) return;

					Undo.AddProgIcon(FocusBuf, index, item->Icon);
				
					Icon.Del(item->Icon);
					item->Icon = -1;

					if (item->DatBlok == BufIcoID)
					{
						item->Icon = Icon.Dup(0);
						item->DatIndex = item->Icon;
					}
				}

// aktualizace ikon ve stromech
				UpdateAllTree();
			}

// ukon�en� z�znamu UNDO
			Undo.Stop();

// aktualizace p�ep�na�� pro vybranou polo�ku
			ProgAktItem();

// nastaven� p��znaku modifikace
			SetModi();
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace p�ep�na�� pro vybranou polo�ku

void ProgAktItem()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// obsluha stromu
	if ((FocusBuf != BufEdiID) ||
		(EditMode == BufEdiID))
	{

// nalezen� aktivn�ho prvku
		CText txt;
		CText txt2;

		CBufProg* buf = BufProg + FocusBuf;
		int index = buf->Select();
		if (index >= 0)
		{
			PROGITEM* item = &((*buf)[index]);

// povolen� tla��tek (z�kaz p�i uzam�en�)
			BOOL enable = ((item->Param & (PR_LOCK | PR_LOCK_DEP)) == 0);
			EnableCommand(IDN_LOCK, ((item->Param & (PR_LOCK_DEP | PR_CD | PR_CD2)) == 0));
			EnableCommand(IDN_OFF, enable);
			EnableCommand(IDN_DEF_NAME, enable && !EditName && (FocusBuf != BufStrID)
					&& (FocusBuf != BufClsID));
			EnableCommand(IDN_EDIT_NAME, enable && (FocusBuf != BufStrID)
					&& ((FocusBuf != BufClsID) || (item->Parent >= 0)));
			EnableCommand(IDN_DEF_ICON, enable && !EditName && (item->Icon != 0) && 
						(FocusBuf != BufClsID) && (FocusBuf != BufStrID));
			EnableCommand(IDN_EDIT_ICON, enable && !EditName &&	
						(FocusBuf != BufClsID) && (FocusBuf != BufStrID));

// zapnut� tla��tek
			CheckCommand(IDN_LOCK, (item->Param & PR_LOCK));
			CheckCommand(IDN_OFF, (item->Param & PR_OFF));
			CheckCommand(IDN_EDIT_NAME, EditName);
			CheckCommand(IDN_DEF_NAME, (item->Name < 0));
			CheckCommand(IDN_EDIT_ICON, (FocusBuf == BufEdiID) && (EditMode == BufIcoID));
			CheckCommand(IDN_DEF_ICON, (item->Icon < 0));

// p��prava stavov�ho textu
			txt.Load(IDN_STAT_REF);
			txt.Add(_T(": "));

			int ref = buf->GetRefer(index);

			txt2.Int(ref);
			txt.Add(txt2);
		}

// tla��tka jsou nedefinovan� - jejich z�kaz
		else
		{
			EnableCommand(IDN_LOCK, FALSE);
			CheckCommand(IDN_LOCK, FALSE);

			EnableCommand(IDN_OFF, FALSE);
			CheckCommand(IDN_OFF, FALSE);

			EnableCommand(IDN_DEF_NAME, FALSE);
			CheckCommand(IDN_DEF_NAME, FALSE);

			EnableCommand(IDN_EDIT_NAME, FALSE);
			CheckCommand(IDN_EDIT_NAME, FALSE);

			EnableCommand(IDN_DEF_ICON, FALSE);
			CheckCommand(IDN_DEF_ICON, FALSE);

			EnableCommand(IDN_EDIT_ICON, FALSE);
			CheckCommand(IDN_EDIT_ICON, FALSE);
		}

// nastaven� stavov�ho textu
		SetStatusText2(txt);
	}

	ProgUpdateClipboard();
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace p��znak� LOCK a OFF jedn� polo�ky (state = TVIS_BOLD, TVIS_CUT)

void UpdateLockItem(CBufProg* buf, PROGITEM* item, int state)
{
// lok�ln� prom�nn�
	int* param;							// adresa parametr� polo�ky
	int oldstate;						// star� stav polo�ky
	int index;							// ukazatel indexu potomk�
	TV_ITEM tvi;						// polo�ka stromu
	
// oprava nov�ho stavu OFF podle deklara�n� polo�ky
	if ((item->RefBlok >= 0) && (item->RefIndex >= 0))
	{
		if (BufProg[item->RefBlok][item->RefIndex].Param & (PR_OFF | PR_OFF_DEP))
		{
			state |= TVIS_CUT;
		}
	}

// p��prava star�ho stavu polo�ky
	oldstate = 0;
	param = &(item->Param);
	if (*param & (PR_LOCK | PR_LOCK_DEP)) oldstate = TVIS_BOLD;
	if (*param & (PR_OFF | PR_OFF_DEP)) oldstate |= TVIS_CUT;

// nastaven� z�visl�ch p��znak�
	*param &= ~(PR_LOCK_DEP | PR_OFF_DEP);
	if (state & TVIS_BOLD) *param |= PR_LOCK_DEP;
	if (state & TVIS_CUT) *param |= PR_OFF_DEP;

// oprava stavu polo�ky
	if (*param & (PR_LOCK | PR_LOCK_DEP)) state |= TVIS_BOLD;
	if (*param & (PR_OFF | PR_OFF_DEP)) state |= TVIS_CUT;

// oprava stavu polo�ky ve stromu
	if ((state != oldstate) && (item->HTree != NULL))
	{
		tvi.mask = TVIF_STATE;
		tvi.hItem = item->HTree;
		tvi.stateMask = TVIS_CUT | TVIS_BOLD;
		tvi.state = state;
		::SendMessage(buf->Tree(), TVM_SETITEM, 0, (LPARAM)&tvi);
	}

// nastaven� parametr� datov� polo�ky
	if (((DWORD)(item->DatBlok) < (DWORD)PROGBUFNUM) && (item->DatIndex >= 0))
	{
		UpdateLockItem(BufProg + item->DatBlok, &(BufProg[item->DatBlok][item->DatIndex]), state);
	}

// nastaven� potomk�
	index = item->Child;
	BOOL iscomment = ((item->Func == IDF_COMMENT) && (item->Parent >= 0));
	while (buf->IsValid(index))
	{
		item = &((*buf)[index]);
		if (iscomment && (item->Func != IDF_COMMENT))
		{
			UpdateLockItem(buf, item, state | TVIS_CUT);
		}
		else
		{
			UpdateLockItem(buf, item, state);
		}
		index = item->Next;
	}
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace p��znak� LOCK a OFF jednoho bufferu

void UpdateLockBuf(int bufID)
{
// lok�ln� prom�nn�
	CBufProg* buf = BufProg + bufID;	// buffer
	int index = buf->First();			// index polo�ky
	PROGITEM* item;						// adresa polo�ky

// cyklus p�es v�echny polo�ky ROOT
	while (buf->IsValid(index))
	{

// adresa polo�ky
		item = &((*buf)[index]);

// aktualizace polo�ky
		UpdateLockItem(buf, item, 0);

// index dal�� polo�ky
		index = item->Next;
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace p��znak� LOCK a OFF buffer� (LOCAL a EDIT jsou aktualizov�ny z�visle)

void UpdateLock()
{
	UpdateLockBuf(BufStrID);
	UpdateLockBuf(BufClsID);
	UpdateLockBuf(BufObjID);

// pro spr�vn� prov�z�n� zopakov�n� n�vaznost�
// (jinak nespr�vn� funkce nap�. je-li glob�ln� prom�nn� ve skupin�
//  a p�ep�n� se vypnut� cel� skupiny -> v editoru se aktualizuje �patn�)

	UpdateLockBuf(BufStrID);
	UpdateLockBuf(BufClsID);
	UpdateLockBuf(BufObjID);
}


/////////////////////////////////////////////////////////////////////////////
// zah�jen� editace ikony prvku

void ProgOnEditIcon()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// ukon�en� preview
	PreviewStop();

// test, zda je okno stromu
	if (((FocusBuf != BufEdiID) ||
		(EditMode == BufEdiID)) &&
		(FocusBuf != BufStrID) &&
		(FocusBuf != BufClsID))
	{
		CBufProg* buf = BufProg + FocusBuf;

		int index = buf->Select();
		if (index >= 0)
		{
			PROGITEM* item = &((*buf)[index]);

			if ((FocusBuf == BufEdiID) && (item->Func == IDF_COLOR))
			{
				ProgCol::SelectColor(index);
				return;
			}

			if ((item->Icon < 0) && 
					((buf->BufID() != BufEdiID) || 
						((item->Func != IDF_ICON) && 
						(item->Func != IDF_NONE))))
			{
				item->Icon = Icon.Dup(buf->GetIcon(index));
				Undo.AddProgIcon(FocusBuf, index, -1);
				ProgAktItem();
				SetModi();
			}

			int icon = buf->GetIcon(index);

			if (Icon.IsValid(icon))
			{
				if (!SetEditMode(BufIcoID, icon)) return;
				BufEdi.Nadpis(Text.GetTxt(buf->GetText(index)));
				FocusBuf = BufEdiID;
				ProgSetFocus();
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// zah�jen�/ukon�en� editace jm�na prvku

void ProgOnEditName()
{

// kontrola, zda je re�im editace
	if (!ProgMode) return;

// ukon�en� preview
	PreviewStop();

// test, zda je okno stromu
	if (((FocusBuf != BufEdiID) ||
		(EditMode == BufEdiID)) &&
		(FocusBuf != BufStrID))
	{
		CBufProg* buf = BufProg + FocusBuf;

// ukon�en� editace
		if (EditName)
		{
			::SendMessage(buf->Tree(), TVM_ENDEDITLABELNOW, FALSE, 0);
		}

// zah�jen� editace
		else
		{
			int index = buf->Select();
			if (index >= 0)
			{
				PROGITEM* item = &((*buf)[index]);
				if (item->HTree)
				{
					::SendMessage(buf->Tree(), TVM_EDITLABEL, 0, (LPARAM)item->HTree);
				}
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// zah�jen� editace jm�na prvku (vrac� TRUE=p�eru�it editaci)

BOOL ProgOnBeginLabelEdit(HWND hWnd, HTREEITEM hItem)
{

// kontrola, zda je re�im editace a zda nen� editace prvku
	if (!ProgMode || EditName) return TRUE;

// ukon�en� preview
	PreviewStop();

// rozli�en� zdrojov�ho okna
	CBufProg* buf = BufProgFromHandle(hWnd);
	if ((buf == NULL) || (buf->Tree() == NULL)) return TRUE;

// nalezen� polo�ky
	int index = buf->Find(hItem);
	if (index < 0) return TRUE;

// na�ten� handle edita�n�ho okna
	EditNameWnd = (HWND)::SendMessage(buf->Tree(), TVM_GETEDITCONTROL, 0, 0);
	if (EditNameWnd == NULL) return TRUE;

// p��znak zah�jen� editace
	EditName = TRUE;

// aktualizace p�ep�na�� pro vybranou polo�ku
	ProgAktItem();

// update voleb clipboardu
	ProgUpdateClipboard();
	EnableCommand(IDN_CUT, TRUE);
	EnableCommand(IDN_COPY, TRUE);

// p��znak povolen� editace
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// ukon�en� editace jm�na prvku

// pomocn� prom�nn� k podr�en� textu pro prvek
CText LabelText;

void ProgOnEndLabelEdit(HWND hWnd, TV_ITEM* tvi)
{

// kontrola, zda je re�im editace a zda je editace prvku
	if (!ProgMode || !EditName) return;

// test, zda je text platn�
	if (tvi->pszText)
	{

// rozli�en� zdrojov�ho okna
		int bufID = BufProgIDFromHandle(hWnd);
		if (bufID >= 0)
		{
			CBufProg* buf = BufProg + bufID;

// nalezen� polo�ky
			int index = buf->Find(tvi->hItem);
			if (index >= 0)
			{
				PROGITEM* item = &((*buf)[index]);

// �schova star�ho jm�na
				int oldindex = buf->GetText(index);
				CMultiText oldtxt;
				if (buf->At(index).Name >= 0)
				{
					oldtxt = Text.Get(oldindex);
				}
				else
				{
					oldtxt.MultiText(Text.GetTxt(oldindex));
				}
				CMultiText newtxt;
				newtxt = oldtxt;

// p��prava nov�ho jm�na
				CText txt(tvi->pszText);
				if (bufID == BufClsID)
				{
					txt.TrimLeft();
					txt.TrimRight();
				}

// kontrola, zda je text zm�n�n .... vy�azeno, aby bylo aktualizov�no v�dy jm�no souboru v knihovn�
//				if (txt != oldtxt.MultiText())
				{

// pr�zdn� text bude nahrazen implicitn�m textem
					if (txt.IsEmpty())
					{
						oldtxt.MultiText(Jazyk, EmptyText);
						txt = oldtxt.MultiText();
						LabelText = txt;
						tvi->pszText = (LPTSTR)(LPCTSTR)LabelText;
					}

// pro buffer t��d �schova star�ho jm�na souboru
					CText oldname;
					CText oldicon;
					CText oldkey;

					if ((bufID == BufClsID) && (item->Parent >= 0))
					{
						ProgLib::InitName(index);
						oldname = ProgLib::m_Name;
						oldicon = ProgLib::m_IconName;
						oldkey = ProgLib::m_NameKey;
					}

// nastaven� nov�ho textu
					newtxt.MultiText(txt);

// vytvo�en� nov�ho textu nebo nastaven� existuj�c�ho
					ASSERT(FocusBuf == bufID);	// pro� je zde najednou pou�ito FocusBuf ??? !!!!!!
					if (item->Name < 0)
					{
						item->Name = Text.Add(newtxt);
						Undo.AddProgName(FocusBuf, index, -1);
					}
					else
					{
						Undo.AddProgNameSet(FocusBuf, index, Text.Get(item->Name));
						Text.Set(item->Name, newtxt);
					}

// v bufferu t��d kontrola, zda jm�no ji� existuje
					if ((bufID == BufClsID) && (item->Parent >= 0))
					{

// p��prava nov�ho jm�na souboru
						CText newname = newtxt.MultiText();
						newname.KorigShort();

						if (item->Func == IDF_GROUP)
						{
							newtxt.MultiText(JAZYK000, newname);
						}
						else
						{
							if (ProgLib::m_AltExt)
							{
								newtxt.MultiText(JAZYK000, newname + _T('.') + ProgLib::m_Ext2);
							}
							else
							{
								newtxt.MultiText(JAZYK000, newname + _T('.') + ProgLib::m_Ext);
							}
						}
						Text.Set(item->Name, newtxt);

						ProgLib::InitName(index);

// alternativn� jm�no
						CText altname;
						if (ProgLib::m_AltExt)
						{
							altname = ProgLib::m_Name.Left(ProgLib::m_Name.Length() - 3) + ProgLib::m_Ext;
						}
						else
						{
							altname = ProgLib::m_Name.Left(ProgLib::m_Name.Length() - 3) + ProgLib::m_Ext2;
						}

// existuje soubor nebo adres�� tohoto jm�na?
						CText oldname2(oldname);
						oldname2.UpperCase();
						CText newname2;

						int nn = 1;
						while (((int)::GetFileAttributes(ProgLib::m_Name) != -1) ||
								(ProgLib::m_Ext2.IsNotEmpty() && ((int)::GetFileAttributes(altname) != -1)))
						{
// je to stejn� jm�no - OK
							newname2 = ProgLib::m_Name;
							newname2.UpperCase();
							if (newname2 == oldname2) break;

//							if (ProgLib::m_Name == oldname) break;

// zv��en� ��ta�e indexu jm�na
							nn++;
							if (nn >= 10000) break;

// sestaven� nov�ho jm�na polo�ky
							CText txt0;

							if (ShortName)
							{
								CText txtint;
								txtint.Int(nn);

								while ((newname.Length() + txtint.Length()) > 8)
								{
									newname.Delete(newname.Length() - 1);
								}
								txt0 = newname + txtint;
							}
							else
							{
								txt0 = newname;
								txt0.SetNumObj(nn);
								newtxt.SetNumObj(nn);
							}

// zm�na textu pro polo�ku
							if (item->Func == IDF_GROUP)
							{
								newtxt.MultiText(JAZYK000, txt0);
							}
							else
							{
								if (ProgLib::m_AltExt)
								{
									newtxt.MultiText(JAZYK000, txt0 + _T('.') + ProgLib::m_Ext2);
								}
								else
								{
									newtxt.MultiText(JAZYK000, txt0 + _T('.') + ProgLib::m_Ext);
								}
							}
							Text.Set(item->Name, newtxt);

							ProgLib::InitName(index);

							if (ProgLib::m_AltExt)
							{
								altname = ProgLib::m_Name.Left(ProgLib::m_Name.Length() - 3) + ProgLib::m_Ext;
							}
							else
							{
								altname = ProgLib::m_Name.Left(ProgLib::m_Name.Length() - 3) + ProgLib::m_Ext2;
							}
						}

// nastaven� nov�ho textu
						LabelText = Text.GetTxt(item->Name);
						tvi->pszText = (LPTSTR)(LPCTSTR)LabelText;

// p�ejmenov�n� souboru nebo adres��e
						if (::MoveFile(oldname, ProgLib::m_Name) && 
							ProgLib::m_IconName.IsNotEmpty())
						{
							::MoveFile(oldicon, ProgLib::m_IconName);
						}

// nastaven� aliases jm�na
						for (int lang = 0; lang < JAZYKNUM; lang++)
						{
							if (lang != JAZYK000)
							{
								if (oldkey != ProgLib::m_NameKey)
								{
									::WritePrivateProfileString(JazykInfo[lang].LangIDini, oldkey, NULL, ProgLib::m_Aliases);
								}

								if ((newtxt.MultiText(lang)).IsEmpty())
								{
									::WritePrivateProfileString(JazykInfo[lang].LangIDini, ProgLib::m_NameKey, NULL, ProgLib::m_Aliases);
								}
								else
								{
									::WritePrivateProfileString(JazykInfo[lang].LangIDini, ProgLib::m_NameKey, newtxt.MultiText(lang), ProgLib::m_Aliases);
								}
							}
						}
						
						Undo.DelAll();

						ProgDirChangeIgnore = 2;
					}

// kontrola, zda je to jm�no objektu
					if (((bufID == BufObjID) || (bufID == BufLocID))
						&& (buf->TestObj(item->Func)))
					{

// odstran�n� mezer ze za��tku a konce jm�na
						txt.TrimLeft();						// odstran�n� mezer z po��tku jm�na
						txt.TrimRight();					// odstran�n� mezer z konce jm�na
						if (txt.IsEmpty() || (txt.Length() != Text.GetTxt(item->Name).Length()))
						{
							if (txt.IsEmpty()) txt = _T('#');	// n�hradn� jm�no
							Text.SetTxt(item->Name, txt);			// nastaven� nov�ho jm�na
							LabelText = Text.GetTxt(item->Name);
							tvi->pszText = (LPTSTR)(LPCTSTR)LabelText;
						}
						newtxt.MultiText(txt);

// p��prava po��te�n� a koncov� polo�ky k hled�n� v lok�ln�m bufferu
						int frst = -1;
						int last = -1;
						if (bufID == BufLocID)
						{
							frst = index;
							while (buf->At(frst).Parent >= 0)
							{
								frst = buf->At(frst).Parent;
							}
							last = buf->At(frst).Next;
						}

// kontrola, zda je jm�no jedine�n�
						if ((BufLoc.FindObj(newtxt, index, frst, last) >= 0) ||
							(BufObj.FindObj(newtxt, index, -1, -1) >= 0))
						{

// nalezen� jedine�n�ho jm�na
							int n = 1;
							do {
								n++;
								newtxt.SetNumObj(n);
							} while ((BufLoc.FindObj(newtxt, index, frst, last) >= 0) ||
									(BufObj.FindObj(newtxt, index, -1, -1) >= 0));

// nastaven� nov�ho jm�na polo�ky
							Text.Set(item->Name, newtxt);

// nastaven� nov�ho textu
							txt = Text.GetTxt(item->Name);
							LabelText = txt;
							tvi->pszText = (LPTSTR)(LPCTSTR)LabelText;
						}
					}

// p�i shod� s implicitn�m jazykem se pou�ije implicitn� text
					oldtxt = Text.Get(item->Name);
					if ((Jazyk != JazykDef) && (oldtxt.MultiText(Jazyk) == oldtxt.MultiText(JazykDef)))
					{
						oldtxt.MultiText(EmptyText);
						Text.Set(item->Name, oldtxt);
					}

// nastaven� p��znaku modifikace
					if (bufID != BufClsID) SetModi();

// aktualizace text� v z�visl�ch bufferech
					if (item->Refer)
					{
						AktNameTree(BufObjID, txt, buf->BufID(), index);
						AktNameTree(BufLocID, txt, buf->BufID(), index);
						AktNameTree(BufEdiID, txt, buf->BufID(), index);
						AktNameTree(BufClsID, txt, buf->BufID(), index);
					}
				}
			}
		}
	}

// p��znak ukon�en� editace
	EditName = FALSE;
	EditNameWnd = NULL;

	Undo.Stop();

// aktualizace p�ep�na�� pro vybranou polo�ku
	ProgAktItem();

// update voleb clipboardu
	ProgUpdateClipboard();
	ProgUpdateInOut();
	ProgUpdateUndoRedo();
}

/////////////////////////////////////////////////////////////////////////////
// obsluha kl�vesy (vrac� TRUE=zru�en� kl�vesy)

BOOL ProgOnKeyDown(HWND hWnd, int key, int data)
{
// lok�ln� prom�nn�
	CBufProg* buf;					// buffer programu
	int index;						// index polo�ky
	PROGITEM* item;					// adresa polo�ky

// kontrola, zda je re�im editace
	if (!ProgMode) return FALSE;

// obsluha okna stromu
	buf = BufProgFromHandle(hWnd);
	if (buf)
	{

// p�ep�n�n� oken TAB
		if (key == VK_TAB)
		{
			if ((::GetKeyState(VK_MENU) >= 0) && (::GetKeyState(VK_CONTROL) >= 0))
			{
				if (::GetKeyState(VK_SHIFT) < 0)
				{
					switch (FocusBuf)
					{
					case BufObjID:	FocusBuf = BufLocID; break;
					case BufEdiID:	FocusBuf = BufObjID; break;
					case BufStrID:	FocusBuf = BufEdiID; break;
					case BufClsID:	FocusBuf = BufStrID; break;
					case BufLocID:	FocusBuf = BufClsID; break;
					}
				}
				else
				{
					switch (FocusBuf)
					{
					case BufObjID:	FocusBuf = BufEdiID; break;
					case BufEdiID:	FocusBuf = BufStrID; break;
					case BufStrID:	FocusBuf = BufClsID; break;
					case BufClsID:	FocusBuf = BufLocID; break;
					case BufLocID:	FocusBuf = BufObjID; break;
					}
				}
				ProgSetFocus();
				return TRUE;
			}
		}

// edita�n� pole
		if (FocusBuf == BufEdiID)
		{
			switch(EditMode)
			{
			case BufLogID:
				if (key == VK_RETURN) 
				{	EditLog::Switch();
					return TRUE;
				}
				break;

			case BufMapID:
				if (EditMap::OnKeyDown(key)) return TRUE;
				break;

			case BufIcoID:
			case BufPicID:
				if (EditIcon::OnKeyDown(key)) return TRUE;
				break;
			}

			if ((key == VK_RETURN) || (key == VK_ESCAPE))
			{
				switch(EditMode)
				{
				case BufNumID:
					EditNum::ReLoad();
					break;
				}
			}
		}

		switch (key)
		{
		case VK_RETURN:

// nalezen� aktivn�ho prvku
			index = buf->Select();
			if (index >= 0)
			{
				item = &((*buf)[index]);

// zah�jen� editace jm�na prvku
				if (!EditName)
				{
					if ((::GetKeyState(VK_MENU) < 0) &&
						((item->Param &  (PR_LOCK | PR_LOCK_DEP)) == 0) &&
						(buf->BufID() != BufStrID) &&
						((buf->BufID() != BufClsID) || (item->Parent >= 0)))
					{
						::PostMessage(buf->Tree(), TVM_EDITLABEL, 0, (LPARAM)item->HTree);
					}
				}
			}
			break;
		}
	}

// kl�vesy p�i editaci jm�na prvku (pozor na opakovan� vno�en�)
	if (EditName)
	{
		buf = BufProg + FocusBuf;

		switch (key)
		{
		case VK_UP:
		case VK_DOWN:
		case VK_PRIOR:
		case VK_NEXT:
			{
				::SendMessage(buf->Tree(), TVM_ENDEDITLABELNOW, FALSE, 0);
				ASSERT(!EditName);
				EditName = FALSE;
				::SendMessage(buf->Tree(), WM_KEYDOWN, key, data);
				index = buf->Select();
				if (index >= 0)
				{
					item = &((*buf)[index]);
					::PostMessage(buf->Tree(), TVM_EDITLABEL, 0, (LPARAM)item->HTree);
				}
			}
			return TRUE;
		}
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// vstup znaku z kl�vesnice (TRUE=obslou�eno)

BOOL ProgOnChar(HWND hWnd, TCHAR znak)
{
// kontrola, zda je re�im editace
	if (!ProgMode) return FALSE;

// rozli�en� podle edita�n�ho m�du
	if (FocusBuf == BufEdiID)
	{
		switch (EditMode)
		{
		case BufNumID:
			return EditNum::OnChar(znak);

		case BufIcoID:
		case BufPicID:
			return EditIcon::OnChar(znak);
		}
	}
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace defini�n�ch ukazatel�

//void AktAllDef()
//{
//
//}

/////////////////////////////////////////////////////////////////////////////
// zobrazen� polo�ky (nap�. po hled�n�)

void DispItem(int bufID, int index)
{
// adresa bufferu, kontrola zad�n�
	if (((DWORD)bufID >= (DWORD)PROGBUFNUM) || (bufID == BufIntID)) return;
	CBufProg* buf = BufProg + bufID;
	if (buf->IsNotValid(index)) return;

// pro EDIT a LOCAL se bude nastavovat prvn� polo�ka
	if ((bufID == BufEdiID) || (bufID == BufLocID))
	{

// nalezen� ROOT polo�ky
		int first = index;
		while (buf->At(first).Parent >= 0)
		{
			first = buf->At(first).Parent;
		}

// buffer EDIT
		if (bufID == BufEdiID)
		{
			first = BufLoc.SrcDat(BufEdiID, first);
			ASSERT(first >= 0);
			if (first < 0) return;

			if (BufLoc[first].Parent >= 0)
			{
				SetEditMode(BufEdiID, BufLoc[first].DatIndex);
			}
			else
			{
				SetEditMode(BufLocID, first);
			}
		}
		else

// buffer LOCAL
		{
			SetEditMode(BufLocID, first);
		}
	}

// p�epnut� na okno
	::SetFocus(buf->Tree());

// p�epnut� na aktivn� polo�ku
	buf->Select(index);
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace polo�ky s po�adovanou referenc�

BOOL AktRef(int bufID, int RefBlok, int RefIndex, BOOL next)
{
// nalezen� polo�ky v bufferu
	int index = BufProg[bufID].SrcRef(RefBlok, RefIndex, next);

// zobrazen� nalezen� polo�ky
	if (index >= 0)
	{
		DispItem(bufID, index);
		return TRUE;
	}
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// skok na pou�it� polo�ky v po�adovan�m sm�ru

void ProgOnRefNextPrev(BOOL next)
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;
	
// aktu�ln� buffer
	int bufID = FocusBuf;
	CBufProg* buf = BufProg + bufID;

// aktu�ln� polo�ka
	int index = buf->Select();
	if (index < 0) return;
	PROGITEM* item = &((*buf)[index]);

// nalezen� polo�ky
	switch (bufID)
	{
	case BufEdiID:
		AktRef(BufEdiID, item->RefBlok, item->RefIndex, next);
		break;

	case BufClsID:
		if (!AktRef(BufLocID, bufID, index, next))
		{
			AktRef(BufObjID, bufID, index, next);
		}
		break;

	default:
		AktRef(BufEdiID, bufID, index, next);
	}
}


/////////////////////////////////////////////////////////////////////////////
// n�sleduj�c� deklarace

void ProgOnRefNext()
{
	ProgOnRefNextPrev(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// p�ede�l� deklarace

void ProgOnRefPrev()
{
	ProgOnRefNextPrev(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// definice deklarace

void ProgOnRefDef()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;
	
// aktu�ln� buffer
	int bufID = FocusBuf;
	CBufProg* buf = BufProg + bufID;

// aktu�ln� polo�ka
	int index = buf->Select();
	if (index < 0) return;
	PROGITEM* item = &((*buf)[index]);

// skok na deklara�n� polo�ku
	DispItem(item->RefBlok, item->RefIndex);
}


/////////////////////////////////////////////////////////////////////////////
// obsluha dialogu hled�n� textu

BOOL CALLBACK FindDialogProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
	case WM_INITDIALOG:
		{
			CenterWindow(hWnd, MainFrame);
			CText txt;

			txt.Load(IDN_FIND_DIALOG);
			txt.SetWindowTextFont(hWnd);

			DialogCustomText(hWnd, IDN_FINDTEXT);

			txt.Load(IDN_CANCEL0);
			txt.SetDialogTextFont(hWnd, IDCANCEL);

			FindStringEdit = ::GetDlgItem(hWnd, IDN_FIND_EDIT);
			FindString.SetWindowTextFont(FindStringEdit);
		}
		return TRUE;

	case WM_DRAWITEM:				// 0x02B
		if (DialogDraw(hWnd, lParam)) return TRUE;
		return FALSE;

	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
		case 0:
		case 1:
			switch (LOWORD(wParam))
			{
			case IDOK:
			case IDCANCEL:
				FindString.GetWindowText(FindStringEdit);
				::EndDialog(hWnd, LOWORD(wParam));
				return TRUE;
			}
			break;
		}
		break;

	}
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// hled�n� textu

void ProgOnFind()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// vymaz�n� text�
//	SetStatusText(EmptyText);
	SetStatusText2(EmptyText);

// zad�n� textu k hled�n�
	if (::DialogBoxParam(
				hInstance,
				MAKEINTRESOURCE(IDN_FIND_DIALOG),
				MainFrame,
				(DLGPROC)FindDialogProc,
				(LPARAM)IDN_FIND_DIALOG) != IDOK) return;

// nalezen� dal��ho v�skytu textu
	ProgOnFindNext();
}

void ProgOnFindNextPrev(BOOL next)
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// p��prava textu k hled�n�
	CText txt(FindString);
	txt.UpperCase();
	if (txt.IsEmpty()) return;
	CText name;
	int nameN;

// p��prava aktivn�ho bufferu
	int bufID = FocusBuf;
	CBufProg* buf = BufProg + bufID;

// p��prava po��te�n� polo�ky
	int index = buf->Select();
	PROGITEM* item;

// cyklus p�es v�echny buffery
	for (int i = 5+1; i > 0; i--)
	{

// �schova prvn� polo�ky
		int first = buf->First();
		if (first >= 0)
		{

// cyklus p�es v�echny polo�ky
			for (;;)
			{

// index dal�� polo�ky
				if (next)
				{
					if (index < 0)
					{
						index = buf->NextItem(index);
					}
					else
					{
						index = buf->NextItem(index);
						if (index == first) break;
					}
				}
				else
				{
					if (index == first) break;
					index = buf->PrevItem(index);
				}

// adresa polo�ky
				item = &((*buf)[index]);

// nekontroluje se ROOT polo�ka LOCAL a EDIT
				if (((bufID != BufEdiID) && (bufID != BufLocID)) ||
					(item->Parent >= 0))
				{

// test textu v polo�ce
					nameN = buf->GetText(index);
					if (nameN >= 0)
					{
						name = Text.GetTxt(nameN);
						name.UpperCase();
						if (name.Find(txt) >= 0)
						{
							DispItem(bufID, index);
							return;
						}
					}
				}
			}
		}

// index dal��ho bufferu
		if (next)
		{
			switch (bufID)
			{
			case BufEdiID: bufID = BufStrID; break;
			case BufStrID: bufID = BufClsID; break;
			case BufClsID: bufID = BufLocID; break;
			case BufLocID: bufID = BufObjID; break;
			case BufObjID: bufID = BufEdiID; break;
			}
		}
		else
		{
			switch (bufID)
			{
			case BufEdiID: bufID = BufObjID; break;
			case BufObjID: bufID = BufLocID; break;
			case BufLocID: bufID = BufClsID; break;
			case BufClsID: bufID = BufStrID; break;
			case BufStrID: bufID = BufEdiID; break;
			}
		}
		buf = BufProg + bufID;
		index = -1;
	}
}

void ProgOnFindNext()
{
	ProgOnFindNextPrev(TRUE);
}

void ProgOnFindPrev()
{
	ProgOnFindNextPrev(FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// horizont�ln� posuvn�k edita�n�ho pole

void ProgOnHScroll(int code, int pos)
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// na�ten� pozice pro jin� k�dy ne� ta�en�
	if ((code != SB_THUMBPOSITION) && (code != SB_THUMBTRACK))
	{
		pos = ::GetScrollPos(HScroll, SB_CTL);
	}

// rozli�en� podle edita�n�ho m�du
	switch (EditMode)
	{
	case BufMapID:
		EditMap::OnHScroll(code, pos);
		break;

	case BufSprID:
		EditSprite::OnHScroll(code, pos);
		break;

	case BufIcoID:
	case BufPicID:
		EditIcon::OnHScroll(code, pos);
		break;
	}
}


/////////////////////////////////////////////////////////////////////////////
// vertik�ln� posuvn�k edita�n�ho pole

void ProgOnVScroll(int code, int pos)
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// na�ten� pozice pro jin� k�dy ne� ta�en�
	if ((code != SB_THUMBPOSITION) && (code != SB_THUMBTRACK))
	{
		pos = ::GetScrollPos(VScroll, SB_CTL);
	}

// rozli�en� podle edita�n�ho m�du
	switch (EditMode)
	{
	case BufMapID:
		EditMap::OnVScroll(code, pos);
		break;

	case BufSprID:
		EditSprite::OnVScroll(code, pos);
		break;

	case BufIcoID:
	case BufPicID:
		EditIcon::OnVScroll(code, pos);
		break;
	}
}


/////////////////////////////////////////////////////////////////////////////
// v�b�r v�eho

void ProgOnAll()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// editace jm�na prvku
	if (EditName)
	{
		::SendMessage(EditNameWnd, EM_SETSEL, 0, (LPARAM)-1);
	}

// rozli�en� podle edita�n�ho m�du
	if (FocusBuf == BufEdiID)
	{
		switch (EditMode)
		{
		case BufNumID:
			EditNum::SelectAll();
			break;

		case BufMapID:
			EditMap::SelectAll();
			break;

		case BufTxtID:
			EditText::SelectAll();
			break;

		case BufIcoID:
		case BufPicID:
			EditIcon::SelectAll();
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// zru�en� bloku

void ProgOnDelete()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// editace jm�na prvku
	if (EditName)
	{
		long int start, end;
		::SendMessage(EditNameWnd, EM_GETSEL, (WPARAM)&start, (WPARAM)&end);

		if (start == end)
		{
			::SendMessage(EditNameWnd, WM_KEYDOWN,	VK_DELETE, 0x01530001);
			::SendMessage(EditNameWnd, WM_KEYUP,	VK_DELETE, 0xC1530001);
		}
		else
		{
			::SendMessage(EditNameWnd, WM_CLEAR, 0, 0);
		}
		return;
	}

// rozli�en� podle edita�n�ho m�du
	if (FocusBuf == BufEdiID)
	{
		switch (EditMode)
		{
		case BufEdiID:
			ProgClip::Delete(BufEdiID);
			break;

		case BufNumID:
			EditNum::Delete();
			break;

		case BufMapID:
			EditMap::Delete();
			break;

		case BufTxtID:
			EditText::Delete();
			break;

		case BufIcoID:
		case BufPicID:
			EditIcon::Delete();
			break;
		}
	}
	else
	{
		ProgClip::Delete(FocusBuf);
	}
}

/////////////////////////////////////////////////////////////////////////////
// kopie bloku

void ProgOnCopy()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// editace jm�na prvku
	if (EditName)
	{
		::SendMessage(EditNameWnd, WM_COPY, 0, 0);
		return;
	}

// rozli�en� podle edita�n�ho m�du
	if (FocusBuf == BufEdiID)
	{
		switch (EditMode)
		{
		case BufEdiID:
			ProgClip::Copy(BufEdiID);
			break;

		case BufNumID:
			EditNum::Copy();
			break;

		case BufMapID:
			EditMap::Copy();
			break;

		case BufSndID:
			EditSound::Copy();
			break;

		case BufMusID:
			EditMusic::Copy();
			break;

		case BufTxtID:
			EditText::Copy();
			break;

		case BufIcoID:
		case BufPicID:
			EditIcon::Copy();
			break;
		}
	}
	else
	{
		ProgClip::Copy(FocusBuf);
	}
}

/////////////////////////////////////////////////////////////////////////////
// vyst�i�en� bloku

void ProgOnCut()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// editace jm�na prvku
	if (EditName)
	{
		::SendMessage(EditNameWnd, WM_CUT, 0, 0);
		return;
	}

// rozli�en� podle edita�n�ho m�du
	if (FocusBuf == BufEdiID)
	{
		switch (EditMode)
		{
		case BufEdiID:
			ProgClip::Cut(BufEdiID);
			break;

		case BufNumID:
			EditNum::Cut();
			break;

		case BufMapID:
			EditMap::Cut();
			break;

		case BufTxtID:
			EditText::Cut();
			break;

		case BufIcoID:
		case BufPicID:
			EditIcon::Cut();
			break;
		}
	}
	else
	{
		ProgClip::Cut(FocusBuf);
	}
}

/////////////////////////////////////////////////////////////////////////////
// vlo�en� bloku

void ProgOnPaste()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// editace jm�na prvku
	if (EditName)
	{
		::SendMessage(EditNameWnd, WM_PASTE, 0, 0);
		return;
	}

// rozli�en� podle edita�n�ho m�du
	if (FocusBuf == BufEdiID)
	{
		switch (EditMode)
		{
		case BufEdiID:
			ProgClip::Paste(BufEdiID);
			break;

		case BufNumID:
			EditNum::Paste();
			break;

		case BufMapID:
			EditMap::Paste();
			break;

		case BufSndID:
			EditSound::Paste();
			break;

		case BufMusID:
			EditMusic::Paste();
			break;

		case BufTxtID:
			EditText::Paste();
			break;

		case BufIcoID:
		case BufPicID:
			EditIcon::Paste();
			break;
		}
	}
	else
	{
		ProgClip::Paste(FocusBuf);
	}
}

/////////////////////////////////////////////////////////////////////////////
// UNDO

void ProgOnUndo()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// editace jm�na prvku
	if (EditName)
	{
		::SendMessage(EditNameWnd, EM_UNDO, 0, 0);
		return;
	}

// editace ��sla plochy
	if (EditMapNumWnd)
	{
		::SendMessage(EditMapNumWnd, EM_UNDO, 0, 0);
		return;
	}

	Undo.Undo();
}

/////////////////////////////////////////////////////////////////////////////
// REDO

void ProgOnRedo()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// editace jm�na prvku
	if (EditName)
	{
		::SendMessage(EditNameWnd, EM_UNDO, 0, 0);
		return;
	}

// editace ��sla plochy
	if (EditMapNumWnd)
	{
		::SendMessage(EditMapNumWnd, EM_UNDO, 0, 0);
		return;
	}

	Undo.Redo();
}

/////////////////////////////////////////////////////////////////////////////
// nulov�n� p��znak� na�ten� obr�zk� pro preview

void PreviewPicNul()
{
 	for (int k = PreviewPicSize - 1; k >= 0; k--)
	{
		PreviewPicLoad[k] = FALSE;
	}
}

/////////////////////////////////////////////////////////////////////////////
// test zm�ny adres��e

void TestDirChange(HANDLE* dir1, HANDLE* dir2, CText& path1, CText& path2, int idf)
{
	HANDLE olddir1 = *dir1;
	HANDLE olddir2 = *dir2;

// zah�jen� sledov�n�
//	BeginDirChange(dir1, dir2, path1, path2);

// b�n� adres��
	if (*dir1 != INVALID_HANDLE_VALUE)
	{
		while ((olddir1 == INVALID_HANDLE_VALUE) || (::WaitForSingleObject(*dir1, 0) == WAIT_OBJECT_0))
		{
			if (ProgDirChangeIgnore <= 0)
			{
				int i = BufCls.First();
				while (BufCls.IsValid(i))
				{
					if (BufCls[i].Func == idf)
					{
						while (BufCls.IsValid(BufCls[i].Child))
						{
							BufCls.Del(BufCls[i].Child);
						}

						BufCls[i].Param &= ~PR_EXP;

						if (BufCls[i].HTree != NULL)
						{
							TV_ITEM tvi;
							tvi.hItem = BufCls[i].HTree;
							tvi.mask = TVIF_CHILDREN | TVIF_STATE;
							tvi.cChildren = 1;
							tvi.stateMask = TVIS_EXPANDED;
							tvi.state = 0;
							::SendMessage(BufCls.Tree(), TVM_SETITEM, 0, (LPARAM)&tvi);
						}

						if (idf == IDF_PIC)
						{
							PreviewPicNul();
						}

						Undo.DelAll();
						break;
					}
					i = BufCls[i].Next;
				}
			}

			if (olddir1 != INVALID_HANDLE_VALUE)
			{
				::FindNextChangeNotification(*dir1);
			}
			olddir1 = *dir1;
		}
	}

// alternativn� adres��
	if (*dir2 != INVALID_HANDLE_VALUE)
	{
		while ((olddir2 == INVALID_HANDLE_VALUE) || (::WaitForSingleObject(*dir2, 0) == WAIT_OBJECT_0))
		{
			if (ProgDirChangeIgnore <= 0)
			{
				int i = BufCls.First();
				while (BufCls.IsValid(i))
				{
					if (BufCls[i].Func == idf)
					{
						i = BufCls[i].Child;
						while (BufCls.IsValid(i))
						{
							if ((BufCls[i].Func == IDF_GROUP) && (BufCls[i].Param & (PR_CD | PR_CD2)))
							{
								while (BufCls.IsValid(BufCls[i].Child))
								{
									BufCls.Del(BufCls[i].Child);
								}

								BufCls[i].Param &= ~PR_EXP;

								if (BufCls[i].HTree != NULL)
								{
									TV_ITEM tvi;
									tvi.hItem = BufCls[i].HTree;
									tvi.mask = TVIF_CHILDREN | TVIF_STATE;
									tvi.cChildren = 1;
									tvi.stateMask = TVIS_EXPANDED;
									tvi.state = 0;
									::SendMessage(BufCls.Tree(), TVM_SETITEM, 0, (LPARAM)&tvi);
								}

								if (idf == IDF_PIC)
								{
									PreviewPicNul();
								}

								Undo.DelAll();
								break;
							}

							i = BufCls[i].Next;
						}
						break;
					}
					i = BufCls[i].Next;
				}
			}

			if (olddir2 != INVALID_HANDLE_VALUE)
			{
				::FindNextChangeNotification(*dir2);
			}
			olddir2 = *dir2;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// obsluha �asova�e (TRUE=obslou�eno)

BOOL ProgOnTimer(UINT timerID)
{
// kontrola, zda je re�im editace
	if (!ProgMode) return FALSE;

// nastaven� automatick�ho fokusu
	if (timerID == FocusTimerID)
	{
		if (FocusTimerN > 0)
		{
			if (LMouseDown || RMouseDown)
			{
				if (FocusTimerN < 2) FocusTimerN = 2;
			}
			else
			{
				FocusTimerN--;
				if (FocusTimerN == 0)
				{
					if (FocusEditModeBuf >= 0)
					{
						SetEditMode(FocusEditModeBuf, FocusEditModeInx);
						FocusEditModeBuf = -1;
					}

					FocusBuf = FocusNextBuf;
					ProgSetFocus();
				}
			}
			return TRUE;
		}
	}

// hl�en� o zm�n� adres��e
	if (timerID == ProgDirChangeTimerID)
	{
		TestDirChange(&FunctionDirChange, &FunctionDirChange2, FunctionPath, FunctionPath2, IDF_FNC);
		TestDirChange(&NumberDirChange, &NumberDirChange2, NumberPath, NumberPath2, IDF_REAL);
		TestDirChange(&TextDirChange, &TextDirChange2, TextPath, TextPath2, IDF_TEXT);
		TestDirChange(&BoolDirChange, &BoolDirChange2, BoolPath, BoolPath2, IDF_BOOL);
		TestDirChange(&IconDirChange, &IconDirChange2, IconPath, IconPath2, IDF_ICON);
		TestDirChange(&MapDirChange, &MapDirChange2, MapPath, MapPath2, IDF_MAP);
		TestDirChange(&PictureDirChange, &PictureDirChange2, PicturePath, PicturePath2, IDF_PIC);
		TestDirChange(&SpriteDirChange, &SpriteDirChange2, SpritePath, SpritePath2, IDF_SPRITE);
		TestDirChange(&SoundDirChange, &SoundDirChange2, SoundPath, SoundPath2, IDF_SND);
		TestDirChange(&MusicDirChange, &MusicDirChange2, MusicPath, MusicPath2, IDF_MUS);

		if (ProgDirChangeIgnore > 0)
		{
			ProgDirChangeIgnore--;
		}
		return TRUE;
	}

// zah�jen� preview
	if (timerID == PreviewTimerID)
	{
		PreviewTimerN--;
		if (PreviewTimerN == 0)
		{
			PreviewStart();
		}
	}

// rozli�en� podle edita�n�ho m�du
	switch (EditMode)
	{
//	case BufIcoID:
//	case BufPicID:
//		return EditIcon::OnTimer(timerID);
	case BufSndID:
		return EditSound::OnTimer(timerID);

	case BufMusID:
		return EditMusic::OnTimer(timerID);
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� fokusu na okno editoru s opo�d�n�m

void SetFocusEdit()
{
	FocusNextBuf = BufEdiID;

	FocusEditModeBuf = -1;

	FocusTimerN = 2;
	if (FocusTimer == 0)
	{
		FocusTimer = ::SetTimer(MainFrame, FocusTimerID, TimerConst, NULL);
	}
}

void SetFocusEditMode(int buf, int inx)
{
	FocusNextBuf = BufEdiID;

	FocusEditModeBuf = buf;
	FocusEditModeInx = inx;

	FocusTimerN = 3;
	if (FocusTimer == 0)
	{
		FocusTimer = ::SetTimer(MainFrame, FocusTimerID, TimerConst, NULL);
	}
}

/////////////////////////////////////////////////////////////////////////////
// zobrazen� sou�adnic grafick�ho editoru

void DispMouseXY()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

	if ((FocusBuf != BufEdiID) || 
		((EditMode != BufIcoID) && 
		(EditMode != BufPicID) && 
		(EditMode != BufMapID) &&
		(EditMode != BufSprID)))
		return;

	CText txt;
	CText txt2;
	int i;

	if ((MouseX != MOUSEINV) && (MouseY != MOUSEINV))
	{
		if ((EditMode == BufPicID) && !EditIcon::NewDimPoint)
		{
			txt = _T(" X=");
			txt2.Double((double)Round((double)MouseX/32*100 + 0.5)/100);
			txt2 += _T(',');
			for (i = Round(1.75*(9-txt2.Length())); i > 0; i--) txt2 += _T(' ');
			txt.Add(txt2);
			txt.Add(_T("Y="));
			txt2.Double((double)Round((double)MouseY/32*100 + 0.5)/100);
			txt.Add(txt2);
		}
		else
		{
			txt = _T(" X=");
			txt2.Int(MouseX);
			txt2 += _T(',');
			for (i = Round(1.7*(9-txt2.Length())); i > 0; i--) txt2 += _T(' ');
			txt.Add(txt2);
			txt.Add(_T("Y="));
			txt2.Int(MouseY);
			txt.Add(txt2);
		}
	}

	SetStatusText2(txt);
}


/////////////////////////////////////////////////////////////////////////////
// vykreslen� prvku (vrac� TRUE=obslou�eno)

BOOL ProgOnDrawItem(DRAWITEMSTRUCT* di)
{
// kontrola, zda je re�im editace
	if (!ProgMode) return FALSE;

// p��prava lok�ln�ch prom�nn�ch
	HWND wnd = di->hwndItem;
	HDC dc = di->hDC;
	RECT* rc = &di->rcItem;
	int index = di->itemID;
	int* tab = NULL;
	DWORD state = di->itemState;

// nalezen� tabulky comboboxu
	if ((wnd == ToolEditIcon) && ((DWORD)index < (DWORD)30))
	{
		tab = ToolBarEditPicTools;
	}

	if ((wnd == ToolEditIconWidth) && ((DWORD)index < (DWORD)30))
	{
		tab = ToolBarEditPicWidths;
	}

	if ((wnd == ToolEditMap) && ((DWORD)index < (DWORD)30))
	{
		tab = ToolBarEditMapTools;
	}

// obsluha COMBO boxu
	if (di->CtlType == ODT_COMBOBOX)
	{

// vymaz�n� podkladu
		::FillRect(dc, rc, (HBRUSH)::GetStockObject(LTGRAY_BRUSH));

// kontrola, zda je to platn� prvek
		if (tab)
		{

// vytvo�en� kompatibiln�ho DC
			HDC dc2 = ::CreateCompatibleDC(dc);

// v�b�r bitmapy toolbaru
			::SelectObject(dc2, ToolBarBmp);

// vykreslen� ikony
			::BitBlt(dc, (rc->left+rc->right)/2-8, 
				(rc->top+rc->bottom)/2-8, 16, 16, dc2, 16*tab[index*2], 0, SRCCOPY);

// zru�en� DC
			::DeleteDC(dc2);

// vykreslen� r�me�ku kolem vybran�ho prvku
			if ((state & ODS_SELECTED) && ((state & ODS_COMBOBOXEDIT) == 0))
			{
				::DrawEdge(dc, rc, EDGE_RAISED, BF_RECT);

// zobrazen� n�pov�dy k vybran�mu prvku
				CText txt;
				txt.Load(di->itemData);
				if (txt.IsNotEmpty())
				{
					txt.Delete(txt.Find(_T('\n')));
					SetStatusText(txt);
				}
			}
		}
		return TRUE;
	}
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� rozm�r� obr�zku/plochy

void ProgOnDimen()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// rozli�en� podle edita�n�ho m�du
	switch (EditMode)
	{
	case BufMapID:
		EditMap::OnDimen();
		break;

	case BufSprID:
		EditSprite::OnDimen();
		break;

	case BufIcoID:
	case BufPicID:
		EditIcon::OnDimen();
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// zapnut�/vypnut� rastru

void ProgOnRastr()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// rozli�en� podle edita�n�ho m�du
	switch (EditMode)
	{
	case BufMapID:
		EditMap::OnRastr();
		break;

	case BufIcoID:
	case BufPicID:
		EditIcon::OnRastr();
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// start p�ehr�v�n�

void ProgOnPlay()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// ukon�en� preview
	PreviewStop();

// rozli�en� podle edita�n�ho m�du
	switch (EditMode)
	{
	case BufSndID:
		EditSound::OnPlay();
		break;

	case BufMusID:
		EditMusic::OnPlay();
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// zastaven� nahr�v�n� i p�ehr�v�n�

void ProgOnStop()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// rozli�en� podle edita�n�ho m�du
	switch (EditMode)
	{
	case BufSndID:
		EditSound::OnStop();
		break;

	case BufMusID:
		EditMusic::OnStop();
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// p�epnut� p��znaku opakov�n�

void ProgOnLoop()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// rozli�en� podle edita�n�ho m�du
	switch (EditMode)
	{
	case BufSndID:
		EditSound::OnLoop();
		break;

	case BufMusID:
		EditMusic::OnLoop();
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// start nahr�v�n�

void ProgOnRecord()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// ukon�en� preview
	PreviewStop();

// rozli�en� podle edita�n�ho m�du
	switch (EditMode)
	{
	case BufSndID:
		EditSound::OnRecord();
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// pozastaven� nahr�v�n�, p�ehr�v�n�

void ProgOnPause()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// ukon�en� preview
	PreviewStop();

// rozli�en� podle edita�n�ho m�du
	switch (EditMode)
	{
	case BufSndID:
		EditSound::OnPause();
		break;

	case BufMusID:
		EditMusic::OnPause();
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// p�ev�jen� zp�t

void ProgOnRew()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// rozli�en� podle edita�n�ho m�du
	switch (EditMode)
	{
	case BufMusID:
		EditMusic::OnRew();
		break;
	}
}


/////////////////////////////////////////////////////////////////////////////
// p�ev�jen� vp�ed

void ProgOnFrw()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// rozli�en� podle edita�n�ho m�du
	switch (EditMode)
	{
	case BufMusID:
		EditMusic::OnFrw();
		break;
	}
}


/////////////////////////////////////////////////////////////////////////////
// start programu

void ProgOnStart()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// ukon�en� preview
	PreviewStop();

// ulo�en� programu
	if (Modi) ProgFile::SaveMaxi();
	if (Modi) return;

// nastaven� k�du jazyku do registr� Windows
	SetJazykReg(LangID);

// spu�t�n� programu
	Exec(CText(_T('"')) + JmenoLoad + _T('"'), ProgramPath + Cesta, FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// test indexu, zda to je obr�zek v bufferu knihovny (vrac� TRUE=je ok, v knihovn� je jm�no)

BOOL PreviewPicTest(int index)
{
// test, zda to je platn� index bufferu
	if (BufCls.IsValid(index))
	{

// adresa polo�ky
		PROGITEM* item = &BufCls[index];

// test, zda to nen� ROOT polo�ka a zda to je soubor
		if ((item->Parent >= 0) && (item->Func != IDF_GROUP))
		{

// p��prava jm�na souboru
			ProgLib::InitName(index);

// test, zda to je obr�zek
			if ((ProgLib::m_Ext == _T("BMP")) || 
				(ProgLib::m_Ext == _T("JPG")))
			{
				return TRUE;
			}
		}
	}

// nen� to platn� obr�zek
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace cachov�n� obr�zku (vrac� TRUE=pod kurzorem je obr�zek)

BOOL PreviewPicCache()
{
// test, zda je aktivn� okno t��d a zda nen� editace jm�na polo�ky
	if ((FocusBuf == BufClsID) && !EditName)
	{

// index aktivn� polo�ky
		int index = BufCls.Select();

// test indexu, zda to je obr�zek
		if (PreviewPicTest(index))
		{

// �schova aktu�ln�ho stavu polo�ek cachov�n�, vynulov�n� polo�ek
			CPicture	pic[PreviewPicSize];			// obr�zky pro cachov�n�
			CText		name[PreviewPicSize];			// jm�na obr�zk� pro cachov�n� (pr�zdn� = nen�)
			BOOL		load[PreviewPicSize];			// p��znaky na��t�n� obr�zk� (TRUE = byl na��t�n)

			for (int i = PreviewPicSize - 1; i >= 0; i--)
			{
				pic[i] = PreviewPicPic[i];
				name[i] = PreviewPicName[i];
				load[i] = PreviewPicLoad[i];

				PreviewPicPic[i].Empty();
				PreviewPicName[i].Empty();
				PreviewPicLoad[i] = FALSE;
			}

// indexy polo�ek k na�ten�
			int inx[PreviewPicSize];
			inx[PreviewPicAkt] = index;
			inx[PreviewPicNxt] = BufCls.NextItem(index);
			inx[PreviewPicPre] = BufCls.PrevItem(index);

// na�ten� polo�ek
			for (i = PreviewPicSize - 1; i >= 0; i--)
			{
				if (PreviewPicTest(inx[i]))
				{
					PreviewPicName[i] = ProgLib::m_Name;

					for (int j = PreviewPicSize - 1; j >= 0; j--)
					{
						if (name[j] == ProgLib::m_Name)
						{
							if (i == PreviewPicAkt)
							{
								if (j == PreviewPicNxt)	PreviewPicVpred = TRUE;
								if (j == PreviewPicPre)	PreviewPicVpred = FALSE;
							}

							PreviewPicPic[i] = pic[j];
							PreviewPicLoad[i] = load[j];
							break;
						}
					}
				}
			}

			return TRUE;
		}
	}

// pod kurzorem nen� obr�zek
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// zapnut� re�imu preview obr�zku

void PreviewPicOn()
{
// test, zda je re�im preview obr�zku ji� zapnut
	if (PreviewPic) return;

// p��znak zapnut� re�imu preview obr�zku
	PreviewPic = TRUE;

// vypnut� posuvn�k�
	if (HScrollDisp) ::ShowWindow(HScroll, SW_HIDE);
	if (VScrollDisp) ::ShowWindow(VScroll, SW_HIDE);

// vypnut� editac�
	EditSound::OnStop();
	EditMusic::OnStop();
	EditMap::EndEditSwcNum();
	HideEditWnd(-1);

// p�ekreslen� okna
	ProgOnSize();
}

/////////////////////////////////////////////////////////////////////////////
// vypnut� re�imu preview obr�zku

void PreviewPicOff()
{
// test, zda je re�imu preview obr�zku ji� vypnut
	if (!PreviewPic) return;

// p��znak vypnut� re�imu preview obr�zku
	PreviewPic = FALSE;

// zapnut� posuvn�k�
	if (HScrollDisp) ::ShowWindow(HScroll, SW_SHOW);
	if (VScrollDisp) ::ShowWindow(VScroll, SW_SHOW);

// zapnut� editac�
	switch (EditMode)
	{
	case BufEdiID:
		::ShowWindow(TreeEdi, SW_SHOW);
		break;

	case BufNumID:
		::ShowWindow(EditNumWnd, SW_SHOW);
		break;

	case BufLogID:
		::ShowWindow(EditLogWnd, SW_SHOW);
		break;

	case BufTxtID:
		::ShowWindow(EditTextWnd, SW_SHOW);
		break;

	case BufSndID:
		::ShowWindow(EditSound::DispWnd, SW_SHOW);
		break;

	case BufMusID:
		::ShowWindow(EditMusic::DispWnd, SW_SHOW);
		break;
	}

// p�ekreslen� okna
	ProgOnSize();
}


/////////////////////////////////////////////////////////////////////////////
// zastaven� preview, zah�jen� nov�ho ��t�n� �asu

void PreviewStop()
{
// ukon�en� prob�haj�c�ho preview
	if (PreviewWav)
	{
		::PlaySound(NULL, NULL, 0);
		PreviewWav = FALSE;
	}

	if (IsPreviewTempName)
	{
		::DeleteFile(PreviewTempName);
		IsPreviewTempName = FALSE;
	}

	if (PreviewMid)
	{
		::mciSendCommand(PreviewMid, MCI_STOP, 0, NULL);
		::mciSendCommand(PreviewMid, MCI_CLOSE, 0, NULL);
		PreviewMid = 0;
	}

// test, zda je nov� preview povoleno
	if ((FocusBuf == BufClsID) && !EditName)
	{

// vytvo�en� �asova�e
		if (PreviewTimer == 0)
		{
			PreviewTimer = ::SetTimer(MainFrame, PreviewTimerID, TimerConst, NULL);
		}
		PreviewTimerN = 3;

// zobrazen� preview obr�zku (je-li ji� na�ten)
		if (PreviewPicCache())
		{
			if (PreviewPicLoad[PreviewPicAkt])
			{
				if (PreviewPic)
				{
					ProgOnSize();
				}
				else
				{
					PreviewPicOn();
				}
				PreviewTimerN = 1;
			}
		}
	}

// nen� okno t��d - ukon�en� �asova�e
	else
	{
		PreviewPicOff();

		if (PreviewTimer)
		{
			::KillTimer(MainFrame, PreviewTimer);
			PreviewTimer = 0;
		}
		PreviewTimerN = -1;
	}
}

// tabulka testovan�ch jmen pro opakov�n� (mal� p�smena!)
LPCTSTR TestRepeatTab[] = 
{
	NULL,
	NULL,
	_T("(opakov�n�)"),
	_T("(opakovan�)"),
	_T("(repeat)"),
	_T("(repeate)"),
	_T("(repeated)"),
	_T("(repeating)"),
	_T("(loop)"),
	NULL
};

// test p��znaku opakov�n� ve jm�n� zvuku
BOOL TestRepeat(CText name)
{
	name.LowerCase();

	CText txt1;
	txt1.Load(IDN_OPAKOVANI);
	txt1 = _T('(') + txt1 + _T(')');
	TestRepeatTab[0] = txt1;

	CText txt2;
	txt2.Load(IDN_OPAKOVANE);
	txt2 = _T('(') + txt2 + _T(')');
	TestRepeatTab[1] = txt2;

	CText txt;

	LPCTSTR* tab = TestRepeatTab;
	while (*tab != NULL)
	{
		txt = *tab;
		if (name.Find(txt) >= 0) return true;
		tab++;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// zah�jen� preview p�ehr�v�n� souboru pod kurzorem

void PreviewStart()
{
// nulov�n� ��ta�e proti op�tovn�mu vyvol�n�
	PreviewTimerN = -1;

// zastaven� prob�haj�c�ho p�ehr�v�n�
	if (PreviewWav)
	{
		::PlaySound(NULL, NULL, 0);
		PreviewWav = FALSE;
	}

	if (IsPreviewTempName)
	{
		::DeleteFile(PreviewTempName);
		IsPreviewTempName = FALSE;
	}

	if (PreviewMid)
	{
		::mciSendCommand(PreviewMid, MCI_STOP, 0, NULL);
		::mciSendCommand(PreviewMid, MCI_CLOSE, 0, NULL);
		PreviewMid = 0;
	}

// test, zda je aktivn� okno t��d a zda nen� editace jm�na polo�ky
	if ((FocusBuf == BufClsID) && !EditName)
	{

// zji�t�n� aktivn� polo�ky
		int index = BufCls.Select();
		if (index >= 0)
		{

// adresa polo�ky
			PROGITEM* item = &BufCls[index];

// test, zda to nen� ROOT polo�ka a zda to je soubor
			if ((item->Parent >= 0) && (item->Func != IDF_GROUP))
			{

// p��prava jm�na souboru
				ProgLib::InitName(index);
				CText text = Text.GetTxt(item->Name);

// p�ehr�v�n� souboru WAV
				if ((ProgLib::m_Ext == _T("WAV")) || (ProgLib::m_Ext == _T("MP3")))
				{
					CText name = ProgLib::m_Name;

					if (ProgLib::m_AltExt)
					{
						CSound snd;
						snd.LoadFile(name);

						TCHAR tmppath[101];
						tmppath[0] = 0;
						::GetTempPath(100, tmppath);
						PreviewTempName[0] = 0;
						::GetTempFileName(tmppath, _T("PET"), 0, PreviewTempName);
						name = PreviewTempName;
						snd.SaveFile(name);
						IsPreviewTempName = TRUE;
					}

					PreviewWav = ::PlaySound(name, NULL,
						SND_ASYNC | SND_FILENAME | SND_NODEFAULT | SND_NOSTOP | SND_NOWAIT
						| (TestRepeat(text) ? SND_LOOP : 0));

					return;
				}

// p�ehr�v�n� souboru MID
				if (ProgLib::m_Ext == _T("MID"))
				{

// otev�en� v�stupn�ho za��zen� MID
					MCI_OPEN_PARMS mop;

					mop.lpstrDeviceType = _T("sequencer");
					mop.lpstrElementName = ProgLib::m_Name;

					MCIERROR res = ::mciSendCommand(NULL, MCI_OPEN,
					    MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD) &mop);

					if (res == 0)
					{
						PreviewMid = mop.wDeviceID;

// p�ehr�n� souboru
						MCI_PLAY_PARMS mpp;
						res = ::mciSendCommand(PreviewMid, MCI_PLAY, 0, (DWORD) &mpp);

// p�i chyb� uzav�en� za��zen�
						if (res)
						{
							::mciSendCommand(PreviewMid, MCI_STOP, 0, NULL);
						    ::mciSendCommand(PreviewMid, MCI_CLOSE, 0, NULL);
							PreviewMid = 0;
						}
					}
					return;
				}

// preview obr�zku
				if ((ProgLib::m_Ext == _T("BMP")) || (ProgLib::m_Ext == _T("JPG")))
				{
					PreviewPicCache();

					if (!PreviewPicLoad[PreviewPicAkt])
					{
						PreviewPicPic[PreviewPicAkt].LoadFile(PreviewPicName[PreviewPicAkt], FALSE);
						PreviewPicLoad[PreviewPicAkt] = TRUE;
						PreviewTimerN = 1;
					}
					else
					{
						int inx = (PreviewPicVpred) ? PreviewPicNxt : PreviewPicPre;

						if (!PreviewPicLoad[inx])
						{
							PreviewPicPic[inx].LoadFile(PreviewPicName[inx], FALSE);
							PreviewPicLoad[inx] = TRUE;
							PreviewTimerN = 1;
						}
						else
						{
							inx ^= (PreviewPicNxt ^ PreviewPicPre);

							if (!PreviewPicLoad[inx])
							{
								PreviewPicPic[inx].LoadFile(PreviewPicName[inx], FALSE);
								PreviewPicLoad[inx] = TRUE;
							}
						}
					}

					if (PreviewPic)
					{
						ProgOnSize();
					}
					else
					{
						PreviewPicOn();
					}

					return;
				}
			}
		}
	}

// pro neplatnou polo�ku vypnut� re�imu preview obr�zku
	PreviewPicOff();
}
