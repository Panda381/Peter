
#include "Main.h"

/***************************************************************************\
*																			*
*								Obsluha clipboardu							*
*																			*
\***************************************************************************/

DWORD	CF_PETPROGP = (DWORD)-1;				// ID form�tu "Program"
DWORD	CF_PETPROGG = (DWORD)-1;				// ID form�tu "Glob�ln�"
DWORD	CF_PETPROGL = (DWORD)-1;				// ID form�tu "Lok�ln�"

namespace ProgClip
{

// z�hlav� clipboardu - program
const CLIPHEAD ClipHeadP = {
	'P','E','T',									// identifikace
	1,												// verze
	VerzeCom + VerzeRel*10 + VerzeMin*100 + VerzeMaj*1000, // verze editoru
	0,												// parametry
	SIZEOFCLIPHEAD,									// offset za��tku dat
	1,												// po�et datov�ch blok�
	0,0,'C','L','I','P','B','R','D','P',0,0,0,0,
};

// z�hlav� clipboardu - glob�ln�
const CLIPHEAD ClipHeadG = {
	'P','E','T',									// identifikace
	1,												// verze
	VerzeCom + VerzeRel*10 + VerzeMin*100 + VerzeMaj*1000, // verze editoru
	0,												// parametry
	SIZEOFCLIPHEAD,									// offset za��tku dat
	1,												// po�et datov�ch blok�
	0,0,'C','L','I','P','B','R','D','G',0,0,0,0,
};

// z�hlav� clipboardu - lok�ln�
const CLIPHEAD ClipHeadL = {
	'P','E','T',									// identifikace
	1,												// verze
	VerzeCom + VerzeRel*10 + VerzeMin*100 + VerzeMaj*1000, // verze editoru
	0,												// parametry
	SIZEOFCLIPHEAD,									// offset za��tku dat
	1,												// po�et datov�ch blok�
	0,0,'C','L','I','P','B','R','D','L',0,0,0,0,
};

/////////////////////////////////////////////////////////////////////////////
// parametry

BOOL		Lock = FALSE;				// p��znak prob�haj�c� obsluhy

// buffer pro ulo�en�
BYTE*		m_Buf = NULL;				// datov� buffer
int			m_Num = 0;					// po�et bajt� v bufferu, p�i �ten� �tec� offset
int			m_Max = 0;					// velikost bufferu, p�i �ten� velikost dat

CMultiText	m_Text;						// buffer k na�ten�/ulo�en� textu
CIcon		m_Icon;						// buffer k na�ten�/ulo�en� ikony
CPicture	m_Pic;						// buffer k na�ten�/ulo�en� obr�zku
CMap		m_Map;						// buffer k na�ten�/ulo�en� plochy
CSprite		m_Sprite;					// buffer k na�ten�/ulo�en� sprajtu
CSound		m_Sound;					// buffer k na�ten�/ulo�en� zvuku
CMusic		m_Music;					// buffer k na�ten�/ulo�en� hudby

int			m_LocFirst;					// v�choz� polo�ka v lok�ln�m bufferu

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
// p�id�n� dat do v�stupn�ho bufferu (vrac� FALSE=chyba pam�ti)

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
// p�id�n� v�cejazy�n�ho textu do v�stupn�ho bufferu, vrac� d�lku v bajtech, -1=chyba pam�ti

int AddText(int inx)
{
// text k ulo�en� (m��e b�t i neplatn�)
	m_Text = Text.Get(inx);
	CText txt;

	int len = 0;

// cyklus p�es v�echny jazyku
	for (int lan = 0; lan < JAZYKNUM; lan++)
	{

// text k ulo�en�
		txt = m_Text.MultiText(lan);

// d�lka textu v bajtech
		long len0 = txt.Length();

// p�id�n� d�lky
		BOOL ret = Add(&len0, sizeof(long));
		if (!ret) return -1;
		len += sizeof(long);

// p�id�n� textu
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

// d�lka cel�ho textu
	return len;
}


/////////////////////////////////////////////////////////////////////////////
// p�id�n� polo�ky programu (vrac� po�et p�enesen�ch prvk� ve v�tvi, -1=chyba pam�ti)

int AddProg(int bufID, int index, BOOL isfirst)
{
// lok�ln� prom�nn�
	PETPROG dst;				// c�lov� polo�ka k ulo�en�
	int dstinx;					// offset c�lov� polo�ky v bufferu
	PETPROG* pet;				// adresa c�lov� polo�ky v bufferu
	int inx;					// pomocn� index zdrojov� polo�ky
	int size;					// pomocn� d�lka dat polo�ky
	int count = 0;				// ��ta� vlo�en�ch prvk�
	int first = index;			// �schova indexu v�choz� polo�ky
	PROGITEM* src;				// adresa zdrojov� polo�ky
	CBufProg* buf = BufProg + bufID;	// adresa bufferu
	int i;						// pomocn� registr

// cyklus p�es celou v�tev
	do 
	{
		ASSERT(buf->IsValid(index));
		if (buf->IsNotValid(index)) break;
		count++;					// zv��en� ��ta�e prvk�

// adresa zdrojov� polo�ky
		src = &buf->At(index);

// p��prava parametr� polo�ky
		dst.Param = 0;
		if (src->Child >= 0)		dst.Param |= PETPROG_CHILDS;
		if (src->Next >= 0)			dst.Param |= PETPROG_NEXT;
		if (src->Param & PR_EXP)	dst.Param |= PETPROG_EXP;
		if (src->Param & PR_LOCK)	dst.Param |= PETPROG_LOCK;
		if (src->Param & PR_OFF)	dst.Param |= PETPROG_OFF;
		if (src->Param & PR_NOMOVE)	dst.Param |= PETPROG_NOMOVE;
		if (src->Param & PR_INTERN)	dst.Param |= PETPROG_INTERN;

// u prvn� polo�ky v�tve zru�en� z�mku
		if ((index == first) && isfirst) dst.Param &= ~PETPROG_LOCK;

// ulo�en� polo�ky programu
		dstinx = m_Num;								// offset c�lov� polo�ky
		if (!Add(&dst, SIZEOFPETPROG)) return -1;	// ulo�en� polo�ky do bufferu

// referen�n� text
		pet = (PETPROG*)(m_Buf + dstinx);			// adresa c�lov� polo�ky
		pet->RefIndex = 0;							// d�lka jm�na referen�n� polo�ky
		pet->RefBlok = -1;							// neplatn� referen�n� blok

		switch (src->RefBlok)						// podle referen�n�ho bloku
		{
		case BufObjID:
		case BufLocID:
			i = AddText(BufProg[src->RefBlok].GetText(src->RefIndex));
			if (i < 0) return -1;
			pet = (PETPROG*)(m_Buf + dstinx);		// adresa c�lov� polo�ky
			pet->RefIndex = i;						// d�lka textu v bajtech
			pet->RefBlok = src->RefBlok;			// ��slo referen�n�ho bloku
			break;
		}

// ulo�en� datov� polo�ky (a d�lky dat)
		pet = (PETPROG*)(m_Buf + dstinx);
		pet->DatBlok = src->DatBlok;
		inx = src->DatIndex;						// index po�adovan� polo�ky
		int ninx = m_Num;							// �schova offsetu za��tku dat

		switch (src->DatBlok)
		{
// lok�ln� funkce, funkce
		case BufLocID:
		case BufEdiID:
			{
				ASSERT(BufProg[src->DatBlok].IsValid(inx));
				long cnt;							// po�et prvk�
				int cntn = m_Num;					// �schova offsetu ��ta�e
				if (!Add(&cnt, sizeof(long))) return -1;// ulo�en� ��ta�e
				cnt = AddProg(src->DatBlok, inx, FALSE); // ulo�en� lok�ln� definice
				if (cnt < 0) return -1;				// chyba pam�ti
				*(long*)(m_Buf + cntn) = cnt;		// po�et prvk�				
			}
			break;

// ��slo
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

// logick� prom�nn�
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

				long reficon = 0;					// ��ta� referenc� ikon
				int nreficon = m_Num;				// offset referenc� ikon
				if (!Add(&reficon, sizeof(long))) return -1; // ulo�en� ��ta�e referenc�

				size = m_Map.Width() * m_Map.Height();	// po�et polo�ek
				CBufIndex bufinx;					// buffer mapov�n� ikon
				if (!bufinx.NumClear(Icon.Num())) return -1;

				int icon;							// pomocn� ��slo ikony
				int ref;							// index definice objektu ikony
				CBufProg* bf;						// adresa bufferu s definic�
				int i;								// index prvku plochy
				ASSERT(Icon.IsValid(0));

				for (i = 0; i < size; i++)
				{
					icon = m_Map[i].Icon;			// ikona
					if (Icon.IsNotValid(icon))		// je ikona platn�?
					{
						icon = 0;					// n�hradn� ikona
					}

					if (bufinx[icon] < 0)			// je ikona zmapovan�?
					{
						bufinx[icon] = reficon;		// n�hradn� index ikony
						reficon++;					// zv��en� ��ta�e referenc�

						bf = &BufObj;
						ref = bf->SrcDat(BufIcoID, icon);

						if ((ref < 0) && (bufID == BufLocID))
						{
							bf = &BufLoc;
							ref = bf->SrcDat(BufIcoID, icon);
						}

						ref = bf->GetText(ref);
						if (AddText(ref) < 0) return -1;	// (pro neplatn� text se ulo�� pr�zdn� text)
					}
				}
				*(long*)(m_Buf + nreficon) = reficon; // nastaven� po�tu ikon

				for (i = 0; i < size; i++)
				{
					icon = m_Map[i].Icon;
					if (Icon.IsNotValid(icon))		// je ikona platn�?
					{
						icon = 0;					// n�hradn� ikona
					}

					if (!Add(&bufinx[icon], sizeof(long)) ||
						!Add(&m_Map[i].Param, sizeof(DWORD))) return -1;
				}
			}
			break;

// obr�zek
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

// nastaven� d�lky dat
		pet = (PETPROG*)(m_Buf + dstinx);				// adresa z�hlav�
		pet->DatIndex = m_Num - ninx;					// d�lka dat

// ulo�en� ikony
		pet->Icon = -1;									// implicitn� ikona
		inx = src->Icon;								// index ikony
		if (Icon.IsValid(inx))							// je platn� ikona?
		{
			pet->Icon = ICONSIZE;						// velikost ikony
			m_Icon = Icon.Get(inx);
			if (!m_Icon.DeComp() || !Add(m_Icon.DataData(), ICONSIZE)) return -1;
		}

// ulo�en� textu jm�na polo�ky
		pet = (PETPROG*)(m_Buf + dstinx);
		pet->Name = -1;									// implicitn� jm�no
		inx = src->Name;								// index jm�na
		if (Text.IsValid(inx))							// je platn� jm�no?
		{
			int i = AddText(inx);						// ulo�en� textu
			if (i < 0) return -1;
			pet = (PETPROG*)(m_Buf + dstinx);
			pet->Name = i;
		}

// ulo�en� ��sla funkce
		pet->Func = src->Func - IDF;

// posun na potomka polo�ky
		if ((index != first) || (src->Child >= 0))
		{
			index = src->Child;
		}

// nen�-li potomek, p�esun na n�sleduj�c� polo�ku
		if (index < 0)					// je potomek?
		{
			index = src->Next;			// pokra�ov�n� dal�� polo�kou

// nen�-li dal�� polo�ka, vyno�en� k rodi�i
			while ((index < 0) && (src->Parent >= 0))
			{
				index = src->Parent;	// n�vrat k rodi�i
				if (index == first) break; // je ji� op�t v�choz� polo�ka
				src = &buf->At(index);	// adresa rodi�e
				index = src->Next;		// dal�� polo�ka za rodi�em
			}
		}

// dokud nen� dosa�eno v�choz� polo�ky
	} while (index != first);

	return count;						// po�et ulo�en�ch polo�ek
}


/////////////////////////////////////////////////////////////////////////////
// kopie do clipboardu (TRUE=operace OK)

BOOL Copy(int bufID)
{
// kontrola, zda neprob�h� obsluha
	if (Lock) return FALSE;

// kontrola povolen�ch buffer�
	if ((bufID != BufObjID) &&
		(bufID != BufLocID) &&
		(bufID != BufEdiID))
	{
		return FALSE;
	}

// zapnut� p��znaku obsluhy
	Lock = TRUE;

// p��prava v�choz� polo�ky k ulo�en�
	CBufProg* buf = BufProg + bufID;
	int first = buf->Select();
	if (first < 0)
	{
		Lock = FALSE;
		return FALSE;
	}

// kontrola platnosti polo�ky 
// (nelze kop�rovat ani intern� glob�ln� objekty - musely by se zajistit parametry sprajtu)
	if (buf->At(first).Param & (PR_NOMOVE | PR_INTERN))
	{
		Lock = FALSE;
		return FALSE;
	}

// zapnut� �ekac�ho kurzoru
	BeginWaitCursor();

// otev�en� schr�nky
	if (!::OpenClipboard(MainFrame))
	{
		EndWaitCursor();
		Lock = FALSE;
		return FALSE;
	}

// vypr�zdn�n� schr�nky
	if (!::EmptyClipboard())
	{
		EndWaitCursor();
		::CloseClipboard();
		Lock = FALSE;
		UpdateClipboard();
		return FALSE;
	}

// p��prava bufferu
	MemFree(m_Buf);
	m_Buf = NULL;
	m_Num = 0;
	m_Max = 0;

// vytvo�en� z�hlav� bufferu
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

// ulo�en� v�tve programu
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

// vytvo�en� glob�ln�ho bufferu pro data
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

// uzamknut� bufferu
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

// odemknut� bufferu
	::GlobalUnlock(global);

// ulo�en� dat do schr�nky
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

// uzav�en� schr�nky
	::CloseClipboard();

// zru�en� bufferu
	MemFree(m_Buf);
	m_Buf = NULL;

// vypnut� �ekac�ho kurzoru
	EndWaitCursor();

// vypnut� p��znaku obsluhy
	Lock = FALSE;

// aktualizace voleb bloku
	UpdateClipboard();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� dat z bufferu

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
// na�ten� textu z bufferu

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
// na�ten� ikony z bufferu

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
// na�ten� obr�zku z bufferu

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
// na�ten� programu z bufferu (vrac� index polo�ky, -1=chyba pam�ti)

int GetProg(int bufID, int prev, int num)
{
// lok�ln� prom�nn�
	CBufProg* buf = BufProg + bufID;	// adresa bufferu
	PETPROG src;						// �ten� polo�ka
	PROGITEM dst;						// ukl�dan� polo�ka
	int* parent;						// buffer odkaz� na rodi�e
	BOOL* next;							// buffer p��znak� dal��ch prvk�
	int parmax;							// velikost bufferu rodi��
	int parnum;							// po�et rodi��
	int blok;							// pomocn� ��slo bloku
	int inx;							// pomocn� ��slo indexu
	int result = -1;					// v�sledn� index
	CBufProg* bufref;					// referen�n� buffer
	int reffrst;						// prvn� polo�ka k hled�n� reference
	int refnext;						// p��t� polo�ka k hled�n� reference
	int nn;								// pomocn� ��slo jm�na
	BOOL inpar;							// polo�ka je vstupn�m parametrem
	BOOL newpar;						// p��znak, �e byl vytvo�en vstupn� parametr
	BOOL isloc;							// p��znak zalo�en� lok�ln� definice
	int inxloc = 0;						// index lok�ln� definice

// vypnut� p�ekreslov�n�
	buf->RedrawOff();

// p��prava bufferu rodi��
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
		parent[0] = buf->At(prev).Parent;	// rodi�
	}

// na�ten� polo�ek
	for (; num > 0; num--)
	{
// nen� to vstupn� parametr funkce
		inpar = FALSE;
		newpar = FALSE;
		isloc = FALSE;

// na�ten� z�hlav� polo�ky
		Get(&src, SIZEOFPETPROG);

// ��slo funkce
		dst.Func = IDF_COMMENT;
		if ((DWORD)src.Func < (DWORD)FncNum)
		{
			dst.Func = src.Func + IDF;
		}

// v bufferu objekt� se implicitn� ikona m�n� na b�nou
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

// nastaven� p��znaku dal�� polo�ky
		next[parnum] = (src.Param & PETPROG_NEXT);

// na�ten� jm�na reference
		GetText(src.RefIndex);

// na�ten� deklarace
		bufref = &BufObj;		// p�ednastaven� reference na glob�ln� buffer
		reffrst = -1;			// p�ednastaven� - hledat od za��tku
		refnext = -1;			// p�ednastaven� - hledat po konec

		switch (src.RefBlok)
		{
		case BufLocID:
			bufref = &BufLoc;				// reference na lok�ln� buffer
			reffrst = m_LocFirst;			// hledan� prvn� lok�ln� polo�ka

	// hledac� oblast pro vstupn� prom�nnou funkce
			if ((bufID == BufEdiID) &&								// pouze v okn� editoru
				(parent[parnum] >= 0) &&							// polo�ka m� n�jak�ho rodi�e
				(BufEdi[parent[parnum]].Func == IDF_FNC) &&			// rodi�em je funkce
				(BufEdi[parent[parnum]].RefBlok == BufObjID) &&		// glob�ln� funkce - referuje na glob�ln� buffer
				(BufEdi[parent[parnum]].Parent >= 0))				// mus� m�t rodi�e, tj. nesm� to b�t ROOT polo�ka
			{
				reffrst = BufEdi[parent[parnum]].RefIndex;			// reference do glob�ln�ch objekt�
				if (BufObj.IsValid(reffrst) && (BufObj[reffrst].DatBlok == BufLocID)) // je to funkce ukazujic� do lok�ln�ho bufferu?
				{
					reffrst = BufObj[reffrst].DatIndex;				// index odpov�daj�c� lok�ln� definice
					inpar = TRUE;									// p��znak, �e je to vstupn� parametr funkce
				}
				else
				{
					reffrst = -1;									// to je chyba - lok�ln� definice nenalezena
				}
			}

			if (bufref->IsValid(reffrst))							// je lok�ln� polo�ka platn�?
			{
				refnext = bufref->At(reffrst).Next;					// posledn� hledan� polo�ka
			}

	// !!!!!! pokra�uje obsluha BufObjID !!!

		case BufObjID:
			dst.RefBlok = bufref->BufID();							// referen�n� blok
			inx = bufref->FindObj(m_Text, -1, reffrst, refnext);	// nalezen� objektu dan�ho jm�na

	// pokus o nalezen� objektu stejn�ho typu
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

	// vstupn� prom�nn� mus� referovat tak� na vstupn� prom�nnou
			if (inpar && 
				bufref->IsValid(inx) && 
				(bufref->At(inx).Parent >= 0) && 
				(bufref->At(bufref->At(inx).Parent).Func != IDF_PAR))
			{
				inx = -1;
			}

	// objekt nenalezen - bude se vytv��et
			if (bufref->IsNotValid(inx))	// pokud reference nebyla nalezena
			{
				if (dst.Func == IDF_NONE) dst.Func = IDF_ICON; // bude kopie ikony

	// c�l pro vstupn� prom�nnou funkce
				if (inpar && BufLoc.IsValid(reffrst) && (BufLoc[reffrst].Child >= 0))
				{
					reffrst = BufLoc[reffrst].Child;
					while (BufLoc.IsValid(reffrst) && (BufLoc[reffrst].Func != IDF_PAR))
					{
						reffrst = BufLoc[reffrst].Next;
					}

					if (BufLoc.IsValid(reffrst)) newpar = TRUE;
				}

	// vytvo�en� nov�ho objektu
				inx = bufref->Copy(reffrst, -2,	// kopie objektu
						ProgFile::ImportBlok[dst.Func - IDF],
						ProgFile::ImportIndex[dst.Func - IDF]);

				if (bufref->IsValid(inx))	// je te� ji� reference platn�?
				{
	// zru�en� potomk� objektu (prom�nn� sprajtu a seznamu)
					while (bufref->At(inx).Child >= 0)
					{
						if (!bufref->Del(bufref->At(inx).Child)) break;
					}

	// umo�n�n� pozd�j�� zru�en� takto vytvo�en�ho objektu
					bufref->At(inx).Param |= PR_NEW;					// p��znak nov� polo�ky
					bufref->At(inx).Param &= ~(PR_NOMOVE | PR_INTERN);	// aby se dal zru�it

	// zaji�t�n� jedine�nosti jm�na nov�ho objektu
					int overloc = -1;					// p�esko�en� v lok�ln�m bufferu
					int overobj = inx;					// p�esko�en� v glob�ln�m bufferu

					if (newpar)							// je to vstupn� prom�nn� funkce?
					{
						reffrst = BufLoc[reffrst].Parent; // za��tek lok�ln� definice s prom�nnou
						refnext = BufLoc[reffrst].Next;	// posledn� hledan� polo�ka
						overloc = inx;					// p�esko�en� v lok�ln�m bufferu
						overobj = -1;					// p�esko�en� v glob�ln�m bufferu
					}
					else
					{
						reffrst = -1;						// pro glob�ln� buffer prohled�vat cel� lok�ln� buffer
						refnext = -1;						// p��t� polo�ka

						if (src.RefBlok == BufLocID)		// polo�ka je v lok�ln�m bufferu
						{
							overloc = inx;					// p�esko�en� v lok�ln�m bufferu
							overobj = -1;					// p�esko�en� v glob�ln�m bufferu

							reffrst = m_LocFirst;			// hledan� prvn� lok�ln� polo�ka
							if (BufLoc.IsValid(reffrst))	// je lok�ln� polo�ka platn�?
							{
								refnext = BufLoc[reffrst].Next;	// posledn� hledan� polo�ka
							}
						}
					}

					while ((BufLoc.FindObj(m_Text, overloc, reffrst, refnext) >= 0) ||	// je jm�no v lok�ln�m bufferu?
							(BufObj.FindObj(m_Text, overobj, -1, -1) >= 0))				// je jm�no v glob�ln�m bufferu?
					{
						nn++;
						m_Text.SetNumObj(nn);
					}

	// nastaven� jm�na nov�ho objektu
					int oldname = bufref->At(inx).Name;
					bufref->At(inx).Name = Text.Add(m_Text); // nastaven� jm�na polo�ky
					Undo.AddProgName(bufref->BufID(), inx, oldname);

					if (bufref->At(inx).HTree)			// je polo�ka zobrazena?
					{
						TV_ITEM tvi;
						tvi.mask = TVIF_TEXT;
						tvi.hItem = bufref->At(inx).HTree;
						tvi.pszText = (LPTSTR)(LPCTSTR)m_Text.MultiText();	// oprava textu zobrazen� polo�ky
						::SendMessage(bufref->Tree(), TVM_SETITEM, 0, (LPARAM)&tvi);
					}

					if (newpar)
					{
						CText txt(m_Text.MultiText());
						AktNameTree(BufEdiID, txt, BufLocID, inx);
					}
				}
			}

			dst.RefIndex = inx;				// nalezen� referen�n� polo�ka
			break;

	// reference nen� na ��dn� objekt, referuje se do bufferu t��d nebo struktur
		default:
			inx = dst.Func - IDF;						// ��slo funkce
			dst.RefBlok = ProgFile::ImportBlok[inx];	// referen�n� blok
			dst.RefIndex = ProgFile::ImportIndex[inx];	// referen�n� polo�ka
		}

// zd�d�n� parametr� z deklarace
		blok = dst.RefBlok;								// referen�n� blok
		inx = dst.RefIndex;								// referen�n� index

		if (((DWORD)blok < (DWORD)PROGBUFNUM) &&		// je platn� ��slo bloku?
			BufProg[blok].IsValid(inx))					// je platn� referen�n� polo�ka?
		{
			PROGITEM* itm = &BufProg[blok][inx];		// adresa referen�n� polo�ky
			dst.Func = itm->Func;						// ��slo funkce
			dst.SrcMask = itm->SrcMask;
			dst.DstMask = itm->DstMask;
			dst.Param |= itm->Param & (PR_ONE | PR_INTERN | PR_NOMOVE | PR_SETPAR | PR_PARPAR);
		}
		else
		{
			dst.RefBlok = ProgFile::ImportBlok[IDF_COMMENT - IDF];	// referen�n� blok
			dst.RefIndex = ProgFile::ImportIndex[IDF_COMMENT - IDF];	// referen�n� polo�ka
			dst.Func = IDF_COMMENT;
			dst.SrcMask = PR_ALL;
			dst.DstMask = PR_ALL;

			blok = dst.RefBlok;								// referen�n� blok
			inx = dst.RefIndex;								// referen�n� index
			if (((DWORD)blok < (DWORD)PROGBUFNUM) &&		// je platn� ��slo bloku?
				BufProg[blok].IsValid(inx))					// je platn� referen�n� polo�ka?
			{
				PROGITEM* itm = &BufProg[blok][inx];		// adresa referen�n� polo�ky
				dst.Func = itm->Func;						// ��slo funkce
				dst.SrcMask = itm->SrcMask;
				dst.DstMask = itm->DstMask;
				dst.Param |= itm->Param & (PR_ONE | PR_INTERN | PR_NOMOVE | PR_SETPAR | PR_PARPAR);
			}
		}

// korekce parametr� indexu seznamu
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

// v editoru se ru�� intern� p��znak
		if (bufID == BufEdiID)
		{
			dst.Param &= ~PR_INTERN;
		}

// zalo�en� lok�ln� definice
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

// p��prava k na�ten� dat
		blok = src.DatBlok;					// datov� blok
		inx = src.DatIndex;					// d�lka dat
		dst.DatBlok = blok;					// datov� blok
		dst.DatIndex = -1;					// nejsou data

		if (inx < 0) inx = 0;				// ochrana p�i podte�en� �daje d�lky dat
		int newnum = m_Num + inx;			// nov� ukazatel �ten� dat

		switch(blok)						// podle datov�ho bloku
		{
// lok�ln� funkce, funkce
		case BufLocID:
		case BufEdiID:
			if (inx > sizeof(long))
			{
				long cnt;					// po�et prvk�
				Get(&cnt, sizeof(long));	// po�et prvk�
				int locfirst = m_LocFirst;	// �schova v�choz� polo�ky hled�n� v lok�ln�m bufferu
				dst.DatIndex = GetProg(blok, BufProg[blok].First(), cnt); // na�ten� programu
				m_LocFirst = locfirst;		// n�vrat v�choz� polo�ky hled�n� v lok�ln�m bufferu
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

// ��slo
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
			
// logick� prom�nn�
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

			// na�ten� referenc�
				for (i = 0; i < refs; i++)
				{
					GetText(m_Max - m_Num);			// na�ten� referen�n�ho textu

			// nalezen� objektu v n�kter�m z buffer�
					reffrst = -1;
					refnext = -1;
					CBufProg* bf = &BufObj;			// bude se hledat v glob�ln�m bufferu
					inx = bf->FindObj(m_Text, -1, -1, -1); // nalezen� reference

					if ((inx < 0) && (bufID == BufLocID))	// nebyla polo�ka nalezena?
					{
						bf = &BufLoc;				// bude se hledat v lok�ln�m bufferu
						reffrst = m_LocFirst;		// hledan� prvn� lok�ln� polo�ka
						if (bf->IsValid(reffrst))	// je lok�ln� polo�ka platn�?
						{
							refnext = bf->At(reffrst).Next;	// posledn� hledan� polo�ka
						}
						inx = bf->FindObj(m_Text, -1, reffrst, refnext);
					}

			// pokus o nalezen� objektu spr�vn�ho typu
					nn = 1;
					while (bf->IsValid(inx) && (bf->At(inx).Func != IDF_ICON)
							&& (bf->At(inx).Func != IDF_NONE))
					{
						nn++;
						m_Text.SetNumObj(nn);

						reffrst = -1;
						refnext = -1;
						CBufProg* bf = &BufObj;			// bude se hledat v glob�ln�m bufferu
						inx = bf->FindObj(m_Text, -1, -1, -1); // nalezen� reference

						if ((inx < 0) && (bufID == BufLocID))	// nebyla polo�ka nalezena?
						{
							bf = &BufLoc;				// bude se hledat v lok�ln�m bufferu
							reffrst = m_LocFirst;		// hledan� prvn� lok�ln� polo�ka
							if (bf->IsValid(reffrst))	// je lok�ln� polo�ka platn�?
							{
								refnext = bf->At(reffrst).Next;	// posledn� hledan� polo�ka
							}
							inx = bf->FindObj(m_Text, -1, reffrst, refnext);
						}
					}
		
			// objekt nenalezen - bude se vytv��et (v glob�ln�m bufferu)
					if (bf->IsNotValid(inx))
					{
						bf = &BufObj;
						inx = bf->Copy(-1, -2,		// kopie nov� ikony
								ProgFile::ImportBlok[IDF_ICON - IDF],
								ProgFile::ImportIndex[IDF_ICON - IDF]);

			// zaji�t�n� jedine�nosti jm�na nov� ikony
						if (bf->IsValid(inx))
						{
							bf->At(inx).Param |= PR_NEW;					// p��znak nov� polo�ky
		
							refnext = -1;						// p��t� polo�ka
							reffrst = m_LocFirst;			// hledan� prvn� lok�ln� polo�ka
							if (BufLoc.IsValid(reffrst))	// je lok�ln� polo�ka platn�?
							{
								refnext = BufLoc[reffrst].Next;	// posledn� hledan� polo�ka
							}

							while ((BufLoc.FindObj(m_Text, -1, reffrst, refnext) >= 0) ||	// je jm�no v lok�ln�m bufferu?
									(BufObj.FindObj(m_Text, inx, -1, -1) >= 0))				// je jm�no v glob�ln�m bufferu?
							{
								nn++;
								m_Text.SetNumObj(nn);
							}

			// nastaven� jm�na nov�ho objektu
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

			// �schova indexu ikony
					bufinx[i] = bf->GetIcon(inx);
					if (bufinx[i] < 0) bufinx[i] = 0;
				}

			// na�ten� ikon
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

		// inrekentace ��ta�� pou�it� ikon
			Map[dst.DatIndex].RefInc();
			break;

// obr�zek
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
			dst.DatBlok = -1;					// nen� datov� blok
		}

		m_Num = newnum;						// nov� ukazatel �ten� dat
		if (m_Num > m_Max) m_Num = m_Max;	// oprava p�i p�ete�en� konce

// na�ten� ikony
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

// na�ten� jm�na
		dst.Name = -1;
		if (src.Name >= 0)
		{
			GetText(src.Name);
			dst.Name = Text.Add(m_Text);
		}

// v objektech kontrola, zda jm�no ji� existuje (zda byl ji� d��ve vytvo�en pomocn� objekt)
		inx = -1;									// p�ednastaven� - prvek neexistuje

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
				reffrst = m_LocFirst;		// hledan� prvn� lok�ln� polo�ka
				if (buf->IsValid(reffrst))	// je lok�ln� polo�ka platn�?
				{
					refnext = buf->At(reffrst).Next; // posledn� hledan� polo�ka
				}
			}

			inx = buf->FindObj(m_Text, -1, reffrst, refnext);

	// prvek vyhovuje, m��e se p�esunout sem (byl nalezen d��ve vytvo�en� pomocn� prvek)
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

// zaji�t�n� jedine�nosti jm�na objektu
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
				reffrst = m_LocFirst;		// hledan� prvn� lok�ln� polo�ka
				if (buf->IsValid(reffrst))	// je lok�ln� polo�ka platn�?
				{
					refnext = buf->At(reffrst).Next; // posledn� hledan� polo�ka
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

// vlo�en� prvku do bufferu (pokud nebyl p�esunut)
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
		prev = -2;						// odte� ji� ukl�dat na konec
		if (result < 0) result = inx;	// p�i prvn�m pou�it� to bude v�choz� prvek

// pokud budou potomci, zv��en� ��ta�e rodi��
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

// sn�en� ��ta�e rodi��
		else
		{
			while (!next[parnum] && (parnum > 0))
			{
				parnum--;
			}
		}

// p�i nedostatku dat po prvn�m pr�chodu konec
		if (m_Num >= m_Max) break;
	}

// zru�en� buffer�
	MemFree(parent);
	MemFree(next);

// zapnut� p�ekreslov�n�
	buf->RedrawOn();

	return result;
}


/////////////////////////////////////////////////////////////////////////////
// test polo�en� z clipbardu (buffer i polo�ka jsou platn�)

BOOL TestPaste(int bufID, int inx)
{
// adresa bufferu a polo�ky
	CBufProg* buf = BufProg + bufID;
	if (buf->IsNotValid(inx)) return FALSE;
	PROGITEM* item = &buf->At(inx);
	int fnc = item->Func;

// test pro neplatn� rodi� - glob�ln� bufferu nebo definice funkce (nejsou povoleny ��slice)
	if (item->Parent < 0)
	{
		return ((fnc != IDF_LIST_SIZE) && ((fnc < IDF_0) || (fnc > IDF_9)));
	}

// rodi� polo�ky
	PROGITEM* parent = &buf->At(item->Parent);
	int fncpar = parent->Func;

// kontrola bufferu objekt�
	switch (bufID)
	{
	case BufObjID:
	case BufLocID:

// do velikosti seznamu lze p�et�hnout ��slice
		if (fncpar == IDF_LIST_SIZE)
		{
			return ((fnc >= IDF_0) && (fnc <= IDF_9));
		}

// polo�en� koment��e
		if (fnc == IDF_COMMENT)
		{
			return (((parent->Param & PR_ONE) == 0) &&
					(fncpar != IDF_PAR) &&
					(fncpar != IDF_OUT) &&
					(fncpar != IDF_FNC) &&
					(fncpar != IDF_COMMAND_LINE) &&
					(fncpar != IDF_RETURN_CODE));
		}

// polo�en� do koment��e (z�kaz - jen dal�� koment��)
		if (fncpar == IDF_COMMENT) return FALSE;

// do seznamu lze polo�it datov� prom�nn�
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

// skupinu, seznam ani funkci nelze um�stit do vstupn�ch/v�stupn�ch prom�nn�ch
		if (((fnc == IDF_GROUP) || (fnc == IDF_LIST) || (fnc == IDF_FNC)) &&
			((fncpar == IDF_PAR) || (fncpar == IDF_OUT))) return FALSE;

// test, zda je povolen pouze jeden parametr (v�stupn� prom�nn�)
		if ((parent->Param & PR_ONE) && ((item->Prev >= 0) || (item->Next >= 0))) return FALSE;

// do skupiny (p��p. parametr�) lze p�et�hnout prom�nn�, skupinu a seznam
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

// kontrola edita�n�ho bufferu
	default:

// polo�en� koment��e
		if (fnc == IDF_COMMENT)
		{
			return ((parent->Parent < 0) || (((parent->Param & PR_ONE) == 0) && (fncpar != IDF_FNC)));
		}

// polo�en� do koment��e
		if (fncpar == IDF_COMMENT) return TRUE;

// test, zda je povolen pouze jeden parametr
		if ((parent->Param & PR_ONE) && ((item->Prev >= 0) || (item->Next >= 0))) return FALSE;

// polo�en� v�tve CASE
		if (fnc == IDF_CASE_ON) return (fncpar == IDF_CASE);
		if (fnc == IDF_CASE_EQU) return (fncpar == IDF_CASE_ON);

// porovn�n� masky zdroje a c�le ta�en�
		if ((item->SrcMask & parent->DstMask) == 0) return FALSE;

// z�kaz polo�en� polo�ky s parametrem do parametru
		if ((fnc != IDF_FNC) &&									// funkce je povolena, m��e m�t vstupn� parametry
			((parent->DstMask & PR_COMMAND) == 0) &&			// c�lem nejsou p��kazy
			((item->SrcMask & PR_COMMAND) != 0) &&				// zdroj m��e b�t jako p��kaz
			((item->Param & PR_PARPAR) == 0) &&					// krom� p��padu, kdy m� povoleny parametry v�dy
			(item->Child >= 0)) return FALSE;					// polo�ka m� n�jak�ho potomka

// z�kaz polo�en� do polo�ky, kter� nen� p��kazem
		if (((parent->SrcMask & PR_COMMAND) != 0) &&			// c�l m��e b�t jako p��kaz
			((parent->Param & PR_PARPAR) == 0) &&				// krom� p��padu, kdy m� povoleny parametry v�dy
			(parent->Parent >= 0) &&							// c�l je n���m potomkem
			((buf->At(parent->Parent).DstMask & PR_ALLDATA) != 0) &&  // rodi� c�le p�ij�m� data
			((buf->At(parent->Parent).DstMask & PR_COMMAND) == 0))	// p�itom rodi� c�le nep�ij�m� p��kazy (=nen� jako p��kaz)
			return FALSE;

		return TRUE;
	}
}


/////////////////////////////////////////////////////////////////////////////
// navr�cen� z bufferu

void Paste(int bufID)
{
// kontrola, zda neprob�h� obsluha
	if (Lock) return;

// kontrola povolen�ch buffer�
	if ((bufID != BufObjID) &&
		(bufID != BufLocID) &&
		(bufID != BufEdiID))
	{
		return;
	}

// zapnut� p��znaku obsluhy
	Lock = TRUE;

// p��prava v�choz� polo�ky k ulo�en� (m��e b�t i neplatn�!)
	CBufProg* buf = BufProg + bufID;
	int first = buf->Select();

// kontrola polo�ky
	if (buf->IsValid(first) &&
		((buf->At(first).Param & (PR_LOCK | PR_LOCK_DEP)) != 0))
	{
		Lock = FALSE;
		return;
	}

// zapnut� �ekac�ho kurzoru
	BeginWaitCursor();

// otev�en� schr�nky
	if (!::OpenClipboard(MainFrame))
	{
		EndWaitCursor();
		Lock = FALSE;
		return;
	}

// na�ten� dat schr�nky
	HGLOBAL		global;			// glob�ln� buffer s daty
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

// nejsou p�ipravena vhodn� data
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
	
// uzamknut� bufferu
	m_Buf = (BYTE*) ::GlobalLock(global);
	if (m_Buf == NULL)
	{
		::CloseClipboard();
		EndWaitCursor();
		Lock = FALSE;
		return;
	}

// p��prava z�hlav�
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

// kontrola z�hlav�
	if (MemCompare(((CLIPHEAD*)m_Buf)->Ident, head->Ident, 4) &&
		(((CLIPHEAD*)m_Buf)->Data >= SIZEOFCLIPHEAD) &&
		(((CLIPHEAD*)m_Buf)->Pocet > 0) &&
		MemCompare(((CLIPHEAD*)m_Buf)->Clip.Jmeno, head->Clip.Jmeno, 8))
	{
// vynulov�n� p��znaku PR_NEW v objektech
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

// �nicializace tabulky importu funkc�
		ProgFile::InitImportTab();

// p�ednastaven� v�choz� polo�ky lok�ln�ho bufferu
		m_LocFirst = BufLoc.Disp();

// na�ten� dat
		buf->RedrawOff();
		m_Num = ((CLIPHEAD*)m_Buf)->Data;
		if (m_Num > m_Max) m_Num = m_Max;
		int prev = first;
		i = GetProg(bufID, first, ((CLIPHEAD*)m_Buf)->Clip.Pocet);
		if (i >= 0)
		{
			first = i;

// kontrola povolen� operace
			if (buf->IsValid(prev) && !TestPaste(bufID, i))
			{

// p�esun jako potomka polo�ky
				first = buf->Move(prev, -2, first);

// p�i chyb� bude n�hradn� polo�en�
				if (!TestPaste(bufID, first))
				{
					first = buf->Move(buf->Disp(), -2, first);
				}
			}
		}
		buf->RedrawOn();
		buf->Select(first);

// p��znak modifikace souboru
		SetModi();
	}

// odemknut� bufferu
	::GlobalUnlock(global);

// uzav�en� schr�nky
	::CloseClipboard();

// vypnut� �ekac�ho kurzoru
	EndWaitCursor();

// vypnut� p��znaku obsluhy
	Lock = FALSE;

// aktualizace voleb bloku
	UpdateClipboard();

// ukon�en� z�znamu jedn� UNDO operace
	Undo.Stop();
}


/////////////////////////////////////////////////////////////////////////////
// zru�en� bloku

void Delete(int bufID)
{
// kontrola, zda neprob�h� obsluha
	if (Lock) return;

// kontrola povolen�ch buffer�
	if ((bufID != BufObjID) &&
		(bufID != BufLocID) &&
		(bufID != BufEdiID) &&
		(bufID != BufClsID))
	{
		return;
	}

// zapnut� p��znaku obsluhy
	Lock = TRUE;

// p��prava v�choz� polo�ky ke zru�en�
	CBufProg* buf = BufProg + bufID;
	int first = buf->Select();

// kontrola polo�ky
	if (buf->IsNotValid(first) ||
		((buf->At(first).Param & (PR_NOMOVE | PR_INTERN | PR_LOCK | PR_LOCK_DEP)) != 0))
	{
		Lock = FALSE;
		return;
	}

// test zru�en� polo�ky s referencemi
	if (!buf->TestRefer(first)) 
	{
		Lock = FALSE;
		return;
	}

// ru�en� polo�ky v bufferu t��d
	if (bufID == BufClsID)
	{
		ProgLib::Delete(first);
		Lock = FALSE;
		return;
	}

// zru�en� polo�ky
	BeginWaitCursor();
	buf->RedrawOff();
	buf->Del(first);
	SetModi();
	buf->RedrawOn();
	if (EditMode == BufMapID) ProgOnPaint();
	EndWaitCursor();

// vypnut� p��znaku obsluhy
	Lock = FALSE;

// aktualizace informac� o polo�ce, aktualizace menu clipboardu
	ProgAktItem();

// ukon�en� z�znamu jedn� UNDO operace
	Undo.Stop();
}


/////////////////////////////////////////////////////////////////////////////
// vyst�i�en� bloku

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
