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

#pragma once

#include <windows.h>


class preview
{
public:
	preview();
	~preview();

	/**
	 * Run preview
	 * \param wnd_parent parent window handle
	 * \return exit code
	 */
	int run(const HWND wnd_parent);

private:
	/**
	 * Draw preview window
	 * \param wnd window handle
	 */
	void draw(const HWND wnd) const;

	/**
	 * Processes messages for the window (see MSDN for more info)
	 */
	static LRESULT CALLBACK wnd_proc(HWND wnd, UINT msg, WPARAM w_param, LPARAM l_param);

private:
	HBITMAP _image;	///< Preview image
};
