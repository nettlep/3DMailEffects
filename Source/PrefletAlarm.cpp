// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _            _                                           
// |  __ \           / _| |    | |     /\   | |                                          
// | |__) |_ __  ___| |_| | ___| |_   /  \  | | __ _ _ __ _ __ ___       ___ _ __  _ __  
// |  ___/| '__|/ _ \  _| |/ _ \ __| / /\ \ | |/ _` | '__| '_ ` _ \     / __| '_ \| '_ \ 
// | |    | |  |  __/ | | |  __/ |_ / ____ \| | (_| | |  | | | | | | _ | (__| |_) | |_) |
// |_|    |_|   \___|_| |_|\___|\__/_/    \_\_|\__,_|_|  |_| |_| |_|(_) \___| .__/| .__/ 
//                                                                          | |   | |    
//                                                                          |_|   |_|    
//
//   Preflet for Email Account alarm settings
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 12/21/2003 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2003, Fluid Studios, all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "3dme.h"
#include "PrefletAlarm.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

IMPLEMENT_DYNAMIC(PrefletAlarm, Preflet)
BEGIN_MESSAGE_MAP(PrefletAlarm, Preflet)
	ON_BN_CLICKED(IDC_ALARM_PLAY_SOUND_CHECK, OnBnClickedAlarmPlaySoundCheck)
	ON_BN_CLICKED(IDC_ALARM_BROWSE_SOUND_BUTTON, OnBnClickedAlarmBrowseSoundButton)
	ON_BN_CLICKED(IDC_ALARM_PLAY_SOUND_BUTTON, OnBnClickedAlarmPlaySoundButton)
	ON_BN_CLICKED(IDC_ALARM_VISIT_WEB_PAGE_CHECK, OnBnClickedAlarmVisitWebPageCheck)
	ON_BN_CLICKED(IDC_ALARM_RUN_PROGRAM_CHECK, OnBnClickedAlarmRunProgramCheck)
	ON_BN_CLICKED(IDC_ALARM_BROWSE_PROGRAM_BUTTON, OnBnClickedAlarmBrowseProgramButton)
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	PrefletAlarm::PrefletAlarm(const wstring & registryKey, CWnd* pParent /*=NULL*/)
	: Preflet(PrefletAlarm::IDD, registryKey, pParent), _programIcon(0)
{
	// These associate our atoms (things we want to store in the preferences) with the user controls in the peflet dialogs.
	// This also puts them in a central location where they can be loaded, saved, accessed and transferred to/from the controls.
	// Nifty, huh? :)

	atoms().add(_T("show3D"), IDC_ALARM_SHOW_3D_CHECK, PrefletAtom::ControlType::Button, true);
	atoms().add(_T("showWindow"), IDC_ALARM_SHOW_WINDOW_CHECK, PrefletAtom::ControlType::Button, true);
	atoms().add(_T("runEmailClient"), IDC_ALARM_RUN_EMAIL_CLIENT_CHECK, PrefletAtom::ControlType::Button, false);
	atoms().add(_T("playSound"), IDC_ALARM_PLAY_SOUND_CHECK, PrefletAtom::ControlType::Button, false);
	atoms().add(_T("soundFile"), IDC_ALARM_SOUND_FILE_EDIT, PrefletAtom::ControlType::Edit, wstring(_T("")));
	atoms().add(_T("visitWebPage"), IDC_ALARM_VISIT_WEB_PAGE_CHECK, PrefletAtom::ControlType::Button, false);
	atoms().add(_T("webPage"), IDC_ALARM_WEB_PAGE_EDIT, PrefletAtom::ControlType::Edit, wstring(_T("")));
	atoms().add(_T("runProgram"), IDC_ALARM_RUN_PROGRAM_CHECK, PrefletAtom::ControlType::Button, false);
	atoms().add(_T("program"), IDC_ALARM_PROGRAM_EDIT, PrefletAtom::ControlType::Edit, wstring(_T("")));
	atoms().readFromRegistry(registryKey);
}

// ---------------------------------------------------------------------------------------------------------------------------------

	PrefletAlarm::~PrefletAlarm()
{
	if (programIcon()) DestroyIcon(programIcon());
	programIcon() = 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAlarm::OnPreInitPreflet()
{
	GetDlgItem(IDC_ALARM_PLAY_SOUND_BUTTON)->SendMessage(BM_SETIMAGE, (WPARAM) IMAGE_ICON, (LPARAM) (HICON) theApp.LoadIcon(IDI_SOUND));
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAlarm::OnPostInitPreflet()
{
	enableControls();

	CString	prog;
	GetDlgItem(IDC_ALARM_PROGRAM_EDIT)->GetWindowText(prog);
	setProgramIcon(wstring(prog));
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAlarm::enableControls()
{
	BOOL	enabled;

	enabled = static_cast<CButton *>(GetDlgItem(IDC_ALARM_PLAY_SOUND_CHECK))->GetCheck();
	GetDlgItem(IDC_ALARM_SOUND_FILE_EDIT)->EnableWindow(enabled);
	GetDlgItem(IDC_ALARM_BROWSE_SOUND_BUTTON)->EnableWindow(enabled);
	GetDlgItem(IDC_ALARM_PLAY_SOUND_BUTTON)->EnableWindow(enabled);

	enabled = static_cast<CButton *>(GetDlgItem(IDC_ALARM_VISIT_WEB_PAGE_CHECK))->GetCheck();
	GetDlgItem(IDC_ALARM_WEB_PAGE_EDIT)->EnableWindow(enabled);

	enabled = static_cast<CButton *>(GetDlgItem(IDC_ALARM_RUN_PROGRAM_CHECK))->GetCheck();
	GetDlgItem(IDC_ALARM_PROGRAM_EDIT)->EnableWindow(enabled);
	GetDlgItem(IDC_ALARM_BROWSE_PROGRAM_BUTTON)->EnableWindow(enabled);
	GetDlgItem(IDC_ALARM_PROGRAM_ICON)->EnableWindow(enabled);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAlarm::setProgramIcon(const wstring & program)
{
	if (programIcon()) DestroyIcon(programIcon());
	programIcon() = 0;

	HICON	icon = ::ExtractIcon(theApp.m_hInstance, program.c_str(), 0);
	if (reinterpret_cast<int>(icon) != 0 && reinterpret_cast<int>(icon) != 1) programIcon() = icon;

	// Get the window size (based on the height of the edit box)

	CRect	r;
	GetDlgItem(IDC_ALARM_PROGRAM_EDIT)->GetWindowRect(r);
	r.left = r.left - r.Height() - 4;
	r.right = r.left + r.Height() + 1;
	r.top -= 1;
	r.bottom += 1;

	// Set the icon

	CStatic *	iconWindow = (CStatic *) GetDlgItem(IDC_ALARM_PROGRAM_ICON);
	iconWindow->SetIcon(programIcon());

	// Move the window to the proper size

	ScreenToClient(r);
	iconWindow->MoveWindow(r);
	iconWindow->RedrawWindow();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAlarm::OnBnClickedAlarmPlaySoundCheck()
{
	enableControls();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAlarm::OnBnClickedAlarmBrowseSoundButton()
{
	wchar_t		fname[MAX_PATH];
	memset(fname, 0, sizeof(fname));
	wchar_t		filters[] = _T("Wave files (*.wav)\0*.wav\0All files (*.*)\0*.*\0\0");
	OPENFILENAME	of;
	memset(&of, 0, sizeof(OPENFILENAME));

	of.lStructSize  = sizeof(OPENFILENAME);
	of.hwndOwner    = GetSafeHwnd();
	of.lpstrFilter  = filters;
	of.nFilterIndex = 1;
	of.lpstrFile    = fname;
	of.nMaxFile     = MAX_PATH - 1;
	of.lpstrTitle   = _T("Choose alarm sound file");
	of.Flags        = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_EXPLORER;
	of.lpstrDefExt  = _T("wav");
	of.lpstrInitialDir = theApp.soundPath().c_str();

	if (!GetOpenFileName(&of)) return;

	GetDlgItem(IDC_ALARM_SOUND_FILE_EDIT)->SetWindowText(of.lpstrFile);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAlarm::OnBnClickedAlarmPlaySoundButton()
{
	CString	soundFile;
	GetDlgItem(IDC_ALARM_SOUND_FILE_EDIT)->GetWindowText(soundFile);
	sndPlaySound((LPCTSTR) soundFile, SND_ASYNC|SND_NOSTOP);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAlarm::OnBnClickedAlarmVisitWebPageCheck()
{
	enableControls();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAlarm::OnBnClickedAlarmRunProgramCheck()
{
	enableControls();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAlarm::OnBnClickedAlarmBrowseProgramButton()
{
	wchar_t		fname[MAX_PATH];
	memset(fname, 0, sizeof(fname));
	wchar_t		filters[] = _T("Executable files (*.exe)\0*.exe\0All files (*.*)\0*.*\0\0");
	OPENFILENAME	of;
	memset(&of, 0, sizeof(OPENFILENAME));

	of.lStructSize  = sizeof(OPENFILENAME);
	of.hwndOwner    = GetSafeHwnd();
	of.lpstrFilter  = filters;
	of.nFilterIndex = 1;
	of.lpstrFile    = fname;
	of.nMaxFile     = MAX_PATH - 1;
	of.lpstrTitle   = _T("Choose alarm program file");
	of.Flags        = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_EXPLORER;
	of.lpstrDefExt  = _T("exe");

	if (!GetOpenFileName(&of)) return;

	GetDlgItem(IDC_ALARM_PROGRAM_EDIT)->SetWindowText(of.lpstrFile);
	setProgramIcon(of.lpstrFile);
}

// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletAlarm.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
