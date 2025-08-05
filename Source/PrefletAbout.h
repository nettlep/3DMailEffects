// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _            _                 _       _     
// |  __ \           / _| |    | |     /\   | |               | |     | |    
// | |__) |_ __  ___| |_| | ___| |_   /  \  | |__   ___  _   _| |_    | |__  
// |  ___/| '__|/ _ \  _| |/ _ \ __| / /\ \ | '_ \ / _ \| | | | __|   | '_ \ 
// | |    | |  |  __/ | | |  __/ |_ / ____ \| |_) | (_) | |_| | |_  _ | | | |
// |_|    |_|   \___|_| |_|\___|\__/_/    \_\_.__/ \___/ \__,_|\__|(_)|_| |_|
//                                                                           
//                                                                           
//
//   About info
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

#ifndef	_H_PREFLETABOUT
#define _H_PREFLETABOUT

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

#include "Preflet.h"
#include "HyperLink.h"
#include "afxwin.h"

// ---------------------------------------------------------------------------------------------------------------------------------

class	PrefletAbout : public Preflet
{
public:
	// Enumerations

		enum		{ IDD = IDD_PREFLET_ABOUT };

	// Construction/Destruction

				PrefletAbout(const wstring & registryKey, CWnd* pParent = NULL);
virtual				~PrefletAbout() {}

	// Operators

	// Implementation

virtual		void		DoDataExchange(CDataExchange* pDX);
virtual		void		OnPreInitPreflet();
afx_msg		void		OnLButtonDown(UINT nFlags, CPoint point);

	// Accessors

private:
	// Data members

		CStatic		versionStatic;
		CStatic		regStatic;
		CHyperLink	homePage;
		CHyperLink	companyPage;

				DECLARE_DYNAMIC(PrefletAbout)
				DECLARE_MESSAGE_MAP()
};

#endif // _H_PREFLETABOUT
// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletAbout.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
