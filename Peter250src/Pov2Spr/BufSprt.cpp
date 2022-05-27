
#include "Main.h"

/***************************************************************************\
*																			*
*									Sprajt									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inicializa�n� pr�zdn� sprajt (modifikuje se po�et referenc�!)

SPRITEDATA*	EmptySpriteData = NULL;

/////////////////////////////////////////////////////////////////////////////
// statick� inicializace sprajt�

void InitSprite()
{
	EmptySpriteData = (SPRITEDATA*)MemGet(sizeof(CPicture) + SIZEOFSPRITEDATA);
	EmptySpriteData->Refer = 1;		// po�et referenc�
	EmptySpriteData->Faze = 1;		// po�et f�z�
	EmptySpriteData->Smer = 1;		// po�et sm�r�
	EmptySpriteData->Klid = 1;		// po�et klidov�ch f�z�
	EmptySpriteData->Delay = SPRITEDEFDELAY; // prodleva mezi f�zemi
	EmptySpriteData->Level = SPRITEDEFLEVEL; // hladina k p�ekreslov�n�
	EmptySpriteData->Kroku = SPRITEDEFKROKU; // po�et f�z� na jednotku
	EmptySpriteData->Data[0].Init(); // inicializace obr�zku (velikost ICONSIZE)
}


/////////////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CSprite::CSprite() 
{ 
	Attach(EmptySpriteData); 
};

CSprite::CSprite(const CSprite& src) 
{ 
	Attach(src.pData); 
};

CSprite::CSprite(int faze, int smer)
{
	NewBuffer(faze, smer);
};

CSprite::~CSprite() 
{ 
	Detach(); 
};


/////////////////////////////////////////////////////////////////////////////
// statick� konstruktor a destruktor

void CSprite::Init()
{ 
	Attach(EmptySpriteData); 
};

void CSprite::Init(SPRITEDATA* data)
{ 
	Attach(data); 
};

void CSprite::Init(int faze, int smer)
{
	NewBuffer(faze, smer);
};

void CSprite::Term()
{ 
	Detach(); 
};


/////////////////////////////////////////////////////////////////////////////
// kopie do vlastn�ho bufferu p�ed modifikac�

void CSprite::CopyWrite()
{
	SPRITEDATA* data = pData;			// adresa star�ch dat
	long* refer = &(data->Refer);	// po�et referenc�

	if (*refer > 1)					// je n�jak� jin� majitel?
	{
		NewBuffer(data->Faze, data->Smer);	// vytvo�en� nov�ho bufferu

		for (int i = data->Faze * data->Smer - 1; i >= 0; i--)
		{
			pData->Data[i] = data->Data[i];
		}

		pData->Klid = data->Klid;	// p�enesen� po�tu klidov�ch f�z�
		pData->Delay = data->Delay;	// p�enesen� prodlevy mezi f�zemi
		pData->Level = data->Level;	// p�enesen� hladiny k p�ekreslov�n�
		pData->Kroku = data->Kroku;	// p�enesen� po�tu f�z� na jednotku

// odpojen� star�ch dat - v multithread m��e nastat i zru�en�
		if (LongDecrement(refer))
		{
#ifdef _MT
			MemFree(data);			// p��padn� zru�en� dat
#endif	// _MT
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� nov�ho sprajtu (p�ipraveno k z�pisu)

void CSprite::New(int faze, int smer)
{
	Detach();						// odpojen� star�ho sprajtu
	NewBuffer(faze, smer);			// vytvo�en� nov�ho bufferu
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� nov�ch rozm�r� sprajtu (po�et f�z� pro klid a pohyb, po�et sm�r�)

void CSprite::ReSize(int klid, int pohyb, int smer)
{
// kontrola, zda se parametry m�n�
	if ((klid == pData->Klid) &&
		(pohyb == pData->Faze - pData->Klid) &&
		(smer == pData->Smer))
	{
		return;
	}

// korekce hodnot
	if (klid < 1) klid = 1;
	if (klid > SPRITEMAXKLID) klid = SPRITEMAXKLID;
	if (pohyb < 0) pohyb = 0;
	if (pohyb > SPRITEMAXPOHYB) pohyb = SPRITEMAXPOHYB;
	if (smer < 1) smer = 1;
	if (smer > SPRITEMAXSMER) smer = SPRITEMAXSMER;

// vytvo�en� nov�ho bufferu
	SPRITEDATA* olddata = pData;
	NewBuffer(klid + pohyb, smer);
	SPRITEDATA* newdata = pData;
	newdata->Klid = klid;

// ukazatele dat
	CPicture* src = olddata->Data;
	CPicture* dst = newdata->Data;

// p��prava parametr�
	int oldklid = olddata->Klid;
	int newklid = newdata->Klid;
	int oldfaze = olddata->Faze;
	int newfaze = newdata->Faze;
	int oldpohyb = oldfaze - oldklid;
	int newpohyb = newfaze - newklid;
	int oldsmer = olddata->Smer;
	int newsmer = newdata->Smer;

// p�enesen� dat
	int snew, sold, newoff, oldoff, f;

	for (snew = 0; snew < newsmer; snew++)
	{
		sold = ((snew * oldsmer + (newsmer-1)/2) / newsmer) % oldsmer;

		newoff = snew * newfaze;
		oldoff = sold * oldfaze;

		for (f = 0; f < newklid; f++)
		{
			dst[newoff + f] = src[oldoff + (f % oldklid)];
		}

		newoff += newklid;
		oldoff += oldklid;

		if (oldpohyb)
		{
			for (f = 0; f < newpohyb; f++)
			{
				dst[newoff + f] = src[oldoff + (f % oldpohyb)];
			}
		}
	}

// odpojen� star�ho bufferu
	pData = olddata;
	Detach();
	pData = newdata;
}


/////////////////////////////////////////////////////////////////////////////
// poskytnut� prvku (s kontrolou platnosti offsetu/indexu)

const CPicture& _fastcall CSprite::Get(const int off) const
{
	if (IsValid(off))				// je offset platn�?
	{
		return pData->Data[off];	// prvek na dan�m offsetu
	}
	return EmptySpriteData->Data[0]; // neplatn� offset
}

const CPicture& _fastcall CSprite::Get(const int faze, const int smer) const
{
	if (IsValid(faze, smer))				// je index platn�?
	{
		return pData->Data[faze + smer*pData->Faze];	// prvek
	}
	return EmptySpriteData->Data[0]; // neplatn� offset
}


////////////////////////////////////////////////////////////////////
// nastaven� prvku (s kontrolou platnosti offsetu/indexu)

void _fastcall CSprite::Set(const int off, const CPicture& data)
{
	if (IsValid(off))				// je offset platn�?
	{
		pData->Data[off] = data;
	}
}

void _fastcall CSprite::Set(const int faze, const int smer, const CPicture& data)
{
	if (IsValid(faze, smer))				// je index platn�?
	{
		pData->Data[faze + smer*pData->Faze] = data;
	}
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� sprajtu ze souboru (TRUE=operace OK)

BOOL CSprite::LoadFile(CString jmeno)
{
// otev�en� souboru
	HANDLE hFile = ::CreateFile(jmeno, GENERIC_READ, 
		FILE_SHARE_READ, NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, NULL);

// test, zda byl soubor nalezen
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

// zji�t�n� velikosti souboru
	int size = (int)::SetFilePointer(hFile, 0, NULL, FILE_END);
	::SetFilePointer(hFile, 0, NULL, FILE_BEGIN);

// kontrola velikosti souboru
	if ((size < 100) || (size > 200000000))
	{
		::CloseHandle(hFile);		// uzav�en� souboru
		return FALSE;
	}

// p��prava bufferu k na�ten� souboru
	BYTE* buf = (BYTE*)MemGet(size + 1000);

// na�ten� souboru
	DWORD read;
	::ReadFile(hFile, buf, size, &read, NULL);

// uzav�en� souboru
	::CloseHandle(hFile);		// uzav�en� souboru

// na�ten� z�kladn�ch parametr�
	SPRITEFILE* spr = (SPRITEFILE*)buf;
	int faze = spr->Faze;			// po�et f�z� celkem
	int smer = spr->Smer;			// po�et sm�r� celkem
	int klid = spr->Klid;			// po�et klidov�ch f�z�
	int width = spr->Width;			// ���ka obr�zku
	int height = spr->Height;		// v��ka obr�zku
	int delay = spr->Delay;			// prodleva mezi f�zemi
	int level = spr->Level;			// hladina k p�ekreslov�n�
	double kroku = spr->Kroku;		// po�et f�z� na krok
	int pals = spr->Colors;			// po�et palet
	int format = spr->Format;		// form�t dat

// kontrola platnosti souboru
	if ((read != (DWORD)size) ||
		(spr->Ident[0] != 'P') ||
		(spr->Ident[1] != 'S') ||
		(spr->Ident[2] != 'P') ||
		(spr->Ident[3] != 'R') ||
		(faze < 1) ||
		(faze > SPRITEMAXFAZE) ||
		(smer < 1) ||
		(smer > SPRITEMAXSMER) ||
		(klid < 1) ||
		(klid > SPRITEMAXKLID) ||
		(klid > faze) ||
		(faze-klid > SPRITEMAXPOHYB) ||
		(width < 1) ||
		(height < 1) ||
		(delay < 0) ||
		(delay > SPRITEMAXDELAY) ||
		(kroku < 0) ||
		(kroku > SPRITEMAXKROKU) ||
		(pals < 10) ||
		(pals > 256) ||
		(format > 1))
	{
		MemFree(buf);
		return FALSE;
	}

// p��prava konverzn� tabulky palet
	BITMAPINFO* bmp = (BITMAPINFO*)MemGet(sizeof(BITMAPINFO) + pals*4 + 200);
	bmp->bmiHeader.biClrUsed = pals;
	bmp->bmiHeader.biBitCount = 8;
	MemCopy(bmp->bmiColors, spr->ColorTab, pals*4);
	bmp->bmiColors[0] = StdBitmapInfo->bmiColors[0];
	GenKonvPal(bmp);
	MemFree(bmp);

// vytvo�en� sprajtu a nastaven� parametr�
	New(faze, smer);
	pData->Klid = klid;			// po�et klidov�ch f�z�
	pData->Delay = delay;		// prodleva mezi f�zemi
	pData->Level = level;		// hladina k p�ekreslov�n�
	pData->Kroku = kroku;		// po�et f�z� na jednotkovou vzd�lenost

// dekomprimace dat
	if (format == 1)
	{
		BYTE* buf0 = (BYTE*)MemGet(faze*smer*width*height + 1000);
		DeCompress(buf0, buf + SIZEOFSPRITEFILE + pals*4 + 4, faze*smer*width*height, width);

		for (int i = 0; i < faze*smer; i++)
		{
			pData->Data[i].New(width, height);
			pData->Data[i].CopyKonvData(buf0 + i*width*height);
		}
		MemFree(buf0);
	}

// na�ten� obr�zk�
	else
	{
		for (int i = 0; i < faze*smer; i++)
		{
			pData->Data[i].New(width, height);
			pData->Data[i].CopyKonvData(buf + SIZEOFSPRITEFILE + pals*4 + i*width*height);
		}
	}

// zru�en� bufferu
	MemFree(buf);

// p��znak - na�teno OK
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// ulo�en� do souboru (FALSE=chyba)

BOOL CSprite::SaveFile(CString jmeno) const
{

// otev�en� souboru
	HANDLE hFile = ::CreateFile(jmeno, GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

// test, zda byl soubor vytvo�en
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

// p��prava velikosti souboru
	int width = pData->Data[0].Width();
	int height = pData->Data[0].Height();
	int icons = pData->Faze * pData->Smer;

// buffer k ulo�en� souboru
	BYTE* buf = (BYTE*)MemGet(SIZEOFSPRITEFILE + StdColors*4 + 4 + icons*width*height*3/2);

// napln�n� z�hlav� souboru
	SPRITEFILE* head = (SPRITEFILE*)buf;
	head->Ident[0] = 'P';					// identifikace
	head->Ident[1] = 'S';
	head->Ident[2] = 'P';
	head->Ident[3] = 'R';
	head->Faze = pData->Faze;				// po�et f�z� celkem
	head->Smer = pData->Smer;				// po�et sm�r�
	head->Klid = pData->Klid;				// z toho po�et klidov�ch f�z�
	head->Width = (WORD)width;				// ���ka obr�zku
	head->Height = (WORD)height;			// v��ka obr�zku
	head->Delay = pData->Delay;				// prodleva mezi f�zemi v milisekund�ch
	head->Level = pData->Level;				// hladina k p�ekreslov�n�
	head->Kroku = pData->Kroku;				// po�et f�z� na jednotkovou vzd�lenost
	head->Colors = (short)StdColors;		// po�et palet
	head->Format = 1;						// form�t - je komprese dat

// p�enesen� palet
	MemCopy(head->ColorTab, StdBitmapInfo->bmiColors, StdColors*4);

// p��prava a komprimace dat obr�zku
	BYTE* buf0 = (BYTE*)MemGet(width*height*icons);
	BYTE* dst = buf0;
	for (int i = 0; i < icons; i++)
	{
		MemCopy(dst, pData->Data[i].DataData(), width*height);
		dst += width*height;
	}
	int n = Compress(buf + SIZEOFSPRITEFILE + StdColors*4 + 4, buf0, width*height*icons, width);
	*(int*)(buf + SIZEOFSPRITEFILE + StdColors*4) = n;
	MemFree(buf0);

// ulo�en� souboru
	DWORD write;
	int size = SIZEOFSPRITEFILE + StdColors*4 + 4 + n;
	BOOL result = ::WriteFile(hFile, buf, size, &write, NULL);

// uzav�en� souboru
	::CloseHandle(hFile);

// zru�en� datov�ho bufferu
	MemFree(buf);

// p�i chyb� zru�en� souboru
	if (!result || (write != (DWORD)size))
	{
		::DeleteFile(jmeno);
		return FALSE;
	}

// p��znak - ulo�eno OK
	return TRUE;
}

/*
BOOL CSprite::LoadFile(CString jmeno)
{
// otev�en� souboru
	HANDLE hFile = ::CreateFile(jmeno, GENERIC_READ, 
		FILE_SHARE_READ, NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, NULL);

// test, zda byl soubor nalezen
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

// zji�t�n� velikosti souboru
	int size = (int)::SetFilePointer(hFile, 0, NULL, FILE_END);
	::SetFilePointer(hFile, 0, NULL, FILE_BEGIN);

// kontrola velikosti souboru
	if ((size < 100) || (size > 200000000))
	{
		::CloseHandle(hFile);		// uzav�en� souboru
		return FALSE;
	}

// p��prava bufferu k na�ten� souboru
	BYTE* buf = (BYTE*)MemGet(size + 100);

// na�ten� souboru
	DWORD read;
	::ReadFile(hFile, buf, size, &read, NULL);

// uzav�en� souboru
	::CloseHandle(hFile);		// uzav�en� souboru

// na�ten� z�kladn�ch parametr�
	SPRITEFILE* spr = (SPRITEFILE*)buf;
	int faze = spr->Faze;			// po�et f�z� celkem
	int smer = spr->Smer;			// po�et sm�r� celkem
	int klid = spr->Klid;			// po�et klidov�ch f�z�
	int width = spr->Width;			// ���ka obr�zku
	int height = spr->Height;		// v��ka obr�zku
	int delay = spr->Delay;			// prodleva mezi f�zemi
	int level = spr->Level;			// hladina k p�ekreslov�n�
	double kroku = spr->Kroku;		// po�et f�z� na krok
	int pals = spr->Colors;			// po�et palet

// kontrola platnosti souboru
	if ((read != (DWORD)size) ||
		(spr->Ident[0] != 'P') ||
		(spr->Ident[1] != 'S') ||
		(spr->Ident[2] != 'P') ||
		(spr->Ident[3] != 'R') ||
		(faze < 1) ||
		(faze > SPRITEMAXFAZE) ||
		(smer < 1) ||
		(smer > SPRITEMAXSMER) ||
		(klid < 1) ||
		(klid > SPRITEMAXKLID) ||
		(klid > faze) ||
		(faze-klid > SPRITEMAXPOHYB) ||
		(width < 1) ||
		(height < 1) ||
		(delay < 0) ||
		(delay > SPRITEMAXDELAY) ||
		(kroku < 0) ||
		(kroku > SPRITEMAXKROKU) ||
		(pals < 10) ||
		(pals > 256) ||
		((int)(SIZEOFSPRITEFILE + pals*4 + faze*smer*width*height) > size))
	{
		MemFree(buf);
		return FALSE;
	}

// p��prava konverzn� tabulky palet
	BITMAPINFO* bmp = (BITMAPINFO*)MemGet(sizeof(BITMAPINFO) + pals*4 + 200);
	bmp->bmiHeader.biClrUsed = pals;
	bmp->bmiHeader.biBitCount = 8;
	MemCopy(bmp->bmiColors, spr->ColorTab, pals*4);
	bmp->bmiColors[0] = StdBitmapInfo->bmiColors[0];
	GenKonvPal(bmp);
	MemFree(bmp);

// vytvo�en� sprajtu a nastaven� parametr�
	New(faze, smer);
	pData->Klid = klid;			// po�et klidov�ch f�z�
	pData->Delay = delay;		// prodleva mezi f�zemi
	pData->Level = level;		// hladina k p�ekreslov�n�
	pData->Kroku = kroku;		// po�et f�z� na jednotkovou vzd�lenost

// na�ten� obr�zk�
	for (int i = 0; i < faze*smer; i++)
	{
		pData->Data[i].New(width, height);
		pData->Data[i].CopyKonvData(buf + SIZEOFSPRITEFILE + pals*4 + i*width*height);
	}

// zru�en� bufferu
	MemFree(buf);

// p��znak - na�teno OK
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// ulo�en� do souboru (FALSE=chyba)

BOOL CSprite::SaveFile(CString jmeno) const
{

// otev�en� souboru
	HANDLE hFile = ::CreateFile(jmeno, GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

// test, zda byl soubor vytvo�en
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

// p��prava velikosti souboru
	int width = pData->Data[0].Width();
	int height = pData->Data[0].Height();
	int icons = pData->Faze * pData->Smer;
	int size = SIZEOFSPRITEFILE + StdColors*4 + icons*width*height;

// buffer k ulo�en� souboru
	BYTE* buf = (BYTE*)MemGet(size);

// napln�n� z�hlav� souboru
	SPRITEFILE* head = (SPRITEFILE*)buf;
	head->Ident[0] = 'P';					// identifikace
	head->Ident[1] = 'S';
	head->Ident[2] = 'P';
	head->Ident[3] = 'R';
	head->Faze = pData->Faze;				// po�et f�z� celkem
	head->Smer = pData->Smer;				// po�et sm�r�
	head->Klid = pData->Klid;				// z toho po�et klidov�ch f�z�
	head->Width = (WORD)width;				// ���ka obr�zku
	head->Height = (WORD)height;			// v��ka obr�zku
	head->Delay = pData->Delay;				// prodleva mezi f�zemi v milisekund�ch
	head->Level = pData->Level;				// hladina k p�ekreslov�n�
	head->Kroku = pData->Kroku;				// po�et f�z� na jednotkovou vzd�lenost
	head->Colors = StdColors;				// po�et palet

// p�enesen� palet
	MemCopy(head->ColorTab, StdBitmapInfo->bmiColors, StdColors*4);

// p�enesen� dat obr�zku
	BYTE* dst = buf + SIZEOFSPRITEFILE + StdColors*4;
	for (int i = 0; i < icons; i++)
	{
		MemCopy(dst, pData->Data[i].DataData(), width*height);
		dst += width*height;
	}


// ulo�en� souboru
	DWORD write;
	BOOL result = ::WriteFile(hFile, buf, size, &write, NULL);

// uzav�en� souboru
	::CloseHandle(hFile);

// zru�en� datov�ho bufferu
	MemFree(buf);

// p�i chyb� zru�en� souboru
	if (!result || (write != (DWORD)size))
	{
		::DeleteFile(jmeno);
		return FALSE;
	}

// p��znak - ulo�eno OK
	return TRUE;
}
*/

/////////////////////////////////////////////////////////////////////////////
// oper�tor p�i�azen�

const CSprite& CSprite::operator= (const CSprite& src)
{
	Detach();				// zru�en� star�ch dat
	Attach(src.pData);		// p�i�azen� nov�ch dat
	return *this;
}


/***************************************************************************\
*																			*
*								Buffer sprajt�								*
*																			*
\***************************************************************************/


////////////////////////////////////////////////////////////////////
// vytvo�en� nov�ch dat

#define NEWDATANUM 256				// po�et nov� vytvo�en�ch polo�ek (1 KB)

void CBufSprite::NewData()
{
	int i = NEWDATANUM;			// po�et nov�ch polo�ek
	int next = m_Max;			// p��t� polo�ka - 1
	m_Max = next + i;			// zv��en� po�tu polo�ek (o 1 KB)

	MemBuf(m_Data, m_Max);		// zv��en� velikosti bufferu

	MemBuf(m_Valid, m_Max);		// zv��en� velikosti bufferu platnosti
	MemFill(m_Valid + next, NEWDATANUM, FALSE); // nastaven� p��znak� na neplatn� polo�ky

	CSprite* data = m_Data + next - 1; // ukazatel dat - 1
	for (; i > 0; i--)
	{
		data++;					// zv��en� ukazatele polo�ek
		next++;					// zv��en� indexu p��t� polo�ky
		*(int*)data = next;		// odkaz na p��t� polo�ku
	}
	*(int*)data = m_Next;		// nav�z�n� na dal�� polo�ku
	m_Next = m_Max-NEWDATANUM;	// odkaz na prvn� novou polo�ku
};


////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CBufSprite::CBufSprite()
{
	m_Data = NULL;			// nen� buffer dat
	m_Valid = NULL;			// nen� buffer platnosti
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
	m_Next = -1;			// p�i�t� voln� polo�ka (-1=nen�)
}

CBufSprite::~CBufSprite()
{
	DelAll();				// zru�en� v�ech polo�ek
}


////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

void CBufSprite::Init()
{
	m_Data = NULL;			// nen� buffer dat
	m_Valid = NULL;			// nen� buffer platnosti
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
	m_Next = -1;			// p�i�t� voln� polo�ka (-1=nen�)
}

void CBufSprite::Term()
{
	DelAll();				// zru�en� v�ech polo�ek
}

////////////////////////////////////////////////////////////////////
// zru�en� v�ech polo�ek v bufferu (ukl�d�n� za�ne op�t po �ad�)

void CBufSprite::DelAll()
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
// poskytnut� polo�ky (s kontrolou platnosti indexu)

const CSprite& _fastcall CBufSprite::Get(const int index) const
{
	if (IsValid(index))			// je index platn�?
	{
		return m_Data[index];	// polo�ka na dan�m indexu
	}
	return EmptySprite;			// pro neplatn� index vr�t� pr�zdn� sprajt
}

////////////////////////////////////////////////////////////////////
// nastaven� polo�ky (s kontrolou platnosti indexu)

void _fastcall CBufSprite::Set(const int index, const CSprite& data)
{
	if (IsValid(index))			// je index platn�?
	{
		m_Data[index] = data;
	}
}

////////////////////////////////////////////////////////////////////
// zru�en� polo�ky (s kontrolou platnosti indexu)

void _fastcall CBufSprite::Del(const int index)
{
	if (IsValid(index))						// je index platn�?
	{
		m_Data[index].Term();				// ukon�en� objektu
		*(int*)(m_Data + index) = m_Next;	// p��t� voln� polo�ka
		m_Valid[index] = FALSE;				// zru�en� p��znaku platnosti
		m_Num--;							// sn�en� ��ta�e platn�ch polo�ek
		m_Next = index;						// odkaz na tuto polo�ku
	}
}


////////////////////////////////////////////////////////////////////
// vytvo�en� polo�ky (vrac� index polo�ky)

int CBufSprite::New()
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result].Init();		// inicializace polo�ky
	return result;
}

int CBufSprite::New(int faze, int smer)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result].Init(faze, smer);	// inicializace polo�ky
	return result;
}

////////////////////////////////////////////////////////////////////
// p�id�n� polo�ky (vrac� index polo�ky)

int _fastcall CBufSprite::Add(const CSprite& data)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result].Init(data.Data());	// inicializace polo�ky
	return result;
}


////////////////////////////////////////////////////////////////////
// duplikace polo�ky (s kontrolou platnosti indexu, vrac� index polo�ky)

int _fastcall CBufSprite::Dup(const int index)
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


/////////////////////////////////////////////////////////////////////////////
// oper�tor p�i�azen�

const CBufSprite& CBufSprite::operator= (const CBufSprite& src)
{
	DelAll();					// zru�en� star�ch dat
	int index = 0;				// index na��tan� polo�ky
	int i = src.Max();			// velikost zdrojov�ho bufferu

	for (; i > 0; i--)			// pro v�echny polo�ky v bufferu
	{
		if (src.m_Valid[index])	// je to platn� polo�ka?
		{
			Add(src[index]);	// kopie polo�ky
		}
		index++;				// inkrementace �tec�ho indexu
	}
	ASSERT(m_Num == src.Num());
	return *this;
}
