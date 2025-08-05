// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _    ____  _   _                                    
// |  __ \           / _| |    | |  / __ \| | | |                                   
// | |__) |_ __  ___| |_| | ___| |_| |  | | |_| |__   ___ _ __      ___ _ __  _ __  
// |  ___/| '__|/ _ \  _| |/ _ \ __| |  | | __| '_ \ / _ \ '__|    / __| '_ \| '_ \ 
// | |    | |  |  __/ | | |  __/ |_| |__| | |_| | | |  __/ |    _ | (__| |_) | |_) |
// |_|    |_|   \___|_| |_|\___|\__|\____/ \__|_| |_|\___|_|   (_) \___| .__/| .__/ 
//                                                                     | |   | |    
//                                                                     |_|   |_|    
//
// Preferences container: holds other values that aren't associated to a control in a dialog
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 09/04/2004 by Paul Nettle
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
#include "3DME.h"
#include "PrefletOther.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

IMPLEMENT_DYNAMIC(PrefletOther, Preflet)
BEGIN_MESSAGE_MAP(PrefletOther, Preflet)
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	PrefletOther::PrefletOther(const wstring & registryKey, CWnd* pParent /*=NULL*/)
	: Preflet(0, registryKey, pParent)
{
	// These are atoms that get managed within the registry, but do not have a control associated to them

	atoms().add(_T("disableChecking"), false);
	atoms().add(_T("disableAlarms"), false);
	atoms().add(_T("disableAlarmRuns"), false);
	atoms().add(_T("disableReminders"), false);
	atoms().add(_T("disableReminderRuns"), false);
	atoms().add(_T("disablePopups"), false);
	atoms().add(_T("disable3DPopups"), false);
	atoms().add(_T("showNew"), false);
	atoms().add(_T("autoDeleteSpam"), false);
	atoms().add(_T("previewWrap"), true);
	atoms().add(_T("previewLinkify"), true);
	atoms().add(_T("checkSpam"), false);
	atoms().add(_T("spamNoNew"), false);
	atoms().add(_T("animationScreenshots"), false);
	atoms().add(_T("sClickClose"), false);
	atoms().add(_T("sClickEmailClient"), false);
	atoms().add(_T("dClickClose"), false);
	atoms().add(_T("dClickEmailClient"), false);
	atoms().add(_T("softenEdges3D"), false);
	atoms().add(_T("stayOnTop3D"), false);
	atoms().add(_T("rotateAlways"), false);

//	atoms().add(_T("key"), wstring(_T(""))); -- this gets handled manually, since we need the key in order to manage prefernces
	atoms().add(_T("autoDeleteSubjects"), wstring(_T("")));
	atoms().add(_T("autoDeleteSenders"), wstring(_T("")));
	atoms().add(_T("binData"), wstring(_T("")));

	atoms().add(_T("windowX"), 12345);
	atoms().add(_T("windowY"), 12345);
	atoms().add(_T("previewWindowX"), 12345);
	atoms().add(_T("previewWindowY"), 12345);
	atoms().add(_T("previewWindowW"), 728);
	atoms().add(_T("previewWindowH"), 502);
	atoms().add(_T("3DWindowX"), 12345);
	atoms().add(_T("3DWindowY"), 12345);
	atoms().add(_T("3DWindowScale"), 2);
	atoms().add(_T("3DExtraRotationX"), 0);
	atoms().add(_T("3DExtraRotationY"), 0);
	atoms().add(_T("currentTip"), -1);
	atoms().add(_T("currentBetaRegisteredPatchLevel"), 0);
	atoms().add(_T("currentRegisteredPatchLevel"), 0);
	atoms().add(_T("currentBetaSharewarePatchLevel"), 0);
	atoms().add(_T("currentSharewarePatchLevel"), 0);

	// This next one is really just a trick... set this value to integer 555 (and disable auto-updates) to truly disable them

	atoms().add(_T("stayOmTop"), 0);

	atoms().readFromRegistry(registryKey);
}

// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletOther.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

