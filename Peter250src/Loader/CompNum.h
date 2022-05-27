
/***************************************************************************\
*																			*
*						Kompilace programu - ��slo							*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// p�id�n� jednoho prvku do bufferu programu (vrac� index prvku)

inline int CompAddItem(PROCNUM func)
	{ return CompAddItem((PROCCOM)func); }

inline int CompAddItem(PROCNUM func, int data)
	{ return CompAddItem((PROCCOM)func, data); }

inline int CompAddItem(PROCNUM func, int data, int list)
	{ return CompAddItem((PROCCOM)func, data, list); }


/////////////////////////////////////////////////////////////////////////////
// p�id�n� ��seln� konstanty do bufferu programu

void CompAddItemNum(double data);


/////////////////////////////////////////////////////////////////////////////
// p�eklad ��seln�ho v�razu (vrac� true = operace OK)

bool _fastcall CompNum(int index);


/////////////////////////////////////////////////////////////////////////////
// p�eklad p��kazu s ��seln�m parametrem

bool CompNumPar(int index, PROCCOM func, int data);

inline bool CompNumPar(int index, PROCNUM func, int data)
	{ return CompNumPar(index, (PROCCOM)func, data); }

inline bool CompNumPar(int index, PROCTXT func, int data)
	{ return CompNumPar(index, (PROCCOM)func, data); }

inline bool CompNumPar(int index, PROCLOG func, int data)
	{ return CompNumPar(index, (PROCCOM)func, data); }

inline bool CompNumPar(int index, PROCICO func, int data)
	{ return CompNumPar(index, (PROCCOM)func, data); }

inline bool CompNumPar(int index, PROCMAP func, int data)
	{ return CompNumPar(index, (PROCCOM)func, data); }

inline bool CompNumPar(int index, PROCPIC func, int data)
	{ return CompNumPar(index, (PROCCOM)func, data); }

inline bool CompNumPar(int index, PROCSPR func, int data)
	{ return CompNumPar(index, (PROCCOM)func, data); }

inline bool CompNumPar(int index, PROCSND func, int data)
	{ return CompNumPar(index, (PROCCOM)func, data); }

inline bool CompNumPar(int index, PROCMUS func, int data)
	{ return CompNumPar(index, (PROCCOM)func, data); }


bool CompNumPar(int index, PROCCOM func, int data, int list);

inline bool CompNumPar(int index, PROCNUM func, int data, int list)
	{ return CompNumPar(index, (PROCCOM)func, data, list); }

inline bool CompNumPar(int index, PROCTXT func, int data, int list)
	{ return CompNumPar(index, (PROCCOM)func, data, list); }

inline bool CompNumPar(int index, PROCLOG func, int data, int list)
	{ return CompNumPar(index, (PROCCOM)func, data, list); }

inline bool CompNumPar(int index, PROCICO func, int data, int list)
	{ return CompNumPar(index, (PROCCOM)func, data, list); }

inline bool CompNumPar(int index, PROCMAP func, int data, int list)
	{ return CompNumPar(index, (PROCCOM)func, data, list); }

inline bool CompNumPar(int index, PROCPIC func, int data, int list)
	{ return CompNumPar(index, (PROCCOM)func, data, list); }

inline bool CompNumPar(int index, PROCSPR func, int data, int list)
	{ return CompNumPar(index, (PROCCOM)func, data, list); }

inline bool CompNumPar(int index, PROCSND func, int data, int list)
	{ return CompNumPar(index, (PROCCOM)func, data, list); }

inline bool CompNumPar(int index, PROCMUS func, int data, int list)
	{ return CompNumPar(index, (PROCCOM)func, data, list); }


bool CompNumPar(int index, PROCCOM func);

inline bool CompNumPar(int index, PROCNUM func)
	{ return CompNumPar(index, (PROCCOM)func); }

inline bool CompNumPar(int index, PROCTXT func)
	{ return CompNumPar(index, (PROCCOM)func); }

inline bool CompNumPar(int index, PROCLOG func)
	{ return CompNumPar(index, (PROCCOM)func); }

inline bool CompNumPar(int index, PROCICO func)
	{ return CompNumPar(index, (PROCCOM)func); }

inline bool CompNumPar(int index, PROCMAP func)
	{ return CompNumPar(index, (PROCCOM)func); }

inline bool CompNumPar(int index, PROCPIC func)
	{ return CompNumPar(index, (PROCCOM)func); }

inline bool CompNumPar(int index, PROCSPR func)
	{ return CompNumPar(index, (PROCCOM)func); }

inline bool CompNumPar(int index, PROCSND func)
	{ return CompNumPar(index, (PROCCOM)func); }

inline bool CompNumPar(int index, PROCMUS func)
	{ return CompNumPar(index, (PROCCOM)func); }


/////////////////////////////////////////////////////////////////////////////
// p�eklad p��kazu s ��seln�m podparametrem (hledan�m podle identifikace)

void CompNumSubPar(int index, int idf, double def);


/////////////////////////////////////////////////////////////////////////////
// p�eklad p��kazu se skupinou ��seln�ch v�raz� (data = po�et v�raz� - 1)

inline bool CompNumGrp(int index, PROCNUM func, PROCNUM func1)
	{ return CompGroup(CompNum, index, (PROCCOM)func, (PROCCOM)func1); };

inline bool CompNumGrp(int index, PROCLOG func, PROCLOG func1)
	{ return CompGroup(CompNum, index, (PROCCOM)func, (PROCCOM)func1); };

inline bool CompNumGrp(int index, PROCCOM func, PROCCOM func1)
	{ return CompGroup(CompNum, index, (PROCCOM)func, (PROCCOM)func1); };
