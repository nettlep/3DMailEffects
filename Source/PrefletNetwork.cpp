// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _   _   _      _                       _                         
// |  __ \           / _| |    | | | \ | |    | |                     | |                        
// | |__) |_ __  ___| |_| | ___| |_|  \| | ___| |___      __ ___  _ __| | __     ___ _ __  _ __  
// |  ___/| '__|/ _ \  _| |/ _ \ __| . ` |/ _ \ __\ \ /\ / // _ \| '__| |/ /    / __| '_ \| '_ \ 
// | |    | |  |  __/ | | |  __/ |_| |\  |  __/ |_ \ V  V /| (_) | |  |   <  _ | (__| |_) | |_) |
// |_|    |_|   \___|_| |_|\___|\__|_| \_|\___|\__| \_/\_/  \___/|_|  |_|\_\(_) \___| .__/| .__/ 
//                                                                                  | |   | |    
//                                                                                  |_|   |_|    
//
//   Preflet for Network Settings
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

#include "stdafx.h"
#include "3dme.h"
#include "PrefletNetwork.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

IMPLEMENT_DYNAMIC(PrefletNetwork, Preflet)
BEGIN_MESSAGE_MAP(PrefletNetwork, Preflet)
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	PrefletNetwork::PrefletNetwork(const wstring & registryKey, CWnd* pParent /*=NULL*/)
	: Preflet(PrefletNetwork::IDD, registryKey, pParent)
{
	// These associate our atoms (things we want to store in the preferences) with the user controls in the peflet dialogs.
	// This also puts them in a central location where they can be loaded, saved, accessed and transferred to/from the controls.
	// Nifty, huh? :)

	atoms().add(_T("disableAutodial"), IDC_DISABLE_AUTODIAL, PrefletAtom::ControlType::Button, true);
	atoms().readFromRegistry(registryKey);
}

// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletNetwork.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
