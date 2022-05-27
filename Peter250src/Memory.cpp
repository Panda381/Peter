
#include "Main.h"

/***************************************************************************\
*																			*
*								Obsluha pam�ti								*
*																			*
\***************************************************************************/

/*
Organizace spr�vce pam�ti:
--------------------------
Pro dosa�en� maxim�ln� rychlosti p�id�lov�n� a uvol�ov�n� datov�ch blok� se
datov� bloky nep�id�luj� se skute�nou velikost�, ale p�id�l� se blok o velikosti
nejbli��� vy��� mocniny 2. Pou��vaj� se bloky o velikosti 8, 16, 32, 64, 128,
256, 512, 1024, 2048 a 4096 Bajt� - tedy 10 typ� blok� (pro procesory Alpha 
je�t� 8192 Bajt�). Datov� bloky o stejn� velikosti jsou organizov�ny do datov�ch
str�nek s evidenc� voln�ch blok�. To umo��uje snadn� a rychl� uvol�ov�n� a 
p�id�lov�n� blok� bez nebezpe�� neefektivn� fragmentace pam�ti.

P�i p�id�lov�n� nov�ho datov�ho bloku se pou�ije tabulka popisova�� datov�ch
modul�. Ka�d� popisova� popisuje jeden typ (velikost) datov�ho bloku, je tedy
pou�ito 10 (pro Alpha 11) popisova��. Popisova� pro p��slu�nou velikost datov�ho
bloku obsahuje ukazatel na prvn� voln� datov� blok dan� velikosti. Na za��tku
ka�d�ho voln�ho datov�ho bloku je ukazatel na dal�� a p�edch�zej�c� voln� datov�
blok stejn� velikosti. Je-li datov� blok pou�it, vy�ad� se z evidence voln�ch
blok� p�esm�rov�n�m ukazatel� mimo blok.

Nen�-li k dispozici ��dn� dal�� voln� datov� blok, vytvo�� se nov� str�nka
(stejn�ch) datov�ch blok�. Pou�ije se bu� rezervn� str�nka uvoln�n� p�i
p�ede�l�m ru�en� nebo je vytvo�ena nov� str�nka.

Oblast cel�ho adresov�ho prostoru (2 GB) je rozd�lena na oblasti - regiony.
Pro x86 je velikost regionu 1 MB, adresov� prostor tedy zahrnuje 2048 region�.
Pro ka�d� region si spr�vce pam�ti eviduje, zda je pou�it �i zda je voln�,
P�i po�adavku p�id�len� dal�� pam�ti je od syst�mu vy��d�na rezervace
regionu, kter� by m�l b�t podle evidence voln� (tj. je po�adov�n ur�it� adresov�
prostor). Pokud syst�m tento prostor pou��v�, je po�adov�na dal�� adresa a�
do nalezen� voln�ho prostoru.

Region je rozd�len do str�nek (pro x86 velikost 4 KB). Prvn� str�nka je v�dy
eviden�n� str�nka. Obsahuje ��ta� pou�it�ch str�nek a ukazatele pro spojen�
datov�ch str�nek do �et�zce voln�ch str�nek. P�i p�id�len� nov� datov� str�nky
je str�nka odpojena z �et�zce voln�ch datov�ch str�nek a od syst�mu je vy��d�no,
aby na adresu str�nky p�ipojil fyzickou pam�. Pot� jsou v�echny datov� bloky
na str�nce za�azeny do �et�zce datov�ch blok� pro po�adovanou velikost bloku.
*/

/////////////////////////////////////////////////////////////////////////////
// obsluha v�cetokov�ho re�imu

#ifdef _MT
#define HEAPSERIALIZE 0					// pro v�cetokov� re�im je serializace
#else
#define HEAPSERIALIZE HEAP_NO_SERIALIZE	// vypnut� serializace, nen�-li v�cetokov� re�im
#endif

#ifdef _MT
static	CRITICAL_SECTION	MemoryCriticalSection;	// kritick� sekce pro spr�vce pam�ti
#endif

/////////////////////////////////////////////////////////////////////////////
// debugger

#ifndef ASSERT

#ifdef _M_IX86
#define debugbreak _asm{int 3}
#else	// _M_IX86
#define debugbreak DebugBreak()
#endif	// _M_IX86

#if defined(_DEBUG) && !defined(_OPTIM)
#define ASSERT(f)			{ if (!(f)) debugbreak; }
#define VERIFY(f)			ASSERT(f)
#define ASSERTERROR			debugbreak
#else	// _DEBUG _OPTIM
#define ASSERT(f)			((void)0)
#define VERIFY(f)			((void)(f))
#define ASSERTERROR			((void)0)
#endif	// _DEBUG _OPTIM

#endif	// ASSERT

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
#define NUMREGS			(0x78000000/REGSIZE-1)	// po�et region� v pam�ti - bez syst�mov�ch region� (2039 pop�. 509)
#define REGSYSTEM		(0x800000/REGSIZE)		// po�et syst�mov�ch region� na za��tku (8 pop�. 2)
#define MAXREGS			((0x40000000/REGSIZE)*4)// po�et region� celkem (4096 pop�. 1024)

/////////////////////////////////////////////////////////////////////////////
// struktury

typedef struct ITEM_ ITEM;
typedef struct DATA_ DATA;
typedef struct PAGE_ PAGE;
typedef struct MODUL_ MODUL;
typedef struct REGION_ REGION;

// struktura datov�ho prvku - 8 bajt�
struct ITEM_
{
	ITEM*		NextItem;				// adresa dal��ho voln�ho prvku (NULL=konec)
	ITEM*		PrevItem;				// adresa p�ede�l�ho voln�ho prvku (NULL=za��tek)
};

// struktura dat str�nky - velikost PAGESIZE
struct DATA_
{
	ITEM		Items[PAGEITEMS];		// data jedn� str�nky
};

// struktura popisova�e str�nky - velikost 16 bajt�
// U voln� str�nky je neplatn� ukazatel modulu, u pou�it� ukazatele voln�ch str�nek
struct PAGE_
{
	PAGE*		NextPage;				// adresa dal�� str�nky (NULL=konec)
	PAGE*		PrevPage;				// adresa p�ede�l� str�nky (NULL=za��tek)
	MODUL*		Modul;					// adresa popisova�e modulu
	long		Used;					// ��ta� pou�it�ch polo�ek
};

// struktura popisova�e modulu
struct MODUL_
{
	ITEM*		FreeItem;				// adresa voln�ho prvku (NULL=nen�)
	PAGE*		ResPage;				// rezervn� str�nka (NULL=nen�)
	long		Size;					// velikost datov� polo�ky
};

// struktura regionu - velikost REGSIZE
struct REGION_
{
	long		Used;					// ��ta� pou�it�ch str�nek
	long		res[3];					// rezerva pro zarovn�n� na velikost popisova�e str�nky
	PAGE		Pages[DATAPAGES];		// popisova�e str�nek
	DATA		Data[DATAPAGES];		// data (datov� str�nky)
};


/////////////////////////////////////////////////////////////////////////////
// prom�nn�

BOOL MemoryError = FALSE;				// p��znak chyby pam�ti

BOOL MemoryOK = FALSE;					// p��znak platn�ho spr�vce pam�ti

static	HANDLE	hHeap = NULL;			// handle haldy

// obsluha region�
static	bool*	UsedRegion;				// buffer p��znak� pou�it� region�
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
					NULL,	NULL,	0x4000 };	// modul 0x4000 je jen pro rezervu


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� rezervn�ho regionu (p�i chyb� pam�ti vrac� FALSE)

static BOOL NewRegion()
{
// nalezen� nepou�it�ho regionu
	for (int i = NUMREGS;  i > 0; i--)
	{

// adresa zkou�en�ho regionu
		REGION* region = (REGION*)(NextRegion*REGSIZE); // adresa regionu
		bool* used = UsedRegion + NextRegion;	// p��znak pou�it� regionu

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
			REGION* newregion = (REGION*)::VirtualAlloc(region,
					REGSIZE, MEM_RESERVE, PAGE_READWRITE);

// vytvo�en� prvn� str�nky pro popisova� regionu
			if (newregion != NULL)
			{
				if ((newregion != region) || (::VirtualAlloc(region, PAGESIZE,
						MEM_COMMIT, PAGE_READWRITE) == NULL))
				{

// p�i chyb� uvoln�n� regionu, bude pokra�ov�no dal��m regionem
					::VirtualFree(newregion, 0, MEM_RELEASE);
				}
				else

// p��znak pou�it� regionu
				{
					*used = true;		// p��znak pou�it� regionu
					ResRegion = region;	// bude te� jako rezervn� region
					region->Used = 0;	// ��ta� pou�it�ch str�nek

// napojen� str�nek do �et�zce nep�ipojen�ch str�nek
					PAGE* page = NULL;
					PAGE* nextpage = region->Pages;

					for (i = DATAPAGES; i > 0; i--)
					{
						PAGE* prevpage = page;
						page = nextpage;
						nextpage++;
						page->PrevPage = prevpage;
						page->NextPage = nextpage;
					}

					page->NextPage = FreePage;
					FreePage = region->Pages;
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// zru�en� rezevn�ho regionu (str�nky mus� b�t uvoln�ny a odpojeny!)

static void DelRegion()
{
// kontrola, zda je rezervn� region
	REGION* region = ResRegion;
	if (region == NULL) return;
	ResRegion = NULL;

// odpojen� str�nek z �et�zce nep�ipojen�ch str�nek
	PAGE* page = region->Pages;
	for (int i = DATAPAGES; i > 0; i--)
	{
		PAGE* nextpage = page->NextPage;
		PAGE* prevpage = page->PrevPage;

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
	UsedRegion[(DWORD)region/REGSIZE] = false;

// odpojen� str�nky popisova�e regionu
	VERIFY (::VirtualFree(region, PAGESIZE, MEM_DECOMMIT));

// uvoln�n� regionu
	VERIFY (::VirtualFree(region, 0, MEM_RELEASE));
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� rezervn� str�nky pro dan� modul (p�i chyb� pam�ti vrac� FALSE)

static BOOL NewPage(MODUL* modul)
{
// vytvo�en� nov�ho regionu, nen�-li dal�� voln� str�nka
	if (FreePage == NULL)
	{
		if (!NewRegion())
		{
			return FALSE;
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
		return FALSE;
	}

// inkremetace pou�it�ch str�nek regionu
	region->Used++;						// inkrementace pou�it�ch str�nek

// zru�en� rezervace regiony
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
	ITEM* nextitem = data->Items;		// ukazatel adresy prvku
	ITEM* item = NULL;					// nen� p�ede�l� prvek
	for (int i = PAGESIZE/size; i > 0; i--)
	{
		ITEM* previtem = item;
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
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// zru�en� rezervn� str�nky (v�echny polo�ky mus� b�t uvoln�ny!)

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
	for (int i = PAGESIZE/size; i > 0; i--)
	{
		ITEM* nextitem = item->NextItem;
		ITEM* previtem = item->PrevItem;

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

// odpojen� dat str�nky
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
// vytvo�en� nov�ho datov�ho prvku pro dan� modul (p�i chyb� pam�ti vrac� NULL)

static ITEM* NewItem(MODUL* modul)
{
// vytvo�en� nov� str�nky, nen�-li voln� prvek
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

// inkrementace pou�it�ch polo�ek str�nky
	page->Used++;						// inkrementace pou�it�ch polo�ek

// zru�en� rezervace str�nky
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
// chybov� hl�en� - chyba pam�ti

static void MemErr()
{
	if (MemoryError) return;
	MemoryError = TRUE;

	::MessageBox(MainFrame, MemErrTextP, MemErrNadpisP, MB_OK | MB_ICONSTOP);
}


/////////////////////////////////////////////////////////////////////////////
// p�id�len� bloku pam�ti (povolena 0 - vr�t� mal� blok) (vrac� NULL=chyba)

void* _fastcall MemGet(int size)
{
// p�ednastaven� v�sledku operace na p��znak ne�sp�chu
	void* result = NULL;

// kontrola zadan� velikosti bloku
	ASSERT((DWORD)size < (DWORD)0x7ffffff0);
	if ((DWORD)size >= (DWORD)0x7ffffff0) 
	{
		MemErr();
		return NULL;
	}
	if (size == 0) size = 1;

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

// nedostatek pam�ti
	if (result == NULL)
	{
		MemErr();
	}

	return result;
}


/////////////////////////////////////////////////////////////////////////////
// uvoln�n� bloku pam�ti (povolena adresa NULL - ignoruje se)

void _fastcall MemFree(void* adr)
{
// adresa NULL se ignoruje
	if (adr == NULL) return;

// nele��-li ve vlastn�m regionu, uvoln�n� pomoc� haldy
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

// uvoln�n� polo�ky
	ASSERT(page->Used > 0);
	MODUL* modul = page->Modul;
	ASSERT(((DWORD)adr & (modul->Size-1)) == 0);
	DelItem(modul, (ITEM*)adr);

// odemknut� p��stupu k pam�ti
#ifdef _MT
	::LeaveCriticalSection(&MemoryCriticalSection);
#endif
}


/////////////////////////////////////////////////////////////////////////////
// zm�na velikosti bloku pam�ti (adresa NULL = vytvo�en�, velikost 0 = zru�en�)
// vrac� NULL=p��znak chyby, star� blok p�i chyb� nebude zm�n�n

void* _fastcall MemSize(void* adr, int size)
{
	void* newadr;

// p�i adrese NULL vytvo�en� nov� pam�ti
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

// kontrola, zda je adresa platn�
		ASSERT(page->Used > 0);
		MODUL* modul = page->Modul;
		int oldsize = modul->Size;
		ASSERT(((DWORD)adr & (oldsize-1)) == 0);

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

// p�enesen� bloku z haldy do mal�ch blok�
	if (size <= PAGESIZE/2)
	{
		int oldsize = (int)::HeapSize(hHeap, HEAPSERIALIZE, adr);
		ASSERT(oldsize != -1);
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
// inicializace spr�vce pam�ti (TRUE=inicializace OK)

BOOL MemInit()
{
// kontrola konstant
#pragma warning ( disable: 4127)				// hl�en� - konstantn� podm�nka
	ASSERT(sizeof(ITEM) == 8);
	ASSERT(sizeof(PAGE) == 16);
	ASSERT(sizeof(REGION) == REGSIZE);
	ASSERT((REGSIZE & 0xfffff) == 0);
	ASSERT(PAGESIZE <= 0x4000);
#pragma warning ( default: 4127)				// hl�en� - konstantn� podm�nka

// funkce m��e b�t vol�na jen jednou
	ASSERT(hHeap == NULL);
	ASSERT(UsedRegion == NULL);
	if ((hHeap != NULL) || (UsedRegion != NULL)) return FALSE;

// vytvo�en� haldy
	hHeap = ::HeapCreate(HEAPSERIALIZE, PAGESIZE, 0);
	if (hHeap == NULL) return FALSE;

// vytvo�en� bufferu p��znak� pou�it� region�
	UsedRegion = (bool*) ::HeapAlloc(hHeap, HEAPSERIALIZE, MAXREGS * sizeof(bool));
	if (UsedRegion == NULL) return FALSE;
	for (int i = 0; i < MAXREGS; i++) { UsedRegion[i] = false; };

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
	MemoryOK = FALSE;

// kontrola, zda je spr�vce pam�ti ji� ukon�en
	ASSERT(hHeap != NULL);
	if (hHeap == NULL) return;

// uvoln�n� haldy
	::HeapDestroy(hHeap);

// uvoln�n� uzamyk�n� p��stupu k pam�ti
#ifdef _MT
	::DeleteCriticalSection(&MemoryCriticalSection);
#endif
}


/////////////////////////////////////////////////////////////////////////////
// v�cetokov� inkrementace ��sla LONG INT

#if defined(_MT) && defined(_M_IX86)

void _fastcall LongIncrement(long* num)
{
	_asm 
	{
		INC		DWORD PTR [ECX]
	}
}

#endif


/////////////////////////////////////////////////////////////////////////////
// v�cetokov� dekrementace ��sla LONG INT (vrac� TRUE = v�sledek je 0)

#if defined(_MT) && defined(_M_IX86)
#pragma warning ( disable: 4035)

BOOL _fastcall LongDecrement(long* num)
{
	_asm 
	{
		XOR		EAX, EAX
		DEC		DWORD PTR [ECX]
		SETE	AL
	}
}

#pragma warning ( default: 4035)
#endif
