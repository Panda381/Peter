
#include "Main.h"

/***************************************************************************\
*																			*
*								Historie editace							*
*																			*
\***************************************************************************/


namespace HistEdit
{

/////////////////////////////////////////////////////////////////////////////
// položka historie editace

typedef struct HISTITEM_ {
	int		BufID;						// editovaný buffer
	int		Index;						// editovaná položka (-2 = obrázek sprajtu)
	int		Select;						// aktivní položka editoru programu
	int		First;						// horní øádek v programu
	int		Left;						// levý poèátek obrázku
	int		Bottom;						// spodní poèátek obrázku
	int		Sprite;						// index sprajtu editovaného obrázku
	int		Picture;					// editovaný obrázek sprajtu (relativnì)
} HISTITEM;

#define SIZEOFHISTITEM (8*sizeof(int))	// velikost položky

#define MAXHISTITEM 128					// maximální poèet položek v bufferu

/////////////////////////////////////////////////////////////////////////////
// parametry

HISTITEM*	m_Buf = NULL;				// buffer položek historie
int			m_Num = 0;					// poèet položek v bufferu historie
int			m_Max = 0;					// velikost bufferu historie
int			m_Akt = 0;					// aktivní položka v bufferu
int			m_Fill = FALSE;				// probíhá navracení historie


/////////////////////////////////////////////////////////////////////////////
// vynulování bufferu historie

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
// zrušení jedné položky z bufferu

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
// zrušení položek v bufferu (vèetnì editované) podle dat (pro sprajt pic=-1 všechny obrázky)
// pøi chybì pamìti vrací FALSE

bool Del(int bufID, int index, int sprite, int pic)
{
// kontrola, zda je režim editace a zda je to platný buffer
	if (!ProgMode || (bufID < 0)) return true;

// pøíprava ukazatele položek
	BOOL update = FALSE;
	int i = m_Num - 1;
	HISTITEM* item = m_Buf + i;

// cyklus pøes všechny položky dolù
	for (; i >= 0; i--)
	{

// kontrola, zda je to hledaná položka
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

// vypuštìní položky z bufferu
		{
			Del(i);
			item = m_Buf + i;
			update = TRUE;
		}
		item--;
	}

// redukce duplikovaných položek
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

// kontrola, zda to je editovaná položka
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

// není-li v bufferu žádná položka, navrácení hlavního editoru
		if (m_Num == 0)
		{
			m_Fill = TRUE;
			SetEditMode(BufEdiID, 0);
			ProgOnSetFocus(BufProg[FocusBuf].Tree());
			m_Fill = FALSE;
			update = TRUE;
		}
		else

// jinak navrácení z aktivního ukazatele
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
// kontrola, zda je režim editace
	if (!ProgMode) return;

// update voleb
	EnableCommand(IDN_HIST_PREV, m_Akt > 0);	
	EnableCommand(IDN_HIST_NEXT, m_Akt < m_Num-1);	
}


/////////////////////////////////////////////////////////////////////////////
// uložení editovaného prvku do bufferu historie

void Push()
{
// kontrola, zda je režim editace
	if (!ProgMode) return;

// kontrola, zda není ukládání pøíp. zda to není první editace
	if (m_Fill || (EditBuf < 0)) return;

// omezení poètu položek v bufferu
	if (m_Num >= MAXHISTITEM)
	{
		Del(0);
		Update();
	}

// kontrola, zda to není stejná položka
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

// zrušení položek za aktivním ukazatelem
	m_Num = m_Akt + 1;

// zvìtšení bufferu
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

// úschova položky do bufferu
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

// zvýšení ukazatele položek
	m_Akt++;

// aktualizace voleb
	Update();
}


/////////////////////////////////////////////////////////////////////////////
// návrat editovaného prvku z bufferu historie (z aktivního ukazatele)

void Pop()
{
// kontrola, zda je režim editace
	if (!ProgMode) return;

// kontrola povolení operace
	if (m_Fill || (m_Akt >= m_Num)) return;

// pøíznak probíhající zmìny
	m_Fill = TRUE;

// nastavení editované položky
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

// pøíznak ukonèení zmìny
	m_Fill = FALSE;

// aktualizace voleb
	Update();
}


/////////////////////////////////////////////////////////////////////////////
// pøedešlá historie

void OnPrev() 
{
// kontrola, zda je režim editace
	if (!ProgMode) return;

// kontrola, zda je co navracet
	if (m_Akt == 0) return;

// úschova aktuálního stavu, je-li na konci
	if (m_Akt == m_Num)
	{
		Push();
		m_Akt = m_Num-1;
	}

// úschova kurzoru v bufferu
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

// obnovení položky
	Pop();
}


/////////////////////////////////////////////////////////////////////////////
// následující historie

void OnNext() 
{
// kontrola, zda je režim editace
	if (!ProgMode) return;

// kontrola, zda je co navracet
	if (m_Akt >= m_Num - 1) return;

// úschova kurzoru v bufferu
	if ((m_Akt < m_Num) &&
		(EditMode == BufEdiID) &&
		(m_Buf[m_Akt].BufID < PROGBUFNUM))
	{
		m_Buf[m_Akt].First = BufEdi.Top();
		m_Buf[m_Akt].Select = BufEdi.Select();
	}

// posun ukazatele
	m_Akt++;

// obnovení položky
	Pop();
}


}
