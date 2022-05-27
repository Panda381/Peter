
#include "Main.h"

/***************************************************************************\
*																			*
*								Buffer undo/redo							*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// konstruktor a destruktor

CBufUndo::CBufUndo() 
{ 
	m_UndoData = NULL;
	m_UndoNum = 0;
	m_UndoMax = 0;
	m_UndoSize = 0;
	m_UndoLimit = 10000000;
	m_UndoRecs = 0;

	m_RedoData = NULL;
	m_RedoNum = 0;
	m_RedoMax = 0;
	m_RedoRecs = 0;

	m_Play = FALSE;
	m_Undo = TRUE;
	NulRedraw();

	m_EditMode = -1;
	m_EditIndex = -1;
};

CBufUndo::~CBufUndo() 
{ 
	DelAll();
};


/////////////////////////////////////////////////////////////////////////////
// statick� konstruktor a destruktor

void CBufUndo::Init()
{ 
	m_UndoData = NULL;
	m_UndoNum = 0;
	m_UndoMax = 0;
	m_UndoSize = 0;
	m_UndoLimit = 10000000;
	m_UndoRecs = 0;

	m_RedoData = NULL;
	m_RedoNum = 0;
	m_RedoMax = 0;
	m_RedoRecs = 0;

	m_Prog.Init();
	m_Real.Init();
	m_Text.Init();
	m_Bool.Init();
	m_Icon.Init();
	m_Map.Init();
	m_Picture.Init();
	m_Sprite.Init();
	m_Sound.Init();
	m_Music.Init();

	m_Play = FALSE;
	m_Undo = TRUE;
	NulRedraw();

	m_EditMode = -1;
	m_EditIndex = -1;
};

void CBufUndo::Term()
{ 
	DelAll();

	m_Prog.Term();
	m_Real.Term();
	m_Text.Term();
	m_Bool.Term();
	m_Icon.Term();
	m_Map.Term();
	m_Picture.Term();
	m_Sprite.Term();
	m_Sound.Term();
	m_Music.Term();
};


/////////////////////////////////////////////////////////////////////////////
// zru�en� v�ech polo�ek v bufferu

void CBufUndo::DelAll()
{
	MemFree(m_UndoData);
	m_UndoData = NULL;
	m_UndoNum = 0;
	m_UndoMax = 0;
	m_UndoSize = 0;
	m_UndoRecs = 0;

	MemFree(m_RedoData);
	m_RedoData = NULL;
	m_RedoNum = 0;
	m_RedoMax = 0;
	m_RedoRecs = 0;

	m_Prog.DelAll();
	m_Real.DelAll();
	m_Text.DelAll();
	m_Bool.DelAll();
	m_Icon.DelAll();
	m_Map.DelAll();
	m_Picture.DelAll();
	m_Sprite.DelAll();
	m_Sound.DelAll();
	m_Music.DelAll();

	UpdateUndoRedo();
};


/////////////////////////////////////////////////////////////////////////////
// vypr�zdn�n� bufferu REDO (nen�-li p�ehr�v�n�)

void CBufUndo::NulRedo()
{
	if (m_Undo && (m_RedoNum > 0) && !m_Play)
	{
		while (m_RedoNum > 0)
		{
			DelRedo();
		}
		UpdateUndoRedo();
	}
}


/////////////////////////////////////////////////////////////////////////////
// vytvo�en� nov� polo�ky (vrac� NULL=chyba pam�ti)

UNDOITEM* CBufUndo::New()
{
	UNDOITEM* item;

// z�znam prob�h� do UNDO bufferu
	if (m_Undo)
	{

// vypr�zdn�n� REDO bufferu
		NulRedo();

// index nov� polo�ky
		int index = m_UndoNum;

// zv�t�en� bufferu (o 4 KB)
		if (index >= m_UndoMax)
		{
			int undomax = m_UndoMax + 128;
			UNDOITEM* undodata = (UNDOITEM*)MemSize(m_UndoData, undomax*sizeof(UNDOITEM));
			if (undodata == NULL) return NULL;
			m_UndoMax = undomax;
			m_UndoData = undodata;
		}

// adresa nov� polo�ky
		m_UndoNum++;
		item = m_UndoData + index;
	}

// z�znam prob�h� do UNDO bufferu
	else
	{

// index nov� polo�ky
		int index = m_RedoNum;

// zv�t�en� bufferu (o 4 KB)
		if (index >= m_RedoMax)
		{
			int redomax = m_RedoMax + 128;
			UNDOITEM* redodata = (UNDOITEM*)MemSize(m_RedoData, redomax*sizeof(UNDOITEM));
			if (redodata == NULL) return NULL;
			m_RedoMax = redomax;
			m_RedoData = redodata;
		}

// adresa nov� polo�ky
		m_RedoNum++;
		item = m_RedoData + index;
	}

// vymaz�n� nov� polo�ky
	MemFill(item, SIZEOFUNDOITEM, -1);
	item->Stop = FALSE;

	return item;
}


/////////////////////////////////////////////////////////////////////////////
// zji�t�n� p�ibli�n� velikosti datov� polo�ky (jen kv�li kontrole p�ete�en�)

int _fastcall CBufUndo::GetSize(int bufID, int index)
{
	if (index < 0) return 0;

	switch (bufID)
	{
	case BufIntID:
	case BufClsID:
	case BufObjID:
	case BufLocID:
	case BufEdiID:
	case BufStrID:
		return SIZEOFPROGITEM;

	case BufNumID:
		return sizeof(m_Real[0]);

	case BufTxtID:
		return m_Text[index].MultiText().Length() * sizeof(TCHAR);

	case BufLogID:
		return sizeof(m_Bool[0]);

	case BufIcoID:
		return ICONSIZE;

	case BufMapID:
		return m_Map[index].Size();

	case BufPicID:
		return m_Picture[index].Size();

	case BufSprID:
		return m_Sprite[index].Size();

	case BufSndID:
		return m_Sound[index].Size();

	case BufMusID:
		return m_Music[index].Size();
	}

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// p�i�ten� velikosti polo�ky (jen pokud je z�znam do UNDO)

void _fastcall CBufUndo::AddSize(UNDOITEM* item)
{
	if (m_Undo)
	{
		m_UndoSize += GetSize(item->DatBufID, item->DatIndex);
	}
}


/////////////////////////////////////////////////////////////////////////////
// zru�en� datov� polo�ky z buffer�

void _fastcall CBufUndo::DelData(int bufID, int index)
{
	switch (bufID)
	{
	case BufIntID:
	case BufClsID:
	case BufObjID:
	case BufLocID:
	case BufEdiID:
	case BufStrID:
		m_Prog.Del0(index);
		break;

	case BufNumID:
		m_Real.Del(index);
		break;

	case BufTxtID:
		m_Text.Del(index);
		break;

	case BufLogID:
		m_Bool.Del(index);
		break;

	case BufIcoID:
		m_Icon.Del(index);
		break;

	case BufMapID:
		m_Map.Del(index);
		break;

	case BufPicID:
		m_Picture.Del(index);
		break;

	case BufSprID:
		m_Sprite.Del(index);
		break;

	case BufSndID:
		m_Sound.Del(index);
		break;

	case BufMusID:
		m_Music.Del(index);
		break;
	}
}


/////////////////////////////////////////////////////////////////////////////
// zru�en� posledn� polo�ky z undo/redo

void CBufUndo::DelUndo()
{
	if (m_UndoNum > 0)
	{
		m_UndoNum--;
		UNDOITEM* item = m_UndoData + m_UndoNum;
		if (item->Stop) m_UndoRecs--;
		int blok = item->DatBufID;
		int inx = item->DatIndex;
		m_UndoSize -= GetSize(blok, inx);
		DelData(blok, inx);
	}
}

void CBufUndo::DelRedo()
{
	if (m_RedoNum > 0)
	{
		m_RedoNum--;
		UNDOITEM* item = m_RedoData + m_RedoNum;
		if (item->Stop) m_RedoRecs--;
		int blok = item->DatBufID;
		int inx = item->DatIndex;
		DelData(blok, inx);
	}
}


/////////////////////////////////////////////////////////////////////////////
// redukce velikosti UNDO bufferu

void CBufUndo::Reduct()
{
	int num = 0;
	int blok;
	int inx;
	UNDOITEM* item = m_UndoData;

	while ((m_UndoRecs > 1) && (m_UndoSize > m_UndoLimit))
	{
		do {
			blok = item->DatBufID;
			inx = item->DatIndex;
			m_UndoSize -= GetSize(blok, inx);
			DelData(blok, inx);
			num++;
			item++;
		} while (!item[-1].Stop);
		m_UndoRecs--;
	}

	if (num > 0)
	{
		MemCopy(m_UndoData, m_UndoData + num, (m_UndoNum - num) * sizeof(UNDOITEM));
		m_UndoNum -= num;
	}
}


/////////////////////////////////////////////////////////////////////////////
// ukon�en� z�znamu operac� (nastav� p��znak konce posloupnosti)

void CBufUndo::Stop()
{
// zastaven� nahr�v�n� UNDO z�znamu
	if (m_Undo)
	{
		int inx = m_UndoNum - 1;
		if ((inx >= 0) && !m_UndoData[inx].Stop)
		{
			m_UndoData[inx].Stop = TRUE;
			m_UndoRecs++;
		}	
	}
	else

// zastaven� nahr�v�n� REDO z�znamu
	{
		int inx = m_RedoNum - 1;
		if ((inx >= 0) && !m_RedoData[inx].Stop)
		{
			m_RedoData[inx].Stop = TRUE;
			m_RedoRecs++;
		}
	}

// proveden� redukce
	Reduct();

// aktualizace voleb undo/redo
	UpdateUndoRedo();
}


/////////////////////////////////////////////////////////////////////////////
// aktualizace voleb undo a redo

void CBufUndo::UpdateUndoRedo()
{
	if (!m_Play)
	{
		EnableCommand(IDN_UNDO, (m_UndoRecs > 0));
		EnableCommand(IDN_REDO, (m_RedoRecs > 0));
	}
}


/////////////////////////////////////////////////////////////////////////////
// vypnut� p��znak� p�ekreslen� okna

void CBufUndo::NulRedraw()
{
	for (int i = 0; i < PROGBUFNUM; i++)
	{
		m_Redraw[i] = TRUE;
	}
}


/////////////////////////////////////////////////////////////////////////////
// vypnut�/zapnut� p�ekreslov�n� okna

void CBufUndo::SetRedraw(int bufID, BOOL redraw)
{
	if (((DWORD)bufID < (DWORD)PROGBUFNUM) && (m_Redraw[bufID] != redraw))
	{
		m_Redraw[bufID] = redraw;
		::SendMessage(BufProg[bufID].Tree(), WM_SETREDRAW, redraw, 0);
	}
}


/////////////////////////////////////////////////////////////////////////////
// ozna�en� editovan� polo�ky

void CBufUndo::SelectEdit(int bufID, int index)
{
	int def = BufObj.Def();

	if (BufObj.IsValid(def) && 
		(BufObj[def].DatBlok == bufID) && 
		(BufObj[def].DatIndex == index))
	{
		BufObj.Select(def);
		return;
	}

	def = BufLoc.Def();

	if (BufLoc.IsValid(def) && 
		(BufLoc[def].DatBlok == bufID) && 
		(BufLoc[def].DatIndex == index))
	{
		BufLoc.Select(def);
		return;
	}


}


/////////////////////////////////////////////////////////////////////////////
// proveden� operace UNDO

void CBufUndo::Undo()
{
// kontrola povolen� po�adavku
	if ((m_UndoRecs <= 0) || !ProgMode || m_Play) return;

// zapnut� p��znaku p�ehr�v�n� UNDO, nahr�v�n� REDO
	m_Undo = FALSE;
	m_Play = TRUE;

// p�ednastaven� - nen� nastaven� edita�n�ho m�du
	m_EditMode = -1;
	m_SelectMode = -1;
	m_MapMode = -1;

// vypnut� p��znak� p�ekreslov�n� oken
	NulRedraw();

// p�ehr�t� z�znamu UNDO
	if (m_UndoNum > 0)
	{
		do {
			Play(m_UndoData + m_UndoNum - 1);
			DelUndo();
		} while ((m_UndoNum > 0) && (!m_UndoData[m_UndoNum-1].Stop));
	}

// ukon�en� z�znamu pro REDO
	Stop();

// zapnut� p�ekreslov�n� oken
	for (int i = 0; i < PROGBUFNUM; i++)
	{
		SetRedraw(i, TRUE);
	}

// nastaven� edita�n�ho m�du
	if (m_EditMode >= 0)
	{
		if (!SetEditMode(m_EditMode, m_EditIndex))
		{
			SetEditMode(BufEdiID, 0);
		}
	}

	if (m_SelectMode >= 0)
	{
		SelectEdit(m_SelectMode, m_SelectIndex);
	}

	if (m_MapMode >= 0)
	{
		EditMap::SetMode(m_MapMode);
	}

// vypnut� p��znaku p�ehr�v�n� UNDO
	m_Play = FALSE;
	m_Undo = TRUE;

// p��znak modifikace souboru
	SetModi();

// aktualizace voleb undo/redo
	UpdateUndoRedo();

// aktualizace zobrazen� informac� o polo�ce
	ProgAktItem();
}


/////////////////////////////////////////////////////////////////////////////
// proveden� operace REDO

void CBufUndo::Redo()
{
// kontrola povolen� po�adavku
	if ((m_RedoRecs <= 0) || !ProgMode || m_Play) return;

// zapnut� p��znaku p�ehr�v�n� REDO, nahr�v�n� UNDO (=implicitn�)
	m_Play = TRUE;

// p�ednastaven� - nen� nastaven� edita�n�ho m�du
	m_EditMode = -1;
	m_SelectMode = -1;
	m_MapMode = -1;

// vypnut� p��znak� p�ekreslov�n� oken
	NulRedraw();

// p�ehr�t� z�znamu REDO
	if (m_RedoNum > 0)
	{
		do {
			Play(m_RedoData + m_RedoNum - 1);
			DelRedo();
		} while ((m_RedoNum > 0) && (!m_RedoData[m_RedoNum-1].Stop));
	}

// ukon�en� z�znamu pro UNDO
	Stop();

// zapnut� p�ekreslov�n� oken
	for (int i = 0; i < PROGBUFNUM; i++)
	{
		SetRedraw(i, TRUE);
	}

// nastaven� edita�n�ho m�du
	if (m_EditMode >= 0)
	{
		SetEditMode(m_EditMode, m_EditIndex);
	}

	if (m_SelectMode >= 0)
	{
		SelectEdit(m_SelectMode, m_SelectIndex);
	}

	if (m_MapMode >= 0)
	{
		EditMap::SetMode(m_MapMode);
	}

// vypnut� p��znaku p�ehr�v�n� REDO
	m_Play = FALSE;

// p��znak modifikace souboru
	SetModi();

// aktualizace voleb undo/redo
	UpdateUndoRedo();

// aktualizace zobrazen� informac� o polo�ce
	ProgAktItem();
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o p�id�n� polo�ky do programov�ho bufferu (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddProgIns(int bufID, int index)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_INSERT;
	item->BufID = bufID;
	item->Index = index;

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o zru�en� polo�ky z programov�ho bufferu (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddProgDel(int bufID, int index, PROGITEM* data)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_DELETE;
	item->BufID = bufID;
	item->Index = index;

// �schova polo�ky do bufferu
	item->DatBufID = BufEdiID;
	item->DatIndex = m_Prog.Insert0(data);

// zm�na ��ta�e velikosti
	AddSize(item);

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o p�esunu polo�ky v programov�m bufferu (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddProgMove(int bufID, int index, int oldparent, int oldprev)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_MOVE;
	item->BufID = bufID;
	item->Index = index;
	item->Param1 = oldparent;
	item->Param2 = oldprev;

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o zm�n� p��znaku LOCK prvku (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddProgLock(int bufID, int index)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_LOCK;
	item->BufID = bufID;
	item->Index = index;

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o zm�n� p��znaku OFF prvku (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddProgOff(int bufID, int index)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_OFF;
	item->BufID = bufID;
	item->Index = index;

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o zm�n� jm�na prvku (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddProgName(int bufID, int index, int name)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_NAME;
	item->BufID = bufID;
	item->Index = index;
	item->Param1 = name;

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o nastaven� jm�na prvku (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddProgNameSet(int bufID, int index, const CMultiText& data)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_NAMESET;
	item->BufID = bufID;
	item->Index = index;

// �schova polo�ky do bufferu
	item->DatBufID = BufTxtID;
	item->DatIndex = m_Text.Add(data);

// zm�na ��ta�e velikosti
	AddSize(item);

// proveden� redukce
	Reduct();
	return true;
}

bool CBufUndo::AddProgNameSet(int bufID, int index, const CText& data)
{
	CMultiText txt;
	txt = Text.Get(index);
	txt.MultiText(data);
	return AddProgNameSet(bufID, index, txt);
}

/////////////////////////////////////////////////////////////////////////////
// z�znam o zm�n� ikony prvku (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddProgIcon(int bufID, int index, int icon)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_ICON;
	item->BufID = bufID;
	item->Index = index;
	item->Param1 = icon;

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o p�id�n� polo�ky do bufferu ��sel (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddRealIns(int index)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_INSERT;
	item->BufID = BufNumID;
	item->Index = index;

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o zru�en� polo�ky z bufferu ��sel (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddRealDel(int index, double data)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_DELETE;
	item->BufID = BufNumID;
	item->Index = index;

// �schova polo�ky do bufferu
	item->DatBufID = BufNumID;
	item->DatIndex = m_Real.Add(data);

// zm�na ��ta�e velikosti
	AddSize(item);

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o zm�n� polo�ky v bufferu ��sel (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddRealSet(int index, double data)
{
// ��slo se ukl�d� jen jednou
	if (TestSet(BufNumID, index)) return true;

// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_SET;
	item->BufID = BufNumID;
	item->Index = index;

// �schova polo�ky do bufferu
	item->DatBufID = BufNumID;
	item->DatIndex = m_Real.Add(data);

// zm�na ��ta�e velikosti
	AddSize(item);

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o p�id�n� polo�ky do bufferu text� (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddTextIns(int index)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_INSERT;
	item->BufID = BufTxtID;
	item->Index = index;

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o zru�en� polo�ky z bufferu text� (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddTextDel(int index, const CMultiText& data)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_DELETE;
	item->BufID = BufTxtID;
	item->Index = index;

// �schova polo�ky do bufferu
	item->DatBufID = BufTxtID;
	item->DatIndex = m_Text.Add(data);

// zm�na ��ta�e velikosti
	AddSize(item);

// proveden� redukce
	Reduct();
	return true;
}

bool CBufUndo::AddTextDel(int index, const CText& data)
{
	CMultiText txt;
	txt = Text.Get(index);
	txt.MultiText(data);
	return AddTextDel(index, txt);
}

/////////////////////////////////////////////////////////////////////////////
// z�znam o zm�n� polo�ky v bufferu text� (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddTextSet(int index, const CMultiText& data)
{
// text se ukl�d� jen jednou
	if (TestSet(BufTxtID, index)) return true;

// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_SET;
	item->BufID = BufTxtID;
	item->Index = index;

// �schova polo�ky do bufferu
	item->DatBufID = BufTxtID;
	item->DatIndex = m_Text.Add(data);

// zm�na ��ta�e velikosti
	AddSize(item);

// proveden� redukce
	Reduct();
	return true;
}

bool CBufUndo::AddTextSet(int index, const CText& data)
{
	CMultiText txt;
	txt = Text.Get(index);
	txt.MultiText(data);
	return AddTextSet(index, txt);
}

/////////////////////////////////////////////////////////////////////////////
// z�znam o p�id�n� polo�ky do bufferu logick�ch hodnot (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddBoolIns(int index)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_INSERT;
	item->BufID = BufLogID;
	item->Index = index;

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o zru�en� polo�ky z bufferu logick�ch hodnot (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddBoolDel(int index, BOOL data)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_DELETE;
	item->BufID = BufLogID;
	item->Index = index;

// �schova polo�ky do bufferu
	item->DatBufID = BufLogID;
	item->DatIndex = m_Bool.Add(data);

// zm�na ��ta�e velikosti
	AddSize(item);

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o zm�n� polo�ky v bufferu logick�ch hodnot (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddBoolSet(int index, BOOL data)
{
// logick� hodnota se ukl�d� jen jednou
	if (TestSet(BufLogID, index)) return true;

// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_SET;
	item->BufID = BufLogID;
	item->Index = index;

// �schova polo�ky do bufferu
	item->DatBufID = BufLogID;
	item->DatIndex = m_Bool.Add(data);

// zm�na ��ta�e velikosti
	AddSize(item);

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o p�id�n� polo�ky do bufferu ikon (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddIconIns(int index)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_INSERT;
	item->BufID = BufIcoID;
	item->Index = index;

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o zru�en� polo�ky z bufferu ikon (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddIconDel(int index, const CIcon& data)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_DELETE;
	item->BufID = BufIcoID;
	item->Index = index;

// �schova polo�ky do bufferu
	item->DatBufID = BufIcoID;
	item->DatIndex = m_Icon.Add(data);

// zm�na ��ta�e velikosti
	AddSize(item);

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o zm�n� polo�ky v bufferu ikon (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddIconSet(int index, const CIcon& data)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_SET;
	item->BufID = BufIcoID;
	item->Index = index;

// �schova polo�ky do bufferu
	item->DatBufID = BufIcoID;
	item->DatIndex = m_Icon.Add(data);

// zm�na ��ta�e velikosti
	AddSize(item);

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o p�id�n� polo�ky do bufferu ploch (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddMapIns(int index)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_INSERT;
	item->BufID = BufMapID;
	item->Index = index;

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o zru�en� polo�ky z bufferu ploch (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddMapDel(int index, const CMap& data)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_DELETE;
	item->BufID = BufMapID;
	item->Index = index;

// �schova polo�ky do bufferu
	item->DatBufID = BufMapID;
	item->DatIndex = m_Map.Add(data);

// zm�na ��ta�e velikosti
	AddSize(item);

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o zm�n� polo�ky v bufferu ploch (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddMapSet(int index, const CMap& data, int mode)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_SET;
	item->BufID = BufMapID;
	item->Index = index;

// �schova polo�ky do bufferu
	item->DatBufID = BufMapID;
	item->DatIndex = m_Map.Add(data);
	item->Param1 = mode;

// zm�na ��ta�e velikosti
	AddSize(item);

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o zm�n� dat - ikona plochy (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddMapIcoSet(int index, int offs, int icon)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_SETICON;
	item->BufID = BufMapID;
	item->Index = index;
	item->Param1 = offs;
	item->Param2 = icon;

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o p�id�n� polo�ky do bufferu obr�zk� (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddPicIns(int index)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_INSERT;
	item->BufID = BufPicID;
	item->Index = index;

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o zru�en� polo�ky z bufferu obr�zk� (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddPicDel(int index, const CPicture& data)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_DELETE;
	item->BufID = BufPicID;
	item->Index = index;

// �schova polo�ky do bufferu
	item->DatBufID = BufPicID;
	item->DatIndex = m_Picture.Add(data);

// zm�na ��ta�e velikosti
	AddSize(item);

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o zm�n� polo�ky v bufferu obr�zk� (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddPicSet(int index, const CPicture& data)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_SET;
	item->BufID = BufPicID;
	item->Index = index;

// �schova polo�ky do bufferu
	item->DatBufID = BufPicID;
	item->DatIndex = m_Picture.Add(data);

// zm�na ��ta�e velikosti
	AddSize(item);

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o p�id�n� polo�ky do bufferu sprajt� (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddSprIns(int index)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_INSERT;
	item->BufID = BufSprID;
	item->Index = index;

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o zru�en� polo�ky z bufferu sprajt� (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddSprDel(int index, const CSprite& data)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_DELETE;
	item->BufID = BufSprID;
	item->Index = index;

// �schova polo�ky do bufferu
	item->DatBufID = BufSprID;
	item->DatIndex = m_Sprite.Add(data);

// zm�na ��ta�e velikosti
	AddSize(item);

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o zm�n� polo�ky v bufferu sprajt� (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddSprSet(int index, const CSprite& data)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_SET;
	item->BufID = BufSprID;
	item->Index = index;

// �schova polo�ky do bufferu
	item->DatBufID = BufSprID;
	item->DatIndex = m_Sprite.Add(data);

// zm�na ��ta�e velikosti
	AddSize(item);

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o zm�n� dat - obr�zek sprajtu (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddSprPicSet(int index, int pic, CPicture& data)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_SETPIC;
	item->BufID = BufSprID;
	item->Index = index;

// �schova polo�ky do bufferu
	item->DatBufID = BufPicID;
	item->DatIndex = m_Picture.Add(data);
	item->Param1 = pic;

// zm�na ��ta�e velikosti
	AddSize(item);

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o p�id�n� polo�ky do bufferu zvuk� (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddSndIns(int index)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_INSERT;
	item->BufID = BufSndID;
	item->Index = index;

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o zru�en� polo�ky z bufferu zvuk� (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddSndDel(int index, const CSound& data)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_DELETE;
	item->BufID = BufSndID;
	item->Index = index;

// �schova polo�ky do bufferu
	item->DatBufID = BufSndID;
	item->DatIndex = m_Sound.Add(data);

// zm�na ��ta�e velikosti
	AddSize(item);

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o zm�n� polo�ky v bufferu zvuk� (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddSndSet(int index, const CSound& data)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_SET;
	item->BufID = BufSndID;
	item->Index = index;

// �schova polo�ky do bufferu
	item->DatBufID = BufSndID;
	item->DatIndex = m_Sound.Add(data);

// zm�na ��ta�e velikosti
	AddSize(item);

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o p�id�n� polo�ky do bufferu hudby (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddMusIns(int index)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_INSERT;
	item->BufID = BufMusID;
	item->Index = index;

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o zru�en� polo�ky z bufferu hudby (vrac� FALSE=chyba pam�ti)

bool CBufUndo::AddMusDel(int index, const CMusic& data)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_DELETE;
	item->BufID = BufMusID;
	item->Index = index;

// �schova polo�ky do bufferu
	item->DatBufID = BufMusID;
	item->DatIndex = m_Music.Add(data);

// zm�na ��ta�e velikosti
	AddSize(item);

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// z�znam o zm�n� polo�ky v bufferu hudby

bool CBufUndo::AddMusSet(int index, const CMusic& data)
{
// vytvo�en� polo�ky
	UNDOITEM* item = New();
	if (item == NULL) return false;

// napln�n� z�znamu
	item->Oper = UNDO_SET;
	item->BufID = BufMusID;
	item->Index = index;

// �schova polo�ky do bufferu
	item->DatBufID = BufMusID;
	item->DatIndex = m_Music.Add(data);

// zm�na ��ta�e velikosti
	AddSize(item);

// proveden� redukce
	Reduct();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// test, zda je polo�ka pro zm�nu dat ji� v bufferu

bool CBufUndo::TestSet(int bufID, int index)
{
	if (!m_Play && m_Undo && (m_UndoNum > 0))
	{
		UNDOITEM* item = m_UndoData + m_UndoNum - 1;
		if ((item->BufID == bufID) &&
			(item->Index == index) &&
			(item->Oper == UNDO_SET))
		{
			NulRedo();
			return true;
		}
	}
	return false;
}


/////////////////////////////////////////////////////////////////////////////
// zafixov�n� posledn�ho z�znamu o zm�n�

void CBufUndo::Fixup()
{
	if (!m_Play && m_Undo && (m_UndoNum > 0))
	{
		UNDOITEM* item = m_UndoData + m_UndoNum - 1;
		if (item->Oper == UNDO_SET)
		{
			item->Oper = UNDO_SETFIX;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// p�ehr�t� jedn� polo�ky

void CBufUndo::Play(UNDOITEM* item)
{
	int oper = item->Oper;
	int inx = item->Index;
	int datinx = item->DatIndex;

	switch (item->BufID)
	{
	case BufIntID:
	case BufClsID:
	case BufObjID:
	case BufLocID:
	case BufEdiID:
	case BufStrID:
		{
			CBufProg* buf = BufProg + item->BufID;
			SetRedraw(item->BufID, FALSE);

			switch (oper)
			{
			case UNDO_INSERT:
				buf->Del(inx);
				break;

			case UNDO_DELETE:
				buf->UndoIns(&m_Prog[datinx], inx);
				break;

			case UNDO_MOVE:
				buf->Move(item->Param1, item->Param2, inx);
				break;

			case UNDO_LOCK:
				DispItem(item->BufID, inx);
				ProgOnLock();
				break;

			case UNDO_OFF:
				DispItem(item->BufID, inx);
				ProgOnOff();
				break;

			case UNDO_NAME:
				{
					DispItem(item->BufID, inx);
					PROGITEM* item2 = &buf->At(inx);

					AddProgName(item->BufID, inx, item2->Name);

					item2->Name = item->Param1;
					CText text;
					text = Text.GetTxt(buf->GetText(inx));

					if (item2->HTree)
					{
						TV_ITEM tvi;
						tvi.mask = TVIF_TEXT;
						tvi.hItem = item2->HTree;
						tvi.pszText = (LPTSTR)(LPCTSTR)text;
						::SendMessage(buf->Tree(), TVM_SETITEM, 0, (LPARAM)&tvi);
					}

					if (item2->Refer)
					{
						AktNameTree(BufObjID, text, buf->BufID(), inx);
						AktNameTree(BufLocID, text, buf->BufID(), inx);
						AktNameTree(BufEdiID, text, buf->BufID(), inx);
						AktNameTree(BufClsID, text, buf->BufID(), inx);
					}
				}
				break;

			case UNDO_NAMESET:
				{
					DispItem(item->BufID, inx);
					PROGITEM* item2 = &buf->At(inx);

					AddProgNameSet(item->BufID, inx, Text.Get(item2->Name));

					CMultiText text;
					text = m_Text[datinx];
					Text.Set(item2->Name, text);

					CText txt;
					txt = text.MultiText();

					if (item2->HTree)
					{
						TV_ITEM tvi;
						tvi.mask = TVIF_TEXT;
						tvi.hItem = item2->HTree;
						tvi.pszText = (LPTSTR)(LPCTSTR)txt;
						::SendMessage(buf->Tree(), TVM_SETITEM, 0, (LPARAM)&tvi);
					}

					if (item2->Refer)
					{
						AktNameTree(BufObjID, txt, buf->BufID(), inx);
						AktNameTree(BufLocID, txt, buf->BufID(), inx);
						AktNameTree(BufEdiID, txt, buf->BufID(), inx);
						AktNameTree(BufClsID, txt, buf->BufID(), inx);
					}
				}
				break;

			case UNDO_ICON:
				DispItem(item->BufID, inx);
				AddProgIcon(item->BufID, inx, buf->At(inx).Icon);
				buf->At(inx).Icon = item->Param1;
				if (buf->At(inx).DatBlok == BufIcoID)
				{
					buf->At(inx).DatIndex = item->Param1;
				}
				UpdateAllTree();
				break;

			default:
				ASSERTERROR;
			}
		}
		break;

	case BufNumID:
		{
			switch (oper)
			{
			case UNDO_INSERT:
				Real.Del(inx);
				break;

			case UNDO_DELETE:
				Real.UndoIns(m_Real[datinx], inx);
				break;

			case UNDO_SETFIX:
			case UNDO_SET:
				AddRealSet(inx, Real.Get(inx));
				Real.Set(inx, m_Real[datinx]);
				m_EditMode = BufNumID;
				m_EditIndex = inx;
				m_SelectMode = BufNumID;
				m_SelectIndex = inx;
				break;

			default:
				ASSERTERROR;
			}
		}
		break;

	case BufTxtID:
		{
			switch (oper)
			{
			case UNDO_INSERT:
				Text.Del(inx);
				break;

			case UNDO_DELETE:
				Text.UndoIns(m_Text[datinx], inx);
				break;

			case UNDO_SETFIX:
			case UNDO_SET:
				AddTextSet(inx, Text.Get(inx));
				Text.Set(inx, m_Text[datinx]);
				m_EditMode = BufTxtID;
				m_EditIndex = inx;
				m_SelectMode = BufTxtID;
				m_SelectIndex = inx;
				break;

			default:
				ASSERTERROR;
			}
		}
		break;

	case BufLogID:
		{
			switch (oper)
			{
			case UNDO_INSERT:
				Bool.Del(inx);
				break;

			case UNDO_DELETE:
				Bool.UndoIns(m_Bool[datinx], inx);
				break;

			case UNDO_SETFIX:
			case UNDO_SET:
				AddBoolSet(inx, Bool.Get(inx));
				Bool.Set(inx, m_Bool[datinx]);
				m_EditMode = BufLogID;
				m_EditIndex = inx;
				m_SelectMode = BufLogID;
				m_SelectIndex = inx;
				break;

			default:
				ASSERTERROR;
			}
		}
		break;

	case BufIcoID:
		{
			switch (oper)
			{
			case UNDO_INSERT:
				Icon.Del(inx);
				break;

			case UNDO_DELETE:
				Icon.UndoIns(m_Icon[datinx], inx);
				break;

			case UNDO_SETFIX:
			case UNDO_SET:
				AddIconSet(inx, Icon.Get(inx));
				Icon.Set(inx, m_Icon[datinx]);
				BufCls.ReDispIcon(inx);
				BufObj.ReDispIcon(inx);
				BufLoc.ReDispIcon(inx);
				BufEdi.ReDispIcon(inx);
				BufStr.ReDispIcon(inx);
				m_EditMode = BufIcoID;
				m_EditIndex = inx;
				m_SelectMode = BufIcoID;
				m_SelectIndex = inx;
				break;

			default:
				ASSERTERROR;
			}
		}
		break;

	case BufMapID:
		{
			switch (oper)
			{
			case UNDO_INSERT:
				if (Map.IsValid(inx))
				{
					Map[inx].RefDec();
					Map.Del(inx);
				}
				break;

			case UNDO_DELETE:
				Map.UndoIns(m_Map[datinx], inx);
				ASSERT(Map.IsValid(inx));
				Map[inx].RefInc();
				break;

			case UNDO_SETFIX:
			case UNDO_SET:
				AddMapSet(inx, Map.Get(inx), item->Param1);
				Map[inx].RefDec();
				Map.Set(inx, m_Map[datinx]);
				Map[inx].RefInc();
				m_EditMode = BufMapID;
				m_EditIndex = inx;
				m_SelectMode = BufMapID;
				m_SelectIndex = inx;
				m_MapMode = item->Param1;
				break;

			case UNDO_SETICON:
				Map[inx].CopyWrite();
				AddMapIcoSet(inx, item->Param1, Map[inx][item->Param1].Icon);
//				Map[inx].
					RefIconDec(Map[inx][item->Param1].Icon);
//				Map[inx].
					RefIconInc(item->Param2);
				Map[inx][item->Param1].Icon = item->Param2;
				if ((EditMode == BufMapID) && (EditItem == inx))
				{
					m_EditMode = BufMapID;
					m_EditIndex = inx;
					m_SelectMode = BufMapID;
					m_SelectIndex = inx;
				}
				break;

			default:
				ASSERTERROR;
			}
		}
		break;

	case BufPicID:
		{
			switch (oper)
			{
			case UNDO_INSERT:
				Picture.Del(inx);
				break;

			case UNDO_DELETE:
				Picture.UndoIns(m_Picture[datinx], inx);
				break;

			case UNDO_SETFIX:
			case UNDO_SET:
				AddPicSet(inx, Picture.Get(inx));
				Picture.Set(inx, m_Picture[datinx]);
				m_EditMode = BufPicID;
				m_EditIndex = inx;
				m_SelectMode = BufPicID;
				m_SelectIndex = inx;
				break;

			default:
				ASSERTERROR;
			}
		}
		break;

	case BufSprID:
		{
			switch (oper)
			{
			case UNDO_INSERT:
				Sprite.Del(inx);
				break;

			case UNDO_DELETE:
				Sprite.UndoIns(m_Sprite[datinx], inx);
				break;

			case UNDO_SETFIX:
			case UNDO_SET:
				AddSprSet(inx, Sprite.Get(inx));
				Sprite.Set(inx, m_Sprite[datinx]);
				m_EditMode = BufSprID;
				m_EditIndex = inx;
				m_SelectMode = BufSprID;
				m_SelectIndex = inx;
				break;

			case UNDO_SETPIC:
				Sprite[inx].CopyWrite();
				AddSprPicSet(inx, item->Param1, Sprite[inx][item->Param1]);
				Sprite[inx].CopyWrite();
				Sprite[inx][item->Param1] = m_Picture[datinx];
				Sprite[inx][item->Param1].CopyWrite();
				EditSprite::Index = inx;
				EditSprite::IndexPic = item->Param1;
				m_EditMode = BufPicID;
				m_EditIndex = -2;
				m_SelectMode = BufSprID;
				m_SelectIndex = inx;
				break;

			default:
				ASSERTERROR;
			}
		}
		break;

	case BufSndID:
		{
			switch (oper)
			{
			case UNDO_INSERT:
				Sound.Del(inx);
				break;

			case UNDO_DELETE:
				Sound.UndoIns(m_Sound[datinx], inx);
				break;

			case UNDO_SETFIX:
			case UNDO_SET:
				AddSndSet(inx, Sound.Get(inx));
				Sound.Set(inx, m_Sound[datinx]);
				m_EditMode = BufSndID;
				m_EditIndex = inx;
				m_SelectMode = BufSndID;
				m_SelectIndex = inx;
				break;

			default:
				ASSERTERROR;
			}
		}
		break;

	case BufMusID:
		{
			switch (oper)
			{
			case UNDO_INSERT:
				Music.Del(inx);
				break;

			case UNDO_DELETE:
				Music.UndoIns(m_Music[datinx], inx);
				break;

//			case UNDO_SETFIX:
//			case UNDO_SET:
//				AddMusSet(inx, Music.Get(inx));
//				Music.Set(inx, m_Music[datinx]);
//				m_EditMode = BufMusID;
//				m_EditIndex = inx;
//				m_SelectMode = BufMusID;
//				m_SelectIndex = inx;
//				break;

			default:
				ASSERTERROR;
			}
		}
		break;

	default:
		ASSERTERROR;
	}
}
