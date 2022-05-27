
#include "Main.h"

/***************************************************************************\
*																			*
*									Zvuk									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inicializaèní prázdný zvuk (modifikuje se poèet referencí!)

BYTE	EmptySoundData0 = 0;

SOUNDDATA EmptySoundData = { 
	1,								// èítaè referencí
	0,								// velikost dat
	SOUNDDEFSAMPLES,				// vzorkovací kmitoèet
	SOUNDDEFFORMAT,					// formát
	SOUNDDEFCHANNELS,				// poèet kanálù
	SOUNDDEFBITS,					// poèet bitù
	NULL,							// DirectSoundBuffer
	0,								// velikost záhlaví WAVEFORMATEX
	&EmptySoundData0				// adresa dat
};


/////////////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CSound::CSound() 
{ 
	Attach(&EmptySoundData); 
};

CSound::CSound(const CSound& src) 
{ 
	Attach(src.pData); 
};

CSound::CSound(int size)
{
	NewBuffer(size);
};

CSound::~CSound() 
{ 
	Detach(); 
};


/////////////////////////////////////////////////////////////////////////////
// statický konstruktor a destruktor

void CSound::Init()
{ 
	Attach(&EmptySoundData); 
};

void CSound::Init(SOUNDDATA* data)
{ 
	Attach(data); 
};

void CSound::Init(int size)
{
	NewBuffer(size);
};

void CSound::Term()
{ 
	Detach(); 
};


/////////////////////////////////////////////////////////////////////////////
// pøipojení/odpojení dat

void CSound::NewBufferData(int size)
{
	NewBuffer(size);
}

void CSound::AttachData(SOUNDDATA* data)
{
	Attach(data);
}


void CSound::DetachData()
{
	Detach();
}


/////////////////////////////////////////////////////////////////////////////
// kopie do vlastního bufferu pøed modifikací

void CSound::CopyWrite()
{
	SOUNDDATA* data = pData;			// adresa starých dat
	long* refer = &(data->Refer);		// poèet referencí

	if (*refer > 1)						// je nìjaký jiný majitel?
	{
		NewBuffer(data->Size);			// vytvoøení nového bufferu
		BYTE* adr = pData->Data;
		MemCopy(pData, data, SIZEOFSOUNDDATA);
		pData->Data = adr;
		pData->DSBuffer = NULL;
		MemCopy(adr, data->Data, pData->Size);
		pData->Refer = 1;				// poèet referencí

// odpojení starých dat - v multithread mùže nastat i zrušení
		if (LongDecrement(refer))
		{
#ifdef _MT
			MemFree(data);				// pøípadné zrušení dat
#endif	// _MT
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// duplikace zvuku (pøíp. pøivlastnìní)
/*
void CSound::DupCopy()
{
// pøivlastnìní nerušitelných dat
	if (pData->Refer > (BigInt - 1000))
	{
		SOUNDDATA* data = (SOUNDDATA*)MemGet(SIZEOFSOUNDDATA);
		MemCopy(data, pData, SIZEOFSOUNDDATA);
		data->Refer = BigInt;				// poèet referencí
		data->DSBuffer = NULL;
		Detach();							// odpojení starých dat
		pData = data;
	}

// jinak klasické pøivlastnìní bufferu
	else
	{
		CopyWrite();
	}
}
*/

/////////////////////////////////////////////////////////////////////////////
// vyprázdnìní zvuku (uvolnìní dat)

void CSound::Empty()
{ 
	DetachData(); 
	AttachData(&EmptySoundData); 
}


/////////////////////////////////////////////////////////////////////////////
// vytvoøení nového zvuku (pøipraveno k zápisu, data jsou náhodná)

void CSound::New(int size)
{
	DetachData();					// odpojení starého zvuku
	NewBufferData(size);			// vytvoøení nového bufferu
}

/////////////////////////////////////////////////////////////////////////////
// nastavení velikosti dat

void CSound::ReSize(int size)
{
// kopie pøed zápisem
	CopyWrite();

// nastavení nové velikosti bufferu
	ASSERT(size >= 0);
	if (size < 0) size = 0;
	MemBuf(pData->Data, size);
	pData->Size = size;				// velikost dat
}


/////////////////////////////////////////////////////////////////////////////
// naètení zvuku ze souboru

#ifndef _MINI

long MP3freq[4] = { 44100, 48000, 32000, 0 };
BYTE MP3chan[4] = { 2, 2, 2, 1 };
BYTE MPheadmode[4] = {	ACM_MPEG_STEREO,
						ACM_MPEG_JOINTSTEREO,
						ACM_MPEG_DUALCHANNEL,
						ACM_MPEG_SINGLECHANNEL};
BYTE MP3rate[2*4*16] = 
{
// verze 0
	0,	4,	6,	7,	8,	10,	12,	14,	16,	18,	20,	22,	24,	28,	32,	0,	// layer 1
	0,	1,	2,	3,	4,	5,	6,	7,	8,	10,	12,	14,	16,	18,	20,	0,	// layer 2
	0,	1,	2,	3,	4,	5,	6,	7,	8,	10,	12,	14,	16,	18,	20,	0,	// layer 3
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	// layer 4

// verze 1
	0,	4,	8,	12,	16,	20,	24,	28,	32,	36,	40,	44,	48,	52,	56,	0,	// layer 1
	0,	4,	6,	7,	8,	10,	12,	14,	16,	20,	24,	28,	32,	40,	48,	0,	// layer 2
	0,	4,	5,	6,	7,	8,	10,	12,	14,	16,	20,	24,	28,	32,	40,	0,	// layer 3
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0	// layer 4
};

#endif // _MINI

#pragma warning ( disable: 4701)		// hlášení - neinicializovaná promìnná "hd"
void CSound::LoadFile()
{

#ifndef _MINI

// úschova offsetu souboru
	int oldoff = FileReadOff;
	Empty();

// naètení záhlaví souboru (identifikace a základní popisovaè formátu)
	WAVHEAD hd;
	int i;
	for (i = 0; i < 4 + 4 + 28; i++)
	{
		*((BYTE*)&hd + i) = FileReadByte();
	}

// formát dat souboru
	int formatsize = hd.WavFormat.nFormatSize;	// velikost popisovaèe formátu
	int format = hd.WavFormat.wFormatTag;			// formát dat
	if (formatsize < SIZEOFWAVEFORMATEX)
	{
		format = WAVE_FORMAT_PCM;	// nejsou doplòující data formátu
		formatsize = 16;
	}

// kontrola záhlaví souboru
	int size = hd.nFileSize;					// velikost souboru
	int newoff = oldoff + 8 + size;				// nový offset za zvukem

	if ((hd.tWavIdent[0] != 'R') ||
		(hd.tWavIdent[1] != 'I') ||
		(hd.tWavIdent[2] != 'F') ||
		(hd.tWavIdent[3] != 'F') ||
		(size < 28 + 8) ||
		(hd.WavFormat.tFormatIdent[0] != 'W') ||
		(hd.WavFormat.tFormatIdent[1] != 'A') ||
		(hd.WavFormat.tFormatIdent[2] != 'V') ||
		(hd.WavFormat.tFormatIdent[3] != 'E') ||
		(hd.WavFormat.tFormatIdent[4] != 'f') ||
		(hd.WavFormat.tFormatIdent[5] != 'm') ||
		(hd.WavFormat.tFormatIdent[6] != 't') ||
		(hd.WavFormat.tFormatIdent[7] != ' ') ||
		(formatsize > 100000))
	{

// pøeskoèení záhlaví ID3
		if (((*(DWORD*)&hd) & 0xffffff) == '3DI')
		{
			for (int ii = 3000; ii > 0; ii--)
			{
				int off = FileReadOff;

				if (FileReadByte() == 0xff)
				{
					FileReadOff = off;
					break;
				}
			}

			if (ii == 0) FileReadOff = oldoff;

			for (i = 0; i < 4 + 4 + 28; i++)
			{
				*((BYTE*)&hd + i) = FileReadByte();
			}
		}

// interpretace jako soubor MP3
		DWORD hdr = *(DWORD*)&hd;
		int ver = ((hdr >> 11) & 0x1);
		int layer = 4 - ((hdr >> 9) & 0x3);

		int freq = MP3freq[(hdr >> 18) & 0x3];
		if (ver == 0) freq /= 2;

		int chan = MP3chan[(hdr >> 30 ) & 0x3];
		int rate = MP3rate[(ver*4 + (layer - 1))*16 + ((hdr >> 20) & 0xf)] * 1000;

		if (((hdr & 0xf0ff) == 0xf0ff) &&
			(layer == 3) &&
			(freq > 0) &&
			(rate > 0) &&
			FileWriteFlush() && 
			FileReadOpen())
		{

// pøíprava velikosti souboru
			FileReadOff = oldoff;
			size = ::GetFileSize(FileReadHandle, NULL) - FileReadOff;
			if (size < 0) size = 0;

// vytvoøení bufferu souboru
#define MPEG3FORMATSIZE 30
			New(size + MPEG3FORMATSIZE + 16);	// pøehrávaè MPEG potøebuje rezervu za daty
			pData->Size -= 16;
			pData->Samples = freq;				// vzorkovací kmitoèet
			pData->Channels = (short)chan;		// poèet kanálù
			pData->Bits = 0;					// poèet bitù na vzorek
			pData->Format = WAVE_FORMAT_MPEGLAYER3;	// formát
			pData->DSBuffer = NULL;				// není DirectSound buffer
			pData->SizeHead = MPEG3FORMATSIZE;	// velikost popisovaèe formátu

// inicializace záhlaví MP3
			MPEGLAYER3WAVEFORMAT* mp3 = (MPEGLAYER3WAVEFORMAT*)pData->Data;
			mp3->wfx.wFormatTag = WAVE_FORMAT_MPEGLAYER3;
			mp3->wfx.nChannels = (WORD)chan;
			mp3->wfx.nSamplesPerSec = freq;
			mp3->wfx.nAvgBytesPerSec = rate;
			mp3->wfx.nBlockAlign = (WORD)((layer == 1) ? 4 : 1);
			mp3->wfx.wBitsPerSample = 0;
			mp3->wfx.cbSize = MPEGLAYER3_WFX_EXTRA_BYTES;
			mp3->wID = MPEGLAYER3_ID_MPEG;
			mp3->fdwFlags = MPEGLAYER3_FLAG_PADDING_OFF;
			mp3->nBlockSize = (WORD)((288 << ver) * chan * rate / freq);
			mp3->nFramesPerBlock = 1;
			mp3->nCodecDelay = 1393;

// naètení dat
			FileReadBlok(pData->Data + MPEG3FORMATSIZE, size);

// zkrácení o komentáø na konci souboru
			if ((pData->Size >= (128 + MPEG3FORMATSIZE)) &&
				(pData->Data[pData->Size - 128 + 0] == 'T') &&
				(pData->Data[pData->Size - 128 + 1] == 'A') &&
				(pData->Data[pData->Size - 128 + 2] == 'G'))
			{
				pData->Size -= 128;
			}
			return;
		}

// interpretace jako MPEG
		MPEG1WAVEFORMAT mp;
		BYTE* data = (BYTE*)&hd;

		mp.wfx.wFormatTag = WAVE_FORMAT_MPEG;

		int i = (data[3] >> 6);
		mp.fwHeadMode = MPheadmode[i];
		mp.wfx.nChannels = MP3chan[i];

		mp.fwHeadModeExt = (WORD)(1 << (data[3] >> 4));
		mp.wHeadEmphasis = (WORD)((data[3] & 3) + 1);
		mp.fwHeadFlags = (WORD)(
			((data[2] & 1) ? ACM_MPEG_PRIVATEBIT : 0) +
			((data[3] & 8) ? ACM_MPEG_COPYRIGHT : 0) +
			((data[3] & 4) ? ACM_MPEG_ORIGINALHOME : 0) +
			((data[1] & 1) ? ACM_MPEG_PROTECTIONBIT : 0) +
			((data[1] & 8) ? ACM_MPEG_ID_MPEG1 : 0));

		i = ((data[1] >> 1) & 3);
		layer = 4 - i;
		mp.fwHeadLayer = (WORD)(8 >> layer);
		mp.wfx.nSamplesPerSec = MP3freq[(data[2] >> 2) & 3];

		ver = ((data[1] >> 3) & 1);
		int byterate = MP3rate[(ver*4 + (layer - 1))*16 + (data[2] >> 4)] * 1000;
		mp.wfx.nAvgBytesPerSec = byterate;
		mp.dwHeadBitrate = byterate * 8;

		mp.wfx.nBlockAlign = 1;
		if ((mp.wfx.nSamplesPerSec != 44100) &&
			!((layer == 3) && ((data[2] >> 4) == 0)))
		{
			if (layer == 1)
			{
				mp.wfx.nBlockAlign = (WORD)
					(4 * ((mp.dwHeadBitrate * 12) / mp.wfx.nSamplesPerSec));
			}
			else
			{
				mp.wfx.nBlockAlign = (WORD)
					((144 * mp.dwHeadBitrate) / mp.wfx.nSamplesPerSec);
			}
		}

		mp.wfx.wBitsPerSample = 0;
		mp.wfx.cbSize = sizeof(MPEG1WAVEFORMAT) - sizeof(WAVEFORMATEX);
		mp.dwPTSLow = 0;
		mp.dwPTSHigh = 0;

		if (((*(WORD*)data & 0xf0ff) == 0xf0ff) &&
			(mp.wfx.nSamplesPerSec > 0) &&
			(mp.wfx.nAvgBytesPerSec > 0) &&
			FileWriteFlush() && 
			FileReadOpen())
		{

// pøíprava velikosti souboru
			FileReadOff = oldoff;
			size = ::GetFileSize(FileReadHandle, NULL) - FileReadOff;
			if (size < 0) size = 0;

// vytvoøení bufferu souboru
			New(size + sizeof(MPEG1WAVEFORMAT) + 16);	// pøehrávaè MPEG potøebuje rezervu za daty
			pData->Size -= 16;
			pData->Samples = mp.wfx.nSamplesPerSec;		// vzorkovací kmitoèet
			pData->Channels = (short)mp.wfx.nChannels;			// poèet kanálù
			pData->Bits = 0;					// poèet bitù na vzorek
			pData->Format = WAVE_FORMAT_MPEG;	// formát
			pData->DSBuffer = NULL;					// DirectSound buffer
			pData->SizeHead = sizeof(MPEG1WAVEFORMAT);	// velikost popisovaèe formátu

// inicializace záhlaví MP
			MemCopy(pData->Data, &mp, sizeof(MPEG1WAVEFORMAT));

// naètení dat
			FileReadBlok(pData->Data + sizeof(MPEG1WAVEFORMAT), size);

// zkrácení o komentáø na konci souboru
			if ((pData->Size >= (128 + sizeof(MPEG1WAVEFORMAT))) &&
				(pData->Data[pData->Size - 128 + 0] == 'T') &&
				(pData->Data[pData->Size - 128 + 1] == 'A') &&
				(pData->Data[pData->Size - 128 + 2] == 'G'))
			{
				pData->Size -= 128;
			}
			return;
		}

// neplatný formát souboru
		FileReadOff = oldoff;
		FileError = true;
		return;
	}

// naètení popisovaèe formátu
	WAVEFORMATEX* wf = (WAVEFORMATEX*)MemGet(formatsize + 16);
	wf->wFormatTag = (WORD)format;
	wf->nChannels = hd.WavFormat.nChannels;
	wf->nSamplesPerSec = hd.WavFormat.nSamplesPerSec;
	wf->nAvgBytesPerSec = hd.WavFormat.nAvgBytesPerSec;
	wf->nBlockAlign = hd.WavFormat.nBlockAlign;
	wf->wBitsPerSample = hd.WavFormat.wBitsPerSample;
	for (i = 0; i < formatsize - 16; i++)
	{
		*((BYTE*)wf + 16 + i) = FileReadByte();
	}

	if (format != WAVE_FORMAT_PCM)
	{
		wf->cbSize = (WORD)(formatsize - SIZEOFWAVEFORMATEX);

		if (format == WAVE_FORMAT_MPEGLAYER3)
		{
			wf->cbSize = MPEGLAYER3_WFX_EXTRA_BYTES;
		}
	}

// nalezení datového bloku
	while (FileReadOff < newoff)
	{

// naètení záhlaví
		for (i = 0; i < 8; i++)
		{
			*((BYTE*)(&hd.WavData) + i) = FileReadByte();
		}

// kontrola záhlaví
		size = hd.WavData.nDataSize;
		if ((hd.WavData.tDataIdent[0] == 'd') &&
			(hd.WavData.tDataIdent[1] == 'a') &&
			(hd.WavData.tDataIdent[2] == 't') &&
			(hd.WavData.tDataIdent[3] == 'a') &&
			(size >= 0))
		{

// naètení dat
			if (format == WAVE_FORMAT_PCM)
			{
				New(size);
				FileReadBlok(pData->Data, size);
				pData->SizeHead = 0;
			}
			else
			{
				New(size + formatsize);
				MemCopy(pData->Data, wf, formatsize);
				FileReadBlok(pData->Data + formatsize, size);
				pData->SizeHead = formatsize;
			}

			FileReadOff = newoff;

// úschova a korekce formátu dat souboru
			int channels = wf->nChannels;		// poèet kanálù
			int samples = wf->nSamplesPerSec;	// vzorkovací kmitoèet
			int bits = wf->wBitsPerSample;		// poèet bitù na vzorek

			if (format == WAVE_FORMAT_PCM)
			{
				if (samples < ((11025 + 22050)/2))
				{
					samples = 11025;
				}
				else
				{
					if (samples > ((22050 + 44100)/2))
					{
						samples = 44100;
					}
					else
					{
						samples = 22050;
					}
				}
				if (channels != 2) channels = 1;
				if (bits != 16) bits = 8;
			}

// nastavení parametrù zvuku
			pData->Samples = samples;			// vzorkovací kmitoèet
			pData->Channels = (short)channels;			// poèet kanálù
			pData->Bits = (short)bits;					// poèet bitù na vzorek
			pData->Format = format;				// formát

			MemFree(wf);
			return;
		}

// posun ukazatele na další záhlaví
		if (size < 0) break;
		FileReadOff += size;
	}

// chyba - datový blok nenalezen
	FileReadOff = oldoff;
	FileError = true;
	MemFree(wf);

#endif // _MINI
}
#pragma warning ( default: 4701)		// hlášení - neinicializovaná promìnná "hd"


/////////////////////////////////////////////////////////////////////////////
// uložení do souboru

void CSound::SaveFile() const
{
#ifndef _MINI

// pøíprava záhlaví souboru
	WAVHEAD head;
	head.tWavIdent[0] = 'R';
	head.tWavIdent[1] = 'I';
	head.tWavIdent[2] = 'F';
	head.tWavIdent[3] = 'F';

	head.WavFormat.tFormatIdent[0] = 'W';
	head.WavFormat.tFormatIdent[1] = 'A';
	head.WavFormat.tFormatIdent[2] = 'V';
	head.WavFormat.tFormatIdent[3] = 'E';
	head.WavFormat.tFormatIdent[4] = 'f';
	head.WavFormat.tFormatIdent[5] = 'm';
	head.WavFormat.tFormatIdent[6] = 't';
	head.WavFormat.tFormatIdent[7] = ' ';

	head.WavData.tDataIdent[0] = 'd';
	head.WavData.tDataIdent[1] = 'a';
	head.WavData.tDataIdent[2] = 't';
	head.WavData.tDataIdent[3] = 'a';

// uložení záhlaví a dat souboru pro formát PCM
	if (pData->Format == WAVE_FORMAT_PCM)
	{
		head.nFileSize = pData->Size + sizeof(WAVHEAD) - 8;

		head.WavFormat.nFormatSize = 16;
		head.WavFormat.wFormatTag = WAVE_FORMAT_PCM;
		head.WavFormat.nChannels = (WORD)pData->Channels;
		head.WavFormat.nSamplesPerSec = pData->Samples;
		head.WavFormat.nAvgBytesPerSec = DataRate();
		head.WavFormat.nBlockAlign = (WORD)Align();
		head.WavFormat.wBitsPerSample = (WORD)pData->Bits;

		head.WavData.nDataSize = pData->Size;

		FileWriteBlok((BYTE*)&head, sizeof(WAVHEAD));
		FileWriteBlok(pData->Data, pData->Size);
	}

// uložení záhlaví a dat pro jiný formát
	else
	{
		head.nFileSize = pData->Size + 12 + sizeof(WAVDATA);

		head.WavFormat.nFormatSize = pData->SizeHead;

		head.WavData.nDataSize = pData->Size - pData->SizeHead;

		FileWriteBlok((BYTE*)&head, 8 + 12);
		FileWriteBlok(pData->Data, pData->SizeHead);
		FileWriteBlok((BYTE*)&head.WavData, sizeof(WAVDATA));
		FileWriteBlok(pData->Data + pData->SizeHead, pData->Size - pData->SizeHead);
	}

#endif // _MINI
}


/////////////////////////////////////////////////////////////////////////////
// konverze formátu na 8 bitù

void _fastcall CSound::Conv8Bit()
{
// dekomprese zvuku na formát PCM
	DeComp();

// pokud je již 8 bitù, nic se nedìje
	if ((Bits() == 8) || (Format() != WAVE_FORMAT_PCM)) return;

// úschova adresy starých dat
	SOUNDDATA* olddata = pData;			// adresa starých dat
	int size = olddata->Size/2;			// velikost dat

// vytvoøení bufferu pro nová data
	NewBufferData(size);				// vytvoøení nového bufferu
	SOUNDDATA* newdata = pData;			// adresa nových dat

// pøenesení parametrù zvuku
	newdata->Samples = olddata->Samples; // vzorkovací kmitoèet
	newdata->Format = olddata->Format;	// formát dat
	newdata->Channels = olddata->Channels; // poèet kanálù
	newdata->Bits = 8;					// poèet bitù
	newdata->SizeHead = 0;
	
// konverze dat
	short* src = (short*)olddata->Data;	// stará data
	BYTE* dst = newdata->Data;			// nová data

	for(; size > 0; size--)
	{
		*dst = (BYTE)((WORD)(*src + 0x8000 + 128)/256);
		dst++;
		src++;
	}

// odpojení starých dat
	pData = olddata;					// adresa starých dat
	DetachData();						// odpojení starých dat
	pData = newdata;					// adresa nových dat
}


/////////////////////////////////////////////////////////////////////////////
// konverze formátu na 16 bitù

void _fastcall CSound::Conv16Bit()
{
// dekomprese zvuku na formát PCM
	DeComp();

// pokud je již 16 bitù, nic se nedìje
	if ((Bits() == 16) || (Format() != WAVE_FORMAT_PCM)) return;

// úschova adresy starých dat
	SOUNDDATA* olddata = pData;			// adresa starých dat
	int size = olddata->Size;			// velikost dat

// vytvoøení bufferu pro nová data
	NewBufferData(size*2);				// vytvoøení nového bufferu
	SOUNDDATA* newdata = pData;			// adresa nových dat

// pøenesení parametrù zvuku
	newdata->Samples = olddata->Samples; // vzorkovací kmitoèet
	newdata->Format = olddata->Format;	// formát dat
	newdata->Channels = olddata->Channels; // poèet kanálù
	newdata->Bits = 16;					// poèet bitù
	newdata->SizeHead = 0;
	
// konverze dat
	BYTE* src = olddata->Data;			// stará data
	short* dst = (short*)newdata->Data;	// nová data

	for(; size > 0; size--)
	{
		*dst = (short)((char)(*src - 128) * 256);
		dst++;
		src++;
	}

// odpojení starých dat
	pData = olddata;					// adresa starých dat
	DetachData();						// odpojení starých dat
	pData = newdata;					// adresa nových dat
}


/////////////////////////////////////////////////////////////////////////////
// konverze formátu na stereo

void _fastcall CSound::ConvStereo()
{
// dekomprese zvuku na formát PCM
	DeComp();

// pokud je již stereo, nic se nedìje
	if ((Channels() == 2) || (Format() != WAVE_FORMAT_PCM)) return;

// úschova adresy starých dat
	SOUNDDATA* olddata = pData;			// adresa starých dat
	int size = olddata->Size;			// velikost dat

// vytvoøení bufferu pro nová data
	NewBufferData(size*2);				// vytvoøení nového bufferu
	SOUNDDATA* newdata = pData;			// adresa nových dat

// pøenesení parametrù zvuku
	newdata->Samples = olddata->Samples; // vzorkovací kmitoèet
	newdata->Format = olddata->Format;	// formát dat
	newdata->Channels = 2;				// poèet kanálù
	newdata->SizeHead = 0;
	newdata->Bits = olddata->Bits;		// poèet bitù
	
// konverze dat - 8 bitù
	if (olddata->Bits == 8)
	{
		BYTE* src = olddata->Data;		// stará data
		BYTE* dst = newdata->Data;		// nová data

		for(; size > 0; size--)
		{
			BYTE data = *src;
			src++;
			*dst = data;
			dst++;
			*dst = data;
			dst++;
		}
	}
	else

// konverze dat - 16 bitù
	{
		size /= 2;
		short* src = (short*)olddata->Data;	// stará data
		short* dst = (short*)newdata->Data;	// nová data

		for(; size > 0; size--)
		{
			short data = *src;
			src++;
			*dst = data;
			dst++;
			*dst = data;
			dst++;
		}
	}

// odpojení starých dat
	pData = olddata;					// adresa starých dat
	DetachData();						// odpojení starých dat
	pData = newdata;					// adresa nových dat
}


/////////////////////////////////////////////////////////////////////////////
// konverze formátu na mono

void _fastcall CSound::ConvMono()
{
// dekomprese zvuku na formát PCM
	DeComp();

// pokud je již mono, nic se nedìje
	if ((Channels() == 1) || (Format() != WAVE_FORMAT_PCM)) return;

// úschova adresy starých dat
	SOUNDDATA* olddata = pData;			// adresa starých dat
	int size = olddata->Size/2;			// velikost dat

// vytvoøení bufferu pro nová data
	NewBufferData(size);				// vytvoøení nového bufferu
	SOUNDDATA* newdata = pData;			// adresa nových dat

// pøenesení parametrù zvuku
	newdata->Samples = olddata->Samples; // vzorkovací kmitoèet
	newdata->Format = olddata->Format;	// formát dat
	newdata->Channels = 1;				// poèet kanálù
	newdata->SizeHead = 0;
	newdata->Bits = olddata->Bits;		// poèet bitù
	
// konverze dat - 8 bitù
	if (olddata->Bits == 8)
	{
		BYTE* src = olddata->Data;		// stará data
		BYTE* dst = newdata->Data;		// nová data

		for(; size > 0; size--)
		{
			*dst = (BYTE)((WORD)(src[0] + src[1] + ((int)dst & 1))/2);
			src++;
			src++;
			dst++;
		}
	}
	else

// konverze dat - 16 bitù
	{
		size /= 2;
		short* src = (short*)olddata->Data;	// stará data
		short* dst = (short*)newdata->Data;	// nová data

		for(; size > 0; size--)
		{
			*dst = (short)((src[0] + src[1])/2);
			src++;
			src++;
			dst++;
		}
	}

// odpojení starých dat
	pData = olddata;					// adresa starých dat
	DetachData();						// odpojení starých dat
	pData = newdata;					// adresa nových dat
}

/////////////////////////////////////////////////////////////////////////////
// inverze zvuku

void _fastcall CSound::ConvInv()
{
// dekomprese zvuku na formát PCM
	DeComp();

// musí být PCM formát
	if (Format() != WAVE_FORMAT_PCM) return;

// úschova adresy starých dat
	SOUNDDATA* olddata = pData;			// adresa starých dat
	int size = olddata->Size;			// velikost dat

// vytvoøení bufferu pro nová data
	NewBufferData(size);				// vytvoøení nového bufferu
	SOUNDDATA* newdata = pData;			// adresa nových dat

// pøenesení parametrù zvuku
	newdata->Samples = olddata->Samples; // vzorkovací kmitoèet
	newdata->Format = olddata->Format;	// formát dat
	newdata->Channels = olddata->Channels;	// poèet kanálù
	newdata->SizeHead = 0;
	newdata->Bits = olddata->Bits;		// poèet bitù
	
// konverze dat - 8 bitù
	if (olddata->Bits == 8)
	{
		BYTE* src = olddata->Data + size;	// stará data
		BYTE* dst = newdata->Data;		// nová data

		for(; size > 0; size--)
		{
			src--;
			*dst = *src;
			dst++;
		}
	}
	else

// konverze dat - 16 bitù
	{
		size /= 2;
		short* src = (short*)olddata->Data + size;	// stará data
		short* dst = (short*)newdata->Data;	// nová data

		for(; size > 0; size--)
		{
			src--;
			*dst = *src;
			dst++;
		}
	}

// odpojení starých dat
	pData = olddata;					// adresa starých dat
	DetachData();						// odpojení starých dat
	pData = newdata;					// adresa nových dat
}


/////////////////////////////////////////////////////////////////////////////
// dekomprese zvuku na formát PCM

void _fastcall CSound::DeComp()
{
// pokud je již PCM, nic se neprovede
	if (Format() == WAVE_FORMAT_PCM) return;

// standardní cílový formát
	WAVEFORMATEX wf;
	wf.wFormatTag = WAVE_FORMAT_PCM;
	wf.nChannels = 2;
	wf.nSamplesPerSec = 22050;
	wf.wBitsPerSample = 16;
	wf.cbSize = 0;

// doporuèený cílový formát
	::acmFormatSuggest(
		NULL,								// handle proudu
		(WAVEFORMATEX*)DataData(),			// popisovaè formátu zdroje
		&wf,								// popisovaè formátu cíle
		sizeof(WAVEFORMATEX),				// velikost popisovaèe formátu cíle
		ACM_FORMATSUGGESTF_WBITSPERSAMPLE |	// je požadováno 16 bitù na vzorek
		ACM_FORMATSUGGESTF_WFORMATTAG);		// je požadován formát PCM

// korekce cílového formátu
	if (wf.nChannels != 2) wf.nChannels = 1;
	if (wf.wBitsPerSample != 8) wf.wBitsPerSample = 16;
	wf.nBlockAlign = (WORD)(wf.nChannels * (wf.wBitsPerSample/8));
	wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;

// otevøení konverzního proudu
	HACMSTREAM has;

	if (::acmStreamOpen(
		&has,								// handle proudu
		NULL,								// handle ovladaèe ACM
		(WAVEFORMATEX*)DataData(),			// popisovaè formátu zdroje
		&wf,								// popisovaè formátu cíle
		NULL,								// popisovaè filtru
		0,									// není callback funkce pro hlášení
		0,									// data pro callback funkci
		ACM_STREAMOPENF_NONREALTIME)		// pøíznaky konverze
		!= 0) return;

// urèení velikosti bufferu
	int size = (Size() - SizeHead()) * 4;
	::acmStreamSize(has, Size() - SizeHead(), (DWORD*)&size, ACM_STREAMSIZEF_SOURCE);

// vytvoøení bufferu pro cíl konverze
	BYTE* buf = (BYTE*)MemGet(size);

// záhlaví proudu
	ACMSTREAMHEADER ash;
	MemFill(&ash, sizeof(ash), 0);
	ash.cbStruct = sizeof(ACMSTREAMHEADER);
	ash.pbSrc = DataData() + SizeHead();
	ash.cbSrcLength = Size() - SizeHead();
	ash.pbDst = buf;
	ash.cbDstLength = size;

// inicializace záhlaví proudu
	::acmStreamPrepareHeader(has, &ash, 0);

// provedení konverze
	MMRESULT res = ::acmStreamConvert(has, &ash, 0);

// deinicializace záhlaví proudu
	::acmStreamUnprepareHeader(has, &ash, 0);

// uzavøení konverzního proudu
	::acmStreamClose(has, 0);

// pokud probìhla konverze v poøádku, použijí se nová data
	if (res == 0)
	{
		Detach();
		pData = (SOUNDDATA*)MemGet(SIZEOFSOUNDDATA);
		pData->Refer = 1;
		pData->Size = ash.cbDstLengthUsed;
		pData->Samples = wf.nSamplesPerSec;
		pData->Format = WAVE_FORMAT_PCM;
		pData->Channels = (short)wf.nChannels;
		pData->Bits = (short)wf.wBitsPerSample;
		pData->SizeHead = 0;
		pData->DSBuffer = NULL;
		pData->Data = buf;
	}

// jinak se zruší cílový buffer
	else
	{
		MemFree(buf);
	}
}


/////////////////////////////////////////////////////////////////////////////
// konverze vzorkovací frekvence

void _fastcall CSound::ConvRate(int rate)
{
// dekomprese zvuku na formát PCM
	DeComp();

// pokud je již nastaveno, nic se nedìje
	int oldrate = Samples();
	if (rate < 10) rate = 10;
	if (rate > 1000000) rate = 1000000;
	if ((oldrate == rate) || (Format() != WAVE_FORMAT_PCM)) return;

// konverze frekvence
	ConvSpeed((double)oldrate/rate);
	Samples(rate);
}


/////////////////////////////////////////////////////////////////////////////
// konverze rychlosti zvuku (1=beze zmìny)

void _fastcall CSound::ConvSpeed(double koef)
{
// dekomprese zvuku na formát PCM
	DeComp();

// inverze
	if (koef < 0)
	{
		ConvInv();
		koef = -koef;
	}

// kontrola platnosti
	if (Format() != WAVE_FORMAT_PCM) return;
	if (koef == 1) return;
	if (koef < 0.00001) koef = 0.00001;
	if (koef > 100000) koef = 100000;

// zarovnání adresy dat
	int align = -Align();

// úschova adresy starých dat
	SOUNDDATA* olddata = pData;			// adresa starých dat
	int oldsize = olddata->Size;		// stará velikost dat
	int newsize = Round(oldsize/koef) & align;
	if (oldsize <= 0) return;

// vytvoøení bufferu pro nová data
	NewBufferData(newsize);				// vytvoøení nového bufferu
	SOUNDDATA* newdata = pData;			// adresa nových dat

// pøenesení parametrù zvuku
	newdata->Samples = olddata->Samples;// vzorkovací kmitoèet
	newdata->Format = olddata->Format;	// formát dat
	newdata->Channels = olddata->Channels;	// poèet kanálù
	newdata->DSBuffer = NULL;
	newdata->SizeHead = 0;
	newdata->Bits = olddata->Bits;		// poèet bitù

// konverze dat - 8 bitù
	int inx = 0;
	double oldinx;

	if (olddata->Bits == 8)
	{
		BYTE* src = olddata->Data;		// stará data
		BYTE* dst = newdata->Data;		// nová data

		if (olddata->Channels == 1)
		{
			for(int size = newsize; size > 0; size--)
			{
				oldinx = inx*koef;
				int i = RoundM(oldinx);
				if ((DWORD)i >= (DWORD)oldsize) i = oldsize - 1;
				int j = i + 1;
				if ((DWORD)j >= (DWORD)oldsize) j = oldsize - 1;

				dst[inx] = (BYTE)(src[i] + Round((src[j] - src[i])*(oldinx - i)));

				inx++;
			}
		}
		else
		{
			oldsize /= 2;

			for(int size = newsize/2; size > 0; size--)
			{
				oldinx = inx*koef;
				int i = RoundM(oldinx);
				if ((DWORD)i >= (DWORD)oldsize) i = oldsize - 1;
				int j = i + 1;
				if ((DWORD)j >= (DWORD)oldsize) j = oldsize - 1;

				dst[2*inx] = (BYTE)(src[2*i] + 
					Round((src[2*j] - src[2*i])*(oldinx - i)));

				dst[2*inx + 1] = (BYTE)(src[2*i + 1] + 
					Round((src[2*j + 1] - src[2*i + 1])*(oldinx - i)));

				inx++;
			}
		}
	}
	else

// konverze dat - 16 bitù
	{
		short* src = (short*)olddata->Data;	// stará data
		short* dst = (short*)newdata->Data;	// nová data

		if (olddata->Channels == 1)
		{
			oldsize /= 2;

			for(int size = newsize/2; size > 0; size--)
			{
				oldinx = inx*koef;
				int i = RoundM(oldinx);
				if ((DWORD)i >= (DWORD)oldsize) i = oldsize - 1;
				int j = i + 1;
				if ((DWORD)j >= (DWORD)oldsize) j = oldsize - 1;

				dst[inx] = (short)(src[i] + Round((src[j] - src[i])*(oldinx - i)));

				inx++;
			}
		}
		else
		{
			oldsize /= 4;

			for(int size = newsize/4; size > 0; size--)
			{
				oldinx = inx*koef;
				int i = RoundM(oldinx);
				if ((DWORD)i >= (DWORD)oldsize) i = oldsize - 1;
				int j = i + 1;
				if ((DWORD)j >= (DWORD)oldsize) j = oldsize - 1;

				dst[2*inx] = (short)(src[2*i] + 
					Round((src[2*j] - src[2*i])*(oldinx - i)));

				dst[2*inx + 1] = (short)(src[2*i + 1] + 
					Round((src[2*j + 1] - src[2*i + 1])*(oldinx - i)));

				inx++;
			}
		}
	}

// odpojení starých dat
	pData = olddata;					// adresa starých dat
	DetachData();						// odpojení starých dat
	pData = newdata;					// adresa nových dat
}


/////////////////////////////////////////////////////////////////////////////
// pøiètení zvuku

void _fastcall CSound::Add(CSound src)
{
// dekomprese zvuku na formát PCM
	DeComp();
	src.DeComp();

// zajištìní stejného poètu bitù
	if ((Format() != WAVE_FORMAT_PCM) || (src.Format() != WAVE_FORMAT_PCM)) return;
	if (src.Bits() == 16) Conv16Bit();
	if (Bits() == 16) src.Conv16Bit();

// zajištìní stejného poètu kanálù
	if (src.Stereo()) ConvStereo();
	if (Stereo()) src.ConvStereo();

// zajištìní stejného vzorkovacího kmitoètu
	int rate = 	src.Samples();
	if (rate != Samples())
	{
		if (Samples() > rate) rate = Samples();
		if (rate <= 11025 + 25)
		{
			rate = 11025;
		}
		else
		{
			if (rate <= 22050 + 50)
			{
				rate = 22050;
			}
			else
			{
				rate = 44100;
			}
		}

		src.ConvRate(rate);
		ConvRate(rate);
	}

// zarovnání aktuální velikosti
	pData->Size &= -Align();

// zmìna velikosti bufferu
	int size1 = pData->Size;
	int size2 = src.Size();
	ReSize(size1 + size2);

// pøenesení dat
	MemCopy(DataData() + size1, src.DataData(), size2);
}


/////////////////////////////////////////////////////////////////////////////
// vygenerování tónu

void _fastcall CSound::TonGen(double freq, double len)
{
// omezení délky zvuku
	if (len < 0) len = 0;
	if (len > 60*60) len = 60*60;

// omezení frekvence
	freq = fabs(freq);
	if (freq < 1) freq = 1;
	if (freq > 11025) freq = 11025;

// výpoèet poètu vln
	int i = Round(len*freq);
	if (i < 1) i = 1;

// velikost dat (vzorkù)
	i = Round(i/freq * 22050);

// vytvoøení nového bufferu
	New(2*i);
	Bits(16);
	Samples(22050);

// vygenerování tónu
	short* dst = (short*)DataData();
	double koef = 2 * pi * freq / 22050;

	for (; i > 0; i--)
	{
		*dst = (short)Round(32767 * sin(i * koef));
		dst++;
	}
}


/////////////////////////////////////////////////////////////////////////////
// operátor pøiøazení

const CSound& CSound::operator= (const CSound& src)
{
	Detach();				// zrušení starých dat
	Attach(src.pData);		// pøiøazení nových dat
	return *this;
}


/***************************************************************************\
*																			*
*								Buffer zvuku								*
*																			*
\***************************************************************************/


////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CBufSound::CBufSound()
{
	m_Data = NULL;			// není buffer dat
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není buffer položek
}

CBufSound::~CBufSound()
{
	DelAll();				// zrušení všech položek
}


////////////////////////////////////////////////////////////////////
// statický konstruktor a destruktor

void CBufSound::Init()
{
	m_Data = NULL;			// není buffer dat
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není buffer položek
}

void CBufSound::Term()
{
	DelAll();				// zrušení všech položek
}


////////////////////////////////////////////////////////////////////
// vytvoøení nových dat (oddìleno kvùli lepší optimalizaci)

void CBufSound::NewData()
{
	m_Max *= 2;
	if (m_Max == 0) m_Max = 0x400;
//	m_Max += 0x400;				// 4 KB
	MemBuf(m_Data, m_Max);
}


////////////////////////////////////////////////////////////////////
// zrušení všech položek v bufferu (ukládání zaène opìt po øadì)

void CBufSound::DelAll()
{
	Del(m_Num);					// zrušení položek
	MemBuf(m_Data, 0);			// zrušení bufferu dat
	m_Max = 0;					// není žádná položka v bufferu
}

////////////////////////////////////////////////////////////////////
// poskytnutí položky (s kontrolou platnosti indexu)

const CSound& _fastcall CBufSound::Get(const int index) const
{
	if (IsValid(index))			// je index platný?
	{
		return m_Data[index];	// položka na daném indexu
	}
	return EmptySound;			// pro neplatný index vrátí prázdný zvuk
}

////////////////////////////////////////////////////////////////////
// nastavení položky (s kontrolou platnosti indexu)

void _fastcall CBufSound::Set(const int index, const CSound& data)
{
	if (IsValid(index))			// je index platný?
	{
		m_Data[index] = data;
	}
}


/////////////////////////////////////////////////////////////////////////////
// vyprázdnìní položky (bez jejího zrušení - jen pro uvolnìní dat)

void _fastcall CBufSound::Empty(const int index)
{
	if (IsValid(index))					// je index platný?
	{
		m_Data[index].Empty();
	}
}


////////////////////////////////////////////////////////////////////
// zrušení položek z konce bufferu

void _fastcall CBufSound::Del(int num)
{
	if (num > m_Num) num = m_Num;

	CSound* snd = m_Data + m_Num - 1;
	m_Num -= num;

	for (; num > 0; num--)
	{
		snd->Term();
		snd--;
	}
}


////////////////////////////////////////////////////////////////////
// vytvoøení položky (vrací index položky)

int CBufSound::New()
{
	int result = NewItem();		// vytvoøení nové položky
	m_Data[result].Init();		// inicializace položky
	return result;
}

int CBufSound::New(int size)
{
	int result = NewItem();		// vytvoøení nové položky
	m_Data[result].Init(size);	// inicializace položky
	return result;
}

////////////////////////////////////////////////////////////////////
// pøidání položky (vrací index položky)

int _fastcall CBufSound::Add(const CSound& data)
{
	int result = NewItem();		// vytvoøení nové položky
	m_Data[result].Init(data.Data());	// inicializace položky
	return result;
}

int _fastcall CBufSound::Add(SOUNDDATA* data)
{
	int result = NewItem();		// vytvoøení nové položky
	m_Data[result].Init(data);	// inicializace položky
	return result;
}


////////////////////////////////////////////////////////////////////
// duplikace položky (s kontrolou platnosti indexu, vrací index první položky)

int _fastcall CBufSound::Dup(const int index)
{
	int result = NewItem();		// vytvoøení nové položky

	if (IsValid(index))			// je index platný?
	{
		m_Data[result].Init(m_Data[index].Data());	// kopie položky
	}
	else
	{
		m_Data[result].Init();		// inicializace neplatné položky
	}
	return result;
}

int _fastcall CBufSound::Dup(const int index, int num)
{
	int result = NewItem();		// vytvoøení nové položky

	if (IsValid(index))					// je index platný?
	{
		SOUNDDATA* data = m_Data[index].Data();

		m_Data[result].Init(data);		// kopie položky

		for (num--; num > 0; num--)
		{
			m_Data[NewItem()].Init(data);
		}
	}
	else
	{
		m_Data[result].Init();		// inicializace neplatné položky

		for (num--; num > 0; num--)
		{
			m_Data[NewItem()].Init();
		}
	}
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// operátor pøiøazení

const CBufSound& CBufSound::operator= (const CBufSound& src)
{
	Del(m_Num);					// zrušení starých dat

	int index = 0;				// index naèítané položky
	int i = src.m_Num;			// velikost zdrojového bufferu

	for (; i > 0; i--)			// pro všechny položky v bufferu
	{
		Add(src[index]);	// kopie položky
		index++;				// inkrementace ètecího indexu
	}
	ASSERT(m_Num == src.m_Num);
	return *this;
}
