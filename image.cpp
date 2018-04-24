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

#include "image.h"


image::image()
:	_image(NULL),
	_buffer(NULL)
{
}


image::~image()
{
	delete _image;
	if (_buffer) {
		GlobalUnlock(_buffer);
		GlobalFree(_buffer);
	}
}


bool image::load(const UINT_PTR id)
{
	HRSRC res = FindResource(NULL, MAKEINTRESOURCE(id), L"PNG");
	if (!res)
		return false;

	DWORD res_sz = SizeofResource(NULL, res);
	if (!res_sz)
		return false;

	const void* res_data = LockResource(LoadResource(NULL, res));
	if (!res_data)
		return false;

	_buffer = GlobalAlloc(GMEM_MOVEABLE, res_sz);
	if (!_buffer)
		return false;

	LPVOID buff_ptr = GlobalLock(_buffer);
	if (!buff_ptr)
		return false;
	CopyMemory(buff_ptr, res_data, res_sz);
	IStream* inp_stream = NULL;
	if (CreateStreamOnHGlobal(_buffer, FALSE, &inp_stream) == S_OK) {
		_image = Gdiplus::Bitmap::FromStream(inp_stream);
		inp_stream->Release();
	}
	if (_image && _image->GetLastStatus() != Gdiplus::Ok) {
		delete _image;
		_image = NULL;
	}

	return (_image != NULL);
}
