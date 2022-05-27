
/***************************************************************************\
*																			*
*								Ikonové promìnné							*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// struktura záhlaví ikony - 16 bajtù
// Pøi komprimaci udává první dvojslovo dat délku komprimovaných dat
// Dekomprimace i komprimace se provádí s daty spoleènými pro všechny ikony,
// je proto nutno v pøípadì potøeby zajistit vlastnictví ikony!
// Komprimace se provádí pouze u ikon 32x32!

typedef struct ICONDATA_
{
	long	Refer;					// (4) èítaè referencí na ikonu
	long	Param;					// (4) parametry (prùhlednost, komprese)
	long	Size;					// (4) velikost nezkomprimovaných dat ikony (typicky 32x32 = 1024)
	BYTE*	Data;					// (4) ukazatel na data ikony
} ICONDATA;

//////////////////////////////////////////////////////////////////////////////
// pøíznaky typu ikony a obrázku
//enum PICPARAM {
//	PicParamPic,			// pouze obrázek bez pozadí
//	PicParamMix,			// obrázek mixovaný s pozadím
//	PicParamBack,			// pouze pozadí (prázdný obrázek)
//	PicParamNone,			// neznámý obsah
//	PicParamComp,			// komprimovaná data
//};

#define SIZEOFICONDATA	(3*sizeof(long) + sizeof(BYTE*)) // velikost záhlaví

extern ICONDATA EmptyIconData;				// data prázdné ikony
extern	const CIcon		EmptyIcon;			// prázdná ikona

/////////////////////////////////////////////////////////////////////////////
// statické funkce

// statická inicializace ikon (pøi chybì pamìti vrací FALSE)
bool InitIcon();

// vytvoøení dat ikony (pøi chybì pamìti vrací NULL)
ICONDATA* _fastcall NewIconData(int size);

// zrušení dat ikony (oddìleno kvùli lepší optimalizaci)
void _fastcall DelIconData(ICONDATA* data);


/////////////////////////////////////////////////////////////////////////////
// ikona

class CIcon
{

// ------------------------- interní promìnné a funkce ----------------------

private:

// promìnné
	ICONDATA*		pData;			// ukazatel na záhlaví ikony (NULL=není nic)

// pøipojení dat ikony
	inline void attach(ICONDATA* data)
	{
		ASSERT(data != NULL);

		pData = data;
		LongIncrement(&(data->Refer));
	}

// odpojení (a zrušení) dat ikony
	inline void detach(ICONDATA* data)
	{
		ASSERT(data != NULL);
		ASSERT(data->Refer > 0);

		if (LongDecrement(&(data->Refer)))
		{
			DelIconData(data);
		}
	}


// ---------------------------- veøejné funkce ------------------------------

public:

// konstruktor a destruktor
	CIcon();					// prázdná ikona o velikosti ICONSIZE
	CIcon(const CIcon& src);
	CIcon(ICONDATA* data);
	~CIcon();

// statický konstruktor a destruktor
	void Init();				// prázdná ikona o velikosti ICONSIZE
	void _fastcall Init(const CIcon& src);
	void _fastcall Init(ICONDATA* data);
	bool _fastcall Init(int size);	// pøi chybì pamìti vrací FALSE, ikona není vytvoøena
	void Term();

// poskytnutí ukazatele na data
	inline ICONDATA* Data() const { return pData; };
	inline BYTE* DataData() const { return pData->Data; };

// poskytnutí velikosti nezkomprimovaných dat ikony
	inline int Size() const { return pData->Size; };

// poskytnutí/nastavení parametrù ikony
	inline int Param() const { return pData->Param; };
	inline void Param(int param) { pData->Param = param; };

// vyprázdnìní (uvolnìní dat) - pøipojí se standardní prázdná ikona ICONSIZE
	void Empty();

// test, zda je ikona prázdná (zda ukazuje na standardní prázdnou ikonu ICONSIZE)
	inline BOOL IsEmpty() { return ((DWORD)pData == (DWORD)&EmptyIconData); };
	inline BOOL IsNotEmpty() { return ((DWORD)pData != (DWORD)&EmptyIconData); };

// vymazání obsahu ikony (naplnìní prùhlednou barvou), zajistí pøivlastnìní (a dekomprimaci) bufferu,
// pøi chybì pamìti vrací FALSE, pùvodní obsah nezmìnìn
	bool Clear();

// vymazání obsahu ikony s nastavením velikosti (naplnìní prùhlednou barvou), 
// zajistí pøivlastnìní (a dekomprimaci) bufferu, pøi chybì pamìti vrací FALSE, pùvodní obsah nezmìnìn
	bool _fastcall Clear(int size);

// kopie do vlastního bufferu pøed modifikací (komprimovaná data zùstanou komprimovaná)
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool CopyWrite();

// vytvoøení nové ikony se stejnou velikostí (pøipraveno k zápisu, data jsou náhodná)
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool New();

// vytvoøení nové ikony (pøipraveno k zápisu, data jsou náhodná)
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool _fastcall New(int size);

// kontrola platnosti offsetu bodu
	inline BOOL IsValid(const int off) const 
		{ return ((DWORD)off < (DWORD)pData->Size); };

	inline BOOL IsNotValid(const int off) const 
		{ return ((DWORD)off >= (DWORD)pData->Size); };

// poskytnutí pøístupu k bodu ikony (bez kontroly offsetu) - ikona nesmí být komprimovaná!
// v pøípadì zápisu musí být zajištìno pøivlastnìní bufferu!
	inline BYTE& operator[] (const int off) 
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline const BYTE& operator[] (const int off) const
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline BYTE& At(const int off) 
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline const BYTE& At(const int off) const
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

// poskytnutí bodu (s kontrolou platnosti offsetu) - ikona nesmí být komprimovaná!
// pro neplatný offset vrací barvu pozadí
	BYTE _fastcall Get(const int off) const;

// nastavení bodu (s kontrolou platnosti offsetu) - ikona nesmí být komprimovaná!
// pøed zápisem je nutno zajistit pøivlastnìní bufferu!
	void _fastcall Set(const int off, const BYTE data);

// komprese dat ikony 32*32, vrací velikost dat (pøi chybì pamìti vrací <0, data jsou nezmìnìna)
// komprese se provádí ve spoleèném bufferu pro všechny promìnné!
// (v pøípadì potøeby je nutno zajistit pøivlastnìní bufferu)
// pùvodní buffer s daty je zrušen
// (velikost komprimovaných dat je udávána bez dvojslova s velikostí, buffer s daty
// je tedy o 4 vìtší, data zaèínají na offsetu 4 v bufferu)
	int Comp() const;

// dekomprimace dat ikony 32*32 (jsou-li komprimována), pøi chybì vrací FALSE, data jsou nezmìnìna
// dekomprese se provádí ve spoleèném bufferu pro všechny promìnné
// (v pøípadì potøeby je nutno zajistit pøivlastnìní bufferu)
// pùvodní buffer s komprimovanými daty je zrušen
	bool DeComp() const;

// kopie nových dat ikony (rozmìry zùstanou nezmìnìny) - zajistí pøivlastnìní (a dekomprimaci) bufferu,
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool CopyData(BYTE* src);

// kopie komprimovaných nových dat ikony (rozmìry zùstanou nezmìnìny) - zajistí odpojení dat
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool CompCopy(BYTE* src);

// kopie nových dat ikony s konverzí (rozmìry zùstanou nezmìnìny) - zajistí odpojení dat
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool CopyKonvData(BYTE* src);

// extrakt ikony ze souboru formátu PET (pøi chybì vrátí FALSE, obsah nezmìnìn)
	bool Extract(CText jmeno);

// naètení ikony ze souboru formátu ICO (FALSE=chyba, obsah nezmìnìn)
	bool LoadFile(CText jmeno);

// uložení ikony do souboru formátu ICO (komprimovanou ikonu dekomprimuje) (FALSE=chyba)
	bool SaveFile(CText jmeno) const;

// operátor pøiøazení
	const CIcon& operator= (const CIcon& src);
	const CIcon& operator= (ICONDATA* src);
};


/***************************************************************************\
*																			*
*								Buffer ikon									*
*																			*
\***************************************************************************/

class CBufIcon : public CBuffer<CIcon>
{

// ------------------------- interní promìnné a funkce ----------------------

private:

// promìnné
	int			m_Width;		// šíøka jedné ikony
	int			m_Height;		// výška jedné ikony
	int			m_WidthByte;	// délka jedné linky v bajtech
	int			m_IconSize;		// velikost ikony v bajtech

// ---------------------------- veøejné funkce ------------------------------

public:

// konstruktor
	CBufIcon();

// statický konstruktor
	void Init();

// poskytnutí šíøky ikony
	inline int Width() const { return m_Width; };

// poskytnutí výšky ikony
	inline int Height() const { return m_Height; };

// poskytnutí/nastavení velikosti ikony (pøípadné existující ikony zruší!)
	void _fastcall IconSize(const int width, const int height);
	inline int IconSize() { return m_IconSize; };

// pøidání položky (vrací index položky, <0 = chyba)
// provádí záznam do UNDO bufferu
	inline int _fastcall Add(const CIcon& icon) { return CBuffer<CIcon>::Add(icon); }
	int _fastcall Add(ICONDATA* data);

// naètení ikon z resource (vynulovat buffer, aby bylo ukládáno po øadì!), vrací TRUE=operace OK
	bool Load(const int nID, int num);

// vygenerování dat bitmapy (vrací ukazatel na buffer s rezervou 8 bajtù na konci, pøi chybì vrací NULL)
	BYTE* GenerBitmap(SMALLICON smallicon);

// vygenerování masky ikon z dat barevné bitmapy (smallicon = polovièní velikost)
// barevná bitmapa musí mít na konci rezervu pro zarovnání linky na 8 bajtù !
// vrací adresu na buffer, pøi chybì vrací NULL
	BYTE* GenerMask(BYTE* bitmapa, SMALLICON smallicon);

// vygenerování seznamu ikon (drag = tažení, smallicon = polovièní velikost,
//		himg = použitý seznam, pro himg=NULL vytvoøí nový seznam)
// pøi chybì vrací NULL, pùvodní seznam ikon pøitom zùstává nezmìnìn
	HIMAGELIST GenerList(bool drag, SMALLICON smallicon, HIMAGELIST himg = NULL);
};
