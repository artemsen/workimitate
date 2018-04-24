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

/*! \file Configure.cpp

    Implementation of the CConfigure class
*/

#include "StdAfx.h"
#include "Configure.h"
#include "Settings.h"
#include "Resource.h"


int CConfigure::Run(IN HINSTANCE hInstance, IN HWND hParent)
{
	return DialogBox(hInstance, (LPCTSTR)IDD_CONFIGURE, hParent, (DLGPROC)WndProc);
}


LRESULT CALLBACK CConfigure::WndProc(IN HWND hWnd, IN UINT message, IN WPARAM wParam, IN LPARAM lParam)
{
	switch (message) {
		case WM_INITDIALOG: {
			CSettings objSet;
			SetDlgItemText(hWnd, IDC_EDIT1, objSet.Path);
			SendDlgItemMessage(hWnd, IDC_CHECK1, BM_SETCHECK, objSet.IncludeSubFolders ? BST_CHECKED : BST_UNCHECKED, 0);

			int nAddedId;
			nAddedId = SendDlgItemMessage(hWnd, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"Microsoft Visual Studio 2008");
			SendDlgItemMessage(hWnd, IDC_COMBO1, CB_SETITEMDATA, nAddedId, (LPARAM)CSettings::enuVS9);
			nAddedId = SendDlgItemMessage(hWnd, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"Microsoft Visual C++ 6");
			SendDlgItemMessage(hWnd, IDC_COMBO1, CB_SETITEMDATA, nAddedId, (LPARAM)CSettings::enuVC6);
			nAddedId = SendDlgItemMessage(hWnd, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)"Metrowerks CodeWarrior");
			SendDlgItemMessage(hWnd, IDC_COMBO1, CB_SETITEMDATA, nAddedId, (LPARAM)CSettings::enuCW);
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
				TCHAR szVal[32];
				if (HIWORD(wParam) == 1000)
					strcpy(szVal, _T("1 sec"));
				else
					sprintf(szVal, _T("%i msec"), HIWORD(wParam));
				SetDlgItemText(hWnd, IDC_TYPINGSPEED, szVal);
			}
			return FALSE;
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDOK:
					{
					CSettings objSet(false);
					TCHAR pszPath[MAX_PATH];
					GetDlgItemText(hWnd, IDC_EDIT1, pszPath, MAX_PATH);
					strcpy(objSet.Path, pszPath);
					objSet.IncludeSubFolders = (SendDlgItemMessage(hWnd, IDC_CHECK1, BM_GETCHECK, 0, 0) == BST_CHECKED);
					objSet.EnhColor = (SendDlgItemMessage(hWnd, IDC_CHECK2, BM_GETCHECK, 0, 0) == BST_CHECKED);

					int nCurrItem = SendDlgItemMessage(hWnd, IDC_COMBO1, CB_GETCURSEL, 0, 0);
					objSet.IDE = (CSettings::ImitateIDE)SendDlgItemMessage(hWnd, IDC_COMBO1, CB_GETITEMDATA, (WPARAM)nCurrItem, 0);

					objSet.Speed = SendDlgItemMessage(hWnd, IDC_SLIDER1, TBM_GETPOS, 0, 0);

					objSet.Save();
					EndDialog(hWnd, LOWORD(wParam));
					}
					return TRUE;
				case IDC_COMBO1:
					if (HIWORD(wParam) == CBN_SELENDOK) {
						int nCurrItem = SendDlgItemMessage(hWnd, IDC_COMBO1, CB_GETCURSEL, 0, 0);
						CSettings::ImitateIDE enuIDE = (CSettings::ImitateIDE)SendDlgItemMessage(hWnd, IDC_COMBO1, CB_GETITEMDATA, (WPARAM)nCurrItem, 0);
						EnableWindow(GetDlgItem(hWnd, IDC_CHECK2), enuIDE != CSettings::enuCW);
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
					bi.lpszTitle = "Please, select folder with source files (cpp, h, hpp, ...etc)";
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
