
#include "Main.h"

/***************************************************************************\
*																			*
*								Textov� �et�zce								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// inicializa�n� pr�zdn� �et�zec (modifikuje se po�et referenc�!)

STRINGDATA	EmptyStringData		= { 1, 0, -1, 0 };


/////////////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CString::CString() 
{ 
	Attach(&EmptyStringData); 
};

CString::CString(const CString& src) 
{ 
	Attach(src.pData); 
};

CString::CString(const TCHAR chr)
{
	if (chr == 0)
	{
		Empty();
	}
	else
	{
		NewBuffer(1);
		pData->Data[0] = chr;
	}
}

CString::CString(LPCTSTR text, const int length)
{
	int len = 0;
	if (text)
	{
		len = length;
		if (len < 0)
		{
			len = lstrlen(text);
		}
	}
	NewBuffer(len);	
	MemCopy(pData->Data, text, len*sizeof(TCHAR));
}

#ifdef _UNICODE

CString::CString(LPCSTR text, const int length)
{
	int len = 0;
	if (text)
	{
		len = length;
		if (len < 0)
		{
			len = lstrlenA(text);
		}
	}
	NewBuffer(len);
	::MultiByteToWideChar(CodePage, 0, text, len, pData->Data, len+1);
}

#else //_UNICODE

CString::CString(LPCWSTR text, const int length)
{
	int len = 0;
	if (text)
	{
		len = length;
		if (len < 0)
		{
			len = lstrlenW(text);
		}
	}
	NewBuffer(len);
	::WideCharToMultiByte(CodePage, 0, text, len, pData->Data, len+1, NULL, NULL);
}

#endif //_UNICODE

CString::~CString() 
{ 
	Detach(); 
};


/////////////////////////////////////////////////////////////////////////////
// statick� konstruktor a destruktor

void CString::Init()
{ 
	Attach(&EmptyStringData); 
};

void CString::Init(STRINGDATA* data)
{ 
	Attach(data); 
};

void CString::Init(const CString& src) 
{ 
	Attach(src.pData); 
};

void CString::Init(LPCTSTR text, const int length)
{
	int len = 0;
	if (text)
	{
		len = length;
		if (len < 0)
		{
			len = lstrlen(text);
		}
	}
	NewBuffer(len);	
	MemCopy(pData->Data, text, len*sizeof(TCHAR));
}

#ifdef _UNICODE

void CString::Init(LPCSTR text, const int length)
{
	int len = 0;
	if (text)
	{
		len = length;
		if (len < 0)
		{
			len = lstrlenA(text);
		}
	}
	NewBuffer(len);
	::MultiByteToWideChar(CodePage, 0, text, len, pData->Data, len+1);
}

#else //_UNICODE

void CString::Init(LPCWSTR text, const int length)
{
	int len = 0;
	if (text)
	{
		len = length;
		if (len < 0)
		{
			len = lstrlenW(text);
		}
	}
	NewBuffer(len);
	::WideCharToMultiByte(CodePage, 0, text, len, pData->Data, len+1, NULL, NULL);
}

#endif //_UNICODE

void CString::Term()
{ 
	Detach(); 
};


/////////////////////////////////////////////////////////////////////////////
// z�pis do bufferu (bez koncov� nuly!)

void CString::Write(TCHAR* buf) const
{
	MemCopy(buf, pData->Data, pData->Length*sizeof(TCHAR));
}

#ifdef _UNICODE

void CString::Write(LPSTR buf) const
{
	::WideCharToMultiByte(CodePage, 0, pData->Data, pData->Length, buf, pData->Length+1, NULL, NULL);
}

#else // _UNICODE

void CString::Write(LPWSTR buf) const
{
	::MultiByteToWideChar(CodePage, 0, pData->Data, pData->Length, buf, pData->Length+1);
}

#endif // _UNICODE


/////////////////////////////////////////////////////////////////////////////
// nastaven� d�lky textu (p�i prodlou�en� textu obsah nedefinovan�)

void CString::Length(int len)
{
	CopyWrite();
	SizeBuffer(len);
}


/////////////////////////////////////////////////////////////////////////////
// hled�n� textu v �et�zci (-1 = nenalezeno)

int _fastcall CString::Find(const CString& str) const
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


/////////////////////////////////////////////////////////////////////////////
// hled�n� textu v �et�zci (-1 = nenalezeno)

int _fastcall CString::Find(LPCTSTR txt) const
{
// hledan� �et�zec pr�zdn� - bude pozice 0
	int lenstr = 0;
	if (txt)
	{
		lenstr = lstrlen(txt);
	}
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


/////////////////////////////////////////////////////////////////////////////
// hled�n� znaku v �et�zci (-1 = nenalezeno)

int _fastcall CString::Find(const TCHAR chr) const
{
	TCHAR* data = pData->Data;
	while ((*data) && (*data != chr))
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
// hled�n� znaku od konce �et�zce zp�t (-1 = nenalezeno)

int _fastcall CString::RevFind(const TCHAR chr) const
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


/////////////////////////////////////////////////////////////////////////////
// po�et ��dk� textu

int CString::LineNum() const
{
// p��prava ukazatel�
	TCHAR* data = pData->Data;		// za��tek dat textu
	int radky = 0;						// ��ta� ��dk� textu

// spo�ten� ��dk�
	TCHAR chr;
	do
	{
		chr = *data;
		data++;

		if ((chr == 13) && (*data == 10))
		{
			radky++;
		}
	} while (chr);

// korekce po�tu ��dk�, nen�-li posledn� ��dek ukon�en
	int len = pData->Length;
	if (((len >= 2) && ((pData->Data[len-2] != 13) || (pData->Data[len-1] != 10))) || (len == 1))
	{
		radky++;
	}

// po�et zji�t�n�ch ��dk�
	return radky;
}

/////////////////////////////////////////////////////////////////////////////
// nalezen� ��dku

CString _fastcall CString::GetLine(int radek) const
{
	int delka = pData->Length;			// d�lka textu celkem

	int zacatek = 0;						// index za��tku ��dku
	int dalsi = 0;							// index konce ��dku

	LPTSTR data = pData->Data;			// ukazatel dat
	for (; dalsi < delka; dalsi++)
	{
		if ((*data == 13) && (dalsi < (delka-1)) && (data[1] == 10))
		{
			radek--;
			if (radek < 0) break;
			dalsi++;
			data++;
			zacatek = dalsi + 1;
		}
		data++;
	}
	if (radek > 0) zacatek = dalsi;

	return Mid(zacatek, dalsi - zacatek);	// navr�cen� ��dku
}

/////////////////////////////////////////////////////////////////////////////
// blok textu (-1=konec ��dku nebo textu)

CString CString::GetBlock(int begline, int begpos, int endline, int endpos) const
{
#define MAXNUM	0x7fffffff

// korekce koncov�ho ��dku a pozice
	if (begline < 0) begline = MAXNUM;
	if (begpos < 0) begpos = MAXNUM;
	if (endline < 0) endline = MAXNUM;
	if (endpos < 0) endpos = MAXNUM;

	BOOL endmax = ((endline == MAXNUM) && (endpos == MAXNUM));

// korekce po�ad� po��tku a konce
	if ((endline < begline) || ((endline == begline) && (endpos < begpos)))
	{
		int t = begline;
		begline = endline;
		endline = t;

		t = begpos;
		begpos = endpos;
		endpos = t;
	}

// je pr�zdn� text
	endline -= begline;
	if (endline == 0) endpos -= begpos;

	if ((endline == 0) && (endpos == 0)) return EmptyString;

// p��prava ukazatel� dat
	int delka = pData->Length;			// d�lka textu celkem
	LPTSTR data = pData->Data;			// ukazatel dat

// nalezen� ��dku po��tku bloku
	int beg = 0;

	if (begline > 0)
	{
		for (; beg < delka;)
		{
			if ((*data == 13) && (beg < (delka-1)) && (data[1] == 10))
			{
				begline--;
				if (begline == 0)
				{
					beg++;
					beg++;
					data++;
					data++;
					break;
				}
			}

			beg++;
			data++;
		}
	}

// nalezen� pozice po��tku bloku
	for (; beg < delka;)
	{
		if ((begpos == 0) || ((*data == 13) && (beg < (delka-1)) && (data[1] == 10)))
		{
			break;
		}

		begpos--;
		beg++;
		data++;
	}

// konec bloku neomezen
	int end = delka;

	if (!endmax)
	{
		end = beg;

// nalezen� ��dku konce bloku
		if (endline > 0)
		{
			for (; end < delka;)
			{
				if ((*data == 13) && (end < (delka-1)) && (data[1] == 10))
				{
					endline--;
					if (endline == 0)
					{
						end++;
						end++;
						data++;
						data++;
						break;
					}
				}

				end++;
				data++;
			}
		}

// nalezen� pozice konce bloku
		for (; end < delka;)
		{
			if ((endpos == 0) || ((*data == 13) && (end < (delka-1)) && (data[1] == 10)))
			{
				break;
			}

			endpos--;
			end++;
			data++;
		}
	}

// navr�cen� bloku textu
	return Mid(beg, end - beg);
}


/////////////////////////////////////////////////////////////////////////////
// vypr�zdn�n� �et�zce (uvoln�n� dat)

void CString::Empty()
{ 
	Detach(); 
	Attach(&EmptyStringData); 
}


/////////////////////////////////////////////////////////////////////////////
// konverze na velk� p�smena

void CString::UpperCase()
{
	CopyWrite();
	::CharUpper(pData->Data);
}


/////////////////////////////////////////////////////////////////////////////
// konverze na mal� p�smena

void CString::LowerCase()
{
	CopyWrite();
	::CharLower(pData->Data);
}


/////////////////////////////////////////////////////////////////////////////
// lev� ��st �et�zce

CString _fastcall CString::Left(int count) const
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
	return CString(pData->Data, count);
}


/////////////////////////////////////////////////////////////////////////////
// prav� ��st �et�zce

CString _fastcall CString::Right(int count) const
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
	return CString(pData->Data + len - count, count);
}


/////////////////////////////////////////////////////////////////////////////
// st�edn� ��st �et�zce

CString _fastcall CString::Mid(int first, int count) const
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
	
	return CString(pData->Data + first, count);
}


/////////////////////////////////////////////////////////////////////////////
// zru�en� textu z konce �et�zce

void CString::Delete(int first, int count)
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

	if (count)
	{
		CopyWrite();
		int firstcount = first + count;
		MemCopy(pData->Data + first, pData->Data + firstcount, len - firstcount + 1);
		pData->Length -= count;
	}
}


/////////////////////////////////////////////////////////////////////////////
// odstran�n� mezer ze za��tku/konce �et�zce

void CString::TrimLeft()
{
	CopyWrite();					// p�ivlastn�n� bufferu

	TCHAR* data = pData->Data;		// ukazatel textu

#ifdef _UNICODE
	while ((*data) && ((WORD)*data <= (WORD)' '))
#else
	while ((*data) && ((BYTE)*data <= (BYTE)' '))
#endif
	{
		data++;
	}

	int dif = data - pData->Data;

	if (dif)
	{
		pData->Length -= dif;
		MemCopy(pData->Data, data, (pData->Length+1)*sizeof(TCHAR));
	}
}

void CString::TrimRight()
{
	CopyWrite();					// p�ivlastn�n� bufferu

	int i = pData->Length;			// d�lka textu
	TCHAR* data = pData->Data + i;	// adresa za koncem textu

	for (; i != 0; i--)				// pro v�echny znaky textu
	{
		data--;						// posun adresy
#ifdef _UNICODE
		if ((WORD)*data > (WORD)' ') break;	// pro platn� znak konec
#else
		if ((BYTE)*data > (BYTE)' ') break;	// pro platn� znak konec
#endif
	}

	pData->Length = i;				// nov� d�lka textu
	pData->Data[i] = 0;				// ozna�en� konce textu
}


/////////////////////////////////////////////////////////////////////////////
// korekce v�skytu nuly v textu

void CString::KorigNul()
{
	int len = ::lstrlen(pData->Data);
	if ((len < pData->Length) && (len >= 0))
	{
		pData->Length = len;
		pData->Data[len] = 0;
	}
}

////////////////////////////////////////////////////////////////////
// nastaven� ��sla jm�na objektu (1, 2, ...)

const CString DefObjName(_T('#'));	// implicitn� jm�no

void _fastcall CString::SetNumObj(const int num)
{
// vypu�t�n� ��slic a mezer z konce jm�na
	for (;;)
	{
		TCHAR znak = this->LastChar();	// na�ten� posledn�ho znaku
		if ((znak == ' ') ||			// je mezera
			(znak == 9) ||				// tabul�tor
			((znak >= '0') &&			// nebo ��slice
			(znak <= '9')))
		{
			this->Delete(this->Length() - 1, 1); // vypu�t�n� posledn�ho znaku
		}
		else
		{
			break;
		}
	}

// implicitn� jm�no, je-li pr�zdn� text
	if (this->IsEmpty())
	{
		*this = DefObjName;
	}

// ��slo se nastavuje jen pro 2 a v�ce
	if (num > 1)
	{
		*this += _T(' ');
		CString txt;
		txt.Int(num);
		*this += txt;
	}
}


//////////////////////////////////////////////////////////////////////////////
// poskytnut� posledn�ho znaku �et�zce (pro pr�zdn� �et�zec vrac� 0)

TCHAR _fastcall CString::LastChar() const
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

/////////////////////////////////////////////////////////////////////////////
// na�ten� textu z resource

void CString::Load(const int nID)
{
	TCHAR buf[0x201];
	buf[0] = 0;
	int len = ::LoadString(hInstance, nID, buf, 0x200);
	Detach();
	NewBuffer(len);	
	MemCopy(pData->Data, buf, len*sizeof(TCHAR));
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� ze souboru form�tu TXT (false=chyba)
/*
bool CString::LoadFile(CString jmeno)
{
// otev�en� souboru
	HANDLE hFile = ::CreateFile(jmeno, GENERIC_READ, 
		FILE_SHARE_READ, NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, NULL);

// test, zda byl soubor nalezen
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

// zji�t�n� velikosti souboru
	int size = (int)::SetFilePointer(hFile, 0, NULL, FILE_END);
	::SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	if (size < 0) size = 0;

// �schova p�vodn�ho obsahu pro p��pad chyby
	CString txt(*this);

// vytvo�en� nov�ho bufferu
	Detach();
	NewBuffer(size/sizeof(TCHAR));

#ifdef _UNICODE

// buffer k na�ten� souboru
	char* buf = (char*)MemGet(size);

// na�ten� souboru
	DWORD read;
	::ReadFile(hFile, buf, size, &read, NULL);

// na�ten� textu
	::MultiByteToWideChar(CodePage, 0, buf, pData->Length, pData->Data, pData->Length+1);

// zru�en� bufferu
	MemFree(buf);

#else

// na�ten� souboru
	DWORD read;
	::ReadFile(hFile, pData->Data, size, &read, NULL);

#endif

// uzav�en� souboru
	::CloseHandle(hFile);		// uzav�en� souboru

// kontrola spr�vnosti operace
	if (read != (DWORD)size)
	{
		Detach();
		Attach(txt.Data());
		return false;
	}

// ochrana proti na�ten� nuly
	int len = ::lstrlen(pData->Data);
	if ((len < pData->Length) && (len >= 0))
	{
		pData->Length = len;
		pData->Data[len] = 0;
	}

// p��znak - na�teno OK
	return true;
}
*/

/////////////////////////////////////////////////////////////////////////////
// ulo�en� textu do souboru form�tu TXT (false=chyba)
/*
bool CString::SaveFile(CString jmeno) const
{
// otev�en� souboru
	HANDLE hFile = ::CreateFile(jmeno, GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

// test, zda byl soubor vytvo�en
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

#ifdef _UNICODE
	
// buffer k dek�dov�n� souboru
	char* buf = (char*) MemGet(pData->Length + 100);
	Write(buf);
	
// ulo�en� souboru
	DWORD write;
	BOOL result = ::WriteFile(hFile, buf, pData->Length, &write, NULL);

// zru�en� bufferu
	MemFree(buf);

#else

// ulo�en� souboru
	DWORD write;
	BOOL result = ::WriteFile(hFile, pData->Data, pData->Length, &write, NULL);

#endif

// uzav�en� souboru
	::CloseHandle(hFile);

// p�i chyb� zru�en� souboru
	if (!result || (write != (DWORD)(pData->Length)))
	{
		::DeleteFile(jmeno);
		return false;
	}

// p��znak - ulo�eno OK
	return true;
}
*/
/////////////////////////////////////////////////////////////////////////////
// p��prava jm�na p�echodn�ho souboru

void CString::TempName()
{
 	TCHAR* pth = (TCHAR*)MemGet(256);
	TCHAR* fil = (TCHAR*)MemGet(512);
	pth[0] = 0;
	fil[0] = 0;
	::GetTempPath(255, pth);
	::GetTempFileName(pth, _T("PET"), 0, fil);
	*this = fil;
	MemFree(pth);
	MemFree(fil);
}


/////////////////////////////////////////////////////////////////////////////
// kopie do vlastn�ho bufferu p�ed modifikac�

void CString::CopyWrite()
{
	STRINGDATA* data = pData;		// adresa star�ch dat
	long* refer = &(data->Refer);	// po�et referenc�

	if (*refer > 1)					// je n�jak� jin� majitel?
	{
		NewBuffer(data->Length);	// vytvo�en� nov�ho bufferu
		MemCopy(pData->Data, data->Data, data->Length*sizeof(TCHAR));

// odpojen� star�ch dat - v multithread m��e nastat i zru�en�
		if (LongDecrement(refer))
		{
#ifdef _MT
			MemFree(data);			// p��padn� zru�en� �et�zce
#endif	// _MT
		}
	}

	pData->Orig = -1;
}


//////////////////////////////////////////////////////////////////////////////
// poskytnut� znaku na pozici s kontrolou

TCHAR _fastcall CString::Get(const int index) const
{ 
	if (IsValid(index))
	{
		return pData->Data[index];
	}
	return 0;
}


//////////////////////////////////////////////////////////////////////////////
// nastaven� znaku na pozici s kontrolou (zajist� p�ivlastn�n� bufferu)

void _fastcall CString::Set(const int index, const TCHAR chr)
{
	if (IsValid(index) && (chr != 0))
	{
		CopyWrite();
		pData->Data[index] = chr;
	}
}


//////////////////////////////////////////////////////////////////////////////
// na�ten� textu okna

void CString::GetWindowText(const HWND wnd)
{
	Detach();
	NewBuffer(::GetWindowTextLength(wnd));
	::GetWindowText(wnd, pData->Data, pData->Length+1);
}


//////////////////////////////////////////////////////////////////////////////
// nastaven� textu okna

void _fastcall CString::SetWindowText(const HWND wnd) const
{
	::SetWindowText(wnd, pData->Data);
}


//////////////////////////////////////////////////////////////////////////////
// na�ten� textu dialogov�ho prvku

void CString::GetDialogText(const HWND wnd, int id)
{
	HWND item = ::GetDlgItem(wnd, id);
	if (item)
	{
		Detach();
		NewBuffer(::GetWindowTextLength(item));
		::GetWindowText(item, pData->Data, pData->Length+1);
	}
	else
	{
		Empty();
	}
}


//////////////////////////////////////////////////////////////////////////////
// nastaven� textu dialogov�ho prvku

void _fastcall CString::SetDialogText(const HWND wnd, int id) const
{
	HWND item = ::GetDlgItem(wnd, id);
	if (item)
	{
		::SetWindowText(item, pData->Data);
	}
}


//////////////////////////////////////////////////////////////////////////////
// na�ten� aktivn�ho adres��e

void CString::GetAktDir()
{
	TCHAR buf[_MAX_PATH];
	buf[0] = 0;
	int len = (int)::GetCurrentDirectory(_MAX_PATH, buf);
	if (len < 0) len = 0;
	if (len > _MAX_PATH) len = _MAX_PATH;
	
	Detach();
	NewBuffer(len);
	MemCopy(pData->Data, buf, len*sizeof(TCHAR));
}


//////////////////////////////////////////////////////////////////////////////
// korekce textu na jm�no souboru (vypu�t�n� zak�zan�ch znak�, vrac� opravenou pozici kurzoru)

int CString::FileName(int curs)
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

int CString::PathName(int curs)
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
// konverze ��sla INT na text

void CString::Int(int num)
{
// p��prava bufferu
	TCHAR buf[15];
	TCHAR* dst = buf+15;
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
	Detach();
	NewBuffer(len);
	MemCopy(pData->Data, dst, len*sizeof(TCHAR));
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
// p�id�n� 2 ��slic

void _fastcall CString::Add2Dig(char num)
{
	if ((BYTE)num > (BYTE)99)
	{
		if (num < 0)
		{
			num = 0;
		}
		else
		{
			num = 99;
		}
	}

// �schova indexu
	int len = pData->Length;

// p�ivlastn�n� bufferu pro z�pis
	CopyWrite();			// p�ivlastn�n� bufferu

// zv��en� velikosti bufferu
	SizeBuffer(len+2);		// zm�na velikosti bufferu

// dek�dov�n� prvn� ��slice
	pData->Data[len] = (TCHAR)(num/10 + _T('0'));

// dek�dov�n� druh� ��slice
	pData->Data[len+1] = (TCHAR)(num % 10 + _T('0'));
}


//////////////////////////////////////////////////////////////////////////////
// p�id�n� 4 ��slic

void _fastcall CString::Add4Dig(short num)
{
	if ((WORD)num > (WORD)9999)
	{
		if (num < 0)
		{
			num = 0;
		}
		else
		{
			num = 9999;
		}
	}
	Add2Dig((char)(num/100));
	Add2Dig((char)(num % 100));
}


//////////////////////////////////////////////////////////////////////////////
// p�id�n� 2 ��slic se zarovn�n�m mezerou

void _fastcall CString::Add2DigSpc(char num)
{
// dek�dov�n� ��slic
	Add2Dig(num);

// adresa p�edposledn�ho znaku
	TCHAR* data = pData->Data + pData->Length - 2;

// n�hrada prvn� nuly mezerou
	if (*data == _T('0')) *data = _T(' ');
}


//////////////////////////////////////////////////////////////////////////////
// p�id�n� textu se zn�mou d�lkou

void _fastcall CString::Add(LPCTSTR text, int num)
{
// kontrola d�lky textu
	if (num <= 0) return;

// p�ivlastn�n� bufferu pro z�pis
	CopyWrite();

// �schova star� d�lky textu
	int len = pData->Length;

// zv��en� velikosti bufferu
	SizeBuffer(len + num);

// p�id�n� textu
	MemCopy(pData->Data + len, text, num*sizeof(TCHAR));
}

#ifdef _UNICODE
void _fastcall CString::Add(char* text, int num)
{
	CString text2(text, num);
	this->Add(text2, num);
}
#else
void _fastcall CString::Add(WCHAR* text, int num)
{
	CString text2(text, num);
	this->Add(text2, num);
}
#endif

//////////////////////////////////////////////////////////////////////////////
// konverze ��sla DWORD na HEX (8 ��slic)

void CString::Hex(DWORD num)
{
// p��prava bufferu
	Detach();
	NewBuffer(8);
	TCHAR* dst = pData->Data + 8;

// dek�dov�n� ��slic
	TCHAR chr;
	for (int i = 8; i != 0; i--)
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
}

//////////////////////////////////////////////////////////////////////////////
// konverze ��sla DOUBLE na text

void CString::Double(double num)
{
// p�ete�en� "1#INF"
	if (*(ULONGLONG*)(&num) == DOUBLE_INFINITY_VALUE)
	{
		Detach();
		NewBuffer(5);
		pData->Data[0] = '1';
		pData->Data[1] = '#';
		pData->Data[2] = 'I';
		pData->Data[3] = 'N';
		pData->Data[4] = 'F';
		return;
	}

// dek�dov�n� nuly
	if (num == 0)
	{
		Detach();
		NewBuffer(1);
		pData->Data[0] = '0';
		return;
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
				*dst = Carka;
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
		*dst = Carka;
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
			*dst = Carka;
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
	int len = dst - buf;
	Detach();
	NewBuffer(len);
	MemCopy(pData->Data, buf, len*sizeof(TCHAR));
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

// na�ten� ��sla v HEX k�du
	if (chr == '$')
	{
		txt++;
		chr = *txt;

		while ((chr == ' ') || (chr == 9))
		{
			txt++;
			chr = *txt;
		}

		while (chr != 0)
		{
			if ((chr >= 'a') && (chr <= 'f'))
			{
				chr -= 'a' - 'A';
			}

			if (((chr >= '0') && (chr <= '9')) ||
				((chr >= 'A') && (chr <= 'F')))
			{
				int ch = chr - '0';
				if (ch >= 10) ch -= 7;
				result *= 16;
				result += ch;
			}
			else
			{
				break;
			}
			txt++;
			chr = *txt;
		}

		return result;
	}

// na�ten� ��sla v BIN k�du
	if (chr == '%')
	{
		txt++;
		chr = *txt;

		while ((chr == ' ') || (chr == 9))
		{
			txt++;
			chr = *txt;
		}

		while (chr != 0)
		{
			if ((chr == '0') || (chr == '1'))
			{
				result *= 2;
				result += chr - '0';
			}
			else
			{
				break;
			}
			txt++;
			chr = *txt;
		}

		return result;
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
// oper�tor p�i�azen�

const CString& _fastcall CString::operator= (const CString& str)
{
	Detach();				// zru�en� star�ch dat
	Attach(str.pData);		// p�i�azen� nov�ch dat
	return *this;
}

const CString& _fastcall CString::operator= (LPCTSTR txt)
{
	int len = 0;
	if (txt) len = lstrlen(txt);
	Detach();
	NewBuffer(len);	
	MemCopy(pData->Data, txt, len*sizeof(TCHAR));
	return *this;
}

#ifdef _UNICODE

const CString& _fastcall CString::operator= (LPCSTR txt)
{
	int len = 0;
	if (txt) len = lstrlenA(txt);
	Detach();
	NewBuffer(len);
	::MultiByteToWideChar(CodePage, 0, txt, len, pData->Data, len+1);
	return *this;
}

#else //_UNICODE

const CString& _fastcall CString::operator= (LPCWSTR txt)
{
	int len = 0;
	if (txt) len = lstrlenW(txt);
	Detach();
	NewBuffer(len);
	::WideCharToMultiByte(CodePage, 0, txt, len, pData->Data, len+1, NULL, NULL);
	return *this;
}

#endif //_UNICODE

const CString& _fastcall CString::operator= (const TCHAR chr)
{
	if (chr == 0)
	{
		Empty();
	}
	else
	{
		Detach();
		NewBuffer(1);
		pData->Data[0] = chr;
	}
	return *this;
}


/////////////////////////////////////////////////////////////////////////////
// oper�tor p�i�ten�

const CString& _fastcall CString::operator+= (const CString& str)
{
	int len1 = pData->Length;
	int len2 = str.pData->Length;
	CopyWrite();			// p�ivlastn�n� bufferu
	SizeBuffer(len1+len2);	// zm�na velikosti bufferu
	MemCopy(pData->Data + len1, str.pData->Data, len2*sizeof(TCHAR));
	return *this;
}

const CString& _fastcall CString::operator+= (LPCTSTR txt)
{
	int len1 = pData->Length;
	int len2 = 0;
	if (txt) len2 = lstrlen(txt);
	CopyWrite();			// p�ivlastn�n� bufferu
	SizeBuffer(len1+len2);	// zm�na velikosti bufferu
	MemCopy(pData->Data + len1, txt, len2*sizeof(TCHAR));
	return *this;
}

const CString& _fastcall CString::operator+= (const TCHAR chr)
{
	int len = pData->Length;
	CopyWrite();			// p�ivlastn�n� bufferu
	SizeBuffer(len+1);		// zm�na velikosti bufferu
	pData->Data[len] = chr;
	return *this;
}


//////////////////////////////////////////////////////////////////////////////
// oper�tory sou�tu

CString _fastcall operator+ (const CString& str1, const CString& str2)
{
	return CString(str1, str2);
}

CString _fastcall operator+ (LPCTSTR txt, const CString& str)
{
	return CString(txt, str);
}

CString _fastcall operator+ (const CString& str, LPCTSTR txt)
{
	return CString(str, txt);
}

CString _fastcall operator+ (const TCHAR chr, const CString& str)
{
	return CString(chr, str);
}

CString _fastcall operator+ (const CString& str, const TCHAR chr)
{
	return CString(str, chr);
}


/////////////////////////////////////////////////////////////////////////////
// oper�tor porovn�n�

bool _fastcall operator==(const CString& str1, const CString& str2)
{ 
	int delka = str1.Length();
	return ((delka == str2.Length()) &&
		MemCompare(str1.DataData(), str2.DataData(), delka*sizeof(TCHAR)));
};

bool _fastcall operator==(LPCTSTR txt, const CString& str)
{ 
	int delka = 0;
	if (txt) delka = lstrlen(txt);
	return ((delka == str.Length()) &&
		MemCompare(str.DataData(), txt, delka*sizeof(TCHAR)));
};

bool _fastcall operator==(const CString& str, LPCTSTR txt)
{ 
	int delka = 0;
	if (txt) delka = lstrlen(txt);
	return ((delka == str.Length()) &&
		MemCompare(str.DataData(), txt, delka*sizeof(TCHAR)));
};

bool _fastcall operator==(const CString& str, const TCHAR chr)
{ 
	return ((str.Length() == 1) && (str[0] == chr));
};

bool _fastcall operator==(const TCHAR chr, const CString& str)
{ 
	return ((str.Length() == 1) && (str[0] == chr));
};

bool _fastcall operator!=(const CString& str1, const CString& str2)
{ 
	int delka = str1.Length();
	return ((delka != str2.Length()) || !MemCompare(str1.DataData(),
			str2.DataData(), delka*sizeof(TCHAR)));
};

bool _fastcall operator!=(LPCTSTR txt, const CString& str)
{ 
	int delka = 0;
	if (txt) delka = lstrlen(txt);
	return ((delka != str.Length()) || !MemCompare(str.DataData(),
			txt, delka*sizeof(TCHAR)));
};

bool _fastcall operator!=(const CString& str, LPCTSTR txt)
{ 
	int delka = 0;
	if (txt) delka = lstrlen(txt);
	return ((delka != str.Length()) || !MemCompare(str.DataData(),
			txt, delka*sizeof(TCHAR)));
};

bool _fastcall operator!=(const CString& str, const TCHAR chr)
{ 
	return ((str.Length() != 1) || (str[0] != chr));
};

bool _fastcall operator!=(const TCHAR chr, const CString& str)
{ 
	return ((str.Length() != 1) || (str[0] != chr));
};

bool _fastcall operator< (const CString& str1, const CString& str2)
{
	return (::CompareString(
				LOCALE_USER_DEFAULT, 
				0, 
				str1.DataData(),
				str1.Length(),
				str2.DataData(),
				str2.Length()) < 2);

//	return (::lstrcmpi(str1.DataData(), str2.DataData()) < 0);
}

bool _fastcall operator<=(const CString& str1, const CString& str2)
{
	return (::CompareString(
				LOCALE_USER_DEFAULT, 
				0, 
				str1.DataData(),
				str1.Length(),
				str2.DataData(),
				str2.Length()) <= 2);

//	return (::lstrcmpi(str1.DataData(), str2.DataData()) <= 0);
}

bool _fastcall operator> (const CString& str1, const CString& str2)
{
	return (::CompareString(
				LOCALE_USER_DEFAULT, 
				0, 
				str1.DataData(),
				str1.Length(),
				str2.DataData(),
				str2.Length()) > 2);

//	return (::lstrcmpi(str1.DataData(), str2.DataData()) > 0);
}

bool _fastcall operator>=(const CString& str1, const CString& str2)
{
	return (::CompareString(
				LOCALE_USER_DEFAULT, 
				0, 
				str1.DataData(),
				str1.Length(),
				str2.DataData(),
				str2.Length()) >= 2);

//	return (::lstrcmpi(str1.DataData(), str2.DataData()) >= 0);
}


/***************************************************************************\
*																			*
*								Buffer text�								*
*																			*
\***************************************************************************/

////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CBufText::CBufText()
{
	m_Data = NULL;			// nen� buffer dat
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
}

CBufText::~CBufText()
{
	DelAll();				// zru�en� v�ech polo�ek
}


////////////////////////////////////////////////////////////////////
// statick� konstruktor a destruktor

void CBufText::Init()
{
	m_Data = NULL;			// nen� buffer dat
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
}

void CBufText::Term()
{
	DelAll();				// zru�en� v�ech polo�ek
}


////////////////////////////////////////////////////////////////////
// vytvo�en� nov�ch dat

void CBufText::NewData()
{
	m_Max *= 2;
	if (m_Max == 0) m_Max = 0x400;
//	m_Max += 0x400;			// zv��en� po�tu polo�ek (o 4 KB)
	MemBuf(m_Data, m_Max);	// zv��en� velikosti bufferu
};


////////////////////////////////////////////////////////////////////
// zru�en� v�ech polo�ek v bufferu

void CBufText::DelAll()
{
	Del(m_Num);					// zru�en� polo�ek
	MemBuf(m_Data, 0);			// zru�en� bufferu dat
	m_Max = 0;					// nen� ��dn� polo�ka v bufferu
}

////////////////////////////////////////////////////////////////////
// poskytnut� polo�ky (s kontrolou platnosti indexu)

const CString& _fastcall CBufText::Get(const int index) const
{
	if (IsValid(index))			// je index platn�?
	{
		return m_Data[index];	// polo�ka na dan�m indexu
	}
	return EmptyString;		// pro neplatn� index vr�t� pr�zdn� �et�zec
}

////////////////////////////////////////////////////////////////////
// nastaven� polo�ky (s kontrolou platnosti indexu)

void _fastcall CBufText::Set(const int index, const CString& data)
{
	if (IsValid(index))			// je index platn�?
	{
		m_Data[index] = data;
	}
}


/////////////////////////////////////////////////////////////////////////////
// vypr�zdn�n� polo�ky (bez jej�ho zru�en� - jen pro uvoln�n� dat)

void _fastcall CBufText::Empty(const int index)
{
	if (IsValid(index))					// je index platn�?
	{
		m_Data[index].Empty();
	}
}


////////////////////////////////////////////////////////////////////
// zru�en� polo�ek z konce bufferu

void _fastcall CBufText::Del(int num)
{
	if (num > m_Num) num = m_Num;

	CString* str = m_Data + m_Num - 1;
	m_Num -= num;

	for (; num > 0; num--)
	{
		str->Term();
		str--;
	}
}


////////////////////////////////////////////////////////////////////
// vytvo�en� polo�ky (vrac� index polo�ky)

int CBufText::New()
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result].Init();		// inicializace polo�ky
	return result;
}


////////////////////////////////////////////////////////////////////
// p�id�n� polo�ky (vrac� index polo�ky)

int _fastcall CBufText::Add(const CString& data)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result].Init(data.Data());	// inicializace polo�ky
	return result;
}


int _fastcall CBufText::Add(const WCHAR* data, int len)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result].Init(data, len);	// inicializace polo�ky
	return result;
}


int _fastcall CBufText::Add(const char* data, int len)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result].Init(data, len);	// inicializace polo�ky
	return result;
}


////////////////////////////////////////////////////////////////////
// duplikace polo�ky (s kontrolou platnosti indexu, vrac� index prvn� polo�ky)

int _fastcall CBufText::Dup(const int index)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky

	if (IsValid(index))			// je index platn�?
	{
		m_Data[result].Init(m_Data[index].Data());	// kopie polo�ky
	}
	else
	{
		m_Data[result].Init();		// inicializace neplatn� polo�ky
	}
	return result;
}

int _fastcall CBufText::Dup(const int index, int num)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky

	if (IsValid(index))					// je index platn�?
	{
		STRINGDATA* data = m_Data[index].Data();

		m_Data[result].Init(data);		// kopie polo�ky

		for (num--; num > 0; num--)
		{
			m_Data[NewItem()].Init(data);
		}
	}
	else
	{
		m_Data[result].Init();		// inicializace neplatn� polo�ky

		for (num--; num > 0; num--)
		{
			m_Data[NewItem()].Init();
		}
	}
	return result;
}


////////////////////////////////////////////////////////////////////
// na�ten� textu z resource (vrac� index polo�ky)

int _fastcall CBufText::Load(const int nID)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result].Init();		// inicializace polo�ky
	m_Data[result].Load(nID);	// na�ten� textu z resource
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// oper�tor p�i�azen�

const CBufText& CBufText::operator= (const CBufText& src)
{
	Del(m_Num);					// zru�en� star�ch dat

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
