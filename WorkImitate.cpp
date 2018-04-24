/**************************************************************************
 *  WorkImitate screensaver (http://workimitate.sourceforge.net)          *
 *  Copyright (C) 2008 by Artem A. Senichev <artemsen@gmail.com>          *
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

/*! \file WorkImitate.cpp

    Defines the entry point for the application
*/

#include "stdafx.h"
#include "Preview.h"
#include "Configure.h"
#include "ScreenSaver.h"


/**
 * Programm entry point (see MSDN for more info)
 * @param hInstance application instance
 * @param hPrevInstance previous application instance
 * @param lpCmdLine command line (arguments)
 * @param nCmdShow window type
 * @return result
 */
int APIENTRY WinMain(IN HINSTANCE hInstance, IN HINSTANCE /*hPrevInstance*/, IN LPSTR lpCmdLine, IN int /*nCmdShow*/)
{
	//Process command line options
	enum SSMode {
		SSMScrSaver,
		SSMPreview,
		SSMConfigure
	} enuMode;
	
	if (strstr(lpCmdLine, "/c") == lpCmdLine)		//Configure mode
		enuMode = SSMConfigure;
	else if (strstr(lpCmdLine, "/p") == lpCmdLine)	//Preview mode
		enuMode = SSMPreview;
	else
		enuMode = SSMScrSaver;

	HWND hWndParent = NULL;
	if (enuMode == SSMConfigure || enuMode == SSMPreview) {
		//Define parent window handle
#pragma warning (disable: 4312) 
		hWndParent = (HWND)atoi(lpCmdLine + 3);
#pragma warning (default: 4312) 
		if (!hWndParent) {
			MessageBox(NULL, L"Unable to get window handle", NULL, MB_ICONERROR | MB_OK);
			return 1;
		}
	}

	switch (enuMode) {
		case SSMPreview:	//Preview mode
			return CPreview::Run(hInstance, hWndParent);
		case SSMConfigure:	//Configure mode
			return CConfigure::Run(hInstance, hWndParent);
		case SSMScrSaver:	//Screen saver mode
			return CScreenSaver::Run(hInstance, hWndParent);
	}

	return 0;
}
