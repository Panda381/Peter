
/***************************************************************************\
*																			*
*						Kompilace programu - ikona							*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// pøidání jednoho prvku do bufferu programu (vrací index prvku)

inline int CompAddItem(PROCICO func)
	{ return CompAddItem((PROCCOM)func); }

inline int CompAddItem(PROCICO func, int data)
	{ return CompAddItem((PROCCOM)func, data); }

inline int CompAddItem(PROCICO func, int data, int list)
	{ return CompAddItem((PROCCOM)func, data, list); }


/////////////////////////////////////////////////////////////////////////////
// pøeklad výrazu s ikonou (vrací true = operace OK)

bool _fastcall CompIco(int index);


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu s parametrem ikony

bool CompIcoPar(int index, PROCCOM func, int data);

inline bool CompIcoPar(int index, PROCNUM func, int data)
	{ return CompIcoPar(index, (PROCCOM)func, data); }

inline bool CompIcoPar(int index, PROCTXT func, int data)
	{ return CompIcoPar(index, (PROCCOM)func, data); }

inline bool CompIcoPar(int index, PROCLOG func, int data)
	{ return CompIcoPar(index, (PROCCOM)func, data); }

inline bool CompIcoPar(int index, PROCICO func, int data)
	{ return CompIcoPar(index, (PROCCOM)func, data); }

inline bool CompIcoPar(int index, PROCMAP func, int data)
	{ return CompIcoPar(index, (PROCCOM)func, data); }

inline bool CompIcoPar(int index, PROCPIC func, int data)
	{ return CompIcoPar(index, (PROCCOM)func, data); }

inline bool CompIcoPar(int index, PROCSPR func, int data)
	{ return CompIcoPar(index, (PROCCOM)func, data); }

inline bool CompIcoPar(int index, PROCSND func, int data)
	{ return CompIcoPar(index, (PROCCOM)func, data); }

inline bool CompIcoPar(int index, PROCMUS func, int data)
	{ return CompIcoPar(index, (PROCCOM)func, data); }


bool CompIcoPar(int index, PROCCOM func, int data, int list);

inline bool CompIcoPar(int index, PROCNUM func, int data, int list)
	{ return CompIcoPar(index, (PROCCOM)func, data, list); }

inline bool CompIcoPar(int index, PROCTXT func, int data, int list)
	{ return CompIcoPar(index, (PROCCOM)func, data, list); }

inline bool CompIcoPar(int index, PROCLOG func, int data, int list)
	{ return CompIcoPar(index, (PROCCOM)func, data, list); }

inline bool CompIcoPar(int index, PROCICO func, int data, int list)
	{ return CompIcoPar(index, (PROCCOM)func, data, list); }

inline bool CompIcoPar(int index, PROCMAP func, int data, int list)
	{ return CompIcoPar(index, (PROCCOM)func, data, list); }

inline bool CompIcoPar(int index, PROCPIC func, int data, int list)
	{ return CompIcoPar(index, (PROCCOM)func, data, list); }

inline bool CompIcoPar(int index, PROCSPR func, int data, int list)
	{ return CompIcoPar(index, (PROCCOM)func, data, list); }

inline bool CompIcoPar(int index, PROCSND func, int data, int list)
	{ return CompIcoPar(index, (PROCCOM)func, data, list); }

inline bool CompIcoPar(int index, PROCMUS func, int data, int list)
	{ return CompIcoPar(index, (PROCCOM)func, data, list); }


bool CompIcoPar(int index, PROCCOM func);

inline bool CompIcoPar(int index, PROCNUM func)
	{ return CompIcoPar(index, (PROCCOM)func); }

inline bool CompIcoPar(int index, PROCTXT func)
	{ return CompIcoPar(index, (PROCCOM)func); }

inline bool CompIcoPar(int index, PROCLOG func)
	{ return CompIcoPar(index, (PROCCOM)func); }

inline bool CompIcoPar(int index, PROCICO func)
	{ return CompIcoPar(index, (PROCCOM)func); }

inline bool CompIcoPar(int index, PROCMAP func)
	{ return CompIcoPar(index, (PROCCOM)func); }

inline bool CompIcoPar(int index, PROCPIC func)
	{ return CompIcoPar(index, (PROCCOM)func); }

inline bool CompIcoPar(int index, PROCSPR func)
	{ return CompIcoPar(index, (PROCCOM)func); }

inline bool CompIcoPar(int index, PROCSND func)
	{ return CompIcoPar(index, (PROCCOM)func); }

inline bool CompIcoPar(int index, PROCMUS func)
	{ return CompIcoPar(index, (PROCCOM)func); }


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu se skupinou ikon (data = poèet výrazù - 1)

inline bool CompIcoGrp(int index, PROCLOG func, PROCLOG func1)
	{ return CompGroup(CompIco, index, (PROCCOM)func, (PROCCOM)func1); };


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu s podparametrem ikony (hledaným podle identifikace)

void CompIcoSubPar(int index, int idf);
