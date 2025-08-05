// ---------------------------------------------------------------------------------------------------------------------------------
//  ______                 _ _ _____                  _               _____  _           _     
// |  ____|               (_) |  __ \                (_)             |  __ \| |         | |    
// | |__   _ __ ___   __ _ _| | |__) |_ __  _____   ___  _____      __ |  | | | __ _    | |__  
// |  __| | '_ ` _ \ / _` | | |  ___/| '__|/ _ \ \ / / |/ _ \ \ /\ / / |  | | |/ _` |   | '_ \ 
// | |____| | | | | | (_| | | | |    | |  |  __/\ V /| |  __/\ V  V /| |__| | | (_| | _ | | | |
// |______|_| |_| |_|\__,_|_|_|_|    |_|   \___| \_/ |_|\___| \_/\_/ |_____/|_|\__, |(_)|_| |_|
//                                                                              __/ |          
//                                                                             |___/           
//
// Email Preview Dialog
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 12/26/2000 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2000, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------
//{{AFX_INCLUDES()
#include "webbrowser2.h"
//}}AFX_INCLUDES

#ifndef	_H_EMAILPREVIEWDLG
#define _H_EMAILPREVIEWDLG

// ---------------------------------------------------------------------------------------------------------------------------------
// Files required by this module
// ---------------------------------------------------------------------------------------------------------------------------------

#include "AccountList.h"
#include "afxwin.h"

// ---------------------------------------------------------------------------------------------------------------------------------
// Forward declarations
// ---------------------------------------------------------------------------------------------------------------------------------

class	C3DmeDlg;

// ---------------------------------------------------------------------------------------------------------------------------------

class	EmailPreview : public CDialog
{
public:
				EmailPreview(C3DmeDlg *dlg, const sAccount &account, const sEmail &email, CWnd* pParent = NULL);
				~EmailPreview();

inline	const	string		&body() const {return _body;}
inline		string		&body() {return _body;}

inline	const	sAccount	&account() const {return _account;}
inline	const	sEmail		&email() const {return _email;}

	//{{AFX_DATA(EmailPreview)
	enum { IDD = IDD_PREVIEW_DIALOG };
	CStatic	statusStatic;
	CButton	linkifyCheckbox;
	CButton	wrapCheckbox;
	CStatic	urlStatic;
	CButton	stopButton;
	CButton	saveButton;
	CButton	refreshButton;
	CButton	homeButton;
	CButton	forwardButton;
	CButton	backButton;
	CComboBox	multipartCombo;
	CButton	closeButton;
	CButton	deleteButton;
	CEdit fromEdit;
	CEdit replyToEdit;
	CEdit dateEdit;
	CEdit subjectEdit;
	CStatic fromLabel;
	CStatic replyToLabel;
	CStatic dateLabel;
	CStatic subjectLabel;
	CStatic mimePartLabel;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(EmailPreview)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
		CToolTipCtrl	*toolTip;
		vector<HBITMAP>	bitmaps;
		CFont		headerFont;
		C3DmeDlg	&parent;
		sAccount	_account;
		sEmail		_email;
		string		_body;
		wstring		lastURL;
		bool		lastEnableState;
		string		lastStatusText;
		bool		sized;

		void		populatePreviewText();

	//{{AFX_MSG(EmailPreview)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnDelete();
	afx_msg void OnClose();
	afx_msg void OnSelchangeMultipartCombo();
	afx_msg void OnSaveAttach();
	afx_msg void OnBack();
	afx_msg void OnForward();
	afx_msg void OnStop();
	afx_msg void OnRefresh();
	afx_msg void OnHome();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnWrap();
	afx_msg void OnLinkify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _H_EMAILPREVIEWDLG
// ---------------------------------------------------------------------------------------------------------------------------------
// EmailPreviewDlg.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

