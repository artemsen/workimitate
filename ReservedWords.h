/**************************************************************************
 *  WorkImitate screensaver (http://workimitate.sourceforge.net)          *
 *  Copyright (C) 2007-2008 by Artem A. Senichev <artemsen@gmail.com>     *
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

/*! \file ReservedWords.h

    Contain lists with reserved C++ words
*/


//! CPP reserved words
static const char* g_CPPReservedWord[] = {
	"auto",
	"bad_cast",
	"bad_typeid",
	"bool",
	"break",
	"case",
	"catch",
	"char",
	"class",
	"const",
	"const_cast",
	"continue",
	"default",
	"delete",
	"do",
	"double",
	"dynamic_cast",
	"else",
	"enum",
	"except",
	"explicit",
	"extern",
	"false",
	"finally",
	"float",
	"for",
	"friend",
	"goto",
	"if",
	"inline",
	"int",
	"long",
	"mutable",
	"namespace",
	"new",
	"operator",
	"private",
	"protected",
	"public",
	"register",
	"reinterpret_cast",
	"return",
	"short",
	"signed",
	"sizeof",
	"static",
	"static_cast",
	"struct",
	"switch",
	"template",
	"this",
	"throw",
	"true",
	"try",
	"type_info",
	"typedef",
	"typeid",
	"typename",
	"union",
	"unsigned",
	"using",
	"virtual",
	"void",
	"volatile",
	"while"
};

//! CPP preprocessor directives
static const char* g_CPPPreprocessor[] = {
	"#pragma",
	"#include",
	"#define",
	"#if",
	"defined",
	"#ifdef",
	"#ifndef",
	"#else",
	"#elif",
	"#endif",
	"comment",
	"__FILE__",
	"__FUNCTION__",
	"__LINE__"
};
