
/***************************************************************************\
*																			*
*									Zvuk									*
*																			*
\***************************************************************************/

#define	SOUNDDEFSAMPLES		22050				// implicitní pøenosová rychlost
#define	SOUNDDEFFORMAT		WAVE_FORMAT_PCM		// implicitní formát dat
#define	SOUNDDEFCHANNELS	1					// implicitní poèet kanálù
#define	SOUNDDEFBITS		16					// implicitní poèet bitù
#define	SOUNDDEFALIGN		(SOUNDDEFCHANNELS * (SOUNDDEFBITS/8))	
												// implicitní zarovávání
#define	SOUNDDEFRATE		(SOUNDDEFSAMPLES * SOUNDDEFCHANNELS * (SOUNDDEFBITS/8))
												 // implicitní rychlost pøenosu

/////////////////////////////////////////////////////////////////////////////
// struktura položky dat zvuku (32 bajtù)
// pro jiný formát než PCM je na zaèátku dat popisovaè formátu WAVEFORMATEX s udanou
// velikostí doplòujících dat "cbSize", za popisovaèem bezprostøednì následují data

typedef struct SOUNDDATA_
{
	long	Refer;					// (4) èítaè referencí
	long	Size;					// (4) velikost dat (bajtù)	- vèetnì pøípadného popisovaèe WAVEFORMATEX
	long	Samples;				// (4) vzorkovací kmitoèet - vzorkù za sekundu
									//		(pro PCM jsou jen hodnoty 11025/22050/44100)
	long	Format;					// (4) formát dat (1 = PCM, pro jiný formát je na zaèátku dat záhlaví WAVEFORMATEX)
	short	Channels;				// (2) poèet kanálù (pro PCM jsou jen hodnoty 1 = mono, 2 = stereo)
	short	Bits;					// (2) poèet bitù na vzorek kanálu (pro PCM jsou jen hodnoty 8 nebo 16)
	IDirectSoundBuffer* DSBuffer;	// (4) DirectSound buffer (NULL=není)
	long	SizeHead;				// (4) velikost záhlaví WAVEFORMATEX na zaèátku dat (pro PCM je = 0)
	BYTE*	Data;					// (4) ukazatel na data (s pøípadným popisovaèem WAVEFORMATEX na zaèátku)

} SOUNDDATA;

// velikost položky dat zvuku (bez dat)
#define SIZEOFSOUNDDATA	(5*sizeof(long) + 2*sizeof(short) + sizeof(IDirectSoundBuffer*) + sizeof(BYTE*))

extern SOUNDDATA EmptySoundData;			// data prázdného zvuku


/////////////////////////////////////////////////////////////////////////////
// standardní popisovaè pro export/import

#pragma pack( push )
#pragma pack( 2 )

// popisovaè dat souboru WAV (8 B)
typedef struct WAVDATA_ {
	char			tDataIdent[4];		// (4) identifikace dat "data"
	DWORD			nDataSize;			// (4) velikost následujících dat zvuku
} WAVDATA;

// popisovaè formátu souboru WAV (28 B)
typedef struct WAVFORMAT_ {
	char			tFormatIdent[8];	// (8) identifikace záhlaví formátu (= "WAVEfmt ")
	DWORD			nFormatSize;		// (4) velikost následujících dat formátu

	WORD			wFormatTag;			// (2) formát dat (1 = PCM)
	WORD			nChannels;			// (2) poèet kanálù (1 = mono, 2 = stereo)
	DWORD			nSamplesPerSec;		// (4) vzorkovací kmitoèet (vzorkù za sekundu)
	DWORD			nAvgBytesPerSec;	// (4) pøenosová rychlost (bajtù za sekundu)
	WORD			nBlockAlign;		// (2) zarovnávání dat (bity*kanály/8)

	WORD			wBitsPerSample;		// (2) poèet bitù na jeden vzorek
} WAVFORMAT;

// struktura souboru WAV (44 B)
typedef struct WAVHEAD_ {
	char		tWavIdent[4];		// (4) identifikace souboru (= "RIFF")
	DWORD		nFileSize;			// (4) velikost následujících dat souboru

	WAVFORMAT	WavFormat;			// blok popisovaèe formátu
	WAVDATA		WavData;			// blok dat zvuku
} WAVHEAD;

#pragma pack( pop )


/////////////////////////////////////////////////////////////////////////////
// zvuk

class CSound
{
// ------------------------- interní promìnné a funkce ----------------------

private:

// promìnné - pouze ukazatel na data
	SOUNDDATA*		pData;					// ukazatel na záhlaví zvuku

// pøipojení dat
	inline void Attach(SOUNDDATA* data)
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

// vytvoøení nového bufferu - starý buffer musí být odpojen!
	inline void NewBuffer(int size)
	{
		ASSERT(size >= 0);
		if (size < 0) size = 0;
		SOUNDDATA* data = (SOUNDDATA*)MemGet(SIZEOFSOUNDDATA);
		pData = data;						// adresa dat
		data->Refer = 1;					// poèet referencí
		data->Size = size;					// velikost dat
		data->Samples = SOUNDDEFSAMPLES;	// vzorkovací kmitoèet
		data->Format = SOUNDDEFFORMAT;		// formát dat
		data->Channels = SOUNDDEFCHANNELS;	// poèet kanálù
		data->Bits = SOUNDDEFBITS;			// poèet bitù na vzorek
		data->DSBuffer = NULL;				// není DirectSound buffer
		data->SizeHead = 0;					// velikost záhlaví WAVEFORMATEX
		data->Data = (BYTE*)MemGet(size);	// vytvoøení bufferu pro data
	}


// ---------------------------- veøejné funkce ------------------------------

public:

// konstruktor a destruktor
	CSound();								// standardní konstruktor
	CSound(const CSound& src);				// kopírovací konstruktor
	CSound(int size);						// konstruktor s vytvoøením dat
	~CSound();								// standardní destruktor

// statický konstruktor a destruktor
	void Init();							// statický konstruktor
	void Init(SOUNDDATA* data);				// statický konstruktor se zadáním dat
	void Init(int size);					// statický konstruktor s vytvoøením dat
	void Term();							// statický destruktor

// pøipojení/odpojení dat
	void NewBufferData(int size);			// vytvoøení nového bufferu
	void AttachData(SOUNDDATA* data);		// pøipojení dat
	void DetachData();						// odpojení dat

// poskytnutí ukazatele na data
	inline SOUNDDATA* Data() const { return pData; };
	inline BYTE* DataData() const { return pData->Data; };

// poskytnutí velikosti dat zvuku (bez záhlaví)
	inline int Size() const { return pData->Size; };

// poskytnutí/nastavení vzorkovacího kmitoètu
	inline int Samples() const { return pData->Samples; };
	inline void Samples(int samples) { pData->Samples = samples; };
//	inline BOOL LowRate() const { return pData->Samples <= 22050*3/4; };
//	inline BOOL HighRate() const { return pData->Samples >= 22050*3/2; };

// poskytnutí/nastavení formátu dat
	inline int Format() const { return pData->Format; };
	inline void Format(int format) { pData->Format = format; };

// dekomprese zvuku na formát PCM
	void _fastcall DeComp();

// poskytnutí velikosti záhlaví WAVEFORMATEX
	inline int SizeHead() const { return pData->SizeHead; };

// poskytnutí/nastavení poètu kanálù (obsluhováno jen 1 nebo 2)
	inline int Channels() const { return pData->Channels; };
	inline void Channels(int channels) { pData->Channels = (short)channels; };
	inline BOOL Stereo() const { return (pData->Channels == 2); };
	inline BOOL Mono() const { return (pData->Channels == 1); };

// poskytnutí/nastavení poètu bitù na vzorek (obsluhováno jen 8 nebo 16)
	inline int Bits() const { return pData->Bits; };
	inline void Bits(int bits) { pData->Bits = (short)bits; };
	inline int Bytes() const { return pData->Bits/8; };

// poskytnutí zarovnávání dat
	inline int Align() const { return pData->Channels * (pData->Bits/8); };

// poskytnutí pøenosové rychlosti dat (bajtù za sekundu)
	inline int DataRate() const { return pData->Samples * pData->Channels * (pData->Bits/8); };

// kopie do vlastního bufferu pøed modifikací
	void CopyWrite();

// duplikace zvuku (pøíp. pøivlastnìní)
//	void DupCopy();

// vyprázdnìní zvuku (uvolnìní dat)
	void Empty();

// test, zda je zvuk prázdný
	inline BOOL IsEmpty() { return ((DWORD)pData == (DWORD)&EmptySoundData); };
	inline BOOL IsNotEmpty() { return ((DWORD)pData != (DWORD)&EmptySoundData); };

// vytvoøení nového zvuku (pøipraveno k zápisu, data jsou náhodná)
	void New(int size);

// nastavení velikosti zvuku
	void ReSize(int size);

// naètení zvuku ze souboru
	void LoadFile();

// uložení do souboru
	void SaveFile() const;

// konverze formátu na 8 bitù
	void _fastcall Conv8Bit();

// konverze formátu na 16 bitù
	void _fastcall Conv16Bit();

// konverze formátu na stereo
	void _fastcall ConvStereo();

// konverze formátu na mono
	void _fastcall ConvMono();

// inverze zvuku
	void _fastcall ConvInv();

// konverze vzorkovací frekvence
	void _fastcall ConvRate(int rate);

// konverze rychlosti zvuku (1=beze zmìny)
	void _fastcall ConvSpeed(double koef);

// pøiètení zvuku
	void _fastcall Add(CSound src);

// vygenerování tónu
	void _fastcall TonGen(double freq, double len);

// operátor pøiøazení
	const CSound& operator= (const CSound& src);

// operátory porovnání
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
// ------------------------- interní promìnné a funkce ----------------------

private:

// promìnné
	CSound*		m_Data;		// ukazatel na data
	int			m_Num;		// poèet platných položek v bufferu
	int			m_Max;		// velikost bufferu (položek)

// vytvoøení nové položky
	inline int NewItem()
	{
		int i = m_Num;
		if (i >= m_Max)
		{
			NewData();				// vytvoøení nových dat
		}

		m_Num = i + 1;
		return i;
	};

// vytvoøení nových dat (oddìleno kvùli lepší optimalizaci)
	void NewData();


// ---------------------------- veøejné funkce ------------------------------

public:

// konstruktor a destruktor
	CBufSound();
	~CBufSound();

// statický konstruktor a destruktor
	void Init();			// statický konstruktor
	void Term();			// statický destruktor

// zrušení všech položek v bufferu (ukládání zaène opìt po øadì od zaèátku)
	void DelAll();

// poskytnutí bufferu zvukù
	inline CSound* Data() const { return m_Data; };

// poskytnutí poètu platných položek v bufferu
	inline int Num() const { return m_Num; };

// poskytnutí velikosti bufferu
	inline int Max() const { return m_Max; };

// kontrola platnosti položky
	inline BOOL IsValid(const int index) const
		{ return ((DWORD)index < (DWORD)m_Num); };

	inline BOOL IsNotValid(const int index) const
		{ return ((DWORD)index >= (DWORD)m_Num); };

// poskytnutí pøístupu k položce (bez kontroly indexu)
	inline CSound& operator[] (const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const CSound& operator[] (const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline CSound& At(const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const CSound& At(const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

// poskytnutí položky (s kontrolou platnosti indexu)
	const CSound& _fastcall Get(const int index) const;

// nastavení položky (s kontrolou platnosti indexu)
	void _fastcall Set(const int index, const CSound& data);

// vyprázdnìní položky (bez jejího zrušení - jen pro uvolnìní dat)
	void _fastcall Empty(const int index);

// zrušení položek z konce bufferu
	void _fastcall Del(int num);

// vytvoøení prázdné položky (vrací index položky)
	int New();
	int New(int size);

// pøidání položky (vrací index položky)
	int _fastcall Add(const CSound& data);
	int _fastcall Add(SOUNDDATA* data);

// duplikace položky (s kontrolou platnosti indexu, vrací index první položky)
	int _fastcall Dup(const int index);
	int _fastcall Dup(const int index, int num);

// operátor pøiøazení
	const CBufSound& operator= (const CBufSound& src);
};

