// ---------------------------------------------------------------------------------------------------------------------------------
//  ______                 _ _ _      _     _       _     
// |  ____|               (_) | |    (_)   | |     | |    
// | |__   _ __ ___   __ _ _| | |     _ ___| |_    | |__  
// |  __| | '_ ` _ \ / _` | | | |    | / __| __|   | '_ \ 
// | |____| | | | | | (_| | | | |____| \__ \ |_  _ | | | |
// |______|_| |_| |_|\__,_|_|_|______|_|___/\__|(_)|_| |_|
//                                                        
//                                                        
//
// 3D Mail Effects (Email list handler class)
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

#ifndef	_H_EMAILLIST
#define _H_EMAILLIST

// ---------------------------------------------------------------------------------------------------------------------------------
// Files required by this module
// ---------------------------------------------------------------------------------------------------------------------------------

#include "Mime.h"

// ---------------------------------------------------------------------------------------------------------------------------------

class sEmail
{
public:
	sEmail() : size(0), receivedSize(0), newFlag(false), autoDeleteFlag(false), pendingFlag(false) {}
	Mime		mimeHeader;
	string		messageID;
	unsigned int	size;
	unsigned int	receivedSize;
	bool		newFlag;
	bool		autoDeleteFlag;
	bool		pendingFlag;
};

// ---------------------------------------------------------------------------------------------------------------------------------

class	EmailList : public list<sEmail>
{
public:

inline				EmailList() : list<sEmail>(0) {}
virtual		unsigned int	countNewEmails() const;
virtual		unsigned int	countPendingEmails() const;
virtual		void		markRead();
virtual		void		markAutoDelete(const WStringList & bySender, const WStringList & bySubject);

private:
};

#endif // _H_EMAILLIST
// ---------------------------------------------------------------------------------------------------------------------------------
// EmailList.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

