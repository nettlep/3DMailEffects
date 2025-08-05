// ---------------------------------------------------------------------------------------------------------------------------------
//  _     _   _             _     
// | |   | | | |           | |    
// | |__ | |_| |_ _ __     | |__  
// | '_ \| __| __| '_ \    | '_ \ 
// | | | | |_| |_| |_) | _ | | | |
// |_| |_|\__|\__| .__/ (_)|_| |_|
//               | |              
//               |_|              
//
// Description:
//
//   HTTP client
//
// Notes:
//
//   Best viewed with 8-character tabs and (at least) 132 columns
//
// History:
//
//   08/21/2001 by Paul Nettle: Original creation
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2001, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_HTTP
#define _H_HTTP

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

#include "Socket.h"

// ---------------------------------------------------------------------------------------------------------------------------------

class	http
{
public:
	// Construction/Destruction

				http();
virtual				~http();

	// Operators

	// Implementation

virtual		void		parseURL(const string & url, string & server, unsigned int & port, string & request, bool & ssl);
virtual		bool		parseResponseHeader(string & responseHeader);
virtual		void		addCookie(const string & cookie);
virtual		void		parseCookie(const string & cookie, string & name, string & value, string & path, string & domain) const;
virtual		void		connect(const string & url);
virtual		void		disconnect();
virtual		string		buildRequestHeader(const string & userAgent, const string & postData = "") const;
virtual		string		getCookieString() const;
virtual		wstring		sendRequest(const string & url, const string & userAgent = "", const string & postData = "", const unsigned int depth = 0);
virtual		wstring		sendRequestAndGetPage(const string & url, string & pageData, const string & userAgent = "", const string & postData = "", const unsigned int depth = 0);
virtual		wstring		getPage(const string & url, string & pageData, const string & userAgent = "");
virtual		wstring		postPage(const string & url, const string & postData, string & pageData, const string & userAgent = "");

static		string		get3DMEBrowserString();
static		wstring		statusCodeString(const unsigned int resp);

	// Accessors

inline		Socket &	socket()			{return *_socket;}
inline	const	Socket &	socket() const			{return *_socket;}
inline		StringList &	cookies()			{return _cookies;}
inline	const	StringList &	cookies() const			{return _cookies;}
inline		string &	currentServer()			{return _currentServer;}
inline	const	string &	currentServer() const		{return _currentServer;}
inline		string &	currentRequest()		{return _currentRequest;}
inline	const	string &	currentRequest() const		{return _currentRequest;}
inline		string &	referrer()			{return _referrer;}
inline	const	string &	referrer() const		{return _referrer;}
inline		string &	defaultUserAgent()		{return _defaultUserAgent;}
inline	const	string &	defaultUserAgent() const	{return _defaultUserAgent;}
inline		unsigned int &	currentPort()			{return _currentPort;}
inline	const	unsigned int	currentPort() const		{return _currentPort;}
inline		unsigned int &	timeout()			{return _timeout;}
inline	const	unsigned int	timeout() const			{return _timeout;}

	// Response header values

inline		unsigned int &	statusCode()			{return _statusCode;}
inline	const	unsigned int	statusCode() const		{return _statusCode;}
inline		unsigned int &	contentLength()			{return _contentLength;}
inline	const	unsigned int	contentLength() const		{return _contentLength;}
inline		string &	location()			{return _location;}
inline	const	string &	location() const		{return _location;}

private:
	// Explicitly deny these...

inline				http(http &rhs) {}
inline				operator=(http &rhs) {}

	// Data members

		Socket *	_socket;
		StringList	_cookies;
		string		_currentServer;
		string		_currentRequest;
		string		_referrer;
		string		_defaultUserAgent;
		unsigned int	_currentPort;
		unsigned int	_timeout;

		// Response header values

		unsigned int	_statusCode;
		unsigned int	_contentLength;
		string		_location;
};

#endif // _H_HTTP
// ---------------------------------------------------------------------------------------------------------------------------------
// http.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
