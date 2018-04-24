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

#include "Image.h"

#pragma pack (push, 1)
//! Targa header
struct IMAGE_TGA_HEADER {
	unsigned char	Length;
	unsigned char	ColormapType;
	unsigned char	ImageType;
	unsigned short	ColormapIndex;
	unsigned short	ColormapLength;
	unsigned char	ColormapEntrySize;
	unsigned short	OriginX;
	unsigned short	OriginY;
	unsigned short	Width;
	unsigned short	Height;
	unsigned char	PixelSize;
	unsigned char	ImageDesc;
};
#define IMAGE_TGA_RLE		0x0A	//RLE compressed TGA
#pragma pack (pop)


CImage::CImage()
:	_ImgWidth(0), _ImgHeight(0)
{
}


CImage::~CImage()
{
	DeleteObject(_Bitmap);
}


bool CImage::Load(const HINSTANCE hinst, const UINT_PTR resId)
{
	//Find resource in resource file
	HRSRC res = FindResource(hinst, MAKEINTRESOURCE(resId), L"IMAGE");
	if (!res)
		return false;

	//Load resource into memory
	const BYTE* resData = reinterpret_cast<const BYTE*>(LoadResource(hinst, res));
	if (!resData)
		return false;

	const IMAGE_TGA_HEADER* header = reinterpret_cast<const IMAGE_TGA_HEADER*>(resData);
	resData += sizeof(IMAGE_TGA_HEADER);

	assert(header->ColormapType == 0);	//Colormap not supported
	assert(header->ImageType == IMAGE_TGA_RLE);

	//Read the width and height
	_ImgWidth  = header->Width;
	_ImgHeight = header->Height;
	assert(_ImgWidth != 0 && _ImgHeight != 0 && _ImgWidth < 4096 && _ImgHeight < 4096);	//Undefined image size

	//Calculate the number of bytes per pixel
	const size_t bpp = header->PixelSize / 8;
	assert(bpp == 4);

	//Calculate the size
	const size_t sizeInBytes = _ImgWidth * _ImgHeight * bpp;

	//Read the data
	vector<unsigned char> imgData;
	imgData.resize(sizeInBytes);
	size_t bufPos = 0;
	while (bufPos < _ImgWidth * _ImgHeight * bpp) {
		unsigned char packetHeader = *resData;			
		resData += sizeof(unsigned char);
		const unsigned char packetLength = (packetHeader & 0x7F) + 1;
		if ((packetHeader & 0x80) != 0) {	// Run-length encoding packet (RLE)
			const unsigned char* packet = reinterpret_cast<const unsigned char*>(resData);
			resData += bpp;
			for (unsigned int j = 0; j < packetLength * bpp; ++j) {
				assert(bufPos <= sizeInBytes);	//Incorrect format
				imgData[bufPos++] = packet[j % bpp];
			}
		}
		else {									//Raw packet
			for (size_t j = 0; j < packetLength * bpp; ++j) {
				assert(bufPos <= sizeInBytes);	//Incorrect format
				imgData[bufPos++] = *resData;
				resData += sizeof(unsigned char);
			}
		}
	}

	_Bitmap = CreateBitmap(static_cast<int>(_ImgWidth), static_cast<int>(_ImgHeight), 1, 32, &imgData[0]);

	FreeResource(res);
	return true;
}


void CImage::Draw(const HDC dc, const int x, const int y, const int width /*= 0*/, const int height /*= 0*/)
{
	HDC dcMem = CreateCompatibleDC(dc);
	HBITMAP hbmOld = static_cast<HBITMAP>(SelectObject(dcMem, _Bitmap));

	BITMAP bm;
	GetObject(_Bitmap, sizeof(bm), &bm);

	if (width == 0 || height == 0)
		BitBlt(dc, x, y, bm.bmWidth, bm.bmHeight, dcMem, 0, 0, SRCCOPY);
	else
		StretchBlt(dc, 0, 0, width, height, dcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

	SelectObject(dcMem, hbmOld);
	DeleteDC(dcMem);
}
