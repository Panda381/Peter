
#include "Main.h"

/***************************************************************************\
*																			*
*							Buffer re�ln�ch ��sel							*
*																			*
\***************************************************************************/


////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CBufReal::CBufReal()
{
	m_Data = NULL;			// nen� buffer dat
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
}

CBufReal::~CBufReal()
{
	DelAll();				// zru�en� bufferu
}


////////////////////////////////////////////////////////////////////
// statick� konstruktor a destruktor

void CBufReal::Init()
{
	m_Data = NULL;			// nen� buffer dat
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
}

void CBufReal::Term()
{
	DelAll();				// zru�en� bufferu
}


////////////////////////////////////////////////////////////////////
// vytvo�en� nov�ch dat (odd�leno kv�li lep�� optimalizaci)

void CBufReal::NewData()
{
	m_Max *= 2;
	if (m_Max == 0) m_Max = 0x200;
//	m_Max += 0x200;			// 4 KB
	MemBuf(m_Data, m_Max);
}


////////////////////////////////////////////////////////////////////
// nastaven� po�tu polo�ek v bufferu (nov� polo�ky neinicializovan�)

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
// zru�en� v�ech polo�ek v bufferu

void CBufReal::DelAll()
{
	MemBuf(m_Data, 0);		// zru�en� bufferu dat
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� ��dn� polo�ka v bufferu
}


////////////////////////////////////////////////////////////////////
// poskytnut� polo�ky (s kontrolou platnosti indexu)

double _fastcall CBufReal::Get(const int index) const
{
	if (IsValid(index))					// je index platn�?
	{
		return m_Data[index];
	}
	return 0;
}


////////////////////////////////////////////////////////////////////
// nastaven� polo�ky (s kontrolou platnosti indexu)

void _fastcall CBufReal::Set(const int index, const double data)
{
	if (IsValid(index))					// je index platn�?
	{
		m_Data[index] = data;
	}
}


////////////////////////////////////////////////////////////////////
// vlo�en� polo�ky do bufferu na po�adovanou pozici (omez� index)

void _fastcall CBufReal::Insert(int index, const double data)
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
		MemMove(m_Data + index + 1, m_Data + index, num*sizeof(double));
	}

// nastaven� polo�ky
	m_Data[index] = data;
}


////////////////////////////////////////////////////////////////////
// zru�en� polo�ek z konce bufferu

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
// zru�en� polo�ky z bufferu (ostatn� polo�ky se p�isunou)

void _fastcall CBufReal::DelItem(const int index)
{
	if (IsValid(index))					// je polo�ka platn�?
	{
		m_Num--;
		if (index < m_Num)
		{
			MemCopy(m_Data + index, m_Data + index + 1, (m_Num - index)*sizeof(double));
		}
	}
}

////////////////////////////////////////////////////////////////////
// vytvo�en� polo�ky (vrac� index polo�ky)

int CBufReal::New()
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result] = 0;			// inicializace polo�ky
	return result;
}


////////////////////////////////////////////////////////////////////
// p�id�n� polo�ky (vrac� index polo�ky)

int _fastcall CBufReal::Add(const double data)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result] = data;		// inicializace polo�ky
	return result;
}


////////////////////////////////////////////////////////////////////
// duplikace polo�ky (s kontrolou platnosti indexu, vrac� index prvn� polo�ky)

int _fastcall CBufReal::Dup(const int index)
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

int _fastcall CBufReal::Dup(const int index, int num)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky

	if (IsValid(index))					// je index platn�?
	{
		double data = m_Data[index];

		m_Data[result] = data;			// kopie polo�ky

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
			m_Data[NewItem()]= 0;
		}
	}
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� ��sla ze souboru (na��t� do prom�nn�, true=operace OK)
/*
bool CBufReal::LoadFile(double& data, CString jmeno)
{
// na�ten� textov�ho souboru
	CString txt;
	if (!txt.LoadFile(jmeno)) return false;

// odstran�n� mezer a jin�ch odd�lova��
	txt.TrimLeft();
	txt.TrimRight();

// na�ten� ��sla
	data = Double(txt);
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// ulo�en� ��sla do souboru form�tu NUM (false=chyba)

bool CBufReal::SaveFile(double data, CString jmeno) const
{
// p��prava ��sla
	CString txt;
	txt.Double(data);

// ulo�en� textu do souboru
	return txt.SaveFile(jmeno);
}
*/

/////////////////////////////////////////////////////////////////////////////
// oper�tor p�i�azen�

const CBufReal& CBufReal::operator= (const CBufReal& src)
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
