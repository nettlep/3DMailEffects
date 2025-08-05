// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             _____  _           _     
// |  __ \           |  __ \| |         | |    
// | |__) | ___  __ _| |  | | | __ _    | |__  
// |  _  / / _ \/ _` | |  | | |/ _` |   | '_ \ 
// | | \ \|  __/ (_| | |__| | | (_| | _ | | | |
// |_|  \_\\___|\__, |_____/|_|\__, |(_)|_| |_|
//               __/ |          __/ |          
//              |___/          |___/           
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

#ifndef	_H_REGDLG
#define _H_REGDLG

// ---------------------------------------------------------------------------------------------------------------------------------

class	RegDlg : public CDialog
{
public:
				RegDlg(CWnd* pParent = NULL);

	//{{AFX_DATA(RegDlg)
	enum { IDD = IDD_REG };
	CString	regCode;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(RegDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(RegDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _H_REGDLG
// ---------------------------------------------------------------------------------------------------------------------------------
// RegDlg.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

