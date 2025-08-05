// ---------------------------------------------------------------------------------------------------------------------------------
//                                       _   _      _     _                        
//     /\                               | | | |    (_)   | |                       
//    /  \    ___  ___  ___  _   _ _ __ | |_| |     _ ___| |_      ___ _ __  _ __  
//   / /\ \  / __|/ __|/ _ \| | | | '_ \| __| |    | / __| __|    / __| '_ \| '_ \ 
//  / ____ \| (__| (__| (_) | |_| | | | | |_| |____| \__ \ |_  _ | (__| |_) | |_) |
// /_/    \_\\___|\___|\___/ \__,_|_| |_|\__|______|_|___/\__|(_) \___| .__/| .__/ 
//                                                                    | |   | |    
//                                                                    |_|   |_|    
//
// 3D Mail Effects (Account handler class)
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 12/05/2000 by Paul Nettle
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
#include "AccountList.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// --------------------------------------------------------------------------------------------------------------------------------

unsigned int	AccountList::actualSize() const
{
	unsigned int	count = 0;
	for (const_iterator i = begin(); i != end(); ++i)
	{
		if (i->deleted) continue;
		count++;
	}
	return count;
}

// --------------------------------------------------------------------------------------------------------------------------------

unsigned int	AccountList::countErrors() const
{
	unsigned int	count = 0;
	for (const_iterator i = begin(); i != end(); ++i)
	{
		if (i->disabled || i->deleted) continue;
		if (i->lastError.length()) count++;
	}
	return count;
}

// --------------------------------------------------------------------------------------------------------------------------------

unsigned int	AccountList::countActive() const
{
	unsigned int	count = 0;
	for (const_iterator i = begin(); i != end(); ++i)
	{
		if (i->disabled || i->deleted) continue;
		count++;
	}
	return count;
}

// --------------------------------------------------------------------------------------------------------------------------------

unsigned int	AccountList::countTotalEmails() const
{
	// Tally up the TOTAL emails

	unsigned int	count = 0;
	for (const_iterator i = begin(); i != end(); ++i)
	{
		if (i->disabled || i->deleted) continue;
		count += i->emailList.size();
	}
	return count;
}

// --------------------------------------------------------------------------------------------------------------------------------

unsigned int	AccountList::countNewEmails() const
{
	// Tally up the TOTAL emails

	unsigned int	count = 0;
	for (const_iterator i = begin(); i != end(); ++i)
	{
		if (i->disabled || i->deleted) continue;
		count += i->emailList.countPendingEmails();
	}
	return count;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	AccountList::resetCheckMailTimers(const int currentAccount)
{
	int	index = 0;
	for (iterator i = begin(); i != end(); ++i, ++index)
	{
		if (i->disabled || i->deleted)
		{
			index--;
			continue;
		}
		else if (currentAccount == ALL_ACCOUNTS || currentAccount == index)
		{
			i->checkMailTimer = 0;
		}
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	AccountList::resetReminderTimers(const int currentAccount)
{
	int	index = 0;
	for (iterator i = begin(); i != end(); ++i, ++index)
	{
		if (i->disabled || i->deleted)
		{
			index--;
			continue;
		}
		else if (currentAccount == ALL_ACCOUNTS || currentAccount == index)
		{
			i->reminderTimer = 0;
		}
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	AccountList::markRead(const int currentAccount)
{
	int	index = 0;
	for (iterator i = begin(); i != end(); ++i, ++index)
	{
		if (i->disabled || i->deleted)
		{
			index--;
			continue;
		}
		else if (currentAccount == ALL_ACCOUNTS || currentAccount == index)
		{
			i->newPending = false;
			i->emailList.markRead();
		}
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	AccountList::readConfiguration(const unsigned int count)
{
	clear();

#if	defined(_REGISTERED_) || defined(_LITE_)
	for (unsigned int i = 0; i < count; i++)
	{
		sAccount	a;
		wchar_t		tag[1024];

		// Account name

		_snwprintf(tag, 1024, _T("%d_accountName"), i);
		a.accountName = theApp.GetProfileString(_T("options"), tag, _T("UnNamed"));

		// Use webmail?

		_snwprintf(tag, 1024, _T("%d_useWebmail"), i);
		a.useWebmail = theApp.GetProfileInt(_T("options"), tag, 0) ? true:false;

		// Use color?

		_snwprintf(tag, 1024, _T("%d_useColor"), i);
		a.useColor = theApp.GetProfileInt(_T("options"), tag, 0) ? true:false;

		_snwprintf(tag, 1024, _T("%d_displayColor"), i);
		a.displayColor = theApp.GetProfileInt(_T("options"), tag, 0);

		// Quick-check?

		_snwprintf(tag, 1024, _T("%d_checkOnly"), i);
		a.checkOnly = theApp.GetProfileInt(_T("options"), tag, 0) ? true:false;

		// Webmail script

		_snwprintf(tag, 1024, _T("%d_webmailScript"), i);
		a.webmailScript = theApp.GetProfileString(_T("options"), tag, _T(""));

		// Server address

		_snwprintf(tag, 1024, _T("%d_serverAddress"), i);
		a.serverAddress = theApp.GetProfileString(_T("options"), tag, _T(""));

		// Server port

		_snwprintf(tag, 1024, _T("%d_serverPort"), i);
		a.serverPort = theApp.GetProfileInt(_T("options"), tag, 110);

		// Timeout

		_snwprintf(tag, 1024, _T("%d_timeout"), i);
		a.timeout = theApp.GetProfileInt(_T("options"), tag, 60);

		// Preview lines

		_snwprintf(tag, 1024, _T("%d_previewLines"), i);
		a.previewLines = theApp.GetProfileInt(_T("options"), tag, 999);

		// Username

		_snwprintf(tag, 1024, _T("%d_userName"), i);
		a.userName = theApp.GetProfileString(_T("options"), tag, _T(""));

		// Userpass
		//
		// We used to store it in "n_userPass" as a string, but now we store it in "n_userPassword" as a binary.
		// So, we first try to get it from the userPassword entry, and if not there, we revert back to the old
		// style. Next time we write it out, we'll write out blanks to the old one, just so we don't get confused.

		LPBYTE		binpass = 0;
		unsigned int	binpassLength = 0;
		_snwprintf(tag, 1024, _T("%d_userPassword"), i);
		if (theApp.GetProfileBinary(_T("options"), tag, &binpass, &binpassLength))
		{
			a.userPass = atow(decrypt((const char *) binpass, binpassLength));
			delete[] binpass;
		}

		// If it's not stored in the new location, try the old location

		else
		{
			_snwprintf(tag, 1024, _T("%d_userPass"), i);
			wstring	pass = theApp.GetProfileString(_T("options"), tag, _T(""));
			binpassLength = pass.length();
			a.userPass = atow(decrypt(wtoa(pass).c_str(), binpassLength));
		}

		// Alarm

		_snwprintf(tag, 1024, _T("%d_alarm"), i);
		a.alarmSoundEnable = theApp.GetProfileInt(_T("options"), tag, 0) ? true:false;

		// Alarm file

		_snwprintf(tag, 1024, _T("%d_alarmFile"), i);
		a.alarmSoundFile = theApp.GetProfileString(_T("options"), tag, _T(""));

		// Alarm popup

		_snwprintf(tag, 1024, _T("%d_alarmPopup"), i);
		a.alarmPopup = theApp.GetProfileInt(_T("options"), tag, 1) ? true:false;

		// 3D popup

		_snwprintf(tag, 1024, _T("%d_3dPopup"), i);
		a.alarm3DPopup = theApp.GetProfileInt(_T("options"), tag, 1) ? true:false;

		// Reminder

		_snwprintf(tag, 1024, _T("%d_reminder"), i);
		a.reminderSoundEnable = theApp.GetProfileInt(_T("options"), tag, 0) ? true:false;

		// Reminder file

		_snwprintf(tag, 1024, _T("%d_reminderFile"), i);
		a.reminderSoundFile = theApp.GetProfileString(_T("options"), tag, _T(""));

		// Reminder seconds

		_snwprintf(tag, 1024, _T("%d_reminderSeconds"), i);
		a.reminderSeconds = theApp.GetProfileInt(_T("options"), tag, 500);

		// Run parms

		_snwprintf(tag, 1024, _T("%d_alarm"), i);
//		a.alarmRunParms.readConfiguration(tag);

		_snwprintf(tag, 1024, _T("%d_reminder"), i);
//		a.reminderRunParms.readConfiguration(tag);

		// How often to check mail

		_snwprintf(tag, 1024, _T("%d_checkMailSeconds"), i);
		a.checkMailSeconds = theApp.GetProfileInt(_T("options"), tag, 500);

		// Disabled?

		_snwprintf(tag, 1024, _T("%d_disabled"), i);
		a.disabled = theApp.GetProfileInt(_T("options"), tag, 0) ? true:false;

		// Other stuff

		a.checkMailTimer = 0;
		a.reminderTimer = 0;
		a.newPending = false;
		a.deleted = false;

		// Add it to the list

		push_back(a);
	}
#endif // _REGISTERED_ || _LITE_
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	AccountList::writeConfiguration() const
{
#if	defined(_REGISTERED_) || defined(_LITE_)
	int	index = 0;
	for (const_iterator i = begin(); i != end(); ++i, ++index)
	{
		if (i->deleted)
		{
			index--;
			continue;
		}

		const sAccount	&a = *i;
		wchar_t		tag[1024];

		// Account name

		_snwprintf(tag, 1024, _T("%d_accountName"), index);
		theApp.WriteProfileString(_T("options"), tag, a.accountName.c_str());

		// Use webmail

		_snwprintf(tag, 1024, _T("%d_useWebmail"), index);
		theApp.WriteProfileInt(_T("options"), tag, a.useWebmail ? 1:0);

		// Use color

		_snwprintf(tag, 1024, _T("%d_useColor"), index);
		theApp.WriteProfileInt(_T("options"), tag, a.useColor ? 1:0);

		_snwprintf(tag, 1024, _T("%d_displayColor"), index);
		theApp.WriteProfileInt(_T("options"), tag, a.displayColor);

		// Check only

		_snwprintf(tag, 1024, _T("%d_checkOnly"), index);
		theApp.WriteProfileInt(_T("options"), tag, a.checkOnly ? 1:0);

		// Webmail script

		_snwprintf(tag, 1024, _T("%d_webmailScript"), index);
		theApp.WriteProfileString(_T("options"), tag, a.webmailScript.c_str());

		// Server address

		_snwprintf(tag, 1024, _T("%d_serverAddress"), index);
		theApp.WriteProfileString(_T("options"), tag, a.serverAddress.c_str());

		// Server port

		_snwprintf(tag, 1024, _T("%d_serverPort"), index);
		theApp.WriteProfileInt(_T("options"), tag, a.serverPort);

		// Timeout

		_snwprintf(tag, 1024, _T("%d_timeout"), index);
		theApp.WriteProfileInt(_T("options"), tag, a.timeout);

		// Preview lines

		_snwprintf(tag, 1024, _T("%d_previewLines"), index);
		theApp.WriteProfileInt(_T("options"), tag, a.previewLines);

		// Username

		_snwprintf(tag, 1024, _T("%d_userName"), index);
		theApp.WriteProfileString(_T("options"), tag, a.userName.c_str());

		// Userpass

		_snwprintf(tag, 1024, _T("%d_userPassword"), index);
		unsigned int	binpassLength = a.userPass.length();
		const char *	binpass = encrypt(wtoa(a.userPass).c_str(), binpassLength);
		theApp.WriteProfileBinary(_T("options"), tag, (LPBYTE) binpass, binpassLength);

		_snwprintf(tag, 1024, _T("%d_userPass"), index);
		theApp.WriteProfileString(_T("options"), tag, _T(""));

		// Alarm

		_snwprintf(tag, 1024, _T("%d_alarm"), index);
		theApp.WriteProfileInt(_T("options"), tag, a.alarmSoundEnable ? 1:0);

		// Alarm file

		_snwprintf(tag, 1024, _T("%d_alarmFile"), index);
		theApp.WriteProfileString(_T("options"), tag, a.alarmSoundFile.c_str());

		// Alarm popup

		_snwprintf(tag, 1024, _T("%d_alarmPopup"), index);
		theApp.WriteProfileInt(_T("options"), tag, a.alarmPopup ? 1:0);

		// 3D popup

		_snwprintf(tag, 1024, _T("%d_3dPopup"), index);
		theApp.WriteProfileInt(_T("options"), tag, a.alarm3DPopup ? 1:0);

		// Reminder sound

		_snwprintf(tag, 1024, _T("%d_reminder"), index);
		theApp.WriteProfileInt(_T("options"), tag, a.reminderSoundEnable ? 1:0);

		// Reminder sound file

		_snwprintf(tag, 1024, _T("%d_reminderFile"), index);
		theApp.WriteProfileString(_T("options"), tag, a.reminderSoundFile.c_str());

		// Reminder seconds

		_snwprintf(tag, 1024, _T("%d_reminderSeconds"), index);
		theApp.WriteProfileInt(_T("options"), tag, a.reminderSeconds);

		// Run parms

		_snwprintf(tag, 1024, _T("%d_alarm"), index);
//		a.alarmRunParms.writeConfiguration(tag);

		_snwprintf(tag, 1024, _T("%d_reminder"), index);
//		a.reminderRunParms.writeConfiguration(tag);

		// How often to check mail

		_snwprintf(tag, 1024, _T("%d_checkMailSeconds"), index);
		theApp.WriteProfileInt(_T("options"), tag, a.checkMailSeconds);

		// Disabled?

		_snwprintf(tag, 1024, _T("%d_disabled"), index);
		theApp.WriteProfileInt(_T("options"), tag, a.disabled ? 1:0);
	}
#endif // _REGISTERED_ || _LITE_
}

// ---------------------------------------------------------------------------------------------------------------------------------
// AccountList.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

