
#include "Main.h"

/***************************************************************************\
*																			*
*							Buffer reálných èísel							*
*																			*
\***************************************************************************/


////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CBufReal::CBufReal()
{
	m_Data = NULL;			// není buffer dat
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není buffer položek
}

CBufReal::~CBufReal()
{
	DelAll();				// zrušení bufferu
}


////////////////////////////////////////////////////////////////////
// statický konstruktor a destruktor

void CBufReal::Init()
{
	m_Data = NULL;			// není buffer dat
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není buffer položek
}

void CBufReal::Term()
{
	DelAll();				// zrušení bufferu
}


////////////////////////////////////////////////////////////////////
// vytvoøení nových dat (oddìleno kvùli lepší optimalizaci)

void CBufReal::NewData()
{
	m_Max *= 2;
	if (m_Max == 0) m_Max = 0x200;
//	m_Max += 0x200;			// 4 KB
	MemBuf(m_Data, m_Max);
}


////////////////////////////////////////////////////////////////////
// nastavení poètu položek v bufferu (nové položky neinicializované)

void _fastcall CBufReal::Num(int num)
{
	m_Num = num;
	if (num > m_Max)
	{
		num += 0x800;
		num &= ~0x3ff;
		m_Max = num;
		MemBuf(m_Data, num);
	}
}

////////////////////////////////////////////////////////////////////
// zrušení všech položek v bufferu

void CBufReal::DelAll()
{
	MemBuf(m_Data, 0);		// zrušení bufferu dat
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není žádná položka v bufferu
}


////////////////////////////////////////////////////////////////////
// poskytnutí položky (s kontrolou platnosti indexu)

double _fastcall CBufReal::Get(const int index) const
{
	if (IsValid(index))					// je index platný?
	{
		return m_Data[index];
	}
	return 0;
}


////////////////////////////////////////////////////////////////////
// nastavení položky (s kontrolou platnosti indexu)

void _fastcall CBufReal::Set(const int index, const double data)
{
	if (IsValid(index))					// je index platný?
	{
		m_Data[index] = data;
	}
}


////////////////////////////////////////////////////////////////////
// vložení položky do bufferu na požadovanou pozici (omezí index)

void _fastcall CBufReal::Insert(int index, const double data)
{
// omezení indexu
	if ((DWORD)index > (DWORD)m_Num)
	{
		if (index < 0) 
		{
			index = 0;
		}
		else
		{
			index = m_Num;
		}
	}

// poèet položek k odsunu
	int num = m_Num - index;

// zvýšení poètu položek v bufferu
	Num(m_Num + 1);

// odsun položek
	if (num > 0)
	{
		MemMove(m_Data + index + 1, m_Data + index, num*sizeof(double));
	}

// nastavení položky
	m_Data[index] = data;
}


////////////////////////////////////////////////////////////////////
// zrušení položek z konce bufferu

void _fastcall CBufReal::Del(int num)
{
	ASSERT(num >= 0);
	m_Num -= num;
	if (m_Num < 0)
	{
		m_Num = 0;
	}
}


////////////////////////////////////////////////////////////////////
// zrušení položky z bufferu (ostatní položky se pøisunou)

void _fastcall CBufReal::DelItem(const int index)
{
	if (IsValid(index))					// je položka platná?
	{
		m_Num--;
		if (index < m_Num)
		{
			MemCopy(m_Data + index, m_Data + index + 1, (m_Num - index)*sizeof(double));
		}
	}
}

////////////////////////////////////////////////////////////////////
// vytvoøení položky (vrací index položky)

int CBufReal::New()
{
	int result = NewItem();		// vytvoøení nové položky
	m_Data[result] = 0;			// inicializace položky
	return result;
}


////////////////////////////////////////////////////////////////////
// pøidání položky (vrací index položky)

int _fastcall CBufReal::Add(const double data)
{
	int result = NewItem();		// vytvoøení nové položky
	m_Data[result] = data;		// inicializace položky
	return result;
}


////////////////////////////////////////////////////////////////////
// duplikace položky (s kontrolou platnosti indexu, vrací index první položky)

int _fastcall CBufReal::Dup(const int index)
{
	int result = NewItem();				// vytvoøení nové položky

	if (IsValid(index))					// je index platný?
	{
		m_Data[result] = m_Data[index];	// kopie položky
	}
	else
	{
		m_Data[result] = 0;				// inicializace neplatné položky
	}
	return result;
}

int _fastcall CBufReal::Dup(const int index, int num)
{
	int result = NewItem();		// vytvoøení nové položky

	if (IsValid(index))					// je index platný?
	{
		double data = m_Data[index];

		m_Data[result] = data;			// kopie položky

		for (num--; num > 0; num--)
		{
			m_Data[NewItem()] = data;
		}
	}
	else
	{
		m_Data[result] = 0;		// inicializace neplatné položky

		for (num--; num > 0; num--)
		{
			m_Data[NewItem()]= 0;
		}
	}
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// naètení èísla ze souboru (naèítá do promìnné, true=operace OK)
/*
bool CBufReal::LoadFile(double& data, CString jmeno)
{
// naètení textového souboru
	CString txt;
	if (!txt.LoadFile(jmeno)) return false;

// odstranìní mezer a jiných oddìlovaèù
	txt.TrimLeft();
	txt.TrimRight();

// naètení èísla
	data = Double(txt);
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// uložení èísla do souboru formátu NUM (false=chyba)

bool CBufReal::SaveFile(double data, CString jmeno) const
{
// pøíprava èísla
	CString txt;
	txt.Double(data);

// uložení textu do souboru
	return txt.SaveFile(jmeno);
}
*/

/////////////////////////////////////////////////////////////////////////////
// operátor pøiøazení

const CBufReal& CBufReal::operator= (const CBufReal& src)
{
	m_Num = 0;					// zrušení starých dat
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
