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


/*!
  Work with programm settings
*/
struct Settings
{
private:
	Settings() {}
	static Settings* _Settings;

public:
	/**
	 * Get settings implementation
	 * \return pointer to settings implementation
	 */
	static Settings* Get();

	/**
	 * Save settings
	 * \return result (false if error)
	 */
	static bool Save();

	//! Types of imitated IDE
	enum ImitateIDE {
		enuVS9 = 0,						///< Microsoft Visual Studio 9 (2008)
		enuVC6,							///< Microsoft Visual C++ 6 (98)
		enuCW,							///< Metrowerks CodeWarrior
		enuCarbide						///< Nokia Carbide C/C++
	};

	//Settings values
	wstring			Path;				///< Path
	wstring			ExtFilter;			///< File extensions filter
	bool			IncludeSubFolders;	///< Include subfolders flag
	ImitateIDE		IDE;				///< Imitated IDE type
	UINT			Speed;				///< Typing speed
};
