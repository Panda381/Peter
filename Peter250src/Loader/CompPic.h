
/***************************************************************************\
*																			*
*						Kompilace programu - obrázek						*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// pøidání jednoho prvku do bufferu programu (vrací index prvku)

inline int CompAddItem(PROCPIC func)
	{ return CompAddItem((PROCCOM)func); }

inline int CompAddItem(PROCPIC func, int data)
	{ return CompAddItem((PROCCOM)func, data); }

inline int CompAddItem(PROCPIC func, int data, int list)
	{ return CompAddItem((PROCCOM)func, data, list); }


/////////////////////////////////////////////////////////////////////////////
// pøeklad výrazu s obrázkem (vrací true = operace OK)

bool _fastcall CompPic(int index);


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu s parametrem obrázku

bool CompPicPar(int index, PROCCOM func, int data);

inline bool CompPicPar(int index, PROCNUM func, int data)
	{ return CompPicPar(index, (PROCCOM)func, data); }

inline bool CompPicPar(int index, PROCTXT func, int data)
	{ return CompPicPar(index, (PROCCOM)func, data); }

inline bool CompPicPar(int index, PROCLOG func, int data)
	{ return CompPicPar(index, (PROCCOM)func, data); }

inline bool CompPicPar(int index, PROCICO func, int data)
	{ return CompPicPar(index, (PROCCOM)func, data); }

inline bool CompPicPar(int index, PROCMAP func, int data)
	{ return CompPicPar(index, (PROCCOM)func, data); }

inline bool CompPicPar(int index, PROCPIC func, int data)
	{ return CompPicPar(index, (PROCCOM)func, data); }

inline bool CompPicPar(int index, PROCSPR func, int data)
	{ return CompPicPar(index, (PROCCOM)func, data); }

inline bool CompPicPar(int index, PROCSND func, int data)
	{ return CompPicPar(index, (PROCCOM)func, data); }

inline bool CompPicPar(int index, PROCMUS func, int data)
	{ return CompPicPar(index, (PROCCOM)func, data); }


bool CompPicPar(int index, PROCCOM func, int data, int list);

inline bool CompPicPar(int index, PROCNUM func, int data, int list)
	{ return CompPicPar(index, (PROCCOM)func, data, list); }

inline bool CompPicPar(int index, PROCTXT func, int data, int list)
	{ return CompPicPar(index, (PROCCOM)func, data, list); }

inline bool CompPicPar(int index, PROCLOG func, int data, int list)
	{ return CompPicPar(index, (PROCCOM)func, data, list); }

inline bool CompPicPar(int index, PROCICO func, int data, int list)
	{ return CompPicPar(index, (PROCCOM)func, data, list); }

inline bool CompPicPar(int index, PROCMAP func, int data, int list)
	{ return CompPicPar(index, (PROCCOM)func, data, list); }

inline bool CompPicPar(int index, PROCPIC func, int data, int list)
	{ return CompPicPar(index, (PROCCOM)func, data, list); }

inline bool CompPicPar(int index, PROCSPR func, int data, int list)
	{ return CompPicPar(index, (PROCCOM)func, data, list); }

inline bool CompPicPar(int index, PROCSND func, int data, int list)
	{ return CompPicPar(index, (PROCCOM)func, data, list); }

inline bool CompPicPar(int index, PROCMUS func, int data, int list)
	{ return CompPicPar(index, (PROCCOM)func, data, list); }


bool CompPicPar(int index, PROCCOM func);

inline bool CompPicPar(int index, PROCNUM func)
	{ return CompPicPar(index, (PROCCOM)func); }

inline bool CompPicPar(int index, PROCTXT func)
	{ return CompPicPar(index, (PROCCOM)func); }

inline bool CompPicPar(int index, PROCLOG func)
	{ return CompPicPar(index, (PROCCOM)func); }

inline bool CompPicPar(int index, PROCICO func)
	{ return CompPicPar(index, (PROCCOM)func); }

inline bool CompPicPar(int index, PROCMAP func)
	{ return CompPicPar(index, (PROCCOM)func); }

inline bool CompPicPar(int index, PROCPIC func)
	{ return CompPicPar(index, (PROCCOM)func); }

inline bool CompPicPar(int index, PROCSPR func)
	{ return CompPicPar(index, (PROCCOM)func); }

inline bool CompPicPar(int index, PROCSND func)
	{ return CompPicPar(index, (PROCCOM)func); }

inline bool CompPicPar(int index, PROCMUS func)
	{ return CompPicPar(index, (PROCCOM)func); }


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu se skupinou obrázkù (data = poèet výrazù - 1)

inline bool CompPicGrp(int index, PROCLOG func, PROCLOG func1)
	{ return CompGroup(CompPic, index, (PROCCOM)func, (PROCCOM)func1); };


/////////////////////////////////////////////////////////////////////////////
// pøeklad pøíkazu s podparametrem obrázku (hledaným podle identifikace)

void CompPicSubPar(int index, int idf);
