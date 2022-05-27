
/***************************************************************************\
*																			*
*						Ikonové promìnné (velikost 32x32)					*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// struktura záhlaví ikony - 24 bajtù

typedef struct ICONDATA_
{
	long	Refer;					// (4) èítaè referencí na ikonu
	long	Param;					// (4) parametry (prùhlednost ikony)
	HICON	HIcon;					// (4) handle ikony Windows (NULL = nevytvoøena)
	HCURSOR	Cursor;					// (4) handle kurzoru myši (NULL = nevytvoøen)
	long	res;					// (4) ...rezerva pro zarovnání
	BYTE*	Data;					// (4) ukazatel na data ikony
} ICONDATA;

#define SIZEOFICONDATA	(5*sizeof(long) + sizeof(BYTE*)) // velikost záhlaví

extern ICONDATA EmptyIconData;		// data prázdné ikony (kurzor myši = implicitní šipka)


/////////////////////////////////////////////////////////////////////////////
// ikona

class CIcon
{

// ------------------------- interní promìnné a funkce ----------------------

private:

// promìnné - pouze ukazatel na data
	ICONDATA*		pData;			// ukazatel na záhlaví ikony

// pøipojení dat
	inline void Attach(ICONDATA* data)
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
			if (pData->HIcon != NULL)
			{
				::DestroyIcon(pData->HIcon);
			}

			if (pData->Cursor != NULL)
			{
				::DestroyCursor(pData->Cursor);
			}

			ASSERT(pData->Data);
			MemFree(pData->Data);		// zrušení dat ikony
			MemFree(pData);				// zrušení záhlaví ikony
#ifdef _DEBUG
			pData = NULL;
#endif
		}
	}

// vytvoøení nového záhlaví a bufferu dat ikony - staré záhlaví musí být odpojeno!
	inline void NewBuffer()
	{
		ICONDATA* data = (ICONDATA*)MemGet(SIZEOFICONDATA); // vytvoøení záhlaví
		pData = data;
		data->Refer = 1;						// poèet referencí
		data->Param = PicParamNone;				// parametry - neznámé
		data->HIcon = NULL;						// není ikona
		data->Cursor = NULL;					// není kurzor
		data->Data = (BYTE*)MemGet(ICONSIZE);	// vytvoøení bufferu dat ikony
	}

// ---------------------------- veøejné funkce ------------------------------

public:

// konstruktor a destruktor
	CIcon();
	CIcon(const CIcon& src);
	~CIcon();

// statický konstruktor a destruktor
	void Init();
	void Init(ICONDATA* data);
	void InitNew();
	void Term();

// poskytnutí ukazatele na data
	inline ICONDATA* Data() const { return pData; };
	inline BYTE* DataData() const { return pData->Data; };

// poskytnutí parametrù ikony (pøi provádìní programu)
	inline int Param() const { return pData->Param; };

// nastavení parametrù ikony
	inline void Param(int param) { pData->Param = param; };

// kopie nových dat ikony - zajistí odpojení dat
	void CopyData(BYTE* src);

// kopie nových dat ikony s konverzí - zajistí odpojení dat
	void CopyKonvData(BYTE* src);

// kopie do vlastního bufferu pøed modifikací
	void CopyWrite();

// vyprázdnìní ikony (uvolnìní dat)
	void Empty();

// test, zda je ikona prázdná
	inline BOOL IsEmpty() { return ((DWORD)pData == (DWORD)&EmptyIconData); };
	inline BOOL IsNotEmpty() { return ((DWORD)pData != (DWORD)&EmptyIconData); };

// vytvoøení nové ikony (pøipraveno k zápisu, data jsou náhodná)
	void New();

// naètení ikony ze souboru
	void LoadFile();

// uložení ikony do souboru
	bool SaveFile(CString jmeno);
	void SaveFile();

// vytvoøení ikony WINDOWS
	HICON HIcon();

// vytvoøení kurzoru myši
	HCURSOR Cursor();

// dekomprimace dat ikony (jsou-li komprimována)
	void DeComp();

// operátor pøiøazení
	const CIcon& operator= (const CIcon& src);
	const CIcon& operator= (ICONDATA* src);

// operátory porovnání
	friend inline BOOL operator==(const CIcon& s1, const CIcon& s2)
		{ return (DWORD)s1.pData == (DWORD)s2.pData; };

	friend inline BOOL operator!=(const CIcon& s1, const CIcon& s2)
		{ return (DWORD)s1.pData != (DWORD)s2.pData; };
};


/***************************************************************************\
*																			*
*								Buffer ikon									*
*																			*
\***************************************************************************/

class CBufIcon
{

// ------------------------- interní promìnné a funkce ----------------------

private:

// promìnné
	CIcon*		m_Data;			// ukazatel na data
	int			m_Num;			// poèet platných položek v bufferu
	int			m_Max;			// velikost bufferu (položek)

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
	CBufIcon();
	~CBufIcon();

// statický konstruktor a destruktor
	void Init();
	void Term();

// zrušení všech položek v bufferu
	void DelAll();

// poskytnutí bufferu ikon
	inline CIcon* Data() const { return m_Data; };

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
	inline CIcon& operator[] (const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; };

	inline const CIcon& operator[] (const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; };

	inline CIcon& At(const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; };

	inline const CIcon& At(const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; };

// poskytnutí položky (s kontrolou platnosti indexu)
	const CIcon& _fastcall Get(const int index) const;

// nastavení položky (s kontrolou platnosti indexu)
	void _fastcall Set(const int index, const CIcon& data);

// vyprázdnìní položky (bez jejího zrušení - jen pro uvolnìní dat)
	void _fastcall Empty(const int index);

// zrušení položek z konce bufferu
	void _fastcall Del(int num);

// operátor pøiøazení
	const CBufIcon& _fastcall operator= (const CBufIcon& src);

// vytvoøení prázdné položky - data neinicializovaná (vrací index položky)
	int New();

// pøidání položky (vrací index položky)
	int _fastcall Add(const CIcon& data);
	int _fastcall Add(ICONDATA* data);

// duplikace položky (s kontrolou platnosti indexu, vrací index první položky)
	int _fastcall Dup(const int index);
	int _fastcall Dup(const int index, int num);
};
