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
    Implementation of the CScreenSaver class
*/

#include "Common.h"
#include "ScreenSaver.h"
#include "Resource.h"
#include "Settings.h"


list<wstring>	CScreenSaver::_FileList;
HWND			CScreenSaver::_RichEdit = NULL;
CIDEImitate*	CScreenSaver::_IDEImitate = NULL;
map<CIDEImitate::ImagePlace, int> CScreenSaver::_Coordinates;

#define TIMER_ID	0x01


int CScreenSaver::Run(const HINSTANCE hinst, const HWND wndParent)
{
	Settings* settings = Settings::Get();

	//Define imitated IDE
	_IDEImitate = NULL;
	switch (settings->IDE) {
		case Settings::enuVS9:		_IDEImitate = new CIDEVS9();
			break;
		case Settings::enuVC6:		_IDEImitate = new CIDEVC6();
			break;
		case Settings::enuCW:		_IDEImitate = new CIDECW();
			break;
		case Settings::enuCarbide:	_IDEImitate = new CIDECarbide();
			break;
		default:
			assert(false && "Undefined type");
	}
	if (!_IDEImitate) {
		MessageBox(wndParent, L"Imitated IDE undefined", NULL, MB_ICONERROR | MB_OK);
		return 1;
	}

	//Load images
	if (!_IDEImitate->LoadImages(hinst)) {
		MessageBox(wndParent, L"Unable to load images", NULL, MB_ICONERROR | MB_OK);
		return 1;
	}

	//Register window class and create main window
	static const wchar_t* windowClassName = L"WorkImitateWndClass";
	WNDCLASSEX wcex;
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= &CScreenSaver::WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hinst;
	wcex.hIcon			= LoadIcon(hinst, (LPCTSTR)_IDEImitate->GetIconId());
	wcex.hCursor		= NULL;
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= windowClassName;
	wcex.hIconSm		= LoadIcon(hinst, (LPCTSTR)_IDEImitate->GetIconId());
	if (!RegisterClassEx(&wcex)) {
		MessageBox(wndParent, L"Unable to register window class", NULL, MB_ICONERROR | MB_OK);
		return 1;
	}
	HWND wnd = CreateWindow(windowClassName, NULL, WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, hinst, NULL);
	if (!wnd) {
		MessageBox(wndParent, L"Unable to create window", NULL, MB_ICONERROR | MB_OK);
		return 1;
	}

	//Initialize richedit
	HINSTANCE richEditLib = LoadLibrary(L"riched32.dll");
	if (!richEditLib) {
		MessageBox(wndParent, L"Unable to load RichEdit library", NULL, MB_ICONERROR | MB_OK);
		return 1;
	}

	_RichEdit = CreateWindow(RICHEDIT_CLASS, NULL, WS_CHILD | WS_BORDER | WS_VSCROLL | WS_HSCROLL | WS_VISIBLE | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, 0, 0, 0, 0, wnd, NULL, hinst, NULL);
	CHARFORMAT chfOut;
	ZeroMemory(&chfOut, sizeof(chfOut));
	chfOut.cbSize = sizeof(CHARFORMAT);
	chfOut.dwMask = CFM_BOLD | CFM_COLOR | CFM_SIZE | CFM_FACE | CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT;
	lstrcpy(chfOut.szFaceName, /*sizeof(chfOut.szFaceName) / sizeof(TCHAR),*/ L"Courier New");
	chfOut.yHeight = 10 * 20;
	chfOut.dwEffects = 0;
	chfOut.crTextColor = RGB(0, 0, 0);
	SendMessage(_RichEdit, EM_SETCHARFORMAT, 0, (LPARAM)&chfOut);
	SendMessage(_RichEdit, EM_SETEVENTMASK, 0, ENM_KEYEVENTS);
	SetFocus(_RichEdit);

	//Show main window
	ShowWindow(wnd, SW_SHOWMAXIMIZED);

	//Getimages heights
	const int topHeight = static_cast<int>(_IDEImitate->GetImage(CIDEImitate::IPTop)->GetHeight());
	const int bottomHeight = static_cast<int>(_IDEImitate->GetImage(CIDEImitate::IPBottom)->GetHeight());
	const int leftWidth = static_cast<int>(_IDEImitate->GetImage(CIDEImitate::IPLeft)->GetWidth());
	
	//Set windows position and size
	RECT rcWnd;
	GetClientRect(wnd, &rcWnd);
	_Coordinates[CIDEImitate::IPTop] = 0;
	_Coordinates[CIDEImitate::IPBottom] = rcWnd.bottom - bottomHeight;
	_Coordinates[CIDEImitate::IPLeft] = topHeight;
	SetWindowPos(_RichEdit, NULL, leftWidth, topHeight, rcWnd.right - leftWidth, rcWnd.bottom - bottomHeight - topHeight, SWP_SHOWWINDOW | SWP_NOZORDER);

	//Randomize timer
	srand(GetTickCount());

	//Hide cursor and update main window
	ShowCursor(FALSE);
	UpdateWindow(wnd);

	//Getting file list
	if (!settings->Path.empty())
		GetFileList(settings->Path.c_str(), _FileList);

	//Register timer
	SetTimer(wnd, TIMER_ID, static_cast<int>(settings->Speed), &CScreenSaver::OnTimer);

	//Main message loop
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, NULL, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	FreeLibrary(richEditLib);
	UnregisterClass(windowClassName, hinst);

	return static_cast<int>(msg.wParam);
}


LRESULT CALLBACK CScreenSaver::WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
		case WM_NOTIFY:
			if (LPNMHDR(lParam)->hwndFrom == _RichEdit && LPNMHDR(lParam)->code == EN_MSGFILTER)
				PostQuitMessage(0);
			else
				return DefWindowProc(wnd, msg, wParam, lParam);
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC dc = BeginPaint(wnd, &ps);
				_IDEImitate->GetImage(CIDEImitate::IPTop)->Draw(dc, 0, _Coordinates[CIDEImitate::IPTop]);
				_IDEImitate->GetImage(CIDEImitate::IPLeft)->Draw(dc, 0, _Coordinates[CIDEImitate::IPLeft]);
				_IDEImitate->GetImage(CIDEImitate::IPBottom)->Draw(dc, 0, _Coordinates[CIDEImitate::IPBottom]);
				EndPaint(wnd, &ps);
			}
			break;
		case WM_DESTROY:
#ifndef _DEBUG
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
#endif	//_DEBUG
			PostQuitMessage(0);
			break;
#ifndef _DEBUG
		case WM_SETCURSOR:
			{
				static int giveMeChance = 20;
				if (!--giveMeChance)
					PostQuitMessage(0);
			}
			break;
#endif	//_DEBUG
		default:
			return DefWindowProc(wnd, msg, wParam, lParam);
   }
   return 0;
}


size_t CScreenSaver::GetFileList(const wchar_t* path, list<wstring>& fileList)
{
	if (lstrlen(path) == 0)
		return 0;

	//Convert extensions filter string to set
	static set<wstring> fileExt;
	if (fileExt.empty()) {
		wstring extString(Settings::Get()->ExtFilter);
		size_t posStart = 0;
		size_t posEnd = 0;
		static const wchar_t* delims = L" ;,";
		while (posEnd < extString.length()) {
			posStart = extString.find_first_not_of(delims, posEnd);
			if (posStart == wstring::npos)
				break;
			posEnd = extString.find_first_of(delims, posStart);
			fileExt.insert(extString.substr(posStart, posEnd - posStart));
		}
	}

	static const bool includeSubdir = Settings::Get()->IncludeSubFolders;

	wstring findPath = path;
	findPath += L"\\*";

	WIN32_FIND_DATA findFileData;
	HANDLE findHandle = FindFirstFile(findPath.c_str(), &findFileData);
	if (findHandle == INVALID_HANDLE_VALUE)
		return 0;

	do {
		wstring fullPath = path;
		fullPath += L'\\';
		fullPath += findFileData.cFileName;

		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (includeSubdir && lstrcmp(findFileData.cFileName, L".") != 0 && lstrcmp(findFileData.cFileName, L"..") != 0)
				GetFileList(fullPath.c_str(), fileList);
		}
		else {
			//Check for file size
			if (findFileData.nFileSizeLow && !findFileData.nFileSizeHigh) {	//Ignore too big or zero-sized files
				//Check file extenstion mask
				const size_t extPos = fullPath.rfind('.');
				if (extPos != string::npos) {
					wstring ext = fullPath.substr(extPos + 1);
					transform(ext.begin(), ext.end(), ext.begin(), tolower);
					if (fileExt.find(ext) != fileExt.end())
						fileList.push_back(fullPath);
				}
			}
		}
	}
	while (FindNextFile(findHandle, &findFileData));

	FindClose(findHandle);

	return fileList.size();
}


void CALLBACK CScreenSaver::OnTimer(HWND wnd, UINT /*msg*/, UINT_PTR /*idEvent*/, DWORD /*dwTime*/)
{
	char szText[] = {0, 0};
	CParser::BlockType enuType = _IDEImitate->GetNextSymbol(&szText[0]);

	if (enuType == CParser::BTEOF) {
		if (_FileList.empty()) {
			//Set default content
			HRSRC hRsrc = FindResource(NULL, MAKEINTRESOURCE(IDR_SAMPLECODE), L"BINDATA");
			if (hRsrc) {
				const char* lpRsrc = static_cast<const char*>(LoadResource(NULL, hRsrc));
				if (lpRsrc)
					_IDEImitate->SetContent(lpRsrc);
				FreeResource(hRsrc);
			}
			//Set window title
			wchar_t pszWndTitle[1024];
			swprintf(pszWndTitle, _IDEImitate->GetWndTitle(), L"[File not found]");
			SetWindowText(wnd, pszWndTitle);
		}
		else {
			const size_t index = rand() % _FileList.size();
			list<wstring>::const_iterator it = _FileList.begin();
			advance(it, index);
			wstring strFileName = *it;
			_IDEImitate->LoadContent(strFileName.c_str());

			//Set window title
			wchar_t pszWndTitle[1024];
			swprintf(pszWndTitle, _IDEImitate->GetWndTitle(), strFileName.substr(strFileName.find_last_of('\\') + 1).c_str());
			SetWindowText(wnd, pszWndTitle);
		}
		SetWindowText(_RichEdit, L"");	//Clear window
	}
	else {
		GETTEXTLENGTHEX gtl;
		ZeroMemory(&gtl, sizeof(gtl));
		gtl.codepage = CP_ACP;
		gtl.flags = GTL_NUMCHARS;
		LONG dwCharCount = static_cast<LONG>(SendMessage(_RichEdit, EM_GETTEXTLENGTHEX, (WPARAM)&gtl, 0));

		CHARRANGE cr;
		cr.cpMin = cr.cpMax = dwCharCount;
		SendMessage(_RichEdit, EM_EXSETSEL, 0, (LPARAM)&cr);

		SendMessageA(_RichEdit, EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)szText);

		if (szText[0] != '\r' && szText[0] != '\n' && szText[0] != '\t' && szText[0] != ' ') {
			cr.cpMin = dwCharCount;
			cr.cpMax = -1;
			SendMessage(_RichEdit, EM_EXSETSEL, 0, (LPARAM)&cr);

			CHARFORMAT chfOut;
			ZeroMemory(&chfOut, sizeof(chfOut));
			chfOut.cbSize = sizeof(CHARFORMAT);
			chfOut.dwMask = CFM_COLOR;
			chfOut.crTextColor = _IDEImitate->GetColorByBlockType(enuType);
			SendMessage(_RichEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&chfOut);

			cr.cpMin = cr.cpMax = -1;
			SendMessage(_RichEdit, EM_EXSETSEL, 0, (LPARAM)&cr);
		}
	}	
}
