
/***************************************************************************\
*																			*
*								Editor hudby								*
*																			*
\***************************************************************************/

namespace EditMusic
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

// zobrazen� d�lky hudby
	void DispLength();

// zobrazen�
	void Disp();

// obsluha �asova�e (TRUE=obslou�eno)
	BOOL OnTimer(UINT timerID);

// ukon�en� p�ehr�v�n�
	void OnMMNotify(DWORD flags, int devID);

// start p�ehr�v�n� (init = jen zji�t�n� d�lky a p�eru�en�)
	void OnPlay(BOOL init = FALSE);

// pozastaven� nahr�v�n�, p�ehr�v�n�
	void OnPause();

// zastaven� nahr�v�n� i p�ehr�v�n�
	void OnStop();

// p�epnut� p��znaku opakov�n�
	void OnLoop();

// p�evinut� zp�t
	void OnRew();

// p�evinut� vp�ed
	void OnFrw();

// aktualizace voleb
	void UpdateMenu();

// kopie bloku
	void Copy();

// navr�cen� bloku
	void Paste();

// aktualizace voleb bloku
	void UpdateClipboard();
}
