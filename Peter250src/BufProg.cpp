
#include "Main.h"

/***************************************************************************\
*																			*
*							Buffer programu									*
*																			*
\***************************************************************************/

////////////////////////////////////////////////////////////////////
// Buffer pøistupuje ke globálním datovým seznamùm (Text, Icon, ...) !
////////////////////////////////////////////////////////////////////

// inicializaèní prázdná položka (pro vytvoøení nové položky)
PROGITEM	InitProgItem = {
	IDF_COMMENT,				// funkce

	0,							// zdrojové vlastností
	0,							// cílové vlastností
	0,							// parametry

	-1,							// referenèní blok
	-1,							// referenèní index
	-1,							// datový blok
	-1,							// datový index

	-1,							// rodiè
	-1,							// následující položka
	-1,							// pøedcházející položka
	-1,							// potomek

	-1,							// ikona
	-1,							// jméno

	NULL,						// položka stromu

	0							// èítaè referencí
};


////////////////////////////////////////////////////////////////////
// vytvoøení nových dat (vrací TRUE=operace OK) (oddìleno kvùli lepší optimalizaci)

#define NEWDATANUM 256				// poèet novì vytvoøených položek (16 KB)

bool CBufProg::NewData()
{
// nový poèet položek
	int next = m_Max;				// pøíští položka - 1
	int max = next + NEWDATANUM;	// nový poèet položek

// zvìtšení bufferu dat
	PROGITEM* newdata = (PROGITEM*)MemSize(m_Data, max*sizeof(PROGITEM));
	if (newdata == NULL) return false;
	m_Data = newdata;

// zvìtšení bufferu platnosti
	bool* newvalid = (bool*)MemSize(m_Valid, max*sizeof(bool));
	if (newvalid == NULL) return false;
	m_Valid = newvalid;

// nový maximální poèet položek v bufferu
	m_Max = max;

// vymazání pøíznakù platnosti položek (nastavení na pøíznak neplatnosti)
	int i;
	newvalid += next;
	for (i = NEWDATANUM; i > 0; i--) { *newvalid = false; newvalid++; }

// zaèlenìní do øetìzce volných položek
	newdata += next - 1;			// ukazatel dat - 1
	for (i = NEWDATANUM; i > 0; i--)
	{
		newdata++;					// zvýšení ukazatele položek
		next++;						// zvýšení indexu pøíští položky
		*(int*)newdata = next;		// odkaz na pøíští položku
	}
	*(int*)newdata = m_Next;		// navázání na další položku
	m_Next = m_Max-NEWDATANUM;		// odkaz na první novou položku

// pøíznak operace OK
	return true;
};


////////////////////////////////////////////////////////////////////
// vytvoøení nové položky (vrací index položky, pøi chybì vrací -1)

int CBufProg::NewItem()
{
// vytvoøení nových dat, není-li volná další položka
	if (m_Next < 0)				// není další položka?
	{
		if (!NewData()) return -1;	// vytvoøení nových dat
	}

// vyjmutí položky z øetìzce volných položek
	int i = m_Next;				// pøíští volná položka
	m_Next = *(int*)(m_Data + i); // další položka
	m_Valid[i] = true;			// nastavení pøíznaku platnosti položky
	m_Num++;					// zvýšení èítaèe platných položek

// novì vytvoøená položka
	return i;
};


////////////////////////////////////////////////////////////////////
// zrušení položky - zaøazení do volných položek (nekontroluje index a neruší objekt)

void _fastcall CBufProg::DelItem(const int index)
{
	*(int*)(m_Data + index) = m_Next;	// pøíští volná položka
	m_Valid[index] = false;				// zrušení pøíznaku platnosti
	m_Num--;							// snížení èítaèe platných položek
	m_Next = index;						// odkaz na tuto položku
}

////////////////////////////////////////////////////////////////////
// aktualizace fontu v oknì

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
// pøíprava indexu stavové ikony (upravit s INDEXTOSTATEIMAGEMASK() !)

int CBufProg::GetStateImage(int index)
{
// implicitní stav - nic
	int state = STATE_NONE;

// je položka platná?
	if (IsValid(index))
	{

// adresa položky
		PROGITEM* item = m_Data + index;

// obsluha okna editoru (požadavek nastavení promìnné) ...
		if ((m_BufID == BufEdiID) &&

// ... indikuje se v pøípadì, že není žádný parametr ,,,
			(item->Child < 0) &&

// ... položka vyžaduje nìjaká data ...
			(item->DstMask & PR_ALLDATA) && 

// ... ale nesmí vyžadovat i pøíkazy ...				
			((item->DstMask & PR_COMMAND) == 0) &&

// ... položka musí mít platného rodièe ...
			(item->Parent >= 0)
			)
		{

// adresa rodièe
			PROGITEM* parent = m_Data + item->Parent;

// položka musí být použita buï jako pøíkaz ...
			if ((parent->DstMask & PR_COMMAND) || 

// ... nebo jako parametr funkce ...
				(parent->Func == IDF_FNC) ||

// ... nebo jsou parametry vyžadovány vždy
				(item->Param & PR_PARPAR) ||

// ... nebo nelze použít jako pøíkaz - je to tedy parametr nìèeho
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

// pøíprava stavu pro okna objektù (definovaná položka)
		if (index == m_Def)
		{
			state = STATE_DEF;
		}
	}

// index stavové ikony
	return state;
}


////////////////////////////////////////////////////////////////////
// nové zobrazení položky

void CBufProg::DispNewItem(int index)
{
// kontrola platnosti položky a stromu
	if (IsNotValid(index) || (m_Tree == NULL)) return;

// lokální promìnné
	TV_INSERTSTRUCT tvins;					// struktura k vložení položky
	PROGITEM* item = m_Data + index;		// adresa položky k zobrazení

// adresa položky k zobrazení
	item = m_Data + index;

// maska parametrù položky
	tvins.item.mask = TVIF_CHILDREN | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_STATE | TVIF_TEXT;

// pøíprava textu položky
	tvins.item.pszText = (LPTSTR)(LPCTSTR)Text.GetTxt(GetText(index));

// ikona položky - zpìtné volání
	tvins.item.iImage =  I_IMAGECALLBACK;
	tvins.item.iSelectedImage =  I_IMAGECALLBACK;

// pøednastavení stavu
	tvins.item.stateMask = TVIS_EXPANDED | TVIS_BOLD | TVIS_CUT | TVIS_STATEIMAGEMASK;

// pøíprava stavové ikony
	tvins.item.state = INDEXTOSTATEIMAGEMASK(GetStateImage(index));

// pøíprava pøíznaku rozvinutí položky
	if (item->Param & PR_EXP)
	{
		tvins.item.state |= TVIS_EXPANDED;
	}

// pøíznak uzamèení položky
	if (item->Param & (PR_LOCK | PR_LOCK_DEP))
	{
		tvins.item.state |= TVIS_BOLD;
	}

// pøíznak vypnutí položky
	if (item->Param & (PR_OFF | PR_OFF_DEP))
	{
		tvins.item.state |= TVIS_CUT;
	}

// pøíprava položky potomkù
	if ((item->Child >= 0) || ((m_BufID == BufClsID) && ((item->Func == IDF_GROUP) || ((item->Parent < 0) && (item->Func != IDF_FNC)))))
	{
		tvins.item.cChildren = 1;
	}
	else
	{
		tvins.item.cChildren = 0;
	}

// pøíprava rodièe položky
	if (item->Parent >= 0)
	{
		tvins.hParent = m_Data[item->Parent].HTree;
	}
	else
	{
		tvins.hParent = TVI_ROOT;
	}

// pøíprava pøedcházející položky
	if (item->Prev >= 0)
	{
		tvins.hInsertAfter = m_Data[item->Prev].HTree;
	}
	else
	{
		tvins.hInsertAfter = TVI_FIRST;
	}

// zobrazení položky ve stromu a úschova handle položky
	item->HTree = (HTREEITEM)::SendMessage(m_Tree, TVM_INSERTITEM, 0, (LPARAM)&tvins);
}


////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CBufProg::CBufProg()
{
	m_Data = NULL;			// není buffer dat
	m_Valid = NULL;			// není buffer platnosti
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není buffer položek
	m_Next = -1;			// pøiští volná položka (-1=není)
	m_Undo = false;			// neregistrovat zmìny pro UNDO

	m_Redraw = 0;			// pøekreslování okna zapnuto
	m_First = -1;			// není první (ROOT) položka)
	m_Tree = NULL;			// není pøipojený strom
	m_Disp = -2;			// není nic zobrazeno
	m_IconWidth = 0;		// šíøka ikon (0 = není)
	m_IconHeight = 0;		// výška ikon (0 = není)
	m_Def = -1;				// definovaná položka (-1 = není)

	m_CharSet = DEFAULT_CHARSET; // implicitní znaková sada
	m_Font = NULL;			// font není
}

CBufProg::~CBufProg()
{
	DelAll();				// zrušení bufferu
}


////////////////////////////////////////////////////////////////////
// statický konstruktor a destruktor

void CBufProg::Init()
{
	m_Data = NULL;			// není buffer dat
	m_Valid = NULL;			// není buffer platnosti
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není buffer položek
	m_Next = -1;			// pøiští volná položka (-1=není)
	m_Undo = false;			// neregistrovat zmìny pro UNDO

	m_Redraw = 0;			// pøekreslování okna zapnuto
	m_First = -1;			// není první (ROOT) položka)
	m_Tree = NULL;			// není pøipojený strom
	m_Disp = -2;			// není nic zobrazeno
	m_IconWidth = 0;		// šíøka ikon (0 = není)
	m_IconHeight = 0;		// výška ikon (0 = není)
	m_Def = -1;				// definovaná položka (-1 = není)

	m_Nadpis.Init();		// inicializace nadpisu okna

	m_FontSet.Init();		// inicializace standardního fontu
	m_FontSet.Height = -1;	// výška fontu neurèena
	m_CharSet = DEFAULT_CHARSET; // implicitní znaková sada
	m_Font = NULL;			// font není
}

void CBufProg::Term()
{
	DelAll();				// zrušení bufferu
	m_Nadpis.Term();		// deinicializace nadpisu okna
	m_FontSet.Term();		// deinicializace defincie fontu
	DelFont(m_Font);		// zrušení fontu
}


////////////////////////////////////////////////////////////////////
// zrušení všech položek v bufferu (ukládání zaène opìt po øadì, nemìní obsah okna)

void CBufProg::DelAll()
{
	MemFree(m_Data);		// zrušení datového bufferu
	m_Data = NULL;			// není datový buffer
	MemFree(m_Valid);		// zrušení bufferu platnosti
	m_Valid = NULL;			// není buffer platnosti
	m_Num = 0;				// není žádná platná položka
	m_Max = 0;				// není žádná položka v bufferu
	m_Next = -1;			// není pøíští položka

	m_First = -1;			// není první (ROOT) položka)
	m_Disp = -2;			// není nic zobrazeno
	m_Def = -1;				// definovaná položka (-1 = není)
}


////////////////////////////////////////////////////////////////////
// vypnutí pøekreslování okna stromu

void CBufProg::RedrawOff()
{
// test, zda bylo pøekreslování zapnuto
	if (m_Redraw == 0)
	{

// vypnutí aktualizace stromu
		if (m_Tree != NULL)
		{
			::SendMessage(m_Tree, WM_SETREDRAW, FALSE, 0);
		}
	}

// zvýšení èítaèe úrovnì vypnutí pøekreslování
	m_Redraw++;
}


////////////////////////////////////////////////////////////////////
// zapnutí pøekreslování okna stromu

void CBufProg::RedrawOn()
{
// snížení èítaèe úrovnì vypnutí pøekreslování
	m_Redraw--;

// test, zda bude pøekreslování zapnuto
	if (m_Redraw == 0)
	{

// zapnutí aktualizace stromu
		if (m_Tree != NULL)
		{
			::SendMessage(m_Tree, WM_SETREDRAW, TRUE, 0);
		}
	}
}


////////////////////////////////////////////////////////////////////
// nastavení rodièe zobrazení položek + zobrazení položek (-1 = vše, -2 = nic)

void CBufProg::Disp(int disp)
{
// test, zda se rodiè zobrazení položek mìní
	if (disp == m_Disp) return;

// vypnutí aktualizace stromu
	RedrawOff();

// lokální promìnné
	PROGITEM* item;
	int index;

// vymazání aktuálnì zobrazeného obsahu stromu (je-li nìco zobrazeno)
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

// kontrola platnosti nového rodièe položek k zobrazení
	if ((disp != -1) && IsNotValid(disp))
	{
		m_Disp = -2;		
		RedrawOn();
		return;
	}

// nastavení nového rodièe zobrazení položek
	m_Disp = disp;

// pøíprava první položky k zobrazení
	if (disp < 0)
	{
		index = m_First;
	}
	else
	{
		index = m_Data[disp].Child;
	}

// cyklus zobrazení položek
	if (index >= 0)
	{
		do {

// nové zobrazení položky
			DispNewItem(index);

// adresa položky
			item = m_Data + index;

// nalezení následující položky
			index = item->Child;			// vnoøení na potomka

			if (index < 0)					// je potomek?
			{
				index = item->Next;			// pokraèování další položkou

				while ((index < 0) && (item->Parent >= 0))
				{
					index = item->Parent;	// návrat k rodièi
					if (index == disp) break; // je již opìt výchozí položka
					item = m_Data + index;	// adresa rodièe
					index = item->Next;		// další položka za rodièem
				}
			}

// dokud je další platná položka
		} while (index != disp);
	}

// zapnutí aktualizace stromu
	RedrawOn();
};


////////////////////////////////////////////////////////////////////
// nastaveni ikon ve stromu (zoom: TRUE = ikony 32*32)

void CBufProg::IconList(SMALLICON zoom)
{
// lokální promìnné
	HIMAGELIST img;			// seznam ikon
	HIMAGELIST imgs;		// seznam stavových ikon
	int icon;				// rozmìr ikon

// použity velké ikony
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

// test, zda je rozmìr ikon již nastaven
	if ((icon == m_IconWidth) && (icon == m_IconHeight)) return;

// nastavení nového rozmìru ikon
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
// nastavení definované položky (-1 = odznaèení)

void CBufProg::Def(int index)
{
// kontrola indexu
	if (IsNotValid(index)) index = -1;

// kontrola, zda se definovaná položka mìní
	if (index == m_Def) return;

// test, zda je zobrazen strom
	if (m_Tree != NULL)
	{

// lokální promìnné
		TV_ITEM tvi;

// zrušení oznaèení staré položky
		if (IsValid(m_Def) && (m_Data[m_Def].HTree != NULL))
		{
			tvi.mask = TVIF_STATE;
			tvi.stateMask = TVIS_STATEIMAGEMASK;
			tvi.hItem = m_Data[m_Def].HTree;
			tvi.state = INDEXTOSTATEIMAGEMASK(STATE_NONE);
			::SendMessage(m_Tree, TVM_SETITEM, 0, (LPARAM)&tvi);
		}

// uložení nové položky
		m_Def = index;

// bude zobrazení nové položky
		if (IsValid(index) && (m_Data[index].HTree != NULL))
		{

// pøíprava klientských rozmìrù okna
			RECT wrc;
			::GetClientRect(m_Tree, &wrc);

// zjištìní, zda je položka viditelná (zda je rychlejší nevypínat pøekreslování)
			RECT rc;
			*(HTREEITEM*)&rc = m_Data[index].HTree;
			BOOL visible = ::SendMessage(m_Tree, TVM_GETITEMRECT, FALSE, (LPARAM) &rc);

// upøesnìní viditelnosti položky
			if (visible)
			{
				visible = ((rc.top >= 0) && (rc.bottom <= wrc.bottom));
			}

// vypnutí pøekreslování, není-li položka viditelná
			if (!visible) RedrawOff();

// nastavení oznaèení nové položky
			tvi.mask = TVIF_STATE;
			tvi.stateMask = TVIS_STATEIMAGEMASK;
			tvi.hItem = m_Data[index].HTree;
			tvi.state = INDEXTOSTATEIMAGEMASK(STATE_DEF);
			::SendMessage(m_Tree, TVM_SETITEM, 0, (LPARAM)&tvi);

// zajištìní viditelnosti položky
			::SendMessage(m_Tree, TVM_ENSUREVISIBLE, 0, (LPARAM)m_Data[index].HTree);

// zapnutí pøekreslování
			if (!visible) RedrawOn();
		}
	}
	else

// jinak nastavení indexu položky bez pøekreslení okna
	{
		m_Def = index;
	}
}


////////////////////////////////////////////////////////////////////
// nastavení nadpisu okna

void CBufProg::Nadpis(const CText& text)
{
// kontrola, zda se text mìní
	if (m_Nadpis == text) return;

// nastavení nového nadpisu
	m_Nadpis = text;

// zobrazení nového nadpisu
	if (m_Tree != NULL)
	{
		ProgDispNadpis();
	}
}


////////////////////////////////////////////////////////////////////
// poskytnutí první zobrazené položky (-1 = není)

int CBufProg::Top()
{
// kontrola, zda je strom platný
	if (m_Tree == NULL) return -1;

// naètení zobrazené položky
	HTREEITEM htree = (HTREEITEM)::SendMessage(m_Tree, TVM_GETNEXTITEM, TVGN_FIRSTVISIBLE, 0);

// nalezení indexu položky
	return Find(htree);
}


////////////////////////////////////////////////////////////////////
// nastavení první zobrazené položky

void CBufProg::Top(int index)
{
// kontrola, zda je strom platný
	if (m_Tree == NULL) return;

// kontrola, zda je položka platná
	if (IsNotValid(index)) return;

// úschova souèasnì zobrazené první položky
	HTREEITEM htree = (HTREEITEM)::SendMessage(m_Tree, TVM_GETNEXTITEM, TVGN_FIRSTVISIBLE, 0);

// je-li již položka nastavená, nic se neprovede
	if (m_Data[index].HTree == htree) return;

// nastavení položky
	RedrawOff();
	::SendMessage(m_Tree, TVM_SELECTITEM, TVGN_FIRSTVISIBLE, (LPARAM)m_Data[index].HTree);
	RedrawOn();
}


////////////////////////////////////////////////////////////////////
// poskytnutí vybrané položky (-1 = není)

int CBufProg::Select()
{
// kontrola, zda je strom platný
	if (m_Tree == NULL) return -1;

// naètení vybrané položky
	HTREEITEM htree = (HTREEITEM)::SendMessage(m_Tree, TVM_GETNEXTITEM, TVGN_CARET, 0);

// nalezení indexu položky
	return Find(htree);
}


////////////////////////////////////////////////////////////////////
// nastavení vybrané položky (-1 = není)

void CBufProg::Select(int index)
{
// kontrola, zda je strom platný
	if (m_Tree == NULL) return;

// je-li již položka vybraná, nic se neprovede
	if (index == Select()) return;

// není-li položka platná, vypnutí vybrané položky
	if (IsNotValid(index) || (m_Data[index].HTree == NULL))
	{
		::SendMessage(m_Tree, TVM_SELECTITEM, TVGN_CARET, 0);
	}
	else
	{

// pøíprava klientských rozmìrù okna
		RECT wrc;
		::GetClientRect(m_Tree, &wrc);

// zjištìní, zda je položka viditelná (zda je rychlejší nevypínat pøekreslování)
		RECT rc;
		*(HTREEITEM*)&rc = m_Data[index].HTree;
		BOOL visible = ::SendMessage(m_Tree, TVM_GETITEMRECT, FALSE, (LPARAM) &rc);

// upøesnìní viditelnosti položky
		if (visible)
		{
			visible = ((rc.top >= 0) && (rc.bottom <= wrc.bottom));
		}

// vypnutí pøekreslování, není-li položka viditelná
		if (!visible) RedrawOff();

// zmìna vybrané položky
		::SendMessage(m_Tree, TVM_SELECTITEM, TVGN_CARET, (LPARAM)m_Data[index].HTree);

// zajištìní viditelnosti položky
		::SendMessage(m_Tree, TVM_ENSUREVISIBLE, 0, (LPARAM)m_Data[index].HTree);

// zapnutí pøekreslování
		if (!visible) RedrawOn();
	}
}


////////////////////////////////////////////////////////////////////
// aktualizace zobrazení stavové ikony položky pro okno editoru programu

void _fastcall CBufProg::AktStateImage(int index)
{
// je položka platná (a je to okno editoru programu)?
	if ((m_BufID == BufEdiID) && 
		IsValid(index) && 
		(m_Data[index].HTree != NULL) && 
		(m_Tree != NULL))
	{

// požadovaný nový stav položky
		DWORD newstate = INDEXTOSTATEIMAGEMASK(GetStateImage(index));

// naètení starého stavu položky
		TV_ITEM tvi;
		tvi.hItem = m_Data[index].HTree;
		tvi.mask = TVIF_STATE;
		tvi.stateMask = TVIS_STATEIMAGEMASK;
		tvi.state = newstate;
		::SendMessage(m_Tree, TVM_GETITEM, 0, (LPARAM)&tvi);
		
// nastavení nového stavu položky, pokud se mìní
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
// index následující položky ve stromu (cyklicky, -1 = navrátí se první položka)

int _fastcall CBufProg::NextItem(int index)
{
// kontrola platnosti položky
	if (IsNotValid(index)) return m_First;

// adresa položky
	PROGITEM* item = m_Data + index;

// pøesun na potomka
	index = item->Child;				// potomek

// není-li potomek, pøesun na následující prvek
	if (index < 0)						// je nìjaký potomek?
	{
		index = item->Next;				// následující prvek

// není-li následující prvek, pøesun na rodièe
		while (index < 0)				// je následující prvek?
		{
			index = item->Parent;		// index rodièe

			if (index < 0)				// je platný rodiè ?
			{
				index = m_First;		// první položka v ROOT
				ASSERT(index >= 0);
			}
			else
			{
				item = m_Data + index;	// adresa rodièe
				index = item->Next;		// následující prvek
			}
		}
	}
	return index;
}


/////////////////////////////////////////////////////////////////////////////
// index pøedcházející položky ve stromu (cyklicky, -1 = navrátí se poslední položka)

int _fastcall CBufProg::PrevItem(int index)
{
// kontrola platnosti položky
	if (IsNotValid(index)) index = m_First;

// není žádná položka
	if (IsNotValid(index)) return -1;

// adresa položky
	PROGITEM* item = m_Data + index;

// pøesun na pøedcházející položku
	index = item->Prev;				// pøedcházející položka

// kontrola, zda je pøedcházející položka
	if (index >= 0)					// je pøedcházející položka?
	{
		item = m_Data + index;		// adresa pøedcházející položky

// má-li pøedcházející položka potomka, pøesun na potomka
		if (item->Child >= 0)		// je nìjaký potomek?
		{
			index = item->Child;	// potomek
			item = m_Data + index;	// adresa potomka

// nalezení poslední položky potomka
			while ((item->Next >= 0) || (item->Child >= 0))
			{
				while (item->Next >= 0)
				{
					index = item->Next;	// další položka
					item = m_Data + index; // adresa další položky
				}

				if (item->Child >= 0)	// je nìjaký potomek?
				{
					index = item->Child; // potomek
					item = m_Data + index; // adresa potomka
				}
			}
		}
	}

// není pøedcházející položka, pøesun na rodièe
	else
	{
		index = item->Parent;		// index rodièe

// není-li další rodiè, pøesun na první položku ROOT
		if (index < 0)				// je rodiè?
		{
			index = m_First;		// první položka v ROOT
			ASSERT(index >= 0);
			item = m_Data + index;	// adresa první položky

// nalezení úplnì poslední položky stromu
			while ((item->Next >= 0) || (item->Child >= 0))
			{
				while (item->Next >= 0)
				{
					index = item->Next;	// další položka
					item = m_Data + index; // adresa další položky
				}

				if (item->Child >= 0)	// je nìjaký potomek?
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
// navrácení položky v UNDO operaci (vrací TRUE=operace OK)
// provádí záznam do UNDO bufferu

bool CBufProg::UndoIns(const PROGITEM* item, const int index)
{
// vytvoøení nových dat (zajištìní existence položky)
	ASSERT(index >= 0);
	while (index >= m_Max)
	{
		if (!NewData()) return false;
	}
	ASSERT(IsNotValid(index));					// položka musí být neplatná

// lokální promìnné
	PROGITEM*	itemNew = m_Data + index;			// adresa nové položky
	int			parent = item->Parent;				// rodiè položky
	PROGITEM*	itemPar = m_Data + parent;			// adresa rodièe
	int			prev = item->Prev;					// pøedcházející položka
	PROGITEM*	itemAft = m_Data + prev;			// adresa pøedcházející položky
	int			next = item->Next;					// následující položka
	PROGITEM*	itemNxt = m_Data + next;			// adresa následující položky

	ASSERT((parent == -1) || (IsValid(parent)));

// záznam operace pro UNDO
	if (m_Undo)
	{
		if (!Undo.AddProgIns(m_BufID, index)) return false;
	}

// vyjmutí položky z volných položek
	int i = m_Next;								// ukazatel øetìzce volných položek

// navrácena hned první položka z øetìzce volných položek
	if (i == index)								// je to hned první položka?
	{
		m_Next = *(int*)itemNew;				// odkaz na další položku
	}
	else
	{

// nalezení položky v øetìzci volných položek
		while (*(int*)(m_Data + i) != index)
		{
			i = *(int*)(m_Data + i);

// tento pøípad nesmí nikdy nastat - položka nebyla nalezena!!!
			ASSERT(i >= 0);
			if (i < 0)								// to je chyba!!!!!!!
			{
				Undo.DelAll();
				return false;
			}
		}

// pøeskoèení odkazu na další položku
		*(int*)(m_Data + i) = *(int*)itemNew;
	}

// nastavení pøíznaku platnosti položky
	m_Num++;									// zvýšení èítaèe položek
	m_Valid[index] = true;						// pøíznak platnosti položky

// pøenesení dat položky
	MemCopy(itemNew, item, SIZEOFPROGITEM);

// napojení na pøedcházející položku a na rodièe
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

			if ((itemNew->Next < 0) && (itemPar->HTree != NULL) && (m_Tree != NULL))	// první potomek rodièe?
			{
				TV_ITEM tvi;
				tvi.hItem = itemPar->HTree;
				tvi.mask = TVIF_CHILDREN;
				tvi.cChildren = 1;
				::SendMessage(m_Tree, TVM_SETITEM, 0, (LPARAM)&tvi);

				AktStateImage(parent);		// aktualizace stavové ikony pro okno editoru
			}
		}
		else
		{
			itemNew->Parent = -1;			// není rodiè
			itemNew->Next = m_First;		// pøedøazení pøed první ROOT položku
			m_First = index;				// položka bude první ROOT položkou
		}
	}

// napojení pøed následující položku
	if (IsValid(next))
	{
		itemNxt->Prev = index;
	}
	else
	{
		itemNew->Next = -1;
	}

// inicializace ostatních parametrù
	itemNew->Child = -1;					// není žádný potomek
	itemNew->HTree = NULL;					// není zobrazení položky stromu
	itemNew->Refer = 0;						// nejsou žádné reference na položku

// zvýšení èítaèe u referenèní položky (kromì bufferu tøíd)
	if (((DWORD)(itemNew->RefBlok) < (DWORD)PROGBUFNUM) && (m_BufID != BufClsID))
	{
		if (BufProg[itemNew->RefBlok].IsValid(itemNew->RefIndex))
		{
			BufProg[itemNew->RefBlok][itemNew->RefIndex].Refer++;
		}
	}

// zobrazení položky, je-li v zobrazené vìtvi
	if ((((parent >= 0) && (itemPar->HTree != NULL)) ||
		(m_Disp == parent)))
	{
		DispNewItem(index);
	}

	return true;
}


////////////////////////////////////////////////////////////////////
// vložení položky bez navázání dat (pro UNDO buffer)
// vrací index položky, pøi chybì pamìti vrací -1

int CBufProg::Insert0(const PROGITEM* item)
{
	int result = NewItem();		// vytvoøení nové položky
	if (result >= 0)
	{
		MemCopy(m_Data + result, item, SIZEOFPROGITEM);
	}
	return result;
}


////////////////////////////////////////////////////////////////////
// zrušení položky bez navázání dat (pro UNDO buffer)

void CBufProg::Del0(int index)
{
	if (IsValid(index)) DelItem(index);
}


////////////////////////////////////////////////////////////////////
// vložení položky (vrací index položky, pøi chybì pamìti vrací -1) - parametry nemusí být platné
//  parent = rodiè (-1=ROOT), after=pøedcházející položka 
//		(-1=první, -2=poslední, -3=abecednì, -4=abecednì s pøedností skupin)
//  Položky nastavené funkcí: Parent, Next, Prev, Child, HTree, Refer,

int CBufProg::Insert(const PROGITEM* item, int parent, int after)
{
// založení nové položky
	int index = NewItem();					// vytvoøení nové položky
	if (index < 0) return -1;				// chyba pamìti

// záznam operace pro UNDO
	if (m_Undo)
	{
		if (!Undo.AddProgIns(m_BufID, index))
		{
			DelItem(index);
			return -1;
		}
	}

// kopie obsahu položky
	PROGITEM* itemNew = m_Data + index;		// adresa nové položky
	MemCopy(itemNew, item, SIZEOFPROGITEM);	// kopie položky

// kontrola platnosti rodièe, pøíprava adresy rodièe
	if (IsNotValid(parent)) parent = -1;	// není platný - bude to ROOT položka
	PROGITEM* itemPar = m_Data + parent;	// adresa rodièe

// nastavení odkazu na rodièe
	itemNew->Parent = parent;				// odkaz na rodièe (pro ROOT bude = -1)

// nalezení pøedchozí položky abecednì
	if (after == -3)						// je abecední tøídìní?
	{
		after = -1;							// uložení na zaèátek

// pøíprava textu vkládané položky
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

// pøíprava indexu první položky
		int inx = m_First;					// první položka, není-li rodiè platný
		if (IsValid(parent))				// je rodiè platný?
		{
			inx = itemPar->Child;			// první potomek rodièe
		}

// nalezení pozice
		while (IsValid(inx) && (txt >= Text.GetTxt(GetText(inx))))
		{
			after = inx;					// položka bude pøedcházející
			inx = m_Data[inx].Next;			// posun na další položku
		}
	}

// nalezení pøedchozí položky abecednì s pøedností skupin
	if (after == -4)						// je abecední tøídìní?
	{
		after = -1;							// uložení na zaèátek

// pøíprava textu vkládané položky
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

// pøíprava indexu první položky
		int inx = m_First;					// první položka, není-li rodiè platný
		if (IsValid(parent))				// je rodiè platný?
		{
			inx = itemPar->Child;			// první potomek rodièe
		}

// nalezení pozice
		while (IsValid(inx) &&				// položka je platná
				(
					(
						(m_Data[inx].Func == IDF_GROUP) && // ne-skupina se øadí vždy za skupinu
						(item->Func != IDF_GROUP)
					) ||
					(
						(m_Data[inx].Func == IDF_GROUP) &&	// abecední setøídìní skupin
						(item->Func == IDF_GROUP) &&
						(txt >= Text.GetTxt(GetText(inx)))
					) ||
					(
						(m_Data[inx].Func != IDF_GROUP) &&	// abecední setøídìní ne-skupin
						(item->Func != IDF_GROUP) &&
						(txt >= Text.GetTxt(GetText(inx)))
					)
				)
			)
		{
			after = inx;					// položka bude pøedcházející
			inx = m_Data[inx].Next;			// posun na další položku
		}
	}

// pøíprava adresy pøedešlé položky (i když je neplatná)
	PROGITEM* itemAft = m_Data + after;				// adresa pøedcházející položky

// nemá-li to být první položka, bude se hledat napojení na pøedcházející položku
	if (after != -1)
	{

// nalezení stejného rodièe, je-li pøedešlá položka v podvìtvi
		if (IsValid(after))					// je pøedešlá položka platná?
		{
			while ((itemAft->Parent != parent) && (itemAft->Parent >= 0))
			{
				after = itemAft->Parent;
				itemAft = m_Data + after;			// adresa pøedcházející položky
			}
		}

// ovìøení, zda je pøedcházející položka platná (musí mít stejného rodièe)
		if (IsNotValid(after) || (itemAft->Parent != parent))
		{	

// pøíprava výchozí položky - buï první potomek rodièe nebo první položka ROOT
			if (parent >= 0)					// je rodiè platný?
			{
				after = itemPar->Child;			// první potomek rodièe
			}
			else
			{
				after = m_First;				// jinak první položka v ROOT
			}

// nalezení poslední položky
			if (after >= 0)						// je nìjaká položka?
			{
				itemAft = m_Data + after;		// adresa položky

				while (itemAft->Next >= 0)		// je platná další položka?
				{
					after = itemAft->Next;		// posun na další položku
					itemAft = m_Data + after;	// adresa další položky
				}
			}
		}
	}

// napojení položky na pøedcházející položku (-1 = první)
	itemNew->Prev = after;					// odkaz na pøedcházející položku
	if (after >= 0)							// je pøedcházející položka platná?
	{
		itemNew->Next = itemAft->Next;		// pøenesení následující položky
		itemAft->Next = index;				// navázání odkazu na tuto položku
	}
	else
	{
		if (parent >= 0)					// je rodiè platný?
		{
			itemNew->Next = itemPar->Child;	// pøedøazení pøed prvního potomka
			itemPar->Child = index;			// položka bude prvním potomkem rodièe

			if ((itemNew->Next < 0) && (itemPar->HTree != NULL) && (m_Tree != NULL))	// první potomek rodièe?
			{
				TV_ITEM tvi;
				tvi.hItem = itemPar->HTree;
				tvi.mask = TVIF_CHILDREN;
				tvi.cChildren = 1;
				::SendMessage(m_Tree, TVM_SETITEM, 0, (LPARAM)&tvi);

				AktStateImage(parent);		// aktualizace stavové ikony
			}
		}
		else
		{
			itemNew->Next = m_First;		// pøedøazení pøed první ROOT položku
			m_First = index;				// položka bude první ROOT položkou
		}
	}

// napojení pøed následující položku
	if (itemNew->Next >= 0)
	{
		m_Data[itemNew->Next].Prev = index;
	}

// inicializace ostatních parametrù
	itemNew->Child = -1;					// není žádný potomek
	itemNew->HTree = NULL;					// není zobrazení položky stromu
	itemNew->Refer = 0;						// nejsou žádné reference na položku

// zvýšení èítaèe u referenèní položky (ne pro buffer tøíd)
	if (((DWORD)(itemNew->RefBlok) < (DWORD)PROGBUFNUM) && (m_BufID != BufClsID))
	{
		if (BufProg[itemNew->RefBlok].IsValid(itemNew->RefIndex))
		{
			BufProg[itemNew->RefBlok][itemNew->RefIndex].Refer++;
		}
	}

// zobrazení položky, je-li v zobrazené vìtvi
	if ((((parent >= 0) && (itemPar->HTree != NULL)) ||
		(m_Disp == parent)))
	{
		DispNewItem(index);
	}

// navrácení indexu nové položky
	return index;
}


////////////////////////////////////////////////////////////////////
// naètení inicializaèní tabulky bufferu (starý obsah bufferu se neruší - lze pøidávat)
// pøi chybì pamìti vrací FALSE

bool CBufProg::InitTab(const INITTREE* tab, const int num)
{
// vypnutí pøekreslování okna
	RedrawOff();

// pøíprava bufferu rodièù
	int ParBuf[30];							// to snad bude vždy dostaèující
	for (int i = 0; i < 30; i++) ParBuf[i] = -1; // rodiè je neplatný

// inicializace nemìnných parametrù položky
	PROGITEM item;							// vytváøená nová položka
	item.RefBlok = BufIntID;				// reference na buffer interních funkcí
	item.DatBlok = -1;						// datový blok není
	item.DatIndex = -1;						// datový index
	item.Icon = -1;							// ikona implicitní
	item.Name = -1;							// jméno implicitní

// cyklus pøes položky tabulky
	for (i = 0; i < num; i++)
	{

// pøíprava odkazu na deklaraèní funkci
		item.Func = tab[i].Func;			// identifikaèní kód funkce
		int fnc = tab[i].Func - IDF;		// index funkce
		ASSERT(BufInt.IsValid(fnc));
		PROGITEM* itemfnc = &BufInt[fnc];	// adresa deklaraèní funkce

// pøenesení parametrù z deklaraèní funkce
		item.SrcMask = itemfnc->SrcMask;	// zdrojové vlastnosti
		item.DstMask = itemfnc->DstMask;	// cílové vlastnosti
		item.Param = itemfnc->Param;		// parametry

// odkaz na deklaraèní funkci
		item.RefIndex = fnc;				// referenèní index

// pøidání položky do bufferu (a aktualizace bufferu rodièù)
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
// zapnutí pøekreslování okna
	RedrawOn();

	return true;
}


////////////////////////////////////////////////////////////////////
// vytvoøení prázdné položky (vrací index položky, -1=chyba pamìti)

int CBufProg::New()
{
	int result = NewItem();		// vytvoøení nové položky
	if (result >= 0)
	{
		MemCopy(m_Data + result, &InitProgItem, SIZEOFPROGITEM);
	}
	return result;
}


////////////////////////////////////////////////////////////////////
// nalezení položky podle handle (-1 = není)

int CBufProg::Find(HTREEITEM htree)
{
// je to platný handle?
	if (htree == NULL) return -1;

// pøíprava výchozí zobrazené položky
	int index;
	if (m_Disp < 0)
	{
		index = m_First;
	}
	else
	{
		index = m_Data[m_Disp].Child;
	}

// kontrola, zda je platná výchozí položka
	if (IsNotValid(index)) return -1;

// pøíprava koncového indexu
	int last = m_Data[index].Parent;		// koncový index

// cyklus nalezení položky
	for (;;)
	{

// adresa testované položky
		PROGITEM* item = m_Data + index;			// adresa testované položky

// test, zda je to hledaná položka
		if (item->HTree == htree) return index;

// pokus o vnoøení do potomkù
		index = item->Child;			// vnoøení na potomka
		if (index < 0)					// není žádný potomek?
		{

// nejsou-li potomci, zkusí se následující položka
			index = item->Next;			// pokraèování další položkou

// není-li ani další položka, vrátí se k rodièi
			while (index < 0)
			{
				index = item->Parent;	// návrat k rodièi
				if (index == last) return -1;	// položka nenalezena
				item = m_Data + index;	// adresa rodièe
				index = item->Next;		// další položka za rodièem
			}
		}
	}
}


////////////////////////////////////////////////////////////////////
// nalezení položky s ukazatelem na data (-1 = není)

int CBufProg::SrcDat(int bufID, int index)
{
// kontrola platnosti údajù
	if ((bufID < 0) || (index < 0)) return -1;

// nalezení položky
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
// nalezení referující položky od aktuální položky cyklicky (-1 = nenalezeno)

int CBufProg::SrcRef(int RefBlok, int RefIndex, BOOL next)
{
// pøíprava výchozí položky
	int index = Select();						// vybraná položka
	PROGITEM* item;								// adresa položky

// test první položky, není-li žádná položka vybraná (jen pøi smìru vpøed)
	if (IsNotValid(index))						// hledá se od zaèátku?
	{		
		index = m_First;						// index první položky
		if (index < 0) return -1;				// není první položka

		if (next)								// hledá se smìrem vpøed?
		{
			item = m_Data + index;				// adresa položky
			if ((item->RefBlok == RefBlok) &&	// souhlasí referenèní buffer ?
				(item->RefIndex == RefIndex))	// souhlasí referenèní index ?
			{
				return index;					// index nalezené položky
			}
		}
	}

// cyklus vyhledání položky
	for (int i = m_Num; i > 0; i--)
	{

// index další položky (cyklicky, až opìt po vybranou položku)
		if (next)
		{
			index = NextItem(index);
		}
		else
		{
			index = PrevItem(index);
		}

// adresa položky
		item = m_Data + index;

// test, zda to je hledaná položka
		if ((item->RefBlok == RefBlok) &&		// souhlasí referenèní buffer ?
			(item->RefIndex == RefIndex))		// souhlasí referenèní index ?
		{
			return index;						// index nalezené položky
		}
	}

	return -1;									// položka nenalezena
}


////////////////////////////////////////////////////////////////////
// nalezení indexu textu k položce (-1 = neplatný)

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
// nalezení indexu ikony k položce (-1 = neplatný)

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
// poskytnutí poètu referencí pro uživatele

int CBufProg::GetRefer(int index)
{
// kontrola platnosti položky
	int ref = 0;
	if (IsValid(index))
	{

// adresa položky
		PROGITEM* item = m_Data + index;

// poèet referencí na položku
		ref = item->Refer;

// z editoru pøesmìrování na deklaraèní buffer
		if ((m_BufID == BufEdiID) && ((DWORD)item->RefBlok < (DWORD)PROGBUFNUM) && (item->RefBlok != BufEdiID))
		{
			ref = BufProg[item->RefBlok].GetRefer(item->RefIndex);
		}

// v lokálním bufferu korekce vstupních promìnných
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
// test povolení zrušení vìtve z hlediska referencí na položky (TRUE=OK)

bool CBufProg::TestRefer(int index)
{
// kontrola, zda je položka platná a zda je to buffer objektù
	if (IsNotValid(index) || ((m_BufID != BufObjID) && (m_BufID != BufLocID))) return true;

// úschova výchozí položky
	int first = index;						// úschova výchozí položky
	PROGITEM* item;

// cyklus pøes celou vìtev
	for (;;)
	{

// test, zda jsou na položku nìjaké reference
		if (GetRefer(index) > 0)
		{

// potvrzení operace
			CText text;
			CText nadpis;
			if (!text.Load(IDN_TESTREFER) ||
				!nadpis.Load(IDN_TESTREFER2)) return false;

			return (::MessageBox(MainFrame, text, nadpis,
				MB_OKCANCEL | MB_ICONEXCLAMATION) == IDOK);
		}

// adresa položky
		item = m_Data + index;

// nalezení následující položky
		if ((index == first) && (item->Child < 0)) return true;

		index = item->Child;			// vnoøení na potomka
		if (index < 0)					// není žádný potomek?
		{
			index = item->Next;			// pokraèování další položkou

			while (index < 0)
			{
				index = item->Parent;	// návrat k rodièi
				if (index == first) return true;
				ASSERT(index >= 0);
				item = m_Data + index;	// adresa rodièe
				index = item->Next;		// další položka za rodièem
			}
		}
	}
}


////////////////////////////////////////////////////////////////////
// pøekreslení ikony ve stromu

void CBufProg::ReDispIcon(int icon)
{
// kontrola platnosti ikony a je-li nìjaký strom
	if (Icon.IsNotValid(icon) || (m_Tree == NULL)) return;

// pøíprava indexu položky
	int index = m_Max-1;
	PROGITEM* item = m_Data + index;
	int icon2;
	TV_ITEM tvi;
	tvi.mask = TVIF_IMAGE;

// cyklus pøes všechny položky
	for (; index >= 0; index--)
	{

// kontrola, zda je položka platná a zda je zobrazena
		if (m_Valid[index] && (item->HTree != NULL))
		{

// nalezení indexu ikony
			icon2 = GetIcon(index);

// kontrola, zda je to hledaná ikona
			if (icon2 == icon)
			{

// zajištìní pøekreslení položky
				tvi.hItem = item->HTree;
				tvi.iImage = 0;
				::SendMessage(m_Tree, TVM_SETITEM, 0, (LPARAM)&tvi);
				tvi.iImage = I_IMAGECALLBACK;
				::SendMessage(m_Tree, TVM_SETITEM, 0, (LPARAM)&tvi);
			}
		}

// posun adresy položky
		item--;
	}
}


////////////////////////////////////////////////////////////////////
// test èísla funkce, zda je to objekt (s jedineèným jménem)

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
// nalezení objektu daného jména (-1 = nenalezeno) - rozlišuje velká/malá písmena a mezery
//		name ..... hledaný text
//		over ..... pøeskoèená položka (-1 = není)
//		first .... první položka (-1 = od zaèátku)
//		next ..... pøíští položka pro zastavení (-1 = po konec)

int CBufProg::FindObj(const CMultiText& name, int over, int first, int next)
{
// prázdný text - položka nenalezena
	if (name.MultiText().IsEmpty()) return -1;

// korekce první položky
	if (IsNotValid(first))
	{
		first = m_First;
		if (IsNotValid(first)) return -1;
	}

// korekce pøíští položky
	if (IsNotValid(next))
	{
		next = m_First;
	}

// cyklus pøes všechny položky
	CMultiText txt;
	do
	{

// adresa položky
		PROGITEM* item = m_Data + first;

// test, zda je to položka objektu
		if (TestObj(item->Func) &&
			(first != over))
		{
			CBufProg* buf = this;
			int inx = first;

// pøesmìrování z editoru na objekty
			if ((item->RefBlok == BufObjID) ||
				(item->RefBlok == BufLocID))
			{
				inx = item->RefIndex;
				buf = BufProg + item->RefBlok;
				ASSERT(buf->IsValid(inx));
			}

// netestuje se ROOT lokální funkce
			if ((m_BufID != BufLocID) || (item->Parent >= 0))
			{

// porovnání jména objektu (staèí shodnost jednoho v kterémkoliv z jazykù)
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

// dalši položka
		first = NextItem(first);
		ASSERT(first >= 0);

// až po poslední položku
	} while (first != next);

// objekt nenalezen
	return -1;
}


////////////////////////////////////////////////////////////////////
// zrušení vìtve stromu (pøi chybì pamìti vrací FALSE - stav není navrácen!)

bool CBufProg::Del(int index)
{
// kontrola platnosti indexu
	if (IsNotValid(index)) return true;

// lokální promìnné
	int first = index;					// úschova indexu výchozí položky
	PROGITEM* item;						// adresa rušené položky
	CBufProg* buf;						// adresa referujícího bufferu
	PROGITEM* item2;					// adresa referující položky
	int i;								// pomocný èítaè (index referující položky)
	int next = index;					// index pøíští položky k rušení
	int parent;							// index rodièe rušené položky

// cyklus pøes všechny položky - zaèíná od nejhlubších položek
	do {

// pøíprava adresy položky
		index = next;					// index rušené položky
		item = m_Data + index;			// adresa rušené položky
		parent = item->Parent;			// index rodièe rušené položky

// pokud je nìjaký potomek, vnoøí se nejdøíve do potomka
		if (item->Child >= 0)			// má položka potomky?
		{
			next = item->Child;			// pøíštì se ruší potomek položky
			index = next;				// index aktuální položky (kvùli kontrole WHILE)
		}

// položka je bez dalších potomkù, mùže se zrušit
		else
		{

// kontrola, jestli jsou na položku nìjaké reference
			if (item->Refer > 0)
			{

// zrušení referencí z bufferu editoru programu
				ASSERT((m_BufID == BufObjID) || (m_BufID == BufLocID));

				buf = &BufEdi;			// adresa bufferu editoru programu
				i = buf->Max()-1;		// index poslední položky v bufferu
				item2 = buf->Data() + i; // adresa poslední položky

				for (; i >= 0; i--)		// cyklus pøes všechny položky bufferu
				{
					if (buf->m_Valid[i])	// je to platná položka?
					{
						if ((item2->RefIndex == index) && // je to hledaná reference?
							(item2->RefBlok == m_BufID))
						{
							if (!buf->Del(i)) return false;	// zrušení referující položky
						}
					}
					item2--;			// posun adresy referující položky
				}

// pro ikonu zrušení její použití ve všech plochách
				if ((item->Refer > 0) && (item->DatBlok == BufIcoID))
				{
					Map.DelIcon(item->DatIndex);
				}

				ASSERT(item->Refer == 0);	// teï již nesmí nic referovat!
			}

// zrušení programových dat
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

// záznam UNDO operace (data se pøi UNDO musí navracet døíve)
			if (m_Undo)
			{
				if (!Undo.AddProgDel(m_BufID, index, item))
				{
					Undo.DelAll();
					return false;
				}
			}

// zrušení dat položky
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

// zrušení ikony položky
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

// zrušení textu položky
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

// zrušení zobrazení položky
			if (item->HTree != NULL)
			{
				::SendMessage(m_Tree, TVM_DELETEITEM, 0, (LPARAM)item->HTree);
			}

// úschova pøíští položky
			next = item->Next;

// napojení pøedcházející položky na následující
			if (next >= 0)
			{
				m_Data[next].Prev = item->Prev;
			}

// napojení následující položky na pøedcházejíci
			if (item->Prev >= 0)
			{
				m_Data[item->Prev].Next = next;
			}

// oprava odkazu z rodièe
			if ((parent >= 0) && (m_Data[parent].Child == index))
			{
				m_Data[parent].Child = next;
			}

// pøesmìrování ukazatele na první položku
			if (index == m_First)
			{
				m_First = next;
			}

// pøesmìrování rodièe zobrazených položek
			if (index == m_Disp)
			{
				m_Disp = -2;
			}

// není-li další položka, poukraèuje se rodièem
			if ((next < 0) && (index != first))
			{
				next = parent;
			}

// snížení èítaèe referencí
			if (((DWORD)item->RefBlok < (DWORD)PROGBUFNUM) && (m_BufID != BufClsID) &&
				BufProg[item->RefBlok].IsValid(item->RefIndex))
			{
				BufProg[item->RefBlok][item->RefIndex].Refer--;
			}

// zrušení definované položky
			if (index == m_Def) m_Def = -1;

// zrušení položky
			DelItem(index);
		}

// dokud nebyla zrušena výchozí položka
	} while (index != first);

// korekce zobrazení potomkù u rodièe
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
// pøesun vìtve stromu (vrací nový index položky) 
// (parent: -1 = ROOT, after: -1 = zaèátek, -2 = konec)

int CBufProg::Move(int parent, int after, int src)
{
// lokální promìnné
	PROGITEM*	item;						// adresa pøenášené položky
	PROGITEM*	itemPar;					// adresa cílového rodièe
	PROGITEM*	itemAft;					// adresa cílové pøedcházející položky
	CBufProg*	bufedi = &BufEdi;			// editaèní buffer
	int			inxedi;						// index položky v editaèním bufferu
	PROGITEM*	itemedi;					// adresa položky v editaèním bufferu
	int oldparent;							// úschovaný rodiè zdrojové položky
	int first;								// uschovaná výchozí zdrojová položka

// kontrola platnosti zdrojového indexu
	ASSERT(IsValid(src));
	if (IsNotValid(src)) return src;		// nic se nemìní
	item = m_Data + src;					// adresa položky
	oldparent = item->Parent;				// pùvodní rodiè zdrojové položky


// -------------- korekce ukazatelù

// kontrola platnosti nového rodièe
	if (IsNotValid(parent))					// je platný rodiè?
	{
		parent = -1;						// není platný - bude to ROOT položka
	}

// adresa cílového rodièe (i neplatný)
	itemPar = m_Data + parent;				// adresa rodièe (i když je neplatný)

// adresa cílové pøedcházející položky (i neplatné)
	itemAft = m_Data + after;				// adresa pøedcházející položky (i když je neplatná)

// není-li požadována první položka, bude se hledat napojení na pøedcházející položku
	if (after != -1)
	{

// korekce pøedešlé cílové položky, je-li v podvìtvi
		if (IsValid(after))					// je pøedešlá položka platná?
		{
			while ((itemAft->Parent != parent) && (itemAft->Parent >= 0))
			{
				after = itemAft->Parent;
				itemAft = m_Data + after;			// adresa pøedcházející položky
			}
		}

// ovìøení, zda je pøedcházející položka platná (musí mít stejného rodièe)
		if (IsNotValid(after) || (itemAft->Parent != parent))
		{	

// pøíprava výchozí pøedešlé položky - buï první potomek rodièe nebo první položka ROOT
			if (parent >= 0)					// je rodiè platný?
			{
				after = itemPar->Child;			// první potomek rodièe
			}
			else
			{
				after = m_First;				// jinak první položka v ROOT
			}

// nalezení poslední položky
			if (after >= 0)						// je nìjaká položka?
			{
				itemAft = m_Data + after;		// adresa položky

				while (itemAft->Next >= 0)		// je platná další položka?
				{
					after = itemAft->Next;		// posun na další položku
					itemAft = m_Data + after;	// adresa další položky
				}
			}
		}
	}

// kontrola, zda se vùbec nìco mìní
	if ((oldparent == parent) && 
		((item->Prev == after) || (src == after)))
	{
		return src;
	}

// pøíznak modifikace souboru
	SetModi();

// záznam pro UNDO
	if (m_Undo)
	{
		if (!Undo.AddProgMove(m_BufID, src, oldparent, item->Prev))
		{
			Undo.DelAll();
		}
	}

// ---------------- odpojení vìtve

// odpojení od pøedcházející položky
	if (item->Prev >= 0)
	{
		m_Data[item->Prev].Next = item->Next;
	}
	else
	{

// odpojení od rodièe
		if (oldparent >= 0)
		{
			m_Data[oldparent].Child = item->Next;

// aktualizace rodièe, pokud to byl jeho poslední potomek
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

// oprava ukazatele na první položku
	if (m_First == src)
	{
		m_First = item->Next;
	}

// odpojení od následující položky
	if (item->Next >= 0)
	{
		m_Data[item->Next].Prev = item->Prev;
	}

// zrušení zobrazení položky
	if (item->HTree)
	{
		::SendMessage(m_Tree, TVM_DELETEITEM, 0, (LPARAM)item->HTree);
	}

// ----------------- odsun vstupního parametru

// kontrola, zda je odsunut vstupní parametr mezi lokální promìnné
	if ((m_BufID == BufLocID) &&
		(oldparent >= 0) &&
		(m_Data[oldparent].Func == IDF_PAR) &&
		(parent != oldparent))
	{

// nalezení parametru funkce v editaèním bufferu (odlišovat od ROOT funkce v editoru)
		inxedi = bufedi->Max() - 1;			// index poslední položky
		itemedi = bufedi->Data() + inxedi;	// adresa poslední položky
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

// zrušení parametru funkce
			{
				bufedi->Del(inxedi);
			}
			itemedi--;
		}
	}

// --------------- pøipojení vìtve

// nastavení odkazu na nového rodièe
	item->Parent = parent;				// odkaz na rodièe (pro ROOT bude = -1)

// napojení položky na pøedcházející položku
	item->Prev = after;					// odkaz na pøedcházející položku
	if (after >= 0)							// je pøedcházející položka platná?
	{
		item->Next = itemAft->Next;			// pøenesení následující položky
		itemAft->Next = src;				// navázání odkazu na tuto položku
	}
	else
	{
		if (parent >= 0)					// je rodiè platný?
		{
			if ((itemPar->Child < 0) && itemPar->HTree)	// je to první potomek rodièe
			{
				TV_ITEM tvi;
				tvi.hItem = itemPar->HTree;
				tvi.mask = TVIF_CHILDREN;
				tvi.cChildren = 1;
				::SendMessage(m_Tree, TVM_SETITEM, 0, (LPARAM)&tvi);
			}

			item->Next = itemPar->Child;	// pøedøazení pøed prvního potomka
			itemPar->Child = src;			// položka bude prvním potomkem rodièe
		}
		else
		{
			item->Next = m_First;			// pøedøazení pøed první ROOT položku
			m_First = src;					// položka bude první ROOT položkou
		}
	}

// napojení položky na následující položku
	if (item->Next >= 0)
	{
		m_Data[item->Next].Prev = src;
	}

// --------------- pøísun vstupního parametru

// kontrola, zda je pøisunut vstupní parametr z lokálních promìnných
	if ((m_BufID == BufLocID) &&
		(parent >= 0) &&
		(itemPar->Func == IDF_PAR) &&
		(parent != oldparent))
	{

// pøíprava indexu globální funkce
		int reffnc = BufObj.SrcDat(BufLocID, itemPar->Parent);
		if (reffnc >= 0)
		{

// nalezení funkce v editaèním bufferu
			inxedi = bufedi->Max() - 1;			// index poslední položky
			itemedi = bufedi->Data() + inxedi;	// adresa poslední položky
			for (; inxedi >= 0; inxedi--)
			{

// je to použití funkce?
				if (bufedi->m_Valid[inxedi] &&
					(itemedi->RefIndex == reffnc) &&
					(itemedi->RefBlok == BufObjID) &&
					(itemedi->Func == IDF_FNC) &&
					((itemedi->DstMask & PR_ALL) == 0))
				{

// kopie parametru funkce
					int ii = bufedi->Copy(inxedi, -2, BufLocID, src);
					ASSERT(ii >= 0);

// zákaz pøesunu parametru
					if (ii >= 0)
					{
						bufedi->At(ii).Param |= PR_NOMOVE;
					}
				}
				itemedi--;
			}
		}
	}

// --------------- zobrazení vìtve na nové pozici

// zobrazení pøenesených položek, jsou-li v zobrazené vìtvi
	if ((((parent >= 0) && (m_Data[parent].HTree != NULL)) || (m_Disp == parent)))
	{
		first = src;							// výchozí položka
		do {

// zobrazení položky
			DispNewItem(src);

// adresa položky
			item = m_Data + src;

// nalezení následující položky
			if ((src != first) || (item->Child >= 0))
			{
				src = item->Child;			// vnoøení na potomka
			}

			if (src < 0)
			{
				src = item->Next;			// pokraèování další položkou

				while (src < 0)
				{
					src = item->Parent;	// návrat k rodièi
					ASSERT(src >= 0);
					if (src == first) break;
					item = m_Data + src;	// adresa rodièe
					src = item->Next;		// další položka za rodièem
				}
			}

// dokud je další platná položka
		} while (src != first);
	}

// aktualizace stavových ikon rodièù
	AktStateImage(parent);
	if (parent != oldparent)
	{
		AktStateImage(oldparent);
	}

	return src;
}


////////////////////////////////////////////////////////////////////
// kopie vìtve stromu - vrací cílový index položky
// (parent: -1 = ROOT, pos: -1 = zaèátek, -2 = konec)

int CBufProg::Copy(int parent, int pos, int bufID, int src)
{
// lokální promìnné
	ASSERT((DWORD)bufID < (DWORD)PROGBUFNUM);
	CBufProg* buf = BufProg + bufID;			// zdrojový buffer
	PROGITEM item;								// pracovní buffer položky
	PROGITEM* itemsrc;							// adresa zdrojové položky
	PROGITEM* itemdst;							// adresa cílové položky
	PROGITEM*	itemPar;						// adresa cílového rodièe
	PROGITEM*	itemAft;						// adresa cílové pøedcházející položky
	int dst;									// index cílové položky
	int first = src;							// úschova výchozí zdrojové položky
	int result;									// výsledek
	int newparent;								// úschova cílového rodièe

// kontrola zdrojové položky
	ASSERT(buf->IsValid(src));
	if(buf->IsNotValid(src)) return -1;

// zapnutí èekacího kurzoru
	BeginWaitCursor();

// -------------- korekce ukazatelù

// kontrola platnosti nového rodièe
	if (IsNotValid(parent))						// je platný rodiè?
	{
		parent = -1;							// není platný - bude to ROOT položka
	}
	result = parent;							// výsledek
	newparent = parent;							// úschova cílového rodièe

// adresa cílového rodièe (i neplatný)
	itemPar = m_Data + parent;				// adresa rodièe (i když je neplatný)

// adresa cílové pøedcházející položky (i neplatné)
	itemAft = m_Data + pos;					// adresa pøedcházející položky (i když je neplatná)

// není-li požadována první položka, bude se hledat napojení na pøedcházející položku
	if (pos != -1)
	{

// korekce pøedešlé cílové položky, je-li v podvìtvi
		if (IsValid(pos))					// je pøedešlá položka platná?
		{
			while ((itemAft->Parent != parent) && (itemAft->Parent >= 0))
			{
				pos = itemAft->Parent;
				itemAft = m_Data + pos;			// adresa pøedcházející položky
			}
		}

// ovìøení, zda je pøedcházející položka platná (musí mít stejného rodièe)
		if (IsNotValid(pos) || (itemAft->Parent != parent))
		{	

// pøíprava výchozí pøedešlé položky - buï první potomek rodièe nebo první položka ROOT
			if (parent >= 0)					// je rodiè platný?
			{
				pos = itemPar->Child;			// první potomek rodièe
			}
			else
			{
				pos = m_First;				// jinak první položka v ROOT
			}

// nalezení poslední položky
			if (pos >= 0)						// je nìjaká položka?
			{
				itemAft = m_Data + pos;		// adresa položky

				while (itemAft->Next >= 0)		// je platná další položka?
				{
					pos = itemAft->Next;		// posun na další položku
					itemAft = m_Data + pos;	// adresa další položky
				}
			}
		}
	}

// ------------ pøíprava parametrù kopírované položky

// cyklus pøes všechny položky zdrojové vìtve
	do {

// pøi kopii skupiny z okna tøíd zajištìní naètení obsahu skupiny
		if ((bufID == BufClsID) && 
			(buf->At(src).Func == IDF_GROUP) &&
			(buf->At(src).Child < 0))
		{
			ProgLib::Load(src);
		}

// kopie obsahu položky do pracovního bufferu
		MemCopy(&item, &(buf->At(src)), SIZEOFPROGITEM);	// kopie položky

// test, zda se mají kopírovat potomci
		if (	(	(bufID == BufStrID) &&				// kopie z bufferu interních struktur
					(	(item.Func == IDF_NUM)// ||		// èíslo se kopíruje bez obsahu
//						(item.Func == IDF_WHILE_BODY)	// cyklus se kopíruje bez pøerušení
					)
				) ||
				((bufID == BufClsID) && (item.Func != IDF_GROUP)) || // tøídy se kopírují bez potomkù
				((bufID == BufLocID) && ((m_BufID == BufEdiID) || (m_BufID == BufClsID))) ||	// z lokálních objektù vše bez potomkù
				((bufID == BufObjID) && ((m_BufID == BufEdiID) || (m_BufID == BufClsID)))	// z globálních objektù vše bez potomkù
			)
		{
			item.Child = -1;							// zrušení potomkù
		}
	
// zmìna reference na zdrojové okno, je-li kopie mezi okny
		if (bufID != m_BufID)
		{
			item.RefBlok = bufID;						// blok s deklarací
			item.RefIndex = src;						// položka s deklarací
			item.DatBlok = -1;							// nejsou data
			item.DatIndex = -1;							// nejsou data

// oprava reference pøi pøetažení z okna tøíd
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

// implicitní jméno a ikona (ne pøi kopii z okna tøíd)
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
					item.Name = -1;								// implicitní jméno
					item.Icon = -1;								// implicitní ikona
				}
			}
		}

// pøi kopii z okna struktur rozvinutí položky
		if ((bufID == BufStrID) && (item.Child >= 0))
		{
			item.Param |= PR_EXP;
		}

// pøi kopii z okna tøíd zrušení zámku a vypnutí
		if (bufID == BufClsID)
		{
			item.Param &= ~(PR_LOCK | PR_OFF);

			if (m_BufID != BufClsID)
			{
				item.Param &= ~PR_ALTEXT;
			}
		}

// u první položky zrušení zámku
		if (src == first)
		{
			item.Param &= ~PR_LOCK;
		}

// zrušení interního pøíznaku a pøíznaku nové položky a pøíznaku alternativní knihovny
		item.Param &= ~(PR_INTERN | PR_NEW | PR_CD | PR_CD2);

// korekce parametrù indexu seznamu a sprajtu
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

// základní ikona se mìní na bìžnou ikonu
		if (item.Func == IDF_NONE)
		{
			item.Func = IDF_ICON;
		}

// ----------- duplikace dat položky

// zrušení textu textové a èíselné konstanty a komentáøe pøi kopii z okna struktur
		if (((item.Func == IDF_TEXT_CONST) || 
			(item.Func == IDF_NUM) || 
			(item.Func == IDF_COMMENT)) && 
			(bufID == BufStrID))
		{
			item.Name = Text.New();
		}
		else

// duplikace textu, je-li platný (ne pro textovou konstantu)
		{
			if (item.Name >= 0)
			{
				item.Name = Text.Dup(item.Name);
			}
		}

// duplikace dat obrázku pøi kopii uvnitø/do okna objektù
		if ((item.Func == IDF_ICON) && ((m_BufID == BufObjID) || (m_BufID == BufLocID)))
		{
			if ((bufID == BufClsID) && (item.Icon >= 0))
			{
				item.DatIndex = item.Icon;
			}

			item.DatBlok = BufIcoID;			// data v bufferu obrázkù
			if (item.DatIndex < 0)				// je nìjaký obrázek pøidìlen?
			{
				item.DatIndex = 0;				// implicitní ikona 0 (podklad)
				item.Param |= PR_NEW;			// pøíznak nové položky
			}
			item.DatIndex = Icon.Dup(item.DatIndex); // duplikace obrázku
			item.Icon = item.DatIndex;			// bude to souèasnì ikona
		}

// duplikace ikony barevné konstanty
		if (item.Func == IDF_COLOR)
		{
			item.Icon = Icon.Dup(buf->GetIcon(src));
		}
		else

// duplikace ikony pøi kopii uvnitø okna (ne pro barevnou konstantu ani pro pøedmìt)
		{
			if ((item.Icon >= 0) && ((item.Func != IDF_ICON) || (m_BufID == BufClsID)))
			{
				item.Icon = Icon.Dup(item.Icon);
			}
		}

// zajištìní dat pøi kopii z knihovny tøíd (ne pro ikonu - je již naètena)
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

// duplikace dat (pokud jsou) - vèetnì obsahu funkce
		{
			CopyDupData(&item);
		}

// jméno souboru pøi pøetažení z okna tøíd se mùže zrušit
		if ((item.Name >= 0) && (m_BufID != BufClsID))
		{
			Text[item.Name].MultiText(JAZYK000, EmptyText);
		}

// zajištìní nových dat pro položku - vèetnì obsahu funkce
		CopyNewData(&item);

// -------------- zajištìní jedineènosti jména

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

					// implicitní jméno parametrù sprajtu (pøi kopii z okna tøíd)
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

// -------------- uložení položky

// kopie položky do cílového bufferu (pozor - Insert mùže pohnout zdrojovou i cílovou adresou)
		dst = Insert(&item, parent, pos);
		if (src == first) result = dst;			// výsledný index
		pos = -2;								// pøíštì již budou položky na konec

// -------------- vytvoøení parametrù sprajtu

// pøi kopii sprajtu z okna tøíd pøidání parametrù
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

// -------------- pøi kopii globální funkce kopie též vstupních parametrù

// kontrola, zda se kopíruje globální funkce z objektù do editoru
		if ((item.Func == IDF_FNC) &&
			(m_BufID == BufEdiID) && 
			(bufID == BufObjID) &&
			(BufObj[src].DatBlok == BufLocID) &&
			(BufObj[src].DatIndex >= 0))
		{

// nalezení definice vstupních promìnných
			int inx = BufObj[src].DatIndex;
			PROGITEM* itm;
			do 
			{
				inx = BufLoc.NextItem(inx);
				itm = &BufLoc[inx];
			} while ((itm->Func != IDF_PAR) && (itm->Parent >= 0));
			inx = itm->Child;

// kopie vstupních promìnných
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

// --------------- vytvoøení vstupního parametru (pozor na kopii celé lokální definice)

// kontrola, zda je vytvoøen vstupní parametr 
		if ((m_BufID == BufLocID) &&
			(parent >= 0) &&
			(m_Data[parent].Func == IDF_PAR))
		{

// pøíprava indexu globální funkce
			int reffnc = BufObj.SrcDat(BufLocID, m_Data[parent].Parent);
			if (reffnc >= 0)
			{

// nalezení funkce v editaèním bufferu
				CBufProg* bufedi = &BufEdi;
				int inxedi = bufedi->Max() - 1;					// index poslední položky
				PROGITEM* itemedi = bufedi->Data() + inxedi;	// adresa poslední položky
				for (; inxedi >= 0; inxedi--)
				{

// je to použití funkce?
					if (bufedi->m_Valid[inxedi] &&
						(itemedi->RefIndex == reffnc) &&
						(itemedi->RefBlok == BufObjID) &&
						(itemedi->Func == IDF_FNC) &&
						((itemedi->DstMask & PR_ALL) == 0))
					{

// kopie parametru funkce
						int ii = bufedi->Copy(inxedi, -2, BufLocID, dst);
						ASSERT(ii >= 0);

// zákaz pøesunu parametru
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

// pøi kopii funkce kopie definice programu
		if (((item.DatBlok == BufLocID) || (item.DatBlok == BufEdiID)) &&
			(m_BufID == bufID) && (item.DatIndex == m_Data[dst].DatIndex))
		{
			int srcroot = item.DatIndex;
			ASSERT(srcroot >= 0);
			int dstroot = BufProg[item.DatBlok].Copy(-1, -2, item.DatBlok, srcroot);
			m_Data[dst].DatIndex = dstroot;
			ASSERT(dstroot >= 0);

// pøi kopii globální funkce korekce referencí na lokální promìnné
			if ((item.DatBlok == BufLocID) && (m_BufID == BufObjID))
			{

// pøíprava ukazatelù pro vyhledávání definic funkcí
				CBufProg* bufloc = &BufLoc;		// adresa bufferu lokálních objektù
				int locinx = dstroot;			// index v lokální definici pøi vyhledávání funkcí
				PROGITEM* locitem;				// položka v lokální definici pøi vyhledávání
				locitem = &bufloc->At(locinx);	// adresa položky

// pøíprava ukazatelù pro vyhledávání lokálních promìnných v editoru
				CBufProg* bufedi = &BufEdi;		// adresa bufferu editoru
				int ediinx;						// index v bufferu editoru
				PROGITEM* ediitem;				// adresa v bufferu editoru

// pøíprava ukazatelù pro reindexaci odkazu na lokální promìnnou
				int refinx;						// referenèní index hledané položky
				int num;						// relativní pozice položky v lokální definici
				int inx;						// ukazatel indexu v lokální definici
				PROGITEM* itm;					// položka v lokální definici

// cyklus vyhledávání definic funkcí v cílové lokální definici (pozor - obsloužit i root)
				do 
				{

// test, zda je to definice funkce
					ediinx = locitem->DatIndex;
					if ((locitem->DatBlok == BufEdiID) && 
						(bufedi->IsValid(ediinx)))
					{
						ediitem = &bufedi->At(ediinx);		// adresa položky

// cyklus vyhledávání lokálních promìnných v editoru
						do 
						{

// test, zda je to odkaz na lokální položku a zda to není parametr funkce
							refinx = ediitem->RefIndex;
							if ((ediitem->RefBlok == BufLocID) &&
								bufloc->IsValid(refinx) &&
								(ediitem->Parent >= 0) &&
								(((bufedi->At(ediitem->Parent).DstMask & PR_ALL) != 0) || (bufedi->At(ediitem->Parent).Func != IDF_FNC)))
							{

// zjištìní relativní pozice položky ve zdrojové lokální definici
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
				
// kontrola nalezení položky
								if (itm->Parent >= 0)
								{

// nalezení odpovídající zkopírované položky
									inx = dstroot;
									for (; num > 0; num--)
									{
										inx = bufloc->NextItem(inx);
									}

// pøesmìrování ukazatele
									itm->Refer--;
									ediitem->RefIndex = inx;
									bufloc->At(inx).Refer++;
								}
							}

// posun ukazatele v editoru na další položku
							ediinx = bufedi->NextItem(ediinx);	// index další položky
							ASSERT(ediinx >= 0);
							ediitem = &bufedi->At(ediinx);		// adresa položky
						} while (ediitem->Parent >= 0);			// dokud není další definice
					}

// posun ukazatele v lokální definici na další položku
					locinx = bufloc->NextItem(locinx);	// index další položky
					ASSERT(locinx >= 0);
					locitem = &bufloc->At(locinx);		// adresa položky
				} while (locitem->Parent >= 0);			// dokud není další definice
			}
		}

// ---------- další položka

// pøíprava indexu další položky
		itemdst = &At(dst);
		itemsrc = &buf->At(src);
		if ((src != first) || (item.Child >= 0))
		{
			src = item.Child;				// vnoøení na potomka
			if (src < 0)					// není žádný potomek?
			{
				src = itemsrc->Next;		// pokraèování další položkou

				while ((src < 0) && (itemsrc->Parent >= 0))
				{
					src = itemsrc->Parent;	// návrat zdroje k rodièi
					dst = parent;			// návrat cíle k rodièi
					itemdst = &At(dst);		// adresa cílové položky
					parent = itemdst->Parent; // nový rodiè
					if (src == first) break; // je celá vìtev
					itemsrc = &buf->At(src); // adresa rodièe
					src = itemsrc->Next;	// další položka za rodièem
				}
			}
			else
			{
				parent = dst;				// jinak tato položka bude rodièem
			}

// pøi kopii z okna tøíd pøeskoèení skupin
			if (bufID == BufClsID)
			{
				while ((src >= 0) && (src != first) && (buf->At(src).Func == IDF_GROUP))
				{
					src = buf->At(src).Next;
				}

				if (src < 0) src = first;
			}
		}

// až po výchozí položku
	} while (src != first);

// aktualizace stavové ikony rodièe
	AktStateImage(newparent);

// vypnutí èekacího kurzoru
	EndWaitCursor();

	return result;
}


////////////////////////////////////////////////////////////////////
// duplikace dat u kopírované položky (ne pro ikonu a ne pro funkci)

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
// zajištìní nových dat u kopírované položky (je voláno i z Clipboard::Paste)

void CBufProg::CopyNewData(PROGITEM* item)
{
// test, zda data nejsou vytvoøena a zda je to buffer objektù
	if (((item->DatBlok >= 0) && (item->DatIndex >= 0)) ||
		((m_BufID != BufObjID) && (m_BufID != BufLocID)))
		return;

// vytvoøení nových dat pro promìnné
	switch (item->Func)
	{

// èíselná promìnná
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

// textová promìnná
	case IDF_TEXT:		item->DatBlok = BufTxtID;
						item->DatIndex = Text.New();
						item->Param |= PR_NEW;
						break;

// logická promìnná
	case IDF_SPRITE_VISIBLE:
	case IDF_SPRITE_MOVE:
	case IDF_BOOL:		item->DatBlok = BufLogID;
						item->DatIndex = Bool.New();
						item->Param |= PR_NEW;
						break;

// promìnná plochy
	case IDF_MAP:		item->DatBlok = BufMapID;
						ASSERT(Map.IsValid(0));
						item->DatIndex = Map.New(Map[0].Width(), Map[0].Height());
						Map[item->DatIndex].RefInc();
						item->Param |= PR_NEW;
						break;

// promìnná obrázku
	case IDF_PIC:		item->DatBlok = BufPicID;
						ASSERT(Map.IsValid(0));
						item->DatIndex = Picture.New(Map[0].Width()*ICONWIDTH, Map[0].Height()*ICONHEIGHT);
						item->Param |= PR_NEW;
						break;

// promìnná sprajtu
	case IDF_SPRITE:	item->DatBlok = BufSprID;
						item->DatIndex = Sprite.New();
						item->Param |= PR_NEW;
						break;

// promìnná zvuku
	case IDF_SND:		item->DatBlok = BufSndID;
						item->DatIndex = Sound.New();
						item->Param |= PR_NEW;
						break;

// promìnná hudby
	case IDF_MUS:		item->DatBlok = BufMusID;
						item->DatIndex = Music.New();
						item->Param |= PR_NEW;
						break;

// funkce
	case IDF_FNC:
		{
			item->Param |= PR_NEW;

// vytvoøení definice v editaèním bufferu
			PROGITEM item2;								// pracovní položka
			MemFill(&item2, SIZEOFPROGITEM, -1);		// implicitní hodnoty položek
			item2.Func = IDF_FNC;						// identifikace funkce
			item2.SrcMask = 0;							// maska zdrojových vlastností
			item2.DstMask = PR_COMMAND;					// maska cílových vlastností
			item2.Param = PR_EXP | PR_NOMOVE;			// parametry
			item2.RefBlok = BufIntID;					// reference na interní buffer
			item2.RefIndex = IDF_FNC - IDF;				// referenèní index
			item2.DatIndex = BufEdi.Insert(&item2, -1, -2);	// vložení položky do bufferu

// inicializace odkazu na definici z lokálního bufferu
			if (m_BufID == BufLocID)
			{
				item->DatBlok = BufEdiID;				// index bufferu
				item->DatIndex = item2.DatIndex;		// index položky definice
			}
			else

// vytvoøení deklarace v lokálním bufferu (je-li kopie do globálního bufferu)
			{
				item2.DstMask = PR_ALLDATA;				// maska cílových vlastností
				item2.DatBlok = BufEdiID;				// odkaz na editaèní buffer
				item->DatIndex = BufLoc.Insert(&item2, -1, -2); // vložení položky do bufferu
				item->DatBlok = BufLocID;				// lokální buffer

// vložení záhlaví pro vstupní parametry
				item2.Func = IDF_PAR;					// vstupní parametry
				item2.DstMask = PR_ALLDATA;				// maska cílových parametrù
				item2.Param = PR_NOMOVE;				// parametry
				item2.DatBlok = -1;						// není blok s daty
				item2.DatIndex = -1;					// nejsou data
				item2.RefIndex = IDF_PAR - IDF;			// index v interním bufferu
				BufLoc.Insert(&item2, item->DatIndex, -2);	// vložení položky do bufferu

// vložení záhlaví pro výstupní parametr
				item2.Func = IDF_OUT;					// výstupní parametr
				item2.Param = PR_NOMOVE | PR_ONE;		// parametry
				item2.RefIndex = IDF_OUT - IDF;			// index v interním bufferu
				BufLoc.Insert(&item2, item->DatIndex, -2);	// vložení položky do bufferu
			}
		}
	}
}
