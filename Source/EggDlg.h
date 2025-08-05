// ---------------------------------------------------------------------------------------------------------------------------------
//  ______             _____  _           _     
// |  ____|           |  __ \| |         | |    
// | |__    __ _  __ _| |  | | | __ _    | |__  
// |  __|  / _` |/ _` | |  | | |/ _` |   | '_ \ 
// | |____| (_| | (_| | |__| | | (_| | _ | | | |
// |______|\__, |\__, |_____/|_|\__, |(_)|_| |_|
//          __/ | __/ |          __/ |          
//         |___/ |___/          |___/           
//
// Lame easter egg
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 01/03/2001 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2001, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_EGGDLG
#define _H_EGGDLG

// ---------------------------------------------------------------------------------------------------------------------------------

class	EggDlg : public CDialog
{
public:
					EggDlg(CWnd* pParent = NULL);

	//{{AFX_DATA(EggDlg)
	enum { IDD = IDD_EGG };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(EggDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(EggDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _H_EGGDLG
// ---------------------------------------------------------------------------------------------------------------------------------
// EggDlg.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

