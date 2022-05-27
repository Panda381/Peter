
#include "Main.h"

/***************************************************************************\
*																			*
*									Plocha									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inicializa�n� pr�zdn� plocha (modifikuje se po�et referenc�!)

MAPDATA*	EmptyMapData =	NULL;


/////////////////////////////////////////////////////////////////////////////
// statick� inicializace ploch

void InitMap()
{
#pragma warning ( disable: 4127)				// hl�en� - konstantn� podm�nka
	ASSERT(SIZEOFMAPITEM == 8);
#pragma warning ( default: 4127)				// hl�en� - konstantn� podm�nka
	EmptyMapData = (MAPDATA*)MemGet(SIZEOFMAPDATA + SIZEOFMAPITEM);
	EmptyMapData->Refer = 1;			// po�et referenc�
	EmptyMapData->Width = 1;			// ���ka
	EmptyMapData->Height = 1;			// v��ka
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
// statick� konstruktor a destruktor

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
// vymaz�n� plochy

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
// kopie do vlastn�ho bufferu p�ed modifikac�

void CMap::CopyWrite()
{
	MAPDATA* data = pData;			// adresa star�ch dat
	long* refer = &(data->Refer);	// po�et referenc�

	if (*refer > 1)					// je n�jak� jin� majitel?
	{
		NewBuffer(data->Width, data->Height);	// vytvo�en� nov�ho bufferu

		MAPITEM* src = data->Data;
		MAPITEM* dst = pData->Data;
		for (int i = data->Width*data->Height; i > 0; i--)
		{
			dst->Icon = src->Icon;
			dst->Param = src->Param;
			dst++;
			src++;
		}

// odpojen� star�ch dat - v multithread m��e nastat i zru�en�
		if (LongDecrement(refer))
		{
#ifdef _MT
			MAPITEM* item = data->Data;
			for (int i = data->Width * data->Height; i > 0; i--)
			{
				item->Icon.Term();
				item++;
			}

			MemFree(data);			// p��padn� zru�en� dat
#endif	// _MT
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// vypr�zdn�n� plochy (uvoln�n� dat)

void CMap::Empty()
{ 
	Detach(); 
	Attach(EmptyMapData); 
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� nov� plochy (p�ipraveno k z�pisu, data jsou n�hodn�)

void CMap::New(int width, int height)
{
	Detach();						// odpojen� star� plochy
	NewBuffer(width, height);		// vytvo�en� nov�ho bufferu
}


/////////////////////////////////////////////////////////////////////////////
// poskytnut� prvku (s kontrolou platnosti offsetu/indexu)

const MAPITEM& _fastcall CMap::Get(const int off) const
{
	if (IsValid(off))				// je offset platn�?
	{
		return pData->Data[off];	// prvek na dan�m offsetu
	}
	return EmptyMapData->Data[0];	// neplatn� offset
}

const MAPITEM& _fastcall CMap::Get(const int x, const int y) const
{
	if (IsValid(x, y))				// je index platn�?
	{
		return pData->Data[x + y*pData->Width];	// prvek
	}
	return EmptyMapData->Data[0];	// neplatn� offset
}


////////////////////////////////////////////////////////////////////
// nastaven� prvku (s kontrolou platnosti offsetu/indexu)

void _fastcall CMap::Set(const int off, const MAPITEM& data)
{
	if (IsValid(off))				// je offset platn�?
	{
		MAPITEM* item = pData->Data + off;
		item->Icon = data.Icon;
		item->Param = data.Param;
	}
}

void _fastcall CMap::Set(const int x, const int y, const MAPITEM& data)
{
	if (IsValid(x, y))				// je index platn�?
	{
		MAPITEM* item = pData->Data + x + y*pData->Width;
		item->Icon = data.Icon;
		item->Param = data.Param;
	}
}


/////////////////////////////////////////////////////////////////////////////
// oper�tor p�i�azen�

const CMap& CMap::operator= (const CMap& src)
{
	Detach();				// zru�en� star�ch dat
	Attach(src.pData);		// p�i�azen� nov�ch dat
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
	m_Data = NULL;			// nen� buffer dat
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
}

CBufMap::~CBufMap()
{
	DelAll();				// zru�en� v�ech polo�ek
}


////////////////////////////////////////////////////////////////////
// statick� konstruktor a destruktor

void CBufMap::Init()
{
	m_Data = NULL;			// nen� buffer dat
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
}

void CBufMap::Term()
{
	DelAll();				// zru�en� v�ech polo�ek
}


////////////////////////////////////////////////////////////////////
// vytvo�en� nov�ch dat

void CBufMap::NewData()
{
	m_Max *= 2;
	if (m_Max == 0) m_Max = 0x400;
//	m_Max += 0x400;			// zv��en� po�tu polo�ek (o 4 KB)
	MemBuf(m_Data, m_Max);	// zv��en� velikosti bufferu
};


////////////////////////////////////////////////////////////////////
// zru�en� v�ech polo�ek v bufferu
 
void CBufMap::DelAll()
{
	Del(m_Num);					// zru�en� polo�ek
	MemBuf(m_Data, 0);			// zru�en� bufferu dat
	m_Max = 0;					// nen� ��dn� polo�ka v bufferu
}

////////////////////////////////////////////////////////////////////
// poskytnut� polo�ky (s kontrolou platnosti indexu)

const CMap& _fastcall CBufMap::Get(const int index) const
{
	if (IsValid(index))			// je index platn�?
	{
		return m_Data[index];	// polo�ka na dan�m indexu
	}
	return EmptyMap;			// pro neplatn� index vr�t� pr�zdnou plochu
}

////////////////////////////////////////////////////////////////////
// nastaven� polo�ky (s kontrolou platnosti indexu)

void _fastcall CBufMap::Set(const int index, const CMap& data)
{
	if (IsValid(index))			// je index platn�?
	{
		m_Data[index] = data;
	}
}


/////////////////////////////////////////////////////////////////////////////
// vypr�zdn�n� polo�ky (bez jej�ho zru�en� - jen pro uvoln�n� dat)

void _fastcall CBufMap::Empty(const int index)
{
	if (IsValid(index))					// je index platn�?
	{
		m_Data[index].Empty();
	}
}


////////////////////////////////////////////////////////////////////
// zru�en� polo�ek z konce bufferu

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
// vytvo�en� polo�ky (vrac� index polo�ky)

int CBufMap::New()
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result].Init();		// inicializace polo�ky
	return result;
}

int CBufMap::New(int width, int height)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result].Init(width, height);	// inicializace polo�ky
	m_Data[result].Clear();		// vymaz�n� plochy
	return result;
}

////////////////////////////////////////////////////////////////////
// p�id�n� polo�ky (vrac� index polo�ky)

int _fastcall CBufMap::Add(const CMap& data)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result].Init(data.Data());	// inicializace polo�ky
	return result;
}

int _fastcall CBufMap::Add(MAPDATA* data)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result].Init(data);	// inicializace polo�ky
	return result;
}


////////////////////////////////////////////////////////////////////
// duplikace polo�ky (s kontrolou platnosti indexu, vrac� index prvn� polo�ky)

int _fastcall CBufMap::Dup(const int index)
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

int _fastcall CBufMap::Dup(const int index, int num)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky

	if (IsValid(index))					// je index platn�?
	{
		MAPDATA* data = m_Data[index].Data();

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

const CBufMap& CBufMap::operator= (const CBufMap& src)
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
