
/***************************************************************************\
*																			*
*								Obsluha pam�ti								*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inicializace spr�vce pam�ti

BOOL MemInit();


/////////////////////////////////////////////////////////////////////////////
// ukon�en� spr�vce pam�ti

void MemTerm();


/////////////////////////////////////////////////////////////////////////////
// p�id�len� bloku pam�ti (povolena 0) (NULL=chyba)

void* _fastcall MemGet(int size);


/////////////////////////////////////////////////////////////////////////////
// uvoln�n� bloku pam�ti (povoleno NULL)

void _fastcall MemFree(void* adr);


/////////////////////////////////////////////////////////////////////////////
// zm�na velikosti bloku pam�ti (povolena adresa NULL i velikost 0)

void* _fastcall MemSize(void* adr, int size);


/////////////////////////////////////////////////////////////////////////////
// zm�na velikosti bufferu prvk� (zad�v� se reference na ukazatel a po�et prvk�)

#define MemBuf(adr, items)	*((void**)&adr) = MemSize((void*)adr, (items) * sizeof(adr[0]))


/////////////////////////////////////////////////////////////////////////////
// oper�tor new

inline void* operator new (unsigned int size)
{
	return MemGet(size);
}


/////////////////////////////////////////////////////////////////////////////
// oper�tor delete

inline void operator delete (void* adr)
{
	MemFree(adr);
}


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
// napln�n� bloku pam�ti konstantou

inline void MemFill(void* dst, int count, char val = 0)
{
#ifdef _OPTIM
	memset(dst, val, count);
#else
	for (; count > 0; count--)
	{
		*(char*)dst = val;
		dst = (char*)dst + 1;
	}
#endif
}


/////////////////////////////////////////////////////////////////////////////
// kopie bloku dat

inline void MemCopy(void* dst, const void* src, int count)
{
#ifdef _OPTIM
	memcpy(dst, src, count);
#else
	for (; count > 0; count--)
	{
		*(char*)dst = *(char*)src;
		dst = (char*)dst + 1;
		src = (char*)src + 1;
	}
#endif
}	


/////////////////////////////////////////////////////////////////////////////
// p�esun bloku dat

inline void MemMove(void* dst, const void* src, int count)
{
	if ((dst <= src) || ((char*)dst >= ((char*)src + count)))
	{
#ifdef _OPTIM
		memcpy(dst, src, count);
#else
		for (; count > 0; count--)
		{
			*(char*)dst = *(char*)src;
			dst = (char*)dst + 1;
			src = (char*)src + 1;
		}
#endif
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
// porovn�n� bloku dat (TRUE=shoda)

inline BOOL MemCompare(const void* buf1, const void* buf2, int count)
{
#ifdef _OPTIM
	return memcmp(buf1, buf2, count) == 0;
#else
	for (; ((count > 0) && (*(char*)buf1 == *(char*)buf2)); count--)
	{
		buf1 = (char*)buf1 + 1;
		buf2 = (char*)buf2 + 1;
	}
	return (count == 0);
#endif
}
