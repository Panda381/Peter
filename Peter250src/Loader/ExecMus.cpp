
#include "Main.h"

/***************************************************************************\
*																			*
*						Provádìní programu - hudba							*
*																			*
\***************************************************************************/

#pragma optimize("t", on)			// optimalizace na maximální rychlost

// obsluha hudby
CString	MusicName;				// jméno pøechodného souboru s hudbou
UINT	MusicDevice = 0;		// ID zaøízení MCI pro pøehrávání hudby (0=není otevøeno)
int		MusicDelka = MUSICMAXDELKA;	// délka hudby v ms
bool	MusicPlaing = false;	// pøíznak pøehrávání hudby (mùže být v pauze]
bool	MusicPausing = false;	// pøíznak pauzy hudby
bool	MusicLoop = false;		// pøíznak opakování skladby
bool	MusicEnd = false;		// pøíznak ukonèení pøehrávání

int		MusicAktPos = 0;		// aktuální pozice hudby
int		MusicStopPos = MUSICSTOPWAIT;	// èítaè pro zastavení hudby

int		MusicError = -1;		// povolení opakovaného otevøení hudby (pøi < 0)

/***************************************************************************\
*																			*
*								obsluha hudby								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// zahájení pøehrávání hudby

void MusicPlay(CMusic mus, bool loop)
{
// povolení otevøení hudbz
	if (MusicError >= 0) return;

// zastavení pøehrávané hudby
	MusicStop();

// pøíprava jména pøechodného souboru
	MusicName.TempName();

// vytvoøení pøechodného souboru
	HANDLE file = ::CreateFile(MusicName, GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);

	if (file != INVALID_HANDLE_VALUE)
	{

// zápis dat do pøechodného souboru
		DWORD writen;
		BOOL result = ::WriteFile(file, mus.DataData(), mus.Size(), &writen, NULL);

// uzavøení souboru
		::CloseHandle(file);

// kontrola operace zápisu
		if (result && ((int)writen == mus.Size()))
		{

// otevøení výstupního zaøízení
			MCI_OPEN_PARMS mcio;
			mcio.lpstrDeviceType = _T("sequencer");
			mcio.lpstrElementName = MusicName;
			result = ::mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE |
				MCI_OPEN_ELEMENT, (DWORD)&mcio);

// ID zaøízení
			if (result == 0)
			{
				MusicDevice = mcio.wDeviceID;

// nastavení èasového formátu na milisekundy
				MCI_SET_PARMS mcis;
				mcis.dwTimeFormat = MCI_FORMAT_MILLISECONDS;
				::mciSendCommand(MusicDevice, MCI_SET, MCI_SET_TIME_FORMAT, (DWORD)&mcis);

// naètení délky skladby
				MCI_STATUS_PARMS mcit;
				mcit.dwItem = MCI_STATUS_LENGTH;
				mcit.dwReturn = 0;
				::mciSendCommand(MusicDevice, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)&mcit);
				MusicDelka = mcit.dwReturn;
				if (MusicDelka < 1) MusicDelka = 1;
				if (MusicDelka > MUSICMAXDELKA) MusicDelka = MUSICMAXDELKA;

// pøehrátí souboru
				MusicLoop = loop;
				MusicPlaing = true;
				MusicStopPos = MUSICSTOPWAIT;

				bool oldpause = MusicPausing;

				MusicPausing = true;
				if (MusicAktPos > 0) SetMusicPos(MusicAktPos);
				if (!oldpause) MusicPause();
				return;
			}
		}

// pøi chybì zrušení pøechodného souboru
		::DeleteFile(MusicName);
	}
	
// pøi chybì zrušení jména pøechodného souboru
	MusicName.Empty();

// nastavení èítaèe pro další pokus o otevøení
	MusicError = MUSICERRORWAIT;
}


/////////////////////////////////////////////////////////////////////////////
// ukonèení pøehrávání (obsluha hlášení)

void OnMusicEnd()
{
	MusicStopPos = MUSICSTOPWAIT;
	MusicError = -1;

	if (MusicPlaing)
	{
		if (MusicLoop)
		{
			MusicPausing = true;
			SetMusicPos(0);
			MusicPause();
		}
		else
		{
			MusicStop();
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastavení pozice hudby v ms

void SetMusicPos(int pos)
{
	if (pos < 0) pos = 0;
	if (pos >= MusicDelka) pos = MusicDelka-1;
	MusicAktPos = pos;
	MusicStopPos = MUSICSTOPWAIT;

	if (MusicPlaing)
	{
		MCI_SEEK_PARMS mcis;
		mcis.dwTo = pos;

		if (MusicPausing)
		{
			::mciSendCommand(MusicDevice, MCI_SEEK, MCI_TO | MCI_WAIT, (DWORD)&mcis);
		}
		else
		{
			MusicPause();
			::mciSendCommand(MusicDevice, MCI_SEEK, MCI_TO | MCI_WAIT, (DWORD)&mcis);
			MusicPause();
		}
	}
}
 

/////////////////////////////////////////////////////////////////////////////
// pauza pøehrávání

void MusicPause()
{
	MusicPausing = !MusicPausing;
	MusicStopPos = MUSICSTOPWAIT;

	if (MusicPlaing)
	{
		MusicError = -1;

		if (MusicPausing)
		{
			::mciSendCommand(MusicDevice, MCI_PAUSE, MCI_WAIT, NULL);
		}
		else
		{
			MCI_PLAY_PARMS mcip;
			mcip.dwCallback = (DWORD)MainFrame;
			if (::mciSendCommand(MusicDevice, MCI_PLAY, MCI_NOTIFY, (DWORD)&mcip))
			{
				MusicPausing = true;
				MusicError = MUSICERRORWAIT;
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// ukonèení pøehrávání hudby (pøi volání z programu je vypnut ještì pøíznak pauzy)

void MusicStop()
{
// vypnutí pøíznaku pøehrávání
	MusicPlaing = false;
	MusicError = -1;

// uzavøení pøehrávacího zaøízení
	if (MusicDevice)
	{
		::mciSendCommand(MusicDevice, MCI_STOP, MCI_WAIT, NULL);
		::mciSendCommand(MusicDevice, MCI_CLOSE, MCI_WAIT, NULL);
		MusicDevice = 0;
		MusicDelka = MUSICMAXDELKA;
		MusicAktPos = 0;
	}

// zrušení pøechodného souboru
	if (MusicName.IsNotEmpty())
	{
		::DeleteFile(MusicName);
		MusicName.Empty();
	}
}


/////////////////////////////////////////////////////////////////////////////
// obsluha hudby na pozadí

void PlayMusicBack()
{
// èítaè povolení otevøení hudby
	MusicError--;

// ukonèení pøehrávání hudby
	if (MusicEnd)
	{
		MusicEnd = false;
		OnMusicEnd();
	}

// naètení aktuální pozice hudby
	if (MusicPlaing)
	{
		MCI_STATUS_PARMS mcit;
		mcit.dwReturn = MusicAktPos;

		mcit.dwItem = MCI_STATUS_POSITION;
		::mciSendCommand(MusicDevice, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)&mcit);

		if ((int)mcit.dwReturn < 0) mcit.dwReturn = 0;
		if ((int)mcit.dwReturn > MusicDelka) mcit.dwReturn = MusicDelka;

		if (!MusicPausing && 
			((int)mcit.dwReturn > MusicDelka - 20) && 
			((int)mcit.dwReturn == MusicAktPos))
		{
			MusicStopPos--;
			if (MusicStopPos < 0) OnMusicEnd();
		}
		else
		{
			MusicAktPos = mcit.dwReturn;
			MusicStopPos = MUSICSTOPWAIT;
		}

// nový pokus o odpauzování
		if ((MusicError == 0) && MusicPausing)
		{
			MusicPause();
		}
	}
}


/***************************************************************************\
*																			*
*									funkce									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// funkce s návratem hudby

void _fastcall FMusicFunc(CMusic& music)
{
// úschova indexu volané funkce
	int data = ExecItem[-1].Data;

// úschova indexu promìnné s návratovou hodnotou
	int res = ExecItem[-1].List;

// inicializace lokálních promìnných
	FCommand();

// úschova ukazatele programu
	EXECITEM* oldexe = ExecItem;

// nová adresa programu
	ExecItem = ProgBuf + data;

// kontrola hloubky vnoøení
	Hloubka--;
	if (Hloubka >= 0)
	{

// vyvolání funkce
		FCommand();
	}
	Hloubka++;

// návrat adresy programu
	ExecItem = oldexe;

// zrušení požadavku o pøerušení
	Break &= ~(BREAKFUNC | BREAKWHILE);

// návrat výsledku operace
	music = Music[Music.Num() - res];

// zrušení lokálních promìnných
	FCommand();
}


/***************************************************************************\
*																			*
*								promìnné									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// prázdná hudba

void _fastcall FMusicEmpty(CMusic& music) { music.Empty(); };


/////////////////////////////////////////////////////////////////////////////
// globální hudba (Data = index)

void _fastcall FMusicObj(CMusic& music)
{
// index hudby
	int inx = ExecItem[-1].Data;

// nastavení hudby
	music = Music[inx];
}


/////////////////////////////////////////////////////////////////////////////
// globalní hudba v seznamu (Data = index, List = seznam)

void _fastcall FMusicObjList(CMusic& music)
{
// index seznamu
	int list = ExecItem[-1].List;

// index hudby, inkrementace indexu seznamu
	int inx = ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// nastavení hudby
	music = Music[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lokální hudba (Data = index)

void _fastcall FMusicLoc(CMusic& music)
{
// index hudby
	int inx = Music.Num() - ExecItem[-1].Data;

// nastavení hudby
	music = Music[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lokální hudba v seznamu (Data = index, List = seznam)

void _fastcall FMusicLocList(CMusic& music)
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index hudby, inkrementace indexu seznamu
	int inx = Music.Num() - ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// nastavení hudby
	music = Music[inx];
}

/////////////////////////////////////////////////////////////////////////////
// naètení hudby ze souboru

void _fastcall FGetFileMusic(CMusic& music)
{
	music.Empty();
	music.LoadFile();
}
