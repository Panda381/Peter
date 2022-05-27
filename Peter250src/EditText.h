
/***************************************************************************\
*																			*
*								Editor textu								*
*																			*
\***************************************************************************/

namespace EditText
{
	extern	int		Index;						// editovaná položka

// opakovaná inicializace po zmìnì jazyku
	void StartReInit();

// inicializace pøi startu
	void StartInit();

// zahájení editace
	void BegEdit(int index);

// pøesun editaèního pole
	HDWP MoveEdit(HDWP hdwp);

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
}
