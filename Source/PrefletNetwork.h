// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _   _   _      _                       _        _     
// |  __ \           / _| |    | | | \ | |    | |                     | |      | |    
// | |__) |_ __  ___| |_| | ___| |_|  \| | ___| |___      __ ___  _ __| | __   | |__  
// |  ___/| '__|/ _ \  _| |/ _ \ __| . ` |/ _ \ __\ \ /\ / // _ \| '__| |/ /   | '_ \ 
// | |    | |  |  __/ | | |  __/ |_| |\  |  __/ |_ \ V  V /| (_) | |  |   <  _ | | | |
// |_|    |_|   \___|_| |_|\___|\__|_| \_|\___|\__| \_/\_/  \___/|_|  |_|\_\(_)|_| |_|
//                                                                                    
//                                                                                    
//
//   Preflet for Network Settings
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 12/14/2003 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2003, Fluid Studios, all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_PREFLETNETWORK
#define _H_PREFLETNETWORK

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

#include "Preflet.h"

// ---------------------------------------------------------------------------------------------------------------------------------

class	PrefletNetwork : public Preflet
{
public:
	// Enumerations

		enum		{ IDD = IDD_PREFLET_NETWORK };

	// Construction/Destruction

				PrefletNetwork(const wstring & registryKey, CWnd* pParent = NULL);

	// Operators

	// Implementation

	// Accessors

private:
	// Data members
				DECLARE_DYNAMIC(PrefletNetwork)
				DECLARE_MESSAGE_MAP()
};

#endif // _H_PREFLETNETWORK
// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletNetwork.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
