
/***************************************************************************\
*																			*
*								Editor hudby								*
*																			*
\***************************************************************************/

namespace EditMusic
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

// zobrazení délky hudby
	void DispLength();

// zobrazení
	void Disp();

// obsluha èasovaèe (TRUE=obslouženo)
	BOOL OnTimer(UINT timerID);

// ukonèení pøehrávání
	void OnMMNotify(DWORD flags, int devID);

// start pøehrávání (init = jen zjištìní délky a pøerušení)
	void OnPlay(BOOL init = FALSE);

// pozastavení nahrávání, pøehrávání
	void OnPause();

// zastavení nahrávání i pøehrávání
	void OnStop();

// pøepnutí pøíznaku opakování
	void OnLoop();

// pøevinutí zpìt
	void OnRew();

// pøevinutí vpøed
	void OnFrw();

// aktualizace voleb
	void UpdateMenu();

// kopie bloku
	void Copy();

// navrácení bloku
	void Paste();

// aktualizace voleb bloku
	void UpdateClipboard();
}
