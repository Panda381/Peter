
/***************************************************************************\
*																			*
*									Plocha									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// struktura jednoho prvku dat plochy - 8 bajt�

typedef struct MAPITEM_
{
	CIcon	Icon;					// (4) ikona (p�i prov�d�n� programu)
	DWORD	Param;					// (4) parametry pol��ka
									//		bit 0 a� bit 9 ........ (10 bit�) hodnota 1 (0 a� 1023)
									//		bit 10 a� bit 19 ...... (10 bit�) hodnota 2 (0 a� 1023)
									//		bit 20 a� bit 26 ...... (7 bit�) hodnota 3 (0 a� 127)
									//		bit 27 a� bit 31 ...... (5 bit�) p��znaky 1 a� 5
} MAPITEM;

#define SIZEOFMAPITEM	(sizeof(long) + sizeof(CIcon))	// velikost jednoho prvku plochy

/////////////////////////////////////////////////////////////////////////////
// struktura polo�ky dat plochy - 12 bajt� + data

typedef struct MAPDATA_
{
	long		Refer;					// (4) ��ta� referenc� na plochu
	long		Width;					// (4) ���ka plochy v pol��k�ch
	long		Height;					// (4) v��ka plochy v pol��k�ch
	MAPITEM		Data[1];				// data plochy (zleva doprava a zdola nahoru)
} MAPDATA;

// parametry:
#define	MAP_VALUE1_MASK		0x000003FF		// maska hodnoty 1 pol��ka
#define MAP_VALUE1_ROT		0				// po�et rotac� pro hodnotu 1 pol��ka
#define MAP_VALUE1_MAX		1023			// maxim�ln� hodnota pro hodnotu 1 pol��ka
#define MAP_VALUE2_MASK		0x000FFC00		// maska hodnoty 2 pol��ka
#define MAP_VALUE2_ROT		10				// po�et rotac� pro hodnotu 2 pol��ka
#define MAP_VALUE2_MAX		1023			// maxim�ln� hodnota pro hodnotu 2 pol��ka
#define MAP_VALUE3_MASK		0x07F00000		// maska hodnoty 3 pol��ka
#define MAP_VALUE3_ROT		20				// po�et rotac� pro hodnotu 3 pol��ka
#define MAP_VALUE3_MAX		127				// maxim�ln� hodnota pro hodnotu 3 pol��ka

// zachovat po�ad� hodnot p�ep�na�� - pou��v� se p�i zobrazen�
#define MAP_FLAG1			0x08000000		// p��znak 1
#define MAP_FLAG2			0x10000000		// p��znak 2
#define MAP_FLAG3			0x20000000		// p��znak 3
#define MAP_FLAG4			0x40000000		// p��znak 4
#define MAP_FLAG5			0x80000000		// p��znak 5

#define SIZEOFMAPDATA	(3*sizeof(long))	// velikost polo�ky dat plochy (bez dat)

//#define MAPMAXWIDTH 0x1000					// maxim�ln� ���ka plochy
//#define MAPMAXHEIGHT 0x1000					// maxim�ln� v��ka plochy


/////////////////////////////////////////////////////////////////////////////
// popisova� plochy v souboru (20 B)

typedef struct MAPFILE_
{
	char	Ident[4];				// (4) identifik�tor (= "PMAP")
	DWORD	Param;					// (4) parametry - nastaveno na 0
	long	Width;					// (4) ���ka plochy
	long	Height;					// (4) v��ka plochy
	long	Colors;					// (4) po�et palet v tabulce
	RGBQUAD	ColorTab[1];			// (4) tabulka palet
} MAPFILE;

// Za popisova�em n�sleduje:
//		- tabulka palet (form�t RGBQUAD)
//		- (4) po�et definovan�ch ikon
//		- definice ikon ve form�tu:
//				- (4) d�lka textu ve znac�ch
//				- text jm�na ikony
//				- (ICONSIZE) ikona
//		- definice plochy ve form�tu MAPITEM

#define SIZEOFMAPFILE (4*sizeof(char) + sizeof(DWORD) + 3*sizeof(long))

// statick� inicializace plch

void InitMap();
extern MAPDATA* EmptyMapData;				// data pr�zdn� plochy


/////////////////////////////////////////////////////////////////////////////
// plocha

class CMap
{

// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn� - pouze ukazatel na data
	MAPDATA*		pData;			// ukazatel na z�hlav� plochy

// p�ipojen� dat
	inline void Attach(MAPDATA* data)
	{
		ASSERT(data);
		pData = data;
		LongIncrement(&(data->Refer));
	}

// odpojen� (a zru�en�) dat
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

// vytvo�en� nov�ho bufferu - star� buffer mus� b�t odpojen!
	inline void NewBuffer(int width, int height)
	{
		ASSERT((width > 0) && (height > 0));
		if (width <= 0) width = 1;
		if (height <= 0) height = 1;
		MAPDATA* data = (MAPDATA*)MemGet(SIZEOFMAPDATA + width*height*SIZEOFMAPITEM);
		data->Refer = 1;				// po�et referenc�
		data->Width = width;			// ���ka
		data->Height = height;			// v��ka

		MAPITEM* item = data->Data;
		for (int i = width*height; i > 0; i--)
		{
			item->Icon.Init();
			item++;
		}
		pData = data;					// adresa dat
	}

// ---------------------------- ve�ejn� funkce ------------------------------

public:

// konstruktor a destruktor
	CMap();								// standardn� konstruktor
	CMap(const CMap& src);				// kop�rovac� konstruktor
	CMap(int width, int height);		// konstruktor s vytvo�en�m obr�zku
	~CMap();							// standardn� destruktor

// statick� konstruktor a destruktor
	void Init();						// statick� konstruktor
	void Init(MAPDATA* data);			// statick� konstruktor se zad�n�m dat
	void Init(int width, int height);	// statick� konstruktor s vytvo�en�m obr�zku
	void Term();						// statick� destruktor

// poskytnut� ukazatele na data
	inline MAPDATA* Data() const { return pData; };
	inline MAPITEM* DataData() const { return pData->Data; };

// poskytnut� ���ky plochy
	inline int Width() const { return pData->Width; };

// poskytnut� v��ky plochy
	inline int Height() const { return pData->Height; };

// poskytnut� velikosti dat plochy (bez z�hlav�)
	inline int Size() const { return (pData->Height * pData->Width * SIZEOFMAPITEM); };

// vymaz�n� obsahu plochy (napln�n� ikonami 0, vynulovan� parametry)
	void Clear();

// kopie do vlastn�ho bufferu p�ed modifikac�
	void CopyWrite();

// vypr�zdn�n� plochy (uvoln�n� dat)
	void Empty();

// test, zda je plocha pr�zdn�
	inline BOOL IsEmpty() { return ((DWORD)pData == (DWORD)EmptyMapData); };
	inline BOOL IsNotEmpty() { return ((DWORD)pData != (DWORD)EmptyMapData); };

// vytvo�en� nov� plochy (p�ipraveno k z�pisu, data jsou n�hodn�)
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

// poskytnut� p��stupu k prvku (bez kontroly offsetu/indexu)
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

// poskytnut� prvku (s kontrolou platnosti offsetu/indexu)
	const MAPITEM& _fastcall Get(const int off) const;
	const MAPITEM& _fastcall Get(const int x, const int y) const;

// nastaven� prvku (s kontrolou platnosti offsetu/indexu)
	void _fastcall Set(const int off, const MAPITEM& data);
	void _fastcall Set(const int x, const int y, const MAPITEM& data);

// oper�tor p�i�azen�
	const CMap& operator= (const CMap& src);

// oper�tory porovn�n�
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

// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn�
	CMap*		m_Data;		// ukazatel na data
	int			m_Num;		// po�et platn�ch polo�ek v bufferu
	int			m_Max;		// velikost bufferu (polo�ek)

// vytvo�en� nov� polo�ky
	inline int NewItem()
	{
		int i = m_Num;				// po�et polo�ek
		if (i >= m_Max)				// nen� dal�� polo�ka?
		{
			NewData();				// vytvo�en� nov�ch dat
		}

		m_Num = i + 1;
		return i;
	};

// vytvo�en� nov�ch dat (odd�leno kv�li lep�� optimalizaci)
	void NewData();

// ---------------------------- ve�ejn� funkce ------------------------------

public:

// konstruktor a destruktor
	CBufMap();
	~CBufMap();

// statick� konstruktor a destruktor
	void Init();			// statick� konstruktor
	void Term();			// statick� destruktor

// zru�en� v�ech polo�ek v bufferu (ukl�d�n� za�ne op�t po �ad� od za��tku)
	void DelAll();

// poskytnut� bufferu dat
	inline CMap* Data() const { return m_Data; };

// poskytnut� po�tu platn�ch polo�ek v bufferu
	inline int Num() const { return m_Num; };

// poskytnut� velikosti bufferu
	inline int Max() const { return m_Max; };

// kontrola platnosti polo�ky
	inline BOOL IsValid(const int index) const
		{ return ((DWORD)index < (DWORD)m_Num); };

	inline BOOL IsNotValid(const int index) const
		{ return ((DWORD)index >= (DWORD)m_Num); };

// poskytnut� p��stupu k polo�ce (bez kontroly indexu)
	inline CMap& operator[] (const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const CMap& operator[] (const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline CMap& At(const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const CMap& At(const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

// poskytnut� polo�ky (s kontrolou platnosti indexu)
	const CMap& _fastcall Get(const int index) const;

// nastaven� polo�ky (s kontrolou platnosti indexu)
	void _fastcall Set(const int index, const CMap& data);

// vypr�zdn�n� polo�ky (bez jej�ho zru�en� - jen pro uvoln�n� dat)
	void _fastcall Empty(const int index);

// zru�en� polo�ek z konce bufferu
	void _fastcall Del(int num);

// vytvo�en� pr�zdn� polo�ky (vrac� index polo�ky)
	int New();
	int New(int width, int height); // plochu vyma�e ikonami 0

// p�id�n� polo�ky (vrac� index polo�ky)
	int _fastcall Add(const CMap& data);
	int _fastcall Add(MAPDATA* data);

// duplikace polo�ky (s kontrolou platnosti indexu, vrac� index prvn� polo�ky)
	int _fastcall Dup(const int index);
	int _fastcall Dup(const int index, int num);

// oper�tor p�i�azen�
	const CBufMap& operator= (const CBufMap& src);
};

