
#include "Main.h"

/***************************************************************************\
*																			*
*								Textov� �et�zce								*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inicializa�n� pr�zdn� �et�zec (modifikuje se po�et referenc�!)

TEXTDATA	EmptyTextData		= { 1, 0, 0 };
const CText		EmptyText;		// pr�zdn� �et�zec

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� dat textu - d�lka zad�na ve znac�ch (p�i chyb� pam�ti vrac� NULL)

TEXTDATA* _fastcall NewTextData(int length)
{
	ASSERT(length >= 0);

// vytvo�en� bufferu
	TEXTDATA* data = (TEXTDATA*)MemGet((length+1)*sizeof(TCHAR) + SIZEOFTEXTDATA);
	if (data != NULL)
	{

// nastaven� parametr�
		data->Refer = 1;				// po�et referenc�
		data->Length = length;			// d�lka
		data->Data[length] = 0;			// ozna�en� konce textu
	}

// adresa dat
	return data;
}


/////////////////////////////////////////////////////////////////////////////
// zru�en� dat textu (odd�leno kv�li lep�� optimalizaci)

void _fastcall DelTextData(TEXTDATA* data)
{
	ASSERT(data != NULL);

	MemFree(data);
}

/////////////////////////////////////////////////////////////////////////////
// zm�na velikosti bufferu (d�lka zad�na ve znac�ch) - zajist� p�ivlastn�n� bufferu
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool CText::SizeBuffer(int length)
{
	ASSERT(length >= 0);

// test, zda se velikost bufferu m�n�
	if (length == pData->Length) return true;

// p�ivlastn�n� bufferu
	if (!CopyWrite()) return false;

// nov� velikost bufferu
	TEXTDATA* data = (TEXTDATA*)MemSize(pData, (length+1)*sizeof(TCHAR) + SIZEOFTEXTDATA);
	if (data == NULL) return false;

// nastaven� parametr�
	pData = data;					// adresa dat
	data->Length = length;			// d�lka
	data->Data[length] = 0;			// ozna�en� konce textu
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

CText::CText(const TCHAR chr) // p�i chyb� pam�ti bude �et�zec pr�zdn�
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

CText::CText(LPCTSTR text) // p�i chyb� pam�ti bude �et�zec pr�zdn�
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

CText::CText(LPCTSTR text, const int length) // p�i chyb� pam�ti bude �et�zec pr�zdn�
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

CText::CText(LPCSTR text) // p�i chyb� pam�ti bude �et�zec pr�zdn�
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

CText::CText(LPCSTR text, const int length) // p�i chyb� pam�ti bude �et�zec pr�zdn�
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

CText::CText(LPCWSTR text) // p�i chyb� pam�ti bude �et�zec pr�zdn�
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

CText::CText(LPCWSTR text, const int length) // p�i chyb� pam�ti bude �et�zec pr�zdn�
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
// slu�ovac� konstruktory (ur�eno pro oper�tor +) - p�i chyb� pam�ti bude �et�zec pr�zdn�

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
// statick� konstruktor a destruktor

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

bool _fastcall CText::Init(const TCHAR chr) // p�i chyb� pam�ti vr�t� FALSE, text nen� platn�
{
	pData = NewTextData(1);
	if (pData != NULL)
	{
		pData->Data[0] = chr;
		return true;
	}
	return false;
}

bool _fastcall CText::Init(LPCTSTR text) // p�i chyb� pam�ti vr�t� FALSE, text nen� platn�
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

bool _fastcall CText::Init(LPCTSTR text, const int length) // p�i chyb� pam�ti vr�t� FALSE, text nen� platn�
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

bool _fastcall CText::Init(LPCSTR text) // p�i chyb� pam�ti vr�t� FALSE, text nen� platn�
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

bool _fastcall CText::Init(LPCSTR text, const int length) // p�i chyb� pam�ti vr�t� FALSE, text nen� platn�
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

bool _fastcall CText::Init(LPCWSTR text) // p�i chyb� pam�ti vr�t� FALSE, text nen� platn�
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

bool _fastcall CText::Init(LPCWSTR text, const int length) // p�i chyb� pam�ti vr�t� FALSE, text nen� platn�
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
// z�pis do bufferu (bez koncov� nuly)

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
// z�pis do bufferu (s koncovou nulou)

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
// hled�n� textu v �et�zci (<0 = nenalezeno, pos=v�choz� pozice)

int _fastcall CText::Find(const CText& str) const
{
// hledan� �et�zec pr�zdn� - bude pozice 0
	int lenstr = str.pData->Length;
	if (lenstr == 0) return 0;

// hledan� �et�zec m� d�lku 1 - p�evod na hled�n� znaku
	TCHAR* datastr = str.pData->Data;
	TCHAR chr = datastr[0];
	if (lenstr == 1) return Find(chr);

// p��prava ukazatel�
	int i = pData->Length - lenstr;		// po�et testovan�ch pozic - 1
	datastr++;							// druh� znak hledan�ho �et�zce
	lenstr--;							// d�lka hledan�ho �et�zce - 1
	TCHAR* data = pData->Data;			// za��tek prohled�van�ho �et�zce

// cyklus hled�n�
	for (; i >= 0; i--)
	{
		if (*(data++) == chr)			// shoduje se prvn� znak?
		{
			if (MemCompare(data, datastr, lenstr*sizeof(TCHAR)))	// shoduje se zbytek �et�zce?
			{
				return (data - pData->Data - 1);	// n�vrat pozice za��tku �et�zce
			}
		}
	}

	return -1;
}

int _fastcall CText::Find(const CText& str, int pos) const
{
// omezen� a kontrola v�choz� pozice
	if (pos < 0) pos = 0;
	if (pos >= pData->Length) return -1;

// hledan� �et�zec pr�zdn� - bude v�choz� pozice
	int lenstr = str.pData->Length;
	if (lenstr == 0) return pos;

// hledan� �et�zec m� d�lku 1 - p�evod na hled�n� znaku
	TCHAR* datastr = str.pData->Data;
	TCHAR chr = datastr[0];
	if (lenstr == 1) return Find(chr, pos);

// p��prava ukazatel�
	int i = pData->Length - lenstr - pos;	// po�et testovan�ch pozic - 1
	datastr++;							// druh� znak hledan�ho �et�zce
	lenstr--;							// d�lka hledan�ho �et�zce - 1
	TCHAR* data = pData->Data + pos;	// za��tek prohled�van�ho �et�zce

// cyklus hled�n�
	for (; i >= 0; i--)
	{
		if (*(data++) == chr)			// shoduje se prvn� znak?
		{
			if (MemCompare(data, datastr, lenstr*sizeof(TCHAR)))	// shoduje se zbytek �et�zce?
			{
				return (data - pData->Data - 1);	// n�vrat pozice za��tku �et�zce
			}
		}
	}

	return -1;
}


/////////////////////////////////////////////////////////////////////////////
// hled�n� textu v �et�zci (<0 = nenalezeno, pos=v�choz� pozice)

int _fastcall CText::Find(LPCTSTR txt) const
{
// p��prava d�lky hledan�ho �et�zce
	int lenstr = 0;
	if (txt != NULL) lenstr = ::lstrlen(txt);

// hledan� �et�zec pr�zdn� - bude pozice 0
	if (lenstr == 0) return 0;

// hledan� �et�zec m� d�lku 1 - p�evod na hled�n� znaku
	const TCHAR* datastr = txt;
	TCHAR chr = datastr[0];
	if (lenstr == 1) return Find(chr);

// p��prava ukazatel�
	int i = pData->Length - lenstr;		// po�et testovan�ch pozic - 1
	datastr++;							// druh� znak hledan�ho �et�zce
	lenstr--;							// d�lka hledan�ho �et�zce - 1
	TCHAR* data = pData->Data;			// za��tek prohled�van�ho �et�zce

// cyklus hled�n�
	for (; i >= 0; i--)
	{
		if (*(data++) == chr)			// shoduje se prvn� znak?
		{
			if (MemCompare(data, datastr, lenstr*sizeof(TCHAR)))	// shoduje se zbytek �et�zce?
			{
				return (data - pData->Data - 1);	// n�vrat pozice za��tku �et�zce
			}
		}
	}

	return -1;
}

int _fastcall CText::Find(LPCTSTR txt, int pos) const
{
// omezen� a kontrola v�choz� pozice
	if (pos < 0) pos = 0;
	if (pos >= pData->Length) return -1;

// p��prava d�lky hledan�ho �et�zce
	int lenstr = 0;
	if (txt != NULL) lenstr = ::lstrlen(txt);

// hledan� �et�zec pr�zdn� - bude v�choz� pozice
	if (lenstr == 0) return pos;

// hledan� �et�zec m� d�lku 1 - p�evod na hled�n� znaku
	const TCHAR* datastr = txt;
	TCHAR chr = datastr[0];
	if (lenstr == 1) return Find(chr, pos);

// p��prava ukazatel�
	int i = pData->Length - lenstr - pos;	// po�et testovan�ch pozic - 1
	datastr++;							// druh� znak hledan�ho �et�zce
	lenstr--;							// d�lka hledan�ho �et�zce - 1
	TCHAR* data = pData->Data + pos;	// za��tek prohled�van�ho �et�zce

// cyklus hled�n�
	for (; i >= 0; i--)
	{
		if (*(data++) == chr)			// shoduje se prvn� znak?
		{
			if (MemCompare(data, datastr, lenstr*sizeof(TCHAR)))	// shoduje se zbytek �et�zce?
			{
				return (data - pData->Data - 1);	// n�vrat pozice za��tku �et�zce
			}
		}
	}

	return -1;
}


/////////////////////////////////////////////////////////////////////////////
// hled�n� znaku v �et�zci (<0 = nenalezeno, pos=v�choz� pozice)

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
// hled�n� znaku od konce �et�zce zp�t (<0 = nenalezeno, pos=v�choz� pozice)

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
// vypr�zdn�n� �et�zce (s p��padn�m zru�en�m bufferu)

void CText::Empty()
{ 
	Term();
	Init();
}


/////////////////////////////////////////////////////////////////////////////
// konverze na velk� p�smena
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool CText::UpperCase()
{
	if (!CopyWrite()) return false;
	::CharUpper(pData->Data);
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// konverze na mal� p�smena
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool CText::LowerCase()
{
	if (!CopyWrite()) return false;
	::CharLower(pData->Data);
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// lev� ��st �et�zce (p�i chyb� pam�ti vr�t� pr�zdn� �et�zec)

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
// prav� ��st �et�zce (p�i chyb� pam�ti vr�t� pr�zdn� �et�zec)

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
// st�edn� ��st �et�zce (p�i chyb� pam�ti vr�t� pr�zdn� �et�zec)

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
// zru�en� textu z konce �et�zce
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

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
// zru�en� posledn�ho znaku z konce �et�zce
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

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
// odstran�n� mezer ze za��tku/konce �et�zce
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool CText::TrimLeft()
{
// nalezen� platn�ch dat nebo konce textu
	TCHAR* data = pData->Data;		// ukazatel textu
	while ((*data > 0) && (*data <= _T(' '))) data++;

// test, zda bude n�co vypou�t�no
	int dif = data - pData->Data;
	if (dif > 0)
	{

// p�ivlastn�n� bufferu
		if (!CopyWrite()) return false;

// zru�en� dat
		pData->Length -= dif;
		MemCopy(pData->Data, data, (pData->Length+1)*sizeof(TCHAR));
	}
	return true;
}

bool CText::TrimRight()
{
// nalezen� konce platn�ch dat
	int i = pData->Length;			// d�lka textu
	TCHAR* data = pData->Data + i;	// adresa za koncem textu

	for (; i > 0; i--)				// pro v�echny znaky textu
	{
		data--;						// posun adresy
#ifdef _UNICODE
		if ((WORD)*data > (WORD)' ') break;	// pro platn� znak konec
#else
		if ((BYTE)*data > (BYTE)' ') break;	// pro platn� znak konec
#endif
	}

// test, zda se d�lka textu m�n�
	if (i != pData->Length)
	{

// p�ivlastn�n� bufferu
		if (!CopyWrite()) return false;

// nov� d�lka dat
		pData->Length = i;				// nov� d�lka textu
		pData->Data[i] = 0;				// ozna�en� konce textu
	}
	return true;
}


////////////////////////////////////////////////////////////////////
// nastaven� ��sla jm�na objektu (1, 2, ...)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

const CText DefObjName(_T('#'));	// implicitn� jm�no

bool _fastcall CText::SetNumObj(const int num)
{
// pracovn� kopie textu (aby nebyl obsah zm�n�n p�i chyb�)
	CText text(*this);

// vypu�t�n� ��slic a mezer z konce jm�na
	for (;;)
	{
		TCHAR znak = text.LastChar();	// na�ten� posledn�ho znaku
		if ((znak == ' ') ||			// je mezera
			(znak == 9) ||				// tabul�tor
			((znak >= '0') &&			// nebo ��slice
			(znak <= '9')))
		{
			if (!text.DeleteLast()) return false;	// vypu�t�n� posledn�ho znaku
		}
		else
		{
			break;
		}
	}

// implicitn� jm�no, je-li pr�zdn� text
	if (text.IsEmpty())
	{
		text = DefObjName;
	}

// ��slo se nastavuje jen pro 2 a v�ce
	if (num > 1)
	{
		if (!text.Add(_T(' ')) ||
			!text.AddInt(num)) return false;
	}

// v�e OK - ulo�en� textu
	*this = text;
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// p�id�n� textu
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

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
// p�id�n� textu ��sla
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

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
// sou�et text�
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool _fastcall CText::Add(const CText& str1, const CText& str2)
{ 
// d�lka text�
	int len1 = str1.pData->Length;
	int len2 = str2.pData->Length;

// vytvo�en� bufferu
	TEXTDATA* data = NewTextData(len1+len2);
	if (data == NULL) return false;

// na�ten� text�
	MemCopy(data->Data, str1.pData->Data, len1*sizeof(TCHAR));
	MemCopy(data->Data + len1, str2.pData->Data, len2*sizeof(TCHAR));

// odpojen� star�ch dat
	detach(pData);

// p�ipojen� nov�ch dat
	pData = data;

	return true;
}

bool _fastcall CText::Add(LPCTSTR txt, const CText& str) 
{ 
// d�lka text�
	int len1 = 0;
	if (txt != NULL) len1 = ::lstrlen(txt);
	int len2 = str.pData->Length;

// vytvo�en� bufferu
	TEXTDATA* data = NewTextData(len1+len2);
	if (data == NULL) return false;

// na�ten� text�
	MemCopy(data->Data, txt, len1*sizeof(TCHAR));
	MemCopy(data->Data + len1, str.pData->Data, len2*sizeof(TCHAR));

// odpojen� star�ch dat
	detach(pData);

// p�ipojen� nov�ch dat
	pData = data;

	return true;
}

bool _fastcall CText::Add(const CText& str, LPCTSTR txt) 
{ 
// d�lka text�
	int len1 = str.pData->Length;
	int len2 = 0;
	if (txt != NULL) len2 = ::lstrlen(txt);

// vytvo�en� bufferu
	TEXTDATA* data = NewTextData(len1+len2);
	if (data == NULL) return false;

// na�ten� text�
	MemCopy(data->Data, str.pData->Data, len1*sizeof(TCHAR));
	MemCopy(data->Data + len1, txt, len2*sizeof(TCHAR));

// odpojen� star�ch dat
	detach(pData);

// p�ipojen� nov�ch dat
	pData = data;

	return true;
}

bool _fastcall CText::Add(const TCHAR chr, const CText& str) 
{ 
// d�lka textu
	int len = str.pData->Length;

// vytvo�en� bufferu
	TEXTDATA* data = NewTextData(1+len);
	if (data == NULL) return false;

// na�ten� text�
	data->Data[0] = chr;
	MemCopy(data->Data + 1, str.pData->Data, len*sizeof(TCHAR));

// odpojen� star�ch dat
	detach(pData);

// p�ipojen� nov�ch dat
	pData = data;

	return true;
}

bool _fastcall CText::Add(const CText& str, const TCHAR chr) 
{ 
// d�lka textu
	int len = str.pData->Length;

// vytvo�en� bufferu
	TEXTDATA* data = NewTextData(len+1);
	if (data == NULL) return false;

// na�ten� text�
	MemCopy(data->Data, str.pData->Data, len*sizeof(TCHAR));
	data->Data[len] = chr;

// odpojen� star�ch dat
	detach(pData);

// p�ipojen� nov�ch dat
	pData = data;

	return true;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� textu
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

void _fastcall CText::Set(const CText& str)
{
	detach(pData);			// zru�en� star�ch dat
	attach(str.pData);		// p�ipojen� nov�ch dat
}

bool _fastcall CText::Set(const TCHAR chr)
{
// vytvo�en� bufferu dat
	TEXTDATA* data = NewTextData(1);
	if (data == NULL) return false;

// napln�n� bufferu
	data->Data[0] = chr;

// odpojen� star�ch dat
	detach(pData);

// p�ipojen� nov�ch dat
	pData = data;

	return true;
}

bool _fastcall CText::Set(LPCTSTR txt)
{
// d�lka textu
	int len = 0;
	if (txt != NULL) len = ::lstrlen(txt);

// vytvo�en� bufferu pro data
	TEXTDATA* data = NewTextData(len);	
	if (data == NULL) return false;

// kopie textu
	MemCopy(data->Data, txt, len*sizeof(TCHAR));

// odpojen� star�ch dat
	detach(pData);

// p�ipojen� nov�ch dat
	pData = data;

	return true;
}

bool _fastcall CText::Set(LPCTSTR txt, const int length)
{
// d�lka textu
	int len = 0;
	if (txt != NULL)
	{
		len = length;
		if (len < 0) len = ::lstrlen(txt);
	}

// vytvo�en� bufferu pro data
	TEXTDATA* data = NewTextData(len);	
	if (data == NULL) return false;

// kopie textu
	MemCopy(data->Data, txt, len*sizeof(TCHAR));

// odpojen� star�ch dat
	detach(pData);

// p�ipojen� nov�ch dat
	pData = data;

	return true;
}

#ifdef _UNICODE

bool _fastcall CText::Set(LPCSTR txt)
{
// d�lka textu
	int len = 0;
	if (txt != NULL) len = ::lstrlenA(txt);

// vytvo�en� bufferu pro data
	TEXTDATA* data = NewTextData(len);	
	if (data == NULL) return false;

// kopie textu
	::MultiByteToWideChar(CP_ACP, 0, txt, len, data->Data, len);

// odpojen� star�ch dat
	detach(pData);

// p�ipojen� nov�ch dat
	pData = data;

	return true;
}

bool _fastcall CText::Set(LPCSTR txt, const int length)
{
// d�lka textu
	int len = 0;
	if (txt != NULL)
	{
		len = length;
		if (len < 0) len = ::lstrlenA(txt);
	}

// vytvo�en� bufferu pro data
	TEXTDATA* data = NewTextData(len);	
	if (data == NULL) return false;

// kopie textu
	::MultiByteToWideChar(CP_ACP, 0, txt, len, data->Data, len);

// odpojen� star�ch dat
	detach(pData);

// p�ipojen� nov�ch dat
	pData = data;

	return true;
}

#else //_UNICODE

bool _fastcall CText::Set(LPCWSTR txt)
{
// d�lka textu
	int len = 0;
	if (txt != NULL) len = ::lstrlenW(txt);

// vytvo�en� bufferu pro data
	TEXTDATA* data = NewTextData(len);	
	if (data == NULL) return false;

// kopie textu
	::WideCharToMultiByte(CP_ACP, 0, txt, len, data->Data, len, NULL, NULL);

// odpojen� star�ch dat
	detach(pData);

// p�ipojen� nov�ch dat
	pData = data;

	return true;
}

bool _fastcall CText::Set(LPCWSTR txt, const int length)
{
// d�lka textu
	int len = 0;
	if (txt != NULL)
	{
		len = length;
		if (len < 0) len = ::lstrlenW(txt);
	}

// vytvo�en� bufferu pro data
	TEXTDATA* data = NewTextData(len);	
	if (data == NULL) return false;

// kopie textu
	::WideCharToMultiByte(CP_ACP, 0, txt, len, data->Data, len, NULL, NULL);

// odpojen� star�ch dat
	detach(pData);

// p�ipojen� nov�ch dat
	pData = data;

	return true;
}

#endif //_UNICODE


/////////////////////////////////////////////////////////////////////////////
// na�ten� textu z resource (p�i chyb� vrac� FALSE, p�vodn� obsah nezm�n�n)

bool CText::Load(const int nID)
{
// na�ten� textu do bufferu
	TCHAR buf[0x201];
	int len = ::LoadString(hInstance, nID, buf, 0x200);
	if (len <= 0) return false;

// nastaven� textu
	if (len > 0x200) len = 0x200;
	if (!Set(buf, len)) return false;

// korekce v�skytu nuly v textu
	KorigNul();

// p��znak - na�teno OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// korekce v�skytu nuly v textu

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
// na�ten� ze souboru form�tu TXT (FALSE=chyba, obsah nezm�n�n)
/*
bool CText::LoadFile(CText jmeno)
{
// otev�en� souboru mapovan�ho do pam�ti (uzav�en p�i destrukci!)
	CFileMap file;
	if (!file.Open(jmeno)) return false;

// na�ten� textu
	if (!Set((char*)file.Adr(), file.Size())) return false;

// korekce v�skytu nuly v textu
	KorigNul();

// p��znak - na�teno OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// ulo�en� textu do souboru form�tu TXT (FALSE=chyba)

bool CText::SaveFile(CText jmeno) const
{	
// vytvo�en� souboru
	CFile file;
	file.Name(jmeno);
	if (!file.Create()) return false;

#ifdef _UNICODE
	
// buffer k dek�dov�n� souboru
	char* buf = (char*) MemGet(pData->Length + 100);
	if (buf == NULL)
	{
		file.Close();
		file.Delete();
		return false;
	}		
	Write(buf);
	
// ulo�en� souboru
	BOOL result = file.Write(buf, pData->Length);

// zru�en� bufferu
	MemFree(buf);

#else

// ulo�en� souboru
	BOOL result = file.Write(pData->Data, pData->Length);

#endif

// uzav�en� souboru
	file.Close();

// p�i chyb� zru�en� souboru
	if (!result)
	{
		file.Delete();
		return false;
	}

// p��znak - ulo�eno OK
	return true;
}
*/

/////////////////////////////////////////////////////////////////////////////
// kopie do vlastn�ho bufferu p�ed modifikac�
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool CText::CopyWrite()
{
// �schova ukazatel�
	TEXTDATA* olddata = pData;	// adresa star�ch dat
	long* refer = &(olddata->Refer);// po�et referenc�

// test, zda je nutn� p�ivlastn�n�
	if (*refer > 1)					// je n�jak� jin� majitel?
	{

// vytvo�en� nov�ho bufferu
		TEXTDATA* newdata = NewTextData(olddata->Length);
		if (newdata == NULL) return false;

// p�enesen� dat
		MemCopy(newdata->Data, olddata->Data, olddata->Length*sizeof(TCHAR));

// odpojen� star�ch dat
		detach(olddata);

// p�ipojen� nov�ch dat
		pData = newdata;
	}

// operace OK
	return true;
}


//////////////////////////////////////////////////////////////////////////////
// poskytnut� znaku na pozici s kontrolou (pro neplatn� znak vr�t� 0)

TCHAR _fastcall CText::Get(const int index) const
{ 
	if (IsValid(index))
	{
		return pData->Data[index];
	}
	return 0;
}


//////////////////////////////////////////////////////////////////////////////
// poskytnut� posledn�ho znaku �et�zce (pro pr�zdn� �et�zec vrac� 0)

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
// nastaven� znaku na pozici s kontrolou (zajist� p�ivlastn�n� bufferu, p�i chyb� pam�ti vrac� FALSE)

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
// na�ten� textu okna (p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n)

bool CText::GetWindowText(const HWND wnd)
{
// vytvo�en� nov�ho bufferu
	TEXTDATA* data = NewTextData(::GetWindowTextLength(wnd));
	if (data == NULL) return false;

// na�ten� dat
	::GetWindowText(wnd, data->Data, data->Length+1);
	data->Data[data->Length] = 0;

// odpojen� star�ch dat
	detach(pData);

// p�ipojen� nov�ch dat
	pData = data;

// ochrana proti nule v textu
	KorigNul();

// operace OK
	return true;
}


//////////////////////////////////////////////////////////////////////////////
// nastaven� textu okna

void _fastcall CText::SetWindowText(const HWND wnd) const
{
	::SetWindowText(wnd, pData->Data);
}


//////////////////////////////////////////////////////////////////////////////
// na�ten� textu dialogov�ho prvku (p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n)

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
// nastaven� textu dialogov�ho prvku

void _fastcall CText::SetDialogText(const HWND wnd, int id) const
{
	HWND item = ::GetDlgItem(wnd, id);
	if (item != NULL)
	{
		::SetWindowText(item, pData->Data);
	}
}


//////////////////////////////////////////////////////////////////////////////
// na�ten� aktivn�ho adres��e (p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n)

bool CText::GetAktDir()
{
// na�ten� textu do bufferu
	TCHAR buf[_MAX_PATH+1];
	int len = (int)::GetCurrentDirectory(_MAX_PATH, buf);

// nastaven� textu
	if (len < 0) len = 0;
	if (len > _MAX_PATH) len = _MAX_PATH;
	if (!Set(buf, len)) return false;

// korekce v�skytu nuly v textu
	KorigNul();

// p��znak - na�teno OK
	return true;
}


//////////////////////////////////////////////////////////////////////////////
// korekce textu na jm�no souboru (vypu�t�n� zak�zan�ch znak�, vrac� opravenou pozici kurzoru)
// ignoruje chybu pam�ti p�i p�ivlastn�n� bufferu

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
// korekce textu na jm�no cesty (vypu�t�n� zak�zan�ch znak�, vrac� opravenou pozici kurzoru)
// ignoruje chybu pam�ti p�i p�ivlastn�n� bufferu

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
// konverze ��sla INT na text (p�i chyb� pam�ti vrac� FALSE)

bool CText::Int(int num)
{
// p��prava bufferu
	TCHAR buf[16];
	TCHAR* dst = buf+16;
	int len = 0;

// p��prava znam�nka
	bool sign = false;
	if (num < 0)
	{
		sign = true;
		num = -num;
	}

// konverze ��slic
	do {
		dst--;
		*dst = (TCHAR)(((DWORD)num % 10) + '0');
		*(DWORD*)(&num) /= 10;
		len++;
	} while (num != 0);

// ulo�en� znam�nka
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
// konverze textu na ��slo INT

int Int(LPCTSTR txt)
{
// p��prava prom�nn�ch
	DWORD result = 0;
	bool sign = false;
	TCHAR chr;

// na�ten� znam�nka
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
	
// na�ten� ��slic
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

// n�vrat s korekc� znam�nka
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
// konverze ��sla DWORD na HEX (8 ��slic) (p�i chyb� pam�ti vrac� FALSE)

bool CText::Hex(DWORD num)
{
// p��prava bufferu
	TEXTDATA* data = NewTextData(8);
	if (data == NULL) return false;
	TCHAR* dst = data->Data + 8;

// dek�dov�n� ��slic
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

// odpojen� star�ch dat
	detach(pData);

// p�ipojen� nov�ch dat
	pData = data;

	return true;
}

//////////////////////////////////////////////////////////////////////////////
// konverze ��sla DOUBLE na text (p�i chyb� pam�ti vrac� FALSE)

const CText InfDoubleText(_T("1#INF"), 5);
const CText NulDoubleText(_T('0'));

bool CText::Double(double num)
{
// p�ete�en� "1#INF"
	if (*(ULONGLONG*)(&num) == DOUBLE_INFINITY_VALUE)
	{
		Set(InfDoubleText);
		return true;
	}

// dek�dov�n� nuly
	if (num == 0)
	{
		Set(NulDoubleText);
		return true;
	}

// p��prava znam�nka
	bool sign = false;
	if (num < 0)
	{
		sign = true;
		num = -num;
	}

// rozd�len� ��sla na exponent a mantisu
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

// dek�dov�n� mantisy
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
	
// zji�t�n� po�tu platn�ch ��slic
	TCHAR* dst = mantT+15;
	int digits = 15;
	for (; digits > 1; digits--)
	{
		dst--;
		if (*dst != '0') break;
	}

// p��prava znam�nka do v�choz�ho bufferu
	TCHAR buf[30];
	dst = buf;
	if (sign)
	{
		*dst = '-';
		dst++;
	}

// dek�dov�n� ��sla bez exponentu, ��slo >= 1
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

// dek�dov�n� ��sla bez exponentu, ��slo < 1
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

// dek�dov�n� ��sla s exponentem
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

// ulo�en� ��sla do bufferu
	return Set(buf, dst-buf);
}


//////////////////////////////////////////////////////////////////////////////
// konverze textu na ��slo DOUBLE

double Double(LPCTSTR txt)
{
// p��prava prom�nn�ch
	double result = 0;			// v�sledek
	int expN = 0;				// exponent
	double zlomek = 1;			// zlomek desetinn� ��sti
	bool sign = false;			// p��znak z�porn�ho znam�nka
	bool signE = false;			// znam�nko exponentu
	TCHAR chr = *txt;			// na�ten� znak

// na�ten� znam�nka
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
	
// na�ten� ��slic cel� ��sti
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

// na�ten� ��slic desetinn� ��sti
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

// vypu�t�n� mezer
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

// vypu�t�n� mezer a ur�en� znam�nka
		while ((chr == ' ') || (chr == 9) || (chr == '+') || (chr == '-'))
		{
			if (chr == '-')
			{
				signE = !signE;
			}
			txt++;
			chr = *txt;
		}

// na�ten� ��slic exponentu
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

// vyn�soben� ��sla exponentem
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

// p��znak p�ete�en�
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

// n�vrat s korekc� znam�nka
	if (sign)
	{
		return -result;
	}
	return result;
}	


/////////////////////////////////////////////////////////////////////////////
// oper�tor porovn�n�

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


