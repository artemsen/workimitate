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

#include "parser.h"
#include "settings.h"
#include "resource.h"
#include <algorithm>
#include <assert.h>

//! C++ 11 reserved words
const wchar_t* _reserved[] = {
	L"alignas",
	L"alignof",
	L"and",
	L"and_eq",
	L"asm",
	L"auto",
	L"bitand",
	L"bitor",
	L"bool",
	L"break",
	L"case",
	L"catch",
	L"char",
	L"char16_t",
	L"char32_t",
	L"class",
	L"compl",
	L"const",
	L"constexpr",
	L"const_cast",
	L"continue",
	L"decltype",
	L"default",
	L"delete",
	L"do",
	L"double",
	L"dynamic_cast",
	L"else",
	L"enum",
	L"explicit",
	L"export",
	L"extern",
	L"false",
	L"float",
	L"for",
	L"friend",
	L"goto",
	L"if",
	L"inline",
	L"int",
	L"long",
	L"mutable",
	L"namespace",
	L"new",
	L"noexcept",
	L"not",
	L"not_eq",
	L"nullptr",
	L"operator",
	L"or",
	L"or_eq",
	L"private",
	L"protected",
	L"public",
	L"register",
	L"reinterpret_cast",
	L"return",
	L"short",
	L"signed",
	L"sizeof",
	L"static",
	L"static_assert",
	L"static_cast",
	L"struct",
	L"switch",
	L"template",
	L"this",
	L"thread_local",
	L"throw",
	L"true",
	L"try",
	L"typedef",
	L"typeid",
	L"typename",
	L"union",
	L"unsigned",
	L"using",
	L"virtual",
	L"void",
	L"volatile",
	L"wchar_t",
	L"while",
	L"xor",
	L"xor_eq"
};

//! C++ stl classes and types
const wchar_t* _stl[] = {
	L"string", L"wstring",
	L"queue", L"deque", L"stack",
	L"list", L"vector",
	L"map", L"multimap",
	L"set", L"multiset",
	L"iterator", L"const_iterator", L"reverse_iterator", L"const_reverse_iterator",
	L"difference_type", L"value_type",
	L"size_t", L"size_type"
};

//! C++ preprocessor directives
const wchar_t* _preprocessor[] = {
	L"__TIMESTAMP__", L"__TIME__", L"__DATE__",
	L"__FILE__", L"__LINE__", L"__FUNCTION__", L"__STDC__",
	L"__cplusplus", L"lib", L"comment", L"defined"
};

//! C++ win32 defined types
const wchar_t* _win_types[] = {
 L"APIENTRY", L"ATOM", L"BOOL", L"BOOLEAN", L"BYTE", L"CALLBACK", L"CCHAR", L"CHAR", L"COLORREF", L"CONST",
 L"DWORD", L"DWORD_PTR", L"DWORD32", L"DWORD64", L"DWORDLONG", L"FLOAT", L"HACCEL", L"HALF_PTR", L"HANDLE",
 L"HBITMAP", L"HBRUSH", L"HCOLORSPACE", L"HCONV", L"HCONVLIST", L"HCURSOR", L"HDC", L"HDDEDATA", L"HDESK",
 L"HDROP", L"HDWP", L"HENHMETAFILE", L"HFILE", L"HFONT", L"HGDIOBJ", L"HGLOBAL", L"HHOOK", L"HICON",
 L"HINSTANCE", L"HKEY", L"HKL", L"HLOCAL", L"HMENU", L"HMETAFILE", L"HMODULE", L"HMONITOR", L"HPALETTE",
 L"HPEN", L"HRESULT", L"HRGN", L"HRSRC", L"HSZ", L"HWINSTA", L"HWND", L"INT", L"INT_PTR", L"INT16", L"INT32",
 L"INT64", L"INT8", L"LANGID", L"LCID", L"LCTYPE", L"LGRPID", L"LONG", L"LONG_PTR", L"LONG32", L"LONG64",
 L"LONGLONG", L"LPARAM", L"LPBOOL", L"LPBYTE", L"LPCOLORREF", L"LPCSTR", L"LPCTSTR", L"LPCVOID", L"LPCWSTR",
 L"LPDWORD", L"LPHANDLE", L"LPINT", L"LPLONG", L"LPSTR", L"LPTSTR", L"LPVOID", L"LPWORD", L"LPWSTR", L"LRESULT",
 L"PBOOL", L"PBOOLEAN", L"PBYTE", L"PCHAR", L"PCSTR", L"PCTSTR", L"PCWSTR", L"PDWORD", L"PDWORD_PTR",
 L"PDWORD32", L"PDWORD64", L"PDWORDLONG", L"PFLOAT", L"PHALF_PTR", L"PHANDLE", L"PHKEY", L"PINT", L"PINT_PTR",
 L"PINT16", L"PINT32", L"PINT64", L"PINT8", L"PLCID", L"PLONG", L"PLONG_PTR", L"PLONG32", L"PLONG64",
 L"PLONGLONG", L"POINTER_32", L"POINTER_64", L"POINTER_SIGNED", L"POINTER_UNSIGNED", L"PSHORT", L"PSIZE_T",
 L"PSSIZE_T", L"PSTR", L"PTBYTE", L"PTCHAR", L"PTSTR", L"PUCHAR", L"PUHALF_PTR", L"PUINT", L"PUINT_PTR",
 L"PUINT16", L"PUINT32", L"PUINT64", L"PUINT8", L"PULONG", L"PULONG_PTR", L"PULONG32", L"PULONG64",
 L"PULONGLONG", L"PUSHORT", L"PVOID", L"PWCHAR", L"PWORD", L"PWSTR", L"QWORD", L"SC_HANDLE", L"SC_LOCK",
 L"SERVICE_STATUS_HANDLE", L"SHORT", L"SIZE_T", L"SSIZE_T", L"TBYTE", L"TCHAR", L"UCHAR", L"UHALF_PTR",
 L"UINT", L"UINT_PTR", L"UINT16", L"UINT32", L"UINT64", L"UINT8", L"ULONG", L"ULONG_PTR", L"ULONG32", L"ULONG64",
 L"ULONGLONG", L"UNICODE_STRING", L"USHORT", L"USN", L"VOID", L"WCHAR", L"WINAPI", L"WORD", L"WPARAM"
};


parser::parser()
:	_position(0),
	_next_tok_pos(0)
{
}


void parser::intialize()
{
	const wchar_t* path = settings::instance().sources_path();
	if (path && *path) {
		srand(GetTickCount());
		get_file_list(path);
	}
}


bool parser::step()
{
	++_position;
	const bool rc = (_position < _content.length());
	if (!rc)	//Load next file
		load_next_file();

	if (!rc || _position == _next_tok_pos)
		parse_next_token();

	return rc;
}


wchar_t parser::current_symbol() const
{
	assert(_position < _content.length());
	return _content[_position];
}


token_type parser::current_token() const
{
	assert(_position < _content.length());
	return _curr_tok;
}


void parser::get_file_list(const wchar_t* path)
{
	assert(path && *path);

	const settings& sett = settings::instance();
	const set<wstring>& ext_filter = sett.extenstion_filter();

	wstring find_path = path;
	if (!find_path.empty() && *find_path.rbegin() != L'\\' && *find_path.rbegin() != L'/')
		find_path += L'\\';
	find_path += L'*';

	WIN32_FIND_DATA find_file_data;
	HANDLE find_handle = FindFirstFile(find_path.c_str(), &find_file_data);
	if (find_handle != INVALID_HANDLE_VALUE) {
		do {
			wstring full_path = path;
			if (!full_path.empty() && *full_path.rbegin() != L'\\' && *full_path.rbegin() != L'/')
				full_path += L'\\';
			full_path += find_file_data.cFileName;

			if (find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if (sett.recursive_search() &&
					lstrcmp(find_file_data.cFileName, L".") != 0 &&
					lstrcmp(find_file_data.cFileName, L"..") != 0)
						get_file_list(full_path.c_str());
			}
			else {
				//Check for file size
				if (find_file_data.nFileSizeLow > 16 && !find_file_data.nFileSizeHigh) {	//Ignore too big or too small files
					//Check file extension mask
					const size_t ext_pos = full_path.rfind('.');
					if (ext_pos != string::npos) {
						wstring ext = full_path.substr(ext_pos + 1);
						transform(ext.begin(), ext.end(), ext.begin(), tolower);
						if (ext_filter.find(ext) != ext_filter.end())
							_file_list.push_back(full_path);
					}
				}
			}
		}
		while (FindNextFile(find_handle, &find_file_data));

		FindClose(find_handle);
	}
}


void parser::load_next_file()
{
	_next_tok_pos = _position = 0;
	_content.clear();

	if (!_file_list.empty()) {
		int try_count = static_cast<int>(_file_list.size());
		while (--try_count >= 0 && _content.empty()) {
			const size_t index = rand() % _file_list.size();
			const wstring& file_name = _file_list[index];
			load_file(file_name.c_str());
		}
	}

	if (_content.empty()) {
		HRSRC res = FindResource(NULL, MAKEINTRESOURCE(IDR_SAMPLE_CODE), L"TEXT");
		if (res) {
			DWORD res_sz = SizeofResource(NULL, res);
			if (res_sz) {
				const void* res_data = LockResource(LoadResource(NULL, res));
				if (res_data) {
					_content.resize(res_sz);
					MultiByteToWideChar(CP_ACP, 0, reinterpret_cast<LPCSTR>(res_data), res_sz, &_content.front(), _content.size() * sizeof(wchar_t));
				}
			}
			FreeResource(res);
		}
	}

	if (_content.empty())
		_content = L"Error";
}


void parser::load_file(const wchar_t* file_name)
{
	assert(file_name && *file_name);

	HANDLE file = CreateFile(file_name, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE)
		return;

	LARGE_INTEGER file_size;
	if (!GetFileSizeEx(file, &file_size) || file_size.QuadPart < 16) {
		CloseHandle(file);
		return;
	}

	//Read file data
	string buff(static_cast<size_t>(file_size.QuadPart), 0);
	DWORD read_bytes;
	if (!ReadFile(file, &buff.front(), buff.size(), &read_bytes, NULL)) {
		CloseHandle(file);
		return;
	}
	CloseHandle(file);

	//Check for BOM signature
	if (static_cast<unsigned char>(buff[0]) == 0xff && static_cast<unsigned char>(buff[1]) == 0xfe)	//Unicode LE file
		_content = reinterpret_cast<const wchar_t*>(buff.c_str() + 2);
	else if (static_cast<unsigned char>(buff[0]) == 0xef && static_cast<unsigned char>(buff[1]) == 0xbb && static_cast<unsigned char>(buff[2]) == 0xbf)	{	//UTF-8
		_content.resize(buff.size() - 3);
		MultiByteToWideChar(CP_UTF8, 0, buff.c_str() + 3, buff.length() - 3, &_content.front(), _content.size() * sizeof(wchar_t));
	}
	else {	//ANSI
		_content.resize(buff.size());
		MultiByteToWideChar(CP_ACP, 0, buff.c_str(), buff.length(), &_content.front(), _content.size() * sizeof(wchar_t));
	}

	//Remove all '\r' symbols
	size_t pos = 0;
	while ((pos = _content.find('\r', pos)) != string::npos)
		_content.erase(pos, 1);
}


void parser::parse_next_token()
{
	//Skip unreadable symbols (blanks)
	while (_next_tok_pos < _content.length() && _content[_next_tok_pos] <= ' ')
		++_next_tok_pos;
	if (_next_tok_pos >= _content.length())
		return;	//EOF

	_curr_tok = tok_normal;	//Default

	//Multiple string comment
	if (_content[_next_tok_pos] == '/' && _next_tok_pos + 1 < _content.length() && _content[_next_tok_pos + 1] == '*') {
		_curr_tok = tok_comment;
		const size_t pos = _content.find(L"*/", _next_tok_pos + 2);
		_next_tok_pos = (pos == string::npos ? _content.length() : pos + 2);
	}

	//Single string comment
	else if (_content[_next_tok_pos] == '/' && _next_tok_pos + 1 < _content.length() && _content[_next_tok_pos + 1] == '/') {
		_curr_tok = tok_comment;
		const size_t pos = _content.find('\n', _next_tok_pos + 2);
		_next_tok_pos = (pos == string::npos ? _content.length() : pos + 1);
	}

	//Preprocessor directives
	else if (_content[_next_tok_pos] == '#') {
		_curr_tok = tok_directive;
		const size_t pos = _content.find_first_of(L" \n\t", _next_tok_pos + 1);
		_next_tok_pos = (pos == string::npos ? _content.length() : pos + 1);
	}

	//Quoted string
	else if (_content[_next_tok_pos] == '\"' || _content[_next_tok_pos] == '\'') {
		_curr_tok = tok_quoted;
		const bool single_quoted = _content[_next_tok_pos] == '\'';
		size_t end_pos = _next_tok_pos;
		while (++end_pos < _content.length()) {
			if (_content[end_pos] == '\\')
				++end_pos;
			else if (_content[end_pos] == (single_quoted ? '\'' : '\"')) {
				++end_pos;
				break;
			}
		}
		_next_tok_pos = end_pos;
	}
	//All another
	else {
		size_t end_word_pos = _content.find_first_of(L" ();:.,<>\"`'=/+-*[]{}~!?@%^&|\\\r\n\t", _next_tok_pos);
		if (end_word_pos == string::npos)
			end_word_pos = _content.size();
		if (end_word_pos == _next_tok_pos)
			++end_word_pos;	//Special symbol

		const wstring current_word = _content.substr(_next_tok_pos, end_word_pos - _next_tok_pos);

		//Check for reserved C++ words
		for (size_t i = 0; _curr_tok == tok_normal && i < sizeof(_reserved) / sizeof(_reserved[0]); ++i) {
			if (current_word.compare(_reserved[i]) == 0)
				_curr_tok = tok_reserved;
		}

		//Check for STL classes and types
		for (size_t i = 0; _curr_tok == tok_normal && i < sizeof(_stl) / sizeof(_stl[0]); ++i) {
			if (current_word.compare(_stl[i]) == 0)
				_curr_tok = tok_class;
		}

		//Check for ñ++ preprocessor directives
		for (size_t i = 0; _curr_tok == tok_normal && i < sizeof(_preprocessor) / sizeof(_preprocessor[0]); ++i) {
			if (current_word.compare(_preprocessor[i]) == 0)
				_curr_tok = tok_directive;
		}

		//Check for win32 defined types
		for (size_t i = 0; _curr_tok == tok_normal && i < sizeof(_win_types) / sizeof(_win_types[0]); ++i) {
			if (current_word.compare(_win_types[i]) == 0)
				_curr_tok = tok_class;
		}

		//Check for class/namespace name
		if (_curr_tok == tok_normal) {
			size_t name_check = end_word_pos;
			while (name_check < _content.length() && _content[name_check] <= ' ')
				++name_check;
			if (name_check + 1 < _content.length() && _content[name_check] == ':' && _content[name_check + 1] == ':')
				_curr_tok = tok_class;
		}

		//Check for macro name
		if (_curr_tok == tok_normal && current_word.length() > 1 && !isdigit(current_word[0])) {
			size_t check = 0;
			while (check < current_word.length() && (current_word[check] == '_' || isdigit(current_word[check]) || isupper(current_word[check])))
				++check;
			if (check == current_word.length())
				_curr_tok = tok_macros;
		}

		//Check for function name (call)
		if (_curr_tok == tok_normal) {
			size_t fx_name_check = end_word_pos;
			while (fx_name_check < _content.length() && _content[fx_name_check] <= ' ')
				++fx_name_check;
			if (fx_name_check < _content.length() && _content[fx_name_check] == '(' && (isalpha(current_word[0]) || current_word[0] == '_'))
				_curr_tok = tok_function;
		}

		_next_tok_pos = end_word_pos;
	}
}
