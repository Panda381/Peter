
/***************************************************************************\
*																			*
*									Sprajt									*
*																			*
\***************************************************************************/

#define	SPRITEDEFDELAY	55			// implicitní prodleva
#define	SPRITEDEFLEVEL	1000		// implicitní hladina
#define	SPRITEDEFKROKU	8			// implicitní poèet krokù

#define SPRITEMAXKLID  1024			// maximální poèet klidovıch fází
#define SPRITEMAXPOHYB 1024			// maximální poèet fází pro pohyb
#define SPRITEMAXFAZE  (SPRITEMAXKLID+SPRITEMAXPOHYB) // max. poèet fází
#define SPRITEMAXSMER  1024			// maximální poèet smìrù
#define SPRITEMAXDELAY 3600000		// maximální prodleva mezi fázemi
#define SPRITEMAXKROKU 1000000		// maximální poèet krokù na jednotku

/////////////////////////////////////////////////////////////////////////////
// struktura poloky dat sprajtu (32 bajtù + data)
// Všechny obrázky mají stejnou velikost, musí bıt minimálnì 1 obrázek,
// - velikost obrázku se zjišuje z prvního obrázku

typedef struct SPRITEDATA_
{
	long		Refer;				// (4) èítaè referencí na sprajt
	long		Faze;				// (4) poèet fází celkem
	long		Smer;				// (4) poèet smìrù
	long		Klid;				// (4) z toho poèet klidovıch fází
	long		Delay;				// (4) prodleva mezi dvìma fázemi (v milisekundách)
	long		Level;				// (4) hladina k pøekreslování
	double		Kroku;				// (8) poèet fází na jednotkovou vzdálenost (0=sleduje ihned)
	CPicture	Data[1];			// obrázky sprajtu (v poøadí fáze/smìr)
} SPRITEDATA;

#define SIZEOFSPRITEDATA	(6*sizeof(long) + sizeof(double)) // velikost poloky dat sprajtu (bez dat)

extern SPRITEDATA* EmptySpriteData;				// data prázdného sprajtu

/////////////////////////////////////////////////////////////////////////////
// popisovaè sprajtu v souboru (40 B)

typedef struct SPRITEFILE_
{
	char	Ident[4];				// (4) identifikátor (= "PSPR")
	long	Faze;					// (4) poèet fází celkem
	long	Smer;					// (4) poèet smìrù
	long	Klid;					// (4) z toho poèet klidovıch fází
	WORD	Width;					// (2) šíøka obrázku
	WORD	Height;					// (2) vıška obrázku
	long	Delay;					// (4) prodleva mezi fázemi v milisekundách
	long	Level;					// (4) hladina k vykreslování (0 = pøedmìty)
	double	Kroku;					// (8) poèet fází na jednotkovou vzdálenost (0=ihned)
	short	Colors;					// (2) poèet palet v tabulce
	short	Format;					// (2) formát dat a parametry
									//			0 = nekomprimováno (starı formát)
									//			1 = komprese
	RGBQUAD	ColorTab[1];			// (4) tabulka palet
} SPRITEFILE;

// Za popisovaèem následuje tabulka palet (formát RGBQUAD) a data obrázkù
// V komprimovaném formátu následuje za tabulkou palet dvojslovo=velikost zkomprimovanıch
// dat obrázkù (komprimováno vše najednou), následují zkomprimovaná data obrázkù

#define SIZEOFSPRITEFILE (4*sizeof(char) + 5*sizeof(long) + 2*sizeof(short) + 2*sizeof(WORD) + sizeof(double))

// statická inicializace sprajtù (pøi chybì pamìti vrací FALSE)
BOOL InitSprite();


/////////////////////////////////////////////////////////////////////////////
// sprajt

class CSprite
{

// ------------------------- interní promìnné a funkce ----------------------

private:

// promìnné
	SPRITEDATA*		pData;					// (4) ukazatel na záhlaví sprajtu

	bool			m_Visible;				// (1) pøíznak viditelnosti sprajtu
	bool			m_Moving;				// (1) pøíznak pohybu sprajtu
	bool			m_DispReq;				// (1) pøíznak poadavku pøekreslení
	bool			m_KlidMove;				// (1) je pohyb v klidu

	int				m_X;					// (4) aktuální souøadnice X (v bodech)
	int				m_Y;					// (4) aktuální souøadnice Y (v bodech)

	int				m_Timer;				// (4) èítaè hodin pro další fázi
	int				m_KrokCit;				// (4) èítaè fází pro celı pohyb
	int				m_FazeN;				// (4) aktuální fáze (pozice ve sprajtu)
	int				m_SmerN;				// (4) aktuální smìr (øádek ve sprajtu)

	double			m_CilX;					// (8) cílová souøadnice X (v políèkách)
	double			m_CilY;					// (8) cílová souøadnice Y (v políèkách)

	double			m_AktX;					// (8) aktuální souøadnice X (v políèkách)
	double			m_AktY;					// (8) aktuální souøadnice Y (v políèkách)

	double			m_dX;					// (8) pøírustek souøadnice X (v políèkách)
	double			m_dY;					// (8) pøírustek souøadnice Y (v políèkách)

	double			m_Smer;					// (8) aktuální smìr (v úhlové míøe)
	int				m_FazeNext;				// (4) pøíští fáze (urèená uivatelem), mimo rozsah=implicitní

	bool			m_StatusVis;			// (1) pøíznak zobrazení stavového indikátoru
	bool			m_res1;
	bool			m_res2;
	bool			m_res3;

	double			m_Status;				// (8) stav * 0x40000000 (0 a 1, <= -1 vypnuto)

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

// vytvoøení nového bufferu - starı buffer musí bıt odpojen! (obrázky nastaveny na prázdné)
	inline void NewBuffer(int faze, int smer)
	{
		ASSERT((faze > 0) && (smer > 0));
		if (faze <= 0) faze = 1;
		if (smer <= 0) smer = 1;
		SPRITEDATA* data = (SPRITEDATA*)MemGet(faze*smer*sizeof(CIcon) + SIZEOFSPRITEDATA);
		data->Refer = 1;				// poèet referencí
		data->Faze = faze;				// poèet fází
		data->Smer = smer;				// poèet smìrù
		data->Klid = 1;					// poèet klidovıch fází
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
	void Init();							// statickı konstruktor
	void Init(SPRITEDATA* data);			// statickı konstruktor se zadáním dat
	void Init(int faze, int smer);			// statickı konstruktor s vytvoøením sprajtu
	void Term();							// statickı destruktor

// poskytnutí ukazatele na data sprajtu
	inline SPRITEDATA* Data() const { return pData; };
	inline CPicture* DataData() const { return pData->Data; };

// poskytnutí poètu fází sprajtu
	inline int Faze() const { return pData->Faze; };

// poskytnutí poètu smìrù sprajtu
	inline int Smer() const { return pData->Smer; };

// poskytnutí/nastavení (bez zmìny fází celkem) poètu klidovıch fází sprajtu
	inline int Klid() const { return pData->Klid; };
	inline void Klid(int klid) { pData->Klid = klid; };

// poskytnutí/nastavení prodlevy mezi dvìma fázemi
	inline int Delay() const { return pData->Delay; };
	inline void Delay(int delay) { pData->Delay = delay; };

// poskytnutí/nastavení hladiny zobrazení sprajtu (0 = pøedmìty)
	inline int Level() const { return pData->Level; };
	inline void Level(int level) { pData->Level = level; };

// poskytnutí/nastavení poètu krokù na jednotku (0 = ihned)
	inline double Kroku() const { return pData->Kroku; };
	inline void Kroku(double kroku) { pData->Kroku = kroku; };

// poskytnutí šíøky obrázku sprajtu
	inline int Width() const { return pData->Data[0].Width(); };

// poskytnutí vıšky obrázku sprajtu
	inline int Height() const { return pData->Data[0].Height(); };

// poskytnutí velikosti dat sprajtu - data obrázkù bez záhlaví
	inline int Size() const { return Faze() * Smer() * Width() * Height(); };

// poskytnutí/nastavení pøíznaku viditelnosti
	inline bool Visible() const { return m_Visible; };
	inline void Visible(bool visible) { m_Visible = visible; };

// poskytnutí/nastavení pøíznaku pohybu
	inline bool Moving() const { return m_Moving; };
	inline void Moving(bool moving) { m_Moving = moving; };

// poskytnutí/nastavení pøíznaku poadavku pøekreslení
	inline bool DispReq() const { return m_DispReq; };
	inline void DispReq(bool dispreq) { m_DispReq = dispreq; };

// poskytnutí/nastavení pøíznaku pohybu v klidu
	inline bool KlidMove() const { return m_KlidMove; };
	inline void KlidMove(bool klidmove) { m_KlidMove = klidmove; };

// poskytnutí/nastavení/dekrementace èítaèe hodin pro další fázi
	inline int Timer() const { return m_Timer; };
	inline void Timer(int timer) { m_Timer = timer; };
	inline BOOL TimerDec() { m_Timer--; return (m_Timer <= 0); };

// poskytnutí/nastavení/dekrementace èítaèe fází pro celı pohyb
	inline int KrokCit() const { return m_KrokCit; };
	inline void KrokCit(int krokcit) { m_KrokCit = krokcit; };
	inline BOOL KrokDec() { m_KrokCit--; return (m_KrokCit < 0); };

// poskytnutí/nastavení aktuální fáze
	inline int FazeN() const { return m_FazeN; };
	inline void FazeN(int fazen) { m_FazeN = fazen; };

// poskytnutí/nastavení pøíští fáze (mimo rozsah=implicitní)
	inline int FazeNext() const { return m_FazeNext; };
	inline void FazeNext(int fazenext) { m_FazeNext = fazenext; };

// poskytnutí/nastavení stavu (rozsah 0 a 1, <0=vypnuto)
	inline double Status() const { return m_Status; };
	inline bool IsStatus() const { return m_StatusVis; };
	inline void Status(double status) { m_Status = status; m_StatusVis = (m_Status > -1); };

// poskytnutí/nastavení aktuálního smìru
	inline int SmerN() const { return m_SmerN; };
	inline void SmerN(int smern) { m_SmerN = smern; };

// poskytnutí dat aktuálního obrázku (podle aktuálního smìru a fáze, bez kontroly)
	PICTUREDATA* AktPicData();

// poskytnutí/nastavení cílové souøadnice X (v políèkách)
	inline double CilX() const { return m_CilX; };
	inline void CilX(double cilx) { m_CilX = cilx; };
	inline double* PCilX() { return &m_CilX; };

// poskytnutí/nastavení cílové souøadnice Y (v políèkách)
	inline double CilY() const { return m_CilY; };
	inline void CilY(double cily) { m_CilY = cily; };
	inline double* PCilY() { return &m_CilY; };

// poskytnutí/nastavení aktuální souøadnice X (v políèkách)
	inline double AktX() const { return m_AktX; };
	inline void AktX(double aktx) { m_AktX = aktx; };

// poskytnutí/nastavení aktuální souøadnice Y (v políèkách)
	inline double AktY() const { return m_AktY; };
	inline void AktY(double akty) { m_AktY = akty; };

// poskytnutí/nastavení aktuální souøadnice X (v bodech)
	inline int X() const { return m_X; };
	inline void X(int x) { m_X = x; };

// poskytnutí/nastavení aktuální souøadnice Y (v bodech)
	inline int Y() const { return m_Y; };
	inline void Y(int y) { m_Y = y; };

// poskytnutí/nastavení pøirustku souøadnice X (v políèkách)
	inline double dX() const { return m_dX; };
	inline void dX(double dx) { m_dX = dx; };

// poskytnutí/nastavení pøirustku souøadnice Y (v políèkách)
	inline double dY() const { return m_dY; };
	inline void dY(double dy) { m_dY = dy; };

// poskytnutí/nastavení aktuálního smìru (v úhlové míøe)
	inline double SmerR() const { return m_Smer; };
	inline void SmerR(double smerr) { m_Smer = smerr; };

// kopie do vlastního bufferu pøed modifikací
	void CopyWrite();

// vyprázdnìní sprajtu (uvolnìní dat)
	void Empty();

// test, zda je sprajt prázdnı
	inline BOOL IsEmpty() { return ((DWORD)pData == (DWORD)EmptySpriteData); };
	inline BOOL IsNotEmpty() { return ((DWORD)pData != (DWORD)EmptySpriteData); };

// vytvoøení nového sprajtu (pøipraveno k zápisu, data jsou náhodná)
	void New(int faze, int smer);

// nastavení novıch rozmìrù sprajtu (poèet fází pro klid a pohyb, poèet smìrù)
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

// naètení sprajtu ze souboru
	void LoadFile();

// uloení do souboru
	void SaveFile() const;

// operátor pøiøazení
	const CSprite& operator= (const CSprite& src);

// operátory porovnání
	friend inline BOOL operator==(const CSprite& s1, const CSprite& s2)
		{ return (DWORD)s1.pData == (DWORD)s2.pData; };

	friend inline BOOL operator!=(const CSprite& s1, const CSprite& s2)
		{ return (DWORD)s1.pData != (DWORD)s2.pData; };
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
	int			m_Num;		// poèet platnıch poloek v bufferu
	int			m_Max;		// velikost bufferu (poloek)

// vytvoøení nové poloky
	inline int NewItem()
	{
		int i = m_Num;				// poèet poloek
		if (i >= m_Max)				// není další poloka?
		{
			NewData();				// vytvoøení novıch dat
		}

		m_Num = i + 1;
		return i;
	};

// vytvoøení novıch dat (oddìleno kvùli lepší optimalizaci)
	void NewData();

// ---------------------------- veøejné funkce ------------------------------

public:

// konstruktor a destruktor
	CBufSprite();
	~CBufSprite();

// statickı konstruktor a destruktor
	void Init();			// statickı konstruktor
	void Term();			// statickı destruktor

// zrušení všech poloek v bufferu (ukládání zaène opìt po øadì od zaèátku)
	void DelAll();

// poskytnutí bufferu sprajtù
	inline CSprite* Data() const { return m_Data; };

// poskytnutí poètu platnıch poloek v bufferu
	inline int Num() const { return m_Num; };

// poskytnutí velikosti bufferu
	inline int Max() const { return m_Max; };

// kontrola platnosti poloky
	inline BOOL IsValid(const int index) const
		{ return ((DWORD)index < (DWORD)m_Num); };

	inline BOOL IsNotValid(const int index) const
		{ return ((DWORD)index >= (DWORD)m_Num); };

// poskytnutí pøístupu k poloce (bez kontroly indexu)
	inline CSprite& operator[] (const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const CSprite& operator[] (const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline CSprite& At(const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const CSprite& At(const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

// poskytnutí poloky (s kontrolou platnosti indexu)
	const CSprite& _fastcall Get(const int index) const;

// nastavení poloky (s kontrolou platnosti indexu)
	void _fastcall Set(const int index, const CSprite& data);

// vyprázdnìní poloky (bez jejího zrušení - jen pro uvolnìní dat)
	void _fastcall Empty(const int index);

// zrušení poloek z konce bufferu
	void _fastcall Del(int num);

// vytvoøení prázdné poloky (vrací index poloky)
	int New();
	int New(int faze, int smer);

// pøidání poloky (vrací index poloky)
	int _fastcall Add(const CSprite& data);

// duplikace poloky (s kontrolou platnosti indexu, vrací index první poloky)
	int _fastcall Dup(const int index);
	int _fastcall Dup(const int index, int num);

// operátor pøiøazení
	const CBufSprite& operator= (const CBufSprite& src);
};

