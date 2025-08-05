// ---------------------------------------------------------------------------------------------------------------------------------
//  _______ _        ____   __ _______ _          _____              _____  _           _     
// |__   __(_)      / __ \ / _|__   __| |        |  __ \            |  __ \| |         | |    
//    | |   _ _ __ | |  | | |_   | |  | |__   ___| |  | | __ _ _   _| |  | | | __ _    | |__  
//    | |  | | '_ \| |  | |  _|  | |  | '_ \ / _ \ |  | |/ _` | | | | |  | | |/ _` |   | '_ \ 
//    | |  | | |_) | |__| | |    | |  | | | |  __/ |__| | (_| | |_| | |__| | | (_| | _ | | | |
//    |_|  |_| .__/ \____/|_|    |_|  |_| |_|\___|_____/ \__,_|\__, |_____/|_|\__, |(_)|_| |_|
//           | |                                                __/ |          __/ |          
//           |_|                                               |___/          |___/           
//
// Show a tip of the day
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 12/27/2000 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2000, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_TIPOFTHEDAYDLG
#define _H_TIPOFTHEDAYDLG

// ---------------------------------------------------------------------------------------------------------------------------------
// Files required by this module
// ---------------------------------------------------------------------------------------------------------------------------------

#include "Bitmap.h"

// ---------------------------------------------------------------------------------------------------------------------------------

class	TipOfTheDayDlg : public CDialog
{
public:
				TipOfTheDayDlg(CWnd* pParent = NULL);
				~TipOfTheDayDlg();

	//{{AFX_DATA(TipOfTheDayDlg)
	enum { IDD = IDD_STARTUP_TIPS };
	CButton	okButton;
	CButton	nextTipButton;
	CButton	showTipsButton;
	BOOL	showTips;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(TipOfTheDayDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
		CFont		tipFont;
		Bitmap		background;
		int		tipIndex;
		CRect		textRect;
static	const	int		borderWidth;
static	const	int		lineGap;
static	const	wchar_t	*	tips[];

	//{{AFX_MSG(TipOfTheDayDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnNextTip();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _H_TIPOFTHEDAYDLG
// ---------------------------------------------------------------------------------------------------------------------------------
// TipOfTheDayDlg.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

