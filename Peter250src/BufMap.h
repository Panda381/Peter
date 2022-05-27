
/***************************************************************************\
*																			*
*									Plocha									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// struktura jednoho prvku dat plochy - 8 bajtù

typedef struct MAPITEM_
{
	long	Icon;					// (4) index ikony v bufferu ikon
	DWORD	Param;					// (4) parametry políèka
									//		bit 0 až bit 9 ........ (10 bitù) hodnota 1 (0 až 1023)
									//		bit 10 až bit 19 ...... (10 bitù) hodnota 2 (0 až 1023)
									//		bit 20 až bit 26 ...... (7 bitù) hodnota 3 (0 až 127)
									//		bit 27 až bit 31 ...... (5 bitù) pøíznaky 1 až 5
} MAPITEM;

#define SIZEOFMAPITEM	(sizeof(long) + sizeof(DWORD))	// velikost jednoho prvku plochy

/////////////////////////////////////////////////////////////////////////////
// struktura položky dat plochy - 12 bajtù + data

typedef struct MAPDATA_
{
	long		Refer;					// (4) èítaè referencí na plochu
	long		Width;					// (4) šíøka plochy v políèkách
	long		Height;					// (4) výška plochy v políèkách
	MAPITEM		Data[1];				// data plochy
} MAPDATA;

// parametry:
#define	MAP_VALUE1_MASK		0x000003FF		// maska hodnoty 1 políèka
#define MAP_VALUE1_ROT		0				// poèet rotací pro hodnotu 1 políèka
#define MAP_VALUE1_MAX		1023			// maximální hodnota pro hodnotu 1 políèka
#define MAP_VALUE2_MASK		0x000FFC00		// maska hodnoty 2 políèka
#define MAP_VALUE2_ROT		10				// poèet rotací pro hodnotu 2 políèka
#define MAP_VALUE2_MAX		1023			// maximální hodnota pro hodnotu 2 políèka
#define MAP_VALUE3_MASK		0x07F00000		// maska hodnoty 3 políèka
#define MAP_VALUE3_ROT		20				// poèet rotací pro hodnotu 3 políèka
#define MAP_VALUE3_MAX		127				// maximální hodnota pro hodnotu 3 políèka

// zachovat poøadí hodnot pøepínaèù - používá se pøi zobrazení
#define MAP_FLAG1			0x08000000		// pøíznak 1
#define MAP_FLAG2			0x10000000		// pøíznak 2
#define MAP_FLAG3			0x20000000		// pøíznak 3
#define MAP_FLAG4			0x40000000		// pøíznak 4
#define MAP_FLAG5			0x80000000		// pøíznak 5

#define SIZEOFMAPDATA	(3*sizeof(long))	// velikost položky dat plochy (bez dat)

#define MAPMAXWIDTH 0x1000					// maximální šíøka plochy
#define MAPMAXHEIGHT 0x1000					// maximální výška plochy


/////////////////////////////////////////////////////////////////////////////
// popisovaè plochy v souboru (20 B)

typedef struct MAPFILE_
{
	char	Ident[4];				// (4) identifikátor (= "PMAP")
	char	Verze;					// (1) verze
									//		0=jednojazyková verze UNICODE
									//		1=vícejazyèná verze
	char	Jazyku;					// (1) poèet jazykù (pro jednojazykovou verzi = 0)
	short	Param;					// (1) doplòující parametr (nastaveno na 0)
	long	Width;					// (4) šíøka plochy
	long	Height;					// (4) výška plochy
	long	Colors;					// (4) poèet palet v tabulce
	RGBQUAD	ColorTab[1];			// (4) tabulka palet
} MAPFILE;

// Za popisovaèem následuje:
//
// JEDNOJAZYÈNÁ VERZE:
//		- tabulka palet (formát RGBQUAD)
//		- (4) poèet definovaných ikon
//		- definice ikon ve formátu:
//				- (4) délka textu ve znacích
//				- text jména ikony v kódu UNICODE
//				- (ICONSIZE) ikona
//		- definice plochy ve formátu MAPITEM
//
// VÍCEJAZYÈNÁ VERZE:
//		- tabulka palet (formát RGBQUAD)
//		- (4) poèet definovaných ikon
//		- definice ikon ve formátu:
//				- texty pro všechny jazyky:
//					- (4) identifikátor jazyku (0=implicitní)
//					- (4) kódová stránka jazyku (0=UNICODE)
//				    - (4) délka textu ve znacích
//				    - text jména ikony
//				- (ICONSIZE) ikona
//		- definice plochy ve formátu MAPITEM


#define SIZEOFMAPFILE (6*sizeof(char) + sizeof(short) + 3*sizeof(long))


/////////////////////////////////////////////////////////////////////////////
// inicializaèní prázdná plocha (modifikuje se poèet referencí!)
// Prázdná plocha ukazuje na prázdnou ikonu 0!

extern MAPDATA EmptyMapData;				// data prázdné plochy
extern	const CMap	EmptyMap;			// prázdná plocha

/////////////////////////////////////////////////////////////////////////////
// statické funkce

// vytvoøení dat plochy (pøi chybì pamìti vrací NULL)
MAPDATA* _fastcall NewMapData(int width, int height);

// zrušení dat plochy (oddìleno kvùli lepší optimalizaci)
void _fastcall DelMapData(MAPDATA* data);

// zvýšení reference na ikonu (v bufferu globálních a lokálních objektù)
void _fastcall RefIconInc(const int icon);

// snížení reference na ikonu (v bufferu globálních a lokálních objektù)
void _fastcall RefIconDec(const int icon);


/////////////////////////////////////////////////////////////////////////////
// plocha

class CMap
{

// ------------------------- interní promìnné a funkce ----------------------

private:

// promìnné
	MAPDATA*		pData;			// ukazatel na záhlaví plochy

// pøipojení dat plochy
	inline void attach(MAPDATA* data)
	{
		ASSERT(data != NULL);

		pData = data;
		LongIncrement(&(data->Refer));
	}

// odpojení (a zrušení) dat plochy
	inline void detach(MAPDATA* data)
	{
		ASSERT(data != NULL);
		ASSERT(data->Refer > 0);

		if (LongDecrement(&(data->Refer)))
		{
			DelMapData(data);
		}
	}


// ---------------------------- veøejné funkce ------------------------------

public:

// konstruktor a destruktor
	CMap();						// prázdná plocha o velikosti 1x1 (ikona 0)
	CMap(const CMap& src);
	CMap(MAPDATA* data);
	~CMap();

// statický konstruktor a destruktor
	void Init();				// prázdná plocha o velikosti 1x1 (ikona 0)
	void _fastcall Init(const CMap& src);
	void _fastcall Init(MAPDATA* data);
	bool _fastcall Init(int width, int height);	// pøi chybì pamìti vrátí FALSE, plocha není vytvoøena
	void Term();

// poskytnutí ukazatele na data
	inline MAPDATA* Data() const { return pData; };
	inline MAPITEM* DataData() const { return pData->Data; };

// poskytnutí šíøky plochy
	inline int Width() const { return pData->Width; };

// poskytnutí výšky plochy
	inline int Height() const { return pData->Height; };

// poskytnutí velikosti dat plochy (bez záhlaví)
	inline int Size() const { return (pData->Height * pData->Width * SIZEOFMAPITEM); };

// vyprázdnìní (uvolnìní dat) - pøipojí se standardní prázdná plocha (velikost 1x1, ikona 0)
	void Empty();

// test, zda je plocha prázdná (zda ukazuje na standardní prázdnou plochu 1x1)
	inline BOOL IsEmpty() { return ((DWORD)pData == (DWORD)&EmptyMapData); };
	inline BOOL IsNotEmpty() { return ((DWORD)pData != (DWORD)&EmptyMapData); };

// vymazání obsahu plochy (naplnìní ikonami 0, vynulované parametry), zajistí pøivlastnìní bufferu,
// pøi chybì pamìti vrací FALSE, pùvodní obsah nezmìnìn
	bool Clear();

// vymazání obsahu plochy s nastavením velikosti (naplnìní ikonami 0, vynulované parametry),
// zajistí pøivlastnìní bufferu, pøi chybì pamìti vrací FALSE, pùvodní obsah nezmìnìn
	bool _fastcall Clear(int width, int height);

// kopie do vlastního bufferu pøed modifikací
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool CopyWrite();

// vytvoøení nové plochy se stejnou velikostí (pøipraveno k zápisu, data jsou náhodná)
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool New();

// vytvoøení nové plochy (pøipraveno k zápisu, data jsou náhodná)
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool _fastcall New(int width, int height);

// zmìna velikosti plochy
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool Resize(int width, int height);

// kontrola platnosti offsetu prvku
	inline BOOL IsValid(const int off) const 
		{ return ((DWORD)off < (DWORD)(pData->Width * pData->Height)); };

	inline BOOL IsNotValid(const int off) const 
		{ return ((DWORD)off >= (DWORD)(pData->Width * pData->Height)); };

// kontrola platnosti indexu prvku
	inline BOOL IsValid(const int x, const int y) const
		{ return (((DWORD)x < (DWORD)pData->Width) && ((DWORD)y < (DWORD)pData->Height)); };

	inline BOOL IsNotValid(const int x, const int y) const
		{ return (((DWORD)x >= (DWORD)pData->Width) || ((DWORD)y >= (DWORD)pData->Height)); };

// poskytnutí pøístupu k prvku (bez kontroly offsetu/indexu)
// v pøípadì zápisu musí být zajištìno pøivlastnìní bufferu!
	inline MAPITEM& operator[] (const int off) 
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline const MAPITEM& operator[] (const int off) const
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline MAPITEM& At(const int off) 
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline const MAPITEM& At(const int off) const
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline MAPITEM& At(const int x, const int y) 
		{ ASSERT(IsValid(x, y)); return pData->Data[x + y*pData->Width]; }

	inline const MAPITEM& At(const int x, const int y) const
		{ ASSERT(IsValid(x, y)); return pData->Data[x + y*pData->Width]; }

// poskytnutí prvku (s kontrolou platnosti offsetu/indexu)
// pro neplatný prvek vrací pøístup na prvek prázdné plochy
	const MAPITEM& _fastcall Get(const int off) const;
	const MAPITEM& _fastcall Get(const int x, const int y) const;

// nastavení prvku (s kontrolou platnosti offsetu/indexu)
// pøed zápisem je nutno zajistit pøivlastnìní bufferu!
	void _fastcall Set(const int off, const MAPITEM& data);
	void _fastcall Set(const int x, const int y, const MAPITEM& data);

// zvýšení reference na všechny ikony v ploše
	void RefInc();

// snížení reference na všechny ikony v ploše
	void RefDec();

// zrušení použití ikony v ploše (index = index plochy v seznamu) - nahradí ikonou 0
// ignoruje pøípadnou chybu pamìti!
	void _fastcall DelIcon(int icon, int index, bool undo);

// naètení plochy ze souboru (TRUE=operace OK, pøi chybì obsah nezmìnìn)
	bool LoadFile(CText jmeno);

// uložení do souboru (TRUE=operace OK)
	bool SaveFile(CText jmeno) const;

// operátor pøiøazení
	const CMap& operator= (const CMap& src);
	const CMap& operator= (MAPDATA* src);
};

/***************************************************************************\
*																			*
*								Buffer ploch								*
*																			*
\***************************************************************************/

class CBufMap : public CBuffer<CMap>
{

// ---------------------------- veøejné funkce ------------------------------

public:
// vytvoøení nové plochy (vrací index položky, <0 = chyba)
// provádí záznam do UNDO bufferu
	inline int New() { return CBuffer<CMap>::New(); } // prázdná plocha 1x1 (ikona 0)
	int _fastcall New(int width, int height); // plochu vymaže ikonami 0

// pøidání položky (vrací index položky, <0 = chyba)
// provádí záznam do UNDO bufferu
	inline int _fastcall Add(const CMap& data) { return CBuffer<CMap>::Add(data); }
	int _fastcall Add(MAPDATA* data);

// zrušení použití ikony ze všech ploch - nahradí ikonou 0
// ignoruje pøípadnou chybu pamìti!!!
	void _fastcall DelIcon(int icon);
};

