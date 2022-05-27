
/***************************************************************************\
*																			*
*								Obsluha clipboardu							*
*																			*
\***************************************************************************/

extern	DWORD	CF_PETPROGP;				// ID formátu "Program"
extern	DWORD	CF_PETPROGG;				// ID formátu "Globální"
extern	DWORD	CF_PETPROGL;				// ID formátu "Lokální"

// záhlaví clipboardu
typedef struct CLIPHEAD_ {
	char				Ident[3];		// (3) identifikace = "PET"
	BYTE				Verze;			// (1) verze = 1
	WORD				Editor;			// (2) verze editoru v tisícinách
	WORD				Param;			// (2) parametry = 0
	long				Data;			// (4) offset zaèátku dat (= délka záhlaví)
	long				Pocet;			// (4) poèet datových blokù = 1
	PETINDEX			Clip;			// tabulka indexù
} CLIPHEAD;

#define SIZEOFCLIPHEAD (3*sizeof(char) + sizeof(BYTE) + 2*sizeof(WORD) + 2*sizeof(long) + SIZEOFPETINDEX)

namespace ProgClip
{

// inicializace obsluhy clipboardu
	void StartInit();

// kopie do clipboardu (TRUE=operace OK)
	BOOL Copy(int bufID);

// navrácení z bufferu
	void Paste(int bufID);

// zrušení bloku
	void Delete(int bufID);

// vystøižení bloku
	void Cut(int bufID);

// aktualizace voleb bloku
	void UpdateClipboard();
}
