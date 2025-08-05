// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _    _____ _    _                                
// |  __ \           / _| |    | |  / ____| |  (_)                               
// | |__) |_ __  ___| |_| | ___| |_| (___ | | ___ _ __  ___      ___ _ __  _ __  
// |  ___/| '__|/ _ \  _| |/ _ \ __|\___ \| |/ / | '_ \/ __|    / __| '_ \| '_ \ 
// | |    | |  |  __/ | | |  __/ |_ ____) |   <| | | | \__ \ _ | (__| |_) | |_) |
// |_|    |_|   \___|_| |_|\___|\__|_____/|_|\_\_|_| |_|___/(_) \___| .__/| .__/ 
//                                                                  | |   | |    
//                                                                  |_|   |_|    
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

#include "stdafx.h"
#include "3dme.h"
#include "PrefletSkins.h"
#include ".\PrefletSkins.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

IMPLEMENT_DYNAMIC(PrefletSkins, Preflet)
BEGIN_MESSAGE_MAP(PrefletSkins, Preflet)
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	PrefletSkins::PrefletSkins(const wstring & registryKey, CWnd* pParent /*=NULL*/)
	: Preflet(PrefletSkins::IDD, registryKey, pParent)
{
	// These associate our atoms (things we want to store in the preferences) with the user controls in the peflet dialogs.
	// This also puts them in a central location where they can be loaded, saved, accessed and transferred to/from the controls.
	// Nifty, huh? :)

	atoms().add(_T("skinName"), wstring(_T("default.skin")));
	atoms().add(_T("modelName"), wstring(_T("default.3ds")));
	atoms().add(_T("materialName"), wstring(_T("default.bmp")));

	atoms().readFromRegistry(registryKey);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletSkins::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MORE_SKINS_LINK, moreSkinsLink);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletSkins::OnPostInitPreflet()
{
	enableControls();

	moreSkinsLink.SetURL(_T("http://www.3dmaileffects.com/extras.shtml"));
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletSkins::enableControls()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletSkins.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
