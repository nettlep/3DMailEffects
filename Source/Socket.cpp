// ---------------------------------------------------------------------------------------------------------------------------------
//   _____            _         _                        
//  / ____|          | |       | |                       
// | (___   ___   ___| | __ ___| |_      ___ _ __  _ __  
//  \___ \ / _ \ / __| |/ // _ \ __|    / __| '_ \| '_ \ 
//  ____) | (_) | (__|   <|  __/ |_  _ | (__| |_) | |_) |
// |_____/ \___/ \___|_|\_\\___|\__|(_) \___| .__/| .__/ 
//                                          | |   | |    
//                                          |_|   |_|    
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

#include "stdafx.h"
#include "3Dme.h"
#include "Socket.h"
#include "http.h"
#include "mime.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

#define	ERROR_STRING_WRAPPER(a)	{wstring b = a; if (b.length()) return b;}

// ---------------------------------------------------------------------------------------------------------------------------------

	Socket::Socket() : _created(false), _connected(false), _disableProxy(false), _timeout(0), _readBuffer(0), _readBufferRead(0), _readBufferOccupied(0)
{
	LOGBLOCK("Socket::Socket()");

	// Clear the last error

	WSASetLastError(0);

	// Create the CAsyncSocket

	if (!winSocket().Create(0, SOCK_STREAM))
	{
		LOG("Unable to create socket", Logger::LOG_CRIT);
		return;
	}

	// Set the socket options

	BOOL	val = TRUE;
	if (!winSocket().SetSockOpt(SO_REUSEADDR, &val, sizeof(val)))
	{
		LOG("Unable to set socket options", Logger::LOG_CRIT);
		return;
	}

	// No ASYNC callbacks will be used...

	winSocket().AsyncSelect(0);

	// Read buffer...

	readBuffer() = new char[READ_BUF_SIZE];

	// We're all setup

	created() = true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

	Socket::~Socket()
{
	LOGBLOCK("Socket::~Socket()");

	delete[] readBuffer();

	disconnect();
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	Socket::isIP(const string & str)
{
	string	s = str;
	stripLeadingWS(s);
	stripTrailingWS(s);

	// Convert the string to dotted values, making sure each one is in range

	string::size_type	idx = 0;

	// Part 1

	unsigned int	part1 = atoi(s.substr(idx).c_str());
	if (part1 > 255) return false;

	idx = s.find(".", idx);
	if (idx == string::npos) return false;
	++idx;

	// Part 2

	unsigned int	part2 = atoi(s.substr(idx).c_str());
	if (part2 > 255) return false;

	idx = s.find(".", idx);
	if (idx == string::npos) return false;
	++idx;

	// Part 3

	unsigned int	part3 = atoi(s.substr(idx).c_str());
	if (part3 > 255) return false;

	idx = s.find(".", idx);
	if (idx == string::npos) return false;
	++idx;

	// Part 4

	unsigned int	part4 = atoi(s.substr(idx).c_str());
	if (part4 > 255) return false;

	// Next, convert our numeric back to a string, to make sure it matches

	char	dsp[90];
	sprintf(dsp, "%d.%d.%d.%d", part1, part2, part3, part4);

	return s == string(dsp);
}

// ---------------------------------------------------------------------------------------------------------------------------------

string	Socket::reverseIP(const string & str)
{
	string	s = str;
	stripLeadingWS(s);
	stripTrailingWS(s);

	// Convert the string to dotted values, making sure each one is in range

	string::size_type	idx = 0;

	// Part 1

	unsigned int	part1 = atoi(s.substr(idx).c_str());
	if (part1 > 255) return "";

	idx = s.find(".", idx);
	if (idx == string::npos) return "";
	++idx;

	// Part 2

	unsigned int	part2 = atoi(s.substr(idx).c_str());
	if (part2 > 255) return "";

	idx = s.find(".", idx);
	if (idx == string::npos) return "";
	++idx;

	// Part 3

	unsigned int	part3 = atoi(s.substr(idx).c_str());
	if (part3 > 255) return "";

	idx = s.find(".", idx);
	if (idx == string::npos) return "";
	++idx;

	// Part 4

	unsigned int	part4 = atoi(s.substr(idx).c_str());
	if (part4 > 255) return "";

	// Next, convert our numeric back to a string, to make sure it matches

	char	dsp[90];
	sprintf(dsp, "%d.%d.%d.%d", part4, part3, part2, part1);

	return string(dsp);
}

// ---------------------------------------------------------------------------------------------------------------------------------

unsigned int	Socket::ipFromString(const string & ipString)
{
	// Get the hostent structure

	struct	hostent *	he = gethostbyname(ipString.c_str());
	if (!he) return 0;

	// Get the ip address in string format

	unsigned int	intip =		((unsigned char) he->h_addr_list[0][3] << 24) |
					((unsigned char) he->h_addr_list[0][2] << 16) |
					((unsigned char) he->h_addr_list[0][1] <<  8) |
					((unsigned char) he->h_addr_list[0][0] <<  0);

	return	intip;
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	Socket::lookup(const string & address, string & ip)
{
	// Validate input

	if (!address.length()) return _T("No address specified!");

	// Don't bother trying to lookup an IP address

	if (isIP(address))
	{
		ip = address;
		return _T("");
	}

	// Get the hostent structure

	struct	hostent *	he = gethostbyname(address.c_str());
	if (!he) return wstring(_T("Unable to locate [DNS] ")) + atow(address);

	// Get the ip address in string format

	ip = 	integerToString((unsigned char) he->h_addr_list[0][0]) + "." +
		integerToString((unsigned char) he->h_addr_list[0][1]) + "." +
		integerToString((unsigned char) he->h_addr_list[0][2]) + "." +
		integerToString((unsigned char) he->h_addr_list[0][3]);
	return _T("");
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	Socket::connect(const string & address, const unsigned int port)
{
	LOGBLOCK("Socket::connect(" + address + ", " + integerToString(port) + ")");

	// ALWAYS make sure the socket was created

	if (!created()) return wstring(_T("Unable to connect (no socket)"));

	// In case we're connected, disconnect

	ERROR_STRING_WRAPPER(disconnect());

	// Block

	ERROR_STRING_WRAPPER(setBlocking(true));

	// Connect to the host (proxy dependent)

	wstring	type = theApp.prefs().proxy()[_T("proxyType")].stringValue();
	if (disableProxy()) type = _T("None");

	if (type == _T("SOCKS4") || type == _T("SOCKS4a (w/DNS)"))
	{
		ERROR_STRING_WRAPPER(connectSocks4x(address, port));
	}
	else if (type == _T("SOCKS5"))
	{
		ERROR_STRING_WRAPPER(connectSocks5(address, port));
	}
	else if (type == _T("CERN/HTTP"))
	{
		ERROR_STRING_WRAPPER(connectCern(address, port));
	}
	else
	{
		ERROR_STRING_WRAPPER(connectNoProxy(address, port));
	}

	// We're officially connected...

	connected() = true;

	// Turn blocking back off

	ERROR_STRING_WRAPPER(setBlocking(false));

	// Done

	return _T("");
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	Socket::connectNoProxy(const string & address, const unsigned int port)
{
	if (!created()) return wstring(_T("connectNoProxy failed (no socket)"));

	// Get the IP from the address, do DNS query if needed

	string	ipString;
	ERROR_STRING_WRAPPER(lookup(address, ipString));

	// Direct connection

	if (!winSocket().Connect(atow(ipString).c_str(), port))
	{
		return wstring(_T("Unable to connect to ")) + atow(address) + _T("\n") + lastError();
	}

	return _T("");
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	Socket::connectSocks4x(const string & address, const unsigned int port)
{
	LOGFUNC("Socket::connectSocks4x");

	if (!created()) return wstring(_T("connectSocks4x failed (no socket)"));

	// Always need to do DNS lookup on the proxy server, if it's not already an IP address

	string	proxyIP;
	LOG("Looking up the proxy server...");
	ERROR_STRING_WRAPPER(lookup(wtoa(theApp.prefs().proxy()[_T("proxyHost")].stringValue()), proxyIP));

	// Connect to the proxy server

	LOG("Connecting to proxy server..." + wtoa(theApp.prefs().proxy()[_T("proxyHost")].stringValue()) + ":" + integerToString(theApp.prefs().proxy()[_T("proxyPort")].integerValue()));
	if (!winSocket().Connect(atow(proxyIP).c_str(), theApp.prefs().proxy()[_T("proxyPort")].integerValue()))
	{
		return wstring(_T("Unable to connect to proxy ")) + theApp.prefs().proxy()[_T("proxyHost")].stringValue() + _T(":") + integerToWString(theApp.prefs().proxy()[_T("proxyPort")].integerValue()) + _T("\n") + lastError();
	}

	// Are we responsible for our own DNS?

	string	destIP;
	if (!theApp.prefs().proxy()[_T("proxyResolve")].booleanValue())
	{
		LOG("Performing my own DNS lookup for: " + address);
		ERROR_STRING_WRAPPER(lookup(address, destIP));
	}

	// Let the proxy server do it...

	else
	{
		destIP = "0.0.0.1";
	}

	// Build the proxy server connect request
	//
	//		+----+----+----+----+----+----+----+----+----+----+....+----+
	//		| VN | CD | DSTPORT |      DSTIP        | USERID       |NULL|
	//		+----+----+----+----+----+----+----+----+----+----+....+----+
	// # of bytes:	   1    1      2              4           variable       1

#pragma	pack(1)
	struct	tag_s4_request
	{
		unsigned char	version;
		unsigned char	command;
		unsigned short	dstPort;
		unsigned int	dstIP;
		char		userID[4];
		unsigned char	nullByte;
	} request;
#pragma	pack()

	memset(&request, 0, sizeof(request));
	request.version = 4;
	request.command = 1;
	request.dstPort = ((port & 0xff) << 8) | ((port >> 8) & 0xff);
	request.dstIP = ipFromString(destIP);
	request.userID[0] = '3';
	request.userID[1] = 'D';
	request.userID[2] = 'M';
	request.userID[3] = 'E';
	request.nullByte = 0;

	// the output buffer (so we can do this in one packet)

	char		buf[1024];
	unsigned int	bufSize = sizeof(request);
	memcpy(buf, &request, bufSize);

	// Do we need to append the hostname onto the end of the packet?

	if (theApp.prefs().proxy()[_T("proxyResolve")].booleanValue())
	{
		strcpy(&buf[bufSize], address.c_str());
		bufSize += address.length() + 1;
	}

	// Send the buffer

	LOG("Sending request to proxy server...");
	ERROR_STRING_WRAPPER(send(buf, bufSize));

	// Get the response
	//
	//		+----+----+----+----+----+----+----+----+
	//		| VN | CD | DSTPORT |      DSTIP        |
	//		+----+----+----+----+----+----+----+----+
	// # of bytes:	   1    1      2              4

	LOG("Receiving response...");
	ERROR_STRING_WRAPPER(recv((char *) buf, bufSize, 8));
	if (bufSize != 8) return _T("Proxy not responding properly");
	if (buf[0] != 0) return _T("Proxy not responding properly");

	// 90: request granted
	// 91: request rejected or failed
	// 92: request rejected becasue SOCKS server cannot connect to
	//     identd on the client
	// 93: request rejected because the client program and identd
	//     report different user-ids

	if (buf[1] == 91) return _T("Proxy connection rejected/failed");
	if (buf[1] == 92) return _T("You must run ident to use this proxy");
	if (buf[1] == 93) return _T("Invalid userID to proxy");
	if (buf[1] != 90) return _T("Unknown response from proxy");

	// Success!

	LOG("Connected to destination through proxy");
	return _T("");
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	Socket::connectSocks5(const string & address, const unsigned int port)
{
	LOGFUNC("Socket::connectSocks5");

	if (!created()) return wstring(_T("connectSocks5 failed (no socket)"));

	// Always need to do DNS lookup on the proxy server, if it's not already an IP address

	string	proxyIP;
	LOG("Looking up the proxy server...");
	ERROR_STRING_WRAPPER(lookup(wtoa(theApp.prefs().proxy()[_T("proxyHost")].stringValue()), proxyIP));

	// Connect to the proxy server

	LOG("Connecting to proxy server..." + wtoa(theApp.prefs().proxy()[_T("proxyHost")].stringValue()) + ":" + integerToString(theApp.prefs().proxy()[_T("proxyPort")].integerValue()));
	if (!winSocket().Connect(atow(proxyIP).c_str(), theApp.prefs().proxy()[_T("proxyPort")].integerValue()))
	{
		return wstring(_T("Unable to connect to proxy ")) + theApp.prefs().proxy()[_T("proxyHost")].stringValue() + _T(":") + integerToWString(theApp.prefs().proxy()[_T("proxyPort")].integerValue()) + _T("\n") + lastError();
	}

	// Are we responsible for our own DNS?

	string	destIP;
	if (!theApp.prefs().proxy()[_T("proxyResolve")].booleanValue())
	{
		LOG("Performing my own DNS lookup for: " + address);
		ERROR_STRING_WRAPPER(lookup(address, destIP));
	}

	// Build the proxy server connect request
	//
	//		+-----+----------+--------+....+
	//		| VER | NMETHODS | METHODS     |
	//		+-----+----------+--------+....+
	// # of bytes:	   1       1       variable...

	unsigned int	bufSize = 0;
	char		buf[1024];

	buf[0] = 5; // version
	buf[1] = 2; // 2 methods
	buf[2] = 0; // authentication: [none]
	buf[3] = 2; // authentication: [user/pass]

	// Modify the request based on authentication info

	if (theApp.prefs().proxy()[_T("proxyAuth")].booleanValue())
	{
		LOG("Using authentication...");
		buf[1] = 2; // use both methods
		bufSize = 4;
	}
	else
	{
		LOG("Not using authentication...");
		buf[1] = 1; // use only one method (last byte never sent)
		bufSize = 3;
	}

	// Send the buffer

	LOG("Sending authentication method request to proxy...");
	ERROR_STRING_WRAPPER(send(buf, bufSize));

	// Get the response
	//
	//		+-----+--------+
	//		| VER | METHOD |
	//		+-----+--------+
	// # of bytes:	   1      1

	LOG("Receiving authentication method request from proxy...");
	ERROR_STRING_WRAPPER(recv((char *) buf, bufSize, 2));
	if (bufSize != 2) return _T("Proxy not responding properly");
	if (buf[0] != 5) return _T("Proxy not responding properly");
	if (buf[1] == 0xff) return _T("Proxy error: No acceptable methods of authentication");

	// Using username/password?

	if (buf[1] == 2)
	{
		bufSize = 0;

		buf[bufSize] = 5; // version
		++bufSize;

		buf[bufSize] = theApp.prefs().proxy()[_T("proxyUser")].stringValue().length();
		++bufSize;

		strcpy(&buf[bufSize], wtoa(theApp.prefs().proxy()[_T("proxyUser")].stringValue()).c_str());
		bufSize += theApp.prefs().proxy()[_T("proxyUser")].stringValue().length();

		buf[bufSize] = theApp.prefs().proxy()[_T("proxyPass")].stringValue().length();
		++bufSize;

		strcpy((char *) &buf[bufSize], wtoa(theApp.prefs().proxy()[_T("proxyPass")].stringValue()).c_str());
		bufSize += theApp.prefs().proxy()[_T("proxyPass")].stringValue().length();

		// Send the authentication

		LOG("Sending username/password to proxy...");
		ERROR_STRING_WRAPPER(send(buf, bufSize));

		// Get the response
		//
		//		+-----+--------+
		//		| VER | METHOD |
		//		+-----+--------+
		// # of bytes:	   1      1

		LOG("Receiving authentication response from proxy...");
		ERROR_STRING_WRAPPER(recv((char *) buf, bufSize, 2));
		if (bufSize != 2) return _T("Proxy not responding properly");
		if (buf[0] != 5) return _T("Proxy not responding properly");

		// 0 == authorization succeeded
		// any other value == failed

		if (buf[1] != 0) return _T("Proxy authentication failed");
	}

	// Send the connection request
	//
	//		+-----+-----+-----+---------+----------+----+----+
	//		| VER | CMD | RSV |  ATYPE  | DESTADDR | DSTPORT |
	//		+-----+-----+-----+---------+----------+----+----+
	// # of bytes:	   1     1     1        1     variable      2

	{
		bufSize = 0;

		buf[bufSize] = 5; // version
		++bufSize;

		buf[bufSize] = 1; // connect
		++bufSize;

		buf[bufSize] = 0; // reserved
		++bufSize;

		// Address type: 1 = IPV4 (dword), 3 = domainname (zstr)

		if (!theApp.prefs().proxy()[_T("proxyResolve")].booleanValue())
		{
			unsigned int	dstIP = ipFromString(destIP);
			buf[bufSize] = 1;
			++bufSize;

			buf[bufSize] = (dstIP >>  0) & 0xff;
			++bufSize;
			buf[bufSize] = (dstIP >>  8) & 0xff;
			++bufSize;
			buf[bufSize] = (dstIP >> 16) & 0xff;
			++bufSize;
			buf[bufSize] = (dstIP >> 24) & 0xff;
			++bufSize;
		}
		else
		{
			buf[bufSize] = 3; // Null-terminated domain name
			++bufSize;

			strcpy((char *) &buf[bufSize], address.c_str());
			bufSize += address.length();

			buf[bufSize] = 0; // Null-termination
			++bufSize;
		}

		// Port

		buf[bufSize] = (port >> 8) & 0xff;
		++bufSize;
		buf[bufSize] = (port) & 0xff;
		++bufSize;

		// Send the connection request

		LOG("Sending connection request to proxy...");
		ERROR_STRING_WRAPPER(send(buf, bufSize));

		// Send the connection request
		//
		//		+-----+-----+-----+---------+----------+----+----+
		//		| VER | REP | RSV |  ATYPE  | BINDADDR | BNDPORT |
		//		+-----+-----+-----+---------+----------+----+----+
		// # of bytes:	   1     1     1        1     variable      2

		// Read just the first four bytes

		LOG("Receiving connection request response from proxy...");
		ERROR_STRING_WRAPPER(recv((char *) buf, bufSize, 4));
		if (bufSize != 4) return _T("Proxy not responding properly");

		// Check the response code

		if (buf[0] != 5) return _T("Proxy not responding properly");

		// REPLY is one of:
		//
		// 0x00 Succeeded
		// 0x01 general SOCKS server failure
		// 0x02 connection not allowed by ruleset
		// 0x03 network unreachable
		// 0x04 host unreachable
		// 0x05 connection refused
		// 0x06 TTL expired
		// 0x07 command not supported
		// 0x08 address type not supported
		// 0x09 to 0xFF unassigned

		if (buf[1] != 0)
		{
			if (buf[1] == 1) return _T("Proxy error: general SOCKS server failure");
			if (buf[1] == 2) return _T("Proxy error: connection not allowed by ruleset");
			if (buf[1] == 3) return _T("Proxy error: network unreachable");
			if (buf[1] == 4) return _T("Proxy error: host unreachable");
			if (buf[1] == 5) return _T("Proxy error: connection refused");
			if (buf[1] == 6) return _T("Proxy error: TTL expired");
			if (buf[1] == 7) return _T("Proxy error: command not supported");
			if (buf[1] == 8) return _T("Proxy error: address type not supported");
			return _T("Proxy returned an unknown error");
		}

		// At this point, we don't care about the rest of the data to be sent, but we can't
		// leave it in the socket buffer, so read it in
		//
		// Get the bind address

		if (buf[3] == 1) // IPV4 type
		{
			char	foo[256];
			ERROR_STRING_WRAPPER(recv(foo, bufSize, 4));
			if (bufSize != 4) return _T("Proxy not responding properly");
		}
		else if (buf[3] == 4) // IPV6 type
		{
			char	foo[256];
			ERROR_STRING_WRAPPER(recv(foo, bufSize, 16));
			if (bufSize != 16) return _T("Proxy not responding properly");
		}
		else if (buf[3] == 3) // zstr type
		{
			// Receive until we get a zero

			char	foo[256];
			for(;;)
			{
				ERROR_STRING_WRAPPER(recv(foo, bufSize, 1));
				if (!bufSize) break;
				if (foo[0] == 0) break;
			}
		}

		// Read the port

		ERROR_STRING_WRAPPER(recv((char *) buf, bufSize, 2));
		if (bufSize != 2) return _T("Proxy not responding properly");
	}

	// Success!

	LOG("Connected to destination through proxy");
	return _T("");
}

// ---------------------------------------------------------------------------------------------------------------------------------
// SEND:
// 
//    CONNECT www.3dmaileffects.com:80 HTTP/1.1
//    User-Agent: Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)
//    Host: www.3dmaileffects.com
//    Content-Length: 0
//    Pragma: no-cache
// 
// RECV:
// 
//    HTTP/1.0 200 Connection established
//    Proxy-agent: WinProxy-Ver4.0 R1f
//    Connection: close
// 
// Connection exists
// ---------------------------------------------------------------------------------------------------------------------------------

wstring	Socket::connectCern(const string & address, const unsigned int port)
{
	LOGFUNC("Socket::connectSocks4x");

	if (!created()) return wstring(_T("connectCern failed (no socket)"));

	// Always need to do DNS lookup on the proxy server, if it's not already an IP address

	string	proxyIP;
	LOG("Looking up the proxy server...");
	ERROR_STRING_WRAPPER(lookup(wtoa(theApp.prefs().proxy()[_T("proxyHost")].stringValue()), proxyIP));

	// Connect to the proxy server

	LOG("Connecting to proxy server..." + wtoa(theApp.prefs().proxy()[_T("proxyHost")].stringValue()) + ":" + integerToString(theApp.prefs().proxy()[_T("proxyPort")].integerValue()));
	if (!winSocket().Connect(atow(proxyIP).c_str(), theApp.prefs().proxy()[_T("proxyPort")].integerValue()))
	{
		return wstring(_T("Unable to connect to proxy ")) + theApp.prefs().proxy()[_T("proxyHost")].stringValue() + _T(":") + integerToWString(theApp.prefs().proxy()[_T("proxyPort")].integerValue()) + _T("\n") + lastError();
	}

	// Are we responsible for our own DNS?

	string	destAddr = address;
	if (!theApp.prefs().proxy()[_T("proxyResolve")].booleanValue())
	{
		LOG("Performing my own DNS lookup for: " + address);
		ERROR_STRING_WRAPPER(lookup(address, destAddr));
	}

	// Build the HTTP requst header

	string	requestHeader;
	requestHeader += "CONNECT " + destAddr + ":" + integerToString(port) + " HTTP/1.1\r\n";
	requestHeader += "User-Agent: 3D Mail Effects Application\r\n";
	requestHeader += "Host: " + destAddr + "\r\n";
	requestHeader += "Content-Length: 0\r\n";
	requestHeader += "Pragma: no-cache\r\n";

	if (theApp.prefs().proxy()[_T("proxyAuth")].booleanValue())
	{
		LOG("Using authentication...");

		// Encode the data

		string	clearText = wtoa(theApp.prefs().proxy()[_T("proxyUser")].stringValue()) + ":" + wtoa(theApp.prefs().proxy()[_T("proxyPass")].stringValue());
		string	authString = encodeBase64((const unsigned char *) clearText.c_str(), clearText.length());
		if (!authString.length()) return _T("Check your username/password: Unable to encrypt");

		// Add the enctrypted data to the request header

		requestHeader += "Authorization: Basic " + authString + "\r\n";
		requestHeader += "Proxy-authorization: Basic " + authString + "\r\n";
	}
	else
	{
		LOG("Not using authentication...");
	}

	requestHeader += "\r\n";

	// Send the buffer

	LOG("Sending request to proxy server:");
	RAW("----------------------------------------------------------------------------------------------------------------------------------");
	RAW(requestHeader);
	RAW("----------------------------------------------------------------------------------------------------------------------------------");

	ERROR_STRING_WRAPPER(send(requestHeader));

	// Get the response
	//
	//    HTTP/1.0 200 Connection established
	//    Proxy-agent: WinProxy-Ver4.0 R1f
	//    Connection: close

	string		responseHeader;
	StringList	terminatorList;
	terminatorList.push_back("\r\n\r\n");
	unsigned int	bytesReceived;
	LOG("Receiving response...");
	ERROR_STRING_WRAPPER(recvUntil(responseHeader, bytesReceived, terminatorList, 10000));

	// Find the response code
	//
	// Strip off everything before the response code

	string::size_type idx = responseHeader.find(" ");
	if (idx == string::npos) return _T("Proxy server not responding properly");
	responseHeader.erase(0, idx+1);

	// Strip off everything after the EOL

	idx = responseHeader.find("\r\n");
	if (idx == string::npos) return _T("Proxy server not responding properly");
	responseHeader.erase(idx);

	// Strip off everything after the response code (if anything)

	idx = responseHeader.find(" ");
	if (idx != string::npos) responseHeader.erase(idx);

	// What is that response code??

	unsigned int	responseCode = atoi(responseHeader.c_str());
	LOG("Received response code: " + integerToString(responseCode));

	if (responseCode != 200)
	{
		return http::statusCodeString(responseCode);
	}

	// Success!

	LOG("Connected to destination through proxy");
	return _T("");
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	Socket::disconnect()
{
	LOGBLOCK("Socket::disconnect()");

	// Only disconnect if we're connected

	if (created() && connected())
	{
		winSocket().Close();
	}

	connected() = false;

	// Done

	return _T("");
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	Socket::setBlocking(const bool blocking)
{
	// ALWAYS make sure the socket was created

	if (!created()) return wstring(_T("setBlocking failed (no socket)"));

	// Set the requested blocking mode

	DWORD	mode = blocking ? 0:1;
	if (!winSocket().IOCtl(FIONBIO, &mode)) return wstring(_T("Unable to set socket blocking: \n")) + lastError();

	// Done

	return _T("");
}

// --------------------------------------------------------------------------------------------------------------------------------

wstring	Socket::send(const string & str)
{
	if (!created()) return wstring(_T("send failed (no socket)"));

	return send(str.c_str(), str.length());
}

// --------------------------------------------------------------------------------------------------------------------------------

wstring	Socket::send(const char * buf, const unsigned int length)
{
	LOGBLOCK("Socket::send([binary])");
	{
		HEX(buf, length);
	}

	// ALWAYS make sure the socket was created

	if (!created()) return wstring(_T("send failed (no socket)"));

	// Send the data

	if (buf)
	{
		if (winSocket().Send(buf, length) == SOCKET_ERROR)
		{
			return wstring(_T("Unable to sednd data over socket: \n")) + lastError();
		}
	}

	// All's well!

	return _T("");
}

// --------------------------------------------------------------------------------------------------------------------------------

wstring	Socket::recvUntil(string & str, unsigned int & bytesReceived, const StringList & terminatorList, const unsigned int maxBytesToReceive)
{
	LOGBLOCK("Socket::recvUntil([string], maxBytesToReceive = " + integerToString(maxBytesToReceive) + ")");

	if (!created()) return wstring(_T("recvUntil failed (no socket)"));

	// Start fresh

	bytesReceived = 0;

	// Build a list of the last characters in each termination string
	string	lastChars;
	{
		LOGBLOCK("TerminationList");

		for (StringList::const_iterator i = terminatorList.begin(); i != terminatorList.end(); ++i)
		{
			if (i->length())
			{
				HEX(i->c_str(), i->length());
				lastChars += (*i)[i->length() - 1];
			}
			else
			{
				LOG("[Empty terminator]");
				lastChars += (char) 255;
			}
		}
	}

	// Our receive buffer

	char *		recvBuffer = NULL;
	unsigned int	recvBufferAllocated = 0;
const	unsigned int	recvBufferStep = 64 * 1024;

	// Read the data in

	wstring	errorStr;
	for(;;)
	{
		// Prime the pump... (and delay dealing with errors until the end if the loop)

		errorStr = primeBuffer();

		// Nothing to read?

		if (!readBufferAvail()) break;

		// Scan through the read buffer for a termination end-char

		int	firstOccurrence = -1;
		for (unsigned int i = 0; i < lastChars.length(); ++i)
		{
			char *	src = readBuffer() + readBufferRead();
			char *	ptr = (char *) memchr(src, lastChars[i], readBufferAvail());
			if (!ptr) continue;

			int	dist = ptr - src + 1;
			if (firstOccurrence < 0 || dist < firstOccurrence) firstOccurrence = dist;
		}

		// If we didn't find an termination point, just include the whole buffer

		if (firstOccurrence == -1) firstOccurrence = readBufferAvail();

		// Grow the receive buffer?

		if (recvBufferAllocated <= bytesReceived + firstOccurrence + 1)
		{
			unsigned int	newSize = recvBufferAllocated + recvBufferStep;
			char *		temp = (char *) realloc(recvBuffer, newSize);
			if (!temp)
			{
				free(recvBuffer);
				recvBuffer = NULL;
				errorStr = _T("Unable to allocate receive buffer memory");
				break;
			}
			recvBuffer = temp;
			recvBufferAllocated = newSize;
		}

		// Copy the data over...

		memcpy(&recvBuffer[bytesReceived], readBuffer() + readBufferRead(), firstOccurrence);
		readBufferRead() += firstOccurrence;
		bytesReceived += firstOccurrence;

		// Terminate our recvBuffer

		recvBuffer[bytesReceived] = 0;

		// Check against each terminator

		bool	done = false;
		for (StringList::const_iterator i = terminatorList.begin(); i != terminatorList.end() && !done; ++i)
		{
			// If the input isn't long enough to contain the terminator, skip it

			if (bytesReceived < i->length()) continue;

			// If the final chars don't match, skip it

			if (recvBuffer[bytesReceived - 1] != (*i)[i->length() - 1]) continue;

			// Get the last few chars of the input for comparison against the terminator

			string	endOfInput = &recvBuffer[bytesReceived - i->length()];

			// Match?

			if (areEqualNocase(*i, endOfInput)) done = true;
		}

		// Was there an error

		if (errorStr.length()) break;

		if (done) break;
	}

	// Handle the excess in our receive buffer

	if (recvBuffer)
	{
		str = (char *) recvBuffer;
		free(recvBuffer);
	}

	HEX(str.c_str(), str.length());
	return errorStr;
}

// --------------------------------------------------------------------------------------------------------------------------------

wstring	Socket::recv(string & str, unsigned int & bytesReceived, const unsigned int length)
{
	LOGBLOCK("Socket::recv([string], length = " + integerToString(length) + ")");

	if (!created()) return wstring(_T("recv failed (no socket)"));

	// Start fresh

	bytesReceived = 0;

	// Read the data in chunks

	for(;;)
	{
		// Our temporary receive buffer

		char	buf[READ_BUF_SIZE + 1];

		// How much to receive?

		unsigned int	maxThisTime = READ_BUF_SIZE - 1;
		if (maxThisTime > length - bytesReceived)
		{
			maxThisTime = length - bytesReceived;
		}

		// If we've got no more data to read, bail

		if (!maxThisTime) break;

		unsigned int	bytesThisTime;
		wstring	result = recv(buf, bytesThisTime, maxThisTime);

		// If we got anything, deal with it

		if (bytesThisTime)
		{
			buf[bytesThisTime] = 0;
			str += buf;
			bytesReceived += bytesThisTime;
		}

		// If we didn't get it all, then there's no more to get

		if (bytesThisTime != maxThisTime) break;

		// Was there an error

		if (result.length()) return result;
	}

	return _T("");
}

// --------------------------------------------------------------------------------------------------------------------------------

wstring	Socket::recv(char * buf, unsigned int & bytesReceived, const unsigned int length)
{
	if (!created()) return wstring(_T("recv failed (no socket)"));

	// Working copy

	int	len = length;
	bytesReceived = 0;

	while(len)
	{
		// How many bytes to copy from the readBuffer() into the destination 'buf'?

		unsigned int	cpCount = len;
		if (cpCount > readBufferAvail()) cpCount = readBufferAvail();

		// Copy as much as we can from the buffer

		memcpy(buf + bytesReceived, &readBuffer()[readBufferRead()], cpCount);

		// Keep track

		readBufferRead() += cpCount;
		len -= cpCount;
		bytesReceived += cpCount;

		// Prime the buffer for another pass...

		if (len)
		{
			wstring	err = primeBuffer();
			if (err.length()) return err;
		}

		// If the buffer primes with no data, we're out of data to read

		if (!readBufferAvail()) break;
	}

	return _T("");
}

// --------------------------------------------------------------------------------------------------------------------------------

wstring	Socket::primeBuffer()
{
	if (!created()) return wstring(_T("primeBuffer failed (no socket)"));

	// Only prime the pump when it's empty

	if (readBufferAvail()) return _T("");

	// Reset our counters

	readBufferOccupied() = 0;
	readBufferRead() = 0;

	// Setup our timeout

	unsigned int	endTime = time(NULL) + timeout();
	if (!timeout()) endTime = 0xffffffff;

	// Only prime the pump when it's empty

	while ((unsigned int) time(NULL) < endTime && !readBufferAvail())
	{
		// Read some data

		int	ret = winSocket().Receive(readBuffer(), READ_BUF_SIZE);

		// Error?

		if (ret == SOCKET_ERROR)
		{
			int	errCode = winSocket().GetLastError();

			// We don't mind blocking errors.. we'll wait...

			if (errCode != WSAEWOULDBLOCK)
			{
				return wstring(_T("Unable to read data from socket: \n")) + lastError();
			}

			// It's blocking, wait a bit...

			allowBackgroundProcessing();
		}

		// Did we receive something

		else if (ret)
		{
			// If we're using a timeout, reset it

			if (timeout()) endTime = time(NULL) + timeout();

			// Log the data

			if (ret > 1)
			{
				LOGBLOCK("Bytes received: " + integerToString(ret));
				HEX((const char *) readBuffer(), ret);
			}

			// Track the bytes received

			readBufferOccupied() = ret;
		}

		// No error, but no data... bail

		else
		{
			break;
		}
	}

	// Timed out?

	if ((unsigned int) time(NULL) > endTime && !readBufferAvail())
	{
		WSASetLastError(WSAETIMEDOUT);
	}

	return _T("");
}

// --------------------------------------------------------------------------------------------------------------------------------

wstring	Socket::lastError()
{
	int	wec = WSAGetLastError();
	int	lec = GetLastError();
	int	sec, ecl = 4;
	winSocket().GetSockOpt(SO_ERROR, &sec, &ecl);

	{
		wstring	es = errorString(wec);
		LOG("WSAGetLastError returned [" + integerToString(wec) + "]: " + wtoa(es), Logger::LOG_CRIT);
	}

	{
		wstring	es = errorString(lec);
		LOG("GetLastError returned [" + integerToString(lec) + "]: " + wtoa(es), Logger::LOG_CRIT);
	}

	{
		wstring	es = errorString(sec);
		LOG("GetSockOpt returned [" + integerToString(sec) + "]: " + wtoa(es), Logger::LOG_CRIT);
		return es;
	}
}

// --------------------------------------------------------------------------------------------------------------------------------

wstring	Socket::errorString(const int err)
{
	switch(err)
	{
		case WSABASEERR            : return _T("WSABASEERR");
		case WSAEINTR              : return _T("WSAEINTR");
		case WSAEBADF              : return _T("Bad file descriptor");
		case WSAEACCES             : return _T("WSAEACCES");
		case WSAEFAULT             : return _T("WSAEFAULT");
		case WSAEINVAL             : return _T("WSAEINVAL");
		case WSAEMFILE             : return _T("WSAEMFILE");
		case WSAEWOULDBLOCK        : return _T("WSAEWOULDBLOCK");
		case WSAEINPROGRESS        : return _T("WSAEINPROGRESS");
		case WSAEALREADY           : return _T("WSAEALREADY");
		case WSAENOTSOCK           : return _T("Invalid socket");
		case WSAEDESTADDRREQ       : return _T("WSAEDESTADDRREQ");
		case WSAEMSGSIZE           : return _T("WSAEMSGSIZE");
		case WSAEPROTOTYPE         : return _T("WSAEPROTOTYPE");
		case WSAENOPROTOOPT        : return _T("WSAENOPROTOOPT");
		case WSAEPROTONOSUPPORT    : return _T("Protocol not supported");
		case WSAESOCKTNOSUPPORT    : return _T("WSAESOCKTNOSUPPORT");
		case WSAEOPNOTSUPP         : return _T("WSAEOPNOTSUPP");
		case WSAEPFNOSUPPORT       : return _T("WSAEPFNOSUPPORT");
		case WSAEAFNOSUPPORT       : return _T("WSAEAFNOSUPPORT");
		case WSAEADDRINUSE         : return _T("Address is already in use");
		case WSAEADDRNOTAVAIL      : return _T("Address not available");
		case WSAENETDOWN           : return _T("Network is down");
		case WSAENETUNREACH        : return _T("Network is unreachable");
		case WSAENETRESET          : return _T("WSAENETRESET");
		case WSAECONNABORTED       : return _T("Connection aborted");
		case WSAECONNRESET         : return _T("WSAECONNRESET");
		case WSAENOBUFS            : return _T("No buffers available");
		case WSAEISCONN            : return _T("WSAEISCONN");
		case WSAENOTCONN           : return _T("Not connected");
		case WSAESHUTDOWN          : return _T("WSAESHUTDOWN");
		case WSAETOOMANYREFS       : return _T("WSAETOOMANYREFS");
		case WSAETIMEDOUT          : return _T("Timeout");
		case WSAECONNREFUSED       : return _T("Connection refused, check settings");
		case WSAELOOP              : return _T("WSAELOOP");
		case WSAENAMETOOLONG       : return _T("Name too long");
		case WSAEHOSTDOWN          : return _T("Host down");
		case WSAEHOSTUNREACH       : return _T("Host unreachable");
		case WSAENOTEMPTY          : return _T("WSAENOTEMPTY");
		case WSAEPROCLIM           : return _T("WSAEPROCLIM");
		case WSAEUSERS             : return _T("WSAEUSERS");
		case WSAEDQUOT             : return _T("WSAEDQUOT");
		case WSAESTALE             : return _T("WSAESTALE");
		case WSAEREMOTE            : return _T("WSAEREMOTE");
		case WSASYSNOTREADY        : return _T("WSASYSNOTREADY");
		case WSAVERNOTSUPPORTED    : return _T("WSAVERNOTSUPPORTED");
		case WSANOTINITIALISED     : return _T("WSANOTINITIALISED");
		case WSAEDISCON            : return _T("WSAEDISCON");
		case WSAHOST_NOT_FOUND     : return _T("Host not found");
		default                    : return _T("");
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------
// Socket.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
