// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _    _____                              _                        
// |  __ \           / _| |    | |  / ____|                            | |                       
// | |__) |_ __  ___| |_| | ___| |_| (___  _   _ _ __  _ __   ___  _ __| |_      ___ _ __  _ __  
// |  ___/| '__|/ _ \  _| |/ _ \ __|\___ \| | | | '_ \| '_ \ / _ \| '__| __|    / __| '_ \| '_ \ 
// | |    | |  |  __/ | | |  __/ |_ ____) | |_| | |_) | |_) | (_) | |  | |_  _ | (__| |_) | |_) |
// |_|    |_|   \___|_| |_|\___|\__|_____/ \__,_| .__/| .__/ \___/|_|   \__|(_) \___| .__/| .__/ 
//                                              | |   | |                           | |   | |    
//                                              |_|   |_|                           |_|   |_|    
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

#include "stdafx.h"
#include "3dme.h"
#include "PrefletSupport.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

IMPLEMENT_DYNAMIC(PrefletSupport, Preflet)
BEGIN_MESSAGE_MAP(PrefletSupport, Preflet)
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	PrefletSupport::PrefletSupport(const wstring & registryKey, CWnd* pParent /*=NULL*/)
	: Preflet(PrefletSupport::IDD, registryKey, pParent)
{
	// These associate our atoms (things we want to store in the preferences) with the user controls in the peflet dialogs.
	// This also puts them in a central location where they can be loaded, saved, accessed and transferred to/from the controls.
	// Nifty, huh? :)

	//atoms().add(_T("zzzzz"), IDC_ZZZZZ, PrefletAtom::ControlType::Button, true);
	atoms().readFromRegistry(registryKey);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletSupport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VERSION_STATIC, versionStatic);
	DDX_Control(pDX, IDC_HELP_PAGE_STATIC, helpPage);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletSupport::OnPreInitPreflet()
{
#if	!defined(_LITE_)
	wstring	verString = APP_STRING _T(" (patch level ") + integerToWString(getCurrentPatchLevel()) + _T(")");
#else
	wstring	verString = APP_STRING;
#endif // !_LITE_

	versionStatic.SetWindowText(verString.c_str());
}

// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletSupport.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
