
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
	MemFill(EmptyPictureData.Data, ICONSIZE, BackCol);
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
	MemFill(pData->Data, pData->Width * pData->Height, BackCol);
	
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
	MemFill(pData->Data, pData->Width * pData->Height, BackCol);
	
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
					MemFill(dst, k, BackCol);
					dst += k;
					src += oldwidth;
				}
			}
		}

// vymaz�n� zbyl�ch linek
		i = height - oldheight;
		if (i > 0)
		{
			MemFill(dst, i*width, BackCol);
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
// exportov�n� obr�zku do bufferu TRUECOLORALPHABETA (vrac� buffer, NULL=chyba pam�ti)

BYTE* CPicture::ExportTrueAlphaBeta()
{
// zaji�t�n� dekomprimace
	if (!DeComp()) return NULL;

// vytvo�en� bufferu
	int size = pData->Width * pData->Height;
	BYTE* buf = (BYTE*)MemGet(size * 5);
	if (buf == NULL) return NULL;

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
// importov�n� obr�zku z bufferu TRUECOLORALPHABETA (vrac� FALSE=chyba pam�ti)

bool CPicture::ImportTrueAlphaBeta(BYTE* buf, BOOL dith)
{
// zaji�t�n� pr�zdn�ho obr�zku
	if (!New()) return false;

// rozm�ry obr�zku
	int width = pData->Width;
	int height = pData->Height;

// ukazatele dat
	BYTE* src = buf;
	BYTE* dst = pData->Data;

// konverze s ditheringem
	if (dith)
	{

// p��prava bufferu odchylky pro dithering
		int* odch = (int*)MemGet((3*width + 6 + 10) * sizeof(int));
		if (odch == NULL) return false;
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

// p��znak operace OK
	return true;
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
// horizont�ln� filtr (vrac� FALSE=chyba pam�ti)

bool HorizontalFilter(BYTE* oldbuf, int oldwidth, int oldheight, BYTE* newbuf, int newwidth)
{
// p��prava ukazatel� podpory
	double xfactor = (double)newwidth/oldwidth;
	double support = FSUPPORT/xfactor;
	if (support < FSUPPORT) support = FSUPPORT;

// buffer konverze bod�
	ContribInfo* cinfo = (ContribInfo*)MemGet(Round(support*2+3)*sizeof(ContribInfo) + 128);
	if (cinfo == NULL) return false;

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

// p��znak operace OK
	return true;
}

/////////////////////////////////////////////////////////////////////////
// vertik�ln� filtr (vrac� FALSE=chyba pam�ti)

bool VerticalFilter(BYTE* oldbuf, int oldwidth, int oldheight, BYTE* newbuf, int newheight)
{
// p��prava ukazatel� podpory
	double yfactor = (double)newheight/oldheight;
	double support = FSUPPORT/yfactor;
	if (support < FSUPPORT) support = FSUPPORT;

// buffer konverze bod�
	ContribInfo* cinfo = (ContribInfo*)MemGet(Round(support*2+3)*sizeof(ContribInfo) + 128);
	if (cinfo == NULL) return false;

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

// p��znak operace OK
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zm�na velikosti obr�zku (int = interpolovat, dith = ditherovat)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool CPicture::Zoom(int newwidth, int newheight, BOOL inter, BOOL dith)
{
// �schova star�ch rozm�r�
	int oldwidth = pData->Width;
	int oldheight = pData->Height;

// test, zda bude zm�na velikosti obr�zku
	if ((newwidth == oldwidth) && (newheight == oldheight)) return true;

// zaji�t�n� dekomprimace
	if (!DeComp()) return false;

// bude konverze bez interpolace
	if (!inter)
	{

// vytvo�en� bufferu horizont�ln� konverzn� tabulky
		int* konv = (int*)MemGet(newwidth * sizeof(int));
		if (konv == NULL) return false;

// vytvo�en� nov�ho (pr�zdn�ho) obr�zku
		CPicture pic;
		if (!pic.New(newwidth, newheight))
		{
			MemFree(konv);
			return false;
		}

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

		return true;
	}

// vytvo�en� bufferu s daty TRUECOLOR
	BYTE* buf = ExportTrueAlphaBeta();
	if (buf == NULL) return false;

// p�i zv��en� v��ky obr�zku se konvertuje nejd��ve horizont�ln�
	if (newheight >= oldheight)
	{

// test, zda je pot�eba prov�d�t horizont�ln� konverzi
		if (newwidth != oldwidth)
		{

// vytvo�en� v�stupn�ho bufferu pro horizont�ln� konverzi
			BYTE* buf2 = (BYTE*)MemGet(newwidth*oldheight*5);
			if (buf2 == NULL)
			{
				MemFree(buf);
				return false;
			}

// proveden� horizont�ln� konverze
			if (!HorizontalFilter(buf, oldwidth, oldheight, buf2, newwidth))
			{
				MemFree(buf);
				MemFree(buf2);
				return false;
			}

// p�enesen� na vstupn� buffer
			MemFree(buf);
			buf = buf2;
		}

// test, zda je pot�eba prov�d�t vertik�ln� konverzi
		if (newheight != oldheight)
		{

// vytvo�en� v�stupn�ho bufferu pro vertik�ln� konverzi
			BYTE* buf2 = (BYTE*)MemGet(newheight*newwidth*5);
			if (buf2 == NULL)
			{
				MemFree(buf);
				return false;
			}

// proveden� vertik�ln� konverze
			if (!VerticalFilter(buf, newwidth, oldheight, buf2, newheight))
			{
				MemFree(buf);
				MemFree(buf2);
				return false;
			}

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
			if (buf2 == NULL)
			{
				MemFree(buf);
				return false;
			}

// proveden� vertik�ln� konverze
			if (!VerticalFilter(buf, oldwidth, oldheight, buf2, newheight))
			{
				MemFree(buf);
				MemFree(buf2);
				return false;
			}

// p�enesen� na vstupn� buffer
			MemFree(buf);
			buf = buf2;
		}

// test, zda je pot�eba prov�d�t horizont�ln� konverzi
		if (newwidth != oldwidth)
		{

// vytvo�en� v�stupn�ho bufferu pro horizont�ln� konverzi
			BYTE* buf2 = (BYTE*)MemGet(newwidth*newheight*5);
			if (buf2 == NULL)
			{
				MemFree(buf);
				return false;
			}

// proveden� horizont�ln� konverze
			if (!HorizontalFilter(buf, oldwidth, newheight, buf2, newwidth))
			{
				MemFree(buf);
				MemFree(buf2);
				return false;
			}

// p�enesen� na vstupn� buffer
			MemFree(buf);
			buf = buf2;
		}
	}

// vytvo�en� nov�ho (pr�zdn�ho) obr�zku
	CPicture oldpic(*this);

	if (!New(newwidth, newheight))
	{
		MemFree(buf);
		return false;
	}

// importov�n� obr�zku z datov�ho bufferu
	if (!ImportTrueAlphaBeta(buf, dith))
	{
		*this = oldpic;
		MemFree(buf);
		return false;
	}

// zru�en� datov�ho bufferu
	MemFree(buf);

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
	return BackCol;					// neplatn� offset
}

BYTE _fastcall CPicture::Get(const int x, const int y) const
{
	ASSERT(pData->Param != PicParamComp);

	if (IsValid(x, y))				// je index platn�?
	{
		return pData->Data[x + y*pData->Width];	// prvek
	}
	return BackCol;					// neplatn� offset
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


/////////////////////////////////////////////////////////////////////////////
// na�ten� obr�zku ze souboru, comp=komprimovat (FALSE=chyba, obsah nezm�n�n)

bool CPicture::LoadFile(CText jmeno, BOOL comp)
{
// otev�en� souboru mapovan�ho do pam�ti (uzav�en p�i destrukci!)
	CFileMap file;			// vstupn� soubor (BMP nebo JPG)
	CBuf buf;				// buffer pro dekomprimaci JPEG
	BOOL jpeg = FALSE;		// p��znak obr�zku JPEG

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

// na�ten� souboru JPG
		if (!JPEG::Decomp(&file, &buf)) return false;

// velikost souboru bez z�hlav� 14 bajt�
		size = buf.Size() - 14;

// identifika�n� z�hlav�
		hd = (char*)buf.Adr();
		bmp = (BITMAPINFO*)(buf.Adr() + 14);
	
// kontrola velikosti souboru a z�hlav�
		if ((size < 50) || 
			(hd[0] != 'B') || 
			(hd[1] != 'M') ||
			!TestBMP(bmp))
		{
			MemFree(buf.Adr());
			return false;
		}

// p��znak, �e je obr�zek JPEG
		jpeg = TRUE;
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
		if (bmp2 == NULL)
		{
			if (jpeg) MemFree(buf.Adr());
			return false;
		}
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
		if (bmp2 == NULL)
		{
			if (jpeg) MemFree(buf.Adr());
			return false;
		}
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
		if (jpeg) MemFree(buf.Adr());
		return false;
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

// uvoln�n� p��padn�ho bufferu
	MemFree(bmp2);

// odpojen� star�ch dat
	detach(pData);

// p�ipojen� nov�ch dat
	pData = newdata;

// komprimace dat (chyba nevad�)
	if (comp) Comp();

// zru�en� bufferu JPEG
	if (jpeg) MemFree(buf.Adr());

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
	bmp->biXPelsPerMeter = 5906;				// horizont�ln� 150 bod� na palec
	bmp->biYPelsPerMeter = 5906;				// vertik�ln� 150 bod� na palec
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


/***************************************************************************\
*																			*
*								Buffer obr�zk�								*
*																			*
\***************************************************************************/

////////////////////////////////////////////////////////////////////
// p�id�n� z�znamu o vlo�en� polo�ky do UNDO bufferu

template <> bool CBuffer<CPicture>::UndoAddIns(int index)
{
	return Undo.AddPicIns(index);
}



////////////////////////////////////////////////////////////////////
// vytvo�en� pr�zdn� polo�ky (vrac� index polo�ky, <0 = chyba)
// prov�d� z�znam do UNDO bufferu

int _fastcall CBufPic::New(int width, int height)
{
// vytvo�en� nov� polo�ky
	int result = New();
	if (result >= 0)
	{

// inicializace objektu
		if (!m_Data[result].Clear(width, height))
		{
			Del(result);
			if (m_Undo) Undo.DelAll();
			result = -1;
		}
	}

// index nov� polo�ky (nebo <0 = chyba)
	return result;
}

////////////////////////////////////////////////////////////////////
// p�id�n� polo�ky (vrac� index polo�ky, <0 = chyba pam�ti)
// prov�d� z�znam do UNDO bufferu

int _fastcall CBufPic::Add(PICTUREDATA* data)
{
	CPicture pic(data);
	int result = Add(pic);		// result mus� b�t odd�len, pro inline by se �patn� volal destruktor
	return result;
}
