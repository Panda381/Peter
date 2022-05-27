
/***************************************************************************\
*																			*
*									Obr�zky									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// struktura polo�ky dat obr�zku - (64 bajt� + data) nebo (24 bajt� + data)

typedef struct PICTUREDATA_
{
	long	Refer;					// (4) ��ta� referenc� na obr�zek
	long	Param;					// (4) parametry (pr�hlednost, komprese)
	long	Width;					// (4) ���ka obr�zku v bodech
	long	Height;					// (4) v��ka obr�zku v link�ch
	BYTE*	Data;					// (4) ukazatel na data (velikost Width*Height)

#ifndef _MINI
	long	TextInx;				// (4) index textury v bufferu textur (-1 = nen�)
	long	TextWidth;				// (4) ���ka textury, platn� pokud TextData!=NULL
	long	TextHeight;				// (4) v��ka textury, platn� pokud TextData!=NULL
	long	TextIsMip;				// (4) vygenerov�no s mipmaps
	long	TextMipMaps;			// (4) po�et vygenerovan�ch mipmaps, platn� pokud TextData!=NULL
									//	  (v datech ulo�eny sekven�n� za sebou a� po rozm�r 1x1)
	long	TextDevType;			// (4) typ pou�it�ho za��zen�, platn� pokud TextData!=NULL
									//			( = D3DIntAkt*100 + D3DDevAkt)
	BOOL	TextTrans;				// (4) textura obsahuje pr�hlednou barvu

	BYTE*	TextData;				// (4) data textury ve form�tu D3DFMT_A8R8G8B8, 32 bit� (NULL=nejsou vytvo�ena)
	BYTE*	TextDataRGBA;			// (4) data textury ve form�tu RGBA, 32 bit� (pro OpenGL)
	BYTE*	TextDataR8G8B8;			// (4) data textury ve form�tu D3DFMT_R8G8B8, 24 bit�
	BYTE*	TextDataR5G6B5;			// (4) data textury ve form�tu D3DFMT_R5G6B5, 16 bit�
	BYTE*	TextDataA1R5G5B5;		// (4) data textury ve form�tu D3DFMT_A1R5G5B5, 16 bit�
	BYTE*	TextDataA4R4G4B4;		// (4) data textury ve form�tu D3DFMT_A4R4G4B4, 16 bit�
	double	TextSmooth;				// (8) aktu�ln� zjemn�n� textury (nastaveno p�i prvn�m pou�it� jako textura)
									//     (0=data jsou ze souboru a nesm� se proto ru�it)
#else
	long	res;					// (4) zarovn�n� dat
#endif // _MINI

} PICTUREDATA;

// Podporovan� form�ty textur (p��kazem CreateTexture):
//	D3DFMT_A8R8G8B8	(32 bit� s alpha)
//	D3DFMT_X8R8G8B8 (32 bit�)
//	D3DFMT_R5G6B5 (16 bit�)
//	D3DFMT_X1R5G5B5 (15 bit�)
//	D3DFMT_A1R5G5B5 (15 bit� s alpha)
//	D3DFMT_A4R4G4B4 (12 bit� s alpha)
//	D3DFMT_DXT1 (komprese DXT1)
//	D3DFMT_DXT2 (komprese DXT2)
//	D3DFMT_DXT3 (komprese DXT3)
//	D3DFMT_DXT4 (komprese DXT4)
//	D3DFMT_DXT5 (komprese DXT5)

// >>> !!!!! V sou�asnosti Petr pou��v� pouze form�t D3DFMT_A8R8G8B8 !!!!! <<<


#ifndef _MINI
#define SIZEOFPICTUREDATA	(4*sizeof(long) + sizeof(BYTE*) + 6*sizeof(long) + sizeof(BOOL) + 6*sizeof(BYTE*) + sizeof(double))
#else  // _MINI
#define SIZEOFPICTUREDATA	(4*sizeof(long) + sizeof(BYTE*) + sizeof(long))
#endif // _MINI

extern PICTUREDATA EmptyPictureData;	// data pr�zdn�ho obr�zku


/////////////////////////////////////////////////////////////////////////////
// obr�zek

class CPicture
{

// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn� - pouze ukazatel na data
	PICTUREDATA*		pData;			// ukazatel na z�hlav� obr�zku

// p�ipojen� dat
	inline void Attach(PICTUREDATA* data)
	{
		ASSERT(data);
		pData = data;
		LongIncrement(&(data->Refer));
	}

// odpojen� (a zru�en�) dat
	void _fastcall Detach();

// vytvo�en� nov�ho bufferu - star� buffer mus� b�t odpojen!
	inline void NewBuffer(int width, int height)
	{
		ASSERT((width > 0) && (height > 0));
		if (width <= 0) width = 1;
		if (height <= 0) height = 1;
		PICTUREDATA* data = (PICTUREDATA*)MemGet(SIZEOFPICTUREDATA);
		data->Refer = 1;				// po�et referenc�
		data->Param = PicParamNone;		// parametry
		data->Width = width;		// ���ka
		data->Height = height;	// v��ka
		data->Data = (BYTE*)MemGet(width*height);
#ifndef _MINI
		data->TextInx = -1;
		data->TextData = NULL;
		data->TextDataRGBA = NULL;
		data->TextDataR8G8B8 = NULL;
		data->TextDataR5G6B5 = NULL;
		data->TextDataA1R5G5B5 = NULL;
		data->TextDataA4R4G4B4 = NULL;
#endif // _MINI
		pData = data;					// adresa dat
	}

// ---------------------------- ve�ejn� funkce ------------------------------

public:

// konstruktor a destruktor
	CPicture();							// standardn� konstruktor
	CPicture(const CPicture& src);		// kop�rovac� konstruktor
	CPicture(int width, int height);	// konstruktor s vytvo�en�m obr�zku
	~CPicture();						// standardn� destruktor

// statick� konstruktor a destruktor
	void Init();						// statick� konstruktor
	void Init(PICTUREDATA* data);		// statick� konstruktor se zad�n�m dat
	void Init(int width, int height);	// statick� konstruktor s vytvo�en�m obr�zku
	void Term();						// statick� destruktor

#ifndef _MINI
// importov�n� textury z True Color bufferu
// ... Funkce vstupn� buffer zru��!
// ... Buffery pro texturu mus� b�t zru�eny!
	void ImportTexture(BYTE* buf, int newwidth, int newheight);

// vytvo�en� dat textury, nen�-li vytvo�ena
	void InitTexture(int stage);

// inicializace MipMap textury
//	void InitMipMap();

#endif // _MINI

// poskytnut� ukazatele na data
	inline PICTUREDATA* Data() const { return pData; }; // poskytnut� adresy dat
	inline BYTE* DataData() const { return pData->Data; }; // poskytnut� adresy dat

// poskytnut�/nastaven� parametru obr�zku
	inline int Param() const { return pData->Param; };
	inline void Param(int param) { pData->Param = param; };

// poskytnut� ���ky obr�zky
	inline int Width() const { return pData->Width; };

// poskytnut� v��ky ob��zku
	inline int Height() const { return pData->Height; };

// poskytnut� velikost dat obr�zku (bez z�hlav�)
	inline int Size() const { return (Width()*Height()); };

// vymaz�n� obsahu obr�zku (napln�n� pr�hlednou barvou)
	inline void Clear() { MemFill(pData->Data, Size(), BackCol); };

// kopie nov�ch dat obr�zku (rozm�ry z�stanou nezm�n�ny) - zajist� odpojen� dat
	void CopyData(BYTE* src);

// kopie nov�ch dat obr�zku s konverz� (rozm�ry z�stanou nezm�n�ny) - zajist� odpojen� dat
	void CopyKonvData(BYTE* src);

// kopie do vlastn�ho bufferu p�ed modifikac�
	void CopyWrite();

// vypr�zdn�n� obr�zku (uvoln�n� dat)
	void Empty();

// test, zda je obr�zek pr�zdn�
	inline BOOL IsEmpty() { return ((DWORD)pData == (DWORD)&EmptyPictureData); };
	inline BOOL IsNotEmpty() { return ((DWORD)pData != (DWORD)&EmptyPictureData); };

// vytvo�en� nov�ho obr�zku (p�ipraveno k z�pisu, data jsou n�hodn�)
	void New(int width, int height);

// nastaven� nov�ch rozm�r� obr�zku (nov� data jsou vymaz�na)
	void Resize(int width, int height);

// exportov�n� obr�zku do bufferu TRUECOLORALPHABETA (vrac� buffer)
	BYTE* ExportTrueAlphaBeta();

// importov�n� obr�zku z bufferu TRUECOLORALPHABETA
	void ImportTrueAlphaBeta(BYTE* buf, BOOL dith);

// zm�na velikosti obr�zku (inter = interpolovat, dith = ditherovat)
	void Zoom(int width, int height, BOOL inter, BOOL dith);

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

// poskytnut� p��stupu k bodu (bez kontroly offsetu/indexu)
	inline BYTE& operator[] (const int off) 
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline const BYTE& operator[] (const int off) const
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline BYTE& At(const int off) 
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline const BYTE& At(const int off) const
		{ ASSERT(IsValid(off)); return pData->Data[off]; }

	inline BYTE& At(const int x, const int y) 
		{ ASSERT(IsValid(x, y)); return pData->Data[x + y*Width()]; }

	inline const BYTE& At(const int x, const int y) const
		{ ASSERT(IsValid(x, y)); return pData->Data[x + y*Width()]; }

// poskytnut� bodu (s kontrolou platnosti offsetu/indexu)
	BYTE _fastcall Get(const int off) const;
	BYTE _fastcall Get(const int x, const int y) const;

// nastaven� bodu (s kontrolou platnosti offsetu/indexu)
	void _fastcall Set(const int off, const BYTE data);
	void _fastcall Set(const int x, const int y, const BYTE data);

// p�evr�cen� obr�zku
	void XFlip();
	void YFlip();

// zm�na jasu obr�zku
	void Level(double koef, bool dith);

// oto�en� obr�zku
	void Rotate(double angle, bool inter, bool dith);

// na�ten� obr�zku ze souboru (tex = importovat texturu)
	void LoadFile(bool tex);

// ulo�en� obr�zku do souboru
	void SaveFile();

// oper�tor p�i�azen�
	const CPicture& operator= (const CPicture& src);
	const CPicture& operator= (PICTUREDATA* src);
	const CPicture& operator= (CIcon& icon);
	const CPicture& operator= (CMap& map);

// dekomprimace dat obr�zku (jsou-li komprimov�na)
	void DeComp();

// maskov�n� obr�zku
	void Mask(CPicture& mask);

// z�m�na/n�hrada barvy v obr�zku
	void XCol(BYTE col1, BYTE col2);
	void SCol(BYTE oldcol, BYTE newcol);

// oper�tory porovn�n�
	friend inline BOOL operator==(const CPicture& s1, const CPicture& s2)
		{ return (DWORD)s1.pData == (DWORD)s2.pData; };

	friend inline BOOL operator!=(const CPicture& s1, const CPicture& s2)
		{ return (DWORD)s1.pData != (DWORD)s2.pData; };
};

/////////////////////////////////////////////////////////////////////////////
// konverze obr�zku v bufferu TrueColorAlphaBeta (vrac� nov� buffer)

BYTE* ZoomTrueColor(BYTE* buf, int oldwidth, int oldheight, int newwidth, int newheight);


/***************************************************************************\
*																			*
*								Buffer obr�zk�								*
*																			*
\***************************************************************************/

class CBufPic
{

// ------------------------- intern� prom�nn� a funkce ----------------------

private:

// prom�nn�
	CPicture*	m_Data;		// ukazatel na data
	int			m_Num;		// po�et platn�ch polo�ek v bufferu
	int			m_Max;		// velikost bufferu (polo�ek)

// vytvo�en� nov� polo�ky
	inline int NewItem()
	{
		int i = m_Num;
		if (i >= m_Max)
		{
			NewData();				// vytvo�en� nov�ch dat
		}

		m_Num = i + 1;
		return i;
	};

// vytvo�en� nov�ch dat (odd�leno kv�li lep�� optimalizaci)
	void NewData();

// ---------------------------- ve�ejn� funkce ------------------------------

public:

// konstruktor a destruktor
	CBufPic();
	~CBufPic();

// statick� konstruktor a destruktor
	void Init();
	void Term();

// zru�en� v�ech polo�ek v bufferu
	void DelAll();

// poskytnut� bufferu dat
	inline CPicture* Data() const { return m_Data; };

// poskytnut� po�tu platn�ch polo�ek v bufferu
	inline int Num() const { return m_Num; };

// poskytnut� velikosti bufferu
	inline int Max() const { return m_Max; };

// kontrola platnosti polo�ky
	inline BOOL IsValid(const int index) const
		{ return ((DWORD)index < (DWORD)m_Num); };

	inline BOOL IsNotValid(const int index) const
		{ return ((DWORD)index >= (DWORD)m_Num); };

// poskytnut� p��stupu k polo�ce (bez kontroly indexu)
	inline CPicture& operator[] (const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const CPicture& operator[] (const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline CPicture& At(const int index)
		{ ASSERT(IsValid(index)); return m_Data[index]; }

	inline const CPicture& At(const int index) const
		{ ASSERT(IsValid(index)); return m_Data[index]; }

// poskytnut� polo�ky (s kontrolou platnosti indexu)
	const CPicture& _fastcall Get(const int index) const;

// nastaven� polo�ky (s kontrolou platnosti indexu)
	void _fastcall Set(const int index, const CPicture& data);

// vypr�zdn�n� polo�ky (bez jej�ho zru�en� - jen pro uvoln�n� dat)
	void _fastcall Empty(const int index);

// zru�en� polo�ek z konce bufferu
	void _fastcall Del(int num);

// vytvo�en� pr�zdn� polo�ky (vrac� index polo�ky)
	int New();
	int New(int width, int height);			// obr�zek vyma�e pr�hlednou barvou

// p�id�n� polo�ky (vrac� index polo�ky)
	int _fastcall Add(const CPicture& data);
	int _fastcall Add(PICTUREDATA* data);

// duplikace polo�ky (s kontrolou platnosti indexu, vrac� index prvn� polo�ky)
	int _fastcall Dup(const int index);
	int _fastcall Dup(const int index, int num);

// oper�tor p�i�azen�
	const CBufPic& _fastcall operator= (const CBufPic& src);
};

