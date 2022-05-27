
/***************************************************************************\
*																			*
*						Ikonov� prom�nn� (velikost 32x32)					*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// struktura z�hlav� ikony - 24 bajt�

typedef struct ICONDATA_
{
	long	Refer;					// (4) ��ta� referenc� na ikonu
	long	Param;					// (4) parametry (pr�hlednost ikony)
	HICON	HIcon;					// (4) handle ikony Windows (NULL = nevytvo�ena)
	HCURSOR	Cursor;					// (4) handle kurzoru my�i (NULL = nevytvo�en)
	long	res;					// (4) ...rezerva pro zarovn�n�
	BYTE*	Data;					// (4) ukazatel na data ikony
} ICONDATA;

#define SIZEOFICONDATA	(5*sizeof(long) + sizeof(BYTE*)) // velikost z�hlav�

extern ICONDATA EmptyIconData;		// data pr�zdn� ikony (kurzor my�i = implicitn� �ipka)


/////////////////////////////////////////////////////////////////////////////
// ikona

class CIcon
{

// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn� - pouze ukazatel na data
	ICONDATA*		pData;			// ukazatel na z�hlav� ikony

// p�ipojen� dat
	inline void Attach(ICONDATA* data)
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
			if (pData->HIcon != NULL)
			{
				::DestroyIcon(pData->HIcon);
			}

			if (pData->Cursor != NULL)
			{
				::DestroyCursor(pData->Cursor);
			}

			ASSERT(pData->Data);
			MemFree(pData->Data);		// zru�en� dat ikony
			MemFree(pData);				// zru�en� z�hlav� ikony
#ifdef _DEBUG
			pData = NULL;
#endif
		}
	}

// vytvo�en� nov�ho z�hlav� a bufferu dat ikony - star� z�hlav� mus� b�t odpojeno!
	inline void NewBuffer()
	{
		ICONDATA* data = (ICONDATA*)MemGet(SIZEOFICONDATA); // vytvo�en� z�hlav�
		pData = data;
		data->Refer = 1;						// po�et referenc�
		data->Param = PicParamNone;				// parametry - nezn�m�
		data->HIcon = NULL;						// nen� ikona
		data->Cursor = NULL;					// nen� kurzor
		data->Data = (BYTE*)MemGet(ICONSIZE);	// vytvo�en� bufferu dat ikony
	}

// ---------------------------- ve�ejn� funkce ------------------------------

public:

// konstruktor a destruktor
	CIcon();
	CIcon(const CIcon& src);
	~CIcon();

// statick� konstruktor a destruktor
	void Init();
	void Init(ICONDATA* data);
	void InitNew();
	void Term();

// poskytnut� ukazatele na data
	inline ICONDATA* Data() const { return pData; };
	inline BYTE* DataData() const { return pData->Data; };

// poskytnut� parametr� ikony (p�i prov�d�n� programu)
	inline int Param() const { return pData->Param; };

// nastaven� parametr� ikony
	inline void Param(int param) { pData->Param = param; };

// kopie nov�ch dat ikony - zajist� odpojen� dat
	void CopyData(BYTE* src);

// kopie nov�ch dat ikony s konverz� - zajist� odpojen� dat
	void CopyKonvData(BYTE* src);

// kopie do vlastn�ho bufferu p�ed modifikac�
	void CopyWrite();

// vypr�zdn�n� ikony (uvoln�n� dat)
	void Empty();

// test, zda je ikona pr�zdn�
	inline BOOL IsEmpty() { return ((DWORD)pData == (DWORD)&EmptyIconData); };
	inline BOOL IsNotEmpty() { return ((DWORD)pData != (DWORD)&EmptyIconData); };

// vytvo�en� nov� ikony (p�ipraveno k z�pisu, data jsou n�hodn�)
	void New();

// na�ten� ikony ze souboru
	void LoadFile();

// ulo�en� ikony do souboru
	bool SaveFile(CString jmeno);
	void SaveFile();

// vytvo�en� ikony WINDOWS
	HICON HIcon();

// vytvo�en� kurzoru my�i
	HCURSOR Cursor();

// dekomprimace dat ikony (jsou-li komprimov�na)
	void DeComp();

// oper�tor p�i�azen�
	const CIcon& operator= (const CIcon& src);
	const CIcon& operator= (ICONDATA* src);

// oper�tory porovn�n�
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

// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn�
	CIcon*		m_Data;			// ukazatel na data
	int			m_Num;			// po�et platn�ch polo�ek v bufferu
	int			m_Max;			// velikost bufferu (polo�ek)

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
	CBufIcon();
	~CBufIcon();

// statick� konstruktor a destruktor
	void Init();
	void Term();

// zru�en� v�ech polo�ek v bufferu
	void DelAll();

// poskytnut� bufferu ikon
	inline CIcon* Data() const { return m_Data; };

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
	inline CIcon& operator[] (const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; };

	inline const CIcon& operator[] (const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; };

	inline CIcon& At(const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; };

	inline const CIcon& At(const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; };

// poskytnut� polo�ky (s kontrolou platnosti indexu)
	const CIcon& _fastcall Get(const int index) const;

// nastaven� polo�ky (s kontrolou platnosti indexu)
	void _fastcall Set(const int index, const CIcon& data);

// vypr�zdn�n� polo�ky (bez jej�ho zru�en� - jen pro uvoln�n� dat)
	void _fastcall Empty(const int index);

// zru�en� polo�ek z konce bufferu
	void _fastcall Del(int num);

// oper�tor p�i�azen�
	const CBufIcon& _fastcall operator= (const CBufIcon& src);

// vytvo�en� pr�zdn� polo�ky - data neinicializovan� (vrac� index polo�ky)
	int New();

// p�id�n� polo�ky (vrac� index polo�ky)
	int _fastcall Add(const CIcon& data);
	int _fastcall Add(ICONDATA* data);

// duplikace polo�ky (s kontrolou platnosti indexu, vrac� index prvn� polo�ky)
	int _fastcall Dup(const int index);
	int _fastcall Dup(const int index, int num);
};
