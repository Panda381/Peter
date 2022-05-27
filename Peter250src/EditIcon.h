
/***************************************************************************\
*																			*
*							Editor ikon a obrázkù							*
*																			*
\***************************************************************************/

namespace EditIcon
{
extern	BOOL	NewDimPoint;			// rozmìry zadány v bodech

// ----------- obsluha volby editace -------

// inicializace editoru ikon pøi startu
	void StartInit();

// zahájení editace
	void BegEdit(int bufID, int index);

// nastavení rozmìrù obrázku
	void OnDimen();


// ----------- obsluha zobrazení ------------

// pøepoèet rozmìrù editoru ikon a obrázkù
	void ReCalc();

// zobrazení celého okna editoru
	void Disp();

// aktualizaèní pøekreslení obrázku (s pøepoètem rozmìrù)
	void ReDisp();

// pøekreslení okrajù kolem obrázku
	void DispRam(HDC dc);

// aktualizaèní pøekreslení obrázku
	void DispAkt(HDC dc);

// vykreslení políèka výbìru barvy
	void DispCol(int col);
	void DispCol(HDC dc, int col);

// zvìtšení mìøítka
	void ZoomIn();

// zmenšení mìøítka
	void ZoomOut();

// aktualizace tlaèítek pro zvìtšení/zmenšení mìøítka
	void UpdateZoomInOut();

// pøekreslení ikon ve stromech
	void UpdateTree();

// zapnutí/vypnutí rastru
	void OnRastr();

// ---------------- obsluha posuvníkù ---------

// pøesun posuvníkù pøi zmìnì rozmìrù okna
	HDWP MoveScroll(HDWP hdwp);

// aktualizace zobrazení posuvníkù
	void SetScroll();

// horizontální posuvník
	void OnHScroll(int code, int pos);

// vertikální posuvník
	void OnVScroll(int code, int pos);

// ------------ obsluha editace -------------

// úschova obrázku do pomocného bufferu
	void Push();

// návrat obrázku z pomocného bufferu
	void Pop();

// naètení bodu z bufferu
	inline BYTE _fastcall GetPush(int x, int y);

// nastavení bodu v bufferu
	inline void _fastcall SetPush(int x, int y, BYTE col);

// nastavení indikátoru barvy
	void SetColInd(BYTE col);

// naètení bodu obrázku
	inline BYTE _fastcall GetPoint(int x, int y);

// obsluha zobrazení kurzoru myši (TRUE=obslouženo)
	BOOL OnSetCursor(int x, int y);

// stisk tlaèítka myši (TRUE = obslouženo)
	BOOL OnButtonDown(UINT flags, int x, int y, BOOL right, BOOL dclick);

// posun myši
	void OnMouseMove(UINT flags, int x, int y);

// uvolnìní tlaèítka myši
	void OnButtonUp(UINT keys, BOOL right);

// vstup znaku z klávesnice (TRUE=obslouženo)
	BOOL OnChar(TCHAR znak);

// vstup klávesy z klávesnice (TRUE=obslouženo)
	BOOL OnKeyDown(int key);

// nastavení editaèního módu
	void SetMode(int mode);

// provedení editace bloku (-1 = zatím neurèeno)
	void EditBlok(int mode);

// aktualizace voleb výbìru módu
	void UpdateMenu();

// -------------- obsluha kreslení ---------

// vykreslení bodu
	void _fastcall SetPoint(int x, int y, BYTE col);

// vykreslení bodu štìtce
	void SetPaint(int x, int y, BYTE col);

// nastavení bodu spreje
	void SetSpray(int x, int y, BYTE col);

// nastavení èáry štìtcem
	void SetLinePaint(int x1, int y1, int x2, int y2, BYTE col);

// vykreslení obdélníku
	void SetRect(int x1, int y1, int x2, int y2, BYTE col);

// vykreslení obdélníku s výplní
	void SetRectFill(int x1, int y1, int x2, int y2, BYTE col);

// vykreslení kružnice
	void SetRound(int x1, int y1, int x2, int y2, BYTE col);

// kreslení kruhu
	void SetRoundFill(int x1, int y1, int x2, int y2, BYTE col);

// kreslení elipsy
	void SetElip(int x1, int y1, int x2, int y2, BYTE col);

// kreslení oválu
	void SetElipFill(int x1, int y1, int x2, int y2, BYTE col);

// kreslení koule
	void SetKoule(int x1, int y1, int x2, int y2, BYTE col0);

// výplò (col = nová barva, fil = podklad)
	void SetFill(int x, int y, BYTE col, BYTE fil);

// zobrazení textu
	void ReDispText();

// -------------- obsluha bloku -----------

// naètení bodu z bloku
	inline BYTE _fastcall GetBlok(int x, int y);

// nastavení bodu v bloku
	inline void _fastcall SetBlok(int x, int y, BYTE col);

// zapnutí/vypnutí výbìru bloku
	void DispSelect();

// zobrazení bloku pøi pøesunu
	void MoveSelect();

// test bodu, zda je uvnitø vybraného bloku
	BOOL TestBlok(int x, int y);

// zrušení bloku
	void Delete();

// kopie bloku (TRUE=operace OK)
	BOOL Copy();

// vystøížení bloku
	void Cut();

// vložení bloku
	void Paste();

// výbìr všeho
	void SelectAll();

// aktualizace voleb bloku
	void UpdateClipboard();

// otisk bloku
	void OnOtisk();

// -------------- obsluha UNDO -------------

// omezení velikosti UNDO bufferu
//	void LimitUndoSize();

// nastavení nové velikosti UNDO bufferu
//	void SetUndoNum(int num);

// uložení obrázku do undo bufferu
	void PushUndo();

// navrácení ikony z undo bufferu
//	void PopUndo();

// UNDO
//	void Undo();

// REDO
//	void Redo();

// aktualizace voleb UNDO a REDO
//	void UpdateUndoRedo();
}
