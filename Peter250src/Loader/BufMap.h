
/***************************************************************************\
*																			*
*									Plocha									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// struktura jednoho prvku dat plochy - 8 bajtù

typedef struct MAPITEM_
{
	CIcon	Icon;					// (4) ikona (pøi provádìní programu)
	DWORD	Param;					// (4) parametry políèka
									//		bit 0 až bit 9 ........ (10 bitù) hodnota 1 (0 až 1023)
									//		bit 10 až bit 19 ...... (10 bitù) hodnota 2 (0 až 1023)
									//		bit 20 až bit 26 ...... (7 bitù) hodnota 3 (0 až 127)
									//		bit 27 až bit 31 ...... (5 bitù) pøíznaky 1 až 5
} MAPITEM;

#define SIZEOFMAPITEM	(sizeof(long) + sizeof(CIcon))	// velikost jednoho prvku plochy

/////////////////////////////////////////////////////////////////////////////
// struktura položky dat plochy - 12 bajtù + data

typedef struct MAPDATA_
{
	long		Refer;					// (4) èítaè referencí na plochu
	long		Width;					// (4) šíøka plochy v políèkách
	long		Height;					// (4) výška plochy v políèkách
	MAPITEM		Data[1];				// data plochy (zleva doprava a zdola nahoru)
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

//#define MAPMAXWIDTH 0x1000					// maximální šíøka plochy
//#define MAPMAXHEIGHT 0x1000					// maximální výška plochy


/////////////////////////////////////////////////////////////////////////////
// popisovaè plochy v souboru (20 B)

typedef struct MAPFILE_
{
	char	Ident[4];				// (4) identifikátor (= "PMAP")
	DWORD	Param;					// (4) parametry - nastaveno na 0
	long	Width;					// (4) šíøka plochy
	long	Height;					// (4) výška plochy
	long	Colors;					// (4) poèet palet v tabulce
	RGBQUAD	ColorTab[1];			// (4) tabulka palet
} MAPFILE;

// Za popisovaèem následuje:
//		- tabulka palet (formát RGBQUAD)
//		- (4) poèet definovaných ikon
//		- definice ikon ve formátu:
//				- (4) délka textu ve znacích
//				- text jména ikony
//				- (ICONSIZE) ikona
//		- definice plochy ve formátu MAPITEM

#define SIZEOFMAPFILE (4*sizeof(char) + sizeof(DWORD) + 3*sizeof(long))

// statická inicializace plch

void InitMap();
extern MAPDATA* EmptyMapData;				// data prázdné plochy


/////////////////////////////////////////////////////////////////////////////
// plocha

class CMap
{

// ------------------------- interní promìnné a funkce ----------------------

private:

// promìnné - pouze ukazatel na data
	MAPDATA*		pData;			// ukazatel na záhlaví plochy

// pøipojení dat
	inline void Attach(MAPDATA* data)
	{
		ASSERT(data);
		pData = data;
		LongIncrement(&(data->Refer));
	}

// odpojení (a zrušení) dat
	inline void Detach()
	{
		ASSERT(pData);
		ASSERT(pData->Refer > 0);

		if (LongDecrement(&(pData->Refer)))
		{
			MAPITEM* item = pData->Data;
			for (int i = pData->Width * pData->Height; i > 0; i--)
			{
				item->Icon.Term();
				item++;
			}
			MemFree(pData);
#ifdef _DEBUG
			pData = NULL;
#endif
		}
	}

// vytvoøení nového bufferu - starý buffer musí být odpojen!
	inline void NewBuffer(int width, int height)
	{
		ASSERT((width > 0) && (height > 0));
		if (width <= 0) width = 1;
		if (height <= 0) height = 1;
		MAPDATA* data = (MAPDATA*)MemGet(SIZEOFMAPDATA + width*height*SIZEOFMAPITEM);
		data->Refer = 1;				// poèet referencí
		data->Width = width;			// šíøka
		data->Height = height;			// výška

		MAPITEM* item = data->Data;
		for (int i = width*height; i > 0; i--)
		{
			item->Icon.Init();
			item++;
		}
		pData = data;					// adresa dat
	}

// ---------------------------- veøejné funkce ------------------------------

public:

// konstruktor a destruktor
	CMap();								// standardní konstruktor
	CMap(const CMap& src);				// kopírovací konstruktor
	CMap(int width, int height);		// konstruktor s vytvoøením obrázku
	~CMap();							// standardní destruktor

// statický konstruktor a destruktor
	void Init();						// statický konstruktor
	void Init(MAPDATA* data);			// statický konstruktor se zadáním dat
	void Init(int width, int height);	// statický konstruktor s vytvoøením obrázku
	void Term();						// statický destruktor

// poskytnutí ukazatele na data
	inline MAPDATA* Data() const { return pData; };
	inline MAPITEM* DataData() const { return pData->Data; };

// poskytnutí šíøky plochy
	inline int Width() const { return pData->Width; };

// poskytnutí výšky plochy
	inline int Height() const { return pData->Height; };

// poskytnutí velikosti dat plochy (bez záhlaví)
	inline int Size() const { return (pData->Height * pData->Width * SIZEOFMAPITEM); };

// vymazání obsahu plochy (naplnìní ikonami 0, vynulované parametry)
	void Clear();

// kopie do vlastního bufferu pøed modifikací
	void CopyWrite();

// vyprázdnìní plochy (uvolnìní dat)
	void Empty();

// test, zda je plocha prázdná
	inline BOOL IsEmpty() { return ((DWORD)pData == (DWORD)EmptyMapData); };
	inline BOOL IsNotEmpty() { return ((DWORD)pData != (DWORD)EmptyMapData); };

// vytvoøení nové plochy (pøipraveno k zápisu, data jsou náhodná)
	void New(int width, int height);

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
	const MAPITEM& _fastcall Get(const int off) const;
	const MAPITEM& _fastcall Get(const int x, const int y) const;

// nastavení prvku (s kontrolou platnosti offsetu/indexu)
	void _fastcall Set(const int off, const MAPITEM& data);
	void _fastcall Set(const int x, const int y, const MAPITEM& data);

// operátor pøiøazení
	const CMap& operator= (const CMap& src);

// operátory porovnání
	friend inline BOOL operator==(const CMap& s1, const CMap& s2)
		{ return (DWORD)s1.pData == (DWORD)s2.pData; };

	friend inline BOOL operator!=(const CMap& s1, const CMap& s2)
		{ return (DWORD)s1.pData != (DWORD)s2.pData; };
};

/***************************************************************************\
*																			*
*								Buffer ploch								*
*																			*
\***************************************************************************/

class CBufMap
{

// ------------------------- interní promìnné a funkce ----------------------

private:

// promìnné
	CMap*		m_Data;		// ukazatel na data
	int			m_Num;		// poèet platných položek v bufferu
	int			m_Max;		// velikost bufferu (položek)

// vytvoøení nové položky
	inline int NewItem()
	{
		int i = m_Num;				// poèet položek
		if (i >= m_Max)				// není další položka?
		{
			NewData();				// vytvoøení nových dat
		}

		m_Num = i + 1;
		return i;
	};

// vytvoøení nových dat (oddìleno kvùli lepší optimalizaci)
	void NewData();

// ---------------------------- veøejné funkce ------------------------------

public:

// konstruktor a destruktor
	CBufMap();
	~CBufMap();

// statický konstruktor a destruktor
	void Init();			// statický konstruktor
	void Term();			// statický destruktor

// zrušení všech položek v bufferu (ukládání zaène opìt po øadì od zaèátku)
	void DelAll();

// poskytnutí bufferu dat
	inline CMap* Data() const { return m_Data; };

// poskytnutí poètu platných položek v bufferu
	inline int Num() const { return m_Num; };

// poskytnutí velikosti bufferu
	inline int Max() const { return m_Max; };

// kontrola platnosti položky
	inline BOOL IsValid(const int index) const
		{ return ((DWORD)index < (DWORD)m_Num); };

	inline BOOL IsNotValid(const int index) const
		{ return ((DWORD)index >= (DWORD)m_Num); };

// poskytnutí pøístupu k položce (bez kontroly indexu)
	inline CMap& operator[] (const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const CMap& operator[] (const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline CMap& At(const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const CMap& At(const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

// poskytnutí položky (s kontrolou platnosti indexu)
	const CMap& _fastcall Get(const int index) const;

// nastavení položky (s kontrolou platnosti indexu)
	void _fastcall Set(const int index, const CMap& data);

// vyprázdnìní položky (bez jejího zrušení - jen pro uvolnìní dat)
	void _fastcall Empty(const int index);

// zrušení položek z konce bufferu
	void _fastcall Del(int num);

// vytvoøení prázdné položky (vrací index položky)
	int New();
	int New(int width, int height); // plochu vymaže ikonami 0

// pøidání položky (vrací index položky)
	int _fastcall Add(const CMap& data);
	int _fastcall Add(MAPDATA* data);

// duplikace položky (s kontrolou platnosti indexu, vrací index první položky)
	int _fastcall Dup(const int index);
	int _fastcall Dup(const int index, int num);

// operátor pøiøazení
	const CBufMap& operator= (const CBufMap& src);
};

