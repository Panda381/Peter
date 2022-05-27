
/***************************************************************************\
*																			*
*									Obr�zky									*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// struktura polo�ky dat obr�zku - 16 bajt� + data

typedef struct PICTUREDATA_
{
	long	Refer;					// (4) ��ta� referenc� na obr�zek
	long	Width;					// (4) ���ka obr�zku v bodech
	long	Height;					// (4) v��ka obr�zku v link�ch
	BYTE*	Data;					// (4) ukazatel na data (velikost Width*Height)
} PICTUREDATA;

#define SIZEOFPICTUREDATA	(3*sizeof(long) + sizeof(BYTE*))

extern PICTUREDATA EmptyPictureData;	// data pr�zdn�ho obr�zku (velikost ICONSIZE)


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
	inline void Detach()
	{
		ASSERT(pData);
		ASSERT(pData->Refer > 0);

		if (LongDecrement(&(pData->Refer)))
		{
			ASSERT(pData->Data);
			MemFree(pData->Data);
			MemFree(pData);
#ifdef _DEBUG
			pData = NULL;
#endif
		}
	}

// vytvo�en� nov�ho bufferu - star� buffer mus� b�t odpojen!
	inline void NewBuffer(int width, int height)
	{
		ASSERT((width > 0) && (height > 0));
		if (width <= 0) width = 1;
		if (height <= 0) height = 1;
		PICTUREDATA* data = (PICTUREDATA*)MemGet(SIZEOFPICTUREDATA);
		data->Refer = 1;				// po�et referenc�
		data->Width = width;			// ���ka
		data->Height = height;			// v��ka
		data->Data = (BYTE*)MemGet(width*height);
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

// poskytnut� ukazatele na data
	inline PICTUREDATA* Data() const { return pData; }; // poskytnut� adresy dat
	inline BYTE* DataData() const { return pData->Data; }; // poskytnut� adresy dat

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

// vytvo�en� nov�ho obr�zku (p�ipraveno k z�pisu, data jsou n�hodn�)
	void New(int width, int height);

// nastaven� nov�ch rozm�r� obr�zku (nov� data jsou vymaz�na)
	void Resize(int width, int height);
	void HalfSize();

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

// na�ten� obr�zku ze souboru (TRUE=operace OK)
//	BOOL LoadFile(CString jmeno);

// na�ten� obr�zku ze souboru s polovi�n� velikost� (TRUE=operace OK)
	BOOL LoadFile2(CString jmeno);

// ulo�en� obr�zku do souboru form�tu BMP (FALSE=chyba)
	BOOL SaveFile(CString jmeno) const;

// oper�tor p�i�azen�
	const CPicture& operator= (const CPicture& src);
};

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
	BYTE*		m_Valid;	// p��znaky platnosti polo�ek
	int			m_Num;		// po�et platn�ch polo�ek v bufferu
	int			m_Max;		// velikost bufferu (polo�ek)
	int			m_Next;		// p��t� voln� polo�ka (-1=nen�)

// vytvo�en� nov� polo�ky
	inline int NewItem()
	{
		if (m_Next < 0)				// nen� dal�� polo�ka?
		{
			NewData();				// vytvo�en� nov�ch dat
		}

		int i = m_Next;				// p��t� volna polo�ka
		m_Next = *(int*)(m_Data + i); // dal�� polo�ka
		m_Valid[i] = TRUE;			// nastaven� p��znaku platnosti polo�ky
		m_Num++;					// zv��en� ��ta�e platn�ch polo�ek
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

// zru�en� v�ech polo�ek v bufferu (ukl�d�n� za�ne op�t po �ad� od za��tku)
	void DelAll();

// poskytnut� bufferu dat
	inline CPicture* Data() const { return m_Data; };

// poskytnut� pole platnost�
	inline BYTE* Valid() const { return m_Valid; };

// poskytnut� po�tu platn�ch polo�ek v bufferu
	inline int Num() const { return m_Num; };

// poskytnut� velikosti bufferu (v�etn� zru�en�ch polo�ek)
	inline int Max() const { return m_Max; };

// kontrola platnosti polo�ky
	inline BOOL IsValid(const int index) const
		{ return (((DWORD)index < (DWORD)m_Max) && m_Valid[index]); };

	inline BOOL IsNotValid(const int index) const
		{ return (((DWORD)index >= (DWORD)m_Max) || !m_Valid[index]); };

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

// zru�en� polo�ky (s kontrolou platnosti indexu)
	void _fastcall Del(const int index);

// vytvo�en� pr�zdn� polo�ky (vrac� index polo�ky)
	int New();
	int New(int width, int height);			// obr�zek vyma�e pr�hlednou barvou

// p�id�n� polo�ky (vrac� index polo�ky)
	int _fastcall Add(const CPicture& data);
	int _fastcall Add(PICTUREDATA* data);

// duplikace polo�ky (s kontrolou platnosti indexu, vrac� index polo�ky)
	int _fastcall Dup(const int index);

// oper�tor p�i�azen�
	const CBufPic& operator= (const CBufPic& src);
};

