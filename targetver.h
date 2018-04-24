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

/*! \file targetver.h

	The following macros define the minimum required platform.  The minimum required platform
	is the earliest version of Windows, Internet Explorer etc. that has the necessary features to run 
	your application.  The macros work by enabling all features available on platform versions up to and 
	including the version specified.
*/


#pragma once

#ifndef WINVER
#define WINVER			0x0400
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT	0x0400
#endif

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS	0x0400
#endif

#ifndef _WIN32_IE
#define _WIN32_IE		0x0400
#endif
