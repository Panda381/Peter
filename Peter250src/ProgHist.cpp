
#include "Main.h"

/***************************************************************************\
*																			*
*								Historie editace							*
*																			*
\***************************************************************************/


namespace HistEdit
{

/////////////////////////////////////////////////////////////////////////////
// polo�ka historie editace

typedef struct HISTITEM_ {
	int		BufID;						// editovan� buffer
	int		Index;						// editovan� polo�ka (-2 = obr�zek sprajtu)
	int		Select;						// aktivn� polo�ka editoru programu
	int		First;						// horn� ��dek v programu
	int		Left;						// lev� po��tek obr�zku
	int		Bottom;						// spodn� po��tek obr�zku
	int		Sprite;						// index sprajtu editovan�ho obr�zku
	int		Picture;					// editovan� obr�zek sprajtu (relativn�)
} HISTITEM;

#define SIZEOFHISTITEM (8*sizeof(int))	// velikost polo�ky

#define MAXHISTITEM 128					// maxim�ln� po�et polo�ek v bufferu

/////////////////////////////////////////////////////////////////////////////
// parametry

HISTITEM*	m_Buf = NULL;				// buffer polo�ek historie
int			m_Num = 0;					// po�et polo�ek v bufferu historie
int			m_Max = 0;					// velikost bufferu historie
int			m_Akt = 0;					// aktivn� polo�ka v bufferu
int			m_Fill = FALSE;				// prob�h� navracen� historie


/////////////////////////////////////////////////////////////////////////////
// vynulov�n� bufferu historie

void DelAll()
{
	MemFree(m_Buf);
	m_Buf = NULL;
	m_Num = 0;
	m_Max = 0;
	m_Akt = 0;
	m_Fill = FALSE;
	Update();
}


/////////////////////////////////////////////////////////////////////////////
// zru�en� jedn� polo�ky z bufferu

void Del(int index)
{
	ASSERT((DWORD)index < (DWORD)m_Num);
	if ((DWORD)index < (DWORD)m_Num)
	{
		if (index < m_Num - 1)
		{
			MemCopy(m_Buf + index, m_Buf + index + 1, 
				(m_Num - 1 - index) * SIZEOFHISTITEM);
		}
		m_Num--;
		if (m_Akt > index) m_Akt--;
	}
}


/////////////////////////////////////////////////////////////////////////////
// zru�en� polo�ek v bufferu (v�etn� editovan�) podle dat (pro sprajt pic=-1 v�echny obr�zky)
// p�i chyb� pam�ti vrac� FALSE

bool Del(int bufID, int index, int sprite, int pic)
{
// kontrola, zda je re�im editace a zda je to platn� buffer
	if (!ProgMode || (bufID < 0)) return true;

// p��prava ukazatele polo�ek
	BOOL update = FALSE;
	int i = m_Num - 1;
	HISTITEM* item = m_Buf + i;

// cyklus p�es v�echny polo�ky dol�
	for (; i >= 0; i--)
	{

// kontrola, zda je to hledan� polo�ka
		if	(
				(item->Index == index) &&
				(item->BufID == bufID) &&
				(
					(index != -2) ||
					(bufID != BufPicID) ||
					(
						(item->Sprite == sprite) &&
						(
							(item->Picture == pic) ||
							(pic < 0)
						)
					)
				)
			)

// vypu�t�n� polo�ky z bufferu
		{
			Del(i);
			item = m_Buf + i;
			update = TRUE;
		}
		item--;
	}

// redukce duplikovan�ch polo�ek
	if (update)
	{
		i = m_Num - 1;
		item = m_Buf + i;
		for (; i > 0; i--)
		{
			if (	
					(item[0].Index == item[-1].Index) &&
					(item[0].BufID == item[-1].BufID) &&
					(
						(item[0].Index != -2) ||
						(item[0].BufID != BufPicID) ||
						(
							(item[0].Sprite == item[-1].Sprite) &&
							(item[0].Picture == item[-1].Picture)
						)
					)
				)
			{
				Del(i);
				item = m_Buf + i;
			}
			item--;
		}
	}

// kontrola, zda to je editovan� polo�ka
	if	(
			(EditItem == index) &&
			(EditBuf == bufID) &&
			(
				(index != -2) ||
				(bufID != BufPicID) ||
				(
					(EditItemSprite == sprite) &&
					(
						(EditItemPic == pic) ||
						(pic < 0)
					)
				)
			)
		)
	{

// nen�-li v bufferu ��dn� polo�ka, navr�cen� hlavn�ho editoru
		if (m_Num == 0)
		{
			m_Fill = TRUE;
			SetEditMode(BufEdiID, 0);
			ProgOnSetFocus(BufProg[FocusBuf].Tree());
			m_Fill = FALSE;
			update = TRUE;
		}
		else

// jinak navr�cen� z aktivn�ho ukazatele
		{
			if (m_Akt >= m_Num)
			{
				m_Akt = m_Num - 1;
			}
			Pop();
		}
	}

// aktualizace menu
	if (update) Update();

	return true;
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace voleb pro obsluhu historie

void Update()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// update voleb
	EnableCommand(IDN_HIST_PREV, m_Akt > 0);	
	EnableCommand(IDN_HIST_NEXT, m_Akt < m_Num-1);	
}


/////////////////////////////////////////////////////////////////////////////
// ulo�en� editovan�ho prvku do bufferu historie

void Push()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// kontrola, zda nen� ukl�d�n� p��p. zda to nen� prvn� editace
	if (m_Fill || (EditBuf < 0)) return;

// omezen� po�tu polo�ek v bufferu
	if (m_Num >= MAXHISTITEM)
	{
		Del(0);
		Update();
	}

// kontrola, zda to nen� stejn� polo�ka
	if (m_Akt > 0)
	{
		HISTITEM* item = m_Buf + m_Akt - 1;

		if ((EditBuf == item->BufID) &&
			(EditItem == item->Index))
		{
			if ((EditBuf == BufPicID) &&
				(EditItem == -2))
			{
				if ((EditItemSprite == item->Sprite) &&
					(EditItemPic == item->Picture))
				{
					return;
				}
			}
			else
			{
				return;
			}
		}
	}

// zru�en� polo�ek za aktivn�m ukazatelem
	m_Num = m_Akt + 1;

// zv�t�en� bufferu
	if (m_Num > m_Max)
	{
		m_Max += 128;
		HISTITEM* buf = (HISTITEM*)MemSize(m_Buf, m_Max * sizeof(HISTITEM));
		if (buf == NULL)
		{
			m_Max -= 128;
			m_Num = m_Max;
			m_Akt = m_Num - 1;
			if (m_Akt < 0) m_Akt = 0;
			return;
		}
		m_Buf = buf;
	}

// �schova polo�ky do bufferu
	HISTITEM* item = m_Buf + m_Akt;
	item->BufID = EditBuf;
	item->Index = EditItem;
	item->Sprite = EditItemSprite;
	item->Picture = EditItemPic;
	if (EditBuf < PROGBUFNUM)
	{
		item->First = BufEdi.Top();
		item->Select = BufEdi.Select();
	}

// zv��en� ukazatele polo�ek
	m_Akt++;

// aktualizace voleb
	Update();
}


/////////////////////////////////////////////////////////////////////////////
// n�vrat editovan�ho prvku z bufferu historie (z aktivn�ho ukazatele)

void Pop()
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// kontrola povolen� operace
	if (m_Fill || (m_Akt >= m_Num)) return;

// p��znak prob�haj�c� zm�ny
	m_Fill = TRUE;

// nastaven� editovan� polo�ky
	HISTITEM* item = m_Buf + m_Akt;
	if ((item->BufID == BufPicID) && (item->Index == -2))
	{
		EditSprite::Index = item->Sprite;
		EditSprite::IndexPic = item->Picture;
	}
	SetEditMode(item->BufID, item->Index);

	if (item->BufID < PROGBUFNUM)
	{
		BufEdi.Top(item->First);
		BufEdi.Select(item->Select);
	}

// aktualizace fokusu
	ProgSetFocus();

// p��znak ukon�en� zm�ny
	m_Fill = FALSE;

// aktualizace voleb
	Update();
}


/////////////////////////////////////////////////////////////////////////////
// p�ede�l� historie

void OnPrev() 
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// kontrola, zda je co navracet
	if (m_Akt == 0) return;

// �schova aktu�ln�ho stavu, je-li na konci
	if (m_Akt == m_Num)
	{
		Push();
		m_Akt = m_Num-1;
	}

// �schova kurzoru v bufferu
	if ((m_Akt < m_Num) &&
		(EditMode == BufEdiID) &&
		(m_Buf[m_Akt].BufID < PROGBUFNUM))
	{
		m_Buf[m_Akt].First = BufEdi.Top();
		m_Buf[m_Akt].Select = BufEdi.Select();
	}

// posun ukazatele v bufferu
	m_Akt--;
	if (m_Akt < 0) m_Akt = 0;

// obnoven� polo�ky
	Pop();
}


/////////////////////////////////////////////////////////////////////////////
// n�sleduj�c� historie

void OnNext() 
{
// kontrola, zda je re�im editace
	if (!ProgMode) return;

// kontrola, zda je co navracet
	if (m_Akt >= m_Num - 1) return;

// �schova kurzoru v bufferu
	if ((m_Akt < m_Num) &&
		(EditMode == BufEdiID) &&
		(m_Buf[m_Akt].BufID < PROGBUFNUM))
	{
		m_Buf[m_Akt].First = BufEdi.Top();
		m_Buf[m_Akt].Select = BufEdi.Select();
	}

// posun ukazatele
	m_Akt++;

// obnoven� polo�ky
	Pop();
}


}
