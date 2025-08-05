// ---------------------------------------------------------------------------------------------------------------------------------
//  __  __       _ _ _______ _                         _     _     
// |  \/  |     (_) |__   __| |                       | |   | |    
// | \  / | __ _ _| |  | |  | |__  _ __  ___  __ _  __| |   | |__  
// | |\/| |/ _` | | |  | |  | '_ \| '__|/ _ \/ _` |/ _` |   | '_ \ 
// | |  | | (_| | | |  | |  | | | | |  |  __/ (_| | (_| | _ | | | |
// |_|  |_|\__,_|_|_|  |_|  |_| |_|_|   \___|\__,_|\__,_|(_)|_| |_|
//                                                                 
//                                                                 
//
// 3D Mail Effects (Mail communications thread handler class)
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 12/04/2000 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2000, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_MAILTHREAD
#define _H_MAILTHREAD

// ---------------------------------------------------------------------------------------------------------------------------------
// Files required by this module
// ---------------------------------------------------------------------------------------------------------------------------------

#include "AccountList.h"

// ---------------------------------------------------------------------------------------------------------------------------------
// Forward declarations
// ---------------------------------------------------------------------------------------------------------------------------------

class	C3DmeDlg;

// ---------------------------------------------------------------------------------------------------------------------------------

class	MailThread : public CWinThread
{
	DECLARE_DYNCREATE(MailThread)

protected:
				MailThread(); // protected constructor used by dynamic creation
virtual				~MailThread();

public:
		enum	Command
		{
			CMD_CHECK,
			CMD_DELETE,
			CMD_RETRIEVE,
			CMD_TERMINATE
		};

		typedef	struct	tag_message
		{
			Command		command;
			sAccount	*account;
			string		messageID;
		} sMessage;

inline		void		queueMessage(const Command command, sAccount *account = NULL, string messageID = "")
		{
			sMessage	m;
			m.command = command;
			m.account = account;
			m.messageID = messageID;
			messageQueue.push_back(m);
		}

inline		void		clearQueue()
		{
			messageQueue.clear();
		}

inline		C3DmeDlg 	*&dialog()       {return _dialog;}
inline	const	C3DmeDlg * const &dialog() const {return _dialog;}

inline		sEmail	 	&lastRetrievedEmail()       {return _lastRetrievedEmail;}
inline	const	sEmail		&lastRetrievedEmail() const {return _lastRetrievedEmail;}


	//{{AFX_VIRTUAL(MailThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

protected:
		time_t		thisTime;
		time_t		lastTime;
		time_t		elapsedTime;
		sEmail		_lastRetrievedEmail;

		C3DmeDlg	*_dialog;
		list<sMessage>	messageQueue;

virtual		void		soundAlarm(const sAccount &a);
virtual		void		soundReminder(const sAccount &a) const;
virtual		void		checkMail(sMessage &message);
virtual		void		deleteMail(sMessage &message);
virtual		void		retrieveMail(sMessage &message);
virtual		void		terminate(sMessage &message);

	//{{AFX_MSG(MailThread)
	afx_msg	void OnTimer(unsigned int wparam, long lparam);
//	afx_msg	void OnTerminateThread(unsigned int wparam, long lparam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}

#endif // _H_MAILTHREAD
// ---------------------------------------------------------------------------------------------------------------------------------
// MailThread.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

