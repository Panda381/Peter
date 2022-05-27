
#include "Main.h"

/***************************************************************************\
*																			*
*								Ikonov� prom�nn�							*
*																			*
\***************************************************************************/

////////////////////////////////////////////////////////////////////
// Pozn.: linky souboru BMP pro 256 barev jsou zarovn�v�ny na 4 bajty
//        linky souboru BMP pro 2 barvy (maska) jsou zarovn�ny na 2 bajty
////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// inicializa�n� pr�zdn� ikona (modifikuje se po�et referenc�!) (viz MAIN.CPP)

ICONDATA	EmptyIconData		= { 1, PicParamBack, ICONSIZE, NULL };
const CIcon		EmptyIcon;			// pr�zdn� ikona


/////////////////////////////////////////////////////////////////////////////
// statick� inicializace obsluhy ikon (p�i chyb� pam�ti vrac� FALSE)

bool InitIcon()
{
	EmptyIconData.Data = (BYTE*)MemGet(ICONSIZE);
	if (EmptyIconData.Data == NULL) return false;
	MemFill(EmptyIconData.Data, ICONSIZE, BackCol);
	return true;
}


////////////////////////////////////////////////////////////////////
// vytvo�en� dat ikony (p�i chyb� pam�ti vrac� NULL)

ICONDATA* _fastcall NewIconData(int size)
{
	ASSERT(size > 0);

// vytvo�en� z�hlav� ikony
	ICONDATA* data = (ICONDATA*)MemGet(SIZEOFICONDATA); // vytvo�en� z�hlav�
	if (data != NULL)
	{

// nastaven� dat ikony
		data->Refer = 1;					// po�et referenc�
		data->Param = PicParamNone;			// parametry (obsah nezn�m�)
		data->Size = size;					// velikost dat ikony

// vytvo�en� bufferu dat ikony
		BYTE* datadata = (BYTE*)MemGet(size);	// vytvo�en� bufferu dat ikony
		data->Data = datadata;				// adresa dat

// p�i chyb� pam�ti zru�en� z�hlav� ikony
		if (datadata == NULL)
		{
			MemFree(data);
			data = NULL;
		}
	}
	return data;
}

/////////////////////////////////////////////////////////////////////////////
// zru�en� dat ikony (odd�leno kv�li lep�� optimalizaci)

void _fastcall DelIconData(ICONDATA* data)
{
	ASSERT(data != NULL);
	ASSERT(data->Data != NULL);

	MemFree(data->Data);		// zru�en� dat ikony
	MemFree(data);				// zru�en� z�hlav� ikony
}


/////////////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CIcon::CIcon() 
{ 
	attach(&EmptyIconData); 
};

CIcon::CIcon(const CIcon& src) 
{ 
	attach(src.pData); 
};

CIcon::CIcon(ICONDATA* data)
{ 
	attach(data); 
};

CIcon::~CIcon() 
{ 
	detach(pData); 
};

/////////////////////////////////////////////////////////////////////////////
// statick� konstruktor a destruktor

void CIcon::Init()
{ 
	attach(&EmptyIconData); 
};

void _fastcall CIcon::Init(const CIcon& src) 
{ 
	attach(src.pData); 
};

void _fastcall CIcon::Init(ICONDATA* data)
{ 
	attach(data); 
};

bool _fastcall CIcon::Init(int size)	// p�i chyb� pam�ti vrac� FALSE, ikona nen� vytvo�ena
{ 
	pData = NewIconData(size);
	return (pData != NULL);
};

void CIcon::Term()
{ 
	detach(pData); 
};

/////////////////////////////////////////////////////////////////////////////
// vypr�zdn�n� ikony (uvoln�n� dat) - p�ipoj� se standardn� pr�zdn� ikona ICONSIZE

void CIcon::Empty()
{ 
	Term();
	Init();
}


////////////////////////////////////////////////////////////////////
// vymaz�n� obsahu ikony (napln�n� pr�hlednou barvou), zajist� p�ivlastn�n� (a dekomprimaci) bufferu,
// p�i chyb� pam�ti vrac� FALSE, p�vodn� obsah nezm�n�n

bool CIcon::Clear()
{
// vytvo�en� nov�ho bufferu, je-li pot�eba
	if (!New()) return false;

// vymaz�n� bufferu
	MemFill(pData->Data, pData->Size, BackCol);
	
// nastaven� parametr� na pozad�	
	pData->Param = PicParamBack;
	return true;
}


////////////////////////////////////////////////////////////////////
// vymaz�n� obsahu ikony s nastaven�m velikosti (napln�n� pr�hlednou barvou),
// zajist� p�ivlastn�n� (a dekomprimaci) bufferu, p�i chyb� pam�ti vrac� FALSE, p�vodn� obsah nezm�n�n

bool _fastcall CIcon::Clear(int size)
{
// vytvo�en� nov�ho bufferu, je-li pot�eba
	if (!New(size)) return false;

// vymaz�n� bufferu
	MemFill(pData->Data, size, BackCol);
	
// nastaven� parametr� na pozad�	
	pData->Param = PicParamBack;
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// kopie ikony do vlastn�ho bufferu p�ed modifikac� (komprimovan� data z�stanou komprimovan�)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool CIcon::CopyWrite()
{
// �schova ukazatel�
	ICONDATA* olddata = pData;		// adresa star�ch dat
	long* refer = &(olddata->Refer);// po�et referenc�

// test, zda je nutn� p�ivlastn�n�
	if (*refer > 1)					// je n�jak� jin� majitel?
	{

// p��prava velikosti bufferu s daty
		int size = olddata->Size;
		if (olddata->Param == PicParamComp)
		{
			size = *(long*)(olddata->Data) + 4;
		}

// vytvo�en� nov�ho bufferu
		ICONDATA* newdata = NewIconData(size);
		if (newdata == NULL) return false;

// p�enesen� dat
		MemCopy(newdata->Data, olddata->Data, size);
		newdata->Param = olddata->Param;	// p�enesen� parametr�
		newdata->Size = olddata->Size; // velikost ikony

// odpojen� star�ch dat
		detach(olddata);

// p�ipojen� nov�ch dat
		pData = newdata;
	}

// operace OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� nov� ikony se stejnou velikost� (p�ipraveno k z�pisu, data jsou n�hodn�)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool CIcon::New()
{
// ukazatel na star� data
	ICONDATA* olddata = pData;			// adresa star�ch dat

// test, zda je nutn� vytvo�en� nov�ho bufferu
	if ((olddata->Refer > 1) ||				// na buffer je v�ce referenc�
		(olddata->Param == PicParamComp))	// data jsou komprimovan�
	{

// vytvo�en� nov�ho bufferu
		ICONDATA* newdata = NewIconData(pData->Size);
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
// vytvo�en� nov� ikony (p�ipraveno k z�pisu, data jsou n�hodn�)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool _fastcall CIcon::New(int size)
{
// ukazatel na star� data
	ICONDATA* olddata = pData;			// adresa star�ch dat

// test, zda je nutn� vytvo�en� nov�ho bufferu
	if ((olddata->Refer > 1) ||				// na buffer je v�ce referenc�
		(olddata->Size != size) ||			// nesouhlas� velikost
		(olddata->Param == PicParamComp))	// data jsou komprimovan�
	{

// vytvo�en� nov�ho bufferu
		ICONDATA* newdata = NewIconData(size);
		if (newdata == NULL) return false;

// odpojen� star�ch dat
		detach(olddata);

// p�ipojen� nov�ch dat
		pData = newdata;
	}

// operace OK
	return true;
}


////////////////////////////////////////////////////////////////////
// poskytnut� bodu (s kontrolou platnosti offsetu) - ikona nesm� b�t komprimovan�!
// pro neplatn� offset vrac� barvu pozad�

BYTE _fastcall CIcon::Get(const int off) const
{
	ASSERT(pData->Param != PicParamComp);

	if (IsValid(off))				// je offset platn�?
	{
		return pData->Data[off];	// bod na dan�m offsetu
	}
	return BackCol;					// pro neplatn� offset barva pozad�
}


////////////////////////////////////////////////////////////////////
// nastaven� bodu (s kontrolou platnosti offsetu) - ikona nesm� b�t komprimovan�!
// p�ed z�pisem je nutno zajistit p�ivlastn�n� bufferu!

void _fastcall CIcon::Set(const int off, const BYTE data)
{
	ASSERT(pData->Param != PicParamComp);
	ASSERT(pData->Refer == 1);

	if (IsValid(off))				// je offset platn�?
	{
		pData->Data[off] = data;
	}
}


/////////////////////////////////////////////////////////////////////////////
// komprese dat ikony 32*32, vrac� velikost dat (p�i chyb� pam�ti vrac� <0, data jsou nezm�n�na)
// komprese se prov�d� ve spole�n�m bufferu pro v�echny prom�nn�!
// (v p��pad� pot�eby je nutno zajistit p�ivlastn�n� bufferu)
// p�vodn� buffer s daty je zru�en
// (velikost komprimovan�ch dat je ud�v�na bez dvojslova s velikost�, buffer s daty
// je tedy o 4 v�t��, data za��naj� na offsetu 4 v bufferu)

int CIcon::Comp() const
{
// test, zda je ikona ji� komprimov�na
	if (pData->Param == PicParamComp)
	{
		return *(long*)pData->Data;
	}

// kontrola velikosti ikony
	ASSERT(pData->Size == ICONSIZE);
	if (pData->Size != ICONSIZE) return -1;

// vytvo�en� nov�ho bufferu pro data
	BYTE* newdata = (BYTE*)MemGet(ICONSIZE*2);
	if (newdata == NULL) return -1;

// komprese dat
	BYTE* olddata = pData->Data;
	int newsize = Compress(newdata + 4, olddata, ICONSIZE, ICONWIDTH);
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
// dekomprimace dat ikony 32*32 (jsou-li komprimov�na), p�i chyb� vrac� FALSE, data jsou nezm�n�na
// dekomprese se prov�d� ve spole�n�m bufferu pro v�echny prom�nn�
// (v p��pad� pot�eby je nutno zajistit p�ivlastn�n� bufferu)
// p�vodn� buffer s komprimovan�mi daty je zru�en

bool CIcon::DeComp() const
{
	// havarie
// test, zda je ikona komprimov�na
	if (pData->Param == PicParamComp)
	{

// kontrola velikosti ikony
		ASSERT(pData->Size == ICONSIZE);
		if (pData->Size != ICONSIZE) return false;

// vytvo�en� nov�ho bufferu pro data
		BYTE* newdata = (BYTE*)MemGet(ICONSIZE);
		if (newdata == NULL) return false;

// dekomprese dat
		BYTE* olddata = pData->Data;
		DeCompress(newdata, olddata + 4, ICONSIZE, ICONWIDTH);

// zru�en� star�ho bufferu
		MemFree(olddata);

// nastaven� parametr�
		pData->Data = newdata;
		pData->Param = PicParamNone;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// kopie nov�ch dat ikony (rozm�ry z�stanou nezm�n�ny) - zajist� p�ivlastn�n� (a dekomprimaci) bufferu,
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool CIcon::CopyData(BYTE* src)
{
// vytvo�en� nov�ho bufferu, je-li pot�eba
	if (!New()) return false;

// kopie dat do bufferu
	MemCopy(pData->Data, src, pData->Size);
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// kopie komprimovan�ch nov�ch dat ikony (rozm�ry z�stanou nezm�n�ny) - zajist� odpojen� dat
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool CIcon::CompCopy(BYTE* src)
{
// �schova parametr�
	int datasize = pData->Size;
	int size = *(long*)src + 4;

// vytvo�en� nov�ho bufferu
	if (!New(size)) return false;

// nastaven� parametr�
	pData->Size = datasize;
	pData->Param = PicParamComp;

// kopie dat do bufferu
	MemCopy(pData->Data, src, size);
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// kopie nov�ch dat ikony s konverz� (rozm�ry z�stanou nezm�n�ny) - zajist� odpojen� dat
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool CIcon::CopyKonvData(BYTE* src)
{
// vytvo�en� nov�ho bufferu, je-li pot�eba
	if (!New()) return false;

// kopie dat do bufferu
	KonvCopy(pData->Data, src, pData->Size);
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// extrakt ikony ze souboru form�tu PET (p�i chyb� vr�t� FALSE, obsah nezm�n�n)

bool CIcon::Extract(CText jmeno)
{
// otev�en� souboru mapovan�ho do pam�ti (uzav�en p�i destrukci!)
	CFileMap file;
	if (!file.Open(jmeno)) return false;

// nalezen� implicitn� ikony
	BITMAPINFO* bmp = file.NTIcon();
	if (bmp == NULL) return false;

// vygenerov�n� palet pro konverzi
	GenKonvPal(bmp);

// vytvo�en� nov�ho bufferu
	if (!New(ICONSIZE)) return false;

// kopie dat ikony
	KonvCopy(pData->Data, (BYTE*)(bmp->bmiColors + 256), ICONSIZE);

// korekce pozad� ikony
	BYTE* src = (BYTE*)(bmp->bmiColors + 256) + ICONSIZE;
	BYTE* dst = pData->Data;

	for (int i = ICONSIZE/8; i > 0; i--)
	{
		BYTE maska = *src;

		if (maska & 0x80) dst[0] = BackCol;
		if (maska & 0x40) dst[1] = BackCol;
		if (maska & 0x20) dst[2] = BackCol;
		if (maska & 0x10) dst[3] = BackCol;
		if (maska & 0x08) dst[4] = BackCol;
		if (maska & 0x04) dst[5] = BackCol;
		if (maska & 0x02) dst[6] = BackCol;
		if (maska & 0x01) dst[7] = BackCol;

		src++;
		dst += 8;
	}

// p��znak operace OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� ikony ze souboru form�tu ICO (FALSE=chyba, obsah nezm�n�n)

bool CIcon::LoadFile(CText jmeno)
{
// otev�en� souboru mapovan�ho do pam�ti (uzav�en p�i destrukci!)
	CFileMap file;
	if (!file.Open(jmeno)) return false;

// kontrola velikosti souboru
	if (file.Size() < 200) return false;

// p��prava ukazatel� na ikonu
	ICONDIR* dir = (ICONDIR*)file.Adr();

// po�et ikon v souboru
	int num = dir->Count;

// kontrola platnosti ikony
	if ((dir->Type != 1) ||
		(num < 1) ||
		(num > 256) ||
		((int)(num*SIZEOFICONDIRENTRY + SIZEOFICONDIR + 100) > file.Size()))
	{
		return false;
	}

// p�ednastaven� neplatn� ikony
	int index = -1;
	int i, j;

// nalezen� ikony s rozm�ry 32x32
	ICONDIRENTRY* entry = dir->Data;
	for (i = 0; i < num; i++)
	{
		if	(	
				(entry->Width == 32) && 
				(entry->Height == 32) &&
				(
					(entry->Colors == 0) ||
					(
						(entry->Colors == 16) && 
						(index < 0)
					)
				)
			)
		{
			index = i;
		}
		entry++;
	}

// nalezen� ikony s rozm�ry 16*16
	entry = dir->Data;
	if (index < 0)
	{
		for (i = 0; i < num; i++)
		{
			if	(	
					(entry->Width == 16) && 
					(entry->Height == 16) &&
					(
						(entry->Colors == 0) ||
						(
							(entry->Colors == 16) && 
							(index < 0)
						)
					)
				)
			{
				index = i;
			}
			entry++;
		}
	}

// kontrola, zda byla ikona nalezena
	if (index < 0) return false;

// p��prava parametr� ikony
	entry = dir->Data + index;
	int width = entry->Width;
	int height = entry->Height;
	int colors = entry->Colors;
	if (colors == 0) colors = 256;
	
// p��prava velikosti z�hlav� a dat
	int sizehead = sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD);
	int sizedata = width*height + width*height/8;
	if (colors == 16)
	{
		sizehead = sizeof(BITMAPINFOHEADER) + 16*sizeof(RGBQUAD);
		sizedata = width*height/2 + width*height/8;
	}

// kontrola adresy ikony
	if (file.IsNotValid(file.Adr() + entry->Offset, sizehead + sizedata))
	{
		return false;
	}

// vygenerov�n� konverzn� tabulky palet
	BITMAPINFO* bmp = (BITMAPINFO*)MemGet(sizehead);
	if (bmp == NULL) return false;

	MemCopy(bmp, file.Adr() + entry->Offset, sizehead);

	bmp->bmiHeader.biClrUsed = 0;

	if (colors == 16)
	{
		bmp->bmiHeader.biBitCount = 4;
	}
	else
	{
		bmp->bmiHeader.biBitCount = 8;
	}

	GenKonvPal(bmp);
	MemFree(bmp);

// vytvo�en� nov�ho bufferu pro data
	if (!New(ICONSIZE)) return false;

// p��prava zdrojov� a c�lov� adresy
	BYTE* dst = pData->Data;
	BYTE* src = file.Adr() + entry->Offset + sizehead;

// konverze pro 16 barev
	BYTE col;
	if (colors == 16)
	{

// ikona 32x32
		if (width == 32)
		{
			for (i = 32*32/2; i > 0; i--)
			{
				*dst = KonvPal[*src >> 4];
				dst++;
				*dst = KonvPal[*src & 0xf];
				dst++;
				src++;
			}
		}
		else

// ikona 16x16
		{
			for (i = 16; i > 0; i--)
			{
				for (j = 8; j > 0; j--)
				{
					col = KonvPal[*src >> 4];
					dst[0] = col;
					dst[32] = col;
					dst++;
					dst[0] = col;
					dst[32] = col;
					dst++;

					col = KonvPal[*src & 0xf];
					dst[0] = col;
					dst[32] = col;
					dst++;
					dst[0] = col;
					dst[32] = col;
					dst++;

					src++;
				}
				dst += 32;
			}
		}
	}
	else

// konverze pro 256 barev
	{

// ikona 32x32
		if (width == 32)
		{
			KonvCopy(dst, src, ICONSIZE);
		}
		else

// ikona 16x16
		{
			for (i = 16; i > 0; i--)
			{
				for (j = 16; j > 0; j--)
				{
					col = KonvPal[*src];
					dst[0] = col;
					dst[32] = col;
					dst++;
					dst[0] = col;
					dst[32] = col;
					dst++;
					src++;
				}
				dst += 32;
			}
		}
	}

// p��prava zdrojov� a c�lov� adresy
	dst = pData->Data;
	src = file.Adr() + entry->Offset + sizehead + sizedata - width*height/8;

// korekce pozad� ikony pro 32x32 (pro ikony 16x16 to m��eme ignorovat)
	if (width == 32)
	{
		for (i = ICONSIZE/8; i > 0; i--)
		{
			BYTE maska = *src;

			if (maska & 0x80) dst[0] = BackCol;
			if (maska & 0x40) dst[1] = BackCol;
			if (maska & 0x20) dst[2] = BackCol;
			if (maska & 0x10) dst[3] = BackCol;
			if (maska & 0x08) dst[4] = BackCol;
			if (maska & 0x04) dst[5] = BackCol;
			if (maska & 0x02) dst[6] = BackCol;
			if (maska & 0x01) dst[7] = BackCol;

			src++;
			dst += 8;
		}
	}

// komprese (chyba se ignoruje)
	Comp();

// p��znak - ikona na�tena OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// ulo�en� ikony do souboru form�tu ICO (FALSE=chyba)

bool CIcon::SaveFile(CText jmeno) const
{
// kontrola velikosti ikony (mus� b�t 32x32)
	ASSERT(pData->Size == ICONSIZE);
	if (pData->Size != ICONSIZE) return false;

// dekomprimace dat ikony
	if (!DeComp()) return false;

// vytvo�en� souboru
	CFile file;
	file.Name(jmeno);
	if (!file.Create()) return false;

// p��prava velikosti ikony
	int size =	SIZEOFICONDIR +					// z�hlav� adres��e
				SIZEOFICONDIRENTRY +			// prvn� adres��ov� polo�ka
				sizeof(BITMAPINFOHEADER) +		// z�hlav� BMP
				256 * sizeof(RGBQUAD) +			// tabulka palet
				ICONSIZE +						// velikost dat ikony
				ICONSIZE/8;						// velikost masky

// buffer pro ikonu
	BYTE* buf = (BYTE*)MemGet(size);
	if (buf == NULL)
	{
		file.Close();
		file.Delete();
		return false;
	}

// vymaz�n� bufferu
	MemFill(buf, size, 0);

// vypln�n� z�hlav� adres��e
	ICONDIR* dir = (ICONDIR*)buf;
	dir->Type = 1;								// typ souboru = ikona
	dir->Count = 1;								// po�et polo�ek v adres��i

// vypln�n� adres��ov� polo�ky
	ICONDIRENTRY* entry = dir->Data;
	entry->Width = ICONWIDTH;					// ���ka ikony
	entry->Height = ICONHEIGHT;					// v��ka ikony
	entry->Size = size - SIZEOFICONDIR - SIZEOFICONDIRENTRY;
	entry->Offset = SIZEOFICONDIR + SIZEOFICONDIRENTRY;

// vypln�n� z�hlav� bitmapy
	BITMAPINFOHEADER* bmp = (BITMAPINFOHEADER*)(buf + SIZEOFICONDIR + SIZEOFICONDIRENTRY);
	bmp->biSize = sizeof(BITMAPINFOHEADER);		// velikost z�hlav�
	bmp->biWidth = ICONWIDTH;					// ���ka obr�zku
	bmp->biHeight = ICONHEIGHT*2;				// v��ka obr�zku a masky
	bmp->biPlanes = 1;							// po�et barevn�ch rovin
	bmp->biBitCount = 8;						// po�et bit� na bod
	bmp->biSizeImage = ICONSIZE;				// velikost obr�zku

// ulo�en� palet (bez barvy pozad� 0 - ta z�stane nastavena na 0, tj �ern�)
	MemCopy(buf + SIZEOFICONDIR + SIZEOFICONDIRENTRY + sizeof(BITMAPINFOHEADER) +
			sizeof(RGBQUAD), StdBitmapInfo->bmiColors + 1, 255*sizeof(RGBQUAD));

// ulo�en� dat ikony
	BYTE* src = buf + SIZEOFICONDIR + SIZEOFICONDIRENTRY + 
						sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD);
	MemCopy(src, pData->Data, ICONSIZE);

// vygenerov�n� masky
	BYTE* dst = src + ICONSIZE;

	for (int i = ICONSIZE/8; i > 0; i--)
	{
		BYTE stradac = 0;

		if (src[0] == BackCol) stradac  = 0x80;
		if (src[1] == BackCol) stradac |= 0x40;
		if (src[2] == BackCol) stradac |= 0x20;
		if (src[3] == BackCol) stradac |= 0x10;
		if (src[4] == BackCol) stradac |= 0x08;
		if (src[5] == BackCol) stradac |= 0x04;
		if (src[6] == BackCol) stradac |= 0x02;
		if (src[7] == BackCol) stradac |= 0x01;

		src += 8;
		*dst = stradac;
		dst++;
	}

// ulo�en� souboru
	BOOL result = file.Write(buf, size);

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

// p��znak operace OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// oper�tor p�i�azen�

const CIcon& CIcon::operator= (const CIcon& src)
{
	detach(pData);			// zru�en� star�ch dat
	attach(src.pData);		// p�i�azen� nov�ch dat
	return *this;
}

const CIcon& CIcon::operator= (ICONDATA* src)
{
	detach(pData);			// zru�en� star�ch dat
	attach(src);			// p�i�azen� nov�ch dat
	return *this;
}


/***************************************************************************\
*																			*
*								Buffer ikon									*
*																			*
\***************************************************************************/

////////////////////////////////////////////////////////////////////
// p�id�n� z�znamu o vlo�en� polo�ky do UNDO bufferu

template <> bool CBuffer<CIcon>::UndoAddIns(int index)
{
	return Undo.AddIconIns(index);
}

////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CBufIcon::CBufIcon()
{
	m_Width = ICONWIDTH;				// ���ka jedn� ikony
	m_Height = ICONHEIGHT;				// v��ka jedn� ikony
	m_WidthByte = ICONWIDTH;			// d�lka jedn� linky v bajtech
	m_IconSize = ICONSIZE;				// velikost ikony v bajtech
}

/////////////////////////////////////////////////////////////////////////////
// statick� konstruktor a destruktor

void CBufIcon::Init()
{
	CBuffer<CIcon>::Init();
	m_Width = ICONWIDTH;				// ���ka jedn� ikony
	m_Height = ICONHEIGHT;				// v��ka jedn� ikony
	m_WidthByte = ICONWIDTH;			// d�lka jedn� linky v bajtech
	m_IconSize = ICONSIZE;				// velikost ikony v bajtech
}

////////////////////////////////////////////////////////////////////
// nastaven� velikosti ikon (p��padn� existuj�c� ikony zru��!)

void _fastcall CBufIcon::IconSize(const int width, const int height)
{
	DelAll();							// zru�en� existuj�c�ch ikon

	m_Width = width;					// ���ka jedn� ikony
	m_Height = height;					// v��ka jedn� ikony
	m_WidthByte = (width + 3) & ~3;		// d�lka linky jedn� ikony
	m_IconSize = m_WidthByte * height;	// velikost ikony v bajtech

	m_EmptyItem.Clear(m_IconSize);		// nov� data pr�zdn� ikony
}

////////////////////////////////////////////////////////////////////
// p�id�n� polo�ky (vrac� index polo�ky, <0 = chyba pam�ti)
// prov�d� z�znam do UNDO bufferu

int _fastcall CBufIcon::Add(ICONDATA* data)
{
	CIcon icon(data);
	int result = Add(icon);		// result mus� b�t odd�len, pro inline by se �patn� volal destruktor
	return result;
}

////////////////////////////////////////////////////////////////////
// na�ten� ikon z resource (vynulovat buffer, aby bylo ukl�d�no po �ad�!), vrac� TRUE=operace OK

bool CBufIcon::Load(const int nID, int num)
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

// p��prava konverzn� tabulky palet
	GenKonvPal(bmp);

// p��prava po�tu ikon horizont�ln�
	int numX = bmp->bmiHeader.biWidth/m_Width;
	ASSERT((bmp->bmiHeader.biWidth % m_Width) == 0);

// p��prava p��rustku zdrojov� adresy mezi linkami
	int srcinc = (bmp->bmiHeader.biWidth + 3) & ~3;

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

// p��prava zdrojov� adresy dat (za prav�m horn�m rohem)
	BYTE* src = (BYTE*)(bmp->bmiColors + colors) + srcinc*bmp->bmiHeader.biHeight + m_Width*numX;

// cyklus na�ten� ikon
	int citX = 1;
	for (; num > 0; num--)
	{

// posun adresy na dal�� ��dek ikon
		if (--citX == 0)
		{
			src -=	srcinc*m_Height + m_Width*numX;
			citX = numX;
		}

// vytvo�en� nov� polo�ky ikony
		int item = New();
		if (item < 0)
		{
			MemFree(bmp2);
			return false;
		}

// zaji�t�n� p�ivlastn�n� bufferu (bez uchov�n� p�vodn�ch dat)
		if (!m_Data[item].New())
		{
			Del(item);
			MemFree(bmp2);
			return false;
		}

// p�enos dat ikony
		BYTE* dst = m_Data[item].DataData();
		for (int height = m_Height; height > 0; height--)
		{
			KonvCopy(dst, src, m_Width);
			dst += m_WidthByte;
			src += srcinc;
		}

		if ((m_Height == ICONHEIGHT) && (m_Width == ICONWIDTH))
		{
			m_Data[item].Comp();
		}

// zv��en� adresy ikony
		src += m_Width - m_Height*srcinc;
	}

// p��padn� zru�en� bufferu
	MemFree(bmp2);

	return true;
}


////////////////////////////////////////////////////////////////////
// vygenerov�n� dat bitmapy (vrac� ukazatel na buffer s rezervou 8 bajt� na konci, p�i chyb� vrac� NULL)

BYTE* CBufIcon::GenerBitmap(SMALLICON smallicon)
{
// p��prava ukazatel� pro norm�ln� ikonu
	int	widthbyte = m_WidthByte; // d�lka linky ikony v bajtech
	int	srcinc = widthbyte;		// p��rustek zdrojov� adresy
	int	width2 = m_Width;		// c�lov� ���ka ikony
	int	height2 = m_Height;		// c�lov� v��ka ikony

// p��prava ukazatel� pro malou ikonu
	switch (smallicon)
	{
	case SI_SMALL:
		width2 = m_Width/2;
		height2 = m_Height/2;
		srcinc = 2*widthbyte - m_Width;
		break;

	case SI_MEDIUM:
		width2 = m_Width*3/4;
		height2 = m_Height*3/4;
		srcinc = 4*widthbyte - m_Width;
		break;
	}
	int dstinc = (m_Num*width2 + 3) & ~3; // p��rustek c�lov� adresy (d�lka c�lov� linky)

// vytvo�en� bufferu (s rezervou 8 bajt� na konci)
	BYTE* buffer = (BYTE*)MemGet(dstinc*height2 + 8);
	if (buffer == NULL) return NULL;

// ukazatel c�lov� adresy
	BYTE* dst = buffer;					// ukazatel c�lov� adresy

// cyklus p�es v�echny ikony
	int i, j, k;
	for (i = 0; i < m_Max; i++)
	{

// test, zda je ikona platn�
		if (m_Valid[i])
		{

// p��prava zdrojov� adresy dat ikony
			m_Data[i].DeComp();
			BYTE* src = m_Data[i].DataData();

			switch (smallicon)
			{
// mal� ikona
			case SI_SMALL:
// cyklus p�es v�echny linky jedn� ikony
				for (j = height2; j > 0; j--)
				{
// kopie jedn� linky ikony
					for (k = width2; k > 0; k--)
					{
						*dst = ColAvrg(src[0], src[1], src[widthbyte], src[widthbyte+1]);
						src++;
						src++;
						dst++;
					}

// zv��en� adres na dal�� linku
					src += srcinc;
					dst += dstinc - width2;
				}
				break;

			case SI_MEDIUM:
				for (j = height2/3; j > 0; j--)
				{

// kopie jedn� linky ikony
					for (k = width2/3; k > 0; k--)
					{
						dst[0] = src[0];
						dst[1] = ColAvrg(src[1], src[2], src[1], src[2]);
						dst[2] = ColAvrg(src[2], src[3], src[2], src[3]);
						
						dst[0+dstinc] = ColAvrg(src[widthbyte], src[2*widthbyte], src[widthbyte], src[2*widthbyte]);
						dst[1+dstinc] = ColAvrg(src[1+widthbyte], src[1+2*widthbyte], src[2+widthbyte], src[2+2*widthbyte]);
						dst[2+dstinc] = ColAvrg(src[2+widthbyte], src[2+2*widthbyte], src[3+widthbyte], src[3+2*widthbyte]);

						dst[0+2*dstinc] = ColAvrg(src[2*widthbyte], src[3*widthbyte], src[2*widthbyte], src[3*widthbyte]);
						dst[1+2*dstinc] = ColAvrg(src[1+2*widthbyte], src[1+3*widthbyte], src[2+2*widthbyte], src[2+3*widthbyte]);
						dst[2+2*dstinc] = ColAvrg(src[2+2*widthbyte], src[2+3*widthbyte], src[3+2*widthbyte], src[3+3*widthbyte]);

						src += 4;
						dst += 3;
					}

// zv��en� adres na dal�� linku
					src += srcinc;
					dst += dstinc*3 - width2;
				}
				break;

// velk� ikona
			default:

// cyklus p�es v�echny linky jedn� ikony
				for (j = height2; j > 0; j--)
				{
					MemCopy(dst, src, width2);

// zv��en� adres na dal�� linku
					src += srcinc;
					dst += dstinc;
				}
			}

// posun ukl�dac� adresy na dal�� ikonu
			dst += width2 - height2*dstinc;
		}
	}

// adresa bufferu s daty
	return buffer;
}


/////////////////////////////////////////////////////////////////////////////
// vygenerov�n� masky ikon z dat barevn� bitmapy (smallicon = polovi�n� velikost)
// barevn� bitmapa mus� m�t na konci rezervu pro zarovn�n� linky na 8 bajt� !
// vrac� adresu na buffer, p�i chyb� vrac� NULL

BYTE* CBufIcon::GenerMask(BYTE* bitmapa, SMALLICON smallicon)
{
// p��prava ukazatel� pro norm�ln� ikonu
	int width2 = m_Width;				// c�lov� ���ka ikony
	int height2 = m_Height;				// c�lov� v��ka ikony

// p��prava ukazatel� pro malou ikonu
	switch (smallicon)
	{
	case SI_SMALL:
		width2 /= 2;
		height2 /= 2;
		break;

	case SI_MEDIUM:
		width2 = m_Width*3/4;
		height2 = m_Height*3/4;
		break;
	}

// d�lka linky v bodech
	int width = m_Num*width2;

// p��rustek zdrojov� adresy
	int srcinc = (width + 3) & ~3;

// p��rustek c�lov� adresy
	int dstinc = ((width + 15) & ~0xf) / 8;

// vytvo�en� bufferu pro masku
	BYTE* data = (BYTE*)MemGet(dstinc * height2);
	if (data == NULL) return NULL;

// ukazatel c�lov� adresy
	BYTE* dst = data;

// cyklus p�es v�echny linky
	for (int i = height2-1; i >= 0; i--)
	{

// ukazatel zdrojov� adresy
		BYTE* src = bitmapa + srcinc*i;

// cyklus p�es v�echny bajty c�lov� linky
		for (int j = dstinc; j > 0; j--)
		{

// vygenerov�n� bajtu pro 8 bod� (m��e p�es�hnout za konec linky!)
			BYTE stradac = 0;
			if (src[0] == BackCol) stradac  = 0x80;
			if (src[1] == BackCol) stradac |= 0x40;
			if (src[2] == BackCol) stradac |= 0x20;
			if (src[3] == BackCol) stradac |= 0x10;
			if (src[4] == BackCol) stradac |= 0x08;
			if (src[5] == BackCol) stradac |= 0x04;
			if (src[6] == BackCol) stradac |= 0x02;
			if (src[7] == BackCol) stradac |= 0x01;
			src += 8;

// ulo�en� c�lov�ho bajtu
			dst[0] = stradac;			// ulo�en� st�ada�e bit� masky
			dst++;						// zv��en� ukl�dac� adresy masky
		}
	}

// adresa bufferu s daty
	return data;
}

/////////////////////////////////////////////////////////////////////////////
// vygenerov�n� seznamu ikon (drag = ta�en�, smallicon = polovi�n� velikost,
//		himg = pou�it� seznam, pro himg=NULL vytvo�� nov� seznam)
// p�i chyb� vrac� NULL, p�vodn� seznam ikon p�itom z�st�v� nezm�n�n

HIMAGELIST CBufIcon::GenerList(bool drag, SMALLICON smallicon, HIMAGELIST himg)
{
// p��prava c�lov� ���ky a v��ky ikony
	int width2 = m_Width;				// c�lov� ���ka ikony
	int height2 = m_Height;				// c�lov� v��ka ikony
	switch (smallicon)
	{
	case SI_SMALL:
		width2 /= 2;
		height2 /= 2;
		break;

	case SI_MEDIUM:
		width2 = m_Width*3/4;
		height2 = m_Height*3/4;
		break;
	}

// vytvo�en� dat barevn� a mono bitmapy
	BYTE* data = GenerBitmap(smallicon);	// vygenerov�n� barevn� bitmapy
	if (data == NULL) return NULL;			// chyba pam�ti
	BYTE* mono = GenerMask(data, smallicon); // vygenerov�n� masky bitmapy
	if (mono == NULL)					// chyba pam�ti
	{
		MemFree(data);					// zru�en� bufferu barevn� bitmapy
		return NULL;
	}

// vytvo�en� nov�ho seznamu ikon
	bool newimg = false;				// p��znak vytvo�en� nov�ho seznamu
	if (himg == NULL)
	{
		himg = ::ImageList_Create(width2, height2, ILC_COLORDDB | ILC_MASK,m_Num,50);
		newimg = true;
	}
	ASSERT(himg);	

	if (himg == NULL)
	{
		MemFree(data);
		MemFree(mono);
		return NULL;
	}

// korekce na obr�zky ta�en�
	if (drag)
	{
		BYTE* dst = mono;
		BYTE maska = 0xAA;

		for (int j = height2; j > 0; j--)
		{
			for (int i = (((m_Num*width2) + 15) & ~0xf) /8; i > 0; i--)
			{
				dst[0] |= maska;
				dst++;
			}
			maska ^= 0xff;
		}
	}

// vytvo�en� mono bitmapy (maska)
	HBITMAP bmpMono = ::CreateBitmap(width2*m_Num, height2, 1, 1, mono);
	ASSERT(bmpMono != NULL);
	if (bmpMono == NULL)
	{
		if (newimg) ::ImageList_Destroy(himg);
		MemFree(data);
		MemFree(mono);
		return NULL;
	}

// otev�en� DC displeje
	HDC dc = ::GetDC(0);
	ASSERT(dc != NULL);

// v�b�r a realizace vlastn�ch palet 
	HPALETTE OldPal = ::SelectPalette(dc, StdPalette, FALSE);
	::RealizePalette(dc);

// p��prava z�hlav� BMP
	StdBitmapInfo->bmiHeader.biWidth = width2*m_Num;
	StdBitmapInfo->bmiHeader.biHeight = height2;

// vytvo�en� barevn� bitmapy
	HBITMAP bmp = ::CreateDIBitmap(dc,&(StdBitmapInfo->bmiHeader),
		CBM_INIT, data, StdBitmapInfo, DIB_RGB_COLORS);
	ASSERT(bmp != NULL);

// p�id�n� bitmapy k seznamu
	int result = ::ImageList_Add(himg, bmp, bmpMono);
	ASSERT(result != -1);

// zru�en� bitmap
	if (bmp != NULL) ::DeleteObject(bmp);
	::DeleteObject(bmpMono);

// uvoln�n� palet a DC displeje
	if (OldPal != NULL) ::SelectPalette(dc,OldPal,FALSE);
	::ReleaseDC(0,dc);

// zru�en� pracovn�ch buffer� s daty
	MemFree(data);
	MemFree(mono);

// chyba
	if (result == -1)
	{
		if (newimg) ::ImageList_Destroy(himg);
		return NULL;
	}

// navr�cen� seznamu ikon
	return himg;
}
