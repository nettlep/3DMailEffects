// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _    _____                              _       _     
// |  __ \           / _| |    | |  / ____|                            | |     | |    
// | |__) |_ __  ___| |_| | ___| |_| (___  _   _ _ __  _ __   ___  _ __| |_    | |__  
// |  ___/| '__|/ _ \  _| |/ _ \ __|\___ \| | | | '_ \| '_ \ / _ \| '__| __|   | '_ \ 
// | |    | |  |  __/ | | |  __/ |_ ____) | |_| | |_) | |_) | (_) | |  | |_  _ | | | |
// |_|    |_|   \___|_| |_|\___|\__|_____/ \__,_| .__/| .__/ \___/|_|   \__|(_)|_| |_|
//                                              | |   | |                             
//                                              |_|   |_|                             
//
//   Support info
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 08/31/2003 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2003, Fluid Studios, all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_PREFLETSUPPORT
#define _H_PREFLETSUPPORT

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

#include "Preflet.h"
#include "afxwin.h"
#include "HyperLink.h"

// ---------------------------------------------------------------------------------------------------------------------------------

class	PrefletSupport : public Preflet
{
public:
	// Enumerations

		enum		{ IDD = IDD_PREFLET_SUPPORT };

	// Construction/Destruction

				PrefletSupport(const wstring & registryKey, CWnd* pParent = NULL);
virtual				~PrefletSupport() {}

	// Operators

	// Implementation

virtual		void		DoDataExchange(CDataExchange* pDX);
virtual		void		OnPreInitPreflet();

	// Accessors

private:
	// Data members

		CStatic		versionStatic;
		CHyperLink	helpPage;

				DECLARE_DYNAMIC(PrefletSupport)
				DECLARE_MESSAGE_MAP()
};

#endif // _H_PREFLETSUPPORT
// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletSupport.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
