
#include "Main.h"

/***************************************************************************\
*																			*
*								Ikonov� prom�nn�							*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inicializa�n� pr�zdn� ikona (modifikuje se po�et referenc�!) (viz MAIN.CPP)

ICONDATA	EmptyIconData		= { 1, PicParamNone, NULL, NULL, 0, NULL};


/////////////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CIcon::CIcon() 
{ 
	Attach(&EmptyIconData); 
};

CIcon::CIcon(const CIcon& src) 
{ 
	Attach(src.pData); 
};

CIcon::~CIcon() 
{ 
	Detach(); 
};

/////////////////////////////////////////////////////////////////////////////
// statick� konstruktor a destruktor

void CIcon::Init()
{ 
	Attach(&EmptyIconData); 
};

void CIcon::Init(ICONDATA* data)
{ 
	Attach(data); 
};

void CIcon::InitNew()
{ 
	NewBuffer(); 
};

void CIcon::Term()
{ 
	Detach(); 
};


/////////////////////////////////////////////////////////////////////////////
// kopie nov�ch dat ikony - zajist� odpojen� dat

void CIcon::CopyData(BYTE* src)
{
	ASSERT(src);
	ICONDATA* data = pData;			// adresa star�ch dat (z�hlav�)
	long* refer = &(data->Refer);	// po�et referenc�

	if (*refer > 1)					// je n�jak� jin� majitel?
	{
		NewBuffer();				// vytvo�en� nov�ho bufferu

// odpojen� star�ch dat - v multithread m��e nastat i zru�en�
		if (LongDecrement(refer))
		{
#ifdef _MT
			if (data->HIcon != NULL)
			{
				::DestroyIcon(data->HIcon);
			}

			if (data->Cursor != NULL)
			{
				::DestroyCursor(data->Cursor);
			}

			MemFree(data->Data);	// p��padn� zru�en� dat
			MemFree(data);			// p��padn� zru�en� z�hlav�
#endif	// _MT
		}
	}
	else
	{
		if (data->HIcon != NULL)
		{
			::DestroyIcon(data->HIcon);
			data->HIcon = NULL;
		}

		if (data->Cursor != NULL)
		{
			::DestroyCursor(data->Cursor);
			data->Cursor = NULL;
		}
	}

	MemCopy(pData->Data, src, ICONSIZE); // kopie obsahu ikony
}


/////////////////////////////////////////////////////////////////////////////
// dekomprimace dat ikony (jsou-li komprimov�na)

void CIcon::DeComp()
{
	if (pData->Param == PicParamComp)
	{
		BYTE* data = (BYTE*)MemGet(ICONSIZE);
		DeCompress(data, pData->Data + 4, ICONSIZE, ICONWIDTH);
		pData->Data = data;
		pData->Refer -= BigInt;
		pData->Param = PicParamNone;
	}
}



/////////////////////////////////////////////////////////////////////////////
// kopie nov�ch dat ikony s konverz� - zajist� odpojen� dat

void CIcon::CopyKonvData(BYTE* src)
{
	ASSERT(src);
	ICONDATA* data = pData;			// adresa star�ch dat (z�hlav�)
	long* refer = &(data->Refer);	// po�et referenc�

	if (*refer > 1)					// je n�jak� jin� majitel?
	{
		NewBuffer();				// vytvo�en� nov�ho bufferu

// odpojen� star�ch dat - v multithread m��e nastat i zru�en�
		if (LongDecrement(refer))
		{
#ifdef _MT
			if (data->HIcon != NULL)
			{
				::DestroyIcon(data->HIcon);
			}

			if (data->Cursor != NULL)
			{
				::DestroyCursor(data->Cursor);
			}
			MemFree(data->Data);	// p��padn� zru�en� dat
			MemFree(data);			// p��padn� zru�en� z�hlav�
#endif	// _MT
		}
	}
	else
	{
		if (data->HIcon != NULL)
		{
			::DestroyIcon(data->HIcon);
			data->HIcon = NULL;
		}

		if (data->Cursor != NULL)
		{
			::DestroyCursor(data->Cursor);
			data->Cursor = NULL;
		}
	}

	KonvCopy(pData->Data, src, ICONSIZE); // kopie obsahu ikony
}


/////////////////////////////////////////////////////////////////////////////
// kopie ikony do vlastn�ho bufferu p�ed modifikac�

void CIcon::CopyWrite()
{
	DeComp();
	ICONDATA* data = pData;			// adresa star�ch dat (z�hlav�)
	long* refer = &(data->Refer);	// po�et referenc�

	if (*refer > 1)					// je n�jak� jin� majitel?
	{
		NewBuffer();				// vytvo�en� nov�ho bufferu
		MemCopy(pData->Data, data->Data, ICONSIZE);
		pData->Param = data->Param;	// p�enesen� parametr�

// odpojen� star�ch dat - v multithread m��e nastat i zru�en�
		if (LongDecrement(refer))
		{
#ifdef _MT
			if (data->HIcon != NULL)
			{
				::DestroyIcon(data->HIcon);
			}

			if (data->Cursor != NULL)
			{
				::DestroyCursor(data->Cursor);
			}
			MemFree(data->Data);	// p��padn� zru�en� dat
			MemFree(data);			// p��padn� zru�en� z�hlav�
#endif	// _MT
		}
	}
	else
	{
		if (data->HIcon != NULL)
		{
			::DestroyIcon(data->HIcon);
			data->HIcon = NULL;
		}

		if (data->Cursor != NULL)
		{
			::DestroyCursor(data->Cursor);
			data->Cursor = NULL;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// vypr�zdn�n� ikony (uvoln�n� dat)

void CIcon::Empty()
{ 
	Detach(); 
	Attach(&EmptyIconData); 
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� nov� ikony (p�ipraveno k z�pisu, data jsou n�hodn�)

void CIcon::New()
{
	if (pData->Refer != 1)
	{
		Detach();						// odpojen� star� ikony
		NewBuffer();					// vytvo�en� nov�ho bufferu
	}
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� ikony ze souboru

#pragma warning ( disable: 4701)		// hl�en� - neinicializovan� prom�nn�
void CIcon::LoadFile()
{
#ifndef _MINI

// �schova offsetu souboru
	int oldoff = FileReadOff;

// na�ten� z�hlav� souboru
	ICONDIR dir;
	dir.Type = 0;
	FileReadBlok((BYTE*)&dir, SIZEOFICONDIR);

// kontrola z�hlav� souboru
	int count = dir.Count;
	if ((dir.Reserved != 0) ||
		(dir.Type != 1) ||
		(count <= 0) ||
		(count > 255))
	{
		FileReadOff = oldoff;
		FileError = true;
		return;
	}

// p�ednastaven� neplatn� ikony
	int off = -1;
	int width, colors;
	int i;
	int newoff = FileReadOff;

// nalezen� ikony s rozm�ry 32x32
	for (i = 0; i < count; i++)
	{

// na�ten� adres��ov� polo�ky
		FileReadBlok((BYTE*)dir.Data, SIZEOFICONDIRENTRY);

// test, zda to je hledan� ikona
		if	(	
				(dir.Data[0].Width == 32) && 
				(dir.Data[0].Height == 32) &&
				(
					(dir.Data[0].Colors == 0) ||
					(
						(dir.Data[0].Colors == 16) && 
						(off < 0)
					)
				)
			)
		{
			colors = dir.Data[0].Colors;
			off = oldoff + dir.Data[0].Offset;
			width = 32;
		}

// posun offsetu konce
		int j = oldoff + dir.Data[0].Offset + dir.Data[0].Size;
		if (j > newoff) newoff = j;
	}

// nalezen� ikony s rozm�ry 16*16
	if (off < 0)
	{
		FileReadOff = oldoff + SIZEOFICONDIR;
		for (i = 0; i < count; i++)
		{
// na�ten� adres��ov� polo�ky
			FileReadBlok((BYTE*)dir.Data, SIZEOFICONDIRENTRY);

// test, zda to je hledan� ikona
			if	(	
					(dir.Data[0].Width == 16) && 
					(dir.Data[0].Height == 16) &&
					(
						(dir.Data[0].Colors == 0) ||
						(
							(dir.Data[0].Colors == 16) && 
							(off < 0)
						)
					)
				)
			{
				colors = dir.Data[0].Colors;
				off = oldoff + dir.Data[0].Offset;
				width = 16;
			}
		}
	}

// kontrola, zda byla ikona nalezena
	if (off < 0)
	{
		FileReadOff = oldoff;
		FileError = true;
		return;
	}
	if (colors == 0) colors = 256;

// p��prava velikosti z�hlav� a dat
	int sizehead = sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD);
	int sizedata = width*width + width*width/8;
	if (colors == 16)
	{
		sizehead = sizeof(BITMAPINFOHEADER) + 16*sizeof(RGBQUAD);
		sizedata = width*width/2 + width*width/8;
	}

// vytvo�en� nov�ho bufferu pro data
	Detach();
	NewBuffer();

// na�ten� dat do pomocn�ho buferu
	FileReadOff = off;
	int size = sizehead + sizedata;
	BYTE* buf = (BYTE*)MemGet(size);
	FileReadBlok(buf, size);
	FileReadOff = newoff;

// vygenerov�n� konverzn� tabulky palet
	((BITMAPINFO*)buf)->bmiHeader.biClrUsed = 0;
	if (colors == 16)
	{
		((BITMAPINFO*)buf)->bmiHeader.biBitCount = 4;
	}
	else
	{
		((BITMAPINFO*)buf)->bmiHeader.biBitCount = 8;
	}
	GenKonvPal((BITMAPINFO*)buf);

// p��prava zdrojov� a c�lov� adresy
	BYTE* dst = pData->Data;
	BYTE* src = buf + sizehead;

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
				for (int j = 8; j > 0; j--)
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
				for (int j = 16; j > 0; j--)
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
	src = buf + sizehead + sizedata - width*width/8;

// korekce pozad� ikony pro 32x32
	BYTE maska;

	if (width == 32)
	{
		for (i = ICONSIZE/8; i > 0; i--)
		{
			maska = *src;

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

// uvoln�n� bufferu
	MemFree(buf);				// uvoln�n� bufferu

#endif // _MINI
}
#pragma warning ( default: 4701)		// hl�en� - neinicializovan� prom�nn�


/////////////////////////////////////////////////////////////////////////////
// ulo�en� ikony do souboru

void CIcon::SaveFile()
{
#ifndef _MINI

	DeComp();

// p��prava velikosti ikony
	int size =	SIZEOFICONDIR +					// z�hlav� adres��e
				SIZEOFICONDIRENTRY +			// prvn� adres��ov� polo�ka
				sizeof(BITMAPINFOHEADER) +		// z�hlav� BMP
				256 * sizeof(RGBQUAD) +			// tabulka palet
				ICONSIZE +						// velikost dat ikony
				ICONSIZE/8;						// velikost masky

// buffer pro ikonu
	BYTE* buf = (BYTE*)MemGet(size);

// vymaz�n� bufferu
	MemFill(buf, size);

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
	int i;
	BYTE stradac;

	for (i = ICONSIZE/8; i > 0; i--)
	{
		stradac = 0;

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
	FileWriteBlok(buf, size);

// zru�en� bufferu
	MemFree(buf);

#endif // _MINI
}


/////////////////////////////////////////////////////////////////////////////
// ulo�en� ikony do souboru form�tu ICO (false=chyba)

bool CIcon::SaveFile(CString jmeno)
{
	DeComp();

// otev�en� souboru
	HANDLE hFile = ::CreateFile(jmeno, GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

// test, zda byl soubor vytvo�en
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

// p��prava velikosti ikony
	int size =	SIZEOFICONDIR +					// z�hlav� adres��e
				SIZEOFICONDIRENTRY +			// prvn� adres��ov� polo�ka
				sizeof(BITMAPINFOHEADER) +		// z�hlav� BMP
				256 * sizeof(RGBQUAD) +			// tabulka palet
				ICONSIZE +						// velikost dat ikony
				ICONSIZE/8;						// velikost masky

// buffer pro ikonu
	BYTE* buf = (BYTE*)MemGet(size);

// vymaz�n� bufferu
	MemFill(buf, size);

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
	int i;
	BYTE stradac;

	for (i = ICONSIZE/8; i > 0; i--)
	{
		stradac = 0;

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
	DWORD write;
	BOOL result = ::WriteFile(hFile, buf, size, &write, NULL);

// uzav�en� souboru
	::CloseHandle(hFile);

// zru�en� bufferu
	MemFree(buf);

// p�i chyb� zru�en� ikony
	if (!result || (write != (DWORD)size))
	{
		::DeleteFile(jmeno);
		return false;
	}

// p��znak - ikona ulo�ena OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� ikony WINDOWS

HICON CIcon::HIcon()
{
// nen� ikona vytvo�ena?
	if (pData->HIcon == NULL)
	{

// p��prava jm�na p�echodn�ho souboru
		CString name;
		name.TempName();

// ulo�en� ikony do souboru
		if (SaveFile(name))
		{

// vytvo�en� ikony
			pData->HIcon = ::ExtractIcon(hInstance, name, 0);

// chyba
			if ((int)pData->HIcon == 1) pData->HIcon = NULL;

// zru�en� p�echodn�ho souboru
			::DeleteFile(name);
		}
	}
	return pData->HIcon;
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� kurzoru my�i

HCURSOR CIcon::Cursor()
{
// nen� kurzor vytvo�en?
	if (pData->Cursor == NULL)
	{

// dekomprimace ikony
		DeComp();

// vytvo�en� pracovn�ch buffer�
		BYTE* src = pData->Data;					// zdrojov� adresa dat
		BYTE* dstAND = (BYTE*)MemGet(ICONSIZE/8);	// maska AND
		BYTE* dstand = dstAND + (ICONHEIGHT-1)*ICONWIDTH/8;	// adresa posledn� linky AND
		BYTE* dstXOR = (BYTE*)MemGet(ICONSIZE/8);	// maska XOR
		BYTE* dstxor = dstXOR + (ICONHEIGHT-1)*ICONWIDTH/8; // adresa posledn� linky XOR
		int hotX = ICONWIDTH/2;						// hork� bod X
		int hotY = ICONHEIGHT/2;					// hork� bod Y

// cyklus p�es v�echny linky
		for (int i = ICONHEIGHT - 1; i >= 0; i--)
		{

// cyklus p�es v�echny bajty na lince
			for (int j = ICONWIDTH/8 - 1; j >= 0; j--)
			{

// p��prava st�ada�� a masky
				BYTE stradacAND = 0xff;					// st�ada� bit� AND
				BYTE stradacXOR = 0;					// st�ada� bit� XOR
				BYTE maska = 0x80;						// bitov� maska bodu

// cyklus p�es bity bajtu
				for (int k = 7; k >= 0; k--)
				{

// na�ten� barvy
					BYTE barva = *src;
					src++;

// nen� pr�hledn� barva ?
					if (barva != BackCol)
					{

// b�l� a �ern� barva - b�n� barva
						if (barva >= WhiteCol)
						{
							stradacAND ^= maska;		// bude AND
							if (barva < (StdColors-ColLev))
							{
								stradacXOR |= maska;	// bude XOR (b�l� barva)
							}
						}
						else
						{

// �lut�, zelen� a modr� barva - hork� bod
							if (barva >= YellowCol - ColLev)
							{
								hotX = ICONWIDTH - j*8 - k;
								hotY = i;
							}

// �erven� a �lut� barva - inverze
							if (barva <= YellowCol + ColLev)
							{
								stradacXOR |= maska;	// bude XOR
							}
							
							else
							{

// zelen� barva - platn� data (sv�tl�/tmav�), modr� barva - pr�hledn�
								if (barva < BlueCol - ColLev)
								{
									stradacAND ^= maska;	// bude AND
									if (((barva - ResCols) % ColLev) < (ColLev/2))
									{
										stradacXOR |= maska;	// bude XOR (b�l� barva)
									}
								}
							}
						}
					}

// posun masky na dal�� bit
					maska >>= 1;
				}

// ulo�en� bajtu (osmice bod�)
				*dstand = stradacAND;
				dstand++;
				*dstxor = stradacXOR;
				dstxor++;

			}

// posun adresy na dal�� linku
			dstand -= 2*ICONWIDTH/8;
			dstxor -= 2*ICONWIDTH/8;
		}

// vytvo�en� kurzoru
		pData->Cursor = ::CreateCursor(hInstance, hotX, hotY, ICONWIDTH, ICONHEIGHT, dstAND, dstXOR);
		ASSERT(pData->Cursor != NULL);

// zru�en� buffer�
		MemFree(dstAND);
		MemFree(dstXOR);
	}
	return pData->Cursor;
}


/////////////////////////////////////////////////////////////////////////////
// oper�tor p�i�azen�

const CIcon& CIcon::operator= (const CIcon& src)
{
	Detach();				// zru�en� star�ch dat
	Attach(src.pData);		// p�i�azen� nov�ch dat
	return *this;
}

const CIcon& CIcon::operator= (ICONDATA* src)
{
	Detach();				// zru�en� star�ch dat
	Attach(src);			// p�i�azen� nov�ch dat
	return *this;
}


/***************************************************************************\
*																			*
*								Buffer ikon									*
*																			*
\***************************************************************************/


////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CBufIcon::CBufIcon()
{
	m_Data = NULL;						// nen� buffer dat
	m_Num = 0;							// nen� ��dn� platn� polo�ka
	m_Max = 0;							// nen� buffer polo�ek
}

CBufIcon::~CBufIcon()
{
	DelAll();							// zru�en� v�ech polo�ek
}


/////////////////////////////////////////////////////////////////////////////
// statick� konstruktor a destruktor

void CBufIcon::Init()
{
	m_Data = NULL;						// nen� buffer dat
	m_Num = 0;							// nen� ��dn� platn� polo�ka
	m_Max = 0;							// nen� buffer polo�ek
}

void CBufIcon::Term()
{
	DelAll();							// zru�en� v�ech polo�ek
}


////////////////////////////////////////////////////////////////////
// vytvo�en� nov�ch dat (odd�leno kv�li lep�� optimalizaci)

void CBufIcon::NewData()
{
	m_Max *= 2;
	if (m_Max == 0) m_Max = 0x400;
//	m_Max += 0x400;				// 4 KB
	MemBuf(m_Data, m_Max);
}


////////////////////////////////////////////////////////////////////
// zru�en� v�ech polo�ek v bufferu

void CBufIcon::DelAll()
{
	Del(m_Num);					// zru�en� polo�ek
	MemBuf(m_Data, 0);			// zru�en� bufferu dat
	m_Max = 0;					// nen� ��dn� polo�ka v bufferu
}


////////////////////////////////////////////////////////////////////
// poskytnut� polo�ky (s kontrolou platnosti indexu)

const CIcon& _fastcall CBufIcon::Get(const int index) const
{
	if (IsValid(index))			// je index platn�?
	{
		return m_Data[index];	// polo�ka na dan�m indexu
	}
	return EmptyIcon;			// pro neplatn� index vr�t� pr�zdnou ikonu
}

////////////////////////////////////////////////////////////////////
// nastaven� polo�ky (s kontrolou platnosti indexu)

void _fastcall CBufIcon::Set(const int index, const CIcon& data)
{
	if (IsValid(index))					// je index platn�?
	{
		m_Data[index] = data;
	}
}


/////////////////////////////////////////////////////////////////////////////
// vypr�zdn�n� polo�ky (bez jej�ho zru�en� - jen pro uvoln�n� dat)

void _fastcall CBufIcon::Empty(const int index)
{
	if (IsValid(index))					// je index platn�?
	{
		m_Data[index].Empty();
	}
}


////////////////////////////////////////////////////////////////////
// zru�en� polo�ek z konce bufferu

void _fastcall CBufIcon::Del(int num)
{
	if (num > m_Num) num = m_Num;

	CIcon* icon = m_Data + m_Num - 1;
	m_Num -= num;

	for (; num > 0; num--)
	{
		icon->Term();
		icon--;
	}
}


/////////////////////////////////////////////////////////////////////////////
// oper�tor p�i�azen�

const CBufIcon& _fastcall CBufIcon::operator= (const CBufIcon& src)
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


////////////////////////////////////////////////////////////////////
// vytvo�en� polo�ky (vrac� index polo�ky)

int CBufIcon::New()
{
	int result = NewItem();				// vytvo�en� nov� polo�ky
	m_Data[result].InitNew();			// inicializace polo�ky
	return result;
}


////////////////////////////////////////////////////////////////////
// p�id�n� polo�ky (vrac� index polo�ky)

int _fastcall CBufIcon::Add(const CIcon& data)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result].Init(data.Data());	// inicializace polo�ky
	return result;
}

int _fastcall CBufIcon::Add(ICONDATA* data)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result].Init(data);	// inicializace polo�ky
	return result;
}


////////////////////////////////////////////////////////////////////
// duplikace polo�ky (s kontrolou platnosti indexu, vrac� index prvn� polo�ky)

int _fastcall CBufIcon::Dup(const int index)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky

	if (IsValid(index))					// je index platn�?
	{
		m_Data[result].Init(m_Data[index].Data());	// kopie polo�ky
	}
	else
	{
		m_Data[result].Init();		// inicializace neplatn� polo�ky
	}
	return result;
}

int _fastcall CBufIcon::Dup(const int index, int num)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky

	if (IsValid(index))					// je index platn�?
	{
		ICONDATA* data = m_Data[index].Data();

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
