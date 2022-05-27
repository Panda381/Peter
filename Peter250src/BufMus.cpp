
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
const CMusic		EmptyMusic;				// pr�zdn� hudba


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� dat hudby (p�i chyb� pam�ti vrac� NULL)

MUSICDATA* _fastcall NewMusicData(int size)
{
	ASSERT(size >= 0);

// vytvo�en� z�hlav� hudby
	MUSICDATA* data = (MUSICDATA*)MemGet(SIZEOFMUSICDATA);
	if (data != NULL)
	{

// nastaven� dat hudby
		data->Refer = 1;				// po�et referenc�
		data->Size = size;				// velikost dat

// vytvo�en� bufferu dat hudby
		BYTE* datadata = (BYTE*)MemGet(size);
		data->Data = datadata;

// p�i chyb� pam�ti zru�en� z�hlav� hudby
		if (datadata == NULL)
		{
			MemFree(data);
			data = NULL;
		}
	}
	return data;
}

/////////////////////////////////////////////////////////////////////////////
// zru�en� dat hudby (odd�leno kv�li lep�� optimalizaci)

void _fastcall DelMusicData(MUSICDATA* data)
{
	ASSERT(data != NULL);
	ASSERT(data->Data != NULL);

	MemFree(data->Data);		// zru�en� dat hudby
	MemFree(data);				// zru�en� z�hlav� hudby
}

/////////////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CMusic::CMusic() 
{ 
	attach(&EmptyMusicData); 
};

CMusic::CMusic(const CMusic& src) 
{ 
	attach(src.pData); 
};

CMusic::CMusic(MUSICDATA* data) 
{ 
	attach(data); 
};

CMusic::~CMusic() 
{ 
	detach(pData);
};


/////////////////////////////////////////////////////////////////////////////
// statick� konstruktor a destruktor

void CMusic::Init()
{ 
	attach(&EmptyMusicData); 
};

void CMusic::Init(const CMusic& src) 
{ 
	attach(src.pData); 
};

void CMusic::Init(MUSICDATA* data)
{ 
	attach(data); 
};

bool CMusic::Init(int size)		// p�i chyb� pam�ti vrac� FALSE, hudba nen� vytvo�ena
{
	pData = NewMusicData(size);
	return (pData != NULL);
};

void CMusic::Term()
{ 
	detach(pData); 
};


/////////////////////////////////////////////////////////////////////////////
// vypr�zdn�n� hudby (uvoln�n� dat) - p�ipoj� se standardn� pr�zdn� hudba

void CMusic::Empty()
{ 
	Term();
	Init();
}


/////////////////////////////////////////////////////////////////////////////
// kopie do vlastn�ho bufferu p�ed modifikac�
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool CMusic::CopyWrite()
{
// �schova ukazatel�
	MUSICDATA* olddata = pData;			// adresa star�ch dat
	long* refer = &(olddata->Refer);	// po�et referenc�

// test, zda je nutn� p�ivlastn�n�
	if (*refer > 1)						// je n�jak� jin� majitel?
	{

// vytvo�en� nov�ho bufferu
		MUSICDATA* newdata = NewMusicData(olddata->Size);
		if (newdata == NULL) return false;

// p�enesen� dat
		MemCopy(newdata->Data, olddata->Data, olddata->Size);

// odpojen� star�ch dat
		detach(olddata);

// p�ipojen� nov�ch dat
		pData = newdata;
	}

// operace OK
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� nov� hudby (p�ipraveno k z�pisu, data jsou n�hodn�)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool CMusic::New(int size)
{
// ukazatel na star� data
	MUSICDATA* olddata = pData;

// test, zda je nutn� vytvo�en� nov�ho bufferu
	if ((olddata->Refer > 1) ||		// na buffer je v�ce referenc�
		(olddata->Size != size))	// nesouhlas� velikost
	{

// vytvo�en� nov�ho bufferu
		MUSICDATA* newdata = NewMusicData(size);
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
// na�ten� hudby ze souboru (TRUE=operace OK)

bool CMusic::LoadFile(CText jmeno)
{
// otev�en� souboru mapovan�ho do pam�ti (uzav�en p�i destrukci!)
	CFileMap file;
	if (!file.Open(jmeno)) return false;

// vytvo�en� nov�ho bufferu
	MUSICDATA* newdata = NewMusicData(file.Size());
	if (newdata == NULL) return false;

// p�enesen� dat do bufferu
	MemCopy(newdata->Data, file.Adr(), file.Size());

// odpojen� star�ch dat
	detach(pData);

// p�ipojen� nov�ch dat
	pData = newdata;

// p��znak operace OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// ulo�en� do souboru form�tu MID (FALSE=chyba)

bool CMusic::SaveFile(CText jmeno) const
{
// vytvo�en� souboru
	CFile file;
	file.Name(jmeno);
	if (!file.Create()) return false;

// ulo�en� souboru
	BOOL result = file.Write(pData->Data, pData->Size);

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

const CMusic& CMusic::operator= (const CMusic& src)
{
	detach(pData);			// zru�en� star�ch dat
	attach(src.pData);		// p�i�azen� nov�ch dat
	return *this;
}

const CMusic& CMusic::operator= (MUSICDATA* src)
{
	detach(pData);			// zru�en� star�ch dat
	attach(src);			// p�i�azen� nov�ch dat
	return *this;
}


/***************************************************************************\
*																			*
*								Buffer hudby								*
*																			*
\***************************************************************************/

////////////////////////////////////////////////////////////////////
// p�id�n� z�znamu o vlo�en� polo�ky do UNDO bufferu

template <> bool CBuffer<CMusic>::UndoAddIns(int index)
{
	return Undo.AddMusIns(index);
}

////////////////////////////////////////////////////////////////////
// vytvo�en� nov� polo�ky - obsah n�hodn� (vrac� index polo�ky, <0 = chyba)
// prov�d� z�znam do UNDO bufferu

int CBufMusic::New(int size)
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
// p�id�n� polo�ky (vrac� index polo�ky, <0 = chyba pam�ti)
// prov�d� z�znam do UNDO bufferu

int _fastcall CBufMusic::Add(MUSICDATA* data)
{
	CMusic mus(data);
	int result = Add(mus);		// result mus� b�t odd�len, pro inline by se �patn� volal destruktor
	return result;
}
