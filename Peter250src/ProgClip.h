
/***************************************************************************\
*																			*
*								Obsluha clipboardu							*
*																			*
\***************************************************************************/

extern	DWORD	CF_PETPROGP;				// ID form�tu "Program"
extern	DWORD	CF_PETPROGG;				// ID form�tu "Glob�ln�"
extern	DWORD	CF_PETPROGL;				// ID form�tu "Lok�ln�"

// z�hlav� clipboardu
typedef struct CLIPHEAD_ {
	char				Ident[3];		// (3) identifikace = "PET"
	BYTE				Verze;			// (1) verze = 1
	WORD				Editor;			// (2) verze editoru v tis�cin�ch
	WORD				Param;			// (2) parametry = 0
	long				Data;			// (4) offset za��tku dat (= d�lka z�hlav�)
	long				Pocet;			// (4) po�et datov�ch blok� = 1
	PETINDEX			Clip;			// tabulka index�
} CLIPHEAD;

#define SIZEOFCLIPHEAD (3*sizeof(char) + sizeof(BYTE) + 2*sizeof(WORD) + 2*sizeof(long) + SIZEOFPETINDEX)

namespace ProgClip
{

// inicializace obsluhy clipboardu
	void StartInit();

// kopie do clipboardu (TRUE=operace OK)
	BOOL Copy(int bufID);

// navr�cen� z bufferu
	void Paste(int bufID);

// zru�en� bloku
	void Delete(int bufID);

// vyst�i�en� bloku
	void Cut(int bufID);

// aktualizace voleb bloku
	void UpdateClipboard();
}
