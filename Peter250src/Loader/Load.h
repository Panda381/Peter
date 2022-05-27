
/***************************************************************************\
*																			*
*								Obsluha souboru								*
*																			*
\***************************************************************************/

// Zm�ny p�i distribu�n� verzi:
//    - zm�n� se blok programu z "petprg" na "rtexts"
//    - zm�n� se identifikace z "PET" na "REL"
//    - v�echna jm�na datov�ch blok� v z�hlav� se nastav� na 0
//    - importovac� tabulka je pr�zdn�
//    - vypust� se v�echny texty jmen polo�ek krom�:
//				- hlavn� funkce
//				- IDF_TEXT_CONST
//				- IDF_TEXT_LINE
//				- IDF_NUM
//				- IDF_LIST_SIZE

/////////////////////////////////////////////////////////////////////////////
// index datov�ho bloku (32 B)

typedef struct PETINDEX_ {
	long				Delka;		// (4) d�lka datov�ho bloku (bajt�)
	long				Pocet;		// (4) po�et polo�ek v datov�m bloku
	char				Jmeno[8];	// (8) jm�no datov�ho bloku (8 znak�)
	long				Verze;		// (4) dopl�uj�c� informace 1 (= 0) (verze)
									//		obr�zky:	1=je komprimace
									//		ikony	:	1=je komprimace
									//		sprajty:	1=je komprimace
									//		texty:		1=je v�cejazy�n� verze
									//					0=je jeden jazyk UNICODE
	long				Extra2;		// (4) dopl�uj�c� informace 2 (= 0)
									//		texty:		po�et jazyk�
									//		import:		licen�n� ��slo u�ivatele
	long				Extra3;		// (4) dopl�uj�c� informace 3 (= 0)
	long				Extra4;		// (4) dopl�uj�c� informace 4 (= 0)
} PETINDEX;

#define	SIZEOFPETINDEX	(6*sizeof(long) + 8*sizeof(char))	// velikost indexu


/////////////////////////////////////////////////////////////////////////////
// z�hlav� souboru (16 + NUMOFINDEX*32 B) (indexy buffer� mus� souhlasit s Buf???ID !!!)

#define NUMOFINDEX 16				// po�et index� pro ukl�d�n�

typedef struct PETHEAD_ {
	char				Ident[3];		// (3) identifikace = "PET"
	BYTE				Verze;			// (1) verze = 1
	WORD				Editor;			// (2) verze editoru v tis�cin�ch
	WORD				Param;			// (2) parametry = 0
	long				Data;			// (4) offset za��tku dat od za��tku z�hlav� (= d�lka z�hlav�)
	long				Pocet;			// (4) po�et datov�ch blok�
	PETINDEX			pi[NUMOFINDEX];	// tabulka index�
#define	piImport		pi[BufIntID]	// 0: (32) blok "IMPORT  " - import intern�ch prvk� (seznam ASCIIZ jmen)
#define	piClass			pi[BufClsID]	// 1: (32) blok "CLASS   " - t��dy
#define	piGlobal		pi[BufObjID]	// 2: (32) blok "GLOBAL  " - glob�ln� objekty
#define	piLocal			pi[BufLocID]	// 3: (32) blok "LOCAL   " - lok�ln� objekty
#define	piProgram		pi[BufEdiID]	// 4: (32) blok "PROGRAM " - program (obsah funkc�)
#define	piStruc			pi[BufStrID]	// 5: (32) blok "STRUC   " - z�kladn� struktury
#define	piReal			pi[BufNumID]	// 6: (32) blok "REAL    " - obsah ��seln�ch prom�nn�ch
#define	piText			pi[BufTxtID]	// 7: (32) blok "TEXT    " - texty (text 0 = jm�no programu)
#define	piBool			pi[BufLogID]	// 8: (32) blok "BOOL    " - logick� hodnoty (bajty)
#define	piIcon			pi[BufIcoID]	// 9: (32) blok "PICTURE " - obr�zky 32*32 (obr�zek 0 = ikona programu)
#define	piMap			pi[BufMapID]	// 10: (32) blok "MAP     " - mapy ploch
#define	piPic			pi[BufPicID]	// 11: (32) blok "BACKGRND" - pozad�
#define piSprite		pi[BufSprID]	// 12: (32) blok "SPRITE  " - sprajty
#define	piSound			pi[BufSndID]	// 13: (32) blok "SOUND   " - zvuky
#define	piMusic			pi[BufMusID]	// 14: (32) blok "MUSIC   " - hudba
#define	piPalette		pi[BufPalID]	// 15: (32) blok "PALETTE " - palety ve form�tu BMP
} PETHEAD;

#define SIZEOFPETHEAD (3*sizeof(char) + sizeof(BYTE) + 2*sizeof(WORD) + 2*sizeof(long))	// z�kladn� velikost z�hlav� (bez index�)

/////////////////////////////////////////////////////////////////////////////
// polo�ka programu v souboru (32 B) - pou��v� se i pro clipboard
 
typedef struct PETPROG_ {
	long			Param;			// (4) parametry

	long			RefBlok;		// (4) blok s deklarac� (-1 = nen�)
	long			RefIndex;		// (4) index s deklarac� (-1 = nen�)
									//		pro clipboard d�lka jm�na v bajtech
	long			DatBlok;		// (4) blok s daty (-1 = nen�)
	long			DatIndex;		// (4) index s daty (-1 = nen�)
									//		pro clipboard d�lka dat v bajtech
	long			Icon;			// (4) ikona (-1 = implicitn�)
									//		pro clipboard d�lka dat ikony v bajtech
	long			Name;			// (4) text jm�na (-1 = implicitn�)
									//		pro clipboard d�lka jm�na v bajtech
	long			Func;			// (4) ��slo funkce pro loader a clipboard (��slov�no od 0)
} PETPROG;

#define SIZEOFPETPROG (8*sizeof(long))	// velikost polo�ky programu

// parametry:
#define PETPROG_CHILDS	0x0001		// p��znak, �e jsou potomci
#define PETPROG_NEXT	0x0002		// p��znak, �e bude potomek stejn� hladiny
#define PETPROG_EXP		0x0004		// p��znak rozvinut� potomk�
#define PETPROG_LOCK	0x0008		// p��znak uzamknut� (zv�razn�n�)
#define PETPROG_OFF		0x0010		// p��znak vypnut� (ze�ednut�)
#define PETPROG_NOMOVE	0x0020		// p��znak z�kazu p�esunu polo�ky
#define PETPROG_INTERN	0x0040		// p��znak intern� polo�ky (neru�it)
#define	PETPROG_OFF_DEP	0x0080		// p��znak z�visl�ho vypnut� (pou��v� loader)


/////////////////////////////////////////////////////////////////////////////
// polo�ka dopl�uj�c�ch parametr� zdrojov�ho programu (velikost 32 bajt�)

typedef struct PETPROG2_ {
	int				Parent;			// (4) index rodi�e (-1 = ROOT)
	int				Items;			// (4) po�et polo�ek ve v�tvi v�etn� t�to, minim�ln� 1
	int				Data;			// (4) index dat (-1 = nen�)
	int				List;			// (4) index seznamu (-1 = nen�)
	int				Local;			// (4) relativn� index lok�ln� prom�nn� (-1=nen�)
	int				LocalList;		// (4) relativn� index lok�ln�ho seznamu (-1=nen�)
	int				Funkce;			// (4) index funkce v programu (-1 = nezn�m�)
	int				res;
} PETPROG2;


/////////////////////////////////////////////////////////////////////////////
// polo�ka plochy v souboru (prvek plochy je stejn� jako pro CMap)

typedef struct MAPPROG_ {
	long			Width;			// (4) ���ka plochy (ikon)
	long			Height;			// (4) v��ka plochy (ikon)
	MAPITEM			Data[1];		// polo�ky plochy
} MAPPROG;

#define SIZEOFMAPPROG (2*sizeof(long))


/////////////////////////////////////////////////////////////////////////////
// popisova� jazyku textu - jen pro v�cejazy�nou verzi

typedef struct TEXTPROG_ {
	long			LangID;			// (4) identifik�tor jazyku (0=fiktivn� jazyk pro star�� verze)
	long			CodePage;		// (4) k�dov� str�nka jazyku (0=UNICODE)
} TEXTPROG;

#define SIZEOFTEXTPROG (2*sizeof(long))


/////////////////////////////////////////////////////////////////////////////
// polo�ka obr�zku v souboru (obr�zky jsou komprimov�ny!)

typedef struct PICPROG_ {
	long			Width;		// ���ka obr�zku v bodech
	long			Height;		// v��ka obr�zku v bodech
	BYTE			Data[1];	// data (velikost Width*Height)
								//	na za��tku dat je dvouslovo=velikost komprimovan�ch dat,
								//  n�sleduj� komprimovan� data obr�zku
} PICPROG;

#define SIZEOFPICPROG (2*sizeof(long))


/////////////////////////////////////////////////////////////////////////////
// polo�ka sprajtu v souboru 

typedef struct SPRITEPROG_ {
	long			Faze;			// (4) po�et f�z� celkem
	long			Smer;			// (4) po�et sm�r�
	long			Klid;			// (4) z toho po�et klidov�ch f�z�
	WORD			Width;			// (2) ���ka obr�zku
	WORD			Height;			// (2) v��ka obr�zku
	long			Delay;			// (4) prodleva mezi dv�ma f�zemi (milisekund)
	long			Level;			// (4) hladina k p�ekreslov�n� (0 = p�edm�ty)
	double			Kroku;			// (8) po�et f�z� na krok (0 = ihned)
	BYTE			Data[1];		// data - obr�zky sprajtu (po�ad�: f�ze, sm�r)
} SPRITEPROG;

#define SIZEOFSPRITEPROG (5*sizeof(long) + 2*sizeof(WORD) + sizeof(double))


/////////////////////////////////////////////////////////////////////////////
// polo�ka zvuku v souboru

// verze souboru 0
typedef struct SOUND0PROG_ {
// popisova� souboru
	char	tWavIdent[4];		// (4) identifikace souboru (= "RIFF")
	DWORD	nFileSize;			// (4) velikost souboru bez 8 bajt� (na toto z�hlav�)

// popisova� form�tu
	char	tFormatIdent[8];	// (8) identifikace z�hlav� form�tu (= "WAVEfmt")
	DWORD	nFormatSize;		// (4) velikost n�sleduj�c�ch dat popisova�e form�tu (= 0x10)
	WORD	wFormatTag;			// (2) form�t dat (1 = PCM)
	WORD	nChannels;			// (2) po�et kan�l� (1 = mono, 2 = stereo)
	DWORD	nSamplesPerSec;		// (4) vzorkovac� kmito�et (vzork� za sekundu)
	DWORD	nAvgBytesPerSec;	// (4) p�enosov� rychlost (bajt� za sekundu)
	WORD	nBlockAlign;		// (2) zarovn�v�n� dat (bity*kan�ly/8)
	WORD	wBitsPerSample;		// (2) po�et bit� na jeden vzorek

// popisova� dat
	char	tDataIdent[4];		// (4) identifikace dat "data"
	DWORD	nDataSize;			// (4) velikost n�sleduj�c�ch dat (v bajtech)
} SOUND0PROG;

#define SIZEOFSOUND0PROG sizeof(SOUND0PROG)

// ostatn� verze
typedef struct SOUNDPROG_ {
	long	Size;				// (4) velikost dat (bajt�) - v�etn� p��padn�ho z�hlav� WAVEFORMATEX
	DWORD	Samples;			// (4) vzorkovac� kmito�et
	WORD	Format;				// (2) form�t dat (1 = PCM)
								//		pro jin� form�t ne� PCM je na za��tku dat popisova�
								//		form�tu WAVEFORMATEX s udanou velikost� dopl�uj�c�ch
								//		dat "cbSize", za popisova�em n�sleduj� data
	BYTE	Channels;			// (1) po�et kan�l� (1 = mono, 2 = stereo)
	BYTE	Bits;				// (1) po�et bit� na vzorek kan�lu (8 nebo 16)
	BYTE	Data[1];			// data (p��p. popisova� WAVEFORMATEX + data)
} SOUNDPROG;

#define SIZEOFWAVEFORMATEX 18	// velikost z�hlav� WAVEFORMATEX

#define SIZEOFSOUNDPROG (sizeof(long) + sizeof(DWORD) + sizeof(WORD) + 2*sizeof(BYTE))

/////////////////////////////////////////////////////////////////////////////
// prom�nn�

// zdrojov� buffer glob�ln�ch objekt�
extern		int			BufObjN;
extern		PETPROG*	BufObj;
extern		PETPROG2*	BufObj2;

// zdrojov� buffer lok�ln�ch objekt�
extern		int			BufLocN;
extern		PETPROG*	BufLoc;
extern		PETPROG2*	BufLoc2;

// zdrojov� buffer editoru
extern		int			BufEdiN;
extern		PETPROG*	BufEdi;
extern		PETPROG2*	BufEdi2;


/////////////////////////////////////////////////////////////////////////////
// na�ten� textov� konstanty (ukazatel� ukazuj� na polo�ku konstanty)

double LoadNum(PETPROG* prog, PETPROG2* prog2);


/////////////////////////////////////////////////////////////////////////////
// na�ten� jednoho textu z programu

CString LoadText0();


/////////////////////////////////////////////////////////////////////////////
// na�ten� programu

void Load();
