
/***************************************************************************\
*																			*
*									Hudba									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// struktura položky dat hudby - 16 bajtù

typedef struct MUSICDATA_
{
	long	Refer;					// (4) èítaè referencí
	long	Size;					// (4) velikost dat (bajtù)
	long	res;					// (4) ... rezerva (pro zarovnání)
	BYTE*	Data;					// (4) ukazatel na data

} MUSICDATA;

// velikost položky dat hudby (bez dat)
#define SIZEOFMUSICDATA	(3*sizeof(long) + sizeof(BYTE*))

extern MUSICDATA EmptyMusicData;		// data prázdné hudby
extern	const CMusic	EmptyMusic;		// prázdná hudba

/////////////////////////////////////////////////////////////////////////////
// statické funkce

// vytvoøení dat hudby (pøi chybì pamìti vrací NULL)
MUSICDATA* _fastcall NewMusicData(int size);

// zrušení dat hudby (oddìleno kvùli lepší optimalizaci)
void _fastcall DelMusicData(MUSICDATA* data);

/////////////////////////////////////////////////////////////////////////////
// hudba

class CMusic
{

// ------------------------- interní promìnné a funkce ----------------------

private:

// promìnné
	MUSICDATA*		pData;					// ukazatel na záhlaví hudby

// pøipojení dat hudby
	inline void attach(MUSICDATA* data)
	{
		ASSERT(data != NULL);

		pData = data;
		LongIncrement(&(data->Refer));
	}

// odpojení (a zrušení) dat hudby
	inline void detach(MUSICDATA* data)
	{
		ASSERT(data != NULL);
		ASSERT(data->Refer > 0);

		if (LongDecrement(&(data->Refer)))
		{
			DelMusicData(data);
		}
	}


// ---------------------------- veøejné funkce ------------------------------

public:

// konstruktor a destruktor
	CMusic();					// prázdná hudba o délce 0
	CMusic(const CMusic& src);
	CMusic(MUSICDATA* data);
	~CMusic();

// statický konstruktor a destruktor
	void Init();				// prázdná hudba o délce 0
	void _fastcall Init(const CMusic& src);
	void _fastcall Init(MUSICDATA* data);
	bool _fastcall Init(int size);		// pøi chybì pamìti vrací FALSE, hudba není vytvoøena
	void Term();

// poskytnutí ukazatele na data
	inline MUSICDATA* Data() const { return pData; };
	inline BYTE* DataData() const { return pData->Data; };

// poskytnutí velikosti dat hudby
	inline int Size() const { return pData->Size; };

// vyprázdnìní (uvolnìní dat) - pøipojí se standardní prázdná hudba
	void Empty();

// kopie do vlastního bufferu pøed modifikací
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool CopyWrite();

// vytvoøení nové hudby (pøipraveno k zápisu, data jsou náhodná)
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool New(int size);

// naètení hudby ze souboru (TRUE=operace OK)
	bool LoadFile(CText jmeno);

// uložení do souboru formátu MID (FALSE=chyba)
	bool SaveFile(CText jmeno) const;

// operátor pøiøazení
	const CMusic& operator= (const CMusic& src);
	const CMusic& operator= (MUSICDATA* src);
};

/***************************************************************************\
*																			*
*								Buffer hudby								*
*																			*
\***************************************************************************/

class CBufMusic : public CBuffer<CMusic>
{

// ---------------------------- veøejné funkce ------------------------------

public:

// vytvoøení nové položky (prázdné/obsah náhodný) (vrací index položky, <0 = chyba)
// provádí záznam do UNDO bufferu
	inline int New() { return CBuffer<CMusic>::New(); }
	int New(int size);

// pøidání položky (vrací index položky, <0 = chyba)
// provádí záznam do UNDO bufferu
	inline int _fastcall Add(const CMusic& data) { return CBuffer<CMusic>::Add(data); }
	int _fastcall Add(MUSICDATA* data);
};

