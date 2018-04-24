/**************************************************************************
 *  WorkImitate screensaver (http://workimitate.sourceforge.net)          *
 *  Copyright (C) 2008 by Artem A. Senichev <artemsen@gmail.com>          *
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
    Defines the entry point for the application
*/

#include "Common.h"
#include "Configure.h"
#include "Preview.h"
#include "ScreenSaver.h"


/**
 * Programm entry point (see MSDN for more info)
 */
int APIENTRY WinMain(IN HINSTANCE hInstance, IN HINSTANCE /*hPrevInstance*/, IN LPSTR lpCmdLine, IN int /*nCmdShow*/)
{
	int retCode = -1;

	char mode = 0;	//By default - install query
	HWND wndParent = NULL;

	//Parse command line
	if (lstrlenA(lpCmdLine) != 0) {
		while (*lpCmdLine && !isalpha(*lpCmdLine))
			++lpCmdLine;

		if (*lpCmdLine) {
			mode = *lpCmdLine++;
			
			//Get window handle
			while (*lpCmdLine && !isdigit(*lpCmdLine))
				++lpCmdLine;

			if (*lpCmdLine)
				wndParent = reinterpret_cast<HWND>(atoi(lpCmdLine));
		}
	}

	switch (mode) {
		case 'c':
		case 'C':
			//Configure mode
			retCode = CConfigure::Run(hInstance, wndParent);
			break;
		case 'p':
		case 'P':
			//Preview mode
			retCode = CPreview::Run(hInstance, wndParent);
			break;
		case 's':
		case 'S':
			//Screen saver mode
			retCode = CScreenSaver::Run(hInstance, wndParent);
			break;
		case'i':
		case'I':
			//Install mode
			{
				wstring moduleName(MAX_PATH, 0);
				moduleName.resize(GetModuleFileName(NULL, &moduleName[0], moduleName.size() - 1));
				wstring scrsaverName(MAX_PATH, 0);
				scrsaverName.resize(GetSystemDirectory(&scrsaverName[0], scrsaverName.size() - 1));
				scrsaverName += L"\\WorkImitate.scr";

				typedef BOOL (WINAPI *LPFN_Wow64DisableWow64FsRedirection)(PVOID*);
				LPFN_Wow64DisableWow64FsRedirection fnWow64DisableWow64FsRedirection =
					(LPFN_Wow64DisableWow64FsRedirection)GetProcAddress(GetModuleHandle(L"kernel32"), "Wow64DisableWow64FsRedirection");
				if (fnWow64DisableWow64FsRedirection != NULL) {
					PVOID oldValue;
					fnWow64DisableWow64FsRedirection(&oldValue);
				}

				CopyFile(moduleName.c_str(), scrsaverName.c_str(), FALSE);
			}
			break;
		default:
			//Install query mode
			if (MessageBox(NULL, L"Do you want to install Work Imitate screen saver ?", L"Work Imitate screen saver", MB_YESNO | MB_ICONQUESTION) == IDYES) {
				wstring moduleName(MAX_PATH, 0);
				moduleName.resize(GetModuleFileName(NULL, &moduleName[0], moduleName.size() - 1));
				ShellExecute(NULL, L"runas", moduleName.c_str(), L"/i", NULL, SW_SHOWNORMAL);

				wstring scrsaverName(MAX_PATH, 0);
				scrsaverName.resize(GetSystemDirectory(&scrsaverName[0], scrsaverName.size() - 1));
				scrsaverName += L"\\WorkImitate.scr";

				typedef BOOL (WINAPI *LPFN_Wow64DisableWow64FsRedirection)(PVOID*);
				LPFN_Wow64DisableWow64FsRedirection fnWow64DisableWow64FsRedirection =
					(LPFN_Wow64DisableWow64FsRedirection)GetProcAddress(GetModuleHandle(L"kernel32"), "Wow64DisableWow64FsRedirection");
				if (fnWow64DisableWow64FsRedirection != NULL) {
					PVOID oldValue;
					fnWow64DisableWow64FsRedirection(&oldValue);
				}

				wstring cmd = L"desk.cpl,InstallScreenSaver ";
				cmd += scrsaverName;
				ShellExecute(NULL, L"open", L"rundll32.exe", cmd.c_str(), NULL, SW_SHOWNORMAL);
			}
			break;
	}

	return retCode;
}
