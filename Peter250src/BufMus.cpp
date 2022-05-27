
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
const CMusic		EmptyMusic;				// prázdná hudba


/////////////////////////////////////////////////////////////////////////////
// vytvoøení dat hudby (pøi chybì pamìti vrací NULL)

MUSICDATA* _fastcall NewMusicData(int size)
{
	ASSERT(size >= 0);

// vytvoøení záhlaví hudby
	MUSICDATA* data = (MUSICDATA*)MemGet(SIZEOFMUSICDATA);
	if (data != NULL)
	{

// nastavení dat hudby
		data->Refer = 1;				// poèet referencí
		data->Size = size;				// velikost dat

// vytvoøení bufferu dat hudby
		BYTE* datadata = (BYTE*)MemGet(size);
		data->Data = datadata;

// pøi chybì pamìti zrušení záhlaví hudby
		if (datadata == NULL)
		{
			MemFree(data);
			data = NULL;
		}
	}
	return data;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení dat hudby (oddìleno kvùli lepší optimalizaci)

void _fastcall DelMusicData(MUSICDATA* data)
{
	ASSERT(data != NULL);
	ASSERT(data->Data != NULL);

	MemFree(data->Data);		// zrušení dat hudby
	MemFree(data);				// zrušení záhlaví hudby
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
// statický konstruktor a destruktor

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

bool CMusic::Init(int size)		// pøi chybì pamìti vrací FALSE, hudba není vytvoøena
{
	pData = NewMusicData(size);
	return (pData != NULL);
};

void CMusic::Term()
{ 
	detach(pData); 
};


/////////////////////////////////////////////////////////////////////////////
// vyprázdnìní hudby (uvolnìní dat) - pøipojí se standardní prázdná hudba

void CMusic::Empty()
{ 
	Term();
	Init();
}


/////////////////////////////////////////////////////////////////////////////
// kopie do vlastního bufferu pøed modifikací
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn

bool CMusic::CopyWrite()
{
// úschova ukazatelù
	MUSICDATA* olddata = pData;			// adresa starých dat
	long* refer = &(olddata->Refer);	// poèet referencí

// test, zda je nutné pøivlastnìní
	if (*refer > 1)						// je nìjaký jiný majitel?
	{

// vytvoøení nového bufferu
		MUSICDATA* newdata = NewMusicData(olddata->Size);
		if (newdata == NULL) return false;

// pøenesení dat
		MemCopy(newdata->Data, olddata->Data, olddata->Size);

// odpojení starých dat
		detach(olddata);

// pøipojení nových dat
		pData = newdata;
	}

// operace OK
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení nové hudby (pøipraveno k zápisu, data jsou náhodná)
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn

bool CMusic::New(int size)
{
// ukazatel na stará data
	MUSICDATA* olddata = pData;

// test, zda je nutné vytvoøení nového bufferu
	if ((olddata->Refer > 1) ||		// na buffer je více referencí
		(olddata->Size != size))	// nesouhlasí velikost
	{

// vytvoøení nového bufferu
		MUSICDATA* newdata = NewMusicData(size);
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
// naètení hudby ze souboru (TRUE=operace OK)

bool CMusic::LoadFile(CText jmeno)
{
// otevøení souboru mapovaného do pamìti (uzavøen pøi destrukci!)
	CFileMap file;
	if (!file.Open(jmeno)) return false;

// vytvoøení nového bufferu
	MUSICDATA* newdata = NewMusicData(file.Size());
	if (newdata == NULL) return false;

// pøenesení dat do bufferu
	MemCopy(newdata->Data, file.Adr(), file.Size());

// odpojení starých dat
	detach(pData);

// pøipojení nových dat
	pData = newdata;

// pøíznak operace OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// uložení do souboru formátu MID (FALSE=chyba)

bool CMusic::SaveFile(CText jmeno) const
{
// vytvoøení souboru
	CFile file;
	file.Name(jmeno);
	if (!file.Create()) return false;

// uložení souboru
	BOOL result = file.Write(pData->Data, pData->Size);

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

const CMusic& CMusic::operator= (const CMusic& src)
{
	detach(pData);			// zrušení starých dat
	attach(src.pData);		// pøiøazení nových dat
	return *this;
}

const CMusic& CMusic::operator= (MUSICDATA* src)
{
	detach(pData);			// zrušení starých dat
	attach(src);			// pøiøazení nových dat
	return *this;
}


/***************************************************************************\
*																			*
*								Buffer hudby								*
*																			*
\***************************************************************************/

////////////////////////////////////////////////////////////////////
// pøidání záznamu o vložení položky do UNDO bufferu

template <> bool CBuffer<CMusic>::UndoAddIns(int index)
{
	return Undo.AddMusIns(index);
}

////////////////////////////////////////////////////////////////////
// vytvoøení nové položky - obsah náhodný (vrací index položky, <0 = chyba)
// provádí záznam do UNDO bufferu

int CBufMusic::New(int size)
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
// pøidání položky (vrací index položky, <0 = chyba pamìti)
// provádí záznam do UNDO bufferu

int _fastcall CBufMusic::Add(MUSICDATA* data)
{
	CMusic mus(data);
	int result = Add(mus);		// result musí být oddìlen, pro inline by se špatnì volal destruktor
	return result;
}
