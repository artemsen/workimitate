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
#include <GdiPlus.h>


class image
{
public:
	image();
	~image();

	/**
	 * Load image from resources
	 * \param id image id
	 * \return false if error
	 */
	bool load(const UINT_PTR id);

	/**
	 * Check for image loaded
	 * \return true if image loaded
	 */
	inline bool loaded() const { return _image != NULL; }

	/**
	 * Get image
	 * \return pointer to image
	 */
	inline Gdiplus::Bitmap*	img() const { return _image; }

	/**
	 * Get image width
	 * \return image width
	 */
	inline int width() const { return _image == NULL ? 0 : _image->GetWidth(); }

	/**
	 * Get image height
	 * \return image height
	 */
	inline int height() const { return _image == NULL ? 0 : _image->GetHeight(); }

private:
	Gdiplus::Bitmap*	_image;
	HGLOBAL				_buffer;
};
