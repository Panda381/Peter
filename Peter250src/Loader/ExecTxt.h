
/***************************************************************************\
*																			*
*							Prov�d�n� programu - text						*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inline vol�n� p��kazu - text

inline void FText(CString& text)
{
	ExecItem++;
	ExecItem[-1].ExecTxt(text);
}


/////////////////////////////////////////////////////////////////////////////
// na�ten� jm�na souboru

void _fastcall FFileName(CString& text);


void _fastcall FGetStatus(CString& text);		// zji�t�n� textu stavov�ho ��dku
void _fastcall FGetCaption(CString& text);		// zji�t�n� textu nadpisu okna

// funkce
void _fastcall FTextFunc(CString& text);		// funkce s n�vratem textu

// prom�nn�
void _fastcall FTextCRLF(CString& text);		// text CR/LF
void _fastcall FTextEmpty(CString& text);		// pr�zdn� text
void _fastcall FTextObj(CString& text);			// glob�ln� text (Data = index)
void _fastcall FTextObjList(CString& text);		// globaln� text v seznamu (Data = index, List = seznam)
void _fastcall FTextLoc(CString& text);			// lok�ln� text (Data = index)
void _fastcall FTextLocList(CString& text);		// lok�ln� text v seznamu (Data = index, List = seznam)

// konverze
void _fastcall FTextFormat(CString& text);		// p�evod ��sla na text
void _fastcall FTextFormat2(CString& text);		// p�evod ��sla na text - 2 ��slice
void _fastcall FTextFormatH(CString& text);		// p�evod ��sla na text - 8 ��slic HEX
void _fastcall FTextChar(CString& text);		// p�evod ��sla na znak
void _fastcall FTextLower(CString& text);		// p�evod na mal� p�smena
void _fastcall FTextUpper(CString& text);		// p�evod na velk� p�smena

// obsluha �asu
void _fastcall FTimeDateEur(CString& text);		// p�evod data na evropsk� form�t
void _fastcall FTimeDateUsa(CString& text);		// p�evod data na americk� form�t
void _fastcall FTimeDateJap(CString& text);		// p�evod data na japonsk� form�t
void _fastcall FTimeTime24(CString& text);		// p�evod �asu na form�t 24
void _fastcall FTimeTime12(CString& text);		// p�evod �asu na form�t 12

// obsluha text�
void _fastcall FGetFileAct(CString& text);		// na�ten� aktivn�ho adres��e
void _fastcall FGetDiskLabel(CString& text);	// na�ten� jmenovky disku
void _fastcall FGetFileRead(CString& text);		// soubor pro �ten�
void _fastcall FGetFileWrite(CString& text);	// soubor pro z�pis
void _fastcall FGetFileText0(CString& text);	// na�ten� textu zakon�en�ho nulou
void _fastcall FGetFileTextCR(CString& text);	// na�ten� textu zakon�en�ho CR
void _fastcall FGetFileTextN(CString& text);	// na�ten� textu s pevnou d�lkou

void _fastcall FFileListAll(CString& text);		// seznam soubor�
void _fastcall FFileList(CString& text);		// seznam soubor�

// obsluha kl�vesnice
void _fastcall FKeyCharWait(CString& text);		// �ek�n� na snak z kl�vesnice
void _fastcall FKeyChar(CString& text);			// vstup znaku z kl�vesnice

// obsluha dialog�
void _fastcall FDialogText(CString& text);		// dialog box s n�vratem textu
void _fastcall FDialogOpen(CString& text);		// dialog otev�en� souboru
void _fastcall FDialogSave(CString& text);		// dialog ulo�en� souboru
void _fastcall FDialogFind(CString& text);		// dialog vyhled�n� textu
void _fastcall FDialogColor(CString& text);		// dialog v�b�ru barvy

// obsluha text�
void _fastcall FTextLeft(CString& text);		// lev� ��st textu
void _fastcall FTextRight(CString& text);		// prav� ��st textu
void _fastcall FTextMid(CString& text);			// st�edn� ��st textu
void _fastcall FTextlineGet(CString& text);		// ��dek textu
void _fastcall FTextBlock(CString& text);		// blok textu
void _fastcall FTextAdd1(CString& text);		// sou�et jednoho textu - ponech� se beze zm�ny
void _fastcall FTextAdd(CString& text);			// sou�et text� (Data = po�et text� - 1)

// konzola
void _fastcall FConsoleIn(CString& text);		// vstup textu z konzoly

void _fastcall FGetCommandLine(CString& text);	// p��kazov� ��dek
void _fastcall FCommandPar(CString& text);		// parametr p��kazov�ho ��dku
void _fastcall FExecName(CString& text);		// jm�no programu

// dialogy
void _fastcall FGetWindowText(CString& text);	// text okna
void WindowTextValidate();						// aktualizace textu okna

// DirectPlay
void _fastcall FConnectList(CString& text);		// seznam spojen�
void _fastcall FGetConnectAct(CString& text);	// aktivn� spojen�
void _fastcall FGameList(CString& text);		// seznam her
void _fastcall FGetGameAct(CString& text);		// aktivn� hra
void _fastcall FPlayerList(CString& text);		// seznam hr���
void _fastcall FGetPlayerAct(CString& text);	// aktivn� hr��
void _fastcall FGetGameDataT(CString& text);	// na�ten� textu z paketu
void _fastcall FGetGameIP(CString& text);		// zji�t�n� IP po��ta�e

// UDP
void _fastcall FGetUDPSendIP(CString& text);	// zji�t�n� vys�lac� IP adresy UDP
void _fastcall FGetUDPRecIP(CString& text);		// zji�t�n� p�ijat� IP adresy UDP
void _fastcall FGetUDPText(CString& text);		// p��jem textu UDP

void _fastcall FGetUserFont(CString& text);		// zji�t�n� u�ivatelsk�ho fontu
void _fastcall FEnumFonts(CString& text);		// zji�t�n� seznamu font�

void _fastcall FGetAlias(CString& text);		// na�ten� alias souboru

// porty
void _fastcall FGetComText(CString& text);		// na�ten� znaku z portu

// mixer
void _fastcall FGetMixDevice(CString& text);	// zji�t�n� mix�n�ch za��zen�
void _fastcall FGetMixLineDst(CString& text);	// zji�t�n� seznamu c�lov�ch sign�l�
void _fastcall FGetMixLineSrc(CString& text);	// zji�t�n� seznamu zdrojov�ch sign�l�
void _fastcall FGetMixControl(CString& text);	// zji�t�n� seznamu ovl�dac�ch prvk�
void _fastcall FGetMixCtrlList(CString& text);	// zji�t�n� seznamu hodnot ovl�dac�ch prvk�

// DDE
void _fastcall FGetDDEApp(CString& text);		// zji�t�n� seznamu aplikac�
void _fastcall FGetDDETopic(CString& text);		// zji�t�n� seznamu t�mat

// Direct3D
void _fastcall FGet3DMode(CString& text);		// zji�t�n� videom�du Direct3D

extern	TCHAR	Textcrlf[3];

// obsluha DLL
void _fastcall FGetMemoryText0(CString& text);	// text zakon�en� nulou
void _fastcall FGetMemoryText0P(CString& text);	// ukazatel na text zakon�en� nulou
void _fastcall FGetMemoryTextN(CString& text);	// text s pevnou d�lkou

// konfigurace
void _fastcall GetIniProfileString(CString& text); // na�ten� textu z INI (v textu je ulo�ena implicitn� hodnota)
void _fastcall FGetIniFile(CString& text);		// jm�no konfigura�n�ho souboru
void _fastcall FGetIniSection(CString& text);	// jm�no konfigura�n� sekce
void _fastcall FGetIniValue(CString& text);		// jm�no konfigura�n�ho parametru
void _fastcall FGetIniString(CString& text);	// �ten� konfigura�n�ho textu
void _fastcall FGetIniString0(CString& text);	// �ten� konfigura�n�ho textu
void _fastcall FGetRegSubkey(CString& text);	// kl�� registr�
void _fastcall FGetRegValue(CString& text);		// jm�no polo�ky registr�
void _fastcall FGetRegString(CString& text);	// �ten� textu z registru
void _fastcall FGetRegString0(CString& text);	// �ten� textu z registru
