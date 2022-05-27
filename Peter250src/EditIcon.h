
/***************************************************************************\
*																			*
*							Editor ikon a obr�zk�							*
*																			*
\***************************************************************************/

namespace EditIcon
{
extern	BOOL	NewDimPoint;			// rozm�ry zad�ny v bodech

// ----------- obsluha volby editace -------

// inicializace editoru ikon p�i startu
	void StartInit();

// zah�jen� editace
	void BegEdit(int bufID, int index);

// nastaven� rozm�r� obr�zku
	void OnDimen();


// ----------- obsluha zobrazen� ------------

// p�epo�et rozm�r� editoru ikon a obr�zk�
	void ReCalc();

// zobrazen� cel�ho okna editoru
	void Disp();

// aktualiza�n� p�ekreslen� obr�zku (s p�epo�tem rozm�r�)
	void ReDisp();

// p�ekreslen� okraj� kolem obr�zku
	void DispRam(HDC dc);

// aktualiza�n� p�ekreslen� obr�zku
	void DispAkt(HDC dc);

// vykreslen� pol��ka v�b�ru barvy
	void DispCol(int col);
	void DispCol(HDC dc, int col);

// zv�t�en� m���tka
	void ZoomIn();

// zmen�en� m���tka
	void ZoomOut();

// aktualizace tla��tek pro zv�t�en�/zmen�en� m���tka
	void UpdateZoomInOut();

// p�ekreslen� ikon ve stromech
	void UpdateTree();

// zapnut�/vypnut� rastru
	void OnRastr();

// ---------------- obsluha posuvn�k� ---------

// p�esun posuvn�k� p�i zm�n� rozm�r� okna
	HDWP MoveScroll(HDWP hdwp);

// aktualizace zobrazen� posuvn�k�
	void SetScroll();

// horizont�ln� posuvn�k
	void OnHScroll(int code, int pos);

// vertik�ln� posuvn�k
	void OnVScroll(int code, int pos);

// ------------ obsluha editace -------------

// �schova obr�zku do pomocn�ho bufferu
	void Push();

// n�vrat obr�zku z pomocn�ho bufferu
	void Pop();

// na�ten� bodu z bufferu
	inline BYTE _fastcall GetPush(int x, int y);

// nastaven� bodu v bufferu
	inline void _fastcall SetPush(int x, int y, BYTE col);

// nastaven� indik�toru barvy
	void SetColInd(BYTE col);

// na�ten� bodu obr�zku
	inline BYTE _fastcall GetPoint(int x, int y);

// obsluha zobrazen� kurzoru my�i (TRUE=obslou�eno)
	BOOL OnSetCursor(int x, int y);

// stisk tla��tka my�i (TRUE = obslou�eno)
	BOOL OnButtonDown(UINT flags, int x, int y, BOOL right, BOOL dclick);

// posun my�i
	void OnMouseMove(UINT flags, int x, int y);

// uvoln�n� tla��tka my�i
	void OnButtonUp(UINT keys, BOOL right);

// vstup znaku z kl�vesnice (TRUE=obslou�eno)
	BOOL OnChar(TCHAR znak);

// vstup kl�vesy z kl�vesnice (TRUE=obslou�eno)
	BOOL OnKeyDown(int key);

// nastaven� edita�n�ho m�du
	void SetMode(int mode);

// proveden� editace bloku (-1 = zat�m neur�eno)
	void EditBlok(int mode);

// aktualizace voleb v�b�ru m�du
	void UpdateMenu();

// -------------- obsluha kreslen� ---------

// vykreslen� bodu
	void _fastcall SetPoint(int x, int y, BYTE col);

// vykreslen� bodu �t�tce
	void SetPaint(int x, int y, BYTE col);

// nastaven� bodu spreje
	void SetSpray(int x, int y, BYTE col);

// nastaven� ��ry �t�tcem
	void SetLinePaint(int x1, int y1, int x2, int y2, BYTE col);

// vykreslen� obd�ln�ku
	void SetRect(int x1, int y1, int x2, int y2, BYTE col);

// vykreslen� obd�ln�ku s v�pln�
	void SetRectFill(int x1, int y1, int x2, int y2, BYTE col);

// vykreslen� kru�nice
	void SetRound(int x1, int y1, int x2, int y2, BYTE col);

// kreslen� kruhu
	void SetRoundFill(int x1, int y1, int x2, int y2, BYTE col);

// kreslen� elipsy
	void SetElip(int x1, int y1, int x2, int y2, BYTE col);

// kreslen� ov�lu
	void SetElipFill(int x1, int y1, int x2, int y2, BYTE col);

// kreslen� koule
	void SetKoule(int x1, int y1, int x2, int y2, BYTE col0);

// v�pl� (col = nov� barva, fil = podklad)
	void SetFill(int x, int y, BYTE col, BYTE fil);

// zobrazen� textu
	void ReDispText();

// -------------- obsluha bloku -----------

// na�ten� bodu z bloku
	inline BYTE _fastcall GetBlok(int x, int y);

// nastaven� bodu v bloku
	inline void _fastcall SetBlok(int x, int y, BYTE col);

// zapnut�/vypnut� v�b�ru bloku
	void DispSelect();

// zobrazen� bloku p�i p�esunu
	void MoveSelect();

// test bodu, zda je uvnit� vybran�ho bloku
	BOOL TestBlok(int x, int y);

// zru�en� bloku
	void Delete();

// kopie bloku (TRUE=operace OK)
	BOOL Copy();

// vyst��en� bloku
	void Cut();

// vlo�en� bloku
	void Paste();

// v�b�r v�eho
	void SelectAll();

// aktualizace voleb bloku
	void UpdateClipboard();

// otisk bloku
	void OnOtisk();

// -------------- obsluha UNDO -------------

// omezen� velikosti UNDO bufferu
//	void LimitUndoSize();

// nastaven� nov� velikosti UNDO bufferu
//	void SetUndoNum(int num);

// ulo�en� obr�zku do undo bufferu
	void PushUndo();

// navr�cen� ikony z undo bufferu
//	void PopUndo();

// UNDO
//	void Undo();

// REDO
//	void Redo();

// aktualizace voleb UNDO a REDO
//	void UpdateUndoRedo();
}
