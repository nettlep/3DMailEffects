// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _    _____               _ _ _                            
// |  __ \           / _| |    | |  / ____|             | (_) |                           
// | |__) |_ __  ___| |_| | ___| |_| |     _ __  ___  __| |_| |_ ___      ___ _ __  _ __  
// |  ___/| '__|/ _ \  _| |/ _ \ __| |    | '__|/ _ \/ _` | | __/ __|    / __| '_ \| '_ \ 
// | |    | |  |  __/ | | |  __/ |_| |____| |  |  __/ (_| | | |_\__ \ _ | (__| |_) | |_) |
// |_|    |_|   \___|_| |_|\___|\__|\_____|_|   \___|\__,_|_|\__|___/(_) \___| .__/| .__/ 
//                                                                           | |   | |    
//                                                                           |_|   |_|    
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

#include "stdafx.h"
#include "3dme.h"
#include "PrefletCredits.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

IMPLEMENT_DYNAMIC(PrefletCredits, Preflet)
BEGIN_MESSAGE_MAP(PrefletCredits, Preflet)
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	PrefletCredits::PrefletCredits(const wstring & registryKey, CWnd* pParent /*=NULL*/)
	: Preflet(PrefletCredits::IDD, registryKey, pParent)
{
	// These associate our atoms (things we want to store in the preferences) with the user controls in the peflet dialogs.
	// This also puts them in a central location where they can be loaded, saved, accessed and transferred to/from the controls.
	// Nifty, huh? :)

	//atoms().add(_T("zzzzz"), IDC_ZZZZZ, PrefletAtom::ControlType::Button, true);
	atoms().readFromRegistry(registryKey);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletCredits::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UPX_PAGE_STATIC, upxPage);
	DDX_Control(pDX, IDC_CURL_PAGE_STATIC, curlPage);
	DDX_Control(pDX, IDC_GNOME_PAGE_STATIC, gnomePage);
}

// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletCredits.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
