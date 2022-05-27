
/***************************************************************************\
*																			*
*								Buffer celých èísel							*
*																			*
\***************************************************************************/

class CInt
{
	int		m_Data;
public:
	inline	CInt() {}
	inline	CInt(int data) { m_Data = data; }
	inline	const CInt operator= (const int src) { m_Data = src; return *this; }
	inline	operator int() const { return m_Data; }
	inline	void Init() { m_Data = 0; }
	inline	void Init(int data) { m_Data = data; }
	inline	void Term() {}
	inline	void Empty() { m_Data = 0; }
};

class CBufInt : public CBuffer<CInt>
{
};


/***************************************************************************\
*																			*
*							Buffer logických promìnných						*
*																			*
\***************************************************************************/
// pracuje s hodnotami 0 = FALSE, nenulová hodnota = TRUE

class CBufBool : public CBufInt 
{ 

public:

// naètení logické hodnoty ze souboru (naèítá do promìnné, vrací TRUE=operace OK)
	bool LoadFile(BOOL& data, CText jmeno);

// uložení logické hodnoty do souboru formátu LOG (vrací TRUE=operace OK)
	bool SaveFile(BOOL data, CText jmeno) const;
};
