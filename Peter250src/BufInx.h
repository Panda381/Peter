
/***************************************************************************\
*																			*
*							Buffer pøemapování indexù						*
*																			*
\***************************************************************************/

class CBufIndex
{

// ------------------------- interní promìnné a funkce ----------------------

private:

// promìnné
	int*	m_Data;			// ukazatel na data
	int		m_Num;			// poèet položek v bufferu
	int		m_Max;			// velikost bufferu (položek)

// ---------------------------- veøejné funkce ------------------------------

public:

// konstruktor a destruktor
	CBufIndex();
	~CBufIndex();

// statický konstruktor a destruktor
	void Init();
	void Term();

// zrušení všech položek v bufferu
	void DelAll();

// poskytnutí adresy dat
	inline int* Data() const { return m_Data; };

// poskytnutí/nastavení poètu položek v bufferu
// (nové položky neinicializované, vrací FALSE=chyba pamìti)
	inline int Num() const { return m_Num; };
	bool _fastcall Num(const int num);

// nastavení poètu položek s vymazáním "-1", vrací FALSE=chyba pamìti
	bool _fastcall NumClear(const int num);

// vymazání všech položek v bufferu zadanou hodnotou
	void _fastcall Clear(const int clear);

// kontrola platnosti položky (kontroluje se pouze rozsah indexu)
	inline BOOL IsValid(const int index) const
		{ return ((DWORD)index < (DWORD)m_Num); };

	inline BOOL IsNotValid(const int index) const
		{ return ((DWORD)index >= (DWORD)m_Num); };

// poskytnutí pøístupu k položce (bez kontroly indexu)
	inline int& operator[] (const int index) 
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const int& operator[] (const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline int& At(const int index) 
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const int& At(const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

// poskytnutí položky (s kontrolou platnosti indexu, pro neplatnou vrátí -1)
	int _fastcall Get(const int index) const;

// nastavení položky (s kontrolou platnosti indexu)
	void _fastcall Set(const int index, const int data);

// pøidání položky na konec bufferu (vrací index položky, pøi chybì pamìti vrací <0)
	int _fastcall Add(const int data);

// operátor pøiøazení (pøi chybì pamìti poèet položek nesouhlasí)
	const CBufIndex& _fastcall operator= (const CBufIndex& srcbuf);
};
