// ---------------------------------------------------------------------------------------------------------------------------------
//  _    _           _ _____  _           _     
// | |  | |         | |  __ \| |         | |    
// | |  | |_ __   __| | |  | | | __ _    | |__  
// | |  | | '_ \ / _` | |  | | |/ _` |   | '_ \ 
// | |__| | |_) | (_| | |__| | | (_| | _ | | | |
//  \____/| .__/ \__,_|_____/|_|\__, |(_)|_| |_|
//        | |                    __/ |          
//        |_|                   |___/           
//
// 3D Mail Effects Updater Dialog
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

#ifndef	_H_UPDDLG
#define _H_UPDDLG
#include <string>
using namespace std;

// ---------------------------------------------------------------------------------------------------------------------------------

class	CUpdDlg : public CDialog
{
public:
				CUpdDlg(CWnd* pParent = NULL);

	//{{AFX_DATA(CUpdDlg)
	enum { IDD = IDD_UPD_DIALOG };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CUpdDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	bool	doRename(const string & programPath, const string & base, const string & ext);

protected:
		HICON		m_hIcon;

	//{{AFX_MSG(CUpdDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // _H_UPDDLG
// ---------------------------------------------------------------------------------------------------------------------------------
// UpdDlg.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

