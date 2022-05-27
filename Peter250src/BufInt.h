
/***************************************************************************\
*																			*
*								Buffer cel�ch ��sel							*
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
*							Buffer logick�ch prom�nn�ch						*
*																			*
\***************************************************************************/
// pracuje s hodnotami 0 = FALSE, nenulov� hodnota = TRUE

class CBufBool : public CBufInt 
{ 

public:

// na�ten� logick� hodnoty ze souboru (na��t� do prom�nn�, vrac� TRUE=operace OK)
	bool LoadFile(BOOL& data, CText jmeno);

// ulo�en� logick� hodnoty do souboru form�tu LOG (vrac� TRUE=operace OK)
	bool SaveFile(BOOL data, CText jmeno) const;
};
