
#include "Main.h"

/***************************************************************************\
*																			*
*								Textové øetìzce								*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inicializaèní prázdný øetìzec (modifikuje se poèet referencí!)

TEXTDATA	EmptyTextData		= { 1, 0, 0 };
const CText		EmptyText;		// prázdný øetìzec

/////////////////////////////////////////////////////////////////////////////
// vytvoøení dat textu - délka zadána ve znacích (pøi chybì pamìti vrací NULL)

TEXTDATA* _fastcall NewTextData(int length)
{
	ASSERT(length >= 0);

// vytvoøení bufferu
	TEXTDATA* data = (TEXTDATA*)MemGet((length+1)*sizeof(TCHAR) + SIZEOFTEXTDATA);
	if (data != NULL)
	{

// nastavení parametrù
		data->Refer = 1;				// poèet referencí
		data->Length = length;			// délka
		data->Data[length] = 0;			// oznaèení konce textu
	}

// adresa dat
	return data;
}


/////////////////////////////////////////////////////////////////////////////
// zrušení dat textu (oddìleno kvùli lepší optimalizaci)

void _fastcall DelTextData(TEXTDATA* data)
{
	ASSERT(data != NULL);

	MemFree(data);
}

/////////////////////////////////////////////////////////////////////////////
// zmìna velikosti bufferu (délka zadána ve znacích) - zajistí pøivlastnìní bufferu
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn

bool CText::SizeBuffer(int length)
{
	ASSERT(length >= 0);

// test, zda se velikost bufferu mìní
	if (length == pData->Length) return true;

// pøivlastnìní bufferu
	if (!CopyWrite()) return false;

// nová velikost bufferu
	TEXTDATA* data = (TEXTDATA*)MemSize(pData, (length+1)*sizeof(TCHAR) + SIZEOFTEXTDATA);
	if (data == NULL) return false;

// nastavení parametrù
	pData = data;					// adresa dat
	data->Length = length;			// délka
	data->Data[length] = 0;			// oznaèení konce textu
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CText::CText() 
{ 
	attach(&EmptyTextData); 
};

CText::CText(const CText& src) 
{ 
	attach(src.pData); 
};

CText::CText(TEXTDATA* data) 
{ 
	attach(data); 
};

CText::CText(const TCHAR chr) // pøi chybì pamìti bude øetìzec prázdný
{
	pData = NewTextData(1);

	if (pData != NULL)
	{
		pData->Data[0] = chr;
	}
	else
	{
		attach(&EmptyTextData); 
	}
}

CText::CText(LPCTSTR text) // pøi chybì pamìti bude øetìzec prázdný
{
	int len = 0;
	if (text != NULL) len = ::lstrlen(text);

	pData = NewTextData(len);	
	if (pData != NULL)
	{
		MemCopy(pData->Data, text, len*sizeof(TCHAR));
	}
	else
	{
		attach(&EmptyTextData); 
	}
}

CText::CText(LPCTSTR text, const int length) // pøi chybì pamìti bude øetìzec prázdný
{
	int len = 0;
	if (text != NULL)
	{
		len = length;
		if (len < 0) len = ::lstrlen(text);
	}

	pData = NewTextData(len);	
	if (pData != NULL)
	{
		MemCopy(pData->Data, text, len*sizeof(TCHAR));
	}
	else
	{
		attach(&EmptyTextData); 
	}
}

#ifdef _UNICODE

CText::CText(LPCSTR text) // pøi chybì pamìti bude øetìzec prázdný
{
	int len = 0;
	if (text != NULL) len = ::lstrlenA(text);

	pData = NewTextData(len);	
	if (pData != NULL)
	{
		::MultiByteToWideChar(CP_ACP, 0, text, len, pData->Data, len);
	}
	else
	{
		attach(&EmptyTextData); 
	}
}

CText::CText(LPCSTR text, const int length) // pøi chybì pamìti bude øetìzec prázdný
{
	int len = 0;
	if (text != NULL)
	{
		len = length;
		if (len < 0) len = ::lstrlenA(text);
	}

	pData = NewTextData(len);	
	if (pData != NULL)
	{
		::MultiByteToWideChar(CP_ACP, 0, text, len, pData->Data, len);
	}
	else
	{
		attach(&EmptyTextData); 
	}
}

#else //_UNICODE

CText::CText(LPCWSTR text) // pøi chybì pamìti bude øetìzec prázdný
{
	int len = 0;
	if (text != NULL) len = ::lstrlenW(text);

	pData = NewTextData(len);	
	if (pData != NULL)
	{
		::WideCharToMultiByte(CP_ACP, 0, text, len, pData->Data, len, NULL, NULL);
	}
	else
	{
		attach(&EmptyTextData); 
	}
}

CText::CText(LPCWSTR text, const int length) // pøi chybì pamìti bude øetìzec prázdný
{
	int len = 0;
	if (text != NULL)
	{
		len = length;
		if (len < 0) len = ::lstrlenW(text);
	}

	pData = NewTextData(len);	
	if (pData != NULL)
	{
		::WideCharToMultiByte(CP_ACP, 0, text, len, pData->Data, len, NULL, NULL);
	}
	else
	{
		attach(&EmptyTextData); 
	}
}

#endif //_UNICODE

CText::~CText() 
{ 
	detach(pData); 
};


/////////////////////////////////////////////////////////////////////////////
// sluèovací konstruktory (urèeno pro operátor +) - pøi chybì pamìti bude øetìzec prázdný

CText::CText(const CText& str1, const CText& str2)
{ 
	int len1 = str1.pData->Length;
	int len2 = str2.pData->Length;
	pData = NewTextData(len1+len2);
	if (pData != NULL)
	{
		MemCopy(pData->Data, str1.pData->Data, len1*sizeof(TCHAR));
		MemCopy(pData->Data + len1, str2.pData->Data, len2*sizeof(TCHAR));
	}
	else
	{
		attach(&EmptyTextData); 
	}
}

CText::CText(LPCTSTR txt, const CText& str)
{ 
	int len1 = 0;
	if (txt != NULL) len1 = ::lstrlen(txt);
	int len2 = str.pData->Length;
	pData = NewTextData(len1+len2);
	if (pData != NULL)
	{
		MemCopy(pData->Data, txt, len1*sizeof(TCHAR));
		MemCopy(pData->Data + len1, str.pData->Data, len2*sizeof(TCHAR));
	}
	else
	{
		attach(&EmptyTextData); 
	}
}

CText::CText(const CText& str, LPCTSTR txt)
{ 
	int len1 = str.pData->Length;
	int len2 = 0;
	if (txt != NULL) len2 = ::lstrlen(txt);
	pData = NewTextData(len1+len2);
	if (pData != NULL)
	{
		MemCopy(pData->Data, str.pData->Data, len1*sizeof(TCHAR));
		MemCopy(pData->Data + len1, txt, len2*sizeof(TCHAR));
	}
	else
	{
		attach(&EmptyTextData); 
	}
}

CText::CText(const TCHAR chr, const CText& str)
{ 
	int len = str.pData->Length;
	pData = NewTextData(1+len);
	if (pData != NULL)
	{
		pData->Data[0] = chr;
		MemCopy(pData->Data + 1, str.pData->Data, len*sizeof(TCHAR));
	}
	else
	{
		attach(&EmptyTextData); 
	}
}

CText::CText(const CText& str, const TCHAR chr)
{ 
	int len = str.pData->Length;
	pData = NewTextData(len+1);
	if (pData != NULL)
	{
		MemCopy(pData->Data, str.pData->Data, len*sizeof(TCHAR));
		pData->Data[len] = chr;
	}
	else
	{
		attach(&EmptyTextData); 
	}
}


/////////////////////////////////////////////////////////////////////////////
// statický konstruktor a destruktor

void CText::Init()
{ 
	attach(&EmptyTextData); 
};

void _fastcall CText::Init(const CText& src) 
{ 
	attach(src.pData); 
};

void _fastcall CText::Init(TEXTDATA* data)
{ 
	attach(data); 
};

bool _fastcall CText::Init(const TCHAR chr) // pøi chybì pamìti vrátí FALSE, text není platný
{
	pData = NewTextData(1);
	if (pData != NULL)
	{
		pData->Data[0] = chr;
		return true;
	}
	return false;
}

bool _fastcall CText::Init(LPCTSTR text) // pøi chybì pamìti vrátí FALSE, text není platný
{
	int len = 0;
	if (text != NULL) len = ::lstrlen(text);

	pData = NewTextData(len);	
	if (pData != NULL)
	{
		MemCopy(pData->Data, text, len*sizeof(TCHAR));
		return true;
	}
	return false;
}

bool _fastcall CText::Init(LPCTSTR text, const int length) // pøi chybì pamìti vrátí FALSE, text není platný
{
	int len = 0;
	if (text != NULL)
	{
		len = length;
		if (len < 0) len = ::lstrlen(text);
	}

	pData = NewTextData(len);	
	if (pData != NULL)
	{
		MemCopy(pData->Data, text, len*sizeof(TCHAR));
		return true;
	}
	return false;
}

#ifdef _UNICODE

bool _fastcall CText::Init(LPCSTR text) // pøi chybì pamìti vrátí FALSE, text není platný
{
	int len = 0;
	if (text != NULL) len = ::lstrlenA(text);

	pData = NewTextData(len);	
	if (pData != NULL)
	{
		::MultiByteToWideChar(CP_ACP, 0, text, len, pData->Data, len);
		return true;
	}
	return false;
}

bool _fastcall CText::Init(LPCSTR text, const int length) // pøi chybì pamìti vrátí FALSE, text není platný
{
	int len = 0;
	if (text != NULL)
	{
		len = length;
		if (len < 0) len = ::lstrlenA(text);
	}

	pData = NewTextData(len);	
	if (pData != NULL)
	{
		::MultiByteToWideChar(CP_ACP, 0, text, len, pData->Data, len);
		return true;
	}
	return false;
}

#else //_UNICODE

bool _fastcall CText::Init(LPCWSTR text) // pøi chybì pamìti vrátí FALSE, text není platný
{
	int len = 0;
	if (text != NULL) len = ::lstrlenW(text);

	pData = NewTextData(len);	
	if (pData != NULL)
	{
		::WideCharToMultiByte(CP_ACP, 0, text, len, pData->Data, len, NULL, NULL);
		return true;
	}
	return false;
}

bool _fastcall CText::Init(LPCWSTR text, const int length) // pøi chybì pamìti vrátí FALSE, text není platný
{
	int len = 0;
	if (text != NULL)
	{
		len = length;
		if (len < 0) len = ::lstrlenW(text);
	}

	pData = NewTextData(len);	
	if (pData != NULL)
	{
		::WideCharToMultiByte(CP_ACP, 0, text, len, pData->Data, len, NULL, NULL);
		return true;
	}
	return false;
}

#endif //_UNICODE

void CText::Term()
{ 
	detach(pData); 
};


/////////////////////////////////////////////////////////////////////////////
// zápis do bufferu (bez koncové nuly)

void CText::Write(TCHAR* buf) const
{
	MemCopy(buf, pData->Data, pData->Length*sizeof(TCHAR));
}

#ifdef _UNICODE

void CText::Write(LPSTR buf) const
{
	::WideCharToMultiByte(CP_ACP, 0, pData->Data, pData->Length, buf, pData->Length, NULL, NULL);
}

#else // _UNICODE

void CText::Write(LPWSTR buf) const
{
	::MultiByteToWideChar(CP_ACP, 0, pData->Data, pData->Length, buf, pData->Length);
}

#endif // _UNICODE


/////////////////////////////////////////////////////////////////////////////
// zápis do bufferu (s koncovou nulou)

void CText::WriteNull(TCHAR* buf) const
{
	MemCopy(buf, pData->Data, (pData->Length+1)*sizeof(TCHAR));
}

#ifdef _UNICODE

void CText::WriteNull(LPSTR buf) const
{
	::WideCharToMultiByte(CP_ACP, 0, pData->Data, pData->Length, buf, pData->Length, NULL, NULL);
	buf[pData->Length] = 0;
}

#else // _UNICODE

void CText::WriteNull(LPWSTR buf) const
{
	::MultiByteToWideChar(CP_ACP, 0, pData->Data, pData->Length, buf, pData->Length);
	buf[pData->Length] = 0;
}

#endif // _UNICODE


/////////////////////////////////////////////////////////////////////////////
// hledání textu v øetìzci (<0 = nenalezeno, pos=výchozí pozice)

int _fastcall CText::Find(const CText& str) const
{
// hledaný øetìzec prázdný - bude pozice 0
	int lenstr = str.pData->Length;
	if (lenstr == 0) return 0;

// hledaný øetìzec má délku 1 - pøevod na hledání znaku
	TCHAR* datastr = str.pData->Data;
	TCHAR chr = datastr[0];
	if (lenstr == 1) return Find(chr);

// pøíprava ukazatelù
	int i = pData->Length - lenstr;		// poèet testovaných pozic - 1
	datastr++;							// druhý znak hledaného øetìzce
	lenstr--;							// délka hledaného øetìzce - 1
	TCHAR* data = pData->Data;			// zaèátek prohledávaného øetìzce

// cyklus hledání
	for (; i >= 0; i--)
	{
		if (*(data++) == chr)			// shoduje se první znak?
		{
			if (MemCompare(data, datastr, lenstr*sizeof(TCHAR)))	// shoduje se zbytek øetìzce?
			{
				return (data - pData->Data - 1);	// návrat pozice zaèátku øetìzce
			}
		}
	}

	return -1;
}

int _fastcall CText::Find(const CText& str, int pos) const
{
// omezení a kontrola výchozí pozice
	if (pos < 0) pos = 0;
	if (pos >= pData->Length) return -1;

// hledaný øetìzec prázdný - bude výchozí pozice
	int lenstr = str.pData->Length;
	if (lenstr == 0) return pos;

// hledaný øetìzec má délku 1 - pøevod na hledání znaku
	TCHAR* datastr = str.pData->Data;
	TCHAR chr = datastr[0];
	if (lenstr == 1) return Find(chr, pos);

// pøíprava ukazatelù
	int i = pData->Length - lenstr - pos;	// poèet testovaných pozic - 1
	datastr++;							// druhý znak hledaného øetìzce
	lenstr--;							// délka hledaného øetìzce - 1
	TCHAR* data = pData->Data + pos;	// zaèátek prohledávaného øetìzce

// cyklus hledání
	for (; i >= 0; i--)
	{
		if (*(data++) == chr)			// shoduje se první znak?
		{
			if (MemCompare(data, datastr, lenstr*sizeof(TCHAR)))	// shoduje se zbytek øetìzce?
			{
				return (data - pData->Data - 1);	// návrat pozice zaèátku øetìzce
			}
		}
	}

	return -1;
}


/////////////////////////////////////////////////////////////////////////////
// hledání textu v øetìzci (<0 = nenalezeno, pos=výchozí pozice)

int _fastcall CText::Find(LPCTSTR txt) const
{
// pøíprava délky hledaného øetìzce
	int lenstr = 0;
	if (txt != NULL) lenstr = ::lstrlen(txt);

// hledaný øetìzec prázdný - bude pozice 0
	if (lenstr == 0) return 0;

// hledaný øetìzec má délku 1 - pøevod na hledání znaku
	const TCHAR* datastr = txt;
	TCHAR chr = datastr[0];
	if (lenstr == 1) return Find(chr);

// pøíprava ukazatelù
	int i = pData->Length - lenstr;		// poèet testovaných pozic - 1
	datastr++;							// druhý znak hledaného øetìzce
	lenstr--;							// délka hledaného øetìzce - 1
	TCHAR* data = pData->Data;			// zaèátek prohledávaného øetìzce

// cyklus hledání
	for (; i >= 0; i--)
	{
		if (*(data++) == chr)			// shoduje se první znak?
		{
			if (MemCompare(data, datastr, lenstr*sizeof(TCHAR)))	// shoduje se zbytek øetìzce?
			{
				return (data - pData->Data - 1);	// návrat pozice zaèátku øetìzce
			}
		}
	}

	return -1;
}

int _fastcall CText::Find(LPCTSTR txt, int pos) const
{
// omezení a kontrola výchozí pozice
	if (pos < 0) pos = 0;
	if (pos >= pData->Length) return -1;

// pøíprava délky hledaného øetìzce
	int lenstr = 0;
	if (txt != NULL) lenstr = ::lstrlen(txt);

// hledaný øetìzec prázdný - bude výchozí pozice
	if (lenstr == 0) return pos;

// hledaný øetìzec má délku 1 - pøevod na hledání znaku
	const TCHAR* datastr = txt;
	TCHAR chr = datastr[0];
	if (lenstr == 1) return Find(chr, pos);

// pøíprava ukazatelù
	int i = pData->Length - lenstr - pos;	// poèet testovaných pozic - 1
	datastr++;							// druhý znak hledaného øetìzce
	lenstr--;							// délka hledaného øetìzce - 1
	TCHAR* data = pData->Data + pos;	// zaèátek prohledávaného øetìzce

// cyklus hledání
	for (; i >= 0; i--)
	{
		if (*(data++) == chr)			// shoduje se první znak?
		{
			if (MemCompare(data, datastr, lenstr*sizeof(TCHAR)))	// shoduje se zbytek øetìzce?
			{
				return (data - pData->Data - 1);	// návrat pozice zaèátku øetìzce
			}
		}
	}

	return -1;
}


/////////////////////////////////////////////////////////////////////////////
// hledání znaku v øetìzci (<0 = nenalezeno, pos=výchozí pozice)

int _fastcall CText::Find(const TCHAR chr) const
{
	TCHAR* data = pData->Data;
	while ((*data != 0) && (*data != chr))
	{
		data++;
	}

	if (*data == 0)
	{
		return -1;
	}
	
	return data - pData->Data;
}

int _fastcall CText::Find(const TCHAR chr, int pos) const
{
	if (pos < 0) pos = 0;
	if (pos >= pData->Length) return -1;

	TCHAR* data = pData->Data + pos;
	while ((*data != 0) && (*data != chr))
	{
		data++;
	}

	if (*data == 0)
	{
		return -1;
	}
	
	return data - pData->Data;
}


/////////////////////////////////////////////////////////////////////////////
// hledání znaku od konce øetìzce zpìt (<0 = nenalezeno, pos=výchozí pozice)

int _fastcall CText::RevFind(const TCHAR chr) const
{
	int i = pData->Length;
	TCHAR* data = pData->Data + i;
	i--;

	for (; i >= 0; i--)
	{
		data--;
		if (*data == chr) break;
	}

	return i;
}

int _fastcall CText::RevFind(const TCHAR chr, int pos) const
{
	if (pos > pData->Length) pos = pData->Length;

	TCHAR* data = pData->Data + pos;
	pos--;

	for (; pos >= 0; pos--)
	{
		data--;
		if (*data == chr) break;
	}

	return pos;
}

/////////////////////////////////////////////////////////////////////////////
// vyprázdnìní øetìzce (s pøípadným zrušením bufferu)

void CText::Empty()
{ 
	Term();
	Init();
}


/////////////////////////////////////////////////////////////////////////////
// konverze na velká písmena
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn

bool CText::UpperCase()
{
	if (!CopyWrite()) return false;
	::CharUpper(pData->Data);
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// konverze na malá písmena
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn

bool CText::LowerCase()
{
	if (!CopyWrite()) return false;
	::CharLower(pData->Data);
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// levá èást øetìzce (pøi chybì pamìti vrátí prázdný øetìzec)

CText _fastcall CText::Left(int count) const
{
	int len = pData->Length;

	if ((DWORD)count > (DWORD)len)
	{
		if (count < 0)
		{
			count = 0;
		}
		else
		{
			count = len;
		}
	}
	return CText(pData->Data, count);
}


/////////////////////////////////////////////////////////////////////////////
// pravá èást øetìzce (pøi chybì pamìti vrátí prázdný øetìzec)

CText _fastcall CText::Right(int count) const
{
	int len = pData->Length;

	if ((DWORD)count > (DWORD)len)
	{
		if (count < 0)
		{
			count = 0;
		}
		else
		{
			count = len;
		}
	}
	return CText(pData->Data + len - count, count);
}


/////////////////////////////////////////////////////////////////////////////
// støední èást øetìzce (pøi chybì pamìti vrátí prázdný øetìzec)

CText _fastcall CText::Mid(int first, int count) const
{
	int len = pData->Length;

	if ((DWORD)first > (DWORD)len)
	{
		if (first < 0)
		{
			first = 0;
		}
		else
		{
			first = len;
		}
	}

	if (count < 0) count = 0;
	if ((DWORD)(first + count) > (DWORD)len) count = len - first;
	
	return CText(pData->Data + first, count);
}


/////////////////////////////////////////////////////////////////////////////
// zrušení textu z konce øetìzce
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn

bool CText::Delete(int first, int count)
{
	int len = pData->Length;

	if ((DWORD)first > (DWORD)len)
	{
		if (first < 0)
		{
			first = 0;
		}
		else
		{
			first = len;
		}
	}

	if (count < 0) count = 0;
	if ((DWORD)(first + count) > (DWORD)len) count = len - first;

	if (count > 0)
	{
		if (!CopyWrite()) return false;
		int next = first + count;
		MemCopy(pData->Data + first, pData->Data + next, (len - next + 1)*sizeof(TCHAR));
		pData->Length -= count;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// zrušení posledního znaku z konce øetìzce
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn

bool CText::DeleteLast()
{
	if (pData->Length > 0)
	{
		if (!CopyWrite()) return false;
		pData->Length -= 1;
		pData->Data[pData->Length] = 0;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// odstranìní mezer ze zaèátku/konce øetìzce
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn

bool CText::TrimLeft()
{
// nalezení platných dat nebo konce textu
	TCHAR* data = pData->Data;		// ukazatel textu
	while ((*data > 0) && (*data <= _T(' '))) data++;

// test, zda bude nìco vypouštìno
	int dif = data - pData->Data;
	if (dif > 0)
	{

// pøivlastnìní bufferu
		if (!CopyWrite()) return false;

// zrušení dat
		pData->Length -= dif;
		MemCopy(pData->Data, data, (pData->Length+1)*sizeof(TCHAR));
	}
	return true;
}

bool CText::TrimRight()
{
// nalezení konce platných dat
	int i = pData->Length;			// délka textu
	TCHAR* data = pData->Data + i;	// adresa za koncem textu

	for (; i > 0; i--)				// pro všechny znaky textu
	{
		data--;						// posun adresy
#ifdef _UNICODE
		if ((WORD)*data > (WORD)' ') break;	// pro platný znak konec
#else
		if ((BYTE)*data > (BYTE)' ') break;	// pro platný znak konec
#endif
	}

// test, zda se délka textu mìní
	if (i != pData->Length)
	{

// pøivlastnìní bufferu
		if (!CopyWrite()) return false;

// nová délka dat
		pData->Length = i;				// nová délka textu
		pData->Data[i] = 0;				// oznaèení konce textu
	}
	return true;
}


////////////////////////////////////////////////////////////////////
// nastavení èísla jména objektu (1, 2, ...)
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn

const CText DefObjName(_T('#'));	// implicitní jméno

bool _fastcall CText::SetNumObj(const int num)
{
// pracovní kopie textu (aby nebyl obsah zmìnìn pøi chybì)
	CText text(*this);

// vypuštìní èíslic a mezer z konce jména
	for (;;)
	{
		TCHAR znak = text.LastChar();	// naètení posledního znaku
		if ((znak == ' ') ||			// je mezera
			(znak == 9) ||				// tabulátor
			((znak >= '0') &&			// nebo èíslice
			(znak <= '9')))
		{
			if (!text.DeleteLast()) return false;	// vypuštìní posledního znaku
		}
		else
		{
			break;
		}
	}

// implicitní jméno, je-li prázdný text
	if (text.IsEmpty())
	{
		text = DefObjName;
	}

// èíslo se nastavuje jen pro 2 a více
	if (num > 1)
	{
		if (!text.Add(_T(' ')) ||
			!text.AddInt(num)) return false;
	}

// vše OK - uložení textu
	*this = text;
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// pøidání textu
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn

bool _fastcall CText::Add(const CText& str)
{
	int len1 = pData->Length;
	int len2 = str.pData->Length;
	if (!SizeBuffer(len1+len2)) return false;
	MemCopy(pData->Data + len1, str.pData->Data, len2*sizeof(TCHAR));
	return true;
}

bool _fastcall CText::Add(LPCTSTR txt)
{
	int len1 = pData->Length;
	int len2 = 0;
	if (txt) len2 = ::lstrlen(txt);
	if (!SizeBuffer(len1+len2)) return false;
	MemCopy(pData->Data + len1, txt, len2*sizeof(TCHAR));
	return true;
}

bool _fastcall CText::Add(const TCHAR chr)
{
	int len = pData->Length;
	if (!SizeBuffer(len+1)) return false;
	pData->Data[len] = chr;
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// pøidání textu èísla
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn

bool _fastcall CText::AddInt(const int num)
{
	CText txt;
	if (!txt.Int(num)) return false;
	return Add(txt);
}

bool _fastcall CText::AddDouble(const double num)
{
	CText txt;
	if (!txt.Double(num)) return false;
	return Add(txt);
}


/////////////////////////////////////////////////////////////////////////////
// souèet textù
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn

bool _fastcall CText::Add(const CText& str1, const CText& str2)
{ 
// délka textù
	int len1 = str1.pData->Length;
	int len2 = str2.pData->Length;

// vytvoøení bufferu
	TEXTDATA* data = NewTextData(len1+len2);
	if (data == NULL) return false;

// naètení textù
	MemCopy(data->Data, str1.pData->Data, len1*sizeof(TCHAR));
	MemCopy(data->Data + len1, str2.pData->Data, len2*sizeof(TCHAR));

// odpojení starých dat
	detach(pData);

// pøipojení nových dat
	pData = data;

	return true;
}

bool _fastcall CText::Add(LPCTSTR txt, const CText& str) 
{ 
// délka textù
	int len1 = 0;
	if (txt != NULL) len1 = ::lstrlen(txt);
	int len2 = str.pData->Length;

// vytvoøení bufferu
	TEXTDATA* data = NewTextData(len1+len2);
	if (data == NULL) return false;

// naètení textù
	MemCopy(data->Data, txt, len1*sizeof(TCHAR));
	MemCopy(data->Data + len1, str.pData->Data, len2*sizeof(TCHAR));

// odpojení starých dat
	detach(pData);

// pøipojení nových dat
	pData = data;

	return true;
}

bool _fastcall CText::Add(const CText& str, LPCTSTR txt) 
{ 
// délka textù
	int len1 = str.pData->Length;
	int len2 = 0;
	if (txt != NULL) len2 = ::lstrlen(txt);

// vytvoøení bufferu
	TEXTDATA* data = NewTextData(len1+len2);
	if (data == NULL) return false;

// naètení textù
	MemCopy(data->Data, str.pData->Data, len1*sizeof(TCHAR));
	MemCopy(data->Data + len1, txt, len2*sizeof(TCHAR));

// odpojení starých dat
	detach(pData);

// pøipojení nových dat
	pData = data;

	return true;
}

bool _fastcall CText::Add(const TCHAR chr, const CText& str) 
{ 
// délka textu
	int len = str.pData->Length;

// vytvoøení bufferu
	TEXTDATA* data = NewTextData(1+len);
	if (data == NULL) return false;

// naètení textù
	data->Data[0] = chr;
	MemCopy(data->Data + 1, str.pData->Data, len*sizeof(TCHAR));

// odpojení starých dat
	detach(pData);

// pøipojení nových dat
	pData = data;

	return true;
}

bool _fastcall CText::Add(const CText& str, const TCHAR chr) 
{ 
// délka textu
	int len = str.pData->Length;

// vytvoøení bufferu
	TEXTDATA* data = NewTextData(len+1);
	if (data == NULL) return false;

// naètení textù
	MemCopy(data->Data, str.pData->Data, len*sizeof(TCHAR));
	data->Data[len] = chr;

// odpojení starých dat
	detach(pData);

// pøipojení nových dat
	pData = data;

	return true;
}


/////////////////////////////////////////////////////////////////////////////
// nastavení textu
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn

void _fastcall CText::Set(const CText& str)
{
	detach(pData);			// zrušení starých dat
	attach(str.pData);		// pøipojení nových dat
}

bool _fastcall CText::Set(const TCHAR chr)
{
// vytvoøení bufferu dat
	TEXTDATA* data = NewTextData(1);
	if (data == NULL) return false;

// naplnìní bufferu
	data->Data[0] = chr;

// odpojení starých dat
	detach(pData);

// pøipojení nových dat
	pData = data;

	return true;
}

bool _fastcall CText::Set(LPCTSTR txt)
{
// délka textu
	int len = 0;
	if (txt != NULL) len = ::lstrlen(txt);

// vytvoøení bufferu pro data
	TEXTDATA* data = NewTextData(len);	
	if (data == NULL) return false;

// kopie textu
	MemCopy(data->Data, txt, len*sizeof(TCHAR));

// odpojení starých dat
	detach(pData);

// pøipojení nových dat
	pData = data;

	return true;
}

bool _fastcall CText::Set(LPCTSTR txt, const int length)
{
// délka textu
	int len = 0;
	if (txt != NULL)
	{
		len = length;
		if (len < 0) len = ::lstrlen(txt);
	}

// vytvoøení bufferu pro data
	TEXTDATA* data = NewTextData(len);	
	if (data == NULL) return false;

// kopie textu
	MemCopy(data->Data, txt, len*sizeof(TCHAR));

// odpojení starých dat
	detach(pData);

// pøipojení nových dat
	pData = data;

	return true;
}

#ifdef _UNICODE

bool _fastcall CText::Set(LPCSTR txt)
{
// délka textu
	int len = 0;
	if (txt != NULL) len = ::lstrlenA(txt);

// vytvoøení bufferu pro data
	TEXTDATA* data = NewTextData(len);	
	if (data == NULL) return false;

// kopie textu
	::MultiByteToWideChar(CP_ACP, 0, txt, len, data->Data, len);

// odpojení starých dat
	detach(pData);

// pøipojení nových dat
	pData = data;

	return true;
}

bool _fastcall CText::Set(LPCSTR txt, const int length)
{
// délka textu
	int len = 0;
	if (txt != NULL)
	{
		len = length;
		if (len < 0) len = ::lstrlenA(txt);
	}

// vytvoøení bufferu pro data
	TEXTDATA* data = NewTextData(len);	
	if (data == NULL) return false;

// kopie textu
	::MultiByteToWideChar(CP_ACP, 0, txt, len, data->Data, len);

// odpojení starých dat
	detach(pData);

// pøipojení nových dat
	pData = data;

	return true;
}

#else //_UNICODE

bool _fastcall CText::Set(LPCWSTR txt)
{
// délka textu
	int len = 0;
	if (txt != NULL) len = ::lstrlenW(txt);

// vytvoøení bufferu pro data
	TEXTDATA* data = NewTextData(len);	
	if (data == NULL) return false;

// kopie textu
	::WideCharToMultiByte(CP_ACP, 0, txt, len, data->Data, len, NULL, NULL);

// odpojení starých dat
	detach(pData);

// pøipojení nových dat
	pData = data;

	return true;
}

bool _fastcall CText::Set(LPCWSTR txt, const int length)
{
// délka textu
	int len = 0;
	if (txt != NULL)
	{
		len = length;
		if (len < 0) len = ::lstrlenW(txt);
	}

// vytvoøení bufferu pro data
	TEXTDATA* data = NewTextData(len);	
	if (data == NULL) return false;

// kopie textu
	::WideCharToMultiByte(CP_ACP, 0, txt, len, data->Data, len, NULL, NULL);

// odpojení starých dat
	detach(pData);

// pøipojení nových dat
	pData = data;

	return true;
}

#endif //_UNICODE


/////////////////////////////////////////////////////////////////////////////
// naètení textu z resource (pøi chybì vrací FALSE, pùvodní obsah nezmìnìn)

bool CText::Load(const int nID)
{
// naètení textu do bufferu
	TCHAR buf[0x201];
	int len = ::LoadString(hInstance, nID, buf, 0x200);
	if (len <= 0) return false;

// nastavení textu
	if (len > 0x200) len = 0x200;
	if (!Set(buf, len)) return false;

// korekce výskytu nuly v textu
	KorigNul();

// pøíznak - naèteno OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// korekce výskytu nuly v textu

void CText::KorigNul()
{
	int len = ::lstrlen(pData->Data);
	if ((len < pData->Length) && (len >= 0))
	{
		pData->Length = len;
		pData->Data[len] = 0;
	}
}


/////////////////////////////////////////////////////////////////////////////
// naètení ze souboru formátu TXT (FALSE=chyba, obsah nezmìnìn)
/*
bool CText::LoadFile(CText jmeno)
{
// otevøení souboru mapovaného do pamìti (uzavøen pøi destrukci!)
	CFileMap file;
	if (!file.Open(jmeno)) return false;

// naètení textu
	if (!Set((char*)file.Adr(), file.Size())) return false;

// korekce výskytu nuly v textu
	KorigNul();

// pøíznak - naèteno OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// uložení textu do souboru formátu TXT (FALSE=chyba)

bool CText::SaveFile(CText jmeno) const
{	
// vytvoøení souboru
	CFile file;
	file.Name(jmeno);
	if (!file.Create()) return false;

#ifdef _UNICODE
	
// buffer k dekódování souboru
	char* buf = (char*) MemGet(pData->Length + 100);
	if (buf == NULL)
	{
		file.Close();
		file.Delete();
		return false;
	}		
	Write(buf);
	
// uložení souboru
	BOOL result = file.Write(buf, pData->Length);

// zrušení bufferu
	MemFree(buf);

#else

// uložení souboru
	BOOL result = file.Write(pData->Data, pData->Length);

#endif

// uzavøení souboru
	file.Close();

// pøi chybì zrušení souboru
	if (!result)
	{
		file.Delete();
		return false;
	}

// pøíznak - uloženo OK
	return true;
}
*/

/////////////////////////////////////////////////////////////////////////////
// kopie do vlastního bufferu pøed modifikací
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn

bool CText::CopyWrite()
{
// úschova ukazatelù
	TEXTDATA* olddata = pData;	// adresa starých dat
	long* refer = &(olddata->Refer);// poèet referencí

// test, zda je nutné pøivlastnìní
	if (*refer > 1)					// je nìjaký jiný majitel?
	{

// vytvoøení nového bufferu
		TEXTDATA* newdata = NewTextData(olddata->Length);
		if (newdata == NULL) return false;

// pøenesení dat
		MemCopy(newdata->Data, olddata->Data, olddata->Length*sizeof(TCHAR));

// odpojení starých dat
		detach(olddata);

// pøipojení nových dat
		pData = newdata;
	}

// operace OK
	return true;
}


//////////////////////////////////////////////////////////////////////////////
// poskytnutí znaku na pozici s kontrolou (pro neplatný znak vrátí 0)

TCHAR _fastcall CText::Get(const int index) const
{ 
	if (IsValid(index))
	{
		return pData->Data[index];
	}
	return 0;
}


//////////////////////////////////////////////////////////////////////////////
// poskytnutí posledního znaku øetìzce (pro prázdný øetìzec vrací 0)

TCHAR _fastcall CText::LastChar() const
{
	int len = pData->Length;
	if (len <= 0)
	{
		return 0;
	}
	else
	{
		return pData->Data[len-1];
	}
}


//////////////////////////////////////////////////////////////////////////////
// nastavení znaku na pozici s kontrolou (zajistí pøivlastnìní bufferu, pøi chybì pamìti vrací FALSE)

bool _fastcall CText::Set(const int index, const TCHAR chr)
{
	if (IsValid(index))
	{
		if (!CopyWrite()) return false;
		pData->Data[index] = chr;
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////////
// naètení textu okna (pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn)

bool CText::GetWindowText(const HWND wnd)
{
// vytvoøení nového bufferu
	TEXTDATA* data = NewTextData(::GetWindowTextLength(wnd));
	if (data == NULL) return false;

// naètení dat
	::GetWindowText(wnd, data->Data, data->Length+1);
	data->Data[data->Length] = 0;

// odpojení starých dat
	detach(pData);

// pøipojení nových dat
	pData = data;

// ochrana proti nule v textu
	KorigNul();

// operace OK
	return true;
}


//////////////////////////////////////////////////////////////////////////////
// nastavení textu okna

void _fastcall CText::SetWindowText(const HWND wnd) const
{
	::SetWindowText(wnd, pData->Data);
}


//////////////////////////////////////////////////////////////////////////////
// naètení textu dialogového prvku (pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn)

bool CText::GetDialogText(const HWND wnd, int id)
{
	HWND item = ::GetDlgItem(wnd, id);
	if (item != NULL)
	{
		return GetWindowText(item);
	}

	Empty();
	return true;
}


//////////////////////////////////////////////////////////////////////////////
// nastavení textu dialogového prvku

void _fastcall CText::SetDialogText(const HWND wnd, int id) const
{
	HWND item = ::GetDlgItem(wnd, id);
	if (item != NULL)
	{
		::SetWindowText(item, pData->Data);
	}
}


//////////////////////////////////////////////////////////////////////////////
// naètení aktivního adresáøe (pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn)

bool CText::GetAktDir()
{
// naètení textu do bufferu
	TCHAR buf[_MAX_PATH+1];
	int len = (int)::GetCurrentDirectory(_MAX_PATH, buf);

// nastavení textu
	if (len < 0) len = 0;
	if (len > _MAX_PATH) len = _MAX_PATH;
	if (!Set(buf, len)) return false;

// korekce výskytu nuly v textu
	KorigNul();

// pøíznak - naèteno OK
	return true;
}


//////////////////////////////////////////////////////////////////////////////
// korekce textu na jméno souboru (vypuštìní zakázaných znakù, vrací opravenou pozici kurzoru)
// ignoruje chybu pamìti pøi pøivlastnìní bufferu

int CText::FileName(int curs)
{
	for (int i = pData->Length-1; i >= 0; i--)
	{
		TCHAR znak = pData->Data[i];
		if ((BYTE)znak < 32) znak = 0;

		switch(znak)
		{
		case 0:
		case _T('*'):
		case _T('|'):
		case _T('\\'):
		case _T(':'):
		case _T('"'):
		case _T('<'):
		case _T('>'):
		case _T('?'):
		case _T('/'):
			Delete(i,1);
			if (curs > i) curs--;
		}
	}
	return curs;
}


//////////////////////////////////////////////////////////////////////////////
// korekce textu na jméno cesty (vypuštìní zakázaných znakù, vrací opravenou pozici kurzoru)
// ignoruje chybu pamìti pøi pøivlastnìní bufferu

int CText::PathName(int curs)
{
	for (int i = pData->Length-1; i >= 0; i--)
	{
		TCHAR znak = pData->Data[i];
		if ((BYTE)znak < 32) znak = 0;

		switch(znak)
		{
		case _T(':'):
			if (RevFind('\\', i) < 0) break;

		case 0:
		case _T('*'):
		case _T('|'):
		case _T('"'):
		case _T('<'):
		case _T('>'):
		case _T('?'):
		case _T('/'):
			Delete(i,1);
			if (curs > i) curs--;
		}
	}
	return curs;
}


//////////////////////////////////////////////////////////////////////////////
// konverze èísla INT na text (pøi chybì pamìti vrací FALSE)

bool CText::Int(int num)
{
// pøíprava bufferu
	TCHAR buf[16];
	TCHAR* dst = buf+16;
	int len = 0;

// pøíprava znaménka
	bool sign = false;
	if (num < 0)
	{
		sign = true;
		num = -num;
	}

// konverze èíslic
	do {
		dst--;
		*dst = (TCHAR)(((DWORD)num % 10) + '0');
		*(DWORD*)(&num) /= 10;
		len++;
	} while (num != 0);

// uložení znaménka
	if (sign)
	{
		dst--;
		*dst = '-';
		len++;
	}

// kopie do bufferu
	return Set(dst, len);
}


//////////////////////////////////////////////////////////////////////////////
// konverze textu na èíslo INT

int Int(LPCTSTR txt)
{
// pøíprava promìnných
	DWORD result = 0;
	bool sign = false;
	TCHAR chr;

// naètení znaménka
	while ((chr = *txt) != 0)
	{
		if (chr == '-')
		{
			sign = !sign;
		}
		else
		{
			if ((chr != '+') && (chr != ' ') && (chr != 9))
			{
				break;
			}
		}
		txt++;
	}			
	
// naètení èíslic
	while ((chr = *txt) != 0)
	{
		if ((chr >= '0') && (chr <= '9'))
		{
			if (result <= (DWORD)(0x7fffffff / 10))
			{
				result *= 10;
				result += (DWORD)(chr - '0');
			}
			else
			{
				result = 0x80000000;
			}

			if (result > 0x80000000)
			{
				result = 0x80000000;
			}
		}
		else
		{
			break;
		}
		txt++;
	}

// návrat s korekcí znaménka
	if (sign)
	{
		return -(int)result;
	}

	if (result >= 0x80000000)
	{
		return 0x7fffffff;
	}

	return result;
}	


//////////////////////////////////////////////////////////////////////////////
// konverze èísla DWORD na HEX (8 èíslic) (pøi chybì pamìti vrací FALSE)

bool CText::Hex(DWORD num)
{
// pøíprava bufferu
	TEXTDATA* data = NewTextData(8);
	if (data == NULL) return false;
	TCHAR* dst = data->Data + 8;

// dekódování èíslic
	TCHAR chr;
	for (int i = 8; i > 0; i--)
	{
		dst--;
		chr = (TCHAR)(num & 0xf);
		if (chr <= 9)
		{
			*dst = (TCHAR)(chr + '0');
		}
		else
		{
			*dst = (TCHAR)(chr - 10 + 'A');
		}

		num >>= 4;
	}

// odpojení starých dat
	detach(pData);

// pøipojení nových dat
	pData = data;

	return true;
}

//////////////////////////////////////////////////////////////////////////////
// konverze èísla DOUBLE na text (pøi chybì pamìti vrací FALSE)

const CText InfDoubleText(_T("1#INF"), 5);
const CText NulDoubleText(_T('0'));

bool CText::Double(double num)
{
// pøeteèení "1#INF"
	if (*(ULONGLONG*)(&num) == DOUBLE_INFINITY_VALUE)
	{
		Set(InfDoubleText);
		return true;
	}

// dekódování nuly
	if (num == 0)
	{
		Set(NulDoubleText);
		return true;
	}

// pøíprava znaménka
	bool sign = false;
	if (num < 0)
	{
		sign = true;
		num = -num;
	}

// rozdìlení èísla na exponent a mantisu
	int expI = 0;
	double mantD = num;

	if (mantD >= 1)
	{
		if (mantD >= 1e256) { mantD *= 1e-256; expI += 256; };
		if (mantD >= 1e128) { mantD *= 1e-128; expI += 128; };
		if (mantD >= 1e64)  { mantD *= 1e-64;  expI += 64;  };
		if (mantD >= 1e32)  { mantD *= 1e-32;  expI += 32;  };
		if (mantD >= 1e16)  { mantD *= 1e-16;  expI += 16;  };
		if (mantD >= 1e8)   { mantD *= 1e-8;   expI += 8;   };
		if (mantD >= 1e4)   { mantD *= 1e-4;   expI += 4;   };
		if (mantD >= 1e2)   { mantD *= 1e-2;   expI += 2;   };
		if (mantD >= 1e1)   { mantD *= 1e-1;   expI += 1;   };
	}
	else
	{
		if (mantD < 1e-256) { mantD *= 1e257;  expI -= 257; };
		if (mantD < 1e-128) { mantD *= 1e129;  expI -= 129; };
		if (mantD < 1e-64)  { mantD *= 1e65;   expI -= 65;  };
		if (mantD < 1e-32)  { mantD *= 1e33;   expI -= 33;  };
		if (mantD < 1e-16)  { mantD *= 1e17;   expI -= 17;  };
		if (mantD < 1e-8)   { mantD *= 1e9;    expI -= 9;   };
		if (mantD < 1e-4)   { mantD *= 1e5;    expI -= 5;   };
		if (mantD < 1e-2)   { mantD *= 1e3;    expI -= 3;   };
		if (mantD < 1e-1)   { mantD *= 1e2;    expI -= 2;   };
		if (mantD < 1e0)    { mantD *= 1e1;    expI -= 1;   };
	}

	mantD *= 1e7;
	mantD += 6e-8;

	if (mantD >= 1e8)
	{
		mantD /= 10;
		expI++;
	}

	int mantH = (int)(mantD);
	int mantL = (int)((mantD - mantH)*1e7);

// dekódování mantisy
	int i;
	TCHAR mantT[15];
	TCHAR* mantP = mantT+15;
	for (i = 7; i > 0; i--)
	{
		mantP--;
		*mantP = (TCHAR)((mantL % 10) + '0');
		mantL /= 10;
	}
	for (i = 8; i > 0; i--)
	{
		mantP--;
		*mantP = (TCHAR)((mantH % 10) + '0');
		mantH /= 10;
	}
	
// zjištìní poètu platných èíslic
	TCHAR* dst = mantT+15;
	int digits = 15;
	for (; digits > 1; digits--)
	{
		dst--;
		if (*dst != '0') break;
	}

// pøíprava znaménka do výchozího bufferu
	TCHAR buf[30];
	dst = buf;
	if (sign)
	{
		*dst = '-';
		dst++;
	}

// dekódování èísla bez exponentu, èíslo >= 1
	if ((expI < 15) && (expI >= 0))
	{
		for (;;)
		{
			*dst = *mantP;
			dst++;
			mantP++;
			digits--;
			if ((expI <= 0) && (digits <= 0)) break;
			if (expI == 0)
			{
				*dst = '.';
				dst++;
			}
			expI--;
		}
	}
	else

// dekódování èísla bez exponentu, èíslo < 1
	if ((expI < 0) && (expI >= -3))
	{
		*dst = '0';
		dst++;
		*dst = '.';
		dst++;

		expI++;
		for (;expI < 0; expI++)
		{
			*dst = '0';
			dst++;
		}

		for (;digits > 0; digits--)
		{
			*dst = *mantP;
			dst++;
			mantP++;
		}
	}
	else

// dekódování èísla s exponentem
	{
		*dst = *mantP;
		dst++;
		mantP++;
		digits--;

		if (digits > 0)
		{
			*dst = '.';
			dst++;

			for (; digits > 0; digits--)
			{
				*dst = *mantP;
				dst++;
				mantP++;
			}
		}

		*dst = 'e';
		dst++;

		if (expI < 0)
		{
			*dst = '-';
			dst++;
			expI = -expI;
		}

		if (expI > 99)
		{
			*dst = (TCHAR)(expI/100 + '0');
			expI = expI % 100;
			dst++;

			*dst = (TCHAR)(expI/10 + '0');
			expI = expI % 10;
			dst++;
		}
		else
			if (expI > 9)
			{
				*dst = (TCHAR)(expI/10 + '0');
				expI = expI % 10;
				dst++;
			}

		*dst = (TCHAR)(expI + '0');
		dst++;
	}

// uložení èísla do bufferu
	return Set(buf, dst-buf);
}


//////////////////////////////////////////////////////////////////////////////
// konverze textu na èíslo DOUBLE

double Double(LPCTSTR txt)
{
// pøíprava promìnných
	double result = 0;			// výsledek
	int expN = 0;				// exponent
	double zlomek = 1;			// zlomek desetinné èásti
	bool sign = false;			// pøíznak záporného znaménka
	bool signE = false;			// znaménko exponentu
	TCHAR chr = *txt;			// naètený znak

// naètení znaménka
	while (chr != 0)
	{
		if (chr == '-')
		{
			sign = !sign;
		}
		else
		{
			if ((chr != '+') && (chr != ' ') && (chr != 9))
			{
				break;
			}
		}
		txt++;
		chr = *txt;
	}			
	
// naètení èíslic celé èásti
	while (chr != 0)
	{
		if ((chr >= '0') && (chr <= '9'))
		{
			result *= 10;
			result += chr - '0';
		}
		else
		{
			break;
		}
		txt++;
		chr = *txt;
	}

// naètení èíslic desetinné èásti
	if ((chr == '.') || (chr == ','))
	{
		txt++;
		chr = *txt;

		while (chr != 0)
		{
			if ((chr >= '0') && (chr <= '9'))
			{
				zlomek *= 10;
				result += (chr - '0')/zlomek;
			}
			else
			{
				break;
			}
			txt++;
			chr = *txt;
		}
	}

// vypuštìní mezer
	while ((chr == ' ') || (chr == 9))
	{
		txt++;
		chr = *txt;
	}

// test, zda je exponent
	if ((chr == 'E') || (chr == 'e'))
	{
		txt++;
		chr = *txt;

// vypuštìní mezer a urèení znaménka
		while ((chr == ' ') || (chr == 9) || (chr == '+') || (chr == '-'))
		{
			if (chr == '-')
			{
				signE = !signE;
			}
			txt++;
			chr = *txt;
		}

// naètení èíslic exponentu
		while (chr != 0)
		{
			if ((chr >= '0') && (chr <= '9'))
			{
				expN *= 10;
				expN += chr - '0';
			}
			else
			{
				break;
			}
			txt++;
			chr = *txt;
		}

// vynásobení èísla exponentem
		if (expN != 0)
		{
			if (signE)
			{
				if (expN >= 256) { result *= 1e-256; expN -= 256; };
				if (expN >= 128) { result *= 1e-128; expN -= 128; };
				if (expN >=  64) { result *=  1e-64; expN -=  64; };
				if (expN >=  32) { result *=  1e-32; expN -=  32; };
				if (expN >=  16) { result *=  1e-16; expN -=  16; };
				if (expN >=   8) { result *=   1e-8; expN -=   8; };
				if (expN >=   4) { result *=   1e-4; expN -=   4; };
				if (expN >=   2) { result *=   1e-2; expN -=   2; };
				if (expN >=   1) { result *=   1e-1;              };
			}
			else
			{
				if (expN >= 256) { result *= 1e256; expN -= 256; };
				if (expN >= 128) { result *= 1e128; expN -= 128; };
				if (expN >=  64) { result *=  1e64; expN -=  64; };
				if (expN >=  32) { result *=  1e32; expN -=  32; };
				if (expN >=  16) { result *=  1e16; expN -=  16; };
				if (expN >=   8) { result *=   1e8; expN -=   8; };
				if (expN >=   4) { result *=   1e4; expN -=   4; };
				if (expN >=   2) { result *=   1e2; expN -=   2; };
				if (expN >=   1) { result *=   1e1;              };
			}
		}
	}

// pøíznak pøeteèení
	if (chr == '#')
	{
		if ((txt[1] == 'I') &&
			(txt[2] == 'N') &&
			(txt[3] == 'F') &&
			((result == 1) || (result == 0)))
		{
			*(ULONGLONG*)(&result) = DOUBLE_INFINITY_VALUE;
			return result;
		}
	}

// návrat s korekcí znaménka
	if (sign)
	{
		return -result;
	}
	return result;
}	


/////////////////////////////////////////////////////////////////////////////
// operátor porovnání

BOOL _fastcall operator==(const CText& str1, const CText& str2)
{ 
	int delka = str1.Length();
	return ((delka == str2.Length()) &&
		MemCompare(str1.DataData(), str2.DataData(), delka*sizeof(TCHAR)));
};

BOOL _fastcall operator==(LPCTSTR txt, const CText& str)
{ 
	int delka = 0;
	if (txt) delka = ::lstrlen(txt);
	return ((delka == str.Length()) &&
		MemCompare(str.DataData(), txt, delka*sizeof(TCHAR)));
};

BOOL _fastcall operator==(const CText& str, LPCTSTR txt)
{ 
	int delka = 0;
	if (txt) delka = ::lstrlen(txt);
	return ((delka == str.Length()) &&
		MemCompare(str.DataData(), txt, delka*sizeof(TCHAR)));
};

BOOL _fastcall operator==(const CText& str, const TCHAR chr)
{ 
	return ((str.Length() == 1) && (str[0] == chr));
};

BOOL _fastcall operator==(const TCHAR chr, const CText& str)
{ 
	return ((str.Length() == 1) && (str[0] == chr));
};

BOOL _fastcall operator!=(const CText& str1, const CText& str2)
{ 
	int delka = str1.Length();
	return ((delka != str2.Length()) || !MemCompare(str1.DataData(),
			str2.DataData(), delka*sizeof(TCHAR)));
};

BOOL _fastcall operator!=(LPCTSTR txt, const CText& str)
{ 
	int delka = 0;
	if (txt) delka = ::lstrlen(txt);
	return ((delka != str.Length()) || !MemCompare(str.DataData(),
			txt, delka*sizeof(TCHAR)));
};

BOOL _fastcall operator!=(const CText& str, LPCTSTR txt)
{ 
	int delka = 0;
	if (txt) delka = ::lstrlen(txt);
	return ((delka != str.Length()) || !MemCompare(str.DataData(),
			txt, delka*sizeof(TCHAR)));
};

BOOL _fastcall operator!=(const CText& str, const TCHAR chr)
{ 
	return ((str.Length() != 1) || (str[0] != chr));
};

BOOL _fastcall operator!=(const TCHAR chr, const CText& str)
{ 
	return ((str.Length() != 1) || (str[0] != chr));
};

bool _fastcall operator< (const CText& str1, const CText& str2)
{
	return (::CompareString(
				LOCALE_USER_DEFAULT, 
				0, 
				str1.DataData(),
				str1.Length(),
				str2.DataData(),
				str2.Length()) < 2);
}

bool _fastcall operator<=(const CText& str1, const CText& str2)
{
	return (::CompareString(
				LOCALE_USER_DEFAULT, 
				0, 
				str1.DataData(),
				str1.Length(),
				str2.DataData(),
				str2.Length()) <= 2);
}

bool _fastcall operator> (const CText& str1, const CText& str2)
{
	return (::CompareString(
				LOCALE_USER_DEFAULT, 
				0, 
				str1.DataData(),
				str1.Length(),
				str2.DataData(),
				str2.Length()) > 2);
}

bool _fastcall operator>=(const CText& str1, const CText& str2)
{
	return (::CompareString(
				LOCALE_USER_DEFAULT, 
				0, 
				str1.DataData(),
				str1.Length(),
				str2.DataData(),
				str2.Length()) >= 2);
}


