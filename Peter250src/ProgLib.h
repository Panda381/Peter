
/***************************************************************************\
*																			*
*									Knihovny								*
*																			*
\***************************************************************************/

namespace ProgLib
{

// promìnné
extern	BOOL	m_Loading;			// probíhá obsluha naèítání
extern	int		m_Parent;			// index položky k naètení obsahu
extern	int		m_Root;				// ROOT položka vìtve
extern	int		m_Func;				// identifikace funkce
extern	BOOL	m_AltExt;			// pøíznak alternativní pøípony (JPG, MP3, RMI)
extern	CText	m_SubPath;			// podcesta (s "\" na konci nebo prázdný)
extern	CText	m_Path;				// cesta do adresáøe (s "\" na konci)
extern	CText	m_Aliases;			// jméno INI souboru s ALIASES
extern	CText	m_Ext;				// pøípona souborù 3 znaky
extern	CText	m_Ext2;				// alternativní pøípona souborù 3 znaky (prázdné = není)
extern	CText	m_Name;				// plné jméno souboru
extern	CText	m_NameKey;			// jméno souboru použité jako klíè INI (jméno s pøíponou)
extern	CText	m_IconName;			// jméno ikony k souboru (prázdné = není)

// statická inicializace obsluhy
	void StartInit();

// pøíprava jména souboru (musí být platný potomek s platným rodièem!)
	void _fastcall InitName(int index);

// pøíprava cesty (index = poslední adresáø)
	void _fastcall InitPath(int index);

// naètení obsahu položky v oknì tøíd
	void Load(int index);

// zrušení položky z bufferu tøíd (se zrušením souboru)
	void Delete(int index);

// kopie položky z jiného okna (musí být platné!)
	int Copy(int parent, int pos, int bufID, int src);

// pøejmenování položky v oknì
	int Move(int parent, int pos, int src);
};
