// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _            _                   __  __                 _     
// |  __ \           / _| |    | |     /\   | |                 |  \/  |               | |    
// | |__) |_ __  ___| |_| | ___| |_   /  \  | |_  ___  _ __ ___ | \  / | __ _ _ __     | |__  
// |  ___/| '__|/ _ \  _| |/ _ \ __| / /\ \ | __|/ _ \| '_ ` _ \| |\/| |/ _` | '_ \    | '_ \ 
// | |    | |  |  __/ | | |  __/ |_ / ____ \| |_| (_) | | | | | | |  | | (_| | |_) | _ | | | |
// |_|    |_|   \___|_| |_|\___|\__/_/    \_\\__|\___/|_| |_| |_|_|  |_|\__,_| .__/ (_)|_| |_|
//                                                                           | |              
//                                                                           |_|              
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

#ifndef	_H_PREFLETATOMMAP
#define _H_PREFLETATOMMAP

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

#include "PrefletAtom.h"

// ---------------------------------------------------------------------------------------------------------------------------------

class	PrefletAtomMap : public map<wstring, PrefletAtom>
{
public:
	// Construction/Destruction

virtual				~PrefletAtomMap();

	// Operators

	// Implementation

virtual		void		add(const wstring & name, const int controlID, const PrefletAtom::ControlType controlType, const bool defaultValue = false, const int slideRangeMin = 0, const int slideRangeMax = 0);
virtual		void		add(const wstring & name, const int controlID, const PrefletAtom::ControlType controlType, const int defaultValue = 0, const int slideRangeMin = 0, const int slideRangeMax = 0);
virtual		void		add(const wstring & name, const int controlID, const PrefletAtom::ControlType controlType, const float defaultValue = 0, const int slideRangeMin = 0, const int slideRangeMax = 0);
virtual		void		add(const wstring & name, const int controlID, const PrefletAtom::ControlType controlType, const wstring & defaultValue = _T(""), const int slideRangeMin = 0, const int slideRangeMax = 0);
virtual		void		add(const wstring & name, const bool defaultValue = false);
virtual		void		add(const wstring & name, const int defaultValue = 0);
virtual		void		add(const wstring & name, const float defaultValue = 0);
virtual		void		add(const wstring & name, const wstring & defaultValue = _T(""));
virtual		void		readFromRegistry(const wstring & subkey);
virtual		void		writeToRegistry(const wstring & subkey) const;
virtual		void		readFromControls();
virtual		void		writeToControls();
virtual		void		createControls();
virtual		void		destroyControls();

	// Accessors

private:
	// Data members
};

#endif // _H_PREFLETATOMMAP
// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletAtomMap.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
