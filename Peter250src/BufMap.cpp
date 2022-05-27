
#include "Main.h"

/***************************************************************************\
*																			*
*									Plocha									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inicializa�n� pr�zdn� plocha (modifikuje se po�et referenc�!)
// Pr�zdn� plocha ukazuje na pr�zdnou ikonu 0!

MAPDATA	EmptyMapData		= { 1, 1, 1, 0, 0 };
const CMap		EmptyMap;			// pr�zdn� plocha


////////////////////////////////////////////////////////////////////
// vytvo�en� dat plochy (p�i chyb� pam�ti vrac� NULL)

MAPDATA* _fastcall NewMapData(int width, int height)
{
	ASSERT((width > 0) && (height > 0));

// vytvo�en� dat plochy
	MAPDATA* data = (MAPDATA*)MemGet(SIZEOFMAPDATA + width*height*SIZEOFMAPITEM);
	if (data != NULL)
	{

// nastaven� parametr� plochy
		data->Refer = 1;				// po�et referenc�
		data->Width = width;			// ���ka
		data->Height = height;			// v��ka
	}

	return data;
}


/////////////////////////////////////////////////////////////////////////////
// zru�en� dat plochy (odd�leno kv�li lep�� optimalizaci)

void _fastcall DelMapData(MAPDATA* data)
{
	ASSERT(data != NULL);

	MemFree(data);
}


/////////////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CMap::CMap() 
{ 
	attach(&EmptyMapData); 
};

CMap::CMap(const CMap& src) 
{ 
	attach(src.pData); 
};

CMap::CMap(MAPDATA* data) 
{ 
	attach(data); 
};

CMap::~CMap() 
{ 
	detach(pData); 
};


/////////////////////////////////////////////////////////////////////////////
// statick� konstruktor a destruktor

void CMap::Init()
{ 
	attach(&EmptyMapData);
};

void _fastcall CMap::Init(const CMap& src) 
{ 
	attach(src.pData); 
};

void _fastcall CMap::Init(MAPDATA* data)
{ 
	attach(data); 
};

bool _fastcall CMap::Init(int width, int height)	// p�i chyb� pam�ti vr�t� FALSE, plocha nen� vytvo�ena
{
	pData = NewMapData(width, height);
	return (pData != NULL);
};

void CMap::Term()
{ 
	detach(pData); 
};


/////////////////////////////////////////////////////////////////////////////
// vypr�zdn�n� plochy (uvoln�n� dat)

void CMap::Empty()
{ 
	Term();
	Init();
}


/////////////////////////////////////////////////////////////////////////////
// vymaz�n� obsahu plochy (napln�n� ikonami 0, vynulovan� parametry), zajist� p�ivlastn�n� bufferu,
// p�i chyb� pam�ti vrac� FALSE, p�vodn� obsah nezm�n�n

bool CMap::Clear()
{
// vytvo�en� nov�ho bufferu, je-li pot�eba
	if (!New()) return false;

// vymaz�n� bufferu
	MemFill(pData->Data, pData->Width * pData->Height * SIZEOFMAPITEM, 0);
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// vymaz�n� obsahu plochy s nastaven�m velikosti (napln�n� ikonami 0, vynulovan� parametry),
// zajist� p�ivlastn�n� bufferu, p�i chyb� pam�ti vrac� FALSE, p�vodn� obsah nezm�n�n

bool _fastcall CMap::Clear(int width, int height)
{
// vytvo�en� nov�ho bufferu, je-li pot�eba
	if (!New(width, height)) return false;

// vymaz�n� bufferu
	MemFill(pData->Data, pData->Width * pData->Height * SIZEOFMAPITEM, 0);
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// kopie do vlastn�ho bufferu p�ed modifikac�
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool CMap::CopyWrite()
{
// �schova ukazatel�
	MAPDATA* olddata = pData;		// adresa star�ch dat
	long* refer = &(olddata->Refer);// po�et referenc�

// test, zda je nutn� p�ivlastn�n�
	if (*refer > 1)					// je n�jak� jin� majitel?
	{

// vytvo�en� nov�ho bufferu
		MAPDATA* newdata = NewMapData(olddata->Width, olddata->Height);
		if (newdata == NULL) return false;

// p�enesen� dat
		MemCopy(newdata->Data, olddata->Data, olddata->Width * olddata->Height * SIZEOFMAPITEM);

// odpojen� star�ch dat
		detach(olddata);

// p�ipojen� nov�ch dat
		pData = newdata;
	}

// operace OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� nov� plochy se stejnou velikost� (p�ipraveno k z�pisu, data jsou n�hodn�)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool CMap::New()
{
// ukazatel na star� data
	MAPDATA* olddata = pData;

// test, zda je nutn� vytvo�en� nov�ho bufferu
	if (olddata->Refer > 1)			// na buffer je v�ce referenc�
	{

// vytvo�en� nov�ho bufferu
		MAPDATA* newdata = NewMapData(pData->Width, pData->Height);
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
// vytvo�en� nov� plochy (p�ipraveno k z�pisu, data jsou n�hodn�)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool _fastcall CMap::New(int width, int height)
{
// ukazatel na star� data
	MAPDATA* olddata = pData;

// test, zda je nutn� vytvo�en� nov�ho bufferu
	if ((olddata->Refer > 1) ||			// na buffer je v�ce referenc�
		(olddata->Width != width) ||	// nesouhlas� ���ka
		(olddata->Height != height))	// nesouhlas� v��ka
	{

// vytvo�en� nov�ho bufferu
		MAPDATA* newdata = NewMapData(width, height);
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
// zm�na velikosti plochy
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n

bool CMap::Resize(int width, int height)
{
// �schova star�ch rozm�r�
	int oldwidth = pData->Width;
	int oldheight = pData->Height;

// test, zda je pot�eba velikost plochy m�nit
	if ((width != oldwidth) || (height != oldheight))
	{

// vytvo�en� nov�ho bufferu
		MAPDATA* olddata = pData;
		MAPDATA* newdata = NewMapData(width, height);
		if (newdata == NULL) return false;

// p��prava po�tu ��dk� ke kopii
		int i = height;
		if (oldheight < i) i = oldheight;

// p��prava d�lky ��dku ke kopii
		int j = width;
		if (oldwidth < j) j = oldwidth;

// p��prava zbytku ��dku k vymaz�n�
		int k = width - j;

// p��rustek zdrojov� a c�lov� adresy
		int srcinc = oldwidth * SIZEOFMAPITEM;
		int dstinc = width * SIZEOFMAPITEM;

// p��prava zdrojov� a c�lov� adresy
		BYTE* src = (BYTE*)(olddata->Data);
		BYTE* dst = (BYTE*)(newdata->Data);

// kopie platn�ch ��dk� p�i shodn� d�lce ��dk�
		if (width == oldwidth)
		{
			i *= dstinc;
			MemCopy(dst, src, i);
			dst += i;
			src += i;
		}

// kopie platn�ch ��dk� p�i rozd�ln� d�lce ��dk�
		else
		{
			j *= SIZEOFMAPITEM;

			if (k == 0)
			{
				for (; i > 0; i--)
				{
					MemCopy(dst, src, j);
					dst += j;
					src += srcinc;
				}
			}
			else
			{
				k *= SIZEOFMAPITEM;

				for (; i > 0; i--)
				{
					MemCopy(dst, src, j);
					dst += j;
					MemFill(dst, k, 0);
					dst += k;
					src += srcinc;
				}
			}
		}

// vymaz�n� zbyl�ch ��dk�
		i = height - oldheight;
		if (i > 0)
		{
			MemFill(dst, i*dstinc, 0);
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
// poskytnut� prvku (s kontrolou platnosti offsetu/indexu)
// pro neplatn� prvek vrac� p��stup na prvek pr�zdn� plochy

const MAPITEM& _fastcall CMap::Get(const int off) const
{
	if (IsValid(off))				// je offset platn�?
	{
		return pData->Data[off];	// prvek na dan�m offsetu
	}
	return EmptyMapData.Data[0];	// neplatn� offset
}

const MAPITEM& _fastcall CMap::Get(const int x, const int y) const
{
	if (IsValid(x, y))				// je index platn�?
	{
		return pData->Data[x + y*pData->Width];	// prvek
	}
	return EmptyMapData.Data[0];	// neplatn� offset
}


////////////////////////////////////////////////////////////////////
// nastaven� prvku (s kontrolou platnosti offsetu/indexu)
// p�ed z�pisem je nutno zajistit p�ivlastn�n� bufferu!

void _fastcall CMap::Set(const int off, const MAPITEM& data)
{
	if (IsValid(off))				// je offset platn�?
	{
		pData->Data[off] = data;
	}
}

void _fastcall CMap::Set(const int x, const int y, const MAPITEM& data)
{
	if (IsValid(x, y))				// je index platn�?
	{
		pData->Data[x + y*pData->Width] = data;
	}
}


/////////////////////////////////////////////////////////////////////////////
// zv��en� reference na ikonu (v bufferu glob�ln�ch a lok�ln�ch objekt�)

void _fastcall RefIconInc(const int icon)
{
// nalezen� ikony v bufferu glob�ln�ch objekt�
	int index = BufObj.SrcDat(BufIcoID, icon);
	if (index >= 0)
	{
		BufObj[index].Refer++;
		return;
	}

// nalezen� ikony v bufferu lok�ln�ch objekt�
	index = BufLoc.SrcDat(BufIcoID, icon);
	if (index >= 0)
	{
		BufLoc[index].Refer++;
		return;
	}

// ikona nenalezena - to je chyba!!!!
	ASSERTERROR;
}


/////////////////////////////////////////////////////////////////////////////
// zv��en� reference na v�echny ikony v plo�e

void CMap::RefInc()
{
	MAPITEM* item = DataData();

	for (int i = pData->Height*pData->Width; i > 0; i--)
	{
		RefIconInc(item->Icon);
		item++;
	}
}


/////////////////////////////////////////////////////////////////////////////
// sn�en� reference na ikonu (v bufferu glob�ln�ch a lok�ln�ch objekt�)

void _fastcall RefIconDec(const int icon)
{
// nalezen� ikony v bufferu glob�ln�ch objekt�
	int index = BufObj.SrcDat(BufIcoID, icon);
	if (index >= 0)
	{
		BufObj[index].Refer--;
		return;
	}

// nalezen� ikony v bufferu lok�ln�ch objekt�
	index = BufLoc.SrcDat(BufIcoID, icon);
	if (index >= 0)
	{
		BufLoc[index].Refer--;
		return;
	}

// ikona nenalezena - to je chyba!!!!
	ASSERTERROR;
}


/////////////////////////////////////////////////////////////////////////////
// sn�en� reference na v�echny ikony v plo�e

void CMap::RefDec()
{
	MAPITEM* item = DataData();
	for (int i = pData->Height*pData->Width; i > 0; i--)
	{
		RefIconDec(item->Icon);
		item++;
	}
}


/////////////////////////////////////////////////////////////////////////////
// zru�en� pou�it� ikony v plo�e (index = index plochy v seznamu) - nahrad� ikonou 0
// ignoruje p��padnou chybu pam�ti!

void _fastcall CMap::DelIcon(int icon, int index, bool undo)
{
// nesm� to b�t ikona 0 (=z�kladn� ikona)
	ASSERT(icon > 0);
	if (icon <= 0) return;

// ukazatel polo�ek
	MAPITEM* item = DataData();

// ukazatel indexu polo�ky v plo�e
	int off = 0;

// p��znak proveden� kopie bufferu
	bool copy = false;

// cyklus p�es v�echny polo�ky plochy
	for (int i = pData->Height*pData->Width; i > 0; i--)
	{

// je to hledan� ikona?
		if (item->Icon == icon)
		{

// zaji�t�n� p�ivlastn�n� bufferu
			if (!copy)
			{
				if (!CopyWrite()) return;
				item = DataData() + off;
				copy = true;
			}

// sn�en� reference na ikonu z buffer�
			RefIconDec(icon);

// z�znam o zru�en� ikony do UNDO bufferu
			if (undo)
			{
				if (!Undo.AddMapIcoSet(index, off, icon))
				{
					Undo.DelAll();
				}
			}

// nastaven� nov� ikony
			item->Icon = 0;

// zv��en� reference na novou ikonu
			RefIconInc(0);
		}

// zv��en� ukazatele polo�ek
		item++;
		off++;
	}
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� plochy ze souboru (TRUE=operace OK, p�i chyb� obsah nezm�n�n)

bool CMap::LoadFile(CText jmeno)
{
// otev�en� souboru mapovan�ho do pam�ti (uzav�en p�i destrukci!)
	CFileMap file;
	if (!file.Open(jmeno)) return false;

// kontrola velikosti souboru
	int size = file.Size();
	if (size < 100) return false;

// na�ten� z�kladn�ch parametr�
	MAPFILE* map = (MAPFILE*)file.Adr();
	int verze = map->Verze;			// verze souboru
	int jazyku = map->Jazyku;		// po�et jazyk�
	if (verze < 1) jazyku = 1;		// pro verzi 0 bude 1 jazyk
	int width = map->Width;			// ���ka plochy
	int height = map->Height;		// v��ka plochy
	int pals = map->Colors;			// po�et palet

// kontrola platnosti souboru
	if ((map->Ident[0] != 'P') ||
		(map->Ident[1] != 'M') ||
		(map->Ident[2] != 'A') ||
		(map->Ident[3] != 'P') ||
		(width < 1) ||
		(width > MAPMAXWIDTH) ||
		(height < 1) ||
		(height > MAPMAXHEIGHT) ||
		(pals < 2) ||
		(pals > 256) ||
		(size < width*height*8 + pals*4 + 30))
	{
		return false;
	}

// p��prava konverzn� tabulky palet
	BITMAPINFO* bmp = (BITMAPINFO*)MemGet(sizeof(BITMAPINFO) + pals*4 + 200);
	if (bmp == NULL) return false;
	bmp->bmiHeader.biClrUsed = pals;
	bmp->bmiHeader.biBitCount = 8;
	MemCopy(bmp->bmiColors, map->ColorTab, pals*4);

// barva 0 bude jako pozad�
	bmp->bmiColors[0] = StdBitmapInfo->bmiColors[0];

// barva 1 bude jako st�n
	bmp->bmiColors[1] = StdBitmapInfo->bmiColors[1];

	GenKonvPal(bmp);
	MemFree(bmp);

// p��prava po�tu referenc�
	BYTE* src = file.Adr() + SIZEOFMAPFILE + pals*4;
	int refs = *(int*)src;
	src += 4;
	size -= SIZEOFMAPFILE + pals*4 + 4;

// kontrola po�tu referenc�
	if ((refs < 1) || (refs > width*height)) return false;

// vytvo�en� bufferu referenc�
	CBufIndex inx;
	if (!inx.NumClear(refs)) return false;

// z�loha aktu�ln� plochy (zru�eno destruktorem p�i ukon�en� funkce)
	CMap backup(pData);

// vytvo�en� nov� plochy (vymaz�n� na ikony 0)
	if (!Clear(width, height)) return false;

// na�ten� referenc�
	CMultiText txt;					// text reference
	CText txt0;
	CIcon icon;						// ikona, rozm�ry ICONSIZE

// cyklus p�es v�echny ikony
	for (int i = 0; i < refs; i++)
	{
		txt.Empty();

// cyklus p�es v�echny jazyky
		for (int j = 0; j < jazyku; j++)
		{

// kontrola dat
			if (size < 3*4 + ICONSIZE)
			{
				*this = backup;
				return false;
			}

// na�ten� parametr� textu
			int id = JazykInfo[JazykDef].LangID & 0xff;
			int cp = 0;

			if (verze >= 1)
			{
				id = *(long*)src & 0xff;	// identifik�tor jazyku
				src += 4;
				size -= 4;
				if (id == 0) id = (JazykInfo[JazykDef].LangID & 0xff);

				cp = *(long*)src;			// k�dov� str�nka
				src += 4;
				size -= 4;
			}

// na�ten� d�lky textu
			int len = *(long*)src;			// d�lka textu ve znac�ch
			if (len < 0) len = 0;
			src += 4;						// posun zdrojov� adresy
			size -= 4;						// sn�en� zb�vaj�c�ch dat
			int len0 = len;					// d�lka textu v bajtech
			if (cp == 0) len0 = 2*len;		// d�lka textu v bajtech pro UNICODE

// kontrola d�lky textu a ikony
			if (((DWORD)len > 1000000) ||
				(len0 + ICONSIZE > size) ||
				(size <= 0))
			{
				*this = backup;
				return false;
			}

// na�ten� textu
			if (cp == 0)
			{
				if (!txt0.Set((WCHAR*)src, len))
				{
					*this = backup;
					return false;
				}
			}
			else
			{
				if (!txt0.Set((char*)src, len))
				{
					*this = backup;
					return false;
				}
			}
			txt0.KorigNul();
			src += len0;
			size -= len0;

// vyhled�n� ID jazyku
			for (int lan = 0; lan < JAZYKNUM; lan++)
			{
				if (lan != JAZYK000)
				{
					if ((JazykInfo[lan].LangID & 0xff) == id)
					{
						txt.MultiText(lan, txt0);
						break;
					}
				}
			}
		}

// na�ten� ikony
		if (!icon.CopyKonvData(src))
		{
			*this = backup;
			return false;
		}
		src += ICONSIZE;
		size -= ICONSIZE;

// nalezen� objektu v n�kter�m z buffer�
		int reffrst = -1;
		int refnext = -1;
		CBufProg* bf = &BufObj;				// bude se hledat v glob�ln�m bufferu
		int index = bf->FindObj(txt, -1, -1, -1); // nalezen� reference

		if (index < 0)					// nebyla polo�ka nalezena?
		{
			bf = &BufLoc;				// bude se hledat v lok�ln�m bufferu
			reffrst = bf->Disp();		// hledan� prvn� lok�ln� polo�ka
			if (bf->IsValid(reffrst))	// je lok�ln� polo�ka platn�?
			{
				refnext = bf->At(reffrst).Next;	// posledn� hledan� polo�ka
			}
			index = bf->FindObj(txt, -1, reffrst, refnext);
		}

// pokus o nalezen� objektu spr�vn�ho typu
		int nn = 1;
		while (bf->IsValid(index) && (bf->At(index).Func != IDF_ICON)
				&& (bf->At(index).Func != IDF_NONE))
		{
			nn++;
			txt.SetNumObj(nn);
			index = bf->FindObj(txt, -1, reffrst, refnext);
		}
		
// objekt nenalezen - bude se vytv��et (v glob�ln�m bufferu)
		if (bf->IsNotValid(index))
		{
			bf = &BufObj;

			int ic = Icon.Add(icon);
			if (ic < 0)
			{
				*this = backup;
				return false;
			}

			int tx = Text.Add(txt);
			if (tx < 0)
			{
				Icon.Del(ic);
				*this = backup;
				return false;
			}

			index = bf->Copy(-1, -2, BufClsID, CLASSICONINDEX);

			if (bf->IsValid(index))
			{

// nastaven� ikony
				ASSERT(bf->At(index).Func == IDF_ICON);
				if (bf->At(index).Icon >= 0)
				{
					Icon.Del(bf->At(index).Icon);
				}
				bf->At(index).Icon = ic;

				bf->At(index).DatIndex = bf->At(index).Icon;
				bf->At(index).DatBlok = BufIcoID;

// zaji�t�n� jedine�nosti jm�na nov� ikony
				reffrst = BufLoc.Disp();		// hledan� prvn� lok�ln� polo�ka
				refnext = -1;
				if (BufLoc.IsValid(reffrst))	// je lok�ln� polo�ka platn�?
				{
					refnext = BufLoc.At(reffrst).Next;	// posledn� hledan� polo�ka
				}

				nn = 0;
				while ((bf->FindObj(txt, index, -1, -1) >= 0) ||
					(BufLoc.FindObj(txt, -1, reffrst, refnext) >= 0))
				{
					nn++;
					txt.SetNumObj(nn);
				}

// nastaven� jm�na nov�ho objektu
				bf->At(index).Name = tx;

				if (bf->At(index).HTree)
				{
					TV_ITEM tvi;
					tvi.mask = TVIF_TEXT;
					tvi.hItem = bf->At(index).HTree;
					tvi.pszText = (LPTSTR)(LPCTSTR)txt.MultiText();
					::SendMessage(bf->Tree(), TVM_SETITEM, 0, (LPARAM)&tvi);
				}
			}
			else
			{
				Text.Del(tx);
				Icon.Del(ic);
				*this = backup;
				return false;
			}
		}

// �schova indexu ikony
		inx[i] = bf->GetIcon(index);
		if (inx[i] < 0) inx[i] = 0;
	}

// p�enesen� ikon
	for (i = 0; i < width*height; i++)
	{
		if (size < 8) break;

		int n = *(int*)src;
		if ((DWORD)n < (DWORD)refs)
		{
			pData->Data[i].Icon = inx[n];
		}
		else
		{
			pData->Data[i].Icon = 0;
		}
		pData->Data[i].Param = *(DWORD*)(src + 4);

		src += 8;
		size -= 8;
	}

// p��znak - na�teno OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// ulo�en� do souboru (FALSE=chyba)

bool CMap::SaveFile(CText jmeno) const
{
// vytvo�en� souboru
	CFile file;
	file.Name(jmeno);
	if (!file.Create()) return false;

// p��prava bufferu z�hlav�
	int bufsize = SIZEOFMAPFILE + StdColors*4 + 4 + 10000;
	BYTE* buf = (BYTE*)MemGet(bufsize);
	if (buf == NULL)
	{
		file.Close();
		file.Delete();
		return false;
	}
	
// vypln�n� z�hlav� souboru
	MAPFILE* map = (MAPFILE*)buf;
	map->Ident[0] = 'P';
	map->Ident[1] = 'M';
	map->Ident[2] = 'A';
	map->Ident[3] = 'P';
	map->Verze = 1;
	map->Jazyku = JAZYKNUM - 1;			// bez JAZYK000
	map->Param = 0;
	map->Width = pData->Width;
	map->Height = pData->Height;
	map->Colors = StdColors;

// p�enesen� palet
	MemCopy(map->ColorTab, StdBitmapInfo->bmiColors, StdColors*4);

// offset ��ta�e ikon
	int iconn = SIZEOFMAPFILE + StdColors*4;
	*(long*)(buf + iconn) = 0;

// p��prava k ulo�en� definic ikon
	int size = SIZEOFMAPFILE + StdColors*4 + 4;	// po�et bajt� v bufferu

	CBufIndex inx;					// buffer mapov�n� index� ikon
	if (!inx.NumClear(Icon.Max()))
	{
		MemFree(buf);
		file.Close();
		file.Delete();
		return false;
	}

	int icons = pData->Width * pData->Height;	// po�et ikon v plo�e celkem
	CMultiText txt;								// jm�no ikony
	CText txt0;

// cyklus p�es v�echny ikony plochy
	for (int i = 0; i < icons; i++)
	{

// ikona plochy
		int icon = pData->Data[i].Icon;
		ASSERT(Icon.IsValid(icon));
		if (Icon.IsNotValid(icon)) icon = 0;

// kontrola, zda je ikona ji� mapovan�
		if (inx[icon] < 0)
		{

// index mapovan� ikony
			inx[icon] = *(long*)(buf + iconn);
			(*(long*)(buf + iconn))++;

// nalezen� reference na ikonu
			CBufProg* bf = &BufObj;
			int ref = bf->SrcDat(BufIcoID, icon);

			if (ref < 0)
			{
				bf = &BufLoc;
				ref = bf->SrcDat(BufIcoID, icon);
			}
			ASSERT(ref >= 0);

// p��prava jm�na ikony (pro neplatnou ikonu vr�t� pr�zdn� �et�zec)
			txt = Text.Get(bf->GetText(ref));

// cyklus p�es v�echny jazyky
			for (int lan = 0; lan < JAZYKNUM; lan++)
			{
				if (lan != JAZYK000)
				{
					txt0 = txt.MultiText(lan);
					int len = txt0.Length();

// zv��en� velikosti bufferu
					if (bufsize < size + 3*4 + len + ICONSIZE)
					{
						bufsize = size + 3*4 + len + ICONSIZE + 10000;
						BYTE* buf2 = (BYTE*)MemSize(buf, bufsize);
						if (buf2 == NULL)
						{
							MemFree(buf);
							file.Close();
							file.Delete();
							return false;
						}
						buf = buf2;
					}

// ulo�en� parametr� a textu jm�na ikony
					*(long*)(buf + size) = JazykInfo[lan].LangID;
					size += 4;
					*(long*)(buf + size) = JazykInfo[lan].CodePage;
					size += 4;
					*(long*)(buf + size) = len;
					size += 4;
					txt0.Write((char*)(buf + size));
					size += len;
				}
			}

// ulo�en� ikony (pro neplatnou ikonu ulo�� pr�zdnou ikonu)
			Icon.Get(bf->GetIcon(ref)).DeComp();
			MemCopy(buf + size, Icon.Get(bf->GetIcon(ref)).DataData(), ICONSIZE);
			size += ICONSIZE;
		}
	}

// zv��en� velikosti bufferu pro data plochy
	if (bufsize < size + 8*icons)
	{
		bufsize = size + 8*icons;
		BYTE* buf2 = (BYTE*)MemSize(buf, bufsize);
		if (buf2 == NULL)
		{
			MemFree(buf);
			file.Close();
			file.Delete();
			return false;
		}
		buf = buf2;
	}

// ulo�en� plochy
	for (i = 0; i < icons; i++)
	{
		int icon = pData->Data[i].Icon;
		if (Icon.IsNotValid(icon)) icon = 0;
		*(long*)(buf + size) = inx[icon];
		size += 4;
		*(DWORD*)(buf + size) = pData->Data[i].Param;
		size += 4;
	}

// ulo�en� souboru
	BOOL result = file.Write(buf, size);

// uzav�en� souboru
	file.Close();

// zru�en� bufferu
	MemFree(buf);

// p�i chyb� z�pisu zru�en� souboru
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

const CMap& CMap::operator= (const CMap& src)
{
	detach(pData);				// zru�en� star�ch dat
	attach(src.pData);			// p�i�azen� nov�ch dat
	return *this;
}

const CMap& CMap::operator= (MAPDATA* src)
{
	detach(pData);				// zru�en� star�ch dat
	attach(src);				// p�i�azen� nov�ch dat
	return *this;
}


/***************************************************************************\
*																			*
*								Buffer ploch								*
*																			*
\***************************************************************************/

////////////////////////////////////////////////////////////////////
// p�id�n� z�znamu o vlo�en� polo�ky do UNDO bufferu

template <> bool CBuffer<CMap>::UndoAddIns(int index)
{
	return Undo.AddMapIns(index);
}

////////////////////////////////////////////////////////////////////
// vytvo�en� nov� plochy (vrac� index polo�ky, <0 = chyba)
// prov�d� z�znam do UNDO bufferu

int _fastcall CBufMap::New(int width, int height)
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
// p�id�n� polo�ky (vrac� index polo�ky, <0 = chyba)
// prov�d� z�znam do UNDO bufferu

int _fastcall CBufMap::Add(MAPDATA* data)
{
	CMap map(data);
	int result = Add(map);			// result mus� b�t odd�len, pro inline by se �patn� volal destruktor
	return result;
}

/////////////////////////////////////////////////////////////////////////////
// zru�en� pou�it� ikony ze v�ech ploch - nahrad� ikonou 0
// ignoruje p��padnou chybu pam�ti!!!

void _fastcall CBufMap::DelIcon(int icon)
{
	ASSERT(icon > 0);

	for (int i = m_Max - 1; i >= 0; i--)
	{
		if (m_Valid[i])
		{
			m_Data[i].DelIcon(icon, i, m_Undo);
		}
	}
}
