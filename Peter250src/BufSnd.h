
/***************************************************************************\
*																			*
*									Zvuk									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// struktura polo�ky dat zvuku (32 bajt�)
// pro jin� form�t ne� PCM je na za��tku dat popisova� form�tu WAVEFORMATEX s udanou
// velikost� dopl�uj�c�ch dat "cbSize", za popisova�em bezprost�edn� n�sleduj� data

typedef struct SOUNDDATA_
{
	long	Refer;					// (4) ��ta� referenc�
	long	Size;					// (4) velikost dat (bajt�)	- v�etn� p��padn�ho popisova�e WAVEFORMATEX
	long	Samples;				// (4) vzorkovac� kmito�et - vzork� za sekundu
									//		(pro PCM jsou jen hodnoty 11025/22050/44100)
	long	Format;					// (4) form�t dat (1 = PCM, pro jin� form�t je na za��tku dat z�hlav� WAVEFORMATEX)
	long	Channels;				// (4) po�et kan�l� (pro PCM jsou jen hodnoty 1 = mono, 2 = stereo)
	long	Bits;					// (4) po�et bit� na vzorek kan�lu (pro PCM jsou jen hodnoty 8 nebo 16)
	long	SizeHead;				// (4) velikost z�hlav� WAVEFORMATEX na za��tku dat (pro PCM je = 0)
	BYTE*	Data;					// (4) ukazatel na data (s p��padn�m popisova�em WAVEFORMATEX na za��tku)

} SOUNDDATA;

// velikost polo�ky dat zvuku (bez dat)
#define SIZEOFSOUNDDATA	(7*sizeof(long) + sizeof(BYTE*))

extern SOUNDDATA EmptySoundData;			// data pr�zdn�ho zvuku
extern	const CSound		EmptySound;		// pr�zdn� zvuk


/////////////////////////////////////////////////////////////////////////////
// standardn� popisova� pro export/import

#pragma pack( push )
#pragma pack( 2 )

// popisova� dat souboru WAV (8 B)
typedef struct WAVDATA_ {
	char			tDataIdent[4];		// (4) identifikace dat "data"
	DWORD			nDataSize;			// (4) velikost n�sleduj�c�ch dat zvuku
} WAVDATA;

// popisova� form�tu souboru WAV (28 B)
typedef struct WAVFORMAT_ {
	char			tFormatIdent[8];	// (8) identifikace z�hlav� form�tu (= "WAVEfmt ")
	DWORD			nFormatSize;		// (4) velikost n�sleduj�c�ch dat form�tu

	WORD			wFormatTag;			// (2) form�t dat (1 = PCM)
	WORD			nChannels;			// (2) po�et kan�l� (1 = mono, 2 = stereo)
	DWORD			nSamplesPerSec;		// (4) vzorkovac� kmito�et (vzork� za sekundu)
	DWORD			nAvgBytesPerSec;	// (4) p�enosov� rychlost (bajt� za sekundu)
	WORD			nBlockAlign;		// (2) zarovn�v�n� dat (bity*kan�ly/8)

	WORD			wBitsPerSample;		// (2) po�et bit� na jeden vzorek
} WAVFORMAT;

// struktura souboru WAV (44 B)
typedef struct WAVHEAD_ {
	char		tWavIdent[4];		// (4) identifikace souboru (= "RIFF")
	DWORD		nFileSize;			// (4) velikost n�sleduj�c�ch dat souboru

	WAVFORMAT	WavFormat;			// blok popisova�e form�tu
	WAVDATA		WavData;			// blok dat zvuku
} WAVHEAD;

#pragma pack( pop )

/////////////////////////////////////////////////////////////////////////////
// statick� funkce

// vytvo�en� dat zvuku (p�i chyb� pam�ti vrac� NULL)
SOUNDDATA* _fastcall NewSoundData(int size);

// zru�en� dat zvuku (odd�leno kv�li lep�� optimalizaci)
void _fastcall DelSoundData(SOUNDDATA* data);


/////////////////////////////////////////////////////////////////////////////
// zvuk

class CSound
{
// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn� - pouze ukazatel na data
	SOUNDDATA*		pData;					// ukazatel na z�hlav� zvuku

// p�ipojen� dat zvuku
	inline void attach(SOUNDDATA* data)
	{
		ASSERT(data != NULL);

		pData = data;
		LongIncrement(&(data->Refer));
	}

// odpojen� (a zru�en�) dat zvuku
	inline void detach(SOUNDDATA* data)
	{
		ASSERT(data);
		ASSERT(data->Refer > 0);

		if (LongDecrement(&(data->Refer)))
		{
			DelSoundData(data);
		}
	}


// ---------------------------- ve�ejn� funkce ------------------------------

public:

// konstruktor a destruktor
	CSound();					// pr�zdn� zvuk o d�lce 0
	CSound(const CSound& src);
	CSound(SOUNDDATA* data);
	~CSound();

// statick� konstruktor a destruktor
	void Init();				// pr�zdn� zvuk o d�lce 0
	void _fastcall Init(const CSound& src);
	void _fastcall Init(SOUNDDATA* data);
	bool _fastcall Init(int size);	// p�i chyb� pam�ti vrac� FALSE, zvuk nen� vytvo�en

	void Term();

// poskytnut� ukazatele na data
	inline SOUNDDATA* Data() const { return pData; };
	inline BYTE* DataData() const { return pData->Data; };

// poskytnut� velikosti dat zvuku (bez z�hlav�)
	inline int Size() const { return pData->Size; };

// poskytnut�/nastaven� vzorkovac�ho kmito�tu
	inline int Samples() const { return pData->Samples; };
	inline void Samples(int samples) { pData->Samples = samples; };

// poskytnut�/nastaven� form�tu dat
	inline int Format() const { return pData->Format; };
	inline void Format(int format) { pData->Format = format; };

// poskytnut�/nastaven� po�tu kan�l�
	inline int Channels() const { return pData->Channels; };
	inline void Channels(int channels) { pData->Channels = channels; };

// poskytnut�/nastaven� po�tu bit� na vzorek
	inline int Bits() const { return pData->Bits; };
	inline void Bits(int bits) { pData->Bits = bits; };

// vypr�zdn�n� (uvoln�n� dat) - p�ipoj� se standardn� pr�zdn� zvuk
	void Empty();

// kopie do vlastn�ho bufferu p�ed modifikac�
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool CopyWrite();

// vytvo�en� nov�ho zvuku (p�ipraveno k z�pisu, data i parametry jsou n�hodn�)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool _fastcall New(int size);

// nastaven� velikosti zvuku
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool ReSize(int size);

// na�ten� zvuku ze souboru (TRUE=operace OK)
	bool LoadFile(CText jmeno);

// ulo�en� do souboru form�tu WAV (FALSE=chyba)
	bool SaveFile(CText jmeno) const;

// oper�tor p�i�azen�
	const CSound& operator= (const CSound& src);
	const CSound& operator= (SOUNDDATA* src);
};

/***************************************************************************\
*																			*
*								Buffer zvuku								*
*																			*
\***************************************************************************/

class CBufSound : public CBuffer<CSound>
{

// ---------------------------- ve�ejn� funkce ------------------------------

public:

// vytvo�en� nov� polo�ky - obsah n�hodn� (vrac� index polo�ky, <0 = chyba)
// prov�d� z�znam do UNDO bufferu
	inline int New() { return CBuffer<CSound>::New(); }
	int New(int size);

// p�id�n� polo�ky (vrac� index polo�ky, <0 = chyba)
// prov�d� z�znam do UNDO bufferu
	inline int _fastcall Add(const CSound& data) { return CBuffer<CSound>::Add(data); }
	int _fastcall Add(SOUNDDATA* data);
};

