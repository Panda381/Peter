
/***************************************************************************\
*																			*
*							Provádìní programu - text						*
*																			*
\***************************************************************************/


/////////////////////////////////////////////////////////////////////////////
// inline volání pøíkazu - text

inline void FText(CString& text)
{
	ExecItem++;
	ExecItem[-1].ExecTxt(text);
}


/////////////////////////////////////////////////////////////////////////////
// naètení jména souboru

void _fastcall FFileName(CString& text);


void _fastcall FGetStatus(CString& text);		// zjištìní textu stavového øádku
void _fastcall FGetCaption(CString& text);		// zjištìní textu nadpisu okna

// funkce
void _fastcall FTextFunc(CString& text);		// funkce s návratem textu

// promìnné
void _fastcall FTextCRLF(CString& text);		// text CR/LF
void _fastcall FTextEmpty(CString& text);		// prázdný text
void _fastcall FTextObj(CString& text);			// globální text (Data = index)
void _fastcall FTextObjList(CString& text);		// globalní text v seznamu (Data = index, List = seznam)
void _fastcall FTextLoc(CString& text);			// lokální text (Data = index)
void _fastcall FTextLocList(CString& text);		// lokální text v seznamu (Data = index, List = seznam)

// konverze
void _fastcall FTextFormat(CString& text);		// pøevod èísla na text
void _fastcall FTextFormat2(CString& text);		// pøevod èísla na text - 2 èíslice
void _fastcall FTextFormatH(CString& text);		// pøevod èísla na text - 8 èíslic HEX
void _fastcall FTextChar(CString& text);		// pøevod èísla na znak
void _fastcall FTextLower(CString& text);		// pøevod na malá písmena
void _fastcall FTextUpper(CString& text);		// pøevod na velká písmena

// obsluha èasu
void _fastcall FTimeDateEur(CString& text);		// pøevod data na evropský formát
void _fastcall FTimeDateUsa(CString& text);		// pøevod data na americký formát
void _fastcall FTimeDateJap(CString& text);		// pøevod data na japonský formát
void _fastcall FTimeTime24(CString& text);		// pøevod èasu na formát 24
void _fastcall FTimeTime12(CString& text);		// pøevod èasu na formát 12

// obsluha textù
void _fastcall FGetFileAct(CString& text);		// naètení aktivního adresáøe
void _fastcall FGetDiskLabel(CString& text);	// naètení jmenovky disku
void _fastcall FGetFileRead(CString& text);		// soubor pro ètení
void _fastcall FGetFileWrite(CString& text);	// soubor pro zápis
void _fastcall FGetFileText0(CString& text);	// naètení textu zakonèeného nulou
void _fastcall FGetFileTextCR(CString& text);	// naètení textu zakonèeného CR
void _fastcall FGetFileTextN(CString& text);	// naètení textu s pevnou délkou

void _fastcall FFileListAll(CString& text);		// seznam souborù
void _fastcall FFileList(CString& text);		// seznam souborù

// obsluha klávesnice
void _fastcall FKeyCharWait(CString& text);		// èekání na snak z klávesnice
void _fastcall FKeyChar(CString& text);			// vstup znaku z klávesnice

// obsluha dialogù
void _fastcall FDialogText(CString& text);		// dialog box s návratem textu
void _fastcall FDialogOpen(CString& text);		// dialog otevøení souboru
void _fastcall FDialogSave(CString& text);		// dialog uložení souboru
void _fastcall FDialogFind(CString& text);		// dialog vyhledání textu
void _fastcall FDialogColor(CString& text);		// dialog výbìru barvy

// obsluha textù
void _fastcall FTextLeft(CString& text);		// levá èást textu
void _fastcall FTextRight(CString& text);		// pravá èást textu
void _fastcall FTextMid(CString& text);			// støední èást textu
void _fastcall FTextlineGet(CString& text);		// øádek textu
void _fastcall FTextBlock(CString& text);		// blok textu
void _fastcall FTextAdd1(CString& text);		// souèet jednoho textu - ponechá se beze zmìny
void _fastcall FTextAdd(CString& text);			// souèet textù (Data = poèet textù - 1)

// konzola
void _fastcall FConsoleIn(CString& text);		// vstup textu z konzoly

void _fastcall FGetCommandLine(CString& text);	// pøíkazový øádek
void _fastcall FCommandPar(CString& text);		// parametr pøíkazového øádku
void _fastcall FExecName(CString& text);		// jméno programu

// dialogy
void _fastcall FGetWindowText(CString& text);	// text okna
void WindowTextValidate();						// aktualizace textu okna

// DirectPlay
void _fastcall FConnectList(CString& text);		// seznam spojení
void _fastcall FGetConnectAct(CString& text);	// aktivní spojení
void _fastcall FGameList(CString& text);		// seznam her
void _fastcall FGetGameAct(CString& text);		// aktivní hra
void _fastcall FPlayerList(CString& text);		// seznam hráèù
void _fastcall FGetPlayerAct(CString& text);	// aktivní hráè
void _fastcall FGetGameDataT(CString& text);	// naètení textu z paketu
void _fastcall FGetGameIP(CString& text);		// zjištìní IP poèítaèe

// UDP
void _fastcall FGetUDPSendIP(CString& text);	// zjištìní vysílací IP adresy UDP
void _fastcall FGetUDPRecIP(CString& text);		// zjištìní pøijaté IP adresy UDP
void _fastcall FGetUDPText(CString& text);		// pøíjem textu UDP

void _fastcall FGetUserFont(CString& text);		// zjištìní uživatelského fontu
void _fastcall FEnumFonts(CString& text);		// zjištìní seznamu fontù

void _fastcall FGetAlias(CString& text);		// naètení alias souboru

// porty
void _fastcall FGetComText(CString& text);		// naètení znaku z portu

// mixer
void _fastcall FGetMixDevice(CString& text);	// zjištìní mixážních zaøízení
void _fastcall FGetMixLineDst(CString& text);	// zjištìní seznamu cílových signálù
void _fastcall FGetMixLineSrc(CString& text);	// zjištìní seznamu zdrojových signálù
void _fastcall FGetMixControl(CString& text);	// zjištìní seznamu ovládacích prvkù
void _fastcall FGetMixCtrlList(CString& text);	// zjištìní seznamu hodnot ovládacích prvkù

// DDE
void _fastcall FGetDDEApp(CString& text);		// zjištìní seznamu aplikací
void _fastcall FGetDDETopic(CString& text);		// zjištìní seznamu témat

// Direct3D
void _fastcall FGet3DMode(CString& text);		// zjištìní videomódu Direct3D

extern	TCHAR	Textcrlf[3];

// obsluha DLL
void _fastcall FGetMemoryText0(CString& text);	// text zakonèený nulou
void _fastcall FGetMemoryText0P(CString& text);	// ukazatel na text zakonèený nulou
void _fastcall FGetMemoryTextN(CString& text);	// text s pevnou délkou

// konfigurace
void _fastcall GetIniProfileString(CString& text); // naètení textu z INI (v textu je uložena implicitní hodnota)
void _fastcall FGetIniFile(CString& text);		// jméno konfiguraèního souboru
void _fastcall FGetIniSection(CString& text);	// jméno konfiguraèní sekce
void _fastcall FGetIniValue(CString& text);		// jméno konfiguraèního parametru
void _fastcall FGetIniString(CString& text);	// ètení konfiguraèního textu
void _fastcall FGetIniString0(CString& text);	// ètení konfiguraèního textu
void _fastcall FGetRegSubkey(CString& text);	// klíè registrù
void _fastcall FGetRegValue(CString& text);		// jméno položky registrù
void _fastcall FGetRegString(CString& text);	// ètení textu z registru
void _fastcall FGetRegString0(CString& text);	// ètení textu z registru
