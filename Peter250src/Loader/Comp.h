
/***************************************************************************\
*																			*
*							Kompilace programu								*
*																			*
\***************************************************************************/

// korekce lokálních promìnných (pøi volání ve vstupních parametrech)

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
// pøeklad pøíkazu se skupinou (data = poèet parametrù)

bool CompGroup(COMPCOMP comp, int index, PROCCOM func, PROCCOM func1);


/////////////////////////////////////////////////////////////////////////////
// pøeklad volání funkce (typ = ID promìnné výsledku, IDF_FNC = není)

bool CompFunc(int index, int typ);


/////////////////////////////////////////////////////////////////////////////
// kompilace programu

void Comp();
