
#include "Main.h"

/***************************************************************************\
*																			*
*							Buffer programu									*
*																			*
\***************************************************************************/

////////////////////////////////////////////////////////////////////
// Buffer p�istupuje ke glob�ln�m datov�m seznam�m (Text, Icon, ...) !
////////////////////////////////////////////////////////////////////

// inicializa�n� pr�zdn� polo�ka (pro vytvo�en� nov� polo�ky)
PROGITEM	InitProgItem = {
	IDF_COMMENT,				// funkce

	0,							// zdrojov� vlastnost�
	0,							// c�lov� vlastnost�
	0,							// parametry

	-1,							// referen�n� blok
	-1,							// referen�n� index
	-1,							// datov� blok
	-1,							// datov� index

	-1,							// rodi�
	-1,							// n�sleduj�c� polo�ka
	-1,							// p�edch�zej�c� polo�ka
	-1,							// potomek

	-1,							// ikona
	-1,							// jm�no

	NULL,						// polo�ka stromu

	0							// ��ta� referenc�
};


////////////////////////////////////////////////////////////////////
// vytvo�en� nov�ch dat (vrac� TRUE=operace OK) (odd�leno kv�li lep�� optimalizaci)

#define NEWDATANUM 256				// po�et nov� vytvo�en�ch polo�ek (16 KB)

bool CBufProg::NewData()
{
// nov� po�et polo�ek
	int next = m_Max;				// p��t� polo�ka - 1
	int max = next + NEWDATANUM;	// nov� po�et polo�ek

// zv�t�en� bufferu dat
	PROGITEM* newdata = (PROGITEM*)MemSize(m_Data, max*sizeof(PROGITEM));
	if (newdata == NULL) return false;
	m_Data = newdata;

// zv�t�en� bufferu platnosti
	bool* newvalid = (bool*)MemSize(m_Valid, max*sizeof(bool));
	if (newvalid == NULL) return false;
	m_Valid = newvalid;

// nov� maxim�ln� po�et polo�ek v bufferu
	m_Max = max;

// vymaz�n� p��znak� platnosti polo�ek (nastaven� na p��znak neplatnosti)
	int i;
	newvalid += next;
	for (i = NEWDATANUM; i > 0; i--) { *newvalid = false; newvalid++; }

// za�len�n� do �et�zce voln�ch polo�ek
	newdata += next - 1;			// ukazatel dat - 1
	for (i = NEWDATANUM; i > 0; i--)
	{
		newdata++;					// zv��en� ukazatele polo�ek
		next++;						// zv��en� indexu p��t� polo�ky
		*(int*)newdata = next;		// odkaz na p��t� polo�ku
	}
	*(int*)newdata = m_Next;		// nav�z�n� na dal�� polo�ku
	m_Next = m_Max-NEWDATANUM;		// odkaz na prvn� novou polo�ku

// p��znak operace OK
	return true;
};


////////////////////////////////////////////////////////////////////
// vytvo�en� nov� polo�ky (vrac� index polo�ky, p�i chyb� vrac� -1)

int CBufProg::NewItem()
{
// vytvo�en� nov�ch dat, nen�-li voln� dal�� polo�ka
	if (m_Next < 0)				// nen� dal�� polo�ka?
	{
		if (!NewData()) return -1;	// vytvo�en� nov�ch dat
	}

// vyjmut� polo�ky z �et�zce voln�ch polo�ek
	int i = m_Next;				// p��t� voln� polo�ka
	m_Next = *(int*)(m_Data + i); // dal�� polo�ka
	m_Valid[i] = true;			// nastaven� p��znaku platnosti polo�ky
	m_Num++;					// zv��en� ��ta�e platn�ch polo�ek

// nov� vytvo�en� polo�ka
	return i;
};


////////////////////////////////////////////////////////////////////
// zru�en� polo�ky - za�azen� do voln�ch polo�ek (nekontroluje index a neru�� objekt)

void _fastcall CBufProg::DelItem(const int index)
{
	*(int*)(m_Data + index) = m_Next;	// p��t� voln� polo�ka
	m_Valid[index] = false;				// zru�en� p��znaku platnosti
	m_Num--;							// sn�en� ��ta�e platn�ch polo�ek
	m_Next = index;						// odkaz na tuto polo�ku
}

////////////////////////////////////////////////////////////////////
// aktualizace fontu v okn�

void CBufProg::AktFont()
{
	if (m_Tree != NULL)
	{
		if ((m_Font == NULL) ||
			!(TreeFont == m_FontSet) ||
			(CharSet != m_CharSet))
		{
			HFONT oldfont = m_Font;
			m_Font = GetFont(&TreeFont);
			m_FontSet = TreeFont;
			m_CharSet = CharSet;
			::SendMessage(m_Tree, WM_SETFONT, (WPARAM)m_Font, TRUE);
			DelFont(oldfont);
		}
	}
	else
	{
		DelFont(m_Font);
		m_Font = NULL;
		m_FontSet.Height = -1;
	}
}

////////////////////////////////////////////////////////////////////
// p��prava indexu stavov� ikony (upravit s INDEXTOSTATEIMAGEMASK() !)

int CBufProg::GetStateImage(int index)
{
// implicitn� stav - nic
	int state = STATE_NONE;

// je polo�ka platn�?
	if (IsValid(index))
	{

// adresa polo�ky
		PROGITEM* item = m_Data + index;

// obsluha okna editoru (po�adavek nastaven� prom�nn�) ...
		if ((m_BufID == BufEdiID) &&

// ... indikuje se v p��pad�, �e nen� ��dn� parametr ,,,
			(item->Child < 0) &&

// ... polo�ka vy�aduje n�jak� data ...
			(item->DstMask & PR_ALLDATA) && 

// ... ale nesm� vy�adovat i p��kazy ...				
			((item->DstMask & PR_COMMAND) == 0) &&

// ... polo�ka mus� m�t platn�ho rodi�e ...
			(item->Parent >= 0)
			)
		{

// adresa rodi�e
			PROGITEM* parent = m_Data + item->Parent;

// polo�ka mus� b�t pou�ita bu� jako p��kaz ...
			if ((parent->DstMask & PR_COMMAND) || 

// ... nebo jako parametr funkce ...
				(parent->Func == IDF_FNC) ||

// ... nebo jsou parametry vy�adov�ny v�dy
				(item->Param & PR_PARPAR) ||

// ... nebo nelze pou��t jako p��kaz - je to tedy parametr n��eho
				((item->SrcMask & PR_COMMAND) == 0))
			{
				if (item->Param & PR_SETPAR)
				{
					state = STATE_SET;
				}
				else
				{
					state = STATE_SET_OPT;
				}
			}
		}

// p��prava stavu pro okna objekt� (definovan� polo�ka)
		if (index == m_Def)
		{
			state = STATE_DEF;
		}
	}

// index stavov� ikony
	return state;
}


////////////////////////////////////////////////////////////////////
// nov� zobrazen� polo�ky

void CBufProg::DispNewItem(int index)
{
// kontrola platnosti polo�ky a stromu
	if (IsNotValid(index) || (m_Tree == NULL)) return;

// lok�ln� prom�nn�
	TV_INSERTSTRUCT tvins;					// struktura k vlo�en� polo�ky
	PROGITEM* item = m_Data + index;		// adresa polo�ky k zobrazen�

// adresa polo�ky k zobrazen�
	item = m_Data + index;

// maska parametr� polo�ky
	tvins.item.mask = TVIF_CHILDREN | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_STATE | TVIF_TEXT;

// p��prava textu polo�ky
	tvins.item.pszText = (LPTSTR)(LPCTSTR)Text.GetTxt(GetText(index));

// ikona polo�ky - zp�tn� vol�n�
	tvins.item.iImage =  I_IMAGECALLBACK;
	tvins.item.iSelectedImage =  I_IMAGECALLBACK;

// p�ednastaven� stavu
	tvins.item.stateMask = TVIS_EXPANDED | TVIS_BOLD | TVIS_CUT | TVIS_STATEIMAGEMASK;

// p��prava stavov� ikony
	tvins.item.state = INDEXTOSTATEIMAGEMASK(GetStateImage(index));

// p��prava p��znaku rozvinut� polo�ky
	if (item->Param & PR_EXP)
	{
		tvins.item.state |= TVIS_EXPANDED;
	}

// p��znak uzam�en� polo�ky
	if (item->Param & (PR_LOCK | PR_LOCK_DEP))
	{
		tvins.item.state |= TVIS_BOLD;
	}

// p��znak vypnut� polo�ky
	if (item->Param & (PR_OFF | PR_OFF_DEP))
	{
		tvins.item.state |= TVIS_CUT;
	}

// p��prava polo�ky potomk�
	if ((item->Child >= 0) || ((m_BufID == BufClsID) && ((item->Func == IDF_GROUP) || ((item->Parent < 0) && (item->Func != IDF_FNC)))))
	{
		tvins.item.cChildren = 1;
	}
	else
	{
		tvins.item.cChildren = 0;
	}

// p��prava rodi�e polo�ky
	if (item->Parent >= 0)
	{
		tvins.hParent = m_Data[item->Parent].HTree;
	}
	else
	{
		tvins.hParent = TVI_ROOT;
	}

// p��prava p�edch�zej�c� polo�ky
	if (item->Prev >= 0)
	{
		tvins.hInsertAfter = m_Data[item->Prev].HTree;
	}
	else
	{
		tvins.hInsertAfter = TVI_FIRST;
	}

// zobrazen� polo�ky ve stromu a �schova handle polo�ky
	item->HTree = (HTREEITEM)::SendMessage(m_Tree, TVM_INSERTITEM, 0, (LPARAM)&tvins);
}


////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CBufProg::CBufProg()
{
	m_Data = NULL;			// nen� buffer dat
	m_Valid = NULL;			// nen� buffer platnosti
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
	m_Next = -1;			// p�i�t� voln� polo�ka (-1=nen�)
	m_Undo = false;			// neregistrovat zm�ny pro UNDO

	m_Redraw = 0;			// p�ekreslov�n� okna zapnuto
	m_First = -1;			// nen� prvn� (ROOT) polo�ka)
	m_Tree = NULL;			// nen� p�ipojen� strom
	m_Disp = -2;			// nen� nic zobrazeno
	m_IconWidth = 0;		// ���ka ikon (0 = nen�)
	m_IconHeight = 0;		// v��ka ikon (0 = nen�)
	m_Def = -1;				// definovan� polo�ka (-1 = nen�)

	m_CharSet = DEFAULT_CHARSET; // implicitn� znakov� sada
	m_Font = NULL;			// font nen�
}

CBufProg::~CBufProg()
{
	DelAll();				// zru�en� bufferu
}


////////////////////////////////////////////////////////////////////
// statick� konstruktor a destruktor

void CBufProg::Init()
{
	m_Data = NULL;			// nen� buffer dat
	m_Valid = NULL;			// nen� buffer platnosti
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� buffer polo�ek
	m_Next = -1;			// p�i�t� voln� polo�ka (-1=nen�)
	m_Undo = false;			// neregistrovat zm�ny pro UNDO

	m_Redraw = 0;			// p�ekreslov�n� okna zapnuto
	m_First = -1;			// nen� prvn� (ROOT) polo�ka)
	m_Tree = NULL;			// nen� p�ipojen� strom
	m_Disp = -2;			// nen� nic zobrazeno
	m_IconWidth = 0;		// ���ka ikon (0 = nen�)
	m_IconHeight = 0;		// v��ka ikon (0 = nen�)
	m_Def = -1;				// definovan� polo�ka (-1 = nen�)

	m_Nadpis.Init();		// inicializace nadpisu okna

	m_FontSet.Init();		// inicializace standardn�ho fontu
	m_FontSet.Height = -1;	// v��ka fontu neur�ena
	m_CharSet = DEFAULT_CHARSET; // implicitn� znakov� sada
	m_Font = NULL;			// font nen�
}

void CBufProg::Term()
{
	DelAll();				// zru�en� bufferu
	m_Nadpis.Term();		// deinicializace nadpisu okna
	m_FontSet.Term();		// deinicializace defincie fontu
	DelFont(m_Font);		// zru�en� fontu
}


////////////////////////////////////////////////////////////////////
// zru�en� v�ech polo�ek v bufferu (ukl�d�n� za�ne op�t po �ad�, nem�n� obsah okna)

void CBufProg::DelAll()
{
	MemFree(m_Data);		// zru�en� datov�ho bufferu
	m_Data = NULL;			// nen� datov� buffer
	MemFree(m_Valid);		// zru�en� bufferu platnosti
	m_Valid = NULL;			// nen� buffer platnosti
	m_Num = 0;				// nen� ��dn� platn� polo�ka
	m_Max = 0;				// nen� ��dn� polo�ka v bufferu
	m_Next = -1;			// nen� p��t� polo�ka

	m_First = -1;			// nen� prvn� (ROOT) polo�ka)
	m_Disp = -2;			// nen� nic zobrazeno
	m_Def = -1;				// definovan� polo�ka (-1 = nen�)
}


////////////////////////////////////////////////////////////////////
// vypnut� p�ekreslov�n� okna stromu

void CBufProg::RedrawOff()
{
// test, zda bylo p�ekreslov�n� zapnuto
	if (m_Redraw == 0)
	{

// vypnut� aktualizace stromu
		if (m_Tree != NULL)
		{
			::SendMessage(m_Tree, WM_SETREDRAW, FALSE, 0);
		}
	}

// zv��en� ��ta�e �rovn� vypnut� p�ekreslov�n�
	m_Redraw++;
}


////////////////////////////////////////////////////////////////////
// zapnut� p�ekreslov�n� okna stromu

void CBufProg::RedrawOn()
{
// sn�en� ��ta�e �rovn� vypnut� p�ekreslov�n�
	m_Redraw--;

// test, zda bude p�ekreslov�n� zapnuto
	if (m_Redraw == 0)
	{

// zapnut� aktualizace stromu
		if (m_Tree != NULL)
		{
			::SendMessage(m_Tree, WM_SETREDRAW, TRUE, 0);
		}
	}
}


////////////////////////////////////////////////////////////////////
// nastaven� rodi�e zobrazen� polo�ek + zobrazen� polo�ek (-1 = v�e, -2 = nic)

void CBufProg::Disp(int disp)
{
// test, zda se rodi� zobrazen� polo�ek m�n�
	if (disp == m_Disp) return;

// vypnut� aktualizace stromu
	RedrawOff();

// lok�ln� prom�nn�
	PROGITEM* item;
	int index;

// vymaz�n� aktu�ln� zobrazen�ho obsahu stromu (je-li n�co zobrazeno)
	if (m_Disp != -2)
	{
		if (m_Tree != NULL)
		{
			::SendMessage(m_Tree, TVM_DELETEITEM, 0, (LPARAM)TVI_ROOT);
		}

		for (index = m_Max - 1; index >= 0; index--)
		{
			if (m_Valid[index]) m_Data[index].HTree = NULL;
		}
	}

// kontrola platnosti nov�ho rodi�e polo�ek k zobrazen�
	if ((disp != -1) && IsNotValid(disp))
	{
		m_Disp = -2;		
		RedrawOn();
		return;
	}

// nastaven� nov�ho rodi�e zobrazen� polo�ek
	m_Disp = disp;

// p��prava prvn� polo�ky k zobrazen�
	if (disp < 0)
	{
		index = m_First;
	}
	else
	{
		index = m_Data[disp].Child;
	}

// cyklus zobrazen� polo�ek
	if (index >= 0)
	{
		do {

// nov� zobrazen� polo�ky
			DispNewItem(index);

// adresa polo�ky
			item = m_Data + index;

// nalezen� n�sleduj�c� polo�ky
			index = item->Child;			// vno�en� na potomka

			if (index < 0)					// je potomek?
			{
				index = item->Next;			// pokra�ov�n� dal�� polo�kou

				while ((index < 0) && (item->Parent >= 0))
				{
					index = item->Parent;	// n�vrat k rodi�i
					if (index == disp) break; // je ji� op�t v�choz� polo�ka
					item = m_Data + index;	// adresa rodi�e
					index = item->Next;		// dal�� polo�ka za rodi�em
				}
			}

// dokud je dal�� platn� polo�ka
		} while (index != disp);
	}

// zapnut� aktualizace stromu
	RedrawOn();
};


////////////////////////////////////////////////////////////////////
// nastaveni ikon ve stromu (zoom: TRUE = ikony 32*32)

void CBufProg::IconList(SMALLICON zoom)
{
// lok�ln� prom�nn�
	HIMAGELIST img;			// seznam ikon
	HIMAGELIST imgs;		// seznam stavov�ch ikon
	int icon;				// rozm�r ikon

// pou�ity velk� ikony
	switch (zoom)
	{
	case SI_BIG:
		icon = 32;
		img = ProgImageList;
		imgs = ProgStateList;
		break;

	case SI_MEDIUM:
		icon = 24;
		img = ProgImageListMedium;
		imgs = ProgStateListMedium;
		break;

	default:
		icon = 16;
		img = ProgImageListSmall;
		imgs = ProgStateListSmall;
	}

// test, zda je rozm�r ikon ji� nastaven
	if ((icon == m_IconWidth) && (icon == m_IconHeight)) return;

// nastaven� nov�ho rozm�ru ikon
	m_IconWidth = icon;
	m_IconHeight = icon;

	if (m_Tree != NULL)
	{
		::SendMessage(m_Tree, TVM_SETIMAGELIST, TVSIL_NORMAL, (LPARAM)img);
		::SendMessage(m_Tree, TVM_SETIMAGELIST, TVSIL_STATE, (LPARAM)imgs);
		::SendMessage(m_Tree, TVM_SETINDENT, icon+3, 0);
	}
}


////////////////////////////////////////////////////////////////////
// nastaven� definovan� polo�ky (-1 = odzna�en�)

void CBufProg::Def(int index)
{
// kontrola indexu
	if (IsNotValid(index)) index = -1;

// kontrola, zda se definovan� polo�ka m�n�
	if (index == m_Def) return;

// test, zda je zobrazen strom
	if (m_Tree != NULL)
	{

// lok�ln� prom�nn�
		TV_ITEM tvi;

// zru�en� ozna�en� star� polo�ky
		if (IsValid(m_Def) && (m_Data[m_Def].HTree != NULL))
		{
			tvi.mask = TVIF_STATE;
			tvi.stateMask = TVIS_STATEIMAGEMASK;
			tvi.hItem = m_Data[m_Def].HTree;
			tvi.state = INDEXTOSTATEIMAGEMASK(STATE_NONE);
			::SendMessage(m_Tree, TVM_SETITEM, 0, (LPARAM)&tvi);
		}

// ulo�en� nov� polo�ky
		m_Def = index;

// bude zobrazen� nov� polo�ky
		if (IsValid(index) && (m_Data[index].HTree != NULL))
		{

// p��prava klientsk�ch rozm�r� okna
			RECT wrc;
			::GetClientRect(m_Tree, &wrc);

// zji�t�n�, zda je polo�ka viditeln� (zda je rychlej�� nevyp�nat p�ekreslov�n�)
			RECT rc;
			*(HTREEITEM*)&rc = m_Data[index].HTree;
			BOOL visible = ::SendMessage(m_Tree, TVM_GETITEMRECT, FALSE, (LPARAM) &rc);

// up�esn�n� viditelnosti polo�ky
			if (visible)
			{
				visible = ((rc.top >= 0) && (rc.bottom <= wrc.bottom));
			}

// vypnut� p�ekreslov�n�, nen�-li polo�ka viditeln�
			if (!visible) RedrawOff();

// nastaven� ozna�en� nov� polo�ky
			tvi.mask = TVIF_STATE;
			tvi.stateMask = TVIS_STATEIMAGEMASK;
			tvi.hItem = m_Data[index].HTree;
			tvi.state = INDEXTOSTATEIMAGEMASK(STATE_DEF);
			::SendMessage(m_Tree, TVM_SETITEM, 0, (LPARAM)&tvi);

// zaji�t�n� viditelnosti polo�ky
			::SendMessage(m_Tree, TVM_ENSUREVISIBLE, 0, (LPARAM)m_Data[index].HTree);

// zapnut� p�ekreslov�n�
			if (!visible) RedrawOn();
		}
	}
	else

// jinak nastaven� indexu polo�ky bez p�ekreslen� okna
	{
		m_Def = index;
	}
}


////////////////////////////////////////////////////////////////////
// nastaven� nadpisu okna

void CBufProg::Nadpis(const CText& text)
{
// kontrola, zda se text m�n�
	if (m_Nadpis == text) return;

// nastaven� nov�ho nadpisu
	m_Nadpis = text;

// zobrazen� nov�ho nadpisu
	if (m_Tree != NULL)
	{
		ProgDispNadpis();
	}
}


////////////////////////////////////////////////////////////////////
// poskytnut� prvn� zobrazen� polo�ky (-1 = nen�)

int CBufProg::Top()
{
// kontrola, zda je strom platn�
	if (m_Tree == NULL) return -1;

// na�ten� zobrazen� polo�ky
	HTREEITEM htree = (HTREEITEM)::SendMessage(m_Tree, TVM_GETNEXTITEM, TVGN_FIRSTVISIBLE, 0);

// nalezen� indexu polo�ky
	return Find(htree);
}


////////////////////////////////////////////////////////////////////
// nastaven� prvn� zobrazen� polo�ky

void CBufProg::Top(int index)
{
// kontrola, zda je strom platn�
	if (m_Tree == NULL) return;

// kontrola, zda je polo�ka platn�
	if (IsNotValid(index)) return;

// �schova sou�asn� zobrazen� prvn� polo�ky
	HTREEITEM htree = (HTREEITEM)::SendMessage(m_Tree, TVM_GETNEXTITEM, TVGN_FIRSTVISIBLE, 0);

// je-li ji� polo�ka nastaven�, nic se neprovede
	if (m_Data[index].HTree == htree) return;

// nastaven� polo�ky
	RedrawOff();
	::SendMessage(m_Tree, TVM_SELECTITEM, TVGN_FIRSTVISIBLE, (LPARAM)m_Data[index].HTree);
	RedrawOn();
}


////////////////////////////////////////////////////////////////////
// poskytnut� vybran� polo�ky (-1 = nen�)

int CBufProg::Select()
{
// kontrola, zda je strom platn�
	if (m_Tree == NULL) return -1;

// na�ten� vybran� polo�ky
	HTREEITEM htree = (HTREEITEM)::SendMessage(m_Tree, TVM_GETNEXTITEM, TVGN_CARET, 0);

// nalezen� indexu polo�ky
	return Find(htree);
}


////////////////////////////////////////////////////////////////////
// nastaven� vybran� polo�ky (-1 = nen�)

void CBufProg::Select(int index)
{
// kontrola, zda je strom platn�
	if (m_Tree == NULL) return;

// je-li ji� polo�ka vybran�, nic se neprovede
	if (index == Select()) return;

// nen�-li polo�ka platn�, vypnut� vybran� polo�ky
	if (IsNotValid(index) || (m_Data[index].HTree == NULL))
	{
		::SendMessage(m_Tree, TVM_SELECTITEM, TVGN_CARET, 0);
	}
	else
	{

// p��prava klientsk�ch rozm�r� okna
		RECT wrc;
		::GetClientRect(m_Tree, &wrc);

// zji�t�n�, zda je polo�ka viditeln� (zda je rychlej�� nevyp�nat p�ekreslov�n�)
		RECT rc;
		*(HTREEITEM*)&rc = m_Data[index].HTree;
		BOOL visible = ::SendMessage(m_Tree, TVM_GETITEMRECT, FALSE, (LPARAM) &rc);

// up�esn�n� viditelnosti polo�ky
		if (visible)
		{
			visible = ((rc.top >= 0) && (rc.bottom <= wrc.bottom));
		}

// vypnut� p�ekreslov�n�, nen�-li polo�ka viditeln�
		if (!visible) RedrawOff();

// zm�na vybran� polo�ky
		::SendMessage(m_Tree, TVM_SELECTITEM, TVGN_CARET, (LPARAM)m_Data[index].HTree);

// zaji�t�n� viditelnosti polo�ky
		::SendMessage(m_Tree, TVM_ENSUREVISIBLE, 0, (LPARAM)m_Data[index].HTree);

// zapnut� p�ekreslov�n�
		if (!visible) RedrawOn();
	}
}


////////////////////////////////////////////////////////////////////
// aktualizace zobrazen� stavov� ikony polo�ky pro okno editoru programu

void _fastcall CBufProg::AktStateImage(int index)
{
// je polo�ka platn� (a je to okno editoru programu)?
	if ((m_BufID == BufEdiID) && 
		IsValid(index) && 
		(m_Data[index].HTree != NULL) && 
		(m_Tree != NULL))
	{

// po�adovan� nov� stav polo�ky
		DWORD newstate = INDEXTOSTATEIMAGEMASK(GetStateImage(index));

// na�ten� star�ho stavu polo�ky
		TV_ITEM tvi;
		tvi.hItem = m_Data[index].HTree;
		tvi.mask = TVIF_STATE;
		tvi.stateMask = TVIS_STATEIMAGEMASK;
		tvi.state = newstate;
		::SendMessage(m_Tree, TVM_GETITEM, 0, (LPARAM)&tvi);
		
// nastaven� nov�ho stavu polo�ky, pokud se m�n�
		if (newstate != (tvi.state & TVIS_STATEIMAGEMASK))
		{
			tvi.mask = TVIF_STATE;
			tvi.stateMask = TVIS_STATEIMAGEMASK;
			tvi.state = newstate;
			::SendMessage(m_Tree, TVM_SETITEM, 0, (LPARAM)&tvi);
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// index n�sleduj�c� polo�ky ve stromu (cyklicky, -1 = navr�t� se prvn� polo�ka)

int _fastcall CBufProg::NextItem(int index)
{
// kontrola platnosti polo�ky
	if (IsNotValid(index)) return m_First;

// adresa polo�ky
	PROGITEM* item = m_Data + index;

// p�esun na potomka
	index = item->Child;				// potomek

// nen�-li potomek, p�esun na n�sleduj�c� prvek
	if (index < 0)						// je n�jak� potomek?
	{
		index = item->Next;				// n�sleduj�c� prvek

// nen�-li n�sleduj�c� prvek, p�esun na rodi�e
		while (index < 0)				// je n�sleduj�c� prvek?
		{
			index = item->Parent;		// index rodi�e

			if (index < 0)				// je platn� rodi� ?
			{
				index = m_First;		// prvn� polo�ka v ROOT
				ASSERT(index >= 0);
			}
			else
			{
				item = m_Data + index;	// adresa rodi�e
				index = item->Next;		// n�sleduj�c� prvek
			}
		}
	}
	return index;
}


/////////////////////////////////////////////////////////////////////////////
// index p�edch�zej�c� polo�ky ve stromu (cyklicky, -1 = navr�t� se posledn� polo�ka)

int _fastcall CBufProg::PrevItem(int index)
{
// kontrola platnosti polo�ky
	if (IsNotValid(index)) index = m_First;

// nen� ��dn� polo�ka
	if (IsNotValid(index)) return -1;

// adresa polo�ky
	PROGITEM* item = m_Data + index;

// p�esun na p�edch�zej�c� polo�ku
	index = item->Prev;				// p�edch�zej�c� polo�ka

// kontrola, zda je p�edch�zej�c� polo�ka
	if (index >= 0)					// je p�edch�zej�c� polo�ka?
	{
		item = m_Data + index;		// adresa p�edch�zej�c� polo�ky

// m�-li p�edch�zej�c� polo�ka potomka, p�esun na potomka
		if (item->Child >= 0)		// je n�jak� potomek?
		{
			index = item->Child;	// potomek
			item = m_Data + index;	// adresa potomka

// nalezen� posledn� polo�ky potomka
			while ((item->Next >= 0) || (item->Child >= 0))
			{
				while (item->Next >= 0)
				{
					index = item->Next;	// dal�� polo�ka
					item = m_Data + index; // adresa dal�� polo�ky
				}

				if (item->Child >= 0)	// je n�jak� potomek?
				{
					index = item->Child; // potomek
					item = m_Data + index; // adresa potomka
				}
			}
		}
	}

// nen� p�edch�zej�c� polo�ka, p�esun na rodi�e
	else
	{
		index = item->Parent;		// index rodi�e

// nen�-li dal�� rodi�, p�esun na prvn� polo�ku ROOT
		if (index < 0)				// je rodi�?
		{
			index = m_First;		// prvn� polo�ka v ROOT
			ASSERT(index >= 0);
			item = m_Data + index;	// adresa prvn� polo�ky

// nalezen� �pln� posledn� polo�ky stromu
			while ((item->Next >= 0) || (item->Child >= 0))
			{
				while (item->Next >= 0)
				{
					index = item->Next;	// dal�� polo�ka
					item = m_Data + index; // adresa dal�� polo�ky
				}

				if (item->Child >= 0)	// je n�jak� potomek?
				{
					index = item->Child; // potomek
					item = m_Data + index; // adresa potomka
				}
			}
		}
	}
	return index;
}


////////////////////////////////////////////////////////////////////
// navr�cen� polo�ky v UNDO operaci (vrac� TRUE=operace OK)
// prov�d� z�znam do UNDO bufferu

bool CBufProg::UndoIns(const PROGITEM* item, const int index)
{
// vytvo�en� nov�ch dat (zaji�t�n� existence polo�ky)
	ASSERT(index >= 0);
	while (index >= m_Max)
	{
		if (!NewData()) return false;
	}
	ASSERT(IsNotValid(index));					// polo�ka mus� b�t neplatn�

// lok�ln� prom�nn�
	PROGITEM*	itemNew = m_Data + index;			// adresa nov� polo�ky
	int			parent = item->Parent;				// rodi� polo�ky
	PROGITEM*	itemPar = m_Data + parent;			// adresa rodi�e
	int			prev = item->Prev;					// p�edch�zej�c� polo�ka
	PROGITEM*	itemAft = m_Data + prev;			// adresa p�edch�zej�c� polo�ky
	int			next = item->Next;					// n�sleduj�c� polo�ka
	PROGITEM*	itemNxt = m_Data + next;			// adresa n�sleduj�c� polo�ky

	ASSERT((parent == -1) || (IsValid(parent)));

// z�znam operace pro UNDO
	if (m_Undo)
	{
		if (!Undo.AddProgIns(m_BufID, index)) return false;
	}

// vyjmut� polo�ky z voln�ch polo�ek
	int i = m_Next;								// ukazatel �et�zce voln�ch polo�ek

// navr�cena hned prvn� polo�ka z �et�zce voln�ch polo�ek
	if (i == index)								// je to hned prvn� polo�ka?
	{
		m_Next = *(int*)itemNew;				// odkaz na dal�� polo�ku
	}
	else
	{

// nalezen� polo�ky v �et�zci voln�ch polo�ek
		while (*(int*)(m_Data + i) != index)
		{
			i = *(int*)(m_Data + i);

// tento p��pad nesm� nikdy nastat - polo�ka nebyla nalezena!!!
			ASSERT(i >= 0);
			if (i < 0)								// to je chyba!!!!!!!
			{
				Undo.DelAll();
				return false;
			}
		}

// p�esko�en� odkazu na dal�� polo�ku
		*(int*)(m_Data + i) = *(int*)itemNew;
	}

// nastaven� p��znaku platnosti polo�ky
	m_Num++;									// zv��en� ��ta�e polo�ek
	m_Valid[index] = true;						// p��znak platnosti polo�ky

// p�enesen� dat polo�ky
	MemCopy(itemNew, item, SIZEOFPROGITEM);

// napojen� na p�edch�zej�c� polo�ku a na rodi�e
	if (IsValid(prev))
	{
		itemNew->Next = itemAft->Next;
		itemAft->Next = index;
	}
	else
	{
		itemNew->Prev = -1;
		if (IsValid(parent))
		{
			itemNew->Next = itemPar->Child;
			itemPar->Child = index;

			if ((itemNew->Next < 0) && (itemPar->HTree != NULL) && (m_Tree != NULL))	// prvn� potomek rodi�e?
			{
				TV_ITEM tvi;
				tvi.hItem = itemPar->HTree;
				tvi.mask = TVIF_CHILDREN;
				tvi.cChildren = 1;
				::SendMessage(m_Tree, TVM_SETITEM, 0, (LPARAM)&tvi);

				AktStateImage(parent);		// aktualizace stavov� ikony pro okno editoru
			}
		}
		else
		{
			itemNew->Parent = -1;			// nen� rodi�
			itemNew->Next = m_First;		// p�ed�azen� p�ed prvn� ROOT polo�ku
			m_First = index;				// polo�ka bude prvn� ROOT polo�kou
		}
	}

// napojen� p�ed n�sleduj�c� polo�ku
	if (IsValid(next))
	{
		itemNxt->Prev = index;
	}
	else
	{
		itemNew->Next = -1;
	}

// inicializace ostatn�ch parametr�
	itemNew->Child = -1;					// nen� ��dn� potomek
	itemNew->HTree = NULL;					// nen� zobrazen� polo�ky stromu
	itemNew->Refer = 0;						// nejsou ��dn� reference na polo�ku

// zv��en� ��ta�e u referen�n� polo�ky (krom� bufferu t��d)
	if (((DWORD)(itemNew->RefBlok) < (DWORD)PROGBUFNUM) && (m_BufID != BufClsID))
	{
		if (BufProg[itemNew->RefBlok].IsValid(itemNew->RefIndex))
		{
			BufProg[itemNew->RefBlok][itemNew->RefIndex].Refer++;
		}
	}

// zobrazen� polo�ky, je-li v zobrazen� v�tvi
	if ((((parent >= 0) && (itemPar->HTree != NULL)) ||
		(m_Disp == parent)))
	{
		DispNewItem(index);
	}

	return true;
}


////////////////////////////////////////////////////////////////////
// vlo�en� polo�ky bez nav�z�n� dat (pro UNDO buffer)
// vrac� index polo�ky, p�i chyb� pam�ti vrac� -1

int CBufProg::Insert0(const PROGITEM* item)
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	if (result >= 0)
	{
		MemCopy(m_Data + result, item, SIZEOFPROGITEM);
	}
	return result;
}


////////////////////////////////////////////////////////////////////
// zru�en� polo�ky bez nav�z�n� dat (pro UNDO buffer)

void CBufProg::Del0(int index)
{
	if (IsValid(index)) DelItem(index);
}


////////////////////////////////////////////////////////////////////
// vlo�en� polo�ky (vrac� index polo�ky, p�i chyb� pam�ti vrac� -1) - parametry nemus� b�t platn�
//  parent = rodi� (-1=ROOT), after=p�edch�zej�c� polo�ka 
//		(-1=prvn�, -2=posledn�, -3=abecedn�, -4=abecedn� s p�ednost� skupin)
//  Polo�ky nastaven� funkc�: Parent, Next, Prev, Child, HTree, Refer,

int CBufProg::Insert(const PROGITEM* item, int parent, int after)
{
// zalo�en� nov� polo�ky
	int index = NewItem();					// vytvo�en� nov� polo�ky
	if (index < 0) return -1;				// chyba pam�ti

// z�znam operace pro UNDO
	if (m_Undo)
	{
		if (!Undo.AddProgIns(m_BufID, index))
		{
			DelItem(index);
			return -1;
		}
	}

// kopie obsahu polo�ky
	PROGITEM* itemNew = m_Data + index;		// adresa nov� polo�ky
	MemCopy(itemNew, item, SIZEOFPROGITEM);	// kopie polo�ky

// kontrola platnosti rodi�e, p��prava adresy rodi�e
	if (IsNotValid(parent)) parent = -1;	// nen� platn� - bude to ROOT polo�ka
	PROGITEM* itemPar = m_Data + parent;	// adresa rodi�e

// nastaven� odkazu na rodi�e
	itemNew->Parent = parent;				// odkaz na rodi�e (pro ROOT bude = -1)

// nalezen� p�edchoz� polo�ky abecedn�
	if (after == -3)						// je abecedn� t��d�n�?
	{
		after = -1;							// ulo�en� na za��tek

// p��prava textu vkl�dan� polo�ky
		CText txt;
		if (item->Name >= 0)
		{
			txt = Text.GetTxt(item->Name);
		}
		else
		{
			if (item->RefBlok >= 0)
			{
				txt = Text.GetTxt(BufProg[item->RefBlok].GetText(item->RefIndex));
			}
		}

// p��prava indexu prvn� polo�ky
		int inx = m_First;					// prvn� polo�ka, nen�-li rodi� platn�
		if (IsValid(parent))				// je rodi� platn�?
		{
			inx = itemPar->Child;			// prvn� potomek rodi�e
		}

// nalezen� pozice
		while (IsValid(inx) && (txt >= Text.GetTxt(GetText(inx))))
		{
			after = inx;					// polo�ka bude p�edch�zej�c�
			inx = m_Data[inx].Next;			// posun na dal�� polo�ku
		}
	}

// nalezen� p�edchoz� polo�ky abecedn� s p�ednost� skupin
	if (after == -4)						// je abecedn� t��d�n�?
	{
		after = -1;							// ulo�en� na za��tek

// p��prava textu vkl�dan� polo�ky
		CText txt;
		if (item->Name >= 0)
		{
			txt = Text.GetTxt(item->Name);
		}
		else
		{
			if (item->RefBlok >= 0)
			{
				txt = Text.GetTxt(BufProg[item->RefBlok].GetText(item->RefIndex));
			}
		}

// p��prava indexu prvn� polo�ky
		int inx = m_First;					// prvn� polo�ka, nen�-li rodi� platn�
		if (IsValid(parent))				// je rodi� platn�?
		{
			inx = itemPar->Child;			// prvn� potomek rodi�e
		}

// nalezen� pozice
		while (IsValid(inx) &&				// polo�ka je platn�
				(
					(
						(m_Data[inx].Func == IDF_GROUP) && // ne-skupina se �ad� v�dy za skupinu
						(item->Func != IDF_GROUP)
					) ||
					(
						(m_Data[inx].Func == IDF_GROUP) &&	// abecedn� set��d�n� skupin
						(item->Func == IDF_GROUP) &&
						(txt >= Text.GetTxt(GetText(inx)))
					) ||
					(
						(m_Data[inx].Func != IDF_GROUP) &&	// abecedn� set��d�n� ne-skupin
						(item->Func != IDF_GROUP) &&
						(txt >= Text.GetTxt(GetText(inx)))
					)
				)
			)
		{
			after = inx;					// polo�ka bude p�edch�zej�c�
			inx = m_Data[inx].Next;			// posun na dal�� polo�ku
		}
	}

// p��prava adresy p�ede�l� polo�ky (i kdy� je neplatn�)
	PROGITEM* itemAft = m_Data + after;				// adresa p�edch�zej�c� polo�ky

// nem�-li to b�t prvn� polo�ka, bude se hledat napojen� na p�edch�zej�c� polo�ku
	if (after != -1)
	{

// nalezen� stejn�ho rodi�e, je-li p�ede�l� polo�ka v podv�tvi
		if (IsValid(after))					// je p�ede�l� polo�ka platn�?
		{
			while ((itemAft->Parent != parent) && (itemAft->Parent >= 0))
			{
				after = itemAft->Parent;
				itemAft = m_Data + after;			// adresa p�edch�zej�c� polo�ky
			}
		}

// ov��en�, zda je p�edch�zej�c� polo�ka platn� (mus� m�t stejn�ho rodi�e)
		if (IsNotValid(after) || (itemAft->Parent != parent))
		{	

// p��prava v�choz� polo�ky - bu� prvn� potomek rodi�e nebo prvn� polo�ka ROOT
			if (parent >= 0)					// je rodi� platn�?
			{
				after = itemPar->Child;			// prvn� potomek rodi�e
			}
			else
			{
				after = m_First;				// jinak prvn� polo�ka v ROOT
			}

// nalezen� posledn� polo�ky
			if (after >= 0)						// je n�jak� polo�ka?
			{
				itemAft = m_Data + after;		// adresa polo�ky

				while (itemAft->Next >= 0)		// je platn� dal�� polo�ka?
				{
					after = itemAft->Next;		// posun na dal�� polo�ku
					itemAft = m_Data + after;	// adresa dal�� polo�ky
				}
			}
		}
	}

// napojen� polo�ky na p�edch�zej�c� polo�ku (-1 = prvn�)
	itemNew->Prev = after;					// odkaz na p�edch�zej�c� polo�ku
	if (after >= 0)							// je p�edch�zej�c� polo�ka platn�?
	{
		itemNew->Next = itemAft->Next;		// p�enesen� n�sleduj�c� polo�ky
		itemAft->Next = index;				// nav�z�n� odkazu na tuto polo�ku
	}
	else
	{
		if (parent >= 0)					// je rodi� platn�?
		{
			itemNew->Next = itemPar->Child;	// p�ed�azen� p�ed prvn�ho potomka
			itemPar->Child = index;			// polo�ka bude prvn�m potomkem rodi�e

			if ((itemNew->Next < 0) && (itemPar->HTree != NULL) && (m_Tree != NULL))	// prvn� potomek rodi�e?
			{
				TV_ITEM tvi;
				tvi.hItem = itemPar->HTree;
				tvi.mask = TVIF_CHILDREN;
				tvi.cChildren = 1;
				::SendMessage(m_Tree, TVM_SETITEM, 0, (LPARAM)&tvi);

				AktStateImage(parent);		// aktualizace stavov� ikony
			}
		}
		else
		{
			itemNew->Next = m_First;		// p�ed�azen� p�ed prvn� ROOT polo�ku
			m_First = index;				// polo�ka bude prvn� ROOT polo�kou
		}
	}

// napojen� p�ed n�sleduj�c� polo�ku
	if (itemNew->Next >= 0)
	{
		m_Data[itemNew->Next].Prev = index;
	}

// inicializace ostatn�ch parametr�
	itemNew->Child = -1;					// nen� ��dn� potomek
	itemNew->HTree = NULL;					// nen� zobrazen� polo�ky stromu
	itemNew->Refer = 0;						// nejsou ��dn� reference na polo�ku

// zv��en� ��ta�e u referen�n� polo�ky (ne pro buffer t��d)
	if (((DWORD)(itemNew->RefBlok) < (DWORD)PROGBUFNUM) && (m_BufID != BufClsID))
	{
		if (BufProg[itemNew->RefBlok].IsValid(itemNew->RefIndex))
		{
			BufProg[itemNew->RefBlok][itemNew->RefIndex].Refer++;
		}
	}

// zobrazen� polo�ky, je-li v zobrazen� v�tvi
	if ((((parent >= 0) && (itemPar->HTree != NULL)) ||
		(m_Disp == parent)))
	{
		DispNewItem(index);
	}

// navr�cen� indexu nov� polo�ky
	return index;
}


////////////////////////////////////////////////////////////////////
// na�ten� inicializa�n� tabulky bufferu (star� obsah bufferu se neru�� - lze p�id�vat)
// p�i chyb� pam�ti vrac� FALSE

bool CBufProg::InitTab(const INITTREE* tab, const int num)
{
// vypnut� p�ekreslov�n� okna
	RedrawOff();

// p��prava bufferu rodi��
	int ParBuf[30];							// to snad bude v�dy dosta�uj�c�
	for (int i = 0; i < 30; i++) ParBuf[i] = -1; // rodi� je neplatn�

// inicializace nem�nn�ch parametr� polo�ky
	PROGITEM item;							// vytv��en� nov� polo�ka
	item.RefBlok = BufIntID;				// reference na buffer intern�ch funkc�
	item.DatBlok = -1;						// datov� blok nen�
	item.DatIndex = -1;						// datov� index
	item.Icon = -1;							// ikona implicitn�
	item.Name = -1;							// jm�no implicitn�

// cyklus p�es polo�ky tabulky
	for (i = 0; i < num; i++)
	{

// p��prava odkazu na deklara�n� funkci
		item.Func = tab[i].Func;			// identifika�n� k�d funkce
		int fnc = tab[i].Func - IDF;		// index funkce
		ASSERT(BufInt.IsValid(fnc));
		PROGITEM* itemfnc = &BufInt[fnc];	// adresa deklara�n� funkce

// p�enesen� parametr� z deklara�n� funkce
		item.SrcMask = itemfnc->SrcMask;	// zdrojov� vlastnosti
		item.DstMask = itemfnc->DstMask;	// c�lov� vlastnosti
		item.Param = itemfnc->Param;		// parametry

// odkaz na deklara�n� funkci
		item.RefIndex = fnc;				// referen�n� index

// p�id�n� polo�ky do bufferu (a aktualizace bufferu rodi��)
		int level = tab[i].Level;
		if (level & NOREFER) item.Param |= PR_NOREFER;
		level = level & ~NOREFER;

		ASSERT(level < 28);
		int ii = Insert(&item, ParBuf[level], -2);
		if (ii < 0)
		{
			RedrawOn();
			return false;
		}
		ParBuf[level + 1] = ii;
	}
// zapnut� p�ekreslov�n� okna
	RedrawOn();

	return true;
}


////////////////////////////////////////////////////////////////////
// vytvo�en� pr�zdn� polo�ky (vrac� index polo�ky, -1=chyba pam�ti)

int CBufProg::New()
{
	int result = NewItem();		// vytvo�en� nov� polo�ky
	if (result >= 0)
	{
		MemCopy(m_Data + result, &InitProgItem, SIZEOFPROGITEM);
	}
	return result;
}


////////////////////////////////////////////////////////////////////
// nalezen� polo�ky podle handle (-1 = nen�)

int CBufProg::Find(HTREEITEM htree)
{
// je to platn� handle?
	if (htree == NULL) return -1;

// p��prava v�choz� zobrazen� polo�ky
	int index;
	if (m_Disp < 0)
	{
		index = m_First;
	}
	else
	{
		index = m_Data[m_Disp].Child;
	}

// kontrola, zda je platn� v�choz� polo�ka
	if (IsNotValid(index)) return -1;

// p��prava koncov�ho indexu
	int last = m_Data[index].Parent;		// koncov� index

// cyklus nalezen� polo�ky
	for (;;)
	{

// adresa testovan� polo�ky
		PROGITEM* item = m_Data + index;			// adresa testovan� polo�ky

// test, zda je to hledan� polo�ka
		if (item->HTree == htree) return index;

// pokus o vno�en� do potomk�
		index = item->Child;			// vno�en� na potomka
		if (index < 0)					// nen� ��dn� potomek?
		{

// nejsou-li potomci, zkus� se n�sleduj�c� polo�ka
			index = item->Next;			// pokra�ov�n� dal�� polo�kou

// nen�-li ani dal�� polo�ka, vr�t� se k rodi�i
			while (index < 0)
			{
				index = item->Parent;	// n�vrat k rodi�i
				if (index == last) return -1;	// polo�ka nenalezena
				item = m_Data + index;	// adresa rodi�e
				index = item->Next;		// dal�� polo�ka za rodi�em
			}
		}
	}
}


////////////////////////////////////////////////////////////////////
// nalezen� polo�ky s ukazatelem na data (-1 = nen�)

int CBufProg::SrcDat(int bufID, int index)
{
// kontrola platnosti �daj�
	if ((bufID < 0) || (index < 0)) return -1;

// nalezen� polo�ky
	int i = m_Max - 1;
	PROGITEM* item = m_Data + i;
	for (; i >= 0; i--)
	{
		if ((m_Valid[i]) &&
			(item->DatIndex == index) &&
			(item->DatBlok == bufID))
		{
			return i;
		}
		item--;
	}
	return -1;
}


/////////////////////////////////////////////////////////////////////////////
// nalezen� referuj�c� polo�ky od aktu�ln� polo�ky cyklicky (-1 = nenalezeno)

int CBufProg::SrcRef(int RefBlok, int RefIndex, BOOL next)
{
// p��prava v�choz� polo�ky
	int index = Select();						// vybran� polo�ka
	PROGITEM* item;								// adresa polo�ky

// test prvn� polo�ky, nen�-li ��dn� polo�ka vybran� (jen p�i sm�ru vp�ed)
	if (IsNotValid(index))						// hled� se od za��tku?
	{		
		index = m_First;						// index prvn� polo�ky
		if (index < 0) return -1;				// nen� prvn� polo�ka

		if (next)								// hled� se sm�rem vp�ed?
		{
			item = m_Data + index;				// adresa polo�ky
			if ((item->RefBlok == RefBlok) &&	// souhlas� referen�n� buffer ?
				(item->RefIndex == RefIndex))	// souhlas� referen�n� index ?
			{
				return index;					// index nalezen� polo�ky
			}
		}
	}

// cyklus vyhled�n� polo�ky
	for (int i = m_Num; i > 0; i--)
	{

// index dal�� polo�ky (cyklicky, a� op�t po vybranou polo�ku)
		if (next)
		{
			index = NextItem(index);
		}
		else
		{
			index = PrevItem(index);
		}

// adresa polo�ky
		item = m_Data + index;

// test, zda to je hledan� polo�ka
		if ((item->RefBlok == RefBlok) &&		// souhlas� referen�n� buffer ?
			(item->RefIndex == RefIndex))		// souhlas� referen�n� index ?
		{
			return index;						// index nalezen� polo�ky
		}
	}

	return -1;									// polo�ka nenalezena
}


////////////////////////////////////////////////////////////////////
// nalezen� indexu textu k polo�ce (-1 = neplatn�)

int _fastcall CBufProg::GetText(const int index)
{
	if (IsNotValid(index)) return -1;

	PROGITEM* item = m_Data + index;

	while ((item->Name < 0) && ((DWORD)item->RefBlok < (DWORD)PROGBUFNUM))
	{
		item = &(BufProg[item->RefBlok][item->RefIndex]);
	}

	return item->Name;
}

////////////////////////////////////////////////////////////////////
// nalezen� indexu ikony k polo�ce (-1 = neplatn�)

int _fastcall CBufProg::GetIcon(const int index)
{
	if (IsNotValid(index)) return -1;

	PROGITEM* item = m_Data + index;

	while ((item->Icon < 0) && ((DWORD)item->RefBlok < (DWORD)PROGBUFNUM))
	{
		item = &(BufProg[item->RefBlok][item->RefIndex]);
	}

	return item->Icon;
}


////////////////////////////////////////////////////////////////////
// poskytnut� po�tu referenc� pro u�ivatele

int CBufProg::GetRefer(int index)
{
// kontrola platnosti polo�ky
	int ref = 0;
	if (IsValid(index))
	{

// adresa polo�ky
		PROGITEM* item = m_Data + index;

// po�et referenc� na polo�ku
		ref = item->Refer;

// z editoru p�esm�rov�n� na deklara�n� buffer
		if ((m_BufID == BufEdiID) && ((DWORD)item->RefBlok < (DWORD)PROGBUFNUM) && (item->RefBlok != BufEdiID))
		{
			ref = BufProg[item->RefBlok].GetRefer(item->RefIndex);
		}

// v lok�ln�m bufferu korekce vstupn�ch prom�nn�ch
		if ((m_BufID == BufLocID) && (ref > 0) && (item->DatBlok >= 0))
		{
			int inx2 = item->Parent;

			if (inx2 >= 0)
			{
				PROGITEM* item2 = m_Data + inx2;
				if (item2->Func == IDF_PAR)
				{
					while (item2->Parent >= 0)
					{
						inx2 = item2->Parent;
						item2 = m_Data + inx2;
					}

					inx2 = BufObj.SrcDat(BufLocID, inx2);
					if (BufObj.IsValid(inx2))
					{
						ref -= BufObj[inx2].Refer;
					}
					ASSERT(ref >= 0);
					if (ref < 0) ref = 0;
				}
			}
		}
	}
	return ref;
}


////////////////////////////////////////////////////////////////////
// test povolen� zru�en� v�tve z hlediska referenc� na polo�ky (TRUE=OK)

bool CBufProg::TestRefer(int index)
{
// kontrola, zda je polo�ka platn� a zda je to buffer objekt�
	if (IsNotValid(index) || ((m_BufID != BufObjID) && (m_BufID != BufLocID))) return true;

// �schova v�choz� polo�ky
	int first = index;						// �schova v�choz� polo�ky
	PROGITEM* item;

// cyklus p�es celou v�tev
	for (;;)
	{

// test, zda jsou na polo�ku n�jak� reference
		if (GetRefer(index) > 0)
		{

// potvrzen� operace
			CText text;
			CText nadpis;
			if (!text.Load(IDN_TESTREFER) ||
				!nadpis.Load(IDN_TESTREFER2)) return false;

			return (::MessageBox(MainFrame, text, nadpis,
				MB_OKCANCEL | MB_ICONEXCLAMATION) == IDOK);
		}

// adresa polo�ky
		item = m_Data + index;

// nalezen� n�sleduj�c� polo�ky
		if ((index == first) && (item->Child < 0)) return true;

		index = item->Child;			// vno�en� na potomka
		if (index < 0)					// nen� ��dn� potomek?
		{
			index = item->Next;			// pokra�ov�n� dal�� polo�kou

			while (index < 0)
			{
				index = item->Parent;	// n�vrat k rodi�i
				if (index == first) return true;
				ASSERT(index >= 0);
				item = m_Data + index;	// adresa rodi�e
				index = item->Next;		// dal�� polo�ka za rodi�em
			}
		}
	}
}


////////////////////////////////////////////////////////////////////
// p�ekreslen� ikony ve stromu

void CBufProg::ReDispIcon(int icon)
{
// kontrola platnosti ikony a je-li n�jak� strom
	if (Icon.IsNotValid(icon) || (m_Tree == NULL)) return;

// p��prava indexu polo�ky
	int index = m_Max-1;
	PROGITEM* item = m_Data + index;
	int icon2;
	TV_ITEM tvi;
	tvi.mask = TVIF_IMAGE;

// cyklus p�es v�echny polo�ky
	for (; index >= 0; index--)
	{

// kontrola, zda je polo�ka platn� a zda je zobrazena
		if (m_Valid[index] && (item->HTree != NULL))
		{

// nalezen� indexu ikony
			icon2 = GetIcon(index);

// kontrola, zda je to hledan� ikona
			if (icon2 == icon)
			{

// zaji�t�n� p�ekreslen� polo�ky
				tvi.hItem = item->HTree;
				tvi.iImage = 0;
				::SendMessage(m_Tree, TVM_SETITEM, 0, (LPARAM)&tvi);
				tvi.iImage = I_IMAGECALLBACK;
				::SendMessage(m_Tree, TVM_SETITEM, 0, (LPARAM)&tvi);
			}
		}

// posun adresy polo�ky
		item--;
	}
}


////////////////////////////////////////////////////////////////////
// test ��sla funkce, zda je to objekt (s jedine�n�m jm�nem)

bool _fastcall CBufProg::TestObj(int func)
{
	switch(func)
	{
	case IDF_FNC:
	case IDF_REAL:
	case IDF_BOOL:
	case IDF_TEXT:
	case IDF_ICON:
	case IDF_MAP:
	case IDF_PIC:
	case IDF_SND:
	case IDF_MUS:
	case IDF_SPRITE:
	case IDF_LIST:
	case IDF_LIST_SIZE:
	case IDF_LIST_INDEX:
	case IDF_LIST_AUTO:
	case IDF_SPRITE_X:
	case IDF_SPRITE_Y:
	case IDF_SPRITE_SMER:
	case IDF_SPRITE_MOVE:
	case IDF_SPRITE_VISIBLE:
	case IDF_SPRITE_FAZE:
	case IDF_SPRITE_STATUS:
	case IDF_RETURN_CODE:
	case IDF_COMMAND_LINE:
	case IDF_NONE:
		return true;
	}
	return false;
}


////////////////////////////////////////////////////////////////////
// nalezen� objektu dan�ho jm�na (-1 = nenalezeno) - rozli�uje velk�/mal� p�smena a mezery
//		name ..... hledan� text
//		over ..... p�esko�en� polo�ka (-1 = nen�)
//		first .... prvn� polo�ka (-1 = od za��tku)
//		next ..... p��t� polo�ka pro zastaven� (-1 = po konec)

int CBufProg::FindObj(const CMultiText& name, int over, int first, int next)
{
// pr�zdn� text - polo�ka nenalezena
	if (name.MultiText().IsEmpty()) return -1;

// korekce prvn� polo�ky
	if (IsNotValid(first))
	{
		first = m_First;
		if (IsNotValid(first)) return -1;
	}

// korekce p��t� polo�ky
	if (IsNotValid(next))
	{
		next = m_First;
	}

// cyklus p�es v�echny polo�ky
	CMultiText txt;
	do
	{

// adresa polo�ky
		PROGITEM* item = m_Data + first;

// test, zda je to polo�ka objektu
		if (TestObj(item->Func) &&
			(first != over))
		{
			CBufProg* buf = this;
			int inx = first;

// p�esm�rov�n� z editoru na objekty
			if ((item->RefBlok == BufObjID) ||
				(item->RefBlok == BufLocID))
			{
				inx = item->RefIndex;
				buf = BufProg + item->RefBlok;
				ASSERT(buf->IsValid(inx));
			}

// netestuje se ROOT lok�ln� funkce
			if ((m_BufID != BufLocID) || (item->Parent >= 0))
			{

// porovn�n� jm�na objektu (sta�� shodnost jednoho v kter�mkoliv z jazyk�)
				int itx = buf->GetText(inx);
				if (itx >= 0)
				{
					txt = Text.Get(itx);

					BOOL ok = false;

					for (int lan = 0; lan < JAZYKNUM; lan++)
					{
						if (name.MultiText(lan).IsNotEmpty() &&
							(txt.MultiText(lan) == name.MultiText(lan)))
						{
							ok = true;
							break;
						}
					}

					if (ok) return first;
				}
			}
		}

// dal�i polo�ka
		first = NextItem(first);
		ASSERT(first >= 0);

// a� po posledn� polo�ku
	} while (first != next);

// objekt nenalezen
	return -1;
}


////////////////////////////////////////////////////////////////////
// zru�en� v�tve stromu (p�i chyb� pam�ti vrac� FALSE - stav nen� navr�cen!)

bool CBufProg::Del(int index)
{
// kontrola platnosti indexu
	if (IsNotValid(index)) return true;

// lok�ln� prom�nn�
	int first = index;					// �schova indexu v�choz� polo�ky
	PROGITEM* item;						// adresa ru�en� polo�ky
	CBufProg* buf;						// adresa referuj�c�ho bufferu
	PROGITEM* item2;					// adresa referuj�c� polo�ky
	int i;								// pomocn� ��ta� (index referuj�c� polo�ky)
	int next = index;					// index p��t� polo�ky k ru�en�
	int parent;							// index rodi�e ru�en� polo�ky

// cyklus p�es v�echny polo�ky - za��n� od nejhlub��ch polo�ek
	do {

// p��prava adresy polo�ky
		index = next;					// index ru�en� polo�ky
		item = m_Data + index;			// adresa ru�en� polo�ky
		parent = item->Parent;			// index rodi�e ru�en� polo�ky

// pokud je n�jak� potomek, vno�� se nejd��ve do potomka
		if (item->Child >= 0)			// m� polo�ka potomky?
		{
			next = item->Child;			// p��t� se ru�� potomek polo�ky
			index = next;				// index aktu�ln� polo�ky (kv�li kontrole WHILE)
		}

// polo�ka je bez dal��ch potomk�, m��e se zru�it
		else
		{

// kontrola, jestli jsou na polo�ku n�jak� reference
			if (item->Refer > 0)
			{

// zru�en� referenc� z bufferu editoru programu
				ASSERT((m_BufID == BufObjID) || (m_BufID == BufLocID));

				buf = &BufEdi;			// adresa bufferu editoru programu
				i = buf->Max()-1;		// index posledn� polo�ky v bufferu
				item2 = buf->Data() + i; // adresa posledn� polo�ky

				for (; i >= 0; i--)		// cyklus p�es v�echny polo�ky bufferu
				{
					if (buf->m_Valid[i])	// je to platn� polo�ka?
					{
						if ((item2->RefIndex == index) && // je to hledan� reference?
							(item2->RefBlok == m_BufID))
						{
							if (!buf->Del(i)) return false;	// zru�en� referuj�c� polo�ky
						}
					}
					item2--;			// posun adresy referuj�c� polo�ky
				}

// pro ikonu zru�en� jej� pou�it� ve v�ech ploch�ch
				if ((item->Refer > 0) && (item->DatBlok == BufIcoID))
				{
					Map.DelIcon(item->DatIndex);
				}

				ASSERT(item->Refer == 0);	// te� ji� nesm� nic referovat!
			}

// zru�en� programov�ch dat
			if ((item->DatIndex >= 0) && ((DWORD)item->DatBlok < (DWORD)PROGBUFNUM))
			{
				if (!HistEdit::Del(item->DatBlok, item->DatIndex, -1, -1)) return false;

				switch (item->DatBlok)
				{
				case BufLocID:	if (!BufLoc.Del(item->DatIndex)) return false;
								break;

				case BufEdiID:	if (!BufEdi.Del(item->DatIndex)) return false;
								break;
				}
			}

// z�znam UNDO operace (data se p�i UNDO mus� navracet d��ve)
			if (m_Undo)
			{
				if (!Undo.AddProgDel(m_BufID, index, item))
				{
					Undo.DelAll();
					return false;
				}
			}

// zru�en� dat polo�ky
			if ((item->DatIndex >= 0) && ((DWORD)item->DatBlok >= (DWORD)PROGBUFNUM))
			{
				if (!HistEdit::Del(item->DatBlok, item->DatIndex, -1, -1))
				{
					Undo.DelAll();
					return false;
				}

				switch (item->DatBlok)
				{
				case BufNumID:	if (!Real.Del(item->DatIndex))
								{
									Undo.DelAll();
									return false;
								}
								break;

				case BufTxtID:	if (!Text.Del(item->DatIndex))
								{
									Undo.DelAll();
									return false;
								}
								break;

				case BufLogID:	if (!Bool.Del(item->DatIndex))
								{
									Undo.DelAll();
									return false;
								}
								break;

				case BufIcoID:	if (!Icon.Del(item->DatIndex))
								{
									Undo.DelAll();
									return false;
								}
								break;

				case BufMapID:	
								Map[item->DatIndex].RefDec();
								if (!Map.Del(item->DatIndex))
								{
									Undo.DelAll();
									return false;
								}
								break;

				case BufPicID:	if (!Picture.Del(item->DatIndex))
								{
									Undo.DelAll();
									return false;
								}
								break;

				case BufSprID:	
								if (!HistEdit::Del(BufPicID, -2, item->DatIndex, -1) ||
								    !Sprite.Del(item->DatIndex))
								{
									Undo.DelAll();
									return false;
								}
								break;

				case BufSndID:	if (!Sound.Del(item->DatIndex))
								{
									Undo.DelAll();
									return false;
								}
								break;

				case BufMusID:	if (!Music.Del(item->DatIndex))
								{
									Undo.DelAll();
									return false;
								}
								break;

				default:
					ASSERTERROR;
				}
			}

// zru�en� ikony polo�ky
			if ((item->Icon >= 0) && (item->DatBlok != BufIcoID))
			{
				if (!HistEdit::Del(BufIcoID, item->Icon, -1, -1))
				{
					Undo.DelAll();
					return false;
				}
				
				ASSERT(Icon.IsValid(item->Icon));
				ASSERT(item->Icon >= FncNum);
				if (!Icon.Del(item->Icon))
				{
					Undo.DelAll();
					return false;
				}
			}

// zru�en� textu polo�ky
			if (item->Name >= 0)
			{
				if (!HistEdit::Del(BufTxtID, item->Name, -1, -1))
				{
					Undo.DelAll();
					return false;
				}

				ASSERT(Text.IsValid(item->Name));
				if (!Text.Del(item->Name))
				{
					Undo.DelAll();
					return false;
				}
			}

// zru�en� zobrazen� polo�ky
			if (item->HTree != NULL)
			{
				::SendMessage(m_Tree, TVM_DELETEITEM, 0, (LPARAM)item->HTree);
			}

// �schova p��t� polo�ky
			next = item->Next;

// napojen� p�edch�zej�c� polo�ky na n�sleduj�c�
			if (next >= 0)
			{
				m_Data[next].Prev = item->Prev;
			}

// napojen� n�sleduj�c� polo�ky na p�edch�zej�ci
			if (item->Prev >= 0)
			{
				m_Data[item->Prev].Next = next;
			}

// oprava odkazu z rodi�e
			if ((parent >= 0) && (m_Data[parent].Child == index))
			{
				m_Data[parent].Child = next;
			}

// p�esm�rov�n� ukazatele na prvn� polo�ku
			if (index == m_First)
			{
				m_First = next;
			}

// p�esm�rov�n� rodi�e zobrazen�ch polo�ek
			if (index == m_Disp)
			{
				m_Disp = -2;
			}

// nen�-li dal�� polo�ka, poukra�uje se rodi�em
			if ((next < 0) && (index != first))
			{
				next = parent;
			}

// sn�en� ��ta�e referenc�
			if (((DWORD)item->RefBlok < (DWORD)PROGBUFNUM) && (m_BufID != BufClsID) &&
				BufProg[item->RefBlok].IsValid(item->RefIndex))
			{
				BufProg[item->RefBlok][item->RefIndex].Refer--;
			}

// zru�en� definovan� polo�ky
			if (index == m_Def) m_Def = -1;

// zru�en� polo�ky
			DelItem(index);
		}

// dokud nebyla zru�ena v�choz� polo�ka
	} while (index != first);

// korekce zobrazen� potomk� u rodi�e
	if (parent >= 0)
	{
		item = m_Data + parent;

		if ((item->Child < 0) && (item->HTree != NULL))
		{
			TV_ITEM tvi;
			tvi.hItem = item->HTree;

			if (m_BufID == BufClsID)
			{
				item->Param &= ~PR_EXP;
				tvi.mask = TVIF_STATE;
				tvi.stateMask = TVIS_EXPANDED;
				tvi.state = 0;
			}
			else
			{
				tvi.mask = TVIF_CHILDREN;
				tvi.cChildren = 0;
			}

			::SendMessage(m_Tree, TVM_SETITEM, 0, (LPARAM)&tvi);
			AktStateImage(parent);
		}
	}

	return true;
}


////////////////////////////////////////////////////////////////////
// p�esun v�tve stromu (vrac� nov� index polo�ky) 
// (parent: -1 = ROOT, after: -1 = za��tek, -2 = konec)

int CBufProg::Move(int parent, int after, int src)
{
// lok�ln� prom�nn�
	PROGITEM*	item;						// adresa p�en�en� polo�ky
	PROGITEM*	itemPar;					// adresa c�lov�ho rodi�e
	PROGITEM*	itemAft;					// adresa c�lov� p�edch�zej�c� polo�ky
	CBufProg*	bufedi = &BufEdi;			// edita�n� buffer
	int			inxedi;						// index polo�ky v edita�n�m bufferu
	PROGITEM*	itemedi;					// adresa polo�ky v edita�n�m bufferu
	int oldparent;							// �schovan� rodi� zdrojov� polo�ky
	int first;								// uschovan� v�choz� zdrojov� polo�ka

// kontrola platnosti zdrojov�ho indexu
	ASSERT(IsValid(src));
	if (IsNotValid(src)) return src;		// nic se nem�n�
	item = m_Data + src;					// adresa polo�ky
	oldparent = item->Parent;				// p�vodn� rodi� zdrojov� polo�ky


// -------------- korekce ukazatel�

// kontrola platnosti nov�ho rodi�e
	if (IsNotValid(parent))					// je platn� rodi�?
	{
		parent = -1;						// nen� platn� - bude to ROOT polo�ka
	}

// adresa c�lov�ho rodi�e (i neplatn�)
	itemPar = m_Data + parent;				// adresa rodi�e (i kdy� je neplatn�)

// adresa c�lov� p�edch�zej�c� polo�ky (i neplatn�)
	itemAft = m_Data + after;				// adresa p�edch�zej�c� polo�ky (i kdy� je neplatn�)

// nen�-li po�adov�na prvn� polo�ka, bude se hledat napojen� na p�edch�zej�c� polo�ku
	if (after != -1)
	{

// korekce p�ede�l� c�lov� polo�ky, je-li v podv�tvi
		if (IsValid(after))					// je p�ede�l� polo�ka platn�?
		{
			while ((itemAft->Parent != parent) && (itemAft->Parent >= 0))
			{
				after = itemAft->Parent;
				itemAft = m_Data + after;			// adresa p�edch�zej�c� polo�ky
			}
		}

// ov��en�, zda je p�edch�zej�c� polo�ka platn� (mus� m�t stejn�ho rodi�e)
		if (IsNotValid(after) || (itemAft->Parent != parent))
		{	

// p��prava v�choz� p�ede�l� polo�ky - bu� prvn� potomek rodi�e nebo prvn� polo�ka ROOT
			if (parent >= 0)					// je rodi� platn�?
			{
				after = itemPar->Child;			// prvn� potomek rodi�e
			}
			else
			{
				after = m_First;				// jinak prvn� polo�ka v ROOT
			}

// nalezen� posledn� polo�ky
			if (after >= 0)						// je n�jak� polo�ka?
			{
				itemAft = m_Data + after;		// adresa polo�ky

				while (itemAft->Next >= 0)		// je platn� dal�� polo�ka?
				{
					after = itemAft->Next;		// posun na dal�� polo�ku
					itemAft = m_Data + after;	// adresa dal�� polo�ky
				}
			}
		}
	}

// kontrola, zda se v�bec n�co m�n�
	if ((oldparent == parent) && 
		((item->Prev == after) || (src == after)))
	{
		return src;
	}

// p��znak modifikace souboru
	SetModi();

// z�znam pro UNDO
	if (m_Undo)
	{
		if (!Undo.AddProgMove(m_BufID, src, oldparent, item->Prev))
		{
			Undo.DelAll();
		}
	}

// ---------------- odpojen� v�tve

// odpojen� od p�edch�zej�c� polo�ky
	if (item->Prev >= 0)
	{
		m_Data[item->Prev].Next = item->Next;
	}
	else
	{

// odpojen� od rodi�e
		if (oldparent >= 0)
		{
			m_Data[oldparent].Child = item->Next;

// aktualizace rodi�e, pokud to byl jeho posledn� potomek
			if ((item->Next < 0) && (m_Data[oldparent].HTree != NULL))
			{
				TV_ITEM tvi;
				tvi.hItem = m_Data[oldparent].HTree;
				tvi.mask = TVIF_CHILDREN;
				tvi.cChildren = 0;
				::SendMessage(m_Tree, TVM_SETITEM, 0, (LPARAM)&tvi);
			}
		}
	}

// oprava ukazatele na prvn� polo�ku
	if (m_First == src)
	{
		m_First = item->Next;
	}

// odpojen� od n�sleduj�c� polo�ky
	if (item->Next >= 0)
	{
		m_Data[item->Next].Prev = item->Prev;
	}

// zru�en� zobrazen� polo�ky
	if (item->HTree)
	{
		::SendMessage(m_Tree, TVM_DELETEITEM, 0, (LPARAM)item->HTree);
	}

// ----------------- odsun vstupn�ho parametru

// kontrola, zda je odsunut vstupn� parametr mezi lok�ln� prom�nn�
	if ((m_BufID == BufLocID) &&
		(oldparent >= 0) &&
		(m_Data[oldparent].Func == IDF_PAR) &&
		(parent != oldparent))
	{

// nalezen� parametru funkce v edita�n�m bufferu (odli�ovat od ROOT funkce v editoru)
		inxedi = bufedi->Max() - 1;			// index posledn� polo�ky
		itemedi = bufedi->Data() + inxedi;	// adresa posledn� polo�ky
		for (; inxedi >= 0; inxedi--)
		{

// je to parametr funkce?
			if (bufedi->m_Valid[inxedi] &&
				(itemedi->RefIndex == src) &&
				(itemedi->RefBlok == BufLocID) &&
				(itemedi->Parent >= 0) &&
				(bufedi->At(itemedi->Parent).Func == IDF_FNC) &&
				((bufedi->At(itemedi->Parent).DstMask & PR_ALL) == 0) &&
				(bufedi->At(itemedi->Parent).RefBlok == BufObjID))

// zru�en� parametru funkce
			{
				bufedi->Del(inxedi);
			}
			itemedi--;
		}
	}

// --------------- p�ipojen� v�tve

// nastaven� odkazu na nov�ho rodi�e
	item->Parent = parent;				// odkaz na rodi�e (pro ROOT bude = -1)

// napojen� polo�ky na p�edch�zej�c� polo�ku
	item->Prev = after;					// odkaz na p�edch�zej�c� polo�ku
	if (after >= 0)							// je p�edch�zej�c� polo�ka platn�?
	{
		item->Next = itemAft->Next;			// p�enesen� n�sleduj�c� polo�ky
		itemAft->Next = src;				// nav�z�n� odkazu na tuto polo�ku
	}
	else
	{
		if (parent >= 0)					// je rodi� platn�?
		{
			if ((itemPar->Child < 0) && itemPar->HTree)	// je to prvn� potomek rodi�e
			{
				TV_ITEM tvi;
				tvi.hItem = itemPar->HTree;
				tvi.mask = TVIF_CHILDREN;
				tvi.cChildren = 1;
				::SendMessage(m_Tree, TVM_SETITEM, 0, (LPARAM)&tvi);
			}

			item->Next = itemPar->Child;	// p�ed�azen� p�ed prvn�ho potomka
			itemPar->Child = src;			// polo�ka bude prvn�m potomkem rodi�e
		}
		else
		{
			item->Next = m_First;			// p�ed�azen� p�ed prvn� ROOT polo�ku
			m_First = src;					// polo�ka bude prvn� ROOT polo�kou
		}
	}

// napojen� polo�ky na n�sleduj�c� polo�ku
	if (item->Next >= 0)
	{
		m_Data[item->Next].Prev = src;
	}

// --------------- p��sun vstupn�ho parametru

// kontrola, zda je p�isunut vstupn� parametr z lok�ln�ch prom�nn�ch
	if ((m_BufID == BufLocID) &&
		(parent >= 0) &&
		(itemPar->Func == IDF_PAR) &&
		(parent != oldparent))
	{

// p��prava indexu glob�ln� funkce
		int reffnc = BufObj.SrcDat(BufLocID, itemPar->Parent);
		if (reffnc >= 0)
		{

// nalezen� funkce v edita�n�m bufferu
			inxedi = bufedi->Max() - 1;			// index posledn� polo�ky
			itemedi = bufedi->Data() + inxedi;	// adresa posledn� polo�ky
			for (; inxedi >= 0; inxedi--)
			{

// je to pou�it� funkce?
				if (bufedi->m_Valid[inxedi] &&
					(itemedi->RefIndex == reffnc) &&
					(itemedi->RefBlok == BufObjID) &&
					(itemedi->Func == IDF_FNC) &&
					((itemedi->DstMask & PR_ALL) == 0))
				{

// kopie parametru funkce
					int ii = bufedi->Copy(inxedi, -2, BufLocID, src);
					ASSERT(ii >= 0);

// z�kaz p�esunu parametru
					if (ii >= 0)
					{
						bufedi->At(ii).Param |= PR_NOMOVE;
					}
				}
				itemedi--;
			}
		}
	}

// --------------- zobrazen� v�tve na nov� pozici

// zobrazen� p�enesen�ch polo�ek, jsou-li v zobrazen� v�tvi
	if ((((parent >= 0) && (m_Data[parent].HTree != NULL)) || (m_Disp == parent)))
	{
		first = src;							// v�choz� polo�ka
		do {

// zobrazen� polo�ky
			DispNewItem(src);

// adresa polo�ky
			item = m_Data + src;

// nalezen� n�sleduj�c� polo�ky
			if ((src != first) || (item->Child >= 0))
			{
				src = item->Child;			// vno�en� na potomka
			}

			if (src < 0)
			{
				src = item->Next;			// pokra�ov�n� dal�� polo�kou

				while (src < 0)
				{
					src = item->Parent;	// n�vrat k rodi�i
					ASSERT(src >= 0);
					if (src == first) break;
					item = m_Data + src;	// adresa rodi�e
					src = item->Next;		// dal�� polo�ka za rodi�em
				}
			}

// dokud je dal�� platn� polo�ka
		} while (src != first);
	}

// aktualizace stavov�ch ikon rodi��
	AktStateImage(parent);
	if (parent != oldparent)
	{
		AktStateImage(oldparent);
	}

	return src;
}


////////////////////////////////////////////////////////////////////
// kopie v�tve stromu - vrac� c�lov� index polo�ky
// (parent: -1 = ROOT, pos: -1 = za��tek, -2 = konec)

int CBufProg::Copy(int parent, int pos, int bufID, int src)
{
// lok�ln� prom�nn�
	ASSERT((DWORD)bufID < (DWORD)PROGBUFNUM);
	CBufProg* buf = BufProg + bufID;			// zdrojov� buffer
	PROGITEM item;								// pracovn� buffer polo�ky
	PROGITEM* itemsrc;							// adresa zdrojov� polo�ky
	PROGITEM* itemdst;							// adresa c�lov� polo�ky
	PROGITEM*	itemPar;						// adresa c�lov�ho rodi�e
	PROGITEM*	itemAft;						// adresa c�lov� p�edch�zej�c� polo�ky
	int dst;									// index c�lov� polo�ky
	int first = src;							// �schova v�choz� zdrojov� polo�ky
	int result;									// v�sledek
	int newparent;								// �schova c�lov�ho rodi�e

// kontrola zdrojov� polo�ky
	ASSERT(buf->IsValid(src));
	if(buf->IsNotValid(src)) return -1;

// zapnut� �ekac�ho kurzoru
	BeginWaitCursor();

// -------------- korekce ukazatel�

// kontrola platnosti nov�ho rodi�e
	if (IsNotValid(parent))						// je platn� rodi�?
	{
		parent = -1;							// nen� platn� - bude to ROOT polo�ka
	}
	result = parent;							// v�sledek
	newparent = parent;							// �schova c�lov�ho rodi�e

// adresa c�lov�ho rodi�e (i neplatn�)
	itemPar = m_Data + parent;				// adresa rodi�e (i kdy� je neplatn�)

// adresa c�lov� p�edch�zej�c� polo�ky (i neplatn�)
	itemAft = m_Data + pos;					// adresa p�edch�zej�c� polo�ky (i kdy� je neplatn�)

// nen�-li po�adov�na prvn� polo�ka, bude se hledat napojen� na p�edch�zej�c� polo�ku
	if (pos != -1)
	{

// korekce p�ede�l� c�lov� polo�ky, je-li v podv�tvi
		if (IsValid(pos))					// je p�ede�l� polo�ka platn�?
		{
			while ((itemAft->Parent != parent) && (itemAft->Parent >= 0))
			{
				pos = itemAft->Parent;
				itemAft = m_Data + pos;			// adresa p�edch�zej�c� polo�ky
			}
		}

// ov��en�, zda je p�edch�zej�c� polo�ka platn� (mus� m�t stejn�ho rodi�e)
		if (IsNotValid(pos) || (itemAft->Parent != parent))
		{	

// p��prava v�choz� p�ede�l� polo�ky - bu� prvn� potomek rodi�e nebo prvn� polo�ka ROOT
			if (parent >= 0)					// je rodi� platn�?
			{
				pos = itemPar->Child;			// prvn� potomek rodi�e
			}
			else
			{
				pos = m_First;				// jinak prvn� polo�ka v ROOT
			}

// nalezen� posledn� polo�ky
			if (pos >= 0)						// je n�jak� polo�ka?
			{
				itemAft = m_Data + pos;		// adresa polo�ky

				while (itemAft->Next >= 0)		// je platn� dal�� polo�ka?
				{
					pos = itemAft->Next;		// posun na dal�� polo�ku
					itemAft = m_Data + pos;	// adresa dal�� polo�ky
				}
			}
		}
	}

// ------------ p��prava parametr� kop�rovan� polo�ky

// cyklus p�es v�echny polo�ky zdrojov� v�tve
	do {

// p�i kopii skupiny z okna t��d zaji�t�n� na�ten� obsahu skupiny
		if ((bufID == BufClsID) && 
			(buf->At(src).Func == IDF_GROUP) &&
			(buf->At(src).Child < 0))
		{
			ProgLib::Load(src);
		}

// kopie obsahu polo�ky do pracovn�ho bufferu
		MemCopy(&item, &(buf->At(src)), SIZEOFPROGITEM);	// kopie polo�ky

// test, zda se maj� kop�rovat potomci
		if (	(	(bufID == BufStrID) &&				// kopie z bufferu intern�ch struktur
					(	(item.Func == IDF_NUM)// ||		// ��slo se kop�ruje bez obsahu
//						(item.Func == IDF_WHILE_BODY)	// cyklus se kop�ruje bez p�eru�en�
					)
				) ||
				((bufID == BufClsID) && (item.Func != IDF_GROUP)) || // t��dy se kop�ruj� bez potomk�
				((bufID == BufLocID) && ((m_BufID == BufEdiID) || (m_BufID == BufClsID))) ||	// z lok�ln�ch objekt� v�e bez potomk�
				((bufID == BufObjID) && ((m_BufID == BufEdiID) || (m_BufID == BufClsID)))	// z glob�ln�ch objekt� v�e bez potomk�
			)
		{
			item.Child = -1;							// zru�en� potomk�
		}
	
// zm�na reference na zdrojov� okno, je-li kopie mezi okny
		if (bufID != m_BufID)
		{
			item.RefBlok = bufID;						// blok s deklarac�
			item.RefIndex = src;						// polo�ka s deklarac�
			item.DatBlok = -1;							// nejsou data
			item.DatIndex = -1;							// nejsou data

// oprava reference p�i p�eta�en� z okna t��d
			if (bufID == BufClsID)
			{
				if (item.Func == IDF_GROUP)
				{
					item.RefBlok = BufStrID;
					item.RefIndex = GroupIndex;
				}
				else
				{
					while (BufCls[item.RefIndex].Parent >= 0)
					{
						item.RefIndex = BufCls[item.RefIndex].Parent;
					}
				}
			}
			else

// implicitn� jm�no a ikona (ne p�i kopii z okna t��d)
			{
				if ((m_BufID == BufClsID) && (item.Func != IDF_GROUP))
				{
					if (item.Name >= 0)
					{
						item.Name = Text.Dup(buf->GetText(src));
					}

					if ((item.Icon >= 0) && (item.Func != IDF_ICON))
					{
						item.Icon = Icon.Dup(item.Icon);
					}
					else
					{
						item.Icon = -1;
					}

					item.RefBlok = BufIntID;
					item.RefIndex = item.Func - IDF;
				}
				else
				{
					item.Name = -1;								// implicitn� jm�no
					item.Icon = -1;								// implicitn� ikona
				}
			}
		}

// p�i kopii z okna struktur rozvinut� polo�ky
		if ((bufID == BufStrID) && (item.Child >= 0))
		{
			item.Param |= PR_EXP;
		}

// p�i kopii z okna t��d zru�en� z�mku a vypnut�
		if (bufID == BufClsID)
		{
			item.Param &= ~(PR_LOCK | PR_OFF);

			if (m_BufID != BufClsID)
			{
				item.Param &= ~PR_ALTEXT;
			}
		}

// u prvn� polo�ky zru�en� z�mku
		if (src == first)
		{
			item.Param &= ~PR_LOCK;
		}

// zru�en� intern�ho p��znaku a p��znaku nov� polo�ky a p��znaku alternativn� knihovny
		item.Param &= ~(PR_INTERN | PR_NEW | PR_CD | PR_CD2);

// korekce parametr� indexu seznamu a sprajtu
		if ((item.Func == IDF_LIST_INDEX) ||
			(item.Func == IDF_LIST_AUTO) ||
			(item.Func == IDF_LIST_SIZE) ||
			(item.Func == IDF_SPRITE_X) ||
			(item.Func == IDF_SPRITE_Y) ||
			(item.Func == IDF_SPRITE_SMER) ||
			(item.Func == IDF_SPRITE_MOVE) ||
			(item.Func == IDF_SPRITE_VISIBLE) ||
			(item.Func == IDF_SPRITE_FAZE) ||
			(item.Func == IDF_SPRITE_STATUS))
		{
			item.Param &= ~PR_NOMOVE;

			if (m_BufID != BufEdiID)
			{
				item.Param |= PR_INTERN;
			}
		}

// z�kladn� ikona se m�n� na b�nou ikonu
		if (item.Func == IDF_NONE)
		{
			item.Func = IDF_ICON;
		}

// ----------- duplikace dat polo�ky

// zru�en� textu textov� a ��seln� konstanty a koment��e p�i kopii z okna struktur
		if (((item.Func == IDF_TEXT_CONST) || 
			(item.Func == IDF_NUM) || 
			(item.Func == IDF_COMMENT)) && 
			(bufID == BufStrID))
		{
			item.Name = Text.New();
		}
		else

// duplikace textu, je-li platn� (ne pro textovou konstantu)
		{
			if (item.Name >= 0)
			{
				item.Name = Text.Dup(item.Name);
			}
		}

// duplikace dat obr�zku p�i kopii uvnit�/do okna objekt�
		if ((item.Func == IDF_ICON) && ((m_BufID == BufObjID) || (m_BufID == BufLocID)))
		{
			if ((bufID == BufClsID) && (item.Icon >= 0))
			{
				item.DatIndex = item.Icon;
			}

			item.DatBlok = BufIcoID;			// data v bufferu obr�zk�
			if (item.DatIndex < 0)				// je n�jak� obr�zek p�id�len?
			{
				item.DatIndex = 0;				// implicitn� ikona 0 (podklad)
				item.Param |= PR_NEW;			// p��znak nov� polo�ky
			}
			item.DatIndex = Icon.Dup(item.DatIndex); // duplikace obr�zku
			item.Icon = item.DatIndex;			// bude to sou�asn� ikona
		}

// duplikace ikony barevn� konstanty
		if (item.Func == IDF_COLOR)
		{
			item.Icon = Icon.Dup(buf->GetIcon(src));
		}
		else

// duplikace ikony p�i kopii uvnit� okna (ne pro barevnou konstantu ani pro p�edm�t)
		{
			if ((item.Icon >= 0) && ((item.Func != IDF_ICON) || (m_BufID == BufClsID)))
			{
				item.Icon = Icon.Dup(item.Icon);
			}
		}

// zaji�t�n� dat p�i kopii z knihovny t��d (ne pro ikonu - je ji� na�tena)
		if ((bufID == BufClsID) && 
			(item.Func != IDF_GROUP) &&
			(item.Func != IDF_ICON) &&
			(item.Name >= 0) &&
			(item.DatIndex < 0) &&
			(BufCls[src].Parent >= 0))
		{
			ProgLib::InitPath(BufCls[src].Parent);
			CText name;

			name = ProgLib::m_Path + (Text.Get(item.Name)).MultiText(JAZYK000);

			item.DatBlok = -1;

			switch(ProgLib::m_Func)
			{
			case IDF_FNC:
				break;

			case IDF_REAL:
				{
					double num;
					if (Real.LoadFile(num, name))
					{
						item.DatBlok = BufNumID;
						item.DatIndex = Real.Add(num);
					}
				}
				break;

			case IDF_TEXT:
				{
					CText txt;
					if (txt.LoadFile(name))
					{
						item.DatBlok = BufTxtID;
						item.DatIndex = Text.AddTxt(txt);
					}
				}
				break;

			case IDF_BOOL:
				{
					BOOL log;
					if (Bool.LoadFile(log, name))
					{
						item.DatBlok = BufLogID;
						item.DatIndex = Bool.Add(log);
					}
				}
				break;

			case IDF_MAP:
				{
					CMap map;
					if (map.LoadFile(name))
					{
						item.DatBlok = BufMapID;
						item.DatIndex = Map.Add(map);
						Map[item.DatIndex].RefInc();
					}
				}
				break;

			case IDF_PIC:
				{
					CPicture pic;
					if (pic.LoadFile(name, TRUE))
					{
						item.DatBlok = BufPicID;
						item.DatIndex = Picture.Add(pic);
					}
				}
				break;

			case IDF_SPRITE:
				{
					CSprite sprite;
					if (sprite.LoadFile(name))
					{
						item.DatBlok = BufSprID;
						item.DatIndex = Sprite.Add(sprite);
					}
				}
				break;

			case IDF_SND:
				{
					CSound sound;
					if (sound.LoadFile(name))
					{
						item.DatBlok = BufSndID;
						item.DatIndex = Sound.Add(sound);
					}
				}
				break;

			case IDF_MUS:
				{
					CMusic music;
					if (music.LoadFile(name))
					{
						item.DatBlok = BufMusID;
						item.DatIndex = Music.Add(music);
					}
				}
				break;
			}
		}
		else

// duplikace dat (pokud jsou) - v�etn� obsahu funkce
		{
			CopyDupData(&item);
		}

// jm�no souboru p�i p�eta�en� z okna t��d se m��e zru�it
		if ((item.Name >= 0) && (m_BufID != BufClsID))
		{
			Text[item.Name].MultiText(JAZYK000, EmptyText);
		}

// zaji�t�n� nov�ch dat pro polo�ku - v�etn� obsahu funkce
		CopyNewData(&item);

// -------------- zaji�t�n� jedine�nosti jm�na

		if (((m_BufID == BufObjID) || (m_BufID == BufLocID)) &&
			TestObj(item.Func))
		{
			CMultiText txt;

			if (item.Name >= 0)
			{
				txt = Text.Get(item.Name);
			}
			else
			{
				if (item.RefBlok >= 0)
				{
					CText txt0;
					int texti = BufProg[item.RefBlok].GetText(item.RefIndex);
					txt0 = Text.GetTxt(texti);
					txt.MultiText(txt0);

					// implicitn� jm�no parametr� sprajtu (p�i kopii z okna t��d)
					switch (item.Func)
					{
					case IDF_SPRITE_X:
					case IDF_SPRITE_Y:
					case IDF_SPRITE_SMER:
					case IDF_SPRITE_VISIBLE:
					case IDF_SPRITE_MOVE:
					case IDF_SPRITE_FAZE:
					case IDF_SPRITE_STATUS:
						if (IsValid(parent))
						{
							int pari = GetText(parent);
							if ((pari >= 0) && (texti >= 0))
							{
								for (int lan = 0; lan < JAZYKNUM; lan++)
								{
									txt0 = Text[pari].MultiText(lan);
									if (txt0.IsNotEmpty())
									{
										txt.MultiText(lan, txt0 + _T(' ') + Text[texti].MultiText(lan));
									}
								}
								item.Name = Text.Add(txt);
							}
						}
					}
				}
			}

			int nn = 1;
			int reffrst = -1;
			int refnext = -1;

			if (m_BufID == BufLocID)
			{
				reffrst = parent;

				if (IsValid(reffrst))
				{
					while (At(reffrst).Parent >= 0)
					{
						reffrst = At(reffrst).Parent;
					}
					refnext = At(reffrst).Next;
				}

			}

			while ((BufLoc.FindObj(txt, -1, reffrst, refnext) >= 0) || 
				   (BufObj.FindObj(txt, -1, -1, -1) >= 0))
			{
				nn++;
				txt.SetNumObj(nn);
			}

			if (nn > 1)
			{
				if (item.Name < 0)
				{
					item.Name = Text.Add(txt);
				}
				else
				{
					Text.Set(item.Name, txt);
				}
			}
		}

// -------------- ulo�en� polo�ky

// kopie polo�ky do c�lov�ho bufferu (pozor - Insert m��e pohnout zdrojovou i c�lovou adresou)
		dst = Insert(&item, parent, pos);
		if (src == first) result = dst;			// v�sledn� index
		pos = -2;								// p��t� ji� budou polo�ky na konec

// -------------- vytvo�en� parametr� sprajtu

// p�i kopii sprajtu z okna t��d p�id�n� parametr�
		if ((item.Func == IDF_SPRITE) && (item.Child == -1) && 
			((m_BufID == BufObjID) || (m_BufID == BufLocID)))
		{
			Copy(dst, -2, BufIntID, IDF_SPRITE_X - IDF);
			Copy(dst, -2, BufIntID, IDF_SPRITE_Y - IDF);
			Copy(dst, -2, BufIntID, IDF_SPRITE_SMER - IDF);
			Copy(dst, -2, BufIntID, IDF_SPRITE_VISIBLE - IDF);
			Copy(dst, -2, BufIntID, IDF_SPRITE_MOVE - IDF);
			Copy(dst, -2, BufIntID, IDF_SPRITE_FAZE - IDF);
			Copy(dst, -2, BufIntID, IDF_SPRITE_STATUS - IDF);
		}

// -------------- p�i kopii glob�ln� funkce kopie t� vstupn�ch parametr�

// kontrola, zda se kop�ruje glob�ln� funkce z objekt� do editoru
		if ((item.Func == IDF_FNC) &&
			(m_BufID == BufEdiID) && 
			(bufID == BufObjID) &&
			(BufObj[src].DatBlok == BufLocID) &&
			(BufObj[src].DatIndex >= 0))
		{

// nalezen� definice vstupn�ch prom�nn�ch
			int inx = BufObj[src].DatIndex;
			PROGITEM* itm;
			do 
			{
				inx = BufLoc.NextItem(inx);
				itm = &BufLoc[inx];
			} while ((itm->Func != IDF_PAR) && (itm->Parent >= 0));
			inx = itm->Child;

// kopie vstupn�ch prom�nn�ch
			if (itm->Func == IDF_PAR)
			{
				while (inx >= 0)
				{
					int ii = BufEdi.Copy(dst, -2, BufLocID, inx);
					if (BufEdi.IsValid(ii)) BufEdi[ii].Param |= PR_NOMOVE;
					itm = &BufLoc[inx];
					inx = itm->Next;
				}
			}
		}

// --------------- vytvo�en� vstupn�ho parametru (pozor na kopii cel� lok�ln� definice)

// kontrola, zda je vytvo�en vstupn� parametr 
		if ((m_BufID == BufLocID) &&
			(parent >= 0) &&
			(m_Data[parent].Func == IDF_PAR))
		{

// p��prava indexu glob�ln� funkce
			int reffnc = BufObj.SrcDat(BufLocID, m_Data[parent].Parent);
			if (reffnc >= 0)
			{

// nalezen� funkce v edita�n�m bufferu
				CBufProg* bufedi = &BufEdi;
				int inxedi = bufedi->Max() - 1;					// index posledn� polo�ky
				PROGITEM* itemedi = bufedi->Data() + inxedi;	// adresa posledn� polo�ky
				for (; inxedi >= 0; inxedi--)
				{

// je to pou�it� funkce?
					if (bufedi->m_Valid[inxedi] &&
						(itemedi->RefIndex == reffnc) &&
						(itemedi->RefBlok == BufObjID) &&
						(itemedi->Func == IDF_FNC) &&
						((itemedi->DstMask & PR_ALL) == 0))
					{

// kopie parametru funkce
						int ii = bufedi->Copy(inxedi, -2, BufLocID, dst);
						ASSERT(ii >= 0);

// z�kaz p�esunu parametru
						if (ii >= 0)
						{
							bufedi->At(ii).Param |= PR_NOMOVE;
						}
					}
					itemedi--;
				}
			}
		}

// ---------- kopie definice programu

// p�i kopii funkce kopie definice programu
		if (((item.DatBlok == BufLocID) || (item.DatBlok == BufEdiID)) &&
			(m_BufID == bufID) && (item.DatIndex == m_Data[dst].DatIndex))
		{
			int srcroot = item.DatIndex;
			ASSERT(srcroot >= 0);
			int dstroot = BufProg[item.DatBlok].Copy(-1, -2, item.DatBlok, srcroot);
			m_Data[dst].DatIndex = dstroot;
			ASSERT(dstroot >= 0);

// p�i kopii glob�ln� funkce korekce referenc� na lok�ln� prom�nn�
			if ((item.DatBlok == BufLocID) && (m_BufID == BufObjID))
			{

// p��prava ukazatel� pro vyhled�v�n� definic funkc�
				CBufProg* bufloc = &BufLoc;		// adresa bufferu lok�ln�ch objekt�
				int locinx = dstroot;			// index v lok�ln� definici p�i vyhled�v�n� funkc�
				PROGITEM* locitem;				// polo�ka v lok�ln� definici p�i vyhled�v�n�
				locitem = &bufloc->At(locinx);	// adresa polo�ky

// p��prava ukazatel� pro vyhled�v�n� lok�ln�ch prom�nn�ch v editoru
				CBufProg* bufedi = &BufEdi;		// adresa bufferu editoru
				int ediinx;						// index v bufferu editoru
				PROGITEM* ediitem;				// adresa v bufferu editoru

// p��prava ukazatel� pro reindexaci odkazu na lok�ln� prom�nnou
				int refinx;						// referen�n� index hledan� polo�ky
				int num;						// relativn� pozice polo�ky v lok�ln� definici
				int inx;						// ukazatel indexu v lok�ln� definici
				PROGITEM* itm;					// polo�ka v lok�ln� definici

// cyklus vyhled�v�n� definic funkc� v c�lov� lok�ln� definici (pozor - obslou�it i root)
				do 
				{

// test, zda je to definice funkce
					ediinx = locitem->DatIndex;
					if ((locitem->DatBlok == BufEdiID) && 
						(bufedi->IsValid(ediinx)))
					{
						ediitem = &bufedi->At(ediinx);		// adresa polo�ky

// cyklus vyhled�v�n� lok�ln�ch prom�nn�ch v editoru
						do 
						{

// test, zda je to odkaz na lok�ln� polo�ku a zda to nen� parametr funkce
							refinx = ediitem->RefIndex;
							if ((ediitem->RefBlok == BufLocID) &&
								bufloc->IsValid(refinx) &&
								(ediitem->Parent >= 0) &&
								(((bufedi->At(ediitem->Parent).DstMask & PR_ALL) != 0) || (bufedi->At(ediitem->Parent).Func != IDF_FNC)))
							{

// zji�t�n� relativn� pozice polo�ky ve zdrojov� lok�ln� definici
								num = 0;
								inx = srcroot;
								itm = &bufloc->At(inx);

								while (inx != refinx)
								{
									num++;
									inx = bufloc->NextItem(inx);
									itm = &bufloc->At(inx);
									if (itm->Parent < 0) break;
								}
				
// kontrola nalezen� polo�ky
								if (itm->Parent >= 0)
								{

// nalezen� odpov�daj�c� zkop�rovan� polo�ky
									inx = dstroot;
									for (; num > 0; num--)
									{
										inx = bufloc->NextItem(inx);
									}

// p�esm�rov�n� ukazatele
									itm->Refer--;
									ediitem->RefIndex = inx;
									bufloc->At(inx).Refer++;
								}
							}

// posun ukazatele v editoru na dal�� polo�ku
							ediinx = bufedi->NextItem(ediinx);	// index dal�� polo�ky
							ASSERT(ediinx >= 0);
							ediitem = &bufedi->At(ediinx);		// adresa polo�ky
						} while (ediitem->Parent >= 0);			// dokud nen� dal�� definice
					}

// posun ukazatele v lok�ln� definici na dal�� polo�ku
					locinx = bufloc->NextItem(locinx);	// index dal�� polo�ky
					ASSERT(locinx >= 0);
					locitem = &bufloc->At(locinx);		// adresa polo�ky
				} while (locitem->Parent >= 0);			// dokud nen� dal�� definice
			}
		}

// ---------- dal�� polo�ka

// p��prava indexu dal�� polo�ky
		itemdst = &At(dst);
		itemsrc = &buf->At(src);
		if ((src != first) || (item.Child >= 0))
		{
			src = item.Child;				// vno�en� na potomka
			if (src < 0)					// nen� ��dn� potomek?
			{
				src = itemsrc->Next;		// pokra�ov�n� dal�� polo�kou

				while ((src < 0) && (itemsrc->Parent >= 0))
				{
					src = itemsrc->Parent;	// n�vrat zdroje k rodi�i
					dst = parent;			// n�vrat c�le k rodi�i
					itemdst = &At(dst);		// adresa c�lov� polo�ky
					parent = itemdst->Parent; // nov� rodi�
					if (src == first) break; // je cel� v�tev
					itemsrc = &buf->At(src); // adresa rodi�e
					src = itemsrc->Next;	// dal�� polo�ka za rodi�em
				}
			}
			else
			{
				parent = dst;				// jinak tato polo�ka bude rodi�em
			}

// p�i kopii z okna t��d p�esko�en� skupin
			if (bufID == BufClsID)
			{
				while ((src >= 0) && (src != first) && (buf->At(src).Func == IDF_GROUP))
				{
					src = buf->At(src).Next;
				}

				if (src < 0) src = first;
			}
		}

// a� po v�choz� polo�ku
	} while (src != first);

// aktualizace stavov� ikony rodi�e
	AktStateImage(newparent);

// vypnut� �ekac�ho kurzoru
	EndWaitCursor();

	return result;
}


////////////////////////////////////////////////////////////////////
// duplikace dat u kop�rovan� polo�ky (ne pro ikonu a ne pro funkci)

void CBufProg::CopyDupData(PROGITEM* item)
{
// duplikace dat
	switch (item->DatBlok)
	{
	case BufTxtID:	item->DatIndex = Text.Dup(item->DatIndex);
					break;

	case BufNumID:	item->DatIndex = Real.Dup(item->DatIndex);
					break;

	case BufLogID:	item->DatIndex = Bool.Dup(item->DatIndex);
					break;

	case BufMapID:	item->DatIndex = Map.Dup(item->DatIndex);
					Map[item->DatIndex].RefInc();
					break;

	case BufPicID:	item->DatIndex = Picture.Dup(item->DatIndex);
					break;

	case BufSprID:	item->DatIndex = Sprite.Dup(item->DatIndex);
					break;

	case BufSndID:	item->DatIndex = Sound.Dup(item->DatIndex);
					break;

	case BufMusID:	item->DatIndex = Music.Dup(item->DatIndex);
					break;
	}
}


////////////////////////////////////////////////////////////////////
// zaji�t�n� nov�ch dat u kop�rovan� polo�ky (je vol�no i z Clipboard::Paste)

void CBufProg::CopyNewData(PROGITEM* item)
{
// test, zda data nejsou vytvo�ena a zda je to buffer objekt�
	if (((item->DatBlok >= 0) && (item->DatIndex >= 0)) ||
		((m_BufID != BufObjID) && (m_BufID != BufLocID)))
		return;

// vytvo�en� nov�ch dat pro prom�nn�
	switch (item->Func)
	{

// ��seln� prom�nn�
	case IDF_LIST_AUTO:
	case IDF_LIST_INDEX:
	case IDF_SPRITE_X:
	case IDF_SPRITE_Y:
	case IDF_SPRITE_SMER:
	case IDF_SPRITE_FAZE:
	case IDF_SPRITE_STATUS:
	case IDF_REAL:		item->DatBlok = BufNumID;
						item->DatIndex = Real.New();
						item->Param |= PR_NEW;
						break;

// textov� prom�nn�
	case IDF_TEXT:		item->DatBlok = BufTxtID;
						item->DatIndex = Text.New();
						item->Param |= PR_NEW;
						break;

// logick� prom�nn�
	case IDF_SPRITE_VISIBLE:
	case IDF_SPRITE_MOVE:
	case IDF_BOOL:		item->DatBlok = BufLogID;
						item->DatIndex = Bool.New();
						item->Param |= PR_NEW;
						break;

// prom�nn� plochy
	case IDF_MAP:		item->DatBlok = BufMapID;
						ASSERT(Map.IsValid(0));
						item->DatIndex = Map.New(Map[0].Width(), Map[0].Height());
						Map[item->DatIndex].RefInc();
						item->Param |= PR_NEW;
						break;

// prom�nn� obr�zku
	case IDF_PIC:		item->DatBlok = BufPicID;
						ASSERT(Map.IsValid(0));
						item->DatIndex = Picture.New(Map[0].Width()*ICONWIDTH, Map[0].Height()*ICONHEIGHT);
						item->Param |= PR_NEW;
						break;

// prom�nn� sprajtu
	case IDF_SPRITE:	item->DatBlok = BufSprID;
						item->DatIndex = Sprite.New();
						item->Param |= PR_NEW;
						break;

// prom�nn� zvuku
	case IDF_SND:		item->DatBlok = BufSndID;
						item->DatIndex = Sound.New();
						item->Param |= PR_NEW;
						break;

// prom�nn� hudby
	case IDF_MUS:		item->DatBlok = BufMusID;
						item->DatIndex = Music.New();
						item->Param |= PR_NEW;
						break;

// funkce
	case IDF_FNC:
		{
			item->Param |= PR_NEW;

// vytvo�en� definice v edita�n�m bufferu
			PROGITEM item2;								// pracovn� polo�ka
			MemFill(&item2, SIZEOFPROGITEM, -1);		// implicitn� hodnoty polo�ek
			item2.Func = IDF_FNC;						// identifikace funkce
			item2.SrcMask = 0;							// maska zdrojov�ch vlastnost�
			item2.DstMask = PR_COMMAND;					// maska c�lov�ch vlastnost�
			item2.Param = PR_EXP | PR_NOMOVE;			// parametry
			item2.RefBlok = BufIntID;					// reference na intern� buffer
			item2.RefIndex = IDF_FNC - IDF;				// referen�n� index
			item2.DatIndex = BufEdi.Insert(&item2, -1, -2);	// vlo�en� polo�ky do bufferu

// inicializace odkazu na definici z lok�ln�ho bufferu
			if (m_BufID == BufLocID)
			{
				item->DatBlok = BufEdiID;				// index bufferu
				item->DatIndex = item2.DatIndex;		// index polo�ky definice
			}
			else

// vytvo�en� deklarace v lok�ln�m bufferu (je-li kopie do glob�ln�ho bufferu)
			{
				item2.DstMask = PR_ALLDATA;				// maska c�lov�ch vlastnost�
				item2.DatBlok = BufEdiID;				// odkaz na edita�n� buffer
				item->DatIndex = BufLoc.Insert(&item2, -1, -2); // vlo�en� polo�ky do bufferu
				item->DatBlok = BufLocID;				// lok�ln� buffer

// vlo�en� z�hlav� pro vstupn� parametry
				item2.Func = IDF_PAR;					// vstupn� parametry
				item2.DstMask = PR_ALLDATA;				// maska c�lov�ch parametr�
				item2.Param = PR_NOMOVE;				// parametry
				item2.DatBlok = -1;						// nen� blok s daty
				item2.DatIndex = -1;					// nejsou data
				item2.RefIndex = IDF_PAR - IDF;			// index v intern�m bufferu
				BufLoc.Insert(&item2, item->DatIndex, -2);	// vlo�en� polo�ky do bufferu

// vlo�en� z�hlav� pro v�stupn� parametr
				item2.Func = IDF_OUT;					// v�stupn� parametr
				item2.Param = PR_NOMOVE | PR_ONE;		// parametry
				item2.RefIndex = IDF_OUT - IDF;			// index v intern�m bufferu
				BufLoc.Insert(&item2, item->DatIndex, -2);	// vlo�en� polo�ky do bufferu
			}
		}
	}
}
