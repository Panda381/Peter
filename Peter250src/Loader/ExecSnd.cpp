
#include "Main.h"

/***************************************************************************\
*																			*
*						Prov�d�n� programu - zvuk							*
*																			*
\***************************************************************************/

#pragma optimize("t", on)			// optimalizace na maxim�ln� rychlost

// obsluha p�ehr�v�n� zvuku
HWAVEOUT	WaveOut = NULL;					// handle v�stupn�ho za��zen� (NULL=nen� otev�eno)
WAVEHDR		WaveHeader[SOUNDBUFFERU];		// popisova�e dat
BYTE*		SoundBuf[SOUNDBUFFERU];			// zvukov� buffery
int			SoundKanalu = 0;				// po�et aktivn�ch zvukov�ch kan�l�
SOUNDCHAN*	SoundChan;						// buffer zvukov�ch kan�l�
int			SoundError = -1;				// povolen� opakovan�ho otev�en� zvuku (p�i < 0)
bool		WaveFormatPCM = true;			// je p�ehr�v�n form�t PCM (vlastn� obsluha)
char*		WaveData2 = NULL;				// ukazatel dat pro nestandardn� form�t
int			WaveSize2 = 0;					// ��ta� dat pro nestandardn� form�t
int			WaveLoop = 1;					// ��ta� opakov�n� pro nestandardn� form�t
CSound		Sound2;							// p�ehr�van� nestandardn� zvuk, naposledy zadan� zvuk
double		WaveBeg2 = 0;					// posunut� offset pro nestandardn� form�t (sekund)
bool		SoundPausing = false;			// p��znak pauzy zvuku
int			SoundBlokSize = 1024;			// zarovn�v�n� velikosti bloku nestandardn�ho form�tu
int			BytesPerSec = 22050;			// p�enosov� rychlost pro nestandardn� form�t
int			SoundBufSize = SOUNDBUFSIZE;	// velikost bufferu nestandardn�ho form�tu
WORD		SoundID = 0;					// rozli�ovac� k�d zvuku

// obsluha DirectSound
#define		DSSOUNDOUTRATE 22050			// v�stupn� vzorkovac� kmito�et pro DirectSound

// ----- p�i testov�n� bez pou�it� DirectSound nastavit InitDSound na TRUE ----

BOOL		InitDSound = FALSE;				// prob�hla inicializace DirectSound
BOOL		DSoundOK = FALSE;				// DirectSound �sp�n� inicializov�no

#define VOLUMESTEPS 1001					// po�et krok� tabulky hlasitosti
short*		VolumeTab;						// tabulka hlasitosti (konverze na decibely)

DIRECTSOUNDCREATE	pDirectSoundCreate = NULL;	// funkce DirectSoundCreate (NULL=nen�)
HINSTANCE	DSoundDLL = NULL;				// knihovna DSOUND.DLL
IDirectSound* DirectSound = NULL;			// DirectSound interface
int			DSoundFlags = DSBCAPS_STICKYFOCUS |
							DSBCAPS_GLOBALFOCUS |
							DSBCAPS_GETCURRENTPOSITION2 |
							DSBCAPS_CTRLFREQUENCY |
							DSBCAPS_CTRLPAN |
							DSBCAPS_CTRLVOLUME;

/***************************************************************************\
*																			*
*								obsluha zvuku								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// deinicializace DirectSound

void TermDirectSound()
{

// uvoln�n� rozhran� DirectSound
	if (DirectSound != NULL)
	{
		DirectSound->Release();
		DirectSound = NULL;
	}

// uvoln�n� knihovny DSOUND.DLL
	if (DSoundDLL != NULL)
	{
		::FreeLibrary(DSoundDLL);
		DSoundDLL = NULL;
	}

// zru�en� tabulky hlasitosti
	if (VolumeTab != NULL)
	{
		MemFree(VolumeTab);
		VolumeTab = NULL;
	}

// p��znak neplatnosti DirectSound
	DSoundOK = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// inicializace DirectSound (vrac� TRUE = DirectSound je OK)

BOOL InitDirectSound()
{
// test, zda ji� prob�hala inicializce
	if (!InitDSound)
	{
		InitDSound = TRUE;

// na�ten� knihovny DSOUND.DLL
		DSoundDLL = ::LoadLibrary("DSOUND.DLL");
		if (DSoundDLL != NULL)
		{

// importov�n� funkce DirectSoundCreate
			pDirectSoundCreate = (DIRECTSOUNDCREATE)::GetProcAddress(DSoundDLL, "DirectSoundCreate");
			if (pDirectSoundCreate != NULL)
			{

// vytvo�en� rozhran� DirectSound
				if ((pDirectSoundCreate(NULL, &DirectSound, NULL) == DS_OK) && (DirectSound != NULL))
				{

// nastaven� m�du spolupr�ce DirectSound
					if (DirectSound->SetCooperativeLevel(MainFrame, DSSCL_PRIORITY) == DS_OK)
					{

// popisova� prim�rn�ho bufferu
						DSBUFFERDESC dsbd;
						MemFill(&dsbd, sizeof(DSBUFFERDESC), 0);
						dsbd.dwSize = sizeof(DSBUFFERDESC);
						dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_STICKYFOCUS;

// vytvo�en� prim�rn�ho bufferu
						IDirectSoundBuffer* prim = NULL;

						HRESULT res = DirectSound->CreateSoundBuffer(&dsbd, &prim, NULL);

						if ((res != DS_OK) || (prim == NULL))
						{
							dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER;
							res = DirectSound->CreateSoundBuffer(&dsbd, &prim, NULL);
						}

						if ((res == DS_OK) && (prim != NULL))
						{

// form�t prim�rn�ho bufferu
							WAVEFORMATEX wfx;
							wfx.wFormatTag = WAVE_FORMAT_PCM;
							wfx.nChannels = 2;
							wfx.nSamplesPerSec = DSSOUNDOUTRATE;
							wfx.nAvgBytesPerSec = 4 * DSSOUNDOUTRATE;
							wfx.nBlockAlign = 4;
							wfx.wBitsPerSample = 16;
							wfx.cbSize = 0;

// nastaven� form�tu prim�rn�ho bufferu
							res = prim->SetFormat(&wfx);
							prim->Release();

							if (res == DS_OK)
							{

// pokus o vytvo�en� sekund�rn�ho bufferu
								wfx.wFormatTag = WAVE_FORMAT_PCM;
								wfx.nChannels = 2;
								wfx.nSamplesPerSec = DSSOUNDOUTRATE;
								wfx.nBlockAlign = 4;
								wfx.nAvgBytesPerSec = DSSOUNDOUTRATE*4;
								wfx.wBitsPerSample = 16;
								wfx.cbSize = 0;

								DSBUFFERDESC dsbd;
								dsbd.dwSize = sizeof(DSBUFFERDESC);
								// je to streaming buffer aby z�stal v pam�ti kv�li mo�nosti duplikov�n�
								dsbd.dwFlags = DSoundFlags;
								dsbd.dwBufferBytes = 65000;
								dsbd.dwReserved = 0;
								dsbd.lpwfxFormat = &wfx;

								IDirectSoundBuffer* buf = NULL;
								if (DirectSound->CreateSoundBuffer(&dsbd, &(buf), NULL) != DS_OK) buf = NULL;

								if (buf == NULL)
								{
									DSoundFlags = DSBCAPS_GETCURRENTPOSITION2 |
													DSBCAPS_CTRLFREQUENCY |
													DSBCAPS_CTRLPAN |
													DSBCAPS_CTRLVOLUME;
									dsbd.dwFlags = DSoundFlags;
									if (DirectSound->CreateSoundBuffer(&dsbd, &(buf), NULL) != DS_OK) buf = NULL;

									if (buf == NULL)
									{
										DSoundFlags = DSBCAPS_CTRLFREQUENCY |
														DSBCAPS_CTRLVOLUME;
										dsbd.dwFlags = DSoundFlags;
										if (DirectSound->CreateSoundBuffer(&dsbd, &(buf), NULL) != DS_OK) buf = NULL;
									}
								}

								if (buf != NULL)
								{
									void* dst = NULL;
									DWORD n = 0;
									res = buf->Lock(0, 65000, &dst, &n, NULL, NULL, 0);
									if (res == DS_OK) buf->Unlock(dst, n, NULL, 0);

									buf->Release();

// vygenerov�n� tabulky hlasitosti
									if (res == DS_OK)
									{
										VolumeTab = (short*)MemGet(VOLUMESTEPS * sizeof(short));

										VolumeTab[0] = -10000;

										for (int i = 1; i < VOLUMESTEPS; i++)
										{
											VolumeTab[i] = (short)Round(2000 * log10((double)i/(VOLUMESTEPS-1)));
										}

										DSoundOK = TRUE;
										return TRUE;
									}
								}
							}
						}
					}
				}				
			}

// p�i chyb� uvoln�n� DirectSound
			TermDirectSound();
		}
	}

	return DSoundOK;
}


/////////////////////////////////////////////////////////////////////////////
// otev�en� zvukov�ho kan�lu (balance: -1=vlevo, 0=st�ed, +1=vpravo)

void PlaySoundOpen(CSound sound, int loop, bool stop, double volume, double balance, double speed)
{
// inicializace DirectSound
	InitDirectSound();

// kontrola velikosti dat
	if (sound.Size() <= 0) return;

// korekce po�tu opakov�n�
	if ((loop < 1) || (loop > 100000)) loop = BigInt;

// zastaven� p�ehr�v�n�, bude-li nebo byl-li jin� form�t ne� PCM
	if (!WaveFormatPCM || (sound.Format() != WAVE_FORMAT_PCM))
	{
		PlaySoundStop();
	}

// obsluha nestandardn�ho form�tu
	if (sound.Format() != WAVE_FORMAT_PCM)
	{

// povolen� otev�en� zvuku
		if (SoundError > 0) return;

// p��znak p�ehr�v�n� zvuku
		WaveFormatPCM = false;
		Sound2 = sound;

// p��prava zarovn�v�n� dat a velikosti bufferu
		BytesPerSec = sound.Samples() * sound.Channels() / 2;				// bajt� za sekundu pro ADPCM
		SoundBlokSize = 1024;											// velikost bloku pro ADPCM (asi tak n�jak)

		if ((sound.Format() >= 0x50) && (sound.Format() < 0x60))				// je MPEG ?
		{
			WAVEFORMATEX* wfx = (WAVEFORMATEX*)(sound.DataData());
			BytesPerSec = wfx->nAvgBytesPerSec;									// bajt� za sekundu pro MPEG
			SoundBlokSize = ((MPEGLAYER3WAVEFORMAT*)(sound.DataData()))->nBlockSize;	// velikost bloku pro MPEG
		}
		if (BytesPerSec <= 2) BytesPerSec = 2;
		if (SoundBlokSize < 2) SoundBlokSize = 2;
		SoundBufSize = (SOUNDBUFSIZE + SoundBlokSize-1) / SoundBlokSize * SoundBlokSize;

// otev�en� p�ehr�vac�ho za��zen�
		MMRESULT res = ::waveOutOpen(
			&WaveOut,								// handle za��zen�
			WAVE_MAPPER,							// vybrat nejvhodn�j�� za��zen�
			(WAVEFORMATEX*)(sound.DataData()),		// popisova� form�tu
			NULL,									// nen� funkce CALLBACK
			NULL,									// data CALLBACK
			CALLBACK_NULL);							// nen� CALLBACK

		if (res != MMSYSERR_NOERROR)
		{
			WaveOut = NULL;
			SoundError = SOUNDERRORWAIT;
			return;
		}

// p��prava ukazatel� dat
		WaveBeg2 = 0;
		WaveLoop = loop;
		WaveData2 = (char*)sound.DataData() + sound.SizeHead();	// adresa dat
		WaveSize2 = sound.Size() - sound.SizeHead();
		if (WaveSize2 < 0)
		{
			WaveSize2 = 0;
			WaveLoop = 0;
		}
		return;
	}

// bude form�t PCM
	WaveFormatPCM = true;
	Sound2 = sound;

// odpauzov�n�
	SoundPause(false);

// p��prava balance
	if (balance < -1) balance = -1;
	if (balance > 1) balance = 1;
	
	double left = 1;
	double right = 1;
	if (balance > 0)
	{
		left = 1 - balance;
	}
	else
	{
		right = 1 + balance;
	}

// p��prava hlasitosti
	volume = fabs(volume);
	if (volume > 1) volume = 1;
	left *= volume;
	right *= volume;

// rychlost p�ehr�v�n� v bajtech na vzorek
	speed = fabs(speed);
	double speedB = speed * sound.DataRate() / SOUNDOUTRATE;
	if (speedB >= sound.Size()) speedB = sound.Size();

// parametry pro DirectSound
	int dsfreq = Round(speed * DSSOUNDOUTRATE);
	if (dsfreq < DSBFREQUENCY_MIN) dsfreq = DSBFREQUENCY_MIN;
	if (dsfreq > DSBFREQUENCY_MAX) dsfreq = DSBFREQUENCY_MAX;

	short dsvolume = 0;
	short dsbalance = 0;

	if (DSoundOK)
	{
		dsvolume = VolumeTab[Round(volume * (VOLUMESTEPS-1))];

		if (balance > 0)
		{
			dsbalance = (short)-VolumeTab[Round((1-balance) * (VOLUMESTEPS-1))];
		}
		else
		{
			dsbalance = VolumeTab[Round((1+balance) * (VOLUMESTEPS-1))];
		}
	}

// p��prava prom�nn�ch k prohled�n� tabulky kan�l�
	int i = MAXSOUNDKANALU;			// ��ta� testovan�ch kan�l�
	int cit = SoundKanalu;			// ��ta� zbyl�ch aktivn�ch kan�l�
	int min = BigInt;				// velikost dat voln�ho kan�lu
	int index = 0;					// index nalezen�ho voln�ho kan�lu
	SOUNDCHAN* sndchan = SoundChan;	// ukazatel kan�l�

// nalezen� zvuku, zda je ji� p�ehr�v�n, nalezen� nejstar��ho zvuku
	for (; (i > 0) && (cit > 0); i--)
	{
		if (sndchan->Num > 0)
		{
			cit--;

			if ((sndchan->Sound == sound) && (sndchan->SoundID == SoundID))
			{

				IDirectSoundBuffer* buf = sndchan->DSBuffer;

// p�i zastaven� p�ehr�v�n� zvuku inicializace od za��tku zvuku
				if (stop)
				{
					sndchan->Offset = 0;
					if (buf != NULL) buf->SetCurrentPosition(0);
				}

// nastaven� parametr� pro DirectSound
				if (buf != NULL)
				{
					if (dsfreq != sndchan->DSFreq)
					{
						buf->SetFrequency(dsfreq);
					}

					if (dsvolume != sndchan->DSVolume)
					{
						buf->SetVolume(dsvolume);
					}

					if (dsbalance != sndchan->DSBalance)
					{
						buf->SetPan(dsbalance);
					}

// zah�jen� p�ehr�v�n�, pokud neprob�h�
					DWORD stat = 0;
					buf->GetStatus(&stat);

					if ((stat & DSBSTATUS_PLAYING) == 0)
					{
						buf->Play(0, 0, (loop == BigInt) ? DSBPLAY_LOOPING : 0);
					}
				}

// nastaven� nov�ch parametr� zvuku
				sndchan->Left = left;
				sndchan->Right = right;
				sndchan->SpeedB = speedB;
				sndchan->Loop = loop;
				sndchan->DSFreq = dsfreq;
				sndchan->DSVolume = dsvolume;
				sndchan->DSBalance = dsbalance;

				return;
			}
		}

// nalezen� voln�ho kan�lu (s nejmen��mi zbyl�mi daty)
		if (sndchan->Num == 0)
		{
			if (min > 0)
			{
				index = MAXSOUNDKANALU-i;
				min = 0;
			}
		}
		else
		{
			int nn = sndchan->Num - Round(sndchan->Offset);

			if (nn < min)
			{
				index = MAXSOUNDKANALU-i;
				min = nn;
			}
		}

// zv��en� ukazatele v bufferu
		sndchan++;
	}

// korekce voln�ho kan�lu, je-li k dispozici n�jak� nepou�it� kan�l
	if ((i > 0) && (SoundChan[index].Num > 0)) index = MAXSOUNDKANALU-i;

// zv��en� ��ta�e kan�l�, pokud byl kan�l voln�
	sndchan = SoundChan + index;
	if (sndchan->Num == 0) SoundKanalu++;

// zru�en� bufferu DirectSound
	if (sndchan->DSBuffer != NULL)
	{
		sndchan->DSBuffer->Release();
		sndchan->DSBuffer = NULL;
	}

// nastaven� ukazatel� pro nov� kan�l
	sndchan->Num = sound.Size();
	sndchan->Data = sound.DataData();
	sndchan->Left = left;
	sndchan->Right = right;
	sndchan->Offset = 0;
	sndchan->SpeedB = speedB;
	sndchan->Loop = loop;
	sndchan->Sound = sound;
	sndchan->DSFreq = dsfreq;
	sndchan->DSVolume = dsvolume;
	sndchan->DSBalance = dsbalance;
	sndchan->SoundID = SoundID;

// v�stup pomoc� DirectSound
	if (DSoundOK)
	{

// zru�en� vzoru bufferu je-li ztracen
		SOUNDDATA* sd = sound.Data();
		if (sd->DSBuffer != NULL)
		{
			DWORD stat = 0;
			sd->DSBuffer->GetStatus(&stat);

			if ((stat & DSBSTATUS_BUFFERLOST) != 0)
			{
				sd->DSBuffer->Release();
				sd->DSBuffer = NULL;
			}
		}

// test, zda existuje vzor zvuku
		if (sd->DSBuffer == NULL)
		{

// p��prava standardn�ho form�tu zvuku
			CSound snd = sound;
			snd.DeComp();
			snd.Conv16Bit();
			snd.ConvStereo();
			snd.ConvRate(DSSOUNDOUTRATE);

// vytvo�en� bufferu DirectSound
			WAVEFORMATEX wfx;
			wfx.wFormatTag = WAVE_FORMAT_PCM;
			wfx.nChannels = 2;
			wfx.nSamplesPerSec = DSSOUNDOUTRATE;
			wfx.nBlockAlign = 4;
			wfx.nAvgBytesPerSec = DSSOUNDOUTRATE*4;
			wfx.wBitsPerSample = 16;
			wfx.cbSize = 0;

			DSBUFFERDESC dsbd;
			dsbd.dwSize = sizeof(DSBUFFERDESC);
			// je to streaming buffer aby z�stal v pam�ti kv�li mo�nosti duplikov�n�
			dsbd.dwFlags = DSoundFlags;
			dsbd.dwBufferBytes = snd.Size();
			dsbd.dwReserved = 0;
			dsbd.lpwfxFormat = &wfx;

			IDirectSoundBuffer* buf = NULL;
			if (DirectSound->CreateSoundBuffer(&dsbd, &(buf), NULL) != DS_OK) buf = NULL;

// p�enesen� dat do bufferu
			if (buf != NULL)
			{
				void* dst = NULL;
				DWORD n = 0;

				if (buf->Lock(0, snd.Size(), &dst, &n, NULL, NULL, 0) == DS_OK)
				{
					if ((int)n < 0) n = 0;
					if ((int)n > snd.Size()) n = snd.Size();

					MemCopy(dst, snd.DataData(), n);

					buf->Unlock(dst, n, NULL, 0);
				}
				else
				{
					buf->Release();
					buf = NULL;
				}
			}

			sd->DSBuffer = buf;
		}

// vytvo�en� kopie bufferu (p�i ne�sp�chu se pou�ije origin�l, nap�. je-li HW buffer)
		if (sd->DSBuffer != NULL)
		{
			if (DirectSound->DuplicateSoundBuffer(sd->DSBuffer, &(sndchan->DSBuffer)) != DS_OK)
			{
				sndchan->DSBuffer = sd->DSBuffer;
				sd->DSBuffer = NULL;
			}

// nastaven� parametr� zvuku
			IDirectSoundBuffer* buf = sndchan->DSBuffer;

			buf->SetFrequency(dsfreq);
			buf->SetVolume(dsvolume);
			buf->SetPan(dsbalance);

// zah�jen� p�ehr�v�n� zvuku
			buf->SetCurrentPosition(0);
			buf->Play(0, 0, (loop == BigInt) ? DSBPLAY_LOOPING : 0);
			return;
		}
	}

// otev�en� v�stupn�ho za��zen�
	if (WaveOut == NULL)
	{

// povolen� otev�en� zvuku
		if (SoundError > 0) return;

// vynulov�n� buffer�
		for (i = SOUNDBUFFERU-1; i >= 0; i--)
		{
			WaveHeader[i].dwFlags = WHDR_DONE;
		}

// p��prava struktury popisova�e form�tu
		WAVEFORMATEX wf;							// popisova� form�tu
		wf.wFormatTag = WAVE_FORMAT_PCM;			// form�t dat
		wf.nChannels = 2;							// po�et kan�l�
		wf.nSamplesPerSec = SOUNDOUTRATE;			// vzorkovac� kmito�et
		wf.nAvgBytesPerSec = SOUNDOUTRATE*4;		// p�enosov� rychlost dat
		wf.nBlockAlign = 4;							// zarovn�v�n� dat
		wf.wBitsPerSample = 16;						// po�et bit� na vzorek
		wf.cbSize = 0;								// dopl�kov� data ve struktu�e

// otev�en� p�ehr�vac�ho za��zen�
		MMRESULT res = ::waveOutOpen(
			&WaveOut,								// handle za��zen�
			WAVE_MAPPER,							// vybrat nejvhodn�j�� za��zen�
			&wf,									// popisova� form�tu
			NULL,									// nen� funkce CALLBACK
			NULL,									// data CALLBACK
			CALLBACK_NULL);							// nen� CALLBACK

		if (res != MMSYSERR_NOERROR)
		{
			sndchan->Num = 0;
			WaveOut = NULL;
			SoundKanalu = 0;
			SoundError = SOUNDERRORWAIT;
			return;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// uzav�en� zvukov�ho kan�lu

void PlaySoundClose(CSound sound)
{
// nestandardn� zvuk
	if (!WaveFormatPCM)
	{
		if (Sound2 == sound)
		{
			PlaySoundStop();
		}
		return;
	}

// p��prava prom�nn�ch k prohled�n� tabulky kan�l�
	int i = MAXSOUNDKANALU;			// ��ta� testovan�ch kan�l�
	int cit = SoundKanalu;			// ��ta� zbyl�ch aktivn�ch kan�l�
	SOUNDCHAN* sndchan = SoundChan;	// ukazatel kan�l�

// nalezen� zvuku
	for (; (i > 0) && (cit > 0); i--)
	{
		if (sndchan->Num > 0)
		{
			cit--;

			if ((sndchan->Sound == sound) && (sndchan->SoundID == SoundID))
			{

// zru�en� zvuku v kan�lu
				sndchan->Num = 0;
				sndchan->Sound.Empty();
				SoundKanalu--;

				IDirectSoundBuffer* buf = sndchan->DSBuffer;
				if (buf != NULL)
				{
					buf->Stop();
					buf->Release();
					sndchan->DSBuffer = NULL;
				}
				return;
			}
		}

// zv��en� ukazatele v bufferu
		sndchan++;
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� pozice p�ehr�v�n� zvuku (v sekund�ch)

void SetSoundPos(double pos)
{
// v pauze se pozice nenastavuje
	if (SoundPausing) return;

// korekce pozice
	if (pos < 0) pos = 0;

// pozice zvuku v nestandardn�m form�tu
	if (!WaveFormatPCM)
	{

// zvuk nen� p�ehr�v�n
		if (WaveOut == NULL) return;

// obsluhov�ny jen n�kter� form�ty
		if ((Sound2.Format() == WAVE_FORMAT_ADPCM) ||
			(Sound2.Format() == WAVE_FORMAT_MPEGLAYER3))
		{
			double len = (double)(Sound2.Size() - Sound2.SizeHead()) / BytesPerSec;		// d�lka zvuku celkem (v sekund�ch)
			if (pos > len) pos = len;												// omezen� maxim�ln� pozice

			int n = Round(BytesPerSec * pos + SoundBlokSize/2) / SoundBlokSize * SoundBlokSize;		// nov� offset zarovnan� na blok
			if (n < 0) n = 0;
			if (n > (Sound2.Size() - Sound2.SizeHead())) n = Sound2.Size() - Sound2.SizeHead();	// omezen� offsetu
	
			WaveData2 = (char*)Sound2.DataData() + Sound2.SizeHead() + n;			// nov� adresa dat
			WaveSize2 = Sound2.Size() - Sound2.SizeHead() - n;						// nov� velikost dat
			WaveBeg2 = (double)n / BytesPerSec;										// fiktivn� offset za��tku skladby

			if (WaveSize2 < 0)
			{
				WaveSize2 = 0;
				WaveLoop = 0;
			}

			::waveOutReset(WaveOut);												// zastaven� p�ehr�v�n�

			for (int i = SOUNDBUFFERU-1; i >= 0; i--)
			{
				if (WaveHeader[i].dwFlags & WHDR_PREPARED)
				{
					waveOutUnprepareHeader(
						WaveOut,
						WaveHeader + i,
						sizeof(WAVEHDR));
				}
				WaveHeader[i].dwFlags = WHDR_DONE;
			}

			::waveOutClose(WaveOut);										// uzav�en� v�stupn�ho za��zen�

			if (::waveOutOpen(
					&WaveOut,								// handle za��zen�
					WAVE_MAPPER,							// vybrat nejvhodn�j�� za��zen�
					(WAVEFORMATEX*)(Sound2.DataData()),		// popisova� form�tu
					NULL,									// nen� funkce CALLBACK
					NULL,									// data CALLBACK
					CALLBACK_NULL) != MMSYSERR_NOERROR)		// nen� CALLBACK
			{
				WaveOut = NULL;
			}
		}
		return;
	}

// pozice zvuku ve standardn�m form�tu
	int i = MAXSOUNDKANALU;			// ��ta� testovan�ch kan�l�
	int cit = SoundKanalu;			// ��ta� zbyl�ch aktivn�ch kan�l�
	SOUNDCHAN* sndchan = SoundChan;	// ukazatel kan�l�

	for (; (i > 0) && (cit > 0); i--)
	{
		if (sndchan->Num > 0)
		{
			cit--;
			if ((sndchan->Sound == Sound2) && (sndchan->SoundID == SoundID))
			{
				double len = (double)(Sound2.Size()) / Sound2.DataRate();
				if (pos > len) pos = len;
				int n = Round(Sound2.DataRate() * pos);
				if (n > Sound2.Size()) n = Sound2.Size();
				sndchan->Offset = (n & ~(Sound2.Align()-1));

				if (sndchan->DSBuffer != NULL)
				{
					n = Round(4 * DSSOUNDOUTRATE * pos) & ~3;
					sndchan->DSBuffer->SetCurrentPosition(n);
				}
				return;
			}
		}
		sndchan++;
	}
	return;
}


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� pozice p�ehr�v�n� zvuku

double GetSoundPos()
{
// pozice zvuku v nestandardn�m form�tu
	if (!WaveFormatPCM)
	{
		if (WaveOut == NULL) return 0;

		MMTIME tm;
		tm.wType = TIME_SAMPLES;
		tm.u.sample = 0;
		::waveOutGetPosition(WaveOut, &tm, sizeof(MMTIME));
		return ((double)tm.u.sample / Sound2.Samples() ) + WaveBeg2;
	}

// obsluha naposledy zadan�ho zvuku
	int i = MAXSOUNDKANALU;			// ��ta� testovan�ch kan�l�
	int cit = SoundKanalu;			// ��ta� zbyl�ch aktivn�ch kan�l�
	SOUNDCHAN* sndchan = SoundChan;	// ukazatel kan�l�

	for (; (i > 0) && (cit > 0); i--)
	{
		if (sndchan->Num > 0)
		{
			cit--;
			if ((sndchan->Sound == Sound2) && (sndchan->SoundID == SoundID))
			{
				if (sndchan->DSBuffer != NULL)
				{
					DWORD n = 0;
					sndchan->DSBuffer->GetCurrentPosition(&n, NULL);
					return ((double)(int)n / (DSSOUNDOUTRATE * 4));
				}

				int n = Round(sndchan->Offset);
				if (n < 0) n = 0;
				if (n > Sound2.Size()) n = Sound2.Size();
				return ((double)n / Sound2.DataRate());
			}
		}
		sndchan++;
	}
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// zastaven� p�ehr�v�n� zvuk�

void PlaySoundStop()
{
// je v�stupn� za�izen� otev�eno?
	if (WaveOut != NULL)
	{

// p�eru�en� prob�haj�c�ho p�ehr�v�n�
		::waveOutReset(WaveOut);

// uvoln�n� buffer�
		for (int i = SOUNDBUFFERU-1; i >= 0; i--)
		{
			if (WaveHeader[i].dwFlags & WHDR_PREPARED)
			{
				waveOutUnprepareHeader(
					WaveOut,
					WaveHeader + i,
					sizeof(WAVEHDR));
			}
			WaveHeader[i].dwFlags = WHDR_DONE;
		}

// uzav�en� p�ehr�vac�ho za�izen�
		::waveOutClose(WaveOut);
		WaveOut = NULL;
	}

// uvoln�n� v�ech kan�l�
	SOUNDCHAN* sndchan = SoundChan;	// ukazatel kan�l�
	for (int i = MAXSOUNDKANALU; i > 0; i--)
	{
		sndchan->Num = 0;
		sndchan->Sound.Empty();

		if (sndchan->DSBuffer != NULL)
		{
			sndchan->DSBuffer->Stop();
			sndchan->DSBuffer->Release();
			sndchan->DSBuffer = NULL;
		}

		sndchan++;
	}
	Sound2.Empty();
	SoundKanalu = 0;
	WaveSize2 = 0;
	WaveLoop = 0;
	SoundPausing = false;
}


/////////////////////////////////////////////////////////////////////////////
// pauza p�ehr�v�n� zvuku

void SoundPause(bool pause)
{
	if ((WaveOut != NULL) && (pause != SoundPausing))
	{
		SoundPausing = pause;

		if (pause)
		{
			::waveOutPause(WaveOut);
		}
		else
		{
			::waveOutRestart(WaveOut);
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// z�pis zvuku do bufferu (vrac� po�et zapsan�ch vzork�, tj. v n�sobc�ch 4 bajt�)

int _fastcall SoundWriteBuf(SOUNDCHAN* snd, short* dst, int num)
{
#define MAXDATA 0x7fff

// ��ta� ulo�en�ch vzork�
	int result = 0;

// velikost bufferu
	int n = snd->Num;

// adresa bufferu
	BYTE* src = snd->Data;

// hlasitost kan�l�
	double left = snd->Left;
	double right = snd->Right;

// offset dat
	double offset = snd->Offset;

// rychlost dat
	double speedB = snd->SpeedB;

// p��znak typu zvuku
	int typ = 0;

#define SOUND8BIT	0
#define SOUND16BIT	1
	if (snd->Sound.Bits() == 16) typ |= SOUND16BIT;
	
#define SOUNDMONO	0
#define SOUNDSTEREO	2
	if (snd->Sound.Stereo()) typ |= SOUNDSTEREO;

// rozli�en� typu zvuku
	switch (typ)
	{
	case SOUND8BIT | SOUNDMONO:
		for (; num > 0; num--)
		{
			int off = Round(offset);

			if ((DWORD)off < (DWORD)n)
			{
				int data0 = (short)(char)(src[off] - 128) * 256;

				int data = Round(data0 * left) + dst[0];
				if (data > MAXDATA) data = MAXDATA;
				if (data < -MAXDATA) data = -MAXDATA;
				dst[0] = (short)data;

				data = Round(data0 * right) + dst[1];
				if (data > MAXDATA) data = MAXDATA;
				if (data < -MAXDATA) data = -MAXDATA;
				dst[1] = (short)data;

			}
			else
			{
				break;
			}

			dst++;
			dst++;

			result++;
			offset += speedB;
		}
		break;

	case SOUND8BIT | SOUNDSTEREO:
		for (; num > 0; num--)
		{
			int leftoff = Round(offset) & ~1;
			if ((DWORD)leftoff < (DWORD)n)
			{
				int data = Round(((short)(char)(src[leftoff] - 128)
							* 256) * left) + *dst;
				if (data > MAXDATA) data = MAXDATA;
				if (data < -MAXDATA) data = -MAXDATA;
				*dst = (short)data;
			}
			dst++;

			int rightoff = leftoff | 1;
			if ((DWORD)rightoff < (DWORD)n)
			{
				int data = Round(((short)(char)(src[rightoff] - 128)
							* 256) * right) + *dst;
				if (data > MAXDATA) data = MAXDATA;
				if (data < -MAXDATA) data = -MAXDATA;
				*dst = (short)data;
			}
			else
			{
				if ((DWORD)leftoff >= (DWORD)n) break;
			}
			dst++;

			result++;
			offset += speedB;
		}
		break;

	case SOUND16BIT | SOUNDMONO:
		n--; if (n <= 0) break;

		for (; num > 0; num--)
		{
			int off = Round(offset) & ~1;
			if ((DWORD)off < (DWORD)n)
			{
				int data = Round(*(short*)(src + off) * left) + dst[0];
				if (data > MAXDATA) data = MAXDATA;
				if (data < -MAXDATA) data = -MAXDATA;
				dst[0] = (short)data;

				data = Round(*(short*)(src + off) * right) + dst[1];
				if (data > MAXDATA) data = MAXDATA;
				if (data < -MAXDATA) data = -MAXDATA;
				dst[1] = (short)data;
			}
			else
			{
				break;
			}
			dst++;
			dst++;

			result++;
			offset += speedB;
		}
		break;

	case SOUND16BIT | SOUNDSTEREO:
		n--; if (n <= 0) break;

		for (; num > 0; num--)
		{
			int leftoff = Round(offset) & ~3;
			if ((DWORD)leftoff < (DWORD)n)
			{
				int data = Round(*(short*)(src + leftoff) * left) + *dst;
				if (data > MAXDATA) data = MAXDATA;
				if (data < -MAXDATA) data = -MAXDATA;
				*dst = (short)data;
			}
			dst++;

			int rightoff = leftoff + 2;
			if ((DWORD)rightoff < (DWORD)n)
			{
				int data = Round(*(short*)(src + rightoff) * right) + *dst;
				if (data > MAXDATA) data = MAXDATA;
				if (data < -MAXDATA) data = -MAXDATA;
				*dst = (short)data;
			}
			else
			{
				if ((DWORD)leftoff >= (DWORD)n) break;
			}
			dst++;

			result++;
			offset += speedB;
		}
		break;

	default:
		ASSERTERROR;
	}

	snd->Offset = offset;
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// obsluha p�ehr�v�n� zvuku na pozad�

void PlaySoundBack()
{
// ��ta� povolen� otev�en� zvuku
	SoundError--;

	if (SoundPausing) return;

// obsluha nestandardn�ho form�tu
	if (!WaveFormatPCM)
	{

// test, zda prob�h� zvukov� v�stup
		if (WaveOut != NULL)
		{

// test, zda je ukon�eno p�ehr�v�n�
			BOOL stop = ((WaveSize2 <= 0) && (WaveLoop <= 0));
			int i;
			for (i = SOUNDBUFFERU-1; i >= 0; i--)
			{
				if ((WaveHeader[i].dwFlags & WHDR_DONE) == 0)
				{
					stop = FALSE;
					break;
				}
			}

// ukon�en� p�ehr�v�n�
			if (stop)
			{
				PlaySoundStop();
				return;
			}
		
// obsluha buffer�
			for (i = SOUNDBUFFERU-1; ((i >= 0) && ((WaveSize2 > 0) || (WaveLoop > 0))); i--)
			{
				WAVEHDR* wh = &WaveHeader[i];
				if (wh->dwFlags & WHDR_DONE)
				{

// uvoln�n� bufferu pro z�pis
					if (wh->dwFlags & WHDR_PREPARED)
					{
						::waveOutUnprepareHeader(WaveOut, wh, sizeof(WAVEHDR));
					}

// p��prava popisova�e dat
					wh->lpData = WaveData2;						// adresa dat
					int size = WaveSize2;
					if (size > SoundBufSize) size = SoundBufSize;
					WaveSize2 -= size;
					WaveData2 += size;
					wh->dwBufferLength = size;				// velikost dat
					wh->dwBytesRecorded = size;				// velikost dat nahran�ch v bufferu
					wh->dwUser = 0;							// u�ivatelsk� data
					wh->dwFlags = WHDR_BEGINLOOP | WHDR_ENDLOOP; // parametry
					wh->dwLoops = 1;						// po�et opakov�n�
					wh->lpNext = NULL;						// nen� dal�� buffer
					wh->reserved = 0;

// p��prava bufferu k p�ehr�v�n�
					MMRESULT res = ::waveOutPrepareHeader(
						WaveOut,									// handle za��zen�
						wh,										// popisova� dat
						sizeof(WAVEHDR));							// velikost popisova�e dat

					if (res != MMSYSERR_NOERROR)
					{
						wh->dwFlags |= WHDR_DONE;			// p�i velikosti zbytku dat < 512 B to neprojde!!!!
					}
					else
					{

// p�ed�n� bufferu p�ehr�vac�mu za��zen�
						res = ::waveOutWrite(
							WaveOut,									// handle za��zen�
							wh,								// popisova� dat
							sizeof(WAVEHDR));							// velikost popisova�e dat

						if (res != MMSYSERR_NOERROR)
						{
							wh->dwFlags |= WHDR_DONE;
						}
					}

// obsluha opakov�n� zvuku
					if (WaveSize2 <= 0)
					{
						WaveLoop--;
						if (WaveLoop > 0)
						{
							WaveData2 = (char*)Sound2.DataData() + Sound2.SizeHead();			// nov� adresa dat
							WaveSize2 = Sound2.Size() - Sound2.SizeHead();						// nov� velikost dat
							WaveBeg2 = 0;										// fiktivn� offset za��tku skladby
						}
					}
				}
			}
		}
		return;
	}

// obsluha p�ehr�v�n� DirectSound
	if (DSoundOK)
	{
		int i = MAXSOUNDKANALU;			// ��ta� testovan�ch kan�l�
		int cit = SoundKanalu;			// ��ta� zbyl�ch aktivn�ch kan�l�
		SOUNDCHAN* sndchan = SoundChan;	// ukazatel kan�l�

		for (; (i > 0) && (cit > 0); i--)
		{
			if (sndchan->Num > 0)
			{
				cit--;

				IDirectSoundBuffer* buf = sndchan->DSBuffer;

				if (buf != NULL)
				{
					DWORD stat = 0;

					buf->GetStatus(&stat);
					if ((stat & DSBSTATUS_PLAYING) == 0)
					{
						if (((stat & DSBSTATUS_BUFFERLOST) == 0) && (sndchan->Loop > 1))
						{
							if (sndchan->Loop != BigInt)
							{
								sndchan->Loop--;
								buf->SetCurrentPosition(0);
								buf->Play(0, 0, 0);
							}
						}
						else
						{
							buf->Stop();

							SoundKanalu--;
							sndchan->Num = 0;
							buf->Release();
							sndchan->DSBuffer = NULL;
						}
					}
				}
			}

			sndchan++;
		}

		if (WaveOut == NULL) return;
	}

// test, zda jsou n�jak� aktivn� kan�ly (a p��p. uzav�en� v�stupu)
	if (SoundKanalu <= 0)
	{
		if (WaveOut != NULL)
		{
			for (int i = SOUNDBUFFERU-1; i >= 0; i--)
			{
				if ((WaveHeader[i].dwFlags & WHDR_DONE) == 0) return;
			}
			PlaySoundStop();
		}
		return;
	}

// nalezen� voln�ho bufferu
	WAVEHDR* wh = WaveHeader;
	for (int b = SOUNDBUFFERU; (b > 0) && (SoundKanalu > 0); b--)
	{
		if (wh->dwFlags & WHDR_DONE)
		{

// uvoln�n� bufferu pro z�pis
			if (wh->dwFlags & WHDR_PREPARED)
			{
				::waveOutUnprepareHeader(WaveOut, wh, sizeof(WAVEHDR));
			}

// adresa dat bufferu
			BYTE* buf = SoundBuf[SOUNDBUFFERU-b];

// p��prava struktury popisova�e dat
			wh->lpData = (char*)buf;			// adresa bufferu s daty
			wh->dwBufferLength = SOUNDBUFSIZE;	// velikost dat v bufferu
			wh->dwBytesRecorded = SOUNDBUFSIZE;	// po�et dat v bufferu
			wh->dwUser = 0;						// u�ivatelsk� data
			wh->dwFlags = 0;					// parametry
			wh->dwLoops = 0;					// po�et opakov�n�
			wh->lpNext = NULL;					// nen� dal�� buffer
			wh->reserved = 0;

// vymaz�n� bufferu
			MemFill(buf, SOUNDBUFSIZE, 0);

// p��prava prom�nn�ch k prohled�n� tabulky kan�l�
			int cit = SoundKanalu;			// ��ta� zbyl�ch aktivn�ch kan�l�
			int i = MAXSOUNDKANALU;			// ��ta� testovan�ch kan�l�
			SOUNDCHAN* sndchan = SoundChan;	// ukazatel kan�l�

// nalezen� aktivn�ho zvukov�ho kan�lu
			for (; (i > 0) && (cit > 0) && !Pause; i--)
			{
				if (sndchan->Num > 0)
				{
					cit--;

// z�pis dat do bufferu
					int num = SOUNDBUFSIZE/4;
					short* buf2 = (short*)buf;

					int n = SoundWriteBuf(sndchan, buf2, num);

					num -= n;
					buf2 += n*2;

// ukon�en� zvuku
					while ((DWORD)Round(sndchan->Offset) >= (DWORD)sndchan->Num)
					{
						sndchan->Loop--;

						if (sndchan->Loop > 0)
						{
							sndchan->Offset = 0;

							if (num > 0)
							{
								n = SoundWriteBuf(sndchan, buf2, num);
								num -= n;
								buf2 += n*2;
							}
							else
							{
								break;
							}
						}
						else
						{
							SoundKanalu--;
							sndchan->Sound.Empty();
							sndchan->Num = 0;

							IDirectSoundBuffer* buf = sndchan->DSBuffer;

							if (buf != NULL)
							{
								buf->Stop();
								buf->Release();
								sndchan->DSBuffer = NULL;
							}
							break;
						}
					}
				}

// zv��en� ukazatele v bufferu kan�l�
				sndchan++;
			}

// p��prava bufferu k p�ehr�v�n�
			waveOutPrepareHeader(WaveOut, wh, sizeof(WAVEHDR));

// p�ed�n� bufferu p�ehr�vac�mu za��zen�
			waveOutWrite(WaveOut, wh, sizeof(WAVEHDR));
		}

// adresa dal��ho bufferu
		wh++;
	}
}
	

/***************************************************************************\
*																			*
*									funkce									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// funkce s n�vratem zvuku

void _fastcall FSoundFunc(CSound& sound)
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
	sound = Sound[Sound.Num() - res];

// zru�en� lok�ln�ch prom�nn�ch
	FCommand();
}


/***************************************************************************\
*																			*
*								prom�nn�									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// pr�zdn� zvuk

void _fastcall FSoundEmpty(CSound& sound) { sound.Empty(); };


/////////////////////////////////////////////////////////////////////////////
// glob�ln� zvuk (Data = index)

void _fastcall FSoundObj(CSound& sound)
{
// index zvuku
	int inx = ExecItem[-1].Data;

// nastaven� zvuku
	sound = Sound[inx];
}


/////////////////////////////////////////////////////////////////////////////
// globaln� zvuk v seznamu (Data = index, List = seznam)

void _fastcall FSoundObjList(CSound& sound)
{
// index seznamu
	int list = ExecItem[-1].List;

// index zvuku, inkrementace indexu seznamu
	int inx = ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// nastaven� zvuku
	sound = Sound[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lok�ln� zvuk (Data = index)

void _fastcall FSoundLoc(CSound& sound)
{
// index zvuku
	int inx = Sound.Num() - ExecItem[-1].Data;

// nastaven� zvuku
	sound = Sound[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lok�ln� zvuk v seznamu (Data = index, List = seznam)

void _fastcall FSoundLocList(CSound& sound)
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index zvuku, inkrementace indexu seznamu
	int inx = Sound.Num() - ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// nastaven� zvuku
	sound = Sound[inx];
}


/***************************************************************************\
*																			*
*								konverze									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// konverze na 8 bit�

void _fastcall FSoundConv8Bit(CSound& sound)
{
// na�ten� zvuku ke konverzi
	FSound(sound);

// konverze na 8 bit�
	sound.Conv8Bit();
}


/////////////////////////////////////////////////////////////////////////////
// konverze na 16 bit�

void _fastcall FSoundConv16Bit(CSound& sound)
{
// na�ten� zvuku ke konverzi
	FSound(sound);

// konverze na 16 bit�
	sound.Conv16Bit();
}


/////////////////////////////////////////////////////////////////////////////
// konverze na stereo

void _fastcall FSoundConvStereo(CSound& sound)
{
// na�ten� zvuku ke konverzi
	FSound(sound);

// konverze na stereo
	sound.ConvStereo();
}


/////////////////////////////////////////////////////////////////////////////
// konverze na mono

void _fastcall FSoundConvMono(CSound& sound)
{
// na�ten� zvuku ke konverzi
	FSound(sound);

// konverze na mono
	sound.ConvMono();
}


/////////////////////////////////////////////////////////////////////////////
// konverze na frekvenci 11025

void _fastcall FSoundConv11025(CSound& sound)
{
// na�ten� zvuku ke konverzi
	FSound(sound);

// konverze na frekvenci 11025
	sound.ConvRate(11025);
}


/////////////////////////////////////////////////////////////////////////////
// konverze na frekvenci 22050

void _fastcall FSoundConv22050(CSound& sound)
{
// na�ten� zvuku ke konverzi
	FSound(sound);

// konverze na frekvenci 22050
	sound.ConvRate(22050);
}


/////////////////////////////////////////////////////////////////////////////
// konverze na frekvenci 44100

void _fastcall FSoundConv44100(CSound& sound)
{
// na�ten� zvuku ke konverzi
	FSound(sound);

// konverze na frekvenci 44100
	sound.ConvRate(44100);
}

/////////////////////////////////////////////////////////////////////////////
// sou�et jednoho zvuku - ponech� se beze zm�ny

void _fastcall FSoundAdd1(CSound& sound) { FSound(sound); }


/////////////////////////////////////////////////////////////////////////////
// sou�et zvuk� (Data = po�et zvuk� - 1)

void _fastcall FSoundAdd(CSound& sound)
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data;

// na�ten� prvn�ho zvuku
	if (i >= 0) FSound(sound);

// pomocn� buffer zvuku
	CSound sound2;

// cyklus p�es v�echny zvuky
	for (; i > 0; i--)
	{

// na�ten� dal��ho zvuku
		FSound(sound2);
		sound.Add(sound2);
	}
}


/////////////////////////////////////////////////////////////////////////////
// konverze rychlosti zvuku

void _fastcall FSoundSpeed(CSound& sound)
{
// na�ten� vstupn�ho zvuku
	FSound(sound);

// na�ten� koeficientu p�evodu
	double koef = FNum();

// konverze rychlosti zvuku
	sound.ConvSpeed(koef);
}


/////////////////////////////////////////////////////////////////////////////
// t�nov� gener�tor

void _fastcall FTonGen(CSound& sound)
{
// na�ten� frekvence
	double freq = FNum();

// na�ten� d�lky
	double len = FNum();

// vygenerov�n� t�nu
	sound.TonGen(freq, len);
}

/////////////////////////////////////////////////////////////////////////////
// na�ten� zvuku ze souboru

void _fastcall FGetFileSound(CSound& sound)
{
	sound.Empty();
	sound.LoadFile();
}

/////////////////////////////////////////////////////////////////////////////
// dekomprimace zvuku

void _fastcall FSoundConvPCM(CSound& sound)
{
// na�ten� vstupn�ho zvuku
	FSound(sound);

// dekomprese zvuku
	sound.DeComp();
}
