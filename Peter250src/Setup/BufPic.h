
/***************************************************************************\
*																			*
*									Obrázky									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// struktura položky dat obrázku - 32 bajtù
// Pøi komprimaci udává první dvojslovo dat délku komprimovaných dat
// Dekomprimace i komprimace se provádí s daty spoleènými pro všechny obrázky
// je proto nutno v pøípadì potøeby zajistit vlastnictví obrázku!

typedef struct PICTUREDATA_
{
	long	Refer;					// (4) èítaè referencí na obrázek
	long	Width;					// (4) šíøka obrázku v bodech
	long	Height;					// (4) výška obrázku v linkách
	long	Param;					// (4) parametry (prùhlednost, komprese)
	long	Res1;					// (4) ... pro zarovnání
	long	Res2;					// (4) ... pro zarovnání
	long	Res3;					// (4) ... pro zarovnání
	BYTE*	Data;					// (4) ukazatel na data (velikost Width*Height)
} PICTUREDATA;

//////////////////////////////////////////////////////////////////////////////
// pøíznaky typu ikony a obrázku
//enum PICPARAM {
//	PicParamPic,			// pouze obrázek bez pozadí
//	PicParamMix,			// obrázek mixovaný s pozadím
//	PicParamBack,			// pouze pozadí (prázdný obrázek)
//	PicParamNone,			// neznámý obsah
//	PicParamComp,			// komprimovaná data
//};

#define SIZEOFPICTUREDATA	(7*sizeof(long) + sizeof(BYTE*))

extern PICTUREDATA EmptyPictureData;	// data prázdného obrázku (velikost ICONSIZE)
extern	const CPicture	EmptyPicture;	// prázdný obrázek

/////////////////////////////////////////////////////////////////////////////
// statické funkce

// statická inicializace obrázkù (pøi chybì pamìti vrací FALSE)
bool InitPicture();

// vytvoøení dat obrázku (pøi chybì pamìti vrací NULL)
PICTUREDATA* _fastcall NewPictureData(int width, int height);

// zrušení dat obrázku (oddìleno kvùli lepší optimalizaci)
void _fastcall DelPictureData(PICTUREDATA* data);


/////////////////////////////////////////////////////////////////////////////
// obrázek

class CPicture
{

// ------------------------- interní promìnné a funkce ----------------------

private:

// promìnné - pouze ukazatel na data
	PICTUREDATA*		pData;			// ukazatel na záhlaví obrázku

// pøipojení dat obrázku
	inline void attach(PICTUREDATA* data)
	{
		ASSERT(data != NULL);

		pData = data;
		LongIncrement(&(data->Refer));
	}

// odpojení (a zrušení) dat obrázku
	inline void detach(PICTUREDATA* data)
	{
		ASSERT(data != NULL);
		ASSERT(data->Refer > 0);

		if (LongDecrement(&(data->Refer)))
		{
			DelPictureData(data);
		}
	}

// ---------------------------- veøejné funkce ------------------------------

public:

// konstruktor a destruktor
	CPicture();						// prázdný obrázek o velikosti ICONSIZE
	CPicture(const CPicture& src);
	CPicture(PICTUREDATA* data);
	~CPicture();

// statický konstruktor a destruktor
	void Init();					// prázdný obrázek o velikosti ICONSIZE
	void _fastcall Init(const CPicture& src);
	void _fastcall Init(PICTUREDATA* data);
	bool _fastcall Init(int width, int height); // pøi chybì pamìti vrací FALSE, obrázek není vytvoøen
	void Term();

// poskytnutí ukazatele na data
	inline PICTUREDATA* Data() const { return pData; }; // poskytnutí adresy dat
	inline BYTE* DataData() const { return pData->Data; }; // poskytnutí adresy dat

// poskytnutí šíøky obrázky
	inline int Width() const { return pData->Width; };

// poskytnutí výšky obøázku
	inline int Height() const { return pData->Height; };

// poskytnutí velikosti nezkomprimovaných dat obrázku (bez záhlaví)
	inline int Size() const { return pData->Width * pData->Height; };

// vyprázdnìní (uvolnìní dat)
	void Empty();

// test, zda je obrázek prázdný (zda ukazuje na standardní prázdný obrázek ICONSIZE)
	inline BOOL IsEmpty() { return ((DWORD)pData == (DWORD)&EmptyPictureData); };
	inline BOOL IsNotEmpty() { return ((DWORD)pData != (DWORD)&EmptyPictureData); };

// vymazání obsahu obrázku (naplnìní prùhlednou barvou), zajistí pøivlastnìní (a dekomprimaci) bufferu,
// pøi chybì pamìti vrací FALSE, pùvodní obsah nezmìnìn
	bool Clear();

// vymazání obsahu obrázku s nastavením velikosti (naplnìní prùhlednou barvou), zajistí pøivlastnìní (a dekomprimaci) bufferu,
// pøi chybì pamìti vrací FALSE, pùvodní obsah nezmìnìn
	bool _fastcall Clear(int width, int height);

// kopie obrázku do vlastního bufferu pøed modifikací (komprimovaná data zùstanou komprimovaná)
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool CopyWrite();

// vytvoøení nového obrázku se stejnou velikostí (pøipraveno k zápisu, data jsou náhodná)
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool New();

// vytvoøení nového obrázku (pøipraveno k zápisu, data jsou náhodná)
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool _fastcall New(int width, int height);

// nastavení nových rozmìrù obrázku (nová data jsou vymazána)
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool Resize(int width, int height);

// kontrola platnosti offsetu bodu
	inline BOOL IsValid(const int off) const 
		{ return ((DWORD)off < (DWORD)Size()); };

	inline BOOL IsNotValid(const int off) const 
		{ return ((DWORD)off >= (DWORD)Size()); };

// kontrola platnosti indexu 
	inline BOOL IsValid(const int x, const int y) const
		{ return (((DWORD)x < (DWORD)pData->Width) && ((DWORD)y < (DWORD)pData->Height)); };

	inline BOOL IsNotValid(const int x, const int y) const
		{ return (((DWORD)x >= (DWORD)pData->Width) || ((DWORD)y >= (DWORD)pData->Height)); };

// poskytnutí pøístupu k bodu (bez kontroly offsetu/indexu) - obrázek nesmí být komprimovaný!
// v pøípadì zápisu musí být zajištìno pøivlastnìní bufferu!
	inline BYTE& operator[] (const int off) 
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline const BYTE& operator[] (const int off) const
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline BYTE& At(const int off) 
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline const BYTE& At(const int off) const
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline BYTE& At(const int x, const int y) 
		{ ASSERT(IsValid(x, y)); return pData->Data[x + y*pData->Width]; }

	inline const BYTE& At(const int x, const int y) const
		{ ASSERT(IsValid(x, y)); return pData->Data[x + y*pData->Width]; }

// poskytnutí bodu (s kontrolou platnosti offsetu/indexu), obrázek nesmí být komprimovaný!
// pro neplatný bod vrátí barvu pozadí)
	BYTE _fastcall Get(const int off) const;
	BYTE _fastcall Get(const int x, const int y) const;

// nastavení bodu (s kontrolou platnosti offsetu/indexu), obrázek nesmí být komprimovaný!
// pøed zápisem je nutno zajistit pøivlastnìní bufferu!
	void _fastcall Set(const int off, const BYTE data);
	void _fastcall Set(const int x, const int y, const BYTE data);

// komprese dat obrázku, vrací velikost dat (pøi chybì pamìti vrací <0, data jsou nezmìnìna)
// komprese se provádí ve spoleèném bufferu pro všechny promìnné!
// (v pøípadì potøeby je nutno zajistit pøivlastnìní bufferu)
// pùvodní buffer s daty je zrušen
// (velikost komprimovaných dat je udávána bez dvojslova s velikostí, buffer s daty
// je tedy o 4 vìtší, data zaèínají na offsetu 4 v bufferu)
	int Comp() const;

// dekomprimace dat obrázku (jsou-li komprimována), pøi chybì vrací FALSE
// dekomprese se provádí ve spoleèném bufferu pro všechny promìnné
// (v pøípadì potøeby je nutno zajistit pøivlastnìní bufferu)
// pùvodní buffer s komprimovanými daty je zrušen
	bool DeComp() const;

// kopie nových dat obrázku (rozmìry zùstanou nezmìnìny) - zajistí odpojení dat
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool CopyData(BYTE* src);

// kopie komprimovaných nových dat obrázku (rozmìry zùstanou nezmìnìny) - zajistí odpojení dat
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool CompCopy(BYTE* src);

// kopie výøezu do obrázku (se zprùhlednìním)
	void AddCopy(const CPicture& src, int x, int y, int w, int h, int xs, int ys);
	void AddCopy(const CPicture& src, int x, int y);
	void AddCopy(int id, int x, int y);

// kopie nových dat obrázku s konverzí (rozmìry zùstanou nezmìnìny) - zajistí odpojení dat
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool CopyKonvData(BYTE* src);

// naètení obrázku ze souboru (FALSE=chyba, obsah nezmìnìn)
//	bool LoadFile(CText jmeno);

// uložení obrázku do souboru formátu BMP (komprimovaný obrázek dekomprimuje)(FALSE=chyba)
//	bool SaveFile(CText jmeno) const;

// vygenerování obrázku pro tažení (pøi chybì vrací NULL)
//	HIMAGELIST GenerDrag(const CText& text);

// naètení obrázku z resource
	bool CPicture::Load(const int nID);

// operátor pøiøazení
	const CPicture& operator= (const CPicture& src);
	const CPicture& operator= (PICTUREDATA* src);
};

