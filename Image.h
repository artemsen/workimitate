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

#include "Common.h"


/*!
  Load and draw images
*/
class CImage  
{
public:
	CImage();
	~CImage();

	/**
	 * Load image from resources
	 * \param hinst instance of application
	 * \param resId ID image in resources
	 * \return boolean result (true if image loaded)
	 */
	bool Load(const HINSTANCE hinst, const UINT_PTR resId);

	/**
	 * Draw image
	 * \param dc output device context handle
	 * \param x horizontal coordinate
	 * \param y vertical coordinate
	 * \param width width of image
	 * \param height height of image
	 */
	void Draw(const HDC dc, const int x, const int y, const int width = 0, const int height = 0);

	/**
	 * Image properties: Get image width
	 * \return image width
	 */
	inline size_t GetWidth() const		{ return _ImgWidth; }

	/**
	 * Image properties: Get image height
	 * \return image height
	 */
	inline size_t GetHeight() const		{ return _ImgHeight; }

	/**
	 * Check for correct loaded image
	 * \return true if image loaded
	 */
	inline bool IsLoaded() const		{ return _Bitmap != NULL; }

private:
	//Class variables
	size_t	_ImgWidth;	///< Image width
	size_t	_ImgHeight;	///< Image height
	HBITMAP	_Bitmap;	///< Image bitmap
};
