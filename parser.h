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
#include <vector>
#include <string>

using namespace std;


//! Token types
enum token_type {
	tok_normal,		///< Usual (normal) text
	tok_macros,		///< Macros
	tok_comment,	///< Commentary
	tok_quoted,		///< Quoted string
	tok_reserved,	///< Reserved C/C++/C#/Java words
	tok_directive,	///< Compiler directive
	tok_class,		///< Classes and namespases names
	tok_function	///< Function name (call)
};


class parser
{
public:
	parser();

	/**
	 * Initialize parser
	 */
	void intialize();

	/**
	 * Step to next symbol
	 * \return false if EOF (new file loaded)
	 */
	bool step();

	/**
	 * Get current symbol
	 * \return current symbol
	 */
	wchar_t current_symbol() const;

	/**
	 * Get current token
	 * \return current token
	 */
	token_type current_token() const;

private:
	/**
	 * Get file list
	 * \param path path to folder
	 */
	void get_file_list(const wchar_t* path);

	/**
	 * Load next file to parser
	 */
	void load_next_file();

	/**
	 * Load file content
	 * \param file_name file name
	 */
	void load_file(const wchar_t* file_name);

	/**
	 * Parse next token
	 */
	void parse_next_token();

private:
	vector<wstring>	_file_list;			///< List of source files to show in imitated IDE
	wstring			_content;			///< Current file content
	token_type		_curr_tok;			///< Current token type
	size_t			_position;			///< Current position in file
	size_t			_next_tok_pos;		///< Start position of a next token
};
