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
    Implementation of the CConfigure class
*/

#include "Configure.h"
#include "Settings.h"
#include "Resource.h"


int CConfigure::Run(const HINSTANCE hinst, const HWND wndParent)
{
	return static_cast<int>(DialogBox(hinst, (LPCTSTR)IDD_CONFIGURE, wndParent, reinterpret_cast<DLGPROC>(&CConfigure::WndProc)));
}


LRESULT CALLBACK CConfigure::WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
		case WM_INITDIALOG: {
			Settings* settings = Settings::Get();
			SetDlgItemText(wnd, IDC_PATH_EDIT, settings->Path.c_str());
			SetDlgItemText(wnd, IDC_EXTFILTER_EDIT, settings->ExtFilter.c_str());
			SendDlgItemMessage(wnd, IDC_SUBDIR_CHECK, BM_SETCHECK, settings->IncludeSubFolders ? BST_CHECKED : BST_UNCHECKED, 0);

			LRESULT addedId;
			addedId = SendDlgItemMessage(wnd, IDC_IDETYPE_COMBO, CB_ADDSTRING, 0, (LPARAM)L"Microsoft Visual Studio 2008");
			SendDlgItemMessage(wnd, IDC_IDETYPE_COMBO, CB_SETITEMDATA, addedId, (LPARAM)Settings::enuVS9);
			addedId = SendDlgItemMessage(wnd, IDC_IDETYPE_COMBO, CB_ADDSTRING, 0, (LPARAM)L"Microsoft Visual C++ 6");
			SendDlgItemMessage(wnd, IDC_IDETYPE_COMBO, CB_SETITEMDATA, addedId, (LPARAM)Settings::enuVC6);
			addedId = SendDlgItemMessage(wnd, IDC_IDETYPE_COMBO, CB_ADDSTRING, 0, (LPARAM)L"Metrowerks CodeWarrior");
			SendDlgItemMessage(wnd, IDC_IDETYPE_COMBO, CB_SETITEMDATA, addedId, (LPARAM)Settings::enuCW);
			addedId = SendDlgItemMessage(wnd, IDC_IDETYPE_COMBO, CB_ADDSTRING, 0, (LPARAM)L"Nokia Carbide.c++");
			SendDlgItemMessage(wnd, IDC_IDETYPE_COMBO, CB_SETITEMDATA, addedId, (LPARAM)Settings::enuCarbide);

			SendDlgItemMessage(wnd, IDC_IDETYPE_COMBO, CB_SETCURSEL, (WPARAM)settings->IDE, 0);
			SendMessage(wnd, WM_COMMAND, MAKELONG(IDC_IDETYPE_COMBO, CBN_SELENDOK), 0);

			SendDlgItemMessage(wnd, IDC_TYPINGSPEED_SLIDER, TBM_SETRANGE, (WPARAM)TRUE, MAKELPARAM(10, 200));
			SendDlgItemMessage(wnd, IDC_TYPINGSPEED_SLIDER, TBM_SETPOS, (WPARAM)TRUE, settings->Speed);
			WndProc(wnd, WM_HSCROLL, MAKELONG(SB_THUMBTRACK, settings->Speed), 0);

			static HICON icon = LoadIcon(GetModuleHandle(NULL), (LPCTSTR)IDI_WORKIMITATE);
			SendMessage(wnd, WM_SETICON, ICON_BIG, (LPARAM)icon);
			}
			return TRUE;
		case WM_HSCROLL:
			if (LOWORD(wParam) == SB_THUMBTRACK) {
				wchar_t szVal[32];
				swprintf(szVal, L"%i msec", HIWORD(wParam));
				SetDlgItemText(wnd, IDC_TYPINGSPEED_STATIC, szVal);
			}
			return FALSE;
		case WM_NOTIFY:
			switch (((LPNMHDR)lParam)->code) {
				case NM_CLICK:
				case NM_RETURN:
				{
					const PNMLINK nmLink = reinterpret_cast<const PNMLINK>(lParam);
					if (nmLink->hdr.idFrom == IDC_WEBSITE_LINK)
						ShellExecute(NULL, L"open", L"http://workimitate.sourceforge.net", NULL, NULL, SW_SHOW);
					else if (nmLink->hdr.idFrom == IDC_EMAIL_LINK)
						ShellExecute(NULL, L"open", L"mailto:artemsen@gmail.com", NULL, NULL, SW_SHOW);
				}
				break;
				default:
					break;
			}
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDOK:
					{
						Settings* settings = Settings::Get();

						settings->Path.resize(SendDlgItemMessage(wnd, IDC_PATH_EDIT, WM_GETTEXTLENGTH, 0, 0) + 1);
						GetDlgItemText(wnd, IDC_PATH_EDIT, &settings->Path[0], static_cast<int>(settings->Path.size()));

						settings->ExtFilter.resize(SendDlgItemMessage(wnd, IDC_EXTFILTER_EDIT, WM_GETTEXTLENGTH, 0, 0) + 1);
						GetDlgItemText(wnd, IDC_EXTFILTER_EDIT, &settings->ExtFilter[0], static_cast<int>(settings->ExtFilter.size()));

						settings->IncludeSubFolders = (SendDlgItemMessage(wnd, IDC_SUBDIR_CHECK, BM_GETCHECK, 0, 0) == BST_CHECKED);

						LRESULT currItem = SendDlgItemMessage(wnd, IDC_IDETYPE_COMBO, CB_GETCURSEL, 0, 0);
						settings->IDE = static_cast<Settings::ImitateIDE>(SendDlgItemMessage(wnd, IDC_IDETYPE_COMBO, CB_GETITEMDATA, (WPARAM)currItem, 0));

						settings->Speed = static_cast<UINT>(SendDlgItemMessage(wnd, IDC_TYPINGSPEED_SLIDER, TBM_GETPOS, 0, 0));

						Settings::Save();
						EndDialog(wnd, LOWORD(wParam));
					}
					return TRUE;
				case IDCANCEL:
					EndDialog(wnd, LOWORD(wParam));
					return TRUE;
				case IDC_BROWSE_BUTTON:
					{
						TCHAR cn[MAX_PATH];
						BROWSEINFO bi;
						ZeroMemory (&bi, sizeof(bi));
						bi.hwndOwner = wnd;
						bi.ulFlags = BIF_RETURNONLYFSDIRS;
						bi.lpszTitle = L"Please, select folder with source files (cpp, h, hpp, ...etc)";
						bi.lpfn = &CConfigure::BFFCallBack;
						bi.lParam = (LPARAM)wnd;
						LPITEMIDLIST pList = SHBrowseForFolder(&bi);
						if (SHGetPathFromIDList(pList, cn))
							SetDlgItemText(wnd, IDC_PATH_EDIT, cn);
					}
					return TRUE;
				default:
					break;
			}
			break;
		default:
			break;
	}
	return 0;
}


int CALLBACK CConfigure::BFFCallBack(HWND wnd, UINT msg, LPARAM /*lParam*/, LPARAM lpData)
{
	if (msg == BFFM_INITIALIZED) {
		//Set initial path
		wchar_t pszPath[MAX_PATH];
		GetDlgItemText((HWND)lpData, IDC_PATH_EDIT, pszPath, MAX_PATH / sizeof(wchar_t));
		SendMessage(wnd, BFFM_SETSELECTION, TRUE, (LPARAM)pszPath);
	}
	return 0;
}
