
#include "Main.h"

/***************************************************************************\
*																			*
*						Prov�d�n� programu - obr�zek						*
*																			*
\***************************************************************************/

#pragma optimize("t", on)			// optimalizace na maxim�ln� rychlost


/////////////////////////////////////////////////////////////////////////////
// na�ten� v��ezu obr�zku z plochy

void _fastcall FPicGet(CPicture& pic)
{
// vypr�zdn�n� obr�zku
	pic.Empty();

// na�ten� parametr� v��ezu
	int x = FIntX();				// po��tek v��ezu X
	int y = FIntY();				// po��tek v��ezu Y
	int width = FIntX();			// ���ka v��ezu
	int height = FIntY();			// v��ka v��ezu

// korekce pro �daje mimo rozsah
	if (width >= 100000) width = Width;
	if (height >= 100000) height = Height;

// kontrola platnosti parametr�
	if ((width <= 0) || (height <= 0)) return;

// okraje obr�zku vlevo a vpravo, ���ka st�edn� ��sti
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
	
// okraje obr�zku dole a naho�e, v��ka st�edn� ��sti
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

// vytvo�en� obr�zku
	pic.New(width, height);

// vymaz�n� doln�ch linek
	BYTE* dst = pic.DataData();
	MemFill(dst, height1*width, BlackCol);
	dst += height1*width;

// p�enesen� dat obr�zku
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

// vymaz�n� doln�ch linek
	MemFill(dst, height2*width, BlackCol);
}


/***************************************************************************\
*																			*
*									funkce									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// funkce s n�vratem obr�zku

void _fastcall FPicFunc(CPicture& pic)
{
// �schova indexu volan� funkce
	int data = ExecItem[-1].Data;

// �schova indexu prom�nn� s n�vratovou hodnotou
	int res = ExecItem[-1].List;

// inicializace lok�ln�ch prom�nn�ch
	FCommand();

// �schova ukazatele programu
	EXECITEM* oldexe = ExecItem;

// nov� adresa programu
	ExecItem = ProgBuf + data;

// kontrola hloubky vno�en�
	Hloubka--;
	if (Hloubka >= 0)
	{

// vyvol�n� funkce
		FCommand();
	}
	Hloubka++;

// n�vrat adresy programu
	ExecItem = oldexe;

// zru�en� po�adavku o p�eru�en�
	Break &= ~(BREAKFUNC | BREAKWHILE);

// n�vrat v�sledku operace
	pic = Picture[Picture.Num() - res];

// zru�en� lok�ln�ch prom�nn�ch
	FCommand();
}


/***************************************************************************\
*																			*
*								prom�nn�									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// pr�zdn� obr�zek

void _fastcall FPicEmpty(CPicture& pic) { pic.Empty(); };


/////////////////////////////////////////////////////////////////////////////
// glob�ln� obr�zek (Data = index)

void _fastcall FPicObj(CPicture& pic)
{
// index obr�zku
	int inx = ExecItem[-1].Data;

// nastaven� obr�zku
	pic = Picture[inx];
}


/////////////////////////////////////////////////////////////////////////////
// globaln� obr�zek v seznamu (Data = index, List = seznam)

void _fastcall FPicObjList(CPicture& pic)
{
// index seznamu
	int list = ExecItem[-1].List;

// index obr�zku, inkrementace indexu seznamu
	int inx = ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// nastaven� obr�zku
	pic = Picture[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lok�ln� obr�zek (Data = index)

void _fastcall FPicLoc(CPicture& pic)
{
// index obr�zku
	int inx = Picture.Num() - ExecItem[-1].Data;

// nastaven� obr�zku
	pic = Picture[inx];
}


/////////////////////////////////////////////////////////////////////////////
// lok�ln� obr�zek v seznamu (Data = index, List = seznam)

void _fastcall FPicLocList(CPicture& pic)
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index obr�zku, inkrementace indexu seznamu
	int inx = Picture.Num() - ExecItem[-1].Data;
	inx += List.AutoIncInx(list);

// nastaven� obr�zku
	pic = Picture[inx];
}

/////////////////////////////////////////////////////////////////////////////
// na�ten� obr�zku ze souboru

void _fastcall FGetFilePicture(CPicture& pic)
{
	pic.Empty();
	pic.LoadFile(false);
}

/////////////////////////////////////////////////////////////////////////////
// horizont�ln� p�evr�cen� obr�zku

void _fastcall FPicXFlip(CPicture& pic)
{
	FPicture(pic);
	pic.XFlip();
}

/////////////////////////////////////////////////////////////////////////////
// vertik�ln� p�evr�cen� obr�zku

void _fastcall FPicYFlip(CPicture& pic)
{
	FPicture(pic);
	pic.YFlip();
}


/////////////////////////////////////////////////////////////////////////////
// zm�na velikosti obr�zku

void _fastcall FPicZoom(CPicture& pic)
{
// na�ten� obr�zku ke konverzi
	FPicture(pic);

// na�ten� po�adovan�ch rozm�r� a p�ep�na�� obr�zku
	int newwidth = FIntX();			// nov� ���ka obr�zku
	int newheight = FIntY();		// nov� v��ka obr�zku
	bool inter = FBool();			// interpolace
	bool dith = FBool();			// dithering

// test, zda m� b�t ���ka a v��ka implicitn�
	bool impwidth = ((newwidth > 0x8000) || (newwidth < -0x8000));
	bool impheight = ((newheight > 0x8000) || (newheight < -0x8000));

// obr�zek se nem�n�
	if (impwidth && impheight) return;

// aktu�ln� ���ka a v��ka obr�zku
	int oldwidth = pic.Width();
	int oldheight = pic.Height();

// v�sledkem bude pr�zdn� obr�zek
	if ((oldwidth <= 0) || (oldheight <= 0) || (newwidth == 0) || (newheight == 0))
	{
		pic.Empty();
		return;
	}

// implicitn� ���ka
	if (impwidth)
	{
		newwidth = Round(((double)newheight/oldheight) * oldwidth);
		if (newwidth < -0x8000) newwidth = -0x8000;
		if (newwidth > 0x8000) newwidth = 0x8000;
	}

// implicitn� v��ka
	if (impheight)
	{
		newheight = Round(((double)newwidth/oldwidth) * oldheight);
		if (newheight < -0x8000) newheight = -0x8000;
		if (newheight > 0x8000) newheight = 0x8000;
	}

// kontrola po p�epo�tech, zda nebude pr�zdn� obr�zek
	if ((newwidth == 0) || (newheight == 0))
	{
		pic.Empty();
		return;
	}

// test, zda bude horizont�ln� p�evr�cen�
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

// test, zda bude vertik�ln� p�evr�cen�
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

// proveden� konverze s p��padn�m p�evr�cen�m
	if (xflip1) pic.XFlip();
	if (yflip1) pic.YFlip();
	pic.Zoom(newwidth, newheight, inter, dith);
	if (xflip2) pic.XFlip();
	if (yflip2) pic.YFlip();
}

/////////////////////////////////////////////////////////////////////////////
// zm�na jasu obr�zku

void _fastcall FPicLevel(CPicture& pic)
{
// na�ten� obr�zku ke konverzi
	FPicture(pic);

// na�ten� po�adovan�ho jasu obr�zku
	double koef = FNum();			// koeficient jasu obr�zku

// na�ten� p��znaku ditheringu
	bool dith = FBool();			// dithering

// konverze jasu obr�zku
	pic.Level(koef, dith);
}


/////////////////////////////////////////////////////////////////////////////
// oto�en� obr�zku

void _fastcall FPicRotate(CPicture& pic)
{
// na�ten� obr�zku ke konverzi
	FPicture(pic);

// na�ten� po�adovan�ho �hlu
	double angle = FNum();

// na�ten� p��znaku interpolace
	bool inter = FBool();			// interpolace

// na�ten� p��znaku ditheringu
	bool dith = FBool();			// dithering

// oto�en� obr�zku
	pic.Rotate(angle, inter, dith);
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� obr�zku okna

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
// konverze plochy na obr�zek

void _fastcall FMapPicture(CPicture& pic)
{
	CMap map;
	FMap(map);
	pic = map;
}

/////////////////////////////////////////////////////////////////////////////
// konverze ikony na obr�zek

void _fastcall FIconPicture(CPicture& pic)
{
	CIcon icon;
	FIcon(icon);
	pic = icon;
}

/////////////////////////////////////////////////////////////////////////////
// maskov�n� obr�zku

void _fastcall FPicMask(CPicture& pic)
{
// na�ten� obr�zku ke konverzi
	FPicture(pic);

// na�ten� masky
	CPicture mask;
	FPicture(mask);

// maskov�n� obr�zku
	pic.Mask(mask);
}

/////////////////////////////////////////////////////////////////////////////
// z�m�na barev obr�zku

void _fastcall FPicXCol(CPicture& pic)
{
// na�ten� obr�zku ke konverzi
	FPicture(pic);

// na�ten� barev
	BYTE col1 = (BYTE)FNumCol();
	BYTE col2 = (BYTE)FNumCol();

// z�m�na barev v obr�zku
	pic.XCol(col1, col2);
}

/////////////////////////////////////////////////////////////////////////////
// n�hrada barvy obr�zku

void _fastcall FPicSCol(CPicture& pic)
{
// na�ten� obr�zku ke konverzi
	FPicture(pic);

// na�ten� barev
	BYTE oldcol = (BYTE)FNumCol();
	BYTE newcol = (BYTE)FNumCol();

// n�hrada barvy v obr�zku
	pic.SCol(oldcol, newcol);
}


// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� obr�zku textury Direct3D objektu

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
// zji�t�n� obr�zku textury Direct3D pozad� sc�ny

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
// --------------------- konec vypnut� pro MINI verzi -------------------
