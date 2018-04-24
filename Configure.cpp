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

/*! \file Configure.cpp

    Implementation of the CConfigure class
*/

#include "StdAfx.h"
#include "Configure.h"
#include "Settings.h"
#include "Resource.h"


int CConfigure::Run(IN HINSTANCE hInstance, IN HWND hParent)
{
	return static_cast<int>(DialogBox(hInstance, (LPCTSTR)IDD_CONFIGURE, hParent, (DLGPROC)WndProc));
}


LRESULT CALLBACK CConfigure::WndProc(IN HWND hWnd, IN UINT uMsg, IN WPARAM wParam, IN LPARAM /*lParam*/)
{
	switch (uMsg) {
		case WM_INITDIALOG: {
			CSettings objSet;
			SetDlgItemText(hWnd, IDC_EDIT1, objSet.Path);
			SendDlgItemMessage(hWnd, IDC_CHECK1, BM_SETCHECK, objSet.IncludeSubFolders ? BST_CHECKED : BST_UNCHECKED, 0);

			LRESULT nAddedId;
			nAddedId = SendDlgItemMessage(hWnd, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"Microsoft Visual Studio 2008");
			SendDlgItemMessage(hWnd, IDC_COMBO1, CB_SETITEMDATA, nAddedId, (LPARAM)CSettings::enuVS9);
			nAddedId = SendDlgItemMessage(hWnd, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"Microsoft Visual C++ 6");
			SendDlgItemMessage(hWnd, IDC_COMBO1, CB_SETITEMDATA, nAddedId, (LPARAM)CSettings::enuVC6);
			nAddedId = SendDlgItemMessage(hWnd, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"Metrowerks CodeWarrior");
			SendDlgItemMessage(hWnd, IDC_COMBO1, CB_SETITEMDATA, nAddedId, (LPARAM)CSettings::enuCW);
			nAddedId = SendDlgItemMessage(hWnd, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"Nokia Carbide.c++");
			SendDlgItemMessage(hWnd, IDC_COMBO1, CB_SETITEMDATA, nAddedId, (LPARAM)CSettings::enuCarbide);

			SendDlgItemMessage(hWnd, IDC_COMBO1, CB_SETCURSEL, (WPARAM)objSet.IDE, 0);
			SendDlgItemMessage(hWnd, IDC_CHECK2, BM_SETCHECK, objSet.EnhColor ? BST_CHECKED : BST_UNCHECKED, 0);
			SendMessage(hWnd, WM_COMMAND, MAKELONG(IDC_COMBO1, CBN_SELENDOK), 0);

			SendDlgItemMessage(hWnd, IDC_SLIDER1, TBM_SETRANGE, (WPARAM)TRUE, MAKELPARAM(10, 1000));
			SendDlgItemMessage(hWnd, IDC_SLIDER1, TBM_SETTICFREQ, 50, 0);
			SendDlgItemMessage(hWnd, IDC_SLIDER1, TBM_SETPOS, (WPARAM)TRUE, objSet.Speed);
			WndProc(hWnd, WM_HSCROLL, MAKELONG(SB_THUMBTRACK, objSet.Speed), 0);
			}
			return TRUE;
		case WM_HSCROLL:
			if (LOWORD(wParam) == SB_THUMBTRACK) {
				wchar_t szVal[32];
				if (HIWORD(wParam) == 1000)
					wcscpy_s(szVal, L"1 sec");
				else
					swprintf(szVal, sizeof(szVal) / sizeof(wchar_t), L"%i msec", HIWORD(wParam));
				SetDlgItemText(hWnd, IDC_TYPINGSPEED, szVal);
			}
			return FALSE;
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDOK:
					{
					CSettings objSet(false);
					wchar_t pszPath[MAX_PATH];
					GetDlgItemText(hWnd, IDC_EDIT1, pszPath, MAX_PATH / sizeof(wchar_t));
					wcscpy_s(objSet.Path, sizeof(objSet.Path) / sizeof(wchar_t), pszPath);
					objSet.IncludeSubFolders = (SendDlgItemMessage(hWnd, IDC_CHECK1, BM_GETCHECK, 0, 0) == BST_CHECKED);
					objSet.EnhColor = (SendDlgItemMessage(hWnd, IDC_CHECK2, BM_GETCHECK, 0, 0) == BST_CHECKED);

					LRESULT nCurrItem = SendDlgItemMessage(hWnd, IDC_COMBO1, CB_GETCURSEL, 0, 0);
					objSet.IDE = (CSettings::ImitateIDE)SendDlgItemMessage(hWnd, IDC_COMBO1, CB_GETITEMDATA, (WPARAM)nCurrItem, 0);

					objSet.Speed = static_cast<UINT>(SendDlgItemMessage(hWnd, IDC_SLIDER1, TBM_GETPOS, 0, 0));

					objSet.Save();
					EndDialog(hWnd, LOWORD(wParam));
					}
					return TRUE;
				case IDCANCEL:
					EndDialog(hWnd, LOWORD(wParam));
					return TRUE;
				case IDC_BUTTON1:
					{
					TCHAR cn[MAX_PATH];
					BROWSEINFO bi;
					ZeroMemory (&bi, sizeof(bi));
					bi.hwndOwner = hWnd;
					bi.ulFlags = BIF_RETURNONLYFSDIRS;
					bi.lpszTitle = L"Please, select folder with source files (cpp, h, hpp, ...etc)";
					bi.lpfn = &CConfigure::BFFCallBack;
					bi.lParam = (LPARAM)hWnd;
					LPITEMIDLIST pList = SHBrowseForFolder(&bi);
					if (SHGetPathFromIDList(pList, cn))
						SetDlgItemText(hWnd, IDC_EDIT1, cn);
					}
					return TRUE;
			}
			break;
	}
	return 0;
}

int CALLBACK CConfigure::BFFCallBack(IN HWND hWnd, IN UINT uMsg, IN LPARAM /*lParam*/, IN LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED) {
		//Set initial path
		wchar_t pszPath[MAX_PATH];
		GetDlgItemText((HWND)lpData, IDC_EDIT1, pszPath, MAX_PATH / sizeof(wchar_t));
		SendMessage(hWnd, BFFM_SETSELECTION, TRUE, (LPARAM)pszPath);
	}
	return 0;
}
