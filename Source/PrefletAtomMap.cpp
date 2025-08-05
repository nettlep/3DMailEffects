// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _            _                   __  __                                  
// |  __ \           / _| |    | |     /\   | |                 |  \/  |                                 
// | |__) |_ __  ___| |_| | ___| |_   /  \  | |_  ___  _ __ ___ | \  / | __ _ _ __       ___ _ __  _ __  
// |  ___/| '__|/ _ \  _| |/ _ \ __| / /\ \ | __|/ _ \| '_ ` _ \| |\/| |/ _` | '_ \     / __| '_ \| '_ \ 
// | |    | |  |  __/ | | |  __/ |_ / ____ \| |_| (_) | | | | | | |  | | (_| | |_) | _ | (__| |_) | |_) |
// |_|    |_|   \___|_| |_|\___|\__/_/    \_\\__|\___/|_| |_| |_|_|  |_|\__,_| .__/ (_) \___| .__/| .__/ 
//                                                                           | |            | |   | |    
//                                                                           |_|            |_|   |_|    
//
//   Manages atoms stored in our various preferences & settings
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 08/25/2003 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2003, Fluid Studios, all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "3dme.h"
#include "PrefletAtomMap.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

	PrefletAtomMap::~PrefletAtomMap()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAtomMap::add(const wstring & name, const int controlID, const PrefletAtom::ControlType controlType, const bool defaultValue, const int slideRangeMin, const int slideRangeMax)
{
	(*this)[name] = PrefletAtom(controlID, controlType, defaultValue, slideRangeMin, slideRangeMax);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAtomMap::add(const wstring & name, const int controlID, const PrefletAtom::ControlType controlType, const int defaultValue, const int slideRangeMin, const int slideRangeMax)
{
	(*this)[name] = PrefletAtom(controlID, controlType, defaultValue, slideRangeMin, slideRangeMax);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAtomMap::add(const wstring & name, const int controlID, const PrefletAtom::ControlType controlType, const float defaultValue, const int slideRangeMin, const int slideRangeMax)
{
	(*this)[name] = PrefletAtom(controlID, controlType, defaultValue, slideRangeMin, slideRangeMax);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAtomMap::add(const wstring & name, const int controlID, const PrefletAtom::ControlType controlType, const wstring & defaultValue, const int slideRangeMin, const int slideRangeMax)
{
	(*this)[name] = PrefletAtom(controlID, controlType, defaultValue, slideRangeMin, slideRangeMax);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAtomMap::add(const wstring & name, const bool defaultValue)
{
	(*this)[name] = PrefletAtom(defaultValue);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAtomMap::add(const wstring & name, const int defaultValue)
{
	(*this)[name] = PrefletAtom(defaultValue);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAtomMap::add(const wstring & name, const float defaultValue)
{
	(*this)[name] = PrefletAtom(defaultValue);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAtomMap::add(const wstring & name, const wstring & defaultValue)
{
	(*this)[name] = PrefletAtom(defaultValue);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAtomMap::readFromRegistry(const wstring & subkey)
{
	for (iterator i = begin(); i != end(); ++i)
	{
		i->second.readFromRegistry(i->first, subkey);
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAtomMap::writeToRegistry(const wstring & subkey) const
{
	for (const_iterator i = begin(); i != end(); ++i)
	{
		i->second.writeToRegistry(i->first, subkey);
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAtomMap::readFromControls()
{
	for (iterator i = begin(); i != end(); ++i)
	{
		i->second.readFromControl(i->first);
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAtomMap::writeToControls()
{
	for (iterator i = begin(); i != end(); ++i)
	{
		i->second.writeToControl(i->first);
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAtomMap::createControls()
{
	for (iterator i = begin(); i != end(); ++i)
	{
		i->second.createControl();
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAtomMap::destroyControls()
{
	for (iterator i = begin(); i != end(); ++i)
	{
		i->second.destroyControl();
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletAtomMap.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
