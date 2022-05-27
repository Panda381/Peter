
/***************************************************************************\
*																			*
*								Ikonov� prom�nn�							*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// struktura z�hlav� ikony - 16 bajt�
// P�i komprimaci ud�v� prvn� dvojslovo dat d�lku komprimovan�ch dat
// Dekomprimace i komprimace se prov�d� s daty spole�n�mi pro v�echny ikony,
// je proto nutno v p��pad� pot�eby zajistit vlastnictv� ikony!
// Komprimace se prov�d� pouze u ikon 32x32!

typedef struct ICONDATA_
{
	long	Refer;					// (4) ��ta� referenc� na ikonu
	long	Param;					// (4) parametry (pr�hlednost, komprese)
	long	Size;					// (4) velikost nezkomprimovan�ch dat ikony (typicky 32x32 = 1024)
	BYTE*	Data;					// (4) ukazatel na data ikony
} ICONDATA;

//////////////////////////////////////////////////////////////////////////////
// p��znaky typu ikony a obr�zku
//enum PICPARAM {
//	PicParamPic,			// pouze obr�zek bez pozad�
//	PicParamMix,			// obr�zek mixovan� s pozad�m
//	PicParamBack,			// pouze pozad� (pr�zdn� obr�zek)
//	PicParamNone,			// nezn�m� obsah
//	PicParamComp,			// komprimovan� data
//};

#define SIZEOFICONDATA	(3*sizeof(long) + sizeof(BYTE*)) // velikost z�hlav�

extern ICONDATA EmptyIconData;				// data pr�zdn� ikony
extern	const CIcon		EmptyIcon;			// pr�zdn� ikona

/////////////////////////////////////////////////////////////////////////////
// statick� funkce

// statick� inicializace ikon (p�i chyb� pam�ti vrac� FALSE)
bool InitIcon();

// vytvo�en� dat ikony (p�i chyb� pam�ti vrac� NULL)
ICONDATA* _fastcall NewIconData(int size);

// zru�en� dat ikony (odd�leno kv�li lep�� optimalizaci)
void _fastcall DelIconData(ICONDATA* data);


/////////////////////////////////////////////////////////////////////////////
// ikona

class CIcon
{

// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn�
	ICONDATA*		pData;			// ukazatel na z�hlav� ikony (NULL=nen� nic)

// p�ipojen� dat ikony
	inline void attach(ICONDATA* data)
	{
		ASSERT(data != NULL);

		pData = data;
		LongIncrement(&(data->Refer));
	}

// odpojen� (a zru�en�) dat ikony
	inline void detach(ICONDATA* data)
	{
		ASSERT(data != NULL);
		ASSERT(data->Refer > 0);

		if (LongDecrement(&(data->Refer)))
		{
			DelIconData(data);
		}
	}


// ---------------------------- ve�ejn� funkce ------------------------------

public:

// konstruktor a destruktor
	CIcon();					// pr�zdn� ikona o velikosti ICONSIZE
	CIcon(const CIcon& src);
	CIcon(ICONDATA* data);
	~CIcon();

// statick� konstruktor a destruktor
	void Init();				// pr�zdn� ikona o velikosti ICONSIZE
	void _fastcall Init(const CIcon& src);
	void _fastcall Init(ICONDATA* data);
	bool _fastcall Init(int size);	// p�i chyb� pam�ti vrac� FALSE, ikona nen� vytvo�ena
	void Term();

// poskytnut� ukazatele na data
	inline ICONDATA* Data() const { return pData; };
	inline BYTE* DataData() const { return pData->Data; };

// poskytnut� velikosti nezkomprimovan�ch dat ikony
	inline int Size() const { return pData->Size; };

// poskytnut�/nastaven� parametr� ikony
	inline int Param() const { return pData->Param; };
	inline void Param(int param) { pData->Param = param; };

// vypr�zdn�n� (uvoln�n� dat) - p�ipoj� se standardn� pr�zdn� ikona ICONSIZE
	void Empty();

// test, zda je ikona pr�zdn� (zda ukazuje na standardn� pr�zdnou ikonu ICONSIZE)
	inline BOOL IsEmpty() { return ((DWORD)pData == (DWORD)&EmptyIconData); };
	inline BOOL IsNotEmpty() { return ((DWORD)pData != (DWORD)&EmptyIconData); };

// vymaz�n� obsahu ikony (napln�n� pr�hlednou barvou), zajist� p�ivlastn�n� (a dekomprimaci) bufferu,
// p�i chyb� pam�ti vrac� FALSE, p�vodn� obsah nezm�n�n
	bool Clear();

// vymaz�n� obsahu ikony s nastaven�m velikosti (napln�n� pr�hlednou barvou), 
// zajist� p�ivlastn�n� (a dekomprimaci) bufferu, p�i chyb� pam�ti vrac� FALSE, p�vodn� obsah nezm�n�n
	bool _fastcall Clear(int size);

// kopie do vlastn�ho bufferu p�ed modifikac� (komprimovan� data z�stanou komprimovan�)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool CopyWrite();

// vytvo�en� nov� ikony se stejnou velikost� (p�ipraveno k z�pisu, data jsou n�hodn�)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool New();

// vytvo�en� nov� ikony (p�ipraveno k z�pisu, data jsou n�hodn�)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool _fastcall New(int size);

// kontrola platnosti offsetu bodu
	inline BOOL IsValid(const int off) const 
		{ return ((DWORD)off < (DWORD)pData->Size); };

	inline BOOL IsNotValid(const int off) const 
		{ return ((DWORD)off >= (DWORD)pData->Size); };

// poskytnut� p��stupu k bodu ikony (bez kontroly offsetu) - ikona nesm� b�t komprimovan�!
// v p��pad� z�pisu mus� b�t zaji�t�no p�ivlastn�n� bufferu!
	inline BYTE& operator[] (const int off) 
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline const BYTE& operator[] (const int off) const
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline BYTE& At(const int off) 
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline const BYTE& At(const int off) const
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

// poskytnut� bodu (s kontrolou platnosti offsetu) - ikona nesm� b�t komprimovan�!
// pro neplatn� offset vrac� barvu pozad�
	BYTE _fastcall Get(const int off) const;

// nastaven� bodu (s kontrolou platnosti offsetu) - ikona nesm� b�t komprimovan�!
// p�ed z�pisem je nutno zajistit p�ivlastn�n� bufferu!
	void _fastcall Set(const int off, const BYTE data);

// komprese dat ikony 32*32, vrac� velikost dat (p�i chyb� pam�ti vrac� <0, data jsou nezm�n�na)
// komprese se prov�d� ve spole�n�m bufferu pro v�echny prom�nn�!
// (v p��pad� pot�eby je nutno zajistit p�ivlastn�n� bufferu)
// p�vodn� buffer s daty je zru�en
// (velikost komprimovan�ch dat je ud�v�na bez dvojslova s velikost�, buffer s daty
// je tedy o 4 v�t��, data za��naj� na offsetu 4 v bufferu)
	int Comp() const;

// dekomprimace dat ikony 32*32 (jsou-li komprimov�na), p�i chyb� vrac� FALSE, data jsou nezm�n�na
// dekomprese se prov�d� ve spole�n�m bufferu pro v�echny prom�nn�
// (v p��pad� pot�eby je nutno zajistit p�ivlastn�n� bufferu)
// p�vodn� buffer s komprimovan�mi daty je zru�en
	bool DeComp() const;

// kopie nov�ch dat ikony (rozm�ry z�stanou nezm�n�ny) - zajist� p�ivlastn�n� (a dekomprimaci) bufferu,
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool CopyData(BYTE* src);

// kopie komprimovan�ch nov�ch dat ikony (rozm�ry z�stanou nezm�n�ny) - zajist� odpojen� dat
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool CompCopy(BYTE* src);

// kopie nov�ch dat ikony s konverz� (rozm�ry z�stanou nezm�n�ny) - zajist� odpojen� dat
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool CopyKonvData(BYTE* src);

// extrakt ikony ze souboru form�tu PET (p�i chyb� vr�t� FALSE, obsah nezm�n�n)
	bool Extract(CText jmeno);

// na�ten� ikony ze souboru form�tu ICO (FALSE=chyba, obsah nezm�n�n)
	bool LoadFile(CText jmeno);

// ulo�en� ikony do souboru form�tu ICO (komprimovanou ikonu dekomprimuje) (FALSE=chyba)
	bool SaveFile(CText jmeno) const;

// oper�tor p�i�azen�
	const CIcon& operator= (const CIcon& src);
	const CIcon& operator= (ICONDATA* src);
};


/***************************************************************************\
*																			*
*								Buffer ikon									*
*																			*
\***************************************************************************/

class CBufIcon : public CBuffer<CIcon>
{

// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn�
	int			m_Width;		// ���ka jedn� ikony
	int			m_Height;		// v��ka jedn� ikony
	int			m_WidthByte;	// d�lka jedn� linky v bajtech
	int			m_IconSize;		// velikost ikony v bajtech

// ---------------------------- ve�ejn� funkce ------------------------------

public:

// konstruktor
	CBufIcon();

// statick� konstruktor
	void Init();

// poskytnut� ���ky ikony
	inline int Width() const { return m_Width; };

// poskytnut� v��ky ikony
	inline int Height() const { return m_Height; };

// poskytnut�/nastaven� velikosti ikony (p��padn� existuj�c� ikony zru��!)
	void _fastcall IconSize(const int width, const int height);
	inline int IconSize() { return m_IconSize; };

// p�id�n� polo�ky (vrac� index polo�ky, <0 = chyba)
// prov�d� z�znam do UNDO bufferu
	inline int _fastcall Add(const CIcon& icon) { return CBuffer<CIcon>::Add(icon); }
	int _fastcall Add(ICONDATA* data);

// na�ten� ikon z resource (vynulovat buffer, aby bylo ukl�d�no po �ad�!), vrac� TRUE=operace OK
	bool Load(const int nID, int num);

// vygenerov�n� dat bitmapy (vrac� ukazatel na buffer s rezervou 8 bajt� na konci, p�i chyb� vrac� NULL)
	BYTE* GenerBitmap(SMALLICON smallicon);

// vygenerov�n� masky ikon z dat barevn� bitmapy (smallicon = polovi�n� velikost)
// barevn� bitmapa mus� m�t na konci rezervu pro zarovn�n� linky na 8 bajt� !
// vrac� adresu na buffer, p�i chyb� vrac� NULL
	BYTE* GenerMask(BYTE* bitmapa, SMALLICON smallicon);

// vygenerov�n� seznamu ikon (drag = ta�en�, smallicon = polovi�n� velikost,
//		himg = pou�it� seznam, pro himg=NULL vytvo�� nov� seznam)
// p�i chyb� vrac� NULL, p�vodn� seznam ikon p�itom z�st�v� nezm�n�n
	HIMAGELIST GenerList(bool drag, SMALLICON smallicon, HIMAGELIST himg = NULL);
};
