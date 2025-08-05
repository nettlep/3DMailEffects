// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __         _     
// |  __ \           / _|       | |    
// | |__) |_ __  ___| |_ ___    | |__  
// |  ___/| '__|/ _ \  _/ __|   | '_ \ 
// | |    | |  |  __/ | \__ \ _ | | | |
// |_|    |_|   \___|_| |___/(_)|_| |_|
//                                     
//                                     
//
// Preferences container: holds preferences values, allows access to them and provides easy UI access
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 09/01/2004 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2004, Fluid Studios, all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_PREFS
#define _H_PREFS

// ---------------------------------------------------------------------------------------------------------------------------------
// Required module includes
// ---------------------------------------------------------------------------------------------------------------------------------

#include "PrefletMap.h"
#include "PrefletGlobal.h"
#include "PrefletWindow.h"
#include "PrefletSkins.h"
#include "PrefletAbout.h"
#include "PrefletSupport.h"
#include "PrefletCredits.h"
#include "PrefletNetwork.h"
#include "PrefletProxy.h"
#include "PrefletEmailSettings.h"
#include "PrefletEmailAccount.h"
#include "PrefletAlarm.h"
#include "PrefletReminder.h"
#include "PrefletOther.h"

// ---------------------------------------------------------------------------------------------------------------------------------

class	Prefs
{
public:
	// Construction/Destruction

					Prefs();
virtual					~Prefs();

	// Operators

inline		Preflet &		operator[](const wstring & str)		{Preflet * ptr = preflets()[str]; ASSERT(ptr); return *ptr;}

	// Implementation

virtual		void			init();
virtual		bool			processUI(CWnd * parent = NULL, const wstring & defaultPreflet = _T(""));

	// Accessors

inline		PrefletMap &		preflets()				{return _preflets;}
inline	const	PrefletMap &		preflets() const			{return _preflets;}

	// Simplification routines

inline		Preflet &		about()					{return (*this)[_T("About")];}
inline		Preflet &		support()				{return (*this)[_T("About\\Support information")];}
inline		Preflet &		credits()				{return (*this)[_T("About\\Credits")];}
inline		Preflet &		network()				{return (*this)[_T("Network Settings")];}
inline		Preflet &		proxy()					{return (*this)[_T("Network Settings\\Proxy Settings")];}
inline		Preflet &		email()					{return (*this)[_T("Email Settings")];}
inline		Preflet &		email(const wstring & accountName)	{return (*this)[_T("Email Settings\\") + accountName];}
inline		Preflet &		alarms(const wstring & accountName)	{return (*this)[_T("Email Settings\\") + accountName + _T("\\Alarms")];}
inline		Preflet &		reminders(const wstring & accountName)	{return (*this)[_T("Email Settings\\") + accountName + _T("\\Reminders")];}
inline		Preflet &		global()				{return (*this)[_T("Global settings")];}
inline		Preflet &		window()				{return (*this)[_T("Window settings")];}
inline		Preflet &		skin()					{return (*this)[_T("Window settings\\Skin settings")];}
inline		Preflet &		other()					{return (*this)[_T("Other")];}

private:
	// Disallow these

					Prefs(const Prefs & rhs);
		Prefs &			operator =(const Prefs & rhs);

	// Data members

		PrefletMap		_preflets;
};

#endif // _H_PREFS
// ---------------------------------------------------------------------------------------------------------------------------------
// Prefs.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

