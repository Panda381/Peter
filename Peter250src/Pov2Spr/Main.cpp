
#include "Main.h"

//////////////////////////////////////////////////////////////////////////////
// inicializaËnÌ segmenty CRT (konstruktory a destruktory)

typedef void (__cdecl *_PVFV)(void);		// ukazatel na funkci void fnc(void)
typedef void (*PFV)(void);					// funkce void fnc(void)

#pragma data_seg(".CRT$XCA")
_PVFV	__xc_a[] = { NULL };				// konstruktory C++
#pragma data_seg(".CRT$XCZ")
_PVFV	__xc_z[] = { NULL };

#pragma data_seg()							// resetov·nÌ na bÏûnou datovou sekci

#pragma comment(linker, "/MERGE:.CRT=.data")	// p¯ipojenÌ CRT sekcÌ do datovÈ sekce


//////////////////////////////////////////////////////////////////////////////
// glob·lnÌ promÏnnÈ

CString		CommandLine;				// p¯Ìkazov˝ ¯·dek
int			VerzeOS;					// verze systÈmu
HINSTANCE	hInstance = NULL;			// instance programu

BYTE*		StdPalImport;				// tabulka importu palet
BYTE*		StdPalImportDither;			// tabulka importu palet s ditheringem
BITMAPINFO* StdBitmapInfo;				// standardnÌ z·hlavÌ BMP
BYTE*		KonvPal;					// konverznÌ tabulka palet


bool	ConsoleOn = false;		// p¯Ìznak reûimu konzoly
HANDLE	ConIn = INVALID_HANDLE_VALUE;	// handle pro vstup z konzoly
HANDLE	ConOut = INVALID_HANDLE_VALUE;	// handle pro v˝stup na konzolu
HANDLE	ConErr = INVALID_HANDLE_VALUE;	// handle pro chybov˝ v˝stup na konzolu

bool	Dither = true;					// pouûÌt dithering

//////////////////////////////////////////////////////////////////////////////
// pr·zdnÈ objekty (nap¯. pro n·vrat neplatnÈ poloûky z funkce)

CString		EmptyString;				// pr·zdn˝ ¯etÏzec
CPicture	EmptyPicture;				// pr·zdn˝ obr·zek
CSprite		EmptySprite;				// pr·zdn˝ sprajt

//////////////////////////////////////////////////////////////////////////////
// konstanty

const char*	Compiled =	"Gemtree 32-compiler"; // jen tak pro zmatenÌ

// tabulka hladin standardnÌch barev (rozsah 0 aû 256)
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
// po zmÏnÏ palet zruö soubor PALIMP.DAT a spusù DEBUG verzi pro novÈ vygenerov·nÌ
// (pozor - generov·nÌ m˘ûe trvat desÌtky sekund). Potom znovu p¯eklad.
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// tabulka odstÌn˘ standardnÌch barev (0 aû 255) - po¯adÌ B, G, R, F,
const RGBQUAD ColColTab[] = 
{ 
	0,		0,		255,		0,			//  0: Ëerven·
	72,		72,		255,		0,			//  1:
	116,	116,	255,		0,			//  2:
	158,	166,	255,		0,			//  3:
	210,	220,	255,		0,			//  4:
	0,		114,	255,		0,			//  5: hnÏd·
	150,	198,	255,		0,			//  6:
	0,		182,	255,		0,			//  7: oranûov·
	96,		192,	255,		0,			//  8:
	0,		255,	255,		0,			//  9: ûlut·
	128,	255,	255,		0,			// 10:
	192,	255,	255,		0,			// 11: svÏtle ûlut·
	0,		255,	178,		0,			// 12: ûlutozelen·
	0,		255,	0,			0,			// 13: zelen·
	128,	255,	128,		0,			// 14:
	192,	255,	192,		0,			// 15:
	146,	255,	0,			0,			// 16: namodrale zelen·
	222,	255,	138,		0,			// 17:
	255,	255,	0,			0,			// 18: modrozelen·
	255,	186,	0,			0,			// 19:
	255,	222,	124,		0,			// 20:
	255,	128,	0,			0,			// 21:
	255,	0,		0,			0,			// 22: modr·
	255,	80,		72,			0,			// 23:
	255,	144,	128,		0,			// 24:
	255,	192,	154,		0,			// 25:
	255,	238,	210,		0,			// 26:
	255,	0,		114,		0,			// 27:
	255,	0,		174,		0,			// 28:
	255,	162,	198,		0,			// 29:
	255,	0,		255,		0,			// 30: fialov·
	255,	128,	255,		0,			// 31: svÏtle fialov·
	138,	0,		255,		0,			// 32: fialovÏ Ëerven·
	192,	128,	255,		0,			// 33:
	210,	186,	255,		0,			// 34:

	255,	255,	255,		0,			// 35: bÌl· (nepouûije se, vygeneruje se)
	96,		96,		96,			0,			// 36: öed· (nepouûije se, vygeneruje se)
};

const int ColLev = sizeof(ColLevTab)/sizeof(ColLevTab[0]);	// poËet hladin barev
const int ColCol = sizeof(ColColTab)/sizeof(ColColTab[0]);	// poËet odstÌn˘ barev
const int StdColors = ResCols + ColCol*ColLev;			// poËet vlastnÌch palet (zaËÌnajÌ od 0)
const BYTE WhiteCol = StdColors - 2*ColLev;			// bÌl· barva
const BYTE BlackCol = StdColors - 1;				// Ëern· barva

//////////////////////////////////////////////////////////////////////////////
// lok·lnÌ promÏnnÈ

#ifdef _MT
static	CRITICAL_SECTION	ExitCriticalSection;	// kritick· sekce pro konec programu
#endif

//CString UvText(  _T("Pov2Spr v1.0 - konverze obr·zk˘ na sprajt; (c) Miroslav NÏmeËek\n"));
//CString HelpText(_T("   POV2SPR vstup v˝stup smÏr˘ klid pohyb prodleva hladina krok dither\n")
//				 _T("               vstup .... vstupnÌ soubor BMP prvnÌho obr·zku\n")
//				 _T("               v˝stup ... v˝stupnÌ soubor SPR sprajtu\n")
//				 _T("               smÏr˘ .... poËet smÏr˘ 0 aû 1000\n")
//				 _T("               klid ..... poËet klidov˝ch f·zÌ 1 aû 1000\n")
//				 _T("               pohyb .... poËet f·zÌ pohybu 0 aû 1000\n")
//				 _T("               prodleva . prodleva mezi f·zemi v milisekund·ch\n")
//				 _T("               hladina .. hladina k zobrazenÌ, 0=p¯edmÏty\n")
//				 _T("               krok ..... poËet f·zÌ na jednotkovou vzd·lenost\n")
//				 _T("               dither ... pouûÌt dithering 1=ano, 0=ne\n")
//				 _T("stisknÏte <Enter>... "));
//
//CString WriteErr1(_T("Chyba z·pisu do v˝stupnÌho souboru "));
//CString WriteErr2(_T("!\n"));
//
//CString ReadErr1(_T("Chyba ËtenÌ ze vstupnÌho souboru "));
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

CString InName;				// vstupnÌ soubor
CString OutName;			// v˝stupnÌ soubor
CPicture Picture;			// vstupnÌ obr·zek
CSprite Sprite;				// v˝stupnÌ sprajt

/***************************************************************************\
*																			*
*									konzola									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// zapnutÌ/vypnutÌ konzoly

void ConsoleOnSet(bool on)
{
	if (ConsoleOn == on) return;
	ConsoleOn = on;

// zapnutÌ konzoly
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
// v˝stup textu na konzolu

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
// v˝stup chybovÈho textu na konzolu

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

// nalezenÌ parametru
	for (; i >= 0; i--)
	{

// nalezenÌ zaË·tku parametru
		while (	(pos < len) && 
				(CommandLine[pos] <= _T(' ')) && 
				(CommandLine[pos] > 0)) 
		{
			pos++;
		}
		beg = pos;

// nalezenÌ konce parametru
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

// p¯enesenÌ parametru
	text = CommandLine.Mid(beg, pos-beg);

// zruöenÌ uvozovek
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
// inicializace ˙seku inicializaËnÌch/terminaËnÌch funkcÌ

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
// hlavnÌ start programu

void mainCRTStartup()
{
// ˙schova verze systÈmu
	VerzeOS = (int)::GetVersion();

// handle instance programu
	hInstance = ::GetModuleHandle(NULL);

// inicializace uzamyk·nÌ ukonËenÌ programu
#ifdef _MT
	::InitializeCriticalSection(&ExitCriticalSection);
#endif

// inicializace spr·vce pamÏti
	if (!MemInit()) 
	{
		Exit(EXITCODE_MEMERR);
	}

// inicializace koprocesoru
#ifdef _M_IX86
	WORD stat;
	_asm {
		wait						// synchronizace
		fnstcw		stat			// uloûenÌ ¯ÌdicÌho slova
		wait						// synchronizace
		mov			ax,stat			// stav ¯ÌdicÌho slova
		and			ah,not 0xc		// implicitnÌ zaokrouhlov·nÌ
		or			ah,3			// p¯esnost 64 bit˘
		mov			stat,ax			// nov˝ stav ¯ÌdicÌho slova
		fldcw		stat			// nastavenÌ novÈho ¯ÌdicÌho slova
	}
#endif

// inicializace dat pr·zdnÈ ikony a pr·zdnÈho obr·zku
	EmptyPictureData.Data = (BYTE*)MemGet(ICONSIZE);
	MemFill(EmptyPictureData.Data, ICONSIZE, BackCol);

// inicializace obsluhy sprajt˘
	InitSprite();						// statick· inicializace sprajt˘

// inicializace glob·lnÌch objekt˘
	InitTerm(__xc_a, __xc_z);

// inicializace standardnÌho z·hlavÌ BMP
	StdBitmapInfo = (BITMAPINFO*) MemGet(sizeof(BITMAPINFO) + sizeof(RGBQUAD)*255);
	MemFill(StdBitmapInfo, sizeof(BITMAPINFO) + sizeof(RGBQUAD)*255);	// vynulov·nÌ
	StdBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);	// velikost z·hlavÌ
	StdBitmapInfo->bmiHeader.biWidth = ICONWIDTH;	// öÌ¯ka bitmapy
	StdBitmapInfo->bmiHeader.biHeight = ICONHEIGHT;	// v˝öka bitmapy
	StdBitmapInfo->bmiHeader.biPlanes = 1;			// poËet barevn˝ch rovin
	StdBitmapInfo->bmiHeader.biBitCount = 8;		// poËet bit˘ na bod
	StdBitmapInfo->bmiHeader.biCompression = BI_RGB; // nenÌ komprese
	StdBitmapInfo->bmiHeader.biClrImportant = StdColors; // poËet d˘leûit˝ch palet

// vytvo¯enÌ standardnÌch palet
	RGBQUAD* rgb = StdBitmapInfo->bmiColors;		// zaË·tek tabulky palet

// nemÏnit paletu barvu pozadÌ - pouûÌv· se p¯i importu obr·zk˘
	rgb->rgbRed =	BACKCOLOR_RED;					// pr˘hledn· barva
	rgb->rgbGreen =	BACKCOLOR_GREEN;
	rgb->rgbBlue =	BACKCOLOR_BLUE;
	rgb++;

	rgb->rgbRed =	SHADCOLOR_RED;					// barva stÌnu
	rgb->rgbGreen =	SHADCOLOR_GREEN;
	rgb->rgbBlue =	SHADCOLOR_BLUE;
	rgb++;

	int i,j,k;
	for (i = 0; i < (ColCol-2); i++)				// pro vöechny barvy bez bÌlÈ a öedÈ
	{
		*rgb = ColColTab[i];						// p¯enesenÌ z·kladnÌ barvy

		for (j = 1; j < ColLev; j++)				// pro vöechny odstÌny
		{
			k = ColLevTab[j];						// n·sobÌcÌ koeficient
			rgb[j].rgbRed = (BYTE)(rgb->rgbRed*k/256);		// Ëerven·
			rgb[j].rgbGreen = (BYTE)(rgb->rgbGreen*k/256);	// zelen·
			rgb[j].rgbBlue = (BYTE)(rgb->rgbBlue*k/256);	// modr·
		}
		rgb += ColLev;								// adresa dalöÌ barvy
	}

	i = WhiteCol;									// index bÌlÈ barvy
	for (; i <= BlackCol; i++)						// od bÌlÈ barvy aû po Ëernou
	{
		k = (BlackCol-i)*255/(2*ColLev-1);			// odstÌn bÌlÈ barvy
		rgb->rgbRed = (BYTE)k;						// Ëerven·
		rgb->rgbGreen = (BYTE)k;					// zelen·
		rgb->rgbBlue = (BYTE)k;						// modr·
		rgb++;										// zv˝öenÌ adresy barvy
	}

// vygenerov·nÌ tabulky pro import palet a tabulky palet (pro konverzi BMP)
	StdPalImport = (BYTE*)MemGet(64 * 64 * 64);		// tabulka pro import palet
	StdPalImportDither = (BYTE*)MemGet(64 * 64 * 64 * 4);	// tabulka pro import palet s dithering

// naËtenÌ tabulky importu palet
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

// vytvo¯enÌ konverznÌ tabulky palet pro Ìmport soubor˘ BMP
	KonvPal = (BYTE*)MemGet(256);			// konverznÌ tabulka palet

// ˙schova p¯ÌkazovÈho ¯·dku
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

// cyklus p¯es vöechny obr·zky
	for (int faze = 0; faze < klid+pohyb; faze++)
	{
		for (int smer = 0; smer < smeru; smer++)
		{

// naËtenÌ obr·zku
			if (!Sprite.At(faze, smer).LoadFile2(InName))
			{
				ConsoleOut(ReadErr1);
				ConsoleOut(InName);
				ConsoleOut(ReadErr2);
				Exit(1);
			}

// zmenöenÌ obr·zku
//			Sprite.At(faze, smer).HalfSize();
			Sprite.At(faze, smer).Resize(Sprite.Width(), Sprite.Height());

// zruöenÌ vstupnÌho souboru
//			::DeleteFile(InName);

// zv˝öenÌ jmÈna souboru
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

// z·pis spraju
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
// ukonËenÌ programu

void Exit(int code)
{
// uzamknutÌ ukonËenÌ programu
#ifdef _MT
	::EnterCriticalSection(&ExitCriticalSection);
#endif

// ukonËenÌ spr·vce pamÏti
	MemTerm();

// uvolnÏnÌ uzamyk·nÌ ukonËenÌ programu
#ifdef _MT
	::DeleteCriticalSection(&ExitCriticalSection);
#endif

// ukonËenÌ programu
	ExitProcess(code);
}

/////////////////////////////////////////////////////////////////////////////
// generov·nÌ konverznÌ tabulky barev obr·zku

void GenKonvPal(BITMAPINFO* bmp)
{
// lok·lnÌ promÏnnÈ
	int			i;									// ËÌtaË barev
	int			palet = bmp->bmiHeader.biClrUsed;	// poËet palet v bitmapÏ
	RGBQUAD*	col = bmp->bmiColors;				// ukazatel barevn˝ch sloûek
	BYTE*		pal = KonvPal;						// ukazatel konverznÌch palet
	DWORD		BackColData = *(DWORD*)(StdBitmapInfo->bmiColors + BackCol); // paleta pozadÌ
	DWORD		ShadColData = *(DWORD*)(StdBitmapInfo->bmiColors + ShadCol); // paleta stÌnu

// p¯Ìprava poËtu palet
	if (palet == 0) palet = (1 << bmp->bmiHeader.biBitCount);
	i = palet;
	if ((palet < 1) || (palet > 256)) return;

// cyklus p¯es platnÈ barvy
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

// vymaz·nÌ zbyl˝ch neplatn˝ch barev (import na Ëernou barvu)
	MemFill(pal, 256-palet, BlackCol);
}


/////////////////////////////////////////////////////////////////////////////
// vygenerov·nÌ st¯ednÌ barvy (ze 4 bod˘)

BYTE _fastcall ColAvrg(BYTE col1, BYTE col2, BYTE col3, BYTE col4)
{
// lok·lnÌ promÏnnÈ
	int			r = 0;									// Ëerven· sloûka
	int			g = 0;									// zelen· sloûka
	int			b = 0;									// modr· sloûka
	BYTE		n = 0;									// poËet platn˝ch bod˘
	RGBQUAD*	rgb;									// ukazatel palet
	int			shad = 0;								// ËÌtaË stÌn˘

// prvnÌ bod
	if (col1 != BackCol)
	{
		if (col1 == ShadCol)
		{
			shad++;
		}
		else
		{
			n++;											// zv˝öenÌ ËÌtaËe bod˘
			rgb = StdBitmapInfo->bmiColors + col1;			// definice palet bodu
			r = rgb->rgbRed;								// Ëerven· sloûka
			g = rgb->rgbGreen;								// zelen· sloûka
			b = rgb->rgbBlue;								// modr· sloûka
		}
	}

// druh˝ bod
	if (col2 != BackCol)
	{
		if (col2 == ShadCol)
		{
			shad++;
		}
		else
		{
			n++;											// zv˝öenÌ ËÌtaËe bod˘
			rgb = StdBitmapInfo->bmiColors + col2;			// definice palet bodu
			r += rgb->rgbRed;								// Ëerven· sloûka
			g += rgb->rgbGreen;								// zelen· sloûka
			b += rgb->rgbBlue;								// modr· sloûka
		}
	}

// t¯etÌ bod
	if (col3 != BackCol)
	{
		if (col3 == ShadCol)
		{
			shad++;
		}
		else
		{
			n++;											// zv˝öenÌ ËÌtaËe bod˘
			rgb = StdBitmapInfo->bmiColors + col3;			// definice palet bodu
			r += rgb->rgbRed;								// Ëerven· sloûka
			g += rgb->rgbGreen;								// zelen· sloûka
			b += rgb->rgbBlue;								// modr· sloûka
		}
	}

// Ëtvrt˝ bod
	if (col4 != BackCol)
	{
		if (col4 == ShadCol)
		{
			shad++;
		}
		else
		{
			n++;											// zv˝öenÌ ËÌtaËe bod˘
			rgb = StdBitmapInfo->bmiColors + col4;			// definice palet bodu
			r += rgb->rgbRed;								// Ëerven· sloûka
			g += rgb->rgbGreen;								// zelen· sloûka
			b += rgb->rgbBlue;								// modr· sloûka
		}
	}

// bod byl stÌn
	if (shad > 2)
	{
		return ShadCol;
	}

// bod byl pozadÌ
	if (n < 2)
	{
		return BackCol;
	}

// jinak vygenerov·nÌ bodu podle barevn˝ch sloûek
	else
	{
		return PalImport((BYTE)(r/n), (BYTE)(g/n), (BYTE)(b/n));
	}
}

/////////////////////////////////////////////////////////////////////////////
// vygenerov·nÌ st¯ednÌ barvy (ze 4 bod˘) s ditheringem

BYTE ColAvrgDither(BYTE col1, BYTE col2, BYTE col3, BYTE col4, int x, int y)
{
// lok·lnÌ promÏnnÈ
	int			r = 0;									// Ëerven· sloûka
	int			g = 0;									// zelen· sloûka
	int			b = 0;									// modr· sloûka
	BYTE		n = 0;									// poËet platn˝ch bod˘
	RGBQUAD*	rgb;									// ukazatel palet
	int			shad = 0;								// ËÌtaË stÌn˘

// prvnÌ bod
	if (col1 != BackCol)
	{
		if (col1 == ShadCol)
		{
			shad++;
		}
		else
		{
			n++;											// zv˝öenÌ ËÌtaËe bod˘
			rgb = StdBitmapInfo->bmiColors + col1;			// definice palet bodu
			r = rgb->rgbRed;								// Ëerven· sloûka
			g = rgb->rgbGreen;								// zelen· sloûka
			b = rgb->rgbBlue;								// modr· sloûka
		}
	}

// druh˝ bod
	if (col2 != BackCol)
	{
		if (col2 == ShadCol)
		{
			shad++;
		}
		else
		{
			n++;											// zv˝öenÌ ËÌtaËe bod˘
			rgb = StdBitmapInfo->bmiColors + col2;			// definice palet bodu
			r += rgb->rgbRed;								// Ëerven· sloûka
			g += rgb->rgbGreen;								// zelen· sloûka
			b += rgb->rgbBlue;								// modr· sloûka
		}
	}

// t¯etÌ bod
	if (col3 != BackCol)
	{
		if (col3 == ShadCol)
		{
			shad++;
		}
		else
		{
			n++;											// zv˝öenÌ ËÌtaËe bod˘
			rgb = StdBitmapInfo->bmiColors + col3;			// definice palet bodu
			r += rgb->rgbRed;								// Ëerven· sloûka
			g += rgb->rgbGreen;								// zelen· sloûka
			b += rgb->rgbBlue;								// modr· sloûka
		}
	}

// Ëtvrt˝ bod
	if (col4 != BackCol)
	{
		if (col4 == ShadCol)
		{
			shad++;
		}
		else
		{
			n++;											// zv˝öenÌ ËÌtaËe bod˘
			rgb = StdBitmapInfo->bmiColors + col4;			// definice palet bodu
			r += rgb->rgbRed;								// Ëerven· sloûka
			g += rgb->rgbGreen;								// zelen· sloûka
			b += rgb->rgbBlue;								// modr· sloûka
		}
	}

// bod byl stÌn
	if (shad > 2)
	{
		return ShadCol;
	}

// bod byl pozadÌ
	if (n < 2)
	{
		return BackCol;
	}

// jinak vygenerov·nÌ bodu podle barevn˝ch sloûek
	else
	{
		return PalImportDither((BYTE)(r/n), (BYTE)(g/n), (BYTE)(b/n), x, y);
	}
}

/////////////////////////////////////////////////////////////////////////////
// dekomprese dat

#define MAXLENX 25					// min. dÈlka dlouhÈho ¯etÏzce
#define MAXLEN (MAXLENX+254)		// maxim·lnÌ dÈlka ¯etÏzce
#define SUBSTLEN	7				// dÈlka nahrazen· dlouh˝m kÛdem

void DeKomp(BYTE* dstBuf, int dstNum, BYTE* srcBuf, int srcNum)
{
// naËtenÌ jednoho bitu ze stavovÈho slova
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


	BYTE* dst = dstBuf;				// ukazatel cÌlovÈ adresy
	int dsti = 0;					// ËÌtaË cÌlov˝ch dat
	BYTE* src = srcBuf;				// ukazatel zdrojovÈ adresy
	int srci = 0;					// ËÌtaË zdrojov˝ch dat
	BYTE* src2;						// pomocn˝ ukazatel
	int srci2;

	WORD status = 0;				// st¯adaË stavovÈho slova
	BYTE offsetL, offsetH;			// offset k opakov·nÌ
	int delka;						// dÈlka k opakov·nÌ
	int bit;						// 1 = naËten˝ bit

	for (;;)
	{
// naËtenÌ prvnÌho bitu p¯Ìznaku
		DekBit

// p¯esun bajtu bez komprese
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

// jinak bude opakov·nÌ ¯etÏzce
		else
		{
			offsetH = 0;
			offsetL = 0;

// prvnÌ bit dÈlky
			DekBit
			delka = bit;

// zv˝öenÌ ËÌtaËe dÈlky
			for (;;)
			{
				delka++;
				delka++;

// naËtenÌ p¯Ìznaku konce kÛdu
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

// korekce pro n·hradnÌ kÛd
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

// stanovenÌ offsetu - vyööÌ bajt
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

// naËtenÌ offsetu - niûöÌ bajt
			if (srci >= srcNum) break;
			offsetL = *src;
			src++;
			srci++;

// p¯enesenÌ ¯etÏzce
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
// p¯evod re·lnÈho ËÌsla na celÈ ËÌslo se zaokrouhlenÌm k nejbliûöÌ hodnotÏ

int Round(double num)
{
#ifdef _M_IX86

	DWORD		result;				// v˝sledek operace

	_asm {
		fld			num				// naËtenÌ ËÌsla k provedenÌ operace
		fistp		result			// p¯evod ËÌsla na celÈ ËÌslo
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
// p¯evod re·lnÈho ËÌsla na celÈ ËÌslo s o¯ÌznutÌm k nule

int Round0(double num)
{
#ifdef _M_IX86

	WORD		oldstat;			// star˝ stav ¯ÌdicÌho slova
	WORD		newstat;			// nov˝ stav ¯ÌdicÌho slova
	int			result;				// v˝sledek operace

	_asm {
		fld			num				// naËtenÌ ËÌsla k provedenÌ operace
		wait						// synchronizace
		fnstcw		oldstat			// uloûeni ¯ÌdicÌho slova
		wait						// synchronizace
		mov			ax,oldstat		// star˝ stav ¯ÌdicÌho slova
		or			ah,0xc			// mÛd zaokrouhlenÌ smÏrem k nule
		mov			newstat,ax		// nov˝ stav ¯ÌdicÌho slova
		fldcw		newstat			// nastavenÌ novÈho ¯ÌdicÌho slova
		fistp		result			// p¯evod ËÌsla na celÈ ËÌslo
		fldcw		oldstat			// navr·cenÌ p˘vodnÌho ¯ÌdicÌho slova
	}
	return result;

#else

	return (int)num;

#endif
}

/////////////////////////////////////////////////////////////////////////////
// p¯evod re·lnÈho ËÌsla na celÈ ËÌslo se zaokrouhlenÌm nahoru (ke kladnÈ hodnotÏ)

int RoundP(double num)
{
#ifdef _M_IX86

	WORD		oldstat;			// star˝ stav ¯ÌdicÌho slova
	WORD		newstat;			// nov˝ stav ¯ÌdicÌho slova
	int			result;				// v˝sledek operace

	_asm {
		fld			num				// naËtenÌ ËÌsla k provedenÌ operace
		wait						// synchronizace
		fnstcw		oldstat			// uloûeni ¯ÌdicÌho slova
		wait						// synchronizace
		mov			ax,oldstat		// star˝ stav ¯ÌdicÌho slova
		and			ah,not 0xc;		// vynulov·nÌ ¯ÌdicÌch bit˘ pro zaokrouhlenÌ
		or			ah,8			// mÛd zaokrouhlenÌ smÏrem nahoru
		mov			newstat,ax		// nov˝ stav ¯ÌdicÌho slova
		fldcw		newstat			// nastavenÌ novÈho ¯ÌdicÌho slova
		fistp		result			// p¯evod ËÌsla na celÈ ËÌslo
		fldcw		oldstat			// navr·cenÌ p˘vodnÌho ¯ÌdicÌho slova
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
// p¯evod re·lnÈho ËÌsla na celÈ ËÌslo se zaokrouhlenÌm dol˘ (k z·pornÈ hodnotÏ)

int RoundM(double num)
{
#ifdef _M_IX86

	WORD		oldstat;			// star˝ stav ¯ÌdicÌho slova
	WORD		newstat;			// nov˝ stav ¯ÌdicÌho slova
	int			result;				// v˝sledek operace

	_asm {
		fld			num				// naËtenÌ ËÌsla k provedenÌ operace
		wait						// synchronizace
		fnstcw		oldstat			// uloûeni ¯ÌdicÌho slova
		wait						// synchronizace
		mov			ax,oldstat		// star˝ stav ¯ÌdicÌho slova
		and			ah,not 0xc;		// vynulov·nÌ ¯ÌdicÌch bit˘ pro zaokrouhlenÌ
		or			ah,4			// mÛd zaokrouhlenÌ smÏrem dol˘
		mov			newstat,ax		// nov˝ stav ¯ÌdicÌho slova
		fldcw		newstat			// nastavenÌ novÈho ¯ÌdicÌho slova
		fistp		result			// p¯evod ËÌsla na celÈ ËÌslo
		fldcw		oldstat			// navr·cenÌ p˘vodnÌho ¯ÌdicÌho slova
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
