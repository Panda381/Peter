
/***************************************************************************\
*																			*
*								Editor ploch								*
*																			*
\***************************************************************************/

namespace EditMap
{
extern	BOOL	MDraging;				// pøíznak tažení ikony editoru plochy

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

// zapnutí/vypnutí rastru
	void OnRastr();

// obsluha zobrazení kurzoru myši (TRUE=obslouženo)
	BOOL OnSetCursor(int x, int y);

// ---------------- obsluha posuvníkù ---------

// pøesun posuvníkù pøi zmìnì rozmìrù okna
	HDWP MoveScroll(HDWP hdwp);

// aktualizace zobrazení posuvníkù
	void SetScroll();

// aktualizace pozice editaèního pole
	void SetEditMapNum();

// horizontální posuvník
	void OnHScroll(int code, int pos);

// vertikální posuvník
	void OnVScroll(int code, int pos);

// -------------- obsluha editace -----------

// zahájení editace èíselného parametru
	void BegEditSwcNum(int x, int y);

// zmìna editovaného èísla
	void OnChangeEditSwcNum();

// ukonèení editace èísla
	void EndEditSwcNum();

// ukonèení tažení
	void EndDrag();

// stisk tlaèítka myši (TRUE = obslouženo)
	BOOL OnButtonDown(UINT flags, int x, int y, BOOL right, BOOL dclick);

// posun myši
	void OnMouseMove(UINT flags, int x, int y);

// uvolnìní tlaèítka myši
	void OnButtonUp(UINT keys, BOOL right);

// položení ikony zvnìjšku
	void DragDrop(int icon);

// vstup klávesy z klávesnice (TRUE=obslouženo)
	BOOL OnKeyDown(int key);

// nastavení editaèního módu
	void SetMode(int mode);

// aktualizace voleb výbìru módu
	void UpdateMenu();

// -------------- obsluha bloku -----------

// výbìr všeho
	void SelectAll();

// zrušení bloku
	void Delete();

// kopie bloku
	void Copy();

// vystøížení bloku
	void Cut();

// navrácení bloku
	void Paste();

// aktualizace voleb bloku
	void UpdateClipboard();

// mód výplnì
	void OnFillMap();

// editaèní mód
	void OnEditMap();

// -------------- obsluha UNDO -------------

// úschova plochy pro UNDO
	void PushUndo();
}
