// ---------------------------------------------------------------------------------------------------------------------------------
//                                         
//                                         
//  _ __   ___  _ __       ___ _ __  _ __  
// | '_ \ / _ \| '_ \     / __| '_ \| '_ \ 
// | |_) | (_) | |_) | _ | (__| |_) | |_) |
// | .__/ \___/| .__/ (_) \___| .__/| .__/ 
// | |         | |            | |   | |    
// |_|         |_|            |_|   |_|    
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

#include "stdafx.h"
#include "3Dme.h"
#include "pop.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

#define	zzz() {wstring sss = _T(__FILE__) + wstring(_T("[")) + integerToWString(__LINE__) + wstring(_T("]")); LOG(wtoa(sss), Logger::LOG_CRIT);}

// ---------------------------------------------------------------------------------------------------------------------------------

#define	POPTHROW(a)	{static wstring b; b = (a) + socket().lastError();LOGBLOCK("An exception has occurred"); LOG(wtoa(b), Logger::LOG_CRIT); throw b;}

// ---------------------------------------------------------------------------------------------------------------------------------

	pop::pop()
	: _previewLines(10)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

	pop::~pop()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	pop::initiateSession(const string &address, const unsigned int port, const string &username, const string &password)
{
	if (!socket().created()) POPTHROW(_T("Cannot connect to POP server due to previous socket error"));

	// Connect

	wstring	err = socket().connect(address, port);
	if (err.length()) POPTHROW(err);

	// Sign on

	string	result;
	unsigned int	tbr;
	if (!sendCommand("", result, tbr))			POPTHROW(_T("Server is not responding properly to commands\n"));
	if (!sendCommand("user " + username, result, tbr))	POPTHROW(_T("Invalid username\n"));
	if (!sendCommand("pass " + password, result, tbr))	POPTHROW(_T("Invalid username or password\n"));
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	pop::queryMail(const bool checkOnly, const string &address, const unsigned int port, const string &username, const string &password, EmailList &oldMails, EmailList &emails)
{
	// Start fresh

	emails.clear();

	try
	{
		LOGBLOCK("Attempting to query for mail on server [" + address + "]");

		// Initiate the session

		initiateSession(address, port, username, password);

		// List the emails

		string		listing, uniqueListing;
		unsigned int	tbr;
		if (!sendCommand("list", listing, tbr, true))		POPTHROW(_T("Unable to send [list] command to server\n"));
		if (!sendCommand("uidl", uniqueListing, tbr, true))	POPTHROW(_T("Unable to send [uidl] command to server\n"));

		// Get the detail information on each email

		while (listing.length())
		{
			// Just to be safe...

			if (!listing.length() || !uniqueListing.length()) POPTHROW(_T("Server returned mismatched listing and unique listings"));

			// Get the index entry (includes length)

			string	lEntry;
			string::size_type	idx = listing.find("\r\n");
			if (idx != string::npos)
			{
				lEntry = listing.substr(0, idx);
				listing.erase(0, idx+2);
			}
			else
			{
				lEntry = listing;
				listing.erase();
			}

			stripLeadingWS(lEntry);
			stripTrailingWS(lEntry);
			idx = lEntry.find(" ");
			if (idx == string::npos) POPTHROW(_T("Invalid response from server from [list] command\n"));
			string			length = lEntry.substr(idx);
			stripLeadingWS(length);
			lEntry.erase(idx);

			// Get the unique index entry

			string	uEntry;
			idx = uniqueListing.find("\r\n");
			if (idx != string::npos)
			{
				uEntry = uniqueListing.substr(0, idx);
				uniqueListing.erase(0, idx+2);
			}
			else
			{
				uEntry = uniqueListing;
				uniqueListing.erase();
			}

			sEmail	email;
			bool	found = false;

			// I've seen an exception go through here once... and a user (on 98 has reported a lot of them.) After HOURS of trying to
			// figure out what it could be, I got nowhere. So, here is the hack-of-the-year's fix:

			try{

				// Size

				email.messageID = uEntry;
				idx = email.messageID.find(" ");
				if (idx != string::npos) email.messageID.erase(0, idx+1);

				// Look to see if this messageID already exists

				for (EmailList::iterator j = oldMails.begin(); j != oldMails.end(); ++j)
				{
					if (j->messageID == email.messageID)
					{
						found = true;
						j->newFlag = false;
						emails.push_back(*j);
						break;
					}
				}

			// Hack-of-the-year's fix (end)

			} catch(...) { }

			// Does this email already exist in the list?

			if (!found)
			{
				email.size = atoi(length.c_str());

				// We start a scope here so we can force a destruction of 'emailData' -- which might be rather large (the entire email)
				string	header, body;
				{
					string	emailData;
					tbr = 0;

					if (!checkOnly)
					{
						if (!sendCommand("top " + lEntry + " " + integerToString(previewLines()), emailData, tbr, true))
						{
							POPTHROW(_T("Unable to send [top] command to server\n"));
						}
					}
					else
					{
						time_t	t = time(NULL);
						string	dateString = string("date: ") + asctime(localtime(&t)) + "\n";
						dateString[dateString.length() - 2] = '\r';

						string::size_type idx = dateString.find(" ");
						idx = dateString.find(" ", idx+1);
						dateString = dateString.substr(0, idx) + string(",") + dateString.substr(idx);

						emailData.erase();
						emailData += dateString;
						emailData += "from: Not retrieved\r\n";
						emailData += "subject: Not retrieved\r\n";
						emailData += "message-id: " + email.messageID + "\r\n";
						emailData += "content-type: text/plain\r\n";
						emailData += "\r\n";
						emailData += "You have chosen not to retrieve the contents of this email. If you wish to be able to preview your emails, please check your configuration settings.\n\nThank you.\n";
					}

					// Split the emailData into header & body

					string::size_type	idx = emailData.find("\r\n\r\n");

					if (idx != string::npos)
					{
						header = emailData.substr(0, idx+2);
						body = emailData.substr(idx+2);
					}
					else
					{
						header = emailData;
					}
				}

				// Parse the header

				unsigned int	embeddedSkip;
				StringList	headerLines;
				convertToStringList(header, headerLines);
				email.mimeHeader.parse(headerLines, embeddedSkip);

				// Parse the parts of the email

				email.mimeHeader.parseParts(body);

				// Locate the blank line in the header
				// If it's valid, commit it to the list

				if (email.mimeHeader.validEmail() && email.messageID.length())
				{
					email.receivedSize = tbr;
					email.newFlag = true;
					email.pendingFlag = true;

					// If the user doesn't want notification of spam emails, don't mark it new

					if (email.mimeHeader.spam() && theApp.prefs().other()[_T("checkSpam")].booleanValue() && theApp.prefs().other()[_T("spamNoNew")].booleanValue())
						email.newFlag = false;

					// Add the email

					emails.push_front(email);
				}
			}
		}

		// Shutdown the connection

		string	result;
		if (!sendCommand("quit", result, tbr)) POPTHROW(_T("Unable to send [quit] command to server\n"));

		// We're done with this...

		socket().disconnect();
	}
	catch(const wstring & err)
	{
		return err;
	}

	return _T("");
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	pop::retrieveMail(const string &address, const unsigned int port, const string &username, const string &password, const string &messageID, sEmail &email)
{
#if 0
	try
	{
		LOGBLOCK("Attempting to retrieve message ID [" + messageID + "] on server [" + address + "]");

		// Initiate the session

		initiateSession(address, port, username, password);

		// List the emails

		string	_listing;
		unsigned int	tbr;
		if (!sendCommand("uidl", _listing, tbr, true)) POPTHROW(_T("Unable to send [uidl] command to server\n"));

		StringList	listing;
		convertToStringList(_listing, listing);

		// Get the detail information on each email

		int	found = -1;
		for (StringList::iterator i = listing.begin(); i != listing.end(); ++i)
		{
			// Get the index and size from this line in the list

			string			index = *i;
			stripLeadingWS(index);
			stripTrailingWS(index);
			string::size_type	idx = index.find(" ");
			if (idx == string::npos) POPTHROW(_T("Invalid response from server from [list] command\n"));
			string			thisID = index.substr(idx + 1);
			index.erase(idx);

			if (thisID == messageID)
			{
				found = atoi(index.c_str());
				break;
			}
		}

		if (found == -1) POPTHROW(_T("The message to be retrieved does not seem to exist on the server\n"));

		// Try to retrieve the message

		StringList	result;
		if (!sendCommand("retr " + integerToString(found), result, tbr)) POPTHROW(_T("Unable to send [retr] command to server\n"));

		// Parse the header

		unsigned int	embeddedSkip;
		email.mimeHeader.parse(result, embeddedSkip);

		// Pre-allocate storage for our buffer for faster copies

		unsigned int	totalLength = 0;
		for (StringList::iterator j = result.begin(); j != result.end(); ++j)
		{
			totalLength += j->length() + 2;
		}

		char *	buffer = new char [totalLength + 1];

		// Build the buffer

		unsigned int	index = 0;
		for (StringList::iterator j = result.begin(); j != result.end(); ++j)
		{
			memcpy(buffer + index, j->c_str(), j->length());
			index += j->length();

			memcpy(buffer + index, "\r\n", 2);
			index += 2;
		}
		buffer[index] = 0;

		// Parse the parts of the email

		email.mimeHeader.parseParts(buffer);
		delete[] buffer;

		// If it's valid, commit it to the list

		if (email.mimeHeader.validEmail() && email.messageID.length())
		{
			email.receivedSize = tbr;
			email.newFlag = true;
			email.pendingFlag = true;
		}

		// Shutdown the connection

		if (!sendCommand("quit", result, tbr)) POPTHROW(_T("Unable to send [quit] command to server\n"));

		// We're done with this...

		socket().disconnect();
	}
	catch(const wstring & err)
	{
		return err;
	}
#endif
	return _T("");
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	pop::deleteMail(const string &address, const unsigned int port, const string &username, const string &password, const string &messageID)
{
	try
	{
		LOGBLOCK("Attempting to delete message ID [" + messageID + "] on server [" + address + "]");

		// Initiate the session

		initiateSession(address, port, username, password);

		// List the emails

		string	_listing;
		unsigned int	tbr;
		if (!sendCommand("uidl", _listing, tbr, true)) POPTHROW(_T("Unable to send [uidl] command to server\n"));

		StringList	listing;
		convertToStringList(_listing, listing);

		// Locate the email to be deleted

		int	found = -1;
		for (StringList::iterator i = listing.begin(); i != listing.end(); ++i)
		{
			// Get the index and size from this line in the list

			string			index = *i;
			stripLeadingWS(index);
			stripTrailingWS(index);
			string::size_type	idx = index.find(" ");
			if (idx == string::npos) POPTHROW(_T("Invalid response from server from [uidl] command\n"));
			string			thisID = index.substr(idx + 1);
			index.erase(idx);

			if (thisID == messageID)
			{
				found = atoi(index.c_str());
				break;
			}
		}

		if (found == -1) POPTHROW(_T("The message to be deleted does not seem to exist on the server\n"));

		// Try to delete the message

		string	result;
		if (!sendCommand("dele " + integerToString(found), result, tbr)) POPTHROW(_T("Unable to send [dele] command to server\n"));

		// Shutdown the connection

		if (!sendCommand("quit", result, tbr)) POPTHROW(_T("Unable to send [quit] command to server\n"));

		// We're done with this...

		socket().disconnect();
	}
	catch(const wstring & err)
	{
		return err;
	}

	return _T("");
}

// --------------------------------------------------------------------------------------------------------------------------------

bool	pop::sendCommand(const string &command, string & result, unsigned int &totalBytesReceived, bool multiline)
{
	// Start fresh

	totalBytesReceived = 0;

	// Send the command

	if (command.length())
	{
		string	cmd = command + "\r\n";

		// Hide their password in the log
		{
			string	c = cmd;
			if (areEqualNocase(c, "pass ", 5))
			{
				for (unsigned int i = 5; i < c.length() - 2; i++)
				{
					c[i] = '*';
				}
			}
		}

		wstring	err = socket().send(cmd);
		if (err.length())
		{
			LOG("socket().send() failed!", Logger::LOG_CRIT);
			LOG("Error: " + wtoa(err), Logger::LOG_CRIT);
			return false;
		}
	}

	// Build the list of strings to terminate on...

	StringList	terminationList;
	terminationList.push_back("\r\n-ERR");
	if (multiline)
	{
		terminationList.push_back("\r\n.\r\n");
	}
	else
	{
		terminationList.push_back("\r\n");
	}

	// Read the result

	result.erase();
	wstring	err = socket().recvUntil(result, totalBytesReceived, terminationList);

	// Deal with errors

	if (err.length())
	{
		// Did we get an error from the host?

		if (findNocase(result, "\r\n-ERR\r\n", 0) != string::npos)
		{
			LOG("Received -ERR from host!", Logger::LOG_CRIT);
			return false;
		}
		else
		{
			LOG(wtoa(err), Logger::LOG_CRIT);
			return false;
		}
	}

	// Validate the response

	if (!result.length())
	{
		LOG("It seems we didn't get a response!", Logger::LOG_CRIT);
		return false;
	}

	if (result.length() < 5)
	{
		LOG("It seems we didn't get a proper response (length < 5)!", Logger::LOG_CRIT);
		return false;
	}

	if (!areEqualNocase(result.substr(0, 3), "+OK"))
	{
		LOG("It seems we got an invalid response from the host (missing \"+OK\")", Logger::LOG_CRIT);
		return false;
	}

	// Erase the first (status response) line

	string::size_type	idx = result.find("\r\n");
	if (idx != string::npos)	result.erase(0, idx+2);
	else				result.erase();

	// Single-line responses are deemed OK at this point

	if (!multiline) return true;

	// Multiline responses need at least one more line

	if (!result.length())
	{
		LOG("It seems our multiline response was blank!", Logger::LOG_CRIT);
		return false;
	}

	if (result.length() < 3)
	{
		LOG("It seems our multiline response is too short (length < 3)!", Logger::LOG_CRIT);
		return false;
	}

	// Multiline responses need the last line removed

	result[result.length()-2] = ' '; // Hide the last \r
	result[result.length()-1] = ' '; // Hide the last \n
	idx = result.rfind("\r\n");
	if (idx != string::npos)	result.erase(idx);
	else				result.erase();

	// All's well!

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------
// pop.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

