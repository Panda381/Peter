
/***************************************************************************\
*																			*
*								Buffer seznamù								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// struktura prvku seznamu (16 Bajtù)

typedef struct LISTDATA_
{
	long	Index;					// (4) aktuální index seznamu
	long	Size;					// (4) velikost seznamu (položek)
	long	Auto;					// (4) automatická inkrementace indexu
	long	Res;					// (4) ... rezerva
} LISTDATA;


class CBufList
{

// ------------------------- interní promìnné a funkce ----------------------

private:

// promìnné
	LISTDATA*	m_Data;		// ukazatel na data
	int			m_Num;		// poèet platných položek v bufferu
	int			m_Max;		// velikost bufferu (položek)

// vytvoøení nové položky
	inline int NewItem()
	{
		int i = m_Num;				// poèet položek
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
	CBufList();
	~CBufList();

// statický konstruktor a destruktor
	void Init();			// statický konstruktor
	void Term();			// statický destruktor

// zrušení všech položek v bufferu
	void DelAll();

// poskytnutí bufferu dat
	inline LISTDATA* Data() const { return m_Data; };

// poskytnutí poètu platných položek v bufferu
	inline int Num() const { return m_Num; };

// poskytnutí velikosti bufferu
	inline int Max() const { return m_Max; };

// kontrola platnosti položky
	inline BOOL IsValid(const int index) const
		{ return ((DWORD)index < (DWORD)m_Num); };

	inline BOOL IsNotValid(const int index) const
		{ return ((DWORD)index >= (DWORD)m_Num); };

// poskytnutí pøístupu k položce (bez kontroly indexu)
	inline LISTDATA& operator[] (const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const LISTDATA& operator[] (const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline LISTDATA& At(const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const LISTDATA& At(const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

// zjištìní aktuálního indexu seznamu
	inline const int _fastcall Inx(int index)
	{
		return m_Data[index].Index;
	}

// automatická inkrementace indexu
	inline void _fastcall AutoInc(int index)
	{
	// adresa položky seznamu
		LISTDATA* list = m_Data + index;

	// test, zda bude inkrementace
		if (list->Auto != 0)
		{

	// nový index
			int newindex = list->Index + list->Auto;

	// kontrola pøeteèení indexu
			int size = list->Size;

			if ((DWORD)newindex >= (DWORD)size)
			{
				while (newindex < 0) newindex += size;
				while (newindex >= size) newindex -= size;
			}

	// nastavení nového indexu
			list->Index = newindex;
		}
	}

// automatická inkrementace indexu, vrací pùvodní index
	inline int _fastcall AutoIncInx(int index)
	{
	// adresa položky seznamu
		LISTDATA* list = m_Data + index;

	// úschova pùvodního indexu
		int result = list->Index;

	// test, zda bude inkrementace
		if (list->Auto != 0)
		{

	// nový index
			int newindex = result + list->Auto;

	// kontrola pøeteèení indexu
			int size = list->Size;

			if ((DWORD)newindex >= (DWORD)size)
			{
				while (newindex < 0) newindex += size;
				while (newindex >= size) newindex -= size;
			}

	// nastavení nového indexu
			list->Index = newindex;
		}

	// návrat pùvodního indexu
		return result;
	}

// poskytnutí položky (s kontrolou platnosti indexu)
	const LISTDATA& _fastcall Get(const int index) const;

// nastavení položky (s kontrolou platnosti indexu)
	void _fastcall Set(const int index, const LISTDATA& data);

// zrušení položek z konce bufferu
	void _fastcall Del(int num);

// vytvoøení prázdné položky (vrací index položky)
	int New();

// pøidání položky (vrací index položky)
	int _fastcall Add(const LISTDATA& data);

// duplikace položky (s kontrolou platnosti indexu, vrací index první položky)
	int _fastcall Dup(const int index);
	int _fastcall Dup(const int index, int num);

// operátor pøiøazení
	const CBufList& operator= (const CBufList& src);
};

