
#include "Main.h"

/***************************************************************************\
*																			*
*								Obrázkové promìnné							*
*																			*
\***************************************************************************/

////////////////////////////////////////////////////////////////////
// Pozn.: linky souboru BMP pro 256 barev jsou zarovnávány na 4 bajty
//        linky souboru BMP pro 2 barvy (maska) jsou zarovnány na 2 bajty
////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// inicializaèní prázdný obrázek (modifikuje se poèet referencí!)

// Prázdný obrázek musí mít rozmìry ICONSIZE kvùli vytvoøení nového sprajtu!
PICTUREDATA	EmptyPictureData		= { 1, ICONWIDTH, ICONHEIGHT, PicParamBack, 0, 0, 0, NULL };
const CPicture	EmptyPicture;				// prázdný obrázek

/////////////////////////////////////////////////////////////////////////////
// statická inicializace obrázkù (pøi chybì pamìti vrací FALSE)

bool InitPicture()
{
// prázdný obrázek
	EmptyPictureData.Data = (BYTE*)MemGet(ICONSIZE);
	if (EmptyPictureData.Data == NULL) return false;
	MemFill(EmptyPictureData.Data, ICONSIZE, (char)(BYTE)BackCol);
	return true;
}

////////////////////////////////////////////////////////////////////
// vytvoøení dat obrázku (pøi chybì pamìti vrací NULL)

PICTUREDATA* _fastcall NewPictureData(int width, int height)
{
	ASSERT((width > 0) && (height > 0));

// vytvoøení záhlaví obrázku
	PICTUREDATA* data = (PICTUREDATA*)MemGet(SIZEOFPICTUREDATA); // vytvoøení záhlaví
	if (data != NULL)
	{

// nastavení dat obrázku
		data->Refer = 1;					// poèet referencí
		data->Width = width;				// šíøka
		data->Height = height;				// výška
		data->Param = PicParamNone;			// parametry (obsah neznámý)

// vytvoøení bufferu dat obrázku
		BYTE* datadata = (BYTE*)MemGet(width*height);
		data->Data = datadata;				// adresa dat

// pøi chybì pamìti zrušení záhlaví obrázku
		if (datadata == NULL)
		{
			MemFree(data);
			data = NULL;
		}
	}
	return data;
}

/////////////////////////////////////////////////////////////////////////////
// zrušení dat obrázku (oddìleno kvùli lepší optimalizaci)

void _fastcall DelPictureData(PICTUREDATA* data)
{
	ASSERT(data != NULL);
	ASSERT(data->Data != NULL);

	MemFree(data->Data);		// zrušení dat obrázku
	MemFree(data);				// zrušení záhlaví obrázku
}


/////////////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CPicture::CPicture() 
{ 
	attach(&EmptyPictureData); 
};

CPicture::CPicture(const CPicture& src) 
{ 
	attach(src.pData); 
};

CPicture::CPicture(PICTUREDATA* data) 
{ 
	attach(data); 
};

CPicture::~CPicture() 
{ 
	detach(pData); 
};


/////////////////////////////////////////////////////////////////////////////
// statický konstruktor a destruktor

void CPicture::Init()
{ 
	attach(&EmptyPictureData); 
};

void _fastcall CPicture::Init(const CPicture& src) 
{ 
	attach(src.pData); 
};

void _fastcall CPicture::Init(PICTUREDATA* data)
{ 
	attach(data); 
};

bool _fastcall CPicture::Init(int width, int height) // pøi chybì pamìti vrací FALSE, obrázek není vytvoøen
{
	pData = NewPictureData(width, height);
	return (pData != NULL);
};

void CPicture::Term()
{ 
	detach(pData); 
};


/////////////////////////////////////////////////////////////////////////////
// vyprázdnìní obrázku (uvolnìní dat)

void CPicture::Empty()
{ 
	Term();
	Init();
}


////////////////////////////////////////////////////////////////////
// vymazání obsahu obrázku (naplnìní prùhlednou barvou), zajistí pøivlastnìní (a dekomprimaci) bufferu,
// pøi chybì pamìti vrací FALSE, pùvodní obsah nezmìnìn

bool CPicture::Clear()
{
// vytvoøení nového bufferu, je-li potøeba
	if (!New()) return false;

// vymazání bufferu
	MemFill(pData->Data, pData->Width * pData->Height, (char)(BYTE)BackCol);
	
// nastavení parametrù na pozadí	
	pData->Param = PicParamBack;
	return true;
}


////////////////////////////////////////////////////////////////////
// vymazání obsahu obrázku s nastavením velikosti (naplnìní prùhlednou barvou), zajistí pøivlastnìní (a dekomprimaci) bufferu,
// pøi chybì pamìti vrací FALSE, pùvodní obsah nezmìnìn

bool _fastcall CPicture::Clear(int width, int height)
{
// vytvoøení nového bufferu, je-li potøeba
	if (!New(width, height)) return false;

// vymazání bufferu
	MemFill(pData->Data, pData->Width * pData->Height, (char)(BYTE)BackCol);
	
// nastavení parametrù na pozadí	
	pData->Param = PicParamBack;
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// kopie obrázku do vlastního bufferu pøed modifikací (komprimovaná data zùstanou komprimovaná)
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn

bool CPicture::CopyWrite()
{
// úschova ukazatelù
	PICTUREDATA* olddata = pData;	// adresa starých dat
	long* refer = &(olddata->Refer);// poèet referencí

// test, zda je nutné pøivlastnìní
	if (*refer > 1)					// je nìjaký jiný majitel?
	{

// vytvoøení nového bufferu
		int size;
		PICTUREDATA* newdata;
		int width = olddata->Width;
		int height = olddata->Height;

		if (olddata->Param == PicParamComp)
		{
			size = *(long*)(olddata->Data) + 4;
			newdata = NewPictureData(size, 1);
			if (newdata == NULL) return false;
			newdata->Width = width;
			newdata->Height = height;
		}
		else
		{
			size = width * height;
			newdata = NewPictureData(width, height);
			if (newdata == NULL) return false;
		}

// pøenesení dat
		MemCopy(newdata->Data, olddata->Data, size);
		newdata->Param = olddata->Param;

// odpojení starých dat
		detach(olddata);

// pøipojení nových dat
		pData = newdata;
	}

// operace OK
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// kopie výøezu do obrázku (se zprùhlednìním) - nekontroluje hranice!

void CPicture::AddCopy(const CPicture& src, int x, int y)
{
	if (!CopyWrite()) return;

	BYTE* s = src.DataData();

	for (int h = src.Height(); h > 0; h--)
	{
		BYTE* d = DataData() + y*Width() + x;

		for (int w = src.Width(); w > 0; w--)
		{
			BYTE col = *s;
			if (col != BackCol) *d = col;

			s++;
			d++;
		}

		y++;
	}
}

void CPicture::AddCopy(int id, int x, int y)
{
	if (!CopyWrite()) return;

	CPicture src;
	if (!src.Load(id)) return;

	BYTE* s = src.DataData();

	for (int h = src.Height(); h > 0; h--)
	{
		BYTE* d = DataData() + y*Width() + x;

		for (int w = src.Width(); w > 0; w--)
		{
			BYTE col = *s;
			if (col != BackCol) *d = col;

			s++;
			d++;
		}

		y++;
	}
}

void CPicture::AddCopy(const CPicture& src, int x, int y, int w, int h, int xs, int ys)
{
	if (!CopyWrite()) return;

	for (; h > 0; h--)
	{
		BYTE* s = src.DataData() + ys*src.Width() + xs;
		BYTE* d = DataData() + y*Width() + x;

		for (int i = w; i > 0; i--)
		{
			BYTE col = *s;
			if (col != BackCol) *d = col;

			s++;
			d++;
		}

		y++;
		ys++;
	}
}

/////////////////////////////////////////////////////////////////////////////
// vytvoøení nového obrázku se stejnou velikostí (pøipraveno k zápisu, data jsou náhodná)
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn

bool CPicture::New()
{		 
// ukazatel na stará data
	PICTUREDATA* olddata = pData;			// adresa starých dat

// test, zda je nutné vytvoøení nového bufferu
	if ((olddata->Refer > 1) ||				// na buffer je více referencí
		(olddata->Param == PicParamComp))	// data jsou komprimovaná
	{

// vytvoøení nového bufferu
		PICTUREDATA* newdata = NewPictureData(pData->Width, pData->Height);
		if (newdata == NULL) return false;

// odpojení starých dat
		detach(olddata);

// pøipojení nových dat
		pData = newdata;
	}

// operace OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// vytvoøení nového obrázku (pøipraveno k zápisu, data jsou náhodná)
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn

bool _fastcall CPicture::New(int width, int height)
{		 
// ukazatel na stará data
	PICTUREDATA* olddata = pData;			// adresa starých dat

// test, zda je nutné vytvoøení nového bufferu
	if ((olddata->Refer > 1) ||				// na buffer je více referencí
		(olddata->Width != width) ||		// nesouhlasí šíøka
		(olddata->Height != height) ||		// nesouhlasí výška
		(olddata->Param == PicParamComp))	// data jsou komprimovaná
	{

// vytvoøení nového bufferu
		PICTUREDATA* newdata = NewPictureData(width, height);
		if (newdata == NULL) return false;

// odpojení starých dat
		detach(olddata);

// pøipojení nových dat
		pData = newdata;
	}

// operace OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// nastavení nových rozmìrù obrázku (nová data jsou vymazána)
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn

bool CPicture::Resize(int width, int height)
{
// zajištìní dekomprimace
	if (!DeComp()) return false;

// pøivlastnìní bufferu
	if (!CopyWrite()) return false;

// úschova starých rozmìrù
	int oldwidth = pData->Width;
	int oldheight = pData->Height;

// test, zda je potøeba velikost obrázku mìnit
	if ((width != oldwidth) || (height != oldheight))
	{

// vytvoøení nového bufferu
		PICTUREDATA* olddata = pData;
		PICTUREDATA* newdata = NewPictureData(width, height);
		if (newdata == NULL) return false;

// pøíprava poètu linek ke kopii
		int i = height;
		if (oldheight < i) i = oldheight;

// pøíprava délky linky ke kopii
		int j = width;
		if (oldwidth < j) j = oldwidth;

// pøíprava zbytku linky k vymazání
		int k = width - j;

// pøíprava zdrojové a cílové adresy
		BYTE* src = olddata->Data;
		BYTE* dst = newdata->Data;

// kopie platných linek pøi shodì délek linek
		if (width == oldwidth)
		{
			i *= width;
			MemCopy(dst, src, i);
			dst += i;
			src += i;
		}

// kopie platných linek pøi rozdílné délce linek
		else
		{
			if (k == 0)
			{
				for (; i > 0; i--)
				{
					MemCopy(dst, src, j);
					dst += width;
					src += oldwidth;
				}
			}
			else
			{
				for (; i > 0; i--)
				{
					MemCopy(dst, src, j);
					dst += j;
					MemFill(dst, k, (char)(BYTE)BackCol);
					dst += k;
					src += oldwidth;
				}
			}
		}

// vymazání zbylých linek
		i = height - oldheight;
		if (i > 0)
		{
			MemFill(dst, i*width, (char)(BYTE)BackCol);
		}

// odpojení starých dat
		detach(olddata);

// pøipojení nových dat
		pData = newdata;
	}

// operace OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// poskytnutí bodu (s kontrolou platnosti offsetu/indexu), obrázek nesmí být komprimovaný!
// pro neplatný bod vrátí barvu pozadí)

BYTE _fastcall CPicture::Get(const int off) const
{
	ASSERT(pData->Param != PicParamComp);

	if (IsValid(off))				// je offset platný?
	{
		return pData->Data[off];	// bod na daném offsetu
	}
	return (BYTE)BackCol;					// neplatný offset
}

BYTE _fastcall CPicture::Get(const int x, const int y) const
{
	ASSERT(pData->Param != PicParamComp);

	if (IsValid(x, y))				// je index platný?
	{
		return pData->Data[x + y*pData->Width];	// prvek
	}
	return (BYTE)BackCol;					// neplatný offset
}


////////////////////////////////////////////////////////////////////
// nastavení bodu (s kontrolou platnosti offsetu/indexu), obrázek nesmí být komprimovaný!
// pøed zápisem je nutno zajistit pøivlastnìní bufferu!

void _fastcall CPicture::Set(const int off, const BYTE data)
{
	ASSERT(pData->Param != PicParamComp);
	ASSERT(pData->Refer == 1);

	if (IsValid(off))				// je offset platný?
	{
		pData->Data[off] = data;
	}
}

void _fastcall CPicture::Set(const int x, const int y, const BYTE data)
{
	ASSERT(pData->Param != PicParamComp);
	ASSERT(pData->Refer == 1);

	if (IsValid(x, y))				// je index platný?
	{
		pData->Data[x + y*pData->Width] = data;
	}
}


/////////////////////////////////////////////////////////////////////////////
// komprese dat obrázku, vrací velikost dat (pøi chybì pamìti vrací <0, data jsou nezmìnìna)
// komprese se provádí ve spoleèném bufferu pro všechny promìnné!
// (v pøípadì potøeby je nutno zajistit pøivlastnìní bufferu)
// pùvodní buffer s daty je zrušen
// (velikost komprimovaných dat je udávána bez dvojslova s velikostí, buffer s daty
// je tedy o 4 vìtší, data zaèínají na offsetu 4 v bufferu)

int CPicture::Comp() const
{
// test, zda je obrázek již komprimován
	if (pData->Param == PicParamComp)
	{
		return *(long*)pData->Data;
	}

// velikost dat obrázku
	int size = pData->Width * pData->Height;

// vytvoøení nového bufferu pro data
	BYTE* newdata = (BYTE*)MemGet(2*size + 200);
	if (newdata == NULL) return -1;

// komprese dat
	BYTE* olddata = pData->Data;
	int newsize = Compress(newdata + 4, olddata, size, pData->Width);
	*(long*)newdata = newsize;

// vytvoøení bufferu se správnou velikostí (MemSize() by blok nezmenšil)
	BYTE* newdata2 = (BYTE*)MemGet(newsize + 4);
	if (newdata2 == NULL)
	{
		MemFree(newdata);
		return -1;
	}

// kopie dat do nového bufferu
	MemCopy(newdata2, newdata, newsize+4);
	MemFree(newdata);

// zrušení starého bufferu
	MemFree(olddata);

// nastavení parametrù
	pData->Data = newdata2;
	pData->Param = PicParamComp;

// nová velikost dat
	return newsize;
}


/////////////////////////////////////////////////////////////////////////////
// dekomprimace dat obrázku (jsou-li komprimována), pøi chybì vrací FALSE
// dekomprese se provádí ve spoleèném bufferu pro všechny promìnné
// (v pøípadì potøeby je nutno zajistit pøivlastnìní bufferu)
// pùvodní buffer s komprimovanými daty je zrušen

bool CPicture::DeComp() const
{
// test, zda je obrázek komprimován
	if (pData->Param == PicParamComp)
	{

// vytvoøení nového bufferu pro data
		int size = pData->Width * pData->Height;
		BYTE* newdata = (BYTE*)MemGet(size);
		if (newdata == NULL) return false;

// dekomprese dat
		BYTE* olddata = pData->Data;
		DeCompress(newdata, olddata + 4, size, pData->Width);

// zrušení starého bufferu
		MemFree(olddata);

// nastavení parametrù
		pData->Data = newdata;
		pData->Param = PicParamNone;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// kopie nových dat obrázku (rozmìry zùstanou nezmìnìny) - zajistí odpojení dat
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn

bool CPicture::CopyData(BYTE* src)
{
// vytvoøení nového bufferu, je-li potøeba
	if (!New()) return false;

// kopie dat do bufferu
	MemCopy(pData->Data, src, pData->Width * pData->Height);
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// kopie komprimovaných nových dat obrázku (rozmìry zùstanou nezmìnìny) - zajistí odpojení dat
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn

bool CPicture::CompCopy(BYTE* src)
{
// úschova parametrù
	int width = pData->Width;
	int height = pData->Height;
	int size = *(long*)src + 4;

// vytvoøení nového bufferu
	if (!New(size, 1)) return false;

// nastavení parametrù
	pData->Width = width;
	pData->Height = height;
	pData->Param = PicParamComp;

// kopie dat do bufferu
	MemCopy(pData->Data, src, size);
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// kopie nových dat obrázku s konverzí (rozmìry zùstanou nezmìnìny) - zajistí odpojení dat
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn

bool CPicture::CopyKonvData(BYTE* src)
{
// vytvoøení nového bufferu, je-li potøeba
	if (!New()) return false;

// kopie dat do bufferu
	KonvCopy(pData->Data, src, pData->Width * pData->Height);
	return true;
}


////////////////////////////////////////////////////////////////////
// naètení obrázku z resource

bool CPicture::Load(const int nID)
{
// nalezení resource bitmapy
	CResource res;
	if (!res.Open(nID, RT_BITMAP)) { ASSERTERROR; return false; }

// adresa bitmapy
	BITMAPINFO* bmp = (BITMAPINFO*)res.Adr();
	ASSERT(bmp->bmiHeader.biBitCount == 8);
	int colors = bmp->bmiHeader.biClrUsed;
	if (colors == 0) colors = 256;
	ASSERT((DWORD)colors <= 256);

// pøíprava pøírustku zdrojové adresy mezi linkami
	int srcinc = (bmp->bmiHeader.biWidth + 3) & ~3;

// nová data obrázku
	New(bmp->bmiHeader.biWidth, bmp->bmiHeader.biHeight);

// dekomprese bitmapy
	BITMAPINFO*	bmp2 = NULL; // pomocný buffer pro dekompresi bitmapy
	if (bmp->bmiHeader.biCompression == BI_RLE8)
	{
		int sizehead = sizeof(BITMAPINFO) + (colors-1)*sizeof(RGBQUAD);
		bmp2 = (BITMAPINFO*)MemGet(sizehead + srcinc*bmp->bmiHeader.biHeight);
		if (bmp2 == NULL) return false;
		MemCopy(bmp2, bmp, sizehead);
		DekompRLE8((BYTE*)(bmp2->bmiColors + colors), srcinc*bmp->bmiHeader.biHeight, 
					(BYTE*)(bmp->bmiColors + colors), res.Size() - sizehead); 
		bmp = bmp2;
	}

	BYTE* src = (BYTE*)(bmp->bmiColors + colors);
	BYTE* dst = pData->Data;

	for (int i = bmp->bmiHeader.biHeight; i > 0; i--)
	{
		MemCopy(dst, src, bmp->bmiHeader.biWidth);
		src += srcinc;
		dst += bmp->bmiHeader.biWidth;
	}


// pøípadné zrušení bufferu
	MemFree(bmp2);

	return true;
}


/////////////////////////////////////////////////////////////////////////////
// naètení obrázku ze souboru (FALSE=chyba, obsah nezmìnìn)
/*
bool CPicture::LoadFile(CText jmeno)
{
// otevøení souboru mapovaného do pamìti (uzavøen pøi destrukci!)
	CFileMap file;
	if (!file.Open(jmeno)) return false;

// velikost souboru bez záhlaví 14 bajtù
	int size = file.Size() - 14;

// identifikaèní záhlaví
	char* hd = (char*)file.Adr();
	BITMAPINFO* bmp = (BITMAPINFO*)(file.Adr() + 14);
	
// kontrola velikosti souboru a záhlaví
	if ((size < 50) || 
		(hd[0] != 'B') || 
		(hd[1] != 'M') ||
		!TestBMP(bmp))
	{
		return false;
	}

// pøíprava parametrù bitmapy
	int width = bmp->bmiHeader.biWidth;			// šíøka
	int height = bmp->bmiHeader.biHeight;		// výška
	int bits = bmp->bmiHeader.biBitCount;		// poèet bitù na bod
	int colors = bmp->bmiHeader.biClrUsed;		// poèet použitých barev
	if (colors <= 0) colors = (1 << bits);		// implicitní poèet barev
	if (bits > 8) colors = 0;					// pro TRUE COLOR nejsou palety
	int sizehead = sizeof(BITMAPINFOHEADER) + colors*sizeof(RGBQUAD); // velikost záhlaví
	size -= sizehead;							// oprava velikosti dat
	if (size < 0) size = 0;

// dekomprese bitmapy RLE8
	BITMAPINFO* bmp2 = NULL;

	if (bmp->bmiHeader.biCompression == BI_RLE8)
	{
		int newsize = ((width + 3) & ~3) * height;
		bmp2 = (BITMAPINFO*)MemGet(sizehead + newsize + 10000);
		if (bmp2 == NULL) return false;
		MemCopy(bmp2, bmp, sizehead);
		bmp2->bmiHeader.biCompression = BI_RGB;
		DekompRLE8((BYTE*)bmp2+sizehead, newsize, (BYTE*)bmp+sizehead, size);
		bmp = bmp2;
		size = newsize;
	}

// dekomprese bitmapy RLE4
	if (bmp->bmiHeader.biCompression == BI_RLE4)
	{
		int newsize = (((width+1)/2 + 3) & ~3) * height;
		bmp2 = (BITMAPINFO*)MemGet(sizehead + newsize + 10000);
		if (bmp2 == NULL) return false;
		MemCopy(bmp2, bmp, sizehead);
		bmp2->bmiHeader.biCompression = BI_RGB;
		DekompRLE4((BYTE*)bmp2+sizehead, newsize, (BYTE*)bmp+sizehead, size);
		bmp = bmp2;
		size = newsize;
	}

// vytvoøení nového bufferu
	PICTUREDATA* newdata = NewPictureData(width, height);
	if (newdata == NULL)
	{
		MemFree(bmp2);
		return false;
	}

// vygenerování konverzní tabulky palet
//	if (bits <= 8)
//	{
//		GenKonvPal(bmp);
//	}

// pøíprava bufferu odchylky pro dithering
	int* odch = NULL;
	if (Dither)
	{
		odch = (int*)MemGet((3*width + 6) * sizeof(int));
		MemFill(odch, (3*width + 6) * sizeof(int), 0);
	}

// pøíprava parametrù ke konverzi
	BYTE* dst = newdata->Data;				// ukládací adresa
	BYTE* src = (BYTE*)bmp + sizehead;		// ètecí adresa
	int srcinc;								// pøírustek zdrojové adresy
	int i, j;								// pracovní èítaèe
	BYTE r, g, b;							// barevné složky
	WORD srcdat;							// zdrojová data 16 bitù

// rozlišení podle poètu bodù
	switch (bits)
	{

// 1 bit
	case 1:
		srcinc = ((width+7)/8 + 1) & ~1;	// pøírustek zdrojové adresy

		for (i = height; i > 0; i--)		// cyklus pøes všechny linky
		{
			for (j = 0; j < width; j++)
			{
				*dst = KonvPal[(src[j/8] >> (7 - (j & 7))) & 1];
				dst++;
			}
			src += srcinc;					// zvýšení zdrojové adresy
		}
		break;

// 4 bity
	case 4:
		srcinc = ((width+1)/2 + 3) & ~3;	// pøírustek zdrojové adresy

		for (i = height; i > 0; i--)		// cyklus pøes všechny linky
		{
			for (j = 0; j < width; j++)
			{
				if (j & 1)
				{
					*dst = KonvPal[src[j/2] & 0xf];
				}
				else
				{
					*dst = KonvPal[src[j/2] >> 4];
				}
				dst++;
			}
			src += srcinc;					// zvýšení zdrojové adresy
		}
		break;

// 8 bitù
	case 8:
		srcinc = ((width + 3) & ~3) - width; // pøírustek zdrojové adresy

		for (i = height; i > 0; i--)		// cyklus pøes všechny linky
		{
			if (Dither)
			{
				int* odch0 = odch + 3;			// ukazatel v bufferu odchylek

				for (j = width; j > 0; j--)		// cyklus pøes všechny body
				{
				// bod k zápisu
					BYTE col = *src;
					src++;
					if (col >= colors) col = 0;
					RGBQUAD* rgb = bmp->bmiColors + col;

				// pozadí
					if ((*(int*)rgb & 0xffffff) == (BACKCOLOR_BLUE | (BACKCOLOR_GREEN*256) | (BACKCOLOR_RED*256*256)))
					{
						col = BackCol;

						*odch0 = 0;
						odch0++;
						*odch0 = 0;
						odch0++;
						*odch0 = 0;
						odch0++;
					}
					else
					{	

				// stín
					  if ((*(int*)rgb & 0xffffff) == (SHADCOLOR_BLUE | (SHADCOLOR_GREEN*256) | (SHADCOLOR_RED*256*256)))
					  {
						col = ShadCol;

						*odch0 = 0;
						odch0++;
						*odch0 = 0;
						odch0++;
						*odch0 = 0;
						odch0++;
					  }
					  else
					  {	

				// požadovaná barva
						b = rgb->rgbBlue;			// modrá složka
						g = rgb->rgbGreen;			// zelená složka
						r = rgb->rgbRed;			// èervená složka

				// zkorigovaná barva
						int b2 = b - (odch0[-3] + odch0[0] + odch0[3])*5/8;		// modrá složka
						int g2 = g - (odch0[-2] + odch0[1] + odch0[4])*5/8;		// zelená složka
						int r2 = r - (odch0[-1] + odch0[2] + odch0[5])*5/8;		// èervená složka

				// omezení pøeteèení barvy
						if (b2 < 0) b2 = 0;
						if (b2 > 255) b2 = 255;
						if (g2 < 0) g2 = 0;
						if (g2 > 255) g2 = 255;
						if (r2 < 0) r2 = 0;
						if (r2 > 255) r2 = 255;

				// import barvy
						col = PalImport((BYTE)r2, (BYTE)g2, (BYTE)b2);

				// odchylka aktuálního bodu od požadované barvy
						rgb = StdBitmapInfo->bmiColors + col;
						*odch0 = rgb->rgbBlue - b;
						odch0++;

						*odch0 = rgb->rgbGreen - g;
						odch0++;

						*odch0 = rgb->rgbRed - r;
						odch0++;
					  }
					}

				// uložení bodu
					*dst = col;
					dst++;						// zvýšení cílové adresy
				}
			}
			else
			{
				KonvCopy(dst, src, width);
				src += width;
				dst += width;
			}
			src += srcinc;					// zvýšení zdrojové adresy
		}
		break;

// 16 bitù
	case 16:
		srcinc = ((2*width + 3) & ~3) - 2*width;

		for (i = height; i > 0; i--)		// cyklus pøes všechny linky
		{
			if (Dither)
			{
				int* odch0 = odch + 3;			// ukazatel v bufferu odchylek

				for (j = width; j > 0; j--)		// cyklus pøes všechny body na lince
				{

				// požadovaná barva
					srcdat = *(WORD*)src;		// data jednoho bodu
					b = (BYTE)(srcdat & 0x1F);	// modrá složka
					b = (BYTE)(b*8 + b/4);
					srcdat >>= 5;				// zrušení bitù modré složky
					g = (BYTE)(srcdat & 0x1F);	// zelená složka
					g = (BYTE)(g*8 + g/4);
					srcdat >>= 5;				// zrušení bitù zelené složky
					r = (BYTE)(srcdat & 0x1F);	// èervená složka
					r = (BYTE)(r*8 + r/4);
					src++;						// zvýšení zdrojové adresy
					src++;						// zvýšení zdrojové adresy

				// zkorigovaná barva
					int b2 = b - (odch0[-3] + odch0[0] + odch0[3])*5/8;		// modrá složka
					int g2 = g - (odch0[-2] + odch0[1] + odch0[4])*5/8;		// zelená složka
					int r2 = r - (odch0[-1] + odch0[2] + odch0[5])*5/8;		// èervená složka

				// omezení pøeteèení barvy
					if (b2 < 0) b2 = 0;
					if (b2 > 255) b2 = 255;
					if (g2 < 0) g2 = 0;
					if (g2 > 255) g2 = 255;
					if (r2 < 0) r2 = 0;
					if (r2 > 255) r2 = 255;

				// import barvy
					BYTE col = PalImport((BYTE)r2, (BYTE)g2, (BYTE)b2);

				// odchylka aktuálního bodu od požadované barvy
					RGBQUAD* rgb = StdBitmapInfo->bmiColors + col;
					*odch0 = rgb->rgbBlue - b;
					odch0++;

					*odch0 = rgb->rgbGreen - g;
					odch0++;

					*odch0 = rgb->rgbRed - r;
					odch0++;

				// uložení bodu
					*dst = col;	// import barvy do vlastních palet
					dst++;						// zvýšení cílové adresy
				}
			}
			else
			{
				for (j = width; j > 0; j--)		// cyklus pøes všechny body
				{
					srcdat = *(WORD*)src;		// data jednoho bodu
					b = (BYTE)(srcdat & 0x1F);	// modrá složka
					b = (BYTE)(b*8 + b/4);
					srcdat >>= 5;				// zrušení bitù modré složky
					g = (BYTE)(srcdat & 0x1F);	// zelená složka
					g = (BYTE)(g*8 + g/4);
					srcdat >>= 5;				// zrušení bitù zelené složky
					r = (BYTE)(srcdat & 0x1F);	// èervená složka
					r = (BYTE)(r*8 + r/4);
					*dst = PalImport(r, g, b);	// import barvy do vlastních palet
					src++;						// zvýšení zdrojové adresy
					src++;
					dst++;						// zvýšení cílové adresy
				}
			}
			src += srcinc;
		}
		break;

// 24 bitù
	case 24:
		srcinc = ((3*width + 3) & ~3) - 3*width;

		for (i = height; i > 0; i--)			// cyklus pøes všechny linky
		{
			if (Dither)
			{
				int* odch0 = odch + 3;			// ukazatel v bufferu odchylek

				for (j = width; j > 0; j--)		// cyklus pøes všechny body na lince
				{

				// pozadí
					if ((*(int*)src & 0xffffff) == (BACKCOLOR_BLUE | (BACKCOLOR_GREEN*256) | (BACKCOLOR_RED*256*256)))
					{
						*dst = BackCol;
						src += 3;

						*odch0 = 0;
						odch0++;
						*odch0 = 0;
						odch0++;
						*odch0 = 0;
						odch0++;
					}
					else
					{	

				// stín
					  if ((*(int*)src & 0xffffff) == (SHADCOLOR_BLUE | (SHADCOLOR_GREEN*256) | (SHADCOLOR_RED*256*256)))
					  {
						*dst = ShadCol;
						src += 3;

						*odch0 = 0;
						odch0++;
						*odch0 = 0;
						odch0++;
						*odch0 = 0;
						odch0++;
					  }
					  else
					  {	

				// požadovaná barva
						b = *src;					// modrá složka
						src++;						// zvýšení zdrojové adresy
						g = *src;					// zelená složka
						src++;						// zvýšení zdrojové adresy
						r = *src;					// èervená složka
						src++;						// zvýšení zdrojové adresy

				// zkorigovaná barva
						int b2 = b - (odch0[-3] + odch0[0] + odch0[3])*5/8;		// modrá složka
						int g2 = g - (odch0[-2] + odch0[1] + odch0[4])*5/8;		// zelená složka
						int r2 = r - (odch0[-1] + odch0[2] + odch0[5])*5/8;		// èervená složka

				// omezení pøeteèení barvy
						if (b2 < 0) b2 = 0;
						if (b2 > 255) b2 = 255;
						if (g2 < 0) g2 = 0;
						if (g2 > 255) g2 = 255;
						if (r2 < 0) r2 = 0;
						if (r2 > 255) r2 = 255;

				// import barvy
						BYTE col = PalImport((BYTE)r2, (BYTE)g2, (BYTE)b2);

				// odchylka aktuálního bodu od požadované barvy
						RGBQUAD* rgb = StdBitmapInfo->bmiColors + col;
						*odch0 = rgb->rgbBlue - b;
						odch0++;

						*odch0 = rgb->rgbGreen - g;
						odch0++;

						*odch0 = rgb->rgbRed - r;
						odch0++;

				// uložení bodu
						*dst = col;	// import barvy do vlastních palet
					  }
					}
					dst++;						// zvýšení cílové adresy
				}
			}
			else
			{
				for (j = width; j > 0; j--)		// cyklus pøes všechny body
				{
					if ((*(int*)src & 0xffffff) == (BACKCOLOR_BLUE | (BACKCOLOR_GREEN*256) | (BACKCOLOR_RED*256*256)))
					{
						*dst = BackCol;
						src += 3;
					}
					else
					{
				// stín
					  if ((*(int*)src & 0xffffff) == (SHADCOLOR_BLUE | (SHADCOLOR_GREEN*256) | (SHADCOLOR_RED*256*256)))
					  {
						*dst = ShadCol;
						src += 3;
					  }
					  else
					  {	

						b = *src;					// modrá složka
						src++;						// zvýšení zdrojové adresy
						g = *src;					// zelená složka
						src++;						// zvýšení zdrojové adresy
						r = *src;					// èervená složka
						src++;						// zvýšení zdrojové adresy
						*dst = PalImport(r, g, b);	// import barvy do vlastních palet
					  }
					}
					dst++;						// zvýšení cílové adresy
				}
			}
			src += srcinc;
		}

/*
// vyhlazení ditheringu obrázku
		if (Dither)
		{
		  for (int aa = 5; aa > 0; aa--)
		  {
			srcinc = (3*width + 3) & ~3;

			dst = newdata->Data + (height-1)*width - 1;	// ukládací adresa
			src = (BYTE*)bmp + sizehead + (height-2)*srcinc + 3*width - 3;	// ètecí adresa

			for (i = height - 2; i > 0; i--)		// cyklus pøes všechny linky (bez okrajových)
			{
				src -= 3;							// pøeskoèení prvního bodu
				dst--;

				for (j = width - 2; j > 0; j--)		// cyklus pøes všechny body na lince (bez okrajových)
				{

					if ((*dst != BackCol) && (*dst != ShadCol))
					{
						int bo = 0;					// støadaè odchylek
						int go = 0;
						int ro = 0;
//						int no = 10;

						BYTE* dst0 = dst + width + 1;
						BYTE* src0 = src + srcinc + 3;

						for (int ii = 0; ii < 3; ii++)
						{
							for (int jj = 0; jj < 3; jj++)
							{
								BYTE c = *dst0;
								dst0--;
								if ((c != BackCol) && (c != ShadCol))
//									((ii != 2) || (jj != 2)))
//									(((ii == 0) && (jj == 1)) ||
//									 ((ii == 0) && (jj == 2)) ||
//									 ((ii == 1) && (jj == 0))
//									 ))
								{
									RGBQUAD* rgb = StdBitmapInfo->bmiColors + c;
									bo += rgb->rgbBlue - src0[0];
									go += rgb->rgbGreen - src0[1];
									ro += rgb->rgbRed - src0[2];
//									no++;
								}
								src0 -= 3;
							}
							dst0 -= width - 3;
							src0 -= srcinc - 3*3;
						}

				// požadovaná barva
						BYTE c = *dst;
						RGBQUAD* rgb = StdBitmapInfo->bmiColors + c;

//						int b;
//						int g;
//						int r;

				// zkorigovaná barva
						int b2;
						int g2;
						int r2;

						b2 = (src[0] + rgb->rgbBlue)/2;	// modrá složka
						g2 = (src[1] + rgb->rgbGreen)/2;	// zelená složka
						r2 = (src[2] + rgb->rgbRed)/2;		// èervená složka

						if ((i + j) & 1)
						{
							b2 = b2 - bo/4; //3/no;		// modrá složka
							g2 = g2 - go/4; //3/no;		// zelená složka
							r2 = r2 - ro/4; //3/no;		// èervená složka
						}
						else
						{
							b2 = b2 - bo/10; //3/no;		// modrá složka
							g2 = g2 - go/10; //3/no;		// zelená složka
							r2 = r2 - ro/10; //3/no;		// èervená složka
						}

				// omezení pøeteèení barvy
						if (b2 < 0) b2 = 0;
						if (b2 > 255) b2 = 255;
						if (g2 < 0) g2 = 0;
						if (g2 > 255) g2 = 255;
						if (r2 < 0) r2 = 0;
						if (r2 > 255) r2 = 255;

				// import barvy
						*dst = PalImport((BYTE)r2, (BYTE)g2, (BYTE)b2);
					}

					src -= 3;						// další bod
					dst--;
				}

				src -= 3;							// pøeskoèení posledního bodu
				dst--;

				src -= srcinc - 3*width;			// další linka
			}
		  }
		}
*/
//		break;
/*
// 32 bitù
	case 32:
		for (i = height; i > 0; i--)		// cyklus pøes všechny linky
		{
			if (Dither)
			{
				int* odch0 = odch + 3;			// ukazatel v bufferu odchylek

				for (j = width; j > 0; j--)		// cyklus pøes všechny body na lince
				{

				// pozadí
					if ((*(int*)src & 0xffffff) == (BACKCOLOR_BLUE | (BACKCOLOR_GREEN*256) | (BACKCOLOR_RED*256*256)))
					{
						*dst = BackCol;
						src += 4;

						*odch0 = 0;
						odch0++;
						*odch0 = 0;
						odch0++;
						*odch0 = 0;
						odch0++;
					}
					else
					{	

				// stín
					  if ((*(int*)src & 0xffffff) == (SHADCOLOR_BLUE | (SHADCOLOR_GREEN*256) | (SHADCOLOR_RED*256*256)))
					  {
						*dst = ShadCol;
						src += 4;

						*odch0 = 0;
						odch0++;
						*odch0 = 0;
						odch0++;
						*odch0 = 0;
						odch0++;
					  }
					  else
					  {	

				// požadovaná barva
						b = *src;					// modrá složka
						src++;						// zvýšení zdrojové adresy
						g = *src;					// zelená složka
						src++;						// zvýšení zdrojové adresy
						r = *src;					// èervená složka
						src++;						// zvýšení zdrojové adresy
						src++;						// zvýšení zdrojové adresy

				// zkorigovaná barva
						int b2 = b - (odch0[-3] + odch0[0] + odch0[3])*5/8;		// modrá složka
						int g2 = g - (odch0[-2] + odch0[1] + odch0[4])*5/8;		// zelená složka
						int r2 = r - (odch0[-1] + odch0[2] + odch0[5])*5/8;		// èervená složka

				// omezení pøeteèení barvy
						if (b2 < 0) b2 = 0;
						if (b2 > 255) b2 = 255;
						if (g2 < 0) g2 = 0;
						if (g2 > 255) g2 = 255;
						if (r2 < 0) r2 = 0;
						if (r2 > 255) r2 = 255;

				// import barvy
						BYTE col = PalImport((BYTE)r2, (BYTE)g2, (BYTE)b2);

				// odchylka aktuálního bodu od požadované barvy
						RGBQUAD* rgb = StdBitmapInfo->bmiColors + col;
						*odch0 = rgb->rgbBlue - b;
						odch0++;

						*odch0 = rgb->rgbGreen - g;
						odch0++;

						*odch0 = rgb->rgbRed - r;
						odch0++;

				// uložení bodu
						*dst = col;	// import barvy do vlastních palet
					  }
					}
					dst++;						// zvýšení cílové adresy
				}
			}
			else
			{
				for (j = width; j > 0; j--)		// cyklus pøes všechny body
				{
					if ((*(int*)src & 0xffffff) == (BACKCOLOR_BLUE | (BACKCOLOR_GREEN*256) | (BACKCOLOR_RED*256*256)))
					{
						*dst = BackCol;
						src += 4;
					}
					else
					{

				// stín
					  if ((*(int*)src & 0xffffff) == (SHADCOLOR_BLUE | (SHADCOLOR_GREEN*256) | (SHADCOLOR_RED*256*256)))
					  {
						*dst = ShadCol;
						src += 4;
					  }
					  else
					  {	

						b = *src;					// modrá složka
						src++;						// zvýšení zdrojové adresy
						g = *src;					// zelená složka
						src++;						// zvýšení zdrojové adresy
						r = *src;					// èervená složka
						src++;						// zvýšení zdrojové adresy
						src++;						// zvýšení zdrojové adresy
						*dst = PalImport(r, g, b);	// import barvy do vlastních palet
					  }
					}
					dst++;						// zvýšení cílové adresy
				}
			}
		}
		break;
	}

// uvolnìní bufferu odchylky pro dithering
	MemFree(odch);

// uvolnìní pøípadného bufferu
	MemFree(bmp2);

// odpojení starých dat
	detach(pData);

// pøipojení nových dat
	pData = newdata;

// komprimace dat (chyba nevadí)
	Comp();

// pøíznak - obrázek naèten OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// uložení do souboru formátu BMP (komprimovaný obrázek dekomprimuje) (FALSE=chyba)

bool CPicture::SaveFile(CText jmeno) const
{
// dekomprimace dat obrázku
	if (!DeComp()) return false;

// vytvoøení souboru
	CFile file;
	file.Name(jmeno);
	if (!file.Create()) return false;

// pøíprava velikosti záhlaví souboru
	int headsize = sizeof(BITMAPFILEHEADER) + 
					sizeof(BITMAPINFOHEADER) +
					StdColors*sizeof(RGBQUAD);

// pøíprava bufferu pro obrázek
	BYTE* buf = (BYTE*)MemGet(headsize + (pData->Width+6)*pData->Height*2 + 1000);
	if (buf == NULL)
	{
		file.Close();
		file.Delete();
		return false;
	}
	MemFill(buf, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER), 0);

// komprese dat
	int size = KompRLE8(buf + headsize, pData->Data, pData->Width, pData->Height);

// naplnìní záhlaví popisovaèe souboru
	BITMAPFILEHEADER* head = (BITMAPFILEHEADER*) buf;
	buf[0] = 'B';								// identifikace souboru
	buf[1] = 'M';
	head->bfSize = headsize + size;				// velikost souboru
	head->bfOffBits = headsize;					// offset dat

// naplnìní záhlaví popisovaèe dat obrázku
	BITMAPINFOHEADER* bmp = (BITMAPINFOHEADER*)(buf + sizeof(BITMAPFILEHEADER));
	bmp->biSize = sizeof(BITMAPINFOHEADER);		// velikost struktury
	bmp->biWidth = pData->Width;				// šíøka
	bmp->biHeight = pData->Height;				// výška
	bmp->biPlanes = 1;							// poèet barevných rovin
	bmp->biBitCount = 8;						// poèet bitù na bod
	bmp->biCompression = BI_RLE8;				// komprese
	bmp->biSizeImage = size;					// velikost dat obrázku
	bmp->biClrUsed = StdColors;					// poèet použitých palet
	bmp->biClrImportant = StdColors;			// poèet dùležitých palet

// pøenesení palet
	MemCopy(buf + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER),
				StdBitmapInfo->bmiColors, StdColors*sizeof(RGBQUAD));

// uložení souboru
	BOOL result = file.Write(buf, size + headsize);

// uzavøení souboru
	file.Close();

// zrušení bufferu
	MemFree(buf);

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
// operátor pøiøazení

const CPicture& CPicture::operator= (const CPicture& src)
{
	detach(pData);				// zrušení starých dat
	attach(src.pData);		// pøiøazení nových dat
	return *this;
}

const CPicture& CPicture::operator= (PICTUREDATA* src)
{
	detach(pData);				// zrušení starých dat
	attach(src);				// pøiøazení nových dat
	return *this;
}


/////////////////////////////////////////////////////////////////////////////
// vygenerování obrázku pro tažení (pøi chybì vrací NULL)
/*
HIMAGELIST CPicture::GenerDrag(const CText& text)
{
// dekomprimace dat obrázku
	if (!DeComp()) return NULL;

// pøíprava bufferu textu
	char* textbuf = NULL;				// buffer s textem
	if (text.IsNotEmpty())
	{
#ifdef _UNICODE
		textbuf = (char*)MemGet(text.Length()+1);
		if (textbuf == NULL) return NULL;
		text.WriteNull(textbuf);
#else
		textbuf = text.DataData();
#endif
	}

// pøíprava rozmìrù
	int width = pData->Width;			// šíøka obrázku
	int height = pData->Height;			// výška obrázku
	int widtht = 0;						// šíøka textové èásti
	int len = 0;						// délka textu

// zjištìní šíøky textové èásti a délky textu (s omezením - pøi TRUECOLOR nefunguje pøi velikosti 512*32 bodù)
	if (text.IsNotEmpty())
	{
		widtht = 6 + 16;				// šíøka pro úvodní a koncovou mezeru

		for (; ((len < text.Length()) && (widtht < (512-width-16-32))); len++)
		{
			widtht += StdFontsWidth[(BYTE)textbuf[len]] + 1;
		}

		if (height < 16) height = 16;
	}

// celková šíøka obrázku s textem
	int widthc = width + widtht;		// celková šíøka obrázku s textem
	int widthbyte = (widthc + 3) & ~3;	// délka linky barevné bitmapy

// buffer pro barevnou bitmapu
	int datasize = (widthbyte+8) * height;
	BYTE* data = (BYTE*)MemGet(datasize);
	if (data == NULL)
	{
#ifdef _UNICODE
		MemFree(textbuf);
#endif
		return NULL;
	}

// vymazání bufferu (pokud bude text)
	if (widtht > 0) MemFill(data, datasize, BackCol);

// vytvoøení dat barevné bitmapy
	BYTE* dst = data;
	BYTE* src = pData->Data;
	for (int i = pData->Height; i > 0; i--)
	{
		MemCopy(dst, src, width);
		dst += widthbyte;
		src += width;
	}

// dekódování textu
	if (widtht > 0)
	{
		BYTE* dst2 = data + width + 6 + (height - 16)/2*widthbyte;

		for (i = 0; i < len; i++)
		{
			BYTE c = (BYTE)textbuf[i];

			BYTE* dst3 = dst2;
			BYTE* src2 = StdFonts[c].DataData();
			for (int k = 16; k > 0; k--)
			{
				MemCopy(dst3, src2, 16);
				dst3 += widthbyte;
				src2 += 16;
			}
			dst2 += StdFontsWidth[c] + 1;
		}

	}

#ifdef _UNICODE
	MemFree(textbuf);
#endif

// buffer pro mono bitmapu
	int dstinc = ((widthc + 15) & ~0xf) / 8;	// délka linky mono bitmapy
	BYTE* mono = (BYTE*)MemGet(dstinc * height);
	if (mono == NULL)
	{
		MemFree(data);
		return NULL;
	}

// vygenerování mono bitmapy
	dst = mono;
	BYTE maska2 = 0xAA;

	for (i = height-1; i >= 0; i--)
	{
		src = data + widthbyte*i;
		int n = ((width + 15) & ~0xf) / 8;

		for (int j = dstinc - 1; j >= 0; j--)
		{
			BYTE stradac = 0;
			BYTE maska = 0x80;
			for (int k = 8; k > 0; k--)
			{
				if (*src == BackCol) stradac |= maska;
				src++;
				maska >>= 1;
			}

			if (n > 0)
			{
				*dst = (BYTE)(stradac | maska2);	
			}
			else
			{
				*dst = (BYTE)(stradac);
			}
			dst++;
			n--;
		}
		maska2 ^= 0xff;
	}

// vytvoøení mono bitmapy (maska)
	HBITMAP bmpMono = ::CreateBitmap(widthc, height, 1, 1, mono);
	ASSERT (bmpMono != NULL);
	if (bmpMono == NULL)
	{
		MemFree(mono);
		MemFree(data);
		return NULL;
	}

// otevøení DC displeje
	HDC dc = ::GetDC(0);
	ASSERT(dc != NULL);

// výbìr a realizace vlastních palet 
	HPALETTE OldPal = ::SelectPalette(dc, StdPalette, FALSE);
	::RealizePalette(dc);

// pøíprava záhlaví BMP
	StdBitmapInfo->bmiHeader.biWidth = widthc;
	StdBitmapInfo->bmiHeader.biHeight = height;

// vytvoøení barevné bitmapy
	HBITMAP bmp = ::CreateDIBitmap(dc, &(StdBitmapInfo->bmiHeader),
		CBM_INIT, data, StdBitmapInfo, DIB_RGB_COLORS);
	ASSERT(bmp != NULL);

// vytvoøení seznamu
	HIMAGELIST hImg = ::ImageList_Create(widthc, height, ILC_COLORDDB | ILC_MASK, 1, 1);
	ASSERT(hImg != NULL);

// pøidání bitmapy k seznamu
	int result = ::ImageList_Add(hImg, bmp, bmpMono);
	ASSERT(result != -1);

// zrušení bitmap
	if (bmp != NULL) ::DeleteObject(bmp);
	::DeleteObject(bmpMono);

// uvolnìní palet a DC displeje
	if (OldPal != NULL) ::SelectPalette(dc,OldPal,FALSE);
	::ReleaseDC(0,dc);

// zrušení pracovních bufferù s daty
	MemFree(mono);
	MemFree(data);

// kontrola operace
	if (result == -1)
	{
		::ImageList_Destroy(hImg);
		hImg = NULL;
	}

// navrácení seznamu
	return hImg;
}
*/

