
#include "Main.h"

/***************************************************************************\
*																			*
*							Obsluha ta�en� ikon								*
*																			*
\***************************************************************************/

// parametry ta�en�
BOOL		Draging = FALSE;			// p��znak prob�haj�c�ho ta�en�
int			DragIconWidth;				// ���ka ta�en� ikony
int			DragIconHeight;				// v��ka ta�en� ikony
int			DragHotSpotX;				// bod uchycen� prvku X
int			DragHotSpotY;				// bod uchycen� prvku Y
int			DragTestSpotX;				// testovac� bod X (st�ed ikony)
int			DragTestSpotY;				// testovac� bod Y (st�ed ikony)
HIMAGELIST	DragImageList;				// seznam obr�zk� pro ta�en�
BOOL		DragRight;					// p��znak prav�ho tla��tka
BOOL		DragLeftInt;				// p��znak lev�ho tla��tka + intern� prvek
BOOL		DragCopy;					// p��znak kop�rov�n� polo�ky
BOOL		DragDelete;					// p��znak ru�en� polo�ky

// zdroj ta�en�
HWND		DragSrcTree;				// strom zdroje ta�en�
CBufProg*	DragSrcBuf;					// buffer zdroje ta�en�
int			DragSrcBufID;				// ID bufferu zdroje ta�en�
int			DragSrcParent;				// index rodi�e zdroje ta�en� (-1=root)
HTREEITEM	DragSrcHItem;				// handle zdroje ta�en�
int			DragSrcIndex;				// index zdroje ta�en�
PROGITEM*	DragSrcItem;				// polo�ka zdroje ta�en�
int			DragSrcFunc;				// funkce zdroje ta�en�
int			DragSrcSrcMask;				// maska zdrojov�ch vlastnost� zdroje ta�en�
int			DragSrcDstMask;				// maska c�lov�ch vlastnost� zdroje ta�en�
int			DragSrcParam;				// parametry polo�ky zdroje ta�en�

// c�l ta�en�
HWND		DragDstTree;				// strom c�le ta�en�
CBufProg*	DragDstBuf;					// buffer c�le ta�en�
int			DragDstBufID;				// ID bufferu c�le ta�en�
int			DragDstParent;				// index rodi�e c�le ta�en� (-1=root)
HTREEITEM	DragDstHParent;				// handle rodi�e c�le ta�en�
int			DragDstPoz;					// index pozice c�le ta�en� (-1=za��tek, -2=konec)
HTREEITEM	DragDstHPoz;				// handle pozice c�le ta�en� (NULL=za��tek nebo konec)
int			DragDstFunc;				// funkce rodi�e c�le ta�en�
int			DragDstSrcMask;				// maska zdrojov�ch vlastnost� rodi�e c�le ta�en�
int			DragDstDstMask;				// maska c�lov�ch vlastnost� rodi�e c�le ta�en�
int			DragDstParam;				// parametry polo�ky rodi�e c�le ta�en�

/////////////////////////////////////////////////////////////////////////////
// zah�jen� ta�eni

void ProgOnBeginDrag(HWND hWnd, TV_ITEM* tvi, int x, int y, BOOL right)
{
// kontrola, zda neprob�h� ta�en�
	ASSERT(!Draging);
	if (Draging) return;

// kontrola, zda je re�im editace
	ASSERT(ProgMode);
	if (!ProgMode) return;

// �schova p��znaku prav�ho tla��tka
	DragRight = right;

// p��prava zdrojov�ho okna a zdrojov�ho bufferu
	DragSrcTree = hWnd;
	ASSERT(::IsWindow(hWnd));
	DragSrcBufID = BufProgIDFromHandle(DragSrcTree);
	ASSERT(DragSrcBufID >= 0);
	if (DragSrcBufID < 0) return;
	DragSrcBuf = &BufProg[DragSrcBufID];

// �schova rozm�r� ikon
	DragIconWidth = DragSrcBuf->IconWidth();
	DragIconHeight = DragSrcBuf->IconHeight();

// nalezen� polo�ky
	DragSrcHItem = tvi->hItem;
	DragSrcIndex = DragSrcBuf->Find(DragSrcHItem);
	ASSERT(DragSrcIndex >= 0);
	if (DragSrcIndex < 0) return;
	ASSERT(DragSrcBuf->IsValid(DragSrcIndex));
	DragSrcItem = &((*DragSrcBuf)[DragSrcIndex]);
	DragSrcParent = DragSrcItem->Parent;

// �schova vlastnost� zdrojov� polo�ky
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

// kontrola, jestli to nen� zak�zan� prvek
	if (DragSrcParam & PR_NOMOVE) return;

// na�ten� sou�adnic textu prvku
	RECT	rc;							// sou�adnice ta�en�ho prvku
	*(HTREEITEM*)(&rc) = DragSrcHItem;
	BOOL result = ::SendMessage(DragSrcTree, TVM_GETITEMRECT, TRUE, (LPARAM)&rc);
	ASSERT(result);
	if (!result) return;

// v�po�et bodu uchycen� ta�en�ho prvku
	DragHotSpotX = x - (rc.left - 3 - DragIconWidth);
	DragHotSpotY = y - rc.top;

// v�po�et testovac�ho bodu
	DragTestSpotX = DragIconWidth/4*3;
	DragTestSpotY = - DragIconHeight/4;

// vygenerov�n� ta�en� ikony
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

// nastaven� p��znaku zah�jen� ta�en� (odte� se ji� nesm� p�eru�it)
	Draging = TRUE;

// p�ednastaven� neplatn�ho c�le
	DragDstTree = NULL;					// neplatn� c�lov� strom	
	DragDstParent = -1;					// neplatn� c�lov� polo�ka

// v�b�r c�lov�ho prvku
	VERIFY(::SendMessage(DragSrcTree, TVM_SELECTITEM, TVGN_DROPHILITE, (LPARAM)DragSrcHItem));

// zapnut� zobrazen� obr�zku ta�en� (bez uzamyk�n� okna)
	::ImageList_DragShowNolock(TRUE);

// p�i ta�en� skupiny z okna t��d se mus� nejd��ve zjistit, zda obsahuje potomky
	if ((DragSrcBufID == BufClsID) &&
		(DragSrcParent >= 0) && 
		(DragSrcFunc == IDF_GROUP) &&
		(DragSrcItem->Child < 0))
	{
		ProgLib::Load(DragSrcIndex);
		DragSrcItem = &((*DragSrcBuf)[DragSrcIndex]);
	}

// korekce jen kop�rovan�ch prvk�
	DragLeftInt = FALSE;			// nen� lev� tla��tko + intern� prvek
	if (DragSrcParam & (PR_INTERN | PR_LOCK | PR_LOCK_DEP))
	{
		if (!DragRight)				// bylo lev� tla��tko?
		{
			DragLeftInt = TRUE;		// je lev� tla��tko + intern� prvek
		}
		DragRight = TRUE;			// bude kop�rov�n� jako prav� tla��tko
	}

// p��prava p��znaku kop�rov�n�
	DragCopy = TRUE;				// bude kop�rov�n�
	DragDelete = !DragRight;		// pro lev� tla��tko zru�en� star� polo�ky

// zapnut� kurzoru pro ta�en�
	if (DragRight)
	{
		CurAkt = CurCopy;
	}
	else
	{
		CurAkt = CurMove;
	}
	::SetCursor(CurAkt);

// vytvo�en� nov�ho obr�zku ta�en�
	::ImageList_SetDragCursorImage(DragImageList, 0, 0, 0);

// zah�jen� operace ta�en�
	::ImageList_BeginDrag(DragImageList, 0, 0, 0);

// zru�en� v�b�ru c�le
	::SendMessage(DragSrcTree, TVM_SELECTITEM, TVGN_DROPHILITE, (LPARAM)NULL);

// zah�jen� operace ta�en�, z�kaz aktualizace okna
	::ImageList_DragEnter(NULL, MouseScreen.x - DragHotSpotX, MouseScreen.y - DragHotSpotY);

// p�eta�en� obr�zku na aktu�ln� pozici my�i
	::ImageList_DragMove(MouseScreen.x - DragHotSpotX, MouseScreen.y - DragHotSpotY);

// zah�jen� zachyt�v�n� my�i
	::SetCapture(MainFrame);
};


/////////////////////////////////////////////////////////////////////////////
// nalezen� c�lov�ho okna, nad kter�m je ta�eno (vrac� FALSE=nenalezeno)

BOOL SrcDropWin(int x, int y)
{
// p�ednastaven� neplatn�ho c�le
	DragDstTree = NULL;					// neplatn� c�lov� strom	
	DragDstParent = -1;					// neplatn� c�lov� polo�ka
	
// nalezen� okna, nad kter�m je ta�eno
	POINT pt;
	pt.x = x;
	pt.y = y;
	HWND wnd = ::ChildWindowFromPointEx(MainFrame, pt, CWP_SKIPINVISIBLE | CWP_SKIPDISABLED);
	if (wnd == NULL) return FALSE;

// p�i ta�en� intern�ho prvku lev�m tla��tkem z�kaz stejn�ho okna
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

// okno glob�ln�ch objekt�
	if (wnd == TreeObj)
	{
		DragDstTree = TreeObj;
		DragDstBuf = &BufObj;
		DragDstBufID = BufObjID;
		return TRUE;
	}

// okno lok�ln�ch objekt�
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

// okno t��d
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
// nalezen� polo�ky pod kurzorem ve stromu (NULL=konec ROOT, -1=za��tek ROOT)

HTREEITEM ItemTreePos(int x, int y, int height)
{
// lok�ln� prom�nn�
	HTREEITEM item;						// nalezen� polo�ka
	TV_HITTESTINFO hti;					// struktura pro nalezen� polo�ky

// nalezen� polo�ky na dan� sou�adnici
	hti.pt.x = x;
	hti.pt.y = y;
	item = (HTREEITEM) ::SendMessage(DragDstTree, TVM_HITTEST, 0, (LPARAM)&hti);
	if (item != NULL) return item;

// nov� pokus, je-li kurzor p��li� vlevo �i p��li� vpravo
	if (hti.flags & (TVHT_TOLEFT | TVHT_TORIGHT))
	{
		x = 20;
		hti.pt.x = 20;					// asi tak n�jak na za��tek ��dku
		hti.pt.y = y;
		item = (HTREEITEM) ::SendMessage(DragDstTree, TVM_HITTEST, 0, (LPARAM)&hti);
		if (item != NULL) return item;
	}

// je-li kurzor nad horn�m okrajem, mus�me naj�t p�edch�zej�c� skrytou polo�ku
	if (hti.flags & TVHT_ABOVE)
	{
		item = (HTREEITEM) ::SendMessage(DragDstTree, TVM_GETNEXTITEM, TVGN_FIRSTVISIBLE, 0);
		if (item == NULL) return (HTREEITEM)-1;
		item = (HTREEITEM) ::SendMessage(DragDstTree, TVM_GETNEXTITEM, TVGN_PREVIOUSVISIBLE, (LPARAM)item);
		if (item == NULL) return (HTREEITEM)-1;
		return item;
	}

// je-li kurzor p��li� dole, zkus�me je�t� jeden pokus o trochu v��
	if (item == NULL)
	{
		y -= height/4*3;
		hti.pt.x = x;
		hti.pt.y = y;
		item = (HTREEITEM) ::SendMessage(DragDstTree, TVM_HITTEST, 0, (LPARAM)&hti);
	}

// n�vrat v�sledku (a� u� je jak�koliv)
	return item;
}


/////////////////////////////////////////////////////////////////////////////
// test povolen� c�le ta�en� - operace p�id�n� k rodi�i (FALSE=nepovoleno)

BOOL TestDropItem()
{
// do zamknut� polo�ky nelze t�hnout
	if (DragDstParam & (PR_LOCK | PR_LOCK_DEP)) return FALSE;

// rozli�en� podle c�lov�ho bufferu
	switch (DragDstBufID)
	{

//-------------- glob�ln� a lok�ln� objekty
	case BufObjID:
	case BufLocID:

// z editoru nelze nikdy t�hnout
		if (DragSrcBufID == BufEdiID) return FALSE;

// z druh�ho okna objekt� nelze t�hnout
		if (DragSrcBufID == (BufLocID ^ BufObjID ^ DragDstBufID)) return FALSE;

// do velikosti seznamu lze p�et�hnout ��slice
		if ((DragDstFunc == IDF_LIST_SIZE) &&
			(DragSrcFunc >= IDF_0) &&
			(DragSrcFunc <= IDF_9)) return TRUE;

// koment�� lze polo�it kamkoliv (krom� 1 parametru), do koment��e nelze t�hnout nic (krom� dal�� koment��)
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

// z okna struktur lze p�et�hnout jen skupinu a seznam (ne do seznamu ani parametr�)
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

// do seznamu lze p�et�hnout datov� prom�nn�
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

// skupinu, seznam ani funkci nelze um�stit do vstupn�ch/v�stupn�ch prom�nn�ch
		if (((DragSrcFunc == IDF_GROUP) || (DragSrcFunc == IDF_LIST) || (DragSrcFunc == IDF_FNC)) &&
			((DragDstFunc == IDF_PAR) || (DragDstFunc == IDF_OUT))) return FALSE;

// test, zda je povolen pouze jeden parametr (v�stupn� prom�nn�)
		if ((DragDstParam & PR_ONE) && (DragDstParent >= 0) &&
			((*DragDstBuf)[DragDstParent].Child >= 0)) return FALSE;

// do skupiny, ROOT (p��p. parametr�) lze p�et�hnout prom�nn�, skupinu a seznam
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

// ze t��d nelze nikdy t�hnout
		if (DragSrcBufID == BufClsID) return FALSE;
		
// koment�� lze polo�it t�m�� kamkoliv
		if (DragSrcFunc == IDF_COMMENT)
		{
			return (((DragDstParam & PR_ONE) == 0) && 
					(DragDstFunc != IDF_FNC));
		}

// do koment��e lze t�hnout cokoliv
		if ((DragDstFunc == IDF_COMMENT)) return TRUE;

// z objekt� nelze t�hnout skupinu objekt� a ��slice z velikosti seznamu
		if (((DragSrcBufID == BufObjID) || (DragSrcBufID == BufLocID)) &&
			((DragSrcFunc == IDF_GROUP) || (DragSrcSrcMask & PR_DIGIT))) return FALSE;

// seznam objekt� nen� povolen nikdy
		if (DragSrcFunc == IDF_LIST) return FALSE;

// test, zda m� c�l povolen pouze jeden parametr a zda u� je obsazen
		if ((DragDstParam & PR_ONE) && (DragDstParent >= 0) &&
			(DragDstBuf->At(DragDstParent).Child >= 0)) return FALSE;

// p�eta�en� v�tve CASE
		if (DragSrcFunc == IDF_CASE_ON) return (DragDstFunc == IDF_CASE);
		if (DragSrcFunc == IDF_CASE_EQU) return (DragDstFunc == IDF_CASE_ON);

// porovn�n� masky zdroje a c�le ta�en�
		if ((DragSrcSrcMask & DragDstDstMask) == 0) return FALSE;

// z�kaz p�eta�en� polo�ky s parametrem do parametru
		if ((DragSrcBufID == BufEdiID) &&						// probl�m jen s p�etahov�n�m uvnit� editoru
			(DragSrcFunc != IDF_FNC) &&							// funkce je povolena, m��e m�t vstupn� parametry
			((DragDstDstMask & PR_COMMAND) == 0) &&				// c�lem nejsou p��kazy
			((DragSrcSrcMask & PR_COMMAND) != 0) &&				// zdroj m��e b�t jako p��kaz
			((DragSrcParam & PR_PARPAR) == 0) &&				// krom� p��padu, kdy m� povoleny parametry v�dy
			(DragSrcItem->Child >= 0)) return FALSE;			// polo�ka m� n�jak�ho potomka

// z�kaz p�eta�en� do polo�ky, kter� nen� p��kazem
		if ((DragDstParent >= 0) &&								// je n�jak� c�l
			((DragDstSrcMask & PR_COMMAND) != 0) &&				// c�l m��e b�t jako p��kaz
			((DragDstParam & PR_PARPAR) == 0) &&				// krom� p��padu, kdy m� povoleny parametry v�dy
			(DragDstBuf->At(DragDstParent).Parent >= 0) &&		// c�l je n���m potomkem
			((DragDstBuf->At(DragDstBuf->At(DragDstParent).Parent).DstMask & PR_ALLDATA) != 0) &&  // rodi� c�le p�ij�m� data
			((DragDstBuf->At(DragDstBuf->At(DragDstParent).Parent).DstMask & PR_COMMAND) == 0))		// p�itom rodi� c�le nep�ij�m� p��kazy (=nen� jako p��kaz)
			return FALSE;

		return TRUE;

//---------- okno t��d
	
	case BufClsID:

// ze struktur lze t�hnout jen skupinu
		if (DragSrcBufID == BufStrID)
		{
			if (DragSrcFunc != IDF_GROUP) return FALSE;
			return ((DragDstFunc == IDF_GROUP) || ((DragDstParent >= 0) &&
						(DragDstBuf->At(DragDstParent).Parent < 0)));
		}

// z objekt� lze t�hnout objekt s daty (bez potomk�)
		if ((DragSrcBufID == BufObjID) || (DragSrcBufID == BufLocID))
		{
			if ((DragSrcItem->DatBlok >= 0) &&
				(DragSrcItem->DatIndex >= 0) &&
				((DragSrcItem->Child < 0) || (DragSrcItem->Func == IDF_SPRITE)) &&
				(DragDstParent >= 0))
			{
// p��prava c�lov� funkce v�tve
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

// p�etahov�n� uvnit� okna t��d - jen potomci
		if ((DragSrcBufID == BufClsID) && 
			(DragSrcItem->Parent >= 0) &&
			(DragDstParent >= 0))
		{

// p��prava c�lov� funkce v�tve
			PROGITEM* paritem = &DragDstBuf->At(DragDstParent);
			while (paritem->Parent >= 0)
			{
				paritem = &DragDstBuf->At(paritem->Parent);
			}
			int dstfunc = paritem->Func;

// p��prava zdrojov� funkce
			paritem = DragSrcItem;
			while (paritem->Parent >= 0)
			{
				paritem = &DragSrcBuf->At(paritem->Parent);
			}
			int srcfunc = paritem->Func;

// z�kaz ta�en� do jin� v�tve
			if (srcfunc != dstfunc) return FALSE;

// p�i p�esunu z�kaz ponech�n� ve stejn� v�tvi
//			if (!DragRight && (DragDstParent == DragSrcParent)) return FALSE;

// prvek nelze polo�it na prvek (krom� do z�kladn� polo�ky)
			if (DragDstBuf->At(DragDstParent).Parent < 0) return TRUE;
			if (DragDstBuf->At(DragDstParent).Func != IDF_GROUP) return FALSE;
			return TRUE;
		}
		return FALSE;
	}

	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// test cyklick� operace (TRUE=je OK)

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
// nalezen� platn� c�lov� polo�ky v c�lov�m bufferu (z klientsk�ch sou�adnic v okn� stromu, TRUE=nalezeno)

BOOL FindDragDst(int x, int y)
{
// nalezen� polo�ky stromu na dan� pozici
	DragDstHPoz = ItemTreePos(x, y, DragDstBuf->IconHeight());

// polo�ka je na za��tku ROOT
	if (DragDstHPoz == (HTREEITEM)-1)			// za��tek ROOT
	{
		DragDstHPoz = NULL;
		DragDstPoz = -1;							// bude polo�en� na za��tek ROOT
	}

// nalezen� polo�ky v bufferu
	else
	{
		DragDstPoz = DragDstBuf->Find(DragDstHPoz);

// polo�ka je na konci ROOT
		if (DragDstPoz < 0)							// konec ROOT
		{
			DragDstPoz = -2;						// bude polo�en� za konec polo�ek
			DragDstHPoz = NULL;
		}
	}

// implicitn� rodi�, nen�-li c�l nalezen
	DragDstParent = DragDstBuf->Disp();			// implicitn� rodi� pro polo�en� do ROOT

// p��prava odsazov�n� prvk� v c�lov�m stromu (jen je-li c�lem platn� polo�ka)
	if (DragDstPoz >= 0)
	{
		int indent = ::SendMessage(DragDstTree, TVM_GETINDENT, 0, 0);
		if (indent < 4) indent = 4;
		if (indent > 128) indent = 128;

// zji�t�n� sou�adnice polo�ky pod kurzorem (ze sou�adnice textu)
		RECT rc;
		*(HTREEITEM*)(&rc) = DragDstHPoz;
		if (!::SendMessage(DragDstTree, TVM_GETITEMRECT, TRUE, (LPARAM)&rc))
		{
			rc.left = DragDstBuf->IconWidth() + 3;
		}
		int pozx = rc.left - DragDstBuf->IconWidth() - 3;
		if (pozx < 0) pozx = 0;

// pokud je kurzor za polo�kou, bude polo�ka rodi�em, ulo�en� bude na za��tek
		if (x > (pozx + DragDstBuf->IconWidth()))
		{
			DragDstParent = DragDstPoz;
			DragDstPoz = -1;
			DragDstHPoz = NULL;
		}
		else

// jinak nalezen� polo�ky na stejn� pozici X
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

// handle rodi�e
	DragDstHParent = NULL;
	if (DragDstParent >= 0)
	{
		DragDstHParent = DragDstBuf->At(DragDstParent).HTree;
	}

// p��prava parametr� c�le ta�en�, je-li platn� rodi�
	if ((DragDstParent >= 0) && (DragDstParent != DragDstBuf->Disp()))
	{
		DragDstFunc = DragDstBuf->At(DragDstParent).Func;
		DragDstSrcMask = DragDstBuf->At(DragDstParent).SrcMask;
		DragDstDstMask = DragDstBuf->At(DragDstParent).DstMask;
		DragDstParam = DragDstBuf->At(DragDstParent).Param;
	}
	else

// p��prava parametr� c�le, je-li rodi�em ROOT
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

// test povolen� operace
	return (TestDropItem() && TestCykl());
}


/////////////////////////////////////////////////////////////////////////////
// posun my�i p�i ta�en�

void ProgOnMoveDrag(UINT flags, int x, int y)
{
// lok�ln� prom�nn�
	HWND	oldtree;					// p�vodn� c�lov� strom
	int		oldparent;					// p�vodn� c�lov� index
	POINT	ptScreen;					// kurzor my�i v sou�adnic�ch displeje
	POINT	ptTree;						// kurzor my�i v sou�adnic�ch okna stromu
	HCURSOR	curnew = CurNoDrag;			// nov� kurzor my�i

// kontrola, zda prob�h� obsluha ta�en�
	if (!Draging) return;

// �schova p�vodn�ho c�le ta�en�
	oldtree = DragDstTree;				// p�vodn� c�lov� strom
	oldparent = DragDstParent;			// p�vodn� c�lov� rodi�

// p�ednastaven� neplatn�ho c�le operace
	DragDstTree = NULL;					// nen� c�lov� strom
	DragDstBuf = NULL;					// nen� c�lov� buffer
	DragDstParent = -1;					// nen� c�lov� rodi�
	DragDstHParent = NULL;				// nen� c�lov� rodi�

// p��prava nov� sou�adnice obr�zku
	ptScreen.x = x - DragHotSpotX;
	ptScreen.y = y - DragHotSpotY;
	ptTree.x = ptScreen.x + DragTestSpotX;
	ptTree.y = ptScreen.y + DragTestSpotY;
	::ClientToScreen(MainFrame, &ptScreen);

// p�eta�en� obr�zku na novou pozici
	::ImageList_DragMove(ptScreen.x, ptScreen.y);

	DragCopy = FALSE;
	DragDelete = FALSE;

// nalezen� okna, nad kter�m je ta�eno
	if ((SrcDropWin(ptTree.x, ptTree.y - DragTestSpotY + DragIconHeight/2)) || 
		(SrcDropWin(ptTree.x, ptTree.y - DragTestSpotY + DragIconHeight)) || 
		(SrcDropWin(ptTree.x, ptTree.y - DragTestSpotY)))
	{

// ta�eni do okna editoru plochy
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

// ta�eni do okna editoru sprajtu
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

// p�i ta�en� do okna struktur bude odhozen�
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

// nastaven� p��znak� m�du kopie nebo p�esunu
						DragCopy = TRUE;
						DragDelete = (!DragRight && (DragSrcBufID == DragDstBufID)); 

// p��prava kurzoru pro kop�rov�n� nebo p�esun
						if (DragDelete)
						{
							curnew = CurMove;						// bude p�esun
						}
						else
						{
							curnew = CurCopy;						// bude kopie
						}

// nalezen� polo�ky pod kurzorem (=pozice c�le ta�en�)
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

// nastaven� nov�ho kurzoru my�i
	if (curnew != CurAkt)
	{
		CurAkt = curnew;
		::SetCursor(CurAkt);
	}

// zru�en� v�b�ru star� c�lov� polo�ky p�i zm�n� okna stromu
	if ((oldtree != NULL) && (oldtree != DragDstTree))
	{
		::ImageList_DragLeave(NULL);
		::SendMessage(oldtree, TVM_SELECTITEM, TVGN_DROPHILITE, (LPARAM)NULL);
		::ImageList_DragEnter(NULL, ptScreen.x, ptScreen.y);
	}

// v�b�r nov� c�lov� polo�ky (zm�nil se strom nebo polo�ka)
	if (((DragDstParent != oldparent) || (oldtree != DragDstTree))
		&& (DragDstTree != NULL))
	{
		::ImageList_DragLeave(NULL);
		::SendMessage(DragDstTree, TVM_SELECTITEM, TVGN_DROPHILITE, (LPARAM)DragDstHParent);
		::ImageList_DragEnter(NULL, ptScreen.x, ptScreen.y);
	}
}


/////////////////////////////////////////////////////////////////////////////
// ukon�en� ta�en�

void ProgOnEndDrag()
{
// kontrola, zda prob�h� obsluha ta�en�
	if (!Draging) return;

// p�eru�en� ta�en�
	BreakDrag();

// test zru�en� polo�ky s referencemi
	if (DragDelete && !DragCopy &&
		!DragSrcBuf->TestRefer(DragSrcIndex)) return;

// ta�en� do editoru plochy nebo sprajtu
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
	
// vypnut� p�ekreslov�n� c�lov�ho okna p�ed operac�
	if (DragDstTree != NULL) DragDstBuf->RedrawOff();

// proveden� operace
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

// aktualizace LOCK a OFF prvk� v buferech
		UpdateLock();

// zaji�t�n� viditelnosti p�enesen� polo�ky
		if (dst >= 0)
		{
			::SendMessage(DragDstTree, TVM_ENSUREVISIBLE, 0, (LPARAM)DragDstBuf->At(dst).HTree);
		}
	}
	else
	{
		if (DragDelete)
		{

// ru�en� polo�ky v bufferu t��d
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

// zapnut� p�ekreslov�n� okna po operaci
	if (DragDstTree != NULL) DragDstBuf->RedrawOn();

// aktualizace informac� o aktu�ln� polo�ce
	ProgAktItem();

// ukon�en� z�znamu jedn� UNDO operace
	Undo.Stop();
}

/////////////////////////////////////////////////////////////////////////////
// p�eru�en� ta�en�

void BreakDrag()
{

// kontrola, zda prob�h� obsluha ta�en�
	if (Draging)
	{

// vypnut� p��znaku ta�en�
		Draging = FALSE;

// zastaven� ta�en�
		::ImageList_DragLeave(NULL);

// ukon�en� ta�en�
		::ImageList_EndDrag();

// zru�en� seznamu obr�zk� ta�en�
		::ImageList_Destroy(DragImageList);

// zru�en� v�b�ru c�le operace u c�lov�ho okna
		if (DragDstTree)
		{
			::SendMessage(DragDstTree, TVM_SELECTITEM, TVGN_DROPHILITE, (LPARAM)NULL);
		}

// uvoln�ni zachyt�v�n� my�i
		::ReleaseCapture();

// nastaven� b�n�ho kurzoru my�i
		::SetCursor(CurArrow);
	}
}
