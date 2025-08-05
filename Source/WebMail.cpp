// ---------------------------------------------------------------------------------------------------------------------------------
//  _   _   _      _     __  __       _ _                      
// | | | | | |    | |   |  \/  |     (_) |                     
// | | | | | | ___| |__ | \  / | __ _ _| |     ___ _ __  _ __  
// | | | | | |/ _ \ '_ \| |\/| |/ _` | | |    / __| '_ \| '_ \ 
// |  V _ V  |  __/ |_) | |  | | (_| | | | _ | (__| |_) | |_) |
//  \__/ \__/ \___|_.__/|_|  |_|\__,_|_|_|(_) \___| .__/| .__/ 
//                                                | |   | |    
//                                                |_|   |_|    
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

#include "stdafx.h"
#include "3Dme.h"
#include "Mime.h"
#include "EmailList.h"
#include "WebMail.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	WebMail::execute(const wstring &filename, const ENTRYPOINT entry)
{
	LOGFUNC("Executing WebMail script: " + wtoa(filename));

	// Where to start?

	string	entryPoint;

	switch(entry)
	{
		case EP_DELETE:
			entryPoint = "__delete:";
			break;

		default:
			entryPoint = "__retrieve:";
			break;
	}

	// Load up the script

	wstring	err = loadScript(filename);
	if (err.length())
	{
		LOG("Unable to load script", Logger::LOG_CRIT);
		return err;
	}

	// Find the __retrieve:

	unsigned int	currentIP = 0xffffffff;
	for (unsigned int i = 0; i < scriptLines().size(); i++)
	{
		currentIP = i;
		if (areEqualNocase(scriptLines()[i], entryPoint))
		{
			currentIP = i;
			break;
		}
	}

	// If we didn't find the start, complain

	if (currentIP == 0xffffffff)
	{
		LOG("Unable to find entry point (" + entryPoint + ") of script");
		return _T("Unable to find entry point in script: ") + filename;
	}
	else
	{
		LOG("Entry point (" + entryPoint + ") found at line " + integerToString(currentIP));
	}

	// Next line (first line of start routine)

	currentIP++;

	// Predefined varables...

	Var	uname;
	uname.name = "__user_name";
	uname.contents = userName();
	setVar(uname);

	Var	upass;
	upass.name = "__user_pass";
	upass.contents = userPass();
	setVar(upass);

	Var	idlist;
	idlist.name = "__msgid_list";
	idlist.contents = messageIDList();
	setVar(idlist);

	Var	delme;
	delme.name = "__delete_me";
	delme.contents = deleteMe();
	setVar(delme);

	try
	{
		// Start processing the script

		for(;;)
		{
			// Parse the line

			string	control, parms;
			if (!parseLine(currentIP, control, parms))
			{
				LOG("End of script file reached");
				break;
			}

			// Skip blank lines

			if (!control.length())
			{
				currentIP++;
				continue;
			}

			LOG("Line " + integerToString(currentIP) + ": [" + control + "] " + parms);

			StringVect	parmList;
			if (!parseParms(parms, parmList)) throw_wstring (_T("Unable to parse script parms"))

			// Is this a label?

			if (control.find(":") != string::npos)
			{
				currentIP++;
				continue;
			}

			// Blank?

			if (!control.length())
			{
				currentIP++;
				continue;
			}

			// Comment?

			if (control.length() >= 2 && control.substr(0, 2) == "//")
			{
				currentIP++;
				continue;
			}

			// Must be a command of some kind... do it.

			if (areEqualNocase(control, "setvar"))
			{
				if (!_setvar(currentIP, parmList)) throw_wstring(_T("Script error, see log file"))
				currentIP++;
			}
			else if (areEqualNocase(control, "gethttp"))
			{
				if (!_gethttp(currentIP, parmList)) throw_wstring(_T("Script error, see log file"))
			}
			else if (areEqualNocase(control, "posthttp"))
			{
				if (!_posthttp(currentIP, parmList)) throw_wstring(_T("Script error, see log file"))
			}
			else if (areEqualNocase(control, "onerror"))
			{
				if (!_onerror(currentIP, parmList)) throw_wstring(_T("Script error, see log file"))
				currentIP++;
			}
			else if (areEqualNocase(control, "call"))
			{
				if (!_call(currentIP, parmList)) throw_wstring(_T("Script error, see log file"))
			}
			else if (areEqualNocase(control, "find"))
			{
				if (!_find(currentIP, parmList)) throw_wstring(_T("Script error, see log file"))
			}
			else if (areEqualNocase(control, "rfind"))
			{
				if (!_rfind(currentIP, parmList)) throw_wstring(_T("Script error, see log file"))
			}
			else if (areEqualNocase(control, "erase"))
			{
				if (!_erase(currentIP, parmList)) throw_wstring(_T("Script error, see log file"))
				currentIP++;
			}
			else if (areEqualNocase(control, "skip"))
			{
				if (!_skip(currentIP, parmList)) throw_wstring(_T("Script error, see log file"))
				currentIP++;
			}
			else if (areEqualNocase(control, "goto"))
			{
				if (!_goto(currentIP, parmList)) throw_wstring(_T("Script error, see log file"))
			}
			else if (areEqualNocase(control, "error"))
			{
				string	errString;
				resolveParm(parmList[0], errString);
				LOG("Script returned error: " + errString, Logger::LOG_CRIT);
				throw_wstring(atow(errString))
			}
			else if (areEqualNocase(control, "stop"))
			{
				LOG("Script stopped");
				break;
			}
			else if (areEqualNocase(control, "return"))
			{
				if (!_return(currentIP, parmList)) throw_wstring(_T("Script error, see log file"))
			}
			else if (areEqualNocase(control, "dumpvar"))
			{
				if (!_dumpvar(currentIP, parmList)) throw_wstring(_T("Script error, see log file"))
				currentIP++;
			}
			else if (areEqualNocase(control, "addmail"))
			{
				if (!_addmail(currentIP, parmList)) throw_wstring(_T("Script error, see log file"))
				currentIP++;
			}
			else if (areEqualNocase(control, "remtags"))
			{
				if (!_remtags(currentIP, parmList)) throw_wstring(_T("Script error, see log file"))
				currentIP++;
			}
			else if (areEqualNocase(control, "unhtml"))
			{
				if (!_unhtml(currentIP, parmList)) throw_wstring(_T("Script error, see log file"))
				currentIP++;
			}
			else if (areEqualNocase(control, "strip"))
			{
				if (!_strip(currentIP, parmList)) throw_wstring(_T("Script error, see log file"))
				currentIP++;
			}
			else if (areEqualNocase(control, "convertreserveduri"))
			{
				if (!_convertReservedURI(currentIP, parmList)) throw_wstring(_T("Script error, see log file"))
				currentIP++;
			}
			else if (areEqualNocase(control, "setcookie"))
			{
				if (!_setCookie(currentIP, parmList)) throw_wstring(_T("Script error, see log file"))
				currentIP++;
			}
			else if (areEqualNocase(control, "setReferrer"))
			{
				if (!_setReferrer(currentIP, parmList)) throw_wstring(_T("Script error, see log file"))
				currentIP++;
			}
			else if (areEqualNocase(control, "setUserAgent"))
			{
				if (!_setUserAgent(currentIP, parmList)) throw_wstring(_T("Script error, see log file"))
				currentIP++;
			}
			else if (areEqualNocase(control, "dumpCookies"))
			{
				if (!_dumpCookies(currentIP, parmList)) throw_wstring(_T("Script error, see log file"))
				currentIP++;
			}
			else
			{
				LOG("Unknown control keyword: " + control, Logger::LOG_CRIT);
				throw_wstring(_T("Script error, see log file"))
			}
		}
	}
	catch (const wstring & errstr)
	{
		for (unsigned int i = 0; i < ipStack.size(); ++i) UNDENT("HANDLING EXCEPTION");

		wstring	name = filename;
		string::size_type idx = name.rfind(_T("\\"));
		if (idx != string::npos) name.erase(0, idx + 1);

		LOG("Script or syntax error @ line " + integerToString(currentIP), Logger::LOG_CRIT);
		return name + _T("(") + integerToWString(currentIP) + _T(") error:\n") + errstr;
	}

	return _T("");
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	WebMail::loadScript(const wstring &filename)
{
	// Open the file

	ifstream        stream(wtoa(filename).c_str(), ios::in);
	if (!stream) return _T("Unable to open webmail script: ") + filename;

	// Start clean

	scriptLines().clear();

	// We'll add a simple comment at the beginning so that the script processor returns
	// proper 1-based line numbers

	scriptLines().push_back("// line 0");

	// Read & parse each line in the file

	for(;;)
	{
		// A line of text...

		string	line;

		// Read the line

		getline(stream, line);
		if (stream.bad() || stream.fail()) break;

		// Ignore any indentation & empty chars at the end of the line

		stripLeadingWS(line);
		stripTrailingWS(line);

		// Ignore comments

		if (line.substr(0, 2) == "//") line = "";

		// Add this to the script

		scriptLines().push_back(line);
	}

	stream.close();
	return _T("");
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::parseLine(const unsigned int ip, string &control, string &parms)
{
	// Start clean

	control = "";
	parms = "";

	// Past the end?

	if (ip >= scriptLines().size()) return false;

	// Get a line

	string	line = scriptLines()[ip];

	// Is this line blank?

	if (line.length())
	{
		// Find the first space

		string::size_type idx = line.find_first_of("\x09\x0a\x0b\x0c\x0d\x20");

		// Just a control word, and no parms?

		if (idx == string::npos)
		{
			control = line;
		}

		// Control word & parms

		else
		{
			control = line.substr(0, idx);
			parms = line.substr(idx+1);
			stripLeadingWS(parms);
		}
	}

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

unsigned int	WebMail::findLabel(const string &label) const
{

	string	tester(label + ":");

	for (unsigned int i = 0; i < scriptLines().size(); i++)
	{
		if (scriptLines()[i] == tester) return i;
	}

	return 0xffffffff;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::parseParms(const string &parms, StringVect &out)
{
	// Start fresh

	string		in(parms);
	out.clear();

	// Start parsing

	while(in.length())
	{
		// Prepare it

		stripLeadingWS(in);

		// Is this parm a variable?

		if (in[0] != '\"')
		{
			string			varname;
			string::size_type	idx = in.find_first_of("\x09\x0a\x0b\x0c\x0d\x20");

			if (idx == string::npos)
			{
				varname = in;
				in.erase();
			}
			else
			{
				varname = in.substr(0, idx);
				in.erase(0, idx);
				stripLeadingWS(in);
			}

			out.push_back(varname);
		}

		// String literal

		else
		{
			// Remove the starting quote...

			in.erase(0, 1);

			// Put it in the result so we know it's a string...

			string	temp;
			temp += "\"";

			while(in.length())
			{
				// End of string?

				if (in[0] == '\"')
				{
					in.erase(0, 1);
					break;
				}

				// Escaped char?

				else if (in[0] == '\\')
				{
					in.erase(0, 1);

					if (!in.length())
					{
						LOG("Parms contain backslash at end of line??", Logger::LOG_CRIT);
						return false;
					}

					switch(in[0])
					{
						case 'x':
						case 'X':
						{
							in.erase(0, 1);

							if (in.length() < 2)
							{
								LOG("Parms contain incomplete hexidecimal character in string", Logger::LOG_CRIT);
								return false;
							}

							char		i0 = tolower(in[0]);
							char		i1 = tolower(in[1]);
							in.erase(0, 2);

							unsigned int	hexChar = 0;
							     if (i0 >= 'a' && i0 <= 'f') hexChar |= (i0 - 'a' + 10) << 4;
							else if (i0 >= '0' && i0 <= '9') hexChar |= (i0 - '0') << 4;
							     if (i1 >= 'a' && i1 <= 'f') hexChar |= (i1 - 'a' + 10);
							else if (i1 >= '0' && i1 <= '9') hexChar |= (i1 - '0');
							break;
						}

						case 'a':
						case 'A':
							in.erase(0, 1);
							temp += "\a";
							break;

						case 'b':
						case 'B':
							in.erase(0, 1);
							temp += "\b";
							break;

						case 'f':
						case 'F':
							in.erase(0, 1);
							temp += "\f";
							break;

						case 'n':
						case 'N':
							in.erase(0, 1);
							temp += "\n";
							break;

						case 'r':
						case 'R':
							in.erase(0, 1);
							temp += "\r";
							break;

						case 't':
						case 'T':
							in.erase(0, 1);
							temp += "\t";
							break;

						case 'v':
						case 'V':
							in.erase(0, 1);
							temp += "\v";
							break;

						case '\\':
							in.erase(0, 1);
							temp += "\\";
							break;

						case '\?':
							in.erase(0, 1);
							temp += "\?";
							break;

						case '\'':
							in.erase(0, 1);
							temp += "\'";
							break;

						case '\"':
							in.erase(0, 1);
							temp += "\"";
							break;

						default:
							LOG("Unknown escape code: \\" + in[0], Logger::LOG_CRIT);
							return false;
					}
				}

				// Plain char... add it to the temp string

				else
				{
					temp += in[0];
					in.erase(0, 1);
				}
			}

			// Terminat the string literal

			temp += "\"";
			out.push_back(temp);
		}
	}

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::varExist(const string &name) const
{
	const VarVect	&v = variables();

	for (unsigned int i = 0; i < v.size(); i++)
	{
		if (areEqualNocase(v[i].name, name)) return true;
	}

	return false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::setVar(const Var &var)
{
	// Create a simpler version of the contents for logging...

	string	temp = var.contents;
	temp.insert(var.pointer, "^");
	int	t = var.pointer - 10;
	if (t < 0) t = 0;
	temp.erase(0, t);
	if (temp.length() > 50)
	{
		temp.erase(50);
		temp += "...";
	}

	for(;;)
	{
		string::size_type idx = temp.find_first_of("\r\n\t");
		if (idx == string::npos) break;

		if (temp[idx] == '\r')
		{
			temp.erase(idx);
			temp.insert(idx, "\\r");
		}
		else if (temp[idx] == '\n')
		{
			temp.erase(idx);
			temp.insert(idx, "\\n");
		}
		else if (temp[idx] == '\t')
		{
			temp.erase(idx);
			temp.insert(idx, "\\t");
		}
	}

	// Does it exist?

	VarVect	&v = variables();

	for (unsigned int i = 0; i < v.size(); i++)
	{
		if (areEqualNocase(v[i].name, var.name))
		{
			LOG("Updating variable '" + var.name + "' with: \"" + temp + "\"");
			v[i] = var;
			return true;
		}
	}

	LOG("Adding variable '" + var.name + "' with contents \"" + temp + "\"");
	v.push_back(var);
	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::getVar(const string &name, Var &var) const
{
	const VarVect	&v = variables();

	for (unsigned int i = 0; i < v.size(); i++)
	{
		if (areEqualNocase(v[i].name, name))
		{
			var = v[i];
			return true;
		}
	}

	LOG("Script variable referenced before it was declared: " + name, Logger::LOG_CRIT);
	return false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::resolveParm(const string &str, string &result) const
{
	// Is this a string literal?

	if (str[0] == '\"')
	{
		result = str;
		result.erase(0, 1);
		result.erase(result.length() - 1);
		return true;
	}

	// It's a variable, resole it

	Var	v;
	if (!getVar(str, v)) return false;
	result = v.contents;

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::resolveIndex(const string &str, unsigned int &index) const
{
	// Is the string an index from a variable?
	
	if (str[0] == '@')
	{
		Var	v;
		if (!getVar(str.substr(1), v))
		{
			LOG("Trying to get an index/count from a variable that hasn't been declared yet: " + str.substr(1));
			return false;
		}
		index = v.pointer;
	}

	// Treat it like it's just a number

	else
	{
		if (str[0] < '0' || str[0] > '9')
		{
			LOG("Invalid index/count value: " + str);
			return false;
		}
		index = atoi(str.c_str());
	}

	// Okay

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::resolveCount(const string &str, int &index) const
{
	// Is the string an index from a variable?
	
	if (str[0] == '@')
	{
		Var	v;
		if (!getVar(str.substr(1), v))
		{
			LOG("Trying to get an index/count from a variable that hasn't been declared yet: " + str.substr(1));
			return false;
		}
		index = v.pointer;
	}

	// Treat it like it's just a number

	else
	{
		if ((str[0] < '0' || str[0] > '9') && str[0] != '-')
		{
			LOG("Invalid index/count value: " + str);
			return false;
		}
		index = atoi(str.c_str());
	}

	// Okay

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::handleOnError(unsigned int &currentIP)
{
	if (!currentOnError.length())
	{
		LOG("An error occurred, yet no OnError was set!", Logger::LOG_CRIT);
		return false;
	}

	unsigned int	newIP = findLabel(currentOnError);
	if (newIP == 0xffffffff)
	{
		LOG("OnError - label not found: " + currentOnError, Logger::LOG_CRIT);
		return false;
	}

	LOG("Jumping to current OnError label: " + currentOnError + " @ line " + integerToString(currentIP));
	currentIP = newIP;
	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	WebMail::push(const unsigned int currentIP, const string &routineName)
{
	INDENT(routineName);
	ipStack.push_front(currentIP);
	onErrorStack.push_front(currentOnError);
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::pop(unsigned int &currentIP)
{
	if (!ipStack.size()) return false;

	currentIP = ipStack.front();
	ipStack.pop_front();

	currentOnError = onErrorStack.front();
	onErrorStack.pop_front();

	UNDENT("RETURNED");
	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::_setvar(unsigned int &currentIP, const StringVect &parms)
{
	if (!parms.size())
	{
		LOG("SetVar requires at least one parameter", Logger::LOG_CRIT);
		return false;
	}

	// Get the variable name

	Var	newVar;
	newVar.name = parms[0];

	for (unsigned int i = 1; i < parms.size(); i++)
	{
		string	temp;
		if (!resolveParm(parms[i], temp)) return false;
		newVar.contents += temp;
	}

	// Put it there...

	setVar(newVar);
	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::_gethttp(unsigned int &currentIP, const StringVect &parms)
{
	if (parms.size() != 2)
	{
		LOG("GetHTTP requires two parameters", Logger::LOG_CRIT);
		return false;
	}

	Var	newVar;
	newVar.name = parms[0];

	// Get the URL

	string	url;
	resolveParm(parms[1], url);

	// Get the page

	wstring	err = website().getPage(url, newVar.contents, "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; .NET CLR 1.0.3800)");

	// Update the redirection server (if any)

	if (err.length())
	{
		LOG("Error trying to get page: " + wtoa(err));
		return handleOnError(currentIP);
	}

	setVar(newVar);
	currentIP++;
	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::_posthttp(unsigned int &currentIP, const StringVect &parms)
{
	if (parms.size() != 3)
	{
		LOG("PostHTTP requires three parameters", Logger::LOG_CRIT);
		return false;
	}

	Var	newVar;
	newVar.name = parms[0];

	// Get the URL

	string	url;
	resolveParm(parms[1], url);

	// Get the Post data

	string	postData;
	resolveParm(parms[2], postData);

	// Get the page

	wstring	err = website().postPage(url, postData, newVar.contents, "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; .NET CLR 1.0.3800)");

	// Update the redirection server (if any)

	if (err.length())
	{
		LOG("Error trying to get page (via post): " + wtoa(err));
		return handleOnError(currentIP);
	}

	setVar(newVar);
	currentIP++;
	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::_onerror(unsigned int &currentIP, const StringVect &parms)
{
	if (parms.size() != 1)
	{
		LOG("OnError requires one parameter", Logger::LOG_CRIT);
		return false;
	}

	// See if the label is valid

	unsigned int	newIP = findLabel(parms[0]);
	if (newIP == 0xffffffff)
	{
		LOG("OnError - label not found: " + parms[0], Logger::LOG_CRIT);
		return false;
	}

	currentOnError = parms[0];
	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::_call(unsigned int &currentIP, const StringVect &parms)
{
	if (parms.size() != 1)
	{
		LOG("Call requires one parameter", Logger::LOG_CRIT);
		return false;
	}

	unsigned int	newIP = findLabel(parms[0]);
	if (newIP == 0xffffffff)
	{
		LOG("Call - label not found: " + parms[0], Logger::LOG_CRIT);
		return false;
	}

	// Push the stack

	push(currentIP, parms[0]);
	currentIP = newIP;

	// Done

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::_find(unsigned int &currentIP, const StringVect &parms)
{
	if (parms.size() < 2)
	{
		LOG("Find requires at least two parameters", Logger::LOG_CRIT);
		return false;
	}

	// Get the dest

	Var	srcVar;
	if (!getVar(parms[0], srcVar))
	{
		LOG("Find - source variable not declared: " + parms[0], Logger::LOG_CRIT);
		return false;
	}

	// Get the search critiera

	string	criteria;

	for (unsigned int i = 1; i < parms.size(); i++)
	{
		string	temp;
		if (!resolveParm(parms[i], temp)) return false;
		criteria += temp;
	}

	if (!criteria.length())
	{
		LOG("Find - empty search criteria", Logger::LOG_CRIT);
		return false;
	}

	string::size_type idx = findNocase(srcVar.contents, criteria, srcVar.pointer);
	if (idx == string::npos)
	{
		return handleOnError(currentIP);
	}

	srcVar.pointer = idx;
	setVar(srcVar);
	currentIP++;
	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::_rfind(unsigned int &currentIP, const StringVect &parms)
{
	if (parms.size() < 2)
	{
		LOG("RFind requires at least two parameters", Logger::LOG_CRIT);
		return false;
	}

	// Get the dest

	Var	srcVar;
	if (!getVar(parms[0], srcVar))
	{
		LOG("RFind - source variable not declared: " + parms[0], Logger::LOG_CRIT);
		return false;
	}

	// Get the search critiera

	string	criteria;

	for (unsigned int i = 1; i < parms.size(); i++)
	{
		string	temp;
		if (!resolveParm(parms[i], temp)) return false;
		criteria += temp;
	}

	if (!criteria.length())
	{
		LOG("RFind - empty search criteria", Logger::LOG_CRIT);
		return false;
	}

	string::size_type idx = rfindNocase(srcVar.contents, criteria, srcVar.pointer);
	if (idx == string::npos)
	{
		return handleOnError(currentIP);
	}

	srcVar.pointer = idx;
	setVar(srcVar);
	currentIP++;
	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::_erase(unsigned int &currentIP, const StringVect &parms)
{
	if (!parms.size())
	{
		LOG("Erase requires at least one parameter", Logger::LOG_CRIT);
		return false;
	}

	// Get the dest

	Var	dstVar;
	if (!getVar(parms[0], dstVar))
	{
		LOG("Erase - dest variable not declared: " + parms[0]);
		return false;
	}

	// Get the index

	unsigned int	index = 0;
	if (parms.size() >= 2)
	{
		if (!resolveIndex(parms[1], index))
		{
			LOG("Erase - parm #2 (index) is invalid");
	        	return false;
		}
	}
	if (index >= dstVar.contents.length()) return true;

	// Get the count

	unsigned int	count = 0xffffffff;
	if (parms.size() >= 3)
	{
		if (!resolveIndex(parms[2], count))
		{
			LOG("Erase - Parm #3 (count) is invalid");
			return false;
		}
	}

	if (count + index >= dstVar.contents.length())
	{
		count = dstVar.contents.length() - index;
	}

	dstVar.contents.erase(index, count);
	dstVar.pointer = index;
	setVar(dstVar);
	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::_skip(unsigned int &currentIP, const StringVect &parms)
{
	if (parms.size() != 2)
	{
		LOG("Skip requires two parameters", Logger::LOG_CRIT);
		return false;
	}

	// Get the dest

	Var	dstVar;
	if (!getVar(parms[0], dstVar))
	{
		LOG("Skip - dest variable not declared: " + parms[0]);
		return false;
	}

	// Get the count

	int	count = 0;
	if (!resolveCount(parms[1], count))
	{
		LOG("Skip - Parm #2 (count) is invalid");
		return false;
	}

	dstVar.pointer = (int) dstVar.pointer + count;

	if ((int) dstVar.pointer < 0)
	{
		LOG("Skip - Warning -- skipping past the beginning of the variable, will only skip to the first char.");
		dstVar.pointer = 0;
	}

	if (dstVar.pointer > dstVar.contents.length())
	{
		LOG("Skip - Warning -- skipping past the end of the variable, will only skip to the last char.");
		dstVar.pointer = dstVar.contents.length() - 1;
	}

	setVar(dstVar);
	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::_goto(unsigned int &currentIP, const StringVect &parms)
{
	if (parms.size() != 1)
	{
		LOG("Goto requires one parameter", Logger::LOG_CRIT);
		return false;
	}

	unsigned int	newIP = findLabel(parms[0]);
	if (newIP == 0xffffffff)
	{
		LOG("Goto - label not found: " + parms[0], Logger::LOG_CRIT);
		return false;
	}
	currentIP = newIP;
	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::_return(unsigned int &currentIP, const StringVect &parms)
{
	bool	okay = pop(currentIP);

	// Signal the end of the script

	if (!okay)	currentIP = 0xffffffff;
	else		currentIP++; // don't re-execute the call

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::_dumpvar(unsigned int &currentIP, const StringVect &parms)
{
	if (parms.size() != 1)
	{
		LOG("dumpvar requires one parameter", Logger::LOG_CRIT);
		return false;
	}

	// Get the dest

	Var	dstVar;
	if (!getVar(parms[0], dstVar))
	{
		LOG("DumpVar - dest variable not declared: " + parms[0]);
		return false;
	}

	RAW(
		"------------------------------------------------------------------------------------------------------------------------------------\n"
		"Dumping variable named: \"" + dstVar.name + "\" which has a pointer at " + integerToString(dstVar.pointer) + "\n" +
		"------------------------------------------------------------------------------------------------------------------------------------\n"
		+ dstVar.contents + "\n" +
		"------------------------------------------------------------------------------------------------------------------------------------\n"
	);

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::_addmail(unsigned int &currentIP, const StringVect &parms)
{
	if (parms.size() != 7)
	{
		LOG("addmail requires seven parameters", Logger::LOG_CRIT);
		return false;
	}

	string	msgid, from, to, subj, date, body, attach;
	if (!resolveParm(parms[0], msgid)) return false;
	if (!resolveParm(parms[1], from)) return false;
	if (!resolveParm(parms[2], to)) return false;
	if (!resolveParm(parms[3], subj)) return false;
	if (!resolveParm(parms[4], date)) return false;
	if (!resolveParm(parms[5], body)) return false;
	if (!resolveParm(parms[6], attach)) return false;

	{
		LOGBLOCK("Adding email");
		LOG("ID  : " + msgid);
		LOG("From: " + from);
		LOG("To  : " + to);
		LOG("Subj: " + subj);
		LOG("Date: " + date);
		if (attach.length())	LOG("Atch: Yes");
		else			LOG("Atch: No");
	}

	if (!from.length()) from = "No sender specified";
	if (!to.length()) to = "No recipient specified";
	if (!date.length()) date = "No date specified";

	// Look to see if this messageID already exists

	bool	found = false;
	for (EmailList::iterator i = oldMails().begin(); i != oldMails().end(); ++i)
	{
		if (i->messageID == msgid)
		{
			found = true;
			i->newFlag = false;
			newMails().push_back(*i);
			break;
		}
	}

	// Does this email already exist in the list?

	if (!found)
	{
		// Generate a MIME header so we can go with it

		string	mimeHeader;
		mimeHeader += "date: " + date + "\r\n";
		mimeHeader += "from: " + from + "\r\n";
		mimeHeader += "to: " + to + "\r\n";
		mimeHeader += "subject: " + subj + "\r\n";
		mimeHeader += "content-type: text/html\r\n";
		mimeHeader += "\r\n";

		sEmail	e;
		e.messageID = msgid;
		e.newFlag = true;
		e.pendingFlag = true;
		e.receivedSize = body.length();
		e.size = body.length();
		StringList	sl;
		convertToStringList(mimeHeader, sl);
		unsigned int	embeddedSkip;
		e.mimeHeader.parse(sl, embeddedSkip);
		e.mimeHeader.parseParts("\r\n" + body);
		if (attach.length()) e.mimeHeader.hasAttachment() = true;
		newMails().push_back(e);
	}

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::_remtags(unsigned int &currentIP, const StringVect &parms)
{
	if (parms.size() != 1)
	{
		LOG("remtags requires one parameter", Logger::LOG_CRIT);
		return false;
	}

	// Get the dest

	Var	dstVar;
	if (!getVar(parms[0], dstVar))
	{
		LOG("remtags - dest variable not declared: " + parms[0]);
		return false;
	}

	if (dstVar.contents[0] == '<')
	{
		string::size_type idx = dstVar.contents.find(">");
		if (idx != string::npos) dstVar.contents.erase(0, idx+1);
		if (dstVar.pointer > 0) dstVar.pointer -= idx + 1;
		setVar(dstVar);
	}

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::_unhtml(unsigned int &currentIP, const StringVect &parms)
{
	if (parms.size() != 1)
	{
		LOG("unhtml requires one parameter", Logger::LOG_CRIT);
		return false;
	}

	// Get the dest

	Var	dstVar;
	if (!getVar(parms[0], dstVar))
	{
		LOG("unhtml - dest variable not declared: " + parms[0]);
		return false;
	}

	bool	changed = false;
	for(;;)
	{
		string::size_type idx;

		idx = findNocase(dstVar.contents, "&gt;");
		if (idx != string::npos)
		{
			dstVar.contents.erase(idx, 4);
			dstVar.contents.insert(idx, ">");
			if (dstVar.pointer > idx) dstVar.pointer -= 3;
			changed = true;
			continue;
		}

		idx = findNocase(dstVar.contents, "&lt;");
		if (idx != string::npos)
		{
			dstVar.contents.erase(idx, 4);
			dstVar.contents.insert(idx, "<");
			if (dstVar.pointer > idx) dstVar.pointer -= 3;
			changed = true;
			continue;
		}

		idx = findNocase(dstVar.contents, "&quot;");
		if (idx != string::npos)
		{
			dstVar.contents.erase(idx, 6);
			dstVar.contents.insert(idx, "\"");
			if (dstVar.pointer > idx) dstVar.pointer -= 5;
			changed = true;
			continue;
		}

		idx = findNocase(dstVar.contents, "&nbsp;");
		if (idx != string::npos)
		{
			dstVar.contents.erase(idx, 6);
			dstVar.contents.insert(idx, " ");
			if (dstVar.pointer > idx) dstVar.pointer -= 5;
			changed = true;
			continue;
		}

		idx = findNocase(dstVar.contents, "&amp;");
		if (idx != string::npos)
		{
			dstVar.contents.erase(idx, 5);
			dstVar.contents.insert(idx, "&");
			if (dstVar.pointer > idx) dstVar.pointer -= 4;
			changed = true;
			continue;
		}

		idx = findNocase(dstVar.contents, "&#");
		if (idx != string::npos)
		{
			string::size_type	semicolon = dstVar.contents.find(";", idx);
			if (semicolon != string::npos)
			{
				string	number = dstVar.contents.substr(idx+2, semicolon-idx-2);
				dstVar.contents.erase(idx, semicolon-idx+1);
				char	foo[10];
				sprintf(foo, "%c", atoi(number.c_str()));
				dstVar.contents.insert(idx, foo);
				if (dstVar.pointer > idx) dstVar.pointer -= semicolon-idx;
				changed = true;
				continue;
			}
		}

		break;
	}

	if (changed) setVar(dstVar);
	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::_strip(unsigned int &currentIP, const StringVect &parms)
{
	if (parms.size() != 1)
	{
		LOG("strip requires one parameter", Logger::LOG_CRIT);
		return false;
	}

	// Get the dest

	Var	dstVar;
	if (!getVar(parms[0], dstVar))
	{
		LOG("strip - dest variable not declared: " + parms[0]);
		return false;
	}

	stripLeadingWS(dstVar.contents);
	stripTrailingWS(dstVar.contents);
	dstVar.pointer = 0;
	setVar(dstVar);
	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::_convertReservedURI(unsigned int &currentIP, const StringVect &parms)
{
	if (parms.size() != 1)
	{
		LOG("convertReservedURI requires one parameter", Logger::LOG_CRIT);
		return false;
	}

	// Get the dest

	Var	dstVar;
	if (!getVar(parms[0], dstVar))
	{
		LOG("convertReservedURI - dest variable not declared: " + parms[0]);
		return false;
	}

	string	result;

	// Convert the reserved characters into hex octets
	//
	// This information comes from RFC 2396, "URI Generic Syntax", August 1998

	for (unsigned int i = 0; i < dstVar.contents.length(); ++i)
	{
		if (dstVar.contents[i] == ';' ||
		    dstVar.contents[i] == '/' ||
		    dstVar.contents[i] == '?' ||
		    dstVar.contents[i] == ':' ||
		    dstVar.contents[i] == '@' ||
		    dstVar.contents[i] == '&' ||
		    dstVar.contents[i] == '=' ||
		    dstVar.contents[i] == '+' ||
		    dstVar.contents[i] == '$' ||
		    dstVar.contents[i] == ',')
		{
			char	foo[10];
			sprintf(foo, "%%%02X", dstVar.contents[i]);
			result += foo;
		}
		else
		{
			result += dstVar.contents[i];
		}
	}

	dstVar.contents = result;

	dstVar.pointer = 0;
	setVar(dstVar);
	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::_setCookie(unsigned int &currentIP, const StringVect &parms)
{
	if (!parms.size())
	{
		LOG("SetCookie requires at least one parameter", Logger::LOG_CRIT);
		return false;
	}

	// Get the variable name

	string	cookie;

	for (unsigned int i = 0; i < parms.size(); i++)
	{
		string	temp;
		if (!resolveParm(parms[i], temp)) return false;
		cookie += temp;
	}

	// Put it there...

	website().addCookie(cookie);

	// Done

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::_setReferrer(unsigned int &currentIP, const StringVect &parms)
{
	if (!parms.size())
	{
		LOG("SetReferrer requires at least one parameter", Logger::LOG_CRIT);
		return false;
	}

	// Get the variable name

	string	referrer;

	for (unsigned int i = 0; i < parms.size(); i++)
	{
		string	temp;
		if (!resolveParm(parms[i], temp)) return false;
		referrer += temp;
	}

	// Put it there...

	website().referrer() = referrer;

	// Done

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::_setUserAgent(unsigned int &currentIP, const StringVect &parms)
{
	if (!parms.size())
	{
		LOG("SetBrowser requires at least one parameter", Logger::LOG_CRIT);
		return false;
	}

	// Get the variable name

	string	userAgent;

	for (unsigned int i = 0; i < parms.size(); i++)
	{
		string	temp;
		if (!resolveParm(parms[i], temp)) return false;
		userAgent += temp;
	}

	// Put it there...

	website().defaultUserAgent() = userAgent;

	// Done

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	WebMail::_dumpCookies(unsigned int &currentIP, const StringVect &parms)
{
	if (parms.size())
	{
		LOG("dumpCookies doesn't use any parameters, ignoring them", Logger::LOG_CRIT);
	}

	// Get the variable name

	RAW("------------------------------------------------------------------------------------------------------------------------------------");
	RAW("Dumping cookies:");
	RAW("------------------------------------------------------------------------------------------------------------------------------------");

	for (StringList::const_iterator & i = website().cookies().begin(); i != website().cookies().end(); ++i)
	{
		RAW("--> " + *i);
	}

	RAW("------------------------------------------------------------------------------------------------------------------------------------");

	// Done

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------
// WebMail.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

