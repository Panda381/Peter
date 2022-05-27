	
#include "Main.h"

/***************************************************************************\
*																			*
*								Obsluha souboru								*
*																			*
\***************************************************************************/

char DATAsize[] = "DATAsize";					// identifikace adresy a velikosti dat v Loaderu

IMAGE_SECTION_HEADER	PetProgHeader = {
	'.', 'p', 'e', 't', 'p', 'r', 'g', 0,		// jméno sekce
	0,											// virtuální velikost
	0,											// virtuální adresa
	0,											// velikost dat v souboru
	0,											// offset dat v souboru
	0,											// ... (offset relokací)
	0,											// ... (offset èísel øádkù)
	0,											// ... (poèet relokací)
	0,											// ... (poèet èísel øádkù)
	IMAGE_SCN_MEM_READ |						// vlastnosti
	IMAGE_SCN_MEM_DISCARDABLE |
	IMAGE_SCN_CNT_INITIALIZED_DATA
};

// velikost stránky zarovnávání pamìti
#ifndef _M_ALPHA
#define	PAGESIZE		0x1000			// velikost alokaèní stránky pro ostatní procesory (4096)
#define PAGEFILE		0x1000			// velikost stránky v souboru
#else
#define	PAGESIZE		0x2000			// velikost alokaèní stránky pro procesor Alpha (8192)
#define PAGEFILE		0x2000			// velikost stránky v souboru
#endif

/////////////////////////////////////////////////////////////////////////////
// buffer programu - konstruktor a destruktor

CPetProg::CPetProg()
{ 
	m_Data = NULL; 
	m_Num = 0; 
	m_Max = 0; 
};

CPetProg::~CPetProg() 
{ 
	MemFree(m_Data);
	m_Data = NULL; 
};

void CPetProg::DelAll() 
{ 
	MemFree(m_Data);
	m_Data = NULL; 
	m_Num = 0; 
	m_Max = 0; 
};


/////////////////////////////////////////////////////////////////////////////
// buffer programu - pøidání položky na konec bufferu (vrací index položky)
// pøi chybì pamìti vrací -1

int _fastcall CPetProg::Add(const PETPROG* item)
{
	int index = m_Num;
	m_Num = index + 1;
	if (index >= m_Max)
	{
		m_Max += 128;
		PETPROG* data = (PETPROG*)MemSize(m_Data, m_Max * sizeof(PETPROG));
		if (data == NULL)
		{
			m_Max -= 128;
			return -1;
		}
		m_Data = data;
	}
	MemCopy(m_Data + index, item, SIZEOFPETPROG);
	return index;
}


/////////////////////////////////////////////////////////////////////////////
// buffer textù importu - konstruktor a destruktor

CBufChar::CBufChar()
{
	m_Data = NULL; 
	m_Size = 0;
	m_Num = 0; 
	m_Max = 0; 
}

CBufChar::~CBufChar() 
{ 
	MemFree(m_Data);
	m_Data = NULL; 
};

void CBufChar::DelAll() 
{ 
	MemFree(m_Data);
	m_Data = NULL; 
	m_Size = 0;
	m_Num = 0; 
	m_Max = 0; 
};


/////////////////////////////////////////////////////////////////////////////
// buffer textù importu - pøidání položky na konec bufferu (vrací index)
// pøi chybì pamìti vrací -1

int _fastcall CBufChar::Add(const char* text, int len)
{
	ASSERT(len <= 255);
	if (len > 255) len = 255;
	while (m_Size + 1 + len > m_Max)
	{
		m_Max += 2048;
		char* data = (char*)MemSize(m_Data, m_Max * sizeof(char));
		if (data == NULL)
		{
			m_Max -= 2048;
			return -1;
		}
		m_Data = data;
	}
	*(BYTE*)(m_Data + m_Size) = (BYTE)len;
	m_Size++;
	MemCopy(m_Data + m_Size, text, len);
	m_Size += len;
	m_Num++;
	return (m_Num - 1);
}

int _fastcall CBufChar::Add(const char* text)
{
	int len = ::lstrlenA(text);
	return Add(text, len);
}

int _fastcall CBufChar::Add(const CText& text)
{
	int len = text.Length();

#ifdef _UNICODE
	char* text2 = (char*) MemGet(len + 1);
	if (text2 == NULL) return -1;
	::WideCharToMultiByte(CodePage, 0, text, -1, text2, len + 1, NULL, NULL);
	int inx = Add(text2, len);
	MemFree(text2);
	return inx;
#else
	return Add((const char*)text, len);
#endif
}


/////////////////////////////////////////////////////////////////////////////
// buffer textù - konstruktor a destruktor

CBufChar2::CBufChar2()
{
	m_Data = NULL; 
	m_Size = 0;
	m_Num = 0; 
	m_NumVal = 0;
	m_Max = 0; 
}

CBufChar2::~CBufChar2() 
{ 
	MemFree(m_Data);
	m_Data = NULL; 
};

void CBufChar2::DelAll() 
{ 
	MemFree(m_Data);
	m_Data = NULL; 
	m_Size = 0;
	m_Num = 0; 
	m_NumVal = 0;
	m_Max = 0; 
};


/////////////////////////////////////////////////////////////////////////////
// buffer textù - pøidání položky na konec bufferu (vrací index)
// pøi chybì pamìti vrací -1

int _fastcall CBufChar2::Add(const char* text, int len)
{
	while (m_Size + 4 + len > m_Max)
	{
		m_Max += 2048;
		char* data = (char*)MemSize(m_Data, m_Max * sizeof(char));
		if (data == NULL)
		{
			m_Max -= 2048;
			return -1;
		}
		m_Data = data;
	}
	*(long*)(m_Data + m_Size) = len;
	m_Size += 4;
	MemCopy(m_Data + m_Size, text, len);
	m_Size += len;
	if (len != 0) m_NumVal++;
	m_Num++;
	return (m_Num - 1);
}

int _fastcall CBufChar2::Add(const char* text)
{
	int len = ::lstrlenA(text);
	return Add(text, len);
}

int _fastcall CBufChar2::Add(const CText& text)
{
	int len = text.Length();

#ifdef _UNICODE
	char* text2 = (char*) MemGet(len + 1);
	if (text2 == NULL) return -1;
	::WideCharToMultiByte(CodePage, 0, text, -1, text2, len + 1, NULL, NULL);
	int inx = Add(text2, len);
	MemFree(text2);
	return inx;
#else
	return Add((const char*)text, len);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// obsluha souborù

namespace ProgFile
{

/////////////////////////////////////////////////////////////////////////////
// lokální a globální promìnné

CText	FileName;				// jméno souboru
int		FileOffset;				// offset záhlaví v souboru
int		FileVer;				// verze souboru
HANDLE	hFile;					// handle souboru
PETHEAD* FileHead = NULL;		// buffer s naèteným záhlavím souboru
BOOL	Release = FALSE;		// uložit distribuèní verzi programu
int		MainFnc = 0;			// index hlavní globální funkce

// údaje pro uložení
int		SaveFileSize;			// offset velikosti dat v souboru
int		SaveVirtSize;			// offset virtuální velikosti v souboru
int		SaveVirtSize2;			// offset virtuální velikosti v souboru 2 (pro program)
int		SaveDataOff;			// offset zaèátku dat v souboru
int		SaveDataVirt;			// virtuální adresa dat
int		SaveImageSize;			// offset velikosti obrazu v souboru

// chyba pøi naèítání
BOOL	LoadError = FALSE;		// pøíznak chyby pøi naèítání (nekompatibilní soubor)

// mapování interních funkcí na okno struktur a tøíd
int*	ImportBlok = NULL;		// tabulka blokù pro import funkcí
int*	ImportIndex = NULL;		// tabulka indexù pro import funkcí

// pøemapování blokù a indexù
int		RemapBlokNum = 0;		// poèet pøemapovávaných bufferù
int*	RemapBlok = NULL;		// tabulka pro pøemapování blokù (-1=není blok)
int*	RemapIndexNum = NULL;	// poèty pøemapovávaných indexù
int**	RemapIndex = NULL;		// tabulky pro pøemapování indexù

int		IconInx;				// index bufferu s ikonami (-1 = není)
int		TextInx;				// index bufferu s texty (-1 = není)

// buffer s naètenými daty
BYTE*	DatBuf = NULL;			// datový buffer
BYTE*	DatEnd = NULL;			// adresa konce dat v bufferu
int		DatNum = 0;				// délka dat v bufferu
int		DatVerze = 0;			// verze dat (napø. 1 = komprimace, pro texty kódová stránka)

// ukládání souboru
// Bìhem obsluhy ukládání se nesmí dekomprimovat ikony ani obrázky!
BOOL	Saving = FALSE;			// probíhá ukládání nebo naèítání
BOOL	SaveOK = TRUE;			// pøíznak ukládání OK

CBufIndex	SaveRemapInt;		// buffer mapování interních funkcí
CBufChar	SaveInt;			// buffer textù interních funkcí

CBufIndex	SaveRemapObj;		// buffer mapování globálních objektù
CPetProg	SaveObj;			// buffer globálních objektù

CBufIndex	SaveRemapLoc;		// buffer mapování lokálních objektù
CPetProg	SaveLoc;			// buffer lokálních objektù

CBufIndex	SaveRemapEdi;		// buffer mapování programu
CPetProg	SaveEdi;			// buffer programu

CBufIndex	SaveRemapReal;		// buffer mapování èísel
CBufReal	SaveReal;			// buffer èísel

CBufIndex	SaveRemapText;		// buffer mapování textù
CBufChar2	SaveText[JAZYKNUM];	// buffer textù

CBufIndex	SaveRemapBool;		// buffer mapování logických promìnných
CBufBool	SaveBool;			// buffer logických promìnných

CBufIndex	SaveRemapIcon;		// buffer mapování ikon
CBufIcon	SaveIcon;			// buffer ikon
int			SaveIconSize;		// velikost bloku ikon

CBufIndex	SaveRemapMap;		// buffer mapování ploch
CBufMap		SaveMap;			// buffer ploch
int			SaveMapSize;		// velikost bloku ploch

CBufIndex	SaveRemapSprite;	// buffer mapování sprajtù
CBufSprite	SaveSprite;			// buffer sprajtù
int			SaveSpriteSize;		// velikost bloku sprajtù

CBufIndex	SaveRemapSound;		// buffer mapování zvukù
CBufSound	SaveSound;			// buffer zvukù
int			SaveSoundSize;		// velikost bloku zvukù

CBufIndex	SaveRemapPicture;	// buffer mapování obrázkù
CBufPic		SavePicture;		// buffer obrázkù
int			SavePictureSize;	// velikost bloku obrázkù

CBufIndex	SaveRemapMusic;		// buffer mapování hudby
CBufMusic	SaveMusic;			// buffer hudby
int			SaveMusicSize;		// velikost bloku hudby



/***************************************************************************\
*																			*
*									uložení									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// záhlaví pro uložení (poèet položek NUMOFINDEX), používá se i pro naèítání!

const PETHEAD SaveHead = {
	'P','E','T',									// identifikace
	1,												// verze souboru
	VerzeCom + VerzeRel*10 + VerzeMin*100 + VerzeMaj*1000, // verze editoru
	0,												// parametry
	SIZEOFPETHEAD + NUMOFINDEX*SIZEOFPETINDEX,		// offset zaèátku dat
	NUMOFINDEX,										// poèet datových blokù
	0,0,'I','M','P','O','R','T',' ',' ',0,0,0,0,	//  0: blok IMPORT
	0,0,'C','L','A','S','S',' ',' ',' ',0,0,0,0,	//  1: blok CLASS
	0,0,'G','L','O','B','A','L',' ',' ',0,0,0,0,	//  2: blok GLOBAL
	0,0,'L','O','C','A','L',' ',' ',' ',0,0,0,0,	//  3: blok LOCAL
	0,0,'P','R','O','G','R','A','M',' ',0,0,0,0,	//  4: blok PROGRAM
	0,0,'S','T','R','U','C',' ',' ',' ',0,0,0,0,	//  5: blok STRUC
	0,0,'R','E','A','L',' ',' ',' ',' ',0,0,0,0,	//  6: blok REAL
	0,0,'T','E','X','T',' ',' ',' ',' ',1,0,0,0,	//  7: blok TEXT
	0,0,'B','O','O','L',' ',' ',' ',' ',0,0,0,0,	//  8: blok BOOL
	0,0,'P','I','C','T','U','R','E',' ',1,0,0,0,	//  9: blok PICTURE
	0,0,'M','A','P',' ',' ',' ',' ',' ',0,0,0,0,	// 10: blok MAP
	0,0,'B','A','C','K','G','R','N','D',1,0,0,0,	// 11: blok BACKGRND
	0,0,'S','P','R','I','T','E',' ',' ',1,0,0,0,	// 12: blok SPRITE
	0,0,'S','O','U','N','D',' ',' ',' ',0,0,0,0,	// 13: blok SOUND
	0,0,'M','U','S','I','C',' ',' ',' ',0,0,0,0,	// 14: blok MUSIC
	0,0,'P','A','L','E','T','T','E',' ',0,0,0,0,	// 15: blok PALETTE
};

/////////////////////////////////////////////////////////////////////////////
// zápis dat do výstupního souboru

void SaveWriteFile(void* buf, int len)
{
	if (SaveOK)
	{
		if (len < 0) len = 0;
		DWORD writen = 0;
		SaveOK = (::WriteFile(hFile, buf, len,  &writen, NULL) && (writen == (DWORD)len));
	}
}


/////////////////////////////////////////////////////////////////////////////
// zápis zavadìèe programu

void SaveLoader(BOOL mini)
{
// pøi chybì konec
	if (!SaveOK) return;

// detekce, zda lze použít minimalizovanou verzi zavadìèe (bez ukládání a naèítání obrázkù, zvukù, hudby, ikon, bez sítì a portù a bez Direct3D)
	int loadid = IDN_LOADERM;

	if (!mini)
	{
		loadid = IDN_LOADERS;

		int inx = BufEdi.First();
		while (BufEdi.IsValid(inx))
		{
			PROGITEM* item = &(BufEdi[inx]);

			switch (item->Func)
			{
		// DLL
			case IDF_DLL_GROUP:
			case IDF_DLL_LOAD:
			case IDF_DLL_FREE:
			case IDF_DLL_IMPORT:
			case IDF_DLL_IMPORT_LIB:
			case IDF_DLL_IMPORT_NAME:
			case IDF_DLL_IMPORT_PAR:
			case IDF_DLL_EXEC:
			case IDF_MEMORY_NEW:
			case IDF_MEMORY_FREE:
			case IDF_MEMORY_RESIZE:
			case IDF_MEMORY_RESIZE_ADR:
			case IDF_MEMORY_RESIZE_NEW:
			case IDF_MEMORY_READ:
			case IDF_MEMORY_WRITE:
			case IDF_MEMORY_COPY:
			case IDF_MEMORY_COMPARE:
			case IDF_MEMORY_BYTES:
			case IDF_MEMORY_BYTEU:
			case IDF_MEMORY_WORDS:
			case IDF_MEMORY_WORDU:
			case IDF_MEMORY_3BYTES:
			case IDF_MEMORY_3BYTEU:
			case IDF_MEMORY_DWORDS:
			case IDF_MEMORY_DWORDU:
			case IDF_MEMORY_FLOAT:
			case IDF_MEMORY_DOUBLE:
			case IDF_MEMORY_BOOL:
			case IDF_MEMORY_TEXT0:
			case IDF_MEMORY_TEXT0P:
			case IDF_MEMORY_TEXTN:
			case IDF_MEMORY_TEXTN_LEN:
			case IDF_MEMORY_LOAD:
			case IDF_MEMORY_SAVE:
			case IDF_WINDOW_HANDLE:
			case IDF_MEMORY_3D:
			case IDF_MEMORY_3D_VN:
			case IDF_MEMORY_3D_VERT:
			case IDF_MEMORY_3D_NORM:
			case IDF_MEMORY_3D_TEXT:
			case IDF_MEMORY_3D_COL:
			case IDF_MEMORY_3D_FN:
			case IDF_MEMORY_3D_FACE:
			case IDF_DIRECT3D_MEMORY:
			case IDF_DIRECT3D_CHILD:

		// konfigurace
			case IDF_INI_FILE:
			case IDF_INI_SECTION:
			case IDF_INI_VALUE:
			case IDF_INI_STRING:
			case IDF_INI_NUM:
			case IDF_REG_KEY:
			case IDF_REG_SUBKEY:
			case IDF_REG_VALUE:
			case IDF_REG_STRING:
			case IDF_REG_NUM:

		// soubory
			case IDF_FILE_PICTURE:
			case IDF_FILE_SOUND:
			case IDF_FILE_MUSIC:
			case IDF_FILE_ICON:

		// sériové porty
			case IDF_COM_GROUP:
			case IDF_COM_OPEN:
			case IDF_COM_CLOSE:
			case IDF_COM_ID:
			case IDF_COM_BAUD:
			case IDF_COM_STOP:
			case IDF_COM_BITS:
			case IDF_COM_PARIT:
			case IDF_COM_IN:
			case IDF_COM_SEND:
			case IDF_COM_RECEIVE:
			case IDF_COM_NUM:
			case IDF_COM_TEXT:

		// DirectPlay
			case IDF_CONNECT_LIST:
			case IDF_CONNECT_ACT:
			case IDF_GAME_IP:
			case IDF_GAME_LIST:
			case IDF_GAME_ACT:
			case IDF_GAME_HOST:
			case IDF_PLAYER_MAX:
			case IDF_GAME_N1:
			case IDF_GAME_N2:
			case IDF_GAME_N3:
			case IDF_GAME_N4:
			case IDF_GAME_N5:
			case IDF_GAME_N6:
			case IDF_GAME_N7:
			case IDF_GAME_N8:
			case IDF_GAME_L1:
			case IDF_GAME_L2:
			case IDF_GAME_L3:
			case IDF_GAME_L4:
			case IDF_GAME_L5:
			case IDF_GAME_L6:
			case IDF_GAME_L7:
			case IDF_GAME_L8:
			case IDF_PLAYER_LIST:
			case IDF_PLAYER_ACT:
			case IDF_PLAYER_INX:
			case IDF_GAME_PACKET:
			case IDF_GAME_DATAN:
			case IDF_GAME_DATAB:
			case IDF_GAME_DATAT:
			case IDF_GAME_DATAL:

		// UDP
			case IDF_UDP_SEND_IP:
			case IDF_UDP_SEND_PORT:
			case IDF_UDP_REC_SIZE:
			case IDF_UDP_REC_IP:
			case IDF_UDP_REC_PORT:
			case IDF_UDP_MEM:
			case IDF_UDP_TEXT:

		// DDE
			case IDF_DDE_APP:
			case IDF_DDE_TOPIC:
			case IDF_DDE_ITEM:
			case IDF_DDE_SERVER:
			case IDF_DDE_TYPE:
			case IDF_DDE_NUMBER:
			case IDF_DDE_BOOL:
			case IDF_DDE_TEXT:
			case IDF_DDE_ICON:
			case IDF_DDE_PICTURE:
			case IDF_DDE_SOUND:
			case IDF_DDE_MUSIC:
			case IDF_DDE_SPRITE:
			case IDF_DDE_EXECUTE:

		// mixer
			case IDF_MIX_DEVICE:
			case IDF_MIX_LINE_DST:
			case IDF_MIX_LINE_SRC:
			case IDF_MIX_LINE_TYPE:
			case IDF_MIX_CONTROL:
			case IDF_MIX_CONTROL_TYPE:
			case IDF_MIX_CONTROL_VAL:
			case IDF_MIX_CONTROL_LIST:
			case IDF_MIX_CHANNEL:

		// Direct3D
			case IDF_DIRECT3D_VIEW:
			case IDF_DIRECT3D_OBJECT_ID:
			case IDF_DIRECT3D_ELAPSED:
			case IDF_DIRECT3D_INTERFACE:
			case IDF_DIRECT3D_DRIVER:
			case IDF_DIRECT3D_FREE:
			case IDF_DIRECT3D_SPEED:
			case IDF_DIRECT3D_WIREFRAME:
			case IDF_DIRECT3D_LIGHTON:
			case IDF_DIRECT3D_SHADES:
			case IDF_DIRECT3D_MAGFILTER:
			case IDF_DIRECT3D_MINFILTER:
			case IDF_DIRECT3D_MIPFILTER:
			case IDF_DIRECT3D_OWIREFRAME:
			case IDF_DIRECT3D_OLIGHTON:
			case IDF_DIRECT3D_OSHADES:
			case IDF_DIRECT3D_OCULLING:
			case IDF_DIRECT3D_SCENE_COLOR:
			case IDF_DIRECT3D_SCENE_TEXTURE:
			case IDF_DIRECT3D_FOG_COLOR:
			case IDF_DIRECT3D_FOG_MODE:
			case IDF_DIRECT3D_FOG_START:
			case IDF_DIRECT3D_FOG_END:
			case IDF_DIRECT3D_FOG_DENSITY:
			case IDF_DIRECT3D_VIEW_FRONT:
			case IDF_DIRECT3D_VIEW_BACK:
			case IDF_DIRECT3D_PROJECTION:
			case IDF_DIRECT3D_RENDER:
			case IDF_DIRECT3D_EMPTY:
			case IDF_DIRECT3D_WALL:
			case IDF_DIRECT3D_CIRCLE:
			case IDF_DIRECT3D_TRIANGLE:
			case IDF_DIRECT3D_CUBE:
			case IDF_DIRECT3D_SPHERE:
			case IDF_DIRECT3D_HEMISPHERE:
			case IDF_DIRECT3D_CYLINDER:
			case IDF_DIRECT3D_CONE:
			case IDF_DIRECT3D_CONE2:
			case IDF_DIRECT3D_TORUS:
			case IDF_DIRECT3D_ROTARY:
			case IDF_DIRECT3D_2DOBJECT:
			case IDF_DIRECT3D_2DSTATIC:
			case IDF_DIRECT3D_PICTURE:
			case IDF_DIRECT3D_TERRAIN:
			case IDF_DIRECT3D_TERRAIN_MAP:
			case IDF_DIRECT3D_TEXT:
			case IDF_DIRECT3D_FILE:
			case IDF_DIRECT3D_VISIBLE:
			case IDF_DIRECT3D_OBJECT_DELETE:
			case IDF_DIRECT3D_CLONE:
			case IDF_DIRECT3D_SPLITS:
			case IDF_DIRECT3D_UPPER:
			case IDF_DIRECT3D_LOWER:
			case IDF_DIRECT3D_LINK:
			case IDF_DIRECT3D_INVERT:
			case IDF_DIRECT3D_NORM:
			case IDF_DIRECT3D_NORMALS:
			case IDF_DIRECT3D_ZGROUP:
			case IDF_DIRECT3D_LEVEL:
			case IDF_DIRECT3D_SCALEX:
			case IDF_DIRECT3D_SCALEY:
			case IDF_DIRECT3D_SCALEZ:
			case IDF_DIRECT3D_ROTATEX:
			case IDF_DIRECT3D_ROTATEY:
			case IDF_DIRECT3D_ROTATEZ:
			case IDF_DIRECT3D_TRANSX:
			case IDF_DIRECT3D_TRANSY:
			case IDF_DIRECT3D_TRANSZ:
			case IDF_DIRECT3D_ORDER:
			case IDF_DIRECT3D_COLOR:
			case IDF_DIRECT3D_AMBIREF:
			case IDF_DIRECT3D_EMISSIVE:
			case IDF_DIRECT3D_POWER:
			case IDF_DIRECT3D_SPECULAR:
			case IDF_DIRECT3D_ALPHAREF:
			case IDF_DIRECT3D_TRANSPARENT:
			case IDF_DIRECT3D_STAGE:
			case IDF_DIRECT3D_TEXTURE:
			case IDF_DIRECT3D_TEXTURE_FILE:
			case IDF_DIRECT3D_SMOOTH:
			case IDF_DIRECT3D_OMAGFILTER:
			case IDF_DIRECT3D_OMINFILTER:
			case IDF_DIRECT3D_OMIPFILTER:
			case IDF_DIRECT3D_MIPMAPS:
			case IDF_DIRECT3D_BIAS:
			case IDF_DIRECT3D_TEXTMAP:
			case IDF_DIRECT3D_ADDRESSU:
			case IDF_DIRECT3D_ADDRESSV:
			case IDF_DIRECT3D_COLOROP:
			case IDF_DIRECT3D_COLORARG1:
			case IDF_DIRECT3D_COLORARG2:
			case IDF_DIRECT3D_ALPHAOP:
			case IDF_DIRECT3D_ALPHAARG1:
			case IDF_DIRECT3D_ALPHAARG2:
			case IDF_DIRECT3D_TFACTOR:
			case IDF_DIRECT3D_LIGHT:
			case IDF_DIRECT3D_LIGHT_COLOR:
			case IDF_DIRECT3D_LIGHT_TYPE:
			case IDF_DIRECT3D_LIGHT_UMBRA:
			case IDF_DIRECT3D_LIGHT_PENUMBRA:
			case IDF_DIRECT3D_LIGHT_RANGE:
			case IDF_DIRECT3D_LIGHT_CATTEN:
			case IDF_DIRECT3D_LIGHT_LATTEN:
			case IDF_DIRECT3D_LIGHT_QATTEN:
			case IDF_DIRECT3D_AMBIENT:
			case IDF_DIRECT3D_ZTEST:
			case IDF_SHADOW_RANGE:
			case IDF_DIRECT3D_LOD_OBJECT:
			case IDF_DIRECT3D_LOD_DISTANCE:
				loadid = IDN_LOADER;
			}

			inx = BufEdi.NextItem(inx);
			if (inx == BufEdi.First()) break;
		}
	}

// naètení zavadìèe
	CResource loader;
	if (!loader.OpenCopy(loadid, _T("LOADER")))
	{
		SaveOK = FALSE;
		return;
	}

// adresa ikony
	BITMAPINFO* bmp = loader.NTIcon();
	if (bmp == NULL)
	{
CHYBA:
		loader.CloseCopy();
		SaveOK = FALSE;
		return;
	}

// uložení palet (barva 0 bude èerná)
	MemCopy(bmp->bmiColors, StdBitmapInfo->bmiColors, 4*256);
	*(DWORD*)bmp->bmiColors = 0;

// pøenesení dat ikony
	CIcon ico = Icon.Get(BufObj.GetIcon(MainFnc));
	if (!ico.CopyWrite() || !ico.DeComp()) goto CHYBA;
	MemCopy(bmp->bmiColors + 256, ico.DataData(), ICONSIZE);

// pøíprava ke korekci pozadí ikony
	BYTE stradac;
	BYTE* src = (BYTE*)(bmp->bmiColors + 256);
	BYTE* dst = src + ICONSIZE;

// korekce pozadí ikony
	int i;
	for (i = ICONSIZE/8; i > 0; i--)
	{
		stradac = 0;

		if (src[0] == BackCol)	stradac  = 0x80;
		if (src[1] == BackCol)	stradac |= 0x40;
		if (src[2] == BackCol)	stradac |= 0x20;
		if (src[3] == BackCol)	stradac |= 0x10;
		if (src[4] == BackCol)	stradac |= 0x08;
		if (src[5] == BackCol)	stradac |= 0x04;
		if (src[6] == BackCol)	stradac |= 0x02;
		if (src[7] == BackCol)	stradac |= 0x01;

		*dst = stradac;
		src += 8;
		dst++;
	}

// adresa záhlaví NT
	IMAGE_NT_HEADERS* hdr = loader.NTHeader();
	if (hdr == NULL) goto CHYBA;

// pøíprava offsetu zaèátku dat v souboru
	SaveDataOff = (loader.Size() + (PAGEFILE-1)) & ~(PAGEFILE-1);

// pøíprava virtuální adresy dat, úschova offsetu dat
	SaveDataVirt = (hdr->OptionalHeader.SizeOfImage + PAGESIZE-1) & ~(PAGESIZE-1);
	SaveImageSize = (BYTE*)&hdr->OptionalHeader.SizeOfImage - loader.Adr();

// adresa vkládané sekce
	IMAGE_SECTION_HEADER* sec = (IMAGE_SECTION_HEADER*)((BYTE*)(&hdr->OptionalHeader) 
		+ hdr->FileHeader.SizeOfOptionalHeader + hdr->FileHeader.NumberOfSections 
		* sizeof(IMAGE_SECTION_HEADER));

// zvýšení èítaèe sekcí
	hdr->FileHeader.NumberOfSections++;

// nastavení sekce
	MemCopy(sec, &PetProgHeader, IMAGE_SIZEOF_SECTION_HEADER);
	if (Release)
	{
		sec->Name[1] = _T('r');
		sec->Name[2] = _T('t');
		sec->Name[3] = _T('e');
		sec->Name[4] = _T('x');
		sec->Name[5] = _T('t');
		sec->Name[6] = _T('s');
	}

// nastavení offsetu zaèátku dat v souboru
	sec->PointerToRawData = SaveDataOff;
	sec->VirtualAddress = SaveDataVirt;
	SaveFileSize = (BYTE*)&sec->SizeOfRawData - loader.Adr();
	SaveVirtSize = (BYTE*)&sec->Misc.VirtualSize - loader.Adr();

// nalezení datové sekce
	sec = loader.NTSection(hdr, ".data");
	if (sec == NULL) goto CHYBA;

// nalezení identifikátoru adresy dat
	dst = loader.Adr() + sec->PointerToRawData;
	i = sec->SizeOfRawData;
	if (loader.IsNotValid(dst, i)) goto CHYBA;
	i -= 7;
	DWORD d = *(DWORD*)DATAsize;

	for (; i > 0; i--)
	{
		if (*(DWORD*)dst == d)
		{
			if (MemCompare(dst, DATAsize, 8))
			{
				break;
			}
		}
		dst++;
	}

// nastavení virtuální adresy
	if (!MemCompare(dst, DATAsize, 8)) goto CHYBA;
	*(DWORD*)dst = SaveDataVirt;
	SaveVirtSize2 = dst - loader.Adr() + 4;

// nastavení konzolového módu
	if (!mini)
	{
		ASSERT(hdr->OptionalHeader.Subsystem == IMAGE_SUBSYSTEM_WINDOWS_GUI);
		int index = BufEdi.First();
		if (BufEdi.IsValid(index))
		{
			index = BufEdi[index].Child;
			if (BufEdi.IsValid(index) && 
				(BufEdi[index].Func == IDF_CONSOLE_ON) &&
				((BufEdi[index].Param & (PR_OFF | PR_OFF_DEP)) == 0))
			{
				hdr->OptionalHeader.Subsystem = IMAGE_SUBSYSTEM_WINDOWS_CUI;
			}
		}
	}

// zápis zavadìèe
	SaveWriteFile(loader.Adr(), loader.Size());

// zarovnání délky zavadìèe
	i = SaveDataOff - loader.Size();
	if (i > 0)
	{
		dst = (BYTE*)MemGet(i);
		if (dst == NULL) goto CHYBA;
		MemFill(dst, i, 0);
		SaveWriteFile(dst, i);
		MemFree(dst);
	}

// zrušení bufferu zavadìèe
	loader.CloseCopy();
}


/////////////////////////////////////////////////////////////////////////////
// pøidání ikony k seznamu pro uložení (pøi chybì pamìti vrací FALSE)
// (pro neplatnou ikonu dosadí prázdnou ikonu)

bool SaveAddIcon(int icon)
{
// kontrola indexu
	if ((DWORD)icon >= (DWORD)Icon.Max()) 
	{
		SaveOK = FALSE;
		return false;
	}

// test, zda ikona ještì není uložena
	if (SaveRemapIcon[icon] < 0)
	{

// pøidání ikony k seznamu
		int i = SaveIcon.Add(Icon.Get(icon));
		if (i < 0)
		{
			SaveOK = FALSE;
			return false;
		}
		SaveRemapIcon[icon] = i;

// komprese ikony
		int n = SaveIcon[i].Comp();
		if (n < 0)
		{
			SaveOK = FALSE;
			return false;
		}

// zvýšení èítaèe velikosti dat ikon
		SaveIconSize += n + 4;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// pøidání plochy k seznamu pro uložení (pøi chybì pamìti vrací FALSE)
// (pro neplatnou plochu dosadí prázdnou plochu)

bool SaveAddMap(int map)
{
// kontrola indexu
	if ((DWORD)map >= (DWORD)Map.Max()) 
	{
		SaveOK = FALSE;
		return false;
	}

// test, zda plocha ještì není uložena
	if (SaveRemapMap[map] < 0)
	{

// pøidání plochy k seznamu
		int i = SaveMap.Add(Map.Get(map));
		if (i < 0)
		{
			SaveOK = FALSE;
			return false;
		}
		SaveRemapMap[map] = i;

// zvýšení èítaèe velikosti dat
		int ii = SaveMap[i].Width() * SaveMap[i].Height();
		SaveMapSize += ii * SIZEOFMAPITEM + SIZEOFMAPPROG;

// pøíprava k uložení ikon (budou se modifikovat indexy ikon)
		if (!SaveMap[i].CopyWrite())
		{
			SaveOK = FALSE;
			return false;
		}
		MAPITEM* mapitem = SaveMap[i].DataData();

// cyklus pøes všechny ikony
		for (; ii > 0; ii--)
		{

// ikona k uložení
			long icon = mapitem->Icon;
			ASSERT(Icon.IsValid(icon));

// uložení ikony
			if (!SaveAddIcon(icon)) return false;
			mapitem->Icon = SaveRemapIcon[icon];

// zvýšení ukazatele ikon
			mapitem++;
		}
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// pøidání textu k seznamu pro uložení (pøi chybì pamìti vrací FALSE)

bool SaveAddText(int txt)
{
// kontrola indexu
	if ((DWORD)txt >= (DWORD)Text.Max()) 
	{
		SaveOK = FALSE;
		return false;
	}

// test, zda text ještì není uložen
	if (SaveRemapText[txt] < 0)
	{

// cyklus pøes všechny jazyky
		for (int jaz = 0; jaz < JAZYKNUM; jaz++)
		{

// pøidání textu k seznamu
			int j = SaveText[jaz].Size();

			int i = SaveText[jaz].Add(Text.Get(txt).MultiText(jaz));

			if (i < 0)
			{
				SaveOK = FALSE;
				return false;
			}
			SaveRemapText[txt] = i;

// zakódování textu pro distribuèní verzi
			if (Release)
			{
				for (j = j + 4; j < SaveText[jaz].Size(); j++)
				{
					SaveText[jaz].Data()[j] = (char)(123 - SaveText[jaz].Data()[j]);
				}
			}
		}
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// pøidání sprajtu k seznamu pro uložení (pøi chybì pamìti vrací FALSE)
// (pro neplatný sprajt dosadí prázdný sprajt)

bool SaveAddSprite(int spr)
{
// kontrola indexu
	if ((DWORD)spr >= (DWORD)Sprite.Max()) 
	{
		SaveOK = FALSE;
		return false;
	}

// test, zda sprajt ještì není uložen
	if (SaveRemapSprite[spr] < 0)
	{

// pøidání sprajtu k seznamu
		int i = SaveSprite.Add(Sprite.Get(spr));
		if (i < 0)
		{
			SaveOK = FALSE;
			return false;
		}
		SaveRemapSprite[spr] = i;

// pøíprava k uložení obrázkù sprajtu
		int n = SaveSprite[i].Faze() * SaveSprite[i].Smer();
		SaveSpriteSize += SIZEOFSPRITEPROG;

// cyklus pøes všechny obrázky sprajtu
		for (int ii = 0; ii < n; ii++)
		{

// komprese obrázku sprajtu
			int j = SaveSprite[i][ii].Comp();
			if (j < 0)
			{
				SaveOK = FALSE;
				return false;
			}

// zvýšení èítaèe velikosti
			SaveSpriteSize += j + 4;
		}
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// vygenerování programu

void SavePrg(CPetProg* prg, CBufProg* buf, CBufIndex* remap)
{
// pøi chybì konec
	if (!SaveOK) return;

// lokální promìnné
	int			index;				// index zdrojové položky
	PROGITEM*	src;				// adresa zdrojové položky
	PETPROG		dst;				// cílová položka
	int			inx;				// index datové položky

// výchozí index položky
	index = buf->First();

// cyklus pøes všechny platné položky
	while (buf->IsValid(index))
	{

// adresa zdrojové položky
		src = &buf->At(index);

// základní inicializace ukazatelù
		dst.Param = 0;
		dst.RefBlok = src->RefBlok;
		dst.RefIndex = src->RefIndex;
		dst.DatBlok = src->DatBlok;
		dst.DatIndex = src->DatIndex;
		dst.Icon = src->Icon;
		dst.Name = src->Name;
		dst.Func = src->Func - IDF;
		ASSERT(dst.Func < FncNum);

// parametry
		if (src->Child >= 0)			dst.Param |= PETPROG_CHILDS;
		if (src->Next >= 0)				dst.Param |= PETPROG_NEXT;
		if (src->Param & PR_EXP)		dst.Param |= PETPROG_EXP;
		if (src->Param & PR_LOCK)		dst.Param |= PETPROG_LOCK;
		if (src->Param & PR_OFF)		dst.Param |= PETPROG_OFF;
		if (src->Param & PR_NOMOVE)		dst.Param |= PETPROG_NOMOVE;
		if (src->Param & PR_INTERN)		dst.Param |= PETPROG_INTERN;
		if (src->Param & PR_OFF_DEP)	dst.Param |= PETPROG_OFF_DEP;

// zachovat poøadí pøemapování bufferù!

// pøemapování reference do bufferu globálních objektù
		if (dst.RefBlok == BufObjID)
		{
			dst.RefIndex = SaveRemapObj[dst.RefIndex];
		}

// pøemapování reference do bufferu lokálních objektù
		if (dst.RefBlok == BufLocID)
		{
			dst.RefIndex = SaveRemapLoc[dst.RefIndex];
		}

// pøi odkazu reference na buffer struktur zmìna na interní prvek
		if (dst.RefBlok == BufStrID)
		{
			dst.RefBlok = BufStr[dst.RefIndex].RefBlok;
			dst.RefIndex = BufStr[dst.RefIndex].RefIndex;
		}

// pøi odkazu reference na buffer tøíd zmìna na interní prvek
		if (dst.RefBlok == BufClsID)
		{
			dst.RefBlok = BufCls[dst.RefIndex].RefBlok;
			dst.RefIndex = BufCls[dst.RefIndex].RefIndex;
		}

// pøi odkazu reference na interní prvek doplnìní textu importu funkce
		if (dst.RefBlok == BufIntID)
		{
			int i = SaveRemapInt[dst.RefIndex];
			if (i < 0)
			{
				i = SaveInt.Add(InitTabFunc[dst.RefIndex].Import);
				if (i < 0)
				{
					SaveOK = FALSE;
					return;
				}
				SaveRemapInt[dst.RefIndex] = i;
			}
			dst.RefIndex = i;
		}

// pøi odkazu ikony na ikonu doplnìní ikony
		if (dst.Icon >= 0)
		{
			if (!Release ||
				(src->Func == IDF_COLOR) ||
				(src->Func == IDF_ICON) ||
				(src->Func == IDF_NONE) ||
				((buf->BufID() == BufObjID) && (index == MainFnc)))
			{
				if (!SaveAddIcon(dst.Icon)) return;
				dst.Icon = SaveRemapIcon[dst.Icon];
			}
			else
			{
				dst.Icon = -1;
			}
		}

// pøi odkazu jména na text doplnìní textu
		if (dst.Name >= 0)
		{
			if (!Release ||
				(src->Func == IDF_TEXT_CONST) ||
				(src->Func == IDF_NUM) ||
				(src->Func == IDF_LIST_SIZE) ||
				((buf->BufID() == BufObjID) && (index == MainFnc)))
			{
				if (!SaveAddText(dst.Name)) return;
				dst.Name = SaveRemapText[dst.Name];
			}
			else
			{
				dst.Name = -1;
			}
		}

// odkaz na data
		inx = dst.DatIndex;
		int inx0 = inx;

		switch (dst.DatBlok)
		{
		case BufNumID:
			inx = SaveRemapReal[inx0];
			if (inx < 0)
			{
				inx = SaveReal.Add(Real.Get(inx0));
				if (inx < 0)
				{
					SaveOK = FALSE;
					return;
				}
				SaveRemapReal[inx0] = inx;
			}
			break;

		case BufTxtID:
			if (!SaveAddText(inx0)) return;
			inx = SaveRemapText[inx0];
			break;

		case BufLogID:
			inx = SaveRemapBool[inx0];
			if (inx < 0)
			{
				inx = SaveBool.Add(Bool.Get(inx0));
				if (inx < 0)
				{
					SaveOK = FALSE;
					return;
				}
				SaveRemapBool[inx0] = inx;
			}
			break;

		case BufIcoID:
			if (!SaveAddIcon(inx0)) return;
			inx = SaveRemapIcon[inx0];
			break;

		case BufMapID:
			if (!SaveAddMap(inx0)) return;
			inx = SaveRemapMap[inx0];
			break;

		case BufPicID:
			inx = SaveRemapPicture[inx0];
			if (inx < 0)
			{
				inx = SavePicture.Add(Picture.Get(inx0));
				if (inx < 0)
				{
					SaveOK = FALSE;
					return;
				}
				SaveRemapPicture[inx0] = inx;

				int j = SavePicture[inx].Comp();
				if (j < 0)
				{
					SaveOK = FALSE;
					return;
				}
				SavePictureSize += j + 4 + SIZEOFPICPROG;
			}
			break;

		case BufSprID:
			if (!SaveAddSprite(inx0)) return;
			inx = SaveRemapSprite[inx0];
			break;

		case BufSndID:
			inx = SaveRemapSound[inx0];
			if (inx < 0)
			{
				inx = SaveSound.Add(Sound.Get(inx0));
				if (inx < 0)
				{
					SaveOK = FALSE;
					return;
				}
				SaveRemapSound[inx0] = inx;
				SaveSoundSize += SaveSound[inx].Data()->Size + SIZEOFSOUNDPROG;
			}
			break;

		case BufMusID:
			inx = SaveRemapMusic[inx0];
			if (inx < 0)
			{
				inx = SaveMusic.Add(Music.Get(inx0));
				if (inx < 0)
				{
					SaveOK = FALSE;
					return;
				}
				SaveRemapMusic[inx0] = inx;
				SaveMusicSize += SaveMusic[inx].Data()->Size + sizeof(long);
			}
			break;
		}
		dst.DatIndex = inx;

// pøidání cílové položky
		remap->At(index) = prg->Add(&dst);
		if (remap->At(index) < 0)
		{
			SaveOK = FALSE;
			return;
		}

// pøemapování z lokálních objektù do editoru
		if ((buf->BufID() == BufEdiID) && (src->Parent < 0))
		{
			int inx = BufLoc.SrcDat(BufEdiID, index);
			if (inx >= 0)
			{
				inx = SaveRemapLoc[inx];
				if (inx >= 0)
				{
					PETPROG* pet = &SaveLoc[inx];
					ASSERT(pet->DatBlok == BufEdiID);
					if (pet->DatBlok == BufEdiID)
					{
						pet->DatIndex = remap->At(index);
					}
				}
			}
			else
			{
				ASSERTERROR;
			}
		}

// pøemapování z globálních objektù do lokálních objektù
		if ((buf->BufID() == BufLocID) && (src->Parent < 0))
		{
			int inx = BufObj.SrcDat(BufLocID, index);
			if (inx >= 0)
			{
				inx = SaveRemapObj[inx];
				if (inx >= 0)
				{
					PETPROG* pet = &SaveObj[inx];
					ASSERT(pet->DatBlok == BufLocID);
					if (pet->DatBlok == BufLocID)
					{
						pet->DatIndex = remap->At(index);
					}
				}
			}
			else
			{
				ASSERTERROR;
			}
		}

// pokus o vnoøení do potomkù
		index = src->Child;				// vnoøení na potomka
		if (index < 0)					// není žádný potomek?
		{

// nejsou-li potomci, zkusí se následující položka
			index = src->Next;			// pokraèování další položkou

// není-li ani další položka, vrátí se k rodièi
			while ((index < 0) && (src->Parent >= 0))
			{
				index = src->Parent;	// návrat k rodièi
				src = &(buf->At(index)); // adresa rodièe
				index = src->Next;		// další položka za rodièem
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// uložení programu (prázdné jméno = implicitní, rel = uložit distribuèní verzi)

void Save(BOOL mini, CText name, BOOL rel)
{
// kontrola, zda je režim editace
	if (!ProgMode || Saving) return;

// zafixování zmìn UNDO
	Undo.Fixup();

// zapnutí pøíznaku ukládání
	Saving = TRUE;
	SaveOK = TRUE;
	hFile = NULL;
	Release = rel;

// zapnutí kurzoru èekání
	BeginWaitCursor();

// index hlavní funkce
	MainFnc = BufObj.SrcDat(BufLocID, 0);

// inicializace statických objektù
	if (!SaveRemapInt.NumClear(BufInt.Max())) SaveOK = FALSE;
	SaveInt.DelAll();

	if (!SaveRemapObj.NumClear(BufObj.Max())) SaveOK = FALSE;
	SaveObj.DelAll();

	if (!SaveRemapLoc.NumClear(BufLoc.Max())) SaveOK = FALSE;
	SaveLoc.DelAll();

	if (!SaveRemapEdi.NumClear(BufEdi.Max())) SaveOK = FALSE;
	SaveEdi.DelAll();

	if (!SaveRemapReal.NumClear(Real.Max())) SaveOK = FALSE;
	SaveReal.DelAll();

	if (!SaveRemapText.NumClear(Text.Max())) SaveOK = FALSE;
	for (int jaz = 0; jaz < JAZYKNUM; jaz++)
	{
		SaveText[jaz].DelAll();
	}
	
	if (!SaveRemapBool.NumClear(Bool.Max())) SaveOK = FALSE;
	SaveBool.DelAll();

	if (!SaveRemapIcon.NumClear(Icon.Max())) SaveOK = FALSE;
	SaveIcon.DelAll();
	SaveIconSize = 0;
	SaveAddIcon(0);			// ikona pozadí (index 0)

	if (!SaveRemapMap.NumClear(Map.Max())) SaveOK = FALSE;
	SaveMap.DelAll();
	SaveMapSize = 0;
	SaveAddMap(0);			// implicitní plocha (index 0)

	if (!SaveRemapSprite.NumClear(Sprite.Max())) SaveOK = FALSE;
	SaveSprite.DelAll();
	SaveSpriteSize = 0;
	SaveAddSprite(0);		// implicitní sprajt - Petøík (index 0)
	SaveAddSprite(1);		// implicitní sprajt - Lucinka (index 1)

	if (!SaveRemapSound.NumClear(Sound.Max())) SaveOK = FALSE;
	SaveSound.DelAll();
	SaveSoundSize = 0;

	if (!SaveRemapPicture.NumClear(Picture.Max())) SaveOK = FALSE;
	SavePicture.DelAll();
	SavePictureSize = 0;

	if (!SaveRemapMusic.NumClear(Music.Max())) SaveOK = FALSE;
	SaveMusic.DelAll();
	SaveMusicSize = 0;

// pøíprava jména souboru
	if (name.IsEmpty())
	{
		CreatePath(ProgramPath + Cesta);
		FileName = JmenoSave;
	}
	else
	{
		FileName = name;
	}

// pøíprava bufferu záhlaví souboru
	FileHead = (PETHEAD*)MemSize(FileHead, sizeof(SaveHead));
	*FileHead = SaveHead;
	if (Release)
	{
		FileHead->Ident[0] = _T('R');
		FileHead->Ident[1] = _T('E');
		FileHead->Ident[2] = _T('L');

		for (int k = 0; k < NUMOFINDEX; k++)
		{
			for (int l = 0; l < 8; l++)
			{
				FileHead->pi[k].Jmeno[l] = 0;
			}
		}
	}

// jméno pøechodného souboru
	if (name.IsEmpty())
	{
		CreatePath(BackupPath + Cesta);
	}

	CText txt0;
	txt0 = Jmeno.MultiText((int)JAZYK000);
	CText BackName = BackupPath + Cesta + txt0;

// kontrola, zda výstupní soubor není chránìn proti zápisu
	if (SaveOK)
	{
		int attrib = (int)::GetFileAttributes(FileName);
		SaveOK = ((attrib == -1) || ((attrib & FILE_ATTRIBUTE_READONLY) == 0));
	}

// zálohování starého souboru
	BOOL backup = FALSE;
	if (SaveOK && name.IsEmpty())
	{
		::DeleteFile(BackName);
		::MoveFile(FileName, BackName);
		backup = TRUE;
	}

// vytvoøení výstupního souboru
	hFile = INVALID_HANDLE_VALUE;
	if (SaveOK)
	{
		hFile = ::CreateFile(FileName, GENERIC_WRITE, 0, NULL,
					CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		SaveOK = (hFile != INVALID_HANDLE_VALUE);
	}

// zápis zavadìèe programu
	SaveLoader(mini);

// vygenerování bufferù programu
	SavePrg(&SaveObj, &BufObj, &SaveRemapObj);
	SavePrg(&SaveLoc, &BufLoc, &SaveRemapLoc);
	SavePrg(&SaveEdi, &BufEdi, &SaveRemapEdi);

// pøíprava a uložení záhlaví souboru
	if (Release) SaveInt.DelAll();
	FileHead->piImport.Pocet = SaveInt.Num();
	FileHead->piImport.Delka = SaveInt.Size();
	FileHead->piGlobal.Pocet = SaveObj.Num();
	FileHead->piGlobal.Delka = SaveObj.Num() * SIZEOFPETPROG;
	FileHead->piLocal.Pocet = SaveLoc.Num();
	FileHead->piLocal.Delka = SaveLoc.Num() * SIZEOFPETPROG;
	FileHead->piProgram.Pocet = SaveEdi.Num();
	FileHead->piProgram.Delka = SaveEdi.Num() * SIZEOFPETPROG;
	FileHead->piReal.Pocet = SaveReal.Num();
	FileHead->piReal.Delka = SaveReal.Num() * sizeof(double);

	int savetextn = SaveText[0].Num();
	int savetexts = 0;
	int jazyku = 0;
	int jazyk1 = 0;

	for (jaz = 0; jaz < JAZYKNUM; jaz++)
	{
		if (SaveText[jaz].NumVal() != 0)
		{
			if (jazyku == 0)
			{
				jazyk1 = jaz;
			}

			savetexts += SaveText[jaz].Size() + SIZEOFTEXTPROG;
			jazyku++;
		}
	}

	if (jazyku == 0)
	{
		jazyku = 1;
		jazyk1 = JazykDef;
		savetexts = SaveText[jazyk1].Size() + SIZEOFTEXTPROG;
	}

	FileHead->piText.Pocet = savetextn;
	FileHead->piText.Delka = savetexts;
	FileHead->piText.Extra2 = jazyku;
	FileHead->piImport.Extra2 = 0;

	FileHead->piBool.Pocet = SaveBool.Num();
	FileHead->piBool.Delka = SaveBool.Num() * sizeof(char);
	FileHead->piIcon.Pocet = SaveIcon.Num();
	FileHead->piIcon.Delka = SaveIconSize;
	FileHead->piMap.Pocet = SaveMap.Num();
	FileHead->piMap.Delka = SaveMapSize;
	FileHead->piSprite.Pocet = SaveSprite.Num();
	FileHead->piSprite.Delka = SaveSpriteSize;
	FileHead->piSound.Pocet = SaveSound.Num();
	FileHead->piSound.Delka = SaveSoundSize;
	FileHead->piPic.Pocet = SavePicture.Num();
	FileHead->piPic.Delka = SavePictureSize;
	FileHead->piMusic.Pocet = SaveMusic.Num();
	FileHead->piMusic.Delka = SaveMusicSize;
	FileHead->piPalette.Pocet = StdColors;
	FileHead->piPalette.Delka = StdColors * sizeof(RGBQUAD);

// zápis záhlaví souboru
	SaveWriteFile(FileHead, sizeof(SaveHead));

// zápis bufferu textù interních funkcí
	SaveWriteFile(SaveInt.Data(), SaveInt.Size());

// zápis bufferu globálních objektù
	SaveWriteFile(SaveObj.Data(), SaveObj.Num() * SIZEOFPETPROG);

// zápis bufferu lokálních objektù
	SaveWriteFile(SaveLoc.Data(), SaveLoc.Num() * SIZEOFPETPROG);

// zápis bufferu editoru
	SaveWriteFile(SaveEdi.Data(), SaveEdi.Num() * SIZEOFPETPROG);

// zápis bufferu èísel
	SaveWriteFile(SaveReal.Data(), SaveReal.Num() * sizeof(double));

// zápis bufferu textù

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Na pøechodnou dobu (pro zabránìní havárií u starších verzí) bude
// jako 1. text ukládán fiktivní jazyk s prázdnými texty a LangID = -1
//	char* bb = (char*)MemGet(ii);
//	MemFill(bb, ii, 0);
//	SaveWriteFile(bb, ii);
//	MemFree(bb);
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	TEXTPROG tt;
	tt.LangID = JazykInfo[jazyk1].LangID;
	tt.CodePage = JazykInfo[jazyk1].CodePage;

	SaveWriteFile(&tt, SIZEOFTEXTPROG);
	SaveWriteFile(SaveText[jazyk1].Data(), SaveText[jazyk1].Size());

	for (jaz = jazyk1 + 1; jaz < JAZYKNUM; jaz++)
	{
		if (SaveText[jaz].NumVal() != 0)
		{
			tt.LangID = JazykInfo[jaz].LangID;
			tt.CodePage = JazykInfo[jaz].CodePage;

			SaveWriteFile(&tt, SIZEOFTEXTPROG);
			SaveWriteFile(SaveText[jaz].Data(), SaveText[jaz].Size());
		}
	}

// zápis bufferu logických hodnot
	if (SaveOK)
	{
		int i = SaveBool.Num();
		char* data = (char*)MemGet(i);
		if (data != NULL)
		{
			for (i--; i >= 0; i--) { data[i] = (char)SaveBool[i]; };
			SaveWriteFile(data, SaveBool.Num());
			MemFree(data);
		}
		else
		{
			SaveOK = FALSE;
		}
	}

// zápis bufferu ikon
	if (SaveOK)
	{
		char* data = (char*)MemGet(SaveIconSize);
		if (data != NULL)
		{
			BYTE* dst = (BYTE*)data;
			for (int i = 0; i < SaveIcon.Num(); i++) 
			{ 
				int n = SaveIcon[i].Comp() + 4;
				MemCopy(dst, SaveIcon[i].DataData(), n); 
				dst += n;
			};
			ASSERT(((char*)dst - data) == SaveIconSize);
			SaveWriteFile(data, SaveIconSize);
			MemFree(data);
		}
		else
		{
			SaveOK = FALSE;
		}
	}

// zápis bufferu ploch
	if (SaveOK)
	{
		char* data = (char*)MemGet(SaveMapSize);
		if (data != NULL)
		{
			char* dst = data;
			for (int i = 0; i < SaveMap.Num(); i++)
			{
				int w = SaveMap[i].Width();
				int h = SaveMap[i].Height();
				((MAPPROG*)dst)->Width = w;
				((MAPPROG*)dst)->Height = h;
				MemCopy(((MAPPROG*)dst)->Data, SaveMap[i].DataData(), w * h * SIZEOFMAPITEM);
				dst += w * h * SIZEOFMAPITEM + SIZEOFMAPPROG;
			}
			ASSERT((dst - data) == SaveMapSize);
			SaveWriteFile(data, SaveMapSize);
			MemFree(data);
		}
		else
		{
			SaveOK = FALSE;
		}
	}

// zápis bufferu obrázkù
	if (SaveOK)
	{
		char* data = (char*)MemGet(SavePictureSize);
		if (data != NULL)
		{
			BYTE* dst = (BYTE*)data;
			for (int i = 0; i < SavePicture.Num(); i++) 
			{ 
				int w =  SavePicture[i].Width();
				((PICPROG*)dst)->Width = w;
				int h =  SavePicture[i].Height();
				((PICPROG*)dst)->Height = h;
				int s = SavePicture[i].Comp() + 4;
				MemCopy(((PICPROG*)dst)->Data, SavePicture[i].DataData(), s);
				dst += s + SIZEOFPICPROG;
			};
			ASSERT(((char*)dst - data) == SavePictureSize);				
			SaveWriteFile(data, SavePictureSize);
			MemFree(data);
		}
		else
		{
			SaveOK = FALSE;
		}
	}

// zápis bufferu sprajtù
	if (SaveOK)
	{
		char* data = (char*)MemGet(SaveSpriteSize);
		if (data != NULL)
		{
			char* dst = data;
			for (int i = 0; i < SaveSprite.Num(); i++)
			{
				int f = SaveSprite[i].Faze();
				int s = SaveSprite[i].Smer();
				((SPRITEPROG*)dst)->Faze = f;
				((SPRITEPROG*)dst)->Smer = s;
				((SPRITEPROG*)dst)->Klid = SaveSprite[i].Klid();
				int width = SaveSprite[i].Width();
				((SPRITEPROG*)dst)->Width = (WORD)width;
				int height = SaveSprite[i].Height();
				((SPRITEPROG*)dst)->Height = (WORD)height;
				((SPRITEPROG*)dst)->Delay = SaveSprite[i].Delay();
				((SPRITEPROG*)dst)->Level = SaveSprite[i].Level();
				((SPRITEPROG*)dst)->Kroku = SaveSprite[i].Kroku();
				dst += SIZEOFSPRITEPROG;
				for (int j = 0; j < f*s; j++)
				{
					int size = SaveSprite[i][j].Comp() + 4;
					MemCopy(dst, SaveSprite[i][j].DataData(), size);
					dst += size;
				}
			}
			ASSERT((dst - data) == SaveSpriteSize);
			SaveWriteFile(data, SaveSpriteSize);
			MemFree(data);
		}
		else
		{
			SaveOK = FALSE;
		}
	}

// zápis bufferu zvukù
	if (SaveOK)
	{
		char* data = (char*)MemGet(SaveSoundSize);
		if (data != NULL)
		{
			char* dst = data;
			for (int i = 0; i < SaveSound.Num(); i++)
			{
				int s = (SaveSound[i].Data())->Size;
				SOUNDPROG* snd = (SOUNDPROG*)dst;
				snd->Size = s;
				snd->Samples = (SaveSound[i].Data())->Samples;
				snd->Format = (WORD)(SaveSound[i].Data())->Format;
				snd->Channels = (BYTE)(SaveSound[i].Data())->Channels;
				snd->Bits = (BYTE)(SaveSound[i].Data())->Bits;

				MemCopy(snd->Data, SaveSound[i].DataData(), s);

				dst += s + SIZEOFSOUNDPROG;
			}
			ASSERT((dst - data) == SaveSoundSize);
			SaveWriteFile(data, SaveSoundSize);
			MemFree(data);
		}
		else
		{
			SaveOK = FALSE;
		}
	}

// zápis bufferu hudby
	if (SaveOK)
	{
		char* data = (char*)MemGet(SaveMusicSize);
		if (data != NULL)
		{
			BYTE* dst = (BYTE*)data;
			for (int i = 0; i < SaveMusic.Num(); i++) 
			{ 
				int s = SaveMusic[i].Size();
				*(long*)dst = s;
				dst += sizeof(long);
				MemCopy(dst, SaveMusic[i].DataData(), s); 
				dst += s;
			};
			ASSERT(((char*)dst - data) == SaveMusicSize);
			SaveWriteFile(data, SaveMusicSize);
			MemFree(data);
		}
		else
		{
			SaveOK = FALSE;
		}
	}

// zápis palet
	SaveWriteFile(StdBitmapInfo->bmiColors, StdColors*sizeof(RGBQUAD));

// zjištìní velikosti dat
	int oldsize = ::SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
	if (oldsize < 0) oldsize = 0;
	int newsize = (oldsize + 0x1ff) & ~0x1ff;

// zarovnání na sektor
	long n = newsize - oldsize;
	if (n > 0)
	{
		BYTE* dat = (BYTE*)MemGet(n);
		if (dat != NULL)
		{
			MemFill(dat, n, 0);
			SaveWriteFile(dat, n);
			MemFree(dat);
		}
	}

// zápis virtuální velikosti
	if (SaveOK)
	{
		n = oldsize - SaveDataOff;
		::SetFilePointer(hFile, SaveVirtSize, NULL, FILE_BEGIN);
		SaveWriteFile(&n, 4);
		::SetFilePointer(hFile, SaveVirtSize2, NULL, FILE_BEGIN);
		SaveWriteFile(&n, 4);
	}

// zápis velikosti dat
	if (SaveOK)
	{
		n = newsize - SaveDataOff;
		::SetFilePointer(hFile, SaveFileSize, NULL, FILE_BEGIN);
		SaveWriteFile(&n, 4);
	}

// zápis velikosti obrazu
	if (SaveOK)
	{
		n = (SaveDataVirt + (oldsize - SaveDataOff) + PAGESIZE-1) & ~(PAGESIZE-1);
		::SetFilePointer(hFile, SaveImageSize, NULL, FILE_BEGIN);
		SaveWriteFile(&n, 4);
	}

// uzavøení souboru
	if (hFile != NULL)
	{
		::CloseHandle(hFile);
	}

// pøi chybì navrácení starého souboru
	if (!SaveOK && backup)
	{
		::DeleteFile(FileName);
		::MoveFile(BackName, FileName);
	}

// pøi úspìšné operaci resetování pøíznaku modifikace
	if (SaveOK && !Release)
	{
		ResModi();
		JmenoLoad = JmenoSave;
	}

// vypnutí kurzoru èekání
	EndWaitCursor();

// pøíznak konce ukládání
	Saving = FALSE;

// chybové hlášení
	if (!SaveOK && !MemoryError)
	{
		::DialogBoxParam(
			hInstance,
			MAKEINTRESOURCE(IDN_WRITEERR),
			MainFrame,
			(DLGPROC)AboutBoxProc,
			(LPARAM)IDN_WRITEERR);
	}

// vyprázdnìní bufferù
	SaveRemapInt.DelAll();
	SaveInt.DelAll();
	SaveRemapObj.DelAll();
	SaveObj.DelAll();
	SaveRemapLoc.DelAll();
	SaveLoc.DelAll();
	SaveRemapEdi.DelAll();
	SaveEdi.DelAll();
	SaveRemapReal.DelAll();
	SaveReal.DelAll();
	SaveRemapText.DelAll();
	for (jaz = 0; jaz < JAZYKNUM; jaz++)
	{
		SaveText[jaz].DelAll();
	}
	SaveRemapBool.DelAll();
	SaveBool.DelAll();
	SaveRemapIcon.DelAll();
	SaveIcon.DelAll();
	SaveRemapMap.DelAll();
	SaveMap.DelAll();
	SaveRemapSprite.DelAll();
	SaveSprite.DelAll();
	SaveRemapSound.DelAll();
	SaveSound.DelAll();
	SaveRemapPicture.DelAll();
	SavePicture.DelAll();
	SaveRemapMusic.DelAll();
	SaveMusic.DelAll();
}

/////////////////////////////////////////////////////////////////////////////
// uložení nespustitelnì (miniverze)

void SaveMini()
{
	Save(TRUE, EmptyText, FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// uložení spustitelnì (maxiverze)

void SaveMaxi()
{
	Save(FALSE, EmptyText, FALSE);
}

// pøíprava masky souborù
// øetìzec _T("Programy (*.exe)\0*.EXE\0Všechny soubory (*.*)\0*.*\0\0")
void InitFiltr(TCHAR* filtr)
{
	CText txt;

	txt.Load(IDN_PROGRAMY);
	txt.Add(_T(" (*.exe)"));
	txt.Write(filtr);
	int filtrn = txt.Length();

	filtr[filtrn] = 0;
	filtrn++;

	txt = _T("*.EXE");
	txt.Write(filtr + filtrn);
	filtrn += txt.Length();

	filtr[filtrn] = 0;
	filtrn++;

	txt.Load(IDN_VSECHNY_SOUBORY);
	txt.Add(_T(" (*.*)"));
	txt.Write(filtr + filtrn);
	filtrn += txt.Length();

	filtr[filtrn] = 0;
	filtrn++;

	txt = _T("*.*");
	txt.Write(filtr + filtrn);
	filtrn += txt.Length();

	filtr[filtrn] = 0;
	filtrn++;
	filtr[filtrn] = 0;
}

/////////////////////////////////////////////////////////////////////////////
// uložení do jiného jména

void SaveAs()
{
// pøíprava filtru
	TCHAR* filtr = (TCHAR*)MemGet(1024);
	if (filtr == NULL) return;
	InitFiltr(filtr);

// pøíprava struktury k zadání jména souboru
	OPENFILENAME ofn;
	MemFill(&ofn, sizeof(ofn), 0);
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = MainFrame;
	ofn.lpstrFilter = filtr;
	ofn.Flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;

// pøíprava titulku okna
	CText txt;
	txt.Load(IDN_SAVE_AS1);
	ofn.lpstrTitle = txt;

// aktuální adresáø
	CText path = ProgramPath + Cesta;
	ofn.lpstrInitialDir = path;

// pøednastavené jméno souboru
	CText name;
	name = Jmeno.MultiText();
	name.FileName(0);
	int n = name.Length() + 1;
	if (n < 1024) n = 1024;

// buffer k zadání jména souboru
	TCHAR* file = (TCHAR*)MemGet(n*sizeof(TCHAR));
	if (file == NULL)
	{
		MemFree(filtr);
		return;
	}
	MemFill(file, n*sizeof(TCHAR), 0);
	name.Write(file);
	ofn.lpstrFile = file;
	ofn.nMaxFile = n;

// zadání jména souboru
	if (!::GetSaveFileName(&ofn))
	{
		MemFree(filtr);
		MemFree(file);
		return;
	}

// korekce jména souboru
	MemFree(filtr);
	name = file;
	MemFree(file);
	name.TrimLeft();
	name.TrimRight();
	if (name.IsEmpty()) return;
	if (name.RevFind(_T('.')) < 0)
	{
		name += TextExe;
	}

// uložení souboru
	Save(FALSE, name, FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// uložení distribuèní verze

void SaveRel()
{
// pøíprava filtru
	TCHAR* filtr = (TCHAR*)MemGet(1024);
	if (filtr == NULL) return;
	InitFiltr(filtr);

// pøíprava struktury k zadání jména souboru
	OPENFILENAME ofn;
	MemFill(&ofn, sizeof(ofn), 0);
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = MainFrame;
	ofn.lpstrFilter = filtr;
	ofn.Flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;

// pøíprava titulku okna
	CText txt;
	txt.Load(IDN_SAVE_DISTR1);
	ofn.lpstrTitle = txt;

// pøednastavené jméno souboru
	CText name;
	name = Jmeno.MultiText();
	name.FileName(0);
	int n = name.Length() + 1;
	if (n < 1024) n = 1024;

// buffer k zadání jména souboru
	TCHAR* file = (TCHAR*)MemGet(n*sizeof(TCHAR));
	if (file == NULL)
	{
		MemFree(filtr);
		return;
	}
	MemFill(file, n*sizeof(TCHAR), 0);
	name.Write(file);
	ofn.lpstrFile = file;
	ofn.nMaxFile = n;

// zadání jména souboru
	if (!::GetSaveFileName(&ofn))
	{
		MemFree(filtr);
		MemFree(file);
		return;
	}

// korekce jména souboru
	MemFree(filtr);
	name = file;
	MemFree(file);
	name.TrimLeft();
	name.TrimRight();
	if (name.IsEmpty()) return;
	if (name.RevFind(_T('.')) < 0)
	{
		name += TextExe;
	}

// test, zda soubor již existuje
	if ((int)::GetFileAttributes(name) != -1)
	{
		CText txt1;
		txt1.Load(IDN_SAVE_DISTR2);
		CText txt2;
		txt2.Load(IDN_SAVE_DISTR3);

		if (::MessageBox(MainFrame, txt2, txt1, MB_OKCANCEL | MB_ICONWARNING) != IDOK) return;
	}

// uložení souboru
	Save(FALSE, name, TRUE);
}

/***************************************************************************\
*																			*
*									naètení									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// inicializace konverzní tabulky importu funkcí (vrací FALSE=chyba pamìti)
// - voláno i z obsluhy clipboardu (funkce Paste)

BOOL InitImportTab()
{
// vytvoøení bufferu blokù pro import funkcí
	int* buf = (int*)MemSize(ImportBlok, FncNum * sizeof(int));
	if (buf == NULL) return FALSE;
	ImportBlok = buf;

// vytvoøení bufferu indexù pro import funkcí
	buf = (int*)MemSize(ImportIndex, FncNum * sizeof(int));
	if (buf == NULL) return FALSE;
	ImportIndex = buf;

// nasmìrování položek bufferù importu na interní buffer
	int i;
	for (i = FncNum-1; i >= 0; i--)
	{
		ImportBlok[i] = BufIntID;
		ImportIndex[i] = i;
	}

// nasmìrování funkcí na buffer struktur (hledat od konce - použije se tak první výskyt)
	for (i = BufStr.Max()-1; i >= 0; i--)
	{
		if (BufStr.IsValid(i) && ((BufStr[i].Param & PR_NOREFER) == 0))
		{
			int fnc = BufStr[i].Func - IDF;
			ASSERT((DWORD)fnc < (DWORD)FncNum);
			ImportBlok[fnc] = BufStrID;
			ImportIndex[fnc] = i;
		}
	}

// nasmìrování funkcí na buffer tøíd (hledat od konce - použije se tak výchozí položka tøídy objektu)
	for (i = BufCls.Max()-1; i >= 0; i--)
	{
		if (BufCls.IsValid(i))
		{
			int fnc = BufCls[i].Func - IDF;
			if (fnc != (IDF_GROUP - IDF))
			{
				ASSERT((DWORD)fnc < (DWORD)FncNum);
				ImportBlok[fnc] = BufClsID;
				ImportIndex[fnc] = i;
			}
		}
	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// naètení dat ze souboru (vrací TRUE=operace OK)

BOOL FileLoadRead(void* buf, int len)
{
	if (len < 0) len = 0;
	DWORD read = 0;
	if (::ReadFile(hFile, buf, len, &read, NULL) && (read == (DWORD)len))
	{
		return TRUE;
	}

	LoadError = TRUE;
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// nastavení ukazatele v souboru

void FilePointer(int off)
{
	::SetFilePointer(hFile, off, NULL, FILE_BEGIN);
}


/////////////////////////////////////////////////////////////////////////////
// naètení záhlaví souboru (vrací TRUE=operace OK)

BOOL FileLoadHead()
{
// buffer k naètení DOS záhlaví souboru
	IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)MemGet(sizeof(IMAGE_DOS_HEADER));
	if (dos == NULL) return FALSE;

// naètení a kontrola DOS záhlaví souboru
	FilePointer(0);
	if (!FileLoadRead(dos, sizeof(IMAGE_DOS_HEADER)) ||
		(dos->e_magic != IMAGE_DOS_SIGNATURE) ||
		(dos->e_lfanew < 0x60) ||
		(dos->e_lfanew >= 0x1000))
	{
		MemFree(dos);
		return FALSE;
	}

// uvolnìní DOS záhlaví souboru
	int off = dos->e_lfanew;						// offset NT záhlaví
	MemFree(dos);

// buffer k naètení NT záhlaví souboru
	IMAGE_NT_HEADERS* hdr = (IMAGE_NT_HEADERS*)MemGet(sizeof(IMAGE_NT_HEADERS));
	if (hdr == NULL) return FALSE;

// naètení a kontrola NT záhlaví souboru
	FilePointer(off);
	if (!FileLoadRead(hdr, sizeof(IMAGE_NT_HEADERS)) ||
		(hdr->Signature != IMAGE_NT_SIGNATURE))
	{
		MemFree(hdr);
		return FALSE;
	}

// uvolnìní NT záhlaví
	int opt = hdr->FileHeader.SizeOfOptionalHeader; // velikost volitelné sekce
	int sekce = hdr->FileHeader.NumberOfSections;	// poèet sekcí
	off += ((DWORD)(&hdr->OptionalHeader) - (DWORD)hdr + opt);
	MemFree(hdr);

// kontrola velikosti záhlaví vèetnì sekcí
	if ((sekce < 1) ||								// minimálnì sekcí
		(sekce > 1000) ||							// maximálnì sekcí
		(opt < 50) ||								// minimální velikost záhlaví
		(opt > 10000))								// maximální velikost záhlaví
	{
		return FALSE;
	}

// buffer k naètení popisovaèù sekcí
	IMAGE_SECTION_HEADER* sec = (IMAGE_SECTION_HEADER*)MemGet(sekce * IMAGE_SIZEOF_SECTION_HEADER);
	if (sec == NULL) return FALSE;

// naètení popisovaèù sekcí
	FilePointer(off);
	if (!FileLoadRead(sec, sekce * IMAGE_SIZEOF_SECTION_HEADER))
	{
		MemFree(sec);
		return FALSE;
	}

// nalezení sekce Petøíka
	for (int i = 0; i < sekce; i++)
	{
		if (MemCompare(sec[i].Name, PetProgHeader.Name, 8)) break;
	}

// kontrola, zda byla nalezena sekce Petøíka
	if (i >= sekce)
	{
		MemFree(sec);
		return FALSE;
	}

// uvolnìní bufferu sekcí
	FileOffset = sec[i].PointerToRawData;
	MemFree(sec);

// pøíprava bufferu k naètení záhlaví Petøíka
	MemFree(FileHead);
	FileHead = (PETHEAD*)MemGet(SIZEOFPETHEAD);
	if (FileHead == NULL) return FALSE;

// naètení záhlaví Petøíka
	FilePointer(FileOffset);
	if (!FileLoadRead(FileHead, SIZEOFPETHEAD)) return FALSE;
	RemapBlokNum = FileHead->Pocet;

// kontrola záhlaví PET
	if ((FileHead->Ident[0] != 'P') ||
		(FileHead->Ident[1] != 'E') ||
		(FileHead->Ident[2] != 'T') ||
		(FileHead->Verze != 1) ||
		(FileHead->Data < (long)(RemapBlokNum * SIZEOFPETINDEX + SIZEOFPETHEAD)) ||
		((DWORD)RemapBlokNum > 100)) return FALSE;

// úschova verze souboru
	FileVer = FileHead->Verze;

// nastavení velikosti bufferu a naètení zbytku záhlaví
	PETHEAD* filehead = (PETHEAD*)MemSize(FileHead, FileHead->Data);
	if (filehead == NULL) return FALSE;
	FileHead = filehead;
	if (!FileLoadRead(FileHead->pi, FileHead->Data - SIZEOFPETHEAD)) return FALSE;

// inicializace bufferu pro pøemapování blokù
	int* remapblok = (int*)MemSize(RemapBlok, RemapBlokNum * sizeof(int));
	if (remapblok == NULL) return FALSE;
	RemapBlok = remapblok;
	MemFill(remapblok, RemapBlokNum * sizeof(int), -1);

// inicializace bufferu poètù pro pøemapování indexù
	int* remapindexnum = (int*)MemSize(RemapIndexNum, RemapBlokNum * sizeof(int));
	if (remapindexnum == NULL) return FALSE;
	RemapIndexNum = remapindexnum;
	MemFill(remapindexnum, RemapBlokNum * sizeof(int), 0);

// inicializace bufferu bufferù pro pøemapování indexù
	int** remapindex = (int**)MemSize(RemapIndex, RemapBlokNum * sizeof(int*));
	if (remapindex == NULL) return FALSE;
	RemapIndex = remapindex;
	MemFill(remapindex, RemapBlokNum * sizeof(int*), 0);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// nalezení datového bloku (vrací index bloku v záhlaví, -1=nenalezeno)
// (bufID = index bufferu, itemsize = velikost položky; 0=promìnlivá)

int FileLoadIndex(const char* jmeno, int bufID, int itemsize)
{
// lokální promìnné
	int i;										// ukazatel indexu bloku
	long off = FileHead->Data + FileOffset;		// offset zaèátku bloku

// prohledání tabulky popisovaèù datových blokù
	for (i = 0; i < FileHead->Pocet; i++)
	{

// porovnání identifikátoru jedné položky
		if (MemCompare(FileHead->pi[i].Jmeno, jmeno, 8))
		{

// nastavení ukazatele v souboru na zaèátek dat bloku
			FilePointer(off);

// nastavení mapování bufferu
			RemapBlok[i] = bufID;

// pøíprava délky dat k naètení
			DatNum = FileHead->pi[i].Delka;
			if (DatNum < 0) DatNum = 0;

// pøíprava poètu položek
			RemapIndexNum[i] = FileHead->pi[i].Pocet;
			if (RemapIndexNum[i] < 0) RemapIndexNum[i] = 0;

// verze dat
			DatVerze = FileHead->pi[i].Verze;

// korekce poètu položek, je-li známá velikost položky
			if (itemsize > 0)
			{
				if ((RemapIndexNum[i]*itemsize) > DatNum)
				{
					RemapIndexNum[i] = DatNum / itemsize;
					LoadError = TRUE;
				}
			}

// pøíprava bufferu pro mapování položek
			int* remapindex = (int*)MemSize(RemapIndex[i], RemapIndexNum[i] * sizeof(int));
			if (remapindex == NULL)
			{
				RemapIndexNum[i] = 0;
				DatNum = 0;
			}
			else
			{
				RemapIndex[i] = remapindex;
				MemFill(remapindex, RemapIndexNum[i]*sizeof(int), -1);
			}

// vytvoøení buffer k naètení dat (na konci pojistná rezerva kvùli textùm atd.)
			MemFree(DatBuf);
			DatBuf = (BYTE*)MemGet(DatNum + 2*RemapIndexNum[i] + 64);
			if (DatBuf == NULL)
			{
				RemapIndexNum[i] = 0;
				DatNum = 0;
			}
			else
			{
				MemFill(DatBuf + DatNum, 2*RemapIndexNum[i] + 64, 0);
			}

// naètení dat do bufferu
			if (!FileLoadRead(DatBuf, DatNum))
			{
				RemapIndexNum[i] = 0;
				DatNum = 0;
			}

// adresa konce dat
			DatEnd = DatBuf + DatNum;

// navrácení indexu bufferu
			return i;
		}

// posun offsetu zaèátku bloku
		off += FileHead->pi[i].Delka;
	}

// blok nenalezen
	return -1;
}


/////////////////////////////////////////////////////////////////////////////
// naètení palet

void FileLoadPalet()
{
// nalezení bufferu palet (pøi chybì použije standardní palety)
	int inx = FileLoadIndex(SaveHead.piPalette.Jmeno, BufPalID, sizeof(RGBQUAD));
	if (inx < 0) 
	{
		GenKonvPal(StdBitmapInfo);
		return;
	}

// pøíprava poètu palet
	int pocet = RemapIndexNum[inx];
	if (pocet > 255) pocet = 255;

// pøíprava bufferu k naètení palet
	BITMAPINFO* bmp;
	bmp = (BITMAPINFO*)MemGet(sizeof(BITMAPINFO) + sizeof(RGBQUAD)*pocet);
	if (bmp == NULL) return;

// naplnìní bufferu palet
	MemCopy(bmp, StdBitmapInfo, sizeof(BITMAPINFO));
	MemCopy(bmp->bmiColors, DatBuf, pocet * sizeof(RGBQUAD));

// barva 0 je vždy použita jako pozadí
	bmp->bmiColors[0] = StdBitmapInfo->bmiColors[0];

// barva 1 bude jako stín (kromì importu ze starší verze)
// !!!!!!!!!!!!!!!! IMPORT !!!!!!!!!!!!!!!!!!!
	if (pocet >= StdColors)
	{
		bmp->bmiColors[1] = StdBitmapInfo->bmiColors[1];
	}

// import palet
	bmp->bmiHeader.biClrUsed = pocet;
	bmp->bmiHeader.biClrImportant = pocet;
	GenKonvPal(bmp);

// zrušení bufferu
	MemFree(bmp);
}


/////////////////////////////////////////////////////////////////////////////
// naètení èísel

void FileLoadReal()
{
// nalezení bufferu èísel
	int inx = FileLoadIndex(SaveHead.piReal.Jmeno, BufNumID, sizeof(double));
	if (inx < 0) return;

// pøíprava ukazatelù
	int num = RemapIndexNum[inx];		// poèet položek k naètení
	int* index = RemapIndex[inx];		// tabulka indexù položek
	double* data = (double*)DatBuf;			// ukazatel dat k naètení

// naètení èísel z bufferu
	for (; num > 0; num--)
	{
		*index = Real.Add(*data);
		data++;
		index++;
	}
	if ((BYTE*)data != DatEnd) LoadError = TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// naètení textù

void FileLoadText()
{
// nalezení bufferu textù
	int inx = FileLoadIndex(SaveHead.piText.Jmeno, BufTxtID, 0);
	TextInx = inx;
	if (inx < 0) return;

// pøíprava ukazatelù
	int num0 = RemapIndexNum[inx];		// poèet položek k naètení
	int jazyku = FileHead->pi[inx].Extra2; // poèet jazykù
	if (DatVerze != 1) jazyku = 1;
	int* index0 = RemapIndex[inx];		// tabulka indexù položek
	char* data = (char*)DatBuf;
	bool first = true;					// je první jazyk

// naètení textù z bufferu pro všechny jazyky
	for (; jazyku > 0; jazyku--)
	{

// pøíprava kódu jazyku
		int jazyk;
		int codepage;
		int num = num0;
		int* index = index0;

		if (DatVerze == 1)
		{
			for (jazyk = 0; jazyk < JAZYKNUM; jazyk++)
			{
				if ((JazykInfo[jazyk].LangID & 0xff) == (((TEXTPROG*)data)->LangID & 0xff))
				{
					break;
				}
			}
			codepage = ((TEXTPROG*)data)->CodePage;
			data += SIZEOFTEXTPROG;
		}
		else
		{
			jazyk = JazykDef;
			codepage = 0;
		}

// pøeskoèení pro neplatný text
		if ((DWORD)jazyk >= (DWORD)JAZYKNUM)
		{
			int nnn = 1;
			if (codepage == 0) nnn = 2;

			for (; num > 0; num--)
			{
				if ((BYTE*)(data + 4) > DatEnd) break;
				int len = *(long*)data;
				if (len < 0) len = 0;
				data = data + 4 + len*nnn;
			}
		}

// naètení textù
		else
		{
			for (; num > 0; num--)
			{
				if ((BYTE*)(data + 4) > DatEnd) break;
				int len = *(long*)data;				// délka textu (znakù)
				if (len < 0) len = 0;
				data = data + 4;					// pøeskoèení délky

				CText txt;

				if (codepage == 0)
				{
					txt.Set((WCHAR*)data, len);
					data += len;
				}
				else
				{
					txt.Set(data, len);
				}
				data += len;

				if (first)
				{
					CMultiText mt;
					mt.MultiText(jazyk, txt);
					*index = Text.Add(mt);
				}
				else
				{
					Text[*index].MultiText(jazyk, txt);
				}

				index++;
			}

			first = false;
		}
	}
	if ((BYTE*)data != DatEnd) LoadError = TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// naètení logických hodnot

void FileLoadBool()
{
// nalezení bufferu logických hodnot
	int inx = FileLoadIndex(SaveHead.piBool.Jmeno, BufLogID, sizeof(char));
	if (inx < 0) return;

// pøíprava ukazatelù
	int num = RemapIndexNum[inx];		// poèet položek k naètení
	int* index = RemapIndex[inx];		// tabulka indexù položek
	char* data = (char*)DatBuf;			// ukazatel dat k naètení

// naètení hodnot z bufferu
	for (; num > 0; num--)
	{
		*index = Bool.Add(*data);
		data++;
		index++;
	}
	if ((BYTE*)data != DatEnd) LoadError = TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// naètení ikon

void FileLoadIcon()
{
// nalezení bufferu ikon
	int inx = FileLoadIndex(SaveHead.piIcon.Jmeno, BufIcoID, 0);
	IconInx = inx;					// index bufferu ikon
	if (inx < 0) return;

// pøíprava ukazatelù
	int num = RemapIndexNum[inx];		// poèet položek k naètení
	int* index = RemapIndex[inx];		// tabulka indexù položek
	MemFill(index, num * sizeof(int), 0); // pro pøípad chyby - ikony 0
	BYTE* data = DatBuf;					// ukazatel dat k naètení
	ASSERT(Icon.IconSize() == ICONSIZE);
	BYTE* data0 = (BYTE*)MemGet(ICONSIZE);	// pomocný bufferu pro dekompresi

// pøenesení dat pro ikonu 0 (= ikona podkladu)
	if (num > 0)
	{
		ASSERT(Icon.IsValid(0));
		*index = 0;
		if (!Icon[0].CopyWrite() || !Icon[0].DeComp()) return;

		int size = ICONSIZE;			// velikost dat ikony
		BYTE* data1 = data;

		if (DatVerze == 1)				// komprese
		{
			data1 = data0;
			size = *(long*)data + 4;
			DeCompress(data1, data + 4, ICONSIZE, ICONWIDTH);
		}

		KonvCopy(Icon[0].DataData(), data1, ICONSIZE); // kopie dat s konverzí
		Icon[0].Comp();				// komprese dat ikony

		data += size;
		index++;
		num--;
	}

// naètení hodnot z bufferu
	for (; ((num > 0) && (data < DatEnd)); num--)
	{
		int size = ICONSIZE;

		int i = Icon.New();					// nová ikona
		if (i < 0) return;
		*index = i;
		Icon[i].CopyWrite();			// pøipravena k zápisu

		if (DatVerze == 1)					// jsou komprimovaná data
		{
			size = *(long*)data + 4;		// velikost komprimovaných dat

			if (KonvPalOK)
			{
				Icon[i].CompCopy(data); // kopie dat bez dekomprese
			}
			else
			{
				DeCompress(data0, data + 4, ICONSIZE, ICONWIDTH); // dekomprese dat
				KonvCopy(Icon[i].DataData(), data0, ICONSIZE); // kopie s konverzí
			}
		}
		else
		{
			KonvCopy(Icon[i].DataData(), data, ICONSIZE);	// kopie nezkomprimovaných dat
		}

		Icon[i].Comp();						// komprese ikony
		data += size;
		index++;
	}

// uvolnìní pomocného bufferu
	MemFree(data0);

	if (data != DatEnd) LoadError = TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// naètení ploch

void FileLoadMap()
{
// nalezení bufferu ploch
	int inx = FileLoadIndex(SaveHead.piMap.Jmeno, BufMapID, 0);
	if (inx < 0) return;

// pøíprava ukazatelù
	int num = RemapIndexNum[inx];		// poèet položek k naètení
	int* index = RemapIndex[inx];		// tabulka indexù položek
	BYTE* data = DatBuf;					// ukazatel dat k naètení

// pøíprava bufferu pro pøemapování ikon
	int iconnum = 0;
	int* iconindex = NULL;
	if (IconInx >= 0)
	{
		iconnum = RemapIndexNum[IconInx];
		iconindex = RemapIndex[IconInx];
	}

// naètení hodnot z bufferu
	for (; num > 0; num--)
	{

// kontrola velikosti dat
		if ((BYTE*)(((MAPPROG*)data)->Data) > DatEnd)
		{
			LoadError = TRUE;
			return;
		}

// pøíprava šíøky plochy
		int width = ((MAPPROG*)data)->Width;
		if (width < 1) width = 1;

// pøíprava výšky plochy
		int height = ((MAPPROG*)data)->Height;
		if (height < 1) height = 1;

// kontrola velikosti dat
		if ((width*height <= 0) ||
			((BYTE*)(((MAPPROG*)data)->Data + width*height) > DatEnd))
		{
			LoadError = TRUE;
			return;
		}

// vytvoøení nové plochy
		*index = Map.New(width, height);
		if (*index < 0) return;
		if (!Map[*index].CopyWrite()) return;

// pøenesení dat plochy
		MAPITEM* srcitem = ((MAPPROG*)data)->Data;
		MAPITEM* dstitem = Map[*index].Data()->Data;

		for (int i = width * height; i > 0; i--)
		{
			if ((DWORD)srcitem->Icon < (DWORD)iconnum)
			{
				dstitem->Icon = iconindex[srcitem->Icon];
			}
			else
			{
				dstitem->Icon = 0;
				LoadError = TRUE;
			}

			dstitem->Param = srcitem->Param;

			srcitem++;
			dstitem++;
		}

// posun adresy dat
		data = (BYTE*)srcitem;

		index++;
	}

	if (data != DatEnd) LoadError = TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// naètení obrázkù

void FileLoadPic()
{
// nalezení bufferu obrázkù
	int inx = FileLoadIndex(SaveHead.piPic.Jmeno, BufPicID, 0);
	if (inx < 0) return;

// pøíprava ukazatelù
	int num = RemapIndexNum[inx];		// poèet položek k naètení
	int* index = RemapIndex[inx];		// tabulka indexù položek
	BYTE* data = DatBuf;					// ukazatel dat k naètení

// naètení hodnot z bufferu
	for (; num > 0; num--)
	{

// kontrola velikosti dat
		if ((BYTE*)(((PICPROG*)data)->Data) > DatEnd)
		{
			LoadError = TRUE;
			return;
		}

// pøíprava šíøky obrázku
		int width = ((PICPROG*)data)->Width;
		if (width < 1) width = 1;

// pøíprava výšky obrázku
		int height = ((PICPROG*)data)->Height;
		if (height < 1) height = 1;

		if (width*height <= 0)
		{
			LoadError = TRUE;
			return;
		}
		data += SIZEOFPICPROG;

// vytvoøení nového obrázku
		*index = Picture.New(width, height);
		if (*index < 0) return;
		if (!Picture[*index].CopyWrite()) return;

// pøenesení dat obrázku
		int size;
		if (DatVerze == 1)
		{
			if ((data + sizeof(long)) > DatEnd)
			{
				LoadError = TRUE;
				return;
			}

			size = *(long*)data + sizeof(long);

			if ((size < 5) || ((data + size) > DatEnd))
			{
				LoadError = TRUE;
				return;
			}

			if (KonvPalOK)
			{
				Picture[*index].CompCopy(data);
			}
			else
			{
				BYTE* data0 = (BYTE*)MemGet(width*height);
				if (data0 == NULL) return;
				DeCompress(data0, data + sizeof(long), width*height, width);
				KonvCopy(Picture[*index].Data()->Data, data0, width*height);
				MemFree(data0);
			}
		}
		else
		{
			size = width * height;

			if ((data + size) > DatEnd)
			{
				LoadError = TRUE;
				return;
			}

			KonvCopy(Picture[*index].Data()->Data, data, size);
		}

// komprese dat obrázku
		Picture[*index].Comp();

// posun adresy dat
		data += size;

		index++;
	}

	if (data != DatEnd) LoadError = TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// naètení sprajtù

void FileLoadSprite()
{
// nalezení bufferu sprajtù
	int inx = FileLoadIndex(SaveHead.piSprite.Jmeno, BufSprID, 0);
	if (inx < 0) return;

// pøíprava ukazatelù
	int num = RemapIndexNum[inx];		// poèet položek k naètení
	int* index = RemapIndex[inx];		// tabulka indexù položek
	BYTE* data = DatBuf;					// ukazatel dat k naètení

// naètení hodnot z bufferu
	for (; num > 0; num--)
	{

// kontrola velikosti dat
		if ((BYTE*)(((SPRITEPROG*)data)->Data) > DatEnd)
		{
			LoadError = TRUE;
			return;
		}

// pøíprava poètu fází sprajtu
		int faze = ((SPRITEPROG*)data)->Faze;
		if (faze < 1) faze = 1;

// pøíprava poètu smìrù sprajtu
		int smer = ((SPRITEPROG*)data)->Smer;
		if (smer < 1) smer = 1;

// vytvoøení nového sprajtu
		*index = Sprite.New(faze, smer);
		if (*index < 0) return;
		if (!Sprite[*index].CopyWrite()) return;
		SPRITEDATA* sprite = Sprite[*index].Data();	// adresa dat nového sprajtu

// poèet klidových fází
		sprite->Klid = ((SPRITEPROG*)data)->Klid;
		if (sprite->Klid < 1) sprite->Klid = 1;
		if (sprite->Klid > faze) sprite->Klid = faze;

// šíøka obrázku sprajtu
		int width = ((SPRITEPROG*)data)->Width;
		if (width < 1) width = 1;

// výška obrázku sprajtu
		int height = ((SPRITEPROG*)data)->Height;
		if (height < 1) height = 1;

		if ((width*height) <= 0)
		{
			LoadError = TRUE;
			return;
		}

// prodleva mezi fázemi
		sprite->Delay = ((SPRITEPROG*)data)->Delay;
		if (sprite->Delay < 0) sprite->Delay = 0;

// hladina k pøekreslování
		sprite->Level = ((SPRITEPROG*)data)->Level;

// poèet fází na jednotkovou vzdálenost
		sprite->Kroku = ((SPRITEPROG*)data)->Kroku;
		if (sprite->Kroku < 0) sprite->Kroku = 0;

// pøenesení obrázkù sprajtu
		BYTE* srcitem = ((SPRITEPROG*)data)->Data;
		CPicture* dstitem = sprite->Data;

		for (int size = faze * smer; size > 0; size--)
		{
			if (!dstitem->New(width, height) ||
				!dstitem->CopyWrite()) return;

			int s;

			if (DatVerze == 1)
			{
				if ((srcitem + sizeof(long)) > DatEnd)
				{
					LoadError = TRUE;
					return;
				}

				s = *(long*)srcitem;
				srcitem += sizeof(long);

				if ((s <= 0) || ((srcitem + s) > DatEnd))
				{
					LoadError = TRUE;
					return;
				}

				if (KonvPalOK)
				{
					dstitem->CompCopy(srcitem-4);
				}
				else
				{
					BYTE* data0 = (BYTE*)MemGet(width*height);
					if (data0 == NULL) return;
					DeCompress(data0, srcitem, width*height, width);
					dstitem->CopyKonvData(data0);
					MemFree(data0);
				}
			}
			else
			{
				s = width*height;

				if ((srcitem + s) > DatEnd)
				{
					LoadError = TRUE;
					return;
				}

				dstitem->CopyKonvData(srcitem);
			}

			srcitem += s;
			dstitem->Comp();
			dstitem++;
		}

// posun adresy dat
		data = srcitem;

		index++;
	}

	if (data != DatEnd) LoadError = TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// naètení zvukù

void FileLoadSound()
{
// nalezení bufferu zvukù
	int inx = FileLoadIndex(SaveHead.piSound.Jmeno, BufSndID, 0);
	if (inx < 0) return;

// pøíprava ukazatelù
	int num = RemapIndexNum[inx];	// poèet položek k naètení
	int* index = RemapIndex[inx];	// tabulka indexù položek
	BYTE* data = DatBuf;			// ukazatel dat k naètení

// naètení hodnot z bufferu
	for (; num > 0; num--)
	{

// kontrola velikosti dat
		if ((BYTE*)(((SOUNDPROG*)data)->Data) > DatEnd)
		{
			LoadError = TRUE;
			return;
		}

// pøíprava délky zvuku
		int size0 = ((SOUNDPROG*)data)->Size;
		int size = size0 + SIZEOFSOUNDPROG;
		if ((size0 < 0) || (data + size > DatEnd))
		{
			LoadError = TRUE;
			return;
		}

// vytvoøení nového zvuku (malá rezerva pro pøekryv konce pøi MP3)
		*index = Sound.New(size0 + 16);
		if (*index < 0) return;
		if (!Sound[*index].CopyWrite()) return;
		SOUNDDATA* dstdata = Sound[*index].Data();
		dstdata->Size -= 16;

// pøenesení definice zvuku
		dstdata->Samples = ((SOUNDPROG*)data)->Samples;
		dstdata->Format = ((SOUNDPROG*)data)->Format;
		dstdata->Channels = ((SOUNDPROG*)data)->Channels;
		dstdata->Bits = ((SOUNDPROG*)data)->Bits;

// pøenesení dat zvuku
		MemCopy(dstdata->Data, ((SOUNDPROG*)data)->Data, size0);

// velikost a kontrola záhlaví pro nestandardní formát
		if (dstdata->Format != WAVE_FORMAT_PCM)
		{
			if (size0 < SIZEOFWAVEFORMATEX)
			{
				dstdata->Format = WAVE_FORMAT_PCM;
				LoadError = TRUE;
			}
			else
			{
				dstdata->SizeHead = ((WAVEFORMATEX*)(dstdata->Data))->cbSize + SIZEOFWAVEFORMATEX;

				if (dstdata->SizeHead > size0)
				{
					dstdata->Format = WAVE_FORMAT_PCM;
					LoadError = TRUE;
				}
			}
		}

// korekce dat standardního formátu
		if (dstdata->Format == WAVE_FORMAT_PCM)
		{
			if (dstdata->Samples < ((11025 + 22050)/2))
			{
				dstdata->Samples = 11025;
			}
			else
			{
				if (dstdata->Samples > ((22050 + 44100)/2))
				{
					dstdata->Samples = 44100;
				}
				else
				{
					dstdata->Samples = 22050;
				}
			}
			
			if (dstdata->Channels != 2) dstdata->Channels = 1;
			if (dstdata->Bits != 16) dstdata->Bits = 8;

			dstdata->SizeHead = 0;
		}

// posun adresy dat
		data += size;

		index++;
	}

	if (data != DatEnd) LoadError = TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// naètení hudby

void FileLoadMusic()
{
// nalezení bufferu hudby
	int inx = FileLoadIndex(SaveHead.piMusic.Jmeno, BufMusID, 0);
	if (inx < 0) return;

// pøíprava ukazatelù
	int num = RemapIndexNum[inx];		// poèet položek k naètení
	int* index = RemapIndex[inx];		// tabulka indexù položek
	BYTE* data = DatBuf;					// ukazatel dat k naètení

// naètení hodnot z bufferu
	for (; num > 0; num--)
	{

// kontrola velikosti dat
		if ((data + sizeof(long)) > DatEnd)
		{
			LoadError = TRUE;
			return;
		}

// pøíprava délky hudby
		int size = *(long*)data;
		data += sizeof(long);
		if ((size < 0) || (data + size > DatEnd))
		{
			LoadError = TRUE;
			return;
		}

// vytvoøení nové hudby
		*index = Music.New(size);
		if (*index < 0) return;
		if (!Music[*index].CopyWrite()) return;

// pøenesení dat hudby
		MemCopy(Music[*index].Data()->Data, data, size);

// posun adresy dat
		data += size;

		index++;
	}

	if (data != DatEnd) LoadError = TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// naètení tabulky pro import funkcí

void FileLoadImport()
{
// nalezení bufferu importu
	int inx = FileLoadIndex(SaveHead.piImport.Jmeno, BufIntID, 0);
	if (inx < 0) return;

// pøíprava ukazatelù
	int num = RemapIndexNum[inx];		// poèet položek k naètení
	int* index = RemapIndex[inx];		// tabulka indexù položek
	char* data = (char*)DatBuf;			// ukazatel dat k naètení

// vynulování bufferu pøemapování indexù
	for (int i = 0; i < num; i++)
	{
		index[i] = IDF_COMMENT - IDF;
	}

// naètení textù z bufferu
	for (; num > 0; num--)
	{

// kontrola velikosti dat
		if ((BYTE*)(data + sizeof(BYTE)) > DatEnd)
		{
			LoadError = TRUE;
			return;
		}

// pøíprava délky hledaného textu
		int len = *(BYTE*)data;			// délka textu
		ASSERT(len >= 0);
		data++;							// zaèátek textu

// kontrola velikosti dat
		if ((BYTE*)(data + len) > DatEnd)
		{
			LoadError = TRUE;
			return;
		}

// vyhledání funkce
		if (len > 0)
		{

// !!!!!!! pøechodné - importování starých funkcí

			if ((len == 9) && MemCompare(data, "d3d_zbias", 9))
			{
				i = IDF_DIRECT3D_ZGROUP - IDF;
				*index = i;
			}
			else

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

			for (i = 0; i < FncNum; i++)
			{
				if (MemCompare(data, InitTabFunc[i].Import, len)
					&& (InitTabFunc[i].Import[len] == 0))
				{
					*index = i;
					break;
				}
			}

			if (i == FncNum) LoadError = TRUE;
		}
		else
		{
			LoadError = TRUE;
		}

		data += len;
		index++;
	}

	if ((BYTE*)data != DatEnd) LoadError = TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// naètení programového bufferu

void FileLoadProg(const char* name, int bufID)
{
// lokální promìnné
	PROGITEM item;					// ukládaná položka
	CBufProg* buf = BufProg + bufID; // buffer programu

// nalezení bufferu programu
	int inx = FileLoadIndex(name, bufID, SIZEOFPETPROG);
	if (inx < 0) return;

// pøíprava bufferu rodièù
	int parmax = 1024;				// velikost bufferu rodièù
	int parnum = 0;					// poèet rodièù v bufferu
	int* parent = (int*)MemGet(1024*sizeof(int));	// buffer rodièù
	if (parent == NULL) return;
	BOOL* next = (BOOL*)MemGet(1024*sizeof(BOOL));	// buffer pøíznakù další položky
	if (next == NULL)
	{
		MemFree(parent);
		return;
	}
	parent[0] = -1;						// inicializace první položky

// pøíprava ukazatelù
	int num = RemapIndexNum[inx];		// poèet položek k naètení
	int* index = RemapIndex[inx];		// tabulka indexù položek
	PETPROG* data = (PETPROG*)DatBuf;		// ukazatel dat k naètení

// naètení položek z bufferu
	for (; num > 0; num--)
	{

// parametry prvku
		item.Param = 0;
		if (data->Param & PETPROG_EXP)		item.Param |= PR_EXP;
		if (data->Param & PETPROG_LOCK)		item.Param |= PR_LOCK;
		if (data->Param & PETPROG_OFF)		item.Param |= PR_OFF;
		if (data->Param & PETPROG_NOMOVE)	item.Param |= PR_NOMOVE;
		if (data->Param & PETPROG_INTERN)	item.Param |= PR_INTERN;
		if (data->Param & PETPROG_OFF_DEP)	item.Param |= PR_OFF_DEP;

// nastavení pøíznaku další položky
		next[parnum] = (data->Param & PETPROG_NEXT);

// deklaraèní blok a index
		int blok = data->RefBlok;
		int n = data->RefIndex;
		item.RefBlok = -1;
		item.RefIndex = -1;

// pøemapování deklarace
		if (((DWORD)blok < (DWORD)RemapBlokNum) &&
			((DWORD)n < (DWORD)RemapIndexNum[blok]))
		{
			item.RefBlok = RemapBlok[blok];
			item.RefIndex = RemapIndex[blok][n];
		}

// pøi odkazu na interní prvek zmìna na buffer struktur nebo tøíd
		if (item.RefBlok == BufIntID)
		{
			item.RefBlok = ImportBlok[item.RefIndex];
			item.RefIndex = ImportIndex[item.RefIndex];
		}

// zdìdìní parametrù
		if (((DWORD)item.RefBlok < (DWORD)PROGBUFNUM) &&
			BufProg[item.RefBlok].IsValid(item.RefIndex))
		{
			item.Func = BufProg[item.RefBlok][item.RefIndex].Func;
		}
		else
		{
			item.Func = IDF_COMMENT;
			item.RefBlok = ImportBlok[IDF_COMMENT - IDF];
			item.RefIndex = ImportIndex[IDF_COMMENT - IDF];
			LoadError = TRUE;
		}

		ASSERT (((DWORD)item.RefBlok < (DWORD)PROGBUFNUM) && BufProg[item.RefBlok].IsValid(item.RefIndex));

		item.SrcMask = BufProg[item.RefBlok][item.RefIndex].SrcMask;
		item.DstMask = BufProg[item.RefBlok][item.RefIndex].DstMask;
		item.Param |= BufProg[item.RefBlok][item.RefIndex].Param & (PR_ONE | PR_INTERN | PR_NOMOVE | PR_SETPAR | PR_PARPAR);

// korekce parametrù indexu seznamu
		if ((item.Func == IDF_LIST_INDEX) ||
			(item.Func == IDF_LIST_AUTO) ||
			(item.Func == IDF_LIST_SIZE) ||
			(item.Func == IDF_SPRITE_X) ||
			(item.Func == IDF_SPRITE_Y) ||
			(item.Func == IDF_SPRITE_SMER) ||
			(item.Func == IDF_SPRITE_MOVE) ||
			(item.Func == IDF_SPRITE_VISIBLE) ||
			(item.Func == IDF_SPRITE_FAZE) ||
			(item.Func == IDF_SPRITE_STATUS))
		{
			item.Param &= ~PR_NOMOVE;
			item.Param |= PR_INTERN;
		}

// v editoru se ruší interní pøíznak
		if (bufID == BufEdiID)
		{
			item.Param &= ~PR_INTERN;
		}

// definièní blok a index
		blok = data->DatBlok;
		n = data->DatIndex;
		item.DatBlok = -1;
		item.DatIndex = -1;

// pøemapování definice
		if (((DWORD)blok < (DWORD)RemapBlokNum) &&
			((DWORD)n < (DWORD)RemapIndexNum[blok]))
		{
			item.DatBlok = RemapBlok[blok];
			item.DatIndex = RemapIndex[blok][n];
		}

// ikona
		item.Icon = -1;
		if (IconInx >= 0)
		{
			n = data->Icon;
			if ((DWORD)n < (DWORD)RemapIndexNum[IconInx])
			{
				item.Icon = RemapIndex[IconInx][n];
			}
		}

// text
		item.Name = -1;
		if (TextInx >= 0)
		{
			n = data->Name;
			if ((DWORD)n < (DWORD)RemapIndexNum[TextInx])
			{
				item.Name = RemapIndex[TextInx][n];
			}
		}

// vložení prvku do bufferu
		*index = buf->Insert(&item, parent[parnum], -2);
		if (*index < 0)
		{
			MemFree(parent);
			MemFree(next);
			return;
		}

// pokud budou potomci, zvýšení èítaèe rodièù
		if (data->Param & PETPROG_CHILDS)
		{
			parnum++;
			if (parnum >= parmax)
			{
				parmax += 1024;
				int* par = (int*)MemSize(parent, parmax * sizeof(int));
				if (par == NULL)
				{
					MemFree(parent);
					MemFree(next);
					return;
				}
				parent = par;

				BOOL* nxt = (BOOL*)MemSize(next, parmax * sizeof(BOOL));
				if (nxt == NULL)
				{
					MemFree(parent);
					MemFree(next);
					return;
				}
				next = nxt;
			}
			parent[parnum] = *index;
			next[parnum] = TRUE;
		}

// snížení èítaèe rodièù
		else
		{
			while (!next[parnum] && (parnum > 0))
			{
				parnum--;
			}
		}

// zvýšení ukazatelù
		data++;
		index++;
	}

	if ((BYTE*)data != DatEnd) LoadError = TRUE;

// zrušení bufferu rodièù
	MemFree(parent);
	MemFree(next);
}


/////////////////////////////////////////////////////////////////////////////
// naètení programu

void Load()
{
// kontrola, zda již probíhá naèítání (nebo ukládání)
	if (Saving) return;

// zapnutí pøíznaku naèítání
	Saving = TRUE;

// není chyba pøi naèítání
	LoadError = FALSE;

#ifdef _LITE
	LiteError = FALSE;
#endif // _LITE

// zapnutí kurzoru èekání
	BeginWaitCursor();

// pøednastavení, že není žádný blok (pro koncové nulování)
	RemapBlokNum = 0;

// pøednastavení, že není buffer s ikonami
	IconInx = -1;			// není buffer s ikonami
	TextInx = -1;			// není buffer s texty

// vyprázdnìní bufferù (datové buffery jsou již pøipraveny!)
	BufObj.DelAll();
	BufLoc.DelAll();
	BufEdi.DelAll();

// pøíprava jména souboru
	FileName = JmenoLoad;

// otevøení souboru
	hFile = ::CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

// pokraèování jen pøi platném souboru
	if (hFile != INVALID_HANDLE_VALUE)
	{

// inicializace konverzní tabulky importù
		if (InitImportTab())
		{

// naètení záhlaví souboru
			if (FileLoadHead())
			{

// naètení palet
				FileLoadPalet();

// naètení èísel
				FileLoadReal();

// naètení textù
				FileLoadText();

// naètení logických hodnot
				FileLoadBool();

// naètení ikon
				FileLoadIcon();

// naètení ploch
				FileLoadMap();

// naètení obrázkù
				FileLoadPic();

// naètení sprajtù
				FileLoadSprite();

// naètení zvukù
				FileLoadSound();

// naètení hudby
				FileLoadMusic();

// naètení tabulky importu funkcí
				FileLoadImport();

// pøednastavení bufferu editoru (kvùli odkazùm z lokálního bufferu)
				int inx = FileLoadIndex(SaveHead.piProgram.Jmeno, BufEdiID, SIZEOFPETPROG);
				if (inx >= 0)
				{
					int* index = RemapIndex[inx];
					inx = RemapIndexNum[inx] - 1;
					index += inx;
					for (; inx >= 0; inx--)
					{
						*index = inx;
						index--;
					}
				}

// naètení bufferu lokálních funkcí
				FileLoadProg(SaveHead.piLocal.Jmeno, BufLocID);

// naètení bufferu globálních funkcí
				FileLoadProg(SaveHead.piGlobal.Jmeno, BufObjID);

// naètení bufferu definice funkcí
				FileLoadProg(SaveHead.piProgram.Jmeno, BufEdiID);

// reference ikon v plochách
				int im;

				for (im = BufObj.Max()-1; im >= 0; im--)
				{
					if (BufObj.IsValid(im) &&
						(BufObj[im].DatBlok == BufMapID) && 
						Map.IsValid(BufObj[im].DatIndex))
					{
						Map[BufObj[im].DatIndex].RefInc();
					}
				}

				for (im = BufLoc.Max()-1; im >= 0; im--)
				{
					if (BufLoc.IsValid(im) &&
						(BufLoc[im].DatBlok == BufMapID) && 
						Map.IsValid(BufLoc[im].DatIndex))
					{
						Map[BufLoc[im].DatIndex].RefInc();
					}
				}
			}
		}

// uzavøení souboru
		::CloseHandle(hFile);

// chyba ètení souboru (je možné že bude hlásit poškození i pøi chybì pamìti)
		if (LoadError)
		{
			::DialogBoxParam(
				hInstance,
				MAKEINTRESOURCE(IDN_READERR),
				MainFrame,
				(DLGPROC)AboutBoxProc,
				(LPARAM)IDN_READERR);
		}
	}

// chyba otevøení souboru
	else
	{
		::DialogBoxParam(
			hInstance,
			MAKEINTRESOURCE(IDN_OPENERR),
			MainFrame,
			(DLGPROC)AboutBoxProc,
			(LPARAM)IDN_OPENERR);
	}

// zrušení bufferù
	for (int i = 0; i < RemapBlokNum; i++)
	{
		MemFree(RemapIndex[i]);		RemapIndex[i] = NULL;
	}
	MemFree(RemapBlok);		RemapBlok = NULL;
	MemFree(RemapIndexNum);	RemapIndexNum = NULL;
	MemFree(RemapIndex);	RemapIndex = NULL;
	MemFree(DatBuf);		DatBuf = NULL;

// vypnutí kurzoru èekání
	EndWaitCursor();

// vypnutí pøíznaku naèítání
	Saving = FALSE;
}

}


/////////////////////////////////////////////////////////////////////////////
// inicializaèní tabulka okna editoru

const INITTREE InitTabEdi[] = {
	0,			IDF_FNC,
};

/////////////////////////////////////////////////////////////////////////////
// inicializaèní tabulka okna lokálních objektù

const INITTREE InitTabLoc[] = {
	0,			IDF_FNC,
	1,			IDF_PAR,
	2,			IDF_COMMAND_LINE,
	1,			IDF_OUT,
	2,			IDF_RETURN_CODE,
};

const INITTREE InitTabLoc0[] = {
	0,			IDF_FNC,
	1,			IDF_PAR,
	1,			IDF_OUT,
};

/////////////////////////////////////////////////////////////////////////////
// inicializaèní tabulka okna globálních objektù

const INITTREE InitTabObj[] = {
	0,			IDF_GROUP,
	1,			IDF_FNC,
	1,			IDF_MAP,
	1,			IDF_NONE,
	1,			IDF_SPRITE,
	1,			IDF_SPRITE,
};


/////////////////////////////////////////////////////////////////////////////
// kontrola programu po naètení s opravou (vrací FALSE=chyba pamìti)

BOOL ProgramCheck()
{
	int i;

// zajištìní hlavní funkce v oknì editoru
	if (BufEdi.IsNotValid(0))
	{
		BufEdi.InitTab(InitTabEdi, sizeof(InitTabEdi)/sizeof(InitTabEdi[0]));
		if (BufEdi.IsNotValid(0)) return FALSE;
		BufEdi[0].DstMask = PR_COMMAND;
	}
	ASSERT(BufEdi.First() == 0);

// korekce ROOT položek v editoru
	for (i = 0; BufEdi.IsValid(i); i = BufEdi[i].Next)
	{
		PROGITEM* item = &BufEdi[i];

		int b = item->RefBlok;
		int x = item->RefIndex;

		if (((DWORD)b < (DWORD)PROGBUFNUM) && BufProg[b].IsValid(x))
		{
			BufProg[b][x].Refer--;
		}

		item->Func = IDF_FNC;
		item->RefBlok = BufIntID;
		item->RefIndex = IDF_FNC - IDF;
		item->SrcMask = 0;
		item->DstMask = PR_COMMAND;
		item->Param = PR_EXP | PR_NOMOVE;
		BufInt[IDF_FNC - IDF].Refer++;

		if (BufLoc.SrcDat(BufEdiID, i) < 0)
		{
			int ii = BufLoc.Num();
			if (ii == 0)
			{
				if (!BufLoc.InitTab(InitTabLoc, sizeof(InitTabLoc)/sizeof(InitTabLoc[0]))) return FALSE;
				if (BufLoc.IsNotValid(ii)) return FALSE;

				BufLoc[ii].DstMask = PR_ALLDATA;
				BufLoc[ii].DatBlok = BufEdiID;				// hlavní lokální funkce
				BufLoc[ii].DatIndex = i;					// index funkce v editoru
				BufLoc[ii].Param = PR_EXP | PR_NOMOVE;

				BufLoc[ii+1].Param |= PR_EXP | PR_ONE;
				BufLoc[ii+2].Param |= PR_INTERN;
				BufLoc[ii+3].Param |= PR_EXP;
				BufLoc[ii+4].Param |= PR_INTERN;
			}
			else
			{
				if (!BufLoc.InitTab(InitTabLoc0, sizeof(InitTabLoc0)/sizeof(InitTabLoc0[0]))) return FALSE;
				if (BufLoc.IsNotValid(ii)) return FALSE;

				BufLoc[ii].DstMask = PR_ALLDATA;
				BufLoc[ii].DatBlok = BufEdiID;				// hlavní lokální funkce
				BufLoc[ii].DatIndex = i;					// index funkce v editoru
				BufLoc[ii].Param = PR_EXP | PR_NOMOVE;

				BufLoc[ii+1].Param |= PR_EXP | PR_ONE;
				BufLoc[ii+2].Param |= PR_EXP;
			}
		}
	}

	ASSERT(BufLoc.First() == 0);

// korekce ROOT položek v lokálním bufferu
	for (i = 0; BufLoc.IsValid(i); i = BufLoc[i].Next)
	{
		PROGITEM* item = &BufLoc[i];

		int b = item->RefBlok;
		int x = item->RefIndex;

		if (((DWORD)b < (DWORD)PROGBUFNUM) && BufProg[b].IsValid(x))
		{
			BufProg[b][x].Refer--;
		}

		item->Func = IDF_FNC;
		item->RefBlok = BufIntID;
		item->RefIndex = IDF_FNC - IDF;
		item->DstMask = PR_ALLDATA;
		item->Param = PR_EXP | PR_NOMOVE;
		BufInt[IDF_FNC - IDF].Refer++;

		if ((item->DatBlok != BufEdiID) || (BufEdi.IsNotValid(item->DatIndex)))
		{
			int ii = BufEdi.Num();
			BufEdi.InitTab(InitTabEdi, sizeof(InitTabEdi)/sizeof(InitTabEdi[0]));
			if (BufEdi.IsNotValid(ii)) return FALSE;
			BufEdi[ii].DstMask = PR_COMMAND;
			item->DatBlok = BufEdiID;
			item->DatIndex = ii;
		}

		if (BufObj.SrcDat(BufLocID, i) < 0)
		{
			if (i == 0)
			{
				int ii = BufObj.Num();

				if (!BufObj.InitTab(InitTabObj, sizeof(InitTabObj)/sizeof(InitTabObj[0]))) return FALSE;
				if (BufObj.IsNotValid(ii)) return FALSE;

				Icon.Load(IDN_MAINFRAME, 1);				// naètení ikony Petøíka
				ASSERT(Icon.IsValid(FncNum));
				BufObj[ii+1].Icon = FncNum;

				Text.Add(Jmeno);								// pøidání jména programu
				ASSERT(Text.IsValid(FncNum));
				BufObj[ii+1].Name = FncNum;

				BufObj[ii+1].DatBlok = BufLocID;				// odkaz na hlavní lokální definici
				BufObj[ii+1].DatIndex = 0;						// index hlavní lokální definice
				BufObj[ii+1].RefBlok = BufClsID;				// reference na buffer tøíd
				BufObj[ii+1].RefIndex = CLASSFNCINDEX;			// reference na funkci v bufferu tøíd
				BufCls[CLASSFNCINDEX].Refer++;
				BufObj[ii+1].Param |= PR_NOMOVE;				// zákaz kopie hlavní funkce

				BufObj[ii].Param |= PR_NOMOVE | PR_EXP;		// zákaz kopie hlavní skupiny
				BufObj[ii].Name = Text.Load(IDN_ZAKLADNI_OBJEKTY);
				BufObj[ii].RefBlok = BufStrID;
				BufObj[ii].RefIndex = GroupIndex;
				BufStr[GroupIndex].Refer++;

				BufObj[ii+2].Param |= PR_INTERN;
				BufObj[ii+2].DatBlok = BufMapID;
				BufObj[ii+2].DatIndex = Map.New(20, 15);		// nová plocha, plochu vymaže
				BufObj[ii+2].RefBlok = BufClsID;				// reference na buffer tøíd
				BufObj[ii+2].RefIndex = CLASSMAPINDEX;			// reference na plochu v bufferu tøíd
				BufCls[CLASSMAPINDEX].Refer++;
				BufObj[ii+2].DatIndex = 0;

				BufObj[ii+3].Param |= PR_INTERN;
				BufObj[ii+3].DatBlok = BufIcoID;
				BufObj[ii+3].DatIndex = 0;
				BufObj[ii+3].Icon = 0;
				BufObj[ii+3].Name = 0;
				BufObj[ii+3].RefBlok = BufClsID;				// reference na buffer tøíd
				BufObj[ii+3].RefIndex = CLASSICONINDEX;		// reference na ikonu v bufferu tøíd
				BufCls[CLASSICONINDEX].Refer++;
				ASSERT(Icon.IsValid(0));

				BufObj[ii+4].Param |= PR_INTERN;
				BufObj[ii+4].Icon = Icon.Dup(IDF_PETRIK - IDF);
				BufObj[ii+4].Name = Text.Dup(IDF_PETRIK - IDF);
				BufObj[ii+4].DatBlok = BufSprID;
				Sprite.Add(Sprite0);
				BufObj[ii+4].DatIndex = 0;
				BufObj[ii+4].RefBlok = BufClsID;
				BufObj[ii+4].RefIndex = CLASSSPRITEINDEX;
				BufCls[CLASSSPRITEINDEX].Refer++;

				BufObj[ii+5].Param |= PR_INTERN;
				BufObj[ii+5].Icon = Icon.Dup(IDF_PETRA - IDF);
				BufObj[ii+5].Name = Text.Dup(IDF_PETRA - IDF);
				BufObj[ii+5].DatBlok = BufSprID;
				Sprite.Add(Sprite1);
				BufObj[ii+5].DatIndex = 1;
				BufObj[ii+5].RefBlok = BufClsID;
				BufObj[ii+5].RefIndex = CLASSSPRITEINDEX;
				BufCls[CLASSSPRITEINDEX].Refer++;
			}
			else
			{
				int ii = BufObj.Insert(&BufInt[IDF_FNC - IDF], -1, -2);
				if (BufObj.IsNotValid(ii)) return FALSE;

				BufObj[ii].Name = -1;
				BufObj[ii].Icon = -1;
				BufObj[ii].DatBlok = BufLocID;
				BufObj[ii].DatIndex = i;
				BufObj[ii].RefBlok = BufClsID;				// reference na buffer tøíd
				BufObj[ii].RefIndex = CLASSFNCINDEX;			// reference na funkci v bufferu tøíd
				BufCls[CLASSFNCINDEX].Refer++;
			}
		}
	}

// korekce ROOT položek v globálním bufferu
	for (i = 0; BufObj.IsValid(i); i = BufObj[i].Next)
	{
		PROGITEM* item = &BufObj[i];

		if ((item->Func == IDF_LIST_SIZE) ||
			(item->Func == IDF_LIST_INDEX) ||
			(item->Func == IDF_LIST_AUTO))
		{
			item->Param &= ~(PR_NOMOVE | PR_INTERN);
		}
	}

	return TRUE;
}
