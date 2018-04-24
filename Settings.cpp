/***************************************************************************
 *   Copyright (C) 2007-2008 by Artem A. Senichev                          *
 *   artemsen@gmail.com                                                    *
 *                                                                         *
 *   This file is part of the WorkImitate screen saver                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


/*! \file Settings.cpp

    Implementation of the CSettings class
*/

#include "StdAfx.h"
#include "Settings.h"

//! Path to registry to save/load settings
#define REG_SETTINGS	_T("SOFTWARE\\WorkImitate")
#define REG_PATH		_T("Path")
#define REG_INCSF		_T("InclSubFolders")
#define REG_ECS			_T("EnhColorSyntax")
#define REG_IDE			_T("IDE")
#define REG_SPEED		_T("Speed")


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
	else {
		//Set default values
		strcpy(Path, "");
		IncludeSubFolders = true;
		EnhColor = true;
		IDE = enuVS9;
		Speed = 50;
		//Not implemented: Val.Exts
	}

	/* Not implemented - always constant set */
	Exts.insert("h");
	Exts.insert("hpp");
	Exts.insert("hrh");
	Exts.insert("c");
	Exts.insert("cpp");

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
