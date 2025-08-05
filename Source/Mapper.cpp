// ---------------------------------------------------------------------------------------------------------------------------------
//  __  __                                                  
// |  \/  |                                                 
// | \  / | __ _ _ __  _ __   ___ _ __      ___ _ __  _ __  
// | |\/| |/ _` | '_ \| '_ \ / _ \ '__|    / __| '_ \| '_ \ 
// | |  | | (_| | |_) | |_) |  __/ |    _ | (__| |_) | |_) |
// |_|  |_|\__,_| .__/| .__/ \___|_|   (_) \___| .__/| .__/ 
//              | |   | |                      | |   | |    
//              |_|   |_|                      |_|   |_|    
//
// Mapper (STL map extension class)
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 12/29/2000 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2000, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "3Dme.h"
#include "Mapper.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

bool	Mapper::exist(const wstring &title) const
{
	// Find the title

	MapperMap::const_iterator pos = _data.find(title);
	if (pos == _data.end()) return false;
	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

const	wstring	&Mapper::getString(const wstring &title) const
{
	// Find the title & return the value

	MapperMap::const_iterator pos = _data.find(title);
	if (pos == _data.end()) return empty;
	return pos->second;
}

// ---------------------------------------------------------------------------------------------------------------------------------

const	wstring	&Mapper::getString(const wstring &title, const wstring &def) const
{
	if (!exist(title)) return def;
	return getString(title);
}

// ---------------------------------------------------------------------------------------------------------------------------------

const	int	Mapper::getInteger(const wstring &title) const
{
	// Get the value from the title

	const wstring	&value = getString(title);

	// If not found, return "blank" value

	if (!value.length()) return 0;

	// Is it hexidecimal?

	if (value[0] == _T('0') && value[1] == _T('x'))
	{
		int	temp = 0;
		swscanf(value.substr(2).c_str(), _T("%x"), &temp);
		return temp;
	}

	// The value

	return (int) _wtol(value.c_str());
}

// ---------------------------------------------------------------------------------------------------------------------------------

const	int	Mapper::getInteger(const wstring &title, const int def) const
{
	if (!exist(title)) return def;
	return getInteger(title);
}

// ---------------------------------------------------------------------------------------------------------------------------------

const	float	Mapper::getFloat(const wstring &title) const
{
	// Get the value from the title

	const wstring	&value = getString(title);

	// If not found, return "blank" value

	if (!value.length()) return 0.0f;

	// The value

	return (float) _wtof(value.c_str());
}

// ---------------------------------------------------------------------------------------------------------------------------------

const	float	Mapper::getFloat(const wstring &title, const float def) const
{
	if (!exist(title)) return def;
	return getFloat(title);
}

// ---------------------------------------------------------------------------------------------------------------------------------
// Mapper.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
