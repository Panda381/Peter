
/***************************************************************************\
*																			*
*							Komprese/dekomprese dat							*
*																			*
\***************************************************************************/


//////////////////////////////////////////////////////////////////////////////
// komprese dat obr�zku:
//		dst = v�stupn� buffer (m�l by b�t minim�ln� o 1/8 v�t�� ne� vstupn� buffer)
//		src = vstupn� buffer s daty
//		size = velikost dat ke kompresi
//		width = d�lka linky bitmapy (pro offset p�ede�l� linky)
// v�stup: velikost dat ve v�stupn�m bufferu

int Compress(BYTE* dst, BYTE* src, int size, int width);


//////////////////////////////////////////////////////////////////////////////
// dekomprese dat
//		dst = v�stupn� buffer
//		src = vstupn� buffer s daty
//		size = velikost v�stupn�ch dat (nezkomprimovan�ch)
//		width = d�lka linky bitmapy (pro offset p�ede�l� linky)

void DeCompress(BYTE* dst, BYTE* src, int size, int width);
