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

#include <Windows.h>
#include <set>
#include <string>
using namespace std;


class settings
{
private:
	settings();

public:
	//! Types of imitated IDE
	enum ide_type {
		ide_vs11 = 0,	///< Microsoft Visual Studio 11
		ide_vs9,		///< Microsoft Visual Studio 9 (2008)
		ide_vc6,		///< Microsoft Visual C++ 6 (98)
		ide_cw,			///< Metrowerks CodeWarrior
		ide_eclipse,	///< Eclipse
		ide_idea		///< IntelliJ IDEA
	};

	/**
	 * Get settings class instance
	 * \return settings class instance
	 */
	static settings& instance();

	/**
	 * Load settings
	 */
	void load();

	/**
	 * Configure screen saver
	 * \param wnd_parent parent window handle
	 * \return exit code
	 */
	int configure(const HWND wnd_parent);

	//Settings accessors
	const wchar_t* sources_path() const				{ return _path.c_str(); }
	bool recursive_search() const					{ return _recursive; }
	const set<wstring>& extenstion_filter() const	{ return _ext_filter; }
	ide_type imitated_ide_type() const				{ return _ide; }
	unsigned int typing_speed() const				{ return _speed; }

private:
	/**
	 * Save current settings
	 */
	void save() const;

	/**
	 * Convert extensions filter to string
	 * \return extensions filter as string
	 */
	wstring filter_to_string() const;

	/**
	 * Convert extensions filter from string
	 * \param filter extensions filter as string
	 */
	void filter_from_string(const wstring& filter);

	/**
	 * Processes messages for the window (see MSDN for more info)
	 */
	static INT_PTR CALLBACK dlg_proc(HWND wnd, UINT msg, WPARAM w_param, LPARAM l_param);

	/**
	 * Callback for SHBrowseForFolder (see MSDN for more info)
	 */
	static int CALLBACK bff_callback(HWND wnd, UINT msg, WPARAM w_param, LPARAM l_param);

private:
	//Settings values
	wstring			_path;				///< Path
	bool			_recursive;			///< Include subfolders flag
	set<wstring>	_ext_filter;		///< File extensions filter
	ide_type		_ide;				///< Imitated IDE type
	unsigned int	_speed;				///< Typing speed
};
