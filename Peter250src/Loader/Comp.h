
/***************************************************************************\
*																			*
*							Kompilace programu								*
*																			*
\***************************************************************************/

// korekce lok�ln�ch prom�nn�ch (p�i vol�n� ve vstupn�ch parametrech)

extern int KorigNum;
extern int KorigLog;
extern int KorigIco;
extern int KorigMap;
extern int KorigMus;
extern int KorigPic;
extern int KorigSnd;
extern int KorigSpr;
extern int KorigTxt;
extern int KorigLst;


typedef bool (_fastcall *COMPCOMP) (int index);

/////////////////////////////////////////////////////////////////////////////
// p�eklad p��kazu se skupinou (data = po�et parametr�)

bool CompGroup(COMPCOMP comp, int index, PROCCOM func, PROCCOM func1);


/////////////////////////////////////////////////////////////////////////////
// p�eklad vol�n� funkce (typ = ID prom�nn� v�sledku, IDF_FNC = nen�)

bool CompFunc(int index, int typ);


/////////////////////////////////////////////////////////////////////////////
// kompilace programu

void Comp();
