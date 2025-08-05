// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _   ______                 _ _                                      _       _     
// |  __ \           / _| |    | | |  ____|               (_) |   /\                               | |     | |    
// | |__) |_ __  ___| |_| | ___| |_| |__   _ __ ___   __ _ _| |  /  \    ___  ___  ___  _   _ _ __ | |_    | |__  
// |  ___/| '__|/ _ \  _| |/ _ \ __|  __| | '_ ` _ \ / _` | | | / /\ \  / __|/ __|/ _ \| | | | '_ \| __|   | '_ \ 
// | |    | |  |  __/ | | |  __/ |_| |____| | | | | | (_| | | |/ ____ \| (__| (__| (_) | |_| | | | | |_  _ | | | |
// |_|    |_|   \___|_| |_|\___|\__|______|_| |_| |_|\__,_|_|_/_/    \_\\___|\___|\___/ \__,_|_| |_|\__|(_)|_| |_|
//                                                                                                                
//                                                                                                                
//
//   Preflet for Email Accounts
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

#ifndef	_H_PREFLETEMAILACCOUNT
#define _H_PREFLETEMAILACCOUNT

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

#include "Preflet.h"

// ---------------------------------------------------------------------------------------------------------------------------------

class	PrefletEmailAccount : public Preflet
{
public:
	// Enumerations

		enum		{ IDD = IDD_PREFLET_EMAIL_ACCOUNT };

	// Construction/Destruction

				PrefletEmailAccount(const wstring & registryKey, CWnd* pParent = NULL);

	// Operators

	// Implementation

virtual		void		OnPreInitPreflet();
virtual		void		OnPostInitPreflet();
afx_msg		void		OnBnClickedAccountTypePop3Radio();
afx_msg		void		OnBnClickedAccountTypeWebmailRadio();
afx_msg		void		OnBnClickedAccountTypeDisabledRadio();
afx_msg		void		OnPaint();
afx_msg		void		OnCbnSelchangeAccountScriptCombo();
afx_msg		void		OnBnClickedAccountUseColorCheck();
afx_msg		void		OnStnClickedAccountColorFrame();
virtual		void		enableControls();
virtual		void		drawColorBox();

	// Accessors

private:
	// Data members

		bool		_once;

				DECLARE_DYNAMIC(PrefletEmailAccount)
				DECLARE_MESSAGE_MAP()
};

#endif // _H_PREFLETEMAILACCOUNT
// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletEmailAccount.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
