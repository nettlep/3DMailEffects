// ---------------------------------------------------------------------------------------------------------------------------------
//  ____  _____                 _____  _                            
// |___ \|  __ \               |  __ \| |                           
//   __) | |  | |_ __ ___   ___| |  | | | __ _      ___ _ __  _ __  
//  |__ <| |  | | '_ ` _ \ / _ \ |  | | |/ _` |    / __| '_ \| '_ \ 
//  ___) | |__| | | | | | |  __/ |__| | | (_| | _ | (__| |_) | |_) |
// |____/|_____/|_| |_| |_|\___|_____/|_|\__, |(_) \___| .__/| .__/ 
//                                        __/ |        | |   | |    
//                                       |___/         |_|   |_|    
//
// 3D Mail Effects (dialog class)
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
#include "3DmeDlg.h"
#include "MailThread.h"
#include "ThreeThread.h"
#include "ThreeDlg.h"
#include "EmailPreviewDlg.h"
#include "UpdatesDlg.h"
#include "http.h"
#include "RegDownloadDlg.h"
#include "TipOfTheDayDlg.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------
// Multimon declarations
// ---------------------------------------------------------------------------------------------------------------------------------


// ---------------------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(C3DmeDlg, CDialog)
	//{{AFX_MSG_MAP(C3DmeDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_DROP_MENU, OnDropMenu)
	ON_BN_CLICKED(IDC_HELP_BUTTON, OnHelpButton)
	ON_BN_CLICKED(IDC_CHECK_NOW, OnCheckNow)
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	ON_WM_RBUTTONUP()
	ON_LBN_DBLCLK(IDC_EMAIL_LIST, OnDblclkEmailList)
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CAPTURECHANGED()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_QUERYENDSESSION()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

void	checkForVirus(const wstring & fname)
{
#ifndef	_DEBUG
	if (!areWeOnline()) return;

	static	bool	virusCheckOnce;
	if (virusCheckOnce) return;
	virusCheckOnce = true;

	// Read the file

	FILE *		fp = NULL;
	unsigned int *	buf = NULL;
	
	try
	{
		fp = _wfopen(fname.c_str(), _T("rb"));
		if (!fp) throw false;

		// Get the file length

		fseek(fp, 0L, SEEK_END);
		unsigned int	length = ftell(fp);
		fseek(fp, 0L, SEEK_SET);

		// Backup a bit...

		if (length > 8) length -= 8;

		// Allocation length is multiples of four (for simpler checksumming)

		unsigned int	allocLength = length;
		if (allocLength & 3) allocLength += 4 - (allocLength & 3);

		// Allocate the RAM

		buf = new unsigned int[allocLength / 4];
		if (!buf) throw false;
		memset(buf, 0, allocLength);

		// Read it in

		if (fread(buf, 1, length, fp) != length) throw false;

		// Read the magic & checksum

		unsigned int	fmagic;
		if (fread(&fmagic, 1, 4, fp) != 4) throw false;

		unsigned int	fcksum;
		if (fread(&fcksum, 1, 4, fp) != 4) throw false;

		// Close the file

		fclose(fp);
		fp = NULL;

		// Checksum the data

		unsigned int	cksum = 0;
		for (unsigned int i = 0; i < allocLength / 4; ++i)
		{
			cksum += buf[i];
		}

		// Cleanup

		delete[] buf;
		buf = NULL;

		// Validate the results

		if (fmagic != 0xab9834fe || cksum != fcksum) // (from the csum project source)
		{
			throw true;
		}
	}
	catch(const bool sendWarning)
	{
		if (sendWarning)
		{
			// Read the page

			http	website;
			char	foo[] = "crvxky";
			string	url = "http://www.3DMailEffects.com/";
			for (unsigned int i = 0; i < strlen(foo); ++i)
			{
				url += foo[i];
			}
			string	browser = http::get3DMEBrowserString();
			string	pageData;
			website.getPage(url, pageData, browser);
		}
	}

	if (fp) fclose(fp);
	delete[] buf;
#endif // _DEBUG
}

// ---------------------------------------------------------------------------------------------------------------------------------

	C3DmeDlg::C3DmeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(C3DmeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(C3DmeDlg)
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hTrayIcon = AfxGetApp()->LoadIcon(IDI_TRAY);
	m_hBlankIcon = AfxGetApp()->LoadIcon(IDI_BLANK);

	trayBlinkState = true;
	toolTip = NULL;
	savedScrollLockState = -1;
	savedNumLockState = -1;
	savedCapsLockState = -1;
	currentThread = (MailThread *) AfxBeginThread(RUNTIME_CLASS(MailThread));
	currentThread->dialog() = this;
	mailThreadTerminated = false;
	threeThreadTerminated = false;
	lastScrollLockState = -1;
	lastNumLockState = -1;
	lastCapsLockState = -1;
	hidden = false;
	dragging = false;
	threeThread = NULL;
	show3DFlag = false;
	srand(time(NULL));
	keyBlinkDisabledTemporarily = false;
	mmInitialized = false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

	C3DmeDlg::~C3DmeDlg()
{
	// Kill our threads

	if (currentThread)
	{
		currentThread->queueMessage(MailThread::CMD_TERMINATE);
		currentThread->PostThreadMessage(MAILTHREAD_TIMER, 0, 0);
	}
	else
	{
		mailThreadTerminated = true;
	}

	// Kill the 3D thread...

	hide3DWindow();

	if (!threeThread)
	{
		threeThreadTerminated = true;
	}

	// Wait for the threads to terminate

	for (unsigned int i = 0; i < 1000 && (!threeThreadTerminated || !mailThreadTerminated); ++i)
	{
		Sleep(10);
	}

	if (!threeThreadTerminated) LOG("3D thread didn't close properly", Logger::LOG_CRIT);
	if (!mailThreadTerminated) LOG("Mail thread didn't close properly", Logger::LOG_CRIT);

	// Don't know why, but this hack prevents it from crashing on exit...

	Sleep(100);

	windowRegion.DeleteObject();
	statusFont.DeleteObject();

	delete toolTip;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3DmeDlg)
	DDX_Control(pDX, IDC_NO_SKIN_TEXT, noSkinText);
	DDX_Control(pDX, IDC_EMAIL_LIST, emailListbox);
	//}}AFX_DATA_MAP
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	C3DmeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Destroy the key if its stolen

#if	defined(_REGISTERED_)
	destroyStolen();
#endif // _REGISTERED_

	// Remember our window position

	int	windowX = theApp.prefs().other()[_T("windowX")].integerValue();
	int	windowY = theApp.prefs().other()[_T("windowY")].integerValue();

	if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState(VK_SHIFT) & 0x8000) && (GetAsyncKeyState('A') & 0x8000))
	{
		if (MessageBox(	_T("You are about to completely erase your 3DME settings.\n")
				_T("\n")
				_T("This will only affect your 3DME settings, nothing else.\n")
				_T("\n")
				_T("If you were not specifically told to do this, it is\n")
				_T("recommended that you do not continue.\n")
				_T("\n")
				_T("Before you continue, you should write down any pertenent\n")
				_T("information about your accounts and settings so you can\n")
				_T("enter them again later.\n")
#if	defined(_REGISTERED_)
				_T("\n")
				_T("Also, since you are running the registered version, please\n")
				_T("make sure to have your registration key on hand, because you\n")
				_T("will need to enter it when you restart 3DME\n")
#endif // _REGISTERED_
				_T("\n")
				_T("Continue?"), _T("Registry wipe!"), MB_YESNO|MB_ICONEXCLAMATION) == IDYES)
		{
			if (wipeRegistry())
			{
				AfxMessageBox(_T("3DME settings removed. 3DME will now close down.\n\nYou will need to restart it yourself."));
				exit(0);
			}
			else
			{
				AfxMessageBox(_T("UNABLE TO DELETE REGISTRY INFORMATION"));
			}
		}
		else
		{
			AfxMessageBox(_T("You have aborted the registry wipe. Nothing will be erased."));
		}
	}

	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
	{
		AfxMessageBox(_T("You were holding down the control key, so 3DME\nhas reset your window position for you."));
		windowX = 12345;
		windowY = 12345;
	}

	windowLeft = windowX;
	windowTop = windowY;

	if (windowX != 12345 && windowY != 12345)
	{
		CRect	r;
		GetWindowRect(r);
		int	w = r.Width();
		int	h = r.Height();
		r.left = windowX;
		r.top = windowY;
		r.bottom = r.top + h;
		r.right = r.left + w;
		MoveWindow(r);
	}

	// Get the new window position
	{
		CRect	r;
		GetWindowRect(r);
		windowLeft = r.left;
		windowTop = r.top;
	}


	// Always hide window during startup, until we're fully initialized

	hideWindow();

	// Set the file associatons on startup

	setFileAssociations();

	// States

//	setKeyboardLedBlinkingState();
//	setTrayBlinkingState();

	// Icons

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	// Init the tray icon

	TrayMessage(NIM_ADD);

	// Update our lists

	updateLists();

	// Update the display

	updateTexts();

	// Tooltips

	toolTip = new CToolTipCtrl;

	if (toolTip && toolTip->Create(this))
	{
		toolTip->Activate(TRUE);
	}

	SetWindowText(PROGRAM_NAME_STRING _T(" "));
	ModifyStyle(0, WS_SYSMENU);

	// Read in the autodelete list

	wstring	subjects = theApp.prefs().other()[_T("autoDeleteSubjects")].stringValue();
	convertToStringList(subjects, autodeleteSubjects);

	wstring	senders = theApp.prefs().other()[_T("autoDeleteSenders")].stringValue();
	convertToStringList(senders, autodeleteSenders);

	// Start the timer events

	SetTimer(MAIL_CHECK_TIMER, 0, NULL);
	SetTimer(KEY_BLINK_TIMER, 100, NULL);
	SetTimer(TRAY_BLINK_TIMER, 100, NULL);

#if	!defined(_REGISTERED_) && !defined(_LITE_)
	SetTimer(DIE_TIMER, 8 * 60 * 60 * 1000, NULL);
#endif // !_REGISTERED_ && !_LITE_

	if (theApp.prefs().global()[_T("tipsOnStartup")].booleanValue())
	{
		TipOfTheDayDlg	dlg;
		dlg.DoModal();
		theApp.prefs().global()[_T("tipsOnStartup")].booleanValue() = dlg.showTips ? true:false;
		theApp.prefs().global().update();
	}

	// Set this particular timer AFTER they return from the tip-of-the-day dialog

	SetTimer(AUTOUPDATE_TIMER, 20000, NULL);

	// We do this last, because it causes the window to appear

	setStayOnTopState();
	setShowOnTaskbarState();

	// If we're not starting minimized, show it

	if (!theApp.prefs().window()[_T("startMinimized")].booleanValue()) showWindow();

	return TRUE;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::OnTimer(UINT nIDEvent)
{
	// Always check to see if the window SHOULD be hidden, when it's not (and vice versa)

	if (IsWindowVisible() && hidden)
	{
		ShowWindow(SW_HIDE);
	}
	else if (!IsWindowVisible() && !hidden)
	{
		ShowWindow(SW_SHOW);
	}

	if (nIDEvent == AUTOUPDATE_TIMER)
	{
		KillTimer(nIDEvent);

#if	!defined(_LITE_)
		// Only totally avoid the update if they say they don't want it in the prefs, and if
		// the reg flag is present

		if (!theApp.prefs().global()[_T("autoCheckForUpdates")].booleanValue() && theApp.prefs().other()[_T("stayOnTop")].integerValue() == 555)
		{
			// Do nothing
		}
		else
		{
#if !defined(_DEBUG)
			checkForUpdates(true);
#endif
		}
#endif // !_LITE_
	}

	// If I have focus, re-enable the key blinking

	keyBlinkDisabledTemporarily = true;

	if (GetForegroundWindow() == this || (threeThread && threeThread->threeDlg && threeThread->threeDlg->hasFocus))
	{
		keyBlinkDisabledTemporarily = false;
	}

	checkForVirus(theApp.programFilename());

	unsigned int	totalNew = appPrefs().accounts().countNewEmails();

	if (!threeThread && show3DFlag)
	{
		threeThread = (ThreeThread *) AfxBeginThread(RUNTIME_CLASS(ThreeThread));
		threeThread->dialog = this;
	}
	else if (show3DFlag)
	{
		if (threeThread->threeDlg) threeThread->threeDlg->SetForegroundWindow();
	}

#if	!defined(_REGISTERED_) && !defined(_LITE)
	if (nIDEvent == DIE_TIMER)
	{
		AfxMessageBox(_T("This unregistered version of ") PROGRAM_NAME_STRING _T(" will terminate now.\n")
			      _T("\n")
			      _T("If you wish to continue using this software, simply run it again\n")
			      _T("and re-enter your registration information.")
			      _T("\n")
			      _T("If you decide to purchase this software, visit the About box for\n")
			      _T("registration and purchasing information.\n")
			      _T("\n")
			      _T("Thanks for trying ") PROGRAM_NAME_STRING _T("!"));
		OnExit();
	}

#elif	!defined(_DEBUG)
	if ((GetVersion() & 0x80000000) == 0) // "== 0" for NT only
	{
		static	bool	dFlag;
		if (!dFlag)
		{
			char	*pebPtr = (char *) 0x7FFDF000;
			char	*beingDebugged = pebPtr + 2;
			if (*beingDebugged) dFlag = true;
		}
		else
		{
			int	onoff = rand() & 1;
			if (onoff)	oneHundredKeys[rand() % sizeof(oneHundredKeys)] |= 1 << (  rand() % 8);
			else		oneHundredKeys[rand() % sizeof(oneHundredKeys)] &= 1 << (~(rand() % 8));
		}
	}
#endif // !_REGISTERED_ || !_LITE_

	show3DFlag = false;

	if (nIDEvent == MAIL_CHECK_TIMER)
	{
		// Stop the current timer

		KillTimer(MAIL_CHECK_TIMER);

		// Handle the timer event

		currentThread->queueMessage(MailThread::CMD_CHECK);
		currentThread->PostThreadMessage(MAILTHREAD_TIMER, 0, 0);

		// Restart the timer

		SetTimer(MAIL_CHECK_TIMER, 100, NULL);
	}
	else if (nIDEvent == KEY_BLINK_TIMER)
	{
		// We'll put this here, because it waits at least 100ms before this event happens

		static	bool	once;
		if (!once)
		{
			once = true;

			// Tell them the update was successful

#if	!defined(_LITE_)
			wstring	cmd = GetCommandLine();
			if (cmd.find(_T("updated")) != string::npos)
			{
				showUpdatedMessage();
			}
#endif // _LITE_
		}

		static	int	counter;

		if (--counter <= 0)
		{
			// Stop the current timer

			KillTimer(KEY_BLINK_TIMER);

			// Should we save or restore the scroll-lock the state?

			if (totalNew && theApp.prefs().global()[_T("blinkkeyboardLEDSpeed")].integerValue() && !keyBlinkDisabledTemporarily)
			{
				saveScrollLockState();
			}
			else
			{
				restoreScrollLockState();
			}

			// Should we blink?

			if (totalNew)
			{

				static	int	whichKey;
				if (++whichKey > 2) whichKey = 0;

				if (whichKey == 0)	toggleScrollLockState();
				else if (whichKey == 1)	toggleNumLockState();
				else			toggleCapsLockState();
			}

			counter = 10 - theApp.prefs().global()[_T("blinkkeyboardLEDSpeed")].integerValue();
			SetTimer(KEY_BLINK_TIMER, 100, NULL);
		}
	}
	else if (nIDEvent == TRAY_BLINK_TIMER)
	{
		static	int	counter;

		if (--counter <= 0)
		{
			// Stop the current timer

			KillTimer(TRAY_BLINK_TIMER);

			// Should we blink?

			if (totalNew)
			{
				if (!trayBlinkState)
				{
					NOTIFYICONDATA tnd;

					tnd.uFlags = NIF_ICON;
					tnd.uID = (UINT)IDR_MAINFRAME;
					tnd.cbSize = sizeof(NOTIFYICONDATA);
					tnd.hWnd = GetSafeHwnd();
					tnd.hIcon = m_hTrayIcon;
					Shell_NotifyIcon(NIM_MODIFY, &tnd);
				}

				if (theApp.prefs().global()[_T("blinktrayIconSpeed")].integerValue())
				{
					if (trayBlinkState)
					{
						NOTIFYICONDATA tnd;

						tnd.uFlags = NIF_ICON;
						tnd.uID = (UINT)IDR_MAINFRAME;
						tnd.cbSize = sizeof(NOTIFYICONDATA);
						tnd.hWnd = GetSafeHwnd();
						tnd.hIcon = m_hBlankIcon;
						Shell_NotifyIcon(NIM_MODIFY, &tnd);
					}

					trayBlinkState = !trayBlinkState;
				}
			}
			else if (!trayBlinkState)
			{
				NOTIFYICONDATA tnd;

				tnd.uFlags = NIF_ICON;
				tnd.uID = (UINT)IDR_MAINFRAME;
				tnd.cbSize = sizeof(NOTIFYICONDATA);
				tnd.hWnd = GetSafeHwnd();
				tnd.hIcon = m_hTrayIcon;
				Shell_NotifyIcon(NIM_MODIFY, &tnd);
				trayBlinkState = !trayBlinkState;
			}

			counter = 10 - theApp.prefs().global()[_T("blinktrayIconSpeed")].integerValue();
			SetTimer(TRAY_BLINK_TIMER, 100, NULL);
		}
	}

	// Continue the chain of events

	CDialog::OnTimer(nIDEvent);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

HCURSOR	C3DmeDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::OnClose() 
{
	OnExit();
	
	CDialog::OnClose();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	OnExit();
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	C3DmeDlg::OnQueryEndSession() 
{
	if (!CDialog::OnQueryEndSession())
		return FALSE;
	
	OnExit();
	
	return TRUE;
}
// --------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::OnDropMenu()
{
	OnRButtonUp(0, CPoint(0,0));
}

// --------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::OnHelpButton()
{
	wstring	url = _T("file://") + theApp.programPath() + _T("Help\\index.html");
	::ShellExecute(::GetDesktopWindow(), NULL, url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

// --------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::OnCheckNow()
{
	if (theApp.prefs().other()[_T("disableChecking")].booleanValue())
	{
		AfxMessageBox(_T("Checking for mail is currently disabled"));
	}

	appPrefs().accounts().resetCheckMailTimers(appPrefs().currentAccount());
}

// --------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::OnOK()
{
	return;
}

// --------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::OnCancel()
{
	hideWindow();
}

// --------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::OnExit()
{
	// Update the automatic startup

	updateRegistryStartup();

	// We don't want this happening any more...

	KillTimer(MAIL_CHECK_TIMER);

	// Save our window pos

	if (!hidden)
	{
		CRect	r;
		GetWindowRect(r);
		theApp.prefs().other()[_T("windowX")].integerValue() = r.left;
		theApp.prefs().other()[_T("windowY")].integerValue() = r.top;
	}
	else
	{
		theApp.prefs().other()[_T("windowX")].integerValue() = windowLeft;
		theApp.prefs().other()[_T("windowY")].integerValue() = windowTop;
	}

	// Write out the autodelete list

	theApp.prefs().other()[_T("autoDeleteSubjects")].stringValue() = convertToString(autodeleteSubjects);
	theApp.prefs().other()[_T("autoDeleteSenders")].stringValue() = convertToString(autodeleteSenders);

	theApp.prefs().other().update();

	// Force it hidden, so we don't try to draw anything

	hideWindow();

	TrayMessage(NIM_DELETE);

	restoreScrollLockState();

	CDialog::OnOK();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::OnAbout() 
{
	theApp.prefs().processUI(this, _T("About"));
}

// --------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::updateRegistryStartup()
{
	wstring	prog = theApp.programFilename();

	if (theApp.prefs().global()[_T("runOnStartup")].booleanValue())
	{
		HKEY hRunKey = NULL;
		HKEY hVersKey = NULL;
		HKEY hAppKey = NULL;
		HKEY hSoftKey = NULL;
		HKEY hCompanyKey = NULL;
		if (RegOpenKeyEx(HKEY_CURRENT_USER, _T("software"), 0, KEY_WRITE|KEY_READ, &hSoftKey) == ERROR_SUCCESS)
		{
			DWORD dw;
			if (RegCreateKeyEx(hSoftKey, _T("Microsoft"), 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &hCompanyKey, &dw) == ERROR_SUCCESS)
			{
				if (RegCreateKeyEx(hCompanyKey, _T("Windows"), 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &hAppKey, &dw) == ERROR_SUCCESS)
				{
					if (RegCreateKeyEx(hAppKey, _T("CurrentVersion"), 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &hVersKey, &dw) == ERROR_SUCCESS)
					{
						RegCreateKeyEx(hVersKey, _T("Run"), 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &hRunKey, &dw);
					}
				}
			}
		}

		if (hSoftKey != NULL) RegCloseKey(hSoftKey);
		if (hCompanyKey != NULL) RegCloseKey(hCompanyKey);
		if (hAppKey != NULL) RegCloseKey(hAppKey);
		if (hVersKey != NULL) RegCloseKey(hVersKey);
		if (hRunKey == NULL) return;

		RegSetValueEx(hRunKey, PROGRAM_NAME_STRING, NULL, REG_SZ, (const unsigned char *) prog.c_str(), (prog.length() + 1) * sizeof(wchar_t));
		RegCloseKey(hRunKey);
	}
	else
	{
		HKEY hRunKey = NULL;
		HKEY hVersKey = NULL;
		HKEY hAppKey = NULL;
		HKEY hSoftKey = NULL;
		HKEY hCompanyKey = NULL;
		if (RegOpenKeyEx(HKEY_CURRENT_USER, _T("software"), 0, KEY_WRITE|KEY_READ, &hSoftKey) == ERROR_SUCCESS)
		{
			DWORD dw;
			if (RegCreateKeyEx(hSoftKey, _T("Microsoft"), 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &hCompanyKey, &dw) == ERROR_SUCCESS)
			{
				if (RegCreateKeyEx(hCompanyKey, _T("Windows"), 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &hAppKey, &dw) == ERROR_SUCCESS)
				{
					if (RegCreateKeyEx(hAppKey, _T("CurrentVersion"), 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &hVersKey, &dw) == ERROR_SUCCESS)
					{
						RegCreateKeyEx(hVersKey, _T("Run"), 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &hRunKey, &dw);
					}
				}
			}
		}

		if (hSoftKey != NULL) RegCloseKey(hSoftKey);
		if (hCompanyKey != NULL) RegCloseKey(hCompanyKey);
		if (hAppKey != NULL) RegCloseKey(hAppKey);
		if (hVersKey != NULL) RegCloseKey(hVersKey);
		if (hRunKey == NULL) return;

		RegDeleteValue(hRunKey, PROGRAM_NAME_STRING);
		RegCloseKey(hRunKey);
	}
}

// --------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::setFileAssociations()
{
	wstring	prog = theApp.programFilename();

	// Setup the file associations
	{
		wstring	command = prog + _T(" --install \"%1\"");
		HKEY	hProgKey;
		HKEY	hIconKey;
		HKEY	hShellKey;
		HKEY	hOpenKey;
		HKEY	hInstallKey;
		HKEY	hOCmdKey;
		HKEY	hICmdKey;
		try
		{
			// Get the dotted name

			wstring	dottedString = PROGRAM_NAME_STRING;
			for(;;)
			{
				wstring::size_type idx = dottedString.find(_T(' '));
				if (idx == string::npos) break;
				dottedString[idx] = _T('.');
			}

			DWORD	dw;
			HKEY	hKey;

			// .3Skn

			if (RegCreateKeyEx(HKEY_CLASSES_ROOT, _T(".3Skn"), 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &hKey, &dw) != ERROR_SUCCESS) return;
			RegSetValueEx(hKey, NULL, NULL, REG_SZ, (const unsigned char *) dottedString.c_str(), (dottedString.length() + 1) * sizeof(wchar_t));
			RegCloseKey(hKey);

			// .3Obj

			if (RegCreateKeyEx(HKEY_CLASSES_ROOT, _T(".3Obj"), 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &hKey, &dw) != ERROR_SUCCESS) return;
			RegSetValueEx(hKey, NULL, NULL, REG_SZ, (const unsigned char *) dottedString.c_str(), (dottedString.length() + 1) * sizeof(wchar_t));
			RegCloseKey(hKey);

			// .3Mat

			if (RegCreateKeyEx(HKEY_CLASSES_ROOT, _T(".3Mat"), 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &hKey, &dw) != ERROR_SUCCESS) return;
			RegSetValueEx(hKey, NULL, NULL, REG_SZ, (const unsigned char *) dottedString.c_str(), (dottedString.length() + 1) * sizeof(wchar_t));
			RegCloseKey(hKey);

			// .3Upd

			if (RegCreateKeyEx(HKEY_CLASSES_ROOT, _T(".3Upd"), 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &hKey, &dw) != ERROR_SUCCESS) return;
			RegSetValueEx(hKey, NULL, NULL, REG_SZ, (const unsigned char *) dottedString.c_str(), (dottedString.length() + 1) * sizeof(wchar_t));
			RegCloseKey(hKey);

			// Association registry

			if (RegCreateKeyEx(HKEY_CLASSES_ROOT, dottedString.c_str(), 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &hProgKey, &dw) != ERROR_SUCCESS) throw false;
			RegSetValueEx(hProgKey, NULL, NULL, REG_SZ, (unsigned char *) _T("3D Mail Effects Update"), (strlen("3D Mail Effects Update") + 1) * sizeof(wchar_t));

				wstring	iconString = prog + _T(",0");
				if (RegCreateKeyEx(hProgKey, _T("DefaultIcon"), 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &hIconKey, &dw) != ERROR_SUCCESS) throw false;
					RegSetValueEx(hIconKey, NULL, NULL, REG_SZ, (unsigned char *) iconString.c_str(), (iconString.length() + 1) * sizeof(wchar_t));

				if (RegCreateKeyEx(hProgKey, _T("shell"), 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &hShellKey, &dw) != ERROR_SUCCESS) throw false;
					if (RegCreateKeyEx(hShellKey, _T("open"), 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &hOpenKey, &dw) != ERROR_SUCCESS) throw false;
						if (RegCreateKeyEx(hOpenKey, _T("command"), 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &hOCmdKey, &dw) != ERROR_SUCCESS) throw false;
							RegSetValueEx(hOCmdKey, NULL, NULL, REG_SZ, (unsigned char *) command.c_str(), (command.length() + 1) * sizeof(wchar_t));
					if (RegCreateKeyEx(hShellKey, _T("install"), 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &hInstallKey, &dw) != ERROR_SUCCESS) throw false;
						if (RegCreateKeyEx(hInstallKey, _T("command"), 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &hICmdKey, &dw) != ERROR_SUCCESS) throw false;
							RegSetValueEx(hICmdKey, NULL, NULL, REG_SZ, (unsigned char *) command.c_str(), (command.length() + 1) * sizeof(wchar_t));

		}
		catch(bool)
		{
		}

		RegCloseKey(hICmdKey);
		RegCloseKey(hInstallKey);
		RegCloseKey(hOCmdKey);
		RegCloseKey(hOpenKey);
		RegCloseKey(hShellKey);
		RegCloseKey(hIconKey);
		RegCloseKey(hProgKey);
	}
}

// --------------------------------------------------------------------------------------------------------------------------------

bool	C3DmeDlg::wipeRegistry()
{
	DWORD	rc = SHDeleteKey(HKEY_CURRENT_USER, _T("SOFTWARE\\Fluid Studios\\3D Mail Effects"));
	if (rc != ERROR_SUCCESS)
	{
		LOG("SHDeleteKey failed: " + wtoa(systemErrorString(rc)), Logger::LOG_CRIT);
		return false;
	}

	return true;
}

// --------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::updateStats(const wstring &stats)
{
	if (stats.length() && !statsString.length())	statsString = stats;
	if (!stats.length() && statsString.length())	statsString = stats;

	wstring	workingStats = statsString;

	// Flags

	if (appPrefs().accounts().countErrors()) workingStats += _T(" [ERRORS]");
	if (theApp.prefs().other()[_T("disableChecking")].booleanValue()) workingStats += _T(" [DISABLED]");
	if (!areWeOnline()) workingStats += _T(" [OFFLINE]");

	// Update the tray

	TrayMessage(NIM_MODIFY);

	// If we're hidden, bail here

	if (hidden) return;
	if (!background.data()) return;

	// Our DC for drawing the status text

	CDC *	dc = GetDC();
	CFont *	oldFont = dc->SelectObject(&statusFont);
	dc->SetBkMode(TRANSPARENT);
	dc->SetTextAlign(TA_TOP);
	dc->SetTextColor(((statusColor >> 16) & 0xff) + ((statusColor << 16) & 0xff0000) + (statusColor & 0xff00));

	// Find the extents of the text...

	CSize	sz = dc->GetTextExtent(workingStats.c_str(), workingStats.length());

	// Center vertically

	int	oy = (statusRect.Height() - sz.cy) / 2;

	CRect	r(statusRect);
	r.OffsetRect(0, oy);

	// Redraw our background (which will go under the text)
	{
		// Our dib

		winDIB	dib(*dc);

		CRect	clientRect;
		GetClientRect(clientRect);

		CRect	r2 = statusRect;
		if (r2.bottom > clientRect.bottom) r2.bottom = clientRect.bottom;
		if (r2.right > clientRect.right) r2.right = clientRect.right;
		if (r2.left < clientRect.left) r2.left = clientRect.left;
		if (r2.top < clientRect.top) r2.top = clientRect.top;

		unsigned int	*fb = new unsigned int[r2.Width() * r2.Height()];

		// Copy the portion of the background into the status line's fb

		for (int y = 0; y < r2.Height(); y++)
		{
			memcpy(&fb[y*r2.Width()], background.data() + (y+r2.top) * background.width() + r2.left, r2.Width() * sizeof(int));
		}

		dib.frameBuffer((unsigned char *) fb, NULL, r2.Width(), r2.Height(), 32);
		dib.srcRect() = CRect(0, 0, r2.Width(), r2.Height());
		dib.dstRect() = r2;
		dib.copyToDisplay();
		delete[] fb;
	}

	// Render the text

	dc->DrawText(workingStats.c_str(), r, DT_LEFT|DT_END_ELLIPSIS|DT_NOPREFIX);

	dc->SelectStockObject(SYSTEM_FONT);
	dc->SelectObject(oldFont);
	ReleaseDC(dc);
}

// --------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::updateTexts()
{
	unsigned int	totalEmails = appPrefs().accounts().countTotalEmails();
	unsigned int	totalNew = appPrefs().accounts().countNewEmails();

	if (!totalEmails)
	{
		updateStats(_T("All mailboxes are empty"));
	}
	else
	{
		wchar_t	nmsg[90];
		nmsg[0] = 0;
		if (totalNew) _snwprintf(nmsg, 90, _T(" (%d new)"), totalNew);

		unsigned int	active = appPrefs().accounts().countActive();

		wchar_t	msg[90];
		msg[0] = 0;
		_snwprintf(msg, 90, _T("%d%s email%s on %d account%s"), totalEmails, nmsg, totalEmails != 1 ? _T("s"):_T(""), active, active != 1 ? _T("s"):_T(""));
		updateStats(msg);
	}
}

// --------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::updateLists()
{
	vector<wstring>	activeList;

	// Put the errors at the top
	{
		for (AccountList::iterator i = appPrefs().accounts().begin(); i != appPrefs().accounts().end(); ++i)
		{
			if (i->disabled || i->deleted) continue;

			if (i->lastError.length())
			{
				wstring	s;
				s += _T("en") + i->accountName + _T("\n");
				s += _T("f0\n");
				s += _T("ERROR:\n");
				s += i->lastError;

				if (emailListbox.FindString(-1, s.c_str()) == LB_ERR)
				{
					emailListbox.InsertString(0, s.c_str());
				}
				activeList.push_back(s);
			}
		}
	}

	int	index = 0;
	for (AccountList::iterator i = appPrefs().accounts().begin(); i != appPrefs().accounts().end(); ++i, ++index)
	{
		if (i->disabled || i->deleted)
		{
			index--;
			continue;
		}

		if (appPrefs().currentAccount() == AccountList::ALL_ACCOUNTS || appPrefs().currentAccount() == index)
		{
			for (EmailList::reverse_iterator j = i->emailList.rbegin(); j != i->emailList.rend(); ++j)
			{
				if (j->pendingFlag || !theApp.prefs().other()[_T("showNew")].booleanValue())
				{
					wstring	composite;
					if (j->pendingFlag)	composite = _T("n");
					else			composite = _T("o");

					if (j->mimeHeader.spam()) composite += _T("y");
					else			composite += _T("n");

					composite += i->accountName + _T("\n");
					if (j->mimeHeader.hasAttachment()) composite += _T("t");
					else			composite += _T("f");
					composite += _T("(") + wsizeString(j->size) + _T(")\n");
					composite += j->mimeHeader.date() + _T("\n");
					composite += j->mimeHeader.from() + _T("\n");
					composite += j->mimeHeader.subject() + _T("\n");
					composite += atow(j->messageID);

					if (emailListbox.FindString(-1, composite.c_str()) == LB_ERR)
					{
						emailListbox.InsertString(0, composite.c_str());
					}

					activeList.push_back(composite);
				}
			}
		}
	}

	// Remove the listbox entries that are not found in the active list

	{
		int	als = activeList.size();
		for (int i = 0; i < emailListbox.GetCount(); )
		{
			int	found = false;
			for (int j = 0; j < als; j++)
			{
				if (emailListbox.FindString(-1, activeList[j].c_str()) == i)
				{
					found = true;
					break;
				}
			}
			if (!found) emailListbox.DeleteString(i);
			else	i++;
		}
	}

	// Remove the invalid ones now...

	if (!emailListbox.GetCount() || !background.data())	emailListbox.ShowWindow(SW_HIDE);
	else							emailListbox.ShowWindow(SW_SHOW);
}

// --------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::TrayMessage(UINT message)
{
	NOTIFYICONDATA tnd;

	switch(message)
	{
		case NIM_ADD:
			tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
			break;

		case NIM_MODIFY:
			tnd.uFlags = NIF_MESSAGE | NIF_TIP;
			break;

		case NIM_DELETE:
			tnd.uFlags = NIF_MESSAGE;
			break;
	}

	wstring	workingStats = statsString;

	// Flags

	if (appPrefs().accounts().countErrors()) workingStats += _T(" [ERRORS]");
	if (theApp.prefs().other()[_T("disableChecking")].booleanValue()) workingStats += _T(" [DISABLED]");

	memset(tnd.szTip, 0, sizeof(tnd.szTip));
	wcsncpy(tnd.szTip, workingStats.c_str(), 64 - 1);
	tnd.uID = (UINT) IDR_MAINFRAME;
	tnd.cbSize = sizeof(NOTIFYICONDATA);
	tnd.hWnd = GetSafeHwnd();
	tnd.uCallbackMessage = TRAY_MESSAGE;
	tnd.hIcon = m_hTrayIcon;

	if (!Shell_NotifyIcon(message, &tnd))
	{
		// If it fails, add the icon, then try again

		NOTIFYICONDATA	ttnd = tnd;
		ttnd.uFlags = NIF_MESSAGE|NIF_ICON|NIF_TIP;
		Shell_NotifyIcon(NIM_ADD, &ttnd);

		// Try again...

		Shell_NotifyIcon(message, &tnd);
	}
} 

// --------------------------------------------------------------------------------------------------------------------------------

LRESULT	C3DmeDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == TRAY_MESSAGE)
	{
		if (wParam == (WPARAM) IDR_MAINFRAME && lParam == WM_RBUTTONUP)
		{
			POINT	loc;
			GetCursorPos(&loc);
			OnRButtonUp(0, loc);
		}
		else if (wParam == (WPARAM) IDR_MAINFRAME && lParam == WM_LBUTTONDBLCLK)
		{
			showWindow();
		}
		else if (wParam == (WPARAM) IDR_MAINFRAME && lParam == WM_LBUTTONUP)
		{
			showWindow();
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

// --------------------------------------------------------------------------------------------------------------------------------

BOOL	C3DmeDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (!lParam)
	{
		if (wParam == MENU_VIEW_MAILS)
		{
			showWindow();
		}
		else if (wParam == MENU_HELP)
		{
			OnHelpButton();
		}
		else if (wParam == MENU_ABOUT)
		{
			OnAbout();
		}
#if	(!defined(_LITE_) && !defined(_REGISTERED_)) || defined(_DEBUG)
		else if (wParam == MENU_ENTER_REGISTRATION_CODE)
		{
			RegDownloadDlg	dlg;
			dlg.DoModal();
		}
#endif	// (!_LITE_ && !_REGISTERED_) || _DEBUG
//#if	defined(_LITE_)
//		else if (wParam == MENU_ABOUT_CLIENT)
//		{
//			AfxMessageBox(
//				"Grey Archer\n"
//				"Making life easier\n"
//				"\n"
//				"www.greyarcher.com\n");
//		}
//#endif // _LITE_
		else if (wParam == MENU_CHECK_UPDATES)
		{
#if	defined(_LITE_)
			if (AfxMessageBox(
				PROGRAM_NAME_STRING _T(" does not allow for automatic updates.\n")
				_T("\n")
				_T("In order to upgrade, you will need to download the shareware\n")
				_T("version, or purchase a registered version.\n")
				_T("\n")
				_T("Upgrading will allow you to enjoy more 3D effects as well as\n")
				_T("automatic program updates.\n")
				_T("\n")
				_T("Would you like to visit the ") PROGRAM_NAME_STRING _T(" home page?"),
				MB_YESNO) == IDYES)
			{
				::ShellExecute(::GetDesktopWindow(), NULL, _T("http://www.3DMailEffects.com/"), NULL, NULL, SW_SHOWNORMAL);
			}
#else
			checkForUpdates(false);
#endif // _LITE_
		}
		else if (wParam == MENU_DISABLE_KEY_BLINKING)
		{
		}
		else if (wParam == MENU_DISABLE_TRAY_BLINKING)
		{
		}
		else if (wParam == MENU_SNAP_TO_DESKTOP_EDGES)
		{
		}
		else if (wParam == MENU_DISABLE_APP_POPUPS)
		{
		}
		else if (wParam == MENU_DISABLE_3D_POPUPS)
		{
		}
		else if (wParam == MENU_DISABLE_ALARM_RUNS)
		{
		}
		else if (wParam == MENU_DISABLE_ALARMS)
		{
		}
		else if (wParam == MENU_DISABLE_REMINDERS)
		{
		}
		else if (wParam == MENU_DISABLE_REMINDER_RUNS)
		{
		}
		else if (wParam == MENU_DISABLE_CHECKING)
		{
		}
		else if (wParam == MENU_SHOW_NEW)
		{
		}
		else if (wParam == MENU_STAY_ON_TOP)
		{
		}
		else if (wParam == MENU_STOP_PENDING_DELETES)
		{
		}
		else if (wParam == MENU_SHOW_3D)
		{
		}
		else if (wParam == MENU_RUN_EMAIL_CLIENT)
		{
		}
		else if (wParam == MENU_CHECK_NOW)
		{
		}
		else if (wParam == MENU_PREFERENCES)
		{
			editPreferences();
		}
		else if (wParam == MENU_MARK_READ)
		{
		}
		else if (wParam == MENU_SHOW_TIP)
		{
		}
		else if (wParam == MENU_HIDE)
		{
			OnCancel();
		}
		else if (wParam == MENU_EXIT)
		{
			OnExit();
		}
		else if (wParam >= 3000) // Skin
		{
		}
		else if (wParam >= 1999) // Select account
		{
			appPrefs().currentAccount() = wParam - 2000;
			updateTexts();
			updateLists();
		}
	}

	return CDialog::OnCommand(wParam, lParam);
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	C3DmeDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (NULL != toolTip) toolTip->RelayEvent(pMsg);
	
	return CDialog::PreTranslateMessage(pMsg);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// Good place for this...

#if	defined(_REGISTERED_)
	destroyStolen();
#endif // _REGISTERED_

	// "Toggles" submenu

	CMenu	toggles;
	toggles.CreatePopupMenu();

	toggles.AppendMenu(MF_STRING|(theApp.prefs().window()[_T("snapToEdges")].booleanValue()           ? MF_CHECKED:MF_UNCHECKED), MENU_SNAP_TO_DESKTOP_EDGES, _T("Snap to desktop edges"));
	toggles.AppendMenu(MF_STRING|(theApp.prefs().other()[_T("showNew")].booleanValue()                ? MF_CHECKED:MF_UNCHECKED), MENU_SHOW_NEW,              _T("Show new mails only"));
	toggles.AppendMenu(MF_STRING|(theApp.prefs().window()[_T("stayOnTop")].booleanValue()             ? MF_CHECKED:MF_UNCHECKED), MENU_STAY_ON_TOP,           _T("Stay on top"));
	toggles.AppendMenu(MF_SEPARATOR);
	toggles.AppendMenu(MF_STRING|(theApp.prefs().global()[_T("blinkkeyboardLEDSpeed")].integerValue() ? MF_UNCHECKED:MF_CHECKED), MENU_DISABLE_KEY_BLINKING,  _T("Disable keyboard LED blinking"));
	toggles.AppendMenu(MF_STRING|(theApp.prefs().global()[_T("blinktrayIconSpeed")].integerValue()    ? MF_UNCHECKED:MF_CHECKED), MENU_DISABLE_TRAY_BLINKING, _T("Disable icon tray blinking"));
	toggles.AppendMenu(MF_STRING|(theApp.prefs().other()[_T("disablePopups")].booleanValue()          ? MF_CHECKED:MF_UNCHECKED), MENU_DISABLE_APP_POPUPS,    _T("Disable all application popups"));
	toggles.AppendMenu(MF_STRING|(theApp.prefs().other()[_T("disable3DPopups")].booleanValue()        ? MF_CHECKED:MF_UNCHECKED), MENU_DISABLE_3D_POPUPS,     _T("Disable all 3D popups"));
	toggles.AppendMenu(MF_STRING|(theApp.prefs().other()[_T("disableAlarms")].booleanValue()          ? MF_CHECKED:MF_UNCHECKED), MENU_DISABLE_ALARMS,        _T("Disable all alarms"));
	toggles.AppendMenu(MF_STRING|(theApp.prefs().other()[_T("disableAlarmRuns")].booleanValue()       ? MF_CHECKED:MF_UNCHECKED), MENU_DISABLE_ALARM_RUNS,    _T("Disable all alarm programs"));
	toggles.AppendMenu(MF_STRING|(theApp.prefs().other()[_T("disableReminders")].booleanValue()       ? MF_CHECKED:MF_UNCHECKED), MENU_DISABLE_REMINDERS,     _T("Disable all reminders"));
	toggles.AppendMenu(MF_STRING|(theApp.prefs().other()[_T("disableReminderRuns")].booleanValue()    ? MF_CHECKED:MF_UNCHECKED), MENU_DISABLE_REMINDER_RUNS, _T("Disable all reminder programs"));
	toggles.AppendMenu(MF_STRING|(theApp.prefs().other()[_T("disableChecking")].booleanValue()        ? MF_CHECKED:MF_UNCHECKED), MENU_DISABLE_CHECKING,      _T("Disable checking for email"));

	// "Accounts" submenu

	CMenu	accounts;
	accounts.CreatePopupMenu();

	int	index = AccountList::ALL_ACCOUNTS;
	accounts.AppendMenu(MF_STRING|(appPrefs().currentAccount() == index ? MF_CHECKED:MF_UNCHECKED), 2000+index, _T("-- All accounts --"));
	accounts.AppendMenu(MF_SEPARATOR);

	index++;

	for (AccountList::iterator i = appPrefs().accounts().begin(); i != appPrefs().accounts().end(); ++i)
	{
		if (i->deleted)
		{
			continue;
		}
		else if (i->disabled)
		{
			accounts.AppendMenu(MF_STRING|MF_DISABLED|MF_GRAYED, 0, i->accountName.c_str());
			continue;
		}
		else
		{
			accounts.AppendMenu(MF_STRING|(appPrefs().currentAccount() == index ? MF_CHECKED:MF_UNCHECKED), 2000+index, i->accountName.c_str());
			index++;
		}
	}

	// "Skins" submenu
	CMenu	skins;
	{
		skins.CreatePopupMenu();
	}

	// Main menu

	CMenu	menu;
	menu.CreatePopupMenu();

#if	!defined(_LITE_)
	wstring	appString = APP_STRING _T(" (patch level ") + integerToWString(getCurrentPatchLevel()) + _T(")");
#else
	wstring	appString = APP_STRING;
#endif // !_LITE_

	menu.AppendMenu(MF_STRING|MF_DISABLED, MENU_ABOUT, appString.c_str());

	menu.AppendMenu(MF_SEPARATOR);

//#if	defined(_LITE_)
//	menu.AppendMenu(MF_STRING, MENU_ABOUT_CLIENT, "Contact Grey Archer");
//	menu.AppendMenu(MF_SEPARATOR);
//#endif // _LITE_

	menu.AppendMenu(MF_STRING|MF_POPUP, (unsigned int) toggles.GetSafeHmenu(), _T("Toggles"));
	menu.AppendMenu(MF_STRING|MF_POPUP, (unsigned int) skins.GetSafeHmenu(), _T("Skins"));
	menu.AppendMenu(MF_STRING|MF_POPUP, (unsigned int) accounts.GetSafeHmenu(), _T("Accounts"));

	menu.AppendMenu(MF_SEPARATOR);

	menu.AppendMenu(MF_STRING, MENU_STOP_PENDING_DELETES, _T("Stop deleting!"));
	menu.AppendMenu(MF_STRING, MENU_PREFERENCES, _T("Preferences"));
	menu.AppendMenu(MF_STRING, MENU_CHECK_NOW, _T("Check for mail now"));
	menu.AppendMenu(MF_STRING, MENU_MARK_READ, _T("Mark read"));
	menu.AppendMenu(MF_STRING, MENU_SHOW_TIP, _T("Display Tip-of-the-day"));
	menu.AppendMenu(MF_STRING, MENU_SHOW_3D, _T("Display 3D popup"));
	menu.AppendMenu(MF_STRING, MENU_RUN_EMAIL_CLIENT, _T("Run your default email client"));

	menu.AppendMenu(MF_SEPARATOR);

	if (!hidden)	menu.AppendMenu(MF_STRING, MENU_HIDE, _T("Hide"));
	else		menu.AppendMenu(MF_STRING, MENU_VIEW_MAILS, _T("Open"));
	menu.AppendMenu(MF_STRING, MENU_ABOUT, _T("About"));
	menu.AppendMenu(MF_STRING, MENU_HELP, _T("Help"));

#if	!defined(_LITE_)
	menu.AppendMenu(MF_STRING, MENU_CHECK_UPDATES, _T("Check for updates"));
#else	// !_LITE_
	menu.AppendMenu(MF_STRING|MF_GRAYED, MENU_CHECK_UPDATES, _T("Check for updates"));
#endif	// !_LITE_


#if	(!defined(_LITE_) && !defined(_REGISTERED_)) || defined(_DEBUG)
	menu.AppendMenu(MF_STRING, MENU_ENTER_REGISTRATION_CODE, _T("Enter registration code"));
#endif	// (!_LITE_ && !_REGISTERED_) || _DEBUG

	menu.AppendMenu(MF_STRING, MENU_EXIT, _T("Exit"));

	POINT	loc;
	GetCursorPos(&loc);

	SetForegroundWindow();
	menu.TrackPopupMenu(TPM_CENTERALIGN, loc.x, loc.y, this, NULL);
	PostMessage(WM_NULL, 0, 0);

	toggles.DestroyMenu();
	accounts.DestroyMenu();
	menu.DestroyMenu();
	
	CDialog::OnRButtonUp(nFlags, point);
}

// ---------------------------------------------------------------------------------------------------------------------------------

int	C3DmeDlg::getScrollLockState()
{
	BYTE	keyState[256];
	GetKeyboardState((LPBYTE)&keyState);
	return keyState[VK_SCROLL] & 1;
}

// ---------------------------------------------------------------------------------------------------------------------------------

int	C3DmeDlg::getNumLockState()
{
	BYTE	keyState[256];
	GetKeyboardState((LPBYTE)&keyState);
	return keyState[VK_NUMLOCK] & 1;
}

// ---------------------------------------------------------------------------------------------------------------------------------

int	C3DmeDlg::getCapsLockState()
{
	BYTE	keyState[256];
	GetKeyboardState((LPBYTE)&keyState);
	return keyState[VK_CAPITAL] & 1;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::saveScrollLockState()
{
	if (savedScrollLockState == -1)
	{
		savedScrollLockState = getScrollLockState();
		lastScrollLockState = savedScrollLockState;

		savedNumLockState = getNumLockState();
		lastNumLockState = savedNumLockState;

		savedCapsLockState = getCapsLockState();
		lastCapsLockState = savedCapsLockState;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::restoreScrollLockState()
{
	if (savedScrollLockState != -1 || savedNumLockState != -1 || savedCapsLockState != -1)
	{
		if (this == GetForegroundWindow())
		{
			if (savedScrollLockState != getScrollLockState()) toggleScrollLockState();
			savedScrollLockState = -1;
			lastScrollLockState = -1;

			if (savedNumLockState != getNumLockState()) toggleNumLockState();
			savedNumLockState = -1;
			lastNumLockState = -1;

			if (savedCapsLockState != getCapsLockState()) toggleCapsLockState();
			savedCapsLockState = -1;
			lastCapsLockState = -1;
		}
		else
		{
			if (savedScrollLockState != lastScrollLockState) toggleScrollLockState();
			savedScrollLockState = -1;
			lastScrollLockState = -1;

			if (savedNumLockState != lastNumLockState) toggleNumLockState();
			savedNumLockState = -1;
			lastNumLockState = -1;

			if (savedCapsLockState != lastCapsLockState) toggleCapsLockState();
			savedCapsLockState = -1;
			lastCapsLockState = -1;
		}
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	C3DmeDlg::objectInForeground()
{
	return (threeThread && threeThread->threeDlg && threeThread->threeDlg->hasFocus);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::toggleScrollLockState()
{
	// Are we blinking?

	if (savedScrollLockState != -1)
	{
		// Did they hit the scroll-lock?

		if (getScrollLockState() != lastScrollLockState && !objectInForeground())
		{
			keyBlinkDisabledTemporarily = true;
		}

		// Simulate a key press & release

		keybd_event( VK_SCROLL, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );
		keybd_event( VK_SCROLL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		lastScrollLockState = !lastScrollLockState;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::toggleNumLockState()
{

	// Are we blinking?

	if (savedNumLockState != -1)
	{
		// Did they hit the num-lock?

		if (getNumLockState() != lastNumLockState && !objectInForeground())
		{
			keyBlinkDisabledTemporarily = true;
		}

		// Simulate a key press & release

		keybd_event( VK_NUMLOCK, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );
		keybd_event( VK_NUMLOCK, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		lastNumLockState = !lastNumLockState;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::toggleCapsLockState()
{
	// Are we blinking?

	if (savedCapsLockState != -1)
	{
		// Did they hit the caps-lock?

		if (getCapsLockState() != lastCapsLockState && !objectInForeground())
		{
			keyBlinkDisabledTemporarily = true;
		}

		// Simulate a key press & release

		keybd_event( VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );
		keybd_event( VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		lastCapsLockState = !lastCapsLockState;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::hide3DWindow()
{
	if (threeThread)
	{
		threeThread->PostThreadMessage(THREETHREAD_TERMINATE, 0, 0);
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::hideWindow()
{
	if (!hidden)
	{
		hidden = true;
		ShowWindow(SW_HIDE);

		// Windows INSISTS on showing the dialog at startup when the message queue goes dry. Therefore, when we hide
		// the window, we also move it waaay off-screen.

		CRect	r;
		GetWindowRect(r);
		r.OffsetRect(-10000, -10000);
		MoveWindow(r, FALSE);

		// Mark them read?

		if (theApp.prefs().window()[_T("markReadOnHide")].booleanValue())
		{
			appPrefs().accounts().markRead(AccountList::ALL_ACCOUNTS);
			updateLists();
			updateTexts();
		}

		if (theApp.prefs().window()[_T("close3DPopupOnHide")].booleanValue())
		{
			hide3DWindow();
		}
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::showWindow()
{
	if (hidden)
	{
		// For the reason why we do this, consult hideWindow()...

		CRect	r;
		GetWindowRect(r);

		CRect	nr = r;
		nr.left = windowLeft;
		nr.top = windowTop;
		nr.right = nr.left + r.Width();
		nr.bottom = nr.top + r.Height();
		MoveWindow(nr);

		// Now show it...

		hidden = false;
		ShowWindow(SW_SHOW);
	}

	setStayOnTopState();
	SetForegroundWindow();
	SetFocus();
	SetActiveWindow();
	::SetActiveWindow(GetSafeHwnd());
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	C3DmeDlg::deleteMail(const wstring &id)
{
	// We need a thread to work with...

	if (!currentThread) return false;

	// Find the account and the email to be deleted...

	for (AccountList::iterator i = appPrefs().accounts().begin(); i != appPrefs().accounts().end(); ++i)
	{
		if (i->disabled || i->deleted) continue;

		for (EmailList::iterator j = i->emailList.begin(); j != i->emailList.end(); ++j)
		{
			if (j->messageID == wtoa(id))
			{
				currentThread->queueMessage(MailThread::CMD_DELETE, (sAccount *) &(*i), j->messageID);
				return true;
			}
		}
	}

	return false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::emailFromListID(const wstring &id, sAccount **aptr, sEmail **eptr)
{
	// Find the account and the email to be deleted...

	*aptr = NULL;
	*eptr = NULL;
	for (AccountList::iterator i = appPrefs().accounts().begin(); i != appPrefs().accounts().end(); ++i)
	{
		if (i->disabled || i->deleted) continue;

		EmailList & el = i->emailList;

		for (EmailList::iterator j = el.begin(); j != el.end(); ++j)
		{
			sEmail & em = *j;

			if (j->messageID == wtoa(id))
			{
				*aptr = (sAccount *) &(*i);
				*eptr = (sEmail *) &(*j);
				break;
			}
		}
		if (*aptr) break;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::previewMail(const wstring &id)
{
	// Find the account and the email to be deleted...

	sAccount	*aptr = NULL;
	sEmail		*eptr = NULL;
	emailFromListID(id, &aptr, &eptr);

	if (aptr && eptr)
	{
		eptr->autoDeleteFlag = false;
		eptr->pendingFlag = false;
		eptr->newFlag = false;
		aptr->newPending = aptr->emailList.countPendingEmails() ? true:false;
		updateStats(_T(""));
		updateLists();
		updateTexts();

		wstring	prompt;

		if (eptr->receivedSize < eptr->size)
		{
			prompt += _T("This email preview may not contain all of the data for this email.\n");
			prompt += _T("You only have ") + wsizeString(eptr->receivedSize) + _T(", out of ") + wsizeString(eptr->size) + _T(" (total).\n");
			prompt += _T("\n");
			prompt += _T("Because of this, this email will probably be incomplete, and will\n");
			prompt += _T("probably be missing attachments if there were any.\n");
			prompt += _T("\n");
			prompt += _T("If you would like to preview large emails like this, go into your\n");
			prompt += _T("account settings and increase the number of preview lines.");
		}

		if (aptr->useWebmail && eptr->mimeHeader.hasAttachment())
		{
			if (prompt.length())
			{
				prompt += _T("\n\nAlso...\n\n");
			}

			prompt += _T("This email has attachments. In order to download them, you will need\n");
			prompt += _T("to visit your webmail provider and download them from your mailbox there.");
		}
		if (prompt.length()) AfxMessageBox(prompt.c_str());

		EmailPreview	prv(this, *aptr, *eptr);
		prv.DoModal();
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::OnDblclkEmailList()
{
	previewMail(emailListbox.getMessageID(emailListbox.GetCaretIndex()));
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	C3DmeDlg::OnEraseBkgnd(CDC* pDC) 
{
	if (!background.data())
	{
		ModifyStyle(WS_SYSMENU, WS_CAPTION);
		noSkinText.ShowWindow(SW_SHOW);
		return CDialog::OnEraseBkgnd(pDC);
	}
	else
	{
		if (noSkinText.IsWindowVisible())
		{
			ModifyStyle(WS_CAPTION, WS_SYSMENU);
			noSkinText.ShowWindow(SW_HIDE);
		}
	}

	// Our dib

	winDIB	dib(*pDC);

	CRect	clientRect;
	GetClientRect(clientRect);

	dib.frameBuffer((unsigned char *) background.data() + fuckfunc_9, NULL, background.width(), background.height(), 32);
	dib.srcRect() = CRect(0, 0, background.width(), background.height());
	dib.dstRect() = CRect(0, 0, background.width(), background.height());
	dib.copyToDisplay();
	updateStats(statsString);

	return TRUE;
}

// --------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::editPreferences()
{
	theApp.prefs().processUI(this);

	setStayOnTopState();
	setShowOnTaskbarState();
//	setKeyboardLedBlinkingState();
//	setTrayBlinkingState();

	updateTexts();
	updateLists();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	dragging = true;
	dragPos = point;
	SetCapture();
	CDialog::OnLButtonDown(nFlags, point);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	dragging = true;
	dragPos = point;
	SetCapture();
	CDialog::OnLButtonDblClk(nFlags, point);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	dragging = false;
	ReleaseCapture();
	CDialog::OnLButtonUp(nFlags, point);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!mmInitialized)
	{
		mmInitialized = true;

		HMODULE hUser32 = GetModuleHandle(TEXT("USER32"));
		if (hUser32)
		{
			*(FARPROC*)&pfnMonitorFromWindow = GetProcAddress(hUser32,"MonitorFromWindow");
			*(FARPROC*)&pfnGetMonitorInfo = GetProcAddress(hUser32,"GetMonitorInfoA");
		}
	}

	// Do the dragging

	if (dragging)
	{
		CRect	wr;
		GetWindowRect(wr);
		int	w = wr.Width();
		int	h = wr.Height();

		CRect	desktopRect;
		if (pfnMonitorFromWindow && pfnGetMonitorInfo)
		{
			HMONITOR hm = pfnMonitorFromWindow(GetSafeHwnd(), 2 /*MONITOR_DEFAULTTONEAREST*/);
			MONITORINFO mi;
			mi.cbSize = sizeof(mi);
			pfnGetMonitorInfo(hm, &mi);
			desktopRect = mi.rcWork;
		}
		else
		{
			CWnd	*dw = GetDesktopWindow();
			dw->GetWindowRect(desktopRect);
		}

		// Place the window

		POINT	p;
		GetCursorPos(&p);
		wr.left = p.x - dragPos.x;
		wr.top = p.y - dragPos.y;
		wr.right = wr.left + w;
		wr.bottom = wr.top + h;

		// Before we move it, let's snap it to the desktop rect

		if (theApp.prefs().window()[_T("snapToEdges")].booleanValue())
		{
			#define ABS(a) ((a) < 0 ? -(a) : (a))

			if (ABS(wr.top - desktopRect.top) < theApp.prefs().window()[_T("edgeSnapDistance")].integerValue())
			{
				wr.top = desktopRect.top;
				wr.bottom = wr.top + h;
			}

			if (ABS(wr.left - desktopRect.left) < theApp.prefs().window()[_T("edgeSnapDistance")].integerValue())
			{
				wr.left = desktopRect.left;
				wr.right = wr.left + w;
			}

			if (ABS(wr.bottom - desktopRect.bottom) < theApp.prefs().window()[_T("edgeSnapDistance")].integerValue())
			{
				wr.bottom = desktopRect.bottom;
				wr.top = wr.bottom - h;
			}

			if (ABS(wr.right - desktopRect.right) < theApp.prefs().window()[_T("edgeSnapDistance")].integerValue())
			{
				wr.right = desktopRect.right;
				wr.left = wr.right - w;
			}
		}

		MoveWindow(wr);
		windowLeft = wr.left;
		windowTop = wr.top;
	}

	CDialog::OnMouseMove(nFlags, point);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::OnCaptureChanged(CWnd *pWnd) 
{
	dragging = false;	
	CDialog::OnCaptureChanged(pWnd);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::show3D()
{
	show3DFlag = true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::setStayOnTopState()
{
	UINT	flags	= SWP_NOMOVE|SWP_NOSIZE;
	if (!hidden) flags |= SWP_SHOWWINDOW;

	if (theApp.prefs().window()[_T("stayOnTop")].booleanValue())	SetWindowPos(&wndTopMost, 0,0,0,0,flags);
	else								SetWindowPos(&wndTop, 0,0,0,0,flags);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::setShowOnTaskbarState()
{
	if (!hidden) ShowWindow(SW_HIDE);
	if (theApp.prefs().window()[_T("showOnTaskbar")].booleanValue())
	{
		ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);
	}
	else
	{
		ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	}
	if (!hidden) ShowWindow(SW_SHOW);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::checkForUpdates(const bool quiet)
{
#if	!defined(_LITE_)
	if (areWeOnline())
	{
		UpdatesDlg	dlg(quiet, theApp.prefs().global()[_T("autoCheckForUpdates")].booleanValue(), this);
		dlg.DoModal();
		if (dlg.restart)
		{
			wstring	str = theApp.programPath() + _T("upd.exe");
			::ShellExecute(::GetDesktopWindow(), _T("open"), str.c_str(), _T("autorun"), NULL, SW_SHOWNORMAL);
			Sleep(1000);
			OnExit();
		}
		else if (dlg.didUpdate)
		{
			showUpdatedMessage();
		}

#if	defined(_BETA_)
		SetTimer(AUTOUPDATE_TIMER, 1 * 24 * 60 * 60 * 1000, NULL);
#else
		SetTimer(AUTOUPDATE_TIMER, 7 * 24 * 60 * 60 * 1000, NULL);
#endif // !_BETA_
	}
	else
	{
		SetTimer(AUTOUPDATE_TIMER, 60000, NULL);
	}
#endif // !_LITE_
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	C3DmeDlg::showUpdatedMessage()
{
#if	defined(_BETA_)
	wstring	updated = (wstring)
			_T("Your copy of ") PROGRAM_NAME_STRING _T(" was updated successfully.\n")
			_T("\n")
			_T("You are currently running beta version ") + BETA_VERSION_STRING + _T(" (patch level ") + integerToWString(getCurrentPatchLevel()) + _T(".)");
#else
	wstring	updated = (wstring)
			_T("Your copy of ") PROGRAM_NAME_STRING _T(" was updated successfully.\n")
			_T("\n")
			_T("You are currently running version ") + VERSION_STRING + _T(" (patch level ") + integerToWString(getCurrentPatchLevel()) + _T(".)");
#endif
	AfxMessageBox(updated.c_str());
}

// ---------------------------------------------------------------------------------------------------------------------------------
// 3DmeDlg.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
