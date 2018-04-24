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
#include "parser.h"
#include "resource.h"


class ide_imitate
{
public:
	//! Image place identifier
	enum image_place {
		/*
		  +--------------------------------+
		  | tl            tm            tr |
		  | lt                          rt |
		  |                                |
		  | lm                          rm |
		  |                                |
		  | lb                          rb |
		  | bl            bm            br |
          +--------------------------------+
		*/

		ip_tl,
		ip_tm,
		ip_tr,

		ip_bl,
		ip_bm,
		ip_br,

		ip_lt,
		ip_lm,
		ip_lb,

		ip_rt,
		ip_rm,
		ip_rb,

		ip_last
	};

	/**
	 * Get IDE window title
	 * \return window title
	 */
	virtual const wchar_t* title() const = 0;

	/**
	 * Get icon ID
	 * \return icon ID
	 */
	virtual UINT_PTR icon_id() const = 0;

	/**
	 * Get resource image resource ID
	 * \param place image place identifier
	 * \return image resource ID (0 if unused)
	 */
	virtual UINT_PTR image_resource_id(const image_place place) const = 0;

	/**
	 * Get color for token type
	 * \param tok a token type
	 * \return color for referred above type
	 */
	virtual COLORREF color_for_token(const token_type tok) const = 0;
};


/*
  Implementation of imitated Microsoft Visual Studio 11 IDE
*/
class ide_vs11_imitate : public ide_imitate
{
public:
	//From ide_imitate
	const wchar_t* title() const	{ return L"WorkImitate - Microsoft Visual Studio"; }

	//From ide_imitate
	UINT_PTR icon_id() const		{ return IDI_IVS11; }

	//From ide_imitate
	UINT_PTR image_resource_id(const image_place place) const
	{
		switch (place) {
			case ip_tl: return IDB_IVS11_TL;
			case ip_tm: return IDB_IVS11_TM;
			case ip_tr: return IDB_IVS11_TR;
			case ip_bl: return IDB_IVS11_BL;
			case ip_bm: return IDB_IVS11_BM;
			case ip_br: return IDB_IVS11_BR;
			case ip_lt: return IDB_IVS11_LT;
			case ip_lm: return IDB_IVS11_LM;
			case ip_lb: return IDB_IVS11_LB;
			case ip_rm: return IDB_IVS11_RM;
		}
		return 0;
	}

	//From ide_imitate
	COLORREF color_for_token(const token_type tok) const
	{
		switch (tok) {
			case tok_normal:	return RGB(0, 0, 0);
			case tok_macros:	return RGB(128, 0, 128);
			case tok_comment:	return RGB(0, 128, 0);
			case tok_quoted:	return RGB(128, 0, 0);
			case tok_reserved:
			case tok_directive:	return RGB(0, 0, 255);
			case tok_class:		return RGB(0, 0, 255);
			case tok_function:	return RGB(163, 21, 21);
		}
		return RGB(0, 0, 0);
	}
};


/*
  Implementation of imitated Microsoft Visual Studio 9 IDE
*/
class ide_vs9_imitate : public ide_imitate
{
public:
	//From ide_imitate
	const wchar_t* title() const	{ return L"WorkImitate - Microsoft Visual Studio"; }

	//From ide_imitate
	UINT_PTR icon_id() const		{ return IDI_IVS9; }

	//From ide_imitate
	UINT_PTR image_resource_id(const image_place place) const
	{
		switch (place) {
			case ip_tl: return IDB_IVS9_TL;
			case ip_tm: return IDB_IVS9_TM;
			case ip_tr: return IDB_IVS9_TR;
			case ip_bl: return IDB_IVS9_BL;
			case ip_bm: return IDB_IVS9_BM;
			case ip_br: return IDB_IVS9_BR;
			case ip_lt: return IDB_IVS9_LT;
			case ip_lm: return IDB_IVS9_LM;
			case ip_rm: return IDB_IVS9_RM;
		}
		return 0;
	}

	//From ide_imitate
	COLORREF color_for_token(const token_type tok) const
	{
		switch (tok) {
			case tok_normal:	return RGB(0, 0, 0);
			case tok_macros:	return RGB(128, 0, 128);
			case tok_comment:	return RGB(0, 128, 0);
			case tok_quoted:	return RGB(128, 0, 0);
			case tok_reserved:
			case tok_directive:	return RGB(0, 0, 255);
			case tok_class:		return RGB(0, 0, 255);
			case tok_function:	return RGB(163, 21, 21);
		}
		return RGB(0, 0, 0);
	}
};


/*
  Implementation of imitated Microsoft Visual C++ 6 IDE
*/
class ide_vc6_imitate : public ide_imitate
{
public:
	//From ide_imitate
	const wchar_t* title() const	{ return L"WorkImitate - Microsoft Visual C++"; }

	//From ide_imitate
	UINT_PTR icon_id() const		{ return IDI_IVC6; }

	//From ide_imitate
	UINT_PTR image_resource_id(const image_place place) const
	{
		switch (place) {
			case ip_tl: return IDB_IVC6_TL;
			case ip_tm: return IDB_IVC6_TM;
			case ip_tr: return IDB_IVC6_TR;
			case ip_bl: return IDB_IVC6_BL;
			case ip_bm: return IDB_IVC6_BM;
			case ip_br: return IDB_IVC6_BR;
			case ip_lt: return IDB_IVC6_LT;
			case ip_lm: return IDB_IVC6_LM;
			case ip_lb: return IDB_IVC6_LB;
			case ip_rm: return IDB_IVC6_RM;
		}
		return 0;
	}

	//From ide_imitate
	COLORREF color_for_token(const token_type tok) const
	{
		switch (tok) {
			case tok_normal:	return RGB(0, 0, 0);
			case tok_macros:	return RGB(128, 0, 128);
			case tok_comment:	return RGB(0, 128, 0);
			case tok_quoted:	return RGB(128, 0, 0);
			case tok_reserved:
			case tok_directive:	return RGB(0, 0, 255);
			case tok_class:		return RGB(0, 0, 255);
			case tok_function:	return RGB(0, 0, 0);
		}
		return RGB(0, 0, 0);
	}
};


/*
  Implementation of imitated CodeWarrior IDE
*/
class ide_cw_imitate : public ide_imitate
{
public:
	//From ide_imitate
	const wchar_t* title() const	{ return L"Metrowerks CodeWarrior - WorkImitate"; }

	//From ide_imitate
	UINT_PTR icon_id() const		{ return IDI_ICW; }

	//From ide_imitate
	UINT_PTR image_resource_id(const image_place place) const
	{
		switch (place) {
			case ip_tl: return IDB_ICW_TL;
			case ip_tm: return IDB_ICW_TM;
			case ip_bl: return IDB_ICW_BL;
			case ip_bm: return IDB_ICW_BM;
			case ip_br: return IDB_ICW_BR;
			case ip_lt: return IDB_ICW_LT;
			case ip_lm: return IDB_ICW_LM;
			case ip_lb: return IDB_ICW_LB;
			case ip_rm: return IDB_ICW_RM;
		}
		return 0;
	}

	//From ide_imitate
	COLORREF color_for_token(const token_type tok) const
	{
		switch (tok) {
			case tok_normal:	return RGB(0, 0, 0);
			case tok_macros:	return RGB(76, 115, 166);
			case tok_comment:	return RGB(179, 0, 0);
			case tok_quoted:	return RGB(102, 102, 102);
			case tok_reserved:
			case tok_directive:	return RGB(0, 0, 179);
			case tok_class:		return RGB(76, 115, 166);
			case tok_function:	return RGB(76, 115, 166);
		}
		return RGB(0, 0, 0);
	}
};


/*
  Implementation of imitated Eclipse IDE
*/
class ide_eclipse_imitate : public ide_imitate
{
public:
	//From ide_imitate
	const wchar_t* title() const	{ return L"Java - WorkImitate/src/main.java - Eclipse"; }

	//From ide_imitate
	UINT_PTR icon_id() const		{ return IDI_IEC; }

	//From ide_imitate
	UINT_PTR image_resource_id(const image_place place) const
	{
		switch (place) {
			case ip_tl: return IDB_IEC_TL;
			case ip_tm: return IDB_IEC_TM;
			case ip_tr: return IDB_IEC_TR;
			case ip_bl: return IDB_IEC_BL;
			case ip_bm: return IDB_IEC_BM;
			case ip_br: return IDB_IEC_BR;
			case ip_lt: return IDB_IEC_LT;
			case ip_lm: return IDB_IEC_LM;
			case ip_rm: return IDB_IEC_RM;
		}
		return 0;
	}

	//From ide_imitate
	COLORREF color_for_token(const token_type tok) const
	{
		switch (tok) {
			case tok_normal:	return RGB(0, 0, 0);
			case tok_macros:	return RGB(0, 0, 200);
			case tok_comment:	return RGB(0, 128, 0);
			case tok_quoted:	return RGB(0, 0, 255);
			case tok_reserved:
			case tok_directive:	return RGB(128, 0, 128);
			case tok_class:		return RGB(0, 0, 0);
			case tok_function:	return RGB(0, 0, 0);
		}
		return RGB(0, 0, 0);
	}
};


/*
  Implementation of imitated IntelliJ Idea IDE
*/
class ide_idea_imitate : public ide_imitate
{
public:
	//From ide_imitate
	const wchar_t* title() const	{ return L"WorkImitate - [WorkImitate/src/main.java] - IntelliJ IDEA"; }

	//From ide_imitate
	UINT_PTR icon_id() const		{ return IDI_IID; }

	//From ide_imitate
	UINT_PTR image_resource_id(const image_place place) const
	{
		switch (place) {
			case ip_tl: return IDB_IID_TL;
			case ip_tm: return IDB_IID_TM;
			case ip_tr: return IDB_IID_TR;
			case ip_bl: return IDB_IID_BL;
			case ip_bm: return IDB_IID_BM;
			case ip_br: return IDB_IID_BR;
			case ip_lt: return IDB_IID_LT;
			case ip_lm: return IDB_IID_LM;
			case ip_rm: return IDB_IID_RM;
		}
		return 0;
	}

	//From ide_imitate
	COLORREF color_for_token(const token_type tok) const
	{
		switch (tok) {
			case tok_normal:	return RGB(0, 0, 0);
			case tok_macros:	return RGB(102, 14, 122);
			case tok_comment:	return RGB(127, 127, 150);
			case tok_quoted:	return RGB(0, 127, 0);
			case tok_reserved:
			case tok_directive:	return RGB(0, 0, 128);
			case tok_class:		return RGB(0, 0, 0);
			case tok_function:	return RGB(0, 0, 0);
		}
		return RGB(0, 0, 0);
	}
};
