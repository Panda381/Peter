
/***************************************************************************\
*																			*
*								Doplòky jádra								*
*																			*
\***************************************************************************/

// Zde jsou doplòky vyžadované pøekladaèem z jádra C++
// POZOR - tento soubor musí zachovat jméno STUB.C !!!!

#pragma warning ( disable: 4100)		// hlášení - nepoužitý formální parametr

int _fltused = 0x9875;
int __fastflag = 0;
int errno = 0;
int* _errno() { return &errno; };

/*
#ifdef _M_IX86

typedef unsigned long DWORD;
typedef unsigned char BYTE;

typedef enum _EXCEPTION_DISPOSITION {
	ExceptionContinueExecution,
	ExceptionContinueSearch,
	ExceptionNestedException,
	ExceptionCollidedUnwind
} EXCEPTION_DISPOSITION;

#define EXCEPTION_NONCONTINUABLE 0x1    // Noncontinuable exception
#define EXCEPTION_MAXIMUM_PARAMETERS 15 // maximum number of exception parameters

typedef struct _EXCEPTION_RECORD {
    DWORD						ExceptionCode;
    DWORD						ExceptionFlags;
    struct _EXCEPTION_RECORD	*ExceptionRecord;
    void*						ExceptionAddress;
    DWORD						NumberParameters;
    unsigned __int64			ExceptionInformation[EXCEPTION_MAXIMUM_PARAMETERS];
    } EXCEPTION_RECORD;

typedef EXCEPTION_RECORD *PEXCEPTION_RECORD;

#define SIZE_OF_80387_REGISTERS      80

#define MAXIMUM_SUPPORTED_EXTENSION     512

typedef struct _FLOATING_SAVE_AREA {
    DWORD   ControlWord;
    DWORD   StatusWord;
    DWORD   TagWord;
    DWORD   ErrorOffset;
    DWORD   ErrorSelector;
    DWORD   DataOffset;
    DWORD   DataSelector;
    BYTE    RegisterArea[SIZE_OF_80387_REGISTERS];
    DWORD   Cr0NpxState;
} FLOATING_SAVE_AREA;

typedef FLOATING_SAVE_AREA *PFLOATING_SAVE_AREA;

typedef struct _CONTEXT {
    DWORD ContextFlags;

    DWORD   Dr0;
    DWORD   Dr1;
    DWORD   Dr2;
    DWORD   Dr3;
    DWORD   Dr6;
    DWORD   Dr7;

    FLOATING_SAVE_AREA FloatSave;

    DWORD   SegGs;
    DWORD   SegFs;
    DWORD   SegEs;
    DWORD   SegDs;

    DWORD   Edi;
    DWORD   Esi;
    DWORD   Ebx;
    DWORD   Edx;
    DWORD   Ecx;
    DWORD   Eax;

    DWORD   Ebp;
    DWORD   Eip;
    DWORD   SegCs;              // MUST BE SANITIZED
    DWORD   EFlags;             // MUST BE SANITIZED
    DWORD   Esp;
    DWORD   SegSs;

    BYTE    ExtendedRegisters[MAXIMUM_SUPPORTED_EXTENSION];

} CONTEXT;

EXCEPTION_DISPOSITION __cdecl _except_handler3 (
	struct _EXCEPTION_RECORD* ExceptionRecord,
	void* EstablisherFrame,
	struct _CONTEXT* ContextRecord,
	void* DispatcherContext
	)
{
	_asm {
		mov		ecx,EstablisherFrame
		cmp		dword ptr [ecx + 12],0
		jne		DALE
		mov		edx,ExceptionRecord
		mov		eax,[edx] 
		lea		ebp,[ecx + 16]
		mov		edx,[ecx + 8]
		jmp		dword ptr [edx + 8]
	DALE:
	}

	return ExceptionContinueSearch;
}

#endif // _M_IX86
*/

// øízení korekcí chyb procesoru
int _adjust_fdiv = 0;		// 1 = korigovat chyby instrukcí Pentia

// nevolat funkci pøi inicializaci programu - je to destruktor 
int atexit(void*(__cdecl *fnc)())
{
	return (fnc == (void*)0) ? -1 : 0;
}
