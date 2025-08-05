// ---------------------------------------------------------------------------------------------------------------------------------
//           _                 _   _____  _                            
//     /\   | |               | | |  __ \| |                           
//    /  \  | |__   ___  _   _| |_| |  | | | __ _      ___ _ __  _ __  
//   / /\ \ | '_ \ / _ \| | | | __| |  | | |/ _` |    / __| '_ \| '_ \ 
//  / ____ \| |_) | (_) | |_| | |_| |__| | | (_| | _ | (__| |_) | |_) |
// /_/    \_\_.__/ \___/ \__,_|\__|_____/|_|\__, |(_) \___| .__/| .__/ 
//                                           __/ |        | |   | |    
//                                          |___/         |_|   |_|    
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

#include "stdafx.h"
#include "3Dme.h"
#include "AboutDlg.h"
#include "EggDlg.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
	toolTip = NULL;
}

// ---------------------------------------------------------------------------------------------------------------------------------

	CAboutDlg::~CAboutDlg()
{
	for (unsigned int i = 0; i < bitmaps.size(); i++)
	{
		DeleteObject(bitmaps[i]);
	}
	delete toolTip;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_REGTEXT, regText);
	DDX_Control(pDX, IDOK, okButton);
	//}}AFX_DATA_MAP
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	bitmaps.push_back(LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BIG_CHECK)));
	GetDlgItem(IDOK)->SendMessage(BM_SETIMAGE, (WPARAM) IMAGE_BITMAP, (LPARAM) bitmaps[0]);

	// Tooltips

	toolTip = new CToolTipCtrl;

	if(toolTip && toolTip->Create(this))
	{
		toolTip->AddTool(&okButton,_T("Close this window"));
		toolTip->Activate(TRUE);
	}

#if	defined(_REGISTERED_)
	wstring	tempString = _T("R E G I S T E R E D\nThank you for your support.");
#elif	defined(_LITE_)
	wstring	tempString = _T("Lite version. Visit the 3DME website for\ninformation on purchasing the full version.");
#else
	wstring	tempString = _T("THIS PRODUCT IS NOT REGISTERED\nPlease register it to unlock its full functionality.");
#endif // _REGISTERED_

#if	defined(_BETA_)
	tempString += _T("\n* * * * * B E T A * * * * *");
#endif // _BETA_

#if	!defined(_LITE_)
	wstring	appString = _T("About ") APP_STRING _T(" (patch level ") + integerToWString(getCurrentPatchLevel()) + _T(")");
#else
	wstring	appString = _T("About ") APP_STRING;
#endif // !_LITE_

	SetWindowText(appString.c_str());

	regText.SetWindowText(tempString.c_str());

	wchar_t	buf[MAX_PATH];
	GetTempPath(MAX_PATH - 1, buf);

	// Our About page uses an image, which is also in the resources...
	{
		wstring	tempFilename = wstring(buf) + _T("logo.jpg");

		HRSRC	hrsc = FindResource(NULL, MAKEINTRESOURCE(IDR_RT_COMPANYLOGO), RT_RCDATA);
		if (!hrsc)
		{
			AfxMessageBox(_T("Unable to find help resource"));
			return TRUE;
		}

		DWORD	helpSize = SizeofResource(NULL, hrsc);
		if (!helpSize)
		{
			AfxMessageBox(_T("Unable to locate help resource"));
			return TRUE;
		}

		HGLOBAL	hHelp = LoadResource(NULL, hrsc);
		if (!hHelp)
		{
			AfxMessageBox(_T("Unable to load help resource"));
			return TRUE;
		}

		void *	helpMem = LockResource(hHelp);
		if (!helpMem)
		{
			AfxMessageBox(_T("Unable to lock help resource"));
			return TRUE;
		}

		FILE *	fp = _wfopen(tempFilename.c_str(), _T("wb"));
		if (fp)
		{
			fwrite(helpMem, helpSize, 1, fp);
			fclose(fp);
		}
	}

	// Create and view the HTML page
	{
		wstring	tempFilename = wstring(buf) + _T("3DMEAboutBox.html");

		HRSRC	hrsc = FindResource(NULL, MAKEINTRESOURCE(IDR_ABOUT), RT_HTML);
		if (!hrsc)
		{
			AfxMessageBox(_T("Unable to find HTML resource"));
			return TRUE;
		}

		DWORD	helpSize = SizeofResource(NULL, hrsc);
		if (!helpSize)
		{
			AfxMessageBox(_T("Unable to locate HTML resource"));
			return TRUE;
		}

		HGLOBAL	hHelp = LoadResource(NULL, hrsc);
		if (!hHelp)
		{
			AfxMessageBox(_T("Unable to load HTML resource"));
			return TRUE;
		}

		void *	helpMem = LockResource(hHelp);
		if (!helpMem)
		{
			AfxMessageBox(_T("Unable to lock HTML resource"));
			return TRUE;
		}

		FILE *	fp = _wfopen(tempFilename.c_str(), _T("wb"));
		if (fp)
		{
			fwrite(helpMem, helpSize, 1, fp);
			fclose(fp);

			static	wstring	url;
			url = _T("file://") + tempFilename + _T("/");

			WebBrowser2 * browser = (WebBrowser2 *) GetDlgItem(IDC_ABOUT_BROWSER);
			browser->Navigate(url.c_str(), NULL, NULL, NULL, NULL);
		}
	}

	return TRUE;
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	CAboutDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (NULL != toolTip) toolTip->RelayEvent(pMsg);
	
	return CDialog::PreTranslateMessage(pMsg);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void CAboutDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
static	count;
	count++;
		
	if (count >= 5)
	{
		count = 0;
		EggDlg	dlg;
		dlg.DoModal();
	}

	CDialog::OnLButtonDown(nFlags, point);
}

// ---------------------------------------------------------------------------------------------------------------------------------
// AboutDlg.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
