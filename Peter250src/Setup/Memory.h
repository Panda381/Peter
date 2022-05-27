
/***************************************************************************\
*																			*
*								Obsluha pam�ti								*
*																			*
\***************************************************************************/

//////////////////////////////////////////////////////////////////////////////
// p�ep�na�e preprocesoru:
//	_DEBUG ............. je debugger
//	_OPTIM ............. je optimalizace
//	_MT ................ v�cetokov� re�im
//	_UNICODE ........... k�dov�n� znak� UNICODE
//
//	_M_IX86 ............ procesor Intel 86
//	_M_ALPHA ........... procesor DEC Alpha
//	_M_MPPC ............ procesor Power Macintosh
//	_M_MRX000 .......... procesor MIPS
//	_M_PPC ............. procesor PowerPC


extern BOOL MemoryError;		// p��znak chyby pam�ti

extern BOOL MemoryOK;			// p��znak platn�ho spr�vce pam�ti

/////////////////////////////////////////////////////////////////////////////
// inicializace spr�vce pam�ti (TRUE=inicializace OK)

BOOL MemInit();


/////////////////////////////////////////////////////////////////////////////
// ukon�en� spr�vce pam�ti

void MemTerm();


/////////////////////////////////////////////////////////////////////////////
// p�id�len� bloku pam�ti (povolena 0 - vr�t� mal� blok) (vrac� NULL=chyba)

void* _fastcall MemGet(int size);


/////////////////////////////////////////////////////////////////////////////
// uvoln�n� bloku pam�ti (povolena adresa NULL - ignoruje se)

void _fastcall MemFree(void* adr);


/////////////////////////////////////////////////////////////////////////////
// zm�na velikosti bloku pam�ti (adresa NULL = vytvo�en�, velikost 0 = zru�en�)
// vrac� NULL=p��znak chyby (nebo zru�en), star� blok p�i chyb� nebude zm�n�n

void* _fastcall MemSize(void* adr, int size);


/////////////////////////////////////////////////////////////////////////////
// v�cetokov� inkrementace ��sla LONG INT

#ifdef _MT
#ifdef _M_IX86

void _fastcall LongIncrement(long* num);

#else	// _M_IX86

inline void LongIncrement(long* num)
{
	::InterlockedIncrement(num);
}

#endif	// _M_IX86
#else	// _MT

inline void LongIncrement(long* num)
{
	(*num)++;
}

#endif	// _MT


/////////////////////////////////////////////////////////////////////////////
// v�cetokov� dekrementace ��sla LONG INT (vrac� TRUE = v�sledek je 0)

#ifdef _MT
#ifdef _M_IX86

BOOL _fastcall LongDecrement(long* num);

#else	// _M_IX86

inline BOOL LongDecrement(long* num)
{
	return (::InterlockedDecrement(num) == 0);
}

#endif	// _M_IX86
#else	// _MT

inline BOOL LongDecrement(long* num)
{
	return (--(*num) == 0);
}

#endif	// _MT


/////////////////////////////////////////////////////////////////////////////
// napln�n� bloku pam�ti konstantou (p�i optimalizaci je nahrazeno instrukc� REP STOS)

inline void _fastcall MemFill(void* dst, int count, char val)
{
	char* dst1 = (char*)dst;

	for (; count > 0; count--)
	{
		*dst1 = val;
		dst1++;
	}
}

inline void _fastcall MemFillShort(void* dst, int count, short val)
{
	short* dst2 = (short*)dst;

	for (; count > 0; count--)
	{
		*dst2 = val;
		dst2++;
	}
}

inline void _fastcall MemFillLong(void* dst, int count, long val)
{
	long* dst4 = (long*)dst;

	for (; count > 0; count--)
	{
		*dst4 = val;
		dst4++;
	}
}


/////////////////////////////////////////////////////////////////////////////
// kopie bloku dat 
// 1) Pozor - delka nesm� b�t z�porn� (d�lka je ��slo bez znam�nka)!!!!!
// 2) kopie prob�h� v�dy od ni���ch adres k vy���m
// 3) P�i optimalizaci je nahrazeno instrukc� REP MOVS. Data se p�en��
//    po dvojslovech a proto se m�rn� li�� od kopie bloku dat po bajtech
//	  v p��pad�, �e zdrojov� adresa je vy��� ne� c�lov� o 1 a� 3 (p�ekryv)

inline void _fastcall MemCopy(void* dst, const void* src, DWORD count)
{
#ifdef _OPTIM
	memcpy(dst, src, count);
#else
	long* dst4 = (long*)dst;
	const long* src4 = (const long*)src;

	DWORD count4 = count/4;

	for (; count4 != 0; count4--)
	{
		*dst4 = *src4;
		dst4++;
		src4++;
	}

	char* dst1 = (char*)dst4;
	const char* src1 = (const char*)src4;

	DWORD count1 = count & 3;

	for (; count1 != 0; count1--)
	{
		*dst1 = *src1;
		dst1++;
		src1++;
	}
#endif
}	

// Pozn.: MemCopyShort by byl p�elo�en stejn� jako MemCopy, nem� proto v�znam

//inline void _fastcall MemCopyLong(void* dst, const void* src, DWORD count)
//{
//#ifdef _OPTIM
//	memcpy(dst, src, 4*count);
//#else
//	long* dst4 = (long*)dst;
//	const long* src4 = (const long*)src;
//
//	for (; count != 0; count--)
//	{
//		*dst4 = *src4;
//		dst4++;
//		src4++;
//	}
//#endif
//}	

//#define MemCopyInt MemCopyLong

/////////////////////////////////////////////////////////////////////////////
// posun bloku dat bez p�ekryvu (nahoru nebo dol�)
// (funkce se pou��v� m�lo, nen� proto p��li� optimalizov�na)

inline void MemMove(void* dst, const void* src, int count)
{
	if ((dst <= src) || ((char*)dst >= ((char*)src + count)))
	{
		for (; count > 0; count--)
		{
			*(char*)dst = *(char*)src;
			dst = (char*)dst + 1;
			src = (char*)src + 1;
		}
	}
	else
	{
		dst = (char*)dst + count - 1;
		src = (char*)src + count - 1;

		for (; count > 0; count--)
		{
			*(char*)dst = *(char*)src;
			dst = (char*)dst - 1;
			src = (char*)src - 1;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// porovn�n� bloku dat (p�i shod� nebo pro d�lku <= 0 vrac� TRUE)

inline BOOL _fastcall MemCompare(const void* buf1, const void* buf2, int count)
{
#ifdef _OPTIM
	return ((count <= 0) || (memcmp(buf1, buf2, count) == 0));
#else
	for (; ((count > 0) && (*(char*)buf1 == *(char*)buf2)); count--)
	{
		buf1 = (char*)buf1 + 1;
		buf2 = (char*)buf2 + 1;
	}
	return (count <= 0);
#endif
}
