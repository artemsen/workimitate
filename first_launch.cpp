/**************************************************************************
 *  WorkImitate screen saver (http://workimitate.sourceforge.net)         *
 *  Copyright (C) 2008-2012 by Artem Senichev <artemsen@gmail.com>        *
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

#include "first_launch.h"
#include "screen_saver.h"
#include "resource.h"


int first_launch::run()
{
	return static_cast<int>(DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_FIRSTLAUNCH), GetDesktopWindow(), &first_launch::dlg_proc));
}


int first_launch::install()
{
	wchar_t curr_module_path[MAX_PATH];
	if (!GetModuleFileName(NULL, curr_module_path, MAX_PATH - 1))
		return -1;
	wchar_t ss_module_path[MAX_PATH];
	if (!GetSystemDirectory(ss_module_path, MAX_PATH - 1))
		return -1;
	lstrcat(ss_module_path, L"\\WorkImitate.scr");

	disable_wow64_redirection();
	
	return CopyFile(curr_module_path, ss_module_path, FALSE) ? 0 : -1;
}


INT_PTR CALLBACK first_launch::dlg_proc(HWND wnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
	if (msg == WM_INITDIALOG) {
		static HBITMAP bmp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_PREVIEW));
		SendDlgItemMessage(wnd, IDC_PREVIEW, STM_SETIMAGE, IMAGE_BITMAP, reinterpret_cast<LPARAM>(bmp));
		static HICON icon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MAIN));
		SendMessage(wnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(icon));
		return TRUE;
	}
	else if (msg == WM_COMMAND) {
		switch (LOWORD(w_param)) {
			case IDC_BTN_START:
				screen_saver().run(wnd);
				return TRUE;
			case IDC_BTN_INSTALL:
				start_install();
				EndDialog(wnd, 0);
				return TRUE;
			case IDCANCEL:
				EndDialog(wnd, 1);
				return TRUE;
		}
	}

	return FALSE;
}


void first_launch::disable_wow64_redirection()
{
	typedef BOOL (WINAPI *LPFN_Wow64DisableWow64FsRedirection)(PVOID*);
	LPFN_Wow64DisableWow64FsRedirection fnWow64DisableWow64FsRedirection =
		(LPFN_Wow64DisableWow64FsRedirection)GetProcAddress(GetModuleHandle(L"kernel32"), "Wow64DisableWow64FsRedirection");
	if (fnWow64DisableWow64FsRedirection != NULL) {
		PVOID old_value;
		fnWow64DisableWow64FsRedirection(&old_value);
	}
}


void first_launch::start_install()
{
	wchar_t curr_module_path[MAX_PATH];
	if (!GetModuleFileName(NULL, curr_module_path, MAX_PATH - 1))
		return;
	HINSTANCE hist = ShellExecute(NULL, L"runas", curr_module_path, L"/i", NULL, SW_SHOWNORMAL);
	if (reinterpret_cast<int>(hist) <= 32)
		return;

	wchar_t ss_module_path[MAX_PATH];
	if (!GetSystemDirectory(ss_module_path, MAX_PATH - 1))
		return;
	lstrcat(ss_module_path, L"\\WorkImitate.scr");

	disable_wow64_redirection();

	wchar_t cfg_cmd[MAX_PATH + 32];
	lstrcpy(cfg_cmd, L"desk.cpl,InstallScreenSaver ");
	lstrcat(cfg_cmd, ss_module_path);
	ShellExecute(NULL, L"open", L"rundll32.exe", cfg_cmd, NULL, SW_SHOWNORMAL);
}
