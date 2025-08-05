// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _   ______                 _ _  _____      _   _   _                     _     
// |  __ \           / _| |    | | |  ____|               (_) |/ ____|    | | | | (_)                   | |    
// | |__) |_ __  ___| |_| | ___| |_| |__   _ __ ___   __ _ _| | (___   ___| |_| |_ _ _ __   __ _ ___    | |__  
// |  ___/| '__|/ _ \  _| |/ _ \ __|  __| | '_ ` _ \ / _` | | |\___ \ / _ \ __| __| | '_ \ / _` / __|   | '_ \ 
// | |    | |  |  __/ | | |  __/ |_| |____| | | | | | (_| | | |____) |  __/ |_| |_| | | | | (_| \__ \ _ | | | |
// |_|    |_|   \___|_| |_|\___|\__|______|_| |_| |_|\__,_|_|_|_____/ \___|\__|\__|_|_| |_|\__, |___/(_)|_| |_|
//                                                                                          __/ |              
//                                                                                         |___/               
//
//   Preflet for Email Settings
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 12/15/2003 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2003, Fluid Studios, all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_PREFLETEMAILSETTINGS
#define _H_PREFLETEMAILSETTINGS

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

#include "Preflet.h"

// ---------------------------------------------------------------------------------------------------------------------------------

class	PrefletEmailSettings : public Preflet
{
public:
	// Enumerations

		enum		{ IDD = IDD_PREFLET_EMAIL_SETTINGS };

	// Construction/Destruction

				PrefletEmailSettings(const wstring & registryKey, CWnd* pParent = NULL);

	// Operators

	// Implementation

virtual		void		OnPreInitPreflet();
virtual		void		OnPostInitPreflet();
virtual		void		enableControls();
afx_msg		void		OnBnClickedAccountCreateButton();
afx_msg		void		OnBnClickedAccountDeleteButton();
afx_msg		void		OnCbnSelchangeAccountDeleteCombo();

	// Accessors

private:
	// Data members
				DECLARE_DYNAMIC(PrefletEmailSettings)
				DECLARE_MESSAGE_MAP()
};

#endif // _H_PREFLETEMAILSETTINGS
// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletEmailSettings.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
