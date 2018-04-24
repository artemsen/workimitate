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
  Work with programm settings
*/
class CSettings
{
public:

	/**
	 * Constructor
	 * param fLoadSettings indication to load settings (true if needed)
	 */
	CSettings(IN bool fLoadSettings = true);
	//! Default destructor
	~CSettings(void);


	//! Types of imitated IDE
	enum ImitateIDE {
		enuVS9 = 0,						///< Microsoft Visual Studio 9 (2008)
		enuVC6,							///< Microsoft Visual C++ 6 (98)
		enuCW							///< Metrowerks CodeWarrior
	};

	//Settings values
	TCHAR		Path[MAX_PATH];		///< Path
	bool		IncludeSubFolders;	///< Include subfolders flag
	bool		EnhColor;			///< Enhanced color syntax
	ImitateIDE	IDE;				///< Imitated IDE type
	int			Speed;				///< Typing speed
	set<string>	Exts;				///< Files extensions


	/**
	 * Load settings
	 * @return result (false if error)
	 */
	bool Load(void);


	/**
	 * Save settings
	 * @return result (false if error)
	 */
	bool Save(void);
};
