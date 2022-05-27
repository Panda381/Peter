
/***************************************************************************\
*																			*
*						Kompilace programu - zvuk							*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// p�id�n� jednoho prvku do bufferu programu (vrac� index prvku)

inline int CompAddItem(PROCSND func)
	{ return CompAddItem((PROCCOM)func); }

inline int CompAddItem(PROCSND func, int data)
	{ return CompAddItem((PROCCOM)func, data); }

inline int CompAddItem(PROCSND func, int data, int list)
	{ return CompAddItem((PROCCOM)func, data, list); }


/////////////////////////////////////////////////////////////////////////////
// p�eklad v�razu se zvukem (vrac� true = operace OK)

bool _fastcall CompSnd(int index);


/////////////////////////////////////////////////////////////////////////////
// p�eklad p��kazu s parametrem zvuku

bool CompSndPar(int index, PROCCOM func, int data);

inline bool CompSndPar(int index, PROCNUM func, int data)
	{ return CompSndPar(index, (PROCCOM)func, data); }

inline bool CompSndPar(int index, PROCTXT func, int data)
	{ return CompSndPar(index, (PROCCOM)func, data); }

inline bool CompSndPar(int index, PROCLOG func, int data)
	{ return CompSndPar(index, (PROCCOM)func, data); }

inline bool CompSndPar(int index, PROCICO func, int data)
	{ return CompSndPar(index, (PROCCOM)func, data); }

inline bool CompSndPar(int index, PROCMAP func, int data)
	{ return CompSndPar(index, (PROCCOM)func, data); }

inline bool CompSndPar(int index, PROCPIC func, int data)
	{ return CompSndPar(index, (PROCCOM)func, data); }

inline bool CompSndPar(int index, PROCSPR func, int data)
	{ return CompSndPar(index, (PROCCOM)func, data); }

inline bool CompSndPar(int index, PROCSND func, int data)
	{ return CompSndPar(index, (PROCCOM)func, data); }

inline bool CompSndPar(int index, PROCMUS func, int data)
	{ return CompSndPar(index, (PROCCOM)func, data); }


bool CompSndPar(int index, PROCCOM func, int data, int list);

inline bool CompSndPar(int index, PROCNUM func, int data, int list)
	{ return CompSndPar(index, (PROCCOM)func, data, list); }

inline bool CompSndPar(int index, PROCTXT func, int data, int list)
	{ return CompSndPar(index, (PROCCOM)func, data, list); }

inline bool CompSndPar(int index, PROCLOG func, int data, int list)
	{ return CompSndPar(index, (PROCCOM)func, data, list); }

inline bool CompSndPar(int index, PROCICO func, int data, int list)
	{ return CompSndPar(index, (PROCCOM)func, data, list); }

inline bool CompSndPar(int index, PROCMAP func, int data, int list)
	{ return CompSndPar(index, (PROCCOM)func, data, list); }

inline bool CompSndPar(int index, PROCPIC func, int data, int list)
	{ return CompSndPar(index, (PROCCOM)func, data, list); }

inline bool CompSndPar(int index, PROCSPR func, int data, int list)
	{ return CompSndPar(index, (PROCCOM)func, data, list); }

inline bool CompSndPar(int index, PROCSND func, int data, int list)
	{ return CompSndPar(index, (PROCCOM)func, data, list); }

inline bool CompSndPar(int index, PROCMUS func, int data, int list)
	{ return CompSndPar(index, (PROCCOM)func, data, list); }


bool CompSndPar(int index, PROCCOM func);

inline bool CompSndPar(int index, PROCNUM func)
	{ return CompSndPar(index, (PROCCOM)func); }

inline bool CompSndPar(int index, PROCTXT func)
	{ return CompSndPar(index, (PROCCOM)func); }

inline bool CompSndPar(int index, PROCLOG func)
	{ return CompSndPar(index, (PROCCOM)func); }

inline bool CompSndPar(int index, PROCICO func)
	{ return CompSndPar(index, (PROCCOM)func); }

inline bool CompSndPar(int index, PROCMAP func)
	{ return CompSndPar(index, (PROCCOM)func); }

inline bool CompSndPar(int index, PROCPIC func)
	{ return CompSndPar(index, (PROCCOM)func); }

inline bool CompSndPar(int index, PROCSPR func)
	{ return CompSndPar(index, (PROCCOM)func); }

inline bool CompSndPar(int index, PROCSND func)
	{ return CompSndPar(index, (PROCCOM)func); }

inline bool CompSndPar(int index, PROCMUS func)
	{ return CompSndPar(index, (PROCCOM)func); }


/////////////////////////////////////////////////////////////////////////////
// p�eklad p��kazu s podparametrem zvuku (hledan�m podle identifikace)

void CompSndSubPar(int index, int idf);


/////////////////////////////////////////////////////////////////////////////
// p�eklad p��kazu se skupinou zvukov�ch parametr� (data = po�et parametr� - 1)

inline bool CompSndGrp(int index, PROCSND func, PROCSND func1)
	{ return CompGroup(CompSnd, index, (PROCCOM)func, (PROCCOM)func1); };

inline bool CompSndGrp(int index, PROCLOG func, PROCLOG func1)
	{ return CompGroup(CompSnd, index, (PROCCOM)func, (PROCCOM)func1); };
