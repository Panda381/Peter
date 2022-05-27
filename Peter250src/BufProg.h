
/***************************************************************************\
*																			*
*							Buffer programu									*
*																			*
\***************************************************************************/

#define		PROGBUFNUM	6				// po�et programov�ch buffer�

// identifikace buffer� (nem�nit po�ad� - muselo by se zm�nit p�i ukl�d�n� souboru!)
#define		BufIntID	0				// intern� buffer seznamu funkc� (nem� strom)
#define		BufClsID	1				// buffer t��d objekt�
#define		BufObjID	2				// buffer glob�ln�ch objekt�
#define		BufLocID	3				// buffer lok�ln�ch objekt�
#define		BufEdiID	4				// buffer editace
#define		BufStrID	5				// buffer struktur

// datov� buffery (mus� odpov�dat index�m v SaveHead !!!!)
#define		BufNumID	PROGBUFNUM		// (6) buffer ��sel
#define		BufTxtID	PROGBUFNUM+1	// (7) buffer text�
#define		BufLogID	PROGBUFNUM+2	// (8) buffer logick�ch prom�nn�ch
#define		BufIcoID	PROGBUFNUM+3	// (9) buffer ikon
#define		BufMapID	PROGBUFNUM+4	// (10) buffer ploch
#define		BufPicID	PROGBUFNUM+5	// (11) buffer obr�zk�
#define		BufSprID	PROGBUFNUM+6	// (12) buffer sprajt�
#define		BufSndID	PROGBUFNUM+7	// (13) buffer zvuk�
#define		BufMusID	PROGBUFNUM+8	// (14) buffer hudby
#define		BufPalID	PROGBUFNUM+9	// (15) buffer palet

// P�i zm�n� zm�nit definice i v Loader !!!!!

#define	PR_LITE		0x0000		// prvek vypnut pro LITE verzi

/////////////////////////////////////////////////////////////////////////////
// definice stav� ikon

enum STATE {
	STATE_NONE = 0,
	STATE_DEF,
	STATE_SET,						// nastaven�
	STATE_SET_OPT,					// nepovinn� nastaven�
};


/////////////////////////////////////////////////////////////////////////////
// struktura polo�ky programu - 64 bajt�

typedef struct PROGITEM_
{
	int			Func;				// identifika�n� ��slo funkce (= IDF_...)

	int			SrcMask;			// maska zdrojov�ch vlastnost�
	int			DstMask;			// maska c�lov�ch vlastnost�
	int			Param;				// parametry polo�ky

	int			RefBlok;			// referen�n� blok (-1 = nen�)
	int			RefIndex;			// referen�n� index
	int			DatBlok;			// datov� blok (-1 = nen�)
	int			DatIndex;			// datov� index

	int			Parent;				// index rodi�e (-1 = nen�)
	int			Next;				// index n�sleduj�c� polo�ky (-1 = nen�)
	int			Prev;				// index p�edch�zej�c� polo�ky (-1 = nen�)
	int			Child;				// index prvniho potomka (-1 = nen�)

	int			Icon;				// index ikony (-1 = implicitn�)
	int			Name;				// index jm�na (-1 = �mplicitn�)

	HTREEITEM	HTree;				// handle polo�ky stromu (NULL=nen�)

	int			Refer;				// ��ta� referenc� na polo�ku
} PROGITEM;

#define	SIZEOFPROGITEM	(15*sizeof(int) + sizeof(HTREEITEM))	// velikost polo�ky

// zdrojov� a c�lov� vlastnosti:
#define	PR_COMMAND	0x0001		// je p��kazem/p�ij�m� p��kazy
#define	PR_EXPRES	0x0002		// je ��seln�m v�razem/p�ij�m� ��seln� v�raz
#define	PR_TEXT		0x0004		// je textem/p�ij�m� text
#define	PR_BOOL		0x0008		// je logick�m v�razem/p�ij�m� logick� v�raz
#define	PR_ICON		0x0010		// je ikonou/p�ij�m� ikonu
#define	PR_MAP		0x0020		// je plochou/p�ij�m� plochu
#define	PR_PIC		0x0040		// je obr�zkem/p�ij�m� obr�zky
#define	PR_SPRITE	0x0080		// je sprajtem/p�ij�m� sprajty
#define	PR_SOUND	0x0100		// je zvukem/p�ij�m� zvuky
#define	PR_MUSIC	0x0200		// je hudbou/p�ij�m� hudbu

#define	PR_DIGIT	0x0400		// je ��slic�/p�ij�m� ��slice

#define PR_ALLDATA	(	PR_EXPRES |	\
						PR_TEXT |	\
						PR_BOOL |	\
						PR_ICON |	\
						PR_MAP |	\
						PR_PIC |	\
						PR_SPRITE |	\
						PR_SOUND |	\
						PR_MUSIC)	// v�echny mo�nosti dat (krom� p��kazu)

#define	PR_ALL		(PR_ALLDATA | PR_COMMAND) // v�echny mo�nosti vlastnost� (funkce, skupina)

// parametry a vlastnosti
#define	PR_EXP		0x0001		// potomci prvku jsou rozvinuti
#define	PR_LOCK		0x0002		// prvek uzamknut
#define	PR_LOCK_DEP	0x0004		// prvek z�visle uzamknut
#define	PR_OFF		0x0008		// prvek vypnut
#define	PR_OFF_DEP	0x0010		// prvek z�visle vypnut
#define	PR_ONE		0x0020		// prvek m��e m�t jen 1 potomka
#define	PR_INTERN	0x0040		// z�kaz ru�en� a p�esunu
#define	PR_NOMOVE	0x0080		// z�kaz ru�en�, p�esunu a kopie
#define PR_NEW		0x0100		// nov� polo�ka (bude p�eps�na z cliboard::paste)
#define PR_SETPAR	0x0200		// je-li mo�n� parametr, je nutn� po�adov�n
#define PR_PARPAR	0x0400		// m� parametr p�i pou�it� jako p��kaz i jako parametr
#define PR_CD		0x0800		// je alternativn� knihovna
#define PR_ALTEXT	0x1000		// je alternativn� p��pona souboru (JPG, MP3, RMI)
#define PR_CD2		0x2000		// je alternativn� knihovna 2

#define PR_NOREFER	0x4000		// ned�lat reference na prvek

#define NOREFER		0x4000		// p��znak v tabulce struktur

/////////////////////////////////////////////////////////////////////////////
// polo�ka inicializa�n� tabulky strom�

typedef struct INITTREE_ {
	short	Level;			// hladina (0 = ROOT, NOREFER=ned�lat reference)
	short	Func;			// identifikace funkce (IDF_...)
} INITTREE;


/////////////////////////////////////////////////////////////////////////////
// buffer programu

class CBufProg
{

// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn�
	PROGITEM*	m_Data;			// buffer dat
	bool*		m_Valid;		// p��znaky platnosti polo�ek
	int			m_Num;			// po�et platn�ch polo�ek v bufferu
	int			m_Max;			// velikost bufferu (polo�ek)
	int			m_Next;			// p��t� voln� polo�ka (-1 = nen�)
	bool		m_Undo;			// po�adavek registrace zm�n pro UNDO

	int			m_Redraw;		// ��ta� pro vypnut� p�ekreslov�n� okna (0=zapnuto)
	int			m_First;		// index prvn� (ROOT) polo�ky (-1 = nen�)
	HWND		m_Tree;			// p�ipojen� strom (NULL = nen�)
	int			m_Disp;			// rodi� zobrazen� polo�ek (-1 = v�e, -2 = nic)
	int			m_IconWidth;	// ���ka ikony (0 = nen�)
	int			m_IconHeight;	// v��ka ikony (0 = nen�)
	int			m_BufID;		// identifikace tohoto bufferu
	int			m_Def;			// definovan� polo�ka (-1 = nen�)
	CText		m_Nadpis;		// nadpis okna

	CFont		m_FontSet;		// uschovan� nastaven� fontu
	DWORD		m_CharSet;		// znakov� sada fontu
	HFONT		m_Font;			// font (NULL=nen�)

// vytvo�en� nov�ch dat (vrac� TRUE=operace OK) (odd�leno kv�li lep�� optimalizaci)
	bool NewData();

// vytvo�en� nov� polo�ky (vrac� index polo�ky, p�i chyb� vrac� -1)
	int NewItem();

// zru�en� polo�ky - za�azen� do voln�ch polo�ek (nekontroluje index)
	void _fastcall DelItem(const int index);

// p��prava indexu stavov� ikony (upravit s INDEXTOSTATEIMAGEMASK() !)
	int GetStateImage(int index);

// nov� zobrazen� polo�ky
	void DispNewItem(int index);

// zaji�t�n� nov�ch dat u kop�rovan� polo�ky (vol�no t� z Clipboard::Paste)
	void CopyNewData(PROGITEM* item);

// duplikace dat u kop�rovan� polo�ky
	void CopyDupData(PROGITEM* item);

// ---------------------------- ve�ejn� funkce ------------------------------

public:

// konstruktor a destruktor
	CBufProg();
	~CBufProg();

// statick� konstruktor a destruktor
	void Init();
	void Term();

// zru�en� v�ech polo�ek v bufferu (ukl�d�n� za�ne op�t po �ad� od za��tku, nem�n� obsah okna)
	void DelAll();

// poskytnut� bufferu dat
	inline PROGITEM* Data() const { return m_Data; };

// poskytnut� bufferu platnosti (hodnoty TRUE a FALSE)
	inline bool* Valid() const { return m_Valid; };

// poskytnut� po�tu platn�ch polo�ek v bufferu
	inline int Num() const { return m_Num; };

// poskytnut� velikosti bufferu (v�etn� zru�en�ch polo�ek)
	inline int Max() const { return m_Max; };

// nastaven� po�adavku registrace zm�n pro UNDO
	inline void UndoOn() { m_Undo = true; };
	inline void UndoOff() { m_Undo = false; };
	inline void UndoSet(const bool undo) { m_Undo = undo; };

// kontrola platnosti polo�ky
	inline BOOL IsValid(const int index) const
		{ return (((DWORD)index < (DWORD)m_Max) && m_Valid[index]); };

	inline BOOL IsNotValid(const int index) const
		{ return (((DWORD)index >= (DWORD)m_Max) || !m_Valid[index]); };

// poskytnut� p��stupu k polo�ce (bez kontroly indexu)
	inline PROGITEM& operator[] (const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const PROGITEM& operator[] (const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline PROGITEM& At(const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const PROGITEM& At(const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

// vypnut�/zapnut� p�ekreslov�n� (mus� b�t sp�rov�no!)
	void RedrawOff();
	void RedrawOn();

// poskytnut� indexu prvn� ROOT polo�ky (-1 = nen�)
	inline int First() const { return m_First; };

// poskytnut�/nastaven� handle stromu (neaktualizuje zobrazen�, zneplatn� ikony)
	inline HWND Tree() const { return m_Tree; };
	inline void Tree(const HWND tree)
	{ 
		if (tree != m_Tree)
		{
			m_Tree = tree; 
			m_IconWidth = 0; 
			m_IconHeight = 0; 
			m_FontSet.Height = -1;
			AktFont();
		}
	};

// aktualizace fontu v okn�
	void AktFont();

// poskytnut�/nastaven� rodi�e zobrazen� polo�ek (aktualizuje zobrazen�, -1 = v�e, -2 = nic)
	inline int Disp() const { return m_Disp; };
	void Disp(int disp);

// poskytnut�/nastaven� rozm�r� ikon (zoom: TRUE = ikony 32*32)
	inline int IconWidth() const { return m_IconWidth; };
	inline void IconWidth(int width) { m_IconWidth = width; };
	inline int IconHeight() const { return m_IconHeight; };
	inline void IconHeight(int height) { m_IconHeight = height; };
	void IconList(SMALLICON zoom);

// poskytnut�/nastaven� ID bufferu
	inline int BufID() const { return m_BufID; };
	inline void BufID(int id) { m_BufID = id; };

// poskytnut�/nastaven� definovan� polo�ky (-1 = odzna�en�)
	inline int Def() const { return m_Def; };
	void Def(int index);

// poskytnut�/nastaven� nadpisu okna
	const CText& Nadpis() const { return m_Nadpis; };
	void Nadpis(const CText& text);

// poskytnut�/nastaven� prvn� zobrazen� polo�ky (-1 = nen�)
	int Top();
	void Top(int index);

// poskytnut�/nastaven� vybran� polo�ky (-1 = nen�)
	int Select();
	void Select(int index);

// aktualizace zobrazen� stavov� ikony polo�ky pro okno editoru programu
	void _fastcall AktStateImage(int index);

// index n�sleduj�c� polo�ky ve stromu (cyklicky, -1 = navr�t� se prvn� polo�ka)
	int _fastcall NextItem(int index);

// index p�edch�zej�c� polo�ky ve stromu (cyklicky, -1 = navr�t� se posledn� polo�ka)
	int _fastcall PrevItem(int index);

// navr�cen� polo�ky v UNDO operaci (vrac� TRUE=operace OK)
// prov�d� z�znam do UNDO bufferu
	bool UndoIns(const PROGITEM* item, const int index);

// vlo�en� polo�ky bez nav�z�n� dat (pro UNDO buffer)
// vrac� index polo�ky, p�i chyb� pam�ti vrac� -1
	int Insert0(const PROGITEM* item);

// zru�en� polo�ky bez nav�z�n� dat (pro UNDO buffer)
	void Del0(int index);

// vlo�en� polo�ky (vrac� index polo�ky) - parametry nemus� b�t platn�
//  parent = rodi� (-1=ROOT), after=p�edch�zej�c� polo�ka 
//		(-1=prvn�, -2=posledn�, -3=abecedn�, -4=abecedn� s p�ednost� skupin)
//  Polo�ky nastaven� funkc�: Parent, Next, Prev, Child, HTree, Refer,
	int Insert(const PROGITEM* item, int parent, int after);

// na�ten� inicializa�n� tabulky bufferu (star� obsah bufferu se neru�� - lze p�id�vat)
// p�i chyb� pam�ti vrac� FALSE
	bool InitTab(const INITTREE* tab, const int num);

// vytvo�en� pr�zdn� polo�ky (vrac� index polo�ky, -1=chyba pam�ti)
	int New();

// nalezen� polo�ky podle handle (-1 = nen�)
	int Find(HTREEITEM htree);

// nalezen� polo�ky s ukazatelem na data (-1 = nen�)
	int SrcDat(int bufID, int index);

// nalezen� referuj�c� polo�ky od aktu�ln� polo�ky cyklicky (-1 = nenalezeno)
	int SrcRef(int RefBlok, int RefIndex, BOOL next);

// nalezen� indexu textu k polo�ce (-1 = neplatn�)
	int _fastcall GetText(const int index);

// nalezen� indexu ikony k polo�ce (-1 = neplatn�)
	int _fastcall GetIcon(const int index);

// poskytnut� po�tu referenc� pro u�ivatele
	int GetRefer(int index);

// test povolen� zru�en� v�tve z hlediska referenc� na polo�ky (TRUE=OK)
	bool TestRefer(int index);

// p�ekreslen� ikony ve stromu
	void ReDispIcon(int icon);

// test ��sla funkce, zda je to objekt (s jedine�n�m jm�nem)
	bool _fastcall TestObj(int func);

// nalezen� objektu dan�ho jm�na (-1 = nenalezeno) - rozli�uje velk�/mal� p�smena
//		name ..... hledan� text
//		over ..... p�esko�en� polo�ka (-1 = nen�)
//		first .... prvn� polo�ka (-1 = od za��tku)
//		next ..... p��t� polo�ka pro zastaven� (-1 = po konec)
	int FindObj(const CMultiText& name, int over = -1, int first = -1, int next = -1);

// zru�en� v�tve stromu (p�i chyb� pam�ti vrac� FALSE - stav nen� navr�cen!)
	bool Del(int index);

// p�esun v�tve stromu (vrac� nov� index polo�ky)
// (parent: -1 = ROOT, after: -1 = za��tek, -2 = konec)
	int Move(int parent, int after, int src);

// kopie v�tve stromu - vrac� c�lov� index polo�ky (p�i chyb� vrac� <0)
// (parent: -1 = ROOT, pos: -1 = za��tek, -2 = konec)
	int Copy(int parent, int pos, int bufID, int src);
};
