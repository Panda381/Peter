
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
	long		Level;				// (4) hladina k p�ekreslov�n�
	double		Kroku;				// (8) po�et f�z� na jednotkovou vzd�lenost (0=sleduje ihned)
	CPicture	Data[1];			// obr�zky sprajtu (v po�ad� f�ze/sm�r)
} SPRITEDATA;

#define SIZEOFSPRITEDATA	(6*sizeof(long) + sizeof(double)) // velikost polo�ky dat sprajtu (bez dat)

extern SPRITEDATA* EmptySpriteData;				// data pr�zdn�ho sprajtu

/////////////////////////////////////////////////////////////////////////////
// popisova� sprajtu v souboru (40 B)

typedef struct SPRITEFILE_
{
	char	Ident[4];				// (4) identifik�tor (= "PSPR")
	long	Faze;					// (4) po�et f�z� celkem
	long	Smer;					// (4) po�et sm�r�
	long	Klid;					// (4) z toho po�et klidov�ch f�z�
	WORD	Width;					// (2) ���ka obr�zku
	WORD	Height;					// (2) v��ka obr�zku
	long	Delay;					// (4) prodleva mezi f�zemi v milisekund�ch
	long	Level;					// (4) hladina k vykreslov�n�
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

// statick� inicializace sprajt�
void InitSprite();


/////////////////////////////////////////////////////////////////////////////
// sprajt

class CSprite
{

// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn� - pouze ukazatel na data
	SPRITEDATA*		pData;					// ukazatel na z�hlav� sprajtu

// p�ipojen� dat
	inline void Attach(SPRITEDATA* data)
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
			CPicture* pic = pData->Data;
			for (int i = pData->Faze * pData->Smer; i > 0; i--)
			{
				pic->Term();
				pic++;
			}
			MemFree(pData);
#ifdef _DEBUG
			pData = NULL;
#endif
		}
	}

// vytvo�en� nov�ho bufferu - star� buffer mus� b�t odpojen! (obr�zky nastaveny na pr�zdn�)
	inline void NewBuffer(int faze, int smer)
	{
		ASSERT((faze > 0) && (smer > 0));
		if (faze <= 0) faze = 1;
		if (smer <= 0) smer = 1;
		SPRITEDATA* data = (SPRITEDATA*)MemGet(faze*smer*sizeof(CPicture) + SIZEOFSPRITEDATA);
		data->Refer = 1;				// po�et referenc�
		data->Faze = faze;				// po�et f�z�
		data->Smer = smer;				// po�et sm�r�
		data->Klid = 1;					// po�et klidov�ch f�z�
		data->Delay = SPRITEDEFDELAY;	// prodleva mezi f�zemi
		data->Level = SPRITEDEFLEVEL;	// hladina k p�ekreslov�n�
		data->Kroku = SPRITEDEFKROKU;	// po�et f�z� na jednotku
		pData = data;					// adresa dat

		CPicture* pic = data->Data;
		for (int i = faze*smer; i > 0; i--)
		{
			pic->Init();
			pic++;
		}
	}

// ---------------------------- ve�ejn� funkce ------------------------------

public:

// konstruktor a destruktor
	CSprite();								// standardn� konstruktor
	CSprite(const CSprite& src);			// kop�rovac� konstruktor
	CSprite(int faze, int smer);			// konstruktor s vytvo�en�m sprajtu
	~CSprite();								// standardn� destruktor

// konstruktor a destruktor
	void Init();							// statick� konstruktor
	void Init(SPRITEDATA* data);			// statick� konstruktor se zad�n�m dat
	void Init(int faze, int smer);			// statick� konstruktor s vytvo�en�m sprajtu
	void Term();							// statick� destruktor

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

// poskytnut�/nastaven� prodlevy mezi dv�ma f�zemi
	inline int Delay() const { return pData->Delay; };
	inline void Delay(int delay) { pData->Delay = delay; };

// poskytnut�/nastaven� hladiny zobrazen� sprajtu
	inline int Level() const { return pData->Level; };
	inline void Level(int level) { pData->Level = level; };

// poskytnut�/nastaven� po�tu krok� na jednotku (0 = ihned)
	inline double Kroku() const { return pData->Kroku; };
	inline void Kroku(double kroku) { pData->Kroku = kroku; };

// poskytnut� ���ky obr�zku sprajtu
	inline int Width() const { return pData->Data[0].Width(); };

// poskytnut� v��ky obr�zku sprajtu
	inline int Height() const { return pData->Data[0].Height(); };

// poskytnut� velikosti dat sprajtu - data obr�zk� bez z�hlav�
	inline int Size() const { return Faze() * Smer() * Width() * Height(); };

// kopie do vlastn�ho bufferu p�ed modifikac�
	void CopyWrite();

// vytvo�en� nov�ho sprajtu (p�ipraveno k z�pisu, data jsou n�hodn�)
	void New(int faze, int smer);

// nastaven� nov�ch rozm�r� sprajtu (po�et f�z� pro klid a pohyb, po�et sm�r�)
	void ReSize(int klid, int pohyb, int smer);

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
	const CPicture& _fastcall Get(const int off) const;
	const CPicture& _fastcall Get(const int faze, const int smer) const;

// nastaven� prvku (s kontrolou platnosti offsetu/indexu)
	void _fastcall Set(const int off, const CPicture& data);
	void _fastcall Set(const int faze, const int smer, const CPicture& data);

// na�ten� sprajtu ze souboru (TRUE=operace OK)
	BOOL LoadFile(CString jmeno);

// ulo�en� do souboru (FALSE=chyba)
	BOOL SaveFile(CString jmeno) const;

// oper�tor p�i�azen�
	const CSprite& operator= (const CSprite& src);
};

/***************************************************************************\
*																			*
*								Buffer sprajt�								*
*																			*
\***************************************************************************/

class CBufSprite
{

// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn�
	CSprite*	m_Data;		// ukazatel na data
	BYTE*		m_Valid;	// p��znaky platnosti polo�ek
	int			m_Num;		// po�et platn�ch polo�ek v bufferu
	int			m_Max;		// velikost bufferu (polo�ek)
	int			m_Next;		// p��t� voln� polo�ka (-1=nen�)

// vytvo�en� nov� polo�ky
	inline int NewItem()
	{
		if (m_Next < 0)				// nen� dal�� polo�ka?
		{
			NewData();				// vytvo�en� nov�ch dat
		}

		int i = m_Next;				// p��t� volna polo�ka
		m_Next = *(int*)(m_Data + i); // dal�� polo�ka
		m_Valid[i] = TRUE;			// nastaven� p��znaku platnosti polo�ky
		m_Num++;					// zv��en� ��ta�e platn�ch polo�ek
		return i;
	};

// vytvo�en� nov�ch dat (odd�leno kv�li lep�� optimalizaci)
	void NewData();

// ---------------------------- ve�ejn� funkce ------------------------------

public:

// konstruktor a destruktor
	CBufSprite();
	~CBufSprite();

// statick� konstruktor a destruktor
	void Init();			// statick� konstruktor
	void Term();			// statick� destruktor

// zru�en� v�ech polo�ek v bufferu (ukl�d�n� za�ne op�t po �ad� od za��tku)
	void DelAll();

// poskytnut� bufferu sprajt�
	inline CSprite* Data() const { return m_Data; };

// poskytnut� tabulky platnosti (hodnoty TRUE a FALSE)
	inline BYTE* Valid() const { return m_Valid; };

// poskytnut� po�tu platn�ch polo�ek v bufferu
	inline int Num() const { return m_Num; };

// poskytnut� velikosti bufferu (v�etn� zru�en�ch polo�ek)
	inline int Max() const { return m_Max; };

// kontrola platnosti polo�ky
	inline BOOL IsValid(const int index) const
		{ return (((DWORD)index < (DWORD)m_Max) && m_Valid[index]); };

	inline BOOL IsNotValid(const int index) const
		{ return (((DWORD)index >= (DWORD)m_Max) || !m_Valid[index]); };

// poskytnut� p��stupu k polo�ce (bez kontroly indexu)
	inline CSprite& operator[] (const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const CSprite& operator[] (const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline CSprite& At(const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const CSprite& At(const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

// poskytnut� polo�ky (s kontrolou platnosti indexu)
	const CSprite& _fastcall Get(const int index) const;

// nastaven� polo�ky (s kontrolou platnosti indexu)
	void _fastcall Set(const int index, const CSprite& data);

// zru�en� polo�ky (s kontrolou platnosti indexu)
	void _fastcall Del(const int index);

// vytvo�en� pr�zdn� polo�ky (vrac� index polo�ky)
	int New();
	int New(int faze, int smer);

// p�id�n� polo�ky (vrac� index polo�ky)
	int _fastcall Add(const CSprite& data);

// duplikace polo�ky (s kontrolou platnosti indexu, vrac� index polo�ky)
	int _fastcall Dup(const int index);

// oper�tor p�i�azen�
	const CBufSprite& operator= (const CBufSprite& src);
};

