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

#include <windows.h>
#include <GdiPlus.h>
#include "screen_saver.h"
#include "settings.h"
#include "preview.h"
#include "first_launch.h"

//Common controls support
#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif


/**
 * Program entry point (see MSDN for more info)
 */
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR cmd_line, int)
{
	int ret_code = -1;

	char mode = 0;	//By default - install query
	HWND wnd_parent = NULL;

	//Parse command line
	if (cmd_line && *cmd_line) {
		while (*cmd_line && !isalpha(*cmd_line))
			++cmd_line;

		if (*cmd_line) {
			mode = *cmd_line++;

			//Get window handle
			while (*cmd_line && !isdigit(*cmd_line))
				++cmd_line;

			if (*cmd_line)
				wnd_parent = reinterpret_cast<HWND>(atoi(cmd_line));
		}
	}

#ifdef _DEBUG
	if (!wnd_parent)
		wnd_parent = GetDesktopWindow();
#endif // _DEBUG

	//Load settings
	settings::instance().load();

	//Initialize GDI+
	ULONG_PTR _gdiplus_token;
	Gdiplus::GdiplusStartupInput gdiplus_si;
	Gdiplus::GdiplusStartup(&_gdiplus_token, &gdiplus_si, NULL);

	switch (mode) {
		case 'c':	//Configure mode
		case 'C':
			ret_code = settings::instance().configure(wnd_parent);
			break;
		case 'p':	//Preview mode
		case 'P':
			ret_code = preview().run(wnd_parent);
			break;
		case 's':	//Screen saver mode
		case 'S':
			ret_code = screen_saver().run(wnd_parent);
			break;
		case'i':	//Install mode
		case'I':
			ret_code = first_launch().install();		
			break;
		default:
			ret_code = first_launch().run();
	}

	Gdiplus::GdiplusShutdown(_gdiplus_token);

	return ret_code;
}
