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

/*!
    Implementation of the CIDEImitate class inheritances
*/

#include "Common.h"
#include "IDEImitate.h"
#include "Settings.h"


//MS Macros
static const char* g_MSVCMacro[] = {
	"assert",
	"__in", "__out", "__inout", "__in_opt", "__out_opt", "__inout_opt", "__in_ecount", "__out_ecount", "__inout_ecount", "__in_bcount", "__out_bcount", "__inout_bcount", "__out_xcount", "__deref_opt_out", "__deref_out",
	"__volatile",
};

//MS CPP directives/macros/typedefs
static const char* g_MSVCDirectives[] = {
	"PCSTR", "PSTR", "PCTSTR", "PTSTR", "PCWSTR", "PWSTR", "LPCSTR", "LPSTR", "LPCTSTR", "LPTSTR", "LPCWSTR", "LPWSTR",
	"WORD", "PWORD", "DWORD", "PDWORD", "DWORD32", "PDWORD32", "DWORD64", "PDWORD64",
	"INT", "PINT", "INT32", "PINT32", "INT64", "PINT64", "UINT", "PUINT", "UINT32", "PUINT32", "UINT64", "PUINT64",
	"LONG", "PLONG", "LONG32", "PLONG32", "LONG64", "PLONG64", "ULONG", "PULONG", "ULONG32", "PULONG32", "ULONG64", "PULONG64",
	"CHAR", "PCHAR", "WCHAR", "PWCHAR", "UCHAR", "PUCHAR",
	"VOID", "PVOID",
	"SHORT", "PSHORT", "USHORT", "PUSHORT",
	"BOOL", "PBOOL", "BOOLEAN", "PBOOLEAN", "boolean",
	"HANDLE", "PHANDLE", "HINSTANCE", "HMODULE", "HWND",
	"FILE", "BSTR", "LOGICAL", "SYSTEMTIME",	
	"COLORREF",
	"SOCKET", "WSADATA",
	"NTSTATUS",
	"UNICODE_STRING", "PUNICODE_STRING",
	"_DEBUG",
	"KSPIN_LOCK", "KIRQL",
	"NPAGED_LOOKASIDE_LIST", "LIST_ENTRY", "PLIST_ENTRY",
	"_WIN64", "_WIN32",
};


bool CIDEImitate::LoadImages(const HINSTANCE hinst)
{
	UINT_PTR topID =    GetImageId(IPTop);
	UINT_PTR bottomID = GetImageId(IPBottom);
	UINT_PTR leftID =   GetImageId(IPLeft);

	return (
		(topID ? _ImageTop.Load(hinst, topID) : true) &&
		(bottomID ? _ImageBottom.Load(hinst, bottomID) : true) &&
		(leftID ? _ImageLeft.Load(hinst, leftID) : true)
		);
}


CImage* CIDEImitate::GetImage(const ImagePlace type)
{
	switch (type) {
		case IPTop:		return _ImageTop.IsLoaded() ? &_ImageTop : NULL;
		case IPBottom:	return _ImageBottom.IsLoaded() ? &_ImageBottom : NULL;
		case IPLeft:	return _ImageLeft.IsLoaded() ? &_ImageLeft : NULL;
		default:
			break;
	}
	return NULL;
}


UINT_PTR CIDEVC6::GetImageId(const ImagePlace type) const
{
	switch (type) {
		case IPTop:		return IDB_VC6TOP;
		case IPBottom:	return IDB_VC6BOTTOM;
		case IPLeft:	return IDB_VC6LEFT;
		default:
			break;
	}
	return 0;
}


COLORREF CIDEMS::GetColorByBlockType(const BlockType type) const
{
	switch (type) {
		case BTNormal:		return RGB(0, 0, 0);
		case BTQuotedStr:	return RGB(140, 0, 0);
		case BTMacros:		return RGB(160, 0, 160);
		case BTComment:		return RGB(0, 128, 0);
		case BTCPPReserved:
		case BTDirective:	return RGB(0, 0, 255);
		case BTClassName:	return RGB(0, 0, 255);
		case BTFuncName:	return RGB(140, 0, 0);
		default:
			break;
	}
	return CParser::GetColorByBlockType(type);
}


UINT_PTR CIDEVS9::GetImageId(const ImagePlace type) const
{
	switch (type) {
		case IPTop:		return IDB_VS9TOP;
		case IPBottom:	return IDB_VS9BOTTOM;
		case IPLeft:	return IDB_VS9LEFT;
		default:
			break;
	}
	return 0;
}


CParser::BlockType CIDEVS9::GetBlockType(const char* word)
{
	if (!strlen(word))
		return CParser::GetBlockType(word);

	//MS Macros
	if (Contain(word, g_MSVCMacro, sizeof(g_MSVCMacro) / sizeof(g_MSVCMacro[0])))
		return BTMacros;

	//MS CPP directives and macros
	if (Contain(word, g_MSVCDirectives, sizeof(g_MSVCDirectives) / sizeof(g_MSVCDirectives[0])))
		return BTDirective;

	return CParser::GetBlockType(word);
}



UINT_PTR CIDECW::GetImageId(const ImagePlace type) const
{
	switch (type) {
		case IPTop:		return IDB_CWTOP;
		case IPBottom:	return IDB_CWBOTTOM;
		case IPLeft:	return IDB_CWLEFT;
		default:
			break;
	}
	return 0;
}


COLORREF CIDECW::GetColorByBlockType(const BlockType type) const
{
	switch (type) {
		case BTNormal:		return RGB(0, 0, 0);
		case BTMacros:
		case BTClassName:
		case BTFuncName:	return RGB(76, 115, 166);
		case BTComment:		return RGB(179, 0, 0);
		case BTQuotedStr:	return RGB(102, 102, 102);
		case BTDirective:
		case BTCPPReserved:	return RGB(0, 0, 179);
		default:
			break;
	}
	return CParser::GetColorByBlockType(type);
}


CParser::BlockType CIDECW::GetBlockType(const char* word)
{
	if (!strlen(word))
		return CParser::GetBlockType(word);

	//All words that start with Q,T,C or R - is a class
	if (word[0] == 'Q' || word[0] == 'T' || word[0] == 'C' || word[0] == 'R')
		return BTClassName;
	//All words that start with _ - is a macros
	if (word[0] == '_')
		return BTMacros;

	return CParser::GetBlockType(word);
}


UINT_PTR CIDECarbide::GetImageId(const ImagePlace type) const
{
	switch (type) {
		case IPTop:		return IDB_CCTOP;
		case IPBottom:	return IDB_CCBOTTOM;
		case IPLeft:	return IDB_CCLEFT;
		default:
			break;
	}
	return 0;
}
