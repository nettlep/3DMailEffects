// ---------------------------------------------------------------------------------------------------------------------------------
//  _     _   _                              
// | |   | | | |                             
// | |__ | |_| |_ _ __       ___ _ __  _ __  
// | '_ \| __| __| '_ \     / __| '_ \| '_ \ 
// | | | | |_| |_| |_) | _ | (__| |_) | |_) |
// |_| |_|\__|\__| .__/ (_) \___| .__/| .__/ 
//               | |            | |   | |    
//               |_|            |_|   |_|    
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

#include "stdafx.h"
#include "3Dme.h"
#include "http.h"
#include <curl\cURL.h>

// ---------------------------------------------------------------------------------------------------------------------------------

struct MemoryStruct
{
	char *memory;
	size_t size;
};

size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
	register int realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)data;
	  
	mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory)
	{
		memcpy(&(mem->memory[mem->size]), ptr, realsize);
		mem->size += realsize;
		mem->memory[mem->size] = 0;
	}
	return realsize;
}

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

#define	HTTPTHROW(a)	{static wstring b; b = (a);LOGBLOCK("An exception has occurred"); LOG(wtoa(b), Logger::LOG_CRIT); throw b;}
#define	WRAP(a)		{static wstring b; b = (a);if (b.length()) throw b;}

// ---------------------------------------------------------------------------------------------------------------------------------

	http::http() : _socket(static_cast<Socket *>(0)), _currentPort(80), _statusCode(99), _contentLength(0), _timeout(0)
{
	defaultUserAgent() = get3DMEBrowserString();
}

// ---------------------------------------------------------------------------------------------------------------------------------

	http::~http()
{
	disconnect();
}

// ---------------------------------------------------------------------------------------------------------------------------------
// Test cases:
//
//	http://www.fluidstudios.com
//	http://www.fluidstudios.com/
//	http://www.fluidstudios.com/dir
//	http://www.fluidstudios.com/dir/
//	http://www.fluidstudios.com/dir/file.html
//
//	http://www.fluidstudios.com:80
//	http://www.fluidstudios.com:80/
//	http://www.fluidstudios.com:80/dir
//	http://www.fluidstudios.com:80/dir/
//	http://www.fluidstudios.com:80/dir/file.html
//
//	www.fluidstudios.com
//	www.fluidstudios.com/
//	www.fluidstudios.com/dir
//	www.fluidstudios.com/dir/
//	www.fluidstudios.com/dir/file.html
//
//	www.fluidstudios.com:80
//	www.fluidstudios.com:80/
//	www.fluidstudios.com:80/dir
//	www.fluidstudios.com:80/dir/
//	www.fluidstudios.com:80/dir/file.html
//
//	/
//	/dir
//	/dir/
//	/dir/file.html
//
//	foo.html
//
// ---------------------------------------------------------------------------------------------------------------------------------

void	http::parseURL(const string & url, string & server, unsigned int & port, string & request, bool & ssl)
{
	// Default to standard HTTP (not HTTPS)

	ssl = false;

	// A clean working copy is always nice...

	string	workingCopy = url;
	stripLeadingWS(workingCopy);
	stripTrailingWS(workingCopy);

	// Default port & request

	port = currentPort();
	request = "/";
	server = currentServer();

	// Do we have a URL?

	if (!workingCopy.length()) return;

	// Check for "http://" or "https://"

	{
		string::size_type	idx = workingCopy.find("/");
		if (idx != string::npos && idx > 2 && workingCopy[idx-1] == ':' && workingCopy[idx+1] == '/')
		{
			// SSL?

			if (workingCopy[idx-2] == 's' || workingCopy[idx-2] == 'S') ssl = true;

			// Strip the HTTP://

			workingCopy.erase(0, idx+2);

			// Server gets everything up to the first / or the end

			string::size_type	slash = workingCopy.find("/");
			if (slash != string::npos)
			{
				server = workingCopy.substr(0, slash);
				workingCopy.erase(0, slash);
			}
			else
			{
				server = workingCopy;
				workingCopy.erase();
			}

			// If there is a colon inside the server, then it's a port

			wstring::size_type	colon = server.find(":");
			if (colon != string::npos)
			{
				// We have a port, and it's between the ':' and the end of the string

				string	portString = server.substr(colon + 1);
				server.erase(colon);
				port = atoi(portString.c_str());
			}
		}
	}

	// Finally, the remainder is the request

	if (workingCopy.length()) request = workingCopy;

	// Make sure we have a leading slash on it

	if (!request.length() || request[0] != '/')
	{
		request = "/" + request;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------
// Parse the response header, and remove the header from the input string (including the header's terminating CRLF)
// ---------------------------------------------------------------------------------------------------------------------------------

bool	http::parseResponseHeader(string & responseHeader)
{
	// Defaults

	statusCode() = 99; // invalid response header from host, as defined by me, not HTTP specs
	contentLength() = 0xffffffff;

	// Find the end of the header

	string::size_type	headerEnd = responseHeader.find("\r\n\r\n");
	if (headerEnd == string::npos) return false;

	// Remove the header fron the input and copy it to a local

	string	header = responseHeader.substr(0, headerEnd + 4);
	responseHeader.erase(0, headerEnd + 4);

	// Parse the status code
	//
	// HTTP/x.x ??? ...
	{
		string::size_type	idx = header.find(" ");
		if (idx == string::npos) return false;
		header.erase(0, idx+1);

		idx = header.find(" ");
		if (idx == string::npos) return false;
		string	codeString = header.substr(0, idx);
		header.erase(0, idx+1);

		stripLeadingWS(codeString);
		stripTrailingWS(codeString);

		statusCode() = atoi(codeString.c_str());
	}

	// Is there a content length?

	string::size_type	idx = findNocase(header, "\r\ncontent-length:");
	if (idx != string::npos)
	{
		// Skip to the end of the prefix

		idx += 17;
		if (idx > header.length()) return false;

		// Find the end of this line in the header

		string::size_type	end = header.find("\r\n", idx);
		if (end == string::npos) return false;

		// Get the number

		string	contentLengthString = header.substr(idx, end - idx);

		stripLeadingWS(contentLengthString);
		stripTrailingWS(contentLengthString);

		contentLength() = atoi(contentLengthString.c_str());
	}

	// Is there a redirection location?

	idx = findNocase(header, "\r\nlocation:");
	if (idx != string::npos)
	{
		// Skip to the end of the prefix

		idx += 11;
		if (idx > header.length()) return false;

		// Find the end of this line in the header

		string::size_type	end = header.find("\r\n", idx);
		if (end == string::npos) return false;

		// Get the location

		location() = header.substr(idx, end - idx);
		stripLeadingWS(location());
		stripTrailingWS(location());
	}

	// Parse the cookies

	for(;;)
	{
		idx = findNocase(header, "\r\nset-cookie:");
		if (idx == string::npos) break;

		// Erase the prefix

		header.erase(idx, 13);

		// Find the end

		string::size_type	end = header.find("\r\n", idx);
		if (end == string::npos) return false;

		// Get the cookie

		string	thisCookie = header.substr(idx, end - idx);

		// Strip out the rest of the cookie from the header

		header.erase(idx, end-idx);

		addCookie(thisCookie);
	}

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	http::addCookie(const string & cookie)
{
	// Working copy

	string	thisCookie = cookie;

	// Strip out everything past the semicolon and get just the raw cookie data

	stripLeadingWS(thisCookie);
	stripTrailingWS(thisCookie);

	// The name of this cookie

	string	name = thisCookie;
	{
		string::size_type	equal = name.find("=");
		if (equal != string::npos) name.erase(equal);
	}

	// Look for the same cookie

	bool	found = false;
	for (StringList::iterator i = cookies().begin(); i != cookies().end(); ++i)
	{
		string	curCookie = *i;
		string::size_type	equal = curCookie.find("=");
		if (equal == string::npos) continue;

		// If the cookies are the same, replace

		if (areEqualNocase(name, curCookie.substr(0, equal)))
		{
			*i = thisCookie;
			found = true;
			break;
		}
	}

	// If we didn't find the cookie in the list, add it

	if (!found)
	{
		cookies().push_back(thisCookie);
		LOG("Storing cookie: " + thisCookie);
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	http::parseCookie(const string & cookie, string & name, string & value, string & path, string & domain) const
{
	// Working copy...

	string	thisCookie = cookie;

	// CLEAR! [spoken in ER tones...]

	name.erase();
	value.erase();
	path.erase();
	domain.erase();

	while(thisCookie.length())
	{
		// Make sure we got clean data to work with

		stripLeadingWS(thisCookie);
		stripTrailingWS(thisCookie);

		// Get a piece

		string	data;

		string::size_type	semicolon = thisCookie.find(";");
		if (semicolon == string::npos)
		{
			data = thisCookie;
			thisCookie.erase();
		}
		else
		{
			data = thisCookie.substr(0, semicolon);
			thisCookie.erase(0, semicolon + 1);
		}

		stripTrailingWS(data);

		// What kind of data is this?

		if (areEqualNocase(data, "expires=", 8))
		{
			// Skip these...
		}
		else if (areEqualNocase(data, "path=", 5))
		{
			path = data.substr(5);
		}
		else if (areEqualNocase(data, "domain=", 7))
		{
			domain = data.substr(7);
		}
		else // Must be name/value
		{
			string::size_type	equal = data.find("=");
			if (equal != string::npos)
			{
				name = data.substr(0, equal);
				value = data.substr(equal+1);
			}
		}
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	http::connect(const string & url)
{
	LOG("Connecting to server: " + url);

	// First, disconnect

	disconnect();

	// Create a new socket

	_socket = new Socket();

	socket().timeout() = timeout();

	// Disable the socket proxy management if we'll be doing it ourselves...

	if (theApp.prefs().proxy()[_T("httpProxy")].booleanValue())
	{
		socket().disableProxy() = true;
	}

	// Parse the URL

	string		server, request;
	unsigned int	port;
	bool		ssl;
	parseURL(url, server, port, request, ssl);

	// Connect

	if (theApp.prefs().proxy()[_T("httpProxy")].booleanValue())
	{
		wstring	err = socket().connect(wtoa(theApp.prefs().proxy()[_T("httpProxyHost")].stringValue()), theApp.prefs().proxy()[_T("httpProxyPort")].integerValue());
		if (err.length()) HTTPTHROW(err);
	}
	else
	{
		wstring	err = socket().connect(server, port);
		if (err.length()) HTTPTHROW(err);
	}

	if (!socket().created()) HTTPTHROW(_T("Cannot connect to HTTP server due to previous socket error"));

	// Successful connection, keep track of where we are

	currentServer() = server;
	currentPort() = port;
	currentRequest() = request;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	http::disconnect()
{
	LOG("Disconnecting");

	if (_socket)
	{
		delete _socket;
		_socket = NULL;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

string	http::buildRequestHeader(const string & userAgent, const string & postData) const
{
	// Build the request

	string	requestHeader;

	// Post or get?

	string	command;
	if (postData.length())	command = "POST";
	else			command = "GET";

	if (theApp.prefs().proxy()[_T("httpProxy")].booleanValue())
	{
		requestHeader += command + " HTTP://" + currentServer() + ":" + integerToString(currentPort()) + currentRequest() + " HTTP/1.1\r\n";
	}
	else
	{
		requestHeader += command + " " + currentRequest() + " HTTP/1.1\r\n";
	}

	requestHeader += "Accept: */*\r\n";
	requestHeader += "Cache-Control: no-cache\r\n";
	requestHeader += "Accept-Language: en-us\r\n";
	requestHeader += "Referer: 3D Mail Effects Application\r\n";

	// Post?

	if (postData.length())
	{
		char	clen[90];
		sprintf(clen, "%d", postData.length());
		requestHeader += "Content-Type: application/x-www-form-urlencoded\r\n";
		requestHeader += "Content-Length: " + string(clen) + "\r\n";
	}

	// User-Agent

	if (userAgent.length())
	{
		requestHeader += "User-Agent: " + userAgent + "\r\n";
	}
	else
	{
		requestHeader += "User-Agent: " + defaultUserAgent() + "\r\n";
	}

	if (referrer().length())
	{
		requestHeader += "Referrer: " + referrer() + "\r\n";
	}

	requestHeader += "Host: " + currentServer() + "\r\n";
	requestHeader += "Connection: Keep-Alive\r\n";

	// Cookies

	string	cookieList = getCookieString();

	// If there were any cookies, add them to the request header

	if (cookieList.length())
	{
		// Strip the last semicolon off the cookie list

		cookieList.erase(cookieList.length() - 2);
		requestHeader += "Cookie: " + cookieList + "\r\n";
	}

	// Terminate the header

	requestHeader += "\r\n";

	// Add the post data (if any)

	if (postData.length()) requestHeader += postData;

	LOG("Sending request:");
	RAW("----------------------------------------------------------------------------------------------------------------------------------");
	RAW(requestHeader);
	RAW("----------------------------------------------------------------------------------------------------------------------------------");

	return requestHeader;
}

// ---------------------------------------------------------------------------------------------------------------------------------

string	http::getCookieString() const
{
	string	cookieList;
	for (StringList::const_iterator i = cookies().begin(); i != cookies().end(); ++i)
	{
		// Parse the cookie

		string	name, value, path, domain;
		parseCookie(*i, name, value, path, domain);

		// Skip empty cookies

		if (!value.length()) continue;

		// Does this cookie apply to the domain?

		if (domain.length())
		{
			// Is the domain found in the string?

			string::size_type	idx = findNocase(currentServer(), domain);

			// If the domain isn't in the server, bail

			if (idx == string::npos) continue;

			// Make sure the domain is the last few chars of the server name

			if (domain.length() != currentServer().length() - idx) continue;
		}

		// Does this cookie apply to the path?

		if (path.length())
		{
			// Is the path found in the request?

			string::size_type	idx = findNocase(currentRequest(), path);

			// If the path isn't in the request, bail

			if (idx == string::npos) continue;

			// Make sure the path is the first few chars of the request

			if (idx != 0) continue;
		}

		// Add the cookie to the header

		cookieList += name + "=" + value + "; ";
	}

	return cookieList;
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	http::sendRequest(const string & url, const string & userAgent, const string & postData, const unsigned int depth)
{
	try
	{
		// Make the connection

		connect(url);

		// Build the request header

		string	requestHeader = buildRequestHeader(userAgent, postData);

		// Send the request

		WRAP(socket().send(requestHeader));

		// Get the response (skipping repeated continues)

		for(;;)
		{
			string		responseHeader;
			StringList	terminatorList;
			terminatorList.push_back("\r\n\r\n");
			unsigned int	bytesReceived;
			WRAP(socket().recvUntil(responseHeader, bytesReceived, terminatorList, 10000));

			LOG("Received response header:");
			RAW("----------------------------------------------------------------------------------------------------------------------------------");
			RAW(responseHeader);
			RAW("----------------------------------------------------------------------------------------------------------------------------------");

			// Parse the response header

			string	header = responseHeader;
			if (!parseResponseHeader(responseHeader)) HTTPTHROW(_T("Unable to receive entire response from host"));

			// Handle REFRESH: header entry

			string::size_type	idx = findNocase(header, "Refresh:", 0);
			if (idx != string::npos)
			{
				string	refreshURL = header;
				refreshURL.erase(0, idx);
				idx = findNocase(refreshURL, "url=", 0);
				if (idx != string::npos)
				{
					refreshURL.erase(0, idx+4);
					idx = refreshURL.find("\n");
					if (idx != string::npos) refreshURL.erase(idx);
					stripLeadingWS(refreshURL);
					stripTrailingWS(refreshURL);

					LOG("Received REFRESH: header from server -- refreshing to: " + refreshURL);

					return sendRequest(refreshURL, userAgent, "", depth+1);
				}
			}

			// Continue? Just get another header...

			if (statusCode() == 100)
			{
				LOG("Received code (" + integerToString(statusCode()) + ") from server -- continuing");

				continue;
			}

			// If it's a redirection, jump there now

			if (statusCode() == 300 || statusCode() == 301 || statusCode() == 302 || statusCode() == 307)
			{
				LOG("Received code (" + integerToString(statusCode()) + ") from server -- redirecting");

				if (depth > 20) throw_wstring (_T("Redirection recursion too deep"))

				if (!location().length()) throw_wstring (_T("Redirection to an unknown location"))
				string	newURL = location();
				return sendRequest(newURL, userAgent, "", depth + 1);
			}

			// We'll disallow everything except a specific "OK"

			if (statusCode() != 200)
			{
				LOG("Received code (" + integerToString(statusCode()) + ") from server -- OK");

				throw_wstring (statusCodeString(statusCode()))
			}

			return _T("");
		}
	}
	catch(const wstring & err)
	{
		return err;
	}

	return _T("");
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	http::sendRequestAndGetPage(const string & url, string & pageData, const string & userAgent, const string & postData, const unsigned int depth)
{
	try
	{
		struct MemoryStruct chunk;
		memset(&chunk, 0, sizeof(chunk));

		// Parse the URL (required to maintain a chain of links from site to site)

		string		server, request;
		unsigned int	port;
		bool		ssl;
		parseURL(url, server, port, request, ssl);

		// Update these...

		currentServer() = server;
		currentPort() = port;
		currentRequest() = request;

		// Init the curl session

		CURL *	handle = curl_easy_init();
		if (!handle) HTTPTHROW(_T("Unable to initialize cURL library"));

		// No progress meter, quiet

		curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1);
		curl_easy_setopt(handle, CURLOPT_MUTE, 1);

		// Specify URL to get

		string		newURL;
		if (ssl)
		{
			newURL = "https://" + server + request;
		}
		else
		{
			char	portString[90];
			sprintf(portString, "%d", port);
			newURL = "http://" + server + ":" + portString + request;
		}
		
		curl_easy_setopt(handle, CURLOPT_URL, newURL.c_str());

		// Send all data to this function

		curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(handle, CURLOPT_WRITEHEADER, (void *)&chunk);

		// User agent

		string	agent = userAgent;
		if (!agent.length()) agent = defaultUserAgent();

		curl_easy_setopt(handle, CURLOPT_USERAGENT, agent.c_str());

		// Proxy info

		if (theApp.prefs().proxy()[_T("httpProxy")].booleanValue())
		{
			string	host = wtoa(theApp.prefs().proxy()[_T("httpProxyHost")].stringValue());
			char	hostString[4096];
			strcpy(hostString, host.c_str());
			curl_easy_setopt(handle, CURLOPT_PROXY, hostString);
			curl_easy_setopt(handle, CURLOPT_PROXYPORT, theApp.prefs().proxy()[_T("httpProxyPort")].integerValue());
		}
		else if (theApp.prefs().proxy()[_T("proxyType")].stringValue() == _T("CERN/HTTP"))
		{
			string	host = wtoa(theApp.prefs().proxy()[_T("proxyHost")].stringValue());
			char	hostString[4096];
			strcpy(hostString, host.c_str());
			curl_easy_setopt(handle, CURLOPT_PROXY, hostString);
			curl_easy_setopt(handle, CURLOPT_PROXYPORT, theApp.prefs().proxy()[_T("proxyPort")].integerValue());
			if (theApp.prefs().proxy()[_T("proxyAuth")].booleanValue())
			{
				wstring	userpass = theApp.prefs().proxy()[_T("proxyUser")].stringValue() + wstring(_T(":")) + theApp.prefs().proxy()[_T("proxyPass")].stringValue();
				curl_easy_setopt(handle, CURLOPT_PROXYUSERPWD, wtoa(userpass).c_str());
			}
		}

		// Post data

		if (postData.length())
		{
			curl_easy_setopt(handle, CURLOPT_POSTFIELDS, postData.c_str());
		}

		// Cookie data

		string	cookieList = getCookieString();
		if (cookieList.length())
		{
			curl_easy_setopt(handle, CURLOPT_COOKIE, cookieList.c_str());
		}

		// Referrer

		if (referrer().length())
		{
			curl_easy_setopt(handle, CURLOPT_REFERER, referrer().c_str());
		}

		// We pass our 'chunk' struct to the callback function

		curl_easy_setopt(handle, CURLOPT_FILE, (void *)&chunk);

		// Get it!

		curl_easy_perform(handle);

		// Cleanup curl stuff

		curl_easy_cleanup(handle);

		if (!chunk.memory) HTTPTHROW(_T("Connection failure"));

		// Put our data into a single string for convenience

		for (unsigned int i = 0; i < chunk.size; ++i)
		{
			if (chunk.memory[i] == 0) chunk.memory[i] = ' ';
		}
		pageData = chunk.memory;
		free(chunk.memory);

		// Handle the response

		for(;;)
		{
			// Get the first group of headers

			string		responseHeader;
			string::size_type	idx = pageData.find("\r\n\r\n");
			if (idx == string::npos)
			{
				responseHeader = pageData;
				pageData = "";
			}
			else
			{
				responseHeader = pageData.substr(0, idx+4);
				pageData.erase(0, idx+4);
			}

			LOG("Received response header:");
			RAW("----------------------------------------------------------------------------------------------------------------------------------");
			RAW(responseHeader);
			RAW("----------------------------------------------------------------------------------------------------------------------------------");

			// Parse the response header

			string	header = responseHeader;
			if (!parseResponseHeader(responseHeader)) HTTPTHROW(_T("Unable to receive entire response from host"));

			// Handle REFRESH: header entry

			idx = findNocase(header, "Refresh:", 0);
			if (idx != string::npos)
			{
				string	refreshURL = header;
				refreshURL.erase(0, idx);
				idx = findNocase(refreshURL, "url=", 0);
				if (idx != string::npos)
				{
					refreshURL.erase(0, idx+4);
					idx = refreshURL.find("\n");
					if (idx != string::npos) refreshURL.erase(idx);
					stripLeadingWS(refreshURL);
					stripTrailingWS(refreshURL);

					LOG("Received REFRESH: header from server -- refreshing to: " + refreshURL);

					return sendRequestAndGetPage(refreshURL, pageData, userAgent, "", depth+1);
				}
			}

			// Continue? Just get another header...

			if (statusCode() == 100)
			{
				LOG("Received code (" + integerToString(statusCode()) + ") from server -- continuing");

				continue;
			}

			// If it's a redirection, jump there now

			if (statusCode() == 300 || statusCode() == 301 || statusCode() == 302 || statusCode() == 307)
			{
				LOG("Received code (" + integerToString(statusCode()) + ") from server -- redirecting");

				if (depth > 20) throw_wstring (_T("Redirection recursion too deep"))

				if (!location().length()) throw_wstring (_T("Redirection to an unknown location"))
				string	newURL = location();
				return sendRequestAndGetPage(newURL, pageData, userAgent, "", depth + 1);
			}

			// We'll disallow everything except a specific "OK"

			if (statusCode() != 200)
			{
				LOG("Received code (" + integerToString(statusCode()) + ") from server -- OK");

				throw_wstring (statusCodeString(statusCode()))
			}

			return _T("");
		}
	}
	catch(const wstring & err)
	{
		return err;
	}

	return _T("");
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	http::getPage(const string & url, string & pageData, const string & userAgent)
{
	LOGBLOCK("http::getPage(" + url + ")");
	return postPage(url, "", pageData, userAgent);
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	http::postPage(const string & url, const string & postData, string & pageData, const string & userAgent)
{
	LOGBLOCK("http::postPage(" + url + ")");
	if (postData.length())	LOG("Post data: " + postData);
	else			LOG("Post data is empty, will use GET instead");

	// Clear this

	pageData.erase();

	try
	{
		// Send the request

		WRAP(sendRequestAndGetPage(url, pageData, userAgent, postData));

		// Save the URL for the next connection's referrer

		referrer() = url;

		// Keep reading the page data

//		if (contentLength())
//		{
//			unsigned int	bytesReceived;
//			WRAP(socket().recv(pageData, bytesReceived, contentLength()));
//		}

		return _T("");
	}
	catch(wstring &err)
	{
		return err;
	}

	return _T("");
}

// ---------------------------------------------------------------------------------------------------------------------------------

string	http::get3DMEBrowserString()
{
	string	browser = "3DME " + wtoa(VERSION_STRING) + "-" + integerToString(getCurrentPatchLevel());

#if	defined(_REGISTERED_)
	char		temp[20];
	unsigned int	uid;
	IDKEY(uid);
	sprintf(temp, "%08X", uid);
	if ((keyfunc_0) != keyres__0)	browser += string(" (build: ") + temp + ")";
	else				browser += string(" [build: ") + temp + "]";
#endif // _REGISTERED_

	return browser;
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	http::statusCodeString(const unsigned int resp)
{
	switch(resp)
	{
		case  99: return _T("Invalid response header");
		case 100: return _T("Continue");
		case 101: return _T("Switching Protocols");
		case 200: return _T("OK");
		case 201: return _T("Created");
		case 202: return _T("Accepted");
		case 203: return _T("Non-Authoritative Information");
		case 204: return _T("The requested URL has no content");
		case 205: return _T("Reset Content");
		case 206: return _T("Partial Content");
		case 300: return _T("Multiple Choices");
		case 301: return _T("Moved Permanently");
		case 302: return _T("Found");
		case 303: return _T("See Other");
		case 304: return _T("Not Modified");
		case 305: return _T("Use Proxy");
		case 307: return _T("Temporary Redirect");
		case 400: return _T("The requested URL was a bad request");
		case 401: return _T("The requested URL was unauthorized");
		case 402: return _T("Payment Required");
		case 403: return _T("The requested URL was forbidden");
		case 404: return _T("The requested URL was not found");
		case 405: return _T("Method Not Allowed");
		case 406: return _T("Not Acceptable");
		case 407: return _T("Proxy Authentication Required");
		case 408: return _T("Request Time-out");
		case 409: return _T("Conflict");
		case 410: return _T("The requested URL was gone");
		case 411: return _T("Length Required");
		case 412: return _T("Precondition Failed");
		case 413: return _T("Request Entity Too Large");
		case 414: return _T("Request-URI Too Large");
		case 415: return _T("Unsupported Media Type");
		case 416: return _T("Requested range not satisfiable");
		case 417: return _T("Expectation Failed");
		case 425: return _T("Refused access");
		case 500: return _T("The requested URL was not found due to an internal server error");
		case 501: return _T("Not Implemented");
		case 502: return _T("Bad Gateway");
		case 503: return _T("Service Unavailable");
		case 504: return _T("Gateway Time-out");
		case 505: return _T("HTTP Version not supported");
		default: return _T("Unknown resposne: ") + integerToWString(resp);
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------
// http.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
