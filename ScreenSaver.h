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

#include "IDEImitate.h"
#include "Image.h"


/*!
  Screen saver class
*/
class CScreenSaver
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
	 * Timer handler (see MSDN for more info)
	 */
	static void CALLBACK OnTimer(HWND wnd, UINT msg, UINT_PTR idEvent, DWORD dwTime);

	/**
	 * Get file list
	 * \param path path to folder
	 * \param fileList file list
	 * \return found files count
	 */
	static size_t GetFileList(const wchar_t* path, list<wstring>& fileList);

private:
	//Class variables
	static list<wstring>	_FileList;		///< List of source files to show in imitated IDE
	static HWND				_RichEdit;		///< RichEdit window handle
	static CIDEImitate*		_IDEImitate;	///< IDE imitate implementation
	static map<CIDEImitate::ImagePlace, int> _Coordinates;	///< Coordinates of images
};
