
/***************************************************************************\
*																			*
*							Komprese/dekomprese dat							*
*																			*
\***************************************************************************/


//////////////////////////////////////////////////////////////////////////////
// komprese dat obrázku:
//		dst = výstupní buffer (mìl by být minimálnì o 1/8 vìtší než vstupní buffer)
//		src = vstupní buffer s daty
//		size = velikost dat ke kompresi
//		width = délka linky bitmapy (pro offset pøedešlé linky)
// výstup: velikost dat ve výstupním bufferu

int Compress(BYTE* dst, BYTE* src, int size, int width);


//////////////////////////////////////////////////////////////////////////////
// dekomprese dat
//		dst = výstupní buffer
//		src = vstupní buffer s daty
//		size = velikost výstupních dat (nezkomprimovaných)
//		width = délka linky bitmapy (pro offset pøedešlé linky)

void DeCompress(BYTE* dst, BYTE* src, int size, int width);
