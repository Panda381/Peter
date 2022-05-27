
#include "Main.h"

/***************************************************************************\
*																			*
*									Zvuk									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inicializa�n� pr�zdn� zvuk (modifikuje se po�et referenc�!)

BYTE	EmptySoundData0 = 0;

SOUNDDATA EmptySoundData = { 
	1,								// ��ta� referenc�
	0,								// velikost dat
	SOUNDDEFSAMPLES,				// vzorkovac� kmito�et
	SOUNDDEFFORMAT,					// form�t
	SOUNDDEFCHANNELS,				// po�et kan�l�
	SOUNDDEFBITS,					// po�et bit�
	NULL,							// DirectSoundBuffer
	0,								// velikost z�hlav� WAVEFORMATEX
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
// statick� konstruktor a destruktor

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
// p�ipojen�/odpojen� dat

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
// kopie do vlastn�ho bufferu p�ed modifikac�

void CSound::CopyWrite()
{
	SOUNDDATA* data = pData;			// adresa star�ch dat
	long* refer = &(data->Refer);		// po�et referenc�

	if (*refer > 1)						// je n�jak� jin� majitel?
	{
		NewBuffer(data->Size);			// vytvo�en� nov�ho bufferu
		BYTE* adr = pData->Data;
		MemCopy(pData, data, SIZEOFSOUNDDATA);
		pData->Data = adr;
		pData->DSBuffer = NULL;
		MemCopy(adr, data->Data, pData->Size);
		pData->Refer = 1;				// po�et referenc�

// odpojen� star�ch dat - v multithread m��e nastat i zru�en�
		if (LongDecrement(refer))
		{
#ifdef _MT
			MemFree(data);				// p��padn� zru�en� dat
#endif	// _MT
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// duplikace zvuku (p��p. p�ivlastn�n�)
/*
void CSound::DupCopy()
{
// p�ivlastn�n� neru�iteln�ch dat
	if (pData->Refer > (BigInt - 1000))
	{
		SOUNDDATA* data = (SOUNDDATA*)MemGet(SIZEOFSOUNDDATA);
		MemCopy(data, pData, SIZEOFSOUNDDATA);
		data->Refer = BigInt;				// po�et referenc�
		data->DSBuffer = NULL;
		Detach();							// odpojen� star�ch dat
		pData = data;
	}

// jinak klasick� p�ivlastn�n� bufferu
	else
	{
		CopyWrite();
	}
}
*/

/////////////////////////////////////////////////////////////////////////////
// vypr�zdn�n� zvuku (uvoln�n� dat)

void CSound::Empty()
{ 
	DetachData(); 
	AttachData(&EmptySoundData); 
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� nov�ho zvuku (p�ipraveno k z�pisu, data jsou n�hodn�)

void CSound::New(int size)
{
	DetachData();					// odpojen� star�ho zvuku
	NewBufferData(size);			// vytvo�en� nov�ho bufferu
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� velikosti dat

void CSound::ReSize(int size)
{
// kopie p�ed z�pisem
	CopyWrite();

// nastaven� nov� velikosti bufferu
	ASSERT(size >= 0);
	if (size < 0) size = 0;
	MemBuf(pData->Data, size);
	pData->Size = size;				// velikost dat
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� zvuku ze souboru

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

#pragma warning ( disable: 4701)		// hl�en� - neinicializovan� prom�nn� "hd"
void CSound::LoadFile()
{

#ifndef _MINI

// �schova offsetu souboru
	int oldoff = FileReadOff;
	Empty();

// na�ten� z�hlav� souboru (identifikace a z�kladn� popisova� form�tu)
	WAVHEAD hd;
	int i;
	for (i = 0; i < 4 + 4 + 28; i++)
	{
		*((BYTE*)&hd + i) = FileReadByte();
	}

// form�t dat souboru
	int formatsize = hd.WavFormat.nFormatSize;	// velikost popisova�e form�tu
	int format = hd.WavFormat.wFormatTag;			// form�t dat
	if (formatsize < SIZEOFWAVEFORMATEX)
	{
		format = WAVE_FORMAT_PCM;	// nejsou dopl�uj�c� data form�tu
		formatsize = 16;
	}

// kontrola z�hlav� souboru
	int size = hd.nFileSize;					// velikost souboru
	int newoff = oldoff + 8 + size;				// nov� offset za zvukem

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

// p�esko�en� z�hlav� ID3
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

// p��prava velikosti souboru
			FileReadOff = oldoff;
			size = ::GetFileSize(FileReadHandle, NULL) - FileReadOff;
			if (size < 0) size = 0;

// vytvo�en� bufferu souboru
#define MPEG3FORMATSIZE 30
			New(size + MPEG3FORMATSIZE + 16);	// p�ehr�va� MPEG pot�ebuje rezervu za daty
			pData->Size -= 16;
			pData->Samples = freq;				// vzorkovac� kmito�et
			pData->Channels = (short)chan;		// po�et kan�l�
			pData->Bits = 0;					// po�et bit� na vzorek
			pData->Format = WAVE_FORMAT_MPEGLAYER3;	// form�t
			pData->DSBuffer = NULL;				// nen� DirectSound buffer
			pData->SizeHead = MPEG3FORMATSIZE;	// velikost popisova�e form�tu

// inicializace z�hlav� MP3
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

// na�ten� dat
			FileReadBlok(pData->Data + MPEG3FORMATSIZE, size);

// zkr�cen� o koment�� na konci souboru
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

// p��prava velikosti souboru
			FileReadOff = oldoff;
			size = ::GetFileSize(FileReadHandle, NULL) - FileReadOff;
			if (size < 0) size = 0;

// vytvo�en� bufferu souboru
			New(size + sizeof(MPEG1WAVEFORMAT) + 16);	// p�ehr�va� MPEG pot�ebuje rezervu za daty
			pData->Size -= 16;
			pData->Samples = mp.wfx.nSamplesPerSec;		// vzorkovac� kmito�et
			pData->Channels = (short)mp.wfx.nChannels;			// po�et kan�l�
			pData->Bits = 0;					// po�et bit� na vzorek
			pData->Format = WAVE_FORMAT_MPEG;	// form�t
			pData->DSBuffer = NULL;					// DirectSound buffer
			pData->SizeHead = sizeof(MPEG1WAVEFORMAT);	// velikost popisova�e form�tu

// inicializace z�hlav� MP
			MemCopy(pData->Data, &mp, sizeof(MPEG1WAVEFORMAT));

// na�ten� dat
			FileReadBlok(pData->Data + sizeof(MPEG1WAVEFORMAT), size);

// zkr�cen� o koment�� na konci souboru
			if ((pData->Size >= (128 + sizeof(MPEG1WAVEFORMAT))) &&
				(pData->Data[pData->Size - 128 + 0] == 'T') &&
				(pData->Data[pData->Size - 128 + 1] == 'A') &&
				(pData->Data[pData->Size - 128 + 2] == 'G'))
			{
				pData->Size -= 128;
			}
			return;
		}

// neplatn� form�t souboru
		FileReadOff = oldoff;
		FileError = true;
		return;
	}

// na�ten� popisova�e form�tu
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

// nalezen� datov�ho bloku
	while (FileReadOff < newoff)
	{

// na�ten� z�hlav�
		for (i = 0; i < 8; i++)
		{
			*((BYTE*)(&hd.WavData) + i) = FileReadByte();
		}

// kontrola z�hlav�
		size = hd.WavData.nDataSize;
		if ((hd.WavData.tDataIdent[0] == 'd') &&
			(hd.WavData.tDataIdent[1] == 'a') &&
			(hd.WavData.tDataIdent[2] == 't') &&
			(hd.WavData.tDataIdent[3] == 'a') &&
			(size >= 0))
		{

// na�ten� dat
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

// �schova a korekce form�tu dat souboru
			int channels = wf->nChannels;		// po�et kan�l�
			int samples = wf->nSamplesPerSec;	// vzorkovac� kmito�et
			int bits = wf->wBitsPerSample;		// po�et bit� na vzorek

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

// nastaven� parametr� zvuku
			pData->Samples = samples;			// vzorkovac� kmito�et
			pData->Channels = (short)channels;			// po�et kan�l�
			pData->Bits = (short)bits;					// po�et bit� na vzorek
			pData->Format = format;				// form�t

			MemFree(wf);
			return;
		}

// posun ukazatele na dal�� z�hlav�
		if (size < 0) break;
		FileReadOff += size;
	}

// chyba - datov� blok nenalezen
	FileReadOff = oldoff;
	FileError = true;
	MemFree(wf);

#endif // _MINI
}
#pragma warning ( default: 4701)		// hl�en� - neinicializovan� prom�nn� "hd"


/////////////////////////////////////////////////////////////////////////////
// ulo�en� do souboru

void CSound::SaveFile() const
{
#ifndef _MINI

// p��prava z�hlav� souboru
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

// ulo�en� z�hlav� a dat souboru pro form�t PCM
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

// ulo�en� z�hlav� a dat pro jin� form�t
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
// konverze form�tu na 8 bit�

void _fastcall CSound::Conv8Bit()
{
// dekomprese zvuku na form�t PCM
	DeComp();

// pokud je ji� 8 bit�, nic se ned�je
	if ((Bits() == 8) || (Format() != WAVE_FORMAT_PCM)) return;

// �schova adresy star�ch dat
	SOUNDDATA* olddata = pData;			// adresa star�ch dat
	int size = olddata->Size/2;			// velikost dat

// vytvo�en� bufferu pro nov� data
	NewBufferData(size);				// vytvo�en� nov�ho bufferu
	SOUNDDATA* newdata = pData;			// adresa nov�ch dat

// p�enesen� parametr� zvuku
	newdata->Samples = olddata->Samples; // vzorkovac� kmito�et
	newdata->Format = olddata->Format;	// form�t dat
	newdata->Channels = olddata->Channels; // po�et kan�l�
	newdata->Bits = 8;					// po�et bit�
	newdata->SizeHead = 0;
	
// konverze dat
	short* src = (short*)olddata->Data;	// star� data
	BYTE* dst = newdata->Data;			// nov� data

	for(; size > 0; size--)
	{
		*dst = (BYTE)((WORD)(*src + 0x8000 + 128)/256);
		dst++;
		src++;
	}

// odpojen� star�ch dat
	pData = olddata;					// adresa star�ch dat
	DetachData();						// odpojen� star�ch dat
	pData = newdata;					// adresa nov�ch dat
}


/////////////////////////////////////////////////////////////////////////////
// konverze form�tu na 16 bit�

void _fastcall CSound::Conv16Bit()
{
// dekomprese zvuku na form�t PCM
	DeComp();

// pokud je ji� 16 bit�, nic se ned�je
	if ((Bits() == 16) || (Format() != WAVE_FORMAT_PCM)) return;

// �schova adresy star�ch dat
	SOUNDDATA* olddata = pData;			// adresa star�ch dat
	int size = olddata->Size;			// velikost dat

// vytvo�en� bufferu pro nov� data
	NewBufferData(size*2);				// vytvo�en� nov�ho bufferu
	SOUNDDATA* newdata = pData;			// adresa nov�ch dat

// p�enesen� parametr� zvuku
	newdata->Samples = olddata->Samples; // vzorkovac� kmito�et
	newdata->Format = olddata->Format;	// form�t dat
	newdata->Channels = olddata->Channels; // po�et kan�l�
	newdata->Bits = 16;					// po�et bit�
	newdata->SizeHead = 0;
	
// konverze dat
	BYTE* src = olddata->Data;			// star� data
	short* dst = (short*)newdata->Data;	// nov� data

	for(; size > 0; size--)
	{
		*dst = (short)((char)(*src - 128) * 256);
		dst++;
		src++;
	}

// odpojen� star�ch dat
	pData = olddata;					// adresa star�ch dat
	DetachData();						// odpojen� star�ch dat
	pData = newdata;					// adresa nov�ch dat
}


/////////////////////////////////////////////////////////////////////////////
// konverze form�tu na stereo

void _fastcall CSound::ConvStereo()
{
// dekomprese zvuku na form�t PCM
	DeComp();

// pokud je ji� stereo, nic se ned�je
	if ((Channels() == 2) || (Format() != WAVE_FORMAT_PCM)) return;

// �schova adresy star�ch dat
	SOUNDDATA* olddata = pData;			// adresa star�ch dat
	int size = olddata->Size;			// velikost dat

// vytvo�en� bufferu pro nov� data
	NewBufferData(size*2);				// vytvo�en� nov�ho bufferu
	SOUNDDATA* newdata = pData;			// adresa nov�ch dat

// p�enesen� parametr� zvuku
	newdata->Samples = olddata->Samples; // vzorkovac� kmito�et
	newdata->Format = olddata->Format;	// form�t dat
	newdata->Channels = 2;				// po�et kan�l�
	newdata->SizeHead = 0;
	newdata->Bits = olddata->Bits;		// po�et bit�
	
// konverze dat - 8 bit�
	if (olddata->Bits == 8)
	{
		BYTE* src = olddata->Data;		// star� data
		BYTE* dst = newdata->Data;		// nov� data

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

// konverze dat - 16 bit�
	{
		size /= 2;
		short* src = (short*)olddata->Data;	// star� data
		short* dst = (short*)newdata->Data;	// nov� data

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

// odpojen� star�ch dat
	pData = olddata;					// adresa star�ch dat
	DetachData();						// odpojen� star�ch dat
	pData = newdata;					// adresa nov�ch dat
}


/////////////////////////////////////////////////////////////////////////////
// konverze form�tu na mono

void _fastcall CSound::ConvMono()
{
// dekomprese zvuku na form�t PCM
	DeComp();

// pokud je ji� mono, nic se ned�je
	if ((Channels() == 1) || (Format() != WAVE_FORMAT_PCM)) return;

// �schova adresy star�ch dat
	SOUNDDATA* olddata = pData;			// adresa star�ch dat
	int size = olddata->Size/2;			// velikost dat

// vytvo�en� bufferu pro nov� data
	NewBufferData(size);				// vytvo�en� nov�ho bufferu
	SOUNDDATA* newdata = pData;			// adresa nov�ch dat

// p�enesen� parametr� zvuku
	newdata->Samples = olddata->Samples; // vzorkovac� kmito�et
	newdata->Format = olddata->Format;	// form�t dat
	newdata->Channels = 1;				// po�et kan�l�
	newdata->SizeHead = 0;
	newdata->Bits = olddata->Bits;		// po�et bit�
	
// konverze dat - 8 bit�
	if (olddata->Bits == 8)
	{
		BYTE* src = olddata->Data;		// star� data
		BYTE* dst = newdata->Data;		// nov� data

		for(; size > 0; size--)
		{
			*dst = (BYTE)((WORD)(src[0] + src[1] + ((int)dst & 1))/2);
			src++;
			src++;
			dst++;
		}
	}
	else

// konverze dat - 16 bit�
	{
		size /= 2;
		short* src = (short*)olddata->Data;	// star� data
		short* dst = (short*)newdata->Data;	// nov� data

		for(; size > 0; size--)
		{
			*dst = (short)((src[0] + src[1])/2);
			src++;
			src++;
			dst++;
		}
	}

// odpojen� star�ch dat
	pData = olddata;					// adresa star�ch dat
	DetachData();						// odpojen� star�ch dat
	pData = newdata;					// adresa nov�ch dat
}

/////////////////////////////////////////////////////////////////////////////
// inverze zvuku

void _fastcall CSound::ConvInv()
{
// dekomprese zvuku na form�t PCM
	DeComp();

// mus� b�t PCM form�t
	if (Format() != WAVE_FORMAT_PCM) return;

// �schova adresy star�ch dat
	SOUNDDATA* olddata = pData;			// adresa star�ch dat
	int size = olddata->Size;			// velikost dat

// vytvo�en� bufferu pro nov� data
	NewBufferData(size);				// vytvo�en� nov�ho bufferu
	SOUNDDATA* newdata = pData;			// adresa nov�ch dat

// p�enesen� parametr� zvuku
	newdata->Samples = olddata->Samples; // vzorkovac� kmito�et
	newdata->Format = olddata->Format;	// form�t dat
	newdata->Channels = olddata->Channels;	// po�et kan�l�
	newdata->SizeHead = 0;
	newdata->Bits = olddata->Bits;		// po�et bit�
	
// konverze dat - 8 bit�
	if (olddata->Bits == 8)
	{
		BYTE* src = olddata->Data + size;	// star� data
		BYTE* dst = newdata->Data;		// nov� data

		for(; size > 0; size--)
		{
			src--;
			*dst = *src;
			dst++;
		}
	}
	else

// konverze dat - 16 bit�
	{
		size /= 2;
		short* src = (short*)olddata->Data + size;	// star� data
		short* dst = (short*)newdata->Data;	// nov� data

		for(; size > 0; size--)
		{
			src--;
			*dst = *src;
			dst++;
		}
	}

// odpojen� star�ch dat
	pData = olddata;					// adresa star�ch dat
	DetachData();						// odpojen� star�ch dat
	pData = newdata;					// adresa nov�ch dat
}


/////////////////////////////////////////////////////////////////////////////
// dekomprese zvuku na form�t PCM

void _fastcall CSound::DeComp()
{
// pokud je ji� PCM, nic se neprovede
	if (Format() == WAVE_FORMAT_PCM) return;

// standardn� c�lov� form�t
	WAVEFORMATEX wf;
	wf.wFormatTag = WAVE_FORMAT_PCM;
	wf.nChannels = 2;
	wf.nSamplesPerSec = 22050;
	wf.wBitsPerSample = 16;
	wf.cbSize = 0;

// doporu�en� c�lov� form�t
	::acmFormatSuggest(
		NULL,								// handle proudu
		(WAVEFORMATEX*)DataData(),			// popisova� form�tu zdroje
		&wf,								// popisova� form�tu c�le
		sizeof(WAVEFORMATEX),				// velikost popisova�e form�tu c�le
		ACM_FORMATSUGGESTF_WBITSPERSAMPLE |	// je po�adov�no 16 bit� na vzorek
		ACM_FORMATSUGGESTF_WFORMATTAG);		// je po�adov�n form�t PCM

// korekce c�lov�ho form�tu
	if (wf.nChannels != 2) wf.nChannels = 1;
	if (wf.wBitsPerSample != 8) wf.wBitsPerSample = 16;
	wf.nBlockAlign = (WORD)(wf.nChannels * (wf.wBitsPerSample/8));
	wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;

// otev�en� konverzn�ho proudu
	HACMSTREAM has;

	if (::acmStreamOpen(
		&has,								// handle proudu
		NULL,								// handle ovlada�e ACM
		(WAVEFORMATEX*)DataData(),			// popisova� form�tu zdroje
		&wf,								// popisova� form�tu c�le
		NULL,								// popisova� filtru
		0,									// nen� callback funkce pro hl�en�
		0,									// data pro callback funkci
		ACM_STREAMOPENF_NONREALTIME)		// p��znaky konverze
		!= 0) return;

// ur�en� velikosti bufferu
	int size = (Size() - SizeHead()) * 4;
	::acmStreamSize(has, Size() - SizeHead(), (DWORD*)&size, ACM_STREAMSIZEF_SOURCE);

// vytvo�en� bufferu pro c�l konverze
	BYTE* buf = (BYTE*)MemGet(size);

// z�hlav� proudu
	ACMSTREAMHEADER ash;
	MemFill(&ash, sizeof(ash), 0);
	ash.cbStruct = sizeof(ACMSTREAMHEADER);
	ash.pbSrc = DataData() + SizeHead();
	ash.cbSrcLength = Size() - SizeHead();
	ash.pbDst = buf;
	ash.cbDstLength = size;

// inicializace z�hlav� proudu
	::acmStreamPrepareHeader(has, &ash, 0);

// proveden� konverze
	MMRESULT res = ::acmStreamConvert(has, &ash, 0);

// deinicializace z�hlav� proudu
	::acmStreamUnprepareHeader(has, &ash, 0);

// uzav�en� konverzn�ho proudu
	::acmStreamClose(has, 0);

// pokud prob�hla konverze v po��dku, pou�ij� se nov� data
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

// jinak se zru�� c�lov� buffer
	else
	{
		MemFree(buf);
	}
}


/////////////////////////////////////////////////////////////////////////////
// konverze vzorkovac� frekvence

void _fastcall CSound::ConvRate(int rate)
{
// dekomprese zvuku na form�t PCM
	DeComp();

// pokud je ji� nastaveno, nic se ned�je
	int oldrate = Samples();
	if (rate < 10) rate = 10;
	if (rate > 1000000) rate = 1000000;
	if ((oldrate == rate) || (Format() != WAVE_FORMAT_PCM)) return;

// konverze frekvence
	ConvSpeed((double)oldrate/rate);
	Samples(rate);
}


/////////////////////////////////////////////////////////////////////////////
// konverze rychlosti zvuku (1=beze zm�ny)

void _fastcall CSound::ConvSpeed(double koef)
{
// dekomprese zvuku na form�t PCM
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

// zarovn�n� adresy dat
	int align = -Align();

// �schova adresy star�ch dat
	SOUNDDATA* olddata = pData;			// adresa star�ch dat
	int oldsize = olddata->Size;		// star� velikost dat
	int newsize = Round(oldsize/koef) & align;
	if (oldsize <= 0) return;

// vytvo�en� bufferu pro nov� data
	NewBufferData(newsize);				// vytvo�en� nov�ho bufferu
	SOUNDDATA* newdata = pData;			// adresa nov�ch dat

// p�enesen� parametr� zvuku
	newdata->Samples = olddata->Samples;// vzorkovac� kmito�et
	newdata->Format = olddata->Format;	// form�t dat
	newdata->Channels = olddata->Channels;	// po�et kan�l�
	newdata->DSBuffer = NULL;
	newdata->SizeHead = 0;
	newdata->Bits = olddata->Bits;		// po�et bit�

// konverze dat - 8 bit�
	int inx = 0;
	double oldinx;

	if (olddata->Bits == 8)
	{
		BYTE* src = olddata->Data;		// star� data
		BYTE* dst = newdata->Data;		// nov� data

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

// konverze dat - 16 bit�
	{
		short* src = (short*)olddata->Data;	// star� data
		short* dst = (short*)newdata->Data;	// nov� data

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

// odpojen� star�ch dat
	pData = olddata;					// adresa star�ch dat
	DetachData();						// odpojen� star�ch dat
	pData = newdata;					// adresa nov�ch dat
}


/////////////////////////////////////////////////////////////////////////////
// p�i�ten� zvuku

void _fastcall CSound::Add(CSound src)
{
// dekomprese zvuku na form�t PCM
	DeComp();
	src.DeComp();

// zaji�t�n� stejn�ho po�tu bit�
	if ((Format() != WAVE_FORMAT_PCM) || (src.Format() != WAVE_FORMAT_PCM)) return;
	if (src.Bits() == 16) Conv16Bit();
	if (Bits() == 16) src.Conv16Bit();

// zaji�t�n� stejn�ho po�tu kan�l�
	if (src.Stereo()) ConvStereo();
	if (Stereo()) src.ConvStereo();

// zaji�t�n� stejn�ho vzorkovac�ho kmito�tu
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

// zarovn�n� aktu�ln� velikosti
	pData->Size &= -Align();

// zm�na velikosti bufferu
	int size1 = pData->Size;
	int size2 = src.Size();
	ReSize(size1 + size2);

// p�enesen� dat
	MemCopy(DataData() + size1, src.DataData(), size2);
}


/////////////////////////////////////////////////////////////////////////////
// vygenerov�n� t�nu

void _fastcall CSound::TonGen(double freq, double len)
{
// omezen� d�lky zvuku
	if (len < 0) len = 0;
	if (len > 60*60) len = 60*60;

// omezen� frekvence
	freq = fabs(freq);
	if (freq < 1) freq = 1;
	if (freq > 11025) freq = 11025;

// v�po�et po�tu vln
	int i = Round(len*freq);
	if (i < 1) i = 1;

// velikost dat (vzork�)
	i = Round(i/freq * 22050);

// vytvo�en� nov�ho bufferu
	New(2*i);
	Bits(16);
	Samples(22050);

// vygenerov�n� t�nu
	short* dst = (short*)DataData();
	double koef = 2 * pi * freq / 22050;

	for (; i > 0; i--)
	{
		*dst = (short)Round(32767 * sin(i * koef));
		dst++;
	}
}


/////////////////////////////////////////////////////////////////////////////
// oper�tor p�i�azen�

const CSound& CSound::operator= (const CSound& src)
{
	Detach();				// zru�en� star�ch dat
	Attach(src.pData);		// p�i�azen� nov�ch dat
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
	m_Data = NULL;			// nen� buffer dat
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
}

CBufSound::~CBufSound()
{
	DelAll();				// zru�en� v�ech polo�ek
}


////////////////////////////////////////////////////////////////////
// statick� konstruktor a destruktor

void CBufSound::Init()
{
	m_Data = NULL;			// nen� buffer dat
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
}

void CBufSound::Term()
{
	DelAll();				// zru�en� v�ech polo�ek
}


////////////////////////////////////////////////////////////////////
// vytvo�en� nov�ch dat (odd�leno kv�li lep�� optimalizaci)

void CBufSound::NewData()
{
	m_Max *= 2;
	if (m_Max == 0) m_Max = 0x400;
//	m_Max += 0x400;				// 4 KB
	MemBuf(m_Data, m_Max);
}


////////////////////////////////////////////////////////////////////
// zru�en� v�ech polo�ek v bufferu (ukl�d�n� za�ne op�t po �ad�)

void CBufSound::DelAll()
{
	Del(m_Num);					// zru�en� polo�ek
	MemBuf(m_Data, 0);			// zru�en� bufferu dat
	m_Max = 0;					// nen� ��dn� polo�ka v bufferu
}

////////////////////////////////////////////////////////////////////
// poskytnut� polo�ky (s kontrolou platnosti indexu)

const CSound& _fastcall CBufSound::Get(const int index) const
{
	if (IsValid(index))			// je index platn�?
	{
		return m_Data[index];	// polo�ka na dan�m indexu
	}
	return EmptySound;			// pro neplatn� index vr�t� pr�zdn� zvuk
}

////////////////////////////////////////////////////////////////////
// nastaven� polo�ky (s kontrolou platnosti indexu)

void _fastcall CBufSound::Set(const int index, const CSound& data)
{
	if (IsValid(index))			// je index platn�?
	{
		m_Data[index] = data;
	}
}


/////////////////////////////////////////////////////////////////////////////
// vypr�zdn�n� polo�ky (bez jej�ho zru�en� - jen pro uvoln�n� dat)

void _fastcall CBufSound::Empty(const int index)
{
	if (IsValid(index))					// je index platn�?
	{
		m_Data[index].Empty();
	}
}


////////////////////////////////////////////////////////////////////
// zru�en� polo�ek z konce bufferu

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
// vytvo�en� polo�ky (vrac� index polo�ky)

int CBufSound::New()
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result].Init();		// inicializace polo�ky
	return result;
}

int CBufSound::New(int size)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result].Init(size);	// inicializace polo�ky
	return result;
}

////////////////////////////////////////////////////////////////////
// p�id�n� polo�ky (vrac� index polo�ky)

int _fastcall CBufSound::Add(const CSound& data)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result].Init(data.Data());	// inicializace polo�ky
	return result;
}

int _fastcall CBufSound::Add(SOUNDDATA* data)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result].Init(data);	// inicializace polo�ky
	return result;
}


////////////////////////////////////////////////////////////////////
// duplikace polo�ky (s kontrolou platnosti indexu, vrac� index prvn� polo�ky)

int _fastcall CBufSound::Dup(const int index)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky

	if (IsValid(index))			// je index platn�?
	{
		m_Data[result].Init(m_Data[index].Data());	// kopie polo�ky
	}
	else
	{
		m_Data[result].Init();		// inicializace neplatn� polo�ky
	}
	return result;
}

int _fastcall CBufSound::Dup(const int index, int num)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky

	if (IsValid(index))					// je index platn�?
	{
		SOUNDDATA* data = m_Data[index].Data();

		m_Data[result].Init(data);		// kopie polo�ky

		for (num--; num > 0; num--)
		{
			m_Data[NewItem()].Init(data);
		}
	}
	else
	{
		m_Data[result].Init();		// inicializace neplatn� polo�ky

		for (num--; num > 0; num--)
		{
			m_Data[NewItem()].Init();
		}
	}
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// oper�tor p�i�azen�

const CBufSound& CBufSound::operator= (const CBufSound& src)
{
	Del(m_Num);					// zru�en� star�ch dat

	int index = 0;				// index na��tan� polo�ky
	int i = src.m_Num;			// velikost zdrojov�ho bufferu

	for (; i > 0; i--)			// pro v�echny polo�ky v bufferu
	{
		Add(src[index]);	// kopie polo�ky
		index++;				// inkrementace �tec�ho indexu
	}
	ASSERT(m_Num == src.m_Num);
	return *this;
}
