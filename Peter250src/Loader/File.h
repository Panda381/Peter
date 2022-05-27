
/***************************************************************************\
*																			*
*									Soubor									*
*																			*
\***************************************************************************/

class CFile
{
private:
	HANDLE	m_File;				// handle souboru (INVALID_HANDLE_VALUE=neotev�en)
	CString	m_Name;				// jm�no souboru

public:
// konstruktor a destruktor
	CFile();
	~CFile();

// statick� konstruktor a destruktor
//	void Init();
//	void Term();

// odpojen� souboru bez uzav�en� (nap�. konzola)
	inline void Detach() { m_File = INVALID_HANDLE_VALUE; };

// poskytnut�/nastaven� jm�na souboru
	inline void Name(const CString& name) { m_Name = name; };
	inline CString& Name() { return m_Name; };
	inline const CString& Name() const { return m_Name; };

// poskytnut�/nastaven� handle souboru (INVALID_HANDLE_VALUE = nen� otev�en)
	inline void File(const HANDLE file) { m_File = file; };
	inline HANDLE File() const { return m_File; };
	inline operator HANDLE() const { return m_File; }

// otev�en� souboru pro �ten� i z�pis
	BOOL Open();

// otev�en� souboru pro �ten�
	BOOL OpenRead();

// otev�en� souboru pro z�pis
	BOOL OpenWrite();

// otev�en� souboru bez p��stupu (nastaven� vlastnost�)
	BOOL OpenNone();

// otev�en� adres��e (pro nastaven� vlastnost�)
	BOOL OpenDirectory();

// vytvo�en� souboru
	BOOL Create();

// uzav�en� souboru
	BOOL Close();

// vytvo�en�/zru�en� konzoly
	inline BOOL AllocConsole() { return ::AllocConsole(); };
	inline BOOL FreeConsole() { return ::FreeConsole(); };

// otev�en� pro standardn� vstup (neuzav�rat!)
	BOOL OpenInput();

// otev�en� pro standardn� v�stup (neuzav�rat!)
	BOOL OpenOutput();

// otev�en� pro chybov� v�stup (neuzav�rat!)
	BOOL OpenError();

// �ten� ze souboru
	BOOL Read(void* buf, int num);

// z�pis do souboru
	BOOL Write(void* buf, int num);

// zru�en� souboru (mus� b�t uzav�en)
	BOOL Delete();

// poskytnut� velikosti souboru (mus� b�t otev�en, -1 = chyba)
	int Size();

// poskytnut� typu souboru (mus� b�t otev�en)
//		FILE_TYPE_UNKNOWN ... nezn�m�
//		FILE_TYPE_DISK ...... diskov� soubor
//		FILE_TYPE_CHAR ...... znakov� za��zen�
//		FILE_TYPE_PIPE ...... kan�l
	int Type();

// poskytnut�/nastaven� �asu vytvo�en� (mus� b�t otev�en)
	BOOL GetCreationTime(FILETIME* time);
	BOOL SetCreationTime(FILETIME* time);

// poskytnut�/nastaven� �asu posledn�ho �ten� (mus� b�t otev�en)
	BOOL GetLastRead(FILETIME* time);
	BOOL SetLastRead(FILETIME* time);

// poskytnut�/nastaven� �asu posledn�ho z�pisu (mus� b�t otev�en)
	BOOL GetLastWrite(FILETIME* time);
	BOOL SetLastWrite(FILETIME* time);

// poskytnut� atribut� souboru (nemus� b�t otev�en, -1=chyba)
	int Attribute();

// nastaven� atribut� souboru (nemus� b�t otev�en)
	BOOL Attribute(int atr);

// resetov�n� ukazatele v souboru
	BOOL Reset();

// poskytnut� ukazatele v souboru (-1 = chyba)
	int Seek();

// nastaven� ukazatele v souboru
	BOOL Seek(int pos);

// relativn� nastaven� ukazatele v souboru
	BOOL SeekRel(int pos);

// nastaven� ukazatele v souboru na konec
	BOOL End();

// oper�tor p�i�azen�
	const CFile& _fastcall operator= (const CFile& file);
};


/***************************************************************************\
*																			*
*								Datov� buffer								*
*																			*
\***************************************************************************/

class CBuf
{
private:
	BYTE*	m_Adr;			// adresa bufferu v pam�ti
	int		m_Size;			// velikost bufferu
	BYTE*	m_End;			// adresa za koncem bufferu

public:
// konstruktor
	CBuf();

// statick� konstruktor
//	void Init();

// nastaven� adresy a velikosti bufferu
	inline void AdrSize(BYTE* adr, int size)
		{ m_Adr = adr; m_Size = size; m_End = adr + size; };

// poskytnut�/nastaven� adresy bufferu
	inline BYTE* Adr() { return m_Adr; };
	inline void Adr(BYTE* adr) { m_Adr = adr; m_End = adr + m_Size; };

// poskytnut�/nastaven� velikosti bufferu
	inline int Size() { return m_Size; };
	inline void Size(int size) { m_Size = size; m_End = m_Adr + size; };

// poskytnut� adresy za koncem bufferu
	inline BYTE* End() { return m_End; };

// kontrola platnosti adresy
	inline BOOL IsValid(void* adr) { return ((adr >= m_Adr) && (adr < m_End)); };
	inline BOOL IsNotValid(void* adr) { return ((adr < m_Adr) || (adr >= m_End)); };

// kontrola platnosti bloku
	BOOL IsValid(void* adr, int size);
	BOOL IsNotValid(void* adr, int size);

// adresa NT z�hlav� souboru EXE (NULL=nen�)
	IMAGE_NT_HEADERS* NTHeader();

// nalezen� NT sekce souboru EXE (NULL=nen�)
	IMAGE_SECTION_HEADER* NTSection(IMAGE_NT_HEADERS* hdr, char* name);

// nalezen� resource dat souboru EXE (NULL=nen�)
//	BYTE* NTResource();
};


/***************************************************************************\
*																			*
*					Pam�ov� mapovan� soubor (jen pro �ten�)				*
*																			*
\***************************************************************************/

class CFileMap : public CBuf, private CFile
{
private:
	HANDLE		m_Map;		// handle mapov�n� (NULL=neotev�eno)

// nastaven� velikosti bufferu
	inline void Size(int size) { CBuf::Size(size); };

// nastaven� adresy bufferu
	inline void Adr(BYTE* adr) { CBuf::Adr(adr); };

public:
// konstruktor a destruktor
	CFileMap();
	~CFileMap();

// statick� konstruktor a destruktor
//	void Init();
//	void Term();

// poskytnut� velikosti souboru
	inline int Size() { return CBuf::Size(); };

// poskytnut� adresy v pam�ti (NULL = nen� otev�eno)
	inline BYTE* Adr() { return CBuf::Adr(); };

// poskytnut� adresy za pam�ov�m oknem (NULL = nen� otev�eno)
	inline BYTE* End() { return CBuf::End(); };

// kontrola platnosti adresy
	inline BOOL IsValid(void* adr) { return CBuf::IsValid(adr); };
	inline BOOL IsNotValid(void* adr) { return CBuf::IsNotValid(adr); };

// kontrola platnosti bloku
	inline BOOL IsValid(void* adr, int size) { return CBuf::IsValid(adr, size); };
	inline BOOL IsNotValid(void* adr, int size) { return CBuf::IsNotValid(adr, size); };

// otev�en�
	BOOL Open(const CString& name);

// uzav�en� (p�i destrukci uzav�r� automaticky)
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
	HINSTANCE	m_Instance;	// instance s daty (NULL=implicitn�)

// nastaven� velikosti bufferu
	inline void Size(int size) { CBuf::Size(size); };

// nastaven� adresy bufferu
	inline void Adr(BYTE* adr) { CBuf::Adr(adr); };

public:
// konstruktor a destruktor
	CResource();
	~CResource();

// statick� konstruktor a destruktor
//	void Init();
//	void Term();

// poskytnut� velikosti souboru
	inline int Size() { return CBuf::Size(); };

// poskytnut� adresy v pam�ti (NULL = nen� otev�eno)
	inline BYTE* Adr() { return CBuf::Adr(); };

// poskytnut� adresy za pam�ov�m oknem (NULL = nen� otev�eno)
	inline BYTE* End() { return CBuf::End(); };

// kontrola platnosti adresy
	inline BOOL IsValid(void* adr) { return CBuf::IsValid(adr); };
	inline BOOL IsNotValid(void* adr) { return CBuf::IsNotValid(adr); };

// kontrola platnosti bloku
	inline BOOL IsValid(void* adr, int size) { return CBuf::IsValid(adr, size); };
	inline BOOL IsNotValid(void* adr, int size) { return CBuf::IsNotValid(adr, size); };

// otev�en� souboru s resource
	BOOL OpenFile(LPCTSTR name);

// uzav�en� souboru s resource (p�i destrukci se uzav�r� automaticky)
	void CloseFile();

// otev�en� resource
	BOOL Open(int id, LPCTSTR typ);

// otev�en� kopie dat (pro mo�nost z�pisu, buffer je nutno po pou�it� zru�it)
	BOOL OpenCopy(int id, LPCTSTR typ);

// zru�en� bufferu s kopi� resource dat
	void CloseCopy();

// uzav�en� resource (p�i destrukci uzav�r� automaticky)
	void Close();
};
