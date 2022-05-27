
/***************************************************************************\
*																			*
*								Dopl�ky j�dra								*
*																			*
\***************************************************************************/

// Zde jsou dopl�ky vy�adovan� p�eklada�em z j�dra C++
// POZOR - tento soubor mus� zachovat jm�no STUB.C !!!!

int _fltused = 0x9875;
//int _ldused = 0x9873;
int __fastflag = 0;
//int _errno = 0;

int errno = 0;
int* _errno() { return &errno; };


//int _except_list;
//int __CxxFrameHandler = 0;

// korekce chyb procesoru Pentuim
int _adjust_fdiv = 0;		// 1 = korigovat chyby instrukc� Pentia

// nevolat funkci p�i inicializaci programu - je to destruktor 
//int atexit(void(*fnc())){ /*fnc();*/ fnc; return 0; };

int atexit(void*(__cdecl *fnc)())
{
	return (fnc == (void*)0) ? -1 : 0;
}

//void* memset(void* dst, int c, unsigned count);
//{
//	char* dst2 = (char*)dst;
//	for (; count > 0; count--)
//	{
//		*dst2 = (char)c;
//		dst2++;
//	}
//	return dst;
//}

#ifdef _M_IX86

__int64 _ftol() 
{
	unsigned short	oldstat;		// star� stav ��dic�ho slova
	unsigned short	newstat;		// nov� stav ��dic�ho slova
	__int64			result;			// v�sledek operace

	_asm {
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
};

#endif


//double __cdecl _fltinf(const char*, int, int, int);

//double strtod(const char* text)
//{
//	return _fltin(text, 10, 0, 0);
//}

//double StrToDouble(LPCTSTR text)
//{
//#ifdef _UNICODE
//	return wcstod(text,0);
//#else
//	return strtod(text);
/*
char ctype = 'A';
char* _pctype = &ctype;
char __decimal_point = '.';

int __mb_cur_max = 2;

// ur�en� typu znaku:
// -----------------
//	_UPPER	0x01
//	_LOWER	0x02
//	_DIGIT	0x04
//	_SPACE	0x08
//	_PUNCT	0x10
// _CONTROL 0x20
//  _BLANK	0x40
//  _HEX    0x80
//  _LEADBYTE 0x8000
//  _ALPHA  (0x0100 | _UPPER | _LOWER)

int __cdecl _isctype(int c, int mask)
{
	int kod = 0;
	if ((c >= 'A') && (c <= 'Z')) kod |= 1;
	if ((c >= 'a') && (c <= 'z')) kod |= 2;
	if ((c >= '0') && (c <= '9')) kod |= 4;
	if ((c == ' ') || (c == 9)) kod |= 8;
	if ((c == '.') || (c == ',')) kod |= 0x10;
	if (c < ' ') kod |= 0x20;

	return (kod & mask);
}
*/

// Importy z knihoven:
// -------------------
// "..\lib\tran.lib" (obsad� 12 KB):
// __int64 _ftol(double n) { n; return 0; };
// double _CIfmod(double n) { return n; };
// double _CIasin(double n) { return n; };
// double _CIacos(double n) { return n; };
// double _CIpow(double n) { return n; };

// "comctl32.lib":
// __imp_CreateStatusWindowA
	
// "winmm.lib":
// __imp_mciSendCommandA

// "gdi32.lib":
// __imp__CreatePalette
// __imp__RealizePalette
// __imp__SelectPalette
// __imp__DeleteObject
// __imp__StretchDIBits
// __imp__SetStretchBltMode

// "user32.lib":
// u�ivatelsk� interface (okna, kurzor, kl�vesnice)

// "kernel32.lib"
// syst�mov� obsluha (soubory, texty, pam�, syst�m)

// zru�en� knihovny:
// ----------------
// "uuid.lib"
// "shell32.lib"
// "advapi32.lib"
// "comdlg32.lib"
// "winspool.lib"
