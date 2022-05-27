
/***************************************************************************\
*																			*
*							Prov�d�n� programu - ikona						*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inline vol�n� p��kazu - ikona

inline void FIcon(CIcon& icon)
{
	ExecItem++;
	ExecItem[-1].ExecIco(icon);
}


void _fastcall FIconGet(CIcon& icon);		// na�ten� ikony z plochy

// funkce
void _fastcall FIconFunc(CIcon& icon);		// funkce s n�vratem ikony

// prom�nn�
void _fastcall FIconEmpty(CIcon& icon);		// pr�zdn� ikona
void _fastcall FIconObj(CIcon& icon);		// glob�ln� ikona (Data = index)
void _fastcall FIconObjList(CIcon& icon);	// globaln� ikona v seznamu (Data = index, List = seznam)
void _fastcall FIconLoc(CIcon& icon);		// lok�ln� ikona (Data = index)
void _fastcall FIconLocList(CIcon& icon);	// lok�ln� ikona v seznamu (Data = index, List = seznam)

// obsluha Petra a Petry
void _fastcall FPredmet(CIcon& icon);		// na�ten� p�edm�tu p�ed Petrem
void _fastcall FPredmet2(CIcon& icon);		// na�ten� p�edm�tu p�ed Petrou
void _fastcall FPredmetPoz(CIcon& icon);	// na�ten� p�edm�tu pod Petrem
void _fastcall FPredmet2Poz(CIcon& icon);	// na�ten� p�edm�tu pod Petrou

void _fastcall FGetMapIcon(CIcon& icon);	// na�ten� p�edm�tu z plochy

void _fastcall FGetWindowIcon(CIcon& icon);	// na�ten� ikony okna

void _fastcall FGetFileIcon(CIcon& icon);	// na�ten� ikony ze souboru
