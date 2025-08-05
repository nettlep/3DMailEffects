// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _   _____                                             
// |  __ \           / _| |    | | |  __ \                                            
// | |__) |_ __  ___| |_| | ___| |_| |__) |_ __  ___ __  ___   _      ___ _ __  _ __  
// |  ___/| '__|/ _ \  _| |/ _ \ __|  ___/| '__|/ _ \\ \/ / | | |    / __| '_ \| '_ \ 
// | |    | |  |  __/ | | |  __/ |_| |    | |  | (_) |>  <| |_| | _ | (__| |_) | |_) |
// |_|    |_|   \___|_| |_|\___|\__|_|    |_|   \___//_/\_\\__, |(_) \___| .__/| .__/ 
//                                                          __/ |        | |   | |    
//                                                         |___/         |_|   |_|    
//
//   Preflet for Proxy Settings
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
#include "PrefletProxy.h"
#include ".\prefletproxy.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

IMPLEMENT_DYNAMIC(PrefletProxy, Preflet)
BEGIN_MESSAGE_MAP(PrefletProxy, Preflet)
	ON_CBN_SELCHANGE(IDC_PROXY_TYPE_COMBO, OnCbnSelchangeProxyTypeCombo)
	ON_BN_CLICKED(IDC_PROXY_AUTH_CHECK, OnBnClickedProxyAuthCheck)
	ON_BN_CLICKED(IDC_PROXY_RESOLVE_CHECK, OnBnClickedProxyResolveCheck)
	ON_BN_CLICKED(IDC_HTTP_PROXY_CHECK, OnBnClickedHttpProxyCheck)
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	PrefletProxy::PrefletProxy(const wstring & registryKey, CWnd* pParent /*=NULL*/)
	: Preflet(PrefletProxy::IDD, registryKey, pParent)
{
	// These associate our atoms (things we want to store in the preferences) with the user controls in the peflet dialogs.
	// This also puts them in a central location where they can be loaded, saved, accessed and transferred to/from the controls.
	// Nifty, huh? :)

	atoms().add(_T("proxyType"), IDC_PROXY_TYPE_COMBO, PrefletAtom::ControlType::Combo, wstring(_T("None")));
	atoms().add(_T("proxyHost"), IDC_PROXY_HOST_EDIT, PrefletAtom::ControlType::Edit, wstring(_T("")));
	atoms().add(_T("proxyPort"), IDC_PROXY_PORT_EDIT, PrefletAtom::ControlType::Edit, wstring(_T("")));
	atoms().add(_T("proxyUser"), IDC_PROXY_USER_EDIT, PrefletAtom::ControlType::Edit, wstring(_T("")));
	atoms().add(_T("proxyPass"), IDC_PROXY_PASS_EDIT, PrefletAtom::ControlType::Edit, wstring(_T("")));
	atoms().add(_T("proxyAuth"), IDC_PROXY_AUTH_CHECK, PrefletAtom::ControlType::Button, false);
	atoms().add(_T("proxyResolve"), IDC_PROXY_RESOLVE_CHECK, PrefletAtom::ControlType::Button, false);
	atoms().add(_T("httpProxy"), IDC_HTTP_PROXY_CHECK, PrefletAtom::ControlType::Button, false);
	atoms().add(_T("httpProxyHost"), IDC_HTTP_PROXY_HOST_EDIT, PrefletAtom::ControlType::Edit, wstring(_T("")));
	atoms().add(_T("httpProxyPort"), IDC_HTTP_PROXY_PORT_EDIT, PrefletAtom::ControlType::Edit, wstring(_T("")));
	atoms().readFromRegistry(registryKey);
}
// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletProxy::OnPreInitPreflet()
{
	CComboBox *	ptr = static_cast<CComboBox *>(GetDlgItem(IDC_PROXY_TYPE_COMBO));
	if (ptr)
	{
		ptr->AddString(_T("None"));
		ptr->AddString(_T("SOCKS4"));
		ptr->AddString(_T("SOCKS4a (w/DNS)"));
		ptr->AddString(_T("SOCKS5"));
		ptr->AddString(_T("CERN/HTTP"));
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletProxy::OnPostInitPreflet()
{
	enableControls();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletProxy::enableControls()
{
	CString		str;
	CComboBox *	proxyTypeCombo = static_cast<CComboBox *>(GetDlgItem(IDC_PROXY_TYPE_COMBO));
	proxyTypeCombo->GetWindowText(str);
	BOOL		enabled = str != "None" ? TRUE:FALSE;

	static_cast<CStatic *>(GetDlgItem(IDC_PROXY_HOST_STATIC))->EnableWindow(enabled);
	static_cast<CEdit *>(GetDlgItem(IDC_PROXY_HOST_EDIT))->EnableWindow(enabled);
	static_cast<CStatic *>(GetDlgItem(IDC_PROXY_PORT_STATIC))->EnableWindow(enabled);
	static_cast<CEdit *>(GetDlgItem(IDC_PROXY_PORT_EDIT))->EnableWindow(enabled);
	static_cast<CButton *>(GetDlgItem(IDC_PROXY_AUTH_CHECK))->EnableWindow(enabled);
	static_cast<CButton *>(GetDlgItem(IDC_PROXY_RESOLVE_CHECK))->EnableWindow(enabled);

	if (enabled && static_cast<CButton *>(GetDlgItem(IDC_PROXY_AUTH_CHECK))->GetCheck() == FALSE) enabled = false;

	static_cast<CStatic *>(GetDlgItem(IDC_PROXY_USER_STATIC))->EnableWindow(enabled);
	static_cast<CEdit *>(GetDlgItem(IDC_PROXY_USER_EDIT))->EnableWindow(enabled);
	static_cast<CStatic *>(GetDlgItem(IDC_PROXY_PASS_STATIC))->EnableWindow(enabled);
	static_cast<CEdit *>(GetDlgItem(IDC_PROXY_PASS_EDIT))->EnableWindow(enabled);

	enabled = static_cast<CButton *>(GetDlgItem(IDC_HTTP_PROXY_CHECK))->GetCheck();

	static_cast<CStatic *>(GetDlgItem(IDC_HTTP_PROXY_HOST_STATIC))->EnableWindow(enabled);
	static_cast<CEdit *>(GetDlgItem(IDC_HTTP_PROXY_HOST_EDIT))->EnableWindow(enabled);
	static_cast<CStatic *>(GetDlgItem(IDC_HTTP_PROXY_PORT_STATIC))->EnableWindow(enabled);
	static_cast<CEdit *>(GetDlgItem(IDC_HTTP_PROXY_PORT_EDIT))->EnableWindow(enabled);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletProxy::OnCbnSelchangeProxyTypeCombo()
{
	enableControls();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletProxy::OnBnClickedProxyAuthCheck()
{
	enableControls();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletProxy::OnBnClickedProxyResolveCheck()
{
	enableControls();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletProxy::OnBnClickedHttpProxyCheck()
{
	enableControls();
}

// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletProxy.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
