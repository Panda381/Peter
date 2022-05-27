
/***************************************************************************\
*																			*
*							Buffer re�ln�ch ��sel							*
*																			*
\***************************************************************************/


class CBufReal
{

// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn�
	double*		m_Data;		// ukazatel na data
	int			m_Num;		// po�et platn�ch polo�ek v bufferu
	int			m_Max;		// velikost bufferu (polo�ek)

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
	CBufReal();
	~CBufReal();

// statick� konstruktor a destruktor
	void Init();			// statick� konstruktor
	void Term();			// statick� destruktor

// zru�en� v�ech polo�ek v bufferu
	void DelAll();

// zji�t�n� po�tu platn�ch polo�ek v bufferu
	inline int Num() const { return m_Num; };

// nastaven� po�tu polo�ek v bufferu (nov� polo�ky neinicializovan�)
	void _fastcall Num(int num);

// zji�t�n� velikosti bufferu (v�etn� zru�en�ch polo�ek)
	inline int Max() const { return m_Max; };

// data bufferu
	inline double* Data() const { return m_Data; };

// kontrola platnosti polo�ky
	inline BOOL IsValid(const int index) const
		{ return ((DWORD)index < (DWORD)m_Num); };

	inline BOOL IsNotValid(const int index) const
		{ return ((DWORD)index >= (DWORD)m_Num); };

// poskytnut� p��stupu k polo�ce (bez kontroly indexu)
	inline double& operator[] (const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

// poskytnut� p��stupu k polo�ce (bez kontroly indexu)
	inline const double& operator[] (const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

// poskytnut� polo�ky (s kontrolou platnosti indexu)
	double _fastcall Get(const int index) const;

// nastaven� polo�ky (s kontrolou platnosti indexu)
	void _fastcall Set(const int index, const double data);

// vlo�en� polo�ky do bufferu na po�adovanou pozici (omez� index)
	void _fastcall Insert(int index, const double data);

// zru�en� polo�ek z konce bufferu
	void _fastcall Del(int num);

// zru�en� polo�ky z bufferu (ostatn� polo�ky se p�isunou)
	void _fastcall DelItem(const int index);

// vytvo�en� pr�zdn� polo�ky (vrac� index polo�ky)
	int New();

// p�id�n� polo�ky (vrac� index polo�ky)
	int _fastcall Add(const double data);

// duplikace polo�ky (s kontrolou platnosti indexu, vrac� index prvn� polo�ky)
	int _fastcall Dup(const int index);
	int _fastcall Dup(const int index, int num);

// na�ten� ��sla ze souboru (na��t� do prom�nn�, true=operace OK)
//	bool LoadFile(double& data, CString jmeno);

// ulo�en� ��sla do souboru form�tu NUM (false=chyba)
//	bool SaveFile(double data, CString jmeno) const;

// oper�tor p�i�azen�
	const CBufReal& operator= (const CBufReal& src);
};
