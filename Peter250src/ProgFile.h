
/***************************************************************************\
*																			*
*								Obsluha souboru								*
*																			*
\***************************************************************************/

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

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Na p�echodnou dobu (pro zabr�n�n� hav�ri� u star��ch verz�) bude
// jako 1. text ukl�d�n fiktivn� jazyk s pr�zdn�mi texty a LangID = 0
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#define	SIZEOFPETINDEX	(6*sizeof(long) + 8*sizeof(char))	// velikost indexu


/////////////////////////////////////////////////////////////////////////////
// z�hlav� souboru (16 + NUMOFINDEX*32 B) (indexy buffer� mus� souhlasit s Buf???ID !!!)

#define NUMOFINDEX 16				// po�et index� pro ukl�d�n�

typedef struct PETHEAD_ {
	char				Ident[3];		// (3) identifikace = "PET"
	BYTE				Verze;			// (1) verze souboru = 1
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
// polo�ka plochy v souboru (prvek plochy je stejn� jako pro CMap)

typedef struct MAPPROG_ {
	long			Width;			// (4) ���ka plochy (ikon)
	long			Height;			// (4) v��ka plochy (ikon)
	MAPITEM			Data[1];		// polo�ky plochy
} MAPPROG;

#define SIZEOFMAPPROG (2*sizeof(long))

extern	IMAGE_SECTION_HEADER	PetProgHeader;

/////////////////////////////////////////////////////////////////////////////
// popisova� jazyku textu - jen pro v�cejazy�nou verzi

typedef struct TEXTPROG_ {
	long			LangID;			// (4) identifik�tor jazyku (0=fiktivn� jazyk pro star�� verze)
	long			CodePage;		// (4) k�dov� str�nka jazyku (0=UNICODE)
} TEXTPROG;

#define SIZEOFTEXTPROG (2*sizeof(long))

/////////////////////////////////////////////////////////////////////////////
// polo�ka obr�zku v souboru

typedef struct PICPROG_ {
	long			Width;		// ���ka obr�zku v bodech
	long			Height;		// v��ka obr�zku v bodech
	BYTE			Data[1];	// data 
								//		m�d komprimace:		(4) d�lka komprimovan�ch dat
								//							(x) zkomprimovan� data
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
	long			Level;			// (4) hladina k p�ekreslov�n�
	double			Kroku;			// (8) po�et f�z� na jednotkovou vzd�lenost (0 = ihned)
	BYTE			Data[1];		// data - obr�zky sprajtu (po�ad�: f�ze, sm�r)
									//		ka�d� obr�zek v m�du komprimace:	(4) d�lka komprimovan�ch dat
									//											(x) zkomprimovan� data
} SPRITEPROG;

#define SIZEOFSPRITEPROG (5*sizeof(long) + 2*sizeof(WORD) + sizeof(double))


/////////////////////////////////////////////////////////////////////////////
// polo�ka zvuku v souboru

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
// buffer programu

class CPetProg
{
private:
	PETPROG*	m_Data;				// buffer dat
	int			m_Num;				// po�et polo�ek v bufferu
	int			m_Max;				// velikost bufferu

public:

// konstruktor a destruktor
	CPetProg();
	~CPetProg();

// poskytnut� po�tu polo�ek v bufferu
	inline int Num() const { return m_Num; };

// poskytnut� velikosti bufferu
	inline int Max() const { return m_Max; };

// zru�en� v�ech dat
	void DelAll();

// poskytnut� dat bufferu
	inline PETPROG* Data() const { return m_Data; };

// test platnosti polo�ky
	inline BOOL IsValid(const int index) const
		{ return ((DWORD)index < (DWORD)m_Num); }

	inline BOOL IsNotValid(const int index) const
		{ return ((DWORD)index >= (DWORD)m_Num); }

// poskytnut� p��stupu k polo�ce (bez kontroly platnosti polo�ky)
	inline PETPROG& operator[] (const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

// p�id�n� polo�ky na konec bufferu (vrac� index polo�ky)
// p�i chyb� pam�ti vrac� -1
	int _fastcall Add(const PETPROG* item);
};


/////////////////////////////////////////////////////////////////////////////
// buffer text� importu

class CBufChar
{
private:
	char*		m_Data;		// buffer text�
	int			m_Size;		// velikost dat v bufferu (bajt�)
	int			m_Num;		// po�et text� v bufferu
	int			m_Max;		// velikost bufferu (bajt�)

public:

// konstruktor a destruktor
	CBufChar();
	~CBufChar();

// poskytnut� po�tu polo�ek v bufferu
	inline int Num() const { return m_Num; };

// poskytnut� velikosti bufferu (ve znac�ch)
	inline int Max() const { return m_Max; };

// zru�en� v�ech dat
	void DelAll();

// poskytnut� velikosti dat v bufferu (v bajtech)
	inline int Size() const { return m_Size; };

// poskytnut� dat bufferu
	inline char* Data() const { return m_Data; };

// p�id�n� polo�ky na konec bufferu (vrac� index polo�ky)
// p�i chyb� pam�ti vrac� -1
	int _fastcall Add(const char* text, int len);
	int _fastcall Add(const char* text);
	int _fastcall Add(const CText& text);
};

/////////////////////////////////////////////////////////////////////////////
// buffer text�

class CBufChar2
{
private:
	char*		m_Data;		// buffer text�
	int			m_Size;		// velikost dat v bufferu (bajt�)
	int			m_Num;		// po�et text� v bufferu
	int			m_NumVal;	// po�et platn�ch (nepr�zdn�ch) text�
	int			m_Max;		// velikost bufferu (bajt�)

public:

// konstruktor a destruktor
	CBufChar2();
	~CBufChar2();

// poskytnut� po�tu polo�ek v bufferu
	inline int Num() const { return m_Num; };

// poskytnut� po�tu platn�ch (nepr�zdn�ch) text�
	inline int NumVal() const { return m_NumVal; };

// poskytnut� velikosti bufferu (ve znac�ch)
	inline int Max() const { return m_Max; };

// zru�en� v�ech dat
	void DelAll();

// poskytnut� velikosti dat v bufferu (v bajtech)
	inline int Size() const { return m_Size; };

// poskytnut� dat bufferu
	inline char* Data() const { return m_Data; };

// p�id�n� polo�ky na konec bufferu (vrac� index polo�ky)
// p�i chyb� pam�ti vrac� -1
	int _fastcall Add(const char* text, int len);
	int _fastcall Add(const char* text);
	int _fastcall Add(const CText& text);
};

namespace ProgFile
{
// prom�nn�
	extern const PETHEAD SaveHead;		// standardn� z�hlav�

// mapov�n� intern�ch funkc� na okno struktur a t��d
	extern	int*	ImportBlok;		// tabulka blok� pro import funkc�
	extern	int*	ImportIndex;		// tabulka index� pro import funkc�

// inicializace konverzn� tabulky importu funkc� (vrac� FALSE=chyba pam�ti)
	BOOL InitImportTab();

// ulo�en� nespustiteln� (miniverze)
	void SaveMini();

// ulo�en� spustiteln� (maxiverze)
	void SaveMaxi();

// ulo�en� do jin�ho jm�na
	void SaveAs();

// ulo�en� distribu�n� verze
	void SaveRel();

// na�ten� programu
	void Load();
}

// kontrola programu s opravou (vrac� FALSE=chyba pam�ti)
BOOL ProgramCheck();
