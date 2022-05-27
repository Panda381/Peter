
/***************************************************************************\
*																			*
*									Obr�zky									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// struktura polo�ky dat obr�zku - 32 bajt�
// P�i komprimaci ud�v� prvn� dvojslovo dat d�lku komprimovan�ch dat
// Dekomprimace i komprimace se prov�d� s daty spole�n�mi pro v�echny obr�zky
// je proto nutno v p��pad� pot�eby zajistit vlastnictv� obr�zku!

typedef struct PICTUREDATA_
{
	long	Refer;					// (4) ��ta� referenc� na obr�zek
	long	Width;					// (4) ���ka obr�zku v bodech
	long	Height;					// (4) v��ka obr�zku v link�ch
	long	Param;					// (4) parametry (pr�hlednost, komprese)
	long	Res1;					// (4) ... pro zarovn�n�
	long	Res2;					// (4) ... pro zarovn�n�
	long	Res3;					// (4) ... pro zarovn�n�
	BYTE*	Data;					// (4) ukazatel na data (velikost Width*Height)
} PICTUREDATA;

//////////////////////////////////////////////////////////////////////////////
// p��znaky typu ikony a obr�zku
//enum PICPARAM {
//	PicParamPic,			// pouze obr�zek bez pozad�
//	PicParamMix,			// obr�zek mixovan� s pozad�m
//	PicParamBack,			// pouze pozad� (pr�zdn� obr�zek)
//	PicParamNone,			// nezn�m� obsah
//	PicParamComp,			// komprimovan� data
//};

#define SIZEOFPICTUREDATA	(7*sizeof(long) + sizeof(BYTE*))

extern PICTUREDATA EmptyPictureData;	// data pr�zdn�ho obr�zku (velikost ICONSIZE)
extern	const CPicture	EmptyPicture;	// pr�zdn� obr�zek

/////////////////////////////////////////////////////////////////////////////
// statick� funkce

// statick� inicializace obr�zk� (p�i chyb� pam�ti vrac� FALSE)
bool InitPicture();

// vytvo�en� dat obr�zku (p�i chyb� pam�ti vrac� NULL)
PICTUREDATA* _fastcall NewPictureData(int width, int height);

// zru�en� dat obr�zku (odd�leno kv�li lep�� optimalizaci)
void _fastcall DelPictureData(PICTUREDATA* data);


/////////////////////////////////////////////////////////////////////////////
// obr�zek

class CPicture
{

// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn� - pouze ukazatel na data
	PICTUREDATA*		pData;			// ukazatel na z�hlav� obr�zku

// p�ipojen� dat obr�zku
	inline void attach(PICTUREDATA* data)
	{
		ASSERT(data != NULL);

		pData = data;
		LongIncrement(&(data->Refer));
	}

// odpojen� (a zru�en�) dat obr�zku
	inline void detach(PICTUREDATA* data)
	{
		ASSERT(data != NULL);
		ASSERT(data->Refer > 0);

		if (LongDecrement(&(data->Refer)))
		{
			DelPictureData(data);
		}
	}

// ---------------------------- ve�ejn� funkce ------------------------------

public:

// konstruktor a destruktor
	CPicture();						// pr�zdn� obr�zek o velikosti ICONSIZE
	CPicture(const CPicture& src);
	CPicture(PICTUREDATA* data);
	~CPicture();

// statick� konstruktor a destruktor
	void Init();					// pr�zdn� obr�zek o velikosti ICONSIZE
	void _fastcall Init(const CPicture& src);
	void _fastcall Init(PICTUREDATA* data);
	bool _fastcall Init(int width, int height); // p�i chyb� pam�ti vrac� FALSE, obr�zek nen� vytvo�en
	void Term();

// poskytnut� ukazatele na data
	inline PICTUREDATA* Data() const { return pData; }; // poskytnut� adresy dat
	inline BYTE* DataData() const { return pData->Data; }; // poskytnut� adresy dat

// poskytnut� ���ky obr�zky
	inline int Width() const { return pData->Width; };

// poskytnut� v��ky ob��zku
	inline int Height() const { return pData->Height; };

// poskytnut� velikosti nezkomprimovan�ch dat obr�zku (bez z�hlav�)
	inline int Size() const { return pData->Width * pData->Height; };

// vypr�zdn�n� (uvoln�n� dat)
	void Empty();

// test, zda je obr�zek pr�zdn� (zda ukazuje na standardn� pr�zdn� obr�zek ICONSIZE)
	inline BOOL IsEmpty() { return ((DWORD)pData == (DWORD)&EmptyPictureData); };
	inline BOOL IsNotEmpty() { return ((DWORD)pData != (DWORD)&EmptyPictureData); };

// vymaz�n� obsahu obr�zku (napln�n� pr�hlednou barvou), zajist� p�ivlastn�n� (a dekomprimaci) bufferu,
// p�i chyb� pam�ti vrac� FALSE, p�vodn� obsah nezm�n�n
	bool Clear();

// vymaz�n� obsahu obr�zku s nastaven�m velikosti (napln�n� pr�hlednou barvou), zajist� p�ivlastn�n� (a dekomprimaci) bufferu,
// p�i chyb� pam�ti vrac� FALSE, p�vodn� obsah nezm�n�n
	bool _fastcall Clear(int width, int height);

// kopie obr�zku do vlastn�ho bufferu p�ed modifikac� (komprimovan� data z�stanou komprimovan�)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool CopyWrite();

// vytvo�en� nov�ho obr�zku se stejnou velikost� (p�ipraveno k z�pisu, data jsou n�hodn�)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool New();

// vytvo�en� nov�ho obr�zku (p�ipraveno k z�pisu, data jsou n�hodn�)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool _fastcall New(int width, int height);

// nastaven� nov�ch rozm�r� obr�zku (nov� data jsou vymaz�na)
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool Resize(int width, int height);

// kontrola platnosti offsetu bodu
	inline BOOL IsValid(const int off) const 
		{ return ((DWORD)off < (DWORD)Size()); };

	inline BOOL IsNotValid(const int off) const 
		{ return ((DWORD)off >= (DWORD)Size()); };

// kontrola platnosti indexu 
	inline BOOL IsValid(const int x, const int y) const
		{ return (((DWORD)x < (DWORD)pData->Width) && ((DWORD)y < (DWORD)pData->Height)); };

	inline BOOL IsNotValid(const int x, const int y) const
		{ return (((DWORD)x >= (DWORD)pData->Width) || ((DWORD)y >= (DWORD)pData->Height)); };

// poskytnut� p��stupu k bodu (bez kontroly offsetu/indexu) - obr�zek nesm� b�t komprimovan�!
// v p��pad� z�pisu mus� b�t zaji�t�no p�ivlastn�n� bufferu!
	inline BYTE& operator[] (const int off) 
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline const BYTE& operator[] (const int off) const
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline BYTE& At(const int off) 
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline const BYTE& At(const int off) const
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline BYTE& At(const int x, const int y) 
		{ ASSERT(IsValid(x, y)); return pData->Data[x + y*pData->Width]; }

	inline const BYTE& At(const int x, const int y) const
		{ ASSERT(IsValid(x, y)); return pData->Data[x + y*pData->Width]; }

// poskytnut� bodu (s kontrolou platnosti offsetu/indexu), obr�zek nesm� b�t komprimovan�!
// pro neplatn� bod vr�t� barvu pozad�)
	BYTE _fastcall Get(const int off) const;
	BYTE _fastcall Get(const int x, const int y) const;

// nastaven� bodu (s kontrolou platnosti offsetu/indexu), obr�zek nesm� b�t komprimovan�!
// p�ed z�pisem je nutno zajistit p�ivlastn�n� bufferu!
	void _fastcall Set(const int off, const BYTE data);
	void _fastcall Set(const int x, const int y, const BYTE data);

// komprese dat obr�zku, vrac� velikost dat (p�i chyb� pam�ti vrac� <0, data jsou nezm�n�na)
// komprese se prov�d� ve spole�n�m bufferu pro v�echny prom�nn�!
// (v p��pad� pot�eby je nutno zajistit p�ivlastn�n� bufferu)
// p�vodn� buffer s daty je zru�en
// (velikost komprimovan�ch dat je ud�v�na bez dvojslova s velikost�, buffer s daty
// je tedy o 4 v�t��, data za��naj� na offsetu 4 v bufferu)
	int Comp() const;

// dekomprimace dat obr�zku (jsou-li komprimov�na), p�i chyb� vrac� FALSE
// dekomprese se prov�d� ve spole�n�m bufferu pro v�echny prom�nn�
// (v p��pad� pot�eby je nutno zajistit p�ivlastn�n� bufferu)
// p�vodn� buffer s komprimovan�mi daty je zru�en
	bool DeComp() const;

// kopie nov�ch dat obr�zku (rozm�ry z�stanou nezm�n�ny) - zajist� odpojen� dat
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool CopyData(BYTE* src);

// kopie komprimovan�ch nov�ch dat obr�zku (rozm�ry z�stanou nezm�n�ny) - zajist� odpojen� dat
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool CompCopy(BYTE* src);

// kopie v��ezu do obr�zku (se zpr�hledn�n�m)
	void AddCopy(const CPicture& src, int x, int y, int w, int h, int xs, int ys);
	void AddCopy(const CPicture& src, int x, int y);
	void AddCopy(int id, int x, int y);

// kopie nov�ch dat obr�zku s konverz� (rozm�ry z�stanou nezm�n�ny) - zajist� odpojen� dat
// p�i chyb� pam�ti vr�t� FALSE, obsah bude nezm�n�n
	bool CopyKonvData(BYTE* src);

// na�ten� obr�zku ze souboru (FALSE=chyba, obsah nezm�n�n)
//	bool LoadFile(CText jmeno);

// ulo�en� obr�zku do souboru form�tu BMP (komprimovan� obr�zek dekomprimuje)(FALSE=chyba)
//	bool SaveFile(CText jmeno) const;

// vygenerov�n� obr�zku pro ta�en� (p�i chyb� vrac� NULL)
//	HIMAGELIST GenerDrag(const CText& text);

// na�ten� obr�zku z resource
	bool CPicture::Load(const int nID);

// oper�tor p�i�azen�
	const CPicture& operator= (const CPicture& src);
	const CPicture& operator= (PICTUREDATA* src);
};

