
#include "Main.h"

/***************************************************************************\
*																			*
*									Editor zvuk�							*
*																			*
\***************************************************************************/

namespace EditSound
{

/////////////////////////////////////////////////////////////////////////////
// parametry

// editovan� zvuk
int		Index = 0;						// editovan� polo�ka
SOUNDDATA* Data = NULL;					// editovan� zvuk
int		BytesPerSec;					// po�et bajt� za sekundu

// zobrazen�
int		DLeft;							// sou�adnice X displeje
int		DTop;							// sou�adnice Y displeje
int		DWidth;							// ���ka displeje
int		DHeight;						// v��ka displeje

// zobrazen� �asu
HWND	DispWnd = NULL;					// okno zobrazen� �asu
int		LastTime = 0;					// naposledy zobrazen� �as (sekund)

// ovl�d�n�
BOOL	Play = FALSE;					// prob�h� p�ehr�v�n�
BOOL	Record = FALSE;					// prob�h� nahr�v�n�
BOOL	Loop = FALSE;					// zapnuta smy�ka
BOOL	Pause = FALSE;					// zapauzov�n� p�ehr�v�n�

// p�ehr�v�n� a nar�v�n�
HWAVEOUT	WaveOut;					// handle za��zen� pro p�ehr�v�n�
HWAVEIN		WaveIn;						// handle za��zen� pro nahr�v�n�
WAVEHDR		WaveHdr[2];					// popisova� bufferu dat
DWORD		Timer = 0;						// �asova� pro zobrazen� �asu
char*		DataData = NULL;			// ukazatel dat p�i p�ehr�v�n�
int			DataSize = 0;				// ��ta� zbyl�ch dat p�i p�ehr�v�n�
int			MaxBufSize = 0x20000;		// max. velikost bufferu

#define SoundTimerID 15123				// ID �asova�e
#define MAXLENGTH 60					// maxim�ln� d�lka nahr�v�n� (sekund)
#define SOUNDDEFSAMPLES		22050
#define SOUNDDEFCHANNELS	1
#define SOUNDDEFBITS		16
#define SOUNDDEFRATE		(SOUNDDEFSAMPLES * SOUNDDEFCHANNELS * (SOUNDDEFBITS/8))
#define SOUNDDEFALIGN		(SOUNDDEFCHANNELS * (SOUNDDEFBITS/8))

/////////////////////////////////////////////////////////////////////////////
// inicializace p�i startu programu

void StartInit()
{
// vytvo�en� okna displeje
	DispWnd = ::CreateWindowEx(
		WS_EX_CLIENTEDGE | WS_EX_DLGMODALFRAME, // roz���en� styl
		_T("STATIC"),					// t��da
		_T("00:00"),					// text
		WS_CHILD | SS_CENTER,			// styl
		300,							// X
		200,							// Y
		200,							// ���ka
		100,							// v��ka
		MainFrame,						// rodi�
		NULL,							// ID
		hInstance,						// instance
		NULL);							// data

// nastaven� fontu okna displeje
	HFONT font = ::CreateFont(80, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, FIXED_PITCH | FF_MODERN, _T("Courier New"));
	::SendMessage(DispWnd, WM_SETFONT, (WPARAM)font, FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// zah�jen� editace (mus� b�t platn� index!)

void BegEdit(int index)
{
// index prvku
	Index = index;
	ASSERT(Sound.IsValid(index));

// vypnut� p��padn�ho p�ehr�v�n�
	OnStop();

// kopie p�ed editac�
	Sound[index].CopyWrite();

// adresa zvuku
	Data = Sound[index].Data();

// po�et bajt� za sekundu
	BytesPerSec = Data->Samples * Data->Channels * (Data->Bits/8);
	if (BytesPerSec == 0)
	{
		BytesPerSec = Data->Samples * Data->Channels / 2;
		if ((Data->Format >= 0x50) && (Data->Format < 0x60))
		{
			WAVEFORMATEX* wfx = (WAVEFORMATEX*)(Data->Data);
			BytesPerSec = wfx->nAvgBytesPerSec;
		}
	}
	if (BytesPerSec <= 0) BytesPerSec = 1;

// zobrazen� d�lky zvuku
	DispLength();

// zobrazen� okna
	::ShowWindow(DispWnd, SW_SHOW);

// zobrazen�
	Disp();

// aktualizace voleb
	UpdateMenu();
}


/////////////////////////////////////////////////////////////////////////////
// p�esun okna displeje

HDWP MoveDisp(HDWP hdwp)
{
	int oldwidth = DWidth;
	int oldheight = DHeight;

	DWidth = 235;
	if (DWidth > EditWidth-4) DWidth = EditWidth-4;
	if (DWidth < 1) DWidth = 1;

	DHeight = 85;
	if (DHeight > EditHeight-4) DHeight = EditHeight-4;
	if (DHeight < 1) DHeight = 1;

	DLeft = EditX + (EditWidth - DWidth)/2;
	DTop = EditY + (EditHeight - DHeight)/2;

	hdwp = ::DeferWindowPos(hdwp, DispWnd, NULL, 
			DLeft, DTop, DWidth, DHeight, 
			(((oldwidth != DWidth) || (oldheight != DHeight)) ? SWP_NOCOPYBITS : 0) |
			SWP_DRAWFRAME | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	return hdwp;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� �asov�ho �daje (v sekund�ch)

void DispSet(int time)
{
	if (time != LastTime)
	{
		LastTime = time;

		CText txt;

		txt = (TCHAR)((time / 600) % 10 + '0');
		time %= 600;

		txt.Add((TCHAR)(time / 60 + '0'));
		time %= 60;

		txt.Add(_T(':'));

		txt.Add((TCHAR)(time / 10 + '0'));
		time %= 10;

		txt.Add((TCHAR)(time + '0'));

		txt.SetWindowText(DispWnd);
	}
}


/////////////////////////////////////////////////////////////////////////////
// zobrazen� d�lky zvuku

void DispLength()
{
	DispSet((Data->Size + BytesPerSec/2)/ BytesPerSec);
}


/////////////////////////////////////////////////////////////////////////////
// zobrazen� okna (vymaz�n� okol� kolem displeje)

void Disp()
{
// otev�en� kontextu displeje
	HDC dc = ::GetDC(MainFrame);

// p��prava �t�tce k vymaz�n� podkladu
	HBRUSH brush = (HBRUSH)::GetStockObject(LTGRAY_BRUSH); // �t�tec k vymaz�n� plochy

// vymaz�n� plochy naho�e nad displejem
	RECT rc;
	rc.left = EditX + 2;
	rc.right = EditX + EditWidth - 2;
	rc.top = EditY + 2;
	rc.bottom = DTop;
	if (rc.top < rc.bottom)
	{
		::FillRect(dc, &rc, brush);
	}

// vymaz�n� plochy dole pod displejem (left a right je nastaveno)
	rc.top = DTop + DHeight;
	rc.bottom = EditY + EditHeight - 2;
	if (rc.top < rc.bottom)
	{
		::FillRect(dc, &rc, brush);
	}

// vymaz�n� plochy vlevo od displeje (left je nastaveno)
	rc.right = DLeft;
	rc.top = DTop;
	rc.bottom = DTop + DHeight;
	if (rc.left < rc.right)
	{
		::FillRect(dc, &rc, brush);
	}

// vymaz�n� plochy vpravo od displeje (top a bottom je nastaveno)
	rc.left = DLeft + DWidth;
	rc.right = EditX + EditWidth - 2;
	if (rc.left < rc.right)
	{
		::FillRect(dc, &rc, brush);
	}

// zru�en� �t�tce podkladu (i kdy� podle dokumentace ru�en� nen� nutn�)
	::DeleteObject(brush);

// uvoln�n� kontextu displeje
	::ReleaseDC(MainFrame, dc);
}


/////////////////////////////////////////////////////////////////////////////
// start p�ehr�v�n�

void OnPlay()
{
// v pauze pokra�ov�n� pauzou
	if (Pause)
	{
		OnPause();
		return;
	}

// v m�du p�ehr�v�n� vypnut� p�ehr�v�n�
	if (Play)
	{
		OnStop();
		return;
	}

// kontrola povolen� operace
	if (Record || (Data->Size <= Data->SizeHead))
	{
		UpdateMenu();
		return;
	}

// p��prava popisova�e dat
	WaveHdr[0].dwFlags = WHDR_DONE;
	WaveHdr[1].dwFlags = WHDR_DONE;

// p��prava popisova�e form�tu
	WAVEFORMATEX	wf0;
	WAVEFORMATEX* wf = &wf0;
	
	if (Data->Format == WAVE_FORMAT_PCM)
	{
		MaxBufSize = 0x400000;							// pro PCM je velik� buffer (nedekomprimuje se - zvol�me 4 MB)
		wf->wFormatTag = (WORD)Data->Format;			// form�t dat
		wf->nChannels = (WORD)Data->Channels;			// po�et kan�l�
		wf->nSamplesPerSec = Data->Samples;				// vzorkovac� kmito�et
		wf->nAvgBytesPerSec = BytesPerSec;				// p�enosov� rychlost dat
		wf->nBlockAlign = (WORD)(Data->Channels*(Data->Bits/8));	// zarovn�v�n� dat
		wf->wBitsPerSample = (WORD)Data->Bits;			// po�et bit� na vzorek
		wf->cbSize = 0;									// dopl�kov� data ve struktu�e
	}
	else
	{
		wf = (WAVEFORMATEX*)Data->Data;
		MaxBufSize = 0x100000;							// pro ostatn� form�ty omez�me na 1 MB
		if ((Data->Format >= 0x50) && (Data->Format < 0x60))
		{
			MaxBufSize = 0x20000;						// pro MPEG form�ty omez�me na 128 KB - velk� dekomprese
		}
	}

// otev�en� p�ehr�vac�ho za��zen�
	MMRESULT res = ::waveOutOpen(
		&WaveOut,									// handle za��zen�
		WAVE_MAPPER,								// vybrat nejvhodn�j�� za��zen�
		wf,											// popisova� form�tu
		NULL,										// nen� CALLBACK funkce
		NULL,										// data CALLBACK
		CALLBACK_NULL);								// nen� CALLBACK

	if (res != MMSYSERR_NOERROR)
	{
		UpdateMenu();
		return;
	}

// p��prava ukazatele dat
	DataData = (char*)Data->Data + Data->SizeHead;		// adresa dat
	DataSize = Data->Size - Data->SizeHead;				// velikost dat
	if (DataSize > MaxBufSize) Loop = false;			// pro v�ce buffer� nen� zaji�t�no opakov�n�

// spu�t�n� �asova�e
	Timer = ::SetTimer(MainFrame, SoundTimerID, TimerConst, NULL);

// zapnut� p��znaku p�ehr�v�n�
	Play = TRUE;

// aktualizace voleb
	UpdateMenu();

// zah�jen� p�ehr�v�n�
	OnTimer(SoundTimerID);
}


/////////////////////////////////////////////////////////////////////////////
// obsluha �asova�e (TRUE=obslou�eno)

BOOL OnTimer(UINT timerID)
{
// obsluha zobrazen� �asu
	if (timerID == SoundTimerID)
	{
		if (Play)
		{

// zastaven� p�ehr�v�n�
			if ((DataSize <= 0) && (WaveHdr[0].dwFlags & WHDR_DONE) && (WaveHdr[1].dwFlags & WHDR_DONE))
			{
				OnStop();
			}
			else
			{

// obsluha buffer�
				for (int i = 0; ((i < 2) && (DataSize > 0)); i++)
				{
					WAVEHDR* wh = &WaveHdr[i];
					if (wh->dwFlags & WHDR_DONE)
					{

// uvoln�n� bufferu pro z�pis
						if (wh->dwFlags & WHDR_PREPARED)
						{
							::waveOutUnprepareHeader(WaveOut, wh, sizeof(WAVEHDR));
						}

// p��prava popisova�e dat
						wh->lpData = DataData;	// adresa dat
						int size = DataSize;								// velikost dat
						if (size > MaxBufSize) size = MaxBufSize;			// omezen� velikosti bufferu
						DataSize -= size;									// sn�en� ��ta�e velikosti dat
						DataData += size;									// posun ukazatele dat
						wh->dwBufferLength = size;						// velikost dat
						wh->dwBytesRecorded = size;						// velikost dat nahran�ch v bufferu
						wh->dwUser = 0;										// u�ivatelsk� data
						wh->dwFlags = WHDR_BEGINLOOP | WHDR_ENDLOOP; // parametry
						wh->dwLoops = (Loop) ? 200000000 : 1;		// po�et opakov�n�
						wh->lpNext = NULL;							// nen� dal�� buffer
						wh->reserved = 0;

// p��prava bufferu k p�ehr�v�n�
						MMRESULT res = ::waveOutPrepareHeader(
						WaveOut,									// handle za��zen�
						wh,											// popisova� dat
						sizeof(WAVEHDR));							// velikost popisova�e dat

						if (res != MMSYSERR_NOERROR)
						{
							OnStop();
							return TRUE;
						}

// p�ed�n� bufferu p�ehr�vac�mu za��zen�
						res = ::waveOutWrite(
						WaveOut,									// handle za��zen�
						wh,											// popisova� dat
						sizeof(WAVEHDR));							// velikost popisova�e dat

						if (res != MMSYSERR_NOERROR)
						{
							OnStop();
							return TRUE;
						}
					}
				}
			}
		}

		if (Play || Record)
		{

// zastaven� nahr�v�n�
			if (Record && (WaveHdr[0].dwFlags & WHDR_DONE))
			{
				OnStop();
			}

// p�iprava bufferu k na�ten� pozice
			MMTIME tm;
			tm.wType = TIME_SAMPLES;
			tm.u.sample = 0;

// na�ten� aktu�ln� pozice
			if (Play)
			{
				::waveOutGetPosition(WaveOut, &tm, sizeof(MMTIME));
			}
			else
			{
				::waveInGetPosition(WaveIn, &tm, sizeof(MMTIME));
			}

// zobrazen� aktu�ln� pozice, zastaven� operace
			DispSet(tm.u.sample / Data->Samples );
		}
		return TRUE;
	}

	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// start nahr�v�n�

void OnRecord()
{
// v m�du nahr�v�n� vypnuti nahr�v�n�
	if (Record)
	{
		OnStop();
		return;
	}

// kontrola povolen� operace
	if (Play)
	{
		UpdateMenu();
		return;
	}

// p��prava popisova�e dat
	WaveHdr[0].dwFlags = WHDR_DONE;
	WaveHdr[1].dwFlags = WHDR_DONE;

// p��prava popisova�e form�tu
	WAVEFORMATEX	wf;
	wf.wFormatTag = WAVE_FORMAT_PCM;				// form�t dat
	wf.nChannels = SOUNDDEFCHANNELS;				// po�et kan�l�
	wf.nSamplesPerSec = SOUNDDEFSAMPLES;			// vzorkovac� kmito�et
	wf.nAvgBytesPerSec = SOUNDDEFRATE;				// p�enosov� rychlost dat
	wf.nBlockAlign = SOUNDDEFALIGN;					// zarovn�v�n� dat
	wf.wBitsPerSample = SOUNDDEFBITS;				// po�et bit� na vzorek
	wf.cbSize = 0;									// dopl�kov� data ve struktu�e

// otev�en� nahr�vac�ho za��zen�
	MMRESULT res = ::waveInOpen(
		&WaveIn,									// handle za��zen�
		WAVE_MAPPER,								// vybrat nejvhodn�j�� za��zen�
		&wf,										// popisova� form�tu
		NULL,										// nen� CALLBACK funkce
		NULL,										// data CALLBACK
		CALLBACK_NULL);								// nen� CALLBACK

	if (res != MMSYSERR_NOERROR)
	{
		UpdateMenu();
		return;
	}

// �schova pro UNDO
	Undo.AddSndSet(Index, Sound[Index]);
	Undo.Stop();

// p��prava bufferu
	Sound[Index].New(MAXLENGTH*SOUNDDEFRATE);		// vytvo�en� nov�ho bufferu
	Sound[Index].Samples(SOUNDDEFSAMPLES);
	Sound[Index].Format(WAVE_FORMAT_PCM);
	Sound[Index].Channels(SOUNDDEFCHANNELS);
	Sound[Index].Bits(SOUNDDEFBITS);
	SetModi();										// p��znak modifikace programu
	Data = Sound[Index].Data();						// adresa dat
	BytesPerSec = SOUNDDEFRATE;						// po�et bajt� za sekundu

// p��prava popisova�e dat
	WaveHdr[0].lpData = (char*)Data->Data;				// adresa dat
	WaveHdr[0].dwBufferLength = MAXLENGTH*SOUNDDEFRATE;// velikost dat
	WaveHdr[0].dwBytesRecorded = 0;					// velikost dat nahran�ch v bufferu
	WaveHdr[0].dwUser = 0;								// u�ivatelsk� data
	WaveHdr[0].dwFlags = 0;							// parametry
	WaveHdr[0].dwLoops = 1;							// po�et opakov�n�
	WaveHdr[0].lpNext = NULL;							// nen� dal�� buffer
	WaveHdr[0].reserved = 0;

// p��prava bufferu k nahr�v�n�
	res = ::waveInPrepareHeader(
		WaveIn,										// handle za��zen�
		&WaveHdr[0],									// popisova� dat
		sizeof(WAVEHDR));							// velikost popisova�e dat

	if (res != MMSYSERR_NOERROR)
	{
		::waveInClose(WaveIn);						// uzav�en� za�izen�
		WaveIn = NULL;								// zru�en� handle za��zen�

		UpdateMenu();
		return;
	}

// p�ed�n� bufferu nahr�vac�mu za��zen�
	res = ::waveInAddBuffer(
		WaveIn,										// handle za��zen�
		&WaveHdr[0],									// popisova� dat
		sizeof(WAVEHDR));							// velikost popisova�e dat

	if (res != MMSYSERR_NOERROR)
	{
		::waveInUnprepareHeader(
			WaveIn,									// handle za��zen�
			&WaveHdr[0],								// popisova� dat
			sizeof(WAVEHDR));						// velikost popisova�e dat

		::waveInClose(WaveIn);						// uzav�en� za�izen�
		WaveIn = NULL;								// zru�en� handle za��zen�

		UpdateMenu();
		return;
	}

// spu�t�n� �asova�e
	Timer = ::SetTimer(MainFrame, SoundTimerID, TimerConst, NULL);

// zah�jen� nahr�v�n�
	res = ::waveInStart(WaveIn);

// zapnut� p��znaku nahr�v�n�
	Record = TRUE;

// aktualizace voleb
	UpdateMenu();
}


/////////////////////////////////////////////////////////////////////////////
// zastaven� nahr�v�n� i p�ehr�v�n� (m��e b�t vol�no i zvenku p�i neaktivn�m editoru)

void OnStop()
{
// zastaven� �asova�e (mus� b�t p�ed zobrazen�m d�lky)
	if (Timer)
	{
		::KillTimer(MainFrame, Timer);
		Timer = 0;
	}

// vypnut� p�ehr�v�n�
	if (Play)
	{

// p��znak ukon�en� p�ehr�v�n�
		Play = FALSE;

// p�eru�en� prob�haj�c�ho nahr�v�n�
		VERIFY(::waveOutReset(WaveOut) == MMSYSERR_NOERROR);

// uvoln�n� bufferu
		if (WaveHdr[1].dwFlags & WHDR_PREPARED)
		{
			VERIFY(::waveOutUnprepareHeader(
				WaveOut,								// handle za��zen�
				&WaveHdr[1],							// popisova� dat
				sizeof(WAVEHDR)) == MMSYSERR_NOERROR);	// velikost popisova�e dat
		}

		if (WaveHdr[0].dwFlags & WHDR_PREPARED)
		{
			VERIFY(::waveOutUnprepareHeader(
				WaveOut,								// handle za��zen�
				&WaveHdr[0],							// popisova� dat
				sizeof(WAVEHDR)) == MMSYSERR_NOERROR);	// velikost popisova�e dat
		}

// uzav�en� p�ehr�vac�ho za��zen�
		VERIFY(::waveOutClose(WaveOut) == MMSYSERR_NOERROR);	// uzav�en� za�izen�
		WaveOut = NULL;								// zru�en� handle za��zen�

// zobrazen� d�lky zvuku
		DispLength();
	}

// vypnut� nahr�v�n�
	if (Record)
	{

// p��znak ukon�en� nahr�v�n�
		Record = FALSE;

// p�eru�en� prob�haj�c�ho nahr�v�n�
		::waveInStop(WaveIn);
		::waveInReset(WaveIn);

// uvoln�n� bufferu
		::waveInUnprepareHeader(
			WaveIn,									// handle za��zen�
			&WaveHdr[0],							// popisova� dat
			sizeof(WAVEHDR));						// velikost popisova�e dat

// �schova velikosti dat
		int num = WaveHdr[0].dwBytesRecorded;
		if (num > MAXLENGTH*SOUNDDEFRATE) num = MAXLENGTH*SOUNDDEFRATE;
		if (num < 0) num = 0;

// uzav�en� nahr�vac�ho za��zen�
		::waveInClose(WaveIn);						// uzav�en� za�izen�
		WaveIn = NULL;								// zru�en� handle za��zen�

// nastaven� velikosti bufferu
		Sound[Index].ReSize(num);					// nastaven�

// zobrazen� d�lky zvuku
		DispLength();
	}

// aktualizace voleb
	Pause = FALSE;
	UpdateMenu();
}


/////////////////////////////////////////////////////////////////////////////
// p�epnut� p��znaku opakov�n�

void OnLoop()
{
// kontrola povolen� operace
	if (Play || Record) return;

// p�epnut� p��znaku opakov�n�
	Loop = !Loop;

// aktualizace voleb
	UpdateMenu();
}


/////////////////////////////////////////////////////////////////////////////
// pozastaven� p�ehr�v�n�

void OnPause()
{
// kontrola povolen� operace
	if (!Play) return;

// p�epnut� p��znaku opakov�n�
	Pause = !Pause;

// zastaven� p�ehr�v�n�
	if (Pause)
	{
		::waveOutPause(WaveOut);
	}
	else
	{
		::waveOutRestart(WaveOut);
	}

// aktualizace voleb
	UpdateMenu();
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace voleb

void UpdateMenu()
{
	if (ProgMode && (FocusBuf == BufEdiID) && (EditMode == BufSndID))
	{
		CheckCommand(IDN_PLAY, Play && !Pause);
		CheckCommand(IDN_LOOP, Loop);
		CheckCommand(IDN_RECORD, Record);
		CheckCommand(IDN_PAUSE, Pause);
		EnableCommand(IDN_PLAY, !Record);
		EnableCommand(IDN_STOP, Play || Record);
		EnableCommand(IDN_LOOP, !Play && !Record);
		EnableCommand(IDN_RECORD, !Play && (VerzeOS <= 0));   // JAKJAK 070611 dle MIN rady
		EnableCommand(IDN_PAUSE, Play);
	}
}


/////////////////////////////////////////////////////////////////////////////
// kopie do bufferu

void Copy()
{
// otev�en� schr�nky
	if (!::OpenClipboard(MainFrame))
		return;

// zapnut� �ekac�ho kurzoru
	BeginWaitCursor();

// vypr�zdn�n� schr�nky
	if (!::EmptyClipboard())
	{
		EndWaitCursor();
		::CloseClipboard();
		return;
	}

// vytvo�en� glob�ln�ho bufferu pro data
	HGLOBAL global = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, Data->Size + sizeof(WAVHEAD));
	if (global == NULL)
	{
		EndWaitCursor();
		::CloseClipboard();
		return;
	}

// uzamknut� bufferu
	BYTE* data = (BYTE*) ::GlobalLock(global);

// inicializace z�hlav� souboru
	((WAVHEAD*)data)->tWavIdent[0] = 'R';
	((WAVHEAD*)data)->tWavIdent[1] = 'I';
	((WAVHEAD*)data)->tWavIdent[2] = 'F';
	((WAVHEAD*)data)->tWavIdent[3] = 'F';
	((WAVHEAD*)data)->nFileSize = Data->Size + sizeof(WAVFORMAT) + sizeof(WAVDATA);

// inicializace form�tu
	WAVFORMAT* wf = &(((WAVHEAD*)data)->WavFormat);
	wf->tFormatIdent[0] = 'W';
	wf->tFormatIdent[1] = 'A';
	wf->tFormatIdent[2] = 'V';
	wf->tFormatIdent[3] = 'E';
	wf->tFormatIdent[4] = 'f';
	wf->tFormatIdent[5] = 'm';
	wf->tFormatIdent[6] = 't';
	wf->tFormatIdent[7] = ' ';
	wf->nFormatSize = 16;

	wf->wFormatTag = WAVE_FORMAT_PCM;
	wf->nChannels = (WORD)Data->Channels;
	wf->nSamplesPerSec = Data->Samples;
	wf->nAvgBytesPerSec = BytesPerSec;
	wf->nBlockAlign = (WORD)((Data->Bits/8) * Data->Channels);
	wf->wBitsPerSample = (WORD)Data->Bits;
	
// inicializace dat
	WAVDATA* wd = &(((WAVHEAD*)data)->WavData);
	wd->tDataIdent[0] = 'd';
	wd->tDataIdent[1] = 'a';
	wd->tDataIdent[2] = 't';
	wd->tDataIdent[3] = 'a';
	wd->nDataSize = Data->Size;

// p�enesen� dat
	MemCopy(data + sizeof(WAVHEAD), Data->Data, Data->Size);

// odemknut� bufferu
	::GlobalUnlock(global);

// ulo�en� dat do schr�nky
	::SetClipboardData(CF_WAVE, global);

// uzav�en� schr�nky
	::CloseClipboard();

// vypnut� �ekac�ho kurzoru
	EndWaitCursor();

// aktualizace voleb bloku
	UpdateClipboard();
}


/////////////////////////////////////////////////////////////////////////////
// navr�cen� z bufferu

void Paste()
{
// lok�ln� prom�nn�
	HGLOBAL		global;			// glob�ln� buffer s daty
	BYTE*		data;			// ukazatel na data
	int			size;			// velikost dat souboru
	int			channels;		// po�et kan�l�
	int			samples;		// vzorkovac� kmito�et
	int			bits;			// po�et bit� na vzorek

// otev�en� schr�nky
	if (!::OpenClipboard(MainFrame))
		return;

// zapnut� �ekac�ho kurzoru
	BeginWaitCursor();

// na�ten� dat schr�nky
	global = ::GetClipboardData(CF_WAVE);
	if (global == NULL)
	{
		EndWaitCursor();
		::CloseClipboard();
		return;
	}
	
// uzamknut� bufferu
	data = (BYTE*) ::GlobalLock(global);

// kontrola platnosti form�tu souboru
	if ((data[0] == 'R') &&
		(data[1] == 'I') &&
		(data[2] == 'F') &&
		(data[3] == 'F'))
	{
// velikost dat
		size = ((WAVHEAD*)data)->nFileSize;
		if (size < 0) size = 0;
		data += 8;

// kontrola popisova�e form�tu
		if ((data[0] == 'W') &&
			(data[1] == 'A') &&
			(data[2] == 'V') &&
			(data[3] == 'E') &&
			(data[4] == 'f') &&
			(data[5] == 'm') &&
			(data[6] == 't') &&
			(data[7] == ' ') &&
			(((WAVFORMAT*)data)->wFormatTag == WAVE_FORMAT_PCM))
		{
			channels = ((WAVFORMAT*)data)->nChannels;
			if (channels != 2) channels = 1;

			samples = ((WAVFORMAT*)data)->nSamplesPerSec;
			if (samples < 0) samples = SOUNDDEFSAMPLES;

			bits = ((WAVFORMAT*)data)->wBitsPerSample;
			if (bits != 16) bits = 8;

			size -= ((WAVFORMAT*)data)->nFormatSize + 12;
			data += ((WAVFORMAT*)data)->nFormatSize + 12;

// nalezen� bloku dat
			for (;;)
			{
				if (size < 10) break;

				if ((data[0] == 'd') &&
					(data[1] == 'a') &&
					(data[2] == 't') &&
					(data[3] == 'a') &&
					(size < 700000000))
				{
					if (size > (int)((WAVDATA*)data)->nDataSize)
					{
						size = ((WAVDATA*)data)->nDataSize;
					}
					if (size < 0) size = 0;
					data += 8;

// kopie dat do bufferu
					Sound[Index].New(size);			// vytvo�en� nov�ho bufferu
					SetModi();						// p��znak modifikace programu
					Data = Sound[Index].Data();		// adresa dat
					BytesPerSec = samples * channels * (bits/8);
					Data->Samples = samples;
					Data->Format = WAVE_FORMAT_PCM;
					Data->Channels = (BYTE)channels;
					Data->Bits = (BYTE)bits;
					Data->SizeHead = 0;
					MemCopy(Data->Data, data, size);
					DispLength();
					break;
				}
				
				size -= ((WAVDATA*)data)->nDataSize + 8;
				data += ((WAVDATA*)data)->nDataSize + 8;
			}
		}
	}

// odemknut� bufferu
	::GlobalUnlock(global);

// uzav�en� schr�nky
	::CloseClipboard();

// vypnut� �ekac�ho kurzoru
	EndWaitCursor();

// aktualizace voleb bloku
	UpdateClipboard();
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace voleb bloku

void UpdateClipboard()
{
	if ((FocusBuf == BufEdiID) && (EditMode == BufSndID))
	{
		EnableCommand(IDN_CUT, FALSE);
		EnableCommand(IDN_COPY, Data->Size > 1);
		EnableCommand(IDN_PASTE, ::IsClipboardFormatAvailable(CF_WAVE));
		EnableCommand(IDN_CLEAR, FALSE);
		EnableCommand(IDN_ALL, FALSE);
	}
}


}
