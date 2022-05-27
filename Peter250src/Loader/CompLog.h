
/***************************************************************************\
*																			*
*					Kompilace programu - logický výraz						*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// pøidání jednoho prvku do bufferu programu (vrací index prvku)

inline int CompAddItem(PROCLOG func)
	{ return CompAddItem((PROCCOM)func); }

inline int CompAddItem(PROCLOG func, int data)
	{ return CompAddItem((PROCCOM)func, data); }

inline int CompAddItem(PROCLOG func, int data, int list)
	{ return CompAddItem((PROCCOM)func, data, list); }


/////////////////////////////////////////////////////////////////////////////
// pøeklad logického výrazu (vrací true = operace OK)

bool _fastcall CompLog(int index);


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu s logickým parametrem

bool CompLogPar(int index, PROCCOM func, int data);

inline bool CompLogPar(int index, PROCNUM func, int data)
	{ return CompLogPar(index, (PROCCOM)func, data); }

inline bool CompLogPar(int index, PROCTXT func, int data)
	{ return CompLogPar(index, (PROCCOM)func, data); }

inline bool CompLogPar(int index, PROCLOG func, int data)
	{ return CompLogPar(index, (PROCCOM)func, data); }

inline bool CompLogPar(int index, PROCICO func, int data)
	{ return CompLogPar(index, (PROCCOM)func, data); }

inline bool CompLogPar(int index, PROCMAP func, int data)
	{ return CompLogPar(index, (PROCCOM)func, data); }

inline bool CompLogPar(int index, PROCPIC func, int data)
	{ return CompLogPar(index, (PROCCOM)func, data); }

inline bool CompLogPar(int index, PROCSPR func, int data)
	{ return CompLogPar(index, (PROCCOM)func, data); }

inline bool CompLogPar(int index, PROCSND func, int data)
	{ return CompLogPar(index, (PROCCOM)func, data); }

inline bool CompLogPar(int index, PROCMUS func, int data)
	{ return CompLogPar(index, (PROCCOM)func, data); }


bool CompLogPar(int index, PROCCOM func, int data, int list);

inline bool CompLogPar(int index, PROCNUM func, int data, int list)
	{ return CompLogPar(index, (PROCCOM)func, data, list); }

inline bool CompLogPar(int index, PROCTXT func, int data, int list)
	{ return CompLogPar(index, (PROCCOM)func, data, list); }

inline bool CompLogPar(int index, PROCLOG func, int data, int list)
	{ return CompLogPar(index, (PROCCOM)func, data, list); }

inline bool CompLogPar(int index, PROCICO func, int data, int list)
	{ return CompLogPar(index, (PROCCOM)func, data, list); }

inline bool CompLogPar(int index, PROCMAP func, int data, int list)
	{ return CompLogPar(index, (PROCCOM)func, data, list); }

inline bool CompLogPar(int index, PROCPIC func, int data, int list)
	{ return CompLogPar(index, (PROCCOM)func, data, list); }

inline bool CompLogPar(int index, PROCSPR func, int data, int list)
	{ return CompLogPar(index, (PROCCOM)func, data, list); }

inline bool CompLogPar(int index, PROCSND func, int data, int list)
	{ return CompLogPar(index, (PROCCOM)func, data, list); }

inline bool CompLogPar(int index, PROCMUS func, int data, int list)
	{ return CompLogPar(index, (PROCCOM)func, data, list); }


bool CompLogPar(int index, PROCCOM func);

inline bool CompLogPar(int index, PROCNUM func)
	{ return CompLogPar(index, (PROCCOM)func); }

inline bool CompLogPar(int index, PROCTXT func)
	{ return CompLogPar(index, (PROCCOM)func); }

inline bool CompLogPar(int index, PROCLOG func)
	{ return CompLogPar(index, (PROCCOM)func); }

inline bool CompLogPar(int index, PROCICO func)
	{ return CompLogPar(index, (PROCCOM)func); }

inline bool CompLogPar(int index, PROCMAP func)
	{ return CompLogPar(index, (PROCCOM)func); }

inline bool CompLogPar(int index, PROCPIC func)
	{ return CompLogPar(index, (PROCCOM)func); }

inline bool CompLogPar(int index, PROCSPR func)
	{ return CompLogPar(index, (PROCCOM)func); }

inline bool CompLogPar(int index, PROCSND func)
	{ return CompLogPar(index, (PROCCOM)func); }

inline bool CompLogPar(int index, PROCMUS func)
	{ return CompLogPar(index, (PROCCOM)func); }


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu se skupinou logických výrazù (data = poèet výrazù - 1)

inline bool CompLogGrp(int index, PROCLOG func, PROCLOG func1)
	{ return CompGroup(CompLog, index, (PROCCOM)func, (PROCCOM)func1); };


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu s logickým podparametrem (hledaným podle identifikace)

void CompLogSubPar(int index, int idf, bool def);
