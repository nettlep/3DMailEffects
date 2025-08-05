// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             _____  _                            
// |  __ \           |  __ \| |                           
// | |__) | ___  __ _| |  | | | __ _      ___ _ __  _ __  
// |  _  / / _ \/ _` | |  | | |/ _` |    / __| '_ \| '_ \ 
// | | \ \|  __/ (_| | |__| | | (_| | _ | (__| |_) | |_) |
// |_|  \_\\___|\__, |_____/|_|\__, |(_) \___| .__/| .__/ 
//               __/ |          __/ |        | |   | |    
//              |___/          |___/         |_|   |_|    
//
// Registration key input dialog box
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 01/09/2001 by Paul Nettle
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
#include "3dme.h"
#include "RegDlg.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(RegDlg, CDialog)
	//{{AFX_MSG_MAP(RegDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	RegDlg::RegDlg(CWnd* pParent /*=NULL*/)
	: CDialog(RegDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(RegDlg)
	regCode = _T("");
	//}}AFX_DATA_INIT
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	RegDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RegDlg)
	DDX_Text(pDX, IDC_REG_CODE, regCode);
	DDV_MaxChars(pDX, regCode, 99);
	//}}AFX_DATA_MAP
}

// ---------------------------------------------------------------------------------------------------------------------------------
// RegDlg.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

