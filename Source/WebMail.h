// ---------------------------------------------------------------------------------------------------------------------------------
//  _   _   _      _     __  __       _ _     _     
// | | | | | |    | |   |  \/  |     (_) |   | |    
// | | | | | | ___| |__ | \  / | __ _ _| |   | |__  
// | | | | | |/ _ \ '_ \| |\/| |/ _` | | |   | '_ \ 
// |  V _ V  |  __/ |_) | |  | | (_| | | | _ | | | |
//  \__/ \__/ \___|_.__/|_|  |_|\__,_|_|_|(_)|_| |_|
//                                                  
//                                                  
//
// WebMail Script management
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 03/05/2001 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2001, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_WEBMAIL
#define _H_WEBMAIL

// ---------------------------------------------------------------------------------------------------------------------------------
// Files required by this module
// ---------------------------------------------------------------------------------------------------------------------------------

#include "http.h"

// ---------------------------------------------------------------------------------------------------------------------------------

class	Var
{
public:
			Var() : pointer(0) {};
	string		name;
	string		contents;
	unsigned int	pointer;
};

// ---------------------------------------------------------------------------------------------------------------------------------

typedef	vector<string>	StringVect;
typedef	vector<Var>	VarVect;

// ---------------------------------------------------------------------------------------------------------------------------------

class	WebMail
{
public:
		enum		ENTRYPOINT
				{
					EP_RETRIEVE,
					EP_DELETE
				};
	// Construction/Destruction

				WebMail(EmailList &oldMails, EmailList &newMails) :_oldMails(oldMails), _newMails(newMails) {};
virtual				~WebMail() {};

	// Operators

	// Implementation

virtual		wstring		execute(const wstring &filename, const ENTRYPOINT entry);

	// Accessors

inline	const	StringVect	&scriptLines() const {return _scriptLines;}
inline		StringVect	&scriptLines() {return _scriptLines;}

inline	const	VarVect		&variables() const {return _variables;}
inline		VarVect		&variables() {return _variables;}

inline	const	string		&userName() const {return _userName;}
inline		string		&userName() {return _userName;}

inline	const	string		&userPass() const {return _userPass;}
inline		string		&userPass() {return _userPass;}

inline	const	string		&messageIDList() const {return _messageIDList;}
inline		string		&messageIDList() {return _messageIDList;}

inline	const	string		&deleteMe() const {return _deleteMe;}
inline		string		&deleteMe() {return _deleteMe;}

inline	const	http		&website() const {return _website;}
inline		http		&website() {return _website;}

inline	const	EmailList	&oldMails() const {return _oldMails;}
inline		EmailList	&oldMails() {return _oldMails;}

inline	const	EmailList	&newMails() const {return _newMails;}
inline		EmailList	&newMails() {return _newMails;}

private:
	// Data members

		VarVect		_variables;
		StringVect	_scriptLines;
		string		_userName;
		string		_userPass;
		string		_messageIDList;
		string		_deleteMe;
		http		_website;
		EmailList	&_oldMails;
		EmailList	&_newMails;
		string		currentOnError;
		list<string>	onErrorStack;
		list<unsigned int> ipStack;

	// Utilitarian

virtual		wstring		loadScript(const wstring &filename);
virtual		bool		parseLine(const unsigned int ip, string &control, string &parms);
virtual		unsigned int	findLabel(const string &label) const;
virtual		bool		parseParms(const string &parms, StringVect &out);
virtual		bool		varExist(const string &name) const;
virtual		bool		setVar(const Var &var);
virtual		bool		getVar(const string &name, Var &var) const;
virtual		bool		resolveParm(const string &str, string &result) const;
virtual		bool		resolveIndex(const string &str, unsigned int &index) const;
virtual		bool		resolveCount(const string &str, int &index) const;
virtual		bool		handleOnError(unsigned int &currentIP);
virtual		void		push(const unsigned int currentIP, const string &routineName);
virtual		bool		pop(unsigned int &currentIP);

virtual		bool		_setvar(unsigned int &currentIP, const StringVect &parms);
virtual		bool		_posthttp(unsigned int &currentIP, const StringVect &parms);
virtual		bool		_gethttp(unsigned int &currentIP, const StringVect &parms);
virtual		bool		_onerror(unsigned int &currentIP, const StringVect &parms);
virtual		bool		_call(unsigned int &currentIP, const StringVect &parms);
virtual		bool		_find(unsigned int &currentIP, const StringVect &parms);
virtual		bool		_rfind(unsigned int &currentIP, const StringVect &parms);
virtual		bool		_erase(unsigned int &currentIP, const StringVect &parms);
virtual		bool		_skip(unsigned int &currentIP, const StringVect &parms);
virtual		bool		_goto(unsigned int &currentIP, const StringVect &parms);
virtual		bool		_return(unsigned int &currentIP, const StringVect &parms);
virtual		bool		_dumpvar(unsigned int &currentIP, const StringVect &parms);
virtual		bool		_addmail(unsigned int &currentIP, const StringVect &parms);
virtual		bool		_remtags(unsigned int &currentIP, const StringVect &parms);
virtual		bool		_unhtml(unsigned int &currentIP, const StringVect &parms);
virtual		bool		_strip(unsigned int &currentIP, const StringVect &parms);
virtual		bool		_convertReservedURI(unsigned int &currentIP, const StringVect &parms);
virtual		bool		_setCookie(unsigned int &currentIP, const StringVect &parms);
virtual		bool		_setReferrer(unsigned int &currentIP, const StringVect &parms);
virtual		bool		_setUserAgent(unsigned int &currentIP, const StringVect &parms);
virtual		bool		_dumpCookies(unsigned int &currentIP, const StringVect &parms);
};

#endif // _H_WEBMAIL
// ---------------------------------------------------------------------------------------------------------------------------------
// WebMail.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

