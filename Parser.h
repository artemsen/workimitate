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


/*!
  Parsing source CPP file
*/
class CParser
{
public:
	//! Default constructor
	CParser();
	//! Default destructor
	virtual ~CParser();

	//! Block types
	enum BlockType {
		BTEOF = 0,		///< End of file
		BTNormal,		///< Usual (normal) text
		BTMacros,		///< Macros
		BTComment,		///< Commentary
		BTQuotedStr,	///< Quoted string
		BTCPPReserved,	///< Reserved C++ words
		BTDirective,	///< Compiler directive
		BTClassName,	///< Classes and namespases
		BTFuncName		///< Function name (call)
	};

	/**
	 * Load content from file
	 * \param fileName a file name
	 * \return boolean result (true if file processed)
	 */
	bool LoadContent(const wchar_t* fileName);

	/**
	 * Set dummy content
	 * \param content a content
	 */
	void SetContent(const char* content);

	/**
	 * Get next symbol and block type from content
	 * \param symbol reference to save next symbol
	 * \return block type of next symbol
	 */
	virtual BlockType GetNextSymbol(char* symbol);

	/**
	 * Get color for defined block type
	 * \param enuType a block type
	 * \return color for referred above type
	 */
	virtual COLORREF GetColorByBlockType(const BlockType type) const;


protected:
	/**
	 * Check for inclide word in list
	 * \param word checked word
	 * \param checkList list
	 * \param listSize list size
	 * \return true if contain
	 */
	bool Contain(const char* word, const char* checkList[], const size_t listSize) const;

	/**
	 * Get block type for reffered word
	 * \param word next word in content
	 * \return block type for next word
	 */
	virtual BlockType GetBlockType(const char* word);

	/**
	 * Define next block
	 */
	virtual void DefineNextBlock();


private:
	/**
	 * Reset internal state
	 */
	void Reset();

private:
	//Class variables
	string		_Content;		///< File content
	BlockType	_BlockType;		///< Current block type
	size_t		_CurrentPos;	///< Current position in file
	size_t		_EndBlockPos;	///< Last position in current block
};
