
/***************************************************************************\
*																			*
*								Editor èísel								*
*																			*
\***************************************************************************/

namespace EditNum
{
// inicializace pøi startu
	void StartInit();

// zahájení editace
	void BegEdit(int index);

// pøesun editaèního pole
	HDWP MoveEdit(HDWP hdwp);

// zobrazení
	void Disp();

// vstup znaku z klávesnice (TRUE=zrušit)
	BOOL OnChar(TCHAR znak);

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

// zmìna textu
	void OnChange();

// znovunaètení èísla
	void ReLoad();
}
