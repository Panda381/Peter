
#include "Main.h"

/***************************************************************************\
*																			*
*									Hudba									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inicializa�n� pr�zdn� hudba (modifikuje se po�et referenc�!)

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
// statick� konstruktor a destruktor

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
// kopie do vlastn�ho bufferu p�ed modifikac�

void CMusic::CopyWrite()
{
	MUSICDATA* data = pData;			// adresa star�ch dat
	long* refer = &(data->Refer);		// po�et referenc�

	if (*refer > 1)						// je n�jak� jin� majitel?
	{
		NewBuffer(data->Size);			// vytvo�en� nov�ho bufferu
		MemCopy(pData->Data, data->Data, pData->Size);
		pData->Res = data->Res;

// odpojen� star�ch dat - v multithread m��e nastat i zru�en�
		if (LongDecrement(refer))
		{
#ifdef _MT
			MemFree(data->Data);		// zru�en� dat
			MemFree(data);				// zru�en� z�hlav�
#endif	// _MT
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// vypr�zdn�n� hudby (uvoln�n� dat)

void CMusic::Empty()
{ 
	Detach(); 
	Attach(&EmptyMusicData); 
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� nov� hudby (p�ipraveno k z�pisu, data jsou n�hodn�)

void CMusic::New(int size)
{
	Detach();						// odpojen� star� hudby
	NewBuffer(size);				// vytvo�en� nov�ho bufferu
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� hudby ze souboru

void CMusic::LoadFile()
{
#ifndef _MINI

// �schova offsetu souboru
	int oldoff = FileReadOff;

// na�ten� z�hlav� souboru
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

// na�ten� souboru RIFF
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

// zji�t�n� konce sekc�
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

// na�ten� dat
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
// ulo�en� do souboru form�tu MID (false=chyba)
/*
bool CMusic::SaveFile(CString jmeno) const
{
// otev�en� souboru
	HANDLE hFile = ::CreateFile(jmeno, GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

// test, zda byl soubor vytvo�en
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

// ulo�en� souboru
	DWORD write;
	BOOL result = ::WriteFile(hFile, pData->Data, pData->Size, &write, NULL);

// uzav�en� souboru
	::CloseHandle(hFile);

// p�i chyb� zru�en� souboru
	if (!result || (write != (DWORD)pData->Size))
	{
		::DeleteFile(jmeno);
		return false;
	}

// p��znak - ulo�eno OK
	return true;
}
*/
/////////////////////////////////////////////////////////////////////////////
// ulo�en� do souboru

void CMusic::SaveFile() const
{
#ifndef _MINI
	FileWriteBlok(pData->Data, pData->Size);
#endif // _MINI
}

/////////////////////////////////////////////////////////////////////////////
// oper�tor p�i�azen�

const CMusic& CMusic::operator= (const CMusic& src)
{
	Detach();				// zru�en� star�ch dat
	Attach(src.pData);		// p�i�azen� nov�ch dat
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
	m_Data = NULL;			// nen� buffer dat
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
}

CBufMusic::~CBufMusic()
{
	DelAll();				// zru�en� v�ech polo�ek
}


////////////////////////////////////////////////////////////////////
// statick� konstruktor a destruktor

void CBufMusic::Init()
{
	m_Data = NULL;			// nen� buffer dat
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
}

void CBufMusic::Term()
{
	DelAll();				// zru�en� v�ech polo�ek
}


////////////////////////////////////////////////////////////////////
// vytvo�en� nov�ch dat (odd�leno kv�li lep�� optimalizaci)

void CBufMusic::NewData()
{
	m_Max *= 2;
	if (m_Max == 0) m_Max = 0x400;
//	m_Max += 0x400;				// 4 KB
	MemBuf(m_Data, m_Max);
}


////////////////////////////////////////////////////////////////////
// zru�en� v�ech polo�ek v bufferu

void CBufMusic::DelAll()
{
	Del(m_Num);
	MemBuf(m_Data, 0);			// zru�en� bufferu dat
	m_Max = 0;					// nen� ��dn� polo�ka v bufferu
}

////////////////////////////////////////////////////////////////////
// poskytnut� polo�ky (s kontrolou platnosti indexu)

const CMusic& _fastcall CBufMusic::Get(const int index) const
{
	if (IsValid(index))			// je index platn�?
	{
		return m_Data[index];	// polo�ka na dan�m indexu
	}
	return EmptyMusic;			// pro neplatn� index vr�t� pr�zdnou hudbu
}

////////////////////////////////////////////////////////////////////
// nastaven� polo�ky (s kontrolou platnosti indexu)

void _fastcall CBufMusic::Set(const int index, const CMusic& data)
{
	if (IsValid(index))			// je index platn�?
	{
		m_Data[index] = data;
	}
}


/////////////////////////////////////////////////////////////////////////////
// vypr�zdn�n� polo�ky (bez jej�ho zru�en� - jen pro uvoln�n� dat)

void _fastcall CBufMusic::Empty(const int index)
{
	if (IsValid(index))					// je index platn�?
	{
		m_Data[index].Empty();
	}
}


////////////////////////////////////////////////////////////////////
// zru�en� polo�ek z konce bufferu

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
// vytvo�en� polo�ky (vrac� index polo�ky)

int CBufMusic::New()
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result].Init();		// inicializace polo�ky
	return result;
}

int CBufMusic::New(int size)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result].Init(size);	// inicializace polo�ky
	return result;
}

////////////////////////////////////////////////////////////////////
// p�id�n� polo�ky (vrac� index polo�ky)

int _fastcall CBufMusic::Add(const CMusic& data)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result].Init(data.Data());	// inicializace polo�ky
	return result;
}

int _fastcall CBufMusic::Add(MUSICDATA* data)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result].Init(data);	// inicializace polo�ky
	return result;
}


////////////////////////////////////////////////////////////////////
// duplikace polo�ky (s kontrolou platnosti indexu, vrac� index prvn� polo�ky)

int _fastcall CBufMusic::Dup(const int index)
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

int _fastcall CBufMusic::Dup(const int index, int num)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky

	if (IsValid(index))					// je index platn�?
	{
		MUSICDATA* data = m_Data[index].Data();

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

const CBufMusic& CBufMusic::operator= (const CBufMusic& src)
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
