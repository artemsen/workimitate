/**************************************************************************
 *  WorkImitate screensaver (http://workimitate.sourceforge.net)          *
 *  Copyright (C) 2007-2010 by Artem A. Senichev <artemsen@gmail.com>     *
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


/*!
    Implementation of the Settings
*/

#include "Common.h"
#include "Settings.h"

//! Path to registry to save/load settings
#define REG_SETTINGS	L"SOFTWARE\\WorkImitate"
#define REG_PATH		L"Path"
#define REG_INCSF		L"InclSubFolders"
#define REG_EXTFILTER	L"ExtFilter"
#define REG_IDE			L"IDE"
#define REG_SPEED		L"Speed"

Settings* Settings::_Settings = NULL;


Settings* Settings::Get()
{
	if (_Settings == NULL) {
		_Settings = new Settings();
		
		//Set default values
		_Settings->IncludeSubFolders = true;
		_Settings->IDE = enuVS9;
		_Settings->Speed = 50;
		_Settings->ExtFilter = L"h,hpp,hrh,c,cpp";

		//Load settings from registry
		HKEY regKey;
		DWORD result = RegOpenKeyEx(HKEY_CURRENT_USER, REG_SETTINGS, 0, KEY_READ, &regKey);
		if (result == ERROR_SUCCESS) {
			_Settings->Path.resize(1024);
			DWORD dataLen = static_cast<DWORD>(_Settings->Path.size());
			RegQueryValueEx(regKey, REG_PATH, NULL, NULL, reinterpret_cast<LPBYTE>(&_Settings->Path[0]), &dataLen);
			_Settings->Path.resize(dataLen);

			wstring extFilter(1024, 0);
			dataLen = extFilter.size();
			if (RegQueryValueEx(regKey, REG_EXTFILTER, NULL, NULL, reinterpret_cast<LPBYTE>(&extFilter[0]), &dataLen) == ERROR_SUCCESS) {
				extFilter.resize(dataLen);
				if (!extFilter.empty())
					_Settings->ExtFilter = extFilter;
			}

			dataLen = sizeof(DWORD);
			DWORD dwordData = 0;
			RegQueryValueEx(regKey, REG_INCSF, NULL, NULL, reinterpret_cast<LPBYTE>(&dwordData), &dataLen);
			_Settings->IncludeSubFolders = dwordData != 0;

			dataLen = sizeof(DWORD);
			dwordData = 0;
			RegQueryValueEx(regKey, REG_IDE, NULL, NULL, reinterpret_cast<LPBYTE>(&dwordData), &dataLen);
			_Settings->IDE = static_cast<ImitateIDE>(dwordData);

			dataLen = sizeof(DWORD);
			dwordData = 0;
			RegQueryValueEx(regKey, REG_SPEED, NULL, NULL, reinterpret_cast<LPBYTE>(&dwordData), &dataLen);
			_Settings->Speed = dwordData;

			RegCloseKey(regKey);
		}
	}

	return _Settings;
}


bool Settings::Save()
{
	HKEY regKey;
	DWORD result = RegCreateKeyEx(HKEY_CURRENT_USER, REG_SETTINGS, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &regKey, NULL);
	if (result == ERROR_SUCCESS) {
		RegSetValueEx(regKey, REG_PATH, 0, REG_SZ, reinterpret_cast<LPBYTE>(&_Settings->Path[0]), _Settings->Path.length() * sizeof(wchar_t));

		RegSetValueEx(regKey, REG_EXTFILTER, 0, REG_SZ, reinterpret_cast<LPBYTE>(&_Settings->ExtFilter[0]), _Settings->ExtFilter.length() * sizeof(wchar_t));

		DWORD dwordData = _Settings->IncludeSubFolders ? 1 : 0;
		RegSetValueEx(regKey, REG_INCSF, 0, REG_DWORD, reinterpret_cast<LPBYTE>(&dwordData), sizeof(dwordData));

		dwordData = _Settings->IDE;
		RegSetValueEx(regKey, REG_IDE, 0, REG_DWORD, reinterpret_cast<LPBYTE>(&dwordData), sizeof(dwordData));

		dwordData = _Settings->Speed;
		RegSetValueEx(regKey, REG_SPEED, 0, REG_DWORD, reinterpret_cast<LPBYTE>(&dwordData), sizeof(dwordData));

		RegCloseKey(regKey);
		return true;
	}
	return false;
}
