
#include "Main.h"

//////////////////////////////////////////////////////////////////////////////
// inicializa�n� segmenty CRT (konstruktory a destruktory)

typedef void (__cdecl *_PVFV)(void);		// ukazatel na funkci void fnc(void)
typedef void (*PFV)(void);					// funkce void fnc(void)

#pragma data_seg(".CRT$XCA")
_PVFV	__xc_a[] = { NULL };				// konstruktory C++
#pragma data_seg(".CRT$XCZ")
_PVFV	__xc_z[] = { NULL };

#pragma data_seg()							// resetov�n� na b�nou datovou sekci

#pragma comment(linker, "/MERGE:.CRT=.data")	// p�ipojen� CRT sekc� do datov� sekce


//////////////////////////////////////////////////////////////////////////////
// glob�ln� prom�nn�

CString		CommandLine;				// p��kazov� ��dek
int			VerzeOS;					// verze syst�mu
HINSTANCE	hInstance = NULL;			// instance programu

BYTE*		StdPalImport;				// tabulka importu palet
BYTE*		StdPalImportDither;			// tabulka importu palet s ditheringem
BITMAPINFO* StdBitmapInfo;				// standardn� z�hlav� BMP
BYTE*		KonvPal;					// konverzn� tabulka palet


bool	ConsoleOn = false;		// p��znak re�imu konzoly
HANDLE	ConIn = INVALID_HANDLE_VALUE;	// handle pro vstup z konzoly
HANDLE	ConOut = INVALID_HANDLE_VALUE;	// handle pro v�stup na konzolu
HANDLE	ConErr = INVALID_HANDLE_VALUE;	// handle pro chybov� v�stup na konzolu

bool	Dither = true;					// pou��t dithering

//////////////////////////////////////////////////////////////////////////////
// pr�zdn� objekty (nap�. pro n�vrat neplatn� polo�ky z funkce)

CString		EmptyString;				// pr�zdn� �et�zec
CPicture	EmptyPicture;				// pr�zdn� obr�zek
CSprite		EmptySprite;				// pr�zdn� sprajt

//////////////////////////////////////////////////////////////////////////////
// konstanty

const char*	Compiled =	"Gemtree 32-compiler"; // jen tak pro zmaten�

// tabulka hladin standardn�ch barev (rozsah 0 a� 256)
const int ColLevTab[] = 
{
	64*4,
	54*4,
	44*4,
	33*4,
	22*4,
	11*4,
};

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// po zm�n� palet zru� soubor PALIMP.DAT a spus� DEBUG verzi pro nov� vygenerov�n�
// (pozor - generov�n� m��e trvat des�tky sekund). Potom znovu p�eklad.
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// tabulka odst�n� standardn�ch barev (0 a� 255) - po�ad� B, G, R, F,
const RGBQUAD ColColTab[] = 
{ 
	0,		0,		255,		0,			//  0: �erven�
	72,		72,		255,		0,			//  1:
	116,	116,	255,		0,			//  2:
	158,	166,	255,		0,			//  3:
	210,	220,	255,		0,			//  4:
	0,		114,	255,		0,			//  5: hn�d�
	150,	198,	255,		0,			//  6:
	0,		182,	255,		0,			//  7: oran�ov�
	96,		192,	255,		0,			//  8:
	0,		255,	255,		0,			//  9: �lut�
	128,	255,	255,		0,			// 10:
	192,	255,	255,		0,			// 11: sv�tle �lut�
	0,		255,	178,		0,			// 12: �lutozelen�
	0,		255,	0,			0,			// 13: zelen�
	128,	255,	128,		0,			// 14:
	192,	255,	192,		0,			// 15:
	146,	255,	0,			0,			// 16: namodrale zelen�
	222,	255,	138,		0,			// 17:
	255,	255,	0,			0,			// 18: modrozelen�
	255,	186,	0,			0,			// 19:
	255,	222,	124,		0,			// 20:
	255,	128,	0,			0,			// 21:
	255,	0,		0,			0,			// 22: modr�
	255,	80,		72,			0,			// 23:
	255,	144,	128,		0,			// 24:
	255,	192,	154,		0,			// 25:
	255,	238,	210,		0,			// 26:
	255,	0,		114,		0,			// 27:
	255,	0,		174,		0,			// 28:
	255,	162,	198,		0,			// 29:
	255,	0,		255,		0,			// 30: fialov�
	255,	128,	255,		0,			// 31: sv�tle fialov�
	138,	0,		255,		0,			// 32: fialov� �erven�
	192,	128,	255,		0,			// 33:
	210,	186,	255,		0,			// 34:

	255,	255,	255,		0,			// 35: b�l� (nepou�ije se, vygeneruje se)
	96,		96,		96,			0,			// 36: �ed� (nepou�ije se, vygeneruje se)
};

const int ColLev = sizeof(ColLevTab)/sizeof(ColLevTab[0]);	// po�et hladin barev
const int ColCol = sizeof(ColColTab)/sizeof(ColColTab[0]);	// po�et odst�n� barev
const int StdColors = ResCols + ColCol*ColLev;			// po�et vlastn�ch palet (za��naj� od 0)
const BYTE WhiteCol = StdColors - 2*ColLev;			// b�l� barva
const BYTE BlackCol = StdColors - 1;				// �ern� barva

//////////////////////////////////////////////////////////////////////////////
// lok�ln� prom�nn�

#ifdef _MT
static	CRITICAL_SECTION	ExitCriticalSection;	// kritick� sekce pro konec programu
#endif

//CString UvText(  _T("Pov2Spr v1.0 - konverze obr�zk� na sprajt; (c) Miroslav N�me�ek\n"));
//CString HelpText(_T("   POV2SPR vstup v�stup sm�r� klid pohyb prodleva hladina krok dither\n")
//				 _T("               vstup .... vstupn� soubor BMP prvn�ho obr�zku\n")
//				 _T("               v�stup ... v�stupn� soubor SPR sprajtu\n")
//				 _T("               sm�r� .... po�et sm�r� 0 a� 1000\n")
//				 _T("               klid ..... po�et klidov�ch f�z� 1 a� 1000\n")
//				 _T("               pohyb .... po�et f�z� pohybu 0 a� 1000\n")
//				 _T("               prodleva . prodleva mezi f�zemi v milisekund�ch\n")
//				 _T("               hladina .. hladina k zobrazen�, 0=p�edm�ty\n")
//				 _T("               krok ..... po�et f�z� na jednotkovou vzd�lenost\n")
//				 _T("               dither ... pou��t dithering 1=ano, 0=ne\n")
//				 _T("stiskn�te <Enter>... "));
//
//CString WriteErr1(_T("Chyba z�pisu do v�stupn�ho souboru "));
//CString WriteErr2(_T("!\n"));
//
//CString ReadErr1(_T("Chyba �ten� ze vstupn�ho souboru "));
//CString ReadErr2(_T("!\n"));

CString UvText(  _T("Pov2Spr v1.1 - pictures to sprite conversion; (c) Ing. Miroslav Nemecek\n"));
CString HelpText(_T("   POV2SPR in out dir still move delay level step dither\n")
				 _T("               in ...... input file BMP of the first picture\n")
				 _T("               out ..... output file SPR of the sprite\n")
				 _T("               dir ..... number of directions 0 to 1000\n")
				 _T("               still ... number of standstill phases 1 to 1000\n")
				 _T("               move .... number of moving phases 0 to 1000\n")
				 _T("               delay ... delay between phases in milliseconds\n")
				 _T("               level ... level to be displayed, 0=items\n")
				 _T("               step .... number of phases per one-step distance\n")
				 _T("               dither .. use dithering 1=yes, 0=no\n")
				 _T("press <Enter>... "));

CString WriteErr1(_T("Error writing into output file "));
CString WriteErr2(_T("!\n"));

CString ReadErr1(_T("Error reading from input file "));
CString ReadErr2(_T("!\n"));

CString InName;				// vstupn� soubor
CString OutName;			// v�stupn� soubor
CPicture Picture;			// vstupn� obr�zek
CSprite Sprite;				// v�stupn� sprajt

/***************************************************************************\
*																			*
*									konzola									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// zapnut�/vypnut� konzoly

void ConsoleOnSet(bool on)
{
	if (ConsoleOn == on) return;
	ConsoleOn = on;

// zapnut� konzoly
	if (on)
	{
		if ((ConIn == INVALID_HANDLE_VALUE) || (ConIn == NULL))
		{
			ConIn = ::GetStdHandle(STD_INPUT_HANDLE);
		}

		if ((ConOut == INVALID_HANDLE_VALUE) || (ConOut == NULL))
		{
			ConOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
		}

		if ((ConErr == INVALID_HANDLE_VALUE) || (ConErr == NULL))
		{
			ConErr = ::GetStdHandle(STD_ERROR_HANDLE);
		}

	}
}


/////////////////////////////////////////////////////////////////////////////
// v�stup textu na konzolu

void ConsoleOut(const CString& text)
{
	ConsoleOnSet(true);
		DWORD writen;
		int len = text.Length();
		char* buf = (char*)MemGet(len);
		::CharToOemBuff(text.DataData(), buf, len);
		::WriteFile(ConOut, buf, len, &writen, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// v�stup chybov�ho textu na konzolu

void ConsoleErr(const CString& text)
{
	ConsoleOnSet(true);
		DWORD writen;
		int len = text.Length();
		char* buf = (char*)MemGet(len);
		::CharToOemBuff(text.DataData(), buf, len);
		::WriteFile(ConErr, buf, len, &writen, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// vstup textu z konzoly

void ConsoleIn(CString& text)
{
	ConsoleOnSet(true);
		DWORD readen = 0;
		char* bufoem = (char*)MemGet(0x2000);
		::ReadFile(ConIn, bufoem, 0x2000, &readen, NULL);
		TCHAR* buf = (TCHAR*)MemGet(readen * sizeof(TCHAR));
		::OemToCharBuff(bufoem, buf, readen);
		text.Term();
		text.Init(buf, readen);

		while (text.IsNotEmpty() && ((text[text.Length()-1] == 13) ||
				(text[text.Length()-1] == 10)))
		{
			text.Delete(text.Length()-1);
		}
}

CString CommandPar(int i)
{
	CString text;

	int pos = 0;
	int beg = 0;
	int len = CommandLine.Length();

// nalezen� parametru
	for (; i >= 0; i--)
	{

// nalezen� za��tku parametru
		while (	(pos < len) && 
				(CommandLine[pos] <= _T(' ')) && 
				(CommandLine[pos] > 0)) 
		{
			pos++;
		}
		beg = pos;

// nalezen� konce parametru
		bool uvoz = false;
		while (	(pos < len) &&
				(	(CommandLine[pos] < 0) ||
					(CommandLine[pos] > _T(' ')) ||
					uvoz
				) )
		{
			if (CommandLine[pos] == _T('"')) uvoz = !uvoz;
			pos++;
		}
	}

// p�enesen� parametru
	text = CommandLine.Mid(beg, pos-beg);

// zru�en� uvozovek
	pos = 0;
	len = text.Length();
	while (pos < len)
	{
		if (text[pos] == _T('"'))
		{
			text.Delete(pos, 1);
			len--;
			if ((pos < len) && (text[pos] != _T('"'))) pos--;
		}
		pos++;
	}

	return text;
}

//////////////////////////////////////////////////////////////////////////////
// inicializace �seku inicializa�n�ch/termina�n�ch funkc�

void InitTerm(_PVFV* beg, _PVFV* end)
{
	for (; beg < end; beg++)
	{
		if ((*beg != NULL) && (*beg != (PFV)-1))
		{
			(**beg)();
		}
	}
}


//////////////////////////////////////////////////////////////////////////////
// hlavn� start programu

void mainCRTStartup()
{
// �schova verze syst�mu
	VerzeOS = (int)::GetVersion();

// handle instance programu
	hInstance = ::GetModuleHandle(NULL);

// inicializace uzamyk�n� ukon�en� programu
#ifdef _MT
	::InitializeCriticalSection(&ExitCriticalSection);
#endif

// inicializace spr�vce pam�ti
	if (!MemInit()) 
	{
		Exit(EXITCODE_MEMERR);
	}

// inicializace koprocesoru
#ifdef _M_IX86
	WORD stat;
	_asm {
		wait						// synchronizace
		fnstcw		stat			// ulo�en� ��dic�ho slova
		wait						// synchronizace
		mov			ax,stat			// stav ��dic�ho slova
		and			ah,not 0xc		// implicitn� zaokrouhlov�n�
		or			ah,3			// p�esnost 64 bit�
		mov			stat,ax			// nov� stav ��dic�ho slova
		fldcw		stat			// nastaven� nov�ho ��dic�ho slova
	}
#endif

// inicializace dat pr�zdn� ikony a pr�zdn�ho obr�zku
	EmptyPictureData.Data = (BYTE*)MemGet(ICONSIZE);
	MemFill(EmptyPictureData.Data, ICONSIZE, BackCol);

// inicializace obsluhy sprajt�
	InitSprite();						// statick� inicializace sprajt�

// inicializace glob�ln�ch objekt�
	InitTerm(__xc_a, __xc_z);

// inicializace standardn�ho z�hlav� BMP
	StdBitmapInfo = (BITMAPINFO*) MemGet(sizeof(BITMAPINFO) + sizeof(RGBQUAD)*255);
	MemFill(StdBitmapInfo, sizeof(BITMAPINFO) + sizeof(RGBQUAD)*255);	// vynulov�n�
	StdBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);	// velikost z�hlav�
	StdBitmapInfo->bmiHeader.biWidth = ICONWIDTH;	// ���ka bitmapy
	StdBitmapInfo->bmiHeader.biHeight = ICONHEIGHT;	// v��ka bitmapy
	StdBitmapInfo->bmiHeader.biPlanes = 1;			// po�et barevn�ch rovin
	StdBitmapInfo->bmiHeader.biBitCount = 8;		// po�et bit� na bod
	StdBitmapInfo->bmiHeader.biCompression = BI_RGB; // nen� komprese
	StdBitmapInfo->bmiHeader.biClrImportant = StdColors; // po�et d�le�it�ch palet

// vytvo�en� standardn�ch palet
	RGBQUAD* rgb = StdBitmapInfo->bmiColors;		// za��tek tabulky palet

// nem�nit paletu barvu pozad� - pou��v� se p�i importu obr�zk�
	rgb->rgbRed =	BACKCOLOR_RED;					// pr�hledn� barva
	rgb->rgbGreen =	BACKCOLOR_GREEN;
	rgb->rgbBlue =	BACKCOLOR_BLUE;
	rgb++;

	rgb->rgbRed =	SHADCOLOR_RED;					// barva st�nu
	rgb->rgbGreen =	SHADCOLOR_GREEN;
	rgb->rgbBlue =	SHADCOLOR_BLUE;
	rgb++;

	int i,j,k;
	for (i = 0; i < (ColCol-2); i++)				// pro v�echny barvy bez b�l� a �ed�
	{
		*rgb = ColColTab[i];						// p�enesen� z�kladn� barvy

		for (j = 1; j < ColLev; j++)				// pro v�echny odst�ny
		{
			k = ColLevTab[j];						// n�sob�c� koeficient
			rgb[j].rgbRed = (BYTE)(rgb->rgbRed*k/256);		// �erven�
			rgb[j].rgbGreen = (BYTE)(rgb->rgbGreen*k/256);	// zelen�
			rgb[j].rgbBlue = (BYTE)(rgb->rgbBlue*k/256);	// modr�
		}
		rgb += ColLev;								// adresa dal�� barvy
	}

	i = WhiteCol;									// index b�l� barvy
	for (; i <= BlackCol; i++)						// od b�l� barvy a� po �ernou
	{
		k = (BlackCol-i)*255/(2*ColLev-1);			// odst�n b�l� barvy
		rgb->rgbRed = (BYTE)k;						// �erven�
		rgb->rgbGreen = (BYTE)k;					// zelen�
		rgb->rgbBlue = (BYTE)k;						// modr�
		rgb++;										// zv��en� adresy barvy
	}

// vygenerov�n� tabulky pro import palet a tabulky palet (pro konverzi BMP)
	StdPalImport = (BYTE*)MemGet(64 * 64 * 64);		// tabulka pro import palet
	StdPalImportDither = (BYTE*)MemGet(64 * 64 * 64 * 4);	// tabulka pro import palet s dithering

// na�ten� tabulky importu palet
	HRSRC hRes = ::FindResource(hInstance, MAKEINTRESOURCE(IDN_PALIMP), _T("LOADER"));
	HGLOBAL hData = ::LoadResource(hInstance, hRes);
	ASSERT((hRes != NULL) && (hData != NULL));
	if ((hRes == NULL) || (hData == NULL)) Exit(EXITCODE_LOADRES);
	DeKomp(StdPalImport, 64*64*64, (BYTE*)::LockResource(hData)+6, ::SizeofResource(hInstance, hRes)-6);

	hRes = ::FindResource(hInstance, MAKEINTRESOURCE(IDN_PALIMPD), _T("LOADER"));
	hData = ::LoadResource(hInstance, hRes);
	ASSERT((hRes != NULL) && (hData != NULL));
	if ((hRes == NULL) || (hData == NULL)) Exit(EXITCODE_LOADRES);
	DeKomp(StdPalImportDither, 64*64*64*4, (BYTE*)::LockResource(hData)+6, ::SizeofResource(hInstance, hRes)-6);

// vytvo�en� konverzn� tabulky palet pro �mport soubor� BMP
	KonvPal = (BYTE*)MemGet(256);			// konverzn� tabulka palet

// �schova p��kazov�ho ��dku
	ConsoleOut(UvText);
	CommandLine = ::GetCommandLine();

#ifdef _DEBUG
	if (CommandPar(1).IsEmpty())
	{
		CommandLine = _T("Pov2Spr AutoB01.BMP AutoB.SPR 64 1 0 55 1000 8");
	}
#endif

	if (CommandPar(9).IsEmpty() || CommandPar(10).IsNotEmpty())
	{
		ConsoleOut(HelpText);
		CString cek;
		ConsoleIn(cek);
		Exit(1);
	}
// parametry
	InName = CommandPar(1);

	OutName = CommandPar(2);

	int smeru = Int(CommandPar(3));
	if (smeru < 1) smeru = 1;
	if (smeru > SPRITEMAXSMER) smeru = SPRITEMAXSMER;

	int klid = Int(CommandPar(4));
	if (klid < 1) klid = 1;
	if (klid > SPRITEMAXKLID) klid = SPRITEMAXKLID;

	int pohyb = Int(CommandPar(5));
	if (pohyb < 0) pohyb = 0;
	if (pohyb > SPRITEMAXPOHYB) pohyb = SPRITEMAXPOHYB;

	Sprite.ReSize(klid, pohyb, smeru);

	Sprite.Delay(Int(CommandPar(6)));
	if (Sprite.Delay() < 55) Sprite.Delay(55);
	if (Sprite.Delay() > 4000000) Sprite.Delay(4000000);
	Sprite.Delay((Sprite.Delay() + 22) / 55 * 55);
	if (Sprite.Delay() > SPRITEMAXDELAY) Sprite.Delay(SPRITEMAXDELAY);

	Sprite.Level(Int(CommandPar(7)));

	Sprite.Kroku(Double(CommandPar(8)));
	if (Sprite.Kroku() < 0) Sprite.Kroku(0);
	if (Sprite.Kroku() > SPRITEMAXKROKU) Sprite.Kroku(SPRITEMAXKROKU);

	Dither = (Int(CommandPar(9)) != 0);

// cyklus p�es v�echny obr�zky
	for (int faze = 0; faze < klid+pohyb; faze++)
	{
		for (int smer = 0; smer < smeru; smer++)
		{

// na�ten� obr�zku
			if (!Sprite.At(faze, smer).LoadFile2(InName))
			{
				ConsoleOut(ReadErr1);
				ConsoleOut(InName);
				ConsoleOut(ReadErr2);
				Exit(1);
			}

// zmen�en� obr�zku
//			Sprite.At(faze, smer).HalfSize();
			Sprite.At(faze, smer).Resize(Sprite.Width(), Sprite.Height());

// zru�en� vstupn�ho souboru
//			::DeleteFile(InName);

// zv��en� jm�na souboru
			int pos = InName.RevFind('.');
			if (pos < 0) 
			{
				pos = InName.Length();
				InName += _T(".bmp");
			}

			while (--pos >= 0)
			{
				if ((InName[pos] < '0') || (InName[pos] > '9')) break;
				InName[pos]++;
				if (InName[pos] <= '9') break;
				InName[pos] = '0';
			}
		}
	}

// z�pis spraju
	if (!Sprite.SaveFile(OutName))
	{
		ConsoleOut(WriteErr1);
		ConsoleOut(OutName);
		ConsoleOut(WriteErr2);
		Exit(1);
	}

// konec programu
	Exit(EXITCODE_OK);
};

//////////////////////////////////////////////////////////////////////////////
// ukon�en� programu

void Exit(int code)
{
// uzamknut� ukon�en� programu
#ifdef _MT
	::EnterCriticalSection(&ExitCriticalSection);
#endif

// ukon�en� spr�vce pam�ti
	MemTerm();

// uvoln�n� uzamyk�n� ukon�en� programu
#ifdef _MT
	::DeleteCriticalSection(&ExitCriticalSection);
#endif

// ukon�en� programu
	ExitProcess(code);
}

/////////////////////////////////////////////////////////////////////////////
// generov�n� konverzn� tabulky barev obr�zku

void GenKonvPal(BITMAPINFO* bmp)
{
// lok�ln� prom�nn�
	int			i;									// ��ta� barev
	int			palet = bmp->bmiHeader.biClrUsed;	// po�et palet v bitmap�
	RGBQUAD*	col = bmp->bmiColors;				// ukazatel barevn�ch slo�ek
	BYTE*		pal = KonvPal;						// ukazatel konverzn�ch palet
	DWORD		BackColData = *(DWORD*)(StdBitmapInfo->bmiColors + BackCol); // paleta pozad�
	DWORD		ShadColData = *(DWORD*)(StdBitmapInfo->bmiColors + ShadCol); // paleta st�nu

// p��prava po�tu palet
	if (palet == 0) palet = (1 << bmp->bmiHeader.biBitCount);
	i = palet;
	if ((palet < 1) || (palet > 256)) return;

// cyklus p�es platn� barvy
	for (; i > 0; i--)
	{
		if (*(DWORD*)col == BackColData)
		{
			*pal = BackCol;
		}
		else
		{
			if (*(DWORD*)col == ShadColData)
			{
				*pal = ShadCol;
			}
			else
			{
				*pal = PalImport(col->rgbRed, col->rgbGreen, col->rgbBlue);
			}
		}
		pal++;
		col++;
	}

// vymaz�n� zbyl�ch neplatn�ch barev (import na �ernou barvu)
	MemFill(pal, 256-palet, BlackCol);
}


/////////////////////////////////////////////////////////////////////////////
// vygenerov�n� st�edn� barvy (ze 4 bod�)

BYTE _fastcall ColAvrg(BYTE col1, BYTE col2, BYTE col3, BYTE col4)
{
// lok�ln� prom�nn�
	int			r = 0;									// �erven� slo�ka
	int			g = 0;									// zelen� slo�ka
	int			b = 0;									// modr� slo�ka
	BYTE		n = 0;									// po�et platn�ch bod�
	RGBQUAD*	rgb;									// ukazatel palet
	int			shad = 0;								// ��ta� st�n�

// prvn� bod
	if (col1 != BackCol)
	{
		if (col1 == ShadCol)
		{
			shad++;
		}
		else
		{
			n++;											// zv��en� ��ta�e bod�
			rgb = StdBitmapInfo->bmiColors + col1;			// definice palet bodu
			r = rgb->rgbRed;								// �erven� slo�ka
			g = rgb->rgbGreen;								// zelen� slo�ka
			b = rgb->rgbBlue;								// modr� slo�ka
		}
	}

// druh� bod
	if (col2 != BackCol)
	{
		if (col2 == ShadCol)
		{
			shad++;
		}
		else
		{
			n++;											// zv��en� ��ta�e bod�
			rgb = StdBitmapInfo->bmiColors + col2;			// definice palet bodu
			r += rgb->rgbRed;								// �erven� slo�ka
			g += rgb->rgbGreen;								// zelen� slo�ka
			b += rgb->rgbBlue;								// modr� slo�ka
		}
	}

// t�et� bod
	if (col3 != BackCol)
	{
		if (col3 == ShadCol)
		{
			shad++;
		}
		else
		{
			n++;											// zv��en� ��ta�e bod�
			rgb = StdBitmapInfo->bmiColors + col3;			// definice palet bodu
			r += rgb->rgbRed;								// �erven� slo�ka
			g += rgb->rgbGreen;								// zelen� slo�ka
			b += rgb->rgbBlue;								// modr� slo�ka
		}
	}

// �tvrt� bod
	if (col4 != BackCol)
	{
		if (col4 == ShadCol)
		{
			shad++;
		}
		else
		{
			n++;											// zv��en� ��ta�e bod�
			rgb = StdBitmapInfo->bmiColors + col4;			// definice palet bodu
			r += rgb->rgbRed;								// �erven� slo�ka
			g += rgb->rgbGreen;								// zelen� slo�ka
			b += rgb->rgbBlue;								// modr� slo�ka
		}
	}

// bod byl st�n
	if (shad > 2)
	{
		return ShadCol;
	}

// bod byl pozad�
	if (n < 2)
	{
		return BackCol;
	}

// jinak vygenerov�n� bodu podle barevn�ch slo�ek
	else
	{
		return PalImport((BYTE)(r/n), (BYTE)(g/n), (BYTE)(b/n));
	}
}

/////////////////////////////////////////////////////////////////////////////
// vygenerov�n� st�edn� barvy (ze 4 bod�) s ditheringem

BYTE ColAvrgDither(BYTE col1, BYTE col2, BYTE col3, BYTE col4, int x, int y)
{
// lok�ln� prom�nn�
	int			r = 0;									// �erven� slo�ka
	int			g = 0;									// zelen� slo�ka
	int			b = 0;									// modr� slo�ka
	BYTE		n = 0;									// po�et platn�ch bod�
	RGBQUAD*	rgb;									// ukazatel palet
	int			shad = 0;								// ��ta� st�n�

// prvn� bod
	if (col1 != BackCol)
	{
		if (col1 == ShadCol)
		{
			shad++;
		}
		else
		{
			n++;											// zv��en� ��ta�e bod�
			rgb = StdBitmapInfo->bmiColors + col1;			// definice palet bodu
			r = rgb->rgbRed;								// �erven� slo�ka
			g = rgb->rgbGreen;								// zelen� slo�ka
			b = rgb->rgbBlue;								// modr� slo�ka
		}
	}

// druh� bod
	if (col2 != BackCol)
	{
		if (col2 == ShadCol)
		{
			shad++;
		}
		else
		{
			n++;											// zv��en� ��ta�e bod�
			rgb = StdBitmapInfo->bmiColors + col2;			// definice palet bodu
			r += rgb->rgbRed;								// �erven� slo�ka
			g += rgb->rgbGreen;								// zelen� slo�ka
			b += rgb->rgbBlue;								// modr� slo�ka
		}
	}

// t�et� bod
	if (col3 != BackCol)
	{
		if (col3 == ShadCol)
		{
			shad++;
		}
		else
		{
			n++;											// zv��en� ��ta�e bod�
			rgb = StdBitmapInfo->bmiColors + col3;			// definice palet bodu
			r += rgb->rgbRed;								// �erven� slo�ka
			g += rgb->rgbGreen;								// zelen� slo�ka
			b += rgb->rgbBlue;								// modr� slo�ka
		}
	}

// �tvrt� bod
	if (col4 != BackCol)
	{
		if (col4 == ShadCol)
		{
			shad++;
		}
		else
		{
			n++;											// zv��en� ��ta�e bod�
			rgb = StdBitmapInfo->bmiColors + col4;			// definice palet bodu
			r += rgb->rgbRed;								// �erven� slo�ka
			g += rgb->rgbGreen;								// zelen� slo�ka
			b += rgb->rgbBlue;								// modr� slo�ka
		}
	}

// bod byl st�n
	if (shad > 2)
	{
		return ShadCol;
	}

// bod byl pozad�
	if (n < 2)
	{
		return BackCol;
	}

// jinak vygenerov�n� bodu podle barevn�ch slo�ek
	else
	{
		return PalImportDither((BYTE)(r/n), (BYTE)(g/n), (BYTE)(b/n), x, y);
	}
}

/////////////////////////////////////////////////////////////////////////////
// dekomprese dat

#define MAXLENX 25					// min. d�lka dlouh�ho �et�zce
#define MAXLEN (MAXLENX+254)		// maxim�ln� d�lka �et�zce
#define SUBSTLEN	7				// d�lka nahrazen� dlouh�m k�dem

void DeKomp(BYTE* dstBuf, int dstNum, BYTE* srcBuf, int srcNum)
{
// na�ten� jednoho bitu ze stavov�ho slova
#define DekBit		bit = status & 1;				\
					status >>= 1;					\
					if (status == 0)				\
					{								\
						if (srci >= srcNum) break;	\
						status = *(WORD*)src;		\
						src++;						\
						src++;						\
						srci++;						\
						srci++;						\
						bit = status & 1;			\
						status >>= 1;				\
						status |= 0x8000;			\
					}


	BYTE* dst = dstBuf;				// ukazatel c�lov� adresy
	int dsti = 0;					// ��ta� c�lov�ch dat
	BYTE* src = srcBuf;				// ukazatel zdrojov� adresy
	int srci = 0;					// ��ta� zdrojov�ch dat
	BYTE* src2;						// pomocn� ukazatel
	int srci2;

	WORD status = 0;				// st�ada� stavov�ho slova
	BYTE offsetL, offsetH;			// offset k opakov�n�
	int delka;						// d�lka k opakov�n�
	int bit;						// 1 = na�ten� bit

	for (;;)
	{
// na�ten� prvn�ho bitu p��znaku
		DekBit

// p�esun bajtu bez komprese
		if (bit == 0)
		{
			if (srci >= srcNum) break;
			if (dsti >= dstNum) break;
			*dst = *src;
			dst++;
			dsti++;
			src++;
			srci++;
		}

// jinak bude opakov�n� �et�zce
		else
		{
			offsetH = 0;
			offsetL = 0;

// prvn� bit d�lky
			DekBit
			delka = bit;

// zv��en� ��ta�e d�lky
			for (;;)
			{
				delka++;
				delka++;

// na�ten� p��znaku konce k�du
				DekBit

				if (bit == 0)
				{
					break;
				}

				if (delka >= (MAXLENX-4+1))
				{
					delka++;
					delka++;
					break;
				}
			}

// korekce pro n�hradn� k�d
			if (delka >= SUBSTLEN)
			{
				if (delka == SUBSTLEN)
				{
					if (srci >= srcNum) break;
					delka = *src;
					src++;
					srci++;
					if (delka == 255) break;
					delka += MAXLENX;
				}
				else
				{
					delka--;
				}
			}

// stanoven� offsetu - vy��� bajt
			if (delka != 2)
			{
				DekBit
				if (bit == 0)
				{
					DekBit
					offsetH <<= 1;
					offsetH |= bit;

					DekBit
					offsetH <<= 1;
					offsetH |= bit;

					DekBit
					offsetH <<= 1;
					offsetH |= bit;

					if (offsetH > 1)
					{
						DekBit
						offsetH <<= 1;
						offsetH |= bit;

						offsetH--;
						if (offsetH > 6)
						{
							DekBit
							offsetH <<= 1;
							offsetH |= bit;

							offsetH -= 7;
							if (offsetH > 13)
							{
								DekBit
								offsetH <<= 1;
								offsetH |= bit;

								offsetH -= 14;
							}
						}
					}
					else
					{
						offsetH++;
					}
				}
			}

// na�ten� offsetu - ni��� bajt
			if (srci >= srcNum) break;
			offsetL = *src;
			src++;
			srci++;

// p�enesen� �et�zce
			srci2 = dsti - (WORD)(offsetL + offsetH*256);
			if (srci2 < 0) break;
			src2 = &(dstBuf[srci2]);

			for (; delka > 0; delka--)
			{
				if (srci2 >= dstNum) break;
				if (dsti >= dstNum) break;
				*dst = *src2;
				dst++;
				dsti++;
				src2++;
				srci2++;
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// p�evod re�ln�ho ��sla na cel� ��slo se zaokrouhlen�m k nejbli��� hodnot�

int Round(double num)
{
#ifdef _M_IX86

	DWORD		result;				// v�sledek operace

	_asm {
		fld			num				// na�ten� ��sla k proveden� operace
		fistp		result			// p�evod ��sla na cel� ��slo
	}
	return result;

#else

	if (num >= 0)
	{
		return (int)(num + 0.5);
	}
	else
	{
		return (int)(num - 0.5);
	}

#endif
}

/////////////////////////////////////////////////////////////////////////////
// p�evod re�ln�ho ��sla na cel� ��slo s o��znut�m k nule

int Round0(double num)
{
#ifdef _M_IX86

	WORD		oldstat;			// star� stav ��dic�ho slova
	WORD		newstat;			// nov� stav ��dic�ho slova
	int			result;				// v�sledek operace

	_asm {
		fld			num				// na�ten� ��sla k proveden� operace
		wait						// synchronizace
		fnstcw		oldstat			// ulo�eni ��dic�ho slova
		wait						// synchronizace
		mov			ax,oldstat		// star� stav ��dic�ho slova
		or			ah,0xc			// m�d zaokrouhlen� sm�rem k nule
		mov			newstat,ax		// nov� stav ��dic�ho slova
		fldcw		newstat			// nastaven� nov�ho ��dic�ho slova
		fistp		result			// p�evod ��sla na cel� ��slo
		fldcw		oldstat			// navr�cen� p�vodn�ho ��dic�ho slova
	}
	return result;

#else

	return (int)num;

#endif
}

/////////////////////////////////////////////////////////////////////////////
// p�evod re�ln�ho ��sla na cel� ��slo se zaokrouhlen�m nahoru (ke kladn� hodnot�)

int RoundP(double num)
{
#ifdef _M_IX86

	WORD		oldstat;			// star� stav ��dic�ho slova
	WORD		newstat;			// nov� stav ��dic�ho slova
	int			result;				// v�sledek operace

	_asm {
		fld			num				// na�ten� ��sla k proveden� operace
		wait						// synchronizace
		fnstcw		oldstat			// ulo�eni ��dic�ho slova
		wait						// synchronizace
		mov			ax,oldstat		// star� stav ��dic�ho slova
		and			ah,not 0xc;		// vynulov�n� ��dic�ch bit� pro zaokrouhlen�
		or			ah,8			// m�d zaokrouhlen� sm�rem nahoru
		mov			newstat,ax		// nov� stav ��dic�ho slova
		fldcw		newstat			// nastaven� nov�ho ��dic�ho slova
		fistp		result			// p�evod ��sla na cel� ��slo
		fldcw		oldstat			// navr�cen� p�vodn�ho ��dic�ho slova
	}
	return result;

#else

	if (num >= 0)
	{
		int i = (int)num;
		if ((double)i == num) return i;
		return i+1;
	}
	else
	{
		return (int)num;
	}

#endif
}

/////////////////////////////////////////////////////////////////////////////
// p�evod re�ln�ho ��sla na cel� ��slo se zaokrouhlen�m dol� (k z�porn� hodnot�)

int RoundM(double num)
{
#ifdef _M_IX86

	WORD		oldstat;			// star� stav ��dic�ho slova
	WORD		newstat;			// nov� stav ��dic�ho slova
	int			result;				// v�sledek operace

	_asm {
		fld			num				// na�ten� ��sla k proveden� operace
		wait						// synchronizace
		fnstcw		oldstat			// ulo�eni ��dic�ho slova
		wait						// synchronizace
		mov			ax,oldstat		// star� stav ��dic�ho slova
		and			ah,not 0xc;		// vynulov�n� ��dic�ch bit� pro zaokrouhlen�
		or			ah,4			// m�d zaokrouhlen� sm�rem dol�
		mov			newstat,ax		// nov� stav ��dic�ho slova
		fldcw		newstat			// nastaven� nov�ho ��dic�ho slova
		fistp		result			// p�evod ��sla na cel� ��slo
		fldcw		oldstat			// navr�cen� p�vodn�ho ��dic�ho slova
	}
	return result;

#else

	if (num >= 0)
	{
		return (int)num;
	}
	else
	{
		int i = (int)num;
		if ((double)i == num) return i;
		return i-1;
	}

#endif
}


/////////////////////////////////////////////////////////////////////////////
// funkce pro vypnutou optimalizaci

#ifndef _OPTIM
int abs(int num) 
{ 
	if (num < 0) 
	{
		return -num; 
	} 
	else 
	{ 
		return num; 
	} 
};
#endif
