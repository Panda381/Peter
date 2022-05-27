
/***************************************************************************\
*																			*
*							Prov�d�n� programu - sprajt						*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inline vol�n� p��kazu - sprajt

inline void FSprite(CSprite& sprite)
{
	ExecItem++;
	ExecItem[-1].ExecSpr(sprite);
}


// funkce
void _fastcall FSpriteFunc(CSprite& sprite);		// funkce s n�vratem sprajtu

// prom�nn�
void _fastcall FSpriteEmpty(CSprite& sprite);		// pr�zdn� sprajt
void _fastcall FSpriteObj(CSprite& sprite);			// glob�ln� sprajt (Data = index)
void _fastcall FSpriteObjList(CSprite& sprite);		// globaln� sprajt v seznamu (Data = index, List = seznam)
void _fastcall FSpriteLoc(CSprite& sprite);			// lok�ln� sprajt (Data = index)
void _fastcall FSpriteLocList(CSprite& sprite);		// lok�ln� sprajt v seznamu (Data = index, List = seznam)

void _fastcall FGetFileSprite(CSprite& sprite);		// na�ten� sprajtu ze souboru
