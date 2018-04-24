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

#include "Resource.h"
#include "Parser.h"
#include "Image.h"


/*!
  Imitated IDE abstract class
*/
class CIDEImitate : public CParser
{
public:
	
	//! Image place identifier
	enum ImagePlace {
		IPTop,
		IPBottom,
		IPLeft
	};

	/**
	 * Get resource image ID (0 if unused)
	 * \param type image place identifier
	 * \return image ID
	 */
	virtual UINT_PTR GetImageId(const ImagePlace type) const = 0;

	/**
	 * Get IDE icon ID
	 * \return icon ID
	 */
	virtual UINT_PTR GetIconId() const = 0;

	/**
	 * Get IDE window title template
	 * \return window title template
	 */
	virtual const wchar_t* GetWndTitle() const = 0;

public:

	/**
	 * Load images
	 * \param hInstance instance of appliaction
	 * \return boolean result (false if error)
	 */
	bool LoadImages(const HINSTANCE hinst);

	/**
	 * Get image image ID (0 if unused)
	 * \param type image place identifier
	 * \return pointer to instance of image class
	 */
	CImage* GetImage(const ImagePlace type);

private:
	//Class variables
	CImage	_ImageTop;		///< Top image
	CImage	_ImageBottom;	///< Bottom image
	CImage	_ImageLeft;		///< Left image
};


/*!
  Implementation of imitated Microsoft IDE
*/
class CIDEMS : public CIDEImitate
{
public:
	//From CParser
	COLORREF GetColorByBlockType(const BlockType type) const;
};


/*!
  Implementation of imitated Microsoft Visual C++ 6 (98) IDE
*/
class CIDEVC6 : public CIDEMS
{
public:
	//From CIDEImitate
	UINT_PTR GetImageId(const ImagePlace type) const;
	UINT_PTR GetIconId() const			{ return IDI_VC6; }
	const wchar_t* GetWndTitle() const	{ return L"WorkImitate - Microsoft Visual C++ - [%s]"; }
};


/*!
  Implementation of imitated Microsoft Visual Studio 2008 IDE
*/
class CIDEVS9 : public CIDEMS
{
public:
	//From CIDEImitate
	UINT_PTR GetImageId(const ImagePlace type) const;
	UINT_PTR GetIconId() const			{ return IDI_VS9; }
	const wchar_t* GetWndTitle() const	{ return L"WorkImitate - Microsoft Visual Studio [%s]"; }

	//From CParser
	BlockType GetBlockType(IN const char* pszWord);
};


/*!
  Implementation of imitated Metrowerks CodeWarrior IDE
*/
class CIDECW : public CIDEImitate
{
public:
	//From CIDEImitate
	UINT_PTR GetImageId(const ImagePlace type) const;
	UINT_PTR GetIconId() const			{ return IDI_CW; }
	const wchar_t* GetWndTitle() const	{ return L"Metrowerks CodeWarrior - [%s]"; }

	//From CParser
	COLORREF GetColorByBlockType(const BlockType type) const;
	BlockType GetBlockType(const char* word);
};


/*!
  Implementation of imitated Nokia Carbide.c++
*/
class CIDECarbide : public CIDEImitate
{
public:
	//From CIDEImitate
	UINT_PTR GetImageId(const ImagePlace type) const;
	UINT_PTR GetIconId() const			{ return IDI_CARBIDE; }
	const wchar_t* GetWndTitle() const	{ return L"Carbide C/C++ - %s - Carbide.c++"; }
};
