
/***************************************************************************\
*																			*
*									Zvuk									*
*																			*
\***************************************************************************/

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
	long	Channels;				// (4) poèet kanálù (pro PCM jsou jen hodnoty 1 = mono, 2 = stereo)
	long	Bits;					// (4) poèet bitù na vzorek kanálu (pro PCM jsou jen hodnoty 8 nebo 16)
	long	SizeHead;				// (4) velikost záhlaví WAVEFORMATEX na zaèátku dat (pro PCM je = 0)
	BYTE*	Data;					// (4) ukazatel na data (s pøípadným popisovaèem WAVEFORMATEX na zaèátku)

} SOUNDDATA;

// velikost položky dat zvuku (bez dat)
#define SIZEOFSOUNDDATA	(7*sizeof(long) + sizeof(BYTE*))

extern SOUNDDATA EmptySoundData;			// data prázdného zvuku
extern	const CSound		EmptySound;		// prázdný zvuk


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
// statické funkce

// vytvoøení dat zvuku (pøi chybì pamìti vrací NULL)
SOUNDDATA* _fastcall NewSoundData(int size);

// zrušení dat zvuku (oddìleno kvùli lepší optimalizaci)
void _fastcall DelSoundData(SOUNDDATA* data);


/////////////////////////////////////////////////////////////////////////////
// zvuk

class CSound
{
// ------------------------- interní promìnné a funkce ----------------------

private:

// promìnné - pouze ukazatel na data
	SOUNDDATA*		pData;					// ukazatel na záhlaví zvuku

// pøipojení dat zvuku
	inline void attach(SOUNDDATA* data)
	{
		ASSERT(data != NULL);

		pData = data;
		LongIncrement(&(data->Refer));
	}

// odpojení (a zrušení) dat zvuku
	inline void detach(SOUNDDATA* data)
	{
		ASSERT(data);
		ASSERT(data->Refer > 0);

		if (LongDecrement(&(data->Refer)))
		{
			DelSoundData(data);
		}
	}


// ---------------------------- veøejné funkce ------------------------------

public:

// konstruktor a destruktor
	CSound();					// prázdný zvuk o délce 0
	CSound(const CSound& src);
	CSound(SOUNDDATA* data);
	~CSound();

// statický konstruktor a destruktor
	void Init();				// prázdný zvuk o délce 0
	void _fastcall Init(const CSound& src);
	void _fastcall Init(SOUNDDATA* data);
	bool _fastcall Init(int size);	// pøi chybì pamìti vrací FALSE, zvuk není vytvoøen

	void Term();

// poskytnutí ukazatele na data
	inline SOUNDDATA* Data() const { return pData; };
	inline BYTE* DataData() const { return pData->Data; };

// poskytnutí velikosti dat zvuku (bez záhlaví)
	inline int Size() const { return pData->Size; };

// poskytnutí/nastavení vzorkovacího kmitoètu
	inline int Samples() const { return pData->Samples; };
	inline void Samples(int samples) { pData->Samples = samples; };

// poskytnutí/nastavení formátu dat
	inline int Format() const { return pData->Format; };
	inline void Format(int format) { pData->Format = format; };

// poskytnutí/nastavení poètu kanálù
	inline int Channels() const { return pData->Channels; };
	inline void Channels(int channels) { pData->Channels = channels; };

// poskytnutí/nastavení poètu bitù na vzorek
	inline int Bits() const { return pData->Bits; };
	inline void Bits(int bits) { pData->Bits = bits; };

// vyprázdnìní (uvolnìní dat) - pøipojí se standardní prázdný zvuk
	void Empty();

// kopie do vlastního bufferu pøed modifikací
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool CopyWrite();

// vytvoøení nového zvuku (pøipraveno k zápisu, data i parametry jsou náhodné)
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool _fastcall New(int size);

// nastavení velikosti zvuku
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
	bool ReSize(int size);

// naètení zvuku ze souboru (TRUE=operace OK)
	bool LoadFile(CText jmeno);

// uložení do souboru formátu WAV (FALSE=chyba)
	bool SaveFile(CText jmeno) const;

// operátor pøiøazení
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

// ---------------------------- veøejné funkce ------------------------------

public:

// vytvoøení nové položky - obsah náhodný (vrací index položky, <0 = chyba)
// provádí záznam do UNDO bufferu
	inline int New() { return CBuffer<CSound>::New(); }
	int New(int size);

// pøidání položky (vrací index položky, <0 = chyba)
// provádí záznam do UNDO bufferu
	inline int _fastcall Add(const CSound& data) { return CBuffer<CSound>::Add(data); }
	int _fastcall Add(SOUNDDATA* data);
};

