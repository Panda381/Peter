
/***************************************************************************\
*																			*
*							Provádìní programu								*
*																			*
\***************************************************************************/

// ------- konstanty

// øízení programu
#define WHILEOUT	50				// èítaè time-out cyklu
#define REDISPOUT	4				// èítaè time-out pøekreslení displeje
#define MODULMERITKA 32				// modul rozlišení mìøítka (musí být dìlitelem 32 !!!!)
#define MODULMERITKA2 4				// modul rozlišení mìøítka pro celoobrazovkový mód
#define DEFCOLOR 0x00ffffff			// implicitní barva pro grafiku
#define MAXHLOUBKA	200				// maximální hloubka vnoøení volání funkcí

// obsluha souborù
#define FILECLOSETIME	10;			// èas pro uzavøení souborù
#define FILEBUFFER		0xf000		// velikost souborového bufferu

// bity pro pøerušení (Break)
#define BREAKPROG	0x1				// úplné pøerušení programu
#define BREAKFUNC	0x2				// pøerušení funkce
#define BREAKWHILE	0x4				// pøerušení cyklu

// obsluha klávesnice
#define KEYBUFSIZE		20			// velikost bufferu kláves
#define KEYMAPSIZE		256			// velikost mapy stisknutých kláves
#define VK_RETURN_NUM	0xad		// ENTER v numerické klávesnici

// ------ promìnné

// buffer provádìného programu
extern	int			ProgStart;		// index volání hlavní funkce
extern	EXECITEM*	ProgBuf;		// buffer programu
extern	int			ProgNum;		// poèet položek programu
extern	int			ProgMax;		// velikost bufferu programu
extern	EXECITEM*	ExecItem;		// ukazatel provádìného programu
extern	int			Hloubka;		// hloubka vnoøení do funkcí

// buffer obrázkù
extern BYTE*	BackBuf;			// buffer obrázku pozadí
extern BYTE*	PicBuf;				// buffer obrázku sestavy (s ikonami a sprajty)
extern BYTE*	TextBuf;			// buffer pro výstup textu

// aktualizace pøekreslení ikon
extern bool*	AktBuf;				// buffer požadavkù k pøekreslení ikon

extern int		AktLeft;			// levý okraj k pøekreslení - vèetnì (v bodech)
extern int		AktTop;				// horní okraj k pøekreslení - vèetnì (v bodech)
extern int		AktRight;			// pravý okraj k pøekreslení - bez (v bodech)
extern int		AktBottom;			// dolní okraj k pøekreslení - bez (v bodech)

extern	bool	IsOverlay;			// zobrazeno pøekryvné okno Windows (otevøení souborù atd.)

// aktivní zobrazená plocha
extern int		WidthN;				// šíøka plochy v ikonách
extern int		HeightN;			// výška plochy v ikonách
extern int		Width;				// šíøka plochy v bodech
extern int		Height;				// výška plochy v bodech
extern int		WidthByte;			// délka linky v bajtech
extern int		RowByte;			// délka øádku ikon v bajtech
extern int		DispLeft;			// levý okraj k zobrazení v oknì
extern int		DispTop;			// horní okraj k zobrazení v oknì
extern int		DispWidth;			// zobrazená šiøka v bodech
extern int		DispHeight;			// zobrazená výška v bodech

extern	double	Meritko;			// mìøítko zobrazení
extern	double	Meritko0;			// uschované mìøítko zobrazení pro normální okno

// zobrazení textu
extern	int		GraphAreaWidth;		// šíøka pole k zobrazení textu centrovanì (v bodech)

// nadpis a stavový øádek
extern	bool	CaptionAkt;			// požadavek k pøekreslení nadpisu okna
extern	CString	CaptionText;		// nový text titulku k nastavení
extern	bool	StatusAkt;			// požadavek k pøekreslení stavového øádku
extern	CString	StatusText;			// nový stavový text k nastavení

// buffer sprajtù
extern	CBufInt	SpriteVisible;		// buffer indexù viditelných sprajtù
extern	CBufInt	SpriteMoving;		// buffer indexù pohybujících se sprajtù
extern	CBufInt	SpriteDispReq;		// buffer sprajtù vyžadujících pøekreslení
extern	CBufInt	SpriteDispLevel;	// hladiny sprajtù vyžadujících pøekreslení

extern	int		SpriteWinX1;		// levý okraj okna sprajtù
extern	int		SpriteWinY1;		// dolní okraj okna sprajtù
extern	int		SpriteWinX2;		// pravý okraj okna sprajtù
extern	int		SpriteWinY2;		// horní okraj okna sprajtù

extern	int		WhileOut;			// èítaè time-out cyklu
extern	int		ReDispOut;			// èítaè time-out pøekreslení displeje

// obsluha myši
extern	bool	MouseValid;			// pøíznak platnosti údajù o myši

// obsluha èasovaèe
extern	UINT	TimerID;			// ID èasovaèe
extern	bool	TimerReq;			// pøišel signál od èasovaèe

extern	bool	Pause;				// pøíznak zapaouzování programu
extern	volatile int Break;			// pøíznaky pøerušení (0 = není pøerušení)

extern	double		MouseX;			// souøadnice myši X (v ikonových souøadnicích)
extern	double		MouseY;			// souøadnice myši Y (v ikonových souøadnicích)

// obsluha CD
#define CDAKTTIME		18			// èas pro aktualizace údajù o CD
#define CDERRORTIME		(3*18)		// èas pro nový pokus pøi chybì
#define CDCLOSETIME		30			// èas pro uzavøení pøístupu k CD (a zneplatnìní informací o disku)
#define CDDEFLENGTH		(60*60*1000) // implicitní délka CD
#define CDMAXLENGTH		(24*60*60*1000) // maximální délka CD
#define CDMAXTRACKS		999			// maximální poèet stop CD

extern	UINT	CDDevice;			// ID zaøízení MCI pro pøehrávání CD (0=není otevøeno)
extern	int		CDError;			// èítaè pro nový pokus pøi chybì (musí být < 0)
extern	int		CDValidTime;		// èítaè platnosti údajù o CD (musí být > 0)
extern	bool	CDDiskValid;		// údaje o disku jsou platné (nebyl vymìnìn disk)
extern	int		CDStart;			// startovací pozice CD v ms
extern	int		CDLength;			// délka CD v ms
extern	int		CDTracks;			// poèet stop CD
extern	int		CDPos;				// pøehrávaná pozice CD v ms
extern	int		CDTrack;			// pøehrávaná stopa
extern	int		CDTrackLength;		// délka pøehrávané stopy v ms
extern	int		CDTrackPos;			// pozice v pøehrávané stopì v ms
extern	bool	CDPlaing;			// pøíznak pøehrávání CD
extern	bool	CDPausing;			// pøíznak pauzy CD

// obsluha klávesnice
extern	int		KeyBufWrite;		// ukládací index do bufferu klávesnice
extern	int		KeyBufRead;			// ètecí index z bufferu klávesnice
extern	int		KeyBuf[KEYBUFSIZE];	// buffer klávesnice

extern	int		CharBufWrite;		// ukládací index do bufferu znakù
extern	int		CharBufRead;		// ètecí index z bufferu znakù
extern	int		CharBuf[KEYBUFSIZE];// buffer znakù

extern	bool	CtrlAgain;			// pøíznak opakovaného stisku Ctrl

extern	int		KeyMapNum;			// poèet stisknutých kláves
extern	bool	KeyMap[KEYMAPSIZE];	// mapa stisknutých kláves

extern	bool	NumLock;			// stav Num Lock
extern	bool	CapsLock;			// stav Caps Lock
extern	bool	ScrollLock;			// stav Scroll Lock
extern	bool	InsertLock;			// stav Insert

extern	bool	AktNumLock;			// požadavek nastavení Num Lock
extern	bool	AktCapsLock;		// požadavek nastavení Caps Lock
extern	bool	AktScrollLock;		// požadavek nastavení Scroll Lock
extern	bool	AktInsertLock;		// požadavek nastavení Insert

// obsluha souborù
extern	bool	FileError;			// pøíznak chyby souborù
extern	int		FileCloseTime;		// èítaè èasu pro uzavøení souborù
extern	int		FileTextNLen;		// délka textu s pevnou délkou
extern	bool	FileTextUnicode;	// text je v kódu UNICODE

extern	CString	FileRead;			// jméno souboru pro ètení
extern	HANDLE	FileReadHandle;		// handle souboru pro ètení
extern	int		FileReadOff;		// offset v souboru pro ètení
extern	BYTE*	FileReadBuf;		// buffer pro ètení
extern	int		FileReadBufOff;		// offset bufferu pro ètení
extern	int		FileReadBufN;		// poèet bajtù v bufferu pro ètení

extern	CString	FileWrite;			// jméno souboru pro zápis
extern	HANDLE	FileWriteHandle;	// handle souboru pro zápis
extern	int		FileWriteOff;		// offset v souboru pro zápis
extern	BYTE*	FileWriteBuf;		// buffer pro zápis
extern	int		FileWriteBufOff;	// offset bufferu pro zápis
extern	int		FileWriteBufN;		// poèet bajtù v bufferu pro zápis

extern	CString	AliasName;			// jméno souboru ALIASES
extern	CString	AliasKey;			// klíè ALIASES
extern	CString	AliasGroup;			// skupina ALIASES

extern	__int64	DiskSize;			// velikost disku (z funkce GetDiskSpace)
extern	__int64	DiskFree;			// volné místo disku (z funkce GetDiskSpace)
extern	__int64	DiskFreeUser;		// volné místo uživatele (z funkce GetDiskSpace)

// konzola
extern	bool	ConsoleOn;			// pøíznak režimu konzoly

// joystick
#define MAXJOYSTICK 4						// max. poèet joystickù
#define JOYSTICKTIME (5*18)					// doba do dalšího testu joysticku
extern	int		JoystickNext[MAXJOYSTICK];	// èítaè do dalšího testu (musí být < 0)
extern	bool	JoystickValid[MAXJOYSTICK];	// pøíznak platnosti údajù joysticku
extern	double	JoystickX[MAXJOYSTICK];		// souøadnice X
extern	double	JoystickY[MAXJOYSTICK];		// souøadnice Y
extern	double	JoystickZ[MAXJOYSTICK];		// souøadnice Z
extern	bool	Joystick1[MAXJOYSTICK];		// tlaèítko 1
extern	bool	Joystick2[MAXJOYSTICK];		// tlaèítko 2
extern	bool	Joystick3[MAXJOYSTICK];		// tlaèítko 3
extern	bool	Joystick4[MAXJOYSTICK];		// tlaèítko 4

// prvky oken
extern	CString ButtonClassName;
extern	CString EditClassName;
extern	CString RichEditClassName;
extern	CString ListBoxClassName;
extern	CString ComboBoxClassName;
extern	CString StaticClassName;
extern	CString TabsClassName;

extern	bool	DialogGraph;			// je grafické pozadí hlavního dialogového okna

extern	bool	DialogMode;				// dialogový mód
extern	int		DialogParent;			// aktivní rodièovské okno
extern	int		WindowID;				// aktivní prvek (-1 = neplatný)
extern	int		WindowFocus;			// prvek s fokusem (-1 = není nebo neznámý)
extern	int		ButtonEsc;				// tlaèítko pro Esc

extern	int		StdColorBtnFace;		// systémová barva podkladu tlaèítek
extern	int		StdColorBtnText;		// systémová barva textu tlaèítek
extern	HBRUSH	StdBrushBtn;			// štìtec pozadí tlaèítek

extern	int		StdColorWindow;			// systémová barva podkladu editoru
extern	int		StdColorWindowText;		// systémová barva textu editoru
extern	HBRUSH	StdBrushWindow;			// štìtec pozadí editoru

extern	int		StdColorHighlightText;	// systémová barva textu kurzoru
extern	int		StdColorHighlight;		// systémová barva pozadí kurzoru
extern	HBRUSH	StdBrushHighlight;		// štìtec pozadí kurzoru

// správce fontù
typedef struct FONTITEM_
{
	bool	Bold;					// je zvýraznìní
	bool	Italic;					// je kurzíva
	bool	Underline;				// je podtržení
	bool	Strikeout;				// je pøeškrtnutí
	bool	Serif;					// jsou patièky
	bool	Fixed;					// je pevná rozteè
	int		Height;					// bodová výška znakù (0=bìžná)
	double	Width;					// relativní šíøka znakù (0=bìžná, 1=základní)
	double	Angle;					// úhel sklonu v radiánech (0=bìžný)
	HFONT	Font;					// vytvoøený font (NULL=volná položka)
	DWORD	CharSet;				// znaková sada fontu
	CString	UserFont;				// jméno uživatelského fontu
	int		Used;					// èítaè použití fontu (blokování proti zrušení)
} FONTITEM;

#define FONTTABSIZE	64				// velikost tabulky fontù
#define	FONTTABSTD	4				// poèet standardních fontù na zaèátku tabulky
extern	int			FontNext;		// index pøíštího volného fontu k uložení
extern	FONTITEM*	FontTab;		// tabulka fontù
extern	HFONT		FontDefault;	// implicitní systémový font (pøi chybì fontù)
extern	CString		UserFont;		// uživatelský font
extern	CString		FontList;		// seznam systémových fontù

// oblasti kurzoru myši
typedef struct MOUSEREG_
{
	int			x1;						// (4) poèátek oblasti vlevo (vèetnì)
	int			y1;						// (4) poèátek oblasti dole (vèetnì)
	int			x2;						// (4) konec oblasti vpravo (bez)
	int			y2;						// (4) konec oblasti nahoøe (bez)
	HCURSOR		Cursor;					// (4) handle kurzoru myši
	int			res;					// (4) ... rezerva pro zarovnání
} MOUSEREG;

extern	MOUSEREG*	MouseReg;			// regiony kurzoru myši
extern	int			MouseRegNum;		// poèet regionù kurzoru myši
extern	int			MouseRegMax;		// velikost bufferu regionù kurzoru myši

// DirectPlay
// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

extern	bool		Coinit;				// probìhla inicializace COM knihovny

extern	CString	DirectPlayConnects;		// seznam spojení DirectPlay
extern	CString	DirectPlayConnectsAkt;	// aktivní spojení (prázdné = nevybráno)
extern	CString	DirectPlayGames;		// seznam her DirectPlay
extern	CString	DirectPlayGamesAkt;		// aktivní hra (prázdné = nevybráno)
extern	DWORD	DirectPlayGamesParam[4];// parametry hry (15 bitù èíslo + 0x4000, 16. bit = pøíznak)
extern	bool	DirectPlayGamesHost;	// poèítaè je hostitelem
extern	int		DirectPlayPlayersMax;	// maximální poèet hráèù (0 = neomezeno)
extern	CString	DirectPlayPlayers;		// seznam hráèù DirectPlay
extern	CString	DirectPlayPlayersAkt;	// aktivní hráè (prázdné = nevybrán)
extern	int		DirectPlayPlayersAktN;	// aktivní hráè (-1 = není)
extern	BYTE*	DirectPlayDataIn;		// datový blok vstupních dat (první 2 bajty = odesílatel)
extern	int		DirectPlayDataInN;		// velikost vstupních dat
extern	int		DirectPlayDataInO;		// offset ètení ze vstupních dat

extern	GUID DirectPlayGuidData;		// identifikátor spojení DirectPlay
extern	CString	DirectPlayGuidText;		// identifikaèní text pro DirectPlay

// WSA 1.1 rozhraní pro DirectPlay
typedef	int (PASCAL FAR *WSASTARTUP) (WORD wVersionRequired, LPWSADATA lpWSAData);
typedef	int (PASCAL FAR *WSACLEANUP) ();
typedef	int (PASCAL FAR *WSAGETHOSTNAME) (char FAR* name, int namelen);
typedef	struct hostent FAR* (PASCAL FAR *WSAGETHOSTBYNAME) (const char FAR* name);
typedef	char FAR* (PASCAL FAR *WSAINET_NTOA) (struct in_addr in);

extern	WSASTARTUP	pWSAStartup;		// funkce "WSAStartup"
extern	WSACLEANUP	pWSACleanup;		// funkce "WSACleanup"
extern	WSAGETHOSTNAME	pWSAGetHostName;// funkce "gethostname"
extern	WSAGETHOSTBYNAME pWSAGetHostByName;// funkce "gethostbyname"
extern	WSAINET_NTOA pWSAInetNtoa;		// funkce "inet_ntoa"
// PC formát IP adresy: první èíslo je nejvýše, tj. napø. 127.0.0.1 je 0x7f000001
// WSA formát IP adresy je v MAC formátu: napø. 127.0.0.1 je 0x0100007f

// rozšíøení WSA 2.2 rozhraní pro UDP
//#ifdef UDP_OVER
typedef SOCKET (PASCAL FAR *WSASOCKET) (int af, int type, int protocol,
						LPWSAPROTOCOL_INFOA lpProtocolInfo, GROUP g, DWORD dwFlags);

//#else



typedef WORD (PASCAL FAR *WSAHTONS) (WORD hostshort);
typedef WSAEVENT (PASCAL FAR *WSACREATEEVENT) (void);
typedef BOOL (PASCAL FAR *WSACLOSEEVENT) (WSAEVENT hEvent);
typedef int (PASCAL FAR *WSASETSOCKOPT) (SOCKET s, int level, int optname,
								const char FAR * optval, int optlen);
typedef int (PASCAL FAR *WSABIND) (SOCKET s, const struct sockaddr FAR * name, int namelen);
typedef int (PASCAL FAR *WSACLOSESOCKET) (SOCKET s);
typedef int (PASCAL FAR *WSASENDTO) (SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount,
    LPDWORD lpNumberOfBytesSent, DWORD dwFlags, const struct sockaddr FAR * lpTo,
    int iTolen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
typedef int (PASCAL FAR *WSARECVFROM) (SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount,
    LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, struct sockaddr FAR * lpFrom, LPINT lpFromlen,
	LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
typedef BOOL (PASCAL FAR *WSAGETOVERLAPPEDRESULT) (SOCKET s, LPWSAOVERLAPPED lpOverlapped,
    LPDWORD lpcbTransfer, BOOL fWait, LPDWORD lpdwFlags);
typedef int (PASCAL FAR *WSAGETLASTERROR) (void);
typedef BOOL (PASCAL FAR *WSASETEVENT) (WSAEVENT hEvent);
typedef DWORD (PASCAL FAR *WSAINETADDR) (const char FAR * cp);

extern	WSASOCKET pWSASocket;			// funkce "WSASocketA"
extern	WSAHTONS pWSAhtons;				// funkce "htons" (pøevede WORD èíslo portu - na PC prohodí bajty)
extern	WSACREATEEVENT pWSACreateEvent;	// funkce "WSACreateEvent"
extern	WSACLOSEEVENT pWSACloseEvent;	// funkce "WSACloseEvent"
extern	WSASETSOCKOPT pWSASetSockOpt;	// funkce "setsockopt"
extern	WSABIND pWSABind;				// funkce "bind"
extern	WSACLOSESOCKET pWSACloseSocket;	// funkce "closesocket"
extern	WSASENDTO pWSASendTo;			// funkce "WSASendTo"
extern	WSARECVFROM pWSARecvFrom;		// funkce "WSARecvFrom"
extern	WSAGETOVERLAPPEDRESULT pWSAGetOverlappedResult; // funkce WSAGetOverlappedResult"
extern	WSAGETLASTERROR pWSAGetLastError; // funkce "WSAGetLastError"
extern	WSASETEVENT pWSASetEvent;		// funkce "WSASetEvent"
extern	WSAINETADDR pWSAInetAddr;		// funkce "inet_addr"

// knihovna WSA
extern	HINSTANCE WSALibrary;			// WSA knihovna
extern	bool	Wsaload;				// knihovna WSA byla naèítána
extern	bool	Wsainit;				// knihovna WSA 1.1 úspìšnì inicializována
extern	bool	Wsainit2;				// knihovna WSA 2.2 úspìšnì inicializována
extern	WSADATA	Wsadata;				// data knihovny WSA
extern	CString	HostIP;					// IP adresa poèítaèe
extern	int		HostIPValid;			// èítaè platnosti IP adresy poèítaèe (je-li > 0)

// porty (není u MINI verze)
typedef struct COMITEM_
{
	HANDLE		File;					// identifikátor (INVALID_HANDLE_VALUE = nepoužitý)
	DCB			Dcb;					// informace o portu
} COMITEM;

#define	COMSMAX		64					// max. poèet portù
extern	COMITEM*	Coms;				// tabulky portù
extern	int			ComAkt;				// aktivní port

// mixer (není u MINI verze)
typedef struct MIXDEV_
{
	CString	Name;						// jméno mixážního zaøízení
	int		Dest;						// poèet cílových signálù
} MIXDEV;

typedef struct MIXLINE_
{
	CString Name;						// jméno signálu
	int		Dest;						// index cílového signálu
	int		Source;						// index zdrojového signálu (-1=jen cílový)
	int		SourceI;					// index poèáteèního zdrojového signálu
	DWORD	LineID;						// identifikátor signálu
	DWORD	Type;						// typ signálu
	int		Channels;					// poèet samostatných kanálù
	int		Sources;					// poèet pøipojených zdrojových signálù
	int		Controls;					// poèet ovládacích prvkù signálu
} MIXLINE;

typedef struct MIXCTRL_
{
	CString	Name;						// jméno ovládacího prvku
	DWORD	ControlID;					// identifikátor ovládacího prvku
	DWORD	Type;						// typ ovládacího prvku
	int		Steps;						// poèet krokù nastavení hodnoty bez poèáteèní 0 (0=maximálnì)
	int		Channels;					// poèet samostatných kanálù
	int		List;						// poèet prvkù pro seznam (1=není seznam)
	double	Min;						// minimální hodnota prvku
	double	Max;						// maximální hodnota prvku
	double	Rozsah;						// rozsah hodnot prvku
	double	Inc;						// pøírustek kroku prvku
} MIXCTRL;

extern	int		MixDevN;				// poèet mixážních zaøízení
extern	int		MixDevA;				// aktivní mixážní zaøízení (-1=není)
extern	CString	MixDevT;				// víceøádkový seznam mixážních zaøízení
extern	MIXDEV*	MixDev;					// seznam mixážních zaøízení
extern	HMIXER	MixDevH;				// handle mixeru (NULL=není)

extern	int		MixDest;				// poèet cílových signálù mixeru
extern	int		MixLineN;				// poèet signálù mixeru
extern	MIXLINE* MixLine;				// seznam signálù (na zaèátku jsou cílové)
extern	CString	MixLineDT;				// víceøádkový seznam cílových signálù
extern	CString	MixLineST;				// víceøádkový seznam zdrojových signálù

extern	int		MixLineDA;				// aktivní cílový signál
extern	int		MixLineSA;				// aktivní zdrojový signál (-1=jen cílový)
extern	int		MixLineA;				// aktivní signál absolutnì (v tabulce)

extern	int		MixCtrlN;				// poèet ovládacích prvkù signálu
extern	MIXCTRL* MixCtrl;				// seznam ovládacích prvkù
extern	CString	MixCtrlT;				// víceøádkový seznam ovládacích prvkù
extern	int		MixCtrlA;				// aktivní ovládací prvek (-1=není)
extern	int		MixChannel;				// nastavovaný kanál (-1=všechny)

extern	int		MixValN;				// poèet hodnot prvku (1=není seznam hodnot)
extern	CString* MixVal;				// buffer jmen ovládacích prvkù
extern	CString	MixValT;				// seznam hodnot prvku seznamu
extern	int		MixValA;				// aktivní prvek seznamu

// obsluha DDE
extern	CString		DDEDefApp;			// implicitní jméno aplikace pro DDE

typedef struct DDEAPPITEM_				// položka aplikace
{
	CString		AppName;				// jméno aplikace
	int			ServFirst;				// index prvního serveru s tímto jménem aplikace
} DDEAPPITEM;

extern	DDEAPPITEM*	DDEAppList;			// seznam aplikací
extern	int			DDEAppNum;			// poèet aplikací
extern	int			DDEAppMax;			// velikost bufferu aplikací
extern	bool		DDEAppLoad;			// naèítá se seznam aplikací

extern	int			DDEAppAkt;			// aktivní aplikace (-1=všechny)
extern	int			DDEServAkt;			// aktivní server (-1=všechny)
extern	CString		DDEAppAktName;		// jméno zadané aktivní aplikace/serveru

typedef	struct DDESERVITEM_				// položka serveru
{
	int			AppIndex;				// index aplikace
	CString		ServName;				// plné jméno serveru s handle okna ([12345678] Name)
	int			ServNext;				// index dalšího serveru s tímto jménem aplikace (-1=není)
	HWND		ServHandle;				// handle okna serveru
	int			ServServ;				// poèet témat, pro které je to aktivní server
	CString		Execute;				// pøíkaz k provedení	
	int			TopicFirst;				// index prvního tématu tohoto serveru
} DDESERVITEM;

extern	DDESERVITEM* DDEServList;		// seznam severù
extern	int			DDEServNum;			// poèet serverù
extern	int			DDEServMax;			// velikost bufferu serverù

typedef struct DDETOPITEM_				// položka tématu
{
	int			ServIndex;				// index serveru
	CString		TopicName;				// jméno tématu
	int			TopicNext;				// index dalšího tématu serveru (-1=není)
	int			TopicServ;				// poèet položek, pro které je téma serverem
	int			DataFirst;				// index první položky tohoto tématu (-1=není)
} DDETOPITEM;

extern	DDETOPITEM*	DDETopList;			// seznam témat
extern	int			DDETopNum;			// poèet témat
extern	int			DDETopMax;			// velikost bufferu témat
extern	int			DDETopAkt;			// aktivní téma (-1=všechny)

typedef struct DDEDATAITEM_				// položka tabulky prvkù DDE
{
	int			TopicIndex;				// index tématu
	CString		DataName;				// jméno prvku
	int			DataNext;				// index dalšího prvku tématu (-1=není)
	int			DataType;				// typ prvku (0 až 7, -1=neplatný)
	double		DataNumber;				// 0: èíslo
	bool		DataBool;				// 1: pøíznak
	bool		Server;					// prvek je serverem
	bool		res2;
	bool		res3;
	CString		DataText;				// 2: text
	CIcon		DataIcon;				// 3: ikona
	CPicture	DataPicture;			// 4: obrázek
	CSound		DataSound;				// 5: zvuk
	CMusic		DataMusic;				// 6: hudba
	CSprite		DataSprite;				// 7: sprajt

} DDEDATAITEM;

extern	DDEDATAITEM* DDEDataList;		// seznam datových položek
extern	int			DDEDataNum;			// poèet datových položek
extern	int			DDEDataMax;			// velikost bufferu datových položek
extern	int			DDEDataAkt;			// aktivní položka (-1=všechny)

// obsluha DLL
extern	void*		DLLMemoryRead;		// ukazatel ètení z pamìti
extern	void*		DLLMemoryWrite;		// ukazatel zápisu do pamìti
extern	int			DLLMemoryTextNLen;	// délka textu s pevnou délkou

// konfigurace
extern CString		INIFile;			// jméno konfiguraèního souboru
extern CString		INISection;			// jméno konfiguraèní sekce
extern CString		INIKey;				// jméno konfiguraèního parametru
extern int			REGKey;				// skupina registrù (1=CURRENT_USER, 2=LOCAL_MACHINE)
extern CString		REGSubkey;			// jméno klíèe
extern CString		REGValue;			// jméno položky

// UDP
extern SOCKET		UDPSendSocket;		// UDP vysílací soket (INVALID_SOCKET = není)
extern SOCKADDR_IN	UDPSendAddr;		// vysílací adresa UDP
extern WSAOVERLAPPED UDPSendOver;		// overlapped struktura vysílaèe
extern WSABUF		UDPSendBuf;			// odesílací buffer UDP
extern bool			UDPSending;			// probíhá vysílání UDP

extern SOCKET		UDPRecSocket;		// UDP pøijímací soket (INVALID_SOCKET = není)
extern SOCKADDR_IN	UDPRecAddr;			// pøijímací adresa UDP
extern SOCKADDR_IN	UDPRecRecAddr;		// pøijatá adresa UDP
extern WSAOVERLAPPED UDPRecOver;		// overlapped struktura pøijímaèe
extern WSABUF		UDPRecBuf;			// pøijímací buffer UDP
extern bool			UDPReceiving;		// probíhá overlapped pøíjem UDP
extern int			UDPRecData;			// velikost dat v pøijímacím bufferu

/////////////////////////////////////////////////////////////////////////////
// obsluha DDE

void DDEAppListLoad();					// naètení seznamu aplikací

/////////////////////////////////////////////////////////////////////////////
// obsluha mixeru (není u MINI verze)

bool MixDevInit();						// inicializace mixážních zaøízení (vrací TRUE=OK)
bool MixDevSet(int mix);				// nastavení aktivního mixeru (vrací TRUE=OK)
void MixDstSet(int dst);				// nastavení cílového signálu (-1=žádný)
void MixSrcSet(int src);				// nastavení zdrojového signálu (-1=jen cílový)
void MixCtrlSet(int ctrl);				// nastavení ovládacího prvku (-1=žádný)
double GetMixCtrlVal();					// zjištìní hodnoty ovládacího prvku
void SetMixCtrlVal(double val);			// nastavení hodnoty ovládacího prvku

/////////////////////////////////////////////////////////////////////////////
// obsluha portù (není u MINI verze)

void ComReinit();						// inicializace pøíjmu dat
void ComClose(int i);					// uzavøení portu (i=platný index)
void GetComDCB();						// naètení DCB aktivního portu
void SetComDCB();						// nastavení DCB aktivního portu
void ComSend(BYTE data);				// vyslání bajtu na komunikaèní port
BYTE ComReceive();						// pøíjem bajtu z komunikaèního portu (0=nejsou data)

/////////////////////////////////////////////////////////////////////////////
// DirectPlay (není u MINI verze)

void DirectSendData(BYTE* data, int n);	// zápis dat do výstupního bufferu
void DirectSend(int id);				// vyslání bloku dat (-1 = všem)
void DirectReceiveData(BYTE* data, int n); // ètení dat ze vstupního bufferu (pøi nedostatku dat doplní nulami)
int DirectReceive();					// pøíjem bloku dat (-1 = není)
void SetPlayerAct(CString txt);			// nastavení aktivního hráèe
bool DirectPlayEnumPlayers();			// seznam hráèù (FALSE=chyba)
void SetPlayerMax(int n);				// nastavení max. hráèù (0=neomezeno)
void GetGameDesc();						// naètení parametrù hry
void SetGameN(int n, int i);			// nastavení èíselné hodnoty hry (n = rozsah -16384 až +16383, i = index 0 až 7)
void SetGameL(bool n, int i);			// nastavení pøepínaèe hry (i = index 0 až 7)
void SetGameAct(CString txt);			// nastavení aktivní hry
bool DirectPlayEnumGames();				// seznam her (FALSE=chyba)
void SetConnectAct(CString txt);		// nastavení aktivního spojení
bool DirectPlayConInit();				// inicializace seznamu spojení (FALSE=chyba)
void DirectPlayConUninit();				// ukonèení seznamu spojení
bool DirectPlayInit();					// inicializace objektu DirectPlay (FALSE=chyba)
void DirectPlayUninit();				// uvolnìní objektu DirectPlay
bool COMInit();							// inicializace knihovny COM (FALSE=chyba)
void COMUninit();						// uvolnìní knihovny COM

/////////////////////////////////////////////////////////////////////////////
// UDP

void WSAInitialize();					// inicializace WSA knihovny
bool WSAInitialize1();					// inicializace WSA 1.1 knihovny (vrací TRUE je-li OK)
bool WSAInitialize2();					// inicializace WSA 2.2 knihovny (vrací TRUE je-li OK)
void WSADeinitialize();					// deinicializace WSA knihovny

void InitUDP();							// inicializace obsluhy UDP (inicializace bufferù)
bool OpenSendUDP();						// otevøení vysílacího soketu UDP (pøi chybì vrací FALSE)
void CloseSendUDP();					// zavøení vysílacího soketu UDP
bool OpenRecUDP();						// otevøení pøijímacího soketu UDP (pøi chybì vrací FALSE)
void CloseRecUDP();						// zavøení pøijímacího soketu UDP
void SendUDP(const void* data, int len);// odeslání UDP paketu
#ifdef UDP_OVER
void StartRecUDP();						// start pøíjmu UDP paketu
#endif
int RecUDP();							// pøíjem UDP paketu (vrací velikost dat v pøijímacím bufferu)

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------

/////////////////////////////////////////////////////////////////////////////
// dialogy

void DialogOn(bool on);					// režim dialogu
void WindowNew(int typ, int styl);		// vytvoøení okna
HFONT GetFont(bool bold, bool italic, bool underline, bool strikeout,
			  bool serif, bool fixed, int height, double width, double angle); // vytvoøení fontu (nutné uvolnìní pomocí FreeFont)
void FreeFont(HFONT font);				// uvolnìní fontu pøidìleného s GetFont
void WindowSetFont(WINITEM* item);	// aktualizace fontu
void WindowItemNew(int typ, CString& classname, int width, int height, int style, int exstyle); // vytvoøení prvku okna
void WindowDel(int inx);				// zrušení prvku
void FocusNext();						// posun fokusu na další prvek
void FocusPrev();						// posun fokusu na pøedchozí prvek
void RadioNext();						// posun fokusu na další pøepínaè
void RadioPrev();						// posun fokusu na pøedchozí pøepínaè
void RadioSet(int inx);					// nastavení pøepínaèe


/////////////////////////////////////////////////////////////////////////////
// joystick

void AktJoystick(int inx);				// aktualizace údajù o joysticku


/////////////////////////////////////////////////////////////////////////////
// konzola

void ConsoleOnSet(bool on);			// zapnutí/vypnutí konzoly
void ConsoleOut(const CString& text);	// výstup na konzolu
void ConsoleErr(const CString& text);	// chybový výstup na konzolu
void ConsoleIn(CString& text);		// vstup textu z konzoly


/////////////////////////////////////////////////////////////////////////////
// obsluha CD diskù

void CDPlay(int stopa);				// zahájení pøehrávání od zadané stopy
void CDSetTrack(int stopa);			// nastavení stopy
void CDStop();						// zastavení pøehrávání CD
void CDEject(bool eject);			// vysunutí CD
void CDPause();						// pauza CD
void CDSetPos(int pozice);			// nastavení pozice CD v ms + pøehrávání
void CDSetTrackPos(int pozice);		// nastavení pozice v aktivní stopì CD v ms + pøehrávání
void CDClose();						// uzavøení ovladaèe CD
bool CDOpen();						// otevøení ovladaèe CD
bool CDAkt();						// aktualizace údajù o CD disku


/////////////////////////////////////////////////////////////////////////////
// zjištìní informací o velikosti a volném místu aktivního disku

void GetDiskSpace();


/////////////////////////////////////////////////////////////////////////////
// obsluha souborù

bool FileReadOpen();				// otevøení souboru pro ètení
bool FileWriteOpen();				// otevøení souboru pro zápis
bool FileWriteFlush();				// vyprázdnìní zápisového bufferu
void _fastcall FileWriteBlok(BYTE* buf, int size); // zápis bloku do souboru
void FileWriteByte(BYTE data);		// zápis bajtu do výstupního souboru
void FileWriteWord(WORD data);		// zápis slova do výstupního souboru
void FileWriteDWord(DWORD data);	// zápis dvojslova do výstupního souboru
WORD FileReadWord();				// ètení slova ze vstupního souboru
BYTE FileReadByte();				// ètení bajtu ze vstupního souboru
void FileClose();					// uzavøení souborù
void FileReadBlok(BYTE* buf, int size); //naètení bloku ze souboru
void _fastcall InitAlias(CString name);		// pøíprava jména ALIAS
void _fastcall InitAliasGroup(int lan);		// inicializace skupiny ALIAS

/////////////////////////////////////////////////////////////////////////////
// provádìní programu

void Exec();


/////////////////////////////////////////////////////////////////////////////
// naètení souøadnic myši

void GetAktMousePos();


/////////////////////////////////////////////////////////////////////////////
// inicializace informací o ploše

void ExecInitMap();


/////////////////////////////////////////////////////////////////////////////
// nastavování rozmìrù okna

void OnSizing(RECT* rc, int side);


/////////////////////////////////////////////////////////////////////////////
// aktualizace rozmìrù okna

void _fastcall OnSize(WINITEM* item);


/////////////////////////////////////////////////////////////////////////////
// pøekreslení okna

void _fastcall OnPaint(WINITEM* item, int inx);


/////////////////////////////////////////////////////////////////////////////
// pøekreslení celého okna

void _fastcall RePaint(WINITEM* item);


/////////////////////////////////////////////////////////////////////////////
// aktualizaèní pøekreslení okna

void ReDisp();


/////////////////////////////////////////////////////////////////////////////
// èekání po zadanou dobu

void _fastcall TimeWait(int count);


/////////////////////////////////////////////////////////////////////////////
// obsluha èasovaèe

void BackTimer();


/////////////////////////////////////////////////////////////////////////////
// test èasovaèe

BOOL TestTimer();


/////////////////////////////////////////////////////////////////////////////
// èekání na signál od èasovaèe s obsluhou zpráv

void WaitTimer(bool wait);


/////////////////////////////////////////////////////////////////////////////
// pøesun sprajtu

void SpriteMove(int index, double cilx, double cily);


/////////////////////////////////////////////////////////////////////////////
// nastavení smìru sprajtu

void SetSpriteSmer(int index, double smer);


/////////////////////////////////////////////////////////////////////////////
// nastavení viditelnosti sprajtu

void SetSpriteVisible(int index, bool visible);


/////////////////////////////////////////////////////////////////////////////
// nastavení sprajtu

void SetSprite(int inx, const CSprite& src);


/////////////////////////////////////////////////////////////////////////////
// zjištìní pøíští fáze sprajtu (pro promìnnou fáze sprajtu)

int SpriteNextFaze(int index);


/////////////////////////////////////////////////////////////////////////////
// zajištìní vymazání sprajtu (požadavek pøekreslení ikon)

void _fastcall SpriteClear(int index);


/////////////////////////////////////////////////////////////////////////////
// nastavení požadavku pøekreslení ikony (bez kontroly souøadnic)

inline void SetAktIcon(int x, int y, bool b)
{
	AktBuf[x + WidthN*y] = b;
}


/////////////////////////////////////////////////////////////////////////////
// nastavení požadavku pøekreslení všech ikon

void AktAllIcon();


/////////////////////////////////////////////////////////////////////////////
// nastavení požadavku pøekreslení ikony v intervalu (vèetnì koncových bodù)

void _fastcall AktIconBoxG(int x1, int y1, int x2, int y2);
void _fastcall AktIconBox(int x1, int y1, int x2, int y2);
