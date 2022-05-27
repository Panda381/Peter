
#include "Main.h"

/***************************************************************************\
*																			*
*						Provádìní programu - obrázek						*
*																			*
\***************************************************************************/

#pragma optimize("t", on)			// optimalizace na maximální rychlost


/////////////////////////////////////////////////////////////////////////////
// naètení výøezu obrázku z plochy

void _fastcall FPicGet(CPicture& pic)
{
// vyprázdnìní obrázku
	pic.Empty();

// naètení parametrù výøezu
	int x = FIntX();				// poèátek výøezu X
	int y = FIntY();				// poèátek výøezu Y
	int width = FIntX();			// šíøka výøezu
	int height = FIntY();			// výška výøezu

// korekce pro údaje mimo rozsah
	if (width >= 100000) width = Width;
	if (height >= 100000) height = Height;

// kontrola platnosti parametrù
	if ((width <= 0) || (height <= 0)) return;

// okraje obrázku vlevo a vpravo, šíøka støední èásti
	int width0 = width;

	int width1 = 0;
	if (x < 0)
	{
		width1 = -x;
		if (width1 > width0) width1 = width0;
		width0 -= width1;
		x = 0;
	}

	int width2 = 0;
	if ((x + width0) > Width)
	{
		width2 = x + width0 - Width;
		if (width2 > width0) width2 = width0;
		width0 -= width2;
	}
	
// okraje obrázku dole a nahoøe, výška støední èásti
	int height0 = height;

	int height1 = 0;
	if (y < 0)
	{
		height1 = -y;
		if (height1 > height0) height1 = height0;
		height0 -= height1;
		y = 0;
	}

	int height2 = 0;
	if ((y + height0) > Height)
	{
		height2 = y + height0 - Height;
		if (height2 > height0) height2 = height0;
		height0 -= height2;
	}

// vytvoøení obrázku
	pic.New(width, height);

// vymazání dolních linek
	BYTE* dst = pic.DataData();
	MemFill(dst, height1*width, BlackCol);
	dst += height1*width;

// pøenesení dat obrázku
	BYTE* src = BackBuf + x + y*WidthByte;
	for (; height0 > 0; height0--)
	{
		MemFill(dst, width1, BlackCol);
		dst += width1;

		MemCopy(dst, src, width0);
		dst += width0;
		src += WidthByte;

		MemFill(dst, width2, BlackCol);
		dst += width2;
	}

// vymazání dolních linek
	MemFill(dst, height2*width, BlackCol);
}


/***************************************************************************\
*																			*
*									funkce									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// funkce s návratem obrázku

void _fastcall FPicFunc(CPicture& pic)
{
// úschova indexu volané funkce
	int data = ExecItem[-1].Data;

// úschova indexu promìnné s návratovou hodnotou
	int res = ExecItem[-1].List;

// inicializace lokálních promìnných
	FCommand();

// úschova ukazatele programu
	EXECITEM* oldexe = ExecItem;

// nová adresa programu
	ExecItem = ProgBuf + data;

// kontrola hloubky vnoøení
	Hloubka--;
	if (Hloubka >= 0)
	{

// vyvolání funkce
		FCommand();
	}
	Hloubka++;

// návrat adresy programu
	ExecItem = oldexe;

// zrušení požadavku o pøerušení
	Break &= ~(BREAKFUNC | BREAKWHILE);

// návrat výsledku operace
	pic = Picture[Picture.Num() - res];

// zrušení lokálních promìnných
	FCommand();
}


/***************************************************************************\
*																			*
*								promìnné									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// prázdný obrázek

void _fastcall FPicEmpty(CPicture& pic) { pic.Empty(); };


/////////////////////////////////////////////////////////////////////////////
// globální obrázek (Data = index)

void _fastcall FPicObj(CPicture& pic)
{
// index obrázku
	int inx = ExecItem[-1].Data;

// nastavení obrázku
	pic = Picture[inx];
}


/////////////////////////////////////////////////////////////////////////////
// globalní obrázek v seznamu (Data = index, List = seznam)

void _fastcall FPicObjList(CPicture& pic)
{
// index seznamu
	int list = ExecItem[-1].List;

// index obrázku, inkrementace indexu seznamu
	int inx = ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// nastavení obrázku
	pic = Picture[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lokální obrázek (Data = index)

void _fastcall FPicLoc(CPicture& pic)
{
// index obrázku
	int inx = Picture.Num() - ExecItem[-1].Data;

// nastavení obrázku
	pic = Picture[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lokální obrázek v seznamu (Data = index, List = seznam)

void _fastcall FPicLocList(CPicture& pic)
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index obrázku, inkrementace indexu seznamu
	int inx = Picture.Num() - ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// nastavení obrázku
	pic = Picture[inx];
}

/////////////////////////////////////////////////////////////////////////////
// naètení obrázku ze souboru

void _fastcall FGetFilePicture(CPicture& pic)
{
	pic.Empty();
	pic.LoadFile(false);
}

/////////////////////////////////////////////////////////////////////////////
// horizontální pøevrácení obrázku

void _fastcall FPicXFlip(CPicture& pic)
{
	FPicture(pic);
	pic.XFlip();
}

/////////////////////////////////////////////////////////////////////////////
// vertikální pøevrácení obrázku

void _fastcall FPicYFlip(CPicture& pic)
{
	FPicture(pic);
	pic.YFlip();
}


/////////////////////////////////////////////////////////////////////////////
// zmìna velikosti obrázku

void _fastcall FPicZoom(CPicture& pic)
{
// naètení obrázku ke konverzi
	FPicture(pic);

// naètení požadovaných rozmìrù a pøepínaèù obrázku
	int newwidth = FIntX();			// nová šíøka obrázku
	int newheight = FIntY();		// nová výška obrázku
	bool inter = FBool();			// interpolace
	bool dith = FBool();			// dithering

// test, zda má být šíøka a výška implicitní
	bool impwidth = ((newwidth > 0x8000) || (newwidth < -0x8000));
	bool impheight = ((newheight > 0x8000) || (newheight < -0x8000));

// obrázek se nemìní
	if (impwidth && impheight) return;

// aktuální šíøka a výška obrázku
	int oldwidth = pic.Width();
	int oldheight = pic.Height();

// výsledkem bude prázdný obrázek
	if ((oldwidth <= 0) || (oldheight <= 0) || (newwidth == 0) || (newheight == 0))
	{
		pic.Empty();
		return;
	}

// implicitní šíøka
	if (impwidth)
	{
		newwidth = Round(((double)newheight/oldheight) * oldwidth);
		if (newwidth < -0x8000) newwidth = -0x8000;
		if (newwidth > 0x8000) newwidth = 0x8000;
	}

// implicitní výška
	if (impheight)
	{
		newheight = Round(((double)newwidth/oldwidth) * oldheight);
		if (newheight < -0x8000) newheight = -0x8000;
		if (newheight > 0x8000) newheight = 0x8000;
	}

// kontrola po pøepoètech, zda nebude prázdný obrázek
	if ((newwidth == 0) || (newheight == 0))
	{
		pic.Empty();
		return;
	}

// test, zda bude horizontální pøevrácení
	bool xflip1 = false;
	bool xflip2 = false;
	if (newwidth < 0)
	{
		newwidth = -newwidth;

		if (newwidth > oldwidth)
		{
			xflip1 = true;
		}
		else
		{
			xflip2 = true;
		}
	}

// test, zda bude vertikální pøevrácení
	bool yflip1 = false;
	bool yflip2 = false;
	if (newheight < 0)
	{
		newheight = -newheight;

		if (newheight > oldheight)
		{
			yflip1 = true;
		}
		else
		{
			yflip2 = true;
		}
	}

// provedení konverze s pøípadným pøevrácením
	if (xflip1) pic.XFlip();
	if (yflip1) pic.YFlip();
	pic.Zoom(newwidth, newheight, inter, dith);
	if (xflip2) pic.XFlip();
	if (yflip2) pic.YFlip();
}

/////////////////////////////////////////////////////////////////////////////
// zmìna jasu obrázku

void _fastcall FPicLevel(CPicture& pic)
{
// naètení obrázku ke konverzi
	FPicture(pic);

// naètení požadovaného jasu obrázku
	double koef = FNum();			// koeficient jasu obrázku

// naètení pøíznaku ditheringu
	bool dith = FBool();			// dithering

// konverze jasu obrázku
	pic.Level(koef, dith);
}


/////////////////////////////////////////////////////////////////////////////
// otoèení obrázku

void _fastcall FPicRotate(CPicture& pic)
{
// naètení obrázku ke konverzi
	FPicture(pic);

// naètení požadovaného úhlu
	double angle = FNum();

// naètení pøíznaku interpolace
	bool inter = FBool();			// interpolace

// naètení pøíznaku ditheringu
	bool dith = FBool();			// dithering

// otoèení obrázku
	pic.Rotate(angle, inter, dith);
}


/////////////////////////////////////////////////////////////////////////////
// naètení obrázku okna

void _fastcall FGetWindowPic(CPicture& pic)
{
	if (Win.IsValid(WindowID))
	{
		pic = Win[WindowID].Picture;
	}
	else
	{
		pic.Empty();
	}
}

/////////////////////////////////////////////////////////////////////////////
// konverze plochy na obrázek

void _fastcall FMapPicture(CPicture& pic)
{
	CMap map;
	FMap(map);
	pic = map;
}

/////////////////////////////////////////////////////////////////////////////
// konverze ikony na obrázek

void _fastcall FIconPicture(CPicture& pic)
{
	CIcon icon;
	FIcon(icon);
	pic = icon;
}

/////////////////////////////////////////////////////////////////////////////
// maskování obrázku

void _fastcall FPicMask(CPicture& pic)
{
// naètení obrázku ke konverzi
	FPicture(pic);

// naètení masky
	CPicture mask;
	FPicture(mask);

// maskování obrázku
	pic.Mask(mask);
}

/////////////////////////////////////////////////////////////////////////////
// zámìna barev obrázku

void _fastcall FPicXCol(CPicture& pic)
{
// naètení obrázku ke konverzi
	FPicture(pic);

// naètení barev
	BYTE col1 = (BYTE)FNumCol();
	BYTE col2 = (BYTE)FNumCol();

// zámìna barev v obrázku
	pic.XCol(col1, col2);
}

/////////////////////////////////////////////////////////////////////////////
// náhrada barvy obrázku

void _fastcall FPicSCol(CPicture& pic)
{
// naètení obrázku ke konverzi
	FPicture(pic);

// naètení barev
	BYTE oldcol = (BYTE)FNumCol();
	BYTE newcol = (BYTE)FNumCol();

// náhrada barvy v obrázku
	pic.SCol(oldcol, newcol);
}


// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

/////////////////////////////////////////////////////////////////////////////
// zjištìní obrázku textury Direct3D objektu

void _fastcall FGetD3DTexture(CPicture& pic)
{
	pic.Empty();

	if (IsValidID())
	{
		D3DFITEM* item = GetID();
		int texture = item->Texture[0];
		if (texture >= 0)
		{
			pic = D3DT_Get(texture)->Pic;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// zjištìní obrázku textury Direct3D pozadí scény

void _fastcall FGetD3DSceneTexture(CPicture& pic)
{
	if (D3DBackText >= 0)
	{
		pic = D3DT_Get(D3DBackText)->Pic;
	}
	else
	{
		pic.Empty();
	}
}

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------
