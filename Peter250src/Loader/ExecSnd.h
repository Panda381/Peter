
/***************************************************************************\
*																			*
*							Prov�d�n� programu - zvuk						*
*																			*
\***************************************************************************/

#define MAXSOUNDKANALU	1024		// maxim�ln� po�et zvukov�ch kan�l�
#define SOUNDBUFFERU	3			// po�et zvukov�ch buffer�
#define SOUNDOUTRATE	22050		// v�stupn� vzorkovac� kmito�et
#define SOUNDBUFSIZE	(0x1000*4)	// velikost bufferu (d�liteln� 4 !!!)
#define SOUNDERRORWAIT	(2*18)		// �ek�n� na nov� pokus o otev�en� zvuku

// obsluha p�ehr�v�n� zvuku
typedef struct SOUNDCHAN_ {
	int		Num;					// (4) po�et bajt� v bufferu (0=voln�)
	BYTE*	Data;					// (4) adresa bufferu dat
	double	Left;					// (8) hlasitost lev�ho kan�lu
	double	Right;					// (8) hlasitost prav�ho kan�lu
	double	Offset;					// (8) ukazatel dat (v bajtech)
	double	SpeedB;					// (8) p��rustek ukazatele dat (v bajtech)
	int		Loop;					// (4) ��ta� opakov�n� zvuku (BigInt = nekone�n�)
	CSound	Sound;					// (4) prom�nn� se zvukem
	IDirectSoundBuffer* DSBuffer;	// (4) DirectSound buffer
	int		DSFreq;					// (4) frekvence pro DirectSound
	short	DSVolume;				// (2) hlasitost pro DirectSound
	short	DSBalance;				// (2) vyv�en� pro DirectSound
	WORD	SoundID;				// (2) rozli�ovac� k�d zvuku
	short	res;					// (2) .. pro zarovn�n�
} SOUNDCHAN;

extern	HWAVEOUT	WaveOut;						// handle v�stupn�ho za��zen� (NULL=nen� otev�eno)
extern	WAVEHDR		WaveHeader[SOUNDBUFFERU];		// popisova�e dat
extern	BYTE*		SoundBuf[SOUNDBUFFERU];			// zvukov� buffery
extern	int			SoundKanalu;					// po�et aktivn�ch zvukov�ch kan�l�
extern	SOUNDCHAN*	SoundChan;						// buffer zvukov�ch kan�l�
extern	int			SoundError;						// povolen� opakovan�ho otev�en� zvuku (p�i < 0)
extern	bool		WaveFormatPCM;					// je p�ehr�v�n form�t PCM (vlastn� obsluha)
extern	char*		WaveData2;						// ukazatel dat pro nestandardn� form�t
extern	int			WaveSize2;						// ��ta� dat pro nestandardn� form�t
extern	int			WaveLoop;						// ��ta� opakov�n� pro nestandardn� form�t
extern	CSound		Sound2;							// p�ehr�van� nestandardn� zvuk, naposledy zadan� zvuk
extern	bool		SoundPausing;					// p��znak pauzy zvuku
extern	WORD		SoundID;						// rozli�ovac� k�d zvuku

/////////////////////////////////////////////////////////////////////////////
// funkce pro inicializaci DirectSound

typedef	HRESULT (WINAPI *DIRECTSOUNDCREATE) (LPGUID, LPDIRECTSOUND*, IUnknown FAR *);

/////////////////////////////////////////////////////////////////////////////
// deinicializace DirectSound

void TermDirectSound();

/////////////////////////////////////////////////////////////////////////////
// obsluha zvuku

void PlaySoundOpen(CSound sound, int loop,	// otev�en� zvukov�ho kan�lu
		bool stop, double volume, double balance, double speed);
void PlaySoundClose(CSound sound);	// uzav�en� zvukov�ho kan�lu
void PlaySoundStop();				// zastaven� p�ehr�v�n� zvuk�
void PlaySoundBack();				// obsluha p�ehr�v�n� zvuku na pozad�
void SetSoundPos(double pos);		// nastaven� pozice p�ehr�v�n� zvuku (v sekund�ch)
double GetSoundPos();				// zji�t�n� pozice p�ehr�v�n� zvuku (v sekund�ch)
void SoundPause(bool pause);		// pauza p�ehr�v�n� zvuku


/////////////////////////////////////////////////////////////////////////////
// inline vol�n� p��kazu - zvuk

inline void FSound(CSound& sound)
{
	ExecItem++;
	ExecItem[-1].ExecSnd(sound);
}


// funkce
void _fastcall FSoundFunc(CSound& sound);		// funkce s n�vratem zvuku

// prom�nn�
void _fastcall FSoundEmpty(CSound& sound);		// pr�zdn� zvuk
void _fastcall FSoundObj(CSound& sound);		// glob�ln� zvuk (Data = index)
void _fastcall FSoundObjList(CSound& sound);	// globaln� zvuk v seznamu (Data = index, List = seznam)
void _fastcall FSoundLoc(CSound& sound);		// lok�ln� zvuk (Data = index)
void _fastcall FSoundLocList(CSound& sound);	// lok�ln� zvuk v seznamu (Data = index, List = seznam)

// konverze
void _fastcall FSoundConv8Bit(CSound& sound);	// konverze na 8 bit�
void _fastcall FSoundConv16Bit(CSound& sound);	// konverze na 16 bit�
void _fastcall FSoundConvStereo(CSound& sound);	// konverze na stereo
void _fastcall FSoundConvMono(CSound& sound);	// konverze na mono
void _fastcall FSoundConv11025(CSound& sound);	// konverze na frekvenci 11025
void _fastcall FSoundConv22050(CSound& sound);	// konverze na frekvenci 22050
void _fastcall FSoundConv44100(CSound& sound);	// konverze na frekvenci 44100
void _fastcall FSoundConvCopy(CSound& sound);	// duplikace zvuku
void _fastcall FSoundAdd1(CSound& sound);		// sou�et jednoho zvuku - ponech� se beze zm�ny
void _fastcall FSoundAdd(CSound& sound);		// sou�et zvuk� (Data = po�et zvuk� - 1)
void _fastcall FSoundSpeed(CSound& sound);		// konverze rychlosti zvuku
void _fastcall FTonGen(CSound& sound);			// t�nov� gener�tor

void _fastcall FGetFileSound(CSound& sound);	// na�ten� zvuku ze souboru

void _fastcall FSoundConvPCM(CSound& sound);	// dekomprimace zvuku
