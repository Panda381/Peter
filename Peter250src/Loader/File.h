
/***************************************************************************\
*																			*
*									Soubor									*
*																			*
\***************************************************************************/

class CFile
{
private:
	HANDLE	m_File;				// handle souboru (INVALID_HANDLE_VALUE=neotevøen)
	CString	m_Name;				// jméno souboru

public:
// konstruktor a destruktor
	CFile();
	~CFile();

// statickı konstruktor a destruktor
//	void Init();
//	void Term();

// odpojení souboru bez uzavøení (napø. konzola)
	inline void Detach() { m_File = INVALID_HANDLE_VALUE; };

// poskytnutí/nastavení jména souboru
	inline void Name(const CString& name) { m_Name = name; };
	inline CString& Name() { return m_Name; };
	inline const CString& Name() const { return m_Name; };

// poskytnutí/nastavení handle souboru (INVALID_HANDLE_VALUE = není otevøen)
	inline void File(const HANDLE file) { m_File = file; };
	inline HANDLE File() const { return m_File; };
	inline operator HANDLE() const { return m_File; }

// otevøení souboru pro ètení i zápis
	BOOL Open();

// otevøení souboru pro ètení
	BOOL OpenRead();

// otevøení souboru pro zápis
	BOOL OpenWrite();

// otevøení souboru bez pøístupu (nastavení vlastností)
	BOOL OpenNone();

// otevøení adresáøe (pro nastavení vlastností)
	BOOL OpenDirectory();

// vytvoøení souboru
	BOOL Create();

// uzavøení souboru
	BOOL Close();

// vytvoøení/zrušení konzoly
	inline BOOL AllocConsole() { return ::AllocConsole(); };
	inline BOOL FreeConsole() { return ::FreeConsole(); };

// otevøení pro standardní vstup (neuzavírat!)
	BOOL OpenInput();

// otevøení pro standardní vıstup (neuzavírat!)
	BOOL OpenOutput();

// otevøení pro chybovı vıstup (neuzavírat!)
	BOOL OpenError();

// ètení ze souboru
	BOOL Read(void* buf, int num);

// zápis do souboru
	BOOL Write(void* buf, int num);

// zrušení souboru (musí bıt uzavøen)
	BOOL Delete();

// poskytnutí velikosti souboru (musí bıt otevøen, -1 = chyba)
	int Size();

// poskytnutí typu souboru (musí bıt otevøen)
//		FILE_TYPE_UNKNOWN ... neznámı
//		FILE_TYPE_DISK ...... diskovı soubor
//		FILE_TYPE_CHAR ...... znakové zaøízení
//		FILE_TYPE_PIPE ...... kanál
	int Type();

// poskytnutí/nastavení èasu vytvoøení (musí bıt otevøen)
	BOOL GetCreationTime(FILETIME* time);
	BOOL SetCreationTime(FILETIME* time);

// poskytnutí/nastavení èasu posledního ètení (musí bıt otevøen)
	BOOL GetLastRead(FILETIME* time);
	BOOL SetLastRead(FILETIME* time);

// poskytnutí/nastavení èasu posledního zápisu (musí bıt otevøen)
	BOOL GetLastWrite(FILETIME* time);
	BOOL SetLastWrite(FILETIME* time);

// poskytnutí atributù souboru (nemusí bıt otevøen, -1=chyba)
	int Attribute();

// nastavení atributù souboru (nemusí bıt otevøen)
	BOOL Attribute(int atr);

// resetování ukazatele v souboru
	BOOL Reset();

// poskytnutí ukazatele v souboru (-1 = chyba)
	int Seek();

// nastavení ukazatele v souboru
	BOOL Seek(int pos);

// relativní nastavení ukazatele v souboru
	BOOL SeekRel(int pos);

// nastavení ukazatele v souboru na konec
	BOOL End();

// operátor pøiøazení
	const CFile& _fastcall operator= (const CFile& file);
};


/***************************************************************************\
*																			*
*								Datovı buffer								*
*																			*
\***************************************************************************/

class CBuf
{
private:
	BYTE*	m_Adr;			// adresa bufferu v pamìti
	int		m_Size;			// velikost bufferu
	BYTE*	m_End;			// adresa za koncem bufferu

public:
// konstruktor
	CBuf();

// statickı konstruktor
//	void Init();

// nastavení adresy a velikosti bufferu
	inline void AdrSize(BYTE* adr, int size)
		{ m_Adr = adr; m_Size = size; m_End = adr + size; };

// poskytnutí/nastavení adresy bufferu
	inline BYTE* Adr() { return m_Adr; };
	inline void Adr(BYTE* adr) { m_Adr = adr; m_End = adr + m_Size; };

// poskytnutí/nastavení velikosti bufferu
	inline int Size() { return m_Size; };
	inline void Size(int size) { m_Size = size; m_End = m_Adr + size; };

// poskytnutí adresy za koncem bufferu
	inline BYTE* End() { return m_End; };

// kontrola platnosti adresy
	inline BOOL IsValid(void* adr) { return ((adr >= m_Adr) && (adr < m_End)); };
	inline BOOL IsNotValid(void* adr) { return ((adr < m_Adr) || (adr >= m_End)); };

// kontrola platnosti bloku
	BOOL IsValid(void* adr, int size);
	BOOL IsNotValid(void* adr, int size);

// adresa NT záhlaví souboru EXE (NULL=není)
	IMAGE_NT_HEADERS* NTHeader();

// nalezení NT sekce souboru EXE (NULL=není)
	IMAGE_SECTION_HEADER* NTSection(IMAGE_NT_HEADERS* hdr, char* name);

// nalezení resource dat souboru EXE (NULL=není)
//	BYTE* NTResource();
};


/***************************************************************************\
*																			*
*					Pamìovì mapovanı soubor (jen pro ètení)				*
*																			*
\***************************************************************************/

class CFileMap : public CBuf, private CFile
{
private:
	HANDLE		m_Map;		// handle mapování (NULL=neotevøeno)

// nastavení velikosti bufferu
	inline void Size(int size) { CBuf::Size(size); };

// nastavení adresy bufferu
	inline void Adr(BYTE* adr) { CBuf::Adr(adr); };

public:
// konstruktor a destruktor
	CFileMap();
	~CFileMap();

// statickı konstruktor a destruktor
//	void Init();
//	void Term();

// poskytnutí velikosti souboru
	inline int Size() { return CBuf::Size(); };

// poskytnutí adresy v pamìti (NULL = není otevøeno)
	inline BYTE* Adr() { return CBuf::Adr(); };

// poskytnutí adresy za pamìovım oknem (NULL = není otevøeno)
	inline BYTE* End() { return CBuf::End(); };

// kontrola platnosti adresy
	inline BOOL IsValid(void* adr) { return CBuf::IsValid(adr); };
	inline BOOL IsNotValid(void* adr) { return CBuf::IsNotValid(adr); };

// kontrola platnosti bloku
	inline BOOL IsValid(void* adr, int size) { return CBuf::IsValid(adr, size); };
	inline BOOL IsNotValid(void* adr, int size) { return CBuf::IsNotValid(adr, size); };

// otevøení
	BOOL Open(const CString& name);

// uzavøení (pøi destrukci uzavírá automaticky)
	void Close();
};


/***************************************************************************\
*																			*
*								Resource data								*
*																			*
\***************************************************************************/

class CResource : public CBuf
{
private:
	HINSTANCE	m_Instance;	// instance s daty (NULL=implicitní)

// nastavení velikosti bufferu
	inline void Size(int size) { CBuf::Size(size); };

// nastavení adresy bufferu
	inline void Adr(BYTE* adr) { CBuf::Adr(adr); };

public:
// konstruktor a destruktor
	CResource();
	~CResource();

// statickı konstruktor a destruktor
//	void Init();
//	void Term();

// poskytnutí velikosti souboru
	inline int Size() { return CBuf::Size(); };

// poskytnutí adresy v pamìti (NULL = není otevøeno)
	inline BYTE* Adr() { return CBuf::Adr(); };

// poskytnutí adresy za pamìovım oknem (NULL = není otevøeno)
	inline BYTE* End() { return CBuf::End(); };

// kontrola platnosti adresy
	inline BOOL IsValid(void* adr) { return CBuf::IsValid(adr); };
	inline BOOL IsNotValid(void* adr) { return CBuf::IsNotValid(adr); };

// kontrola platnosti bloku
	inline BOOL IsValid(void* adr, int size) { return CBuf::IsValid(adr, size); };
	inline BOOL IsNotValid(void* adr, int size) { return CBuf::IsNotValid(adr, size); };

// otevøení souboru s resource
	BOOL OpenFile(LPCTSTR name);

// uzavøení souboru s resource (pøi destrukci se uzavírá automaticky)
	void CloseFile();

// otevøení resource
	BOOL Open(int id, LPCTSTR typ);

// otevøení kopie dat (pro monost zápisu, buffer je nutno po pouití zrušit)
	BOOL OpenCopy(int id, LPCTSTR typ);

// zrušení bufferu s kopií resource dat
	void CloseCopy();

// uzavøení resource (pøi destrukci uzavírá automaticky)
	void Close();
};
