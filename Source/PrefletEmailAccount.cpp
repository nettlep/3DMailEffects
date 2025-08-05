// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _   ______                 _ _                                      _                        
// |  __ \           / _| |    | | |  ____|               (_) |   /\                               | |                       
// | |__) |_ __  ___| |_| | ___| |_| |__   _ __ ___   __ _ _| |  /  \    ___  ___  ___  _   _ _ __ | |_      ___ _ __  _ __  
// |  ___/| '__|/ _ \  _| |/ _ \ __|  __| | '_ ` _ \ / _` | | | / /\ \  / __|/ __|/ _ \| | | | '_ \| __|    / __| '_ \| '_ \ 
// | |    | |  |  __/ | | |  __/ |_| |____| | | | | | (_| | | |/ ____ \| (__| (__| (_) | |_| | | | | |_  _ | (__| |_) | |_) |
// |_|    |_|   \___|_| |_|\___|\__|______|_| |_| |_|\__,_|_|_/_/    \_\\___|\___|\___/ \__,_|_| |_|\__|(_) \___| .__/| .__/ 
//                                                                                                              | |   | |    
//                                                                                                              |_|   |_|    
//
//   Preflet for Email Accounts
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
#include "PrefletEmailAccount.h"
#include ".\prefletemailaccount.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

extern "C" __declspec( dllimport ) bool WINAPI FSColorPickerDoModal(unsigned int * currentColor, unsigned int * originalColor, const int initialExpansionState);

// ---------------------------------------------------------------------------------------------------------------------------------

IMPLEMENT_DYNAMIC(PrefletEmailAccount, Preflet)
BEGIN_MESSAGE_MAP(PrefletEmailAccount, Preflet)
	ON_BN_CLICKED(IDC_ACCOUNT_TYPE_POP3_RADIO, OnBnClickedAccountTypePop3Radio)
	ON_BN_CLICKED(IDC_ACCOUNT_TYPE_WEBMAIL_RADIO, OnBnClickedAccountTypeWebmailRadio)
	ON_BN_CLICKED(IDC_ACCOUNT_TYPE_DISABLED_RADIO, OnBnClickedAccountTypeDisabledRadio)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_ACCOUNT_SCRIPT_COMBO, OnCbnSelchangeAccountScriptCombo)
	ON_BN_CLICKED(IDC_ACCOUNT_USE_COLOR_CHECK, OnBnClickedAccountUseColorCheck)
	ON_STN_CLICKED(IDC_ACCOUNT_COLOR_FRAME, OnStnClickedAccountColorFrame)
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	PrefletEmailAccount::PrefletEmailAccount(const wstring & registryKey, CWnd* pParent /*=NULL*/)
	: Preflet(PrefletEmailAccount::IDD, registryKey, pParent), _once(false)
{
	// These associate our atoms (things we want to store in the preferences) with the user controls in the peflet dialogs.
	// This also puts them in a central location where they can be loaded, saved, accessed and transferred to/from the controls.
	// Nifty, huh? :)

	atoms().add(_T("typePOP3"), IDC_ACCOUNT_TYPE_POP3_RADIO, PrefletAtom::ControlType::Button, true);
	atoms().add(_T("typeWebmail"), IDC_ACCOUNT_TYPE_WEBMAIL_RADIO, PrefletAtom::ControlType::Button, false);
	atoms().add(_T("typeDisabled"), IDC_ACCOUNT_TYPE_DISABLED_RADIO, PrefletAtom::ControlType::Button, false);
	atoms().add(_T("host"), IDC_ACCOUNT_HOST_EDIT, PrefletAtom::ControlType::Edit, wstring(_T("")));
	atoms().add(_T("port"), IDC_ACCOUNT_PORT_EDIT, PrefletAtom::ControlType::Edit, wstring(_T("110")));
	atoms().add(_T("script"), IDC_ACCOUNT_SCRIPT_COMBO, PrefletAtom::ControlType::Combo, wstring(_T("Select Webmail Script")));
	atoms().add(_T("username"), IDC_ACCOUNT_USERNAME_EDIT, PrefletAtom::ControlType::Edit, wstring(_T("")));
	atoms().add(_T("password"), IDC_ACCOUNT_PASSWORD_EDIT, PrefletAtom::ControlType::Edit, wstring(_T("")));
	atoms().add(_T("checkFrequency"), IDC_ACCOUNT_CHECK_FREQ_EDIT, PrefletAtom::ControlType::Edit, wstring(_T("5")));
	atoms().add(_T("checkFrequencyGranularity"), IDC_ACCOUNT_CHECK_FREQ_COMBO, PrefletAtom::ControlType::Combo, wstring(_T("Minutes")));
	atoms().add(_T("timeout"), IDC_ACCOUNT_SERVER_TIMEOUT_EDIT, PrefletAtom::ControlType::Edit, wstring(_T("120")));
	atoms().add(_T("retrieveAll"), IDC_ACCOUNT_RETRIEVE_ENTIRE_RADIO, PrefletAtom::ControlType::Button, true);
	atoms().add(_T("retrieveHeaders"), IDC_ACCOUNT_RETRIEVE_HEADERS_RADIO, PrefletAtom::ControlType::Button, false);
	atoms().add(_T("retrieveNothing"), IDC_ACCOUNT_RETRIEVE_NOTHING_RADIO, PrefletAtom::ControlType::Button, false);
	atoms().add(_T("useColor"), IDC_ACCOUNT_USE_COLOR_CHECK, PrefletAtom::ControlType::Button, false);
	atoms().add(_T("color"), static_cast<int>(0x00000000));
	atoms().readFromRegistry(registryKey);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletEmailAccount::OnPreInitPreflet()
{
	// Set the flag for showing/hiding the controls in WM_PAINT

	_once = true;

	// Pre-populate the combo boxes

	CComboBox *	scriptCombo = static_cast<CComboBox *>(GetDlgItem(IDC_ACCOUNT_SCRIPT_COMBO));
	scriptCombo->AddString(_T("Select Webmail Script"));

	CComboBox *	frequencyCombo = static_cast<CComboBox *>(GetDlgItem(IDC_ACCOUNT_CHECK_FREQ_COMBO));
	frequencyCombo->AddString(_T("Seconds"));
	frequencyCombo->AddString(_T("Minutes"));
	frequencyCombo->AddString(_T("Hours"));
	frequencyCombo->AddString(_T("Days"));
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletEmailAccount::OnPostInitPreflet()
{
	enableControls();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletEmailAccount::OnBnClickedAccountTypePop3Radio()
{
	enableControls();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletEmailAccount::OnBnClickedAccountTypeWebmailRadio()
{
	enableControls();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletEmailAccount::OnBnClickedAccountTypeDisabledRadio()
{
	enableControls();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletEmailAccount::OnPaint()
{
	CPaintDC dc(this);
	drawColorBox();

	// We need to hide/show some windows based on the account type selection. We can't do this in the OnInitXxxx() routine
	// since the windows don't actually have visibility yet, so this is as good a place as any to do this.

	if (_once)
	{
		_once = false;
		enableControls();
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletEmailAccount::OnCbnSelchangeAccountScriptCombo()
{
	enableControls();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletEmailAccount::OnBnClickedAccountUseColorCheck()
{
	drawColorBox();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletEmailAccount::OnStnClickedAccountColorFrame()
{
	if (!static_cast<CButton *>(GetDlgItem(IDC_ACCOUNT_USE_COLOR_CHECK))->GetCheck()) return;

	// Usage for the Fluid Studios Color Picker is trivial. There are two input colors:
	// 'currentColor' and 'originalColor'. The 'currentColor' represents the default color
	// for the color picker -- it starts off as if the user has selected this color.
	//
	// The other color, 'originalColor', is useful for times when you want to allow the user
	// to change a color. It shows this color in the bottom half of the "two-tone box." This
	// is useful for letting the user see the new color next to the original color they started
	// out with. If you set this value, then it's usually a good idea to set the 'currentColor'
	// to the same thing. This gives the user the imporession of "changing a color" as opposed to
	// "creating a color" (if that makes any sense :)
	//
	// Note that the dialog stores the previous color values in the registry. If you give it no
	// defaults, then the dialog will start with the values from the last time it was run. You
	// can force this by passing in NULL pointers. However, doing this means that the dialog has
	// nowhere to place the result (i.e. if you give it a NULL pointer, it can't tell you which
	// color the user selected.)
	//
	// So, you may optionally use the value 0x80000000 (high bit only) in either of these to tell
	// the color picker to start with its own defaults. On return, these colors will be set to
	// what the user selected.
	//
	// The final value specifies the initial state of the dialog, in terms of expansion. Here are
	// the values:
	//
	// 0 - use defaults
	// 1 - force expanded    show expansion button
	// 2 - force expanded    hide the expansion button
	// 3 - force compressed  show expansion button
	// 4 - force compressed  hide expansion button
	//
	// Return code is simple. true = "OK", false = "Cancel"

#define	invertColor(x) ( (((x)>>16)&0xff)|(((x)<<16)&0xff0000)|((x)&0xff00) )

	// PDNDEBUG -- need real color here
	unsigned int	currentColor = invertColor(0xff00ff);
	unsigned int	originalColor = invertColor(0xff00ff);

	if (FSColorPickerDoModal(&currentColor, &originalColor, 0))
	{
		// PDNDEBUG -- need real color here
		//_account.displayColor = invertColor(currentColor);
		drawColorBox();
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletEmailAccount::enableControls()
{
	// We'll use these for global disable/enable

	bool	pop3 = false;
	bool	webmail = false;
	bool	disabled = false;

	// Show POP3 account settings

	if (static_cast<CButton *>(GetDlgItem(IDC_ACCOUNT_TYPE_POP3_RADIO))->GetCheck() == TRUE)
	{
		GetDlgItem(IDC_ACCOUNT_SCRIPT_COMBO)->ShowWindow(FALSE);
		GetDlgItem(IDC_ACCOUNT_SCRIPT_STATIC)->ShowWindow(FALSE);

		GetDlgItem(IDC_ACCOUNT_HOST_EDIT)->ShowWindow(TRUE);
		GetDlgItem(IDC_ACCOUNT_HOST_STATIC)->ShowWindow(TRUE);
		GetDlgItem(IDC_ACCOUNT_PORT_EDIT)->ShowWindow(TRUE);
		GetDlgItem(IDC_ACCOUNT_PORT_STATIC)->ShowWindow(TRUE);

		pop3 = true;
	}

	// Show Webmail account settings

	else if (static_cast<CButton *>(GetDlgItem(IDC_ACCOUNT_TYPE_WEBMAIL_RADIO))->GetCheck() == TRUE)
	{
		GetDlgItem(IDC_ACCOUNT_SCRIPT_COMBO)->ShowWindow(TRUE);
		GetDlgItem(IDC_ACCOUNT_SCRIPT_STATIC)->ShowWindow(TRUE);

		GetDlgItem(IDC_ACCOUNT_HOST_EDIT)->ShowWindow(FALSE);
		GetDlgItem(IDC_ACCOUNT_HOST_STATIC)->ShowWindow(FALSE);
		GetDlgItem(IDC_ACCOUNT_PORT_EDIT)->ShowWindow(FALSE);
		GetDlgItem(IDC_ACCOUNT_PORT_STATIC)->ShowWindow(FALSE);

		CString		str;
		CComboBox *	scriptCombo = static_cast<CComboBox *>(GetDlgItem(IDC_ACCOUNT_SCRIPT_COMBO));
		scriptCombo->GetWindowText(str);
		if (str != "Select Webmail Script")	webmail = true;
		else					disabled = true;
	}

	// Disabled

	else if (static_cast<CButton *>(GetDlgItem(IDC_ACCOUNT_TYPE_DISABLED_RADIO))->GetCheck() == TRUE)
	{
		GetDlgItem(IDC_ACCOUNT_SCRIPT_COMBO)->ShowWindow(FALSE);
		GetDlgItem(IDC_ACCOUNT_SCRIPT_STATIC)->ShowWindow(FALSE);

		GetDlgItem(IDC_ACCOUNT_HOST_EDIT)->ShowWindow(FALSE);
		GetDlgItem(IDC_ACCOUNT_HOST_STATIC)->ShowWindow(FALSE);
		GetDlgItem(IDC_ACCOUNT_PORT_EDIT)->ShowWindow(FALSE);
		GetDlgItem(IDC_ACCOUNT_PORT_STATIC)->ShowWindow(FALSE);

		disabled = true;
	}

	GetDlgItem(IDC_SEPARATOR1_STATIC)->EnableWindow(disabled == false);
	GetDlgItem(IDC_ACCOUNT_USERNAME_EDIT)->EnableWindow(disabled == false);
	GetDlgItem(IDC_ACCOUNT_USERNAME_STATIC)->EnableWindow(disabled == false);
	GetDlgItem(IDC_ACCOUNT_PASSWORD_EDIT)->EnableWindow(disabled == false);
	GetDlgItem(IDC_ACCOUNT_PASSWORD_STATIC)->EnableWindow(disabled == false);
	GetDlgItem(IDC_ACCOUNT_CHECK_FREQ_EDIT)->EnableWindow(disabled == false);
	GetDlgItem(IDC_ACCOUNT_CHECK_FREQ_COMBO)->EnableWindow(disabled == false);
	GetDlgItem(IDC_ACCOUNT_CHECK_FREQ_STATIC)->EnableWindow(disabled == false);
	GetDlgItem(IDC_ACCOUNT_SERVER_TIMEOUT_EDIT)->EnableWindow(disabled == false);
	GetDlgItem(IDC_ACCOUNT_SERVER_TIMEOUT_STATIC)->EnableWindow(disabled == false);
	GetDlgItem(IDC_ACCOUNT_SERVER_TIMEOUT_STATIC2)->EnableWindow(disabled == false);
	GetDlgItem(IDC_SEPARATOR2_STATIC)->EnableWindow(disabled == false);
	GetDlgItem(IDC_ACCOUNT_RETRIEVE_ENTIRE_RADIO)->EnableWindow(pop3 == true);
	GetDlgItem(IDC_ACCOUNT_RETRIEVE_HEADERS_RADIO)->EnableWindow(pop3 == true);
	GetDlgItem(IDC_ACCOUNT_RETRIEVE_NOTHING_RADIO)->EnableWindow(pop3 == true);
	GetDlgItem(IDC_ACCOUNT_USE_COLOR_CHECK)->EnableWindow(disabled == false);
	GetDlgItem(IDC_ACCOUNT_COLOR_FRAME)->EnableWindow(disabled == false);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletEmailAccount::drawColorBox()
{
	CDC *	dc = GetDC();

	CRect		r;
	CStatic *	colorBox = static_cast<CStatic *>(GetDlgItem(IDC_ACCOUNT_COLOR_FRAME));
	colorBox->GetWindowRect(r);
	ScreenToClient(r);

	//dc.Draw3dRect(r, GetSysColor(COLOR_3DLIGHT), GetSysColor(COLOR_3DSHADOW));

	r.DeflateRect(2,2,2,2);

	if (static_cast<CButton *>(GetDlgItem(IDC_ACCOUNT_USE_COLOR_CHECK))->GetCheck())
	{
		// PDNDEBUG -- get the real color here...
		dc->FillSolidRect(r, 0xff00ff);
	}
	else
	{
		dc->FillSolidRect(r, GetSysColor(COLOR_3DFACE));
	}

	ReleaseDC(dc);
}

// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletEmailAccount.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
