
#include "Main.h"

/***************************************************************************\
*																			*
*								Obsluha pamìti								*
*																			*
\***************************************************************************/

/*
Organizace správce pamìti:
--------------------------
Pro dosažení maximální rychlosti pøidìlování a uvolòování datových blokù se
datové bloky nepøidìlují se skuteènou velikostí, ale pøidìlí se blok o velikosti
nejbližší vyšší mocniny 2. Používají se bloky o velikosti 8, 16, 32, 64, 128,
256, 512, 1024, 2048 a 4096 Bajtù - tedy 10 typù blokù (pro procesory Alpha 
ještì 8192 Bajtù). Datové bloky o stejné velikosti jsou organizovány do datových
stránek s evidencí volných blokù. To umožòuje snadné a rychlé uvolòování a 
pøidìlování blokù bez nebezpeèí neefektivní fragmentace pamìti.

Pøi pøidìlování nového datového bloku se použije tabulka popisovaèù datových
modulù. Každý popisovaè popisuje jeden typ (velikost) datového bloku, je tedy
použito 10 (pro Alpha 11) popisovaèù. Popisovaè pro pøíslušnou velikost datového
bloku obsahuje ukazatel na první volný datový blok dané velikosti. Na zaèátku
každého volného datového bloku je ukazatel na další a pøedcházející volný datový
blok stejné velikosti. Je-li datový blok použit, vyøadí se z evidence volných
blokù pøesmìrováním ukazatelù mimo blok.

Není-li k dispozici žádný další volný datový blok, vytvoøí se nová stránka
(stejných) datových blokù. Použije se buï rezervní stránka uvolnìná pøi
pøedešlém rušení nebo je vytvoøena nová stránka.

Oblast celého adresového prostoru (2 GB) je rozdìlena na oblasti - regiony.
Pro x86 je velikost regionu 1 MB, adresový prostor tedy zahrnuje 2048 regionù.
Pro každý region si správce pamìti eviduje, zda je použit èi zda je volný,
Pøi požadavku pøidìlení další pamìti je od systému vyžádána rezervace
regionu, který by mìl být podle evidence volný (tj. je požadován urèitý adresový
prostor). Pokud systém tento prostor používá, je požadována další adresa až
do nalezení volného prostoru.

Region je rozdìlen do stránek (pro x86 velikost 4 KB). První stránka je vždy
evidenèní stránka. Obsahuje èítaè použitých stránek a ukazatele pro spojení
datových stránek do øetìzce volných stránek. Pøi pøidìlení nové datové stránky
je stránka odpojena z øetìzce volných datových stránek a od systému je vyžádáno,
aby na adresu stránky pøipojil fyzickou pamì. Poté jsou všechny datové bloky
na stránce zaøazeny do øetìzce datových blokù pro požadovanou velikost bloku.
*/

/////////////////////////////////////////////////////////////////////////////
// obsluha vícetokového režimu

#ifdef _MT
#define HEAPSERIALIZE 0					// pro vícetokový režim je serializace
#else
#define HEAPSERIALIZE HEAP_NO_SERIALIZE	// vypnutí serializace, není-li vícetokový režim
#endif

#ifdef _MT
static	CRITICAL_SECTION	MemoryCriticalSection;	// kritická sekce pro správce pamìti
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
// konstanty - velikosti musí být mocnina 2 !

#ifndef _M_ALPHA
#define	PAGESIZE		0x1000			// velikost alokaèní stránky pro ostatní procesory (4096)
#else
#define	PAGESIZE		0x2000			// velikost alokaèní stránky pro procesor Alpha (8192)
#endif

#define PAGEITEMS		(PAGESIZE/sizeof(ITEM))	// poèet prvkù na stránku (512 popø. 1024)
#define	REGPAGES		(PAGESIZE/sizeof(PAGE)) // poèet stránek na region (256 popø. 512)
#define DATAPAGES		(REGPAGES-1)			// poèet datových stránek na region (255 popø. 511)
#define REGSIZE			(PAGESIZE*REGPAGES)		// velikost regionu (1 MB popø. 4 MB)
#define NUMREGS			(0x78000000/REGSIZE-1)	// poèet regionù v pamìti - bez systémových regionù (2039 popø. 509)
#define REGSYSTEM		(0x800000/REGSIZE)		// poèet systémových regionù na zaèátku (8 popø. 2)
#define MAXREGS			((0x40000000/REGSIZE)*4)// poèet regionù celkem (4096 popø. 1024)

/////////////////////////////////////////////////////////////////////////////
// struktury

typedef struct ITEM_ ITEM;
typedef struct DATA_ DATA;
typedef struct PAGE_ PAGE;
typedef struct MODUL_ MODUL;
typedef struct REGION_ REGION;

// struktura datového prvku - 8 bajtù
struct ITEM_
{
	ITEM*		NextItem;				// adresa dalšího volného prvku (NULL=konec)
	ITEM*		PrevItem;				// adresa pøedešlého volného prvku (NULL=zaèátek)
};

// struktura dat stránky - velikost PAGESIZE
struct DATA_
{
	ITEM		Items[PAGEITEMS];		// data jedné stránky
};

// struktura popisovaèe stránky - velikost 16 bajtù
// U volné stránky je neplatný ukazatel modulu, u použité ukazatele volných stránek
struct PAGE_
{
	PAGE*		NextPage;				// adresa další stránky (NULL=konec)
	PAGE*		PrevPage;				// adresa pøedešlé stránky (NULL=zaèátek)
	MODUL*		Modul;					// adresa popisovaèe modulu
	long		Used;					// èítaè použitých položek
};

// struktura popisovaèe modulu
struct MODUL_
{
	ITEM*		FreeItem;				// adresa volného prvku (NULL=není)
	PAGE*		ResPage;				// rezervní stránka (NULL=není)
	long		Size;					// velikost datové položky
};

// struktura regionu - velikost REGSIZE
struct REGION_
{
	long		Used;					// èítaè použitých stránek
	long		res[3];					// rezerva pro zarovnání na velikost popisovaèe stránky
	PAGE		Pages[DATAPAGES];		// popisovaèe stránek
	DATA		Data[DATAPAGES];		// data (datové stránky)
};


/////////////////////////////////////////////////////////////////////////////
// promìnné

BOOL MemoryError = FALSE;				// pøíznak chyby pamìti

BOOL MemoryOK = FALSE;					// pøíznak platného správce pamìti

static	HANDLE	hHeap = NULL;			// handle haldy

// obsluha regionù
static	bool*	UsedRegion;				// buffer pøíznakù použití regionù
static	int		NextRegion = REGSYSTEM;	// index pøíštì pøidìlovaného regionu
static	REGION*	ResRegion = NULL;		// rezervní region (NULL=není)

// obsluha nepøipojených stránek (následuje øetìzec pomocí NextPage)
static	PAGE*	FreePage = NULL;		// první nepøipojená stránka (NULL=není)

// popisovaèe datových modulù
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
// vytvoøení rezervního regionu (pøi chybì pamìti vrací FALSE)

static BOOL NewRegion()
{
// nalezení nepoužitého regionu
	for (int i = NUMREGS;  i > 0; i--)
	{

// adresa zkoušeného regionu
		REGION* region = (REGION*)(NextRegion*REGSIZE); // adresa regionu
		bool* used = UsedRegion + NextRegion;	// pøíznak použití regionu

// posun indexu pøíštího regionu
		NextRegion++;				// zvýšení indexu regionu
		if (NextRegion >= REGSYSTEM+NUMREGS)
		{
			NextRegion = REGSYSTEM;
		}
		
// test, zda je region použitý		
		if (!*used)
		{

// rezervování nového regionu
			REGION* newregion = (REGION*)::VirtualAlloc(region,
					REGSIZE, MEM_RESERVE, PAGE_READWRITE);

// vytvoøení první stránky pro popisovaè regionu
			if (newregion != NULL)
			{
				if ((newregion != region) || (::VirtualAlloc(region, PAGESIZE,
						MEM_COMMIT, PAGE_READWRITE) == NULL))
				{

// pøi chybì uvolnìní regionu, bude pokraèováno dalším regionem
					::VirtualFree(newregion, 0, MEM_RELEASE);
				}
				else

// pøíznak použití regionu
				{
					*used = true;		// pøíznak použití regionu
					ResRegion = region;	// bude teï jako rezervní region
					region->Used = 0;	// èítaè použitých stránek

// napojení stránek do øetìzce nepøipojených stránek
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
// zrušení rezevního regionu (stránky musí být uvolnìny a odpojeny!)

static void DelRegion()
{
// kontrola, zda je rezervní region
	REGION* region = ResRegion;
	if (region == NULL) return;
	ResRegion = NULL;

// odpojení stránek z øetìzce nepøipojených stránek
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

// zrušení pøíznaku použití regionu
	UsedRegion[(DWORD)region/REGSIZE] = false;

// odpojení stránky popisovaèe regionu
	VERIFY (::VirtualFree(region, PAGESIZE, MEM_DECOMMIT));

// uvolnìní regionu
	VERIFY (::VirtualFree(region, 0, MEM_RELEASE));
}


/////////////////////////////////////////////////////////////////////////////
// vytvoøení rezervní stránky pro daný modul (pøi chybì pamìti vrací FALSE)

static BOOL NewPage(MODUL* modul)
{
// vytvoøení nového regionu, není-li další volná stránka
	if (FreePage == NULL)
	{
		if (!NewRegion())
		{
			return FALSE;
		}
	}

// urèení adresy dat pøipojované stránky
	PAGE* page = FreePage;				// pøipojovaná stránka
	REGION* region = (REGION*)((DWORD)page & ~(REGSIZE-1)); // adresa regionu
	DATA* data = (DATA*)((DWORD)region + 
		((DWORD)page & (REGSIZE-1)) * (sizeof(DATA)/sizeof(PAGE))); // data

// pøipojení dat stránky
	if (::VirtualAlloc(data, PAGESIZE,
			MEM_COMMIT, PAGE_READWRITE) == NULL)
	{
		return FALSE;
	}

// inkremetace použitých stránek regionu
	region->Used++;						// inkrementace použitých stránek

// zrušení rezervace regiony
	if (region == ResRegion)			// je to rezervní region?
	{
		ResRegion = NULL;				// zrušení rezervního regionu
	}

// vyøazení stránky z øetìzce nepøipojených stránek
	PAGE* nextpage = page->NextPage;
	FreePage = nextpage;
	if (nextpage != NULL)
	{
		nextpage->PrevPage = NULL;
	}

// napojení do øetìzce volných položek modulu
	int size = modul->Size;				// velikost datové položky
	ITEM* nextitem = data->Items;		// ukazatel adresy prvku
	ITEM* item = NULL;					// není pøedešlý prvek
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

// inicializace položek popisovaèe stránky
	page->Modul = modul;				// adresa modulu
	page->Used = 0;						// poèet použitých položek

// nastavení stránky jako rezervní
	modul->ResPage = page;				// bude teï jako rezervní stránka
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// zrušení rezervní stránky (všechny položky musí být uvolnìny!)

static void DelPage(MODUL* modul)
{
// kontrola, zda je rezervní stránka
	PAGE* page = modul->ResPage;
	if (page == NULL) return;
	modul->ResPage = NULL;

// pøíprava adresy regionu a dat
	REGION* region = (REGION*)((DWORD)page & ~(REGSIZE-1)); // adresa regionu
	DATA* data = (DATA*)((DWORD)region + 
		((DWORD)page & (REGSIZE-1)) * (sizeof(DATA)/sizeof(PAGE))); // data

// odpojení z øetìzce volných položek
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

// odpojení dat stránky
	VERIFY (::VirtualFree(data, PAGESIZE, MEM_DECOMMIT));

// napojení stránky do øetìzce nepøipojených stránek
	page->PrevPage = NULL;
	page->NextPage = FreePage;
	if (FreePage != NULL)
	{
		FreePage->PrevPage = page;
	}
	FreePage = page;
	
// snížení èítaèe použitých stránek regionu, pøíp. uvolnìní regionu
	region->Used--;
	if (region->Used == 0)
	{
		if (ResRegion != NULL)
		{
			DelRegion();		// uvolnìní rezervního regionu
		}
		ResRegion = region;		// je to nyní rezervní region
	}
}


/////////////////////////////////////////////////////////////////////////////
// vytvoøení nového datového prvku pro daný modul (pøi chybì pamìti vrací NULL)

static ITEM* NewItem(MODUL* modul)
{
// vytvoøení nové stránky, není-li volný prvek
	if (modul->FreeItem == NULL)
	{
		if (!NewPage(modul))
		{
			return NULL;
		}
	}

// urèení adresy popisovaèe stránky
	ITEM* item = modul->FreeItem;		// adresa prvku
	REGION* region = (REGION*)((DWORD)item & ~(REGSIZE-1)); // adresa regionu
	DATA* data = (DATA*)((DWORD)item & ~(PAGESIZE-1)); // data
	PAGE* page = (PAGE*)((DWORD)region +
		((DWORD)data & (REGSIZE-1)) / (sizeof(DATA)/sizeof(PAGE))); // stránka

// inkrementace použitých položek stránky
	page->Used++;						// inkrementace použitých položek

// zrušení rezervace stránky
	if (page == modul->ResPage)			// je to rezervní stránka?
	{
		modul->ResPage = NULL;			// zrušení rezervní stránky
	}

// vyøazení prvku z øetìzce volných prvkù
	ITEM* nextitem = item->NextItem;	// adresa dalšího prvku
	modul->FreeItem = nextitem;
	if (nextitem != NULL)
	{
		nextitem->PrevItem = NULL;
	}
	return item;
}


/////////////////////////////////////////////////////////////////////////////
// zrušení datového prvku

static void DelItem(MODUL* modul, ITEM* item)
{
// pøíprava adres popisovaèù
	REGION* region = (REGION*)((DWORD)item & ~(REGSIZE-1)); // adresa regionu
	DATA* data = (DATA*)((DWORD)item & ~(PAGESIZE-1)); // data
	PAGE* page = (PAGE*)((DWORD)region +
		((DWORD)data & (REGSIZE-1)) / (sizeof(DATA)/sizeof(PAGE))); // stránka

// zaøazení do øetìzce volných prvkù
	item->PrevItem = NULL;
	item->NextItem = modul->FreeItem;
	if (item->NextItem != NULL)
	{
		item->NextItem->PrevItem = item;
	}
	modul->FreeItem = item;

// dekrementace použitých prvkù stránky, pøíp. uvolnìní stránky
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
// chybové hlášení - chyba pamìti

static void MemErr()
{
	if (MemoryError) return;
	MemoryError = TRUE;

	::MessageBox(MainFrame, MemErrTextP, MemErrNadpisP, MB_OK | MB_ICONSTOP);
}


/////////////////////////////////////////////////////////////////////////////
// pøidìlení bloku pamìti (povolena 0 - vrátí malý blok) (vrací NULL=chyba)

void* _fastcall MemGet(int size)
{
// pøednastavení výsledku operace na pøíznak neúspìchu
	void* result = NULL;

// kontrola zadané velikosti bloku
	ASSERT((DWORD)size < (DWORD)0x7ffffff0);
	if ((DWORD)size >= (DWORD)0x7ffffff0) 
	{
		MemErr();
		return NULL;
	}
	if (size == 0) size = 1;

// bude pøidìlení malého bloku
	if (size <= PAGESIZE)
	{

// uzamknutí pøístupu k pamìti
#ifdef _MT
		::EnterCriticalSection(&MemoryCriticalSection);
#endif

// urèení modulu velikosti prvku
		MODUL* modul = Modul;		// zaèátek popisovaèù modulù
		while (size > modul->Size)
		{
			modul++;				// zvýšení ukazatele modulù
		}

// vytvoøení datového prvku
		result = (void*)NewItem(modul);
		ASSERT(result != NULL);

// odemknutí pøístupu k pamìti
#ifdef _MT
		::LeaveCriticalSection(&MemoryCriticalSection);
#endif
	}

// pøidìlení bloku z haldy, pokud nebyl blok pøidìlen
	if (result == NULL)
	{
		result = ::HeapAlloc(hHeap, HEAPSERIALIZE, (size + 0xff) & ~0xff);
	}

// nedostatek pamìti
	if (result == NULL)
	{
		MemErr();
	}

	return result;
}


/////////////////////////////////////////////////////////////////////////////
// uvolnìní bloku pamìti (povolena adresa NULL - ignoruje se)

void _fastcall MemFree(void* adr)
{
// adresa NULL se ignoruje
	if (adr == NULL) return;

// neleží-li ve vlastním regionu, uvolnìní pomocí haldy
	if (!UsedRegion[(DWORD)adr/REGSIZE])
	{
		::HeapFree(hHeap, HEAPSERIALIZE, adr);
		return;
	}

// uzamknutí pøístupu k pamìti
#ifdef _MT
	::EnterCriticalSection(&MemoryCriticalSection);
#endif

// pøíprava ukazatelù
	REGION* region = (REGION*)((DWORD)adr & ~(REGSIZE-1)); // adresa regionu
	DATA* data = (DATA*)((DWORD)adr & ~(PAGESIZE-1)); // data
	PAGE* page = (PAGE*)((DWORD)region +
		((DWORD)data & (REGSIZE-1)) / (sizeof(DATA)/sizeof(PAGE))); // stránka

// uvolnìní položky
	ASSERT(page->Used > 0);
	MODUL* modul = page->Modul;
	ASSERT(((DWORD)adr & (modul->Size-1)) == 0);
	DelItem(modul, (ITEM*)adr);

// odemknutí pøístupu k pamìti
#ifdef _MT
	::LeaveCriticalSection(&MemoryCriticalSection);
#endif
}


/////////////////////////////////////////////////////////////////////////////
// zmìna velikosti bloku pamìti (adresa NULL = vytvoøení, velikost 0 = zrušení)
// vrací NULL=pøíznak chyby, starý blok pøi chybì nebude zmìnìn

void* _fastcall MemSize(void* adr, int size)
{
	void* newadr;

// pøi adrese NULL vytvoøení nové pamìti
	if (adr == NULL)
	{
		if (size <= 0) return NULL;
		return MemGet(size);
	}

// pøi velikosti 0 uvolnìní bloku
	if (size <= 0)
	{
		MemFree(adr);
		return NULL;
	}

// kontrola, zda blok leží ve vlastním regionu
	if (UsedRegion[(DWORD)adr/REGSIZE])
	{

// pøíprava ukazatelù
		REGION* region = (REGION*)((DWORD)adr & ~(REGSIZE-1)); // adresa regionu
		DATA* data = (DATA*)((DWORD)adr & ~(PAGESIZE-1)); // data
		PAGE* page = (PAGE*)((DWORD)region +
			((DWORD)data & (REGSIZE-1)) / (sizeof(DATA)/sizeof(PAGE))); // stránka

// kontrola, zda je adresa platná
		ASSERT(page->Used > 0);
		MODUL* modul = page->Modul;
		int oldsize = modul->Size;
		ASSERT(((DWORD)adr & (oldsize-1)) == 0);

// test, zda je potøeba velikost mìnit
		if (((size <= 8) && (oldsize <= 8)) ||
			((size <= oldsize) && (size > oldsize/4)))
		{
			return adr;					// adresa nezmìnìna
		}

// vytvoøení nového bloku
		newadr = MemGet(size);

// kopie starých dat bloku
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

// pøenesení bloku z haldy do malých blokù
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

// realokace bloku v haldì
	newadr = ::HeapReAlloc(hHeap, HEAPSERIALIZE, adr, (size + 0xff) & ~0xff);
	if (newadr == NULL) MemErr();	// chyba pamìti

	return newadr;
}


/////////////////////////////////////////////////////////////////////////////
// inicializace správce pamìti (TRUE=inicializace OK)

BOOL MemInit()
{
// kontrola konstant
#pragma warning ( disable: 4127)				// hlášení - konstantní podmínka
	ASSERT(sizeof(ITEM) == 8);
	ASSERT(sizeof(PAGE) == 16);
	ASSERT(sizeof(REGION) == REGSIZE);
	ASSERT((REGSIZE & 0xfffff) == 0);
	ASSERT(PAGESIZE <= 0x4000);
#pragma warning ( default: 4127)				// hlášení - konstantní podmínka

// funkce mùže být volána jen jednou
	ASSERT(hHeap == NULL);
	ASSERT(UsedRegion == NULL);
	if ((hHeap != NULL) || (UsedRegion != NULL)) return FALSE;

// vytvoøení haldy
	hHeap = ::HeapCreate(HEAPSERIALIZE, PAGESIZE, 0);
	if (hHeap == NULL) return FALSE;

// vytvoøení bufferu pøíznakù použití regionù
	UsedRegion = (bool*) ::HeapAlloc(hHeap, HEAPSERIALIZE, MAXREGS * sizeof(bool));
	if (UsedRegion == NULL) return FALSE;
	for (int i = 0; i < MAXREGS; i++) { UsedRegion[i] = false; };

// inicializace uzamykání pøístupu k pamìti
#ifdef _MT
	::InitializeCriticalSection(&MemoryCriticalSection);
#endif

// vytvoøení prvního regionu (pro zkoušku pamìti)
	MemoryOK = NewRegion();
	return MemoryOK;
}


/////////////////////////////////////////////////////////////////////////////
// ukonèení správce pamìti

void MemTerm()
{
	MemoryOK = FALSE;

// kontrola, zda je správce pamìti již ukonèen
	ASSERT(hHeap != NULL);
	if (hHeap == NULL) return;

// uvolnìní haldy
	::HeapDestroy(hHeap);

// uvolnìní uzamykání pøístupu k pamìti
#ifdef _MT
	::DeleteCriticalSection(&MemoryCriticalSection);
#endif
}


/////////////////////////////////////////////////////////////////////////////
// vícetoková inkrementace èísla LONG INT

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
// vícetoková dekrementace èísla LONG INT (vrací TRUE = výsledek je 0)

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
