
/***************************************************************************\
*																			*
*						Kompilace programu - hudba							*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// pøidání jednoho prvku do bufferu programu (vrací index prvku)

inline int CompAddItem(PROCMUS func)
	{ return CompAddItem((PROCCOM)func); }

inline int CompAddItem(PROCMUS func, int data)
	{ return CompAddItem((PROCCOM)func, data); }

inline int CompAddItem(PROCMUS func, int data, int list)
	{ return CompAddItem((PROCCOM)func, data, list); }


/////////////////////////////////////////////////////////////////////////////
// pøeklad výrazu s hudbou (vrací true = operace OK)

bool _fastcall CompMus(int index);


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu s parametrem hudby

bool CompMusPar(int index, PROCCOM func, int data);

inline bool CompMusPar(int index, PROCNUM func, int data)
	{ return CompMusPar(index, (PROCCOM)func, data); }

inline bool CompMusPar(int index, PROCTXT func, int data)
	{ return CompMusPar(index, (PROCCOM)func, data); }

inline bool CompMusPar(int index, PROCLOG func, int data)
	{ return CompMusPar(index, (PROCCOM)func, data); }

inline bool CompMusPar(int index, PROCICO func, int data)
	{ return CompMusPar(index, (PROCCOM)func, data); }

inline bool CompMusPar(int index, PROCMAP func, int data)
	{ return CompMusPar(index, (PROCCOM)func, data); }

inline bool CompMusPar(int index, PROCPIC func, int data)
	{ return CompMusPar(index, (PROCCOM)func, data); }

inline bool CompMusPar(int index, PROCSPR func, int data)
	{ return CompMusPar(index, (PROCCOM)func, data); }

inline bool CompMusPar(int index, PROCSND func, int data)
	{ return CompMusPar(index, (PROCCOM)func, data); }

inline bool CompMusPar(int index, PROCMUS func, int data)
	{ return CompMusPar(index, (PROCCOM)func, data); }


bool CompMusPar(int index, PROCCOM func, int data, int list);

inline bool CompMusPar(int index, PROCNUM func, int data, int list)
	{ return CompMusPar(index, (PROCCOM)func, data, list); }

inline bool CompMusPar(int index, PROCTXT func, int data, int list)
	{ return CompMusPar(index, (PROCCOM)func, data, list); }

inline bool CompMusPar(int index, PROCLOG func, int data, int list)
	{ return CompMusPar(index, (PROCCOM)func, data, list); }

inline bool CompMusPar(int index, PROCICO func, int data, int list)
	{ return CompMusPar(index, (PROCCOM)func, data, list); }

inline bool CompMusPar(int index, PROCMAP func, int data, int list)
	{ return CompMusPar(index, (PROCCOM)func, data, list); }

inline bool CompMusPar(int index, PROCPIC func, int data, int list)
	{ return CompMusPar(index, (PROCCOM)func, data, list); }

inline bool CompMusPar(int index, PROCSPR func, int data, int list)
	{ return CompMusPar(index, (PROCCOM)func, data, list); }

inline bool CompMusPar(int index, PROCSND func, int data, int list)
	{ return CompMusPar(index, (PROCCOM)func, data, list); }

inline bool CompMusPar(int index, PROCMUS func, int data, int list)
	{ return CompMusPar(index, (PROCCOM)func, data, list); }


bool CompMusPar(int index, PROCCOM func);

inline bool CompMusPar(int index, PROCNUM func)
	{ return CompMusPar(index, (PROCCOM)func); }

inline bool CompMusPar(int index, PROCTXT func)
	{ return CompMusPar(index, (PROCCOM)func); }

inline bool CompMusPar(int index, PROCLOG func)
	{ return CompMusPar(index, (PROCCOM)func); }

inline bool CompMusPar(int index, PROCICO func)
	{ return CompMusPar(index, (PROCCOM)func); }

inline bool CompMusPar(int index, PROCMAP func)
	{ return CompMusPar(index, (PROCCOM)func); }

inline bool CompMusPar(int index, PROCPIC func)
	{ return CompMusPar(index, (PROCCOM)func); }

inline bool CompMusPar(int index, PROCSPR func)
	{ return CompMusPar(index, (PROCCOM)func); }

inline bool CompMusPar(int index, PROCSND func)
	{ return CompMusPar(index, (PROCCOM)func); }

inline bool CompMusPar(int index, PROCMUS func)
	{ return CompMusPar(index, (PROCCOM)func); }

/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu se skupinou hudby (data = poèet výrazù - 1)

inline bool CompMusGrp(int index, PROCLOG func, PROCLOG func1)
	{ return CompGroup(CompMus, index, (PROCCOM)func, (PROCCOM)func1); };
