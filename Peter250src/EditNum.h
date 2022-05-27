
/***************************************************************************\
*																			*
*								Editor ��sel								*
*																			*
\***************************************************************************/

namespace EditNum
{
// inicializace p�i startu
	void StartInit();

// zah�jen� editace
	void BegEdit(int index);

// p�esun edita�n�ho pole
	HDWP MoveEdit(HDWP hdwp);

// zobrazen�
	void Disp();

// vstup znaku z kl�vesnice (TRUE=zru�it)
	BOOL OnChar(TCHAR znak);

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

// zm�na textu
	void OnChange();

// znovuna�ten� ��sla
	void ReLoad();
}
