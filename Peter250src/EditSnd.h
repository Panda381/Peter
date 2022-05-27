
/***************************************************************************\
*																			*
*								Editor zvuk�								*
*																			*
\***************************************************************************/

namespace EditSound
{
	extern HWND DispWnd;				// okno displeje

// inicializace p�i startu
	void StartInit();

// zah�jen� editace
	void BegEdit(int index);

// p�esun okna displeje
	HDWP MoveDisp(HDWP hdwp);

// nastaven� �asov�ho �daje (v sekund�ch)
	void DispSet(int time);

// zobrazen� d�lky zvuku
	void DispLength();

// zobrazen�
	void Disp();

// obsluha �asova�e (TRUE=obslou�eno)
	BOOL OnTimer(UINT timerID);

// start p�ehr�v�n�
	void OnPlay();

// pozastaven� nahr�v�n�, p�ehr�v�n�
	void OnPause();

// zastaven� nahr�v�n� i p�ehr�v�n�
	void OnStop();

// p�epnut� p��znaku opakov�n�
	void OnLoop();

// start nahr�v�n�
	void OnRecord();

// aktualizace voleb
	void UpdateMenu();

// kopie bloku
	void Copy();

// navr�cen� bloku
	void Paste();

// aktualizace voleb bloku
	void UpdateClipboard();
}
