// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _    _____ _       _           _     _     
// |  __ \           / _| |    | |  / ____| |     | |         | |   | |    
// | |__) |_ __  ___| |_| | ___| |_| |  __| | ___ | |__   __ _| |   | |__  
// |  ___/| '__|/ _ \  _| |/ _ \ __| | |_ | |/ _ \| '_ \ / _` | |   | '_ \ 
// | |    | |  |  __/ | | |  __/ |_| |__| | | (_) | |_) | (_| | | _ | | | |
// |_|    |_|   \___|_| |_|\___|\__|\_____|_|\___/|_.__/ \__,_|_|(_)|_| |_|
//                                                                         
//                                                                         
//
//   Global options preflet dialog
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 08/24/2003 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2003, Fluid Studios, all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_PREFLETGLOBAL
#define _H_PREFLETGLOBAL

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

#include "Preflet.h"

// ---------------------------------------------------------------------------------------------------------------------------------

class	PrefletGlobal : public Preflet
{
public:
	// Enumerations

		enum		{ IDD = IDD_PREFLET_GLOBAL };

	// Construction/Destruction

				PrefletGlobal(const wstring & registryKey, CWnd* pParent = NULL);
virtual				~PrefletGlobal() {}

	// Operators

	// Implementation

virtual		void		update();
virtual		void		updateLogger();

	// Accessors

private:
				DECLARE_DYNAMIC(PrefletGlobal)
				DECLARE_MESSAGE_MAP()
};

#endif // _H_PREFLETGLOBAL
// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletGlobal.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
