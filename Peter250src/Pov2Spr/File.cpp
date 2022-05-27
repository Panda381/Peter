
#include "Main.h"

/***************************************************************************\
*																			*
*								Obsluha souborù								*
*																			*
\***************************************************************************/


//////////////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CFile::CFile()
{
	m_File = INVALID_HANDLE_VALUE;
}

//////////////////////////////////////////////////////////////////////////////
// statický konstruktor a destruktor

void CFile::Init()
{
	m_File = INVALID_HANDLE_VALUE;
	m_Name.Init();
}

void CFile::Term()
{
	m_Name.Term();
}


//////////////////////////////////////////////////////////////////////////////
// otevøení souboru pro ètení i zápis

BOOL CFile::Open()
{
	ASSERT(m_File == INVALID_HANDLE_VALUE);
	m_File = ::CreateFile(m_Name, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	return (m_File != INVALID_HANDLE_VALUE);
}


//////////////////////////////////////////////////////////////////////////////
// otevøení souboru pro ètení

BOOL CFile::OpenRead()
{
	ASSERT(m_File == INVALID_HANDLE_VALUE);
	m_File = ::CreateFile(m_Name, GENERIC_READ, FILE_SHARE_READ |
		FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	return (m_File != INVALID_HANDLE_VALUE);
}


//////////////////////////////////////////////////////////////////////////////
// otevøení souboru pro zápis

BOOL CFile::OpenWrite()
{
	ASSERT(m_File == INVALID_HANDLE_VALUE);
	m_File = ::CreateFile(m_Name, GENERIC_WRITE, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	return (m_File != INVALID_HANDLE_VALUE);
}


//////////////////////////////////////////////////////////////////////////////
// otevøení souboru bez pøístupu (nastavení vlastností)

BOOL CFile::OpenNone()
{
	ASSERT(m_File == INVALID_HANDLE_VALUE);
	m_File = ::CreateFile(m_Name, 0, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	return (m_File != INVALID_HANDLE_VALUE);
}


//////////////////////////////////////////////////////////////////////////////
// otevøení adresáøe (pro nastavení vlastností)

BOOL CFile::OpenDirectory()
{
	ASSERT(m_File == INVALID_HANDLE_VALUE);
	m_File = ::CreateFile(m_Name, 0, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	return (m_File != INVALID_HANDLE_VALUE);
}


//////////////////////////////////////////////////////////////////////////////
// vytvoøení souboru

BOOL CFile::Create()
{
	ASSERT(m_File == INVALID_HANDLE_VALUE);
	m_File = ::CreateFile(m_Name, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	return (m_File != INVALID_HANDLE_VALUE);
}


//////////////////////////////////////////////////////////////////////////////
// otevøení pro standardní vstup (neuzavírat!)

BOOL CFile::OpenInput()
{
	ASSERT(m_File == INVALID_HANDLE_VALUE);
	m_File = ::GetStdHandle(STD_INPUT_HANDLE);
	return (m_File != INVALID_HANDLE_VALUE);
}


//////////////////////////////////////////////////////////////////////////////
// otevøení pro standardní výstup (neuzavírat!)

BOOL CFile::OpenOutput()
{
	ASSERT(m_File == INVALID_HANDLE_VALUE);
	m_File = ::GetStdHandle(STD_OUTPUT_HANDLE);
	return (m_File != INVALID_HANDLE_VALUE);
}


//////////////////////////////////////////////////////////////////////////////
// otevøení pro chybový výstup (neuzavírat!)

BOOL CFile::OpenError()
{
	ASSERT(m_File == INVALID_HANDLE_VALUE);
	m_File = ::GetStdHandle(STD_ERROR_HANDLE);
	return (m_File != INVALID_HANDLE_VALUE);
}


//////////////////////////////////////////////////////////////////////////////
// uzavøení souboru

BOOL CFile::Close()
{
	if (m_File != INVALID_HANDLE_VALUE)
	{
		if (!::CloseHandle(m_File))
		{
			return FALSE;
		}
		m_File = INVALID_HANDLE_VALUE;
	}
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////////
// ètení ze souboru

BOOL CFile::Read(void* buf, int num)
{
	ASSERT(m_File != INVALID_HANDLE_VALUE);
	DWORD read;
	return (::ReadFile(m_File, buf, num, &read, NULL)
		&& (read == (DWORD)num));
}


//////////////////////////////////////////////////////////////////////////////
// zápis do souboru

BOOL CFile::Write(void* buf, int num)
{
	ASSERT(m_File != INVALID_HANDLE_VALUE);
	DWORD write;
	return (::WriteFile(m_File, buf, num, &write, NULL)
		&& (write == (DWORD)num));
}


//////////////////////////////////////////////////////////////////////////////
// zrušení souboru (musí být uzavøen)

BOOL CFile::Delete()
{
	ASSERT(m_File == INVALID_HANDLE_VALUE);
	return ::DeleteFile(m_Name);
}


//////////////////////////////////////////////////////////////////////////////
// poskytnutí velikosti souboru (musí být otevøen, -1 = chyba)

int CFile::Size()
{
	ASSERT(m_File != INVALID_HANDLE_VALUE);
	return ::GetFileSize(m_File, NULL);
}


//////////////////////////////////////////////////////////////////////////////
// poskytnutí typu souboru (musí být otevøen)
//		FILE_TYPE_UNKNOWN ... neznámý
//		FILE_TYPE_DISK ...... diskový soubor
//		FILE_TYPE_CHAR ...... znakové zaøízení
//		FILE_TYPE_PIPE ...... kanál

int CFile::Type()
{
	ASSERT(m_File != INVALID_HANDLE_VALUE);
	return ::GetFileType(m_File);
}


//////////////////////////////////////////////////////////////////////////////
// poskytnutí/nastavení èasu vytvoøení (musí být otevøen)

BOOL CFile::GetCreationTime(FILETIME* time)
{
	ASSERT(m_File != INVALID_HANDLE_VALUE);
	return ::GetFileTime(m_File, time, NULL, NULL);
}

BOOL CFile::SetCreationTime(FILETIME* time)
{
	ASSERT(m_File != INVALID_HANDLE_VALUE);
	return ::SetFileTime(m_File, time, NULL, NULL);
}


//////////////////////////////////////////////////////////////////////////////
// poskytnutí/nastavení èasu posledního ètení (musí být otevøen)

BOOL CFile::GetLastRead(FILETIME* time)
{
	ASSERT(m_File != INVALID_HANDLE_VALUE);
	return ::GetFileTime(m_File, NULL, time, NULL);
}

BOOL CFile::SetLastRead(FILETIME* time)
{
	ASSERT(m_File != INVALID_HANDLE_VALUE);
	return ::SetFileTime(m_File, NULL, time, NULL);
}


//////////////////////////////////////////////////////////////////////////////
// poskytnutí/nastavení èasu posledního zápisu (musí být otevøen)

BOOL CFile::GetLastWrite(FILETIME* time)
{
	ASSERT(m_File != INVALID_HANDLE_VALUE);
	return ::GetFileTime(m_File, NULL, NULL, time);
}

BOOL CFile::SetLastWrite(FILETIME* time)
{
	ASSERT(m_File != INVALID_HANDLE_VALUE);
	return ::SetFileTime(m_File, NULL, NULL, time);
}


//////////////////////////////////////////////////////////////////////////////
// poskytnutí atributù souboru (nemusí být otevøen, -1=chyba)

int CFile::Attribute()
{
	return ::GetFileAttributes(m_Name);
}


//////////////////////////////////////////////////////////////////////////////
// nastavení atributù souboru (nemusí být otevøen)

BOOL CFile::Attribute(int atr)
{
	return ::SetFileAttributes(m_Name, atr & 
		~(FILE_ATTRIBUTE_DIRECTORY | 
		FILE_ATTRIBUTE_COMPRESSED |
		FILE_ATTRIBUTE_OFFLINE |
		FILE_ATTRIBUTE_TEMPORARY));
}


//////////////////////////////////////////////////////////////////////////////
// resetování ukazatele v souboru

BOOL CFile::Reset()
{
	ASSERT(m_File != INVALID_HANDLE_VALUE);
	return ::SetFilePointer(m_File, 0, NULL, FILE_BEGIN);
}


//////////////////////////////////////////////////////////////////////////////
// poskytnutí ukazatele v souboru (-1 = chyba)

int CFile::Seek()
{
	ASSERT(m_File != INVALID_HANDLE_VALUE);
	return ::SetFilePointer(m_File, 0, NULL, FILE_CURRENT);
}


//////////////////////////////////////////////////////////////////////////////
// nastavení ukazatele v souboru

BOOL CFile::Seek(int pos)
{
	ASSERT(m_File != INVALID_HANDLE_VALUE);
	return ((int)::SetFilePointer(m_File, pos, NULL, FILE_BEGIN) != -1);
}


//////////////////////////////////////////////////////////////////////////////
// relativní nastavení ukazatele v souboru

BOOL CFile::SeekRel(int pos)
{
	ASSERT(m_File != INVALID_HANDLE_VALUE);
	return ((int)::SetFilePointer(m_File, pos, NULL, FILE_CURRENT) != -1);
}


//////////////////////////////////////////////////////////////////////////////
// nastavení ukazatele v souboru na konec

BOOL CFile::End()
{
	ASSERT(m_File != INVALID_HANDLE_VALUE);
	return ((int)::SetFilePointer(m_File, 0, NULL, FILE_END) != -1);
}


//////////////////////////////////////////////////////////////////////////////
// operátor pøiøazení

const CFile& _fastcall CFile::operator= (const CFile& file)
{
	ASSERT(m_File == INVALID_HANDLE_VALUE);
	m_File = file.m_File;
	m_Name = file.m_Name;
	return *this;
}


/***************************************************************************\
*																			*
*								Datový buffer								*
*																			*
\***************************************************************************/

//////////////////////////////////////////////////////////////////////////////
// konstruktor

CBuf::CBuf()
{
	m_Adr = NULL;				// adresa bufferu v pamìti
	m_End = NULL;				// adresa za koncem bufferu
	m_Size = 0;					// velikost bufferu
}


//////////////////////////////////////////////////////////////////////////////
// statický konstruktor

void CBuf::Init()
{
	m_Adr = NULL;				// adresa bufferu v pamìti
	m_End = NULL;				// adresa za koncem bufferu
	m_Size = 0;					// velikost bufferu
}


//////////////////////////////////////////////////////////////////////////////
// kontrola platnosti bloku

BOOL CBuf::IsValid(void* adr, int size)
{
	return (((DWORD)size <= (DWORD)m_Size) &&
			IsValid(adr) && IsValid((BYTE*)adr + size - 1));
}


BOOL CBuf::IsNotValid(void* adr, int size)
{
	return (((DWORD)size > (DWORD)m_Size) ||
			IsNotValid(adr) || IsNotValid((BYTE*)adr + size - 1));
}


//////////////////////////////////////////////////////////////////////////////
// adresa NT záhlaví souboru EXE (NULL=není)

IMAGE_NT_HEADERS* CBuf::NTHeader()
{
// kontrola DOS záhlaví
	IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)Adr();	// adresa DOS záhlaví
	if (IsNotValid(dos, sizeof(IMAGE_DOS_HEADER)) ||	// velikost pro DOS záhlaví
		(dos->e_magic != IMAGE_DOS_SIGNATURE))			// identifikátor
		return NULL;

// adresa NT záhlaví
	int off = dos->e_lfanew;						// offset NT záhlaví
	IMAGE_NT_HEADERS* hdr = (IMAGE_NT_HEADERS*)(Adr() + off); // adresa NT záhlaví

// kontrola NT záhlaví
	if ((off < 0x60) ||								// min. offset NT záhlaví
		(off >= 0x1000) ||							// max. offset NT záhlaví
		(IsNotValid(hdr, sizeof(IMAGE_NT_HEADERS))) ||	// kontrola velikosti bufferu
		(hdr->Signature != IMAGE_NT_SIGNATURE))		// identifikátor
		return NULL;

// kontrola velikosti záhlaví vèetnì sekcí
	off = hdr->FileHeader.SizeOfOptionalHeader;		// velikost volitelné sekce
	int sekce = hdr->FileHeader.NumberOfSections;	// poèet sekcí
	if ((sekce < 1) ||								// minimálnì sekcí
		(sekce > 1000) ||							// maximálnì sekcí
		(off < 50) ||								// minimální velikost záhlaví
		(off > 10000) ||							// maximální velikost záhlaví
		(IsNotValid(hdr, sizeof(DWORD) + 
			sizeof(IMAGE_FILE_HEADER) + off + 
			sekce*IMAGE_SIZEOF_SECTION_HEADER)))	// kontrola velikosti sekcí
		return NULL;

// adresa NT záhlaví
	return hdr;
}


//////////////////////////////////////////////////////////////////////////////
// nalezení NT sekce souboru EXE (NULL=není)

IMAGE_SECTION_HEADER* CBuf::NTSection(IMAGE_NT_HEADERS* hdr, char* name)
{
// poèet sekcí
	int num = hdr->FileHeader.NumberOfSections;

// adresa první sekce
	IMAGE_SECTION_HEADER* sec = (IMAGE_SECTION_HEADER*)
		((BYTE*)(&hdr->OptionalHeader) + hdr->FileHeader.SizeOfOptionalHeader);

// cyklus pøes všechny sekce
	for (; num > 0; num--)
	{
// adresa jmen
		char* src = name;
		char* dst = (char*)sec->Name;

// porovnání jména sekce
		int i = 0;
		while (*src == *dst)
		{
			if (*src == 0)
			{
				return sec;
			}
			src++;
			dst++;
			i++;
			if (i == 8)
			{
				return sec;
			}
		}

// adresa další sekce
		sec++;
	}

// sekce nenalezena
	return NULL;
}


/*
//////////////////////////////////////////////////////////////////////////////
// nalezení resource dat souboru EXE (NULL=není)

BYTE* CBuf::NTResource()
{
// pøíprava NT záhlaví
	IMAGE_NT_HEADERS* hdr = NTHeader();
	if (hdr == NULL) return NULL;

// pøíprava sekce resource
	IMAGE_SECTION_HEADER* sec = NTSection(hdr, ".rsrc");
	if (sec == NULL) return NULL;

// adresa resource dat
	BYTE* res = Adr() + sec->PointerToRawData;
	if (IsNotValid(res, sec->SizeOfRawData)) return NULL;

// adresa dat
	return res;
}
*/


//////////////////////////////////////////////////////////////////////////////
// nalezení implicitní ikony v NT souboru (NULL=není)

#define DIR			IMAGE_RESOURCE_DIRECTORY
#define DIRSIZE		sizeof(DIR)
#define ENTRY		IMAGE_RESOURCE_DIRECTORY_ENTRY
#define ENTRYSIZE	sizeof(ENTRY)

BITMAPINFO* CBuf::NTIcon()
{
// lokální promìnné
	IMAGE_NT_HEADERS* hdr;				// NT záhlaví
	IMAGE_SECTION_HEADER* sec;			// sekce resource
	BYTE* res;							// adresa dat resource
	DIR* dir;							// adresa adresáøe resource
	int num;							// poèet položek v typovém adresáøi
	ENTRY* entry;						// položka adresáøe
	BITMAPINFO* bmp;					// adresa ikony
	IMAGE_RESOURCE_DATA_ENTRY* data;	// data ikony

// pøíprava NT záhlaví
	hdr = NTHeader();
	if (hdr == NULL) goto CHYBA;

// pøíprava sekce resource
	sec = NTSection(hdr, ".rsrc");
	if (sec == NULL) goto CHYBA;

// adresa resource dat
	res = Adr() + sec->PointerToRawData;
	if (IsNotValid(res, sec->SizeOfRawData)) goto CHYBA;

// adresa typového adresáøe resource
	dir = (DIR*)res;
	if (IsNotValid(dir, DIRSIZE)) goto CHYBA;

// poèet položek v typovém adresáøi resource
	num = dir->NumberOfNamedEntries + dir->NumberOfIdEntries;

// adresa první položky typového adresáøe
	entry = (ENTRY*)(dir + 1);

// cyklus pøes položky typového adresáøe
	for (; num > 0; num--)
	{
// kontrola platnosti adresy položky
		if (IsNotValid(entry, ENTRYSIZE)) goto CHYBA;

// kontrola, zda je položka adresáø typu ikon
		if (((entry->Name & IMAGE_RESOURCE_NAME_IS_STRING) == 0) &&
			(entry->Id == (WORD)RT_ICON) &&
			(entry->OffsetToData & IMAGE_RESOURCE_DATA_IS_DIRECTORY))
		{

// pøíprava jmenného adresáøe ikon
			dir = (DIR*)(res + entry->OffsetToDirectory);
			if (IsNotValid(dir, DIRSIZE)) goto CHYBA;
			entry = (ENTRY*)(dir + 1);
			if (IsNotValid(entry, ENTRYSIZE)) goto CHYBA;

// pøíprava jazykového adresáøe ikony
			dir = (DIR*)(res + entry->OffsetToDirectory);
			if (IsNotValid(dir, DIRSIZE)) goto CHYBA;
			entry = (ENTRY*)(dir + 1);
			if (IsNotValid(entry, ENTRYSIZE)) goto CHYBA;

// pøíprava datové položky první ikony
			data = (IMAGE_RESOURCE_DATA_ENTRY*)(res + entry->OffsetToData);
			if (IsNotValid(data, sizeof(IMAGE_RESOURCE_DATA_ENTRY))) goto CHYBA;

// pøíprava ukazatele dat na data ikony
			bmp = (BITMAPINFO*)(res + data->OffsetToData - sec->VirtualAddress);
			if (IsNotValid(bmp, sizeof(BITMAPINFOHEADER) + 
				256*sizeof(RGBQUAD) + ICONSIZE + ICONSIZE/8)) goto CHYBA;

// kontrola BMP záhlaví
			if ((bmp->bmiHeader.biSize != sizeof(BITMAPINFOHEADER)) ||
				(bmp->bmiHeader.biWidth != ICONWIDTH) ||
				(bmp->bmiHeader.biHeight != ICONHEIGHT*2) ||
				(bmp->bmiHeader.biPlanes != 1) ||
				(bmp->bmiHeader.biBitCount != 8))
				goto CHYBA;

// vše je OK
			return bmp;
		}

// adresa další položky adresáøe
		entry++;
	}

// chyba
CHYBA:
	return NULL;
}


/***************************************************************************\
*																			*
*					Pamìovì mapovaný soubor (jen pro ètení)				*
*																			*
\***************************************************************************/

//////////////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CFileMap::CFileMap()
{
	m_Map = NULL;				// není mapování
}

CFileMap::~CFileMap()
{
	Close();
}


//////////////////////////////////////////////////////////////////////////////
// statický konstruktor a destruktor

void CFileMap::Init()
{
	CBuf::Init();				// inicializace bufferu
	CFile::Init();				// inicializace souboru
	m_Map = NULL;				// není mapování
}

void CFileMap::Term()
{
	CFile::Term();
	Close();
}


//////////////////////////////////////////////////////////////////////////////
// otevøení

BOOL CFileMap::Open(const CString& name)
{
// otevøení mapovaného souboru
	CFile::Name(name);
	if (CFile::OpenRead())
	{

// zjištìní velikosti souboru
		ASSERT(Size() == 0);
		Size(CFile::Size());
		if (Size() != -1)
		{

// vytvoøení mapování
			ASSERT(m_Map == NULL);
			m_Map = ::CreateFileMapping(CFile::File(), NULL, PAGE_READONLY, 0, Size(), NULL);
			if (m_Map != NULL)
			{

// namapování do okna v pamìti
				ASSERT(Adr() == NULL);
				Adr((BYTE*)::MapViewOfFile(m_Map, FILE_MAP_READ, 0, 0, Size()));
				if (Adr() != NULL)
				{

// operace je OK
					return TRUE;
				}

// uzavøení mapování pøi chybì
				::CloseHandle(m_Map);
				m_Map = NULL;
			}
		}

// uzavøení souboru pøi chybì
		Size(0);
		CFile::Close();
	}

// chybový návrat
	return FALSE;
}


//////////////////////////////////////////////////////////////////////////////
// uzavøení

void CFileMap::Close()
{
// uvolnìní okna v pamìti
	ASSERT(Adr() != NULL);
	if (Adr() != NULL)
	{
		::UnmapViewOfFile(Adr());
		Adr(NULL);
	}

// uzavøení mapování
	ASSERT(m_Map != NULL);
	if (m_Map != NULL)
	{
		::CloseHandle(m_Map);
		m_Map = NULL;
	}

// uzavøení souboru
	Size(0);
	CFile::Close();
}


/***************************************************************************\
*																			*
*								Resource data								*
*																			*
\***************************************************************************/

//////////////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CResource::CResource()
{
	m_Instance = NULL;			// implicitní instance
}

CResource::~CResource()
{
	CloseFile();				// uzavøení souboru
	Close();					// uzavøení resource
}


//////////////////////////////////////////////////////////////////////////////
// statický konstruktor a destruktor

void CResource::Init()
{
	CBuf::Init();				// inicializace bufferu
	m_Instance = NULL;			// implicitní instance
}

void CResource::Term()
{
	CloseFile();				// uzavøení souboru
	Close();					// uzavøení resource
}


//////////////////////////////////////////////////////////////////////////////
// naètení souboru s resource

BOOL CResource::OpenFile(LPCTSTR name)
{
	ASSERT(m_Instance == NULL);
	m_Instance = ::LoadLibrary(name);
	return (m_Instance != NULL);
}


//////////////////////////////////////////////////////////////////////////////
// uzavøení souboru s resource

void CResource::CloseFile()
{
	if (m_Instance)
	{
		::FreeLibrary(m_Instance);
		m_Instance = NULL;
	}
}


//////////////////////////////////////////////////////////////////////////////
// otevøení resource

BOOL CResource::Open(int id, LPCTSTR typ)
{
// nalezení resource
	HRSRC res = ::FindResource(m_Instance, MAKEINTRESOURCE(id), typ);
	if (res)
	{

// zjištìní velikosti dat
		ASSERT(Size() == 0);
		Size(::SizeofResource(m_Instance, res));
		if (Size() > 0)
		{

// naètení dat do pamìti
			HGLOBAL glob = ::LoadResource(m_Instance, res);
			if (glob)
			{

// uzamknutí dat
				ASSERT(Adr() == NULL);
				Adr((BYTE*)::LockResource(glob));
				if (Adr())
				{

// operace je OK
					return TRUE;
				}
			}
		}
		Size(0);
	}

// chybový návrat
	return FALSE;
}


//////////////////////////////////////////////////////////////////////////////
// otevøení kopie dat (pro možnost zápisu, buffer je nutno po použití zrušit)

BOOL CResource::OpenCopy(int id, LPCTSTR typ)
{
	if (!Open(id, typ)) return FALSE;

	BYTE* buf = (BYTE*)MemGet(Size());
	MemCopy(buf, Adr(), Size());
	Adr(buf);
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////////
// zrušení bufferu s kopií resource dat

void CResource::CloseCopy()
{
	MemFree(Adr());
	Close();
}


//////////////////////////////////////////////////////////////////////////////
// uzavøení (rezervováno - zatím není nutné provádìt)

void CResource::Close()
{
	Adr(0);
	Size(0);
}
