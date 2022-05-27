
#include "Main.h"

/***************************************************************************\
*																			*
*							Obsluha tažení ikon								*
*																			*
\***************************************************************************/

// parametry tažení
BOOL		Draging = FALSE;			// pøíznak probíhajícího tažení
int			DragIconWidth;				// šíøka tažené ikony
int			DragIconHeight;				// výška tažené ikony
int			DragHotSpotX;				// bod uchycení prvku X
int			DragHotSpotY;				// bod uchycení prvku Y
int			DragTestSpotX;				// testovací bod X (støed ikony)
int			DragTestSpotY;				// testovací bod Y (støed ikony)
HIMAGELIST	DragImageList;				// seznam obrázkù pro tažení
BOOL		DragRight;					// pøíznak pravého tlaèítka
BOOL		DragLeftInt;				// pøíznak levého tlaèítka + interní prvek
BOOL		DragCopy;					// pøíznak kopírování položky
BOOL		DragDelete;					// pøíznak rušení položky

// zdroj tažení
HWND		DragSrcTree;				// strom zdroje tažení
CBufProg*	DragSrcBuf;					// buffer zdroje tažení
int			DragSrcBufID;				// ID bufferu zdroje tažení
int			DragSrcParent;				// index rodièe zdroje tažení (-1=root)
HTREEITEM	DragSrcHItem;				// handle zdroje tažení
int			DragSrcIndex;				// index zdroje tažení
PROGITEM*	DragSrcItem;				// položka zdroje taŸení
int			DragSrcFunc;				// funkce zdroje tažení
int			DragSrcSrcMask;				// maska zdrojových vlastností zdroje tažení
int			DragSrcDstMask;				// maska cílových vlastností zdroje tažení
int			DragSrcParam;				// parametry položky zdroje tažení

// cíl tažení
HWND		DragDstTree;				// strom cíle tažení
CBufProg*	DragDstBuf;					// buffer cíle tažení
int			DragDstBufID;				// ID bufferu cíle tažení
int			DragDstParent;				// index rodièe cíle tažení (-1=root)
HTREEITEM	DragDstHParent;				// handle rodièe cíle tažení
int			DragDstPoz;					// index pozice cíle tažení (-1=zaèátek, -2=konec)
HTREEITEM	DragDstHPoz;				// handle pozice cíle tažení (NULL=zaèátek nebo konec)
int			DragDstFunc;				// funkce rodièe cíle tažení
int			DragDstSrcMask;				// maska zdrojových vlastností rodièe cíle tažení
int			DragDstDstMask;				// maska cílových vlastností rodièe cíle tažení
int			DragDstParam;				// parametry položky rodièe cíle tažení

/////////////////////////////////////////////////////////////////////////////
// zahájení taženi

void ProgOnBeginDrag(HWND hWnd, TV_ITEM* tvi, int x, int y, BOOL right)
{
// kontrola, zda neprobíhá tažení
	ASSERT(!Draging);
	if (Draging) return;

// kontrola, zda je režim editace
	ASSERT(ProgMode);
	if (!ProgMode) return;

// úschova pøíznaku pravého tlaèítka
	DragRight = right;

// pøíprava zdrojového okna a zdrojového bufferu
	DragSrcTree = hWnd;
	ASSERT(::IsWindow(hWnd));
	DragSrcBufID = BufProgIDFromHandle(DragSrcTree);
	ASSERT(DragSrcBufID >= 0);
	if (DragSrcBufID < 0) return;
	DragSrcBuf = &BufProg[DragSrcBufID];

// úschova rozmìrù ikon
	DragIconWidth = DragSrcBuf->IconWidth();
	DragIconHeight = DragSrcBuf->IconHeight();

// nalezení položky
	DragSrcHItem = tvi->hItem;
	DragSrcIndex = DragSrcBuf->Find(DragSrcHItem);
	ASSERT(DragSrcIndex >= 0);
	if (DragSrcIndex < 0) return;
	ASSERT(DragSrcBuf->IsValid(DragSrcIndex));
	DragSrcItem = &((*DragSrcBuf)[DragSrcIndex]);
	DragSrcParent = DragSrcItem->Parent;

// úschova vlastností zdrojové položky
	DragSrcFunc = DragSrcItem->Func;
	DragSrcSrcMask = DragSrcItem->SrcMask;
	DragSrcDstMask = DragSrcItem->DstMask;
	DragSrcParam = DragSrcItem->Param;

// parametry pro funkci
	if (DragSrcFunc == IDF_FNC)
	{
		DragSrcSrcMask = PR_COMMAND;

		int inx = DragSrcIndex;
		if (DragSrcBufID == BufEdiID) inx = DragSrcItem->RefIndex;

		if ((((DragSrcBufID == BufEdiID) && (DragSrcItem->RefBlok == BufObjID)) || (DragSrcBufID == BufObjID)) &&
		    BufObj.IsValid(inx) && (BufObj[inx].DatBlok == BufLocID))
		{
			inx = BufObj[inx].DatIndex;
			if (BufLoc.IsValid(inx) && (BufLoc[inx].DatBlok == BufEdiID))
			{
				inx = BufLoc[inx].Child;
				while (BufLoc.IsValid(inx))
				{
					if (BufLoc[inx].Func == IDF_OUT)
					{
						inx = BufLoc[inx].Child;
						if (BufLoc.IsValid(inx) && (BufLoc[inx].DatBlok >= 0))
						{
							DragSrcSrcMask = BufLoc[inx].SrcMask | PR_COMMAND;
						}
						break;
					}
					inx = BufLoc[inx].Next;
				}
			}
		}
	}

// kontrola, jestli to není zakázaný prvek
	if (DragSrcParam & PR_NOMOVE) return;

// naètení souøadnic textu prvku
	RECT	rc;							// souøadnice taŸeného prvku
	*(HTREEITEM*)(&rc) = DragSrcHItem;
	BOOL result = ::SendMessage(DragSrcTree, TVM_GETITEMRECT, TRUE, (LPARAM)&rc);
	ASSERT(result);
	if (!result) return;

// výpoèet bodu uchycení taženého prvku
	DragHotSpotX = x - (rc.left - 3 - DragIconWidth);
	DragHotSpotY = y - rc.top;

// výpoèet testovacího bodu
	DragTestSpotX = DragIconWidth/4*3;
	DragTestSpotY = - DragIconHeight/4;

// vygenerování tažené ikony
//	DragImageList = (HIMAGELIST)::SendMessage(DragSrcTree, TVM_CREATEDRAGIMAGE, 0, (LPARAM)DragSrcHItem);
//	if (DragImageList == NULL) return;

	CPicture dragicon;
	if (!dragicon.New(DragIconWidth, DragIconHeight)) return;
	int iconinx = DragSrcBuf->GetIcon(DragSrcIndex);
	ASSERT(iconinx >= 0);
	CIcon icon = Icon.Get(iconinx);
	if (!icon.DeComp()) return;

	if (icon.Size() == (DragIconWidth*DragIconHeight))
	{
		if (!dragicon.CopyData(icon.DataData())) return;
	}
	else
	{
		if ((icon.Size()*9/16) == (DragIconWidth*DragIconHeight))
		{
			CBufIcon iconlist;
			iconlist.IconSize(ICONWIDTH, ICONHEIGHT);
			if (iconlist.Add(icon) != 0) return;
			BYTE* bf = iconlist.GenerBitmap(SI_MEDIUM);
			if (bf == NULL) return;
			if (!dragicon.CopyData(bf)) return;
			MemFree(bf);
		}
		else
		{
			if ((icon.Size()/4) != (DragIconWidth*DragIconHeight)) return;

			CBufIcon iconlist;
			iconlist.IconSize(ICONWIDTH, ICONHEIGHT);
			if (iconlist.Add(icon) != 0) return;
			BYTE* bf = iconlist.GenerBitmap(SI_SMALL);
			if (bf == NULL) return;
			if (!dragicon.CopyData(bf)) return;
			MemFree(bf);
		}
	}

	int textinx = DragSrcBuf->GetText(DragSrcIndex);
	ASSERT(textinx >= 0);
	DragImageList = dragicon.GenerDrag(Text.GetTxt(textinx));
	ASSERT(DragImageList != NULL);
	if (DragImageList == NULL) return;

// nastavení pøíznaku zahájení tažení (odteï se již nesmí pøerušit)
	Draging = TRUE;

// pøednastavení neplatného cíle
	DragDstTree = NULL;					// neplatný cílový strom	
	DragDstParent = -1;					// neplatná cílová položka

// výbìr cílového prvku
	VERIFY(::SendMessage(DragSrcTree, TVM_SELECTITEM, TVGN_DROPHILITE, (LPARAM)DragSrcHItem));

// zapnutí zobrazení obrázku tažení (bez uzamykání okna)
	::ImageList_DragShowNolock(TRUE);

// pøi tažení skupiny z okna tøíd se musí nejdøíve zjistit, zda obsahuje potomky
	if ((DragSrcBufID == BufClsID) &&
		(DragSrcParent >= 0) && 
		(DragSrcFunc == IDF_GROUP) &&
		(DragSrcItem->Child < 0))
	{
		ProgLib::Load(DragSrcIndex);
		DragSrcItem = &((*DragSrcBuf)[DragSrcIndex]);
	}

// korekce jen kopírovaných prvkù
	DragLeftInt = FALSE;			// není levé tlaèítko + interní prvek
	if (DragSrcParam & (PR_INTERN | PR_LOCK | PR_LOCK_DEP))
	{
		if (!DragRight)				// bylo levé tlaèítko?
		{
			DragLeftInt = TRUE;		// je levé tlaèítko + interní prvek
		}
		DragRight = TRUE;			// bude kopírování jako pravé tlaèítko
	}

// pøíprava pøíznaku kopírování
	DragCopy = TRUE;				// bude kopírování
	DragDelete = !DragRight;		// pro levé tlaèítko zrušení staré položky

// zapnutí kurzoru pro tažení
	if (DragRight)
	{
		CurAkt = CurCopy;
	}
	else
	{
		CurAkt = CurMove;
	}
	::SetCursor(CurAkt);

// vytvoøení nového obrázku tažení
	::ImageList_SetDragCursorImage(DragImageList, 0, 0, 0);

// zahájení operace tažení
	::ImageList_BeginDrag(DragImageList, 0, 0, 0);

// zrušení výbìru cíle
	::SendMessage(DragSrcTree, TVM_SELECTITEM, TVGN_DROPHILITE, (LPARAM)NULL);

// zahájení operace tažení, zákaz aktualizace okna
	::ImageList_DragEnter(NULL, MouseScreen.x - DragHotSpotX, MouseScreen.y - DragHotSpotY);

// pøetažení obrázku na aktuální pozici myši
	::ImageList_DragMove(MouseScreen.x - DragHotSpotX, MouseScreen.y - DragHotSpotY);

// zahájení zachytávání myši
	::SetCapture(MainFrame);
};


/////////////////////////////////////////////////////////////////////////////
// nalezení cílového okna, nad kterým je taženo (vrací FALSE=nenalezeno)

BOOL SrcDropWin(int x, int y)
{
// pøednastavení neplatného cíle
	DragDstTree = NULL;					// neplatný cílový strom	
	DragDstParent = -1;					// neplatná cílová položka
	
// nalezení okna, nad kterým je taženo
	POINT pt;
	pt.x = x;
	pt.y = y;
	HWND wnd = ::ChildWindowFromPointEx(MainFrame, pt, CWP_SKIPINVISIBLE | CWP_SKIPDISABLED);
	if (wnd == NULL) return FALSE;

// pøi tažení interního prvku levým tlaèítkem zákaz stejného okna
	if (DragLeftInt)
	{
		if (wnd == DragSrcTree)
		{
			return FALSE;
		}
	}

// okno editace
	if (wnd == TreeEdi)
	{
		DragDstTree = TreeEdi;
		DragDstBuf = &BufEdi;
		DragDstBufID = BufEdiID;
		return TRUE;
	}

// okno globálních objektù
	if (wnd == TreeObj)
	{
		DragDstTree = TreeObj;
		DragDstBuf = &BufObj;
		DragDstBufID = BufObjID;
		return TRUE;
	}

// okno lokálních objektù
	if (wnd == TreeLoc)
	{
		DragDstTree = TreeLoc;
		DragDstBuf = &BufLoc;
		DragDstBufID = BufLocID;
		return TRUE;
	}

// okno struktur
	if (wnd == TreeStr)
	{
		DragDstTree = TreeStr;
		DragDstBuf = &BufStr;
		DragDstBufID = BufStrID;
		return TRUE;
	}

// okno tøíd
	if (wnd == TreeCls)
	{
		DragDstTree = TreeCls;
		DragDstBuf = &BufCls;
		DragDstBufID = BufClsID;
		return TRUE;
	}

// okno editoru plochy nebo sprajtu
	if ((wnd == MainFrame) &&
		((DragSrcBufID == BufObjID) || (DragSrcBufID == BufLocID)) &&
		(x >= EditX) &&
		(x < EditX + EditWidth) &&
		(y >= EditY) &&
		(y < EditY + EditHeight))
	{
		switch (EditMode)
		{
		case BufSprID:
			if ((DragSrcSrcMask & PR_PIC) != 0)
			{
				DragDstBufID = EditMode;
				return TRUE;
			}
			break;

		case BufMapID:
			if ((DragSrcSrcMask & PR_ICON) != 0)
			{
				DragDstBufID = EditMode;
				return TRUE;
			}
			break;
		}
	}

// okno nenalezeno
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// nalezení položky pod kurzorem ve stromu (NULL=konec ROOT, -1=zaèátek ROOT)

HTREEITEM ItemTreePos(int x, int y, int height)
{
// lokální promìnné
	HTREEITEM item;						// nalezená položka
	TV_HITTESTINFO hti;					// struktura pro nalezení položky

// nalezení položky na dané souøadnici
	hti.pt.x = x;
	hti.pt.y = y;
	item = (HTREEITEM) ::SendMessage(DragDstTree, TVM_HITTEST, 0, (LPARAM)&hti);
	if (item != NULL) return item;

// nový pokus, je-li kurzor pøíliš vlevo èi pøíliš vpravo
	if (hti.flags & (TVHT_TOLEFT | TVHT_TORIGHT))
	{
		x = 20;
		hti.pt.x = 20;					// asi tak nìjak na zaèátek øádku
		hti.pt.y = y;
		item = (HTREEITEM) ::SendMessage(DragDstTree, TVM_HITTEST, 0, (LPARAM)&hti);
		if (item != NULL) return item;
	}

// je-li kurzor nad horním okrajem, musíme najít pøedcházející skrytou položku
	if (hti.flags & TVHT_ABOVE)
	{
		item = (HTREEITEM) ::SendMessage(DragDstTree, TVM_GETNEXTITEM, TVGN_FIRSTVISIBLE, 0);
		if (item == NULL) return (HTREEITEM)-1;
		item = (HTREEITEM) ::SendMessage(DragDstTree, TVM_GETNEXTITEM, TVGN_PREVIOUSVISIBLE, (LPARAM)item);
		if (item == NULL) return (HTREEITEM)-1;
		return item;
	}

// je-li kurzor pøíliš dole, zkusíme ještì jeden pokus o trochu výš
	if (item == NULL)
	{
		y -= height/4*3;
		hti.pt.x = x;
		hti.pt.y = y;
		item = (HTREEITEM) ::SendMessage(DragDstTree, TVM_HITTEST, 0, (LPARAM)&hti);
	}

// návrat výsledku (až už je jakýkoliv)
	return item;
}


/////////////////////////////////////////////////////////////////////////////
// test povolení cíle tažení - operace pøidání k rodièi (FALSE=nepovoleno)

BOOL TestDropItem()
{
// do zamknuté položky nelze táhnout
	if (DragDstParam & (PR_LOCK | PR_LOCK_DEP)) return FALSE;

// rozlišení podle cílového bufferu
	switch (DragDstBufID)
	{

//-------------- globální a lokální objekty
	case BufObjID:
	case BufLocID:

// z editoru nelze nikdy táhnout
		if (DragSrcBufID == BufEdiID) return FALSE;

// z druhého okna objektù nelze táhnout
		if (DragSrcBufID == (BufLocID ^ BufObjID ^ DragDstBufID)) return FALSE;

// do velikosti seznamu lze pøetáhnout èíslice
		if ((DragDstFunc == IDF_LIST_SIZE) &&
			(DragSrcFunc >= IDF_0) &&
			(DragSrcFunc <= IDF_9)) return TRUE;

// komentáø lze položit kamkoliv (kromì 1 parametru), do komentáøe nelze táhnout nic (kromì další komentáø)
		if (DragSrcFunc == IDF_COMMENT)
		{
			return (((DragDstParam & PR_ONE) == 0) && 
					(DragDstFunc != IDF_PAR) &&
					(DragDstFunc != IDF_OUT) &&
					(DragDstFunc != IDF_FNC) &&
					(DragDstFunc != IDF_COMMAND_LINE) &&
					(DragDstFunc != IDF_RETURN_CODE));
		}
		if (DragDstFunc == IDF_COMMENT) return FALSE;

// z okna struktur lze pøetáhnout jen skupinu a seznam (ne do seznamu ani parametrù)
		if (DragSrcBufID == BufStrID)
		{
			return (((DragSrcFunc == IDF_GROUP) ||
					(DragSrcFunc == IDF_LIST)) && 
					(DragDstFunc != IDF_PAR) &&
					(DragDstFunc != IDF_OUT) &&
					(DragDstFunc != IDF_FNC) &&
					(DragDstFunc != IDF_LIST) &&
					(DragDstFunc != IDF_COMMAND_LINE) &&
					(DragDstFunc != IDF_RETURN_CODE) &&
					((DragDstParam & PR_ONE) == 0));
		}

// do seznamu lze pøetáhnout datové promìnné
		if (DragDstFunc == IDF_LIST)
		{
			switch (DragSrcFunc)
			{
			case IDF_REAL:
			case IDF_BOOL:
			case IDF_TEXT:
			case IDF_ICON:
			case IDF_NONE:
			case IDF_MAP:
			case IDF_PIC:
			case IDF_SND:
			case IDF_MUS:
			case IDF_SPRITE:
				return TRUE;
			}
			return FALSE;
		}

// skupinu, seznam ani funkci nelze umístit do vstupních/výstupních promìnných
		if (((DragSrcFunc == IDF_GROUP) || (DragSrcFunc == IDF_LIST) || (DragSrcFunc == IDF_FNC)) &&
			((DragDstFunc == IDF_PAR) || (DragDstFunc == IDF_OUT))) return FALSE;

// test, zda je povolen pouze jeden parametr (výstupní promìnná)
		if ((DragDstParam & PR_ONE) && (DragDstParent >= 0) &&
			((*DragDstBuf)[DragDstParent].Child >= 0)) return FALSE;

// do skupiny, ROOT (pøíp. parametrù) lze pøetáhnout promìnné, skupinu a seznam
		if ((DragDstFunc == IDF_GROUP) || (DragDstParent == -1) || 
				(DragDstFunc == IDF_PAR) || (DragDstFunc == IDF_OUT))
		{
			switch (DragSrcFunc)
			{
			case IDF_GROUP:
			case IDF_LIST:
			case IDF_FNC:
			case IDF_REAL:
			case IDF_BOOL:
			case IDF_TEXT:
			case IDF_ICON:
			case IDF_NONE:
			case IDF_MAP:
			case IDF_PIC:
			case IDF_SND:
			case IDF_MUS:
			case IDF_SPRITE:
				return TRUE;
			}
		}

		return FALSE;

//-------------- editor

	case BufEdiID:

// ze tøíd nelze nikdy táhnout
		if (DragSrcBufID == BufClsID) return FALSE;
		
// komentáø lze položit témìø kamkoliv
		if (DragSrcFunc == IDF_COMMENT)
		{
			return (((DragDstParam & PR_ONE) == 0) && 
					(DragDstFunc != IDF_FNC));
		}

// do komentáøe lze táhnout cokoliv
		if ((DragDstFunc == IDF_COMMENT)) return TRUE;

// z objektù nelze táhnout skupinu objektù a èíslice z velikosti seznamu
		if (((DragSrcBufID == BufObjID) || (DragSrcBufID == BufLocID)) &&
			((DragSrcFunc == IDF_GROUP) || (DragSrcSrcMask & PR_DIGIT))) return FALSE;

// seznam objektù není povolen nikdy
		if (DragSrcFunc == IDF_LIST) return FALSE;

// test, zda má cíl povolen pouze jeden parametr a zda už je obsazen
		if ((DragDstParam & PR_ONE) && (DragDstParent >= 0) &&
			(DragDstBuf->At(DragDstParent).Child >= 0)) return FALSE;

// pøetažení vìtve CASE
		if (DragSrcFunc == IDF_CASE_ON) return (DragDstFunc == IDF_CASE);
		if (DragSrcFunc == IDF_CASE_EQU) return (DragDstFunc == IDF_CASE_ON);

// porovnání masky zdroje a cíle tažení
		if ((DragSrcSrcMask & DragDstDstMask) == 0) return FALSE;

// zákaz pøetažení položky s parametrem do parametru
		if ((DragSrcBufID == BufEdiID) &&						// problém jen s pøetahováním uvnitø editoru
			(DragSrcFunc != IDF_FNC) &&							// funkce je povolena, mùže mít vstupní parametry
			((DragDstDstMask & PR_COMMAND) == 0) &&				// cílem nejsou pøíkazy
			((DragSrcSrcMask & PR_COMMAND) != 0) &&				// zdroj mùže být jako pøíkaz
			((DragSrcParam & PR_PARPAR) == 0) &&				// kromì pøípadu, kdy má povoleny parametry vždy
			(DragSrcItem->Child >= 0)) return FALSE;			// položka má nìjakého potomka

// zákaz pøetažení do položky, která není pøíkazem
		if ((DragDstParent >= 0) &&								// je nìjaký cíl
			((DragDstSrcMask & PR_COMMAND) != 0) &&				// cíl mùže být jako pøíkaz
			((DragDstParam & PR_PARPAR) == 0) &&				// kromì pøípadu, kdy má povoleny parametry vždy
			(DragDstBuf->At(DragDstParent).Parent >= 0) &&		// cíl je nìèím potomkem
			((DragDstBuf->At(DragDstBuf->At(DragDstParent).Parent).DstMask & PR_ALLDATA) != 0) &&  // rodiè cíle pøijímá data
			((DragDstBuf->At(DragDstBuf->At(DragDstParent).Parent).DstMask & PR_COMMAND) == 0))		// pøitom rodiè cíle nepøijímá pøíkazy (=není jako pøíkaz)
			return FALSE;

		return TRUE;

//---------- okno tøíd
	
	case BufClsID:

// ze struktur lze táhnout jen skupinu
		if (DragSrcBufID == BufStrID)
		{
			if (DragSrcFunc != IDF_GROUP) return FALSE;
			return ((DragDstFunc == IDF_GROUP) || ((DragDstParent >= 0) &&
						(DragDstBuf->At(DragDstParent).Parent < 0)));
		}

// z objektù lze táhnout objekt s daty (bez potomkù)
		if ((DragSrcBufID == BufObjID) || (DragSrcBufID == BufLocID))
		{
			if ((DragSrcItem->DatBlok >= 0) &&
				(DragSrcItem->DatIndex >= 0) &&
				((DragSrcItem->Child < 0) || (DragSrcItem->Func == IDF_SPRITE)) &&
				(DragDstParent >= 0))
			{
// pøíprava cílové funkce vìtve
				PROGITEM* paritem = &DragDstBuf->At(DragDstParent);
				while (paritem->Parent >= 0)
				{
					paritem = &DragDstBuf->At(paritem->Parent);
				}
				int dstfunc = paritem->Func;

				int srcfunc = DragSrcFunc;
				if (srcfunc == IDF_NONE)
				{
					srcfunc = IDF_ICON;
				}
				if (srcfunc == IDF_FNC) return false;

				paritem = &DragDstBuf->At(DragDstParent);
				if (paritem->Parent < 0)
				{
					return (dstfunc == srcfunc);
				}

				return ((DragDstFunc == IDF_GROUP) && (dstfunc == srcfunc));
			}
			return FALSE;
		}

// pøetahování uvnitø okna tøíd - jen potomci
		if ((DragSrcBufID == BufClsID) && 
			(DragSrcItem->Parent >= 0) &&
			(DragDstParent >= 0))
		{

// pøíprava cílové funkce vìtve
			PROGITEM* paritem = &DragDstBuf->At(DragDstParent);
			while (paritem->Parent >= 0)
			{
				paritem = &DragDstBuf->At(paritem->Parent);
			}
			int dstfunc = paritem->Func;

// pøíprava zdrojové funkce
			paritem = DragSrcItem;
			while (paritem->Parent >= 0)
			{
				paritem = &DragSrcBuf->At(paritem->Parent);
			}
			int srcfunc = paritem->Func;

// zákaz tažení do jiné vìtve
			if (srcfunc != dstfunc) return FALSE;

// pøi pøesunu zákaz ponechání ve stejné vìtvi
//			if (!DragRight && (DragDstParent == DragSrcParent)) return FALSE;

// prvek nelze položit na prvek (kromì do základní položky)
			if (DragDstBuf->At(DragDstParent).Parent < 0) return TRUE;
			if (DragDstBuf->At(DragDstParent).Func != IDF_GROUP) return FALSE;
			return TRUE;
		}
		return FALSE;
	}

	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// test cyklické operace (TRUE=je OK)

BOOL TestCykl()
{
	if (DragSrcBufID == DragDstBufID)
	{
		int item = DragDstParent;

		while (item >= 0)
		{
			if (item == DragSrcIndex) return FALSE;
			item = DragSrcBuf->At(item).Parent;
		}
	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// nalezení platné cílové položky v cílovém bufferu (z klientských souøadnic v oknì stromu, TRUE=nalezeno)

BOOL FindDragDst(int x, int y)
{
// nalezení položky stromu na dané pozici
	DragDstHPoz = ItemTreePos(x, y, DragDstBuf->IconHeight());

// položka je na zaèátku ROOT
	if (DragDstHPoz == (HTREEITEM)-1)			// zaèátek ROOT
	{
		DragDstHPoz = NULL;
		DragDstPoz = -1;							// bude položení na zaèátek ROOT
	}

// nalezení položky v bufferu
	else
	{
		DragDstPoz = DragDstBuf->Find(DragDstHPoz);

// položka je na konci ROOT
		if (DragDstPoz < 0)							// konec ROOT
		{
			DragDstPoz = -2;						// bude položení za konec položek
			DragDstHPoz = NULL;
		}
	}

// implicitní rodiè, není-li cíl nalezen
	DragDstParent = DragDstBuf->Disp();			// implicitní rodiè pro položení do ROOT

// pøíprava odsazování prvkù v cílovém stromu (jen je-li cílem platná položka)
	if (DragDstPoz >= 0)
	{
		int indent = ::SendMessage(DragDstTree, TVM_GETINDENT, 0, 0);
		if (indent < 4) indent = 4;
		if (indent > 128) indent = 128;

// zjištìní souøadnice položky pod kurzorem (ze souøadnice textu)
		RECT rc;
		*(HTREEITEM*)(&rc) = DragDstHPoz;
		if (!::SendMessage(DragDstTree, TVM_GETITEMRECT, TRUE, (LPARAM)&rc))
		{
			rc.left = DragDstBuf->IconWidth() + 3;
		}
		int pozx = rc.left - DragDstBuf->IconWidth() - 3;
		if (pozx < 0) pozx = 0;

// pokud je kurzor za položkou, bude položka rodièem, uložení bude na zaèátek
		if (x > (pozx + DragDstBuf->IconWidth()))
		{
			DragDstParent = DragDstPoz;
			DragDstPoz = -1;
			DragDstHPoz = NULL;
		}
		else

// jinak nalezení položky na stejné pozici X
		{
			while ((x < pozx) && 
					(DragDstBuf->At(DragDstPoz).Parent >= 0) &&
					(DragDstBuf->At(DragDstPoz).Parent != DragDstBuf->Disp()))
			{
				DragDstPoz = DragDstBuf->At(DragDstPoz).Parent;
				pozx -= indent;
			}

			DragDstParent = DragDstBuf->At(DragDstPoz).Parent;
		}
	}

// handle rodièe
	DragDstHParent = NULL;
	if (DragDstParent >= 0)
	{
		DragDstHParent = DragDstBuf->At(DragDstParent).HTree;
	}

// pøíprava parametrù cíle tažení, je-li platný rodiè
	if ((DragDstParent >= 0) && (DragDstParent != DragDstBuf->Disp()))
	{
		DragDstFunc = DragDstBuf->At(DragDstParent).Func;
		DragDstSrcMask = DragDstBuf->At(DragDstParent).SrcMask;
		DragDstDstMask = DragDstBuf->At(DragDstParent).DstMask;
		DragDstParam = DragDstBuf->At(DragDstParent).Param;
	}
	else

// pøíprava parametrù cíle, je-li rodièem ROOT
	{
		DragDstSrcMask = 0;
		DragDstDstMask = 0;
		DragDstParam = 0;
		DragDstFunc = IDF_GROUP;

		switch(DragDstBufID)
		{
		case BufObjID:
		case BufLocID:
			DragDstDstMask = PR_ALLDATA;
			break;

		case BufEdiID:
			DragDstDstMask = PR_COMMAND;
			break;
		}
	}

// test povolení operace
	return (TestDropItem() && TestCykl());
}


/////////////////////////////////////////////////////////////////////////////
// posun myši pøi tažení

void ProgOnMoveDrag(UINT flags, int x, int y)
{
// lokální promìnné
	HWND	oldtree;					// pùvodní cílový strom
	int		oldparent;					// pùvodní cílový index
	POINT	ptScreen;					// kurzor myši v souøadnicích displeje
	POINT	ptTree;						// kurzor myši v souøadnicích okna stromu
	HCURSOR	curnew = CurNoDrag;			// nový kurzor myši

// kontrola, zda probíhá obsluha tažení
	if (!Draging) return;

// úschova pùvodního cíle tažení
	oldtree = DragDstTree;				// pùvodní cílový strom
	oldparent = DragDstParent;			// pùvodní cílový rodiè

// pøednastavení neplatného cíle operace
	DragDstTree = NULL;					// není cílový strom
	DragDstBuf = NULL;					// není cílový buffer
	DragDstParent = -1;					// není cílový rodiè
	DragDstHParent = NULL;				// není cílový rodiè

// pøíprava nové souøadnice obrázku
	ptScreen.x = x - DragHotSpotX;
	ptScreen.y = y - DragHotSpotY;
	ptTree.x = ptScreen.x + DragTestSpotX;
	ptTree.y = ptScreen.y + DragTestSpotY;
	::ClientToScreen(MainFrame, &ptScreen);

// pøetažení obrázku na novou pozici
	::ImageList_DragMove(ptScreen.x, ptScreen.y);

	DragCopy = FALSE;
	DragDelete = FALSE;

// nalezení okna, nad kterým je taženo
	if ((SrcDropWin(ptTree.x, ptTree.y - DragTestSpotY + DragIconHeight/2)) || 
		(SrcDropWin(ptTree.x, ptTree.y - DragTestSpotY + DragIconHeight)) || 
		(SrcDropWin(ptTree.x, ptTree.y - DragTestSpotY)))
	{

// taženi do okna editoru plochy
		if (DragDstBufID == BufMapID)
		{
			EditMap::OnMouseMove(flags, x, y);	
			if ((MouseX != MOUSEINV) && (MouseY != MOUSEINV))
			{
				DragCopy = TRUE;
				curnew = CurCopy;
			}
		}
		else

// taženi do okna editoru sprajtu
		{
			if (DragDstBufID == BufSprID)
			{
				if (DragSrcBuf->At(DragSrcIndex).DatBlok == BufPicID)
				{
					EditSprite::OnMouseMove(flags, x, y);	
					if ((MouseX != MOUSEINV) && (MouseY != MOUSEINV))
					{
						DragCopy = TRUE;
						curnew = CurCopy;
					}
				}
			}
			else

// pøi tažení do okna struktur bude odhození
			{
				if ((DragDstBufID == BufStrID) &&
					(DragSrcBufID != BufStrID) &&
					!DragRight &&
					((DragSrcBufID != BufClsID) || ((DragSrcParent >= 0) && (DragSrcItem->Child < 0))))
				{
					DragCopy = FALSE;
					DragDelete = TRUE;
					curnew = CurDelete;
				}
				else
				{
					if (DragDstBufID >= PROGBUFNUM)
					{
						DragCopy = FALSE;
						DragDelete = FALSE;
					}
					else
					{

// nastavení pøíznakù módu kopie nebo pøesunu
						DragCopy = TRUE;
						DragDelete = (!DragRight && (DragSrcBufID == DragDstBufID)); 

// pøíprava kurzoru pro kopírování nebo pøesun
						if (DragDelete)
						{
							curnew = CurMove;						// bude pøesun
						}
						else
						{
							curnew = CurCopy;						// bude kopie
						}

// nalezení položky pod kurzorem (=pozice cíle tažení)
						::ClientToScreen(MainFrame, &ptTree);
						::ScreenToClient(DragDstTree, &ptTree);
						int dx = DragDstBuf->IconWidth()/2;
						int dy = DragDstBuf->IconHeight()/2;

						if (!FindDragDst(ptTree.x     , ptTree.y     ) &&
							!FindDragDst(ptTree.x - dx, ptTree.y - dy) &&
							!FindDragDst(ptTree.x - dx, ptTree.y     ) &&
							!FindDragDst(ptTree.x - dx, ptTree.y + dy) &&
							!FindDragDst(ptTree.x     , ptTree.y - dy) &&
							!FindDragDst(ptTree.x     , ptTree.y + dy) &&
							!FindDragDst(ptTree.x + dx, ptTree.y - dy) &&
							!FindDragDst(ptTree.x + dx, ptTree.y     ) &&
							!FindDragDst(ptTree.x + dx, ptTree.y + dy) &&
							!FindDragDst(ptTree.x     , ptTree.y - 2*dy) &&
							!FindDragDst(ptTree.x - 2*dx, ptTree.y - 2*dy) &&
							!FindDragDst(ptTree.x - 2*dx, ptTree.y   ) &&
							!FindDragDst(ptTree.x + 2*dx, ptTree.y - 2*dy) &&
							!FindDragDst(ptTree.x + 2*dx, ptTree.y   ))
						{
							DragDstParent = -1;
							DragDstHParent = NULL;
							DragDstPoz = -1;
							DragDstHPoz = NULL;
							DragCopy = FALSE;
							DragDelete = FALSE;
							curnew = CurNoDrag;
						}
					}
				}
			}
		}
	}

// nastavení nového kurzoru myši
	if (curnew != CurAkt)
	{
		CurAkt = curnew;
		::SetCursor(CurAkt);
	}

// zrušení výbìru staré cílové položky pøi zmìnì okna stromu
	if ((oldtree != NULL) && (oldtree != DragDstTree))
	{
		::ImageList_DragLeave(NULL);
		::SendMessage(oldtree, TVM_SELECTITEM, TVGN_DROPHILITE, (LPARAM)NULL);
		::ImageList_DragEnter(NULL, ptScreen.x, ptScreen.y);
	}

// výbìr nové cílové položky (zmìnil se strom nebo položka)
	if (((DragDstParent != oldparent) || (oldtree != DragDstTree))
		&& (DragDstTree != NULL))
	{
		::ImageList_DragLeave(NULL);
		::SendMessage(DragDstTree, TVM_SELECTITEM, TVGN_DROPHILITE, (LPARAM)DragDstHParent);
		::ImageList_DragEnter(NULL, ptScreen.x, ptScreen.y);
	}
}


/////////////////////////////////////////////////////////////////////////////
// ukonèení tažení

void ProgOnEndDrag()
{
// kontrola, zda probíhá obsluha tažení
	if (!Draging) return;

// pøerušení tažení
	BreakDrag();

// test zrušení položky s referencemi
	if (DragDelete && !DragCopy &&
		!DragSrcBuf->TestRefer(DragSrcIndex)) return;

// tažení do editoru plochy nebo sprajtu
	if (DragCopy)
	{
		if (DragDstBufID == BufMapID)
		{
			EditMap::DragDrop(DragSrcBuf->GetIcon(DragSrcIndex));
			return;
		}

		if (DragDstBufID == BufSprID)
		{
			EditSprite::DragDrop(DragSrcBuf->At(DragSrcIndex).DatIndex);
			return;
		}
	}
	
// vypnutí pøekreslování cílového okna pøed operací
	if (DragDstTree != NULL) DragDstBuf->RedrawOff();

// provedení operace
	if (DragCopy)
	{
		int dst = -1;

		if (DragDelete)
		{
			ASSERT(DragDstBufID == DragSrcBufID);

			if (DragDstBufID == DragSrcBufID)
			{
				if (DragDstBufID == BufClsID)
				{
					if (DragDstParent != DragSrcParent)
					{
						dst = ProgLib::Move(DragDstParent, DragDstPoz, DragSrcIndex);
					}
				}
				else
				{
					dst = DragDstBuf->Move(DragDstParent, DragDstPoz, DragSrcIndex);
				}
			}
		}
		else
		{
			if (DragDstBufID == BufClsID)
			{
				dst = ProgLib::Copy(DragDstParent, DragDstPoz, DragSrcBufID, DragSrcIndex);
			}
			else
			{
				dst = DragDstBuf->Copy(DragDstParent, DragDstPoz, DragSrcBufID, DragSrcIndex);
				SetModi();
			}
		}

// aktualizace LOCK a OFF prvkù v buferech
		UpdateLock();

// zajištìní viditelnosti pøenesené položky
		if (dst >= 0)
		{
			::SendMessage(DragDstTree, TVM_ENSUREVISIBLE, 0, (LPARAM)DragDstBuf->At(dst).HTree);
		}
	}
	else
	{
		if (DragDelete)
		{

// rušení položky v bufferu tøíd
			if (DragSrcBufID == BufClsID)
			{
				ProgLib::Delete(DragSrcIndex);
			}
			else
			{
				DragSrcBuf->Del(DragSrcIndex);
				SetModi();
				if (EditMode == BufMapID) ProgOnPaint();
			}
		}
	}

// zapnutí pøekreslování okna po operaci
	if (DragDstTree != NULL) DragDstBuf->RedrawOn();

// aktualizace informací o aktuální položce
	ProgAktItem();

// ukonèení záznamu jedné UNDO operace
	Undo.Stop();
}

/////////////////////////////////////////////////////////////////////////////
// pøerušení tažení

void BreakDrag()
{

// kontrola, zda probíhá obsluha tažení
	if (Draging)
	{

// vypnutí pøíznaku tažení
		Draging = FALSE;

// zastavení tažení
		::ImageList_DragLeave(NULL);

// ukonèení tažení
		::ImageList_EndDrag();

// zrušení seznamu obrázkù tažení
		::ImageList_Destroy(DragImageList);

// zrušení výbìru cíle operace u cílového okna
		if (DragDstTree)
		{
			::SendMessage(DragDstTree, TVM_SELECTITEM, TVGN_DROPHILITE, (LPARAM)NULL);
		}

// uvolnìni zachytávání myši
		::ReleaseCapture();

// nastavení bìžného kurzoru myši
		::SetCursor(CurArrow);
	}
}
