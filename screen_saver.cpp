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

#include "screen_saver.h"
#include <richedit.h>
#include <GdiPlus.h>
#include <assert.h>
#include "settings.h"
#include "ide_imitate.h"


#define TIMER_ID	0x01
const wchar_t* _main_window_class_name = L"WorkImitateWndClass";

screen_saver::screen_saver()
:	_ide_imitate(NULL),
	_rich_edit_lib(NULL), _rich_edit(NULL),
	_wnd_alpha(0),
	_mouse_move(20)
{
}


screen_saver::~screen_saver()
{
	delete _ide_imitate;
	if (_rich_edit_lib)
		FreeLibrary(_rich_edit_lib);
	UnregisterClass(_main_window_class_name, GetModuleHandle(NULL));
}


int screen_saver::run(const HWND wnd_parent)
{
	const settings& sett = settings::instance();

	//Define imitated IDE
	switch (sett.imitated_ide_type()) {
		case settings::ide_vs9:		_ide_imitate = new ide_vs9_imitate();		break;
		case settings::ide_vc6:		_ide_imitate = new ide_vc6_imitate();		break;
		case settings::ide_cw:		_ide_imitate = new ide_cw_imitate();		break;
		case settings::ide_eclipse:	_ide_imitate = new ide_eclipse_imitate();	break;
		case settings::ide_idea:	_ide_imitate = new ide_idea_imitate();		break;
		case settings::ide_vs11:
		default:
			_ide_imitate = new ide_vs11_imitate();
	}

	load_images();

	//Initialize rich edit control
	_rich_edit_lib = LoadLibrary(L"riched32.dll");
	if (!_rich_edit_lib) {
		MessageBox(wnd_parent, L"Unable to load RichEdit library", NULL, MB_ICONERROR | MB_OK);
		return -1;
	}

	HWND wnd = create_window();
	if (!wnd) {
		MessageBox(wnd_parent, L"Unable to create window", NULL, MB_ICONERROR | MB_OK);
		return -1;
	}

	//Show main window in max size and set rich edit control position and size
	ShowWindow(wnd, SW_SHOWMAXIMIZED);
	RECT wnd_rc;
	GetClientRect(wnd, &wnd_rc);
	wnd_rc.left   += _images[ide_imitate::ip_lm].width();
	wnd_rc.right  -= _images[ide_imitate::ip_rm].width();
	wnd_rc.top    += _images[ide_imitate::ip_tm].height();
	wnd_rc.bottom -= _images[ide_imitate::ip_bm].height();
	SetWindowPos(_rich_edit, NULL, wnd_rc.left, wnd_rc.top, wnd_rc.right - wnd_rc.left, wnd_rc.bottom - wnd_rc.top, SWP_SHOWWINDOW | SWP_NOZORDER);

	UpdateWindow(wnd);
	_parser.intialize();

	SetTimer(wnd, TIMER_ID,
#ifdef _DEBUG
	1
#else
	sett.typing_speed()
#endif // _DEBUG
	, &screen_saver::on_timer);

	//Main message loop
	MSG msg;
	while (GetMessage(&msg, wnd, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	KillTimer(wnd, TIMER_ID);

	return static_cast<int>(msg.wParam);
}


HWND screen_saver::create_window()
{
	assert(_ide_imitate);
	assert(!_rich_edit);

	//Register window class and create main window
	WNDCLASSEX wcex;
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= &screen_saver::wnd_proc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= GetModuleHandle(NULL);
	wcex.hIcon			= wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(_ide_imitate->icon_id()));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= _main_window_class_name;
	RegisterClassEx(&wcex);

	HWND wnd = CreateWindowEx(WS_EX_LAYERED, _main_window_class_name, NULL, WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, NULL, NULL);
	if (!wnd)
		return NULL;

	//Set window title
	SetWindowText(wnd, _ide_imitate->title());

	//Set this class as window data
	SetWindowLongPtr(wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	//Create Rich edit control
	_rich_edit = CreateWindow(RICHEDIT_CLASS, NULL, WS_CHILD | WS_VSCROLL | WS_HSCROLL | WS_VISIBLE | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, 0, 0, 0, 0, wnd, NULL, NULL, NULL);
	CHARFORMAT chf;
	ZeroMemory(&chf, sizeof(chf));
	chf.cbSize = sizeof(chf);
	chf.dwMask = CFM_COLOR | CFM_BOLD | CFM_SIZE | CFM_FACE | CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT;
	lstrcpy(chf.szFaceName, L"Courier New");
	chf.yHeight = 10 * 20;
	chf.dwEffects = 0;
	chf.crTextColor = RGB(0, 0, 0);
	SendMessage(_rich_edit, EM_SETCHARFORMAT, 0, reinterpret_cast<LPARAM>(&chf));
	SendMessage(_rich_edit, EM_SETEVENTMASK, 0, ENM_KEYEVENTS);
	SetFocus(_rich_edit);

	return wnd;
}


void screen_saver::load_images()
{
	for (size_t i = 0; i < ide_imitate::ip_last; ++i) {
		const UINT_PTR res_id = _ide_imitate->image_resource_id(static_cast<ide_imitate::image_place>(i));
		if (res_id)
			_images[i].load(res_id);
	}
}


void screen_saver::draw(const HWND wnd) const
{
	assert(_ide_imitate);
	assert(wnd);

	PAINTSTRUCT ps;
	HDC dc = BeginPaint(wnd, &ps);
	assert(dc);

	Gdiplus::Graphics graphics(dc);

	RECT wnd_rc;
	GetClientRect(wnd, &wnd_rc);

	//Fill middle lines
	if (_images[ide_imitate::ip_lm].loaded()) {
		const INT start = _images[ide_imitate::ip_tl].height() + _images[ide_imitate::ip_lt].height();
		const INT end = wnd_rc.bottom - _images[ide_imitate::ip_bl].height() - _images[ide_imitate::ip_lb].height();
		const INT step = _images[ide_imitate::ip_lm].height();
		for (INT i = start; i < end; i += step)
			graphics.DrawImage(_images[ide_imitate::ip_lm].img(), 0, i);
	}
	if (_images[ide_imitate::ip_rm].loaded()) {
		const INT start = _images[ide_imitate::ip_tr].height() + _images[ide_imitate::ip_rt].height();
		const INT end = wnd_rc.bottom - _images[ide_imitate::ip_br].height() - _images[ide_imitate::ip_rb].height();
		const INT step = _images[ide_imitate::ip_rm].height();
		const INT x = wnd_rc.right - _images[ide_imitate::ip_rm].width();
		for (INT i = start; i < end; i += step)
			graphics.DrawImage(_images[ide_imitate::ip_rm].img(), x, i);
	}
	if (_images[ide_imitate::ip_tm].loaded()) {
		const INT start = _images[ide_imitate::ip_tl].height();
		const INT end = wnd_rc.right - _images[ide_imitate::ip_tr].width();
		const INT step = _images[ide_imitate::ip_tm].width();
		for (INT i = start; i < end; i += step)
			graphics.DrawImage(_images[ide_imitate::ip_tm].img(), i, 0);
	}
	if (_images[ide_imitate::ip_bm].loaded()) {
		const INT start = _images[ide_imitate::ip_bl].width();
		const INT end = wnd_rc.right - _images[ide_imitate::ip_br].width();
		const INT step = _images[ide_imitate::ip_bm].width();
		const INT y = wnd_rc.bottom - _images[ide_imitate::ip_bm].height();
		for (INT i = start; i < end; i += step)
			graphics.DrawImage(_images[ide_imitate::ip_bm].img(), i, y);
	}

	//Fill inner corners
	if (_images[ide_imitate::ip_lt].loaded())
		graphics.DrawImage(_images[ide_imitate::ip_lt].img(), 0, _images[ide_imitate::ip_tl].height());
	if (_images[ide_imitate::ip_lb].loaded())
		graphics.DrawImage(_images[ide_imitate::ip_lb].img(), 0, wnd_rc.bottom - _images[ide_imitate::ip_lb].height() - _images[ide_imitate::ip_bl].height());

	//Fill outer corners
	if (_images[ide_imitate::ip_tl].loaded())
		graphics.DrawImage(_images[ide_imitate::ip_tl].img(), 0, 0);
	if (_images[ide_imitate::ip_tr].loaded())
		graphics.DrawImage(_images[ide_imitate::ip_tr].img(), wnd_rc.right - _images[ide_imitate::ip_tr].width(), 0);
	if (_images[ide_imitate::ip_bl].loaded())
		graphics.DrawImage(_images[ide_imitate::ip_bl].img(), 0, wnd_rc.bottom - _images[ide_imitate::ip_bl].height());
	if (_images[ide_imitate::ip_br].loaded())
		graphics.DrawImage(_images[ide_imitate::ip_br].img(), wnd_rc.right - _images[ide_imitate::ip_br].width(), wnd_rc.bottom - _images[ide_imitate::ip_br].height());

	EndPaint(wnd, &ps);
}


LRESULT CALLBACK screen_saver::wnd_proc(HWND wnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
	switch (msg) {
		case WM_NOTIFY: {
			screen_saver* inst = reinterpret_cast<screen_saver*>(GetWindowLongPtr(wnd, GWLP_USERDATA));
			const LPNMHDR hdr = reinterpret_cast<LPNMHDR>(l_param);
			if (inst && inst->_rich_edit == hdr->hwndFrom && hdr->code == EN_MSGFILTER)
				PostMessage(wnd, WM_CLOSE, 0, 0);
			}
			break;
		case WM_PAINT: {
			screen_saver* inst = reinterpret_cast<screen_saver*>(GetWindowLongPtr(wnd, GWLP_USERDATA));
			if (inst)
				inst->draw(wnd);
			}
			break;
 		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			PostMessage(wnd, WM_CLOSE, 0, 0);
			break;
#ifndef _DEBUG
		case WM_SETCURSOR: {
			screen_saver* inst = reinterpret_cast<screen_saver*>(GetWindowLongPtr(wnd, GWLP_USERDATA));
			if (inst && !--inst->_mouse_move)
				PostMessage(wnd, WM_CLOSE, 0, 0);
			}
			break;
#endif	//_DEBUG
	}

	return DefWindowProc(wnd, msg, w_param, l_param);
}


void CALLBACK screen_saver::on_timer(HWND wnd, UINT /*msg*/, UINT_PTR /*event*/, DWORD /*time*/)
{
	screen_saver* inst = reinterpret_cast<screen_saver*>(GetWindowLongPtr(wnd, GWLP_USERDATA));
	if (!inst)
		return;

	//Make visible from transparent state
	if (inst->_wnd_alpha < 255) {
		inst->_wnd_alpha += settings::instance().typing_speed() / 2;
		if (inst->_wnd_alpha > 255)
			inst->_wnd_alpha = 255;
		SetLayeredWindowAttributes(wnd, 0, inst->_wnd_alpha, LWA_ALPHA);
	}

	if (!inst->_parser.step())
		SetWindowText(inst->_rich_edit, L"");	//Clear window

	const wchar_t text[2] = { inst->_parser.current_symbol(), 0 };
	const GETTEXTLENGTHEX gtl = { GTL_NUMCHARS, 1200 };
	const LONG char_count = static_cast<LONG>(SendMessage(inst->_rich_edit, EM_GETTEXTLENGTHEX, reinterpret_cast<WPARAM>(&gtl), 0));

	CHARRANGE cr;
	cr.cpMin = cr.cpMax = char_count;
	SendMessage(inst->_rich_edit, EM_EXSETSEL, 0, reinterpret_cast<LPARAM>(&cr));

	SendMessage(inst->_rich_edit, EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(text));

	if (text[0] != '\r' && text[0] != '\n' && text[0] != '\t' && text[0] != ' ') {
		cr.cpMin = char_count;
		cr.cpMax = -1;
		SendMessage(inst->_rich_edit, EM_EXSETSEL, 0, reinterpret_cast<LPARAM>(&cr));

		CHARFORMAT chf;
		ZeroMemory(&chf, sizeof(chf));
		chf.cbSize = sizeof(chf);
		chf.dwMask = CFM_COLOR;
		chf.crTextColor = inst->_ide_imitate->color_for_token(inst->_parser.current_token());
		SendMessage(inst->_rich_edit, EM_SETCHARFORMAT, SCF_SELECTION, reinterpret_cast<LPARAM>(&chf));

		cr.cpMin = cr.cpMax = -1;
		SendMessage(inst->_rich_edit, EM_EXSETSEL, 0, reinterpret_cast<LPARAM>(&cr));
	}

	//Scroll rich edit control's bar
	SendMessage(inst->_rich_edit, EM_SCROLL, SB_PAGEDOWN, 0);
}
