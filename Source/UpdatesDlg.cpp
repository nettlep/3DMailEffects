// ---------------------------------------------------------------------------------------------------------------------------------
//  _    _           _       _            _____  _                            
// | |  | |         | |     | |          |  __ \| |                           
// | |  | |_ __   __| | __ _| |_  ___ ___| |  | | | __ _      ___ _ __  _ __  
// | |  | | '_ \ / _` |/ _` | __|/ _ \ __| |  | | |/ _` |    / __| '_ \| '_ \ 
// | |__| | |_) | (_| | (_| | |_|  __/__ \ |__| | | (_| | _ | (__| |_) | |_) |
//  \____/| .__/ \__,_|\__,_|\__|\___|___/_____/|_|\__, |(_) \___| .__/| .__/ 
//        | |                                       __/ |        | |   | |    
//        |_|                                      |___/         |_|   |_|    
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

#include "stdafx.h"
#if	!defined(_LITE_)
#include "3dme.h"
#include "http.h"
#include "Download.h"
#include "UpdatesDlg.h"
#include "Cab.h"
#include "3DmeDlg.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(UpdatesDlg, CDialog)
	//{{AFX_MSG_MAP(UpdatesDlg)
	ON_BN_CLICKED(IDC_DOWNLOAD, OnDownload)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_BETA, OnCheckBeta)
	ON_WM_ERASEBKGND()
	ON_CBN_SELCHANGE(IDC_VERSION_LIST, OnSelchangeVersionList)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	UpdatesDlg::UpdatesDlg(const bool quiet, const bool autoUpdateFlag, CWnd* pParent /*=NULL*/)
	: _quiet(quiet), _autoUpdateFlag(autoUpdateFlag), CDialog(UpdatesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(UpdatesDlg)
	//}}AFX_DATA_INIT

	restart = false;
	didUpdate = false;
	beta = false;
	eraseShrunk = false;

	updateFont.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_TT_ALWAYS, DEFAULT_QUALITY, FIXED_PITCH|FF_SWISS, NULL);

	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)	forceUpdate = true;
	else						forceUpdate = false;
#if	defined(_BETA_)
	forceUpdate = true;
	beta = true;
#endif // _BETA_
}

// ---------------------------------------------------------------------------------------------------------------------------------

	UpdatesDlg::~UpdatesDlg()
{
	updateFont.DeleteObject();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	UpdatesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(UpdatesDlg)
	DDX_Control(pDX, IDC_VERSION_LIST, versionList);
	DDX_Control(pDX, IDC_CHECK_BETA, checkBetaButton);
	DDX_Control(pDX, IDC_DOWNLOAD, downloadButton);
	DDX_Control(pDX, IDC_UPDATE_TEXT, updateText);
	DDX_Control(pDX, IDC_UPDATE_STATUS, updateStatus);
	//}}AFX_DATA_MAP
}

// ---------------------------------------------------------------------------------------------------------------------------------

static	DWORD CALLBACK richCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	UpdatesDlg	*ptr = (UpdatesDlg *) dwCookie;
	const char	*start = ptr->historyData.c_str() + ptr->streamed;
	int	len = ptr->historyData.length() - ptr->streamed;
	if (len <= 0)
	{
		*pcb = 0;
		return 0;
	}

	int	amountToStream = cb;
	if (amountToStream > len) amountToStream = len;
	ptr->streamed += amountToStream;

	memcpy(pbBuff, start, amountToStream);
	*pcb = amountToStream;
	return 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	UpdatesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Calculate the sizing difference

	CRect	clientRect;
	GetClientRect(clientRect);

	CRect	statusRect;
	updateStatus.GetWindowRect(statusRect);

	resizeDiff = clientRect.Height() - statusRect.Height();
	resizeDiff /= 2;
	resizeDiff -= 10;

	SetTimer(UPDATE_TIMER, 100, NULL);

	if (_quiet)
	{
		// Move the window waaaay offscreen to prevent it from appearing

		CRect	r;
		GetWindowRect(r);
		r.OffsetRect(-10000, -10000);
		MoveWindow(r, FALSE);
	}

	return TRUE;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	UpdatesDlg::OnDownload()
{
	// Figure out which files to download...

	StringList	URLs;
	WStringList	filenames;
	wstring		statusLine = _T("Downloading files needed to upgrade to v") + atow(updates.currentVersion) + _T(" (patch level ") + integerToWString(updates.currentPatchLevel) + _T(")");
	bool		shouldRestart = false;
	unsigned int	currentPatchLevel = getCurrentPatchLevel();

	// Force an update?
	
	if (forceUpdate) currentPatchLevel = 0;

	// Check the updates for which files we need to download

	if (VERSION_STRING != atow(updates.currentVersion) || forceUpdate)
	{
		// Populate the list of all files for this version

		#if	defined(_REGISTERED_)
		URLs.push_back(updates.registeredURL);
		#else
		URLs.push_back(updates.sharewareURL);
		#endif // _REGISTERED_

		// Reset the current patch level to 0

		currentPatchLevel = 0;
		if (updates.restart) shouldRestart = true;
	}

	// Handle any patches that we might need to download...
	{
		for (unsigned int i = currentPatchLevel; i < updates.currentPatchLevel; i++)
		{
			#if	defined(_REGISTERED_)
			const sPatch	&currentPatch = updates.rpatches[i];
			#else
			const sPatch	&currentPatch = updates.spatches[i];
			#endif // _REGISTERED_

			URLs.push_back(currentPatch.url);
			if (currentPatch.restart) shouldRestart = true;
		}
	}

	// Go through and get the filenames from the URLs

	for (StringList::const_iterator i = URLs.begin(); i != URLs.end(); ++i)
	{
		string	thisFile = *i;
		string::size_type	idx = thisFile.find_last_of("/");
		if (idx != string::npos)
		{
			thisFile.erase(0, idx+1);
		}
		else
		{
			char	tmplate[] = "updateXXXXX";
			thisFile = mktemp(tmplate);
		}

		filenames.push_back(getTempDir() + _T("\\") + atow(thisFile));
	}

	// Okay, begin downloading

	Download	dlg(statusLine, URLs, filenames);
	if (dlg.DoModal() == IDOK)
	{
		restart = false;
		didUpdate = false;
		bool	encryptedOK = true;

		updateStatus.SetWindowText(_T("Extracting the new update/patches..."));
		RedrawWindow();
		Sleep(100);

		for (WStringList::const_iterator i = filenames.begin(); i != filenames.end(); ++i)
		{
			wstring	res = DecryptCabFile(*i);
			if (res.length())
			{
				res = _T("Unable to extract new update:\n\n") + res;
				AfxMessageBox(res.c_str());
				encryptedOK = false;
				break;
			}
			else
			{
				_wunlink(i->c_str());
			}
		}

		if (encryptedOK)
		{
			restart = shouldRestart ? true:false;
			didUpdate = true;

			// Update our current patch level

			setCurrentPatchLevel(updates.currentPatchLevel);

#if	!defined(_BETA_)
			if (beta)
			{
				MessageBox(	_T("You will soon be running a beta copy (you can verify this by looking\n")
						_T("in the about box after the program restarts.) If you wish to revert\n")
						_T("back to a public release version, simply do a normal update.\n"),
						_T("Beta update notice"), MB_OK);
				restart = true;
			}
#endif // !_BETA_

			// Always check for updates

			if (!restart)
			{
				string	pd;
				getUpdatePage(pd);
			}

			PostMessage(WM_COMMAND, IDOK);
		}
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	UpdatesDlg::OnCheckBeta() 
{
#if	!defined(_BETA_)
	if (!beta) // if we're not currently looking at beta information, see if they really want to...
	if (MessageBox(	_T("Please note that although the beta software is not guaranteed to\n")
			_T("be stable. It's not even guaranteed to work. It may destroy your\n")
			_T("hard drive, catch your barn on fire, and cause your neighbor to\n")
			_T("go into convulsions.\n")
			_T("\n")
			_T("On the other hand, there is also a good chance that the beta\n")
			_T("software will run properly.\n")
			_T("\n")
			_T("You decide.\n")
			_T("\n")
			_T("Do you want to continue checking for the availability of a beta?"),
			_T("Beta warning notice!"), MB_YESNO) == IDNO) return;
#endif // !_BETA_
	beta = !beta;
	shrinkWindow();
	SetTimer(UPDATE_TIMER, 100, NULL);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	UpdatesDlg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == UPDATE_TIMER)
	{
		// Kill the timer

		KillTimer(UPDATE_TIMER);

		// Get my current patch level

		unsigned int	currentPatchLevel = getCurrentPatchLevel();

		// Get the update information

		string	pageData;
		if (getUpdatePage(pageData) && pageData.length())
		{
			// Parse it

			updates = parseUpdates(pageData);

			// Latest version?

			bool	newVersion = false;
#if	defined(_BETA_)
			if (atow(updates.currentVersion) == (wstring) BETA_VERSION_STRING && currentPatchLevel == updates.currentPatchLevel)
#else
			if (atow(updates.currentVersion) == (wstring) VERSION_STRING && currentPatchLevel == updates.currentPatchLevel)
#endif // _BETA_
			{
				if (beta)	updateStatus.SetWindowText(_T("You are currently running the most recent beta of ") PROGRAM_NAME_STRING);
				else		updateStatus.SetWindowText(_T("You are currently running the most recent version of ") PROGRAM_NAME_STRING);
			}
			else
			{
				newVersion = true;
				wstring		updateText;
#if	defined(_BETA_)
				if (beta)	updateText = _T("A new beta of ") PROGRAM_NAME_STRING _T(" is available for download");
				else		updateText = _T("A version of ") PROGRAM_NAME_STRING _T(" is available for download");
#else
				if (beta)	updateText = _T("A beta of ") PROGRAM_NAME_STRING _T(" is available for download");
				else		updateText = _T("A new version of ") PROGRAM_NAME_STRING _T(" is available for download");
#endif // _BETA_
				updateText += _T(": ") + atow(updates.currentVersion) + _T(" (patch level ") + integerToWString(updates.currentPatchLevel) + _T(")");
				updateStatus.SetWindowText(updateText.c_str());
			}

			// Enable/Disable the download button

			if (forceUpdate || newVersion)	downloadButton.EnableWindow(TRUE);
			else				downloadButton.EnableWindow(FALSE);

			// Don't notify them of a new version if they don't want it

			if (_quiet && !_autoUpdateFlag)
			{
				newVersion = false;
			}

			// If there isn't a new version, bail now

			if (_quiet && newVersion == false)
			{
				PostMessage(WM_COMMAND, IDOK);
			}
			else
			{
				// Okay, there is a new version, show them the window

				((C3DmeDlg *) GetParent())->showWindow();

				// Move the window back on screen

				if (_quiet)
				{
					CRect	r;
					GetWindowRect(r);
					r.OffsetRect(10000, 10000);
					MoveWindow(r, FALSE);
				}

				ShowWindow(SW_SHOW);

				// Update the button texts

#if	defined(_BETA_)
				if (beta)	downloadButton.SetWindowText(_T("Get beta"));
				else		downloadButton.SetWindowText(_T("Remove beta"));
#else
				if (beta)	downloadButton.SetWindowText(_T("Get beta"));
				else		downloadButton.SetWindowText(_T("Update me"));
#endif // _BETA_

				if (beta)	checkBetaButton.SetWindowText(_T("Check for non-beta"));
				else		checkBetaButton.SetWindowText(_T("Check for beta"));

				// Restore the window size

				restoreWindow();
				OnSelchangeVersionList();

				// Try to convince them to update

				if (!beta && newVersion)
				{
					AfxMessageBox(	_T("Hold on, pardner!\n")
							_T("\n")
							_T("A new verson of 3DME (or a patch) is available.\n")
							_T("\n")
							_T("Updating 3DME is quick and painless. 3DME does\n")
							_T("all the work for you. You don't even have to run\n")
							_T("a setup program.\n")
							_T("\n")
							_T("People on fast connections can actually get updated\n")
							_T("in as little as 10 seconds!\n")
							_T("\n")
							_T("Just close this window and click on \"Update Me\""));
				}
			}
		}
	}

	CDialog::OnTimer(nIDEvent);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	UpdatesDlg::OnCancel()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	UpdatesDlg::OnOK() 
{
	// Destroy stolen keys...

#if	defined(_REGISTERED_)
	destroyStolen();
#endif // _REGISTERED_
	
	CDialog::OnOK();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	UpdatesDlg::shrinkWindow()
{
	CRect	wr;
	GetWindowRect(wr);
	wr.top    += resizeDiff;
	wr.bottom -= resizeDiff;
	MoveWindow(wr);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	UpdatesDlg::restoreWindow()
{
	CRect	wr;
	GetWindowRect(wr);
	wr.top    -= resizeDiff;
	wr.bottom += resizeDiff;
	MoveWindow(wr);
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	UpdatesDlg::OnEraseBkgnd(CDC* pDC) 
{
	if (!eraseShrunk)
	{
		eraseShrunk = true;
		shrinkWindow();
	}

	return CDialog::OnEraseBkgnd(pDC);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	UpdatesDlg::OnSelchangeVersionList() 
{
	// Prepare...

	updateText.SetFont(&updateFont, TRUE);

	// Make it writable

	updateText.SetOptions(ECOOP_SET, ECO_AUTOWORDSELECTION|ECO_NOHIDESEL|ECO_SAVESEL);

	// Dump it out

	updateText.SetSel(0, -1);
	updateText.Clear();

	// Populate the history data

	historyData.erase();
	if ((int) updates.historyInformation.size() > versionList.GetCurSel())
	{
		historyData.erase();
		int	curSel = versionList.GetCurSel();

		if (curSel >= 0 && curSel < (int) updates.historyInformation.size())
		{
			historyData = updates.historyInformation[curSel];
		}
	}

	// Populate the edit control

	EDITSTREAM	es;
	es.dwCookie = (DWORD) this;
	es.dwError = 0;
	es.pfnCallback = richCallback;
	streamed = 0;
	updateText.StreamIn(SF_TEXT, es);

	// Make it read-only

	updateText.SetOptions(ECOOP_SET, ECO_AUTOWORDSELECTION|ECO_NOHIDESEL|ECO_SAVESEL|ECO_READONLY);

	// Restore the scrollbar to the top

	CHARRANGE	cr;
	cr.cpMax = 0;
	cr.cpMin = 0;
	updateText.SetSel(cr);
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	UpdatesDlg::getUpdatePage(string &pageData)
{
	// Set the URL

	string	url;

	if (beta)
	{
		updateStatus.SetWindowText(_T("Checking for a beta..."));
		allowBackgroundProcessing();
		url = "http://www.3DMailEffects.com/updates/beta.txt";
	}
	else
	{
		updateStatus.SetWindowText(_T("Checking for a software update..."));
		allowBackgroundProcessing();
		url = "http://www.3DMailEffects.com/updates/update.txt";
	}

	// Build the browser string for the query

	string	browser = http::get3DMEBrowserString();

	// Go get the page

	try
	{
		// Read the page

		http	website;
		wstring	err = website.getPage(url, pageData, browser);
		if (err.length()) throw_wstring (err);
	}
	catch(const wstring & err)
	{
		// If this is a silent (automatic) check, don't complain about errors

		if (!_quiet)
		{
			wstring	str;
			str += _T("Unable to check for a new version at this time.\n");
			str += _T("Please try again later.\n");
			str += _T("\n");
			str += _T("While checking for updates to ") PROGRAM_NAME_STRING _T(",\n");
			str += _T("the following error occurred:\n");
			str += _T("\n");
			str += err;
			AfxMessageBox(str.c_str());
		}

		// Close the dialog

		PostMessage(WM_COMMAND, IDOK);
		return false;
	}

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

sUpdate	UpdatesDlg::parseUpdates(const string &in)
{
	// Make a working copy

	string	pageData = in;

	// Our result

	sUpdate	result;
	result.currentPatchLevel = 0;

	// Strip out all \r's

	for(;;)
	{
		string::size_type	idx = pageData.find("\r");
		if (idx == string::npos) break;
		pageData.erase(idx, 1);
	}

	// Clear the drop list

	while(versionList.GetCount()) versionList.DeleteString(0);

	// Convert the file to a string list

	StringList	lines;
	convertToStringList(pageData, lines);

	// Parse the file

	for(StringList::iterator i = lines.begin(); i != lines.end(); ++i)
	{
		string	line = *i;
		stripLeadingWS(line);

		// Blank?

		if (!line.length()) continue;

		// Comment?

		if (line[0] == '#') continue;

		// Part of a history list?

		if (line.substr(0, 2) == ">>")
		{
			line.erase(0, 2);
			if (result.historyInformation.size())
			{
				result.historyInformation.back() += line + "\r\n";
			}
			continue;
		}

		// Control word?

		string::size_type	idx = line.find(":");
		if (idx == string::npos) continue;

		// Get the control word

		string	word = line.substr(0, idx);
		stripTrailingWS(word);

		// Get the data for that word

		string	data = line;
		data.erase(0, idx+1);
		stripLeadingWS(data);
		stripTrailingWS(data);

		if (areEqualNocase(word, "version"))
		{
			result.currentVersion = data;
		}
		else if (areEqualNocase(word, "patches"))
		{
			result.currentPatchLevel = atoi(data.c_str());
		}
		else if (areEqualNocase(word, "restart"))
		{
			result.restart = atoi(data.c_str());
		}
		else if (areEqualNocase(word, "shareware"))
		{
			result.sharewareURL = data;
		}
		else if (areEqualNocase(word, "registered"))
		{
			result.registeredURL = data;
		}
		else if (areEqualNocase(word, "spatch"))
		{
			sPatch	p = parsePatch(data);
			if (p.url.length()) result.spatches.push_back(p);
		}
		else if (areEqualNocase(word, "rpatch"))
		{
			sPatch	p = parsePatch(data);
			if (p.url.length()) result.rpatches.push_back(p);
		}
		else if (areEqualNocase(word, "filesums"))
		{
			result.filesums += data;
		}
		else if (areEqualNocase(word, "history"))
		{
			versionList.AddString(atow(data).c_str());
			result.historyInformation.push_back("");
		}
	}

	// Write the filesums in the registry...

	wchar_t	zzz[90];
	memset(zzz, 0, sizeof(zzz));
	theApp.prefs().other()[_T("binData")].stringValue() = atow(result.filesums);
	theApp.prefs().other().update();

	// Set the current selecion on the drop list

	versionList.SetCurSel(0);

	// Done

	return result;
}

// ---------------------------------------------------------------------------------------------------------------------------------

sPatch	UpdatesDlg::parsePatch(const string &in) const
{
	// Maka a working copy

	string	working = in;

	// Our patch

	sPatch	result;

	// Get the patch level

	string::size_type	idx = working.find(",");
	if (idx == string::npos) return result;
	result.restart = atoi(working.substr(0, idx).c_str());
	working.erase(0, idx+1);
	stripLeadingWS(working);

	// Get the patch level

	idx = working.find(",");
	if (idx == string::npos) return result;
	result.level = atoi(working.substr(0, idx).c_str());
	working.erase(0, idx+1);
	stripLeadingWS(working);

	// The patch size

	idx = working.find(",");
	if (idx == string::npos) return result;
	result.size = atoi(working.substr(0, idx).c_str());
	working.erase(0, idx+1);
	stripLeadingWS(working);

	// The patch URL

	result.url = working;

	// Done

	return result;
}
#endif	// !_LITE_

// ---------------------------------------------------------------------------------------------------------------------------------
// UpdatesDlg.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
