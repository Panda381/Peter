
/***************************************************************************\
*																			*
*						Kompilace programu - pøíkaz							*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// pøidání jednoho prvku do bufferu programu (vrací index prvku)

int CompAddItem(PROCCOM func);
int CompAddItem(PROCCOM func, int data);
int CompAddItem(PROCCOM func, int data, int list);


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu (vrací true = operace OK)

bool _fastcall CompCom(int index);


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu se skupinou pøíkazù (data = poèet pøíkazù - 1)

inline bool CompComGrp(int index, PROCCOM func, PROCCOM func1)
	{ return CompGroup(CompCom, index, (PROCCOM)func, (PROCCOM)func1); };


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu s podparametrem pøíkazù (hledaným podle identifikace), Jump = pøeskoèení skupiny

void CompComSubPar(int index, int idf);
