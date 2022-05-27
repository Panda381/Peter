
/***************************************************************************\
*																			*
*							Editor logických hodnot							*
*																			*
\***************************************************************************/

#define EDITLOGID 30231

namespace EditLog
{
// inicializace pøi startu
	void StartInit();

// zahájení editace
	void BegEdit(int index);

// pøesun editaèního pole
	HDWP MoveEdit(HDWP hdwp);

// zobrazení
	void Disp();

// pøepnutí pøepínaèe
	void Switch();
}
