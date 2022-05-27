
#include "Main.h"

/***************************************************************************\
*																			*
*								Obsluha clipboardu							*
*																			*
\***************************************************************************/

DWORD	CF_PETPROGP = (DWORD)-1;				// ID formátu "Program"
DWORD	CF_PETPROGG = (DWORD)-1;				// ID formátu "Globální"
DWORD	CF_PETPROGL = (DWORD)-1;				// ID formátu "Lokální"

namespace ProgClip
{

// záhlaví clipboardu - program
const CLIPHEAD ClipHeadP = {
	'P','E','T',									// identifikace
	1,												// verze
	VerzeCom + VerzeRel*10 + VerzeMin*100 + VerzeMaj*1000, // verze editoru
	0,												// parametry
	SIZEOFCLIPHEAD,									// offset zaèátku dat
	1,												// poèet datových blokù
	0,0,'C','L','I','P','B','R','D','P',0,0,0,0,
};

// záhlaví clipboardu - globální
const CLIPHEAD ClipHeadG = {
	'P','E','T',									// identifikace
	1,												// verze
	VerzeCom + VerzeRel*10 + VerzeMin*100 + VerzeMaj*1000, // verze editoru
	0,												// parametry
	SIZEOFCLIPHEAD,									// offset zaèátku dat
	1,												// poèet datových blokù
	0,0,'C','L','I','P','B','R','D','G',0,0,0,0,
};

// záhlaví clipboardu - lokální
const CLIPHEAD ClipHeadL = {
	'P','E','T',									// identifikace
	1,												// verze
	VerzeCom + VerzeRel*10 + VerzeMin*100 + VerzeMaj*1000, // verze editoru
	0,												// parametry
	SIZEOFCLIPHEAD,									// offset zaèátku dat
	1,												// poèet datových blokù
	0,0,'C','L','I','P','B','R','D','L',0,0,0,0,
};

/////////////////////////////////////////////////////////////////////////////
// parametry

BOOL		Lock = FALSE;				// pøíznak probíhající obsluhy

// buffer pro uložení
BYTE*		m_Buf = NULL;				// datový buffer
int			m_Num = 0;					// poèet bajtù v bufferu, pøi ètení ètecí offset
int			m_Max = 0;					// velikost bufferu, pøi ètení velikost dat

CMultiText	m_Text;						// buffer k naètení/uložení textu
CIcon		m_Icon;						// buffer k naètení/uložení ikony
CPicture	m_Pic;						// buffer k naètení/uložení obrázku
CMap		m_Map;						// buffer k naètení/uložení plochy
CSprite		m_Sprite;					// buffer k naètení/uložení sprajtu
CSound		m_Sound;					// buffer k naètení/uložení zvuku
CMusic		m_Music;					// buffer k naètení/uložení hudby

int			m_LocFirst;					// výchozí položka v lokálním bufferu

/////////////////////////////////////////////////////////////////////////////
// inicializace obsluhy clipboardu

void StartInit()
{
	CF_PETPROGP = ::RegisterClipboardFormat(_T("PetProgP") VerzeTxt0);
	CF_PETPROGG = ::RegisterClipboardFormat(_T("PetProgG") VerzeTxt0);
	CF_PETPROGL = ::RegisterClipboardFormat(_T("PetProgL") VerzeTxt0);

	ASSERT(CF_PETPROGP);
	ASSERT(CF_PETPROGG);
	ASSERT(CF_PETPROGL);
}


/////////////////////////////////////////////////////////////////////////////
// pøidání dat do výstupního bufferu (vrací FALSE=chyba pamìti)

BOOL Add(const void* src, const int size)
{
	if (size + m_Num > m_Max)
	{
		int max = (m_Num + size + 0x2000) & ~0x0fff;
		BYTE* buf = (BYTE*)MemSize(m_Buf, max);
		if (buf == NULL) return FALSE;
		m_Buf = buf;
		m_Max = max;
	}

	MemCopy(m_Buf + m_Num, src, size);
	m_Num += size;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// pøidání vícejazyèného textu do výstupního bufferu, vrací délku v bajtech, -1=chyba pamìti

int AddText(int inx)
{
// text k uložení (mùže být i neplatný)
	m_Text = Text.Get(inx);
	CText txt;

	int len = 0;

// cyklus pøes všechny jazyku
	for (int lan = 0; lan < JAZYKNUM; lan++)
	{

// text k uložení
		txt = m_Text.MultiText(lan);

// délka textu v bajtech
		long len0 = txt.Length();

// pøidání délky
		BOOL ret = Add(&len0, sizeof(long));
		if (!ret) return -1;
		len += sizeof(long);

// pøidání textu
#ifdef _UNICODE
		char* buf = (char*)MemGet(len0 + 10);
		if (buf == NULL) return -1;
		txt.Write(buf);
		ret = Add(buf, len0);
		MemFree(buf);
#else
		ret = Add(txt.DataData(), len0);
#endif
		if (!ret) return -1;
		len += len0;
	}

// délka celého textu
	return len;
}


/////////////////////////////////////////////////////////////////////////////
// pøidání položky programu (vrací poèet pøenesených prvkù ve vìtvi, -1=chyba pamìti)

int AddProg(int bufID, int index, BOOL isfirst)
{
// lokální promìnné
	PETPROG dst;				// cílová položka k uložení
	int dstinx;					// offset cílové položky v bufferu
	PETPROG* pet;				// adresa cílové položky v bufferu
	int inx;					// pomocný index zdrojové položky
	int size;					// pomocná délka dat položky
	int count = 0;				// èítaè vložených prvkù
	int first = index;			// úschova indexu výchozí položky
	PROGITEM* src;				// adresa zdrojové položky
	CBufProg* buf = BufProg + bufID;	// adresa bufferu
	int i;						// pomocný registr

// cyklus pøes celou vìtev
	do 
	{
		ASSERT(buf->IsValid(index));
		if (buf->IsNotValid(index)) break;
		count++;					// zvýšení èítaèe prvkù

// adresa zdrojové položky
		src = &buf->At(index);

// pøíprava parametrù položky
		dst.Param = 0;
		if (src->Child >= 0)		dst.Param |= PETPROG_CHILDS;
		if (src->Next >= 0)			dst.Param |= PETPROG_NEXT;
		if (src->Param & PR_EXP)	dst.Param |= PETPROG_EXP;
		if (src->Param & PR_LOCK)	dst.Param |= PETPROG_LOCK;
		if (src->Param & PR_OFF)	dst.Param |= PETPROG_OFF;
		if (src->Param & PR_NOMOVE)	dst.Param |= PETPROG_NOMOVE;
		if (src->Param & PR_INTERN)	dst.Param |= PETPROG_INTERN;

// u první položky vìtve zrušení zámku
		if ((index == first) && isfirst) dst.Param &= ~PETPROG_LOCK;

// uložení položky programu
		dstinx = m_Num;								// offset cílové položky
		if (!Add(&dst, SIZEOFPETPROG)) return -1;	// uložení položky do bufferu

// referenèní text
		pet = (PETPROG*)(m_Buf + dstinx);			// adresa cílové položky
		pet->RefIndex = 0;							// délka jména referenèní položky
		pet->RefBlok = -1;							// neplatný referenèní blok

		switch (src->RefBlok)						// podle referenèního bloku
		{
		case BufObjID:
		case BufLocID:
			i = AddText(BufProg[src->RefBlok].GetText(src->RefIndex));
			if (i < 0) return -1;
			pet = (PETPROG*)(m_Buf + dstinx);		// adresa cílové položky
			pet->RefIndex = i;						// délka textu v bajtech
			pet->RefBlok = src->RefBlok;			// èíslo referenèního bloku
			break;
		}

// uložení datové položky (a délky dat)
		pet = (PETPROG*)(m_Buf + dstinx);
		pet->DatBlok = src->DatBlok;
		inx = src->DatIndex;						// index požadované položky
		int ninx = m_Num;							// úschova offsetu zaèátku dat

		switch (src->DatBlok)
		{
// lokální funkce, funkce
		case BufLocID:
		case BufEdiID:
			{
				ASSERT(BufProg[src->DatBlok].IsValid(inx));
				long cnt;							// poèet prvkù
				int cntn = m_Num;					// úschova offsetu èítaèe
				if (!Add(&cnt, sizeof(long))) return -1;// uložení èítaèe
				cnt = AddProg(src->DatBlok, inx, FALSE); // uložení lokální definice
				if (cnt < 0) return -1;				// chyba pamìti
				*(long*)(m_Buf + cntn) = cnt;		// poèet prvkù				
			}
			break;

// èíslo
		case BufNumID:
			{
				double num = Real.Get(inx);
				if (!Add(&num, sizeof(double))) return -1;
			}
			break;

// text
		case BufTxtID:
			if (AddText(inx) < 0) return -1;
			break;

// logická promìnná
		case BufLogID:
			{
				long log = Bool.Get(inx);
				if (!Add(&log, sizeof(long))) return -1;
			}
			break;

// ikona
		case BufIcoID:
			m_Icon = Icon.Get(inx);
			if (!m_Icon.DeComp() || !Add(m_Icon.DataData(), ICONSIZE)) return -1;
			break;

// plocha
		case BufMapID:
			{
				m_Map = Map.Get(inx);
				if (!Add(&m_Map.Data()->Width, sizeof(long))) return -1;
				if (!Add(&m_Map.Data()->Height, sizeof(long))) return -1;

				long reficon = 0;					// èítaè referencí ikon
				int nreficon = m_Num;				// offset referencí ikon
				if (!Add(&reficon, sizeof(long))) return -1; // uložení èítaèe referencí

				size = m_Map.Width() * m_Map.Height();	// poèet položek
				CBufIndex bufinx;					// buffer mapování ikon
				if (!bufinx.NumClear(Icon.Num())) return -1;

				int icon;							// pomocné èíslo ikony
				int ref;							// index definice objektu ikony
				CBufProg* bf;						// adresa bufferu s definicí
				int i;								// index prvku plochy
				ASSERT(Icon.IsValid(0));

				for (i = 0; i < size; i++)
				{
					icon = m_Map[i].Icon;			// ikona
					if (Icon.IsNotValid(icon))		// je ikona platná?
					{
						icon = 0;					// náhradní ikona
					}

					if (bufinx[icon] < 0)			// je ikona zmapovaná?
					{
						bufinx[icon] = reficon;		// náhradní index ikony
						reficon++;					// zvýšení èítaèe referencí

						bf = &BufObj;
						ref = bf->SrcDat(BufIcoID, icon);

						if ((ref < 0) && (bufID == BufLocID))
						{
							bf = &BufLoc;
							ref = bf->SrcDat(BufIcoID, icon);
						}

						ref = bf->GetText(ref);
						if (AddText(ref) < 0) return -1;	// (pro neplatný text se uloží prázdný text)
					}
				}
				*(long*)(m_Buf + nreficon) = reficon; // nastavení poètu ikon

				for (i = 0; i < size; i++)
				{
					icon = m_Map[i].Icon;
					if (Icon.IsNotValid(icon))		// je ikona platná?
					{
						icon = 0;					// náhradní ikona
					}

					if (!Add(&bufinx[icon], sizeof(long)) ||
						!Add(&m_Map[i].Param, sizeof(DWORD))) return -1;
				}
			}
			break;

// obrázek
		case BufPicID:
			m_Pic = Picture.Get(inx);
			if (!Add(&m_Pic.Data()->Width,	sizeof(long))	||
				!Add(&m_Pic.Data()->Height,	sizeof(long))	||
				!m_Pic.DeComp()								||
				!Add(m_Pic.DataData(),		m_Pic.Size())	) return -1;
			break;

// sprajt
		case BufSprID:
			{
				m_Sprite = Sprite.Get(inx);
				SPRITEDATA* spritedata = m_Sprite.Data();
				int num = spritedata->Faze * spritedata->Smer;
				if (!Add(&spritedata->Faze,				sizeof(long))	||
					!Add(&spritedata->Smer,				sizeof(long))	||
					!Add(&spritedata->Klid,				sizeof(long))	||
					!Add(&m_Sprite[0].Data()->Width,	sizeof(WORD))	||
					!Add(&m_Sprite[0].Data()->Height,	sizeof(WORD))	||
					!Add(&spritedata->Delay,			sizeof(long))	||
					!Add(&spritedata->Level,			sizeof(long))	||
					!Add(&spritedata->Kroku,			sizeof(double))	) return -1;
				size = m_Sprite[0].Size();
				for (int pic = 0; pic < num; pic++)
				{
					if (!m_Sprite[pic].DeComp() ||
						!Add(m_Sprite[pic].DataData(), size)) return -1;
				}
			}
			break;

// zvuk
		case BufSndID:
			{
				m_Sound = Sound.Get(inx);
				SOUNDDATA* sounddata = m_Sound.Data();
				if (!Add(&sounddata->Samples,	sizeof(DWORD))	||
					!Add(&sounddata->Format,	sizeof(WORD))	||
					!Add(&sounddata->Channels,	sizeof(BYTE))	||
					!Add(&sounddata->Bits,		sizeof(BYTE))	||
					!Add(m_Sound.DataData(),	m_Sound.Size())	) return -1;
			}
			break;

// hudba
		case BufMusID:
			m_Music = Music.Get(inx);
			if (!Add(m_Music.DataData(), m_Music.Size())) return -1;
			break;
		}

// nastavení délky dat
		pet = (PETPROG*)(m_Buf + dstinx);				// adresa záhlaví
		pet->DatIndex = m_Num - ninx;					// délka dat

// uložení ikony
		pet->Icon = -1;									// implicitní ikona
		inx = src->Icon;								// index ikony
		if (Icon.IsValid(inx))							// je platná ikona?
		{
			pet->Icon = ICONSIZE;						// velikost ikony
			m_Icon = Icon.Get(inx);
			if (!m_Icon.DeComp() || !Add(m_Icon.DataData(), ICONSIZE)) return -1;
		}

// uložení textu jména položky
		pet = (PETPROG*)(m_Buf + dstinx);
		pet->Name = -1;									// implicitní jméno
		inx = src->Name;								// index jména
		if (Text.IsValid(inx))							// je platné jméno?
		{
			int i = AddText(inx);						// uložení textu
			if (i < 0) return -1;
			pet = (PETPROG*)(m_Buf + dstinx);
			pet->Name = i;
		}

// uložení èísla funkce
		pet->Func = src->Func - IDF;

// posun na potomka položky
		if ((index != first) || (src->Child >= 0))
		{
			index = src->Child;
		}

// není-li potomek, pøesun na následující položku
		if (index < 0)					// je potomek?
		{
			index = src->Next;			// pokraèování další položkou

// není-li další položka, vynoøení k rodièi
			while ((index < 0) && (src->Parent >= 0))
			{
				index = src->Parent;	// návrat k rodièi
				if (index == first) break; // je již opìt výchozí položka
				src = &buf->At(index);	// adresa rodièe
				index = src->Next;		// další položka za rodièem
			}
		}

// dokud není dosaženo výchozí položky
	} while (index != first);

	return count;						// poèet uložených položek
}


/////////////////////////////////////////////////////////////////////////////
// kopie do clipboardu (TRUE=operace OK)

BOOL Copy(int bufID)
{
// kontrola, zda neprobíhá obsluha
	if (Lock) return FALSE;

// kontrola povolených bufferù
	if ((bufID != BufObjID) &&
		(bufID != BufLocID) &&
		(bufID != BufEdiID))
	{
		return FALSE;
	}

// zapnutí pøíznaku obsluhy
	Lock = TRUE;

// pøíprava výchozí položky k uložení
	CBufProg* buf = BufProg + bufID;
	int first = buf->Select();
	if (first < 0)
	{
		Lock = FALSE;
		return FALSE;
	}

// kontrola platnosti položky 
// (nelze kopírovat ani interní globální objekty - musely by se zajistit parametry sprajtu)
	if (buf->At(first).Param & (PR_NOMOVE | PR_INTERN))
	{
		Lock = FALSE;
		return FALSE;
	}

// zapnutí èekacího kurzoru
	BeginWaitCursor();

// otevøení schránky
	if (!::OpenClipboard(MainFrame))
	{
		EndWaitCursor();
		Lock = FALSE;
		return FALSE;
	}

// vyprázdnìní schránky
	if (!::EmptyClipboard())
	{
		EndWaitCursor();
		::CloseClipboard();
		Lock = FALSE;
		UpdateClipboard();
		return FALSE;
	}

// pøíprava bufferu
	MemFree(m_Buf);
	m_Buf = NULL;
	m_Num = 0;
	m_Max = 0;

// vytvoøení záhlaví bufferu
	BOOL res;
	switch (bufID)
	{
	case BufObjID:
		res = Add(&ClipHeadG, SIZEOFCLIPHEAD);
		break;

	case BufLocID:
		res = Add(&ClipHeadL, SIZEOFCLIPHEAD);
		break;

	default:
		res = Add(&ClipHeadP, SIZEOFCLIPHEAD);
	}

	if (!res)
	{
		EndWaitCursor();
		::CloseClipboard();
		MemFree(m_Buf);
		m_Buf = NULL;
		Lock = FALSE;
		UpdateClipboard();
		return FALSE;
	}

// uložení vìtve programu
	int count = AddProg(bufID, first, TRUE);
	if (count < 0)
	{
		EndWaitCursor();
		::CloseClipboard();
		MemFree(m_Buf);
		m_Buf = NULL;
		Lock = FALSE;
		UpdateClipboard();
		return FALSE;
	}

	((CLIPHEAD*)m_Buf)->Clip.Pocet = count;
	((CLIPHEAD*)m_Buf)->Clip.Delka = m_Num - SIZEOFCLIPHEAD;

// vytvoøení globálního bufferu pro data
	HGLOBAL global = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, m_Num);
	if (global == NULL)
	{
		EndWaitCursor();
		::CloseClipboard();
		MemFree(m_Buf);
		m_Buf = NULL;
		Lock = FALSE;
		UpdateClipboard();
		return FALSE;
	}

// uzamknutí bufferu
	BYTE* data = (BYTE*) ::GlobalLock(global);
	if (data == NULL)
	{
		::GlobalFree(global);
		EndWaitCursor();
		::CloseClipboard();
		MemFree(m_Buf);
		m_Buf = NULL;
		Lock = FALSE;
		UpdateClipboard();
		return FALSE;
	}

// kopie dat do bufferu
	MemCopy(data, m_Buf, m_Num);

// odemknutí bufferu
	::GlobalUnlock(global);

// uložení dat do schránky
	HANDLE handle;
	switch (bufID)
	{
	case BufObjID:
		handle = ::SetClipboardData(CF_PETPROGG, global);
		break;

	case BufLocID:
		handle = ::SetClipboardData(CF_PETPROGL, global);
		break;

	default:
		handle = ::SetClipboardData(CF_PETPROGP, global);
	}

	if (handle == NULL) ::GlobalFree(global);

// uzavøení schránky
	::CloseClipboard();

// zrušení bufferu
	MemFree(m_Buf);
	m_Buf = NULL;

// vypnutí èekacího kurzoru
	EndWaitCursor();

// vypnutí pøíznaku obsluhy
	Lock = FALSE;

// aktualizace voleb bloku
	UpdateClipboard();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// naètení dat z bufferu

void Get(void* dst, int size)
{
	if (size < 0) size = 0;

	if (m_Num + size > m_Max)
	{
		MemFill(dst, size, 0);
		MemCopy(dst, m_Buf + m_Num, m_Max - m_Num);
		m_Num = m_Max;
	}
	else
	{
		MemCopy(dst, m_Buf + m_Num, size);
		m_Num += size;
	}
}


/////////////////////////////////////////////////////////////////////////////
// naètení textu z bufferu

void GetText(int size)
{
	if (size < 0) size = 0;

	if (m_Num + size > m_Max)
	{
		size = m_Max - m_Num;
	}

	CText txt;

	for (int lan = 0; lan < JAZYKNUM; lan++)
	{
		if (size < sizeof(long)) return;
		long len = *(long*)(m_Buf + m_Num);
		m_Num += sizeof(long);
		size -= sizeof(long);
		if (len > size) len = size;

		txt.Term();
		txt.Init((char*)(m_Buf + m_Num), len);
		m_Num += len;
		size -= len;

		m_Text.MultiText(lan, txt);
	}
}


/////////////////////////////////////////////////////////////////////////////
// naètení ikony z bufferu

void GetIcon(int size)
{
	if (size < 0) size = 0;

	if (m_Num + size > m_Max)
	{
		size = m_Max - m_Num;
	}

	int size2 = size;
	if (size2 > ICONSIZE) size2 = ICONSIZE;

	if (m_Icon.New(ICONSIZE))
	{
		MemCopy(m_Icon.DataData(), m_Buf + m_Num, size2);
	}
	m_Icon.Comp();

	m_Num += size;
}


/////////////////////////////////////////////////////////////////////////////
// naètení obrázku z bufferu

void GetPicture(int width, int height)
{
	if (width < 1) width = 1;
	if (height < 1) height = 1;

	int size = width*height;
	if (size < 0) size = 0;

	if (m_Num + size > m_Max)
	{
		size = m_Max - m_Num;
	}

	if (m_Pic.New(width, height))
	{
		MemCopy(m_Pic.DataData(), m_Buf + m_Num, size);
	}
	m_Pic.Comp();

	m_Num += size;
}


/////////////////////////////////////////////////////////////////////////////
// naètení programu z bufferu (vrací index položky, -1=chyba pamìti)

int GetProg(int bufID, int prev, int num)
{
// lokální promìnné
	CBufProg* buf = BufProg + bufID;	// adresa bufferu
	PETPROG src;						// ètená položka
	PROGITEM dst;						// ukládaná položka
	int* parent;						// buffer odkazù na rodièe
	BOOL* next;							// buffer pøíznakù dalších prvkù
	int parmax;							// velikost bufferu rodièù
	int parnum;							// poèet rodièù
	int blok;							// pomocné èíslo bloku
	int inx;							// pomocné èíslo indexu
	int result = -1;					// výsledný index
	CBufProg* bufref;					// referenèní buffer
	int reffrst;						// první položka k hledání reference
	int refnext;						// pøíští položka k hledání reference
	int nn;								// pomocné èíslo jména
	BOOL inpar;							// položka je vstupním parametrem
	BOOL newpar;						// pøíznak, že byl vytvoøen vstupní parametr
	BOOL isloc;							// pøíznak založení lokální definice
	int inxloc = 0;						// index lokální definice

// vypnutí pøekreslování
	buf->RedrawOff();

// pøíprava bufferu rodièù
	parmax = 1024;
	parnum = 0;

	parent = (int*)MemGet(1024*sizeof(int));
	if (parent == NULL)
	{
		buf->RedrawOn();
		return -1;
	}
	parent[0] = buf->Disp();

	next = (BOOL*)MemGet(1024*sizeof(BOOL));
	if (next == NULL)
	{
		MemFree(parent);
		buf->RedrawOn();
		return -1;
	}
	next[0] = TRUE;

	if (buf->IsValid(prev))
	{
		parent[0] = buf->At(prev).Parent;	// rodiè
	}

// naètení položek
	for (; num > 0; num--)
	{
// není to vstupní parametr funkce
		inpar = FALSE;
		newpar = FALSE;
		isloc = FALSE;

// naètení záhlaví položky
		Get(&src, SIZEOFPETPROG);

// èíslo funkce
		dst.Func = IDF_COMMENT;
		if ((DWORD)src.Func < (DWORD)FncNum)
		{
			dst.Func = src.Func + IDF;
		}

// v bufferu objektù se implicitní ikona mìní na bìžnou
		if ((bufID != BufEdiID) && (dst.Func == IDF_NONE))
		{
			dst.Func = IDF_ICON;
		}

// parametry prvku
		dst.Param = 0;
		if (src.Param & PETPROG_EXP)	dst.Param |= PR_EXP;
		if (src.Param & PETPROG_LOCK)	dst.Param |= PR_LOCK;
		if (src.Param & PETPROG_OFF)	dst.Param |= PR_OFF;
		if (src.Param & PETPROG_NOMOVE)	dst.Param |= PR_NOMOVE;
		if (src.Param & PETPROG_INTERN)	dst.Param |= PR_INTERN;

// nastavení pøíznaku další položky
		next[parnum] = (src.Param & PETPROG_NEXT);

// naètení jména reference
		GetText(src.RefIndex);

// naètení deklarace
		bufref = &BufObj;		// pøednastavení reference na globální buffer
		reffrst = -1;			// pøednastavení - hledat od zaèátku
		refnext = -1;			// pøednastavení - hledat po konec

		switch (src.RefBlok)
		{
		case BufLocID:
			bufref = &BufLoc;				// reference na lokální buffer
			reffrst = m_LocFirst;			// hledaná první lokální položka

	// hledací oblast pro vstupní promìnnou funkce
			if ((bufID == BufEdiID) &&								// pouze v oknì editoru
				(parent[parnum] >= 0) &&							// položka má nìjakého rodièe
				(BufEdi[parent[parnum]].Func == IDF_FNC) &&			// rodièem je funkce
				(BufEdi[parent[parnum]].RefBlok == BufObjID) &&		// globální funkce - referuje na globální buffer
				(BufEdi[parent[parnum]].Parent >= 0))				// musí mít rodièe, tj. nesmí to být ROOT položka
			{
				reffrst = BufEdi[parent[parnum]].RefIndex;			// reference do globálních objektù
				if (BufObj.IsValid(reffrst) && (BufObj[reffrst].DatBlok == BufLocID)) // je to funkce ukazujicí do lokálního bufferu?
				{
					reffrst = BufObj[reffrst].DatIndex;				// index odpovídající lokální definice
					inpar = TRUE;									// pøíznak, že je to vstupní parametr funkce
				}
				else
				{
					reffrst = -1;									// to je chyba - lokální definice nenalezena
				}
			}

			if (bufref->IsValid(reffrst))							// je lokální položka platná?
			{
				refnext = bufref->At(reffrst).Next;					// poslední hledaná položka
			}

	// !!!!!! pokraèuje obsluha BufObjID !!!

		case BufObjID:
			dst.RefBlok = bufref->BufID();							// referenèní blok
			inx = bufref->FindObj(m_Text, -1, reffrst, refnext);	// nalezení objektu daného jména

	// pokus o nalezení objektu stejného typu
			nn = 1;
			while (bufref->IsValid(inx) && (bufref->At(inx).Func != dst.Func) &&
						(
							(bufref->At(inx).Func != IDF_NONE) || (dst.Func != IDF_ICON)
						)
				  )
			{
				nn++;
				m_Text.SetNumObj(nn);
				inx = bufref->FindObj(m_Text, -1, reffrst, refnext);
			}

	// vstupní promìnná musí referovat také na vstupní promìnnou
			if (inpar && 
				bufref->IsValid(inx) && 
				(bufref->At(inx).Parent >= 0) && 
				(bufref->At(bufref->At(inx).Parent).Func != IDF_PAR))
			{
				inx = -1;
			}

	// objekt nenalezen - bude se vytváøet
			if (bufref->IsNotValid(inx))	// pokud reference nebyla nalezena
			{
				if (dst.Func == IDF_NONE) dst.Func = IDF_ICON; // bude kopie ikony

	// cíl pro vstupní promìnnou funkce
				if (inpar && BufLoc.IsValid(reffrst) && (BufLoc[reffrst].Child >= 0))
				{
					reffrst = BufLoc[reffrst].Child;
					while (BufLoc.IsValid(reffrst) && (BufLoc[reffrst].Func != IDF_PAR))
					{
						reffrst = BufLoc[reffrst].Next;
					}

					if (BufLoc.IsValid(reffrst)) newpar = TRUE;
				}

	// vytvoøení nového objektu
				inx = bufref->Copy(reffrst, -2,	// kopie objektu
						ProgFile::ImportBlok[dst.Func - IDF],
						ProgFile::ImportIndex[dst.Func - IDF]);

				if (bufref->IsValid(inx))	// je teï již reference platná?
				{
	// zrušení potomkù objektu (promìnné sprajtu a seznamu)
					while (bufref->At(inx).Child >= 0)
					{
						if (!bufref->Del(bufref->At(inx).Child)) break;
					}

	// umožnìní pozdìjší zrušení takto vytvoøeného objektu
					bufref->At(inx).Param |= PR_NEW;					// pøíznak nové položky
					bufref->At(inx).Param &= ~(PR_NOMOVE | PR_INTERN);	// aby se dal zrušit

	// zajištìní jedineènosti jména nového objektu
					int overloc = -1;					// pøeskoèení v lokálním bufferu
					int overobj = inx;					// pøeskoèení v globálním bufferu

					if (newpar)							// je to vstupní promìnná funkce?
					{
						reffrst = BufLoc[reffrst].Parent; // zaèátek lokální definice s promìnnou
						refnext = BufLoc[reffrst].Next;	// poslední hledaná položka
						overloc = inx;					// pøeskoèení v lokálním bufferu
						overobj = -1;					// pøeskoèení v globálním bufferu
					}
					else
					{
						reffrst = -1;						// pro globální buffer prohledávat celý lokální buffer
						refnext = -1;						// pøíští položka

						if (src.RefBlok == BufLocID)		// položka je v lokálním bufferu
						{
							overloc = inx;					// pøeskoèení v lokálním bufferu
							overobj = -1;					// pøeskoèení v globálním bufferu

							reffrst = m_LocFirst;			// hledaná první lokální položka
							if (BufLoc.IsValid(reffrst))	// je lokální položka platná?
							{
								refnext = BufLoc[reffrst].Next;	// poslední hledaná položka
							}
						}
					}

					while ((BufLoc.FindObj(m_Text, overloc, reffrst, refnext) >= 0) ||	// je jméno v lokálním bufferu?
							(BufObj.FindObj(m_Text, overobj, -1, -1) >= 0))				// je jméno v globálním bufferu?
					{
						nn++;
						m_Text.SetNumObj(nn);
					}

	// nastavení jména nového objektu
					int oldname = bufref->At(inx).Name;
					bufref->At(inx).Name = Text.Add(m_Text); // nastavení jména položky
					Undo.AddProgName(bufref->BufID(), inx, oldname);

					if (bufref->At(inx).HTree)			// je položka zobrazena?
					{
						TV_ITEM tvi;
						tvi.mask = TVIF_TEXT;
						tvi.hItem = bufref->At(inx).HTree;
						tvi.pszText = (LPTSTR)(LPCTSTR)m_Text.MultiText();	// oprava textu zobrazené položky
						::SendMessage(bufref->Tree(), TVM_SETITEM, 0, (LPARAM)&tvi);
					}

					if (newpar)
					{
						CText txt(m_Text.MultiText());
						AktNameTree(BufEdiID, txt, BufLocID, inx);
					}
				}
			}

			dst.RefIndex = inx;				// nalezená referenèní položka
			break;

	// reference není na žádný objekt, referuje se do bufferu tøíd nebo struktur
		default:
			inx = dst.Func - IDF;						// èíslo funkce
			dst.RefBlok = ProgFile::ImportBlok[inx];	// referenèní blok
			dst.RefIndex = ProgFile::ImportIndex[inx];	// referenèní položka
		}

// zdìdìní parametrù z deklarace
		blok = dst.RefBlok;								// referenèní blok
		inx = dst.RefIndex;								// referenèní index

		if (((DWORD)blok < (DWORD)PROGBUFNUM) &&		// je platné èíslo bloku?
			BufProg[blok].IsValid(inx))					// je platná referenèní položka?
		{
			PROGITEM* itm = &BufProg[blok][inx];		// adresa referenèní položky
			dst.Func = itm->Func;						// èíslo funkce
			dst.SrcMask = itm->SrcMask;
			dst.DstMask = itm->DstMask;
			dst.Param |= itm->Param & (PR_ONE | PR_INTERN | PR_NOMOVE | PR_SETPAR | PR_PARPAR);
		}
		else
		{
			dst.RefBlok = ProgFile::ImportBlok[IDF_COMMENT - IDF];	// referenèní blok
			dst.RefIndex = ProgFile::ImportIndex[IDF_COMMENT - IDF];	// referenèní položka
			dst.Func = IDF_COMMENT;
			dst.SrcMask = PR_ALL;
			dst.DstMask = PR_ALL;

			blok = dst.RefBlok;								// referenèní blok
			inx = dst.RefIndex;								// referenèní index
			if (((DWORD)blok < (DWORD)PROGBUFNUM) &&		// je platné èíslo bloku?
				BufProg[blok].IsValid(inx))					// je platná referenèní položka?
			{
				PROGITEM* itm = &BufProg[blok][inx];		// adresa referenèní položky
				dst.Func = itm->Func;						// èíslo funkce
				dst.SrcMask = itm->SrcMask;
				dst.DstMask = itm->DstMask;
				dst.Param |= itm->Param & (PR_ONE | PR_INTERN | PR_NOMOVE | PR_SETPAR | PR_PARPAR);
			}
		}

// korekce parametrù indexu seznamu
		if ((dst.Func == IDF_LIST_INDEX) ||
			(dst.Func == IDF_LIST_AUTO) ||
			(dst.Func == IDF_LIST_SIZE) ||
			(dst.Func == IDF_SPRITE_X) ||
			(dst.Func == IDF_SPRITE_Y) ||
			(dst.Func == IDF_SPRITE_SMER) ||
			(dst.Func == IDF_SPRITE_MOVE) ||
			(dst.Func == IDF_SPRITE_VISIBLE) ||
			(dst.Func == IDF_SPRITE_FAZE) ||
			(dst.Func == IDF_SPRITE_STATUS))
		{
			dst.Param &= ~PR_NOMOVE;
			dst.Param |= PR_INTERN;
		}

// v editoru se ruší interní pøíznak
		if (bufID == BufEdiID)
		{
			dst.Param &= ~PR_INTERN;
		}

// založení lokální definice
		if ((bufID == BufLocID) && (parent[parnum] < 0))
		{
			dst.DatBlok = BufEdiID;
			dst.DatIndex = -1;
			dst.Name = -1;
			dst.Icon = -1;
			inxloc = buf->Insert(&dst, parent[parnum], prev);
			isloc = TRUE;
			m_LocFirst = inxloc;
		}

// pøíprava k naètení dat
		blok = src.DatBlok;					// datový blok
		inx = src.DatIndex;					// délka dat
		dst.DatBlok = blok;					// datový blok
		dst.DatIndex = -1;					// nejsou data

		if (inx < 0) inx = 0;				// ochrana pøi podteèení údaje délky dat
		int newnum = m_Num + inx;			// nový ukazatel ètení dat

		switch(blok)						// podle datového bloku
		{
// lokální funkce, funkce
		case BufLocID:
		case BufEdiID:
			if (inx > sizeof(long))
			{
				long cnt;					// poèet prvkù
				Get(&cnt, sizeof(long));	// poèet prvkù
				int locfirst = m_LocFirst;	// úschova výchozí položky hledání v lokálním bufferu
				dst.DatIndex = GetProg(blok, BufProg[blok].First(), cnt); // naètení programu
				m_LocFirst = locfirst;		// návrat výchozí položky hledání v lokálním bufferu
				if (dst.DatIndex < 0)
				{
					MemFree(parent);
					MemFree(next);
					buf->RedrawOn();
					return -1;
				}
			}
			else
			{
				m_Num += inx;
			}
			break;

// èíslo
		case BufNumID:
			if (inx >= sizeof(double))
			{
				double num;
				Get(&num, sizeof(double));
				dst.DatIndex = Real.Add(num);
			}
			else
			{
				dst.DatIndex = Real.New();
			}
			if (dst.DatIndex < 0)
			{
				MemFree(parent);
				MemFree(next);
				buf->RedrawOn();
				return -1;
			}
			break;

// text
		case BufTxtID:
			GetText(inx);
			dst.DatIndex = Text.Add(m_Text);
			if (dst.DatIndex < 0)
			{
				MemFree(parent);
				MemFree(next);
				buf->RedrawOn();
				return -1;
			}
			break;
			
// logická promìnná
		case BufLogID:
			if (inx >= sizeof(long))
			{
				long num;
				Get(&num, sizeof(long));
				dst.DatIndex = Bool.Add(num);
			}
			else
			{
				dst.DatIndex = Bool.New();
			}
			if (dst.DatIndex < 0)
			{
				MemFree(parent);
				MemFree(next);
				buf->RedrawOn();
				return -1;
			}
			break;

// ikona
		case BufIcoID:
			if (inx >= ICONSIZE)
			{
				GetIcon(inx);
				dst.DatIndex = Icon.Add(m_Icon);
			}
			else
			{
				dst.DatIndex = Icon.New();
			}
			if (dst.DatIndex < 0)
			{
				MemFree(parent);
				MemFree(next);
				buf->RedrawOn();
				return -1;
			}
			break;

// plocha
		case BufMapID:
			inx -= 3*sizeof(long);
			if (inx >= 0)
			{
				long width, height, refs;
				Get(&width, sizeof(long));
				Get(&height, sizeof(long));
				Get(&refs, sizeof(long));
				dst.DatIndex = Map.New(width, height);
				if (dst.DatIndex < 0)
				{
					MemFree(parent);
					MemFree(next);
					buf->RedrawOn();
					return -1;
				}

				int i;
				CBufIndex bufinx;
				bufinx.NumClear(refs);

			// naètení referencí
				for (i = 0; i < refs; i++)
				{
					GetText(m_Max - m_Num);			// naètení referenèního textu

			// nalezení objektu v nìkterém z bufferù
					reffrst = -1;
					refnext = -1;
					CBufProg* bf = &BufObj;			// bude se hledat v globálním bufferu
					inx = bf->FindObj(m_Text, -1, -1, -1); // nalezení reference

					if ((inx < 0) && (bufID == BufLocID))	// nebyla položka nalezena?
					{
						bf = &BufLoc;				// bude se hledat v lokálním bufferu
						reffrst = m_LocFirst;		// hledaná první lokální položka
						if (bf->IsValid(reffrst))	// je lokální položka platná?
						{
							refnext = bf->At(reffrst).Next;	// poslední hledaná položka
						}
						inx = bf->FindObj(m_Text, -1, reffrst, refnext);
					}

			// pokus o nalezení objektu správného typu
					nn = 1;
					while (bf->IsValid(inx) && (bf->At(inx).Func != IDF_ICON)
							&& (bf->At(inx).Func != IDF_NONE))
					{
						nn++;
						m_Text.SetNumObj(nn);

						reffrst = -1;
						refnext = -1;
						CBufProg* bf = &BufObj;			// bude se hledat v globálním bufferu
						inx = bf->FindObj(m_Text, -1, -1, -1); // nalezení reference

						if ((inx < 0) && (bufID == BufLocID))	// nebyla položka nalezena?
						{
							bf = &BufLoc;				// bude se hledat v lokálním bufferu
							reffrst = m_LocFirst;		// hledaná první lokální položka
							if (bf->IsValid(reffrst))	// je lokální položka platná?
							{
								refnext = bf->At(reffrst).Next;	// poslední hledaná položka
							}
							inx = bf->FindObj(m_Text, -1, reffrst, refnext);
						}
					}
		
			// objekt nenalezen - bude se vytváøet (v globálním bufferu)
					if (bf->IsNotValid(inx))
					{
						bf = &BufObj;
						inx = bf->Copy(-1, -2,		// kopie nové ikony
								ProgFile::ImportBlok[IDF_ICON - IDF],
								ProgFile::ImportIndex[IDF_ICON - IDF]);

			// zajištìní jedineènosti jména nové ikony
						if (bf->IsValid(inx))
						{
							bf->At(inx).Param |= PR_NEW;					// pøíznak nové položky
		
							refnext = -1;						// pøíští položka
							reffrst = m_LocFirst;			// hledaná první lokální položka
							if (BufLoc.IsValid(reffrst))	// je lokální položka platná?
							{
								refnext = BufLoc[reffrst].Next;	// poslední hledaná položka
							}

							while ((BufLoc.FindObj(m_Text, -1, reffrst, refnext) >= 0) ||	// je jméno v lokálním bufferu?
									(BufObj.FindObj(m_Text, inx, -1, -1) >= 0))				// je jméno v globálním bufferu?
							{
								nn++;
								m_Text.SetNumObj(nn);
							}

			// nastavení jména nového objektu
							int oldname = bf->At(inx).Name;
							bf->At(inx).Name = Text.Add(m_Text);
							Undo.AddProgName(bf->BufID(), inx, oldname);
							if (bf->At(inx).HTree)
							{
								TV_ITEM tvi;
								tvi.mask = TVIF_TEXT;
								tvi.hItem = bf->At(inx).HTree;
								tvi.pszText = (LPTSTR)(LPCTSTR)m_Text.MultiText();
								::SendMessage(bf->Tree(), TVM_SETITEM, 0, (LPARAM)&tvi);
							}
						}
					}

			// úschova indexu ikony
					bufinx[i] = bf->GetIcon(inx);
					if (bufinx[i] < 0) bufinx[i] = 0;
				}

			// naètení ikon
				MAPDATA* map = Map[dst.DatIndex].Data();
				int size = width*height;

				for (i = 0; i < size; i++)
				{
					Get(&map->Data[i].Icon, sizeof(long));
					if ((DWORD)map->Data[i].Icon < (DWORD)refs)
					{
						map->Data[i].Icon = bufinx[map->Data[i].Icon];
					}
					else
					{
						map->Data[i].Icon = 0;
					}
					Get(&map->Data[i].Param, sizeof(DWORD));
				}

			}
			else
			{
				dst.DatIndex = Map.New();
				if (dst.DatIndex < 0)
				{
					MemFree(parent);
					MemFree(next);
					buf->RedrawOn();
					return -1;
				}
			}

		// inrekentace èítaèù použití ikon
			Map[dst.DatIndex].RefInc();
			break;

// obrázek
		case BufPicID:
			inx -= 2*sizeof(long);
			if (inx >= 0)
			{
				long width, height;
				Get(&width, sizeof(long));
				Get(&height, sizeof(long));
				dst.DatIndex = Picture.New(width, height);
				if (dst.DatIndex < 0)
				{
					MemFree(parent);
					MemFree(next);
					buf->RedrawOn();
					return -1;
				}
				Get(Picture[dst.DatIndex].DataData(), width*height);
				Picture[dst.DatIndex].Comp();
			}
			else
			{
				dst.DatIndex = Picture.New();
				if (dst.DatIndex < 0)
				{
					MemFree(parent);
					MemFree(next);
					buf->RedrawOn();
					return -1;
				}
			}
			break;

// sprajt
		case BufSprID:
			inx -= 5*sizeof(long) + 2*sizeof(WORD) + sizeof(double);
			if (inx >= 0)
			{
				long faze, smer;
				Get(&faze, sizeof(long));
				Get(&smer, sizeof(long));
				int size = faze * smer;
				dst.DatIndex = Sprite.New(faze, smer);
				if (dst.DatIndex < 0)
				{
					MemFree(parent);
					MemFree(next);
					buf->RedrawOn();
					return -1;
				}
				SPRITEDATA* spr = Sprite[dst.DatIndex].Data();
				Get(&spr->Klid, sizeof(long));
				WORD width;
				Get(&width, sizeof(WORD));
				WORD height;
				Get(&height, sizeof(WORD));
				Get(&spr->Delay, sizeof(long));
				Get(&spr->Level, sizeof(long));
				Get(&spr->Kroku, sizeof(double));
				int i = 0;
				for (; size > 0; size--)
				{
					GetPicture(width, height);
					spr->Data[i] = m_Pic;
					i++;
				}
			}
			else
			{
				dst.DatIndex = Sprite.New();
				if (dst.DatIndex < 0)
				{
					MemFree(parent);
					MemFree(next);
					buf->RedrawOn();
					return -1;
				}
			}
			break;

// zvuk
		case BufSndID:
			inx -= sizeof(DWORD) + sizeof(WORD) + 2*sizeof(BYTE);
			if (inx >= 0)
			{
				dst.DatIndex = Sound.New(inx);
				if (dst.DatIndex < 0)
				{
					MemFree(parent);
					MemFree(next);
					buf->RedrawOn();
					return -1;
				}
				SOUNDDATA* snd = Sound[dst.DatIndex].Data();
				Get(&snd->Samples, sizeof(DWORD));
				Get(&snd->Format, sizeof(WORD));
				Get(&snd->Channels, sizeof(BYTE));
				Get(&snd->Bits, sizeof(BYTE));
				Get(snd->Data, inx);
			}
			else
			{
				dst.DatIndex = Sound.New();
				if (dst.DatIndex < 0)
				{
					MemFree(parent);
					MemFree(next);
					buf->RedrawOn();
					return -1;
				}
			}
			break;

// hudba
		case BufMusID:
			if (inx >= 0)
			{
				dst.DatIndex = Music.New(inx);
				if (dst.DatIndex < 0)
				{
					MemFree(parent);
					MemFree(next);
					buf->RedrawOn();
					return -1;
				}
				Get(Music[dst.DatIndex].DataData(), inx);
			}
			else
			{
				dst.DatIndex = Music.New();
				if (dst.DatIndex < 0)
				{
					MemFree(parent);
					MemFree(next);
					buf->RedrawOn();
					return -1;
				}
			}
			break;

		default:
			dst.DatBlok = -1;					// není datový blok
		}

		m_Num = newnum;						// nový ukazatel ètení dat
		if (m_Num > m_Max) m_Num = m_Max;	// oprava pøi pøeteèení konce

// naètení ikony
		dst.Icon = -1;
		if (src.Icon >= 0)
		{
			GetIcon(src.Icon);
			dst.Icon = Icon.Add(m_Icon);
		}

// korekce pro objekt ikony
		if (dst.DatBlok == BufIcoID)
		{
			if (dst.DatIndex >= 0)
			{
				Icon.Del(dst.Icon);
				dst.Icon = dst.DatIndex;
			}
		}

// naètení jména
		dst.Name = -1;
		if (src.Name >= 0)
		{
			GetText(src.Name);
			dst.Name = Text.Add(m_Text);
		}

// v objektech kontrola, zda jméno již existuje (zda byl již døíve vytvoøen pomocný objekt)
		inx = -1;									// pøednastavení - prvek neexistuje

		if (((bufID == BufLocID) || (bufID == BufObjID)) && buf->TestObj(dst.Func))
		{
			if (dst.Name >= 0)
			{
				m_Text = Text.Get(dst.Name);
			}
			else
			{
				m_Text = Text.Get(BufProg[dst.RefBlok].GetText(dst.RefIndex));
			}

			reffrst = -1;
			refnext = -1;
			if (bufID == BufLocID)
			{
				reffrst = m_LocFirst;		// hledaná první lokální položka
				if (buf->IsValid(reffrst))	// je lokální položka platná?
				{
					refnext = buf->At(reffrst).Next; // poslední hledaná položka
				}
			}

			inx = buf->FindObj(m_Text, -1, reffrst, refnext);

	// prvek vyhovuje, mùže se pøesunout sem (byl nalezen døíve vytvoøený pomocný prvek)
			if (inx >= 0)
			{
				if ((buf->At(inx).Func == dst.Func) &&
					((buf->At(inx).Param & PR_NEW) != 0))
				{
					inx = buf->Move(parent[parnum], prev, inx);
					if (inx >= 0)
					{
						int olddata = buf->At(inx).DatIndex;
						int newdata = dst.DatIndex;

						if (newdata >= 0)
						{
							switch(dst.DatBlok)
							{
							case BufLocID:
								buf->At(inx).DatIndex = newdata;
								BufLoc.Del(olddata);
								break;

							case BufEdiID:
								buf->At(inx).DatIndex = newdata;
								BufEdi.Del(olddata);
								break;

							case BufNumID:
								Real[olddata] = Real[newdata];
								Real.Del(newdata);
								break;

							case BufTxtID:
								Text[olddata] = Text[newdata];
								Text.Del(newdata);
								break;
								
							case BufLogID:
								Bool[olddata] = Bool[newdata];
								Bool.Del(newdata);
								break;

							case BufIcoID:
								Icon[olddata] = Icon[newdata];
								Icon.Del(newdata);
								break;

							case BufMapID:
								Map[olddata].RefDec();
								Map[olddata] = Map[newdata];
								Map.Del(newdata);
								break;

							case BufPicID:
								Picture[olddata] = Picture[newdata];
								Picture.Del(newdata);
								break;

							case BufSprID:
								Sprite[olddata] = Sprite[newdata];
								Sprite.Del(newdata);
								break;

							case BufSndID:
								Sound[olddata] = Sound[newdata];
								Sound.Del(newdata);
								break;

							case BufMusID:
								Music[olddata] = Music[newdata];
								Music.Del(newdata);
								break;
							}
						}
					}
				}
				else
				{
					inx = -1;
				}
			}
		}

// zajištìní jedineènosti jména objektu
		if (inx < 0)
		{
			if (((bufID == BufObjID) || (bufID == BufLocID)) && buf->TestObj(dst.Func))
			{
				if (dst.Name >= 0)
				{
					m_Text = Text.Get(dst.Name);
				}
				else
				{
					m_Text = Text.Get(BufProg[dst.RefBlok].GetText(dst.RefIndex));
				}

				refnext = -1;
				reffrst = m_LocFirst;		// hledaná první lokální položka
				if (buf->IsValid(reffrst))	// je lokální položka platná?
				{
					refnext = buf->At(reffrst).Next; // poslední hledaná položka
				}

				nn = 1;
				while ((BufLoc.FindObj(m_Text, -1, reffrst, refnext) >= 0) ||
						(BufObj.FindObj(m_Text, -1, -1, -1) >= 0))
				{
					nn++;
					m_Text.SetNumObj(nn);
				}

				if (nn > 1)
				{
					if (dst.Name < 0)
					{
						dst.Name = Text.Add(m_Text);
					}
					else
					{
						Text.Set(dst.Name, m_Text);
					}
				}
			}

// vložení prvku do bufferu (pokud nebyl pøesunut)
			if (newpar && (BufEdi[parent[parnum]].Child >= 0))
			{
				inx = BufEdi[parent[parnum]].Child;
				while (BufEdi[inx].Next >= 0) inx = BufEdi[inx].Next;
			}
			else
			{
				if (isloc)
				{
					inx = inxloc;
					BufLoc[inx].DatIndex = dst.DatIndex;
				}
				else
				{
					inx = buf->Insert(&dst, parent[parnum], prev);
				}
			}
		}
		prev = -2;						// odteï již ukládat na konec
		if (result < 0) result = inx;	// pøi prvním použití to bude výchozí prvek

// pokud budou potomci, zvýšení èítaèe rodièù
		if (src.Param & PETPROG_CHILDS)
		{
			parnum++;
			if (parnum >= parmax)
			{
				parmax += 1024;
				parent = (int*)MemSize(parent, parmax * sizeof(int));
				next = (int*)MemSize(next, parmax * sizeof(int));
				if ((parent == NULL) || (next == NULL))
				{
					MemFree(parent);
					MemFree(next);
					buf->RedrawOn();
					return -1;
				}
			}
			parent[parnum] = inx;
			next[parnum] = TRUE;
		}

// snížení èítaèe rodièù
		else
		{
			while (!next[parnum] && (parnum > 0))
			{
				parnum--;
			}
		}

// pøi nedostatku dat po prvním prùchodu konec
		if (m_Num >= m_Max) break;
	}

// zrušení bufferù
	MemFree(parent);
	MemFree(next);

// zapnutí pøekreslování
	buf->RedrawOn();

	return result;
}


/////////////////////////////////////////////////////////////////////////////
// test položení z clipbardu (buffer i poloŸka jsou platné)

BOOL TestPaste(int bufID, int inx)
{
// adresa bufferu a položky
	CBufProg* buf = BufProg + bufID;
	if (buf->IsNotValid(inx)) return FALSE;
	PROGITEM* item = &buf->At(inx);
	int fnc = item->Func;

// test pro neplatný rodiè - globální bufferu nebo definice funkce (nejsou povoleny èíslice)
	if (item->Parent < 0)
	{
		return ((fnc != IDF_LIST_SIZE) && ((fnc < IDF_0) || (fnc > IDF_9)));
	}

// rodiè položky
	PROGITEM* parent = &buf->At(item->Parent);
	int fncpar = parent->Func;

// kontrola bufferu objektù
	switch (bufID)
	{
	case BufObjID:
	case BufLocID:

// do velikosti seznamu lze pøetáhnout èíslice
		if (fncpar == IDF_LIST_SIZE)
		{
			return ((fnc >= IDF_0) && (fnc <= IDF_9));
		}

// položení komentáøe
		if (fnc == IDF_COMMENT)
		{
			return (((parent->Param & PR_ONE) == 0) &&
					(fncpar != IDF_PAR) &&
					(fncpar != IDF_OUT) &&
					(fncpar != IDF_FNC) &&
					(fncpar != IDF_COMMAND_LINE) &&
					(fncpar != IDF_RETURN_CODE));
		}

// položení do komentáøe (zákaz - jen další komentáø)
		if (fncpar == IDF_COMMENT) return FALSE;

// do seznamu lze položit datové promìnné
		if (fncpar == IDF_LIST)
		{
			switch (fnc)
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
		if (((fnc == IDF_GROUP) || (fnc == IDF_LIST) || (fnc == IDF_FNC)) &&
			((fncpar == IDF_PAR) || (fncpar == IDF_OUT))) return FALSE;

// test, zda je povolen pouze jeden parametr (výstupní promìnná)
		if ((parent->Param & PR_ONE) && ((item->Prev >= 0) || (item->Next >= 0))) return FALSE;

// do skupiny (pøíp. parametrù) lze pøetáhnout promìnné, skupinu a seznam
		if ((fncpar == IDF_GROUP) || (fncpar == IDF_PAR) || (fncpar == IDF_OUT))
		{
			switch (fnc)
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

// kontrola editaèního bufferu
	default:

// položení komentáøe
		if (fnc == IDF_COMMENT)
		{
			return ((parent->Parent < 0) || (((parent->Param & PR_ONE) == 0) && (fncpar != IDF_FNC)));
		}

// položení do komentáøe
		if (fncpar == IDF_COMMENT) return TRUE;

// test, zda je povolen pouze jeden parametr
		if ((parent->Param & PR_ONE) && ((item->Prev >= 0) || (item->Next >= 0))) return FALSE;

// položení vìtve CASE
		if (fnc == IDF_CASE_ON) return (fncpar == IDF_CASE);
		if (fnc == IDF_CASE_EQU) return (fncpar == IDF_CASE_ON);

// porovnání masky zdroje a cíle tažení
		if ((item->SrcMask & parent->DstMask) == 0) return FALSE;

// zákaz položení položky s parametrem do parametru
		if ((fnc != IDF_FNC) &&									// funkce je povolena, mùže mít vstupní parametry
			((parent->DstMask & PR_COMMAND) == 0) &&			// cílem nejsou pøíkazy
			((item->SrcMask & PR_COMMAND) != 0) &&				// zdroj mùže být jako pøíkaz
			((item->Param & PR_PARPAR) == 0) &&					// kromì pøípadu, kdy má povoleny parametry vždy
			(item->Child >= 0)) return FALSE;					// položka má nìjakého potomka

// zákaz položení do položky, která není pøíkazem
		if (((parent->SrcMask & PR_COMMAND) != 0) &&			// cíl mùže být jako pøíkaz
			((parent->Param & PR_PARPAR) == 0) &&				// kromì pøípadu, kdy má povoleny parametry vždy
			(parent->Parent >= 0) &&							// cíl je nìèím potomkem
			((buf->At(parent->Parent).DstMask & PR_ALLDATA) != 0) &&  // rodiè cíle pøijímá data
			((buf->At(parent->Parent).DstMask & PR_COMMAND) == 0))	// pøitom rodiè cíle nepøijímá pøíkazy (=není jako pøíkaz)
			return FALSE;

		return TRUE;
	}
}


/////////////////////////////////////////////////////////////////////////////
// navrácení z bufferu

void Paste(int bufID)
{
// kontrola, zda neprobíhá obsluha
	if (Lock) return;

// kontrola povolených bufferù
	if ((bufID != BufObjID) &&
		(bufID != BufLocID) &&
		(bufID != BufEdiID))
	{
		return;
	}

// zapnutí pøíznaku obsluhy
	Lock = TRUE;

// pøíprava výchozí položky k uložení (mùže být i neplatná!)
	CBufProg* buf = BufProg + bufID;
	int first = buf->Select();

// kontrola položky
	if (buf->IsValid(first) &&
		((buf->At(first).Param & (PR_LOCK | PR_LOCK_DEP)) != 0))
	{
		Lock = FALSE;
		return;
	}

// zapnutí èekacího kurzoru
	BeginWaitCursor();

// otevøení schránky
	if (!::OpenClipboard(MainFrame))
	{
		EndWaitCursor();
		Lock = FALSE;
		return;
	}

// naètení dat schránky
	HGLOBAL		global;			// globální buffer s daty
	switch (bufID)
	{
	case BufObjID:
		global = ::GetClipboardData(CF_PETPROGG);
		break;

	case BufLocID:
		global = ::GetClipboardData(CF_PETPROGL);
		break;

	default:
		global = ::GetClipboardData(CF_PETPROGP);
	}

// nejsou pøipravena vhodná data
	if (global == NULL)
	{
		::CloseClipboard();
		EndWaitCursor();
		Lock = FALSE;
		return;
	}

// velikost dat
	m_Max = (int)::GlobalSize(global);
	if (m_Max < SIZEOFCLIPHEAD)
	{
		::CloseClipboard();
		EndWaitCursor();
		Lock = FALSE;
		return;
	}
	
// uzamknutí bufferu
	m_Buf = (BYTE*) ::GlobalLock(global);
	if (m_Buf == NULL)
	{
		::CloseClipboard();
		EndWaitCursor();
		Lock = FALSE;
		return;
	}

// pøíprava záhlaví
	const CLIPHEAD* head;
	switch (bufID)
	{
	case BufObjID:
		head = &ClipHeadG;
		break;

	case BufLocID:
		head = &ClipHeadL;
		break;

	default:
		head = &ClipHeadP;
	}

// kontrola záhlaví
	if (MemCompare(((CLIPHEAD*)m_Buf)->Ident, head->Ident, 4) &&
		(((CLIPHEAD*)m_Buf)->Data >= SIZEOFCLIPHEAD) &&
		(((CLIPHEAD*)m_Buf)->Pocet > 0) &&
		MemCompare(((CLIPHEAD*)m_Buf)->Clip.Jmeno, head->Clip.Jmeno, 8))
	{
// vynulování pøíznaku PR_NEW v objektech
		int i;

		for (i = BufObj.Max()-1; i >= 0; i--)
		{
			if (BufObj.IsValid(i))
			{
				BufObj[i].Param &= ~PR_NEW;
			}
		}

		for (i = BufLoc.Max()-1; i >= 0; i--)
		{
			if (BufLoc.IsValid(i))
			{
				BufLoc[i].Param &= ~PR_NEW;
			}
		}

// ínicializace tabulky importu funkcí
		ProgFile::InitImportTab();

// pøednastavení výchozí položky lokálního bufferu
		m_LocFirst = BufLoc.Disp();

// naètení dat
		buf->RedrawOff();
		m_Num = ((CLIPHEAD*)m_Buf)->Data;
		if (m_Num > m_Max) m_Num = m_Max;
		int prev = first;
		i = GetProg(bufID, first, ((CLIPHEAD*)m_Buf)->Clip.Pocet);
		if (i >= 0)
		{
			first = i;

// kontrola povolení operace
			if (buf->IsValid(prev) && !TestPaste(bufID, i))
			{

// pøesun jako potomka položky
				first = buf->Move(prev, -2, first);

// pøi chybì bude náhradní položení
				if (!TestPaste(bufID, first))
				{
					first = buf->Move(buf->Disp(), -2, first);
				}
			}
		}
		buf->RedrawOn();
		buf->Select(first);

// pøíznak modifikace souboru
		SetModi();
	}

// odemknutí bufferu
	::GlobalUnlock(global);

// uzavøení schránky
	::CloseClipboard();

// vypnutí èekacího kurzoru
	EndWaitCursor();

// vypnutí pøíznaku obsluhy
	Lock = FALSE;

// aktualizace voleb bloku
	UpdateClipboard();

// ukonèení záznamu jedné UNDO operace
	Undo.Stop();
}


/////////////////////////////////////////////////////////////////////////////
// zrušení bloku

void Delete(int bufID)
{
// kontrola, zda neprobíhá obsluha
	if (Lock) return;

// kontrola povolených bufferù
	if ((bufID != BufObjID) &&
		(bufID != BufLocID) &&
		(bufID != BufEdiID) &&
		(bufID != BufClsID))
	{
		return;
	}

// zapnutí pøíznaku obsluhy
	Lock = TRUE;

// pøíprava výchozí položky ke zrušení
	CBufProg* buf = BufProg + bufID;
	int first = buf->Select();

// kontrola položky
	if (buf->IsNotValid(first) ||
		((buf->At(first).Param & (PR_NOMOVE | PR_INTERN | PR_LOCK | PR_LOCK_DEP)) != 0))
	{
		Lock = FALSE;
		return;
	}

// test zrušení položky s referencemi
	if (!buf->TestRefer(first)) 
	{
		Lock = FALSE;
		return;
	}

// rušení položky v bufferu tøíd
	if (bufID == BufClsID)
	{
		ProgLib::Delete(first);
		Lock = FALSE;
		return;
	}

// zrušení položky
	BeginWaitCursor();
	buf->RedrawOff();
	buf->Del(first);
	SetModi();
	buf->RedrawOn();
	if (EditMode == BufMapID) ProgOnPaint();
	EndWaitCursor();

// vypnutí pøíznaku obsluhy
	Lock = FALSE;

// aktualizace informací o položce, aktualizace menu clipboardu
	ProgAktItem();

// ukonèení záznamu jedné UNDO operace
	Undo.Stop();
}


/////////////////////////////////////////////////////////////////////////////
// vystøižení bloku

void Cut(int bufID)
{
	if (Copy(bufID))
	{
		Delete(bufID);
	}
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace voleb bloku

void UpdateClipboard()
{
	if ((FocusBuf != BufEdiID) || (EditMode == BufEdiID))
	{
		BOOL enable = ((FocusBuf == BufObjID) ||
						(FocusBuf == BufLocID) ||
						(FocusBuf == BufEdiID) ||
						(FocusBuf == BufClsID));

		int sel = BufProg[FocusBuf].Select();
		BOOL delenable = FALSE;
		BOOL copyenable = FALSE;
		BOOL pasteenable = enable;

		if ((sel >= 0) && enable)
		{
			PROGITEM* item = &BufProg[FocusBuf].At(sel);

			delenable = ((item->Param & (PR_NOMOVE | PR_INTERN | PR_LOCK | PR_LOCK_DEP)) == 0) &&
				((FocusBuf != BufClsID) || ((item->Parent >= 0) && (item->Child < 0)));

			copyenable = (((item->Param & (PR_NOMOVE | PR_INTERN)) == 0) && (FocusBuf != BufClsID));

			pasteenable = ((item->Param & (PR_LOCK | PR_LOCK_DEP)) == 0);
		}

		EnableCommand(IDN_CUT, delenable & copyenable);
		EnableCommand(IDN_COPY, copyenable);
		switch (FocusBuf)
		{
		case BufEdiID:
			EnableCommand(IDN_PASTE, pasteenable && ::IsClipboardFormatAvailable(CF_PETPROGP));
			break;

		case BufObjID:
			EnableCommand(IDN_PASTE, pasteenable && ::IsClipboardFormatAvailable(CF_PETPROGG));
			break;

		case BufLocID:
			EnableCommand(IDN_PASTE, pasteenable && ::IsClipboardFormatAvailable(CF_PETPROGL));
			break;

		default:
			EnableCommand(IDN_PASTE, FALSE);
		}
		EnableCommand(IDN_CLEAR, delenable);
		EnableCommand(IDN_ALL, FALSE);
	}
}



}
