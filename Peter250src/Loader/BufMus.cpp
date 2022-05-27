
#include "Main.h"

/***************************************************************************\
*																			*
*									Hudba									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inicializaèní prázdná hudba (modifikuje se poèet referencí!)

BYTE	EmptyMusicData0 = 0;

MUSICDATA EmptyMusicData = { 1, 0, 0, &EmptyMusicData0 };


/////////////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CMusic::CMusic() 
{ 
	Attach(&EmptyMusicData); 
};

CMusic::CMusic(const CMusic& src) 
{ 
	Attach(src.pData); 
};

CMusic::CMusic(int size)
{
	NewBuffer(size);
};

CMusic::~CMusic() 
{ 
	Detach(); 
};


/////////////////////////////////////////////////////////////////////////////
// statický konstruktor a destruktor

void CMusic::Init()
{ 
	Attach(&EmptyMusicData); 
};

void CMusic::Init(MUSICDATA* data)
{ 
	Attach(data); 
};

void CMusic::Init(int size)
{
	NewBuffer(size);
};

void CMusic::Term()
{ 
	Detach(); 
};


/////////////////////////////////////////////////////////////////////////////
// kopie do vlastního bufferu pøed modifikací

void CMusic::CopyWrite()
{
	MUSICDATA* data = pData;			// adresa starých dat
	long* refer = &(data->Refer);		// poèet referencí

	if (*refer > 1)						// je nìjaký jiný majitel?
	{
		NewBuffer(data->Size);			// vytvoøení nového bufferu
		MemCopy(pData->Data, data->Data, pData->Size);
		pData->Res = data->Res;

// odpojení starých dat - v multithread mùže nastat i zrušení
		if (LongDecrement(refer))
		{
#ifdef _MT
			MemFree(data->Data);		// zrušení dat
			MemFree(data);				// zrušení záhlaví
#endif	// _MT
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// vyprázdnìní hudby (uvolnìní dat)

void CMusic::Empty()
{ 
	Detach(); 
	Attach(&EmptyMusicData); 
}


/////////////////////////////////////////////////////////////////////////////
// vytvoøení nové hudby (pøipraveno k zápisu, data jsou náhodná)

void CMusic::New(int size)
{
	Detach();						// odpojení staré hudby
	NewBuffer(size);				// vytvoøení nového bufferu
}


/////////////////////////////////////////////////////////////////////////////
// naètení hudby ze souboru

void CMusic::LoadFile()
{
#ifndef _MINI

// úschova offsetu souboru
	int oldoff = FileReadOff;

// naètení záhlaví souboru
	BYTE buf[16];
	buf[0] = 0;
	FileReadBlok(buf, 16);

// test,zda je soubor RIFF
	int size = *(int*)(buf + 4) + 8;
	if ((buf[0] == 'R') &&
		(buf[1] == 'I') &&
		(buf[2] == 'F') &&
		(buf[3] == 'F') &&
		((DWORD)size < 100000000) &&
		(buf[8] == 'R') &&
		(buf[9] == 'M') &&
		(buf[10] == 'I') &&
		(buf[11] == 'D') &&
		(buf[12] == 'd') &&
		(buf[13] == 'a') &&
		(buf[14] == 't') &&
		(buf[15] == 'a'))

// naètení souboru RIFF
	{
		FileReadOff = oldoff;
		New(size);
		FileReadBlok(pData->Data, size);
		return;
	}

// test,zda je soubor MID
	int count = buf[11] + 256*buf[10];
	if ((buf[0] != 'M') ||
		(buf[1] != 'T') ||
		(buf[2] != 'h') ||
		(buf[3] != 'd') ||
		(count <= 0) ||
		(count > 1000))
	{
		FileReadOff = oldoff;
		FileError = true;
		return;
	}

// zjištìní konce sekcí
	FileReadOff = oldoff + 14;
	for (; count > 0; count--)
	{
		MemFill(buf, 8);
		FileReadBlok(buf, 8);
		if ((buf[0] != 'M') ||
			(buf[1] != 'T') ||
			(buf[2] != 'r') ||
			(buf[3] != 'k'))
		{
			FileReadOff = oldoff;
			FileError = true;
			return;
		}
		size = buf[7] + 256*(buf[6] + 256*(buf[5] + 256*buf[4]));
		FileReadOff += size;
	}

// naètení dat
	size = FileReadOff - oldoff;
	FileReadOff = oldoff;
	if ((size <= 16) || (size > 100000000))
	{
		FileError = true;
		return;
	}
	New(size);
	FileReadBlok(pData->Data, size);

#endif // _MINI
}


/////////////////////////////////////////////////////////////////////////////
// uložení do souboru formátu MID (false=chyba)
/*
bool CMusic::SaveFile(CString jmeno) const
{
// otevøení souboru
	HANDLE hFile = ::CreateFile(jmeno, GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

// test, zda byl soubor vytvoøen
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

// uložení souboru
	DWORD write;
	BOOL result = ::WriteFile(hFile, pData->Data, pData->Size, &write, NULL);

// uzavøení souboru
	::CloseHandle(hFile);

// pøi chybì zrušení souboru
	if (!result || (write != (DWORD)pData->Size))
	{
		::DeleteFile(jmeno);
		return false;
	}

// pøíznak - uloženo OK
	return true;
}
*/
/////////////////////////////////////////////////////////////////////////////
// uložení do souboru

void CMusic::SaveFile() const
{
#ifndef _MINI
	FileWriteBlok(pData->Data, pData->Size);
#endif // _MINI
}

/////////////////////////////////////////////////////////////////////////////
// operátor pøiøazení

const CMusic& CMusic::operator= (const CMusic& src)
{
	Detach();				// zrušení starých dat
	Attach(src.pData);		// pøiøazení nových dat
	return *this;
}


/***************************************************************************\
*																			*
*								Buffer hudby								*
*																			*
\***************************************************************************/


////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CBufMusic::CBufMusic()
{
	m_Data = NULL;			// není buffer dat
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není buffer položek
}

CBufMusic::~CBufMusic()
{
	DelAll();				// zrušení všech položek
}


////////////////////////////////////////////////////////////////////
// statický konstruktor a destruktor

void CBufMusic::Init()
{
	m_Data = NULL;			// není buffer dat
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není buffer položek
}

void CBufMusic::Term()
{
	DelAll();				// zrušení všech položek
}


////////////////////////////////////////////////////////////////////
// vytvoøení nových dat (oddìleno kvùli lepší optimalizaci)

void CBufMusic::NewData()
{
	m_Max *= 2;
	if (m_Max == 0) m_Max = 0x400;
//	m_Max += 0x400;				// 4 KB
	MemBuf(m_Data, m_Max);
}


////////////////////////////////////////////////////////////////////
// zrušení všech položek v bufferu

void CBufMusic::DelAll()
{
	Del(m_Num);
	MemBuf(m_Data, 0);			// zrušení bufferu dat
	m_Max = 0;					// není žádná položka v bufferu
}

////////////////////////////////////////////////////////////////////
// poskytnutí položky (s kontrolou platnosti indexu)

const CMusic& _fastcall CBufMusic::Get(const int index) const
{
	if (IsValid(index))			// je index platný?
	{
		return m_Data[index];	// položka na daném indexu
	}
	return EmptyMusic;			// pro neplatný index vrátí prázdnou hudbu
}

////////////////////////////////////////////////////////////////////
// nastavení položky (s kontrolou platnosti indexu)

void _fastcall CBufMusic::Set(const int index, const CMusic& data)
{
	if (IsValid(index))			// je index platný?
	{
		m_Data[index] = data;
	}
}


/////////////////////////////////////////////////////////////////////////////
// vyprázdnìní položky (bez jejího zrušení - jen pro uvolnìní dat)

void _fastcall CBufMusic::Empty(const int index)
{
	if (IsValid(index))					// je index platný?
	{
		m_Data[index].Empty();
	}
}


////////////////////////////////////////////////////////////////////
// zrušení položek z konce bufferu

void _fastcall CBufMusic::Del(int num)
{
	if (num > m_Num) num = m_Num;

	CMusic* mus = m_Data + m_Num - 1;
	m_Num -= num;

	for (; num > 0; num--)
	{
		mus->Term();
		mus--;
	}
}


////////////////////////////////////////////////////////////////////
// vytvoøení položky (vrací index položky)

int CBufMusic::New()
{
	int result = NewItem();		// vytvoøení nové položky
	m_Data[result].Init();		// inicializace položky
	return result;
}

int CBufMusic::New(int size)
{
	int result = NewItem();		// vytvoøení nové položky
	m_Data[result].Init(size);	// inicializace položky
	return result;
}

////////////////////////////////////////////////////////////////////
// pøidání položky (vrací index položky)

int _fastcall CBufMusic::Add(const CMusic& data)
{
	int result = NewItem();		// vytvoøení nové položky
	m_Data[result].Init(data.Data());	// inicializace položky
	return result;
}

int _fastcall CBufMusic::Add(MUSICDATA* data)
{
	int result = NewItem();		// vytvoøení nové položky
	m_Data[result].Init(data);	// inicializace položky
	return result;
}


////////////////////////////////////////////////////////////////////
// duplikace položky (s kontrolou platnosti indexu, vrací index první položky)

int _fastcall CBufMusic::Dup(const int index)
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

int _fastcall CBufMusic::Dup(const int index, int num)
{
	int result = NewItem();		// vytvoøení nové položky

	if (IsValid(index))					// je index platný?
	{
		MUSICDATA* data = m_Data[index].Data();

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

const CBufMusic& CBufMusic::operator= (const CBufMusic& src)
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
