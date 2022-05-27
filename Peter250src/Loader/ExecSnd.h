
/***************************************************************************\
*																			*
*							Provádìní programu - zvuk						*
*																			*
\***************************************************************************/

#define MAXSOUNDKANALU	1024		// maximální poèet zvukových kanálù
#define SOUNDBUFFERU	3			// poèet zvukových bufferù
#define SOUNDOUTRATE	22050		// výstupní vzorkovací kmitoèet
#define SOUNDBUFSIZE	(0x1000*4)	// velikost bufferu (dìlitelný 4 !!!)
#define SOUNDERRORWAIT	(2*18)		// èekání na nový pokus o otevøení zvuku

// obsluha pøehrávání zvuku
typedef struct SOUNDCHAN_ {
	int		Num;					// (4) poèet bajtù v bufferu (0=volný)
	BYTE*	Data;					// (4) adresa bufferu dat
	double	Left;					// (8) hlasitost levého kanálu
	double	Right;					// (8) hlasitost pravého kanálu
	double	Offset;					// (8) ukazatel dat (v bajtech)
	double	SpeedB;					// (8) pøírustek ukazatele dat (v bajtech)
	int		Loop;					// (4) èítaè opakování zvuku (BigInt = nekoneèný)
	CSound	Sound;					// (4) promìnná se zvukem
	IDirectSoundBuffer* DSBuffer;	// (4) DirectSound buffer
	int		DSFreq;					// (4) frekvence pro DirectSound
	short	DSVolume;				// (2) hlasitost pro DirectSound
	short	DSBalance;				// (2) vyvážení pro DirectSound
	WORD	SoundID;				// (2) rozlišovací kód zvuku
	short	res;					// (2) .. pro zarovnání
} SOUNDCHAN;

extern	HWAVEOUT	WaveOut;						// handle výstupního zaøízení (NULL=není otevøeno)
extern	WAVEHDR		WaveHeader[SOUNDBUFFERU];		// popisovaèe dat
extern	BYTE*		SoundBuf[SOUNDBUFFERU];			// zvukové buffery
extern	int			SoundKanalu;					// poèet aktivních zvukových kanálù
extern	SOUNDCHAN*	SoundChan;						// buffer zvukových kanálù
extern	int			SoundError;						// povolení opakovaného otevøení zvuku (pøi < 0)
extern	bool		WaveFormatPCM;					// je pøehráván formát PCM (vlastní obsluha)
extern	char*		WaveData2;						// ukazatel dat pro nestandardní formát
extern	int			WaveSize2;						// èítaè dat pro nestandardní formát
extern	int			WaveLoop;						// èítaè opakování pro nestandardní formát
extern	CSound		Sound2;							// pøehrávaný nestandardní zvuk, naposledy zadaný zvuk
extern	bool		SoundPausing;					// pøíznak pauzy zvuku
extern	WORD		SoundID;						// rozlišovací kód zvuku

/////////////////////////////////////////////////////////////////////////////
// funkce pro inicializaci DirectSound

typedef	HRESULT (WINAPI *DIRECTSOUNDCREATE) (LPGUID, LPDIRECTSOUND*, IUnknown FAR *);

/////////////////////////////////////////////////////////////////////////////
// deinicializace DirectSound

void TermDirectSound();

/////////////////////////////////////////////////////////////////////////////
// obsluha zvuku

void PlaySoundOpen(CSound sound, int loop,	// otevøení zvukového kanálu
		bool stop, double volume, double balance, double speed);
void PlaySoundClose(CSound sound);	// uzavøení zvukového kanálu
void PlaySoundStop();				// zastavení pøehrávání zvukù
void PlaySoundBack();				// obsluha pøehrávání zvuku na pozadí
void SetSoundPos(double pos);		// nastavení pozice pøehrávání zvuku (v sekundách)
double GetSoundPos();				// zjištìní pozice pøehrávání zvuku (v sekundách)
void SoundPause(bool pause);		// pauza pøehrávání zvuku


/////////////////////////////////////////////////////////////////////////////
// inline volání pøíkazu - zvuk

inline void FSound(CSound& sound)
{
	ExecItem++;
	ExecItem[-1].ExecSnd(sound);
}


// funkce
void _fastcall FSoundFunc(CSound& sound);		// funkce s návratem zvuku

// promìnné
void _fastcall FSoundEmpty(CSound& sound);		// prázdný zvuk
void _fastcall FSoundObj(CSound& sound);		// globální zvuk (Data = index)
void _fastcall FSoundObjList(CSound& sound);	// globalní zvuk v seznamu (Data = index, List = seznam)
void _fastcall FSoundLoc(CSound& sound);		// lokální zvuk (Data = index)
void _fastcall FSoundLocList(CSound& sound);	// lokální zvuk v seznamu (Data = index, List = seznam)

// konverze
void _fastcall FSoundConv8Bit(CSound& sound);	// konverze na 8 bitù
void _fastcall FSoundConv16Bit(CSound& sound);	// konverze na 16 bitù
void _fastcall FSoundConvStereo(CSound& sound);	// konverze na stereo
void _fastcall FSoundConvMono(CSound& sound);	// konverze na mono
void _fastcall FSoundConv11025(CSound& sound);	// konverze na frekvenci 11025
void _fastcall FSoundConv22050(CSound& sound);	// konverze na frekvenci 22050
void _fastcall FSoundConv44100(CSound& sound);	// konverze na frekvenci 44100
void _fastcall FSoundConvCopy(CSound& sound);	// duplikace zvuku
void _fastcall FSoundAdd1(CSound& sound);		// souèet jednoho zvuku - ponechá se beze zmìny
void _fastcall FSoundAdd(CSound& sound);		// souèet zvukù (Data = poèet zvukù - 1)
void _fastcall FSoundSpeed(CSound& sound);		// konverze rychlosti zvuku
void _fastcall FTonGen(CSound& sound);			// tónový generátor

void _fastcall FGetFileSound(CSound& sound);	// naètení zvuku ze souboru

void _fastcall FSoundConvPCM(CSound& sound);	// dekomprimace zvuku
