// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _   ______                 _ _  _____      _   _   _                                      
// |  __ \           / _| |    | | |  ____|               (_) |/ ____|    | | | | (_)                                     
// | |__) |_ __  ___| |_| | ___| |_| |__   _ __ ___   __ _ _| | (___   ___| |_| |_ _ _ __   __ _ ___      ___ _ __  _ __  
// |  ___/| '__|/ _ \  _| |/ _ \ __|  __| | '_ ` _ \ / _` | | |\___ \ / _ \ __| __| | '_ \ / _` / __|    / __| '_ \| '_ \ 
// | |    | |  |  __/ | | |  __/ |_| |____| | | | | | (_| | | |____) |  __/ |_| |_| | | | | (_| \__ \ _ | (__| |_) | |_) |
// |_|    |_|   \___|_| |_|\___|\__|______|_| |_| |_|\__,_|_|_|_____/ \___|\__|\__|_|_| |_|\__, |___/(_) \___| .__/| .__/ 
//                                                                                          __/ |            | |   | |    
//                                                                                         |___/             |_|   |_|    
//
//   Preflet for Email Settings
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 12/15/2003 by Paul Nettle
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
#include "PrefletEmailSettings.h"
#include ".\prefletemailsettings.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

IMPLEMENT_DYNAMIC(PrefletEmailSettings, Preflet)
BEGIN_MESSAGE_MAP(PrefletEmailSettings, Preflet)
	ON_BN_CLICKED(IDC_ACCOUNT_CREATE_BUTTON, OnBnClickedAccountCreateButton)
	ON_BN_CLICKED(IDC_ACCOUNT_DELETE_BUTTON, OnBnClickedAccountDeleteButton)
	ON_CBN_SELCHANGE(IDC_ACCOUNT_DELETE_COMBO, OnCbnSelchangeAccountDeleteCombo)
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	PrefletEmailSettings::PrefletEmailSettings(const wstring & registryKey, CWnd* pParent /*=NULL*/)
	: Preflet(PrefletEmailSettings::IDD, registryKey, pParent)
{
	// These associate our atoms (things we want to store in the preferences) with the user controls in the peflet dialogs.
	// This also puts them in a central location where they can be loaded, saved, accessed and transferred to/from the controls.
	// Nifty, huh? :)

	atoms().add(_T("confirmDeletes"), IDC_EMAIL_CONFIRM_DELETE_CHECK, PrefletAtom::ControlType::Button, true);
	atoms().readFromRegistry(registryKey);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletEmailSettings::OnPreInitPreflet()
{
	// PDNDEBUG -- Populate the delete account listbox with our account names

	CComboBox *	deleteCombo = static_cast<CComboBox *>(GetDlgItem(IDC_ACCOUNT_DELETE_COMBO));
	deleteCombo->AddString(_T("Select email account"));
	deleteCombo->SetCurSel(0);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletEmailSettings::OnPostInitPreflet()
{
	enableControls();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletEmailSettings::enableControls()
{
	CComboBox *	deleteCombo = static_cast<CComboBox *>(GetDlgItem(IDC_ACCOUNT_DELETE_COMBO));
	CString		str;
	deleteCombo->GetWindowText(str);

	bool	enabled = deleteCombo->GetCount() > 1 ? TRUE:FALSE;
	deleteCombo->EnableWindow(enabled);

	if (enabled && str == _T("Select email account")) enabled = FALSE;

	static_cast<CButton *>(GetDlgItem(IDC_ACCOUNT_DELETE_BUTTON))->EnableWindow(enabled);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletEmailSettings::OnBnClickedAccountCreateButton()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletEmailSettings::OnBnClickedAccountDeleteButton()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletEmailSettings::OnCbnSelchangeAccountDeleteCombo()
{
	enableControls();
}

// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletEmailSettings.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
