
#include "Main.h"

/***************************************************************************\
*																			*
*							Buffer pøemapování indexù						*
*																			*
\***************************************************************************/


////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CBufIndex::CBufIndex()
{
	m_Data = NULL;			// není buffer dat
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není buffer položek
}

CBufIndex::~CBufIndex()
{
	DelAll();				// zrušení bufferu
}


////////////////////////////////////////////////////////////////////
// statický konstruktor a destruktor

void CBufIndex::Init()
{
	m_Data = NULL;			// není buffer dat
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není buffer položek
}

void CBufIndex::Term()
{
	DelAll();				// zrušení bufferu
}


////////////////////////////////////////////////////////////////////
// zrušení všech položek v bufferu

void CBufIndex::DelAll()
{
	MemFree(m_Data);		// zrušení bufferu dat
	m_Data = NULL;			// pøíznak neplatnosti bufferu
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není žádná položka v bufferu
}


////////////////////////////////////////////////////////////////////
// nastavení poètu položek v bufferu
// (nové položky neinicializované, vrací FALSE=chyba pamìti)

bool _fastcall CBufIndex::Num(const int num)
{
	ASSERT(num >= 0);

// test, zda je nutno zvýšit velikost bufferu
	if (num > m_Max)
	{

// zvìtšení bufferu
		int newmax = (num + 0x400) & ~0xff;
		int* newdata = (int*)MemSize(m_Data, newmax*sizeof(int));
		if (newdata == NULL) return false;
		m_Data = newdata;
		m_Max = newmax;
	}

// nový poèet položek v bufferu
	m_Num = num;
	return true;
}


////////////////////////////////////////////////////////////////////
// nastavení poètu položek s vymazáním "-1", vrací FALSE=chyba pamìti

bool _fastcall CBufIndex::NumClear(const int num)
{
	if (Num(num))
	{
		Clear(-1);
		return true;
	}
	return false;
}


////////////////////////////////////////////////////////////////////
// vymazání všech položek v bufferu zadanou hodnotou

void _fastcall CBufIndex::Clear(const int clear)
{
	int* dst = m_Data;
	for (int i = m_Num; i > 0; i--)
	{
		*dst = clear;
		dst++;
	}
}


////////////////////////////////////////////////////////////////////
// poskytnutí položky (s kontrolou platnosti indexu, pro neplatnou vrátí -1)

int _fastcall CBufIndex::Get(const int index) const
{
	if (IsValid(index))					// je index platný?
	{
		return m_Data[index];
	}
	return -1;
}


////////////////////////////////////////////////////////////////////
// nastavení položky (s kontrolou platnosti indexu)

void _fastcall CBufIndex::Set(const int index, const int data)
{
	if (IsValid(index))					// je index platný?
	{
		m_Data[index] = data;
	}
}


////////////////////////////////////////////////////////////////////
// pøidání položky (vrací index položky, pøi chybì pamìti vrací <0)

int _fastcall CBufIndex::Add(const int data)
{
// index nové položky
	int index = m_Num;

// nastavení nového poètu položek
	if (!Num(index + 1)) return -1;

// inicializace položky
	m_Data[index] = data;

// index nové položky
	return index;
}

/////////////////////////////////////////////////////////////////////////////
// operátor pøiøazení (pøi chybì pamìti poèet položek nesouhlasí)

const CBufIndex& _fastcall CBufIndex::operator= (const CBufIndex& srcbuf)
{
	if (Num(srcbuf.Num()))
	{
		MemCopy(m_Data, srcbuf.Data(), m_Num*sizeof(int));
	}
	return *this;
}
