
/***************************************************************************\
*																			*
*							Buffer re�ln�ch ��sel							*
*																			*
\***************************************************************************/

class CReal
{
	double	m_Data;
public:
	inline	CReal() {}
	inline	CReal(double data) { m_Data = data; }
	inline	const CReal operator= (const double src) { m_Data = src; return *this; }
	inline	operator double() const { return m_Data; }
	inline	void Init() { m_Data = 0; }
	inline	void Init(double data) { m_Data = data; }
	inline	void Term() {}
	inline	void Empty() { m_Data = 0; }
};

class CBufReal : public CBuffer<CReal>
{

// ---------------------------- ve�ejn� funkce ------------------------------

public:

// na�ten� ��sla ze souboru (na��t� do prom�nn�, TRUE=operace OK)
	bool LoadFile(double& data, CText jmeno);

// ulo�en� ��sla do souboru form�tu NUM (FALSE=chyba)
	bool SaveFile(double data, CText jmeno) const;
};
