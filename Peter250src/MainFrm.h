
/***************************************************************************\
*																			*
*							Hlavn� okno aplikace							*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// glob�ln� prom�nn�

extern	HWND		MainFrame;				// hlavn� okno aplikace
extern	HMENU		Menu;					// aktivn� menu v hlavn�m okn�
extern	HWND		ToolBar;				// n�strojov� li�ta

extern	HBITMAP		ToolBarBmp;				// bitmapa panelu n�stroj�

extern	RECT		ClientRect;				// klientsk� oblast hlavn�ho okna
extern	int			ClientWidth;			// ���ka klientsk� oblasti
extern	int			ClientHeight;			// v��ka klientsk� oblasti

extern	CText		MainFrameName;			// titulek hlavn�ho okna - jm�no aplikace
extern	CText		MainFrameText;			// �pln� text titulku hlavn�ho okna

// kurzory my�i
extern	HCURSOR		CurAkt;					// aktivn� kurzor

extern	HCURSOR		CurArrow;				// (standardn�) �ipka
extern	HCURSOR		CurCil;					// c�l operace
extern	HCURSOR		CurCopy;				// kopie
extern	HCURSOR		CurDelete;				// zru�en�
extern	HCURSOR		CurElip;				// elipsa
extern	HCURSOR		CurFill;				// v�pl�
extern	HCURSOR		CurFillElip;			// ov�l
extern	HCURSOR		CurFillRect;			// vypln�n� obd�ln�k
extern	HCURSOR		CurFillRound;			// kruh
extern	HCURSOR		CurKapatko;				// kap�tko
extern	HCURSOR		CurKoule;				// koule
extern	HCURSOR		CurLine;				// ��ra
extern	HCURSOR		CurMove;				// p�esun
extern	HCURSOR		CurNoDrag;				// z�kaz ta�en�
extern	HCURSOR		CurPaint;				// �t�tec
extern	HCURSOR		CurPen;					// pero
extern	HCURSOR		CurRect;				// obd�ln�k
extern	HCURSOR		CurRound;				// kru�nice
extern	HCURSOR		CurRuka;				// ruka
extern	HCURSOR		CurSelect;				// v�b�r bloku
extern	HCURSOR		CurSelectMove;			// p�esun v�b�ru
extern	HCURSOR		CurSplitH;				// rozd�len� horizont�ln�
extern	HCURSOR		CurSplitV;				// rozd�len� vertik�ln�
extern	HCURSOR		CurSpray;				// sprej
extern	HCURSOR		CurWait;				// �ek�n�

// stav my�i (aktualizov�no v PreTranslateMessage)
extern	BOOL		LMouseDown;				// lev� tla��tko stisknuto
extern	BOOL		LMouseDClick;			// dvojklik lev�ho tla��tka
extern	BOOL		RMouseDown;				// prav� tla��tko stisknuto
extern	BOOL		RMouseDClick;			// dvojklik prav�ho tla��tka
extern	HWND		MouseWnd;				// okno pod kurzorem my�i
extern	POINT		MouseClient;			// sou�adnice my�i v okn� pod kurzorem
extern	POINT		MouseScreen;			// sou�adnice my�i na displeji
extern	POINT		MouseMain;				// sou�adnice my�i v hlavn�m okn�

extern	HWND		StatusBar;				// stavov� li�ta
extern	CText		StatusText;				// zobrazen� text stavov� li�ty
extern	CText		StatusText2;			// zobrazen� text stavov� li�ty 2
extern	int			StatusWidth;			// ���ka panelu 2 stavov� li�ty
extern	BOOL		StatusBig;				// mo�nost pou��t velk� fonty stavov� li�ty

/////////////////////////////////////////////////////////////////////////////
// menu

// definice polo�ky menu
typedef struct MENUITEM_ 
{
	int		id0;							// identifikace jm�na polo�ky (0=separ�tor nebo konec)
	int		id;								// identifikace p��kazu a n�pov�dy polo�ky (0=n�sleduje podmenu nebo konec)
	int		bitmap;							// ��slo ikony s obr�zkem (-1 = nen�)
	int		typ;							// typ polo�ky - kombinace p��znak� (0=nic)
} MENUITEM;

#define MSWC	1							// je sp�na�
#define MRAD	2							// je p�ep�na�

// informace o polo�ce spu�t�n�ho menu
typedef struct MENUAKTITEM_
{
	int		id0;							// identifikace jm�na polo�ky (0=separ�tor nebo konec)
	int		id;								// identifikace p��kazu a n�pov�dy polo�ky (0=n�sleduje podmenu nebo konec)
	int		bitmap;							// ��slo ikony s obr�zkem (-1 = nen�)
	int		typ;							// typ polo�ky - kombinace p��znak� (0=nic)
	HMENU	menu;							// menu s polo�kou
	BOOL	main;							// je to polo�ka hlavn�ho menu
	int		inx;							// index polo�ky v menu
	int		width;							// ���ka polo�ky (s p�ihl�dnut�m k ostatn�m polo�k�m)
	int		height;							// v��ka polo�ky
	CText	text;							// text polo�ky v menu (bez zkratkov� kl�vesy)
	CText	textshort;						// text zkratkov� kl�vesy
	int		key;							// hork� kl�vesa - velk� p�smeno (0=nen�)
	CText	help;							// dlouh� n�pov�da
	CText	help2;							// kr�tk� n�pov�da (max. 79 znak�)
} MENUAKTITEM;


extern MENUITEM** MenuAkt;					// aktu�ln� menu (kv�li cachov�n�, NULL = aktualizovat v�dy)

extern MENUITEM MenuSoubor1[];				// soubor 1
extern MENUITEM MenuSoubor2[];				// soubor 2
extern MENUITEM MenuEditace[];				// editace
extern MENUITEM MenuZobraz[];				// zobrazen�
extern MENUITEM MenuProgram[];				// program
extern MENUITEM MenuObrazek[];				// obr�zek
extern MENUITEM MenuIkona[];				// ikona
extern MENUITEM MenuNastroje[];				// n�stroje
extern MENUITEM MenuTloustka[];				// tlou��ka pera
extern MENUITEM MenuPlocha[];				// plocha
extern MENUITEM MenuSprajt[];				// sprajt
extern MENUITEM MenuSound[];				// zvuk
extern MENUITEM MenuMusic[];				// hudba
extern MENUITEM MenuHlp[];					// n�pov�da

/////////////////////////////////////////////////////////////////////////////
// nastaven� nov�ho menu

void SetNewMenu(MENUITEM** item);


/////////////////////////////////////////////////////////////////////////////
// p��prava jm�na hlavn�ho okna

void MainFrameNameInit();


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� hlavn�ho okna aplikace

BOOL MainFrameCreate();


/////////////////////////////////////////////////////////////////////////////
// nastaven� fontu stavov�ho ��dku

void SetStatusFont();


/////////////////////////////////////////////////////////////////////////////
// zobrazen� jm�na souboru v titulku okna

void ZobrazJmeno();


/////////////////////////////////////////////////////////////////////////////
// povolen� volby v menu a v panelu n�stroj�

void EnableCommand(int id, BOOL enable);

/////////////////////////////////////////////////////////////////////////////
// zapnut� volby v menu a v panelu n�stroj�

void CheckCommand(int id, BOOL check);

/////////////////////////////////////////////////////////////////////////////
// nastaven�/aktualizace ���ky pole 2 stavov� li�ty

void SetStatusWidth(int width);


/////////////////////////////////////////////////////////////////////////////
// nastaven� textu stavov� li�ty

void SetStatusText(const CText& text);


/////////////////////////////////////////////////////////////////////////////
// nastaven� textu stavov� li�ty 2

void SetStatusText2(const CText& text);


/////////////////////////////////////////////////////////////////////////////
// zru�en� v�ech tla��tek panelu n�stroj� (a� po zadan� minim�ln� mno�stv�)

void ToolBarClear(int min);


/////////////////////////////////////////////////////////////////////////////
// p�id�n� tla��tek do panelu n�stroj�

void ToolBarAdd(TBBUTTON* button, int num);


/////////////////////////////////////////////////////////////////////////////
// nastaven� obr�zku tla��tka v panelu n�stroj�

void ToolBarBitmap(int id, int bitmap);


/////////////////////////////////////////////////////////////////////////////
// aktualizace zobrazen� panelu n�stroj�

void ToolBarResize();

/////////////////////////////////////////////////////////////////////////////
// aktualizace klientsk�ch sou�adnic

void InitClientRect();

/////////////////////////////////////////////////////////////////////////////
// obsluha informa�n�ho okna About

BOOL CALLBACK AboutBoxProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

void OnAboutBox();

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� kombinovan�ho pole v panelu n�stroj�

HWND CreateToolCombo(int id, int* items, int index);

/////////////////////////////////////////////////////////////////////////////
// v�b�r prvku v COMBO boxu

void SelectCombo(HWND combo, int id, int* items);

/////////////////////////////////////////////////////////////////////////////
// zobrazen� kurzoru �ek�n� (zah�jen� a ukon�en� mus� b�t do p�ru!)

void BeginWaitCursor();

/////////////////////////////////////////////////////////////////////////////
// vypnut� kurzoru �ek�n� (zah�jen� a ukon�en� mus� b�t do p�ru!)

void EndWaitCursor();

/////////////////////////////////////////////////////////////////////////////
// obsluha zpr�vy p�ed rozesl�n�m do oken

BOOL PreTranslateMessage(MSG* msg);


/////////////////////////////////////////////////////////////////////////////
// �asov� p�ekreslen� okna (zadan�ho bufferu, -1=v�e)

void TimeRepaint(int bufID);


/////////////////////////////////////////////////////////////////////////////
// aktualizace p�ede�l�ho okna

void AktPrevWindow();


/////////////////////////////////////////////////////////////////////////////
// nastaven� fontu a stylu pro vlastn� v�pis textu

void DialogCustomText(HWND hWnd, DWORD id);


/////////////////////////////////////////////////////////////////////////////
// nastaven� vlastn�ho v�pisu textu

void DialogCustom0(HWND hWnd, DWORD id);


/////////////////////////////////////////////////////////////////////////////
// nastaven� vlastn�ho v�pisu textu

void DialogCustom(HWND hWnd, DWORD id, BOOL load, BOOL font);


/////////////////////////////////////////////////////////////////////////////
// vlastn� vykreslen� prvk� v dialogov�ch oknech (vrac� TRUE=obslou�eno)

BOOL DialogDraw(HWND hWnd, LPARAM lParam);


/////////////////////////////////////////////////////////////////////////////
// nastaven� vlastn�ho p�ep�na�e RADIO

void DialogCustomRadio(HWND hWnd, DWORD id);


/////////////////////////////////////////////////////////////////////////////
// nastaven� vlastn�ho p�ep�na�e CHECK

void DialogCustomCheck(HWND hWnd, DWORD id);


/////////////////////////////////////////////////////////////////////////////
// nastaven� vlastn�ho r�me�ku

void DialogCustomBox(HWND hWnd, DWORD id);


/////////////////////////////////////////////////////////////////////////////
// nastaven� hodnoty p�ep�na�e

void DialogSetCheck(HWND hWnd, DWORD id, BOOL check);


/////////////////////////////////////////////////////////////////////////////
// nastaven� sm�ru textu zprava doleva   JAKJAK

void SetRightToLeft(BOOL righttoleft);


/////////////////////////////////////////////////////////////////////////////
// indexy obr�zk� tla��tek pro ToolBar

enum ToolButton {
	ButtonStart,			// start
	ButtonNew,				// nov� soubor
	ButtonOpen,				// otev�en� souboru
	ButtonClose,			// uzav�en� souboru
	ButtonSave,				// ulo�en� souboru
	ButtonSaveModi,			// ulo�en� modifikovan�ho souboru
	ButtonUndo,				// undo
	ButtonRedo,				// redo
	ButtonCut,				// cut
	ButtonCopy,				// copy
	ButtonPaste,			// paste
	ButtonClear,			// clear
	ButtonAll,				// select all
	ButtonPrint,			// tisk
	ButtonHelpCont,			// help
	ButtonHelp,				// help k obsahu
	ButtonIn,				// zoom in
	ButtonOut,				// zoom out
	ButtonPen,				// pero
	ButtonPaint,			// �t�tec
	ButtonLine,				// ��ra
	ButtonBox,				// obd�ln�k
	ButtonFillBox,			// vypln�n� obd�ln�k
	ButtonCircle,			// kru�nice
	ButtonFillCircle,		// kruh
	ButtonEllipse,			// elipsa
	ButtonFillEllipse,		// ov�l
	ButtonWidth1,			// ���ka 1
	ButtonWidth2,			// ���ka 2
	ButtonWidth3,			// ���ka 3
	ButtonWidth4,			// ���ka 4
	ButtonWidth5,			// ���ka 5
	ButtonPipe,				// kap�tko
	ButtonFill,				// v�pl�
	ButtonSpray,			// sprej
	ButtonText,				// text
	ButtonSelect,			// v�b�r
	ButtonYFlip,			// p�evr�cen� podle vertik�ln� osy
	ButtonXFlip,			// p�evr�cen� podle horizont�ln� osy
	ButtonRRot,				// oto�en� doprava
	ButtonLRot,				// oto�en� doleva
	ButtonXCol,				// z�m�na barev
	ButtonSCol,				// nastaven� barvy
	ButtonMul2,				// zv�t�en� 2x
	ButtonDiv2,				// zmen�en� 2x
	ButtonLRot45,			// oto�en� o 45 stup��
	ButtonLRot30,			// oto�en� o 30 stup��
	ButtonLRot22,			// oto�en� o 22.5 stup��
	ButtonArrow,			// m�d �ipky
	ButtonMap,				// m�d obr�zk� mapy
	ButtonSwc1,				// p�ep�na� 1
	ButtonSwc2,				// p�ep�na� 2
	ButtonSwc3,				// p�ep�na� 3
	ButtonSwc4,				// p�ep�na� 4
	ButtonSwc5,				// p�ep�na� 5
	ButtonSwcN1,			// ��seln� p�ep�na� 1
	ButtonSwcN2,			// ��seln� p�ep�na� 2
	ButtonSwcN3,			// ��seln� p�ep�na� 3
	ButtonPlay,				// p�ehr�v�n�
	ButtonPause,			// pozastaven� p�ehr�v�n�
	ButtonStop,				// zastaven� p�ehr�v�n�
	ButtonRew,				// rychl� p�ev�jen� zp�t
	ButtonFrw,				// rychl� p�ev�jen� vp�ed
	ButtonLoop,				// opakovan� p�ehr�v�n�
	ButtonRecord,			// nahr�v�n�
	ButtonTest,				// test sprajtu
	ButtonHistPrev,			// n�vrat v historii zp�t
	ButtonHistNext,			// n�vrat v historii vp�ed
	ButtonFind,				// hled�n�
	ButtonFindPrev,			// hled�n� zp�t
	ButtonFindNext,			// hled�n� vp�ed
	ButtonRefDef,			// nalezen� deklarace
	ButtonRefPrev,			// p�edchoz� reference
	ButtonRefNext,			// n�sleduj�c� reference
	ButtonLock,				// z�mek
	ButtonOff,				// vypnut� funkce
	ButtonDefName,			// implicitn� jm�no
	ButtonDefIcon,			// implicitn� ikona
	ButtonSphere,			// koule
	ButtonDimen,			// rozm�ry
	ButtonSpritePar,		// parametry sprajtu
	ButtonEditName,			// editace jm�na
	ButtonEditIcon,			// editace ikony
	ButtonLRot67,			// oto�en� o 67.5 stup��
	ButtonLRot60,			// oto�en� o 60 stup��
	ButtonRRot67,			// oto�en� o 67.5 stup�� vpravo
	ButtonRRot60,			// oto�en� o 60 stup�� vpravo
	ButtonRRot45,			// oto�en� o 45 stup�� vpravo
	ButtonRRot30,			// oto�en� o 30 stup�� vpravo
	ButtonRRot22,			// oto�en� o 22.5 stup�� vpravo
	ButtonEditBlok,			// �pravy obr�zku
	ButtonRastr,			// rastr
	ButtonOtisk,			// otisk bloku
	ButtonFillMap,			// v�pl� plochy pol��kem
	ButtonEditMap,			// editace pol��ek plochy
	ButtonDirNew,			// nov� slo�ka program�
	ButtonMulDiv,			// voliteln� zv�t�en�
	ButtonCheck,			// zapnut p�ep�na� menu
	ButtonCheckOff,			// vypnut p�ep�na� menu
	ButtonRadioOn,			// p�ep�na� zapnut
	ButtonRadioOff,			// p�ep�na� vypnut
	ButtonBul,				// bulharsk� vlajka
	ButtonCz,				// �esk� vlajka
	ButtonDan,				// d�nsk� vlajka
	ButtonGer,				// n�meck� vlajka
	ButtonRec,				// �eck� vlajka
	ButtonEng,				// anglick� vlajka
	ButtonSpa,				// �pan�lsk� vlajka
	ButtonFin,				// finsk� vlajka
	ButtonFra,				// francouzsk� vlajka
	ButtonMad,				// ma�arsk� vlajka
	ButtonIsl,				// islandsk� vlajka
	ButtonIta,				// italsk� vlajka
	ButtonHol,				// holandsk� vlajka
	ButtonNor,				// norsk� vlajka
	ButtonPol,				// polsk� vlajka
	ButtonPor,				// portugalsk� vlajka
	ButtonRum,				// rumunsk� vlajka
	ButtonRus,				// rusk� vlajka
	ButtonSrb,				// srbochorvatsk� vlajka
	ButtonSlo,				// slovensk� vlajka
	ButtonAlb,				// alb�nsk� vlajka
	ButtonSwe,				// �w�dsk� vlajka
	ButtonTur,				// tureck� vlajka
	ButtonVie,				// vietnamsk� vlajka
	ButtonAra,				// arabsk� vlajka - asi egyptsk�, JAKJAK
	ButtonHeb,				// hebrejsk� vlajka - asi izraelsk�

	ToolButtonNum,			// po�et tla��tek celkem
};
