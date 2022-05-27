
#include "Main.h"

/***************************************************************************\
*																			*
*								Buffer cel�ch ��sel							*
*																			*
\***************************************************************************/


////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CBufInt::CBufInt()
{
	m_Data = NULL;			// nen� buffer dat
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
}

CBufInt::CBufInt(int num)
{
	m_Data = NULL;			// nen� buffer dat
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
	Num(num);				// vytvo�en� bufferu index�
	Clear(-1);				// vymaz�n� bufferu
}

CBufInt::~CBufInt()
{
	DelAll();				// zru�en� bufferu
}


////////////////////////////////////////////////////////////////////
// statick� konstruktor a destruktor

void CBufInt::Init()
{
	m_Data = NULL;			// nen� buffer dat
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
}

void CBufInt::Init(int num)
{
	m_Data = NULL;			// nen� buffer dat
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
	Num(num);				// vytvo�en� bufferu index�
	Clear(-1);				// vymaz�n� bufferu
}

void CBufInt::Term()
{
	DelAll();				// zru�en� bufferu
}


////////////////////////////////////////////////////////////////////
// vytvo�en� nov�ch dat (odd�leno kv�li lep�� optimalizaci)

void CBufInt::NewData()
{
	m_Max *= 2;
	if (m_Max == 0) m_Max = 0x400;
//	m_Max += 0x400;			// 4 KB
	MemBuf(m_Data, m_Max);
}


////////////////////////////////////////////////////////////////////
// zru�en� v�ech polo�ek v bufferu

void CBufInt::DelAll()
{
	MemBuf(m_Data, 0);		// zru�en� bufferu dat
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� ��dn� polo�ka v bufferu
}


////////////////////////////////////////////////////////////////////
// nastaven� po�tu polo�ek v bufferu (nov� polo�ky neinicializovan�)

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
// vymaz�n� v�ech polo�ek v bufferu zadanou hodnotou

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
// poskytnut� polo�ky (s kontrolou platnosti indexu)

int _fastcall CBufInt::Get(const int index) const
{
	if (IsValid(index))					// je index platn�?
	{
		return m_Data[index];
	}
	return 0;
}


////////////////////////////////////////////////////////////////////
// nastaven� polo�ky (s kontrolou platnosti indexu)

void _fastcall CBufInt::Set(const int index, const int data)
{
	if (IsValid(index))					// je index platn�?
	{
		m_Data[index] = data;
	}
}


////////////////////////////////////////////////////////////////////
// vlo�en� polo�ky do bufferu na po�adovanou pozici (omez� index)

void _fastcall CBufInt::Insert(int index, const int data)
{
// omezen� indexu
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

// po�et polo�ek k odsunu
	int num = m_Num - index;

// zv��en� po�tu polo�ek v bufferu
	Num(m_Num + 1);

// odsun polo�ek
	if (num > 0)
	{
		MemMove(m_Data + index + 1, m_Data + index, num*sizeof(int));
	}

// nastaven� polo�ky
	m_Data[index] = data;
}


////////////////////////////////////////////////////////////////////
// zru�en� polo�ek z konce bufferu

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
// zru�en� polo�ky z bufferu (ostatn� polo�ky se p�isunou)

void _fastcall CBufInt::DelItem(const int index)
{
	if (IsValid(index))					// je polo�ka platn�?
	{
		m_Num--;
		if (index < m_Num)
		{
			MemCopy(m_Data + index, m_Data + index + 1, (m_Num - index)*sizeof(int));
		}
	}
}


////////////////////////////////////////////////////////////////////
// vytvo�en� polo�ky (vrac� index polo�ky)

int CBufInt::New()
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result] = 0;			// inicializace polo�ky (mus� b�t 0 == FALSE)
	return result;
}


////////////////////////////////////////////////////////////////////
// p�id�n� polo�ky (vrac� index polo�ky)

int _fastcall CBufInt::Add(const int data)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result] = data;		// inicializace polo�ky
	return result;
}


////////////////////////////////////////////////////////////////////
// duplikace polo�ky (s kontrolou platnosti indexu, vrac� index prvn� polo�ky)

int _fastcall CBufInt::Dup(const int index)
{
	int result = NewItem();				// vytvo�en� nov� polo�ky

	if (IsValid(index))					// je index platn�?
	{
		m_Data[result] = m_Data[index];	// kopie polo�ky
	}
	else
	{
		m_Data[result] = 0;				// inicializace neplatn� polo�ky
	}
	return result;
}

int _fastcall CBufInt::Dup(const int index, int num)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky

	if (IsValid(index))					// je index platn�?
	{
		int data = m_Data[index];

		m_Data[result] = data;		// kopie polo�ky

		for (num--; num > 0; num--)
		{
			m_Data[NewItem()] = data;
		}
	}
	else
	{
		m_Data[result] = 0;		// inicializace neplatn� polo�ky

		for (num--; num > 0; num--)
		{
			m_Data[NewItem()] = 0;
		}
	}
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// oper�tor p�i�azen�

const CBufInt& CBufInt::operator= (const CBufInt& src)
{
	m_Num = 0;					// zru�en� star�ch dat
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


/***************************************************************************\
*																			*
*							Buffer logick�ch prom�nn�ch						*
*																			*
\***************************************************************************/

////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CBufBool::CBufBool()
{
	m_Data = NULL;			// nen� buffer dat
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
}

CBufBool::CBufBool(int num)
{
	m_Data = NULL;			// nen� buffer dat
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
	Num(num);				// vytvo�en� bufferu
	Clear(false);			// vymaz�n� bufferu
}

CBufBool::~CBufBool()
{
	DelAll();				// zru�en� bufferu
}


////////////////////////////////////////////////////////////////////
// statick� konstruktor a destruktor

void CBufBool::Init()
{
	m_Data = NULL;			// nen� buffer dat
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
}

void CBufBool::Term()
{
	DelAll();				// zru�en� bufferu
}


////////////////////////////////////////////////////////////////////
// vytvo�en� nov�ch dat (odd�leno kv�li lep�� optimalizaci)

void CBufBool::NewData()
{
	m_Max *= 2;
	if (m_Max == 0) m_Max = 0x1000;
//	m_Max += 0x1000;			// 4 KB
	MemBuf(m_Data, m_Max);
}


////////////////////////////////////////////////////////////////////
// zru�en� v�ech polo�ek v bufferu

void CBufBool::DelAll()
{
	MemBuf(m_Data, 0);		// zru�en� bufferu dat
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� ��dn� polo�ka v bufferu
}

////////////////////////////////////////////////////////////////////
// nastaven� po�tu polo�ek v bufferu (nov� polo�ky neinicializovan�)

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
// vymaz�n� v�ech polo�ek v bufferu zadanou hodnotou

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
// poskytnut� polo�ky (s kontrolou platnosti indexu)

bool _fastcall CBufBool::Get(const int index) const
{
	if (IsValid(index))					// je index platn�?
	{
		return m_Data[index];
	}
	return false;
}


////////////////////////////////////////////////////////////////////
// nastaven� polo�ky (s kontrolou platnosti indexu)

void _fastcall CBufBool::Set(const int index, const bool data)
{
	if (IsValid(index))					// je index platn�?
	{
		m_Data[index] = data;
	}
}


////////////////////////////////////////////////////////////////////
// zru�en� polo�ek z konce bufferu

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
// vytvo�en� polo�ky (vrac� index polo�ky)

int CBufBool::New()
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result] = false;		// inicializace polo�ky
	return result;
}


////////////////////////////////////////////////////////////////////
// p�id�n� polo�ky (vrac� index polo�ky)

int _fastcall CBufBool::Add(const bool data)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result] = data;		// inicializace polo�ky
	return result;
}


////////////////////////////////////////////////////////////////////
// duplikace polo�ky (s kontrolou platnosti indexu, vrac� index prvn� polo�ky)

int _fastcall CBufBool::Dup(const int index)
{
	int result = NewItem();				// vytvo�en� nov� polo�ky

	if (IsValid(index))					// je index platn�?
	{
		m_Data[result] = m_Data[index];	// kopie polo�ky
	}
	else
	{
		m_Data[result] = false;			// inicializace neplatn� polo�ky
	}
	return result;
}

int _fastcall CBufBool::Dup(const int index, int num)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky

	if (IsValid(index))					// je index platn�?
	{
		bool data = m_Data[index];

		m_Data[result] = data;		// kopie polo�ky

		for (num--; num > 0; num--)
		{
			m_Data[NewItem()] = data;
		}
	}
	else
	{
		m_Data[result] = false;		// inicializace neplatn� polo�ky

		for (num--; num > 0; num--)
		{
			m_Data[NewItem()] = false;
		}
	}
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// oper�tor p�i�azen�

const CBufBool& CBufBool::operator= (const CBufBool& src)
{
	m_Num = 0;					// zru�en� star�ch dat
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


/////////////////////////////////////////////////////////////////////////////
// na�ten� logick� hodnoty ze souboru (na��t� do prom�nn�, TRUE=operace OK)
/*
bool CBufBool::LoadFile(bool& data, CString jmeno)
{
// na�ten� textov�ho souboru
	CString txt;
	if (!txt.LoadFile(jmeno)) return false;

// odstran�n� mezer a jin�ch odd�lova��
	txt.TrimLeft();
	txt.TrimRight();

// konverze na velk� p�smena
	txt.UpperCase();

// pr�zdn� text je jako FALSE
	if (txt.IsEmpty())
	{
		data = false;
		return true;
	}

// nastaven� dat podle prvn�ho p�smene
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
// ulo�en� logick� hodnoty do souboru form�tu LOG (FALSE=chyba)

bool CBufBool::SaveFile(bool data, CString jmeno) const
{
// p��prava textu
	CString txt;
	if (data)
	{
		txt = _T("true");
	}
	else
	{
		txt = _T("false");
	}

// ulo�en� textu do souboru
	return txt.SaveFile(jmeno);
}
*/