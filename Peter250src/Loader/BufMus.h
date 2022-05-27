
/***************************************************************************\
*																			*
*									Hudba									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// struktura položky dat hudby (16 bajtù + data)

typedef struct MUSICDATA_
{
	long	Refer;					// (4) èítaè referencí
	long	Size;					// (4) velikost dat (bajtù)
	long	Res;					// (4) ... rezerva (pro zarovnání)
	BYTE*	Data;					// (4) ukazatel na data

} MUSICDATA;

// velikost položky dat hudby (bez dat)
#define SIZEOFMUSICDATA	(3*sizeof(long) + sizeof(BYTE*))

extern MUSICDATA EmptyMusicData;			// data prázdné hudby


/////////////////////////////////////////////////////////////////////////////
// hudba

class CMusic
{

// ------------------------- interní promìnné a funkce ----------------------

private:

// promìnné - pouze ukazatel na data
	MUSICDATA*		pData;					// ukazatel na záhlaví hudby

// pøipojení dat
	inline void Attach(MUSICDATA* data)
	{
		ASSERT(data);
		pData = data;
		LongIncrement(&(data->Refer));
	}

// odpojení (a zrušení) dat
	inline void Detach()
	{
		ASSERT(pData);
		ASSERT(pData->Refer > 0);

		if (LongDecrement(&(pData->Refer)))
		{
			ASSERT(pData->Data);
			MemFree(pData->Data);
			MemFree(pData);
#ifdef _DEBUG
			pData = NULL;
#endif
		}
	}

// vytvoøení nového bufferu - starý buffer musí být odpojen!
	inline void NewBuffer(int size)
	{
		ASSERT(size >= 0);
		if (size < 0) size = 0;
		MUSICDATA* data = (MUSICDATA*)MemGet(SIZEOFMUSICDATA);
		pData = data;					// adresa dat
		data->Refer = 1;				// poèet referencí
		data->Size = size;				// velikost dat
		data->Res = 0;					// rezerva
		data->Data = (BYTE*)MemGet(size);
	}


// ---------------------------- veøejné funkce ------------------------------

public:

// konstruktor a destruktor
	CMusic();								// standardní konstruktor
	CMusic(const CMusic& src);				// kopírovací konstruktor
	CMusic(int size);						// konstruktor s vytvoøením dat
	~CMusic();								// standardní destruktor

// statický konstruktor a destruktor
	void Init();							// statický konstruktor
	void Init(MUSICDATA* data);				// statický konstruktor se zadáním dat
	void Init(int size);					// statický konstruktor s vytvoøením dat
	void Term();							// statický destruktor

// poskytnutí ukazatele na data
	inline MUSICDATA* Data() const { return pData; };
	inline BYTE* DataData() const { return pData->Data; };

// poskytnutí velikosti dat hudby
	inline int Size() const { return pData->Size; };

// kopie do vlastního bufferu pøed modifikací
	void CopyWrite();

// vyprázdnìní hudby (uvolnìní dat)
	void Empty();

// test, zda je hudba prázdná
	inline BOOL IsEmpty() { return ((DWORD)pData == (DWORD)&EmptyMusicData); };
	inline BOOL IsNotEmpty() { return ((DWORD)pData != (DWORD)&EmptyMusicData); };

// vytvoøení nové hudby (pøipraveno k zápisu, data jsou náhodná)
	void New(int size);

// naètení hudby ze souboru
	void LoadFile();

// uložení do souboru formátu MID (false=chyba)
//	bool SaveFile(CString jmeno) const;
	void SaveFile() const;

// operátor pøiøazení
	const CMusic& operator= (const CMusic& src);

// operátory porovnání
	friend inline BOOL operator==(const CMusic& s1, const CMusic& s2)
		{ return (DWORD)s1.pData == (DWORD)s2.pData; };

	friend inline BOOL operator!=(const CMusic& s1, const CMusic& s2)
		{ return (DWORD)s1.pData != (DWORD)s2.pData; };
};

/***************************************************************************\
*																			*
*								Buffer hudby								*
*																			*
\***************************************************************************/

class CBufMusic
{

// ------------------------- interní promìnné a funkce ----------------------

private:

// promìnné
	CMusic*		m_Data;		// ukazatel na data
	int			m_Num;		// poèet platných položek v bufferu
	int			m_Max;		// velikost bufferu (položek)

// vytvoøení nové položky
	inline int NewItem()
	{
		int i = m_Num;
		if (i >= m_Max)
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
	CBufMusic();
	~CBufMusic();

// statický konstruktor a destruktor
	void Init();			// statický konstruktor
	void Term();			// statický destruktor

// zrušení všech položek v bufferu
	void DelAll();

// poskytnutí bufferu dat
	inline CMusic* Data() const { return m_Data; };

// poskytnutí poètu platných položek v bufferu
	inline int Num() const { return m_Num; };

// poskytnutí velikosti bufferu (vèetnì zrušených položek)
	inline int Max() const { return m_Max; };

// kontrola platnosti položky
	inline BOOL IsValid(const int index) const
		{ return ((DWORD)index < (DWORD)m_Num); };

	inline BOOL IsNotValid(const int index) const
		{ return ((DWORD)index >= (DWORD)m_Num); };

// poskytnutí pøístupu k položce (bez kontroly indexu)
	inline CMusic& operator[] (const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const CMusic& operator[] (const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline CMusic& At(const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const CMusic& At(const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

// poskytnutí položky (s kontrolou platnosti indexu)
	const CMusic& _fastcall Get(const int index) const;

// nastavení položky (s kontrolou platnosti indexu)
	void _fastcall Set(const int index, const CMusic& data);

// vyprázdnìní položky (bez jejího zrušení - jen pro uvolnìní dat)
	void _fastcall Empty(const int index);

// zrušení položek z konce bufferu
	void _fastcall Del(int num);

// vytvoøení prázdné položky (vrací index položky)
	int New();
	int New(int size);

// pøidání položky (vrací index položky)
	int _fastcall Add(const CMusic& data);
	int _fastcall Add(MUSICDATA* data);

// duplikace položky (s kontrolou platnosti indexu, vrací index první položky)
	int _fastcall Dup(const int index);
	int _fastcall Dup(const int index, int num);

// operátor pøiøazení
	const CBufMusic& operator= (const CBufMusic& src);
};

