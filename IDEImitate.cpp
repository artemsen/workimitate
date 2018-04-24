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

/*! \file IDEImitate.cpp

    Implementation of the CIDEImitate class inheritances
*/

#include "StdAfx.h"
#include "IDEImitate.h"
#include "Settings.h"


bool CIDEImitate::LoadImages(IN HINSTANCE hInstance)
{
	UINT nTopID = GetImageId(IPTop);
	UINT nBottomID = GetImageId(IPBottom);
	UINT nLeftID = GetImageId(IPLeft);

	return (
		(nTopID ? m_imgTop.Load(hInstance, nTopID) : true) &&
		(nBottomID ? m_imgBottom.Load(hInstance, nBottomID) : true) &&
		(nLeftID ? m_imgLeft.Load(hInstance, nLeftID) : true)
		);
}


CImage*	CIDEImitate::GetImage(IN ImagePlace enuType)
{
	switch (enuType) {
		case IPTop:		return m_imgTop.IsLoaded() ? &m_imgTop : NULL;
		case IPBottom:	return m_imgBottom.IsLoaded() ? &m_imgBottom : NULL;
		case IPLeft:	return m_imgLeft.IsLoaded() ? &m_imgLeft : NULL;
	}
	return 0;
}


UINT CIDEVC6::GetImageId(IN ImagePlace enuType) const
{
	switch (enuType) {
		case IPTop:		return IDB_VC6TOP;
		case IPBottom:	return IDB_VC6BOTTOM;
		case IPLeft:	return IDB_VC6LEFT;
	}
	return 0;
}


COLORREF CIDEMS::GetColorByBlockType(IN BlockType enuType) const
{
	static bool fUseEnhColor = CSettings().EnhColor;
	switch (enuType) {
		case BTNormal:		return RGB(0, 0, 0);
		case BTQuotedStr:	return fUseEnhColor ? RGB(120, 120, 120) : RGB(0, 0, 0);
		case BTMacros:		return fUseEnhColor ? RGB(128, 0, 128) : RGB(0, 0, 0);
		case BTComment:		return RGB(0, 128, 0);
		case BTCPPReserved:
		case BTDirective:	return RGB(0, 0, 255);
		case BTClassName:	return fUseEnhColor ? RGB(0, 0, 255) : RGB(0, 0, 0);
		case BTFuncName:	return fUseEnhColor ? RGB(163, 21, 21) : RGB(0, 0, 0);
	}
	return CParser::GetColorByBlockType(enuType);
}


UINT CIDEVS9::GetImageId(IN ImagePlace enuType) const
{
	switch (enuType) {
		case IPTop:		return IDB_VS9TOP;
		case IPBottom:	return IDB_VS9BOTTOM;
		case IPLeft:	return IDB_VS9LEFT;
	}
	return 0;
}


CParser::BlockType CIDEVS9::GetBlockType(IN const char* pszWord)
{
	if (!strlen(pszWord))
		return CParser::GetBlockType(pszWord);

	int i;

	//MS Macros
	static const char* aMSVCMacro[] = {
		"IN",
		"OUT",
		"ASSERT",
		"ATLASSERT",
		"VERIFY",
		"TRACE",
		"UNICODE",
		"TRUE",
		"FALSE",
		"NULL",
		"_T",
		"RGB"};
	for (i = 0; i < (sizeof(aMSVCMacro) / sizeof(aMSVCMacro[0])); i++) {
		if (strcmp(pszWord, aMSVCMacro[i]) == 0)
			return BTDirective;
	}


	//MS CPP directives
	static const char* aMSVCDirectives[] = {
		"__FILE__",
		"__FUNCTION__",
		"__LINE__",
		"_DEBUG",
		"COLORREF",
		"BOOL",
		"HWND",
		"HINSTANCE",
		"HANDLE",
		"HMODULE",
		"LPTSTR",
		"LPCTSTR",
		"LPCSTR",
		"LPCWSTR"};
	for (i = 0; i < (sizeof(aMSVCDirectives) / sizeof(aMSVCDirectives[0])); i++) {
		if (strcmp(pszWord, aMSVCDirectives[i]) == 0)
			return BTDirective;
	}

	return CParser::GetBlockType(pszWord);
}



UINT CIDECW::GetImageId(IN ImagePlace enuType) const
{
	switch (enuType) {
		case IPTop:		return IDB_CWTOP;
		case IPBottom:	return IDB_CWBOTTOM;
		case IPLeft:	return IDB_CWLEFT;
	}
	return 0;
}


COLORREF CIDECW::GetColorByBlockType(IN BlockType enuType) const
{
	switch (enuType) {
		case BTNormal:		return RGB(0, 0, 0);
		case BTMacros:
		case BTClassName:
		case BTFuncName:	return RGB(76, 115, 166);
		case BTComment:		return RGB(179, 0, 0);
		case BTQuotedStr:	return RGB(102, 102, 102);
		case BTDirective:
		case BTCPPReserved:	return RGB(0, 0, 179);
	}
	return CParser::GetColorByBlockType(enuType);
}


CParser::BlockType CIDECW::GetBlockType(IN const char* pszWord)
{
	if (!strlen(pszWord))
		return CParser::GetBlockType(pszWord);

	//All words that start with Q,T,C or R - is a class
	if (pszWord[0] == 'Q' || pszWord[0] == 'T' || pszWord[0] == 'C' || pszWord[0] == 'R')
		return BTClassName;
	//All words that start with _ - is a macros
	if (pszWord[0] == '_')
		return BTMacros;

	return CParser::GetBlockType(pszWord);
}
