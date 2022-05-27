
/***************************************************************************\
*																			*
*							Buffer p�emapov�n� index�						*
*																			*
\***************************************************************************/

class CBufIndex
{

// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn�
	int*	m_Data;			// ukazatel na data
	int		m_Num;			// po�et polo�ek v bufferu
	int		m_Max;			// velikost bufferu (polo�ek)

// ---------------------------- ve�ejn� funkce ------------------------------

public:

// konstruktor a destruktor
	CBufIndex();
	~CBufIndex();

// statick� konstruktor a destruktor
	void Init();
	void Term();

// zru�en� v�ech polo�ek v bufferu
	void DelAll();

// poskytnut� adresy dat
	inline int* Data() const { return m_Data; };

// poskytnut�/nastaven� po�tu polo�ek v bufferu
// (nov� polo�ky neinicializovan�, vrac� FALSE=chyba pam�ti)
	inline int Num() const { return m_Num; };
	bool _fastcall Num(const int num);

// nastaven� po�tu polo�ek s vymaz�n�m "-1", vrac� FALSE=chyba pam�ti
	bool _fastcall NumClear(const int num);

// vymaz�n� v�ech polo�ek v bufferu zadanou hodnotou
	void _fastcall Clear(const int clear);

// kontrola platnosti polo�ky (kontroluje se pouze rozsah indexu)
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

// poskytnut� polo�ky (s kontrolou platnosti indexu, pro neplatnou vr�t� -1)
	int _fastcall Get(const int index) const;

// nastaven� polo�ky (s kontrolou platnosti indexu)
	void _fastcall Set(const int index, const int data);

// p�id�n� polo�ky na konec bufferu (vrac� index polo�ky, p�i chyb� pam�ti vrac� <0)
	int _fastcall Add(const int data);

// oper�tor p�i�azen� (p�i chyb� pam�ti po�et polo�ek nesouhlas�)
	const CBufIndex& _fastcall operator= (const CBufIndex& srcbuf);
};
