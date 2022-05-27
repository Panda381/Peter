
#include "Main.h"

/***************************************************************************\
*																			*
*							Provádìní programu								*
*																			*
\***************************************************************************/

#pragma optimize("t", on)			// optimalizace na maximální rychlost

// buffer provádìného programu
int			ProgStart = 0;		// index volání hlavní funkce
EXECITEM*	ProgBuf = NULL;		// buffer programu
int			ProgNum = 0;		// poèet položek programu
int			ProgMax = 0;		// velikost bufferu programu
EXECITEM*	ExecItem = NULL;	// ukazatel provádìného programu
int			Hloubka = MAXHLOUBKA;	// hloubka vnoøení do funkcí

// buffer obrázkù
BYTE*	BackBuf = NULL;			// buffer obrázku pozadí
BYTE*	PicBuf = NULL;			// buffer obrázku sestavy (s ikonami)
BYTE*	TextBuf = NULL;			// buffer pro výstup textu

// aktualizace pøekreslení ikon
bool*	AktBuf;					// buffer požadavkù k pøekreslení ikon

int		AktLeft;				// levý okraj k pøekreslení - vèetnì (v bodech)
int		AktTop;					// horní okraj k pøekreslení - vèetnì (v bodech)
int		AktRight;				// pravý okraj k pøekreslení - bez (v bodech)
int		AktBottom;				// dolní okraj k pøekreslení - bez (v bodech)

bool	IsOverlay = false;		// zobrazeno pøekryvné okno Windows (otevøení souborù atd.)

// aktivní zobrazená plocha
int		WidthN = 0;				// šíøka plochy v ikonách
int		HeightN = 0;			// výška plochy v ikonách
int		Width = 0;				// šíøka plochy v bodech
int		Height = 0;				// výška plochy v bodech
int		WidthByte = 0;			// délka linky v bajtech
int		RowByte = 0;			// délka øádku ikon v bajtech

// výstupní buffer
bool	DispBufUse = false;		// používá se výstupní buffer (je jiné mìøítko než 1:1)
bool	DispBufUse2 = false;	// používá se výstupní buffer v TRUE COLOR (je mìøítko 2:1)
BYTE*	DispBuf = NULL;			// výstupní buffer
int		DispBufSize = 0;		// aktuální velikost výstupního bufferu
int*	DispMapX = NULL;		// buffer mapování z bodu obrázku na výstupní bod
int*	DispMapY = NULL;		// buffer mapování z linky obrázku na výstupní linku
int*	DispRemapX = NULL;		// buffer zpìtného mapování výstupního bodu na offset v lince obrázku
int*	DispRemapY = NULL;		// buffer zpìtného mapování výstupní linky na offset v obrázku

int		DispLeft = 0;			// levý okraj k zobrazení v oknì
int		DispTop = 0;			// horní okraj k zobrazení v oknì
int		DispWidth = 1;			// zobrazená šiøka v bodech
int		DispWidthByte = 0;		// šíøka linky výstupního bufferu v bajtech
int		DispHeight = 1;			// zobrazená výška v bodech

double	Meritko = 1;			// aktuální mìøítko zobrazení
double	Meritko0 = 1;			// uschované mìøítko zobrazení pro normální okno

// zobrazení textu
int		GraphAreaWidth = 0;		// šíøka pole k zobrazení textu centrovanì (v bodech)

// stavový øádek
bool	CaptionAkt = false;		// požadavek k pøekreslení nadpisu okna
CString	CaptionText;			// nový text titulku k nastavení
bool	StatusAkt = false;		// požadavek k pøekreslení stavového øádku
CString	StatusText;				// nový stavový text k nastavení

// buffer sprajtù
CBufInt	SpriteVisible;			// buffer indexù viditelných sprajtù
CBufInt	SpriteMoving;			// buffer indexù pohybujících se sprajtù
CBufInt	SpriteKlid;				// buffer sprajtù animovaných v klidu
CBufInt	SpriteDispReq;			// buffer sprajtù vyžadujících pøekreslení
CBufInt	SpriteDispLevel;		// hladiny sprajtù vyžadujících pøekreslení
CBufReal SpriteDispY;			// souøadnice sprajtù Y 

int		SpriteWinX1 = 0;		// levý okraj okna sprajtù
int		SpriteWinY1 = 0;		// dolní okraj okna sprajtù
int		SpriteWinX2 = 1000000*ICONWIDTH;	// pravý okraj okna sprajtù
int		SpriteWinY2 = 1000000*ICONHEIGHT;	// horní okraj okna sprajtù

int		WhileOut = WHILEOUT;	// èítaè time-out cyklu
int		ReDispOut = REDISPOUT;	// èítaè time-out pøekreslení displeje

// obsluha èasovaèe
UINT	TimerID = 0;			// ID èasovaèe

bool	Pause = false;			// pøíznak zapaouzování programu
volatile int Break;				// pøíznaky pøerušení (0 = není pøerušení)

double		MouseX = 0;			// souøadnice myši X (v ikonových souøadnicích)
double		MouseY = 0;			// souøadnice myši Y (v ikonových souøadnicích)

// obsluha CD
UINT	CDDevice = 0;			// ID zaøízení MCI pro pøehrávání CD (0=není otevøeno)
int		CDError = -1;			// èítaè pro nový pokus pøi chybì (musí být < 0)
int		CDValidTime = 0;		// èítaè platnosti údajù o CD (musí být > 0)
bool	CDDiskValid = false;	// údaje o disku jsou platné, pøíznak vložení disku
int		CDStart = 2000;			// startovací pozice CD v ms
int		CDLength = CDDEFLENGTH;	// délka CD v ms
int		CDTracks = 1;			// poèet stop CD
int		CDPos = 0;				// pøehrávaná pozice CD v ms
int		CDTrack = 1;			// pøehrávaná stopa
int		CDTrackLength = CDDEFLENGTH;// délka pøehrávané stopy v ms
int		CDTrackPos = 0;			// pozice v pøehrávané stopì v ms
bool	CDPlaing = false;		// pøíznak pøehrávání CD
bool	CDPausing = false;		// pøíznak pauzy CD

// obsluha klávesnice
int		KeyBufWrite = 0;		// ukládací index do bufferu klávesnice
int		KeyBufRead = 0;			// ètecí index z bufferu klávesnice
int		KeyBuf[KEYBUFSIZE];		// buffer klávesnice

int		CharBufWrite = 0;		// ukládací index do bufferu znakù
int		CharBufRead = 0;		// ètecí index z bufferu znakù
int		CharBuf[KEYBUFSIZE];	// buffer znakù

bool	CtrlAgain = false;		// pøíznak opakovaného stisku Ctrl

int		KeyMapNum = 0;			// poèet stisknutých kláves
bool	KeyMap[KEYMAPSIZE];		// mapa stisknutých kláves

bool	NumLock = false;		// stav Num Lock
bool	CapsLock = false;		// stav Caps Lock
bool	ScrollLock = false;		// stav Scroll Lock
bool	InsertLock = false;		// stav Insert

bool	AktNumLock = false;		// požadavek nastavení Num Lock
bool	AktCapsLock = false;	// požadavek nastavení Caps Lock
bool	AktScrollLock = false;	// požadavek nastavení Scroll Lock
bool	AktInsertLock = false;	// požadavek nastavení Insert

// obsluha souborù
bool	FileError = false;		// pøíznak chyby souborù
int		FileCloseTime = 0;		// èítaè èasu pro uzavøení souborù
int		FileTextNLen = 10;		// délka textu s pevnou délkou
bool	FileTextUnicode = false; // text je v kódu UNICODE

CString	FileRead;				// jméno souboru pro ètení
HANDLE	FileReadHandle = NULL;	// handle souboru pro ètení (NULL=není)
int		FileReadOff = 0;		// offset v souboru pro ètení
BYTE*	FileReadBuf = NULL;		// buffer pro ètení
int		FileReadBufOff = 0;		// offset bufferu pro ètení
int		FileReadBufN = 0;		// poèet bajtù v bufferu pro ètení

CString	FileWrite;				// jméno souboru pro zápis
HANDLE	FileWriteHandle = NULL;	// handle souboru pro zápis (NULL=není)
int		FileWriteOff = 0;		// offset v souboru pro zápis
BYTE*	FileWriteBuf = NULL;	// buffer pro zápis
int		FileWriteBufOff = 0;	// offset bufferu pro zápis
int		FileWriteBufN = 0;		// poèet bajtù v bufferu pro zápis

CString	AliasName;				// jméno souboru ALIASES
CString	AliasKey;				// klíè ALIASES
CString	AliasGroup;				// skupina ALIASES

__int64	DiskSize = 0;			// velikost disku (z funkce GetDiskSpace)
__int64	DiskFree = 0;			// volné místo disku (z funkce GetDiskSpace)
__int64	DiskFreeUser = 0;		// volné místo uživatele (z funkce GetDiskSpace)

// konzola
bool	ConsoleOn = false;		// pøíznak režimu konzoly
bool	NewConsole = false;		// byla vytvoøena nová konzola
HANDLE	ConIn = INVALID_HANDLE_VALUE;	// handle pro vstup z konzoly
HANDLE	ConOut = INVALID_HANDLE_VALUE;	// handle pro výstup na konzolu
HANDLE	ConErr = INVALID_HANDLE_VALUE;	// handle pro chybový výstup na konzolu

// joystick
int		JoystickNext[MAXJOYSTICK];	// èítaè do dalšího testu (musí být < 0)
bool	JoystickValid[MAXJOYSTICK];	// pøíznak platnosti údajù joysticku
int		JoystickOldX[MAXJOYSTICK];	// stará souøadnice X
int		JoystickOldY[MAXJOYSTICK];	// stará souøadnice Y
int		JoystickOldZ[MAXJOYSTICK];	// stará souøadnice Z
double	JoystickX[MAXJOYSTICK];		// souøadnice X
double	JoystickY[MAXJOYSTICK];		// souøadnice Y
double	JoystickZ[MAXJOYSTICK];		// souøadnice Z
bool	Joystick1[MAXJOYSTICK];		// tlaèítko 1
bool	Joystick2[MAXJOYSTICK];		// tlaèítko 2
bool	Joystick3[MAXJOYSTICK];		// tlaèítko 3
bool	Joystick4[MAXJOYSTICK];		// tlaèítko 4

// prvky oken
CString ButtonClassName(_T("Button"));
CString EditClassName(_T("Edit"));
CString RichEditClassName(_T("RichEdit"));
CString ListBoxClassName(_T("Listbox"));
CString ComboBoxClassName(_T("Combobox"));
CString StaticClassName(_T("Static"));
CString TabsClassName(_T("SysTabControl32"));

bool	DialogGraph = false;		// je grafické pozadí hlavního dialogového okna

bool	DialogMode = false;			// dialogový mód hlavního okna
int		DialogParent = 0;			// aktivní rodièovské okno
int		WindowID = -1;				// aktivní prvek (-1 = neplatný)
int		WindowFocus = 0;			// prvek s fokusem (-1 = není nebo neznámý)
int		ButtonEsc = -1;				// tlaèítko pro Esc

int		StdColorBtnText;			// systémová barva textu tlaèítek
int		StdColorBtnFace;			// systémová barva podkladu tlaèítek
HBRUSH	StdBrushBtn;				// štìtec pozadí tlaèítek

int		StdColorWindowText;			// systémová barva textu editoru
int		StdColorWindow;				// systémová barva podkladu editoru
HBRUSH	StdBrushWindow;				// štìtec pozadí editoru

int		StdColorHighlightText;		// systémová barva textu kurzoru
int		StdColorHighlight;			// systémová barva pozadí kurzoru
HBRUSH	StdBrushHighlight;			// štìtec pozadí kurzoru

// správce fontù
int			FontNext = FONTTABSTD;	// index pøíštího volného fontu k uložení
FONTITEM*	FontTab = NULL;			// tabulka fontù
HFONT		FontDefault = NULL;		// implicitní systémový font (pøi chybì fontù)
CString		UserFont(_T("Impact"));	// uživatelský font
CString		FontList;				// seznam systémových fontù

// DirectPlay
// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

bool	Coinit = false;				// probìhla inicializace COM knihovny

LPDIRECTPLAY3A	DirectPlay	= NULL;	// objekt DirectPlay3 (NULL=není)

CString	DirectPlayConnects;			// seznam spojení DirectPlay
int		DirectPlayConnectsNum = 0;	// poèet spojení DirectPlay (0=není inicializován)
CString	DirectPlayConnectsAkt;		// aktivní spojení (prázdné = nevybráno)
int		DirectPlayConnectsAktN = -1; // aktivní spojení (-1 = není)
void**	DirectPlayConnectsConn = NULL;	// tabulka informací spojení
int		NextDirectPlayInit = 0;		// èítaè do pøíštího pokusu o otevøení DirectPlay
CString ForDPlay1(" pro DirectPlay");	// rušený text 1
CString ForDPlay2(" for DirectPlay");	// rušený text 2
CString ForDPlay3(" für DirectPlay");	// rušený text 3

CString	DirectPlayGames;			// seznam her DirectPlay
int		DirectPlayGamesNum = 0;		// poèet her DirectPlay
CString	DirectPlayGamesAkt;			// aktivní hra (prázdné = nevybráno)
int		DirectPlayGamesAktN = -1;	// aktivní hra (-1 = není nebo není zatím pøihlášena)
GUID	DirectPlayGamesAktGuid;		// GUID aktivní hry
GUID*	DirectPlayGamesGuid = NULL;	// tabulka GUID her
int		DirectPlayGamesTime = 0;	// èítaè pro aktualizaci seznamu her
DWORD	DirectPlayGamesParam[4];	// parametry hry (15 bitù èíslo + 0x4000, 16. bit = pøíznak)
bool	DirectPlayGamesHost = false;// poèítaè je hostitelem
int		DirectPlayPlayersMax = 0;	// maximální poèet hráèù (0 = neomezeno)
int		DirectPlayGamesParamTime = 0; // èítaè pro aktualizaci parametrù hry

#define MAXDIRECTPLAYERS 1024		// maximální poèet hráèù
CString	DirectPlayPlayers;			// seznam hráèù DirectPlay
int		DirectPlayPlayersNum = 0;	// poèet hráèù DirectPlay (bez zrušených)
CString	DirectPlayPlayersAkt;		// aktivní hráè (prázdné = není)
int		DirectPlayPlayersAktN = -1;	// aktivní hráè (-1 = není)
DPID	DirectPlayPlayersAktID;		// ID aktivního hráèe
bool	DirectPlayPlayersAktCreate = false; // hráè byl vytvoøen
int		DirectPlayPlayersSize = 0;	// velikost tabulky hráèù (= poèet hráèù vèetnì zrušených)
CString* DirectPlayPlayersNames = NULL;	// tabulka jmen hráèù (prázdný text = nepoužito)
DPID*	DirectPlayPlayersID = NULL; // tabulka ID hráèù
int		DirectPlayPlayersTime = 0;	// èítaè pro aktualizaci seznamu hráèù
bool	DirectPlayPlayersIDErr = false; // chyba duplikace identifikaèního èísla hráèe - nutné zmìnit

BYTE*	DirectPlayDataOut = NULL;	// datový blok výstupních dat (první 2 bajty = odesílatel)
int		DirectPlayDataOutM = 1024;	// velikost bufferu vstupních dat
int		DirectPlayDataOutN = 2;		// velikost výstupních dat

BYTE*	DirectPlayDataIn = NULL;	// datový blok vstupních dat (první 2 bajty = odesílatel)
int		DirectPlayDataInN = 0;		// velikost vstupních dat
int		DirectPlayDataInO = 0;		// offset ètení ze vstupních dat

// WSA 1.1 rozhraní pro DirectPlay
WSASTARTUP	pWSAStartup = NULL;		// funkce "WSAStartup"
WSACLEANUP	pWSACleanup = NULL;		// funkce "WSACleanup"
WSAGETHOSTNAME	pWSAGetHostName = NULL;// funkce "gethostname"
WSAGETHOSTBYNAME pWSAGetHostByName = NULL;// funkce "gethostbyname"
WSAINET_NTOA pWSAInetNtoa = NULL;	// funkce "inet_ntoa"

// rozšíøení WSA 2.2 rozhraní pro UDP
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
bool	Wsaload = false;			// knihovna WSA byla naèítána
bool	Wsainit = false;			// knihovna WSA 1.1 úspìšnì inicializována
bool	Wsainit2 = false;			// knihovna WSA 2.2 úspìšnì inicializována
WSADATA	Wsadata;					// data knihovny WSA
CString	HostIP(_T("0.0.0.0"));		// IP adresa poèítaèe
int		HostIPValid = 0;			// èítaè platnosti IP adresy poèítaèe (je-li > 0)

EXTERN_C GUID DirectPlayGuidData = {
	'rteP',							// (4) identifikátor typu dat "Petr"
	0,								// (2) identifikátor 1
	0,								// (2) identifikátor 2
	{0,0,0,0xc0,0x6c,0x47,6,0x65}	// (8) identifikátor 3 (pouze první 2 bajty, ostatní nemìnit)
};

GUID*	DirectPlayGuid = &DirectPlayGuidData;		// identifikace
CString	DirectPlayGuidText;			// identifikaèní text pro DirectPlay

// porty (není u MINI verze)
COMITEM*	Coms;					// tabulky portù
int			ComAkt = 0;				// aktivní port

// mixer (není u MINI verze)
int		MixDevN = 0;				// poèet mixážních zaøízení
int		MixDevA = -1;				// aktivní mixážní zaøízení (-1=není)
CString	MixDevT;					// víceøádkový seznam mixážních zaøízení
MIXDEV*	MixDev = NULL;				// seznam mixážních zaøízení
HMIXER	MixDevH = NULL;				// handle mixeru (NULL=není)

int		MixDest = 0;				// poèet cílových signálù mixeru
int		MixLineN = 0;				// poèet signálù mixeru
MIXLINE* MixLine = NULL;			// seznam signálù (na zaèátku jsou cílové)
CString	MixLineDT;					// víceøádkový seznam cílových signálù
CString	MixLineST;					// víceøádkový seznam zdrojových signálù
int		MixLineDA = -1;				// aktivní cílový signál (-1=žádný)
int		MixLineSA = -1;				// aktivní zdrojový signál cílového signálu (-1=jen cílový)
int		MixLineA = -1;				// aktivní signál absolutnì (v tabulce) (-1=není)

int		MixCtrlN = 0;				// poèet ovládacích prvkù signálu
MIXCTRL* MixCtrl = NULL;			// seznam ovládacích prvkù
CString	MixCtrlT;					// víceøádkový seznam ovládacích prvkù
int		MixCtrlA = -1;				// aktivní ovládací prvek (-1=není)
int		MixChannel = -1;			// nastavovaný kanál (-1=všechny)

int		MixValN = 0;				// poèet hodnot prvku (0=není seznam hodnot)
CString* MixVal = NULL;				// buffer jmen ovládacích prvkù
CString	MixValT;					// seznam hodnot prvku seznamu
int		MixValA = 0;				// aktivní prvek seznamu (-1=není)

// obsluha DDE
CString		DDEDefApp;				// implicitní jméno aplikace pro DDE

DDEAPPITEM*	DDEAppList = NULL;		// seznam aplikací
int			DDEAppNum = 0;			// poèet aplikací
int			DDEAppMax = 0;			// velikost bufferu aplikací
bool		DDEAppLoad = false;		// naèítá se seznam aplikací

int			DDEAppAkt = -1;			// aktivní aplikace (-1=všechny)
CString		DDEAppAktName;			// jméno aktivní aplikace

DDESERVITEM* DDEServList = NULL;	// seznam severù
int			DDEServNum = 0;			// poèet serverù
int			DDEServMax = 0;			// velikost bufferu serverù
int			DDEServAkt = -1;		// aktivní server (-1=všechny)

DDETOPITEM*	DDETopList = NULL;		// seznam témat
int			DDETopNum = 0;			// poèet témat
int			DDETopMax = 0;			// velikost bufferu témat
int			DDETopAkt = -1;			// aktivní téma (-1=všechny)

DDEDATAITEM* DDEDataList = NULL;	// seznam datových položek
int			DDEDataNum = 0;			// poèet datových položek
int			DDEDataMax = 0;			// velikost bufferu datových položek
int			DDEDataAkt = -1;		// aktivní položka (-1=všechny)

// obsluha DLL
void*		DLLMemoryRead = NULL;	// ukazatel ètení z pamìti
void*		DLLMemoryWrite = NULL;	// ukazatel zápisu do pamìti
int			DLLMemoryTextNLen = 0;	// délka textu s pevnou délkou

// konfigurace
CString		INIFile;				// jméno konfiguraèního souboru
CString		INISection(_T("Main"));	// jméno konfiguraèní sekce
CString		INIKey(_T("Config"));	// jméno konfiguraèního parametru
int			REGKey = 1;				// skupina registrù (1=CURRENT_USER, 2=LOCAL_MACHINE)
CString		REGSubkey(_T("Software\\")); // jméno klíèe
CString		REGValue(_T("Config"));	// jméno položky

// UDP
SOCKET		UDPSendSocket = INVALID_SOCKET;	// UDP vysílací soket (INVALID_SOCKET = není)
SOCKADDR_IN	UDPSendAddr;			// vysílací adresa UDP
WSAOVERLAPPED UDPSendOver;			// overlapped struktura vysílaèe
WSABUF		UDPSendBuf;				// odesílací buffer UDP
bool		UDPSending = false;		// probíhá vysílání UDP

SOCKET		UDPRecSocket = INVALID_SOCKET; // UDP pøijímací soket (INVALID_SOCKET = není)
SOCKADDR_IN	UDPRecAddr;				// pøijímací adresa UDP
SOCKADDR_IN	UDPRecRecAddr;			// pøijatá adresa UDP
WSAOVERLAPPED UDPRecOver;			// overlapped struktura pøijímaèe
WSABUF		UDPRecBuf;				// pøijímací buffer UDP
bool		UDPReceiving = false;	// probíhá overlapped pøíjem UDP
int			UDPRecData = 0;			// velikost dat v pøijímacím bufferu

/***************************************************************************\
*																			*
*								obsluha DDE									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// zrušení seznamu serverù

void DDEAppListTerm()
{
	int i;

// zrušení seznamu položek
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

// zrušení seznamu témat
	for (i = DDETopNum-1; i >= 0; i--)
	{
		DDETopList[i].TopicName.Term();
	}

	MemFree(DDETopList);
	DDETopList = NULL;
	DDETopNum = 0;
	DDETopMax = 0;

// zrušení seznamu serverù
	for (i = DDEServNum-1; i >= 0; i--)
	{
		DDEServList[i].ServName.Term();
		DDEServList[i].Execute.Term();
	}
	
	MemFree(DDEServList);
	DDEServList = NULL;
	DDEServNum = 0;
	DDEServMax = 0;

// zrušení seznamu aplikací
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
// naètení seznamu aplikací a témat

void DDEAppListLoad()
{
// zrušení seznamu aplikací
	DDEAppListTerm();

// zjištìní seznamu aplikací a témat
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
// inicializace seznamu mixážních zaøízení (vrací TRUE=OK)

bool MixDevInit()
{
	if (MixDevN == 0)
	{

// zjištìní poètu mixážních zaøízení
		int i = ::mixerGetNumDevs();
		if (i <= 0)
		{
			return false;
		}
		MixDevN = i;

// vytvoøení bufferu mixážních zaøízení
		MIXDEV* md = (MIXDEV*)MemGet(MixDevN * sizeof(MIXDEV));
		MixDev = md;

// naètení informací o jednotlivých mixážních zaøízení
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

// otevøení implicitního mixeru
		if (!MixDevSet(0)) return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// pøidání linky do seznamu linek

void MixAddLine(MIXERLINE* ml)
{
// vytvoøení nové položky
	MixLineN++;
	MixLine = (MIXLINE*)MemSize(MixLine, MixLineN*sizeof(MIXLINE));
	MIXLINE* item = &MixLine[MixLineN-1];

// naètení jména linky
	item->Name.Init(ml->szName);
	if (item->Name.IsEmpty())
	{
		item->Name.Term();
		item->Name.Init(ml->szShortName);
	}

// cílový signál
	item->Dest = ml->dwDestination;

// zdrojový signál (pro cílový je = -1)
	item->Source = ml->dwSource;

// identifikátor signálu
	item->LineID = ml->dwLineID;

// typ signálu
	item->Type = ml->dwComponentType;

// poèet kanálù (nesmí být <= 0!)
	item->Channels = ml->cChannels;
	if (item->Channels < 1) item->Channels = 1;

// poèet zdrojù
	item->Sources = ml->cConnections;

// poèet ovládacích prvkù
	item->Controls = ml->cControls;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení bufferu hodnot prvku

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
// zrušení bufferu ovládacích prvkù

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
// nastavení aktivního mixeru (vrací TRUE=OK)

bool MixDevSet(int mix)
{
// inicializace mixeru
	if (!MixDevInit()) return false;

// korekce èísla mixeru
	if ((DWORD)mix >= (DWORD)MixDevN) mix = MixDevN-1;

// test, zda se èíslo mixeru liší
	if (mix != MixDevA)
	{

// uzavøení pùvodního mixeru
		if (MixDevH != NULL)
		{
			::mixerClose(MixDevH);
		}

// zrušení pùvodního bufferu signálù
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

// zrušení pùvodního bufferu ovládacích prvkù
		MixCtrlTerm();

// otevøení nového mixeru
		if (::mixerOpen(&MixDevH, mix, 0, 0, MIXER_OBJECTF_MIXER) != MMSYSERR_NOERROR)
		{
			MixDevH = NULL;
			MixDevA = -1;
			MixDest = 0;
			return false;
		}

// nový mixer je platný
		MixDevA = mix;
		MixDest = MixDev[mix].Dest;

// naètení cílových signálù
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
		
// naètení zdrojových signálù
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

// nastavení implicitního cílového signálu
		MixDstSet(0);
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// nastavení cílového signálu (-1=žádný)

void MixDstSet(int dst)
{
// inicializace mixeru
	if (!MixDevInit()) return;

// zrušení pùvodních bufferù
	MixLineST.Empty();
	MixLineDA = dst;
	MixLineA = dst;
	MixLineSA = -1;

// zrušení pùvodního bufferu ovládacích prvkù
	MixCtrlTerm();

// korekce èísla signálu
	if ((DWORD)dst >= (DWORD)MixDest)
	{
		MixLineDA = -1;
		MixLineA = -1;
		return;
	}

// sestavení seznamu zdrojových signálù
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

// implicitní zdrojový signál
	MixSrcSet(-1);	
}

/////////////////////////////////////////////////////////////////////////////
// nastavení zdrojového signálu (-1=jen cílový)

void MixSrcSet(int src)
{
// zrušení pùvodního bufferu ovládacích prvkù
	MixCtrlTerm();

// inicializace mixeru
	MixLineSA = -1;
	MixLineA = -1;
	if ((!MixDevInit()) || ((DWORD)MixLineDA >= (DWORD)MixDest)) return;

// korekce èísla signálu
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

// naètení seznamu ovládacích prvkù
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

// úschova parametrù ovládacích prvkù
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
// nastavení ovládacího prvku (-1=žádný)

void MixCtrlSet(int ctrl)
{
// inicializace mixeru
	if (!MixDevInit()) return;

// kontrola èísla prvku
	if ((DWORD)ctrl >= (DWORD)MixCtrlN) ctrl = -1;

// nastavení aktivního prvku
	MixCtrlA = ctrl;

// zjištìní seznamu hodnot prvku seznamu
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
// zjištìní hodnoty ovládacího prvku

double GetMixCtrlVal()
{
// inicializace mixeru
	if (!MixDevInit() || ((DWORD)MixCtrlA >= (DWORD)MixCtrlN)) return 0;
	MIXCTRL* item = &MixCtrl[MixCtrlA];

// test, zda je seznam
	bool list = false;
	if (MixValN > 1) list = true;
	if (list && ((DWORD)MixValA >= (DWORD)item->List)) return 0;

// test, zda je multikanál
	bool multi = false;
	if ((DWORD)MixChannel >= (DWORD)item->Channels) multi = true;

// naètení stavu prvku
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

	mcd.cbDetails = sizeof(MIXERCONTROLDETAILS_SIGNED); // platí i pro UNSIGNED a BOOLEAN
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
// nastavení hodnoty ovládacího prvku

void SetMixCtrlVal(double val)
{
// inicializace mixeru
	if (!MixDevInit() || ((DWORD)MixCtrlA >= (DWORD)MixCtrlN)) return;
	MIXCTRL* item = &MixCtrl[MixCtrlA];

// test, zda je seznam
	bool list = false;
	if (MixValN > 1) list = true;
	if (list && ((DWORD)MixValA >= (DWORD)item->List)) return;

// test, zda je multikanál
	bool multi = false;
	if ((DWORD)MixChannel >= (DWORD)item->Channels) multi = true;

// korekce hodnoty
	if (val > 1) val = 1;
	if (val < 0) val = 0;

// naètení aktuálního stavu prvku
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

	mcd.cbDetails = sizeof(MIXERCONTROLDETAILS_SIGNED); // platí i pro UNSIGNED a BOOLEAN
	mcd.paDetails = dat;

	if (::mixerGetControlDetails((HMIXEROBJ)MixDevH, &mcd,
				MIXER_GETCONTROLDETAILSF_VALUE | MIXER_OBJECTF_HMIXER) != MMSYSERR_NOERROR)
	{
		data->lValue = 0x7fffffff;
	}

// výpoèet aktuálního a požadovaného kroku
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

// nastavení nového stavu, pokud se liší
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
*								Obsluha portù								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// uzavøení portu (i=platný index)

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
// naètení DCB aktivního portu

void GetComDCB()
{
// adresa popisovaèe portu
	COMITEM* item = Coms + ComAkt;
	DCB* dcb = &(item->Dcb);
	dcb->DCBlength = sizeof(DCB);
	HANDLE file = item->File;

// naètení DCB portu
	if ((file == INVALID_HANDLE_VALUE) ||
		!::GetCommState(file, dcb))
	{
		MemFill(dcb, sizeof(DCB), 0);
		dcb->BaudRate = 1200;
		dcb->ByteSize = 8;
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastavení DCB aktivního portu

void SetComDCB()
{
// adresa popisovaèe portu
	COMITEM* item = Coms + ComAkt;
	DCB* dcb = &(item->Dcb);
	dcb->DCBlength = sizeof(DCB);
	HANDLE file = item->File;

// zápis DCB portu
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
// vyslání bajtu na komunikaèní port

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
			file,		// handle zaøízení
			&data,		// adresa dat
			1,			// poèet bajtù k zápisu
			&writen,	// ukazatel zapsaných dat
			NULL);		// pøekryv
	}
}

/////////////////////////////////////////////////////////////////////////////
// pøíjem bajtu z komunikaèního portu (0=nejsou data)

BYTE ComReceive()
{
	BYTE data = 0;

	COMITEM* item = Coms + ComAkt;
	HANDLE file = item->File;
	if (file != INVALID_HANDLE_VALUE)
	{
		DWORD readen;

		::ReadFile(
			file,		// handle zaøízení
			&data,		// adresa bufferu dat
			1,			// poèet bajtù k naètení
			&readen,	// ukazatel naètených dat
			NULL);		// pøekryv

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

// ------------------------ obsluha pøenosu dat -----------------------------

/////////////////////////////////////////////////////////////////////////////
// zápis dat do výstupního bufferu

void DirectSendData(BYTE* data, int n)
{
// test, zda je platný hráè
	if (!DirectPlayPlayersAktCreate) return;

// zvýšení velikosti bufferu
	if ((DirectPlayDataOutN + n) > DirectPlayDataOutM)
	{
		DirectPlayDataOutM = (DirectPlayDataOutN + n + 0x400 + 0x3ff) & ~0x3ff;
		DirectPlayDataOut = (BYTE*)MemSize(DirectPlayDataOut, DirectPlayDataOutM);
	}

// uložení dat do bufferu
	MemCopy(DirectPlayDataOut + DirectPlayDataOutN, data, n);

// zvýšení èítaèe dat v bufferu
	DirectPlayDataOutN += n;
}


/////////////////////////////////////////////////////////////////////////////
// vyslání bloku dat (-1 = všem)

void DirectSend(int id)
{
// test, zda jsou nìjaká data k odeslání
	if (DirectPlayDataOutN <= 2) return;

// nelze zaslat zprávu sám sobì
	if (id == DirectPlayPlayersAktN)
	{
		DirectPlayDataOutN = 2;
		return;
	}

// test, zda je platný hráè
	if (!DirectPlayPlayersAktCreate) return;

// pøíprava èísla odesílatele
	*((WORD*)DirectPlayDataOut) = (WORD)DirectPlayPlayersAktN;

// pøíprava èísla pøíjemce
	DPID dst = DPID_ALLPLAYERS;
	if (((DWORD)id < (DWORD)DirectPlayPlayersSize) &&
		DirectPlayPlayersNames[id].IsNotEmpty())
	{
		dst = DirectPlayPlayersID[id];
	}

// odeslání bloku dat
	DirectPlay->Send(DirectPlayPlayersAktID, dst, 0 /*DPSEND_GUARANTEED*/, DirectPlayDataOut, DirectPlayDataOutN);

// zrušení výstupních dat
	DirectPlayDataOutN = 2;
}


/////////////////////////////////////////////////////////////////////////////
// ètení dat ze vstupního bufferu (pøi nedostatku dat doplní nulami)

void DirectReceiveData(BYTE* data, int n)
{
// poèet dat k pøenesení
	int i = DirectPlayDataInN - DirectPlayDataInO;
	if (i > n) i = n;

// pøenesení platných dat
	if (i > 0)
	{
		MemCopy(data, DirectPlayDataIn + DirectPlayDataInO, i);
		DirectPlayDataInO += i;
	}

// vymazání zbytku dat
	if (n > i)
	{
		MemFill(data + i, n - i, 0);
	}
}


/////////////////////////////////////////////////////////////////////////////
// pøíjem bloku dat (-1 = není)

int DirectReceive()
{
// test, zda je platný hráè
	if (!DirectPlayPlayersAktCreate) return -1;

// nulování èítaèe pro aktualizaci parametrù hry (pøijímá se jako systémová zpráva)
	DirectPlayGamesParamTime = 2*18;

// pøíprava pracovních promìnných
	DPID src;
	DPID dst;
	HRESULT res;
	DirectPlayDataInN = 1023;

// cyklus ètení, dokud nebude správná velikost bufferu
	do
	{

// pøíprava vstupního bufferu (na konci rezerva pro zakonèovací nulu)
		MemFree(DirectPlayDataIn);
		DirectPlayDataIn = (BYTE*)MemGet(DirectPlayDataInN + 1);

// naètení zprávy
		dst = DirectPlayPlayersAktID;
		res = DirectPlay->Receive(&src, &dst, DPRECEIVE_TOPLAYER, DirectPlayDataIn, (DWORD*)&DirectPlayDataInN);

// pøíjem systémové zprávy
		if ((res == DP_OK) && (src == DPID_SYSMSG))
		{
			DPMSG_GENERIC* msg = (DPMSG_GENERIC*)DirectPlayDataIn;
			switch(msg->dwType)
			{
			case DPSYS_CREATEPLAYERORGROUP:		// vytvoøen nový hráè
			case DPSYS_DELETEPLAYERFROMGROUP:	// hráè zrušen ze skupiny
			case DPSYS_DESTROYPLAYERORGROUP:	// hráè zrušen ze hry
			case DPSYS_SETPLAYERORGROUPDATA:	// zmìnìna data hráèe
			case DPSYS_SETPLAYERORGROUPNAME:	// zmìnìno jméno hráèe
				DirectPlayPlayersTime = 0;
				break;

			case DPSYS_SETSESSIONDESC:			// zmìnìno nastavení hry
				GetGameDesc();					// znovunaètení parametrù hry
				break;

			case DPSYS_HOST:					// poèítaè se stává hostitelem
				DirectPlayGamesHost = true;
				break;
			}
			DirectPlayDataInN = 1023;
			res = DPERR_BUFFERTOOSMALL;
		}

// vlastní zpráva se ignoruje
		if ((res == DP_OK) && (DirectPlayDataInN > 2) && (*(WORD*)DirectPlayDataIn == (WORD)DirectPlayPlayersAktN))
		{
			DirectPlayDataInN = 1023;
			res = DPERR_BUFFERTOOSMALL;
		}

// pøi malém bufferu opakování
	} while (res == DPERR_BUFFERTOOSMALL);

// test pøíjmu dat OK
	if ((res == DP_OK) && (DirectPlayDataInN > 2))
	{
		DirectPlayDataIn[DirectPlayDataInN] = 0;	// zakonèovací nula textù
		DirectPlayDataInO = 2;						// offset ètených dat
		return *(WORD*)DirectPlayDataIn;			// èíslo odesílatele
	}
	else
	{
		DirectPlayDataInO = 0;
		DirectPlayDataInN = 0;
		return -1;
	}
}



// ------------------------ obsluha seznamu hráèù ---------------------------

/////////////////////////////////////////////////////////////////////////////
// nastavení aktivního hráèe (prázdné = vypnutí)

void SetPlayerAct(CString txt)
{
// aktualizace seznamu hráèù (dvakrát kvùli korekci chyby pøekryvu identifikaèního èísla)
	DirectPlayEnumPlayers();
	DirectPlayEnumPlayers();

// korekce textu
	txt.TrimLeft();
	txt.TrimRight();

// test, zda je zmìna jména
	if (txt == DirectPlayPlayersAkt) return;

// zrušení starého hráèe
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

// test, zda je otevøena hra
	if (DirectPlayGamesAkt.IsEmpty()) return;

// zajištìní jedineènosti jména nového hráèe
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

// test, zda je nutné mìnit jméno po korekci
	if (txt == DirectPlayPlayersAkt) return;

// pøíprava parametrù hráèe
	DPNAME name;
	MemFill(&name, sizeof(DPNAME), 0);
	name.dwSize = sizeof(DPNAME);
	name.lpszShortNameA = (LPTSTR)(LPCTSTR)txt;

// pøejmenování hráèe
	if (DirectPlayPlayersAktCreate)
	{
		if (DirectPlay->SetPlayerName(DirectPlayPlayersAktID, &name, DPSET_GUARANTEED) == DP_OK)
		{
			DirectPlayPlayersAkt = txt;
		}
	}

	else
	{

// pøíprava identifikaèního kódu hráèe
		long id = 0;
		for (; id < DirectPlayPlayersSize; id++)
		{
			if (DirectPlayPlayersNames[id].IsEmpty()) break;
		}

// vytvoøení hráèe
		if (DirectPlay->CreatePlayer(&DirectPlayPlayersAktID, &name, NULL, &id, sizeof(long), 0) == DP_OK)
		{
			DirectPlayPlayersAktCreate = true;
			DirectPlayPlayersAkt = txt;
		}
	}

// aktualizace seznamu hráèù (opakovanì pro pøípad duplikace èísla)
	DirectPlayPlayersTime = 0;
	DirectPlayEnumPlayers();
	DirectPlayEnumPlayers();
}


/////////////////////////////////////////////////////////////////////////////
// callback pro zjištìní seznamu hráèù

BOOL FAR PASCAL EnumPlayersCallback2(
  DPID dpId,
  DWORD dwPlayerType,
  LPCDPNAME lpName,
  DWORD dwFlags,
  LPVOID lpContext
  )
{
// pøíprava textu jména hráèe
	CString txt(lpName->lpszShortNameA);
	if (txt.IsEmpty()) txt = lpName->lpszLongNameA;
	txt.TrimLeft();
	txt.TrimRight();
	if (txt.IsEmpty()) return TRUE;

// pøíprava a kontrola èísla hráèe
	long id = DirectPlayPlayersSize;
	DWORD size = sizeof(long);
	DirectPlay->GetPlayerData(dpId, &id, &size, 0);
	if ((DWORD)id >= (DWORD)MAXDIRECTPLAYERS) return TRUE;

// zvýšení velikosti bufferù
	if (id >= DirectPlayPlayersSize)
	{
		DirectPlayPlayersNames = (CString*)MemSize(DirectPlayPlayersNames, (id + 1) * sizeof(CString));
		for (; DirectPlayPlayersSize <= id; DirectPlayPlayersSize++)
		{
			DirectPlayPlayersNames[DirectPlayPlayersSize].Init();
		}
		DirectPlayPlayersID = (DPID*)MemSize(DirectPlayPlayersID, DirectPlayPlayersSize * sizeof(DPID));
	}

// zvýšení èítaèe platných hráèù
	DirectPlayPlayersNum++;

// korekce aktivního hráèe
	if (DirectPlayPlayersAktCreate)
	{

// test, zda to je aktivní hráè
		if (dpId == DirectPlayPlayersAktID)
		{
			DirectPlayPlayersAkt = txt;
			DirectPlayPlayersAktN = id;

// kontrola pøekryvu èísel hráèù (korekci provádí hráè s vyšším ID)
			if (DirectPlayPlayersNames[id].IsNotEmpty() && (DirectPlayPlayersID[id] < dpId))
			{
				DirectPlayPlayersIDErr = true;
			}
		}
		else
		{

// kontrola pøekryvu èísel hráèù (korekci provádí hráè s vyšším ID)
			if (DirectPlayPlayersNames[id].IsNotEmpty() && 
				(DirectPlayPlayersID[id] == DirectPlayPlayersAktID) &&
				(DirectPlayPlayersAktID > dpId))
			{
				DirectPlayPlayersIDErr = true;
			}
		}
	}

// uložení parametrù do seznamu
	DirectPlayPlayersNames[id] = txt;
	DirectPlayPlayersID[id] = dpId;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// seznam hráèù (FALSE=chyba)

bool DirectPlayEnumPlayers()
{
// test, zda je seznam ještì platný
	if (DirectPlayPlayersTime > 0) return true;

// test, zda je vybrána hra
	if (DirectPlayGamesAkt.IsEmpty()) return false;

// zrušení bufferu jmen a identifikace hráèù
	for (int i = DirectPlayPlayersSize - 1; i >= 0; i--)
	{
		DirectPlayPlayersNames[i].Term();
	}
	MemFree(DirectPlayPlayersNames);
	DirectPlayPlayersNames = NULL;
	MemFree(DirectPlayPlayersID);
	DirectPlayPlayersID = NULL;
	DirectPlayPlayersSize = 0;

// zjištìní hráèù
	DirectPlayPlayersNum = 0;
	DirectPlayPlayersAkt.Empty();
	DirectPlayPlayersAktN = -1;
	DirectPlayPlayersIDErr = false;
	DirectPlay->EnumPlayers(NULL, EnumPlayersCallback2, NULL, DPENUMPLAYERS_ALL);

// sestavení textu seznamu jmen hráèù
	DirectPlayPlayers.Empty();
	for (i = 0; i < DirectPlayPlayersSize; i++)
	{
		DirectPlayPlayers += DirectPlayPlayersNames[i];
		DirectPlayPlayers += _T(13);
		DirectPlayPlayers += _T(10);
	}

// nový èítaè platnosti seznamu
	if (DirectPlayPlayersNum == 0)
	{
		DirectPlayPlayersTime = 2;
	}
	else
	{
		DirectPlayPlayersTime = 9;
	}

// hráè byl zrušen ze hry
	if ((DirectPlayPlayersAktN < 0) && DirectPlayPlayersAktCreate)
	{
		DirectPlay->DestroyPlayer(DirectPlayPlayersAktID);
		DirectPlayPlayersAktCreate = false;
		DirectPlayDataOutN = 2;
		DirectPlayDataInO = 0;
		DirectPlayDataInN = 0;
	}

// korekce identifikaèního èísla hráèe
	if (DirectPlayPlayersIDErr)
	{

// pøíprava nového identifikaèního kódu hráèe
		long id = 0;
		for (; id < DirectPlayPlayersSize; id++)
		{
			if (DirectPlayPlayersNames[id].IsEmpty()) break;
		}

// zmìna identifikaèního kódu hráèe
		DirectPlay->SetPlayerData(DirectPlayPlayersAktID, &id, sizeof(long), DPSET_GUARANTEED);

// nutná aktualizace
		DirectPlayPlayersTime = 0;
	}

	return true;
}


// ------------------------- obsluha seznamu her ----------------------------


/////////////////////////////////////////////////////////////////////////////
// naètení parametrù hry

void GetGameDesc()
{
// test, zda je otevøena hra
	if (DirectPlayGamesAkt.IsEmpty()) return;

// pøíprava bufferu popisovaèe hry
	DWORD n = sizeof(DPSESSIONDESC2) + 256;
	DPSESSIONDESC2* dsc = (DPSESSIONDESC2*)MemGet(n);
	MemFill(dsc, n, 0);
	dsc->dwSize = sizeof(DPSESSIONDESC2);

// naètení informací o høe
	if (DirectPlay->GetSessionDesc(dsc, &n) == DP_OK)
	{
		DirectPlayGamesParam[0] = dsc->dwUser1;
		DirectPlayGamesParam[1] = dsc->dwUser2;
		DirectPlayGamesParam[2] = dsc->dwUser3;
		DirectPlayGamesParam[3] = dsc->dwUser4;
		DirectPlayPlayersMax = dsc->dwMaxPlayers;
	}

// uvolnìní bufferu
	MemFree(dsc);
}


/////////////////////////////////////////////////////////////////////////////
// nastavení nových parametrù

void SetGameDesc(DPSESSIONDESC2* dsc)
{
// musí být hostitel
	if (!DirectPlayGamesHost) return;

// korekce maximálního poètu hráèù
	if (((int)dsc->dwMaxPlayers != 0) && ((int)dsc->dwMaxPlayers < DirectPlayPlayersNum))
	{
		dsc->dwMaxPlayers = DirectPlayPlayersNum;
	}

// test, zda je zmìna dat
	if (
		(DirectPlayGamesParam[0] != dsc->dwUser1) ||
		(DirectPlayGamesParam[1] != dsc->dwUser2) ||
		(DirectPlayGamesParam[2] != dsc->dwUser3) ||
		(DirectPlayGamesParam[3] != dsc->dwUser4) ||
		(DirectPlayPlayersMax != (int)dsc->dwMaxPlayers)
		)
	{

// pøíprava záhlaví popisovaèe
		dsc->dwSize = sizeof(DPSESSIONDESC2);
		dsc->dwFlags = DPSESSION_MIGRATEHOST;
		dsc->lpszPasswordA = NULL;

// jméno hry
		dsc->lpszSessionNameA = (LPTSTR)(LPCTSTR)DirectPlayGamesAkt;

// nastavení parametrù hry
		if (DirectPlay->SetSessionDesc(dsc, 0) == DP_OK)
		{

// zplatnìní zmìnìných parametrù
			DirectPlayGamesParam[0] = dsc->dwUser1;
			DirectPlayGamesParam[1] = dsc->dwUser2;
			DirectPlayGamesParam[2] = dsc->dwUser3;
			DirectPlayGamesParam[3] = dsc->dwUser4;
			DirectPlayPlayersMax = dsc->dwMaxPlayers;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastavení max. poètu hráèù (0 = neomezeno)

void SetPlayerMax(int n)
{
// test, zda je otevøena hra
	if (DirectPlayGamesAkt.IsEmpty()) return;

// omezení èíselného údaje
	if ((n < 0) || (n > 0xfffff)) n = 0;

// popisovaè hry
	DPSESSIONDESC2 dsc;

// maximální poèet hráèù
	dsc.dwMaxPlayers = n;

// parametry
	dsc.dwUser1 = DirectPlayGamesParam[0];
	dsc.dwUser2 = DirectPlayGamesParam[1];
	dsc.dwUser3 = DirectPlayGamesParam[2];
	dsc.dwUser4 = DirectPlayGamesParam[3];

// nastavení nových parametrù
	SetGameDesc(&dsc);
}

/////////////////////////////////////////////////////////////////////////////
// nastavení èíselné hodnoty hry (n = rozsah -16384 až +16383, i = index 0 až 7)

void SetGameN(int n, int i)
{
// test, zda je otevøena hra
	if (DirectPlayGamesAkt.IsEmpty()) return;

// omezení èíselného údaje
	if (n < -0x4000) n = -0x4000;
	if (n > 0x3fff) n = 0x3fff;

// popisovaè hry
	DPSESSIONDESC2 dsc;

// maximální poèet hráèù
	dsc.dwMaxPlayers = DirectPlayPlayersMax;

// parametry
	dsc.dwUser1 = DirectPlayGamesParam[0];
	dsc.dwUser2 = DirectPlayGamesParam[1];
	dsc.dwUser3 = DirectPlayGamesParam[2];
	dsc.dwUser4 = DirectPlayGamesParam[3];

// nastavení požadovaného parametru
	WORD* dat = (WORD*)&(dsc.dwUser1) + i;
	*dat = (WORD)((*dat & 0x8000) | (n + 0x4000));

// nastavení nových parametrù
	SetGameDesc(&dsc);
}


/////////////////////////////////////////////////////////////////////////////
// nastavení pøepínaèe hry (i = index 0 až 7)

void SetGameL(bool n, int i)
{
// test, zda je otevøena hra
	if (DirectPlayGamesAkt.IsEmpty()) return;

// popisovaè hry
	DPSESSIONDESC2 dsc;

// maximální poèet hráèù
	dsc.dwMaxPlayers = DirectPlayPlayersMax;

// parametry
	dsc.dwUser1 = DirectPlayGamesParam[0];
	dsc.dwUser2 = DirectPlayGamesParam[1];
	dsc.dwUser3 = DirectPlayGamesParam[2];
	dsc.dwUser4 = DirectPlayGamesParam[3];

// nastavení požadovaného parametru
	WORD* dat = (WORD*)&(dsc.dwUser1) + i;
	*dat = (WORD)(*dat & 0x7fff);
	if (n) *dat = (WORD)(*dat | 0x8000);

// nastavení nových parametrù
	SetGameDesc(&dsc);
}


/////////////////////////////////////////////////////////////////////////////
// nastavení aktivní hry (prázdné = vypnutí)

void SetGameAct(CString txt)
{
// korekce textu
	txt.TrimLeft();
	txt.TrimRight();

// test, zda je zmìna aktivní hry
	if (txt == DirectPlayGamesAkt) return;

// uzavøení staré hry
	if (DirectPlayGamesAkt.IsNotEmpty())
	{
		SetPlayerAct(EmptyString);
		DirectPlay->Close();
		DirectPlayGamesAkt.Empty();
		DirectPlayGamesAktN = -1;
	}
	DirectPlayGamesHost = false;

// opuštìní hry
	if (txt.IsEmpty()) return;

// test, zda je otevøeno spojení
	if (DirectPlayConnectsAktN < 0) return;

// zajištìní naètení seznamu her            (zbyteènì požaduje IP i pøi vytváøení hry)
/*
	if (DirectPlayGames.IsEmpty())
	{
		DirectPlayGamesTime = -1000;
		DirectPlayEnumGames();
		if (DirectPlayGamesTime > 3*18) return;
	}
*/

// nalezení shodného textu
	int i;
	for (i = 0; i < DirectPlayGamesNum; i++)
	{
		if (DirectPlayGames.GetLine(i) == txt) break;
	}

// popisovaè hry
	DPSESSIONDESC2 dpDesc;
	MemFill(&dpDesc, sizeof(DPSESSIONDESC2), 0);
	dpDesc.dwSize = sizeof(DPSESSIONDESC2);
	dpDesc.guidApplication = *DirectPlayGuid;

// vytvoøení nové hry
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

// pøipojení k existující høe
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

// aktualizace seznamu her (pøi úspìchu) - také pro zastavení vyhledávání her
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
// pøi chybì již dále neopakovat
	if ((lpThisSD == NULL) || (dwFlags & DPESC_TIMEDOUT)) return FALSE;

// pøíprava textu jména hry
	CString txt(lpThisSD->lpszSessionNameA);
	txt.TrimLeft();
	txt.TrimRight();

// zajištìní jedineènosti jména
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

// pøidání textu k seznamu
	DirectPlayGames += txt;
	DirectPlayGames += _T(13);
	DirectPlayGames += _T(10);

// zvýšení èítaèe her
	int i = DirectPlayGamesNum;
	DirectPlayGamesNum++;
	int i2 = DirectPlayGamesNum;

// úschova GUID hry
	DirectPlayGamesGuid = (GUID*)MemSize(DirectPlayGamesGuid, i2 * sizeof(GUID));
	DirectPlayGamesGuid[i] = lpThisSD->guidInstance;

// test, zda to je aktivní hra
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
// seznam her (FALSE=chyba), pøi otevøené høe se zastaví vyhledávání her

bool DirectPlayEnumGames()
{
// test, zda je seznam ještì platný
	if (DirectPlayGamesTime > 0) return true;

// test, zda je vybráno spojení
	if (DirectPlayConnectsAktN < 0) return false;

// pøíprava popisovaèe
    DPSESSIONDESC2 dpDesc;
	MemFill(&dpDesc, sizeof(DPSESSIONDESC2), 0);
	dpDesc.dwSize = sizeof(DPSESSIONDESC2);
	dpDesc.guidApplication = *DirectPlayGuid;

// zjištìní her (pøi otevøené høe se bere pouze z cache)
	DirectPlayGames.Empty();
	DirectPlayGamesNum = 0;
	DirectPlayGamesAktN = -1;
	HRESULT hres = DirectPlay->EnumSessions(&dpDesc, 0, EnumSessionsCallback, NULL,
//		((DirectPlayGamesTime == -1000) ? 0 : (DirectPlayGamesAkt.IsEmpty() ? DPENUMSESSIONS_ASYNC : DPENUMSESSIONS_STOPASYNC))
		(DirectPlayGamesAkt.IsEmpty() ? DPENUMSESSIONS_ASYNC : DPENUMSESSIONS_STOPASYNC)
		| DPENUMSESSIONS_ALL);

// nový èítaè platnosti seznamu
	if (hres == DPERR_USERCANCEL)
	{
// tato hodnota se použije též k rozlišení pøerušení z funkce pro vytvoøení hry
		DirectPlayGamesTime = 5*18;			// pøerušeno uživatelem
	}
	else
	{
		if (DirectPlayGamesNum == 0)
		{
			DirectPlayGamesTime = 2;		// neúspìch - hned opìt nový pokus
		}
		else
		{
			DirectPlayGamesTime = 9;		// pøi úspìchu aktualizace opìt za chvíli
		}
	}

// doplnìní aktivní hry, pokud nebyla nalezena
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


// ----------------------- obsluha seznamu spojení --------------------------

/////////////////////////////////////////////////////////////////////////////
// vypnutí aktivního spojení

void SetConnectActNul()
{
// test, zda je otevøeno nìjaké spojení
	if (DirectPlayConnectsAktN >= 0)
	{
// zrušení pøíznaku otevøení spojení
		DirectPlayConnectsAktN = -1;
		DirectPlayConnectsAkt.Empty();

// uvolnìní objektu DirectPlay
		DirectPlayUninit();
	}
}


/////////////////////////////////////////////////////////////////////////////
// vyhledání podøetìzce v seznamu spojení (vše konvertováno na velká písmena)
// i ...... index nalezeného spojení
// txt .... zadané jméno spojení
// sub .... hledaný podøetìzec

void GetConnectSubString(int& i, CString& txt, CString sub)
{
// test, zda spojení zatím nebylo nalezeno; test, zda zadaný text obsahuje podøetìzec
	if ((i == DirectPlayConnectsNum) && (txt.Find(sub) >= 0))
	{
		CString txt2;

// cyklus pøes všechna spojení
		for (i = 0; i < DirectPlayConnectsNum; i++)
		{

// naètení textu jednoho spojení
			txt2 = DirectPlayConnects.GetLine(i);

// konverze na velká písmena
			txt2.UpperCase();

// test, zda obsahuje podøetìzec
			if (txt2.Find(sub) >= 0) break;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastavení aktivního spojení (prázdný text = zrušení)

void SetConnectAct(CString txt)
{
// korekce textu
	txt.TrimLeft();
	txt.TrimRight();

// prázdný text - zrušení spojení
	if (txt.IsEmpty())
	{
		SetConnectActNul();
		return;
	}

// inicializace seznamu spojení
	if (!DirectPlayConInit()) return;

// nalezení shodného textu
	int i;
	for (i = 0; i < DirectPlayConnectsNum; i++)
	{
		if (DirectPlayConnects.GetLine(i) == txt) break;
	}

// hledání bez rozlišení velká/malá písmena
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

// hledání podøetìzcù
		GetConnectSubString(i, txt, _T("IPX"));
		GetConnectSubString(i, txt, _T("TCP"));
		GetConnectSubString(i, txt, _T("MODEM"));

// ještì pokus o hledání textu "serial"/"sériov"/"seriov"
		if (i == DirectPlayConnectsNum)
		{
			CString s1("SERIAL");
			CString s2("SÉRIOV");
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

// zadán neplatný øetìzec - vypnutí spojení
	if (i == DirectPlayConnectsNum)
	{
		SetConnectActNul();
		return;
	}

// inicializace spojení (jen je-li zmìnìno)
	if (i != DirectPlayConnectsAktN)
	{

// nulování èítaèù pro aktualizaci informací
		DirectPlayGamesTime = 0;
		DirectPlayPlayersTime = 0;

// vypnutí starého spojení
		SetConnectActNul();

// inicializace seznamu spojení (a kontrola platnosti indexu)
		if (!DirectPlayConInit() || (i >= DirectPlayConnectsNum))
		{
			SetConnectActNul();
			return;
		}

// inicializace spojení
		HRESULT hres = DirectPlay->InitializeConnection(DirectPlayConnectsConn[i], 0);

// kontrola úspìšnosti spojení
		if ((hres != DP_OK) && (hres != DPERR_ALREADYINITIALIZED)) return;
	}

// úschova aktivního spojení
	DirectPlayConnectsAkt = DirectPlayConnects.GetLine(i);
	DirectPlayConnectsAktN = i;
}


/////////////////////////////////////////////////////////////////////////////
// callback pro enumeraci spojení

BOOL FAR PASCAL EnumConnectionsCallback(
  LPCGUID lpguidSP,
  LPVOID lpConnection,
  DWORD dwConnectionSize,
  LPCDPNAME lpName,
  DWORD dwFlags,
  LPVOID lpContext
  )
{
// pøíprava textu jména spojení
	CString txt(lpName->lpszShortNameA);
	if (txt.IsEmpty()) txt = lpName->lpszLongNameA;
	txt.TrimLeft();
	txt.TrimRight();

// zrušení textu "pro DirectPlay"
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

// zajištìní jedineènosti jména
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

// pøidání textu k seznamu
	DirectPlayConnects += txt;

// pøidání konce øádku
	DirectPlayConnects += _T(13);
	DirectPlayConnects += _T(10);

// zvýšení èítaèe spojení
	int i = DirectPlayConnectsNum;
	DirectPlayConnectsNum++;
	int i2 = DirectPlayConnectsNum;

// úschova dat spojení
	DirectPlayConnectsConn = (void**)MemSize(DirectPlayConnectsConn, i2 * sizeof(void*));
	DirectPlayConnectsConn[i] = (void*)MemGet(dwConnectionSize);
	MemCopy(DirectPlayConnectsConn[i], lpConnection, dwConnectionSize);

// pokraèovat v enumeraci
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// inicializace seznamu spojení (FALSE=chyba)

bool DirectPlayConInit()
{
// test,zda je seznam již inicializován
	if (DirectPlayConnectsNum > 0) return true;

// test, zda je možný nový pokus o otevøení
	if (NextDirectPlayInit > 0) return false;
	NextDirectPlayInit = 36;

// inicializace objektu DirectPlay
	if (!DirectPlayInit()) return false;

// zjištìní seznamu možných spojení
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
// uvolnìní seznamu spojení (ukonèí probíhající hru)

void DirectPlayConUninit()
{
// test, zda jsou v seznamu nìjaká spojení
	if (DirectPlayConnectsNum > 0)
	{

// vypnutí aktivní hry
		SetGameAct(EmptyString);

// zrušení seznamu her
		DirectPlayGames.Empty();
		DirectPlayGamesNum = 0;

// uvolnìní informací o spojeních
		for (int i = 0; i < DirectPlayConnectsNum; i++)
		{
			MemFree(DirectPlayConnectsConn[i]);
		}

// zrušení seznamu spojení
		DirectPlayConnects.Empty();
		DirectPlayConnectsNum = 0;

// zrušení aktivního spojení
		DirectPlayConnectsAkt.Empty();
		DirectPlayConnectsAktN = -1;
	}
}


// --------------------- obsluha objektu DirectPlay -------------------------

/////////////////////////////////////////////////////////////////////////////
// inicializace objektu DirectPlay (FALSE=chyba)

bool DirectPlayInit()
{
// test, zda je objekt DirectPlay již vytvoøen
	if (DirectPlay != NULL) return true;

// inicializace COM knihovny
	if (!COMInit()) return false;

// vytvoøení rozhranní DirectPlay3A
	if ((::CoCreateInstance(CLSID_DirectPlay, NULL, CLSCTX_INPROC_SERVER,
			IID_IDirectPlay3A, (void**)&DirectPlay) != DP_OK) || (DirectPlay == NULL))
	{
		DirectPlay = NULL;
		return false;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// uvolnìní objektu DirectPlay (nutné pøi zmìnì spojení nebo hry)

void DirectPlayUninit()
{
// test, zda je objekt DirectPlay vytvoøen
	if (DirectPlay != NULL)
	{

// uvolnìní seznamu spojení
		DirectPlayConUninit();

// uvolnìní objektu DirectPlay
		DirectPlay->Release();
		DirectPlay = NULL;
	}
}


// ------------------------ obsluha knihovny COM ----------------------------

/////////////////////////////////////////////////////////////////////////////
// inicializace knihovny COM (FALSE=chyba) (zùstává inicializována po celou dobu bìhu programu)

bool COMInit()
{
// test, zda je knihovna COM již inicializována
	if (Coinit) return true;

// inicializace knihovny COM
	HRESULT res = ::CoInitialize(NULL);

// kontrola operace
	if ((res != S_OK) && (res != S_FALSE)) return false;

// zapnutí pøíznaku inicializace knihovny
	Coinit = true;
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// uvolnìní knihovny COM (pøi ukonèení programu)

void COMUninit()
{
// test, zda je knihovna COM inicializována
	if (Coinit)
	{

// uvolnìní pøípadného objektu DirectPlay
		DirectPlayUninit();

// uvolnìní knihovny COM
		::CoUninitialize();

// vypnutí pøíznaku inicializace knihovny
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
	// test, zda inicializace již probìhla
	if (!Wsaload)
	{
		// pøíznak pokusu o naètení knihovny
		Wsaload = true;

		// naètení knihovny WSA (verze 1 nebo 2)
		WSALibrary = ::LoadLibrary(_T("WS2_32.DLL"));	// verze 2

		if (WSALibrary == NULL)
		{
			WSALibrary = ::LoadLibrary(_T("WSOCK32.DLL"));		// verze 1
				 
		}

		// import funkcí knihovny WSA 1.1 pro DirectPlay
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

					// import funkcí knihovny WSA 2.2 pro UDP
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
// inicializace WSA 1.1 knihovny (vrací TRUE je-li OK)

bool WSAInitialize1()
{
	WSAInitialize();
	return Wsainit;
}

/////////////////////////////////////////////////////////////////////////////
// inicializace WSA 2.2 knihovny (vrací TRUE je-li OK)

bool WSAInitialize2()
{
	WSAInitialize();
	return Wsainit2;
}

/////////////////////////////////////////////////////////////////////////////
// inicializace obsluhy UDP (inicializace bufferù)

void InitUDP()
{
	// vysílací adresa UDP (INADDR_BROADCAST = (u_long)0xffffffff)
	MemFill(&UDPSendAddr, sizeof(SOCKADDR_IN), 0);
	UDPSendAddr.sin_family = AF_INET;
	UDPSendAddr.sin_port = 0x8813;			// port 5000
	UDPSendAddr.sin_addr.s_addr = 0x0100007f;	// loop IP adresa INADDR_LOOPBACK "127.0.0.1"

	// overlapped struktura vysílaèe
	UDPSendOver.hEvent = NULL;

	// odesílací buffer UDP
	UDPSendBuf.buf = (char*)MemGet(16384);	// alokace bufferu

	// pøijímací adresa UDP
	MemFill(&UDPRecAddr, sizeof(SOCKADDR_IN), 0);
	UDPRecAddr.sin_family = AF_INET;
	UDPRecAddr.sin_port = 0x8813;			// port 5000
	UDPRecAddr.sin_addr.s_addr = 0;			// IP adresa INADDR_ANY = "0.0.0.0"

	// pøijatá adresa UDP
	MemFill(&UDPRecRecAddr, sizeof(SOCKADDR_IN), 0);
	UDPRecRecAddr.sin_family = AF_INET;
	UDPRecRecAddr.sin_port = 0;
	UDPRecRecAddr.sin_addr.s_addr = 0xffffffff; // IP adresa INADDR_NONE

	// overlapped struktura pøijímaèe
	UDPRecOver.hEvent = NULL;

	// pøijímací buffer UDP
	UDPRecBuf.len = 512;					// velikost paketu
	UDPRecBuf.buf = (char*)MemGet(16384);	// alokace bufferu
}

/////////////////////////////////////////////////////////////////////////////
// otevøení vysílacího soketu UDP (pøi chybì vrací FALSE)

bool OpenSendUDP()
{
	// test, zda je soket otevøený
	if (UDPSendSocket == INVALID_SOCKET)
	{
		// inicializace WSA
		if (WSAInitialize2())
		{
			// vytvoøení události overlapped vysílaèe
			UDPSendOver.hEvent = pWSACreateEvent();
			if (UDPSendOver.hEvent != NULL)
			{
				// otevøení vysílacího UDP soketu
				UDPSendSocket = pWSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
				if (UDPSendSocket != INVALID_SOCKET)
				{
					// nastavení timeout vysílaèe
					int par = 50;
					pWSASetSockOpt(UDPSendSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&par, sizeof(par));

					// sdílení použití adres
					par = TRUE;
					pWSASetSockOpt(UDPSendSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&par, sizeof(par));

					// povolen broadcast pøenos
					par = TRUE;
					pWSASetSockOpt(UDPSendSocket, SOL_SOCKET, SO_BROADCAST, (char*)&par, sizeof(par));
				}
			}
		}
	}
	return (UDPSendSocket != INVALID_SOCKET);
}

/////////////////////////////////////////////////////////////////////////////
// zavøení vysílacího soketu UDP

void CloseSendUDP()
{
	// zavøení vysílacího UDP soketu
	if (UDPSendSocket != INVALID_SOCKET)
	{
		pWSASetEvent(UDPSendOver.hEvent);
		pWSACloseSocket(UDPSendSocket);
		UDPSendSocket = INVALID_SOCKET;
	}

	// zrušení události overlapped vysílaèe
	if (UDPSendOver.hEvent != NULL)
	{
		pWSACloseEvent(UDPSendOver.hEvent);
		UDPSendOver.hEvent = NULL;
	}

	UDPSending = false;
}


/////////////////////////////////////////////////////////////////////////////
// otevøení pøijímacího soketu UDP (pøi chybì vrací FALSE)

bool OpenRecUDP()
{
	// test, zda je soket otevøený
	if (UDPRecSocket == INVALID_SOCKET)
	{
		// inicializace WSA
		if (WSAInitialize2())
		{
			// vytvoøení události overlapped pøijímaèe
			UDPRecOver.hEvent = pWSACreateEvent();
			if (UDPRecOver.hEvent != NULL)
			{
				// otevøení pøijímacího UDP soketu
				UDPRecSocket = pWSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
				if (UDPRecSocket != INVALID_SOCKET)
				{
					// nastavení time-out pøijímaèe
					int par = 1000;
					pWSASetSockOpt(UDPRecSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&par, sizeof(par));

					// velikost pøijímacího bufferu
					par = UDPRecBuf.len;
					pWSASetSockOpt(UDPRecSocket, SOL_SOCKET, SO_RCVBUF, (char*)&par, sizeof(par));

					// sdílení použití adres
					par = TRUE;
					pWSASetSockOpt(UDPRecSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&par, sizeof(par));

					// povolen broadcast pøenos
					par = TRUE;
					pWSASetSockOpt(UDPRecSocket, SOL_SOCKET, SO_BROADCAST, (char*)&par, sizeof(par));

					// pojmenování soketu pøijímaèe
					pWSABind(UDPRecSocket, (sockaddr*)&UDPRecAddr, sizeof(SOCKADDR_IN));

					// zahájení pøíjmu
					StartRecUDP();
				}
			}
		}
	}
	return (UDPRecSocket != INVALID_SOCKET);
}

/////////////////////////////////////////////////////////////////////////////
// zavøení pøijímacího soketu UDP

void CloseRecUDP()
{
	// zavøení pøijímacího UDP soketu
	if (UDPRecSocket != INVALID_SOCKET)
	{
		pWSASetEvent(UDPRecOver.hEvent);
		pWSACloseSocket(UDPRecSocket);
		UDPRecSocket = INVALID_SOCKET;
	}

	// zrušení události overlapped pøijímaèe
	if (UDPRecOver.hEvent != NULL)
	{
		pWSACloseEvent(UDPRecOver.hEvent);
		UDPRecOver.hEvent = NULL;

	}
	UDPReceiving = false;
	UDPRecData = 0;
}

/////////////////////////////////////////////////////////////////////////////
// odeslání UDP paketu

void SendUDP(const void* data, int len)
{
	if (WSAInitialize2())
	{
		if (OpenSendUDP())
		{
			int res;
			DWORD send;
			DWORD flags;

			// test, zda minulé vysílání bylo ukonèeno
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

			// kopie dat do vysílacího bufferu
			if (len > 16384) len = 16384;
			if (len <= 0) return;
			UDPSendBuf.len = len;
			MemCopy(UDPSendBuf.buf, data, len);

			// odeslání paketu
			res = pWSASendTo(UDPSendSocket, &UDPSendBuf, 1, &send, 0, (SOCKADDR*)&UDPSendAddr,
									sizeof(SOCKADDR_IN), &UDPSendOver, NULL);

			// paket je odesílán na pozadí
			if (res == SOCKET_ERROR)
			{
				if (pWSAGetLastError() == WSA_IO_PENDING) UDPSending = true;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// start pøíjmu UDP paketu

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

				// probíhá pøíjem
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

				// zahájení nového pøíjmu
				rec = 0;
				flags = 0;
				int fromlen = sizeof(SOCKADDR_IN);
				res = pWSARecvFrom(UDPRecSocket, &UDPRecBuf, 1, &rec, &flags, (SOCKADDR*)&UDPRecRecAddr,
					&fromlen, &UDPRecOver, NULL);

				// paket je odesílán na pozadí
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
// pøíjem UDP paketu (vrací velikost dat v pøijímacím bufferu)

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
	// ukonèení UDP
	CloseSendUDP();
	CloseRecUDP();

	// uvolnìní knihovny WSA
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
// --------------------- konec vypnutí pro MINI verzi -------------------


/***************************************************************************\
*																			*
*									Dialogy									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// režim dialogu

void DialogOn(bool on)
{
	if (on == DialogMode) return;
	DialogMode = on;

	::InvalidateRect(MainFrame, NULL, TRUE);
	::UpdateWindow(MainFrame);
}


/////////////////////////////////////////////////////////////////////////////
// vytvoøení nového okna

void WindowNew(int typ, int styl)
{
// vypnutí celoobrazovkového módu
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
		WS_EX_CONTROLPARENT | WS_EX_DLGMODALFRAME,		// rozšíøený styl okna
		MainFrameClass,									// jméno tøídy okna
		NULL,											// titulek okna
		WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_OVERLAPPED |
			WS_CLIPSIBLINGS	/*| WS_MINIMIZEBOX */| styl,	// styl okna
		item->Left,										// X
		item->Top,										// Y
		item->Width,									// šíøka
		item->Height,									// výška
		parent->Wnd,									// rodiè
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
// vytvoøení fontu (nutné uvolnìní pomocí FreeFont)

HFONT GetFont(bool bold, bool italic, bool underline, bool strikeout, bool serif, bool fixed, int height, double width, double angle)
{
// sada znakù
	DWORD charset = CharSet;
	if (charset == DefCharSet) charset = DEFAULT_CHARSET;

// korekce výšky
	if (height < 0) height = -height;

// nalezení existujícího fontu v tabulce
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

// skuteèná výška znakù
	int height0 = height;
	if (height <= 0) height0 = 16;

// pøíprava šíøky fontu
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

// pøepoèet úhlu na desetiny stupnì
	int angle0 = Round(angle/pi*1800) % 3600;

// vytvoøení fontu
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

// druhý pokus, není-li pøítomna správná znaková sada
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

// pøi neúspìchu náhradní font
	if (font == NULL) font = FontDefault;
	if (font == NULL) return NULL;

// nalezení volné položky k uložení fontu
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

// uložení informací o fontu
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

// ukazatel pøíští položky
	FontNext++;
	if (FontNext >= FONTTABSIZE) FontNext = FONTTABSTD;

// návrat fontu
	return font;
}


/////////////////////////////////////////////////////////////////////////////
// uvolnìní fontu pøidìleného s GetFont

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
// vytvoøení prvku okna

void WindowItemNew(int typ, CString& classname, int width, int height, int style, int exstyle)
{
// vytvoøení nového prvku
	WindowID = Win.New();
	WINITEM* item = &Win[WindowID];

// zapnutí dialogového módu hlavního okna
	if (DialogParent == 0) DialogOn(true);

// adresa rodièe
	WINITEM* parent = &Win[DialogParent];

// zaøazení do øetìzce prvkù
	item->Prev = parent->Prev;
	item->Next = DialogParent;
	parent->Prev = WindowID;
	Win[item->Prev].Next = WindowID;

// inicializace parametrù
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

// vytvoøení prvku
	item->Wnd = ::CreateWindowEx(
		0 | exstyle,					// rozšíøený styl
		classname,						// tøída
		NULL,							// text
		WS_CHILD | style,				// styl
		item->Left,						// X
		item->Top,						// Y
		width,							// šíøka
		height,							// výška
		parent->Wnd,					// rodiè
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

// nastavení fontu
	WindowSetFont(item);

// zjištìní klientské šíøky a výšky
	InitClientRect(item);

// zamìøení fokusu
	if (WindowFocus == DialogParent) FocusNext();
}


/////////////////////////////////////////////////////////////////////////////
// zrušení prvku

void WindowDel(int inx)
{
// nelze zrušit hlavní okno
	if (inx == 0) return;

// zrušení èísla aktuálního prvku
	if (WindowID == inx) WindowID = -1;

// okno musí být platné
	if (Win.IsNotValid(inx)) return;

// adresa prvku
	WINITEM* item = &Win[inx];

// rušení evidence prvku okna v rodièovském oknì
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

// zrušení okna vèetnì jeho potomkù
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

// zrušení položky okna
	Win.Del(inx);

// po zrušení všech prvkù návrat focusu na hlavní okno
	if (Win.Num() == 1)
	{
		::SetFocus(MainFrame);
	}
}


/////////////////////////////////////////////////////////////////////////////
// posun fokusu na další prvek

void FocusNext()
{
// výchozí prvek
	int foc = WindowFocus;
	if (Win.IsNotValid(foc)) foc = DialogParent;
	int foc0 = foc;

// nalezení dalšího prvku
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

// korekce pro pøepínaè
	if (Win[foc].Typ == WINITEM_BUTTONRADIO2)
	{
		foc0 = foc;

		do 
		{
			if (Win[foc0].Check && !Win[foc0].Disable) foc = foc0;
			foc0 = Win[foc0].Next;

		} while (Win[foc0].Typ == WINITEM_BUTTONRADIO);
	}

// nastavení fokusu na prvek
	WindowFocus = foc;
	::SetFocus(Win[foc].Wnd);
}


/////////////////////////////////////////////////////////////////////////////
// posun fokusu na pøedchozí prvek

void FocusPrev()
{
// výchozí prvek
	int foc = WindowFocus;
	if (Win.IsNotValid(foc)) foc = DialogParent;
	int foc0 = foc;

// posun na zaèátek pøepínaèù
	while (Win[foc].Typ == WINITEM_BUTTONRADIO)
	{
		foc = Win[foc].Prev;
	}

// nalezení pøedchozího prvku
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

// korekce pro pøepínaè
	if (Win[foc].Typ == WINITEM_BUTTONRADIO2)
	{
		foc0 = foc;

		do
		{
			if (Win[foc0].Check && !Win[foc0].Disable) foc = foc0;
			foc0 = Win[foc0].Next;

		} while (Win[foc0].Typ == WINITEM_BUTTONRADIO);
	}

// nastavení fokusu na prvek
	WindowFocus = foc;
	::SetFocus(Win[foc].Wnd);
}


/////////////////////////////////////////////////////////////////////////////
// posun fokusu na další pøepínaè

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
// posun fokusu na pøedchozí pøepínaè

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
// nastavení pøepínaèe

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
// aktualizace údajù o joysticku

void AktJoystick(int inx)
{
// test, zda jsou údaje platné
	if (JoystickValid[inx]) return;
	JoystickValid[inx] = true;

// test, zda je povolena další aktualizace
	if (JoystickNext[inx] >= 0) return;

// naètení informací o joysticku
	JOYCAPS jp;
	JOYINFO ji;

	if ((::joyGetDevCaps(inx, &jp, sizeof(JOYCAPS)) != JOYERR_NOERROR) ||
		(::joyGetPos(inx, &ji) != JOYERR_NOERROR))
	{

// pøi chybì vynulování parametrù
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

// souøadnice X
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

// souøadnice Y
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

// souøadnice Z
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

// tlaèítka
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
// zapnutí/vypnutí konzoly

void ConsoleOnSet(bool on)
{
	if (ConsoleOn == on) return;
	ConsoleOn = on;

// zapnutí konzoly
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
// výstup textu na konzolu

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
// výstup chybového textu na konzolu

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
*							obsluha CD diskù								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// zahájení pøehrávání od zvolené stopy

void CDPlay(int stopa)
{
// aktualizace údajù o CD
	if (CDAkt())
	{

// korekce èísla stopy
		if (CDTracks > 0)
		{
			while (stopa < 1) stopa += CDTracks;
			while (stopa > CDTracks) stopa -= CDTracks;
		}

// nastavení formátu stop
		MCI_SET_PARMS mcis;
		mcis.dwTimeFormat = MCI_FORMAT_TMSF;
		::mciSendCommand(CDDevice, MCI_SET, MCI_SET_TIME_FORMAT | MCI_WAIT, (DWORD)&mcis);

// zahájení pøehrávání
		MCI_PLAY_PARMS mcip;
		mcip.dwFrom = stopa;
		::mciSendCommand(CDDevice, MCI_PLAY, MCI_FROM, (DWORD)&mcip);

// zneplatnìní údajù o CD
		CDValidTime = 0;
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastavení stopy

void CDSetTrack(int stopa)
{
// aktualizace údajù o CD
	if (CDAkt())
	{

// korekce èísla stopy
		if (CDTracks > 0)
		{
			while (stopa < 1) stopa += CDTracks;
			while (stopa > CDTracks) stopa -= CDTracks;
		}

// nastavení formátu stop
		MCI_SET_PARMS mcis;
		mcis.dwTimeFormat = MCI_FORMAT_TMSF;
		::mciSendCommand(CDDevice, MCI_SET, MCI_SET_TIME_FORMAT | MCI_WAIT, (DWORD)&mcis);

// nastavení stopy
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

// zneplatnìní údajù o CD
		CDValidTime = 0;
	}
}


/////////////////////////////////////////////////////////////////////////////
// zastavení pøehrávání CD

void CDStop()
{
// aktualizace údajù o CD
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
// vysunutí CD

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
// aktualizace údajù o CD
	if (CDAkt())
	{

// zapauzování pøehrávání
		if (CDPlaing)
		{
			::mciSendCommand(CDDevice, MCI_PAUSE, MCI_WAIT, NULL);
			CDValidTime = 0;
		}
		else
		{

// odpauzování pøehrávání
			if (CDPausing)
			{
				::mciSendCommand(CDDevice, MCI_PLAY, 0, NULL);
				CDValidTime = 0;
			}
		}
 	}
}


/////////////////////////////////////////////////////////////////////////////
// nastavení pozice CD v ms

void CDSetPos(int pozice)
{
// aktualizace údajù o CD
	if (CDAkt())
	{

// nastavení formátu na milisekundy
		MCI_SET_PARMS mcis;
		mcis.dwTimeFormat = MCI_FORMAT_MILLISECONDS;
		::mciSendCommand(CDDevice, MCI_SET, MCI_SET_TIME_FORMAT | MCI_WAIT, (DWORD)&mcis);

// korekce pozice
		if (CDLength > 0)
		{
			while (pozice < 0) pozice += CDLength;
			pozice = pozice % CDLength;
		}

// nastavení pozice pøehrávání
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

// zneplatnìní údajù o CD
		CDValidTime = 0;
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastavení pozice v aktivní stopì CD v ms

void CDSetTrackPos(int pozice)
{
// aktualizace údajù o CD
	if (CDAkt())
	{

// nastavení formátu na stopu
		MCI_SET_PARMS mcis;
		mcis.dwTimeFormat = MCI_FORMAT_TMSF;
		::mciSendCommand(CDDevice, MCI_SET, MCI_SET_TIME_FORMAT | MCI_WAIT, (DWORD)&mcis);

// korekce pozice
		if (pozice >= CDTrackLength) pozice = CDTrackLength - 1;
		if (pozice < 0) pozice = 0;

// výpoèet èasového údaje pozice
		int frame = ((pozice % 1000) * 75 + 500)/1000;
		int sekunda = (pozice / 1000) % 60;
		int minuta = pozice / 60000;

// nastavení pozice pøehrávání
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

// zneplatnìní údajù o CD
		CDValidTime = 0;
	}
}


/////////////////////////////////////////////////////////////////////////////
// uzavøení ovladaèe CD

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
// otevøení ovladaèe CD

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
// aktualizace údajù o CD disku

bool CDAkt()
{
// kontrola, zda je potøeba údaje aktualizovat
	if (CDValidTime > 0) return true;

// vynulování ukazatelù
	if (!CDDiskValid)
	{
		CDStart = 2000;				// startovací pozice média
		CDLength = CDDEFLENGTH;		// délka média v ms
		CDTracks = 1;				// poèet stop média
		CDTrack = 1;				// pøehrávaná stopa
		CDTrackLength = CDDEFLENGTH;// délka pøehrávané stopy
	}
	CDPos = 0;						// pøehrávaná pozice
	CDTrackPos = 0;					// pozice v pøehrávané stopì
	CDPlaing = false;					// neprobíhá pøehrávání
	CDPausing = false;				// není pauza

// otevøení zaøízení
	if (!CDOpen()) return false;
	CDValidTime = CDAKTTIME;

// nastavení formátu na milisekundy
	MCI_SET_PARMS mcis;
	mcis.dwTimeFormat = MCI_FORMAT_MILLISECONDS;
	::mciSendCommand(CDDevice, MCI_SET, MCI_SET_TIME_FORMAT | MCI_WAIT, (DWORD)&mcis);

// naètení pøehrávané pozice CD
	MCI_STATUS_PARMS mcit;
	mcit.dwItem = MCI_STATUS_POSITION;
	mcit.dwReturn = CDStart;
	int result = ::mciSendCommand(CDDevice, MCI_STATUS, MCI_STATUS_ITEM | MCI_WAIT, (DWORD)&mcit);
	mcit.dwReturn -= CDStart;
	if ((int)mcit.dwReturn < 0) mcit.dwReturn = 0;
	if (result == 0) CDPos = mcit.dwReturn;

// naètení aktuálního stavu
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

// test, zda je potøeba aktualizovat informace o vloženém CD disku
	if (!CDDiskValid)
	{
		CDDiskValid = true;

// naètení startovací pozice CD (formát je nastaven na milisekundy)
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

// naètení délky média
		mcit.dwItem = MCI_STATUS_LENGTH;
		result = ::mciSendCommand(CDDevice, MCI_STATUS, MCI_STATUS_ITEM | MCI_WAIT, (DWORD)&mcit);
		if ((result == 0) && ((int)mcit.dwReturn > 0))
		{
			CDLength = mcit.dwReturn;
			if (CDLength > CDMAXLENGTH) CDLength = CDMAXLENGTH;
		}

// naètení poètu stop média
		mcit.dwItem = MCI_STATUS_NUMBER_OF_TRACKS;
		result = ::mciSendCommand(CDDevice, MCI_STATUS, MCI_STATUS_ITEM | MCI_WAIT, (DWORD)&mcit);
		if ((result == 0) && ((int)mcit.dwReturn > 0))
		{
			CDTracks = mcit.dwReturn;
			if (CDTracks > CDMAXTRACKS) CDTracks = CDMAXTRACKS;
		}

// neplatné informace o aktivní stopì
		CDTrack = 0;
	}

// nastavení formátu na stopy
	mcis.dwTimeFormat = MCI_FORMAT_TMSF;
	::mciSendCommand(CDDevice, MCI_SET, MCI_SET_TIME_FORMAT | MCI_WAIT, (DWORD)&mcis);

// naètení pøehrávané pozice CD ve stopì (sekunda = 75 frame)
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

// pøíprava èísla aktuální stopy
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

// naètení délky pøehrávané stopy, pokud se stopa zmìnila
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

// korekce nìkterých údajù
	if (CDTrackLength > CDLength) CDTrackLength = CDLength;
	if (CDTrackPos > CDTrackLength) CDTrackPos = CDTrackLength;
	if (CDPos > CDLength) CDPos = CDLength;
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// obsluha CD na pozadí

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
*							obsluha souborù									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// zjištìní informací o velikosti a volném místu aktivního disku

void GetDiskSpace()
{
// naètení informací novìjší funkcí
	if (!pGetDiskFreeSpaceEx ||
		!pGetDiskFreeSpaceEx(NULL, &DiskFreeUser, &DiskSize, &DiskFree))

// naètení informací starší metodou
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
// otevøení souboru pro ètení

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
// otevøení souboru pro zápis

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
// vyprázdnìní zápisového bufferu

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
// zápis bajtu do výstupního souboru

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
// ètení bajtu ze vstupního souboru

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
//naètení bloku ze souboru

void FileReadBlok(BYTE* buf, int size)
{
// otevøení souboru
	if (!FileReadOpen()) return;

// kontrola velikosti dat
	if (size <= 0)
	{
		FileReadOff += size;
		return;
	}

// naètení velkého bloku dat
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

// pøenos bloku dat z bufferu
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
		
// naètení dalšího bloku dat do bufferu
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
// uzavøení souborù

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
// pøíprava jména ALIAS (musí být oøezané mezery ze zaèátku a konce!)

void _fastcall InitAlias(CString name)
{
// odstranìní "\" z konce
	if (name.LastChar() == _T('\\')) name.Delete(name.Length() - 1);

// nalezení konce cesty
	int pos = name.RevFind(_T('\\')) + 1;
	if (pos == 0) pos = name.RevFind(_T(':')) + 1;

// jméno klíèe a ALIASES
	AliasKey = name.Right(name.Length() - pos);
	AliasName = name.Left(pos) + _T("ALIASES.INI");

// korekce na plné jméno souboru
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
*							provádìní programu								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// inicializace výstupního bufferu

void InitDispBuf()
{
// pøíznak, zda se používá výstupní buffer
	DispBufUse = ((DispWidth != Width) || (DispHeight != Height));
	DispBufUse2 = (((2*DispWidth) == Width) && ((2*DispHeight) == Height));

	if (DispBufUse)
	{

// pøíprava velikosti výstupního bufferu
		int oldsize = DispBufSize;				// pùvodní velikost bufferu
		DispBufSize = DispWidthByte * DispHeight;	// velikost bufferu v bajtech
		if (DispBufUse2) DispBufSize *= 4;		// korekce pro TRUE COLOR

// zvìtšení bufferu
		if (DispBufSize > oldsize)
		{
			MemFree(DispBuf);
			DispBuf = (BYTE*)MemGet(DispBufSize * sizeof(BYTE));
		}

// buffer mapování bodu obrázku na výstupní bod
		int i;
		MemFree(DispMapX);
		DispMapX = (int*)MemGet(Width * sizeof(int));

		for (i = 0; i < Width; i++)
		{
			int off = RoundM(i * Meritko);
			if (off >= DispWidth) off = DispWidth - 1;
			DispMapX[i] = off;
		}

// buffer mapování linky obrázku na výstupní linku
		MemFree(DispMapY);
		DispMapY = (int*)MemGet(Height * sizeof(int));

		for (i = 0; i < Height; i++)
		{
			int off = RoundM(i * Meritko);
			if (off >= DispHeight) off = DispHeight - 1;
			DispMapY[i] = off;
		}

// buffer zpìtného mapování výstupního bodu na offset v lince obrázku
		MemFree(DispRemapX);
		DispRemapX = (int*)MemGet(DispWidth * sizeof(int));

		for (i = 0; i < DispWidth; i++)
		{
			int off = RoundM(i / Meritko);
			if (off >= Width) off = Width - 1;
			DispRemapX[i] = off;
		}

// buffer zpìtného mapování výstupní linky na offset v obrázku
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
// inicializace informací o ploše

void ExecInitMap()
{
	int oldwidth = Width;
	int oldheight = Height;
	int oldsize = WidthByte * oldheight;

	CMap* map = &Map[0];
	WidthN = map->Width();				// šíøka plochy v ikonách
	Width = WidthN * ICONWIDTH;			// šíøka plochy v bodech
	WidthByte = (Width + 3) & ~3;		// délka linky v bajtech
	RowByte = WidthByte * ICONHEIGHT;	// délka øádku ikon v bajtech
	HeightN = map->Height();			// výška plochy v ikonách
	Height = HeightN * ICONHEIGHT;		// výška plochy v bodech

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
// --------------------- konec vypnutí pro MINI verzi -------------------
}


/////////////////////////////////////////////////////////////////////////////
// nastavování rozmìrù hlavního okna

void OnSizing(RECT* rc, int side)
{
// v dialogovém módu není omezení
	if (DialogMode && !DialogGraph) return;

// naètení polohy a rozmìrù okna
	RECT wrc;
	::GetWindowRect(MainFrame, &wrc);

// naètení klientských souøadnic okna
	RECT crc;
	::GetClientRect(MainFrame, &crc);

// korekce o výšku stavové lišty
	if (StatusVisible)
	{
		RECT src;
		::GetWindowRect(StatusBar, &src);
		crc.bottom -= src.bottom - src.top;
	}

// výpoèet nastavené šíøky a výšky klientské oblasti
	int width = (rc->right - rc->left) - ((wrc.right - wrc.left) - (crc.right - crc.left));
	int height = (rc->bottom - rc->top) - ((wrc.bottom - wrc.top) - (crc.bottom - crc.top));

// urèení nového mìøítka
	double meritko = ((double)width/(double)Width + (double)height/(double)Height)/2;

// zarovnání mìøítka
	if (!FullScreen)
	{
		meritko = (double)Round(MODULMERITKA*meritko)/MODULMERITKA;
	}
	else
	{
		meritko = (double)Round(MODULMERITKA2*meritko)/MODULMERITKA2;
	}
	if (meritko < (double)1/ICONWIDTH) meritko = (double)1/ICONWIDTH;

// výpoèet zmìny rozmìrù 
	width -= (int)(Width*meritko + 0.01);
	height -= (int)(Height*meritko + 0.01);

// bude zmenšení šíøky
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

// bude zmenšení výšky
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
// aktualizace rozmìrù okna

void _fastcall OnSize(WINITEM* item)
{
// pøesun stavového øádku
	if ((item == &Win[0]) && !FullScreen)
	{
		::SendMessage(StatusBar, WM_SIZE, 0, 0);
	}

// inicializace klientských rozmìrù
	InitClientRect(item);

// urèení nového mìøítka
	if (item == &Win[0])
	{
		Meritko = (double)(ClientWidth+4) / (double)Width;
		double m = (double)(ClientHeight+4) / (double)Height;
		if (m < Meritko) Meritko = m;

// zarovnání mìøítka
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

// pøíprava rozmìrù k zobrazení
		int oldwidth = DispWidth;
		int oldheight = DispHeight;

		DispWidth = Round(Width*Meritko);
		DispWidthByte = (DispWidth + 3) & ~3;
		DispHeight = Round(Height*Meritko);
		DispLeft = (ClientWidth - DispWidth)/2;
		DispTop = (ClientHeight - DispHeight)/2;

// aktualizace bufferu zobrazení
		if ((oldwidth != DispWidth) || (oldheight != DispHeight))
		{
			InitDispBuf();
		}
	}

// pøekreslení okna
	RePaint(item);
}


/////////////////////////////////////////////////////////////////////////////
// nastavení požadavku pøekreslení všech ikon

void AktAllIcon()
{
	MemFill(AktBuf, HeightN*WidthN*sizeof(bool), true);
}


/////////////////////////////////////////////////////////////////////////////
// nastavení požadavku pøekreslení ikony (s kontrolou souøadnic)

void _fastcall AktIcon(int x, int y)
{
	if (((DWORD)x < (DWORD)WidthN) && ((DWORD)y < (DWORD)HeightN))
	{
		SetAktIcon(x, y, true);
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastavení požadavku pøekreslení ikony v intervalu (vèetnì koncových bodù)

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
// vykreslení sprajtu do výstupní plochy (pozadí musí být vykresleno)

void SpritePaint(int index)
{
// adresa sprajtu
	CSprite* sprite = &Sprite[index];

// jen pokud je sprajt viditelný
	if (!sprite->Visible()) return;

// okno k zobrazení sprajtu
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

// zadaná fáze sprajtu (není-li obsloužena pohybem)
	if ((sprite->Timer() <= 0) && 
		!sprite->Moving() && 
		!sprite->KlidMove() &&
		((DWORD)sprite->FazeNext() < (DWORD)sprite->Faze()))
	{
		sprite->FazeN(sprite->FazeNext());
		sprite->FazeNext(-1);
	}

// adresa aktivního obrázku, adresa dat obrázku
	PICTUREDATA* pic = sprite->AktPicData();
	BYTE* src = pic->Data;

// pøednastavení šíøky a výšky k zobrazení
	int widthbyte = pic->Width;
	int width = widthbyte;
	int height = pic->Height;

// levý okraj k zobrazení
	int left = sprite->X();
	if (left < x1)
	{
		width -= x1 - left;
		src += x1 - left;
		left = x1;
	}

// pravý okraj k zobrazení
	int right = left + width;
	if (right > x2)
	{
		width -= right - x2;
		right = x2;
	}

// kontrola šíøky k vykreslení
	if (width <= 0) return;

// dolní okraj k zobrazení (s nulou nahoøe)
	int bottom = Height - sprite->Y();
	if (bottom > Height - y1)
	{
		height -= bottom - (Height - y1);
		src += (bottom - (Height - y1))*widthbyte;
		bottom = Height - y1;
	}

// horní okraj k zobrazení (nula nahoøe)
	int top = bottom - height;
	if (top < Height - y2)
	{
		height -= Height - y2 - top;
		top = Height - y2;
	}

// kontrola výšky k vykreslení
	if (height <= 0) return;

// zajištìní zmapování obrázku
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

// cílová adresa k dekódování obrázku
	BYTE* dst = PicBuf + left + (Height - bottom)*WidthByte;

// pøírustek adresy
	int srcinc;
	int dstinc;

	int height0 = height;

// rozlišení typu obrázku
	switch(pic->Param)
	{

// pouze obrázek bez pozadí
	case PicParamPic:
		dstinc = WidthByte;
		for (; height > 0; height--)
		{
			MemCopy(dst, src, width);
			src += widthbyte;
			dst += dstinc;
		}
		break;

// mixovaný obrázek s pozadím
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

// stavový indikátor
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

// aktualizace plochy k pøekreslení
	if (left < AktLeft) AktLeft = left;
	if (right > AktRight) AktRight = right;
	if (top < AktTop) AktTop = top;
	if (bottom > AktBottom) AktBottom = bottom;
}


/////////////////////////////////////////////////////////////////////////////
// zajištìní vymazání sprajtu (požadavek pøekreslení ikon)

void _fastcall SpriteClear(int index)
{
// adresa sprajtu
	CSprite* sprite = &Sprite[index];

// jen pokud je sprajt viditelný
	if (sprite->Visible())
	{
		AktIconBoxG(sprite->X(), sprite->Y(), sprite->X() + sprite->Width(), 
							sprite->Y() + sprite->Height());
	}
}


/////////////////////////////////////////////////////////////////////////////
// požadavek k pøekreslení sprajtu

void _fastcall AktSprite(int index)
{
// adresa sprajtu
	CSprite* sprite = &Sprite[index];

// jen není-li ještì zapnut požadavek pøekreslení sprajtu
	if (!sprite->DispReq())
	{

// zapnutí požadavku o pøekreslení sprajtu
		sprite->DispReq(true);

// hladina pøidávaného sprajtu
		int level = sprite->Level();
		double y = sprite->AktY();

// nalezení pozice k vložení do tabulky (dole vyšší hladina a nižší souøadnice Y)
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

// zaøazení sprajtu do tabulky
		SpriteDispLevel.Insert(i, level);
		SpriteDispY.Insert(i, y);
		SpriteDispReq.Insert(i, index);
	}
}


/////////////////////////////////////////////////////////////////////////////
// ukonèení animace v klidu

void _fastcall SpriteKlidMoveEnd(int index)
{
// adresa sprajtu
	CSprite* sprite = &Sprite[index];

// jen je-li animace v klidu
	if (sprite->KlidMove())
	{

// zrušení pøíznaku pohybu
		sprite->KlidMove(false);

// vypuštìní sprajtu z tabulky
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
// zahájení animace v klidu

void _fastcall SpriteKlidMoveBeg(int index)
{
// adresa sprajtu
	CSprite* sprite = &Sprite[index];

// jen není-li animace v klidu a je-li více fází klidu
	if (!sprite->KlidMove() && (sprite->Klid() > 1))
	{

// nastavení pøíznaku pohybu
		sprite->KlidMove(true);

// zaøazení do tabulky pohybu v klidu
		SpriteKlid.Add(index);

// èas do další animace
		if (sprite->Timer() <= 0)
		{
			sprite->Timer((sprite->Delay() + 22)/55);
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace zobrazení sprajtù s kladnou hladinou (zobrazuje se v poøadí shora dolù - dole vyšší hladina)

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
// aktualizace zobrazení sprajtù se zápornou hladinou (zobrazuje se v poøadí shora dolù - dole vyšší hladina)

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
// zahájení pøesunu sprajtu na pozici

void SpriteMove(int index, double cilx, double cily)
{
// adresa sprajtu
	CSprite* sprite = &Sprite[index];

// nastavení nové cílové pozice sprajtu
	sprite->CilX(cilx);
	sprite->CilY(cily);

// pøepoèet souøadnic X a Y na body
	int xN = Round(cilx*ICONWIDTH);
	int yN = Round(cily*ICONHEIGHT);

// sprajt není viditelný - pøesun se dìlá skokem
	if (!sprite->Visible())
	{
		sprite->AktX(cilx);
		sprite->AktY(cily);
		sprite->X(xN);
		sprite->Y(yN);
		return;
	}

// bodové souøadnice nezmìnìny - pøesun se nedìlá (rychlé ukonèení pohybu)
	if ((xN == sprite->X()) && (yN == sprite->Y()))
	{
		SpriteClear(index);	// vymazání kvùli správnému kreslení stínu

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

// ukonèení animace v klidu
	SpriteKlidMoveEnd(index);

// výpoèet úhlového smìru pohybu
	double dx = cilx - sprite->AktX();		// rozdíl souøadnic X v políèkách
	double dy = cily - sprite->AktY();		// rozdíl souøadnic Y v políèkách
	double smer = atan2(dy, dx);			// úhel smìru

// výpoèet indexu smìru pohybu
	double dsmer = pi2/sprite->Smer();		// pøírustek úhlu smìru
	int smerN = Round(smer/dsmer);			// index smìru
	while (smerN >= sprite->Smer()) smerN -= sprite->Smer();
	while (smerN < 0) smerN += sprite->Smer();
	sprite->SmerN(smerN);					// nastavení indexu smìru
	sprite->SmerR(dsmer*smerN);				// nastavení úhlu smìru

// uložení sprajtu do tabulky pohybujících se sprajtù
	if (!sprite->Moving())
	{
		sprite->Moving(true);
		sprite->Timer(1);
		SpriteMoving.Add(index);
	}

// nastavení fáze animace (aby byla hned první fáze pohybu)
	if ((sprite->FazeN() < sprite->Klid()) &&
		(sprite->Faze() > sprite->Klid()))
	{
		sprite->FazeN(sprite->Klid()-1);
	}

// poèet krokù na urazení vzdálenosti
	int krokcit = Round(sqrt(dx*dx + dy*dy) * sprite->Kroku());
	sprite->KrokCit(krokcit);

// pøesun na novou pozici ihned
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

// pøírustek souøadnic
	sprite->dX(dx/krokcit);
	sprite->dY(dy/krokcit);
}


/////////////////////////////////////////////////////////////////////////////
// nastavení smìru sprajtu

void SetSpriteSmer(int index, double smer)
{
// adresa sprajtu
	CSprite* sprite = &Sprite[index];

// lokální promìnné
	int smernum = sprite->Smer();		// poèet smìrù
	double dsmer = pi2/smernum;			// pøírustek smìru

// výpoèet indexu smìru
	int smerN = Round(smer/dsmer);
	while (smerN >= smernum) smerN -= smernum;
	while (smerN < 0) smerN += smernum;

// nastavení nového smìru (pokud se zmìnil)
	if (sprite->SmerN() != smerN)
	{
		sprite->SmerN(smerN);
		sprite->SmerR(dsmer*smerN);
		SpriteClear(index);		// vymazání a pøekreslení sprajtu
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastavení viditelnosti sprajtu

void SetSpriteVisible(int index, bool visible)
{
// adresa sprajtu
	CSprite* sprite = &Sprite[index];

// kontrola, zda je zmìna
	if (sprite->Visible() == visible) return;

// zapnutí viditelnosti
	if (visible)
	{
		sprite->Visible(true);
		SpriteVisible.Add(index);
		SpriteKlidMoveBeg(index);
		SpriteClear(index);		// musí se pøekreslit i sprajty nad/pod
	}
	else

// vypnutí viditelnosti
	{
		SpriteClear(index);		// vymazání - ještì musí být viditelný!
		sprite->Visible(false);

// zrušení z bufferu viditelných sprajtù
		for (int i = SpriteVisible.Num()-1; i >= 0; i--)
		{
			if (SpriteVisible[i] == index)
			{
				SpriteVisible.DelItem(i);
				break;
			}
		}

// ukonèení animace v klidu
		SpriteKlidMoveEnd(index);

// zrychlené dokonèení pohybu
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

// korekce fáze na klidovou
		while (sprite->FazeN() >= sprite->Klid())
		{
			sprite->FazeN(sprite->FazeN() - sprite->Klid());
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastavení sprajtu

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
// aktualizaèní pøekreslení jedné ikony

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

	int dstinc = WidthByte;			// pøírustek cílové adresy
	int i = ICONHEIGHT;

// rozlišení typu ikony
	switch(icon->Param)
	{

// pouze obrázek bez pozadí
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

// mixovaný obrázek s pozadím
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

// pouze pozadí
	case PicParamBack:
		break;

// neznámý obsah (zajistí zmapování)
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
// aktualizaèní pøekreslení jednoho pozadí

void AktDispBack0(int x, int y)
{	
// okraje k zobrazení pozadí
	int left = x*ICONWIDTH;

// adresa v bufferech
	int off = left + y*RowByte;
	BYTE* back = BackBuf + off;
	BYTE* dst = PicBuf + off;

	int dstinc = WidthByte;			// pøírustek cílové adresy

// adresa ikony
	CIcon* ico = &(Map[0].At(x, y).Icon);
	ico->DeComp();
	ICONDATA* icon = ico->Data();

// vykreslení pozadí (kromì plné ikony)
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
// aktualizace pøekreslení ikon

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
// aktualizace pøekreslení pozadí

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
// aktualizace pøíznakù pøekreslování (pro pøekreslení stínù sprajtù)

void AktAktDisp0(int x, int y)
{
// okraje k zobrazení
	int left = x*ICONWIDTH;
	int bottom = Height - y*ICONHEIGHT;

// aktualizace plochy k pøekreslení
	if (left < AktLeft) AktLeft = left;
	int right = left + ICONWIDTH;
	if (right > AktRight) AktRight = right;

	if (bottom > AktBottom) AktBottom = bottom;
	int top = bottom - ICONHEIGHT;
	if (top < AktTop) AktTop = top;

// kontrola, zda je potøebné pøekreslení sprajtù
	for (int i = SpriteVisible.Num()-1; i >= 0; i--)
	{
		int index = SpriteVisible[i];		// index sprajtu
		CSprite* sprite = &Sprite[index];		// adresa sprajtu

		if ((right > sprite->X()) &&
			(left < sprite->X() + sprite->Width()) &&
			(Height - top > sprite->Y()) &&
			(Height - bottom < sprite->Y() + sprite->Height()))
		{

// požadavek k pøekreslení sprajtu, není-li ještì nastaven
			if (!sprite->DispReq())
			{
				AktSprite(index);

// požadavek k pøekreslení ikon pod sprajtem
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
// aktualizaèní pøekreslení okna

// lokální promìnné pro pøekrývané zobrazení
HDC	RDdc = NULL;							// DC výstupního zaøízení
BYTE* RDsrcbuf = NULL;						// adresa bufferu s daty
int RDsrcleft = 0;							// souøadnice X dat v bufferu
int RDsrctop = 0;							// souøadnice Y dat v bufferu
int RDsrcheight = 0;						// výška dat v bufferu
int RDwidthbyte = 0;						// délka linky bufferu v bajtech
int RDwinitem = 0;							// ukazatel indexu položky

// zobrazení bez filtrace
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
				left, top, width, height,				// cílové parametry
				left - RDsrcleft, 0, width, height,		// zdrojové parametry
				RDsrcbuf + RDwidthbyte*(RDsrcheight - 
						((top - RDsrctop) + height)),	// adresa dat v bufferu
				&bi, DIB_RGB_COLORS, SRCCOPY);
		}
		else
		{
			StdBitmapInfo->bmiHeader.biHeight = height; // výška plochy v bodech

			::StretchDIBits(RDdc,
				left, top, width, height,				// cílové parametry
				left - RDsrcleft, 0, width, height,		// zdrojové parametry
				RDsrcbuf + RDwidthbyte*(RDsrcheight - 
						((top - RDsrctop) + height)),	// adresa dat v bufferu
				StdBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
		}
	}
}

// zobrazení s filtrací pøes dialogové prvky
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

// není pøekryv
				if ((right <= left2) ||
					(left >= right2) ||
					(top >= bottom2) ||
					(bottom <= top2))
				{
					RDdisp2(left, top, width, height);
				}
				else
				{

// je pøekryv
					RDdisp2(left, top, width, top2 - top);			// horní pás
					RDdisp2(left, bottom2, width, bottom - bottom2);// dolní pás

					if (top2 > top) top = top2;
					if (bottom2 < bottom) bottom = bottom2;
					height = bottom - top;

					RDdisp2(left, top, left2 - left, height);		// levý pás
					RDdisp2(right2, top, right - right2, height);	// pravý pás			
				}
			}
			else

// není platný prvek, další prvek
			{
				RDdisp2(left, top, width, height);
			}
		}

		RDwinitem--;
	}
}

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

// zobrazení s filtrací pøes 3D okno
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
			RDdisp2(left, top, width, D3DTop - top);	// horní pás
			RDdisp2(left, bottom2, width, bottom - bottom2); // dolní pás

			if (D3DTop > top) top = D3DTop;
			if (bottom2 < bottom) bottom = bottom2;
			height = bottom - top;

			RDdisp2(left, top, D3DLeft - left, height);	// levý pás
			RDdisp2(right2, top, right - right2, height); // pravý pás
		}
	}
}

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------

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

// zapnutí hlavního okna
	if (!MainFrameVisible)
	{
		MainFrameShow();
		return;
	}

// aktualizaèní vykreslení ikon a sprajtù
	if (!DialogMode || DialogGraph)
	{
		AktAktDisp();
		AktDispBack();
		AktDispSpriteNeg();
		AktDispIcon();
		AktDispSprite();
	}

// zmìna textu nadpisu okna
	if (CaptionAkt)
	{
		CaptionAkt = false;
		int crlf = CaptionText.Find(13);
		if (crlf >= 0) CaptionText.Delete(crlf);
		SetCaptionText(CaptionText);
	}

// zmìna textu stavového øádku
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

// kontrola, zda je potøeba provádìt obsluhu pøekreslení
	if ((AktLeft < AktRight) &&
		(AktTop < AktBottom) &&
		(!DialogMode || DialogGraph)
#ifndef _MINI
		&& (!D3D || D3D2D)
#endif // _MINI
		)
	{
		HPALETTE OldPal = NULL;

// šíøka a výška úseku obrázku k vykreslení
		int width = AktRight - AktLeft;
		int height = AktBottom - AktTop;

// otevøení DC okna
		RDdc = ::GetDC(MainFrame);

// nastavení vlastních palet
#ifndef _MINI
		if (!D3D || (D3DIntAkt <= 2))
#endif // _MINI
		{
			OldPal = ::SelectPalette(RDdc, StdPalette, FALSE);
			::RealizePalette(RDdc);
		}

// nastavení STRETCH módu
		::SetStretchBltMode(RDdc, COLORONCOLOR);

// pøíprava parametrù, není-li výstupní buffer
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

// bude vykreslování pomocí výstupního bufferu
		if (DispBufUse)
		{

// pøíprava ukazatelù
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

// cyklus pøes všechny linky
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

// cyklus pøes body na lince
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

// vykreslení plochy
		StdBitmapInfo->bmiHeader.biWidth = widthsrc; // šíøka plochy v bodech
		leftdst0 += DispLeft;
		topdst0 += DispTop;

		RDwinitem = 0;							// adresa hlavního okna

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

		if (D3D)
		{
			RDdisp3(leftdst0, topdst0, widthdst, heightdst);
		}
		else

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------

		{
			RDdisp2(leftdst0, topdst0, widthdst, heightdst);
		}

// návrat pùvodních palet
		if (RDdc != NULL)
		{
			if (OldPal != NULL)
			{
				::SelectPalette(RDdc, OldPal, FALSE);
			}

			::ReleaseDC(MainFrame, RDdc);
		}

// zneplatnìní aktualizaèní oblasti
		AktLeft = Width;
		AktTop = Height;
		AktRight = 0;
		AktBottom = 0;
	}

// èítaè pro time-out pøekreslení displeje
	ReDispOut = REDISPOUT;
}


/////////////////////////////////////////////////////////////////////////////
// pøekreslení celého okna (po zprávì WM_PAINT)

void _fastcall OnPaint(WINITEM* item, int inx)
{
// potvrzení pøekreslení okna
	PAINTSTRUCT ps;
	::BeginPaint(item->Wnd, &ps);
	::EndPaint(item->Wnd, &ps);

// pøekreslení obsahu okna
	RePaint(item);

// v dialogovém módu pøekreslení rámeèkù
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
// pøekreslení celého okna

HDC RPdc = NULL;						// DC výstupního zaøízení
HBRUSH RPbrush = NULL;					// štìtec k vymazání plochy
int RPwinitem = 0;						// ukazatel indexu položky

// vymazání bez filtrace
void RPfill(const RECT& rc)
{
	if ((rc.right > rc.left) && (rc.bottom > rc.top))
	{
		::FillRect(RPdc, &rc, RPbrush);
	}
}

// vymazání s filtrací pøes dialogové prvky
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

// není pøekryv
				if ((rc.right <= rc2.left) ||
					(rc.left >= rc2.right) ||
					(rc.top >= rc2.bottom) ||
					(rc.bottom <= rc2.top))
				{
					RPfill2(rc);
				}
				else
				{

// je pøekryv
					RECT rc3 = rc;
					rc3.bottom = rc2.top;
					RPfill2(rc3);					// horní pás

					rc3.bottom = rc.bottom;
					rc3.top = rc2.bottom;
					RPfill2(rc3);					// dolní pás

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
					RPfill2(rc3);					// levý pás

					rc3.right = rc.right;
					rc3.left = rc2.right;
					RPfill2(rc3);					// pravý pás
				}
			}
			else

// není platný prvek, další prvek
			{
				RPfill2(rc);
			}
		}

		RPwinitem--;
	}
}

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

// zobrazení s filtrací pøes 3D okno
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
			RPfill2(rc3);								// horní pás

			rc3.bottom = rc.bottom;
			rc3.top = bottom2;
			RPfill2(rc3);								 // dolní pás

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
			RPfill2(rc3);					// levý pás

			rc3.right = rc.right;
			rc3.left = right2;
			RPfill2(rc3);					// pravý pás
		}
	}
}

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------

void _fastcall RePaint(WINITEM* item)
{
// test, zda je okno již zapnuto
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

// otevøení DC displeje
	bool newbrush = true;
	RPdc = ::GetDC(item->Wnd);
		
	if ((item == &Win[0]) && FullScreen && (!DialogMode || DialogGraph))
	{
		RPbrush = (HBRUSH)::GetStockObject(BLACK_BRUSH); // štìtec k vymazání plochy
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
			RPbrush = (HBRUSH)::GetStockObject(BLACK_BRUSH); // štìtec k vymazání plochy
			newbrush = true;
		}
	}

// vymazání plochy v dialogovém módu
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

// vymazání plochy nahoøe
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

// vymazání plochy dole
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

// vymazání plochy vlevo
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

// vymazání plochy vpravo
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

// vymazání podkladu rámeèkù
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

// zrušení štìtce podkladu (i když podle dokumentace rušení není nutné)
	if (newbrush) ::DeleteObject(RPbrush);

	::ReleaseDC(item->Wnd, RPdc);

// požadavek k pøekreslení celého okna
	AktLeft = 0;				// levý okraj k pøekreslení
	AktTop = 0;					// horní okraj k pøekreslení
	AktRight = Width;			// pravý okraj k pøekreslení
	AktBottom = Height;			// spodní okraj k pøekreslení

// pøekreslení okna
	if (AktBuf != NULL)	ReDisp();		// pøekreslení okna
}


/////////////////////////////////////////////////////////////////////////////
// zjištìní pøíští fáze sprajtu (pro promìnnou fáze sprajtu)

int SpriteNextFaze(int index)
{
// adresa sprajtu
	CSprite* sprite = &Sprite[index];

// pøíští fáze
	int faze = sprite->FazeNext();
	if ((DWORD)faze < (DWORD)sprite->Faze()) return faze;

// aktuální fáze
	faze = sprite->FazeN();

// test, zda je sprajt v pohybu
	if (sprite->Moving())
	{

// test, zda bude dokonèen pohyb
		if (sprite->KrokCit() <= 0)
		{

// dokonèení pohybu
			if (faze >= sprite->Klid())
			{
				faze = faze - sprite->Klid();
			}
			faze = (faze+1) % sprite->Klid();
		}
		else

// pokraèování pohybu
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

// obsluha pohybu sprajtù v klidu
		if (sprite->Klid() > 1)
		{
			faze = (faze + 1) % sprite->Klid();
		}
	}

	return faze;
}


/////////////////////////////////////////////////////////////////////////////
// obsluha èasovaèe

void BackTimer()
{
// zneplatnìní informací o myši
	MouseValid = false;

// zneplatnìní informací o joysticku
	for (int k = MAXJOYSTICK-1; k >= 0; k--)
	{
		JoystickValid[k] = false;
		JoystickNext[k]--;
	}

// obsluha pøehrávání zvuku na pozadí
	PlaySoundBack();

// obsluha hudby na pozadí
	PlayMusicBack();

// obsluha CD na pozadí
	PlayCDBack();

// uzavøení souborù
	if (FileCloseTime > 0)
	{
		FileCloseTime--;
		if (FileCloseTime == 0) FileClose();
	}

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

// èítaè zákazu DirectPlay
	if (NextDirectPlayInit > 0)	NextDirectPlayInit--;

// èítaè platnosti IP adresy
	if (HostIPValid > 0) HostIPValid--;

// test, zda mùže být obsluha DirectPlay
	if (DirectPlay != NULL)
	{
		DirectPlayPlayersTime--;

// aktualizace parametrù hry DirectPlay
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
// --------------------- konec vypnutí pro MINI verzi -------------------

// v Pauze konec
	if (Pause) return;

// obsluha pohybu sprajtù
	for (int i = SpriteMoving.Num()-1; i >= 0; i--)
	{

// pøíprava parametrù sprajtu
		int index = SpriteMoving[i];
		CSprite* sprite = &Sprite[index];
//		SPRITEDATA* spritedata = sprite->Data();

// èítání èasu pro další fázi pohybu
		if (sprite->TimerDec())
		{

// zajištìní vymazání sprajtu na staré pozici
			SpriteClear(index);

// èítání krokù pro dokonèení pohybu
			if (sprite->KrokDec())
			{

// dokonèení pohybu
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

// pokraèování pohybu
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

// požadavek k pøekreslení sprajtu
			AktSprite(index);
		}
	}

// obsluha pohybu sprajtù v klidu
	for (int j = SpriteKlid.Num()-1; j >= 0; j--)
	{

// pøíprava parametrù sprajtu
		int index = SpriteKlid[j];
		CSprite* sprite = &Sprite[index];
//		SPRITEDATA* spritedata = sprite->Data();

// zahájení pohybu v klidu
		if (!sprite->KlidMove())
		{
			sprite->KlidMove(true);
			sprite->Timer((sprite->Delay() + 22)/55);
		}
		else
		{

// èítání èasu pro další fázi pohybu
			if (sprite->TimerDec())
			{

// zajištìní vymazání sprajtu na pozici (a zajištìní pøekreslení)
				SpriteClear(index);

// posun fáze
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

// èítání time-out pøekreslení displeje
	ReDispOut--;
	if (ReDispOut <= 0) ReDisp();
}


/////////////////////////////////////////////////////////////////////////////
// nastavení pøesmykaèe klávesnice

void SetKeyState(int index, bool state)
{
	BYTE data[256];
	::GetKeyboardState(data);
	data[index] &= ~1;
	if (state) data[index] |= 1;
	::SetKeyboardState(data);
}


/////////////////////////////////////////////////////////////////////////////
// test èasovaèe

BOOL TestTimer()
{
	MSG msg;
	return ::PeekMessage(&msg, NULL, WM_TIMER, WM_TIMER, PM_NOREMOVE);
}

/////////////////////////////////////////////////////////////////////////////
// naètení souøadnic myši

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
// èekání na signál od èasovaèe s obsluhou zpráv

void WaitTimer(bool wait)
{
// nastavení pøesmykaèù klávesnice
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

// první obsluha èasovaèe, je-li požadováno èekání na hodiny
	if (wait) 
	{
		BackTimer();
		ReDisp();
	}

// èekání na signál od èasovaèe
	MSG msg;
	for (;;)
	{

// pøíjem zprávy (není-li potøeba èekat, neèeká se na zprávu)
		if (wait)
		{
			::GetMessage(&msg, NULL, 0, 0);
		}
		else
		{
			if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) == 0) break;
		}

// zpracování zprávy oknem
		if (FindDialogBox)
		{
			if (::IsDialogMessage(FindDialogBox, &msg))
			{
				msg.message = (DWORD)-1;
			}
		}

// obsluha èasovaèe - pøi požadavku èekání již byla jedna obsluha provedena
		if (msg.message == WM_TIMER)
		{
			if (!wait) BackTimer();
			wait = false;
			WhileOut = WHILEOUT;
		}

// ukonèení programu uzavøením okna
		if (msg.message == WM_QUIT)
		{
			Break |= BREAKPROG;
			return;
		}

// obsluha zprávy
		if (msg.message != (DWORD)-1)
		{
			if (!::PreTranslateMessage(&msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
	}

// naètení souøadnic myši
	GetAktMousePos();

// inicializace koprocesoru
	FPUDouble();
}


/////////////////////////////////////////////////////////////////////////////
// èekání po zadanou dobu (poèet impulsù 55 ms)

void _fastcall TimeWait(int count)
{
	for (; ((count > 0) || Pause) && (Break == 0); count--)
	{
		WaitTimer(true);
	}
};


/////////////////////////////////////////////////////////////////////////////
// provádìní programu

void Exec()
{
#ifndef _MINI

// pøíprava konfiguraèních parametrù
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

// pøíprava tabulky fontù
	FontTab = (FONTITEM*)MemGet(FONTTABSIZE * sizeof(FONTITEM));
	MemFill(FontTab, FONTTABSIZE * sizeof(FONTITEM), 0);
	for (int ii = 0; ii < FONTTABSIZE; ii++)
	{
		FontTab[ii].UserFont.Init();
		FontTab[ii].CharSet = DEFAULT_CHARSET;
	}

// standardní font tuèný
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

// standardní font normal
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

// standardní font fixed normal
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

// standardní font fixed tuèný
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

// pøíprava identifikaèního kódu pro DirectPlay
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

// pøíprava výstupního bufferu dat pro DirectPlay
	DirectPlayDataOut = (BYTE*)MemGet(DirectPlayDataOutM);

// pøíprava implicitního jména aplikace
	DDEDefApp = AktCaptionText;

// pøíprava tabulky portù
	Coms = (COMITEM*)MemGet(COMSMAX*sizeof(COMITEM));
	MemFill(Coms, COMSMAX*sizeof(COMITEM), 0);

	for (i = 0; i < COMSMAX; i++)
	{
		Coms[i].File = INVALID_HANDLE_VALUE;
	}
#endif // _MINI

// inicializace informací o ploše
	ExecInitMap();

// inicializace bufferù zvuku
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

// inicializace parametrù hry DirectPlay
#ifndef _MINI
	DirectPlayGamesParam[0] = 0x40004000;
	DirectPlayGamesParam[1] = 0x40004000;
	DirectPlayGamesParam[2] = 0x40004000;
	DirectPlayGamesParam[3] = 0x40004000;

// inicializace 3D grafiky
	Init3D();

#endif // _MINI

// zapnutí sprajtu 0 (Petøík)
	SetSpriteVisible(0, true);

// požadavek k pøekreslení okna
	AktAllIcon();				// aktualizovat všechny ikony

// inicializace èasovaèe
	TimerID = ::SetTimer(MainFrame, 543, TimerConst, NULL);

// obsluha hlavní funkce
	ExecItem = ProgBuf + ProgStart;	// startovací adresa
	if (ProgNum > 0) FCommand();

// uzavøení souborù
	FileClose();

// zastavení pøehrávání zvukù
	PlaySoundStop();

// zastavení pøehrávání hudby
	MusicStop();

// uzavøení DirectSound
	TermDirectSound();

// uvolnìní knihovny COM
#ifndef _MINI
	COMUninit();

// uvolnìní knihovny WSA
	WSADeinitialize();

	DeSelectD3DDev();

// zrušení seznamu aplikací DDE
	DDEAppListTerm();

#endif // _MINI

// ukonèení celoobrazovkového módu
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
// --------------------- konec vypnutí pro MINI verzi -------------------

// ukonèení konzoly
	if (NewConsole) ::FreeConsole();

// uzavøení okna (pokud ještì nebylo uzavøeno zvenku)
	if (::IsWindow(MainFrame))
	{
		::SendMessage(MainFrame, WM_CLOSE, 0, 0);
	}

// uzavøení mixeru
#ifndef _MINI
	if (MixDevH != NULL)
	{
		::mixerClose(MixDevH);
	}
#endif // _MINI

// zrušení èasovaèe
	::KillTimer(MainFrame, TimerID);

// zrušení štìtcù
	::DeleteObject(StdBrushBtn);
	::DeleteObject(StdBrushWindow);
	::DeleteObject(StdBrushHighlight);

// uzavøení komunikaèních portù
#ifndef _MINI
	for (i = 0; i < COMSMAX; i++)
	{
		ComClose(i);
	}
#endif // _MINI

// zrušení bufferu zvukových kanálù
	for (i = 0; i < MAXSOUNDKANALU; i++)
	{
		SoundChan[i].Sound.Term();
	}
	MemFree(SoundChan);

// zrušení fontù
	for (i = 0; i < FONTTABSIZE; i++)
	{
		FontTab[i].UserFont.Term();
		HFONT font = FontTab[i].Font;
		if ((font != NULL) && (font != FontDefault)) ::DeleteObject(font);
	}
	::DeleteObject(FontDefault);
	MemFree(FontTab);
}
