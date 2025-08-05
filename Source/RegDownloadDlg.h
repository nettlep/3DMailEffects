// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             _____                       _                 _ _____  _           _     
// |  __ \           |  __ \                     | |               | |  __ \| |         | |    
// | |__) | ___  __ _| |  | | ___ __      ___ __ | | ___   __ _  __| | |  | | | __ _    | |__  
// |  _  / / _ \/ _` | |  | |/ _ \\ \ /\ / / '_ \| |/ _ \ / _` |/ _` | |  | | |/ _` |   | '_ \ 
// | | \ \|  __/ (_| | |__| | (_) |\ V  V /| | | | | (_) | (_| | (_| | |__| | | (_| | _ | | | |
// |_|  \_\\___|\__, |_____/ \___/  \_/\_/ |_| |_|_|\___/ \__,_|\__,_|_____/|_|\__, |(_)|_| |_|
//               __/ |                                                          __/ |          
//              |___/                                                          |___/           
//
// Description:
//
//   Registered version downloader
//
// Notes:
//
//   Best viewed with 8-character tabs and (at least) 132 columns
//
// History:
//
//   08/21/2001 by Paul Nettle: Original creation
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2001, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_REGDOWNLOADDLG
#define _H_REGDOWNLOADDLG

// ---------------------------------------------------------------------------------------------------------------------------------

class	RegDownloadDlg : public CDialog
{
public:
	RegDownloadDlg(CWnd* pParent = NULL);

	//{{AFX_DATA(RegDownloadDlg)
	enum { IDD = IDD_REG_DOWNLOAD };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(RegDownloadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(RegDownloadDlg)
	afx_msg void OnRegDownloadDlg();
	afx_msg void OnRegWebsite();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _H_REGDOWNLOADDLG
// ---------------------------------------------------------------------------------------------------------------------------------
// RegDownloadDlg.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
