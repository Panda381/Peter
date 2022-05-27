
/***************************************************************************\
*																			*
*								Editor zvukù								*
*																			*
\***************************************************************************/

namespace EditSound
{
	extern HWND DispWnd;				// okno displeje

// inicializace pøi startu
	void StartInit();

// zahájení editace
	void BegEdit(int index);

// pøesun okna displeje
	HDWP MoveDisp(HDWP hdwp);

// nastavení èasového údaje (v sekundách)
	void DispSet(int time);

// zobrazení délky zvuku
	void DispLength();

// zobrazení
	void Disp();

// obsluha èasovaèe (TRUE=obslouženo)
	BOOL OnTimer(UINT timerID);

// start pøehrávání
	void OnPlay();

// pozastavení nahrávání, pøehrávání
	void OnPause();

// zastavení nahrávání i pøehrávání
	void OnStop();

// pøepnutí pøíznaku opakování
	void OnLoop();

// start nahrávání
	void OnRecord();

// aktualizace voleb
	void UpdateMenu();

// kopie bloku
	void Copy();

// navrácení bloku
	void Paste();

// aktualizace voleb bloku
	void UpdateClipboard();
}
