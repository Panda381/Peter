
/***************************************************************************\
*																			*
*							Prov�d�n� programu - obr�zek					*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inline vol�n� p��kazu - obr�zek

inline void FPicture(CPicture& pic)
{
	ExecItem++;
	ExecItem[-1].ExecPic(pic);
}


void _fastcall FPicGet(CPicture& pic);			// na�ten� v��ezu obr�zku z plochy

// funkce
void _fastcall FPicFunc(CPicture& pic);			// funkce s n�vratem obr�zku

// prom�nn�
void _fastcall FPicEmpty(CPicture& pic);		// pr�zdn� obr�zek
void _fastcall FPicObj(CPicture& pic);			// glob�ln� obr�zek (Data = index)
void _fastcall FPicObjList(CPicture& pic);		// globaln� obr�zek v seznamu (Data = index, List = seznam)
void _fastcall FPicLoc(CPicture& pic);			// lok�ln� obr�zek (Data = index)
void _fastcall FPicLocList(CPicture& pic);		// lok�ln� obr�zek v seznamu (Data = index, List = seznam)

void _fastcall FGetFilePicture(CPicture& pic);	// na�ten� obr�zku ze souboru

void _fastcall FPicXFlip(CPicture& pic);		// horizont�ln� p�evr�cen� obr�zku
void _fastcall FPicYFlip(CPicture& pic);		// vertik�ln� p�evr�cen� obr�zku
void _fastcall FPicZoom(CPicture& pic);			// zm�na velikosti obr�zku
void _fastcall FPicLevel(CPicture& pic);		// zm�na jasu obr�zku
void _fastcall FPicRotate(CPicture& pic);		// oto�en� obr�zku
void _fastcall FGetWindowPic(CPicture& pic);	// na�ten� obr�zku okna
void _fastcall FMapPicture(CPicture& pic);		// konverze plochy na obr�zek
void _fastcall FIconPicture(CPicture& pic);		// konverze ikony na obr�zek
void _fastcall FPicMask(CPicture& pic);			// maskov�n� obr�zku
void _fastcall FPicXCol(CPicture& pic);			// z�m�na barev obr�zku
void _fastcall FPicSCol(CPicture& pic);			// n�hrada barvy obr�zku


void _fastcall FGetD3DTexture(CPicture& pic);	// zji�t�n� obr�zku textury Direct3D objektu
void _fastcall FGetD3DSceneTexture(CPicture& pic);	// zji�t�n� obr�zku textury Direct3D pozad� sc�ny
