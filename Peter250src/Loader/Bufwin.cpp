
#include "Main.h"

/***************************************************************************\
*																			*
*								Prvky oken									*
*																			*
\***************************************************************************/

////////////////////////////////////////////////////////////////////
// vytvo�en� nov�ch dat

#define NEWDATANUM 40			// po�et nov� vytvo�en�ch polo�ek (2 KB)

void CBufWin::NewData()
{
	int i = NEWDATANUM;		// po�et nov�ch polo�ek
	int next = m_Max;		// p��t� polo�ka - 1
	m_Max = next + i;		// zv��en� po�tu polo�ek (o 2 KB)

	MemBuf(m_Data, m_Max);	// zv��en� velikosti bufferu

	MemBuf(m_Valid, m_Max);	// zv��en� velikosti bufferu platnosti
	MemFill(m_Valid + next, NEWDATANUM, false); // nastaven� p��znak� na neplatn� polo�ky

	WINITEM* data = m_Data + next - 1; // ukazatel dat - 1
	for (; i > 0; i--)
	{
		data++;				// zv��en� ukazatele polo�ek
		next++;				// zv��en� indexu p��t� polo�ky
		*(int*)data = next; // odkaz na p��t� polo�ku
	}
	*(int*)data = m_Next;	// nav�z�n� na dal�� polo�ku
	m_Next = m_Max-NEWDATANUM;	// odkaz na prvn� novou polo�ku
};


////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CBufWin::CBufWin()
{
	m_Data = NULL;			// nen� buffer dat
	m_Valid = NULL;			// nen� buffer platnosti
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
	m_Next = -1;			// p�i�t� voln� polo�ka (-1=nen�)
}

CBufWin::~CBufWin()
{
	DelAll();				// zru�en� v�ech polo�ek
}


////////////////////////////////////////////////////////////////////
// statick� konstruktor a destruktor

void CBufWin::Init()
{
	m_Data = NULL;			// nen� buffer dat
	m_Valid = NULL;			// nen� buffer platnosti
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
	m_Next = -1;			// p�i�t� voln� polo�ka (-1=nen�)
}

void CBufWin::Term()
{
	DelAll();				// zru�en� v�ech polo�ek
}

////////////////////////////////////////////////////////////////////
// zru�en� v�ech polo�ek v bufferu (ukl�d�n� za�ne op�t po �ad�)

void CBufWin::DelAll()
{
// zru�en� polo�ek
	for (int i = m_Max-1; i >= 0; i--)
	{
		Del(i);
	}

// zru�en� bufferu
	MemBuf(m_Data, 0);			// zru�en� bufferu dat
	MemBuf(m_Valid, 0);			// zru�en� bufferu platnosti
	m_Num = 0;					// nen� ��dn� platn� polo�ka
	m_Max = 0;					// nen� ��dn� polo�ka v bufferu
	m_Next = -1;				// nen� p��t� polo�ka
}

////////////////////////////////////////////////////////////////////
// zru�en� polo�ky (s kontrolou platnosti indexu)

void _fastcall CBufWin::Del(const int index)
{
	if (IsValid(index))						// je index platn�?
	{
		WINITEM* item = m_Data + index;
		item->Text.Term();			// ukon�en� textu
		item->Icon.Term();
		item->Picture.Term();
		MemFree(item->PictureData);

		if ((item->FontBrush != StdBrushBtn) &&
			(item->FontBrush != StdBrushWindow) &&
			(item->FontBrush != NULL))
		{
			::DeleteObject(item->FontBrush);
		}

		FreeFont(item->Font);				// uvoln�n� fontu

		for (int i = item->TabRows * item->TabCols - 1; i >= 0; i--)
		{
			item->TabData[i].Term();
		}

		MemFree(item->TabData);
		MemFree(item->TabAlign);

		*(int*)(m_Data + index) = m_Next;	// p��t� voln� polo�ka
		m_Valid[index] = false;				// zru�en� p��znaku platnosti
		m_Num--;							// sn�en� ��ta�e platn�ch polo�ek
		m_Next = index;						// odkaz na tuto polo�ku
	}
}


////////////////////////////////////////////////////////////////////
// vytvo�en� polo�ky (vrac� index polo�ky)

int _fastcall CBufWin::New()
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	return result;
}
