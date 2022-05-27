
#include "Main.h"

/***************************************************************************\
*																			*
*						Prov�d�n� programu - hudba							*
*																			*
\***************************************************************************/

#pragma optimize("t", on)			// optimalizace na maxim�ln� rychlost

// obsluha hudby
CString	MusicName;				// jm�no p�echodn�ho souboru s hudbou
UINT	MusicDevice = 0;		// ID za��zen� MCI pro p�ehr�v�n� hudby (0=nen� otev�eno)
int		MusicDelka = MUSICMAXDELKA;	// d�lka hudby v ms
bool	MusicPlaing = false;	// p��znak p�ehr�v�n� hudby (m��e b�t v pauze]
bool	MusicPausing = false;	// p��znak pauzy hudby
bool	MusicLoop = false;		// p��znak opakov�n� skladby
bool	MusicEnd = false;		// p��znak ukon�en� p�ehr�v�n�

int		MusicAktPos = 0;		// aktu�ln� pozice hudby
int		MusicStopPos = MUSICSTOPWAIT;	// ��ta� pro zastaven� hudby

int		MusicError = -1;		// povolen� opakovan�ho otev�en� hudby (p�i < 0)

/***************************************************************************\
*																			*
*								obsluha hudby								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// zah�jen� p�ehr�v�n� hudby

void MusicPlay(CMusic mus, bool loop)
{
// povolen� otev�en� hudbz
	if (MusicError >= 0) return;

// zastaven� p�ehr�van� hudby
	MusicStop();

// p��prava jm�na p�echodn�ho souboru
	MusicName.TempName();

// vytvo�en� p�echodn�ho souboru
	HANDLE file = ::CreateFile(MusicName, GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);

	if (file != INVALID_HANDLE_VALUE)
	{

// z�pis dat do p�echodn�ho souboru
		DWORD writen;
		BOOL result = ::WriteFile(file, mus.DataData(), mus.Size(), &writen, NULL);

// uzav�en� souboru
		::CloseHandle(file);

// kontrola operace z�pisu
		if (result && ((int)writen == mus.Size()))
		{

// otev�en� v�stupn�ho za��zen�
			MCI_OPEN_PARMS mcio;
			mcio.lpstrDeviceType = _T("sequencer");
			mcio.lpstrElementName = MusicName;
			result = ::mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE |
				MCI_OPEN_ELEMENT, (DWORD)&mcio);

// ID za��zen�
			if (result == 0)
			{
				MusicDevice = mcio.wDeviceID;

// nastaven� �asov�ho form�tu na milisekundy
				MCI_SET_PARMS mcis;
				mcis.dwTimeFormat = MCI_FORMAT_MILLISECONDS;
				::mciSendCommand(MusicDevice, MCI_SET, MCI_SET_TIME_FORMAT, (DWORD)&mcis);

// na�ten� d�lky skladby
				MCI_STATUS_PARMS mcit;
				mcit.dwItem = MCI_STATUS_LENGTH;
				mcit.dwReturn = 0;
				::mciSendCommand(MusicDevice, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)&mcit);
				MusicDelka = mcit.dwReturn;
				if (MusicDelka < 1) MusicDelka = 1;
				if (MusicDelka > MUSICMAXDELKA) MusicDelka = MUSICMAXDELKA;

// p�ehr�t� souboru
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

// p�i chyb� zru�en� p�echodn�ho souboru
		::DeleteFile(MusicName);
	}
	
// p�i chyb� zru�en� jm�na p�echodn�ho souboru
	MusicName.Empty();

// nastaven� ��ta�e pro dal�� pokus o otev�en�
	MusicError = MUSICERRORWAIT;
}


/////////////////////////////////////////////////////////////////////////////
// ukon�en� p�ehr�v�n� (obsluha hl�en�)

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
// nastaven� pozice hudby v ms

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
// pauza p�ehr�v�n�

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
// ukon�en� p�ehr�v�n� hudby (p�i vol�n� z programu je vypnut je�t� p��znak pauzy)

void MusicStop()
{
// vypnut� p��znaku p�ehr�v�n�
	MusicPlaing = false;
	MusicError = -1;

// uzav�en� p�ehr�vac�ho za��zen�
	if (MusicDevice)
	{
		::mciSendCommand(MusicDevice, MCI_STOP, MCI_WAIT, NULL);
		::mciSendCommand(MusicDevice, MCI_CLOSE, MCI_WAIT, NULL);
		MusicDevice = 0;
		MusicDelka = MUSICMAXDELKA;
		MusicAktPos = 0;
	}

// zru�en� p�echodn�ho souboru
	if (MusicName.IsNotEmpty())
	{
		::DeleteFile(MusicName);
		MusicName.Empty();
	}
}


/////////////////////////////////////////////////////////////////////////////
// obsluha hudby na pozad�

void PlayMusicBack()
{
// ��ta� povolen� otev�en� hudby
	MusicError--;

// ukon�en� p�ehr�v�n� hudby
	if (MusicEnd)
	{
		MusicEnd = false;
		OnMusicEnd();
	}

// na�ten� aktu�ln� pozice hudby
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

// nov� pokus o odpauzov�n�
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
// funkce s n�vratem hudby

void _fastcall FMusicFunc(CMusic& music)
{
// �schova indexu volan� funkce
	int data = ExecItem[-1].Data;

// �schova indexu prom�nn� s n�vratovou hodnotou
	int res = ExecItem[-1].List;

// inicializace lok�ln�ch prom�nn�ch
	FCommand();

// �schova ukazatele programu
	EXECITEM* oldexe = ExecItem;

// nov� adresa programu
	ExecItem = ProgBuf + data;

// kontrola hloubky vno�en�
	Hloubka--;
	if (Hloubka >= 0)
	{

// vyvol�n� funkce
		FCommand();
	}
	Hloubka++;

// n�vrat adresy programu
	ExecItem = oldexe;

// zru�en� po�adavku o p�eru�en�
	Break &= ~(BREAKFUNC | BREAKWHILE);

// n�vrat v�sledku operace
	music = Music[Music.Num() - res];

// zru�en� lok�ln�ch prom�nn�ch
	FCommand();
}


/***************************************************************************\
*																			*
*								prom�nn�									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// pr�zdn� hudba

void _fastcall FMusicEmpty(CMusic& music) { music.Empty(); };


/////////////////////////////////////////////////////////////////////////////
// glob�ln� hudba (Data = index)

void _fastcall FMusicObj(CMusic& music)
{
// index hudby
	int inx = ExecItem[-1].Data;

// nastaven� hudby
	music = Music[inx];
}


/////////////////////////////////////////////////////////////////////////////
// globaln� hudba v seznamu (Data = index, List = seznam)

void _fastcall FMusicObjList(CMusic& music)
{
// index seznamu
	int list = ExecItem[-1].List;

// index hudby, inkrementace indexu seznamu
	int inx = ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// nastaven� hudby
	music = Music[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lok�ln� hudba (Data = index)

void _fastcall FMusicLoc(CMusic& music)
{
// index hudby
	int inx = Music.Num() - ExecItem[-1].Data;

// nastaven� hudby
	music = Music[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lok�ln� hudba v seznamu (Data = index, List = seznam)

void _fastcall FMusicLocList(CMusic& music)
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index hudby, inkrementace indexu seznamu
	int inx = Music.Num() - ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// nastaven� hudby
	music = Music[inx];
}

/////////////////////////////////////////////////////////////////////////////
// na�ten� hudby ze souboru

void _fastcall FGetFileMusic(CMusic& music)
{
	music.Empty();
	music.LoadFile();
}
