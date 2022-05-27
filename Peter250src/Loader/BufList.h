
/***************************************************************************\
*																			*
*								Buffer seznam�								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// struktura prvku seznamu (16 Bajt�)

typedef struct LISTDATA_
{
	long	Index;					// (4) aktu�ln� index seznamu
	long	Size;					// (4) velikost seznamu (polo�ek)
	long	Auto;					// (4) automatick� inkrementace indexu
	long	Res;					// (4) ... rezerva
} LISTDATA;


class CBufList
{

// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn�
	LISTDATA*	m_Data;		// ukazatel na data
	int			m_Num;		// po�et platn�ch polo�ek v bufferu
	int			m_Max;		// velikost bufferu (polo�ek)

// vytvo�en� nov� polo�ky
	inline int NewItem()
	{
		int i = m_Num;				// po�et polo�ek
		if (i >= m_Max)				// nen� dal�� polo�ka?
		{
			NewData();				// vytvo�en� nov�ch dat
		}

		m_Num = i + 1;
		return i;
	};

// vytvo�en� nov�ch dat (odd�leno kv�li lep�� optimalizaci)
	void NewData();

// ---------------------------- ve�ejn� funkce ------------------------------

public:

// konstruktor a destruktor
	CBufList();
	~CBufList();

// statick� konstruktor a destruktor
	void Init();			// statick� konstruktor
	void Term();			// statick� destruktor

// zru�en� v�ech polo�ek v bufferu
	void DelAll();

// poskytnut� bufferu dat
	inline LISTDATA* Data() const { return m_Data; };

// poskytnut� po�tu platn�ch polo�ek v bufferu
	inline int Num() const { return m_Num; };

// poskytnut� velikosti bufferu
	inline int Max() const { return m_Max; };

// kontrola platnosti polo�ky
	inline BOOL IsValid(const int index) const
		{ return ((DWORD)index < (DWORD)m_Num); };

	inline BOOL IsNotValid(const int index) const
		{ return ((DWORD)index >= (DWORD)m_Num); };

// poskytnut� p��stupu k polo�ce (bez kontroly indexu)
	inline LISTDATA& operator[] (const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const LISTDATA& operator[] (const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline LISTDATA& At(const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const LISTDATA& At(const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

// zji�t�n� aktu�ln�ho indexu seznamu
	inline const int _fastcall Inx(int index)
	{
		return m_Data[index].Index;
	}

// automatick� inkrementace indexu
	inline void _fastcall AutoInc(int index)
	{
	// adresa polo�ky seznamu
		LISTDATA* list = m_Data + index;

	// test, zda bude inkrementace
		if (list->Auto != 0)
		{

	// nov� index
			int newindex = list->Index + list->Auto;

	// kontrola p�ete�en� indexu
			int size = list->Size;

			if ((DWORD)newindex >= (DWORD)size)
			{
				while (newindex < 0) newindex += size;
				while (newindex >= size) newindex -= size;
			}

	// nastaven� nov�ho indexu
			list->Index = newindex;
		}
	}

// automatick� inkrementace indexu, vrac� p�vodn� index
	inline int _fastcall AutoIncInx(int index)
	{
	// adresa polo�ky seznamu
		LISTDATA* list = m_Data + index;

	// �schova p�vodn�ho indexu
		int result = list->Index;

	// test, zda bude inkrementace
		if (list->Auto != 0)
		{

	// nov� index
			int newindex = result + list->Auto;

	// kontrola p�ete�en� indexu
			int size = list->Size;

			if ((DWORD)newindex >= (DWORD)size)
			{
				while (newindex < 0) newindex += size;
				while (newindex >= size) newindex -= size;
			}

	// nastaven� nov�ho indexu
			list->Index = newindex;
		}

	// n�vrat p�vodn�ho indexu
		return result;
	}

// poskytnut� polo�ky (s kontrolou platnosti indexu)
	const LISTDATA& _fastcall Get(const int index) const;

// nastaven� polo�ky (s kontrolou platnosti indexu)
	void _fastcall Set(const int index, const LISTDATA& data);

// zru�en� polo�ek z konce bufferu
	void _fastcall Del(int num);

// vytvo�en� pr�zdn� polo�ky (vrac� index polo�ky)
	int New();

// p�id�n� polo�ky (vrac� index polo�ky)
	int _fastcall Add(const LISTDATA& data);

// duplikace polo�ky (s kontrolou platnosti indexu, vrac� index prvn� polo�ky)
	int _fastcall Dup(const int index);
	int _fastcall Dup(const int index, int num);

// oper�tor p�i�azen�
	const CBufList& operator= (const CBufList& src);
};

