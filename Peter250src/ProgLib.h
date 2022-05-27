
/***************************************************************************\
*																			*
*									Knihovny								*
*																			*
\***************************************************************************/

namespace ProgLib
{

// prom�nn�
extern	BOOL	m_Loading;			// prob�h� obsluha na��t�n�
extern	int		m_Parent;			// index polo�ky k na�ten� obsahu
extern	int		m_Root;				// ROOT polo�ka v�tve
extern	int		m_Func;				// identifikace funkce
extern	BOOL	m_AltExt;			// p��znak alternativn� p��pony (JPG, MP3, RMI)
extern	CText	m_SubPath;			// podcesta (s "\" na konci nebo pr�zdn�)
extern	CText	m_Path;				// cesta do adres��e (s "\" na konci)
extern	CText	m_Aliases;			// jm�no INI souboru s ALIASES
extern	CText	m_Ext;				// p��pona soubor� 3 znaky
extern	CText	m_Ext2;				// alternativn� p��pona soubor� 3 znaky (pr�zdn� = nen�)
extern	CText	m_Name;				// pln� jm�no souboru
extern	CText	m_NameKey;			// jm�no souboru pou�it� jako kl�� INI (jm�no s p��ponou)
extern	CText	m_IconName;			// jm�no ikony k souboru (pr�zdn� = nen�)

// statick� inicializace obsluhy
	void StartInit();

// p��prava jm�na souboru (mus� b�t platn� potomek s platn�m rodi�em!)
	void _fastcall InitName(int index);

// p��prava cesty (index = posledn� adres��)
	void _fastcall InitPath(int index);

// na�ten� obsahu polo�ky v okn� t��d
	void Load(int index);

// zru�en� polo�ky z bufferu t��d (se zru�en�m souboru)
	void Delete(int index);

// kopie polo�ky z jin�ho okna (mus� b�t platn�!)
	int Copy(int parent, int pos, int bufID, int src);

// p�ejmenov�n� polo�ky v okn�
	int Move(int parent, int pos, int src);
};
