
#include "Main.h"

/***************************************************************************\
*																			*
*							Buffer re�ln�ch ��sel							*
*																			*
\***************************************************************************/


////////////////////////////////////////////////////////////////////
// p�id�n� z�znamu o vlo�en� polo�ky do UNDO bufferu

template <> bool CBuffer<CReal>::UndoAddIns(int index)
{
	return Undo.AddRealIns(index);
}

/////////////////////////////////////////////////////////////////////////////
// na�ten� ��sla ze souboru (na��t� do prom�nn�, TRUE=operace OK)

bool CBufReal::LoadFile(double& data, CText jmeno)
{
// na�ten� textov�ho souboru
	CText txt;
	if (!txt.LoadFile(jmeno)) return false;

// odstran�n� mezer a jin�ch odd�lova��
	if (!txt.TrimLeft() || !txt.TrimRight()) return false;

// na�ten� ��sla
	data = Double(txt);
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// ulo�en� ��sla do souboru form�tu NUM (FALSE=chyba)

bool CBufReal::SaveFile(double data, CText jmeno) const
{
// p��prava ��sla
	CText txt;
	if (!txt.Double(data)) return false;

// ulo�en� textu do souboru
	return txt.SaveFile(jmeno);
}
