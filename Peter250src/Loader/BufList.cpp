
#include "Main.h"

/***************************************************************************\
*																			*
*								Buffer seznamù								*
*																			*
\***************************************************************************/

LISTDATA	EmptyListData =	{ 0, 1, 0, 0 };


////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CBufList::CBufList()
{
	m_Data = NULL;			// není buffer dat
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není buffer položek
}

CBufList::~CBufList()
{
	DelAll();				// zrušení všech položek
}


////////////////////////////////////////////////////////////////////
// statický konstruktor a destruktor

void CBufList::Init()
{
	m_Data = NULL;			// není buffer dat
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není buffer položek
}

void CBufList::Term()
{
	DelAll();				// zrušení všech položek
}


////////////////////////////////////////////////////////////////////
// vytvoøení nových dat

void CBufList::NewData()
{
	m_Max *= 2;
	if (m_Max == 0) m_Max = 0x100;
//	m_Max += 0x100;			// zvýšení poètu položek (o 4 KB)
	MemBuf(m_Data, m_Max);	// zvýšení velikosti bufferu
};


////////////////////////////////////////////////////////////////////
// zrušení všech položek v bufferu
 
void CBufList::DelAll()
{
	Del(m_Num);					// zrušení položek
	MemBuf(m_Data, 0);			// zrušení bufferu dat
	m_Max = 0;					// není žádná položka v bufferu
}


////////////////////////////////////////////////////////////////////
// automatická inkrementace indexu (nekontroluje správnost!)
/*
void _fastcall CBufList::AutoInc(int index)
{
// adresa položky seznamu
	LISTDATA* list = m_Data + index;

// test, zda bude inkrementace
	if (list->Auto != 0)
	{

// nový index
		int newindex = list->Index + list->Auto;

// kontrola pøeteèení indexu
		if ((DWORD)newindex >= (DWORD)list->Size)
		{
			while (newindex < 0) newindex += list->Size;
			while (newindex >= list->Size) newindex -= list->Size;
		}

// nastavení nového indexu
		list->Index = newindex;
	}
}
*/

////////////////////////////////////////////////////////////////////
// automatická inkrementace indexu, vrací pùvodní index (nekontroluje správnost!)
/*
int _fastcall CBufList::AutoIncInx(int index)
{
// adresa položky seznamu
	LISTDATA* list = m_Data + index;

// úschova pùvodního indexu
	int result = list->Index;

// test, zda bude inkrementace
	if (list->Auto != 0)
	{

// nový index
		int newindex = result + list->Auto;

// kontrola pøeteèení indexu
		if ((DWORD)newindex >= (DWORD)list->Size)
		{
			while (newindex < 0) newindex += list->Size;
			while (newindex >= list->Size) newindex -= list->Size;
		}

// nastavení nového indexu
		list->Index = newindex;
	}

// návrat pùvodního indexu
	return result;
}
*/
////////////////////////////////////////////////////////////////////
// poskytnutí položky (s kontrolou platnosti indexu)

const LISTDATA& _fastcall CBufList::Get(const int index) const
{
	if (IsValid(index))			// je index platný?
	{
		return m_Data[index];	// položka na daném indexu
	}
	return EmptyListData;
}

////////////////////////////////////////////////////////////////////
// nastavení položky (s kontrolou platnosti indexu)

void _fastcall CBufList::Set(const int index, const LISTDATA& data)
{
	if (IsValid(index))			// je index platný?
	{
		m_Data[index] = data;
	}
}

////////////////////////////////////////////////////////////////////
// zrušení položek z konce bufferu

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
// vytvoøení položky (vrací index položky)

int CBufList::New()
{
	int result = NewItem();		// vytvoøení nové položky
	m_Data[result] = EmptyListData;
	return result;
}


////////////////////////////////////////////////////////////////////
// pøidání položky (vrací index položky)

int _fastcall CBufList::Add(const LISTDATA& data)
{
	int result = NewItem();		// vytvoøení nové položky
	m_Data[result] = data;		// inicializace položky
	return result;
}


////////////////////////////////////////////////////////////////////
// duplikace položky (s kontrolou platnosti indexu, vrací index položky)

int _fastcall CBufList::Dup(const int index)
{
	int result = NewItem();		// vytvoøení nové položky

	if (IsValid(index))			// je index platný?
	{
		m_Data[result] = m_Data[index];		// kopie položky
	}
	else
	{
		m_Data[result] = EmptyListData;		// inicializace neplatné položky
	}
	return result;
}

int _fastcall CBufList::Dup(const int index, int num)
{
	int result = NewItem();		// vytvoøení nové položky

	if (IsValid(index))					// je index platný?
	{
		LISTDATA* data = &m_Data[index];

		m_Data[result] = *data;			// kopie položky

		for (num--; num > 0; num--)
		{
			m_Data[NewItem()] = *data;
		}
	}
	else
	{
		m_Data[result] = EmptyListData;		// inicializace neplatné položky

		for (num--; num > 0; num--)
		{
			m_Data[NewItem()] = EmptyListData;
		}
	}
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// operátor pøiøazení

const CBufList& CBufList::operator= (const CBufList& src)
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
