
/***************************************************************************\
*																			*
*								�ablona buffer�								*
*																			*
\***************************************************************************/

// Minim�ln� velikost objektu mus� b�t 4 Bajty!!!!!

template <class OBJECT> class CBuffer
{

// ------------------------- intern� prom�nn� a funkce ----------------------

protected:

// prom�nn�
	OBJECT*		m_Data;			// buffer dat
	bool*		m_Valid;		// buffer platnosti polo�ek
	int			m_Num;			// po�et platn�ch polo�ek v bufferu
	int			m_Max;			// velikost bufferu (polo�ek)
	int			m_Next;			// p��t� voln� polo�ka (-1=nen�)
	bool		m_Undo;			// po�adavek registrace zm�n pro UNDO
	OBJECT		m_EmptyItem;	// pr�zdn� objekt

// vytvo�en� nov�ch dat (vrac� TRUE=operace OK)
	bool NewData();

// vytvo�en� nov� polo�ky (vrac� index polo�ky, p�i chyb� vrac� < 0)
	int NewItem();

// zru�en� polo�ky - za�azen� do voln�ch polo�ek (nekontroluje index)
	void _fastcall DelItem(const int index);
		
// p�id�n� z�znamu o vlo�en� polo�ky do UNDO bufferu
	bool UndoAddIns(int index);

// ---------------------------- ve�ejn� funkce ------------------------------

public:

// konstruktor a destruktor
	CBuffer();
	~CBuffer();

// statick� konstruktor a destruktor
	void Init();
	void Term();

// zru�en� v�ech polo�ek v bufferu (ukl�d�n� za�ne op�t po �ad� od za��tku)
// prov�d� z�znam do UNDO bufferu, p�i chyb� pam�ti vynuluje UNDO
	void DelAll();

// poskytnut� pr�zdn�ho objektu
	inline OBJECT& EmptyItem() { return m_EmptyItem; };
	inline const OBJECT& EmptyItem() const { return m_EmptyItem; };

// poskytnut� bufferu dat
	inline OBJECT* Data() const { return m_Data; };

// poskytnut� bufferu platnosti polo�ek
	inline bool* Valid() const { return m_Valid; };

// poskytnut� po�tu platn�ch polo�ek v bufferu
	inline int Num() const { return m_Num; };

// poskytnut� velikosti bufferu (v�etn� zru�en�ch polo�ek)
	inline int Max() const { return m_Max; };

// nastaven� po�adavku registrace zm�n pro UNDO
	inline void UndoOn() { m_Undo = true; };
	inline void UndoOff() { m_Undo = false; };
	inline void UndoSet(const bool undo) { m_Undo = undo; };

// kontrola platnosti polo�ky
	inline BOOL IsValid(const int index) const
		{ return (((DWORD)index < (DWORD)m_Max) && m_Valid[index]); };

	inline BOOL IsNotValid(const int index) const
		{ return (((DWORD)index >= (DWORD)m_Max) || !m_Valid[index]); };

// poskytnut� p��stupu k polo�ce (bez kontroly indexu)
	inline OBJECT& operator[] (const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; };

	inline const OBJECT& operator[] (const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; };

	inline OBJECT& At(const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; };

	inline const OBJECT& At(const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; };

// poskytnut� polo�ky (s kontrolou platnosti indexu - pro neplatnou vr�t� pr�zdnou polo�ku)
	const OBJECT& _fastcall Get(const int index) const;

// nastaven� polo�ky (s kontrolou platnosti indexu)
	void _fastcall Set(const int index, const OBJECT& data);

// zru�en� polo�ky (s kontrolou platnosti indexu) (vrac� FALSE=chyba pam�ti)
// prov�d� z�znam do UNDO bufferu
	bool _fastcall Del(const int index);

// navr�cen� polo�ky v UNDO operaci (vrac� TRUE=operace OK)
// prov�d� z�znam do UNDO bufferu
	bool UndoIns(const OBJECT& data, const int index);

// oper�tor p�i�azen� (neobsluhuje chybu pam�ti!)
	const CBuffer& _fastcall operator= (const CBuffer& src);

// vytvo�en� nov� pr�zdn� polo�ky (vrac� index polo�ky, <0 = chyba)
// prov�d� z�znam do UNDO bufferu
	int New();

// p�id�n� polo�ky (vrac� index polo�ky, <0 = chyba)
// prov�d� z�znam do UNDO bufferu
	int _fastcall Add(const OBJECT& data);

// duplikace polo�ky (neplatn� polo�ka se duplikuje na pr�zdnou, vrac� index polo�ky, <0 = chyba pam�ti)
// prov�d� z�znam do UNDO bufferu
	int _fastcall Dup(const int index);
};


////////////////////////////////////////////////////////////////////
// vytvo�en� nov�ch dat (vrac� TRUE=operace OK)

#define NEWDATANUM 256				// po�et nov� vytvo�en�ch polo�ek (typicky 1 KB)

template <class OBJECT> bool CBuffer<OBJECT>::NewData()
{
// nov� po�et polo�ek
	int next = m_Max;				// p��t� polo�ka - 1
	int max = next + NEWDATANUM;	// nov� po�et polo�ek

// zv�t�en� bufferu dat
	OBJECT* newdata = (OBJECT*)MemSize(m_Data, max*sizeof(OBJECT));
	if (newdata == NULL) return false;
	m_Data = newdata;

// zv�t�en� bufferu platnosti
	bool* newvalid = (bool*)MemSize(m_Valid, max*sizeof(bool));
	if (newvalid == NULL) return false;
	m_Valid = newvalid;

// nov� maxim�ln� po�et polo�ek v bufferu
	m_Max = max;

// vymaz�n� p��znak� platnosti polo�ek (nastaven� na p��znak neplatnosti)
	int i;
	newvalid += next;
	for (i = NEWDATANUM; i > 0; i--) { *newvalid = false; newvalid++; }

// za�len�n� do �et�zce voln�ch polo�ek
	newdata += next - 1;			// ukazatel dat - 1
	for (i = NEWDATANUM; i > 0; i--)
	{
		newdata++;					// zv��en� ukazatele polo�ek
		next++;						// zv��en� indexu p��t� polo�ky
		*(int*)newdata = next;		// odkaz na p��t� polo�ku
	}
	*(int*)newdata = m_Next;		// nav�z�n� na dal�� polo�ku
	m_Next = m_Max-NEWDATANUM;		// odkaz na prvn� novou polo�ku

// p��znak operace OK
	return true;
};

////////////////////////////////////////////////////////////////////
// vytvo�en� nov� polo�ky (vrac� index polo�ky, p�i chyb� vrac� < 0)

template <class OBJECT> int CBuffer<OBJECT>::NewItem()
{
// vytvo�en� nov�ch dat, nen�-li voln� dal�� polo�ka
	if (m_Next < 0)				// nen� dal�� polo�ka?
	{
		if (!NewData()) return -1;	// vytvo�en� nov�ch dat
	}

// vyjmut� polo�ky z �et�zce voln�ch polo�ek
	int i = m_Next;				// p��t� voln� polo�ka
	m_Next = *(int*)(m_Data + i); // dal�� polo�ka
	m_Valid[i] = true;			// nastaven� p��znaku platnosti polo�ky
	m_Num++;					// zv��en� ��ta�e platn�ch polo�ek

// nov� vytvo�en� polo�ka
	return i;
};

////////////////////////////////////////////////////////////////////
// zru�en� polo�ky - za�azen� do voln�ch polo�ek (nekontroluje index a neru�� objekt)

template <class OBJECT> void _fastcall CBuffer<OBJECT>::DelItem(const int index)
{
	*(int*)(m_Data + index) = m_Next;	// p��t� voln� polo�ka
	m_Valid[index] = false;				// zru�en� p��znaku platnosti
	m_Num--;							// sn�en� ��ta�e platn�ch polo�ek
	m_Next = index;						// odkaz na tuto polo�ku
}

////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

template <class OBJECT> CBuffer<OBJECT>::CBuffer()
{
	m_Data = NULL;						// nen� buffer dat
	m_Valid = NULL;						// nen� buffer platnosti
	m_Num = 0;							// nen� ��dn� platn� polo�ka
	m_Max = 0;							// nen� buffer polo�ek
	m_Next = -1;						// p�i�t� voln� polo�ka (-1=nen�)
	m_Undo = false;						// neregistrovat zm�ny pro UNDO
	m_EmptyItem.Empty();
}

template <class OBJECT> CBuffer<OBJECT>::~CBuffer()
{
	DelAll();							// zru�en� v�ech polo�ek
}

/////////////////////////////////////////////////////////////////////////////
// statick� konstruktor a destruktor

template <class OBJECT> void CBuffer<OBJECT>::Init()
{
	m_Data = NULL;						// nen� buffer dat
	m_Valid = NULL;						// nen� buffer platnosti
	m_Num = 0;							// nen� ��dn� platn� polo�ka
	m_Max = 0;							// nen� buffer polo�ek
	m_Next = -1;						// p�i�t� voln� polo�ka (-1=nen�)
	m_Undo = false;						// neregistrovat zm�ny pro UNDO
	m_EmptyItem.Init();
}

template <class OBJECT> void CBuffer<OBJECT>::Term()
{
	DelAll();							// zru�en� v�ech polo�ek
	m_EmptyItem.Term();
}

////////////////////////////////////////////////////////////////////
// zru�en� v�ech polo�ek v bufferu (ukl�d�n� za�ne op�t po �ad� od za��tku)
// prov�d� z�znam do UNDO bufferu, p�i chyb� pam�ti vynuluje UNDO

template <class OBJECT> void CBuffer<OBJECT>::DelAll()
{
// zru�en� v�ech polo�ek
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

// zru�en� buffer�
	MemFree(m_Data);			// zru�en� bufferu dat
	m_Data = NULL;				// nen� buffer dat
	MemFree(m_Valid);			// zru�en� bufferu platnosti
	m_Valid = NULL;				// nen� bufferu platnosti
	m_Num = 0;					// nen� ��dn� platn� polo�ka
	m_Max = 0;					// nen� ��dn� polo�ka v bufferu
	m_Next = -1;				// nen� p��t� polo�ka
}

////////////////////////////////////////////////////////////////////
// poskytnut� polo�ky (s kontrolou platnosti indexu - pro neplatnou vr�t� pr�zdnou polo�ku)

template <class OBJECT> const OBJECT& _fastcall CBuffer<OBJECT>::Get(const int index) const
{
	if (IsValid(index))			// je index platn�?
	{
		return m_Data[index];	// polo�ka na dan�m indexu
	}
	return m_EmptyItem;			// pro neplatn� index vr�t� pr�zdn� objekt
}

////////////////////////////////////////////////////////////////////
// nastaven� polo�ky (s kontrolou platnosti indexu)

template <class OBJECT> void _fastcall CBuffer<OBJECT>::Set(const int index, const OBJECT& data)
{
	if (IsValid(index))					// je index platn�?
	{
		m_Data[index] = data;
	}
}

////////////////////////////////////////////////////////////////////
// zru�en� polo�ky (s kontrolou platnosti indexu, vrac� FALSE=chyba pam�ti)
// prov�d� z�znam do UNDO bufferu

template <class OBJECT> bool _fastcall CBuffer<OBJECT>::Del(const int index)
{
// kontrola platnosti polo�ky
	if (IsValid(index))						// je index platn�?
	{

// z�znam o zru�en� do UNDO bufferu
		if (m_Undo)
		{
			if (!Undo.AddDel(index, m_Data[index]))
			{
				return false;
			}
		}

// zru�en� polo�ky
		m_Data[index].Term();				// ukon�en� objektu
		DelItem(index);						// zru�en� polo�ky
	}
	return true;
}

////////////////////////////////////////////////////////////////////
// navr�cen� polo�ky v UNDO operaci (vrac� TRUE=operace OK)
// prov�d� z�znam do UNDO bufferu

template <class OBJECT> bool CBuffer<OBJECT>::UndoIns(const OBJECT& data, const int index)
{
// vytvo�en� nov�ch dat (zaji�t�n� existence polo�ky)
	ASSERT(index >= 0);
	while (index >= m_Max)
	{
		if (!NewData()) return false;
	}

// adresa vkl�dan� polo�ky
	ASSERT(IsNotValid(index));					// polo�ka mus� b�t neplatn�
	OBJECT* item = m_Data + index;				// adresa nov� polo�ky

// z�znam operace pro UNDO
	if (m_Undo)
	{
		if (!UndoAddIns(index)) return false;
	}

// vyjmut� polo�ky z voln�ch polo�ek
	int i = m_Next;								// ukazatel �et�zce voln�ch polo�ek

// navr�cena hned prvn� polo�ka z �et�zce voln�ch polo�ek
	if (i == index)								// je to hned prvn� polo�ka?
	{
		m_Next = *(int*)item;					// odkaz na dal�� polo�ku
	}
	else
	{

// nalezen� polo�ky v �et�zci voln�ch polo�ek
		while (*(int*)(m_Data + i) != index)
		{
			i = *(int*)(m_Data + i);

// tento p��pad nesm� nikdy nastat - polo�ka nebyla nalezena!!!
			ASSERT(i >= 0);
			if (i < 0)								// to je chyba!!!!!!!
			{
				Undo.DelAll();
				return false;
			}
		}

// p�esko�en� odkazu na dal�� polo�ku
		*(int*)(m_Data + i) = *(int*)item;
	}

// nastaven� p��znaku platnosti polo�ky
	m_Num++;									// zv��en� ��ta�e polo�ek
	m_Valid[index] = true;						// p��znak platnosti polo�ky

// inicializace polo�ky
	item->Init(data);

// p��znak operace OK
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// oper�tor p�i�azen� (neobsluhuje chybu pam�ti!)

template <class OBJECT> const CBuffer<OBJECT>& _fastcall CBuffer<OBJECT>::operator= (const CBuffer<OBJECT>& src)
{
// zru�en� v�ech star�ch dat
	DelAll();					// zru�en� star�ch dat

// cyklus p�es v�echny polo�ky
	int index = 0;				// index na��tan� polo�ky
	for (int i = src.Max(); i > 0; i--) // pro v�echny polo�ky v bufferu
	{

// p�id�n� polo�ky do bufferu
		if (src.m_Valid[index])	// je to platn� polo�ka?
		{
			Add(src[index]);	// kopie polo�ky
		}
		index++;				// inkrementace �tec�ho indexu
	}

// souhlas� po�et polo�ek?
	ASSERT(m_Num == src.Num());
	return *this;
}

////////////////////////////////////////////////////////////////////
// vytvo�en� nov� pr�zdn� polo�ky (vrac� index polo�ky, <0 = chyba)
// prov�d� z�znam do UNDO bufferu

template <class OBJECT> int CBuffer<OBJECT>::New()
{
// vytvo�en� nov� polo�ky
	int result = NewItem();				// vytvo�en� nov� polo�ky
	if (result >= 0)
	{

// z�znam UNDO
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

// index nov� polo�ky (nebo <0 = chyba)
	return result;
}


////////////////////////////////////////////////////////////////////
// p�id�n� polo�ky (vrac� index polo�ky, <0 = chyba pam�ti)
// prov�d� z�znam do UNDO bufferu

template <class OBJECT> int _fastcall CBuffer<OBJECT>::Add(const OBJECT& data)
{
// vytvo�en� nov� polo�ky
	int result = NewItem();		// vytvo�en� nov� polo�ky
	if (result >= 0)
	{

// z�znam UNDO
		if (m_Undo)
		{
			if (!UndoAddIns(result))
			{
				DelItem(result);
				return -1;
			}
		}

// inicializace objektu
		m_Data[result].Init(data);	// inicializace polo�ky
	}

// index nov� polo�ky (nebo <0 = chyba)
	return result;
}

////////////////////////////////////////////////////////////////////
// duplikace polo�ky (neplatn� polo�ka se duplikuje na pr�zdnou, vrac� index polo�ky, <0 = chyba pam�ti)
// prov�d� z�znam do UNDO bufferu

template <class OBJECT> int _fastcall CBuffer<OBJECT>::Dup(const int index)
{
// vytvo�en� nov� polo�ky
	int result = NewItem();		// vytvo�en� nov� polo�ky
	if (result >= 0)
	{

// z�znam UNDO
		if (m_Undo)
		{
			if (!UndoAddIns(result))
			{
				DelItem(result);
				return -1;
			}
		}

// kopie polo�ky nebo vytvo�en� nov�
		if (IsValid(index))					// je index platn�?
		{
			m_Data[result].Init(m_Data[index]);	// kopie polo�ky
		}
		else
		{
			m_Data[result].Init(m_EmptyItem); 	// inicializace neplatn� polo�ky
		}
	}

// index nov� polo�ky (nebo <0 = chyba)
	return result;
}
