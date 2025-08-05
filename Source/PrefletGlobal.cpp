// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _    _____ _       _           _                      
// |  __ \           / _| |    | |  / ____| |     | |         | |                     
// | |__) |_ __  ___| |_| | ___| |_| |  __| | ___ | |__   __ _| |     ___ _ __  _ __  
// |  ___/| '__|/ _ \  _| |/ _ \ __| | |_ | |/ _ \| '_ \ / _` | |    / __| '_ \| '_ \ 
// | |    | |  |  __/ | | |  __/ |_| |__| | | (_) | |_) | (_| | | _ | (__| |_) | |_) |
// |_|    |_|   \___|_| |_|\___|\__|\_____|_|\___/|_.__/ \__,_|_|(_) \___| .__/| .__/ 
//                                                                       | |   | |    
//                                                                       |_|   |_|    
//
//   Global options preflet dialog
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 08/24/2003 by Paul Nettle
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
#include "3Dme.h"
#include "PrefletGlobal.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

IMPLEMENT_DYNAMIC(PrefletGlobal, Preflet)
BEGIN_MESSAGE_MAP(PrefletGlobal, Preflet)
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	PrefletGlobal::PrefletGlobal(const wstring & registryKey, CWnd* pParent /*=NULL*/)
	: Preflet(PrefletGlobal::IDD, registryKey, pParent)
{
	// These associate our atoms (things we want to store in the preferences) with the user controls in the peflet dialogs.
	// This also puts them in a central location where they can be loaded, saved, accessed and transferred to/from the controls.
	// Nifty, huh? :)

	atoms().add(_T("runOnStartup"), IDC_RUN_ON_STARTUP, PrefletAtom::ControlType::Button, true);
	atoms().add(_T("tipsOnStartup"), IDC_TIPS_ON_STARTUP, PrefletAtom::ControlType::Button, true);
	atoms().add(_T("enableTransactionLogs"), IDC_ENABLE_TRANSACTION_LOGS, PrefletAtom::ControlType::Button, false);
	atoms().add(_T("autoCheckForUpdates"), IDC_AUTO_CHECK_FOR_UPDATES, PrefletAtom::ControlType::Button, true);
	atoms().add(_T("blinkkeyboardLEDSpeed"), IDC_BLINK_KEYBOARD_LED_SPEED, PrefletAtom::ControlType::Slider, 0, 0, 10);
	atoms().add(_T("blinktrayIconSpeed"), IDC_BLINK_TRAY_ICON_SPEED, PrefletAtom::ControlType::Slider, 5, 0, 10);
	atoms().readFromRegistry(registryKey);

	// Init the logger

	updateLogger();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletGlobal::update()
{
	Preflet::update();
	updateLogger();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletGlobal::updateLogger()
{
	logger.logMask() = atoms()[_T("enableTransactionLogs")].booleanValue() ? 0xffffffff : Logger::LOG_CRIT;
}


// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletGlobal.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
