// ---------------------------------------------------------------------------------------------------------------------------------
//                                       _   _      _     _       _     
//     /\                               | | | |    (_)   | |     | |    
//    /  \    ___  ___  ___  _   _ _ __ | |_| |     _ ___| |_    | |__  
//   / /\ \  / __|/ __|/ _ \| | | | '_ \| __| |    | / __| __|   | '_ \ 
//  / ____ \| (__| (__| (_) | |_| | | | | |_| |____| \__ \ |_  _ | | | |
// /_/    \_\\___|\___|\___/ \__,_|_| |_|\__|______|_|___/\__|(_)|_| |_|
//                                                                      
//                                                                      
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

#ifndef	_H_ACCOUNTLIST
#define _H_ACCOUNTLIST

// ---------------------------------------------------------------------------------------------------------------------------------
// Required includes
// ---------------------------------------------------------------------------------------------------------------------------------

#include "EmailList.h"

// ---------------------------------------------------------------------------------------------------------------------------------

typedef	struct
{
	// Account specific stuffs

	wstring		accountName;
	bool		useWebmail;
	bool		useColor;
	unsigned int	displayColor;
	wstring		webmailScript;
	wstring		serverAddress;
	unsigned int	serverPort;
	bool		checkOnly;
	unsigned int	timeout;
	unsigned int	previewLines;
	wstring		userName;
	wstring		userPass;
	unsigned int	checkMailSeconds;

	// Alarms

	bool		alarmPopup;
	bool		alarm3DPopup;
	bool		alarmSoundEnable;
	wstring		alarmSoundFile;
//	RunParms	alarmRunParms;

	// Reminders

	bool		reminderSoundEnable;
	wstring		reminderSoundFile;
	unsigned int	reminderSeconds;
//	RunParms	reminderRunParms;
	int		reminderTimer;

	// Emails

	EmailList	emailList;

	// Internal

	int		checkMailTimer;
	bool		newPending;
	bool		disabled;
	bool		deleted;
	wstring		lastError;
} sAccount;

// ---------------------------------------------------------------------------------------------------------------------------------

class	AccountList : public list<sAccount>
{
public:
				AccountList() : list<sAccount>() {};
virtual				~AccountList() {};

		enum		{ALL_ACCOUNTS = -1};

virtual		unsigned int	actualSize() const;
virtual		unsigned int	countErrors() const;
virtual		unsigned int	countActive() const;
virtual		unsigned int	countTotalEmails() const;
virtual		unsigned int	countNewEmails() const;
virtual		void		resetCheckMailTimers(const int currentAccount);
virtual		void		resetReminderTimers(const int currentAccount);
virtual		void		markRead(const int currentAccount);
virtual		void		readConfiguration(const unsigned int count);
virtual		void		writeConfiguration() const;
};

#endif // _H_ACCOUNTLIST
// ---------------------------------------------------------------------------------------------------------------------------------
// AccountList.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

