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

#pragma once


/*!
  Configure application
*/
class CConfigure
{
public:
	/**
	 * Do configure
	 * @param hInstance application instance for loading resources
	 * @param hParent parent window handle
	 */
	static int Run(IN HINSTANCE hInstance, IN HWND hParent);

private:
	/**
	 * Processes messages for the window (see MSDN for more info)
	 * @param hWnd window handle
	 * @param message message ID
	 * @param wParam param
	 * @param lParam param
	 * @return result
	 */
	static LRESULT CALLBACK WndProc(IN HWND hWnd, IN UINT message, IN WPARAM wParam, IN LPARAM lParam);
};
