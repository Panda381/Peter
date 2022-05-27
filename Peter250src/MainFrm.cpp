
#include "Main.h"

/***************************************************************************\
*																			*
*							Hlavní okno aplikace							*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// globální a lokální promìnné

TCHAR		MainFrameClass[] = _T("PeterMainFrameClass");	// název tøídy hlavního okna

HWND		MainFrame = NULL;						// hlavní okno aplikace
HMENU		Menu = NULL;							// aktivní menu
HWND		ToolBar = NULL;							// nástrojová lišta

HBITMAP		ToolBarBmp = NULL;						// bitmapa panelu nástrojù
HBITMAP		ToolBarMenu = NULL;						// bitmapa panelu nástrojù pro menu (nulováno pozadí)
HBITMAP		ToolBarMask = NULL;						// maska bitmapy panelu nástrojù (nulováno pozadí)

HWND		ToolTips = NULL;						// handle tooltips toolbaru
POINT		LastToolTips;							// poslední souøadnice tooltipu

#define		COMBOBOXSELHEIGHT 20					// výška pole výbìru comboboxu
#define		COMBOBOXHEIGHT 24						// výška øádku comboboxu

RECT		ClientRect;								// klientská oblast hlavního okna
int			ClientWidth;							// šíøka klientské oblasti
int			ClientHeight;							// výška klientské oblasti

HFONT		MenuAktFont = NULL;						// vytvoøený font pro menu

CText		MainFrameName;							// titulek hlavního okna - jméno aplikace
CText		MainFrameText;							// úplný text titulku hlavního okna

// stavový øádek
HWND		StatusBar = NULL;						// stavová lišta
CText		StatusText;								// zobrazený text stavové lišty
CText		StatusText2;							// zobrazený text stavové lišty 2
int			StatusWidth;							// šíøka panelu 2 stavové lišty
HFONT		StatFont = NULL;						// font stavového øádku

int			Waiting = 0;							// pøíznak zobrazení kurzoru èekání

// kurzory myši
HCURSOR		CurAkt;									// aktivní kurzor (použito pro tažení)

HCURSOR		CurArrow;								// (standardní) šipka
HCURSOR		CurCil;									// cíl operace
HCURSOR		CurCopy;								// kopie
HCURSOR		CurDelete;								// zrušení
HCURSOR		CurElip;								// elipsa
HCURSOR		CurFill;								// výplò
HCURSOR		CurFillElip;							// ovál
HCURSOR		CurFillRect;							// vyplnìný obdélník
HCURSOR		CurFillRound;							// kruh
HCURSOR		CurKapatko;								// kapátko
HCURSOR		CurKoule;								// koule
HCURSOR		CurLine;								// èára
HCURSOR		CurMove;								// pøesun
HCURSOR		CurNoDrag;								// zákaz tažení
HCURSOR		CurPaint;								// štìtec
HCURSOR		CurPen;									// pero
HCURSOR		CurRect;								// obdélník
HCURSOR		CurRound;								// kružnice
HCURSOR		CurRuka;								// ruka
HCURSOR		CurSelect;								// výbìr bloku
HCURSOR		CurSelectMove;							// pøesun výbìru
HCURSOR		CurSplitH;								// rozdìlení horizontálnì
HCURSOR		CurSplitV;								// rozdìlení vertikálnì
HCURSOR		CurSpray;								// sprej
HCURSOR		CurWait;								// èekání

// stav myši (aktualizováno v PreTranslateMessage)
BOOL		LMouseDown = FALSE;						// levé tlaèítko stisknuto
BOOL		LMouseDClick = FALSE;					// dvojklik levého tlaèítka
BOOL		RMouseDown = FALSE;						// pravé tlaèítko stisknuto
BOOL		RMouseDClick = FALSE;					// dvojklik pravého tlaèítka
POINT		MouseScreen = {MOUSEINV, MOUSEINV};		// souøadnice myši na displeji
POINT		MouseMain = {MOUSEINV, MOUSEINV};		// souøadnice myši v hlavním oknì

CText		HelpFileName;							// jméno souboru nápovìdy

/////////////////////////////////////////////////////////////////////////////
// opoždìné pøekreslení okna

#define		RepaintTimerID 19123					// ID èasovaèe pro pøekreslení okna
UINT		RepaintTimer = 0;						// èasovaè pro pøekreslení okna
int			RepaintTimerN = 0;						// èítaè pro pøekreslení okna
int			RepaintTimerN2 = 0;						// èítaè pro pøekreslení okna 2
BOOL		RepaintTimerBuf[PROGBUFNUM];			// pøíznaky bufferù k pøekreslení
BOOL		RepaintTimerProg = FALSE;				// pøekreslit okno programu ProgOnPaint

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

/////////////////////////////////////////////////////////////////////////////
// definice menu

MENUITEM** MenuAkt = NULL;							// aktuální menu (kvùli cachování)

// soubor 1
MENUITEM MenuSoubor1[] = 
{
	IDN_FILE0,		0,				-1,				0,
	IDN_START0,		IDN_START,		ButtonStart,	0,
	IDN_OPEN0,		IDN_OPEN,		ButtonOpen,		0,
	0,				1,				-1,				0,
	IDN_NEW0,		IDN_NEW,		ButtonNew,		0,
	IDN_PATH0,		IDN_PATH,		ButtonDirNew,	0,
	IDN_NEWCOPY0,	IDN_NEWCOPY,	ButtonCopy,		0,
	IDN_RENAME0,	IDN_RENAME,		ButtonEditName,	0,
	IDN_DELETE0,	IDN_DELETE,		ButtonClear,	0,
	0,				1,				-1,				0,
	IDN_SETUP0,		0,				-1,				0,
	IDN_SETUP10,	IDN_SETUP1,		-1,				0,
	IDN_SETUP20,	IDN_SETUP2,		-1,				0,
	IDN_SETUP30,	IDN_SETUP3,		-1,				0,
	IDN_SETUP40,	IDN_SETUP4,		-1,				0,
	IDN_SETUP50,	IDN_SETUP5,		-1,				0,
	0,				0,				-1,				0,
	IDN_JAZYK0,		0,				-1,				0,
	IDN_JAZYKAUT0,	IDN_JAZYKAUT,	-1,				MSWC,
	0,				1,				-1,				0,
//	IDN_JAZYKBUL0,	IDN_JAZYKBUL,	ButtonBul, 		MRAD,
	IDN_JAZYKCZ0,	IDN_JAZYKCZ,	ButtonCz, 		MRAD,
	IDN_JAZYKDAN0,	IDN_JAZYKDAN,	ButtonDan, 		MRAD,
	IDN_JAZYKGER0,	IDN_JAZYKGER,	ButtonGer,		MRAD,
//	IDN_JAZYKREC0,	IDN_JAZYKREC,	ButtonRec,		MRAD,
	IDN_JAZYKENG0,	IDN_JAZYKENG,	ButtonEng,		MRAD,
	IDN_JAZYKSPA0,	IDN_JAZYKSPA,	ButtonSpa,		MRAD,
//	IDN_JAZYKFIN0,	IDN_JAZYKFIN,	ButtonFin,		MRAD,
	IDN_JAZYKFRA0,	IDN_JAZYKFRA,	ButtonFra,		MRAD,
	IDN_JAZYKMAD0,	IDN_JAZYKMAD,	ButtonMad,		MRAD,
//	IDN_JAZYKISL0,	IDN_JAZYKISL,	ButtonIsl,		MRAD,
//	IDN_JAZYKITA0,	IDN_JAZYKITA,	ButtonIta,		MRAD,
//	IDN_JAZYKHOL0,	IDN_JAZYKHOL,	ButtonHol,		MRAD,
//	IDN_JAZYKNOR0,	IDN_JAZYKNOR,	ButtonNor,		MRAD,
//	IDN_JAZYKPOL0,	IDN_JAZYKPOL,	ButtonPol,		MRAD,
//	IDN_JAZYKPOR0,	IDN_JAZYKPOR,	ButtonPor,		MRAD,
//	IDN_JAZYKRUM0,	IDN_JAZYKRUM,	ButtonRum,		MRAD,
	IDN_JAZYKRUS0,	IDN_JAZYKRUS,	ButtonRus,		MRAD,
//	IDN_JAZYKSRB0,	IDN_JAZYKSRB,	ButtonSrb,		MRAD,
	IDN_JAZYKSLO0,	IDN_JAZYKSLO,	ButtonSlo,		MRAD,
//	IDN_JAZYKALB0,	IDN_JAZYKALB,	ButtonAlb,		MRAD,
//	IDN_JAZYKSWE0,	IDN_JAZYKSWE,	ButtonSwe,		MRAD,
//	IDN_JAZYKTUR0,	IDN_JAZYKTUR,	ButtonTur,		MRAD,
//	IDN_JAZYKVIE0,	IDN_JAZYKVIE,	ButtonVie,		MRAD,
//	IDN_JAZYKARA0,	IDN_JAZYKARA,	ButtonAra,		MRAD,
//	IDN_JAZYKHEB0,	IDN_JAZYKHEB,	ButtonHeb,		MRAD,
	0,				1,				-1,				0,
	IDN_JAZYKPRE0,	IDN_JAZYKPRE,	-1,				0,
	0,				0,				-1,				0,
	IDN_SHORTNAME0,	IDN_SHORTNAME,	-1,				MSWC,
	0,				1,				-1,				0,
	IDN_EXIT0,		IDN_EXIT,		-1,				0,
	0,				0,				-1,				0,
};

// soubor 1 multi
MENUITEM MenuSoubor11[] = 
{
	IDN_FILE0,		0,				-1,				0,
	IDN_START0,		IDN_START,		ButtonStart,	0,
	IDN_OPEN0,		IDN_OPEN,		ButtonOpen,		0,
	0,				1,				-1,				0,
	IDN_NEW0,		IDN_NEW,		ButtonNew,		0,
	IDN_PATH0,		IDN_PATH,		ButtonDirNew,	0,
	IDN_NEWCOPY0,	IDN_NEWCOPY,	ButtonCopy,		0,
	IDN_RENAME0,	IDN_RENAME,		ButtonEditName,	0,
	IDN_DELETE0,	IDN_DELETE,		ButtonClear,	0,
	0,				1,				-1,				0,
	IDN_SETUP0,		0,				-1,				0,
	IDN_SETUP10,	IDN_SETUP1,		-1,				0,
	IDN_SETUP20,	IDN_SETUP2,		-1,				0,
	IDN_SETUP30,	IDN_SETUP3,		-1,				0,
	IDN_SETUP40,	IDN_SETUP4,		-1,				0,
	IDN_SETUP50,	IDN_SETUP5,		-1,				0,
	0,				0,				-1,				0,
	IDN_JAZYK0,		0,				-1,				0,
	IDN_JAZYKAUT0,	IDN_JAZYKAUT,	-1,				MSWC,
	0,				1,				-1,				0,
	IDN_JAZYKBUL0,	IDN_JAZYKBUL,	ButtonBul, 		MRAD,
	IDN_JAZYKCZ0,	IDN_JAZYKCZ,	ButtonCz, 		MRAD,
	IDN_JAZYKDAN0,	IDN_JAZYKDAN,	ButtonDan, 		MRAD,
	IDN_JAZYKGER0,	IDN_JAZYKGER,	ButtonGer,		MRAD,
	IDN_JAZYKREC0,	IDN_JAZYKREC,	ButtonRec,		MRAD,
	IDN_JAZYKENG0,	IDN_JAZYKENG,	ButtonEng,		MRAD,
	IDN_JAZYKSPA0,	IDN_JAZYKSPA,	ButtonSpa,		MRAD,
	IDN_JAZYKFIN0,	IDN_JAZYKFIN,	ButtonFin,		MRAD,
	IDN_JAZYKFRA0,	IDN_JAZYKFRA,	ButtonFra,		MRAD,
	IDN_JAZYKMAD0,	IDN_JAZYKMAD,	ButtonMad,		MRAD,
	IDN_JAZYKISL0,	IDN_JAZYKISL,	ButtonIsl,		MRAD,
	IDN_JAZYKITA0,	IDN_JAZYKITA,	ButtonIta,		MRAD,
	IDN_JAZYKHOL0,	IDN_JAZYKHOL,	ButtonHol,		MRAD,
	IDN_JAZYKNOR0,	IDN_JAZYKNOR,	ButtonNor,		MRAD,
	IDN_JAZYKPOL0,	IDN_JAZYKPOL,	ButtonPol,		MRAD,
	IDN_JAZYKPOR0,	IDN_JAZYKPOR,	ButtonPor,		MRAD,
	IDN_JAZYKRUM0,	IDN_JAZYKRUM,	ButtonRum,		MRAD,
	IDN_JAZYKRUS0,	IDN_JAZYKRUS,	ButtonRus,		MRAD,
	IDN_JAZYKSRB0,	IDN_JAZYKSRB,	ButtonSrb,		MRAD,
	IDN_JAZYKSLO0,	IDN_JAZYKSLO,	ButtonSlo,		MRAD,
	IDN_JAZYKALB0,	IDN_JAZYKALB,	ButtonAlb,		MRAD,
	IDN_JAZYKSWE0,	IDN_JAZYKSWE,	ButtonSwe,		MRAD,
	IDN_JAZYKTUR0,	IDN_JAZYKTUR,	ButtonTur,		MRAD,
	IDN_JAZYKVIE0,	IDN_JAZYKVIE,	ButtonVie,		MRAD,
	IDN_JAZYKARA0,	IDN_JAZYKARA,	ButtonAra,		MRAD,
	IDN_JAZYKHEB0,	IDN_JAZYKHEB,	ButtonHeb,		MRAD,
	0,				1,				-1,				0,
	IDN_JAZYKPRE0,	IDN_JAZYKPRE,	-1,				0,
	0,				0,				-1,				0,
	IDN_SHORTNAME0,	IDN_SHORTNAME,	-1,				MSWC,
	0,				1,				-1,				0,
	IDN_EXIT0,		IDN_EXIT,		-1,				0,
	0,				0,				-1,				0,
};

// soubor 2
MENUITEM MenuSoubor2[] = 
{
	IDN_FILE0,		0,				-1,				0,
	IDN_START0,		IDN_START,		ButtonStart,	0,
	IDN_CLOSE0,		IDN_CLOSE,		ButtonClose,	0,
	0,				1,				-1,				0,
	IDN_SAVE0,		IDN_SAVE,		ButtonSave,		0,
	IDN_SAVEN0,		IDN_SAVEN,		-1,				0,
	IDN_SAVE_AS0,	IDN_SAVE_AS,	-1,				0,
	IDN_SAVE_REL0,	IDN_SAVE_REL,	-1,				0,
//	IDN_EXPORT0,	IDN_EXPORT,		-1,				0,
//	IDN_IMPORT0,	IDN_IMPORT,		-1,				0,
	0,				1,				-1,				0,
	IDN_SETUP0,		0,				-1,				0,
	IDN_SETUP10,	IDN_SETUP1,		-1,				0,
	IDN_SETUP20,	IDN_SETUP2,		-1,				0,
	IDN_SETUP30,	IDN_SETUP3,		-1,				0,
	IDN_SETUP40,	IDN_SETUP4,		-1,				0,
	IDN_SETUP50,	IDN_SETUP5,		-1,				0,
	0,				0,				-1,				0,
	IDN_JAZYK0,		0,				-1,				0,
	IDN_JAZYKAUT0,	IDN_JAZYKAUT,	-1,				MSWC,
	0,				1,				-1,				0,
//	IDN_JAZYKBUL0,	IDN_JAZYKBUL,	ButtonBul, 		MRAD,
	IDN_JAZYKCZ0,	IDN_JAZYKCZ,	ButtonCz, 		MRAD,
	IDN_JAZYKDAN0,	IDN_JAZYKDAN,	ButtonDan, 		MRAD,
	IDN_JAZYKGER0,	IDN_JAZYKGER,	ButtonGer,		MRAD,
//	IDN_JAZYKREC0,	IDN_JAZYKREC,	ButtonRec,		MRAD,
	IDN_JAZYKENG0,	IDN_JAZYKENG,	ButtonEng,		MRAD,
	IDN_JAZYKSPA0,	IDN_JAZYKSPA,	ButtonSpa,		MRAD,
//	IDN_JAZYKFIN0,	IDN_JAZYKFIN,	ButtonFin,		MRAD,
	IDN_JAZYKFRA0,	IDN_JAZYKFRA,	ButtonFra,		MRAD,
	IDN_JAZYKMAD0,	IDN_JAZYKMAD,	ButtonMad,		MRAD,
//	IDN_JAZYKISL0,	IDN_JAZYKISL,	ButtonIsl,		MRAD,
//	IDN_JAZYKITA0,	IDN_JAZYKITA,	ButtonIta,		MRAD,
//	IDN_JAZYKHOL0,	IDN_JAZYKHOL,	ButtonHol,		MRAD,
//	IDN_JAZYKNOR0,	IDN_JAZYKNOR,	ButtonNor,		MRAD,
//	IDN_JAZYKPOL0,	IDN_JAZYKPOL,	ButtonPol,		MRAD,
//	IDN_JAZYKPOR0,	IDN_JAZYKPOR,	ButtonPor,		MRAD,
//	IDN_JAZYKRUM0,	IDN_JAZYKRUM,	ButtonRum,		MRAD,
	IDN_JAZYKRUS0,	IDN_JAZYKRUS,	ButtonRus,		MRAD,
//	IDN_JAZYKSRB0,	IDN_JAZYKSRB,	ButtonSrb,		MRAD,
	IDN_JAZYKSLO0,	IDN_JAZYKSLO,	ButtonSlo,		MRAD,
//	IDN_JAZYKALB0,	IDN_JAZYKALB,	ButtonAlb,		MRAD,
//	IDN_JAZYKSWE0,	IDN_JAZYKSWE,	ButtonSwe,		MRAD,
//	IDN_JAZYKTUR0,	IDN_JAZYKTUR,	ButtonTur,		MRAD,
//	IDN_JAZYKVIE0,	IDN_JAZYKVIE,	ButtonVie,		MRAD,
//	IDN_JAZYKARA0,	IDN_JAZYKARA,	ButtonAra,		MRAD,
//	IDN_JAZYKHEB0,	IDN_JAZYKHEB,	ButtonHeb,		MRAD,
	0,				1,				-1,				0,
	IDN_JAZYKPRE0,	IDN_JAZYKPRE,	-1,				0,
	0,				0,				-1,				0,
	IDN_SHORTNAME0,	IDN_SHORTNAME,	-1,				MSWC,
	0,				1,				-1,				0,
	IDN_EXIT0,		IDN_EXIT,		-1,				0,
	0,				0,				-1,				0,
};

// soubor 2 multi
MENUITEM MenuSoubor22[] = 
{
	IDN_FILE0,		0,				-1,				0,
	IDN_START0,		IDN_START,		ButtonStart,	0,
	IDN_CLOSE0,		IDN_CLOSE,		ButtonClose,	0,
	0,				1,				-1,				0,
	IDN_SAVE0,		IDN_SAVE,		ButtonSave,		0,
	IDN_SAVEN0,		IDN_SAVEN,		-1,				0,
	IDN_SAVE_AS0,	IDN_SAVE_AS,	-1,				0,
	IDN_SAVE_REL0,	IDN_SAVE_REL,	-1,				0,
//	IDN_EXPORT0,	IDN_EXPORT,		-1,				0,
//	IDN_IMPORT0,	IDN_IMPORT,		-1,				0,
	0,				1,				-1,				0,
	IDN_SETUP0,		0,				-1,				0,
	IDN_SETUP10,	IDN_SETUP1,		-1,				0,
	IDN_SETUP20,	IDN_SETUP2,		-1,				0,
	IDN_SETUP30,	IDN_SETUP3,		-1,				0,
	IDN_SETUP40,	IDN_SETUP4,		-1,				0,
	IDN_SETUP50,	IDN_SETUP5,		-1,				0,
	0,				0,				-1,				0,
	IDN_JAZYK0,		0,				-1,				0,
	IDN_JAZYKAUT0,	IDN_JAZYKAUT,	-1,				MSWC,
	0,				1,				-1,				0,
	IDN_JAZYKBUL0,	IDN_JAZYKBUL,	ButtonBul, 		MRAD,
	IDN_JAZYKCZ0,	IDN_JAZYKCZ,	ButtonCz, 		MRAD,
	IDN_JAZYKDAN0,	IDN_JAZYKDAN,	ButtonDan, 		MRAD,
	IDN_JAZYKGER0,	IDN_JAZYKGER,	ButtonGer,		MRAD,
	IDN_JAZYKREC0,	IDN_JAZYKREC,	ButtonRec,		MRAD,
	IDN_JAZYKENG0,	IDN_JAZYKENG,	ButtonEng,		MRAD,
	IDN_JAZYKSPA0,	IDN_JAZYKSPA,	ButtonSpa,		MRAD,
	IDN_JAZYKFIN0,	IDN_JAZYKFIN,	ButtonFin,		MRAD,
	IDN_JAZYKFRA0,	IDN_JAZYKFRA,	ButtonFra,		MRAD,
	IDN_JAZYKMAD0,	IDN_JAZYKMAD,	ButtonMad,		MRAD,
	IDN_JAZYKISL0,	IDN_JAZYKISL,	ButtonIsl,		MRAD,
	IDN_JAZYKITA0,	IDN_JAZYKITA,	ButtonIta,		MRAD,
	IDN_JAZYKHOL0,	IDN_JAZYKHOL,	ButtonHol,		MRAD,
	IDN_JAZYKNOR0,	IDN_JAZYKNOR,	ButtonNor,		MRAD,
	IDN_JAZYKPOL0,	IDN_JAZYKPOL,	ButtonPol,		MRAD,
	IDN_JAZYKPOR0,	IDN_JAZYKPOR,	ButtonPor,		MRAD,
	IDN_JAZYKRUM0,	IDN_JAZYKRUM,	ButtonRum,		MRAD,
	IDN_JAZYKRUS0,	IDN_JAZYKRUS,	ButtonRus,		MRAD,
	IDN_JAZYKSRB0,	IDN_JAZYKSRB,	ButtonSrb,		MRAD,
	IDN_JAZYKSLO0,	IDN_JAZYKSLO,	ButtonSlo,		MRAD,
	IDN_JAZYKALB0,	IDN_JAZYKALB,	ButtonAlb,		MRAD,
	IDN_JAZYKSWE0,	IDN_JAZYKSWE,	ButtonSwe,		MRAD,
	IDN_JAZYKTUR0,	IDN_JAZYKTUR,	ButtonTur,		MRAD,
	IDN_JAZYKVIE0,	IDN_JAZYKVIE,	ButtonVie,		MRAD,
	IDN_JAZYKARA0,	IDN_JAZYKARA,	ButtonAra,		MRAD,
	IDN_JAZYKHEB0,	IDN_JAZYKHEB,	ButtonHeb,		MRAD,
	0,				1,				-1,				0,
	IDN_JAZYKPRE0,	IDN_JAZYKPRE,	-1,				0,
	0,				0,				-1,				0,
	IDN_SHORTNAME0,	IDN_SHORTNAME,	-1,				MSWC,
	0,				1,				-1,				0,
	IDN_EXIT0,		IDN_EXIT,		-1,				0,
	0,				0,				-1,				0,
};

// editace
MENUITEM MenuEditace[] =
{
	IDN_EDIT0,		0,				-1,				0,
	IDN_UNDO0,		IDN_UNDO,		ButtonUndo,		0,
	IDN_REDO0,		IDN_REDO,		ButtonRedo,		0,
	0,				1,				-1,				0,
	IDN_CUT0,		IDN_CUT,		ButtonCut,		0,
	IDN_COPY0,		IDN_COPY,		ButtonCopy,		0,
	IDN_PASTE0,		IDN_PASTE,		ButtonPaste,	0,
	IDN_CLEAR0,		IDN_CLEAR,		ButtonClear,	0,
	IDN_ALL0,		IDN_ALL,		ButtonAll,		0,
	0,				1,				-1,				0,
	IDN_DITHER0,	IDN_DITHER,		-1,				MSWC,
	0,				0,				-1,				0,
};

// zobrazení
MENUITEM MenuZobraz[] =
{
	IDN_ZOBRAZ0,	0,				-1,				0,
	IDN_IN0,		IDN_IN,			ButtonIn,		0,
	IDN_OUT0,		IDN_OUT,		ButtonOut,		0,
	0,				1,				-1,				0,
	IDN_HIST_PREV0,	IDN_HIST_PREV,	ButtonHistPrev,	0,
	IDN_HIST_NEXT0,	IDN_HIST_NEXT,	ButtonHistNext,	0,
	0,				1,				-1,				0,
	IDN_FIND0,		IDN_FIND,		ButtonFind,		0,
	IDN_FIND_PREV0,	IDN_FIND_PREV,	ButtonFindPrev,	0,
	IDN_FIND_NEXT0,	IDN_FIND_NEXT,	ButtonFindNext,	0,
	0,				0,				-1,				0,
};

// program
MENUITEM MenuProgram[] =
{
	IDN_PROGRAM0,	0,				-1,				0,
	IDN_LOCK0,		IDN_LOCK,		ButtonLock,		MSWC,
	IDN_OFF0,		IDN_OFF,		ButtonOff,		MSWC,
	0,				1,				-1,				0,
	IDN_EDIT_NAME0,	IDN_EDIT_NAME,	ButtonEditName,	MSWC,
	IDN_DEF_NAME0,	IDN_DEF_NAME,	ButtonDefName,	MSWC,
	IDN_EDIT_ICON0,	IDN_EDIT_ICON,	ButtonEditIcon,	MSWC,
	IDN_DEF_ICON0,	IDN_DEF_ICON,	ButtonDefIcon,	MSWC,
	0,				1,				-1,				0,
	IDN_REF_DEF0,	IDN_REF_DEF,	ButtonRefDef,	0,
	IDN_REF_PREV0,	IDN_REF_PREV,	ButtonRefPrev,	0,
	IDN_REF_NEXT0,	IDN_REF_NEXT,	ButtonRefNext,	0,
	0,				0,				-1,				0,
};

// ikona
MENUITEM MenuIkona[] =
{
	IDN_OBRAZEK0,	0,				-1,				0,
	IDN_RASTR0,		IDN_RASTR,		ButtonRastr,	MSWC,
	IDN_EDITBLOK0,	IDN_EDITBLOK,	ButtonEditBlok,	MSWC,
	IDN_OTISK0,		IDN_OTISK,		ButtonOtisk,	MSWC,
	0,				1,				-1,				0,
	IDN_YFLIP0,		IDN_YFLIP,		ButtonYFlip,	0,
	IDN_XFLIP0,		IDN_XFLIP,		ButtonXFlip,	0,
	0,				1,				-1,				0,
	IDN_LROT0,		IDN_LROT,		ButtonLRot,		0,
	IDN_RROT0,		IDN_RROT,		ButtonRRot,		0,
	0,				1,				-1,				0,
	IDN_MUL20,		IDN_MUL2,		ButtonMul2,		0,
	IDN_DIV20,		IDN_DIV2,		ButtonDiv2,		0,
	IDN_MULDIV0,	IDN_MULDIV,		ButtonMulDiv,	0,
	0,				1,				-1,				0,
	IDN_XCOL0,		IDN_XCOL,		ButtonXCol,		0,
	IDN_SCOL0,		IDN_SCOL,		ButtonSCol,		0,
	0,				0,				-1,				0,
};

// obrázek
MENUITEM MenuObrazek[] =
{
	IDN_OBRAZEK0,	0,				-1,				0,
	IDN_DIMEN0,		IDN_DIMEN,		ButtonDimen,	0,
	IDN_RASTR0,		IDN_RASTR,		ButtonRastr,	MSWC,
	IDN_EDITBLOK0,	IDN_EDITBLOK,	ButtonEditBlok,	MSWC,
	IDN_OTISK0,		IDN_OTISK,		ButtonOtisk,	MSWC,
	0,				1,				-1,				0,
	IDN_YFLIP0,		IDN_YFLIP,		ButtonYFlip,	0,
	IDN_XFLIP0,		IDN_XFLIP,		ButtonXFlip,	0,
	0,				1,				-1,				0,
	IDN_LROT0,		IDN_LROT,		ButtonLRot,		0,
	IDN_RROT0,		IDN_RROT,		ButtonRRot,		0,
	0,				1,				-1,				0,
	IDN_MUL20,		IDN_MUL2,		ButtonMul2,		0,
	IDN_DIV20,		IDN_DIV2,		ButtonDiv2,		0,
	IDN_MULDIV0,	IDN_MULDIV,		ButtonMulDiv,	0,
	0,				1,				-1,				0,
	IDN_XCOL0,		IDN_XCOL,		ButtonXCol,		0,
	IDN_SCOL0,		IDN_SCOL,		ButtonSCol,		0,
	0,				0,				-1,				0,
};

// nástroje
MENUITEM MenuNastroje[] =
{
	IDN_NASTROJE0,	0,				-1,				0,
	IDN_PEN0,		IDN_PEN,		ButtonPen,		MRAD,
	IDN_LINE0,		IDN_LINE,		ButtonLine,		MRAD,
	IDN_BOX0,		IDN_BOX,		ButtonBox,		MRAD,
	IDN_FILLBOX0,	IDN_FILLBOX,	ButtonFillBox,	MRAD,
	IDN_CIRCLE0,	IDN_CIRCLE,		ButtonCircle,	MRAD,
	IDN_FILLCIRCLE0,IDN_FILLCIRCLE, ButtonFillCircle,MRAD,
	IDN_ELLIPSE0,	IDN_ELLIPSE,	ButtonEllipse,	MRAD,
	IDN_FILLELLIPSE0,IDN_FILLELLIPSE,ButtonFillEllipse,MRAD,
	IDN_SPHERE0,	IDN_SPHERE,		ButtonSphere,	MRAD,
	IDN_SELECT0,	IDN_SELECT,		ButtonSelect,	MRAD,
	IDN_SPRAY0,		IDN_SPRAY,		ButtonSpray,	MRAD,
	IDN_PIPE0,		IDN_PIPE,		ButtonPipe,		MRAD,
	IDN_FILL0,		IDN_FILL,		ButtonFill,		MRAD,
	IDN_TEXT0,		IDN_TEXT,		ButtonText,		MRAD,
	0,				0,				-1,				0,
};

// tlouška pera
MENUITEM MenuTloustka[] =
{
	IDN_TLOUSTKA0,	0,				-1,				0,
	IDN_WIDTH10,	IDN_WIDTH1,		ButtonWidth1,	MRAD,
	IDN_WIDTH20,	IDN_WIDTH2,		ButtonWidth2,	MRAD,
	IDN_WIDTH30,	IDN_WIDTH3,		ButtonWidth3,	MRAD,
	IDN_WIDTH40,	IDN_WIDTH4,		ButtonWidth4,	MRAD,
	IDN_WIDTH50,	IDN_WIDTH5,		ButtonWidth5,	MRAD,
	0,				0,				-1,				0,
};

// plocha
MENUITEM MenuPlocha[] =
{
	IDN_PLOCHA0,	0,				-1,				0,
	IDN_DIMEN0,		IDN_DIMEN,		ButtonDimen,	0,
	IDN_EDITMAP0,	IDN_EDITMAP,	ButtonEditMap,	MSWC,
	IDN_FILLMAP0,	IDN_FILLMAP,	ButtonFillMap,	MSWC,
	IDN_RASTR0,		IDN_RASTR,		ButtonRastr,	MSWC,
	0,				1,				-1,				0,
	IDN_MAP0,		IDN_MAP,		ButtonMap,		MRAD,
	IDN_SWC10,		IDN_SWC1,		ButtonSwc1,		MRAD,
	IDN_SWC20,		IDN_SWC2,		ButtonSwc2,		MRAD,
	IDN_SWC30,		IDN_SWC3,		ButtonSwc3,		MRAD,
	IDN_SWC40,		IDN_SWC4,		ButtonSwc4,		MRAD,
	IDN_SWC50,		IDN_SWC5,		ButtonSwc5,		MRAD,
	IDN_SWCN10,		IDN_SWCN1,		ButtonSwcN1,	MRAD,
	IDN_SWCN20,		IDN_SWCN2,		ButtonSwcN2,	MRAD,
	IDN_SWCN30,		IDN_SWCN3,		ButtonSwcN3,	MRAD,
	0,				0,				-1,				0,
};

// sprajt
MENUITEM MenuSprajt[] =
{
	IDN_SPRAJT0,	0,				-1,				0,
	IDN_SPRITEPAR0,	IDN_SPRITEPAR,	ButtonSpritePar,0,
	IDN_TEST0,		IDN_TEST,		ButtonTest,		0,
	0,				0,				-1,				0,
};

// zvuk
MENUITEM MenuSound[] =
{
	IDN_SOUND0,		0,				-1,				0,
	IDN_PLAY0,		IDN_PLAY,		ButtonPlay,		MSWC,
	IDN_PAUSE0,		IDN_PAUSE,		ButtonPause,	MSWC,
	IDN_STOP0,		IDN_STOP,		ButtonStop,		0,
	IDN_LOOP0,		IDN_LOOP,		ButtonLoop,		MSWC,
	IDN_RECORD0,	IDN_RECORD,		ButtonRecord,	MSWC,
	0,				0,				-1,				0,
};

// hudba
MENUITEM MenuMusic[] =
{
	IDN_MUSIC0,		0,				-1,				0,
	IDN_PLAY0,		IDN_PLAY,		ButtonPlay,		MSWC,
	IDN_PAUSE0,		IDN_PAUSE,		ButtonPause,	MSWC,
	IDN_STOP0,		IDN_STOP,		ButtonStop,		0,
	IDN_LOOP0,		IDN_LOOP,		ButtonLoop,		MSWC,
	IDN_REW0,		IDN_REW,		ButtonRew,		0,
	IDN_FRW0,		IDN_FRW,		ButtonFrw,		0,
	0,				0,				-1,				0,
};

// nápovìda
MENUITEM MenuHlp[] =
{
	IDN_HLP0,		0,				-1,				0,
	IDN_HELP0,		IDN_HELP,		ButtonHelp,		0,
	IDN_HELPF0,		IDN_HELPF,		-1,				0,
	IDN_HELPT0,		IDN_HELPT,		ButtonHelpCont,	0,
	0,				1,				-1,				0,
	IDN_ABOUT0,		IDN_ABOUT,		-1,				0,
	0,				0,				-1,				0,
};

/////////////////////////////////////////////////////////////////////////////
// pøíprava jména hlavního okna

void MainFrameNameInit()
{
	MainFrameName.Load(IDN_MAINFRAME);
	CText txt;
#ifdef _DEBUG
	MainFrameName += " (DEBUG)";
#endif
}


/////////////////////////////////////////////////////////////////////////////
// vytvoøení masky obrázku panelu nástrojù

void CreateToolBarMask()
{	
// naètení informací o obrázku
	BITMAP bmpinfo;
	if (::GetObject(ToolBarMenu, sizeof(BITMAP), &bmpinfo))
	{
		int width = bmpinfo.bmWidth;
		int height = bmpinfo.bmHeight;

// vytvoøení DC pro pùvodní obrázek
		HDC dc = ::CreateCompatibleDC(NULL);
		if (dc != NULL)
		{

// nastavení obrázku do DC
			HBITMAP bmpold = (HBITMAP)::SelectObject(dc, ToolBarMenu);

// vytvoøení DC pro masku
			HDC dcmask = ::CreateCompatibleDC(dc);
			if (dcmask != NULL)
			{

// vytvoøení obrázku masky
				ToolBarMask = ::CreateBitmap(width, height, 1, 1, NULL);
				if (ToolBarMask != NULL)
				{

// nastavení obrázku masky do DC
					HBITMAP bmpmaskold = (HBITMAP)::SelectObject(dcmask, ToolBarMask);

// vytvoøení masky
					::SetBkColor(dc, ::GetPixel(dc, 0, 0));
					::BitBlt(dcmask, 0, 0, width, height, dc, 0, 0, SRCCOPY);

// zamaskování pùvodního obrázku (barva pozadí se nastaví na èernou)
					::SetBkColor(dc, 0);
					::SetTextColor(dc, 0xffffff);
					::BitBlt(dc, 0, 0, width, height, dcmask, 0, 0, SRCAND);

// návrat staré masky do DC
					::SelectObject(dcmask, bmpmaskold);
				}

// zrušení DC masky
				::DeleteObject(dcmask);
			}

// návrat starého obrázku do DC
			::SelectObject(dc, bmpold);

// zrušení DC obrázku
			::DeleteObject(dc);
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastavení smìru textu zprava doleva

void SetRightToLeft(BOOL righttoleft)
{

#ifndef WS_EX_LAYOUTRTL
#define WS_EX_LAYOUTRTL         0x00400000L
#endif

   ::SetWindowLong(MainFrame, GWL_EXSTYLE, (righttoleft ? (WS_EX_LAYOUTRTL | WS_EX_RTLREADING) : 0)
          | (::GetWindowLong(MainFrame, GWL_EXSTYLE) & ~(WS_EX_LAYOUTRTL | WS_EX_RTLREADING)));
   ::InvalidateRect(MainFrame, NULL, TRUE);
   ::UpdateWindow(MainFrame);
}


/////////////////////////////////////////////////////////////////////////////
// vytvoøení hlavního okna aplikace

BOOL MainFrameCreate()
{
// zajištìní knihovny rozšíøených ovládacích prvkù (až od verze 4.70 COMCTL32.DLL))
//	INITCOMMONCONTROLSEX ctl;
//	ctl.dwSize = sizeof(INITCOMMONCONTROLSEX);
//	ctl.dwICC = ICC_BAR_CLASSES | ICC_LISTVIEW_CLASSES | ICC_PROGRESS_CLASS | ICC_TAB_CLASSES | ICC_TREEVIEW_CLASSES;
//	::InitCommonControlsEx(&ctl);

	::InitCommonControls();

// registrace tøídy okna
	WNDCLASS wcl;
	wcl.style = CS_DBLCLKS;
	wcl.lpfnWndProc = WindowProc;
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;
	wcl.hInstance = hInstance;
	wcl.hIcon = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDN_MAINFRAME));
	wcl.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wcl.hbrBackground = NULL;
	wcl.lpszMenuName = MAKEINTRESOURCE(IDN_SELECT);
	wcl.lpszClassName = MainFrameClass;
	::RegisterClass(&wcl);

// naètení titulku hlavního okna
	MainFrameNameInit();
	MainFrameText = MainFrameName;

// pøíprava pozice a rozmìrù okna
	int x = (ScreenWidth*MainLeft+Scale/2)/Scale;
	if (x > ScreenWidth-30) x = ScreenWidth-30;
	int y = (ScreenHeight*MainTop+Scale/2)/Scale;
	if (y > ScreenHeight-30) y = ScreenHeight-30;
	int width = (ScreenWidth*MainWidth+Scale/2)/Scale;
	if (x+width < 30) width = 30-x;
	int height = (ScreenHeight*MainHeight+Scale/2)/Scale;
	if (y+height < 30) height = 30-y;

// vytvoøení hlavního okna
	MainFrame = ::CreateWindowEx(
		WS_EX_APPWINDOW | WS_EX_CONTROLPARENT | WS_EX_WINDOWEDGE,	// rozšíøený styl
		MainFrameClass,									// jméno tøídy okna
		MainFrameName,									// titulek okna
		WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN 
			| WS_MAXIMIZEBOX | WS_MINIMIZEBOX 
			| WS_OVERLAPPED | WS_SYSMENU | WS_THICKFRAME, // styl okna
		x,												// X
		y,												// Y
		width,											// šíøka
		height,											// výška
		NULL,											// rodiè
		NULL,											// menu
		hInstance,										// instance
		NULL);											// parametry
	if (MainFrame == NULL) return FALSE;

// zobrazení hlavního okna
	if (Maximized)
	{
		::ShowWindow(MainFrame, SW_SHOWMAXIMIZED);
	}
	else
	{
		::ShowWindow(MainFrame, SW_SHOW);
	}
//	::SetFocus(MainFrame);

// vytvoøení akceleraèní tabulky
	Accel = ::LoadAccelerators(hInstance, MAKEINTRESOURCE(IDN_MAINFRAME));
	ASSERT(Accel);

// vytvoøení nástrojové lišty
	ToolBar = ::CreateWindowEx(
		0,												// rozšíøený styl okna
		TOOLBARCLASSNAME,								// jméno tøídy okna
		NULL,											// nadpis okna

		WS_CHILD | WS_VISIBLE |							// styl okna
				TBSTYLE_TOOLTIPS | 0x800,
														//		0x0040 - bez horní oddìlovací linky
														//		0x0080 - zarovnáno na levé stranì

		0,												// souøadnice X
		0,												// souøadnice Y
		0,												// šíøka
		0,												// výška
		MainFrame,										// rodiè okna
		(HMENU)IDN_TOOLBAR,								// identifikátor okna
		hInstance,										// instance
		NULL);											// doplòující parametr

	::SendMessage(ToolBar, TB_SETBITMAPSIZE, 0, MAKELONG(16, 16));	// nastavení velikosti obrázkù tlaèítek
	::SendMessage(ToolBar, TB_SETBUTTONSIZE, 0, MAKELONG(28, 24));
	::SendMessage(ToolBar, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);	// definice velikosti struktury
	TBADDBITMAP tbab;
	tbab.hInst = hInstance;								// handle instance s bitmapou
	tbab.nID = IDN_TOOLBAR;								// ID resource bitmapy
	::SendMessage(ToolBar, TB_ADDBITMAP, ToolButtonNum, (LPARAM)&tbab);	// nastavení bitmapy tlaèítek

// naètení bitmapy panelu nástrojù
	ToolBarBmp = ::LoadBitmap(hInstance, MAKEINTRESOURCE(IDN_TOOLBAR));
	ASSERT(ToolBarBmp);

// vytvoøení masky panelu nástrojù
	ToolBarMenu = ::LoadBitmap(hInstance, MAKEINTRESOURCE(IDN_TOOLBAR));
	ASSERT(ToolBarMenu);
	CreateToolBarMask();

// naètení handle tooltips
	ToolTips = (HWND)::SendMessage(ToolBar, TB_GETTOOLTIPS, 0, 0);

// vytvoøení stavové lišty (viz též CreateStatusWindow)
	StatusBar = ::CreateWindowEx(
		0,
		STATUSCLASSNAME,
		NULL,
		WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
		0,
		0,
		0,
		0,
		MainFrame,
		(HMENU)IDN_STATUS,
		hInstance,
		NULL);

	SetStatusFont();

// naètení kurzorù myši
	CurArrow =		::LoadCursor(0,			MAKEINTRESOURCE(IDC_ARROW));		ASSERT(CurArrow			!= NULL);
	CurCil =		::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CIL));			ASSERT(CurCil			!= NULL);
	CurCopy =		::LoadCursor(hInstance,	MAKEINTRESOURCE(IDC_COPY));			ASSERT(CurCopy			!= NULL);
	CurDelete =		::LoadCursor(hInstance,	MAKEINTRESOURCE(IDC_DELETE));		ASSERT(CurDelete		!= NULL);
	CurElip	=		::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_ELIP));			ASSERT(CurElip			!= NULL);
	CurFill	=		::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_FILL));			ASSERT(CurFill			!= NULL);
	CurFillElip	=	::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_FILLELIP));		ASSERT(CurFillElip		!= NULL);
	CurFillRect	=	::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_FILLRECT));		ASSERT(CurFillRect		!= NULL);
	CurFillRound =	::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_FILLROUND));	ASSERT(CurFillRound		!= NULL);
	CurKapatko=		::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_KAPATKO));		ASSERT(CurKapatko		!= NULL);
	CurKoule =		::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_KOULE));		ASSERT(CurKoule			!= NULL);
	CurLine	=		::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_LINE));			ASSERT(CurLine			!= NULL);
	CurMove =		::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_MOVE));			ASSERT(CurMove			!= NULL);
	CurNoDrag =		::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_NODRAG));		ASSERT(CurNoDrag		!= NULL);
	CurPaint =		::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_PAINT));		ASSERT(CurPaint			!= NULL);
	CurPen =		::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_PEN));			ASSERT(CurPen			!= NULL);
	CurRect =		::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_RECT));			ASSERT(CurRect			!= NULL);
	CurRound =		::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_ROUND));		ASSERT(CurRound			!= NULL);
	CurRuka =		::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_RUKA));			ASSERT(CurRuka			!= NULL);
	CurSelect =		::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_SELECT));		ASSERT(CurSelect		!= NULL);
	CurSelectMove =	::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_SELECTMOVE));	ASSERT(CurSelectMove	!= NULL);
	CurSplitH =		::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_SPLITH));		ASSERT(CurSplitH		!= NULL);
	CurSplitV =		::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_SPLITV));		ASSERT(CurSplitV		!= NULL);
	CurSpray =		::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_SPRAY));		ASSERT(CurSpray			!= NULL);
	CurWait =		::LoadCursor(0,			MAKEINTRESOURCE(IDC_WAIT));			ASSERT(CurWait			!= NULL);

// aktualizace klientské oblasti
	InitClientRect();

// nastavení poètu stavových panelù
	SetStatusWidth(200);

// zahájení režimu výbìru souborù
	SelectStartInit();
	SelectInit();

// startovací inicializace modulu programování
	ProgStartInit();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace klientských souøadnic
// POZOR - toolbar a statusbar nemusí být ještì na správných pozicích!

void InitClientRect()
{
// naètení souøadnic klientské oblasti okna
	::GetClientRect(MainFrame, &ClientRect);

// urèení horního okraje klientské oblasti
	RECT rc;
	::GetWindowRect(ToolBar, &rc);
	ClientRect.top += rc.bottom - rc.top;

// urèení dolního okraje klientské oblasti
	::GetWindowRect(StatusBar, &rc);
	ClientRect.bottom -= rc.bottom - rc.top;

// urèení výšky a šíøky klientské oblasti
	ClientWidth = ClientRect.right - ClientRect.left;
	ClientHeight = ClientRect.bottom - ClientRect.top;
}

/////////////////////////////////////////////////////////////////////////////
// nastavení fontu stavového øádku

void SetStatusFont()
{
// vytvoøení nového fontu
	HFONT oldfont = StatFont;
	StatFont = GetFont(&StatusFont);

// zrušení pøedìlu
	int statpart[1];
	statpart[0] = -1;
	::SendMessage(StatusBar, SB_SETPARTS, 1, (LPARAM) &statpart);

// nastavení nového fontu
	::SendMessage(StatusBar, WM_SETFONT, (WPARAM)StatFont, TRUE);

// upøesnìní rozmìrù fontu
	HDC dc = ::GetDC(StatusBar);
	if (dc != NULL)
	{
		HFONT font2 = (HFONT)::SelectObject(dc, StatFont);
		if (font2 != NULL)
		{
			TEXTMETRIC tm;
			if (::GetTextMetrics(dc, &tm))
			{
				StatusFont.Height0 = tm.tmHeight;
				StatusFont.Width0 = tm.tmAveCharWidth;
			}
			::SelectObject(dc, font2);
		}
		::ReleaseDC(StatusBar, dc);
	}

// nastavení pøedìlu
	if (SelectMode)
	{
		SelectStatusWidth = StatusFont.Width0*70+16;
		StatusWidth = SelectStatusWidth;
	}
	else
	{
		ProgStatusWidth = StatusFont.Width0*23+16;
		StatusWidth = ProgStatusWidth;
	}

// nastavení výšky stavového øádku
	::SendMessage(StatusBar, SB_SETMINHEIGHT, (WPARAM)(StatusFont.Height0), 0);

// návrat pøedìlu a textu
	SetStatusWidth(StatusWidth);
	::SendMessage(StatusBar, SB_SETTEXT, 1 | SBT_OWNERDRAW, (LPARAM)&StatusText2);

// aktualizace okna
	::SendMessage(StatusBar, WM_SIZE, 0, 0);
	InitClientRect();
	SelectOnSize();
	ProgOnSize();

// zrušení starého fontu
	DelFont(oldfont);
}

/////////////////////////////////////////////////////////////////////////////
// zrušení tlaèítek panelu nástrojù (až po minimální množství)

void ToolBarClear(int min)
{
	int i = ::SendMessage(ToolBar, TB_BUTTONCOUNT, 0, 0)-1;
	for (; i >= min; i--)
	{
		::SendMessage(ToolBar, TB_DELETEBUTTON, i, 0);
	}
}


/////////////////////////////////////////////////////////////////////////////
// pøidání tlaèítek do panelu nástrojù

void ToolBarAdd(TBBUTTON* button, int num)
{
	::SendMessage(ToolBar, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);
	::SendMessage(ToolBar, TB_ADDBUTTONS, num, (LPARAM)button);
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace zobrazení panelu nástrojù

void ToolBarResize()
{
	::SendMessage(ToolBar, TB_AUTOSIZE, 0, 0); 
	InitClientRect();
}

/////////////////////////////////////////////////////////////////////////////
// nastavení obrázku tlaèítka v panelu nástrojù

void ToolBarBitmap(int id, int bitmap)
{
	::SendMessage(ToolBar, TB_CHANGEBITMAP, id, bitmap);
}

/////////////////////////////////////////////////////////////////////////////
// nastavení nového menu

// evidence položek aktivního menu
MENUAKTITEM*	MenuAktItems = NULL;		// buffer položek spuštìného menu
int				MenuAktItemsN = 0;			// poèet položek v bufferu spuštìného menu

// zrušení bufferu položek spuštìného menu
void MenuAktItemsDel()
{
	MENUAKTITEM* item = MenuAktItems;

	if (item != NULL)
	{
		for (int i = 0; i < MenuAktItemsN; i++)
		{
			item->text.Term();
			item->textshort.Term();
			item->help.Term();
			item->help2.Term();
			item++;
		}

		MemFree(MenuAktItems);
		MenuAktItems = NULL;
	}
	MenuAktItemsN = 0;
}

// vytvoøení nové položky v bufferu položek spuštìného menu (-1=chyba pamìti)
int MenuAktItemsNew(MENUITEM* vzor, HDC dc)
{
// zvýšení velikosti bufferu
	MENUAKTITEM* item = (MENUAKTITEM*)MemSize(MenuAktItems, (MenuAktItemsN+1)*sizeof(MENUAKTITEM));
	if (item == NULL) return -1;
	MenuAktItems = item;
	item += MenuAktItemsN;
	MenuAktItemsN++;
	MemFill(item, sizeof(MENUAKTITEM), 0);

// inicializace prvkù nové položky
	item->text.Init();
	item->textshort.Init();
	item->help.Init();
	item->help2.Init();

// pøenesení parametrù ze vzoru
	item->id0 = vzor->id0;
	item->id = vzor->id;
	item->bitmap = vzor->bitmap;
	item->typ = vzor->typ;

// naètení textu položky
	CText txt;
	if (vzor->id0 > 0) txt.Load(vzor->id0);

// rozdìlení na text položky a text klávesy
	int i = txt.Find(9);
	if (i >= 0)
	{
		item->textshort = txt.Right(txt.Length() - i - 1);
		txt.Delete(i);
	}
	item->text = txt;

// horká klávesa
	txt.UpperCase();
	item->key = 0;
	i = txt.Find(_T('&'));
	if (i >= 0)
	{
		item->key = txt.Get(i+1);
	}

// naètení nápovìdy
	txt.Load(vzor->id);

// rozdìlení na dlouhou a krátkou nápovìdu
	i = txt.Find(_T('\n'));
	if (i >= 0)
	{
		item->help2 = txt.Right(txt.Length() - i - 1);
		if (item->help2.Length() > 79) item->help2.Delete(79);
		txt.Delete(i);
	}
	item->help = txt;

// zjištìní zobrazené šíøky položky
	if (item->text.IsEmpty())
	{
		item->width = 8;
		item->height = 8;
	}
	else
	{
		txt = item->text + _T(' ') + item->textshort;
		RECT rc;
		rc.top = 0;
		rc.left = 0;
		rc.bottom = 10000;
		rc.right = 10000;
		::DrawText(dc, txt, txt.Length(), &rc,  DT_CALCRECT | DT_LEFT | DT_SINGLELINE);
		item->width = rc.right;
		item->height = rc.bottom;
		if (item->height < 18) item->height = 18;
	}

// adresa položky
	return (MenuAktItemsN-1);
}

// vytvoøení podmenu
HMENU CreateNewSubMenu(MENUITEM** itm, HDC dc)
{
// vytvoøení podmenu
	HMENU menu = ::CreatePopupMenu();
	int pos = 0;
	CText name;
	int minwidth = 0;

// vytvoøení položek
	while (((*itm)->id != 0) || ((*itm)->id0 != 0))
	{
// adresa položky menu
		MENUITEM* item = *itm;
		(*itm)++;

// vytvoøení nové položky v bufferu položek
		int inx = MenuAktItemsNew(item, dc);
		if (inx < 0) break;
		MENUAKTITEM* aktitem = MenuAktItems + inx;

// inicializace informace o položce
		aktitem->menu = menu;
		aktitem->main = FALSE;
		aktitem->inx = pos;

// stanovení minimální šíøky
		int okraj = 60;

		if ((aktitem->width + okraj)> minwidth)
		{
			minwidth = aktitem->width + okraj;
			for (int i = 0; i < MenuAktItemsN; i++)
			{
				if (MenuAktItems[i].menu == menu)
				{
					MenuAktItems[i].width = minwidth;
				}
			}
		}

// pøíprava struktury menu
		MENUITEMINFO mi;
		MemFill(&mi, sizeof(MENUITEMINFO), 0);
		mi.cbSize = sizeof(MENUITEMINFO);

// základní parametry položky
		mi.fMask = MIIM_TYPE | MIIM_STATE | MIIM_ID | MIIM_DATA;
		mi.fType = MFT_OWNERDRAW;
		mi.fState = MFS_ENABLED | MFS_UNCHECKED;
		mi.wID = item->id;
		mi.dwItemData = inx + 1;

// podmenu
		if (item->id0 > 0)
		{
			if (item->id == 0)
			{
				mi.hSubMenu = CreateNewSubMenu(itm, dc);
				mi.fMask |= MIIM_SUBMENU;
			}
		}
		else

// separátor
		{
			mi.fType |= MFT_SEPARATOR;
		}

// identifikátor jazyku
		int jazyk = -1;
		for (int ll = 0; ll < JAZYKNUM; ll++)
		{
			if (item->id == JazykInfo[ll].MenuID)
			{
				jazyk = ll;
				break;
			}
		}

// pøepínaè
		if ((jazyk == Jazyk) ||
			((item->id == IDN_JAZYKAUT) && (JazykUser == JAZYKAUT)) ||
			((item->id == IDN_DITHER) && Dither) ||
			((item->id == IDN_SHORTNAME) && ShortName))
		{
			mi.fState = MFS_CHECKED;
		}

// vložení položky
		::InsertMenuItem(menu, pos, TRUE, &mi);
		pos++;
	}

// pøeskoèení koncové položky
	(*itm)++;
	return menu;
}

void SetNewMenu(MENUITEM** item)
{
// kontrola, zda je menu již vytvoøeno
	if (item == MenuAkt) return;
	MenuAkt = item;

// zrušení bufferu položek aktivního menu
	MenuAktItemsDel();

// vytvoøení hlavního menu
	HMENU menu = ::CreateMenu();
	int pos = 0;
	CText name;

// vytvoøení nového fontu pro menu
	HFONT oldfont = MenuAktFont;
	MenuAktFont = GetFont(&MenuFont);

// pøíprava fontu k detekci šíøky položky
	HDC dc = ::GetDC(MainFrame);
	HFONT dcfont = (HFONT)::SelectObject(dc, MenuAktFont);

// vytvoøení podmenu
	while (*item != NULL)
	{

// adresa položky menu
		MENUITEM* itm = *item;

		if (MultiJazyk)
		{
			if (itm == MenuSoubor1) itm = MenuSoubor11;
			if (itm == MenuSoubor2) itm = MenuSoubor22;
		}

		item++;

// vytvoøení nové položky v bufferu položek
		int inx = MenuAktItemsNew(itm, dc);
		if (inx < 0) break;
		MENUAKTITEM* aktitem = MenuAktItems + inx;
		itm++;

// inicializace informace o položce
		aktitem->menu = menu;
		aktitem->main = TRUE;
		aktitem->inx = pos;

// pøíprava struktury menu
		MENUITEMINFO mi;
		MemFill(&mi, sizeof(MENUITEMINFO), 0);
		mi.cbSize = sizeof(MENUITEMINFO);

// základní parametry položky
		mi.fMask = MIIM_SUBMENU | MIIM_TYPE | MIIM_DATA;
		mi.fType = MFT_OWNERDRAW;
		mi.dwItemData = inx + 1;

// podmenu
		mi.hSubMenu = CreateNewSubMenu(&itm, dc);

// vložení položky
		::InsertMenuItem(menu, pos, TRUE, &mi);
		pos++;
	}

// návrat pùvodního fontu a uzavøení dc
	::SelectObject(dc, dcfont);
	::ReleaseDC(MainFrame, dc);

// nastavení hlavního menu
	::SetMenu(MainFrame, menu);
	::DestroyMenu(Menu);
	Menu = menu;

// zrušení pùvodního fontu
	DelFont(oldfont);
}


/////////////////////////////////////////////////////////////////////////////
// vytvoøení kombinovaného pole v panelu nástrojù

HWND CreateToolCombo(int id, int* items, int index)
{
// naètení souøadnice pøedcházejícího prvku
	RECT prev;
	int i = (int)::SendMessage(ToolBar, TB_BUTTONCOUNT, 0, 0) - 1;
	::SendMessage(ToolBar, TB_GETITEMRECT, i, (LPARAM)&prev);

// pøíprava souøadnic kombinovaného pole
	RECT rc;
	rc.left = prev.right + index*50;
	rc.top = prev.top;
	rc.right = rc.left + 44;
	rc.bottom = ClientRect.bottom;

// vytvoøení kombinovaného pole
	HWND combo = ::CreateWindowEx(
		0,
		_T("COMBOBOX"),
		NULL, 
		WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED,
		rc.left,				// X
		rc.top,					// Y
		rc.right - rc.left,		// šíøka
		rc.bottom - rc.top,		// výška
		ToolBar,				// rodiè
		(HMENU)id,				// identifikace
		hInstance,
		NULL);

// naètení prvkù
	TCHAR txt[2];
	txt[1] = 0;

	for (i = 0; items[2*i] >= 0; i++)
	{
		txt[0] = (TCHAR)(i + _T('A'));
		::SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)txt);
		::SendMessage(combo, CB_SETITEMDATA, i, items[2*i+1]);
	}

// pøipojení k tooltips
	TOOLINFO ti;
	ti.cbSize = sizeof(TOOLINFO);
	ti.uFlags = TTF_SUBCLASS | TTF_IDISHWND;
	ti.hwnd = ToolBar;
	ti.uId = (UINT)combo;
	ti.rect = rc;
	ti.hinst = hInstance;
	ti.lpszText = LPSTR_TEXTCALLBACK;
	::SendMessage(ToolTips, TTM_ADDTOOL, 0, (LPARAM)&ti);

	::SendMessage(ToolTips, TTM_ACTIVATE, TRUE, 0);

	return combo;
}


/////////////////////////////////////////////////////////////////////////////
// výbìr prvku v COMBO boxu

void SelectCombo(HWND combo, int id, int* items)
{
	if (combo)
	{
		for (int i = 0; items[2*i] >= 0; i++)
		{
			if (id == items[2*i+1])
			{
				CheckCommand(id, TRUE);
				::SendMessage(combo, CB_SETCURSEL, i, 0);
			}
			else
			{
				CheckCommand(items[2*i+1], FALSE);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// zobrazení kurzoru èekání (zahájení a ukonèení musí být do páru!)

void BeginWaitCursor()
{
// zvýšení èítaèe èekání
	Waiting++;

// zobrazení kurzoru
	if (Waiting > 0)
	{
		::SetCursor(CurWait);
	}
}

/////////////////////////////////////////////////////////////////////////////
// vypnutí kurzoru èekání (zahájení a ukonèení musí být do páru!)

void EndWaitCursor()
{
// snížení èítaèe èekání
	Waiting--;

// zobrazení kurzoru
	if (Waiting <= 0)
	{
		if (!ProgOnSetCursor())
		{
			::SetCursor(CurArrow);
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastavení/aktualizace šíøky pole 2 stavové lišty

void SetStatusWidth(int width)
{
	int maxwidth = ClientWidth*2/3;
	if (width > maxwidth) width = maxwidth;
	StatusWidth = width;
	int	StatusParts[2];
	StatusParts[0] = ClientWidth - width;
	StatusParts[1] = -1;
	::SendMessage(StatusBar, SB_SETPARTS, 2, (LPARAM) &StatusParts);
}


/////////////////////////////////////////////////////////////////////////////
// nastavení textu stavové lišty

void SetStatusText(const CText& text)
{
	if (text != StatusText)
	{
		StatusText = text;
		::SendMessage(StatusBar, SB_SETTEXT, 0 | SBT_OWNERDRAW, (LPARAM)&StatusText);
		LastToolTips = MouseScreen;
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení textu stavové lišty 2

void SetStatusText2(const CText& text)
{
	if (text != StatusText2)
	{
		StatusText2 = text;
		::SendMessage(StatusBar, SB_SETTEXT, 1 | SBT_OWNERDRAW, (LPARAM)&StatusText2);
	}
}

/////////////////////////////////////////////////////////////////////////////
// zobrazení jména souboru v titulku okna

void ZobrazJmeno()
{
	CText text;
	CText jmeno = Jmeno.MultiText();

	if (jmeno.IsNotEmpty())							// je nìjaké jméno souboru ?
	{
		text = jmeno;
		text.Add(_T(" - "));						// oddìlovaè
	}

	text.Add(MainFrameName);							// pøidání jména aplikace

	if (text != MainFrameText)						// mìní se titulek ?
	{
		MainFrameText = text;						// úschova nového titulku
		::SetWindowText(MainFrame, text);			// nastavení nového titulku
	}
}


/////////////////////////////////////////////////////////////////////////////
// povolení volby v menu a v panelu nástrojù

void EnableCommand(int id, BOOL enable)
{
// povolení tlaèítka nástrojového panelu
	::SendMessage(ToolBar, TB_ENABLEBUTTON, id, enable);

// povolení volby menu
	::EnableMenuItem(Menu, id, MF_BYCOMMAND | enable ? MF_ENABLED : (MF_DISABLED | MF_GRAYED));
}


/////////////////////////////////////////////////////////////////////////////
// zapnutí volby v menu a v panelu nástrojù

void CheckCommand(int id, BOOL check)
{
// zapnutí tlaèítka nástrojového panelu
	::SendMessage(ToolBar, TB_CHECKBUTTON, id, check);

// zapnutí volby menu
	::CheckMenuItem(Menu, id, MF_BYCOMMAND | check ? MF_CHECKED : MF_UNCHECKED);
}


/////////////////////////////////////////////////////////////////////////////
// zobrazení informaèního okna About

BOOL CALLBACK AboutBoxProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	if (!MemoryOK)
	{
		return FALSE;
	}

	switch (nMsg)
	{
	case WM_INITDIALOG:
		{

		CText txt;
		CText txt2;
		txt.Load(lParam);
		txt.SetWindowTextFont(hWnd);

		switch (lParam)
		{

// obsluha hlášení chyby ètení programu
		case IDN_READERR:
			{
				DialogCustomText(hWnd, IDN_READERR1);
				DialogCustomText(hWnd, IDN_READERR2);
				DialogCustomText(hWnd, IDN_READERR3);
			}
			break;

// obsluha hlášení chyby zápisu do programu
		case IDN_WRITEERR:
			{
				DialogCustomText(hWnd, IDN_WRITEERR1);
				DialogCustomText(hWnd, IDN_WRITEERR2);
				DialogCustomText(hWnd, IDN_WRITEERR3);
				DialogCustomText(hWnd, IDN_WRITEERR4);
				DialogCustomText(hWnd, IDN_WRITEERR5);
			}
			break;

// obsluha hlášení chyby otevøení programu
		case IDN_OPENERR:
			{
				DialogCustomText(hWnd, IDN_OPENERR1);
				DialogCustomText(hWnd, IDN_OPENERR2);
				DialogCustomText(hWnd, IDN_OPENERR3);
			}
			break;

// obsluha okna ABOUT
		case IDN_ABOUTBOX:
			{

// nastavení textu verze
				txt.Load(IDC_VERZE);
				txt += _T(' ');
				txt.AddInt(VerzeMaj);
				txt += _T('.');
				txt += _T('0') + VerzeMin;
				txt += _T('0') + VerzeRel;
				txt += _T(" FREEWARE");
				txt.SetDialogTextFont(hWnd, IDC_VERZE);
				DialogCustom0(hWnd, IDC_VERZE);

// text copyright
				DialogCustomText(hWnd, IDC_COPYNAME);

// www
				txt = _T("petr.hostuju.cz");
				txt.SetDialogTextFont(hWnd, IDC_WWW);

				txt = _T("panda38.sweb.cz");
				txt.SetDialogTextFont(hWnd, IDC_WWW2);

				txt = _T("www.gemtree.com");
				txt.SetDialogTextFont(hWnd, IDC_WWW3);
			}
			break;
		 }
		}

		CenterWindow(hWnd, MainFrame);
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
				::EndDialog(hWnd, LOWORD(wParam));
				return TRUE;
			}
			break;
		}
		break;
	}
	return FALSE;
}

void OnAboutBox()
{
	::DialogBoxParam(
		hInstance,
		MAKEINTRESOURCE(IDN_ABOUTBOX),
		MainFrame,
		(DLGPROC)AboutBoxProc,
		(LPARAM)IDN_ABOUTBOX);
}

// pøíprava jména souboru nápovìdy
void InitHelpName()
{
	HelpFileName = HomeDir + _T("Peter_") + JazykInfo[Jazyk].HelpName + _T(".hlp");

	if ((int)::GetFileAttributes(HelpFileName) == -1)
	{
		CText txt;

		if (Jazyk == JAZYKSLO)
		{
			txt = HomeDir + _T("Peter_CZ.hlp");

			if ((int)::GetFileAttributes(txt) != -1)
			{
				HelpFileName = txt;
				return;
			}
		}

		txt = HomeDir + _T("Peter_EN.hlp");

		if ((int)::GetFileAttributes(txt) != -1)
		{
			HelpFileName = txt;
			return;
		}

		WIN32_FIND_DATA wfd;			// struktura pro hledání souborù
		HANDLE file;					// handle hledání souborù
		file = ::FindFirstFile(HomeDir + _T("Peter*.hlp"), &wfd);
		if (file != INVALID_HANDLE_VALUE)
		{
			HelpFileName = HomeDir + CText(wfd.cFileName);
			::FindClose(file);
		}
	}
}


void OnHelp()
{
	InitHelpName();

	if (ProgMode)
	{
		if ((FocusBuf != BufEdiID) || (EditMode == BufEdiID))
		{
			CBufProg* buf = &BufProg[FocusBuf];
			int sel = buf->Select();
			if (buf->IsValid(sel))
			{
				sel = buf->At(sel).Func;
				if ((DWORD)(sel - IDF) < (DWORD)FncNum)
				{
					::WinHelp(MainFrame, HelpFileName, HELP_CONTEXT, sel);
					return;
				}
			}
		}

		int cont = 3;

		if (FocusBuf == BufEdiID)
		{
			switch (EditMode)
			{
			case BufNumID:
				cont = 110;
				break;

			case BufTxtID:
				cont = 130;
				break;

			case BufLogID:
				cont = 120;
				break;

			case BufIcoID:
				cont = 140;
				break;

			case BufMapID:
				cont = 150;
				break;

			case BufPicID:
				cont = 140;
				break;

			case BufSprID:
				cont = 180;
				break;

			case BufSndID:
				cont = 160;
				break;

			case BufMusID:
				cont = 170;
				break;
			}
		}

		::WinHelp(MainFrame, HelpFileName, HELP_CONTEXT, cont);
		return;
	}

	if (SelectMode)
	{
		::WinHelp(MainFrame, HelpFileName, HELP_CONTEXT, 2);
		return;
	}

	::WinHelp(MainFrame, HelpFileName, HELP_CONTEXT, 1);
}


/////////////////////////////////////////////////////////////////////////////
// obsluha zprávy pøed rozesláním do oken (TRUE = zpráva zpracována)

BOOL PreTranslateMessage(MSG* msg)
{
	HWND hWnd = msg->hwnd;
	WPARAM wParam = msg->wParam;
	LPARAM lParam = msg->lParam;

	switch (msg->message)
	{
	case WM_MOUSEMOVE:

// není stisknuto levé tlaæítko
		if ((msg->wParam & MK_LBUTTON) == 0)
		{
			LMouseDown = FALSE;
			LMouseDClick = FALSE;
		}

// není stisknuto pravé tlaèítko
		if ((msg->wParam & MK_RBUTTON) == 0)
		{
			RMouseDown = FALSE;
			RMouseDClick = FALSE;
		}

// úschova aktuální souøadnice kurzoru myši
		MouseScreen.x = LOWORD(lParam);
		MouseScreen.y = HIWORD(lParam);
		::ClientToScreen(hWnd, &MouseScreen);
		MouseMain = MouseScreen;
		::ScreenToClient(MainFrame, &MouseMain);

// vypnutí informací o položce pod kurzorem, je-li posun mimo editaèní pole
		if ((hWnd != MainFrame) ||
			(MouseMain.x < EditX) ||
			(MouseMain.x >= EditX + EditWidth) ||
			(MouseMain.y < EditY) ||
			(MouseMain.y >= EditY + EditHeight))
		{
			MouseX = MOUSEINV;
			MouseY = MOUSEINV;
			DispMouseXY();
		}

// pøi posunu myši daleko od poslední nápovìdy vypnout text ve stavovém øádku (kvùli nápovìdì v menu a toolbox)
		if ((abs(MouseScreen.x - LastToolTips.x) +
			(abs(MouseScreen.y - LastToolTips.y))) > 40)
		{
			LastToolTips = MouseScreen;
			SetStatusText(EmptyText);
		}
		break;

// dvojstisk levého tlaèítka myši
	case WM_LBUTTONDBLCLK:
		LMouseDown = TRUE;
		LMouseDClick = TRUE;
		goto MOUSECLICK;

// dvojstisk pravého tlaèítka myši
	case WM_RBUTTONDBLCLK:
		RMouseDown = TRUE;
		RMouseDClick = TRUE;
		goto MOUSECLICK;

// stisk levého tlaèítka myši
	case WM_LBUTTONDOWN:
		LMouseDown = TRUE;
		LMouseDClick = FALSE;
		goto MOUSECLICK;

// stisk pravého tlaèítka myši
	case WM_RBUTTONDOWN:
		RMouseDown = TRUE;
		RMouseDClick = FALSE;

MOUSECLICK:
		if ((FocusTimer != 0) && (FocusTimerN < 2)) FocusTimerN = 2;

// úschova souøadnic kurzoru myši
		MouseScreen.x = LOWORD(lParam);
		MouseScreen.y = HIWORD(lParam);
		::ClientToScreen(hWnd, &MouseScreen);
		MouseMain = MouseScreen;
		::ScreenToClient(MainFrame, &MouseMain);

// nastavení fokusu na editaèní pole
		if (ProgMode &&
			((hWnd == EditNumWnd) ||
			(hWnd == EditLogWnd) ||
			(hWnd == EditMapNumWnd) ||
			(hWnd == EditTextWnd)))
		{
			FocusBuf = BufEdiID;
			ProgSetFocus();
		}

// kliknutí do hlavního okna v editaèním módu
		if (ProgMode &&
			((hWnd == EditNumWnd) ||
			(hWnd == EditLogWnd) ||
			(hWnd == EditMapNumWnd) ||
			(hWnd == EditTextWnd) ||
			(hWnd == MainFrame) || 
			(hWnd == TreeObj) ||
			(hWnd == TreeLoc) ||
			(hWnd == TreeEdi) ||
			(hWnd == TreeStr) ||
			(hWnd == TreeCls)))
		{
			if (ProgOnButtonDown(wParam, MouseMain.x, MouseMain.y, 
				((msg->message == WM_RBUTTONDOWN) || (msg->message == WM_RBUTTONDBLCLK)),
				((msg->message == WM_LBUTTONDBLCLK) || (msg->message == WM_RBUTTONDBLCLK)))) return TRUE;
		}
		break;

// dvojstisk levého tlaèítka myši
	case WM_NCLBUTTONDBLCLK:
		LMouseDown = TRUE;
		LMouseDClick = TRUE;
		goto NCMOUSECLICK;

// dvojstisk pravého tlaèítka myši
	case WM_NCRBUTTONDBLCLK:
		RMouseDown = TRUE;
		RMouseDClick = TRUE;
		goto NCMOUSECLICK;

// stisk levého tlaèítka myši
	case WM_NCLBUTTONDOWN:
		LMouseDown = TRUE;
		LMouseDClick = FALSE;
		goto NCMOUSECLICK;

// stisk pravého tlaèítka myši
	case WM_NCRBUTTONDOWN:
		RMouseDown = TRUE;
		RMouseDClick = FALSE;

NCMOUSECLICK:

// úschova souøadnic kurzoru myši
		MouseScreen.x = MAKEPOINTS(lParam).x;
		MouseScreen.y = MAKEPOINTS(lParam).y;
		MouseMain = MouseScreen;
		::ScreenToClient(MainFrame, &MouseMain);
		
// kliknutí do hlavního okna v editaèním módu
		if (ProgMode &&
			((hWnd == EditNumWnd) ||
			(hWnd == EditLogWnd) ||
			(hWnd == EditMapNumWnd) ||
			(hWnd == EditTextWnd) ||
			(hWnd == MainFrame) || 
			(hWnd == TreeObj) ||
			(hWnd == TreeLoc) ||
			(hWnd == TreeEdi) ||
			(hWnd == TreeStr) ||
			(hWnd == TreeCls)))
		{
			if (ProgOnButtonDown(wParam, MouseMain.x, MouseMain.y, 
				((msg->message == WM_NCRBUTTONDOWN) || (msg->message == WM_NCRBUTTONDBLCLK)),
				((msg->message == WM_NCLBUTTONDBLCLK) || (msg->message == WM_NCRBUTTONDBLCLK)))) return TRUE;
		}
		break;

// uvolnìní levého tlaèítka myši
	case WM_LBUTTONUP:
		LMouseDown = FALSE;
		LMouseDClick = FALSE;

// úschova souøadnic kurzoru myši
		MouseScreen.x = LOWORD(lParam);
		MouseScreen.y = HIWORD(lParam);
		::ClientToScreen(hWnd, &MouseScreen);
		MouseMain = MouseScreen;
		::ScreenToClient(MainFrame, &MouseMain);

		ProgOnButtonUp(wParam, FALSE);
		break;

// uvolnìní pravého tlaèítka myši
	case WM_RBUTTONUP:
		RMouseDown = FALSE;
		RMouseDClick = FALSE;

// úschova souøadnic kurzoru myši
		MouseScreen.x = LOWORD(lParam);
		MouseScreen.y = HIWORD(lParam);
		::ClientToScreen(hWnd, &MouseScreen);
		MouseMain = MouseScreen;
		::ScreenToClient(MainFrame, &MouseMain);

		ProgOnButtonUp(wParam, TRUE);
		break;

// stisk klávesy
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (SelectMode)
		{
			return SelectOnKeyDown(hWnd, wParam, lParam);
		}
		return ProgOnKeyDown(hWnd, wParam, lParam);

// uvolnìní klávesy (aktualizace pøepínaèù)
	case WM_KEYUP:
	case WM_SYSKEYUP:
		if (ProgMode)
		{
			EditNum::UpdateClipboard();
			EditText::UpdateClipboard();
			EditMap::UpdateClipboard();
			if (EditName)
			{
				ProgUpdateClipboard();
			}
		}
		break;

// znak z klávesnice
	case WM_CHAR:
		if (ProgOnChar(hWnd, (TCHAR)wParam)) return TRUE;
		break;
	}

	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace pøedešlého okna

void AktPrevWindow()
{
	if ((MainFrame != NULL) && ::IsWindow(MainFrame))
	{
		HWND wnd = MainFrame;
		for (int i = 100; i > 0; i--)
		{
			wnd = ::GetNextWindow(wnd, GW_HWNDNEXT);
			if (wnd == NULL) break;
			long styl = ::GetWindowLong(wnd, GWL_STYLE);
			if (styl != 0)
			{
				if ((styl & (WS_VISIBLE | WS_DISABLED | WS_OVERLAPPEDWINDOW | WS_POPUP | WS_CHILD)) == (WS_VISIBLE | WS_OVERLAPPEDWINDOW))
				{
					PrevWindow = wnd;
					break;
				}
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// èasové pøekreslení okna (zadaného bufferu, -1=vše)

void TimeRepaint(int bufID)
{
	RepaintTimerN = 4;
	RepaintTimerN2 = 14;

	if ((DWORD)bufID < (DWORD)PROGBUFNUM) RepaintTimerBuf[bufID] = TRUE;
	if (bufID == -1)
	{
		for (int i = 0; i < PROGBUFNUM; i++) RepaintTimerBuf[i] = TRUE;
		RepaintTimerProg = TRUE;
	}

	if (RepaintTimer == 0)
	{
		RepaintTimer = ::SetTimer(MainFrame, RepaintTimerID, TimerConst, NULL);
	}
}

/////////////////////////////////////////////////////////////////////////////
// èasové pøekreslení okna po ubìhnutí èasu

void TimeRepaintNow()
{
	if (RepaintTimerProg)
	{
		ProgOnPaint();
		RepaintTimerProg = FALSE;
	}
	
	if (ProgMode)
	{
		if (RepaintTimerBuf[BufObjID])
		{
			UpdateWnd(TreeObj);
			RepaintTimerBuf[BufObjID] = FALSE;
		}

		if (RepaintTimerBuf[BufLocID])
		{
			UpdateWnd(TreeLoc);
			RepaintTimerBuf[BufLocID] = FALSE;
		}

		if (RepaintTimerBuf[BufEdiID])
		{
			UpdateWnd(TreeEdi);
			RepaintTimerBuf[BufEdiID] = FALSE;
		}

		if (RepaintTimerBuf[BufStrID])
		{
			UpdateWnd(TreeStr);
			RepaintTimerBuf[BufStrID] = FALSE;
		}

		if (RepaintTimerBuf[BufClsID])
		{
			UpdateWnd(TreeCls);
			RepaintTimerBuf[BufClsID] = FALSE;
		}
	}

	if (SelectMode)
	{
		UpdateWnd(SelectView);
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení fontu a stylu pro vlastní výpis textu

void DialogCustomText(HWND hWnd, DWORD id)
{
	DialogCustom(hWnd, id, TRUE, TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// nastavení vlastního výpisu textu

void DialogCustom0(HWND hWnd, DWORD id)
{
	DialogCustom(hWnd, id, FALSE, FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// nastavení vlastního výpisu textu

void DialogCustom(HWND hWnd, DWORD id, BOOL load, BOOL font)
{
	HWND wnd = ::GetDlgItem(hWnd, id);

	if (wnd != NULL)
	{
		if (load)
		{
			CText txt;
			txt.Load(id);
			txt.SetWindowText(wnd);
		}

		if (font)
		{
			SetFontBezny(wnd);
		}

		DWORD style = ::GetWindowLong(wnd, GWL_STYLE);
		DWORD style2 = (style & 0xf);

		style &= ~0xf;
		style |= SS_OWNERDRAW;

		if (style2 == SS_CENTER) style |= SS_CENTERIMAGE;
		if (style2 == SS_RIGHT) style |= SS_RIGHTJUST;

		::SetWindowLong(wnd, GWL_STYLE, style);
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení vlastního pøepínaèe RADIO

void DialogCustomRadio(HWND hWnd, DWORD id)
{
	HWND wnd = ::GetDlgItem(hWnd, id);

	if (wnd != NULL)
	{
		CText txt;
		txt.Load(id);
		txt.SetWindowText(wnd);
		SetFontBezny(wnd);

		DWORD style = ::GetWindowLong(wnd, GWL_STYLE);

		style &= ~0xf;
		style |= BS_OWNERDRAW | BS_LEFT;

		::SetWindowLong(wnd, GWL_STYLE, style);
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení vlastního pøepínaèe CHECK

void DialogCustomCheck(HWND hWnd, DWORD id)
{
	HWND wnd = ::GetDlgItem(hWnd, id);

	if (wnd != NULL)
	{
		CText txt;
		txt.Load(id);
		txt.SetWindowText(wnd);
		SetFontBezny(wnd);

		DWORD style = ::GetWindowLong(wnd, GWL_STYLE);

		style &= ~0xf;
		style |= BS_OWNERDRAW | BS_RIGHT;

		::SetWindowLong(wnd, GWL_STYLE, style);
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení vlastního rámeèku

void DialogCustomBox(HWND hWnd, DWORD id)
{
	HWND wnd = ::GetDlgItem(hWnd, id);

	if (wnd != NULL)
	{
		CText txt;
		txt.Load(id);
		txt.SetWindowText(wnd);
		SetFontBezny(wnd);

		DWORD style = ::GetWindowLong(wnd, GWL_STYLE);

		style &= ~0xf;
		style |= BS_OWNERDRAW | BS_BOTTOM;

		::SetWindowLong(wnd, GWL_STYLE, style);
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení hodnoty pøepínaèe

void DialogSetCheck(HWND hWnd, DWORD id, BOOL check)
{
	HWND wnd = ::GetDlgItem(hWnd, id);

	if (wnd != NULL)
	{
		DWORD oldstyle = ::GetWindowLong(wnd, GWL_STYLE);
		
		DWORD style = (oldstyle & ~BS_TOP);
		if (check) style |= BS_TOP;

		if (style != oldstyle)
		{
			::SetWindowLong(wnd, GWL_STYLE, style);
			::InvalidateRect(wnd, NULL, TRUE);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// vlastní vykreslení prvkù v dialogových oknech (vrací TRUE=obslouženo)

BOOL DialogDraw(HWND hWnd, LPARAM lParam)
{
	DRAWITEMSTRUCT* di = (DRAWITEMSTRUCT*)lParam;

	HDC dc = di->hDC;
	RECT rc = di->rcItem;
	DWORD id = di->CtlID;
	HWND wnd = di->hwndItem;
	BOOL focus = (di->itemState & ODS_FOCUS);
	BOOL gray = (di->itemState & (ODS_DISABLED | ODS_GRAYED));

	DWORD style = ::GetWindowLong(wnd, GWL_STYLE);

	CText text;
	text.GetWindowText(wnd);

	if (di->CtlType == ODT_BUTTON)
	{
		DWORD cent;

		if (style & (BS_LEFT | BS_RIGHT))
		{
			RECT rc2;
			rc2.left = rc.left;
			rc2.bottom = rc.bottom - 1;
			rc2.top = rc.top + 1;
			rc2.right = rc2.left + (rc2.bottom - rc2.top);

			BOOL radio = (style & BS_LEFT);

			if (!radio)
			{
				rc2.right--;
				rc2.bottom--;
			}

			::DrawFrameControl(dc, &rc2, DFC_BUTTON, 
				(gray ? DFCS_INACTIVE : 0) |
				(radio ? DFCS_BUTTONRADIO : DFCS_BUTTONCHECK) |
				((di->itemState & ODS_SELECTED) ? DFCS_PUSHED : 0) |
				((style & BS_TOP) ? DFCS_CHECKED : 0));

			rc.left += (rc.bottom - rc.top) + 2;

			rc2.left = rc.left - 1;
			rc2.bottom = rc.bottom;
			rc2.top = rc.top;

			::DrawText(dc, text.DataData(), text.Length(), &rc, DT_CALCRECT | DT_LEFT | DT_NOCLIP | DT_SINGLELINE | DT_VCENTER);

			rc2.right = rc.right + 1;

			HBRUSH brush = ::CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
			::FillRect(dc, &rc2, brush);
			::DeleteObject(brush);

			if (focus) ::DrawFocusRect(dc, &rc2);

			cent = DT_LEFT | DT_VCENTER;
		}
		else
		{
			if (style & BS_BOTTOM)
			{
				rc.top += 7;
				::DrawEdge(dc, &rc, EDGE_ETCHED, BF_RECT);
				rc.top -= 7;

				rc.left += 9;
				::DrawText(dc, text.DataData(), text.Length(), &rc, DT_CALCRECT | DT_LEFT | DT_NOCLIP | DT_SINGLELINE | DT_TOP);

				rc.left -= 2;;
				rc.right += 2;
				rc.bottom = rc.top + 16;
				HBRUSH brush = ::CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
				::FillRect(dc, &rc, brush);
				::DeleteObject(brush);
				rc.left += 2;

				gray = FALSE;
				cent = DT_LEFT | DT_TOP;
			}
			else
			{
				HWND w = ::GetFocus();

				BOOL isbutton = ((::GetWindowLong(w, GWL_STYLE) & (0xf | BS_LEFT)) == BS_OWNERDRAW);

				if (focus || ((id == IDOK) && (!isbutton)))
				{
					::FrameRect(dc, &rc, (HBRUSH)::GetStockObject(BLACK_BRUSH));

					rc.bottom--;
					rc.left++;
					rc.right--;
					rc.top++;
				}

				::DrawFrameControl(dc, &rc, DFC_BUTTON, DFCS_BUTTONPUSH |
					((di->itemState & ODS_SELECTED) ? DFCS_PUSHED : 0));

				if (focus)
				{
					::SendMessage(hWnd, DM_SETDEFID, id, 0);

					RECT rc2;
					rc2.bottom = rc.bottom - 3;
					rc2.left = rc.left + 3;
					rc2.right = rc.right - 3;
					rc2.top = rc.top + 3;

					::DrawFocusRect(dc, &rc2);
				}
				else
				{
					if (!isbutton)
					{
						::SendMessage(hWnd, DM_SETDEFID, IDOK, 0);

						if (id != IDOK)
						{
							HWND w = ::GetDlgItem(hWnd, IDOK);
							if (w != NULL)
							{
								::InvalidateRect(w, NULL, TRUE);
							}				
						}
					}
				}

				cent = DT_CENTER | DT_VCENTER;
			}
		}

		::SetBkMode(dc, TRANSPARENT);

		if (gray)
		{
			RECT rc2;
			rc2.bottom = rc.bottom + 1;
			rc2.left = rc.left + 1;
			rc2.right = rc.right + 1;
			rc2.top = rc.top + 1;

			::SetTextColor(dc, (::GetSysColor(COLOR_BTNHILIGHT) & 0xffffff));
			::DrawText(dc, text.DataData(), text.Length(), &rc2, cent | DT_NOCLIP | DT_SINGLELINE);

			::SetTextColor(dc, (::GetSysColor(COLOR_BTNSHADOW) & 0xffffff));
		}
		else
		{
			::SetTextColor(dc, (::GetSysColor(COLOR_BTNTEXT) & 0xffffff));
		}
		::DrawText(dc, text.DataData(), text.Length(), &rc, cent | DT_NOCLIP | DT_SINGLELINE);

		return TRUE;
	}

	if (di->CtlType == ODT_STATIC)
	{
		DWORD style = ::GetWindowLong(wnd, GWL_STYLE);

		::SetTextColor(dc, (::GetSysColor(COLOR_BTNTEXT) & 0xffffff));
		::DrawText(dc, text.DataData(), text.Length(), &rc, 
			((style & SS_CENTERIMAGE) ? DT_CENTER : 
				((style & SS_RIGHTJUST) ? DT_RIGHT : 0))| 
			DT_NOCLIP | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);

		return TRUE;
	}

	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// obsluha zpráv okna (ponechat obsluhu všech zpráv, aby se SWITCH pøekládal tabulkou!)

LRESULT CALLBACK WindowProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	if (!MemoryOK)
	{
		return ::DefWindowProc(hWnd, nMsg, wParam, lParam);
	}

	switch (nMsg)
	{
	case WM_NULL:					// 0x000
		goto DEFAULT;

	case WM_CREATE:					// 0x001
		goto DEFAULT;

	case WM_DESTROY:				// 0x002
		if (!TestModi()) return 0;
		if (RepaintTimer)
		{
			::KillTimer(MainFrame, RepaintTimer);
			RepaintTimer = 0;
		}
		RepaintTimerN = 0;
		RepaintTimerN2 = 0;
		PostQuitMessage(0);
		return 0;

	case WM_MOVE:					// 0x003
		goto DEFAULT;

	case 0x004:						// 0x004
		goto DEFAULT;

	case WM_SIZE:					// 0x005
		ToolBarResize();
		SelectOnSize();
		ProgOnSize();
		TimeRepaint(-1);
		return 0;

	case WM_ACTIVATE:				// 0x006
		goto DEFAULT;

	case WM_SETFOCUS:				// 0x007
		SetJazykReg(LangID);
		SelectSetFocus();
		ProgSetFocus();
		ProgUpdateClipboard();
		ProgUpdateUndoRedo();

		TimeRepaint(-1);
		return 0;

	case WM_KILLFOCUS:				// 0x008
		goto DEFAULT;

	case 0x009:						// 0x009
		goto DEFAULT;

	case WM_ENABLE:					// 0x00A
		goto DEFAULT;

	case WM_SETREDRAW:				// 0x00B
		goto DEFAULT;

	case WM_SETTEXT:				// 0x00C
		goto DEFAULT;

	case WM_GETTEXT:				// 0x00D
		goto DEFAULT;

	case WM_GETTEXTLENGTH:			// 0x00E
		goto DEFAULT;

	case WM_PAINT:					// 0x00F
		{
			PAINTSTRUCT paint;
			::BeginPaint(hWnd, &paint);

			HPALETTE OldPal = ::SelectPalette(paint.hdc, StdPalette, FALSE);
			::RealizePalette(paint.hdc);
			::SelectPalette(paint.hdc,OldPal,FALSE);

			::EndPaint(hWnd, &paint);

			ProgOnPaint();
			return 0;
		}
		goto DEFAULT;

	case WM_CLOSE:					// 0x010
		{
			if (!TestModi()) return 0;

			ResModi();
	
			WINDOWPLACEMENT wp;
			wp.length = sizeof(WINDOWPLACEMENT);
			::GetWindowPlacement(MainFrame, &wp);
			MainLeft = (wp.rcNormalPosition.left*Scale + ScreenWidth/2)/ScreenWidth;
			MainTop = (wp.rcNormalPosition.top*Scale + ScreenHeight/2)/ScreenHeight;
			MainWidth = ((wp.rcNormalPosition.right - wp.rcNormalPosition.left)*Scale + ScreenWidth/2)/ScreenWidth;
			MainHeight = ((wp.rcNormalPosition.bottom - wp.rcNormalPosition.top)*Scale + ScreenHeight/2)/ScreenHeight;

			Maximized = ::IsZoomed(MainFrame);

			AktPrevWindow();
		};

		if (RepaintTimer)
		{
			::KillTimer(MainFrame, RepaintTimer);
			RepaintTimer = 0;
		}
		RepaintTimerN = 0;
		RepaintTimerN2 = 0;

		goto DEFAULT;

	case WM_QUERYENDSESSION:		// 0x011
		goto DEFAULT;

	case WM_QUIT:					// 0x012
		goto DEFAULT;

	case WM_QUERYOPEN:				// 0x013
		goto DEFAULT;

	case WM_ERASEBKGND:				// 0x014
		goto DEFAULT;

	case WM_SYSCOLORCHANGE:			// 0x015
		goto DEFAULT;

	case WM_ENDSESSION:				// 0x016
		goto DEFAULT;

	case 0x017:						// 0x017
		goto DEFAULT;

	case WM_SHOWWINDOW:				// 0x018
		goto DEFAULT;

	case 0x019:						// 0x019
		goto DEFAULT;

	case WM_WININICHANGE:			// 0x01A
		goto DEFAULT;

	case WM_DEVMODECHANGE:			// 0x01B
		goto DEFAULT;

	case WM_ACTIVATEAPP:			// 0x01C
		AktPrevWindow();
		if ((BOOL)wParam)
		{
			HCURSOR curs = ::GetCursor();	// obnovení kurzoru
			::SetCursor(NULL);
			::SetCursor(curs);
		}
		goto DEFAULT;

	case WM_FONTCHANGE:				// 0x01D
		goto DEFAULT;

	case WM_TIMECHANGE:				// 0x01E
		goto DEFAULT;

	case WM_CANCELMODE:				// 0x01F
		goto DEFAULT;

	case WM_SETCURSOR:				// 0x020
		if (Waiting)
		{
			::SetCursor(CurWait);
			return TRUE;
		}

		::GetCursorPos(&MouseScreen);	// naètení souøadnic myši
		MouseMain = MouseScreen;
		::ScreenToClient(MainFrame, &MouseMain); // pøevod na klientské souøadnice

		if (ProgOnSetCursor()) return TRUE;
		goto DEFAULT;

	case WM_MOUSEACTIVATE:			// 0x021
		goto DEFAULT;

	case WM_CHILDACTIVATE:			// 0x022
		goto DEFAULT;

	case WM_QUEUESYNC:				// 0x023
		goto DEFAULT;

	case WM_GETMINMAXINFO:			// 0x024
		goto DEFAULT;

	case 0x025:						// 0x025
		goto DEFAULT;

	case WM_PAINTICON:				// 0x026
		goto DEFAULT;

	case WM_ICONERASEBKGND:			// 0x027
		goto DEFAULT;

	case WM_NEXTDLGCTL:				// 0x028
		goto DEFAULT;

	case 0x029:						// 0x029
		goto DEFAULT;

	case WM_SPOOLERSTATUS:			// 0x02A
		goto DEFAULT;

	case WM_DRAWITEM:				// 0x02B
		{
			DRAWITEMSTRUCT* di = (DRAWITEMSTRUCT*)lParam;

			DWORD smer = JazykInfo[Jazyk].RightToLeft ? DT_RTLREADING : 0;   //JAKJAK

			if (di->CtlID == IDN_STATUS)
			{
				HDC dc = di->hDC;
				::SetBkMode(dc, TRANSPARENT);
				::SetTextColor(dc, (::GetSysColor(COLOR_BTNTEXT) & 0xffffff));
				CText* text = (CText*)di->itemData;
			::DrawText(dc, text->DataData(), text->Length(), &(di->rcItem), DT_LEFT | DT_SINGLELINE | DT_VCENTER | smer);
			}

			if (di->CtlType == ODT_MENU)
			{
				int inx = di->itemData - 1;

				if ((DWORD)inx < (DWORD)MenuAktItemsN)
				{

		// pøíprava ukazatelù
					MENUAKTITEM* item = MenuAktItems + inx;
					HDC dc = di->hDC;
					HFONT oldfont = (HFONT)::SelectObject(dc, MenuAktFont);
					RECT rc = di->rcItem;
					DWORD state = di->itemState;

		// pøíprava barvy pozadí a písma
					DWORD barvapozadi = (::GetSysColor(COLOR_MENU) & 0xffffff);
					DWORD barvatextu = (::GetSysColor(COLOR_MENUTEXT) & 0xffffff);
					DWORD barvastinu = (::GetSysColor(COLOR_3DSHADOW) & 0xffffff);
					DWORD barvasvetla = (::GetSysColor(COLOR_3DHILIGHT) & 0xffffff);

					if ((state & ODS_SELECTED) && !item->main)
					{
						barvapozadi = (::GetSysColor(COLOR_HIGHLIGHT) & 0xffffff);
						barvatextu = (::GetSysColor(COLOR_HIGHLIGHTTEXT) & 0xffffff);
					}

		// vymazání podkladu
					HBRUSH brush = ::CreateSolidBrush(barvapozadi);
					::FillRect(dc, &rc, brush);
					::SetBkColor(dc, barvapozadi);
					::SetBkMode(di->hDC, TRANSPARENT);

		// vykreslení oddìlovaèe
					if (item->id0 == 0)
					{
						rc.top += item->height/2 - 1;
						rc.left += 2;
						rc.right -= 2;
						::DrawEdge(dc, &rc, EDGE_ETCHED, BF_TOP);
					}
					else
					{

		// výpis textu hlavní nabídky
						if (item->main)
						{
							if (state & ODS_SELECTED)
							{
								::DrawEdge(dc, &rc, BDR_SUNKENOUTER, BF_RECT);
								rc.left++;
								rc.top++;
							}

							::SetTextColor(dc, barvatextu);
				::DrawText(dc, item->text, item->text.Length(), &rc, DT_CENTER | DT_SINGLELINE | DT_VCENTER | smer);
						}
						else
						{

		// pøíprava obrázku pøepínaèe
							int icon = -1;
							if (item->typ & MSWC)
							{
								if (state & ODS_CHECKED)
								{
									icon = ButtonCheck;
								}
								else
								{
									icon = ButtonCheckOff;
								}
							}

							if (item->typ & MRAD)
							{
								if (state & ODS_CHECKED)
								{
									icon = ButtonRadioOn;
								}
								else
								{
									icon = ButtonRadioOff;
								}
							}

		// vykreslení pøepínaèe (musí být vybrána správná barva pozadí)
							::SetTextColor(dc, 0);

							if (icon >= 0)
							{
	if (JazykInfo[Jazyk].RightToLeft) icon = ToolButtonNum - icon - 1;   // JAKJAK ... správné vykreslení ikonek
								int x = rc.left + 2;
								int y = rc.top + (rc.bottom - rc.top - 16)/2;

								HDC dc2 = ::CreateCompatibleDC(dc);
								HBITMAP bmpold = (HBITMAP)::SelectObject(dc2, ToolBarMask);
								::BitBlt(dc, x, y, 16, 16, dc2, icon*16, 0, SRCCOPY);

								::SelectObject(dc2, ToolBarMenu);
								::BitBlt(dc, x, y, 16, 16, dc2, icon*16, 0, SRCPAINT);

								::SelectObject(dc2, bmpold);
								::DeleteObject(dc2);
							}
							rc.left += 20;

		// vykreslení ikony obrázku
							icon = item->bitmap;
							if (icon >= 0)
							{
								if ((icon == ButtonSave) && Modi)
								{
									icon = ButtonSaveModi;
								}
	if (JazykInfo[Jazyk].RightToLeft) icon = ToolButtonNum - icon - 1;   // JAKJAK ... správné vykreslení ikonek
								int x = rc.right - 20;
								int y = rc.top + (rc.bottom - rc.top - 16)/2;

								HDC dc2 = ::CreateCompatibleDC(dc);

								HBITMAP bmpold = (HBITMAP)::SelectObject(dc2, ToolBarMask);

								if (state & ODS_GRAYED)
								{
									if ((state & ODS_SELECTED) == 0)
									{
										::SetTextColor(dc, barvastinu);
									}
									else
									{
										::SetTextColor(dc, barvasvetla);
									}
									::BitBlt(dc, x, y, 16, 16, dc2, icon*16, 0, SRCCOPY);
								}
								else
								{
									::BitBlt(dc, x, y, 16, 16, dc2, icon*16, 0, SRCCOPY);
									::SelectObject(dc2, ToolBarMenu);
									::BitBlt(dc, x, y, 16, 16, dc2, icon*16, 0, SRCPAINT);
								}

								::SelectObject(dc2, bmpold);
								::DeleteObject(dc2);
							}
							rc.right -= 30;

		// výpis podkladového textu pro zakázanou položku
							if (state & ODS_GRAYED)
							{
								barvatextu = barvasvetla;

								if ((state & ODS_SELECTED) == 0)
								{
									rc.left++;
									rc.top++;
									::SetTextColor(dc, barvatextu);
									::DrawText(dc, item->text, item->text.Length(), &rc, DT_LEFT | DT_SINGLELINE | DT_VCENTER | smer);
									::DrawText(dc, item->textshort, item->textshort.Length(), &rc, DT_RIGHT | DT_SINGLELINE | DT_VCENTER | smer);
									rc.left--;
									rc.top--;
									rc.right--;
									barvatextu = barvastinu;
								}
							}

		// výpis textu
							::SetTextColor(dc, barvatextu);
							::DrawText(dc, item->text, item->text.Length(), &rc, DT_LEFT | DT_SINGLELINE | DT_VCENTER | smer);
							::DrawText(dc, item->textshort, item->textshort.Length(), &rc, DT_RIGHT | DT_SINGLELINE | DT_VCENTER | smer);
						}
					}

					::DeleteObject(brush);
					::SelectObject(dc, oldfont);
				}
				return TRUE;
			}

			if (ProgOnDrawItem(di)) return TRUE;
		}
		goto DEFAULT;

	case WM_MEASUREITEM:			// 0x02C
		{
			MEASUREITEMSTRUCT* mi = (MEASUREITEMSTRUCT*)lParam;

			if (mi->CtlType == ODT_COMBOBOX)
			{
				if (mi->itemID == (UINT)-1)
				{
					mi->itemHeight = COMBOBOXSELHEIGHT;
				}
				else
				{
					mi->itemHeight = COMBOBOXHEIGHT;
				}
			}

			if (mi->CtlType == ODT_MENU)
			{
				int inx = mi->itemData - 1;

				if ((DWORD)inx < (DWORD)MenuAktItemsN)
				{
					mi->itemHeight = MenuAktItems[inx].height;
					mi->itemWidth = MenuAktItems[inx].width;
				}
				else
				{
					mi->itemHeight = 20;
					mi->itemWidth = 200;
				}
			}
		}
		return TRUE;					

	case WM_DELETEITEM:				// 0x02D
		goto DEFAULT;

	case WM_VKEYTOITEM:				// 0x02E
		goto DEFAULT;

	case WM_CHARTOITEM:				// 0x02F
		goto DEFAULT;

	case WM_SETFONT:				// 0x030
		goto DEFAULT;

	case WM_GETFONT:				// 0x031
		goto DEFAULT;

	case WM_SETHOTKEY:				// 0x032
		goto DEFAULT;

	case WM_GETHOTKEY:				// 0x033
		goto DEFAULT;

	case 0x034:						// 0x034
		goto DEFAULT;

	case 0x035:						// 0x035
		goto DEFAULT;

	case 0x036:						// 0x036
		goto DEFAULT;

	case WM_QUERYDRAGICON:			// 0x037
		goto DEFAULT;

	case 0x038:						// 0x038
		goto DEFAULT;

	case WM_COMPAREITEM:			// 0x039
		goto DEFAULT;

	case 0x040:						// 0x040
		goto DEFAULT;

	case WM_COMPACTING:				// 0x041
		goto DEFAULT;

	case 0x042:						// 0x042
		goto DEFAULT;

	case 0x043:						// 0x043
		goto DEFAULT;

	case WM_COMMNOTIFY:				// 0x044
		goto DEFAULT;

	case 0x045:						// 0x045
		goto DEFAULT;

	case WM_WINDOWPOSCHANGING:		// 0x046
		goto DEFAULT;

	case WM_WINDOWPOSCHANGED:		// 0x047
		goto DEFAULT;

	case WM_POWER:					// 0x048
		goto DEFAULT;

	case 0x049:						// 0x049
		goto DEFAULT;

	case WM_COPYDATA:				// 0x04A
		goto DEFAULT;

	case WM_CANCELJOURNAL:			// 0x04B
		goto DEFAULT;

	case 0x04C:						// 0x04C
		goto DEFAULT;

	case 0x04D:						// 0x04D
		goto DEFAULT;

	case WM_NOTIFY:					// 0x04E
		{
			NMHDR* nmhdr = ((NMHDR*)lParam);
			switch (nmhdr->code)
			{
			case TTN_NEEDTEXT:
			  {
				TOOLTIPTEXT* tip = (TOOLTIPTEXT*)lParam;
				CText txt;
				tip->szText[0] = 0;
				ASSERT(tip->lpszText == tip->szText);

				if (tip->uFlags & TTF_IDISHWND)
				{
					HWND idw = (HWND)tip->hdr.idFrom;
					
					txt.Load(::SendMessage(idw, CB_GETITEMDATA, 
						::SendMessage(idw, CB_GETCURSEL, 0, 0), 0));
				}
				else
				{
					txt.Load(tip->hdr.idFrom);
				}

				int pos = txt.Find(_T('\n'));
				int len = txt.Length() - pos - 1;
				if (len > 79) len = 79;
				if (pos >= 0)
				{
					MemCopy(tip->szText, (LPCTSTR)txt+pos+1, len);
					tip->szText[len] = 0;
					txt.Delete(pos);
				}
				tip->hinst = NULL;

				SetStatusText(txt);
				return 0;
			  }

			case TTN_POP:
				SetStatusText(EmptyText);
				return 0;

			case LVN_ITEMCHANGED:
				SelectOnChange(nmhdr->hwndFrom, (NM_LISTVIEW*)lParam);
				return 0;

			case TVN_ITEMEXPANDING:
			// (pozor - vyvolá se po dvojkliku v editaèním oknì u zrušené položky,
			//  bez zákazu rozvinutí vznikne chyba pamìti! Bez této obsluhy havaruje!)
				return ProgOnExpanding(nmhdr->hwndFrom, &(((NM_TREEVIEW*)lParam)->itemNew), 
						((((NM_TREEVIEW*)lParam)->action & TVE_EXPAND) != 0));

			case TVN_ITEMEXPANDED:
				ProgOnExpand(nmhdr->hwndFrom, &(((NM_TREEVIEW*)lParam)->itemNew), 
						((((NM_TREEVIEW*)lParam)->action & TVE_EXPAND) != 0));
				return 0;

			case TVN_SELCHANGED:
				ProgOnSelChanged(nmhdr->hwndFrom, (NM_TREEVIEW*)lParam);
				return 0;

			case TVN_BEGINDRAG:
				ProgOnBeginDrag(nmhdr->hwndFrom, &(((NM_TREEVIEW*)lParam)->itemNew),
					((NM_TREEVIEW*)lParam)->ptDrag.x, ((NM_TREEVIEW*)lParam)->ptDrag.y, FALSE);
				return 0;

			case TVN_BEGINRDRAG:
				ProgOnBeginDrag(nmhdr->hwndFrom, &(((NM_TREEVIEW*)lParam)->itemNew),
					((NM_TREEVIEW*)lParam)->ptDrag.x, ((NM_TREEVIEW*)lParam)->ptDrag.y, TRUE);
				return 0;

			case TVN_BEGINLABELEDIT:
				return ProgOnBeginLabelEdit(nmhdr->hwndFrom, ((TV_DISPINFO*)lParam)->item.hItem);

			case LVN_BEGINLABELEDIT:
				return SelectOnBeginLabelEdit(nmhdr->hwndFrom, ((LV_DISPINFO*)lParam)->item.iItem);

			case TVN_ENDLABELEDIT:
				ProgOnEndLabelEdit(nmhdr->hwndFrom, &(((TV_DISPINFO*)lParam)->item));
				return TRUE;

			case LVN_ENDLABELEDIT:
				SelectOnEndLabelEdit(nmhdr->hwndFrom, &(((LV_DISPINFO*)lParam)->item));
				return TRUE;

			case TVN_GETDISPINFO:
				ProgOnGetDispInfo(nmhdr->hwndFrom, (TV_DISPINFO*)lParam);
				return 0;

			case LVN_GETDISPINFO:
				SelectOnGetDispInfo((LV_DISPINFO*)lParam);
				return 0;

			case NM_RETURN:
				if (::GetKeyState(VK_MENU) >= 0)
				{
					SelectOnDblClk(nmhdr->hwndFrom);
					ProgOnEnter(nmhdr->hwndFrom);
				}
				return 0;

			case NM_DBLCLK:
				SelectOnDblClk(nmhdr->hwndFrom);
				ProgOnDblClk(nmhdr->hwndFrom);
				return 0;

			case NM_SETFOCUS:
				ProgOnSetFocus(nmhdr->hwndFrom);
				return 0;

			case EN_SELCHANGE:
				{
					EditText::UpdateClipboard();
				}
				return 0;
			}
		}
		goto DEFAULT;

	case 0x04F:						// 0x04F
		goto DEFAULT;

	case WM_INPUTLANGCHANGEREQUEST:	// 0x050
		goto DEFAULT;

	case WM_INPUTLANGCHANGE:		// 0x051
		goto DEFAULT;

	case WM_TCARD:					// 0x052
		goto DEFAULT;

	case WM_HELP:					// 0x053
		goto DEFAULT;

	case WM_USERCHANGED:			// 0x054
		goto DEFAULT;

	case WM_NOTIFYFORMAT:			// 0x055
		goto DEFAULT;

	case 0x056:						// 0x056
		goto DEFAULT;

	case 0x057:						// 0x057
		goto DEFAULT;

	case 0x058:						// 0x058
		goto DEFAULT;

	case 0x059:						// 0x059
		goto DEFAULT;

	case 0x05A:						// 0x05A
		goto DEFAULT;

	case 0x05B:						// 0x05B
		goto DEFAULT;

	case 0x05C:						// 0x05C
		goto DEFAULT;

	case 0x05D:						// 0x05D
		goto DEFAULT;

	case 0x05E:						// 0x05E
		goto DEFAULT;

	case 0x05F:						// 0x05F
		goto DEFAULT;

	case 0x060:						// 0x060
		goto DEFAULT;

	case 0x061:						// 0x061
		goto DEFAULT;

	case 0x062:						// 0x062
		goto DEFAULT;

	case 0x063:						// 0x063
		goto DEFAULT;

	case 0x064:						// 0x064
		goto DEFAULT;

	case 0x065:						// 0x065
		goto DEFAULT;

	case 0x066:						// 0x066
		goto DEFAULT;

	case 0x067:						// 0x067
		goto DEFAULT;

	case 0x068:						// 0x068
		goto DEFAULT;

	case 0x069:						// 0x069
		goto DEFAULT;

	case 0x06A:						// 0x06A
		goto DEFAULT;

	case 0x06B:						// 0x06B
		goto DEFAULT;

	case 0x06C:						// 0x06C
		goto DEFAULT;

	case 0x06D:						// 0x06D
		goto DEFAULT;

	case 0x06E:						// 0x06E
		goto DEFAULT;

	case 0x06F:						// 0x06F
		goto DEFAULT;

	case 0x070:						// 0x070
		goto DEFAULT;

	case 0x071:						// 0x071
		goto DEFAULT;

	case 0x072:						// 0x072
		goto DEFAULT;

	case 0x073:						// 0x073
		goto DEFAULT;

	case 0x074:						// 0x074
		goto DEFAULT;

	case 0x075:						// 0x075
		goto DEFAULT;

	case 0x076:						// 0x076
		goto DEFAULT;

	case 0x077:						// 0x077
		goto DEFAULT;

	case 0x078:						// 0x078
		goto DEFAULT;

	case 0x079:						// 0x079
		goto DEFAULT;

	case 0x07A:						// 0x07A
		goto DEFAULT;

	case WM_CONTEXTMENU:			// 0x07B
		goto DEFAULT;

	case WM_STYLECHANGING:			// 0x07C
		goto DEFAULT;

	case WM_STYLECHANGED:			// 0x07D
		goto DEFAULT;

	case WM_DISPLAYCHANGE:			// 0x07E
		ScreenWidth = ::GetSystemMetrics(SM_CXFULLSCREEN);
		ScreenHeight = ::GetSystemMetrics(SM_CYFULLSCREEN);

		if (ProgMode)
		{
			::ImageList_Destroy(ProgImageList);
			::ImageList_Destroy(ProgImageListMedium);
			::ImageList_Destroy(ProgImageListSmall);
			::ImageList_Destroy(ProgStateList);
			::ImageList_Destroy(ProgStateListMedium);
			::ImageList_Destroy(ProgStateListSmall);

//			Icon16List.NumClear(ICONNUM);
//			Icon32List.NumClear(ICONNUM);

			ProgImageList = ImageList_Create(32, 32, ILC_COLORDDB | ILC_MASK,1,1);
			ProgImageListMedium = ImageList_Create(24, 24, ILC_COLORDDB | ILC_MASK,1,1);
			ProgImageListSmall = ImageList_Create(16, 16, ILC_COLORDDB | ILC_MASK,1,1);
			ProgStateList = IconState.GenerList(FALSE, SI_BIG);
			ProgStateListMedium = IconState.GenerList(FALSE, SI_MEDIUM);
			ProgStateListSmall = IconState.GenerList(FALSE, SI_SMALL);

			BufObj.IconWidth(0);
			BufObj.IconList(BufZoom[BufObjID]);
			BufLoc.IconWidth(0);
			BufLoc.IconList(BufZoom[BufLocID]);
			BufEdi.IconWidth(0);
			BufEdi.IconList(BufZoom[BufEdiID]);
			BufStr.IconWidth(0);
			BufStr.IconList(BufZoom[BufStrID]);
			BufCls.IconWidth(0);
			BufCls.IconList(BufZoom[BufClsID]);
		}

		if (SelectMode)
		{
			::ImageList_Destroy(SelectImageList);
//			IconSelList.NumClear(ICONNUM);
			SelectImageList = ::ImageList_Create(32, 32, ILC_COLORDDB | ILC_MASK, 1, 1);
			::SendMessage(SelectView, LVM_SETIMAGELIST, LVSIL_NORMAL, (LPARAM)SelectImageList);
		}

		goto DEFAULT;

	case WM_GETICON:				// 0x07F
		goto DEFAULT;

	case WM_SETICON:				// 0x080
		goto DEFAULT;

	case WM_NCCREATE:				// 0x081
		goto DEFAULT;

	case WM_NCDESTROY:				// 0x082
		goto DEFAULT;

	case WM_NCCALCSIZE:				// 0x083
		goto DEFAULT;

	case WM_NCHITTEST:				// 0x084
		goto DEFAULT;

	case WM_NCPAINT:				// 0x085
		goto DEFAULT;

	case WM_NCACTIVATE:				// 0x086
		goto DEFAULT;

	case WM_GETDLGCODE:				// 0x087
		goto DEFAULT;

	case 0x088:						// 0x088
		goto DEFAULT;

	case 0x089:						// 0x089
		goto DEFAULT;

	case 0x08A:						// 0x08A
		goto DEFAULT;

	case 0x08B:						// 0x08B
		goto DEFAULT;

	case 0x08C:						// 0x08C
		goto DEFAULT;

	case 0x08D:						// 0x08D
		goto DEFAULT;

	case 0x08E:						// 0x08E
		goto DEFAULT;

	case 0x08F:						// 0x08F
		goto DEFAULT;

	case 0x090:						// 0x090
		goto DEFAULT;

	case 0x091:						// 0x091
		goto DEFAULT;

	case 0x092:						// 0x092
		goto DEFAULT;

	case 0x093:						// 0x093
		goto DEFAULT;

	case 0x094:						// 0x094
		goto DEFAULT;

	case 0x095:						// 0x095
		goto DEFAULT;

	case 0x096:						// 0x096
		goto DEFAULT;

	case 0x097:						// 0x097
		goto DEFAULT;

	case 0x098:						// 0x098
		goto DEFAULT;

	case 0x099:						// 0x099
		goto DEFAULT;

	case 0x09A:						// 0x09A
		goto DEFAULT;

	case 0x09B:						// 0x09B
		goto DEFAULT;

	case 0x09C:						// 0x09C
		goto DEFAULT;

	case 0x09D:						// 0x09D
		goto DEFAULT;

	case 0x09E:						// 0x09E
		goto DEFAULT;

	case 0x09F:						// 0x09F
		goto DEFAULT;

	case WM_NCMOUSEMOVE:			// 0x0A0
		goto DEFAULT;

	case WM_NCLBUTTONDOWN:			// 0x0A1
		goto DEFAULT;

	case WM_NCLBUTTONUP:			// 0x0A2
		goto DEFAULT;

	case WM_NCLBUTTONDBLCLK:		// 0x0A3
		goto DEFAULT;

	case WM_NCRBUTTONDOWN:			// 0x0A4
		goto DEFAULT;

	case WM_NCRBUTTONUP:			// 0x0A5
		goto DEFAULT;

	case WM_NCRBUTTONDBLCLK:		// 0x0A6
		goto DEFAULT;

	case WM_NCMBUTTONDOWN:			// 0x0A7
		goto DEFAULT;

	case WM_NCMBUTTONUP:			// 0x0A8
		goto DEFAULT;

	case WM_NCMBUTTONDBLCLK:		// 0x0A9
		goto DEFAULT;

	case 0x0AA:						// 0x0AA
		goto DEFAULT;

	case 0x0AB:						// 0x0AB
		goto DEFAULT;

	case 0x0AC:						// 0x0AC
		goto DEFAULT;

	case 0x0AD:						// 0x0AD
		goto DEFAULT;

	case 0x0AE:						// 0x0AE
		goto DEFAULT;

	case 0x0AF:						// 0x0AF
		goto DEFAULT;

	case 0x0B0:						// 0x0B0
		goto DEFAULT;

	case 0x0B1:						// 0x0B1
		goto DEFAULT;

	case 0x0B2:						// 0x0B2
		goto DEFAULT;

	case 0x0B3:						// 0x0B3
		goto DEFAULT;

	case 0x0B4:						// 0x0B4
		goto DEFAULT;

	case 0x0B5:						// 0x0B5
		goto DEFAULT;

	case 0x0B6:						// 0x0B6
		goto DEFAULT;

	case 0x0B7:						// 0x0B7
		goto DEFAULT;

	case 0x0B8:						// 0x0B8
		goto DEFAULT;

	case 0x0B9:						// 0x0B9
		goto DEFAULT;

	case 0x0BA:						// 0x0BA
		goto DEFAULT;

	case 0x0BB:						// 0x0BB
		goto DEFAULT;

	case 0x0BC:						// 0x0BC
		goto DEFAULT;

	case 0x0BD:						// 0x0BD
		goto DEFAULT;

	case 0x0BE:						// 0x0BE
		goto DEFAULT;

	case 0x0BF:						// 0x0BF
		goto DEFAULT;

	case 0x0C0:						// 0x0C0
		goto DEFAULT;

	case 0x0C1:						// 0x0C1
		goto DEFAULT;

	case 0x0C2:						// 0x0C2
		goto DEFAULT;

	case 0x0C3:						// 0x0C3
		goto DEFAULT;

	case 0x0C4:						// 0x0C4
		goto DEFAULT;

	case 0x0C5:						// 0x0C5
		goto DEFAULT;

	case 0x0C6:						// 0x0C6
		goto DEFAULT;

	case 0x0C7:						// 0x0C7
		goto DEFAULT;

	case 0x0C8:						// 0x0C8
		goto DEFAULT;

	case 0x0C9:						// 0x0C9
		goto DEFAULT;

	case 0x0CA:						// 0x0CA
		goto DEFAULT;

	case 0x0CB:						// 0x0CB
		goto DEFAULT;

	case 0x0CC:						// 0x0CC
		goto DEFAULT;

	case 0x0CD:						// 0x0CD
		goto DEFAULT;

	case 0x0CE:						// 0x0CE
		goto DEFAULT;

	case 0x0CF:						// 0x0CF
		goto DEFAULT;

	case 0x0D0:						// 0x0D0
		goto DEFAULT;

	case 0x0D1:						// 0x0D1
		goto DEFAULT;

	case 0x0D2:						// 0x0D2
		goto DEFAULT;

	case 0x0D3:						// 0x0D3
		goto DEFAULT;

	case 0x0D4:						// 0x0D4
		goto DEFAULT;

	case 0x0D5:						// 0x0D5
		goto DEFAULT;

	case 0x0D6:						// 0x0D6
		goto DEFAULT;

	case 0x0D7:						// 0x0D7
		goto DEFAULT;

	case 0x0D8:						// 0x0D8
		goto DEFAULT;

	case 0x0D9:						// 0x0D9
		goto DEFAULT;

	case 0x0DA:						// 0x0DA
		goto DEFAULT;

	case 0x0DB:						// 0x0DB
		goto DEFAULT;

	case 0x0DC:						// 0x0DC
		goto DEFAULT;

	case 0x0DD:						// 0x0DD
		goto DEFAULT;

	case 0x0DE:						// 0x0DE
		goto DEFAULT;

	case 0x0DF:						// 0x0DF
		goto DEFAULT;

	case 0x0E0:						// 0x0E0
		goto DEFAULT;

	case 0x0E1:						// 0x0E1
		goto DEFAULT;

	case 0x0E2:						// 0x0E2
		goto DEFAULT;

	case 0x0E3:						// 0x0E3
		goto DEFAULT;

	case 0x0E4:						// 0x0E4
		goto DEFAULT;

	case 0x0E5:						// 0x0E5
		goto DEFAULT;

	case 0x0E6:						// 0x0E6
		goto DEFAULT;

	case 0x0E7:						// 0x0E7
		goto DEFAULT;

	case 0x0E8:						// 0x0E8
		goto DEFAULT;

	case 0x0E9:						// 0x0E9
		goto DEFAULT;

	case 0x0EA:						// 0x0EA
		goto DEFAULT;

	case 0x0EB:						// 0x0EB
		goto DEFAULT;

	case 0x0EC:						// 0x0EC
		goto DEFAULT;

	case 0x0ED:						// 0x0ED
		goto DEFAULT;

	case 0x0EE:						// 0x0EE
		goto DEFAULT;

	case 0x0EF:						// 0x0EF
		goto DEFAULT;

	case 0x0F0:						// 0x0F0
		goto DEFAULT;

	case 0x0F1:						// 0x0F1
		goto DEFAULT;

	case 0x0F2:						// 0x0F2
		goto DEFAULT;

	case 0x0F3:						// 0x0F3
		goto DEFAULT;

	case 0x0F4:						// 0x0F4
		goto DEFAULT;

	case 0x0F5:						// 0x0F5
		goto DEFAULT;

	case 0x0F6:						// 0x0F6
		goto DEFAULT;

	case 0x0F7:						// 0x0F7
		goto DEFAULT;

	case 0x0F8:						// 0x0F8
		goto DEFAULT;

	case 0x0F9:						// 0x0F9
		goto DEFAULT;

	case 0x0FA:						// 0x0FA
		goto DEFAULT;

	case 0x0FB:						// 0x0FB
		goto DEFAULT;

	case 0x0FC:						// 0x0FC
		goto DEFAULT;

	case 0x0FD:						// 0x0FD
		goto DEFAULT;

	case 0x0FE:						// 0x0FE
		goto DEFAULT;

	case 0x0FF:						// 0x0FF
		goto DEFAULT;

	case WM_KEYDOWN:				// 0x100
		goto DEFAULT;

	case WM_KEYUP:					// 0x101
		goto DEFAULT;

	case WM_CHAR:					// 0x102
		goto DEFAULT;

	case WM_DEADCHAR:				// 0x103
		goto DEFAULT;

	case WM_SYSKEYDOWN:				// 0x104
		goto DEFAULT;

	case WM_SYSKEYUP:				// 0x105
		goto DEFAULT;

	case WM_SYSCHAR:				// 0x106
		goto DEFAULT;

	case WM_SYSDEADCHAR:			// 0x107
		goto DEFAULT;

	case 0x108:						// 0x108
		goto DEFAULT;

	case 0x109:						// 0x109
		goto DEFAULT;

	case 0x10A:						// 0x10A
		goto DEFAULT;

	case 0x10B:						// 0x10B
		goto DEFAULT;

	case 0x10C:						// 0x10C
		goto DEFAULT;

	case WM_IME_STARTCOMPOSITION:	// 0x10D
		goto DEFAULT;

	case WM_IME_ENDCOMPOSITION:		// 0x10E
		goto DEFAULT;

	case WM_IME_COMPOSITION:		// 0x10F
		goto DEFAULT;

	case WM_INITDIALOG:				// 0x110
		goto DEFAULT;

	case WM_COMMAND:				// 0x111
		switch (HIWORD(wParam))
		{
		case 0:
		case 1:
			switch (LOWORD(wParam))
			{
			case IDN_OPEN:
				if (SelectMode)
				{
					SelectOnOpen();
					return 0;
				}
				goto JMPCLOSE;

			case IDN_CLOSE:
JMPCLOSE:
				if (ProgMode)
				{
					if (TestModi())
					{
						ResModi();
						ProgTerm();
						SelectInit();
					}
				}
				return 0;

			case IDN_SAVE:
				ProgFile::SaveMaxi();
				return 0;

			case IDN_SAVEN:
				ProgFile::SaveMini();
				return 0;

			case IDN_SAVE_AS:
				ProgFile::SaveAs();
				return 0;

			case IDN_SAVE_REL:
				ProgFile::SaveRel();
				return 0;

			case IDN_START:
				SelectOnStart();
				ProgOnStart();
				return 0;

			case IDN_EXIT:
				{
					if (!TestModi()) return 0;

					ResModi();
			
					WINDOWPLACEMENT wp;
					wp.length = sizeof(WINDOWPLACEMENT);
					::GetWindowPlacement(MainFrame, &wp);
					MainLeft = (wp.rcNormalPosition.left*Scale + ScreenWidth/2)/ScreenWidth;
					MainTop = (wp.rcNormalPosition.top*Scale + ScreenHeight/2)/ScreenHeight;
					MainWidth = ((wp.rcNormalPosition.right - wp.rcNormalPosition.left)*Scale + ScreenWidth/2)/ScreenWidth;
					MainHeight = ((wp.rcNormalPosition.bottom - wp.rcNormalPosition.top)*Scale + ScreenHeight/2)/ScreenHeight;

					Maximized = ::IsZoomed(MainFrame);

					AktPrevWindow();
				};

				if (RepaintTimer)
				{
					::KillTimer(MainFrame, RepaintTimer);
					RepaintTimer = 0;
				}
				RepaintTimerN = 0;
				RepaintTimerN2 = 0;

				PostQuitMessage(0);
				return 0;

			case IDN_DITHER:
				Dither = !Dither;
				CheckCommand(IDN_DITHER, Dither);
				{
					PreviewPicNul();
					PreviewStop();
				}
				return 0;

			case IDN_SHORTNAME:
				ShortName = !ShortName;
				CheckCommand(IDN_SHORTNAME, ShortName);
				return 0;

			case IDN_NEW:
				SelectOnNew();
				return 0;

			case IDN_NEWCOPY:
				SelectOnNewCopy();
				return 0;

			case IDN_RENAME:
				SelectOnRename();
				return 0;

			case IDN_PATH:
				SelectOnPath();
				return 0;

			case IDN_DELETE:
				SelectOnDelete();
				return 0;

			case IDN_HELP:
				OnHelp();
				return 0;

			case IDN_HELPT:
				InitHelpName();
				::WinHelp(MainFrame, HelpFileName, HELP_CONTEXT, 1);
				return 0;

			case IDN_HELPF:
				InitHelpName();
				::WinHelp(MainFrame, HelpFileName, HELP_FINDER, 0);
				return 0;

			case IDN_ABOUT:
				OnAboutBox();
				return 0;

			case IDN_IN:
				ProgOnZoomIn();
				return 0;

			case IDN_OUT:
				ProgOnZoomOut();
				return 0;

			case IDN_LOCK:
				ProgOnLock();
				return 0;

			case IDN_OFF:
				ProgOnOff();
				return 0;

			case IDN_DEF_NAME:
				ProgOnDefName();
				return 0;

			case IDN_DEF_ICON:
				ProgOnDefIcon();
				return 0;

			case IDN_EDIT_NAME:
				ProgOnEditName();
				return 0;

			case IDN_EDIT_ICON:
				ProgOnEditIcon();
				return 0;

			case IDN_REF_NEXT:
				ProgOnRefNext();
				return 0;

			case IDN_REF_PREV:
				ProgOnRefPrev();
				return 0;

			case IDN_REF_DEF:
				ProgOnRefDef();
				return 0;

			case IDN_FIND:
				ProgOnFind();
				return 0;

			case IDN_FIND_NEXT:
				ProgOnFindNext();
				return 0;

			case IDN_FIND_PREV:
				ProgOnFindPrev();
				return 0;

			case IDN_ALL:
				ProgOnAll();
				return 0;

			case IDN_CLEAR:
				ProgOnDelete();
				SelectOnDelete();
				return 0;

			case IDN_COPY:			// zachovat poøadí - v Select se otevøe editace do Prog
				ProgOnCopy();
				SelectOnNewCopy();
				return 0;

			case IDN_CUT:
				ProgOnCut();
				SelectOnCut();
				return 0;

			case IDN_PASTE:
				ProgOnPaste();
				SelectOnPaste();
				return 0;

			case IDN_UNDO:
				ProgOnUndo();
				SelectOnUndo();
				return 0;

			case IDN_REDO:
				ProgOnRedo();
				return 0;

			case IDN_DIMEN:
			case IDN_SPRITEPAR:
				ProgOnDimen();
				return 0;

			case IDN_TEST:
				EditSprite::OnTest();
				return 0;

			case IDN_EDITBLOK:
				switch(EditMode)
				{
				case BufPicID:
				case BufIcoID:
					EditIcon::SetMode(IDN_EDITBLOK);
					break;
				}
				return 0;

			case EDITLOGID:
				EditLog::Switch();
				return 0;

			case IDN_RASTR:
				ProgOnRastr();
				return 0;

			case IDN_PLAY:
				ProgOnPlay();
				return 0;

			case IDN_PAUSE:
				ProgOnPause();
				return 0;

			case IDN_STOP:
				ProgOnStop();
				return 0;

			case IDN_LOOP:
				ProgOnLoop();
				return 0;

			case IDN_RECORD:
				ProgOnRecord();
				return 0;

			case IDN_REW:
				ProgOnRew();
				return 0;

			case IDN_FRW:
				ProgOnFrw();
				return 0;

			case IDN_HIST_PREV:
				HistEdit::OnPrev();
				return 0;

			case IDN_HIST_NEXT:
				HistEdit::OnNext();
				return 0;

			case IDN_OTISK:
				EditIcon::OnOtisk();
				return 0;

			case IDN_EDITMAP:
				EditMap::OnEditMap();
				return 0;

			case IDN_FILLMAP:
				EditMap::OnFillMap();
				return 0;

//			case IDN_EXPORT:
//				ProgExp::OnExport();
//				return 0;

//			case IDN_IMPORT:
//				ProgExp::OnImport();
//				return 0;

			case IDN_JAZYKAUT:
				if (JazykUser == JAZYKAUT)
				{
					SetJazyk(Jazyk);
				}
				else
				{
					SetJazyk(JAZYKAUT);
				}
				return 0;

			case IDN_JAZYKPRE:
				SetJazyk(JazykPrev);
				return 0;

// ----------------------------------------------
// jazyky
			case IDN_JAZYKBUL:
				SetJazyk(JAZYKBUL);
				return 0;

			case IDN_JAZYKCZ:
				SetJazyk(JAZYKCZ);
				return 0;

			case IDN_JAZYKDAN:
				SetJazyk(JAZYKDAN);
				return 0;

			case IDN_JAZYKGER:
				SetJazyk(JAZYKGER);
				return 0;

			case IDN_JAZYKREC:
				SetJazyk(JAZYKREC);
				return 0;

			case IDN_JAZYKENG:
				SetJazyk(JAZYKENG);
				return 0;

			case IDN_JAZYKSPA:
				SetJazyk(JAZYKSPA);
				return 0;

			case IDN_JAZYKFIN:
				SetJazyk(JAZYKFIN);
				return 0;

			case IDN_JAZYKFRA:
				SetJazyk(JAZYKFRA);
				return 0;

			case IDN_JAZYKMAD:
				SetJazyk(JAZYKMAD);
				return 0;

			case IDN_JAZYKISL:
				SetJazyk(JAZYKISL);
				return 0;

			case IDN_JAZYKITA:
				SetJazyk(JAZYKITA);
				return 0;

			case IDN_JAZYKHOL:
				SetJazyk(JAZYKHOL);
				return 0;

			case IDN_JAZYKNOR:
				SetJazyk(JAZYKNOR);
				return 0;

			case IDN_JAZYKPOL:
				SetJazyk(JAZYKPOL);
				return 0;

			case IDN_JAZYKPOR:
				SetJazyk(JAZYKPOR);
				return 0;

			case IDN_JAZYKRUM:
				SetJazyk(JAZYKRUM);
				return 0;

			case IDN_JAZYKRUS:
				SetJazyk(JAZYKRUS);
				return 0;

			case IDN_JAZYKSRB:
				SetJazyk(JAZYKSRB);
				return 0;

			case IDN_JAZYKSLO:
				SetJazyk(JAZYKSLO);
				return 0;

			case IDN_JAZYKALB:
				SetJazyk(JAZYKALB);
				return 0;

			case IDN_JAZYKSWE:
				SetJazyk(JAZYKSWE);
				return 0;

			case IDN_JAZYKTUR:
				SetJazyk(JAZYKTUR);
				return 0;

			case IDN_JAZYKVIE:
				SetJazyk(JAZYKVIE);
				return 0;

			case IDN_JAZYKARA:
				SetJazyk(JAZYKARA);
				return 0;

			case IDN_JAZYKHEB:
				SetJazyk(JAZYKHEB);
				return 0;

// ----------------------------------------------

			case IDN_SETUP1:
			case IDN_SETUP2:
			case IDN_SETUP3:
			case IDN_SETUP4:
			case IDN_SETUP5:
				ProgOnSetup(LOWORD(wParam));
				return 0;

			case IDN_YFLIP:
			case IDN_XFLIP:
			case IDN_LROT:
			case IDN_RROT:
			case IDN_MUL2:
			case IDN_DIV2:
			case IDN_MULDIV:
			case IDN_XCOL:
			case IDN_SCOL:
				switch(EditMode)
				{
				case BufPicID:
				case BufIcoID:
					EditIcon::EditBlok(LOWORD(wParam));
					break;
				}
				return 0;

			case IDN_PEN:
			case IDN_LINE:
			case IDN_BOX:
			case IDN_FILLBOX:
			case IDN_CIRCLE:
			case IDN_FILLCIRCLE:
			case IDN_ELLIPSE:
			case IDN_FILLELLIPSE:
			case IDN_SPHERE:
			case IDN_SELECT:
			case IDN_SPRAY:
			case IDN_PIPE:
			case IDN_FILL:
			case IDN_TEXT:
			case IDN_WIDTH1:
			case IDN_WIDTH2:
			case IDN_WIDTH3:
			case IDN_WIDTH4:
			case IDN_WIDTH5:
				switch(EditMode)
				{
				case BufPicID:
				case BufIcoID:
					EditIcon::SetMode(LOWORD(wParam));
					break;
				}
				return 0;

			case IDN_MAP:
			case IDN_SWC1:
			case IDN_SWC2:
			case IDN_SWC3:
			case IDN_SWC4:
			case IDN_SWC5:
			case IDN_SWCN1:
			case IDN_SWCN2:
			case IDN_SWCN3:
				EditMap::SetMode(LOWORD(wParam));
				return 0;
			}
			break;

		case CBN_SELENDOK:
			{
				HWND wnd = (HWND)lParam;
				int index = ::SendMessage(wnd, CB_GETCURSEL, 0, 0);
				int data = ::SendMessage(wnd, CB_GETITEMDATA, index, 0);

				if (((DWORD)index < 30) &&
					(index != CB_ERR) && 
					((DWORD)data < 32000) &&
					(data != CB_ERR))
				{
					switch (LOWORD(wParam))
					{
					case IDD_WIDTH:
					case IDD_PEN:
						switch(EditMode)
						{
						case BufPicID:
						case BufIcoID:
							EditIcon::SetMode(data);
							break;
						}
						break;

					case IDD_MAP:
						EditMap::SetMode(data);
						break;
					}
				}
				ProgSetFocus();
			}
			break;

		case EN_CHANGE:
			{
				HWND wnd = (HWND)lParam;

				if ((wnd == EditNumWnd) && ProgMode)
				{
					EditNum::OnChange();		// poøadí je nutné!
					ProgUpdateUndoRedo();
				}

				if ((wnd == EditTextWnd) && ProgMode)
				{
					EditText::OnChange();		// poøadí je nutné!
					ProgUpdateUndoRedo();
				}

				if ((wnd == EditMapNumWnd) && ProgMode)
				{
					EditMap::OnChangeEditSwcNum();
					ProgUpdateUndoRedo();
				}
			}
			break;
		}
		goto DEFAULT;

	case WM_SYSCOMMAND:				// 0x112
		goto DEFAULT;

	case WM_TIMER:					// 0x113
		if (wParam == RepaintTimerID)
		{
			if (RepaintTimerN > 0)
			{
				RepaintTimerN--;
				if (RepaintTimerN == 0)
				{
					TimeRepaintNow();
				}
				return 0;
			}

			if (RepaintTimerN2 > 0)
			{
				RepaintTimerN2--;
				if (RepaintTimerN2 == 0)
				{
					TimeRepaintNow();
				}
				return 0;
			}
		}

		if (SelectOnTimer(wParam) || ProgOnTimer(wParam)) return 0;
		goto DEFAULT;

	case WM_HSCROLL:				// 0x114
		if (((HWND)lParam == HScroll) && (HScrollDisp))
		{
			ProgOnHScroll(LOWORD(wParam), HIWORD(wParam));
			return 0;
		}
		goto DEFAULT;

	case WM_VSCROLL:				// 0x115
		if (((HWND)lParam == VScroll) && (VScrollDisp))
		{
			ProgOnVScroll(LOWORD(wParam), HIWORD(wParam));
			return 0;
		}
		goto DEFAULT;

	case WM_INITMENU:				// 0x116
		goto DEFAULT;

	case WM_INITMENUPOPUP:			// 0x117
		goto DEFAULT;

	case 0x118:						// 0x118
		goto DEFAULT;

	case 0x119:						// 0x119
		goto DEFAULT;

	case 0x11A:						// 0x11A
		goto DEFAULT;

	case 0x11B:						// 0x11B
		goto DEFAULT;

	case 0x11C:						// 0x11C
		goto DEFAULT;

	case 0x11D:						// 0x11D
		goto DEFAULT;

	case 0x11E:						// 0x11E
		goto DEFAULT;

	case WM_MENUSELECT:				// 0x11F
		if ((LOWORD(wParam) != 0) && ((HIWORD(wParam) & MF_POPUP) == 0))
		{
			CText txt;
			txt.Load(LOWORD(wParam));
			txt.Delete(txt.Find(_T('\n')));
			SetStatusText(txt);
		}
		else
		{
			SetStatusText(EmptyText);
		}
		return 0;

	case WM_MENUCHAR:				// 0x120
		{
			HMENU menu = (HMENU)lParam;				// menu
			WORD chr = LOWORD(wParam);				// znak z klávesnice

			CText txt;
			txt = (TCHAR)chr;
			txt.UpperCase();						// konverze znaku na velké písmeno
			chr = txt.Get(0);

			WPARAM kod = MNC_IGNORE;				// pøednastaveno - ignorovat znak
			WPARAM inx = 0;

			if (chr != 0)
			{
				for (int i = 0; i < MenuAktItemsN; i++)
				{
					if ((menu == MenuAktItems[i].menu) &&
						(chr == MenuAktItems[i].key))
					{
						kod = MNC_EXECUTE;
						inx = MenuAktItems[i].inx;		// èíslo menu
						break;
					}
				}
			}
			return MAKELONG(inx , kod);
		}
		goto DEFAULT;

	case WM_ENTERIDLE:				// 0x121
		goto DEFAULT;

	case 0x122:						// 0x122
		goto DEFAULT;

	case 0x123:						// 0x123
		goto DEFAULT;

	case 0x124:						// 0x124
		goto DEFAULT;

	case 0x125:						// 0x125
		goto DEFAULT;

	case 0x126:						// 0x126
		goto DEFAULT;

	case 0x127:						// 0x127
		goto DEFAULT;

	case 0x128:						// 0x128
		goto DEFAULT;

	case 0x129:						// 0x129
		goto DEFAULT;

	case 0x12A:						// 0x12A
		goto DEFAULT;

	case 0x12B:						// 0x12B
		goto DEFAULT;

	case 0x12C:						// 0x12C
		goto DEFAULT;

	case 0x12D:						// 0x12D
		goto DEFAULT;

	case 0x12E:						// 0x12E
		goto DEFAULT;

	case 0x12F:						// 0x12F
		goto DEFAULT;

	case 0x130:						// 0x130
		goto DEFAULT;

	case 0x131:						// 0x131
		goto DEFAULT;

	case WM_CTLCOLORMSGBOX:			// 0x132
		goto DEFAULT;

	case WM_CTLCOLOREDIT:			// 0x133
		goto DEFAULT;

	case WM_CTLCOLORLISTBOX:		// 0x134
		goto DEFAULT;

	case WM_CTLCOLORBTN:			// 0x135
		goto DEFAULT;

	case WM_CTLCOLORDLG:			// 0x136
		goto DEFAULT;

	case WM_CTLCOLORSCROLLBAR:		// 0x137
		goto DEFAULT;

	case WM_CTLCOLORSTATIC:			// 0x138
		goto DEFAULT;

	case 0x139:						// 0x139
		goto DEFAULT;

	case 0x13A:						// 0x13A
		goto DEFAULT;

	case 0x13B:						// 0x13B
		goto DEFAULT;

	case 0x13C:						// 0x13C
		goto DEFAULT;

	case 0x13D:						// 0x13D
		goto DEFAULT;

	case 0x13E:						// 0x13E
		goto DEFAULT;

	case 0x13F:						// 0x13F
		goto DEFAULT;

	case 0x140:						// 0x140
		goto DEFAULT;

	case 0x141:						// 0x141
		goto DEFAULT;

	case 0x142:						// 0x142
		goto DEFAULT;

	case 0x143:						// 0x143
		goto DEFAULT;

	case 0x144:						// 0x144
		goto DEFAULT;

	case 0x145:						// 0x145
		goto DEFAULT;

	case 0x146:						// 0x146
		goto DEFAULT;

	case 0x147:						// 0x147
		goto DEFAULT;

	case 0x148:						// 0x148
		goto DEFAULT;

	case 0x149:						// 0x149
		goto DEFAULT;

	case 0x14A:						// 0x14A
		goto DEFAULT;

	case 0x14B:						// 0x14B
		goto DEFAULT;

	case 0x14C:						// 0x14C
		goto DEFAULT;

	case 0x14D:						// 0x14D
		goto DEFAULT;

	case 0x14E:						// 0x14E
		goto DEFAULT;

	case 0x14F:						// 0x14F
		goto DEFAULT;

	case 0x150:						// 0x150
		goto DEFAULT;

	case 0x151:						// 0x151
		goto DEFAULT;

	case 0x152:						// 0x152
		goto DEFAULT;

	case 0x153:						// 0x153
		goto DEFAULT;

	case 0x154:						// 0x154
		goto DEFAULT;

	case 0x155:						// 0x155
		goto DEFAULT;

	case 0x156:						// 0x156
		goto DEFAULT;

	case 0x157:						// 0x157
		goto DEFAULT;

	case 0x158:						// 0x158
		goto DEFAULT;

	case 0x159:						// 0x159
		goto DEFAULT;

	case 0x15A:						// 0x15A
		goto DEFAULT;

	case 0x15B:						// 0x15B
		goto DEFAULT;

	case 0x15C:						// 0x15C
		goto DEFAULT;

	case 0x15D:						// 0x15D
		goto DEFAULT;

	case 0x15E:						// 0x15E
		goto DEFAULT;

	case 0x15F:						// 0x15F
		goto DEFAULT;

	case 0x160:						// 0x160
		goto DEFAULT;

	case 0x161:						// 0x161
		goto DEFAULT;

	case 0x162:						// 0x162
		goto DEFAULT;

	case 0x163:						// 0x163
		goto DEFAULT;

	case 0x164:						// 0x164
		goto DEFAULT;

	case 0x165:						// 0x165
		goto DEFAULT;

	case 0x166:						// 0x166
		goto DEFAULT;

	case 0x167:						// 0x167
		goto DEFAULT;

	case 0x168:						// 0x168
		goto DEFAULT;

	case 0x169:						// 0x169
		goto DEFAULT;

	case 0x16A:						// 0x16A
		goto DEFAULT;

	case 0x16B:						// 0x16B
		goto DEFAULT;

	case 0x16C:						// 0x16C
		goto DEFAULT;

	case 0x16D:						// 0x16D
		goto DEFAULT;

	case 0x16E:						// 0x16E
		goto DEFAULT;

	case 0x16F:						// 0x16F
		goto DEFAULT;

	case 0x170:						// 0x170
		goto DEFAULT;

	case 0x171:						// 0x171
		goto DEFAULT;

	case 0x172:						// 0x172
		goto DEFAULT;

	case 0x173:						// 0x173
		goto DEFAULT;

	case 0x174:						// 0x174
		goto DEFAULT;

	case 0x175:						// 0x175
		goto DEFAULT;

	case 0x176:						// 0x176
		goto DEFAULT;

	case 0x177:						// 0x177
		goto DEFAULT;

	case 0x178:						// 0x178
		goto DEFAULT;

	case 0x179:						// 0x179
		goto DEFAULT;

	case 0x17A:						// 0x17A
		goto DEFAULT;

	case 0x17B:						// 0x17B
		goto DEFAULT;

	case 0x17C:						// 0x17C
		goto DEFAULT;

	case 0x17D:						// 0x17D
		goto DEFAULT;

	case 0x17E:						// 0x17E
		goto DEFAULT;

	case 0x17F:						// 0x17F
		goto DEFAULT;

	case 0x180:						// 0x180
		goto DEFAULT;

	case 0x181:						// 0x181
		goto DEFAULT;

	case 0x182:						// 0x182
		goto DEFAULT;

	case 0x183:						// 0x183
		goto DEFAULT;

	case 0x184:						// 0x184
		goto DEFAULT;

	case 0x185:						// 0x185
		goto DEFAULT;

	case 0x186:						// 0x186
		goto DEFAULT;

	case 0x187:						// 0x187
		goto DEFAULT;

	case 0x188:						// 0x188
		goto DEFAULT;

	case 0x189:						// 0x189
		goto DEFAULT;

	case 0x18A:						// 0x18A
		goto DEFAULT;

	case 0x18B:						// 0x18B
		goto DEFAULT;

	case 0x18C:						// 0x18C
		goto DEFAULT;

	case 0x18D:						// 0x18D
		goto DEFAULT;

	case 0x18E:						// 0x18E
		goto DEFAULT;

	case 0x18F:						// 0x18F
		goto DEFAULT;

	case 0x190:						// 0x190
		goto DEFAULT;

	case 0x191:						// 0x191
		goto DEFAULT;

	case 0x192:						// 0x192
		goto DEFAULT;

	case 0x193:						// 0x193
		goto DEFAULT;

	case 0x194:						// 0x194
		goto DEFAULT;

	case 0x195:						// 0x195
		goto DEFAULT;

	case 0x196:						// 0x196
		goto DEFAULT;

	case 0x197:						// 0x197
		goto DEFAULT;

	case 0x198:						// 0x198
		goto DEFAULT;

	case 0x199:						// 0x199
		goto DEFAULT;

	case 0x19A:						// 0x19A
		goto DEFAULT;

	case 0x19B:						// 0x19B
		goto DEFAULT;

	case 0x19C:						// 0x19C
		goto DEFAULT;

	case 0x19D:						// 0x19D
		goto DEFAULT;

	case 0x19E:						// 0x19E
		goto DEFAULT;

	case 0x19F:						// 0x19F
		goto DEFAULT;

	case 0x1A0:						// 0x1A0
		goto DEFAULT;

	case 0x1A1:						// 0x1A1
		goto DEFAULT;

	case 0x1A2:						// 0x1A2
		goto DEFAULT;

	case 0x1A3:						// 0x1A3
		goto DEFAULT;

	case 0x1A4:						// 0x1A4
		goto DEFAULT;

	case 0x1A5:						// 0x1A5
		goto DEFAULT;

	case 0x1A6:						// 0x1A6
		goto DEFAULT;

	case 0x1A7:						// 0x1A7
		goto DEFAULT;

	case 0x1A8:						// 0x1A8
		goto DEFAULT;

	case 0x1A9:						// 0x1A9
		goto DEFAULT;

	case 0x1AA:						// 0x1AA
		goto DEFAULT;

	case 0x1AB:						// 0x1AB
		goto DEFAULT;

	case 0x1AC:						// 0x1AC
		goto DEFAULT;

	case 0x1AD:						// 0x1AD
		goto DEFAULT;

	case 0x1AE:						// 0x1AE
		goto DEFAULT;

	case 0x1AF:						// 0x1AF
		goto DEFAULT;

	case 0x1B0:						// 0x1B0
		goto DEFAULT;

	case 0x1B1:						// 0x1B1
		goto DEFAULT;

	case 0x1B2:						// 0x1B2
		goto DEFAULT;

	case 0x1B3:						// 0x1B3
		goto DEFAULT;

	case 0x1B4:						// 0x1B4
		goto DEFAULT;

	case 0x1B5:						// 0x1B5
		goto DEFAULT;

	case 0x1B6:						// 0x1B6
		goto DEFAULT;

	case 0x1B7:						// 0x1B7
		goto DEFAULT;

	case 0x1B8:						// 0x1B8
		goto DEFAULT;

	case 0x1B9:						// 0x1B9
		goto DEFAULT;

	case 0x1BA:						// 0x1BA
		goto DEFAULT;

	case 0x1BB:						// 0x1BB
		goto DEFAULT;

	case 0x1BC:						// 0x1BC
		goto DEFAULT;

	case 0x1BD:						// 0x1BD
		goto DEFAULT;

	case 0x1BE:						// 0x1BE
		goto DEFAULT;

	case 0x1BF:						// 0x1BF
		goto DEFAULT;

	case 0x1C0:						// 0x1C0
		goto DEFAULT;

	case 0x1C1:						// 0x1C1
		goto DEFAULT;

	case 0x1C2:						// 0x1C2
		goto DEFAULT;

	case 0x1C3:						// 0x1C3
		goto DEFAULT;

	case 0x1C4:						// 0x1C4
		goto DEFAULT;

	case 0x1C5:						// 0x1C5
		goto DEFAULT;

	case 0x1C6:						// 0x1C6
		goto DEFAULT;

	case 0x1C7:						// 0x1C7
		goto DEFAULT;

	case 0x1C8:						// 0x1C8
		goto DEFAULT;

	case 0x1C9:						// 0x1C9
		goto DEFAULT;

	case 0x1CA:						// 0x1CA
		goto DEFAULT;

	case 0x1CB:						// 0x1CB
		goto DEFAULT;

	case 0x1CC:						// 0x1CC
		goto DEFAULT;

	case 0x1CD:						// 0x1CD
		goto DEFAULT;

	case 0x1CE:						// 0x1CE
		goto DEFAULT;

	case 0x1CF:						// 0x1CF
		goto DEFAULT;

	case 0x1D0:						// 0x1D0
		goto DEFAULT;

	case 0x1D1:						// 0x1D1
		goto DEFAULT;

	case 0x1D2:						// 0x1D2
		goto DEFAULT;

	case 0x1D3:						// 0x1D3
		goto DEFAULT;

	case 0x1D4:						// 0x1D4
		goto DEFAULT;

	case 0x1D5:						// 0x1D5
		goto DEFAULT;

	case 0x1D6:						// 0x1D6
		goto DEFAULT;

	case 0x1D7:						// 0x1D7
		goto DEFAULT;

	case 0x1D8:						// 0x1D8
		goto DEFAULT;

	case 0x1D9:						// 0x1D9
		goto DEFAULT;

	case 0x1DA:						// 0x1DA
		goto DEFAULT;

	case 0x1DB:						// 0x1DB
		goto DEFAULT;

	case 0x1DC:						// 0x1DC
		goto DEFAULT;

	case 0x1DD:						// 0x1DD
		goto DEFAULT;

	case 0x1DE:						// 0x1DE
		goto DEFAULT;

	case 0x1DF:						// 0x1DF
		goto DEFAULT;

	case 0x1E0:						// 0x1E0
		goto DEFAULT;

	case 0x1E1:						// 0x1E1
		goto DEFAULT;

	case 0x1E2:						// 0x1E2
		goto DEFAULT;

	case 0x1E3:						// 0x1E3
		goto DEFAULT;

	case 0x1E4:						// 0x1E4
		goto DEFAULT;

	case 0x1E5:						// 0x1E5
		goto DEFAULT;

	case 0x1E6:						// 0x1E6
		goto DEFAULT;

	case 0x1E7:						// 0x1E7
		goto DEFAULT;

	case 0x1E8:						// 0x1E8
		goto DEFAULT;

	case 0x1E9:						// 0x1E9
		goto DEFAULT;

	case 0x1EA:						// 0x1EA
		goto DEFAULT;

	case 0x1EB:						// 0x1EB
		goto DEFAULT;

	case 0x1EC:						// 0x1EC
		goto DEFAULT;

	case 0x1ED:						// 0x1ED
		goto DEFAULT;

	case 0x1EE:						// 0x1EE
		goto DEFAULT;

	case 0x1EF:						// 0x1EF
		goto DEFAULT;

	case 0x1F0:						// 0x1F0
		goto DEFAULT;

	case 0x1F1:						// 0x1F1
		goto DEFAULT;

	case 0x1F2:						// 0x1F2
		goto DEFAULT;

	case 0x1F3:						// 0x1F3
		goto DEFAULT;

	case 0x1F4:						// 0x1F4
		goto DEFAULT;

	case 0x1F5:						// 0x1F5
		goto DEFAULT;

	case 0x1F6:						// 0x1F6
		goto DEFAULT;

	case 0x1F7:						// 0x1F7
		goto DEFAULT;

	case 0x1F8:						// 0x1F8
		goto DEFAULT;

	case 0x1F9:						// 0x1F9
		goto DEFAULT;

	case 0x1FA:						// 0x1FA
		goto DEFAULT;

	case 0x1FB:						// 0x1FB
		goto DEFAULT;

	case 0x1FC:						// 0x1FC
		goto DEFAULT;

	case 0x1FD:						// 0x1FD
		goto DEFAULT;

	case 0x1FE:						// 0x1FE
		goto DEFAULT;

	case 0x1FF:						// 0x1FF
		goto DEFAULT;

	case WM_MOUSEMOVE:				// 0x200
		ProgOnMouseMove(wParam, MAKEPOINTS(lParam).x, MAKEPOINTS(lParam).y);
		goto DEFAULT;

	case WM_LBUTTONDOWN:			// 0x201
		goto DEFAULT;

	case WM_LBUTTONUP:				// 0x202
		goto DEFAULT;

	case WM_LBUTTONDBLCLK:			// 0x203
		goto DEFAULT;

	case WM_RBUTTONDOWN:			// 0x204
		goto DEFAULT;

	case WM_RBUTTONUP:				// 0x205
		goto DEFAULT;

	case WM_RBUTTONDBLCLK:			// 0x206
		goto DEFAULT;

	case WM_MBUTTONDOWN:			// 0x207
		goto DEFAULT;

	case WM_MBUTTONUP:				// 0x208
		goto DEFAULT;

	case WM_MBUTTONDBLCLK:			// 0x209
		goto DEFAULT;

#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x20A
#endif
	case WM_MOUSEWHEEL:				// 0x20A
		goto DEFAULT;

	case 0x20B:						// 0x20B
		goto DEFAULT;

	case 0x20C:						// 0x20C
		goto DEFAULT;

	case 0x20D:						// 0x20D
		goto DEFAULT;

	case 0x20E:						// 0x20E
		goto DEFAULT;

	case 0x20F:						// 0x20F
		goto DEFAULT;

	case WM_PARENTNOTIFY:			// 0x210
		goto DEFAULT;

	case WM_ENTERMENULOOP:			// 0x211
		goto DEFAULT;

	case WM_EXITMENULOOP:			// 0x212
		goto DEFAULT;

	case WM_NEXTMENU:				// 0x213
		goto DEFAULT;

	case WM_SIZING:					// 0x214
		goto DEFAULT;

	case WM_CAPTURECHANGED:			// 0x215
		goto DEFAULT;

	case WM_MOVING:					// 0x216
		goto DEFAULT;

	case 0x217:						// 0x217
		goto DEFAULT;

	case WM_POWERBROADCAST:			// 0x218
		goto DEFAULT;

	case WM_DEVICECHANGE:			// 0x219
		goto DEFAULT;

	case 0x21A:						// 0x21A
		goto DEFAULT;

	case 0x21B:						// 0x21B
		goto DEFAULT;

	case 0x21C:						// 0x21C
		goto DEFAULT;

	case 0x21D:						// 0x21D
		goto DEFAULT;

	case 0x21E:						// 0x21E
		goto DEFAULT;

	case 0x21F:						// 0x21F
		goto DEFAULT;

	case WM_MDICREATE:				// 0x220
		goto DEFAULT;

	case WM_MDIDESTROY:				// 0x221
		goto DEFAULT;

	case WM_MDIACTIVATE:			// 0x222
		goto DEFAULT;

	case WM_MDIRESTORE:				// 0x223
		goto DEFAULT;

	case WM_MDINEXT:				// 0x224
		goto DEFAULT;

	case WM_MDIMAXIMIZE:			// 0x225
		goto DEFAULT;

	case WM_MDITILE:				// 0x226
		goto DEFAULT;

	case WM_MDICASCADE:				// 0x227
		goto DEFAULT;

	case WM_MDIICONARRANGE:			// 0x228
		goto DEFAULT;

	case WM_MDIGETACTIVE:			// 0x229
		goto DEFAULT;

	case 0x22A:						// 0x22A
		goto DEFAULT;

	case 0x22B:						// 0x22B
		goto DEFAULT;

	case 0x22C:						// 0x22C
		goto DEFAULT;

	case 0x22D:						// 0x22D
		goto DEFAULT;

	case 0x22E:						// 0x22E
		goto DEFAULT;

	case 0x22F:						// 0x22F
		goto DEFAULT;

	case WM_MDISETMENU:				// 0x230
		goto DEFAULT;

	case WM_ENTERSIZEMOVE:			// 0x231
		goto DEFAULT;

	case WM_EXITSIZEMOVE:			// 0x232
		goto DEFAULT;

	case WM_DROPFILES:				// 0x233
		goto DEFAULT;

	case WM_MDIREFRESHMENU:			// 0x234
		goto DEFAULT;

	case 0x235:						// 0x235
		goto DEFAULT;

	case 0x236:						// 0x236
		goto DEFAULT;

	case 0x237:						// 0x237
		goto DEFAULT;

	case 0x238:						// 0x238
		goto DEFAULT;

	case 0x239:						// 0x239
		goto DEFAULT;

	case 0x23A:						// 0x23A
		goto DEFAULT;

	case 0x23B:						// 0x23B
		goto DEFAULT;

	case 0x23C:						// 0x23C
		goto DEFAULT;

	case 0x23D:						// 0x23D
		goto DEFAULT;

	case 0x23E:						// 0x23E
		goto DEFAULT;

	case 0x23F:						// 0x23F
		goto DEFAULT;

	case 0x240:						// 0x240
		goto DEFAULT;

	case 0x241:						// 0x241
		goto DEFAULT;

	case 0x242:						// 0x242
		goto DEFAULT;

	case 0x243:						// 0x243
		goto DEFAULT;

	case 0x244:						// 0x244
		goto DEFAULT;

	case 0x245:						// 0x245
		goto DEFAULT;

	case 0x246:						// 0x246
		goto DEFAULT;

	case 0x247:						// 0x247
		goto DEFAULT;

	case 0x248:						// 0x248
		goto DEFAULT;

	case 0x249:						// 0x249
		goto DEFAULT;

	case 0x24A:						// 0x24A
		goto DEFAULT;

	case 0x24B:						// 0x24B
		goto DEFAULT;

	case 0x24C:						// 0x24C
		goto DEFAULT;

	case 0x24D:						// 0x24D
		goto DEFAULT;

	case 0x24E:						// 0x24E
		goto DEFAULT;

	case 0x24F:						// 0x24F
		goto DEFAULT;

	case 0x250:						// 0x250
		goto DEFAULT;

	case 0x251:						// 0x251
		goto DEFAULT;

	case 0x252:						// 0x252
		goto DEFAULT;

	case 0x253:						// 0x253
		goto DEFAULT;

	case 0x254:						// 0x254
		goto DEFAULT;

	case 0x255:						// 0x255
		goto DEFAULT;

	case 0x256:						// 0x256
		goto DEFAULT;

	case 0x257:						// 0x257
		goto DEFAULT;

	case 0x258:						// 0x258
		goto DEFAULT;

	case 0x259:						// 0x259
		goto DEFAULT;

	case 0x25A:						// 0x25A
		goto DEFAULT;

	case 0x25B:						// 0x25B
		goto DEFAULT;

	case 0x25C:						// 0x25C
		goto DEFAULT;

	case 0x25D:						// 0x25D
		goto DEFAULT;

	case 0x25E:						// 0x25E
		goto DEFAULT;

	case 0x25F:						// 0x25F
		goto DEFAULT;

	case 0x260:						// 0x260
		goto DEFAULT;

	case 0x261:						// 0x261
		goto DEFAULT;

	case 0x262:						// 0x262
		goto DEFAULT;

	case 0x263:						// 0x263
		goto DEFAULT;

	case 0x264:						// 0x264
		goto DEFAULT;

	case 0x265:						// 0x265
		goto DEFAULT;

	case 0x266:						// 0x266
		goto DEFAULT;

	case 0x267:						// 0x267
		goto DEFAULT;

	case 0x268:						// 0x268
		goto DEFAULT;

	case 0x269:						// 0x269
		goto DEFAULT;

	case 0x26A:						// 0x26A
		goto DEFAULT;

	case 0x26B:						// 0x26B
		goto DEFAULT;

	case 0x26C:						// 0x26C
		goto DEFAULT;

	case 0x26D:						// 0x26D
		goto DEFAULT;

	case 0x26E:						// 0x26E
		goto DEFAULT;

	case 0x26F:						// 0x26F
		goto DEFAULT;

	case 0x270:						// 0x270
		goto DEFAULT;

	case 0x271:						// 0x271
		goto DEFAULT;

	case 0x272:						// 0x272
		goto DEFAULT;

	case 0x273:						// 0x273
		goto DEFAULT;

	case 0x274:						// 0x274
		goto DEFAULT;

	case 0x275:						// 0x275
		goto DEFAULT;

	case 0x276:						// 0x276
		goto DEFAULT;

	case 0x277:						// 0x277
		goto DEFAULT;

	case 0x278:						// 0x278
		goto DEFAULT;

	case 0x279:						// 0x279
		goto DEFAULT;

	case 0x27A:						// 0x27A
		goto DEFAULT;

	case 0x27B:						// 0x27B
		goto DEFAULT;

	case 0x27C:						// 0x27C
		goto DEFAULT;

	case 0x27D:						// 0x27D
		goto DEFAULT;

	case 0x27E:						// 0x27E
		goto DEFAULT;

	case 0x27F:						// 0x27F
		goto DEFAULT;

	case WM_IME_SETCONTEXT:			// 0x281
		goto DEFAULT;

	case WM_IME_NOTIFY:				// 0x282
		goto DEFAULT;

	case WM_IME_CONTROL:			// 0x283
		goto DEFAULT;

	case WM_IME_COMPOSITIONFULL:	// 0x284
		goto DEFAULT;

	case WM_IME_SELECT:				// 0x285
		goto DEFAULT;

	case WM_IME_CHAR:				// 0x286
		goto DEFAULT;

	case 0x287:						// 0x287
		goto DEFAULT;

	case 0x288:						// 0x288
		goto DEFAULT;

	case 0x289:						// 0x289
		goto DEFAULT;

	case 0x28A:						// 0x28A
		goto DEFAULT;

	case 0x28B:						// 0x28B
		goto DEFAULT;

	case 0x28C:						// 0x28C
		goto DEFAULT;

	case 0x28D:						// 0x28D
		goto DEFAULT;

	case 0x28E:						// 0x28E
		goto DEFAULT;

	case 0x28F:						// 0x28F
		goto DEFAULT;

	case WM_IME_KEYDOWN:			// 0x290
		goto DEFAULT;

	case WM_IME_KEYUP:				// 0x291
		goto DEFAULT;

	case 0x292:						// 0x292
		goto DEFAULT;

	case 0x293:						// 0x293
		goto DEFAULT;

	case 0x294:						// 0x294
		goto DEFAULT;

	case 0x295:						// 0x295
		goto DEFAULT;

	case 0x296:						// 0x296
		goto DEFAULT;

	case 0x297:						// 0x297
		goto DEFAULT;

	case 0x298:						// 0x298
		goto DEFAULT;

	case 0x299:						// 0x299
		goto DEFAULT;

	case 0x29A:						// 0x29A
		goto DEFAULT;

	case 0x29B:						// 0x29B
		goto DEFAULT;

	case 0x29C:						// 0x29C
		goto DEFAULT;

	case 0x29D:						// 0x29D
		goto DEFAULT;

	case 0x29E:						// 0x29E
		goto DEFAULT;

	case 0x29F:						// 0x29F
		goto DEFAULT;

	case 0x2A0:						// 0x2A0
		goto DEFAULT;

#ifndef WM_MOUSEHOVER
#define WM_MOUSEHOVER 0x2A1
#endif
	case WM_MOUSEHOVER:				// 0x2A1
		goto DEFAULT;

	case 0x2A2:						// 0x2A2
		goto DEFAULT;

#ifndef WM_MOUSELEAVE
#define WM_MOUSELEAVE 0x2A3
#endif
	case WM_MOUSELEAVE:				// 0x2A3
		goto DEFAULT;

	case 0x2A4:						// 0x2A4
		goto DEFAULT;

	case 0x2A5:						// 0x2A5
		goto DEFAULT;

	case 0x2A6:						// 0x2A6
		goto DEFAULT;

	case 0x2A7:						// 0x2A7
		goto DEFAULT;

	case 0x2A8:						// 0x2A8
		goto DEFAULT;

	case 0x2A9:						// 0x2A9
		goto DEFAULT;

	case 0x2AA:						// 0x2AA
		goto DEFAULT;

	case 0x2AB:						// 0x2AB
		goto DEFAULT;

	case 0x2AC:						// 0x2AC
		goto DEFAULT;

	case 0x2AD:						// 0x2AD
		goto DEFAULT;

	case 0x2AE:						// 0x2AE
		goto DEFAULT;

	case 0x2AF:						// 0x2AF
		goto DEFAULT;

	case 0x2B0:						// 0x2B0
		goto DEFAULT;

	case 0x2B1:						// 0x2B1
		goto DEFAULT;

	case 0x2B2:						// 0x2B2
		goto DEFAULT;

	case 0x2B3:						// 0x2B3
		goto DEFAULT;

	case 0x2B4:						// 0x2B4
		goto DEFAULT;

	case 0x2B5:						// 0x2B5
		goto DEFAULT;

	case 0x2B6:						// 0x2B6
		goto DEFAULT;

	case 0x2B7:						// 0x2B7
		goto DEFAULT;

	case 0x2B8:						// 0x2B8
		goto DEFAULT;

	case 0x2B9:						// 0x2B9
		goto DEFAULT;

	case 0x2BA:						// 0x2BA
		goto DEFAULT;

	case 0x2BB:						// 0x2BB
		goto DEFAULT;

	case 0x2BC:						// 0x2BC
		goto DEFAULT;

	case 0x2BD:						// 0x2BD
		goto DEFAULT;

	case 0x2BE:						// 0x2BE
		goto DEFAULT;

	case 0x2BF:						// 0x2BF
		goto DEFAULT;

	case 0x2C0:						// 0x2C0
		goto DEFAULT;

	case 0x2C1:						// 0x2C1
		goto DEFAULT;

	case 0x2C2:						// 0x2C2
		goto DEFAULT;

	case 0x2C3:						// 0x2C3
		goto DEFAULT;

	case 0x2C4:						// 0x2C4
		goto DEFAULT;

	case 0x2C5:						// 0x2C5
		goto DEFAULT;

	case 0x2C6:						// 0x2C6
		goto DEFAULT;

	case 0x2C7:						// 0x2C7
		goto DEFAULT;

	case 0x2C8:						// 0x2C8
		goto DEFAULT;

	case 0x2C9:						// 0x2C9
		goto DEFAULT;

	case 0x2CA:						// 0x2CA
		goto DEFAULT;

	case 0x2CB:						// 0x2CB
		goto DEFAULT;

	case 0x2CC:						// 0x2CC
		goto DEFAULT;

	case 0x2CD:						// 0x2CD
		goto DEFAULT;

	case 0x2CE:						// 0x2CE
		goto DEFAULT;

	case 0x2CF:						// 0x2CF
		goto DEFAULT;

	case 0x2D0:						// 0x2D0
		goto DEFAULT;

	case 0x2D1:						// 0x2D1
		goto DEFAULT;

	case 0x2D2:						// 0x2D2
		goto DEFAULT;

	case 0x2D3:						// 0x2D3
		goto DEFAULT;

	case 0x2D4:						// 0x2D4
		goto DEFAULT;

	case 0x2D5:						// 0x2D5
		goto DEFAULT;

	case 0x2D6:						// 0x2D6
		goto DEFAULT;

	case 0x2D7:						// 0x2D7
		goto DEFAULT;

	case 0x2D8:						// 0x2D8
		goto DEFAULT;

	case 0x2D9:						// 0x2D9
		goto DEFAULT;

	case 0x2DA:						// 0x2DA
		goto DEFAULT;

	case 0x2DB:						// 0x2DB
		goto DEFAULT;

	case 0x2DC:						// 0x2DC
		goto DEFAULT;

	case 0x2DD:						// 0x2DD
		goto DEFAULT;

	case 0x2DE:						// 0x2DE
		goto DEFAULT;

	case 0x2DF:						// 0x2DF
		goto DEFAULT;

	case 0x2E0:						// 0x2E0
		goto DEFAULT;

	case 0x2E1:						// 0x2E1
		goto DEFAULT;

	case 0x2E2:						// 0x2E2
		goto DEFAULT;

	case 0x2E3:						// 0x2E3
		goto DEFAULT;

	case 0x2E4:						// 0x2E4
		goto DEFAULT;

	case 0x2E5:						// 0x2E5
		goto DEFAULT;

	case 0x2E6:						// 0x2E6
		goto DEFAULT;

	case 0x2E7:						// 0x2E7
		goto DEFAULT;

	case 0x2E8:						// 0x2E8
		goto DEFAULT;

	case 0x2E9:						// 0x2E9
		goto DEFAULT;

	case 0x2EA:						// 0x2EA
		goto DEFAULT;

	case 0x2EB:						// 0x2EB
		goto DEFAULT;

	case 0x2EC:						// 0x2EC
		goto DEFAULT;

	case 0x2ED:						// 0x2ED
		goto DEFAULT;

	case 0x2EE:						// 0x2EE
		goto DEFAULT;

	case 0x2EF:						// 0x2EF
		goto DEFAULT;

	case 0x2F0:						// 0x2F0
		goto DEFAULT;

	case 0x2F1:						// 0x2F1
		goto DEFAULT;

	case 0x2F2:						// 0x2F2
		goto DEFAULT;

	case 0x2F3:						// 0x2F3
		goto DEFAULT;

	case 0x2F4:						// 0x2F4
		goto DEFAULT;

	case 0x2F5:						// 0x2F5
		goto DEFAULT;

	case 0x2F6:						// 0x2F6
		goto DEFAULT;

	case 0x2F7:						// 0x2F7
		goto DEFAULT;

	case 0x2F8:						// 0x2F8
		goto DEFAULT;

	case 0x2F9:						// 0x2F9
		goto DEFAULT;

	case 0x2FA:						// 0x2FA
		goto DEFAULT;

	case 0x2FB:						// 0x2FB
		goto DEFAULT;

	case 0x2FC:						// 0x2FC
		goto DEFAULT;

	case 0x2FD:						// 0x2FD
		goto DEFAULT;

	case 0x2FE:						// 0x2FE
		goto DEFAULT;

	case 0x2FF:						// 0x2FF
		goto DEFAULT;

	case WM_CUT:					// 0x300
		goto DEFAULT;

	case WM_COPY:					// 0x301
		goto DEFAULT;

	case WM_PASTE:					// 0x302
		goto DEFAULT;

	case WM_CLEAR:					// 0x303
		goto DEFAULT;

	case WM_UNDO:					// 0x304
		goto DEFAULT;

	case WM_RENDERFORMAT:			// 0x305
		goto DEFAULT;

	case WM_RENDERALLFORMATS:		// 0x306
		goto DEFAULT;

	case WM_DESTROYCLIPBOARD:		// 0x307
		goto DEFAULT;

	case WM_DRAWCLIPBOARD:			// 0x308
		goto DEFAULT;

	case WM_PAINTCLIPBOARD:			// 0x309
		goto DEFAULT;

	case WM_VSCROLLCLIPBOARD:		// 0x30A
		goto DEFAULT;

	case WM_SIZECLIPBOARD:			// 0x30B
		goto DEFAULT;

	case WM_ASKCBFORMATNAME:		// 0x30C
		goto DEFAULT;

	case WM_CHANGECBCHAIN:			// 0x30D
		goto DEFAULT;

	case WM_HSCROLLCLIPBOARD:		// 0x30E
		goto DEFAULT;

	case WM_QUERYNEWPALETTE:		// 0x30F
		{
			HDC dc;
			HPALETTE oldPal;
			dc = ::GetDC(MainFrame);
			oldPal = ::SelectPalette(dc, StdPalette, FALSE);
			::RealizePalette(dc);
			::SelectPalette(dc, oldPal, FALSE);
			::ReleaseDC(MainFrame, dc);

			if (SelectMode)
			{
				::InvalidateRect(SelectView, NULL, FALSE);
				::UpdateWindow(SelectView);
				::InvalidateRect(SelectView, NULL, FALSE);
				::UpdateWindow(SelectView);
			}

			ProgOnPaint();
			UpdateAllTree();
			UpdateAllTree();

			TimeRepaint(-1);
			return TRUE;
		}
		goto DEFAULT;

	case WM_PALETTEISCHANGING:		// 0x310
		goto DEFAULT;

	case WM_PALETTECHANGED:			// 0x311
		if ((HWND)wParam != MainFrame)
		{
			HDC dc;
			HPALETTE oldPal;
			dc = ::GetDC(MainFrame);
			oldPal = ::SelectPalette(dc, StdPalette, FALSE);
			::RealizePalette(dc);
			::SelectPalette(dc, oldPal, FALSE);
			::ReleaseDC(MainFrame, dc);

			if (RepaintTimerN <= 0)
			{
				if (SelectMode)
				{
					::InvalidateRect(SelectView, NULL, FALSE);
					::UpdateWindow(SelectView);
					::InvalidateRect(SelectView, NULL, FALSE);
					::UpdateWindow(SelectView);
				}

				ProgOnPaint();
				UpdateAllTree();
				UpdateAllTree();
			}

			TimeRepaint(-1);
			return TRUE;
		}
		return FALSE;

	case WM_HOTKEY:					// 0x312
		goto DEFAULT;

	case 0x313:						// 0x313
		goto DEFAULT;

	case 0x314:						// 0x314
		goto DEFAULT;

	case 0x315:						// 0x315
		goto DEFAULT;

	case 0x316:						// 0x316
		goto DEFAULT;

	case WM_PRINT:					// 0x317
		goto DEFAULT;

	case WM_PRINTCLIENT:			// 0x318
		goto DEFAULT;

	case MM_MCINOTIFY:
		EditMusic::OnMMNotify((DWORD)wParam, (int)lParam);
		return 0;

	default:
DEFAULT:
		return ::DefWindowProc(hWnd, nMsg, wParam, lParam);
	}
}
