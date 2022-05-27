
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
	22050,							// vzorkovací kmitoèet
	WAVE_FORMAT_PCM,				// formát
	1,								// poèet kanálù
	16,								// poèet bitù
	0,								// velikost záhlaví WAVEFORMATEX
	&EmptySoundData0				// adresa dat
};

const CSound		EmptySound;		// prázdný zvuk

/////////////////////////////////////////////////////////////////////////////
// vytvoøení dat zvuku (pøi chybì pamìti vrací NULL)

SOUNDDATA* _fastcall NewSoundData(int size)
{
	ASSERT(size >= 0);

// vytvoøení záhlaví hudby
	SOUNDDATA* data = (SOUNDDATA*)MemGet(SIZEOFSOUNDDATA);
	if (data != NULL)
	{

// nastavení dat zvuku
		data->Refer = 1;				// poèet referencí
		data->Size = size;				// velikost dat
		data->Samples = 22050;			// implicitní vzorkovací kmitoèet
		data->Format = WAVE_FORMAT_PCM;	// implicitní formát
		data->Channels = 1;				// implicitnì mono
		data->Bits = 16;				// implicitní poèet bitù
		data->SizeHead = 0;				// velikost záhlaví WAVEFORMATEX

// vytvoøení bufferu dat zvuku
		BYTE* datadata = (BYTE*)MemGet(size);
		data->Data = datadata;

// pøi chybì pamìti zrušení záhlaví zvuku
		if (datadata == NULL)
		{
			MemFree(data);
			data = NULL;
		}
	}
	return data;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení dat zvuku (oddìleno kvùli lepší optimalizaci)

void _fastcall DelSoundData(SOUNDDATA* data)
{
	ASSERT(data != NULL);
	ASSERT(data->Data != NULL);

	MemFree(data->Data);		// zrušení dat zvuku
	MemFree(data);				// zrušení záhlaví zvuku
}

/////////////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CSound::CSound() 
{ 
	attach(&EmptySoundData); 
};

CSound::CSound(const CSound& src) 
{ 
	attach(src.pData); 
};

CSound::CSound(SOUNDDATA* data)
{ 
	attach(data); 
};

CSound::~CSound() 
{ 
	detach(pData);
};


/////////////////////////////////////////////////////////////////////////////
// statický konstruktor a destruktor

void CSound::Init()
{ 
	attach(&EmptySoundData); 
};

void _fastcall CSound::Init(const CSound& src) 
{ 
	attach(src.pData); 
};

void _fastcall CSound::Init(SOUNDDATA* data)
{ 
	attach(data); 
};

bool _fastcall CSound::Init(int size)			// pøi chybì pamìti vrací FALSE, zvuk není vytvoøen
{
	pData = NewSoundData(size);
	return (pData != NULL);
};

void CSound::Term()
{ 
	detach(pData);
};


/////////////////////////////////////////////////////////////////////////////
// vyprázdnìní zvuku (uvolnìní dat)

void CSound::Empty()
{ 
	Term(); 
	Init();
}


/////////////////////////////////////////////////////////////////////////////
// kopie do vlastního bufferu pøed modifikací
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn

bool CSound::CopyWrite()
{
// úschova ukazatelù
	SOUNDDATA* olddata = pData;			// adresa starých dat
	long* refer = &(olddata->Refer);	// poèet referencí

// test, zda je nutné pøivlastnìní
	if (*refer > 1)						// je nìjaký jiný majitel?
	{

// vytvoøení nového bufferu
		SOUNDDATA* newdata = NewSoundData(olddata->Size);
		if (newdata == NULL) return false;

// pøenesení dat
		MemCopy(newdata->Data, olddata->Data, olddata->Size);

// pøenesení parametrù
		newdata->Samples = olddata->Samples;
		newdata->Format = olddata->Format;
		newdata->Channels = olddata->Channels;
		newdata->Bits = olddata->Bits;
		newdata->SizeHead = olddata->SizeHead;

// odpojení starých dat
		detach(olddata);

// pøipojení nových dat
		pData = newdata;
	}

// operace OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// vytvoøení nového zvuku (pøipraveno k zápisu, data jsou náhodná)
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn
// parametry (kromì Size) jsou nedefinované!

bool _fastcall CSound::New(int size)
{
// ukazatel na stará data
	SOUNDDATA* olddata = pData;

// test, zda je nutné vytvoøení nového bufferu
	if ((olddata->Refer > 1) ||		// na buffer je více referencí
		(olddata->Size != size))	// nesouhlasí velikost
	{

// vytvoøení nového bufferu
		SOUNDDATA* newdata = NewSoundData(size);
		if (newdata == NULL) return false;

// odpojení starých dat
		detach(olddata);

// pøipojení nových dat
		pData = newdata;
	}

// operace OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// nastavení velikosti dat
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn

bool CSound::ReSize(int size)
{
	ASSERT(size >= 0);

// test, zda se velikost dat mìní
	if (size == pData->Size) return true;

// kopie pøed zápisem
	if (!CopyWrite()) return false;

// nastavení nové velikosti bufferu
	BYTE* newdata = (BYTE*)MemSize(pData->Data, size);
	if (newdata == NULL) return false;

	pData->Data = newdata;
	pData->Size = size;				// velikost dat
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// naètení zvuku ze souboru (TRUE=operace OK)

int MP3freq[4] = { 44100, 4800, 3200, 0 };
BYTE MP3chan[4] = { 2, 2, 2, 1 };
BYTE MP3rate[2*4*16] = 
{
// verze 0
	0x00,0x04,0x06,0x07,0x08,0x0a,0x0c,0x0e,0x10,0x12,0x14,0x16,0x18,0x1c,0x20,0x00,	// layer 1
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x0a,0x0c,0x0e,0x10,0x12,0x14,0x00,	// layer 2
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x0a,0x0c,0x0e,0x10,0x12,0x14,0x00,	// layer 3
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	// layer 4

// verze 1
	0x00,0x04,0x08,0x0c,0x10,0x14,0x18,0x1c,0x20,0x24,0x28,0x2c,0x30,0x34,0x38,0x00,	// layer 1
	0x00,0x04,0x06,0x07,0x08,0x0a,0x0c,0x0e,0x10,0x14,0x18,0x1c,0x20,0x28,0x30,0x00,	// layer 2
	0x00,0x04,0x05,0x06,0x07,0x08,0x0a,0x0c,0x0e,0x10,0x14,0x18,0x1c,0x20,0x28,0x00,	// layer 3
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00		// layer 4
};

bool CSound::LoadFile(CText jmeno)
{
// otevøení souboru mapovaného do pamìti (uzavøen pøi destrukci!)
	CFileMap file;
	if (!file.Open(jmeno)) return false;
	int size = file.Size();

// záhlaví souboru s popisovaèem formátu
	WAVHEAD* wav = (WAVHEAD*) file.Adr();
	
// kontrola záhlaví souboru s popisovaèem formátu
	if ((size < 40) ||
		(wav->tWavIdent[0] != 'R') ||
		(wav->tWavIdent[1] != 'I') ||
		(wav->tWavIdent[2] != 'F') ||
		(wav->tWavIdent[3] != 'F') ||
		(wav->WavFormat.tFormatIdent[0] != 'W') ||
		(wav->WavFormat.tFormatIdent[1] != 'A') ||
		(wav->WavFormat.tFormatIdent[2] != 'V') ||
		(wav->WavFormat.tFormatIdent[3] != 'E') ||
		(wav->WavFormat.tFormatIdent[4] != 'f') ||
		(wav->WavFormat.tFormatIdent[5] != 'm') ||
		(wav->WavFormat.tFormatIdent[6] != 't') ||
		(wav->WavFormat.tFormatIdent[7] != ' '))
	{

// interpretace jako soubor MP3
		DWORD hdr = *(DWORD*)file.Adr();
		int ver = ((hdr >> 11) & 0x1);
		int layer = 4 - ((hdr >> 9) & 0x3);

		int freq = MP3freq[(hdr >> 18) & 0x3];
		if (ver == 0) freq /= 2;

		int chan = MP3chan[(hdr >> 30 ) & 0x3];
		int rate = MP3rate[(ver*4 + (layer - 1))*16 + ((hdr >> 20) & 0xf)] * 1000;

		if (((hdr & 0xf0ff) == 0xf0ff) &&
			(layer == 3) &&
			(freq > 0) &&
			(rate > 0))
		{

// vytvoøení bufferu souboru
#define MPEG3FORMATSIZE 30
			New(size + MPEG3FORMATSIZE + 16);	// pøehrávaè MPEG potøebuje rezervu za daty
			pData->Size -= 16;
			pData->Samples = freq;				// vzorkovací kmitoèet
			pData->Channels = chan;				// poèet kanálù
			pData->Bits = 0;					// poèet bitù na vzorek
			pData->Format = WAVE_FORMAT_MPEGLAYER3;	// formát
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
			MemCopy(pData->Data + MPEG3FORMATSIZE, file.Adr(), size);

// zkrácení o komentáø na konci souboru
			if ((pData->Size >= (128 + MPEG3FORMATSIZE)) &&
				(pData->Data[pData->Size - 128 + 0] == 'T') &&
				(pData->Data[pData->Size - 128 + 1] == 'A') &&
				(pData->Data[pData->Size - 128 + 2] == 'G'))
			{
				pData->Size -= 128;
			}
			return true;
		}

		return false;
	}

// formát dat souboru
	int formatsize = wav->WavFormat.nFormatSize;	// velikost popisovaèe formátu
	int format = wav->WavFormat.wFormatTag;			// formát dat
	if (formatsize < SIZEOFWAVEFORMATEX) format = WAVE_FORMAT_PCM;	// nejsou doplòující data formátu

// první popisovaè dat
	WAVDATA* dat = (WAVDATA*)((BYTE*)wav + 8 + 12 + formatsize);
	size -= 8 + 12 + formatsize + sizeof(WAVDATA);
	if ((formatsize < 14) || (formatsize > 0x7f00) || (size < 0)) return false;

// cyklus nalezení bloku s daty
	while (	(dat->tDataIdent[0] != 'd') ||
			(dat->tDataIdent[1] != 'a') ||
			(dat->tDataIdent[2] != 't') ||
			(dat->tDataIdent[3] != 'a'))
	{

// posun ukazatele na další záhlaví
		if ((DWORD)dat->nDataSize > 0x7FFF0000) return false;
		size -= dat->nDataSize + sizeof(WAVDATA);
		if (size < 0) return false;
		dat = (WAVDATA*)((BYTE*)dat + sizeof(WAVDATA) + dat->nDataSize);
	}

// pøíprava velikosti dat
	if (size > (long)dat->nDataSize) size = dat->nDataSize;
	if (size < 0) size = 0;

// parametry formátu dat souboru
	int channels = wav->WavFormat.nChannels;		// poèet kanálù
	int samples = wav->WavFormat.nSamplesPerSec;	// vzorkovací kmitoèet
	int bits = wav->WavFormat.wBitsPerSample;		// poèet bitù na vzorek
	if (formatsize < 16) bits = 8;

// korekce formátu dat
	if (format == WAVE_FORMAT_PCM)
	{
		formatsize = 0;
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

// vytvoøení bufferu
	if (!New(size + formatsize)) return false;

// kopie formátu
	if (formatsize > 0)
	{
		MemCopy(pData->Data, &(wav->WavFormat.wFormatTag), formatsize);
		((WAVEFORMATEX*)(pData->Data))->cbSize = (WORD)(formatsize - SIZEOFWAVEFORMATEX);
	}

// kopie dat
	MemCopy(pData->Data + formatsize, (BYTE*)dat + sizeof(WAVDATA), size);

// nastavení parametrù zvuku
	pData->Samples = samples;			// vzorkovací kmitoèet
	pData->Format = format;				// formát
	pData->Channels = channels;			// poèet kanálù
	pData->Bits = bits;					// poèet bitù na vzorek
	pData->SizeHead = formatsize;		// velikost popisovaèe formátu

// pøíznak - naèteno OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// uložení do souboru formátu WAV (FALSE=chyba)

bool CSound::SaveFile(CText jmeno) const
{
// vytvoøení souboru
	CFile file;
	file.Name(jmeno);
	if (!file.Create()) return false;

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
	BOOL result;

	if (pData->Format == WAVE_FORMAT_PCM)
	{
		head.nFileSize = pData->Size + sizeof(WAVHEAD) - 8;

		head.WavFormat.nFormatSize = 16;
		head.WavFormat.wFormatTag = WAVE_FORMAT_PCM;
		head.WavFormat.nChannels = (WORD)pData->Channels;
		head.WavFormat.nSamplesPerSec = pData->Samples;
		head.WavFormat.nAvgBytesPerSec = pData->Samples * pData->Channels * (pData->Bits/8);
		head.WavFormat.nBlockAlign = (WORD)(pData->Channels * (pData->Bits/8));
		head.WavFormat.wBitsPerSample = (WORD)pData->Bits;

		head.WavData.nDataSize = pData->Size;

		result = file.Write(&head, sizeof(WAVHEAD)) &&
					file.Write(pData->Data, pData->Size);
	}

// uložení záhlaví a dat pro jiný formát
	else
	{
		head.nFileSize = pData->Size + 12 + sizeof(WAVDATA);

		head.WavFormat.nFormatSize = pData->SizeHead;

		head.WavData.nDataSize = pData->Size - pData->SizeHead;

		result = file.Write(&head, 8 + 12) &&
					file.Write(pData->Data, pData->SizeHead) &&
					file.Write(&(head.WavData), sizeof(WAVDATA)) &&
					file.Write(pData->Data + pData->SizeHead, pData->Size - pData->SizeHead);
	}

// uzavøení souboru
	file.Close();

// pøi chybì zrušení souboru
	if (!result)
	{
		file.Delete();
		return false;
	}

// pøíznak - uloženo OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// operátor pøiøazení

const CSound& CSound::operator= (const CSound& src)
{
	detach(pData);			// zrušení starých dat
	attach(src.pData);		// pøiøazení nových dat
	return *this;
}

const CSound& CSound::operator= (SOUNDDATA* src)
{
	detach(pData);			// zrušení starých dat
	attach(src);			// pøiøazení nových dat
	return *this;
}


/***************************************************************************\
*																			*
*								Buffer zvuku								*
*																			*
\***************************************************************************/

////////////////////////////////////////////////////////////////////
// pøidání záznamu o vložení položky do UNDO bufferu

template <> bool CBuffer<CSound>::UndoAddIns(int index)
{
	return Undo.AddSndIns(index);
}

////////////////////////////////////////////////////////////////////
// vytvoøení nové položky - obsah náhodný (vrací index položky, <0 = chyba)
// provádí záznam do UNDO bufferu

int CBufSound::New(int size)
{
// vytvoøení nové položky
	int result = New();				// vytvoøení nové položky
	if (result >= 0)
	{

// inicializace objektu
		if (!m_Data[result].New(size))
		{
			Del(result);
			if (m_Undo) Undo.DelAll();
			result = -1;
		}
	}

// index nové položky (nebo <0 = chyba)
	return result;
}

////////////////////////////////////////////////////////////////////
// pøidání položky (vrací index položky, <0 = chyba)
// provádí záznam do UNDO bufferu

int _fastcall CBufSound::Add(SOUNDDATA* data)
{
	CSound snd(data);
	int result = Add(snd);		// result musí být oddìlen, pro inline by se špatnì volal destruktor
	return result;
}
