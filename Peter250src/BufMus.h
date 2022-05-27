
/***************************************************************************\
*																			*
*									Hudba									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// struktura polo�ky dat hudby - 16 bajt�

typedef struct MUSICDATA_
{
	long	Refer;					// (4) ��ta� referenc�
	long	Size;					// (4) velikost dat (bajt�)
	long	res;					// (4) ... rezerva (pro zarovn�n�)
	BYTE*	Data;					// (4) ukazatel na data

} MUSICDATA;

// velikost polo�ky dat hudby (bez dat)
#define SIZEOFMUSICDATA	(3*sizeof(long) + sizeof(BYTE*))

extern MUSICDATA EmptyMusicData;		// data pr�zdn� hudby
extern	const CMusic	EmptyMusic;		// pr�zdn� hudba

/////////////////////////////////////////////////////////////////////////////
// statick� funkce

// vytvo�en� dat hudby (p�i chyb� pam�ti vrac� NULL)
MUSICDATA* _fastcall NewMusicData(int size);

// zru�en� dat hudby (odd�leno kv�li lep�� optimalizaci)
void _fastcall DelMusicData(MUSICDATA* data);

/////////////////////////////////////////////////////////////////////////////
// hudba

class CMusic
{

// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn�
	MUSICDATA*		pData;					// ukazatel na z�hlav� hudby

// p�ipojen� dat hudby
	inline void attach(MUSICDATA* data)
	{
		ASSERT(data != NULL);

		pData = data;
		LongIncrement(&(data->Refer));
	}

// odpojen� (a zru�en�) dat hudby
	inline void detach(MUSICDATA* data)
	{
		ASSERT(data != NULL);
		ASSERT(data->Refer > 0);

		if (LongDecrement(&(data->Refer)))
		{
			DelMusicData(data);
		}
	}


// ---------------------------- ve�ejn� funkce ------------------------------

public:

// konstruktor a destruktor
	CMusic();					// pr�zdn� hudba o d�lce 0
	CMusic(const CMusic& src);
	CMusic(MUSICDATA* data);
	~CMusic();

// statick� konstruktor a destruktor
	void Init();				// pr�zdn� hudba o d�lce 0
	void _fastcall Init(const CMusic& src);
	void _fastcall Init(MUSICDATA* data);
	bool _fastcall Init(int size);		// p�i chyb� pam�ti vrac� FALSE, hudba nen� vytvo�ena
	void Term();

// poskytnut� ukazatele na data
	inline MUSICDATA* Data() const { return pData; };
	inline BYTE* DataData() const { return pData->Data; };

// poskytnut� velikosti dat hudby
	inline int Size() const { return pData->Size; };

// vypr�zdn�n� (uvoln�n� dat) - p�ipoj� se standardn� pr�zdn� hudba
	void Empty();

// kopie do vlastn�ho bufferu p�ed modifikac�
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool CopyWrite();

// vytvo�en� nov� hudby (p�ipraveno k z�pisu, data jsou n�hodn�)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool New(int size);

// na�ten� hudby ze souboru (TRUE=operace OK)
	bool LoadFile(CText jmeno);

// ulo�en� do souboru form�tu MID (FALSE=chyba)
	bool SaveFile(CText jmeno) const;

// oper�tor p�i�azen�
	const CMusic& operator= (const CMusic& src);
	const CMusic& operator= (MUSICDATA* src);
};

/***************************************************************************\
*																			*
*								Buffer hudby								*
*																			*
\***************************************************************************/

class CBufMusic : public CBuffer<CMusic>
{

// ---------------------------- ve�ejn� funkce ------------------------------

public:

// vytvo�en� nov� polo�ky (pr�zdn�/obsah n�hodn�) (vrac� index polo�ky, <0 = chyba)
// prov�d� z�znam do UNDO bufferu
	inline int New() { return CBuffer<CMusic>::New(); }
	int New(int size);

// p�id�n� polo�ky (vrac� index polo�ky, <0 = chyba)
// prov�d� z�znam do UNDO bufferu
	inline int _fastcall Add(const CMusic& data) { return CBuffer<CMusic>::Add(data); }
	int _fastcall Add(MUSICDATA* data);
};

