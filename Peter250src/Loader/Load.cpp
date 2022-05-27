
#include "Main.h"

/***************************************************************************\
*																			*
*								Obsluha souboru								*
*																			*
\***************************************************************************/

// zdrojov� buffer glob�ln�ch objekt�
	int			BufObjN;
	PETPROG*	BufObj;
	PETPROG2*	BufObj2;

// zdrojov� buffer lok�ln�ch objekt�
	int			BufLocN;
	PETPROG*	BufLoc;
	PETPROG2*	BufLoc2;

// zdrojov� buffer editoru
	int			BufEdiN;
	PETPROG*	BufEdi;
	PETPROG2*	BufEdi2;

/////////////////////////////////////////////////////////////////////////////
// zji�t�n� adresy za��tku dat bufferu

BYTE* BufAdr(int buf)
{
	BYTE* adr = (BYTE*)PetAdr + PetAdr->Data;
	PETINDEX* pi = PetAdr->pi;

	for (; buf > 0; buf--)
	{
		adr += pi->Delka;
		pi++;
	}

	return adr;
}


/////////////////////////////////////////////////////////////////////////////
// zmapov�n� programov�ho bufferu (napln�n� dopl�uj�c�ho bufferu)

void LoadProg(PETPROG* prog, PETPROG2* prog2, int num)
{
	if (num <= 0) return;

	PETPROG2* bufprog2 = prog2;

// buffer index� rodi��
	CBufInt	parent(num);
	parent[0] = -1;

// p��znaky, �e bude dal�� polo�ka stejn� hladiny
	CBufBool next(num);

// buffer index� k �schov� po�tu polo�ek
	CBufInt items(num);

// ukazatel hloubky vno�en�
	int level = 0;

// ukazatel indexu polo�ky
	int index = 0;

// cyklus p�es v�echny polo�ky
	for (; num > 0; num--)
	{

// nastaven� rodi�e, inicializace parametr�
		prog2->Parent = parent[level];
		prog2->Items = 1;
		prog2->Data = prog->DatIndex;
		prog2->List = -1;
		prog2->Local = -1;
		prog2->LocalList = -1;
		prog2->Funkce = -1;

// �schova p��znaku dal�� polo�ky
		next[level] = ((prog->Param & PETPROG_NEXT) != 0);

// vno�en� na potomka
		if (prog->Param & PETPROG_CHILDS)
		{
			items[level] = index;
			level++;
			parent[level] = index;
		}

// sn�en� ��ta�e rodi��
		else
		{
			while (!next[level] && (level > 0))
			{
				level--;
				int n = items[level];
				bufprog2[n].Items = index - n + 1;
			}
		}

// p��prava pro dal�� polo�ku
		index++;
		prog++;
		prog2++;
	}
	ASSERT(level == 0);
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� textov� konstanty (ukazatel� ukazuj� na polo�ku konstanty)

double LoadNum(PETPROG* prog, PETPROG2* prog2)
{
// �schova indexu jm�na polo�ky
	int name = prog->Name;

// p��prava st�ada�e v�sledku
	CString txt;

// cyklus p�es v�echny potomky prvku
	for (int i = prog2->Items - 1; i > 0; i--)
	{

// zv��en� ukazatele prvku
		prog++;

// pokud nen� prvek vypnut
		if ((prog->Param & (PETPROG_OFF | PETPROG_OFF_DEP)) == 0)
		{

// rozli�en� podle prvku
			switch (prog->Func)
			{
			case IDF_0 - IDF:		txt += _T('0');
									break;

			case IDF_1 - IDF:		txt += _T('1');
									break;

			case IDF_2 - IDF:		txt += _T('2');
									break;

			case IDF_3 - IDF:		txt += _T('3');
									break;

			case IDF_4 - IDF:		txt += _T('4');
									break;

			case IDF_5 - IDF:		txt += _T('5');
									break;

			case IDF_6 - IDF:		txt += _T('6');
									break;

			case IDF_7 - IDF:		txt += _T('7');
									break;

			case IDF_8 - IDF:		txt += _T('8');
									break;

			case IDF_9 - IDF:		txt += _T('9');
									break;

			case IDF_DOT - IDF:		txt += _T('.');
									break;

			case IDF_EXP - IDF:		txt += _T('E');
									break;

			case IDF_MINUS - IDF:	txt += _T('-');
									break;
			}
		}
	}

// pokud nebylo nic na�teno, pou�ije se text jm�na polo�ky
	if (txt.IsEmpty())
	{
		txt = Text.Get(name);
	}

// korekce samotn�ho "-" na "-1"
	if (txt == _T('-'))
	{
		txt += _T('1');
	}

// na�ten� ��sla
	return Double(txt);
}


/////////////////////////////////////////////////////////////////////////////
// zmapov�n� seznam�

void LoadList(PETPROG* prog, PETPROG2* prog2, int num, bool global)
{
// lok�ln� prom�nn�
	int	i;					// ��ta� polo�ek ve v�tvi seznamu
	int size;				// velikost seznamu
	double n;				// na�ten� ��slo
	int olddata;			// p�vodn� index dat
	PETPROG* prg;			// ukazatel polo�ek
	PETPROG2* prg2;			// ukazatel polo�ek
	int list;				// index vytvo�en�ho seznamu
	LISTDATA* plist;		// adresa vytvo�en�ho seznamu

// ukazatel indexu polo�ky
	int index = 0;

// cyklus p�es v�echny polo�ky
	for (; num > 0; num--)
	{

// test, zda je seznam
		if ((prog[index].Func == IDF_LIST - IDF) &&							// je to seznam?
			(prog2[index].List < 0) &&										// nen� vno�en�?
			((prog[index].Param & (PETPROG_OFF | PETPROG_OFF_DEP)) == 0))	// nen� vypnut�?
		{

// adresa prvn� polo�ky (= polo�ka seznamu)
			prg = prog + index;
			prg2 = prog2 + index;

// vytvo�en� seznamu
			list = List.New();								// vytvo�en� seznamu
			prg2->List = list;								// �schova indexu seznamu
			plist = &List[list];							// adresa seznamu

// nalezen� velikosti seznamu
			size = 1;										// p�ednastaven� pro p��pad ne�sp�chu
			for (i = prg2->Items - 1; i > 0; i--)
			{

// adresa dal��ho prvku
				prg++;
				prg2++;

// na�ten� rozm�ru seznamu
				if (prg->Func == IDF_LIST_SIZE - IDF)		// je to velikost seznamu?
				{
					n = LoadNum(prg, prg2);					// na�ten� velikosti seznamu
					if (n < 1) n = 1;						// minim�ln� velikost seznamu
					if (n > 0x7fffffff) n = 0x7fffffff;		// maxim�ln� ��slo int
					size = (int)(n + 0.5);					// velikost seznamu
// 040331 - odstaveno na radu MIN	if (size > 0x100000) size = 0x100000;	// maxim�ln� velikost seznamu
					plist->Size = size;						// �schova velikosti seznamu
					break;
				}
			}

// adresa prvn� polo�ky (= polo�ka seznamu)
			prg = prog + index;
			prg2 = prog2 + index;

// proj�t� v�ech potomk�
			for (i = prg2->Items - 1; i > 0; i--)
			{

// adresa dal��ho prvku
				prg++;
				prg2++;

// nastaven� odkazu na seznam
				prg2->List = list;

// obsluha prvku
				olddata = prg2->Data;
				switch (prg->Func)
				{

// na�ten� indexu seznamu
				case IDF_LIST_INDEX - IDF:
					n = Real.Get(olddata);
					if (n >= 0)
					{
						plist->Index = (int)(n + 0.5) % size;
					}
					else
					{
						plist->Index = (int)(n - 0.5) % size;
					}
					if (plist->Index < 0) plist->Index += size;
					break;

// na�ten� automatick� inkrementace seznamu
				case IDF_LIST_AUTO - IDF:
					n = Real.Get(olddata);
					plist->Auto = Round(n);
//					if (plist->Auto >= size) plist->Auto = size-1;
//					if (plist->Auto <= -size) plist->Auto = -(size-1);
					break;

// rozmno�en� �iseln� prom�nn�
				case IDF_REAL - IDF:
					if ((size > 1) && global)
					{
						prg2->Data = Real.Dup(olddata, size);
					}
					break;

// rozmno�en� textov� prom�nn�
				case IDF_TEXT - IDF:
					if ((size > 1) && global)
					{
						prg2->Data = Text.Dup(olddata, size);
					}
					break;

// rozmno�en� logick� prom�nn�
				case IDF_BOOL - IDF:
					if ((size > 1) && global)
					{
						prg2->Data = Bool.Dup(olddata, size);
					}
					break;

// rozmno�en� prom�nn� ikony
				case IDF_ICON - IDF:
					if ((size > 1) && global)
					{
						prg2->Data = Icon.Dup(olddata, size);
					}
					break;

// rozmno�en� prom�nn� plochy
				case IDF_MAP - IDF:
					if ((size > 1) && global)
					{
						prg2->Data = Map.Dup(olddata, size);
					}
					break;

// rozmno�en� prom�nn� obr�zku
				case IDF_PIC - IDF:
					if ((size > 1) && global)
					{
						prg2->Data = Picture.Dup(olddata, size);
					}
					break;

// rozmno�en� prom�nn� sprajtu
				case IDF_SPRITE - IDF:
					if ((size > 1) && global)
					{
						prg2->Data = Sprite.Dup(olddata, size);
					}
					break;

// rozmno�en� prom�nn� zvuku
				case IDF_SND - IDF:
					if ((size > 1) && global)
					{
						prg2->Data = Sound.Dup(olddata, size);
					}
					break;

// rozmno�en� prom�nn� zvuku
				case IDF_MUS - IDF:
					if ((size > 1) && global)
					{
						prg2->Data = Music.Dup(olddata, size);
					}
					break;
				}
			}
		}

// p��prava pro dal�� polo�ku
		index++;
	}
}


/////////////////////////////////////////////////////////////////////////////
// zmapov�n� lok�ln�ch objekt�

void LoadLocal()
{
// p��prava ukazatel�
	PETPROG* prog = BufLoc;
	PETPROG2* prog2 = BufLoc2;
	int index = 0;
	int i;

// cyklus p�es v�echny lok�ln� funkce
	while (index < BufLocN)
	{

// p��prava ��ta�� indexu lok�ln� prom�nn�
		int	loclst = 0;
		int locnum = 0;
		int loctxt = 0;
		int loclog = 0;
		int locico = 0;
		int locmap = 0;
		int locpic = 0;
		int locspr = 0;
		int locsnd = 0;
		int locmus = 0;

// adresa konce za lok�ln� funkc�
		i = prog2->Items;
		index += i;
		prog += i;
		prog2 += i;

// cyklus p�es v�echny polo�ky shora dol�
		for (; i > 0; i--)
		{

// adresa dal�� (p�ede�l�) polo�ky
			index--;
			prog--;
			prog2--;

// index lok�ln�ho seznamu
			if (prog->Func == IDF_LIST - IDF)
			{
				loclst += 1;
				prog2->LocalList = loclst;
			}

// test, zda to je prom�nn�
			if (prog2->Data >= 0)
			{

// p��prava po�tu prvk� prom�nn�
				int n = 1;
				if (prog2->List >= 0)
				{
					n = List[prog2->List].Size;
				}

// nastaven� indexu lok�ln� prom�nn�
				switch (prog->Func)
				{
				case IDF_REAL - IDF:
					locnum += n;
					prog2->Local = locnum;
					break;

				case IDF_TEXT - IDF:
					loctxt += n;
					prog2->Local = loctxt;
					break;

				case IDF_BOOL - IDF:
					loclog += n;
					prog2->Local = loclog;
					break;

				case IDF_ICON - IDF:
					locico += n;
					prog2->Local = locico;
					break;

				case IDF_MAP - IDF:
					locmap += n;
					prog2->Local = locmap;
					break;

				case IDF_PIC - IDF:
					locpic += n;
					prog2->Local = locpic;
					break;

				case IDF_SPRITE - IDF:
					locspr += n;
					prog2->Local = locspr;
					break;

				case IDF_SND - IDF:
					locsnd += n;
					prog2->Local = locsnd;
					break;

				case IDF_MUS - IDF:
					locmus += n;
					prog2->Local = locmus;
					break;
				}
			}
		}

// aktualizace indexu lok�ln�ho seznamu (ukazatel z�stane za koncem v�tve)
		i = prog2->Items;
		int j = 0;
		int locallist = -1;
		for (; i > 0; i--)
		{

			if (prog->Func == IDF_LIST - IDF)
			{
				j = prog2->Items;
				locallist = prog2->LocalList;
			}

			if (j > 0)
			{
				prog2->LocalList = locallist;
				j--;
			}

			index++;
			prog++;
			prog2++;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// zmapov�n� sprajt�

void LoadSprite(PETPROG* prog, PETPROG2* prog2, int num)
{
// lok�ln� prom�nn�
	int i = -1;				// ��ta� prvk� sprajtu
	int sprite = -1;		// index sprajtu
	int local = -1;			// lok�ln� index sprajtu
	int list = -1;			// seznam
	int locallist = -1;		// lok�ln� seznam

// cyklus p�es v�echny polo�ky
	for (; num > 0; num--)
	{

// test, zda je sprajt
		if (prog->Func == IDF_SPRITE - IDF)			// je to sprajt?
		{
			sprite = prog2->Data;
			local = prog2->Local;
			list = prog2->List;
			locallist = prog2->LocalList;
			i = prog2->Items - 1;
		}

// nastaven� hodnoty sprajtu
		if (i >= 0)
		{
			prog2->Data = sprite;
			prog2->List = list;
			prog2->Local = local;
			prog2->LocalList = locallist;
			i--;
		}

// p��prava pro dal�� polo�ku
		prog++;
		prog2++;
	}
}

/////////////////////////////////////////////////////////////////////////////
// na�ten� jednoho textu z programu

CString LoadText0()
{
// nalezen� adresy textu
	int lan;
	char* data = JazykUkaz[Jazyk];
	if ((data == NULL) || (*(long*)data == 0))
	{
		data = JazykUkaz[JazykDefImp];
		if ((data == NULL) || (*(long*)data == 0))
		{
			data = JazykUkaz[JAZYKENG];
			if ((data == NULL) || (*(long*)data == 0))
			{
				for (lan = 0; lan < JAZYKNUM; lan++)
				{
					data = JazykUkaz[lan];
					if ((data != NULL) && (*(long*)data != 0)) break;
				}
			}
		}
	}

// dek�dov�n� textu
	CString txt;

	if (data != NULL)
	{
		int n = *(long*)data;					// d�lka textu
		data = data + 4;						// p�esko�en� d�lky textu

		txt.Term();

		if (Release)
		{
			char* data2 = (char*)MemGet(n + 16);
			for (int k = 0; k < n; k++)
			{
				data2[k] = (char)(123 - data[k]);
			}
			txt.Init(data2, n);
			MemFree(data2);
		}
		else
		{
			txt.Init(data, n);				// p�id�n� textu do bufferu
		}
	}

// posun ukazatel� textu
	for (lan = 0; lan < JAZYKNUM; lan++)
	{
		data = JazykUkaz[lan];

		if (data != NULL)
		{
			data = data + *(long*)data;
			data = data + 4;
			JazykUkaz[lan] = data;
		}
	}

	txt.Orig(JazykUkaz0);
	JazykUkaz0++;

	return txt;
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� programu

void Load()
{
// lok�ln� prom�nn�
	int i, j, n;

// na�ten� ��sel
	{
		double* data = (double*)BufAdr(BufNumID);
		for (i = PetAdr->piReal.Pocet; i > 0; i--)
		{
			Real.Add(*data);
			data++;
		}
		ASSERT(Real.Num() == PetAdr->piReal.Pocet);
		ASSERT((BYTE*)data - BufAdr(BufNumID) == PetAdr->piReal.Delka);
	}

// na�ten� text�
	{
	// nalezen� za��tk� jazyk�
		int lan;
		char* data = (char*)BufAdr(BufTxtID);
		for (int l = PetAdr->piText.Extra2; l > 0; l--)
		{
			for (lan = 0; lan < JAZYKNUM; lan++)
			{
				if (JazykInfo[lan].LangID == ((TEXTPROG*)data)->LangID)
				{
					JazykInfo[lan].TextTab = data + SIZEOFTEXTPROG;
					break;
				}
			}

			data += SIZEOFTEXTPROG;

			for (i = PetAdr->piText.Pocet; i > 0; i--)
			{
				data += *(long*)data;
				data = data + 4;
			}
		}
		ASSERT((BYTE*)data - BufAdr(BufTxtID) == PetAdr->piText.Delka);

	// p��prava ukazatel� jazyk�
		for (lan = 0; lan < JAZYKNUM; lan++)
		{
			JazykUkaz[lan] = JazykInfo[lan].TextTab;
		}
		JazykUkaz0 = 0;

	// na�ten� text�
		for (i = PetAdr->piText.Pocet; i > 0; i--)
		{
			Text.Add(LoadText0());
		}
		ASSERT(Text.Num() == PetAdr->piText.Pocet);
	}

// na�ten� logick�ch hodnot
	{
		char* data = (char*)BufAdr(BufLogID);
		for (i = PetAdr->piBool.Pocet; i > 0; i--)
		{
#pragma warning ( disable: 4800)		// hl�en� - konverze char na bool
			Bool.Add((bool)*data);
#pragma warning ( default: 4800)		// hl�en� - konverze char na bool
			data++;
		}
		ASSERT(Bool.Num() == PetAdr->piBool.Pocet);
		ASSERT((BYTE*)data - BufAdr(BufLogID) == PetAdr->piBool.Delka);
	}

// na�ten� ikon
	{
		BYTE* data = (BYTE*)BufAdr(BufIcoID);

		for (i = PetAdr->piIcon.Pocet; i > 0; i--)
		{
			ICONDATA* ico = (ICONDATA*)MemGet(SIZEOFICONDATA);
			ico->Refer = BigInt;
			ico->Param = PicParamComp;
			ico->HIcon = NULL;
			ico->Cursor = NULL;
			ico->Data = data;
			Icon.Add(ico);
			data += *(long*)data + 4;
		}
		ASSERT(Icon.Num() == PetAdr->piIcon.Pocet);
		ASSERT((BYTE*)data - BufAdr(BufIcoID) == PetAdr->piIcon.Delka);
		ASSERT(Icon.Num() > 0);
		if (Icon.Num() == 0) Icon.New();
	}

// na�ten� ploch
	{
		MAPPROG* data = (MAPPROG*)BufAdr(BufMapID);

		for (i = PetAdr->piMap.Pocet; i > 0; i--)
		{
			int width = data->Width;
			int height = data->Height;
			CMap map(width, height);
			n = width*height;
			long* src = (long*)data->Data;
			for (j = 0; j < n; j++)
			{
				map[j].Icon = Icon.Get(*src);
				src++;
				map[j].Param = *src;
				src++;
			}
			Map.Add(map);
			data = (MAPPROG*)src;
		}
		ASSERT(Map.Num() == PetAdr->piMap.Pocet);
		ASSERT((BYTE*)data - BufAdr(BufMapID) == PetAdr->piMap.Delka);
		ASSERT(Map.Num() > 0);
		if (Map.Num() == 0) Map.New(15, 10);
	}

// na�ten� obr�zk�
	{
		PICPROG* data = (PICPROG*)BufAdr(BufPicID);

		for (i = PetAdr->piPic.Pocet; i > 0; i--)
		{
			PICTUREDATA* pic = (PICTUREDATA*)MemGet(SIZEOFPICTUREDATA);
			pic->Refer = BigInt;
			pic->Param = PicParamComp;
			int width = data->Width;
			ASSERT(width > 0);
			pic->Width = width;
			int height = data->Height;
			ASSERT(height > 0);
			pic->Height = height;
			pic->Data = data->Data;

#ifndef _MINI
			pic->TextInx = -1;
			pic->TextData = NULL;
			pic->TextDataRGBA = NULL;
			pic->TextDataR8G8B8 = NULL;
			pic->TextDataR5G6B5 = NULL;
			pic->TextDataA1R5G5B5 = NULL;
			pic->TextDataA4R4G4B4 = NULL;
#endif // _MINI

			Picture.Add(pic);
			int size = *(long*)data->Data + 4;
			data = (PICPROG*)((BYTE*)data + SIZEOFPICPROG + size);
		}
		ASSERT(Picture.Num() == PetAdr->piPic.Pocet);
		ASSERT((BYTE*)data - BufAdr(BufPicID) == PetAdr->piPic.Delka);
	}

// na�ten� sprajt�
	{
		SPRITEPROG* data = (SPRITEPROG*)BufAdr(BufSprID);

		for (i = PetAdr->piSprite.Pocet; i > 0; i--)
		{
			int faze = data->Faze;
			int smer = data->Smer;
			int width = data->Width;
			int height = data->Height;
			CSprite spr(faze, smer);
			spr.Klid(data->Klid);
			if (spr.Klid() > faze) spr.Klid(faze);
			spr.Delay(data->Delay);
			spr.Level(data->Level);
			spr.Kroku(data->Kroku);

			n = faze*smer;
			BYTE* src = data->Data;
			for (j = 0; j < n; j++)
			{
				PICTUREDATA* pic = (PICTUREDATA*)MemGet(SIZEOFPICTUREDATA);
				pic->Refer = BigInt;
				pic->Param = PicParamComp;
				pic->Width = width;
				pic->Height = height;
				pic->Data = src;

#ifndef _MINI
				pic->TextInx = -1;
				pic->TextData = NULL;
				pic->TextDataRGBA = NULL;
				pic->TextDataR8G8B8 = NULL;
				pic->TextDataR5G6B5 = NULL;
				pic->TextDataA1R5G5B5 = NULL;
				pic->TextDataA4R4G4B4 = NULL;
#endif // _MINI

				spr[j] = pic;
				src += *(long*)src + 4;
			}
			Sprite.Add(spr);
			data = (SPRITEPROG*)src;
		}
		ASSERT(Sprite.Num() == PetAdr->piSprite.Pocet);
		ASSERT((BYTE*)data - BufAdr(BufSprID) == PetAdr->piSprite.Delka);
		ASSERT(Sprite.Num() > 0);
		if (Sprite.Num() == 0) Sprite.New();
		if (Sprite.Num() == 1) Sprite.New();
	}

// na�ten� zvuk�
	{
		SOUNDPROG* data = (SOUNDPROG*)BufAdr(BufSndID);

		for (i = PetAdr->piSound.Pocet; i > 0; i--)
		{
			SOUNDDATA* snd = (SOUNDDATA*)MemGet(SIZEOFSOUNDDATA);
			snd->Refer = BigInt;
			int size = data->Size;
			snd->Size = size;
			ASSERT(size >= 0);
			snd->Samples = data->Samples;
			snd->Format = data->Format;
			snd->Channels = data->Channels;
			snd->Bits = data->Bits;
			snd->Data = data->Data;
			if (snd->Format == WAVE_FORMAT_PCM)
			{
				snd->SizeHead = 0;
			}
			else
			{
				snd->SizeHead = ((WAVEFORMATEX*)(snd->Data))->cbSize + SIZEOFWAVEFORMATEX;
			}

			Sound.Add(snd);
			data = (SOUNDPROG*)((BYTE*)data + SIZEOFSOUNDPROG + size);
		}
		ASSERT(Sound.Num() == PetAdr->piSound.Pocet);
		ASSERT((BYTE*)data - BufAdr(BufSndID) == PetAdr->piSound.Delka);
	}

// na�ten� hudby
	{
		BYTE* data = (BYTE*)BufAdr(BufMusID);

		for (i = PetAdr->piMusic.Pocet; i > 0; i--)
		{
			MUSICDATA* mus = (MUSICDATA*)MemGet(SIZEOFMUSICDATA);
			mus->Refer = BigInt;
			int size = *(long*)data;
			data += sizeof(long);
			mus->Size = size;
			ASSERT(size >= 0);
			mus->Data = data;
			Music.Add(mus);
			data += size;
		}
		ASSERT(Music.Num() == PetAdr->piMusic.Pocet);
		ASSERT((BYTE*)data - BufAdr(BufMusID) == PetAdr->piMusic.Delka);
	}

// p��prava editoru glob�ln�ch objekt�
	BufObjN = PetAdr->piGlobal.Pocet;
	BufObj2 = (PETPROG2*) MemGet(BufObjN*sizeof(PETPROG2));
	BufObj = (PETPROG*)	BufAdr(BufObjID);
	LoadProg(BufObj, BufObj2, BufObjN);
	TextListBeg = Text.Num();
	LoadList(BufObj, BufObj2, BufObjN, true);
	TextListEnd = Text.Num();
	LoadSprite(BufObj, BufObj2, BufObjN);

// p��prava editoru lok�ln�ch objekt�
	BufLocN = PetAdr->piLocal.Pocet;
	BufLoc2 = (PETPROG2*) MemGet(BufLocN*sizeof(PETPROG2));
	BufLoc = (PETPROG*)	BufAdr(BufLocID);
	LoadProg(BufLoc, BufLoc2, BufLocN);
	LoadList(BufLoc, BufLoc2, BufLocN, false);
	LoadLocal();
	LoadSprite(BufLoc, BufLoc2, BufLocN);

// p��prava editoru programu
	BufEdiN = PetAdr->piProgram.Pocet;
	BufEdi2 = (PETPROG2*) MemGet(BufEdiN*sizeof(PETPROG2));
	BufEdi = (PETPROG*)	BufAdr(BufEdiID);
	LoadProg(BufEdi, BufEdi2, BufEdiN);
}
