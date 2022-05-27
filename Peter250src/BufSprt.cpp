
#include "Main.h"

/***************************************************************************\
*																			*
*									Sprajt									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inicializa�n� pr�zdn� sprajt (modifikuje se po�et referenc�!)

SPRITEDATA*	EmptySpriteData = NULL;
CSprite		const EmptySprite;				// pr�zdn� sprajt


/////////////////////////////////////////////////////////////////////////////
// statick� inicializace sprajt� (p�i chyb� pam�ti vrac� FALSE)

bool InitSprite()
{
	EmptySpriteData = NewSpriteData(1,1);
	return (EmptySpriteData != NULL);
}
	

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� dat sprajtu (p�i chyb� pam�ti vrac� NULL), obr�zky pr�zdn� o velikosti ICONSIZE

SPRITEDATA* _fastcall NewSpriteData(int faze, int smer)
{
	ASSERT((faze > 0) && (smer > 0));

// vytvo�en� dat sprajtu
	SPRITEDATA* data = (SPRITEDATA*)MemGet(faze*smer*sizeof(CPicture) + SIZEOFSPRITEDATA);
	if (data != NULL)
	{

// nastaven� parametr� sprajtu
		data->Refer = 1;				// po�et referenc�
		data->Faze = faze;				// po�et f�z�
		data->Smer = smer;				// po�et sm�r�
		data->Klid = 1;					// po�et klidov�ch f�z�
		data->Delay = SPRITEDEFDELAY;	// prodleva mezi f�zemi (v milisekund�ch)
		data->Level = SPRITEDEFLEVEL;	// hladina k p�ekreslov�n� (0=p�edm�ty)
		data->Kroku = SPRITEDEFKROKU;	// po�et f�z� na jednotkovou vzd�lenost (0=ihned)

// inicializace obr�zk�
		CPicture* pic = data->Data;
		for (int i = faze*smer; i > 0; i--)
		{
			pic->Init();
			pic++;
		}
	}
	return data;
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� dat sprajtu bez inicializace obr�zk� a nastaven� parametr� (p�i chyb� pam�ti vrac� NULL)

SPRITEDATA* _fastcall NewSpriteData0(int faze, int smer)
{
	ASSERT((faze > 0) && (smer > 0));

// vytvo�en� dat sprajtu
	SPRITEDATA* data = (SPRITEDATA*)MemGet(faze*smer*sizeof(CPicture) + SIZEOFSPRITEDATA);
	if (data != NULL)
	{

// nastaven� parametr� sprajtu
		data->Refer = 1;				// po�et referenc�
		data->Faze = faze;				// po�et f�z�
		data->Smer = smer;				// po�et sm�r�
	}
	return data;
}


/////////////////////////////////////////////////////////////////////////////
// zru�en� dat sprajtu (odd�leno kv�li lep�� optimalizaci)

void _fastcall DelSpriteData(SPRITEDATA* data)
{
	ASSERT(data != NULL);

// ukon�en� obr�zk�
	CPicture* pic = data->Data;
	for (int i = data->Faze*data->Smer; i > 0; i--)
	{
		pic->Term();
		pic++;
	}

// zru�en� dat sprajtu
	MemFree(data);
}


/////////////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CSprite::CSprite() 
{ 
	attach(EmptySpriteData); 
};

CSprite::CSprite(const CSprite& src) 
{ 
	attach(src.pData); 
};

CSprite::CSprite(SPRITEDATA* data)
{ 
	attach(data); 
};

CSprite::~CSprite() 
{ 
	detach(pData); 
};


/////////////////////////////////////////////////////////////////////////////
// statick� konstruktor a destruktor

void CSprite::Init()
{ 
	attach(EmptySpriteData); 
};

void _fastcall CSprite::Init(const CSprite& src) 
{ 
	attach(src.pData); 
};

void _fastcall CSprite::Init(SPRITEDATA* data)
{ 
	attach(data); 
};

bool _fastcall CSprite::Init(int faze, int smer)	// p�i chyb� pam�ti vr�t� FALSE, sprajt nen� vytvo�en
{
	pData = NewSpriteData(faze, smer);
	return (pData != NULL);
};

void CSprite::Term()
{ 
	detach(pData); 
};


/////////////////////////////////////////////////////////////////////////////
// vypr�zdn�n� sprajtu (uvoln�n� dat) - p�ipoj� se standardn� pr�zdn� sprajt o velikosti 1x1

void CSprite::Empty()
{ 
	Term();
	Init();
}


/////////////////////////////////////////////////////////////////////////////
// kopie do vlastn�ho bufferu p�ed modifikac� (pozor - obr�zky t�mto nejsou p�ivlastn�ny!)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool CSprite::CopyWrite()
{
// �schova ukazatel�
	SPRITEDATA* olddata = pData;		// adresa star�ch dat
	long* refer = &(olddata->Refer);	// po�et referenc�

// test, zda je nutn� p�ivlastn�n�
	if (*refer > 1)					// je n�jak� jin� majitel?
	{

// vytvo�en� nov�ho bufferu (bez inicializace obr�zk�)
		SPRITEDATA* newdata = NewSpriteData0(olddata->Faze, olddata->Smer);
		if (newdata == NULL) return false;

// kopie obr�zk�
		CPicture* dst = newdata->Data;
		CPicture* src = olddata->Data;
		for (int i = olddata->Faze * olddata->Smer; i > 0; i--)
		{
			dst->Init(src->Data());
			dst++;
			src++;
		}

// p�enesen� parametr�
		newdata->Klid = olddata->Klid;
		newdata->Delay = olddata->Delay;
		newdata->Level = olddata->Level;
		newdata->Kroku = olddata->Kroku;

// odpojen� star�ch dat
		detach(olddata);

// p�ipojen� nov�ch dat
		pData = newdata;
	}

// operace OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� nov�ho sprajtu (p�ipraveno k z�pisu, data jsou n�hodn�)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool _fastcall CSprite::New(int faze, int smer)
{
// ukazatel na star� data
	SPRITEDATA* olddata = pData;

// test, zda je nutn� vytvo�en� nov�ho bufferu
	if ((olddata->Refer > 1) ||
		(olddata->Faze != faze) ||
		(olddata->Smer != smer))
	{

// vytvo�en� nov�ho bufferu (ikony i parametry inicializov�ny)
		SPRITEDATA* newdata = NewSpriteData(faze, smer);
		if (newdata == NULL) return false;

// odpojen� star�ch dat
		detach(olddata);

// p�ipojen� nov�ch dat
		pData = newdata;
	}

// operace OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� nov�ch rozm�r� sprajtu (po�et f�z� pro klid a pohyb, po�et sm�r�)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool CSprite::ReSize(int klid, int pohyb, int smer)
{
// korekce hodnot
	if (klid < 1) klid = 1;
	if (klid > SPRITEMAXKLID) klid = SPRITEMAXKLID;
	if (pohyb < 0) pohyb = 0;
	if (pohyb > SPRITEMAXPOHYB) pohyb = SPRITEMAXPOHYB;
	if (smer < 1) smer = 1;
	if (smer > SPRITEMAXSMER) smer = SPRITEMAXSMER;

// �schova star�ch parametr�
	SPRITEDATA* olddata = pData;
	int oldfaze = olddata->Faze;
	int oldsmer = olddata->Smer;
	int oldklid = olddata->Klid;
	int oldpohyb = oldfaze - oldklid;

// kontrola, zda se parametry m�n�
	if ((klid == oldklid) &&
		(pohyb == oldpohyb) &&
		(smer == oldsmer))
	{
		return true;
	}

// vytvo�en� nov�ho bufferu (ikony neinicializov�ny)
	int faze = klid + pohyb;
	SPRITEDATA* newdata = NewSpriteData0(faze, smer);
	if (newdata == NULL) return false;

// nastaven� parametr�
	newdata->Klid = klid;
	newdata->Delay = olddata->Delay;
	newdata->Level = olddata->Level;
	newdata->Kroku = olddata->Kroku;

// ukazatele dat
	CPicture* src = olddata->Data;
	CPicture* dst = newdata->Data;

// p�enesen� dat (cyklus po nov�ch ��dc�ch)
	for (int i = 0; i < smer; i++)
	{

// index star�ho ��dku (sm�ru)
		int oldi = (i*oldsmer + (smer-1)/2) / smer;
		if (oldi >= oldsmer) oldi = oldsmer-1;

// offset za��tku star�ho a nov�ho ��dku
		int off = i*faze;
		int oldoff = oldi*oldfaze;

// p�enesen� dat v klidu (cyklus po nov�ch klidov�ch pozic�ch)
		for (int j = 0; j < klid; j++)
		{
//			int oldj = (j*oldklid + (klid-1)/2) / klid;
			int oldj = j - (j/oldklid)*oldklid;
//			if (oldj >= oldklid) oldj = oldklid-1;
			dst[off].Init(src[oldoff + oldj].Data());
			off++;
		}

// p�enesen� dat v pohybu (cyklus po nov�ch pozic�ch v pohybu)
		for (j = 0; j < pohyb; j++)
		{
			int oldj;
			if (oldpohyb > 0)
			{
//				oldj = (j*oldpohyb + (pohyb-1)/2) / pohyb + oldklid;
				oldj = j - (j/oldpohyb)*oldpohyb + oldklid;
			}
			else
			{
//				oldj = (j*oldklid + (pohyb-1)/2) / pohyb;
				oldj = j - (j/oldklid)*oldklid;
			}
//			if (oldj >= oldfaze) oldj = oldfaze-1;

			dst[off].Init(src[oldoff + oldj].Data());
			off++;
		}
	}

// odpojen� star�ch dat
	detach(olddata);

// p�ipojen� nov�ch dat
	pData = newdata;

// operace OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// poskytnut� prvku (s kontrolou platnosti offsetu/indexu)
// pro neplatn� prvek vrac� p��stup na prvek pr�zdn�ho sprajtu

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
// na�ten� sprajtu ze souboru
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool CSprite::LoadFile(CText jmeno)
{
// otev�en� souboru mapovan�ho do pam�ti (uzav�en p�i destrukci!)
	CFileMap file;
	if (!file.Open(jmeno)) return false;

// kontrola velikosti souboru
	int size = file.Size();
	if (size < 100) return false;

// na�ten� z�kladn�ch parametr�
	SPRITEFILE* spr = (SPRITEFILE*)file.Adr();
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
	if ((spr->Ident[0] != 'P') ||
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
		return false;
	}

// p��prava konverzn� tabulky palet
	BITMAPINFO* bmp = (BITMAPINFO*)MemGet(sizeof(BITMAPINFO) + pals*4 + 200);
	if (bmp == NULL) return false;
	bmp->bmiHeader.biClrUsed = pals;
	bmp->bmiHeader.biBitCount = 8;
	MemCopy(bmp->bmiColors, spr->ColorTab, pals*4);
	bmp->bmiColors[0] = StdBitmapInfo->bmiColors[0];

// barva 1 bude jako st�n (krom� importu ze star�� verze)
	if (pals >= StdColors)
	{
		bmp->bmiColors[1] = StdBitmapInfo->bmiColors[1];
	}

	GenKonvPal(bmp);
	MemFree(bmp);

// vytvo�en� sprajtu a nastaven� parametr�
	SPRITEDATA* newdata = NewSpriteData(faze, smer);
	if (newdata == NULL) return false;
	newdata->Klid = klid;			// po�et klidov�ch f�z�
	newdata->Delay = delay;		// prodleva mezi f�zemi
	newdata->Level = level;		// hladina k p�ekreslov�n�
	newdata->Kroku = kroku;		// po�et f�z� na jednotkovou vzd�lenost

// dekomprimace bufferu
	BYTE* src = file.Adr() + SIZEOFSPRITEFILE + pals*4;
	BYTE* buf = NULL;
	if (format == 1)
	{
		buf = (BYTE*)MemGet(faze*smer*width*height + 1000);
		if (buf == NULL)
		{
			DelSpriteData(newdata);
			return false;
		}
		DeCompress(buf, src + 4, faze*smer*width*height, width);
		src = buf;
	}

// na�ten� obr�zk�
	CPicture* dst = newdata->Data;
	for (int i = faze*smer; i > 0; i--)
	{
		if (!dst->New(width, height) ||	!dst->CopyKonvData(src) || !dst->Comp())
		{
			DelSpriteData(newdata);
			MemFree(buf);
			return false;
		}
		dst++;
		src += width*height;
	}

// zru�en� p��padn�ho bufferu
	MemFree(buf);

// odpojen� star�ch dat
	detach(pData);

// p�ipojen� nov�ch dat
	pData = newdata;

// p��znak - na�teno OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// ulo�en� do souboru (FALSE=chyba)

bool CSprite::SaveFile(CText jmeno) const
{
// vytvo�en� souboru
	CFile file;
	file.Name(jmeno);
	if (!file.Create()) return false;

// p��prava velikosti souboru
	int width = pData->Data[0].Width();
	int height = pData->Data[0].Height();
	int icons = pData->Faze * pData->Smer;

// buffer k ulo�en� souboru
	BYTE* buf = (BYTE*)MemGet(SIZEOFSPRITEFILE + StdColors*4 + 4 + icons*width*height*3/2);
	if (buf == NULL)
	{
		file.Close();
		file.Delete();
		return false;
	}

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
	if (buf0 == NULL)
	{
		MemFree(buf);
		file.Close();
		file.Delete();
		return false;
	}

	BYTE* dst = buf0;
	for (int i = 0; i < icons; i++)
	{
		if (!pData->Data[i].DeComp())
		{
			MemFree(buf0);
			MemFree(buf);
			file.Close();
			file.Delete();
			return false;
		}
		MemCopy(dst, pData->Data[i].DataData(), width*height);
		dst += width*height;
	}

	int n = Compress(buf + SIZEOFSPRITEFILE + StdColors*4 + 4, buf0, width*height*icons, width);
	*(int*)(buf + SIZEOFSPRITEFILE + StdColors*4) = n;
	MemFree(buf0);

// ulo�en� souboru
	BOOL result = file.Write(buf, SIZEOFSPRITEFILE + StdColors*4 + 4 + n);

// uzav�en� souboru
	file.Close();

// zru�en� datov�ho bufferu
	MemFree(buf);

// p�i chyb� zru�en� souboru
	if (!result)
	{
		file.Delete();
		return false;
	}

// p��znak - ulo�eno OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// oper�tor p�i�azen�

const CSprite& CSprite::operator= (const CSprite& src)
{
	detach(pData);				// zru�en� star�ch dat
	attach(src.pData);		// p�i�azen� nov�ch dat
	return *this;
}

const CSprite& CSprite::operator= (SPRITEDATA* src)
{
	detach(pData);				// zru�en� star�ch dat
	attach(src);				// p�i�azen� nov�ch dat
	return *this;
}

/***************************************************************************\
*																			*
*								Buffer sprajt�								*
*																			*
\***************************************************************************/

////////////////////////////////////////////////////////////////////
// p�id�n� z�znamu o vlo�en� polo�ky do UNDO bufferu

template <> bool CBuffer<CSprite>::UndoAddIns(int index)
{
	return Undo.AddSprIns(index);
}

////////////////////////////////////////////////////////////////////
// vytvo�en� nov�ho sprajtu (vrac� index polo�ky, <0 = chyba), data jsou n�hodn�
// prov�d� z�znam do UNDO bufferu

int _fastcall CBufSprite::New(int faze, int smer)
{
// vytvo�en� nov� polo�ky
	int result = New();
	if (result >= 0)
	{

// inicializace objektu
		if (!m_Data[result].New(faze, smer))
		{
			Del(result);
			if (m_Undo) Undo.DelAll();
			result = -1;
		}
	}

// index nov� polo�ky (nebo <0 = chyba)
	return result;
}
