
/***************************************************************************\
*																			*
*							Prov�d�n� programu - hudba						*
*																			*
\***************************************************************************/

#define MUSICERRORWAIT	(5*18)		// �ek�n� na nov� pokus o otev�en� zvuku
#define MUSICSTOPWAIT	(4*18)		// ��ta� pro zastaven� hudby
#define MUSICMAXDELKA	(24*60*60*1000) // max. d�lka hudby

// obsluha hudby
extern	UINT	MusicDevice;		// ID za��zen� MCI pro p�ehr�v�n� hudby (0=nen� otev�eno)
extern	int		MusicDelka;			// d�lka hudby v ms
extern	bool	MusicPlaing;		// p��znak p�ehr�v�n� hudby
extern	bool	MusicPausing;		// p��znak pauzy hudby
extern	bool	MusicEnd;			// p��znak ukon�en� p�ehr�v�n�
extern	int		MusicError;			// povolen� opakovan�ho otev�en� hudby (p�i < 0)
extern	int		MusicAktPos;		// aktu�ln� pozice hudby

/////////////////////////////////////////////////////////////////////////////
// obsluha hudby

void MusicPlay(CMusic mus, bool loop);	// zah�jen� p�ehr�v�n� hudby
void SetMusicPos(int pos);			// nastaven� pozice hudby v ms
void MusicPause();					// pauza p�ehr�v�n�
void MusicStop();					// ukon�en� p�ehr�v�n� hudby
void OnMusicEnd();					// ukon�en� p�ehr�v�n� (obsluha hl�en�)
void PlayMusicBack();				// obsluha hudby na pozad�


/////////////////////////////////////////////////////////////////////////////
// inline vol�n� p��kazu - hudba

inline void FMusic(CMusic& music)
{
	ExecItem++;
	ExecItem[-1].ExecMus(music);
}


// funkce
void _fastcall FMusicFunc(CMusic& music);		// funkce s n�vratem hudby

// prom�nn�
void _fastcall FMusicEmpty(CMusic& music);		// pr�zdn� hudba
void _fastcall FMusicObj(CMusic& music);		// glob�ln� hudba (Data = index)
void _fastcall FMusicObjList(CMusic& music);	// globaln� hudba v seznamu (Data = index, List = seznam)
void _fastcall FMusicLoc(CMusic& music);		// lok�ln� hudba (Data = index)
void _fastcall FMusicLocList(CMusic& music);	// lok�ln� hudba v seznamu (Data = index, List = seznam)

void _fastcall FGetFileMusic(CMusic& music);	// na�ten� hudby ze souboru
