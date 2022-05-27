
/***************************************************************************\
*																			*
*								Buffer undo/redo							*
*																			*
\***************************************************************************/

// polo�ka UNDO/REDO bufferu (32 bajt�)
typedef struct UNDOITEM_
{
	BOOL	Stop;					// (4) p��znak konce z�znamu operace
	long	Oper;					// (4) prov�d�n� operace
	long	BufID;					// (4) ID bufferu, ve kter�m se operace prov�d�
	long	Index;					// (4) index polo�ky, se kterou se operace prov�d�
	long	DatBufID;				// (4) ID bufferu uschovan�ch dat (-1 = nen�)
	long	DatIndex;				// (4) index uschovan�ch dat (-1 = nejsou data)
	long	Param1;					// (4) parametr 1 (p�i p�esunu p�vodn� rodi�)
	long	Param2;					// (4) parametr 2 (p�i p�esunu p�vodn� p�edchoz� polo�ka)
} UNDOITEM;

#define SIZEOFUNDOITEM	(sizeof(BOOL) + 7*sizeof(long)) // velikost polo�ky

enum UndoOper {
	UNDO_DELETE,					// zru�en� polo�ky
	UNDO_INSERT,					// vlo�en� polo�ky
	UNDO_MOVE,						// p�esun polo�ky v bufferu
	UNDO_LOCK,						// p�epnut� p�iznaku LOCK polo�ky
	UNDO_OFF,						// p�epnut� p��znaku OFF polo�ky
	UNDO_NAME,						// zm�na jm�na polo�ky
	UNDO_NAMESET,					// nastaven� jm�na polo�ky
	UNDO_ICON,						// zm�na ikony polo�ky
	UNDO_SET,						// zm�na dat
	UNDO_SETFIX,					// zm�na dat zafixovan� proti minimalizaci
	UNDO_SETICON,					// zm�na ikony plochy
	UNDO_SETPIC,					// zm�na obr�zku sprajtu
};


/////////////////////////////////////////////////////////////////////////////
// UNDO buffer

class CBufUndo
{

// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn�
	UNDOITEM*	m_UndoData;			// ukazatel na buffer UNDO polo�ek
	int			m_UndoNum;			// po�et UNDO polo�ek v bufferu
	int			m_UndoMax;			// velikost UNDO bufferu (polo�ek)
	int			m_UndoSize;			// celkov� velikost dat pro UNDO
	int			m_UndoLimit;		// omezen� velikosti dat pro UNDO
	int			m_UndoRecs;			// po�et z�znam� v UNDO bufferu

	UNDOITEM*	m_RedoData;			// ukazatel na buffer REDO polo�ek
	int			m_RedoNum;			// po�et REDO polo�ek v bufferu
	int			m_RedoMax;			// velikost REDO bufferu (polo�ek)
	int			m_RedoRecs;			// po�et z�znam� v REDO bufferu

	CBufProg	m_Prog;				// (BufEdiID) programov� buffer
	CBufReal	m_Real;				// (BufNumID) buffer ��sel
	CBufMultiText	m_Text;			// (BufTxtID) buffer text�
	CBufBool	m_Bool;				// (BufLogID) buffer logick�ch hodnot
	CBufIcon	m_Icon;				// (BufIcoID) buffer ikon
	CBufMap		m_Map;				// (BufMapID) buffer ploch
	CBufPic		m_Picture;			// (BufPicID) buffer obr�zk�
	CBufSprite	m_Sprite;			// (BufSprID) buffer sprajt�
	CBufSound	m_Sound;			// (BufSndID) buffer zvuk�
	CBufMusic	m_Music;			// (BufMusID) buffer hudby

	BOOL		m_Play;				// prob�h� p�ehr�v�n� UNDO/REDO z�znamu
	BOOL		m_Undo;				// zaznamen�vat do UNDO bufferu (jinak REDO)
	BOOL		m_Redraw[PROGBUFNUM]; // p��znaky zapnut� p�ekreslen�

	int			m_EditMode;			// buffer edita�n�ho m�du
	int			m_EditIndex;		// index edita�n�ho m�du
	int			m_SelectMode;		// buffer k v�b�ru
	int			m_SelectIndex;		// index k v�b�ru
	int			m_MapMode;			// m�d k nastaven� v plo�e

// zji�t�n� p�ibli�n� velikosti datov� polo�ky (jen kv�li kontrole p�ete�en�)
	int _fastcall GetSize(int bufID, int index);

// p�i�ten� velikosti polo�ky (jen pokud je z�znam do UNDO)
	void _fastcall AddSize(UNDOITEM* item);

// zru�en� datov� polo�ky z buffer�
	void _fastcall DelData(int bufID, int index);

// vypnut� p��znak� p�ekreslen� okna
	void NulRedraw();

// vypnut�/zapnut� p�ekreslov�n� okna
	void SetRedraw(int bufID, BOOL redraw);

// vypr�zdn�n� bufferu REDO (nen�-li p�ehr�v�n�)
	void NulRedo();

// test, zda je polo�ka pro zm�nu dat ji� v bufferu
	bool TestSet(int bufID, int index);

// ozna�en� editovan� polo�ky
	void SelectEdit(int bufID, int index);


// ---------------------------- ve�ejn� funkce ------------------------------

public:

// konstruktor a destruktor
	CBufUndo();
	~CBufUndo();

// statick� konstruktor a destruktor
	void Init();
	void Term();

// zru�en� v�ech polo�ek v bufferu
	void DelAll();

// poskytnut� dat undo
	inline UNDOITEM* DataUndo() const { return m_UndoData; };

// poskytnut� dat redo
	inline UNDOITEM* DataRedo() const { return m_RedoData; };

// poskytnut� po�tu polo�ek undo
	inline int NumUndo() const { return m_UndoNum; };

// poskytnut� po�tu polo�ek redo
	inline int NumRedo() const { return m_RedoNum; };

// poskytnut� velikosti bufferu undo
	inline int MaxUndo() const { return m_UndoMax; };

// poskytnut� velikosti bufferu redo
	inline int MaxRedo() const { return m_RedoMax; };

// poskytnut� po�tu z�znam� undo
	inline int RecsUndo() const { return m_UndoRecs; };

// poskytnut� po�tu z�znam� redo
	inline int RecsRedo() const { return m_RedoRecs; };

// poskytnut� velikosti dat
	inline int Size() const { return m_UndoSize; };

// poskytnut�/nastaven� omezen� velikosti dat
	inline int Limit() const { return m_UndoLimit; };
	inline void Limit(int limit) { m_UndoLimit = limit; };

// poskytnut� p��znaku, �e prob�h� p�ehr�v�n� z�znamu
	inline BOOL Play() const { return m_Play; };

// kontrola platnosti polo�ky
	inline BOOL IsValidUndo(const int index) const
		{ return ((DWORD)index < (DWORD)m_UndoMax); };

	inline BOOL IsNotValidUndo(const int index) const
		{ return ((DWORD)index >= (DWORD)m_UndoMax); };

	inline BOOL IsValidRedo(const int index) const
		{ return ((DWORD)index < (DWORD)m_RedoMax); };

	inline BOOL IsNotValidRedo(const int index) const
		{ return ((DWORD)index >= (DWORD)m_RedoMax); };

// poskytnut� p��stupu k polo�ce (bez kontroly indexu)
	inline UNDOITEM& AtUndo(const int index)
		{ ASSERT(IsValidUndo(index)); return m_UndoData[index]; }

	inline const UNDOITEM& AtUndo(const int index) const
		{ ASSERT(IsValidUndo(index)); return m_UndoData[index]; }

	inline UNDOITEM& AtRedo(const int index)
		{ ASSERT(IsValidRedo(index)); return m_RedoData[index]; }

	inline const UNDOITEM& AtRedo(const int index) const
		{ ASSERT(IsValidRedo(index)); return m_RedoData[index]; }

// zru�en� posledn� polo�ky z undo/redo
	void DelUndo();
	void DelRedo();

// vytvo�en� nov� polo�ky (vrac� NULL=chyba pam�ti)
	UNDOITEM* New();

// redukce velikosti UNDO bufferu
	void Reduct();

// ukon�en� z�znamu operac� (nastav� p��znak konce posloupnosti)
	void Stop();

// aktualizace voleb undo a redo
	void UpdateUndoRedo();

// proveden� operace UNDO
	void Undo();

// proveden� operace REDO
	void Redo();

// z�znam o p�id�n� polo�ky do programov�ho bufferu (vrac� FALSE=chyba pam�ti)
	bool AddProgIns(int bufID, int index);

// z�znam o zru�en� polo�ky z programov�ho bufferu (vrac� FALSE=chyba pam�ti)
	bool AddProgDel(int bufID, int index, PROGITEM* data);

// z�znam o p�esunu polo�ky v programov�m bufferu (vrac� FALSE=chyba pam�ti)
	bool AddProgMove(int bufID, int index, int oldparent, int oldprev);

// z�znam o zm�n� p��znaku LOCK prvku (vrac� FALSE=chyba pam�ti)
	bool AddProgLock(int bufID, int index);

// z�znam o zm�n� p��znaku OFF prvku (vrac� FALSE=chyba pam�ti)
	bool AddProgOff(int bufID, int index);

// z�znam o zm�n� jm�na prvku (vrac� FALSE=chyba pam�ti)
	bool AddProgName(int bufID, int index, int name);

// z�znam o nastaven� jm�na prvku (vrac� FALSE=chyba pam�ti)
	bool AddProgNameSet(int bufID, int index, const CMultiText& data);
	bool AddProgNameSet(int bufID, int index, const CText& data);

// z�znam o zm�n� ikony prvku (vrac� FALSE=chyba pam�ti)
	bool AddProgIcon(int bufID, int index, int icon);

// z�znam o p�id�n� polo�ky do bufferu ��sel (vrac� FALSE=chyba pam�ti)
	bool AddRealIns(int index);

// z�znam o zru�en� polo�ky z bufferu ��sel (vrac� FALSE=chyba pam�ti)
	bool AddRealDel(int index, double data);
	inline bool AddDel(int index, double data) { return AddRealDel(index, data); }

// z�znam o zm�n� dat (vrac� FALSE=chyba pam�ti)
	bool AddRealSet(int index, double data);

// z�znam o p�id�n� polo�ky do bufferu text� (vrac� FALSE=chyba pam�ti)
	bool AddTextIns(int index);

// z�znam o zru�en� polo�ky z bufferu text� (vrac� FALSE=chyba pam�ti)
	bool AddTextDel(int index, const CMultiText& data);
	bool AddTextDel(int index, const CText& data);
	inline bool AddDel(int index, const CMultiText& data) { return AddTextDel(index, data); }
	inline bool AddDel(int index, const CText& data) { return AddTextDel(index, data); }

// z�znam o zm�n� dat (vrac� FALSE=chyba pam�ti)
	bool AddTextSet(int index, const CMultiText& data);
	bool AddTextSet(int index, const CText& data);

// z�znam o p�id�n� polo�ky do bufferu logick�ch hodnot (vrac� FALSE=chyba pam�ti)
	bool AddBoolIns(int index);

// z�znam o zru�en� polo�ky z bufferu logick�ch hodnot (vrac� FALSE=chyba pam�ti)
	bool AddBoolDel(int index, BOOL data);
	inline bool AddDel(int index, const BOOL& data) { return AddBoolDel(index, data); }

// z�znam o zm�n� dat (vrac� FALSE=chyba pam�ti)
	bool AddBoolSet(int index, BOOL data);

// z�znam o p�id�n� polo�ky do bufferu ikon (vrac� FALSE=chyba pam�ti)
	bool AddIconIns(int index);

// z�znam o zru�en� polo�ky z bufferu ikon (vrac� FALSE=chyba pam�ti)
	bool AddIconDel(int index, const CIcon& data);
	inline bool AddDel(int index, const CIcon& data) { return AddIconDel(index, data); }

// z�znam o zm�n� dat (vrac� FALSE=chyba pam�ti)
	bool AddIconSet(int index, const CIcon& data);

// z�znam o p�id�n� polo�ky do bufferu ploch (vrac� FALSE=chyba pam�ti)
	bool AddMapIns(int index);

// z�znam o zru�en� polo�ky z bufferu ploch (vrac� FALSE=chyba pam�ti)
	bool AddMapDel(int index, const CMap& data);
	inline bool AddDel(int index, const CMap& data) { return AddMapDel(index, data); }

// z�znam o zm�n� dat (vrac� FALSE=chyba pam�ti)
	bool AddMapSet(int index, const CMap& data, int mode);

// z�znam o zm�n� ikony v plo�e (p�i zru�en� ikony z plochy) (vrac� FALSE=chyba pam�ti)
	bool AddMapIcoSet(int index, int offs, int icon);

// z�znam o p�id�n� polo�ky do bufferu obr�zk� (vrac� FALSE=chyba pam�ti)
	bool AddPicIns(int index);

// z�znam o zru�en� polo�ky z bufferu obr�zk� (vrac� FALSE=chyba pam�ti)
	bool AddPicDel(int index, const CPicture& data);
	inline bool AddDel(int index, const CPicture& data) { return AddPicDel(index, data); }

// z�znam o zm�n� dat (vrac� FALSE=chyba pam�ti)
	bool AddPicSet(int index, const CPicture& data);

// z�znam o p�id�n� polo�ky do bufferu sprajt� (vrac� FALSE=chyba pam�ti)
	bool AddSprIns(int index);

// z�znam o zru�en� polo�ky z bufferu sprajt� (vrac� FALSE=chyba pam�ti)
	bool AddSprDel(int index, const CSprite& data);
	inline bool AddDel(int index, const CSprite& data) { return AddSprDel(index, data); }

// z�znam o zm�n� dat (vrac� FALSE=chyba pam�ti)
	bool AddSprSet(int index, const CSprite& data);

// z�znam o zm�n� dat - obr�zek sprajtu (vrac� FALSE=chyba pam�ti)
	bool AddSprPicSet(int index, int pic, CPicture& data);

// z�znam o p�id�n� polo�ky do bufferu zvuk� (vrac� FALSE=chyba pam�ti)
	bool AddSndIns(int index);

// z�znam o zru�en� polo�ky z bufferu zvuk� (vrac� FALSE=chyba pam�ti)
	bool AddSndDel(int index, const CSound& data);
	inline bool AddDel(int index, const CSound& data) { return AddSndDel(index, data); }

// z�znam o zm�n� dat (vrac� FALSE=chyba pam�ti)
	bool AddSndSet(int index, const CSound& data);

// z�znam o p�id�n� polo�ky do bufferu hudby (vrac� FALSE=chyba pam�ti)
	bool AddMusIns(int index);

// z�znam o zru�en� polo�ky z bufferu hudby (vrac� FALSE=chyba pam�ti)
	bool AddMusDel(int index, const CMusic& data);
	inline bool AddDel(int index, const CMusic& data) { return AddMusDel(index, data); }

// z�znam o zm�n� dat (vrac� FALSE=chyba pam�ti)
	bool AddMusSet(int index, const CMusic& data);

// zafixov�n� posledn�ho z�znamu o zm�n� dat prodi minimalizaci
	void Fixup();

// p�ehr�t� jedn� polo�ky
	void Play(UNDOITEM* item);
};
