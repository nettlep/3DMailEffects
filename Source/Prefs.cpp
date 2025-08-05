// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __                          
// |  __ \           / _|                         
// | |__) |_ __  ___| |_ ___      ___ _ __  _ __  
// |  ___/| '__|/ _ \  _/ __|    / __| '_ \| '_ \ 
// | |    | |  |  __/ | \__ \ _ | (__| |_) | |_) |
// |_|    |_|   \___|_| |___/(_) \___| .__/| .__/ 
//                                   | |   | |    
//                                   |_|   |_|    
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

#include "stdafx.h"
#include "3dme.h"
#include "Prefs.h"
#include "PreferencesUI.h"

// ---------------------------------------------------------------------------------------------------------------------------------

	Prefs::Prefs()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

	Prefs::~Prefs()
{
	// Cleanup our preflets

	for (PrefletMap::iterator i = preflets().begin(); i != preflets().end(); ++i)
	{
		delete i->second;
	}

	preflets().clear();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	Prefs::init()
{
	#define	prefletHelper(key, type) preflets()[_T(key)] = new type(_T(key));
	#define	prefletHelperW(key, type) preflets()[key] = new type(key);

	// Our preference categories

	prefletHelper("About",                               PrefletAbout);
	prefletHelper("About\\Support information",          PrefletSupport);
	prefletHelper("About\\Credits",                      PrefletCredits);
	prefletHelper("Network Settings",                    PrefletNetwork);
	prefletHelper("Network Settings\\Proxy Settings",    PrefletProxy);
	prefletHelper("Email Settings",                      PrefletEmailSettings);
	prefletHelper("Global settings",                     PrefletGlobal);
	prefletHelper("Window settings",                     PrefletWindow);
	prefletHelper("Window settings\\Skin settings",      PrefletSkins);
	prefletHelper("Other",                               PrefletOther);

	// Populate the email accounts

	HKEY	hKey;
	DWORD	rc = RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Fluid Studios\\3D Mail Effects\\Email Settings"), 0, KEY_READ, &hKey);
	if (rc != ERROR_SUCCESS)
	{
		LOG("Unable to open registry key for email account enumeration: " + wtoa(systemErrorString(rc)), Logger::LOG_CRIT);
	}
	else
	{
		const	unsigned int	keyLen = 4096;
		wchar_t	keyName[keyLen];
		for (int i = 0; true; ++i)
		{
			LONG	rc = RegEnumKey(hKey, i, keyName, keyLen);
			if (rc != ERROR_SUCCESS)
			{
				if (rc != ERROR_NO_MORE_ITEMS)
				{
					LOG("Unable to enumerate all registry keys for email accounts: " + wtoa(systemErrorString(rc)), Logger::LOG_CRIT);
				}

				break;
			}

			// Add the preflet

			prefletHelperW(wstring(_T("Email Settings\\")) + keyName, PrefletEmailAccount);
			prefletHelperW(wstring(_T("Email Settings\\")) + keyName + _T("\\Alarms"), PrefletAlarm);
			prefletHelperW(wstring(_T("Email Settings\\")) + keyName + _T("\\Reminders"), PrefletReminder);
		}

		RegCloseKey(hKey);
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	Prefs::processUI(CWnd * parent, const wstring & defaultPreflet)
{
	PreferencesUI dlg(preflets(), parent);
	dlg.defaultPreflet() = defaultPreflet;
	return dlg.DoModal() == IDOK;
}

// ---------------------------------------------------------------------------------------------------------------------------------
// Prefs.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

