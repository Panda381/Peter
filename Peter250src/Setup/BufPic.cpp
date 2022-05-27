
#include "Main.h"

/***************************************************************************\
*																			*
*								Obr�zkov� prom�nn�							*
*																			*
\***************************************************************************/

////////////////////////////////////////////////////////////////////
// Pozn.: linky souboru BMP pro 256 barev jsou zarovn�v�ny na 4 bajty
//        linky souboru BMP pro 2 barvy (maska) jsou zarovn�ny na 2 bajty
////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// inicializa�n� pr�zdn� obr�zek (modifikuje se po�et referenc�!)

// Pr�zdn� obr�zek mus� m�t rozm�ry ICONSIZE kv�li vytvo�en� nov�ho sprajtu!
PICTUREDATA	EmptyPictureData		= { 1, ICONWIDTH, ICONHEIGHT, PicParamBack, 0, 0, 0, NULL };
const CPicture	EmptyPicture;				// pr�zdn� obr�zek

/////////////////////////////////////////////////////////////////////////////
// statick� inicializace obr�zk� (p�i chyb� pam�ti vrac� FALSE)

bool InitPicture()
{
// pr�zdn� obr�zek
	EmptyPictureData.Data = (BYTE*)MemGet(ICONSIZE);
	if (EmptyPictureData.Data == NULL) return false;
	MemFill(EmptyPictureData.Data, ICONSIZE, (char)(BYTE)BackCol);
	return true;
}

////////////////////////////////////////////////////////////////////
// vytvo�en� dat obr�zku (p�i chyb� pam�ti vrac� NULL)

PICTUREDATA* _fastcall NewPictureData(int width, int height)
{
	ASSERT((width > 0) && (height > 0));

// vytvo�en� z�hlav� obr�zku
	PICTUREDATA* data = (PICTUREDATA*)MemGet(SIZEOFPICTUREDATA); // vytvo�en� z�hlav�
	if (data != NULL)
	{

// nastaven� dat obr�zku
		data->Refer = 1;					// po�et referenc�
		data->Width = width;				// ���ka
		data->Height = height;				// v��ka
		data->Param = PicParamNone;			// parametry (obsah nezn�m�)

// vytvo�en� bufferu dat obr�zku
		BYTE* datadata = (BYTE*)MemGet(width*height);
		data->Data = datadata;				// adresa dat

// p�i chyb� pam�ti zru�en� z�hlav� obr�zku
		if (datadata == NULL)
		{
			MemFree(data);
			data = NULL;
		}
	}
	return data;
}

/////////////////////////////////////////////////////////////////////////////
// zru�en� dat obr�zku (odd�leno kv�li lep�� optimalizaci)

void _fastcall DelPictureData(PICTUREDATA* data)
{
	ASSERT(data != NULL);
	ASSERT(data->Data != NULL);

	MemFree(data->Data);		// zru�en� dat obr�zku
	MemFree(data);				// zru�en� z�hlav� obr�zku
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
// statick� konstruktor a destruktor

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

bool _fastcall CPicture::Init(int width, int height) // p�i chyb� pam�ti vrac� FALSE, obr�zek nen� vytvo�en
{
	pData = NewPictureData(width, height);
	return (pData != NULL);
};

void CPicture::Term()
{ 
	detach(pData); 
};


/////////////////////////////////////////////////////////////////////////////
// vypr�zdn�n� obr�zku (uvoln�n� dat)

void CPicture::Empty()
{ 
	Term();
	Init();
}


////////////////////////////////////////////////////////////////////
// vymaz�n� obsahu obr�zku (napln�n� pr�hlednou barvou), zajist� p�ivlastn�n� (a dekomprimaci) bufferu,
// p�i chyb� pam�ti vrac� FALSE, p�vodn� obsah nezm�n�n

bool CPicture::Clear()
{
// vytvo�en� nov�ho bufferu, je-li pot�eba
	if (!New()) return false;

// vymaz�n� bufferu
	MemFill(pData->Data, pData->Width * pData->Height, (char)(BYTE)BackCol);
	
// nastaven� parametr� na pozad�	
	pData->Param = PicParamBack;
	return true;
}


////////////////////////////////////////////////////////////////////
// vymaz�n� obsahu obr�zku s nastaven�m velikosti (napln�n� pr�hlednou barvou), zajist� p�ivlastn�n� (a dekomprimaci) bufferu,
// p�i chyb� pam�ti vrac� FALSE, p�vodn� obsah nezm�n�n

bool _fastcall CPicture::Clear(int width, int height)
{
// vytvo�en� nov�ho bufferu, je-li pot�eba
	if (!New(width, height)) return false;

// vymaz�n� bufferu
	MemFill(pData->Data, pData->Width * pData->Height, (char)(BYTE)BackCol);
	
// nastaven� parametr� na pozad�	
	pData->Param = PicParamBack;
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// kopie obr�zku do vlastn�ho bufferu p�ed modifikac� (komprimovan� data z�stanou komprimovan�)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool CPicture::CopyWrite()
{
// �schova ukazatel�
	PICTUREDATA* olddata = pData;	// adresa star�ch dat
	long* refer = &(olddata->Refer);// po�et referenc�

// test, zda je nutn� p�ivlastn�n�
	if (*refer > 1)					// je n�jak� jin� majitel?
	{

// vytvo�en� nov�ho bufferu
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

// p�enesen� dat
		MemCopy(newdata->Data, olddata->Data, size);
		newdata->Param = olddata->Param;

// odpojen� star�ch dat
		detach(olddata);

// p�ipojen� nov�ch dat
		pData = newdata;
	}

// operace OK
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// kopie v��ezu do obr�zku (se zpr�hledn�n�m) - nekontroluje hranice!

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
// vytvo�en� nov�ho obr�zku se stejnou velikost� (p�ipraveno k z�pisu, data jsou n�hodn�)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool CPicture::New()
{		 
// ukazatel na star� data
	PICTUREDATA* olddata = pData;			// adresa star�ch dat

// test, zda je nutn� vytvo�en� nov�ho bufferu
	if ((olddata->Refer > 1) ||				// na buffer je v�ce referenc�
		(olddata->Param == PicParamComp))	// data jsou komprimovan�
	{

// vytvo�en� nov�ho bufferu
		PICTUREDATA* newdata = NewPictureData(pData->Width, pData->Height);
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
// vytvo�en� nov�ho obr�zku (p�ipraveno k z�pisu, data jsou n�hodn�)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool _fastcall CPicture::New(int width, int height)
{		 
// ukazatel na star� data
	PICTUREDATA* olddata = pData;			// adresa star�ch dat

// test, zda je nutn� vytvo�en� nov�ho bufferu
	if ((olddata->Refer > 1) ||				// na buffer je v�ce referenc�
		(olddata->Width != width) ||		// nesouhlas� ���ka
		(olddata->Height != height) ||		// nesouhlas� v��ka
		(olddata->Param == PicParamComp))	// data jsou komprimovan�
	{

// vytvo�en� nov�ho bufferu
		PICTUREDATA* newdata = NewPictureData(width, height);
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
// nastaven� nov�ch rozm�r� obr�zku (nov� data jsou vymaz�na)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool CPicture::Resize(int width, int height)
{
// zaji�t�n� dekomprimace
	if (!DeComp()) return false;

// p�ivlastn�n� bufferu
	if (!CopyWrite()) return false;

// �schova star�ch rozm�r�
	int oldwidth = pData->Width;
	int oldheight = pData->Height;

// test, zda je pot�eba velikost obr�zku m�nit
	if ((width != oldwidth) || (height != oldheight))
	{

// vytvo�en� nov�ho bufferu
		PICTUREDATA* olddata = pData;
		PICTUREDATA* newdata = NewPictureData(width, height);
		if (newdata == NULL) return false;

// p��prava po�tu linek ke kopii
		int i = height;
		if (oldheight < i) i = oldheight;

// p��prava d�lky linky ke kopii
		int j = width;
		if (oldwidth < j) j = oldwidth;

// p��prava zbytku linky k vymaz�n�
		int k = width - j;

// p��prava zdrojov� a c�lov� adresy
		BYTE* src = olddata->Data;
		BYTE* dst = newdata->Data;

// kopie platn�ch linek p�i shod� d�lek linek
		if (width == oldwidth)
		{
			i *= width;
			MemCopy(dst, src, i);
			dst += i;
			src += i;
		}

// kopie platn�ch linek p�i rozd�ln� d�lce linek
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

// vymaz�n� zbyl�ch linek
		i = height - oldheight;
		if (i > 0)
		{
			MemFill(dst, i*width, (char)(BYTE)BackCol);
		}

// odpojen� star�ch dat
		detach(olddata);

// p�ipojen� nov�ch dat
		pData = newdata;
	}

// operace OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// poskytnut� bodu (s kontrolou platnosti offsetu/indexu), obr�zek nesm� b�t komprimovan�!
// pro neplatn� bod vr�t� barvu pozad�)

BYTE _fastcall CPicture::Get(const int off) const
{
	ASSERT(pData->Param != PicParamComp);

	if (IsValid(off))				// je offset platn�?
	{
		return pData->Data[off];	// bod na dan�m offsetu
	}
	return (BYTE)BackCol;					// neplatn� offset
}

BYTE _fastcall CPicture::Get(const int x, const int y) const
{
	ASSERT(pData->Param != PicParamComp);

	if (IsValid(x, y))				// je index platn�?
	{
		return pData->Data[x + y*pData->Width];	// prvek
	}
	return (BYTE)BackCol;					// neplatn� offset
}


////////////////////////////////////////////////////////////////////
// nastaven� bodu (s kontrolou platnosti offsetu/indexu), obr�zek nesm� b�t komprimovan�!
// p�ed z�pisem je nutno zajistit p�ivlastn�n� bufferu!

void _fastcall CPicture::Set(const int off, const BYTE data)
{
	ASSERT(pData->Param != PicParamComp);
	ASSERT(pData->Refer == 1);

	if (IsValid(off))				// je offset platn�?
	{
		pData->Data[off] = data;
	}
}

void _fastcall CPicture::Set(const int x, const int y, const BYTE data)
{
	ASSERT(pData->Param != PicParamComp);
	ASSERT(pData->Refer == 1);

	if (IsValid(x, y))				// je index platn�?
	{
		pData->Data[x + y*pData->Width] = data;
	}
}


/////////////////////////////////////////////////////////////////////////////
// komprese dat obr�zku, vrac� velikost dat (p�i chyb� pam�ti vrac� <0, data jsou nezm�n�na)
// komprese se prov�d� ve spole�n�m bufferu pro v�echny prom�nn�!
// (v p��pad� pot�eby je nutno zajistit p�ivlastn�n� bufferu)
// p�vodn� buffer s daty je zru�en
// (velikost komprimovan�ch dat je ud�v�na bez dvojslova s velikost�, buffer s daty
// je tedy o 4 v�t��, data za��naj� na offsetu 4 v bufferu)

int CPicture::Comp() const
{
// test, zda je obr�zek ji� komprimov�n
	if (pData->Param == PicParamComp)
	{
		return *(long*)pData->Data;
	}

// velikost dat obr�zku
	int size = pData->Width * pData->Height;

// vytvo�en� nov�ho bufferu pro data
	BYTE* newdata = (BYTE*)MemGet(2*size + 200);
	if (newdata == NULL) return -1;

// komprese dat
	BYTE* olddata = pData->Data;
	int newsize = Compress(newdata + 4, olddata, size, pData->Width);
	*(long*)newdata = newsize;

// vytvo�en� bufferu se spr�vnou velikost� (MemSize() by blok nezmen�il)
	BYTE* newdata2 = (BYTE*)MemGet(newsize + 4);
	if (newdata2 == NULL)
	{
		MemFree(newdata);
		return -1;
	}

// kopie dat do nov�ho bufferu
	MemCopy(newdata2, newdata, newsize+4);
	MemFree(newdata);

// zru�en� star�ho bufferu
	MemFree(olddata);

// nastaven� parametr�
	pData->Data = newdata2;
	pData->Param = PicParamComp;

// nov� velikost dat
	return newsize;
}


/////////////////////////////////////////////////////////////////////////////
// dekomprimace dat obr�zku (jsou-li komprimov�na), p�i chyb� vrac� FALSE
// dekomprese se prov�d� ve spole�n�m bufferu pro v�echny prom�nn�
// (v p��pad� pot�eby je nutno zajistit p�ivlastn�n� bufferu)
// p�vodn� buffer s komprimovan�mi daty je zru�en

bool CPicture::DeComp() const
{
// test, zda je obr�zek komprimov�n
	if (pData->Param == PicParamComp)
	{

// vytvo�en� nov�ho bufferu pro data
		int size = pData->Width * pData->Height;
		BYTE* newdata = (BYTE*)MemGet(size);
		if (newdata == NULL) return false;

// dekomprese dat
		BYTE* olddata = pData->Data;
		DeCompress(newdata, olddata + 4, size, pData->Width);

// zru�en� star�ho bufferu
		MemFree(olddata);

// nastaven� parametr�
		pData->Data = newdata;
		pData->Param = PicParamNone;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// kopie nov�ch dat obr�zku (rozm�ry z�stanou nezm�n�ny) - zajist� odpojen� dat
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool CPicture::CopyData(BYTE* src)
{
// vytvo�en� nov�ho bufferu, je-li pot�eba
	if (!New()) return false;

// kopie dat do bufferu
	MemCopy(pData->Data, src, pData->Width * pData->Height);
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// kopie komprimovan�ch nov�ch dat obr�zku (rozm�ry z�stanou nezm�n�ny) - zajist� odpojen� dat
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool CPicture::CompCopy(BYTE* src)
{
// �schova parametr�
	int width = pData->Width;
	int height = pData->Height;
	int size = *(long*)src + 4;

// vytvo�en� nov�ho bufferu
	if (!New(size, 1)) return false;

// nastaven� parametr�
	pData->Width = width;
	pData->Height = height;
	pData->Param = PicParamComp;

// kopie dat do bufferu
	MemCopy(pData->Data, src, size);
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// kopie nov�ch dat obr�zku s konverz� (rozm�ry z�stanou nezm�n�ny) - zajist� odpojen� dat
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool CPicture::CopyKonvData(BYTE* src)
{
// vytvo�en� nov�ho bufferu, je-li pot�eba
	if (!New()) return false;

// kopie dat do bufferu
	KonvCopy(pData->Data, src, pData->Width * pData->Height);
	return true;
}


////////////////////////////////////////////////////////////////////
// na�ten� obr�zku z resource

bool CPicture::Load(const int nID)
{
// nalezen� resource bitmapy
	CResource res;
	if (!res.Open(nID, RT_BITMAP)) { ASSERTERROR; return false; }

// adresa bitmapy
	BITMAPINFO* bmp = (BITMAPINFO*)res.Adr();
	ASSERT(bmp->bmiHeader.biBitCount == 8);
	int colors = bmp->bmiHeader.biClrUsed;
	if (colors == 0) colors = 256;
	ASSERT((DWORD)colors <= 256);

// p��prava p��rustku zdrojov� adresy mezi linkami
	int srcinc = (bmp->bmiHeader.biWidth + 3) & ~3;

// nov� data obr�zku
	New(bmp->bmiHeader.biWidth, bmp->bmiHeader.biHeight);

// dekomprese bitmapy
	BITMAPINFO*	bmp2 = NULL; // pomocn� buffer pro dekompresi bitmapy
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


// p��padn� zru�en� bufferu
	MemFree(bmp2);

	return true;
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� obr�zku ze souboru (FALSE=chyba, obsah nezm�n�n)
/*
bool CPicture::LoadFile(CText jmeno)
{
// otev�en� souboru mapovan�ho do pam�ti (uzav�en p�i destrukci!)
	CFileMap file;
	if (!file.Open(jmeno)) return false;

// velikost souboru bez z�hlav� 14 bajt�
	int size = file.Size() - 14;

// identifika�n� z�hlav�
	char* hd = (char*)file.Adr();
	BITMAPINFO* bmp = (BITMAPINFO*)(file.Adr() + 14);
	
// kontrola velikosti souboru a z�hlav�
	if ((size < 50) || 
		(hd[0] != 'B') || 
		(hd[1] != 'M') ||
		!TestBMP(bmp))
	{
		return false;
	}

// p��prava parametr� bitmapy
	int width = bmp->bmiHeader.biWidth;			// ���ka
	int height = bmp->bmiHeader.biHeight;		// v��ka
	int bits = bmp->bmiHeader.biBitCount;		// po�et bit� na bod
	int colors = bmp->bmiHeader.biClrUsed;		// po�et pou�it�ch barev
	if (colors <= 0) colors = (1 << bits);		// implicitn� po�et barev
	if (bits > 8) colors = 0;					// pro TRUE COLOR nejsou palety
	int sizehead = sizeof(BITMAPINFOHEADER) + colors*sizeof(RGBQUAD); // velikost z�hlav�
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

// vytvo�en� nov�ho bufferu
	PICTUREDATA* newdata = NewPictureData(width, height);
	if (newdata == NULL)
	{
		MemFree(bmp2);
		return false;
	}

// vygenerov�n� konverzn� tabulky palet
//	if (bits <= 8)
//	{
//		GenKonvPal(bmp);
//	}

// p��prava bufferu odchylky pro dithering
	int* odch = NULL;
	if (Dither)
	{
		odch = (int*)MemGet((3*width + 6) * sizeof(int));
		MemFill(odch, (3*width + 6) * sizeof(int), 0);
	}

// p��prava parametr� ke konverzi
	BYTE* dst = newdata->Data;				// ukl�dac� adresa
	BYTE* src = (BYTE*)bmp + sizehead;		// �tec� adresa
	int srcinc;								// p��rustek zdrojov� adresy
	int i, j;								// pracovn� ��ta�e
	BYTE r, g, b;							// barevn� slo�ky
	WORD srcdat;							// zdrojov� data 16 bit�

// rozli�en� podle po�tu bod�
	switch (bits)
	{

// 1 bit
	case 1:
		srcinc = ((width+7)/8 + 1) & ~1;	// p��rustek zdrojov� adresy

		for (i = height; i > 0; i--)		// cyklus p�es v�echny linky
		{
			for (j = 0; j < width; j++)
			{
				*dst = KonvPal[(src[j/8] >> (7 - (j & 7))) & 1];
				dst++;
			}
			src += srcinc;					// zv��en� zdrojov� adresy
		}
		break;

// 4 bity
	case 4:
		srcinc = ((width+1)/2 + 3) & ~3;	// p��rustek zdrojov� adresy

		for (i = height; i > 0; i--)		// cyklus p�es v�echny linky
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
			src += srcinc;					// zv��en� zdrojov� adresy
		}
		break;

// 8 bit�
	case 8:
		srcinc = ((width + 3) & ~3) - width; // p��rustek zdrojov� adresy

		for (i = height; i > 0; i--)		// cyklus p�es v�echny linky
		{
			if (Dither)
			{
				int* odch0 = odch + 3;			// ukazatel v bufferu odchylek

				for (j = width; j > 0; j--)		// cyklus p�es v�echny body
				{
				// bod k z�pisu
					BYTE col = *src;
					src++;
					if (col >= colors) col = 0;
					RGBQUAD* rgb = bmp->bmiColors + col;

				// pozad�
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

				// st�n
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

				// po�adovan� barva
						b = rgb->rgbBlue;			// modr� slo�ka
						g = rgb->rgbGreen;			// zelen� slo�ka
						r = rgb->rgbRed;			// �erven� slo�ka

				// zkorigovan� barva
						int b2 = b - (odch0[-3] + odch0[0] + odch0[3])*5/8;		// modr� slo�ka
						int g2 = g - (odch0[-2] + odch0[1] + odch0[4])*5/8;		// zelen� slo�ka
						int r2 = r - (odch0[-1] + odch0[2] + odch0[5])*5/8;		// �erven� slo�ka

				// omezen� p�ete�en� barvy
						if (b2 < 0) b2 = 0;
						if (b2 > 255) b2 = 255;
						if (g2 < 0) g2 = 0;
						if (g2 > 255) g2 = 255;
						if (r2 < 0) r2 = 0;
						if (r2 > 255) r2 = 255;

				// import barvy
						col = PalImport((BYTE)r2, (BYTE)g2, (BYTE)b2);

				// odchylka aktu�ln�ho bodu od po�adovan� barvy
						rgb = StdBitmapInfo->bmiColors + col;
						*odch0 = rgb->rgbBlue - b;
						odch0++;

						*odch0 = rgb->rgbGreen - g;
						odch0++;

						*odch0 = rgb->rgbRed - r;
						odch0++;
					  }
					}

				// ulo�en� bodu
					*dst = col;
					dst++;						// zv��en� c�lov� adresy
				}
			}
			else
			{
				KonvCopy(dst, src, width);
				src += width;
				dst += width;
			}
			src += srcinc;					// zv��en� zdrojov� adresy
		}
		break;

// 16 bit�
	case 16:
		srcinc = ((2*width + 3) & ~3) - 2*width;

		for (i = height; i > 0; i--)		// cyklus p�es v�echny linky
		{
			if (Dither)
			{
				int* odch0 = odch + 3;			// ukazatel v bufferu odchylek

				for (j = width; j > 0; j--)		// cyklus p�es v�echny body na lince
				{

				// po�adovan� barva
					srcdat = *(WORD*)src;		// data jednoho bodu
					b = (BYTE)(srcdat & 0x1F);	// modr� slo�ka
					b = (BYTE)(b*8 + b/4);
					srcdat >>= 5;				// zru�en� bit� modr� slo�ky
					g = (BYTE)(srcdat & 0x1F);	// zelen� slo�ka
					g = (BYTE)(g*8 + g/4);
					srcdat >>= 5;				// zru�en� bit� zelen� slo�ky
					r = (BYTE)(srcdat & 0x1F);	// �erven� slo�ka
					r = (BYTE)(r*8 + r/4);
					src++;						// zv��en� zdrojov� adresy
					src++;						// zv��en� zdrojov� adresy

				// zkorigovan� barva
					int b2 = b - (odch0[-3] + odch0[0] + odch0[3])*5/8;		// modr� slo�ka
					int g2 = g - (odch0[-2] + odch0[1] + odch0[4])*5/8;		// zelen� slo�ka
					int r2 = r - (odch0[-1] + odch0[2] + odch0[5])*5/8;		// �erven� slo�ka

				// omezen� p�ete�en� barvy
					if (b2 < 0) b2 = 0;
					if (b2 > 255) b2 = 255;
					if (g2 < 0) g2 = 0;
					if (g2 > 255) g2 = 255;
					if (r2 < 0) r2 = 0;
					if (r2 > 255) r2 = 255;

				// import barvy
					BYTE col = PalImport((BYTE)r2, (BYTE)g2, (BYTE)b2);

				// odchylka aktu�ln�ho bodu od po�adovan� barvy
					RGBQUAD* rgb = StdBitmapInfo->bmiColors + col;
					*odch0 = rgb->rgbBlue - b;
					odch0++;

					*odch0 = rgb->rgbGreen - g;
					odch0++;

					*odch0 = rgb->rgbRed - r;
					odch0++;

				// ulo�en� bodu
					*dst = col;	// import barvy do vlastn�ch palet
					dst++;						// zv��en� c�lov� adresy
				}
			}
			else
			{
				for (j = width; j > 0; j--)		// cyklus p�es v�echny body
				{
					srcdat = *(WORD*)src;		// data jednoho bodu
					b = (BYTE)(srcdat & 0x1F);	// modr� slo�ka
					b = (BYTE)(b*8 + b/4);
					srcdat >>= 5;				// zru�en� bit� modr� slo�ky
					g = (BYTE)(srcdat & 0x1F);	// zelen� slo�ka
					g = (BYTE)(g*8 + g/4);
					srcdat >>= 5;				// zru�en� bit� zelen� slo�ky
					r = (BYTE)(srcdat & 0x1F);	// �erven� slo�ka
					r = (BYTE)(r*8 + r/4);
					*dst = PalImport(r, g, b);	// import barvy do vlastn�ch palet
					src++;						// zv��en� zdrojov� adresy
					src++;
					dst++;						// zv��en� c�lov� adresy
				}
			}
			src += srcinc;
		}
		break;

// 24 bit�
	case 24:
		srcinc = ((3*width + 3) & ~3) - 3*width;

		for (i = height; i > 0; i--)			// cyklus p�es v�echny linky
		{
			if (Dither)
			{
				int* odch0 = odch + 3;			// ukazatel v bufferu odchylek

				for (j = width; j > 0; j--)		// cyklus p�es v�echny body na lince
				{

				// pozad�
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

				// st�n
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

				// po�adovan� barva
						b = *src;					// modr� slo�ka
						src++;						// zv��en� zdrojov� adresy
						g = *src;					// zelen� slo�ka
						src++;						// zv��en� zdrojov� adresy
						r = *src;					// �erven� slo�ka
						src++;						// zv��en� zdrojov� adresy

				// zkorigovan� barva
						int b2 = b - (odch0[-3] + odch0[0] + odch0[3])*5/8;		// modr� slo�ka
						int g2 = g - (odch0[-2] + odch0[1] + odch0[4])*5/8;		// zelen� slo�ka
						int r2 = r - (odch0[-1] + odch0[2] + odch0[5])*5/8;		// �erven� slo�ka

				// omezen� p�ete�en� barvy
						if (b2 < 0) b2 = 0;
						if (b2 > 255) b2 = 255;
						if (g2 < 0) g2 = 0;
						if (g2 > 255) g2 = 255;
						if (r2 < 0) r2 = 0;
						if (r2 > 255) r2 = 255;

				// import barvy
						BYTE col = PalImport((BYTE)r2, (BYTE)g2, (BYTE)b2);

				// odchylka aktu�ln�ho bodu od po�adovan� barvy
						RGBQUAD* rgb = StdBitmapInfo->bmiColors + col;
						*odch0 = rgb->rgbBlue - b;
						odch0++;

						*odch0 = rgb->rgbGreen - g;
						odch0++;

						*odch0 = rgb->rgbRed - r;
						odch0++;

				// ulo�en� bodu
						*dst = col;	// import barvy do vlastn�ch palet
					  }
					}
					dst++;						// zv��en� c�lov� adresy
				}
			}
			else
			{
				for (j = width; j > 0; j--)		// cyklus p�es v�echny body
				{
					if ((*(int*)src & 0xffffff) == (BACKCOLOR_BLUE | (BACKCOLOR_GREEN*256) | (BACKCOLOR_RED*256*256)))
					{
						*dst = BackCol;
						src += 3;
					}
					else
					{
				// st�n
					  if ((*(int*)src & 0xffffff) == (SHADCOLOR_BLUE | (SHADCOLOR_GREEN*256) | (SHADCOLOR_RED*256*256)))
					  {
						*dst = ShadCol;
						src += 3;
					  }
					  else
					  {	

						b = *src;					// modr� slo�ka
						src++;						// zv��en� zdrojov� adresy
						g = *src;					// zelen� slo�ka
						src++;						// zv��en� zdrojov� adresy
						r = *src;					// �erven� slo�ka
						src++;						// zv��en� zdrojov� adresy
						*dst = PalImport(r, g, b);	// import barvy do vlastn�ch palet
					  }
					}
					dst++;						// zv��en� c�lov� adresy
				}
			}
			src += srcinc;
		}

/*
// vyhlazen� ditheringu obr�zku
		if (Dither)
		{
		  for (int aa = 5; aa > 0; aa--)
		  {
			srcinc = (3*width + 3) & ~3;

			dst = newdata->Data + (height-1)*width - 1;	// ukl�dac� adresa
			src = (BYTE*)bmp + sizehead + (height-2)*srcinc + 3*width - 3;	// �tec� adresa

			for (i = height - 2; i > 0; i--)		// cyklus p�es v�echny linky (bez okrajov�ch)
			{
				src -= 3;							// p�esko�en� prvn�ho bodu
				dst--;

				for (j = width - 2; j > 0; j--)		// cyklus p�es v�echny body na lince (bez okrajov�ch)
				{

					if ((*dst != BackCol) && (*dst != ShadCol))
					{
						int bo = 0;					// st�ada� odchylek
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

				// po�adovan� barva
						BYTE c = *dst;
						RGBQUAD* rgb = StdBitmapInfo->bmiColors + c;

//						int b;
//						int g;
//						int r;

				// zkorigovan� barva
						int b2;
						int g2;
						int r2;

						b2 = (src[0] + rgb->rgbBlue)/2;	// modr� slo�ka
						g2 = (src[1] + rgb->rgbGreen)/2;	// zelen� slo�ka
						r2 = (src[2] + rgb->rgbRed)/2;		// �erven� slo�ka

						if ((i + j) & 1)
						{
							b2 = b2 - bo/4; //3/no;		// modr� slo�ka
							g2 = g2 - go/4; //3/no;		// zelen� slo�ka
							r2 = r2 - ro/4; //3/no;		// �erven� slo�ka
						}
						else
						{
							b2 = b2 - bo/10; //3/no;		// modr� slo�ka
							g2 = g2 - go/10; //3/no;		// zelen� slo�ka
							r2 = r2 - ro/10; //3/no;		// �erven� slo�ka
						}

				// omezen� p�ete�en� barvy
						if (b2 < 0) b2 = 0;
						if (b2 > 255) b2 = 255;
						if (g2 < 0) g2 = 0;
						if (g2 > 255) g2 = 255;
						if (r2 < 0) r2 = 0;
						if (r2 > 255) r2 = 255;

				// import barvy
						*dst = PalImport((BYTE)r2, (BYTE)g2, (BYTE)b2);
					}

					src -= 3;						// dal�� bod
					dst--;
				}

				src -= 3;							// p�esko�en� posledn�ho bodu
				dst--;

				src -= srcinc - 3*width;			// dal�� linka
			}
		  }
		}
*/
//		break;
/*
// 32 bit�
	case 32:
		for (i = height; i > 0; i--)		// cyklus p�es v�echny linky
		{
			if (Dither)
			{
				int* odch0 = odch + 3;			// ukazatel v bufferu odchylek

				for (j = width; j > 0; j--)		// cyklus p�es v�echny body na lince
				{

				// pozad�
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

				// st�n
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

				// po�adovan� barva
						b = *src;					// modr� slo�ka
						src++;						// zv��en� zdrojov� adresy
						g = *src;					// zelen� slo�ka
						src++;						// zv��en� zdrojov� adresy
						r = *src;					// �erven� slo�ka
						src++;						// zv��en� zdrojov� adresy
						src++;						// zv��en� zdrojov� adresy

				// zkorigovan� barva
						int b2 = b - (odch0[-3] + odch0[0] + odch0[3])*5/8;		// modr� slo�ka
						int g2 = g - (odch0[-2] + odch0[1] + odch0[4])*5/8;		// zelen� slo�ka
						int r2 = r - (odch0[-1] + odch0[2] + odch0[5])*5/8;		// �erven� slo�ka

				// omezen� p�ete�en� barvy
						if (b2 < 0) b2 = 0;
						if (b2 > 255) b2 = 255;
						if (g2 < 0) g2 = 0;
						if (g2 > 255) g2 = 255;
						if (r2 < 0) r2 = 0;
						if (r2 > 255) r2 = 255;

				// import barvy
						BYTE col = PalImport((BYTE)r2, (BYTE)g2, (BYTE)b2);

				// odchylka aktu�ln�ho bodu od po�adovan� barvy
						RGBQUAD* rgb = StdBitmapInfo->bmiColors + col;
						*odch0 = rgb->rgbBlue - b;
						odch0++;

						*odch0 = rgb->rgbGreen - g;
						odch0++;

						*odch0 = rgb->rgbRed - r;
						odch0++;

				// ulo�en� bodu
						*dst = col;	// import barvy do vlastn�ch palet
					  }
					}
					dst++;						// zv��en� c�lov� adresy
				}
			}
			else
			{
				for (j = width; j > 0; j--)		// cyklus p�es v�echny body
				{
					if ((*(int*)src & 0xffffff) == (BACKCOLOR_BLUE | (BACKCOLOR_GREEN*256) | (BACKCOLOR_RED*256*256)))
					{
						*dst = BackCol;
						src += 4;
					}
					else
					{

				// st�n
					  if ((*(int*)src & 0xffffff) == (SHADCOLOR_BLUE | (SHADCOLOR_GREEN*256) | (SHADCOLOR_RED*256*256)))
					  {
						*dst = ShadCol;
						src += 4;
					  }
					  else
					  {	

						b = *src;					// modr� slo�ka
						src++;						// zv��en� zdrojov� adresy
						g = *src;					// zelen� slo�ka
						src++;						// zv��en� zdrojov� adresy
						r = *src;					// �erven� slo�ka
						src++;						// zv��en� zdrojov� adresy
						src++;						// zv��en� zdrojov� adresy
						*dst = PalImport(r, g, b);	// import barvy do vlastn�ch palet
					  }
					}
					dst++;						// zv��en� c�lov� adresy
				}
			}
		}
		break;
	}

// uvoln�n� bufferu odchylky pro dithering
	MemFree(odch);

// uvoln�n� p��padn�ho bufferu
	MemFree(bmp2);

// odpojen� star�ch dat
	detach(pData);

// p�ipojen� nov�ch dat
	pData = newdata;

// komprimace dat (chyba nevad�)
	Comp();

// p��znak - obr�zek na�ten OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// ulo�en� do souboru form�tu BMP (komprimovan� obr�zek dekomprimuje) (FALSE=chyba)

bool CPicture::SaveFile(CText jmeno) const
{
// dekomprimace dat obr�zku
	if (!DeComp()) return false;

// vytvo�en� souboru
	CFile file;
	file.Name(jmeno);
	if (!file.Create()) return false;

// p��prava velikosti z�hlav� souboru
	int headsize = sizeof(BITMAPFILEHEADER) + 
					sizeof(BITMAPINFOHEADER) +
					StdColors*sizeof(RGBQUAD);

// p��prava bufferu pro obr�zek
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

// napln�n� z�hlav� popisova�e souboru
	BITMAPFILEHEADER* head = (BITMAPFILEHEADER*) buf;
	buf[0] = 'B';								// identifikace souboru
	buf[1] = 'M';
	head->bfSize = headsize + size;				// velikost souboru
	head->bfOffBits = headsize;					// offset dat

// napln�n� z�hlav� popisova�e dat obr�zku
	BITMAPINFOHEADER* bmp = (BITMAPINFOHEADER*)(buf + sizeof(BITMAPFILEHEADER));
	bmp->biSize = sizeof(BITMAPINFOHEADER);		// velikost struktury
	bmp->biWidth = pData->Width;				// ���ka
	bmp->biHeight = pData->Height;				// v��ka
	bmp->biPlanes = 1;							// po�et barevn�ch rovin
	bmp->biBitCount = 8;						// po�et bit� na bod
	bmp->biCompression = BI_RLE8;				// komprese
	bmp->biSizeImage = size;					// velikost dat obr�zku
	bmp->biClrUsed = StdColors;					// po�et pou�it�ch palet
	bmp->biClrImportant = StdColors;			// po�et d�le�it�ch palet

// p�enesen� palet
	MemCopy(buf + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER),
				StdBitmapInfo->bmiColors, StdColors*sizeof(RGBQUAD));

// ulo�en� souboru
	BOOL result = file.Write(buf, size + headsize);

// uzav�en� souboru
	file.Close();

// zru�en� bufferu
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
*/

/////////////////////////////////////////////////////////////////////////////
// oper�tor p�i�azen�

const CPicture& CPicture::operator= (const CPicture& src)
{
	detach(pData);				// zru�en� star�ch dat
	attach(src.pData);		// p�i�azen� nov�ch dat
	return *this;
}

const CPicture& CPicture::operator= (PICTUREDATA* src)
{
	detach(pData);				// zru�en� star�ch dat
	attach(src);				// p�i�azen� nov�ch dat
	return *this;
}


/////////////////////////////////////////////////////////////////////////////
// vygenerov�n� obr�zku pro ta�en� (p�i chyb� vrac� NULL)
/*
HIMAGELIST CPicture::GenerDrag(const CText& text)
{
// dekomprimace dat obr�zku
	if (!DeComp()) return NULL;

// p��prava bufferu textu
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

// p��prava rozm�r�
	int width = pData->Width;			// ���ka obr�zku
	int height = pData->Height;			// v��ka obr�zku
	int widtht = 0;						// ���ka textov� ��sti
	int len = 0;						// d�lka textu

// zji�t�n� ���ky textov� ��sti a d�lky textu (s omezen�m - p�i TRUECOLOR nefunguje p�i velikosti 512*32 bod�)
	if (text.IsNotEmpty())
	{
		widtht = 6 + 16;				// ���ka pro �vodn� a koncovou mezeru

		for (; ((len < text.Length()) && (widtht < (512-width-16-32))); len++)
		{
			widtht += StdFontsWidth[(BYTE)textbuf[len]] + 1;
		}

		if (height < 16) height = 16;
	}

// celkov� ���ka obr�zku s textem
	int widthc = width + widtht;		// celkov� ���ka obr�zku s textem
	int widthbyte = (widthc + 3) & ~3;	// d�lka linky barevn� bitmapy

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

// vymaz�n� bufferu (pokud bude text)
	if (widtht > 0) MemFill(data, datasize, BackCol);

// vytvo�en� dat barevn� bitmapy
	BYTE* dst = data;
	BYTE* src = pData->Data;
	for (int i = pData->Height; i > 0; i--)
	{
		MemCopy(dst, src, width);
		dst += widthbyte;
		src += width;
	}

// dek�dov�n� textu
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
	int dstinc = ((widthc + 15) & ~0xf) / 8;	// d�lka linky mono bitmapy
	BYTE* mono = (BYTE*)MemGet(dstinc * height);
	if (mono == NULL)
	{
		MemFree(data);
		return NULL;
	}

// vygenerov�n� mono bitmapy
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

// vytvo�en� mono bitmapy (maska)
	HBITMAP bmpMono = ::CreateBitmap(widthc, height, 1, 1, mono);
	ASSERT (bmpMono != NULL);
	if (bmpMono == NULL)
	{
		MemFree(mono);
		MemFree(data);
		return NULL;
	}

// otev�en� DC displeje
	HDC dc = ::GetDC(0);
	ASSERT(dc != NULL);

// v�b�r a realizace vlastn�ch palet 
	HPALETTE OldPal = ::SelectPalette(dc, StdPalette, FALSE);
	::RealizePalette(dc);

// p��prava z�hlav� BMP
	StdBitmapInfo->bmiHeader.biWidth = widthc;
	StdBitmapInfo->bmiHeader.biHeight = height;

// vytvo�en� barevn� bitmapy
	HBITMAP bmp = ::CreateDIBitmap(dc, &(StdBitmapInfo->bmiHeader),
		CBM_INIT, data, StdBitmapInfo, DIB_RGB_COLORS);
	ASSERT(bmp != NULL);

// vytvo�en� seznamu
	HIMAGELIST hImg = ::ImageList_Create(widthc, height, ILC_COLORDDB | ILC_MASK, 1, 1);
	ASSERT(hImg != NULL);

// p�id�n� bitmapy k seznamu
	int result = ::ImageList_Add(hImg, bmp, bmpMono);
	ASSERT(result != -1);

// zru�en� bitmap
	if (bmp != NULL) ::DeleteObject(bmp);
	::DeleteObject(bmpMono);

// uvoln�n� palet a DC displeje
	if (OldPal != NULL) ::SelectPalette(dc,OldPal,FALSE);
	::ReleaseDC(0,dc);

// zru�en� pracovn�ch buffer� s daty
	MemFree(mono);
	MemFree(data);

// kontrola operace
	if (result == -1)
	{
		::ImageList_Destroy(hImg);
		hImg = NULL;
	}

// navr�cen� seznamu
	return hImg;
}
*/

