// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _   _____                 _           _                                
// |  __ \           / _| |    | | |  __ \               (_)         | |                               
// | |__) |_ __  ___| |_| | ___| |_| |__) | ___ _ __ ___  _ _ __   __| | ___ _ __      ___ _ __  _ __  
// |  ___/| '__|/ _ \  _| |/ _ \ __|  _  / / _ \ '_ ` _ \| | '_ \ / _` |/ _ \ '__|    / __| '_ \| '_ \ 
// | |    | |  |  __/ | | |  __/ |_| | \ \|  __/ | | | | | | | | | (_| |  __/ |    _ | (__| |_) | |_) |
// |_|    |_|   \___|_| |_|\___|\__|_|  \_\\___|_| |_| |_|_|_| |_|\__,_|\___|_|   (_) \___| .__/| .__/ 
//                                                                                        | |   | |    
//                                                                                        |_|   |_|    
//
//   Preflet for Email Account reminder settings
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
#include "PrefletReminder.h"
#include ".\prefletreminder.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

IMPLEMENT_DYNAMIC(PrefletReminder, Preflet)
BEGIN_MESSAGE_MAP(PrefletReminder, Preflet)
	ON_BN_CLICKED(IDC_REMINDER_PLAY_SOUND_CHECK, OnBnClickedReminderPlaySoundCheck)
	ON_BN_CLICKED(IDC_REMINDER_BROWSE_SOUND_BUTTON, OnBnClickedReminderBrowseSoundButton)
	ON_BN_CLICKED(IDC_REMINDER_PLAY_SOUND_BUTTON, OnBnClickedReminderPlaySoundButton)
	ON_BN_CLICKED(IDC_REMINDER_VISIT_WEB_PAGE_CHECK, OnBnClickedReminderVisitWebPageCheck)
	ON_BN_CLICKED(IDC_REMINDER_RUN_PROGRAM_CHECK, OnBnClickedReminderRunProgramCheck)
	ON_BN_CLICKED(IDC_REMINDER_BROWSE_PROGRAM_BUTTON, OnBnClickedReminderBrowseProgramButton)
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	PrefletReminder::PrefletReminder(const wstring & registryKey, CWnd* pParent /*=NULL*/)
	: Preflet(PrefletReminder::IDD, registryKey, pParent)
{
	// These associate our atoms (things we want to store in the preferences) with the user controls in the peflet dialogs.
	// This also puts them in a central location where they can be loaded, saved, accessed and transferred to/from the controls.
	// Nifty, huh? :)

	atoms().add(_T("frequency"), IDC_REMINDER_FREQUENCY_EDIT, PrefletAtom::ControlType::Edit, wstring(_T("15")));
	atoms().add(_T("frequencyGranularity"), IDC_REMINDER_FREQUENCY_COMBO, PrefletAtom::ControlType::Combo, wstring(_T("Minutes")));
	atoms().add(_T("playSound"), IDC_REMINDER_PLAY_SOUND_CHECK, PrefletAtom::ControlType::Button, false);
	atoms().add(_T("soundFile"), IDC_REMINDER_SOUND_FILE_EDIT, PrefletAtom::ControlType::Edit, wstring(_T("")));
	atoms().add(_T("visitWebPage"), IDC_REMINDER_VISIT_WEB_PAGE_CHECK, PrefletAtom::ControlType::Button, false);
	atoms().add(_T("webPage"), IDC_REMINDER_WEB_PAGE_EDIT, PrefletAtom::ControlType::Edit, wstring(_T("")));
	atoms().add(_T("runProgram"), IDC_REMINDER_RUN_PROGRAM_CHECK, PrefletAtom::ControlType::Button, false);
	atoms().add(_T("program"), IDC_REMINDER_PROGRAM_EDIT, PrefletAtom::ControlType::Edit, wstring(_T("")));
	atoms().readFromRegistry(registryKey);
}

// ---------------------------------------------------------------------------------------------------------------------------------

	PrefletReminder::~PrefletReminder()
{
	if (programIcon()) DestroyIcon(programIcon());
	programIcon() = 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletReminder::OnPreInitPreflet()
{
	GetDlgItem(IDC_REMINDER_PLAY_SOUND_BUTTON)->SendMessage(BM_SETIMAGE, (WPARAM) IMAGE_ICON, (LPARAM) (HICON) theApp.LoadIcon(IDI_SOUND));

	CComboBox *	combo = static_cast<CComboBox *>(GetDlgItem(IDC_REMINDER_FREQUENCY_COMBO));
	combo->AddString(_T("Seconds"));
	combo->AddString(_T("Minutes"));
	combo->AddString(_T("Hours"));
	combo->AddString(_T("Days"));
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletReminder::OnPostInitPreflet()
{
	enableControls();

	CString	prog;
	GetDlgItem(IDC_REMINDER_PROGRAM_EDIT)->GetWindowText(prog);
	setProgramIcon(wstring(prog));
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletReminder::enableControls()
{
	BOOL	enabled;

	enabled = static_cast<CButton *>(GetDlgItem(IDC_REMINDER_PLAY_SOUND_CHECK))->GetCheck();
	GetDlgItem(IDC_REMINDER_SOUND_FILE_EDIT)->EnableWindow(enabled);
	GetDlgItem(IDC_REMINDER_BROWSE_SOUND_BUTTON)->EnableWindow(enabled);
	GetDlgItem(IDC_REMINDER_PLAY_SOUND_BUTTON)->EnableWindow(enabled);

	enabled = static_cast<CButton *>(GetDlgItem(IDC_REMINDER_VISIT_WEB_PAGE_CHECK))->GetCheck();
	GetDlgItem(IDC_REMINDER_WEB_PAGE_EDIT)->EnableWindow(enabled);

	enabled = static_cast<CButton *>(GetDlgItem(IDC_REMINDER_RUN_PROGRAM_CHECK))->GetCheck();
	GetDlgItem(IDC_REMINDER_PROGRAM_EDIT)->EnableWindow(enabled);
	GetDlgItem(IDC_REMINDER_BROWSE_PROGRAM_BUTTON)->EnableWindow(enabled);
	GetDlgItem(IDC_REMINDER_PROGRAM_ICON)->EnableWindow(enabled);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletReminder::setProgramIcon(const wstring & program)
{
	if (programIcon()) DestroyIcon(programIcon());
	programIcon() = 0;

	HICON	icon = ::ExtractIcon(theApp.m_hInstance, program.c_str(), 0);
	if (reinterpret_cast<int>(icon) != 0 && reinterpret_cast<int>(icon) != 1) programIcon() = icon;

	// Get the window size (based on the height of the edit box)

	CRect	r;
	GetDlgItem(IDC_REMINDER_PROGRAM_EDIT)->GetWindowRect(r);
	r.left = r.left - r.Height() - 4;
	r.right = r.left + r.Height() + 1;
	r.top -= 1;
	r.bottom += 1;

	// Set the icon

	CStatic *	iconWindow = (CStatic *) GetDlgItem(IDC_REMINDER_PROGRAM_ICON);
	iconWindow->SetIcon(programIcon());

	// Move the window to the proper size

	ScreenToClient(r);
	iconWindow->MoveWindow(r);
	iconWindow->RedrawWindow();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletReminder::OnBnClickedReminderPlaySoundCheck()
{
	enableControls();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletReminder::OnBnClickedReminderBrowseSoundButton()
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
	of.lpstrTitle   = _T("Choose reminder sound file");
	of.Flags        = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_EXPLORER;
	of.lpstrDefExt  = _T("wav");
	of.lpstrInitialDir = theApp.soundPath().c_str();

	if (!GetOpenFileName(&of)) return;

	GetDlgItem(IDC_REMINDER_SOUND_FILE_EDIT)->SetWindowText(of.lpstrFile);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletReminder::OnBnClickedReminderPlaySoundButton()
{
	CString	soundFile;
	GetDlgItem(IDC_REMINDER_SOUND_FILE_EDIT)->GetWindowText(soundFile);
	sndPlaySound((LPCTSTR) soundFile, SND_ASYNC|SND_NOSTOP);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletReminder::OnBnClickedReminderVisitWebPageCheck()
{
	enableControls();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletReminder::OnBnClickedReminderRunProgramCheck()
{
	enableControls();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletReminder::OnBnClickedReminderBrowseProgramButton()
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
	of.lpstrTitle   = _T("Choose reminder program file");
	of.Flags        = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_EXPLORER;
	of.lpstrDefExt  = _T("exe");

	if (!GetOpenFileName(&of)) return;

	GetDlgItem(IDC_REMINDER_PROGRAM_EDIT)->SetWindowText(of.lpstrFile);
	setProgramIcon(of.lpstrFile);
}

// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletReminder.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
