// Gener.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BufText.h"
#include "File.h"

#define COMPACT			// kompaktn� instala�n� bal�k

/*
struktura adres��� (skupiny):
	0 ...... hlavn� program aplikace Petr (soubory PETER.EXE a PETER.HLP)
	1 ...... p��klady vytvo�en�ch program� (adres�� PROGRAM)
	2 ...... knihovna p�edm�t� (adres�� ICON)
	3 ...... knihovna obr�zk� (adres�� PICTURE)
	4 ...... knihovna sprajt� (adres�� SPRITE)
	5 ...... knihovna zvuk� (adres�� SOUND)
	6 ...... knihovna hudeb (adres�� MUSIC)
	7 ...... ostatn� knihovny (adres��e BOOL, MAP, NUMBER a TEXT)
	8 ...... zdroje pro generov�n� 3D sprajt� (adres�� SOURCE)

  Peter.exe ur�uje datum a �as soubor�

struktura adres��� pro MINI (skupiny):
	0 ...... hlavn� program aplikace Petr (soubory PETER.EXE a PETER.HLP)

  Peter.exe ur�uje datum a �as soubor�

struktura adres��� pro DATAINST (skupiny):
    0 ...... instala�n� podadres�� (PATH.TXT), n�zev dat (TITLE.TXT, ��dky: �esky, anglicky) - v k�du Windows
    1 ...... instalovan� data

	SETUP .. v�choz� program SETUP - DATAINST

  PATH.TXT ur�uje datum a �as soubor�

*/

//////////////////////////////////////////////////////////////////////////////
// struktura instala�n�ch dat

// Po�ad� dat:
// - z�hlav� instala�n�ch dat, informace o skupin�ch
// - seznam soubor�
// - data soubor� (komprimov�no po skupin�ch) - jen verze MINI a DATAINST


#ifdef _INSTALL
#define GROUPSNUM 2							// po�et skupin DATAINST
#else
#ifdef MINI
#define GROUPSNUM 3							// po�et skupin
#else
#define GROUPSNUM 9							// po�et skupin
#endif
#endif  // _INSTALL

// definice jednoho souboru v seznamu (9 B + text)
typedef struct INSTFILE_ {
	long			Size;					// (4) velikost souboru v bajtech (po dekompresi)
	long			Check;					// (4) kontroln� sou�et dat souboru (v�choz� 0, p�i�ten bajt, rotace vlevo s p�enosem)
	BYTE			NameN;					// (1) d�lka jm�na souboru v�etn� podcesty - ve znac�ch
	char			Name[1];				// (n) jm�no souboru (v�etn� podcesty) - velk� p�smena
} INSTFILE;

// definice jedn� skupiny (16 B)
typedef struct INSTGROUP_ {
	long			Files;					// (4) po�et soubor� ve skupin�
	long			Size;					// (4) velikost skupiny v KB (po nainstalov�n�) - soubory zaokrouhleny na aloka�n� bloky 8 KB
	long			SizeFiles;				// (4) velikost seznamu soubor� (bajt�)
	long			SizeGroup;				// (4) velikost komprimovan�ch dat (bajt�) - u pln� verze 0
} INSTGROUP;

// z�hlav� instala�n�ch dat (16 B + skupiny)
typedef struct INSTHEAD_ {
	char			Ident[4];				// (4) identifikace (text "SET" + b�n�rn� po�et sekc�)
	long			Check;					// (4) kontroln� sou�et zbytku z�hlav� v�etn� seznamu soubor�
	FILETIME		DateTime;				// (8) lok�ln� (!) datum a �as soubor�
	INSTGROUP		Groups[GROUPSNUM];		// definice skupin
} INSTHEAD;


INSTHEAD		Head;						// z�hlav� instala�n�ch dat
int				FilesS = 0;					// velikost seznamu (bajt�)
BYTE*			Files = NULL;				// seznam soubor�

//////////////////////////////////////////////////////////////////////////////
// adres�� soubor� (1 polo�ka 16 B)

typedef struct ADRFILE_ {
	CText			Name;					// jm�no souboru s cestou (velk� p�smena)
	CText			Ext;					// p��pona jm�na souboru (kv�li t��d�n�)
	long			Size;					// velikost souboru (bajt�)
	long			Check;					// kontroln� sou�et dat souboru (v�choz� 0, p�i�ten bajt, rotace vlevo s p�enosem)
} ADRFILE;

int				BufFileN = 0;				// po�et polo�ek v bufferu
ADRFILE*		BufFile = NULL;				// buffer seznamu soubor�

int				GroupNum;					// ��slo generovan� skupiny
CText			GroupNumT;					// ��slo skupiny p�eveden� na text

#if defined(_INSTALL) || defined(COMPACT)
CText			GroupFile;					// jm�no souboru skupiny TMP
CText			GroupFile2;					// jm�no souboru skupiny DAT
HANDLE			GroupFileH = NULL;			// handle souboru skupiny
#endif

HINSTANCE	hInstance = NULL;			// instance programu

//////////////////////////////////////////////////////////////////////////////
// sekce dat

IMAGE_SECTION_HEADER	SetupHeader = {
	'.', 's', 'e', 't', 'u', 'p', 'd', 0,		// jm�no sekce
	0,											// virtu�ln� velikost
	0,											// virtu�ln� adresa
	0,											// velikost dat v souboru
	0,											// offset dat v souboru
	0,											// ... (offset relokac�)
	0,											// ... (offset ��sel ��dk�)
	0,											// ... (po�et relokac�)
	0,											// ... (po�et ��sel ��dk�)
	IMAGE_SCN_MEM_READ |						// vlastnosti
	IMAGE_SCN_MEM_DISCARDABLE |
	IMAGE_SCN_CNT_INITIALIZED_DATA
};

// velikost str�nky zarovn�v�n� pam�ti
#ifndef _M_ALPHA
#define	PAGESIZE		0x1000			// velikost aloka�n� str�nky pro ostatn� procesory (4096)
#define PAGEFILE		0x1000			// velikost str�nky v souboru
#else
#define	PAGESIZE		0x2000			// velikost aloka�n� str�nky pro procesor Alpha (8192)
#define PAGEFILE		0x2000			// velikost str�nky v souboru
#endif


//////////////////////////////////////////////////////////////////////////////
// chyba

void debugbreak(const char* text)
{
	::MessageBox(
		NULL,
		text,
		"Chyba gener�toru",
		MB_OK | MB_ICONERROR);

	::ExitProcess(1);
}


//////////////////////////////////////////////////////////////////////////////
// spu�t�n� programu

#if defined(_INSTALL) || defined(COMPACT)

int Exec(CText command, CText aktdir, BOOL wait)
{
	int result = 0;
	command.TrimLeft();
	if (command.IsEmpty()) return 255;

// korekce aktivn�ho adres��e
	if (aktdir.Length() > 1)
	{
		if (aktdir.LastChar() == '\\')
		{
			aktdir.Delete(aktdir.Length()-1);
		}
	}

// inicializace startovac� tabulky
	STARTUPINFO si;
	MemFill(&si, sizeof(si), 0);
	si.cb = sizeof(si);

// inicializace tabulky procesu
	PROCESS_INFORMATION pi;
	MemFill(&pi, sizeof(pi), 0);

// spu�t�n� programu
	::CreateProcess(
		NULL,								// jm�no programu
		(LPTSTR)(LPCTSTR)command,			// p��kazov� ��dek
		NULL,								// ochrann� atributy procesu
		NULL,								// ochrann� atributy vl�kna
		FALSE,								// d�di�nost handl�
#ifdef _UNICODE
		CREATE_UNICODE_ENVIRONMENT			// parametry
#else
		0
#endif
		| CREATE_DEFAULT_ERROR_MODE,
		NULL,								// prost�ed�
		aktdir.IsEmpty() ? NULL : (LPCTSTR)aktdir, // v�choz� adres��
		&si,								// adresa STARTUPINFO
		&pi);								// adresa PROCESS_INFORMATION

// �ek�n� na ukon�en� programu
	if (wait)
	{
		::WaitForSingleObject(pi.hProcess, INFINITE);	// �ek�n� na ukon�en� procesu
		::GetExitCodeProcess(pi.hProcess, (DWORD*)&result);	// zji�t�n� n�vratov�ho k�du
	}

// uzav�en� handle procesu
	::CloseHandle(pi.hProcess);				// uzav�en� handle procesu
	::CloseHandle(pi.hThread);				// uzav�en� handle toku

	return result;
}

#endif

//////////////////////////////////////////////////////////////////////////////
// vyhled�n� soubor� v jednom podadres��i (vol�no rekurzivn�, nen� po��te�n� ani koncov� "\")

void ReadDir(CText path)
{
// pln� cesta
	CText path2;
	if (path.IsEmpty())
	{
		path2 = GroupNumT;
	}
	else
	{
		path2 = GroupNumT + _T('\\') + path;
	}

// ov��en� platnosti adres��e
	ASSERT((int)::GetFileAttributes(path2) != -1, "Neplatn� cesta " + path2);

// zah�jen� vyhled�v�n� soubor� a podadres���
	WIN32_FIND_DATA fnd;
	HANDLE find = ::FindFirstFile(path2 + _T("\\*.*"), &fnd);
	ASSERT(find != INVALID_HANDLE_VALUE, "Nenalezen ��dn� soubor v adres��i " + path2);

	CText name1;
	CText name2;

// dokud je dal�� polo�ka
	do {

// p��prava jm�na nalezen�ho souboru/adres��e
		name1 = fnd.cFileName;

// test, zda se jedn� o adres��
		if (fnd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{

// test, zda je to platn� adres��
			if ((name1 != _T("..")) && (name1 != _T(".")))
			{

// vno�en� do adres��e
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

// p��prava jm�na souboru
			if (path.IsEmpty())
			{
				name2 = name1;
			}
			else
			{
				name2 = path + _T('\\') + name1;
			}

// soubor PETER.EXE bude pou�it jako vzor pro datum a �as
			name1.UpperCase();
			if (name1 == _T("PETER.EXE"))
			{
				::FileTimeToLocalFileTime(&fnd.ftLastWriteTime, &Head.DateTime);
			}

// zv��en� velikosti bufferu
			BufFileN++;
			BufFile = (ADRFILE*) MemSize(BufFile, BufFileN * sizeof(ADRFILE));
			ASSERT(BufFile, "P�ete�en� pam�ti");

// p��prava p�ipony
			name2.UpperCase();
			int pos = name2.RevFind(_T('.')) + 1;
			ASSERT(pos > 0, "Soubor bez p��pony " + name2);
			name1 = name2.Right(name2.Length() - pos);

// �schova informac� o souboru
			BufFile[BufFileN-1].Name.Init(name2);
			BufFile[BufFileN-1].Ext.Init(name1);
			BufFile[BufFileN-1].Size = fnd.nFileSizeLow;

// za�azen� souboru do evidence skupiny
			Head.Groups[GroupNum].Size += ((fnd.nFileSizeLow + 0x3fff) & ~0x3fff) / 1024; // zarovn�n� na aloka�n� blok 16KB
			Head.Groups[GroupNum].SizeFiles += 2*sizeof(long) + sizeof(BYTE) + name2.Length();
		}


// nalezen� dal�� polo�ky
	} while (::FindNextFile(find, &fnd));

// uzav�en� hled�n�
	::FindClose(find);
}


//////////////////////////////////////////////////////////////////////////////
// na�ten� jedn� skupiny

void ReadGroup(int group)
{
// p��prava jm�na skupiny
	GroupNum = group;
	GroupNumT.Int(group);

// vytvo�en� souboru skupiny TMP
#if defined(_INSTALL) || defined(COMPACT)
	GroupFile = _T("GRP") + GroupNumT + _T(".TMP");
	GroupFile2 = _T("GRP") + GroupNumT + _T(".DAT");
	GroupFileH = ::CreateFile(GroupFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	ASSERT(GroupFileH != INVALID_HANDLE_VALUE, "Nelze vytvo�it soubor " + GroupFile);
#else
	printf(CText("skupina ") + GroupNumT + CText("\n"));
#endif

// vyhled�n� soubor� v adres��i skupiny
	ReadDir(EmptyText);

// set��d�n� seznamu soubor�
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

// buffer k p�enesen� dat soubor�
#define BUFSIZE 0xf000

	BYTE* buf = (BYTE*)MemGet(BUFSIZE);
	ASSERT(buf, "Chyba pam�ti");
	long check;

// cyklus p�es v�echny soubory
	for (i = 0; i < BufFileN; i++)
	{
		check = 0;

// velikost dat souboru
		size = BufFile[i].Size;

// pln� jm�no souboru
		name = GroupNumT + _T('\\') + BufFile[i].Name;

// otev�en� souboru pro �ten�
		HANDLE file = ::CreateFile(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM |
							FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY, NULL);
		ASSERT(file != INVALID_HANDLE_VALUE, "Nelze otev��t soubor " + name);

// dokud jsou n�jak� data
		DWORD readwrite;

		while (size > 0)
		{
			int size0 = size;
			if (size0 > BUFSIZE) size0 = BUFSIZE;
			size -= size0;

			VERIFY(::ReadFile(file, buf, size0, &readwrite, NULL), "Chyba �ten� souboru " + name);
			ASSERT((int)readwrite == size0, "Chyba �ten� souboru " + name);

#if defined(_INSTALL) || defined(COMPACT)
			VERIFY(::WriteFile(GroupFileH, buf, size0, &readwrite, NULL), "Chyba z�pisu do souboru " + GroupFile);
			ASSERT((int)readwrite == size0, "Chyba z�pisu do souboru " + GroupFile);
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

// ov��en�, zda u� nejsou ��dn� data
		::ReadFile(file, buf, BUFSIZE, &readwrite, NULL);
		ASSERT((int)readwrite == 0, "Chyba �ten� souboru" + name);

// uzav�en� vstupn�ho souboru
		::CloseHandle(file);
	}

// zru�en� bufferu dat
	MemFree(buf);

// uzav�en� v�stupn�ho souboru skupiny
#if defined(_INSTALL) || defined(COMPACT)
	::CloseHandle(GroupFileH);

// komprimace souboru
	::DeleteFile(GroupFile2);
	ASSERT((int)::GetFileAttributes(GroupFile2) == -1, "Nelze zru�it soubor " + GroupFile2);
	Exec(_T("HLPGEN.EXE ") + GroupFile + _T(' ') + GroupFile2, EmptyText, TRUE);
	printf("\n");
	ASSERT((int)::GetFileAttributes(GroupFile2) != -1, "Nevytvo�en soubor " + GroupFile2);
	::DeleteFile(GroupFile);
	ASSERT((int)::GetFileAttributes(GroupFile) == -1, "Nelze zru�it soubor " + GroupFile);

// �schova velikosti komprimovan�ch dat skupiny
	GroupFileH = ::CreateFile(GroupFile2, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ASSERT(GroupFileH != INVALID_HANDLE_VALUE, "Nelze otev��t soubor " + GroupFile2);
	Head.Groups[GroupNum].SizeGroup = ::GetFileSize(GroupFileH, NULL);
	::CloseHandle(GroupFileH);
#else
	Head.Groups[GroupNum].SizeGroup = 0;
#endif

// za�azen� seznamu soubor�
	Head.Groups[GroupNum].Files = BufFileN;
	i = FilesS;
	FilesS = i + Head.Groups[GroupNum].SizeFiles;
	Files = (BYTE*)MemSize(Files, FilesS);
	ASSERT(Files, "Chyba pam�ti");
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

	ASSERT((dst - Files) == FilesS, "Nesouhlas� velikost seznamu soubor�");

// zru�en� seznamu soubor�
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
// na�ten� textu jako jm�no souboru

#ifdef _INSTALL

void ReadText(CText name)
{
// otev�en� souboru pro �ten�
	HANDLE file = ::CreateFile(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM |
						FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY, NULL);
	ASSERT(file != INVALID_HANDLE_VALUE, "Nelze otev��t soubor " + name);

// soubor PATH.TXT bude pou�it jako vzor pro datum a �as
	CText name1;
	name1 = name;
	name1.UpperCase();
	if (name1 == _T("0\\PATH.TXT"))
	{
		FILETIME datetime;
		::GetFileTime(file, NULL, NULL, &datetime);
		::FileTimeToLocalFileTime(&datetime, &Head.DateTime);
	}

// p��prava velikosti souboru
	int size = ::GetFileSize(file, NULL);

// zv��en� seznamu
	Head.Groups[0].Files++;
	int i = 2*sizeof(long) + sizeof(BYTE) + size;
	Head.Groups[0].SizeFiles += i;
	int dstoff = FilesS;
	FilesS = dstoff + i;
	Files = (BYTE*)MemSize(Files, FilesS);
	ASSERT(Files, "Chyba pam�ti");
	BYTE* dst = Files + dstoff;

// ulo�en� informac�
	*(long*)dst = 0;			// velikost
	dst += sizeof(long);
	*(long*)dst = 0;			// kontroln� sou�et
	dst += sizeof(long);
	ASSERT(size < 256, "P�ete�en� d�lky textu v " + name);
	*(BYTE*)dst = (BYTE)size;	// d�lka textu
	dst += sizeof(BYTE);

// na�ten� textu
	DWORD readwrite;
	VERIFY(::ReadFile(file, dst, size, &readwrite, NULL), "Chyba �ten� souboru " + name);
	ASSERT((int)readwrite == size, "Chyba �ten� souboru " + name);

// uzav�en� vstupn�ho souboru
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

// inicializace z�hlav� instala�n�ch dat
	MemFill(&Head, sizeof(Head), 0);
	Head.Ident[0] = 'S';
	Head.Ident[1] = 'E';
	Head.Ident[2] = 'T';
	Head.Ident[3] = GROUPSNUM;

#ifdef _INSTALL

// na�ten� informac� pro InstData m�d
	ReadText(_T("0\\Path.txt"));
	ReadText(_T("0\\Title.txt"));

// na�ten� soubor� skupin
	for (int i = 1; i < GROUPSNUM; i++)
#else
	for (int i = 0; i < GROUPSNUM; i++)
#endif
	{
		ReadGroup(i);
	}

// mus� b�t datum a �as
#ifdef _INSTALL
	ASSERT((Head.DateTime.dwLowDateTime != 0) || (Head.DateTime.dwLowDateTime != 0), "Chyb� soubor PATH.TXT");
#else
	ASSERT((Head.DateTime.dwLowDateTime != 0) || (Head.DateTime.dwLowDateTime != 0), "Chyb� soubor PETER.EXE");
#endif

// kontroln� sou�et z�hlav�
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

// kontroln� sou�et seznamu soubor�
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

// pracovn� buffer
	BYTE* buf = (BYTE*)MemGet(BUFSIZE);
	ASSERT(buf, "Chyba pam�ti");
	DWORD readwrite;

// vytvo�en� v�stupn�ho souboru
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

	ASSERT(file != INVALID_HANDLE_VALUE, "Chyba vytvo�en� souboru Setup.exe");

// otev�en� vstupn�ho souboru SETUP.EXE
	HANDLE file0 = ::CreateFile(_T("Setup\\Setup.exe"), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ASSERT(file0 != INVALID_HANDLE_VALUE, "Nenalezen soubor Setup\\Setup.exe");

// p��prava velikosti souboru SETUP.EXE
	int size = ::GetFileSize(file0, NULL);
#ifndef _INSTALL
	ASSERT(size > 66000, "Vadn� soubor SETUP.EXE");
#endif

// offset za��tku dat v souboru
	int dataoff = (size + (PAGEFILE-1)) & ~(PAGEFILE-1);

// na�ten� prvn�ho bloku dat
	int size0 = size;
	if (size0 > BUFSIZE) size0 = BUFSIZE;
	size -= size0;

	VERIFY(::ReadFile(file0, buf, size0, &readwrite, NULL), "Chyba �ten� ze souboru Setup\\Setup.exe");
	ASSERT((int)readwrite == size0, "Chyba �ten� ze souboru Setup\\Setup.exe");

// adresa z�hlav� NT
	CBuf bf;
	bf.Adr(buf);
	bf.Size(size0);
	IMAGE_NT_HEADERS* hdr = bf.NTHeader();
	ASSERT (hdr, "Chyba struktury Setup\\Setup.exe");

// p��prava virtu�ln� adresy dat
	int datavirt = (hdr->OptionalHeader.SizeOfImage + PAGESIZE-1) & ~(PAGESIZE-1);

// adresa vkl�dan� sekce
	IMAGE_SECTION_HEADER* sec = (IMAGE_SECTION_HEADER*)((BYTE*)(&hdr->OptionalHeader) 
		+ hdr->FileHeader.SizeOfOptionalHeader + hdr->FileHeader.NumberOfSections 
		* sizeof(IMAGE_SECTION_HEADER));

// zv��en� ��ta�e sekc�
	hdr->FileHeader.NumberOfSections++;

// nastaven� sekce
	MemCopy(sec, &SetupHeader, IMAGE_SIZEOF_SECTION_HEADER);

// nastaven� offsetu za��tku dat v souboru a virtu�ln� adresy
	sec->PointerToRawData = dataoff;
	sec->VirtualAddress = datavirt;

// nastaven� virtu�ln� velikosti dat
	int datasize = sizeof(Head) + FilesS;

#if defined(_INSTALL) || defined(COMPACT)
	for (i = 0; i < GROUPSNUM; i++)
	{
		datasize += Head.Groups[i].SizeGroup;
	}
#endif

	sec->Misc.VirtualSize = datasize;
	sec->SizeOfRawData = (datasize + 0x1ff) & ~0x1ff;

// nov� velikost obrazu cel�ho programu
	hdr->OptionalHeader.SizeOfImage = (datavirt + datasize + PAGESIZE-1) & ~(PAGESIZE-1);

// ulo�en� prvn�ho bloku dat
	VERIFY(::WriteFile(file, buf, size0, &readwrite, NULL), "Chyba z�pisu do souboru Setup.exe");
	ASSERT((int)readwrite == size0, "Chyba z�pisu do souboru Setup.exe");

// p�enesen� souboru SETUP.EXE
	while (size > 0)
	{
		size0 = size;
		if (size0 > BUFSIZE) size0 = BUFSIZE;
		size -= size0;

		VERIFY(::ReadFile(file0, buf, size0, &readwrite, NULL), "Chyba �ten� ze souboru Setup\\Setup.exe");
		ASSERT((int)readwrite == size0, "Chyba �ten� ze souboru Setup\\Setup.exe");
		VERIFY(::WriteFile(file, buf, size0, &readwrite, NULL), "Chyba z�pisu do souboru Setup.exe");
		ASSERT((int)readwrite == size0, "Chyba z�pisu do souboru Setup.exe");
	}

// ov��en�, zda u� nejsou ��dn� data
	::ReadFile(file0, buf, BUFSIZE, &readwrite, NULL);
	ASSERT(readwrite == 0, "Chyba �ten� ze souboru Setup\\Setup.exe");

// zarovn�n� d�lky programu
	i = dataoff - ::GetFileSize(file0, NULL);
	ASSERT(i >= 0, "Chyba �ten� ze souboru Setup\\Setup.exe");
	if (i > 0)
	{
		MemFill(buf, i, 0);
		::WriteFile(file, buf, i, &readwrite, NULL);
	}

// uzav�en� souboru SETUP.EXE
	::CloseHandle(file0);

// ulo�en� z�hlav�
	VERIFY(::WriteFile(file, &Head, sizeof(INSTHEAD), &readwrite, NULL), "Chyba z�pisu do souboru Setup.exe");
	ASSERT((int)readwrite == sizeof(Head), "Chyba z�pisu do souboru Setup.exe");

// ulo�en� seznamu soubor�
	VERIFY(::WriteFile(file, Files, FilesS, &readwrite, NULL), "Chyba z�pisu do souboru Setup.exe");
	ASSERT((int)readwrite == FilesS, "Chyba z�pisu do souboru Setup.exe");

// p�enesen� dat skupin
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
		ASSERT(file0 != INVALID_HANDLE_VALUE, "Chyba otev�en� souboru " + name);

		size = Head.Groups[i].SizeGroup;

		while (size > 0)
		{
			int size0 = size;
			if (size0 > BUFSIZE) size0 = BUFSIZE;
			size -= size0;

			VERIFY(::ReadFile(file0, buf, size0, &readwrite, NULL), "Chyba �ten� ze souboru " + name);
			ASSERT((int)readwrite == size0, "Chyba �ten� ze souboru " + name);
			VERIFY(::WriteFile(file, buf, size0, &readwrite, NULL), "Chyba z�pisu do souboru Setup.exe");
			ASSERT((int)readwrite == size0, "Chyba z�pisu do souboru Setup.exe");
		}

// ov��en�, zda u� nejsou ��dn� data
		::ReadFile(file0, buf, BUFSIZE, &readwrite, NULL);
		ASSERT(readwrite == 0, "Chyba �ten� ze souboru " + name);

// uzav�en� souboru skupiny
		::CloseHandle(file0);
	}

#endif

// zarovn�n� v�stupn�ho souboru na sektor
	size = ::SetFilePointer(file, 0, NULL, FILE_CURRENT);
	ASSERT(size > 0, "Chyba z�pisu do Setup.exe");
	i = ((size + 0x1ff) & ~0x1ff) - size;

	if (i > 0)
	{
		MemFill(buf, i, 0);
		::WriteFile(file, buf, i, &readwrite, NULL);
	}

// zru�en� bufferu dat
	MemFree(buf);

// nastaven� data a �asu souboru
	FILETIME datetime;
	::LocalFileTimeToFileTime(&Head.DateTime, &datetime);
	::SetFileTime(file, NULL, NULL, &datetime);

// zav�en� v�stupn�ho souboru
	::CloseHandle(file);

// zru�en� soubor� skupin
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

