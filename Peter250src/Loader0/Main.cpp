
#pragma warning ( disable: 4201)		// hl�en� - nepojmenovan� struktura
#include <windows.h>					// z�kladn� definice WINDOWS
#pragma warning ( default: 4201)		// hl�en� - nepojmenovan� struktura

#pragma warning ( disable: 4100)		// hl�en� - nepou�it� form�ln� parametr
#pragma warning ( disable: 4514)		// hl�en� - odstran�na inline funkce

#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////
// Data v datov�m bloku jsou nutn�, aby bylo dost sekc� pro vytvo�en� sekce Petra
char DATAsize[] =									// buffer import� od p�eklada�e
{
	"DATAsize"										// sem bude ulo�ena adresa a velikost dat, sou�asn� je to identifik�tor
};

LPCTSTR ProductKey0 = "Software\\Gemtree Software\\Peter";

void WinMainCRTStartup()
{
	DWORD num = 0;
	HKEY hkey;

// zji�t�n� jazyku spu�t�n�ho Petra
	if (::RegOpenKeyEx(
		HKEY_LOCAL_MACHINE,
		ProductKey0,
		0,
		KEY_QUERY_VALUE,
		&hkey) == ERROR_SUCCESS)
	{
		DWORD type = REG_DWORD;
		DWORD bufsize = 4;

		::RegQueryValueEx(
			hkey,
			"Language",
			0,
			&type,
			(BYTE*)&num,
			&bufsize);

		::RegCloseKey(hkey);
	}

	if (num == 0)
	{
		num = (::GetUserDefaultLangID() & 0xff);
	}

	LPCTSTR nadp;
	LPCTSTR text;

	switch (num)
	{
// �esky
	case 0x05:
		nadp = "Nespustiteln� program";
		text = "Tento program byl vytvo�en programovac�m n�strojem Petr\na je nyn� ulo�en v nespustiteln� form�.";
		break;

// n�mecky
	case 0x07:
		nadp = "Speicherplatzmangel";
		text = "Dieses Programm wurde mit dem Programmiermittel Peter hergestellt.\nJetzt ist es in einer nicht startbaren Form gespeichert.";
		break;

// slovensky
	case 0x1b:
		nadp = "Nespustite�n� program";
		text = "Tento program bol vytvoren� programovac�m n�strojom Peter\na je teraz ulo�en� v nespustite�nej forme.";
		break;

// anglicky
	default:
		nadp = "Nonexecutable program";
		text = "This program was created by a programmable tool Peter\nand is now stored in a nonexecutable form.";
		break;
	}

	::MessageBox(NULL, text, nadp, MB_OK | MB_ICONWARNING);
	::ExitProcess(255);
}
