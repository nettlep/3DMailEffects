// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _   _   _   _ _           _                    _     
// |  __ \           / _| |    | | | | | | | (_)         | |                  | |    
// | |__) |_ __  ___| |_| | ___| |_| | | | | |_ _ __   __| | ___ __      __   | |__  
// |  ___/| '__|/ _ \  _| |/ _ \ __| | | | | | | '_ \ / _` |/ _ \\ \ /\ / /   | '_ \ 
// | |    | |  |  __/ | | |  __/ |_|  V _ V  | | | | | (_| | (_) |\ V  V /  _ | | | |
// |_|    |_|   \___|_| |_|\___|\__|\__/ \__/|_|_| |_|\__,_|\___/  \_/\_/  (_)|_| |_|
//                                                                                   
//                                                                                   
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

#ifndef	_H_PREFLETWINDOW
#define _H_PREFLETWINDOW

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

#include "Preflet.h"

// ---------------------------------------------------------------------------------------------------------------------------------

class	PrefletWindow : public Preflet
{
public:
		enum		{ IDD = IDD_PREFLET_WINDOW };

	// Construction/Destruction

				PrefletWindow(const wstring & registryKey, CWnd* pParent = NULL);
virtual				~PrefletWindow() {}

	// Operators

	// Implementation

virtual		void		OnPostInitPreflet();
virtual		void		enableControls();
afx_msg		void		OnBnClickedSnapToEdges();

	// Accessors

private:
	// Data members
				DECLARE_MESSAGE_MAP()
				DECLARE_DYNAMIC(PrefletWindow)
};

#endif // _H_PREFLETWINDOW
// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletWindow.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

