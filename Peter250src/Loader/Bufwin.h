
/***************************************************************************\
*																			*
*								Prvky oken									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// typy prvk� oken

enum WINTYP {
	WINITEM_WINDOW,					// okno
	WINITEM_WINDOWRO,				// okno bez uzav�en�
	WINITEM_BUTTON,					// tla��tko
	WINITEM_DEFBUTTON,				// implicitn� tla��tko
	WINITEM_BUTTONICON,				// ikonov� sp�na�
	WINITEM_BUTTONCHECK,			// za�krt�vac� tla��tko
	WINITEM_BUTTONCHECK2,			// tla��tkov� p�ep�na�
	WINITEM_BUTTON3,				// 3-stavov� p�ep�na�
	WINITEM_BUTTONRADIO,			// p�ep�nac� tla��tko
	WINITEM_BUTTONRADIO2,			// p�ep�nac� tla��tko = za��tek skupiny
	WINITEM_EDIT,					// textov� ��dek
	WINITEM_MEMO,					// textov� pole
	WINITEM_GROUP,					// skupina
	WINITEM_LISTBOX,				// seznam
	WINITEM_LISTEDIT,				// seznam s editac�
	WINITEM_COMBOBOX,				// rozbalovac� seznam
	WINITEM_COMBOEDIT,				// rozbalovac� seznam s editac�
	WINITEM_WINTEXT,				// text
	WINITEM_WINICON,				// ikona
	WINITEM_WINPIC,					// obr�zek
	WINITEM_RICHEDIT,				// textov� pole Richedit
	WINITEM_RICHMEMO,				// textov� pole bez zalamov�n�
	WINITEM_PROGRESS,				// indik�tor pr�b�hu
	WINITEM_HTRACKBAR,				// horizont�ln� t�hlo
	WINITEM_VTRACKBAR,				// vertik�ln� t�hlo
	WINITEM_TABLE,					// tabulka
	WINITEM_HSCROLLBAR,				// horizont�ln� posuvn�k
	WINITEM_VSCROLLBAR,				// vertik�ln� posuvn�k
	WINITEM_TABS,					// z�lo�ky
};


/////////////////////////////////////////////////////////////////////////////
// struktura polo�ky prvku okna

typedef struct WINITEM_
{
	HWND	Wnd;					// handle prvku
	int		Parent;					// index rodi�e prvku (0=hlavn� okno, -1=nic)
	int		Typ;					// typ prvku
	int		Prev;					// p�edch�zej�c� prvek okna (v�etn� samotn�ho okna)
	int		Next;					// n�sleduj�c� prvek okna (v�etn� samotn�ho okna)
	CString	Text;					// text prvku
	CIcon	Icon;					// ikona
	CPicture Picture;				// obr�zek
	BYTE*	PictureData;			// cachovan� data obr�zku se zarovn�n�m na 4 bajty (NULL=nen�)
	int		Left;					// pozice prvku zleva (v bodech)
	int		Top;					// pozice prvku shora (v bodech)
	int		Width;					// ���ka prvku (v bodech)
	int		Height;					// v��ka prvku (v bodech)
	int		ClientWidth;			// klientsk� ���ka prvku (v bodech)
	int		ClientHeight;			// klientsk� v��ka prvku (v bodech)
	bool	HasCaption;				// m� titulek
	bool	HasBorder;				// m� okraj
	bool	HasSize;				// okno s m�nitelnou velikost� (nebo dal�� varianta okraje dialog. prvku)
	bool	AlwaysTop;				// v�dy naho�e
	bool	MayClose;				// mo�n� uzav�en� okna
	bool	TabStop;				// zar�ka pro TAB
	bool	Show;					// p��znak zobrazen� prvku
	bool	Disable;				// p��znak z�kazu prvku
	bool	Click;					// kliknuto na tla��tko
	bool	TextValid;				// text je platn�
	bool	Check;					// p�ep�na� zapnut
	bool	Check3;					// nedefinovan� stav p�ep�na�e
	bool	Bold;					// zv�razn�n� text
	bool	Italic;					// kurz�va
	bool	Underline;				// podtr�en�
	bool	Strikeout;				// p�e�krtnut�
	bool	Serif;					// pati�ky
	bool	Fixed;					// text s pevnou rozte��
	int		FontHeight;				// bodov� v��ka znak� (0=b�n�)
	double	FontWidth;				// relativn� ���ka znak� (0=b�n�, 1=z�kladn�)
	double	Angle;					// �hel sklonu (0=b�n�)
	int		FontCol;				// barva textu
	int		FontBack;				// barva pozad�
	HBRUSH	FontBrush;				// �t�tec pozad�
	HFONT	Font;					// aktu�ln� font
  // tabulka
	bool	TabHead;				// tabulka m� z�hlav�
	int		TabCol;					// aktivn� sloupec tabulky
	int		TabRow;					// aktivn� ��dek tabulky
	int		TabRowF;				// aktivn� ��dek tabulky se zam��en�m
	int		TabCols;				// po�et sloupc� tabulky
	int		TabRows;				// po�et ��dk� tabulky
	CString* TabData;				// data tabulky (po ��dc�ch)
	BYTE*	TabAlign;				// zarovn�n� sloupc� tabulky
	int		TabDataMax;				// velikost bufferu seznamu dat tabulky (po�et polo�ek)
  // posuvn�k
	int		ScrollMax;				// maxim�ln� hodnota pozice posuvn�ku (0 a� SCROLLRANGE-PAGE)
	double	ScrollPage;				// d�lka str�nky (0.0001 a� 1)
	double	ScrollPos;				// aktu�ln� pozice (0 a� 1)
} WINITEM;

#define SCROLLRANGE	10000			// rozsah posuvn�ku

/////////////////////////////////////////////////////////////////////////////
// prvky oken (index 0 = hlavn� okno)

class CBufWin
{

// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn�
	WINITEM*		m_Data;			// ukazatel na data
	bool*			m_Valid;		// p��znaky platnosti polo�ek
	int				m_Num;			// po�et platn�ch polo�ek v bufferu
	int				m_Max;			// velikost bufferu (polo�ek)
	int				m_Next;			// p��t� voln� polo�ka (-1 = nen�)

// vytvo�en� nov� polo�ky
	inline int NewItem()
	{
		if (m_Next < 0)				// nen� dal�� polo�ka?
		{
			NewData();				// vytvo�en� nov�ch dat
		}

		int i = m_Next;				// p��t� volna polo�ka
		m_Next = *(int*)(m_Data + i); // dal�� polo�ka
		m_Valid[i] = true;			// nastaven� p��znaku platnosti polo�ky
		WINITEM* item = m_Data + i;
		MemFill(item, sizeof(WINITEM), 0);
		item->Text.Init();			// inicializace textu
		item->Icon.Init();
		item->Picture.Init();
		item->PictureData = NULL;
		item->FontBrush = NULL;
		item->ScrollMax = (SCROLLRANGE*9 + 5)/10;
		item->ScrollPage = 0.1;
		item->Prev = i;
		item->Next = i;
		m_Num++;					// zv��en� ��ta�e platn�ch polo�ek
		return i;
	};

// vytvo�en� nov�ch dat (odd�leno kv�li lep�� optimalizaci)
	void NewData();

// ---------------------------- ve�ejn� funkce ------------------------------

public:

// konstruktor a destruktor
	CBufWin();
	~CBufWin();

// statick� konstruktor a destruktor
	void Init();			// statick� konstruktor
	void Term();			// statick� destruktor

// zru�en� v�ech polo�ek v bufferu (ukl�d�n� za�ne op�t po �ad� od za��tku)
	void DelAll();

// poskytnut� bufferu dat
	inline WINITEM* Data() const { return m_Data; };

// poskytnut� pole platnost�
	inline bool* Valid() const { return m_Valid; };

// poskytnut� po�tu platn�ch polo�ek v bufferu
	inline int Num() const { return m_Num; };

// poskytnut� velikosti bufferu (v�etn� zru�en�ch polo�ek)
	inline int Max() const { return m_Max; };

// kontrola platnosti polo�ky
	inline BOOL IsValid(const int index) const
		{ return (((DWORD)index < (DWORD)m_Max) && m_Valid[index]); };

	inline BOOL IsNotValid(const int index) const
		{ return (((DWORD)index >= (DWORD)m_Max) || !m_Valid[index]); };

// poskytnut� p��stupu k polo�ce (bez kontroly indexu)
	inline WINITEM& operator[] (const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const WINITEM& operator[] (const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline WINITEM& At(const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const WINITEM& At(const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

// zru�en� polo�ky (s kontrolou platnosti indexu)
	void _fastcall Del(const int index);

// vytvo�en� pr�zdn� polo�ky (vrac� index polo�ky)
	int _fastcall New();
};
