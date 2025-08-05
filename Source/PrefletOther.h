// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _    ____  _   _                   _     
// |  __ \           / _| |    | |  / __ \| | | |                 | |    
// | |__) |_ __  ___| |_| | ___| |_| |  | | |_| |__   ___ _ __    | |__  
// |  ___/| '__|/ _ \  _| |/ _ \ __| |  | | __| '_ \ / _ \ '__|   | '_ \ 
// | |    | |  |  __/ | | |  __/ |_| |__| | |_| | | |  __/ |    _ | | | |
// |_|    |_|   \___|_| |_|\___|\__|\____/ \__|_| |_|\___|_|   (_)|_| |_|
//                                                                       
//                                                                       
//
// Preferences container: holds other values that aren't associated to a control in a dialog
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 09/04/2004 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2004, Fluid Studios, all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_PREFLETOTHER
#define _H_PREFLETOTHER

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

#include "Preflet.h"

// ---------------------------------------------------------------------------------------------------------------------------------

class	PrefletOther : public Preflet
{
public:
	// Enumerations

	// Construction/Destruction

				PrefletOther(const wstring & registryKey, CWnd* pParent = NULL);
virtual				~PrefletOther() {}

	// Operators

	// Implementation

	// Accessors

private:
				DECLARE_DYNAMIC(PrefletOther)
				DECLARE_MESSAGE_MAP()
};

#endif // _H_PREFLETOTHER
// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletOther.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

