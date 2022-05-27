
/***************************************************************************\
*																			*
*							Provádìní programu - plocha						*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inline volání pøíkazu - plocha

inline void FMap(CMap& map)
{
	ExecItem++;
	ExecItem[-1].ExecMap(map);
}


// funkce
void _fastcall FMapFunc(CMap& map);			// funkce s návratem plochy

// promìnné
void _fastcall FMapEmpty(CMap& map);		// prázdná plocha
void _fastcall FMapMain(CMap& map);			// hlavní plocha
void _fastcall FMapObj(CMap& map);			// globální plocha (Data = index)
void _fastcall FMapObjList(CMap& map);		// globalní plocha v seznamu (Data = index, List = seznam)
void _fastcall FMapLoc(CMap& map);			// lokální plocha (Data = index)
void _fastcall FMapLocList(CMap& map);		// lokální plocha v seznamu (Data = index, List = seznam)

void _fastcall FMapCopy(CMap& map);			// kopie výøezu plochy
void _fastcall FMapNew(CMap& map);			// nová plocha
