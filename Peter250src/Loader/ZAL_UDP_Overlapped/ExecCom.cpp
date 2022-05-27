
#include "Main.h"

/***************************************************************************\
*																			*
*						Prov�d�n� programu - p��kaz							*
*																			*
\***************************************************************************/

#pragma optimize("t", on)			// optimalizace na maxim�ln� rychlost


// oblasti kurzoru my�i
MOUSEREG*	MouseReg = NULL;		// regiony kurzoru my�i
int			MouseRegNum = 0;		// po�et region� kurzoru my�i
int			MouseRegMax = 0;		// velikost bufferu region� kurzoru my�i


/***************************************************************************\
*																			*
*								v�tven� CASE								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// CASE s ��slem

void FCaseReal()
{
// po�et testovan�ch polo�ek
	int num = ExecItem[-1].Data;

// na�ten� testovan�ho v�razu
	double vyraz = FNum();

// uschovan� za��tek obsluhy
	EXECITEM* item;

// cyklus p�es v�echny testovan� v�tve
	for (; num > 0; num--)
	{
		item = ExecItem;

// test jedn� v�tve
		if (FNum() == vyraz)
		{

// podm�nka spln�na - provede se obsluha
			ExecItem = item + item->Jump;
			break;
		}
	}

// proveden� obsluhy
	item = ExecItem;
	FCommand();
	ExecItem = item + item->Jump;
}


/////////////////////////////////////////////////////////////////////////////
// CASE s logick�m v�razem

void FCaseBool()
{
// po�et testovan�ch polo�ek
	int num = ExecItem[-1].Data;

// na�ten� testovan�ho v�razu
	bool vyraz = FBool();

// uschovan� za��tek obsluhy
	EXECITEM* item;

// cyklus p�es v�echny testovan� v�tve
	for (; num > 0; num--)
	{
		item = ExecItem;

// test jedn� v�tve
		if (FBool() == vyraz)
		{

// podm�nka spln�na - provede se obsluha
			ExecItem = item + item->Jump;
			break;
		}
	}

// proveden� obsluhy
	item = ExecItem;
	FCommand();
	ExecItem = item + item->Jump;
}


/////////////////////////////////////////////////////////////////////////////
// CASE s ikonou

void FCaseIcon()
{
// po�et testovan�ch polo�ek
	int num = ExecItem[-1].Data;

// na�ten� testovan�ho v�razu
	CIcon vyraz;
	CIcon vyraz2;
	FIcon(vyraz);

// uschovan� za��tek obsluhy
	EXECITEM* item;

// cyklus p�es v�echny testovan� v�tve
	for (; num > 0; num--)
	{
		item = ExecItem;

// test jedn� v�tve
		FIcon(vyraz2);
		if (vyraz2 == vyraz)
		{

// podm�nka spln�na - provede se obsluha
			ExecItem = item + item->Jump;
			break;
		}
	}

// proveden� obsluhy
	item = ExecItem;
	FCommand();
	ExecItem = item + item->Jump;
}


/////////////////////////////////////////////////////////////////////////////
// CASE se spritem

void FCaseSprite()
{
// po�et testovan�ch polo�ek
	int num = ExecItem[-1].Data;

// na�ten� testovan�ho v�razu
	CSprite vyraz;
	CSprite vyraz2;
	FSprite(vyraz);

// uschovan� za��tek obsluhy
	EXECITEM* item;

// cyklus p�es v�echny testovan� v�tve
	for (; num > 0; num--)
	{
		item = ExecItem;

// test jedn� v�tve
		FSprite(vyraz2);
		if (vyraz2 == vyraz)
		{

// podm�nka spln�na - provede se obsluha
			ExecItem = item + item->Jump;
			break;
		}
	}

// proveden� obsluhy
	item = ExecItem;
	FCommand();
	ExecItem = item + item->Jump;
}


/////////////////////////////////////////////////////////////////////////////
// CASE s plochou

void FCaseMap()
{
// po�et testovan�ch polo�ek
	int num = ExecItem[-1].Data;

// na�ten� testovan�ho v�razu
	CMap vyraz;
	CMap vyraz2;
	FMap(vyraz);

// uschovan� za��tek obsluhy
	EXECITEM* item;

// cyklus p�es v�echny testovan� v�tve
	for (; num > 0; num--)
	{
		item = ExecItem;

// test jedn� v�tve
		FMap(vyraz2);
		if (vyraz2 == vyraz)
		{

// podm�nka spln�na - provede se obsluha
			ExecItem = item + item->Jump;
			break;
		}
	}

// proveden� obsluhy
	item = ExecItem;
	FCommand();
	ExecItem = item + item->Jump;
}


/////////////////////////////////////////////////////////////////////////////
// CASE s hudbou

void FCaseMusic()
{
// po�et testovan�ch polo�ek
	int num = ExecItem[-1].Data;

// na�ten� testovan�ho v�razu
	CMusic vyraz;
	CMusic vyraz2;
	FMusic(vyraz);

// uschovan� za��tek obsluhy
	EXECITEM* item;

// cyklus p�es v�echny testovan� v�tve
	for (; num > 0; num--)
	{
		item = ExecItem;

// test jedn� v�tve
		FMusic(vyraz2);
		if (vyraz2 == vyraz)
		{

// podm�nka spln�na - provede se obsluha
			ExecItem = item + item->Jump;
			break;
		}
	}

// proveden� obsluhy
	item = ExecItem;
	FCommand();
	ExecItem = item + item->Jump;
}


/////////////////////////////////////////////////////////////////////////////
// CASE s textem

void FCaseText()
{
// po�et testovan�ch polo�ek
	int num = ExecItem[-1].Data;

// na�ten� testovan�ho v�razu
	CString vyraz;
	CString vyraz2;
	FText(vyraz);

// uschovan� za��tek obsluhy
	EXECITEM* item;

// cyklus p�es v�echny testovan� v�tve
	for (; num > 0; num--)
	{
		item = ExecItem;

// test jedn� v�tve
		FText(vyraz2);
		if (vyraz2 == vyraz)
		{

// podm�nka spln�na - provede se obsluha
			ExecItem = item + item->Jump;
			break;
		}
	}

// proveden� obsluhy
	item = ExecItem;
	FCommand();
	ExecItem = item + item->Jump;
}


/////////////////////////////////////////////////////////////////////////////
// CASE se zvukem

void FCaseSound()
{
// po�et testovan�ch polo�ek
	int num = ExecItem[-1].Data;

// na�ten� testovan�ho v�razu
	CSound vyraz;
	CSound vyraz2;
	FSound(vyraz);

// uschovan� za��tek obsluhy
	EXECITEM* item;

// cyklus p�es v�echny testovan� v�tve
	for (; num > 0; num--)
	{
		item = ExecItem;

// test jedn� v�tve
		FSound(vyraz2);
		if (vyraz2 == vyraz)
		{

// podm�nka spln�na - provede se obsluha
			ExecItem = item + item->Jump;
			break;
		}
	}

// proveden� obsluhy
	item = ExecItem;
	FCommand();
	ExecItem = item + item->Jump;
}


/////////////////////////////////////////////////////////////////////////////
// CASE s obr�zkem

void FCasePicture()
{
// po�et testovan�ch polo�ek
	int num = ExecItem[-1].Data;

// na�ten� testovan�ho v�razu
	CPicture vyraz;
	CPicture vyraz2;
	FPicture(vyraz);

// uschovan� za��tek obsluhy
	EXECITEM* item;

// cyklus p�es v�echny testovan� v�tve
	for (; num > 0; num--)
	{
		item = ExecItem;

// test jedn� v�tve
		FPicture(vyraz2);
		if (vyraz2 == vyraz)
		{

// podm�nka spln�na - provede se obsluha
			ExecItem = item + item->Jump;
			break;
		}
	}

// proveden� obsluhy
	item = ExecItem;
	FCommand();
	ExecItem = item + item->Jump;
}


/***************************************************************************\
*																			*
*									funkce									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// funkce s lok�ln�mi prom�nn�mi

void FFunc()
{
// �schova indexu volan� funkce
	int data = ExecItem[-1].Data;

// inicializace lok�ln�ch prom�nn�ch
	FCommand();

// �schova ukazatele programu
	EXECITEM* oldexe = ExecItem;

// nov� adresa programu
	ExecItem = ProgBuf + data;

// kontrola hloubky vno�en�
	Hloubka--;
	if (Hloubka >= 0)
	{

// vyvol�n� funkce
		FCommand();
	}
	Hloubka++;

// n�vrat adresy programu
	ExecItem = oldexe;

// zru�en� po�adavku o p�eru�en�
	Break &= ~(BREAKFUNC | BREAKWHILE);

// zru�en� lok�ln�ch prom�nn�ch
	FCommand();
}


/////////////////////////////////////////////////////////////////////////////
// funkce bez lok�ln�ch prom�nn�ch

void FFunc0()
{
// �schova ukazatele programu
	EXECITEM* oldexe = ExecItem;

// nov� adresa programu
	ExecItem = ProgBuf + oldexe[-1].Data;

// kontrola hloubky vno�en�
	Hloubka--;
	if (Hloubka >= 0)
	{

// vyvol�n� funkce
		FCommand();
	}
	Hloubka++;

// n�vrat adresy programu
	ExecItem = oldexe;

// zru�en� po�adavku o p�eru�en�
	Break &= ~(BREAKFUNC | BREAKWHILE);
}


// --------------------------
/////////////////////////////////////////////////////////////////////////////
// vytvo�en� lok�ln�ho seznamu duplikac� (Data = index vzoru)

void FDupList() { List.Dup(ExecItem[-1].Data); }


/////////////////////////////////////////////////////////////////////////////
// zru�en� lok�ln�ho seznamu (Data = po�et)

void FDelList() { List.Del(ExecItem[-1].Data); };


// --------------------------
/////////////////////////////////////////////////////////////////////////////
// vytvo�en� lok�ln� ��seln� prom�nn� duplikac� (Data = index vzoru)

void FDupReal() { Real.Dup(ExecItem[-1].Data); }


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� lok�ln� ��seln� prom�nn� seznamu (Data = index vzoru, List = po�et)

void FDupRealList() { Real.Dup(ExecItem[-1].Data, ExecItem[-1].List); }


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� lok�ln� ��seln� prom�nn� s inicializac�

void FNewReal() { Real.Add(FNum()); }


/////////////////////////////////////////////////////////////////////////////
// zru�en� lok�ln�ch ��seln�ch prom�nn�ch (Data = po�et)

void FDelReal() { Real.Del(ExecItem[-1].Data); };


// --------------------------
/////////////////////////////////////////////////////////////////////////////
// vytvo�en� lok�ln� logick� prom�nn� duplikac� (Data = index vzoru)

void FDupBool() { Bool.Dup(ExecItem[-1].Data); }


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� lok�ln� logick� prom�nn� seznamu (Data = index vzoru, List = po�et)

void FDupBoolList() { Bool.Dup(ExecItem[-1].Data, ExecItem[-1].List); }


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� lok�ln� logick� prom�nn� s inicializac�

void FNewBool() { Bool.Add(FBool()); }


/////////////////////////////////////////////////////////////////////////////
// zru�en� lok�ln�ch logick�ch prom�nn�ch (Data = po�et)

void FDelBool() { Bool.Del(ExecItem[-1].Data); };


// --------------------------
/////////////////////////////////////////////////////////////////////////////
// vytvo�en� lok�ln� prom�nn� ikony duplikac� (Data = index vzoru)

void FDupIcon() { Icon.Dup(ExecItem[-1].Data); }


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� lok�ln� prom�nn� ikony seznamu (Data = index vzoru, List = po�et)

void FDupIconList() { Icon.Dup(ExecItem[-1].Data, ExecItem[-1].List); }


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� lok�ln� prom�nn� ikony s inicializac�

void FNewIcon() 
{ 
	CIcon icon;
	FIcon(icon);
	Icon.Add(icon);
}


/////////////////////////////////////////////////////////////////////////////
// zru�en� lok�ln�ch prom�nn�ch ikon (Data = po�et)

void FDelIcon() { Icon.Del(ExecItem[-1].Data); };


// --------------------------
/////////////////////////////////////////////////////////////////////////////
// vytvo�en� lok�ln� prom�nn� sprajtu duplikac� (Data = index vzoru)

void FDupSprite() { Sprite.Dup(ExecItem[-1].Data); }


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� lok�ln� prom�nn� sprajtu seznamu (Data = index vzoru, List = po�et)

void FDupSpriteList() { Sprite.Dup(ExecItem[-1].Data, ExecItem[-1].List); }


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� lok�ln� prom�nn� sprajtu s inicializac�

void FNewSprite() 
{ 
	CSprite sprite;
	FSprite(sprite);
	Sprite.Add(sprite);
}


/////////////////////////////////////////////////////////////////////////////
// zru�en� lok�ln�ch prom�nn�ch sprajt� (Data = po�et)

void FDelSprite() { Sprite.Del(ExecItem[-1].Data); };


// --------------------------
/////////////////////////////////////////////////////////////////////////////
// vytvo�en� lok�ln� prom�nn� plochy duplikac� (Data = index vzoru)

void FDupMap() { Map.Dup(ExecItem[-1].Data); }


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� lok�ln� prom�nn� plochy seznamu (Data = index vzoru, List = po�et)

void FDupMapList() { Map.Dup(ExecItem[-1].Data, ExecItem[-1].List); }


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� lok�ln� prom�nn� plochy s inicializac�

void FNewMap() 
{ 
	CMap map;
	FMap(map);
	Map.Add(map);
}


/////////////////////////////////////////////////////////////////////////////
// zru�en� lok�ln�ch prom�nn�ch ploch (Data = po�et)

void FDelMap() { Map.Del(ExecItem[-1].Data); };


// --------------------------
/////////////////////////////////////////////////////////////////////////////
// vytvo�en� lok�ln� prom�nn� textu duplikac� (Data = index vzoru)

void FDupText() { Text.Dup(ExecItem[-1].Data); }


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� lok�ln� prom�nn� textu seznamu (Data = index vzoru, List = po�et)

void FDupTextList() { Text.Dup(ExecItem[-1].Data, ExecItem[-1].List); }


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� lok�ln� prom�nn� textu s inicializac�

void FNewText() 
{ 
	CString text;
	FText(text);
	Text.Add(text);
}


/////////////////////////////////////////////////////////////////////////////
// zru�en� lok�ln�ch prom�nn�ch text� (Data = po�et)

void FDelText() { Text.Del(ExecItem[-1].Data); };


// --------------------------
/////////////////////////////////////////////////////////////////////////////
// vytvo�en� lok�ln� prom�nn� zvuku duplikac� (Data = index vzoru)

void FDupSound() { Sound.Dup(ExecItem[-1].Data); }


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� lok�ln� prom�nn� zvuku seznamu (Data = index vzoru, List = po�et)

void FDupSoundList() { Sound.Dup(ExecItem[-1].Data, ExecItem[-1].List); }


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� lok�ln� prom�nn� zvuku s inicializac�

void FNewSound() 
{ 
	CSound sound;
	FSound(sound);
	Sound.Add(sound);
}


/////////////////////////////////////////////////////////////////////////////
// zru�en� lok�ln�ch prom�nn�ch zvuk� (Data = po�et)

void FDelSound() { Sound.Del(ExecItem[-1].Data); };


// --------------------------
/////////////////////////////////////////////////////////////////////////////
// vytvo�en� lok�ln� prom�nn� obr�zku duplikac� (Data = index vzoru)

void FDupPicture() { Picture.Dup(ExecItem[-1].Data); }


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� lok�ln� prom�nn� obr�zku seznamu (Data = index vzoru, List = po�et)

void FDupPictureList() { Picture.Dup(ExecItem[-1].Data, ExecItem[-1].List); }


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� lok�ln� prom�nn� obr�zku s inicializac�

void FNewPicture() 
{ 
	CPicture pic;
	FPicture(pic);
	Picture.Add(pic);
}


/////////////////////////////////////////////////////////////////////////////
// zru�en� lok�ln�ch prom�nn�ch obr�zk� (Data = po�et)

void FDelPicture() { Picture.Del(ExecItem[-1].Data); };

 
// --------------------------
/////////////////////////////////////////////////////////////////////////////
// vytvo�en� lok�ln� prom�nn� hudby duplikac� (Data = index vzoru)

void FDupMusic() { Music.Dup(ExecItem[-1].Data); }


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� lok�ln� prom�nn� hudby seznamu (Data = index vzoru, List = po�et)

void FDupMusicList() { Music.Dup(ExecItem[-1].Data, ExecItem[-1].List); }


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� lok�ln� prom�nn� hudby s inicializac�

void FNewMusic() 
{ 
	CMusic music;
	FMusic(music);
	Music.Add(music);
}


/////////////////////////////////////////////////////////////////////////////
// zru�en� lok�ln�ch prom�nn�ch hudby (Data = po�et)

void FDelMusic() { Music.Del(ExecItem[-1].Data); };


/***************************************************************************\
*																			*
*							nastaven� prom�nn�ch							*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// nastaven� indexu glob�ln�ho seznamu

void FSetList()
{
// index seznamu
	int list = ExecItem[-1].List;

// na�ten� po�adovan�ho indexu
	int inx = FIntR();

// velikost seznamu
	int size = List[list].Size;

// omezen� indexu
	if ((DWORD)inx >= (DWORD)size)
	{
		while (inx < 0) inx += size;
		while (inx >= size) inx -= size;
	}

// nastaven� nov� hodnoty indexu
	List[list].Index = inx;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� indexu lok�ln�ho seznamu

void FSetListLoc()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// na�ten� po�adovan�ho indexu
	int inx = FIntR();

// velikost seznamu
	int size = List[list].Size;

// omezen� indexu
	if ((DWORD)inx >= (DWORD)size)
	{
		while (inx < 0) inx += size;
		while (inx >= size) inx -= size;
	}

// nastaven� nov� hodnoty indexu
	List[list].Index = inx;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� inkrementace glob�ln�ho seznamu

void FSetListAuto()
{
// index seznamu
	int list = ExecItem[-1].List;

// na�ten� po�adovan� inkrementace
	int inx = FIntR();

// velikost seznamu
	int size = List[list].Size;

// omezen� inkrementace
	if ((DWORD)inx >= (DWORD)size)
	{
		while (inx <= -size) inx += size;
		while (inx >= size) inx -= size;
	}

// nastaven� nov� hodnoty inkrementace
	List[list].Auto = inx;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� inkrementace lok�ln�ho seznamu

void FSetListAutoLoc()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// na�ten� po�adovan� inkrementace
	int inx = FIntR();

// velikost seznamu
	int size = List[list].Size;

// omezen� inkrementace
	if ((DWORD)inx >= (DWORD)size)
	{
		while (inx <= -size) inx += size;
		while (inx >= size) inx -= size;
	}

// nastaven� nov� hodnoty inkrementace
	List[list].Auto = inx;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� glob�ln� ikony (Data = index)

void FSetIcon()
{
// index ikony
	int inx = ExecItem[-1].Data;

// nastaven� ikony
	CIcon icon;
	FIcon(icon);
	Icon[inx] = icon;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� ikony v seznamu (Data = index, List = seznam)

void FSetIconList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index ikony
	int inx = ExecItem[-1].Data;
	inx += List.Inx(list);

// nastaven� ikony
	CIcon icon;
	FIcon(icon);
	Icon[inx] = icon;

// inkrementace indexu seznamu
	List.AutoInc(list);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� lok�ln� ikony (Data = index)

void FSetIconLoc()
{
// index ikony
	int inx = Icon.Num() - ExecItem[-1].Data;

// nastaven� ikony
	CIcon icon;
	FIcon(icon);
	Icon[inx] = icon;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� lok�ln� ikony v seznamu (Data = index, List = seznam)

void FSetIconLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index ikony
	int inx = Icon.Num() - ExecItem[-1].Data;
	inx += List.Inx(list);

// nastaven� ikony
	CIcon icon;
	FIcon(icon);
	Icon[inx] = icon;

// inkrementace indexu seznamu
	List.AutoInc(list);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� glob�ln� logick� prom�nn� (Data = index)

void FSetLog()
{
// index ��sla
	int inx = ExecItem[-1].Data;

// nastaven� logick� prom�nn�
	Bool[inx] = FBool();
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� logick� prom�nn� v seznamu (Data = index, List = seznam)

void FSetLogList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index ��sla
	int inx = ExecItem[-1].Data;
	inx += List.Inx(list);

// nastaven� logick� prom�nn�
	Bool[inx] = FBool();

// inkrementace indexu seznamu
	List.AutoInc(list);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� lok�ln� logick� prom�nn� (Data = index)

void FSetLogLoc()
{
// index ��sla
	int inx = Bool.Num() - ExecItem[-1].Data;

// nastaven� logick� prom�nn�
	Bool[inx] = FBool();
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� lok�ln� logick� prom�nn� v seznamu (Data = index, List = seznam)

void FSetLogLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index ��sla
	int inx = Bool.Num() - ExecItem[-1].Data;
	inx += List.Inx(list);

// nastaven� logick� prom�nn�
	Bool[inx] = FBool();

// inkrementace indexu seznamu
	List.AutoInc(list);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� glob�ln� plochy (Data = index)

void FSetMap()
{
// index plochy
	int inx = ExecItem[-1].Data;

// na�ten� mapy k nastaven�
	CMap map;
	FMap(map);

// nastaven� plochy != 0
	if (inx != 0)
	{
		Map[inx] = map;
	}
	else

// nastaven� hlavn� plochy 0
	{

// �schova rozm�r� plochy
		int oldwidth = Width;
		int oldheight = Height;

// nastaven� plochy
		Map[0] = map;

// p��prava plochy pro z�pis
		Map[0].CopyWrite();

// inicializace informac� o plo�e
		ExecInitMap();

// test, zda je pot�eba zm�nit velikost okna
		if ((oldwidth != Width) || (oldheight != Height))
		{

// zm�na v re�imu DirectDraw
			if (FullScreen)
			{
				ReSetDisplayMode();
			}
			else
			{

// na�ten� parametr� okna
				WINDOWPLACEMENT wp;
				wp.length = sizeof(WINDOWPLACEMENT);
				::GetWindowPlacement(MainFrame, &wp);

// sou�asn� st�ed okna
				MainFrameX = (wp.rcNormalPosition.left + wp.rcNormalPosition.right)/2;
				MainFrameY = (wp.rcNormalPosition.top + wp.rcNormalPosition.bottom)/2;

// nastaven� nov�ch parametr� okna
				wp.length = sizeof(WINDOWPLACEMENT);
				AdjustMainFrame(&wp.rcNormalPosition);
				::SetWindowPlacement(MainFrame, &wp);

// pro norm�ln� zobrazen� okna aktualizace rozm�r� a pozice
				switch (wp.flags)
				{
				case SW_HIDE:
				case SW_MINIMIZE:
				case SW_SHOWMAXIMIZED:
				case SW_SHOWMINIMIZED:
				case SW_SHOWMINNOACTIVE:
					break;

				default:
					::SetWindowPos(MainFrame, NULL,
							wp.rcNormalPosition.left, 
							wp.rcNormalPosition.top,
							wp.rcNormalPosition.right - wp.rcNormalPosition.left,
							wp.rcNormalPosition.bottom - wp.rcNormalPosition.top, 
							SWP_NOZORDER | SWP_NOCOPYBITS);
				}
			}
		}

// p�ekreslen� okna
		AktLeft = 0;				// lev� okraj k p�ekreslen�
		AktTop = 0;					// horn� okraj k p�ekreslen�
		AktRight = Width;			// prav� okraj k p�ekreslen�
		AktBottom = Height;			// spodn� okraj k p�ekreslen�
		AktAllIcon();				// aktualizovat v�echny ikony
		OnSize(&Win[0]);			// aktualizace �daj� s p�ekreslen�m
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� plochy v seznamu (Data = index, List = seznam)

void FSetMapList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index plochy
	int inx = ExecItem[-1].Data;
	inx += List.Inx(list);

// nastaven� plochy
	CMap map;
	FMap(map);
	Map[inx] = map;

// inkrementace indexu seznamu
	List.AutoInc(list);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� lok�ln� plochy (Data = index)

void FSetMapLoc()
{
// index plochy
	int inx = Map.Num() - ExecItem[-1].Data;

// nastaven� plochy
	CMap map;
	FMap(map);
	Map[inx] = map;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� lok�ln� plochy v seznamu (Data = index, List = seznam)

void FSetMapLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index plochy
	int inx = Map.Num() - ExecItem[-1].Data;
	inx += List.Inx(list);

// nastaven� plochy
	CMap map;
	FMap(map);
	Map[inx] = map;

// inkrementace indexu seznamu
	List.AutoInc(list);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� glob�ln�ho ��sla (Data = index)

void FSetNum()
{
// index ��sla
	int inx = ExecItem[-1].Data;

// nastaven� ��sla
	Real[inx] = FNum();
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� ��sla v seznamu (Data = index, List = seznam)

void FSetNumList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index ��sla
	int inx = ExecItem[-1].Data;
	inx += List.Inx(list);

// nastaven� ��sla
	Real[inx] = FNum();

// inkrementace indexu seznamu
	List.AutoInc(list);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� lok�ln�ho ��sla (Data = index)

void FSetNumLoc()
{
// index ��sla
	int inx = Real.Num() - ExecItem[-1].Data;

// nastaven� ��sla
	Real[inx] = FNum();
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� lok�ln�ho ��sla v seznamu (Data = index, List = seznam)

void FSetNumLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index ��sla
	int inx = Real.Num() - ExecItem[-1].Data;
	inx += List.Inx(list);

// nastaven� ��sla
	Real[inx] = FNum();

// inkrementace indexu seznamu
	List.AutoInc(list);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� glob�ln�ho obr�zku (Data = index)

void FSetPic()
{
// index obr�zku
	int inx = ExecItem[-1].Data;

// nastaven� obr�zku
	CPicture pic;
	FPicture(pic);
	Picture[inx] = pic;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� obr�zku v seznamu (Data = index, List = seznam)

void FSetPicList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index obr�zku
	int inx = ExecItem[-1].Data;
	inx += List.Inx(list);

// nastaven� obr�zku
	CPicture pic;
	FPicture(pic);
	Picture[inx] = pic;

// inkrementace indexu seznamu
	List.AutoInc(list);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� lok�ln�ho obr�zku (Data = index)

void FSetPicLoc()
{
// index obr�zku
	int inx = Picture.Num() - ExecItem[-1].Data;

// nastaven� obr�zku
	CPicture pic;
	FPicture(pic);
	Picture[inx] = pic;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� lok�ln�ho obr�zku v seznamu (Data = index, List = seznam)

void FSetPicLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index obr�zku
	int inx = Picture.Num() - ExecItem[-1].Data;
	inx += List.Inx(list);

// nastaven� obr�zku
	CPicture pic;
	FPicture(pic);
	Picture[inx] = pic;

// inkrementace indexu seznamu
	List.AutoInc(list);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� glob�ln�ho zvuku (Data = index)

void FSetSound()
{
// index zvuku
	int inx = ExecItem[-1].Data;

// nastaven� zvuku
	CSound sound;
	FSound(sound);
	Sound[inx] = sound;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� zvuku v seznamu (Data = index, List = seznam)

void FSetSoundList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index zvuku
	int inx = ExecItem[-1].Data;
	inx += List.Inx(list);

// nastaven� zvuku
	CSound sound;
	FSound(sound);
	Sound[inx] = sound;

// inkrementace indexu seznamu
	List.AutoInc(list);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� lok�ln�ho zvuku (Data = index)

void FSetSoundLoc()
{
// index zvuku
	int inx = Sound.Num() - ExecItem[-1].Data;

// nastaven� zvuku
	CSound sound;
	FSound(sound);
	Sound[inx] = sound;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� lok�ln�ho zvuku v seznamu (Data = index, List = seznam)

void FSetSoundLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index zvuku
	int inx = Sound.Num() - ExecItem[-1].Data;
	inx += List.Inx(list);

// nastaven� zvuku
	CSound sound;
	FSound(sound);
	Sound[inx] = sound;

// inkrementace indexu seznamu
	List.AutoInc(list);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� glob�ln� hudby (Data = index)

void FSetMusic()
{
// index hudby
	int inx = ExecItem[-1].Data;

// nastaven� hudby
	CMusic music;
	FMusic(music);
	Music[inx] = music;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� hudby v seznamu (Data = index, List = seznam)

void FSetMusicList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index hudby
	int inx = ExecItem[-1].Data;
	inx += List.Inx(list);

// nastaven� hudby
	CMusic music;
	FMusic(music);
	Music[inx] = music;

// inkrementace indexu seznamu
	List.AutoInc(list);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� lok�ln� hudby (Data = index)

void FSetMusicLoc()
{
// index hudby
	int inx = Music.Num() - ExecItem[-1].Data;

// nastaven� hudby
	CMusic music;
	FMusic(music);
	Music[inx] = music;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� lok�ln� hudby v seznamu (Data = index, List = seznam)

void FSetMusicLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index hudby
	int inx = Music.Num() - ExecItem[-1].Data;
	inx += List.Inx(list);

// nastaven� hudby
	CMusic music;
	FMusic(music);
	Music[inx] = music;

// inkrementace indexu seznamu
	List.AutoInc(list);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� glob�ln�ho textu (Data = index)

void FSetText()
{
// index zvuku
	int inx = ExecItem[-1].Data;

// nastaven� textu
	CString text;
	FText(text);
	Text[inx] = text;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� textu v seznamu (Data = index, List = seznam)

void FSetTextList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index textu
	int inx = ExecItem[-1].Data;
	inx += List.Inx(list);

// nastaven� textu
	CString text;
	FText(text);
	Text[inx] = text;

// inkrementace indexu seznamu
	List.AutoInc(list);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� lok�ln�ho textu (Data = index)

void FSetTextLoc()
{
// index textu
	int inx = Text.Num() - ExecItem[-1].Data;

// nastaven� textu
	CString text;
	FText(text);
	Text[inx] = text;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� lok�ln�ho textu v seznamu (Data = index, List = seznam)

void FSetTextLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index textu
	int inx = Text.Num() - ExecItem[-1].Data;
	inx += List.Inx(list);

// nastaven� textu
	CString text;
	FText(text);
	Text[inx] = text;

// inkrementace indexu seznamu
	List.AutoInc(list);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� glob�ln�ho sprajtu (Data = index)

void FSetSprite()
{
// index sprajtu
	int inx = ExecItem[-1].Data;

// nastaven� sprajtu
	CSprite sprite;
	FSprite(sprite);
//	Sprite[inx] = sprite;
	SetSprite(inx, sprite);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� sprajtu v seznamu (Data = index, List = seznam)

void FSetSpriteList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index sprajtu
	int inx = ExecItem[-1].Data;
	inx += List.Inx(list);

// nastaven� sprajtu
	CSprite sprite;
	FSprite(sprite);
	SetSprite(inx, sprite);

// inkrementace indexu seznamu
	List.AutoInc(list);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� lok�ln�ho sprajtu (Data = index)

void FSetSpriteLoc()
{
// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;

// nastaven� sprajtu
	CSprite sprite;
	FSprite(sprite);
//	Sprite[inx] = sprite;
	SetSprite(inx, sprite);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� lok�ln�ho sprajtu v seznamu (Data = index, List = seznam)

void FSetSpriteLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;
	inx += List.Inx(list);

// nastaven� sprajtu
	CSprite sprite;
	FSprite(sprite);
	SetSprite(inx, sprite);

// inkrementace indexu seznamu
	List.AutoInc(list);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� sou�adnice X glob�ln�ho sprajtu (Data = index)

void FSpriteX()
{
// index sprajtu
	int inx = ExecItem[-1].Data;

// po�adovan� hodnota
	double data = FNum();

// nastaven� pozice X sprajtu
	SpriteMove(inx, data, Sprite[inx].CilY());
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� sou�adnice X sprajtu v seznamu (Data = index, List = seznam)

void FSpriteXList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index sprajtu
	int inx = ExecItem[-1].Data;
	inx += List.Inx(list);

// po�adovan� hodnota
	double data = FNum();

// nastaven� pozice X sprajtu
	SpriteMove(inx, data, Sprite[inx].CilY());

// inkrementace indexu seznamu
	List.AutoInc(list);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� sou�adnice X lok�ln�ho sprajtu (Data = index)

void FSpriteXLoc()
{
// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;

// po�adovan� hodnota
	double data = FNum();

// nastaven� pozice X sprajtu
	SpriteMove(inx, data, Sprite[inx].CilY());
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� sou�adnice X lok�ln�ho sprajtu v seznamu (Data = index, List = seznam)

void FSpriteXLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;
	inx += List.Inx(list);

// po�adovan� hodnota
	double data = FNum();

// nastaven� pozice X sprajtu
	SpriteMove(inx, data, Sprite[inx].CilY());

// inkrementace indexu seznamu
	List.AutoInc(list);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� sou�adnice Y glob�ln�ho sprajtu (Data = index)

void FSpriteY()
{
// index sprajtu
	int inx = ExecItem[-1].Data;

// po�adovan� hodnota
	double data = FNum();

// nastaven� pozice Y sprajtu
	SpriteMove(inx, Sprite[inx].CilX(), data);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� sou�adnice Y sprajtu v seznamu (Data = index, List = seznam)

void FSpriteYList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index sprajtu
	int inx = ExecItem[-1].Data;
	inx += List.Inx(list);

// po�adovan� hodnota
	double data = FNum();

// nastaven� pozice Y sprajtu
	SpriteMove(inx, Sprite[inx].CilX(), data);

// inkrementace indexu seznamu
	List.AutoInc(list);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� sou�adnice Y lok�ln�ho sprajtu (Data = index)

void FSpriteYLoc()
{
// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;

// po�adovan� hodnota
	double data = FNum();

// nastaven� pozice Y sprajtu
	SpriteMove(inx, Sprite[inx].CilX(), data);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� sou�adnice Y lok�ln�ho sprajtu v seznamu (Data = index, List = seznam)

void FSpriteYLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;
	inx += List.Inx(list);

// po�adovan� hodnota
	double data = FNum();

// nastaven� pozice Y sprajtu
	SpriteMove(inx, Sprite[inx].CilX(), data);

// inkrementace indexu seznamu
	List.AutoInc(list);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� sm�ru glob�ln�ho sprajtu (Data = index)

void FSpriteSmer()
{
// index sprajtu
	int inx = ExecItem[-1].Data;

// po�adovan� hodnota
	double data = FNum();

// nastaven� sm�ru sprajtu
	SetSpriteSmer(inx, data);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� sm�ru sprajtu v seznamu (Data = index, List = seznam)

void FSpriteSmerList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index sprajtu
	int inx = ExecItem[-1].Data;
	inx += List.Inx(list);

// po�adovan� hodnota
	double data = FNum();

// nastaven� sm�ru sprajtu
	SetSpriteSmer(inx, data);

// inkrementace indexu seznamu
	List.AutoInc(list);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� sm�ru lok�ln�ho sprajtu (Data = index)

void FSpriteSmerLoc()
{
// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;

// po�adovan� hodnota
	double data = FNum();

// nastaven� sm�ru sprajtu
	SetSpriteSmer(inx, data);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� sm�ru lok�ln�ho sprajtu v seznamu (Data = index, List = seznam)

void FSpriteSmerLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;
	inx += List.Inx(list);

// po�adovan� hodnota
	double data = FNum();

// nastaven� sm�ru sprajtu
	SetSpriteSmer(inx, data);

// inkrementace indexu seznamu
	List.AutoInc(list);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� f�ze glob�ln�ho sprajtu (Data = index)

void FSpriteFaze()
{
// index sprajtu
	int inx = ExecItem[-1].Data;

// po�adovan� hodnota
	int data = FIntR();

// nastaven� f�ze sprajtu
	Sprite[inx].FazeNext(data);

// zaji�t�n� p�ekreslen� sprajtu
	SpriteClear(inx);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� f�ze sprajtu v seznamu (Data = index, List = seznam)

void FSpriteFazeList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index sprajtu
	int inx = ExecItem[-1].Data;
	inx += List.Inx(list);

// po�adovan� hodnota
	int data = FIntR();

// nastaven� f�ze sprajtu
	Sprite[inx].FazeNext(data);

// zaji�t�n� p�ekreslen� sprajtu
	SpriteClear(inx);

// inkrementace indexu seznamu
	List.AutoInc(list);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� f�ze lok�ln�ho sprajtu (Data = index)

void FSpriteFazeLoc()
{
// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;

// po�adovan� hodnota
	int data = FIntR();

// nastaven� f�ze sprajtu
	Sprite[inx].FazeNext(data);

// zaji�t�n� p�ekreslen� sprajtu
	SpriteClear(inx);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� f�ze lok�ln�ho sprajtu v seznamu (Data = index, List = seznam)

void FSpriteFazeLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;
	inx += List.Inx(list);

// po�adovan� hodnota
	int data = FIntR();

// nastaven� f�ze sprajtu
	Sprite[inx].FazeNext(data);

// zaji�t�n� p�ekreslen� sprajtu
	SpriteClear(inx);

// inkrementace indexu seznamu
	List.AutoInc(list);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� stavu glob�ln�ho sprajtu (Data = index)

void FSpriteStatus()
{
// index sprajtu
	int inx = ExecItem[-1].Data;

// po�adovan� hodnota
	double data = FNum();

// nastaven� stavu sprajtu
	if (data != Sprite[inx].Status())
	{
		Sprite[inx].Status(data);

// zaji�t�n� p�ekreslen� sprajtu
		SpriteClear(inx);
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� stavu sprajtu v seznamu (Data = index, List = seznam)

void FSpriteStatusList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index sprajtu
	int inx = ExecItem[-1].Data;
	inx += List.Inx(list);

// po�adovan� hodnota
	double data = FNum();

// nastaven� stavu sprajtu
	if (data != Sprite[inx].Status())
	{
		Sprite[inx].Status(data);

// zaji�t�n� p�ekreslen� sprajtu
		SpriteClear(inx);
	}

// inkrementace indexu seznamu
	List.AutoInc(list);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� stavu lok�ln�ho sprajtu (Data = index)

void FSpriteStatusLoc()
{
// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;

// po�adovan� hodnota
	double data = FNum();

// nastaven� stavu sprajtu
	if (data != Sprite[inx].Status())
	{
		Sprite[inx].Status(data);

// zaji�t�n� p�ekreslen� sprajtu
		SpriteClear(inx);
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� stavu lok�ln�ho sprajtu v seznamu (Data = index, List = seznam)

void FSpriteStatusLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;
	inx += List.Inx(list);

// po�adovan� hodnota
	double data = FNum();

// nastaven� stavu sprajtu
	if (data != Sprite[inx].Status())
	{
		Sprite[inx].Status(data);

// zaji�t�n� p�ekreslen� sprajtu
		SpriteClear(inx);
	}

// inkrementace indexu seznamu
	List.AutoInc(list);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� viditelnosti glob�ln�ho sprajtu (Data = index)

void FSpriteVis()
{
// index sprajtu
	int inx = ExecItem[-1].Data;

// po�adovan� hodnota
	bool data = FBool();

// nastaven� viditelnosti sprajtu
	SetSpriteVisible(inx, data);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� viditelnosti sprajtu v seznamu (Data = index, List = seznam)

void FSpriteVisList()
{
// index seznamu
	int list = ExecItem[-1].List;

// index sprajtu
	int inx = ExecItem[-1].Data;
	inx += List.Inx(list);

// po�adovan� hodnota
	bool data = FBool();

// nastaven� viditelnosti sprajtu
	SetSpriteVisible(inx, data);

// inkrementace indexu seznamu
	List.AutoInc(list);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� viditelnosti lok�ln�ho sprajtu (Data = index)

void FSpriteVisLoc()
{
// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;

// po�adovan� hodnota
	bool data = FBool();

// nastaven� viditelnosti sprajtu
	SetSpriteVisible(inx, data);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� viditelnosti lok�ln�ho sprajtu v seznamu (Data = index, List = seznam)

void FSpriteVisLocList()
{
// index seznamu
	int list = List.Num() - ExecItem[-1].List;

// index sprajtu
	int inx = Sprite.Num() - ExecItem[-1].Data;
	inx += List.Inx(list);

// po�adovan� hodnota
	bool data = FBool();

// nastaven� viditelnosti sprajtu
	SetSpriteVisible(inx, data);

// inkrementace indexu seznamu
	List.AutoInc(list);
}



/***************************************************************************\
*																			*
*							��dic� p��kazy									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// podm�nka IF

void FIf()
{
// test podm�nky
	if (FBool())
	{

// proveden� p��kazu p�i spln�n� podm�nky
		FCommand();
		ExecItem += ExecItem->Jump;
	}
	else
	{

// proveden� p��kazu p�i nespln�n� podm�nky
		ExecItem += ExecItem->Jump;
		FCommand();
	}
}


/////////////////////////////////////////////////////////////////////////////
// cyklus WHILE

void FWhile()
{
// obsluha p�ekreslov�n� s time-out
	int oldredispout = ReDispOut;
	bool istimer = false;

// �schova ukazatele podm�nky
	EXECITEM* oldexec = ExecItem;

// dokud je podm�nka spln�na
	while (FBool())
	{

// proveden� p��kazu
		FCommand();

// p�i p�eru�en� konec
		if (Break != 0)
		{
			Break &= ~BREAKWHILE;
			return;
		}

// ��t�n� obsluhy multitaskingu
		WhileOut--;
		if (WhileOut <= 0)
		{
			WhileOut = WHILEOUT;

// obsluha �asova�e
			if (TestTimer())
			{
				WaitTimer(false);			// obsluha zpr�v

				while (Pause && (Break == 0))
				{
#ifndef _MINI
					D3Render(true);
#endif // _MINI
					TimeWait(1);
				}
			}
		}

// obsluha p�ekreslov�n� (trv�-li tato smy�ka po 2 �asov� impulsy)
		if (oldredispout != ReDispOut)
		{
			if (istimer)
			{
				ReDispOut = 0;
				ReDisp();
			}
			istimer = true;
			oldredispout = ReDispOut;
		}

// n�vrat ukazatele na za��tek podm�nky
		ExecItem = oldexec;
	}

// konec cyklu - p�esko�en� p��kazu
	ExecItem += ExecItem->Jump;
}


/////////////////////////////////////////////////////////////////////////////
// cyklus REPEAT

void FRepeat()
{
// obsluha p�ekreslov�n� s time-out
	int oldredispout = ReDispOut;
	bool istimer = false;

// na�ten� po�tu pr�chod�
	int citac = abs(FIntR());

// �schova ukazatele p��kazu
	EXECITEM* oldexec = ExecItem;

// opakov�n� cyklu
	for (; citac > 0; citac--)
	{

// proveden� p��kazu
		FCommand();

// p�i p�eru�en� konec
		if (Break != 0)
		{
			Break &= ~BREAKWHILE;
			return;
		}

// ��t�n� obsluhy multitaskingu
		WhileOut--;
		if (WhileOut <= 0)
		{
			WhileOut = WHILEOUT;

// obsluha �asova�e
			if (TestTimer())
			{
				WaitTimer(false);			// obsluha zpr�v

				while (Pause && (Break == 0))
				{
#ifndef _MINI
					D3Render(true);
#endif // _MINI
					TimeWait(1);
				}
			}
		}

// obsluha p�ekreslov�n� (trv�-li tato smy�ka po 2 �asov� impulsy)
		if (oldredispout != ReDispOut)
		{
			if (istimer)
			{
				ReDispOut = 0;
				ReDisp();
			}
			istimer = true;
			oldredispout = ReDispOut;
		}

// n�vrat ukazatele na za��tek p��kazu
		ExecItem = oldexec;
	}

// konec cyklu - p�esko�en� p��kazu
	ExecItem += ExecItem->Jump;
}


/////////////////////////////////////////////////////////////////////////////
// p�eru�en� cyklu

void FBreak() { Break |= BREAKWHILE; }


/////////////////////////////////////////////////////////////////////////////
// p�eru�en� funkce

void FReturn() { Break |= BREAKFUNC; }


/////////////////////////////////////////////////////////////////////////////
// pr�zdn� p��kaz

void FCommandExec0() { };


/////////////////////////////////////////////////////////////////////////////
// 1 p��kaz

void FCommandExec1()
{ 
// p�i p�eru�en� zrychlen� p�esko�en� p��kazu
	if (Break != 0)
	{
		ExecItem += ExecItem->Jump;
	}
	else

// proveden� p��kazu
	{
		FCommand();
	}
};


/////////////////////////////////////////////////////////////////////////////
// posloupnost p��kaz� (Data = po�et p��kaz� - 1)

void FCommandExec()
{ 
// p��prava po�tu p��kaz�
	int i = ExecItem[-1].Data;

// cyklus p�es v�echny p��kazy
	for (; i >= 0; i--)
	{

// p�i p�eru�en� zrychlen� p�esko�en� p��kazu
		if (Break != 0)
		{
			ExecItem += ExecItem->Jump;
			return;
		}

// proveden� p��kazu
		FCommand();
	}
};


/////////////////////////////////////////////////////////////////////////////
// �ek�n� - 1 impuls

void FWait1()
{ 
#ifndef _MINI
	D3Render(true);
#endif // _MINI

	TimeWait(1);
}


/////////////////////////////////////////////////////////////////////////////
// �ek�n�

void FWait()
{
	int cekej = FInt18();

#ifndef _MINI
	D3Render(true);
#endif // _MINI

	if (cekej <= 0)
	{
		WaitTimer(false);
		ReDisp();
		while (Pause && (Break == 0))
		{
#ifndef _MINI
			D3Render(true);
#endif // _MINI
			WaitTimer(true);
		}
	}
	else
	{
		TimeWait(cekej);
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� textu stavov�ho ��dku

void FSetStatus()
{
	CString text;
	FText(text);
	StatusText = text;
	StatusAkt = true;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� textu nadpisu okna

void FSetCaption()
{
	CString text;
	FText(text);
	CaptionText = text;
	CaptionAkt = true;

	if (ConsoleOn) ReDisp();
}


/***************************************************************************\
*																			*
*							obsluha dialog�									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// dialog box

void FDialogBox() { FNumDialogBox(); };


/***************************************************************************\
*																			*
*							obsluha soubor�									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// nov� soubor

void FFileNew() { FBoolFileNew(); }


/////////////////////////////////////////////////////////////////////////////
// nov� adres��

void FDirNew() { FBoolDirNew(); }


/////////////////////////////////////////////////////////////////////////////
// zru�en� souboru/adres��e

void FFileDelete() { FBoolFileDelete(); }


/////////////////////////////////////////////////////////////////////////////
// nastaven� aktivn�ho adres��e

void FFileAct()
{
// na�ten� po�adovan�ho adres��e
	CString text;
	FText(text);

// nastaven� aktivn�ho adres��e
	if (!::SetCurrentDirectory(text)) FileError = true;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� jmenovky disku

void FDiskLabel()
{
// na�ten� jm�na disku
	CString text;
	FText(text);

// nastaven� jm�na disku
	if (!::SetVolumeLabel(NULL, text)) FileError = true;
}


/////////////////////////////////////////////////////////////////////////////
// soubor pro �ten�

void FFileRead()
{
// uzav�en� soubor�
	FileClose();

// nulov�n� offsetu	pro �ten�
	FileReadOff = 0;

// na�ten� jm�na souboru
	CString text;
	FText(text);
	text.TrimLeft();
	text.TrimRight();
	FileRead = text;
}


/////////////////////////////////////////////////////////////////////////////
// soubor pro z�pis

void FFileWrite()
{
// uzav�en� soubor�
	FileClose();

// nulov�n� offsetu	pro z�pis
	FileWriteOff = 0;

// na�ten� jm�na souboru
	CString text;
	FText(text);
	text.TrimLeft();
	text.TrimRight();
	FileWrite = text;
}


/////////////////////////////////////////////////////////////////////////////
// kopie soubor�

void FFileCopy() { FBoolFileCopy(); };


/////////////////////////////////////////////////////////////////////////////
// p�esun soubor�

void FFileMove() { FBoolFileMove(); };


/////////////////////////////////////////////////////////////////////////////
// nastaven� po�adovan�ho atributu

void _fastcall FFileAttrib(DWORD attrib)
{
// uzav�en� otev�en�ch soubor�
	FileClose();

// na�ten� hodnoty atributu
	bool atr = FBool();

// mus� b�t platn� jm�no v�stupn�ho souboru
	if (FileWrite.IsNotEmpty())
	{

// na�ten� aktu�ln�ch parametr�
		DWORD oldatr = ::GetFileAttributes(FileWrite);
		if ((int)oldatr != -1)
		{

// p��prava nov�ho stavu atribut�
			DWORD newatr = oldatr | attrib;
			if (!atr) newatr ^= attrib;

// nastaven� nov�ch atribut�, pokud jsou zm�n�ny
			if (oldatr != newatr)
			{
				newatr &= ~(FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_COMPRESSED);
				if (::SetFileAttributes(FileWrite, newatr)) return;
			}
		}
	}

// chyba
	FileError = true;
}

void FFileRO() { FFileAttrib(FILE_ATTRIBUTE_READONLY); };	// atribut R/O
void FFileHID() { FFileAttrib(FILE_ATTRIBUTE_HIDDEN); };	// atribut HID
void FFileSYS() { FFileAttrib(FILE_ATTRIBUTE_SYSTEM); };	// atribut SYS
void FFileARC() { FFileAttrib(FILE_ATTRIBUTE_ARCHIVE); };	// atribut ARC


/////////////////////////////////////////////////////////////////////////////
// nastaven� �asu vytvo�en� souboru

void FFileCreate()
{
// uzav�en� otev�en�ch soubor�
	FileClose();

// na�ten� �asu
	__int64 cas = FNumFileTimeUTC();

// nastaven� �asu pro soubor
	if (!FileWriteOpen() ||	!::SetFileTime(FileWriteHandle, (FILETIME*)&cas, NULL, NULL))
	{
		FileError = true;
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� �asu �ten� ze souboru

void FFileOpen()
{
// uzav�en� otev�en�ch soubor�
	FileClose();

// na�ten� �asu
	__int64 cas = FNumFileTimeUTC();

// nastaven� �asu pro soubor
	if (!FileWriteOpen() ||	!::SetFileTime(FileWriteHandle, NULL, (FILETIME*)&cas, NULL))
	{
		FileError = true;
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� �asu z�pisu do souboru

void FFileModi()
{
// uzav�en� otev�en�ch soubor�
	FileClose();

// na�ten� �asu
	__int64 cas = FNumFileTimeUTC();

// nastaven� �asu pro soubor
	if (!FileWriteOpen() ||	!::SetFileTime(FileWriteHandle, NULL, NULL, (FILETIME*)&cas))
	{
		FileError = true;
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� velikosti souboru

void FFileSize()
{
// na�ten� po�adovan� velikosti souboru
	int size = FIntR();
	if (size < 0) size = 0;

// uzav�en� soubor�
	FileClose();

// otev�en� souboru pro z�pis a nastaven� jeho velikosti
	if (!FileWriteOpen() ||
		((int)::SetFilePointer(FileWriteHandle, size, NULL, FILE_BEGIN) == -1) ||
		!::SetEndOfFile(FileWriteHandle))
	{
		FileError = true;
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� offsetu pro �ten�

void FFileReadOff()
{
	int off = FIntR();
	if (off < 0) off = 0;
	FileReadOff = off;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� offsetu pro z�pis

void FFileWriteOff()
{
	int off = FIntR();
	if (off < 0) off = 0;
	FileWriteOff = off;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� p��znaku chyby

void FFileError() { FileError = FBool(); }


/////////////////////////////////////////////////////////////////////////////
// nastaven� d�lky textu s pevnou d�lkou

void FFileTextNLen()
{
	int len = FIntR();
	if (len <= 0) len = 1;
	FileTextNLen = len;
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� p��znaku textu UNICODE

void FFileTextUnicode() { FileTextUnicode = FBool(); }


/////////////////////////////////////////////////////////////////////////////
// z�pis bajtu se znam�nkem

void FFileByteS()
{ 
	int cislo = FIntR();
	if (cislo > 0x7f) cislo = 0x7f;
	if (cislo < -0x80) cislo = -0x80;
	FileWriteByte((BYTE)cislo);
}


/////////////////////////////////////////////////////////////////////////////
// z�pis bajtu bez znam�nka

void FFileByteU()
{ 
	int cislo = FIntR();
	if (cislo > 0xff) cislo = 0xff;
	if (cislo < 0) cislo = 0;
	FileWriteByte((BYTE)cislo);
}


/////////////////////////////////////////////////////////////////////////////
// z�pis slova se znam�nkem

void FFileWordS()
{ 
	int cislo = FIntR();
	if (cislo > 0x7fff) cislo = 0x7fff;
	if (cislo < -0x8000) cislo = -0x8000;
	FileWriteWord((WORD)cislo);
}


/////////////////////////////////////////////////////////////////////////////
// z�pis slova bez znam�nka

void FFileWordU()
{ 
	int cislo = FIntR();
	if (cislo > 0xffff) cislo = 0xffff;
	if (cislo < 0) cislo = 0;
	FileWriteWord((WORD)cislo);
}


/////////////////////////////////////////////////////////////////////////////
// z�pis 3 bajt� se znam�nkem

void FFile3ByteS()
{ 
	long cislo = FIntR();
	if (cislo > 0x7fffff) cislo = 0x7fffff;
	if (cislo < -0x800000) cislo = -0x800000;
	FileWriteWord((WORD)cislo);
	FileWriteByte(((BYTE*)&cislo)[2]);
}


/////////////////////////////////////////////////////////////////////////////
// z�pis 3 bajt� bez znam�nka

void FFile3ByteU()
{ 
	long cislo = FIntR();
	if (cislo > 0xffffff) cislo = 0xffffff;
	if (cislo < 0) cislo = 0;
	FileWriteWord((WORD)cislo);
	FileWriteByte(((BYTE*)&cislo)[2]);
}


/////////////////////////////////////////////////////////////////////////////
// z�pis dvojslova se znam�nkem

void FFileDWordS()
{ 
	long cislo = FIntR();
	FileWriteDWord((DWORD)cislo);
}


/////////////////////////////////////////////////////////////////////////////
// z�pis dvojslova bez znam�nka

void FFileDWordU()
{ 
	__int64 cislo = (__int64)(FNum() + 0.5);
	if (cislo < 0) cislo = 0;
	FileWriteDWord((DWORD)cislo);
}


/////////////////////////////////////////////////////////////////////////////
// z�pis ��sla FLOAT

void FFileFloat()
{ 
	float cislo = (float)FNum();
	FileWriteDWord(((DWORD*)&cislo)[0]);
}


/////////////////////////////////////////////////////////////////////////////
// z�pis ��sla DOUBLE

void FFileDouble()
{ 
	double cislo = FNum();
	FileWriteDWord(((DWORD*)&cislo)[0]);
	FileWriteDWord(((DWORD*)&cislo)[1]);
}


/////////////////////////////////////////////////////////////////////////////
// z�pis logick� hodnoty

void FFileLogic()
{ 
	bool n = FBool();
	FileWriteByte((BYTE)n);
}


/////////////////////////////////////////////////////////////////////////////
// z�pis textu (s omezen�m d�lky, vrac� d�lku ulo�en�ho textu)

int _fastcall FFileText(const int max)
{
// na�ten� textu
	CString text;
	FText(text);

// p��prava d�lky textu
	int len = text.Length();
	if (len > max) len = max;

// z�pis textu
	int i = 0;

// z�pis v k�du UNICODE
	if (FileTextUnicode)
	{
#ifdef _UNICODE
		for (; len > 0; len--)
		{
			FileWriteWord(text[i]);
			i++;
		}
#else
		WCHAR* buf = (WCHAR*)MemGet((text.Length() + 1)*2);
		text.Write(buf);
		for (; len > 0; len--)
		{
			FileWriteWord(buf[i]);
			i++;
		}
		MemFree(buf);
#endif
	}

// z�pis v k�du WINDOWS
	else
	{
#ifdef _UNICODE
		char* buf = (char*)MemGet(text.Length() + 1);
		text.Write(buf);
		for (; len > 0; len--)
		{
			FileWriteByte(buf[i]);
			i++;
		}
		MemFree(buf);
#else
		for (; len > 0; len--)
		{
			FileWriteByte(text[i]);
			i++;
		}
#endif
	}

// n�vrat ulo�en� d�lky textu
	return i;
}


/////////////////////////////////////////////////////////////////////////////
// z�pis textu zakon�en�ho nulou

void FFileText0()
{
// z�pis textu
	FFileText(BigInt);

// z�pis koncov� nuly
	if (FileTextUnicode)
	{
		FileWriteWord(0);
	}
	else
	{
		FileWriteByte(0);
	}
}


/////////////////////////////////////////////////////////////////////////////
// z�pis textu zakon�en�ho CR/LF

void FFileTextCR()
{
// z�pis textu
	FFileText(BigInt);

// z�pis konce ��dku
	if (FileTextUnicode)
	{
		FileWriteWord(13);
		FileWriteWord(10);
	}
	else
	{
		FileWriteByte(13);
		FileWriteByte(10);
	}
}


/////////////////////////////////////////////////////////////////////////////
// z�pis textu s pevnou d�lkou

void FFileTextN()
{
// z�pis textu
	int i = FileTextNLen;
	i = i - FFileText(i);

// z�pis zarovn�vac�ch mezer
	if (FileTextUnicode)
	{
		for (; i > 0; i--)
		{
			FileWriteWord(' ');
		}
	}
	else
	{
		for (; i > 0; i--)
		{
			FileWriteByte(' ');
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// z�pis ikony

void FFileIcon()
{
	CIcon icon;
	FIcon(icon);
	icon.SaveFile();
}

/////////////////////////////////////////////////////////////////////////////
// z�pis obr�zku

void FFilePicture()
{
	CPicture pic;
	FPicture(pic);
	pic.SaveFile();
}

/////////////////////////////////////////////////////////////////////////////
// z�pis zvuku

void FFileSound()
{
	CSound sound;
	FSound(sound);
	sound.SaveFile();
}

/////////////////////////////////////////////////////////////////////////////
// z�pis hudby

void FFileMusic()
{
	CMusic music;
	FMusic(music);
	music.SaveFile();
}

/////////////////////////////////////////////////////////////////////////////
// z�pis sprajtu

void FFileSprite()
{
	CSprite sprite;
	FSprite(sprite);
	sprite.SaveFile();
}

/***************************************************************************\
*																			*
*							obsluha �asu									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// nastaven� aktu�ln�ho �asu

void FTimeCurrent()
{
// na�ten� �asu
	FNumSysTime();

// nastaven� nov�ho �asu
	::SetLocalTime(&SystemTime);
}


/***************************************************************************\
*																			*
*							obsluha CD disku								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// p�ehr�t� CD 

void FCDPlay() { CDPlay(FIntR()); };


/////////////////////////////////////////////////////////////////////////////
// zastaven� p�ehr�v�n� CD

void FCDStop() { CDStop(); };


/////////////////////////////////////////////////////////////////////////////
// pauza CD

void FCDPause() { CDPause(); };
void FCDPauseSet() 
{ 
	bool pause = FBool();
	if (CDAkt())
	{
		if (pause != CDPausing) CDPause(); 
	}
};


/////////////////////////////////////////////////////////////////////////////
// nastaven� pozice CD

void FCDSetPos() { CDSetPos(FInt1000()); };


/////////////////////////////////////////////////////////////////////////////
// nastaven� stopy CD

void FCDSetTrack() { CDSetTrack(FIntR()); };


/////////////////////////////////////////////////////////////////////////////
// nastaven� pozice stopy CD

void FCDSetTrackPos() { CDSetTrackPos(FInt1000()); };


/////////////////////////////////////////////////////////////////////////////
// vysunut� CD

void FCDEject() { CDEject(CDDiskValid); }
void FCDEjectSet() { CDEject(FBool()); }


/***************************************************************************\
*																			*
*							obsluha hudby									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// p�ehr�t� hudby

void FMusicPlay()
{
// na�ten� hudby k p�ehr�t�
	CMusic mus;
	FMusic(mus);

// p�ehr�t� hudby
	MusicPlay(mus, false);
}


/////////////////////////////////////////////////////////////////////////////
// opakovan� p�ehr�v�n� hudby

void FMusicLoop()
{
// na�ten� hudby k p�ehr�t�
	CMusic mus;
	FMusic(mus);

// p�ehr�t� hudby
	MusicPlay(mus, true);
}


/////////////////////////////////////////////////////////////////////////////
// zastaven� p�ehr�v�n� hudby

void FMusicStop() { MusicStop(); MusicPausing = false; };


/////////////////////////////////////////////////////////////////////////////
// pauza hudby

void FMusicPause() { MusicPause(); };
void FMusicPauseSet() 
{ 
	bool pause = FBool();
	if (MusicPlaing) MusicError = -1;
	if (pause != MusicPausing) MusicPause(); 
};


/////////////////////////////////////////////////////////////////////////////
// nastaven� pozice hudby

void FSetMusicPos() { SetMusicPos(FInt1000()); };


/***************************************************************************\
*																			*
*							obsluha Petra a Petry							*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// Petr - �ek�n� na dokon�en� pohybu

void PeterWait()
{
 	while ((Sprite[0].KrokCit() > 0) && (Break == 0)) TimeWait(1);
}


/////////////////////////////////////////////////////////////////////////////
// Petra - �ek�n� na dokon�en� pohybu

void Peter2Wait()
{
 	while ((Sprite[1].KrokCit() > 0) && (Break == 0)) TimeWait(1);
}


/////////////////////////////////////////////////////////////////////////////
// Petr - krok vp�ed

void FKrok()
{
// �ek�n� na dokon�en� pohybu
	PeterWait();

// na�ten� p��t� sou�adnice Petra
	int x;
	int y;
	if (PetrXYNext(x, y))
	{

// p�esun na p��t� pozici
		SpriteMove(0, x, y);
		PeterWait();
	}
}


/////////////////////////////////////////////////////////////////////////////
// Petra - krok vp�ed

void FKrok2()
{
// �ek�n� na dokon�en� pohybu
	Peter2Wait();

// na�ten� p��t� sou�adnice Petra
	int x;
	int y;
	if (Petr2XYNext(x, y))
	{

// p�esun na p��t� pozici
		SpriteMove(1, x, y);
	}
}


/////////////////////////////////////////////////////////////////////////////
// Petr - vlevo

void FVlevo()
{
// �ek�n� na dokon�en� pohybu
	PeterWait();

// oto�en� vlevo
	SetSpriteSmer(0, Sprite[0].SmerR() + uhel90);
}


/////////////////////////////////////////////////////////////////////////////
// Petra - vlevo

void FVlevo2()
{
// �ek�n� na dokon�en� pohybu
	Peter2Wait();

// oto�en� vlevo
	SetSpriteSmer(1, Sprite[1].SmerR() + uhel90);
}


/////////////////////////////////////////////////////////////////////////////
// Petr - vpravo

void FVpravo()
{
// �ek�n� na dokon�en� pohybu
	PeterWait();

// oto�en� vpravo
	SetSpriteSmer(0, Sprite[0].SmerR() - uhel90);
}


/////////////////////////////////////////////////////////////////////////////
// Petra - vpravo

void FVpravo2()
{
// �ek�n� na dokon�en� pohybu
	Peter2Wait();

// oto�en� vpravo
	SetSpriteSmer(1, Sprite[1].SmerR() - uhel90);
}


/////////////////////////////////////////////////////////////////////////////
// Petr - vzad

void FVzad()
{
// �ek�n� na dokon�en� pohybu
	PeterWait();

// oto�en� vzad
	SetSpriteSmer(0, Sprite[0].SmerR() + uhel180);
}


/////////////////////////////////////////////////////////////////////////////
// Petra - vzad

void FVzad2()
{
// �ek�n� na dokon�en� pohybu
	Peter2Wait();

// oto�en� vzad
	SetSpriteSmer(1, Sprite[1].SmerR() + uhel180);
}


/////////////////////////////////////////////////////////////////////////////
// Petr - viditelnost

void FVisible()
{
// �ek�n� na dokon�en� pohybu
	PeterWait();

// nastaven� viditelnosti
	SetSpriteVisible(0, FBool());
}


/////////////////////////////////////////////////////////////////////////////
// Petra - viditelnost

void FVisible2()
{
// �ek�n� na dokon�en� pohybu
	Peter2Wait();

// nastaven� viditelnosti
	SetSpriteVisible(1, FBool());
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� p�edm�tu p�ed Petrem

void FSetPredmet()
{
// na�ten� ikony
	CIcon icon;
	FIcon(icon);

// jen nen�-li to pr�zdn� ikona
	if (icon.IsNotEmpty())
	{

// na�ten� p��t� sou�adnice Petra
		int x;
		int y;
		if (PetrXYNext(x, y))
		{

// adresa p�edm�tu
			MAPITEM* item = &Map[0].At(x, y);

// test, zda je zm�na p�edm�tu
			if (item->Icon != icon)
			{

// �ek�n� na dokon�en� pohybu
				PeterWait();

// nastaven� nov�ho p�edm�tu
				item->Icon = icon;
				SetAktIcon(x, y, true);
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� p�edm�tu p�ed Petrou

void FSetPredmet2()
{
// na�ten� ikony
	CIcon icon;
	FIcon(icon);

// jen nen�-li to pr�zdn� ikona
	if (icon.IsNotEmpty())
	{

// na�ten� p��t� sou�adnice Petry
		int x;
		int y;
		if (Petr2XYNext(x, y))
		{

// adresa p�edm�tu
			MAPITEM* item = &Map[0].At(x, y);

// test, zda je zm�na p�edm�tu
			if (item->Icon != icon)
			{

// �ek�n� na dokon�en� pohybu
				Peter2Wait();

// nastaven� nov�ho p�edm�tu
				item->Icon = icon;
				SetAktIcon(x, y, true);
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� p�edm�tu pod Petrem

void FSetPredmetPoz()
{
// na�ten� ikony
	CIcon icon;
	FIcon(icon);

// jen nen�-li to pr�zdn� ikona
	if (icon.IsNotEmpty())
	{

// na�ten� sou�adnice Petra
		int x;
		int y;
		if (PetrXY(x, y))
		{

// adresa p�edm�tu
			MAPITEM* item = &Map[0].At(x, y);

// test, zda je zm�na p�edm�tu
			if (item->Icon != icon)
			{

// �ek�n� na dokon�en� pohybu
				PeterWait();

// nastaven� nov�ho p�edm�tu
				item->Icon = icon;
				SetAktIcon(x, y, true);
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� p�edm�tu pod Petrou

void FSetPredmet2Poz()
{
// na�ten� ikony
	CIcon icon;
	FIcon(icon);

// jen nen�-li to pr�zdn� ikona
	if (icon.IsNotEmpty())
	{

// na�ten� sou�adnice Petry
		int x;
		int y;
		if (Petr2XY(x, y))
		{

// adresa p�edm�tu
			MAPITEM* item = &Map[0].At(x, y);

// test, zda je zm�na p�edm�tu
			if (item->Icon != icon)
			{

// �ek�n� na dokon�en� pohybu
				Peter2Wait();

// nastaven� nov�ho p�edm�tu
				item->Icon = icon;
				SetAktIcon(x, y, true);
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// Petr - nastaven� sm�ru

void FSetSmer() { PeterWait(); SetSpriteSmer(0, FNum()); };


/////////////////////////////////////////////////////////////////////////////
// Petra - nastaven� sm�ru

void FSetSmer2() { Peter2Wait(); SetSpriteSmer(1, FNum()); };


/////////////////////////////////////////////////////////////////////////////
// Petr - nastaven� sou�adnice X

void FSetPozX() 
{ 
	double num = FNum();
	SpriteMove(0, num, Sprite[0].CilY());
}


/////////////////////////////////////////////////////////////////////////////
// Petra - nastaven� sou�adnice X

void FSetPozX2()
{ 
	double num = FNum();
	SpriteMove(1, num, Sprite[1].CilY());
};


/////////////////////////////////////////////////////////////////////////////
// Petr - nastaven� sou�adnice Y

void FSetPozY()
{ 
	double num = FNum();
	SpriteMove(0, Sprite[0].CilX(), num);
};


/////////////////////////////////////////////////////////////////////////////
// Petra - nastaven� sou�adnice Y

void FSetPozY2()
{ 
	double num = FNum();
	SpriteMove(1, Sprite[1].CilX(), num);
};


/////////////////////////////////////////////////////////////////////////////
// Petr - nastaven� ��sla

void _fastcall FSetSwcN(int mask, char rot)
{
// na�ten� ��sla
	int cislo = FIntR();

// na�ten� pozice Petra
	MAPITEM* item = PetrXYMap();

// nastaven� ��sla, je-li platn� pozice
	if (item != NULL)
	{
		cislo <<= rot;
		cislo &= mask;
		item->Param &= ~mask;
		item->Param |= cislo;
	}
}

void FSetSwcN1() { FSetSwcN(MAP_VALUE1_MASK, MAP_VALUE1_ROT); };
void FSetSwcN2() { FSetSwcN(MAP_VALUE2_MASK, MAP_VALUE2_ROT); };
void FSetSwcN3() { FSetSwcN(MAP_VALUE3_MASK, MAP_VALUE3_ROT); };


/////////////////////////////////////////////////////////////////////////////
// Petra - nastaven� ��sla

void _fastcall FSet2SwcN(int mask, char rot)
{
// na�ten� ��sla
	int cislo = FIntR();

// na�ten� pozice Petry
	MAPITEM* item = Petr2XYMap();

// nastaven� ��sla, je-li platn� pozice
	if (item != NULL)
	{
		cislo <<= rot;
		cislo &= mask;
		DWORD param = item->Param;
		param &= ~mask;
		param |= cislo;
		item->Param = param;
	}
}

void FSetSwcN12() { FSet2SwcN(MAP_VALUE1_MASK, MAP_VALUE1_ROT); };
void FSetSwcN22() { FSet2SwcN(MAP_VALUE2_MASK, MAP_VALUE2_ROT); };
void FSetSwcN32() { FSet2SwcN(MAP_VALUE3_MASK, MAP_VALUE3_ROT); };


/////////////////////////////////////////////////////////////////////////////
// Petr - nastaven� p�ep�na�e

void _fastcall FSetSwc(DWORD mask)
{
// na�ten� hodnoty p�ep�na�e
	bool log = FBool();

// na�ten� pozice Petra
	MAPITEM* item = PetrXYMap();

// nastaven� p�ep�na�e, je-li platn� pozice
	if (item != NULL)
	{
		DWORD param = item->Param | mask;
		if (!log) param ^= mask;
		item->Param = param;
	}
}

void FSetSwc1() { FSetSwc(MAP_FLAG1); };	// p�ep�na� 1 - Petr
void FSetSwc2() { FSetSwc(MAP_FLAG2); };	// p�ep�na� 2 - Petr
void FSetSwc3() { FSetSwc(MAP_FLAG3); };	// p�ep�na� 3 - Petr
void FSetSwc4() { FSetSwc(MAP_FLAG4); };	// p�ep�na� 4 - Petr
void FSetSwc5() { FSetSwc(MAP_FLAG5); };	// p�ep�na� 5 - Petr


/////////////////////////////////////////////////////////////////////////////
// Petra - nastaven� p�ep�na�e

void _fastcall FSet2Swc(DWORD mask)
{
// na�ten� hodnoty p�ep�na�e
	bool log = FBool();

// na�ten� pozice Petry
	MAPITEM* item = Petr2XYMap();

// nastaven� p�ep�na�e, je-li platn� pozice
	if (item != NULL)
	{
		DWORD param = item->Param | mask;
		if (!log) param ^= mask;
		item->Param = param;
	}
}

void FSetSwc12() { FSet2Swc(MAP_FLAG1); };	// p�ep�na� 1 - Petra
void FSetSwc22() { FSet2Swc(MAP_FLAG2); };	// p�ep�na� 2 - Petra
void FSetSwc32() { FSet2Swc(MAP_FLAG3); };	// p�ep�na� 3 - Petra
void FSetSwc42() { FSet2Swc(MAP_FLAG4); };	// p�ep�na� 4 - Petra
void FSetSwc52() { FSet2Swc(MAP_FLAG5); };	// p�ep�na� 5 - Petra


/***************************************************************************\
*																			*
*							obsluha zvuku									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// p�ehr�t� zvuku

void FSoundPlay()
{
// na�ten� zvuku k p�ehr�t�
	CSound sound;
	FSound(sound);

// na�ten� ostatn�ch parametr�
	double volume = FNum();			// hlasitost
	double balance = FNum();		// balance
	double speed = FNum();			// rychlost
	int loop = FIntR();				// opakov�n�
	bool stop = FBool();			// p�eru�en�

// p�ehr�t� zvuku
	PlaySoundOpen(sound, loop, stop, volume, balance, speed);
}


/////////////////////////////////////////////////////////////////////////////
// zastaven� v�ech zvuk�

void FSoundStop() { PlaySoundStop(); };


/////////////////////////////////////////////////////////////////////////////
// zastaven� konkr�tn�ho zvuku

void FSoundStop1()
{
// na�ten� zvuku k zastaven�
	CSound sound;
	FSound(sound);

// zastaven� zvuku
	PlaySoundClose(sound);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� pozice zvuku

void FSetSoundPos() { SetSoundPos(FNum()); };


/////////////////////////////////////////////////////////////////////////////
// pauza zvuku

void FSoundPause() { SoundPause(!SoundPausing); };
void FSoundPauseSet() 
{ 
	bool pause = FBool();
	SoundPause(pause); 
};

/////////////////////////////////////////////////////////////////////////////
// nastaven� rozli�ovac�ho k�du

void FSetSoundDistinguish()
{
	SoundID = (WORD)FIntR();
}

/***************************************************************************\
*																			*
*							obsluha grafiky									*
*																			*
\***************************************************************************/

int	PointWidth = 1;					// nastaven� ���ka grafick�ho bodu
BYTE PointCol = YellowCol;			// nastaven� barva grafick�ho bodu
BYTE PointFill = BackCol;			// barva pozad� pro v�pl�

/////////////////////////////////////////////////////////////////////////////
// zobrazen� jednoduch�ho bodu

void _fastcall FPoint1(int x, int y)
{
	if (((DWORD)x < (DWORD)Width) && ((DWORD)y < (DWORD)Height))
	{
		BackBuf[x + y*WidthByte] = PointCol;
		SetAktIcon(x/ICONWIDTH, y/ICONHEIGHT, true);
	}
}

void _fastcall FPoint01(int x, int y)
{
	if (((DWORD)x < (DWORD)Width) && ((DWORD)y < (DWORD)Height))
	{
		BackBuf[x + y*WidthByte] = PointCol;
	}
}


/////////////////////////////////////////////////////////////////////////////
// zobrazen� bodu nebo kruhu (���ka "PointWidth" mus� b�t > 0 !)

void _fastcall CirclePoint(int x, int y)
{
	switch (PointWidth)
	{
	case 0:
			break;
//      *
	case 1:
			FPoint1(x, y);
			break;

//		oo
//		*o
	case 2:
			FPoint1(x  , y  );
			FPoint1(x+1, y  );

			FPoint1(x  , y+1);
			FPoint1(x+1, y+1);
			break;

//		ooo
//		o*o
//		ooo
	case 3:
			FPoint1(x-1, y-1);
			FPoint1(x  , y-1);
			FPoint1(x+1, y-1);

			FPoint1(x-1, y  );
			FPoint1(x  , y  );
			FPoint1(x+1, y  );

			FPoint1(x-1, y+1);
			FPoint1(x  , y+1);
			FPoint1(x+1, y+1);
			break;

//		 oo 
//		oooo
//		o*oo
//		 oo
	case 4:
			FPoint1(x  , y-1);
			FPoint1(x+1, y-1);

			FPoint1(x-1, y  );
			FPoint1(x  , y  );
			FPoint1(x+1, y  );
			FPoint1(x+2, y  );

			FPoint1(x-1, y+1);
			FPoint1(x  , y+1);
			FPoint1(x+1, y+1);
			FPoint1(x+2, y+1);

			FPoint1(x  , y+2);
			FPoint1(x+1, y+2);
			break;

//		 ooo  
//		ooooo
//		oo*oo
//		ooooo
//		 ooo 
	case 5:
			FPoint1(x-1, y-2);
			FPoint1(x  , y-2);
			FPoint1(x+1, y-2);

			FPoint1(x-2, y-1);
			FPoint1(x-1, y-1);
			FPoint1(x  , y-1);
			FPoint1(x+1, y-1);
			FPoint1(x+2, y-1);

			FPoint1(x-2, y  );
			FPoint1(x-1, y  );
			FPoint1(x  , y  );
			FPoint1(x+1, y  );
			FPoint1(x+2, y  );

			FPoint1(x-2, y+1);
			FPoint1(x-1, y+1);
			FPoint1(x  , y+1);
			FPoint1(x+1, y+1);
			FPoint1(x+2, y+1);

			FPoint1(x-1, y+2);
			FPoint1(x  , y+2);
			FPoint1(x+1, y+2);
			break;

//		 oooo  
//		oooooo 
//		oooooo
//		oo*ooo
//		oooooo
//		 oooo 
	case 6: 
			FPoint1(x-1, y-2);
			FPoint1(x  , y-2);
			FPoint1(x+1, y-2);
			FPoint1(x+2, y-2);

			FPoint1(x-2, y-1);
			FPoint1(x-1, y-1);
			FPoint1(x  , y-1);
			FPoint1(x+1, y-1);
			FPoint1(x+2, y-1);
			FPoint1(x+3, y-1);

			FPoint1(x-2, y  );
			FPoint1(x-1, y  );
			FPoint1(x  , y  );
			FPoint1(x+1, y  );
			FPoint1(x+2, y  );
			FPoint1(x+3, y  );

			FPoint1(x-2, y+1);
			FPoint1(x-1, y+1);
			FPoint1(x  , y+1);
			FPoint1(x+1, y+1);
			FPoint1(x+2, y+1);
			FPoint1(x+3, y+1);

			FPoint1(x-2, y+2);
			FPoint1(x-1, y+2);
			FPoint1(x  , y+2);
			FPoint1(x+1, y+2);
			FPoint1(x+2, y+2);
			FPoint1(x+3, y+2);

			FPoint1(x-1, y+3);
			FPoint1(x  , y+3);
			FPoint1(x+1, y+3);
			FPoint1(x+2, y+3);
			break;

//		  ooo  
//		 ooooo 
//		ooooooo
//		ooo*ooo
//		ooooooo
//		 ooooo 
//		  ooo  
	case 7: 
		    FPoint1(x-1, y-3);
			FPoint1(x  , y-3);
			FPoint1(x+1, y-3);

			FPoint1(x-2, y-2);
			FPoint1(x-1, y-2);
			FPoint1(x  , y-2);
			FPoint1(x+1, y-2);
			FPoint1(x+2, y-2);

			FPoint1(x-3, y-1);
			FPoint1(x-2, y-1);
			FPoint1(x-1, y-1);
			FPoint1(x  , y-1);
			FPoint1(x+1, y-1);
			FPoint1(x+2, y-1);
			FPoint1(x+3, y-1);

			FPoint1(x-3, y  );
			FPoint1(x-2, y  );
			FPoint1(x-1, y  );
			FPoint1(x  , y  );
			FPoint1(x+1, y  );
			FPoint1(x+2, y  );
			FPoint1(x+3, y  );

			FPoint1(x-3, y+1);
			FPoint1(x-2, y+1);
			FPoint1(x-1, y+1);
			FPoint1(x  , y+1);
			FPoint1(x+1, y+1);
			FPoint1(x+2, y+1);
			FPoint1(x+3, y+1);

			FPoint1(x-2, y+2);
			FPoint1(x-1, y+2);
			FPoint1(x  , y+2);
			FPoint1(x+1, y+2);
			FPoint1(x+2, y+2);

		    FPoint1(x-1, y+3);
			FPoint1(x  , y+3);
			FPoint1(x+1, y+3);
			break;

//		  oooo  
//		 oooooo 
//		oooooooo
//		oooooooo
//		ooo*oooo
//		oooooooo
//		 oooooo 
//		  oooo  
	case 8: 
		    FPoint1(x-1, y-3);
			FPoint1(x  , y-3);
			FPoint1(x+1, y-3);
			FPoint1(x+2, y-3);

			FPoint1(x-2, y-2);
			FPoint1(x-1, y-2);
			FPoint1(x  , y-2);
			FPoint1(x+1, y-2);
			FPoint1(x+2, y-2);
			FPoint1(x+3, y-2);

			FPoint1(x-3, y-1);
			FPoint1(x-2, y-1);
			FPoint1(x-1, y-1);
			FPoint1(x  , y-1);
			FPoint1(x+1, y-1);
			FPoint1(x+2, y-1);
			FPoint1(x+3, y-1);
			FPoint1(x+4, y-1);

			FPoint1(x-3, y  );
			FPoint1(x-2, y  );
			FPoint1(x-1, y  );
			FPoint1(x  , y  );
			FPoint1(x+1, y  );
			FPoint1(x+2, y  );
			FPoint1(x+3, y  );
			FPoint1(x+4, y  );

			FPoint1(x-3, y+1);
			FPoint1(x-2, y+1);
			FPoint1(x-1, y+1);
			FPoint1(x  , y+1);
			FPoint1(x+1, y+1);
			FPoint1(x+2, y+1);
			FPoint1(x+3, y+1);
			FPoint1(x+4, y+1);

			FPoint1(x-3, y+2);
			FPoint1(x-2, y+2);
			FPoint1(x-1, y+2);
			FPoint1(x  , y+2);
			FPoint1(x+1, y+2);
			FPoint1(x+2, y+2);
			FPoint1(x+3, y+2);
			FPoint1(x+4, y+2);

			FPoint1(x-2, y+3);
			FPoint1(x-1, y+3);
			FPoint1(x  , y+3);
			FPoint1(x+1, y+3);
			FPoint1(x+2, y+3);
			FPoint1(x+3, y+3);

		    FPoint1(x-1, y+4);
			FPoint1(x  , y+4);
			FPoint1(x+1, y+4);
			FPoint1(x+2, y+4);
			break;

//		  ooooo  
//		 ooooooo 
//		ooooooooo
//		ooooooooo
//		oooo*oooo
//		ooooooooo
//		ooooooooo
//		 ooooooo 
//		  ooooo  
	case 9: 
		    FPoint1(x-2, y-4);
		    FPoint1(x-1, y-4);
			FPoint1(x  , y-4);
			FPoint1(x+1, y-4);
			FPoint1(x+2, y-4);

		    FPoint1(x-3, y-3);
		    FPoint1(x-2, y-3);
		    FPoint1(x-1, y-3);
			FPoint1(x  , y-3);
			FPoint1(x+1, y-3);
			FPoint1(x+2, y-3);
		    FPoint1(x+3, y-3);

		    FPoint1(x-4, y-2);
		    FPoint1(x-3, y-2);
			FPoint1(x-2, y-2);
			FPoint1(x-1, y-2);
			FPoint1(x  , y-2);
			FPoint1(x+1, y-2);
			FPoint1(x+2, y-2);
			FPoint1(x+3, y-2);
		    FPoint1(x+4, y-2);

		    FPoint1(x-4, y-1);
			FPoint1(x-3, y-1);
			FPoint1(x-2, y-1);
			FPoint1(x-1, y-1);
			FPoint1(x  , y-1);
			FPoint1(x+1, y-1);
			FPoint1(x+2, y-1);
			FPoint1(x+3, y-1);
			FPoint1(x+4, y-1);

		    FPoint1(x-4, y  );
			FPoint1(x-3, y  );
			FPoint1(x-2, y  );
			FPoint1(x-1, y  );
			FPoint1(x  , y  );
			FPoint1(x+1, y  );
			FPoint1(x+2, y  );
			FPoint1(x+3, y  );
			FPoint1(x+4, y  );

		    FPoint1(x-4, y+1);
			FPoint1(x-3, y+1);
			FPoint1(x-2, y+1);
			FPoint1(x-1, y+1);
			FPoint1(x  , y+1);
			FPoint1(x+1, y+1);
			FPoint1(x+2, y+1);
			FPoint1(x+3, y+1);
			FPoint1(x+4, y+1);

		    FPoint1(x-4, y+2);
			FPoint1(x-3, y+2);
			FPoint1(x-2, y+2);
			FPoint1(x-1, y+2);
			FPoint1(x  , y+2);
			FPoint1(x+1, y+2);
			FPoint1(x+2, y+2);
			FPoint1(x+3, y+2);
			FPoint1(x+4, y+2);

		    FPoint1(x-3, y+3);
			FPoint1(x-2, y+3);
			FPoint1(x-1, y+3);
			FPoint1(x  , y+3);
			FPoint1(x+1, y+3);
			FPoint1(x+2, y+3);
			FPoint1(x+3, y+3);

		    FPoint1(x-2, y+4);
		    FPoint1(x-1, y+4);
			FPoint1(x  , y+4);
			FPoint1(x+1, y+4);
			FPoint1(x+2, y+4);
			break;

	default:
		{
// lok�ln� prom�nn�
			int width = PointWidth;
			if (width > 20000) width = 20000;	// omezen� kv�li p�ete�en�
			int prumer2 = width*width;			// kvadr�t pr�m�ru
			int dx2;							// kvadr�t vzd�lenosti od st�edu X * 2
			int dy2;							// kvadr�t vzd�lenosti od st�edu Y * 2
			int xx;								// ukazatel sou�adnice X
			int yy;								// ukazatel sou�adnice Y
			BYTE* data;							// ukazatel dat

// po��te�n� sou�adnice X
			int x1 = x - (width - 1) / 2;		// v�choz� sou�adnice X
			if (x1 < 0) x1 = 0;
			
// koncov� sou�adnice X - n�sleduj�c�
			int x2 = x + width / 2 + 1;			// koncov� sou�adnice X (n�sleduj�c�)
			if (x2 > Width) x2 = Width;

// po��te�n� sou�adnice Y
			int y1 = y - (width - 1) / 2;		// v�choz� sou�adnice Y
			if (y1 < 0) y1 = 0;
			
// koncov� sou�adnice Y - n�sleduj�c�
			int y2 = y + width / 2 + 1;			// koncov� sou�adnice Y - n�sleduj�c�
			if (y2 > Height) y2 = Height;

// aktualizace ikon
			AktIconBoxG(x1, y1, x2, y2);

// cyklus p�es v�echny linky
			for (yy = y1; yy < y2; yy++)		// cyklus p�es v�echny linky
			{	
				dy2 = (yy - y)*2;				// vzd�lenost od st�edu Y * 2
				dy2 = dy2*dy2;					// kvadr�t vzd�lenosti Y

// cyklus p�es v�echny pozice na lince
				data = &(BackBuf[x1 + yy * WidthByte]);

				for (xx = x1; xx < x2; xx++)	// cyklus p�es v�echny body na lince
				{
					dx2 = (xx - x)*2;			// vzd�lenost od st�edu X * 2
					dx2 = dx2*dx2;				// kvadr�t vzd�lenosti X

// kontrola, zda je platn� bod kruhu
					if (dx2 + dy2 <= prumer2)
					{
						*data = PointCol;
					}
					data++;
				}
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// zobrazen� bodu

void FPoint()
{
// na�ten� barvy bodu
	PointCol = (BYTE)FNumCol();

// na�ten� ���ky bodu
	PointWidth = FIntR();

// na�ten� sou�adnic bodu
	int x = FIntX();
	int y = FIntY();

// zobrazen� bodu
	if (PointWidth > 0) CirclePoint(x, y);
}


/////////////////////////////////////////////////////////////////////////////
// zobrazen� ��ry

void FLine()
{
// na�ten� barvy ��ry
	int col = FNumCol();
	PointCol = (BYTE)col;

// na�ten� ���ky ��ry
	int width = FIntR();
	PointWidth = width;

// na�ten� sou�adnic ��ry
	int x1 = FIntX();
	int y1 = FIntY();
	int x2 = FIntX();
	int y2 = FIntY();

// kontrola ���ky ��ry
	if (width <= 0) return;

// zaji�t�n�, aby ��ra �la zleva doprava
	int tmp;
	if (x1 > x2)
	{
		tmp = x2;
		x2 = x1;
		x1 = tmp;

		tmp = y2;
		y2 = y1;
		y1 = tmp;
	}

// lok�ln� prom�nn� pro zobrazen� ��ry
	double inkrement;		// p��rustek sou�adnice
	double stradac;			// st�ada� sou�adnice
	int citac;				// ��ta� bod� k zobrazen�
	int x, y;				// ukazatel sou�adnic
	int sirka;				// ��ta� ���ky
	int mwidth = Width;
	int mheight = Height;
	int widthbyte = WidthByte;
	int width2;

// ��ra jde zdola nahoru (zleva doprava)
	if (y1 <= y2)
	{
		AktIconBoxG(x1-width/2-1, y1-width/2-1, x2+width/2+1, y2+width/2+1);

// ��ra jde strm� zdola nahoru
		if ((x2 - x1) <= (y2 - y1))
		{
			inkrement = (double)(x2 - x1) / (y2 - y1);

			width2 = (int)((double)width * sqrt((double)(inkrement*inkrement) + 1) + 0.4);

			for (sirka = 0; sirka < width2; sirka++)
			{
				y = (int)((double)y1 + (inkrement * ((width2+1)/2 - sirka))*0.7 + 0.5);
				citac = y2 - y1;			// po�et krok�
				stradac = x1 - (width2 - 1)/2 + sirka + ((y - y1)*inkrement);

				for (; citac > 0; citac--)
				{
					x = (int)(stradac + 0.5);

					if ((y >= 0) && (y < mheight) &&
						(x >= 0) && (x < mwidth))
					{
						BackBuf[x + y * widthbyte] = (BYTE)col;
					}
					y++;
					stradac += inkrement;
				}
			}	
		}

// ��ra jde m�rn� zdola nahoru
		else
		{
			inkrement = (double)(y2 - y1) / (x2 - x1);

			width2 = (int)((double)width * sqrt((double)(inkrement*inkrement) + 1) + 0.4);

			for (sirka = 0; sirka < width2; sirka++)
			{
				x = (int)((double)x1 + (inkrement * ((width2+1)/2 - sirka))*0.7 + 0.5);
				citac = x2 - x1;			// po�et krok�
				stradac = y1 - (width2 - 1)/2 + sirka + ((x - x1)*inkrement);

				for (; citac > 0; citac--)
				{
					y = (int)(stradac + 0.5);

					if ((y >= 0) && (y < mheight) &&
						(x >= 0) && (x < mwidth))
					{
						BackBuf[x + y * widthbyte] = (BYTE)col;
					}
					x++;
					stradac += inkrement;
				}
			}	
		}
	}

// ��ra jde shora dol� (zleva doprava)
	else
	{
		AktIconBoxG(x1-width/2-1, y2-width/2-1, x2+width/2+1, y1+width/2+1);

// ��ra jde strm� shora dol�
		if ((x2 - x1) <= (y1 - y2))
		{
			inkrement = (double)(x2 - x1) / (y1 - y2);

			width2 = (int)((double)width * sqrt((double)(inkrement*inkrement) + 1) + 0.4);

			for (sirka = 0; sirka < width2; sirka++)
			{
				y = (int)((double)y1 - (inkrement * (((double)width2-2)/2 - sirka))*0.7);
				citac = y1 - y2;			// po�et krok�
				stradac = x1 - (width2 - 1)/2 + sirka + ((y1 - y)*inkrement);

				for (; citac > 0; citac--)
				{
					x = (int)(stradac + 0.5);

					if ((y >= 0) && (y < mheight) &&
						(x >= 0) && (x < mwidth))
					{
						BackBuf[x + y * widthbyte] = (BYTE)col;
					}
					y--;
					stradac += inkrement;
				}
			}	
		}

// ��ra jde m�rn� shora dol�
		else
		{
			inkrement = (double)(y1 - y2) / (x2 - x1);

			width2 = (int)((double)width * sqrt((double)(inkrement*inkrement) + 1) + 0.4);

			for (sirka = 0; sirka < width2; sirka++)
			{
				x = (int)((double)x1 - (inkrement * (((double)width2-2)/2 - sirka))*0.7);
				citac = x2 - x1;			// po�et krok�
				stradac = y1 - (width2 - 1)/2 + sirka + ((x1 - x)*inkrement);

				for (; citac > 0; citac--)
				{
					y = (int)(stradac + 0.5);

					if ((y >= 0) && (y < mheight) &&
						(x >= 0) && (x < mwidth))
					{
						BackBuf[x + y * widthbyte] = (BYTE)col;
					}
					x++;
					stradac -= inkrement;
				}
			}	
		}
	}

// nakreslen� okrajov�ch bod�
	CirclePoint(x1, y1);
	CirclePoint(x2, y2);
}


/////////////////////////////////////////////////////////////////////////////
// zobrazen� horizont�ln� ��ry

void _fastcall HLine(int x1, int x2, int y)
{
	if ((x2 < 0) || (x1 >= Width))
		return;

	if (x1 < 0) x1 = 0;
	if (x2 >= Width) x2 = Width - 1;
	int delka = x2 - x1 + 1;
	
	int width = PointWidth;

	y -= (width-1)/2;

	for (; width > 0; width--)
	{
		if ((y >= 0) && (y < Height))
		{
			MemFill(BackBuf + x1 + y*WidthByte, delka, PointCol);
		}
		y++;
	}
}


/////////////////////////////////////////////////////////////////////////////
// zobrazen� vertik�ln� ��ry

void _fastcall VLine(int x, int y1, int y2)
{
	if ((y2 < 0) || (y1 >= Height))
		return;

	if (y1 < 0) y1 = 0;
	if (y2 >= Height) y2 = Height - 1;
	
	int width = PointWidth;

	x -= (width-1)/2;

	int y;
	int widthbyte = WidthByte;
	BYTE* data;

	for (; width > 0; width--)
	{
		if ((x >= 0) && (x < Width))
		{
			data = BackBuf + x + y1*widthbyte;
			for (y = y1; y <= y2; y++)
			{
				*data = PointCol;
				data += widthbyte;
			}
		}
		x++;
	}
}


/////////////////////////////////////////////////////////////////////////////
// zobrazen� obd�ln�ku

void FBox()
{
// na�ten� barvy obd�ln�ku
	int col = FNumCol();
	PointCol = (BYTE)col;

// na�ten� ���ky ��ry obd�ln�ku
	int width = FIntR();
	PointWidth = width;

// na�ten� sou�adnic obd�ln�ku
	int x1 = FIntX();
	int y1 = FIntY();
	int x2 = FIntX();
	int y2 = FIntY();

// kontrola ���ky ��ry obd�ln�ku
	if (width <= 0) return;

	int tmp;

	if (x1 > x2)
	{
		tmp = x2;
		x2 = x1;
		x1 = tmp;
	}
	
	if (y1 > y2)
	{
		tmp = y2;
		y2 = y1;
		y1 = tmp;
	}

	AktIconBoxG(x1-width/2-1, y1-width/2-1, x2+width/2+1, y2+width/2+1);

	HLine(x1, x2, y1);
	HLine(x1, x2, y2);
	VLine(x1, y1, y2);
	VLine(x2, y1, y2);

	CirclePoint(x1, y1);
	CirclePoint(x2, y1);
	CirclePoint(x1, y2);
	CirclePoint(x2, y2);
}


/////////////////////////////////////////////////////////////////////////////
// zobrazen� vypln�n�ho obd�ln�ku

void FBoxFill()
{
// na�ten� barvy obd�ln�ku
	int col = FNumCol();
	PointCol = (BYTE)col;

// na�ten� sou�adnic obd�ln�ku
	int x1 = FIntX();
	int y1 = FIntY();
	int x2 = FIntX();
	int y2 = FIntY();

	int tmp;

	if (x1 > x2)
	{
		tmp = x2;
		x2 = x1;
		x1 = tmp;
	}
	
	if (y1 > y2)
	{
		tmp = y2;
		y2 = y1;
		y1 = tmp;
	}

	if (x1 < 0) x1 = 0;
	if (x2 < 0) x2 = 0;
	if (y1 < 0) y1 = 0;
	if (y2 < 0) y2 = 0;
	if (x1 > Width) x1 = Width;
	if (x2 > Width) x2 = Width;
	if (y1 > Height) y1 = Height;
	if (y2 > Height) y2 = Height;

	if ((x1 == x2) || (y1 == y2)) return;

	AktIconBoxG(x1, y1, x2, y2);

	tmp = x2 - x1;
	int y;
	for (y = y1; y < y2; y++)
	{
		MemFill(BackBuf + x1 + y*WidthByte, tmp, PointCol);
	}
}


/////////////////////////////////////////////////////////////////////////////
// v�pl�

void _fastcall FFill1(BYTE* data, int x, int y)
{
// lok�ln� prom�nn�
	int min = x;				// minim�ln� sou�adnice X
	int max = x;				// maxim�ln� sou�adnice X
	int i;						// pracovn� ��ta�e
	int widthbyte = WidthByte; // d�lka linky
	int width = Width - 1;	// ���ka plochy - 1
	BYTE* data2;				// pracovn� ukazatel dat
	BYTE* datamin;				// minim�ln� ukazatel dat
	BYTE fill = PointFill;
	BYTE col = PointCol;

// nastaven� aktivn�ho bodu
	*data = col;

// v�pl� minim�ln�m sm�rem
	datamin = data;
	while ((min > 0) && (datamin[-1] == fill))
	{
		min--;
		datamin--;
		*datamin = col;
	}

// v�pl� maxim�ln�m sm�rem
	data2 = data;
	while ((max < width ) && (data2[1] == fill))
	{
		max++;
		data2++;
		*data2 = col;
	}

// aktualizace ikon
	AktIconBoxG(min, y, max, y);

// v�pl� sm�rem dol�
	if (y > 0)
	{
		data2 = datamin - widthbyte;
		y--;
		for (i = min; i <= max; i++)
		{
			if (*data2 == fill)
				FFill1(data2, i, y);
			data2++;
		}
		y++;
	}

// v�pl� sm�rem nahoru
	if (y < Height - 1)
	{
		data2 = datamin + widthbyte;
		y++;
		for (i = min; i <= max; i++)
		{
			if (*data2 == fill)
				FFill1(data2, i, y);
			data2++;
		}
	}	

}


/////////////////////////////////////////////////////////////////////////////
// v�pl�

void FFill()
{
// na�ten� barvy v�pln�
	BYTE col = (BYTE)FNumCol();
	PointCol = col;

// na�ten� sou�adnic v�pln�
	int x = FIntX();
	int y = FIntY();

	if (((DWORD)x >= (DWORD)Width) || ((DWORD)y >= (DWORD)Height))
		return;

	BYTE* data = BackBuf + x + y*WidthByte;
	BYTE fill = *data;
	PointFill = fill;

	if (fill == col) return;

	FFill1(data, x, y);
}


/////////////////////////////////////////////////////////////////////////////
// kru�nice

void FCircle()
{
// na�ten� barvy kru�nice
	int col = FNumCol();
	PointCol = (BYTE)col;

// na�ten� ���ky ��ry kru�nice
	double width = FNum() - 0.4;

// na�ten� sou�adnic st�edu kru�nice
	int x = FIntX();
	int y = FIntY();

// na�ten� polom�ru kru�nice
	double polomer = (double)(int)((FNum()*ICONWIDTH + 0.2)*2)/2;

// lok�ln� prom�nn�
	double polomer2;							// kvadr�t polom�ru
	int dx;										// vzd�lenost bodu X od st�edu
	int dy;										// vzd�lenost bodu Y od st�edu

// po��te�n� polom�r
	polomer -= width / 2;
	if (polomer < 0.5)
	{
		width += polomer - 0.5;
		polomer = 0.5;
	}

// aktualizace ikon
	AktIconBoxG(Round(x - polomer - width), Round(y - polomer - width), 
		Round(x + polomer + width), Round(y + polomer + width));

// cyklus p�es v�echny pr�m�ry
	for (; width > 0; width -= 0.7)
	{
		polomer2 = polomer*polomer;

// cyklus p�es v�echny body
		for (dx = (int)(polomer * 0.72 + 0.8); dx >= 0; dx--)
		{
			dy = (int)(sqrt(polomer2 - (double)dx*dx) + 0.5);

			FPoint01(x - dx, y + dy);
			FPoint01(x + dx, y + dy);
			FPoint01(x - dx, y - dy);
			FPoint01(x + dx, y - dy);

			FPoint01(x - dy, y + dx);
			FPoint01(x + dy, y + dx);
			FPoint01(x - dy, y - dx);
			FPoint01(x + dy, y - dx);
		}

		polomer += 0.7;
	}
}


/////////////////////////////////////////////////////////////////////////////
// kruh

void FCircleFill()
{
// na�ten� barvy kruhu
	int col = FNumCol();
	PointCol = (BYTE)col;

// na�ten� sou�adnic st�edu kruhu
	int x = FIntX();
	int y = FIntY();

// na�ten� pr�m�ru kruhu
	int prumer = FIntX2();
	if (prumer < 1) return;
	if (prumer > 20000) prumer = 20000;
	PointWidth = prumer;

// vykreslen� kruhu
	CirclePoint(x, y);
}


/////////////////////////////////////////////////////////////////////////////
// koule

void FSphere()
{
// na�ten� barvy koule
	BYTE col0 = (BYTE)FNumCol();

// p��prava po�tu barev
	int cols;
	if (col0 >= StdColors - 2*ColLev)
	{
		cols = StdColors - col0;
	}
	else
	{
		cols = (col0 - ResCols + ColLev)/ColLev*ColLev + ResCols - col0 + 1;
	}

// p��prava tabulky palet
	int i;
	BYTE col[20];
//	ASSERT(2*ColLev <= 20);
	for (i = 0; i < cols-1; i++)
	{
		col[i] = (BYTE)(col0+i);
	}
	col[cols-1] = (BYTE)(StdColors - 1);

// na�ten� sou�adnic st�edu koule
	int x = FIntX();
	int y = FIntY();

// na�ten� pr�m�ru koule
	int width = FIntX2();
	if (width < 1) return;
	if (width > 20000) width = 20000;
//	PointWidth = prumer;

	int sxcol = x - width/4;					// st�ed pro barvu X
	int sycol = y + width/4;					// st�ed pro barvu Y

// lok�ln� prom�nn�
	int dxcol2, dycol2;							// kvadr�ty vzd�lenosti pro barvu
	int prumer2 = width*width;			// kvadr�t pr�m�ru
	int dx2;							// kvadr�t vzd�lenosti od st�edu X * 2
	int dy2;							// kvadr�t vzd�lenosti od st�edu Y * 2
	int xx;								// ukazatel sou�adnice X
	int yy;								// ukazatel sou�adnice Y
	BYTE* data;							// ukazatel dat

// po��te�n� sou�adnice X
	int x1 = x - (width - 1) / 2;		// v�choz� sou�adnice X
	if (x1 < 0) x1 = 0;
			
// koncov� sou�adnice X - n�sleduj�c�
	int x2 = x + width / 2 + 1;			// koncov� sou�adnice X (n�sleduj�c�)
	if (x2 > Width) x2 = Width;

// po��te�n� sou�adnice Y
	int y1 = y - (width - 1) / 2;		// v�choz� sou�adnice Y
	if (y1 < 0) y1 = 0;
			
// koncov� sou�adnice Y - n�sleduj�c�
	int y2 = y + width / 2 + 1;			// koncov� sou�adnice Y - n�sleduj�c�
	if (y2 > Height) y2 = Height;

// aktualizace ikon
	AktIconBoxG(x1, y1, x2, y2);

// cyklus p�es v�echny linky
	for (yy = y1; yy < y2; yy++)		// cyklus p�es v�echny linky
	{	
		dy2 = (yy - y)*2;				// vzd�lenost od st�edu Y * 2
		dy2 *= dy2;						// kvadr�t vzd�lenosti Y
		dycol2 = (yy - sycol) * 2;		// vzd�lenost od st�edu Y * 2 pro barvu
		dycol2 *= dycol2;

// cyklus p�es v�echny pozice na lince
		data = BackBuf + x1 + yy*WidthByte;

		for (xx = x1; xx < x2; xx++)	// cyklus p�es v�echny body na lince
		{
			dx2 = (xx - x)*2;			// vzd�lenost od st�edu X * 2
			dx2 *= dx2;					// kvadr�t vzd�lenosti X

// kontrola, zda je platn� bod kruhu
			if (dx2 + dy2 <= prumer2)
			{

				dxcol2 = (xx - sxcol) * 2;	// vzd�lenost od st�edu X * 2 pro barvu
				dxcol2 *= dxcol2;

				i = (int)(cols * 2.2 * sqrt((double)(dxcol2 + dycol2)) / width);
							
				switch(i & 0x3)
				{
				case 0:
					i = i/4;
					break;
				case 1:
					i = i/4 + (xx & yy & 1);
					break;
				case 2:
					i = i/4 + ((xx + yy) & 1);
					break;

				default:
					i = i/4 + ((xx | yy) & 1);
				}
					if (i >= cols) i = cols-1;
				*data = col[i];
			}
			data++;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// vypln�n� troj�heln�k

void FTriangle()
{
// na�ten� barvy troj�heln�ku
	BYTE col = (BYTE)FNumCol();

// na�ten� sou�adnic troj�heln�ku
	int x1 = FIntX();
	int y1 = FIntY();
	int x2 = FIntX();
	int y2 = FIntY();
	int x3 = FIntX();
	int y3 = FIntY();

	int tmp;

// set��d�n� sou�adnic podle v��ky (1. bod nejni���, 3. bod nejvy���)
	if (y2 < y1)
	{
		tmp = x2; x2 = x1; x1 = tmp;
		tmp = y2; y2 = y1; y1 = tmp;
	}

	if (y3 < y1)
	{
		tmp = x3; x3 = x1; x1 = tmp;
		tmp = y3; y3 = y1; y1 = tmp;
	}
	
	if (y3 < y2)
	{
		tmp = x3; x3 = x2; x2 = tmp;
		tmp = y3; y3 = y2; y2 = tmp;
	}

// aktualizace ikon
	int xmin = x1;
	if (x2 < xmin) xmin = x2;
	if (x3 < xmin) xmin = x3;
	int xmax = x3;
	if (x1 > xmax) xmax = x1;
	if (x2 > xmax) xmax = x2;
	AktIconBoxG(xmin, y1, xmax, y3);

// cyklus p�es v�echny linky
	for (int y = y1; y <= y3; y++)
	{

// kontrola, zda je linka platn�
		if ((y >= 0) && (y < Height))
		{

// je �sek pod druh�m bodem
			if (y < y2)
			{

				xmin = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
				xmax = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
	
				if (xmax < xmin)
				{
					tmp = xmax;
					xmax = xmin;
					xmin = tmp;
				}
	
				if ((xmax >= 0) && (xmin < Width))
				{
					if(xmin < 0) xmin = 0;
					if (xmax >= Width) xmax = Width-1;
					MemFill(BackBuf + xmin + y*WidthByte, xmax-xmin+1, col);
				}
			}				// IF je �sek nad druh�m bodem

// je �sek nad druh�m bodem
			else			// test �seku pod druh�m bodem
			{
				if (y2 == y3)
				{
					xmin = x2;
					xmax = x3;
				}
				else
				{
					xmin = x2 + (x3 - x2) * (y - y2) / (y3 - y2);
					xmax = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
				}
	
				if (xmax < xmin)
				{
					tmp = xmax;
					xmax = xmin;
					xmin = tmp;
				}

				if ((xmax >= 0) && (xmin < Width))
				{
					if(xmin < 0) xmin = 0;
					if (xmax >= Width) xmax = Width-1;
					MemFill(BackBuf + xmin + y*WidthByte, xmax-xmin+1, col);
				}
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// zobrazen� textu

void FGraphText()
{
// na�ten� textu
	CString text;
	FText(text);

	int radku = text.LineNum();
	int radek = 0;

// na�ten� barvy
	BYTE col = (BYTE)FNumCol();

// sou�adnice textu
	int x = FIntX();
	int y = Height - FIntY();

// �hel sklonu
	double uhel = FNum();

// v��ka znak�
	int height = (int)fabs(FNum() * ICONHEIGHT);
	int height0 = height;
	if (height == 0) height0 = 16;

	double width = fabs(FNum());		// relativn� ���ka

	y -= (int)(height0*(cos(uhel)));
	x -= (int)(height0*(sin(uhel)));

// na�ten� p�ep�na��
	bool bold = FBool();						// BOLD
	bool italic = FBool();					// ITALIC
	bool underline = FBool();				// UNDERLINE
	bool strikeout = FBool();				// STRIKEOUT
	bool serif = FBool();					// SERIF
	bool courier = FBool();					// COURIER

// vytvo�en� bufferu bitmapy
	int widthbyte = ((Width + 15) & ~0xf)/8;
	MemBuf(TextBuf, widthbyte * Height);
	MemFill(TextBuf, widthbyte * Height, -1);

// vytvo�en� mono bitmapy
	HBITMAP bitmap = ::CreateBitmap(Width, Height, 1, 1, TextBuf);
	ASSERT(bitmap != NULL);
	if (bitmap == NULL) return;

// otev�en� DC displeje
	HDC dc = ::GetDC(MainFrame);
	ASSERT(dc != NULL);
	if (dc != NULL)
	{

// vytvo�en� kompatibiln�ho DC
		HDC dcTemp = ::CreateCompatibleDC(dc);
		ASSERT(dcTemp != NULL);
		if (dcTemp != NULL)
		{

// nastaven� barvy p�sma a pozad�
//			::SetTextColor(dcTemp, 0xffffff);
//			::SetBkColor(dcTemp, 0);
//			::SetBkMode(dcTemp, OPAQUE); //TRANSPARENT);
//			::SelectObject(dcTemp, ::GetStockObject(BLACK_BRUSH));
//			::SetTextAlign(dcTemp, TA_BASELINE | TA_NOUPDATECP);

// v�b�r bitmapy do DC
			HBITMAP oldbitmap = (HBITMAP)::SelectObject(dcTemp, bitmap);

// vytvo�en� fontu
			HFONT font;
			if ((height0 == 16) &&
				(uhel == 0) &&
				!italic &&
				!underline &&
				!strikeout &&
				!serif &&
				(width == 1))
			{
				if (bold)
				{
					if (courier)
					{
						height0 = 15;
						y++;
					}
				}
				else
				{
					if (courier)
					{
						height0 = 10;
						y += 6;
					}
					else
					{
						height0 = 13;
						y += 3;
					}
				}
			}

			font = GetFont(
				bold,
				italic,
				underline,
				strikeout,
				serif,
				courier,
				height,
				width,
				uhel);

// v�b�r fontu
			HFONT oldfont = (HFONT) ::SelectObject(dcTemp, font);

// v�pis textu
			CString txt;
			while (radek < radku)
			{
				txt = text.GetLine(radek);
				int korx = 0;
				if (GraphAreaWidth > 0)
				{
					SIZE size;
					size.cx = 0;
					if (::GetTextExtentPoint32(dcTemp, txt, txt.Length(), &size))
					{
						korx = (GraphAreaWidth - size.cx)/2;
						if (korx < 0) korx = 0;
					}
				}

				::TextOut(dcTemp, x+korx, y, txt, txt.Length());
				radek++;
				y += height0;
			}

// navr�cen� p�vodn�ho fontu
			::SelectObject(dcTemp, oldfont);
			FreeFont(font);

// navr�cen� p�vodn� bitmapy
			::SelectObject(dcTemp, oldbitmap);

// na�ten� dat zp�t do bufferu
			struct {BITMAPINFOHEADER bmiHeader; RGBQUAD bmiColors[2]; } bmpinfo;
			bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmpinfo.bmiHeader.biWidth = Width;
			bmpinfo.bmiHeader.biHeight = Height;
			bmpinfo.bmiHeader.biPlanes = 1;
			bmpinfo.bmiHeader.biBitCount = 1;
			bmpinfo.bmiHeader.biCompression = BI_RGB;
			bmpinfo.bmiHeader.biSizeImage = 0;
			bmpinfo.bmiHeader.biXPelsPerMeter = 0;
			bmpinfo.bmiHeader.biYPelsPerMeter = 0;
			bmpinfo.bmiHeader.biClrUsed = 0;
			bmpinfo.bmiHeader.biClrImportant = 0;
			::GetDIBits(dcTemp, bitmap, 0, Height, TextBuf, (BITMAPINFO*) &bmpinfo, DIB_RGB_COLORS);

// p�enesen� dat do obr�zku
			char* src = (char*)TextBuf;
			BYTE* dst = BackBuf;
			char data;
			for (int i = widthbyte * Height-1; i >= 0; i--)
			{
				data = *src;
				src++;
				
				if (data >= 0)
				{
					*dst = col;
				}
				dst++;
				data <<= 1;

				if (data >= 0)
				{
					*dst = col;
				}
				dst++;
				data <<= 1;

				if (data >= 0)
				{
					*dst = col;
				}
				dst++;
				data <<= 1;

				if (data >= 0)
				{
					*dst = col;
				}
				dst++;
				data <<= 1;

				if (data >= 0)
				{
					*dst = col;
				}
				dst++;
				data <<= 1;

				if (data >= 0)
				{
					*dst = col;
				}
				dst++;
				data <<= 1;

				if (data >= 0)
				{
					*dst = col;
				}
				dst++;
				data <<= 1;

				if (data >= 0)
				{
					*dst = col;
				}
				dst++;
			}

// aktualizace v�ech ikon
			AktIconBox(0, 0, WidthN-1, HeightN-1);

// zru�en� pracovn�ho DC
			::DeleteDC(dcTemp);
		}

// uvoln�n� DC
		::ReleaseDC(0, dc);
	}

// zru�en� bitmapy
	::DeleteObject(bitmap);
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� ���ky pole k zobrazen� centrovan�ho textu

void FSetGraphAreaWidth()
{
	GraphAreaWidth = FIntX();
}


/////////////////////////////////////////////////////////////////////////////
// zobrazen� obr�zku

void FPicDisp()
{
// na�ten� obr�zku k zobrazen�
	CPicture pic;
	FPicture(pic);

// na�ten� ostatn�ch parametr�
	int x = FIntX();				// sou�adnice X
	int y = FIntY();				// sou�adnice Y
	int x22 = FIntX();				// po��tek v��ezu X
	int y22 = FIntY();				// po��tek v��ezu Y
	int width22 = FIntX();			// ���ka
	int height22 = FIntY();			// v��ka
	int width = pic.Width();		// ���ka obr�zku
	int height = pic.Height();		// v��ka obr�zku
	int widthbyte = pic.Width(); // d�lka linky v bajtech

// korekce a kontrola v��ezu
	if (x22 < 0) x22 = 0;
	if (x22 > width) x22 = width;
	if (y22 < 0) y22 = 0;
	if (y22 > height) y22 = height;
	if ((x22 + width22) > width) width22 = width - x22;
	if ((y22 + height22) > height) height22 = height - y22;
	if ((width22 <= 0) || (height22 <= 0)) return;

// p��prava sou�adnic
	int x1 = x;
	int x2 = x + width22;
	int y1 = y;
	int y2 = y + height22;

// zdrojov� adresa
	pic.DeComp();
	BYTE* src = pic.DataData() + y22*widthbyte + x22;		// zdrojov� adresa

// omezen� po��tku p�i podte�en�
	if (x1 < 0)
	{
		src -= x1;
		x1 = 0;
	}

	if (y1 < 0)	
	{
		src -= y1*width;
		y1 = 0;
	}

// omezen� ���ky a v��ky k zobrazen� p�i p�ete�en� konce
	if (x2 > Width) x2 = Width;
	if (y2 > Height) y2 = Height;

// kontrola, zda je co zobrazit
	if ((x1 >= x2) || (y1 >= y2)) return;

// zaji�t�n� zmapov�n� obr�zku
	if (pic.Param() == PicParamNone)
	{
		bool backcol = false;
		bool piccol = false;

		BYTE* data = pic.DataData();
		int width = pic.Width();
//		int datainc = pic.WidthByte() - width;

		for (int m = pic.Height(); m > 0; m--)
		{
			for (int n = width; n > 0; n--)
			{
				if (*data == BackCol)
				{
					backcol = true;
				}
				else
				{
					if (*data == ShadCol) backcol = true;
					piccol = true;
				}
				data++;
			}
//			data += datainc;

			if (piccol && backcol) break;
		}

		int param = PicParamBack;
		if (piccol)
		{
			if (backcol)
			{
				param = PicParamMix;
			}
			else
			{
				param = PicParamPic;
			}
		}
		pic.Param(param);
	}

// c�lov� adresa
	BYTE* dst = BackBuf + x1 + y1*WidthByte; // c�lov� adresa

// p��prava p��rustku adres
	int dstinc = WidthByte;	// p��rustek c�lov� adresy
	int srcinc = widthbyte;				// p��rustek zdrojov� adresy
	width = x2 - x1;				// ���ka k zobrazen�

// rozli�en� typu obr�zku
	switch (pic.Param())
	{

// pouze obr�zek bez pozad�
	case PicParamPic:
		for (y = y2 - y1; y > 0; y--)
		{
			MemCopy(dst, src, width);
			src += srcinc;
			dst += dstinc;
		}
		break;

// mixovan� obr�zek s pozad�m
	case PicParamMix:
		dstinc -= width;
		srcinc -= width;

		for (y = y2 - y1; y > 0; y--)
		{
			for (x = width; x > 0; x--)
			{
				BYTE c = *src;
				if (c != BackCol)
				{
					if (c == ShadCol) c = Shadows[*dst];
					*dst = c;
				}
				src++;
				dst++;
			}
			src += srcinc;
			dst += dstinc;
		}
		break;
	}

// aktualizace ikon
	AktIconBoxG(x1, y1, x2, y2);
}

/////////////////////////////////////////////////////////////////////////////
// zpr�hledn�n�

void FTransparent()
{
// v�po�et zdrojov�ch sou�adnic
	POINT p;
	p.x = DispLeft + Round(Width*(Meritko-1)/2);
	p.y = DispTop + Round(Height*(Meritko-1)/2);
	::ClientToScreen(MainFrame, &p);
	int x = p.x;
	int y = p.y;

// vypnut� okna
	if (MainFrameVisible)
	{
		::ShowWindow(MainFrame, SW_HIDE);
		TimeWait(9);
	}

// otev�en� DC obrazovky - zdroj
	HDC hdcScreen = ::CreateDC(_T("DISPLAY"),NULL, NULL, NULL);
	if (hdcScreen)
	{
// na�ten� rozm�r� displeje
		int horzres = ::GetDeviceCaps(hdcScreen, HORZRES);
		int vertres = ::GetDeviceCaps(hdcScreen, VERTRES);

// nastaven� vlastn�ch palet pro zdroj
		HPALETTE OldPal = ::SelectPalette(hdcScreen, StdPalette, FALSE);
		::RealizePalette(hdcScreen);

// vytvo�en� pam�ov�ho DC pro �schovu obrazovky - c�l
		HDC hdcCompat = ::CreateCompatibleDC(hdcScreen);
		if (hdcCompat)
		{

// nastaven� vlastn�ch palet pro c�l
			HPALETTE OldPal2 = ::SelectPalette(hdcCompat, StdPalette, FALSE);
			::RealizePalette(hdcCompat);

// vytvo�en� bitmapy pro pam�ov� DC c�le
			HBITMAP hBitmap = ::CreateCompatibleBitmap(hdcScreen, Width, Height);
			if (hBitmap)
			{

// v�b�r bitmapy do pam�ov�ho DC c�le
				HBITMAP oldBitmap = (HBITMAP) ::SelectObject(hdcCompat, hBitmap);

// inicializa�n� vymaz�n� plochy
				RECT rc;
				rc.left = 0;
				rc.top = 0;
				rc.right = Width;
				rc.bottom = Height;
				::FillRect(hdcCompat, &rc, (HBRUSH)::GetStockObject(BLACK_BRUSH));

// omezen� zdrojov�ch sou�adnic
				int x0 = 0;
				int y0 = 0;
				int width0 = Width;
				int height0 = Height;

				if (x < 0)
				{
					x0 = -x;
					width0 -= x0;
					x = 0;
				}

				if (x + width0 > horzres)
				{
					width0 = horzres - x;
				}

				if (y < 0)
				{
					y0 = -y;
					height0 -= y0;
					y = 0;
				}

				if (y + height0 > vertres)
				{
					height0 = vertres - y;
				}

// p�enesen� v��ezu do bitmapy
				::BitBlt(hdcCompat, x0, y0, width0, height0, hdcScreen, x, y, SRCCOPY);

// vytvo�en� bufferu k na�ten� dat bitmapy
				BITMAPINFO* bmi = (BITMAPINFO*)MemGet(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256);

				int widthbyte = ((3*Width + 3) & ~3);

				MemFill(bmi, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256, 0);
				bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
				bmi->bmiHeader.biWidth = Width;
				bmi->bmiHeader.biHeight = Height;
				bmi->bmiHeader.biPlanes = 1;
				bmi->bmiHeader.biBitCount = 24;
				bmi->bmiHeader.biCompression = BI_RGB;
				bmi->bmiHeader.biSizeImage = widthbyte * Height;

				BYTE* data = (BYTE*)::MemGet((widthbyte+15) * (Height+2));

// na�ten� dat bitmapy
				if (::GetDIBits(hdcCompat, hBitmap, 0, Height, data, bmi, DIB_RGB_COLORS) == Height)
				{

// p�enesen� dat
					BYTE* dst = BackBuf;
					BYTE* src = data;
					widthbyte -= 3*Width;

					for (int i = Height; i > 0; i--)
					{
						for (int j = Width; j > 0; j--)
						{
							*dst = PalImport(src[2], src[1], src[0]);
							dst++;
							src += 3;
						}
						src += widthbyte;
					}
				}


// uvoln�n� buffer�
				MemFree(data);
				MemFree(bmi);

// n�vrat p�vodn� bitmapy
				::SelectObject(hdcCompat, oldBitmap);

// zru�en� bitmapy
				::DeleteObject(hBitmap);
			}

// n�vrat p�vodn�ch palet c�le
			::SelectPalette(hdcCompat, OldPal2, FALSE);

// zru�en� pam�ov�ho DC
			::DeleteDC(hdcCompat);
		}

// n�vrat p�vodn�ch palet zdroje
		::SelectPalette(hdcScreen, OldPal, FALSE);

// zru�en� DC displeje
		::DeleteDC(hdcScreen);
	}

// aktualizace ikon
	AktAllIcon();

// zapnut� okna
	if (MainFrameVisible)
	{
		::ShowWindow(MainFrame, SW_SHOW);
	}
}

/***************************************************************************\
*																			*
*								obsluha kl�vesnice							*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// �ek�n� na kl�vesu

void FComVKeyWait()
{
// �ek�n� na vstup znaku z kl�vesnice (nebo na p�eru�en�)
	while ((Break == 0) && 
			(KeyBufRead == KeyBufWrite) && 
			(!ConsoleOn))
	{
#ifndef _MINI
		D3Render(true);
#endif // _MINI
		TimeWait(1);
	}

// kontrola, zda byla p�ijata kl�vesa (zda to nebylo jen p�eru�en�)
	if (KeyBufRead != KeyBufWrite)
	{

// zv��en� �tec� adresy
		KeyBufRead++;
		if (KeyBufRead >= KEYBUFSIZE) KeyBufRead = 0;
	}
}


/////////////////////////////////////////////////////////////////////////////
// �ek�n� na snak z kl�vesnice

void FComCharWait()
{
// �ek�n� na vstup znaku z kl�vesnice (nebo na p�eru�en�)
	while ((Break == 0) && 
			(CharBufRead == CharBufWrite) && 
			(!ConsoleOn))
	{
#ifndef _MINI
		D3Render(true);
#endif // _MINI
		TimeWait(1);
	}

// kontrola, zda byla p�ijata kl�vesa (zda to nebylo jen p�eru�en�)
	if (CharBufRead != CharBufWrite)
	{

// zv��en� �tec� adresy
		CharBufRead++;
		if (CharBufRead >= KEYBUFSIZE) CharBufRead = 0;
	}
}


/////////////////////////////////////////////////////////////////////////////
// vypr�zdn�n� bufferu kl�ves

void FKlavFlush()
{
	KeyBufRead = KeyBufWrite;
	CharBufRead = CharBufWrite;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� p�ep�na�e Num Lock

void FSetNumLock()
{
	NumLock = FBool();
	AktNumLock = true;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� p�ep�na�e Caps Lock

void FSetCapsLock()
{
	CapsLock = FBool();
	AktCapsLock = true;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� p�ep�na�e Scroll Lock

void FSetScrollLock()
{
	ScrollLock = FBool();
	AktScrollLock = true;
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� p�ep�na�e Insert

void FSetInsertLock()
{
	InsertLock = FBool();
	AktInsertLock = true;
}



/////////////////////////////////////////////////////////////////////////////
// hromadn� posun sprajt� ve sm�ru X

void FSpriteShiftX()
{
// na�ten� po�adovan�ho posunu
	double dx = FNum();
	if (dx == 0) return;

// posun v�ech sprajt�
	CSprite* sprite = Sprite.Data();

	for (int i = Sprite.Num()-1; i >= 0; i--)
	{
		sprite->CilX(sprite->CilX() + dx);
		sprite->AktX(sprite->AktX() + dx);
		sprite->X(Round(sprite->AktX() * ICONWIDTH));
		sprite++;
	}

// po�adavek aktualizace v�ech ikon
	AktAllIcon();
}


/////////////////////////////////////////////////////////////////////////////
// hromadn� posun sprajt� ve sm�ru Y

void FSpriteShiftY()
{
// na�ten� po�adovan�ho posunu
	double dy = FNum();
	if (dy == 0) return;

// posun v�ech sprajt�
	CSprite* sprite = Sprite.Data();

	for (int i = Sprite.Num()-1; i >= 0; i--)
	{
		sprite->CilY(sprite->CilY() + dy);
		sprite->AktY(sprite->AktY() + dy);
		sprite->Y(Round(sprite->AktY() * ICONHEIGHT));
		sprite++;
	}

// po�adavek aktualizace v�ech ikon
	AktAllIcon();
}


/////////////////////////////////////////////////////////////////////////////
// okno k zobrazen� sprajt�

void FSpriteWinX1()
{
	int x = FIntX();
	if (x < -1000000 * ICONWIDTH) x = -1000000 * ICONWIDTH;
	if (x > 1000000 * ICONWIDTH) x = 1000000 * ICONWIDTH;
	if (x != SpriteWinX1)
	{
		SpriteWinX1 = x;
		AktAllIcon();
	}
}


void FSpriteWinY1()
{
	int y = FIntY();
	if (y < -1000000 * ICONWIDTH) y = -1000000 * ICONWIDTH;
	if (y > 1000000 * ICONHEIGHT) y = 1000000 * ICONHEIGHT;
	if (y != SpriteWinY1)
	{
		SpriteWinY1 = y;
		AktAllIcon();
	}
}


void FSpriteWinX2()
{
	int x = FIntX();
	if (x < -1000000 * ICONWIDTH) x = -1000000 * ICONWIDTH;
	if (x > 1000000 * ICONWIDTH) x = 1000000 * ICONWIDTH;
	if (x != SpriteWinX2)
	{
		SpriteWinX2 = x;
		AktAllIcon();
	}
}


void FSpriteWinY2()
{
	int y = FIntY();
	if (y < -1000000 * ICONWIDTH) y = -1000000 * ICONWIDTH;
	if (y > 1000000 * ICONHEIGHT) y = 1000000 * ICONHEIGHT;
	if (y != SpriteWinY2)
	{
		SpriteWinY2 = y;
		AktAllIcon();
	}
}


/***************************************************************************\
*																			*
*									konzola									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// re�im konzoly

void FConsoleOn() { ConsoleOnSet(!ConsoleOn); }


/////////////////////////////////////////////////////////////////////////////
// v�stup textu na konzolu

void FConsoleOut()
{
	CString text;
	FText(text);
	ConsoleOut(text);
}


/////////////////////////////////////////////////////////////////////////////
// v�stup chybov�ho textu na konzolu

void FConsoleErr()
{
	CString text;
	FText(text);
	ConsoleErr(text);
}


/////////////////////////////////////////////////////////////////////////////
// n�vratov� k�d

void FSetReturnCode()
{
	__int64 cislo = (__int64)(FNum() + 0.5);
	if (cislo < 0) cislo = 0;
	if (cislo > 0xffffffff) cislo = 0xffffffff;
	ReturnCode = (DWORD)cislo;
}


/////////////////////////////////////////////////////////////////////////////
// spu�t�n� programu

void FExec()
{
	CString text;
	FText(text);
	Exec(text, FALSE);
	SetConsoleText();
}

void FExecWait()
{
	CString text;
	FText(text);
	Exec(text, TRUE);
	SetConsoleText();
}


/***************************************************************************\
*																			*
*									dialogy									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// ��slo prvku

void FWindowID()
{
	int id = FIntR();
	if (Win.IsValid(id))
	{
		WindowID = id;
	}
	else
	{
		WindowID = -1;
	}
}


/////////////////////////////////////////////////////////////////////////////
// zru�en� okna

void FWindowDel()
{
	int inx = FIntR();
	WindowDel(inx);
}

int RichEditData = 0;
CString RichEditText;
CString RichEditIdent(_T("{\\RTF"));

DWORD CALLBACK EditStreamCallback(DWORD cookie, BYTE* buf, long num, long* bytes)
{
	int n = RichEditText.Length()*sizeof(TCHAR) - RichEditData;
	if (n > num) n = num;
	if (n < 0) n = 0;
	*bytes = n;
	MemCopy(buf, RichEditText.DataData() + RichEditData/sizeof(TCHAR), n);
	RichEditData += n;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// zv��en� velikosti tabulky

void SetTabSize(WINITEM* item, int rows)
{
	if (rows >= 10000000) return;

	int rows2 = item->TabRows;

	LVITEM lvi;

	while (rows > rows2)
	{
		int i = rows2;
		rows2++;
		item->TabRows = rows2;

		int cols = item->TabCols;

		if (item->TabDataMax < (rows2 * cols))
		{
			item->TabDataMax = (2*item->TabDataMax);
			item->TabData = (CString*)MemSize(item->TabData, item->TabDataMax * sizeof(CString));
		}

		for (int j = (i * cols); j < (rows2 * cols); j++)
		{
			item->TabData[j].Init();
		}

		MemFill(&lvi, sizeof(lvi), 0);
		lvi.mask = LVIF_TEXT | LVIF_NORECOMPUTE;
		lvi.pszText = LPSTR_TEXTCALLBACK;
		lvi.iItem = i;
		lvi.iSubItem = 0;

		::SendMessage(item->Wnd, LVM_INSERTITEM, i, (LPARAM)&lvi);
	}
}

/////////////////////////////////////////////////////////////////////////////
// text okna

void FWindowText()
{
// na�ten� textu pro okno
	CString text;
	FText(text);

// adresa definice prvku
	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];

// aktualizace textu
		WindowTextValidate();

// test, zda se text li��
		if (text != item->Text)
		{
			ProgramFill = true;

// aktualizace textu pro hlavn� okno
			if (WindowID == 0)
			{
				CaptionText = text;
				CaptionAkt = true;
				ReDisp();
			}
			else
			{
				item->Text = text;

				bool newtextvalid = true;		// RichEdit to vyp�n� pro PLANE text

				switch (item->Typ)
				{
// nastaven� textu pro COMBOEDIT
				case WINITEM_COMBOEDIT:
				case WINITEM_LISTEDIT:
					{
						int selstart;
						::SendMessage(item->Wnd, EM_GETSEL, (WPARAM)&selstart, NULL);
						if (selstart > text.Length()) selstart = text.Length();
						::SendMessage(item->Wnd, CB_SELECTSTRING, (WPARAM)-1, (LPARAM)(LPCTSTR)text);
						text.SetWindowText(item->Wnd);
						::SendMessage(item->Wnd, EM_SETSEL, selstart, selstart);
					}
					break;

// nastaven� textu pro textov� pole (s �schovou kurzoru)
				case WINITEM_EDIT:
					{
						int selstart;
						::SendMessage(item->Wnd, EM_GETSEL, (WPARAM)&selstart, NULL);
						if (selstart > text.Length()) selstart = text.Length();
						text.SetWindowText(item->Wnd);
						::SendMessage(item->Wnd, EM_SETSEL, selstart, selstart);
						::SendMessage(item->Wnd, EM_SCROLLCARET, 0, 0);
					}
					break;

// nastaven� textu pro textov� pole (s �schovou kurzoru)
				case WINITEM_MEMO:
					{
						// vypnut� p�ekreslov�n� okna
						::SendMessage(item->Wnd, WM_SETREDRAW, FALSE, 0);

						// �schova pozice kurzoru
						int selstart;
						::SendMessage(item->Wnd, EM_GETSEL, (WPARAM)&selstart, NULL);
						if (selstart > text.Length()) selstart = text.Length();

						// �schova po��tku textu
						int first = ::SendMessage(item->Wnd, EM_GETFIRSTVISIBLELINE, 0, 0);

						// nastaven� nov�ho textu
						text.SetWindowText(item->Wnd);

						// n�vrat p�vodn� pozice kurzoru
						::SendMessage(item->Wnd, EM_SETSEL, selstart, selstart);

						// rolov�n� kurzoru do okna
						::SendMessage(item->Wnd, EM_SCROLLCARET, 0, 0);

						// n�vrat p�vodn�ho po��tku textu
						first -= ::SendMessage(item->Wnd, EM_GETFIRSTVISIBLELINE, 0, 0);
						if (first > 0)
						{
							::SendMessage(item->Wnd, EM_LINESCROLL, 0, first);
						}

						// obnoven� p�ekreslov�n� okna
						::SendMessage(item->Wnd, WM_SETREDRAW, TRUE, 0);
						::InvalidateRect(item->Wnd, NULL, TRUE);
					}
					break;

// nastaven� textu pro textov� pole Richedit (s �schovou kurzoru)
				case WINITEM_RICHMEMO:
				case WINITEM_RICHEDIT:
					{
						// vypnut� p�ekreslov�n� okna
						::SendMessage(item->Wnd, WM_SETREDRAW, FALSE, 0);

						// �schova pozice kurzoru
						CHARRANGE sel;
						::SendMessage(item->Wnd, EM_EXGETSEL, 0, (LPARAM)&sel);
						if (sel.cpMin > text.Length()) sel.cpMin = text.Length();
						sel.cpMax = sel.cpMin;

						// �schova po��tku textu
						int first = ::SendMessage(item->Wnd, EM_GETFIRSTVISIBLELINE, 0, 0);

						// p��prava textov�ho proudu
						RichEditText = text;
						RichEditData = 0;
						EDITSTREAM stream;
						stream.pfnCallback = EditStreamCallback;

						// test, zda je to form�t RichEdit
						CString txt0 = text.Left(RichEditIdent.Length());
						txt0.UpperCase();
						newtextvalid = (item->Typ == WINITEM_RICHEDIT) && (txt0 == RichEditIdent);

						// odesl�n� textov�ho proudu
						::SendMessage(item->Wnd, EM_STREAMIN, (newtextvalid) ? SF_RTF : SF_TEXT, (LPARAM)&stream);

						// n�vrat p�vodn� pozice kurzoru
						::SendMessage(item->Wnd, EM_EXSETSEL, 0, (LPARAM)&sel);

						// rolov�n� kurzoru do okna
						::SendMessage(item->Wnd, EM_SCROLLCARET, 0, 0);

						// n�vrat p�vodn�ho po��tku textu
						first -= ::SendMessage(item->Wnd, EM_GETFIRSTVISIBLELINE, 0, 0);
						if (first > 0)
						{
							::SendMessage(item->Wnd, EM_LINESCROLL, 0, first);
						}

						// obnoven� p�ekreslov�n� okna
						::SendMessage(item->Wnd, WM_SETREDRAW, TRUE, 0);
						::InvalidateRect(item->Wnd, NULL, TRUE);
					}
					break;

// nastaven� textu pro COMBOBOX	
				case WINITEM_COMBOBOX:
					::SendMessage(item->Wnd, CB_SELECTSTRING, (WPARAM)-1, (LPARAM)(LPCTSTR)text);
					break;

// nastaven� textu pro LISTBOX	
				case WINITEM_LISTBOX:
					::SendMessage(item->Wnd, LB_SELECTSTRING, (WPARAM)-1, (LPARAM)(LPCTSTR)text);
					break;

// pro obr�zek se nic nenastav�
				case WINITEM_WINICON:
				case WINITEM_WINPIC:
					break;

// polo�ka tabulky
				case WINITEM_TABLE:
					{
						int row = item->TabRow;

						SetTabSize(item, row+1);

						int rows = item->TabRows;

						int col = item->TabCol;
						int cols = item->TabCols;

						if (((DWORD)col < (DWORD)cols) &&
							((DWORD)row < (DWORD)rows))
						{
							item->TabData[row * cols + col] = item->Text;

							LVITEM lvi;
							lvi.mask = LVIF_TEXT | LVIF_NORECOMPUTE;
							lvi.pszText = LPSTR_TEXTCALLBACK;
							lvi.iItem = row;
							lvi.iSubItem = col;

							::SendMessage(item->Wnd, LVM_SETITEMTEXT, row, (LPARAM)&lvi);
						}
						else
						{
							item->Text.Empty();
						}
					}
					break;

// nastaven� textu prvku
				default:
					text.SetWindowText(item->Wnd);
				}
				item->TextValid = newtextvalid;
			}

			ProgramFill = false;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// pozice okna

void _fastcall WindowMove(WINITEM* item)
{
	::SetWindowPos(item->Wnd, NULL, item->Left, item->Top, item->Width, item->Height,
		SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER | SWP_NOZORDER);

	if ((item->Typ == WINITEM_WINDOW) || (item->Typ == WINITEM_WINDOWRO)) InitClientRect(item);
}

void FWindowX()
{
	int x = FIntX();
	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];
		if (x != item->Left)
		{
			item->Left = x;
			WindowMove(item);
		}
	}
}

void FWindowY()
{
	int y = FIntY();
	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];

		int height;
		if ((item->Typ == WINITEM_WINDOW) || (item->Typ == WINITEM_WINDOWRO))
		{
			height = ScreenHeight0;
		}
		else
		{
			height = Win[item->Parent].ClientHeight;
		}

		y = height - item->Height - y;

		if (y != item->Top)
		{
			item->Top = y;
			WindowMove(item);
		}
	}
}

void FWindowW()
{
	int w = FIntX();
	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];
		w -= item->Width;
		if (w != 0)
		{
			item->Width += w;
			item->ClientWidth += w;
			WindowMove(item);
		}
	}
}

void FWindowH()
{
	int h = FIntY();
	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];
		h -= item->Height;
		if (h != 0)
		{
			item->Height += h;
			item->ClientHeight += h;
			item->Top -= h;
			WindowMove(item);
		}
	}
}

void FWindowWInt()
{
	int w = FIntX();
	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];
		w -= item->ClientWidth;
		if (w != 0)
		{
			item->Width += w;
			item->ClientWidth += w;
			WindowMove(item);
		}
	}
}

void FWindowHInt()
{
	int h = FIntY();
	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];
		h -= item->ClientHeight;
		if (h != 0)
		{
			item->Height += h;
			item->ClientHeight += h;
			item->Top -= h;
			WindowMove(item);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� pozice v prvku okna

void FSetWindowPos()
{
	double pos = FNum();

	ProgramFill = true;

	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];

		switch (item->Typ)
		{
		case WINITEM_HTRACKBAR:
			{
				int n = Round(pos*100);
				if (n < 0) n = 0;
				if (n > 100) n = 100;
				::SendMessage(item->Wnd, TBM_SETPOS, TRUE, n);
			}
			break;

		case WINITEM_VTRACKBAR:
			{
				int n = Round(pos*100);
				if (n < 0) n = 0;
				if (n > 100) n = 100;
				::SendMessage(item->Wnd, TBM_SETPOS, TRUE, 100-n);
			}
			break;

		case WINITEM_PROGRESS:
			{
				int n = Round(pos*100);
				if (n < 0) n = 0;
				if (n > 100) n = 100;
				::SendMessage(item->Wnd, PBM_SETPOS, n, 0);
			}
			break;

		case WINITEM_LISTBOX:
			{
				int n = Round(pos);
				if (n != (int)::SendMessage(item->Wnd, LB_GETCURSEL, 0, 0))
				{
					::SendMessage(item->Wnd, LB_SETCURSEL, n, 0);
				}
			}
			break;

		case WINITEM_TABS:
			{
				int n = Round(pos);
				if (n != (int)::SendMessage(item->Wnd, TCM_GETCURSEL, 0, 0))
				{
					::SendMessage(item->Wnd, TCM_SETCURSEL, n, 0);
				}
			}
			break;

		case WINITEM_LISTEDIT:
		case WINITEM_COMBOBOX:
		case WINITEM_COMBOEDIT:
			{
				int n = Round(pos);
				if (n != (int)::SendMessage(item->Wnd, CB_GETCURSEL, 0, 0))
				{
					::SendMessage(item->Wnd, CB_SETCURSEL, n, 0);
				}
			}
			break;

		case WINITEM_EDIT:
			{
				int n = Round(pos);
				::SendMessage(item->Wnd, EM_SETSEL, n, n);
				::SendMessage(item->Wnd, EM_SCROLLCARET, 0, 0);
			}
			break;

		case WINITEM_MEMO:
			{
				int sel;
				::SendMessage(item->Wnd, EM_GETSEL, (WPARAM)&sel, 0);
				int radek = ::SendMessage(item->Wnd, EM_LINEFROMCHAR, sel, 0);
				int pozice = sel - ::SendMessage(item->Wnd, EM_LINEINDEX, radek, 0);

				int n = Round(pos);
				int n2 = ::SendMessage(item->Wnd, EM_LINEINDEX, n, 0) + pozice;
				::SendMessage(item->Wnd, EM_SETSEL, n2, n2);
				::SendMessage(item->Wnd, EM_SCROLLCARET, 0, 0);
			}
			break;

		case WINITEM_RICHEDIT:
		case WINITEM_RICHMEMO:
			{
				CHARRANGE sel;
				::SendMessage(item->Wnd, EM_EXGETSEL, 0, (LPARAM)&sel);
				int radek = ::SendMessage(item->Wnd, EM_LINEFROMCHAR, sel.cpMin, 0);
				int pozice = sel.cpMin - ::SendMessage(item->Wnd, EM_LINEINDEX, radek, 0);

				int n = Round(pos);
				int n2 = ::SendMessage(item->Wnd, EM_LINEINDEX, n, 0) + pozice;

				CHARRANGE cr;
				cr.cpMax = n2;
				cr.cpMin = n2;

				::SendMessage(item->Wnd, EM_EXSETSEL, 0, (LPARAM)&cr);
				::SendMessage(item->Wnd, EM_SCROLLCARET, 0, 0);
			}
			break;

		case WINITEM_TABLE:
			{
				int row = Round(pos);
				BOOL colOK = (((DWORD)item->TabCol < (DWORD)item->TabCols));

				if ((row != item->TabRow) || (colOK && (row != item->TabRowF)))
				{
					item->TabRow = row;
					item->TextValid = false;

					if (colOK)
					{
						item->TabRowF = row;

						SetTabSize(item, row+1);

						LVITEM lvi;

						lvi.stateMask = LVIS_SELECTED;
						lvi.state = LVIS_SELECTED;

						if (AppActive)
						{
							lvi.stateMask = LVIS_SELECTED | LVIS_FOCUSED;
							lvi.state = LVIS_SELECTED | LVIS_FOCUSED;
						}

						if ((DWORD)row < (DWORD)item->TabRows)
						{
							::SendMessage(item->Wnd, LVM_SETITEMSTATE, row, (LPARAM)&lvi);

							if (item->Show)
							{
								::SendMessage(item->Wnd, LVM_ENSUREVISIBLE, row, FALSE);
							}
						}
						else
						{
							lvi.state = 0;
							::SendMessage(item->Wnd, LVM_SETITEMSTATE, (WPARAM)-1, (LPARAM)&lvi);
						}
					}
				}
			}
			break;

		case WINITEM_HSCROLLBAR:
		case WINITEM_VSCROLLBAR:
			{
				if (pos < 0) pos = 0;
				if (pos > 1) pos = 1;
				if (pos != item->ScrollPos)
				{
					item->ScrollPos = pos;

					::SetScrollPos(item->Wnd, SB_CTL, Round(item->ScrollMax * pos), TRUE);
				}
			}
			break;
		}	 
	}

	ProgramFill = false;
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� pozice 2 v prvku okna

void FSetWindowPos2()
{
	double pos = FNum();

	ProgramFill = true;

	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];

		switch (item->Typ)
		{
		case WINITEM_MEMO:
			{
				int sel;
				::SendMessage(item->Wnd, EM_GETSEL, (WPARAM)&sel, 0);
				int radek = ::SendMessage(item->Wnd, EM_LINEFROMCHAR, sel, 0);
				int radek2 = ::SendMessage(item->Wnd, EM_LINEINDEX, radek, 0);

				int n = Round(pos) + radek2;
				::SendMessage(item->Wnd, EM_SETSEL, n, n);
				::SendMessage(item->Wnd, EM_SCROLLCARET, 0, 0);
			}
			break;

		case WINITEM_RICHEDIT:
		case WINITEM_RICHMEMO:
			{
				CHARRANGE sel;
				::SendMessage(item->Wnd, EM_EXGETSEL, 0, (LPARAM)&sel);
				int radek = ::SendMessage(item->Wnd, EM_LINEFROMCHAR, sel.cpMin, 0);
				int radek2 = ::SendMessage(item->Wnd, EM_LINEINDEX, radek, 0);

				int n = Round(pos) + radek2;

				sel.cpMax = n;
				sel.cpMin = n;

				::SendMessage(item->Wnd, EM_EXSETSEL, 0, (LPARAM)&sel);
				::SendMessage(item->Wnd, EM_SCROLLCARET, 0, 0);
			}
			break;

		case WINITEM_TABLE:
			{
				int col = Round(pos);

				if (col != item->TabCol)
				{
					item->TabCol = col;
					item->TextValid = false;
				}
			}
			break;

		case WINITEM_HSCROLLBAR:
		case WINITEM_VSCROLLBAR:
			{
				double minpage = (double)1/SCROLLRANGE;
				if (pos < minpage) pos = minpage;
				if (pos > 1) pos = 1;
				if (pos != item->ScrollPage)
				{
					item->ScrollPage = pos;
					item->ScrollMax = (SCROLLRANGE - Round(pos * SCROLLRANGE));

					SCROLLINFO si;
					si.cbSize = sizeof(SCROLLINFO);
					si.fMask = SIF_PAGE | SIF_POS;
					si.nPage = Round(SCROLLRANGE * pos);
					si.nPos = Round(item->ScrollMax * item->ScrollPos);

					::SetScrollInfo(item->Wnd, SB_CTL, &si, TRUE);
				}
			}
			break;
		}
	}

	ProgramFill = false;
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� r�me�ku/nadpisu okna

void AktWindowFrame(WINITEM* item)
{
// test, zda to je okno
	bool wnd = ((item->Typ == WINITEM_WINDOW) || (item->Typ == WINITEM_WINDOWRO));

// na�ten� aktu�ln�ho stylu
	long style = ::GetWindowLong(item->Wnd, GWL_STYLE);
	long extstyle = ::GetWindowLong(item->Wnd, GWL_EXSTYLE);

// vynulov�n� p��znak�
	style &= ~(WS_CAPTION | WS_BORDER | WS_DLGFRAME | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX);
	extstyle &= ~(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_DLGMODALFRAME | WS_EX_TOOLWINDOW | WS_EX_STATICEDGE);


// --- varianta pro okna
	if (wnd)
	{

// m�niteln� velikost okna - mohou b�t ikony pro maximalizaci a minimalizaci
		if (item->HasSize) style |= WS_MAXIMIZEBOX | WS_MINIMIZEBOX;

// zapnut� r�me�ku okna
		if (item->HasBorder)
		{
			style |= WS_BORDER;

			if (WindowID == 0) style |= WS_MINIMIZEBOX;

			if (item->HasSize)
			{
				style |= WS_THICKFRAME;
				extstyle |= WS_EX_WINDOWEDGE;
			}
			else
			{
				extstyle |= WS_EX_DLGMODALFRAME;
			}
		}

// vypnut� r�me�ku okna (je-li titulek, nahrad� se tenk�m r�me�kem)
		else
		{
			if (item->HasCaption)
			{
				extstyle |= WS_EX_STATICEDGE;
			}
		}

// zapnut� titulku
		if (item->HasCaption)
		{
			style |= (WS_CAPTION | WS_BORDER);
		}
	}

// --- varianta pro dialogov� prvky

	else
	{
		if (!item->HasSize)
		{

// zapnut� r�me�ku okna
			if (item->HasBorder)
			{
				style |= WS_BORDER;

				extstyle |= WS_EX_CLIENTEDGE;

				if (item->HasCaption)
				{
					extstyle |= WS_EX_DLGMODALFRAME;
				}
			}

// vypnut� r�me�ku okna (je-li titulek, nahrad� se tenk�m r�me�kem)
			else
			{
				if (item->HasCaption)
				{
					extstyle |= WS_EX_STATICEDGE;
				}
			}
		}
		else
		{

// zapnut� r�me�ku okna
			if (item->HasBorder)
			{
				style |= WS_BORDER;
				extstyle |= WS_EX_STATICEDGE;

				if (item->HasCaption)
				{
					extstyle |= WS_EX_CLIENTEDGE | WS_EX_STATICEDGE;
				}
			}

// vypnut� r�me�ku okna (je-li titulek, nahrad� se vyv��en�m r�me�kem)
			else
			{
				if (item->HasCaption)
				{
					extstyle |= WS_EX_DLGMODALFRAME;
				}
				else
				{
					style |= WS_BORDER;
				}
			}
		}
	}

// nastaven� nov�ho stylu
	::SetWindowLong(item->Wnd, GWL_EXSTYLE, extstyle); 
	::SetWindowLong(item->Wnd, GWL_STYLE, style);

// korekce rozm�r� hlavn�ho okna
	if (WindowID == 0)
	{
		MainFrameStyle = style;
		MainFrameStyleEx = extstyle;

		RECT rc;
		AdjustMainFrame(&rc);

		::SetWindowPos(item->Wnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_FRAMECHANGED | SWP_DRAWFRAME |
			SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER | SWP_NOZORDER);
	}
	else

// aktualizace zobrazen� okna
	{
		::SetWindowPos(item->Wnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_DRAWFRAME |
			SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE);
	}

// aktualizace informac� o prvku
	InitClientRect(item);
}


void FSetWindowFrame()
{
	bool frame = FBool();
	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];

		if (item->HasBorder != frame)
		{
			item->HasBorder = frame;
			AktWindowFrame(item);
		}
	}
}

void FSetWindowCaption()
{
	bool caption = FBool();
	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];

		if (item->HasCaption != caption)
		{
			item->HasCaption = caption;
			AktWindowFrame(item);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� okna s m�nitelnou velikost�

void FSetWindowSize()
{
	bool resize = FBool();
	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];

		if (item->HasSize != resize)
		{
			item->HasSize = resize;
			AktWindowFrame(item);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� povolen� uzav�en� okna

void FSetWindowMayClose()
{
	Win[DialogParent].MayClose = FBool();
	ReqClose = false;
}

/////////////////////////////////////////////////////////////////////////////
// maximalizace okna

void FSetWindowMaxim()
{
	bool maxim = FBool();

	if (!MainFrameVisible)
	{
		if (maxim)
		{
			MainFrameVisible = true;
			OnSize(&Win[0]);
			::ShowWindow(MainFrame, SW_SHOWMAXIMIZED);
			::SetFocus(MainFrame);
		}
	}
	else
	{
		::ShowWindow(Win[DialogParent].Wnd, maxim ? SW_MAXIMIZE : SW_RESTORE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// minimalizace okna

void FSetWindowMinim()
{
	bool minim = FBool();

	if (!MainFrameVisible)
	{
		if (minim)
		{
			MainFrameVisible = true;
			OnSize(&Win[0]);
			::ShowWindow(MainFrame, SW_SHOWMINNOACTIVE);
		}
	}
	else
	{
		::ShowWindow(Win[DialogParent].Wnd, minim ? SW_MINIMIZE : SW_RESTORE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� okna v�dy naho�e

void FSetWindowTop()
{
	bool top = FBool();
	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];

		if (item->AlwaysTop != top)
		{
			if ((item->Typ == WINITEM_WINDOW) || (item->Typ == WINITEM_WINDOWRO))
			{
				item->AlwaysTop = top;

				if (WindowID == 0)
				{
					MainFrameStyleEx &= ~WS_EX_TOPMOST;
					if (top) MainFrameStyleEx |= WS_EX_TOPMOST;
				}

				::SetWindowPos(item->Wnd, top ? HWND_TOPMOST : HWND_NOTOPMOST,
						0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOMOVE | SWP_NOSIZE);
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// viditelnost okna

void FWindowVisible()
{
	if (Win.IsValid(WindowID) && (WindowID != 0))
	{
		WINITEM* item = &Win[WindowID];
		item->Show = !item->Show;
		::ShowWindow(item->Wnd, item->Show ? SW_SHOW : SW_HIDE);

		if (item->Show && (item->Typ == WINITEM_TABLE))
		{
			int row = item->TabRowF;

			if ((DWORD)row < (DWORD)item->TabRows)
			{
				LVITEM lvi;
				lvi.stateMask = LVIS_SELECTED;
				lvi.state = LVIS_SELECTED;

				if (AppActive)
				{
					lvi.stateMask = LVIS_SELECTED | LVIS_FOCUSED;
					lvi.state = LVIS_SELECTED | LVIS_FOCUSED;
				}

				::SendMessage(item->Wnd, LVM_SETITEMSTATE, row, (LPARAM)&lvi);

				::SendMessage(item->Wnd, LVM_ENSUREVISIBLE, row, FALSE);
			}
		}
	}
}

void FWindowVisibleSet()
{
	bool show = FBool();
	if (Win.IsValid(WindowID))
	{
		if (Win[WindowID].Show != show) FWindowVisible();
	}
}


/////////////////////////////////////////////////////////////////////////////
// z�kaz prvku

void FWindowDisable()
{
	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];

		if ((item->Typ != WINITEM_WINDOW) && (item->Typ != WINITEM_WINDOWRO))
		{
			item->Disable = !item->Disable;

			if ((item->Typ == WINITEM_EDIT) || 
				(item->Typ == WINITEM_MEMO) || 
				(item->Typ == WINITEM_RICHEDIT) ||
				(item->Typ == WINITEM_RICHMEMO))
			{
				::SendMessage(item->Wnd, EM_SETREADONLY, item->Disable, 0);
			}
			else
			{
				if (item->Typ == WINITEM_TABLE)
				{
					int styl = ::GetWindowLong(item->Wnd, GWL_STYLE);

				// pozn�mka: u ji� zobrazen�ho okna nelze posuvn�ky vypnout
					styl ^= LVS_NOSCROLL;

				// pozn�mka: mo�n� je to chyba syst�mu, �e vyp�n� hlavi�ku, je-li z�kaz
				// scrolov�n�; pro jednozna�nost je to zde zaji�t�no natrvdo
					if (item->TabHead)
					{
						item->TabHead = false;
						styl |= LVS_NOSORTHEADER;
						styl &= ~LVS_NOCOLUMNHEADER;
					}

					::SetWindowLong(item->Wnd, GWL_STYLE, styl);
				}
				else
				{
					::EnableWindow(item->Wnd, !item->Disable);
				}
			}
		}
	}
}

void FWindowDisableSet()
{
	bool disable = FBool();
	if (Win.IsValid(WindowID))
	{
		if (Win[WindowID].Disable != disable) FWindowDisable();
	}
}


/////////////////////////////////////////////////////////////////////////////
// p�ep�na�

void FCheckBoxOn()
{
	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];

		switch (item->Typ)
		{
		case WINITEM_BUTTONRADIO:
		case WINITEM_BUTTONRADIO2:
			if (!item->Check)
			{
				RadioSet(WindowID);
				break;
			}

// pokra�uje BUTTON
		case WINITEM_BUTTONCHECK:
		case WINITEM_BUTTONCHECK2:
		case WINITEM_BUTTONICON:
		case WINITEM_BUTTON3:
			{
				ProgramFill = true;

				item->Check = !item->Check;
				item->Check3 = false;
				::SendMessage(item->Wnd, BM_SETCHECK, item->Check ? BST_CHECKED : BST_UNCHECKED, 0);

				ProgramFill = false;
			}
			break;

		case WINITEM_WINICON:
		case WINITEM_WINPIC:
			{
				item->Check = !item->Check;
				item->Check3 = false;
			}
			break;
		}
	}
}

void FCheckBoxOnSet()
{
	bool check = FBool();
	if (Win.IsValid(WindowID) && (Win[WindowID].Check != check))
	{
		FCheckBoxOn();
	}
}

void FButton3On()
{
	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];

		switch (item->Typ)
		{
		case WINITEM_BUTTON3:
		case WINITEM_BUTTONICON:
		case WINITEM_BUTTONCHECK2:
			{
				ProgramFill = true;
			
				item->Check = false;
				item->Check3 = !item->Check3;
				::SendMessage(item->Wnd, BM_SETCHECK, item->Check3 ? BST_INDETERMINATE : BST_UNCHECKED, 0);

				ProgramFill = false;
			}
			break;

		case WINITEM_WINICON:
		case WINITEM_WINPIC:
			item->Check = false;
			item->Check3 = !item->Check3;
			break;
		}
	}
}

void FButton3OnSet()
{
	bool check3 = FBool();
	if (Win.IsValid(WindowID) && (Win[WindowID].Check3 != check3))
	{
		FButton3On();
	}
}

/////////////////////////////////////////////////////////////////////////////
// okno s fokusem

void FFocus()
{
// zapnut� hlavn�ho okna
	MainFrameShow();

// nastaven� prvku se zam��en�m
	int focus = FIntR();
	if (Win.IsValid(focus) && (Win[focus].Typ != WINITEM_WINDOW) && (Win[focus].Typ != WINITEM_WINDOWRO))
	{
		WindowFocus = focus;
		ProgramFill = true;
		::SetFocus(Win[focus].Wnd);
		ProgramFill = false;
	}
}


/////////////////////////////////////////////////////////////////////////////
// p�smo

void FFontBold()
{
	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];
		item->Bold = !item->Bold;
		WindowSetFont(item);
	}
}

void FFontBoldSet()
{
	bool bold = FBool();
	if (Win.IsValid(WindowID) && (Win[WindowID].Bold != bold))
	{
		FFontBold();
	}
}

void FFontFixed()
{
	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];
		item->Fixed = !item->Fixed;
		WindowSetFont(item);
	}
}

void FFontFixedSet()
{
	bool fixed = FBool();
	if (Win.IsValid(WindowID) && (Win[WindowID].Fixed != fixed))
	{
		FFontFixed();
	}
}

void FFontHeightSet()
{
	int height = FIntY();
	if (Win.IsValid(WindowID) && (Win[WindowID].FontHeight != height))
	{
		WINITEM* item = &Win[WindowID];
		item->FontHeight = height;
		WindowSetFont(item);
	}
}

void FFontItalic()
{
	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];
		item->Italic = !item->Italic;
		WindowSetFont(item);
	}
}

void FFontItalicSet()
{
	bool italic = FBool();
	if (Win.IsValid(WindowID) && (Win[WindowID].Italic != italic))
	{
		FFontItalic();
	}
}

void FFontUnder()
{
	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];
		item->Underline = !item->Underline;
		WindowSetFont(item);
	}
}

void FFontUnderSet()
{
	bool under = FBool();
	if (Win.IsValid(WindowID) && (Win[WindowID].Underline != under))
	{
		FFontUnder();
	}
}

void FFontStrike()
{
	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];
		item->Strikeout = !item->Strikeout;
		WindowSetFont(item);
	}
}

void FFontStrikeSet()
{
	bool strike = FBool();
	if (Win.IsValid(WindowID) && (Win[WindowID].Strikeout != strike))
	{
		FFontStrike();
	}
}

void FFontSerif()
{
	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];
		item->Serif = !item->Serif;
		WindowSetFont(item);
	}
}

void FFontSerifSet()
{
	bool serif = FBool();
	if (Win.IsValid(WindowID) && (Win[WindowID].Serif != serif))
	{
		FFontSerif();
	}
}

void FFontWidthSet()
{
	double width = FNum();
	if (Win.IsValid(WindowID) && (Win[WindowID].FontWidth != width))
	{
		WINITEM* item = &Win[WindowID];
		item->FontWidth = width;
		WindowSetFont(item);
	}
}

void FFontAngleSet()
{
	double angle = FNum();
	if (Win.IsValid(WindowID) && (Win[WindowID].Angle != angle))
	{
		WINITEM* item = &Win[WindowID];
		item->Angle = angle;
		WindowSetFont(item);
	}
}

void FFontColSet()
{
	int color = (FIntR() & 0xffffff);
	if (Win.IsValid(WindowID) && (Win[WindowID].FontCol != color))
	{
		WINITEM* item = &Win[WindowID];
		item->FontCol = color;

		if (item->Typ == WINITEM_TABLE)
		{
			::SendMessage(item->Wnd, LVM_SETTEXTCOLOR, 0, color);
		}
		::InvalidateRect(item->Wnd, NULL, FALSE);
	}
}

void FFontBackSet()
{
	int color = (FIntR() & 0xffffff);
	if (Win.IsValid(WindowID) && (Win[WindowID].FontBack != color))
	{
		WINITEM* item = &Win[WindowID];
		item->FontBack = color;

		if ((item->FontBrush != StdBrushBtn) &&
			(item->FontBrush != StdBrushWindow) &&
			(item->FontBrush != NULL))
		{
			::DeleteObject(item->FontBrush);
		}

		if (color == StdColorBtnFace)
		{
			item->FontBrush = StdBrushBtn;
		}
		else
		{
			if (color == StdColorWindow)
			{
				item->FontBrush = StdBrushWindow;
			}
			else
			{
//				HBRUSH brush = ::CreateHatchBrush(HS_CROSS, item->FontBack);
				HBRUSH brush = ::CreateSolidBrush(item->FontBack);
				if (brush != NULL) item->FontBrush = brush;
			}
		}

		if (item->Typ == WINITEM_TABLE)
		{
			::SendMessage(item->Wnd, LVM_SETTEXTBKCOLOR, 0, color);
			::SendMessage(item->Wnd, LVM_SETBKCOLOR, 0, color);
		}
		::InvalidateRect(item->Wnd, NULL, TRUE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// p�id�n� polo�ky do seznamu

void FListBoxAdd()
{
	CString text;
	FText(text);

	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];

		if (item->Typ == WINITEM_LISTBOX)
		{
			::SendMessage(item->Wnd, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)text);
		}

		if (item->Typ == WINITEM_TABS)
		{
			TCITEM tcitem;
			tcitem.mask = TCIF_TEXT;
			tcitem.pszText = (LPTSTR)(LPCTSTR)text;

			::SendMessage(item->Wnd, TCM_INSERTITEM, 
				::SendMessage(item->Wnd, TCM_GETITEMCOUNT, 0, 0),
				(LPARAM)&tcitem);
		}

		if ((item->Typ == WINITEM_COMBOBOX) || 
			(item->Typ == WINITEM_COMBOEDIT) ||
			(item->Typ == WINITEM_LISTEDIT))
		{
			::SendMessage(item->Wnd, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)text);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// vypr�zdn�n� seznamu

void FListBoxClear()
{
	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];
		HWND wnd = item->Wnd;

		switch (item->Typ)
		{
		case WINITEM_LISTBOX:
			::SendMessage(wnd, LB_RESETCONTENT, 0, 0);
			item->TextValid = false;
			break;

		case WINITEM_TABS:
			::SendMessage(wnd, TCM_DELETEALLITEMS, 0, 0);
			item->TextValid = false;
			break;

		case WINITEM_COMBOBOX:
		case WINITEM_COMBOEDIT:
		case WINITEM_LISTEDIT:
			::SendMessage(wnd, CB_RESETCONTENT, 0, 0);
			item->TextValid = false;
			break;

		case WINITEM_TABLE:
			{
				int oldrows = item->TabRows;

				item->TabRows = 0;
				item->TabRow = 0;
				item->TabRowF = 0;
				item->TabCol = 0;

				::SendMessage(wnd, LVM_DELETEALLITEMS, 0, 0);

				item->TextValid = false;

				int cols = item->TabCols;

				for (int i = oldrows * cols - 1; i >= cols; i--)
				{
					item->TabData[i].Term();
				}

				for (; i >= 0; i--)
				{
					item->TabData[i].Empty();
				}

				item->TabRows = 1;

				LVITEM lvi;
				MemFill(&lvi, sizeof(lvi), 0);
				lvi.mask = LVIF_TEXT | LVIF_NORECOMPUTE;
				lvi.pszText = LPSTR_TEXTCALLBACK;

				::SendMessage(wnd, LVM_INSERTITEM, 0, (LPARAM)&lvi);
			}
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// ���ka sloupce tabulky

void FSetTableWidth()
{
	double width = FNum();

	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];

		int col = item->TabCol;

		if ((DWORD)col < (DWORD)item->TabCols)
		{
			if (width <= 0) width = 0;
			int w0 = Round(width * ICONWIDTH);

			::SendMessage(item->Wnd, LVM_SETCOLUMNWIDTH, col, MAKELPARAM(w0, 0));
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// zarovn�n� sloupce tabulky

void FSetTableAlign()
{
	BYTE align = (BYTE)(FIntR() & 3);

	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];

		int col = item->TabCol;

		if ((DWORD)col < (DWORD)item->TabCols)
		{
//			if (align != item->TabAlign[col])
//			{
				item->TabAlign[col] = align;

//				if (col == 0) align = 0;
//
//				LVCOLUMN lvc;
//				lvc.mask = LVCF_FMT;
//				lvc.fmt = align;
//				::SendMessage(item->Wnd, LVM_SETCOLUMN, col, (LPARAM)&lvc);
//			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// ikona okna

void FWindowIcon()
{
	CIcon icon;
	FIcon(icon);

	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];

		if (item->Icon != icon)
		{
			item->Icon = icon;

			switch (item->Typ)
			{
			case WINITEM_WINDOW:
			case WINITEM_WINDOWRO:
				::SendMessage(item->Wnd, WM_SETICON, ICON_BIG, (LPARAM)icon.HIcon());
				::SendMessage(item->Wnd, WM_SETICON, ICON_SMALL, (LPARAM)icon.HIcon());
				break;

			case WINITEM_BUTTONICON:
				::SendMessage(item->Wnd, BM_SETIMAGE, IMAGE_ICON, (LPARAM)icon.HIcon());
				break;

			case WINITEM_WINICON:
				::SendMessage(item->Wnd, STM_SETIMAGE, IMAGE_ICON, (LPARAM)icon.HIcon());
				break;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// obr�zek okna

void FWindowPic()
{
	CPicture pic;
	FPicture(pic);

	if (Win.IsValid(WindowID))
	{
		WINITEM* item = &Win[WindowID];

		if (item->Picture != pic)
		{
			item->Picture = pic;
			MemFree(item->PictureData);
			item->PictureData = NULL;

			switch (item->Typ)
			{
			case WINITEM_WINPIC:
				::InvalidateRect(item->Wnd, NULL, FALSE);
				::UpdateWindow(item->Wnd);
				break;
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// tla��tko pro kl�vesu Esc

void FSetButtonEsc()
{
	int i = FIntR();
	if (Win.IsNotValid(i))
	{
		i = -1;
	}
	ButtonEsc = i;
}


/////////////////////////////////////////////////////////////////////////////
// celoobrazokov� m�d

void FFullScreen()
{
	SetFullScreen(!FullScreen);
}

void FFullScreenSet()
{
	bool full = FBool();
	SetFullScreen(full);
}


/////////////////////////////////////////////////////////////////////////////
// kurzor my�i

void FMouseCursor()
{
// na�ten� parametr� kurzoru
	CIcon icon;
	FIcon(icon);
	int x1 = FIntX();
	int y1 = FIntY();
	int x2 = FIntX() + x1;
	int y2 = FIntY() + y1;
	
// kontrola platnosti sou�adnic
	if ((x2 <= x1) || (y2 <= y1)) return;

// vypust�n� p�ekryt�ch oblast� region� my�i
	int i = MouseRegNum - 1;				// posledn� polo�ka
	MOUSEREG* reg = &MouseReg[i];			// tabulka region�
	for (; i >= 0; i--)
	{
		if ((reg->x1 >= x1) &&
			(reg->y1 >= y1) &&
			(reg->x2 <= x2) &&
			(reg->y2 <= y2))
		{
			if (i < MouseRegNum-1)
			{
				MemCopy(reg, reg+1, (MouseRegNum-1-i)*sizeof(MOUSEREG));
			}
			MouseRegNum--;
		}
		reg--;
	}

// p�id�n� nov� oblasti
	i = MouseRegNum;
	MouseRegNum++;
	if (MouseRegNum > MouseRegMax)
	{
		MouseRegMax += 64;
		MouseReg = (MOUSEREG*)MemSize(MouseReg, MouseRegMax*sizeof(MOUSEREG));
	}
	reg = &MouseReg[i];
	reg->x1 = x1;
	reg->y1 = y1;
	reg->x2 = x2;
	reg->y2 = y2;
	reg->Cursor = icon.Cursor();

// nastaven� kurzoru my�i, le��-li v nov� oblasti
	int x = MouseMain.x;
	int y = MouseMain.y;
	if (FullScreen)
	{
		if (x < DispLeft) x = DispLeft;
		if (x >= DispLeft + DispWidth) x = DispLeft + DispWidth - 1;
		if (y <= DispTop) y = DispTop+1;
		if (y > DispTop + DispHeight) y = DispTop + DispHeight;
	}

	x = Round((x - DispLeft)/Meritko);
	y = Round((DispHeight - (y - DispTop))/Meritko);

	if ((x >= x1) && (x < x2) && (y >= y1) && (y < y2))
	{
		if (FullScreen && (reg->Cursor == EmptyIconData.Cursor))
		{
			::SetCursor(CurArrow2);
		}
		else
		{
			::SetCursor(reg->Cursor);
		}
	}

// korekce pro implicitn� kurzor
	if ((MouseRegNum == 0) && (icon.IsEmpty()))
	{
		MouseRegNum = 0;
	}
}


/////////////////////////////////////////////////////////////////////////////
// obsluha kurzoru my�i (vrac� TRUE = kurzor my�i nastaven)

BOOL OnSetCursor(int x, int y)
{
// kontrola, zda je my� v oblasti okna
	if ((x < DispLeft) ||
		(x >= DispLeft + DispWidth) ||
		(y <= DispTop) ||
		(y > DispTop + DispHeight))
	{
		if (!FullScreen) return FALSE;

		if (x < DispLeft) x = DispLeft;
		if (x >= DispLeft + DispWidth) x = DispLeft + DispWidth - 1;
		if (y <= DispTop) y = DispTop+1;
		if (y > DispTop + DispHeight) y = DispTop + DispHeight;
	}

// p��prava sou�adnice kurzoru my�i
	int x0 = Round((x - DispLeft)/Meritko);
	int y0 = Round((DispHeight - (y - DispTop))/Meritko);

// nalezen� oblasti my�i a nastaven� kurzoru
	int i = MouseRegNum - 1;				// posledn� polo�ka
	MOUSEREG* reg = &MouseReg[i];			// tabulka region�
	for (; i >= 0; i--)
	{
		if ((x0 >= reg->x1) &&
			(x0 < reg->x2) &&
			(y0 >= reg->y1) &&
			(y0 < reg->y2))
		{
			if (reg->Cursor == EmptyIconData.Cursor)
			{
				if (FullScreen)
				{
					::SetCursor(CurArrow2);
					return TRUE;
				}
				return FALSE;
			}
			::SetCursor(reg->Cursor);
			return TRUE;
		}
		reg--;
	}

// jinak implicitn� kurzor
	if (FullScreen)
	{
		::SetCursor(CurArrow2);
		return TRUE;
	}
	return FALSE;
}

/***************************************************************************\
*																			*
*							obsluha my�i									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// nastaven� nov� pozice kurzoru my�i

void SetMouseXY()
{
	POINT pt;

// v�po�et sou�adnic v okn�
	if ((DialogMode && !DialogGraph) || (DialogParent != 0))
	{
		pt.x = Round(MouseX*ICONWIDTH);
		pt.y = Win[DialogParent].ClientHeight - 1 - Round(MouseY*ICONHEIGHT);
	}
	else
	{
		pt.x = Round(MouseX*ICONWIDTH*Meritko) + DispLeft;
		pt.y = DispHeight - 1 + DispTop - Round(MouseY*ICONHEIGHT*Meritko);
	}

// p�epo�et lok�ln�ch sou�adnic kurzoru na sou�adnice obrazovky
	::ClientToScreen(Win[DialogParent].Wnd, &pt);

// zachov�n� posunu my�i od posledn�ho na��t�n� sou�adnic
	POINT newpt;
	::GetCursorPos(&newpt);
	pt.x += (newpt.x - MouseScreen.x);
	pt.y += (newpt.y - MouseScreen.y);

// nastaven� nov� pozice kurzoru my�i
	::SetCursorPos(pt.x, pt.y);

// na�ten� nov�ch sou�adnic kurzoru my�i
	MouseValid = false;
	GetAktMousePos();
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� sou�adnice X my�i

void FSetMouseX()
{
	double mousex = FNum();
	if (mousex != MouseX)
	{
		MouseX = mousex;
		SetMouseXY();
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� sou�adnice Y my�i

void FSetMouseY()
{
	double mousey = FNum();
	if (mousey != MouseY)
	{
		MouseY = mousey;
		SetMouseXY();
	}
}

/***************************************************************************\
*																			*
*									DirectPlay								*
*																			*
\***************************************************************************/

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

/////////////////////////////////////////////////////////////////////////////
// nastaven� aktivn�ho spojen�

void FSetConnectAct()
{
	CString text;
	FText(text);
	SetConnectAct(text);
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� aktivn� hry

void FSetGameAct()
{
	CString text;
	FText(text);
	SetGameAct(text);
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� aktivn�ho hr��e

void FSetPlayerAct()
{
	CString text;
	FText(text);
	SetPlayerAct(text);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� ��seln� hodnoty hry

void FSetGameN1() { SetGameN(FIntR(), 0); }
void FSetGameN2() { SetGameN(FIntR(), 1); }
void FSetGameN3() { SetGameN(FIntR(), 2); }
void FSetGameN4() { SetGameN(FIntR(), 3); }
void FSetGameN5() { SetGameN(FIntR(), 4); }
void FSetGameN6() { SetGameN(FIntR(), 5); }
void FSetGameN7() { SetGameN(FIntR(), 6); }
void FSetGameN8() { SetGameN(FIntR(), 7); }


/////////////////////////////////////////////////////////////////////////////
// nastaven� p�ep�na�e hry

void FSetGameL1() { SetGameL(FBool(), 0); }
void FSetGameL2() { SetGameL(FBool(), 1); }
void FSetGameL3() { SetGameL(FBool(), 2); }
void FSetGameL4() { SetGameL(FBool(), 3); }
void FSetGameL5() { SetGameL(FBool(), 4); }
void FSetGameL6() { SetGameL(FBool(), 5); }
void FSetGameL7() { SetGameL(FBool(), 6); }
void FSetGameL8() { SetGameL(FBool(), 7); }

/////////////////////////////////////////////////////////////////////////////
// nastaven� max. po�tu hr���

void FSetPlayerMax() { SetPlayerMax(FIntR()); }

/////////////////////////////////////////////////////////////////////////////
// vysl�n� datov�ho paketu

void FSetGamePacket()
{
	DirectSend(FIntR());
}

void FSetGamePacket0()
{
	DirectSend(-1);
}

/////////////////////////////////////////////////////////////////////////////
// vysl�n� ��sla paketem

void FSetGameDataN()
{
	double n = FNum();
	DirectSendData((BYTE*)&n, 8);
}

/////////////////////////////////////////////////////////////////////////////
// vysl�n� textu paketem (nen� pro UNICODE !!!)

void FSetGameDataT()
{
	CString text;
	FText(text);
	DirectSendData((BYTE*)text.DataData(), text.Length() + 1);
}

/////////////////////////////////////////////////////////////////////////////
// vysl�n� p�ep�na�e paketem

void FSetGameDataL()
{
	bool log = FBool();
	BYTE n = (BYTE)(log ? 1 : 0);
	DirectSendData(&n, 1);
}

/////////////////////////////////////////////////////////////////////////////
// vysl�n� bajtu paketem

void FSetGameDataB()
{
	int n = FIntR();
	if (n < 0) n = 0;
	if (n > 255) n = 255;
	BYTE nn = (BYTE)n;
	DirectSendData(&nn, 1);
}

/***************************************************************************\
*																			*
*										UDP									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// nastaven� vys�lac� IP adresy UDP

void FSetUDPSendIP()
{
	CString text;
	FText(text);
	text.TrimLeft();
	text.TrimRight();

	if (WSAInitialize2())
	{
		UDPSendAddr.sin_addr.s_addr = pWSAInetAddr(text);
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� vys�lac�ho portu UDP

void FSetUDPSendPort()
{
	int n = FIntR();
	if (n < 0)
	{
		CloseSendUDP();
	}
	else
	{
		if (n > 0xffff) n = 0xffff;
		n = ((n & 0xff) << 8) | (n >> 8);
		UDPSendAddr.sin_port = (WORD)n;
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� velikosti p�ij�mac�ho bufferu UDP (0=zav�en� UDP)

void FSetUDPRecSize()
{
	int n = FIntR();
	if (n < 0) n = 0;
	if (n > 16384) n = 16384;
	if ((int)UDPRecBuf.len != n)
	{
		UDPRecBuf.len = n;

#ifdef UDP_OVER
		if (UDPRecSocket != INVALID_SOCKET)
		{
			pWSASetSockOpt(UDPRecSocket, SOL_SOCKET, SO_RCVBUF, (char*)&n, sizeof(n));
		}
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� p�ij�mac� IP adresy UDP

void FSetUDPRecIP()
{
	// na�ten� IP adresy
	CString text;
	FText(text);
	text.TrimLeft();
	text.TrimRight();

	if (WSAInitialize2())
	{
		DWORD adr = pWSAInetAddr(text);
		if (adr != UDPRecAddr.sin_addr.s_addr)
		{
			UDPRecAddr.sin_addr.s_addr = adr;

			if (UDPRecSocket != INVALID_SOCKET)
			{
#ifdef UDP_OVER
				CloseRecUDP();
				OpenRecUDP();
#else
				pWSABind(UDPRecSocket, (sockaddr*)&UDPRecAddr, sizeof(SOCKADDR_IN));
#endif
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� p�ij�mac�ho portu UDP

void FSetUDPRecPort()
{
	// na�ten� ��sla portu
	int n = FIntR();
	if (n < 0)
	{
		CloseRecUDP();
	}
	else
	{
		if (n > 0xffff) n = 0xffff;
		n = ((n & 0xff) << 8) | (n >> 8);
		if (UDPRecAddr.sin_port != (WORD)n)
		{
			UDPRecAddr.sin_port = (WORD)n;

			if (UDPRecSocket != INVALID_SOCKET)
			{
#ifdef UDP_OVER
				CloseRecUDP();
				OpenRecUDP();
#else
				pWSABind(UDPRecSocket, (sockaddr*)&UDPRecAddr, sizeof(SOCKADDR_IN));
#endif
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// odesl�n� bin�rn�ho paketu UDP

void FSetUDPMem()
{
	int n = FIntR();
	SendUDP(DLLMemoryRead, n);
	DLLMemoryRead = (char*)DLLMemoryRead + n;
}

/////////////////////////////////////////////////////////////////////////////
// odesl�n� textov�ho paketu UDP

void FSetUDPText()
{
	CString text;
	FText(text);
	SendUDP((LPCSTR)text.DataData(), text.Length());
}

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------


/////////////////////////////////////////////////////////////////////////////
// nastaven� k�dov� str�nky

void FSetCodePage()
{
	int n = FIntR();
	if (n < 0) n = 0;
	CodePage = n;

	switch (n)
	{
	case 1250:
		CharSet = EASTEUROPE_CHARSET;
		break;

	case 1251:
		CharSet = RUSSIAN_CHARSET;
		break;

	case 1252:
		CharSet = ANSI_CHARSET;
		break;

	case 1253:
		CharSet = GREEK_CHARSET;
		break;

	case 1254:
		CharSet = TURKISH_CHARSET;
		break;

	case 1255:
		CharSet = HEBREW_CHARSET;
		break;

	case 1256:
		CharSet = ARABIC_CHARSET;
		break;

	case 1257:
		CharSet = BALTIC_CHARSET;
		break;

	case 1258:
		CharSet = VIETNAMESE_CHARSET;
		break;

	default:
		CharSet = DEFAULT_CHARSET;
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� u�ivatelsk�ho fontu

void FSetUserFont()
{
	CString txt;
	FText(txt);
	txt.TrimLeft();
	txt.TrimRight();
	if (txt.IsNotEmpty()) UserFont = txt;
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� jazyku

void FSetLanguage()
{
	int langID = FIntR();

	if (langID == (int)LangID) return;

	Jazyk = JazykDef;

	for (int ll = 0; ll < JAZYKNUM; ll++)
	{
		if (JazykInfo[ll].LangID == langID)
		{
			Jazyk = ll;
			break;
		}
	}

	InitLang(Jazyk);
	InitKeyb();

// p��prava ukazatel� jazyk�
	for (int lan = 0; lan < JAZYKNUM; lan++)
	{
		JazykUkaz[lan] = JazykInfo[lan].TextTab;
	}
	JazykUkaz0 = 0;

// na�ten� a aktualizace text�
	int poc = PetAdr->piText.Pocet;

	for (int i = poc; i > 0; i--)
	{
		CString txt = LoadText0();
		int orig = txt.Orig();

		for (int inx = Text.Num() - 1; inx >= poc; inx--)
		{
			if ((inx == (TextListEnd - 1)) && (TextListEnd > TextListBeg)) inx = TextListBeg;

			if (Text[inx].Orig() == orig)
			{
				Text.Set(inx, txt);
			}
		}

		if (Text.IsValid(orig) && (Text[orig].Orig() == orig))
		{
			Text.Set(orig, txt);
		}

		if (AktCaptionText.Orig() == orig)
		{
			SetCaptionText(txt);
		}

#ifndef _MINI

		if (DirectPlayGuidText.Orig() == orig)
		{
			DirectPlayGuidText = txt;

			int i;
			for (i = 4; i < 4+6; i++)
			{
				((char*)&DirectPlayGuidData)[i] = 0;
			}

			int n = 4;
			for (i = 0; i < DirectPlayGuidText.Length(); i++)
			{
				((char*)&DirectPlayGuidData)[n] = (char)(((char*)&DirectPlayGuidData)[n] + DirectPlayGuidText[i]);
				n++;
				if (n == 4+6) n = 4;
			}
		}

#endif // _MINI

		if (AktStatusText.Orig() == orig)
		{
			SetStatusText(txt);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� alias

void FSetAlias()
{
// text k nastaven�
	CString txt;
	FText(txt);
	txt.TrimLeft();
	txt.TrimRight();

// test, zda je soubor zadan�
	if (FileWrite.IsEmpty())
	{
		FileError = true;
		return;
	}

// p��prava aliases
	InitAlias(FileWrite);
	InitAliasGroup(Jazyk);

// z�pis aliases
	int res;

	if (txt.IsEmpty())
	{
		res = ::WritePrivateProfileString(AliasGroup, AliasKey, NULL, AliasName);
	}
	else
	{
		res = ::WritePrivateProfileString(AliasGroup, AliasKey, txt, AliasName);
	}
	if (!res) FileError = true;
}


/***************************************************************************\
*																			*
*								Porty										*
*																			*
\***************************************************************************/

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

/////////////////////////////////////////////////////////////////////////////
// nastaven� aktivn�ho portu

void FSetComID()
{
	int id = FIntR();
	if (((DWORD)id < (DWORD)COMSMAX) && (Coms[id].File != INVALID_HANDLE_VALUE))
	{
		ComAkt = id;
	}
}

/////////////////////////////////////////////////////////////////////////////
// uzav�en� portu

void FComClose()
{
	int id = FIntR();
	if ((DWORD)id < (DWORD)COMSMAX)
	{
		ComClose(id);
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� p�enosov� rychlosti portu

void FSetComBaud()
{
	DWORD rate = FIntR();

	COMITEM* item = Coms + ComAkt;
	if (item->File != INVALID_HANDLE_VALUE)
	{
		DCB* dcb = &item->Dcb;

		if (rate != dcb->BaudRate)
		{
			dcb->BaudRate = rate;
			SetComDCB();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� stopbit�

void FSetComStop()
{
	bool stop = FBool();

	COMITEM* item = Coms + ComAkt;
	if (item->File != INVALID_HANDLE_VALUE)
	{
		DCB* dcb = &item->Dcb;

		BYTE newstop = ONESTOPBIT;

		if (stop)
		{
			newstop = TWOSTOPBITS;
			if (dcb->ByteSize <= 5)
			{
				newstop = ONE5STOPBITS;
			}
		}

		if (newstop != dcb->StopBits)
		{
			dcb->StopBits = newstop;
			SetComDCB();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� po�tu bit�

void FSetComBits()
{
	int bits0 = FIntR();
	if (bits0 > 8) bits0 = 8;
	if (bits0 < 5) bits0 = 5;

	BYTE bits = (BYTE)bits0;

	COMITEM* item = Coms + ComAkt;
	if (item->File != INVALID_HANDLE_VALUE)
	{
		DCB* dcb = &item->Dcb;

		if (bits != dcb->ByteSize)
		{
			dcb->ByteSize = bits;

			SetComDCB();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� parity

void FSetComParit()
{
	BYTE parit = (BYTE)FIntR();
	if (parit > 4) parit = 0;

	COMITEM* item = Coms + ComAkt;
	if (item->File != INVALID_HANDLE_VALUE)
	{
		DCB* dcb = &item->Dcb;

		bool par0 = (parit != 0);
		if ((parit != dcb->Parity) || (par0 != (bool)dcb->fParity))
		{
			dcb->Parity = parit;
			dcb->fParity = par0;

			SetComDCB();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// vysl�n� znaku

void FSetComText()
{
	CString txt;
	FText(txt);

	for (int i = 0; i < txt.Length(); i++)
	{
		ComSend((BYTE)txt[i]);
	}
}

/////////////////////////////////////////////////////////////////////////////
// vysl�n� ��sla

void FSetComNum()
{
	ComSend((BYTE)FIntR());
}

/////////////////////////////////////////////////////////////////////////////
// povolen� vys�l�n�, po�adavek k prot�j�ku o vys�l�n� dat (sign�l RTS)

void FSetComSend()
{
	bool ok = FBool();

	COMITEM* item = Coms + ComAkt;
	HANDLE file = item->File;

	if (file != INVALID_HANDLE_VALUE)
	{
		DCB* dcb = &item->Dcb;
		dcb->fRtsControl = ok ? RTS_CONTROL_ENABLE : RTS_CONTROL_DISABLE;
		::EscapeCommFunction(file, ok ? SETRTS : CLRRTS);
	}
}

/////////////////////////////////////////////////////////////////////////////
// povolen� p��jmu, potvrzen� platnosti dat vys�lan�ch k prot�j�ku (sign�l DTR)

void FSetComReceive()
{
	bool ok = FBool();

	COMITEM* item = Coms + ComAkt;
	HANDLE file = item->File;

	if (file != INVALID_HANDLE_VALUE)
	{
		DCB* dcb = &item->Dcb;
		dcb->fDtrControl = ok ? DTR_CONTROL_ENABLE : DTR_CONTROL_DISABLE;
		::EscapeCommFunction(file, ok ? SETDTR : CLRDTR);
	}
}

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------


/***************************************************************************\
*																			*
*								obsluha mixeru								*
*																			*
\***************************************************************************/

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

/////////////////////////////////////////////////////////////////////////////
// nastaven� aktivn�ho mixeru

void FSetMixDevice()
{
// p��prava textu	
	CString txt;
	FText(txt);

// inicializace mixeru
	if (!MixDevInit()) return;

// vyhled�n� jm�na mixeru
	for (int i = MixDevN-1; i > 0; i--)
	{
		if (MixDev[i].Name == txt) break;
	}

// inicializace mixeru
	MixDevSet(i);
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� c�lov�ho sign�lu

void FSetMixLineDst()
{
// p��prava textu	
	CString txt;
	FText(txt);

// inicializace mixeru
	if (!MixDevInit()) return;

// vyhled�n� jm�na sign�lu (pro neplatn� jm�no bude i=-1)
	int i;
	for (i = MixDest-1; i >= 0; i--)
	{
		if (MixLine[i].Name == txt) break;
	}

// nastaven� c�lov�ho sign�lu (-1=��dn�)
	MixDstSet(i);
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� zdrojov�ho sign�lu

void FSetMixLineSrc()
{
// p��prava textu	
	CString txt;
	FText(txt);

// inicializace mixeru
	if (!MixDevInit()) return;
	if ((DWORD)MixLineDA >= (DWORD)MixDest) MixDstSet(0); // nastaven neplatn� c�lov� sign�l
	if (MixLineDA < 0) return;

// vyhled�n� jm�na sign�lu (-1=jen c�lov�)
	int i = MixLine[MixLineDA].SourceI;
	int n = i + MixLine[MixLineDA].Sources;
	int src = -1;

	if (txt.IsNotEmpty())
	{
		for (; i < n; i++)
		{
			if (MixLine[i].Name == txt)
			{
				src = MixLine[i].Source;
				break;
			}
		}
	}

// nastaven� c�lov�ho sign�lu
	MixSrcSet(src);
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� typu sign�lu

void FSetLineType()
{
// po�adovan� ��slo typu sign�lu
	DWORD type = FIntRN();

// inicializace mixeru
	if (!MixDevInit() || (MixLineN == 0)) return;

// vyhled�n� po�adovan�ho typu sign�lu
	MIXLINE* item = MixLine;

	for (int i = MixLineN; i > 0; i--)
	{
		if (item->Type == type)
		{
			int src = item->Source;
			MixDstSet(item->Dest);
			MixSrcSet(src);
			return;
		}
		item++;
	}

// zad�n neplatn� sign�l
	MixDstSet(-1);
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� ovl�dac�ho prvku

void FSetMixControl()
{
// p��prava textu	
	CString txt;
	FText(txt);

// inicializace mixeru
	if (!MixDevInit()) return;
	if ((DWORD)MixLineDA >= (DWORD)MixDest) MixDstSet(0); // nastaven neplatn� c�lov� sign�l

// vyhled�n� jm�na prvku (pro nenalezen� z�stane i = -1)
	int i = MixCtrlN - 1;
	for (; i >= 0; i--)
	{
		if (MixCtrl[i].Name == txt) break;
	}

// nastaven� ovl�dac�ho prvku (-1=nenalezen)
	MixCtrlSet(i);
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� typu ovl�dac�ho prvku

void FSetCtrlType()
{
// po�adovan� ��slo typu ovl�dac�ho prvku
	DWORD type = FIntRN();

// inicializace mixeru
	if (!MixDevInit()) return;
	if ((DWORD)MixLineDA >= (DWORD)MixDest) MixDstSet(0); // nastaven neplatn� c�lov� sign�l

// vyhled�n� typu prvku
	int ctrl = -1;
	for (int i = 0; i < MixCtrlN; i++)
	{
		if (MixCtrl[i].Type == type)
		{
			ctrl = i;
			break;
		}
	}

// nastaven� prvku
	MixCtrlSet(ctrl);
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� hodnoty ovl�dac�ho prvku

void FSetCtrlVal()
{
// po�adovan� hodnota
	double val = FNum();

// nastaven� hodnoty
	SetMixCtrlVal(val);
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� kan�lu ovl�dac�ho prvku

void FSetMixChannel()
{
	MixChannel = FIntR();
}

/////////////////////////////////////////////////////////////////////////////
// v�b�r hodnoty ovl�dac�ho prvku

void FSetMixCtrlList()
{
// p��prava textu	
	CString txt;
	FText(txt);

// inicializace mixeru
	if (!MixDevInit()) return;
	if (MixValN < 2) return;

// vyhled�n� jm�na hodnoty prvku (pro nenalezen� z�stane i = -1)
	int i;
	for (i = MixValN - 1; i >= 0; i--)
	{
		if (MixVal[i] == txt) break;
	}
	MixValA = i;
}

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------

/////////////////////////////////////////////////////////////////////////////
// nastaven� grafick�ho pozad�

void FSetDialogGraph()
{
	bool log = FBool();
	if (log != DialogGraph)
	{
		DialogGraph = log;

		OnSize(&Win[0]);

		if (MainFrameVisible)
		{
			::InvalidateRect(NULL, NULL, TRUE);
		}
	}
}

/***************************************************************************\
*																			*
*									Direct3D								*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// nastaven� videom�du

void FSet3DMode()
{
// p��prava textu	
	CString txt;
	FText(txt);
	txt.LowerCase();

// po�adovan� ���ka
	double k = Double(txt);
	if (k < 0) k = -k;
	if (k < 160) k = k * ICONWIDTH;
	if (k > 10000) k = 10000;
	int width = Round(k);

// po�adovan� v��ka
	int p = txt.Find('*');
	if (p < 0) p = txt.Find('x');
	int height = 0;
	if (p >= 0)
	{
		txt = txt.Mid(p + 1);
		k = Double(txt);
		if (k < 0) k = -k;
		if (k < 100) k = k * ICONHEIGHT;
		if (k > 10000) k = 10000;
		height = Round(k);
	}

// po�adovan� po�et bit�
	p = txt.Find('/');
	if (p < 0) p = txt.Find('\\');
	int bits = 0;
	if (p >= 0)
	{
		bits = Int(txt.Mid(p + 1));
		if (bits < 0) bits = -bits;
		bits = (bits+4) & ~7;
		if (bits > 32) bits = 32;
	}

// parametry videom�du
	if (!FullScreen ||
		(width != UserScreenWidth) ||
		(height != UserScreenHeight) ||
		(bits != UserScreenBits))
	{
		UserScreenWidth = width;
		UserScreenHeight = height;
		UserScreenBits = bits;

// aktivace videom�du
		StartVideoMode();
	}
}

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

/////////////////////////////////////////////////////////////////////////////
// nastaven� okna Direct3D

void FDirect3DView()
{
// blokov�n� Alt+Enter
//	D3DReturn = true;		// blokov�n� Alt+Enter

// na�ten� rozm�r� okna
	D3DX0 = FIntX();		// po��tek X
	D3DY0 = FIntY();		// po��tek Y
	D3DW0 = FIntX();		// ���ka
	D3DH0 = FIntY();		// v��ka

// zapnut� hlavn�ho okna
	MainFrameShow();

// p�epo�et rozm�r�
	if (RecalcD3D())
	{

// pouze zm�na velikost okna
		if (D3D)
		{
			ReSet3DCount = 0;				// resetov�n� opakovan� inicializace
			ReSet3DCountN = 1;				// inicializace p��t�ho ��ta�e

			pD3SizeView();
			pD3MoveView();
			pD3AktPalette(MainFrame);
			AktAllIcon();
		}

// inicializace funkce 3D za��zen�
		else
		{
			UserD3DDevice();
		}
	}

// vypnut� okna 3D
	else
	{
		DeSelectD3DDev();
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� m�du v�pln� Direct3D

void FSetD3DWireframe()
{
	D3DWireframeGlobal = FBool();
}

/////////////////////////////////////////////////////////////////////////////
// zapnut� osv�tlen� Direct3D

void FSetD3DLighton()
{
	D3DLightOnGlobal = FBool();
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� m�du st�nov�n� Direct3D

void FSetD3DShades()
{
	D3DShadesGlobal = FBool();
}

/////////////////////////////////////////////////////////////////////////////
// inverze objektu

void FD3DInvert()
{
	if (IsValidID())
	{
// odpojen� dat od klon�
		D3DF_Modi(D3D_ID);

		D3DFITEM* item = GetID();

// resetov�n� objektu
		pD3Reset(item);

// inverze norm�l
		int n = item->VertNum;
		D3DVECTOR* v = item->Normal;

		if (v != NULL)
		{
			for (; n > 0; n--)
			{
				v->x = - v->x;
				v->y = - v->y;
				v->z = - v->z;

				v++;
			}
		}

// zm�na sm�ru vektor�
		n = item->FaceNum; 
		int* f = item->Face;

		if (f != NULL)
		{
			f++;
			
			for (; n > 0; n--)
			{
				int t = f[0];
				f[0] = f[1];
				f[1] = t;

				f += 3;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// zapnut� filtrace p�i zmen�en�

void FSetD3DMinFilter()
{
	D3DMinFilterGlobal = FBool();
}

/////////////////////////////////////////////////////////////////////////////
// zapnut� filtrace p�i zv�t�en�

void FSetD3DMagFilter()
{
	D3DMagFilterGlobal = FBool();
}

/////////////////////////////////////////////////////////////////////////////
// zapnut� filtrace p�i vzd�len�

void FSetD3DMipFilter()
{
	D3DMipFilterGlobal = FBool();
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� m�du v�pln� objektu Direct3D

void FSetD3DOWireframe()
{
	bool on = FBool();

	if (IsValidID())
	{
		GetID()->Wireframe = on;
	}
}

/////////////////////////////////////////////////////////////////////////////
// zapnut� osv�tlen� objektu Direct3D

void FSetD3DOLighton()
{
	bool on = FBool();

	if (IsValidID())
	{
		GetID()->Lighton = on;
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� m�du st�nov�n� objektu Direct3D

void FSetD3DOShades()
{
	bool on = FBool();

	if (IsValidID())
	{
		GetID()->Shades = on;
	}
}

/////////////////////////////////////////////////////////////////////////////
// zapnut� filtrace p�i zmen�en� textury objektu

void FSetD3DOMinFilter()
{
	bool on = FBool();

	if (IsValidID())
	{
		GetID()->MinFilter[D3DAktStage] = on;
	}
}

/////////////////////////////////////////////////////////////////////////////
// zapnut� filtrace p�i zv�t�en� textury objektu

void FSetD3DOMagFilter()
{
	bool on = FBool();

	if (IsValidID())
	{
		GetID()->MagFilter[D3DAktStage] = on;
	}
}

/////////////////////////////////////////////////////////////////////////////
// zapnut� filtrace p�i vzd�len� textury objektu

void FSetD3DOMipFilter()
{
	bool on = FBool();

	if (IsValidID())
	{
		GetID()->MipFilter[D3DAktStage] = on;
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� odstra�ov�n� ploch objektu

void FSetD3DOCulling()
{
	int val = FIntR();
	if (val < 0) val = 0;
	if (val > 2) val = 0;

	if (IsValidID())
	{
		GetID()->Culling = (short)val;
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� viditelnosti objektu Direct3D

void FSetD3DVisible()
{
	bool vis = FBool();

	if ((D3D_ID > 1) && IsValidID())
	{
		D3DFITEM* item = GetID();

		if (vis != item->Visible)
		{
			item->Visible = vis;

			if (item->Type == D3DFTYPE_LIGHT)
			{
				D3DLITEM* light = D3DL_Get(item->Light);
				light->Akt = true;
				light->Enable = vis;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// zru�en� objektu Direct3D	(Data = po�et prvk� - 1)

void FD3DObjectDelete()
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data + 1;

// zru�eni v�ech prvk� (krom� sc�ny a kamery)
	for (; i > 0; i--)
	{
		int inx = FIntR();

		if (inx > 1) D3DF_Del(inx);
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� aktivn�ho objektu Direct3D

void FSetD3DObjectID()
{
	int i = FIntR();

	if (D3DF_IsValid(i) && (D3DF_Get(i)->Type != D3DFTYPE_SECTOR))
	{
		D3DFITEM* frame = D3DF_Get(i);
		if (frame->IsTerrain)
		{
			TerenID = i;
		}
	}
	else
	{
		i = -1;
	}

	D3D_ID = i;
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� po�ad� rotac� objektu

void FSetD3DOrder()
{
	int order = FIntR();
	if ((DWORD)order > 5) order = D3DFORDER_XYZ;

	if (IsValidID())
	{
		D3DFITEM* frame = GetID();

		if (order != (int)(frame->Order))
		{
			frame->Order = (D3DORDER)order;
			frame->AktOwn = true;
			D3DF_AktMatLink(frame);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� m���tka ve sm�ru X

void FSetD3DScaleX()
{
	double val = FNum();

	if (IsValidID())
	{
		D3DFITEM* frame = GetID();

//		if (val == 0) val = 1;

		if (val != frame->ScaleX)
		{
			frame->ScaleX = val;
			frame->AktOwn = true;
			D3DF_AktMatLink(frame);

			AktScaleB(frame);

			if (frame->Type == D3DFTYPE_PICTURE)
			{
				pD3Reset(frame);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� m���tka ve sm�ru Y

void FSetD3DScaleY()
{
	double val = FNum();

	if (IsValidID())
	{
		D3DFITEM* frame = GetID();

//		if (val == 0) val = 1;

		if (val != frame->ScaleY)
		{
			frame->ScaleY = val;
			frame->AktOwn = true;
			D3DF_AktMatLink(frame);

			AktScaleB(frame);

			if (frame->Type == D3DFTYPE_PICTURE)
			{
				pD3Reset(frame);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� m���tka ve sm�ru Z

void FSetD3DScaleZ()
{
	double val = FNum();

	if (IsValidID())
	{
		D3DFITEM* frame = GetID();

//		if (val == 0) val = 1;

		if (val != frame->ScaleZ)
		{
			frame->ScaleZ = val;
			frame->AktOwn = true;
			D3DF_AktMatLink(frame);

			AktScaleB(frame);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� rotace podle osy X

void FSetD3DRotateX()
{
	double val = FAngle();

	if (IsValidID())
	{
		D3DFITEM* frame = GetID();

		if (val != frame->RotateX)
		{
			frame->RotateX = val;
			frame->SinX = sin(val);
			frame->CosX = cos(val);
			frame->IsRotX = (val != 0);
			frame->AktOwn = true;
			D3DF_AktMatLink(frame);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� rotace podle osy Y

void FSetD3DRotateY()
{
	double val = FAngle();

	if (IsValidID())
	{
		D3DFITEM* frame = GetID();

		if (val != frame->RotateY)
		{
			frame->RotateY = val;
			val = val + frame->RotateY2D;
			frame->SinY = sin(val);
			frame->CosY = cos(val);
			frame->IsRotY = (val != 0);
			frame->AktOwn = true;
			D3DF_AktMatLink(frame);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� rotace podle osy Z

void FSetD3DRotateZ()
{
	double val = FAngle();

	if (IsValidID())
	{
		D3DFITEM* frame = GetID();

		if (val != frame->RotateZ)
		{
			frame->RotateZ = val;
			frame->SinZ = sin(val);
			frame->CosZ = cos(val);
			frame->IsRotZ = (val != 0);
			frame->AktOwn = true;
			D3DF_AktMatLink(frame);

			if (frame->Type == D3DFTYPE_PICTURE)
			{
				pD3Reset(frame);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� posunu ve sm�ru X

void FSetD3DTransX()
{
	double val = FNum();

	if (IsValidID())
	{
		D3DFITEM* frame = GetID();

		while (frame->LODPrev >= 0)
		{
			frame = D3DF_Get(frame->LODPrev);
		}

		for (;;)
		{
			if (val != frame->TransX)
			{
				frame->TransX = val;
				frame->AktOwn = true;
				D3DF_AktMatLink(frame);

				if (frame->Type == D3DFTYPE_PICTURE)
				{
					pD3Reset(frame);
				}
			}

			if (frame->LODNext < 0) break;

			frame = D3DF_Get(frame->LODNext);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� posunu ve sm�ru Y

void FSetD3DTransY()
{
	double val = FNum();

	if (IsValidID())
	{
		D3DFITEM* frame = GetID();

		while (frame->LODPrev >= 0)
		{
			frame = D3DF_Get(frame->LODPrev);
		}

		for (;;)
		{
			if (val != frame->TransY)
			{
				frame->TransY = val;
				frame->AktOwn = true;
				D3DF_AktMatLink(frame);

				if (frame->Type == D3DFTYPE_PICTURE)
				{
					pD3Reset(frame);
				}
			}

			if (frame->LODNext < 0) break;

			frame = D3DF_Get(frame->LODNext);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� posunu ve sm�ru Z

void FSetD3DTransZ()
{
	double val = FNum();

	if (IsValidID())
	{
		D3DFITEM* frame = GetID();

		while (frame->LODPrev >= 0)
		{
			frame = D3DF_Get(frame->LODPrev);
		}

		for (;;)
		{
			if (val != frame->TransZ)
			{
				frame->TransZ = val;
				frame->AktOwn = true;
				D3DF_AktMatLink(frame);
			}

			if (frame->LODNext < 0) break;

			frame = D3DF_Get(frame->LODNext);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� pr�hlednosti

void FSetD3DTransparent()
{
	int n = FIntR();

	if (IsValidID())
	{
		if ((DWORD)n > (DWORD)99) n = 1;

		D3DFITEM* item = GetID();

		if (n != item->SrcDstBlend)
		{
			D3DF_SetBlend(D3D_ID, n);
		}

		if (item->Type == D3DFTYPE_PICTURE)
		{
			item->RendGroup = RENDPIC;
		}
		else
		{
			if (item->BlendRend)
			{
				item->RendGroup = RENDTRANS;
			}
			else
			{
				item->RendGroup = RENDNORM;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� alpfa referen�n� �rovn�

void FSetD3DAlphaRef()
{
	double n = FNum();

	if (IsValidID())
	{
		if (n < 0) n = 0;
		if (n > 1) n = 1;

		D3DFITEM* item = GetID();
		item->AlphaRef = n;
		item->AlphaRef2 = Round(n * 0xff);
		item->IsAlphaRef = (item->AlphaRef2 != 0);
	}
}

/////////////////////////////////////////////////////////////////////////////
// aktualizace materi�lu aktivn�ho objektu

void SetMaterial()
{
	if (IsValidID())
	{
		D3DFITEM* item = GetID();

		D3DMATERIAL8 mat;
		MemCopy(&mat, &(D3DM_Get(item->Material)->Material), sizeof(D3DMATERIAL8));

		BGRFloat(item->Diffuse, &(mat.Diffuse));
		if (item->Ambient == MAXDWORD)
		{
			mat.Ambient = mat.Diffuse;
		}
		else
		{
			BGRFloat(item->Ambient, &(mat.Ambient));
		}

		BGRFloat(item->Emissive, &(mat.Emissive));
		BGRFloat(item->Specular, &(mat.Specular));
//		mat.Diffuse.a = (float)(item->Alpha);
		mat.Power = (float)(item->Power);

		item->Material = D3DM_Add(&mat);
//		item->AktMat = true;
	}
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� barvy objektu

void FSetD3DColor()
{
	hyper col = FIntHN();

	if (IsValidID())
	{
		D3DF_Modi(D3D_ID);

		D3DFITEM* item = GetID();

		switch (col)
		{
		case -2: 
			if (item->Type != D3DFTYPE_PICTURE)
			{
				item->MatSource = MATSOURCE_PARENT;
			}
			break;

		case -1:
			if (item->Type != D3DFTYPE_PICTURE)
			{
				item->MatSource = MATSOURCE_OBJECT;
			}
			break;

		default:
			item->MatSource = MATSOURCE_FRAME;

			DWORD col2 = (DWORD)col;

			if (col2 != item->Diffuse)
			{
				item->Diffuse = col2;
				SetMaterial();
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� odrazu rozpt�len�ho sv�tla

void FSetD3DAmbiRef()
{
	DWORD col = FIntRN();

	if (IsValidID())
	{
		D3DFITEM* item = GetID();

//		if (item->MatSource != MATSOURCE_FRAME)
//		{
			item->MatSource = MATSOURCE_FRAME;
//			item->AktMat = true;
//		}

		if (col != item->Ambient)
		{
			item->Ambient = col;
			SetMaterial();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� barvy sv�tivosti

void FSetD3DEmissive()
{
	DWORD col = FIntRN();

	if (IsValidID())
	{
		D3DFITEM* item = GetID();

//		if (item->MatSource != MATSOURCE_FRAME)
//		{
			item->MatSource = MATSOURCE_FRAME;
//			item->AktMat = true;
//		}

		if (col != item->Emissive)
		{
			item->Emissive = col;
			SetMaterial();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� matnosti

void FSetD3DPower()
{
	double val = FNum();

	if (IsValidID())
	{
		if (val < 0) val = 0;
		if (val > 128) val = 128;

		D3DFITEM* item = GetID();

//		if (item->MatSource != MATSOURCE_FRAME)
//		{
			item->MatSource = MATSOURCE_FRAME;
//			item->AktMat = true;
//		}

		if (val != item->Power)
		{
			item->Power = val;
			SetMaterial();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� barvy odlesku

void FSetD3DSpecular()
{
	DWORD col = FIntRN();

	if (IsValidID())
	{
		D3DFITEM* item = GetID();

//		if (item->MatSource != MATSOURCE_FRAME)
//		{
			item->MatSource = MATSOURCE_FRAME;
//			item->AktMat = true;
//		}

		if (col != item->Specular)
		{
			item->Specular = col;
			SetMaterial();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� textury objektu Direct3D

void FSetD3DTexture()
{
	CPicture pic;
	FPicture(pic);

	if (IsValidID())
	{
		D3DFITEM* item = GetID();

		int texture = item->Texture[D3DAktStage];

		if (texture >= 0)
		{
			if (pic == D3DT_Get(texture)->Pic) return;
			D3DT_Del(texture);
			item->Texture[D3DAktStage] = -1;
//			item->AktMat = true;
		}

		if (pic.IsEmpty()) return;

//		item->AktMat = true;
		item->Texture[D3DAktStage] = D3DT_Add(pic);
	}
}

/////////////////////////////////////////////////////////////////////////////
// na�ten� textury objektu Direct3D ze souboru

void FD3DTextureFile()
{
	CPicture pic;
	pic.LoadFile(true);

	if (IsValidID())
	{
		D3DFITEM* item = GetID();

		int texture = item->Texture[D3DAktStage];

		if (texture >= 0)
		{
			if (pic == D3DT_Get(texture)->Pic) return;
			D3DT_Del(texture);
			item->Texture[D3DAktStage] = -1;
		}

		if (pic.IsEmpty()) return;

		item->Texture[D3DAktStage] = D3DT_Add(pic);
	}
}

/////////////////////////////////////////////////////////////////////////////
// mapov�n� textury

void FD3DTextMap()
{
	D3DTextMap(NULL);
}


// pro (item == NULL) se na��taj� parametry z programu pro aktu�ln� objekt
// pro (item != NULL) mus� b�t platn� polo�ka (item->AutoMap != NULL) !!!!

void D3DTextMap(D3DFITEM* item)
{
// lok�ln� prom�nn�
	int method;						// metoda mapov�n� (0=adresy, 1=vrcholy, 2=plo�ky)
	int type;						// typ mapov�n� (0=plocha, 1=v�lec, 2=koule, 3=chrom, 4=�o�ka)
	double ox;						// sou�adnice X po��tku mapov�n�
	double oy;						// sou�adnice Y po��tku mapov�n�
	double oz;						// sou�adnice Z po��tku mapov�n�
	double rz;						// rotace podle osy Z
	double rx;						// rotace podle osy X
	double ry;						// rotace podle osy Y
	double ou;						// po��tek textury horizont�ln�
	double ov;						// po��tek textury vertik�ln�
	double su;						// m���tko textury horizont�ln�
	double sv;						// m���tko textury vertik�ln�

	BOOL automap = (item != NULL);	//  je automatick� mapov�n�

// na�ten� parametr� z programu
	if (!automap)
	{
		method = FIntR();			// metoda mapov�n� (0=adresy, 1=vrcholy, 2=plo�ky)
		type = FIntR();				// typ mapov�n� (0=plocha, 1=v�lec, 2=koule, 3=chrom, 4=�o�ka)
		ox = FNum();				// sou�adnice X po��tku mapov�n�
		oy = FNum();				// sou�adnice Y po��tku mapov�n�
		oz = FNum();				// sou�adnice Z po��tku mapov�n�
		rz = FNum();				// rotace podle osy Z
		rx = FNum();				// rotace podle osy X
		ry = FNum();				// rotace podle osy Y
		ou = FNum();				// po��tek textury horizont�ln�
		ov = FNum();				// po��tek textury vertik�ln�
		su = FNum();				// m���tko textury horizont�ln�
		sv = FNum();				// m���tko textury vertik�ln�

#define EPS 0.001
#define EPS2 0.499

// korekce parametr�
		if ((DWORD)method > (DWORD)2) method = 0;
		if ((DWORD)type > (DWORD)4) type = 0;
		if ((su > -EPS) && (su < EPS)) su = 1;
		if ((sv > -EPS) && (sv < EPS)) sv = 1;

// adresa objektu
		if (!IsValidID()) return;

// odpojen� dat od klon�
		D3DF_Modi(D3D_ID);

		item = GetID();

// kontrola, zda je to platn� objekt
		if ((item->VertNum <= 0) || (item->Vertex == NULL) ||
			(item->FaceNum <= 0) || (item->Face == NULL) ||
			(item->Normal == NULL) ||
			(item->Type == D3DFTYPE_PICTURE) ||
			((method > 0) && (item->Texture[D3DAktStage] < 0))) return;

// korekce pro chrom a �o�ku
		if ((type == 3) || (type == 4))
		{
			if (type == 3)
			{
				su *= -2;
				ou += 0.5;
			}
			else
			{
				su *= 2;
			}

			ov++;

			item->AddressU[D3DAktStage] = D3DTADDRESS_WRAP;
			item->AddressV[D3DAktStage] = D3DTADDRESS_MIRROR;

// p��prava pro automatick� mapov�n� textur
			if (method == 0)
			{
				AUTOMAP* am = item->AutoMap;

				if (am == NULL)
				{
					am = (AUTOMAP*)MemGet(sizeof(AUTOMAP));
					item->AutoMap = am;
				}

				am->method = method;	// metoda mapov�n� (0=adresy, 1=vrcholy, 2=plo�ky)
				am->type = type;		// typ mapov�n� (0=plocha, 1=v�lec, 2=koule, 3=chrom, 4=�o�ka)
				am->ox = ox;			// sou�adnice X po��tku mapov�n�
				am->oy = oy;			// sou�adnice Y po��tku mapov�n�
				am->oz = oz;			// sou�adnice Z po��tku mapov�n�
				am->rz = rz;			// rotace podle osy Z
				am->rx = rx;			// rotace podle osy X
				am->ry = ry;			// rotace podle osy Y
				am->ou = ou;			// po��tek textury horizont�ln�
				am->ov = ov;			// po��tek textury vertik�ln�
				am->su = su;			// m���tko textury horizont�ln�
				am->sv = sv;			// m���tko textury vertik�ln�

				MemFill(&(am->m), sizeof(D3DMATRIX), 0);

				return;
			}	
		}

// jinak zru�en� p��padn�ho bufferu automatick�ho mapov�n�
		MemFree(item->AutoMap);
		item->AutoMap = NULL;
	}
	else

// na�ten� parametr� z objektu
	{
		AUTOMAP* am = item->AutoMap;

		method = am->method;	// metoda mapov�n� (0=adresy, 1=vrcholy, 2=plo�ky)
		type = am->type;		// typ mapov�n� (0=plocha, 1=v�lec, 2=koule, 3=chrom, 4=�o�ka)
		ox = am->ox;			// sou�adnice X po��tku mapov�n�
		oy = am->oy;			// sou�adnice Y po��tku mapov�n�
		oz = am->oz;			// sou�adnice Z po��tku mapov�n�
		rz = am->rz;			// rotace podle osy Z
		rx = am->rx;			// rotace podle osy X
		ry = am->ry;			// rotace podle osy Y
		ou = am->ou;			// po��tek textury horizont�ln�
		ov = am->ov;			// po��tek textury vertik�ln�
		su = am->su;			// m���tko textury horizont�ln�
		sv = am->sv;			// m���tko textury vertik�ln�
	}

// pro chrom a �o�ku p��prava vektoru pohledu
	if ((type == 3) || (type == 4))
	{
		D3DVECTOR posun;

		D3DFITEM* item2 = item;

		while (item2->Type == D3DFTYPE_SECTOR)	// mus� se pou��t origin�l sektor�
		{
			item2 = D3DF_Get(item2->Parent);
		}

		D3DMATRIX* m = &(item2->MatrixWorld);

		posun.x = (float)(D3DF_Get(1)->MatrixWorld._41 - m->_41);
		posun.y = (float)(D3DF_Get(1)->MatrixWorld._42 - m->_42);
		posun.z = (float)(D3DF_Get(1)->MatrixWorld._43 - m->_43);

// p�i automatick�m mapov�n� test, zda je zm�na
		if (automap)
		{
			D3DMATRIX* m2 = &(item->AutoMap->m);

			if ((m->_11 == m2->_11) &&
				(m->_12 == m2->_12) &&
				(m->_13 == m2->_13) &&
				(m->_21 == m2->_21) &&
				(m->_22 == m2->_22) &&
				(m->_23 == m2->_23) &&
				(m->_31 == m2->_31) &&
				(m->_32 == m2->_32) &&
				(m->_33 == m2->_33) &&
				(posun.x == m2->_41) &&
				(posun.y == m2->_42) &&
				(posun.z == m2->_43))
			{
				return;
			}
			
			MemCopy(m2, m, sizeof(D3DMATRIX));

			m2->_41 = posun.x;
			m2->_42 = posun.y;
			m2->_43 = posun.z;
		}

// korekce pro chrom a �o�ku
		if (type == 3)
		{
			ry += atan2(-posun.x, -posun.z)/2;
		}
		else
		{
			ry += atan2(-posun.x, -posun.z)*1.5;
			ox -= posun.x/8;
			oz -= posun.z/8;
		}
	}

// aktualizovat sektory
	item->AktSector = TRUE;
	D3DF_AktMatLink(item);

// vytvo�en� buffer� mapov�n� barvy a mapov�n� textury
	if (method == 0)
	{
		if(item->TextUV[D3DAktStage] == NULL)
		{
			item->TextUV[D3DAktStage] = (float*)MemGet(item->VertNum * 2 * sizeof(float));
		}
	}
	else
	{
		if (item->VertCol == NULL)
		{
			item->VertCol = (DWORD*)MemGet(item->VertNum * sizeof(DWORD));
		}

		if (item->VertColF == NULL)
		{
			item->VertColF = (D3DCOLORVALUE*)MemGet(item->VertNum * sizeof(D3DCOLORVALUE));
		}
		item->AutoCol = MAXDWORD;
	}

// resetov�n� objektu
	pD3Reset(item);
		
// inicializace mana�eru vrchol�
	D3DF_UsedInit(item->Index);

// obr�zek s barvami
	int width = 0;
	int height = 0;
	BYTE* data = NULL;

	if (method > 0)
	{
		D3DT_Get(item->Texture[D3DAktStage])->Pic.InitTexture(D3DAktStage);
		PICTUREDATA* picdata = D3DT_Get(item->Texture[D3DAktStage])->Pic.Data();
		width = picdata->TextWidth;
		height = picdata->TextHeight;
		data = picdata->TextData;
	}

// p��prava transforma�n� matice (zp�tn� po�ad� rotac� Y-X-Z)
	float siny = (float)sin(-ry);
	float cosy = (float)cos(ry);
	float sinx = (float)sin(-rx);
	float cosx = (float)cos(rx);
	float sinz = (float)sin(-rz);
	float cosz = (float)cos(rz);

	D3DMATRIX m;
	Matrix1(&m);
	m._11 = cosy;
	m._13 = -siny;
	m._31 = siny;
	m._33 = cosy;

	D3DMATRIX m2;
	Matrix1(&m2);
	m2._22 = cosx;
	m2._23 = sinx;
	m2._32 = -sinx;
	m2._33 = cosx;

	D3DMATRIX m3;
	MatrixMul(&m3, &m, &m2);

	Matrix1(&m2);
	m2._11 = cosz;
	m2._12 = sinz;
	m2._21 = -sinz;
	m2._22 = cosz;

	MatrixMul(&m, &m3, &m2);

	D3DMATRIX* m0 = &(item->MatrixWorld);

// cyklus p�es v�echny plo�ky
	int faceinx = 0;

	double kx = 0;
	double ky = 0;
	double kz = 0;
	double tx = 0;
	double ty = 0;
	double tz = 0;

	if ((type == 3) || (type == 4))
	{
		D3DFITEM* item2 = item;

		if (item2->Type == D3DFTYPE_SECTOR)
		{
			tx = item2->TransX;
			ty = item2->TransY;
			tz = item2->TransZ;

			while (item2->Type == D3DFTYPE_SECTOR)	// mus� se pou��t origin�l sektor�
			{
				item2 = D3DF_Get(item2->Parent);
			}
		}

		kx = item2->ScaleX*0.25/item2->BoundR;
		ky = item2->ScaleY*0.25/item2->BoundR;
		kz = item2->ScaleZ*0.25/item2->BoundR;
	}

	for (int faces = item->FaceNum; faces > 0; faces--)
	{

// indexy vektor� plo�ky
		int* f = item->Face;
		int inx1 = f[faceinx];
		faceinx++;
		int inx2 = f[faceinx];
		faceinx++;
		int inx3 = f[faceinx];
		faceinx++;

// sou�adnice vrchol� plo�ky
		D3DVECTOR* v = item->Vertex;

		if ((type == 3) || (type == 4)) v = item->Normal;

		double dx = v[inx1].x;
		double dy = v[inx1].y;
		double dz = v[inx1].z;

		if ((type == 3) || (type == 4))
		{
			D3DVECTOR* v0 = item->Vertex + inx1;
			dx += (v0->x + tx) * kx;
			dy += (v0->y + ty) * ky;
			dz += (v0->z + tz) * kz;

			double dx2 = dx*m0->_11 + dy*m0->_21 + dz*m0->_31;
			double dy2 = dx*m0->_12 + dy*m0->_22 + dz*m0->_32;
			double dz2 = dx*m0->_13 + dy*m0->_23 + dz*m0->_33;

			dx = dx2;
			dy = dy2;
			dz = dz2;
		}

		dx = dx - ox;
		dy = dy - oy;
		dz = dz - oz;

		double x1 = (dx*m._11 + dy*m._21 + dz*m._31);
		double y1 = (dx*m._12 + dy*m._22 + dz*m._32);
		double z1 = (dx*m._13 + dy*m._23 + dz*m._33);


		dx = v[inx2].x;
		dy = v[inx2].y;
		dz = v[inx2].z;

		if ((type == 3) || (type == 4))
		{
			D3DVECTOR* v0 = item->Vertex + inx2;
			dx += (v0->x + tx) * kx;
			dy += (v0->y + ty) * ky;
			dz += (v0->z + tz) * kz;

			double dx2 = dx*m0->_11 + dy*m0->_21 + dz*m0->_31;
			double dy2 = dx*m0->_12 + dy*m0->_22 + dz*m0->_32;
			double dz2 = dx*m0->_13 + dy*m0->_23 + dz*m0->_33;

			dx = dx2;
			dy = dy2;
			dz = dz2;
		}

		dx = dx - ox;
		dy = dy - oy;
		dz = dz - oz;

		double x2 = (dx*m._11 + dy*m._21 + dz*m._31);
		double y2 = (dx*m._12 + dy*m._22 + dz*m._32);
		double z2 = (dx*m._13 + dy*m._23 + dz*m._33);


		dx = v[inx3].x;
		dy = v[inx3].y;
		dz = v[inx3].z;

		if ((type == 3) || (type == 4))
		{
			D3DVECTOR* v0 = item->Vertex + inx3;
			dx += (v0->x + tx) * kx;
			dy += (v0->y + ty) * ky;
			dz += (v0->z + tz) * kz;

			double dx2 = dx*m0->_11 + dy*m0->_21 + dz*m0->_31;
			double dy2 = dx*m0->_12 + dy*m0->_22 + dz*m0->_32;
			double dz2 = dx*m0->_13 + dy*m0->_23 + dz*m0->_33;

			dx = dx2;
			dy = dy2;
			dz = dz2;
		}

		dx = dx - ox;
		dy = dy - oy;
		dz = dz - oz;

		double x3 = (dx*m._11 + dy*m._21 + dz*m._31);
		double y3 = (dx*m._12 + dy*m._22 + dz*m._32);
		double z3 = (dx*m._13 + dy*m._23 + dz*m._33);

// mapovac� sou�adnice vrchol�
		double tu1 = x1;
		double tv1 = y1;

		double tu2 = x2;
		double tv2 = y2;

		double tu3 = x3;
		double tv3 = y3;

// p�emapov�n�
		switch(type)
		{

// �o�ka
		case 4:

// chrom
		case 3:

// koule
		case 2:
			tv1 = atan2(y1, sqrt(z1*z1 + x1*x1))/pi;
			if (tv1 < -EPS2) tv1 = -EPS2;
			if (tv1 > EPS2) tv1 = EPS2;

			tv2 = atan2(y2, sqrt(z2*z2 + x2*x2))/pi;
			if (tv2 < -EPS2) tv2 = -EPS2;
			if (tv2 > EPS2) tv2 = EPS2;

			tv3 = atan2(y3, sqrt(z3*z3 + x3*x3))/pi;
			if (tv3 < -EPS2) tv3 = -EPS2;
			if (tv3 > EPS2) tv3 = EPS2;

		// pokra�uje p�emapov�n� sou�adnice u
// v�lec
		case 1:
			tu1 = atan2(x1, -z1)/pi2;
			if (tu1 < -EPS2) tu1 = -EPS2;
			if (tu1 > EPS2) tu1 = EPS2;

			tu2 = atan2(x2, -z2)/pi2;
			if (tu2 < -EPS2) tu2 = -EPS2;
			if (tu2 > EPS2) tu2 = EPS2;

			tu3 = atan2(x3, -z3)/pi2;
			if (tu3 < -EPS2) tu3 = -EPS2;
			if (tu3 > EPS2) tu3 = EPS2;

			if ((fabs(x1)+fabs(z1)) < EPS) tu1 = (tu2 + tu3)/2;
			if ((fabs(x2)+fabs(z2)) < EPS) tu2 = (tu1 + tu3)/2;
			if ((fabs(x3)+fabs(z3)) < EPS) tu3 = (tu1 + tu2)/2;

			if (fabs(tu2 - tu1) > 0.5)
			{
				if (tu2 < 0.5) tu2 += 1; else tu1 += 1;
			}

			if (fabs(tu3 - tu1) > 0.5)
			{
				if (tu3 < 0.5) tu3 += 1; else tu1 += 1;
			}

			if (fabs(tu3 - tu2) > 0.5)
			{
				if (tu3 < 0.5) tu3 += 1; else tu2 += 1;
			}

			if ((fabs(x1)+fabs(z1)) < EPS) tu1 = (tu2 + tu3)/2;
			if ((fabs(x2)+fabs(z2)) < EPS) tu2 = (tu1 + tu3)/2;
			if ((fabs(x3)+fabs(z3)) < EPS) tu3 = (tu1 + tu2)/2;
			break;
		}

// korekce m���tka a po��tku textury
		tu1 = tu1*su - ou;
		tv1 = ov - tv1*sv;

		tu2 = tu2*su - ou;
		tv2 = ov - tv2*sv;

		tu3 = tu3*su - ou;
		tv3 = ov - tv3*sv;

		switch (method)
		{

// mapov�n� vrchol�
		case 1:
			{
		// vrchol 1
				while (tu1 > 1) tu1--;
				while (tu1 < 0) tu1++;
				while (tv1 > 1) tv1--;
				while (tv1 < 0) tv1++;

				int xx1 = Round0(tu1*width);
				if (xx1 >= width) xx1 = width-1;

				int yy1 = Round0(tv1*height);
				if (yy1 >= height) yy1 = height-1;

				int n1 = 4*(xx1 + yy1*width);
				BYTE b1 = data[n1 + 0];
				BYTE g1 = data[n1 + 1];
				BYTE r1 = data[n1 + 2];
				BYTE a1 = data[n1 + 3];

				D3DF_UsedColorB(faceinx - 3, r1, g1, b1, a1);

		// vrchol 2
				while (tu2 > 1) tu2--;
				while (tu2 < 0) tu2++;
				while (tv2 > 1) tv2--;
				while (tv2 < 0) tv2++;

				int xx2 = Round0(tu2*width);
				if (xx2 >= width) xx2 = width-1;

				int yy2 = Round0(tv2*height);
				if (yy2 >= height) yy2 = height-1;

				int n2 = 4*(xx2 + yy2*width);
				BYTE b2 = data[n2 + 0];
				BYTE g2 = data[n2 + 1];
				BYTE r2 = data[n2 + 2];
				BYTE a2 = data[n2 + 3];

				D3DF_UsedColorB(faceinx - 2, r2, g2, b2, a2);

		// vrchol 3
				while (tu3 > 1) tu3--;
				while (tu3 < 0) tu3++;
				while (tv3 > 1) tv3--;
				while (tv3 < 0) tv3++;

				int xx3 = Round0(tu3*width);
				if (xx3 >= width) xx3 = width-1;

				int yy3 = Round0(tv3*height);
				if (yy3 >= height) yy3 = height-1;

				int n3 = 4*(xx3 + yy3*width);
				BYTE b3 = data[n3 + 0];
				BYTE g3 = data[n3 + 1];
				BYTE r3 = data[n3 + 2];
				BYTE a3 = data[n3 + 3];

				D3DF_UsedColorB(faceinx - 1, r3, g3, b3, a3);
			}
			break;

// mapov�n� plo�ek
		case 2:
			{
				double tu = (tu1 + tu2 + tu3)/3;
				double tv = (tv1 + tv2 + tv3)/3;

				while (tu > 1) tu--;
				while (tu < 0) tu++;
				while (tv > 1) tv--;
				while (tv < 0) tv++;

				int xx = Round0(tu*width);
				if (xx >= width) xx = width-1;

				int yy = Round0(tv*height);
				if (yy >= height) yy = height-1;

				int n = 4*(xx + yy*width);
				BYTE b = data[n + 0];
				BYTE g = data[n + 1];
				BYTE r = data[n + 2];
				BYTE a = data[n + 3];

				D3DF_UsedColorB(faceinx - 3, r, g, b, a);
				D3DF_UsedColorB(faceinx - 2, r, g, b, a);
				D3DF_UsedColorB(faceinx - 1, r, g, b, a);
			}
			break;

// adresy textury
		default:
			D3DF_UsedTextUV(faceinx - 3, (float)tu1, (float)tv1, D3DAktStage);
			D3DF_UsedTextUV(faceinx - 2, (float)tu2, (float)tv2, D3DAktStage);
			D3DF_UsedTextUV(faceinx - 1, (float)tu3, (float)tv3, D3DAktStage);
		}
	}

	if (method > 0)
	{

// zru�en� textury
		D3DT_Del(item->Texture[D3DAktStage]);
		item->Texture[D3DAktStage] = -1;
		item->MatSource = MATSOURCE_OBJECT;
	}
	else
	{

// nastaven� m�du adresov�n�
		if (!automap)
		{
			if (item->AddressU[D3DAktStage] == D3DTADDRESS_CLAMP) item->AddressU[D3DAktStage] = D3DTADDRESS_WRAP;
			if (item->AddressV[D3DAktStage] == D3DTADDRESS_CLAMP) item->AddressV[D3DAktStage] = D3DTADDRESS_WRAP;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� parametr� sv�tla

void SetLight(D3DLITEM* item)
{
	D3DLIGHT8* light = &(item->Light);

// nastaven� difusn� barvy
	BGRFloat(item->Diffuse, &(light->Diffuse));

// nastaven� �hlu sv�teln�ho ku�ele a polost�nu
	double theta = item->Theta;
	double phi = item->Phi;
	if (theta > phi) phi = theta;
	light->Theta = (float)theta;
	light->Phi = (float)phi;
	light->Falloff = 1;

// nastaven� �tlumu
	light->Attenuation0 = 1;
	light->Attenuation1 = (float)(item->Attenuation1);
	light->Attenuation2 = (float)(item->Attenuation2);

// korekce intenzity sv�tla
	double n = item->Intens;
	light->Diffuse.r = (float)(light->Diffuse.r * n);	
	light->Diffuse.g = (float)(light->Diffuse.g * n);	
	light->Diffuse.b = (float)(light->Diffuse.b * n);	
	light->Diffuse.a = (float)(light->Diffuse.a * n);	

	light->Specular = light->Diffuse;

// po�adavek aktualizace sv�tla
	item->Akt = true;
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� barvy sv�tla

void FSetD3DLightColor()
{
	DWORD col = FIntRN();

	if (IsValidID())
	{
		D3DFITEM* item = GetID();

		if (item->Type == D3DFTYPE_LIGHT)
		{
			D3DLITEM* light = D3DL_Get(item->Light);

			if (col != light->Diffuse)
			{
				light->Diffuse = col;
				SetLight(light);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� typu sv�tla

void FSetD3DLightType()
{
	int type = FIntR();

	if (IsValidID())
	{
		D3DFITEM* item = GetID();

		if (item->Type == D3DFTYPE_LIGHT)
		{
			if ((type == D3DLIGHT_POINT) ||
				(type == D3DLIGHT_SPOT) ||
				(type == D3DLIGHT_DIRECTIONAL))
			{
				D3DLITEM* light = D3DL_Get(item->Light);

				if (type != light->Light.Type)
				{
					light->Light.Type = (D3DLIGHTTYPE)type;
					light->Akt = true;
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� �hlu ku�ele sv�tla

void FSetD3DLightUmbra()
{
	double val = FNum();

	if (IsValidID())
	{
		D3DFITEM* item = GetID();

		if (item->Type == D3DFTYPE_LIGHT)
		{
			if (val < 0) val = 0;
			if (val > pi) val = pi;

			D3DLITEM* light = D3DL_Get(item->Light);

			if (val != light->Theta)
			{
				light->Theta = val;
				SetLight(light);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� �hlu polost�nu sv�tla

void FSetD3DLightPenumbra()
{
	double val = FNum();

	if (IsValidID())
	{
		D3DFITEM* item = GetID();

		if (item->Type == D3DFTYPE_LIGHT)
		{
			if (val < 0) val = 0;
			if (val > pi) val = pi;

			D3DLITEM* light = D3DL_Get(item->Light);

			if (val != light->Phi)
			{
				light->Phi = val;
				SetLight(light);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� dosahu sv�tla

void FSetD3DLightRange()
{
	double val = FNum();

	if (IsValidID())
	{
		D3DFITEM* item = GetID();

		if (item->Type == D3DFTYPE_LIGHT)
		{
			if (val < 0) val = 0;

			D3DLITEM* light = D3DL_Get(item->Light);

			if (val != light->Range)
			{
				light->Range = val;
				light->Light.Range = (float)val;
				light->Akt = true;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� intenzity sv�tla

void FSetD3DLightCatten()
{
	double val = FNum();

	if (IsValidID())
	{
		D3DFITEM* item = GetID();

		if (item->Type == D3DFTYPE_LIGHT)
		{
			D3DLITEM* light = D3DL_Get(item->Light);

			if (val != light->Intens)
			{
				light->Intens = val;
				SetLight(light);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� line�rn�ho �tlumu sv�tla

void FSetD3DLightLatten()
{	 
	double val = FNum();

	if (IsValidID())
	{
		D3DFITEM* item = GetID();

		if (item->Type == D3DFTYPE_LIGHT)
		{
			if (val < 0) val = 0;

			D3DLITEM* light = D3DL_Get(item->Light);

			if (val != light->Attenuation1)
			{
				light->Attenuation1 = val;
				SetLight(light);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� kvadratick�ho �tlumu sv�tla

void FSetD3DLightQatten()
{
	double val = FNum();

	if (IsValidID())
	{
		D3DFITEM* item = GetID();

		if (item->Type == D3DFTYPE_LIGHT)
		{
			if (val < 0) val = 0;

			D3DLITEM* light = D3DL_Get(item->Light);

			if (val != light->Attenuation2)
			{
				light->Attenuation2 = val;
				SetLight(light);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� projekce

void FSetD3DProjection()
{
	int n = FIntR();

	if (((DWORD)n < (DWORD)4) && (n != D3DProjection))
	{
		D3DProjection = n;
		D3DProjAkt = true;
		D3DRightHand = (n >= 2);
	}
}

/////////////////////////////////////////////////////////////////////////////
// p�ipojen� jin�ho objektu (Data = po�et prvk� - 1)

void FD3DLink()
{
// p��prava po�tu prvk�
	int i = ExecItem[-1].Data + 1;

// aktivn� prvek se m��e m�nit (p�ipojov�n� vytv��en�ch objekt�)
	int d3did = D3D_ID;

// p�ipojen� v�ech prvk�
	for (; i > 0; i--)
	{
		int inx = FIntR();

		if (D3DF_IsValid(d3did) &&
			(D3DF_Get(d3did)->Type != D3DFTYPE_PICTURE) &&
			D3DF_IsValid(inx) &&
			(D3DF_Get(inx)->Type != D3DFTYPE_PICTURE))
		{
			D3DF_Link(d3did, inx);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// ulo�en� objektu do souboru

//void FD3DSave()
//{
	/*
	CString text;
	FText(text);

	if (D3DF.IsValid(D3D_ID))
	{
		D3DFITEM* frame = &(D3DF[D3D_ID]);

		if (D3DO.IsValid(frame->Obj))
		{
			D3DOITEM* mesh = &(D3DO[frame->Obj]);

			if (mesh->Obj != NULL)
			{
				mesh->Obj->Save((LPCTSTR)text, D3DRMXOF_TEXT, D3DRMXOFSAVE_NORMALS |
											D3DRMXOFSAVE_TEXTURECOORDINATES | 
											D3DRMXOFSAVE_MATERIALS |
											D3DRMXOFSAVE_TEXTURETOPOLOGY);
			}
		}
	}
	*/
//}

/////////////////////////////////////////////////////////////////////////////
// nastaven� barvy pozad� sc�ny

void FSetD3DSceneColor()
{
	DWORD col = FIntRN();

	if (D3DBackText >= 0)
	{
		D3DT_Del(D3DBackText);
		D3DBackText = -1;
		D3DBackTextAkt = true;
	}

	if (col != D3DBackCol)
	{
		D3DBackCol = col;
		if ((col == MAXDWORD) || (col == (MAXDWORD-1))) col = 0;

		BGRFloat(col, &D3DBackColF);
		D3DBackColRGB = XRGB(col);
		D3DBackColAkt = true;
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� textury pozad� sc�ny

void FSetD3DSceneTexture()
{
	CPicture pic;
	FPicture(pic);

	if (D3DBackText >= 0)
	{
		if (pic == D3DT_Get(D3DBackText)->Pic) return;
		D3DT_Del(D3DBackText);
		D3DBackText = -1;
		D3DBackTextAkt = true;
	}

	if (pic.IsEmpty()) return;

	D3DBackText = D3DT_Add(pic);
	D3DBackTextAkt = true;
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� ambient sv�tla

void FSetD3DAmbient()
{
	DWORD col = FIntRN();

	if (col != D3DAmbientCol)
	{
		D3DAmbientCol = col;

		BGRFloat(col, &D3DAmbientColF);

		D3DAmbientColRGB = XRGB(col);

		D3DAmbientColAkt = true;
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� barvy mlhy

void SetFogCol(DWORD col)
{
	FogColor = col;

	if (col == MAXDWORD) col = 0;

	BGRFloat(col, &FogColorF);

	FogColorRGB = XRGB(col);

	FogAkt = true;

	D3DCheckFogOn();
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� barvy mlhy

void FSetD3DFogColor()
{
	DWORD col = FIntRN();

	if (col != FogColor)
	{
		SetFogCol(col);
	}
}

/////////////////////////////////////////////////////////////////////////////
// zapnut� mlhy

void FFogOn()
{
	if (FogColor == MAXDWORD)
	{
		DWORD c = D3DBackCol;
		if (c == MAXDWORD) c = 0;
		SetFogCol(c);
	}
	FogAkt = true;
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� m�du mlhy

void FSetD3DFogMode()
{
	int n = FIntR();
	bool korig = ((n & 4) != 0);
	n = n & ~4;

	if ((DWORD)n > 2)
	{
		n = 1;
		korig = false;
	}

	if ((n != FogType) || (korig != FogKorig) || (FogColor == MAXDWORD))
	{
		FogType = n;
		FogKorig = korig;
		FFogOn();
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� za��tku mlhy

void FSetD3DFogStart()
{
	double n = FNum();

	if ((n != FogStart) || (FogColor == MAXDWORD))
	{
		FogStart = n;
		FFogOn();
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� konce mlhy

void FSetD3DFogEnd()
{
	double n = FNum();

	if ((n != FogEnd) || (FogColor == MAXDWORD))
	{
		FogEnd = n;
		FFogOn();
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� hustoty mlhy

void FSetD3DFogDensity()
{
	double n = FNum();

	if ((n != FogDens) || (FogColor == MAXDWORD))
	{
		FogDens = n;
		FFogOn();
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� prom�tac� roviny

void FSetD3DViewFront()
{
	double n = FNum();
	
	if (n != D3DFrontClip)
	{
		D3DFrontClip = n;
		D3DProjAkt = true;
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� vzd�lenosti dohledu

void FSetD3DViewBack()
{
	double n = FNum();
	
	if (n != D3DBackClip)
	{
		D3DBackClip = n;
		D3DProjAkt = true;
	}
}

/////////////////////////////////////////////////////////////////////////////
// tvo�it horn� podstavu

void FSetD3DUpper()
{
	D3DCreateUpper = FBool();
}

/////////////////////////////////////////////////////////////////////////////
// tvo�it doln� podstavu

void FSetD3DLower()
{
	D3DCreateLower = FBool();
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� slo�itosti objekt�

void FSetD3DSplits()
{
	D3DLevel = FIntR();
	if (D3DLevel < 2) D3DLevel = 2;
	if (D3DLevel > 200) D3DLevel = 200;
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� ovlada�e

void FSetD3DDriver()
{
	int driver = FIntR();

	if ((DWORD)driver >= (DWORD)D3DEVICES)
	{
		driver = 0;
	}

	if (driver != D3DDevUser)
	{
		D3DDevUser = driver;

		if ((driver != D3DDevAkt) || (driver == 0))
		{
			if (D3D || RecalcD3D()) UserD3DDevice();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� rozhran�

void FSetD3DInterface()
{
	int inter = FIntR();

	if ((DWORD)inter >= (DWORD)D3INTERFACES)
	{
		inter = 0;
	}

	if (inter != D3DIntUser)
	{
		D3DIntUser = inter;

		if ((inter != D3DIntAkt) || (inter == 0))
		{
			if (D3D || RecalcD3D()) UserD3DDevice();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� vyhlazen� textur

void FSetD3DSmooth()
{
	D3DSmooth = FNum();
	if (D3DSmooth < 1) D3DSmooth = 1;
	if (D3DSmooth > 1000) D3DSmooth = 1000;
}

/////////////////////////////////////////////////////////////////////////////
// normalizace objektu

void FD3DNorm()
{

// popisova� objektu
	if (IsValidID())
	{
// odpojen� dat od klon�
		D3DF_Modi(D3D_ID);

		D3DFITEM* item = GetID();

		int vn = item->VertNum;
		D3DVECTOR* vp = item->Vertex;

		if ((vn > 0) && (vp != NULL))
		{
			item->ShadowPoint.x = 0;
			item->ShadowPoint.y = 0;
			item->ShadowPoint.z = 0;

// aktualizovat sektory
			item->AktSector = TRUE;
			D3DF_AktMatLink(item);

// p��prava prom�nn�ch - hranice
			double minx = 0;
			double maxx = 0;
			double miny = 0;
			double maxy = 0;
			double minz = 0;
			double maxz = 0;

// zji�t�n� rozm�r� objektu
			for (int i = vn; i > 0; i--)
			{
				double x = vp->x;
				if (x < minx) minx = x;
				if (x > maxx) maxx = x;

				double y = vp->y;
				if (y < miny) miny = y;
				if (y > maxy) maxy = y;

				float z = vp->z;
				if (z < minz) minz = z;
				if (z > maxz) maxz = z;

				vp++;
			}

// v�po�et normaliza�n�ch koeficient�
			double d = maxx - minx;
			double a = maxy - miny;
			if (a > d) d = a;
			a = maxz - minz;
			if (a > d) d = a;
			if (d == 0) d = 1;
			d = 1/d;

			double sx = -(minx + maxx)/2;
			double sy = -(miny + maxy)/2;
			double sz = -(minz + maxz)/2;

// normalizace objektu (pokud je n�jak� zm�na)
			if ((d < 0.9999) || (d > 1.0001) ||
				(sx > 0.0001) || (sx < -0.0001) ||
				(sy > 0.0001) || (sy < -0.0001) ||
				(sz > 0.0001) || (sz < -0.0001))
			{
// zru�en� buffer�
				pD3Reset(item);

// normalizace vrchol�
				vp = item->Vertex;

				for (int i = vn; i > 0; i--)
				{
					vp->x = (float)((vp->x + sx)*d);
					vp->y = (float)((vp->y + sy)*d);
					vp->z = (float)((vp->z + sz)*d);

					vp++;
				}

// p�enesen� zm�ny i na potomky
				int inx = item->Child;

				while (inx >= 0)
				{
					D3DFITEM* item = D3DF_Get(inx);

					item->TransX += sx;
					item->TransX *= d;
					item->TransY += sy;
					item->TransY *= d;
					item->TransZ += sz;
					item->TransZ *= d;

					item->ScaleX *= d;
					item->ScaleY *= d;
					item->ScaleZ *= d;
					item->ScaleB *= d;

					item->AktOwn = true;
					D3DF_AktMatLink(item);
					inx = item->Next;
				}

// aktualizace hranice
				D3DF_MapBound(D3D_ID);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� aktivn�ho stupn� textur

void FSetD3DStage()
{
	int stage = FIntR();
	if ((DWORD)stage < (DWORD)MAX_STAGES)
	{
		D3DAktStage = stage;
	}
}

/////////////////////////////////////////////////////////////////////////////
// korekce texturov�ch operac�

void KorStages(D3DFITEM* item)
{
	int stages = MAX_STAGES;			// po�et stup��

	for (int i = 0; i < MAX_STAGES; i++)
	{

// z�kaz stupn� p�i p�ekro�en� po�tu stup��
		int colorop = item->ColorOp[i];
		if (i >= stages)
		{
			colorop = D3DTOP_DISABLE;
			item->ColorOp[i] = colorop;
		}
		else
		{
			if (colorop == D3DTOP_DISABLE) stages = i;
		}

// povolen� alfa operace, je-li povolena operace barvy
		int alphaop = item->AlphaOp[i];
		if ((colorop != D3DTOP_DISABLE) && (alphaop == D3DTOP_DISABLE))
		{
			alphaop = D3DTOP_MODULATE;
			item->AlphaOp[i] = alphaop;
			item->AlphaArg1[i] = D3DTA_TEXTURE;
			item->AlphaArg2[i] = D3DTA_CURRENT;
		}

// z�kaz alfa operace, je-li zak�z�na operace barvy
		if (colorop == D3DTOP_DISABLE)
		{
			item->ColorArg1[i] = D3DTA_TEXTURE;
			item->ColorArg2[i] = D3DTA_CURRENT;
			alphaop = D3DTOP_DISABLE;
			item->AlphaOp[i] = alphaop;
			item->AlphaArg1[i] = D3DTA_TEXTURE;
			item->AlphaArg2[i] = D3DTA_CURRENT;
		}

// argumenty operac�
		int colorarg1 = item->ColorArg1[i] & 0xf;
		int colorarg2 = item->ColorArg2[i] & 0xf;
		int alphaarg1 = item->AlphaArg1[i] & 0xf;
		int alphaarg2 = item->AlphaArg2[i] & 0xf;

// test, zda je operace vyu��vaj�c� texturu
		item->UseText[i] = ((colorop != D3DTOP_DISABLE) && (
							((colorarg1 == D3DTA_TEXTURE) && (colorop != D3DTOP_SELECTARG2)) ||
							((colorarg2 == D3DTA_TEXTURE) && (colorop != D3DTOP_SELECTARG1)) ||
							((alphaarg1 == D3DTA_TEXTURE) && (alphaop != D3DTOP_SELECTARG2)) ||
							((alphaarg2 == D3DTA_TEXTURE) && (alphaop != D3DTOP_SELECTARG1)) ||
							(colorop == D3DTOP_BLENDTEXTUREALPHA) ||
							(colorop == D3DTOP_BLENDTEXTUREALPHAPM) ||
							(alphaop == D3DTOP_BLENDTEXTUREALPHA) ||
							(alphaop == D3DTOP_BLENDTEXTUREALPHAPM)));
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� operace barev

void FSetD3DColorOp()
{
	int n = FIntR();
	if ((n > 0) && (n < 32))
	{
		if (IsValidID())
		{
			D3DFITEM* item = GetID();
			item->ColorOp[D3DAktStage] = n;
			KorStages(item);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� argumentu 1 barev

void FSetD3DColorArg1()
{
	int n = FIntR();
	if (IsValidID())
	{
		D3DFITEM* item = GetID();
		item->ColorArg1[D3DAktStage] = n;
		KorStages(item);
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� argumentu 2 barev

void FSetD3DColorArg2()
{
	int n = FIntR();
	if (IsValidID())
	{
		D3DFITEM* item = GetID();
		item->ColorArg2[D3DAktStage] = n;
		KorStages(item);
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� operace alfa

void FSetD3DAlphaOp()
{
	int n = FIntR();
	if ((n > 0) && (n < 32))
	{
		if (IsValidID())
		{
			D3DFITEM* item = GetID();
			item->AlphaOp[D3DAktStage] = n;
			KorStages(item);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� argumentu 1 alfa

void FSetD3DAlphaArg1()
{
	int n = FIntR();
	if (IsValidID())
	{
		D3DFITEM* item = GetID();
		item->AlphaArg1[D3DAktStage] = n;
		KorStages(item);
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� argumentu 2 alfa

void FSetD3DAlphaArg2()
{
	int n = FIntR();
	if (IsValidID())
	{
		D3DFITEM* item = GetID();
		item->AlphaArg2[D3DAktStage] = n;
		KorStages(item);
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� horizont�ln�ho adresov�n�

void FSetD3DAddressU()
{
	int n = FIntR();
	if ((n > 0) && (n < 8))
	{
		if (IsValidID())
		{
			GetID()->AddressU[D3DAktStage] = n;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� vertik�ln�ho adresov�n�

void FSetD3DAddressV()
{
	int n = FIntR();
	if ((n > 0) && (n < 8))
	{
		if (IsValidID())
		{
			GetID()->AddressV[D3DAktStage] = n;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� barvy okol� textury
/*
void FSetD3DBorder()
{
	int n = FIntR();

	if (n < 0)
	{
		n = 0;
	}
	else
	{
		n = n & 0xffffff;
	}

	if (IsValidID())
	{
		D3DFITEM* item = GetID();
		item->Border[D3DAktStage] = n;
		BGRFloat(n, &(item->BorderF[D3DAktStage]));
		item->BorderRGB[D3DAktStage] = (XRGB(n) | 0xff000000);
	}
}
*/
/////////////////////////////////////////////////////////////////////////////
// nastaven� zjemn�n� vzd�len�ch textur

void FSetD3DBias()
{
	double n = FNum();

	if (IsValidID())
	{
		GetID()->Bias[D3DAktStage] = (float)n;
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� barvy faktoru textur

void FSetD3DTFactor()
{
	DWORD n = FIntRN();

	if (IsValidID())
	{
		D3DFITEM* item = GetID();
		item->TFactor = n;
		BGRFloat(n, &(item->TFactorF));
		item->TFactorRGB = XRGB(n);
	}
}

/////////////////////////////////////////////////////////////////////////////
// po�et �rovn� mipmaps

void FSetD3DMipMaps()
{
	int n = FIntR();
	if (n < 0) n = 0;

	if (IsValidID())
	{
		D3DFITEM* item = GetID();

		int text = item->Texture[D3DAktStage];

		if (D3DT_IsValid(text))
		{
			D3DTITEM* t = D3DT_Get(text);

			if (n != t->MipMaps)
			{
				t->MipMaps = n;
				FPUFloat();
				pD3TextureRes(t);
				FPUDouble();
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� renderovac� skupiny

void FSetD3DZGroup()
{
	int n = FIntR();
	if (n < 0) n = 0;
	if (n >= MAX_RENDGROUP) n = MAX_RENDGROUP-1;

	if (IsValidID())
	{
		GetID()->RendGroup = n;
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� t��d�n�

void FSetD3DZSort()
{
	bool n = FBool();

	if (IsValidID())
	{
		RendSort[GetID()->RendGroup] = n;
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� hloubkov�ho z�pisu

void FSetD3DZWrite()
{
	bool n = FBool();

	if (IsValidID())
	{
		GetID()->ZWrite = n;
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� hloubkov�ho testu

void FSetD3DZTest()
{
	int n = FIntR();
	if ((n >= 1) && (n <= 8))
	{
		if (IsValidID())
		{
			GetID()->ZTest = n;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� dosahu st�nu

void FSetD3DShadowRange()
{
	double n = FNum();

	if (IsValidID())
	{
		if (n < 0) n = 0;
		bool s = (n > 0);

		int i = D3D_ID;
		int i0 = i;

		do 
		{
			D3DFITEM* item = D3DF_Get(i);

			if (n != item->ShadowRange)
			{
				item->ShadowRange = n;
				item->IsShadow = s;
				item->ShadowPoint.x = 0;
				item->ShadowPoint.y = 0;
				item->ShadowPoint.z = 0;
			}
			item->ShadowParent = (i != i0);

			if (item->Child >= 0)
			{
				i = item->Child;
			}
			else
			{
				if (i == i0) break;

				i = item->Next;

				while (i < 0)
				{
					i = item->Parent;

					if (i == i0) break;

					item = D3DF_Get(i);

					i = item->Next;
				}
			}

		} while (i != i0);
	}
}

/////////////////////////////////////////////////////////////////////////////
// generov�n� norm�l

void FD3DNormals()
{
	double angle = FNum();
	D3DF_AutoNormals(D3D_ID, angle);
}

void FD3DNormals0()
{
	D3DF_AutoNormals(D3D_ID, 0.5);
}

/////////////////////////////////////////////////////////////////////////////
// renderov�n� bez zobrazen�

void FD3DRender()
{
	D3Render(false);
}


/////////////////////////////////////////////////////////////////////////////
// nastaven� objektu pro LOD

void FSetD3DLODObject()
{
// objekt pro LOD
	int n = FIntR();

	if (IsValidID() && (D3D_ID > 1))
	{
		if ((n <= 1) || !D3DF_IsValid(n)) n = -1;

// aktu�ln� objekt
		D3DFITEM* item = GetID();

// test, zda nen� objekt ji� p�ipojen
		if (n != item->LODNext)
		{

// zru�en� star�ho objektu
			D3DF_Del(item->LODNext);

// p�epojen� nov�ho objektu
			item->LODNext = n;

			if (n >= 0)
			{
				D3DFITEM* item2 = D3DF_Get(n);

				int i = item2->LODPrev;

				if (i >= 0)
				{
					D3DF_Get(i)->LODNext = -1;
				}

				item2->LODPrev = -1;

				D3DF_Link(item->Parent, n);

				item2->LODPrev = D3D_ID;

// p�enesen� parametr� objektu
				item2->LODMin2 = item->LODMax2;

				while (n >= 0)
				{
					item2 = D3DF_Get(n);

					item2->TransX = item->TransX;
					item2->TransY = item->TransY;
					item2->TransZ = item->TransZ;
					item2->AktOwn = true;
					D3DF_AktMatLink(item2);

					n = item2->LODNext;
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� vzd�lenosti pro LOD

void FSetD3DLODDistance()
{
	double n = FNum();

	if (IsValidID())
	{
		if (n < 0) n = 0;
		if (n > 1e15) n = 1e15;

		D3DFITEM* item = GetID();

		item->LODMax = n;

		float n2 = (float)(n*n);

		item->LODMax2 = n2;

		int i = item->LODNext;
		if (D3DF_IsValid(i))
		{
			D3DF_Get(i)->LODMin2 = n2;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� vzoru morfov�n�

void FSetD3DMorphModel()
{
	int n = FIntR();

	if (IsValidID())
	{
		D3DFITEM* item = GetID();

		if (D3DF_IsValid(n) && (n > 1))
		{

// inicializace bufferu 1. objektu
			int v = item->VertNum;

			if (item->MorphMax == 0)
			{
				item->MorphVert = (D3DVECTOR**)MemGet(sizeof(D3DVECTOR*));
				item->MorphVert[0] = item->Vertex;
				DuplikData((void**)&(item->MorphVert[0]), v * sizeof(D3DVECTOR));

				item->MorphNorm = (D3DVECTOR**)MemGet(sizeof(D3DVECTOR*));
				item->MorphNorm[0] = item->Normal;
				DuplikData((void**)&(item->MorphNorm[0]), v * sizeof(D3DVECTOR));

				item->MorphUV = (float**)MemGet(MAX_STAGES * sizeof(float*));
				for (int i = 0; i < MAX_STAGES; i++)
				{
					item->MorphUV[i] = item->TextUV[i];
					DuplikData((void**)&(item->MorphUV[i]), v * 2 * sizeof(float));
				}

				item->MorphMax = 1;
			}

// p��prava bufferu pro dal�� objekt (data p�ednastavena na p�ede�l� objekt)
			int i = item->MorphMax;
			item->MorphMax = i + 1;

			item->MorphVert = (D3DVECTOR**)MemSize(item->MorphVert, (i + 1) * sizeof(D3DVECTOR*));
			item->MorphVert[i] = item->MorphVert[i-1];
			DuplikData((void**)&(item->MorphVert[i]), v * sizeof(D3DVECTOR));

			item->MorphNorm = (D3DVECTOR**)MemSize(item->MorphNorm, (i + 1) * sizeof(D3DVECTOR*));
			item->MorphNorm[i] = item->MorphNorm[i-1];
			DuplikData((void**)&(item->MorphNorm[i]), v * sizeof(D3DVECTOR));

			item->MorphUV = (float**)MemSize(item->MorphUV, (i + 1) * MAX_STAGES * sizeof(float*));
			for (int j = 0; j < MAX_STAGES; j++)
			{
				int m = i*MAX_STAGES + j;
				item->MorphUV[m] = item->MorphUV[m - MAX_STAGES];
				DuplikData((void**)&(item->MorphUV[m]), v * 2 * sizeof(float));
			}
			
// p�enesen� dat z dal��ho objektu
			D3DFITEM* item2 = D3DF_Get(n);
			int v2 = item2->VertNum;
			if (v2 > v) v2 = v;

			if ((item->MorphVert[i] != NULL) && (item2->Vertex != NULL))
			{
				MemCopy(item->MorphVert[i], item2->Vertex, v2 * sizeof(D3DVECTOR));
			}

			if ((item->MorphNorm[i] != NULL) && (item2->Normal != NULL))
			{
				MemCopy(item->MorphNorm[i], item2->Normal, v2 * sizeof(D3DVECTOR));
			}

			for (j = 0; j < MAX_STAGES; j++)
			{
				int m = i*MAX_STAGES + j;

				if ((item->MorphUV[m] != NULL) && (item2->TextUV[j] != NULL))
				{
					MemCopy(item->MorphUV[m], item2->TextUV[j], v2 * 2 * sizeof(float));
				}
			}
		}
		else
		{

// reset morfov�n�
			if (n < 0)
			{
				D3DF_MorphReset(item);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// nastaven� stupn� morfov�n�

void FSetD3DMorphLevel()
{
	double n = FNum();

// test platnosti aktu�ln�ho objektu
	if (IsValidID())
	{
		D3DFITEM* item = GetID();

		int max = item->MorphMax;
		if (max == 0) return;

// test, zda je zm�na aktu�ln�ho stupn�
		if ((n > 100000) || (n < -100000)) n = 0;

		while (n < 0) n += max;
		while (n >= max) n -= max;

		if (n != item->MorphStage)
		{
			item->MorphStage = n;

// resetov�n� stavu objektu
			pD3Reset(item);
			D3DF_Modi(D3D_ID);
			MemFree(item->ShadeVert);
			item->ShadeVert = NULL;

// p��prava stupn� a v�hy
			int v = item->VertNum;
			int s = (int)n;
			n = n - s;
			int i;

// pou�ije se cel� stupe� (pro zrychlen� se neinterpoluje)
			if (n == 0)
			{
				if (item->Vertex != NULL)
				{
					MemCopy(item->Vertex, item->MorphVert[s], v * sizeof(D3DVECTOR));
				}

				if (item->Normal != NULL)
				{
					MemCopy(item->Normal, item->MorphNorm[s], v * sizeof(D3DVECTOR));
				}

				for (int i = 0; i < MAX_STAGES; i++)
				{
					if (item->TextUV[i] != NULL)
					{
						MemCopy(item->TextUV[i], item->MorphUV[s*MAX_STAGES + i], v * 2 * sizeof(float));
					}
				}
			}
			else
			{
				FPUFloat();

				int s2 = s + 1;
				if (s2 >= item->MorphMax) s2 = 0;

// interpolace vrchol�
				if (item->Vertex != NULL)
				{
					i = item->VertNum * 3 - 1;

					float* v0 = (float*)(item->Vertex);
					float* v1 = (float*)(item->MorphVert[s]);
					float* v2 = (float*)(item->MorphVert[s2]);

					for (; i >= 0; i--)
					{
						*v0 = (float)(*v1 + (*v2 - *v1)*n);

						v0++;
						v1++;
						v2++;
					}
				}

// interpolace norm�l
				if (item->Normal != NULL)
				{
					i = item->VertNum * 3 - 1;

					float* n0 = (float*)(item->Normal);
					float* n1 = (float*)(item->MorphNorm[s]);
					float* n2 = (float*)(item->MorphNorm[s2]);

					for (; i >= 0; i--)
					{
						*n0 = (float)(*n1 + (*n2 - *n1)*n);

						n0++;
						n1++;
						n2++;
					}
				}

// interpolace sou�adnic textur
				for (int k = 0; k < MAX_STAGES; k++)
				{
					if (item->TextUV[k] != NULL)
					{
						i = item->VertNum * 2 - 1;

						float* uv0 = (float*)(item->TextUV[k]);
						float* uv1 = (float*)(item->MorphUV[s*MAX_STAGES + k]);
						float* uv2 = (float*)(item->MorphUV[s2*MAX_STAGES + k]);

						for (; i >= 0; i--)
						{
							*uv0 = (float)(*uv1 + (*uv2 - *uv1)*n);

							uv0++;
							uv1++;
							uv2++;
						}
					}
				}

				FPUDouble();
			}

// aktualizace hranice
			item->AktSector = TRUE;
			D3DF_MapBound(D3D_ID);
		}
	}
}

/***************************************************************************\
*																			*
*									DDE										*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// nastaven� jm�na aplikace DDE

void FSetDDEApp()
{
// p��prava jm�na aplikace
	CString txt;
	FText(txt);
	DDEAppAktName = txt;

// nalezen� jm�na serveru
	DDEAppAkt = -1;
	DDEServAkt = -1;

	if ((txt.Length() > 11) && (txt.Get(0) == '[') && (txt.Get(9) == ']'))
	{
		DDESERVITEM* servitem = DDEServList;

		for (int i = 0; i < DDEServNum; i++)
		{
			if ((servitem->ServHandle != MainFrame) &&
				(servitem->ServName == txt))
			{
				DDEServAkt = i;
				DDEAppAkt = servitem->AppIndex;
				break;
			}

			servitem++;
		}
	}

// nalezen� jm�na aplikace
	if (DDEServAkt < 0)
	{
		DDEAPPITEM* appitem = DDEAppList;

		for (int i = 0; i < DDEAppNum; i++)
		{
			if (appitem->AppName == txt)
			{
				DDEAppAkt = i;
				break;
			}

			appitem++;
		}
	}
}

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------


/////////////////////////////////////////////////////////////////////////////
// nastaven� aktivn� aplikace

void FSetActive()
{
	bool act = FBool();

	if (act != AppActive)
	{
		if (act && AppMinim) ::ShowWindow(MainFrame, SW_RESTORE);
		::SetForegroundWindow(act ? MainFrame : PrevWindow);
		AppActive = act;
	}
}

/////////////////////////////////////////////////////////////////////////////
// tisk obr�zku

void FPicPrint2() 
{ 
	FPicPrint();
}

/***************************************************************************\
*																			*
*								obsluha DLL									*
*																			*
\***************************************************************************/

// --------------------- vypnuto pro MINI verzi --------------------
#ifndef _MINI

/////////////////////////////////////////////////////////////////////////////
// uvoln�n� DLL knihovny

void FDLLFree()
{
// na�ten� handle knihovny
	int n = FIntR();

// uvoln�n� knihovny
	if (n != 0)
	{
		::FreeLibrary((HMODULE)n);
	}
}

/////////////////////////////////////////////////////////////////////////////
// proveden� DLL funkce

void FDLLExecCom()
{
	FDLLExecNum();
}

/////////////////////////////////////////////////////////////////////////////
// zru�en� bloku pam�ti

void FMemoryFree()
{
// adresa pam�ov�ho bloku
	void* blok = (void*)FIntR();

// zru�en� pam�ov�ho bloku
	MemFree(blok);
}

/////////////////////////////////////////////////////////////////////////////
// ukazatel �ten� z pam�ti

void FSetMemoryRead()
{
	DLLMemoryRead = (void*)FIntR();
}

/////////////////////////////////////////////////////////////////////////////
// ukazatel z�pisu do pam�ti

void FSetMemoryWrite()
{
	DLLMemoryWrite = (void*)FIntR();
}

/////////////////////////////////////////////////////////////////////////////
// kopie dat

void FMemoryCopy()
{
// d�lka dat
	int len = FIntR();

// proveden� kopie dat
	MemCopy(DLLMemoryWrite, DLLMemoryRead, len);
	DLLMemoryRead = (char*)DLLMemoryRead + len;
	DLLMemoryWrite = (char*)DLLMemoryWrite + len;
}

/////////////////////////////////////////////////////////////////////////////
// bajt se znam�nkem

void FSetMemoryByteS()
{
	int data = FIntR();
	char* buf = (char*)DLLMemoryWrite;
	*buf = (char)data;
	buf++;
	DLLMemoryWrite = buf;
}

/////////////////////////////////////////////////////////////////////////////
// slovo se znam�nkem

void FSetMemoryWordS()
{
	int data = FIntR();
	short* buf = (short*)DLLMemoryWrite;
	*buf = (short)data;
	buf++;
	DLLMemoryWrite = buf;
}

/////////////////////////////////////////////////////////////////////////////
// 3-bajt se znam�nkem

void FSetMemory3ByteS()
{
	int data = FIntR();
	BYTE* buf = (BYTE*)DLLMemoryWrite;
	*(WORD*)buf = (WORD)data;
	buf[2] = ((BYTE*)(&data))[2];

	buf += 3;
	DLLMemoryWrite = buf;
}

/////////////////////////////////////////////////////////////////////////////
// dvouslovo se znam�nkem

void FSetMemoryDWordS()
{
	long data = FIntR();
	long* buf = (long*)DLLMemoryWrite;
	*buf = data;
	buf++;
	DLLMemoryWrite = buf;
}

/////////////////////////////////////////////////////////////////////////////
// dvouslovo bez znam�nka

void FSetMemoryDWordU()
{
	DWORD data = FIntRN();
	DWORD* buf = (DWORD*)DLLMemoryWrite;
	*buf = data;
	buf++;
	DLLMemoryWrite = buf;
}

/////////////////////////////////////////////////////////////////////////////
// re�ln� ��slo 4 bajty

void FSetMemoryFloat()
{
	double data = FNum();

	float* buf = (float*)DLLMemoryWrite;
	*buf = (float)data;
	buf++;
	DLLMemoryWrite = buf;
}

/////////////////////////////////////////////////////////////////////////////
// re�ln� ��slo 8 bajt�

void FSetMemoryDouble()
{
	double data = FNum();

	double* buf = (double*)DLLMemoryWrite;
	*buf = data;
	buf++;
	DLLMemoryWrite = buf;
}

/////////////////////////////////////////////////////////////////////////////
// logick� hodnota

void FSetMemoryBool()
{
	BOOL* buf = (BOOL*)DLLMemoryWrite;
	*buf = (BOOL)FBool();
	buf++;
	DLLMemoryWrite = buf;
}

/////////////////////////////////////////////////////////////////////////////
// text zakon�en� nulou

void FSetMemoryText0()
{
	CString text;
	FText(text);
	char* buf = (char*)DLLMemoryWrite;
	text.Write(buf);
	buf += text.Length();
	buf[0] = 0;
	buf++;
	DLLMemoryWrite = buf;
}

/////////////////////////////////////////////////////////////////////////////
// ukazatel na text zakon�en� nulou

void FSetMemoryText0P()
{
	CString text;
	FText(text);
	const char** buf = (const char**)DLLMemoryWrite;
	*buf = text;
	buf++;
	DLLMemoryWrite = buf;
}

/////////////////////////////////////////////////////////////////////////////
// text s pevnou d�lkou

void FSetMemoryTextN()
{
#ifdef _UNICODE
	nepodporovan� p�eklad!!!!	
#endif

	CString text;
	FText(text);
	int len = text.Length();
	int len2 = DLLMemoryTextNLen - len;

	if (len2 < 0)
	{
		len2 = 0;
		len = DLLMemoryTextNLen;
	}

	char* buf = (char*)DLLMemoryWrite;
	MemCopy(buf, text.DataData(), len);
	buf += len;
	MemFill(buf, len2, ' ');
	buf += len2;
	DLLMemoryWrite = buf;
}

/////////////////////////////////////////////////////////////////////////////
// d�lka textu s pevnou d�lkou

void FSetMemoryTextNLen()
{
	int n = FIntR();
	if (n < 0) n = 0;
	DLLMemoryTextNLen = n;
}

/////////////////////////////////////////////////////////////////////////////
// na�ten� bloku dat

void FMemoryLoad()
{
	int len = FIntR();

	BYTE* buf = (BYTE*)DLLMemoryWrite;

	for (; len > 0; len--)
	{
		*buf = FileReadByte();
		buf++;
	}

	DLLMemoryWrite = buf;
}

/////////////////////////////////////////////////////////////////////////////
// z�pis bloku dat

void FMemorySave()
{
	int len = FIntR();

	BYTE* buf = (BYTE*)DLLMemoryRead;

	for (; len > 0; len--)
	{
		FileWriteByte(*buf);
		buf++;
	}

	DLLMemoryRead = buf;
}

/***************************************************************************\
*																			*
*								konfigurace									*
*																			*
\***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// jm�no konfigura�n�ho souboru

void FSetIniFile()
{
	FText(INIFile);
}

/////////////////////////////////////////////////////////////////////////////
// jm�no konfigura�n� sekce

void FSetIniSection()
{
	FText(INISection);
}

/////////////////////////////////////////////////////////////////////////////
// jm�no konfigura�n�ho parametru

void FSetIniValue()
{
	FText(INIKey);
}

/////////////////////////////////////////////////////////////////////////////
// z�pis parametru

void _fastcall SetIniValue(LPCTSTR text)
{
// jm�no hodnoty
	LPCTSTR key = NULL;
	if (INIKey.IsNotEmpty()) key = INIKey;

// nastaven� polo�ky
	::WritePrivateProfileString(INISection, key, text, INIFile);
}


/////////////////////////////////////////////////////////////////////////////
// zru�en� konfigura�n�ho parametru

void FSetIniDel()
{
	SetIniValue(NULL);
}

/////////////////////////////////////////////////////////////////////////////
// z�pis konfigura�n�ho textu

void FSetIniString()
{
	CString text;
	FText(text);
	SetIniValue(text);
}

/////////////////////////////////////////////////////////////////////////////
// z�pis konfigura�n�ho ��sla

void FSetIniNum()
{
	int cislo = FIntR();
	CString text;
	text.Int(cislo);
	SetIniValue(text);
}

/////////////////////////////////////////////////////////////////////////////
// skupina kl��� registr�

void FSetRegKey()
{
	REGKey = FIntR();
}

/////////////////////////////////////////////////////////////////////////////
// kl�� registr�

void FSetRegSubkey()
{
	CString text;
	FText(text);
	REGSubkey = text;
}

/////////////////////////////////////////////////////////////////////////////
// jm�no polo�ky registru

void FSetRegValue()
{
	CString text;
	FText(text);
	REGValue = text;
}

/////////////////////////////////////////////////////////////////////////////
// zru�en� registru

void FSetRegDel()
{
// otev�en� kl��e
	HKEY hkey;

	if (::RegOpenKeyEx(
		(HKEY) (REGKey | 0x80000000),
		REGSubkey,
		0,
		KEY_ALL_ACCESS,
		&hkey) == ERROR_SUCCESS)
	{

// zru�en� registru
		::RegDeleteValue(
			hkey,
			REGValue);

// uzav�en� kl��e
		::RegCloseKey(hkey);
	}
}

/////////////////////////////////////////////////////////////////////////////
// z�pis textu do registru

void FSetRegString()
{
// text k z�pisu
	CString text;
	FText(text);

// otev�en� kl��e
	HKEY hkey;
	DWORD pos;

	if (::RegCreateKeyEx(
		(HKEY) (REGKey | 0x80000000),
		REGSubkey,
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		NULL,
		&hkey,
		&pos) == ERROR_SUCCESS)
	{

// nastaven� hodnoty registru
		::RegSetValueEx(
			hkey,
			REGValue,
			0,
			REG_SZ,
			(const BYTE*)(LPCTSTR)text,
			(text.Length()+1) * sizeof(TCHAR));

// uzav�en� kl��e
		::RegCloseKey(hkey);
	}
}

/////////////////////////////////////////////////////////////////////////////
// z�pis ��sla do registru

void FSetRegNum()
{
	int cislo = FIntR();

// otev�en� kl��e
	HKEY hkey;
	DWORD pos;

	if (::RegCreateKeyEx(
		(HKEY) (REGKey | 0x80000000),
		REGSubkey,
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		NULL,
		&hkey,
		&pos) == ERROR_SUCCESS)
	{

// nastaven� hodnoty registru
		::RegSetValueEx(
			hkey,
			REGValue,
			0,
			REG_DWORD,
			(const BYTE*)&cislo,
			4);

// uzav�en� kl��e
		::RegCloseKey(hkey);
	}
}

#endif // _MINI
// --------------------- konec vypnut� pro MINI verzi -------------------
