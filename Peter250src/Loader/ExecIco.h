
/***************************************************************************\
*																			*
*							Provádìní programu - ikona						*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inline volání pøíkazu - ikona

inline void FIcon(CIcon& icon)
{
	ExecItem++;
	ExecItem[-1].ExecIco(icon);
}


void _fastcall FIconGet(CIcon& icon);		// naètení ikony z plochy

// funkce
void _fastcall FIconFunc(CIcon& icon);		// funkce s návratem ikony

// promìnné
void _fastcall FIconEmpty(CIcon& icon);		// prázdná ikona
void _fastcall FIconObj(CIcon& icon);		// globální ikona (Data = index)
void _fastcall FIconObjList(CIcon& icon);	// globalní ikona v seznamu (Data = index, List = seznam)
void _fastcall FIconLoc(CIcon& icon);		// lokální ikona (Data = index)
void _fastcall FIconLocList(CIcon& icon);	// lokální ikona v seznamu (Data = index, List = seznam)

// obsluha Petra a Petry
void _fastcall FPredmet(CIcon& icon);		// naètení pøedmìtu pøed Petrem
void _fastcall FPredmet2(CIcon& icon);		// naètení pøedmìtu pøed Petrou
void _fastcall FPredmetPoz(CIcon& icon);	// naètení pøedmìtu pod Petrem
void _fastcall FPredmet2Poz(CIcon& icon);	// naètení pøedmìtu pod Petrou

void _fastcall FGetMapIcon(CIcon& icon);	// naètení pøedmìtu z plochy

void _fastcall FGetWindowIcon(CIcon& icon);	// naètení ikony okna

void _fastcall FGetFileIcon(CIcon& icon);	// naètení ikony ze souboru
