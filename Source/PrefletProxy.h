// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _   _____                            _     
// |  __ \           / _| |    | | |  __ \                          | |    
// | |__) |_ __  ___| |_| | ___| |_| |__) |_ __  ___ __  ___   _    | |__  
// |  ___/| '__|/ _ \  _| |/ _ \ __|  ___/| '__|/ _ \\ \/ / | | |   | '_ \ 
// | |    | |  |  __/ | | |  __/ |_| |    | |  | (_) |>  <| |_| | _ | | | |
// |_|    |_|   \___|_| |_|\___|\__|_|    |_|   \___//_/\_\\__, |(_)|_| |_|
//                                                          __/ |          
//                                                         |___/           
//
//   Preflet for Proxy Settings
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

#ifndef	_H_PREFLETPROXY
#define _H_PREFLETPROXY

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

#include "Preflet.h"

// ---------------------------------------------------------------------------------------------------------------------------------

class	PrefletProxy : public Preflet
{
public:
	// Enumerations

		enum		{ IDD = IDD_PREFLET_PROXY };

	// Construction/Destruction

				PrefletProxy(const wstring & registryKey, CWnd* pParent = NULL);

	// Operators

	// Implementation

virtual		void		OnPreInitPreflet();
virtual		void		OnPostInitPreflet();
virtual		void		enableControls();
afx_msg		void		OnCbnSelchangeProxyTypeCombo();
afx_msg		void		OnBnClickedProxyAuthCheck();
afx_msg		void		OnBnClickedProxyResolveCheck();
afx_msg		void		OnBnClickedHttpProxyCheck();

	// Accessors

private:
	// Data members
				DECLARE_DYNAMIC(PrefletProxy)
				DECLARE_MESSAGE_MAP()
};

#endif // _H_PREFLETPROXY
// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletProxy.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
