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


/*!
  Parsing source CPP file
*/
class CParser
{
public:
	//! Default constructor
	CParser(void);
	//! Default destructor
	virtual ~CParser(void);

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
	 * @param pszFileName a file name
	 * @return boolean result (true if file processed)
	 */
	bool LoadContent(IN LPCWSTR lpszFileName);

	/**
	 * Set dummy content
	 * @param pszContent a dummy content
	 */
	void SetContent(IN const char* pszContent);

	/**
	 * Get next symbol and block type from content
	 * @param pSymbol reference to save next symbol
	 * @return block type of next symbol
	 */
	virtual BlockType GetNextSymbol(OUT char* pSymbol);

	/**
	 * Get color for defined block type
	 * @param enuType a block type
	 * @return color for referred above type
	 */
	virtual COLORREF GetColorByBlockType(IN BlockType enuType) const;


protected:
	/**
	 * Get block type for reffered word
	 * @param pszWord next word in content
	 * @return block type for next word
	 */
	virtual BlockType GetBlockType(IN const char* pszWord);

	/**
	 * Define next block
	 */
	virtual void DefineNextBlock(void);


private:
	/**
	 * Reset internal state
	 */
	void Reset(void);


private:
	//Class variables
	string		m_strFileContent;	///< File content
	BlockType	m_enuBlockType;		///< Current block type
	size_t		m_nCurrentPos;		///< Current position in file
	size_t		m_nEndBlockPos;		///< Last position in current block
};
