
#include "Main.h"

/***************************************************************************\
*																			*
*								Obsluha soubor�								*
*																			*
\***************************************************************************/


//////////////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CFile::CFile()
{
	m_File = INVALID_HANDLE_VALUE;
}

//////////////////////////////////////////////////////////////////////////////
// statick� konstruktor a destruktor

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
// otev�en� souboru pro �ten� i z�pis

BOOL CFile::Open()
{
	ASSERT(m_File == INVALID_HANDLE_VALUE);
	m_File = ::CreateFile(m_Name, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	return (m_File != INVALID_HANDLE_VALUE);
}


//////////////////////////////////////////////////////////////////////////////
// otev�en� souboru pro �ten�

BOOL CFile::OpenRead()
{
	ASSERT(m_File == INVALID_HANDLE_VALUE);
	m_File = ::CreateFile(m_Name, GENERIC_READ, FILE_SHARE_READ |
		FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	return (m_File != INVALID_HANDLE_VALUE);
}


//////////////////////////////////////////////////////////////////////////////
// otev�en� souboru pro z�pis

BOOL CFile::OpenWrite()
{
	ASSERT(m_File == INVALID_HANDLE_VALUE);
	m_File = ::CreateFile(m_Name, GENERIC_WRITE, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	return (m_File != INVALID_HANDLE_VALUE);
}


//////////////////////////////////////////////////////////////////////////////
// otev�en� souboru bez p��stupu (nastaven� vlastnost�)

BOOL CFile::OpenNone()
{
	ASSERT(m_File == INVALID_HANDLE_VALUE);
	m_File = ::CreateFile(m_Name, 0, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	return (m_File != INVALID_HANDLE_VALUE);
}


//////////////////////////////////////////////////////////////////////////////
// otev�en� adres��e (pro nastaven� vlastnost�)

BOOL CFile::OpenDirectory()
{
	ASSERT(m_File == INVALID_HANDLE_VALUE);
	m_File = ::CreateFile(m_Name, 0, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	return (m_File != INVALID_HANDLE_VALUE);
}


//////////////////////////////////////////////////////////////////////////////
// vytvo�en� souboru

BOOL CFile::Create()
{
	ASSERT(m_File == INVALID_HANDLE_VALUE);
	m_File = ::CreateFile(m_Name, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	return (m_File != INVALID_HANDLE_VALUE);
}


//////////////////////////////////////////////////////////////////////////////
// otev�en� pro standardn� vstup (neuzav�rat!)

BOOL CFile::OpenInput()
{
	ASSERT(m_File == INVALID_HANDLE_VALUE);
	m_File = ::GetStdHandle(STD_INPUT_HANDLE);
	return (m_File != INVALID_HANDLE_VALUE);
}


//////////////////////////////////////////////////////////////////////////////
// otev�en� pro standardn� v�stup (neuzav�rat!)

BOOL CFile::OpenOutput()
{
	ASSERT(m_File == INVALID_HANDLE_VALUE);
	m_File = ::GetStdHandle(STD_OUTPUT_HANDLE);
	return (m_File != INVALID_HANDLE_VALUE);
}


//////////////////////////////////////////////////////////////////////////////
// otev�en� pro chybov� v�stup (neuzav�rat!)

BOOL CFile::OpenError()
{
	ASSERT(m_File == INVALID_HANDLE_VALUE);
	m_File = ::GetStdHandle(STD_ERROR_HANDLE);
	return (m_File != INVALID_HANDLE_VALUE);
}


//////////////////////////////////////////////////////////////////////////////
// uzav�en� souboru

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
// �ten� ze souboru

BOOL CFile::Read(void* buf, int num)
{
	ASSERT(m_File != INVALID_HANDLE_VALUE);
	DWORD read;
	return (::ReadFile(m_File, buf, num, &read, NULL)
		&& (read == (DWORD)num));
}


//////////////////////////////////////////////////////////////////////////////
// z�pis do souboru

BOOL CFile::Write(void* buf, int num)
{
	ASSERT(m_File != INVALID_HANDLE_VALUE);
	DWORD write;
	return (::WriteFile(m_File, buf, num, &write, NULL)
		&& (write == (DWORD)num));
}


//////////////////////////////////////////////////////////////////////////////
// zru�en� souboru (mus� b�t uzav�en)

BOOL CFile::Delete()
{
	ASSERT(m_File == INVALID_HANDLE_VALUE);
	return ::DeleteFile(m_Name);
}


//////////////////////////////////////////////////////////////////////////////
// poskytnut� velikosti souboru (mus� b�t otev�en, -1 = chyba)

int CFile::Size()
{
	ASSERT(m_File != INVALID_HANDLE_VALUE);
	return ::GetFileSize(m_File, NULL);
}


//////////////////////////////////////////////////////////////////////////////
// poskytnut� typu souboru (mus� b�t otev�en)
//		FILE_TYPE_UNKNOWN ... nezn�m�
//		FILE_TYPE_DISK ...... diskov� soubor
//		FILE_TYPE_CHAR ...... znakov� za��zen�
//		FILE_TYPE_PIPE ...... kan�l

int CFile::Type()
{
	ASSERT(m_File != INVALID_HANDLE_VALUE);
	return ::GetFileType(m_File);
}


//////////////////////////////////////////////////////////////////////////////
// poskytnut�/nastaven� �asu vytvo�en� (mus� b�t otev�en)

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
// poskytnut�/nastaven� �asu posledn�ho �ten� (mus� b�t otev�en)

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
// poskytnut�/nastaven� �asu posledn�ho z�pisu (mus� b�t otev�en)

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
// poskytnut� atribut� souboru (nemus� b�t otev�en, -1=chyba)

int CFile::Attribute()
{
	return ::GetFileAttributes(m_Name);
}


//////////////////////////////////////////////////////////////////////////////
// nastaven� atribut� souboru (nemus� b�t otev�en)

BOOL CFile::Attribute(int atr)
{
	return ::SetFileAttributes(m_Name, atr & 
		~(FILE_ATTRIBUTE_DIRECTORY | 
		FILE_ATTRIBUTE_COMPRESSED |
		FILE_ATTRIBUTE_OFFLINE |
		FILE_ATTRIBUTE_TEMPORARY));
}


//////////////////////////////////////////////////////////////////////////////
// resetov�n� ukazatele v souboru

BOOL CFile::Reset()
{
	ASSERT(m_File != INVALID_HANDLE_VALUE);
	return ::SetFilePointer(m_File, 0, NULL, FILE_BEGIN);
}


//////////////////////////////////////////////////////////////////////////////
// poskytnut� ukazatele v souboru (-1 = chyba)

int CFile::Seek()
{
	ASSERT(m_File != INVALID_HANDLE_VALUE);
	return ::SetFilePointer(m_File, 0, NULL, FILE_CURRENT);
}


//////////////////////////////////////////////////////////////////////////////
// nastaven� ukazatele v souboru

BOOL CFile::Seek(int pos)
{
	ASSERT(m_File != INVALID_HANDLE_VALUE);
	return ((int)::SetFilePointer(m_File, pos, NULL, FILE_BEGIN) != -1);
}


//////////////////////////////////////////////////////////////////////////////
// relativn� nastaven� ukazatele v souboru

BOOL CFile::SeekRel(int pos)
{
	ASSERT(m_File != INVALID_HANDLE_VALUE);
	return ((int)::SetFilePointer(m_File, pos, NULL, FILE_CURRENT) != -1);
}


//////////////////////////////////////////////////////////////////////////////
// nastaven� ukazatele v souboru na konec

BOOL CFile::End()
{
	ASSERT(m_File != INVALID_HANDLE_VALUE);
	return ((int)::SetFilePointer(m_File, 0, NULL, FILE_END) != -1);
}


//////////////////////////////////////////////////////////////////////////////
// oper�tor p�i�azen�

const CFile& _fastcall CFile::operator= (const CFile& file)
{
	ASSERT(m_File == INVALID_HANDLE_VALUE);
	m_File = file.m_File;
	m_Name = file.m_Name;
	return *this;
}


/***************************************************************************\
*																			*
*								Datov� buffer								*
*																			*
\***************************************************************************/

//////////////////////////////////////////////////////////////////////////////
// konstruktor

CBuf::CBuf()
{
	m_Adr = NULL;				// adresa bufferu v pam�ti
	m_End = NULL;				// adresa za koncem bufferu
	m_Size = 0;					// velikost bufferu
}


//////////////////////////////////////////////////////////////////////////////
// statick� konstruktor

void CBuf::Init()
{
	m_Adr = NULL;				// adresa bufferu v pam�ti
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
// adresa NT z�hlav� souboru EXE (NULL=nen�)

IMAGE_NT_HEADERS* CBuf::NTHeader()
{
// kontrola DOS z�hlav�
	IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)Adr();	// adresa DOS z�hlav�
	if (IsNotValid(dos, sizeof(IMAGE_DOS_HEADER)) ||	// velikost pro DOS z�hlav�
		(dos->e_magic != IMAGE_DOS_SIGNATURE))			// identifik�tor
		return NULL;

// adresa NT z�hlav�
	int off = dos->e_lfanew;						// offset NT z�hlav�
	IMAGE_NT_HEADERS* hdr = (IMAGE_NT_HEADERS*)(Adr() + off); // adresa NT z�hlav�

// kontrola NT z�hlav�
	if ((off < 0x60) ||								// min. offset NT z�hlav�
		(off >= 0x1000) ||							// max. offset NT z�hlav�
		(IsNotValid(hdr, sizeof(IMAGE_NT_HEADERS))) ||	// kontrola velikosti bufferu
		(hdr->Signature != IMAGE_NT_SIGNATURE))		// identifik�tor
		return NULL;

// kontrola velikosti z�hlav� v�etn� sekc�
	off = hdr->FileHeader.SizeOfOptionalHeader;		// velikost voliteln� sekce
	int sekce = hdr->FileHeader.NumberOfSections;	// po�et sekc�
	if ((sekce < 1) ||								// minim�ln� sekc�
		(sekce > 1000) ||							// maxim�ln� sekc�
		(off < 50) ||								// minim�ln� velikost z�hlav�
		(off > 10000) ||							// maxim�ln� velikost z�hlav�
		(IsNotValid(hdr, sizeof(DWORD) + 
			sizeof(IMAGE_FILE_HEADER) + off + 
			sekce*IMAGE_SIZEOF_SECTION_HEADER)))	// kontrola velikosti sekc�
		return NULL;

// adresa NT z�hlav�
	return hdr;
}


//////////////////////////////////////////////////////////////////////////////
// nalezen� NT sekce souboru EXE (NULL=nen�)

IMAGE_SECTION_HEADER* CBuf::NTSection(IMAGE_NT_HEADERS* hdr, char* name)
{
// po�et sekc�
	int num = hdr->FileHeader.NumberOfSections;

// adresa prvn� sekce
	IMAGE_SECTION_HEADER* sec = (IMAGE_SECTION_HEADER*)
		((BYTE*)(&hdr->OptionalHeader) + hdr->FileHeader.SizeOfOptionalHeader);

// cyklus p�es v�echny sekce
	for (; num > 0; num--)
	{
// adresa jmen
		char* src = name;
		char* dst = (char*)sec->Name;

// porovn�n� jm�na sekce
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

// adresa dal�� sekce
		sec++;
	}

// sekce nenalezena
	return NULL;
}


/*
//////////////////////////////////////////////////////////////////////////////
// nalezen� resource dat souboru EXE (NULL=nen�)

BYTE* CBuf::NTResource()
{
// p��prava NT z�hlav�
	IMAGE_NT_HEADERS* hdr = NTHeader();
	if (hdr == NULL) return NULL;

// p��prava sekce resource
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
// nalezen� implicitn� ikony v NT souboru (NULL=nen�)

#define DIR			IMAGE_RESOURCE_DIRECTORY
#define DIRSIZE		sizeof(DIR)
#define ENTRY		IMAGE_RESOURCE_DIRECTORY_ENTRY
#define ENTRYSIZE	sizeof(ENTRY)

BITMAPINFO* CBuf::NTIcon()
{
// lok�ln� prom�nn�
	IMAGE_NT_HEADERS* hdr;				// NT z�hlav�
	IMAGE_SECTION_HEADER* sec;			// sekce resource
	BYTE* res;							// adresa dat resource
	DIR* dir;							// adresa adres��e resource
	int num;							// po�et polo�ek v typov�m adres��i
	ENTRY* entry;						// polo�ka adres��e
	BITMAPINFO* bmp;					// adresa ikony
	IMAGE_RESOURCE_DATA_ENTRY* data;	// data ikony

// p��prava NT z�hlav�
	hdr = NTHeader();
	if (hdr == NULL) goto CHYBA;

// p��prava sekce resource
	sec = NTSection(hdr, ".rsrc");
	if (sec == NULL) goto CHYBA;

// adresa resource dat
	res = Adr() + sec->PointerToRawData;
	if (IsNotValid(res, sec->SizeOfRawData)) goto CHYBA;

// adresa typov�ho adres��e resource
	dir = (DIR*)res;
	if (IsNotValid(dir, DIRSIZE)) goto CHYBA;

// po�et polo�ek v typov�m adres��i resource
	num = dir->NumberOfNamedEntries + dir->NumberOfIdEntries;

// adresa prvn� polo�ky typov�ho adres��e
	entry = (ENTRY*)(dir + 1);

// cyklus p�es polo�ky typov�ho adres��e
	for (; num > 0; num--)
	{
// kontrola platnosti adresy polo�ky
		if (IsNotValid(entry, ENTRYSIZE)) goto CHYBA;

// kontrola, zda je polo�ka adres�� typu ikon
		if (((entry->Name & IMAGE_RESOURCE_NAME_IS_STRING) == 0) &&
			(entry->Id == (WORD)RT_ICON) &&
			(entry->OffsetToData & IMAGE_RESOURCE_DATA_IS_DIRECTORY))
		{

// p��prava jmenn�ho adres��e ikon
			dir = (DIR*)(res + entry->OffsetToDirectory);
			if (IsNotValid(dir, DIRSIZE)) goto CHYBA;
			entry = (ENTRY*)(dir + 1);
			if (IsNotValid(entry, ENTRYSIZE)) goto CHYBA;

// p��prava jazykov�ho adres��e ikony
			dir = (DIR*)(res + entry->OffsetToDirectory);
			if (IsNotValid(dir, DIRSIZE)) goto CHYBA;
			entry = (ENTRY*)(dir + 1);
			if (IsNotValid(entry, ENTRYSIZE)) goto CHYBA;

// p��prava datov� polo�ky prvn� ikony
			data = (IMAGE_RESOURCE_DATA_ENTRY*)(res + entry->OffsetToData);
			if (IsNotValid(data, sizeof(IMAGE_RESOURCE_DATA_ENTRY))) goto CHYBA;

// p��prava ukazatele dat na data ikony
			bmp = (BITMAPINFO*)(res + data->OffsetToData - sec->VirtualAddress);
			if (IsNotValid(bmp, sizeof(BITMAPINFOHEADER) + 
				256*sizeof(RGBQUAD) + ICONSIZE + ICONSIZE/8)) goto CHYBA;

// kontrola BMP z�hlav�
			if ((bmp->bmiHeader.biSize != sizeof(BITMAPINFOHEADER)) ||
				(bmp->bmiHeader.biWidth != ICONWIDTH) ||
				(bmp->bmiHeader.biHeight != ICONHEIGHT*2) ||
				(bmp->bmiHeader.biPlanes != 1) ||
				(bmp->bmiHeader.biBitCount != 8))
				goto CHYBA;

// v�e je OK
			return bmp;
		}

// adresa dal�� polo�ky adres��e
		entry++;
	}

// chyba
CHYBA:
	return NULL;
}


/***************************************************************************\
*																			*
*					Pam�ov� mapovan� soubor (jen pro �ten�)				*
*																			*
\***************************************************************************/

//////////////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CFileMap::CFileMap()
{
	m_Map = NULL;				// nen� mapov�n�
}

CFileMap::~CFileMap()
{
	Close();
}


//////////////////////////////////////////////////////////////////////////////
// statick� konstruktor a destruktor

void CFileMap::Init()
{
	CBuf::Init();				// inicializace bufferu
	CFile::Init();				// inicializace souboru
	m_Map = NULL;				// nen� mapov�n�
}

void CFileMap::Term()
{
	CFile::Term();
	Close();
}


//////////////////////////////////////////////////////////////////////////////
// otev�en�

BOOL CFileMap::Open(const CString& name)
{
// otev�en� mapovan�ho souboru
	CFile::Name(name);
	if (CFile::OpenRead())
	{

// zji�t�n� velikosti souboru
		ASSERT(Size() == 0);
		Size(CFile::Size());
		if (Size() != -1)
		{

// vytvo�en� mapov�n�
			ASSERT(m_Map == NULL);
			m_Map = ::CreateFileMapping(CFile::File(), NULL, PAGE_READONLY, 0, Size(), NULL);
			if (m_Map != NULL)
			{

// namapov�n� do okna v pam�ti
				ASSERT(Adr() == NULL);
				Adr((BYTE*)::MapViewOfFile(m_Map, FILE_MAP_READ, 0, 0, Size()));
				if (Adr() != NULL)
				{

// operace je OK
					return TRUE;
				}

// uzav�en� mapov�n� p�i chyb�
				::CloseHandle(m_Map);
				m_Map = NULL;
			}
		}

// uzav�en� souboru p�i chyb�
		Size(0);
		CFile::Close();
	}

// chybov� n�vrat
	return FALSE;
}


//////////////////////////////////////////////////////////////////////////////
// uzav�en�

void CFileMap::Close()
{
// uvoln�n� okna v pam�ti
	ASSERT(Adr() != NULL);
	if (Adr() != NULL)
	{
		::UnmapViewOfFile(Adr());
		Adr(NULL);
	}

// uzav�en� mapov�n�
	ASSERT(m_Map != NULL);
	if (m_Map != NULL)
	{
		::CloseHandle(m_Map);
		m_Map = NULL;
	}

// uzav�en� souboru
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
	m_Instance = NULL;			// implicitn� instance
}

CResource::~CResource()
{
	CloseFile();				// uzav�en� souboru
	Close();					// uzav�en� resource
}


//////////////////////////////////////////////////////////////////////////////
// statick� konstruktor a destruktor

void CResource::Init()
{
	CBuf::Init();				// inicializace bufferu
	m_Instance = NULL;			// implicitn� instance
}

void CResource::Term()
{
	CloseFile();				// uzav�en� souboru
	Close();					// uzav�en� resource
}


//////////////////////////////////////////////////////////////////////////////
// na�ten� souboru s resource

BOOL CResource::OpenFile(LPCTSTR name)
{
	ASSERT(m_Instance == NULL);
	m_Instance = ::LoadLibrary(name);
	return (m_Instance != NULL);
}


//////////////////////////////////////////////////////////////////////////////
// uzav�en� souboru s resource

void CResource::CloseFile()
{
	if (m_Instance)
	{
		::FreeLibrary(m_Instance);
		m_Instance = NULL;
	}
}


//////////////////////////////////////////////////////////////////////////////
// otev�en� resource

BOOL CResource::Open(int id, LPCTSTR typ)
{
// nalezen� resource
	HRSRC res = ::FindResource(m_Instance, MAKEINTRESOURCE(id), typ);
	if (res)
	{

// zji�t�n� velikosti dat
		ASSERT(Size() == 0);
		Size(::SizeofResource(m_Instance, res));
		if (Size() > 0)
		{

// na�ten� dat do pam�ti
			HGLOBAL glob = ::LoadResource(m_Instance, res);
			if (glob)
			{

// uzamknut� dat
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

// chybov� n�vrat
	return FALSE;
}


//////////////////////////////////////////////////////////////////////////////
// otev�en� kopie dat (pro mo�nost z�pisu, buffer je nutno po pou�it� zru�it)

BOOL CResource::OpenCopy(int id, LPCTSTR typ)
{
	if (!Open(id, typ)) return FALSE;

	BYTE* buf = (BYTE*)MemGet(Size());
	MemCopy(buf, Adr(), Size());
	Adr(buf);
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////////
// zru�en� bufferu s kopi� resource dat

void CResource::CloseCopy()
{
	MemFree(Adr());
	Close();
}


//////////////////////////////////////////////////////////////////////////////
// uzav�en� (rezervov�no - zat�m nen� nutn� prov�d�t)

void CResource::Close()
{
	Adr(0);
	Size(0);
}
