
/***************************************************************************\
*																			*
*								Prvky oken									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// typy prvkù oken

enum WINTYP {
	WINITEM_WINDOW,					// okno
	WINITEM_WINDOWRO,				// okno bez uzavøení
	WINITEM_BUTTON,					// tlaèítko
	WINITEM_DEFBUTTON,				// implicitní tlaèítko
	WINITEM_BUTTONICON,				// ikonový spínaè
	WINITEM_BUTTONCHECK,			// zaškrtávací tlaèítko
	WINITEM_BUTTONCHECK2,			// tlaèítkový pøepínaè
	WINITEM_BUTTON3,				// 3-stavový pøepínaè
	WINITEM_BUTTONRADIO,			// pøepínací tlaèítko
	WINITEM_BUTTONRADIO2,			// pøepínací tlaèítko = zaèátek skupiny
	WINITEM_EDIT,					// textový øádek
	WINITEM_MEMO,					// textové pole
	WINITEM_GROUP,					// skupina
	WINITEM_LISTBOX,				// seznam
	WINITEM_LISTEDIT,				// seznam s editací
	WINITEM_COMBOBOX,				// rozbalovací seznam
	WINITEM_COMBOEDIT,				// rozbalovací seznam s editací
	WINITEM_WINTEXT,				// text
	WINITEM_WINICON,				// ikona
	WINITEM_WINPIC,					// obrázek
	WINITEM_RICHEDIT,				// textové pole Richedit
	WINITEM_RICHMEMO,				// textové pole bez zalamování
	WINITEM_PROGRESS,				// indikátor prùbìhu
	WINITEM_HTRACKBAR,				// horizontální táhlo
	WINITEM_VTRACKBAR,				// vertikální táhlo
	WINITEM_TABLE,					// tabulka
	WINITEM_HSCROLLBAR,				// horizontální posuvník
	WINITEM_VSCROLLBAR,				// vertikální posuvník
	WINITEM_TABS,					// záložky
};


/////////////////////////////////////////////////////////////////////////////
// struktura položky prvku okna

typedef struct WINITEM_
{
	HWND	Wnd;					// handle prvku
	int		Parent;					// index rodièe prvku (0=hlavní okno, -1=nic)
	int		Typ;					// typ prvku
	int		Prev;					// pøedcházející prvek okna (vèetnì samotného okna)
	int		Next;					// následující prvek okna (vèetnì samotného okna)
	CString	Text;					// text prvku
	CIcon	Icon;					// ikona
	CPicture Picture;				// obrázek
	BYTE*	PictureData;			// cachovaná data obrázku se zarovnáním na 4 bajty (NULL=není)
	int		Left;					// pozice prvku zleva (v bodech)
	int		Top;					// pozice prvku shora (v bodech)
	int		Width;					// šíøka prvku (v bodech)
	int		Height;					// výška prvku (v bodech)
	int		ClientWidth;			// klientská šíøka prvku (v bodech)
	int		ClientHeight;			// klientská výška prvku (v bodech)
	bool	HasCaption;				// má titulek
	bool	HasBorder;				// má okraj
	bool	HasSize;				// okno s mìnitelnou velikostí (nebo další varianta okraje dialog. prvku)
	bool	AlwaysTop;				// vždy nahoøe
	bool	MayClose;				// možné uzavøení okna
	bool	TabStop;				// zarážka pro TAB
	bool	Show;					// pøíznak zobrazení prvku
	bool	Disable;				// pøíznak zákazu prvku
	bool	Click;					// kliknuto na tlaèítko
	bool	TextValid;				// text je platný
	bool	Check;					// pøepínaè zapnut
	bool	Check3;					// nedefinovaný stav pøepínaèe
	bool	Bold;					// zvýraznìný text
	bool	Italic;					// kurzíva
	bool	Underline;				// podtržení
	bool	Strikeout;				// pøeškrtnutí
	bool	Serif;					// patièky
	bool	Fixed;					// text s pevnou rozteèí
	int		FontHeight;				// bodová výška znakù (0=bìžná)
	double	FontWidth;				// relativní šíøka znakù (0=bìžná, 1=základní)
	double	Angle;					// úhel sklonu (0=bìžný)
	int		FontCol;				// barva textu
	int		FontBack;				// barva pozadí
	HBRUSH	FontBrush;				// štìtec pozadí
	HFONT	Font;					// aktuální font
  // tabulka
	bool	TabHead;				// tabulka má záhlaví
	int		TabCol;					// aktivní sloupec tabulky
	int		TabRow;					// aktivní øádek tabulky
	int		TabRowF;				// aktivní øádek tabulky se zamìøením
	int		TabCols;				// poèet sloupcù tabulky
	int		TabRows;				// poèet øádkù tabulky
	CString* TabData;				// data tabulky (po øádcích)
	BYTE*	TabAlign;				// zarovnání sloupcù tabulky
	int		TabDataMax;				// velikost bufferu seznamu dat tabulky (poèet položek)
  // posuvník
	int		ScrollMax;				// maximální hodnota pozice posuvníku (0 až SCROLLRANGE-PAGE)
	double	ScrollPage;				// délka stránky (0.0001 až 1)
	double	ScrollPos;				// aktuální pozice (0 až 1)
} WINITEM;

#define SCROLLRANGE	10000			// rozsah posuvníku

/////////////////////////////////////////////////////////////////////////////
// prvky oken (index 0 = hlavní okno)

class CBufWin
{

// ------------------------- interní promìnné a funkce ----------------------

private:

// promìnné
	WINITEM*		m_Data;			// ukazatel na data
	bool*			m_Valid;		// pøíznaky platnosti položek
	int				m_Num;			// poèet platných položek v bufferu
	int				m_Max;			// velikost bufferu (položek)
	int				m_Next;			// pøíští volná položka (-1 = není)

// vytvoøení nové položky
	inline int NewItem()
	{
		if (m_Next < 0)				// není další položka?
		{
			NewData();				// vytvoøení nových dat
		}

		int i = m_Next;				// pøíští volna položka
		m_Next = *(int*)(m_Data + i); // další položka
		m_Valid[i] = true;			// nastavení pøíznaku platnosti položky
		WINITEM* item = m_Data + i;
		MemFill(item, sizeof(WINITEM), 0);
		item->Text.Init();			// inicializace textu
		item->Icon.Init();
		item->Picture.Init();
		item->PictureData = NULL;
		item->FontBrush = NULL;
		item->ScrollMax = (SCROLLRANGE*9 + 5)/10;
		item->ScrollPage = 0.1;
		item->Prev = i;
		item->Next = i;
		m_Num++;					// zvýšení èítaèe platných položek
		return i;
	};

// vytvoøení nových dat (oddìleno kvùli lepší optimalizaci)
	void NewData();

// ---------------------------- veøejné funkce ------------------------------

public:

// konstruktor a destruktor
	CBufWin();
	~CBufWin();

// statický konstruktor a destruktor
	void Init();			// statický konstruktor
	void Term();			// statický destruktor

// zrušení všech položek v bufferu (ukládání zaène opìt po øadì od zaèátku)
	void DelAll();

// poskytnutí bufferu dat
	inline WINITEM* Data() const { return m_Data; };

// poskytnutí pole platností
	inline bool* Valid() const { return m_Valid; };

// poskytnutí poètu platných položek v bufferu
	inline int Num() const { return m_Num; };

// poskytnutí velikosti bufferu (vèetnì zrušených položek)
	inline int Max() const { return m_Max; };

// kontrola platnosti položky
	inline BOOL IsValid(const int index) const
		{ return (((DWORD)index < (DWORD)m_Max) && m_Valid[index]); };

	inline BOOL IsNotValid(const int index) const
		{ return (((DWORD)index >= (DWORD)m_Max) || !m_Valid[index]); };

// poskytnutí pøístupu k položce (bez kontroly indexu)
	inline WINITEM& operator[] (const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const WINITEM& operator[] (const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline WINITEM& At(const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const WINITEM& At(const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

// zrušení položky (s kontrolou platnosti indexu)
	void _fastcall Del(const int index);

// vytvoøení prázdné položky (vrací index položky)
	int _fastcall New();
};
