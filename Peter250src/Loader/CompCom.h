
/***************************************************************************\
*																			*
*						Kompilace programu - p��kaz							*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// p�id�n� jednoho prvku do bufferu programu (vrac� index prvku)

int CompAddItem(PROCCOM func);
int CompAddItem(PROCCOM func, int data);
int CompAddItem(PROCCOM func, int data, int list);


/////////////////////////////////////////////////////////////////////////////
// p�eklad p��kazu (vrac� true = operace OK)

bool _fastcall CompCom(int index);


/////////////////////////////////////////////////////////////////////////////
// p�eklad p��kazu se skupinou p��kaz� (data = po�et p��kaz� - 1)

inline bool CompComGrp(int index, PROCCOM func, PROCCOM func1)
	{ return CompGroup(CompCom, index, (PROCCOM)func, (PROCCOM)func1); };


/////////////////////////////////////////////////////////////////////////////
// p�eklad p��kazu s podparametrem p��kaz� (hledan�m podle identifikace), Jump = p�esko�en� skupiny

void CompComSubPar(int index, int idf);
