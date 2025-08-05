// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _    _____ _    _               _     
// |  __ \           / _| |    | |  / ____| |  (_)             | |    
// | |__) |_ __  ___| |_| | ___| |_| (___ | | ___ _ __  ___    | |__  
// |  ___/| '__|/ _ \  _| |/ _ \ __|\___ \| |/ / | '_ \/ __|   | '_ \ 
// | |    | |  |  __/ | | |  __/ |_ ____) |   <| | | | \__ \ _ | | | |
// |_|    |_|   \___|_| |_|\___|\__|_____/|_|\_\_|_| |_|___/(_)|_| |_|
//                                                                                   
//                                                                                   
//
//   Manages application skin settings
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 03/01/2004 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2004, Fluid Studios, all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_PREFLETSKINS
#define _H_PREFLETSKINS

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

#include "Hyperlink.h"
#include "Preflet.h"

// ---------------------------------------------------------------------------------------------------------------------------------

class	PrefletSkins : public Preflet
{
public:
		enum		{ IDD = IDD_PREFLET_SKINS };

	// Construction/Destruction

				PrefletSkins(const wstring & registryKey, CWnd* pParent = NULL);
virtual				~PrefletSkins() {}

	// Operators

	// Implementation

virtual		void		DoDataExchange(CDataExchange* pDX);
virtual		void		OnPostInitPreflet();
virtual		void		enableControls();

	// Accessors

private:
	// Data members

		CHyperLink	moreSkinsLink;

				DECLARE_MESSAGE_MAP()
				DECLARE_DYNAMIC(PrefletSkins)
};

#endif // _H_PREFLETSKINS
// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletSkins.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

