
#include "Main.h"

/***************************************************************************\
*																			*
*								Obsluha pamìti								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// obsluha vícevláknového reimu

#ifdef _MT
#define HEAPSERIALIZE 0					// pro vícevláknovı reim je serializace
#else
#define HEAPSERIALIZE HEAP_NO_SERIALIZE	// vypnutí serializace, není-li vícevláknovı reim
#endif

#ifdef _MT
static	CRITICAL_SECTION	MemoryCriticalSection;	// kritická sekce pro správce pamìti
#endif

/////////////////////////////////////////////////////////////////////////////
// konstanty - velikosti musí bıt mocnina 2 !

#ifndef _M_ALPHA
#define	PAGESIZE		0x1000			// velikost alokaèní stránky pro ostatní procesory (4096)
#else
#define	PAGESIZE		0x2000			// velikost alokaèní stránky pro procesor Alpha (8192)
#endif

#define PAGEITEMS		(PAGESIZE/sizeof(ITEM))	// poèet prvkù na stránku (512 popø. 1024)
#define	REGPAGES		(PAGESIZE/sizeof(PAGE)) // poèet stránek na region (256 popø. 512)
#define DATAPAGES		(REGPAGES-1)			// poèet datovıch stránek na region (255 popø. 511)
#define REGSIZE			(PAGESIZE*REGPAGES)		// velikost regionu (1 MB popø. 4 MB)
#define NUMREGS			(0x78000000/REGSIZE-1)	// poèet regionù v pamìti (2039 popø. 509)
#define REGSYSTEM		(0x800000/REGSIZE)		// poèet systémovıch regionù na zaèátku (8 popø. 2)
#define MAXREGS			((0x40000000/REGSIZE)*4)// poèet regionù celkem (4096 popø. 1024)

/////////////////////////////////////////////////////////////////////////////
// struktury

struct MODUL_;

// struktura datového prvku - 8 bajtù
typedef struct ITEM_
{
	ITEM_*		NextItem;				// adresa dalšího volného prvku (NULL=konec)
	ITEM_*		PrevItem;				// adresa pøedešlého volného prvku (NULL=zaèátek)
} ITEM;

// struktura dat stránky - velikost PAGESIZE
typedef struct DATA_
{
	ITEM		Items[PAGEITEMS];		// data jedné stránky
} DATA;

// struktura popisovaèe stránky - velikost 16 bajtù
typedef struct PAGE_
{
	PAGE_*		NextPage;				// adresa další stránky (NULL=konec)
	PAGE_*		PrevPage;				// adresa pøedešlé stránky (NULL=zaèátek)
	MODUL_*		Modul;					// adresa popisovaèe modulu
	long		Used;					// èítaè pouitıch poloek
} PAGE;

// struktura popisovaèe modulu
typedef struct MODUL_
{
	ITEM*		FreeItem;				// adresa volného prvku
	PAGE*		ResPage;				// rezervní stránka (NULL=není)
	long		Size;					// velikost datové poloky
} MODUL;

// struktura regionu - velikost REGSIZE
typedef struct REGION_
{
	long		Used;					// èítaè pouitıch stránek
	long		res[3];					// rezerva pro zarovnání
	PAGE		Pages[DATAPAGES];		// popisovaèe stránek
	DATA		Data[DATAPAGES];		// data
} REGION;


/////////////////////////////////////////////////////////////////////////////
// promìnné

static	HANDLE	hHeap = NULL;			// handle haldy

bool MemoryOK = false;					// pøíznak platného správce pamìti

// obsluha regionù
static	char*	UsedRegion;				// buffer pøíznakù pouití regionù
static	int		NextRegion = REGSYSTEM;	// index pøíštì pøidìlovaného regionu
static	REGION*	ResRegion = NULL;		// rezervní region (NULL=není)

// obsluha nepøipojenıch stránek (následuje øetìzec pomocí NextPage)
static	PAGE*	FreePage = NULL;		// první nepøipojená stránka (NULL=není)

// popisovaèe datovıch modulù
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
// prototypy lokálních funkcí

static ITEM* NewItem(MODUL* modul);				// vytvoøení nového datového prvku
static void DelItem(MODUL* modul, ITEM* item);	// zrušení datového prvku
static bool NewPage(MODUL* modul);				// vytvoøení nové stránky
static void DelPage(MODUL* modul);				// zrušení rezervní stránky
static bool NewRegion();						// vytvoøení nového regionu
static void DelRegion();						// zrušení rezervního regionu


/////////////////////////////////////////////////////////////////////////////
// inicializace správce pamìti (TRUE=inicializace OK)

bool MemInit()
{
// kontrola konstant
#pragma warning ( disable: 4127)				// hlášení - konstantní podmínka
	ASSERT(sizeof(ITEM) == 8);
	ASSERT(sizeof(PAGE) == 16);
	ASSERT(sizeof(REGION) == REGSIZE);
	ASSERT((REGSIZE & 0xfffff) == 0);
	ASSERT(PAGESIZE <= 0x4000);
	ASSERT(FALSE == 0);
#pragma warning ( default: 4127)				// hlášení - konstantní podmínka

// funkce mùe bıt volána jen jednou
	ASSERT(hHeap == NULL);
	if (hHeap != NULL) return true;

// vytvoøení haldy
	hHeap = ::HeapCreate(HEAPSERIALIZE, PAGESIZE, 0);
	if (hHeap == NULL) return false;

// vytvoøení bufferu pøíznakù pouití regionù
	UsedRegion = (char*) ::HeapAlloc(hHeap, HEAP_ZERO_MEMORY | HEAPSERIALIZE, MAXREGS);
	if (UsedRegion == NULL) return false;

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
	MemoryOK = false;

// kontrola, zda je správce pamìti ji ukonèen
	ASSERT(hHeap != NULL);
	if (hHeap == NULL) return;

// uvolnìní haldy
	::HeapDestroy(hHeap);
	hHeap = NULL;

// uvolnìní uzamykání pøístupu k pamìti
#ifdef _MT
	::DeleteCriticalSection(&MemoryCriticalSection);
#endif
}


/////////////////////////////////////////////////////////////////////////////
// chyba pamìti

void MemErr()
{
	::MessageBox(NULL, JazykInfo[Jazyk].MemErrText, JazykInfo[Jazyk].MemErrNadp, MB_OK | MB_ICONSTOP);
	Exit(EXITCODE_MEMERR);
}


/////////////////////////////////////////////////////////////////////////////
// pøidìlení bloku pamìti (povolena 0) NULL=chyba

void* _fastcall MemGet(int size)
{
	void* result = NULL;

// kontrola zadané velikosti bloku
	ASSERT((size >= 0) && (size < 0x7ffffff0));
	if (size <= 0) size = 1;

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
			modul++;				// zvıšení ukazatele modulù
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

	if (result == NULL) MemErr();	// chyba pamìti

	return result;
}


/////////////////////////////////////////////////////////////////////////////
// uvolnìní bloku pamìti (povoleno NULL)

void _fastcall MemFree(void* adr)
{
// adresa NULL se ignoruje
	if (adr == NULL) return;

// neleí-li ve vlastním regionu, uvolnìní pomocí haldy (ještì není nutné uzamknutí!)
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

// kontrola, zda je stránka platná
	if (page->Used > 0)
	{

// kontrola, zda adresa leí na platné hranici
		MODUL* modul = page->Modul;
		if (((DWORD)adr & (modul->Size-1)) == 0)
		{

// uvolnìní poloky
			DelItem(modul, (ITEM*)adr);
		}
	}

// odemknutí pøístupu k pamìti
#ifdef _MT
	::LeaveCriticalSection(&MemoryCriticalSection);
#endif
}


/////////////////////////////////////////////////////////////////////////////
// zmìna velikosti bloku pamìti (povolena adresa NULL i velikost 0, pøi chybì nezmìnìno)
// Pozn.: zde není nutné uzamykat pamì (znaènì to zpomaluje) !!!

void* _fastcall MemSize(void* adr, int size)
{
	int oldsize;
	void* newadr;

// pøi adrese NULL alokace pamìti
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

// kontrola, zda blok leí ve vlastním regionu
	if (UsedRegion[(DWORD)adr/REGSIZE])
	{

// pøíprava ukazatelù
		REGION* region = (REGION*)((DWORD)adr & ~(REGSIZE-1)); // adresa regionu
		DATA* data = (DATA*)((DWORD)adr & ~(PAGESIZE-1)); // data
		PAGE* page = (PAGE*)((DWORD)region +
			((DWORD)data & (REGSIZE-1)) / (sizeof(DATA)/sizeof(PAGE))); // stránka

// kontrola, zda je stránka platná
		if (page->Used > 0)
		{

// kontrola, zda adresa leí na platné hranici
			MODUL* modul = page->Modul;
			oldsize = modul->Size;

			if (((DWORD)adr & (oldsize-1)) == 0)
			{

// test, zda je potøeba velikost mìnit
				if (((size <= 8) && (oldsize <= 8)) ||
					((size <= oldsize) && (size > oldsize/4)))
				{
					return adr;					// adresa nezmìnìna
				}

// vytvoøení nového bloku
				newadr = MemGet(size);

// kopie starıch dat bloku
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

// neplatná adresa - chybovı návrat
			else
			{
				return NULL;
			}
		}

// neplatná stránka - chybovı návrat
		else
		{
			return NULL;
		}
	}

// pøenesení bloku z haldy do malıch blokù
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

// realokace bloku v haldì
	newadr = ::HeapReAlloc(hHeap, HEAPSERIALIZE, adr, (size + 0xff) & ~0xff);
	if (newadr == NULL) MemErr();	// chyba pamìti
	return newadr;
}


/////////////////////////////////////////////////////////////////////////////
// vytvoøení nového datového prvku

static ITEM* NewItem(MODUL* modul)
{
// zajištìní platného prvku
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

// inkrementace pouitıch promìnnıch stránky, zrušení rezervace stránky
	page->Used++;						// inkrementace pouitıch poloek
	if (page == modul->ResPage)			// je to rezervní stránka?
	{
		modul->ResPage = NULL;			// zrušení rezervní stránky
	}

// vyøazení prvku z øetìzce volnıch prvkù
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

// zaøazení do øetìzce volnıch prvkù
	item->PrevItem = NULL;
	item->NextItem = modul->FreeItem;
	if (item->NextItem != NULL)
	{
		item->NextItem->PrevItem = item;
	}
	modul->FreeItem = item;

// dekrementace pouitıch prvkù stránky, pøíp. uvolnìní stránky
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
// vytvoøení rezervní stránky

static bool NewPage(MODUL* modul)
{
// zajištìní pøipojitelné stránky
	if (FreePage == NULL)
	{
		if (!NewRegion())
		{
			return false;
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
		return false;
	}

// inkremetace pouitıch stránek regionu, zrušení rezervace regiony
	region->Used++;						// inkrementace pouitıch stránek
	if (region == ResRegion)			// je to rezervní region?
	{
		ResRegion = NULL;				// zrušení rezervního regionu
	}

// vyøazení stránky z øetìzce nepøipojenıch stránek
	PAGE* nextpage = page->NextPage;
	FreePage = nextpage;
	if (nextpage != NULL)
	{
		nextpage->PrevPage = NULL;
	}

// napojení do øetìzce volnıch poloek modulu
	int size = modul->Size;				// velikost datové poloky
	ITEM* previtem;
	ITEM* nextitem = data->Items;		// ukazatel adresy prvku
	ITEM* item = NULL;					// není pøedešlı prvek
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

// inicializace poloek popisovaèe stránky
	page->Modul = modul;				// adresa modulu
	page->Used = 0;						// poèet pouitıch poloek

// nastavení stránky jako rezervní
	modul->ResPage = page;				// bude teï jako rezervní stránka
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// zrušení rezervní stránky

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

// odpojení z øetìzce volnıch poloek
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

// odpojení stránky
	VERIFY (::VirtualFree(data, PAGESIZE, MEM_DECOMMIT));

// napojení stránky do øetìzce nepøipojenıch stránek
	page->PrevPage = NULL;
	page->NextPage = FreePage;
	if (FreePage != NULL)
	{
		FreePage->PrevPage = page;
	}
	FreePage = page;
	
// sníení èítaèe pouitıch stránek regionu, pøíp. uvolnìní regionu
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
// vytvoøení rezervního regionu

static bool NewRegion()
{
// lokální promìnné
	REGION* region;						// ukazatel adresy regionu
	PAGE*	page;						// ukazatel adresy stránky
	PAGE*	nextpage;					// pomocnı ukazatel - pøíští stránka
	PAGE*	prevpage;					// pomocnı ukazatel - pøedchozí stránka
	char*	used;						// ukazatel pouitıch regionù
	int i;								// pomocnı èítaè

// nalezení nepouitého regionu
	for (i = NUMREGS;  i > 0; i--)
	{

// adresa zkoušeného regionu
		region = (REGION*)(void*)(NextRegion*REGSIZE); // adresa regionu
		used = &(UsedRegion[NextRegion]);	// pøíznak pouití regionu

// posun indexu pøíštího regionu
		NextRegion++;				// zvıšení indexu regionu
		if (NextRegion >= REGSYSTEM+NUMREGS)
		{
			NextRegion = REGSYSTEM;
		}
		
// test, zda je region pouitı		
		if (!*used)
		{

// rezervování nového regionu
			region = (REGION*)::VirtualAlloc(region,
					REGSIZE, MEM_RESERVE, PAGE_READWRITE);

// vytvoøení první stránky pro popisovaè regionu
			if (region != NULL)
			{
				if (::VirtualAlloc(region, PAGESIZE,
						MEM_COMMIT, PAGE_READWRITE) == NULL)
				{

// pøi chybì uvolnìní regionu a hlášení chyby
					::VirtualFree(region, 0, MEM_RELEASE);
					return false;
				}

// pøíznak pouití regionu
				*used = TRUE;		// pøíznak pouití regionu
				ResRegion = region;	// bude teï jako rezervní region
				region->Used = 0;		// èítaè pouitıch stránek

// napojení stránek do øetìzce nepøipojenıch stránek
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
// zrušení rezevního regionu

static void DelRegion()
{
// kontrola, zda je rezervní region
	REGION* region = ResRegion;
	if (region == NULL) return;
	ResRegion = NULL;

// odpojení stránek z øetìzce nepøipojenıch stránek
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

// zrušení pøíznaku pouití regionu
	UsedRegion[(DWORD)region/REGSIZE] = FALSE;

// odpojení stránky popisovaèe regionu
	VERIFY (::VirtualFree(region, PAGESIZE, MEM_DECOMMIT));

// uvolnìní regionu
	VERIFY (::VirtualFree(region, 0, MEM_RELEASE));
}


/////////////////////////////////////////////////////////////////////////////
// vícetoková inkrementace èísla LONG INT

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
// vícetoková dekrementace èísla LONG INT (vrací true = vısledek je 0)

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
