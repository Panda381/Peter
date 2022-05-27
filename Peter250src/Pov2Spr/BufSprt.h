
/***************************************************************************\
*																			*
*									Sprajt									*
*																			*
\***************************************************************************/

#define	SPRITEDEFDELAY	55			// implicitní prodleva
#define	SPRITEDEFLEVEL	1000		// implicitní hladina
#define	SPRITEDEFKROKU	8			// implicitní poèet krokù

#define SPRITEMAXKLID  1024			// maximální poèet klidových fází
#define SPRITEMAXPOHYB 1024			// maximální poèet fází pro pohyb
#define SPRITEMAXFAZE  (SPRITEMAXKLID+SPRITEMAXPOHYB) // max. poèet fází
#define SPRITEMAXSMER  1024			// maximální poèet smìrù
#define SPRITEMAXDELAY 3600000		// maximální prodleva mezi fázemi
#define SPRITEMAXKROKU 1000000		// maximální poèet krokù na jednotku

/////////////////////////////////////////////////////////////////////////////
// struktura položky dat sprajtu (32 bajtù + data)
// Všechny obrázky mají stejnou velikost, musí být minimálnì 1 obrázek,
// - velikost obrázku se zjišuje z prvního obrázku

typedef struct SPRITEDATA_
{
	long		Refer;				// (4) èítaè referencí na sprajt
	long		Faze;				// (4) poèet fází celkem
	long		Smer;				// (4) poèet smìrù
	long		Klid;				// (4) z toho poèet klidových fází
	long		Delay;				// (4) prodleva mezi dvìma fázemi (v milisekundách)
	long		Level;				// (4) hladina k pøekreslování
	double		Kroku;				// (8) poèet fází na jednotkovou vzdálenost (0=sleduje ihned)
	CPicture	Data[1];			// obrázky sprajtu (v poøadí fáze/smìr)
} SPRITEDATA;

#define SIZEOFSPRITEDATA	(6*sizeof(long) + sizeof(double)) // velikost položky dat sprajtu (bez dat)

extern SPRITEDATA* EmptySpriteData;				// data prázdného sprajtu

/////////////////////////////////////////////////////////////////////////////
// popisovaè sprajtu v souboru (40 B)

typedef struct SPRITEFILE_
{
	char	Ident[4];				// (4) identifikátor (= "PSPR")
	long	Faze;					// (4) poèet fází celkem
	long	Smer;					// (4) poèet smìrù
	long	Klid;					// (4) z toho poèet klidových fází
	WORD	Width;					// (2) šíøka obrázku
	WORD	Height;					// (2) výška obrázku
	long	Delay;					// (4) prodleva mezi fázemi v milisekundách
	long	Level;					// (4) hladina k vykreslování
	double	Kroku;					// (8) poèet fází na jednotkovou vzdálenost (0=ihned)
	short	Colors;					// (2) poèet palet v tabulce
	short	Format;					// (2) formát dat a parametry
									//			0 = nekomprimováno (starý formát)
									//			1 = komprese
	RGBQUAD	ColorTab[1];			// (4) tabulka palet
} SPRITEFILE;

// Za popisovaèem následuje tabulka palet (formát RGBQUAD) a data obrázkù
// V komprimovaném formátu následuje za tabulkou palet dvojslovo=velikost zkomprimovaných
// dat obrázkù (komprimováno vše najednou), následují zkomprimovaná data obrázkù

#define SIZEOFSPRITEFILE (4*sizeof(char) + 5*sizeof(long) + 2*sizeof(short) + 2*sizeof(WORD) + sizeof(double))

// statická inicializace sprajtù
void InitSprite();


/////////////////////////////////////////////////////////////////////////////
// sprajt

class CSprite
{

// ------------------------- interní promìnné a funkce ----------------------

private:

// promìnné - pouze ukazatel na data
	SPRITEDATA*		pData;					// ukazatel na záhlaví sprajtu

// pøipojení dat
	inline void Attach(SPRITEDATA* data)
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

// vytvoøení nového bufferu - starý buffer musí být odpojen! (obrázky nastaveny na prázdné)
	inline void NewBuffer(int faze, int smer)
	{
		ASSERT((faze > 0) && (smer > 0));
		if (faze <= 0) faze = 1;
		if (smer <= 0) smer = 1;
		SPRITEDATA* data = (SPRITEDATA*)MemGet(faze*smer*sizeof(CPicture) + SIZEOFSPRITEDATA);
		data->Refer = 1;				// poèet referencí
		data->Faze = faze;				// poèet fází
		data->Smer = smer;				// poèet smìrù
		data->Klid = 1;					// poèet klidových fází
		data->Delay = SPRITEDEFDELAY;	// prodleva mezi fázemi
		data->Level = SPRITEDEFLEVEL;	// hladina k pøekreslování
		data->Kroku = SPRITEDEFKROKU;	// poèet fází na jednotku
		pData = data;					// adresa dat

		CPicture* pic = data->Data;
		for (int i = faze*smer; i > 0; i--)
		{
			pic->Init();
			pic++;
		}
	}

// ---------------------------- veøejné funkce ------------------------------

public:

// konstruktor a destruktor
	CSprite();								// standardní konstruktor
	CSprite(const CSprite& src);			// kopírovací konstruktor
	CSprite(int faze, int smer);			// konstruktor s vytvoøením sprajtu
	~CSprite();								// standardní destruktor

// konstruktor a destruktor
	void Init();							// statický konstruktor
	void Init(SPRITEDATA* data);			// statický konstruktor se zadáním dat
	void Init(int faze, int smer);			// statický konstruktor s vytvoøením sprajtu
	void Term();							// statický destruktor

// poskytnutí ukazatele na data sprajtu
	inline SPRITEDATA* Data() const { return pData; };
	inline CPicture* DataData() const { return pData->Data; };

// poskytnutí poètu fází sprajtu
	inline int Faze() const { return pData->Faze; };

// poskytnutí poètu smìrù sprajtu
	inline int Smer() const { return pData->Smer; };

// poskytnutí/nastavení (bez zmìny fází celkem) poètu klidových fází sprajtu
	inline int Klid() const { return pData->Klid; };
	inline void Klid(int klid) { pData->Klid = klid; };

// poskytnutí/nastavení prodlevy mezi dvìma fázemi
	inline int Delay() const { return pData->Delay; };
	inline void Delay(int delay) { pData->Delay = delay; };

// poskytnutí/nastavení hladiny zobrazení sprajtu
	inline int Level() const { return pData->Level; };
	inline void Level(int level) { pData->Level = level; };

// poskytnutí/nastavení poètu krokù na jednotku (0 = ihned)
	inline double Kroku() const { return pData->Kroku; };
	inline void Kroku(double kroku) { pData->Kroku = kroku; };

// poskytnutí šíøky obrázku sprajtu
	inline int Width() const { return pData->Data[0].Width(); };

// poskytnutí výšky obrázku sprajtu
	inline int Height() const { return pData->Data[0].Height(); };

// poskytnutí velikosti dat sprajtu - data obrázkù bez záhlaví
	inline int Size() const { return Faze() * Smer() * Width() * Height(); };

// kopie do vlastního bufferu pøed modifikací
	void CopyWrite();

// vytvoøení nového sprajtu (pøipraveno k zápisu, data jsou náhodná)
	void New(int faze, int smer);

// nastavení nových rozmìrù sprajtu (poèet fází pro klid a pohyb, poèet smìrù)
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

// poskytnutí pøístupu k prvku (bez kontroly offsetu/indexu)
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

// poskytnutí prvku (s kontrolou platnosti offsetu/indexu)
	const CPicture& _fastcall Get(const int off) const;
	const CPicture& _fastcall Get(const int faze, const int smer) const;

// nastavení prvku (s kontrolou platnosti offsetu/indexu)
	void _fastcall Set(const int off, const CPicture& data);
	void _fastcall Set(const int faze, const int smer, const CPicture& data);

// naètení sprajtu ze souboru (TRUE=operace OK)
	BOOL LoadFile(CString jmeno);

// uložení do souboru (FALSE=chyba)
	BOOL SaveFile(CString jmeno) const;

// operátor pøiøazení
	const CSprite& operator= (const CSprite& src);
};

/***************************************************************************\
*																			*
*								Buffer sprajtù								*
*																			*
\***************************************************************************/

class CBufSprite
{

// ------------------------- interní promìnné a funkce ----------------------

private:

// promìnné
	CSprite*	m_Data;		// ukazatel na data
	BYTE*		m_Valid;	// pøíznaky platnosti položek
	int			m_Num;		// poèet platných položek v bufferu
	int			m_Max;		// velikost bufferu (položek)
	int			m_Next;		// pøíští volná položka (-1=není)

// vytvoøení nové položky
	inline int NewItem()
	{
		if (m_Next < 0)				// není další položka?
		{
			NewData();				// vytvoøení nových dat
		}

		int i = m_Next;				// pøíští volna položka
		m_Next = *(int*)(m_Data + i); // další položka
		m_Valid[i] = TRUE;			// nastavení pøíznaku platnosti položky
		m_Num++;					// zvýšení èítaèe platných položek
		return i;
	};

// vytvoøení nových dat (oddìleno kvùli lepší optimalizaci)
	void NewData();

// ---------------------------- veøejné funkce ------------------------------

public:

// konstruktor a destruktor
	CBufSprite();
	~CBufSprite();

// statický konstruktor a destruktor
	void Init();			// statický konstruktor
	void Term();			// statický destruktor

// zrušení všech položek v bufferu (ukládání zaène opìt po øadì od zaèátku)
	void DelAll();

// poskytnutí bufferu sprajtù
	inline CSprite* Data() const { return m_Data; };

// poskytnutí tabulky platnosti (hodnoty TRUE a FALSE)
	inline BYTE* Valid() const { return m_Valid; };

// poskytnutí poètu platných položek v bufferu
	inline int Num() const { return m_Num; };

// poskytnutí velikosti bufferu (vèetnì zrušených položek)
	inline int Max() const { return m_Max; };

// kontrola platnosti položky
	inline BOOL IsValid(const int index) const
		{ return (((DWORD)index < (DWORD)m_Max) && m_Valid[index]); };

	inline BOOL IsNotValid(const int index) const
		{ return (((DWORD)index >= (DWORD)m_Max) || !m_Valid[index]); };

// poskytnutí pøístupu k položce (bez kontroly indexu)
	inline CSprite& operator[] (const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const CSprite& operator[] (const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline CSprite& At(const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const CSprite& At(const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

// poskytnutí položky (s kontrolou platnosti indexu)
	const CSprite& _fastcall Get(const int index) const;

// nastavení položky (s kontrolou platnosti indexu)
	void _fastcall Set(const int index, const CSprite& data);

// zrušení položky (s kontrolou platnosti indexu)
	void _fastcall Del(const int index);

// vytvoøení prázdné položky (vrací index položky)
	int New();
	int New(int faze, int smer);

// pøidání položky (vrací index položky)
	int _fastcall Add(const CSprite& data);

// duplikace položky (s kontrolou platnosti indexu, vrací index položky)
	int _fastcall Dup(const int index);

// operátor pøiøazení
	const CBufSprite& operator= (const CBufSprite& src);
};

