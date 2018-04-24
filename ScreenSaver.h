/**************************************************************************
 *  WorkImitate screensaver (http://workimitate.sourceforge.net)          *
 *  Copyright (C) 2007-2008 by Artem A. Senichev <artemsen@gmail.com>     *
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
  Screen ssave class
*/
class CScreenSaver
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

	/**
	 * Timer handler (see MSDN for more info)
	 * @param hWnd handle to the window associated with the timer
	 * @param uMsg specifies the WM_TIMER message
	 * @param idEvent specifies the timer's identifier
	 * @param dwTime specifies the number of milliseconds that have elapsed since the system was started. This is the value returned by the GetTickCount function.
	 */
	static void CALLBACK OnTimer(IN HWND hWnd, IN UINT uMsg, IN UINT idEvent, IN DWORD dwTime);

	/**
	 * Get file list
	 * @param pszPath path to folder
	 * @param fIncludeSubdir flag to inlude subfolders
	 * @param setFileExt set of file extensions (filter)
	 * @param listFiles file list
	 * @return found files count
	 */
	static UINT GetFileList(IN LPCWSTR lpszPath, IN bool fIncludeSubdir, IN const set<wstring>& setFileExt, OUT list<wstring>& listFiles);

private:
	//Class variables
	static list<wstring>	m_listFiles;	///< List of source files to show in imitated IDE
	static HWND				m_hRichEdit;	///< RichEdit window handle
	static CIDEImitate*		m_pIDEImitate;	///< IDE imitate implementation
	static map<CIDEImitate::ImagePlace, int> m_mapCoordinates;	///< Coordinates of images
};
