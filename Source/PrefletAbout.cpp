// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _            _                 _                        
// |  __ \           / _| |    | |     /\   | |               | |                       
// | |__) |_ __  ___| |_| | ___| |_   /  \  | |__   ___  _   _| |_      ___ _ __  _ __  
// |  ___/| '__|/ _ \  _| |/ _ \ __| / /\ \ | '_ \ / _ \| | | | __|    / __| '_ \| '_ \ 
// | |    | |  |  __/ | | |  __/ |_ / ____ \| |_) | (_) | |_| | |_  _ | (__| |_) | |_) |
// |_|    |_|   \___|_| |_|\___|\__/_/    \_\_.__/ \___/ \__,_|\__|(_) \___| .__/| .__/ 
//                                                                         | |   | |    
//                                                                         |_|   |_|    
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

#include "stdafx.h"
#include "3dme.h"
#include "PrefletAbout.h"
#include "EggDlg.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

IMPLEMENT_DYNAMIC(PrefletAbout, Preflet)
BEGIN_MESSAGE_MAP(PrefletAbout, Preflet)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	PrefletAbout::PrefletAbout(const wstring & registryKey, CWnd* pParent /*=NULL*/)
	: Preflet(PrefletAbout::IDD, registryKey, pParent)
{
	// These associate our atoms (things we want to store in the preferences) with the user controls in the peflet dialogs.
	// This also puts them in a central location where they can be loaded, saved, accessed and transferred to/from the controls.
	// Nifty, huh? :)

	//atoms().add(_T("zzzzz"), IDC_ZZZZZ, PrefletAtom::ControlType::Button, true);
	atoms().readFromRegistry(registryKey);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VERSION_STATIC, versionStatic);
	DDX_Control(pDX, IDC_REG_STATIC, regStatic);
	DDX_Control(pDX, IDC_HOME_PAGE_STATIC, homePage);
	DDX_Control(pDX, IDC_COMPANY_PAGE_STATIC, companyPage);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAbout::OnPreInitPreflet()
{
#if	defined(_REGISTERED_)
	wstring	regString = _T("R E G I S T E R E D\nThank you for your support.");
#elif	defined(_LITE_)
	wstring	regString = _T("Lite version. Visit the 3DME website for\ninformation on purchasing the full version.");
#else
	wstring	regString = _T("THIS PRODUCT IS NOT REGISTERED\nPlease register it to unlock its full functionality.");
#endif // _REGISTERED_

#if	defined(_BETA_)
	regString += _T("\n* * * * * B E T A * * * * *");
#endif // _BETA_

#if	!defined(_LITE_)
	wstring	verString = APP_STRING _T(" (patch level ") + integerToWString(getCurrentPatchLevel()) + _T(")");
#else
	wstring	verString = APP_STRING;
#endif // !_LITE_

	regStatic.SetWindowText(regString.c_str());
	versionStatic.SetWindowText(verString.c_str());
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAbout::OnLButtonDown(UINT nFlags, CPoint point)
{
static	count;
	count++;
		
	if (count >= 5)
	{
		count = 0;
		EggDlg	dlg;
		dlg.DoModal();
	}

	CDialog::OnLButtonDown(nFlags, point);
}

// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletAbout.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------