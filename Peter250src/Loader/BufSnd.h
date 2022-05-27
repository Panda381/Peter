
/***************************************************************************\
*																			*
*									Zvuk									*
*																			*
\***************************************************************************/

#define	SOUNDDEFSAMPLES		22050				// implicitn� p�enosov� rychlost
#define	SOUNDDEFFORMAT		WAVE_FORMAT_PCM		// implicitn� form�t dat
#define	SOUNDDEFCHANNELS	1					// implicitn� po�et kan�l�
#define	SOUNDDEFBITS		16					// implicitn� po�et bit�
#define	SOUNDDEFALIGN		(SOUNDDEFCHANNELS * (SOUNDDEFBITS/8))	
												// implicitn� zarov�v�n�
#define	SOUNDDEFRATE		(SOUNDDEFSAMPLES * SOUNDDEFCHANNELS * (SOUNDDEFBITS/8))
												 // implicitn� rychlost p�enosu

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
	short	Channels;				// (2) po�et kan�l� (pro PCM jsou jen hodnoty 1 = mono, 2 = stereo)
	short	Bits;					// (2) po�et bit� na vzorek kan�lu (pro PCM jsou jen hodnoty 8 nebo 16)
	IDirectSoundBuffer* DSBuffer;	// (4) DirectSound buffer (NULL=nen�)
	long	SizeHead;				// (4) velikost z�hlav� WAVEFORMATEX na za��tku dat (pro PCM je = 0)
	BYTE*	Data;					// (4) ukazatel na data (s p��padn�m popisova�em WAVEFORMATEX na za��tku)

} SOUNDDATA;

// velikost polo�ky dat zvuku (bez dat)
#define SIZEOFSOUNDDATA	(5*sizeof(long) + 2*sizeof(short) + sizeof(IDirectSoundBuffer*) + sizeof(BYTE*))

extern SOUNDDATA EmptySoundData;			// data pr�zdn�ho zvuku


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
// zvuk

class CSound
{
// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn� - pouze ukazatel na data
	SOUNDDATA*		pData;					// ukazatel na z�hlav� zvuku

// p�ipojen� dat
	inline void Attach(SOUNDDATA* data)
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
			if (pData->DSBuffer != NULL)
			{
				pData->DSBuffer->Stop();
				pData->DSBuffer->Release();
			}

			ASSERT(pData->Data);
			MemFree(pData->Data);
			MemFree(pData);
#ifdef _DEBUG
			pData = NULL;
#endif
		}
	}

// vytvo�en� nov�ho bufferu - star� buffer mus� b�t odpojen!
	inline void NewBuffer(int size)
	{
		ASSERT(size >= 0);
		if (size < 0) size = 0;
		SOUNDDATA* data = (SOUNDDATA*)MemGet(SIZEOFSOUNDDATA);
		pData = data;						// adresa dat
		data->Refer = 1;					// po�et referenc�
		data->Size = size;					// velikost dat
		data->Samples = SOUNDDEFSAMPLES;	// vzorkovac� kmito�et
		data->Format = SOUNDDEFFORMAT;		// form�t dat
		data->Channels = SOUNDDEFCHANNELS;	// po�et kan�l�
		data->Bits = SOUNDDEFBITS;			// po�et bit� na vzorek
		data->DSBuffer = NULL;				// nen� DirectSound buffer
		data->SizeHead = 0;					// velikost z�hlav� WAVEFORMATEX
		data->Data = (BYTE*)MemGet(size);	// vytvo�en� bufferu pro data
	}


// ---------------------------- ve�ejn� funkce ------------------------------

public:

// konstruktor a destruktor
	CSound();								// standardn� konstruktor
	CSound(const CSound& src);				// kop�rovac� konstruktor
	CSound(int size);						// konstruktor s vytvo�en�m dat
	~CSound();								// standardn� destruktor

// statick� konstruktor a destruktor
	void Init();							// statick� konstruktor
	void Init(SOUNDDATA* data);				// statick� konstruktor se zad�n�m dat
	void Init(int size);					// statick� konstruktor s vytvo�en�m dat
	void Term();							// statick� destruktor

// p�ipojen�/odpojen� dat
	void NewBufferData(int size);			// vytvo�en� nov�ho bufferu
	void AttachData(SOUNDDATA* data);		// p�ipojen� dat
	void DetachData();						// odpojen� dat

// poskytnut� ukazatele na data
	inline SOUNDDATA* Data() const { return pData; };
	inline BYTE* DataData() const { return pData->Data; };

// poskytnut� velikosti dat zvuku (bez z�hlav�)
	inline int Size() const { return pData->Size; };

// poskytnut�/nastaven� vzorkovac�ho kmito�tu
	inline int Samples() const { return pData->Samples; };
	inline void Samples(int samples) { pData->Samples = samples; };
//	inline BOOL LowRate() const { return pData->Samples <= 22050*3/4; };
//	inline BOOL HighRate() const { return pData->Samples >= 22050*3/2; };

// poskytnut�/nastaven� form�tu dat
	inline int Format() const { return pData->Format; };
	inline void Format(int format) { pData->Format = format; };

// dekomprese zvuku na form�t PCM
	void _fastcall DeComp();

// poskytnut� velikosti z�hlav� WAVEFORMATEX
	inline int SizeHead() const { return pData->SizeHead; };

// poskytnut�/nastaven� po�tu kan�l� (obsluhov�no jen 1 nebo 2)
	inline int Channels() const { return pData->Channels; };
	inline void Channels(int channels) { pData->Channels = (short)channels; };
	inline BOOL Stereo() const { return (pData->Channels == 2); };
	inline BOOL Mono() const { return (pData->Channels == 1); };

// poskytnut�/nastaven� po�tu bit� na vzorek (obsluhov�no jen 8 nebo 16)
	inline int Bits() const { return pData->Bits; };
	inline void Bits(int bits) { pData->Bits = (short)bits; };
	inline int Bytes() const { return pData->Bits/8; };

// poskytnut� zarovn�v�n� dat
	inline int Align() const { return pData->Channels * (pData->Bits/8); };

// poskytnut� p�enosov� rychlosti dat (bajt� za sekundu)
	inline int DataRate() const { return pData->Samples * pData->Channels * (pData->Bits/8); };

// kopie do vlastn�ho bufferu p�ed modifikac�
	void CopyWrite();

// duplikace zvuku (p��p. p�ivlastn�n�)
//	void DupCopy();

// vypr�zdn�n� zvuku (uvoln�n� dat)
	void Empty();

// test, zda je zvuk pr�zdn�
	inline BOOL IsEmpty() { return ((DWORD)pData == (DWORD)&EmptySoundData); };
	inline BOOL IsNotEmpty() { return ((DWORD)pData != (DWORD)&EmptySoundData); };

// vytvo�en� nov�ho zvuku (p�ipraveno k z�pisu, data jsou n�hodn�)
	void New(int size);

// nastaven� velikosti zvuku
	void ReSize(int size);

// na�ten� zvuku ze souboru
	void LoadFile();

// ulo�en� do souboru
	void SaveFile() const;

// konverze form�tu na 8 bit�
	void _fastcall Conv8Bit();

// konverze form�tu na 16 bit�
	void _fastcall Conv16Bit();

// konverze form�tu na stereo
	void _fastcall ConvStereo();

// konverze form�tu na mono
	void _fastcall ConvMono();

// inverze zvuku
	void _fastcall ConvInv();

// konverze vzorkovac� frekvence
	void _fastcall ConvRate(int rate);

// konverze rychlosti zvuku (1=beze zm�ny)
	void _fastcall ConvSpeed(double koef);

// p�i�ten� zvuku
	void _fastcall Add(CSound src);

// vygenerov�n� t�nu
	void _fastcall TonGen(double freq, double len);

// oper�tor p�i�azen�
	const CSound& operator= (const CSound& src);

// oper�tory porovn�n�
	friend inline BOOL operator==(const CSound& s1, const CSound& s2)
		{ return (DWORD)s1.pData == (DWORD)s2.pData; };

	friend inline BOOL operator!=(const CSound& s1, const CSound& s2)
		{ return (DWORD)s1.pData != (DWORD)s2.pData; };
};

/***************************************************************************\
*																			*
*								Buffer zvuku								*
*																			*
\***************************************************************************/

class CBufSound
{
// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn�
	CSound*		m_Data;		// ukazatel na data
	int			m_Num;		// po�et platn�ch polo�ek v bufferu
	int			m_Max;		// velikost bufferu (polo�ek)

// vytvo�en� nov� polo�ky
	inline int NewItem()
	{
		int i = m_Num;
		if (i >= m_Max)
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
	CBufSound();
	~CBufSound();

// statick� konstruktor a destruktor
	void Init();			// statick� konstruktor
	void Term();			// statick� destruktor

// zru�en� v�ech polo�ek v bufferu (ukl�d�n� za�ne op�t po �ad� od za��tku)
	void DelAll();

// poskytnut� bufferu zvuk�
	inline CSound* Data() const { return m_Data; };

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
	inline CSound& operator[] (const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const CSound& operator[] (const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline CSound& At(const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const CSound& At(const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

// poskytnut� polo�ky (s kontrolou platnosti indexu)
	const CSound& _fastcall Get(const int index) const;

// nastaven� polo�ky (s kontrolou platnosti indexu)
	void _fastcall Set(const int index, const CSound& data);

// vypr�zdn�n� polo�ky (bez jej�ho zru�en� - jen pro uvoln�n� dat)
	void _fastcall Empty(const int index);

// zru�en� polo�ek z konce bufferu
	void _fastcall Del(int num);

// vytvo�en� pr�zdn� polo�ky (vrac� index polo�ky)
	int New();
	int New(int size);

// p�id�n� polo�ky (vrac� index polo�ky)
	int _fastcall Add(const CSound& data);
	int _fastcall Add(SOUNDDATA* data);

// duplikace polo�ky (s kontrolou platnosti indexu, vrac� index prvn� polo�ky)
	int _fastcall Dup(const int index);
	int _fastcall Dup(const int index, int num);

// oper�tor p�i�azen�
	const CBufSound& operator= (const CBufSound& src);
};

