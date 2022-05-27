
/***************************************************************************\
*																			*
*						Kompilace programu - sprajt							*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// pøidání jednoho prvku do bufferu programu (vrací index prvku)

inline int CompAddItem(PROCSPR func)
	{ return CompAddItem((PROCCOM)func); }

inline int CompAddItem(PROCSPR func, int data)
	{ return CompAddItem((PROCCOM)func, data); }

inline int CompAddItem(PROCSPR func, int data, int list)
	{ return CompAddItem((PROCCOM)func, data, list); }


/////////////////////////////////////////////////////////////////////////////
// pøeklad výrazu se sprajtem (vrací true = operace OK)

bool _fastcall CompSpr(int index);


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu s parametrem sprajtu

bool CompSprPar(int index, PROCCOM func, int data);

inline bool CompSprPar(int index, PROCNUM func, int data)
	{ return CompSprPar(index, (PROCCOM)func, data); }

inline bool CompSprPar(int index, PROCTXT func, int data)
	{ return CompSprPar(index, (PROCCOM)func, data); }

inline bool CompSprPar(int index, PROCLOG func, int data)
	{ return CompSprPar(index, (PROCCOM)func, data); }

inline bool CompSprPar(int index, PROCICO func, int data)
	{ return CompSprPar(index, (PROCCOM)func, data); }

inline bool CompSprPar(int index, PROCMAP func, int data)
	{ return CompSprPar(index, (PROCCOM)func, data); }

inline bool CompSprPar(int index, PROCPIC func, int data)
	{ return CompSprPar(index, (PROCCOM)func, data); }

inline bool CompSprPar(int index, PROCSPR func, int data)
	{ return CompSprPar(index, (PROCCOM)func, data); }

inline bool CompSprPar(int index, PROCSND func, int data)
	{ return CompSprPar(index, (PROCCOM)func, data); }

inline bool CompSprPar(int index, PROCMUS func, int data)
	{ return CompSprPar(index, (PROCCOM)func, data); }


bool CompSprPar(int index, PROCCOM func, int data, int list);

inline bool CompSprPar(int index, PROCNUM func, int data, int list)
	{ return CompSprPar(index, (PROCCOM)func, data, list); }

inline bool CompSprPar(int index, PROCTXT func, int data, int list)
	{ return CompSprPar(index, (PROCCOM)func, data, list); }

inline bool CompSprPar(int index, PROCLOG func, int data, int list)
	{ return CompSprPar(index, (PROCCOM)func, data, list); }

inline bool CompSprPar(int index, PROCICO func, int data, int list)
	{ return CompSprPar(index, (PROCCOM)func, data, list); }

inline bool CompSprPar(int index, PROCMAP func, int data, int list)
	{ return CompSprPar(index, (PROCCOM)func, data, list); }

inline bool CompSprPar(int index, PROCPIC func, int data, int list)
	{ return CompSprPar(index, (PROCCOM)func, data, list); }

inline bool CompSprPar(int index, PROCSPR func, int data, int list)
	{ return CompSprPar(index, (PROCCOM)func, data, list); }

inline bool CompSprPar(int index, PROCSND func, int data, int list)
	{ return CompSprPar(index, (PROCCOM)func, data, list); }

inline bool CompSprPar(int index, PROCMUS func, int data, int list)
	{ return CompSprPar(index, (PROCCOM)func, data, list); }


bool CompSprPar(int index, PROCCOM func);

inline bool CompSprPar(int index, PROCNUM func)
	{ return CompSprPar(index, (PROCCOM)func); }

inline bool CompSprPar(int index, PROCTXT func)
	{ return CompSprPar(index, (PROCCOM)func); }

inline bool CompSprPar(int index, PROCLOG func)
	{ return CompSprPar(index, (PROCCOM)func); }

inline bool CompSprPar(int index, PROCICO func)
	{ return CompSprPar(index, (PROCCOM)func); }

inline bool CompSprPar(int index, PROCMAP func)
	{ return CompSprPar(index, (PROCCOM)func); }

inline bool CompSprPar(int index, PROCPIC func)
	{ return CompSprPar(index, (PROCCOM)func); }

inline bool CompSprPar(int index, PROCSPR func)
	{ return CompSprPar(index, (PROCCOM)func); }

inline bool CompSprPar(int index, PROCSND func)
	{ return CompSprPar(index, (PROCCOM)func); }

inline bool CompSprPar(int index, PROCMUS func)
	{ return CompSprPar(index, (PROCCOM)func); }


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu se skupinou sprajtù (data = poèet výrazù - 1)

inline bool CompSprGrp(int index, PROCLOG func, PROCLOG func1)
	{ return CompGroup(CompSpr, index, (PROCCOM)func, (PROCCOM)func1); };
