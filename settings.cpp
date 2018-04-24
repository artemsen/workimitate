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

#include "settings.h"
#include "resource.h"
#include <Shlobj.h>
#include <algorithm>

//Paths to registry to save/load settings
static const wchar_t* _reg_settings =  L"SOFTWARE\\WorkImitate";
static const wchar_t* _reg_path =      L"Path";
static const wchar_t* _reg_recursive = L"Recursive";
static const wchar_t* _reg_ext_flt =   L"Extensions";
static const wchar_t* _reg_ide =       L"IDE";
static const wchar_t* _reg_speed =     L"Speed";


settings::settings()
:	_recursive(true),
	_ide(ide_vs11),
	_speed(50)
{
	_ext_filter.insert(L"h");
	_ext_filter.insert(L"hpp");
	_ext_filter.insert(L"hrh");
	_ext_filter.insert(L"c");
	_ext_filter.insert(L"cpp");
	_ext_filter.insert(L"cxx");
	_ext_filter.insert(L"java");
	_ext_filter.insert(L"cs");
}


settings& settings::instance()
{
	static settings inst;
	return inst;
}


void settings::load()
{
	HKEY reg_key;
	DWORD result = RegOpenKeyEx(HKEY_CURRENT_USER, _reg_settings, 0, KEY_READ, &reg_key);
	if (result == ERROR_SUCCESS) {
		DWORD data_num;
		DWORD data_len;

		wstring path(1024, 0);
		data_len = static_cast<DWORD>(path.size());
		if (RegQueryValueEx(reg_key, _reg_path, NULL, NULL, reinterpret_cast<LPBYTE>(&path.front()), &data_len) == ERROR_SUCCESS)
			_path = path.c_str();

		data_num = 0;
		data_len = sizeof(data_num);
		if (RegQueryValueEx(reg_key, _reg_recursive, NULL, NULL, reinterpret_cast<LPBYTE>(&data_num), &data_len) == ERROR_SUCCESS)
			_recursive = (data_num != 0);

		wstring ext_filter(1024, 0);
		data_len = ext_filter.size();
		if (RegQueryValueEx(reg_key, _reg_ext_flt, NULL, NULL, reinterpret_cast<LPBYTE>(&ext_filter.front()), &data_len) == ERROR_SUCCESS) {
			ext_filter.resize(data_len);
			if (!ext_filter.empty())
				filter_from_string(ext_filter);
		}

		data_num = 0;
		data_len = sizeof(data_num);
		if (RegQueryValueEx(reg_key, _reg_ide, NULL, NULL, reinterpret_cast<LPBYTE>(&data_num), &data_len) == ERROR_SUCCESS)
			_ide = static_cast<ide_type>(data_num);

		data_num = 0;
		data_len = sizeof(data_num);
		if (RegQueryValueEx(reg_key, _reg_speed, NULL, NULL, reinterpret_cast<LPBYTE>(&data_num), &data_len) == ERROR_SUCCESS)
			_speed = data_num;

		RegCloseKey(reg_key);
	}
}


void settings::save() const
{
	HKEY reg_key;
	const DWORD result = RegCreateKeyEx(HKEY_CURRENT_USER, _reg_settings, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &reg_key, NULL);
	if (result == ERROR_SUCCESS) {
		RegSetValueEx(reg_key, _reg_path, 0, REG_SZ, reinterpret_cast<const BYTE*>(_path.empty() ? NULL : &_path.front()), _path.length() * sizeof(wchar_t));

		DWORD data = _recursive ? 1 : 0;
		RegSetValueEx(reg_key, _reg_recursive, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&data), sizeof(data));

		const wstring ext_filter = filter_to_string();
		RegSetValueEx(reg_key, _reg_ext_flt, 0, REG_SZ, reinterpret_cast<const BYTE*>(ext_filter.empty() ? NULL : &ext_filter.front()), ext_filter.length() * sizeof(wchar_t));

		data = _ide;
		RegSetValueEx(reg_key, _reg_ide, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&data), sizeof(data));

		data = _speed;
		RegSetValueEx(reg_key, _reg_speed, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&data), sizeof(data));

		RegCloseKey(reg_key);
	}
}


int settings::configure(const HWND wnd_parent)
{
	return static_cast<int>(DialogBox(NULL, MAKEINTRESOURCE(IDD_CONFIGURE), wnd_parent, &settings::dlg_proc));
}


wstring settings::filter_to_string() const
{
	wstring ext_filter;
	for (set<wstring>::const_iterator it = _ext_filter.begin(); it != _ext_filter.end(); ++it) {
		if (!ext_filter.empty())
			ext_filter += L"; ";
		ext_filter += *it;
	}
	return ext_filter;
}


void settings::filter_from_string(const wstring& filter)
{
	_ext_filter.clear();
	const wchar_t* delimiters = L" ;,";
	size_t last_pos = filter.find_first_not_of(delimiters, 0);
	size_t next_pos = filter.find_first_of(delimiters, last_pos);
	while (next_pos != string::npos || last_pos != string::npos) {
		wstring ext = filter.substr(last_pos, next_pos - last_pos);
		transform(ext.begin(), ext.end(), ext.begin(), tolower);
		_ext_filter.insert(ext);
		last_pos = filter.find_first_not_of(delimiters, next_pos);
		next_pos = filter.find_first_of(delimiters, last_pos);
	}
}


INT_PTR CALLBACK settings::dlg_proc(HWND wnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
	switch (msg) {
		case WM_INITDIALOG: {
			const settings& inst = settings::instance();
			const wstring ext_filter = inst.filter_to_string();

			SetDlgItemText(wnd, IDC_PATH_EDIT, inst.sources_path());
			SetDlgItemText(wnd, IDC_EXTFILTER_EDIT, ext_filter.c_str());
			SendDlgItemMessage(wnd, IDC_RECURSIVE_CHECK, BM_SETCHECK, inst.recursive_search() ? BST_CHECKED : BST_UNCHECKED, 0);

			LRESULT added_id;
			added_id = SendDlgItemMessage(wnd, IDC_IDETYPE_COMBO, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"Microsoft Visual Studio 11"));
			SendDlgItemMessage(wnd, IDC_IDETYPE_COMBO, CB_SETITEMDATA, added_id, ide_vs11);
			added_id = SendDlgItemMessage(wnd, IDC_IDETYPE_COMBO, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"Microsoft Visual Studio 2008"));
			SendDlgItemMessage(wnd, IDC_IDETYPE_COMBO, CB_SETITEMDATA, added_id, ide_vs9);
			added_id = SendDlgItemMessage(wnd, IDC_IDETYPE_COMBO, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"Microsoft Visual C++ 6"));
			SendDlgItemMessage(wnd, IDC_IDETYPE_COMBO, CB_SETITEMDATA, added_id, ide_vc6);
			added_id = SendDlgItemMessage(wnd, IDC_IDETYPE_COMBO, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"Metrowerks CodeWarrior"));
			SendDlgItemMessage(wnd, IDC_IDETYPE_COMBO, CB_SETITEMDATA, added_id, ide_cw);
			added_id = SendDlgItemMessage(wnd, IDC_IDETYPE_COMBO, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"Eclipse"));
			SendDlgItemMessage(wnd, IDC_IDETYPE_COMBO, CB_SETITEMDATA, added_id, ide_eclipse);
			added_id = SendDlgItemMessage(wnd, IDC_IDETYPE_COMBO, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"IntelliJ IDEA"));
			SendDlgItemMessage(wnd, IDC_IDETYPE_COMBO, CB_SETITEMDATA, added_id, ide_idea);

			SendDlgItemMessage(wnd, IDC_IDETYPE_COMBO, CB_SETCURSEL, inst.imitated_ide_type(), 0);
			SendMessage(wnd, WM_COMMAND, MAKELONG(IDC_IDETYPE_COMBO, CBN_SELENDOK), 0);

			SendDlgItemMessage(wnd, IDC_TYPINGSPEED_SLIDER, TBM_SETRANGE, TRUE, MAKELPARAM(10, 200));
			SendDlgItemMessage(wnd, IDC_TYPINGSPEED_SLIDER, TBM_SETPOS, TRUE, inst.typing_speed());

			static HICON icon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MAIN));
			SendMessage(wnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(icon));
		}
		return TRUE;

		case WM_COMMAND:
			switch (LOWORD(w_param)) {
				case IDOK: {
					settings& inst = settings::instance();

					inst._path.resize(SendDlgItemMessage(wnd, IDC_PATH_EDIT, WM_GETTEXTLENGTH, 0, 0) + 1);
					GetDlgItemText(wnd, IDC_PATH_EDIT, &inst._path.front(), static_cast<int>(inst._path.size()));

					inst._recursive = (SendDlgItemMessage(wnd, IDC_RECURSIVE_CHECK, BM_GETCHECK, 0, 0) == BST_CHECKED);

					wstring ext_filter(SendDlgItemMessage(wnd, IDC_EXTFILTER_EDIT, WM_GETTEXTLENGTH, 0, 0) + 1, 0);
					GetDlgItemText(wnd, IDC_EXTFILTER_EDIT, &ext_filter.front(), static_cast<int>(ext_filter.size()));
					if (!ext_filter.empty())
						inst.filter_from_string(ext_filter);

					LRESULT curr_item = SendDlgItemMessage(wnd, IDC_IDETYPE_COMBO, CB_GETCURSEL, 0, 0);
					inst._ide = static_cast<ide_type>(SendDlgItemMessage(wnd, IDC_IDETYPE_COMBO, CB_GETITEMDATA, static_cast<WPARAM>(curr_item), 0));

					inst._speed = static_cast<UINT>(SendDlgItemMessage(wnd, IDC_TYPINGSPEED_SLIDER, TBM_GETPOS, 0, 0));

					inst.save();
					EndDialog(wnd, LOWORD(w_param));
					}
					return TRUE;

				case IDCANCEL:
					EndDialog(wnd, LOWORD(w_param));
					return TRUE;

				case IDC_BROWSE_BUTTON: {
					wchar_t path[MAX_PATH];
					BROWSEINFO bi;
					ZeroMemory (&bi, sizeof(bi));
					bi.hwndOwner = wnd;
					bi.ulFlags = BIF_RETURNONLYFSDIRS;
					bi.lpszTitle = L"Please, select folder with source files (cpp, h, hpp, ...etc)";
					bi.lpfn = reinterpret_cast<BFFCALLBACK>(&bff_callback);
					bi.lParam = reinterpret_cast<LPARAM>(wnd);
					LPITEMIDLIST item_list = SHBrowseForFolder(&bi);
					if (SHGetPathFromIDList(item_list, path))
						SetDlgItemText(wnd, IDC_PATH_EDIT, path);
				}
				return TRUE;
		}
		break;
	}

	return FALSE;
}


int CALLBACK settings::bff_callback(HWND wnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
	if (msg == BFFM_INITIALIZED) {
		//Set initial path
		wchar_t path[MAX_PATH];
		GetDlgItemText(reinterpret_cast<HWND>(l_param), IDC_PATH_EDIT, path, sizeof(path) / sizeof(path[0]));
		SendMessage(wnd, BFFM_SETSELECTION, TRUE, reinterpret_cast<LPARAM>(path));
	}
	return 0;
}
