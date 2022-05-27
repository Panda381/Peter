
/***************************************************************************\
*																			*
*								Editor sprajtù								*
*																			*
\***************************************************************************/

namespace EditSprite
{
extern	BOOL	MDraging;				// pøíznak tažení ikony editoru sprajtù
//extern	CIcon*	EditIcon;				// editovaná ikona
extern	int		Index;					// editovaný sprajt
extern	int		IndexPic;				// index editovaného obrázku

// inicializace pøi startu
	void StartInit();

// zahájení editace
	void BegEdit(int index);

// nastavení rozmìrù plochy
	void OnDimen();

// ----------- obsluha zobrazení ------------

// pøepoèet rozmìrù editoru ikon a obrázkù
	void ReCalc();

// zobrazení celého okna editoru
	void Disp();

// aktualizaèní pøekreslení plochy (s pøepoètem rozmìrù)
	void ReDisp();

// aktualizaèní pøekreslení plochy
	void DispAkt(HDC dc);

// ---------------- obsluha posuvníkù ---------

// pøesun posuvníkù pøi zmìnì rozmìrù okna
	HDWP MoveScroll(HDWP hdwp);

// aktualizace zobrazení posuvníkù
	void SetScroll();

// horizontální posuvník
	void OnHScroll(int code, int pos);

// vertikální posuvník
	void OnVScroll(int code, int pos);

// -------------- obsluha editace -----------

// ukonèení tažení
	void EndDrag();

// stisk tlaèítka myši (TRUE = obslouženo)
	BOOL OnButtonDown(UINT flags, int x, int y, BOOL right, BOOL dclick);

// posun myši
	void OnMouseMove(UINT flags, int x, int y);

// uvolnìní tlaèítka myši
	void OnButtonUp(UINT keys, BOOL right);

// položení obrázku zvnìjšku
	void DragDrop(int pic);

// test
	void OnTest();

// -------------- obsluha UNDO -------------

// úschova pro UNDO
	void PushUndo();
}
