
#include "Main.h"

/***************************************************************************\
*																			*
*								Obr�zkov� prom�nn�							*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inicializa�n� pr�zdn� obr�zek (modifikuje se po�et referenc�!)

// Pr�zdn� obr�zek mus� m�t rozm�ry ICONSIZE kv�li vytvo�en� nov�ho sprajtu!

PICTUREDATA	EmptyPictureData		= { 1, ICONWIDTH, ICONHEIGHT, NULL };


/////////////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CPicture::CPicture() 
{ 
	Attach(&EmptyPictureData); 
};

CPicture::CPicture(const CPicture& src) 
{ 
	Attach(src.pData); 
};

CPicture::CPicture(int width, int height)
{
	NewBuffer(width, height);
};

CPicture::~CPicture() 
{ 
	Detach(); 
};


/////////////////////////////////////////////////////////////////////////////
// statick� konstruktor a destruktor

void CPicture::Init()
{ 
	Attach(&EmptyPictureData); 
};

void CPicture::Init(PICTUREDATA* data)
{ 
	Attach(data); 
};

void CPicture::Init(int width, int height)
{
	NewBuffer(width, height);
};

void CPicture::Term()
{ 
	Detach(); 
};


/////////////////////////////////////////////////////////////////////////////
// kopie do vlastn�ho bufferu p�ed modifikac�

void CPicture::CopyWrite()
{
	PICTUREDATA* data = pData;		// adresa star�ch dat
	long* refer = &(data->Refer);	// po�et referenc�

	if (*refer > 1)					// je n�jak� jin� majitel?
	{
		NewBuffer(data->Width, data->Height);	// vytvo�en� nov�ho bufferu
		MemCopy(pData->Data, data->Data, data->Width * data->Height);

// odpojen� star�ch dat - v multithread m��e nastat i zru�en�
		if (LongDecrement(refer))
		{
#ifdef _MT
			MemFree(data->Data);
			MemFree(data);			// p��padn� zru�en� dat
#endif	// _MT
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� nov�ho obr�zku (p�ipraveno k z�pisu, data jsou n�hodn�)

void CPicture::New(int width, int height)
{
	Detach();						// odpojen� star�ho obr�zku
	NewBuffer(width, height);		// vytvo�en� nov�ho bufferu
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� nov�ch rozm�r� obr�zku

void CPicture::Resize(int width, int height)
{
	if ((width != pData->Width) || (height != pData->Height))
	{
// vytvo�en� nov�ho bufferu (star� se zat�m jen uschov�)
		PICTUREDATA* olddata = pData;
		NewBuffer(width, height);
		PICTUREDATA* newdata = pData;

// p��prava po�tu linek ke kopii
		int i = newdata->Height;
		if (olddata->Height < i) i = olddata->Height;

// p��prava d�lky linky ke kopii
		int j = newdata->Width;
		if (olddata->Width < j) j = olddata->Width;

// p��prava zbytku linky k vymaz�n�
		int k = newdata->Width - j;

// p��rustek zdrojov� a c�lov� adresy
		int srcinc = olddata->Width;
		int dstinc = newdata->Width;

// p��prava zdrojov� a c�lov� adresy
		BYTE* src = olddata->Data;
		BYTE* dst = newdata->Data;

// barva k vymaz�n� pozad�
		BYTE col = BackCol;

// kopie platn�ch linek
		if (srcinc == dstinc)
		{
			i *= dstinc;
			MemCopy(dst, src, i);
			dst += i;
			src += i;
		}
		else
		{
			if (k == 0)
			{
				for (; i > 0; i--)
				{
					MemCopy(dst, src, j);
					dst += dstinc;
					src += srcinc;
				}
			}
			else
			{
				for (; i > 0; i--)
				{
					MemCopy(dst, src, j);
					MemFill(dst + j, k, col);
					dst += dstinc;
					src += srcinc;
				}
			}
		}

// vymaz�n� zbyl�ch linek
		i = newdata->Height - olddata->Height;
		if (i > 0)
		{
			MemFill(dst, i*dstinc, col);
		}

// odpojen� star�ho bufferu
		pData = olddata;
		Detach();
		pData = newdata;
	}
}


void CPicture::HalfSize()
{
// star� a nov� rozm�ry
	int oldwidth = Width();
	int oldheight = Height();
	int newwidth = oldwidth/2;
	int newheight = oldheight/2;
	if ((newwidth <= 0) || (newheight <= 0)) return;

// vytvo�en� nov�ho bufferu
	PICTUREDATA* olddata = pData;
	NewBuffer(newwidth, newheight);
	PICTUREDATA* newdata = pData;

// konverze obr�zku
	BYTE* src = olddata->Data;
	BYTE* dst = newdata->Data;

	for (int i = newheight; i > 0; i--)
	{
		if (Dither)
		{
			for (int j = newwidth; j > 0; j--)
			{
				dst[0] = ColAvrgDither(
					src[0],
					src[1],
					src[oldwidth],
					src[oldwidth+1],
					j, i);
				src++;
				src++;
				dst++;
			}
		}
		else
		{
			for (int j = newwidth; j > 0; j--)
			{
				dst[0] = ColAvrg(
					src[0],
					src[1],
					src[oldwidth],
					src[oldwidth+1]);
				src++;
				src++;
				dst++;
			}
		}
		src += 2*(oldwidth - newwidth);
	}

// odpojen� star�ho bufferu
	pData = olddata;
	Detach();
	pData = newdata;
}


/////////////////////////////////////////////////////////////////////////////
// poskytnut� bodu (s kontrolou platnosti offsetu/indexu)

BYTE _fastcall CPicture::Get(const int off) const
{
	if (IsValid(off))				// je offset platn�?
	{
		return pData->Data[off];	// bod na dan�m offsetu
	}
	return BackCol;					// neplatn� offset
}

BYTE _fastcall CPicture::Get(const int x, const int y) const
{
	if (IsValid(x, y))				// je index platn�?
	{
		return pData->Data[x + y*Width()];	// prvek
	}
	return BackCol;					// neplatn� offset
}


////////////////////////////////////////////////////////////////////
// nastaven� bodu (s kontrolou platnosti offsetu/indexu)

void _fastcall CPicture::Set(const int off, const BYTE data)
{
	if (IsValid(off))				// je offset platn�?
	{
		pData->Data[off] = data;
	}
}

void _fastcall CPicture::Set(const int x, const int y, const BYTE data)
{
	if (IsValid(x, y))				// je index platn�?
	{
		pData->Data[x + y*Width()] = data;
	}
}


/////////////////////////////////////////////////////////////////////////////
// kopie nov�ch dat obr�zku (rozm�ry z�stanou nezm�n�ny) - zajist� odpojen� dat

void CPicture::CopyData(BYTE* src)
{
	ASSERT(src);
	PICTUREDATA* data = pData;		// adresa star�ch dat (z�hlav�)
	long* refer = &(data->Refer);	// po�et referenc�
	int width = data->Width;		// ���ka obr�zku
	int height = data->Height;		// v��ka obr�zku;

	if (*refer > 1)					// je n�jak� jin� majitel?
	{
		NewBuffer(width, height);	// vytvo�en� nov�ho bufferu

// odpojen� star�ch dat - v multithread m��e nastat i zru�en�
		if (LongDecrement(refer))
		{
#ifdef _MT
			MemFree(data->Data);	// p��padn� zru�en� dat
			MemFree(data);			// p��padn� zru�en� z�hlav�
#endif	// _MT
		}
	}

	MemCopy(pData->Data, src, width*height); // kopie obsahu obr�zku
}


/////////////////////////////////////////////////////////////////////////////
// kopie nov�ch dat obr�zku s konverz� (rozm�ry z�stanou nezm�n�ny) - zajist� odpojen� dat

void CPicture::CopyKonvData(BYTE* src)
{
	ASSERT(src);
	PICTUREDATA* data = pData;		// adresa star�ch dat (z�hlav�)
	long* refer = &(data->Refer);	// po�et referenc�
	int width = data->Width;		// ���ka obr�zku
	int height = data->Height;		// v��ka obr�zku;

	if (*refer > 1)					// je n�jak� jin� majitel?
	{
		NewBuffer(width, height);	// vytvo�en� nov�ho bufferu

// odpojen� star�ch dat - v multithread m��e nastat i zru�en�
		if (LongDecrement(refer))
		{
#ifdef _MT
			MemFree(data->Data);	// p��padn� zru�en� dat
			MemFree(data);			// p��padn� zru�en� z�hlav�
#endif	// _MT
		}
	}

	KonvCopy(pData->Data, src, width*height); // kopie obsahu obr�zku
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� obr�zku ze souboru s polovi�n� velikost� (TRUE=operace OK)

BOOL CPicture::LoadFile2(CString jmeno)
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

// na�ten� z�hlav� souboru
	char hd[14];
	DWORD read;
	::ReadFile(hFile, hd, 14, &read, NULL);

// zji�t�n� velikosti souboru (bez z�hlav� 14 bajt�)
	int size = (int)::SetFilePointer(hFile, 0, NULL, FILE_END) - 14;
	::SetFilePointer(hFile, 14, NULL, FILE_BEGIN);

// kontrola velikosti souboru a z�hlav�
	if ((size < 50) || (size > 20000000) ||	(hd[0] != 'B') || (hd[1] != 'M'))
	{
		::CloseHandle(hFile);		// uzav�en� souboru
		return FALSE;
	}

// p��prava bufferu k na�ten� souboru
	BITMAPINFO* bmp = (BITMAPINFO*)MemGet(size);

// na�ten� souboru do bufferu
	::ReadFile(hFile, bmp, size, &read, NULL);

// uzav�en� souboru
	::CloseHandle(hFile);		// uzav�en� souboru

// kontrola spr�vnosti operace �ten�, kontrola platnosti souboru
	if ((read != (DWORD)size) || !TestBMP(bmp))
	{
		MemFree(bmp);				// uvoln�n� bufferu
		return FALSE;
	}

// p��prava parametr� bitmapy
	int width = bmp->bmiHeader.biWidth;			// ���ka
	int width2 = width/2;
	int height = bmp->bmiHeader.biHeight;		// v��ka
	int height2 = height/2;
	int bits = bmp->bmiHeader.biBitCount;		// po�et bit� na bod
	int colors = bmp->bmiHeader.biClrUsed;		// po�et pou�it�ch barev
	if (colors <= 0) colors = (1 << bits);		// implicitn� po�et barev
	if (bits > 8) colors = 0;					// pro TRUE COLOR nejsou palety
	int sizehead = sizeof(BITMAPINFOHEADER) + colors*sizeof(RGBQUAD); // velikost z�hlav�
	size -= sizehead;							// oprava velikosti dat
	if (size < 0) size = 0;

// dekomprese bitmapy RLE8
	if (bmp->bmiHeader.biCompression == BI_RLE8)
	{
		int newsize = ((width + 3) & ~3) * height;
		BITMAPINFO* bmp2;
		bmp2 = (BITMAPINFO*)MemGet(sizehead + newsize + 10000);
		MemCopy(bmp2, bmp, sizehead);
		bmp2->bmiHeader.biCompression = BI_RGB;
		DekompRLE8((BYTE*)bmp2+sizehead, newsize, (BYTE*)bmp+sizehead, size);
		MemFree(bmp);
		bmp = bmp2;
		size = newsize;
	}

// dekomprese bitmapy RLE4
	if (bmp->bmiHeader.biCompression == BI_RLE4)
	{
		int newsize = (((width+1)/2 + 3) & ~3) * height;
		BITMAPINFO* bmp2;
		bmp2 = (BITMAPINFO*)MemGet(sizehead + newsize + 10000);
		MemCopy(bmp2, bmp, sizehead);
		bmp2->bmiHeader.biCompression = BI_RGB;
		DekompRLE4((BYTE*)bmp2+sizehead, newsize, (BYTE*)bmp+sizehead, size);
		MemFree(bmp);
		bmp = bmp2;
		size = newsize;
	}

// �schova p�vodn�ho obr�zku
	CPicture pic(*this);

// vytvo�en� nov�ho obr�zku
	if (bits == 24)
	{
		New(width2, height2);
	}
	else
	{
		New(width, height);
	}

// vygenerov�n� konverzn� tabulky palet
	if (bits <= 8)
	{
		GenKonvPal(bmp);
	}

// p��prava bufferu odchylky pro dithering
	int* odch = NULL;
	if (Dither)
	{
		odch = (int*)MemGet((3*width + 6) * sizeof(int));
		MemFill(odch, (3*width + 6) * sizeof(int), 0);
	}

// p��prava parametr� ke konverzi
	BYTE* dst = pData->Data;				// ukl�dac� adresa
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
		HalfSize();
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
		HalfSize();
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
		HalfSize();
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
		HalfSize();
		break;

// 24 bit�
	case 24:
		srcinc = (3*width + 3) & ~3;

		for (i = height2; i > 0; i--)		// cyklus p�es v�echny linky
		{
			int* odch0 = odch + 3;			// ukazatel v bufferu odchylek

			for (j = width2; j > 0; j--)		// cyklus p�es v�echny body
			{
				int			r = 0;									// �erven� slo�ka
				int			g = 0;									// zelen� slo�ka
				int			b = 0;									// modr� slo�ka
				BYTE		n = 0;									// po�et platn�ch bod�
				int			s = 0;									// po�et stin�

				if ((*(int*)src & 0xffffff) != (BACKCOLOR_BLUE | (BACKCOLOR_GREEN*256) | (BACKCOLOR_RED*256*256)))
				{
				  if ((*(int*)src & 0xffffff) == (SHADCOLOR_BLUE | (SHADCOLOR_GREEN*256) | (SHADCOLOR_RED*256*256)))
				  {
					  s++;
				  }
				  else
				  {
					n++;											// zv��en� ��ta�e bod�
					r = src[2];										// �erven� slo�ka
					g = src[1];										// zelen� slo�ka
					b = src[0];										// modr� slo�ka
				  }
				}

				if ((*(int*)(src + 3) & 0xffffff) != (BACKCOLOR_BLUE | (BACKCOLOR_GREEN*256) | (BACKCOLOR_RED*256*256)))
				{
				  if ((*(int*)(src + 3) & 0xffffff) == (SHADCOLOR_BLUE | (SHADCOLOR_GREEN*256) | (SHADCOLOR_RED*256*256)))
				  {
					  s++;
				  }
				  else
				  {
					n++;											// zv��en� ��ta�e bod�
					r += src[5];									// �erven� slo�ka
					g += src[4];									// zelen� slo�ka
					b += src[3];									// modr� slo�ka
				  }
				}

				if ((*(int*)(src + srcinc) & 0xffffff) != (BACKCOLOR_BLUE | (BACKCOLOR_GREEN*256) | (BACKCOLOR_RED*256*256)))
				{
				  if ((*(int*)(src + srcinc) & 0xffffff) == (SHADCOLOR_BLUE | (SHADCOLOR_GREEN*256) | (SHADCOLOR_RED*256*256)))
				  {
					  s++;
				  }
				  else
				  {
					n++;											// zv��en� ��ta�e bod�
					r += src[srcinc+2];								// �erven� slo�ka
					g += src[srcinc+1];								// zelen� slo�ka
					b += src[srcinc+0];								// modr� slo�ka
				  }
				}

				if ((*(int*)(src + srcinc + 3) & 0xffffff) != (BACKCOLOR_BLUE | (BACKCOLOR_GREEN*256) | (BACKCOLOR_RED*256*256)))
				{
				  if ((*(int*)(src + srcinc + 3) & 0xffffff) == (SHADCOLOR_BLUE | (SHADCOLOR_GREEN*256) | (SHADCOLOR_RED*256*256)))
				  {
					  s++;
				  }
				  else
				  {
					n++;											// zv��en� ��ta�e bod�
					r += src[srcinc+5];								// �erven� slo�ka
					g += src[srcinc+4];								// zelen� slo�ka
					b += src[srcinc+3];								// modr� slo�ka
				  }
				}

				if (s > 2)
				{
					*dst = ShadCol;

					if (Dither)
					{
						*odch0 = 0;
						odch0++;
						*odch0 = 0;
						odch0++;
						*odch0 = 0;
						odch0++;
					}
				}
				else

				if (n < 2)
				{
					*dst = BackCol;

					if (Dither)
					{
						*odch0 = 0;
						odch0++;
						*odch0 = 0;
						odch0++;
						*odch0 = 0;
						odch0++;
					}
				}
				else
				{
					if (Dither)
					{
						r /= n;
						g /= n;
						b /= n;

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
					else
					{
						*dst = PalImport((BYTE)(r/n), (BYTE)(g/n), (BYTE)(b/n));
					}
				}

				dst++;						// zv��en� c�lov� adresy
				src += 6;
			}
			src += 2*srcinc - 6*width2;
		}
		break;

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
		HalfSize();
		break;
	}

// uvoln�n� bufferu odchylky pro dithering
	MemFree(odch);

// uvoln�n� bufferu
	MemFree(bmp);

// p��znak - obr�zek na�ten OK
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� obr�zku ze souboru (TRUE=operace OK)
/*
BOOL CPicture::LoadFile(CString jmeno)
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

// na�ten� z�hlav� souboru
	char hd[14];
	DWORD read;
	::ReadFile(hFile, hd, 14, &read, NULL);

// zji�t�n� velikosti souboru (bez z�hlav� 14 bajt�)
	int size = (int)::SetFilePointer(hFile, 0, NULL, FILE_END) - 14;
	::SetFilePointer(hFile, 14, NULL, FILE_BEGIN);

// kontrola velikosti souboru a z�hlav�
	if ((size < 50) || (size > 20000000) ||	(hd[0] != 'B') || (hd[1] != 'M'))
	{
		::CloseHandle(hFile);		// uzav�en� souboru
		return FALSE;
	}

// p��prava bufferu k na�ten� souboru
	BITMAPINFO* bmp = (BITMAPINFO*)MemGet(size);

// na�ten� souboru do bufferu
	::ReadFile(hFile, bmp, size, &read, NULL);

// uzav�en� souboru
	::CloseHandle(hFile);		// uzav�en� souboru

// kontrola spr�vnosti operace �ten�, kontrola platnosti souboru
	if ((read != (DWORD)size) || !TestBMP(bmp))
	{
		MemFree(bmp);				// uvoln�n� bufferu
		return FALSE;
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
	if (bmp->bmiHeader.biCompression == BI_RLE8)
	{
		int newsize = ((width + 3) & ~3) * height;
		BITMAPINFO* bmp2;
		bmp2 = (BITMAPINFO*)MemGet(sizehead + newsize + 10000);
		MemCopy(bmp2, bmp, sizehead);
		bmp2->bmiHeader.biCompression = BI_RGB;
		DekompRLE8((BYTE*)bmp2+sizehead, newsize, (BYTE*)bmp+sizehead, size);
		MemFree(bmp);
		bmp = bmp2;
		size = newsize;
	}

// dekomprese bitmapy RLE4
	if (bmp->bmiHeader.biCompression == BI_RLE4)
	{
		int newsize = (((width+1)/2 + 3) & ~3) * height;
		BITMAPINFO* bmp2;
		bmp2 = (BITMAPINFO*)MemGet(sizehead + newsize + 10000);
		MemCopy(bmp2, bmp, sizehead);
		bmp2->bmiHeader.biCompression = BI_RGB;
		DekompRLE4((BYTE*)bmp2+sizehead, newsize, (BYTE*)bmp+sizehead, size);
		MemFree(bmp);
		bmp = bmp2;
		size = newsize;
	}

// �schova p�vodn�ho obr�zku
	CPicture pic(*this);

// vytvo�en� nov�ho obr�zku
	New(width, height);

// vygenerov�n� konverzn� tabulky palet
	if (bits <= 8)
	{
		GenKonvPal(bmp);
	}

// p��prava parametr� ke konverzi
	BYTE* dst = pData->Data;				// ukl�dac� adresa
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
			for (j = width; j > 0; j--)		// cyklus p�es v�echny body
			{
				if (Dither)
				{
					*dst = PalImportDither(bmp->bmiColors[*src].rgbRed,
							bmp->bmiColors[*src].rgbGreen, bmp->bmiColors[*src].rgbBlue, j, i);
				}
				else
				{
					*dst = PalImport(bmp->bmiColors[*src].rgbRed,
							bmp->bmiColors[*src].rgbGreen, bmp->bmiColors[*src].rgbBlue);
				}
				src++;						// zv��en� zdrojov� adresy
				dst++;						// zv��en� c�lov� adresy
			}
			src += srcinc;					// zv��en� zdrojov� adresy
		}
		break;

// 16 bit�
	case 16:
		srcinc = ((2*width + 3) & ~3) - 2*width;

		for (i = height; i > 0; i--)		// cyklus p�es v�echny linky
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
				if (Dither)
				{
					*dst = PalImportDither(r, g, b, j, i);	// import barvy do vlastn�ch palet
				}
				else
				{
					*dst = PalImport(r, g, b);	// import barvy do vlastn�ch palet
				}
				src++;						// zv��en� zdrojov� adresy
				src++;
				dst++;						// zv��en� c�lov� adresy
			}
			src += srcinc;
		}
		break;

// 24 bit�
	case 24:
		srcinc = ((3*width + 3) & ~3) - 3*width;

		for (i = height; i > 0; i--)		// cyklus p�es v�echny linky
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
					if (Dither)
					{
						*dst = PalImportDither(r, g, b, j, i);	// import barvy do vlastn�ch palet
					}
					else
					{
						*dst = PalImport(r, g, b);	// import barvy do vlastn�ch palet
					}
				  }
				}
				dst++;						// zv��en� c�lov� adresy
			}
			src += srcinc;
		}
		break;

// 32 bit�
	case 32:
		for (i = height; i > 0; i--)		// cyklus p�es v�echny linky
		{
			for (j = width; j > 0; j--)		// cyklus p�es v�echny body
			{
				if (*(int*)src == (BACKCOLOR_BLUE | (BACKCOLOR_GREEN*256) | (BACKCOLOR_RED*256*256)))
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
					if (Dither)
					{
						*dst = PalImportDither(r, g, b, j, i);	// import barvy do vlastn�ch palet
					}
					else
					{
						*dst = PalImport(r, g, b);	// import barvy do vlastn�ch palet
					}
				  }
				}
				dst++;						// zv��en� c�lov� adresy
			}
		}
		break;
	}

// uvoln�n� bufferu
	MemFree(bmp);

// p��znak - obr�zek na�ten OK
	return TRUE;
}
*/

/////////////////////////////////////////////////////////////////////////////
// ulo�en� do souboru form�tu BMP (FALSE=chyba)

BOOL CPicture::SaveFile(CString jmeno) const
{
// otev�en� souboru
	HANDLE hFile = ::CreateFile(jmeno, GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

// test, zda byl soubor vytvo�en
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

// p��prava velikosti z�hlav� souboru
	int headsize = sizeof(BITMAPFILEHEADER) + 
					sizeof(BITMAPINFOHEADER) +
					StdColors*sizeof(RGBQUAD);

// p��prava bufferu pro obr�zek
	BYTE* buf = (BYTE*)MemGet(headsize + (pData->Width+6)*pData->Height*2 + 1000);

// komprese dat
	int size = KompRLE8(buf + headsize, pData->Data, pData->Width, pData->Height);

// napln�n� z�hlav� popisova�e souboru
	BITMAPFILEHEADER* head = (BITMAPFILEHEADER*) buf;
	buf[0] = 'B';								// identifikace souboru
	buf[1] = 'M';
	head->bfSize = headsize + size;				// velikost souboru
	head->bfReserved1 = 0;
	head->bfReserved2 = 0;
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
	bmp->biXPelsPerMeter = 0;					// horizont�ln� rozli�en�
	bmp->biYPelsPerMeter = 0;					// vertik�ln� rozli�en�
	bmp->biClrUsed = StdColors;					// po�et pou�it�ch palet
	bmp->biClrImportant = StdColors;			// po�et d�le�it�ch palet

// p�enesen� palet
	MemCopy(buf + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER),
				StdBitmapInfo->bmiColors, StdColors*sizeof(RGBQUAD));

// ulo�en� souboru
	size += headsize;
	DWORD write;
	BOOL result = ::WriteFile(hFile, buf, size, &write, NULL);

// uzav�en� souboru
	::CloseHandle(hFile);

// zru�en� bufferu
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


/////////////////////////////////////////////////////////////////////////////
// oper�tor p�i�azen�

const CPicture& CPicture::operator= (const CPicture& src)
{
	Detach();				// zru�en� star�ch dat
	Attach(src.pData);		// p�i�azen� nov�ch dat
	return *this;
}


/***************************************************************************\
*																			*
*								Buffer obr�zk�								*
*																			*
\***************************************************************************/


////////////////////////////////////////////////////////////////////
// vytvo�en� nov�ch dat

#define NEWDATANUM 256				// po�et nov� vytvo�en�ch polo�ek (1 KB)

void CBufPic::NewData()
{
	int i = NEWDATANUM;			// po�et nov�ch polo�ek
	int next = m_Max;			// p��t� polo�ka - 1
	m_Max = next + i;			// zv��en� po�tu polo�ek (o 1 KB)

	MemBuf(m_Data, m_Max);		// zv��en� velikosti bufferu

	MemBuf(m_Valid, m_Max);		// zv��en� velikosti bufferu platnosti
	MemFill(m_Valid + next, NEWDATANUM, FALSE); // nastaven� p��znak� na neplatn� polo�ky

	CPicture* data = m_Data + next - 1; // ukazatel dat - 1
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

CBufPic::CBufPic()
{
	m_Data = NULL;			// nen� buffer dat
	m_Valid = NULL;			// nen� buffer platnosti
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
	m_Next = -1;			// p�i�t� voln� polo�ka (-1=nen�)
}

CBufPic::~CBufPic()
{
	DelAll();				// zru�en� v�ech polo�ek
}


////////////////////////////////////////////////////////////////////
// statick� konstruktor a destruktor

void CBufPic::Init()
{
	m_Data = NULL;			// nen� buffer dat
	m_Valid = NULL;			// nen� buffer platnosti
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
	m_Next = -1;			// p�i�t� voln� polo�ka (-1=nen�)
}

void CBufPic::Term()
{
	DelAll();				// zru�en� v�ech polo�ek
}

////////////////////////////////////////////////////////////////////
// zru�en� v�ech polo�ek v bufferu (ukl�d�n� za�ne op�t po �ad�)

void CBufPic::DelAll()
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

const CPicture& _fastcall CBufPic::Get(const int index) const
{
	if (IsValid(index))			// je index platn�?
	{
		return m_Data[index];	// polo�ka na dan�m indexu
	}
	return EmptyPicture;		// pro neplatn� index vr�t� pr�zdn� obr�zek
}

////////////////////////////////////////////////////////////////////
// nastaven� polo�ky (s kontrolou platnosti indexu)

void _fastcall CBufPic::Set(const int index, const CPicture& data)
{
	if (IsValid(index))			// je index platn�?
	{
		m_Data[index] = data;
	}
}

////////////////////////////////////////////////////////////////////
// zru�en� polo�ky (s kontrolou platnosti indexu)

void _fastcall CBufPic::Del(const int index)
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

int CBufPic::New()
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result].Init();		// inicializace polo�ky
	return result;
}

int CBufPic::New(int width, int height)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result].Init(width, height);	// inicializace polo�ky
	m_Data[result].Clear();		// vymaz�n� obr�zku
	return result;
}

////////////////////////////////////////////////////////////////////
// p�id�n� polo�ky (vrac� index polo�ky)

int _fastcall CBufPic::Add(const CPicture& data)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result].Init(data.Data());	// inicializace polo�ky
	return result;
}

int _fastcall CBufPic::Add(PICTUREDATA* data)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	m_Data[result].Init(data);	// inicializace polo�ky
	return result;
}


////////////////////////////////////////////////////////////////////
// duplikace polo�ky (s kontrolou platnosti indexu, vrac� index polo�ky)

int _fastcall CBufPic::Dup(const int index)
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

const CBufPic& CBufPic::operator= (const CBufPic& src)
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

