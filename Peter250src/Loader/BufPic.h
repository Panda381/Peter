
/***************************************************************************\
*																			*
*									Obrázky									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// struktura položky dat obrázku - (64 bajtù + data) nebo (24 bajtù + data)

typedef struct PICTUREDATA_
{
	long	Refer;					// (4) èítaè referencí na obrázek
	long	Param;					// (4) parametry (prùhlednost, komprese)
	long	Width;					// (4) šíøka obrázku v bodech
	long	Height;					// (4) výška obrázku v linkách
	BYTE*	Data;					// (4) ukazatel na data (velikost Width*Height)

#ifndef _MINI
	long	TextInx;				// (4) index textury v bufferu textur (-1 = není)
	long	TextWidth;				// (4) šíøka textury, platné pokud TextData!=NULL
	long	TextHeight;				// (4) výška textury, platné pokud TextData!=NULL
	long	TextIsMip;				// (4) vygenerováno s mipmaps
	long	TextMipMaps;			// (4) poèet vygenerovaných mipmaps, platné pokud TextData!=NULL
									//	  (v datech uloženy sekvenènì za sebou až po rozmìr 1x1)
	long	TextDevType;			// (4) typ použitého zaøízení, platné pokud TextData!=NULL
									//			( = D3DIntAkt*100 + D3DDevAkt)
	BOOL	TextTrans;				// (4) textura obsahuje prùhlednou barvu

	BYTE*	TextData;				// (4) data textury ve formátu D3DFMT_A8R8G8B8, 32 bitù (NULL=nejsou vytvoøena)
	BYTE*	TextDataRGBA;			// (4) data textury ve formátu RGBA, 32 bitù (pro OpenGL)
	BYTE*	TextDataR8G8B8;			// (4) data textury ve formátu D3DFMT_R8G8B8, 24 bitù
	BYTE*	TextDataR5G6B5;			// (4) data textury ve formátu D3DFMT_R5G6B5, 16 bitù
	BYTE*	TextDataA1R5G5B5;		// (4) data textury ve formátu D3DFMT_A1R5G5B5, 16 bitù
	BYTE*	TextDataA4R4G4B4;		// (4) data textury ve formátu D3DFMT_A4R4G4B4, 16 bitù
	double	TextSmooth;				// (8) aktuální zjemnìní textury (nastaveno pøi prvním použití jako textura)
									//     (0=data jsou ze souboru a nesmí se proto rušit)
#else
	long	res;					// (4) zarovnání dat
#endif // _MINI

} PICTUREDATA;

// Podporované formáty textur (pøíkazem CreateTexture):
//	D3DFMT_A8R8G8B8	(32 bitù s alpha)
//	D3DFMT_X8R8G8B8 (32 bitù)
//	D3DFMT_R5G6B5 (16 bitù)
//	D3DFMT_X1R5G5B5 (15 bitù)
//	D3DFMT_A1R5G5B5 (15 bitù s alpha)
//	D3DFMT_A4R4G4B4 (12 bitù s alpha)
//	D3DFMT_DXT1 (komprese DXT1)
//	D3DFMT_DXT2 (komprese DXT2)
//	D3DFMT_DXT3 (komprese DXT3)
//	D3DFMT_DXT4 (komprese DXT4)
//	D3DFMT_DXT5 (komprese DXT5)

// >>> !!!!! V souèasnosti Petr používá pouze formát D3DFMT_A8R8G8B8 !!!!! <<<


#ifndef _MINI
#define SIZEOFPICTUREDATA	(4*sizeof(long) + sizeof(BYTE*) + 6*sizeof(long) + sizeof(BOOL) + 6*sizeof(BYTE*) + sizeof(double))
#else  // _MINI
#define SIZEOFPICTUREDATA	(4*sizeof(long) + sizeof(BYTE*) + sizeof(long))
#endif // _MINI

extern PICTUREDATA EmptyPictureData;	// data prázdného obrázku


/////////////////////////////////////////////////////////////////////////////
// obrázek

class CPicture
{

// ------------------------- interní promìnné a funkce ----------------------

private:

// promìnné - pouze ukazatel na data
	PICTUREDATA*		pData;			// ukazatel na záhlaví obrázku

// pøipojení dat
	inline void Attach(PICTUREDATA* data)
	{
		ASSERT(data);
		pData = data;
		LongIncrement(&(data->Refer));
	}

// odpojení (a zrušení) dat
	void _fastcall Detach();

// vytvoøení nového bufferu - starý buffer musí být odpojen!
	inline void NewBuffer(int width, int height)
	{
		ASSERT((width > 0) && (height > 0));
		if (width <= 0) width = 1;
		if (height <= 0) height = 1;
		PICTUREDATA* data = (PICTUREDATA*)MemGet(SIZEOFPICTUREDATA);
		data->Refer = 1;				// poèet referencí
		data->Param = PicParamNone;		// parametry
		data->Width = width;		// šíøka
		data->Height = height;	// výška
		data->Data = (BYTE*)MemGet(width*height);
#ifndef _MINI
		data->TextInx = -1;
		data->TextData = NULL;
		data->TextDataRGBA = NULL;
		data->TextDataR8G8B8 = NULL;
		data->TextDataR5G6B5 = NULL;
		data->TextDataA1R5G5B5 = NULL;
		data->TextDataA4R4G4B4 = NULL;
#endif // _MINI
		pData = data;					// adresa dat
	}

// ---------------------------- veøejné funkce ------------------------------

public:

// konstruktor a destruktor
	CPicture();							// standardní konstruktor
	CPicture(const CPicture& src);		// kopírovací konstruktor
	CPicture(int width, int height);	// konstruktor s vytvoøením obrázku
	~CPicture();						// standardní destruktor

// statický konstruktor a destruktor
	void Init();						// statický konstruktor
	void Init(PICTUREDATA* data);		// statický konstruktor se zadáním dat
	void Init(int width, int height);	// statický konstruktor s vytvoøením obrázku
	void Term();						// statický destruktor

#ifndef _MINI
// importování textury z True Color bufferu
// ... Funkce vstupní buffer zruší!
// ... Buffery pro texturu musí být zrušeny!
	void ImportTexture(BYTE* buf, int newwidth, int newheight);

// vytvoøení dat textury, není-li vytvoøena
	void InitTexture(int stage);

// inicializace MipMap textury
//	void InitMipMap();

#endif // _MINI

// poskytnutí ukazatele na data
	inline PICTUREDATA* Data() const { return pData; }; // poskytnutí adresy dat
	inline BYTE* DataData() const { return pData->Data; }; // poskytnutí adresy dat

// poskytnutí/nastavení parametru obrázku
	inline int Param() const { return pData->Param; };
	inline void Param(int param) { pData->Param = param; };

// poskytnutí šíøky obrázky
	inline int Width() const { return pData->Width; };

// poskytnutí výšky obøázku
	inline int Height() const { return pData->Height; };

// poskytnutí velikost dat obrázku (bez záhlaví)
	inline int Size() const { return (Width()*Height()); };

// vymazání obsahu obrázku (naplnìní prùhlednou barvou)
	inline void Clear() { MemFill(pData->Data, Size(), BackCol); };

// kopie nových dat obrázku (rozmìry zùstanou nezmìnìny) - zajistí odpojení dat
	void CopyData(BYTE* src);

// kopie nových dat obrázku s konverzí (rozmìry zùstanou nezmìnìny) - zajistí odpojení dat
	void CopyKonvData(BYTE* src);

// kopie do vlastního bufferu pøed modifikací
	void CopyWrite();

// vyprázdnìní obrázku (uvolnìní dat)
	void Empty();

// test, zda je obrázek prázdný
	inline BOOL IsEmpty() { return ((DWORD)pData == (DWORD)&EmptyPictureData); };
	inline BOOL IsNotEmpty() { return ((DWORD)pData != (DWORD)&EmptyPictureData); };

// vytvoøení nového obrázku (pøipraveno k zápisu, data jsou náhodná)
	void New(int width, int height);

// nastavení nových rozmìrù obrázku (nová data jsou vymazána)
	void Resize(int width, int height);

// exportování obrázku do bufferu TRUECOLORALPHABETA (vrací buffer)
	BYTE* ExportTrueAlphaBeta();

// importování obrázku z bufferu TRUECOLORALPHABETA
	void ImportTrueAlphaBeta(BYTE* buf, BOOL dith);

// zmìna velikosti obrázku (inter = interpolovat, dith = ditherovat)
	void Zoom(int width, int height, BOOL inter, BOOL dith);

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

// poskytnutí pøístupu k bodu (bez kontroly offsetu/indexu)
	inline BYTE& operator[] (const int off) 
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline const BYTE& operator[] (const int off) const
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline BYTE& At(const int off) 
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline const BYTE& At(const int off) const
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline BYTE& At(const int x, const int y) 
		{ ASSERT(IsValid(x, y)); return pData->Data[x + y*Width()]; }

	inline const BYTE& At(const int x, const int y) const
		{ ASSERT(IsValid(x, y)); return pData->Data[x + y*Width()]; }

// poskytnutí bodu (s kontrolou platnosti offsetu/indexu)
	BYTE _fastcall Get(const int off) const;
	BYTE _fastcall Get(const int x, const int y) const;

// nastavení bodu (s kontrolou platnosti offsetu/indexu)
	void _fastcall Set(const int off, const BYTE data);
	void _fastcall Set(const int x, const int y, const BYTE data);

// pøevrácení obrázku
	void XFlip();
	void YFlip();

// zmìna jasu obrázku
	void Level(double koef, bool dith);

// otoèení obrázku
	void Rotate(double angle, bool inter, bool dith);

// naètení obrázku ze souboru (tex = importovat texturu)
	void LoadFile(bool tex);

// uložení obrázku do souboru
	void SaveFile();

// operátor pøiøazení
	const CPicture& operator= (const CPicture& src);
	const CPicture& operator= (PICTUREDATA* src);
	const CPicture& operator= (CIcon& icon);
	const CPicture& operator= (CMap& map);

// dekomprimace dat obrázku (jsou-li komprimována)
	void DeComp();

// maskování obrázku
	void Mask(CPicture& mask);

// zámìna/náhrada barvy v obrázku
	void XCol(BYTE col1, BYTE col2);
	void SCol(BYTE oldcol, BYTE newcol);

// operátory porovnání
	friend inline BOOL operator==(const CPicture& s1, const CPicture& s2)
		{ return (DWORD)s1.pData == (DWORD)s2.pData; };

	friend inline BOOL operator!=(const CPicture& s1, const CPicture& s2)
		{ return (DWORD)s1.pData != (DWORD)s2.pData; };
};

/////////////////////////////////////////////////////////////////////////////
// konverze obrázku v bufferu TrueColorAlphaBeta (vrací nový buffer)

BYTE* ZoomTrueColor(BYTE* buf, int oldwidth, int oldheight, int newwidth, int newheight);


/***************************************************************************\
*																			*
*								Buffer obrázkù								*
*																			*
\***************************************************************************/

class CBufPic
{

// ------------------------- interní promìnné a funkce ----------------------

private:

// promìnné
	CPicture*	m_Data;		// ukazatel na data
	int			m_Num;		// poèet platných položek v bufferu
	int			m_Max;		// velikost bufferu (položek)

// vytvoøení nové položky
	inline int NewItem()
	{
		int i = m_Num;
		if (i >= m_Max)
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
	CBufPic();
	~CBufPic();

// statický konstruktor a destruktor
	void Init();
	void Term();

// zrušení všech položek v bufferu
	void DelAll();

// poskytnutí bufferu dat
	inline CPicture* Data() const { return m_Data; };

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
	inline CPicture& operator[] (const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const CPicture& operator[] (const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline CPicture& At(const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const CPicture& At(const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

// poskytnutí položky (s kontrolou platnosti indexu)
	const CPicture& _fastcall Get(const int index) const;

// nastavení položky (s kontrolou platnosti indexu)
	void _fastcall Set(const int index, const CPicture& data);

// vyprázdnìní položky (bez jejího zrušení - jen pro uvolnìní dat)
	void _fastcall Empty(const int index);

// zrušení položek z konce bufferu
	void _fastcall Del(int num);

// vytvoøení prázdné položky (vrací index položky)
	int New();
	int New(int width, int height);			// obrázek vymaže prùhlednou barvou

// pøidání položky (vrací index položky)
	int _fastcall Add(const CPicture& data);
	int _fastcall Add(PICTUREDATA* data);

// duplikace položky (s kontrolou platnosti indexu, vrací index první položky)
	int _fastcall Dup(const int index);
	int _fastcall Dup(const int index, int num);

// operátor pøiøazení
	const CBufPic& _fastcall operator= (const CBufPic& src);
};

