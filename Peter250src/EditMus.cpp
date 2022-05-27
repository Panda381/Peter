
#include "Main.h"

/***************************************************************************\
*																			*
*									Editor hudby							*
*																			*
\***************************************************************************/

namespace EditMusic
{

/////////////////////////////////////////////////////////////////////////////
// parametry

// editovan� hudba
int		Index = 0;						// editovan� polo�ka
MUSICDATA*	Data = NULL;				// data hudby
int		Delka = 0;						// d�lka skladby v ms

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
BOOL	Loop = FALSE;					// zapnuta smy�ka
BOOL	Pause = FALSE;					// zapauzov�n� p�ehr�v�n�

// p�ehr�v�n� a nar�v�n�
TCHAR		FileName[MAX_PATH + 101];	// jm�no p�echodn�ho souboru
MCIDEVICEID	DeviceID = 0;				// ID za��zen�
DWORD		Timer = 0;					// �asova� pro zobrazen� �asu

#define MusicTimerID 13263				// ID �asova�e

/////////////////////////////////////////////////////////////////////////////
// inicializace p�i startu programu

void StartInit()
{
// nen� soubor
	FileName[0] = 0;

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
	ASSERT(Music.IsValid(index));

// vypnut� p��padn�ho p�ehr�v�n�
	OnStop();

// kopie p�ed editac�
	Music[index].CopyWrite();

// adresa dat hudby
	Data = Music[index].Data();

// zobrazen� d�lky hudby
	OnPlay(TRUE);
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
// zobrazen� d�lky hudby

void DispLength()
{
	DispSet(Delka / 1000);
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
// start p�ehr�v�n� (init = jen zji�t�n� d�lky a p�eru�en�)

void OnPlay(BOOL init)
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
	if (Data->Size <= 1)
	{
		UpdateMenu();
		return;
	}

// p��prava jm�na p�echodn�ho souboru
	TCHAR tmppath[101];
	tmppath[0] = 0;
	::GetTempPath(100, tmppath);
	FileName[0] = 0;
	::GetTempFileName(tmppath, _T("PET"), 0, FileName);

// vytvo�en� p�echodn�ho souboru
	HANDLE file = ::CreateFile(FileName, GENERIC_READ | GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);

	if (file == INVALID_HANDLE_VALUE)
	{
		FileName[0] = 0;
		UpdateMenu();
		return;
	}

// z�pis dat do p�echodn�ho souboru
	DWORD writen;
	BOOL res = ::WriteFile(file, Data->Data, Data->Size, &writen, NULL);

// uzav�en� souboru
	::CloseHandle(file);

// kontrola operace z�pisu	
	if (!res)
	{
		::DeleteFile(FileName);
		FileName[0] = 0;
		UpdateMenu();
		return;
	}

// otev�en� v�stupn�ho za��zen�
    MCI_OPEN_PARMS mop;

    mop.lpstrDeviceType = _T("sequencer");
    mop.lpstrElementName = FileName;

	res = ::mciSendCommand(NULL, MCI_OPEN,
        MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD) &mop);
	
	if (res)
	{
		::DeleteFile(FileName);
		FileName[0] = 0;
		UpdateMenu();
		return;
	}
    DeviceID = mop.wDeviceID;

// nastaven� �asov�ho form�tu na milisekundy
	MCI_SET_PARMS msp;
	msp.dwTimeFormat = MCI_FORMAT_MILLISECONDS;

	res = ::mciSendCommand(DeviceID, MCI_SET, 
		MCI_SET_TIME_FORMAT, (DWORD) &msp);

// na�ten� d�lky skladby
	MCI_STATUS_PARMS mtp;
	mtp.dwItem = MCI_STATUS_LENGTH;

	res = ::mciSendCommand(DeviceID, MCI_STATUS, 
		MCI_STATUS_ITEM, (DWORD) &mtp);
	Delka = mtp.dwReturn;
	if (Delka < 0) Delka = 0;
	if (Delka > 1000000000) Delka = 1000000000;

// p�ehr�n� souboru
    MCI_PLAY_PARMS mpp;

	if (!init)
	{
	    mpp.dwCallback = (DWORD)MainFrame;
		mpp.dwFrom = 0;
		if ((int)mpp.dwFrom > Delka) mpp.dwFrom = Delka;
		res = ::mciSendCommand(DeviceID, MCI_PLAY, 
			MCI_NOTIFY | MCI_FROM, (DWORD) &mpp);
	}

	if (res || init)
	{
        ::mciSendCommand(DeviceID, MCI_CLOSE, 0, NULL);
		DeviceID = 0;
		::DeleteFile(FileName);
		FileName[0] = 0;
		UpdateMenu();
		return;
	}

// spu�t�n� �asova�e
	Timer = ::SetTimer(MainFrame, MusicTimerID, TimerConst, NULL);

// zapnut� p��znaku p�ehr�v�n�
	Play = TRUE;

// aktualizace voleb
	UpdateMenu();
}


/////////////////////////////////////////////////////////////////////////////
// obsluha �asova�e (TRUE=obslou�eno)

BOOL OnTimer(UINT timerID)
{
// obsluha zobrazen� �asu
	if (timerID == MusicTimerID)
	{
		if (Play)
		{
			MCI_STATUS_PARMS msp;
			msp.dwItem = MCI_STATUS_POSITION;

			::mciSendCommand(DeviceID, MCI_STATUS, 
				MCI_STATUS_ITEM, (DWORD) &msp);

			DispSet(msp.dwReturn/1000);
		}
		return TRUE;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// ukon�en� p�ehr�v�n�

void OnMMNotify(DWORD flags, int devID)
{
	if (flags == MCI_NOTIFY_SUCCESSFUL)
	{
		OnStop();
		if (Loop)
		{
			OnPlay();
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// zastaven� p�ehr�v�n� (m��e b�t vol�no i zvenku p�i neaktivn�m editoru)

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

// uzav�en� p�ehr�vac�ho za��zen�
		if (DeviceID)
		{
			::mciSendCommand(DeviceID, MCI_STOP, 0, NULL);
	        ::mciSendCommand(DeviceID, MCI_CLOSE, 0, NULL);
			DeviceID = 0;
		}

// zru�en� souboru
		if (FileName[0])
		{
			::DeleteFile(FileName);
			FileName[0] = 0;
		}

// zobrazen� d�lky skladby
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
	if (Play) return;

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
		::mciSendCommand(DeviceID, MCI_PAUSE, 0, NULL);
	}
	else
	{
	    MCI_PLAY_PARMS mpp;
		mpp.dwCallback = (DWORD)MainFrame;
		::mciSendCommand(DeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD) &mpp);
	}

// aktualizace voleb
	UpdateMenu();
}


/////////////////////////////////////////////////////////////////////////////
// p�evinut� zp�t

void OnRew()
{
// kontrola, zda je p�ehr�v�n�
	if (!Play) return;

// �schova p��znaku pauzy
	BOOL pause = Pause;

// odpauzov�n�, je-li zapauzov�no
	if (pause) OnPause();

// p��prava nov� pozice
	MCI_PLAY_PARMS mpp;
	mpp.dwCallback = (DWORD)MainFrame;
	mpp.dwFrom = LastTime * 1000 - 10000;

// p�ete�en� hranice
	while ((int)mpp.dwFrom < 0) 
	{
		mpp.dwFrom += Delka;
	}

// nastaven� nov� pozice p�ehr�v�n�
	::mciSendCommand(DeviceID, MCI_PLAY, MCI_NOTIFY | MCI_FROM, 
    (DWORD) &mpp);

// navr�cen� pauzy
	if (pause) OnPause();
}

/////////////////////////////////////////////////////////////////////////////
// p�evinut� vp�ed

void OnFrw()
{
// kontrola, zda je p�ehr�v�n�
	if (!Play) return;

// �schova p��znaku pauzy
	BOOL pause = Pause;

// odpauzov�n�, je-li zapauzov�no
	if (pause) OnPause();

// p��prava nov� pozice
	MCI_PLAY_PARMS mpp;
	mpp.dwCallback = (DWORD)MainFrame;
	mpp.dwFrom = LastTime * 1000 + 10000;

// p�ete�en� hranice
	while ((int)mpp.dwFrom >= Delka) 
	{
		mpp.dwFrom -= Delka;
	}

// nastaven� nov� pozice p�ehr�v�n�
	::mciSendCommand(DeviceID, MCI_PLAY, MCI_NOTIFY | MCI_FROM, 
    (DWORD) &mpp);

// navr�cen� pauzy
	if (pause) OnPause();
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace voleb

void UpdateMenu()
{
	if (ProgMode && (FocusBuf == BufEdiID) && (EditMode == BufMusID))
	{
		CheckCommand(IDN_PLAY, Play && !Pause);
		CheckCommand(IDN_LOOP, Loop);
		CheckCommand(IDN_PAUSE, Pause);
		EnableCommand(IDN_PLAY, TRUE);
		EnableCommand(IDN_STOP, Play);
		EnableCommand(IDN_LOOP, !Play);
		EnableCommand(IDN_PAUSE, Play);
		EnableCommand(IDN_REW, Play);
		EnableCommand(IDN_FRW, Play);
	}
}


/////////////////////////////////////////////////////////////////////////////
// kopie do bufferu

void Copy()
{
	/*
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
	HGLOBAL global = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, Data->Size);
	if (global == NULL)
	{
		EndWaitCursor();
		::CloseClipboard();
		return;
	}

// uzamknut� bufferu
	BYTE* data = (BYTE*) ::GlobalLock(global);

// p�enesen� dat
	MemCopy(data, Data->Data, Data->Size);

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
	*/
}


/////////////////////////////////////////////////////////////////////////////
// navr�cen� z bufferu

void Paste()
{
	/*
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
			if (samples < 0) samples = 22050;

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
					Data->Channels = channels;
					Data->Bits = bits;
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
	*/
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace voleb bloku

void UpdateClipboard()
{
	if ((FocusBuf == BufEdiID) && (EditMode == BufMusID))
	{
		EnableCommand(IDN_CUT, FALSE);
		EnableCommand(IDN_COPY, FALSE); //Data->Size > 1);
		EnableCommand(IDN_PASTE, FALSE); //::IsClipboardFormatAvailable(CF_WAVE));
		EnableCommand(IDN_CLEAR, FALSE);
		EnableCommand(IDN_ALL, FALSE);
	}
}


}
