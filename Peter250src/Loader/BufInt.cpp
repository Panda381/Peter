
#include "Main.h"

/***************************************************************************\
*																			*
*								Buffer celých èísel							*
*																			*
\***************************************************************************/


////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CBufInt::CBufInt()
{
	m_Data = NULL;			// není buffer dat
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není buffer položek
}

CBufInt::CBufInt(int num)
{
	m_Data = NULL;			// není buffer dat
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není buffer položek
	Num(num);				// vytvoøení bufferu indexù
	Clear(-1);				// vymazání bufferu
}

CBufInt::~CBufInt()
{
	DelAll();				// zrušení bufferu
}


////////////////////////////////////////////////////////////////////
// statický konstruktor a destruktor

void CBufInt::Init()
{
	m_Data = NULL;			// není buffer dat
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není buffer položek
}

void CBufInt::Init(int num)
{
	m_Data = NULL;			// není buffer dat
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není buffer položek
	Num(num);				// vytvoøení bufferu indexù
	Clear(-1);				// vymazání bufferu
}

void CBufInt::Term()
{
	DelAll();				// zrušení bufferu
}


////////////////////////////////////////////////////////////////////
// vytvoøení nových dat (oddìleno kvùli lepší optimalizaci)

void CBufInt::NewData()
{
	m_Max *= 2;
	if (m_Max == 0) m_Max = 0x400;
//	m_Max += 0x400;			// 4 KB
	MemBuf(m_Data, m_Max);
}


////////////////////////////////////////////////////////////////////
// zrušení všech položek v bufferu

void CBufInt::DelAll()
{
	MemBuf(m_Data, 0);		// zrušení bufferu dat
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není žádná položka v bufferu
}


////////////////////////////////////////////////////////////////////
// nastavení poètu položek v bufferu (nové položky neinicializované)

void _fastcall CBufInt::Num(int num)
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
// vymazání všech položek v bufferu zadanou hodnotou

void _fastcall CBufInt::Clear(const int clear)
{
	int* dst = m_Data;
	for (int i = m_Num; i > 0; i--)
	{
		*dst = clear;
		dst++;
	}
}


////////////////////////////////////////////////////////////////////
// poskytnutí položky (s kontrolou platnosti indexu)

int _fastcall CBufInt::Get(const int index) const
{
	if (IsValid(index))					// je index platný?
	{
		return m_Data[index];
	}
	return 0;
}


////////////////////////////////////////////////////////////////////
// nastavení položky (s kontrolou platnosti indexu)

void _fastcall CBufInt::Set(const int index, const int data)
{
	if (IsValid(index))					// je index platný?
	{
		m_Data[index] = data;
	}
}


////////////////////////////////////////////////////////////////////
// vložení položky do bufferu na požadovanou pozici (omezí index)

void _fastcall CBufInt::Insert(int index, const int data)
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
		MemMove(m_Data + index + 1, m_Data + index, num*sizeof(int));
	}

// nastavení položky
	m_Data[index] = data;
}


////////////////////////////////////////////////////////////////////
// zrušení položek z konce bufferu

void _fastcall CBufInt::Del(int num)
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

void _fastcall CBufInt::DelItem(const int index)
{
	if (IsValid(index))					// je položka platná?
	{
		m_Num--;
		if (index < m_Num)
		{
			MemCopy(m_Data + index, m_Data + index + 1, (m_Num - index)*sizeof(int));
		}
	}
}


////////////////////////////////////////////////////////////////////
// vytvoøení položky (vrací index položky)

int CBufInt::New()
{
	int result = NewItem();		// vytvoøení nové položky
	m_Data[result] = 0;			// inicializace položky (musí být 0 == FALSE)
	return result;
}


////////////////////////////////////////////////////////////////////
// pøidání položky (vrací index položky)

int _fastcall CBufInt::Add(const int data)
{
	int result = NewItem();		// vytvoøení nové položky
	m_Data[result] = data;		// inicializace položky
	return result;
}


////////////////////////////////////////////////////////////////////
// duplikace položky (s kontrolou platnosti indexu, vrací index první položky)

int _fastcall CBufInt::Dup(const int index)
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

int _fastcall CBufInt::Dup(const int index, int num)
{
	int result = NewItem();		// vytvoøení nové položky

	if (IsValid(index))					// je index platný?
	{
		int data = m_Data[index];

		m_Data[result] = data;		// kopie položky

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
			m_Data[NewItem()] = 0;
		}
	}
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// operátor pøiøazení

const CBufInt& CBufInt::operator= (const CBufInt& src)
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


/***************************************************************************\
*																			*
*							Buffer logických promìnných						*
*																			*
\***************************************************************************/

////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CBufBool::CBufBool()
{
	m_Data = NULL;			// není buffer dat
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není buffer položek
}

CBufBool::CBufBool(int num)
{
	m_Data = NULL;			// není buffer dat
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není buffer položek
	Num(num);				// vytvoøení bufferu
	Clear(false);			// vymazání bufferu
}

CBufBool::~CBufBool()
{
	DelAll();				// zrušení bufferu
}


////////////////////////////////////////////////////////////////////
// statický konstruktor a destruktor

void CBufBool::Init()
{
	m_Data = NULL;			// není buffer dat
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není buffer položek
}

void CBufBool::Term()
{
	DelAll();				// zrušení bufferu
}


////////////////////////////////////////////////////////////////////
// vytvoøení nových dat (oddìleno kvùli lepší optimalizaci)

void CBufBool::NewData()
{
	m_Max *= 2;
	if (m_Max == 0) m_Max = 0x1000;
//	m_Max += 0x1000;			// 4 KB
	MemBuf(m_Data, m_Max);
}


////////////////////////////////////////////////////////////////////
// zrušení všech položek v bufferu

void CBufBool::DelAll()
{
	MemBuf(m_Data, 0);		// zrušení bufferu dat
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není žádná položka v bufferu
}

////////////////////////////////////////////////////////////////////
// nastavení poètu položek v bufferu (nové položky neinicializované)

void _fastcall CBufBool::Num(int num)
{
	m_Num = num;
	if (num > m_Max)
	{
		num += 0x2000;
		num &= ~0xfff;
		m_Max = num;
		MemBuf(m_Data, num);
	}
}


////////////////////////////////////////////////////////////////////
// vymazání všech položek v bufferu zadanou hodnotou

void _fastcall CBufBool::Clear(const bool clear)
{
	bool* dst = m_Data;
	for (int i = m_Num; i > 0; i--)
	{
		*dst = clear;
		dst++;
	}
}

////////////////////////////////////////////////////////////////////
// poskytnutí položky (s kontrolou platnosti indexu)

bool _fastcall CBufBool::Get(const int index) const
{
	if (IsValid(index))					// je index platný?
	{
		return m_Data[index];
	}
	return false;
}


////////////////////////////////////////////////////////////////////
// nastavení položky (s kontrolou platnosti indexu)

void _fastcall CBufBool::Set(const int index, const bool data)
{
	if (IsValid(index))					// je index platný?
	{
		m_Data[index] = data;
	}
}


////////////////////////////////////////////////////////////////////
// zrušení položek z konce bufferu

void _fastcall CBufBool::Del(int num)
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

int CBufBool::New()
{
	int result = NewItem();		// vytvoøení nové položky
	m_Data[result] = false;		// inicializace položky
	return result;
}


////////////////////////////////////////////////////////////////////
// pøidání položky (vrací index položky)

int _fastcall CBufBool::Add(const bool data)
{
	int result = NewItem();		// vytvoøení nové položky
	m_Data[result] = data;		// inicializace položky
	return result;
}


////////////////////////////////////////////////////////////////////
// duplikace položky (s kontrolou platnosti indexu, vrací index první položky)

int _fastcall CBufBool::Dup(const int index)
{
	int result = NewItem();				// vytvoøení nové položky

	if (IsValid(index))					// je index platný?
	{
		m_Data[result] = m_Data[index];	// kopie položky
	}
	else
	{
		m_Data[result] = false;			// inicializace neplatné položky
	}
	return result;
}

int _fastcall CBufBool::Dup(const int index, int num)
{
	int result = NewItem();		// vytvoøení nové položky

	if (IsValid(index))					// je index platný?
	{
		bool data = m_Data[index];

		m_Data[result] = data;		// kopie položky

		for (num--; num > 0; num--)
		{
			m_Data[NewItem()] = data;
		}
	}
	else
	{
		m_Data[result] = false;		// inicializace neplatné položky

		for (num--; num > 0; num--)
		{
			m_Data[NewItem()] = false;
		}
	}
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// operátor pøiøazení

const CBufBool& CBufBool::operator= (const CBufBool& src)
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


/////////////////////////////////////////////////////////////////////////////
// naètení logické hodnoty ze souboru (naèítá do promìnné, TRUE=operace OK)
/*
bool CBufBool::LoadFile(bool& data, CString jmeno)
{
// naètení textového souboru
	CString txt;
	if (!txt.LoadFile(jmeno)) return false;

// odstranìní mezer a jiných oddìlovaèù
	txt.TrimLeft();
	txt.TrimRight();

// konverze na velká písmena
	txt.UpperCase();

// prázdný text je jako FALSE
	if (txt.IsEmpty())
	{
		data = false;
		return true;
	}

// nastavení dat podle prvního písmene
	TCHAR znak = txt[0];
	data = ((znak == 'T') ||
			(znak == 'Y') ||
			(znak == 'J') ||
			(znak == 'A') ||
			(znak ==  1 ) ||
			(znak == '1'));

	return true;
}


/////////////////////////////////////////////////////////////////////////////
// uložení logické hodnoty do souboru formátu LOG (FALSE=chyba)

bool CBufBool::SaveFile(bool data, CString jmeno) const
{
// pøíprava textu
	CString txt;
	if (data)
	{
		txt = _T("true");
	}
	else
	{
		txt = _T("false");
	}

// uložení textu do souboru
	return txt.SaveFile(jmeno);
}
*/