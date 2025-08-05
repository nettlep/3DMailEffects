// ---------------------------------------------------------------------------------------------------------------------------------
//                        _     
//                       | |    
//  _ __   ___  _ __     | |__  
// | '_ \ / _ \| '_ \    | '_ \ 
// | |_) | (_) | |_) | _ | | | |
// | .__/ \___/| .__/ (_)|_| |_|
// | |         | |              
// |_|         |_|              
//
// 3D Mail Effects (Pop3 protocol class)
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 11/16/2000 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2000, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_POP
#define _H_POP

// ---------------------------------------------------------------------------------------------------------------------------------
// Files required by this module
// ---------------------------------------------------------------------------------------------------------------------------------

#include "EmailList.h"
#include "Socket.h"

// ---------------------------------------------------------------------------------------------------------------------------------

class	pop
{
public:
	// Construction/Destruction

				pop();
virtual				~pop();

	// Implementation

		bool		isNew(sEmail &email, const EmailList &oldEmails) const;
		bool		isPending(sEmail &email, const EmailList &oldEmails) const;
		void		initiateSession(const string &address, const unsigned int port, const string &username, const string &password);
		wstring		queryMail(const bool checkOnly, const string &address, const unsigned int port, const string &username, const string &password, EmailList &oldMails, EmailList &emails);
		wstring		retrieveMail(const string &address, const unsigned int port, const string &username, const string &password, const string &messageID, sEmail &email);
		wstring		deleteMail(const string &address, const unsigned int port, const string &username, const string &password, const string &messageID);
		bool		sendCommand(const string &command, string & result, unsigned int &totalBytesReceived, bool multiline = false);

inline		void		setTimeout(unsigned int timeout) {socket().timeout() = timeout;}

	// Accessors

inline		unsigned int &	previewLines()		{return _previewLines;}
inline	const	unsigned int &	previewLines() const	{return _previewLines;}
inline		Socket &	socket()		{return _socket;}
inline	const	Socket &	socket() const		{return _socket;}

private:
	// Explicitly deny these...

inline				pop(pop &rhs) {}
inline				operator=(pop &rhs) {}

	// Data members

		unsigned int	_previewLines;
		Socket		_socket;
};

#endif // _H_POP
// ---------------------------------------------------------------------------------------------------------------------------------
// pop.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

