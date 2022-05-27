
/***************************************************************************\
*																			*
*									Hudba									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// struktura polo�ky dat hudby (16 bajt� + data)

typedef struct MUSICDATA_
{
	long	Refer;					// (4) ��ta� referenc�
	long	Size;					// (4) velikost dat (bajt�)
	long	Res;					// (4) ... rezerva (pro zarovn�n�)
	BYTE*	Data;					// (4) ukazatel na data

} MUSICDATA;

// velikost polo�ky dat hudby (bez dat)
#define SIZEOFMUSICDATA	(3*sizeof(long) + sizeof(BYTE*))

extern MUSICDATA EmptyMusicData;			// data pr�zdn� hudby


/////////////////////////////////////////////////////////////////////////////
// hudba

class CMusic
{

// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn� - pouze ukazatel na data
	MUSICDATA*		pData;					// ukazatel na z�hlav� hudby

// p�ipojen� dat
	inline void Attach(MUSICDATA* data)
	{
		ASSERT(data);
		pData = data;
		LongIncrement(&(data->Refer));
	}

// odpojen� (a zru�en�) dat
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

// vytvo�en� nov�ho bufferu - star� buffer mus� b�t odpojen!
	inline void NewBuffer(int size)
	{
		ASSERT(size >= 0);
		if (size < 0) size = 0;
		MUSICDATA* data = (MUSICDATA*)MemGet(SIZEOFMUSICDATA);
		pData = data;					// adresa dat
		data->Refer = 1;				// po�et referenc�
		data->Size = size;				// velikost dat
		data->Res = 0;					// rezerva
		data->Data = (BYTE*)MemGet(size);
	}


// ---------------------------- ve�ejn� funkce ------------------------------

public:

// konstruktor a destruktor
	CMusic();								// standardn� konstruktor
	CMusic(const CMusic& src);				// kop�rovac� konstruktor
	CMusic(int size);						// konstruktor s vytvo�en�m dat
	~CMusic();								// standardn� destruktor

// statick� konstruktor a destruktor
	void Init();							// statick� konstruktor
	void Init(MUSICDATA* data);				// statick� konstruktor se zad�n�m dat
	void Init(int size);					// statick� konstruktor s vytvo�en�m dat
	void Term();							// statick� destruktor

// poskytnut� ukazatele na data
	inline MUSICDATA* Data() const { return pData; };
	inline BYTE* DataData() const { return pData->Data; };

// poskytnut� velikosti dat hudby
	inline int Size() const { return pData->Size; };

// kopie do vlastn�ho bufferu p�ed modifikac�
	void CopyWrite();

// vypr�zdn�n� hudby (uvoln�n� dat)
	void Empty();

// test, zda je hudba pr�zdn�
	inline BOOL IsEmpty() { return ((DWORD)pData == (DWORD)&EmptyMusicData); };
	inline BOOL IsNotEmpty() { return ((DWORD)pData != (DWORD)&EmptyMusicData); };

// vytvo�en� nov� hudby (p�ipraveno k z�pisu, data jsou n�hodn�)
	void New(int size);

// na�ten� hudby ze souboru
	void LoadFile();

// ulo�en� do souboru form�tu MID (false=chyba)
//	bool SaveFile(CString jmeno) const;
	void SaveFile() const;

// oper�tor p�i�azen�
	const CMusic& operator= (const CMusic& src);

// oper�tory porovn�n�
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

// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn�
	CMusic*		m_Data;		// ukazatel na data
	int			m_Num;		// po�et platn�ch polo�ek v bufferu
	int			m_Max;		// velikost bufferu (polo�ek)

// vytvo�en� nov� polo�ky
	inline int NewItem()
	{
		int i = m_Num;
		if (i >= m_Max)
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
	CBufMusic();
	~CBufMusic();

// statick� konstruktor a destruktor
	void Init();			// statick� konstruktor
	void Term();			// statick� destruktor

// zru�en� v�ech polo�ek v bufferu
	void DelAll();

// poskytnut� bufferu dat
	inline CMusic* Data() const { return m_Data; };

// poskytnut� po�tu platn�ch polo�ek v bufferu
	inline int Num() const { return m_Num; };

// poskytnut� velikosti bufferu (v�etn� zru�en�ch polo�ek)
	inline int Max() const { return m_Max; };

// kontrola platnosti polo�ky
	inline BOOL IsValid(const int index) const
		{ return ((DWORD)index < (DWORD)m_Num); };

	inline BOOL IsNotValid(const int index) const
		{ return ((DWORD)index >= (DWORD)m_Num); };

// poskytnut� p��stupu k polo�ce (bez kontroly indexu)
	inline CMusic& operator[] (const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const CMusic& operator[] (const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline CMusic& At(const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const CMusic& At(const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

// poskytnut� polo�ky (s kontrolou platnosti indexu)
	const CMusic& _fastcall Get(const int index) const;

// nastaven� polo�ky (s kontrolou platnosti indexu)
	void _fastcall Set(const int index, const CMusic& data);

// vypr�zdn�n� polo�ky (bez jej�ho zru�en� - jen pro uvoln�n� dat)
	void _fastcall Empty(const int index);

// zru�en� polo�ek z konce bufferu
	void _fastcall Del(int num);

// vytvo�en� pr�zdn� polo�ky (vrac� index polo�ky)
	int New();
	int New(int size);

// p�id�n� polo�ky (vrac� index polo�ky)
	int _fastcall Add(const CMusic& data);
	int _fastcall Add(MUSICDATA* data);

// duplikace polo�ky (s kontrolou platnosti indexu, vrac� index prvn� polo�ky)
	int _fastcall Dup(const int index);
	int _fastcall Dup(const int index, int num);

// oper�tor p�i�azen�
	const CBufMusic& operator= (const CBufMusic& src);
};

