
#include "Main.h"

/***************************************************************************\
*																			*
*							Buffer p�emapov�n� index�						*
*																			*
\***************************************************************************/


////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CBufIndex::CBufIndex()
{
	m_Data = NULL;			// nen� buffer dat
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
}

CBufIndex::~CBufIndex()
{
	DelAll();				// zru�en� bufferu
}


////////////////////////////////////////////////////////////////////
// statick� konstruktor a destruktor

void CBufIndex::Init()
{
	m_Data = NULL;			// nen� buffer dat
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
}

void CBufIndex::Term()
{
	DelAll();				// zru�en� bufferu
}


////////////////////////////////////////////////////////////////////
// zru�en� v�ech polo�ek v bufferu

void CBufIndex::DelAll()
{
	MemFree(m_Data);		// zru�en� bufferu dat
	m_Data = NULL;			// p��znak neplatnosti bufferu
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� ��dn� polo�ka v bufferu
}


////////////////////////////////////////////////////////////////////
// nastaven� po�tu polo�ek v bufferu
// (nov� polo�ky neinicializovan�, vrac� FALSE=chyba pam�ti)

bool _fastcall CBufIndex::Num(const int num)
{
	ASSERT(num >= 0);

// test, zda je nutno zv��it velikost bufferu
	if (num > m_Max)
	{

// zv�t�en� bufferu
		int newmax = (num + 0x400) & ~0xff;
		int* newdata = (int*)MemSize(m_Data, newmax*sizeof(int));
		if (newdata == NULL) return false;
		m_Data = newdata;
		m_Max = newmax;
	}

// nov� po�et polo�ek v bufferu
	m_Num = num;
	return true;
}


////////////////////////////////////////////////////////////////////
// nastaven� po�tu polo�ek s vymaz�n�m "-1", vrac� FALSE=chyba pam�ti

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
// vymaz�n� v�ech polo�ek v bufferu zadanou hodnotou

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
// poskytnut� polo�ky (s kontrolou platnosti indexu, pro neplatnou vr�t� -1)

int _fastcall CBufIndex::Get(const int index) const
{
	if (IsValid(index))					// je index platn�?
	{
		return m_Data[index];
	}
	return -1;
}


////////////////////////////////////////////////////////////////////
// nastaven� polo�ky (s kontrolou platnosti indexu)

void _fastcall CBufIndex::Set(const int index, const int data)
{
	if (IsValid(index))					// je index platn�?
	{
		m_Data[index] = data;
	}
}


////////////////////////////////////////////////////////////////////
// p�id�n� polo�ky (vrac� index polo�ky, p�i chyb� pam�ti vrac� <0)

int _fastcall CBufIndex::Add(const int data)
{
// index nov� polo�ky
	int index = m_Num;

// nastaven� nov�ho po�tu polo�ek
	if (!Num(index + 1)) return -1;

// inicializace polo�ky
	m_Data[index] = data;

// index nov� polo�ky
	return index;
}

/////////////////////////////////////////////////////////////////////////////
// oper�tor p�i�azen� (p�i chyb� pam�ti po�et polo�ek nesouhlas�)

const CBufIndex& _fastcall CBufIndex::operator= (const CBufIndex& srcbuf)
{
	if (Num(srcbuf.Num()))
	{
		MemCopy(m_Data, srcbuf.Data(), m_Num*sizeof(int));
	}
	return *this;
}
