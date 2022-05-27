
#include "Main.h"

/***************************************************************************\
*																			*
*									Sprajt									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inicializaèní prázdný sprajt (modifikuje se poèet referencí!)

SPRITEDATA*	EmptySpriteData = NULL;

/////////////////////////////////////////////////////////////////////////////
// statická inicializace sprajtù (pøi chybì pamìti vrací FALSE)

BOOL InitSprite()
{
	EmptySpriteData = (SPRITEDATA*)MemGet(sizeof(CPicture) + SIZEOFSPRITEDATA);
	if (EmptySpriteData == NULL) return FALSE;
	EmptySpriteData->Refer = 1;		// poèet referencí
	EmptySpriteData->Faze = 1;		// poèet fází
	EmptySpriteData->Smer = 1;		// poèet smìrù
	EmptySpriteData->Klid = 1;		// poèet klidových fází
	EmptySpriteData->Delay = SPRITEDEFDELAY; // prodleva mezi fázemi
	EmptySpriteData->Level = SPRITEDEFLEVEL; // hladina k pøekreslování
	EmptySpriteData->Kroku = SPRITEDEFKROKU; // poèet fází na jednotku
	EmptySpriteData->Data[0].Init(); // inicializace obrázku (velikost ICONSIZE)
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
// statický konstruktor a destruktor

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
// kopie do vlastního bufferu pøed modifikací

void CSprite::CopyWrite()
{
	SPRITEDATA* data = pData;			// adresa starých dat
	long* refer = &(data->Refer);	// poèet referencí

	if (*refer > 1)					// je nìjaký jiný majitel?
	{
		NewBuffer(data->Faze, data->Smer);	// vytvoøení nového bufferu

		for (int i = data->Faze * data->Smer - 1; i >= 0; i--)
		{
			pData->Data[i] = data->Data[i];
		}

		pData->Klid = data->Klid;	// pøenesení poètu klidových fází
		pData->Delay = data->Delay;	// pøenesení prodlevy mezi fázemi
		pData->Level = data->Level;	// pøenesení hladiny k pøekreslování
		pData->Kroku = data->Kroku;	// pøenesení poètu fází na jednotku

// odpojení starých dat - v multithread mùže nastat i zrušení
		if (LongDecrement(refer))
		{
#ifdef _MT
			MemFree(data);			// pøípadné zrušení dat
#endif	// _MT
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// vytvoøení nového sprajtu (pøipraveno k zápisu, data jsou náhodná)

void CSprite::New(int faze, int smer)
{
	Detach();						// odpojení starého sprajtu
	NewBuffer(faze, smer);			// vytvoøení nového bufferu
}


/////////////////////////////////////////////////////////////////////////////
// nastavení nových rozmìrù sprajtu (poèet fází pro klid a pohyb, poèet smìrù)

void CSprite::ReSize(int klid, int pohyb, int smer)
{
// kontrola, zda se parametry mìní
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

// vytvoøení nového bufferu
	SPRITEDATA* olddata = pData;
	NewBuffer(klid + pohyb, smer);
	SPRITEDATA* newdata = pData;
	newdata->Klid = klid;

// ukazatele dat
	CPicture* src = olddata->Data;
	CPicture* dst = newdata->Data;

// pøíprava parametrù
	int oldklid = olddata->Klid;
	int newklid = newdata->Klid;
	int oldfaze = olddata->Faze;
	int newfaze = newdata->Faze;
	int oldpohyb = oldfaze - oldklid;
	int newpohyb = newfaze - newklid;
	int oldsmer = olddata->Smer;
	int newsmer = newdata->Smer;

// pøenesení dat
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

// odpojení starého bufferu
	pData = olddata;
	Detach();
	pData = newdata;
}


/////////////////////////////////////////////////////////////////////////////
// poskytnutí prvku (s kontrolou platnosti offsetu/indexu)

const CPicture& _fastcall CSprite::Get(const int off) const
{
	if (IsValid(off))				// je offset platný?
	{
		return pData->Data[off];	// prvek na daném offsetu
	}
	return EmptySpriteData->Data[0]; // neplatný offset
}

const CPicture& _fastcall CSprite::Get(const int faze, const int smer) const
{
	if (IsValid(faze, smer))				// je index platný?
	{
		return pData->Data[faze + smer*pData->Faze];	// prvek
	}
	return EmptySpriteData->Data[0]; // neplatný offset
}


/////////////////////////////////////////////////////////////////////////////
// poskytnutí dat aktuálního obrázku (podle aktuálního smìru a fáze, bez kontroly)

PICTUREDATA* CSprite::AktPicData()
{
	ASSERT((DWORD)m_FazeN < (DWORD)pData->Faze);
	ASSERT((DWORD)m_SmerN < (DWORD)pData->Smer);
	CPicture* pic = &pData->Data[m_FazeN + m_SmerN*pData->Faze];
	pic->DeComp();
	return pic->Data();
}


/////////////////////////////////////////////////////////////////////////////
// vyprázdnìní sprajtu (uvolnìní dat)

void CSprite::Empty()
{ 
	Detach(); 
	Attach(EmptySpriteData); 
}


////////////////////////////////////////////////////////////////////
// nastavení prvku (s kontrolou platnosti offsetu/indexu)

void _fastcall CSprite::Set(const int off, const CPicture& data)
{
	if (IsValid(off))				// je offset platný?
	{
		pData->Data[off] = data;
	}
}

void _fastcall CSprite::Set(const int faze, const int smer, const CPicture& data)
{
	if (IsValid(faze, smer))				// je index platný?
	{
		pData->Data[faze + smer*pData->Faze] = data;
	}
}


/////////////////////////////////////////////////////////////////////////////
// naètení sprajtu ze souboru

void CSprite::LoadFile()
{
// úschova offsetu souboru
	int oldoff = FileReadOff;

// naètení záhlaví souboru
	SPRITEFILE spr;
	spr.Colors = 0;
	FileReadBlok((BYTE*)&spr, SIZEOFSPRITEFILE);

// naètení základních parametrù
	int faze = spr.Faze;			// poèet fází celkem
	int smer = spr.Smer;			// poèet smìrù celkem
	int klid = spr.Klid;			// poèet klidových fází
	int width = spr.Width;			// šíøka obrázku
	int height = spr.Height;		// výška obrázku
	int delay = spr.Delay;			// prodleva mezi fázemi
	int level = spr.Level;			// hladina k pøekreslování
	double kroku = spr.Kroku;		// poèet fází na krok
	int pals = spr.Colors;			// poèet palet
	int format = spr.Format;		// formát dat

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

// pøíprava konverzní tabulky palet
	BITMAPINFO* bmp = (BITMAPINFO*)MemGet(sizeof(BITMAPINFO) + pals*4 + 200);
	bmp->bmiHeader.biClrUsed = pals;
	bmp->bmiHeader.biBitCount = 8;
	FileReadBlok((BYTE*)bmp->bmiColors, pals*4);
	bmp->bmiColors[0] = StdBitmapInfo->bmiColors[0];

// barva 1 bude jako stín (kromì importu ze starší verze)
	if (pals >= StdColors)
	{
		bmp->bmiColors[1] = StdBitmapInfo->bmiColors[1];
	}

	GenKonvPal(bmp);
	MemFree(bmp);

// vytvoøení sprajtu a nastavení parametrù
	New(faze, smer);
	pData->Klid = klid;			// poèet klidových fází
	pData->Delay = delay;		// prodleva mezi fázemi
	pData->Level = level;		// hladina k pøekreslování
	pData->Kroku = kroku;		// poèet fází na jednotkovou vzdálenost

// naètení obrázkù
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
// uložení do souboru

void CSprite::SaveFile() const
{
// pøíprava velikosti souboru
	int width = pData->Data[0].Width();
	int height = pData->Data[0].Height();
	int icons = pData->Faze * pData->Smer;

// buffer k uložení souboru
	BYTE* buf = (BYTE*)MemGet(SIZEOFSPRITEFILE + StdColors*4 + 4 + icons*width*height*3/2);

// naplnìní záhlaví souboru
	SPRITEFILE* head = (SPRITEFILE*)buf;
	head->Ident[0] = 'P';					// identifikace
	head->Ident[1] = 'S';
	head->Ident[2] = 'P';
	head->Ident[3] = 'R';
	head->Faze = pData->Faze;				// poèet fází celkem
	head->Smer = pData->Smer;				// poèet smìrù
	head->Klid = pData->Klid;				// z toho poèet klidových fází
	head->Width = (WORD)width;				// šíøka obrázku
	head->Height = (WORD)height;			// výška obrázku
	head->Delay = pData->Delay;				// prodleva mezi fázemi v milisekundách
	head->Level = pData->Level;				// hladina k pøekreslování
	head->Kroku = pData->Kroku;				// poèet fází na jednotkovou vzdálenost
	head->Colors = (short)StdColors;		// poèet palet
	head->Format = 1;						// formát - je komprese dat

// pøenesení palet
	MemCopy(head->ColorTab, StdBitmapInfo->bmiColors, StdColors*4);

// pøíprava a komprimace dat obrázku
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

// uložení souboru
	FileWriteBlok(buf, SIZEOFSPRITEFILE + StdColors*4 + 4 + n);

// zrušení datového bufferu
	MemFree(buf);
}


/////////////////////////////////////////////////////////////////////////////
// operátor pøiøazení

const CSprite& CSprite::operator= (const CSprite& src)
{
	Detach();				// zrušení starých dat
	Attach(src.pData);		// pøiøazení nových dat
	return *this;
}


/***************************************************************************\
*																			*
*								Buffer sprajtù								*
*																			*
\***************************************************************************/


////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CBufSprite::CBufSprite()
{
	m_Data = NULL;			// není buffer dat
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není buffer položek
}

CBufSprite::~CBufSprite()
{
	DelAll();				// zrušení všech položek
}


////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

void CBufSprite::Init()
{
	m_Data = NULL;			// není buffer dat
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není buffer položek
}

void CBufSprite::Term()
{
	DelAll();				// zrušení všech položek
}


////////////////////////////////////////////////////////////////////
// vytvoøení nových dat

void CBufSprite::NewData()
{
	m_Max *= 2;
	if (m_Max == 0) m_Max = 0x400;
//	m_Max += 0x400;			// zvýšení poètu položek (o 4 KB)
	MemBuf(m_Data, m_Max);	// zvýšení velikosti bufferu
};


////////////////////////////////////////////////////////////////////
// zrušení všech položek v bufferu

void CBufSprite::DelAll()
{
	Del(m_Num);					// zrušení položek
	MemBuf(m_Data, 0);			// zrušení bufferu dat
	m_Max = 0;					// není žádná položka v bufferu
}

////////////////////////////////////////////////////////////////////
// poskytnutí položky (s kontrolou platnosti indexu)

const CSprite& _fastcall CBufSprite::Get(const int index) const
{
	if (IsValid(index))			// je index platný?
	{
		return m_Data[index];	// položka na daném indexu
	}
	return EmptySprite;			// pro neplatný index vrátí prázdný sprajt
}

////////////////////////////////////////////////////////////////////
// nastavení položky (s kontrolou platnosti indexu)

void _fastcall CBufSprite::Set(const int index, const CSprite& data)
{
	if (IsValid(index))			// je index platný?
	{
		m_Data[index] = data;
	}
}


/////////////////////////////////////////////////////////////////////////////
// vyprázdnìní položky (bez jejího zrušení - jen pro uvolnìní dat)

void _fastcall CBufSprite::Empty(const int index)
{
	if (IsValid(index))					// je index platný?
	{
		m_Data[index].Empty();
	}
}


////////////////////////////////////////////////////////////////////
// zrušení položek z konce bufferu

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
// vytvoøení položky (vrací index položky)

int CBufSprite::New()
{
	int result = NewItem();		// vytvoøení nové položky
	m_Data[result].Init();		// inicializace položky
	return result;
}

int CBufSprite::New(int faze, int smer)
{
	int result = NewItem();		// vytvoøení nové položky
	m_Data[result].Init(faze, smer);	// inicializace položky
	return result;
}

////////////////////////////////////////////////////////////////////
// pøidání položky (vrací index položky)

int _fastcall CBufSprite::Add(const CSprite& data)
{
	int result = NewItem();		// vytvoøení nové položky
	m_Data[result].Init(data.Data());	// inicializace položky
	return result;
}


////////////////////////////////////////////////////////////////////
// duplikace položky (s kontrolou platnosti indexu, vrací index první položky)

int _fastcall CBufSprite::Dup(const int index)
{
	int result = NewItem();		// vytvoøení nové položky

	if (IsValid(index))			// je index platný?
	{
		m_Data[result].Init(m_Data[index].Data());	// kopie položky
	}
	else
	{
		m_Data[result].Init();		// inicializace neplatné položky
	}
	return result;
}

int _fastcall CBufSprite::Dup(const int index, int num)
{
	int result = NewItem();		// vytvoøení nové položky

	if (IsValid(index))					// je index platný?
	{
		SPRITEDATA* data = m_Data[index].Data();

		m_Data[result].Init(data);		// kopie položky

		for (num--; num > 0; num--)
		{
			m_Data[NewItem()].Init(data);
		}
	}
	else
	{
		m_Data[result].Init();		// inicializace neplatné položky

		for (num--; num > 0; num--)
		{
			m_Data[NewItem()].Init();
		}
	}
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// operátor pøiøazení

const CBufSprite& CBufSprite::operator= (const CBufSprite& src)
{
	Del(m_Num);					// zrušení starých dat

	int index = 0;				// index naèítané položky
	int i = src.m_Num;			// velikost zdrojového bufferu

	for (; i > 0; i--)			// pro všechny položky v bufferu
	{
		Add(src[index]);	// kopie položky
		index++;				// inkrementace ètecího indexu
	}
	ASSERT(m_Num == src.m_Num);
	return *this;
}
