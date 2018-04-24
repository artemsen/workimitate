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
		hWndParent = (HWND)_ttoi(lpCmdLine + 3);
		if (!hWndParent) {
			MessageBox(NULL, _T("Unable to get window handle"), NULL, MB_ICONERROR | MB_OK);
			MessageBox(NULL, lpCmdLine, NULL, MB_ICONERROR | MB_OK);
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
