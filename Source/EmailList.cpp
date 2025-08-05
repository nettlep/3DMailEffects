// ---------------------------------------------------------------------------------------------------------------------------------
//  ______                 _ _ _      _     _
// |  ____|               (_) | |    (_)   | |
// | |__   _ __ ___   __ _ _| | |     _ ___| |_      ___ _ __  _ __
// |  __| | '_ ` _ \ / _` | | | |    | / __| __|    / __| '_ \| '_ \
// | |____| | | | | | (_| | | | |____| \__ \ |_  _ | (__| |_) | |_) |
// |______|_| |_| |_|\__,_|_|_|______|_|___/\__|(_) \___| .__/| .__/
//                                                      | |   | |
//                                                      |_|   |_|
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

#include "stdafx.h"
#include "3Dme.h"
#include "EmailList.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// --------------------------------------------------------------------------------------------------------------------------------

unsigned int	EmailList::countNewEmails() const
{
	unsigned int	count = 0;
	for (const_iterator i = begin(); i != end(); ++i)
	{
		if (i->newFlag && !i->autoDeleteFlag) count++;
	}
	return count;
}

// --------------------------------------------------------------------------------------------------------------------------------

unsigned int	EmailList::countPendingEmails() const
{
	unsigned int	count = 0;
	if (size())
	{
		for (const_iterator i = begin(); i != end(); ++i)
		{
			if (i->pendingFlag) count++;
		}
	}
	return count;
}

// --------------------------------------------------------------------------------------------------------------------------------

void	EmailList::markRead()
{
	for (iterator i = begin(); i != end(); ++i)
	{
		i->newFlag = false;
		i->pendingFlag = false;
	}
}

// --------------------------------------------------------------------------------------------------------------------------------

void	EmailList::markAutoDelete(const WStringList & bySender, const WStringList & bySubject)
{
	for (iterator i = begin(); i != end(); ++i)
	{
		sEmail &	cur = *i;

		for (WStringList::const_iterator j = bySender.begin(); j != bySender.end(); ++j)
		{
			if (cur.mimeHeader.from() == *j)
			{
				cur.autoDeleteFlag = true;
				break;
			}
		}
		for (WStringList::const_iterator j = bySubject.begin(); j != bySubject.end(); ++j)
		{
			if (cur.mimeHeader.subject() == *j)
			{
				cur.autoDeleteFlag = true;
				break;
			}
		}

		if (theApp.prefs().other()[_T("autoDeleteSpam")].booleanValue())
		{
			if (cur.mimeHeader.spam()) cur.autoDeleteFlag = true;
		}
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------
// EmailList.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

