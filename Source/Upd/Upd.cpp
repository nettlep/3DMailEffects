// ---------------------------------------------------------------------------------------------------------------------------------
//  _    _           _                      
// | |  | |         | |                     
// | |  | |_ __   __| |     ___ _ __  _ __  
// | |  | | '_ \ / _` |    / __| '_ \| '_ \ 
// | |__| | |_) | (_| | _ | (__| |_) | |_) |
//  \____/| .__/ \__,_|(_) \___| .__/| .__/ 
//        | |                  | |   | |    
//        |_|                  |_|   |_|    
//
// 3D Mail Effects Updater
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 02/10/2001 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2001, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "Upd.h"
#include "UpdDlg.h"
#include <string>
using namespace std;

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CUpdApp, CWinApp)
	//{{AFX_MSG_MAP(CUpdApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

// --------------------------------------------------------------------------------------------------------------------------------

	CUpdApp::CUpdApp()
{
}

// --------------------------------------------------------------------------------------------------------------------------------

CUpdApp theApp;

// --------------------------------------------------------------------------------------------------------------------------------

BOOL	CUpdApp::InitInstance()
{
//	Enable3dControlsStatic();

	string	cmd = GetCommandLine();

	if (cmd.find("autorun") == string::npos)
	{
		MessageBox(NULL, "This program is part of the 3D Mail Effects auto-\nupdate procedure and should not be run manually.", "3D Mail Effects auto-update", MB_OK|MB_ICONWARNING);
	}
	else
	{
		CUpdDlg dlg;
		dlg.DoModal();
	}

	return FALSE;
}

// ---------------------------------------------------------------------------------------------------------------------------------
// Upd.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

