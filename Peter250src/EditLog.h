
/***************************************************************************\
*																			*
*							Editor logick�ch hodnot							*
*																			*
\***************************************************************************/

#define EDITLOGID 30231

namespace EditLog
{
// inicializace p�i startu
	void StartInit();

// zah�jen� editace
	void BegEdit(int index);

// p�esun edita�n�ho pole
	HDWP MoveEdit(HDWP hdwp);

// zobrazen�
	void Disp();

// p�epnut� p�ep�na�e
	void Switch();
}
