
#include "Main.h"

/***************************************************************************\
*																			*
*							Obsluha ikon a bitmap							*
*																			*
\***************************************************************************/


////////////////////////////////////////////////////////////////////
// test platnosti záhlaví BMP (TRUE=je OK)

BOOL TestBMP(const BITMAPINFO* bitmap)
{
	return 
		(
			(bitmap->bmiHeader.biWidth > 0) &&
			(bitmap->bmiHeader.biHeight > 0) &&
			(bitmap->bmiHeader.biWidth <= 32768) &&
			(bitmap->bmiHeader.biHeight <= 32768) &&
			(bitmap->bmiHeader.biPlanes == 1) &&
			(
				(bitmap->bmiHeader.biBitCount == 1) ||
				(bitmap->bmiHeader.biBitCount == 4) ||
				(bitmap->bmiHeader.biBitCount == 8) ||
				(bitmap->bmiHeader.biBitCount == 16) ||
				(bitmap->bmiHeader.biBitCount == 24) ||
				(bitmap->bmiHeader.biBitCount == 32)
			) &&
			(
				(bitmap->bmiHeader.biCompression == BI_RGB) ||
				(
					(bitmap->bmiHeader.biCompression == BI_RLE8) &&
					(bitmap->bmiHeader.biBitCount == 8)
				) ||
				(
					(bitmap->bmiHeader.biCompression == BI_RLE4) &&
					(bitmap->bmiHeader.biBitCount == 4)
				)
			)
		);
}


////////////////////////////////////////////////////////////////////
// dekomprese dat bitmapy BI_RLE8

void DekompRLE8(BYTE* dst, int dstN, BYTE* src, int srcN)
{
	BYTE data;
	BYTE num;
	int lineoff = 0;				// bodový offset na lince

	for (;;)
	{
		if ((dstN <= 0) || (srcN <= 0)) break;

// naètení prvního a druhého bajtu
		num = *src;
		src++;
		srcN--;

		data = *src;
		src++;
		srcN--;

// opakování bajtu
		if (num != 0)
		{
			lineoff += num;
			dstN -= num;

			MemFill(dst, num, data);
			dst += num;
		}
		else
		{

// pøenos bajtù beze zmìny
			if (data > 2)
			{
				lineoff += data;
				dstN -= data;
				srcN -= data;

				MemCopy(dst, src, data);
				dst += data;
				src += (data + 1) & ~1;
			}
			else
			{

// escape kód - konec linky
				if (data == 0)
				{
					num = (BYTE)(3 - ((lineoff+3) & 3));
					dstN -= num;
					MemFill(dst, num, 0);
					dst += num;
					lineoff = 0;
				}
				else
				{

// escape kód - konec bitmapu
					if (data == 1)
					{
						break;
					}
				}
			}
		}
	}
}


////////////////////////////////////////////////////////////////////
// pøenesení bodu

inline void RLE4Store(BOOL& firstdst, BOOL& firstsrc, BYTE*& dst, const BYTE& data)
{
	if (firstsrc)
	{
		if (firstdst)
		{
			*dst = (BYTE)(data & 0xf0);
			firstdst = FALSE;
		}
		else
		{
			*dst |= (BYTE)(data >> 4);
			firstdst = TRUE;
			dst++;
		}
		firstsrc = FALSE;
	}
	else
	{
		if (firstdst)
		{
			*dst = (BYTE)(data << 4);
			firstdst = FALSE;
		}
		else
		{
			*dst |= (BYTE)(data & 0xf);
			firstdst = TRUE;
			dst++;
		}
		firstsrc = TRUE;
	}
}


////////////////////////////////////////////////////////////////////
// dekomprese dat bitmapy BI_RLE4

void DekompRLE4(BYTE* dst, int dstN, BYTE* src, int srcN)
{
	BYTE data;						// data
	BYTE olddata;
	BYTE num;						// poèet dat
	int lineoff = 0;				// bodový offset na lince

	BOOL firstdst = TRUE;			// ukládá se první tetráda bajtu
	BOOL firstsrc;					// naèítá se první tetráda bajtu

// dokud jsou platná data (to je jen pøibližná kontrola, musí být rezerva)
	while ((dstN > 0) && (srcN > 0))
	{

// naètení prvního a druhého bajtu
		num = *src;
		src++;
		srcN--;

		data = *src;
		src++;
		srcN--;

		firstsrc = TRUE;			// naèítá se první tetráda bajtu

// opakování bajtu
		if (num != 0)
		{
			lineoff += num;
			dstN -= num/2;

			for (; num > 0; num--)
			{
				RLE4Store(firstdst, firstsrc, dst, data);
			}
		}
		else
		{

// pøenos bajtù beze zmìny
			if (data > 2)
			{
				lineoff += data;

				olddata = data;
				srcN -= ((data + 3) & ~3)/2;
				num = (BYTE)(data/2);
				dstN -= num;

				for (; num > 0; num--)
				{
					data = *src;
					src++;
					RLE4Store(firstdst, firstsrc, dst, data);
					RLE4Store(firstdst, firstsrc, dst, data);
				}

				if (olddata & 1)
				{
					data = *src;
					src++;
					RLE4Store(firstdst, firstsrc, dst, data);
				}
				src += (3 - ((olddata-1) & 3))/2;
			}
			else
			{

// escape kód - konec linky
				if (data == 0)
				{
					for (lineoff = 7 - ((lineoff+7) & 7); lineoff > 0; lineoff--)
					{
						RLE4Store(firstdst, firstsrc, dst, data);
					}
				}
				else
				{

// escape kód - konec bitmapu
					if (data == 1)
					{
						break;
					}
				}
			}
		}
	}
}


////////////////////////////////////////////////////////////////////
// komprese bitmapy RLE8

class CRle8
{
public:

	BYTE*	m_Dst;				// ukládací adresa do výstupního bufferu
	BYTE*	m_Src;				// ètecí adresa neuložených dat
	BYTE	m_Neshoda;			// délka neshody
	BYTE	m_Shoda;			// délka shody

// uložení bajtu do výstupního bufferu
	inline void Store(BYTE data);

// pøenesení bajtu neuložených dat
	inline void Move();

// vyprázdnìní bufferu neshody
	void FlushNeshoda();

// zaøazení bajtu do bufferu neshody
	inline void StoreNeshoda();

// vyprázdnìní bufferu shody
	void FlushShoda();
};


////////////////////////////////////////////////////////////////////
// uložení bajtu do výstupního bufferu

inline void CRle8::Store(BYTE data)
{
	*m_Dst = data;
	m_Dst++;
}


////////////////////////////////////////////////////////////////////
// pøenesení bajtu neuložených dat

inline void CRle8::Move()
{
	*m_Dst = *m_Src;
	m_Dst++;
	m_Src++;
}


////////////////////////////////////////////////////////////////////
// vyprázdnìní bufferu neshody

void CRle8::FlushNeshoda()
{
	switch (m_Neshoda)
	{
	case 0:
		return;
		
	case 1:					// 2 B
		Store(1);
		Move();
		break;

	case 2:					// 4 B
		Store(1);
		Move();
		Store(1);
		Move();
		break;

	case 3:					// 6 B
		Store(0);
		Store(3);
		MemCopy(m_Dst, m_Src, 3);
		m_Dst += 3;
		m_Src += 3;
		Store(0);
		break;

	case 4:					// 6 B
		Store(0);
		Store(4);
		MemCopy(m_Dst, m_Src, 4);
		m_Dst += 4;
		m_Src += 4;
		break;

	case 5:					// 8 B
		Store(0);
		Store(5);
		MemCopy(m_Dst, m_Src, 5);
		m_Dst += 5;
		m_Src += 5;
		Store(0);
		break;

	case 6:					// 8 B
		Store(0);
		Store(6);
		MemCopy(m_Dst, m_Src, 6);
		m_Dst += 6;
		m_Src += 6;
		break;

	case 7:					// 10 B
		Store(0);
		Store(7);
		MemCopy(m_Dst, m_Src, 7);
		m_Dst += 7;
		m_Src += 7;
		Store(0);
		break;

	case 8:					// 10 B
		Store(0);
		Store(8);
		MemCopy(m_Dst, m_Src, 8);
		m_Dst += 8;
		m_Src += 8;
		break;

	default:
		Store(0);
		Store(m_Neshoda);
		MemCopy(m_Dst, m_Src, m_Neshoda);
		m_Dst += m_Neshoda;
		m_Src += m_Neshoda;
		if (m_Neshoda & 1)
		{
			Store(0);
		}
	}
	m_Neshoda = 0;
}


////////////////////////////////////////////////////////////////////
// zaøazení bajtu do bufferu neshody

inline void CRle8::StoreNeshoda()
{
	m_Neshoda++;
	if (m_Neshoda == 255)
	{
		FlushNeshoda();
	}
}


////////////////////////////////////////////////////////////////////
// vyprázdnìní bufferu shody

void CRle8::FlushShoda()
{
	switch (m_Shoda)
	{
	case 0:
		return;

	case 1:
		StoreNeshoda();
		break;

	case 2:
		switch (m_Neshoda)
		{
		case 0:						// 2 B
			Store(2);
			Move();
			m_Src++;
			break;

		case 1:						// 4 B
			FlushNeshoda();
			Store(2);
			Move();
			m_Src++;
			break;

		default:
			StoreNeshoda();
			StoreNeshoda();
		}
		break;

	case 3:
		switch (m_Neshoda)
		{
		case 0:						// 2 B
			Store(3);
			Move();
			m_Src += 2;
			break;

		case 1:						// 4 B
		case 2:						// 6 B
			FlushNeshoda();
			Store(3);
			Move();
			m_Src += 2;
			break;

		default:
			StoreNeshoda();
			StoreNeshoda();
			StoreNeshoda();
		}
		break;

	default:
		FlushNeshoda();
		Store(m_Shoda);
		*m_Dst = *m_Src;
		m_Dst++;
		m_Src += m_Shoda;
	}
	m_Shoda = 0;
}

////////////////////////////////////////////////////////////////////
// komprese dat bitmapy BI_RLE8, vrací velikost dat

int KompRLE8(BYTE* dstbuf, BYTE* srcbuf, int width, int height)
{
// buffer komprese
	CRle8 rle;
	rle.m_Dst = dstbuf;						// ukládací adresa
	rle.m_Src = srcbuf;						// ètecí adersa
	rle.m_Neshoda = 0;						// délka neshody
	rle.m_Shoda = 0;						// délka shody

// lokální promìnné
	BYTE* src = srcbuf;							// ètecí adresa

// cyklus pøes všechny linky
	for (int i = height; i > 0; i--)
	{

// cyklus pøes všechny pozice na lince
		for (int j = width; j > 0; j--)
		{

// buffer shody je prázdný
			if (rle.m_Shoda == 0)
			{
				rle.m_Shoda = 1;
			}
			else

// test, zda je shodný bajt
			{
				if (*src == src[-1])
				{
					rle.m_Shoda++;
					if (rle.m_Shoda == 255)
					{
						rle.FlushShoda();
					}
				}
				else
				{
					rle.FlushShoda();
					rle.m_Shoda = 1;
				}
			}
			src++;
		}

// konec linky - vyprázdnìní bufferù
		rle.FlushShoda();
		rle.FlushNeshoda();

// uložení kódu pro konec linky
		rle.Store(0);
		rle.Store(0);
	}

// uložení kódu pro konec bitmapy
	rle.Store(0);
	rle.Store(1);

// návrat délky uložených dat
	return (rle.m_Dst - dstbuf);
}
