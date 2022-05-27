
/***************************************************************************\
*																			*
*								Historie editace							*
*																			*
\***************************************************************************/

namespace HistEdit
{

// vynulov�n� bufferu historie
	void DelAll();

// zru�en� jedn� polo�ky z bufferu
	void Del(int index);

// zru�en� polo�ek v bufferu (v�etn� editovan�) podle dat (pro sprajt pic=-1 v�echny obr�zky)
// p�i chyb� pam�ti vrac� FALSE
	bool Del(int bufID, int index, int sprite, int pic);

// aktualizace voleb pro obsluhu historie
	void Update();

// ulo�en� editovan�ho prvku do bufferu historie
	void Push();

// n�vrat editovan�ho prvku z bufferu historie (z aktivn�ho ukazatele)
	void Pop();

// p�ede�l� historie
	void OnPrev();

// n�sleduj�c� historie
	void OnNext();
}
