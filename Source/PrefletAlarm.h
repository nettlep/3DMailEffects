// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _            _                          _     
// |  __ \           / _| |    | |     /\   | |                        | |    
// | |__) |_ __  ___| |_| | ___| |_   /  \  | | __ _ _ __ _ __ ___     | |__  
// |  ___/| '__|/ _ \  _| |/ _ \ __| / /\ \ | |/ _` | '__| '_ ` _ \    | '_ \ 
// | |    | |  |  __/ | | |  __/ |_ / ____ \| | (_| | |  | | | | | | _ | | | |
// |_|    |_|   \___|_| |_|\___|\__/_/    \_\_|\__,_|_|  |_| |_| |_|(_)|_| |_|
//                                                                            
//                                                                            
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

#ifndef	_H_PREFLETALARM
#define _H_PREFLETALARM

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

#include "Preflet.h"

// ---------------------------------------------------------------------------------------------------------------------------------

class	PrefletAlarm : public Preflet
{
public:
	// Enumerations

		enum		{ IDD = IDD_PREFLET_ALARM };

	// Construction/Destruction

				PrefletAlarm(const wstring & registryKey, CWnd* pParent = NULL);
virtual				~PrefletAlarm();

	// Operators

	// Implementation

virtual		void		OnPreInitPreflet();
virtual		void		OnPostInitPreflet();
virtual		void		enableControls();
virtual		void		setProgramIcon(const wstring & program);
afx_msg		void		OnBnClickedAlarmPlaySoundCheck();
afx_msg		void		OnBnClickedAlarmBrowseSoundButton();
afx_msg		void		OnBnClickedAlarmPlaySoundButton();
afx_msg		void		OnBnClickedAlarmVisitWebPageCheck();
afx_msg		void		OnBnClickedAlarmRunProgramCheck();
afx_msg		void		OnBnClickedAlarmBrowseProgramButton();

	// Accessors

inline		HICON &		programIcon()		{return _programIcon;}
inline	const	HICON		programIcon() const	{return _programIcon;}

private:
	// Data members

		HICON		_programIcon;

				DECLARE_DYNAMIC(PrefletAlarm)
				DECLARE_MESSAGE_MAP()
};

#endif // _H_PREFLETALARM
// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletAlarm.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
