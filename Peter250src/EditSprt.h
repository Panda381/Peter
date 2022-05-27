
/***************************************************************************\
*																			*
*								Editor sprajt�								*
*																			*
\***************************************************************************/

namespace EditSprite
{
extern	BOOL	MDraging;				// p��znak ta�en� ikony editoru sprajt�
//extern	CIcon*	EditIcon;				// editovan� ikona
extern	int		Index;					// editovan� sprajt
extern	int		IndexPic;				// index editovan�ho obr�zku

// inicializace p�i startu
	void StartInit();

// zah�jen� editace
	void BegEdit(int index);

// nastaven� rozm�r� plochy
	void OnDimen();

// ----------- obsluha zobrazen� ------------

// p�epo�et rozm�r� editoru ikon a obr�zk�
	void ReCalc();

// zobrazen� cel�ho okna editoru
	void Disp();

// aktualiza�n� p�ekreslen� plochy (s p�epo�tem rozm�r�)
	void ReDisp();

// aktualiza�n� p�ekreslen� plochy
	void DispAkt(HDC dc);

// ---------------- obsluha posuvn�k� ---------

// p�esun posuvn�k� p�i zm�n� rozm�r� okna
	HDWP MoveScroll(HDWP hdwp);

// aktualizace zobrazen� posuvn�k�
	void SetScroll();

// horizont�ln� posuvn�k
	void OnHScroll(int code, int pos);

// vertik�ln� posuvn�k
	void OnVScroll(int code, int pos);

// -------------- obsluha editace -----------

// ukon�en� ta�en�
	void EndDrag();

// stisk tla��tka my�i (TRUE = obslou�eno)
	BOOL OnButtonDown(UINT flags, int x, int y, BOOL right, BOOL dclick);

// posun my�i
	void OnMouseMove(UINT flags, int x, int y);

// uvoln�n� tla��tka my�i
	void OnButtonUp(UINT keys, BOOL right);

// polo�en� obr�zku zvn�j�ku
	void DragDrop(int pic);

// test
	void OnTest();

// -------------- obsluha UNDO -------------

// �schova pro UNDO
	void PushUndo();
}
