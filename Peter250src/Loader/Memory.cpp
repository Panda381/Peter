
#include "Main.h"

/***************************************************************************\
*																			*
*								Obsluha pam�ti								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// obsluha v�cevl�knov�ho re�imu

#ifdef _MT
#define HEAPSERIALIZE 0					// pro v�cevl�knov� re�im je serializace
#else
#define HEAPSERIALIZE HEAP_NO_SERIALIZE	// vypnut� serializace, nen�-li v�cevl�knov� re�im
#endif

#ifdef _MT
static	CRITICAL_SECTION	MemoryCriticalSection;	// kritick� sekce pro spr�vce pam�ti
#endif

/////////////////////////////////////////////////////////////////////////////
// konstanty - velikosti mus� b�t mocnina 2 !

#ifndef _M_ALPHA
#define	PAGESIZE		0x1000			// velikost aloka�n� str�nky pro ostatn� procesory (4096)
#else
#define	PAGESIZE		0x2000			// velikost aloka�n� str�nky pro procesor Alpha (8192)
#endif

#define PAGEITEMS		(PAGESIZE/sizeof(ITEM))	// po�et prvk� na str�nku (512 pop�. 1024)
#define	REGPAGES		(PAGESIZE/sizeof(PAGE)) // po�et str�nek na region (256 pop�. 512)
#define DATAPAGES		(REGPAGES-1)			// po�et datov�ch str�nek na region (255 pop�. 511)
#define REGSIZE			(PAGESIZE*REGPAGES)		// velikost regionu (1 MB pop�. 4 MB)
#define NUMREGS			(0x78000000/REGSIZE-1)	// po�et region� v pam�ti (2039 pop�. 509)
#define REGSYSTEM		(0x800000/REGSIZE)		// po�et syst�mov�ch region� na za��tku (8 pop�. 2)
#define MAXREGS			((0x40000000/REGSIZE)*4)// po�et region� celkem (4096 pop�. 1024)

/////////////////////////////////////////////////////////////////////////////
// struktury

struct MODUL_;

// struktura datov�ho prvku - 8 bajt�
typedef struct ITEM_
{
	ITEM_*		NextItem;				// adresa dal��ho voln�ho prvku (NULL=konec)
	ITEM_*		PrevItem;				// adresa p�ede�l�ho voln�ho prvku (NULL=za��tek)
} ITEM;

// struktura dat str�nky - velikost PAGESIZE
typedef struct DATA_
{
	ITEM		Items[PAGEITEMS];		// data jedn� str�nky
} DATA;

// struktura popisova�e str�nky - velikost 16 bajt�
typedef struct PAGE_
{
	PAGE_*		NextPage;				// adresa dal�� str�nky (NULL=konec)
	PAGE_*		PrevPage;				// adresa p�ede�l� str�nky (NULL=za��tek)
	MODUL_*		Modul;					// adresa popisova�e modulu
	long		Used;					// ��ta� pou�it�ch polo�ek
} PAGE;

// struktura popisova�e modulu
typedef struct MODUL_
{
	ITEM*		FreeItem;				// adresa voln�ho prvku
	PAGE*		ResPage;				// rezervn� str�nka (NULL=nen�)
	long		Size;					// velikost datov� polo�ky
} MODUL;

// struktura regionu - velikost REGSIZE
typedef struct REGION_
{
	long		Used;					// ��ta� pou�it�ch str�nek
	long		res[3];					// rezerva pro zarovn�n�
	PAGE		Pages[DATAPAGES];		// popisova�e str�nek
	DATA		Data[DATAPAGES];		// data
} REGION;


/////////////////////////////////////////////////////////////////////////////
// prom�nn�

static	HANDLE	hHeap = NULL;			// handle haldy

bool MemoryOK = false;					// p��znak platn�ho spr�vce pam�ti

// obsluha region�
static	char*	UsedRegion;				// buffer p��znak� pou�it� region�
static	int		NextRegion = REGSYSTEM;	// index p��t� p�id�lovan�ho regionu
static	REGION*	ResRegion = NULL;		// rezervn� region (NULL=nen�)

// obsluha nep�ipojen�ch str�nek (n�sleduje �et�zec pomoc� NextPage)
static	PAGE*	FreePage = NULL;		// prvn� nep�ipojen� str�nka (NULL=nen�)

// popisova�e datov�ch modul�
static	MODUL	Modul[12] = {
					NULL,	NULL,	0x0008,
					NULL,	NULL,	0x0010,
					NULL,	NULL,	0x0020,
					NULL,	NULL,	0x0040,
					NULL,	NULL,	0x0080,
					NULL,	NULL,	0x0100,
					NULL,	NULL,	0x0200,
					NULL,	NULL,	0x0400,
					NULL,	NULL,	0x0800,
					NULL,	NULL,	0x1000,
					NULL,	NULL,	0x2000,
					NULL,	NULL,	0x4000 };


/////////////////////////////////////////////////////////////////////////////
// prototypy lok�ln�ch funkc�

static ITEM* NewItem(MODUL* modul);				// vytvo�en� nov�ho datov�ho prvku
static void DelItem(MODUL* modul, ITEM* item);	// zru�en� datov�ho prvku
static bool NewPage(MODUL* modul);				// vytvo�en� nov� str�nky
static void DelPage(MODUL* modul);				// zru�en� rezervn� str�nky
static bool NewRegion();						// vytvo�en� nov�ho regionu
static void DelRegion();						// zru�en� rezervn�ho regionu


/////////////////////////////////////////////////////////////////////////////
// inicializace spr�vce pam�ti (TRUE=inicializace OK)

bool MemInit()
{
// kontrola konstant
#pragma warning ( disable: 4127)				// hl�en� - konstantn� podm�nka
	ASSERT(sizeof(ITEM) == 8);
	ASSERT(sizeof(PAGE) == 16);
	ASSERT(sizeof(REGION) == REGSIZE);
	ASSERT((REGSIZE & 0xfffff) == 0);
	ASSERT(PAGESIZE <= 0x4000);
	ASSERT(FALSE == 0);
#pragma warning ( default: 4127)				// hl�en� - konstantn� podm�nka

// funkce m��e b�t vol�na jen jednou
	ASSERT(hHeap == NULL);
	if (hHeap != NULL) return true;

// vytvo�en� haldy
	hHeap = ::HeapCreate(HEAPSERIALIZE, PAGESIZE, 0);
	if (hHeap == NULL) return false;

// vytvo�en� bufferu p��znak� pou�it� region�
	UsedRegion = (char*) ::HeapAlloc(hHeap, HEAP_ZERO_MEMORY | HEAPSERIALIZE, MAXREGS);
	if (UsedRegion == NULL) return false;

// inicializace uzamyk�n� p��stupu k pam�ti
#ifdef _MT
	::InitializeCriticalSection(&MemoryCriticalSection);
#endif

// vytvo�en� prvn�ho regionu (pro zkou�ku pam�ti)
	MemoryOK = NewRegion();
	return MemoryOK;
}


/////////////////////////////////////////////////////////////////////////////
// ukon�en� spr�vce pam�ti

void MemTerm()
{
	MemoryOK = false;

// kontrola, zda je spr�vce pam�ti ji� ukon�en
	ASSERT(hHeap != NULL);
	if (hHeap == NULL) return;

// uvoln�n� haldy
	::HeapDestroy(hHeap);
	hHeap = NULL;

// uvoln�n� uzamyk�n� p��stupu k pam�ti
#ifdef _MT
	::DeleteCriticalSection(&MemoryCriticalSection);
#endif
}


/////////////////////////////////////////////////////////////////////////////
// chyba pam�ti

void MemErr()
{
	::MessageBox(NULL, JazykInfo[Jazyk].MemErrText, JazykInfo[Jazyk].MemErrNadp, MB_OK | MB_ICONSTOP);
	Exit(EXITCODE_MEMERR);
}


/////////////////////////////////////////////////////////////////////////////
// p�id�len� bloku pam�ti (povolena 0) NULL=chyba

void* _fastcall MemGet(int size)
{
	void* result = NULL;

// kontrola zadan� velikosti bloku
	ASSERT((size >= 0) && (size < 0x7ffffff0));
	if (size <= 0) size = 1;

// bude p�id�len� mal�ho bloku
	if (size <= PAGESIZE)
	{

// uzamknut� p��stupu k pam�ti
#ifdef _MT
		::EnterCriticalSection(&MemoryCriticalSection);
#endif

// ur�en� modulu velikosti prvku
		MODUL* modul = Modul;		// za��tek popisova�� modul�
		while (size > modul->Size)
		{
			modul++;				// zv��en� ukazatele modul�
		}

// vytvo�en� datov�ho prvku
		result = (void*)NewItem(modul);
		ASSERT(result != NULL);

// odemknut� p��stupu k pam�ti
#ifdef _MT
		::LeaveCriticalSection(&MemoryCriticalSection);
#endif
	}

// p�id�len� bloku z haldy, pokud nebyl blok p�id�len
	if (result == NULL)
	{
		result = ::HeapAlloc(hHeap, HEAPSERIALIZE, (size + 0xff) & ~0xff);
	}

	if (result == NULL) MemErr();	// chyba pam�ti

	return result;
}


/////////////////////////////////////////////////////////////////////////////
// uvoln�n� bloku pam�ti (povoleno NULL)

void _fastcall MemFree(void* adr)
{
// adresa NULL se ignoruje
	if (adr == NULL) return;

// nele��-li ve vlastn�m regionu, uvoln�n� pomoc� haldy (je�t� nen� nutn� uzamknut�!)
	if (!UsedRegion[(DWORD)adr/REGSIZE])
	{
		::HeapFree(hHeap, HEAPSERIALIZE, adr);
		return;
	}

// uzamknut� p��stupu k pam�ti
#ifdef _MT
	::EnterCriticalSection(&MemoryCriticalSection);
#endif

// p��prava ukazatel�
	REGION* region = (REGION*)((DWORD)adr & ~(REGSIZE-1)); // adresa regionu
	DATA* data = (DATA*)((DWORD)adr & ~(PAGESIZE-1)); // data
	PAGE* page = (PAGE*)((DWORD)region +
		((DWORD)data & (REGSIZE-1)) / (sizeof(DATA)/sizeof(PAGE))); // str�nka

// kontrola, zda je str�nka platn�
	if (page->Used > 0)
	{

// kontrola, zda adresa le�� na platn� hranici
		MODUL* modul = page->Modul;
		if (((DWORD)adr & (modul->Size-1)) == 0)
		{

// uvoln�n� polo�ky
			DelItem(modul, (ITEM*)adr);
		}
	}

// odemknut� p��stupu k pam�ti
#ifdef _MT
	::LeaveCriticalSection(&MemoryCriticalSection);
#endif
}


/////////////////////////////////////////////////////////////////////////////
// zm�na velikosti bloku pam�ti (povolena adresa NULL i velikost 0, p�i chyb� nezm�n�no)
// Pozn.: zde nen� nutn� uzamykat pam� (zna�n� to zpomaluje) !!!

void* _fastcall MemSize(void* adr, int size)
{
	int oldsize;
	void* newadr;

// p�i adrese NULL alokace pam�ti
	if (adr == NULL)
	{
		if (size <= 0) return NULL;
		return MemGet(size);
	}

// p�i velikosti 0 uvoln�n� bloku
	if (size <= 0)
	{
		MemFree(adr);
		return NULL;
	}

// kontrola, zda blok le�� ve vlastn�m regionu
	if (UsedRegion[(DWORD)adr/REGSIZE])
	{

// p��prava ukazatel�
		REGION* region = (REGION*)((DWORD)adr & ~(REGSIZE-1)); // adresa regionu
		DATA* data = (DATA*)((DWORD)adr & ~(PAGESIZE-1)); // data
		PAGE* page = (PAGE*)((DWORD)region +
			((DWORD)data & (REGSIZE-1)) / (sizeof(DATA)/sizeof(PAGE))); // str�nka

// kontrola, zda je str�nka platn�
		if (page->Used > 0)
		{

// kontrola, zda adresa le�� na platn� hranici
			MODUL* modul = page->Modul;
			oldsize = modul->Size;

			if (((DWORD)adr & (oldsize-1)) == 0)
			{

// test, zda je pot�eba velikost m�nit
				if (((size <= 8) && (oldsize <= 8)) ||
					((size <= oldsize) && (size > oldsize/4)))
				{
					return adr;					// adresa nezm�n�na
				}

// vytvo�en� nov�ho bloku
				newadr = MemGet(size);

// kopie star�ch dat bloku
				if (newadr != NULL)
				{
					if (size >= oldsize)
					{
						MemCopy(newadr, adr, oldsize);
					}
					else
					{
						MemCopy(newadr, adr, size);
					}
					MemFree(adr);
				}
				return newadr;
			}

// neplatn� adresa - chybov� n�vrat
			else
			{
				return NULL;
			}
		}

// neplatn� str�nka - chybov� n�vrat
		else
		{
			return NULL;
		}
	}

// p�enesen� bloku z haldy do mal�ch blok�
	if (size <= PAGESIZE/2)
	{
		oldsize = (int)::HeapSize(hHeap, HEAPSERIALIZE, adr);
		if (oldsize == -1)
		{
			MemErr();
			return NULL;
		}

		newadr = MemGet(size);

		if (newadr != NULL)
		{
			if (size >= oldsize)
			{
				MemCopy(newadr, adr, oldsize);
			}
			else
			{
				MemCopy(newadr, adr, size);
			}
			::HeapFree(hHeap, HEAPSERIALIZE, adr);
		}
		return newadr;
	}

// realokace bloku v hald�
	newadr = ::HeapReAlloc(hHeap, HEAPSERIALIZE, adr, (size + 0xff) & ~0xff);
	if (newadr == NULL) MemErr();	// chyba pam�ti
	return newadr;
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� nov�ho datov�ho prvku

static ITEM* NewItem(MODUL* modul)
{
// zaji�t�n� platn�ho prvku
	if (modul->FreeItem == NULL)
	{
		if (!NewPage(modul))
		{
			return NULL;
		}
	}

// ur�en� adresy popisova�e str�nky
	ITEM* item = modul->FreeItem;		// adresa prvku
	REGION* region = (REGION*)((DWORD)item & ~(REGSIZE-1)); // adresa regionu
	DATA* data = (DATA*)((DWORD)item & ~(PAGESIZE-1)); // data
	PAGE* page = (PAGE*)((DWORD)region +
		((DWORD)data & (REGSIZE-1)) / (sizeof(DATA)/sizeof(PAGE))); // str�nka

// inkrementace pou�it�ch prom�nn�ch str�nky, zru�en� rezervace str�nky
	page->Used++;						// inkrementace pou�it�ch polo�ek
	if (page == modul->ResPage)			// je to rezervn� str�nka?
	{
		modul->ResPage = NULL;			// zru�en� rezervn� str�nky
	}

// vy�azen� prvku z �et�zce voln�ch prvk�
	ITEM* nextitem = item->NextItem;	// adresa dal��ho prvku
	modul->FreeItem = nextitem;
	if (nextitem != NULL)
	{
		nextitem->PrevItem = NULL;
	}
	return item;
}


/////////////////////////////////////////////////////////////////////////////
// zru�en� datov�ho prvku

static void DelItem(MODUL* modul, ITEM* item)
{
// p��prava adres popisova��
	REGION* region = (REGION*)((DWORD)item & ~(REGSIZE-1)); // adresa regionu
	DATA* data = (DATA*)((DWORD)item & ~(PAGESIZE-1)); // data
	PAGE* page = (PAGE*)((DWORD)region +
		((DWORD)data & (REGSIZE-1)) / (sizeof(DATA)/sizeof(PAGE))); // str�nka

// za�azen� do �et�zce voln�ch prvk�
	item->PrevItem = NULL;
	item->NextItem = modul->FreeItem;
	if (item->NextItem != NULL)
	{
		item->NextItem->PrevItem = item;
	}
	modul->FreeItem = item;

// dekrementace pou�it�ch prvk� str�nky, p��p. uvoln�n� str�nky
	page->Used--;
	if (page->Used == 0)
	{
		if (modul->ResPage != NULL)
		{
			DelPage(modul);
		}
		modul->ResPage = page;
	}
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� rezervn� str�nky

static bool NewPage(MODUL* modul)
{
// zaji�t�n� p�ipojiteln� str�nky
	if (FreePage == NULL)
	{
		if (!NewRegion())
		{
			return false;
		}
	}

// ur�en� adresy dat p�ipojovan� str�nky
	PAGE* page = FreePage;				// p�ipojovan� str�nka
	REGION* region = (REGION*)((DWORD)page & ~(REGSIZE-1)); // adresa regionu
	DATA* data = (DATA*)((DWORD)region + 
		((DWORD)page & (REGSIZE-1)) * (sizeof(DATA)/sizeof(PAGE))); // data

// p�ipojen� dat str�nky
	if (::VirtualAlloc(data, PAGESIZE,
			MEM_COMMIT, PAGE_READWRITE) == NULL)
	{
		return false;
	}

// inkremetace pou�it�ch str�nek regionu, zru�en� rezervace regiony
	region->Used++;						// inkrementace pou�it�ch str�nek
	if (region == ResRegion)			// je to rezervn� region?
	{
		ResRegion = NULL;				// zru�en� rezervn�ho regionu
	}

// vy�azen� str�nky z �et�zce nep�ipojen�ch str�nek
	PAGE* nextpage = page->NextPage;
	FreePage = nextpage;
	if (nextpage != NULL)
	{
		nextpage->PrevPage = NULL;
	}

// napojen� do �et�zce voln�ch polo�ek modulu
	int size = modul->Size;				// velikost datov� polo�ky
	ITEM* previtem;
	ITEM* nextitem = data->Items;		// ukazatel adresy prvku
	ITEM* item = NULL;					// nen� p�ede�l� prvek
	for (int i = PAGESIZE/size; i > 0; i--)
	{
		previtem = item;
		item = nextitem;
		nextitem = (ITEM*)((char*)nextitem + size);
		item->PrevItem = previtem;
		item->NextItem = nextitem;
	}
	item->NextItem = modul->FreeItem;
	modul->FreeItem = data->Items;

// inicializace polo�ek popisova�e str�nky
	page->Modul = modul;				// adresa modulu
	page->Used = 0;						// po�et pou�it�ch polo�ek

// nastaven� str�nky jako rezervn�
	modul->ResPage = page;				// bude te� jako rezervn� str�nka
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// zru�en� rezervn� str�nky

static void DelPage(MODUL* modul)
{
// kontrola, zda je rezervn� str�nka
	PAGE* page = modul->ResPage;
	if (page == NULL) return;
	modul->ResPage = NULL;

// p��prava adresy regionu a dat
	REGION* region = (REGION*)((DWORD)page & ~(REGSIZE-1)); // adresa regionu
	DATA* data = (DATA*)((DWORD)region + 
		((DWORD)page & (REGSIZE-1)) * (sizeof(DATA)/sizeof(PAGE))); // data

// odpojen� z �et�zce voln�ch polo�ek
	int size = modul->Size;
	ITEM* item = data->Items;
	ITEM* nextitem;
	ITEM* previtem;
	for (int i = PAGESIZE/size; i > 0; i--)
	{
		nextitem = item->NextItem;
		previtem = item->PrevItem;

		if (previtem != NULL)
		{
			previtem->NextItem = nextitem;
		}
		else
		{
			modul->FreeItem = nextitem;
		}

		if (nextitem != NULL)
		{
			nextitem->PrevItem = previtem;
		}
		item = (ITEM*)((char*)item + size);
	}

// odpojen� str�nky
	VERIFY (::VirtualFree(data, PAGESIZE, MEM_DECOMMIT));

// napojen� str�nky do �et�zce nep�ipojen�ch str�nek
	page->PrevPage = NULL;
	page->NextPage = FreePage;
	if (FreePage != NULL)
	{
		FreePage->PrevPage = page;
	}
	FreePage = page;
	
// sn�en� ��ta�e pou�it�ch str�nek regionu, p��p. uvoln�n� regionu
	region->Used--;
	if (region->Used == 0)
	{
		if (ResRegion != NULL)
		{
			DelRegion();		// uvoln�n� rezervn�ho regionu
		}
		ResRegion = region;		// je to nyn� rezervn� region
	}
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� rezervn�ho regionu

static bool NewRegion()
{
// lok�ln� prom�nn�
	REGION* region;						// ukazatel adresy regionu
	PAGE*	page;						// ukazatel adresy str�nky
	PAGE*	nextpage;					// pomocn� ukazatel - p��t� str�nka
	PAGE*	prevpage;					// pomocn� ukazatel - p�edchoz� str�nka
	char*	used;						// ukazatel pou�it�ch region�
	int i;								// pomocn� ��ta�

// nalezen� nepou�it�ho regionu
	for (i = NUMREGS;  i > 0; i--)
	{

// adresa zkou�en�ho regionu
		region = (REGION*)(void*)(NextRegion*REGSIZE); // adresa regionu
		used = &(UsedRegion[NextRegion]);	// p��znak pou�it� regionu

// posun indexu p��t�ho regionu
		NextRegion++;				// zv��en� indexu regionu
		if (NextRegion >= REGSYSTEM+NUMREGS)
		{
			NextRegion = REGSYSTEM;
		}
		
// test, zda je region pou�it�		
		if (!*used)
		{

// rezervov�n� nov�ho regionu
			region = (REGION*)::VirtualAlloc(region,
					REGSIZE, MEM_RESERVE, PAGE_READWRITE);

// vytvo�en� prvn� str�nky pro popisova� regionu
			if (region != NULL)
			{
				if (::VirtualAlloc(region, PAGESIZE,
						MEM_COMMIT, PAGE_READWRITE) == NULL)
				{

// p�i chyb� uvoln�n� regionu a hl�en� chyby
					::VirtualFree(region, 0, MEM_RELEASE);
					return false;
				}

// p��znak pou�it� regionu
				*used = TRUE;		// p��znak pou�it� regionu
				ResRegion = region;	// bude te� jako rezervn� region
				region->Used = 0;		// ��ta� pou�it�ch str�nek

// napojen� str�nek do �et�zce nep�ipojen�ch str�nek
				page = NULL;
				nextpage = region->Pages;
				for (i = DATAPAGES; i > 0; i--)
				{
					prevpage = page;
					page = nextpage;
					nextpage++;
					page->PrevPage = prevpage;
					page->NextPage = nextpage;
				}
				page->NextPage = FreePage;
				FreePage = region->Pages;
				return true;
			}
		}
	}
	return false;
}


/////////////////////////////////////////////////////////////////////////////
// zru�en� rezevn�ho regionu

static void DelRegion()
{
// kontrola, zda je rezervn� region
	REGION* region = ResRegion;
	if (region == NULL) return;
	ResRegion = NULL;

// odpojen� str�nek z �et�zce nep�ipojen�ch str�nek
	PAGE* page = region->Pages;
	PAGE* nextpage;
	PAGE* prevpage;
	for (int i = DATAPAGES; i > 0; i--)
	{
		nextpage = page->NextPage;
		prevpage = page->PrevPage;

		if (prevpage != NULL)
		{
			prevpage->NextPage = nextpage;
		}
		else
		{
			FreePage = nextpage;
		}

		if (nextpage != NULL)
		{
			nextpage->PrevPage = prevpage;
		}
		page++;
	}

// zru�en� p��znaku pou�it� regionu
	UsedRegion[(DWORD)region/REGSIZE] = FALSE;

// odpojen� str�nky popisova�e regionu
	VERIFY (::VirtualFree(region, PAGESIZE, MEM_DECOMMIT));

// uvoln�n� regionu
	VERIFY (::VirtualFree(region, 0, MEM_RELEASE));
}


/////////////////////////////////////////////////////////////////////////////
// v�cetokov� inkrementace ��sla LONG INT

#ifdef _MT
#ifdef _M_IX86

void _fastcall LongIncrement(long* num)
{
	_asm 
	{
		INC		DWORD PTR [ECX]
	}
}

#endif	// _M_IX86
#endif	// _MT


/////////////////////////////////////////////////////////////////////////////
// v�cetokov� dekrementace ��sla LONG INT (vrac� true = v�sledek je 0)

#ifdef _MT
#ifdef _M_IX86

#pragma warning ( disable: 4035)
bool _fastcall LongDecrement(long* num)
{
	_asm 
	{
//		XOR		EAX, EAX
		DEC		DWORD PTR [ECX]
		SETE	AL
//		JNZ		X1
//		INC		EAX
//	X1:
	}
}
#pragma warning ( default: 4035)

#endif	// _M_IX86
#endif	// _MT
