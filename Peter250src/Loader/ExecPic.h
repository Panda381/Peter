
/***************************************************************************\
*																			*
*							Provádìní programu - obrázek					*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inline volání pøíkazu - obrázek

inline void FPicture(CPicture& pic)
{
	ExecItem++;
	ExecItem[-1].ExecPic(pic);
}


void _fastcall FPicGet(CPicture& pic);			// naètení výøezu obrázku z plochy

// funkce
void _fastcall FPicFunc(CPicture& pic);			// funkce s návratem obrázku

// promìnné
void _fastcall FPicEmpty(CPicture& pic);		// prázdný obrázek
void _fastcall FPicObj(CPicture& pic);			// globální obrázek (Data = index)
void _fastcall FPicObjList(CPicture& pic);		// globalní obrázek v seznamu (Data = index, List = seznam)
void _fastcall FPicLoc(CPicture& pic);			// lokální obrázek (Data = index)
void _fastcall FPicLocList(CPicture& pic);		// lokální obrázek v seznamu (Data = index, List = seznam)

void _fastcall FGetFilePicture(CPicture& pic);	// naètení obrázku ze souboru

void _fastcall FPicXFlip(CPicture& pic);		// horizontální pøevrácení obrázku
void _fastcall FPicYFlip(CPicture& pic);		// vertikální pøevrácení obrázku
void _fastcall FPicZoom(CPicture& pic);			// zmìna velikosti obrázku
void _fastcall FPicLevel(CPicture& pic);		// zmìna jasu obrázku
void _fastcall FPicRotate(CPicture& pic);		// otoèení obrázku
void _fastcall FGetWindowPic(CPicture& pic);	// naètení obrázku okna
void _fastcall FMapPicture(CPicture& pic);		// konverze plochy na obrázek
void _fastcall FIconPicture(CPicture& pic);		// konverze ikony na obrázek
void _fastcall FPicMask(CPicture& pic);			// maskování obrázku
void _fastcall FPicXCol(CPicture& pic);			// zámìna barev obrázku
void _fastcall FPicSCol(CPicture& pic);			// náhrada barvy obrázku


void _fastcall FGetD3DTexture(CPicture& pic);	// zjištìní obrázku textury Direct3D objektu
void _fastcall FGetD3DSceneTexture(CPicture& pic);	// zjištìní obrázku textury Direct3D pozadí scény
