
/***************************************************************************\
*																			*
*							Provádìní programu - hudba						*
*																			*
\***************************************************************************/

#define MUSICERRORWAIT	(5*18)		// èekání na nový pokus o otevøení zvuku
#define MUSICSTOPWAIT	(4*18)		// èítaè pro zastavení hudby
#define MUSICMAXDELKA	(24*60*60*1000) // max. délka hudby

// obsluha hudby
extern	UINT	MusicDevice;		// ID zaøízení MCI pro pøehrávání hudby (0=není otevøeno)
extern	int		MusicDelka;			// délka hudby v ms
extern	bool	MusicPlaing;		// pøíznak pøehrávání hudby
extern	bool	MusicPausing;		// pøíznak pauzy hudby
extern	bool	MusicEnd;			// pøíznak ukonèení pøehrávání
extern	int		MusicError;			// povolení opakovaného otevøení hudby (pøi < 0)
extern	int		MusicAktPos;		// aktuální pozice hudby

/////////////////////////////////////////////////////////////////////////////
// obsluha hudby

void MusicPlay(CMusic mus, bool loop);	// zahájení pøehrávání hudby
void SetMusicPos(int pos);			// nastavení pozice hudby v ms
void MusicPause();					// pauza pøehrávání
void MusicStop();					// ukonèení pøehrávání hudby
void OnMusicEnd();					// ukonèení pøehrávání (obsluha hlášení)
void PlayMusicBack();				// obsluha hudby na pozadí


/////////////////////////////////////////////////////////////////////////////
// inline volání pøíkazu - hudba

inline void FMusic(CMusic& music)
{
	ExecItem++;
	ExecItem[-1].ExecMus(music);
}


// funkce
void _fastcall FMusicFunc(CMusic& music);		// funkce s návratem hudby

// promìnné
void _fastcall FMusicEmpty(CMusic& music);		// prázdná hudba
void _fastcall FMusicObj(CMusic& music);		// globální hudba (Data = index)
void _fastcall FMusicObjList(CMusic& music);	// globalní hudba v seznamu (Data = index, List = seznam)
void _fastcall FMusicLoc(CMusic& music);		// lokální hudba (Data = index)
void _fastcall FMusicLocList(CMusic& music);	// lokální hudba v seznamu (Data = index, List = seznam)

void _fastcall FGetFileMusic(CMusic& music);	// naètení hudby ze souboru
