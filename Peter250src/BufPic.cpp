
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
	MemFill(EmptyPictureData.Data, ICONSIZE, BackCol);
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
	MemFill(pData->Data, pData->Width * pData->Height, BackCol);
	
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
	MemFill(pData->Data, pData->Width * pData->Height, BackCol);
	
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
					MemFill(dst, k, BackCol);
					dst += k;
					src += oldwidth;
				}
			}
		}

// vymazání zbylých linek
		i = height - oldheight;
		if (i > 0)
		{
			MemFill(dst, i*width, BackCol);
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
// exportování obrázku do bufferu TRUECOLORALPHABETA (vrací buffer, NULL=chyba pamìti)

BYTE* CPicture::ExportTrueAlphaBeta()
{
// zajištìní dekomprimace
	if (!DeComp()) return NULL;

// vytvoøení bufferu
	int size = pData->Width * pData->Height;
	BYTE* buf = (BYTE*)MemGet(size * 5);
	if (buf == NULL) return NULL;

// pøevod na TRUE COLOR ALPHA
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
			*dst = 0;		// B: modrá
			dst++;
			*dst = 0;		// G: zelená
			dst++;
			*dst = 0;		// R: èervená
			dst++;
			*dst = 255;		// A: prùhlednost
			dst++;
			*dst = 0;		// S: stín
			dst++;
			break;

		case ShadCol:
			*dst = 0;		// B: modrá
			dst++;
			*dst = 0;		// G: zelená
			dst++;
			*dst = 0;		// R: èervená
			dst++;
			*dst = 0;		// A: prùhlednost
			dst++;
			*dst = 255;		// S: stín
			dst++;
			break;

		default:
			*dst = rgb[col].rgbBlue;		// B: modrá
			dst++; 
			*dst = rgb[col].rgbGreen;		// G: zelená
			dst++;
			*dst = rgb[col].rgbRed;			// R: èervená
			dst++;
			*dst = 0;						// A: prùhlednost
			dst++;
			*dst = 0;						// S: stín
			dst++;
		}
	}

	return buf;
}


/////////////////////////////////////////////////////////////////////////////
// importování obrázku z bufferu TRUECOLORALPHABETA (vrací FALSE=chyba pamìti)

bool CPicture::ImportTrueAlphaBeta(BYTE* buf, BOOL dith)
{
// zajištìní prázdného obrázku
	if (!New()) return false;

// rozmìry obrázku
	int width = pData->Width;
	int height = pData->Height;

// ukazatele dat
	BYTE* src = buf;
	BYTE* dst = pData->Data;

// konverze s ditheringem
	if (dith)
	{

// pøíprava bufferu odchylky pro dithering
		int* odch = (int*)MemGet((3*width + 6 + 10) * sizeof(int));
		if (odch == NULL) return false;
		MemFill(odch, (3*width + 6 + 10) * sizeof(int), 0);
		
// cyklus pøes všechny linky		
		for (int y = height; y > 0; y--)
		{
			int* odch0 = odch + 3;			// ukazatel v bufferu odchylky

// cyklus pøes všechny body na lince
			for (int x = width; x > 0; x--)
			{

// pozadí
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

// stín
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

// požadovaná barva
						BYTE b = src[0];			// modrá složka
						BYTE g = src[1];			// zelená složka
						BYTE r = src[2];			// èervená složka

// zkorigovaná barva
						int b2 = b - (odch0[-3] + odch0[0] + odch0[3])*5/8;		// modrá složka
						int g2 = g - (odch0[-2] + odch0[1] + odch0[4])*5/8;		// zelená složka
						int r2 = r - (odch0[-1] + odch0[2] + odch0[5])*5/8;		// èervená složka

// omezení pøeteèení barvy
						if ((DWORD)b2 > 255) { if (b2 < 0) b2 = 0; else b2 = 255; }
						if ((DWORD)g2 > 255) { if (g2 < 0) g2 = 0; else g2 = 255; }
						if ((DWORD)r2 > 255) { if (r2 < 0) r2 = 0; else r2 = 255; }

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
						*dst = col;
					}
				}
				dst++;
				src += 5;
			}
		}

// zrušení bufferu odchylky pro dithering
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

// pøíznak operace OK
	return true;
}

/////////////////////////////////////////////////////////////////////////
// pomocné funkce pro filtry

#define		Max(x,y) (((x) > (y)) ? (x) : (y))	// maximální hodnota
#define		Min(x,y) (((x) < (y)) ? (x) : (y))	// minimální hodnota

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
// horizontální filtr (vrací FALSE=chyba pamìti)

bool HorizontalFilter(BYTE* oldbuf, int oldwidth, int oldheight, BYTE* newbuf, int newwidth)
{
// pøíprava ukazatelù podpory
	double xfactor = (double)newwidth/oldwidth;
	double support = FSUPPORT/xfactor;
	if (support < FSUPPORT) support = FSUPPORT;

// buffer konverze bodù
	ContribInfo* cinfo = (ContribInfo*)MemGet(Round(support*2+3)*sizeof(ContribInfo) + 128);
	if (cinfo == NULL) return false;

// konverzní faktory
	double scale = Max(1.0/xfactor, 1.0);
	support = Max(scale * FSUPPORT, 0.5);
	if (support <= 0.5)
	{
		support = 0.5;
		scale = 1.0;
	}
	support += 1.0e-7;							// korekce pro zaokrouhlování

// cyklus pøes všechny cílové body
	for (int x = 0; x < newwidth; x++)
	{

// pøíprava pro pøevodní tabulku bodu
		double density = 0.0;					// hustota bodu
		int n = 0;								// èítaè bodù
		double center = (double)x/xfactor;		// zdrojová souøadnice bodu
		int start = Round(center - support);	// poèáteèní bod
		int end = Round(center + support);		// koncový bod

// konverzní tabulka vah bodù
		for (int i = Max(start, 0); i < Min(end + 1, oldwidth); i++)
		{
			cinfo[n].pixel = i;					// èíslo zdrojového bodu
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

// adresa cílového bodu ke konverzi
		BYTE* dst = newbuf + x*5;

// cyklus pøes všechny linky
		for (int y = 0; y < oldheight; y++)
		{

// pøíprava støadaèù vah barev
			double blue = 0;
			double green = 0;
			double red = 0;
			double alpha = 0;
			double shadow = 0;

// výpoèet barevných složek
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

// korekce barev pøi prùhlednosti a stínu
			double as = alpha + shadow;

			if ((as > 0.0001) && (as < 254))
			{
//				double kor = sqrt(1/(1 - as/255));
				double kor = 1/(1 - as/1024);

				blue *= kor;
				green *= kor;
				red *= kor;
			}

// normalizace barevných složek
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

// zrušení bufferu konverze bodù
	MemFree(cinfo);

// pøíznak operace OK
	return true;
}

/////////////////////////////////////////////////////////////////////////
// vertikální filtr (vrací FALSE=chyba pamìti)

bool VerticalFilter(BYTE* oldbuf, int oldwidth, int oldheight, BYTE* newbuf, int newheight)
{
// pøíprava ukazatelù podpory
	double yfactor = (double)newheight/oldheight;
	double support = FSUPPORT/yfactor;
	if (support < FSUPPORT) support = FSUPPORT;

// buffer konverze bodù
	ContribInfo* cinfo = (ContribInfo*)MemGet(Round(support*2+3)*sizeof(ContribInfo) + 128);
	if (cinfo == NULL) return false;

// konverzní faktory
	double scale = Max(1.0/yfactor, 1.0);
	support = Max(scale * FSUPPORT, 0.5);
	if (support <= 0.5)
	{
		support = 0.5;
		scale = 1.0;
	}
	support += 1.0e-7;							// korekce pro zaokrouhlování

// cyklus pøes všechny cílové linky
	for (int y = 0; y < newheight; y++)
	{

// pøíprava pro pøevodní tabulku bodu
		double density = 0.0;					// hustota bodu
		int n = 0;								// èítaè bodù
		double center = (double)y/yfactor;		// zdrojová souøadnice bodu
		int start = Round(center - support);	// poèáteèní bod
		int end = Round(center + support);		// koncový bod

// konverzní tabulka vah bodù
		for (int i = Max(start, 0); i < Min(end + 1, oldheight); i++)
		{
			cinfo[n].pixel = i;					// èíslo zdrojového bodu
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

// adresa cílového bodu ke konverzi
		BYTE* dst = newbuf + y*oldwidth*5;

// cyklus pøes všechny bodu na lince
		for (int x = 0; x < oldwidth; x++)
		{

// pøíprava støadaèù vah barev
			double blue = 0;
			double green = 0;
			double red = 0;
			double alpha = 0;
			double shadow = 0;

// výpoèet barevných složek
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

// korekce barev pøi prùhlednosti a stínu
			double as = alpha + shadow;

			if ((as > 0.0001) && (as < 254))
			{
//				double kor = sqrt(1/(1 - as/255));
				double kor = 1/(1 - as/1024);

				blue *= kor;
				green *= kor;
				red *= kor;
			}

// normalizace barevných složek
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

// zrušení bufferu konverze bodù
	MemFree(cinfo);

// pøíznak operace OK
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// zmìna velikosti obrázku (int = interpolovat, dith = ditherovat)
// pøi chybì pamìti vrátí FALSE, obsah bude nezmìnìn

bool CPicture::Zoom(int newwidth, int newheight, BOOL inter, BOOL dith)
{
// úschova starých rozmìrù
	int oldwidth = pData->Width;
	int oldheight = pData->Height;

// test, zda bude zmìna velikosti obrázku
	if ((newwidth == oldwidth) && (newheight == oldheight)) return true;

// zajištìní dekomprimace
	if (!DeComp()) return false;

// bude konverze bez interpolace
	if (!inter)
	{

// vytvoøení bufferu horizontální konverzní tabulky
		int* konv = (int*)MemGet(newwidth * sizeof(int));
		if (konv == NULL) return false;

// vytvoøení nového (prázdného) obrázku
		CPicture pic;
		if (!pic.New(newwidth, newheight))
		{
			MemFree(konv);
			return false;
		}

// inicializace horizontální konverzní tabulky
		int i, j;
		double k = (double)oldwidth/newwidth;

		for (i = 0; i < newwidth; i++)
		{
			j = RoundM(i * k);
			if (j >= oldwidth) j = oldwidth - 1;
			konv[i] = j;
		}

// provedení konverze
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

// zrušení konverzní tabulky
		MemFree(konv);

// nastavení nového obrázku
		*this = pic;

		return true;
	}

// vytvoøení bufferu s daty TRUECOLOR
	BYTE* buf = ExportTrueAlphaBeta();
	if (buf == NULL) return false;

// pøi zvýšení výšky obrázku se konvertuje nejdøíve horizontálnì
	if (newheight >= oldheight)
	{

// test, zda je potøeba provádìt horizontální konverzi
		if (newwidth != oldwidth)
		{

// vytvoøení výstupního bufferu pro horizontální konverzi
			BYTE* buf2 = (BYTE*)MemGet(newwidth*oldheight*5);
			if (buf2 == NULL)
			{
				MemFree(buf);
				return false;
			}

// provedení horizontální konverze
			if (!HorizontalFilter(buf, oldwidth, oldheight, buf2, newwidth))
			{
				MemFree(buf);
				MemFree(buf2);
				return false;
			}

// pøenesení na vstupní buffer
			MemFree(buf);
			buf = buf2;
		}

// test, zda je potøeba provádìt vertikální konverzi
		if (newheight != oldheight)
		{

// vytvoøení výstupního bufferu pro vertikální konverzi
			BYTE* buf2 = (BYTE*)MemGet(newheight*newwidth*5);
			if (buf2 == NULL)
			{
				MemFree(buf);
				return false;
			}

// provedení vertikální konverze
			if (!VerticalFilter(buf, newwidth, oldheight, buf2, newheight))
			{
				MemFree(buf);
				MemFree(buf2);
				return false;
			}

// pøenesení na vstupní buffer
			MemFree(buf);
			buf = buf2;
		}
	}

// pøi snížení výšky obrázku se konvertuje nejdøíve vertikálnì
	else
	{

// test, zda je potøeba provádìt vertikální konverzi
		if (newheight != oldheight)
		{

// vytvoøení výstupního bufferu pro vertikální konverzi
			BYTE* buf2 = (BYTE*)MemGet(newheight*oldwidth*5);
			if (buf2 == NULL)
			{
				MemFree(buf);
				return false;
			}

// provedení vertikální konverze
			if (!VerticalFilter(buf, oldwidth, oldheight, buf2, newheight))
			{
				MemFree(buf);
				MemFree(buf2);
				return false;
			}

// pøenesení na vstupní buffer
			MemFree(buf);
			buf = buf2;
		}

// test, zda je potøeba provádìt horizontální konverzi
		if (newwidth != oldwidth)
		{

// vytvoøení výstupního bufferu pro horizontální konverzi
			BYTE* buf2 = (BYTE*)MemGet(newwidth*newheight*5);
			if (buf2 == NULL)
			{
				MemFree(buf);
				return false;
			}

// provedení horizontální konverze
			if (!HorizontalFilter(buf, oldwidth, newheight, buf2, newwidth))
			{
				MemFree(buf);
				MemFree(buf2);
				return false;
			}

// pøenesení na vstupní buffer
			MemFree(buf);
			buf = buf2;
		}
	}

// vytvoøení nového (prázdného) obrázku
	CPicture oldpic(*this);

	if (!New(newwidth, newheight))
	{
		MemFree(buf);
		return false;
	}

// importování obrázku z datového bufferu
	if (!ImportTrueAlphaBeta(buf, dith))
	{
		*this = oldpic;
		MemFree(buf);
		return false;
	}

// zrušení datového bufferu
	MemFree(buf);

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
	return BackCol;					// neplatný offset
}

BYTE _fastcall CPicture::Get(const int x, const int y) const
{
	ASSERT(pData->Param != PicParamComp);

	if (IsValid(x, y))				// je index platný?
	{
		return pData->Data[x + y*pData->Width];	// prvek
	}
	return BackCol;					// neplatný offset
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


/////////////////////////////////////////////////////////////////////////////
// naètení obrázku ze souboru, comp=komprimovat (FALSE=chyba, obsah nezmìnìn)

bool CPicture::LoadFile(CText jmeno, BOOL comp)
{
// otevøení souboru mapovaného do pamìti (uzavøen pøi destrukci!)
	CFileMap file;			// vstupní soubor (BMP nebo JPG)
	CBuf buf;				// buffer pro dekomprimaci JPEG
	BOOL jpeg = FALSE;		// pøíznak obrázku JPEG

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

// naètení souboru JPG
		if (!JPEG::Decomp(&file, &buf)) return false;

// velikost souboru bez záhlaví 14 bajtù
		size = buf.Size() - 14;

// identifikaèní záhlaví
		hd = (char*)buf.Adr();
		bmp = (BITMAPINFO*)(buf.Adr() + 14);
	
// kontrola velikosti souboru a záhlaví
		if ((size < 50) || 
			(hd[0] != 'B') || 
			(hd[1] != 'M') ||
			!TestBMP(bmp))
		{
			MemFree(buf.Adr());
			return false;
		}

// pøíznak, že je obrázek JPEG
		jpeg = TRUE;
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

// vytvoøení nového bufferu
	PICTUREDATA* newdata = NewPictureData(width, height);
	if (newdata == NULL)
	{
		MemFree(bmp2);
		if (jpeg) MemFree(buf.Adr());
		return false;
	}

// vygenerování konverzní tabulky palet
	if (bits <= 8)
	{
		GenKonvPal(bmp);
	}

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
		srcinc = ((width+7)/8 + 3) & ~3;	// pøírustek zdrojové adresy

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
		break;

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
	if (comp) Comp();

// zrušení bufferu JPEG
	if (jpeg) MemFree(buf.Adr());

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
	bmp->biXPelsPerMeter = 5906;				// horizontálnì 150 bodù na palec
	bmp->biYPelsPerMeter = 5906;				// vertikálnì 150 bodù na palec
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


/***************************************************************************\
*																			*
*								Buffer obrázkù								*
*																			*
\***************************************************************************/

////////////////////////////////////////////////////////////////////
// pøidání záznamu o vložení položky do UNDO bufferu

template <> bool CBuffer<CPicture>::UndoAddIns(int index)
{
	return Undo.AddPicIns(index);
}



////////////////////////////////////////////////////////////////////
// vytvoøení prázdné položky (vrací index položky, <0 = chyba)
// provádí záznam do UNDO bufferu

int _fastcall CBufPic::New(int width, int height)
{
// vytvoøení nové položky
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

// index nové položky (nebo <0 = chyba)
	return result;
}

////////////////////////////////////////////////////////////////////
// pøidání položky (vrací index položky, <0 = chyba pamìti)
// provádí záznam do UNDO bufferu

int _fastcall CBufPic::Add(PICTUREDATA* data)
{
	CPicture pic(data);
	int result = Add(pic);		// result musí být oddìlen, pro inline by se špatnì volal destruktor
	return result;
}
