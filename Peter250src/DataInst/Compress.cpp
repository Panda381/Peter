
#include "Main.h"


/***************************************************************************\
*																			*
*							Komprese/dekomprese dat							*
*					(pozor - nepodporuje v�cetokov� re�im!)					*
*																			*
\***************************************************************************/


//////////////////////////////////////////////////////////////////////////////
// k�dovac� diagram:
/*
						  |
	+----------0--------(1 bit)-----1-----------+
 p�enos										opakov�n�
n�sleduj�c�ho									|
  bajtu						+-----------0----(1 bit)-----------1----------------+
						opakov�n�											opakov�n�
						p�ede�l�ho											p�ede�l�
						  bajtu												  linky
						    |													|
			+-------0-----(1 bit)---1---+							+-----0--(1 bit)------1-----+
		po�et=1							|						 d�lka=1						|
						+-----000----(3 bity)---jinak---+					 +-----000-----(3 bity)----jinak---+
						|				|				|					 |					|			   |
						|			   001				|					 |				   001			   |
					 po�et=15			|		 po�et=2 a� 7			d�lka=15				|		   d�lka=2 a� 7
									1x opakov�n�										  1x opakov�n�
								  p�ed-p�ede�l�ho										 dal��ho bajtu
									  bajtu												 p�ede�l� linky
*/

#define SUBSTLEN 22		// n�hradn� d�lka (pro k�d = 0)

//////////////////////////////////////////////////////////////////////////////
// lok�ln� prom�nn� pro kompresi

// v�stupn� buffer
BYTE*	DstBuf;					// adresa v�stupn�ho bufferu
BYTE*	DstAdr;					// ukl�dac� adresa do v�stupn�ho bufferu
int		DstNum;					// velikost zbyl�ch v�stupn�ch dat (p�i dekompresi)

// vstupn� buffer
BYTE*	SrcBuf;					// adresa vstupn�ho bufferu
BYTE*	SrcAdr;					// �tec� adresa ze vstupn�ho bufferu
int		SrcNum;					// velikost zbyl�ch vstupn�ch dat (p�i kompresi)

// kompresn� buffer (prvn� 2 bajty je stavov� slovo st�ada�e bit�)
int		BitBit;					// po�et bit� ve stavov�m slov�
int		BitNum;					// po�et bajt� ve vyrovn�vac�m bufferu
BYTE	BitBuf[100];			// vyrovn�vac� v�stupn� buffer

// dekompresn� buffer
WORD	StatBit;				// stavov� slovo

//////////////////////////////////////////////////////////////////////////////
// z�pis vyrovn�vac�ho bufferu �et�zce

void WriteBuf()
{
	int n = BitNum;				// po�et bajt� ve vyrovn�vac�m bufferu
	MemCopy(DstAdr, BitBuf, n);	// z�pis dat do bufferu
	DstAdr += n;				// posun ukl�dac� adresy do bufferu
	BitNum = 2;					// nov� po�et bajt� v bufferu (= stavov� slovo)
}


//////////////////////////////////////////////////////////////////////////////
// z�pis skupiny bit� do stavov�ho slova

void _fastcall WriteBit(int data, int num)
{
// p�id�n� bit� ke st�ada�i bit�
	int i = BitBit;				// star� po�et bit� ve stavov�m slov�
	data <<= i;					// rotace nov�ch dat na pozici
	*(WORD*)BitBuf |= (WORD)data; // p�id�n� nov�ch dat

// p�i p�ete�en� po�tu bit� z�pis �et�zce
	i += num;					// nov� po�et bit�
	if (i > 16)
	{
		WriteBuf();				// z�pis vyrovn�vac�ho bufferu
		i -= 16;				// sn�en� po�et bit� ve st�ada�i
		data >>= 16;			// neulo�en� data
		*(WORD*)BitBuf = (WORD)data; // zbyl� neulo�en� bity
	}

// nov� po�et bit� st�ada�e
	BitBit = i;
}


//////////////////////////////////////////////////////////////////////////////
// p�enos bajtu beze zm�ny

void WriteByte()
{
	WriteBit(0,1);
	BitBuf[BitNum] = *SrcAdr;
	SrcAdr++;
	SrcNum--;
	BitNum++;
}


//////////////////////////////////////////////////////////////////////////////
// komprese dat
//		dst = v�stupn� buffer (m�l by b�t minim�ln� o 1/8 v�t�� ne� vstupn� buffer)
//		src = vstupn� buffer s daty
//		size = velikost dat ke kompresi
//		width = d�lka linky bitmapy (pro offset p�ede�l� linky)
// v�stup: velikost dat ve v�stupn�m bufferu

int Compress(BYTE* dst, BYTE* src, int size, int width)
{
// v�stupn� buffer
	DstBuf = dst;
	DstAdr = dst;

// vstupn� buffer
	SrcBuf = src;
	SrcAdr = src;
	SrcNum = size;
	if (size <= 0) return 0;
	if ((width > size) || (width <= 1)) width = size;

// vyrovn�vac� buffer �et�zce
	BitBit = 0;					// po�et bit� ve stavov�m slov�
	BitNum = 2;					// po�et bajt� ve vyrovn�vac�m bufferu
	*(WORD*)BitBuf = 0;			// stavov� slovo (st�ada� bit�)

// prvn� bajt se p�enese beze zm�ny
	WriteByte();

// cyklus komprese pro prvn� linku
	for (int i = width; (i > 0) && (SrcNum > 0); i--)
	{

// opakov�n� p�ede�l�ho bajtu
		BYTE d = SrcAdr[-1];

		for (int j = 0; (j < SrcNum) && (d == SrcAdr[j]); j++) {};
		if (j > 0)
		{
			SrcAdr += j;
			SrcNum -= j;

			while (j > 0)
			{
				if (j == 1)
				{
					WriteBit(1, 3);
					j -= 1;
				}
				else
				{
					if (j >= SUBSTLEN)
					{
						WriteBit(5, 6);
						j -= SUBSTLEN;
					}
					else
					{
						int n = j;
						if (n > 7) n = 7;
						WriteBit((n << 3) + 5, 6);
						j -= n;
					}
				}
			}
		}

		else
		{

// opakov�n� p�ed-p�ede�l�ho bajtu
			if ((SrcAdr > SrcBuf + 1) && (SrcAdr[0] == SrcAdr[-2]))
			{
				WriteBit(0xd, 6);
				SrcAdr++;
				SrcNum--;
			}
			else
			{

// p�enos bajtu beze zm�ny
				WriteByte();
			}
		}
	}

// cyklus komprese dat
	for (; SrcNum > 0;)
	{

// zji�t�n� d�lky opakov�n� p�ede�l�ho bajtu
		BYTE d = SrcAdr[-1];
		int j;
		for (j = 0; (j < SrcNum) && (d == SrcAdr[j]); j++) {};

// zji�t�n� d�lky opakov�n� p�ede�l� linky
		int  k;
		for (k = 0; (k < SrcNum) && (SrcAdr[k-width] == SrcAdr[k]); k++) {};

// test, zda bude opakov�n�
		if ((j > 0) || (k > 0))
		{

// bude opakov�n� p�ede�l�ho bajtu
			if (j >= k)
			{
				SrcAdr += j;
				SrcNum -= j;

				while (j > 0)
				{
					if (j == 1)
					{
						WriteBit(1, 3);
						j -= 1;
					}
					else
					{
						if (j >= SUBSTLEN)
						{
							WriteBit(5, 6);
							j -= SUBSTLEN;
						}
						else
						{
							int n = j;
							if (n > 7) n = 7;
							WriteBit((n << 3) + 5, 6);
							j -= n;
						}
					}
				}
			}

// bude opakov�n� p�ede�l� linky
			else
			{
				SrcAdr += k;
				SrcNum -= k;

				while (k > 0)
				{
					if (k == 1)
					{
						WriteBit(3, 3);
						break;
					}
					else
					{
						if (k >= SUBSTLEN)
						{
							WriteBit(7, 6);
							k -= SUBSTLEN;
						}
						else
						{
							int n = k;
							if (n > 7) n = 7;
							WriteBit((n << 3) + 7, 6);
							k -= n;
						}
					}
				}
			}
		}
		else
		{

// opakov�n� p�ed-p�ede�l�ho bajtu
			if (SrcAdr[0] == SrcAdr[-2])
			{
				WriteBit(0xd, 6);
				SrcAdr++;
				SrcNum--;
			}
			else
			{

// opakov�n� dal��ho bajtu p�ede�l� linky
				if (SrcAdr[0] == SrcAdr[1-width])
				{
					WriteBit(0xf, 6);
					SrcAdr++;
					SrcNum--;
				}
				else
				{

// p�enos bajtu beze zm�ny
					WriteByte();
				}
			}
		}
	}

// vypr�zdn�n� zbytku dat
	WriteBuf();

// po�et bajt� ve v�stupn�m bufferu
	return (DstAdr-DstBuf);
}


//////////////////////////////////////////////////////////////////////////////
// na�ten� bitu p�i dekompresi

BOOL ReadBit()
{
	WORD statbit = StatBit;
	BOOL b = statbit & 1;
	statbit >>= 1;
	if (statbit == 0)
	{
		statbit = *(WORD*)SrcAdr;
		SrcAdr += 2;
		b = statbit & 1;
		statbit >>= 1;
		statbit |= 0x8000;
	}
	StatBit = statbit;
	return b;
}


//////////////////////////////////////////////////////////////////////////////
// dekomprese dat
//		dst = v�stupn� buffer
//		src = vstupn� buffer s daty
//		size = velikost v�stupn�ch dat (nezkomprimovan�ch)
//		width = d�lka linky bitmapy (pro offset p�ede�l� linky)

void DeCompress(BYTE* dst, BYTE* src, int size, int width)
{
// v�stupn� buffer
	DstBuf = dst;
	DstAdr = dst;

// vstupn� buffer
	SrcBuf = src;
	SrcAdr = src;
	SrcNum = size;
	if ((width > size) || (width <= 1)) width = size;
	width = -width;

	StatBit = 0;

// cyklus p�es po�adovan� data
	for (; size > 0;)
	{

// p�enos bajtu beze zm�ny
		if (!ReadBit())
		{
			*DstAdr = *SrcAdr;
			DstAdr++;
			SrcAdr++;
			size--;
		}

// bude opakov�n�
		else
		{

// bude opakov�n� p�ede�l�ho bajtu
			if (!ReadBit())
			{

// opakov�n� jednoho bajtu
				if (!ReadBit())
				{
					*DstAdr = DstAdr[-1];
					DstAdr++;
					size--;
				}

// na�ten� d�lky dat
				else
				{
					int n = 0;
					if (ReadBit()) n |= 1;
					if (ReadBit()) n |= 2;
					if (ReadBit()) n |= 4;

// opakov�n� bajtu
					switch (n)
					{
					case 1:
						*DstAdr = DstAdr[-2];
						DstAdr++;
						size--;
						break;

					case 0:
						n = SUBSTLEN;

					default:
						{
							size -= n;
							BYTE d = DstAdr[-1];
							do 
							{
								*DstAdr = d;
								DstAdr++;
								n--;
							} while (n > 0);
						}
					}
				}
			}

// bude opakov�n� p�ede�l� linky
			else
			{

// opakov�n� jednoho bajtu
				if (!ReadBit())
				{
					*DstAdr = DstAdr[width];
					DstAdr++;
					size--;
				}

// na�ten� d�lky dat
				else
				{
					int n = 0;
					if (ReadBit()) n |= 1;
					if (ReadBit()) n |= 2;
					if (ReadBit()) n |= 4;

// opakov�n� bajtu
					switch (n)
					{
					case 1:
						*DstAdr = DstAdr[1+width];
						DstAdr++;
						size--;
						break;

					case 0:
						n = SUBSTLEN;

					default:
						{
							size -= n;
							do 
							{
								*DstAdr = DstAdr[width];
								DstAdr++;
								n--;
							} while (n > 0);
						}
					}
				}
			}
		}
	}
}
