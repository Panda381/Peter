
#include "Main.h"

/***************************************************************************\
*																			*
*							Prov�d�n� programu								*
*																			*
\***************************************************************************/

#pragma optimize("t", on)			// optimalizace na maxim�ln� rychlost

// buffer prov�d�n�ho programu
int			ProgStart = 0;		// index vol�n� hlavn� funkce
EXECITEM*	ProgBuf = NULL;		// buffer programu
int			ProgNum = 0;		// po�et polo�ek programu
int			ProgMax = 0;		// velikost bufferu programu
EXECITEM*	ExecItem = NULL;	// ukazatel prov�d�n�ho programu
int			Hloubka = MAXHLOUBKA;	// hloubka vno�en� do funkc�

// buffer obr�zk�
BYTE*	BackBuf = NULL;			// buffer obr�zku pozad�
BYTE*	PicBuf = NULL;			// buffer obr�zku sestavy (s ikonami)
BYTE*	TextBuf = NULL;			// buffer pro v�stup textu

// aktualizace p�ekreslen� ikon
bool*	AktBuf;					// buffer po�adavk� k p�ekreslen� ikon

int		AktLeft;				// lev� okraj k p�ekreslen� - v�etn� (v bodech)
int		AktTop;					// horn� okraj k p�ekreslen� - v�etn� (v bodech)
int		AktRight;				// prav� okraj k p�ekreslen� - bez (v bodech)
int		AktBottom;				// doln� okraj k p�ekreslen� - bez (v bodech)

bool	IsOverlay = false;		// zobrazeno p�ekryvn� okno Windows (otev�en� soubor� atd.)

// aktivn� zobrazen� plocha
int		WidthN = 0;				// ���ka plochy v ikon�ch
int		HeightN = 0;			// v��ka plochy v ikon�ch
int		Width = 0;				// ���ka plochy v bodech
int		Height = 0;				// v��ka plochy v bodech
int		WidthByte = 0;			// d�lka linky v bajtech
int		RowByte = 0;			// d�lka ��dku ikon v bajtech

// v�stupn� buffer
bool	DispBufUse = false;		// pou��v� se v�stupn� buffer (je jin� m���tko ne� 1:1)
bool	DispBufUse2 = false;	// pou��v� se v�stupn� buffer v TRUE COLOR (je m���tko 2:1)
BYTE*	DispBuf = NULL;			// v�stupn� buffer
int		DispBufSize = 0;		// aktu�ln� velikost v�stupn�ho bufferu
int*	DispMapX = NULL;		// buffer mapov�n� z bodu obr�zku na v�stupn� bod
int*	DispMapY = NULL;		// buffer mapov�n� z linky obr�zku na v�stupn� linku
int*	DispRemapX = NULL;		// buffer zp�tn�ho mapov�n� v�stupn�ho bodu na offset v lince obr�zku
int*	DispRemapY = NULL;		// buffer zp�tn�ho mapov�n� v�stupn� linky na offset v obr�zku

int		DispLeft = 0;			// lev� okraj k zobrazen� v okn�
int		DispTop = 0;			// horn� okraj k zobrazen� v okn�
int		DispWidth = 1;			// zobrazen� �i�ka v bodech
int		DispWidthByte = 0;		// ���ka linky v�stupn�ho bufferu v bajtech
int		DispHeight = 1;			// zobrazen� v��ka v bodech

double	Meritko = 1;			// aktu�ln� m���tko zobrazen�
double	Meritko0 = 1;			// uschovan� m���tko zobrazen� pro norm�ln� okno

// zobrazen� textu
int		GraphAreaWidth = 0;		// ���ka pole k zobrazen� textu centrovan� (v bodech)

// stavov� ��dek
bool	CaptionAkt = false;		// po�adavek k p�ekreslen� nadpisu okna
CString	CaptionText;			// nov� text titulku k nastaven�
bool	StatusAkt = false;		// po�adavek k p�ekreslen� stavov�ho ��dku
CString	StatusText;				// nov� stavov� text k nastaven�

// buffer sprajt�
CBufInt	SpriteVisible;			// buffer index� viditeln�ch sprajt�
CBufInt	SpriteMoving;			// buffer index� pohybuj�c�ch se sprajt�
CBufInt	SpriteKlid;				// buffer sprajt� animovan�ch v klidu
CBufInt	SpriteDispReq;			// buffer sprajt� vy�aduj�c�ch p�ekreslen�
CBufInt	SpriteDispLevel;		// hladiny sprajt� vy�aduj�c�ch p�ekreslen�
CBufReal SpriteDispY;			// sou�adnice sprajt� Y 

int		SpriteWinX1 = 0;		// lev� okraj okna sprajt�
int		SpriteWinY1 = 0;		// doln� okraj okna sprajt�
int		SpriteWinX2 = 1000000*ICONWIDTH;	// prav� okraj okna sprajt�
int		SpriteWinY2 = 1000000*ICONHEIGHT;	// horn� okraj okna sprajt�

int		WhileOut = WHILEOUT;	// ��ta� time-out cyklu
int		ReDispOut = REDISPOUT;	// ��ta� time-out p�ekreslen� displeje

// obsluha �asova�e
UINT	TimerID = 0;			// ID �asova�e

bool	Pause = false;			// p��znak zapaouzov�n� programu
volatile int Break;				// p��znaky p�eru�en� (0 = nen� p�eru�en�)

double		MouseX = 0;			// sou�adnice my�i X (v ikonov�ch sou�adnic�ch)
double		MouseY = 0;			// sou�adnice my�i Y (v ikonov�ch sou�adnic�ch)

// obsluha CD
UINT	CDDevice = 0;			// ID za��zen� MCI pro p�ehr�v�n� CD (0=nen� otev�eno)
int		CDError = -1;			// ��ta� pro nov� pokus p�i chyb� (mus� b�t < 0)
int		CDValidTime = 0;		// ��ta� platnosti �daj� o CD (mus� b�t > 0)
bool	CDDiskValid = false;	// �daje o disku jsou platn�, p��znak vlo�en� disku
int		CDStart = 2000;			// startovac� pozice CD v ms
int		CDLength = CDDEFLENGTH;	// d�lka CD v ms
int		CDTracks = 1;			// po�et stop CD
int		CDPos = 0;				// p�ehr�van� pozice CD v ms
int		CDTrack = 1;			// p�ehr�van� stopa
int		CDTrackLength = CDDEFLENGTH;// d�lka p�ehr�van� stopy v ms
int		CDTrackPos = 0;			// pozice v p�ehr�van� stop� v ms
bool	CDPlaing = false;		// p��znak p�ehr�v�n� CD
bool	CDPausing = false;		// p��znak pauzy CD

// obsluha kl�vesnice
int		KeyBufWrite = 0;		// ukl�dac� index do bufferu kl�vesnice
int		KeyBufRead = 0;			// �tec� index z bufferu kl�vesnice
int		KeyBuf[KEYBUFSIZE];		// buffer kl�vesnice

int		CharBufWrite = 0;		// ukl�dac� index do bufferu znak�
int		CharBufRead = 0;		// �tec� index z bufferu znak�
int		CharBuf[KEYBUFSIZE];	// buffer znak�

bool	CtrlAgain = false;		// p��znak opakovan�ho stisku Ctrl

int		KeyMapNum = 0;			// po�et stisknut�ch kl�ves
bool	KeyMap[KEYMAPSIZE];		// mapa stisknut�ch kl�ves

bool	NumLock = false;		// stav Num Lock
bool	CapsLock = false;		// stav Caps Lock
bool	ScrollLock = false;		// stav Scroll Lock
bool	InsertLock = false;		// stav Insert

bool	AktNumLock = false;		// po�adavek nastaven� Num Lock
bool	AktCapsLock = false;	// po�adavek nastaven� Caps Lock
bool	AktScrollLock = false;	// po�adavek nastaven� Scroll Lock
bool	AktInsertLock = false;	// po�adavek nastaven� Insert

// obsluha soubor�
bool	FileError = false;		// p��znak chyby soubor�
int		FileCloseTime = 0;		// ��ta� �asu pro uzav�en� soubor�
int		FileTextNLen = 10;		// d�lka textu s pevnou d�lkou
bool	FileTextUnicode = false; // text je v k�du UNICODE

CString	FileRead;				// jm�no souboru pro �ten�
HANDLE	FileReadHandle = NULL;	// handle souboru pro �ten� (NULL=nen�)
int		FileReadOff = 0;		// offset v souboru pro �ten�
BYTE*	FileReadBuf = NULL;		// buffer pro �ten�
int		FileReadBufOff = 0;		// offset bufferu pro �ten�
int		FileReadBufN = 0;		// po�et bajt� v bufferu pro �ten�

CString	FileWrite;				// jm�no souboru pro z�pis
HANDLE	FileWriteHandle = NULL;	// handle souboru pro z�pis (NULL=nen�)
int		FileWriteOff = 0;		// offset v souboru pro z�pis
BYTE*	FileWriteBuf = NULL;	// buffer pro z�pis
int		FileWriteBufOff = 0;	// offset bufferu pro z�pis
int		FileWriteBufN = 0;		// po�et bajt� v bufferu pro z�pis

CString	AliasName;				// jm�no souboru ALIASES
CString	AliasKey;				// kl�� ALIASES
CString	AliasGroup;				// skupina ALIASES

__int64	DiskSize = 0;			// velikost disku (z funkce GetDiskSpace)
__int64	DiskFree = 0;			// voln� m�sto disku (z funkce GetDiskSpace)
__int64	DiskFreeUser = 0;		// voln� m�sto u�ivatele (z funkce GetDiskSpace)

// konzola
bool	ConsoleOn = false;		// p��znak re�imu konzoly
bool	NewConsole = false;		// byla vytvo�ena nov� konzola
HANDLE	ConIn = INVALID_HANDLE_VALUE;	// handle pro vstup z konzoly
HANDLE	ConOut = INVALID_HANDLE_VALUE;	// handle pro v�stup na konzolu
HANDLE	ConErr = INVALID_HANDLE_VALUE;	// handle pro chybov� v�stup na konzolu

// joystick
int		JoystickNext[MAXJOYSTICK];	// ��ta� do dal��ho testu (mus� b�t < 0)
bool	JoystickValid[MAXJOYSTICK];	// p��znak platnosti �daj� joysticku
int		JoystickOldX[MAXJOYSTICK];	// star� sou�adnice X
int		JoystickOldY[MAXJOYSTICK];	// star� sou�adnice Y
int		JoystickOldZ[MAXJOYSTICK];	// star� sou�adnice Z
double	JoystickX[MAXJOYSTICK];		// sou�adnice X
double	JoystickY[MAXJOYSTICK];		// sou�adnice Y
double	JoystickZ[MAXJOYSTICK];		// sou�adnice Z
bool	Joystick1[MAXJOYSTICK];		// tla��tko 1
bool	Joystick2[MAXJOYSTICK];		// tla��tko 2
bool	Joystick3[MAXJOYSTICK];		// tla��tko 3
bool	Joystick4[MAXJOYSTICK];		// tla��tko 4

// prvky oken
CString ButtonClassName(_T("Button"));
CString EditClassName(_T("Edit"));
CString RichEditClassName(_T("RichEdit"));
CString ListBoxClassName(_T("Listbox"));
CString ComboBoxClassName(_T("Combobox"));
CString StaticClassName(_T("Static"));
CString TabsClassName(_T("SysTabControl32"));

bool	DialogGraph = false;		// je grafick� pozad� hlavn�ho dialogov�ho okna

bool	DialogMode = false;			// dialogov� m�d hlavn�ho okna
int		DialogParent = 0;			// aktivn� rodi�ovsk� okno
int		WindowID = -1;				// aktivn� prvek (-1 = neplatn�)
int		WindowFocus = 0;			// prvek s fokusem (-1 = nen� nebo nezn�m�)
int		ButtonEsc = -1;				// tla��tko pro Esc

int		StdColorBtnText;			// syst�mov� barva textu tla��tek
int		StdColorBtnFace;			// syst�mov� barva podkladu tla��tek
HBRUSH	StdBrushBtn;				// �t�tec pozad� tla��tek

int		StdColorWindowText;			// syst�mov� barva textu editoru
int		StdColorWindow;				// syst�mov� barva podkladu editoru
HBRUSH	StdBrushWindow;				// �t�tec pozad� editoru

int		StdColorHighlightText;		// syst�mov� barva textu kurzoru
int		StdColorHighlight;			// syst�mov� barva pozad� kurzoru
HBRUSH	StdBrushHighlight;			// �t�tec pozad� kurzoru

// spr�vce font�
int			FontNext = FONTTABSTD;	// index p��t�ho voln�ho fontu k ulo�en�
FONTITEM*	FontTab = NULL;			// tabulka font�
HFONT		FontDefault = NULL;		// implicitn� syst�mov� font (p�i chyb� font�)
CString		UserFont(_T("Impact"));	// u�ivatelsk� font
CString		FontList;				// seznam syst�mov�ch font�

// DirectPlay
// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

bool	Coinit = false;				// prob�hla inicializace COM knihovny

LPDIRECTPLAY3A	DirectPlay	= NULL;	// objekt DirectPlay3 (NULL=nen�)

CString	DirectPlayConnects;			// seznam spojen� DirectPlay
int		DirectPlayConnectsNum = 0;	// po�et spojen� DirectPlay (0=nen� inicializov�n)
CString	DirectPlayConnectsAkt;		// aktivn� spojen� (pr�zdn� = nevybr�no)
int		DirectPlayConnectsAktN = -1; // aktivn� spojen� (-1 = nen�)
void**	DirectPlayConnectsConn = NULL;	// tabulka informac� spojen�
int		NextDirectPlayInit = 0;		// ��ta� do p��t�ho pokusu o otev�en� DirectPlay
CString ForDPlay1(" pro DirectPlay");	// ru�en� text 1
CString ForDPlay2(" for DirectPlay");	// ru�en� text 2
CString ForDPlay3(" f�r DirectPlay");	// ru�en� text 3

CString	DirectPlayGames;			// seznam her DirectPlay
int		DirectPlayGamesNum = 0;		// po�et her DirectPlay
CString	DirectPlayGamesAkt;			// aktivn� hra (pr�zdn� = nevybr�no)
int		DirectPlayGamesAktN = -1;	// aktivn� hra (-1 = nen� nebo nen� zat�m p�ihl�ena)
GUID	DirectPlayGamesAktGuid;		// GUID aktivn� hry
GUID*	DirectPlayGamesGuid = NULL;	// tabulka GUID her
int		DirectPlayGamesTime = 0;	// ��ta� pro aktualizaci seznamu her
DWORD	DirectPlayGamesParam[4];	// parametry hry (15 bit� ��slo + 0x4000, 16. bit = p��znak)
bool	DirectPlayGamesHost = false;// po��ta� je hostitelem
int		DirectPlayPlayersMax = 0;	// maxim�ln� po�et hr��� (0 = neomezeno)
int		DirectPlayGamesParamTime = 0; // ��ta� pro aktualizaci parametr� hry

#define MAXDIRECTPLAYERS 1024		// maxim�ln� po�et hr���
CString	DirectPlayPlayers;			// seznam hr��� DirectPlay
int		DirectPlayPlayersNum = 0;	// po�et hr��� DirectPlay (bez zru�en�ch)
CString	DirectPlayPlayersAkt;		// aktivn� hr�� (pr�zdn� = nen�)
int		DirectPlayPlayersAktN = -1;	// aktivn� hr�� (-1 = nen�)
DPID	DirectPlayPlayersAktID;		// ID aktivn�ho hr��e
bool	DirectPlayPlayersAktCreate = false; // hr�� byl vytvo�en
int		DirectPlayPlayersSize = 0;	// velikost tabulky hr��� (= po�et hr��� v�etn� zru�en�ch)
CString* DirectPlayPlayersNames = NULL;	// tabulka jmen hr��� (pr�zdn� text = nepou�ito)
DPID*	DirectPlayPlayersID = NULL; // tabulka ID hr���
int		DirectPlayPlayersTime = 0;	// ��ta� pro aktualizaci seznamu hr���
bool	DirectPlayPlayersIDErr = false; // chyba duplikace identifika�n�ho ��sla hr��e - nutn� zm�nit

BYTE*	DirectPlayDataOut = NULL;	// datov� blok v�stupn�ch dat (prvn� 2 bajty = odes�latel)
int		DirectPlayDataOutM = 1024;	// velikost bufferu vstupn�ch dat
int		DirectPlayDataOutN = 2;		// velikost v�stupn�ch dat

BYTE*	DirectPlayDataIn = NULL;	// datov� blok vstupn�ch dat (prvn� 2 bajty = odes�latel)
int		DirectPlayDataInN = 0;		// velikost vstupn�ch dat
int		DirectPlayDataInO = 0;		// offset �ten� ze vstupn�ch dat

// WSA 1.1 rozhran� pro DirectPlay
WSASTARTUP	pWSAStartup = NULL;		// funkce "WSAStartup"
WSACLEANUP	pWSACleanup = NULL;		// funkce "WSACleanup"
WSAGETHOSTNAME	pWSAGetHostName = NULL;// funkce "gethostname"
WSAGETHOSTBYNAME pWSAGetHostByName = NULL;// funkce "gethostbyname"
WSAINET_NTOA pWSAInetNtoa = NULL;	// funkce "inet_ntoa"

// roz���en� WSA 2.2 rozhran� pro UDP
WSASOCKET pWSASocket = NULL;			// funkce "WSASocketA"
WSAHTONS pWSAhtons = NULL;				// funkce "htons"
WSACREATEEVENT pWSACreateEvent = NULL;	// funkce "WSACreateEvent"
WSACLOSEEVENT pWSACloseEvent = NULL;	// funkce "WSACloseEvent"
WSASETSOCKOPT pWSASetSockOpt = NULL;	// funkce "setsockopt"
WSABIND pWSABind = NULL;				// funkce "bind"
WSACLOSESOCKET pWSACloseSocket = NULL;	// funkce "closesocket"
WSASENDTO pWSASendTo = NULL;			// funkce "WSASendTo"
WSARECVFROM pWSARecvFrom = NULL;		// funkce "WSARecvFrom"
WSAGETOVERLAPPEDRESULT pWSAGetOverlappedResult = NULL; // funkce WSAGetOverlappedResult"
WSAGETLASTERROR pWSAGetLastError = NULL; // funkce "WSAGetLastError"
WSASETEVENT pWSASetEvent = NULL;		// funkce "WSASetEvent"
WSAINETADDR pWSAInetAddr = NULL;		// funkce "inet_addr"

// knihovna WSA
HINSTANCE WSALibrary = NULL;		// WSA knihovna
bool	Wsaload = false;			// knihovna WSA byla na��t�na
bool	Wsainit = false;			// knihovna WSA 1.1 �sp�n� inicializov�na
bool	Wsainit2 = false;			// knihovna WSA 2.2 �sp�n� inicializov�na
WSADATA	Wsadata;					// data knihovny WSA
CString	HostIP(_T("0.0.0.0"));		// IP adresa po��ta�e
int		HostIPValid = 0;			// ��ta� platnosti IP adresy po��ta�e (je-li > 0)

EXTERN_C GUID DirectPlayGuidData = {
	'rteP',							// (4) identifik�tor typu dat "Petr"
	0,								// (2) identifik�tor 1
	0,								// (2) identifik�tor 2
	{0,0,0,0xc0,0x6c,0x47,6,0x65}	// (8) identifik�tor 3 (pouze prvn� 2 bajty, ostatn� nem�nit)
};

GUID*	DirectPlayGuid = &DirectPlayGuidData;		// identifikace
CString	DirectPlayGuidText;			// identifika�n� text pro DirectPlay

// porty (nen� u MINI verze)
COMITEM*	Coms;					// tabulky port�
int			ComAkt = 0;				// aktivn� port

// mixer (nen� u MINI verze)
int		MixDevN = 0;				// po�et mix�n�ch za��zen�
int		MixDevA = -1;				// aktivn� mix�n� za��zen� (-1=nen�)
CString	MixDevT;					// v�ce��dkov� seznam mix�n�ch za��zen�
MIXDEV*	MixDev = NULL;				// seznam mix�n�ch za��zen�
HMIXER	MixDevH = NULL;				// handle mixeru (NULL=nen�)

int		MixDest = 0;				// po�et c�lov�ch sign�l� mixeru
int		MixLineN = 0;				// po�et sign�l� mixeru
MIXLINE* MixLine = NULL;			// seznam sign�l� (na za��tku jsou c�lov�)
CString	MixLineDT;					// v�ce��dkov� seznam c�lov�ch sign�l�
CString	MixLineST;					// v�ce��dkov� seznam zdrojov�ch sign�l�
int		MixLineDA = -1;				// aktivn� c�lov� sign�l (-1=��dn�)
int		MixLineSA = -1;				// aktivn� zdrojov� sign�l c�lov�ho sign�lu (-1=jen c�lov�)
int		MixLineA = -1;				// aktivn� sign�l absolutn� (v tabulce) (-1=nen�)

int		MixCtrlN = 0;				// po�et ovl�dac�ch prvk� sign�lu
MIXCTRL* MixCtrl = NULL;			// seznam ovl�dac�ch prvk�
CString	MixCtrlT;					// v�ce��dkov� seznam ovl�dac�ch prvk�
int		MixCtrlA = -1;				// aktivn� ovl�dac� prvek (-1=nen�)
int		MixChannel = -1;			// nastavovan� kan�l (-1=v�echny)

int		MixValN = 0;				// po�et hodnot prvku (0=nen� seznam hodnot)
CString* MixVal = NULL;				// buffer jmen ovl�dac�ch prvk�
CString	MixValT;					// seznam hodnot prvku seznamu
int		MixValA = 0;				// aktivn� prvek seznamu (-1=nen�)

// obsluha DDE
CString		DDEDefApp;				// implicitn� jm�no aplikace pro DDE

DDEAPPITEM*	DDEAppList = NULL;		// seznam aplikac�
int			DDEAppNum = 0;			// po�et aplikac�
int			DDEAppMax = 0;			// velikost bufferu aplikac�
bool		DDEAppLoad = false;		// na��t� se seznam aplikac�

int			DDEAppAkt = -1;			// aktivn� aplikace (-1=v�echny)
CString		DDEAppAktName;			// jm�no aktivn� aplikace

DDESERVITEM* DDEServList = NULL;	// seznam sever�
int			DDEServNum = 0;			// po�et server�
int			DDEServMax = 0;			// velikost bufferu server�
int			DDEServAkt = -1;		// aktivn� server (-1=v�echny)

DDETOPITEM*	DDETopList = NULL;		// seznam t�mat
int			DDETopNum = 0;			// po�et t�mat
int			DDETopMax = 0;			// velikost bufferu t�mat
int			DDETopAkt = -1;			// aktivn� t�ma (-1=v�echny)

DDEDATAITEM* DDEDataList = NULL;	// seznam datov�ch polo�ek
int			DDEDataNum = 0;			// po�et datov�ch polo�ek
int			DDEDataMax = 0;			// velikost bufferu datov�ch polo�ek
int			DDEDataAkt = -1;		// aktivn� polo�ka (-1=v�echny)

// obsluha DLL
void*		DLLMemoryRead = NULL;	// ukazatel �ten� z pam�ti
void*		DLLMemoryWrite = NULL;	// ukazatel z�pisu do pam�ti
int			DLLMemoryTextNLen = 0;	// d�lka textu s pevnou d�lkou

// konfigurace
CString		INIFile;				// jm�no konfigura�n�ho souboru
CString		INISection(_T("Main"));	// jm�no konfigura�n� sekce
CString		INIKey(_T("Config"));	// jm�no konfigura�n�ho parametru
int			REGKey = 1;				// skupina registr� (1=CURRENT_USER, 2=LOCAL_MACHINE)
CString		REGSubkey(_T("Software\\")); // jm�no kl��e
CString		REGValue(_T("Config"));	// jm�no polo�ky

// UDP
SOCKET		UDPSendSocket = INVALID_SOCKET;	// UDP vys�lac� soket (INVALID_SOCKET = nen�)
SOCKADDR_IN	UDPSendAddr;			// vys�lac� adresa UDP
WSAOVERLAPPED UDPSendOver;			// overlapped struktura vys�la�e
WSABUF		UDPSendBuf;				// odes�lac� buffer UDP
bool		UDPSending = false;		// prob�h� vys�l�n� UDP

SOCKET		UDPRecSocket = INVALID_SOCKET; // UDP p�ij�mac� soket (INVALID_SOCKET = nen�)
SOCKADDR_IN	UDPRecAddr;				// p�ij�mac� adresa UDP
SOCKADDR_IN	UDPRecRecAddr;			// p�ijat� adresa UDP
WSAOVERLAPPED UDPRecOver;			// overlapped struktura p�ij�ma�e
WSABUF		UDPRecBuf;				// p�ij�mac� buffer UDP
bool		UDPReceiving = false;	// prob�h� overlapped p��jem UDP
int			UDPRecData = 0;			// velikost dat v p�ij�mac�m bufferu

/***************************************************************************\
*																			*
*								obsluha DDE									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// zru�en� seznamu server�

void DDEAppListTerm()
{
	int i;

// zru�en� seznamu polo�ek
	for (i = DDEDataNum-1; i >= 0; i--)
	{
		DDEDataList[i].DataName.Term();
		DDEDataList[i].DataText.Term();
		DDEDataList[i].DataIcon.Term();
		DDEDataList[i].DataPicture.Term();
		DDEDataList[i].DataSound.Term();
		DDEDataList[i].DataMusic.Term();
		DDEDataList[i].DataSprite.Term();
	}

	MemFree(DDEDataList);
	DDEDataList = NULL;
	DDEDataNum = 0;
	DDEDataMax = 0;

// zru�en� seznamu t�mat
	for (i = DDETopNum-1; i >= 0; i--)
	{
		DDETopList[i].TopicName.Term();
	}

	MemFree(DDETopList);
	DDETopList = NULL;
	DDETopNum = 0;
	DDETopMax = 0;

// zru�en� seznamu server�
	for (i = DDEServNum-1; i >= 0; i--)
	{
		DDEServList[i].ServName.Term();
		DDEServList[i].Execute.Term();
	}
	
	MemFree(DDEServList);
	DDEServList = NULL;
	DDEServNum = 0;
	DDEServMax = 0;

// zru�en� seznamu aplikac�
	for (i = DDEAppNum-1; i >= 0; i--)
	{
		DDEAppList[i].AppName.Term();
	}

	MemFree(DDEAppList);
	DDEAppList = NULL;
	DDEAppNum = 0;
	DDEAppMax = 0;
}

/////////////////////////////////////////////////////////////////////////////
// na�ten� seznamu aplikac� a t�mat

void DDEAppListLoad()
{
// zru�en� seznamu aplikac�
	DDEAppListTerm();

// zji�t�n� seznamu aplikac� a t�mat
	DDEAppLoad = true;
	::SendMessage((HWND)-1, WM_DDE_INITIATE, (WPARAM)MainFrame, NULL);
	DDEAppLoad = false;
}

/***************************************************************************\
*																			*
*								obsluha mixeru								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// inicializace seznamu mix�n�ch za��zen� (vrac� TRUE=OK)

bool MixDevInit()
{
	if (MixDevN == 0)
	{

// zji�t�n� po�tu mix�n�ch za��zen�
		int i = ::mixerGetNumDevs();
		if (i <= 0)
		{
			return false;
		}
		MixDevN = i;

// vytvo�en� bufferu mix�n�ch za��zen�
		MIXDEV* md = (MIXDEV*)MemGet(MixDevN * sizeof(MIXDEV));
		MixDev = md;

// na�ten� informac� o jednotliv�ch mix�n�ch za��zen�
		MIXERCAPS mc;

		for (i = 0; i < MixDevN; i++)
		{
			::mixerGetDevCaps(i, &mc, sizeof(mc));

			md->Dest = mc.cDestinations;

			md->Name.Init(mc.szPname);

			MixDevT += md->Name;
			MixDevT += 13;
			MixDevT += 10;
		}

// otev�en� implicitn�ho mixeru
		if (!MixDevSet(0)) return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// p�id�n� linky do seznamu linek

void MixAddLine(MIXERLINE* ml)
{
// vytvo�en� nov� polo�ky
	MixLineN++;
	MixLine = (MIXLINE*)MemSize(MixLine, MixLineN*sizeof(MIXLINE));
	MIXLINE* item = &MixLine[MixLineN-1];

// na�ten� jm�na linky
	item->Name.Init(ml->szName);
	if (item->Name.IsEmpty())
	{
		item->Name.Term();
		item->Name.Init(ml->szShortName);
	}

// c�lov� sign�l
	item->Dest = ml->dwDestination;

// zdrojov� sign�l (pro c�lov� je = -1)
	item->Source = ml->dwSource;

// identifik�tor sign�lu
	item->LineID = ml->dwLineID;

// typ sign�lu
	item->Type = ml->dwComponentType;

// po�et kan�l� (nesm� b�t <= 0!)
	item->Channels = ml->cChannels;
	if (item->Channels < 1) item->Channels = 1;

// po�et zdroj�
	item->Sources = ml->cConnections;

// po�et ovl�dac�ch prvk�
	item->Controls = ml->cControls;
}

/////////////////////////////////////////////////////////////////////////////
// zru�en� bufferu hodnot prvku

void MixValTerm()
{
	for (int i = MixValN-1; i >= 0; i--)
	{
		MixVal[i].Term();
	}
	MixValN = 0;
	MemFree(MixVal);
	MixVal = NULL;
	MixValT.Empty();
	MixValA = 0;
}

/////////////////////////////////////////////////////////////////////////////
// zru�en� bufferu ovl�dac�ch prvk�

void MixCtrlTerm()
{
	for (int i = MixCtrlN-1; i >= 0; i--)
	{
		MixCtrl[i].Name.Term();
	}
	MixCtrlN = 0;
	MemFree(MixCtrl);
	MixCtrl = NULL;
	MixCtrlT.Empty();
	MixCtrlA = -1;

	MixValTerm();
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� aktivn�ho mixeru (vrac� TRUE=OK)

bool MixDevSet(int mix)
{
// inicializace mixeru
	if (!MixDevInit()) return false;

// korekce ��sla mixeru
	if ((DWORD)mix >= (DWORD)MixDevN) mix = MixDevN-1;

// test, zda se ��slo mixeru li��
	if (mix != MixDevA)
	{

// uzav�en� p�vodn�ho mixeru
		if (MixDevH != NULL)
		{
			::mixerClose(MixDevH);
		}

// zru�en� p�vodn�ho bufferu sign�l�
		int i;
		if (MixLineN > 0)
		{
			for (i = 0; i < MixLineN; i++)
			{
				MixLine[i].Name.Term();
			}
			MixLineN = 0;
		}

		MemFree(MixLine);
		MixLine = NULL;
		MixLineDT.Empty();
		MixLineST.Empty();
		MixLineDA = -1;
		MixLineSA = -1;
		MixLineA = -1;

// zru�en� p�vodn�ho bufferu ovl�dac�ch prvk�
		MixCtrlTerm();

// otev�en� nov�ho mixeru
		if (::mixerOpen(&MixDevH, mix, 0, 0, MIXER_OBJECTF_MIXER) != MMSYSERR_NOERROR)
		{
			MixDevH = NULL;
			MixDevA = -1;
			MixDest = 0;
			return false;
		}

// nov� mixer je platn�
		MixDevA = mix;
		MixDest = MixDev[mix].Dest;

// na�ten� c�lov�ch sign�l�
		MIXERLINE ml;

		for (i = 0; i < MixDest; i++)
		{
			ml.cbStruct = sizeof(ml);
			ml.dwDestination = i;
			::mixerGetLineInfo((HMIXEROBJ)MixDevH, &ml, MIXER_GETLINEINFOF_DESTINATION | MIXER_OBJECTF_HMIXER);
			ml.dwSource = (DWORD)-1;
			MixAddLine(&ml);

			MixLineDT += MixLine[MixLineN-1].Name;
			MixLineDT += 13;
			MixLineDT += 10;
		}
		
// na�ten� zdrojov�ch sign�l�
		for (i = 0; i < MixDest; i++)
		{
			int prvni = MixLineN;
			MixLine[i].SourceI = prvni;
			int pocet = MixLine[i].Sources;

			for (int j = 0; j < pocet; j++)
			{
				ml.cbStruct = sizeof(ml);
				ml.dwDestination = i;
				ml.dwSource = j;
				::mixerGetLineInfo((HMIXEROBJ)MixDevH, &ml, MIXER_GETLINEINFOF_SOURCE | MIXER_OBJECTF_HMIXER);
				MixAddLine(&ml);
			}
		}

// nastaven� implicitn�ho c�lov�ho sign�lu
		MixDstSet(0);
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� c�lov�ho sign�lu (-1=��dn�)

void MixDstSet(int dst)
{
// inicializace mixeru
	if (!MixDevInit()) return;

// zru�en� p�vodn�ch buffer�
	MixLineST.Empty();
	MixLineDA = dst;
	MixLineA = dst;
	MixLineSA = -1;

// zru�en� p�vodn�ho bufferu ovl�dac�ch prvk�
	MixCtrlTerm();

// korekce ��sla sign�lu
	if ((DWORD)dst >= (DWORD)MixDest)
	{
		MixLineDA = -1;
		MixLineA = -1;
		return;
	}

// sestaven� seznamu zdrojov�ch sign�l�
	int prvni = MixLine[dst].SourceI;
	int pocet = MixLine[dst].Sources;

	MIXLINE* item = &MixLine[prvni];

	for (int i = 0; i < pocet; i++)
	{
		MixLineST += item->Name;
		MixLineST += 13;
		MixLineST += 10;
		item++;
	}

// implicitn� zdrojov� sign�l
	MixSrcSet(-1);	
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� zdrojov�ho sign�lu (-1=jen c�lov�)

void MixSrcSet(int src)
{
// zru�en� p�vodn�ho bufferu ovl�dac�ch prvk�
	MixCtrlTerm();

// inicializace mixeru
	MixLineSA = -1;
	MixLineA = -1;
	if ((!MixDevInit()) || ((DWORD)MixLineDA >= (DWORD)MixDest)) return;

// korekce ��sla sign�lu
	int prvni = MixLine[MixLineDA].SourceI;
	int pocet = MixLine[MixLineDA].Sources;

	if ((DWORD)src >= (DWORD)pocet)
	{
		MixLineA = MixLineDA;
	}
	else
	{
		MixLineSA = src;
		MixLineA = src + prvni;
	}

// na�ten� seznamu ovl�dac�ch prvk�
	MIXLINE* item = &MixLine[MixLineA];
	pocet = item->Controls;
	int chan = item->Channels;
	if (pocet == 0) return;
	
	MixCtrl = (MIXCTRL*)MemGet(pocet * sizeof(MIXCTRL));
	MixCtrlN = pocet;

	MIXERCONTROL* mc = (MIXERCONTROL*)MemGet(pocet * sizeof(MIXERCONTROL));
	MIXERLINECONTROLS ml;
	ml.cbStruct = sizeof(MIXERLINECONTROLS);
	ml.dwLineID = item->LineID;
	ml.cControls = pocet;
	ml.cbmxctrl = sizeof(MIXERCONTROL);
	ml.pamxctrl = mc;
	::mixerGetLineControls((HMIXEROBJ)MixDevH, &ml, MIXER_GETLINECONTROLSF_ALL | MIXER_OBJECTF_HMIXER);

// �schova parametr� ovl�dac�ch prvk�
	MIXCTRL* d = MixCtrl;
	MIXERCONTROL* s = mc;

	for (int i = 0; i < pocet; i++)
	{
		d->Name.Init(s->szName);
		if (d->Name.IsEmpty())
		{
			d->Name.Term();
			d->Name.Init(s->szShortName);
		}
		MixCtrlT += d->Name;
		MixCtrlT += 13;
		MixCtrlT += 10;

		d->ControlID = s->dwControlID;
		d->Type = s->dwControlType;
		d->Steps = s->Metrics.cSteps;
		d->Channels = chan;
		d->List = s->cMultipleItems;
		if (((s->fdwControl & MIXERCONTROL_CONTROLF_MULTIPLE) == 0) || (d->List < 1))
		{
			d->List = 1;
		}

		if (s->fdwControl & MIXERCONTROL_CONTROLF_UNIFORM) d->Channels = 1;

		if (s->Bounds.lMinimum < 0)
		{
			d->Min = s->Bounds.lMinimum;
			d->Max = s->Bounds.lMaximum;
		}
		else
		{
			d->Min = s->Bounds.dwMinimum;
			d->Max = s->Bounds.dwMaximum;
		}

		d->Rozsah = d->Max - d->Min;
		if (d->Rozsah == 0)
		{
			d->Rozsah = 1;
			d->Max = d->Min + 1;
		}

		if (d->Steps == 0)
		{
			d->Inc = 1;
		}
		else
		{
			d->Inc = d->Rozsah / d->Steps;
		}

		s++;
		d++;
	}

	MemFree(mc);

	MixCtrlSet(0);
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� ovl�dac�ho prvku (-1=��dn�)

void MixCtrlSet(int ctrl)
{
// inicializace mixeru
	if (!MixDevInit()) return;

// kontrola ��sla prvku
	if ((DWORD)ctrl >= (DWORD)MixCtrlN) ctrl = -1;

// nastaven� aktivn�ho prvku
	MixCtrlA = ctrl;

// zji�t�n� seznamu hodnot prvku seznamu
	MixValTerm();

	if (ctrl >= 0)
	{
		MIXCTRL* item = &MixCtrl[ctrl];

		if (item->List > 1)
		{
			MixValN = item->List;
			MixVal = (CString*)MemGet(MixValN * sizeof(CString));

			MIXERCONTROLDETAILS_LISTTEXT* dat = (MIXERCONTROLDETAILS_LISTTEXT*)MemGet(
						MixValN * sizeof(MIXERCONTROLDETAILS_LISTTEXT));

			MIXERCONTROLDETAILS mcd;
			mcd.cbStruct = sizeof(mcd);
			mcd.dwControlID = item->ControlID;
			mcd.cChannels = 1;
			mcd.cMultipleItems = MixValN;
			mcd.cbDetails = sizeof(MIXERCONTROLDETAILS_LISTTEXT);
			mcd.paDetails = dat;

			::mixerGetControlDetails((HMIXEROBJ)MixDevH, &mcd,
				MIXER_GETCONTROLDETAILSF_LISTTEXT | MIXER_OBJECTF_HMIXER);

			for (int i = 0; i < MixValN; i++)
			{
				MixVal[i].Init(dat[i].szName);
				MixValT += MixVal[i];
				MixValT += 13;
				MixValT += 10;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� hodnoty ovl�dac�ho prvku

double GetMixCtrlVal()
{
// inicializace mixeru
	if (!MixDevInit() || ((DWORD)MixCtrlA >= (DWORD)MixCtrlN)) return 0;
	MIXCTRL* item = &MixCtrl[MixCtrlA];

// test, zda je seznam
	bool list = false;
	if (MixValN > 1) list = true;
	if (list && ((DWORD)MixValA >= (DWORD)item->List)) return 0;

// test, zda je multikan�l
	bool multi = false;
	if ((DWORD)MixChannel >= (DWORD)item->Channels) multi = true;

// na�ten� stavu prvku
	MIXERCONTROLDETAILS_SIGNED* dat = (MIXERCONTROLDETAILS_SIGNED*)MemGet(
				item->Channels * item->List * sizeof(MIXERCONTROLDETAILS_SIGNED));
	MIXERCONTROLDETAILS_SIGNED* data = dat;

	MIXERCONTROLDETAILS mcd;
	mcd.cbStruct = sizeof(mcd);
	mcd.dwControlID = item->ControlID;
	mcd.cChannels = 1;
	mcd.cMultipleItems = 0;

	if (!multi)
	{
		mcd.cChannels = item->Channels;
		data = &dat[MixChannel * item->List];
	}

	if (list)
	{
		mcd.cMultipleItems = MixValN;
		data += MixValA;
	}

	mcd.cbDetails = sizeof(MIXERCONTROLDETAILS_SIGNED); // plat� i pro UNSIGNED a BOOLEAN
	mcd.paDetails = dat;

	if (::mixerGetControlDetails((HMIXEROBJ)MixDevH, &mcd,
			MIXER_GETCONTROLDETAILSF_VALUE | MIXER_OBJECTF_HMIXER) != MMSYSERR_NOERROR)
	{
		data->lValue = 0;
	}

	double d;

	if (item->Min < 0)
	{
		d = data->lValue;
	}
	else
	{
		d = (DWORD)data->lValue;
	}

	if ((item->Type & 0x00FF0000) == 0x00010000)	// typ dat BOOLEAN
	{
		if (d != 0) d = 1;
	}
	else
	{
		d = Round((d - item->Min)/item->Inc) * item->Inc / item->Rozsah;
	}

	MemFree(dat);
	return d;
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� hodnoty ovl�dac�ho prvku

void SetMixCtrlVal(double val)
{
// inicializace mixeru
	if (!MixDevInit() || ((DWORD)MixCtrlA >= (DWORD)MixCtrlN)) return;
	MIXCTRL* item = &MixCtrl[MixCtrlA];

// test, zda je seznam
	bool list = false;
	if (MixValN > 1) list = true;
	if (list && ((DWORD)MixValA >= (DWORD)item->List)) return;

// test, zda je multikan�l
	bool multi = false;
	if ((DWORD)MixChannel >= (DWORD)item->Channels) multi = true;

// korekce hodnoty
	if (val > 1) val = 1;
	if (val < 0) val = 0;

// na�ten� aktu�ln�ho stavu prvku
	MIXERCONTROLDETAILS_SIGNED* dat = (MIXERCONTROLDETAILS_SIGNED*)MemGet(
				item->Channels * item->List * sizeof(MIXERCONTROLDETAILS_SIGNED));
	MIXERCONTROLDETAILS_SIGNED* data = dat;
	
	MIXERCONTROLDETAILS mcd;
	mcd.cbStruct = sizeof(mcd);
	mcd.dwControlID = item->ControlID;
	mcd.cChannels = 1;
	mcd.cMultipleItems = 0;

	if (!multi)
	{
		mcd.cChannels = item->Channels;
		data = &dat[MixChannel * item->List];
	}

	if (list)
	{
		mcd.cMultipleItems = MixValN;
		data += MixValA;
	}

	mcd.cbDetails = sizeof(MIXERCONTROLDETAILS_SIGNED); // plat� i pro UNSIGNED a BOOLEAN
	mcd.paDetails = dat;

	if (::mixerGetControlDetails((HMIXEROBJ)MixDevH, &mcd,
				MIXER_GETCONTROLDETAILSF_VALUE | MIXER_OBJECTF_HMIXER) != MMSYSERR_NOERROR)
	{
		data->lValue = 0x7fffffff;
	}

// v�po�et aktu�ln�ho a po�adovan�ho kroku
	double d;

	if (item->Min < 0)
	{
		d = data->lValue;
	}
	else
	{
		d = (DWORD)data->lValue;
	}

	int stary;
	int novy;

	if ((item->Type & 0x00FF0000) == 0x00010000)	// typ dat BOOLEAN
	{
		stary = 0;
		if (d != 0) stary = 1;

		novy = 0;
		if (val > 0.5) novy = 1;

		if (list && ((item->Type & 0xFF000000) == 0x70000000)) // typ dat SINGLE SELECT
		{
			if (novy == 0) stary = 0;
			if ((novy == 1) && (stary == 0))
			{
				MIXERCONTROLDETAILS_SIGNED* x = dat;
				if (!multi)
				{
					x = &dat[MixChannel * item->List];
				}

				for (int i = 0; i < item->List; i++)
				{
					x->lValue = 0;
					x++;
				}
			}
		}
	}
	else
	{
		stary =  Round((d - item->Min)/item->Inc);
		novy = Round(val * item->Rozsah / item->Inc);
	}

// nastaven� nov�ho stavu, pokud se li��
	if (stary != novy)
	{
		if ((item->Type & 0x00FF0000) == 0x00010000)	// typ dat BOOLEAN
		{
			data->lValue = novy;
		}
		else
		{
			data->lValue = Round(novy * item->Inc + item->Min);
		}

		::mixerSetControlDetails((HMIXEROBJ)MixDevH, &mcd,
				MIXER_SETCONTROLDETAILSF_VALUE | MIXER_OBJECTF_HMIXER);
	}

	MemFree(dat);
}

/***************************************************************************\
*																			*
*								Obsluha port�								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// uzav�en� portu (i=platn� index)

void ComClose(int i)
{
	COMITEM* item = Coms + i;
	HANDLE file = item->File;

	if (file != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(file);
		item->File = INVALID_HANDLE_VALUE;
	}
}

/////////////////////////////////////////////////////////////////////////////
// na�ten� DCB aktivn�ho portu

void GetComDCB()
{
// adresa popisova�e portu
	COMITEM* item = Coms + ComAkt;
	DCB* dcb = &(item->Dcb);
	dcb->DCBlength = sizeof(DCB);
	HANDLE file = item->File;

// na�ten� DCB portu
	if ((file == INVALID_HANDLE_VALUE) ||
		!::GetCommState(file, dcb))
	{
		MemFill(dcb, sizeof(DCB), 0);
		dcb->BaudRate = 1200;
		dcb->ByteSize = 8;
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� DCB aktivn�ho portu

void SetComDCB()
{
// adresa popisova�e portu
	COMITEM* item = Coms + ComAkt;
	DCB* dcb = &(item->Dcb);
	dcb->DCBlength = sizeof(DCB);
	HANDLE file = item->File;

// z�pis DCB portu
	if (file != INVALID_HANDLE_VALUE)
	{
		if (dcb->StopBits)
		{
			if (dcb->ByteSize <= 5)
			{
				dcb->StopBits = ONE5STOPBITS;
			}
			else
			{
				dcb->StopBits = TWOSTOPBITS;
			}
		}

		::PurgeComm(file, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		::SetCommState(file, dcb);
		GetComDCB();
	}
}

/////////////////////////////////////////////////////////////////////////////
// vysl�n� bajtu na komunika�n� port

void ComSend(BYTE data)
{
	COMITEM* item = Coms + ComAkt;
	HANDLE file = item->File;
	if (file != INVALID_HANDLE_VALUE)
	{
		switch (item->Dcb.ByteSize)
		{
		case 5:
			data &= 0x1f;
			break;

		case 6:
			data &= 0x3f;
			break;

		case 7:
			data &= 0x7f;
			break;
		}

		DWORD writen;

		::WriteFile(
			file,		// handle za��zen�
			&data,		// adresa dat
			1,			// po�et bajt� k z�pisu
			&writen,	// ukazatel zapsan�ch dat
			NULL);		// p�ekryv
	}
}

/////////////////////////////////////////////////////////////////////////////
// p��jem bajtu z komunika�n�ho portu (0=nejsou data)

BYTE ComReceive()
{
	BYTE data = 0;

	COMITEM* item = Coms + ComAkt;
	HANDLE file = item->File;
	if (file != INVALID_HANDLE_VALUE)
	{
		DWORD readen;

		::ReadFile(
			file,		// handle za��zen�
			&data,		// adresa bufferu dat
			1,			// po�et bajt� k na�ten�
			&readen,	// ukazatel na�ten�ch dat
			NULL);		// p�ekryv

		switch (item->Dcb.ByteSize)
		{
		case 5:
			data &= 0x1f;
			break;

		case 6:
			data &= 0x3f;
			break;

		case 7:
			data &= 0x7f;
			break;
		}
	}

	return data;
}


/***************************************************************************\
*																			*
*								DirectPlay									*
*						(v helpu hledat IDirectPlay3::)						*
*																			*
\***************************************************************************/

// ------------------------ obsluha p�enosu dat -----------------------------

/////////////////////////////////////////////////////////////////////////////
// z�pis dat do v�stupn�ho bufferu

void DirectSendData(BYTE* data, int n)
{
// test, zda je platn� hr��
	if (!DirectPlayPlayersAktCreate) return;

// zv��en� velikosti bufferu
	if ((DirectPlayDataOutN + n) > DirectPlayDataOutM)
	{
		DirectPlayDataOutM = (DirectPlayDataOutN + n + 0x400 + 0x3ff) & ~0x3ff;
		DirectPlayDataOut = (BYTE*)MemSize(DirectPlayDataOut, DirectPlayDataOutM);
	}

// ulo�en� dat do bufferu
	MemCopy(DirectPlayDataOut + DirectPlayDataOutN, data, n);

// zv��en� ��ta�e dat v bufferu
	DirectPlayDataOutN += n;
}


/////////////////////////////////////////////////////////////////////////////
// vysl�n� bloku dat (-1 = v�em)

void DirectSend(int id)
{
// test, zda jsou n�jak� data k odesl�n�
	if (DirectPlayDataOutN <= 2) return;

// nelze zaslat zpr�vu s�m sob�
	if (id == DirectPlayPlayersAktN)
	{
		DirectPlayDataOutN = 2;
		return;
	}

// test, zda je platn� hr��
	if (!DirectPlayPlayersAktCreate) return;

// p��prava ��sla odes�latele
	*((WORD*)DirectPlayDataOut) = (WORD)DirectPlayPlayersAktN;

// p��prava ��sla p��jemce
	DPID dst = DPID_ALLPLAYERS;
	if (((DWORD)id < (DWORD)DirectPlayPlayersSize) &&
		DirectPlayPlayersNames[id].IsNotEmpty())
	{
		dst = DirectPlayPlayersID[id];
	}

// odesl�n� bloku dat
	DirectPlay->Send(DirectPlayPlayersAktID, dst, 0 /*DPSEND_GUARANTEED*/, DirectPlayDataOut, DirectPlayDataOutN);

// zru�en� v�stupn�ch dat
	DirectPlayDataOutN = 2;
}


/////////////////////////////////////////////////////////////////////////////
// �ten� dat ze vstupn�ho bufferu (p�i nedostatku dat dopln� nulami)

void DirectReceiveData(BYTE* data, int n)
{
// po�et dat k p�enesen�
	int i = DirectPlayDataInN - DirectPlayDataInO;
	if (i > n) i = n;

// p�enesen� platn�ch dat
	if (i > 0)
	{
		MemCopy(data, DirectPlayDataIn + DirectPlayDataInO, i);
		DirectPlayDataInO += i;
	}

// vymaz�n� zbytku dat
	if (n > i)
	{
		MemFill(data + i, n - i, 0);
	}
}


/////////////////////////////////////////////////////////////////////////////
// p��jem bloku dat (-1 = nen�)

int DirectReceive()
{
// test, zda je platn� hr��
	if (!DirectPlayPlayersAktCreate) return -1;

// nulov�n� ��ta�e pro aktualizaci parametr� hry (p�ij�m� se jako syst�mov� zpr�va)
	DirectPlayGamesParamTime = 2*18;

// p��prava pracovn�ch prom�nn�ch
	DPID src;
	DPID dst;
	HRESULT res;
	DirectPlayDataInN = 1023;

// cyklus �ten�, dokud nebude spr�vn� velikost bufferu
	do
	{

// p��prava vstupn�ho bufferu (na konci rezerva pro zakon�ovac� nulu)
		MemFree(DirectPlayDataIn);
		DirectPlayDataIn = (BYTE*)MemGet(DirectPlayDataInN + 1);

// na�ten� zpr�vy
		dst = DirectPlayPlayersAktID;
		res = DirectPlay->Receive(&src, &dst, DPRECEIVE_TOPLAYER, DirectPlayDataIn, (DWORD*)&DirectPlayDataInN);

// p��jem syst�mov� zpr�vy
		if ((res == DP_OK) && (src == DPID_SYSMSG))
		{
			DPMSG_GENERIC* msg = (DPMSG_GENERIC*)DirectPlayDataIn;
			switch(msg->dwType)
			{
			case DPSYS_CREATEPLAYERORGROUP:		// vytvo�en nov� hr��
			case DPSYS_DELETEPLAYERFROMGROUP:	// hr�� zru�en ze skupiny
			case DPSYS_DESTROYPLAYERORGROUP:	// hr�� zru�en ze hry
			case DPSYS_SETPLAYERORGROUPDATA:	// zm�n�na data hr��e
			case DPSYS_SETPLAYERORGROUPNAME:	// zm�n�no jm�no hr��e
				DirectPlayPlayersTime = 0;
				break;

			case DPSYS_SETSESSIONDESC:			// zm�n�no nastaven� hry
				GetGameDesc();					// znovuna�ten� parametr� hry
				break;

			case DPSYS_HOST:					// po��ta� se st�v� hostitelem
				DirectPlayGamesHost = true;
				break;
			}
			DirectPlayDataInN = 1023;
			res = DPERR_BUFFERTOOSMALL;
		}

// vlastn� zpr�va se ignoruje
		if ((res == DP_OK) && (DirectPlayDataInN > 2) && (*(WORD*)DirectPlayDataIn == (WORD)DirectPlayPlayersAktN))
		{
			DirectPlayDataInN = 1023;
			res = DPERR_BUFFERTOOSMALL;
		}

// p�i mal�m bufferu opakov�n�
	} while (res == DPERR_BUFFERTOOSMALL);

// test p��jmu dat OK
	if ((res == DP_OK) && (DirectPlayDataInN > 2))
	{
		DirectPlayDataIn[DirectPlayDataInN] = 0;	// zakon�ovac� nula text�
		DirectPlayDataInO = 2;						// offset �ten�ch dat
		return *(WORD*)DirectPlayDataIn;			// ��slo odes�latele
	}
	else
	{
		DirectPlayDataInO = 0;
		DirectPlayDataInN = 0;
		return -1;
	}
}



// ------------------------ obsluha seznamu hr��� ---------------------------

/////////////////////////////////////////////////////////////////////////////
// nastaven� aktivn�ho hr��e (pr�zdn� = vypnut�)

void SetPlayerAct(CString txt)
{
// aktualizace seznamu hr��� (dvakr�t kv�li korekci chyby p�ekryvu identifika�n�ho ��sla)
	DirectPlayEnumPlayers();
	DirectPlayEnumPlayers();

// korekce textu
	txt.TrimLeft();
	txt.TrimRight();

// test, zda je zm�na jm�na
	if (txt == DirectPlayPlayersAkt) return;

// zru�en� star�ho hr��e
	if (txt.IsEmpty())
	{
		if (DirectPlayPlayersAktCreate)
		{
			DirectPlay->DestroyPlayer(DirectPlayPlayersAktID);
			DirectPlayPlayersNames[DirectPlayPlayersAktN].Empty();
			DirectPlayPlayersAkt.Empty();
			DirectPlayPlayersAktN = -1;
			DirectPlayPlayersAktCreate = false;

			DirectPlayDataOutN = 2;
			DirectPlayDataInO = 0;
			DirectPlayDataInN = 0;
		}
		return;
	}

// test, zda je otev�ena hra
	if (DirectPlayGamesAkt.IsEmpty()) return;

// zaji�t�n� jedine�nosti jm�na nov�ho hr��e
	int n = 2;

	for (int ii = 0; ii < DirectPlayPlayersSize; ii++)
	{
		if ((txt == DirectPlayPlayersNames[ii]) && (txt != DirectPlayPlayersAkt))
		{
			txt.SetNumObj(n);
			n++;
			ii = -1;
		}
	}

// test, zda je nutn� m�nit jm�no po korekci
	if (txt == DirectPlayPlayersAkt) return;

// p��prava parametr� hr��e
	DPNAME name;
	MemFill(&name, sizeof(DPNAME), 0);
	name.dwSize = sizeof(DPNAME);
	name.lpszShortNameA = (LPTSTR)(LPCTSTR)txt;

// p�ejmenov�n� hr��e
	if (DirectPlayPlayersAktCreate)
	{
		if (DirectPlay->SetPlayerName(DirectPlayPlayersAktID, &name, DPSET_GUARANTEED) == DP_OK)
		{
			DirectPlayPlayersAkt = txt;
		}
	}

	else
	{

// p��prava identifika�n�ho k�du hr��e
		long id = 0;
		for (; id < DirectPlayPlayersSize; id++)
		{
			if (DirectPlayPlayersNames[id].IsEmpty()) break;
		}

// vytvo�en� hr��e
		if (DirectPlay->CreatePlayer(&DirectPlayPlayersAktID, &name, NULL, &id, sizeof(long), 0) == DP_OK)
		{
			DirectPlayPlayersAktCreate = true;
			DirectPlayPlayersAkt = txt;
		}
	}

// aktualizace seznamu hr��� (opakovan� pro p��pad duplikace ��sla)
	DirectPlayPlayersTime = 0;
	DirectPlayEnumPlayers();
	DirectPlayEnumPlayers();
}


/////////////////////////////////////////////////////////////////////////////
// callback pro zji�t�n� seznamu hr���

BOOL FAR PASCAL EnumPlayersCallback2(
  DPID dpId,
  DWORD dwPlayerType,
  LPCDPNAME lpName,
  DWORD dwFlags,
  LPVOID lpContext
  )
{
// p��prava textu jm�na hr��e
	CString txt(lpName->lpszShortNameA);
	if (txt.IsEmpty()) txt = lpName->lpszLongNameA;
	txt.TrimLeft();
	txt.TrimRight();
	if (txt.IsEmpty()) return TRUE;

// p��prava a kontrola ��sla hr��e
	long id = DirectPlayPlayersSize;
	DWORD size = sizeof(long);
	DirectPlay->GetPlayerData(dpId, &id, &size, 0);
	if ((DWORD)id >= (DWORD)MAXDIRECTPLAYERS) return TRUE;

// zv��en� velikosti buffer�
	if (id >= DirectPlayPlayersSize)
	{
		DirectPlayPlayersNames = (CString*)MemSize(DirectPlayPlayersNames, (id + 1) * sizeof(CString));
		for (; DirectPlayPlayersSize <= id; DirectPlayPlayersSize++)
		{
			DirectPlayPlayersNames[DirectPlayPlayersSize].Init();
		}
		DirectPlayPlayersID = (DPID*)MemSize(DirectPlayPlayersID, DirectPlayPlayersSize * sizeof(DPID));
	}

// zv��en� ��ta�e platn�ch hr���
	DirectPlayPlayersNum++;

// korekce aktivn�ho hr��e
	if (DirectPlayPlayersAktCreate)
	{

// test, zda to je aktivn� hr��
		if (dpId == DirectPlayPlayersAktID)
		{
			DirectPlayPlayersAkt = txt;
			DirectPlayPlayersAktN = id;

// kontrola p�ekryvu ��sel hr��� (korekci prov�d� hr�� s vy���m ID)
			if (DirectPlayPlayersNames[id].IsNotEmpty() && (DirectPlayPlayersID[id] < dpId))
			{
				DirectPlayPlayersIDErr = true;
			}
		}
		else
		{

// kontrola p�ekryvu ��sel hr��� (korekci prov�d� hr�� s vy���m ID)
			if (DirectPlayPlayersNames[id].IsNotEmpty() && 
				(DirectPlayPlayersID[id] == DirectPlayPlayersAktID) &&
				(DirectPlayPlayersAktID > dpId))
			{
				DirectPlayPlayersIDErr = true;
			}
		}
	}

// ulo�en� parametr� do seznamu
	DirectPlayPlayersNames[id] = txt;
	DirectPlayPlayersID[id] = dpId;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// seznam hr��� (FALSE=chyba)

bool DirectPlayEnumPlayers()
{
// test, zda je seznam je�t� platn�
	if (DirectPlayPlayersTime > 0) return true;

// test, zda je vybr�na hra
	if (DirectPlayGamesAkt.IsEmpty()) return false;

// zru�en� bufferu jmen a identifikace hr���
	for (int i = DirectPlayPlayersSize - 1; i >= 0; i--)
	{
		DirectPlayPlayersNames[i].Term();
	}
	MemFree(DirectPlayPlayersNames);
	DirectPlayPlayersNames = NULL;
	MemFree(DirectPlayPlayersID);
	DirectPlayPlayersID = NULL;
	DirectPlayPlayersSize = 0;

// zji�t�n� hr���
	DirectPlayPlayersNum = 0;
	DirectPlayPlayersAkt.Empty();
	DirectPlayPlayersAktN = -1;
	DirectPlayPlayersIDErr = false;
	DirectPlay->EnumPlayers(NULL, EnumPlayersCallback2, NULL, DPENUMPLAYERS_ALL);

// sestaven� textu seznamu jmen hr���
	DirectPlayPlayers.Empty();
	for (i = 0; i < DirectPlayPlayersSize; i++)
	{
		DirectPlayPlayers += DirectPlayPlayersNames[i];
		DirectPlayPlayers += _T(13);
		DirectPlayPlayers += _T(10);
	}

// nov� ��ta� platnosti seznamu
	if (DirectPlayPlayersNum == 0)
	{
		DirectPlayPlayersTime = 2;
	}
	else
	{
		DirectPlayPlayersTime = 9;
	}

// hr�� byl zru�en ze hry
	if ((DirectPlayPlayersAktN < 0) && DirectPlayPlayersAktCreate)
	{
		DirectPlay->DestroyPlayer(DirectPlayPlayersAktID);
		DirectPlayPlayersAktCreate = false;
		DirectPlayDataOutN = 2;
		DirectPlayDataInO = 0;
		DirectPlayDataInN = 0;
	}

// korekce identifika�n�ho ��sla hr��e
	if (DirectPlayPlayersIDErr)
	{

// p��prava nov�ho identifika�n�ho k�du hr��e
		long id = 0;
		for (; id < DirectPlayPlayersSize; id++)
		{
			if (DirectPlayPlayersNames[id].IsEmpty()) break;
		}

// zm�na identifika�n�ho k�du hr��e
		DirectPlay->SetPlayerData(DirectPlayPlayersAktID, &id, sizeof(long), DPSET_GUARANTEED);

// nutn� aktualizace
		DirectPlayPlayersTime = 0;
	}

	return true;
}


// ------------------------- obsluha seznamu her ----------------------------


/////////////////////////////////////////////////////////////////////////////
// na�ten� parametr� hry

void GetGameDesc()
{
// test, zda je otev�ena hra
	if (DirectPlayGamesAkt.IsEmpty()) return;

// p��prava bufferu popisova�e hry
	DWORD n = sizeof(DPSESSIONDESC2) + 256;
	DPSESSIONDESC2* dsc = (DPSESSIONDESC2*)MemGet(n);
	MemFill(dsc, n, 0);
	dsc->dwSize = sizeof(DPSESSIONDESC2);

// na�ten� informac� o h�e
	if (DirectPlay->GetSessionDesc(dsc, &n) == DP_OK)
	{
		DirectPlayGamesParam[0] = dsc->dwUser1;
		DirectPlayGamesParam[1] = dsc->dwUser2;
		DirectPlayGamesParam[2] = dsc->dwUser3;
		DirectPlayGamesParam[3] = dsc->dwUser4;
		DirectPlayPlayersMax = dsc->dwMaxPlayers;
	}

// uvoln�n� bufferu
	MemFree(dsc);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� nov�ch parametr�

void SetGameDesc(DPSESSIONDESC2* dsc)
{
// mus� b�t hostitel
	if (!DirectPlayGamesHost) return;

// korekce maxim�ln�ho po�tu hr���
	if (((int)dsc->dwMaxPlayers != 0) && ((int)dsc->dwMaxPlayers < DirectPlayPlayersNum))
	{
		dsc->dwMaxPlayers = DirectPlayPlayersNum;
	}

// test, zda je zm�na dat
	if (
		(DirectPlayGamesParam[0] != dsc->dwUser1) ||
		(DirectPlayGamesParam[1] != dsc->dwUser2) ||
		(DirectPlayGamesParam[2] != dsc->dwUser3) ||
		(DirectPlayGamesParam[3] != dsc->dwUser4) ||
		(DirectPlayPlayersMax != (int)dsc->dwMaxPlayers)
		)
	{

// p��prava z�hlav� popisova�e
		dsc->dwSize = sizeof(DPSESSIONDESC2);
		dsc->dwFlags = DPSESSION_MIGRATEHOST;
		dsc->lpszPasswordA = NULL;

// jm�no hry
		dsc->lpszSessionNameA = (LPTSTR)(LPCTSTR)DirectPlayGamesAkt;

// nastaven� parametr� hry
		if (DirectPlay->SetSessionDesc(dsc, 0) == DP_OK)
		{

// zplatn�n� zm�n�n�ch parametr�
			DirectPlayGamesParam[0] = dsc->dwUser1;
			DirectPlayGamesParam[1] = dsc->dwUser2;
			DirectPlayGamesParam[2] = dsc->dwUser3;
			DirectPlayGamesParam[3] = dsc->dwUser4;
			DirectPlayPlayersMax = dsc->dwMaxPlayers;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� max. po�tu hr��� (0 = neomezeno)

void SetPlayerMax(int n)
{
// test, zda je otev�ena hra
	if (DirectPlayGamesAkt.IsEmpty()) return;

// omezen� ��seln�ho �daje
	if ((n < 0) || (n > 0xfffff)) n = 0;

// popisova� hry
	DPSESSIONDESC2 dsc;

// maxim�ln� po�et hr���
	dsc.dwMaxPlayers = n;

// parametry
	dsc.dwUser1 = DirectPlayGamesParam[0];
	dsc.dwUser2 = DirectPlayGamesParam[1];
	dsc.dwUser3 = DirectPlayGamesParam[2];
	dsc.dwUser4 = DirectPlayGamesParam[3];

// nastaven� nov�ch parametr�
	SetGameDesc(&dsc);
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� ��seln� hodnoty hry (n = rozsah -16384 a� +16383, i = index 0 a� 7)

void SetGameN(int n, int i)
{
// test, zda je otev�ena hra
	if (DirectPlayGamesAkt.IsEmpty()) return;

// omezen� ��seln�ho �daje
	if (n < -0x4000) n = -0x4000;
	if (n > 0x3fff) n = 0x3fff;

// popisova� hry
	DPSESSIONDESC2 dsc;

// maxim�ln� po�et hr���
	dsc.dwMaxPlayers = DirectPlayPlayersMax;

// parametry
	dsc.dwUser1 = DirectPlayGamesParam[0];
	dsc.dwUser2 = DirectPlayGamesParam[1];
	dsc.dwUser3 = DirectPlayGamesParam[2];
	dsc.dwUser4 = DirectPlayGamesParam[3];

// nastaven� po�adovan�ho parametru
	WORD* dat = (WORD*)&(dsc.dwUser1) + i;
	*dat = (WORD)((*dat & 0x8000) | (n + 0x4000));

// nastaven� nov�ch parametr�
	SetGameDesc(&dsc);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� p�ep�na�e hry (i = index 0 a� 7)

void SetGameL(bool n, int i)
{
// test, zda je otev�ena hra
	if (DirectPlayGamesAkt.IsEmpty()) return;

// popisova� hry
	DPSESSIONDESC2 dsc;

// maxim�ln� po�et hr���
	dsc.dwMaxPlayers = DirectPlayPlayersMax;

// parametry
	dsc.dwUser1 = DirectPlayGamesParam[0];
	dsc.dwUser2 = DirectPlayGamesParam[1];
	dsc.dwUser3 = DirectPlayGamesParam[2];
	dsc.dwUser4 = DirectPlayGamesParam[3];

// nastaven� po�adovan�ho parametru
	WORD* dat = (WORD*)&(dsc.dwUser1) + i;
	*dat = (WORD)(*dat & 0x7fff);
	if (n) *dat = (WORD)(*dat | 0x8000);

// nastaven� nov�ch parametr�
	SetGameDesc(&dsc);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� aktivn� hry (pr�zdn� = vypnut�)

void SetGameAct(CString txt)
{
// korekce textu
	txt.TrimLeft();
	txt.TrimRight();

// test, zda je zm�na aktivn� hry
	if (txt == DirectPlayGamesAkt) return;

// uzav�en� star� hry
	if (DirectPlayGamesAkt.IsNotEmpty())
	{
		SetPlayerAct(EmptyString);
		DirectPlay->Close();
		DirectPlayGamesAkt.Empty();
		DirectPlayGamesAktN = -1;
	}
	DirectPlayGamesHost = false;

// opu�t�n� hry
	if (txt.IsEmpty()) return;

// test, zda je otev�eno spojen�
	if (DirectPlayConnectsAktN < 0) return;

// zaji�t�n� na�ten� seznamu her            (zbyte�n� po�aduje IP i p�i vytv��en� hry)
/*
	if (DirectPlayGames.IsEmpty())
	{
		DirectPlayGamesTime = -1000;
		DirectPlayEnumGames();
		if (DirectPlayGamesTime > 3*18) return;
	}
*/

// nalezen� shodn�ho textu
	int i;
	for (i = 0; i < DirectPlayGamesNum; i++)
	{
		if (DirectPlayGames.GetLine(i) == txt) break;
	}

// popisova� hry
	DPSESSIONDESC2 dpDesc;
	MemFill(&dpDesc, sizeof(DPSESSIONDESC2), 0);
	dpDesc.dwSize = sizeof(DPSESSIONDESC2);
	dpDesc.guidApplication = *DirectPlayGuid;

// vytvo�en� nov� hry
	if (i == DirectPlayGamesNum)
	{
		dpDesc.dwFlags = DPSESSION_MIGRATEHOST;
		dpDesc.lpszSessionNameA = (LPTSTR)(LPCTSTR)txt;
		dpDesc.dwMaxPlayers = DirectPlayPlayersMax;
		dpDesc.dwUser1 = DirectPlayGamesParam[0];
		dpDesc.dwUser2 = DirectPlayGamesParam[1];
		dpDesc.dwUser3 = DirectPlayGamesParam[2];
		dpDesc.dwUser4 = DirectPlayGamesParam[3];

		if (DirectPlay->Open(&dpDesc, DPOPEN_CREATE) == DP_OK)
		{
			DirectPlayGamesHost = true;
			DirectPlayGamesAkt = txt;
			DirectPlayGamesAktGuid = dpDesc.guidInstance;
			DirectPlayGamesTime = 0;
		}
	}

// p�ipojen� k existuj�c� h�e
	else
	{
		dpDesc.guidInstance = DirectPlayGamesGuid[i];

		if (DirectPlay->Open(&dpDesc, DPOPEN_JOIN) == DP_OK)
		{
			DirectPlayGamesAkt = txt;
			DirectPlayGamesAktGuid = DirectPlayGamesGuid[i];
			DirectPlayGamesTime = 0;
			GetGameDesc();
		}
	}

// aktualizace seznamu her (p�i �sp�chu) - tak� pro zastaven� vyhled�v�n� her
	DirectPlayEnumGames();
}
 

/////////////////////////////////////////////////////////////////////////////
// callback pro enumeraci her

BOOL FAR PASCAL EnumSessionsCallback(
  LPCDPSESSIONDESC2 lpThisSD,
  LPDWORD lpdwTimeOut,
  DWORD dwFlags,
  LPVOID lpContext
  )
{
// p�i chyb� ji� d�le neopakovat
	if ((lpThisSD == NULL) || (dwFlags & DPESC_TIMEDOUT)) return FALSE;

// p��prava textu jm�na hry
	CString txt(lpThisSD->lpszSessionNameA);
	txt.TrimLeft();
	txt.TrimRight();

// zaji�t�n� jedine�nosti jm�na
	int n = 2;

	if (txt.IsEmpty())
	{
		txt.SetNumObj(1);
	}

	for (int ii = 0; ii < DirectPlayGamesNum; ii++)
	{
		if (txt == DirectPlayGames.GetLine(ii))
		{
			txt.SetNumObj(n);
			n++;
			ii = -1;
		}
	}

// p�id�n� textu k seznamu
	DirectPlayGames += txt;
	DirectPlayGames += _T(13);
	DirectPlayGames += _T(10);

// zv��en� ��ta�e her
	int i = DirectPlayGamesNum;
	DirectPlayGamesNum++;
	int i2 = DirectPlayGamesNum;

// �schova GUID hry
	DirectPlayGamesGuid = (GUID*)MemSize(DirectPlayGamesGuid, i2 * sizeof(GUID));
	DirectPlayGamesGuid[i] = lpThisSD->guidInstance;

// test, zda to je aktivn� hra
	if (DirectPlayGamesAkt == txt)
	{
		DirectPlayGamesAktN = i;
		DirectPlayGamesAktGuid = lpThisSD->guidInstance;
		DirectPlayGamesParam[0] = lpThisSD->dwUser1;
		DirectPlayGamesParam[1] = lpThisSD->dwUser2;
		DirectPlayGamesParam[2] = lpThisSD->dwUser3;
		DirectPlayGamesParam[3] = lpThisSD->dwUser4;
		DirectPlayPlayersMax = lpThisSD->dwMaxPlayers;
	}

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// seznam her (FALSE=chyba), p�i otev�en� h�e se zastav� vyhled�v�n� her

bool DirectPlayEnumGames()
{
// test, zda je seznam je�t� platn�
	if (DirectPlayGamesTime > 0) return true;

// test, zda je vybr�no spojen�
	if (DirectPlayConnectsAktN < 0) return false;

// p��prava popisova�e
    DPSESSIONDESC2 dpDesc;
	MemFill(&dpDesc, sizeof(DPSESSIONDESC2), 0);
	dpDesc.dwSize = sizeof(DPSESSIONDESC2);
	dpDesc.guidApplication = *DirectPlayGuid;

// zji�t�n� her (p�i otev�en� h�e se bere pouze z cache)
	DirectPlayGames.Empty();
	DirectPlayGamesNum = 0;
	DirectPlayGamesAktN = -1;
	HRESULT hres = DirectPlay->EnumSessions(&dpDesc, 0, EnumSessionsCallback, NULL,
//		((DirectPlayGamesTime == -1000) ? 0 : (DirectPlayGamesAkt.IsEmpty() ? DPENUMSESSIONS_ASYNC : DPENUMSESSIONS_STOPASYNC))
		(DirectPlayGamesAkt.IsEmpty() ? DPENUMSESSIONS_ASYNC : DPENUMSESSIONS_STOPASYNC)
		| DPENUMSESSIONS_ALL);

// nov� ��ta� platnosti seznamu
	if (hres == DPERR_USERCANCEL)
	{
// tato hodnota se pou�ije t� k rozli�en� p�eru�en� z funkce pro vytvo�en� hry
		DirectPlayGamesTime = 5*18;			// p�eru�eno u�ivatelem
	}
	else
	{
		if (DirectPlayGamesNum == 0)
		{
			DirectPlayGamesTime = 2;		// ne�sp�ch - hned op�t nov� pokus
		}
		else
		{
			DirectPlayGamesTime = 9;		// p�i �sp�chu aktualizace op�t za chv�li
		}
	}

// dopln�n� aktivn� hry, pokud nebyla nalezena
	if (DirectPlayGamesAkt.IsNotEmpty() && (DirectPlayGamesAktN < 0))
	{
		DirectPlayGames += DirectPlayGamesAkt;
		DirectPlayGames += _T(13);
		DirectPlayGames += _T(10);
		DirectPlayGamesAktN = DirectPlayGamesNum;
		DirectPlayGamesNum++;
		DirectPlayGamesGuid = (GUID*)MemSize(DirectPlayGamesGuid, DirectPlayGamesNum * sizeof(GUID));
		DirectPlayGamesGuid[DirectPlayGamesAktN] = DirectPlayGamesAktGuid;
	}
	return true;
}


// ----------------------- obsluha seznamu spojen� --------------------------

/////////////////////////////////////////////////////////////////////////////
// vypnut� aktivn�ho spojen�

void SetConnectActNul()
{
// test, zda je otev�eno n�jak� spojen�
	if (DirectPlayConnectsAktN >= 0)
	{
// zru�en� p��znaku otev�en� spojen�
		DirectPlayConnectsAktN = -1;
		DirectPlayConnectsAkt.Empty();

// uvoln�n� objektu DirectPlay
		DirectPlayUninit();
	}
}


/////////////////////////////////////////////////////////////////////////////
// vyhled�n� pod�et�zce v seznamu spojen� (v�e konvertov�no na velk� p�smena)
// i ...... index nalezen�ho spojen�
// txt .... zadan� jm�no spojen�
// sub .... hledan� pod�et�zec

void GetConnectSubString(int& i, CString& txt, CString sub)
{
// test, zda spojen� zat�m nebylo nalezeno; test, zda zadan� text obsahuje pod�et�zec
	if ((i == DirectPlayConnectsNum) && (txt.Find(sub) >= 0))
	{
		CString txt2;

// cyklus p�es v�echna spojen�
		for (i = 0; i < DirectPlayConnectsNum; i++)
		{

// na�ten� textu jednoho spojen�
			txt2 = DirectPlayConnects.GetLine(i);

// konverze na velk� p�smena
			txt2.UpperCase();

// test, zda obsahuje pod�et�zec
			if (txt2.Find(sub) >= 0) break;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� aktivn�ho spojen� (pr�zdn� text = zru�en�)

void SetConnectAct(CString txt)
{
// korekce textu
	txt.TrimLeft();
	txt.TrimRight();

// pr�zdn� text - zru�en� spojen�
	if (txt.IsEmpty())
	{
		SetConnectActNul();
		return;
	}

// inicializace seznamu spojen�
	if (!DirectPlayConInit()) return;

// nalezen� shodn�ho textu
	int i;
	for (i = 0; i < DirectPlayConnectsNum; i++)
	{
		if (DirectPlayConnects.GetLine(i) == txt) break;
	}

// hled�n� bez rozli�en� velk�/mal� p�smena
	if (i == DirectPlayConnectsNum)
	{
		txt.UpperCase();
		CString txt2;

		for (i = 0; i < DirectPlayConnectsNum; i++)
		{
			txt2 = DirectPlayConnects.GetLine(i);
			txt2.UpperCase();
			if (txt2 == txt) break;
		}

// hled�n� pod�et�zc�
		GetConnectSubString(i, txt, _T("IPX"));
		GetConnectSubString(i, txt, _T("TCP"));
		GetConnectSubString(i, txt, _T("MODEM"));

// je�t� pokus o hled�n� textu "serial"/"s�riov"/"seriov"
		if (i == DirectPlayConnectsNum)
		{
			CString s1("SERIAL");
			CString s2("S�RIOV");
			CString s3("SERIOV");

			if ((txt.Find(s1) >= 0) || (txt.Find(s2) >= 0) || (txt.Find(s3) >= 0))
			{
				CString txt2;

				for (i = 0; i < DirectPlayConnectsNum; i++)
				{
					txt2 = DirectPlayConnects.GetLine(i);
					txt2.UpperCase();
					if ((txt2.Find(s1) >= 0) || (txt2.Find(s2) >= 0) || (txt2.Find(s3) >= 0)) break;
				}
			}
		}
	}

// zad�n neplatn� �et�zec - vypnut� spojen�
	if (i == DirectPlayConnectsNum)
	{
		SetConnectActNul();
		return;
	}

// inicializace spojen� (jen je-li zm�n�no)
	if (i != DirectPlayConnectsAktN)
	{

// nulov�n� ��ta�� pro aktualizaci informac�
		DirectPlayGamesTime = 0;
		DirectPlayPlayersTime = 0;

// vypnut� star�ho spojen�
		SetConnectActNul();

// inicializace seznamu spojen� (a kontrola platnosti indexu)
		if (!DirectPlayConInit() || (i >= DirectPlayConnectsNum))
		{
			SetConnectActNul();
			return;
		}

// inicializace spojen�
		HRESULT hres = DirectPlay->InitializeConnection(DirectPlayConnectsConn[i], 0);

// kontrola �sp�nosti spojen�
		if ((hres != DP_OK) && (hres != DPERR_ALREADYINITIALIZED)) return;
	}

// �schova aktivn�ho spojen�
	DirectPlayConnectsAkt = DirectPlayConnects.GetLine(i);
	DirectPlayConnectsAktN = i;
}


/////////////////////////////////////////////////////////////////////////////
// callback pro enumeraci spojen�

BOOL FAR PASCAL EnumConnectionsCallback(
  LPCGUID lpguidSP,
  LPVOID lpConnection,
  DWORD dwConnectionSize,
  LPCDPNAME lpName,
  DWORD dwFlags,
  LPVOID lpContext
  )
{
// p��prava textu jm�na spojen�
	CString txt(lpName->lpszShortNameA);
	if (txt.IsEmpty()) txt = lpName->lpszLongNameA;
	txt.TrimLeft();
	txt.TrimRight();

// zru�en� textu "pro DirectPlay"
	if (txt.Right(ForDPlay1.Length()) == ForDPlay1)
	{
		txt.Delete(txt.Length() - ForDPlay1.Length());
	}
	else
	{
		if (txt.Right(ForDPlay2.Length()) == ForDPlay2)
		{
			txt.Delete(txt.Length() - ForDPlay2.Length());
		}
		else
		{
			if (txt.Right(ForDPlay3.Length()) == ForDPlay3)
			{
				txt.Delete(txt.Length() - ForDPlay3.Length());
			}
		}
	}
	txt.TrimRight();

// zaji�t�n� jedine�nosti jm�na
	int n = 2;

	if (txt.IsEmpty())
	{
		txt.SetNumObj(1);
	}

	for (int ii = 0; ii < DirectPlayConnectsNum; ii++)
	{
		if (txt == DirectPlayConnects.GetLine(ii))
		{
			txt.SetNumObj(n);
			n++;
			ii = -1;
		}
	}

// p�id�n� textu k seznamu
	DirectPlayConnects += txt;

// p�id�n� konce ��dku
	DirectPlayConnects += _T(13);
	DirectPlayConnects += _T(10);

// zv��en� ��ta�e spojen�
	int i = DirectPlayConnectsNum;
	DirectPlayConnectsNum++;
	int i2 = DirectPlayConnectsNum;

// �schova dat spojen�
	DirectPlayConnectsConn = (void**)MemSize(DirectPlayConnectsConn, i2 * sizeof(void*));
	DirectPlayConnectsConn[i] = (void*)MemGet(dwConnectionSize);
	MemCopy(DirectPlayConnectsConn[i], lpConnection, dwConnectionSize);

// pokra�ovat v enumeraci
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// inicializace seznamu spojen� (FALSE=chyba)

bool DirectPlayConInit()
{
// test,zda je seznam ji� inicializov�n
	if (DirectPlayConnectsNum > 0) return true;

// test, zda je mo�n� nov� pokus o otev�en�
	if (NextDirectPlayInit > 0) return false;
	NextDirectPlayInit = 36;

// inicializace objektu DirectPlay
	if (!DirectPlayInit()) return false;

// zji�t�n� seznamu mo�n�ch spojen�
	if ((DirectPlay->EnumConnections(DirectPlayGuid, EnumConnectionsCallback, NULL, 0) != DP_OK) ||
		(DirectPlayConnectsNum == 0))
	{
		DirectPlayConnects.Empty();
		DirectPlayConnectsNum = 0;
		return false;
	}

// operace OK
	NextDirectPlayInit = 0;
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// uvoln�n� seznamu spojen� (ukon�� prob�haj�c� hru)

void DirectPlayConUninit()
{
// test, zda jsou v seznamu n�jak� spojen�
	if (DirectPlayConnectsNum > 0)
	{

// vypnut� aktivn� hry
		SetGameAct(EmptyString);

// zru�en� seznamu her
		DirectPlayGames.Empty();
		DirectPlayGamesNum = 0;

// uvoln�n� informac� o spojen�ch
		for (int i = 0; i < DirectPlayConnectsNum; i++)
		{
			MemFree(DirectPlayConnectsConn[i]);
		}

// zru�en� seznamu spojen�
		DirectPlayConnects.Empty();
		DirectPlayConnectsNum = 0;

// zru�en� aktivn�ho spojen�
		DirectPlayConnectsAkt.Empty();
		DirectPlayConnectsAktN = -1;
	}
}


// --------------------- obsluha objektu DirectPlay -------------------------

/////////////////////////////////////////////////////////////////////////////
// inicializace objektu DirectPlay (FALSE=chyba)

bool DirectPlayInit()
{
// test, zda je objekt DirectPlay ji� vytvo�en
	if (DirectPlay != NULL) return true;

// inicializace COM knihovny
	if (!COMInit()) return false;

// vytvo�en� rozhrann� DirectPlay3A
	if ((::CoCreateInstance(CLSID_DirectPlay, NULL, CLSCTX_INPROC_SERVER,
			IID_IDirectPlay3A, (void**)&DirectPlay) != DP_OK) || (DirectPlay == NULL))
	{
		DirectPlay = NULL;
		return false;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// uvoln�n� objektu DirectPlay (nutn� p�i zm�n� spojen� nebo hry)

void DirectPlayUninit()
{
// test, zda je objekt DirectPlay vytvo�en
	if (DirectPlay != NULL)
	{

// uvoln�n� seznamu spojen�
		DirectPlayConUninit();

// uvoln�n� objektu DirectPlay
		DirectPlay->Release();
		DirectPlay = NULL;
	}
}


// ------------------------ obsluha knihovny COM ----------------------------

/////////////////////////////////////////////////////////////////////////////
// inicializace knihovny COM (FALSE=chyba) (z�st�v� inicializov�na po celou dobu b�hu programu)

bool COMInit()
{
// test, zda je knihovna COM ji� inicializov�na
	if (Coinit) return true;

// inicializace knihovny COM
	HRESULT res = ::CoInitialize(NULL);

// kontrola operace
	if ((res != S_OK) && (res != S_FALSE)) return false;

// zapnut� p��znaku inicializace knihovny
	Coinit = true;
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// uvoln�n� knihovny COM (p�i ukon�en� programu)

void COMUninit()
{
// test, zda je knihovna COM inicializov�na
	if (Coinit)
	{

// uvoln�n� p��padn�ho objektu DirectPlay
		DirectPlayUninit();

// uvoln�n� knihovny COM
		::CoUninitialize();

// vypnut� p��znaku inicializace knihovny
		Coinit = false;
	}
}

/***************************************************************************\
*																			*
*										UDP									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// inicializace WSA knihovny

void WSAInitialize()
{
	// test, zda inicializace ji� prob�hla
	if (!Wsaload)
	{
		// p��znak pokusu o na�ten� knihovny
		Wsaload = true;

		// na�ten� knihovny WSA (verze 1 nebo 2)
		WSALibrary = ::LoadLibrary(_T("WS2_32.DLL"));	// verze 2

		if (WSALibrary == NULL)
		{
			WSALibrary = ::LoadLibrary(_T("WSOCK32.DLL"));		// verze 1
				 
		}

		// import funkc� knihovny WSA 1.1 pro DirectPlay
		if (WSALibrary != NULL)
		{
			pWSAStartup = (WSASTARTUP) ::GetProcAddress(WSALibrary, "WSAStartup");
			pWSACleanup = (WSACLEANUP) ::GetProcAddress(WSALibrary, "WSACleanup");
			pWSAGetHostName = (WSAGETHOSTNAME) ::GetProcAddress(WSALibrary, "gethostname");
			pWSAGetHostByName = (WSAGETHOSTBYNAME) ::GetProcAddress(WSALibrary, "gethostbyname");
			pWSAInetNtoa = (WSAINET_NTOA) ::GetProcAddress(WSALibrary, "inet_ntoa");

			if ((pWSAStartup != NULL) &&
				(pWSACleanup != NULL) &&
				(pWSAGetHostName != NULL) &&
				(pWSAGetHostByName != NULL) &&
				(pWSAInetNtoa != NULL))
			{
				// inicializace knihovny WSA 2.2
				if (pWSAStartup(MAKEWORD(2, 2), &Wsadata) == 0)
				{
					Wsainit = true;

					// import funkc� knihovny WSA 2.2 pro UDP
					pWSASocket = (WSASOCKET) ::GetProcAddress(WSALibrary, "WSASocketA");
					pWSAhtons = (WSAHTONS) ::GetProcAddress(WSALibrary, "htons");
					pWSACreateEvent = (WSACREATEEVENT) ::GetProcAddress(WSALibrary, "WSACreateEvent");
					pWSACloseEvent = (WSACLOSEEVENT) ::GetProcAddress(WSALibrary, "WSACloseEvent");
					pWSASetSockOpt = (WSASETSOCKOPT) ::GetProcAddress(WSALibrary, "setsockopt");
					pWSABind = (WSABIND) ::GetProcAddress(WSALibrary, "bind");
					pWSACloseSocket = (WSACLOSESOCKET) ::GetProcAddress(WSALibrary, "closesocket");
					pWSASendTo = (WSASENDTO) ::GetProcAddress(WSALibrary, "WSASendTo");
					pWSARecvFrom = (WSARECVFROM) ::GetProcAddress(WSALibrary, "WSARecvFrom");
					pWSAGetOverlappedResult = (WSAGETOVERLAPPEDRESULT) ::GetProcAddress(WSALibrary, "WSAGetOverlappedResult");
					pWSAGetLastError = (WSAGETLASTERROR) ::GetProcAddress(WSALibrary, "WSAGetLastError");
					pWSASetEvent = (WSASETEVENT) ::GetProcAddress(WSALibrary, "WSASetEvent");
					pWSAInetAddr = (WSAINETADDR) ::GetProcAddress(WSALibrary, "inet_addr");

					if ((pWSASocket != NULL) &&
						(pWSAhtons != NULL) &&
						(pWSACreateEvent != NULL) &&
						(pWSACloseEvent != NULL) &&
						(pWSASetSockOpt != NULL) &&
						(pWSABind != NULL) &&
						(pWSACloseSocket != NULL) &&
						(pWSASendTo != NULL) &&
						(pWSARecvFrom != NULL) &&
						(pWSAGetOverlappedResult != NULL) &&
						(pWSAGetLastError != NULL) &&
						(pWSASetEvent != NULL) &&
						(pWSAInetAddr != NULL))
					{
						Wsainit2 = true;
					}

				}
				else
				{
					// inicializace knihovny WSA 1.1
					if (pWSAStartup(MAKEWORD(1, 1), &Wsadata) == 0)
					{
						Wsainit = true;
					}
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// inicializace WSA 1.1 knihovny (vrac� TRUE je-li OK)

bool WSAInitialize1()
{
	WSAInitialize();
	return Wsainit;
}

/////////////////////////////////////////////////////////////////////////////
// inicializace WSA 2.2 knihovny (vrac� TRUE je-li OK)

bool WSAInitialize2()
{
	WSAInitialize();
	return Wsainit2;
}

/////////////////////////////////////////////////////////////////////////////
// inicializace obsluhy UDP (inicializace buffer�)

void InitUDP()
{
	// vys�lac� adresa UDP (INADDR_BROADCAST = (u_long)0xffffffff)
	MemFill(&UDPSendAddr, sizeof(SOCKADDR_IN), 0);
	UDPSendAddr.sin_family = AF_INET;
	UDPSendAddr.sin_port = 0x8813;			// port 5000
	UDPSendAddr.sin_addr.s_addr = 0x0100007f;	// loop IP adresa INADDR_LOOPBACK "127.0.0.1"

	// overlapped struktura vys�la�e
	UDPSendOver.hEvent = NULL;

	// odes�lac� buffer UDP
	UDPSendBuf.buf = (char*)MemGet(16384);	// alokace bufferu

	// p�ij�mac� adresa UDP
	MemFill(&UDPRecAddr, sizeof(SOCKADDR_IN), 0);
	UDPRecAddr.sin_family = AF_INET;
	UDPRecAddr.sin_port = 0x8813;			// port 5000
	UDPRecAddr.sin_addr.s_addr = 0;			// IP adresa INADDR_ANY = "0.0.0.0"

	// p�ijat� adresa UDP
	MemFill(&UDPRecRecAddr, sizeof(SOCKADDR_IN), 0);
	UDPRecRecAddr.sin_family = AF_INET;
	UDPRecRecAddr.sin_port = 0;
	UDPRecRecAddr.sin_addr.s_addr = 0xffffffff; // IP adresa INADDR_NONE

	// overlapped struktura p�ij�ma�e
	UDPRecOver.hEvent = NULL;

	// p�ij�mac� buffer UDP
	UDPRecBuf.len = 512;					// velikost paketu
	UDPRecBuf.buf = (char*)MemGet(16384);	// alokace bufferu
}

/////////////////////////////////////////////////////////////////////////////
// otev�en� vys�lac�ho soketu UDP (p�i chyb� vrac� FALSE)

bool OpenSendUDP()
{
	// test, zda je soket otev�en�
	if (UDPSendSocket == INVALID_SOCKET)
	{
		// inicializace WSA
		if (WSAInitialize2())
		{
			// vytvo�en� ud�losti overlapped vys�la�e
			UDPSendOver.hEvent = pWSACreateEvent();
			if (UDPSendOver.hEvent != NULL)
			{
				// otev�en� vys�lac�ho UDP soketu
				UDPSendSocket = pWSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
				if (UDPSendSocket != INVALID_SOCKET)
				{
					// nastaven� timeout vys�la�e
					int par = 50;
					pWSASetSockOpt(UDPSendSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&par, sizeof(par));

					// sd�len� pou�it� adres
					par = TRUE;
					pWSASetSockOpt(UDPSendSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&par, sizeof(par));

					// povolen broadcast p�enos
					par = TRUE;
					pWSASetSockOpt(UDPSendSocket, SOL_SOCKET, SO_BROADCAST, (char*)&par, sizeof(par));
				}
			}
		}
	}
	return (UDPSendSocket != INVALID_SOCKET);
}

/////////////////////////////////////////////////////////////////////////////
// zav�en� vys�lac�ho soketu UDP

void CloseSendUDP()
{
	// zav�en� vys�lac�ho UDP soketu
	if (UDPSendSocket != INVALID_SOCKET)
	{
		pWSASetEvent(UDPSendOver.hEvent);
		pWSACloseSocket(UDPSendSocket);
		UDPSendSocket = INVALID_SOCKET;
	}

	// zru�en� ud�losti overlapped vys�la�e
	if (UDPSendOver.hEvent != NULL)
	{
		pWSACloseEvent(UDPSendOver.hEvent);
		UDPSendOver.hEvent = NULL;
	}

	UDPSending = false;
}


/////////////////////////////////////////////////////////////////////////////
// otev�en� p�ij�mac�ho soketu UDP (p�i chyb� vrac� FALSE)

bool OpenRecUDP()
{
	// test, zda je soket otev�en�
	if (UDPRecSocket == INVALID_SOCKET)
	{
		// inicializace WSA
		if (WSAInitialize2())
		{
			// vytvo�en� ud�losti overlapped p�ij�ma�e
			UDPRecOver.hEvent = pWSACreateEvent();
			if (UDPRecOver.hEvent != NULL)
			{
				// otev�en� p�ij�mac�ho UDP soketu
				UDPRecSocket = pWSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
				if (UDPRecSocket != INVALID_SOCKET)
				{
					// nastaven� time-out p�ij�ma�e
					int par = 1000;
					pWSASetSockOpt(UDPRecSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&par, sizeof(par));

					// velikost p�ij�mac�ho bufferu
					par = UDPRecBuf.len;
					pWSASetSockOpt(UDPRecSocket, SOL_SOCKET, SO_RCVBUF, (char*)&par, sizeof(par));

					// sd�len� pou�it� adres
					par = TRUE;
					pWSASetSockOpt(UDPRecSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&par, sizeof(par));

					// povolen broadcast p�enos
					par = TRUE;
					pWSASetSockOpt(UDPRecSocket, SOL_SOCKET, SO_BROADCAST, (char*)&par, sizeof(par));

					// pojmenov�n� soketu p�ij�ma�e
					pWSABind(UDPRecSocket, (sockaddr*)&UDPRecAddr, sizeof(SOCKADDR_IN));

					// zah�jen� p��jmu
					StartRecUDP();
				}
			}
		}
	}
	return (UDPRecSocket != INVALID_SOCKET);
}

/////////////////////////////////////////////////////////////////////////////
// zav�en� p�ij�mac�ho soketu UDP

void CloseRecUDP()
{
	// zav�en� p�ij�mac�ho UDP soketu
	if (UDPRecSocket != INVALID_SOCKET)
	{
		pWSASetEvent(UDPRecOver.hEvent);
		pWSACloseSocket(UDPRecSocket);
		UDPRecSocket = INVALID_SOCKET;
	}

	// zru�en� ud�losti overlapped p�ij�ma�e
	if (UDPRecOver.hEvent != NULL)
	{
		pWSACloseEvent(UDPRecOver.hEvent);
		UDPRecOver.hEvent = NULL;

	}
	UDPReceiving = false;
	UDPRecData = 0;
}

/////////////////////////////////////////////////////////////////////////////
// odesl�n� UDP paketu

void SendUDP(const void* data, int len)
{
	if (WSAInitialize2())
	{
		if (OpenSendUDP())
		{
			int res;
			DWORD send;
			DWORD flags;

			// test, zda minul� vys�l�n� bylo ukon�eno
			if (UDPSending)
			{
				res = pWSAGetOverlappedResult(UDPSendSocket, &UDPSendOver, &send, FALSE, &flags);
				if (!res)
				{
					if (pWSAGetLastError() == WSA_IO_INCOMPLETE) return;
					pWSASetEvent(UDPSendOver.hEvent);
				}
				UDPSending = false;
			}

			// kopie dat do vys�lac�ho bufferu
			if (len > 16384) len = 16384;
			if (len <= 0) return;
			UDPSendBuf.len = len;
			MemCopy(UDPSendBuf.buf, data, len);

			// odesl�n� paketu
			res = pWSASendTo(UDPSendSocket, &UDPSendBuf, 1, &send, 0, (SOCKADDR*)&UDPSendAddr,
									sizeof(SOCKADDR_IN), &UDPSendOver, NULL);

			// paket je odes�l�n na pozad�
			if (res == SOCKET_ERROR)
			{
				if (pWSAGetLastError() == WSA_IO_PENDING) UDPSending = true;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// start p��jmu UDP paketu

void StartRecUDP()
{
	if (UDPRecData == 0)
	{
		if (WSAInitialize2())
		{
			if (OpenRecUDP())
			{
				int res;
				DWORD rec = 0;
				DWORD flags = 0;

				// prob�h� p��jem
				if (UDPReceiving)
				{
					res = pWSAGetOverlappedResult(UDPRecSocket, &UDPRecOver, &rec, FALSE, &flags);
					if (!res)
					{
						if (pWSAGetLastError() == WSA_IO_INCOMPLETE) return;
						pWSASetEvent(UDPRecOver.hEvent);
						UDPReceiving = false;
					}
					else
					{
						UDPReceiving = false;
						UDPRecData = rec;
						return;
					}
				}

				// zah�jen� nov�ho p��jmu
				rec = 0;
				flags = 0;
				int fromlen = sizeof(SOCKADDR_IN);
				res = pWSARecvFrom(UDPRecSocket, &UDPRecBuf, 1, &rec, &flags, (SOCKADDR*)&UDPRecRecAddr,
					&fromlen, &UDPRecOver, NULL);

				// paket je odes�l�n na pozad�
				if (res != 0)
				{
					if (pWSAGetLastError() == WSA_IO_PENDING)
						UDPReceiving = true;
					else
						pWSASetEvent(UDPRecOver.hEvent);
				}
				else
					UDPRecData = rec;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// p��jem UDP paketu (vrac� velikost dat v p�ij�mac�m bufferu)

int RecUDP()
{
	StartRecUDP();
	int n = UDPRecData;
	if (n < 0) n = 0;
	if (n > (int)UDPRecBuf.len) n = UDPRecBuf.len;
	UDPRecData = 0;
	return n;
}

/////////////////////////////////////////////////////////////////////////////
// deinicializace WSA knihovny

void WSADeinitialize()
{
	// ukon�en� UDP
	CloseSendUDP();
	CloseRecUDP();

	// uvoln�n� knihovny WSA
	if (Wsainit)
	{
		pWSACleanup();
		Wsainit = false;
		Wsainit2 = false;
	}

	if (WSALibrary != NULL)
	{
		::FreeLibrary(WSALibrary);
		WSALibrary = NULL;
		Wsaload = false;
	}
}

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------


/***************************************************************************\
*																			*
*									Dialogy									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// re�im dialogu

void DialogOn(bool on)
{
	if (on == DialogMode) return;
	DialogMode = on;

	::InvalidateRect(MainFrame, NULL, TRUE);
	::UpdateWindow(MainFrame);
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� nov�ho okna

void WindowNew(int typ, int styl)
{
// vypnut� celoobrazovkov�ho m�du
	WindowID = Win.New();
	WINITEM* item = &Win[WindowID];
	item->Parent = DialogParent;
	item->Typ = typ;
	item->FontCol = StdColorBtnText;
	item->FontBack = StdColorBtnFace;
	item->FontBrush = StdBrushBtn;
	item->HasBorder = true;
	item->HasCaption = true;
	item->HasSize = false;
	item->MayClose = true;
	item->AlwaysTop = false;

	WINITEM* parent = &Win[DialogParent];
	item->Left = parent->Left;
	item->Top = parent->Top;
	item->Width = parent->Width;
	item->Height = parent->Height;

	parent->Disable = true;

	if (item->Width >= 2*ICONWIDTH)
	{
		item->Left += ICONWIDTH/2;
		item->Width -= ICONWIDTH;
	}

	if (item->Height >= 2*ICONHEIGHT)
	{
		item->Top += ICONHEIGHT/2;
		item->Height -= ICONHEIGHT;
	}

	item->Wnd = ::CreateWindowEx(
		WS_EX_CONTROLPARENT | WS_EX_DLGMODALFRAME,		// roz���en� styl okna
		MainFrameClass,									// jm�no t��dy okna
		NULL,											// titulek okna
		WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_OVERLAPPED |
			WS_CLIPSIBLINGS	/*| WS_MINIMIZEBOX */| styl,	// styl okna
		item->Left,										// X
		item->Top,										// Y
		item->Width,									// ���ka
		item->Height,									// v��ka
		parent->Wnd,									// rodi�
		NULL,											// menu
		hInstance,										// instance
		NULL);											// parametry

	::SetWindowLong(item->Wnd, 0, WindowID);

	DialogParent = WindowID;
	WindowFocus = WindowID;

	ReqClose = false;

	InitClientRect(item);

	::SetFocus(item->Wnd);
	::EnableWindow(parent->Wnd, FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� fontu (nutn� uvoln�n� pomoc� FreeFont)

HFONT GetFont(bool bold, bool italic, bool underline, bool strikeout, bool serif, bool fixed, int height, double width, double angle)
{
// sada znak�
	DWORD charset = CharSet;
	if (charset == DefCharSet) charset = DEFAULT_CHARSET;

// korekce v��ky
	if (height < 0) height = -height;

// nalezen� existuj�c�ho fontu v tabulce
	int i;
	for (i = 0; i < FONTTABSIZE; i++)
	{
		FONTITEM* item = FontTab + i;
		if (item->Font == NULL) break;

		if ((item->Bold == bold) &&
			(item->Italic == italic) &&
			(item->Underline == underline) &&
			(item->Strikeout == strikeout) &&
			(item->Serif == serif) &&
			(item->Fixed == fixed) &&
			(item->Height == height) &&
			(item->Width == width) &&
			(item->Angle == angle) &&
			(item->CharSet == charset) &&
			(
				!fixed ||
				!serif ||
				(item->UserFont == UserFont)
			))
		{
			item->Used++;
			return item->Font;
		}
	}

// skute�n� v��ka znak�
	int height0 = height;
	if (height <= 0) height0 = 16;

// p��prava ���ky fontu
	CString name;
	double koef;

	if (fixed)
	{
		if (serif)
		{
			name = UserFont;
			koef = 0.6;
		}
		else
		{
			name = _T("Courier New");
			koef = 0.58;
		}
	}
	else
	{
		if (serif)
		{
			name = _T("Times New Roman");
			koef = 0.37;
		}
		else
		{
			name = _T("Arial");
			koef = 0.40;
		}
	}

	if (fixed && italic)
	{
		koef *= 0.9;
	}

	if (bold) 
	{
		if (fixed)
		{
			koef *= 0.9;
		}
		else
		{
			koef *= 1.06;
		}
	}

	int width0 = Round(fabs(width) * height0 * koef - 0.1);

// p�epo�et �hlu na desetiny stupn�
	int angle0 = Round(angle/pi*1800) % 3600;

// vytvo�en� fontu
	HFONT font = ::CreateFont(
		height0,
		width0,
		angle0,
		angle0,
		(bold) ? FW_BOLD : FW_NORMAL,
		italic,
		underline,
		strikeout,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, name);

// druh� pokus, nen�-li p��tomna spr�vn� znakov� sada
	if (font == NULL)
	{
		font = ::CreateFont(
			height0,
			width0,
			angle0,
			angle0,
			(bold) ? FW_BOLD : FW_NORMAL,
			italic,
			underline,
			strikeout,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, name);
	}

// p�i ne�sp�chu n�hradn� font
	if (font == NULL) font = FontDefault;
	if (font == NULL) return NULL;

// nalezen� voln� polo�ky k ulo�en� fontu
	FONTITEM* item;
	for (i = FONTTABSIZE-FONTTABSTD; i > 0; i--)
	{
		item = FontTab + FontNext;
		if (item->Used == 0) break;
		FontNext++;
		if (FontNext >= FONTTABSIZE) FontNext = FONTTABSTD;
	}
	
	item = FontTab + FontNext;
	if (item->Font != NULL) ::DeleteObject(item->Font);

// ulo�en� informac� o fontu
	item->Bold = bold;
	item->Italic = italic;
	item->Underline = underline;
	item->Strikeout = strikeout;
	item->Serif = serif;
	item->Fixed = fixed;
	item->Height = height;
	item->Width = width;
	item->Angle = angle;
	item->Font = font;
	item->CharSet = charset;
	item->UserFont = UserFont;
	item->Used = 1;

// ukazatel p��t� polo�ky
	FontNext++;
	if (FontNext >= FONTTABSIZE) FontNext = FONTTABSTD;

// n�vrat fontu
	return font;
}


/////////////////////////////////////////////////////////////////////////////
// uvoln�n� fontu p�id�len�ho s GetFont

void FreeFont(HFONT font)
{
	if (font == NULL) return;

	for (int i = 0; i < FONTTABSIZE; i++)
	{
		FONTITEM* item = FontTab + i;
		if (item->Font == NULL) break;

		if ((item->Font == font) && (item->Used > 0))
		{
			item->Used--;
			return;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace fontu prvku

void WindowSetFont(WINITEM* item)
{
	if ((item->Typ != WINITEM_WINICON) && (item->Typ != WINITEM_WINPIC))
	{
		HFONT oldfont = item->Font;
		item->Font =  GetFont(item->Bold, item->Italic, item->Underline, item->Strikeout,
					item->Serif, item->Fixed, item->FontHeight, item->FontWidth, item->Angle);
		::SendMessage(item->Wnd, WM_SETFONT, (WPARAM)item->Font, TRUE);
		FreeFont(oldfont);
	}
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� prvku okna

void WindowItemNew(int typ, CString& classname, int width, int height, int style, int exstyle)
{
// vytvo�en� nov�ho prvku
	WindowID = Win.New();
	WINITEM* item = &Win[WindowID];

// zapnut� dialogov�ho m�du hlavn�ho okna
	if (DialogParent == 0) DialogOn(true);

// adresa rodi�e
	WINITEM* parent = &Win[DialogParent];

// za�azen� do �et�zce prvk�
	item->Prev = parent->Prev;
	item->Next = DialogParent;
	parent->Prev = WindowID;
	Win[item->Prev].Next = WindowID;

// inicializace parametr�
	item->Parent = DialogParent;
	item->Typ = typ;
	item->Left = (parent->ClientWidth - width)/2;
	item->Top = (parent->ClientHeight - height)/2;
	item->Width = width;
	item->Height = height;
	item->TabStop = ((style & WS_TABSTOP) != 0);
	item->HasCaption = false;
	item->HasSize = false;
	item->MayClose = true;
	item->AlwaysTop = false;
	item->HasBorder = ((exstyle & WS_EX_CLIENTEDGE) != 0);

	switch (typ)
	{
	case WINITEM_TABLE:
		item->TabHead = ((style & LVS_NOCOLUMNHEADER) == 0);

	case WINITEM_EDIT:
	case WINITEM_MEMO:
	case WINITEM_LISTBOX:
	case WINITEM_LISTEDIT:
	case WINITEM_COMBOBOX:
	case WINITEM_COMBOEDIT:
	case WINITEM_RICHEDIT:
	case WINITEM_RICHMEMO:
		item->FontCol = StdColorWindowText;
		item->FontBack = StdColorWindow;
		item->FontBrush = StdBrushWindow;
		break;

	default:
		item->FontCol = StdColorBtnText;
		item->FontBack = StdColorBtnFace;
		item->FontBrush = StdBrushBtn;
	}

// vytvo�en� prvku
	item->Wnd = ::CreateWindowEx(
		0 | exstyle,					// roz���en� styl
		classname,						// t��da
		NULL,							// text
		WS_CHILD | style,				// styl
		item->Left,						// X
		item->Top,						// Y
		width,							// ���ka
		height,							// v��ka
		parent->Wnd,					// rodi�
		(HMENU)WindowID,				// identifikace
		hInstance,						// instance
		NULL);							// parametry

// inicializace prvku
	if (item->Wnd != NULL)
	{
		HWND wnd = item->Wnd;

		switch (typ)
		{
		case WINITEM_MEMO:
			::SendMessage(wnd, EM_SETLIMITTEXT, 0, 0);
			break;

		case WINITEM_RICHMEMO:
			::SendMessage(wnd, EM_SETTEXTMODE, TM_PLAINTEXT, 0);
			{
				PARAFORMAT pf;
				pf.cbSize = sizeof(PARAFORMAT);
				pf.dwMask = PFM_TABSTOPS;
				pf.cTabCount = MAX_TAB_STOPS;
				for (int i = 0; i < MAX_TAB_STOPS; i++)
				{
					pf.rgxTabs[i] = i*960;
				}
				::SendMessage(wnd, EM_SETPARAFORMAT, 0, (LPARAM)&pf);
			}
			::SendMessage(wnd, EM_SETTEXTMODE, TM_PLAINTEXT, 0);

		case WINITEM_RICHEDIT:
			::SendMessage(wnd, EM_EXLIMITTEXT, 0, (LPARAM)0x10000000);
			::SendMessage(wnd, EM_SETEVENTMASK, 0, (LPARAM)ENM_CHANGE); 
			break;

		case WINITEM_HTRACKBAR:
		case WINITEM_VTRACKBAR:
			::SendMessage(wnd, TBM_SETTICFREQ, 10, 0);
			::SendMessage(wnd, TBM_SETPAGESIZE, 0, 10);
			::SendMessage(wnd, TBM_SETRANGE, TRUE, MAKELONG(0, 100));
			if (typ == WINITEM_VTRACKBAR) ::SendMessage(wnd, TBM_SETPOS, TRUE, 100);
			break;

		case WINITEM_HSCROLLBAR:
		case WINITEM_VSCROLLBAR:
			{
				SCROLLINFO si;
				si.cbSize = sizeof(SCROLLINFO);
				si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
				si.nMin = 0;
				si.nMax = SCROLLRANGE;
				si.nPage = (SCROLLRANGE+5)/10;
				si.nPos = 0;

				::SetScrollInfo(wnd, SB_CTL, &si, TRUE);
			}
			break;
		}
	}

// nastaven� fontu
	WindowSetFont(item);

// zji�t�n� klientsk� ���ky a v��ky
	InitClientRect(item);

// zam��en� fokusu
	if (WindowFocus == DialogParent) FocusNext();
}


/////////////////////////////////////////////////////////////////////////////
// zru�en� prvku

void WindowDel(int inx)
{
// nelze zru�it hlavn� okno
	if (inx == 0) return;

// zru�en� ��sla aktu�ln�ho prvku
	if (WindowID == inx) WindowID = -1;

// okno mus� b�t platn�
	if (Win.IsNotValid(inx)) return;

// adresa prvku
	WINITEM* item = &Win[inx];

// ru�en� evidence prvku okna v rodi�ovsk�m okn�
	if ((item->Typ != WINITEM_WINDOW) && (item->Typ != WINITEM_WINDOWRO))
	{
		::DestroyWindow(item->Wnd);

		if (inx == WindowFocus)
		{
			FocusNext();
		}

		Win[item->Prev].Next = item->Next;
		Win[item->Next].Prev = item->Prev;

		if ((item->Parent == 0) &&
			(item->Next == item->Prev))
		{
			DialogOn(false);
		}
	}

// zru�en� okna v�etn� jeho potomk�
	else
	{
		for (int i = Win.Max() - 1; i >= 0; i--)
		{
			if (Win.IsValid(i) && (Win[i].Parent == inx))
			{
				WindowDel(i);
			}
		}

		::EnableWindow(Win[item->Parent].Wnd, TRUE);

		::DestroyWindow(item->Wnd);
		DialogParent = item->Parent;
		WindowFocus = DialogParent;
		Win[DialogParent].Disable = false;
		::SetFocus(Win[DialogParent].Wnd);
		ReqClose = false;
	}

// zru�en� polo�ky okna
	Win.Del(inx);

// po zru�en� v�ech prvk� n�vrat focusu na hlavn� okno
	if (Win.Num() == 1)
	{
		::SetFocus(MainFrame);
	}
}


/////////////////////////////////////////////////////////////////////////////
// posun fokusu na dal�� prvek

void FocusNext()
{
// v�choz� prvek
	int foc = WindowFocus;
	if (Win.IsNotValid(foc)) foc = DialogParent;
	int foc0 = foc;

// nalezen� dal��ho prvku
	for (;;)
	{
		foc = Win[foc].Next;
		if (foc == foc0) break;
		if ((Win[foc].TabStop) && (Win[foc].Show) && (!Win[foc].Disable || 
			(Win[foc].Typ == WINITEM_EDIT) || 
			(Win[foc].Typ == WINITEM_MEMO) || 
			(Win[foc].Typ == WINITEM_RICHEDIT) ||
			(Win[foc].Typ == WINITEM_RICHMEMO))) break;
	}

// korekce pro p�ep�na�
	if (Win[foc].Typ == WINITEM_BUTTONRADIO2)
	{
		foc0 = foc;

		do 
		{
			if (Win[foc0].Check && !Win[foc0].Disable) foc = foc0;
			foc0 = Win[foc0].Next;

		} while (Win[foc0].Typ == WINITEM_BUTTONRADIO);
	}

// nastaven� fokusu na prvek
	WindowFocus = foc;
	::SetFocus(Win[foc].Wnd);
}


/////////////////////////////////////////////////////////////////////////////
// posun fokusu na p�edchoz� prvek

void FocusPrev()
{
// v�choz� prvek
	int foc = WindowFocus;
	if (Win.IsNotValid(foc)) foc = DialogParent;
	int foc0 = foc;

// posun na za��tek p�ep�na��
	while (Win[foc].Typ == WINITEM_BUTTONRADIO)
	{
		foc = Win[foc].Prev;
	}

// nalezen� p�edchoz�ho prvku
	for (;;)
	{
		foc = Win[foc].Prev;
		if (foc == foc0) break;
		if ((Win[foc].TabStop) && (Win[foc].Show) && (!Win[foc].Disable ||
			(Win[foc].Typ == WINITEM_EDIT) || 
			(Win[foc].Typ == WINITEM_MEMO) || 
			(Win[foc].Typ == WINITEM_RICHEDIT) ||
			(Win[foc].Typ == WINITEM_RICHMEMO))) break;
	}

// korekce pro p�ep�na�
	if (Win[foc].Typ == WINITEM_BUTTONRADIO2)
	{
		foc0 = foc;

		do
		{
			if (Win[foc0].Check && !Win[foc0].Disable) foc = foc0;
			foc0 = Win[foc0].Next;

		} while (Win[foc0].Typ == WINITEM_BUTTONRADIO);
	}

// nastaven� fokusu na prvek
	WindowFocus = foc;
	::SetFocus(Win[foc].Wnd);
}


/////////////////////////////////////////////////////////////////////////////
// posun fokusu na dal�� p�ep�na�

void RadioNext()
{
	int foc = WindowFocus;
	if (Win.IsNotValid(foc)) return;
	int foc0 = foc;

	switch (Win[foc].Typ)
	{
	case WINITEM_BUTTONRADIO:
	case WINITEM_BUTTONRADIO2:
		do {
			foc = Win[foc].Next;

			if (Win[foc].Typ != WINITEM_BUTTONRADIO)
			{
				do {
					foc = Win[foc].Prev;
				} while (Win[foc].Typ == WINITEM_BUTTONRADIO);

				if (Win[foc].Typ != WINITEM_BUTTONRADIO2)
				{
					foc = Win[foc].Next;
				}
			}

		} while ((foc != foc0) && (Win[foc].Disable));

		RadioSet(foc);
		WindowFocus = foc;
		::SetFocus(Win[foc].Wnd);
		break;

	case WINITEM_BUTTON:
	case WINITEM_BUTTONICON:
	case WINITEM_BUTTONCHECK2:
	case WINITEM_DEFBUTTON:
		do {
			foc = Win[foc].Next;

			if ((Win[foc].Typ != WINITEM_BUTTON) &&
				(Win[foc].Typ != WINITEM_BUTTONICON) &&
				(Win[foc].Typ != WINITEM_BUTTONCHECK2) &&
				(Win[foc].Typ != WINITEM_DEFBUTTON))
			{
				do {
					foc = Win[foc].Prev;
				} while ((Win[foc].Typ == WINITEM_BUTTON) ||
						(Win[foc].Typ == WINITEM_BUTTONICON) ||
						(Win[foc].Typ == WINITEM_BUTTONCHECK2) ||
						(Win[foc].Typ == WINITEM_DEFBUTTON));

				foc = Win[foc].Next;
			}

		} while ((foc != foc0) && (Win[foc].Disable));

		WindowFocus = foc;
		::SetFocus(Win[foc].Wnd);
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// posun fokusu na p�edchoz� p�ep�na�

void RadioPrev()
{
	int foc = WindowFocus;
	if (Win.IsNotValid(foc)) return;
	int foc0 = foc;

	switch (Win[foc].Typ)
	{
	case WINITEM_BUTTONRADIO2:
		do {
			foc = Win[foc].Next;
		} while (Win[foc].Typ == WINITEM_BUTTONRADIO);
		foc = Win[foc].Prev;

		if (!Win[foc].Disable)
		{
			RadioSet(foc);
			WindowFocus = foc;
			::SetFocus(Win[foc].Wnd);
			break;
		}

	case WINITEM_BUTTONRADIO:
		do {
			foc = Win[foc].Prev;

			if ((Win[foc].Typ != WINITEM_BUTTONRADIO) &&
				(Win[foc].Typ != WINITEM_BUTTONRADIO2))
			{
				do {
					foc = Win[foc].Next;
				} while (Win[foc].Typ == WINITEM_BUTTONRADIO);
				foc = Win[foc].Prev;
			}
		} while ((foc != foc0) && (Win[foc].Disable));

		RadioSet(foc);
		WindowFocus = foc;
		::SetFocus(Win[foc].Wnd);
		break;

	case WINITEM_BUTTON:
	case WINITEM_BUTTONICON:
	case WINITEM_BUTTONCHECK2:
	case WINITEM_DEFBUTTON:
		do {
			foc = Win[foc].Prev;

			if ((Win[foc].Typ != WINITEM_BUTTON) &&
				(Win[foc].Typ != WINITEM_BUTTONICON) &&
				(Win[foc].Typ != WINITEM_BUTTONCHECK2) &&
				(Win[foc].Typ != WINITEM_DEFBUTTON))
			{
				do {
					foc = Win[foc].Next;
				} while ((Win[foc].Typ == WINITEM_BUTTON) ||
						(Win[foc].Typ == WINITEM_BUTTONICON) ||
						(Win[foc].Typ == WINITEM_BUTTONCHECK2) ||
						(Win[foc].Typ == WINITEM_DEFBUTTON));
				foc = Win[foc].Prev;
			}
		} while ((foc != foc0) && (Win[foc].Disable));

		WindowFocus = foc;
		::SetFocus(Win[foc].Wnd);
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� p�ep�na�e

void RadioSet(int inx)
{
	if (Win.IsNotValid(inx)) return;
	WINITEM* item = &Win[inx];

	ProgramFill = true;

	if ((item->Typ == WINITEM_BUTTONRADIO) ||
		(item->Typ == WINITEM_BUTTONRADIO2))
	{
		item->Check = true;;
		item->Check3 = false;
		::SendMessage(item->Wnd, BM_SETCHECK, BST_CHECKED, 0);

		while (item->Typ == WINITEM_BUTTONRADIO)
		{
			item = &Win[item->Prev];

			if ((item->Typ == WINITEM_BUTTONRADIO) ||
				(item->Typ == WINITEM_BUTTONRADIO2))
			{
				item->Check = false;;
				item->Check3 = false;
				::SendMessage(item->Wnd, BM_SETCHECK, BST_UNCHECKED, 0);
			}
		}

		item = &Win[inx];
		for (;;)
		{
			item = &Win[item->Next];

			if (item->Typ == WINITEM_BUTTONRADIO)
			{
				item->Check = false;;
				item->Check3 = false;
				::SendMessage(item->Wnd, BM_SETCHECK, BST_UNCHECKED, 0);
			}
			else
			{
				break;
			}
		}
	}

	ProgramFill = false;
}


/***************************************************************************\
*																			*
*									Joystick								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// aktualizace �daj� o joysticku

void AktJoystick(int inx)
{
// test, zda jsou �daje platn�
	if (JoystickValid[inx]) return;
	JoystickValid[inx] = true;

// test, zda je povolena dal�� aktualizace
	if (JoystickNext[inx] >= 0) return;

// na�ten� informac� o joysticku
	JOYCAPS jp;
	JOYINFO ji;

	if ((::joyGetDevCaps(inx, &jp, sizeof(JOYCAPS)) != JOYERR_NOERROR) ||
		(::joyGetPos(inx, &ji) != JOYERR_NOERROR))
	{

// p�i chyb� vynulov�n� parametr�
		JoystickX[inx] = 0;
		JoystickY[inx] = 0;
		JoystickZ[inx] = 0;
		Joystick1[inx] = false;
		Joystick2[inx] = false;
		Joystick3[inx] = false;
		Joystick4[inx] = false;
		JoystickNext[inx] = JOYSTICKTIME;
		return;
	}

// sou�adnice X
	int mx = jp.wXmax - jp.wXmin + 1;
	if (mx <= 2)
	{
		JoystickX[inx] = 0;
	}
	else
	{
		int tx = mx/512;
		int x = ji.wXpos;
		if (x < (int)jp.wXmin) x = jp.wXmin;
		if (x > (int)jp.wXmax) x = jp.wXmax;
		int dx = x - JoystickOldX[inx];
		if ((dx > tx) || (dx < -tx))
		{
			JoystickOldX[inx] = x;
			JoystickX[inx] = (double)Round((double)(int)(2*x - (jp.wXmin + jp.wXmax + 1))/mx*1000)*0.001;
		}
	}

// sou�adnice Y
	int my = jp.wYmax - jp.wYmin + 1;
	if (my <= 2)
	{
		JoystickY[inx] = 0;
	}
	else
	{
		int ty = my/512;
		int y = ji.wYpos;
		if (y < (int)jp.wYmin) y = jp.wYmin;
		if (y > (int)jp.wYmax) y = jp.wYmax;
		int dy = y - JoystickOldY[inx];
		if ((dy > ty) || (dy < -ty))
		{
			JoystickOldY[inx] = y;
			JoystickY[inx] = (double)Round((double)(int)((jp.wYmin + jp.wYmax + 1) - 2*y)/my*1000)*0.001;
		}
	}

// sou�adnice Z
	int mz = jp.wZmax - jp.wZmin + 1;
	if (mz <= 2)
	{
		JoystickZ[inx] = 0;
	}
	else
	{
		int tz = mz/512;
		int z = ji.wZpos;
		if (z < (int)jp.wZmin) z = jp.wZmin;
		if (z > (int)jp.wZmax) z = jp.wZmax;
		int dz = z - JoystickOldZ[inx];
		if ((dz > tz) || (dz < -tz))
		{
			JoystickOldZ[inx] = z;
			JoystickZ[inx] = (double)Round((double)(int)(2*z - (jp.wZmin + jp.wZmax + 1))/mz*1000)*0.001;
		}
	}

// tla��tka
	Joystick1[inx] = ((ji.wButtons & JOY_BUTTON1) != 0);
	Joystick2[inx] = ((ji.wButtons & JOY_BUTTON2) != 0);
	Joystick3[inx] = ((ji.wButtons & JOY_BUTTON3) != 0);
	Joystick4[inx] = ((ji.wButtons & JOY_BUTTON4) != 0);
}


/***************************************************************************\
*																			*
*									konzola									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// zapnut�/vypnut� konzoly

void ConsoleOnSet(bool on)
{
	if (ConsoleOn == on) return;
	ConsoleOn = on;

// zapnut� konzoly
	if (on)
	{
		if (MainFrameVisible)
		{
			MainFrameVisible = false;
			::ShowWindow(MainFrame, SW_HIDE);
		}

		if ((ConIn == INVALID_HANDLE_VALUE) || (ConIn == NULL))
		{
			ConIn = ::GetStdHandle(STD_INPUT_HANDLE);
		}

		if ((ConOut == INVALID_HANDLE_VALUE) || (ConOut == NULL))
		{
			ConOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
		}

		if ((ConErr == INVALID_HANDLE_VALUE) || (ConErr == NULL))
		{
			ConErr = ::GetStdHandle(STD_ERROR_HANDLE);
		}

		if ((ConIn == INVALID_HANDLE_VALUE) || 
			(ConOut == INVALID_HANDLE_VALUE) ||
			(ConErr == INVALID_HANDLE_VALUE) ||
			(ConIn == NULL) ||
			(ConOut == NULL) ||
			(ConErr == NULL))
		{
			if (::AllocConsole()) NewConsole = true;
			ConIn = ::GetStdHandle(STD_INPUT_HANDLE);
			ConOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
			ConErr = ::GetStdHandle(STD_ERROR_HANDLE);
		}

		SetConsoleText();
	}
	else
	{
		MainFrameShow();
	}
}


/////////////////////////////////////////////////////////////////////////////
// v�stup textu na konzolu

void ConsoleOut(const CString& text)
{
	if (ConsoleOn)
	{
		DWORD writen;
		int len = text.Length();
		char* buf = (char*)MemGet(len+1);
		::CharToOemBuff(text.DataData(), buf, len);
		::WriteFile(ConOut, buf, len, &writen, NULL);
		MemFree(buf);
	}
}


/////////////////////////////////////////////////////////////////////////////
// v�stup chybov�ho textu na konzolu

void ConsoleErr(const CString& text)
{
	if (ConsoleOn)
	{
		DWORD writen;
		int len = text.Length();
		char* buf = (char*)MemGet(len+1);
		::CharToOemBuff(text.DataData(), buf, len);
		::WriteFile(ConErr, buf, len, &writen, NULL);
		MemFree(buf);
	}
}


/////////////////////////////////////////////////////////////////////////////
// vstup textu z konzoly

void ConsoleIn(CString& text)
{
	if (ConsoleOn)
	{
		DWORD readen = 0;
		char* bufoem = (char*)MemGet(0x2000);
		::ReadFile(ConIn, bufoem, 0x1FFF, &readen, NULL);
		TCHAR* buf = (TCHAR*)MemGet((readen+1) * sizeof(TCHAR));
		::OemToCharBuff(bufoem, buf, readen);
		text.Term();
		text.Init(buf, readen);

		MemFree(buf);
		MemFree(bufoem);

		while (text.IsNotEmpty() && ((text[text.Length()-1] == 13) ||
				(text[text.Length()-1] == 10)))
		{
			text.Delete(text.Length()-1);
		}
	}
	else
	{
		text.Empty();
	}
}


/***************************************************************************\
*																			*
*							obsluha CD disk�								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// zah�jen� p�ehr�v�n� od zvolen� stopy

void CDPlay(int stopa)
{
// aktualizace �daj� o CD
	if (CDAkt())
	{

// korekce ��sla stopy
		if (CDTracks > 0)
		{
			while (stopa < 1) stopa += CDTracks;
			while (stopa > CDTracks) stopa -= CDTracks;
		}

// nastaven� form�tu stop
		MCI_SET_PARMS mcis;
		mcis.dwTimeFormat = MCI_FORMAT_TMSF;
		::mciSendCommand(CDDevice, MCI_SET, MCI_SET_TIME_FORMAT | MCI_WAIT, (DWORD)&mcis);

// zah�jen� p�ehr�v�n�
		MCI_PLAY_PARMS mcip;
		mcip.dwFrom = stopa;
		::mciSendCommand(CDDevice, MCI_PLAY, MCI_FROM, (DWORD)&mcip);

// zneplatn�n� �daj� o CD
		CDValidTime = 0;
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� stopy

void CDSetTrack(int stopa)
{
// aktualizace �daj� o CD
	if (CDAkt())
	{

// korekce ��sla stopy
		if (CDTracks > 0)
		{
			while (stopa < 1) stopa += CDTracks;
			while (stopa > CDTracks) stopa -= CDTracks;
		}

// nastaven� form�tu stop
		MCI_SET_PARMS mcis;
		mcis.dwTimeFormat = MCI_FORMAT_TMSF;
		::mciSendCommand(CDDevice, MCI_SET, MCI_SET_TIME_FORMAT | MCI_WAIT, (DWORD)&mcis);

// nastaven� stopy
		if (CDPlaing)
		{
			MCI_PLAY_PARMS mcip;
			mcip.dwFrom = stopa;
			::mciSendCommand(CDDevice, MCI_PLAY, MCI_FROM, (DWORD)&mcip);
		}
		else
		{
			MCI_SEEK_PARMS mcip;
			mcip.dwTo = stopa;
			::mciSendCommand(CDDevice, MCI_SEEK, MCI_TO | MCI_WAIT, (DWORD)&mcip);
		}

// zneplatn�n� �daj� o CD
		CDValidTime = 0;
	}
}


/////////////////////////////////////////////////////////////////////////////
// zastaven� p�ehr�v�n� CD

void CDStop()
{
// aktualizace �daj� o CD
	if (CDAkt())
	{
		if (CDPlaing || CDPausing)
		{
			::mciSendCommand(CDDevice, MCI_STOP, MCI_WAIT, NULL);

			MCI_SET_PARMS mcis;
			mcis.dwTimeFormat = MCI_FORMAT_MILLISECONDS;
			::mciSendCommand(CDDevice, MCI_SET, MCI_SET_TIME_FORMAT | MCI_WAIT, (DWORD)&mcis);

			MCI_SEEK_PARMS mcip;
			mcip.dwTo = CDStart;
			::mciSendCommand(CDDevice, MCI_SEEK, MCI_TO | MCI_WAIT, (DWORD)&mcip);

			CDValidTime = 0;
		}
	}
}
 

/////////////////////////////////////////////////////////////////////////////
// vysunut� CD

void CDEject(bool eject)
{
	if (CDOpen())
	{
		::mciSendCommand(CDDevice, MCI_SET,
				eject ? MCI_SET_DOOR_OPEN : MCI_SET_DOOR_CLOSED, NULL);
	}
}


/////////////////////////////////////////////////////////////////////////////
// pauza CD

void CDPause()
{
// aktualizace �daj� o CD
	if (CDAkt())
	{

// zapauzov�n� p�ehr�v�n�
		if (CDPlaing)
		{
			::mciSendCommand(CDDevice, MCI_PAUSE, MCI_WAIT, NULL);
			CDValidTime = 0;
		}
		else
		{

// odpauzov�n� p�ehr�v�n�
			if (CDPausing)
			{
				::mciSendCommand(CDDevice, MCI_PLAY, 0, NULL);
				CDValidTime = 0;
			}
		}
 	}
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� pozice CD v ms

void CDSetPos(int pozice)
{
// aktualizace �daj� o CD
	if (CDAkt())
	{

// nastaven� form�tu na milisekundy
		MCI_SET_PARMS mcis;
		mcis.dwTimeFormat = MCI_FORMAT_MILLISECONDS;
		::mciSendCommand(CDDevice, MCI_SET, MCI_SET_TIME_FORMAT | MCI_WAIT, (DWORD)&mcis);

// korekce pozice
		if (CDLength > 0)
		{
			while (pozice < 0) pozice += CDLength;
			pozice = pozice % CDLength;
		}

// nastaven� pozice p�ehr�v�n�
		if (CDPlaing)
		{
			MCI_PLAY_PARMS mcip;
			mcip.dwFrom = pozice + CDStart;
			::mciSendCommand(CDDevice, MCI_PLAY, MCI_FROM, (DWORD)&mcip);
		}
		else
		{
			MCI_SEEK_PARMS mcip;
			mcip.dwTo = pozice + CDStart;
			::mciSendCommand(CDDevice, MCI_SEEK, MCI_TO | MCI_WAIT, (DWORD)&mcip);
		}

// zneplatn�n� �daj� o CD
		CDValidTime = 0;
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� pozice v aktivn� stop� CD v ms

void CDSetTrackPos(int pozice)
{
// aktualizace �daj� o CD
	if (CDAkt())
	{

// nastaven� form�tu na stopu
		MCI_SET_PARMS mcis;
		mcis.dwTimeFormat = MCI_FORMAT_TMSF;
		::mciSendCommand(CDDevice, MCI_SET, MCI_SET_TIME_FORMAT | MCI_WAIT, (DWORD)&mcis);

// korekce pozice
		if (pozice >= CDTrackLength) pozice = CDTrackLength - 1;
		if (pozice < 0) pozice = 0;

// v�po�et �asov�ho �daje pozice
		int frame = ((pozice % 1000) * 75 + 500)/1000;
		int sekunda = (pozice / 1000) % 60;
		int minuta = pozice / 60000;

// nastaven� pozice p�ehr�v�n�
		if (CDPlaing)
		{
			MCI_PLAY_PARMS mcip;
			mcip.dwFrom = MCI_MAKE_TMSF(CDTrack, minuta, sekunda, frame);
			::mciSendCommand(CDDevice, MCI_PLAY, MCI_FROM, (DWORD)&mcip);
		}
		else
		{
			MCI_SEEK_PARMS mcip;
			mcip.dwTo = MCI_MAKE_TMSF(CDTrack, minuta, sekunda, frame);
			::mciSendCommand(CDDevice, MCI_SEEK, MCI_TO | MCI_WAIT, (DWORD)&mcip);
		}

// zneplatn�n� �daj� o CD
		CDValidTime = 0;
	}
}


/////////////////////////////////////////////////////////////////////////////
// uzav�en� ovlada�e CD

void CDClose()
{
	if (CDDevice)
	{
		::mciSendCommand(CDDevice, MCI_CLOSE, MCI_WAIT, NULL);
		CDDevice = 0;
	}
	CDDiskValid = false;
}

/////////////////////////////////////////////////////////////////////////////
// otev�en� ovlada�e CD

bool CDOpen()
{
	if (CDDevice == 0)
	{
		if (CDError > 0) return false;

		MCI_OPEN_PARMS mcio;
		mcio.lpstrDeviceType = _T("cdaudio");

		if (::mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_WAIT
				| MCI_OPEN_SHAREABLE, (DWORD)&mcio))
		{
			CDError = CDERRORTIME;
			return false;
		}

		CDDevice = mcio.wDeviceID;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace �daj� o CD disku

bool CDAkt()
{
// kontrola, zda je pot�eba �daje aktualizovat
	if (CDValidTime > 0) return true;

// vynulov�n� ukazatel�
	if (!CDDiskValid)
	{
		CDStart = 2000;				// startovac� pozice m�dia
		CDLength = CDDEFLENGTH;		// d�lka m�dia v ms
		CDTracks = 1;				// po�et stop m�dia
		CDTrack = 1;				// p�ehr�van� stopa
		CDTrackLength = CDDEFLENGTH;// d�lka p�ehr�van� stopy
	}
	CDPos = 0;						// p�ehr�van� pozice
	CDTrackPos = 0;					// pozice v p�ehr�van� stop�
	CDPlaing = false;					// neprob�h� p�ehr�v�n�
	CDPausing = false;				// nen� pauza

// otev�en� za��zen�
	if (!CDOpen()) return false;
	CDValidTime = CDAKTTIME;

// nastaven� form�tu na milisekundy
	MCI_SET_PARMS mcis;
	mcis.dwTimeFormat = MCI_FORMAT_MILLISECONDS;
	::mciSendCommand(CDDevice, MCI_SET, MCI_SET_TIME_FORMAT | MCI_WAIT, (DWORD)&mcis);

// na�ten� p�ehr�van� pozice CD
	MCI_STATUS_PARMS mcit;
	mcit.dwItem = MCI_STATUS_POSITION;
	mcit.dwReturn = CDStart;
	int result = ::mciSendCommand(CDDevice, MCI_STATUS, MCI_STATUS_ITEM | MCI_WAIT, (DWORD)&mcit);
	mcit.dwReturn -= CDStart;
	if ((int)mcit.dwReturn < 0) mcit.dwReturn = 0;
	if (result == 0) CDPos = mcit.dwReturn;

// na�ten� aktu�ln�ho stavu
	mcit.dwItem = MCI_STATUS_MODE;
	result = mciSendCommand(CDDevice, MCI_STATUS, MCI_STATUS_ITEM | MCI_WAIT, (DWORD)&mcit);
	if (result == 0)
	{
		switch (mcit.dwReturn)
		{
		case MCI_MODE_PLAY:		CDPlaing = true;
								break;

		case MCI_MODE_PAUSE:	CDPausing = true;
								break;

		case MCI_MODE_NOT_READY:
		case MCI_MODE_OPEN:
								CDDiskValid = false;
								CDStart = 2000;
								CDLength = CDDEFLENGTH;
								CDTracks = 1;
								CDTrack = 1;
								CDTrackLength = CDDEFLENGTH;
								CDPos = 0;
								CDTrackPos = 0;
								CDPlaing = false;
								CDPausing = false;
								return false;

		default:				if (CDPos > 800)
								{
									CDPausing = true;
								}
		}
	}

// test, zda je pot�eba aktualizovat informace o vlo�en�m CD disku
	if (!CDDiskValid)
	{
		CDDiskValid = true;

// na�ten� startovac� pozice CD (form�t je nastaven na milisekundy)
		mcit.dwItem = MCI_STATUS_POSITION;
		result = ::mciSendCommand(CDDevice, MCI_STATUS, 
				MCI_STATUS_ITEM | MCI_STATUS_START | MCI_WAIT, (DWORD)&mcit);
		if ((result == 0) && ((int)mcit.dwReturn > 0))
		{
			CDPos += CDStart;
			CDStart = mcit.dwReturn;
			if (CDStart > CDMAXLENGTH) CDStart = CDMAXLENGTH;
			CDPos -= CDStart;
			if (CDPos < 0) CDPos = 0;
		}

// na�ten� d�lky m�dia
		mcit.dwItem = MCI_STATUS_LENGTH;
		result = ::mciSendCommand(CDDevice, MCI_STATUS, MCI_STATUS_ITEM | MCI_WAIT, (DWORD)&mcit);
		if ((result == 0) && ((int)mcit.dwReturn > 0))
		{
			CDLength = mcit.dwReturn;
			if (CDLength > CDMAXLENGTH) CDLength = CDMAXLENGTH;
		}

// na�ten� po�tu stop m�dia
		mcit.dwItem = MCI_STATUS_NUMBER_OF_TRACKS;
		result = ::mciSendCommand(CDDevice, MCI_STATUS, MCI_STATUS_ITEM | MCI_WAIT, (DWORD)&mcit);
		if ((result == 0) && ((int)mcit.dwReturn > 0))
		{
			CDTracks = mcit.dwReturn;
			if (CDTracks > CDMAXTRACKS) CDTracks = CDMAXTRACKS;
		}

// neplatn� informace o aktivn� stop�
		CDTrack = 0;
	}

// nastaven� form�tu na stopy
	mcis.dwTimeFormat = MCI_FORMAT_TMSF;
	::mciSendCommand(CDDevice, MCI_SET, MCI_SET_TIME_FORMAT | MCI_WAIT, (DWORD)&mcis);

// na�ten� p�ehr�van� pozice CD ve stop� (sekunda = 75 frame)
	mcit.dwReturn = 0;
	mcit.dwItem = MCI_STATUS_POSITION;
	result = ::mciSendCommand(CDDevice, MCI_STATUS, MCI_STATUS_ITEM | MCI_WAIT, (DWORD)&mcit);
	int oldpos = CDTrackPos;
	if (result == 0)
	{
		CDTrackPos =	MCI_TMSF_MINUTE(mcit.dwReturn) * 60000 +
						MCI_TMSF_SECOND(mcit.dwReturn) * 1000 +
						(MCI_TMSF_FRAME (mcit.dwReturn) * 1000 + 37)/75;
	}
	if (CDTrackPos < 0) CDTrackPos = 0;

// p��prava ��sla aktu�ln� stopy
	int newtrack = MCI_TMSF_TRACK(mcit.dwReturn);
	if ((newtrack < 1) || (newtrack > CDTracks)) newtrack = 1;
	if ((newtrack == CDTrack + 1) &&
		(newtrack > 1) &&
		(CDTrackPos > CDTrackLength - 10000) &&
		(CDTrackPos > 5000) &&
		(CDTrackPos > oldpos - 4000))
	{
		newtrack--;
	}

// na�ten� d�lky p�ehr�van� stopy, pokud se stopa zm�nila
	if (newtrack != CDTrack)
	{
		CDTrack = newtrack;

		mcis.dwTimeFormat = MCI_FORMAT_MILLISECONDS;
		::mciSendCommand(CDDevice, MCI_SET, MCI_SET_TIME_FORMAT | MCI_WAIT, (DWORD)&mcis);
		
		mcit.dwTrack = CDTrack;
		mcit.dwItem = MCI_STATUS_LENGTH;
		result = ::mciSendCommand(CDDevice, MCI_STATUS, MCI_STATUS_ITEM
						| MCI_TRACK | MCI_WAIT, (DWORD) &mcit);

		if ((result == 0) && ((int)mcit.dwReturn > 0))
		{
			CDTrackLength = mcit.dwReturn;
		}
	}

// korekce n�kter�ch �daj�
	if (CDTrackLength > CDLength) CDTrackLength = CDLength;
	if (CDTrackPos > CDTrackLength) CDTrackPos = CDTrackLength;
	if (CDPos > CDLength) CDPos = CDLength;
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// obsluha CD na pozad�

void PlayCDBack()
{
	CDError--;
	CDValidTime--;

	if (CDValidTime == -CDCLOSETIME)
	{
		CDClose();
	}

	if ((CDDevice != 0) && CDPlaing && (CDValidTime > 1))
	{
		CDPos += 55;
		if (CDPos > CDLength) CDPos = CDLength;
		CDTrackPos += 55;
		if (CDTrackPos > CDTrackLength) CDTrackPos = CDTrackLength;
	}
}


/***************************************************************************\
*																			*
*							obsluha soubor�									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� informac� o velikosti a voln�m m�stu aktivn�ho disku

void GetDiskSpace()
{
// na�ten� informac� nov�j�� funkc�
	if (!pGetDiskFreeSpaceEx ||
		!pGetDiskFreeSpaceEx(NULL, &DiskFreeUser, &DiskSize, &DiskFree))

// na�ten� informac� star�� metodou
	{
		DWORD sectc;
		DWORD bytes;
		DWORD freec;
		DWORD sizec;

		if (!::GetDiskFreeSpace(NULL, &sectc, &bytes, &freec, &sizec))
		{
			sectc = 0;
			bytes = 0;
			freec = 0;
			sizec = 0;
		}

		DiskSize = sectc * bytes * sizec;
		DiskFree = sectc * bytes * freec;
		DiskFreeUser = DiskFree;
	}
}


/////////////////////////////////////////////////////////////////////////////
// otev�en� souboru pro �ten�

bool FileReadOpen()
{
	FileCloseTime = FILECLOSETIME;

	if (FileReadHandle != NULL) return true;

	if (FileRead.IsEmpty())
	{
		FileError = true;
		return false;
	}

	FileReadHandle = ::CreateFile(FileRead, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (FileReadHandle == INVALID_HANDLE_VALUE)
	{
		FileReadHandle = NULL;
		FileError = true;
		return false;
	}

	if (FileReadBuf == NULL)
	{
		FileReadBuf = (BYTE*)MemGet(FILEBUFFER);
	}

	return true;
}


/////////////////////////////////////////////////////////////////////////////
// otev�en� souboru pro z�pis

bool FileWriteOpen()
{
	if (FileWriteBufN == 0) FileWriteBufOff = FileWriteOff;

	FileCloseTime = FILECLOSETIME;

	if (FileWriteHandle != NULL) return true;

	if (FileWrite.IsEmpty())
	{
		FileError = true;
		return false;
	}

	FileWriteHandle = ::CreateFile(FileWrite, GENERIC_WRITE, FILE_SHARE_READ,
			NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (FileWriteHandle == INVALID_HANDLE_VALUE)
	{
		FileWriteHandle = NULL;
		FileError = true;
		return false;
	}

	if (FileWriteBuf == NULL)
	{
		FileWriteBuf = (BYTE*)MemGet(FILEBUFFER);
	}

	return true;
}


/////////////////////////////////////////////////////////////////////////////
// vypr�zdn�n� z�pisov�ho bufferu

bool FileWriteFlush()
{
	bool result = true;

	if (FileWriteBufN != 0)
	{
		DWORD write = 0;

		if (!FileWriteOpen() || 
			((int)::SetFilePointer(FileWriteHandle, FileWriteBufOff, NULL, FILE_BEGIN) == -1) ||
			(!::WriteFile(FileWriteHandle, FileWriteBuf, FileWriteBufN, &write, NULL)) ||
			((int)write != FileWriteBufN))
		{
			result = false;
			FileError = true;
		}
		FileWriteBufN = 0;
	}

	FileWriteBufOff = FileWriteOff;
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// z�pis bajtu do v�stupn�ho souboru

void _fastcall FileWriteBlok(BYTE* buf, int size)
{
	for (; size > 0; size--)
	{
		FileWriteByte(*buf);
		buf++;
	}
}

void FileWriteDWord(DWORD data)
{
	FileWriteWord(LOWORD(data));
	FileWriteWord(HIWORD(data));
}

void FileWriteWord(WORD data)
{
	FileWriteByte((BYTE)data);
	FileWriteByte((BYTE)(data >> 8));
}

void FileWriteByte(BYTE data)
{
	if (!FileWriteOpen()) return;

	int off = FileWriteOff - FileWriteBufOff;

	if (((DWORD)off <= (DWORD)FileWriteBufN) && (off < FILEBUFFER))
	{
		FileWriteOff++;
		FileWriteBuf[off] = data;
		off++;
		if (off > FileWriteBufN) FileWriteBufN = off;
	}
	else
	{
		if (FileWriteFlush())
		{
			FileWriteOff++;
			FileWriteBuf[0] = data;
			FileWriteBufN = 1;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// �ten� bajtu ze vstupn�ho souboru

WORD FileReadWord()
{
	BYTE data1 = FileReadByte();
	BYTE data2 = FileReadByte();

	return (WORD)(data1 + data2*256);
}


BYTE FileReadByte()
{
	if (!FileReadOpen()) return 0;

	int off = FileReadOff - FileReadBufOff;

	if ((DWORD)off < (DWORD)FileReadBufN)
	{
		FileReadOff++;
		return FileReadBuf[off];
	}

	FileReadBufN = 0;
	FileReadBufOff = FileReadOff - FILEBUFFER/4;
	if (FileReadBufOff < 0) FileReadBufOff = 0;

	DWORD read = 0;

	if (((int)::SetFilePointer(FileReadHandle, FileReadBufOff, NULL, FILE_BEGIN) == -1) ||
		!::ReadFile(FileReadHandle, FileReadBuf, FILEBUFFER, &read, NULL))
	{
		FileError = true;
	}

	FileReadBufN = (int)read;

	off = FileReadOff - FileReadBufOff;
	FileReadOff++;

	if ((DWORD)off < (DWORD)FileReadBufN) return FileReadBuf[off];

	FileError = true;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
//na�ten� bloku ze souboru

void FileReadBlok(BYTE* buf, int size)
{
// otev�en� souboru
	if (!FileReadOpen()) return;

// kontrola velikosti dat
	if (size <= 0)
	{
		FileReadOff += size;
		return;
	}

// na�ten� velk�ho bloku dat
	DWORD read = 0;
	if (size >= FILEBUFFER)
	{
		if (((int)::SetFilePointer(FileReadHandle, FileReadOff, NULL, FILE_BEGIN) == -1) ||
		!::ReadFile(FileReadHandle, buf, size, &read, NULL))
		{
			FileError = true;
		}
		else
		{
			FileReadOff += size;
		}
		return;
	}

// p�enos bloku dat z bufferu
	for (int i = 4; i > 0; i--)
	{
		int off = FileReadOff - FileReadBufOff;
		if ((DWORD)off < (DWORD)FileReadBufN)
		{
			int num = FileReadBufN - off;
			if (num > size) num = size;
			MemCopy(buf, FileReadBuf + off, num);
			FileReadOff += num;
			size -= num;
			if (size == 0) return;
			buf += num;
		}
		
// na�ten� dal��ho bloku dat do bufferu
		FileReadBufN = 0;
		FileReadBufOff = FileReadOff - FILEBUFFER/4;
		if (FileReadBufOff < 0) FileReadBufOff = 0;

		if (((int)::SetFilePointer(FileReadHandle, FileReadBufOff, NULL, FILE_BEGIN) == -1) ||
			!::ReadFile(FileReadHandle, FileReadBuf, FILEBUFFER, &read, NULL))
		{
			FileError = true;
		}

		FileReadBufN = (int)read;
		if (FileReadBufN == 0) return;
	}
}


/////////////////////////////////////////////////////////////////////////////
// uzav�en� soubor�

void FileClose()
{
	if (FileReadHandle != NULL)
	{
		if (!::CloseHandle(FileReadHandle)) FileError = true;
		FileReadHandle = NULL;
		FileReadBufN = 0;
	}

	if (FileWriteHandle != NULL)
	{
		FileWriteFlush();
		if (!::CloseHandle(FileWriteHandle)) FileError = true;
		FileWriteHandle = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// p��prava jm�na ALIAS (mus� b�t o�ezan� mezery ze za��tku a konce!)

void _fastcall InitAlias(CString name)
{
// odstran�n� "\" z konce
	if (name.LastChar() == _T('\\')) name.Delete(name.Length() - 1);

// nalezen� konce cesty
	int pos = name.RevFind(_T('\\')) + 1;
	if (pos == 0) pos = name.RevFind(_T(':')) + 1;

// jm�no kl��e a ALIASES
	AliasKey = name.Right(name.Length() - pos);
	AliasName = name.Left(pos) + _T("ALIASES.INI");

// korekce na pln� jm�no souboru
	TCHAR buf[1024];
	buf[0] = 0;
	TCHAR* n;
	int len = ::GetFullPathName(AliasName, 1023, buf, &n);
	if ((len > 0) && (len < 1024))
	{
		AliasName.Term();
		AliasName.Init(buf, len);
	}
}

/////////////////////////////////////////////////////////////////////////////
// inicializace skupiny ALIASES

void _fastcall InitAliasGroup(int lan)
{
	CString temp;
	temp.Int(JazykInfo[lan].LangID);
	AliasGroup = _T("LANGID ");
	AliasGroup += temp;
}

/***************************************************************************\
*																			*
*							prov�d�n� programu								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// inicializace v�stupn�ho bufferu

void InitDispBuf()
{
// p��znak, zda se pou��v� v�stupn� buffer
	DispBufUse = ((DispWidth != Width) || (DispHeight != Height));
	DispBufUse2 = (((2*DispWidth) == Width) && ((2*DispHeight) == Height));

	if (DispBufUse)
	{

// p��prava velikosti v�stupn�ho bufferu
		int oldsize = DispBufSize;				// p�vodn� velikost bufferu
		DispBufSize = DispWidthByte * DispHeight;	// velikost bufferu v bajtech
		if (DispBufUse2) DispBufSize *= 4;		// korekce pro TRUE COLOR

// zv�t�en� bufferu
		if (DispBufSize > oldsize)
		{
			MemFree(DispBuf);
			DispBuf = (BYTE*)MemGet(DispBufSize * sizeof(BYTE));
		}

// buffer mapov�n� bodu obr�zku na v�stupn� bod
		int i;
		MemFree(DispMapX);
		DispMapX = (int*)MemGet(Width * sizeof(int));

		for (i = 0; i < Width; i++)
		{
			int off = RoundM(i * Meritko);
			if (off >= DispWidth) off = DispWidth - 1;
			DispMapX[i] = off;
		}

// buffer mapov�n� linky obr�zku na v�stupn� linku
		MemFree(DispMapY);
		DispMapY = (int*)MemGet(Height * sizeof(int));

		for (i = 0; i < Height; i++)
		{
			int off = RoundM(i * Meritko);
			if (off >= DispHeight) off = DispHeight - 1;
			DispMapY[i] = off;
		}

// buffer zp�tn�ho mapov�n� v�stupn�ho bodu na offset v lince obr�zku
		MemFree(DispRemapX);
		DispRemapX = (int*)MemGet(DispWidth * sizeof(int));

		for (i = 0; i < DispWidth; i++)
		{
			int off = RoundM(i / Meritko);
			if (off >= Width) off = Width - 1;
			DispRemapX[i] = off;
		}

// buffer zp�tn�ho mapov�n� v�stupn� linky na offset v obr�zku
		MemFree(DispRemapY);
		DispRemapY = (int*)MemGet(DispHeight * sizeof(int));

		for (i = 0; i < DispHeight; i++)
		{
			int off = RoundM(i / Meritko);
			if (off >= Height) off = Height - 1;
			DispRemapY[i] = (Height - 1 - off) * WidthByte;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// inicializace informac� o plo�e

void ExecInitMap()
{
	int oldwidth = Width;
	int oldheight = Height;
	int oldsize = WidthByte * oldheight;

	CMap* map = &Map[0];
	WidthN = map->Width();				// ���ka plochy v ikon�ch
	Width = WidthN * ICONWIDTH;			// ���ka plochy v bodech
	WidthByte = (Width + 3) & ~3;		// d�lka linky v bajtech
	RowByte = WidthByte * ICONHEIGHT;	// d�lka ��dku ikon v bajtech
	HeightN = map->Height();			// v��ka plochy v ikon�ch
	Height = HeightN * ICONHEIGHT;		// v��ka plochy v bodech

	int size = WidthByte * Height;
	if (size > oldsize)
	{
		MemFree(BackBuf);
		MemFree(PicBuf);
		MemFree(TextBuf);
		MemFree(AktBuf);

		BackBuf = (BYTE*)MemGet(size * sizeof(BYTE));
		PicBuf = (BYTE*)MemGet(size * sizeof(BYTE));
		TextBuf = (BYTE*)MemGet(size * sizeof(BYTE));
		AktBuf = (bool*)MemGet(size * sizeof(bool));
	}

	if ((oldwidth != Width) || (oldheight != Height))
	{
		MemFill(BackBuf, size, BlackCol);

		InitDispBuf();
	}

	DispLeft = 0;
	DispTop = 0;
	AktAllIcon();

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

//	RecalcD3D();

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------
}


/////////////////////////////////////////////////////////////////////////////
// nastavov�n� rozm�r� hlavn�ho okna

void OnSizing(RECT* rc, int side)
{
// v dialogov�m m�du nen� omezen�
	if (DialogMode && !DialogGraph) return;

// na�ten� polohy a rozm�r� okna
	RECT wrc;
	::GetWindowRect(MainFrame, &wrc);

// na�ten� klientsk�ch sou�adnic okna
	RECT crc;
	::GetClientRect(MainFrame, &crc);

// korekce o v��ku stavov� li�ty
	if (StatusVisible)
	{
		RECT src;
		::GetWindowRect(StatusBar, &src);
		crc.bottom -= src.bottom - src.top;
	}

// v�po�et nastaven� ���ky a v��ky klientsk� oblasti
	int width = (rc->right - rc->left) - ((wrc.right - wrc.left) - (crc.right - crc.left));
	int height = (rc->bottom - rc->top) - ((wrc.bottom - wrc.top) - (crc.bottom - crc.top));

// ur�en� nov�ho m���tka
	double meritko = ((double)width/(double)Width + (double)height/(double)Height)/2;

// zarovn�n� m���tka
	if (!FullScreen)
	{
		meritko = (double)Round(MODULMERITKA*meritko)/MODULMERITKA;
	}
	else
	{
		meritko = (double)Round(MODULMERITKA2*meritko)/MODULMERITKA2;
	}
	if (meritko < (double)1/ICONWIDTH) meritko = (double)1/ICONWIDTH;

// v�po�et zm�ny rozm�r� 
	width -= (int)(Width*meritko + 0.01);
	height -= (int)(Height*meritko + 0.01);

// bude zmen�en� ���ky
	if (width != 0)
	{
		switch(side)
		{
		case WMSZ_TOP:
		case WMSZ_TOPLEFT:
		case WMSZ_LEFT:
		case WMSZ_BOTTOMLEFT:
			rc->left += width;
			break;

		case WMSZ_TOPRIGHT:
		case WMSZ_RIGHT:
		case WMSZ_BOTTOMRIGHT:
		case WMSZ_BOTTOM:
			rc->right -= width;
		}
	}

// bude zmen�en� v��ky
	if (height != 0)
	{
		switch(side)
		{
		case WMSZ_TOPLEFT:
		case WMSZ_TOP:
		case WMSZ_TOPRIGHT:
		case WMSZ_LEFT:
			rc->top += height;
			break;

		case WMSZ_BOTTOMLEFT:
		case WMSZ_BOTTOM:
		case WMSZ_BOTTOMRIGHT:
		case WMSZ_RIGHT:
			rc->bottom -= height;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace rozm�r� okna

void _fastcall OnSize(WINITEM* item)
{
// p�esun stavov�ho ��dku
	if ((item == &Win[0]) && !FullScreen)
	{
		::SendMessage(StatusBar, WM_SIZE, 0, 0);
	}

// inicializace klientsk�ch rozm�r�
	InitClientRect(item);

// ur�en� nov�ho m���tka
	if (item == &Win[0])
	{
		Meritko = (double)(ClientWidth+4) / (double)Width;
		double m = (double)(ClientHeight+4) / (double)Height;
		if (m < Meritko) Meritko = m;

// zarovn�n� m���tka
		if (!FullScreen)
		{
			Meritko = (double)(int)(MODULMERITKA*Meritko)/MODULMERITKA;
		}
		else
		{
			Meritko = (double)(int)(MODULMERITKA2*Meritko)/MODULMERITKA2;
		}

		if (Meritko < (double)1/ICONWIDTH) Meritko = (double)1/ICONWIDTH;
		if (!FullScreen && !::IsZoomed(MainFrame) && !::IsIconic(MainFrame)) Meritko0 = Meritko;

// p��prava rozm�r� k zobrazen�
		int oldwidth = DispWidth;
		int oldheight = DispHeight;

		DispWidth = Round(Width*Meritko);
		DispWidthByte = (DispWidth + 3) & ~3;
		DispHeight = Round(Height*Meritko);
		DispLeft = (ClientWidth - DispWidth)/2;
		DispTop = (ClientHeight - DispHeight)/2;

// aktualizace bufferu zobrazen�
		if ((oldwidth != DispWidth) || (oldheight != DispHeight))
		{
			InitDispBuf();
		}
	}

// p�ekreslen� okna
	RePaint(item);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� po�adavku p�ekreslen� v�ech ikon

void AktAllIcon()
{
	MemFill(AktBuf, HeightN*WidthN*sizeof(bool), true);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� po�adavku p�ekreslen� ikony (s kontrolou sou�adnic)

void _fastcall AktIcon(int x, int y)
{
	if (((DWORD)x < (DWORD)WidthN) && ((DWORD)y < (DWORD)HeightN))
	{
		SetAktIcon(x, y, true);
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� po�adavku p�ekreslen� ikony v intervalu (v�etn� koncov�ch bod�)

void _fastcall AktIconBoxG(int x1, int y1, int x2, int y2)
{
	AktIconBox(x1/ICONWIDTH, y1/ICONHEIGHT, x2/ICONWIDTH, y2/ICONHEIGHT);
}

void _fastcall AktIconBox(int x1, int y1, int x2, int y2)
{
	if (x1 < 0) x1 = 0;
	x2++;
	int widthn = WidthN;
	if (x2 > widthn) x2 = widthn;
	x2 -= x1;

	if (x2 > 0)
	{
		if (y1 < 0) y1 = 0;
		y2++;
		if (y2 > HeightN) y2 = HeightN;
		y2 -= y1;

		bool* buf = AktBuf + x1 + widthn*y1;

		for (; y2 > 0; y2--)
		{
			MemFill(buf, x2*sizeof(bool), true);
			buf += widthn;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// vykreslen� sprajtu do v�stupn� plochy (pozad� mus� b�t vykresleno)

void SpritePaint(int index)
{
// adresa sprajtu
	CSprite* sprite = &Sprite[index];

// jen pokud je sprajt viditeln�
	if (!sprite->Visible()) return;

// okno k zobrazen� sprajtu
	int x1 = SpriteWinX1;
	if (x1 < 0) x1 = 0;
	if (x1 > Width) x1 = Width;

	int y1 = SpriteWinY1;
	if (y1 < 0) y1 = 0;
	if (y1 > Height) y1 = Height;

	int x2 = SpriteWinX2;
	if (x2 < 0) x2 = 0;
	if (x2 > Width) x2 = Width;

	int y2 = SpriteWinY2;
	if (y2 < 0) y2 = 0;
	if (y2 > Height) y2 = Height;

	if (x1 > x2)
	{
		int tmp = x2;
		x2 = x1;
		x1 = tmp;
	}

	if (y1 > y2)
	{
		int tmp = y2;
		y2 = y1;
		y1 = tmp;
	}

	if ((x1 >= x2) || (y1 >= y2)) return;

// zadan� f�ze sprajtu (nen�-li obslou�ena pohybem)
	if ((sprite->Timer() <= 0) && 
		!sprite->Moving() && 
		!sprite->KlidMove() &&
		((DWORD)sprite->FazeNext() < (DWORD)sprite->Faze()))
	{
		sprite->FazeN(sprite->FazeNext());
		sprite->FazeNext(-1);
	}

// adresa aktivn�ho obr�zku, adresa dat obr�zku
	PICTUREDATA* pic = sprite->AktPicData();
	BYTE* src = pic->Data;

// p�ednastaven� ���ky a v��ky k zobrazen�
	int widthbyte = pic->Width;
	int width = widthbyte;
	int height = pic->Height;

// lev� okraj k zobrazen�
	int left = sprite->X();
	if (left < x1)
	{
		width -= x1 - left;
		src += x1 - left;
		left = x1;
	}

// prav� okraj k zobrazen�
	int right = left + width;
	if (right > x2)
	{
		width -= right - x2;
		right = x2;
	}

// kontrola ���ky k vykreslen�
	if (width <= 0) return;

// doln� okraj k zobrazen� (s nulou naho�e)
	int bottom = Height - sprite->Y();
	if (bottom > Height - y1)
	{
		height -= bottom - (Height - y1);
		src += (bottom - (Height - y1))*widthbyte;
		bottom = Height - y1;
	}

// horn� okraj k zobrazen� (nula naho�e)
	int top = bottom - height;
	if (top < Height - y2)
	{
		height -= Height - y2 - top;
		top = Height - y2;
	}

// kontrola v��ky k vykreslen�
	if (height <= 0) return;

// zaji�t�n� zmapov�n� obr�zku
	if (pic->Param == PicParamNone)
	{
		bool backcol = false;
		bool piccol = false;
		BYTE* data = pic->Data;

		for (int n = widthbyte * pic->Height; n > 0; n--)
		{
			if (*data == BackCol)
			{
				backcol = true;
				if (piccol) break;
			}
			else
			{
				if (*data == ShadCol)
				{
					backcol = true;
					piccol = true;
					break;
				}

				piccol = true;
				if (backcol) break;
			}
			data++;
		}

		int param = PicParamBack;
		if (piccol)
		{
			if (backcol)
			{
				param = PicParamMix;
			}
			else
			{
				param = PicParamPic;
			}
		}
		pic->Param = param;
	}

// c�lov� adresa k dek�dov�n� obr�zku
	BYTE* dst = PicBuf + left + (Height - bottom)*WidthByte;

// p��rustek adresy
	int srcinc;
	int dstinc;

	int height0 = height;

// rozli�en� typu obr�zku
	switch(pic->Param)
	{

// pouze obr�zek bez pozad�
	case PicParamPic:
		dstinc = WidthByte;
		for (; height > 0; height--)
		{
			MemCopy(dst, src, width);
			src += widthbyte;
			dst += dstinc;
		}
		break;

// mixovan� obr�zek s pozad�m
	case PicParamMix:
		dstinc = WidthByte - width;
		srcinc = widthbyte - width;

		for (; height > 0; height--)
		{
			for (int j = width; j > 0; j--)
			{
				BYTE c = *src;
				if (c != BackCol)
				{
					if (c == ShadCol) c = Shadows[*dst];
					*dst = c;
				}
				src++;
				dst++;
			}
			src += srcinc;
			dst += dstinc;
		}
		break;
	}

// stavov� indik�tor
	if (sprite->IsStatus() && (width >= 3) && (height0 >= 5))
	{
		int status0;

		if (sprite->Status() <= 0)
		{
			status0 = 0;
		}
		else
		{
			if (sprite->Status() >= 1)
			{
				status0 = width;
			}
			else
			{
				status0 = Round(width * sprite->Status());
			}
		}

		dst = PicBuf + left + (Height - bottom)*WidthByte;

		BYTE col = WhiteCol;

		MemFill(dst, width, col);
		dst += WidthByte;

		for (height = height0 - 4; height > 0; height--)
		{
			*dst = col;
			dst[width-1] = col;
			dst += WidthByte;
		}

		if (sprite->Status() >= 0.5)
		{
			col = GreenCol;
		}
		else
		{
			if (sprite->Status() >= 0.25)
			{
				col = YellowCol;
			}
			else
			{
				col = RedCol;
			}
		}

		for (height = 3; height > 0; height--)
		{
			MemFill(dst, status0, col);
			MemFill(dst + status0, width - status0, BlackCol);
			dst += WidthByte;
		}
	}

// aktualizace plochy k p�ekreslen�
	if (left < AktLeft) AktLeft = left;
	if (right > AktRight) AktRight = right;
	if (top < AktTop) AktTop = top;
	if (bottom > AktBottom) AktBottom = bottom;
}


/////////////////////////////////////////////////////////////////////////////
// zaji�t�n� vymaz�n� sprajtu (po�adavek p�ekreslen� ikon)

void _fastcall SpriteClear(int index)
{
// adresa sprajtu
	CSprite* sprite = &Sprite[index];

// jen pokud je sprajt viditeln�
	if (sprite->Visible())
	{
		AktIconBoxG(sprite->X(), sprite->Y(), sprite->X() + sprite->Width(), 
							sprite->Y() + sprite->Height());
	}
}


/////////////////////////////////////////////////////////////////////////////
// po�adavek k p�ekreslen� sprajtu

void _fastcall AktSprite(int index)
{
// adresa sprajtu
	CSprite* sprite = &Sprite[index];

// jen nen�-li je�t� zapnut po�adavek p�ekreslen� sprajtu
	if (!sprite->DispReq())
	{

// zapnut� po�adavku o p�ekreslen� sprajtu
		sprite->DispReq(true);

// hladina p�id�van�ho sprajtu
		int level = sprite->Level();
		double y = sprite->AktY();

// nalezen� pozice k vlo�en� do tabulky (dole vy��� hladina a ni��� sou�adnice Y)
		int i = SpriteDispLevel.Num();
		int* data = SpriteDispLevel.Data() + i - 1;
		double* data2 = SpriteDispY.Data() + i - 1;

		for (; i > 0; i--)
		{
			if (*data > level) break;
			if (*data == level)
			{
				if (*data2 <= y) break;
			}
			data--;
			data2--;
		}

// za�azen� sprajtu do tabulky
		SpriteDispLevel.Insert(i, level);
		SpriteDispY.Insert(i, y);
		SpriteDispReq.Insert(i, index);
	}
}


/////////////////////////////////////////////////////////////////////////////
// ukon�en� animace v klidu

void _fastcall SpriteKlidMoveEnd(int index)
{
// adresa sprajtu
	CSprite* sprite = &Sprite[index];

// jen je-li animace v klidu
	if (sprite->KlidMove())
	{

// zru�en� p��znaku pohybu
		sprite->KlidMove(false);

// vypu�t�n� sprajtu z tabulky
		int i = SpriteKlid.Num()-1;
		int* data = SpriteKlid.Data() + i;
		for (; i >= 0; i--)
		{
			if (*data == index) SpriteKlid.DelItem(i);
			data--;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// zah�jen� animace v klidu

void _fastcall SpriteKlidMoveBeg(int index)
{
// adresa sprajtu
	CSprite* sprite = &Sprite[index];

// jen nen�-li animace v klidu a je-li v�ce f�z� klidu
	if (!sprite->KlidMove() && (sprite->Klid() > 1))
	{

// nastaven� p��znaku pohybu
		sprite->KlidMove(true);

// za�azen� do tabulky pohybu v klidu
		SpriteKlid.Add(index);

// �as do dal�� animace
		if (sprite->Timer() <= 0)
		{
			sprite->Timer((sprite->Delay() + 22)/55);
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace zobrazen� sprajt� s kladnou hladinou (zobrazuje se v po�ad� shora dol� - dole vy��� hladina)

void AktDispSprite()
{
	for (int i = SpriteDispReq.Num() - 1; i >= 0; i--)
	{
		int index = SpriteDispReq[i];
		if (Sprite[index].Level() >= 0)
		{
			Sprite[index].DispReq(false);
			SpritePaint(index);
		}
	}
	SpriteDispReq.Del(SpriteDispReq.Num());
	SpriteDispLevel.Del(SpriteDispLevel.Num());
	SpriteDispY.Del(SpriteDispY.Num());
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace zobrazen� sprajt� se z�pornou hladinou (zobrazuje se v po�ad� shora dol� - dole vy��� hladina)

void AktDispSpriteNeg()
{
	for (int i = SpriteDispReq.Num() - 1; i >= 0; i--)
	{
		int index = SpriteDispReq[i];
		if (Sprite[index].Level() < 0)
		{
			Sprite[index].DispReq(false);
			SpritePaint(index);
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// zah�jen� p�esunu sprajtu na pozici

void SpriteMove(int index, double cilx, double cily)
{
// adresa sprajtu
	CSprite* sprite = &Sprite[index];

// nastaven� nov� c�lov� pozice sprajtu
	sprite->CilX(cilx);
	sprite->CilY(cily);

// p�epo�et sou�adnic X a Y na body
	int xN = Round(cilx*ICONWIDTH);
	int yN = Round(cily*ICONHEIGHT);

// sprajt nen� viditeln� - p�esun se d�l� skokem
	if (!sprite->Visible())
	{
		sprite->AktX(cilx);
		sprite->AktY(cily);
		sprite->X(xN);
		sprite->Y(yN);
		return;
	}

// bodov� sou�adnice nezm�n�ny - p�esun se ned�l� (rychl� ukon�en� pohybu)
	if ((xN == sprite->X()) && (yN == sprite->Y()))
	{
		SpriteClear(index);	// vymaz�n� kv�li spr�vn�mu kreslen� st�nu

		sprite->AktX(cilx);
		sprite->AktY(cily);

		if (sprite->Moving())
		{
			sprite->KrokCit(-1);
			sprite->Moving(false);

			for (int j = SpriteMoving.Num()-1; j >= 0; j--)
			{
				if (SpriteMoving[j] == index)
				{
					SpriteMoving.DelItem(j);
					break;
				}
			}
		}

		if (sprite->Klid() == 1) sprite->FazeN(0);

		AktSprite(index);
		SpriteKlidMoveBeg(index);
		return;
	}

// ukon�en� animace v klidu
	SpriteKlidMoveEnd(index);

// v�po�et �hlov�ho sm�ru pohybu
	double dx = cilx - sprite->AktX();		// rozd�l sou�adnic X v pol��k�ch
	double dy = cily - sprite->AktY();		// rozd�l sou�adnic Y v pol��k�ch
	double smer = atan2(dy, dx);			// �hel sm�ru

// v�po�et indexu sm�ru pohybu
	double dsmer = pi2/sprite->Smer();		// p��rustek �hlu sm�ru
	int smerN = Round(smer/dsmer);			// index sm�ru
	while (smerN >= sprite->Smer()) smerN -= sprite->Smer();
	while (smerN < 0) smerN += sprite->Smer();
	sprite->SmerN(smerN);					// nastaven� indexu sm�ru
	sprite->SmerR(dsmer*smerN);				// nastaven� �hlu sm�ru

// ulo�en� sprajtu do tabulky pohybuj�c�ch se sprajt�
	if (!sprite->Moving())
	{
		sprite->Moving(true);
		sprite->Timer(1);
		SpriteMoving.Add(index);
	}

// nastaven� f�ze animace (aby byla hned prvn� f�ze pohybu)
	if ((sprite->FazeN() < sprite->Klid()) &&
		(sprite->Faze() > sprite->Klid()))
	{
		sprite->FazeN(sprite->Klid()-1);
	}

// po�et krok� na urazen� vzd�lenosti
	int krokcit = Round(sqrt(dx*dx + dy*dy) * sprite->Kroku());
	sprite->KrokCit(krokcit);

// p�esun na novou pozici ihned
	if (krokcit == 0)
	{
		krokcit++;

		if (sprite->Kroku() == 0)
		{
			SpriteClear(index);
			sprite->KrokCit(-1);
			sprite->AktX(cilx);
			sprite->AktY(cily);
			sprite->X(xN);
			sprite->Y(yN);
			sprite->Moving(false);

			for (int k = SpriteMoving.Num()-1; k >= 0; k--)
			{
				if (SpriteMoving[k] == index)
				{
					SpriteMoving.DelItem(k);
					break;
				}
			}

			AktSprite(index);
			SpriteKlidMoveBeg(index);
			return;
		}
	}

// p��rustek sou�adnic
	sprite->dX(dx/krokcit);
	sprite->dY(dy/krokcit);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� sm�ru sprajtu

void SetSpriteSmer(int index, double smer)
{
// adresa sprajtu
	CSprite* sprite = &Sprite[index];

// lok�ln� prom�nn�
	int smernum = sprite->Smer();		// po�et sm�r�
	double dsmer = pi2/smernum;			// p��rustek sm�ru

// v�po�et indexu sm�ru
	int smerN = Round(smer/dsmer);
	while (smerN >= smernum) smerN -= smernum;
	while (smerN < 0) smerN += smernum;

// nastaven� nov�ho sm�ru (pokud se zm�nil)
	if (sprite->SmerN() != smerN)
	{
		sprite->SmerN(smerN);
		sprite->SmerR(dsmer*smerN);
		SpriteClear(index);		// vymaz�n� a p�ekreslen� sprajtu
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� viditelnosti sprajtu

void SetSpriteVisible(int index, bool visible)
{
// adresa sprajtu
	CSprite* sprite = &Sprite[index];

// kontrola, zda je zm�na
	if (sprite->Visible() == visible) return;

// zapnut� viditelnosti
	if (visible)
	{
		sprite->Visible(true);
		SpriteVisible.Add(index);
		SpriteKlidMoveBeg(index);
		SpriteClear(index);		// mus� se p�ekreslit i sprajty nad/pod
	}
	else

// vypnut� viditelnosti
	{
		SpriteClear(index);		// vymaz�n� - je�t� mus� b�t viditeln�!
		sprite->Visible(false);

// zru�en� z bufferu viditeln�ch sprajt�
		for (int i = SpriteVisible.Num()-1; i >= 0; i--)
		{
			if (SpriteVisible[i] == index)
			{
				SpriteVisible.DelItem(i);
				break;
			}
		}

// ukon�en� animace v klidu
		SpriteKlidMoveEnd(index);

// zrychlen� dokon�en� pohybu
		if (sprite->Moving())
		{
			sprite->KrokCit(-1);
			sprite->AktX(sprite->CilX());
			sprite->X(Round(sprite->AktX()*ICONWIDTH));
			sprite->AktY(sprite->CilY());
			sprite->Y(Round(sprite->AktY()*ICONHEIGHT));
			sprite->Moving(false);

			for (int j = SpriteMoving.Num()-1; j >= 0; j--)
			{
				if (SpriteMoving[j] == index)
				{
					SpriteMoving.DelItem(j);
					break;
				}
			}
		}

// korekce f�ze na klidovou
		while (sprite->FazeN() >= sprite->Klid())
		{
			sprite->FazeN(sprite->FazeN() - sprite->Klid());
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� sprajtu

void SetSprite(int inx, const CSprite& src)
{
	CSprite* dst = &Sprite[inx];

	if (src != *dst)
	{
		if (dst->Visible())
		{
			if ((dst->Klid() > 1) && (src.Klid() == 1))
			{
				SpriteKlidMoveEnd(inx);
			}

			SpriteClear(inx);
		}

		if (dst->FazeN() >= dst->Klid())
		{
			dst->FazeN(dst->FazeN() - dst->Klid() + src.Klid());

			if (dst->FazeN() >= src.Faze())
			{
				dst->FazeN(src.Klid());
			}
		}
		else
		{
			if (dst->FazeN() >= src.Klid())
			{
				dst->FazeN(0);
			}
		}

		*dst = src;

		if (dst->FazeN() >= dst->Faze())
		{
			dst->FazeN(0);
		}

		SetSpriteSmer(inx, dst->SmerR());

		if (dst->Visible())
		{
			SpriteKlidMoveBeg(inx);
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// aktualiza�n� p�ekreslen� jedn� ikony

void AktDispIcon0(int x, int y)
{	
// adresa ikony
	CIcon* ico = &(Map[0].At(x, y).Icon);
	ico->DeComp();
	ICONDATA* icon = ico->Data();

// adresa v bufferech
	int off = x*ICONWIDTH + y*RowByte;
	BYTE* dst = PicBuf + off;
	BYTE* src = icon->Data;

	int dstinc = WidthByte;			// p��rustek c�lov� adresy
	int i = ICONHEIGHT;

// rozli�en� typu ikony
	switch(icon->Param)
	{

// pouze obr�zek bez pozad�
	case PicParamPic:
#ifdef _M_IX86
		dstinc -= ICONWIDTH;

		_asm {
			push	esi
			push	edi

			mov		ecx,ICONHEIGHT
			mov		edx,dstinc
			mov		esi,src
			mov		edi,dst

X1:			movs	dword ptr [edi],[esi]
			movs	dword ptr [edi],[esi]
			movs	dword ptr [edi],[esi]
			movs	dword ptr [edi],[esi]
			movs	dword ptr [edi],[esi]
			movs	dword ptr [edi],[esi]
			movs	dword ptr [edi],[esi]
			movs	dword ptr [edi],[esi]

			add		edi,edx
			loop	X1

			pop		edi
			pop		esi
		}
#else
		for (; i > 0; i--)
		{
			MemCopy(dst, src, ICONWIDTH);
			src += ICONWIDTH;
			dst += dstinc;
		}
#endif
		break;

// mixovan� obr�zek s pozad�m
	case PicParamMix:
		dstinc -= ICONWIDTH;

#ifdef _M_IX86
		_asm {

			push	esi
			push	edi
			push	ebx

			mov		ebx,Shadows
			mov		edx,ICONHEIGHT
			mov		esi,src
			mov		edi,dst
			xor		ecx,ecx

X2:			mov		cl,ICONWIDTH/2
X3:			lods	word ptr [esi]
			cmp		al,1
			jb		X41
			jne		X40
			mov		al,[edi]
			xlat	byte ptr [ebx]
X40:		mov		[edi],al
X41:		inc		edi
			cmp		ah,1
			jb		X43
			jne		X42
			mov		al,[edi]
			xlat	byte ptr [ebx]
			mov		ah,al
X42:		mov		[edi],ah
X43:		inc		edi
			loop	X3
			add		edi,dstinc
			dec		edx
			jnz		X2

			pop		ebx
			pop		edi
			pop		esi
		}
#else
		for (; i > 0; i--)
		{
			for (int j = ICONWIDTH; j > 0; j--)
			{
				BYTE c = *src;
				if (c != BackCol)
				{
					if (c == ShadCol) c = Shadows[*dst];
					*dst = c;
				}
				src++;
				dst++;
//				back++;
			}
			dst += dstinc;
//			back += dstinc;
		}
#endif
		break;

// pouze pozad�
	case PicParamBack:
		break;

// nezn�m� obsah (zajist� zmapov�n�)
	default:
		{
			bool backcol = false;
			bool piccol = false;
			dstinc -= ICONWIDTH;

			for (; i > 0; i--)
			{
				for (int j = ICONWIDTH; j > 0; j--)
				{
					if (*src == BackCol)
					{
						backcol = true;
					}
					else
					{
						if (*src == ShadCol)
						{
							backcol = true;
							*dst = Shadows[*dst];
						}
						else
						{
							*dst = *src;
						}
						piccol = true;
					}
					src++;
					dst++;
				}
				dst += dstinc;
			}

			int param = PicParamBack;
			if (piccol)
			{
				if (backcol)
				{
					param = PicParamMix;
				}
				else
				{
					param = PicParamPic;
				}
			}
			icon->Param = param;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// aktualiza�n� p�ekreslen� jednoho pozad�

void AktDispBack0(int x, int y)
{	
// okraje k zobrazen� pozad�
	int left = x*ICONWIDTH;

// adresa v bufferech
	int off = left + y*RowByte;
	BYTE* back = BackBuf + off;
	BYTE* dst = PicBuf + off;

	int dstinc = WidthByte;			// p��rustek c�lov� adresy

// adresa ikony
	CIcon* ico = &(Map[0].At(x, y).Icon);
	ico->DeComp();
	ICONDATA* icon = ico->Data();

// vykreslen� pozad� (krom� pln� ikony)
	if (icon->Param != PicParamPic)
	{
#ifdef _M_IX86
		dstinc -= ICONWIDTH;

		_asm {
			push	esi
			push	edi

			mov		ecx,ICONHEIGHT
			mov		edx,dstinc
			mov		esi,back
			mov		edi,dst

X5:			movs	dword ptr [edi],[esi]
			movs	dword ptr [edi],[esi]
			movs	dword ptr [edi],[esi]
			movs	dword ptr [edi],[esi]
			movs	dword ptr [edi],[esi]
			movs	dword ptr [edi],[esi]
			movs	dword ptr [edi],[esi]
			movs	dword ptr [edi],[esi]

			add		edi,edx
			add		esi,edx
			loop	X5

			pop		edi
			pop		esi
		}
#else
		for (int i = ICONHEIGHT; i > 0; i--)
		{
			MemCopy(dst, back, ICONWIDTH);
			back += dstinc;
			dst += dstinc;
		}
#endif
	}
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace p�ekreslen� ikon

void AktDispIcon()
{
	bool* buf = AktBuf + WidthN*HeightN - 1;

	for (int y = HeightN-1; y >= 0; y--)
	{
		for (int x = WidthN-1; x >= 0; x--)
		{
			if (*buf)
			{
				*buf = false;
				AktDispIcon0(x, y);
			}
			buf--;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace p�ekreslen� pozad�

void AktDispBack()
{
	bool* buf = AktBuf + WidthN*HeightN - 1;

	for (int y = HeightN-1; y >= 0; y--)
	{
		for (int x = WidthN-1; x >= 0; x--)
		{
			if (*buf)
			{
				AktDispBack0(x, y);
			}
			buf--;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace p��znak� p�ekreslov�n� (pro p�ekreslen� st�n� sprajt�)

void AktAktDisp0(int x, int y)
{
// okraje k zobrazen�
	int left = x*ICONWIDTH;
	int bottom = Height - y*ICONHEIGHT;

// aktualizace plochy k p�ekreslen�
	if (left < AktLeft) AktLeft = left;
	int right = left + ICONWIDTH;
	if (right > AktRight) AktRight = right;

	if (bottom > AktBottom) AktBottom = bottom;
	int top = bottom - ICONHEIGHT;
	if (top < AktTop) AktTop = top;

// kontrola, zda je pot�ebn� p�ekreslen� sprajt�
	for (int i = SpriteVisible.Num()-1; i >= 0; i--)
	{
		int index = SpriteVisible[i];		// index sprajtu
		CSprite* sprite = &Sprite[index];		// adresa sprajtu

		if ((right > sprite->X()) &&
			(left < sprite->X() + sprite->Width()) &&
			(Height - top > sprite->Y()) &&
			(Height - bottom < sprite->Y() + sprite->Height()))
		{

// po�adavek k p�ekreslen� sprajtu, nen�-li je�t� nastaven
			if (!sprite->DispReq())
			{
				AktSprite(index);

// po�adavek k p�ekreslen� ikon pod sprajtem
				int x1 = sprite->X()/ICONWIDTH;
				if (x1 < 0) x1 = 0;

				int x2 = (sprite->X() + sprite->Width())/ICONWIDTH + 1;
				if (x2 > WidthN) x2 = WidthN;

				if (x2 > x1)
				{
					int y1 = sprite->Y()/ICONHEIGHT;
					if (y1 < 0) y1 = 0;

					int y2 = (sprite->Y() + sprite->Height())/ICONHEIGHT + 1;
					if (y2 > HeightN) y2 = HeightN;

					for (int y0 = y1; y0 < y2; y0++)
					{
						for (int x0 = x1; x0 < x2; x0++)
						{
							bool* buf = AktBuf + WidthN*y0 + x0;

							if (!*buf)
							{
								*buf = true;
								AktAktDisp0(x0, y0);
							}
						}
					}
				}
			}
		}
	}
}


void AktAktDisp()
{
	bool* buf = AktBuf + WidthN*HeightN - 1;

	for (int y = HeightN-1; y >= 0; y--)
	{
		for (int x = WidthN-1; x >= 0; x--)
		{
			if (*buf)
			{
				AktAktDisp0(x, y);
			}
			buf--;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualiza�n� p�ekreslen� okna

// lok�ln� prom�nn� pro p�ekr�van� zobrazen�
HDC	RDdc = NULL;							// DC v�stupn�ho za��zen�
BYTE* RDsrcbuf = NULL;						// adresa bufferu s daty
int RDsrcleft = 0;							// sou�adnice X dat v bufferu
int RDsrctop = 0;							// sou�adnice Y dat v bufferu
int RDsrcheight = 0;						// v��ka dat v bufferu
int RDwidthbyte = 0;						// d�lka linky bufferu v bajtech
int RDwinitem = 0;							// ukazatel indexu polo�ky

// zobrazen� bez filtrace
void RDdisp(int left, int top, int width, int height)
{
	if ((width > 0) && (height > 0))
	{
		if (DispBufUse2)
		{
			BITMAPINFO bi;
			MemFill(&bi, sizeof(BITMAPINFO), 0);

			bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bi.bmiHeader.biWidth = StdBitmapInfo->bmiHeader.biWidth;
			bi.bmiHeader.biHeight = height;
			bi.bmiHeader.biPlanes = 1;
			bi.bmiHeader.biBitCount = 32;
			bi.bmiHeader.biCompression = BI_RGB;

			::StretchDIBits(RDdc,
				left, top, width, height,				// c�lov� parametry
				left - RDsrcleft, 0, width, height,		// zdrojov� parametry
				RDsrcbuf + RDwidthbyte*(RDsrcheight - 
						((top - RDsrctop) + height)),	// adresa dat v bufferu
				&bi, DIB_RGB_COLORS, SRCCOPY);
		}
		else
		{
			StdBitmapInfo->bmiHeader.biHeight = height; // v��ka plochy v bodech

			::StretchDIBits(RDdc,
				left, top, width, height,				// c�lov� parametry
				left - RDsrcleft, 0, width, height,		// zdrojov� parametry
				RDsrcbuf + RDwidthbyte*(RDsrcheight - 
						((top - RDsrctop) + height)),	// adresa dat v bufferu
				StdBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
		}
	}
}

// zobrazen� s filtrac� p�es dialogov� prvky
void RDdisp2(int left, int top, int width, int height)
{
	if (!DialogMode)
	{
		RDdisp(left, top, width, height);
		return;
	}

	if ((width > 0) && (height > 0))
	{
		RDwinitem++;

		if (RDwinitem >= Win.Max())
		{
			RDdisp(left, top, width, height);
		}
		else
		{
			WINITEM* item = Win.Data() + RDwinitem;

			if (Win.IsValid(RDwinitem) && (item->Parent == 0) && (item->Typ != WINITEM_GROUP) && item->Show)
			{
				int left2 = item->Left;
				int top2 = item->Top;
				int width2 = item->Width;
				int height2 = item->Height;
				int right2 = left2 + width2;
				int bottom2 = top2 + height2;

				int typ2 = item->Typ;

				if ((typ2 == WINITEM_COMBOBOX) || 
					(typ2 == WINITEM_COMBOEDIT) ||
					(typ2 == WINITEM_WINDOW) || 
					(typ2 == WINITEM_WINDOWRO))
				{
					RECT rc;
					::GetWindowRect(item->Wnd, &rc);
					HWND w = Win[item->Parent].Wnd;
					::ScreenToClient(w, (POINT*)&(rc.left));
					::ScreenToClient(w, (POINT*)&(rc.right));

					left2 = rc.left;
					top2 = rc.top;
					right2 = rc.right;
					bottom2 = rc.bottom;
					width2 = right2 - left2;
					height2 = bottom2 - top2;
				}

				int right = left + width;
				int bottom = top + height;

// nen� p�ekryv
				if ((right <= left2) ||
					(left >= right2) ||
					(top >= bottom2) ||
					(bottom <= top2))
				{
					RDdisp2(left, top, width, height);
				}
				else
				{

// je p�ekryv
					RDdisp2(left, top, width, top2 - top);			// horn� p�s
					RDdisp2(left, bottom2, width, bottom - bottom2);// doln� p�s

					if (top2 > top) top = top2;
					if (bottom2 < bottom) bottom = bottom2;
					height = bottom - top;

					RDdisp2(left, top, left2 - left, height);		// lev� p�s
					RDdisp2(right2, top, right - right2, height);	// prav� p�s			
				}
			}
			else

// nen� platn� prvek, dal�� prvek
			{
				RDdisp2(left, top, width, height);
			}
		}

		RDwinitem--;
	}
}

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

// zobrazen� s filtrac� p�es 3D okno
void RDdisp3(int left, int top, int width, int height)
{
	if ((width > 0) && (height > 0))
	{
		int right = left + width;
		int bottom = top + height;
		int right2 = D3DLeft + D3DWidth;
		int bottom2 = D3DTop + D3DHeight;

		if ((right <= D3DLeft) ||
			(left >= right2) ||
			(top >= bottom2) ||
			(bottom <= D3DTop))
		{
			RDdisp2(left, top, width, height);
		}
		else
		{
			RDdisp2(left, top, width, D3DTop - top);	// horn� p�s
			RDdisp2(left, bottom2, width, bottom - bottom2); // doln� p�s

			if (D3DTop > top) top = D3DTop;
			if (bottom2 < bottom) bottom = bottom2;
			height = bottom - top;

			RDdisp2(left, top, D3DLeft - left, height);	// lev� p�s
			RDdisp2(right2, top, right - right2, height); // prav� p�s
		}
	}
}

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------

void ReDisp()
{
// obsluha konzoly
	if (ConsoleOn)
	{
		if (CaptionAkt)
		{
			CaptionAkt = false;
			int crlf = CaptionText.Find(13);
			if (crlf >= 0) CaptionText.Delete(crlf);
			SetCaptionText(CaptionText);
		}
	
		ReDispOut = REDISPOUT;
		return;
	}

// zapnut� hlavn�ho okna
	if (!MainFrameVisible)
	{
		MainFrameShow();
		return;
	}

// aktualiza�n� vykreslen� ikon a sprajt�
	if (!DialogMode || DialogGraph)
	{
		AktAktDisp();
		AktDispBack();
		AktDispSpriteNeg();
		AktDispIcon();
		AktDispSprite();
	}

// zm�na textu nadpisu okna
	if (CaptionAkt)
	{
		CaptionAkt = false;
		int crlf = CaptionText.Find(13);
		if (crlf >= 0) CaptionText.Delete(crlf);
		SetCaptionText(CaptionText);
	}

// zm�na textu stavov�ho ��dku
	if (StatusAkt)
	{
		StatusAkt = false;
		int crlf = StatusText.Find(13);
		if (crlf >= 0) StatusText.Delete(crlf);
		if (StatusText.IsEmpty() || FullScreen) SetStatusVisible(false);
		SetStatusText(StatusText);
		if (StatusText.IsNotEmpty() && !FullScreen) SetStatusVisible(true);
	}

#ifndef _MINI
	if (FullScreen && D3D && !D3D2D)
	{
		ReDispOut = REDISPOUT;
		return;
	}
#endif // _MINI

// kontrola, zda je pot�eba prov�d�t obsluhu p�ekreslen�
	if ((AktLeft < AktRight) &&
		(AktTop < AktBottom) &&
		(!DialogMode || DialogGraph)
#ifndef _MINI
		&& (!D3D || D3D2D)
#endif // _MINI
		)
	{
		HPALETTE OldPal = NULL;

// ���ka a v��ka �seku obr�zku k vykreslen�
		int width = AktRight - AktLeft;
		int height = AktBottom - AktTop;

// otev�en� DC okna
		RDdc = ::GetDC(MainFrame);

// nastaven� vlastn�ch palet
#ifndef _MINI
		if (!D3D || (D3DIntAkt <= 2))
#endif // _MINI
		{
			OldPal = ::SelectPalette(RDdc, StdPalette, FALSE);
			::RealizePalette(RDdc);
		}

// nastaven� STRETCH m�du
		::SetStretchBltMode(RDdc, COLORONCOLOR);

// p��prava parametr�, nen�-li v�stupn� buffer
		int leftdst0 = AktLeft;
		int topdst0 = AktTop;
		int widthsrc = Width;
		int widthdst = width;
		int heightdst = height;

		RDsrcbuf = PicBuf;
		RDsrcleft = DispLeft;
		RDsrctop = DispTop;
		RDsrcheight = Height;
		RDwidthbyte = Width;

// bude vykreslov�n� pomoc� v�stupn�ho bufferu
		if (DispBufUse)
		{

// p��prava ukazatel�
			leftdst0 = 0;
			if (AktLeft > 0)
			{
				leftdst0 = DispMapX[AktLeft-1];
			}

			topdst0 = 0;
			if (AktTop > 0)
			{
				topdst0 = DispMapY[AktTop-1];
			}

			if (AktRight >= (Width - 1))
			{
				widthdst = DispWidth;
			}
			else
			{
				widthdst = DispMapX[AktRight+1];
			}
			widthdst -= leftdst0;

			if (AktBottom >= (Height - 1))
			{
				heightdst = DispHeight;
			}
			else
			{
				heightdst = DispMapY[AktBottom+1];
			}
			heightdst -= topdst0;

// cyklus p�es v�echny linky
			if ((widthdst > 0) && (heightdst > 0))
			{
				if (DispBufUse2)
				{
					RDwidthbyte = (widthdst << 2);
				}
				else
				{
					RDwidthbyte = (widthdst + 3) & ~3;
				}

				BYTE* dst0 = DispBuf + (heightdst-1)*RDwidthbyte;
				int topdst = topdst0;

				for (int i = 0; i < heightdst; i++)
				{
					BYTE* dst = dst0;
					BYTE* src = PicBuf + DispRemapY[topdst];
					int leftdst = leftdst0;

// cyklus p�es body na lince
					if (DispBufUse2)
					{
						for (int j = widthdst; j > 0; j--)
						{
							int srcadr = DispRemapX[leftdst];

							RGBQUAD* rgb = StdBitmapInfo->bmiColors + src[srcadr];

							int r = rgb->rgbRed;
							int g = rgb->rgbGreen;
							int b = rgb->rgbBlue;

							rgb = StdBitmapInfo->bmiColors + src[srcadr + 1];

							r += rgb->rgbRed;
							g += rgb->rgbGreen;
							b += rgb->rgbBlue;

							rgb = StdBitmapInfo->bmiColors + src[srcadr - Width];

							r += rgb->rgbRed;
							g += rgb->rgbGreen;
							b += rgb->rgbBlue;

							rgb = StdBitmapInfo->bmiColors + src[srcadr - Width + 1];

							r += rgb->rgbRed;
							g += rgb->rgbGreen;
							b += rgb->rgbBlue;

							*dst = (BYTE)(b >> 2);
							dst++;

							*dst = (BYTE)(g >> 2);
							dst++;

							*dst = (BYTE)(r >> 2);
							dst++;

							*dst = 0;
							dst++;

							leftdst++;
						}
					}
					else
					{
						for (int j = widthdst; j > 0; j--)
						{
							*dst = src[DispRemapX[leftdst]];
							dst++;
							leftdst++;
						}
					}

					dst0 -= RDwidthbyte;
					topdst++;
				}
			}

			widthsrc = widthdst;
			RDsrcbuf = DispBuf;
			RDsrcleft = DispLeft+leftdst0;
			RDsrctop = DispTop+topdst0;
			RDsrcheight = heightdst;
		}

// vykreslen� plochy
		StdBitmapInfo->bmiHeader.biWidth = widthsrc; // ���ka plochy v bodech
		leftdst0 += DispLeft;
		topdst0 += DispTop;

		RDwinitem = 0;							// adresa hlavn�ho okna

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

		if (D3D)
		{
			RDdisp3(leftdst0, topdst0, widthdst, heightdst);
		}
		else

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------

		{
			RDdisp2(leftdst0, topdst0, widthdst, heightdst);
		}

// n�vrat p�vodn�ch palet
		if (RDdc != NULL)
		{
			if (OldPal != NULL)
			{
				::SelectPalette(RDdc, OldPal, FALSE);
			}

			::ReleaseDC(MainFrame, RDdc);
		}

// zneplatn�n� aktualiza�n� oblasti
		AktLeft = Width;
		AktTop = Height;
		AktRight = 0;
		AktBottom = 0;
	}

// ��ta� pro time-out p�ekreslen� displeje
	ReDispOut = REDISPOUT;
}


/////////////////////////////////////////////////////////////////////////////
// p�ekreslen� cel�ho okna (po zpr�v� WM_PAINT)

void _fastcall OnPaint(WINITEM* item, int inx)
{
// potvrzen� p�ekreslen� okna
	PAINTSTRUCT ps;
	::BeginPaint(item->Wnd, &ps);
	::EndPaint(item->Wnd, &ps);

// p�ekreslen� obsahu okna
	RePaint(item);

// v dialogov�m m�du p�ekreslen� r�me�k�
	if (DialogMode || (item != &Win[0]))
	{
		int i = Win.Max() - 1;
		WINITEM* itm = Win.Data() + i;

		for (; i >= 0; i--)
		{
			if (Win.IsValid(i) && (itm->Typ == WINITEM_GROUP) && (itm->Parent == inx))
			{
				HWND wnd = itm->Wnd;
				::InvalidateRect(wnd, NULL, FALSE);
				::UpdateWindow(wnd);
			}
			itm--;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// p�ekreslen� cel�ho okna

HDC RPdc = NULL;						// DC v�stupn�ho za��zen�
HBRUSH RPbrush = NULL;					// �t�tec k vymaz�n� plochy
int RPwinitem = 0;						// ukazatel indexu polo�ky

// vymaz�n� bez filtrace
void RPfill(const RECT& rc)
{
	if ((rc.right > rc.left) && (rc.bottom > rc.top))
	{
		::FillRect(RPdc, &rc, RPbrush);
	}
}

// vymaz�n� s filtrac� p�es dialogov� prvky
void RPfill2(const RECT& rc)
{
	if (!DialogMode)
	{
		RPfill(rc);
	}

	if ((rc.right > rc.left) && (rc.bottom > rc.top))
	{
 		RPwinitem++;

		if (RPwinitem >= Win.Max())
		{
			RPfill(rc);
		}
		else
		{
			WINITEM* item = Win.Data() + RPwinitem;

			if (Win.IsValid(RPwinitem) && (item->Parent == 0) && (item->Typ != WINITEM_GROUP) && item->Show)
			{
				RECT rc2;
				rc2.left = item->Left;
				rc2.top = item->Top;
				rc2.right = rc2.left + item->Width;
				rc2.bottom = rc2.top + item->Height;

				int typ2 = item->Typ;
				if ((typ2 == WINITEM_COMBOBOX) || 
					(typ2 == WINITEM_COMBOEDIT) ||
					(typ2 == WINITEM_WINDOW) || 
					(typ2 == WINITEM_WINDOWRO))
				{
					::GetWindowRect(item->Wnd, &rc2);
					HWND w = Win[item->Parent].Wnd;
					::ScreenToClient(w, (POINT*)&(rc2.left));
					::ScreenToClient(w, (POINT*)&(rc2.right));
				}

// nen� p�ekryv
				if ((rc.right <= rc2.left) ||
					(rc.left >= rc2.right) ||
					(rc.top >= rc2.bottom) ||
					(rc.bottom <= rc2.top))
				{
					RPfill2(rc);
				}
				else
				{

// je p�ekryv
					RECT rc3 = rc;
					rc3.bottom = rc2.top;
					RPfill2(rc3);					// horn� p�s

					rc3.bottom = rc.bottom;
					rc3.top = rc2.bottom;
					RPfill2(rc3);					// doln� p�s

					rc3 = rc;
					if (rc2.top > rc.top)
					{
						rc3.top = rc2.top;
					}

					if (rc2.bottom < rc.bottom)
					{
						rc3.bottom = rc2.bottom;
					}

					rc3.right = rc2.left;
					RPfill2(rc3);					// lev� p�s

					rc3.right = rc.right;
					rc3.left = rc2.right;
					RPfill2(rc3);					// prav� p�s
				}
			}
			else

// nen� platn� prvek, dal�� prvek
			{
				RPfill2(rc);
			}
		}

		RPwinitem--;
	}
}

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

// zobrazen� s filtrac� p�es 3D okno
void RPfill3(const RECT& rc)
{
	if ((rc.right > rc.left) && (rc.bottom > rc.top))
	{
		int right2 = D3DLeft + D3DWidth;
		int bottom2 = D3DTop + D3DHeight;

		if ((rc.right <= D3DLeft) ||
			(rc.left >= right2) ||
			(rc.top >= bottom2) ||
			(rc.bottom <= D3DTop))
		{
			RPfill2(rc);
		}
		else
		{
			RECT rc3 = rc;
			rc3.bottom = D3DTop;
			RPfill2(rc3);								// horn� p�s

			rc3.bottom = rc.bottom;
			rc3.top = bottom2;
			RPfill2(rc3);								 // doln� p�s

			rc3 = rc;
			if (D3DTop > rc.top)
			{
				rc3.top = D3DTop;
			}

			if (bottom2 < rc.bottom)
			{
				rc3.bottom = bottom2;
			}

			rc3.right = D3DLeft;
			RPfill2(rc3);					// lev� p�s

			rc3.right = rc.right;
			rc3.left = right2;
			RPfill2(rc3);					// prav� p�s
		}
	}
}

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------

void _fastcall RePaint(WINITEM* item)
{
// test, zda je okno ji� zapnuto
	if ((item == &Win[0]) && !MainFrameVisible) return;

	if (IsOverlay && FullScreen) return;

#ifndef _MINI
	if (FullScreen && D3D && !D3D2D && !Resizing)
	{
//		FPUFloat();
//		pD3Disp();
//		FPUDouble();
//		DispAnimate();
		return;
	}
#endif // _MINI

// otev�en� DC displeje
	bool newbrush = true;
	RPdc = ::GetDC(item->Wnd);
		
	if ((item == &Win[0]) && FullScreen && (!DialogMode || DialogGraph))
	{
		RPbrush = (HBRUSH)::GetStockObject(BLACK_BRUSH); // �t�tec k vymaz�n� plochy
	}
	else
	{
		::SetTextColor(RPdc, item->FontCol);
		::SetBkColor(RPdc, item->FontBack);
		RPbrush = item->FontBrush;
		newbrush = false;

		if ((item->AlwaysTop &&
			!item->HasBorder &&
			!item->HasCaption))
		{
			RPbrush = (HBRUSH)::GetStockObject(BLACK_BRUSH); // �t�tec k vymaz�n� plochy
			newbrush = true;
		}
	}

// vymaz�n� plochy v dialogov�m m�du
	RECT rc;
	rc.left = 0;
	rc.right = item->ClientWidth;
	rc.top = 0;
	RPwinitem = 0;

	if ((DialogMode && !DialogGraph) || (item != &Win[0]))
	{
		rc.bottom = item->ClientHeight;

#ifndef _MINI
		if (D3D)
		{
			RPfill3(rc);
		}
		else
#endif // _MINI

		{
			RPfill2(rc);
		}
	}
	else
	{

// vymaz�n� plochy naho�e
		rc.bottom = DispTop;
#ifndef _MINI
		if (D3D)
		{
			RPfill3(rc);
		}
		else
#endif // _MINI

		{
			RPfill2(rc);
		}

// vymaz�n� plochy dole
		rc.top = DispTop + DispHeight;
		rc.bottom = ClientHeight;
#ifndef _MINI
		if (D3D)
		{
			RPfill3(rc);
		}
		else
#endif // _MINI

		{
			RPfill2(rc);
		}

// vymaz�n� plochy vlevo
		rc.bottom = rc.top;
		rc.top = 0;
		rc.right = DispLeft;
#ifndef _MINI
		if (D3D)
		{
			RPfill3(rc);
		}
		else
#endif // _MINI

		{
			RPfill2(rc);
		}

// vymaz�n� plochy vpravo
		rc.left = DispLeft + DispWidth;
		rc.right = ClientWidth;
#ifndef _MINI
		if (D3D)
		{
			RPfill3(rc);
		}
		else
#endif // _MINI

		{
			RPfill2(rc);
		}

// vymaz�n� podkladu r�me�k�
		if (DialogMode)
		{
			HBRUSH brsh = RPbrush;
			RPbrush = item->FontBrush;

			int i = Win.Max() - 1;
			WINITEM* itm = Win.Data() + i;

			for (; i >= 0; i--)
			{
				if (Win.IsValid(i) && (itm->Typ == WINITEM_GROUP) && (itm->Parent == 0))
				{
					rc.left = itm->Left;
					rc.top = itm->Top;
					rc.right = rc.left + itm->Width;
					rc.bottom = rc.top + itm->Height;
#ifndef _MINI
					if (D3D)
					{
						RPfill3(rc);
					}
					else
#endif // _MINI

					{
						RPfill2(rc);
					}
				}
				itm--;
			}

			RPbrush = brsh;
		}
	}

// zru�en� �t�tce podkladu (i kdy� podle dokumentace ru�en� nen� nutn�)
	if (newbrush) ::DeleteObject(RPbrush);

	::ReleaseDC(item->Wnd, RPdc);

// po�adavek k p�ekreslen� cel�ho okna
	AktLeft = 0;				// lev� okraj k p�ekreslen�
	AktTop = 0;					// horn� okraj k p�ekreslen�
	AktRight = Width;			// prav� okraj k p�ekreslen�
	AktBottom = Height;			// spodn� okraj k p�ekreslen�

// p�ekreslen� okna
	if (AktBuf != NULL)	ReDisp();		// p�ekreslen� okna
}


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� p��t� f�ze sprajtu (pro prom�nnou f�ze sprajtu)

int SpriteNextFaze(int index)
{
// adresa sprajtu
	CSprite* sprite = &Sprite[index];

// p��t� f�ze
	int faze = sprite->FazeNext();
	if ((DWORD)faze < (DWORD)sprite->Faze()) return faze;

// aktu�ln� f�ze
	faze = sprite->FazeN();

// test, zda je sprajt v pohybu
	if (sprite->Moving())
	{

// test, zda bude dokon�en pohyb
		if (sprite->KrokCit() <= 0)
		{

// dokon�en� pohybu
			if (faze >= sprite->Klid())
			{
				faze = faze - sprite->Klid();
			}
			faze = (faze+1) % sprite->Klid();
		}
		else

// pokra�ov�n� pohybu
		{
			faze = faze + 1;
			if (faze >= sprite->Faze())
			{
				faze = sprite->Klid();

				if (faze >= sprite->Faze())
				{
					faze = 0;
				}
			}
		}
	}
	else
	{

// obsluha pohybu sprajt� v klidu
		if (sprite->Klid() > 1)
		{
			faze = (faze + 1) % sprite->Klid();
		}
	}

	return faze;
}


/////////////////////////////////////////////////////////////////////////////
// obsluha �asova�e

void BackTimer()
{
// zneplatn�n� informac� o my�i
	MouseValid = false;

// zneplatn�n� informac� o joysticku
	for (int k = MAXJOYSTICK-1; k >= 0; k--)
	{
		JoystickValid[k] = false;
		JoystickNext[k]--;
	}

// obsluha p�ehr�v�n� zvuku na pozad�
	PlaySoundBack();

// obsluha hudby na pozad�
	PlayMusicBack();

// obsluha CD na pozad�
	PlayCDBack();

// uzav�en� soubor�
	if (FileCloseTime > 0)
	{
		FileCloseTime--;
		if (FileCloseTime == 0) FileClose();
	}

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

// ��ta� z�kazu DirectPlay
	if (NextDirectPlayInit > 0)	NextDirectPlayInit--;

// ��ta� platnosti IP adresy
	if (HostIPValid > 0) HostIPValid--;

// test, zda m��e b�t obsluha DirectPlay
	if (DirectPlay != NULL)
	{
		DirectPlayPlayersTime--;

// aktualizace parametr� hry DirectPlay
		if (DirectPlayGamesTime > 0)
		{
			DirectPlayGamesTime--;
		}

		if (DirectPlayGamesParamTime > 0)
		{
			DirectPlayGamesParamTime--;
		}
		else
		{
			GetGameDesc();
			DirectPlayGamesParamTime = 2*18;
		}
	}

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------

// v Pauze konec
	if (Pause) return;

// obsluha pohybu sprajt�
	for (int i = SpriteMoving.Num()-1; i >= 0; i--)
	{

// p��prava parametr� sprajtu
		int index = SpriteMoving[i];
		CSprite* sprite = &Sprite[index];
//		SPRITEDATA* spritedata = sprite->Data();

// ��t�n� �asu pro dal�� f�zi pohybu
		if (sprite->TimerDec())
		{

// zaji�t�n� vymaz�n� sprajtu na star� pozici
			SpriteClear(index);

// ��t�n� krok� pro dokon�en� pohybu
			if (sprite->KrokDec())
			{

// dokon�en� pohybu
				sprite->AktX(sprite->CilX());
				sprite->X(Round(sprite->AktX()*ICONWIDTH));
				sprite->AktY(sprite->CilY());
				sprite->Y(Round(sprite->AktY()*ICONHEIGHT));

				if ((DWORD)sprite->FazeNext() < (DWORD)sprite->Faze())
				{
					sprite->FazeN(sprite->FazeNext());
					sprite->FazeNext(-1);
				}
				else
				{
					if (sprite->FazeN() >= sprite->Klid())
					{
						sprite->FazeN(sprite->FazeN() - sprite->Klid());
					}
					sprite->FazeN((sprite->FazeN()+1) % sprite->Klid());
				}

				if (sprite->Klid() > 1) SpriteKlid.Add(index);

				SpriteMoving.DelItem(i);
				sprite->Moving(false);
			}
			else

// pokra�ov�n� pohybu
			{
				sprite->AktX(sprite->AktX() + sprite->dX());
				sprite->X(Round(sprite->AktX()*ICONWIDTH));
				sprite->AktY(sprite->AktY() + sprite->dY());
				sprite->Y(Round(sprite->AktY()*ICONHEIGHT));
				
				sprite->Timer((sprite->Delay() + 22)/55);

				if ((DWORD)sprite->FazeNext() < (DWORD)sprite->Faze())
				{
					sprite->FazeN(sprite->FazeNext());
					sprite->FazeNext(-1);
				}
				else
				{
					sprite->FazeN(sprite->FazeN() + 1);
					if (sprite->FazeN() >= sprite->Faze())
					{
						sprite->FazeN(sprite->Klid());

						if (sprite->FazeN() >= sprite->Faze())
						{
							sprite->FazeN(0);
						}
					}
				}
			}

// po�adavek k p�ekreslen� sprajtu
			AktSprite(index);
		}
	}

// obsluha pohybu sprajt� v klidu
	for (int j = SpriteKlid.Num()-1; j >= 0; j--)
	{

// p��prava parametr� sprajtu
		int index = SpriteKlid[j];
		CSprite* sprite = &Sprite[index];
//		SPRITEDATA* spritedata = sprite->Data();

// zah�jen� pohybu v klidu
		if (!sprite->KlidMove())
		{
			sprite->KlidMove(true);
			sprite->Timer((sprite->Delay() + 22)/55);
		}
		else
		{

// ��t�n� �asu pro dal�� f�zi pohybu
			if (sprite->TimerDec())
			{

// zaji�t�n� vymaz�n� sprajtu na pozici (a zaji�t�n� p�ekreslen�)
				SpriteClear(index);

// posun f�ze
				if ((DWORD)sprite->FazeNext() < (DWORD)sprite->Faze())
				{
					sprite->FazeN(sprite->FazeNext());
					sprite->FazeNext(-1);
				}
				else
				{
					sprite->FazeN((sprite->FazeN() + 1) % sprite->Klid());
				}
				sprite->Timer((sprite->Delay() + 22)/55);
			}
		}
	}

// ��t�n� time-out p�ekreslen� displeje
	ReDispOut--;
	if (ReDispOut <= 0) ReDisp();
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� p�esmyka�e kl�vesnice

void SetKeyState(int index, bool state)
{
	BYTE data[256];
	::GetKeyboardState(data);
	data[index] &= ~1;
	if (state) data[index] |= 1;
	::SetKeyboardState(data);
}


/////////////////////////////////////////////////////////////////////////////
// test �asova�e

BOOL TestTimer()
{
	MSG msg;
	return ::PeekMessage(&msg, NULL, WM_TIMER, WM_TIMER, PM_NOREMOVE);
}

/////////////////////////////////////////////////////////////////////////////
// na�ten� sou�adnic my�i

void GetAktMousePos()
{
	if (!MouseValid)
	{
		::GetCursorPos(&MouseScreen);
		MouseMain = MouseScreen;
		::ScreenToClient(Win[DialogParent].Wnd, &MouseMain);
		MouseValid = true;
	}

	if ((DialogMode && !DialogGraph) || (DialogParent != 0))
	{
		MouseX = (double)MouseMain.x / ICONWIDTH;
		MouseY = (double)(Win[DialogParent].ClientHeight - 1 - MouseMain.y) / ICONHEIGHT;
	}
	else
	{
		MouseX = (MouseMain.x - DispLeft) / Meritko / ICONWIDTH;
		MouseY = (DispHeight - 1 - (MouseMain.y - DispTop)) / Meritko / ICONHEIGHT;
	}
}

/////////////////////////////////////////////////////////////////////////////
// �ek�n� na sign�l od �asova�e s obsluhou zpr�v

void WaitTimer(bool wait)
{
// nastaven� p�esmyka�� kl�vesnice
	if (AktNumLock)
	{
		AktNumLock = false;
		SetKeyState(VK_NUMLOCK, NumLock);
	}

	if (AktCapsLock)
	{
		AktCapsLock = false;
		SetKeyState(VK_CAPITAL, CapsLock);
	}

	if (AktScrollLock)
	{
		AktScrollLock = false;
		SetKeyState(VK_SCROLL, ScrollLock);
	}

	if (AktInsertLock)
	{
		AktInsertLock = false;
		SetKeyState(VK_INSERT, InsertLock);
	}

// prvn� obsluha �asova�e, je-li po�adov�no �ek�n� na hodiny
	if (wait) 
	{
		BackTimer();
		ReDisp();
	}

// �ek�n� na sign�l od �asova�e
	MSG msg;
	for (;;)
	{

// p��jem zpr�vy (nen�-li pot�eba �ekat, ne�ek� se na zpr�vu)
		if (wait)
		{
			::GetMessage(&msg, NULL, 0, 0);
		}
		else
		{
			if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) == 0) break;
		}

// zpracov�n� zpr�vy oknem
		if (FindDialogBox)
		{
			if (::IsDialogMessage(FindDialogBox, &msg))
			{
				msg.message = (DWORD)-1;
			}
		}

// obsluha �asova�e - p�i po�adavku �ek�n� ji� byla jedna obsluha provedena
		if (msg.message == WM_TIMER)
		{
			if (!wait) BackTimer();
			wait = false;
			WhileOut = WHILEOUT;
		}

// ukon�en� programu uzav�en�m okna
		if (msg.message == WM_QUIT)
		{
			Break |= BREAKPROG;
			return;
		}

// obsluha zpr�vy
		if (msg.message != (DWORD)-1)
		{
			if (!::PreTranslateMessage(&msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
	}

// na�ten� sou�adnic my�i
	GetAktMousePos();

// inicializace koprocesoru
	FPUDouble();
}


/////////////////////////////////////////////////////////////////////////////
// �ek�n� po zadanou dobu (po�et impuls� 55 ms)

void _fastcall TimeWait(int count)
{
	for (; ((count > 0) || Pause) && (Break == 0); count--)
	{
		WaitTimer(true);
	}
};


/////////////////////////////////////////////////////////////////////////////
// prov�d�n� programu

void Exec()
{
#ifndef _MINI

// p��prava konfigura�n�ch parametr�
	CString prog = ExeFileName;
	int pos = prog.RevFind('\\');
	if (pos < 0) pos = prog.RevFind(':');
	if (pos >= 0) prog = prog.Right(prog.Length() - pos - 1);
	prog.Delete(prog.Length() - 4);
	prog.LowerCase();
	TCHAR znak = prog.Get(0);
	if ((znak >= 'a') && (znak <= 'z'))
	{
		znak += ('A' - 'a');
		prog.Set(0, znak);
	}

	REGSubkey += prog;

	INIFile.GetAktDir();
	if (INIFile.LastChar() != '\\') INIFile += '\\';
	INIFile += prog;
	INIFile += ".ini";

#endif // _MINI

// p��prava tabulky font�
	FontTab = (FONTITEM*)MemGet(FONTTABSIZE * sizeof(FONTITEM));
	MemFill(FontTab, FONTTABSIZE * sizeof(FONTITEM), 0);
	for (int ii = 0; ii < FONTTABSIZE; ii++)
	{
		FontTab[ii].UserFont.Init();
		FontTab[ii].CharSet = DEFAULT_CHARSET;
	}

// standardn� font tu�n�
	FontDefault = ::CreateFont(
			16,
			6,
			0,
			0,
			FW_BOLD,
			FALSE,
			FALSE,
			FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
			"Arial");

	if (FontDefault == NULL)
	{
		FontDefault = (HFONT)::GetStockObject(SYSTEM_FONT);
	}

	FontTab[2].Bold = true;
	FontTab[2].Font = FontDefault;
	FontTab[2].Used = 0x40000000;

// standardn� font normal
	FontTab[0].Font = ::CreateFont(
			14,
			5,
			0,
			0,
			FW_NORMAL,
			FALSE,
			FALSE,
			FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
			"Arial");

	if (FontTab[0].Font == NULL) FontTab[0].Font = FontDefault;
	FontTab[0].Used = 0x40000000;

// standardn� font fixed normal
	FontTab[1].Font = ::CreateFont(
			13,
			10,
			0,
			0,
			FW_NORMAL,
			FALSE,
			FALSE,
			FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
			"Courier New");

	if (FontTab[1].Font == NULL) FontTab[1].Font = FontDefault;
	FontTab[1].Fixed = true;
	FontTab[1].Used = 0x40000000;

// standardn� font fixed tu�n�
	FontTab[3].Font = ::CreateFont(
			16,
			8,
			0,
			0,
			FW_BOLD,
			FALSE,
			FALSE,
			FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
			"Courier New");

	if (FontTab[3].Font == NULL) FontTab[3].Font = FontDefault;
	FontTab[3].Bold = true;
	FontTab[3].Fixed = true;
	FontTab[3].Used = 0x40000000;

// p��prava identifika�n�ho k�du pro DirectPlay
	int i;

#ifndef _MINI
	int n = 4;
	DirectPlayGuidText = AktCaptionText;

	for (i = 0; i < DirectPlayGuidText.Length(); i++)
	{
		((char*)&DirectPlayGuidData)[n] = (char)(((char*)&DirectPlayGuidData)[n] + DirectPlayGuidText[i]);
		n++;
		if (n == 4+6) n = 4;
	}

// p��prava v�stupn�ho bufferu dat pro DirectPlay
	DirectPlayDataOut = (BYTE*)MemGet(DirectPlayDataOutM);

// p��prava implicitn�ho jm�na aplikace
	DDEDefApp = AktCaptionText;

// p��prava tabulky port�
	Coms = (COMITEM*)MemGet(COMSMAX*sizeof(COMITEM));
	MemFill(Coms, COMSMAX*sizeof(COMITEM), 0);

	for (i = 0; i < COMSMAX; i++)
	{
		Coms[i].File = INVALID_HANDLE_VALUE;
	}
#endif // _MINI

// inicializace informac� o plo�e
	ExecInitMap();

// inicializace buffer� zvuku
	for (i = 0; i < SOUNDBUFFERU; i++)
	{
		WaveHeader[i].dwFlags = WHDR_DONE;
		SoundBuf[i] = (BYTE*)MemGet(SOUNDBUFSIZE);
	}

	SoundChan = (SOUNDCHAN*)MemGet(MAXSOUNDKANALU * sizeof(SOUNDCHAN));
	MemFill(SoundChan, MAXSOUNDKANALU * sizeof(SOUNDCHAN), 0);
	for (i = 0; i < MAXSOUNDKANALU; i++)
	{
		SoundChan[i].Sound.Init();
	}

	for (i = 0; i < MAXJOYSTICK; i++)
	{
		JoystickNext[i] = -1;
		JoystickValid[i] = false;
		JoystickOldX[i] = -0x7fe00000;
		JoystickOldY[i] = -0x7fe00000;
		JoystickOldZ[i] = -0x7fe00000;
	}

// inicializace parametr� hry DirectPlay
#ifndef _MINI
	DirectPlayGamesParam[0] = 0x40004000;
	DirectPlayGamesParam[1] = 0x40004000;
	DirectPlayGamesParam[2] = 0x40004000;
	DirectPlayGamesParam[3] = 0x40004000;

// inicializace 3D grafiky
	Init3D();

#endif // _MINI

// zapnut� sprajtu 0 (Pet��k)
	SetSpriteVisible(0, true);

// po�adavek k p�ekreslen� okna
	AktAllIcon();				// aktualizovat v�echny ikony

// inicializace �asova�e
	TimerID = ::SetTimer(MainFrame, 543, TimerConst, NULL);

// obsluha hlavn� funkce
	ExecItem = ProgBuf + ProgStart;	// startovac� adresa
	if (ProgNum > 0) FCommand();

// uzav�en� soubor�
	FileClose();

// zastaven� p�ehr�v�n� zvuk�
	PlaySoundStop();

// zastaven� p�ehr�v�n� hudby
	MusicStop();

// uzav�en� DirectSound
	TermDirectSound();

// uvoln�n� knihovny COM
#ifndef _MINI
	COMUninit();

// uvoln�n� knihovny WSA
	WSADeinitialize();

	DeSelectD3DDev();

// zru�en� seznamu aplikac� DDE
	DDEAppListTerm();

#endif // _MINI

// ukon�en� celoobrazovkov�ho m�du
	SetFullScreen(false);

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

	D3GL2TermD3();
	D3GL1TermD3();
	D3GL0TermD3();
	D3DX8TermD3();
	D3DX7TermD3();
	D3DX7TermDD();
	D3DX6TermD3();
	D3DX6TermDD();
	D3DX5TermD3();
	D3DX5TermDD();
	D3DX3TermD3();
	D3DX3TermDD();

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------

// ukon�en� konzoly
	if (NewConsole) ::FreeConsole();

// uzav�en� okna (pokud je�t� nebylo uzav�eno zvenku)
	if (::IsWindow(MainFrame))
	{
		::SendMessage(MainFrame, WM_CLOSE, 0, 0);
	}

// uzav�en� mixeru
#ifndef _MINI
	if (MixDevH != NULL)
	{
		::mixerClose(MixDevH);
	}
#endif // _MINI

// zru�en� �asova�e
	::KillTimer(MainFrame, TimerID);

// zru�en� �t�tc�
	::DeleteObject(StdBrushBtn);
	::DeleteObject(StdBrushWindow);
	::DeleteObject(StdBrushHighlight);

// uzav�en� komunika�n�ch port�
#ifndef _MINI
	for (i = 0; i < COMSMAX; i++)
	{
		ComClose(i);
	}
#endif // _MINI

// zru�en� bufferu zvukov�ch kan�l�
	for (i = 0; i < MAXSOUNDKANALU; i++)
	{
		SoundChan[i].Sound.Term();
	}
	MemFree(SoundChan);

// zru�en� font�
	for (i = 0; i < FONTTABSIZE; i++)
	{
		FontTab[i].UserFont.Term();
		HFONT font = FontTab[i].Font;
		if ((font != NULL) && (font != FontDefault)) ::DeleteObject(font);
	}
	::DeleteObject(FontDefault);
	MemFree(FontTab);
}
