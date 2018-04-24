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

/*! \file ScreenSaver.cpp

    Implementation of the CScreenSaver class
*/

#include "StdAfx.h"
#include "ScreenSaver.h"
#include "Resource.h"
#include "Settings.h"


list<string>	CScreenSaver::m_listFiles;
HWND			CScreenSaver::m_hRichEdit = NULL;
CIDEImitate*	CScreenSaver::m_pIDEImitate = NULL;
map<CIDEImitate::ImagePlace, int> CScreenSaver::m_mapCoordinates;

#define TIMER_ID	0x01


int CScreenSaver::Run(IN HINSTANCE hInstance, IN HWND hParent)
{
	//Define imitated IDE
	m_pIDEImitate = NULL;
	CSettings objSet;
	switch (objSet.IDE) {
		case CSettings::enuVS9: m_pIDEImitate = new CIDEVS9();
			break;
		case CSettings::enuVC6: m_pIDEImitate = new CIDEVC6();
			break;
		case CSettings::enuCW: m_pIDEImitate = new CIDECW();
			break;
	}
	if (!m_pIDEImitate) {
		MessageBox(NULL, _T("Imitated IDE undefined"), NULL, MB_ICONERROR | MB_OK);
		return 1;
	}

	//Load images
	if (!m_pIDEImitate->LoadImages(hInstance)) {
		MessageBox(NULL, _T("Unable to load images"), NULL, MB_ICONERROR | MB_OK);
		return 1;
	}


	//Register window class and create main window
	const char* pszWindowClassName = "WorkImitateWndClass";
	WNDCLASSEX wcex;
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)m_pIDEImitate->GetIconId());
	wcex.hCursor		= NULL;
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= pszWindowClassName;
	wcex.hIconSm		= LoadIcon(hInstance, (LPCTSTR)m_pIDEImitate->GetIconId());
	if (!RegisterClassEx(&wcex)) {
		MessageBox(NULL, _T("Unable to register window class"), NULL, MB_ICONERROR | MB_OK);
		return 1;
	}
	HWND hWnd = CreateWindow(pszWindowClassName, NULL, WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, hInstance, NULL);
	if (!hWnd) {
		MessageBox(NULL, _T("Unable to create window"), NULL, MB_ICONERROR | MB_OK);
		return 1;
	}

	//Initialize richedit
	HINSTANCE hRELib = LoadLibrary("riched32.dll");
	if (!hRELib) {
		MessageBox(NULL, _T("Unable to load RichEdit library"), NULL, MB_ICONERROR | MB_OK);
		return 1;
	}

	m_hRichEdit = CreateWindow(RICHEDIT_CLASS, NULL, WS_CHILD | WS_BORDER | WS_VSCROLL | WS_HSCROLL | WS_VISIBLE | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, 0, 0, 0, 0, hWnd, NULL, hInstance, NULL);
	CHARFORMAT chfOut;
	ZeroMemory(&chfOut, sizeof(chfOut));
	chfOut.cbSize = sizeof(CHARFORMAT);
	chfOut.dwMask = CFM_BOLD | CFM_COLOR | CFM_SIZE | CFM_FACE | CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT;
	strcpy(chfOut.szFaceName, /*sizeof(chfOut.szFaceName) / sizeof(TCHAR),*/ _T("Courier New"));
	chfOut.yHeight = 10 * 20;
	chfOut.dwEffects = 0;
	chfOut.crTextColor = RGB(0, 0, 0);
	SendMessage(m_hRichEdit, EM_SETCHARFORMAT, 0, (LPARAM)&chfOut);
	SendMessage(m_hRichEdit, EM_SETEVENTMASK, 0, ENM_KEYEVENTS);
	SetFocus(m_hRichEdit);

	//Show main window
	ShowWindow(hWnd, SW_SHOWMAXIMIZED);

	//Getimages heights
	HDC hDC = GetWindowDC(hWnd);
	int nTopHeight = m_pIDEImitate->GetImage(CIDEImitate::IPTop)->GetHeight(hDC);
	int nBottomHeight = m_pIDEImitate->GetImage(CIDEImitate::IPBottom)->GetHeight(hDC);
	int nLeftWidth = m_pIDEImitate->GetImage(CIDEImitate::IPLeft)->GetWidth(hDC);
	ReleaseDC(hWnd, hDC);
	
	//Set windows position and size
	RECT rcWnd;
	GetClientRect(hWnd, &rcWnd);
	m_mapCoordinates[CIDEImitate::IPTop] = 0;
	m_mapCoordinates[CIDEImitate::IPBottom] = rcWnd.bottom - nBottomHeight;
	m_mapCoordinates[CIDEImitate::IPLeft] = nTopHeight;
	SetWindowPos(m_hRichEdit, NULL, nLeftWidth, nTopHeight, rcWnd.right - nLeftWidth, rcWnd.bottom - nBottomHeight - nTopHeight, SWP_SHOWWINDOW | SWP_NOZORDER);

	//Randomize timer
	srand(GetTickCount());

	//Hide cursor and update main window
	ShowCursor(FALSE);
	UpdateWindow(hWnd);

	//Getting file list
	if (strlen(objSet.Path))
		GetFileList(objSet.Path, objSet.IncludeSubFolders, objSet.Exts, m_listFiles);

	//Register timer
	SetTimer(hWnd, TIMER_ID, objSet.Speed, OnTimer);

	//Main message loop
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, NULL, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}


LRESULT CALLBACK CScreenSaver::WndProc(IN HWND hWnd, IN UINT message, IN WPARAM wParam, IN LPARAM lParam)
{
	switch (message) {
		case WM_NOTIFY:
			if (LPNMHDR(lParam)->hwndFrom == m_hRichEdit && LPNMHDR(lParam)->code == EN_MSGFILTER)
				PostQuitMessage(0);
			else
				return DefWindowProc(hWnd, message, wParam, lParam);
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hDC = BeginPaint(hWnd, &ps);
				m_pIDEImitate->GetImage(CIDEImitate::IPTop)->Draw(hDC, 0, m_mapCoordinates[CIDEImitate::IPTop]);
				m_pIDEImitate->GetImage(CIDEImitate::IPLeft)->Draw(hDC, 0, m_mapCoordinates[CIDEImitate::IPLeft]);
				m_pIDEImitate->GetImage(CIDEImitate::IPBottom)->Draw(hDC, 0, m_mapCoordinates[CIDEImitate::IPBottom]);
				EndPaint(hWnd, &ps);
			}
			break;
		case WM_DESTROY:
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			PostQuitMessage(0);
			break;
#ifndef _DEBUG
		case WM_SETCURSOR:
			{
			static int nGiveMeChance = 20;
			if (!nGiveMeChance--)
				PostQuitMessage(0);
			}
			break;
#endif
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}


int CScreenSaver::GetFileList(IN const char* pszPath, IN bool fIncludeSubdir, IN const set<string>& setFileExt, OUT list<string>& listFiles)
{
	char szFindMask[MAX_PATH];
	strcpy(szFindMask, pszPath);
	if (szFindMask[strlen(szFindMask) - 1] != '\\')
		strcat(szFindMask, "\\");
	strcat(szFindMask, "*");

	WIN32_FIND_DATA FindFileData;
	HANDLE hFindFile = FindFirstFile(szFindMask, &FindFileData);
	if (hFindFile == INVALID_HANDLE_VALUE)
		return 0;

	while (hFindFile != INVALID_HANDLE_VALUE) {
		string strFullPath = pszPath;
		strFullPath += "\\";
		strFullPath += FindFileData.cFileName;

		if (strcmp(FindFileData.cFileName, ".") && strcmp(FindFileData.cFileName, "..")) {
			if (fIncludeSubdir && PathIsDirectory(strFullPath.c_str()))
				GetFileList(strFullPath.c_str(), fIncludeSubdir, setFileExt, listFiles);
			else {
				//Check for file size
				if (FindFileData.nFileSizeLow && !FindFileData.nFileSizeHigh) {	//Ignore too big or zero-sized files
					//Check file extenstion mask
					size_t nExtIndex = strFullPath.find_last_of('.');
					if (nExtIndex != string::npos) {
						if (setFileExt.find(strFullPath.substr(nExtIndex + 1)) != setFileExt.end())
							listFiles.push_back(strFullPath);
					}
				}
			}
		}

		if (!FindNextFile(hFindFile, &FindFileData))
			break;
	}

	FindClose(hFindFile);

	return listFiles.size();
}


void CALLBACK CScreenSaver::OnTimer(IN HWND hWnd, IN UINT /*uMsg*/, IN UINT /*idEvent*/, IN DWORD /*dwTime*/)
{
	TCHAR szText[] = {0, 0};
	CParser::BlockType enuType = m_pIDEImitate->GetNextSymbol(&szText[0]);

	if (enuType == CParser::BTEOF) {
		//Load next file
		if (m_listFiles.empty()) {
			m_pIDEImitate->SetContent("/*\nSorry, there are no source files in folder.\nPlease check options...........................\n*/");
			//Set window title
			TCHAR pszWndTitle[1024];
			sprintf(pszWndTitle, m_pIDEImitate->GetWndTitle(), "File not found");
			SetWindowText(hWnd, pszWndTitle);
		}
		else {
			int nIndex = int(m_listFiles.size() * rand() / (RAND_MAX + 1.0));
			list<string>::const_iterator it = m_listFiles.begin();
			advance(it, nIndex);
			string strFileName = *it;
			m_pIDEImitate->LoadContent(strFileName.c_str());

			//Set window title
			TCHAR pszWndTitle[1024];
			sprintf(pszWndTitle, m_pIDEImitate->GetWndTitle(), strFileName.substr(strFileName.find_last_of('\\') + 1).c_str());
			SetWindowText(hWnd, pszWndTitle);
		}
		SetWindowText(m_hRichEdit, _T(""));	//Clear window
	}
	else {
		GETTEXTLENGTHEX gtl;
		ZeroMemory(&gtl, sizeof(gtl));
		gtl.codepage = CP_ACP;
		gtl.flags = GTL_NUMCHARS;
		DWORD dwCharCount = SendMessage(m_hRichEdit, EM_GETTEXTLENGTHEX, (WPARAM)&gtl, 0);

		CHARRANGE cr;
		cr.cpMin = cr.cpMax = dwCharCount;
		SendMessage(m_hRichEdit, EM_EXSETSEL, 0, (LPARAM)&cr);

		SendMessage(m_hRichEdit, EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)szText);

		if (szText[0] != '\n' && szText[0] != '\t' && szText[0] != ' ') {
			cr.cpMin = dwCharCount;
			cr.cpMax = -1;
			SendMessage(m_hRichEdit, EM_EXSETSEL, 0, (LPARAM)&cr);

			CHARFORMAT chfOut;
			ZeroMemory(&chfOut, sizeof(chfOut));
			chfOut.cbSize = sizeof(CHARFORMAT);
			chfOut.dwMask = CFM_COLOR;
			chfOut.crTextColor = m_pIDEImitate->GetColorByBlockType(enuType);
			SendMessage(m_hRichEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&chfOut);

			cr.cpMin = cr.cpMax = -1;
			SendMessage(m_hRichEdit, EM_EXSETSEL, 0, (LPARAM)&cr);
		}
	}	
}