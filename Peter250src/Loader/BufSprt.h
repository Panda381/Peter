
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
	long	Level;					// (4) hladina k vykreslov�n� (0 = p�edm�ty)
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

// statick� inicializace sprajt� (p�i chyb� pam�ti vrac� FALSE)
BOOL InitSprite();


/////////////////////////////////////////////////////////////////////////////
// sprajt

class CSprite
{

// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn�
	SPRITEDATA*		pData;					// (4) ukazatel na z�hlav� sprajtu

	bool			m_Visible;				// (1) p��znak viditelnosti sprajtu
	bool			m_Moving;				// (1) p��znak pohybu sprajtu
	bool			m_DispReq;				// (1) p��znak po�adavku p�ekreslen�
	bool			m_KlidMove;				// (1) je pohyb v klidu

	int				m_X;					// (4) aktu�ln� sou�adnice X (v bodech)
	int				m_Y;					// (4) aktu�ln� sou�adnice Y (v bodech)

	int				m_Timer;				// (4) ��ta� hodin pro dal�� f�zi
	int				m_KrokCit;				// (4) ��ta� f�z� pro cel� pohyb
	int				m_FazeN;				// (4) aktu�ln� f�ze (pozice ve sprajtu)
	int				m_SmerN;				// (4) aktu�ln� sm�r (��dek ve sprajtu)

	double			m_CilX;					// (8) c�lov� sou�adnice X (v pol��k�ch)
	double			m_CilY;					// (8) c�lov� sou�adnice Y (v pol��k�ch)

	double			m_AktX;					// (8) aktu�ln� sou�adnice X (v pol��k�ch)
	double			m_AktY;					// (8) aktu�ln� sou�adnice Y (v pol��k�ch)

	double			m_dX;					// (8) p��rustek sou�adnice X (v pol��k�ch)
	double			m_dY;					// (8) p��rustek sou�adnice Y (v pol��k�ch)

	double			m_Smer;					// (8) aktu�ln� sm�r (v �hlov� m��e)
	int				m_FazeNext;				// (4) p��t� f�ze (ur�en� u�ivatelem), mimo rozsah=implicitn�

	bool			m_StatusVis;			// (1) p��znak zobrazen� stavov�ho indik�toru
	bool			m_res1;
	bool			m_res2;
	bool			m_res3;

	double			m_Status;				// (8) stav * 0x40000000 (0 a� 1, <= -1 vypnuto)

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
		SPRITEDATA* data = (SPRITEDATA*)MemGet(faze*smer*sizeof(CIcon) + SIZEOFSPRITEDATA);
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

// poskytnut�/nastaven� hladiny zobrazen� sprajtu (0 = p�edm�ty)
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

// poskytnut�/nastaven� p��znaku viditelnosti
	inline bool Visible() const { return m_Visible; };
	inline void Visible(bool visible) { m_Visible = visible; };

// poskytnut�/nastaven� p��znaku pohybu
	inline bool Moving() const { return m_Moving; };
	inline void Moving(bool moving) { m_Moving = moving; };

// poskytnut�/nastaven� p��znaku po�adavku p�ekreslen�
	inline bool DispReq() const { return m_DispReq; };
	inline void DispReq(bool dispreq) { m_DispReq = dispreq; };

// poskytnut�/nastaven� p��znaku pohybu v klidu
	inline bool KlidMove() const { return m_KlidMove; };
	inline void KlidMove(bool klidmove) { m_KlidMove = klidmove; };

// poskytnut�/nastaven�/dekrementace ��ta�e hodin pro dal�� f�zi
	inline int Timer() const { return m_Timer; };
	inline void Timer(int timer) { m_Timer = timer; };
	inline BOOL TimerDec() { m_Timer--; return (m_Timer <= 0); };

// poskytnut�/nastaven�/dekrementace ��ta�e f�z� pro cel� pohyb
	inline int KrokCit() const { return m_KrokCit; };
	inline void KrokCit(int krokcit) { m_KrokCit = krokcit; };
	inline BOOL KrokDec() { m_KrokCit--; return (m_KrokCit < 0); };

// poskytnut�/nastaven� aktu�ln� f�ze
	inline int FazeN() const { return m_FazeN; };
	inline void FazeN(int fazen) { m_FazeN = fazen; };

// poskytnut�/nastaven� p��t� f�ze (mimo rozsah=implicitn�)
	inline int FazeNext() const { return m_FazeNext; };
	inline void FazeNext(int fazenext) { m_FazeNext = fazenext; };

// poskytnut�/nastaven� stavu (rozsah 0 a� 1, <0=vypnuto)
	inline double Status() const { return m_Status; };
	inline bool IsStatus() const { return m_StatusVis; };
	inline void Status(double status) { m_Status = status; m_StatusVis = (m_Status > -1); };

// poskytnut�/nastaven� aktu�ln�ho sm�ru
	inline int SmerN() const { return m_SmerN; };
	inline void SmerN(int smern) { m_SmerN = smern; };

// poskytnut� dat aktu�ln�ho obr�zku (podle aktu�ln�ho sm�ru a f�ze, bez kontroly)
	PICTUREDATA* AktPicData();

// poskytnut�/nastaven� c�lov� sou�adnice X (v pol��k�ch)
	inline double CilX() const { return m_CilX; };
	inline void CilX(double cilx) { m_CilX = cilx; };
	inline double* PCilX() { return &m_CilX; };

// poskytnut�/nastaven� c�lov� sou�adnice Y (v pol��k�ch)
	inline double CilY() const { return m_CilY; };
	inline void CilY(double cily) { m_CilY = cily; };
	inline double* PCilY() { return &m_CilY; };

// poskytnut�/nastaven� aktu�ln� sou�adnice X (v pol��k�ch)
	inline double AktX() const { return m_AktX; };
	inline void AktX(double aktx) { m_AktX = aktx; };

// poskytnut�/nastaven� aktu�ln� sou�adnice Y (v pol��k�ch)
	inline double AktY() const { return m_AktY; };
	inline void AktY(double akty) { m_AktY = akty; };

// poskytnut�/nastaven� aktu�ln� sou�adnice X (v bodech)
	inline int X() const { return m_X; };
	inline void X(int x) { m_X = x; };

// poskytnut�/nastaven� aktu�ln� sou�adnice Y (v bodech)
	inline int Y() const { return m_Y; };
	inline void Y(int y) { m_Y = y; };

// poskytnut�/nastaven� p�irustku sou�adnice X (v pol��k�ch)
	inline double dX() const { return m_dX; };
	inline void dX(double dx) { m_dX = dx; };

// poskytnut�/nastaven� p�irustku sou�adnice Y (v pol��k�ch)
	inline double dY() const { return m_dY; };
	inline void dY(double dy) { m_dY = dy; };

// poskytnut�/nastaven� aktu�ln�ho sm�ru (v �hlov� m��e)
	inline double SmerR() const { return m_Smer; };
	inline void SmerR(double smerr) { m_Smer = smerr; };

// kopie do vlastn�ho bufferu p�ed modifikac�
	void CopyWrite();

// vypr�zdn�n� sprajtu (uvoln�n� dat)
	void Empty();

// test, zda je sprajt pr�zdn�
	inline BOOL IsEmpty() { return ((DWORD)pData == (DWORD)EmptySpriteData); };
	inline BOOL IsNotEmpty() { return ((DWORD)pData != (DWORD)EmptySpriteData); };

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

// na�ten� sprajtu ze souboru
	void LoadFile();

// ulo�en� do souboru
	void SaveFile() const;

// oper�tor p�i�azen�
	const CSprite& operator= (const CSprite& src);

// oper�tory porovn�n�
	friend inline BOOL operator==(const CSprite& s1, const CSprite& s2)
		{ return (DWORD)s1.pData == (DWORD)s2.pData; };

	friend inline BOOL operator!=(const CSprite& s1, const CSprite& s2)
		{ return (DWORD)s1.pData != (DWORD)s2.pData; };
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
	CBufSprite();
	~CBufSprite();

// statick� konstruktor a destruktor
	void Init();			// statick� konstruktor
	void Term();			// statick� destruktor

// zru�en� v�ech polo�ek v bufferu (ukl�d�n� za�ne op�t po �ad� od za��tku)
	void DelAll();

// poskytnut� bufferu sprajt�
	inline CSprite* Data() const { return m_Data; };

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

// vypr�zdn�n� polo�ky (bez jej�ho zru�en� - jen pro uvoln�n� dat)
	void _fastcall Empty(const int index);

// zru�en� polo�ek z konce bufferu
	void _fastcall Del(int num);

// vytvo�en� pr�zdn� polo�ky (vrac� index polo�ky)
	int New();
	int New(int faze, int smer);

// p�id�n� polo�ky (vrac� index polo�ky)
	int _fastcall Add(const CSprite& data);

// duplikace polo�ky (s kontrolou platnosti indexu, vrac� index prvn� polo�ky)
	int _fastcall Dup(const int index);
	int _fastcall Dup(const int index, int num);

// oper�tor p�i�azen�
	const CBufSprite& operator= (const CBufSprite& src);
};

