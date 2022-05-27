
/***************************************************************************\
*																			*
*							3D rozhraní - OpenGL 2.0						*
*																			*
\***************************************************************************/

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

/////////////////////////////////////////////////////////////////////////////
// inicializace rozhraní OpenGL 2.0 (vrací TRUE=rozhraní je vytvoøeno)

inline bool D3GL2InitD3() { return false; }
inline void D3GL2TermD3() { }

/////////////////////////////////////////////////////////////////////////////
// zahájení funkce zaøízení (vrací TRUE=inicializace OK)
// typ zaøízení: 1=HAL,2=TnLHal,3=REF,4=RGB,5=MMX,6=Ramp,7=Null

inline bool D3GL2Start(int type) { return false; }
inline void D3GL2Stop() { D3GL0Stop(); }

#endif // _MINI
// --------------------- konec vypnutí pro MINI verzi -------------------
