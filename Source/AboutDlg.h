// ---------------------------------------------------------------------------------------------------------------------------------
//           _                 _   _____  _           _     
//     /\   | |               | | |  __ \| |         | |    
//    /  \  | |__   ___  _   _| |_| |  | | | __ _    | |__  
//   / /\ \ | '_ \ / _ \| | | | __| |  | | |/ _` |   | '_ \ 
//  / ____ \| |_) | (_) | |_| | |_| |__| | | (_| | _ | | | |
// /_/    \_\_.__/ \___/ \__,_|\__|_____/|_|\__, |(_)|_| |_|
//                                           __/ |          
//                                          |___/           
//
// 3D Mail Effects (About dialog class)
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 11/16/2000 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2000, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_ABOUTDLG
#define _H_ABOUTDLG

// ---------------------------------------------------------------------------------------------------------------------------------
// Files required by this module
// ---------------------------------------------------------------------------------------------------------------------------------

#include "WebBrowser2.h"

// ---------------------------------------------------------------------------------------------------------------------------------

class	CAboutDlg : public CDialog
{
public:
				CAboutDlg();
				~CAboutDlg();

	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUT };
	CStatic	regText;
	CButton	okButton;

	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAboutDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
		CToolTipCtrl	*toolTip;
		vector<HBITMAP>	bitmaps;

	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // _H_ABOUTDLG
// ---------------------------------------------------------------------------------------------------------------------------------
// AboutDlg.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

