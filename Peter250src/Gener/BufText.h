
/***************************************************************************\
*																			*
*								Textové øetìzce								*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// struktura položky dat øetìzce (8 bajtù + data + koncová 0)

typedef struct TEXTDATA_
{
	long	Refer;					// (4) èítaè referencí na textový øetìzec
	long	Length;					// (4) délka øetìzce ve znacích (bez koncové 0)
	TCHAR	Data[1];				// (x*1 nebo x*2) textová data + koncová 0
} TEXTDATA;

#define SIZEOFTEXTDATA	(2*sizeof(long)) // velikost záhlaví datové položky (bez dat)

extern TEXTDATA EmptyTextData;	// data prázdného øetìzce
extern	const CText	EmptyText;	// prázdný øetìzec

/////////////////////////////////////////////////////////////////////////////
// statické funkce

// vytvoøení dat textu - délka zadána ve znacích (pøi chybì pamìti vrací NULL)
TEXTDATA* _fastcall NewTextData(int length);

// zrušení dat textu (oddìleno kvùli lepší optimalizaci)
void _fastcall DelTextData(TEXTDATA* data);


/////////////////////////////////////////////////////////////////////////////
// textový øetìzec

class CText
{

// ------------------------- interní promìnné a funkce ----------------------

private:

// promìnné
	TEXTDATA*		pData;			// ukazatel na data textu

// pøipojení dat textu
	inline void attach(TEXTDATA* data)
	{
		ASSERT(data != NULL, "Chyba pøipojení dat textové promìnné");

		pData = data;
		LongIncrement(&(data->Refer));
	}

// odpojení (a zrušení) dat textu
	inline void detach(TEXTDATA* data)
	{
		ASSERT(data != NULL, "Chyba odpojení dat textové promìnné");
		ASSERT(data->Refer > 0, "Chyba odpojení dat textové promìnné");

		if (LongDecrement(&(data->Refer)))
		{
			DelTextData(data);
		}
	}

// zmìna velikosti bufferu (délka zadána ve znacích) - zajistí pøivlastnìní bufferu
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool SizeBuffer(int length);

// ---------------------------- veøejné funkce ------------------------------

public:

// konstruktor a destruktor
	CText();
	CText(const CText& src);
	CText(TEXTDATA* data);
	CText(const TCHAR chr);					// pøi chybì pamìti bude øetìzec prázdný
	CText(LPCTSTR text);						// pøi chybì pamìti bude øetìzec prázdný
	CText(LPCTSTR text, const int length);	// pøi chybì pamìti bude øetìzec prázdný
#ifdef _UNICODE
	CText(LPCSTR text);						// pøi chybì pamìti bude øetìzec prázdný
	CText(LPCSTR text, const int length);		// pøi chybì pamìti bude øetìzec prázdný
#else //_UNICODE
	CText(LPCWSTR text);						// pøi chybì pamìti bude øetìzec prázdný
	CText(LPCWSTR text, const int length);	// pøi chybì pamìti bude øetìzec prázdný
#endif //_UNICODE
	~CText();

// sluèovací konstruktury
	CText(const CText& str1, const CText& str2);	// pøi chybì pamìti bude øetìzec prázdný
	CText(LPCTSTR txt, const CText& str);			// pøi chybì pamìti bude øetìzec prázdný
	CText(const CText& str, LPCTSTR txt);			// pøi chybì pamìti bude øetìzec prázdný
	CText(const TCHAR chr, const CText& str);		// pøi chybì pamìti bude øetìzec prázdný
	CText(const CText& str, const TCHAR chr);		// pøi chybì pamìti bude øetìzec prázdný

// statický konstruktor a destruktor
	void Init();
	void _fastcall Init(const CText& src);
	void _fastcall Init(TEXTDATA* data);
	bool _fastcall Init(const TCHAR chr);		// pøi chybì pamìti vrátí FALSE, text není platný
	bool _fastcall Init(LPCTSTR text);			// pøi chybì pamìti vrátí FALSE, text není platný
	bool _fastcall Init(LPCTSTR text, const int length); // pøi chybì pamìti vrátí FALSE, text není platný
#ifdef _UNICODE
	bool _fastcall Init(LPCSTR text);			// pøi chybì pamìti vrátí FALSE, text není platný
	bool _fastcall Init(LPCSTR text, const int length); // pøi chybì pamìti vrátí FALSE, text není platný
#else //_UNICODE
	bool _fastcall Init(LPCWSTR text);			// pøi chybì pamìti vrátí FALSE, text není platný
	bool _fastcall Init(LPCWSTR text, const int length); // pøi chybì pamìti vrátí FALSE, text není platný
#endif //_UNICODE
	void Term();

// poskytnutí adresy dat
	inline TEXTDATA* Data() const { return pData; };
	inline TCHAR* DataData() const { return pData->Data; };

// poskytnutí délky øetìzce
	inline int Length() const { return pData->Length; }

// kontrola, zda je øetìzec prázdný
	inline BOOL IsEmpty() const { return pData->Length == 0; };
	inline BOOL IsNotEmpty() const { return pData->Length != 0; };

// zápis do bufferu (bez koncové nuly)
	void Write(TCHAR* buf) const;
#ifdef _UNICODE
	void Write(LPSTR buf) const;
#else
	void Write(LPWSTR buf) const;
#endif

// zápis do bufferu (s koncovou nulou)
	void WriteNull(TCHAR* buf) const;
#ifdef _UNICODE
	void WriteNull(LPSTR buf) const;
#else
	void WriteNull(LPWSTR buf) const;
#endif


// ------- hledání

// hledání (<0 = nenalezeno, pos=výchozí pozice)
	int _fastcall Find(const CText& str) const;
	int _fastcall Find(const CText& str, int pos) const;
	int _fastcall Find(LPCTSTR txt) const;
	int _fastcall Find(LPCTSTR txt, int pos) const;
	int _fastcall Find(const TCHAR chr) const;
	int _fastcall Find(const TCHAR chr, int pos) const;
	
// hledání znaku od konce øetìzce zpìt (<0 = nenalezeno, pos=výchozí pozice)
	int _fastcall RevFind(const TCHAR chr) const;
	int _fastcall RevFind(const TCHAR chr, int pos) const;

// -------- modifikace textu

// vyprázdnìní øetìzce (pøipojí prázdný øetìzec)
	void Empty();

// konverze malá/velká písmena
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool UpperCase();
	bool LowerCase();

// levá èást øetìzce (pøi chybì pamìti vrátí prázdný øetìzec)
	CText _fastcall Left(int count) const;

// pravá èást øetìzce (pøi chybì pamìti vrátí prázdný øetìzec)
	CText _fastcall Right(int count) const;

// støední èást øetìzce (pøi chybì pamìti vrátí prázdný øetìzec)
	CText _fastcall Mid(int first, int count = 0x7fffffff) const;

// zrušení textu z konce øetìzce
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool Delete(int first, int count = 0x7fffffff);

// zrušení posledního znaku z konce øetìzce
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool DeleteLast();

// odstranìní mezer ze zaèátku/konce øetìzce
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool TrimLeft();
	bool TrimRight();

// korekce výskytu nuly v textu (provádí se ve spoleèném bufferu všech promìnných)
	void KorigNul();

// nastavení èísla jména objektu (1, 2, ...)
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool _fastcall SetNumObj(const int num);

// pøidání textu
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool _fastcall Add(const CText& str);
	bool _fastcall Add(LPCTSTR txt);
	bool _fastcall Add(const TCHAR chr);

// pøidání textu èísla
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool _fastcall AddInt(const int num);
	bool _fastcall AddDouble(const double num);

// souèet textù
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool _fastcall Add(const CText& str1, const CText& str2);
	bool _fastcall Add(LPCTSTR txt, const CText& str);
	bool _fastcall Add(const CText& str, LPCTSTR txt);
	bool _fastcall Add(const TCHAR chr, const CText& str);
	bool _fastcall Add(const CText& str, const TCHAR chr);

// nastavení textu
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
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

// naètení textu z resource (pøi chybì vrací FALSE, pùvodní obsah nezmìnìn)
	bool Load(const int nID);

// naètení ze souboru formátu TXT (FALSE=chyba, obsah nezmìnìn)
//	bool LoadFile(CText jmeno);

// uložení textu do souboru formátu TXT (FALSE=chyba)
//	bool SaveFile(CText jmeno) const;

// ------- pøístup ke znakùm v bufferu

// kopie do vlastního bufferu pøed modifikací
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool CopyWrite();

// kontrola, zda je index znaku platný
	inline BOOL IsValid(const int index) const
		{ return ((DWORD)index < (DWORD)pData->Length); };

	inline BOOL IsNotValid(const int index) const
		{ return ((DWORD)index >= (DWORD)pData->Length); };

// poskytnutí pøístupu ke znaku na pozici (bez kontroly a bez pøivlastnìní bufferu!)
	inline TCHAR& operator[] (int index)
		{ ASSERT(IsValid(index), "Neplatný index znaku"); return pData->Data[index]; }

	inline const TCHAR& operator[] (int index) const
		{ ASSERT(IsValid(index), "Neplatný index znaku"); return pData->Data[index]; }

	inline TCHAR& At(int index)
		{ ASSERT(IsValid(index), "Neplatný index znaku"); return pData->Data[index]; }

	inline const TCHAR& At(int index) const
		{ ASSERT(IsValid(index), "Neplatný index znaku"); return pData->Data[index]; }

// poskytnutí znaku na pozici s kontrolou (pro neplatný znak vrátí 0)
	TCHAR _fastcall Get(const int index) const;

// nastavení znaku na pozici s kontrolou (zajistí pøivlastnìní bufferu)
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool _fastcall Set(const int index, const TCHAR chr);

// poskytnutí posledního znaku øetìzce (pro prázdný øetìzec vrací 0)
	TCHAR _fastcall LastChar() const;

// ------- text oken

// naètení textu okna (pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn)
	bool GetWindowText(const HWND wnd);

// nastavení textu okna
	void _fastcall SetWindowText(const HWND wnd) const;

// naètení textu dialogového prvku (pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn)
	bool GetDialogText(const HWND wnd, int id);

// nastavení textu dialogového prvku
	void _fastcall SetDialogText(const HWND wnd, int id) const;

// -------- adresáøe a soubory

// naètení aktivního adresáøe (pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn)
	bool GetAktDir();

// korekce textu na jméno souboru (vypuštìní zakázaných znakù, 
//	vrací opravenou pozici kurzoru)
//  ignoruje chybu pamìti pøi pøivlastnìní bufferu (pøivlastnit pøedem)
	int FileName(int curs);

// korekce textu na jméno cesty (vypuštìní zakázaných znakù, 
//	vrací opravenou pozici kurzoru)
//  ignoruje chybu pamìti pøi pøivlastnìní bufferu (pøivlastnit pøedem)
	int PathName(int curs);

// -------- konverze èísel

// konverze èísla INT na text (pøi chybì pamìti vrací FALSE)
	bool Int(int num);

// konverze textu na èíslo INT
	friend int Int(LPCTSTR text);

// konverze èísla DWORD na HEX (8 èíslic) (pøi chybì pamìti vrací FALSE)
	bool Hex(DWORD num);

// konverze èísla DOUBLE na text (pøi chybì pamìti vrací false)
	bool Double(double num);

// konverze textu na èíslo DOUBLE
	friend double Double(LPCTSTR txt);

// -------- operátory

// konverze na øetìzec LPCTSTR
	inline operator LPCTSTR() const { return pData->Data; }

// operátory pøiøazení (nepoužívat - nehlásí chybu operace)
	inline const CText& _fastcall operator= (const CText& str) { Set(str); return *this; }
	inline const CText& _fastcall operator= (LPCTSTR txt) { Set(txt); return *this; }
#ifdef _UNICODE
	inline const CText& _fastcall operator= (LPCSTR txt) { Set(txt); return *this; }
#else //_UNICODE
	inline const CText& _fastcall operator= (LPCWSTR txt) { Set(txt); return *this; }
#endif //_UNICODE
	inline const CText& _fastcall operator= (const TCHAR chr) { Set(chr); return *this; };

// operátory pøiètení (nepoužívat - nehlásí chybu operace)
	inline const CText& _fastcall operator+=(const CText& str) { Add(str); return *this; }
	inline const CText& _fastcall operator+=(LPCTSTR txt) { Add(txt); return *this; }
	inline const CText& _fastcall operator+=(const TCHAR chr) { Add(chr); return *this; }

// operátory souètu (nepoužívat - nehlásí chybu operace)
	inline friend CText _fastcall operator+ (const CText& str1, const CText& str2) { return CText(str1, str2); }
	inline friend CText _fastcall operator+ (LPCTSTR txt, const CText& str) { return CText(txt, str); }
	inline friend CText _fastcall operator+ (const CText& str, LPCTSTR txt) { return CText(str, txt); }
	inline friend CText _fastcall operator+ (const TCHAR chr, const CText& str) { return CText(chr, str); }
	inline friend CText _fastcall operator+ (const CText& str, const TCHAR chr) { return CText(str, chr); }

// operátory porovnání
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


