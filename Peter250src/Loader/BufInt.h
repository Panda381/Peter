
/***************************************************************************\
*																			*
*								Buffer cel�ch ��sel							*
*																			*
\***************************************************************************/

class CBufInt
{

// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn�
	int*	m_Data;			// ukazatel na data
	int		m_Num;			// po�et platn�ch polo�ek v bufferu
	int		m_Max;			// velikost bufferu (polo�ek)

// vytvo�en� nov� polo�ky
	inline int NewItem()
	{
		int i = m_Num;
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
	CBufInt();
	CBufInt(int num);		// vytvo�en� bufferu + vymaz�n� hodnotou "-1"
	~CBufInt();

// statick� konstruktor a destruktor
	void Init();
	void Init(int num);		// vytvo�en� bufferu + vymaz�n� hodnotou "-1"
	void Term();

// zru�en� v�ech polo�ek v bufferu
	void DelAll();

// poskytnut� bufferu dat
	inline int* Data() const { return m_Data; };

// poskytnut� po�tu platn�ch polo�ek v bufferu
	inline int Num() const { return m_Num; };

// nastaven� po�tu polo�ek v bufferu (nov� polo�ky neinicializovan�)
	void _fastcall Num(int num);

// poskytnut� velikosti bufferu
	inline int Max() const { return m_Max; };

// vymaz�n� v�ech polo�ek v bufferu zadanou hodnotou
	void _fastcall Clear(const int clear = -1);

// kontrola platnosti polo�ky
	inline BOOL IsValid(const int index) const
		{ return ((DWORD)index < (DWORD)m_Num); };

	inline BOOL IsNotValid(const int index) const
		{ return ((DWORD)index >= (DWORD)m_Num); };

// poskytnut� p��stupu k polo�ce (bez kontroly indexu)
	inline int& operator[] (const int index) 
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const int& operator[] (const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline int& At(const int index) 
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const int& At(const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

// poskytnut� polo�ky (s kontrolou platnosti indexu)
	int _fastcall Get(const int index) const;

// nastaven� polo�ky (s kontrolou platnosti indexu)
	void _fastcall Set(const int index, const int data);

// vlo�en� polo�ky do bufferu na po�adovanou pozici (omez� index)
	void _fastcall Insert(int index, const int data);

// zru�en� polo�ek z konce bufferu
	void _fastcall Del(int num);

// zru�en� polo�ky z bufferu (ostatn� polo�ky se p�isunou)
	void _fastcall DelItem(const int index);

// vytvo�en� pr�zdn� polo�ky (vrac� index polo�ky)
	int New();

// p�id�n� polo�ky (vrac� index polo�ky)
	int _fastcall Add(const int data);

// duplikace polo�ky (s kontrolou platnosti indexu, vrac� index prvn� polo�ky)
	int _fastcall Dup(const int index);
	int _fastcall Dup(const int index, int num);

// oper�tor p�i�azen�
	const CBufInt& operator= (const CBufInt& src);
};


/***************************************************************************\
*																			*
*							Buffer logick�ch prom�nn�ch						*
*																			*
\***************************************************************************/

class CBufBool
{ 

// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn�
	bool*	m_Data;			// ukazatel na data
	int		m_Num;			// po�et platn�ch polo�ek v bufferu
	int		m_Max;			// velikost bufferu (polo�ek)

// vytvo�en� nov� polo�ky
	inline int NewItem()
	{
		int i = m_Num;
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
	CBufBool();
	CBufBool(int num);		// vytvo�en� bufferu + vymaz�n� hodnotou false
	~CBufBool();

// statick� konstruktor a destruktor
	void Init();
	void Term();

// zru�en� v�ech polo�ek v bufferu
	void DelAll();

// poskytnut� bufferu dat
	inline bool* Data() const { return m_Data; };

// poskytnut� po�tu platn�ch polo�ek v bufferu
	inline int Num() const { return m_Num; };

// poskytnut� velikosti bufferu
	inline int Max() const { return m_Max; };

// nastaven� po�tu polo�ek v bufferu (nov� polo�ky neinicializovan�)
	void _fastcall Num(int num);

// vymaz�n� v�ech polo�ek v bufferu zadanou hodnotou
	void _fastcall Clear(const bool clear);

// kontrola platnosti polo�ky
	inline BOOL IsValid(const int index) const
		{ return ((DWORD)index < (DWORD)m_Num); };

	inline BOOL IsNotValid(const int index) const
		{ return ((DWORD)index >= (DWORD)m_Num); };

// poskytnut� p��stupu k polo�ce (bez kontroly indexu)
	inline bool& operator[] (const int index) 
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const bool& operator[] (const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline bool& At(const int index) 
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const bool& At(const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

// poskytnut� polo�ky (s kontrolou platnosti indexu)
	bool _fastcall Get(const int index) const;

// nastaven� polo�ky (s kontrolou platnosti indexu)
	void _fastcall Set(const int index, const bool data);

// zru�en� polo�ek z konce bufferu
	void _fastcall Del(int num);

// vytvo�en� pr�zdn� polo�ky (vrac� index polo�ky)
	int New();

// p�id�n� polo�ky (vrac� index polo�ky)
	int _fastcall Add(const bool data);

// duplikace polo�ky (s kontrolou platnosti indexu, vrac� index prvn� polo�ky)
	int _fastcall Dup(const int index);
	int _fastcall Dup(const int index, int num);

// oper�tor p�i�azen�
	const CBufBool& operator= (const CBufBool& src);

// na�ten� logick� hodnoty ze souboru (na��t� do prom�nn�, TRUE=operace OK)
//	bool LoadFile(bool& data, CString jmeno);

// ulo�en� logick� hodnoty do souboru form�tu LOG (FALSE=chyba)
//	bool SaveFile(bool data, CString jmeno) const;
};
