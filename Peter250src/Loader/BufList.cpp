
#include "Main.h"

/***************************************************************************\
*																			*
*								Buffer seznam�								*
*																			*
\***************************************************************************/

LISTDATA	EmptyListData =	{ 0, 1, 0, 0 };


////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CBufList::CBufList()
{
	m_Data = NULL;			// nen� buffer dat
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
}

CBufList::~CBufList()
{
	DelAll();				// zru�en� v�ech polo�ek
}


////////////////////////////////////////////////////////////////////
// statick� konstruktor a destruktor

void CBufList::Init()
{
	m_Data = NULL;			// nen� buffer dat
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
}

void CBufList::Term()
{
	DelAll();				// zru�en� v�ech polo�ek
}


////////////////////////////////////////////////////////////////////
// vytvo�en� nov�ch dat

void CBufList::NewData()
{
	m_Max *= 2;
	if (m_Max == 0) m_Max = 0x100;
//	m_Max += 0x100;			// zv��en� po�tu polo�ek (o 4 KB)
	MemBuf(m_Data, m_Max);	// zv��en� velikosti bufferu
};


////////////////////////////////////////////////////////////////////
// zru�en� v�ech polo�ek v bufferu
 
void CBufList::DelAll()
{
	Del(m_Num);					// zru�en� polo�ek
	MemBuf(m_Data, 0);			// zru�en� bufferu dat
	m_Max = 0;					// nen� ��dn� polo�ka v bufferu
}


////////////////////////////////////////////////////////////////////
// automatick� inkrementace indexu (nekontroluje spr�vnost!)
/*
void _fastcall CBufList::AutoInc(int index)
{
// adresa polo�ky seznamu
	LISTDATA* list = m_Data + index;

// test, zda bude inkrementace
	if (list->Auto != 0)
	{

// nov� index
		int newindex = list->Index + list->Auto;

// kontrola p�ete�en� indexu
		if ((DWORD)newindex >= (DWORD)list->Size)
		{
			while (newindex < 0) newindex += list->Size;
			while (newindex >= list->Size) newindex -= list->Size;
		}

// nastaven� nov�ho indexu
		list->Index = newindex;
	}
}
*/

////////////////////////////////////////////////////////////////////
// automatick� inkrementace indexu, vrac� p�vodn� index (nekontroluje spr�vnost!)
/*
int _fastcall CBufList::AutoIncInx(int index)
{
// adresa polo�ky seznamu
	LISTDATA* list = m_Data + index;

// �schova p�vodn�ho indexu
	int result = list->Index;

// test, zda bude inkrementace
	if (list->Auto != 0)
	{

// nov� index
		int newindex = result + list->Auto;

// kontrola p�ete�en� indexu
		if ((DWORD)newindex >= (DWORD)list->Size)
		{
			while (newindex < 0) newindex += list->Size;
			while (newindex >= list->Size) newindex -= list->Size;
		}

// nastaven� nov�ho indexu
		list->Index = newindex;
	}

// n�vrat p�vodn�ho indexu
	return result;
}
*/
////////////////////////////////////////////////////////////////////
// poskytnut� polo�ky (s kontrolou platnosti indexu)

const LISTDATA& _fastcall CBufList::Get(const int index) const
{
	if (IsValid(index))			// je index platn�?
	{
		return m_Data[index];	// polo�ka na dan�m indexu
	}
	return EmptyListData;
}

////////////////////////////////////////////////////////////////////
// nastaven� polo�ky (s kontrolou platnosti indexu)

void _fastcall CBufList::Set(const int index, const LISTDATA& data)
{
	if (IsValid(index))			// je index platn�?
	{
		m_Data[index] = data;
	}
}

////////////////////////////////////////////////////////////////////
// zru�en� polo�ek z konce bufferu

void _fastcall CBufList::Del(int num)
{
	ASSERT(num > 0);
	m_Num -= num;
	if (m_Num < 0)
	{
		m_Num = 0;
	}
}


////////////////////////////////////////////////////////////////////
// vytvo�en� polo�ky (vrac� index polo�ky)

int CBufList::New()
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result] = EmptyListData;
	return result;
}


////////////////////////////////////////////////////////////////////
// p�id�n� polo�ky (vrac� index polo�ky)

int _fastcall CBufList::Add(const LISTDATA& data)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result] = data;		// inicializace polo�ky
	return result;
}


////////////////////////////////////////////////////////////////////
// duplikace polo�ky (s kontrolou platnosti indexu, vrac� index polo�ky)

int _fastcall CBufList::Dup(const int index)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky

	if (IsValid(index))			// je index platn�?
	{
		m_Data[result] = m_Data[index];		// kopie polo�ky
	}
	else
	{
		m_Data[result] = EmptyListData;		// inicializace neplatn� polo�ky
	}
	return result;
}

int _fastcall CBufList::Dup(const int index, int num)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky

	if (IsValid(index))					// je index platn�?
	{
		LISTDATA* data = &m_Data[index];

		m_Data[result] = *data;			// kopie polo�ky

		for (num--; num > 0; num--)
		{
			m_Data[NewItem()] = *data;
		}
	}
	else
	{
		m_Data[result] = EmptyListData;		// inicializace neplatn� polo�ky

		for (num--; num > 0; num--)
		{
			m_Data[NewItem()] = EmptyListData;
		}
	}
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// oper�tor p�i�azen�

const CBufList& CBufList::operator= (const CBufList& src)
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
