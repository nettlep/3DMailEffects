// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _   _   _   _ _           _                                     
// |  __ \           / _| |    | | | | | | | (_)         | |                                    
// | |__) |_ __  ___| |_| | ___| |_| | | | | |_ _ __   __| | ___ __      __     ___ _ __  _ __  
// |  ___/| '__|/ _ \  _| |/ _ \ __| | | | | | | '_ \ / _` |/ _ \\ \ /\ / /    / __| '_ \| '_ \ 
// | |    | |  |  __/ | | |  __/ |_|  V _ V  | | | | | (_| | (_) |\ V  V /  _ | (__| |_) | |_) |
// |_|    |_|   \___|_| |_|\___|\__|\__/ \__/|_|_| |_|\__,_|\___/  \_/\_/  (_) \___| .__/| .__/ 
//                                                                                 | |   | |    
//                                                                                 |_|   |_|    
//
//   Manages atoms stored in our various preferences & settings
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 08/25/2003 by Paul Nettle
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
#include "PrefletWindow.h"
#include ".\prefletwindow.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

IMPLEMENT_DYNAMIC(PrefletWindow, Preflet)
BEGIN_MESSAGE_MAP(PrefletWindow, Preflet)
	ON_BN_CLICKED(IDC_SNAP_TO_EDGES_CHECK, OnBnClickedSnapToEdges)
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	PrefletWindow::PrefletWindow(const wstring & registryKey, CWnd* pParent /*=NULL*/)
	: Preflet(PrefletWindow::IDD, registryKey, pParent)
{
	// These associate our atoms (things we want to store in the preferences) with the user controls in the peflet dialogs.
	// This also puts them in a central location where they can be loaded, saved, accessed and transferred to/from the controls.
	// Nifty, huh? :)

	atoms().add(_T("startMinimized"), IDC_START_MINIMIZED_CHECK, PrefletAtom::ControlType::Button, false);
	atoms().add(_T("showOnTaskbar"), IDC_SHOW_ON_TASKBAR_CHECK, PrefletAtom::ControlType::Button, false);
	atoms().add(_T("stayOnTop"), IDC_STAY_ON_TOP_CHECK, PrefletAtom::ControlType::Button, false);
	atoms().add(_T("snapToEdges"), IDC_SNAP_TO_EDGES_CHECK, PrefletAtom::ControlType::Button, true);
	atoms().add(_T("edgeSnapDistance"), IDC_EDGE_SNAP_DISTANCE_EDIT, PrefletAtom::ControlType::Edit, 5);
	atoms().add(_T("markReadOnHide"), IDC_MARK_READ_ON_HIDE_CHECK, PrefletAtom::ControlType::Button, false);
	atoms().add(_T("close3DPopupOnHide"), IDC_CLOSE_3D_POPUP_ON_HIDE_CHECK, PrefletAtom::ControlType::Button, false);
	atoms().readFromRegistry(registryKey);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletWindow::OnPostInitPreflet()
{
	enableControls();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletWindow::enableControls()
{
	BOOL	enabled = static_cast<CButton *>(GetDlgItem(IDC_SNAP_TO_EDGES_CHECK))->GetCheck();
	GetDlgItem(IDC_EDGE_SNAP_DISTANCE_STATIC)->EnableWindow(enabled);
	GetDlgItem(IDC_EDGE_SNAP_DISTANCE_EDIT)->EnableWindow(enabled);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletWindow::OnBnClickedSnapToEdges()
{
	enableControls();
}

// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletWindow.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
