
/***************************************************************************\
*																			*
*								Textov� �et�zce								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// struktura polo�ky dat �et�zce (8 bajt� + data + koncov� 0)

typedef struct TEXTDATA_
{
	long	Refer;					// (4) ��ta� referenc� na textov� �et�zec
	long	Length;					// (4) d�lka �et�zce ve znac�ch (bez koncov� 0)
	TCHAR	Data[1];				// (x*1 nebo x*2) textov� data + koncov� 0
} TEXTDATA;

#define SIZEOFTEXTDATA	(2*sizeof(long)) // velikost z�hlav� datov� polo�ky (bez dat)

extern TEXTDATA EmptyTextData;	// data pr�zdn�ho �et�zce
extern	const CText	EmptyText;	// pr�zdn� �et�zec

/////////////////////////////////////////////////////////////////////////////
// statick� funkce

// vytvo�en� dat textu - d�lka zad�na ve znac�ch (p�i chyb� pam�ti vrac� NULL)
TEXTDATA* _fastcall NewTextData(int length);

// zru�en� dat textu (odd�leno kv�li lep�� optimalizaci)
void _fastcall DelTextData(TEXTDATA* data);


/////////////////////////////////////////////////////////////////////////////
// textov� �et�zec

class CText
{

// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn�
	TEXTDATA*		pData;			// ukazatel na data textu

// p�ipojen� dat textu
	inline void attach(TEXTDATA* data)
	{
		ASSERT(data != NULL);

		pData = data;
		LongIncrement(&(data->Refer));
	}

// odpojen� (a zru�en�) dat textu
	inline void detach(TEXTDATA* data)
	{
		ASSERT(data != NULL);
		ASSERT(data->Refer > 0);

		if (LongDecrement(&(data->Refer)))
		{
			DelTextData(data);
		}
	}

// zm�na velikosti bufferu (d�lka zad�na ve znac�ch) - zajist� p�ivlastn�n� bufferu
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool SizeBuffer(int length);

// ---------------------------- ve�ejn� funkce ------------------------------

public:

// konstruktor a destruktor
	CText();
	CText(const CText& src);
	CText(TEXTDATA* data);
	CText(const TCHAR chr);					// p�i chyb� pam�ti bude �et�zec pr�zdn�
	CText(LPCTSTR text);						// p�i chyb� pam�ti bude �et�zec pr�zdn�
	CText(LPCTSTR text, const int length);	// p�i chyb� pam�ti bude �et�zec pr�zdn�
#ifdef _UNICODE
	CText(LPCSTR text);						// p�i chyb� pam�ti bude �et�zec pr�zdn�
	CText(LPCSTR text, const int length);		// p�i chyb� pam�ti bude �et�zec pr�zdn�
#else //_UNICODE
	CText(LPCWSTR text);						// p�i chyb� pam�ti bude �et�zec pr�zdn�
	CText(LPCWSTR text, const int length);	// p�i chyb� pam�ti bude �et�zec pr�zdn�
#endif //_UNICODE
	~CText();

// slu�ovac� konstruktury
	CText(const CText& str1, const CText& str2);	// p�i chyb� pam�ti bude �et�zec pr�zdn�
	CText(LPCTSTR txt, const CText& str);			// p�i chyb� pam�ti bude �et�zec pr�zdn�
	CText(const CText& str, LPCTSTR txt);			// p�i chyb� pam�ti bude �et�zec pr�zdn�
	CText(const TCHAR chr, const CText& str);		// p�i chyb� pam�ti bude �et�zec pr�zdn�
	CText(const CText& str, const TCHAR chr);		// p�i chyb� pam�ti bude �et�zec pr�zdn�

// statick� konstruktor a destruktor
	void Init();
	void _fastcall Init(const CText& src);
	void _fastcall Init(TEXTDATA* data);
	bool _fastcall Init(const TCHAR chr);		// p�i chyb� pam�ti vr�t� FALSE, text nen� platn�
	bool _fastcall Init(LPCTSTR text);			// p�i chyb� pam�ti vr�t� FALSE, text nen� platn�
	bool _fastcall Init(LPCTSTR text, const int length); // p�i chyb� pam�ti vr�t� FALSE, text nen� platn�
#ifdef _UNICODE
	bool _fastcall Init(LPCSTR text);			// p�i chyb� pam�ti vr�t� FALSE, text nen� platn�
	bool _fastcall Init(LPCSTR text, const int length); // p�i chyb� pam�ti vr�t� FALSE, text nen� platn�
#else //_UNICODE
	bool _fastcall Init(LPCWSTR text);			// p�i chyb� pam�ti vr�t� FALSE, text nen� platn�
	bool _fastcall Init(LPCWSTR text, const int length); // p�i chyb� pam�ti vr�t� FALSE, text nen� platn�
#endif //_UNICODE
	void Term();

// poskytnut� adresy dat
	inline TEXTDATA* Data() const { return pData; };
	inline TCHAR* DataData() const { return pData->Data; };

// poskytnut� d�lky �et�zce
	inline int Length() const { return pData->Length; }

// kontrola, zda je �et�zec pr�zdn�
	inline BOOL IsEmpty() const { return pData->Length == 0; };
	inline BOOL IsNotEmpty() const { return pData->Length != 0; };

// z�pis do bufferu (bez koncov� nuly)
	void Write(TCHAR* buf) const;
#ifdef _UNICODE
	void Write(LPSTR buf) const;
#else
	void Write(LPWSTR buf) const;
#endif

// z�pis do bufferu (s koncovou nulou)
	void WriteNull(TCHAR* buf) const;
#ifdef _UNICODE
	void WriteNull(LPSTR buf) const;
#else
	void WriteNull(LPWSTR buf) const;
#endif


// ------- hled�n�

// hled�n� (<0 = nenalezeno, pos=v�choz� pozice)
	int _fastcall Find(const CText& str) const;
	int _fastcall Find(const CText& str, int pos) const;
	int _fastcall Find(LPCTSTR txt) const;
	int _fastcall Find(LPCTSTR txt, int pos) const;
	int _fastcall Find(const TCHAR chr) const;
	int _fastcall Find(const TCHAR chr, int pos) const;
	
// hled�n� znaku od konce �et�zce zp�t (<0 = nenalezeno, pos=v�choz� pozice)
	int _fastcall RevFind(const TCHAR chr) const;
	int _fastcall RevFind(const TCHAR chr, int pos) const;

// -------- modifikace textu

// vypr�zdn�n� �et�zce (p�ipoj� pr�zdn� �et�zec)
	void Empty();

// konverze mal�/velk� p�smena
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool UpperCase();
	bool LowerCase();

// normalizace na ASCII znaky - pro jm�no souboru
	void NormAscii();

// lev� ��st �et�zce (p�i chyb� pam�ti vr�t� pr�zdn� �et�zec)
	CText _fastcall Left(int count) const;

// prav� ��st �et�zce (p�i chyb� pam�ti vr�t� pr�zdn� �et�zec)
	CText _fastcall Right(int count) const;

// st�edn� ��st �et�zce (p�i chyb� pam�ti vr�t� pr�zdn� �et�zec)
	CText _fastcall Mid(int first, int count = 0x7fffffff) const;

// zru�en� textu z konce �et�zce
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool Delete(int first, int count = 0x7fffffff);

// zru�en� posledn�ho znaku z konce �et�zce
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool DeleteLast();

// odstran�n� mezer ze za��tku/konce �et�zce
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool TrimLeft();
	bool TrimRight();

// korekce v�skytu nuly v textu (prov�d� se ve spole�n�m bufferu v�ech prom�nn�ch)
	void KorigNul();

// korekce jm�na na zkr�cen� jm�no pro knihovnu
	void KorigShort();

// nastaven� ��sla jm�na objektu (1, 2, ...)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool _fastcall SetNumObj(const int num);

// p�id�n� textu
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool _fastcall Add(const CText& str);
	bool _fastcall Add(LPCTSTR txt);
	bool _fastcall Add(const TCHAR chr);

// p�id�n� textu ��sla
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool _fastcall AddInt(const int num);
	bool _fastcall AddDouble(const double num);

// sou�et text�
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool _fastcall Add(const CText& str1, const CText& str2);
	bool _fastcall Add(LPCTSTR txt, const CText& str);
	bool _fastcall Add(const CText& str, LPCTSTR txt);
	bool _fastcall Add(const TCHAR chr, const CText& str);
	bool _fastcall Add(const CText& str, const TCHAR chr);

// nastaven� textu
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	void _fastcall Set(const CText& str);
	bool _fastcall Set(LPCTSTR txt);
	bool _fastcall Set(LPCTSTR txt, const int length);
#ifdef _UNICODE
	bool _fastcall Set(LPCSTR txt);
	bool _fastcall Set(LPCSTR txt, const int length);
#else //_UNICODE
	bool _fastcall Set(LPCWSTR txt);
	bool _fastcall Set(LPCWSTR txt, const int length);
#endif //_UNICODE
	bool _fastcall Set(const TCHAR chr);

// na�ten� textu z resource do dialogov�ho prvku
	void LoadDialogText(const HWND wnd, int id);

// na�ten� textu z resource do dialogov�ho prvku s nastaven�m b�n�ho fontu
	void LoadDialogTextFont(const HWND wnd, int id);

// na�ten� textu z resource (p�i chyb� vrac� FALSE, obsah pr�zdn� text)
	bool Load(const int nID);

// na�ten� ze souboru form�tu TXT (FALSE=chyba, obsah nezm�n�n)
	bool LoadFile(CText jmeno);

// ulo�en� textu do souboru form�tu TXT (FALSE=chyba)
	bool SaveFile(CText jmeno) const;

// ------- p��stup ke znak�m v bufferu

// kopie do vlastn�ho bufferu p�ed modifikac�
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool CopyWrite();

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

// poskytnut� znaku na pozici s kontrolou (pro neplatn� znak vr�t� 0)
	TCHAR _fastcall Get(const int index) const;

// nastaven� znaku na pozici s kontrolou (zajist� p�ivlastn�n� bufferu)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool _fastcall Set(const int index, const TCHAR chr);

// poskytnut� posledn�ho znaku �et�zce (pro pr�zdn� �et�zec vrac� 0)
	TCHAR _fastcall LastChar() const;

// ------- text oken

// na�ten� textu okna (p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n)
	bool GetWindowText(const HWND wnd);

// nastaven� textu okna
	void _fastcall SetWindowText(const HWND wnd) const;
	void _fastcall SetWindowTextFont(const HWND wnd) const;

// na�ten� textu dialogov�ho prvku (p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n)
	bool GetDialogText(const HWND wnd, int id);

// nastaven� textu dialogov�ho prvku
	void _fastcall SetDialogText(const HWND wnd, int id) const;

// nastaven� textu dialogov�ho prvku s nastaven�m b�n�ho fontu
	void _fastcall SetDialogTextFont(const HWND wnd, int id) const;

// -------- adres��e a soubory

// na�ten� aktivn�ho adres��e (p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n)
	bool GetAktDir();

// korekce textu na jm�no souboru (vypu�t�n� zak�zan�ch znak�, 
//	vrac� opravenou pozici kurzoru)
//  ignoruje chybu pam�ti p�i p�ivlastn�n� bufferu (p�ivlastnit p�edem)
	int FileName(int curs);

// korekce textu na jm�no cesty (vypu�t�n� zak�zan�ch znak�, 
//	vrac� opravenou pozici kurzoru)
//  ignoruje chybu pam�ti p�i p�ivlastn�n� bufferu (p�ivlastnit p�edem)
	int PathName(int curs);

// -------- konverze ��sel

// konverze ��sla INT na text (p�i chyb� pam�ti vrac� FALSE)
	bool Int(int num);

// konverze textu na ��slo INT
	friend int Int(LPCTSTR text);

// konverze ��sla DWORD na HEX (8 ��slic) (p�i chyb� pam�ti vrac� FALSE)
	bool Hex(DWORD num);

// konverze ��sla DOUBLE na text (p�i chyb� pam�ti vrac� false)
	bool Double(double num);

// konverze textu na ��slo DOUBLE
	friend double Double(LPCTSTR txt);

// -------- oper�tory

// konverze na �et�zec LPCTSTR
	inline operator LPCTSTR() const { return pData->Data; }

// oper�tory p�i�azen� (nepou��vat - nehl�s� chybu operace)
	inline const CText& _fastcall operator= (const CText& str) { Set(str); return *this; }
	inline const CText& _fastcall operator= (LPCTSTR txt) { Set(txt); return *this; }
#ifdef _UNICODE
	inline const CText& _fastcall operator= (LPCSTR txt) { Set(txt); return *this; }
#else //_UNICODE
	inline const CText& _fastcall operator= (LPCWSTR txt) { Set(txt); return *this; }
#endif //_UNICODE
	inline const CText& _fastcall operator= (const TCHAR chr) { Set(chr); return *this; };

// oper�tory p�i�ten� (nepou��vat - nehl�s� chybu operace)

// POZOR - funkce "+= CText" asi nefunguje (ru�� star� obsah) !!!
	inline const CText& _fastcall operator+=(const CText& str) { Add(str); return *this; }
	inline const CText& _fastcall operator+=(LPCTSTR txt) { Add(txt); return *this; }
	inline const CText& _fastcall operator+=(const TCHAR chr) { Add(chr); return *this; }

// oper�tory sou�tu (nepou��vat - nehl�s� chybu operace)
	inline friend CText _fastcall operator+ (const CText& str1, const CText& str2) { return CText(str1, str2); }
	inline friend CText _fastcall operator+ (LPCTSTR txt, const CText& str) { return CText(txt, str); }
	inline friend CText _fastcall operator+ (const CText& str, LPCTSTR txt) { return CText(str, txt); }
	inline friend CText _fastcall operator+ (const TCHAR chr, const CText& str) { return CText(chr, str); }
	inline friend CText _fastcall operator+ (const CText& str, const TCHAR chr) { return CText(str, chr); }

// oper�tory porovn�n�
	friend BOOL _fastcall operator==(const CText& str1, const CText& str2);
	friend BOOL _fastcall operator==(LPCTSTR txt, const CText& str);
	friend BOOL _fastcall operator==(const CText& str, LPCTSTR txt);
	friend BOOL _fastcall operator==(const CText& str, const TCHAR chr);
	friend BOOL _fastcall operator==(const TCHAR chr, const CText& str);
	friend BOOL _fastcall operator!=(const CText& str1, const CText& str2);
	friend BOOL _fastcall operator!=(LPCTSTR txt, const CText& str);
	friend BOOL _fastcall operator!=(const CText& str, LPCTSTR txt);
	friend BOOL _fastcall operator!=(const CText& str, const TCHAR chr);
	friend BOOL _fastcall operator!=(const TCHAR chr, const CText& str);
	friend bool _fastcall operator< (const CText& str1, const CText& str2);
	friend bool _fastcall operator<=(const CText& str1, const CText& str2);
	friend bool _fastcall operator> (const CText& str1, const CText& str2);
	friend bool _fastcall operator>=(const CText& str1, const CText& str2);
};


/***************************************************************************\
*																			*
*								Buffer text�								*
*																			*
\***************************************************************************/

class CBufText : public CBuffer<CText>
{

// ---------------------------- ve�ejn� funkce ------------------------------

public:

// p�id�n� polo�ky (vrac� index polo�ky, <0 = chyba)
// prov�d� z�znam do UNDO bufferu
	inline int _fastcall Add(const CText& data) { return CBuffer<CText>::Add(data); }
	int _fastcall Add(TEXTDATA* data);
	int _fastcall Add(const WCHAR* data);
	int _fastcall Add(const WCHAR* data, const int len);
	int _fastcall Add(const char* data);
	int _fastcall Add(const char* data, const int len);

// na�ten� textu z resource (vrac� index polo�ky, <0 = chyba)
	int _fastcall Load(const int nID);
};


/***************************************************************************\
*																			*
*						Buffer v�cejazy�n�ch text�							*
*																			*
\***************************************************************************/

//extern const CMultiText	EmptyMultiText;		// pr�zdn� v�cejazykov� �et�zec

// v�cejazy�n� text
class CMultiText
{

// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn�
	CText		m_Data[JAZYKNUM];	// texty (pr�zdn� = je implicitn�)

// ---------------------------- ve�ejn� funkce ------------------------------

public:

// konstruktor a destruktor
	void Init()
	{
		for (int i = 0; i < JAZYKNUM; i++)
		{
			m_Data[i].Init();
		}
	}

// inicializace s kopi�
	void Init(const CMultiText& src)
	{
		for (int i = 0; i < JAZYKNUM; i++)
		{
			m_Data[i].Init(src.m_Data[i]);
		}
	}

// p�eru�en�
	void Term()
	{
		for (int i = 0; i < JAZYKNUM; i++)
		{
			m_Data[i].Term();
		}
	}

// vypr�zdn�n�
	void Empty()
	{
		for (int i = 0; i < JAZYKNUM; i++)
		{
			m_Data[i].Empty();
		}
	}

// �ten� zadan� polo�ky jazyku
	inline const CText& _fastcall MultiText(int index) const { return m_Data[index]; }

// nastaven� zadan� polo�ky jazyku
	inline void _fastcall MultiText(int index, const CText& txt) { m_Data[index] = txt; }

// nastaven� b�n�ho textu
	inline void _fastcall MultiText(const CText& txt) { m_Data[Jazyk] = txt; }

// zji�t�n� b�n�ho textu
	const CText& _fastcall MultiText() const;

// na�ten� textu z resource
	void _fastcall Load(const int nID);

// nastaven� ��sla jm�na objektu (1, 2, ...)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool _fastcall SetNumObj(const int num);

// nastaven� v�cejazy�n�ho textu
	inline const CMultiText& _fastcall operator= (const CMultiText& str)
	{
		for (int i = 0; i < JAZYKNUM; i++) { m_Data[i] = str.m_Data[i]; }
		return *this;
	}
};


class CBufMultiText : public CBuffer<CMultiText>
{

// ---------------------------- ve�ejn� funkce ------------------------------

public:

// p�id�n� polo�ky (vrac� index polo�ky, <0 = chyba)
// prov�d� z�znam do UNDO bufferu
	inline int _fastcall AddTxt(const CText& data)
	{ 
		int i = CBuffer<CMultiText>::New();
		m_Data[i].MultiText(data);
		return i;
	}

// zji�t�n� polo�ky
	inline const CText& _fastcall GetTxt(const int index) const
	{
		return (CBuffer<CMultiText>::Get(index)).MultiText();
	}

// nastaven� polo�ky
	inline void _fastcall SetTxt(const int index, const CText& txt)
	{
		if (CBuffer<CMultiText>::IsValid(index))
		{
			(CBuffer<CMultiText>::At(index)).MultiText(txt);
		}
	}


// na�ten� polo�ek z resource
	int _fastcall Load(const int nID);

//	int _fastcall Add(TEXTDATA* data);
//	int _fastcall Add(const WCHAR* data);
//	int _fastcall Add(const WCHAR* data, const int len);
//	int _fastcall Add(const char* data);
//	int _fastcall Add(const char* data, const int len);

// na�ten� textu z resource (vrac� index polo�ky, <0 = chyba)
//	int _fastcall Load(const int nID);
};
