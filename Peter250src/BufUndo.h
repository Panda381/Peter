
/***************************************************************************\
*																			*
*								Buffer undo/redo							*
*																			*
\***************************************************************************/

// položka UNDO/REDO bufferu (32 bajtù)
typedef struct UNDOITEM_
{
	BOOL	Stop;					// (4) pøíznak konce záznamu operace
	long	Oper;					// (4) provádìná operace
	long	BufID;					// (4) ID bufferu, ve kterém se operace provádí
	long	Index;					// (4) index položky, se kterou se operace provádí
	long	DatBufID;				// (4) ID bufferu uschovaných dat (-1 = není)
	long	DatIndex;				// (4) index uschovaných dat (-1 = nejsou data)
	long	Param1;					// (4) parametr 1 (pøi pøesunu pùvodní rodiè)
	long	Param2;					// (4) parametr 2 (pøi pøesunu pùvodní pøedchozí položka)
} UNDOITEM;

#define SIZEOFUNDOITEM	(sizeof(BOOL) + 7*sizeof(long)) // velikost položky

enum UndoOper {
	UNDO_DELETE,					// zrušení položky
	UNDO_INSERT,					// vložení položky
	UNDO_MOVE,						// pøesun položky v bufferu
	UNDO_LOCK,						// pøepnutí pøiznaku LOCK položky
	UNDO_OFF,						// pøepnutí pøíznaku OFF položky
	UNDO_NAME,						// zmìna jména položky
	UNDO_NAMESET,					// nastavení jména položky
	UNDO_ICON,						// zmìna ikony položky
	UNDO_SET,						// zmìna dat
	UNDO_SETFIX,					// zmìna dat zafixovaná proti minimalizaci
	UNDO_SETICON,					// zmìna ikony plochy
	UNDO_SETPIC,					// zmìna obrázku sprajtu
};


/////////////////////////////////////////////////////////////////////////////
// UNDO buffer

class CBufUndo
{

// ------------------------- interní promìnné a funkce ----------------------

private:

// promìnné
	UNDOITEM*	m_UndoData;			// ukazatel na buffer UNDO položek
	int			m_UndoNum;			// poèet UNDO položek v bufferu
	int			m_UndoMax;			// velikost UNDO bufferu (položek)
	int			m_UndoSize;			// celková velikost dat pro UNDO
	int			m_UndoLimit;		// omezení velikosti dat pro UNDO
	int			m_UndoRecs;			// poèet záznamù v UNDO bufferu

	UNDOITEM*	m_RedoData;			// ukazatel na buffer REDO položek
	int			m_RedoNum;			// poèet REDO položek v bufferu
	int			m_RedoMax;			// velikost REDO bufferu (položek)
	int			m_RedoRecs;			// poèet záznamù v REDO bufferu

	CBufProg	m_Prog;				// (BufEdiID) programový buffer
	CBufReal	m_Real;				// (BufNumID) buffer èísel
	CBufMultiText	m_Text;			// (BufTxtID) buffer textù
	CBufBool	m_Bool;				// (BufLogID) buffer logických hodnot
	CBufIcon	m_Icon;				// (BufIcoID) buffer ikon
	CBufMap		m_Map;				// (BufMapID) buffer ploch
	CBufPic		m_Picture;			// (BufPicID) buffer obrázkù
	CBufSprite	m_Sprite;			// (BufSprID) buffer sprajtù
	CBufSound	m_Sound;			// (BufSndID) buffer zvukù
	CBufMusic	m_Music;			// (BufMusID) buffer hudby

	BOOL		m_Play;				// probíhá pøehrávání UNDO/REDO záznamu
	BOOL		m_Undo;				// zaznamenávat do UNDO bufferu (jinak REDO)
	BOOL		m_Redraw[PROGBUFNUM]; // pøíznaky zapnutí pøekreslení

	int			m_EditMode;			// buffer editaèního módu
	int			m_EditIndex;		// index editaèního módu
	int			m_SelectMode;		// buffer k výbìru
	int			m_SelectIndex;		// index k výbìru
	int			m_MapMode;			// mód k nastavení v ploše

// zjištìní pøibližné velikosti datové položky (jen kvùli kontrole pøeteèení)
	int _fastcall GetSize(int bufID, int index);

// pøiètení velikosti položky (jen pokud je záznam do UNDO)
	void _fastcall AddSize(UNDOITEM* item);

// zrušení datové položky z bufferù
	void _fastcall DelData(int bufID, int index);

// vypnutí pøíznakù pøekreslení okna
	void NulRedraw();

// vypnutí/zapnutí pøekreslování okna
	void SetRedraw(int bufID, BOOL redraw);

// vyprázdnìní bufferu REDO (není-li pøehrávání)
	void NulRedo();

// test, zda je položka pro zmìnu dat již v bufferu
	bool TestSet(int bufID, int index);

// oznaèení editované položky
	void SelectEdit(int bufID, int index);


// ---------------------------- veøejné funkce ------------------------------

public:

// konstruktor a destruktor
	CBufUndo();
	~CBufUndo();

// statický konstruktor a destruktor
	void Init();
	void Term();

// zrušení všech položek v bufferu
	void DelAll();

// poskytnutí dat undo
	inline UNDOITEM* DataUndo() const { return m_UndoData; };

// poskytnutí dat redo
	inline UNDOITEM* DataRedo() const { return m_RedoData; };

// poskytnutí poètu položek undo
	inline int NumUndo() const { return m_UndoNum; };

// poskytnutí poètu položek redo
	inline int NumRedo() const { return m_RedoNum; };

// poskytnutí velikosti bufferu undo
	inline int MaxUndo() const { return m_UndoMax; };

// poskytnutí velikosti bufferu redo
	inline int MaxRedo() const { return m_RedoMax; };

// poskytnutí poètu záznamù undo
	inline int RecsUndo() const { return m_UndoRecs; };

// poskytnutí poètu záznamù redo
	inline int RecsRedo() const { return m_RedoRecs; };

// poskytnutí velikosti dat
	inline int Size() const { return m_UndoSize; };

// poskytnutí/nastavení omezení velikosti dat
	inline int Limit() const { return m_UndoLimit; };
	inline void Limit(int limit) { m_UndoLimit = limit; };

// poskytnutí pøíznaku, že probíhá pøehrávání záznamu
	inline BOOL Play() const { return m_Play; };

// kontrola platnosti položky
	inline BOOL IsValidUndo(const int index) const
		{ return ((DWORD)index < (DWORD)m_UndoMax); };

	inline BOOL IsNotValidUndo(const int index) const
		{ return ((DWORD)index >= (DWORD)m_UndoMax); };

	inline BOOL IsValidRedo(const int index) const
		{ return ((DWORD)index < (DWORD)m_RedoMax); };

	inline BOOL IsNotValidRedo(const int index) const
		{ return ((DWORD)index >= (DWORD)m_RedoMax); };

// poskytnutí pøístupu k položce (bez kontroly indexu)
	inline UNDOITEM& AtUndo(const int index)
		{ ASSERT(IsValidUndo(index)); return m_UndoData[index]; }

	inline const UNDOITEM& AtUndo(const int index) const
		{ ASSERT(IsValidUndo(index)); return m_UndoData[index]; }

	inline UNDOITEM& AtRedo(const int index)
		{ ASSERT(IsValidRedo(index)); return m_RedoData[index]; }

	inline const UNDOITEM& AtRedo(const int index) const
		{ ASSERT(IsValidRedo(index)); return m_RedoData[index]; }

// zrušení poslední položky z undo/redo
	void DelUndo();
	void DelRedo();

// vytvoøení nové položky (vrací NULL=chyba pamìti)
	UNDOITEM* New();

// redukce velikosti UNDO bufferu
	void Reduct();

// ukonèení záznamu operací (nastaví pøíznak konce posloupnosti)
	void Stop();

// aktualizace voleb undo a redo
	void UpdateUndoRedo();

// provedení operace UNDO
	void Undo();

// provedení operace REDO
	void Redo();

// záznam o pøidání položky do programového bufferu (vrací FALSE=chyba pamìti)
	bool AddProgIns(int bufID, int index);

// záznam o zrušení položky z programového bufferu (vrací FALSE=chyba pamìti)
	bool AddProgDel(int bufID, int index, PROGITEM* data);

// záznam o pøesunu položky v programovém bufferu (vrací FALSE=chyba pamìti)
	bool AddProgMove(int bufID, int index, int oldparent, int oldprev);

// záznam o zmìnì pøíznaku LOCK prvku (vrací FALSE=chyba pamìti)
	bool AddProgLock(int bufID, int index);

// záznam o zmìnì pøíznaku OFF prvku (vrací FALSE=chyba pamìti)
	bool AddProgOff(int bufID, int index);

// záznam o zmìnì jména prvku (vrací FALSE=chyba pamìti)
	bool AddProgName(int bufID, int index, int name);

// záznam o nastavení jména prvku (vrací FALSE=chyba pamìti)
	bool AddProgNameSet(int bufID, int index, const CMultiText& data);
	bool AddProgNameSet(int bufID, int index, const CText& data);

// záznam o zmìnì ikony prvku (vrací FALSE=chyba pamìti)
	bool AddProgIcon(int bufID, int index, int icon);

// záznam o pøidání položky do bufferu èísel (vrací FALSE=chyba pamìti)
	bool AddRealIns(int index);

// záznam o zrušení položky z bufferu èísel (vrací FALSE=chyba pamìti)
	bool AddRealDel(int index, double data);
	inline bool AddDel(int index, double data) { return AddRealDel(index, data); }

// záznam o zmìnì dat (vrací FALSE=chyba pamìti)
	bool AddRealSet(int index, double data);

// záznam o pøidání položky do bufferu textù (vrací FALSE=chyba pamìti)
	bool AddTextIns(int index);

// záznam o zrušení položky z bufferu textù (vrací FALSE=chyba pamìti)
	bool AddTextDel(int index, const CMultiText& data);
	bool AddTextDel(int index, const CText& data);
	inline bool AddDel(int index, const CMultiText& data) { return AddTextDel(index, data); }
	inline bool AddDel(int index, const CText& data) { return AddTextDel(index, data); }

// záznam o zmìnì dat (vrací FALSE=chyba pamìti)
	bool AddTextSet(int index, const CMultiText& data);
	bool AddTextSet(int index, const CText& data);

// záznam o pøidání položky do bufferu logických hodnot (vrací FALSE=chyba pamìti)
	bool AddBoolIns(int index);

// záznam o zrušení položky z bufferu logických hodnot (vrací FALSE=chyba pamìti)
	bool AddBoolDel(int index, BOOL data);
	inline bool AddDel(int index, const BOOL& data) { return AddBoolDel(index, data); }

// záznam o zmìnì dat (vrací FALSE=chyba pamìti)
	bool AddBoolSet(int index, BOOL data);

// záznam o pøidání položky do bufferu ikon (vrací FALSE=chyba pamìti)
	bool AddIconIns(int index);

// záznam o zrušení položky z bufferu ikon (vrací FALSE=chyba pamìti)
	bool AddIconDel(int index, const CIcon& data);
	inline bool AddDel(int index, const CIcon& data) { return AddIconDel(index, data); }

// záznam o zmìnì dat (vrací FALSE=chyba pamìti)
	bool AddIconSet(int index, const CIcon& data);

// záznam o pøidání položky do bufferu ploch (vrací FALSE=chyba pamìti)
	bool AddMapIns(int index);

// záznam o zrušení položky z bufferu ploch (vrací FALSE=chyba pamìti)
	bool AddMapDel(int index, const CMap& data);
	inline bool AddDel(int index, const CMap& data) { return AddMapDel(index, data); }

// záznam o zmìnì dat (vrací FALSE=chyba pamìti)
	bool AddMapSet(int index, const CMap& data, int mode);

// záznam o zmìnì ikony v ploše (pøi zrušení ikony z plochy) (vrací FALSE=chyba pamìti)
	bool AddMapIcoSet(int index, int offs, int icon);

// záznam o pøidání položky do bufferu obrázkù (vrací FALSE=chyba pamìti)
	bool AddPicIns(int index);

// záznam o zrušení položky z bufferu obrázkù (vrací FALSE=chyba pamìti)
	bool AddPicDel(int index, const CPicture& data);
	inline bool AddDel(int index, const CPicture& data) { return AddPicDel(index, data); }

// záznam o zmìnì dat (vrací FALSE=chyba pamìti)
	bool AddPicSet(int index, const CPicture& data);

// záznam o pøidání položky do bufferu sprajtù (vrací FALSE=chyba pamìti)
	bool AddSprIns(int index);

// záznam o zrušení položky z bufferu sprajtù (vrací FALSE=chyba pamìti)
	bool AddSprDel(int index, const CSprite& data);
	inline bool AddDel(int index, const CSprite& data) { return AddSprDel(index, data); }

// záznam o zmìnì dat (vrací FALSE=chyba pamìti)
	bool AddSprSet(int index, const CSprite& data);

// záznam o zmìnì dat - obrázek sprajtu (vrací FALSE=chyba pamìti)
	bool AddSprPicSet(int index, int pic, CPicture& data);

// záznam o pøidání položky do bufferu zvukù (vrací FALSE=chyba pamìti)
	bool AddSndIns(int index);

// záznam o zrušení položky z bufferu zvukù (vrací FALSE=chyba pamìti)
	bool AddSndDel(int index, const CSound& data);
	inline bool AddDel(int index, const CSound& data) { return AddSndDel(index, data); }

// záznam o zmìnì dat (vrací FALSE=chyba pamìti)
	bool AddSndSet(int index, const CSound& data);

// záznam o pøidání položky do bufferu hudby (vrací FALSE=chyba pamìti)
	bool AddMusIns(int index);

// záznam o zrušení položky z bufferu hudby (vrací FALSE=chyba pamìti)
	bool AddMusDel(int index, const CMusic& data);
	inline bool AddDel(int index, const CMusic& data) { return AddMusDel(index, data); }

// záznam o zmìnì dat (vrací FALSE=chyba pamìti)
	bool AddMusSet(int index, const CMusic& data);

// zafixování posledního záznamu o zmìnì dat prodi minimalizaci
	void Fixup();

// pøehrátí jedné položky
	void Play(UNDOITEM* item);
};
