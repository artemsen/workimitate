/**************************************************************************
 *  WorkImitate screensaver (http://workimitate.sourceforge.net)          *
 *  Copyright (C) 2007-2008 by Artem A. Senichev <artemsen@gmail.com>     *
 *                                                                        *
 *  This program is free software: you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 3 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 **************************************************************************/


/*! \file Settings.cpp

    Implementation of the CSettings class
*/

#include "StdAfx.h"
#include "Settings.h"

//! Path to registry to save/load settings
#define REG_SETTINGS	L"SOFTWARE\\WorkImitate"
#define REG_PATH		L"Path"
#define REG_INCSF		L"InclSubFolders"
#define REG_ECS			L"EnhColorSyntax"
#define REG_IDE			L"IDE"
#define REG_SPEED		L"Speed"


CSettings::CSettings(IN bool fLoadSettings /*= true*/)
{
	if (fLoadSettings)
		Load();
}


CSettings::~CSettings(void)
{
}


bool CSettings::Load(void)
{
	//Set default values
	wcscpy_s(Path, sizeof(Path) / sizeof(wchar_t), L"");
	IncludeSubFolders = true;
	EnhColor = true;
	IDE = enuVS9;
	Speed = 50;

	/* Not implemented - always constant set */
	Exts.insert(L"h");
	Exts.insert(L"hpp");
	Exts.insert(L"hrh");
	Exts.insert(L"c");
	Exts.insert(L"cpp");

	HKEY hKey;
	DWORD dwResult = RegOpenKeyEx(HKEY_CURRENT_USER, REG_SETTINGS, 0, KEY_READ, &hKey);
	if (dwResult == ERROR_SUCCESS) {

		DWORD dwDataLen = sizeof(Path);
		RegQueryValueEx(hKey, REG_PATH, NULL, NULL, (LPBYTE)&Path, &dwDataLen);

		dwDataLen = sizeof(DWORD);
		DWORD dwData = 0;
		RegQueryValueEx(hKey, REG_INCSF, NULL, NULL, (LPBYTE)&dwData, &dwDataLen);
		IncludeSubFolders = dwData != 0;

		dwDataLen = sizeof(DWORD);
		dwData = 0;
		RegQueryValueEx(hKey, REG_ECS, NULL, NULL, (LPBYTE)&dwData, &dwDataLen);
		EnhColor = dwData != 0;

		dwDataLen = sizeof(DWORD);
		dwData = 0;
		RegQueryValueEx(hKey, REG_IDE, NULL, NULL, (LPBYTE)&dwData, &dwDataLen);
		IDE = (ImitateIDE)dwData;

		dwDataLen = sizeof(DWORD);
		dwData = 0;
		RegQueryValueEx(hKey, REG_SPEED, NULL, NULL, (LPBYTE)&dwData, &dwDataLen);
		Speed = dwData;

		RegCloseKey(hKey);
	}

	return true;	//If error - use default settings
}


bool CSettings::Save(void)
{
	HKEY hKey;
	DWORD dwResult = RegCreateKeyEx(HKEY_CURRENT_USER, REG_SETTINGS, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
	if (dwResult == ERROR_SUCCESS) {
		RegSetValueEx(hKey, REG_PATH, 0, REG_SZ, (LPBYTE)Path, (DWORD)_tcslen(Path) * sizeof(TCHAR));

		DWORD dwData = IncludeSubFolders ? 1 : 0;
		RegSetValueEx(hKey, REG_INCSF, 0, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData));

		dwData = EnhColor ? 1 : 0;
		RegSetValueEx(hKey, REG_ECS, 0, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData));

		dwData = IDE;
		RegSetValueEx(hKey, REG_IDE, 0, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData));

		dwData = Speed;
		RegSetValueEx(hKey, REG_SPEED, 0, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData));

		RegCloseKey(hKey);
		return true;
	}
	return false;
}
