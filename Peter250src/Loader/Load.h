
/***************************************************************************\
*																			*
*								Obsluha souboru								*
*																			*
\***************************************************************************/

// Zmìny pøi distribuèní verzi:
//    - zmìní se blok programu z "petprg" na "rtexts"
//    - zmìní se identifikace z "PET" na "REL"
//    - všechna jména datových blokù v záhlaví se nastaví na 0
//    - importovací tabulka je prázdná
//    - vypustí se všechny texty jmen položek kromì:
//				- hlavní funkce
//				- IDF_TEXT_CONST
//				- IDF_TEXT_LINE
//				- IDF_NUM
//				- IDF_LIST_SIZE

/////////////////////////////////////////////////////////////////////////////
// index datového bloku (32 B)

typedef struct PETINDEX_ {
	long				Delka;		// (4) délka datového bloku (bajtù)
	long				Pocet;		// (4) poèet položek v datovém bloku
	char				Jmeno[8];	// (8) jméno datového bloku (8 znakù)
	long				Verze;		// (4) doplòující informace 1 (= 0) (verze)
									//		obrázky:	1=je komprimace
									//		ikony	:	1=je komprimace
									//		sprajty:	1=je komprimace
									//		texty:		1=je vícejazyèná verze
									//					0=je jeden jazyk UNICODE
	long				Extra2;		// (4) doplòující informace 2 (= 0)
									//		texty:		poèet jazykù
									//		import:		licenèní èíslo uživatele
	long				Extra3;		// (4) doplòující informace 3 (= 0)
	long				Extra4;		// (4) doplòující informace 4 (= 0)
} PETINDEX;

#define	SIZEOFPETINDEX	(6*sizeof(long) + 8*sizeof(char))	// velikost indexu


/////////////////////////////////////////////////////////////////////////////
// záhlaví souboru (16 + NUMOFINDEX*32 B) (indexy bufferù musí souhlasit s Buf???ID !!!)

#define NUMOFINDEX 16				// poèet indexù pro ukládání

typedef struct PETHEAD_ {
	char				Ident[3];		// (3) identifikace = "PET"
	BYTE				Verze;			// (1) verze = 1
	WORD				Editor;			// (2) verze editoru v tisícinách
	WORD				Param;			// (2) parametry = 0
	long				Data;			// (4) offset zaèátku dat od zaèátku záhlaví (= délka záhlaví)
	long				Pocet;			// (4) poèet datových blokù
	PETINDEX			pi[NUMOFINDEX];	// tabulka indexù
#define	piImport		pi[BufIntID]	// 0: (32) blok "IMPORT  " - import interních prvkù (seznam ASCIIZ jmen)
#define	piClass			pi[BufClsID]	// 1: (32) blok "CLASS   " - tøídy
#define	piGlobal		pi[BufObjID]	// 2: (32) blok "GLOBAL  " - globální objekty
#define	piLocal			pi[BufLocID]	// 3: (32) blok "LOCAL   " - lokální objekty
#define	piProgram		pi[BufEdiID]	// 4: (32) blok "PROGRAM " - program (obsah funkcí)
#define	piStruc			pi[BufStrID]	// 5: (32) blok "STRUC   " - základní struktury
#define	piReal			pi[BufNumID]	// 6: (32) blok "REAL    " - obsah èíselných promìnných
#define	piText			pi[BufTxtID]	// 7: (32) blok "TEXT    " - texty (text 0 = jméno programu)
#define	piBool			pi[BufLogID]	// 8: (32) blok "BOOL    " - logické hodnoty (bajty)
#define	piIcon			pi[BufIcoID]	// 9: (32) blok "PICTURE " - obrázky 32*32 (obrázek 0 = ikona programu)
#define	piMap			pi[BufMapID]	// 10: (32) blok "MAP     " - mapy ploch
#define	piPic			pi[BufPicID]	// 11: (32) blok "BACKGRND" - pozadí
#define piSprite		pi[BufSprID]	// 12: (32) blok "SPRITE  " - sprajty
#define	piSound			pi[BufSndID]	// 13: (32) blok "SOUND   " - zvuky
#define	piMusic			pi[BufMusID]	// 14: (32) blok "MUSIC   " - hudba
#define	piPalette		pi[BufPalID]	// 15: (32) blok "PALETTE " - palety ve formátu BMP
} PETHEAD;

#define SIZEOFPETHEAD (3*sizeof(char) + sizeof(BYTE) + 2*sizeof(WORD) + 2*sizeof(long))	// základní velikost záhlaví (bez indexù)

/////////////////////////////////////////////////////////////////////////////
// položka programu v souboru (32 B) - používá se i pro clipboard
 
typedef struct PETPROG_ {
	long			Param;			// (4) parametry

	long			RefBlok;		// (4) blok s deklarací (-1 = není)
	long			RefIndex;		// (4) index s deklarací (-1 = není)
									//		pro clipboard délka jména v bajtech
	long			DatBlok;		// (4) blok s daty (-1 = není)
	long			DatIndex;		// (4) index s daty (-1 = není)
									//		pro clipboard délka dat v bajtech
	long			Icon;			// (4) ikona (-1 = implicitní)
									//		pro clipboard délka dat ikony v bajtech
	long			Name;			// (4) text jména (-1 = implicitní)
									//		pro clipboard délka jména v bajtech
	long			Func;			// (4) èíslo funkce pro loader a clipboard (èíslováno od 0)
} PETPROG;

#define SIZEOFPETPROG (8*sizeof(long))	// velikost položky programu

// parametry:
#define PETPROG_CHILDS	0x0001		// pøíznak, že jsou potomci
#define PETPROG_NEXT	0x0002		// pøíznak, že bude potomek stejné hladiny
#define PETPROG_EXP		0x0004		// pøíznak rozvinutí potomkù
#define PETPROG_LOCK	0x0008		// pøíznak uzamknutí (zvýraznìní)
#define PETPROG_OFF		0x0010		// pøíznak vypnutí (zešednutí)
#define PETPROG_NOMOVE	0x0020		// pøíznak zákazu pøesunu položky
#define PETPROG_INTERN	0x0040		// pøíznak interní položky (nerušit)
#define	PETPROG_OFF_DEP	0x0080		// pøíznak závislého vypnutí (používá loader)


/////////////////////////////////////////////////////////////////////////////
// položka doplòujících parametrù zdrojového programu (velikost 32 bajtù)

typedef struct PETPROG2_ {
	int				Parent;			// (4) index rodièe (-1 = ROOT)
	int				Items;			// (4) poèet položek ve vìtvi vèetnì této, minimálnì 1
	int				Data;			// (4) index dat (-1 = není)
	int				List;			// (4) index seznamu (-1 = není)
	int				Local;			// (4) relativní index lokální promìnné (-1=není)
	int				LocalList;		// (4) relativní index lokálního seznamu (-1=není)
	int				Funkce;			// (4) index funkce v programu (-1 = neznámý)
	int				res;
} PETPROG2;


/////////////////////////////////////////////////////////////////////////////
// položka plochy v souboru (prvek plochy je stejný jako pro CMap)

typedef struct MAPPROG_ {
	long			Width;			// (4) šíøka plochy (ikon)
	long			Height;			// (4) výška plochy (ikon)
	MAPITEM			Data[1];		// položky plochy
} MAPPROG;

#define SIZEOFMAPPROG (2*sizeof(long))


/////////////////////////////////////////////////////////////////////////////
// popisovaè jazyku textu - jen pro vícejazyènou verzi

typedef struct TEXTPROG_ {
	long			LangID;			// (4) identifikátor jazyku (0=fiktivní jazyk pro starší verze)
	long			CodePage;		// (4) kódová stránka jazyku (0=UNICODE)
} TEXTPROG;

#define SIZEOFTEXTPROG (2*sizeof(long))


/////////////////////////////////////////////////////////////////////////////
// položka obrázku v souboru (obrázky jsou komprimovány!)

typedef struct PICPROG_ {
	long			Width;		// šíøka obrázku v bodech
	long			Height;		// výška obrázku v bodech
	BYTE			Data[1];	// data (velikost Width*Height)
								//	na zaèátku dat je dvouslovo=velikost komprimovaných dat,
								//  následují komprimovaná data obrázku
} PICPROG;

#define SIZEOFPICPROG (2*sizeof(long))


/////////////////////////////////////////////////////////////////////////////
// položka sprajtu v souboru 

typedef struct SPRITEPROG_ {
	long			Faze;			// (4) poèet fází celkem
	long			Smer;			// (4) poèet smìrù
	long			Klid;			// (4) z toho poèet klidových fází
	WORD			Width;			// (2) šíøka obrázku
	WORD			Height;			// (2) výška obrázku
	long			Delay;			// (4) prodleva mezi dvìma fázemi (milisekund)
	long			Level;			// (4) hladina k pøekreslování (0 = pøedmìty)
	double			Kroku;			// (8) poèet fází na krok (0 = ihned)
	BYTE			Data[1];		// data - obrázky sprajtu (poøadí: fáze, smìr)
} SPRITEPROG;

#define SIZEOFSPRITEPROG (5*sizeof(long) + 2*sizeof(WORD) + sizeof(double))


/////////////////////////////////////////////////////////////////////////////
// položka zvuku v souboru

// verze souboru 0
typedef struct SOUND0PROG_ {
// popisovaè souboru
	char	tWavIdent[4];		// (4) identifikace souboru (= "RIFF")
	DWORD	nFileSize;			// (4) velikost souboru bez 8 bajtù (na toto záhlaví)

// popisovaè formátu
	char	tFormatIdent[8];	// (8) identifikace záhlaví formátu (= "WAVEfmt")
	DWORD	nFormatSize;		// (4) velikost následujících dat popisovaèe formátu (= 0x10)
	WORD	wFormatTag;			// (2) formát dat (1 = PCM)
	WORD	nChannels;			// (2) poèet kanálù (1 = mono, 2 = stereo)
	DWORD	nSamplesPerSec;		// (4) vzorkovací kmitoèet (vzorkù za sekundu)
	DWORD	nAvgBytesPerSec;	// (4) pøenosová rychlost (bajtù za sekundu)
	WORD	nBlockAlign;		// (2) zarovnávání dat (bity*kanály/8)
	WORD	wBitsPerSample;		// (2) poèet bitù na jeden vzorek

// popisovaè dat
	char	tDataIdent[4];		// (4) identifikace dat "data"
	DWORD	nDataSize;			// (4) velikost následujících dat (v bajtech)
} SOUND0PROG;

#define SIZEOFSOUND0PROG sizeof(SOUND0PROG)

// ostatní verze
typedef struct SOUNDPROG_ {
	long	Size;				// (4) velikost dat (bajtù) - vèetnì pøípadného záhlaví WAVEFORMATEX
	DWORD	Samples;			// (4) vzorkovací kmitoèet
	WORD	Format;				// (2) formát dat (1 = PCM)
								//		pro jiný formát než PCM je na zaèátku dat popisovaè
								//		formátu WAVEFORMATEX s udanou velikostí doplòujících
								//		dat "cbSize", za popisovaèem následují data
	BYTE	Channels;			// (1) poèet kanálù (1 = mono, 2 = stereo)
	BYTE	Bits;				// (1) poèet bitù na vzorek kanálu (8 nebo 16)
	BYTE	Data[1];			// data (pøíp. popisovaè WAVEFORMATEX + data)
} SOUNDPROG;

#define SIZEOFWAVEFORMATEX 18	// velikost záhlaví WAVEFORMATEX

#define SIZEOFSOUNDPROG (sizeof(long) + sizeof(DWORD) + sizeof(WORD) + 2*sizeof(BYTE))

/////////////////////////////////////////////////////////////////////////////
// promìnné

// zdrojový buffer globálních objektù
extern		int			BufObjN;
extern		PETPROG*	BufObj;
extern		PETPROG2*	BufObj2;

// zdrojový buffer lokálních objektù
extern		int			BufLocN;
extern		PETPROG*	BufLoc;
extern		PETPROG2*	BufLoc2;

// zdrojový buffer editoru
extern		int			BufEdiN;
extern		PETPROG*	BufEdi;
extern		PETPROG2*	BufEdi2;


/////////////////////////////////////////////////////////////////////////////
// naètení textové konstanty (ukazatelé ukazují na položku konstanty)

double LoadNum(PETPROG* prog, PETPROG2* prog2);


/////////////////////////////////////////////////////////////////////////////
// naètení jednoho textu z programu

CString LoadText0();


/////////////////////////////////////////////////////////////////////////////
// naètení programu

void Load();
