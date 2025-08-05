// ---------------------------------------------------------------------------------------------------------------------------------
//                       _____             __         _     
//     /\               |  __ \           / _|       | |    
//    /  \   _ __  _ __ | |__) |_ __  ___| |_ ___    | |__  
//   / /\ \ | '_ \| '_ \|  ___/| '__|/ _ \  _/ __|   | '_ \ 
//  / ____ \| |_) | |_) | |    | |  |  __/ | \__ \ _ | | | |
// /_/    \_\ .__/| .__/|_|    |_|   \___|_| |___/(_)|_| |_|
//          | |   | |                                       
//          |_|   |_|                                       
//
// Application preferences dialog & data manager
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 11/19/2000 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2000, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_APPPREFS
#define _H_APPPREFS

// ---------------------------------------------------------------------------------------------------------------------------------
// Files required by this module
// ---------------------------------------------------------------------------------------------------------------------------------

#include "AccountList.h"

// ---------------------------------------------------------------------------------------------------------------------------------

class	AppPrefs
{
public:
inline		AccountList	&accounts()                         {return _accounts;}
inline	const	AccountList	&accounts() const                   {return _accounts;}
inline		int		&currentAccount()                   {return _currentAccount;}
inline	const	int		&currentAccount() const             {return _currentAccount;}

protected:

		AccountList	_accounts;
		int		_currentAccount;
};

#endif // _H_APPPREFS
// ---------------------------------------------------------------------------------------------------------------------------------
// AppPrefs.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

