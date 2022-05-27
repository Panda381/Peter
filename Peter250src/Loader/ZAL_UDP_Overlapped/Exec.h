
/***************************************************************************\
*																			*
*							Prov�d�n� programu								*
*																			*
\***************************************************************************/

// ------- konstanty

// ��zen� programu
#define WHILEOUT	50				// ��ta� time-out cyklu
#define REDISPOUT	4				// ��ta� time-out p�ekreslen� displeje
#define MODULMERITKA 32				// modul rozli�en� m���tka (mus� b�t d�litelem 32 !!!!)
#define MODULMERITKA2 4				// modul rozli�en� m���tka pro celoobrazovkov� m�d
#define DEFCOLOR 0x00ffffff			// implicitn� barva pro grafiku
#define MAXHLOUBKA	200				// maxim�ln� hloubka vno�en� vol�n� funkc�

// obsluha soubor�
#define FILECLOSETIME	10;			// �as pro uzav�en� soubor�
#define FILEBUFFER		0xf000		// velikost souborov�ho bufferu

// bity pro p�eru�en� (Break)
#define BREAKPROG	0x1				// �pln� p�eru�en� programu
#define BREAKFUNC	0x2				// p�eru�en� funkce
#define BREAKWHILE	0x4				// p�eru�en� cyklu

// obsluha kl�vesnice
#define KEYBUFSIZE		20			// velikost bufferu kl�ves
#define KEYMAPSIZE		256			// velikost mapy stisknut�ch kl�ves
#define VK_RETURN_NUM	0xad		// ENTER v numerick� kl�vesnici

// ------ prom�nn�

// buffer prov�d�n�ho programu
extern	int			ProgStart;		// index vol�n� hlavn� funkce
extern	EXECITEM*	ProgBuf;		// buffer programu
extern	int			ProgNum;		// po�et polo�ek programu
extern	int			ProgMax;		// velikost bufferu programu
extern	EXECITEM*	ExecItem;		// ukazatel prov�d�n�ho programu
extern	int			Hloubka;		// hloubka vno�en� do funkc�

// buffer obr�zk�
extern BYTE*	BackBuf;			// buffer obr�zku pozad�
extern BYTE*	PicBuf;				// buffer obr�zku sestavy (s ikonami a sprajty)
extern BYTE*	TextBuf;			// buffer pro v�stup textu

// aktualizace p�ekreslen� ikon
extern bool*	AktBuf;				// buffer po�adavk� k p�ekreslen� ikon

extern int		AktLeft;			// lev� okraj k p�ekreslen� - v�etn� (v bodech)
extern int		AktTop;				// horn� okraj k p�ekreslen� - v�etn� (v bodech)
extern int		AktRight;			// prav� okraj k p�ekreslen� - bez (v bodech)
extern int		AktBottom;			// doln� okraj k p�ekreslen� - bez (v bodech)

extern	bool	IsOverlay;			// zobrazeno p�ekryvn� okno Windows (otev�en� soubor� atd.)

// aktivn� zobrazen� plocha
extern int		WidthN;				// ���ka plochy v ikon�ch
extern int		HeightN;			// v��ka plochy v ikon�ch
extern int		Width;				// ���ka plochy v bodech
extern int		Height;				// v��ka plochy v bodech
extern int		WidthByte;			// d�lka linky v bajtech
extern int		RowByte;			// d�lka ��dku ikon v bajtech
extern int		DispLeft;			// lev� okraj k zobrazen� v okn�
extern int		DispTop;			// horn� okraj k zobrazen� v okn�
extern int		DispWidth;			// zobrazen� �i�ka v bodech
extern int		DispHeight;			// zobrazen� v��ka v bodech

extern	double	Meritko;			// m���tko zobrazen�
extern	double	Meritko0;			// uschovan� m���tko zobrazen� pro norm�ln� okno

// zobrazen� textu
extern	int		GraphAreaWidth;		// ���ka pole k zobrazen� textu centrovan� (v bodech)

// nadpis a stavov� ��dek
extern	bool	CaptionAkt;			// po�adavek k p�ekreslen� nadpisu okna
extern	CString	CaptionText;		// nov� text titulku k nastaven�
extern	bool	StatusAkt;			// po�adavek k p�ekreslen� stavov�ho ��dku
extern	CString	StatusText;			// nov� stavov� text k nastaven�

// buffer sprajt�
extern	CBufInt	SpriteVisible;		// buffer index� viditeln�ch sprajt�
extern	CBufInt	SpriteMoving;		// buffer index� pohybuj�c�ch se sprajt�
extern	CBufInt	SpriteDispReq;		// buffer sprajt� vy�aduj�c�ch p�ekreslen�
extern	CBufInt	SpriteDispLevel;	// hladiny sprajt� vy�aduj�c�ch p�ekreslen�

extern	int		SpriteWinX1;		// lev� okraj okna sprajt�
extern	int		SpriteWinY1;		// doln� okraj okna sprajt�
extern	int		SpriteWinX2;		// prav� okraj okna sprajt�
extern	int		SpriteWinY2;		// horn� okraj okna sprajt�

extern	int		WhileOut;			// ��ta� time-out cyklu
extern	int		ReDispOut;			// ��ta� time-out p�ekreslen� displeje

// obsluha my�i
extern	bool	MouseValid;			// p��znak platnosti �daj� o my�i

// obsluha �asova�e
extern	UINT	TimerID;			// ID �asova�e
extern	bool	TimerReq;			// p�i�el sign�l od �asova�e

extern	bool	Pause;				// p��znak zapaouzov�n� programu
extern	volatile int Break;			// p��znaky p�eru�en� (0 = nen� p�eru�en�)

extern	double		MouseX;			// sou�adnice my�i X (v ikonov�ch sou�adnic�ch)
extern	double		MouseY;			// sou�adnice my�i Y (v ikonov�ch sou�adnic�ch)

// obsluha CD
#define CDAKTTIME		18			// �as pro aktualizace �daj� o CD
#define CDERRORTIME		(3*18)		// �as pro nov� pokus p�i chyb�
#define CDCLOSETIME		30			// �as pro uzav�en� p��stupu k CD (a zneplatn�n� informac� o disku)
#define CDDEFLENGTH		(60*60*1000) // implicitn� d�lka CD
#define CDMAXLENGTH		(24*60*60*1000) // maxim�ln� d�lka CD
#define CDMAXTRACKS		999			// maxim�ln� po�et stop CD

extern	UINT	CDDevice;			// ID za��zen� MCI pro p�ehr�v�n� CD (0=nen� otev�eno)
extern	int		CDError;			// ��ta� pro nov� pokus p�i chyb� (mus� b�t < 0)
extern	int		CDValidTime;		// ��ta� platnosti �daj� o CD (mus� b�t > 0)
extern	bool	CDDiskValid;		// �daje o disku jsou platn� (nebyl vym�n�n disk)
extern	int		CDStart;			// startovac� pozice CD v ms
extern	int		CDLength;			// d�lka CD v ms
extern	int		CDTracks;			// po�et stop CD
extern	int		CDPos;				// p�ehr�van� pozice CD v ms
extern	int		CDTrack;			// p�ehr�van� stopa
extern	int		CDTrackLength;		// d�lka p�ehr�van� stopy v ms
extern	int		CDTrackPos;			// pozice v p�ehr�van� stop� v ms
extern	bool	CDPlaing;			// p��znak p�ehr�v�n� CD
extern	bool	CDPausing;			// p��znak pauzy CD

// obsluha kl�vesnice
extern	int		KeyBufWrite;		// ukl�dac� index do bufferu kl�vesnice
extern	int		KeyBufRead;			// �tec� index z bufferu kl�vesnice
extern	int		KeyBuf[KEYBUFSIZE];	// buffer kl�vesnice

extern	int		CharBufWrite;		// ukl�dac� index do bufferu znak�
extern	int		CharBufRead;		// �tec� index z bufferu znak�
extern	int		CharBuf[KEYBUFSIZE];// buffer znak�

extern	bool	CtrlAgain;			// p��znak opakovan�ho stisku Ctrl

extern	int		KeyMapNum;			// po�et stisknut�ch kl�ves
extern	bool	KeyMap[KEYMAPSIZE];	// mapa stisknut�ch kl�ves

extern	bool	NumLock;			// stav Num Lock
extern	bool	CapsLock;			// stav Caps Lock
extern	bool	ScrollLock;			// stav Scroll Lock
extern	bool	InsertLock;			// stav Insert

extern	bool	AktNumLock;			// po�adavek nastaven� Num Lock
extern	bool	AktCapsLock;		// po�adavek nastaven� Caps Lock
extern	bool	AktScrollLock;		// po�adavek nastaven� Scroll Lock
extern	bool	AktInsertLock;		// po�adavek nastaven� Insert

// obsluha soubor�
extern	bool	FileError;			// p��znak chyby soubor�
extern	int		FileCloseTime;		// ��ta� �asu pro uzav�en� soubor�
extern	int		FileTextNLen;		// d�lka textu s pevnou d�lkou
extern	bool	FileTextUnicode;	// text je v k�du UNICODE

extern	CString	FileRead;			// jm�no souboru pro �ten�
extern	HANDLE	FileReadHandle;		// handle souboru pro �ten�
extern	int		FileReadOff;		// offset v souboru pro �ten�
extern	BYTE*	FileReadBuf;		// buffer pro �ten�
extern	int		FileReadBufOff;		// offset bufferu pro �ten�
extern	int		FileReadBufN;		// po�et bajt� v bufferu pro �ten�

extern	CString	FileWrite;			// jm�no souboru pro z�pis
extern	HANDLE	FileWriteHandle;	// handle souboru pro z�pis
extern	int		FileWriteOff;		// offset v souboru pro z�pis
extern	BYTE*	FileWriteBuf;		// buffer pro z�pis
extern	int		FileWriteBufOff;	// offset bufferu pro z�pis
extern	int		FileWriteBufN;		// po�et bajt� v bufferu pro z�pis

extern	CString	AliasName;			// jm�no souboru ALIASES
extern	CString	AliasKey;			// kl�� ALIASES
extern	CString	AliasGroup;			// skupina ALIASES

extern	__int64	DiskSize;			// velikost disku (z funkce GetDiskSpace)
extern	__int64	DiskFree;			// voln� m�sto disku (z funkce GetDiskSpace)
extern	__int64	DiskFreeUser;		// voln� m�sto u�ivatele (z funkce GetDiskSpace)

// konzola
extern	bool	ConsoleOn;			// p��znak re�imu konzoly

// joystick
#define MAXJOYSTICK 4						// max. po�et joystick�
#define JOYSTICKTIME (5*18)					// doba do dal��ho testu joysticku
extern	int		JoystickNext[MAXJOYSTICK];	// ��ta� do dal��ho testu (mus� b�t < 0)
extern	bool	JoystickValid[MAXJOYSTICK];	// p��znak platnosti �daj� joysticku
extern	double	JoystickX[MAXJOYSTICK];		// sou�adnice X
extern	double	JoystickY[MAXJOYSTICK];		// sou�adnice Y
extern	double	JoystickZ[MAXJOYSTICK];		// sou�adnice Z
extern	bool	Joystick1[MAXJOYSTICK];		// tla��tko 1
extern	bool	Joystick2[MAXJOYSTICK];		// tla��tko 2
extern	bool	Joystick3[MAXJOYSTICK];		// tla��tko 3
extern	bool	Joystick4[MAXJOYSTICK];		// tla��tko 4

// prvky oken
extern	CString ButtonClassName;
extern	CString EditClassName;
extern	CString RichEditClassName;
extern	CString ListBoxClassName;
extern	CString ComboBoxClassName;
extern	CString StaticClassName;
extern	CString TabsClassName;

extern	bool	DialogGraph;			// je grafick� pozad� hlavn�ho dialogov�ho okna

extern	bool	DialogMode;				// dialogov� m�d
extern	int		DialogParent;			// aktivn� rodi�ovsk� okno
extern	int		WindowID;				// aktivn� prvek (-1 = neplatn�)
extern	int		WindowFocus;			// prvek s fokusem (-1 = nen� nebo nezn�m�)
extern	int		ButtonEsc;				// tla��tko pro Esc

extern	int		StdColorBtnFace;		// syst�mov� barva podkladu tla��tek
extern	int		StdColorBtnText;		// syst�mov� barva textu tla��tek
extern	HBRUSH	StdBrushBtn;			// �t�tec pozad� tla��tek

extern	int		StdColorWindow;			// syst�mov� barva podkladu editoru
extern	int		StdColorWindowText;		// syst�mov� barva textu editoru
extern	HBRUSH	StdBrushWindow;			// �t�tec pozad� editoru

extern	int		StdColorHighlightText;	// syst�mov� barva textu kurzoru
extern	int		StdColorHighlight;		// syst�mov� barva pozad� kurzoru
extern	HBRUSH	StdBrushHighlight;		// �t�tec pozad� kurzoru

// spr�vce font�
typedef struct FONTITEM_
{
	bool	Bold;					// je zv�razn�n�
	bool	Italic;					// je kurz�va
	bool	Underline;				// je podtr�en�
	bool	Strikeout;				// je p�e�krtnut�
	bool	Serif;					// jsou pati�ky
	bool	Fixed;					// je pevn� rozte�
	int		Height;					// bodov� v��ka znak� (0=b�n�)
	double	Width;					// relativn� ���ka znak� (0=b�n�, 1=z�kladn�)
	double	Angle;					// �hel sklonu v radi�nech (0=b�n�)
	HFONT	Font;					// vytvo�en� font (NULL=voln� polo�ka)
	DWORD	CharSet;				// znakov� sada fontu
	CString	UserFont;				// jm�no u�ivatelsk�ho fontu
	int		Used;					// ��ta� pou�it� fontu (blokov�n� proti zru�en�)
} FONTITEM;

#define FONTTABSIZE	64				// velikost tabulky font�
#define	FONTTABSTD	4				// po�et standardn�ch font� na za��tku tabulky
extern	int			FontNext;		// index p��t�ho voln�ho fontu k ulo�en�
extern	FONTITEM*	FontTab;		// tabulka font�
extern	HFONT		FontDefault;	// implicitn� syst�mov� font (p�i chyb� font�)
extern	CString		UserFont;		// u�ivatelsk� font
extern	CString		FontList;		// seznam syst�mov�ch font�

// oblasti kurzoru my�i
typedef struct MOUSEREG_
{
	int			x1;						// (4) po��tek oblasti vlevo (v�etn�)
	int			y1;						// (4) po��tek oblasti dole (v�etn�)
	int			x2;						// (4) konec oblasti vpravo (bez)
	int			y2;						// (4) konec oblasti naho�e (bez)
	HCURSOR		Cursor;					// (4) handle kurzoru my�i
	int			res;					// (4) ... rezerva pro zarovn�n�
} MOUSEREG;

extern	MOUSEREG*	MouseReg;			// regiony kurzoru my�i
extern	int			MouseRegNum;		// po�et region� kurzoru my�i
extern	int			MouseRegMax;		// velikost bufferu region� kurzoru my�i

// DirectPlay
// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

extern	bool		Coinit;				// prob�hla inicializace COM knihovny

extern	CString	DirectPlayConnects;		// seznam spojen� DirectPlay
extern	CString	DirectPlayConnectsAkt;	// aktivn� spojen� (pr�zdn� = nevybr�no)
extern	CString	DirectPlayGames;		// seznam her DirectPlay
extern	CString	DirectPlayGamesAkt;		// aktivn� hra (pr�zdn� = nevybr�no)
extern	DWORD	DirectPlayGamesParam[4];// parametry hry (15 bit� ��slo + 0x4000, 16. bit = p��znak)
extern	bool	DirectPlayGamesHost;	// po��ta� je hostitelem
extern	int		DirectPlayPlayersMax;	// maxim�ln� po�et hr��� (0 = neomezeno)
extern	CString	DirectPlayPlayers;		// seznam hr��� DirectPlay
extern	CString	DirectPlayPlayersAkt;	// aktivn� hr�� (pr�zdn� = nevybr�n)
extern	int		DirectPlayPlayersAktN;	// aktivn� hr�� (-1 = nen�)
extern	BYTE*	DirectPlayDataIn;		// datov� blok vstupn�ch dat (prvn� 2 bajty = odes�latel)
extern	int		DirectPlayDataInN;		// velikost vstupn�ch dat
extern	int		DirectPlayDataInO;		// offset �ten� ze vstupn�ch dat

extern	GUID DirectPlayGuidData;		// identifik�tor spojen� DirectPlay
extern	CString	DirectPlayGuidText;		// identifika�n� text pro DirectPlay

// WSA 1.1 rozhran� pro DirectPlay
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
// PC form�t IP adresy: prvn� ��slo je nejv��e, tj. nap�. 127.0.0.1 je 0x7f000001
// WSA form�t IP adresy je v MAC form�tu: nap�. 127.0.0.1 je 0x0100007f

// roz���en� WSA 2.2 rozhran� pro UDP
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
extern	WSAHTONS pWSAhtons;				// funkce "htons" (p�evede WORD ��slo portu - na PC prohod� bajty)
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
extern	bool	Wsaload;				// knihovna WSA byla na��t�na
extern	bool	Wsainit;				// knihovna WSA 1.1 �sp�n� inicializov�na
extern	bool	Wsainit2;				// knihovna WSA 2.2 �sp�n� inicializov�na
extern	WSADATA	Wsadata;				// data knihovny WSA
extern	CString	HostIP;					// IP adresa po��ta�e
extern	int		HostIPValid;			// ��ta� platnosti IP adresy po��ta�e (je-li > 0)

// porty (nen� u MINI verze)
typedef struct COMITEM_
{
	HANDLE		File;					// identifik�tor (INVALID_HANDLE_VALUE = nepou�it�)
	DCB			Dcb;					// informace o portu
} COMITEM;

#define	COMSMAX		64					// max. po�et port�
extern	COMITEM*	Coms;				// tabulky port�
extern	int			ComAkt;				// aktivn� port

// mixer (nen� u MINI verze)
typedef struct MIXDEV_
{
	CString	Name;						// jm�no mix�n�ho za��zen�
	int		Dest;						// po�et c�lov�ch sign�l�
} MIXDEV;

typedef struct MIXLINE_
{
	CString Name;						// jm�no sign�lu
	int		Dest;						// index c�lov�ho sign�lu
	int		Source;						// index zdrojov�ho sign�lu (-1=jen c�lov�)
	int		SourceI;					// index po��te�n�ho zdrojov�ho sign�lu
	DWORD	LineID;						// identifik�tor sign�lu
	DWORD	Type;						// typ sign�lu
	int		Channels;					// po�et samostatn�ch kan�l�
	int		Sources;					// po�et p�ipojen�ch zdrojov�ch sign�l�
	int		Controls;					// po�et ovl�dac�ch prvk� sign�lu
} MIXLINE;

typedef struct MIXCTRL_
{
	CString	Name;						// jm�no ovl�dac�ho prvku
	DWORD	ControlID;					// identifik�tor ovl�dac�ho prvku
	DWORD	Type;						// typ ovl�dac�ho prvku
	int		Steps;						// po�et krok� nastaven� hodnoty bez po��te�n� 0 (0=maxim�ln�)
	int		Channels;					// po�et samostatn�ch kan�l�
	int		List;						// po�et prvk� pro seznam (1=nen� seznam)
	double	Min;						// minim�ln� hodnota prvku
	double	Max;						// maxim�ln� hodnota prvku
	double	Rozsah;						// rozsah hodnot prvku
	double	Inc;						// p��rustek kroku prvku
} MIXCTRL;

extern	int		MixDevN;				// po�et mix�n�ch za��zen�
extern	int		MixDevA;				// aktivn� mix�n� za��zen� (-1=nen�)
extern	CString	MixDevT;				// v�ce��dkov� seznam mix�n�ch za��zen�
extern	MIXDEV*	MixDev;					// seznam mix�n�ch za��zen�
extern	HMIXER	MixDevH;				// handle mixeru (NULL=nen�)

extern	int		MixDest;				// po�et c�lov�ch sign�l� mixeru
extern	int		MixLineN;				// po�et sign�l� mixeru
extern	MIXLINE* MixLine;				// seznam sign�l� (na za��tku jsou c�lov�)
extern	CString	MixLineDT;				// v�ce��dkov� seznam c�lov�ch sign�l�
extern	CString	MixLineST;				// v�ce��dkov� seznam zdrojov�ch sign�l�

extern	int		MixLineDA;				// aktivn� c�lov� sign�l
extern	int		MixLineSA;				// aktivn� zdrojov� sign�l (-1=jen c�lov�)
extern	int		MixLineA;				// aktivn� sign�l absolutn� (v tabulce)

extern	int		MixCtrlN;				// po�et ovl�dac�ch prvk� sign�lu
extern	MIXCTRL* MixCtrl;				// seznam ovl�dac�ch prvk�
extern	CString	MixCtrlT;				// v�ce��dkov� seznam ovl�dac�ch prvk�
extern	int		MixCtrlA;				// aktivn� ovl�dac� prvek (-1=nen�)
extern	int		MixChannel;				// nastavovan� kan�l (-1=v�echny)

extern	int		MixValN;				// po�et hodnot prvku (1=nen� seznam hodnot)
extern	CString* MixVal;				// buffer jmen ovl�dac�ch prvk�
extern	CString	MixValT;				// seznam hodnot prvku seznamu
extern	int		MixValA;				// aktivn� prvek seznamu

// obsluha DDE
extern	CString		DDEDefApp;			// implicitn� jm�no aplikace pro DDE

typedef struct DDEAPPITEM_				// polo�ka aplikace
{
	CString		AppName;				// jm�no aplikace
	int			ServFirst;				// index prvn�ho serveru s t�mto jm�nem aplikace
} DDEAPPITEM;

extern	DDEAPPITEM*	DDEAppList;			// seznam aplikac�
extern	int			DDEAppNum;			// po�et aplikac�
extern	int			DDEAppMax;			// velikost bufferu aplikac�
extern	bool		DDEAppLoad;			// na��t� se seznam aplikac�

extern	int			DDEAppAkt;			// aktivn� aplikace (-1=v�echny)
extern	int			DDEServAkt;			// aktivn� server (-1=v�echny)
extern	CString		DDEAppAktName;		// jm�no zadan� aktivn� aplikace/serveru

typedef	struct DDESERVITEM_				// polo�ka serveru
{
	int			AppIndex;				// index aplikace
	CString		ServName;				// pln� jm�no serveru s handle okna ([12345678] Name)
	int			ServNext;				// index dal��ho serveru s t�mto jm�nem aplikace (-1=nen�)
	HWND		ServHandle;				// handle okna serveru
	int			ServServ;				// po�et t�mat, pro kter� je to aktivn� server
	CString		Execute;				// p��kaz k proveden�	
	int			TopicFirst;				// index prvn�ho t�matu tohoto serveru
} DDESERVITEM;

extern	DDESERVITEM* DDEServList;		// seznam sever�
extern	int			DDEServNum;			// po�et server�
extern	int			DDEServMax;			// velikost bufferu server�

typedef struct DDETOPITEM_				// polo�ka t�matu
{
	int			ServIndex;				// index serveru
	CString		TopicName;				// jm�no t�matu
	int			TopicNext;				// index dal��ho t�matu serveru (-1=nen�)
	int			TopicServ;				// po�et polo�ek, pro kter� je t�ma serverem
	int			DataFirst;				// index prvn� polo�ky tohoto t�matu (-1=nen�)
} DDETOPITEM;

extern	DDETOPITEM*	DDETopList;			// seznam t�mat
extern	int			DDETopNum;			// po�et t�mat
extern	int			DDETopMax;			// velikost bufferu t�mat
extern	int			DDETopAkt;			// aktivn� t�ma (-1=v�echny)

typedef struct DDEDATAITEM_				// polo�ka tabulky prvk� DDE
{
	int			TopicIndex;				// index t�matu
	CString		DataName;				// jm�no prvku
	int			DataNext;				// index dal��ho prvku t�matu (-1=nen�)
	int			DataType;				// typ prvku (0 a� 7, -1=neplatn�)
	double		DataNumber;				// 0: ��slo
	bool		DataBool;				// 1: p��znak
	bool		Server;					// prvek je serverem
	bool		res2;
	bool		res3;
	CString		DataText;				// 2: text
	CIcon		DataIcon;				// 3: ikona
	CPicture	DataPicture;			// 4: obr�zek
	CSound		DataSound;				// 5: zvuk
	CMusic		DataMusic;				// 6: hudba
	CSprite		DataSprite;				// 7: sprajt

} DDEDATAITEM;

extern	DDEDATAITEM* DDEDataList;		// seznam datov�ch polo�ek
extern	int			DDEDataNum;			// po�et datov�ch polo�ek
extern	int			DDEDataMax;			// velikost bufferu datov�ch polo�ek
extern	int			DDEDataAkt;			// aktivn� polo�ka (-1=v�echny)

// obsluha DLL
extern	void*		DLLMemoryRead;		// ukazatel �ten� z pam�ti
extern	void*		DLLMemoryWrite;		// ukazatel z�pisu do pam�ti
extern	int			DLLMemoryTextNLen;	// d�lka textu s pevnou d�lkou

// konfigurace
extern CString		INIFile;			// jm�no konfigura�n�ho souboru
extern CString		INISection;			// jm�no konfigura�n� sekce
extern CString		INIKey;				// jm�no konfigura�n�ho parametru
extern int			REGKey;				// skupina registr� (1=CURRENT_USER, 2=LOCAL_MACHINE)
extern CString		REGSubkey;			// jm�no kl��e
extern CString		REGValue;			// jm�no polo�ky

// UDP
extern SOCKET		UDPSendSocket;		// UDP vys�lac� soket (INVALID_SOCKET = nen�)
extern SOCKADDR_IN	UDPSendAddr;		// vys�lac� adresa UDP
extern WSAOVERLAPPED UDPSendOver;		// overlapped struktura vys�la�e
extern WSABUF		UDPSendBuf;			// odes�lac� buffer UDP
extern bool			UDPSending;			// prob�h� vys�l�n� UDP

extern SOCKET		UDPRecSocket;		// UDP p�ij�mac� soket (INVALID_SOCKET = nen�)
extern SOCKADDR_IN	UDPRecAddr;			// p�ij�mac� adresa UDP
extern SOCKADDR_IN	UDPRecRecAddr;		// p�ijat� adresa UDP
extern WSAOVERLAPPED UDPRecOver;		// overlapped struktura p�ij�ma�e
extern WSABUF		UDPRecBuf;			// p�ij�mac� buffer UDP
extern bool			UDPReceiving;		// prob�h� overlapped p��jem UDP
extern int			UDPRecData;			// velikost dat v p�ij�mac�m bufferu

/////////////////////////////////////////////////////////////////////////////
// obsluha DDE

void DDEAppListLoad();					// na�ten� seznamu aplikac�

/////////////////////////////////////////////////////////////////////////////
// obsluha mixeru (nen� u MINI verze)

bool MixDevInit();						// inicializace mix�n�ch za��zen� (vrac� TRUE=OK)
bool MixDevSet(int mix);				// nastaven� aktivn�ho mixeru (vrac� TRUE=OK)
void MixDstSet(int dst);				// nastaven� c�lov�ho sign�lu (-1=��dn�)
void MixSrcSet(int src);				// nastaven� zdrojov�ho sign�lu (-1=jen c�lov�)
void MixCtrlSet(int ctrl);				// nastaven� ovl�dac�ho prvku (-1=��dn�)
double GetMixCtrlVal();					// zji�t�n� hodnoty ovl�dac�ho prvku
void SetMixCtrlVal(double val);			// nastaven� hodnoty ovl�dac�ho prvku

/////////////////////////////////////////////////////////////////////////////
// obsluha port� (nen� u MINI verze)

void ComReinit();						// inicializace p��jmu dat
void ComClose(int i);					// uzav�en� portu (i=platn� index)
void GetComDCB();						// na�ten� DCB aktivn�ho portu
void SetComDCB();						// nastaven� DCB aktivn�ho portu
void ComSend(BYTE data);				// vysl�n� bajtu na komunika�n� port
BYTE ComReceive();						// p��jem bajtu z komunika�n�ho portu (0=nejsou data)

/////////////////////////////////////////////////////////////////////////////
// DirectPlay (nen� u MINI verze)

void DirectSendData(BYTE* data, int n);	// z�pis dat do v�stupn�ho bufferu
void DirectSend(int id);				// vysl�n� bloku dat (-1 = v�em)
void DirectReceiveData(BYTE* data, int n); // �ten� dat ze vstupn�ho bufferu (p�i nedostatku dat dopln� nulami)
int DirectReceive();					// p��jem bloku dat (-1 = nen�)
void SetPlayerAct(CString txt);			// nastaven� aktivn�ho hr��e
bool DirectPlayEnumPlayers();			// seznam hr��� (FALSE=chyba)
void SetPlayerMax(int n);				// nastaven� max. hr��� (0=neomezeno)
void GetGameDesc();						// na�ten� parametr� hry
void SetGameN(int n, int i);			// nastaven� ��seln� hodnoty hry (n = rozsah -16384 a� +16383, i = index 0 a� 7)
void SetGameL(bool n, int i);			// nastaven� p�ep�na�e hry (i = index 0 a� 7)
void SetGameAct(CString txt);			// nastaven� aktivn� hry
bool DirectPlayEnumGames();				// seznam her (FALSE=chyba)
void SetConnectAct(CString txt);		// nastaven� aktivn�ho spojen�
bool DirectPlayConInit();				// inicializace seznamu spojen� (FALSE=chyba)
void DirectPlayConUninit();				// ukon�en� seznamu spojen�
bool DirectPlayInit();					// inicializace objektu DirectPlay (FALSE=chyba)
void DirectPlayUninit();				// uvoln�n� objektu DirectPlay
bool COMInit();							// inicializace knihovny COM (FALSE=chyba)
void COMUninit();						// uvoln�n� knihovny COM

/////////////////////////////////////////////////////////////////////////////
// UDP

void WSAInitialize();					// inicializace WSA knihovny
bool WSAInitialize1();					// inicializace WSA 1.1 knihovny (vrac� TRUE je-li OK)
bool WSAInitialize2();					// inicializace WSA 2.2 knihovny (vrac� TRUE je-li OK)
void WSADeinitialize();					// deinicializace WSA knihovny

void InitUDP();							// inicializace obsluhy UDP (inicializace buffer�)
bool OpenSendUDP();						// otev�en� vys�lac�ho soketu UDP (p�i chyb� vrac� FALSE)
void CloseSendUDP();					// zav�en� vys�lac�ho soketu UDP
bool OpenRecUDP();						// otev�en� p�ij�mac�ho soketu UDP (p�i chyb� vrac� FALSE)
void CloseRecUDP();						// zav�en� p�ij�mac�ho soketu UDP
void SendUDP(const void* data, int len);// odesl�n� UDP paketu
#ifdef UDP_OVER
void StartRecUDP();						// start p��jmu UDP paketu
#endif
int RecUDP();							// p��jem UDP paketu (vrac� velikost dat v p�ij�mac�m bufferu)

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------

/////////////////////////////////////////////////////////////////////////////
// dialogy

void DialogOn(bool on);					// re�im dialogu
void WindowNew(int typ, int styl);		// vytvo�en� okna
HFONT GetFont(bool bold, bool italic, bool underline, bool strikeout,
			  bool serif, bool fixed, int height, double width, double angle); // vytvo�en� fontu (nutn� uvoln�n� pomoc� FreeFont)
void FreeFont(HFONT font);				// uvoln�n� fontu p�id�len�ho s GetFont
void WindowSetFont(WINITEM* item);	// aktualizace fontu
void WindowItemNew(int typ, CString& classname, int width, int height, int style, int exstyle); // vytvo�en� prvku okna
void WindowDel(int inx);				// zru�en� prvku
void FocusNext();						// posun fokusu na dal�� prvek
void FocusPrev();						// posun fokusu na p�edchoz� prvek
void RadioNext();						// posun fokusu na dal�� p�ep�na�
void RadioPrev();						// posun fokusu na p�edchoz� p�ep�na�
void RadioSet(int inx);					// nastaven� p�ep�na�e


/////////////////////////////////////////////////////////////////////////////
// joystick

void AktJoystick(int inx);				// aktualizace �daj� o joysticku


/////////////////////////////////////////////////////////////////////////////
// konzola

void ConsoleOnSet(bool on);			// zapnut�/vypnut� konzoly
void ConsoleOut(const CString& text);	// v�stup na konzolu
void ConsoleErr(const CString& text);	// chybov� v�stup na konzolu
void ConsoleIn(CString& text);		// vstup textu z konzoly


/////////////////////////////////////////////////////////////////////////////
// obsluha CD disk�

void CDPlay(int stopa);				// zah�jen� p�ehr�v�n� od zadan� stopy
void CDSetTrack(int stopa);			// nastaven� stopy
void CDStop();						// zastaven� p�ehr�v�n� CD
void CDEject(bool eject);			// vysunut� CD
void CDPause();						// pauza CD
void CDSetPos(int pozice);			// nastaven� pozice CD v ms + p�ehr�v�n�
void CDSetTrackPos(int pozice);		// nastaven� pozice v aktivn� stop� CD v ms + p�ehr�v�n�
void CDClose();						// uzav�en� ovlada�e CD
bool CDOpen();						// otev�en� ovlada�e CD
bool CDAkt();						// aktualizace �daj� o CD disku


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� informac� o velikosti a voln�m m�stu aktivn�ho disku

void GetDiskSpace();


/////////////////////////////////////////////////////////////////////////////
// obsluha soubor�

bool FileReadOpen();				// otev�en� souboru pro �ten�
bool FileWriteOpen();				// otev�en� souboru pro z�pis
bool FileWriteFlush();				// vypr�zdn�n� z�pisov�ho bufferu
void _fastcall FileWriteBlok(BYTE* buf, int size); // z�pis bloku do souboru
void FileWriteByte(BYTE data);		// z�pis bajtu do v�stupn�ho souboru
void FileWriteWord(WORD data);		// z�pis slova do v�stupn�ho souboru
void FileWriteDWord(DWORD data);	// z�pis dvojslova do v�stupn�ho souboru
WORD FileReadWord();				// �ten� slova ze vstupn�ho souboru
BYTE FileReadByte();				// �ten� bajtu ze vstupn�ho souboru
void FileClose();					// uzav�en� soubor�
void FileReadBlok(BYTE* buf, int size); //na�ten� bloku ze souboru
void _fastcall InitAlias(CString name);		// p��prava jm�na ALIAS
void _fastcall InitAliasGroup(int lan);		// inicializace skupiny ALIAS

/////////////////////////////////////////////////////////////////////////////
// prov�d�n� programu

void Exec();


/////////////////////////////////////////////////////////////////////////////
// na�ten� sou�adnic my�i

void GetAktMousePos();


/////////////////////////////////////////////////////////////////////////////
// inicializace informac� o plo�e

void ExecInitMap();


/////////////////////////////////////////////////////////////////////////////
// nastavov�n� rozm�r� okna

void OnSizing(RECT* rc, int side);


/////////////////////////////////////////////////////////////////////////////
// aktualizace rozm�r� okna

void _fastcall OnSize(WINITEM* item);


/////////////////////////////////////////////////////////////////////////////
// p�ekreslen� okna

void _fastcall OnPaint(WINITEM* item, int inx);


/////////////////////////////////////////////////////////////////////////////
// p�ekreslen� cel�ho okna

void _fastcall RePaint(WINITEM* item);


/////////////////////////////////////////////////////////////////////////////
// aktualiza�n� p�ekreslen� okna

void ReDisp();


/////////////////////////////////////////////////////////////////////////////
// �ek�n� po zadanou dobu

void _fastcall TimeWait(int count);


/////////////////////////////////////////////////////////////////////////////
// obsluha �asova�e

void BackTimer();


/////////////////////////////////////////////////////////////////////////////
// test �asova�e

BOOL TestTimer();


/////////////////////////////////////////////////////////////////////////////
// �ek�n� na sign�l od �asova�e s obsluhou zpr�v

void WaitTimer(bool wait);


/////////////////////////////////////////////////////////////////////////////
// p�esun sprajtu

void SpriteMove(int index, double cilx, double cily);


/////////////////////////////////////////////////////////////////////////////
// nastaven� sm�ru sprajtu

void SetSpriteSmer(int index, double smer);


/////////////////////////////////////////////////////////////////////////////
// nastaven� viditelnosti sprajtu

void SetSpriteVisible(int index, bool visible);


/////////////////////////////////////////////////////////////////////////////
// nastaven� sprajtu

void SetSprite(int inx, const CSprite& src);


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� p��t� f�ze sprajtu (pro prom�nnou f�ze sprajtu)

int SpriteNextFaze(int index);


/////////////////////////////////////////////////////////////////////////////
// zaji�t�n� vymaz�n� sprajtu (po�adavek p�ekreslen� ikon)

void _fastcall SpriteClear(int index);


/////////////////////////////////////////////////////////////////////////////
// nastaven� po�adavku p�ekreslen� ikony (bez kontroly sou�adnic)

inline void SetAktIcon(int x, int y, bool b)
{
	AktBuf[x + WidthN*y] = b;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� po�adavku p�ekreslen� v�ech ikon

void AktAllIcon();


/////////////////////////////////////////////////////////////////////////////
// nastaven� po�adavku p�ekreslen� ikony v intervalu (v�etn� koncov�ch bod�)

void _fastcall AktIconBoxG(int x1, int y1, int x2, int y2);
void _fastcall AktIconBox(int x1, int y1, int x2, int y2);
