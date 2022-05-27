
/***************************************************************************\
*																			*
*								Šablona bufferù								*
*																			*
\***************************************************************************/

// Minimální velikost objektu musí být 4 Bajty!!!!!

template <class OBJECT> class CBuffer
{

// ------------------------- interní promìnné a funkce ----------------------

protected:

// promìnné
	OBJECT*		m_Data;			// buffer dat
	bool*		m_Valid;		// buffer platnosti položek
	int			m_Num;			// poèet platných položek v bufferu
	int			m_Max;			// velikost bufferu (položek)
	int			m_Next;			// pøíští volná položka (-1=není)
	bool		m_Undo;			// požadavek registrace zmìn pro UNDO
	OBJECT		m_EmptyItem;	// prázdný objekt

// vytvoøení nových dat (vrací TRUE=operace OK)
	bool NewData();

// vytvoøení nové položky (vrací index položky, pøi chybì vrací < 0)
	int NewItem();

// zrušení položky - zaøazení do volných položek (nekontroluje index)
	void _fastcall DelItem(const int index);
		
// pøidání záznamu o vložení položky do UNDO bufferu
	bool UndoAddIns(int index);

// ---------------------------- veøejné funkce ------------------------------

public:

// konstruktor a destruktor
	CBuffer();
	~CBuffer();

// statický konstruktor a destruktor
	void Init();
	void Term();

// zrušení všech položek v bufferu (ukládání zaène opìt po øadì od zaèátku)
// provádí záznam do UNDO bufferu, pøi chybì pamìti vynuluje UNDO
	void DelAll();

// poskytnutí prázdného objektu
	inline OBJECT& EmptyItem() { return m_EmptyItem; };
	inline const OBJECT& EmptyItem() const { return m_EmptyItem; };

// poskytnutí bufferu dat
	inline OBJECT* Data() const { return m_Data; };

// poskytnutí bufferu platnosti položek
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
	inline OBJECT& operator[] (const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; };

	inline const OBJECT& operator[] (const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; };

	inline OBJECT& At(const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; };

	inline const OBJECT& At(const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; };

// poskytnutí položky (s kontrolou platnosti indexu - pro neplatnou vrátí prázdnou položku)
	const OBJECT& _fastcall Get(const int index) const;

// nastavení položky (s kontrolou platnosti indexu)
	void _fastcall Set(const int index, const OBJECT& data);

// zrušení položky (s kontrolou platnosti indexu) (vrací FALSE=chyba pamìti)
// provádí záznam do UNDO bufferu
	bool _fastcall Del(const int index);

// navrácení položky v UNDO operaci (vrací TRUE=operace OK)
// provádí záznam do UNDO bufferu
	bool UndoIns(const OBJECT& data, const int index);

// operátor pøiøazení (neobsluhuje chybu pamìti!)
	const CBuffer& _fastcall operator= (const CBuffer& src);

// vytvoøení nové prázdné položky (vrací index položky, <0 = chyba)
// provádí záznam do UNDO bufferu
	int New();

// pøidání položky (vrací index položky, <0 = chyba)
// provádí záznam do UNDO bufferu
	int _fastcall Add(const OBJECT& data);

// duplikace položky (neplatná položka se duplikuje na prázdnou, vrací index položky, <0 = chyba pamìti)
// provádí záznam do UNDO bufferu
	int _fastcall Dup(const int index);
};


////////////////////////////////////////////////////////////////////
// vytvoøení nových dat (vrací TRUE=operace OK)

#define NEWDATANUM 256				// poèet novì vytvoøených položek (typicky 1 KB)

template <class OBJECT> bool CBuffer<OBJECT>::NewData()
{
// nový poèet položek
	int next = m_Max;				// pøíští položka - 1
	int max = next + NEWDATANUM;	// nový poèet položek

// zvìtšení bufferu dat
	OBJECT* newdata = (OBJECT*)MemSize(m_Data, max*sizeof(OBJECT));
	if (newdata == NULL) return false;
	m_Data = newdata;

// zvìtšení bufferu platnosti
	bool* newvalid = (bool*)MemSize(m_Valid, max*sizeof(bool));
	if (newvalid == NULL) return false;
	m_Valid = newvalid;

// nový maximální poèet položek v bufferu
	m_Max = max;

// vymazání pøíznakù platnosti položek (nastavení na pøíznak neplatnosti)
	int i;
	newvalid += next;
	for (i = NEWDATANUM; i > 0; i--) { *newvalid = false; newvalid++; }

// zaèlenìní do øetìzce volných položek
	newdata += next - 1;			// ukazatel dat - 1
	for (i = NEWDATANUM; i > 0; i--)
	{
		newdata++;					// zvýšení ukazatele položek
		next++;						// zvýšení indexu pøíští položky
		*(int*)newdata = next;		// odkaz na pøíští položku
	}
	*(int*)newdata = m_Next;		// navázání na další položku
	m_Next = m_Max-NEWDATANUM;		// odkaz na první novou položku

// pøíznak operace OK
	return true;
};

////////////////////////////////////////////////////////////////////
// vytvoøení nové položky (vrací index položky, pøi chybì vrací < 0)

template <class OBJECT> int CBuffer<OBJECT>::NewItem()
{
// vytvoøení nových dat, není-li volná další položka
	if (m_Next < 0)				// není další položka?
	{
		if (!NewData()) return -1;	// vytvoøení nových dat
	}

// vyjmutí položky z øetìzce volných položek
	int i = m_Next;				// pøíští volná položka
	m_Next = *(int*)(m_Data + i); // další položka
	m_Valid[i] = true;			// nastavení pøíznaku platnosti položky
	m_Num++;					// zvýšení èítaèe platných položek

// novì vytvoøená položka
	return i;
};

////////////////////////////////////////////////////////////////////
// zrušení položky - zaøazení do volných položek (nekontroluje index a neruší objekt)

template <class OBJECT> void _fastcall CBuffer<OBJECT>::DelItem(const int index)
{
	*(int*)(m_Data + index) = m_Next;	// pøíští volná položka
	m_Valid[index] = false;				// zrušení pøíznaku platnosti
	m_Num--;							// snížení èítaèe platných položek
	m_Next = index;						// odkaz na tuto položku
}

////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

template <class OBJECT> CBuffer<OBJECT>::CBuffer()
{
	m_Data = NULL;						// není buffer dat
	m_Valid = NULL;						// není buffer platnosti
	m_Num = 0;							// není žádná platná položka
	m_Max = 0;							// není buffer položek
	m_Next = -1;						// pøiští volná položka (-1=není)
	m_Undo = false;						// neregistrovat zmìny pro UNDO
	m_EmptyItem.Empty();
}

template <class OBJECT> CBuffer<OBJECT>::~CBuffer()
{
	DelAll();							// zrušení všech položek
}

/////////////////////////////////////////////////////////////////////////////
// statický konstruktor a destruktor

template <class OBJECT> void CBuffer<OBJECT>::Init()
{
	m_Data = NULL;						// není buffer dat
	m_Valid = NULL;						// není buffer platnosti
	m_Num = 0;							// není žádná platná položka
	m_Max = 0;							// není buffer položek
	m_Next = -1;						// pøiští volná položka (-1=není)
	m_Undo = false;						// neregistrovat zmìny pro UNDO
	m_EmptyItem.Init();
}

template <class OBJECT> void CBuffer<OBJECT>::Term()
{
	DelAll();							// zrušení všech položek
	m_EmptyItem.Term();
}

////////////////////////////////////////////////////////////////////
// zrušení všech položek v bufferu (ukládání zaène opìt po øadì od zaèátku)
// provádí záznam do UNDO bufferu, pøi chybì pamìti vynuluje UNDO

template <class OBJECT> void CBuffer<OBJECT>::DelAll()
{
// zrušení všech položek
	if (m_Num > 0)
	{
		for (int i = m_Max-1; i >= 0; i--)
		{
			if (!Del(i))
			{
				if (m_Undo) Undo.DelAll();
			}
		}
	}

// zrušení bufferù
	MemFree(m_Data);			// zrušení bufferu dat
	m_Data = NULL;				// není buffer dat
	MemFree(m_Valid);			// zrušení bufferu platnosti
	m_Valid = NULL;				// není bufferu platnosti
	m_Num = 0;					// není žádná platná položka
	m_Max = 0;					// není žádná položka v bufferu
	m_Next = -1;				// není pøíští položka
}

////////////////////////////////////////////////////////////////////
// poskytnutí položky (s kontrolou platnosti indexu - pro neplatnou vrátí prázdnou položku)

template <class OBJECT> const OBJECT& _fastcall CBuffer<OBJECT>::Get(const int index) const
{
	if (IsValid(index))			// je index platný?
	{
		return m_Data[index];	// položka na daném indexu
	}
	return m_EmptyItem;			// pro neplatný index vrátí prázdný objekt
}

////////////////////////////////////////////////////////////////////
// nastavení položky (s kontrolou platnosti indexu)

template <class OBJECT> void _fastcall CBuffer<OBJECT>::Set(const int index, const OBJECT& data)
{
	if (IsValid(index))					// je index platný?
	{
		m_Data[index] = data;
	}
}

////////////////////////////////////////////////////////////////////
// zrušení položky (s kontrolou platnosti indexu, vrací FALSE=chyba pamìti)
// provádí záznam do UNDO bufferu

template <class OBJECT> bool _fastcall CBuffer<OBJECT>::Del(const int index)
{
// kontrola platnosti položky
	if (IsValid(index))						// je index platný?
	{

// záznam o zrušení do UNDO bufferu
		if (m_Undo)
		{
			if (!Undo.AddDel(index, m_Data[index]))
			{
				return false;
			}
		}

// zrušení položky
		m_Data[index].Term();				// ukonèení objektu
		DelItem(index);						// zrušení položky
	}
	return true;
}

////////////////////////////////////////////////////////////////////
// navrácení položky v UNDO operaci (vrací TRUE=operace OK)
// provádí záznam do UNDO bufferu

template <class OBJECT> bool CBuffer<OBJECT>::UndoIns(const OBJECT& data, const int index)
{
// vytvoøení nových dat (zajištìní existence položky)
	ASSERT(index >= 0);
	while (index >= m_Max)
	{
		if (!NewData()) return false;
	}

// adresa vkládané položky
	ASSERT(IsNotValid(index));					// položka musí být neplatná
	OBJECT* item = m_Data + index;				// adresa nové položky

// záznam operace pro UNDO
	if (m_Undo)
	{
		if (!UndoAddIns(index)) return false;
	}

// vyjmutí položky z volných položek
	int i = m_Next;								// ukazatel øetìzce volných položek

// navrácena hned první položka z øetìzce volných položek
	if (i == index)								// je to hned první položka?
	{
		m_Next = *(int*)item;					// odkaz na další položku
	}
	else
	{

// nalezení položky v øetìzci volných položek
		while (*(int*)(m_Data + i) != index)
		{
			i = *(int*)(m_Data + i);

// tento pøípad nesmí nikdy nastat - položka nebyla nalezena!!!
			ASSERT(i >= 0);
			if (i < 0)								// to je chyba!!!!!!!
			{
				Undo.DelAll();
				return false;
			}
		}

// pøeskoèení odkazu na další položku
		*(int*)(m_Data + i) = *(int*)item;
	}

// nastavení pøíznaku platnosti položky
	m_Num++;									// zvýšení èítaèe položek
	m_Valid[index] = true;						// pøíznak platnosti položky

// inicializace položky
	item->Init(data);

// pøíznak operace OK
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// operátor pøiøazení (neobsluhuje chybu pamìti!)

template <class OBJECT> const CBuffer<OBJECT>& _fastcall CBuffer<OBJECT>::operator= (const CBuffer<OBJECT>& src)
{
// zrušení všech starých dat
	DelAll();					// zrušení starých dat

// cyklus pøes všechny položky
	int index = 0;				// index naèítané položky
	for (int i = src.Max(); i > 0; i--) // pro všechny položky v bufferu
	{

// pøidání položky do bufferu
		if (src.m_Valid[index])	// je to platná položka?
		{
			Add(src[index]);	// kopie položky
		}
		index++;				// inkrementace ètecího indexu
	}

// souhlasí poèet položek?
	ASSERT(m_Num == src.Num());
	return *this;
}

////////////////////////////////////////////////////////////////////
// vytvoøení nové prázdné položky (vrací index položky, <0 = chyba)
// provádí záznam do UNDO bufferu

template <class OBJECT> int CBuffer<OBJECT>::New()
{
// vytvoøení nové položky
	int result = NewItem();				// vytvoøení nové položky
	if (result >= 0)
	{

// záznam UNDO
		if (m_Undo)
		{
			if (!UndoAddIns(result))
			{
				DelItem(result);
				return -1;
			}
		}

// inicializace objektu
		m_Data[result].Init(m_EmptyItem);
	}

// index nové položky (nebo <0 = chyba)
	return result;
}


////////////////////////////////////////////////////////////////////
// pøidání položky (vrací index položky, <0 = chyba pamìti)
// provádí záznam do UNDO bufferu

template <class OBJECT> int _fastcall CBuffer<OBJECT>::Add(const OBJECT& data)
{
// vytvoøení nové položky
	int result = NewItem();		// vytvoøení nové položky
	if (result >= 0)
	{

// záznam UNDO
		if (m_Undo)
		{
			if (!UndoAddIns(result))
			{
				DelItem(result);
				return -1;
			}
		}

// inicializace objektu
		m_Data[result].Init(data);	// inicializace položky
	}

// index nové položky (nebo <0 = chyba)
	return result;
}

////////////////////////////////////////////////////////////////////
// duplikace položky (neplatná položka se duplikuje na prázdnou, vrací index položky, <0 = chyba pamìti)
// provádí záznam do UNDO bufferu

template <class OBJECT> int _fastcall CBuffer<OBJECT>::Dup(const int index)
{
// vytvoøení nové položky
	int result = NewItem();		// vytvoøení nové položky
	if (result >= 0)
	{

// záznam UNDO
		if (m_Undo)
		{
			if (!UndoAddIns(result))
			{
				DelItem(result);
				return -1;
			}
		}

// kopie položky nebo vytvoøení nové
		if (IsValid(index))					// je index platný?
		{
			m_Data[result].Init(m_Data[index]);	// kopie položky
		}
		else
		{
			m_Data[result].Init(m_EmptyItem); 	// inicializace neplatné položky
		}
	}

// index nové položky (nebo <0 = chyba)
	return result;
}
