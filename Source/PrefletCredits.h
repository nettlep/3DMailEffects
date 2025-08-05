// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _    _____               _ _ _           _     
// |  __ \           / _| |    | |  / ____|             | (_) |         | |    
// | |__) |_ __  ___| |_| | ___| |_| |     _ __  ___  __| |_| |_ ___    | |__  
// |  ___/| '__|/ _ \  _| |/ _ \ __| |    | '__|/ _ \/ _` | | __/ __|   | '_ \ 
// | |    | |  |  __/ | | |  __/ |_| |____| |  |  __/ (_| | | |_\__ \ _ | | | |
// |_|    |_|   \___|_| |_|\___|\__|\_____|_|   \___|\__,_|_|\__|___/(_)|_| |_|
//                                                                             
//                                                                             
//
//   Credits info
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

#ifndef	_H_PREFLETCREDITS
#define _H_PREFLETCREDITS

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

#include "Preflet.h"
#include "afxwin.h"
#include "HyperLink.h"

// ---------------------------------------------------------------------------------------------------------------------------------

class	PrefletCredits : public Preflet
{
public:
	// Enumerations

		enum		{ IDD = IDD_PREFLET_CREDITS };

	// Construction/Destruction

				PrefletCredits(const wstring & registryKey, CWnd* pParent = NULL);
virtual				~PrefletCredits() {}

	// Operators

	// Implementation

virtual		void		DoDataExchange(CDataExchange* pDX);

	// Accessors

private:
	// Data members

		CHyperLink	upxPage;
		CHyperLink	curlPage;
		CHyperLink	gnomePage;

				DECLARE_DYNAMIC(PrefletCredits)
				DECLARE_MESSAGE_MAP()
};

#endif // _H_PREFLETCREDITS
// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletCredits.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
