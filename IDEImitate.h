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
		IPLeft,
		IPRight	//Unused - Not implemented
	};

	/**
	 * Get resource image ID (0 if unused)
	 * @param enuType image place identifier
	 * @return image ID
	 */
	virtual UINT GetImageId(IN ImagePlace enuType) const	{ return 0; }

	/**
	 * Get IDE icon ID
	 * @return icon ID
	 */
	virtual unsigned int GetIconId(void) const = 0;

	/**
	 * Get IDE window title template
	 * @return window title template
	 */
	virtual const char* GetWndTitle(void) const = 0;

public:

	/**
	 * Load images
	 * @param hInstance instance of appliaction
	 * @return boolean result (false if error)
	 */
	bool LoadImages(IN HINSTANCE hInstance);

	/**
	 * Get image image ID (0 if unused)
	 * @param enuType image place identifier
	 * @return pointer to instance of image class
	 */
	CImage*	GetImage(IN ImagePlace enuType);

private:
	//Class variables
	CImage	m_imgTop;		///< Top image
	CImage	m_imgBottom;	///< Bottom image
	CImage	m_imgLeft;		///< Left image
	//CImage	m_imgRight;		//Not implemented

};


/*!
  Implementation of imitated Microsoft IDE
*/
class CIDEMS : public CIDEImitate
{
public:
	COLORREF GetColorByBlockType(IN BlockType enuType) const;
};


/*!
  Implementation of imitated Microsoft Visual C++ 6 (98) IDE
*/
class CIDEVC6 : public CIDEMS
{
public:
	UINT GetImageId(IN ImagePlace enuType) const;
	unsigned int GetIconId(void) const	{ return IDI_VC6; }
	const char* GetWndTitle(void) const	{ return "WorkImitate - Microsoft Visual C++ - [%s]"; }
};


/*!
  Implementation of imitated Microsoft Visual Studio 2008 IDE
*/
class CIDEVS9 : public CIDEMS
{
public:
	UINT GetImageId(IN ImagePlace enuType) const;
	unsigned int GetIconId(void) const			{ return IDI_VS9; }
	const char* GetWndTitle(void) const			{ return "WorkImitate - Microsoft Visual Studio [%s]"; }
	BlockType GetBlockType(IN const char* pszWord);
};


/*!
  Implementation of imitated Metrowerks CodeWarrior IDE
*/
class CIDECW : public CIDEImitate
{
public:
	UINT GetImageId(IN ImagePlace enuType) const;
	unsigned int GetIconId(void) const			{ return IDI_CW; }
	const char* GetWndTitle(void) const			{ return "Metrowerks CodeWarrior - [%s]"; }
	COLORREF GetColorByBlockType(IN BlockType enuType) const;
	BlockType GetBlockType(IN const char* pszWord);
};
