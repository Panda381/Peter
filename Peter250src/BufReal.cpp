
#include "Main.h"

/***************************************************************************\
*																			*
*							Buffer reálných èísel							*
*																			*
\***************************************************************************/


////////////////////////////////////////////////////////////////////
// pøidání záznamu o vložení položky do UNDO bufferu

template <> bool CBuffer<CReal>::UndoAddIns(int index)
{
	return Undo.AddRealIns(index);
}

/////////////////////////////////////////////////////////////////////////////
// naètení èísla ze souboru (naèítá do promìnné, TRUE=operace OK)

bool CBufReal::LoadFile(double& data, CText jmeno)
{
// naètení textového souboru
	CText txt;
	if (!txt.LoadFile(jmeno)) return false;

// odstranìní mezer a jiných oddìlovaèù
	if (!txt.TrimLeft() || !txt.TrimRight()) return false;

// naètení èísla
	data = Double(txt);
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// uložení èísla do souboru formátu NUM (FALSE=chyba)

bool CBufReal::SaveFile(double data, CText jmeno) const
{
// pøíprava èísla
	CText txt;
	if (!txt.Double(data)) return false;

// uložení textu do souboru
	return txt.SaveFile(jmeno);
}
