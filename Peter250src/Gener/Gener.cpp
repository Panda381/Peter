// Gener.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BufText.h"
#include "File.h"

#define COMPACT			// kompaktní instalaèní balík

/*
struktura adresáøù (skupiny):
	0 ...... hlavní program aplikace Petr (soubory PETER.EXE a PETER.HLP)
	1 ...... pøíklady vytvoøených programù (adresáø PROGRAM)
	2 ...... knihovna pøedmìtù (adresáø ICON)
	3 ...... knihovna obrázkù (adresáø PICTURE)
	4 ...... knihovna sprajtù (adresáø SPRITE)
	5 ...... knihovna zvukù (adresáø SOUND)
	6 ...... knihovna hudeb (adresáø MUSIC)
	7 ...... ostatní knihovny (adresáøe BOOL, MAP, NUMBER a TEXT)
	8 ...... zdroje pro generování 3D sprajtù (adresáø SOURCE)

  Peter.exe urèuje datum a èas souborù

struktura adresáøù pro MINI (skupiny):
	0 ...... hlavní program aplikace Petr (soubory PETER.EXE a PETER.HLP)

  Peter.exe urèuje datum a èas souborù

struktura adresáøù pro DATAINST (skupiny):
    0 ...... instalaèní podadresáø (PATH.TXT), název dat (TITLE.TXT, øádky: èesky, anglicky) - v kódu Windows
    1 ...... instalovaná data

	SETUP .. výchozí program SETUP - DATAINST

  PATH.TXT urèuje datum a èas souborù

*/

//////////////////////////////////////////////////////////////////////////////
// struktura instalaèních dat

// Poøadí dat:
// - záhlaví instalaèních dat, informace o skupinách
// - seznam souborù
// - data souborù (komprimováno po skupinách) - jen verze MINI a DATAINST


#ifdef _INSTALL
#define GROUPSNUM 2							// poèet skupin DATAINST
#else
#ifdef MINI
#define GROUPSNUM 3							// poèet skupin
#else
#define GROUPSNUM 9							// poèet skupin
#endif
#endif  // _INSTALL

// definice jednoho souboru v seznamu (9 B + text)
typedef struct INSTFILE_ {
	long			Size;					// (4) velikost souboru v bajtech (po dekompresi)
	long			Check;					// (4) kontrolní souèet dat souboru (výchozí 0, pøièten bajt, rotace vlevo s pøenosem)
	BYTE			NameN;					// (1) délka jména souboru vèetnì podcesty - ve znacích
	char			Name[1];				// (n) jméno souboru (vèetnì podcesty) - velká písmena
} INSTFILE;

// definice jedné skupiny (16 B)
typedef struct INSTGROUP_ {
	long			Files;					// (4) poèet souborù ve skupinì
	long			Size;					// (4) velikost skupiny v KB (po nainstalování) - soubory zaokrouhleny na alokaèní bloky 8 KB
	long			SizeFiles;				// (4) velikost seznamu souborù (bajtù)
	long			SizeGroup;				// (4) velikost komprimovaných dat (bajtù) - u plné verze 0
} INSTGROUP;

// záhlaví instalaèních dat (16 B + skupiny)
typedef struct INSTHEAD_ {
	char			Ident[4];				// (4) identifikace (text "SET" + bínárnì poèet sekcí)
	long			Check;					// (4) kontrolní souèet zbytku záhlaví vèetnì seznamu souborù
	FILETIME		DateTime;				// (8) lokální (!) datum a èas souborù
	INSTGROUP		Groups[GROUPSNUM];		// definice skupin
} INSTHEAD;


INSTHEAD		Head;						// záhlaví instalaèních dat
int				FilesS = 0;					// velikost seznamu (bajtù)
BYTE*			Files = NULL;				// seznam souborù

//////////////////////////////////////////////////////////////////////////////
// adresáø souborù (1 položka 16 B)

typedef struct ADRFILE_ {
	CText			Name;					// jméno souboru s cestou (velká písmena)
	CText			Ext;					// pøípona jména souboru (kvùli tøídìní)
	long			Size;					// velikost souboru (bajtù)
	long			Check;					// kontrolní souèet dat souboru (výchozí 0, pøièten bajt, rotace vlevo s pøenosem)
} ADRFILE;

int				BufFileN = 0;				// poèet položek v bufferu
ADRFILE*		BufFile = NULL;				// buffer seznamu souborù

int				GroupNum;					// èíslo generované skupiny
CText			GroupNumT;					// èíslo skupiny pøevedené na text

#if defined(_INSTALL) || defined(COMPACT)
CText			GroupFile;					// jméno souboru skupiny TMP
CText			GroupFile2;					// jméno souboru skupiny DAT
HANDLE			GroupFileH = NULL;			// handle souboru skupiny
#endif

HINSTANCE	hInstance = NULL;			// instance programu

//////////////////////////////////////////////////////////////////////////////
// sekce dat

IMAGE_SECTION_HEADER	SetupHeader = {
	'.', 's', 'e', 't', 'u', 'p', 'd', 0,		// jméno sekce
	0,											// virtuální velikost
	0,											// virtuální adresa
	0,											// velikost dat v souboru
	0,											// offset dat v souboru
	0,											// ... (offset relokací)
	0,											// ... (offset èísel øádkù)
	0,											// ... (poèet relokací)
	0,											// ... (poèet èísel øádkù)
	IMAGE_SCN_MEM_READ |						// vlastnosti
	IMAGE_SCN_MEM_DISCARDABLE |
	IMAGE_SCN_CNT_INITIALIZED_DATA
};

// velikost stránky zarovnávání pamìti
#ifndef _M_ALPHA
#define	PAGESIZE		0x1000			// velikost alokaèní stránky pro ostatní procesory (4096)
#define PAGEFILE		0x1000			// velikost stránky v souboru
#else
#define	PAGESIZE		0x2000			// velikost alokaèní stránky pro procesor Alpha (8192)
#define PAGEFILE		0x2000			// velikost stránky v souboru
#endif


//////////////////////////////////////////////////////////////////////////////
// chyba

void debugbreak(const char* text)
{
	::MessageBox(
		NULL,
		text,
		"Chyba generátoru",
		MB_OK | MB_ICONERROR);

	::ExitProcess(1);
}


//////////////////////////////////////////////////////////////////////////////
// spuštìní programu

#if defined(_INSTALL) || defined(COMPACT)

int Exec(CText command, CText aktdir, BOOL wait)
{
	int result = 0;
	command.TrimLeft();
	if (command.IsEmpty()) return 255;

// korekce aktivního adresáøe
	if (aktdir.Length() > 1)
	{
		if (aktdir.LastChar() == '\\')
		{
			aktdir.Delete(aktdir.Length()-1);
		}
	}

// inicializace startovací tabulky
	STARTUPINFO si;
	MemFill(&si, sizeof(si), 0);
	si.cb = sizeof(si);

// inicializace tabulky procesu
	PROCESS_INFORMATION pi;
	MemFill(&pi, sizeof(pi), 0);

// spuštìní programu
	::CreateProcess(
		NULL,								// jméno programu
		(LPTSTR)(LPCTSTR)command,			// pøíkazový øádek
		NULL,								// ochranné atributy procesu
		NULL,								// ochranné atributy vlákna
		FALSE,								// dìdiènost handlù
#ifdef _UNICODE
		CREATE_UNICODE_ENVIRONMENT			// parametry
#else
		0
#endif
		| CREATE_DEFAULT_ERROR_MODE,
		NULL,								// prostøedí
		aktdir.IsEmpty() ? NULL : (LPCTSTR)aktdir, // výchozí adresáø
		&si,								// adresa STARTUPINFO
		&pi);								// adresa PROCESS_INFORMATION

// èekání na ukonèení programu
	if (wait)
	{
		::WaitForSingleObject(pi.hProcess, INFINITE);	// èekání na ukonèení procesu
		::GetExitCodeProcess(pi.hProcess, (DWORD*)&result);	// zjištìní návratového kódu
	}

// uzavøení handle procesu
	::CloseHandle(pi.hProcess);				// uzavøení handle procesu
	::CloseHandle(pi.hThread);				// uzavøení handle toku

	return result;
}

#endif

//////////////////////////////////////////////////////////////////////////////
// vyhledání souborù v jednom podadresáøi (voláno rekurzivnì, není poèáteèní ani koncový "\")

void ReadDir(CText path)
{
// plná cesta
	CText path2;
	if (path.IsEmpty())
	{
		path2 = GroupNumT;
	}
	else
	{
		path2 = GroupNumT + _T('\\') + path;
	}

// ovìøení platnosti adresáøe
	ASSERT((int)::GetFileAttributes(path2) != -1, "Neplatná cesta " + path2);

// zahájení vyhledávání souborù a podadresáøù
	WIN32_FIND_DATA fnd;
	HANDLE find = ::FindFirstFile(path2 + _T("\\*.*"), &fnd);
	ASSERT(find != INVALID_HANDLE_VALUE, "Nenalezen žádný soubor v adresáøi " + path2);

	CText name1;
	CText name2;

// dokud je další položka
	do {

// pøíprava jména nalezeného souboru/adresáøe
		name1 = fnd.cFileName;

// test, zda se jedná o adresáø
		if (fnd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{

// test, zda je to platný adresáø
			if ((name1 != _T("..")) && (name1 != _T(".")))
			{

// vnoøení do adresáøe
				if (path.IsEmpty())
				{
					ReadDir(name1);
				}
				else
				{
					ReadDir(path + _T('\\') + name1);
				}
			}
		}

// jinak to bude soubor 
		else
		{

// pøíprava jména souboru
			if (path.IsEmpty())
			{
				name2 = name1;
			}
			else
			{
				name2 = path + _T('\\') + name1;
			}

// soubor PETER.EXE bude použit jako vzor pro datum a èas
			name1.UpperCase();
			if (name1 == _T("PETER.EXE"))
			{
				::FileTimeToLocalFileTime(&fnd.ftLastWriteTime, &Head.DateTime);
			}

// zvýšení velikosti bufferu
			BufFileN++;
			BufFile = (ADRFILE*) MemSize(BufFile, BufFileN * sizeof(ADRFILE));
			ASSERT(BufFile, "Pøeteèení pamìti");

// pøíprava pøipony
			name2.UpperCase();
			int pos = name2.RevFind(_T('.')) + 1;
			ASSERT(pos > 0, "Soubor bez pøípony " + name2);
			name1 = name2.Right(name2.Length() - pos);

// úschova informací o souboru
			BufFile[BufFileN-1].Name.Init(name2);
			BufFile[BufFileN-1].Ext.Init(name1);
			BufFile[BufFileN-1].Size = fnd.nFileSizeLow;

// zaøazení souboru do evidence skupiny
			Head.Groups[GroupNum].Size += ((fnd.nFileSizeLow + 0x3fff) & ~0x3fff) / 1024; // zarovnání na alokaèní blok 16KB
			Head.Groups[GroupNum].SizeFiles += 2*sizeof(long) + sizeof(BYTE) + name2.Length();
		}


// nalezení další položky
	} while (::FindNextFile(find, &fnd));

// uzavøení hledání
	::FindClose(find);
}


//////////////////////////////////////////////////////////////////////////////
// naètení jedné skupiny

void ReadGroup(int group)
{
// pøíprava jména skupiny
	GroupNum = group;
	GroupNumT.Int(group);

// vytvoøení souboru skupiny TMP
#if defined(_INSTALL) || defined(COMPACT)
	GroupFile = _T("GRP") + GroupNumT + _T(".TMP");
	GroupFile2 = _T("GRP") + GroupNumT + _T(".DAT");
	GroupFileH = ::CreateFile(GroupFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	ASSERT(GroupFileH != INVALID_HANDLE_VALUE, "Nelze vytvoøit soubor " + GroupFile);
#else
	printf(CText("skupina ") + GroupNumT + CText("\n"));
#endif

// vyhledání souborù v adresáøi skupiny
	ReadDir(EmptyText);

// setøídìní seznamu souborù
	CText name;
	CText ext;
	int size;
	int i, j;

	for (i = 0; i < BufFileN - 1; i++)
	{
		for (j = i + 1; j < BufFileN; j++)
		{
			if ((BufFile[i].Ext > BufFile[j].Ext) ||
				(!(BufFile[i].Ext < BufFile[j].Ext) &&
				(BufFile[i].Name > BufFile[j].Name)))
			{
				name = BufFile[i].Name;
				ext = BufFile[i].Ext;
				size = BufFile[i].Size;

				BufFile[i].Name = BufFile[j].Name;
				BufFile[i].Ext = BufFile[j].Ext;
				BufFile[i].Size = BufFile[j].Size;

				BufFile[j].Name = name;
				BufFile[j].Ext = ext;
				BufFile[j].Size = size;
			}
		}
	}

// buffer k pøenesení dat souborù
#define BUFSIZE 0xf000

	BYTE* buf = (BYTE*)MemGet(BUFSIZE);
	ASSERT(buf, "Chyba pamìti");
	long check;

// cyklus pøes všechny soubory
	for (i = 0; i < BufFileN; i++)
	{
		check = 0;

// velikost dat souboru
		size = BufFile[i].Size;

// plné jméno souboru
		name = GroupNumT + _T('\\') + BufFile[i].Name;

// otevøení souboru pro ètení
		HANDLE file = ::CreateFile(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM |
							FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY, NULL);
		ASSERT(file != INVALID_HANDLE_VALUE, "Nelze otevøít soubor " + name);

// dokud jsou nìjaká data
		DWORD readwrite;

		while (size > 0)
		{
			int size0 = size;
			if (size0 > BUFSIZE) size0 = BUFSIZE;
			size -= size0;

			VERIFY(::ReadFile(file, buf, size0, &readwrite, NULL), "Chyba ètení souboru " + name);
			ASSERT((int)readwrite == size0, "Chyba ètení souboru " + name);

#if defined(_INSTALL) || defined(COMPACT)
			VERIFY(::WriteFile(GroupFileH, buf, size0, &readwrite, NULL), "Chyba zápisu do souboru " + GroupFile);
			ASSERT((int)readwrite == size0, "Chyba zápisu do souboru " + GroupFile);
#endif

			BYTE* buf0 = buf;
			for (; size0 > 0; size0--)
			{
				check += *buf0;
				if (check < 0)
				{
					check *= 2;
					check++;
				}
				else
				{
					check *= 2;
				}

				buf0++;
			}
		}
		BufFile[i].Check = check;

// ovìøení, zda už nejsou žádná data
		::ReadFile(file, buf, BUFSIZE, &readwrite, NULL);
		ASSERT((int)readwrite == 0, "Chyba ètení souboru" + name);

// uzavøení vstupního souboru
		::CloseHandle(file);
	}

// zrušení bufferu dat
	MemFree(buf);

// uzavøení výstupního souboru skupiny
#if defined(_INSTALL) || defined(COMPACT)
	::CloseHandle(GroupFileH);

// komprimace souboru
	::DeleteFile(GroupFile2);
	ASSERT((int)::GetFileAttributes(GroupFile2) == -1, "Nelze zrušit soubor " + GroupFile2);
	Exec(_T("HLPGEN.EXE ") + GroupFile + _T(' ') + GroupFile2, EmptyText, TRUE);
	printf("\n");
	ASSERT((int)::GetFileAttributes(GroupFile2) != -1, "Nevytvoøen soubor " + GroupFile2);
	::DeleteFile(GroupFile);
	ASSERT((int)::GetFileAttributes(GroupFile) == -1, "Nelze zrušit soubor " + GroupFile);

// úschova velikosti komprimovaných dat skupiny
	GroupFileH = ::CreateFile(GroupFile2, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ASSERT(GroupFileH != INVALID_HANDLE_VALUE, "Nelze otevøít soubor " + GroupFile2);
	Head.Groups[GroupNum].SizeGroup = ::GetFileSize(GroupFileH, NULL);
	::CloseHandle(GroupFileH);
#else
	Head.Groups[GroupNum].SizeGroup = 0;
#endif

// zaøazení seznamu souborù
	Head.Groups[GroupNum].Files = BufFileN;
	i = FilesS;
	FilesS = i + Head.Groups[GroupNum].SizeFiles;
	Files = (BYTE*)MemSize(Files, FilesS);
	ASSERT(Files, "Chyba pamìti");
	BYTE* dst = Files + i;

	for (i = 0; i < BufFileN; i++)
	{
		*(long*)dst = BufFile[i].Size;
		dst += sizeof(long);
		*(long*)dst = BufFile[i].Check;
		dst += sizeof(long);
		*(BYTE*)dst = BufFile[i].Name.Length();
		dst += sizeof(BYTE);
		BufFile[i].Name.Write((char*)dst);
		dst += BufFile[i].Name.Length();
	}

	ASSERT((dst - Files) == FilesS, "Nesouhlasí velikost seznamu souborù");

// zrušení seznamu souborù
	for (i = 0; i < BufFileN; i++)
	{
		BufFile[i].Name.Term();
		BufFile[i].Ext.Term();
	}
	MemFree(BufFile);
	BufFile = NULL;
	BufFileN = 0;
}


//////////////////////////////////////////////////////////////////////////////
// naètení textu jako jméno souboru

#ifdef _INSTALL

void ReadText(CText name)
{
// otevøení souboru pro ètení
	HANDLE file = ::CreateFile(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM |
						FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY, NULL);
	ASSERT(file != INVALID_HANDLE_VALUE, "Nelze otevøít soubor " + name);

// soubor PATH.TXT bude použit jako vzor pro datum a èas
	CText name1;
	name1 = name;
	name1.UpperCase();
	if (name1 == _T("0\\PATH.TXT"))
	{
		FILETIME datetime;
		::GetFileTime(file, NULL, NULL, &datetime);
		::FileTimeToLocalFileTime(&datetime, &Head.DateTime);
	}

// pøíprava velikosti souboru
	int size = ::GetFileSize(file, NULL);

// zvýšení seznamu
	Head.Groups[0].Files++;
	int i = 2*sizeof(long) + sizeof(BYTE) + size;
	Head.Groups[0].SizeFiles += i;
	int dstoff = FilesS;
	FilesS = dstoff + i;
	Files = (BYTE*)MemSize(Files, FilesS);
	ASSERT(Files, "Chyba pamìti");
	BYTE* dst = Files + dstoff;

// uložení informací
	*(long*)dst = 0;			// velikost
	dst += sizeof(long);
	*(long*)dst = 0;			// kontrolní souèet
	dst += sizeof(long);
	ASSERT(size < 256, "Pøeteèení délky textu v " + name);
	*(BYTE*)dst = (BYTE)size;	// délka textu
	dst += sizeof(BYTE);

// naètení textu
	DWORD readwrite;
	VERIFY(::ReadFile(file, dst, size, &readwrite, NULL), "Chyba ètení souboru " + name);
	ASSERT((int)readwrite == size, "Chyba ètení souboru " + name);

// uzavøení vstupního souboru
	::CloseHandle(file);
}

#endif // _INSTALL

//////////////////////////////////////////////////////////////////////////////
// start programu

int main(int argc, char* argv[])
{
// handle instance programu
	hInstance = ::GetModuleHandle(NULL);

#ifdef _INSTALL
	printf("Probiha generovani instalatoru PETR (DATAINST)\n"
		   "==============================================\n");
#else
#ifdef MINI
	printf("Probiha generovani instalatoru PETR (MINI)\n"
		   "==========================================\n");
#else
	printf("Probiha generovani instalatoru PETR\n"
		   "===================================\n");
#endif
#endif // _INSTALL

// inicializace záhlaví instalaèních dat
	MemFill(&Head, sizeof(Head), 0);
	Head.Ident[0] = 'S';
	Head.Ident[1] = 'E';
	Head.Ident[2] = 'T';
	Head.Ident[3] = GROUPSNUM;

#ifdef _INSTALL

// naètení informací pro InstData mód
	ReadText(_T("0\\Path.txt"));
	ReadText(_T("0\\Title.txt"));

// naètení souborù skupin
	for (int i = 1; i < GROUPSNUM; i++)
#else
	for (int i = 0; i < GROUPSNUM; i++)
#endif
	{
		ReadGroup(i);
	}

// musí být datum a èas
#ifdef _INSTALL
	ASSERT((Head.DateTime.dwLowDateTime != 0) || (Head.DateTime.dwLowDateTime != 0), "Chybí soubor PATH.TXT");
#else
	ASSERT((Head.DateTime.dwLowDateTime != 0) || (Head.DateTime.dwLowDateTime != 0), "Chybí soubor PETER.EXE");
#endif

// kontrolní souèet záhlaví
	long check = 0;
	BYTE* data = (BYTE*)&Head + 8;
	int n = sizeof(INSTHEAD) - 8;

	for (; n > 0; n--)
	{
		check += *data;
		if (check < 0)
		{
			check *= 2;
			check++;
		}
		else
		{
			check *= 2;
		}

		data++;
	}

// kontrolní souèet seznamu souborù
	data = Files;
	n = FilesS;

	for (; n > 0; n--)
	{
		check += *data;
		if (check < 0)
		{
			check *= 2;
			check++;
		}
		else
		{
			check *= 2;
		}

		data++;
	}

	Head.Check = check;

// pracovní buffer
	BYTE* buf = (BYTE*)MemGet(BUFSIZE);
	ASSERT(buf, "Chyba pamìti");
	DWORD readwrite;

// vytvoøení výstupního souboru
#ifdef _INSTALL
	::DeleteFile(_T("DataInst.exe"));
	HANDLE file = ::CreateFile(_T("DataInst.exe"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
#else
#ifdef COMPACT
#ifdef MINI
	::DeleteFile(_T("PeterMin.exe"));
	HANDLE file = ::CreateFile(_T("PeterMin.exe"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
#else
	::DeleteFile(_T("Peter250.exe"));
	HANDLE file = ::CreateFile(_T("Peter250.exe"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
#endif
#else
	::DeleteFile(_T("Setup.exe"));
	HANDLE file = ::CreateFile(_T("Setup.exe"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
#endif
#endif // _INSTALL

	ASSERT(file != INVALID_HANDLE_VALUE, "Chyba vytvoøení souboru Setup.exe");

// otevøení vstupního souboru SETUP.EXE
	HANDLE file0 = ::CreateFile(_T("Setup\\Setup.exe"), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ASSERT(file0 != INVALID_HANDLE_VALUE, "Nenalezen soubor Setup\\Setup.exe");

// pøíprava velikosti souboru SETUP.EXE
	int size = ::GetFileSize(file0, NULL);
#ifndef _INSTALL
	ASSERT(size > 66000, "Vadný soubor SETUP.EXE");
#endif

// offset zaèátku dat v souboru
	int dataoff = (size + (PAGEFILE-1)) & ~(PAGEFILE-1);

// naètení prvního bloku dat
	int size0 = size;
	if (size0 > BUFSIZE) size0 = BUFSIZE;
	size -= size0;

	VERIFY(::ReadFile(file0, buf, size0, &readwrite, NULL), "Chyba ètení ze souboru Setup\\Setup.exe");
	ASSERT((int)readwrite == size0, "Chyba ètení ze souboru Setup\\Setup.exe");

// adresa záhlaví NT
	CBuf bf;
	bf.Adr(buf);
	bf.Size(size0);
	IMAGE_NT_HEADERS* hdr = bf.NTHeader();
	ASSERT (hdr, "Chyba struktury Setup\\Setup.exe");

// pøíprava virtuální adresy dat
	int datavirt = (hdr->OptionalHeader.SizeOfImage + PAGESIZE-1) & ~(PAGESIZE-1);

// adresa vkládané sekce
	IMAGE_SECTION_HEADER* sec = (IMAGE_SECTION_HEADER*)((BYTE*)(&hdr->OptionalHeader) 
		+ hdr->FileHeader.SizeOfOptionalHeader + hdr->FileHeader.NumberOfSections 
		* sizeof(IMAGE_SECTION_HEADER));

// zvýšení èítaèe sekcí
	hdr->FileHeader.NumberOfSections++;

// nastavení sekce
	MemCopy(sec, &SetupHeader, IMAGE_SIZEOF_SECTION_HEADER);

// nastavení offsetu zaèátku dat v souboru a virtuální adresy
	sec->PointerToRawData = dataoff;
	sec->VirtualAddress = datavirt;

// nastavení virtuální velikosti dat
	int datasize = sizeof(Head) + FilesS;

#if defined(_INSTALL) || defined(COMPACT)
	for (i = 0; i < GROUPSNUM; i++)
	{
		datasize += Head.Groups[i].SizeGroup;
	}
#endif

	sec->Misc.VirtualSize = datasize;
	sec->SizeOfRawData = (datasize + 0x1ff) & ~0x1ff;

// nová velikost obrazu celého programu
	hdr->OptionalHeader.SizeOfImage = (datavirt + datasize + PAGESIZE-1) & ~(PAGESIZE-1);

// uložení prvního bloku dat
	VERIFY(::WriteFile(file, buf, size0, &readwrite, NULL), "Chyba zápisu do souboru Setup.exe");
	ASSERT((int)readwrite == size0, "Chyba zápisu do souboru Setup.exe");

// pøenesení souboru SETUP.EXE
	while (size > 0)
	{
		size0 = size;
		if (size0 > BUFSIZE) size0 = BUFSIZE;
		size -= size0;

		VERIFY(::ReadFile(file0, buf, size0, &readwrite, NULL), "Chyba ètení ze souboru Setup\\Setup.exe");
		ASSERT((int)readwrite == size0, "Chyba ètení ze souboru Setup\\Setup.exe");
		VERIFY(::WriteFile(file, buf, size0, &readwrite, NULL), "Chyba zápisu do souboru Setup.exe");
		ASSERT((int)readwrite == size0, "Chyba zápisu do souboru Setup.exe");
	}

// ovìøení, zda už nejsou žádná data
	::ReadFile(file0, buf, BUFSIZE, &readwrite, NULL);
	ASSERT(readwrite == 0, "Chyba ètení ze souboru Setup\\Setup.exe");

// zarovnání délky programu
	i = dataoff - ::GetFileSize(file0, NULL);
	ASSERT(i >= 0, "Chyba ètení ze souboru Setup\\Setup.exe");
	if (i > 0)
	{
		MemFill(buf, i, 0);
		::WriteFile(file, buf, i, &readwrite, NULL);
	}

// uzavøení souboru SETUP.EXE
	::CloseHandle(file0);

// uložení záhlaví
	VERIFY(::WriteFile(file, &Head, sizeof(INSTHEAD), &readwrite, NULL), "Chyba zápisu do souboru Setup.exe");
	ASSERT((int)readwrite == sizeof(Head), "Chyba zápisu do souboru Setup.exe");

// uložení seznamu souborù
	VERIFY(::WriteFile(file, Files, FilesS, &readwrite, NULL), "Chyba zápisu do souboru Setup.exe");
	ASSERT((int)readwrite == FilesS, "Chyba zápisu do souboru Setup.exe");

// pøenesení dat skupin
#if defined(_INSTALL) || defined(COMPACT)

#ifdef _INSTALL
	for (i = 1; i < GROUPSNUM; i++)
#else
	for (i = 0; i < GROUPSNUM; i++)
#endif
	{
		CText name;
		name.Int(i);
		name = _T("GRP") + name + _T(".DAT");
		file0 = ::CreateFile(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		ASSERT(file0 != INVALID_HANDLE_VALUE, "Chyba otevøení souboru " + name);

		size = Head.Groups[i].SizeGroup;

		while (size > 0)
		{
			int size0 = size;
			if (size0 > BUFSIZE) size0 = BUFSIZE;
			size -= size0;

			VERIFY(::ReadFile(file0, buf, size0, &readwrite, NULL), "Chyba ètení ze souboru " + name);
			ASSERT((int)readwrite == size0, "Chyba ètení ze souboru " + name);
			VERIFY(::WriteFile(file, buf, size0, &readwrite, NULL), "Chyba zápisu do souboru Setup.exe");
			ASSERT((int)readwrite == size0, "Chyba zápisu do souboru Setup.exe");
		}

// ovìøení, zda už nejsou žádná data
		::ReadFile(file0, buf, BUFSIZE, &readwrite, NULL);
		ASSERT(readwrite == 0, "Chyba ètení ze souboru " + name);

// uzavøení souboru skupiny
		::CloseHandle(file0);
	}

#endif

// zarovnání výstupního souboru na sektor
	size = ::SetFilePointer(file, 0, NULL, FILE_CURRENT);
	ASSERT(size > 0, "Chyba zápisu do Setup.exe");
	i = ((size + 0x1ff) & ~0x1ff) - size;

	if (i > 0)
	{
		MemFill(buf, i, 0);
		::WriteFile(file, buf, i, &readwrite, NULL);
	}

// zrušení bufferu dat
	MemFree(buf);

// nastavení data a èasu souboru
	FILETIME datetime;
	::LocalFileTimeToFileTime(&Head.DateTime, &datetime);
	::SetFileTime(file, NULL, NULL, &datetime);

// zavøení výstupního souboru
	::CloseHandle(file);

// zrušení souborù skupin
#if defined(_INSTALL) || defined(COMPACT)

#ifdef _INSTALL
	for (i = 1; i < GROUPSNUM; i++)
#else
	for (i = 0; i < GROUPSNUM; i++)
#endif
	{
		GroupNumT.Int(i);
		GroupFile2 = _T("GRP") + GroupNumT + _T(".DAT");
		::DeleteFile(GroupFile2);
	}

#endif

	return 0;
}

