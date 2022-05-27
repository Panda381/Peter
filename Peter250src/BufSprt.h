
/***************************************************************************\
*																			*
*									Sprajt									*
*																			*
\***************************************************************************/

#define	SPRITEDEFDELAY	55			// implicitn� prodleva
#define	SPRITEDEFLEVEL	1000		// implicitn� hladina
#define	SPRITEDEFKROKU	8			// implicitn� po�et krok�

#define SPRITEMAXKLID  1024			// maxim�ln� po�et klidov�ch f�z�
#define SPRITEMAXPOHYB 1024			// maxim�ln� po�et f�z� pro pohyb
#define SPRITEMAXFAZE  (SPRITEMAXKLID+SPRITEMAXPOHYB) // max. po�et f�z�
#define SPRITEMAXSMER  1024			// maxim�ln� po�et sm�r�
#define SPRITEMAXDELAY 3600000		// maxim�ln� prodleva mezi f�zemi
#define SPRITEMAXKROKU 1000000		// maxim�ln� po�et krok� na jednotku

/////////////////////////////////////////////////////////////////////////////
// struktura polo�ky dat sprajtu (32 bajt� + data)
// V�echny obr�zky maj� stejnou velikost, mus� b�t minim�ln� 1 obr�zek,
// - velikost obr�zku se zji��uje z prvn�ho obr�zku

typedef struct SPRITEDATA_
{
	long		Refer;				// (4) ��ta� referenc� na sprajt
	long		Faze;				// (4) po�et f�z� celkem
	long		Smer;				// (4) po�et sm�r�
	long		Klid;				// (4) z toho po�et klidov�ch f�z�
	long		Delay;				// (4) prodleva mezi dv�ma f�zemi (v milisekund�ch)
	long		Level;				// (4) hladina k p�ekreslov�n� (0=p�edm�ty)
	double		Kroku;				// (8) po�et f�z� na jednotkovou vzd�lenost (0=sleduje ihned)
	CPicture	Data[1];			// obr�zky sprajtu (v po�ad� f�ze/sm�r)
} SPRITEDATA;

#define SIZEOFSPRITEDATA	(6*sizeof(long) + sizeof(double)) // velikost polo�ky dat sprajtu (bez dat)

/////////////////////////////////////////////////////////////////////////////
// popisova� sprajtu v souboru (40 B)

typedef struct SPRITEFILE_
{
	char	Ident[4];				// (4) identifik�tor (= "PSPR")
	long	Faze;					// (4) po�et f�z� celkem
	long	Smer;					// (4) po�et sm�r�
	long	Klid;					// (4) z toho po�et klidov�ch f�z�
	WORD	Width;					// (2) ���ka obr�zku (max. 32768)
	WORD	Height;					// (2) v��ka obr�zku (max. 32768)
	long	Delay;					// (4) prodleva mezi f�zemi v milisekund�ch
	long	Level;					// (4) hladina k vykreslov�n� (0=p�edm�ty)
	double	Kroku;					// (8) po�et f�z� na jednotkovou vzd�lenost (0=ihned)
	short	Colors;					// (2) po�et palet v tabulce
	short	Format;					// (2) form�t dat a parametry
									//			0 = nekomprimov�no (star� form�t)
									//			1 = komprese
	RGBQUAD	ColorTab[1];			// (4) tabulka palet
} SPRITEFILE;

// Za popisova�em n�sleduje tabulka palet (form�t RGBQUAD) a data obr�zk�
// V komprimovan�m form�tu n�sleduje za tabulkou palet dvojslovo=velikost zkomprimovan�ch
// dat obr�zk� (komprimov�no v�e najednou), n�sleduj� zkomprimovan� data obr�zk�

#define SIZEOFSPRITEFILE (4*sizeof(char) + 5*sizeof(long) + 2*sizeof(short) + 2*sizeof(WORD) + sizeof(double))

/////////////////////////////////////////////////////////////////////////////
// inicializa�n� pr�zdn� sprajt (modifikuje se po�et referenc�!)

extern SPRITEDATA* EmptySpriteData;				// data pr�zdn�ho sprajtu
extern	const CSprite		EmptySprite;		// pr�zdn� sprajt

/////////////////////////////////////////////////////////////////////////////
// statick� funkce

// statick� inicializace sprajt� (p�i chyb� pam�ti vrac� FALSE)
bool InitSprite();

// vytvo�en� dat sprajtu (p�i chyb� pam�ti vrac� NULL)
SPRITEDATA* _fastcall NewSpriteData(int faze, int smer);

// vytvo�en� dat sprajtu bez inicializace obr�zk� a nastaven� parametr� (p�i chyb� pam�ti vrac� NULL)
SPRITEDATA* _fastcall NewSpriteData0(int faze, int smer);

// zru�en� dat sprajtu (odd�leno kv�li lep�� optimalizaci)
void _fastcall DelSpriteData(SPRITEDATA* data);


/////////////////////////////////////////////////////////////////////////////
// sprajt

class CSprite
{

// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn�
	SPRITEDATA*		pData;					// ukazatel na z�hlav� sprajtu

// p�ipojen� dat sprajtu
	inline void attach(SPRITEDATA* data)
	{
		ASSERT(data != NULL);

		pData = data;
		LongIncrement(&(data->Refer));
	}

// odpojen� (a zru�en�) dat sprajtu
	inline void detach(SPRITEDATA* data)
	{
		ASSERT(data != NULL);
		ASSERT(data->Refer > 0);

		if (LongDecrement(&(pData->Refer)))
		{
			DelSpriteData(data);
		}
	}


// ---------------------------- ve�ejn� funkce ------------------------------

public:

// konstruktor a destruktor
	CSprite();						// pr�zdn� sprajt o velikosti 1x1 (pr�zdn� obr�zek ICONSIZE)
	CSprite(const CSprite& src);
	CSprite(SPRITEDATA* data);
	~CSprite();

// konstruktor a destruktor
	void Init();					// pr�zdn� sprajt o velikosti 1x1 (pr�zdn� obr�zek ICONSIZE)
	void _fastcall Init(const CSprite& src);
	void _fastcall Init(SPRITEDATA* data);
	bool _fastcall Init(int faze, int smer); // p�i chyb� pam�ti vr�t� FALSE, sprajt nen� vytvo�en
	void Term();

// poskytnut� ukazatele na data sprajtu
	inline SPRITEDATA* Data() const { return pData; };
	inline CPicture* DataData() const { return pData->Data; };

// poskytnut� po�tu f�z� sprajtu
	inline int Faze() const { return pData->Faze; };

// poskytnut� po�tu sm�r� sprajtu
	inline int Smer() const { return pData->Smer; };

// poskytnut�/nastaven� (bez zm�ny f�z� celkem) po�tu klidov�ch f�z� sprajtu
	inline int Klid() const { return pData->Klid; };
	inline void Klid(int klid) { pData->Klid = klid; };

// poskytnut�/nastaven� prodlevy mezi dv�ma f�zemi (v milisekund�ch)
	inline int Delay() const { return pData->Delay; };
	inline void Delay(int delay) { pData->Delay = delay; };

// poskytnut�/nastaven� hladiny zobrazen� sprajtu (0=p�edm�ty)
	inline int Level() const { return pData->Level; };
	inline void Level(int level) { pData->Level = level; };

// poskytnut�/nastaven� po�tu krok� na jednotku (0=ihned)
	inline double Kroku() const { return pData->Kroku; };
	inline void Kroku(double kroku) { pData->Kroku = kroku; };

// poskytnut� ���ky obr�zku sprajtu
	inline int Width() const { return pData->Data[0].Width(); };

// poskytnut� v��ky obr�zku sprajtu
	inline int Height() const { return pData->Data[0].Height(); };

// poskytnut� velikosti dat sprajtu - data obr�zk� bez z�hlav�
	inline int Size() const { return pData->Faze * pData->Smer * pData->Data[0].Width() * pData->Data[0].Height(); };

// vypr�zdn�n� (uvoln�n� dat) - p�ipoj� se standardn� pr�zdn� sprajt o velikosti 1x1
	void Empty();

// test, zda je sprajt pr�zdn� (zda ukazuje na standardn� pr�zdn� sprajt 1x1)
	inline BOOL IsEmpty() { return ((DWORD)pData == (DWORD)EmptySpriteData); };
	inline BOOL IsNotEmpty() { return ((DWORD)pData != (DWORD)EmptySpriteData); };

// kopie do vlastn�ho bufferu p�ed modifikac� (pozor - obr�zky t�mto nejsou p�ivlastn�ny!)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool CopyWrite();

// vytvo�en� nov�ho sprajtu (p�ipraveno k z�pisu, data jsou n�hodn�)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool _fastcall New(int faze, int smer);

// nastaven� nov�ch rozm�r� sprajtu (po�et f�z� pro klid a pohyb, po�et sm�r�)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool ReSize(int klid, int pohyb, int smer);

// kontrola platnosti offsetu prvku
	inline BOOL IsValid(const int off) const 
		{ return ((DWORD)off < (DWORD)(pData->Faze * pData->Smer)); };

	inline BOOL IsNotValid(const int off) const 
		{ return ((DWORD)off >= (DWORD)(pData->Faze * pData->Smer)); };

// kontrola platnosti indexu prvku
	inline BOOL IsValid(const int faze, const int smer) const
		{ return (((DWORD)faze < (DWORD)pData->Faze) && ((DWORD)smer < (DWORD)pData->Smer)); };

	inline BOOL IsNotValid(const int faze, const int smer) const
		{ return (((DWORD)faze >= (DWORD)pData->Faze) || ((DWORD)smer >= (DWORD)pData->Smer)); };

// poskytnut� p��stupu k prvku (bez kontroly offsetu/indexu)
// v p��pad� z�pisu mus� b�t zaji�t�no p�ivlastn�n� bufferu!
	inline CPicture& operator[] (const int off) 
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline const CPicture& operator[] (const int off) const
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline CPicture& At(const int off) 
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline const CPicture& At(const int off) const
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline CPicture& At(const int faze, const int smer)
		{ ASSERT(IsValid(faze, smer)); return pData->Data[faze + smer*pData->Faze]; }

	inline const CPicture& At(const int faze, const int smer) const
		{ ASSERT(IsValid(faze, smer)); return pData->Data[faze + smer*pData->Faze]; }

// poskytnut� prvku (s kontrolou platnosti offsetu/indexu)
// pro neplatn� prvek vrac� p��stup na prvek pr�zdn�ho sprajtu
	const CPicture& _fastcall Get(const int off) const;
	const CPicture& _fastcall Get(const int faze, const int smer) const;

// nastaven� prvku (s kontrolou platnosti offsetu/indexu)
// p�ed z�pisem je nutno zajistit p�ivlastn�n� bufferu!
	void _fastcall Set(const int off, const CPicture& data);
	void _fastcall Set(const int faze, const int smer, const CPicture& data);

// na�ten� sprajtu ze souboru
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool LoadFile(CText jmeno);

// ulo�en� do souboru (FALSE=chyba)
	bool SaveFile(CText jmeno) const;

// oper�tor p�i�azen�
	const CSprite& operator= (const CSprite& src);
	const CSprite& operator= (SPRITEDATA* src);
};

/***************************************************************************\
*																			*
*								Buffer sprajt�								*
*																			*
\***************************************************************************/

class CBufSprite : public CBuffer<CSprite>
{

// ---------------------------- ve�ejn� funkce ------------------------------

public:

// vytvo�en� nov�ho sprajtu (vrac� index polo�ky, <0 = chyba)
// prov�d� z�znam do UNDO bufferu
	inline int New() { return CBuffer<CSprite>::New(); } // pr�zdn� sprajt o velikosti 1x1
	int _fastcall New(int faze, int smer);			// data jsou n�hodn�
};

