
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
// statick� inicializace sprajt� (p�i chyb� pam�ti vrac� FALSE)

BOOL InitSprite()
{
	EmptySpriteData = (SPRITEDATA*)MemGet(sizeof(CPicture) + SIZEOFSPRITEDATA);
	if (EmptySpriteData == NULL) return FALSE;
	EmptySpriteData->Refer = 1;		// po�et referenc�
	EmptySpriteData->Faze = 1;		// po�et f�z�
	EmptySpriteData->Smer = 1;		// po�et sm�r�
	EmptySpriteData->Klid = 1;		// po�et klidov�ch f�z�
	EmptySpriteData->Delay = SPRITEDEFDELAY; // prodleva mezi f�zemi
	EmptySpriteData->Level = SPRITEDEFLEVEL; // hladina k p�ekreslov�n�
	EmptySpriteData->Kroku = SPRITEDEFKROKU; // po�et f�z� na jednotku
	EmptySpriteData->Data[0].Init(); // inicializace obr�zku (velikost ICONSIZE)
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CSprite::CSprite() 
{ 
	m_Visible = false;
	m_Moving = false;
	m_DispReq = false;

	m_X = 0;
	m_Y = 0;

	m_Timer = 0;
	m_KrokCit = 0;
	m_FazeN = 0;
	m_SmerN = 0;

	m_CilX = 0;
	m_CilY = 0;

	m_AktX = 0;
	m_AktY = 0;

	m_dX = 0;
	m_dY = 0;

	m_Smer = 0;
	m_FazeNext = -1;
	m_Status = -1;

	Attach(EmptySpriteData); 
};

CSprite::CSprite(const CSprite& src) 
{ 
	m_Visible = false;
	m_Moving = false;
	m_DispReq = false;

	m_X = 0;
	m_Y = 0;

	m_Timer = 0;
	m_KrokCit = 0;
	m_FazeN = 0;
	m_SmerN = 0;

	m_CilX = 0;
	m_CilY = 0;

	m_AktX = 0;
	m_AktY = 0;

	m_dX = 0;
	m_dY = 0;

	m_Smer = 0;
	m_FazeNext = -1;
	m_Status = -1;

	Attach(src.pData); 
};

CSprite::CSprite(int faze, int smer)
{
	m_Visible = false;
	m_Moving = false;
	m_DispReq = false;

	m_X = 0;
	m_Y = 0;

	m_Timer = 0;
	m_KrokCit = 0;
	m_FazeN = 0;
	m_SmerN = 0;

	m_CilX = 0;
	m_CilY = 0;

	m_AktX = 0;
	m_AktY = 0;

	m_dX = 0;
	m_dY = 0;

	m_Smer = 0;
	m_FazeNext = -1;
	m_Status = -1;

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
	m_Visible = false;
	m_Moving = false;
	m_DispReq = false;

	m_X = 0;
	m_Y = 0;

	m_Timer = 0;
	m_KrokCit = 0;
	m_FazeN = 0;
	m_SmerN = 0;

	m_CilX = 0;
	m_CilY = 0;

	m_AktX = 0;
	m_AktY = 0;

	m_dX = 0;
	m_dY = 0;

	m_Smer = 0;
	m_FazeNext = -1;
	m_Status = -1;

	Attach(EmptySpriteData); 
};

void CSprite::Init(SPRITEDATA* data)
{ 
	m_Visible = false;
	m_Moving = false;
	m_DispReq = false;

	m_X = 0;
	m_Y = 0;

	m_Timer = 0;
	m_KrokCit = 0;
	m_FazeN = 0;
	m_SmerN = 0;

	m_CilX = 0;
	m_CilY = 0;

	m_AktX = 0;
	m_AktY = 0;

	m_dX = 0;
	m_dY = 0;

	m_Smer = 0;
	m_FazeNext = -1;
	m_Status = -1;

	Attach(data); 
};

void CSprite::Init(int faze, int smer)
{
	m_Visible = false;
	m_Moving = false;
	m_DispReq = false;

	m_X = 0;
	m_Y = 0;

	m_Timer = 0;
	m_KrokCit = 0;
	m_FazeN = 0;
	m_SmerN = 0;

	m_CilX = 0;
	m_CilY = 0;

	m_AktX = 0;
	m_AktY = 0;

	m_dX = 0;
	m_dY = 0;

	m_Smer = 0;
	m_FazeNext = -1;
	m_Status = -1;

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
// vytvo�en� nov�ho sprajtu (p�ipraveno k z�pisu, data jsou n�hodn�)

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


/////////////////////////////////////////////////////////////////////////////
// poskytnut� dat aktu�ln�ho obr�zku (podle aktu�ln�ho sm�ru a f�ze, bez kontroly)

PICTUREDATA* CSprite::AktPicData()
{
	ASSERT((DWORD)m_FazeN < (DWORD)pData->Faze);
	ASSERT((DWORD)m_SmerN < (DWORD)pData->Smer);
	CPicture* pic = &pData->Data[m_FazeN + m_SmerN*pData->Faze];
	pic->DeComp();
	return pic->Data();
}


/////////////////////////////////////////////////////////////////////////////
// vypr�zdn�n� sprajtu (uvoln�n� dat)

void CSprite::Empty()
{ 
	Detach(); 
	Attach(EmptySpriteData); 
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
// na�ten� sprajtu ze souboru

void CSprite::LoadFile()
{
// �schova offsetu souboru
	int oldoff = FileReadOff;

// na�ten� z�hlav� souboru
	SPRITEFILE spr;
	spr.Colors = 0;
	FileReadBlok((BYTE*)&spr, SIZEOFSPRITEFILE);

// na�ten� z�kladn�ch parametr�
	int faze = spr.Faze;			// po�et f�z� celkem
	int smer = spr.Smer;			// po�et sm�r� celkem
	int klid = spr.Klid;			// po�et klidov�ch f�z�
	int width = spr.Width;			// ���ka obr�zku
	int height = spr.Height;		// v��ka obr�zku
	int delay = spr.Delay;			// prodleva mezi f�zemi
	int level = spr.Level;			// hladina k p�ekreslov�n�
	double kroku = spr.Kroku;		// po�et f�z� na krok
	int pals = spr.Colors;			// po�et palet
	int format = spr.Format;		// form�t dat

// kontrola platnosti souboru
	if ((spr.Ident[0] != 'P') ||
		(spr.Ident[1] != 'S') ||
		(spr.Ident[2] != 'P') ||
		(spr.Ident[3] != 'R') ||
		(faze < 1) ||
		(faze > SPRITEMAXFAZE) ||
		(smer < 1) ||
		(smer > SPRITEMAXSMER) ||
		(klid < 1) ||
		(klid > SPRITEMAXKLID) ||
		(klid > faze) ||
		(faze - klid > SPRITEMAXPOHYB) ||
		(width < 1) ||
		(width > 32768) ||
		(height < 1) ||
		(height > 32768) ||
		(delay < 0) ||
		(delay > SPRITEMAXDELAY) ||
		(kroku < 0) ||
		(kroku > SPRITEMAXKROKU) ||
		(pals < 10) ||
		(pals > 256) ||
		(format > 1))
	{
		FileReadOff = oldoff;
		FileError = true;
		return;
	}

// p��prava konverzn� tabulky palet
	BITMAPINFO* bmp = (BITMAPINFO*)MemGet(sizeof(BITMAPINFO) + pals*4 + 200);
	bmp->bmiHeader.biClrUsed = pals;
	bmp->bmiHeader.biBitCount = 8;
	FileReadBlok((BYTE*)bmp->bmiColors, pals*4);
	bmp->bmiColors[0] = StdBitmapInfo->bmiColors[0];

// barva 1 bude jako st�n (krom� importu ze star�� verze)
	if (pals >= StdColors)
	{
		bmp->bmiColors[1] = StdBitmapInfo->bmiColors[1];
	}

	GenKonvPal(bmp);
	MemFree(bmp);

// vytvo�en� sprajtu a nastaven� parametr�
	New(faze, smer);
	pData->Klid = klid;			// po�et klidov�ch f�z�
	pData->Delay = delay;		// prodleva mezi f�zemi
	pData->Level = level;		// hladina k p�ekreslov�n�
	pData->Kroku = kroku;		// po�et f�z� na jednotkovou vzd�lenost

// na�ten� obr�zk�
	int i = width*height*faze*smer;
	BYTE* data = (BYTE*)MemGet(i);

	if (format == 1)
	{
		long n;
		FileReadBlok((BYTE*)&n, 4);
		BYTE* data0 = (BYTE*)MemGet(n);
		FileReadBlok(data0, n);
		DeCompress(data, data0, i, width);
		MemFree(data0);
	}
	else
	{
		FileReadBlok(data, i);
	}

	for (i = 0; i < faze*smer; i++)
	{
		pData->Data[i].New(width, height);
		pData->Data[i].CopyKonvData(data + i*width*height);
	}

	MemFree(data);
}


/////////////////////////////////////////////////////////////////////////////
// ulo�en� do souboru

void CSprite::SaveFile() const
{
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
		pData->Data[i].DeComp();
		MemCopy(dst, pData->Data[i].DataData(), width*height);
		dst += width*height;
	}
	int n = Compress(buf + SIZEOFSPRITEFILE + StdColors*4 + 4, buf0, width*height*icons, width);
	*(long*)(buf + SIZEOFSPRITEFILE + StdColors*4) = n;
	MemFree(buf0);

// ulo�en� souboru
	FileWriteBlok(buf, SIZEOFSPRITEFILE + StdColors*4 + 4 + n);

// zru�en� datov�ho bufferu
	MemFree(buf);
}


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
// konstruktor a destruktor

CBufSprite::CBufSprite()
{
	m_Data = NULL;			// nen� buffer dat
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
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
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
}

void CBufSprite::Term()
{
	DelAll();				// zru�en� v�ech polo�ek
}


////////////////////////////////////////////////////////////////////
// vytvo�en� nov�ch dat

void CBufSprite::NewData()
{
	m_Max *= 2;
	if (m_Max == 0) m_Max = 0x400;
//	m_Max += 0x400;			// zv��en� po�tu polo�ek (o 4 KB)
	MemBuf(m_Data, m_Max);	// zv��en� velikosti bufferu
};


////////////////////////////////////////////////////////////////////
// zru�en� v�ech polo�ek v bufferu

void CBufSprite::DelAll()
{
	Del(m_Num);					// zru�en� polo�ek
	MemBuf(m_Data, 0);			// zru�en� bufferu dat
	m_Max = 0;					// nen� ��dn� polo�ka v bufferu
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


/////////////////////////////////////////////////////////////////////////////
// vypr�zdn�n� polo�ky (bez jej�ho zru�en� - jen pro uvoln�n� dat)

void _fastcall CBufSprite::Empty(const int index)
{
	if (IsValid(index))					// je index platn�?
	{
		m_Data[index].Empty();
	}
}


////////////////////////////////////////////////////////////////////
// zru�en� polo�ek z konce bufferu

void _fastcall CBufSprite::Del(int num)
{
	if (num > m_Num) num = m_Num;

	CSprite* spr = m_Data + m_Num - 1;
	m_Num -= num;

	for (; num > 0; num--)
	{
		spr->Term();
		spr--;
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
// duplikace polo�ky (s kontrolou platnosti indexu, vrac� index prvn� polo�ky)

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

int _fastcall CBufSprite::Dup(const int index, int num)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky

	if (IsValid(index))					// je index platn�?
	{
		SPRITEDATA* data = m_Data[index].Data();

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


/////////////////////////////////////////////////////////////////////////////
// oper�tor p�i�azen�

const CBufSprite& CBufSprite::operator= (const CBufSprite& src)
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
