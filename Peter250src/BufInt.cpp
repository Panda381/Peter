
#include "Main.h"

/***************************************************************************\
*																			*
*								Buffer cel�ch ��sel							*
*																			*
\***************************************************************************/

////////////////////////////////////////////////////////////////////
// p�id�n� z�znamu o vlo�en� polo�ky do UNDO bufferu

template <> bool CBuffer<CInt>::UndoAddIns(int index)
{
	return Undo.AddBoolIns(index);
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� logick� hodnoty ze souboru (na��t� do prom�nn�, vrac� TRUE=operace OK)

bool CBufBool::LoadFile(BOOL& data, CText jmeno)
{
// na�ten� textov�ho souboru
	CText txt;
	if (!txt.LoadFile(jmeno)) return false;

// odstran�n� mezer a jin�ch odd�lova��
	if (!txt.TrimLeft() || !txt.TrimRight()) return false;

// konverze na velk� p�smena
	if (!txt.UpperCase()) return false;

// pr�zdn� text je jako FALSE
	if (txt.IsEmpty())
	{
		data = FALSE;
		return true;
	}

// nastaven� dat podle prvn�ho p�smene
	TCHAR znak = txt[0];
	data = ((znak == 'T') ||
			(znak == 'Y') ||
			(znak == 'J') ||
			(znak == 'A') ||
			(znak ==  1 ) ||
			(znak == '1'));

// p��znak operace OK
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// ulo�en� logick� hodnoty do souboru form�tu LOG (vrac� TRUE=operace OK)

bool CBufBool::SaveFile(BOOL data, CText jmeno) const
{
// p��prava textu
	CText txt;
	if (data)
	{
		txt = _T("TRUE");
	}
	else
	{
		txt = _T("FALSE");
	}

// ulo�en� textu do souboru
	return txt.SaveFile(jmeno);
}
