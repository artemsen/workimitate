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

    Implementation of the CParser class
*/

#include "Common.h"
#include "Parser.h"
#include "Settings.h"

//! CPP reserved words
static const char* g_CPPReservedWord[] = {
	"and"
	"and_eq",
	"asm",
	"auto",
	"bad_cast",
	"bad_typeid",
	"bitand",
	"bitor", 
	"bool",
	"break",
	"case",
	"catch",
	"char",
	"class",
	"compl",
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
	"not",
	"not_eq",
	"operator",
	"or", 
	"or_eq",
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
	"wchar_t",
	"while",
	"xor",
	"xor_eq",
};


//! CPP STL types
static const char* g_CPPSTL[] = {
	"string", "wstring",	
	"queue", "deque", "stack",
	"list", "vector",
	"map", "multimap",
	"set", "multiset",
	"iterator", "const_iterator", "reverse_iterator", "const_reverse_iterator",
	"difference_type", "value_type",
	"size_t", "size_type",
};

//! CPP preprocessor directives
static const char* g_CPPPreprocessor[] = {
	"__TIME__", "__DATE__", "__FILE__", "__LINE__", "__FUNCTION__",
	"__cplusplus",
	"#define", "#undef",
	"#ifdef", "#ifndef",
	"#if", "#elif", "#else", "#endif",
	"#error",
	"#include",
	"#line",
	"#pragma",
	"lib",
	"comment",
	"defined",
};


CParser::CParser()
: _BlockType(BTEOF), _CurrentPos(0), _EndBlockPos(0)
{
}


CParser::~CParser()
{
}


void CParser::Reset()
{
	_CurrentPos = 0;
	_EndBlockPos = 0;
	_BlockType = BTEOF;
	_Content.erase();
}


bool CParser::LoadContent(const wchar_t* fileName)
{
	HANDLE file = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE)
		return false;

	BY_HANDLE_FILE_INFORMATION fi;
	GetFileInformationByHandle(file, &fi);
	string content(static_cast<size_t>(fi.nFileSizeLow), 0);

	DWORD numberOfBytesRead;
	ReadFile(file, &content[0], fi.nFileSizeLow, &numberOfBytesRead, NULL);
	CloseHandle(file);

	SetContent(content.c_str());

	return true;
}


void CParser::SetContent(const char* content)
{
	Reset();
	_Content = content;

	//Remove all '\r' sumbols
	size_t pos = 0;
	while ((pos = _Content.find('\r', pos)) != string::npos)
		_Content.erase(pos, 1);
}


COLORREF CParser::GetColorByBlockType(const BlockType type) const
{
	switch (type) {
		case BTNormal:		return RGB(0, 0, 0);
		case BTQuotedStr:	return RGB(128, 0, 0);
		case BTMacros:		return RGB(128, 0, 128);
		case BTComment:		return RGB(0, 128, 0);
		case BTCPPReserved:
		case BTDirective:	return RGB(0, 0, 255);
		case BTClassName:	return RGB(0, 0, 255);
		case BTFuncName:	return RGB(163, 21, 21);
		default:
			break;
	}
	return RGB(0, 0, 0);
}


CParser::BlockType CParser::GetBlockType(const char* word)
{
	//Reserved CPP words
	if (Contain(word, g_CPPReservedWord, sizeof(g_CPPReservedWord) / sizeof(g_CPPReservedWord[0])))
		return BTCPPReserved;

	//CPP STL
	if (Contain(word, g_CPPSTL, sizeof(g_CPPSTL) / sizeof(g_CPPSTL[0])))
		return BTCPPReserved;

	//CPP directives
	if (Contain(word, g_CPPPreprocessor, sizeof(g_CPPPreprocessor) / sizeof(g_CPPPreprocessor[0])))
		return BTDirective;

	return BTNormal;
}


bool CParser::Contain(const char* word, const char* checkList[], const size_t listSize) const
{
	for (size_t i = 0; i < listSize; ++i) {
		if (strcmp(word, checkList[i]) == 0)
			return true;
	}
	return false;
}


void CParser::DefineNextBlock()
{
	//Default
	_BlockType = BTNormal;

	//Skip unreadable symbols (blanks)
	while (_EndBlockPos < _Content.size() && (_Content[_EndBlockPos] == '\r' || _Content[_EndBlockPos] == '\n' || _Content[_EndBlockPos] == '\t' || _Content[_EndBlockPos] == ' '))
		 ++_EndBlockPos;

	if (_EndBlockPos == _Content.size())
		return;	//EOF

	//Multiple string comment
	if (_Content[_EndBlockPos] == '/' && _EndBlockPos + 1 < _Content.size() && _Content[_EndBlockPos + 1] == '*') {
		_BlockType = BTComment;
		const size_t pos = _Content.find("*/", _EndBlockPos + 2);
		_EndBlockPos = (pos == string::npos ? _Content.size() : pos + 2);
	}
	//Single string comment
	else if (_Content[_EndBlockPos + 1] == '/' && _EndBlockPos + 1 < _Content.size() && _Content[_EndBlockPos + 1] == '/') {
		_BlockType = BTComment;
		const size_t pos = _Content.find('\n', _EndBlockPos + 2);
		_EndBlockPos = (pos == string::npos ? _Content.size() : pos + 1);
	}
	//Quoted string
	else if (_Content[_EndBlockPos] == '\"' || _Content[_EndBlockPos] == '\'') {
		_BlockType = BTQuotedStr;
		const bool singleQuoted = _Content[_EndBlockPos] == '\'';
		size_t endPos = _EndBlockPos + 1;
		while (endPos < _Content.size()) {
			endPos = _Content.find(singleQuoted ? '\'' : '\"', endPos);
			if (endPos == string::npos)
				endPos = _Content.size();
			else {
				if (_Content[endPos - 1] != '\\' || (_Content[endPos - 1] == '\\' && _Content[endPos - 2] == '\\')) {
					++endPos;
					break;
				}
			}
		}
		_EndBlockPos = endPos;
	}
	//All another
	else {
		size_t endWordPos = _Content.find_first_of(" ();:.,<>\"`'=/+-*[]{}~!?@%^&|\r\n\t", _EndBlockPos);
		if (endWordPos == string::npos)
			endWordPos = _Content.size();
		if (endWordPos == _EndBlockPos)
			++endWordPos;	//Special symbol

		const string currentWord = _Content.substr(_EndBlockPos, endWordPos - _EndBlockPos);

		//Check for reserved words
		_BlockType = GetBlockType(currentWord.c_str());

		//Not defined yet? Check if it is a class/namespace name
		if (_BlockType == BTNormal) {
			size_t nameCheck = endWordPos;
			while (nameCheck < _Content.size() && (_Content[nameCheck] == '\r' || _Content[nameCheck] == '\n' || _Content[nameCheck] == '\t' || _Content[nameCheck] == ' '))
				++nameCheck;
			if (_Content[nameCheck] == ':' && _Content[nameCheck + 1] == ':')
				_BlockType = BTClassName;			
		}

		//Not defined yet? Check if it is a macro name
		if (_BlockType == BTNormal && currentWord.length() > 1 && !isdigit(currentWord[0])) {
			size_t check = 0;
			while (check < currentWord.length() && (currentWord[check] == '_' || isdigit(currentWord[check]) || isupper(currentWord[check])))
				++check;
			if (check == currentWord.length())
				_BlockType = BTMacros;
		}

		//Not defined yet? Check if it is a function name (call)
		if (_BlockType == BTNormal) {
			size_t fxNameCheck = endWordPos;
			while (fxNameCheck < _Content.size() && (_Content[fxNameCheck] == '\r' || _Content[fxNameCheck] == '\n' || _Content[fxNameCheck] == '\t' || _Content[fxNameCheck] == ' '))
				++fxNameCheck;
			if (_Content[fxNameCheck] == '(' && (isalpha(currentWord[0]) || currentWord[0] == '_'))
				_BlockType = BTFuncName;
		}

		_EndBlockPos = endWordPos;
	}
}


CParser::BlockType CParser::GetNextSymbol(char* symbol)
{
	if (_CurrentPos == _Content.size())
		return BTEOF;

	if (_CurrentPos == _EndBlockPos)
		DefineNextBlock();

	assert(_CurrentPos < _EndBlockPos);

	*symbol = _Content[_CurrentPos++];
	return _BlockType;
}
