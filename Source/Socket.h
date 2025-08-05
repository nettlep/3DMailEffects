// ---------------------------------------------------------------------------------------------------------------------------------
//   _____            _         _       _     
//  / ____|          | |       | |     | |    
// | (___   ___   ___| | __ ___| |_    | |__  
//  \___ \ / _ \ / __| |/ // _ \ __|   | '_ \ 
//  ____) | (_) | (__|   <|  __/ |_  _ | | | |
// |_____/ \___/ \___|_|\_\\___|\__|(_)|_| |_|
//                                            
//                                            
//
// Description:
//
//   Home-brew socket class for Windows (TCP only)
//
// Notes:
//
//   Best viewed with 8-character tabs and (at least) 132 columns
//
// History:
//
//   08/19/2001 by Paul Nettle: Original creation
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2001, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_SOCKET
#define _H_SOCKET

// ---------------------------------------------------------------------------------------------------------------------------------

class	Socket
{
public:

		enum			{READ_BUF_SIZE = 64 * 1024};

	// Construction/Destruction	

					Socket();
virtual					~Socket();

	// Implementation (public)

static		bool			isIP(const string & str);
static		string			reverseIP(const string & str);
static		unsigned int		ipFromString(const string & ipString);
static		wstring			lookup(const string & address, string & ip);
virtual		wstring			connect(const string & address, const unsigned int port);
virtual		wstring			connectNoProxy(const string & address, const unsigned int port);
virtual		wstring			connectSocks4x(const string & address, const unsigned int port);
virtual		wstring			connectSocks5(const string & address, const unsigned int port);
virtual		wstring			connectCern(const string & address, const unsigned int port);
virtual		wstring			disconnect();
virtual		wstring			setBlocking(const bool block);
virtual		wstring			send(const string & str);
virtual		wstring			send(const char * buf, const unsigned int length);
virtual		wstring			recvUntil(string & str, unsigned int & bytesReceived, const StringList & terminatorList, const unsigned int maxBytesToReceive = 0xffffffff);
virtual		wstring			recv(string & str, unsigned int & bytesReceived, const unsigned int length = 0xffffffff);
virtual		wstring			recv(char * buf, unsigned int & bytesReceived, const unsigned int length);
virtual		wstring			lastError();

static		wstring			errorString(const int err);

	// Accessors

inline		CAsyncSocket &		winSocket()			{return _winSocket;}
inline	const	CAsyncSocket &		winSocket() const		{return _winSocket;}
inline		bool &			created()			{return _created;}
inline	const	bool			created() const			{return _created;}
inline		bool &			connected()			{return _connected;}
inline	const	bool			connected() const		{return _connected;}
inline		bool &			disableProxy()			{return _disableProxy;}
inline	const	bool			disableProxy() const		{return _disableProxy;}
inline		unsigned int &		timeout()			{return _timeout;}
inline	const	unsigned int		timeout() const			{return _timeout;}
inline		char *&			readBuffer()			{return _readBuffer;}
inline	const	char *			readBuffer() const		{return _readBuffer;}
inline		unsigned int &		readBufferRead()		{return _readBufferRead;}
inline	const	unsigned int		readBufferRead() const		{return _readBufferRead;}
inline		unsigned int &		readBufferOccupied()		{return _readBufferOccupied;}
inline	const	unsigned int		readBufferOccupied() const	{return _readBufferOccupied;}

	// Utilitiarian

inline	const	unsigned int		readBufferAvail()		{return readBufferOccupied() - readBufferRead();}

private:

	// Implementation (private)

virtual		wstring			primeBuffer();

	// Explicitly deny these...

inline					Socket(Socket &rhs) {}
inline					operator=(Socket &rhs) {}

	// Data members

		CAsyncSocket		_winSocket;
		bool			_created;
		bool			_connected;
		bool			_disableProxy;
		unsigned int		_timeout;
		char *			_readBuffer;
		unsigned int		_readBufferRead;
		unsigned int		_readBufferOccupied;
};

#endif // _H_SOCKET
// ---------------------------------------------------------------------------------------------------------------------------------
// Socket.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
