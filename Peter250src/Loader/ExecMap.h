
/***************************************************************************\
*																			*
*							Prov�d�n� programu - plocha						*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inline vol�n� p��kazu - plocha

inline void FMap(CMap& map)
{
	ExecItem++;
	ExecItem[-1].ExecMap(map);
}


// funkce
void _fastcall FMapFunc(CMap& map);			// funkce s n�vratem plochy

// prom�nn�
void _fastcall FMapEmpty(CMap& map);		// pr�zdn� plocha
void _fastcall FMapMain(CMap& map);			// hlavn� plocha
void _fastcall FMapObj(CMap& map);			// glob�ln� plocha (Data = index)
void _fastcall FMapObjList(CMap& map);		// globaln� plocha v seznamu (Data = index, List = seznam)
void _fastcall FMapLoc(CMap& map);			// lok�ln� plocha (Data = index)
void _fastcall FMapLocList(CMap& map);		// lok�ln� plocha v seznamu (Data = index, List = seznam)

void _fastcall FMapCopy(CMap& map);			// kopie v��ezu plochy
void _fastcall FMapNew(CMap& map);			// nov� plocha
