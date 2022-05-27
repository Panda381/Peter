
/***************************************************************************\
*																			*
*								Editor ploch								*
*																			*
\***************************************************************************/

namespace EditMap
{
extern	BOOL	MDraging;				// p��znak ta�en� ikony editoru plochy

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

// zapnut�/vypnut� rastru
	void OnRastr();

// obsluha zobrazen� kurzoru my�i (TRUE=obslou�eno)
	BOOL OnSetCursor(int x, int y);

// ---------------- obsluha posuvn�k� ---------

// p�esun posuvn�k� p�i zm�n� rozm�r� okna
	HDWP MoveScroll(HDWP hdwp);

// aktualizace zobrazen� posuvn�k�
	void SetScroll();

// aktualizace pozice edita�n�ho pole
	void SetEditMapNum();

// horizont�ln� posuvn�k
	void OnHScroll(int code, int pos);

// vertik�ln� posuvn�k
	void OnVScroll(int code, int pos);

// -------------- obsluha editace -----------

// zah�jen� editace ��seln�ho parametru
	void BegEditSwcNum(int x, int y);

// zm�na editovan�ho ��sla
	void OnChangeEditSwcNum();

// ukon�en� editace ��sla
	void EndEditSwcNum();

// ukon�en� ta�en�
	void EndDrag();

// stisk tla��tka my�i (TRUE = obslou�eno)
	BOOL OnButtonDown(UINT flags, int x, int y, BOOL right, BOOL dclick);

// posun my�i
	void OnMouseMove(UINT flags, int x, int y);

// uvoln�n� tla��tka my�i
	void OnButtonUp(UINT keys, BOOL right);

// polo�en� ikony zvn�j�ku
	void DragDrop(int icon);

// vstup kl�vesy z kl�vesnice (TRUE=obslou�eno)
	BOOL OnKeyDown(int key);

// nastaven� edita�n�ho m�du
	void SetMode(int mode);

// aktualizace voleb v�b�ru m�du
	void UpdateMenu();

// -------------- obsluha bloku -----------

// v�b�r v�eho
	void SelectAll();

// zru�en� bloku
	void Delete();

// kopie bloku
	void Copy();

// vyst��en� bloku
	void Cut();

// navr�cen� bloku
	void Paste();

// aktualizace voleb bloku
	void UpdateClipboard();

// m�d v�pln�
	void OnFillMap();

// edita�n� m�d
	void OnEditMap();

// -------------- obsluha UNDO -------------

// �schova plochy pro UNDO
	void PushUndo();
}
