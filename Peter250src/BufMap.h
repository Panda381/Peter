
/***************************************************************************\
*																			*
*									Plocha									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// struktura jednoho prvku dat plochy - 8 bajt�

typedef struct MAPITEM_
{
	long	Icon;					// (4) index ikony v bufferu ikon
	DWORD	Param;					// (4) parametry pol��ka
									//		bit 0 a� bit 9 ........ (10 bit�) hodnota 1 (0 a� 1023)
									//		bit 10 a� bit 19 ...... (10 bit�) hodnota 2 (0 a� 1023)
									//		bit 20 a� bit 26 ...... (7 bit�) hodnota 3 (0 a� 127)
									//		bit 27 a� bit 31 ...... (5 bit�) p��znaky 1 a� 5
} MAPITEM;

#define SIZEOFMAPITEM	(sizeof(long) + sizeof(DWORD))	// velikost jednoho prvku plochy

/////////////////////////////////////////////////////////////////////////////
// struktura polo�ky dat plochy - 12 bajt� + data

typedef struct MAPDATA_
{
	long		Refer;					// (4) ��ta� referenc� na plochu
	long		Width;					// (4) ���ka plochy v pol��k�ch
	long		Height;					// (4) v��ka plochy v pol��k�ch
	MAPITEM		Data[1];				// data plochy
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

#define MAPMAXWIDTH 0x1000					// maxim�ln� ���ka plochy
#define MAPMAXHEIGHT 0x1000					// maxim�ln� v��ka plochy


/////////////////////////////////////////////////////////////////////////////
// popisova� plochy v souboru (20 B)

typedef struct MAPFILE_
{
	char	Ident[4];				// (4) identifik�tor (= "PMAP")
	char	Verze;					// (1) verze
									//		0=jednojazykov� verze UNICODE
									//		1=v�cejazy�n� verze
	char	Jazyku;					// (1) po�et jazyk� (pro jednojazykovou verzi = 0)
	short	Param;					// (1) dopl�uj�c� parametr (nastaveno na 0)
	long	Width;					// (4) ���ka plochy
	long	Height;					// (4) v��ka plochy
	long	Colors;					// (4) po�et palet v tabulce
	RGBQUAD	ColorTab[1];			// (4) tabulka palet
} MAPFILE;

// Za popisova�em n�sleduje:
//
// JEDNOJAZY�N� VERZE:
//		- tabulka palet (form�t RGBQUAD)
//		- (4) po�et definovan�ch ikon
//		- definice ikon ve form�tu:
//				- (4) d�lka textu ve znac�ch
//				- text jm�na ikony v k�du UNICODE
//				- (ICONSIZE) ikona
//		- definice plochy ve form�tu MAPITEM
//
// V�CEJAZY�N� VERZE:
//		- tabulka palet (form�t RGBQUAD)
//		- (4) po�et definovan�ch ikon
//		- definice ikon ve form�tu:
//				- texty pro v�echny jazyky:
//					- (4) identifik�tor jazyku (0=implicitn�)
//					- (4) k�dov� str�nka jazyku (0=UNICODE)
//				    - (4) d�lka textu ve znac�ch
//				    - text jm�na ikony
//				- (ICONSIZE) ikona
//		- definice plochy ve form�tu MAPITEM


#define SIZEOFMAPFILE (6*sizeof(char) + sizeof(short) + 3*sizeof(long))


/////////////////////////////////////////////////////////////////////////////
// inicializa�n� pr�zdn� plocha (modifikuje se po�et referenc�!)
// Pr�zdn� plocha ukazuje na pr�zdnou ikonu 0!

extern MAPDATA EmptyMapData;				// data pr�zdn� plochy
extern	const CMap	EmptyMap;			// pr�zdn� plocha

/////////////////////////////////////////////////////////////////////////////
// statick� funkce

// vytvo�en� dat plochy (p�i chyb� pam�ti vrac� NULL)
MAPDATA* _fastcall NewMapData(int width, int height);

// zru�en� dat plochy (odd�leno kv�li lep�� optimalizaci)
void _fastcall DelMapData(MAPDATA* data);

// zv��en� reference na ikonu (v bufferu glob�ln�ch a lok�ln�ch objekt�)
void _fastcall RefIconInc(const int icon);

// sn�en� reference na ikonu (v bufferu glob�ln�ch a lok�ln�ch objekt�)
void _fastcall RefIconDec(const int icon);


/////////////////////////////////////////////////////////////////////////////
// plocha

class CMap
{

// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn�
	MAPDATA*		pData;			// ukazatel na z�hlav� plochy

// p�ipojen� dat plochy
	inline void attach(MAPDATA* data)
	{
		ASSERT(data != NULL);

		pData = data;
		LongIncrement(&(data->Refer));
	}

// odpojen� (a zru�en�) dat plochy
	inline void detach(MAPDATA* data)
	{
		ASSERT(data != NULL);
		ASSERT(data->Refer > 0);

		if (LongDecrement(&(data->Refer)))
		{
			DelMapData(data);
		}
	}


// ---------------------------- ve�ejn� funkce ------------------------------

public:

// konstruktor a destruktor
	CMap();						// pr�zdn� plocha o velikosti 1x1 (ikona 0)
	CMap(const CMap& src);
	CMap(MAPDATA* data);
	~CMap();

// statick� konstruktor a destruktor
	void Init();				// pr�zdn� plocha o velikosti 1x1 (ikona 0)
	void _fastcall Init(const CMap& src);
	void _fastcall Init(MAPDATA* data);
	bool _fastcall Init(int width, int height);	// p�i chyb� pam�ti vr�t� FALSE, plocha nen� vytvo�ena
	void Term();

// poskytnut� ukazatele na data
	inline MAPDATA* Data() const { return pData; };
	inline MAPITEM* DataData() const { return pData->Data; };

// poskytnut� ���ky plochy
	inline int Width() const { return pData->Width; };

// poskytnut� v��ky plochy
	inline int Height() const { return pData->Height; };

// poskytnut� velikosti dat plochy (bez z�hlav�)
	inline int Size() const { return (pData->Height * pData->Width * SIZEOFMAPITEM); };

// vypr�zdn�n� (uvoln�n� dat) - p�ipoj� se standardn� pr�zdn� plocha (velikost 1x1, ikona 0)
	void Empty();

// test, zda je plocha pr�zdn� (zda ukazuje na standardn� pr�zdnou plochu 1x1)
	inline BOOL IsEmpty() { return ((DWORD)pData == (DWORD)&EmptyMapData); };
	inline BOOL IsNotEmpty() { return ((DWORD)pData != (DWORD)&EmptyMapData); };

// vymaz�n� obsahu plochy (napln�n� ikonami 0, vynulovan� parametry), zajist� p�ivlastn�n� bufferu,
// p�i chyb� pam�ti vrac� FALSE, p�vodn� obsah nezm�n�n
	bool Clear();

// vymaz�n� obsahu plochy s nastaven�m velikosti (napln�n� ikonami 0, vynulovan� parametry),
// zajist� p�ivlastn�n� bufferu, p�i chyb� pam�ti vrac� FALSE, p�vodn� obsah nezm�n�n
	bool _fastcall Clear(int width, int height);

// kopie do vlastn�ho bufferu p�ed modifikac�
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool CopyWrite();

// vytvo�en� nov� plochy se stejnou velikost� (p�ipraveno k z�pisu, data jsou n�hodn�)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool New();

// vytvo�en� nov� plochy (p�ipraveno k z�pisu, data jsou n�hodn�)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool _fastcall New(int width, int height);

// zm�na velikosti plochy
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
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

// poskytnut� p��stupu k prvku (bez kontroly offsetu/indexu)
// v p��pad� z�pisu mus� b�t zaji�t�no p�ivlastn�n� bufferu!
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
// pro neplatn� prvek vrac� p��stup na prvek pr�zdn� plochy
	const MAPITEM& _fastcall Get(const int off) const;
	const MAPITEM& _fastcall Get(const int x, const int y) const;

// nastaven� prvku (s kontrolou platnosti offsetu/indexu)
// p�ed z�pisem je nutno zajistit p�ivlastn�n� bufferu!
	void _fastcall Set(const int off, const MAPITEM& data);
	void _fastcall Set(const int x, const int y, const MAPITEM& data);

// zv��en� reference na v�echny ikony v plo�e
	void RefInc();

// sn�en� reference na v�echny ikony v plo�e
	void RefDec();

// zru�en� pou�it� ikony v plo�e (index = index plochy v seznamu) - nahrad� ikonou 0
// ignoruje p��padnou chybu pam�ti!
	void _fastcall DelIcon(int icon, int index, bool undo);

// na�ten� plochy ze souboru (TRUE=operace OK, p�i chyb� obsah nezm�n�n)
	bool LoadFile(CText jmeno);

// ulo�en� do souboru (TRUE=operace OK)
	bool SaveFile(CText jmeno) const;

// oper�tor p�i�azen�
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

// ---------------------------- ve�ejn� funkce ------------------------------

public:
// vytvo�en� nov� plochy (vrac� index polo�ky, <0 = chyba)
// prov�d� z�znam do UNDO bufferu
	inline int New() { return CBuffer<CMap>::New(); } // pr�zdn� plocha 1x1 (ikona 0)
	int _fastcall New(int width, int height); // plochu vyma�e ikonami 0

// p�id�n� polo�ky (vrac� index polo�ky, <0 = chyba)
// prov�d� z�znam do UNDO bufferu
	inline int _fastcall Add(const CMap& data) { return CBuffer<CMap>::Add(data); }
	int _fastcall Add(MAPDATA* data);

// zru�en� pou�it� ikony ze v�ech ploch - nahrad� ikonou 0
// ignoruje p��padnou chybu pam�ti!!!
	void _fastcall DelIcon(int icon);
};

