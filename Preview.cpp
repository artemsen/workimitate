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

/*! \file Preview.cpp

    Implementation of the CPreview class
*/

#include "StdAfx.h"
#include "Preview.h"
#include "Resource.h"


CImage CPreview::m_imgPreview;


int CPreview::Run(IN HINSTANCE hInstance, IN HWND hParent)
{
	//Register wnd class
	const char* pszWindowClassName = "WorkImitatePreviewWndClass";
	WNDCLASSEX wcex;
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)CPreview::WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= NULL;
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= pszWindowClassName;
	wcex.hIconSm		= NULL;
	if (!RegisterClassEx(&wcex)) {
		MessageBox(NULL, _T("Unable to register window class"), NULL, MB_ICONERROR | MB_OK);
		return 1;
	}

	//Create main and static winnows
	RECT rcWnd;
	GetClientRect(hParent, &rcWnd);
	HWND hWnd = CreateWindow(pszWindowClassName, NULL, WS_VISIBLE | WS_CHILD, 0, 0, rcWnd.right, rcWnd.bottom, hParent, NULL, hInstance, NULL);
	if (!hWnd)
		return 1;

	m_imgPreview.Load(hInstance, IDB_PREVIEW);

	//Main message loop
	MSG msg;
	while (GetMessage(&msg, hWnd, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, NULL, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	UnregisterClass(pszWindowClassName, hInstance);
	return msg.wParam;
}


LRESULT CALLBACK CPreview::WndProc(IN HWND hWnd, IN UINT message, IN WPARAM wParam, IN LPARAM lParam)
{
	if (message == WM_DESTROY)
		PostQuitMessage(0);
	else if (message == WM_PAINT) {
		RECT rc;
		GetClientRect(hWnd, &rc);
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hWnd, &ps);
		m_imgPreview.Draw(hdc, 0, 0, rc.right, rc.bottom);
		EndPaint(hWnd, &ps);
	}
	else
		return DefWindowProc(hWnd, message, wParam, lParam);
	return 0;
}
