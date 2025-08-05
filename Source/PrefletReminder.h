// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _   _____                 _           _               _     
// |  __ \           / _| |    | | |  __ \               (_)         | |             | |    
// | |__) |_ __  ___| |_| | ___| |_| |__) | ___ _ __ ___  _ _ __   __| | ___ _ __    | |__  
// |  ___/| '__|/ _ \  _| |/ _ \ __|  _  / / _ \ '_ ` _ \| | '_ \ / _` |/ _ \ '__|   | '_ \ 
// | |    | |  |  __/ | | |  __/ |_| | \ \|  __/ | | | | | | | | | (_| |  __/ |    _ | | | |
// |_|    |_|   \___|_| |_|\___|\__|_|  \_\\___|_| |_| |_|_|_| |_|\__,_|\___|_|   (_)|_| |_|
//                                                                                          
//                                                                                          
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

#ifndef	_H_PREFLETREMINDER
#define _H_PREFLETREMINDER

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

#include "Preflet.h"

// ---------------------------------------------------------------------------------------------------------------------------------

class	PrefletReminder : public Preflet
{
public:
	// Enumerations

		enum		{ IDD = IDD_PREFLET_REMINDER };

	// Construction/Destruction

				PrefletReminder(const wstring & registryKey, CWnd* pParent = NULL);
virtual				~PrefletReminder();

	// Operators

	// Implementation

virtual		void		OnPreInitPreflet();
virtual		void		OnPostInitPreflet();
virtual		void		enableControls();
virtual		void		setProgramIcon(const wstring & program);
afx_msg		void		OnBnClickedReminderPlaySoundCheck();
afx_msg		void		OnBnClickedReminderBrowseSoundButton();
afx_msg		void		OnBnClickedReminderPlaySoundButton();
afx_msg		void		OnBnClickedReminderVisitWebPageCheck();
afx_msg		void		OnBnClickedReminderRunProgramCheck();
afx_msg		void		OnBnClickedReminderBrowseProgramButton();

	// Accessors

inline		HICON &		programIcon()		{return _programIcon;}
inline	const	HICON		programIcon() const	{return _programIcon;}

private:
	// Data members

		HICON		_programIcon;

				DECLARE_DYNAMIC(PrefletReminder)
				DECLARE_MESSAGE_MAP()
};

#endif // _H_PREFLETREMINDER
// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletReminder.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
