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
    Implementation of the CPreview class
*/

#include "Preview.h"
#include "Resource.h"


CImage CPreview::_ImgPreview;


int CPreview::Run(const HINSTANCE hinst, const HWND wndParent)
{
	//Register wnd class
	static const wchar_t* windowClassName = L"WorkImitatePreviewWndClass";
	WNDCLASSEX wcex;
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= &CPreview::WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hinst;
	wcex.hIcon			= NULL;
	wcex.hCursor		= NULL;
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= windowClassName;
	wcex.hIconSm		= NULL;
	if (!RegisterClassEx(&wcex)) {
		MessageBox(NULL, L"Unable to register window class", NULL, MB_ICONERROR | MB_OK);
		return 1;
	}

	//Create main and static winnows
	RECT rcWnd;
	GetClientRect(wndParent, &rcWnd);
	HWND previewWnd = CreateWindow(windowClassName, NULL, WS_VISIBLE | WS_CHILD, 0, 0, rcWnd.right, rcWnd.bottom, wndParent, NULL, hinst, NULL);
	if (!previewWnd)
		return 1;

	if (!_ImgPreview.Load(hinst, IDB_PREVIEW))
		return 1;

	//Main message loop
	MSG msg;
	while (GetMessage(&msg, previewWnd, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, NULL, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	UnregisterClass(windowClassName, hinst);
	return static_cast<int>(msg.wParam);
}


LRESULT CALLBACK CPreview::WndProc(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_DESTROY)
		PostQuitMessage(0);
	else if (message == WM_PAINT) {
		RECT rc;
		GetClientRect(wnd, &rc);
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(wnd, &ps);
		_ImgPreview.Draw(hdc, 0, 0, rc.right, rc.bottom);
		EndPaint(wnd, &ps);
	}
	else
		return DefWindowProc(wnd, message, wParam, lParam);
	return 0;
}
