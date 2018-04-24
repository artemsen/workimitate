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

#include "preview.h"
#include "resource.h"

const wchar_t* _preview_window_class_name = L"WorkImitatePreviewWndClass";


preview::preview()
:	_image(NULL)
{
}


preview::~preview()
{
	if (_image)
		DeleteObject(_image);
	UnregisterClass(_preview_window_class_name, GetModuleHandle(NULL));
}


int preview::run(const HWND wnd_parent)
{
	//Load preview image
	_image = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_PREVIEW));
	if (!_image)
		return -1;

	//Register wnd class
	WNDCLASSEX wcex;
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= &preview::wnd_proc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= GetModuleHandle(NULL);
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= _preview_window_class_name;
	wcex.hIconSm		= NULL;
	RegisterClassEx(&wcex);

	//Create main and image windows
	RECT rc_wnd;
	GetClientRect(wnd_parent, &rc_wnd);
	HWND preview_wnd = CreateWindow(_preview_window_class_name, NULL, WS_VISIBLE | WS_CHILD, 0, 0, rc_wnd.right, rc_wnd.bottom, wnd_parent, NULL, NULL, NULL);
	if (!preview_wnd)
		return -1;
	
	SetWindowLongPtr(preview_wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	//Main message loop
	MSG msg;
	while (GetMessage(&msg, preview_wnd, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, NULL, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return static_cast<int>(msg.wParam);
}


void preview::draw(const HWND wnd) const
{
	PAINTSTRUCT ps;
	HDC dc = BeginPaint(wnd, &ps);
	RECT rc;
	GetClientRect(wnd, &rc);
	HDC dc_mem = CreateCompatibleDC(dc);
	HBITMAP bmp_old = static_cast<HBITMAP>(SelectObject(dc_mem, _image));
	BITMAP bm;
	GetObject(_image, sizeof(bm), &bm);
	StretchBlt(dc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, dc_mem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	SelectObject(dc_mem, bmp_old);
	DeleteDC(dc_mem);
	EndPaint(wnd, &ps);
}


LRESULT CALLBACK preview::wnd_proc(HWND wnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
	if (msg == WM_DESTROY)
		PostQuitMessage(0);
	else if (msg == WM_PAINT) {
		preview* inst = reinterpret_cast<preview*>(GetWindowLongPtr(wnd, GWLP_USERDATA));
		if (inst)
			inst->draw(wnd);
	}
	return DefWindowProc(wnd, msg, w_param, l_param);
}
