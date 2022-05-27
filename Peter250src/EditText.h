
/***************************************************************************\
*																			*
*								Editor textu								*
*																			*
\***************************************************************************/

namespace EditText
{
	extern	int		Index;						// editovan� polo�ka

// opakovan� inicializace po zm�n� jazyku
	void StartReInit();

// inicializace p�i startu
	void StartInit();

// zah�jen� editace
	void BegEdit(int index);

// p�esun edita�n�ho pole
	HDWP MoveEdit(HDWP hdwp);

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
}
