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

#pragma once

#include "Common.h"


/*!
  Configure application
*/
class CConfigure
{
public:
	/**
	 * Do configure
	 * \param hinst application instance for loading resources
	 * \param wndParent parent window handle
	 * \return exit status
	 */
	static int Run(const HINSTANCE hinst, const HWND wndParent);

private:
	/**
	 * Processes messages for the window (see MSDN for more info)
	 */
	static LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

	/**
	 * Callback for SHBrowseForFolder (see MSDN for more info)
	 */
	static int CALLBACK BFFCallBack(HWND wnd, UINT msg, LPARAM lParam, LPARAM lpData);
};
