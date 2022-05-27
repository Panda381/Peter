
/***************************************************************************\
*																			*
*							Buffer reálných èísel							*
*																			*
\***************************************************************************/


class CBufReal
{

// ------------------------- interní promìnné a funkce ----------------------

private:

// promìnné
	double*		m_Data;		// ukazatel na data
	int			m_Num;		// poèet platných položek v bufferu
	int			m_Max;		// velikost bufferu (položek)

// vytvoøení nové položky
	inline int NewItem()
	{
		int i = m_Num;
		if (i >= m_Max)				// není další položka?
		{
			NewData();				// vytvoøení nových dat
		}
		m_Num = i + 1;
		return i;
	};

// vytvoøení nových dat (oddìleno kvùli lepší optimalizaci)
	void NewData();


// ---------------------------- veøejné funkce ------------------------------

public:

// konstruktor a destruktor
	CBufReal();
	~CBufReal();

// statický konstruktor a destruktor
	void Init();			// statický konstruktor
	void Term();			// statický destruktor

// zrušení všech položek v bufferu
	void DelAll();

// zjištìní poètu platných položek v bufferu
	inline int Num() const { return m_Num; };

// nastavení poètu položek v bufferu (nové položky neinicializované)
	void _fastcall Num(int num);

// zjištìní velikosti bufferu (vèetnì zrušených položek)
	inline int Max() const { return m_Max; };

// data bufferu
	inline double* Data() const { return m_Data; };

// kontrola platnosti položky
	inline BOOL IsValid(const int index) const
		{ return ((DWORD)index < (DWORD)m_Num); };

	inline BOOL IsNotValid(const int index) const
		{ return ((DWORD)index >= (DWORD)m_Num); };

// poskytnutí pøístupu k položce (bez kontroly indexu)
	inline double& operator[] (const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

// poskytnutí pøístupu k položce (bez kontroly indexu)
	inline const double& operator[] (const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

// poskytnutí položky (s kontrolou platnosti indexu)
	double _fastcall Get(const int index) const;

// nastavení položky (s kontrolou platnosti indexu)
	void _fastcall Set(const int index, const double data);

// vložení položky do bufferu na požadovanou pozici (omezí index)
	void _fastcall Insert(int index, const double data);

// zrušení položek z konce bufferu
	void _fastcall Del(int num);

// zrušení položky z bufferu (ostatní položky se pøisunou)
	void _fastcall DelItem(const int index);

// vytvoøení prázdné položky (vrací index položky)
	int New();

// pøidání položky (vrací index položky)
	int _fastcall Add(const double data);

// duplikace položky (s kontrolou platnosti indexu, vrací index první položky)
	int _fastcall Dup(const int index);
	int _fastcall Dup(const int index, int num);

// naètení èísla ze souboru (naèítá do promìnné, true=operace OK)
//	bool LoadFile(double& data, CString jmeno);

// uložení èísla do souboru formátu NUM (false=chyba)
//	bool SaveFile(double data, CString jmeno) const;

// operátor pøiøazení
	const CBufReal& operator= (const CBufReal& src);
};
