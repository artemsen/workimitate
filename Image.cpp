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

#include "StdAfx.h"
#include "Image.h"

#ifndef HIMETRIC_INCH
#define HIMETRIC_INCH	2540    //HIMETRIC units per inch
#endif


CImage::CImage()
: m_pPicture(NULL)
{

}


CImage::~CImage()
{

}


bool CImage::Load(IN HINSTANCE hInstance, IN UINT nIDRes)
{
	//Find resource in resource file
	HRSRC hRsrc = FindResource(hInstance, MAKEINTRESOURCE(nIDRes), "Images");
	if (!hRsrc)
		return false;

	//Load resource into memory
	DWORD dwLen = SizeofResource(hInstance, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hInstance, hRsrc);
	if (!lpRsrc)
		return false;

	HGLOBAL hGlobal = NULL;
	hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwLen);
	PVOID pvData = GlobalLock(hGlobal);
	ATLASSERT(pvData != NULL);
	memcpy(pvData, lpRsrc, dwLen);
	GlobalUnlock(hGlobal);

	CComPtr<IStream> pStream;
	
	HRESULT hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);
	if (SUCCEEDED(hr))
		hr = OleLoadPicture(pStream, 0, true, IID_IPicture, (void**)&m_pPicture);

	FreeResource(hRsrc);
	return SUCCEEDED(hr);
}


void CImage::Draw(IN HDC hDC, IN int nX, IN int nY, IN int nWidth /*= 0*/, IN int nHeight /*= 0*/)
{
	if (!m_pPicture)
		return;

	//Get the width and the height of the picture
	long hmWidth = 0, hmHeight = 0;
	m_pPicture->get_Width(&hmWidth);
	m_pPicture->get_Height(&hmHeight);

	//Convert himetric to pixels
	if (!nWidth)
		nWidth  = MulDiv(hmWidth, GetDeviceCaps(hDC, LOGPIXELSX), HIMETRIC_INCH);
	if (!nHeight)
		nHeight = MulDiv(hmHeight, GetDeviceCaps(hDC, LOGPIXELSY), HIMETRIC_INCH);

	//Display the picture using IPicture::Render
	m_pPicture->Render(hDC, nX, nY, nWidth, nHeight, 0, hmHeight, hmWidth, -hmHeight, NULL);
}


int CImage::GetHeight(IN HDC hDC) const
{
	if (!m_pPicture)
		return 0;
	//Get the height of the picture
	long hmHeight = 0;
	m_pPicture->get_Height(&hmHeight);
	//Convert himetric to pixels
	return MulDiv(hmHeight, GetDeviceCaps(hDC, LOGPIXELSY), HIMETRIC_INCH);
}


int CImage::GetWidth(IN HDC hDC) const
{
	if (!m_pPicture)
		return 0;
	//Get the width of the picture
	long hmWidth = 0;
	m_pPicture->get_Width(&hmWidth);
	//Convert himetric to pixels
	return MulDiv(hmWidth, GetDeviceCaps(hDC, LOGPIXELSX), HIMETRIC_INCH);
}
