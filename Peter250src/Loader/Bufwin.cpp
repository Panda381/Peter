
#include "Main.h"

/***************************************************************************\
*																			*
*								Prvky oken									*
*																			*
\***************************************************************************/

////////////////////////////////////////////////////////////////////
// vytvoøení nových dat

#define NEWDATANUM 40			// poèet novì vytvoøených položek (2 KB)

void CBufWin::NewData()
{
	int i = NEWDATANUM;		// poèet nových položek
	int next = m_Max;		// pøíští položka - 1
	m_Max = next + i;		// zvýšení poètu položek (o 2 KB)

	MemBuf(m_Data, m_Max);	// zvýšení velikosti bufferu

	MemBuf(m_Valid, m_Max);	// zvýšení velikosti bufferu platnosti
	MemFill(m_Valid + next, NEWDATANUM, false); // nastavení pøíznakù na neplatné položky

	WINITEM* data = m_Data + next - 1; // ukazatel dat - 1
	for (; i > 0; i--)
	{
		data++;				// zvýšení ukazatele položek
		next++;				// zvýšení indexu pøíští položky
		*(int*)data = next; // odkaz na pøíští položku
	}
	*(int*)data = m_Next;	// navázání na další položku
	m_Next = m_Max-NEWDATANUM;	// odkaz na první novou položku
};


////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CBufWin::CBufWin()
{
	m_Data = NULL;			// není buffer dat
	m_Valid = NULL;			// není buffer platnosti
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není buffer položek
	m_Next = -1;			// pøiští volná položka (-1=není)
}

CBufWin::~CBufWin()
{
	DelAll();				// zrušení všech položek
}


////////////////////////////////////////////////////////////////////
// statický konstruktor a destruktor

void CBufWin::Init()
{
	m_Data = NULL;			// není buffer dat
	m_Valid = NULL;			// není buffer platnosti
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není buffer položek
	m_Next = -1;			// pøiští volná položka (-1=není)
}

void CBufWin::Term()
{
	DelAll();				// zrušení všech položek
}

////////////////////////////////////////////////////////////////////
// zrušení všech položek v bufferu (ukládání zaène opìt po øadì)

void CBufWin::DelAll()
{
// zrušení položek
	for (int i = m_Max-1; i >= 0; i--)
	{
		Del(i);
	}

// zrušení bufferu
	MemBuf(m_Data, 0);			// zrušení bufferu dat
	MemBuf(m_Valid, 0);			// zrušení bufferu platnosti
	m_Num = 0;					// není žádná platná položka
	m_Max = 0;					// není žádná položka v bufferu
	m_Next = -1;				// není pøíští položka
}

////////////////////////////////////////////////////////////////////
// zrušení položky (s kontrolou platnosti indexu)

void _fastcall CBufWin::Del(const int index)
{
	if (IsValid(index))						// je index platný?
	{
		WINITEM* item = m_Data + index;
		item->Text.Term();			// ukonèení textu
		item->Icon.Term();
		item->Picture.Term();
		MemFree(item->PictureData);

		if ((item->FontBrush != StdBrushBtn) &&
			(item->FontBrush != StdBrushWindow) &&
			(item->FontBrush != NULL))
		{
			::DeleteObject(item->FontBrush);
		}

		FreeFont(item->Font);				// uvolnìní fontu

		for (int i = item->TabRows * item->TabCols - 1; i >= 0; i--)
		{
			item->TabData[i].Term();
		}

		MemFree(item->TabData);
		MemFree(item->TabAlign);

		*(int*)(m_Data + index) = m_Next;	// pøíští volná položka
		m_Valid[index] = false;				// zrušení pøíznaku platnosti
		m_Num--;							// snížení èítaèe platných položek
		m_Next = index;						// odkaz na tuto položku
	}
}


////////////////////////////////////////////////////////////////////
// vytvoøení položky (vrací index položky)

int _fastcall CBufWin::New()
{
	int result = NewItem();		// vytvoøení nové položky
	return result;
}
