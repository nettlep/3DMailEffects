// ---------------------------------------------------------------------------------------------------------------------------------
//  _____                       _                 _     _     
// |  __ \                     | |               | |   | |    
// | |  | | ___ __      ___ __ | | ___   __ _  __| |   | |__  
// | |  | |/ _ \\ \ /\ / / '_ \| |/ _ \ / _` |/ _` |   | '_ \ 
// | |__| | (_) |\ V  V /| | | | | (_) | (_| | (_| | _ | | | |
// |_____/ \___/  \_/\_/ |_| |_|_|\___/ \__,_|\__,_|(_)|_| |_|
//                                                            
//                                                            
//
// Update file downloader
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 02/09/2001 by Paul Nettle
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
#ifndef	_H_DOWNLOAD
#define _H_DOWNLOAD

// ---------------------------------------------------------------------------------------------------------------------------------
// Files required by this module
// ---------------------------------------------------------------------------------------------------------------------------------

#include "UpdatesDlg.h"

// ---------------------------------------------------------------------------------------------------------------------------------
// Forward declarations
// ---------------------------------------------------------------------------------------------------------------------------------

class	DownloadThread;

// ---------------------------------------------------------------------------------------------------------------------------------

class	Download : public CDialog
{
public:
					Download(const wstring statusLine, const StringList & URLs, const WStringList & filenames, CWnd* pParent = NULL);

	// Accessors

inline		time_t &		startTime()			{return _startTime;}
inline	const	time_t			startTime() const		{return _startTime;}
inline		wstring &		statusLine()			{return _statusLine;}
inline	const	wstring &		statusLine() const		{return _statusLine;}
inline		StringList &		URLs()				{return _URLs;}
inline	const	StringList &		URLs() const			{return _URLs;}
inline		WStringList &		filenames()			{return _filenames;}
inline	const	WStringList &		filenames() const		{return _filenames;}
inline		unsigned int &		totalFilesToDownload()		{return _totalFilesToDownload;}
inline	const	unsigned int		totalFilesToDownload() const	{return _totalFilesToDownload;}
inline		unsigned int &		currentFileToDownload()		{return _currentFileToDownload;}
inline	const	unsigned int		currentFileToDownload()	const	{return _currentFileToDownload;}
inline		DownloadThread *&	downloadThread()		{return _downloadThread;}
inline	const	DownloadThread *	downloadThread() const		{return _downloadThread;}

	//{{AFX_DATA(Download)
	enum { IDD = IDD_DOWNLOAD };
	CStatic	currentFileStatic;
	CProgressCtrl	overallProgress;
	CProgressCtrl	fileProgress;
	CStatic	averageRateStatic;
	CStatic	downloadUrlStatic;
	CStatic	timeLeftStatic;
	CStatic	timeStatic;
	CStatic	statusStatic;
	CStatic	sizeStatic;
	CStatic	rateStatic;
	CStatic	sizeRemainStatic;
	CSliderCtrl	speedSlider;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(Download)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
		void			cancelDownload();

	//{{AFX_MSG(Download)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

		time_t			_startTime;
		wstring			_statusLine;
		StringList		_URLs;
		WStringList		_filenames;
		unsigned int		_totalFilesToDownload;
		unsigned int		_currentFileToDownload;
		DownloadThread *	_downloadThread;
static	const	unsigned int		downloadTimer;
};

//{{AFX_INSERT_LOCATION}}

#endif // _H_DOWNLOAD
#endif // !_LITE_
// ---------------------------------------------------------------------------------------------------------------------------------
// Download.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

