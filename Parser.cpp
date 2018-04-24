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

/*! \file Parser.cpp

    Implementation of the CParser class
*/

#include "StdAfx.h"
#include "Parser.h"
#include "Settings.h"
#include "ReservedWords.h"


CParser::CParser(void)
: m_enuBlockType(BTEOF), m_nCurrentPos(0), m_nEndBlockPos(0)
{
}


CParser::~CParser(void)
{
}


void CParser::Reset(void)
{
	m_nCurrentPos = 0;
	m_nEndBlockPos = 0;
	m_enuBlockType = BTEOF;
	m_strFileContent.erase();
}


bool CParser::LoadContent(IN LPCWSTR lpszFileName)
{
	Reset();

	HANDLE hFile = CreateFile(lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	BY_HANDLE_FILE_INFORMATION fi;
	GetFileInformationByHandle(hFile, &fi);
	char* pData = new char[fi.nFileSizeLow + 1];

	DWORD dwNumberOfBytesRead;
	ReadFile(hFile, pData, fi.nFileSizeLow, &dwNumberOfBytesRead, NULL);
	pData[fi.nFileSizeLow] = 0;
	CloseHandle(hFile);

	//Replace all '\r' sumbols with space
	char* pRPos;
	while ((pRPos = (char*)strchr((const char*)pData, '\r')) != NULL)
		*pRPos = ' ';

	m_strFileContent = pData;
	delete[] pData;
	return true;
}


void CParser::SetContent(IN const char* pszContent)
{
	Reset();
	m_strFileContent = pszContent;
}


COLORREF CParser::GetColorByBlockType(IN BlockType enuType) const
{
	static bool fUseEnhColor = CSettings().EnhColor;
	switch (enuType) {
		case BTNormal:		return RGB(0, 0, 0);
		case BTQuotedStr:	return fUseEnhColor ? RGB(120, 120, 120) : RGB(128, 0, 0);
		case BTMacros:		return fUseEnhColor ? RGB(128, 0, 128) : RGB(0, 0, 0);
		case BTComment:		return RGB(0, 128, 0);
		case BTCPPReserved:
		case BTDirective:	return RGB(0, 0, 255);
		case BTClassName:	return fUseEnhColor ? RGB(0, 0, 255) : RGB(0, 0, 0);
		case BTFuncName:	return fUseEnhColor ? RGB(163, 21, 21) : RGB(0, 0, 0);
	}
	return RGB(0, 0, 0);
}


CParser::BlockType CParser::GetBlockType(IN const char* pszWord)
{
	//Reserved CPP words
	int i;
	for (i = 0; i < (sizeof(g_CPPReservedWord) / sizeof(g_CPPReservedWord[0])); i++) {
		if (strcmp(pszWord, g_CPPReservedWord[i]) == 0)
			return BTCPPReserved;
	}

	//CPP directives
	for (i = 0; i < (sizeof(g_CPPPreprocessor) / sizeof(g_CPPPreprocessor[0])); i++) {
		if (strcmp(pszWord, g_CPPPreprocessor[i]) == 0)
			return BTDirective;
	}

	return BTNormal;
}


void CParser::DefineNextBlock(void)
{
	//Skip unreadable symbols
	while (m_strFileContent[m_nEndBlockPos] == '\n' ||
		   m_strFileContent[m_nEndBlockPos] == '\t' ||
		   m_strFileContent[m_nEndBlockPos] == ' ') {
		if (++m_nEndBlockPos == m_strFileContent.size())
			return;
	}

	//Comments
	if (m_strFileContent[m_nEndBlockPos] == '/' && m_nEndBlockPos + 1 < m_strFileContent.size() && m_strFileContent[m_nEndBlockPos + 1] == '*') {
		m_enuBlockType = BTComment;
		size_t nPos = m_strFileContent.find("*/", m_nEndBlockPos + 2);
		m_nEndBlockPos = (nPos == string::npos ? m_strFileContent.size() : nPos + 2);
	}
	else if (m_strFileContent[m_nEndBlockPos + 1] == '/' && m_nEndBlockPos + 1 < m_strFileContent.size() && m_strFileContent[m_nEndBlockPos + 1] == '/') {
		m_enuBlockType = BTComment;
		size_t nPos = m_strFileContent.find('\n', m_nEndBlockPos + 2);
		m_nEndBlockPos = (nPos == string::npos ? m_strFileContent.size() : nPos + 1);
	}
	//Quoted string
	else if (m_strFileContent[m_nEndBlockPos] == '"') {
		m_enuBlockType = BTQuotedStr;
		m_nEndBlockPos = m_strFileContent.find('"', m_nEndBlockPos + 1) + 1;
	}
	else if (m_strFileContent[m_nEndBlockPos] == '\'') {
		m_enuBlockType = BTQuotedStr;
		m_nEndBlockPos = m_strFileContent.find('\'', m_nEndBlockPos + 1) + 1;
	}
	//All another
	else {
		size_t nEndWordPos = strcspn(m_strFileContent.c_str() + m_nEndBlockPos, " ();:.,<>\"'=/+-*[]~!%^&|\n\t");
		if (nEndWordPos != string::npos) {
			string strCurrentWord = m_strFileContent.substr(m_nEndBlockPos, nEndWordPos ? nEndWordPos : 1);
			//Check for reserved words
			m_enuBlockType = GetBlockType(strCurrentWord.c_str());
			if (m_enuBlockType == BTNormal) {	//Not defined
				//Check if it is function name (call)
				size_t nFxNameCheck = m_nEndBlockPos + nEndWordPos;
				while (m_strFileContent[nFxNameCheck] == ' ' || m_strFileContent[nFxNameCheck] == '\t')	//Skip spaces
					nFxNameCheck++;

				if (m_strFileContent[nFxNameCheck] == '(' && (_istalpha(m_strFileContent[m_nEndBlockPos]) || m_strFileContent[m_nEndBlockPos] == '_')) {
					m_enuBlockType = BTFuncName;
					m_nEndBlockPos += strCurrentWord.length();
				}
				else
					m_nEndBlockPos += (nEndWordPos ? nEndWordPos : 1);
			}
			else
				m_nEndBlockPos += (nEndWordPos ? nEndWordPos : 1);
		}
	}
}


CParser::BlockType CParser::GetNextSymbol(OUT char* pSymbol)
{
	if (m_nCurrentPos == m_strFileContent.size())
		return BTEOF;

	if (m_nCurrentPos == m_nEndBlockPos)
		DefineNextBlock();

	assert(m_nCurrentPos < m_nEndBlockPos);

	*pSymbol = m_strFileContent[m_nCurrentPos++];
	return m_enuBlockType;
}
