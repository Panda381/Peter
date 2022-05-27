
/***************************************************************************\
*																			*
*								Doplòky jádra								*
*																			*
\***************************************************************************/

// Zde jsou doplòky vyžadované pøekladaèem z jádra C++
// POZOR - tento soubor musí zachovat jméno STUB.C !!!!

int _fltused = 0x9875;
//int _ldused = 0x9873;
int __fastflag = 0;
//int _errno = 0;
int errno = 0;
int* _errno() { return &errno; };
//int _except_list;
//int __CxxFrameHandler = 0;

// øízení korekcí chyb procesoru
int _adjust_fdiv = 0;		// 1 = korigovat chyby instrukcí Pentia

// nevolat funkci pøi inicializaci programu - je to destruktor 
//int atexit(void(*fnc())){ /*fnc();*/ fnc; return 0; };

int atexit(void*(__cdecl *fnc)())
{
	return (fnc == (void*)0) ? -1 : 0;
}

void _chkesp()
{
}

//int atexit;
//int atexit() { return 0; };

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

// urèení typu znaku:
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
