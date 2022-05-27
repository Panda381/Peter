
#include "Main.h"

/***************************************************************************\
*																			*
*								Obr�zkov� prom�nn�							*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inicializa�n� pr�zdn� obr�zek (modifikuje se po�et referenc�!)

// Pr�zdn� obr�zek mus� m�t rozm�ry ICONSIZE kv�li vytvo�en� nov�ho sprajtu!

#ifndef _MINI
PICTUREDATA	EmptyPictureData		= {	1,					// ��ta� referenc�
										PicParamNone,		// parametry
										ICONWIDTH,			// ���ka
										ICONHEIGHT,			// v��ka
										NULL,				// adresa dat
										-1,					// index textury
										0,					// ���ka textury
										0,					// v��ka textury
										0,					// je mipmap
										0,					// po�et mipmap
										0,					// typ za��zen�
										TRUE,				// textura obsahuje pr�hlednou barvu
										NULL,				// data textury
										NULL,				// data textury RGBA
										NULL,				// data textury R8G8B8
										NULL,				// data textury R5G6B5
										NULL,				// data textury A1R5G5B5
										NULL,				// data textury A4R4G4B4
										0,					// zjemn�n�
									};
#else // _MINI
PICTUREDATA	EmptyPictureData		= { 1, PicParamNone, ICONWIDTH, ICONHEIGHT, NULL, 0 };
#endif // _MINI

int		AviWidth = 384;				// ���ka AVI
int		AviHeight = 288;			// v��ka AVI

/////////////////////////////////////////////////////////////////////////////
// odpojen� (a zru�en�) dat

void _fastcall CPicture::Detach()
{
	ASSERT(pData);
	ASSERT(pData->Refer > 0);

#ifndef _MINI
	if ((pData->Refer <= 2) && (pData->TextInx >= 0))
	{
		D3DTITEM* titem = D3DT_Get(pData->TextInx);

		if (titem->Ref == 1)
		{
			titem->Ref = 0;

			if (D3D)
			{
				for (int i = 0; i < MAX_STAGES; i++)
				{
					if (D3DT_Active[i] == pData->TextInx)
					{
						D3DT_Active[i] = -1;
						D3DT_On[i] = false;
						pD3TextureOff(i);
					}
				}
				pData->TextInx = -1;
				pD3TextureRes(titem);
			}

			titem->Mips = FALSE;
			pData->Refer = 1;
		}
	}

#endif // _MINI

	if (LongDecrement(&(pData->Refer)))
	{
#ifndef _MINI
		MemFree(pData->TextData);
		MemFree(pData->TextDataRGBA);
		MemFree(pData->TextDataR8G8B8);
		MemFree(pData->TextDataR5G6B5);
		MemFree(pData->TextDataA1R5G5B5);
		MemFree(pData->TextDataA4R4G4B4);
#endif // _MINI
		ASSERT(pData->Data);
		MemFree(pData->Data);
		MemFree(pData);
#ifdef _DEBUG
		pData = NULL;
#endif
	}
}

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


// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI
		 
/////////////////////////////////////////////////////////////////////////////
// importov�n� textury z True Color bufferu
// ... Funkce vstupn� buffer zru��!
// ... Buffery pro texturu mus� b�t zru�eny!

void CPicture::ImportTexture(BYTE* buf, int newwidth, int newheight)
{

// sou�asn� rozm�ry obr�zku
	int oldwidth = pData->Width;
	int oldheight = pData->Height;

// v�choz� rozm�ry textury
	pData->TextWidth = newwidth;
	pData->TextHeight = newheight;

// p��prava po�tu MipMap (a� bude rozm�r 1x1)
	int w = newwidth;
	int h = newheight;
	int sizesum = w*h*4;
	int mips = 1;

	if (pData->TextIsMip)
	{
		while ((w > 1) || (h > 1))
		{
			w = (w >> 1);
			if (w == 0) w++;
			h = (h >> 1);
			if (h == 0) h++;
			sizesum += (w*h*4);
			mips++;
		}
	}
	pData->TextMipMaps = mips;

// buffer textury
	DWORD* dst = (DWORD*)MemGet(sizesum);
	pData->TextData = (BYTE*)dst; 

	DWORD* dstRGBA = (DWORD*)MemGet(sizesum);
	pData->TextDataRGBA = (BYTE*)dstRGBA;

	BYTE* dstR8G8B8 = (BYTE*)MemGet((sizesum/4)*3);
	pData->TextDataR8G8B8 = dstR8G8B8;

	sizesum /= 2;

	WORD* dstR5G6B5 = (WORD*)MemGet(sizesum);
	pData->TextDataR5G6B5 = (BYTE*)dstR5G6B5;

	WORD* dstA1R5G5B5 = (WORD*)MemGet(sizesum);
	pData->TextDataA1R5G5B5 = (BYTE*)dstA1R5G5B5;

	WORD* dstA4R4G4B4 = (WORD*)MemGet(sizesum);
	pData->TextDataA4R4G4B4 = (BYTE*)dstA4R4G4B4;

// cyklus p�es v�echny MipMaps
	for (; mips > 0; mips--)
	{
		int newsize = newwidth*newheight;
		dst = dst + (newsize - newwidth);
		dstRGBA = dstRGBA + (newsize - newwidth);
		dstR8G8B8 = dstR8G8B8 + 3*(newsize - newwidth);
		dstR5G6B5 = dstR5G6B5 + (newsize - newwidth);
		dstA1R5G5B5 = dstA1R5G5B5 + (newsize - newwidth);
		dstA4R4G4B4 = dstA4R4G4B4 + (newsize - newwidth);

// konverze obr�zku na spr�vnou velikost (nevad�, kdy� se rozm�ry nem�n�)
		buf = ZoomTrueColor(buf, oldwidth, oldheight, newwidth, newheight);

// konverze dat do v�stupn�ho bufferu
		BYTE* src = buf;

		for (int i = newheight; i > 0; i--)
		{
			for (int j = newwidth; j > 0; j--)
			{
				*dst = (DWORD)(*(DWORD*)src ^ 0xff000000);
				*dstRGBA = XRGB(*(DWORD*)src);

				*(WORD*)dstR8G8B8 = *(WORD*)src;
				dstR8G8B8[2] = src[2];

				int b = (src[0] >> 3);
				int g = (src[1] >> 2);
				int r = (src[2] >> 3);

				*dstR5G6B5 = (WORD)(b | (g << 5) | (r << 11));

				g = (g >> 1);
				int a = (((BYTE*)dst)[3] >> 4); 

				*dstA1R5G5B5 = (WORD)(b | (g << 5) | (r << 10) | ((a & 0x8) << 12));

				*dstA4R4G4B4 = (WORD)((b >> 1) | ((g >> 1) << 4) | ((r >> 1) << 8) | (a << 12));

				src += 5;

				dst++;
				dstRGBA++;
				dstR8G8B8 += 3;
				dstR5G6B5++;
				dstA1R5G5B5++;
				dstA4R4G4B4++;
			}
			dst -= (2*newwidth);
			dstRGBA -= (2*newwidth);
			dstR8G8B8 -= 3*(2*newwidth);
			dstR5G6B5 -= (2*newwidth);
			dstA1R5G5B5 -= (2*newwidth);
			dstA4R4G4B4 -= (2*newwidth);
		}

// p��prava pro dal�� �rove� MipMaps
		dst = dst + (newwidth + newsize);
		dstRGBA = dstRGBA + (newwidth + newsize);
		dstR8G8B8 = dstR8G8B8 + 3*(newwidth + newsize);
		dstR5G6B5 = dstR5G6B5 + (newwidth + newsize);
		dstA1R5G5B5 = dstA1R5G5B5 + (newwidth + newsize);
		dstA4R4G4B4 = dstA4R4G4B4 + (newwidth + newsize);

		oldwidth = newwidth;
		newwidth = (newwidth >> 1);
		if (newwidth == 0) newwidth++;
		oldheight = newheight;
		newheight = (newheight >> 1);
		if (newheight == 0) newheight++;
	}

// zru�en� datov�ho bufferu
	MemFree(buf);
}

/////////////////////////////////////////////////////////////////////////////
// vytvo�en� dat textury, nen�-li vytvo�ena
// Mus� b�t nastavena polo�ka TextSmooth !

void CPicture::InitTexture(int stage)
{
	int devakt = D3DIntAkt*100 + D3DDevAkt;

// jsou-li data a je to bu� textura ze souboru nebo nebylo zm�n�n� za��zen�, nen� pot�eba zm�ny
	if ((pData->TextData != NULL) &&
		((pData->TextSmooth == 0) ||
		(pData->TextDevType == devakt))) return;

// dekomprese obr�zku
	DeComp();

// sou�asn� rozm�ry obr�zku
	int oldwidth = pData->Width;
	int oldheight = pData->Height;

// nov� rozm�ry obr�zku
	int newwidth = oldwidth;
	int newheight = oldheight;

// aplikace zjemn�n�
	newwidth = Round((double)newwidth/pData->TextSmooth);
	if (newwidth <= 0) newwidth = 1;

	newheight = Round((double)newheight/pData->TextSmooth);
	if (newheight <= 0) newheight = 1;

// korekce na �tvercovou texturu (pou�ije se pr�m�r)
	if (SquareTexture)
	{
		newwidth = (newwidth + newheight + 1)/2;
		newheight = newwidth;
	}

// korekce rozm�r� na mocninu 2 (pou�ije se nejbli��� rozm�r)
	if (TexturePow2)
	{
		int k = (newwidth*3 + 2) / 4;
		newwidth = 1;
		while (newwidth < k) newwidth = (newwidth << 1);

		k = (newheight*3 + 2) / 4;
		newheight = 1;
		while (newheight < k) newheight = (newheight << 1);
	}

// omezen� rozm�r� textury
	if (newwidth > MaxTextureWidth) newwidth = MaxTextureWidth;
	if (newheight > MaxTextureHeight) newheight = MaxTextureHeight;

// zru�en� textury, pokud byla vytvo�ena pro jin� za��zen� a jsou nyn� nutn� men�� rozm�ry
	if ((pData->TextData != NULL) &&
		(pData->TextDevType != devakt) &&
		((pData->TextWidth > newwidth) ||
		(pData->TextHeight > newheight)))
	{
		MemFree(pData->TextData);
		pData->TextData = NULL;
		MemFree(pData->TextDataRGBA);
		pData->TextDataRGBA = NULL;
		MemFree(pData->TextDataR8G8B8);
		pData->TextDataR8G8B8 = NULL;
		MemFree(pData->TextDataR5G6B5);
		pData->TextDataR5G6B5 = NULL;
		MemFree(pData->TextDataA1R5G5B5);
		pData->TextDataA1R5G5B5 = NULL;
		MemFree(pData->TextDataA4R4G4B4);
		pData->TextDataA4R4G4B4 = NULL;
	}
	pData->TextDevType = devakt;

// test, zda je textura ji� vytvo�ena
	if (pData->TextData != NULL) return;

// p��znak mipmap
	pData->TextIsMip = D3DMipFilterAkt[stage];

// detekce pr�hlednosti
	pData->TextTrans = FALSE;
	BYTE* ss = pData->Data;
	for (int kk = oldwidth*oldheight; kk > 0; kk--)
	{
		if (*ss == BackCol)
		{
			pData->TextTrans = TRUE;
			break;
		}

		ss++;
	}

// importov�n� textury
	BYTE* buf = ExportTrueAlphaBeta();
	ImportTexture(buf, newwidth, newheight);
}

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------


/////////////////////////////////////////////////////////////////////////////
// kopie do vlastn�ho bufferu p�ed modifikac� (nekop�ruj� se data textury)

void CPicture::CopyWrite()
{
	DeComp();

	PICTUREDATA* data = pData;		// adresa star�ch dat
	long* refer = &(data->Refer);	// po�et referenc�

	if (*refer > 1)					// je n�jak� jin� majitel?
	{
		NewBuffer(data->Width, data->Height);	// vytvo�en� nov�ho bufferu
		MemCopy(pData->Data, data->Data, data->Width * data->Height);
		pData->Param = data->Param;	// p�enesen� parametr�

// odpojen� star�ch dat - v multithread m��e nastat i zru�en�
		if (LongDecrement(refer))
		{
#ifndef _MINI
			MemFree(data->TextData);
			MemFree(data->TextDataRGBA);
			MemFree(data->TextDataR8G8B8);
			MemFree(data->TextDataR5G6B5);
			MemFree(data->TextDataA1R5G5B5);
			MemFree(data->TextDataA4R4G4B4);
#endif // _MINI

#ifdef _MT
			MemFree(data->Data);
			MemFree(data);			// p��padn� zru�en� dat
#endif	// _MT
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// vypr�zdn�n� obr�zku (uvoln�n� dat)

void CPicture::Empty()
{ 
	Detach(); 
	Attach(&EmptyPictureData); 
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� nov�ho obr�zku (p�ipraveno k z�pisu, data jsou n�hodn�)

void CPicture::New(int width, int height)
{
	Detach();						// odpojen� star�ho obr�zku
	NewBuffer(width, height);		// vytvo�en� nov�ho bufferu
}


/////////////////////////////////////////////////////////////////////////////
// dekomprimace dat obr�zku (jsou-li komprimov�na)

void CPicture::DeComp()
{
	if (pData->Param == PicParamComp)
	{
		int width = pData->Width;
		int height = pData->Height;
		int size = width*height;
		BYTE* data = (BYTE*)MemGet(size);
		DeCompress(data, pData->Data + 4, size, width);
		pData->Data = data;
		pData->Refer -= BigInt;
		pData->Param = PicParamNone;
	}
}

/////////////////////////////////////////////////////////////////////////////
// maskov�n� obr�zku

void CPicture::Mask(CPicture& mask)
{
// dekomprese obr�zku
	CopyWrite();
	mask.DeComp();

// p��prava ���ky
	int dstwidth = Width();
	int srcwidth = mask.Width();
	int width;

	if (srcwidth < dstwidth)
	{
		width = srcwidth;
		dstwidth -= width;
		srcwidth = 0;
	}
	else
	{
		width = dstwidth;
		srcwidth -= width;
		dstwidth = 0;
	}

// p��prava v��ky
	int dstheight = Height();
	int srcheight = mask.Height();
	int height;

	if (srcheight < dstheight)
	{
		height = srcheight;
		dstheight -= height;
		srcheight = 0;
	}
	else
	{
		height = dstheight;
		srcheight -= height;
		dstheight = 0;
	}

// p��prava ukazatel� dat
	BYTE* dstdata = DataData();
	BYTE* srcdata = mask.DataData();

// proveden� konverze
	for (int i = height; i > 0; i--)
	{
		for (int j = width; j > 0; j--)
		{
			if (*srcdata == BackCol) *dstdata = BackCol;
			srcdata++;
			dstdata++;
		}

		MemFill(dstdata, dstwidth, BackCol);
		dstdata += dstwidth;
		srcdata += srcwidth;
	}

// vymaz�n� zbytku linek
	MemFill(dstdata, dstheight * (width + dstwidth), BackCol);
}

/////////////////////////////////////////////////////////////////////////////
// z�m�na barev v obr�zku

void CPicture::XCol(BYTE col1, BYTE col2)
{
// dekomprese obr�zku
	CopyWrite();

// p��prava ukazatel�
	BYTE* data = DataData();

// z�m�na barev
	for (int i = Width()*Height(); i > 0; i--)
	{
		BYTE col = *data;

		if (col == col1)
		{
			*data = col2;
		}
		else
		{
			if (col == col2)
			{
				*data = col1;
			}
		}

		data++;
	}
}

/////////////////////////////////////////////////////////////////////////////
// n�hrada barvy v obr�zku

void CPicture::SCol(BYTE oldcol, BYTE newcol)
{
// dekomprese obr�zku
	CopyWrite();

// p��prava ukazatel�
	BYTE* data = DataData();

// z�m�na barev
	for (int i = Width()*Height(); i > 0; i--)
	{
		if (*data == oldcol)
		{
			*data = newcol;
		}

		data++;
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� nov�ch rozm�r� obr�zku

void CPicture::Resize(int width, int height)
{
	DeComp();

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
					MemFill(dst + j, k, BackCol);
					dst += dstinc;
					src += srcinc;
				}
			}
		}

// vymaz�n� zbyl�ch linek
		i = newdata->Height - olddata->Height;
		if (i > 0)
		{
			MemFill(dst, i*dstinc, BackCol);
		}

// odpojen� star�ho bufferu
		pData = olddata;
		Detach();
		pData = newdata;
	}
}


/////////////////////////////////////////////////////////////////////////////
// exportov�n� obr�zku do bufferu TRUECOLORALPHABETA (vrac� buffer)

BYTE* CPicture::ExportTrueAlphaBeta()
{
// zaji�t�n� dekomprimace
	DeComp();

// vytvo�en� bufferu
	int size = pData->Width * pData->Height;
	BYTE* buf = (BYTE*)MemGet(size * 5);

// p�evod na TRUE COLOR ALPHA
	BYTE* src = pData->Data;
	BYTE* dst = buf;
	RGBQUAD* rgb = StdBitmapInfo->bmiColors;

	for (; size > 0; size--)
	{
		BYTE col = *src;
		src++;

		switch (col)
		{
		case BackCol:
			*dst = 0;		// B: modr�
			dst++;
			*dst = 0;		// G: zelen�
			dst++;
			*dst = 0;		// R: �erven�
			dst++;
			*dst = 255;		// A: pr�hlednost
			dst++;
			*dst = 0;		// S: st�n
			dst++;
			break;

		case ShadCol:
			*dst = 0;		// B: modr�
			dst++;
			*dst = 0;		// G: zelen�
			dst++;
			*dst = 0;		// R: �erven�
			dst++;
			*dst = 0;		// A: pr�hlednost
			dst++;
			*dst = 255;		// S: st�n
			dst++;
			break;

		default:
			*dst = rgb[col].rgbBlue;		// B: modr�
			dst++; 
			*dst = rgb[col].rgbGreen;		// G: zelen�
			dst++;
			*dst = rgb[col].rgbRed;			// R: �erven�
			dst++;
			*dst = 0;						// A: pr�hlednost
			dst++;
			*dst = 0;						// S: st�n
			dst++;
		}
	}

	return buf;
}


/////////////////////////////////////////////////////////////////////////////
// importov�n� obr�zku z bufferu TRUECOLORALPHABETA

void CPicture::ImportTrueAlphaBeta(BYTE* buf, BOOL dith)
{
// rozm�ry obr�zku
	int width = pData->Width;
	int height = pData->Height;

// zaji�t�n� pr�zdn�ho obr�zku
	if (pData->Refer > 1) New(width, height);
	DeComp();

// ukazatele dat
	BYTE* src = buf;
	BYTE* dst = pData->Data;

// konverze s ditheringem
	if (dith)
	{

// p��prava bufferu odchylky pro dithering
		int* odch = (int*)MemGet((3*width + 6 + 10) * sizeof(int));
		MemFill(odch, (3*width + 6 + 10) * sizeof(int), 0);
		
// cyklus p�es v�echny linky		
		for (int y = height; y > 0; y--)
		{
			int* odch0 = odch + 3;			// ukazatel v bufferu odchylky

// cyklus p�es v�echny body na lince
			for (int x = width; x > 0; x--)
			{

// pozad�
				if (src[3] > 128)
				{
					*dst = BackCol;

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
					if ((src[3] + src[4]) > 128)
					{
						*dst = ShadCol;

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
						BYTE b = src[0];			// modr� slo�ka
						BYTE g = src[1];			// zelen� slo�ka
						BYTE r = src[2];			// �erven� slo�ka

// zkorigovan� barva
						int b2 = b - (odch0[-3] + odch0[0] + odch0[3])*5/8;		// modr� slo�ka
						int g2 = g - (odch0[-2] + odch0[1] + odch0[4])*5/8;		// zelen� slo�ka
						int r2 = r - (odch0[-1] + odch0[2] + odch0[5])*5/8;		// �erven� slo�ka

// omezen� p�ete�en� barvy
						if ((DWORD)b2 > 255) { if (b2 < 0) b2 = 0; else b2 = 255; }
						if ((DWORD)g2 > 255) { if (g2 < 0) g2 = 0; else g2 = 255; }
						if ((DWORD)r2 > 255) { if (r2 < 0) r2 = 0; else r2 = 255; }

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
						*dst = col;
					}
				}
				dst++;
				src += 5;
			}
		}

// zru�en� bufferu odchylky pro dithering
		MemFree(odch);
	}

// konverze bez ditheringu
	else
	{
		for (int y = height; y > 0; y--)
		{
			for (int x = width; x > 0; x--)
			{
				if (src[3] > 128)
				{
					*dst = BackCol;
				}
				else
				{
					if ((src[3] + src[4]) > 128)
					{
						*dst = ShadCol;
					}
					else
					{
						*dst = PalImport(src[2], src[1], src[0]);
					}
				}
				dst++;
				src += 5;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////
// pomocn� funkce pro filtry

#define		Max(x,y) (((x) > (y)) ? (x) : (y))	// maxim�ln� hodnota
#define		Min(x,y) (((x) < (y)) ? (x) : (y))	// minim�ln� hodnota

//static double Box(double x)
//{
//  if ((x >= -0.5) && (x < 0.5))
//    return(1.0);
//  return(0.0);
//}

inline double Filter(double x)
{

/*
// Triangle

#define FSUPPORT 1

  if (x < 0.0)
    x=(-x);
  if (x < 1.0)
    return(1.0-x);
  return(0.0);
*/

/*
// Catrom

#define FSUPPORT 2

  if (x < 0)
    x=(-x);
  if (x < 1.0)
    return(0.5*(2.0+x*x*(-5.0+x*3.0)));
  if (x < 2.0)
    return(0.5*(4.0+x*(-8.0+x*(5.0-x))));
  return(0.0);
*/

// Mitchell

#define FSUPPORT 2

  double
    b,
    c;

  b=1.0/3.0;
  c=1.0/3.0;
  if (x < 0)
    x=(-x);
  if (x < 1.0)
    {
      x=((12.0-9.0*b-6.0*c)*(x*x*x))+((-18.0+12.0*b+6.0*c)*x*x)+(6.0-2.0*b);
      return(x/6.0);
    }
 if (x < 2.0)
   {
     x=((-1.0*b-6.0*c)*(x*x*x))+((6.0*b+30.0*c)*x*x)+((-12.0*b-48.0*c)*x)+
       (8.0*b+24.0*c);
     return(x/6.0);
   }
  return(0.0);

}

typedef struct ContribInfo_
{
  int		pixel;
  double    weight;
} ContribInfo;


/////////////////////////////////////////////////////////////////////////
// horizont�ln� filtr

void HorizontalFilter(BYTE* oldbuf, int oldwidth, int oldheight, BYTE* newbuf, int newwidth)
{
// p��prava ukazatel� podpory
	double xfactor = (double)newwidth/oldwidth;
	double support = FSUPPORT/xfactor;
	if (support < FSUPPORT) support = FSUPPORT;

// buffer konverze bod�
	ContribInfo* cinfo = (ContribInfo*)MemGet(Round(support*2+3)*sizeof(ContribInfo) + 128);

// konverzn� faktory
	double scale = Max(1.0/xfactor, 1.0);
	support = Max(scale * FSUPPORT, 0.5);
	if (support <= 0.5)
	{
		support = 0.5;
		scale = 1.0;
	}
	support += 1.0e-7;							// korekce pro zaokrouhlov�n�

// cyklus p�es v�echny c�lov� body
	for (int x = 0; x < newwidth; x++)
	{

// p��prava pro p�evodn� tabulku bodu
		double density = 0.0;					// hustota bodu
		int n = 0;								// ��ta� bod�
		double center = (double)x/xfactor;		// zdrojov� sou�adnice bodu
		int start = Round(center - support);	// po��te�n� bod
		int end = Round(center + support);		// koncov� bod

// konverzn� tabulka vah bod�
		for (int i = Max(start, 0); i < Min(end + 1, oldwidth); i++)
		{
			cinfo[n].pixel = i;					// ��slo zdrojov�ho bodu
			double weight = Filter(((double)i - center)/scale)/scale;
			density += weight;
			cinfo[n].weight = weight;
			n++;
		}

// normalizace vah
		if ((density != 0.0) && (density != 1.0))
		{
			for (int i = 0; i < n; i++)
			{
				cinfo[i].weight /= density;
			}
		}

// adresa c�lov�ho bodu ke konverzi
		BYTE* dst = newbuf + x*5;

// cyklus p�es v�echny linky
		for (int y = 0; y < oldheight; y++)
		{

// p��prava st�ada�� vah barev
			double blue = 0;
			double green = 0;
			double red = 0;
			double alpha = 0;
			double shadow = 0;

// v�po�et barevn�ch slo�ek
			for (int i = 0; i < n; i++)
			{
				BYTE* src = oldbuf + (y*oldwidth + cinfo[i].pixel)*5;

				double weight = cinfo[i].weight;

				blue	+= weight * src[0];
				green	+= weight * src[1];
				red		+= weight * src[2];
				alpha	+= weight * src[3];
				shadow	+= weight * src[4];
			}

// korekce barev p�i pr�hlednosti a st�nu
			double as = alpha + shadow;

			if ((as > 0.0001) && (as < 254))
			{
//				double kor = sqrt(1/(1 - as/255));
				double kor = 1/(1 - as/1024);

				blue *= kor;
				green *= kor;
				red *= kor;
			}

// normalizace barevn�ch slo�ek
			int newblue = Round(blue);
			if ((DWORD)newblue >= 256)
			{
				if (newblue < 0) newblue = 0; else newblue = 255;
			}
			dst[0] = (BYTE)newblue;

			int newgreen = Round(green);
			if ((DWORD)newgreen >= 256)
			{
				if (newgreen < 0) newgreen = 0; else newgreen = 255;
			}
			dst[1] = (BYTE)newgreen;

			int newred = Round(red);
			if ((DWORD)newred >= 256)
			{
				if (newred < 0) newred = 0; else newred = 255;
			}
			dst[2] = (BYTE)newred;

			int newalpha = Round(alpha);
			if ((DWORD)newalpha >= 256)
			{
				if (newalpha < 0) newalpha = 0; else newalpha = 255;
			}
			dst[3] = (BYTE)newalpha;

			int newshadow = Round(shadow);
			if ((DWORD)newshadow >= 256)
			{
				if (newshadow < 0) newshadow = 0; else newshadow = 255;
			}
			dst[4] = (BYTE)newshadow;

			dst += newwidth*5;
		}
	}

// zru�en� bufferu konverze bod�
	MemFree(cinfo);
}

/////////////////////////////////////////////////////////////////////////
// vertik�ln� filtr

void VerticalFilter(BYTE* oldbuf, int oldwidth, int oldheight, BYTE* newbuf, int newheight)
{
// p��prava ukazatel� podpory
	double yfactor = (double)newheight/oldheight;
	double support = FSUPPORT/yfactor;
	if (support < FSUPPORT) support = FSUPPORT;

// buffer konverze bod�
	ContribInfo* cinfo = (ContribInfo*)MemGet(Round(support*2+3)*sizeof(ContribInfo) + 128);

// konverzn� faktory
	double scale = Max(1.0/yfactor, 1.0);
	support = Max(scale * FSUPPORT, 0.5);
	if (support <= 0.5)
	{
		support = 0.5;
		scale = 1.0;
	}
	support += 1.0e-7;							// korekce pro zaokrouhlov�n�

// cyklus p�es v�echny c�lov� linky
	for (int y = 0; y < newheight; y++)
	{

// p��prava pro p�evodn� tabulku bodu
		double density = 0.0;					// hustota bodu
		int n = 0;								// ��ta� bod�
		double center = (double)y/yfactor;		// zdrojov� sou�adnice bodu
		int start = Round(center - support);	// po��te�n� bod
		int end = Round(center + support);		// koncov� bod

// konverzn� tabulka vah bod�
		for (int i = Max(start, 0); i < Min(end + 1, oldheight); i++)
		{
			cinfo[n].pixel = i;					// ��slo zdrojov�ho bodu
			double weight = Filter(((double)i - center)/scale)/scale;
			density += weight;
			cinfo[n].weight = weight;
			n++;
		}

// normalizace vah
		if ((density != 0.0) && (density != 1.0))
		{
			for (int i = 0; i < n; i++)
			{
				cinfo[i].weight /= density;
			}
		}

// adresa c�lov�ho bodu ke konverzi
		BYTE* dst = newbuf + y*oldwidth*5;

// cyklus p�es v�echny bodu na lince
		for (int x = 0; x < oldwidth; x++)
		{

// p��prava st�ada�� vah barev
			double blue = 0;
			double green = 0;
			double red = 0;
			double alpha = 0;
			double shadow = 0;

// v�po�et barevn�ch slo�ek
			for (int i = 0; i < n; i++)
			{
				BYTE* src = oldbuf + (x + cinfo[i].pixel*oldwidth)*5;

				double weight = cinfo[i].weight;

				blue	+= weight * src[0];
				green	+= weight * src[1];
				red		+= weight * src[2];
				alpha	+= weight * src[3];
				shadow	+= weight * src[4];
			}

// korekce barev p�i pr�hlednosti a st�nu
			double as = alpha + shadow;

			if ((as > 0.0001) && (as < 254))
			{
//				double kor = sqrt(1/(1 - as/255));
				double kor = 1/(1 - as/1024);

				blue *= kor;
				green *= kor;
				red *= kor;
			}

// normalizace barevn�ch slo�ek
			int newblue = Round(blue);
			if ((DWORD)newblue >= 256)
			{
				if (newblue < 0) newblue = 0; else newblue = 255;
			}
			dst[0] = (BYTE)newblue;

			int newgreen = Round(green);
			if ((DWORD)newgreen >= 256)
			{
				if (newgreen < 0) newgreen = 0; else newgreen = 255;
			}
			dst[1] = (BYTE)newgreen;

			int newred = Round(red);
			if ((DWORD)newred >= 256)
			{
				if (newred < 0) newred = 0; else newred = 255;
			}
			dst[2] = (BYTE)newred;

			int newalpha = Round(alpha);
			if ((DWORD)newalpha >= 256)
			{
				if (newalpha < 0) newalpha = 0; else newalpha = 255;
			}
			dst[3] = (BYTE)newalpha;

			int newshadow = Round(shadow);
			if ((DWORD)newshadow >= 256)
			{
				if (newshadow < 0) newshadow = 0; else newshadow = 255;
			}
			dst[4] = (BYTE)newshadow;

			dst += 5;
		}
	}

// zru�en� bufferu konverze bod�
	MemFree(cinfo);
}
		

/////////////////////////////////////////////////////////////////////////////
// konverze obr�zku v bufferu TrueColorAlphaBeta (vrac� nov� buffer)

BYTE* ZoomTrueColor(BYTE* buf, int oldwidth, int oldheight, int newwidth, int newheight)
{

// p�i zv��en� v��ky obr�zku se konvertuje nejd��ve horizont�ln�
	if (newheight >= oldheight)
	{

// test, zda je pot�eba prov�d�t horizont�ln� konverzi
		if (newwidth != oldwidth)
		{

// vytvo�en� v�stupn�ho bufferu pro horizont�ln� konverzi
			BYTE* buf2 = (BYTE*)MemGet(newwidth*oldheight*5);

// proveden� horizont�ln� konverze
			HorizontalFilter(buf, oldwidth, oldheight, buf2, newwidth);

// p�enesen� na vstupn� buffer
			MemFree(buf);
			buf = buf2;
		}

// test, zda je pot�eba prov�d�t vertik�ln� konverzi
		if (newheight != oldheight)
		{

// vytvo�en� v�stupn�ho bufferu pro vertik�ln� konverzi
			BYTE* buf2 = (BYTE*)MemGet(newheight*newwidth*5);

// proveden� vertik�ln� konverze
			VerticalFilter(buf, newwidth, oldheight, buf2, newheight);

// p�enesen� na vstupn� buffer
			MemFree(buf);
			buf = buf2;
		}
	}

// p�i sn�en� v��ky obr�zku se konvertuje nejd��ve vertik�ln�
	else
	{

// test, zda je pot�eba prov�d�t vertik�ln� konverzi
		if (newheight != oldheight)
		{

// vytvo�en� v�stupn�ho bufferu pro vertik�ln� konverzi
			BYTE* buf2 = (BYTE*)MemGet(newheight*oldwidth*5);

// proveden� vertik�ln� konverze
			VerticalFilter(buf, oldwidth, oldheight, buf2, newheight);

// p�enesen� na vstupn� buffer
			MemFree(buf);
			buf = buf2;
		}

// test, zda je pot�eba prov�d�t horizont�ln� konverzi
		if (newwidth != oldwidth)
		{

// vytvo�en� v�stupn�ho bufferu pro horizont�ln� konverzi
			BYTE* buf2 = (BYTE*)MemGet(newwidth*newheight*5);

// proveden� horizont�ln� konverze
			HorizontalFilter(buf, oldwidth, newheight, buf2, newwidth);

// p�enesen� na vstupn� buffer
			MemFree(buf);
			buf = buf2;
		}
	}

	return buf;
}

/////////////////////////////////////////////////////////////////////////////
// zm�na velikosti obr�zku (int = interpolovat, dith = ditherovat)

void CPicture::Zoom(int newwidth, int newheight, BOOL inter, BOOL dith)
{
// �schova star�ch rozm�r�
	int oldwidth = pData->Width;
	int oldheight = pData->Height;

// test, zda bude zm�na velikosti obr�zku
	if ((newwidth == oldwidth) && (newheight == oldheight)) return;

// zaji�t�n� dekomprimace
	DeComp();

// bude konverze bez interpolace
	if (!inter)
	{

// vytvo�en� bufferu horizont�ln� konverzn� tabulky
		int* konv = (int*)MemGet(newwidth * sizeof(int));

// vytvo�en� nov�ho (pr�zdn�ho) obr�zku
		CPicture pic;
		pic.New(newwidth, newheight);

// inicializace horizont�ln� konverzn� tabulky
		int i, j;
		double k = (double)oldwidth/newwidth;

		for (i = 0; i < newwidth; i++)
		{
			j = RoundM(i * k);
			if (j >= oldwidth) j = oldwidth - 1;
			konv[i] = j;
		}

// proveden� konverze
		BYTE* dst = pic.DataData();
		k = (double)oldheight/newheight;

		for (i = 0; i < newheight; i++)
		{
			j = RoundM(i * k);
			if (j >= oldheight) j = oldheight - 1;

			BYTE* src = pData->Data + oldwidth*j;

			for (j = 0; j < newwidth; j++)
			{
				*dst = src[konv[j]];
				dst++;
			}
		}

// zru�en� konverzn� tabulky
		MemFree(konv);

// nastaven� nov�ho obr�zku
		*this = pic;

		return;
	}

// vytvo�en� bufferu s daty TRUECOLOR
	BYTE* buf = ExportTrueAlphaBeta();

// konverze obr�zku
	buf = ZoomTrueColor(buf, oldwidth, oldheight, newwidth, newheight);

// vytvo�en� nov�ho (pr�zdn�ho) obr�zku
	New(newwidth, newheight);

// importov�n� obr�zku z datov�ho bufferu
	ImportTrueAlphaBeta(buf, dith);

// zru�en� datov�ho bufferu
	MemFree(buf);
}


/////////////////////////////////////////////////////////////////////////////
// zm�na jasu obr�zku

void CPicture::Level(double koef, bool dith)
{
// kontrola, zda je obr�zek platn�
	if ((pData->Width <= 0) || (pData->Height <= 0)) return;

// omezen� rozsahu koeficient�
	koef = fabs(koef);
	if (koef > 1000) koef = 1000;

// nen� zm�na jasu barvy
	if (koef == 1) return;

// zaji�t�n� dekomprimace
	DeComp();

// konverzn� tabulka
	BYTE* tab = (BYTE*)MemGet(256);

// konverze bez ditheringu
	if (!dith)
	{

// p�ivlastn�n� bufferu
		CopyWrite();

// pr�hledn� barva a barva pozad�
		tab[BackCol] = BackCol;
		tab[ShadCol] = ShadCol;

// p��prava konverzn� tabulky
		for (int i = ResCols; i < StdColors; i++)
		{
			RGBQUAD* rgb = StdBitmapInfo->bmiColors + i;		// definice palet barvy
			BYTE r = rgb->rgbRed;								// �erven� slo�ka
			BYTE g = rgb->rgbGreen;								// zelen� slo�ka
			BYTE b = rgb->rgbBlue;								// modr� slo�ka

			int r2 = Round(r * koef);
			if (r2 > 255) r2 = 255;
			int g2 = Round(g * koef);
			if (g2 > 255) g2 = 255;
			int b2 = Round(b * koef);
			if (b2 > 255) b2 = 255;

			tab[i] = PalImport((BYTE)r2, (BYTE)g2, (BYTE)b2);
		}

// neplatn� barvy budou v�dy �ern�
		MemFill(tab + StdColors, 256 - StdColors, BlackCol);

// proveden� konverze obr�zku
		BYTE* data = pData->Data;
		for (i = pData->Width*pData->Height; i > 0; i--)
		{
			*data = tab[*data];
			data++;
		}
	}

// konverze s ditheringem
	else
	{

// p��prava konverzn� tabulky
		for (int i = 0; i < 256; i++)
		{
			int c = Round(i * koef);
			if (c > 255) c = 255;
			tab[i] = (BYTE)c;
		}

// export do bufferu TRUECOLORALPHA
		BYTE* buf = ExportTrueAlphaBeta();

// proveden� konverze
		BYTE* data = buf;
		for (i = pData->Width*pData->Height; i > 0; i--)
		{
			*data = tab[*data];		// modr�
			data++;
			*data = tab[*data];		// zelen�
			data++;
			*data = tab[*data];		// �erven�
			data++;
			data++;					// pr�hlednost
			data++;					// st�n
		}

// importov�n� obr�zku z datov�ho bufferu
		ImportTrueAlphaBeta(buf, true);

// zru�en� datov�ho bufferu
		MemFree(buf);
	}

// zru�en� konverzn� tabulky
	MemFree(tab);
}


/////////////////////////////////////////////////////////////////////////////
// oto�en� obr�zku

void CPicture::Rotate(double angle, bool inter, bool dith)
{
// ���ka a v��ka obr�zku
	int width = pData->Width;
	int height = pData->Height;

// kontrola, zda je obr�zek platn�
	if ((width <= 0) || (height <= 0)) return;

// korekce �hlu
	while (angle < 0) angle += 128*pi2;
	angle = fmod(angle, pi2);

// nen� rotace
	if ((angle > -0.0001) && (angle < 0.0001)) return;

// zaji�t�n� dekomprimace
	DeComp();

// oto�en� o 90 stup��
	if ((angle > (pi/2 - 0.0001)) && (angle < (pi/2 + 0.0001)))
	{
		CPicture pic(height, width);
		pic.DeComp();
		pic.CopyWrite();

		BYTE* dst = pic.pData->Data;
		BYTE* src = pData->Data + width*(height - 1);

		for (int j = width; j > 0; j--)
		{
			for (int i = height; i > 0; i--)
			{
				*dst = *src;
				dst++;
				src -= width;
			}
			src += height*width + 1;
		}

		*this = pic;
		return;		
	}

// oto�en� o 180 stup��
	if ((angle > (pi - 0.0001)) && (angle < (pi + 0.0001)))
	{
		CopyWrite();

		int i = width * height;
		BYTE* src = pData->Data;
		BYTE* dst = src + i - 1;
		i = i/2;

		for (; i > 0; i--)
		{
			BYTE c = *src;
			*src = *dst;
			*dst = c;
			src++;
			dst--;
		}
		return;
	}

// oto�en� o 270 stup��
	if ((angle > (pi*3/2 - 0.0001)) && (angle < (pi*3/2 + 0.0001)))
	{
		CPicture pic(height, width);
		pic.DeComp();
		pic.CopyWrite();

		BYTE* dst = pic.pData->Data;
		BYTE* src = pData->Data + width - 1;

		for (int j = width; j > 0; j--)
		{
			for (int i = height; i > 0; i--)
			{
				*dst = *src;
				dst++;
				src += width;
			}
			src -= height*width + 1;
		}

		*this = pic;
		return;		
	}

// p�i interpolaci zv�t�en� obr�zku
	if (inter) Zoom(width*2, height*2, false, false);

// p�vodn� velikost obr�zku
	int oldwidth = pData->Width;
	int oldheight = pData->Height;

// d�lka a �hel �hlop���ky
	double angle2 = atan2((double)oldheight, (double)oldwidth);
	double delka2 = sqrt((double)oldwidth*oldwidth + (double)oldheight*oldheight);

// kvadrant �hlu oto�en�
	int kvadrant = RoundM(angle / (pi/2));

// nov� ���ka a v��ka
	int newwidth;
	int newheight;

	switch (kvadrant)
	{
	case 0:
		newwidth = Round(delka2*cos(angle-angle2)+0.4);
		newheight = Round(delka2*sin(angle+angle2)+0.4);
		break;

	case 1:
		newwidth = Round(-delka2*cos(angle+angle2)+0.4);
		newheight = Round(delka2*sin(angle-angle2)+0.4);
		break;

	case 2:
		newwidth = Round(-delka2*cos(angle-angle2)+0.4);
		newheight = Round(-delka2*sin(angle+angle2)+0.4);
		break;

	default:
		newwidth = Round(delka2*cos(angle+angle2)+0.4);
		newheight = Round(-delka2*sin(angle-angle2)+0.4);
		break;
	}
	if (newwidth > 0x8000) newwidth = 0x8000;
	if (newwidth <= 0) newwidth = 1;
	if (newheight > 0x8000) newheight = 0x8000;
	if (newheight <= 0) newheight = 1;

// st�ed nov�ho obr�zku
	int xs = newwidth/2;
	int ys = newheight/2;

	int xs2 = (oldwidth+1)/2;
	int ys2 = (oldheight+1)/2;

// vytvo�en� nov�ho (pr�zdn�ho) obr�zku
	CPicture pic(newwidth, newheight);
	pic.DeComp();
	pic.CopyWrite();

// proveden� konverze obr�zku
	BYTE* src = pData->Data;
	BYTE* dst = pic.pData->Data;

	for (int y = 0; y < newheight; y++)
	{
		int dy = y - ys;
		double dy2 = (double)dy*dy;

		for (int x = 0; x < newwidth; x++)
		{
			int dx = x - xs;

			angle2 = atan2((double)dy, (double)dx) - angle;
			delka2 = sqrt((double)dx*dx + dy2);

			int xr = Round(delka2*cos(angle2)) + oldwidth - xs2;
			int yr = Round(delka2*sin(angle2)) + oldheight - ys2;

			if (((DWORD)xr < (DWORD)oldwidth) && ((DWORD)yr < (DWORD)oldheight))
			{
				*dst = src[xr + oldwidth*yr];
			}
			else
			{
				*dst = BackCol;
			}

			dst++;
		}
	}

// n�hrada nov�m obr�zkem
	*this = pic;

// p�i interpolaci zmen�en� obr�zku
	if (inter) Zoom((newwidth+1)/2, (newheight+1)/2, true, dith);
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
#ifndef _MINI
			MemFree(data->TextData);
			MemFree(data->TextDataRGBA);
			MemFree(data->TextDataR8G8B8);
			MemFree(data->TextDataR5G6B5);
			MemFree(data->TextDataA1R5G5B5);
			MemFree(data->TextDataA4R4G4B4);
#endif // _MINI

#ifdef _MT
			MemFree(data->Data);	// p��padn� zru�en� dat
			MemFree(data);			// p��padn� zru�en� z�hlav�
#endif	// _MT
		}
	}

	data->Param = PicParamNone;		// parametry
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
#ifndef _MINI
			MemFree(data->TextData);
			MemFree(data->TextDataRGBA);
			MemFree(data->TextDataR8G8B8);
			MemFree(data->TextDataR5G6B5);
			MemFree(data->TextDataA1R5G5B5);
			MemFree(data->TextDataA4R4G4B4);
#endif // _MINI

#ifdef _MT
			MemFree(data->Data);	// p��padn� zru�en� dat
			MemFree(data);			// p��padn� zru�en� z�hlav�
#endif	// _MT
		}
	}

	data->Param = PicParamNone;		// parametry
	KonvCopy(pData->Data, src, width*height); // kopie obsahu obr�zku
}

/////////////////////////////////////////////////////////////////////////////
// horizont�ln� p�evr�cen� obr�zku

void CPicture::XFlip()
{
// dekomprese obr�zku
	DeComp();

// p�ivlastn�n� bufferu
	CopyWrite();

// lok�ln� prom�nn�
	int width = pData->Width;
	BYTE* adr = pData->Data;

// p�evr�cen� obr�zku
	for (int i = pData->Height; i > 0; i--)
	{
		BYTE* src = adr;
		adr += width;
		BYTE* dst = adr - 1;

		for (int j = width / 2; j > 0; j--)
		{
			BYTE c = *src;
			*src = *dst;
			*dst = c;
			src++;
			dst--;
		}

	}
}

/////////////////////////////////////////////////////////////////////////////
// vertik�ln� p�evr�cen� obr�zku

void CPicture::YFlip()
{
// dekomprese obr�zku
	DeComp();

// p�ivlastn�n� bufferu
	CopyWrite();

// lok�ln� prom�nn�
	int width = pData->Width;
	BYTE* src = pData->Data;
	BYTE* dst = src + width*(pData->Height-1);

// p�evr�cen� obr�zku
	for (int i = pData->Height / 2; i > 0; i--)
	{
		for (int j = width; j > 0; j--)
		{
			BYTE c = *src;
			*src = *dst;
			*dst = c;
			src++;
			dst++;
		}

		dst -= 2*width;
	}
}

/////////////////////////////////////////////////////////////////////////////
// na�ten� obr�zku ze souboru (tex = importovat texturu)

void CPicture::LoadFile(bool tex)
{
#ifndef _MINI

// �schova offsetu souboru
	int oldoff = FileReadOff;

	CBuf jbuf;			// buffer pro na�ten� z JPEG
	bool jpeg = false;	// je form�t JPEG

// na�ten� z�hlav� souboru
	char hd[88];
	int i;
	for (i = 0; i < 14; i++)
	{
		hd[i] = FileReadByte();
	}

// kontrola z�hlav� souboru
	int size = *(int*)(hd + 2) - 14;
	if ((hd[0] != 'B') ||
		(hd[1] != 'M') ||
		(size < 30))
	{

// p��padn� offset za��tku dat AVI
		int avidata = oldoff;

// na�ten� dal��ch dat pro interpretaci jako AVI
		for (; i < 88; i++)
		{
			hd[i] = FileReadByte();
		}
		AVIMAINHEADER* avi = (AVIMAINHEADER*)(hd + 24);
		size = *(int*)(hd + 16);
		int w = avi->dwWidth;
		int h = avi->dwHeight;

// pokus o interpretaci jako z�hlav� AVI souboru
		if ((size > 40) &&
			(hd[0] == 'R') &&
			(hd[1] == 'I') &&
			(hd[2] == 'F') &&
			(hd[3] == 'F') &&

			(hd[8] == 'A') &&
			(hd[9] == 'V') &&
			(hd[10] == 'I') &&
			(hd[11] == ' ') &&
			(hd[12] == 'L') &&
			(hd[13] == 'I') &&
			(hd[14] == 'S') &&
			(hd[15] == 'T') &&

			(hd[20] == 'h') &&
			(hd[21] == 'd') &&
			(hd[22] == 'r') &&
			(hd[23] == 'l') &&
			(hd[24] == 'a') &&
			(hd[25] == 'v') &&
			(hd[26] == 'i') &&
			(hd[27] == 'h') &&

			(w > 4) &&
			(w <= 32768) &&
			(h > 4) &&
			(h <= 32768))
		{
			AviWidth = w;
			AviHeight = h;

// na�ten� dal��ho z�hlav�
			FileReadOff += size + 20 - 88;

			for (i = 0; i < 12; i++)
			{
				hd[i] = FileReadByte();
			}
			size = *(int*)(hd + 4);

// p�esko�en� v�pln� JUNK
			if ((hd[0] == 'J') &&
				(hd[1] == 'U') &&
				(hd[2] == 'N') &&
				(hd[3] == 'K') &&
				(size > 0))
			{
				FileReadOff += size - 4 + 12;
			}

			avidata = FileReadOff;
		}

// p�esko�en� nevyu�it�ch dat AVI
		do {
			FileReadOff = avidata;
			for (i = 0; i < 8; i++)
			{
				hd[i] = FileReadByte();
			}
			size = *(int*)(hd + 4);

			avidata = FileReadOff + size;
		} while ((size >= 0) &&
				(
				  (
					(hd[0] == '0') &&
					(((hd[2] == 'd') && (hd[3] == 'c')) ||
					 ((hd[2] == 'p') && (hd[3] == 'c')) ||
					 ((hd[2] == 'w') && (hd[3] == 'b')))
				  )

				  ||

				  (
				    (hd[0] == 'J') &&
					(hd[1] == 'U') &&
					(hd[2] == 'N') &&
					(hd[3] == 'K')
				  )
				 )
				 );

// na�ten� obr�zku z AVI
		if ((size >= 0) &&
			(hd[0] == '0') &&
			(hd[2] == 'd') &&
			(hd[3] == 'b'))
		{
			int size0 = AviWidth * AviHeight * 2;

			if (size > size0) size = size0;

			BYTE* buf = (BYTE*)MemGet(size0);
			FileReadBlok(buf, size);
			if (size < size0) MemFill(buf + size, size0 - size, 0);

			New(AviWidth, AviHeight);

			int* odch = NULL;
			if (Dither)
			{
				odch = (int*)MemGet((3*AviWidth + 6) * sizeof(int));
				MemFill(odch, (3*AviWidth + 6) * sizeof(int), 0);
			}

			BYTE* dst = pData->Data;				// ukl�dac� adresa
			BYTE* src = buf;						// �tec� adresa
			BYTE r, g, b;							// barevn� slo�ky
			int j;									// pracovn� ��ta�e
			WORD srcdat;							// zdrojov� data 16 bit�

			int srcinc = ((2*AviWidth + 3) & ~3) - 2*AviWidth;

			for (i = AviHeight; i > 0; i--)		// cyklus p�es v�echny linky
			{
				if (Dither)
				{
					int* odch0 = odch + 3;			// ukazatel v bufferu odchylek

					for (j = AviWidth; j > 0; j--)		// cyklus p�es v�echny body na lince
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
					for (j = AviWidth; j > 0; j--)		// cyklus p�es v�echny body
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

			MemFree(odch);
			MemFree(buf);
			FileReadOff = avidata;
			return;
		}

// pokus o na�ten� jako JPEG
		FileReadOff = oldoff;

		if (((BYTE)hd[0] != 0xff) ||
			((BYTE)hd[1] != 0xd8) ||
			((BYTE)hd[2] != 0xff) ||
			!JPEG::Decomp(&jbuf) ||
			(jbuf.Size() < 40) ||
			((jbuf.Adr())[0] != 'B') ||
			((jbuf.Adr())[1] != 'M') ||
			(*(int*)(jbuf.Adr() + 2) > jbuf.Size()) ||
			(*(int*)(jbuf.Adr() + 2) < 44))
		{

// chyba - neplatn� form�t
			FileReadOff = oldoff;
			FileError = true;
			return;
		}

		size = *(int*)(jbuf.Adr() + 2) - 14;
		jpeg = true;
	}

// p��prava bufferu k na�ten� souboru
	BITMAPINFO* bmp;
	
	if (jpeg)
	{
		bmp = (BITMAPINFO*)(jbuf.Adr() + 14);
	}
	else
	{
		bmp = (BITMAPINFO*)MemGet(size);

// na�ten� souboru do bufferu
		MemFill(bmp, 30);
		FileReadBlok((BYTE*)bmp, size);
	}

// kontrola platnosti souboru
	if (!TestBMP(bmp))
	{
		if (jpeg)
		{
			MemFree(jbuf.Adr());
		}
		else
		{
			MemFree(bmp);				// uvoln�n� bufferu
		}
		FileReadOff = oldoff;
		FileError = true;
		return;
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
		if (jpeg)
		{
			MemFree(jbuf.Adr());
			jpeg = false;
		}
		else
		{
			MemFree(bmp);				// uvoln�n� bufferu
		}
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
		if (jpeg)
		{
			MemFree(jbuf.Adr());
			jpeg = false;
		}
		else
		{
			MemFree(bmp);				// uvoln�n� bufferu
		}
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
	int j;									// pracovn� ��ta�e
	BYTE r, g, b;							// barevn� slo�ky
	WORD srcdat;							// zdrojov� data 16 bit�

// rozli�en� podle po�tu bod�
	switch (bits)
	{

// 1 bit
	case 1:
		srcinc = ((width+7)/8 + 3) & ~3;	// p��rustek zdrojov� adresy

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
		break;
	}

// uvoln�n� bufferu odchylky pro dithering
	MemFree(odch);

// vygenerov�n� textur
	if (tex & ((bits == 24) || (bits == 32)))
	{
		int newwidth = width;
		int newheight = height;

		int k = (newwidth*3 + 2) / 4;
		newwidth = 1;
		while (newwidth < k) newwidth = (newwidth << 1);

		k = (newheight*3 + 2) / 4;
		newheight = 1;
		while (newheight < k) newheight = (newheight << 1);

		BYTE* buf = (BYTE*)MemGet(newwidth * newheight * 5);

		pData->TextIsMip = true;
		pData->TextDevType = D3DIntAkt*100 + D3DDevAkt;
		pData->TextTrans = (bits == 32);
		pData->TextSmooth = 0;

		src = (BYTE*)bmp + sizehead;		// �tec� adresa
		BYTE* dst = buf;

		if (bits == 24)
		{
			srcinc = ((3*width + 3) & ~3) - 3*width;

			for (i = height; i > 0; i--)			// cyklus p�es v�echny linky
			{
				for (j = width; j > 0; j--)		// cyklus p�es v�echny body na lince
				{
					*((DWORD*)dst) = *((DWORD*)src) & 0xFFFFFF;
					src += 3;
					dst += 4;
					*dst = 0;					// st�n
					dst++;
				}
				src += srcinc;
			}
		}
		else
		{
			for (i = height; i > 0; i--)		// cyklus p�es v�echny linky
			{
				for (j = width; j > 0; j--)		// cyklus p�es v�echny body na lince
				{
					*((DWORD*)dst) = *((DWORD*)src) ^ 0xFF000000;
					src += 4;
					dst += 4;
					*dst = 0;					// st�n
					dst++;
				}
			}
		}

		ImportTexture(buf, newwidth, newheight);
	}

// uvoln�n� bufferu
	if (jpeg)
	{
		MemFree(jbuf.Adr());
	}
	else
	{
		MemFree(bmp);				// uvoln�n� bufferu
	}

// p��znak - obr�zek na�ten OK
	return;

#endif // _MINI
}


/////////////////////////////////////////////////////////////////////////////
// ulo�en� do souboru

void CPicture::SaveFile()
{
#ifndef _MINI

// dekomprese obr�zku
	DeComp();

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
	bmp->biXPelsPerMeter = 5906;				// horizont�ln� 150 bod� na palec
	bmp->biYPelsPerMeter = 5906;				// vertik�ln� 150 bod� na palec
	bmp->biClrUsed = StdColors;					// po�et pou�it�ch palet
	bmp->biClrImportant = StdColors;			// po�et d�le�it�ch palet

// p�enesen� palet
	MemCopy(buf + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER),
				StdBitmapInfo->bmiColors, StdColors*sizeof(RGBQUAD));

// ulo�en� souboru
	size += headsize;
	FileWriteBlok((BYTE*)buf, size);

// zru�en� bufferu
	MemFree(buf);

#endif // _MINI
}


/////////////////////////////////////////////////////////////////////////////
// oper�tor p�i�azen�

const CPicture& CPicture::operator= (const CPicture& src)
{
	Detach();				// zru�en� star�ch dat
	Attach(src.pData);		// p�i�azen� nov�ch dat
	return *this;
}

const CPicture& CPicture::operator= (PICTUREDATA* src)
{
	Detach();				// zru�en� star�ch dat
	Attach(src);			// p�i�azen� nov�ch dat
	return *this;
}

const CPicture& CPicture::operator= (CIcon& icon)
{
	icon.DeComp();									// dekomprese dat ikony
	Detach();										// odpojen� star�ho obr�zku
	NewBuffer(ICONWIDTH, ICONHEIGHT);				// vytvo�en� nov�ho bufferu
	MemCopy(pData->Data, icon.DataData(), ICONSIZE); // kopie obsahu obr�zku
	return *this;
}

const CPicture& CPicture::operator= (CMap& map)
{
// parametry plochy
	int mapwidth = map.Width();
	int mapheight = map.Height();

// parametry obr�zku
	int picwidth = mapwidth * ICONWIDTH;
	int picheight = mapheight * ICONHEIGHT;

// vytvo�en� nov�ho obr�zku
	Detach();
	NewBuffer(picwidth, picheight);

// p��prava ukazatel�
	BYTE* dst = pData->Data;
	MAPITEM* item = map.DataData();
	CIcon* icon;
	BYTE* src;

// p�enesen� dat ikon (po ��dc�ch ikon)
	for (int i = mapheight; i > 0; i--)
	{
		for (int j = mapwidth; j > 0; j--)
		{
			icon = &(item->Icon);
			icon->DeComp();
			src = icon->DataData();

			for (int k = ICONHEIGHT; k > 0; k--)
			{
				MemCopy(dst, src, ICONWIDTH);
				dst += picwidth;
				src += ICONWIDTH;
			}

			dst -= ICONHEIGHT*picwidth;
			dst += ICONWIDTH;
			item++;
		}

		dst += picwidth*(ICONHEIGHT-1);
	}
	return *this;
}


/***************************************************************************\
*																			*
*								Buffer obr�zk�								*
*																			*
\***************************************************************************/


////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CBufPic::CBufPic()
{
	m_Data = NULL;			// nen� buffer dat
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
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
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
}

void CBufPic::Term()
{
	DelAll();				// zru�en� v�ech polo�ek
}


////////////////////////////////////////////////////////////////////
// vytvo�en� nov�ch dat (odd�leno kv�li lep�� optimalizaci)

void CBufPic::NewData()
{
	m_Max *= 2;
	if (m_Max == 0) m_Max = 0x400;
//	m_Max += 0x400;				// 4 KB
	MemBuf(m_Data, m_Max);
}


////////////////////////////////////////////////////////////////////
// zru�en� v�ech polo�ek v bufferu (ukl�d�n� za�ne op�t po �ad�)

void CBufPic::DelAll()
{
	Del(m_Num);					// zru�en� polo�ek
	MemBuf(m_Data, 0);			// zru�en� bufferu dat
	m_Max = 0;					// nen� ��dn� polo�ka v bufferu
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


/////////////////////////////////////////////////////////////////////////////
// vypr�zdn�n� polo�ky (bez jej�ho zru�en� - jen pro uvoln�n� dat)

void _fastcall CBufPic::Empty(const int index)
{
	if (IsValid(index))					// je index platn�?
	{
		m_Data[index].Empty();
	}
}


////////////////////////////////////////////////////////////////////
// zru�en� polo�ek z konce bufferu

void _fastcall CBufPic::Del(int num)
{
	if (num > m_Num) num = m_Num;

	CPicture* pic = m_Data + m_Num - 1;
	m_Num -= num;

	for (; num > 0; num--)
	{
		pic->Term();
		pic--;
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
// duplikace polo�ky (s kontrolou platnosti indexu, vrac� index prvn� polo�ky)

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

int _fastcall CBufPic::Dup(const int index, int num)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky

	if (IsValid(index))					// je index platn�?
	{
		PICTUREDATA* data = m_Data[index].Data();

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

const CBufPic& _fastcall CBufPic::operator= (const CBufPic& src)
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

