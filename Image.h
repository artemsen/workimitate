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
  Load and draw images
*/
class CImage  
{
public:
	//! Default constructor
	CImage();
	//! Default destructor
	~CImage();

	/**
	 * Load image from resources
	 * @param hInstance instance of application
	 * @param nIDRes ID image in resources
	 * @return boolean result (true if image loaded)
	 */
	bool Load(IN HINSTANCE hInstance, IN UINT nIDRes);

	/**
	 * Draw image
	 * @param hDC output device context handle
	 * @param nX horizontal coordinate
	 * @param nY vertical coordinate
	 * @param nWidth width of image
	 * @param nHeight height of image
	 */
	void Draw(IN HDC hDC, IN int nX, IN int nY, IN int nWidth = 0, IN int nHeight = 0);

	/**
	 * Get width of image in pixels for specified DC
	 * @param hDC output device context handle
	 * @return images width
	 */
	int GetWidth(IN HDC hDC) const;

	/**
	 * Get height of image in pixels for specified DC
	 * @param hDC output device context handle
	 * @return images height
	 */
	int GetHeight(IN HDC hDC) const;

	/**
	 * Check for correct loaded image
	 * @return true if image loaded
	 */
	bool IsLoaded(void) const		{ return m_pPicture != NULL; }

private:
	//Class variables
	CComPtr<IPicture> m_pPicture;	///< Picture interface pointer
};
