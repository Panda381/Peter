
/***************************************************************************\
*																			*
*						Kompilace programu - text							*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// p�id�n� jednoho prvku do bufferu programu (vrac� index prvku)

inline int CompAddItem(PROCTXT func)
	{ return CompAddItem((PROCCOM)func); }

inline int CompAddItem(PROCTXT func, int data)
	{ return CompAddItem((PROCCOM)func, data); }

inline int CompAddItem(PROCTXT func, int data, int list)
	{ return CompAddItem((PROCCOM)func, data, list); }


/////////////////////////////////////////////////////////////////////////////
// p�eklad textov�ho v�razu (vrac� true = operace OK)

bool _fastcall CompTxt(int index);


/////////////////////////////////////////////////////////////////////////////
// p�eklad p��kazu s textov�m parametrem

bool CompTxtPar(int index, PROCCOM func, int data);

inline bool CompTxtPar(int index, PROCNUM func, int data)
	{ return CompTxtPar(index, (PROCCOM)func, data); }

inline bool CompTxtPar(int index, PROCTXT func, int data)
	{ return CompTxtPar(index, (PROCCOM)func, data); }

inline bool CompTxtPar(int index, PROCLOG func, int data)
	{ return CompTxtPar(index, (PROCCOM)func, data); }

inline bool CompTxtPar(int index, PROCICO func, int data)
	{ return CompTxtPar(index, (PROCCOM)func, data); }

inline bool CompTxtPar(int index, PROCMAP func, int data)
	{ return CompTxtPar(index, (PROCCOM)func, data); }

inline bool CompTxtPar(int index, PROCPIC func, int data)
	{ return CompTxtPar(index, (PROCCOM)func, data); }

inline bool CompTxtPar(int index, PROCSPR func, int data)
	{ return CompTxtPar(index, (PROCCOM)func, data); }

inline bool CompTxtPar(int index, PROCSND func, int data)
	{ return CompTxtPar(index, (PROCCOM)func, data); }

inline bool CompTxtPar(int index, PROCMUS func, int data)
	{ return CompTxtPar(index, (PROCCOM)func, data); }


bool CompTxtPar(int index, PROCCOM func, int data, int list);

inline bool CompTxtPar(int index, PROCNUM func, int data, int list)
	{ return CompTxtPar(index, (PROCCOM)func, data, list); }

inline bool CompTxtPar(int index, PROCTXT func, int data, int list)
	{ return CompTxtPar(index, (PROCCOM)func, data, list); }

inline bool CompTxtPar(int index, PROCLOG func, int data, int list)
	{ return CompTxtPar(index, (PROCCOM)func, data, list); }

inline bool CompTxtPar(int index, PROCICO func, int data, int list)
	{ return CompTxtPar(index, (PROCCOM)func, data, list); }

inline bool CompTxtPar(int index, PROCMAP func, int data, int list)
	{ return CompTxtPar(index, (PROCCOM)func, data, list); }

inline bool CompTxtPar(int index, PROCPIC func, int data, int list)
	{ return CompTxtPar(index, (PROCCOM)func, data, list); }

inline bool CompTxtPar(int index, PROCSPR func, int data, int list)
	{ return CompTxtPar(index, (PROCCOM)func, data, list); }

inline bool CompTxtPar(int index, PROCSND func, int data, int list)
	{ return CompTxtPar(index, (PROCCOM)func, data, list); }

inline bool CompTxtPar(int index, PROCMUS func, int data, int list)
	{ return CompTxtPar(index, (PROCCOM)func, data, list); }


bool CompTxtPar(int index, PROCCOM func);

inline bool CompTxtPar(int index, PROCNUM func)
	{ return CompTxtPar(index, (PROCCOM)func); }

inline bool CompTxtPar(int index, PROCTXT func)
	{ return CompTxtPar(index, (PROCCOM)func); }

inline bool CompTxtPar(int index, PROCLOG func)
	{ return CompTxtPar(index, (PROCCOM)func); }

inline bool CompTxtPar(int index, PROCICO func)
	{ return CompTxtPar(index, (PROCCOM)func); }

inline bool CompTxtPar(int index, PROCMAP func)
	{ return CompTxtPar(index, (PROCCOM)func); }

inline bool CompTxtPar(int index, PROCPIC func)
	{ return CompTxtPar(index, (PROCCOM)func); }

inline bool CompTxtPar(int index, PROCSPR func)
	{ return CompTxtPar(index, (PROCCOM)func); }

inline bool CompTxtPar(int index, PROCSND func)
	{ return CompTxtPar(index, (PROCCOM)func); }

inline bool CompTxtPar(int index, PROCMUS func)
	{ return CompTxtPar(index, (PROCCOM)func); }


/////////////////////////////////////////////////////////////////////////////
// p�eklad p��kazu se skupinou textov�ch parametr� (data = po�et parametr� - 1)

inline bool CompTxtGrp(int index, PROCLOG func, PROCLOG func1)
	{ return CompGroup(CompTxt, index, (PROCCOM)func, (PROCCOM)func1); };

inline bool CompTxtGrp(int index, PROCTXT func, PROCTXT func1)
	{ return CompGroup(CompTxt, index, (PROCCOM)func, (PROCCOM)func1); };


/////////////////////////////////////////////////////////////////////////////
// p�eklad p��kazu s textov�m podparametrem (hledan�m podle identifikace)

void CompTxtSubPar(int index, int idf);
