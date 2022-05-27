
/***************************************************************************\
*																			*
*								Textov� �et�zce								*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// struktura polo�ky dat �et�zce - 8 bajt� + data

typedef struct STRINGDATA_
{
	long	Refer;					// (4) ��ta� referenc� na textov� �et�zec
	long	Length;					// (4) d�lka �et�zce ve znac�ch
	long	Orig;					// (4) index origin�ln�ho textu v programu (-1 nen�)
	TCHAR	Data[1];				// (x*1 nebo x*2) textov� data
} STRINGDATA;

#define SIZEOFSTRINGDATA	(3*sizeof(long)) // velikost z�hlav� datov� polo�ky (bez dat)

extern STRINGDATA EmptyStringData;	// data pr�zdn�ho �et�zce


/////////////////////////////////////////////////////////////////////////////
// textov� �et�zec

class CString
{

// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn� - pouze ukazatel na data
	STRINGDATA*		pData;			// ukazatel na data �et�zce

// p�ipojen� dat
	inline void Attach(STRINGDATA* data)
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
			MemFree(pData);
#ifdef _DEBUG
			pData = NULL;
#endif
		}
	}

// vytvo�en� nov�ho bufferu (d�lka zad�na ve znac�ch) - star� buffer mus� b�t odpojen!
	inline void NewBuffer(int length)
	{
		ASSERT(length >= 0);
		if (length < 0) length = 0;
		STRINGDATA* data = (STRINGDATA*)MemGet(
				(length+1)*sizeof(TCHAR) + SIZEOFSTRINGDATA);
		pData = data;					// adresa dat
		data->Refer = 1;				// po�et referenc�
		data->Length = length;			// d�lka
		data->Orig = -1;				// index origin�ln�ho textu
		data->Data[length] = 0;			// ozna�en� konce textu
	}

// zm�na velikosti bufferu (d�lka zad�na ve znac�ch) - buffer mus� b�t p�ivlastn�n!
	inline void SizeBuffer(int length)
	{
		ASSERT(pData->Refer == 1);
		ASSERT(length >= 0);
		if (length < 0) length = 0;
		STRINGDATA* data = (STRINGDATA*)MemSize(pData, 
				(length+1)*sizeof(TCHAR) + SIZEOFSTRINGDATA);
		pData = data;					// adresa dat
		data->Length = length;			// d�lka
		data->Orig = -1;				// index origin�ln�ho textu
		data->Data[length] = 0;			// ozna�en� konce textu
	}

// slu�ovac� konstruktory (ur�eno pro oper�tor +)
	inline CString(const CString& str1, const CString& str2) 
	{ 
		int len1 = str1.pData->Length;
		int len2 = str2.pData->Length;
		NewBuffer(len1+len2);
		MemCopy(pData->Data, str1.pData->Data, len1*sizeof(TCHAR));
		MemCopy(pData->Data + len1, str2.pData->Data, len2*sizeof(TCHAR));
	}

	inline CString(LPCTSTR txt, const CString& str) 
	{ 
		int len1 = 0;
		if (txt) len1 = lstrlen(txt);
		int len2 = str.pData->Length;
		NewBuffer(len1+len2);
		MemCopy(pData->Data, txt, len1*sizeof(TCHAR));
		MemCopy(pData->Data + len1, str.pData->Data, len2*sizeof(TCHAR));
	}

	inline CString(const CString& str, LPCTSTR txt) 
	{ 
		int len1 = str.pData->Length;
		int len2 = 0;
		if (txt) len2 = lstrlen(txt);
		NewBuffer(len1+len2);
		MemCopy(pData->Data, str.pData->Data, len1*sizeof(TCHAR));
		MemCopy(pData->Data + len1, txt, len2*sizeof(TCHAR));
	}

	inline CString(const TCHAR chr, const CString& str) 
	{ 
		int len = str.pData->Length;
		NewBuffer(1+len);
		pData->Data[0] = chr;
		MemCopy(pData->Data + 1, str.pData->Data, len*sizeof(TCHAR));
	}

	inline CString(const CString& str, const TCHAR chr) 
	{ 
		int len = str.pData->Length;
		NewBuffer(len+1);
		MemCopy(pData->Data, str.pData->Data, len*sizeof(TCHAR));
		pData->Data[len] = chr;
	}

// ---------------------------- ve�ejn� funkce ------------------------------

public:

// konstruktor a destruktor
	CString();
	CString(const CString& src);
	CString(const TCHAR chr);
	CString(LPCTSTR text, const int length = -1);
#ifdef _UNICODE
	CString(LPCSTR text, const int length = -1);
#else //_UNICODE
	CString(LPCWSTR text, const int length = -1);
#endif //_UNICODE
	~CString();

// statick� konstruktor a destruktor
	void Init();
	void Init(STRINGDATA* data);
	void Init(const CString& src);
	void Init(LPCTSTR text, const int length = -1);
#ifdef _UNICODE
	void Init(LPCSTR text, const int length = -1);
#else //_UNICODE
	void Init(LPCWSTR text, const int length = -1);
#endif //_UNICODE
	void Term();

// poskytnut� adresy dat
	inline STRINGDATA* Data() const { return pData; };
	inline TCHAR* DataData() const { return pData->Data; };

// poskytnut�/nastaven� d�lky �et�zce
	inline int Length() const { return pData->Length; }
	void Length(int len);

// poskytnut�/nastaven� indexu origin�ln�ho textu (-1=nen�)
	inline int Orig() const { return pData->Orig; }
	inline void Orig(int orig) { pData->Orig = orig; }

// kontrola, zda je �et�zec pr�zdn�
	inline BOOL IsEmpty() const { return pData->Length == 0; };
	inline BOOL IsNotEmpty() const { return pData->Length != 0; };

// z�pis do bufferu (bez koncov� nuly!)
	void Write(TCHAR* buf) const;
#ifdef _UNICODE
	void Write(LPSTR buf) const;
#else
	void Write(LPWSTR buf) const;
#endif

// ------- hled�n�

// hled�n� (-1 = nenalezeno)
	int _fastcall Find(const CString& str) const;
	int _fastcall Find(LPCTSTR txt) const;
	int _fastcall Find(const TCHAR chr) const;
	
// hled�n� znaku od konce �et�zce zp�t (-1 = nenalezeno)
	int _fastcall RevFind(const TCHAR chr) const;

// po�et ��dk� textu
	int LineNum() const;
	CString _fastcall GetLine(int radek) const;

// blok textu (-1=konec ��dku nebo textu)
	CString GetBlock(int begline, int begpos, int endline, int endpos) const;

// -------- modifikace textu

// vypr�zdn�n� �et�zce (uvoln�n� dat)
	void Empty();

// konverze mal�/velk� p�smena
	void UpperCase();
	void LowerCase();

// lev� ��st �et�zce
	CString _fastcall Left(int count) const;

// prav� ��st �et�zce
	CString _fastcall Right(int count) const;

// st�edn� ��st �et�zce
	CString _fastcall Mid(int first, int count = 0x7fffffff) const;

// zru�en� textu z konce �et�zce
	void Delete(int first, int count = 0x7fffffff);

// odstran�n� mezer ze za��tku/konce �et�zce
	void TrimLeft();
	void TrimRight();

// korekce v�skytu nuly v textu (prov�d� se ve spole�n�m bufferu v�ech prom�nn�ch)
	void KorigNul();

// nastaven� ��sla jm�na objektu (1, 2, ...)
	void _fastcall SetNumObj(const int num);

// na�ten� textu z resource
	void Load(const int nID);

// na�ten� ze souboru form�tu TXT (false=chyba)
//	bool LoadFile(CString jmeno);

// ulo�en� textu do souboru form�tu TXT (false=chyba)
//	bool SaveFile(CString jmeno) const;

// ------- p��stup ke znak�m v bufferu

// kopie do vlastn�ho bufferu p�ed modifikac�
	void CopyWrite();

// kontrola, zda je index znaku platn�
	inline BOOL IsValid(const int index) const
		{ return ((DWORD)index < (DWORD)pData->Length); };

	inline BOOL IsNotValid(const int index) const
		{ return ((DWORD)index >= (DWORD)pData->Length); };

// poskytnut� p��stupu ke znaku na pozici (bez kontroly a bez p�ivlastn�n� bufferu!)
	inline TCHAR& operator[] (int index)
		{ ASSERT(IsValid(index)); return pData->Data[index]; }

	inline const TCHAR& operator[] (int index) const
		{ ASSERT(IsValid(index)); return pData->Data[index]; }

	inline TCHAR& At(int index)
		{ ASSERT(IsValid(index)); return pData->Data[index]; }

	inline const TCHAR& At(int index) const
		{ ASSERT(IsValid(index)); return pData->Data[index]; }

// poskytnut� znaku na pozici s kontrolou
	TCHAR _fastcall Get(const int index) const;

// nastaven� znaku na pozici s kontrolou (zajist� p�ivlastn�n� bufferu)
	void _fastcall Set(const int index, const TCHAR chr);

// poskytnut� posledn�ho znaku �et�zce (pro pr�zdn� �et�zec vrac� 0)
	TCHAR _fastcall LastChar() const;

// ------- text oken

// na�ten� textu okna
	void GetWindowText(const HWND wnd);

// nastaven� textu okna
	void _fastcall SetWindowText(const HWND wnd) const;

// na�ten� textu dialogov�ho prvku
	void GetDialogText(const HWND wnd, int id);

// nastaven� textu dialogov�ho prvku
	void _fastcall SetDialogText(const HWND wnd, int id) const;

// -------- adres��e a soubory

// na�ten� aktivn�ho adres��e
	void GetAktDir();

// korekce textu na jm�no souboru (vypu�t�n� zak�zan�ch znak�, 
//	vrac� opravenou pozici kurzoru)
	int FileName(int curs);

// korekce textu na jm�no cesty (vypu�t�n� zak�zan�ch znak�, 
//	vrac� opravenou pozici kurzoru)
	int PathName(int curs);

// jm�no p�echodn�ho souboru
	void TempName();


// -------- konverze ��sel

// konverze ��sla INT na text
	void Int(int num);

// konverze textu na ��slo INT
	friend int Int(LPCTSTR text);

// p�id�n� 2 ��slic
	void _fastcall Add2Dig(char num);

// p�id�n� 4 ��slic
	void _fastcall Add4Dig(short num);

// p�id�n� 2 ��slic se zarovn�n�m mezerou
	void _fastcall Add2DigSpc(char num);

// p�id�n� textu se zn�mou d�lkou
	void _fastcall Add(LPCTSTR text, int num);
#ifdef _UNICODE
	void _fastcall Add(char* text, int num);
#else
	void _fastcall Add(WCHAR* text, int num);
#endif

// konverze ��sla DWORD na HEX (8 ��slic)
	void Hex(DWORD num);

// konverze ��sla DOUBLE na text
	void Double(double num);

// konverze textu na ��slo DOUBLE
	friend double Double(LPCTSTR txt);

// -------- oper�tory

// konverze na �et�zec LPCTSTR
	inline operator LPCTSTR() const { return pData->Data; }

// oper�tory p�i�azen�
	const CString& _fastcall operator= (const CString& str);
	const CString& _fastcall operator= (LPCTSTR txt);
#ifdef _UNICODE
	const CString& _fastcall operator= (LPCSTR txt);
#else //_UNICODE
	const CString& _fastcall operator= (LPCWSTR txt);
#endif //_UNICODE
	const CString& _fastcall operator= (const TCHAR chr);

// oper�tory p�i�ten�
	const CString& _fastcall operator+=(const CString& str);
	const CString& _fastcall operator+=(LPCTSTR txt);
	const CString& _fastcall operator+=(const TCHAR chr);

// oper�tory sou�tu
	friend CString _fastcall operator+ (const CString& str1, const CString& str2);
	friend CString _fastcall operator+ (LPCTSTR txt, const CString& str);
	friend CString _fastcall operator+ (const CString& str, LPCTSTR txt);
	friend CString _fastcall operator+ (const TCHAR chr, const CString& str);
	friend CString _fastcall operator+ (const CString& str, const TCHAR chr);

// oper�tory porovn�n�
	friend bool _fastcall operator==(const CString& str1, const CString& str2);
	friend bool _fastcall operator==(LPCTSTR txt, const CString& str);
	friend bool _fastcall operator==(const CString& str, LPCTSTR txt);
	friend bool _fastcall operator==(const CString& str, const TCHAR chr);
	friend bool _fastcall operator==(const TCHAR chr, const CString& str);
	friend bool _fastcall operator!=(const CString& str1, const CString& str2);
	friend bool _fastcall operator!=(LPCTSTR txt, const CString& str);
	friend bool _fastcall operator!=(const CString& str, LPCTSTR txt);
	friend bool _fastcall operator!=(const CString& str, const TCHAR chr);
	friend bool _fastcall operator!=(const TCHAR chr, const CString& str);
	friend bool _fastcall operator< (const CString& str1, const CString& str2);
	friend bool _fastcall operator<=(const CString& str1, const CString& str2);
	friend bool _fastcall operator> (const CString& str1, const CString& str2);
	friend bool _fastcall operator>=(const CString& str1, const CString& str2);
};


/***************************************************************************\
*																			*
*								Buffer text�								*
*																			*
\***************************************************************************/

class CBufText
{

// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn�
	CString*	m_Data;		// ukazatel na data
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
	CBufText();
	~CBufText();

// statick� konstruktor a destruktor
	void Init();
	void Term();

// zru�en� v�ech polo�ek v bufferu
	void DelAll();

// poskytnut� bufferu dat
	inline CString* Data() const { return m_Data; };

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
	inline CString& operator[] (const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const CString& operator[] (const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline CString& At(const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const CString& At(const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

// poskytnut� polo�ky (s kontrolou platnosti indexu)
	const CString& _fastcall Get(const int index) const;

// nastaven� polo�ky (s kontrolou platnosti indexu)
	void _fastcall Set(const int index, const CString& data);

// vypr�zdn�n� polo�ky (bez jej�ho zru�en� - jen pro uvoln�n� dat)
	void _fastcall Empty(const int index);

// zru�en� polo�ek z konce bufferu
	void _fastcall Del(int num);

// vytvo�en� pr�zdn� polo�ky (vrac� index polo�ky)
	int New();

// p�id�n� polo�ky (vrac� index polo�ky)
	int _fastcall Add(const CString& data);
	int _fastcall Add(const WCHAR* data, const int len = -1);
	int _fastcall Add(const char* data, const int len = -1);

// duplikace polo�ky (s kontrolou platnosti indexu, vrac� index prvn� polo�ky)
	int _fastcall Dup(const int index);
	int _fastcall Dup(const int index, int num);

// na�ten� textu z resource (vrac� index polo�ky)
	int _fastcall Load(const int nID);

// oper�tor p�i�azen�
	const CBufText& operator= (const CBufText& src);
};

