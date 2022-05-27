
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
	22050,							// vzorkovac� kmito�et
	WAVE_FORMAT_PCM,				// form�t
	1,								// po�et kan�l�
	16,								// po�et bit�
	0,								// velikost z�hlav� WAVEFORMATEX
	&EmptySoundData0				// adresa dat
};

const CSound		EmptySound;		// pr�zdn� zvuk

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� dat zvuku (p�i chyb� pam�ti vrac� NULL)

SOUNDDATA* _fastcall NewSoundData(int size)
{
	ASSERT(size >= 0);

// vytvo�en� z�hlav� hudby
	SOUNDDATA* data = (SOUNDDATA*)MemGet(SIZEOFSOUNDDATA);
	if (data != NULL)
	{

// nastaven� dat zvuku
		data->Refer = 1;				// po�et referenc�
		data->Size = size;				// velikost dat
		data->Samples = 22050;			// implicitn� vzorkovac� kmito�et
		data->Format = WAVE_FORMAT_PCM;	// implicitn� form�t
		data->Channels = 1;				// implicitn� mono
		data->Bits = 16;				// implicitn� po�et bit�
		data->SizeHead = 0;				// velikost z�hlav� WAVEFORMATEX

// vytvo�en� bufferu dat zvuku
		BYTE* datadata = (BYTE*)MemGet(size);
		data->Data = datadata;

// p�i chyb� pam�ti zru�en� z�hlav� zvuku
		if (datadata == NULL)
		{
			MemFree(data);
			data = NULL;
		}
	}
	return data;
}

/////////////////////////////////////////////////////////////////////////////
// zru�en� dat zvuku (odd�leno kv�li lep�� optimalizaci)

void _fastcall DelSoundData(SOUNDDATA* data)
{
	ASSERT(data != NULL);
	ASSERT(data->Data != NULL);

	MemFree(data->Data);		// zru�en� dat zvuku
	MemFree(data);				// zru�en� z�hlav� zvuku
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
// statick� konstruktor a destruktor

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

bool _fastcall CSound::Init(int size)			// p�i chyb� pam�ti vrac� FALSE, zvuk nen� vytvo�en
{
	pData = NewSoundData(size);
	return (pData != NULL);
};

void CSound::Term()
{ 
	detach(pData);
};


/////////////////////////////////////////////////////////////////////////////
// vypr�zdn�n� zvuku (uvoln�n� dat)

void CSound::Empty()
{ 
	Term(); 
	Init();
}


/////////////////////////////////////////////////////////////////////////////
// kopie do vlastn�ho bufferu p�ed modifikac�
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool CSound::CopyWrite()
{
// �schova ukazatel�
	SOUNDDATA* olddata = pData;			// adresa star�ch dat
	long* refer = &(olddata->Refer);	// po�et referenc�

// test, zda je nutn� p�ivlastn�n�
	if (*refer > 1)						// je n�jak� jin� majitel?
	{

// vytvo�en� nov�ho bufferu
		SOUNDDATA* newdata = NewSoundData(olddata->Size);
		if (newdata == NULL) return false;

// p�enesen� dat
		MemCopy(newdata->Data, olddata->Data, olddata->Size);

// p�enesen� parametr�
		newdata->Samples = olddata->Samples;
		newdata->Format = olddata->Format;
		newdata->Channels = olddata->Channels;
		newdata->Bits = olddata->Bits;
		newdata->SizeHead = olddata->SizeHead;

// odpojen� star�ch dat
		detach(olddata);

// p�ipojen� nov�ch dat
		pData = newdata;
	}

// operace OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� nov�ho zvuku (p�ipraveno k z�pisu, data jsou n�hodn�)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
// parametry (krom� Size) jsou nedefinovan�!

bool _fastcall CSound::New(int size)
{
// ukazatel na star� data
	SOUNDDATA* olddata = pData;

// test, zda je nutn� vytvo�en� nov�ho bufferu
	if ((olddata->Refer > 1) ||		// na buffer je v�ce referenc�
		(olddata->Size != size))	// nesouhlas� velikost
	{

// vytvo�en� nov�ho bufferu
		SOUNDDATA* newdata = NewSoundData(size);
		if (newdata == NULL) return false;

// odpojen� star�ch dat
		detach(olddata);

// p�ipojen� nov�ch dat
		pData = newdata;
	}

// operace OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� velikosti dat
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool CSound::ReSize(int size)
{
	ASSERT(size >= 0);

// test, zda se velikost dat m�n�
	if (size == pData->Size) return true;

// kopie p�ed z�pisem
	if (!CopyWrite()) return false;

// nastaven� nov� velikosti bufferu
	BYTE* newdata = (BYTE*)MemSize(pData->Data, size);
	if (newdata == NULL) return false;

	pData->Data = newdata;
	pData->Size = size;				// velikost dat
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� zvuku ze souboru (TRUE=operace OK)

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
// otev�en� souboru mapovan�ho do pam�ti (uzav�en p�i destrukci!)
	CFileMap file;
	if (!file.Open(jmeno)) return false;
	int size = file.Size();

// z�hlav� souboru s popisova�em form�tu
	WAVHEAD* wav = (WAVHEAD*) file.Adr();
	
// kontrola z�hlav� souboru s popisova�em form�tu
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

// vytvo�en� bufferu souboru
#define MPEG3FORMATSIZE 30
			New(size + MPEG3FORMATSIZE + 16);	// p�ehr�va� MPEG pot�ebuje rezervu za daty
			pData->Size -= 16;
			pData->Samples = freq;				// vzorkovac� kmito�et
			pData->Channels = chan;				// po�et kan�l�
			pData->Bits = 0;					// po�et bit� na vzorek
			pData->Format = WAVE_FORMAT_MPEGLAYER3;	// form�t
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
			MemCopy(pData->Data + MPEG3FORMATSIZE, file.Adr(), size);

// zkr�cen� o koment�� na konci souboru
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

// form�t dat souboru
	int formatsize = wav->WavFormat.nFormatSize;	// velikost popisova�e form�tu
	int format = wav->WavFormat.wFormatTag;			// form�t dat
	if (formatsize < SIZEOFWAVEFORMATEX) format = WAVE_FORMAT_PCM;	// nejsou dopl�uj�c� data form�tu

// prvn� popisova� dat
	WAVDATA* dat = (WAVDATA*)((BYTE*)wav + 8 + 12 + formatsize);
	size -= 8 + 12 + formatsize + sizeof(WAVDATA);
	if ((formatsize < 14) || (formatsize > 0x7f00) || (size < 0)) return false;

// cyklus nalezen� bloku s daty
	while (	(dat->tDataIdent[0] != 'd') ||
			(dat->tDataIdent[1] != 'a') ||
			(dat->tDataIdent[2] != 't') ||
			(dat->tDataIdent[3] != 'a'))
	{

// posun ukazatele na dal�� z�hlav�
		if ((DWORD)dat->nDataSize > 0x7FFF0000) return false;
		size -= dat->nDataSize + sizeof(WAVDATA);
		if (size < 0) return false;
		dat = (WAVDATA*)((BYTE*)dat + sizeof(WAVDATA) + dat->nDataSize);
	}

// p��prava velikosti dat
	if (size > (long)dat->nDataSize) size = dat->nDataSize;
	if (size < 0) size = 0;

// parametry form�tu dat souboru
	int channels = wav->WavFormat.nChannels;		// po�et kan�l�
	int samples = wav->WavFormat.nSamplesPerSec;	// vzorkovac� kmito�et
	int bits = wav->WavFormat.wBitsPerSample;		// po�et bit� na vzorek
	if (formatsize < 16) bits = 8;

// korekce form�tu dat
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

// vytvo�en� bufferu
	if (!New(size + formatsize)) return false;

// kopie form�tu
	if (formatsize > 0)
	{
		MemCopy(pData->Data, &(wav->WavFormat.wFormatTag), formatsize);
		((WAVEFORMATEX*)(pData->Data))->cbSize = (WORD)(formatsize - SIZEOFWAVEFORMATEX);
	}

// kopie dat
	MemCopy(pData->Data + formatsize, (BYTE*)dat + sizeof(WAVDATA), size);

// nastaven� parametr� zvuku
	pData->Samples = samples;			// vzorkovac� kmito�et
	pData->Format = format;				// form�t
	pData->Channels = channels;			// po�et kan�l�
	pData->Bits = bits;					// po�et bit� na vzorek
	pData->SizeHead = formatsize;		// velikost popisova�e form�tu

// p��znak - na�teno OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// ulo�en� do souboru form�tu WAV (FALSE=chyba)

bool CSound::SaveFile(CText jmeno) const
{
// vytvo�en� souboru
	CFile file;
	file.Name(jmeno);
	if (!file.Create()) return false;

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

// ulo�en� z�hlav� a dat pro jin� form�t
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

// uzav�en� souboru
	file.Close();

// p�i chyb� zru�en� souboru
	if (!result)
	{
		file.Delete();
		return false;
	}

// p��znak - ulo�eno OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// oper�tor p�i�azen�

const CSound& CSound::operator= (const CSound& src)
{
	detach(pData);			// zru�en� star�ch dat
	attach(src.pData);		// p�i�azen� nov�ch dat
	return *this;
}

const CSound& CSound::operator= (SOUNDDATA* src)
{
	detach(pData);			// zru�en� star�ch dat
	attach(src);			// p�i�azen� nov�ch dat
	return *this;
}


/***************************************************************************\
*																			*
*								Buffer zvuku								*
*																			*
\***************************************************************************/

////////////////////////////////////////////////////////////////////
// p�id�n� z�znamu o vlo�en� polo�ky do UNDO bufferu

template <> bool CBuffer<CSound>::UndoAddIns(int index)
{
	return Undo.AddSndIns(index);
}

////////////////////////////////////////////////////////////////////
// vytvo�en� nov� polo�ky - obsah n�hodn� (vrac� index polo�ky, <0 = chyba)
// prov�d� z�znam do UNDO bufferu

int CBufSound::New(int size)
{
// vytvo�en� nov� polo�ky
	int result = New();				// vytvo�en� nov� polo�ky
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

// index nov� polo�ky (nebo <0 = chyba)
	return result;
}

////////////////////////////////////////////////////////////////////
// p�id�n� polo�ky (vrac� index polo�ky, <0 = chyba)
// prov�d� z�znam do UNDO bufferu

int _fastcall CBufSound::Add(SOUNDDATA* data)
{
	CSound snd(data);
	int result = Add(snd);		// result mus� b�t odd�len, pro inline by se �patn� volal destruktor
	return result;
}
