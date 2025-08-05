// ---------------------------------------------------------------------------------------------------------------------------------
//  _    _           _       _            _____  _           _     
// | |  | |         | |     | |          |  __ \| |         | |    
// | |  | |_ __   __| | __ _| |_  ___ ___| |  | | | __ _    | |__  
// | |  | | '_ \ / _` |/ _` | __|/ _ \ __| |  | | |/ _` |   | '_ \ 
// | |__| | |_) | (_| | (_| | |_|  __/__ \ |__| | | (_| | _ | | | |
//  \____/| .__/ \__,_|\__,_|\__|\___|___/_____/|_|\__, |(_)|_| |_|
//        | |                                       __/ |          
//        |_|                                      |___/           
//
// Update checker for 3DME
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 02/05/2001 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2001, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#if	!defined(_LITE_)
#ifndef	_H_UPDATESDLG
#define _H_UPDATESDLG

// ---------------------------------------------------------------------------------------------------------------------------------

typedef	struct	tag_patch
{
	unsigned int	restart;
	unsigned int	level;
	unsigned int	size;
	string		url;
} sPatch;

// ---------------------------------------------------------------------------------------------------------------------------------

typedef	struct tag_update
{
	string		currentVersion;
	unsigned int	currentPatchLevel;
	unsigned int	restart;

	string		sharewareURL;
	string		registeredURL;

	string		filesums;

	vector<string>	historyInformation;

	vector<sPatch>	spatches;
	vector<sPatch>	rpatches;
} sUpdate;

// ---------------------------------------------------------------------------------------------------------------------------------

class	UpdatesDlg : public CDialog
{
public:
				UpdatesDlg(const bool quiet, const bool autoUpdateFlag, CWnd* pParent = NULL);
virtual				~UpdatesDlg();
		string		historyData;
		int		streamed;
		bool		restart;
		bool		didUpdate;

	//{{AFX_DATA(UpdatesDlg)
	enum { IDD = IDD_VERSION_CHECKER };
	CComboBox	versionList;
	CButton	checkBetaButton;
	CButton	downloadButton;
	CRichEditCtrl	updateText;
	CStatic	updateStatus;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(UpdatesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
		enum		{UPDATE_TIMER = 54321};

		void		shrinkWindow();
		void		restoreWindow();

		unsigned int	resizeDiff;
		bool		_quiet;
		bool		_autoUpdateFlag;
		bool		forceUpdate;
		bool		beta;
		bool		eraseShrunk;
		CFont		updateFont;

		sUpdate		updates;

		bool		getUpdatePage(string &pageData);
		sUpdate		parseUpdates(const string &in);
		sPatch		parsePatch(const string &in) const;

	//{{AFX_MSG(UpdatesDlg)
	afx_msg void OnDownload();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCancel();
	afx_msg void OnCheckBeta();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSelchangeVersionList();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _H_UPDATESDLG
#endif // !_LITE_
// ---------------------------------------------------------------------------------------------------------------------------------
// UpdatesDlg.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

