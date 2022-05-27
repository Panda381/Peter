
/***************************************************************************\
*																			*
*							Buffer programu									*
*																			*
\***************************************************************************/

#define		PROGBUFNUM	6				// poèet programových bufferù

// identifikace bufferù (nemìnit poøadí - muselo by se zmìnit pøi ukládání souboru!)
#define		BufIntID	0				// interní buffer seznamu funkcí (nemá strom)
#define		BufClsID	1				// buffer tøíd objektù
#define		BufObjID	2				// buffer globálních objektù
#define		BufLocID	3				// buffer lokálních objektù
#define		BufEdiID	4				// buffer editace
#define		BufStrID	5				// buffer struktur

// datové buffery (musí odpovídat indexùm v SaveHead !!!!)
#define		BufNumID	PROGBUFNUM		// (6) buffer èísel
#define		BufTxtID	PROGBUFNUM+1	// (7) buffer textù
#define		BufLogID	PROGBUFNUM+2	// (8) buffer logických promìnných
#define		BufIcoID	PROGBUFNUM+3	// (9) buffer ikon
#define		BufMapID	PROGBUFNUM+4	// (10) buffer ploch
#define		BufPicID	PROGBUFNUM+5	// (11) buffer obrázkù
#define		BufSprID	PROGBUFNUM+6	// (12) buffer sprajtù
#define		BufSndID	PROGBUFNUM+7	// (13) buffer zvukù
#define		BufMusID	PROGBUFNUM+8	// (14) buffer hudby
#define		BufPalID	PROGBUFNUM+9	// (15) buffer palet

// Pøi zmìnì zmìnit definice i v Loader !!!!!

#define	PR_LITE		0x0000		// prvek vypnut pro LITE verzi

/////////////////////////////////////////////////////////////////////////////
// definice stavù ikon

enum STATE {
	STATE_NONE = 0,
	STATE_DEF,
	STATE_SET,						// nastavení
	STATE_SET_OPT,					// nepovinné nastavení
};


/////////////////////////////////////////////////////////////////////////////
// struktura položky programu - 64 bajtù

typedef struct PROGITEM_
{
	int			Func;				// identifikaèní èíslo funkce (= IDF_...)

	int			SrcMask;			// maska zdrojových vlastností
	int			DstMask;			// maska cílových vlastností
	int			Param;				// parametry položky

	int			RefBlok;			// referenèní blok (-1 = není)
	int			RefIndex;			// referenèní index
	int			DatBlok;			// datový blok (-1 = není)
	int			DatIndex;			// datový index

	int			Parent;				// index rodièe (-1 = není)
	int			Next;				// index následující položky (-1 = není)
	int			Prev;				// index pøedcházející položky (-1 = není)
	int			Child;				// index prvniho potomka (-1 = není)

	int			Icon;				// index ikony (-1 = implicitní)
	int			Name;				// index jména (-1 = ímplicitní)

	HTREEITEM	HTree;				// handle položky stromu (NULL=není)

	int			Refer;				// èítaè referencí na položku
} PROGITEM;

#define	SIZEOFPROGITEM	(15*sizeof(int) + sizeof(HTREEITEM))	// velikost položky

// zdrojové a cílové vlastnosti:
#define	PR_COMMAND	0x0001		// je pøíkazem/pøijímá pøíkazy
#define	PR_EXPRES	0x0002		// je èíselným výrazem/pøijímá èíselný výraz
#define	PR_TEXT		0x0004		// je textem/pøijímá text
#define	PR_BOOL		0x0008		// je logickým výrazem/pøijímá logický výraz
#define	PR_ICON		0x0010		// je ikonou/pøijímá ikonu
#define	PR_MAP		0x0020		// je plochou/pøijímá plochu
#define	PR_PIC		0x0040		// je obrázkem/pøijímá obrázky
#define	PR_SPRITE	0x0080		// je sprajtem/pøijímá sprajty
#define	PR_SOUND	0x0100		// je zvukem/pøijímá zvuky
#define	PR_MUSIC	0x0200		// je hudbou/pøijímá hudbu

#define	PR_DIGIT	0x0400		// je èíslicí/pøijímá èíslice

#define PR_ALLDATA	(	PR_EXPRES |	\
						PR_TEXT |	\
						PR_BOOL |	\
						PR_ICON |	\
						PR_MAP |	\
						PR_PIC |	\
						PR_SPRITE |	\
						PR_SOUND |	\
						PR_MUSIC)	// všechny možnosti dat (kromì pøíkazu)

#define	PR_ALL		(PR_ALLDATA | PR_COMMAND) // všechny možnosti vlastností (funkce, skupina)

// parametry a vlastnosti
#define	PR_EXP		0x0001		// potomci prvku jsou rozvinuti
#define	PR_LOCK		0x0002		// prvek uzamknut
#define	PR_LOCK_DEP	0x0004		// prvek závisle uzamknut
#define	PR_OFF		0x0008		// prvek vypnut
#define	PR_OFF_DEP	0x0010		// prvek závisle vypnut
#define	PR_ONE		0x0020		// prvek mùže mít jen 1 potomka
#define	PR_INTERN	0x0040		// zákaz rušení a pøesunu
#define	PR_NOMOVE	0x0080		// zákaz rušení, pøesunu a kopie
#define PR_NEW		0x0100		// nová položka (bude pøepsána z cliboard::paste)
#define PR_SETPAR	0x0200		// je-li možný parametr, je nutnì požadován
#define PR_PARPAR	0x0400		// má parametr pøi použití jako pøíkaz i jako parametr
#define PR_CD		0x0800		// je alternativní knihovna
#define PR_ALTEXT	0x1000		// je alternativní pøípona souboru (JPG, MP3, RMI)
#define PR_CD2		0x2000		// je alternativní knihovna 2

#define PR_NOREFER	0x4000		// nedìlat reference na prvek

#define NOREFER		0x4000		// pøíznak v tabulce struktur

/////////////////////////////////////////////////////////////////////////////
// položka inicializaèní tabulky stromù

typedef struct INITTREE_ {
	short	Level;			// hladina (0 = ROOT, NOREFER=nedìlat reference)
	short	Func;			// identifikace funkce (IDF_...)
} INITTREE;


/////////////////////////////////////////////////////////////////////////////
// buffer programu

class CBufProg
{

// ------------------------- interní promìnné a funkce ----------------------

private:

// promìnné
	PROGITEM*	m_Data;			// buffer dat
	bool*		m_Valid;		// pøíznaky platnosti položek
	int			m_Num;			// poèet platných položek v bufferu
	int			m_Max;			// velikost bufferu (položek)
	int			m_Next;			// pøíští volná položka (-1 = není)
	bool		m_Undo;			// požadavek registrace zmìn pro UNDO

	int			m_Redraw;		// èítaè pro vypnutí pøekreslování okna (0=zapnuto)
	int			m_First;		// index první (ROOT) položky (-1 = není)
	HWND		m_Tree;			// pøipojený strom (NULL = není)
	int			m_Disp;			// rodiè zobrazení položek (-1 = vše, -2 = nic)
	int			m_IconWidth;	// šíøka ikony (0 = není)
	int			m_IconHeight;	// výška ikony (0 = není)
	int			m_BufID;		// identifikace tohoto bufferu
	int			m_Def;			// definovaná položka (-1 = není)
	CText		m_Nadpis;		// nadpis okna

	CFont		m_FontSet;		// uschované nastavení fontu
	DWORD		m_CharSet;		// znaková sada fontu
	HFONT		m_Font;			// font (NULL=není)

// vytvoøení nových dat (vrací TRUE=operace OK) (oddìleno kvùli lepší optimalizaci)
	bool NewData();

// vytvoøení nové položky (vrací index položky, pøi chybì vrací -1)
	int NewItem();

// zrušení položky - zaøazení do volných položek (nekontroluje index)
	void _fastcall DelItem(const int index);

// pøíprava indexu stavové ikony (upravit s INDEXTOSTATEIMAGEMASK() !)
	int GetStateImage(int index);

// nové zobrazení položky
	void DispNewItem(int index);

// zajištìní nových dat u kopírované položky (voláno též z Clipboard::Paste)
	void CopyNewData(PROGITEM* item);

// duplikace dat u kopírované položky
	void CopyDupData(PROGITEM* item);

// ---------------------------- veøejné funkce ------------------------------

public:

// konstruktor a destruktor
	CBufProg();
	~CBufProg();

// statický konstruktor a destruktor
	void Init();
	void Term();

// zrušení všech položek v bufferu (ukládání zaène opìt po øadì od zaèátku, nemìní obsah okna)
	void DelAll();

// poskytnutí bufferu dat
	inline PROGITEM* Data() const { return m_Data; };

// poskytnutí bufferu platnosti (hodnoty TRUE a FALSE)
	inline bool* Valid() const { return m_Valid; };

// poskytnutí poètu platných položek v bufferu
	inline int Num() const { return m_Num; };

// poskytnutí velikosti bufferu (vèetnì zrušených položek)
	inline int Max() const { return m_Max; };

// nastavení požadavku registrace zmìn pro UNDO
	inline void UndoOn() { m_Undo = true; };
	inline void UndoOff() { m_Undo = false; };
	inline void UndoSet(const bool undo) { m_Undo = undo; };

// kontrola platnosti položky
	inline BOOL IsValid(const int index) const
		{ return (((DWORD)index < (DWORD)m_Max) && m_Valid[index]); };

	inline BOOL IsNotValid(const int index) const
		{ return (((DWORD)index >= (DWORD)m_Max) || !m_Valid[index]); };

// poskytnutí pøístupu k položce (bez kontroly indexu)
	inline PROGITEM& operator[] (const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const PROGITEM& operator[] (const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline PROGITEM& At(const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const PROGITEM& At(const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

// vypnutí/zapnutí pøekreslování (musí být spárováno!)
	void RedrawOff();
	void RedrawOn();

// poskytnutí indexu první ROOT položky (-1 = není)
	inline int First() const { return m_First; };

// poskytnutí/nastavení handle stromu (neaktualizuje zobrazení, zneplatní ikony)
	inline HWND Tree() const { return m_Tree; };
	inline void Tree(const HWND tree)
	{ 
		if (tree != m_Tree)
		{
			m_Tree = tree; 
			m_IconWidth = 0; 
			m_IconHeight = 0; 
			m_FontSet.Height = -1;
			AktFont();
		}
	};

// aktualizace fontu v oknì
	void AktFont();

// poskytnutí/nastavení rodièe zobrazení položek (aktualizuje zobrazení, -1 = vše, -2 = nic)
	inline int Disp() const { return m_Disp; };
	void Disp(int disp);

// poskytnutí/nastavení rozmìrù ikon (zoom: TRUE = ikony 32*32)
	inline int IconWidth() const { return m_IconWidth; };
	inline void IconWidth(int width) { m_IconWidth = width; };
	inline int IconHeight() const { return m_IconHeight; };
	inline void IconHeight(int height) { m_IconHeight = height; };
	void IconList(SMALLICON zoom);

// poskytnutí/nastavení ID bufferu
	inline int BufID() const { return m_BufID; };
	inline void BufID(int id) { m_BufID = id; };

// poskytnutí/nastavení definované položky (-1 = odznaèení)
	inline int Def() const { return m_Def; };
	void Def(int index);

// poskytnutí/nastavení nadpisu okna
	const CText& Nadpis() const { return m_Nadpis; };
	void Nadpis(const CText& text);

// poskytnutí/nastavení první zobrazené položky (-1 = není)
	int Top();
	void Top(int index);

// poskytnutí/nastavení vybrané položky (-1 = není)
	int Select();
	void Select(int index);

// aktualizace zobrazení stavové ikony položky pro okno editoru programu
	void _fastcall AktStateImage(int index);

// index následující položky ve stromu (cyklicky, -1 = navrátí se první položka)
	int _fastcall NextItem(int index);

// index pøedcházející položky ve stromu (cyklicky, -1 = navrátí se poslední položka)
	int _fastcall PrevItem(int index);

// navrácení položky v UNDO operaci (vrací TRUE=operace OK)
// provádí záznam do UNDO bufferu
	bool UndoIns(const PROGITEM* item, const int index);

// vložení položky bez navázání dat (pro UNDO buffer)
// vrací index položky, pøi chybì pamìti vrací -1
	int Insert0(const PROGITEM* item);

// zrušení položky bez navázání dat (pro UNDO buffer)
	void Del0(int index);

// vložení položky (vrací index položky) - parametry nemusí být platné
//  parent = rodiè (-1=ROOT), after=pøedcházející položka 
//		(-1=první, -2=poslední, -3=abecednì, -4=abecednì s pøedností skupin)
//  Položky nastavené funkcí: Parent, Next, Prev, Child, HTree, Refer,
	int Insert(const PROGITEM* item, int parent, int after);

// naètení inicializaèní tabulky bufferu (starý obsah bufferu se neruší - lze pøidávat)
// pøi chybì pamìti vrací FALSE
	bool InitTab(const INITTREE* tab, const int num);

// vytvoøení prázdné položky (vrací index položky, -1=chyba pamìti)
	int New();

// nalezení položky podle handle (-1 = není)
	int Find(HTREEITEM htree);

// nalezení položky s ukazatelem na data (-1 = není)
	int SrcDat(int bufID, int index);

// nalezení referující položky od aktuální položky cyklicky (-1 = nenalezeno)
	int SrcRef(int RefBlok, int RefIndex, BOOL next);

// nalezení indexu textu k položce (-1 = neplatný)
	int _fastcall GetText(const int index);

// nalezení indexu ikony k položce (-1 = neplatná)
	int _fastcall GetIcon(const int index);

// poskytnutí poètu referencí pro uživatele
	int GetRefer(int index);

// test povolení zrušení vìtve z hlediska referencí na položky (TRUE=OK)
	bool TestRefer(int index);

// pøekreslení ikony ve stromu
	void ReDispIcon(int icon);

// test èísla funkce, zda je to objekt (s jedineèným jménem)
	bool _fastcall TestObj(int func);

// nalezení objektu daného jména (-1 = nenalezeno) - rozlišuje velká/malá písmena
//		name ..... hledaný text
//		over ..... pøeskoèená položka (-1 = není)
//		first .... první položka (-1 = od zaèátku)
//		next ..... pøíští položka pro zastavení (-1 = po konec)
	int FindObj(const CMultiText& name, int over = -1, int first = -1, int next = -1);

// zrušení vìtve stromu (pøi chybì pamìti vrací FALSE - stav není navrácen!)
	bool Del(int index);

// pøesun vìtve stromu (vrací nový index položky)
// (parent: -1 = ROOT, after: -1 = zaèátek, -2 = konec)
	int Move(int parent, int after, int src);

// kopie vìtve stromu - vrací cílový index položky (pøi chybì vrací <0)
// (parent: -1 = ROOT, pos: -1 = zaèátek, -2 = konec)
	int Copy(int parent, int pos, int bufID, int src);
};
