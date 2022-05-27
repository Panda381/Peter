
#include "Main.h"

/***************************************************************************\
*																			*
*									Plocha									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inicializaèní prázdná plocha (modifikuje se poèet referencí!)

MAPDATA*	EmptyMapData =	NULL;


/////////////////////////////////////////////////////////////////////////////
// statická inicializace ploch

void InitMap()
{
#pragma warning ( disable: 4127)				// hlášení - konstantní podmínka
	ASSERT(SIZEOFMAPITEM == 8);
#pragma warning ( default: 4127)				// hlášení - konstantní podmínka
	EmptyMapData = (MAPDATA*)MemGet(SIZEOFMAPDATA + SIZEOFMAPITEM);
	EmptyMapData->Refer = 1;			// poèet referencí
	EmptyMapData->Width = 1;			// šíøka
	EmptyMapData->Height = 1;			// výška
	EmptyMapData->Data[0].Icon.Init();	// inicializace ikony
	EmptyMapData->Data[0].Param = 0;	// parametry
}


/////////////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CMap::CMap() 
{ 
	Attach(EmptyMapData); 
};

CMap::CMap(const CMap& src) 
{ 
	Attach(src.pData); 
};

CMap::CMap(int width, int height)
{
	NewBuffer(width, height);
};

CMap::~CMap() 
{ 
	Detach(); 
};


/////////////////////////////////////////////////////////////////////////////
// statický konstruktor a destruktor

void CMap::Init()
{ 
	Attach(EmptyMapData);
};

void CMap::Init(MAPDATA* data)
{ 
	Attach(data); 
};

void CMap::Init(int width, int height)
{
	NewBuffer(width, height);
};

void CMap::Term()
{ 
	Detach(); 
};


/////////////////////////////////////////////////////////////////////////////
// vymazání plochy

void CMap::Clear()
{ 
	MAPITEM* item = pData->Data;
	CIcon icon(Icon.Get(0));
	for (int i = pData->Width*pData->Height; i > 0; i--)
	{
		item->Icon = icon;
		item->Param = 0;
		item++;
	}
}


/////////////////////////////////////////////////////////////////////////////
// kopie do vlastního bufferu pøed modifikací

void CMap::CopyWrite()
{
	MAPDATA* data = pData;			// adresa starých dat
	long* refer = &(data->Refer);	// poèet referencí

	if (*refer > 1)					// je nìjaký jiný majitel?
	{
		NewBuffer(data->Width, data->Height);	// vytvoøení nového bufferu

		MAPITEM* src = data->Data;
		MAPITEM* dst = pData->Data;
		for (int i = data->Width*data->Height; i > 0; i--)
		{
			dst->Icon = src->Icon;
			dst->Param = src->Param;
			dst++;
			src++;
		}

// odpojení starých dat - v multithread mùže nastat i zrušení
		if (LongDecrement(refer))
		{
#ifdef _MT
			MAPITEM* item = data->Data;
			for (int i = data->Width * data->Height; i > 0; i--)
			{
				item->Icon.Term();
				item++;
			}

			MemFree(data);			// pøípadné zrušení dat
#endif	// _MT
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// vyprázdnìní plochy (uvolnìní dat)

void CMap::Empty()
{ 
	Detach(); 
	Attach(EmptyMapData); 
}


/////////////////////////////////////////////////////////////////////////////
// vytvoøení nové plochy (pøipraveno k zápisu, data jsou náhodná)

void CMap::New(int width, int height)
{
	Detach();						// odpojení staré plochy
	NewBuffer(width, height);		// vytvoøení nového bufferu
}


/////////////////////////////////////////////////////////////////////////////
// poskytnutí prvku (s kontrolou platnosti offsetu/indexu)

const MAPITEM& _fastcall CMap::Get(const int off) const
{
	if (IsValid(off))				// je offset platný?
	{
		return pData->Data[off];	// prvek na daném offsetu
	}
	return EmptyMapData->Data[0];	// neplatný offset
}

const MAPITEM& _fastcall CMap::Get(const int x, const int y) const
{
	if (IsValid(x, y))				// je index platný?
	{
		return pData->Data[x + y*pData->Width];	// prvek
	}
	return EmptyMapData->Data[0];	// neplatný offset
}


////////////////////////////////////////////////////////////////////
// nastavení prvku (s kontrolou platnosti offsetu/indexu)

void _fastcall CMap::Set(const int off, const MAPITEM& data)
{
	if (IsValid(off))				// je offset platný?
	{
		MAPITEM* item = pData->Data + off;
		item->Icon = data.Icon;
		item->Param = data.Param;
	}
}

void _fastcall CMap::Set(const int x, const int y, const MAPITEM& data)
{
	if (IsValid(x, y))				// je index platný?
	{
		MAPITEM* item = pData->Data + x + y*pData->Width;
		item->Icon = data.Icon;
		item->Param = data.Param;
	}
}


/////////////////////////////////////////////////////////////////////////////
// operátor pøiøazení

const CMap& CMap::operator= (const CMap& src)
{
	Detach();				// zrušení starých dat
	Attach(src.pData);		// pøiøazení nových dat
	return *this;
}


/***************************************************************************\
*																			*
*								Buffer ploch								*
*																			*
\***************************************************************************/

////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CBufMap::CBufMap()
{
	m_Data = NULL;			// není buffer dat
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není buffer položek
}

CBufMap::~CBufMap()
{
	DelAll();				// zrušení všech položek
}


////////////////////////////////////////////////////////////////////
// statický konstruktor a destruktor

void CBufMap::Init()
{
	m_Data = NULL;			// není buffer dat
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není buffer položek
}

void CBufMap::Term()
{
	DelAll();				// zrušení všech položek
}


////////////////////////////////////////////////////////////////////
// vytvoøení nových dat

void CBufMap::NewData()
{
	m_Max *= 2;
	if (m_Max == 0) m_Max = 0x400;
//	m_Max += 0x400;			// zvýšení poètu položek (o 4 KB)
	MemBuf(m_Data, m_Max);	// zvýšení velikosti bufferu
};


////////////////////////////////////////////////////////////////////
// zrušení všech položek v bufferu
 
void CBufMap::DelAll()
{
	Del(m_Num);					// zrušení položek
	MemBuf(m_Data, 0);			// zrušení bufferu dat
	m_Max = 0;					// není žádná položka v bufferu
}

////////////////////////////////////////////////////////////////////
// poskytnutí položky (s kontrolou platnosti indexu)

const CMap& _fastcall CBufMap::Get(const int index) const
{
	if (IsValid(index))			// je index platný?
	{
		return m_Data[index];	// položka na daném indexu
	}
	return EmptyMap;			// pro neplatný index vrátí prázdnou plochu
}

////////////////////////////////////////////////////////////////////
// nastavení položky (s kontrolou platnosti indexu)

void _fastcall CBufMap::Set(const int index, const CMap& data)
{
	if (IsValid(index))			// je index platný?
	{
		m_Data[index] = data;
	}
}


/////////////////////////////////////////////////////////////////////////////
// vyprázdnìní položky (bez jejího zrušení - jen pro uvolnìní dat)

void _fastcall CBufMap::Empty(const int index)
{
	if (IsValid(index))					// je index platný?
	{
		m_Data[index].Empty();
	}
}


////////////////////////////////////////////////////////////////////
// zrušení položek z konce bufferu

void _fastcall CBufMap::Del(int num)
{
	if (num > m_Num) num = m_Num;

	CMap* map = m_Data + m_Num - 1;
	m_Num -= num;

	for (; num > 0; num--)
	{
		map->Term();
		map--;
	}
}


////////////////////////////////////////////////////////////////////
// vytvoøení položky (vrací index položky)

int CBufMap::New()
{
	int result = NewItem();		// vytvoøení nové položky
	m_Data[result].Init();		// inicializace položky
	return result;
}

int CBufMap::New(int width, int height)
{
	int result = NewItem();		// vytvoøení nové položky
	m_Data[result].Init(width, height);	// inicializace položky
	m_Data[result].Clear();		// vymazání plochy
	return result;
}

////////////////////////////////////////////////////////////////////
// pøidání položky (vrací index položky)

int _fastcall CBufMap::Add(const CMap& data)
{
	int result = NewItem();		// vytvoøení nové položky
	m_Data[result].Init(data.Data());	// inicializace položky
	return result;
}

int _fastcall CBufMap::Add(MAPDATA* data)
{
	int result = NewItem();		// vytvoøení nové položky
	m_Data[result].Init(data);	// inicializace položky
	return result;
}


////////////////////////////////////////////////////////////////////
// duplikace položky (s kontrolou platnosti indexu, vrací index první položky)

int _fastcall CBufMap::Dup(const int index)
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

int _fastcall CBufMap::Dup(const int index, int num)
{
	int result = NewItem();		// vytvoøení nové položky

	if (IsValid(index))					// je index platný?
	{
		MAPDATA* data = m_Data[index].Data();

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

const CBufMap& CBufMap::operator= (const CBufMap& src)
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
