// ---------------------------------------------------------------------------------------------------------------------------------
//  __  __ _                                     
// |  \/  (_)                                    
// | \  / |_ _ __ ___   ___      ___ _ __  _ __  
// | |\/| | | '_ ` _ \ / _ \    / __| '_ \| '_ \ 
// | |  | | | | | | | |  __/ _ | (__| |_) | |_) |
// |_|  |_|_|_| |_| |_|\___|(_) \___| .__/| .__/ 
//                                  | |   | |    
//                                  |_|   |_|    
//
// Mime file type manager
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 02/12/2001 by Paul Nettle
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
#include "Socket.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

static	bool	testIpForSpam(const string & inip)
{
	// Reverse the IP address

	string	ip = inip;
	string	reversedIP = Socket::reverseIP(ip);
	wstring	err;

	// ORDB

	string	ordbLookupString = reversedIP + ".relays.ordb.org";
	err = Socket::lookup(ordbLookupString, ip);
	if (!err.length() && ip == "127.0.0.2")
	{
		LOG(inip + " ---> determined to be spam by relays.ordb.org", Logger::LOG_CRIT);
		return true;
	}

	// VISI

//	string	visiLookupString = reversedIP + ".relays.visi.com";
//	err = Socket::lookup(visiLookupString, ip);
//	if (!err.length() && ip == "127.0.0.2")
//	{
//		LOG(inip + " ---> determined to be spam by relays.visi.com", Logger::LOG_CRIT);
//		return true;
//	}

	// spamcop

	string	spamcopLookupString = reversedIP + ".bl.spamcop.net";
	err = Socket::lookup(spamcopLookupString, ip);
	if (!err.length() && ip == "127.0.0.2")
	{
		LOG(inip + " ---> determined to be spam by bl.spamcop.net", Logger::LOG_CRIT);
		return true;
	}

	return false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

static	bool	isSpam(const string & body)
{
	if (!theApp.prefs().other()[_T("checkSpam")].booleanValue()) return false;

	string::size_type	idx = body.find(" by ");
	if (idx == string::npos) return false;
	idx += 3;

	idx = body.find_first_not_of("\r\n\t ", idx);
	if (idx == string::npos) return false;

	string::size_type	end = body.find_first_of("\t ", idx);
	if (end == string::npos) return false;

	string	server = body.substr(idx, end-idx);

	// Lookup the IP address

	string	ip;
	wstring	err = Socket::lookup(server, ip);
	if (err.length()) return false;

	if (testIpForSpam(ip)) return true;

	// Scan the text for ip addresses

	idx = 0;

	while(idx < body.length() && idx != string::npos)
	{
		string	thisIP;

		// Find the start of a potential IP address

		string::size_type	start = body.find_first_of("123456789", idx);
		if (start == string::npos) break;

		// Find the end of this potential IP address

		string::size_type	end = body.find_first_not_of("0123456789.", start);
		if (end == string::npos)
		{
			thisIP = body.substr(start);
			idx = string::npos;
		}
		else
		{
			thisIP = body.substr(start, end-start);
			idx = end;
		}

		// Is it a valid IP?

		if (Socket::isIP(thisIP))
		{
			if (testIpForSpam(thisIP)) return true;
		}
	}

	return false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

static	bool	removeComments(string & str)
{
	// As per rfc2822 (section 3.2.3):
	//
	// "Strings of characters enclosed in parentheses are considered comments
	// so long as they do not appear within a "quoted-string", as defined in
	// section 3.2.5.  Comments may nest."

	#define	DQUOTE "\""
	#define	WSP " \t"

	string::size_type	idx = 0;
	while(idx != -1)
	{
		// Find the next pertinent token...

		string::size_type	quote = str.find(DQUOTE, idx);
		string::size_type	lparen = str.find("(", idx);

		// If the quote came first, skip it...

		if (quote != -1 && quote < lparen)
		{
			// Find the end of the quoted string

			for (;;)
			{
				// Find the next quote

				string::size_type	end = str.find(DQUOTE, quote+1);
				if (end == -1) return false;

				// Skip escaped quotes within the quoted string

				if (str[end-1] == '\\') quote = end+1;

				// Yep, this is the end, all right. Skip it.

				idx = end+1;
				break;
			}
		}

		// If a comment, remove it

		else if (lparen != -1)
		{
			// Skip up to the comment, then

			idx = lparen;

			// Locate the end of the comment

			string::size_type	end = lparen + 1;
			int			commentNestedDepth = 1;

			while(commentNestedDepth)
			{
				// Find the start or end of a comment (since they can nest)

				string::size_type	lp = str.find("(", end);
				string::size_type	rp = str.find(")", end);

				// Comment start (nested)

				if (lp != -1 && (lp < rp || rp == -1))
				{
					end = lp + 1;
					if (str[lp-1] != '\\') ++commentNestedDepth;
				}

				// Comment end

				else if (rp != -1)
				{
					end = rp + 1;
					if (str[rp-1] != '\\') --commentNestedDepth;
				}

				// Error condition, no end comment

				else
				{
					return false;
				}
			}

			// Just to keep things clean, we'll skip any whitespace following the comment

			string::size_type	wsp = str.find_first_not_of(WSP, end);
			if (wsp != -1) end = wsp;

			// Remove the comment

			str.erase(idx, end-idx);

			// Make sure the comment is replaced by a single space if there isn't one...

			if (str[idx-1] != ' ' && str[idx-1] != '\t')
			{
				str = str.substr(0, idx) + " " + str.substr(idx);
			}
		}

		// No comments or quoted strings left, we're done

		else
		{
			break;
		}
	}

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

static	void	parseTime(const string &str, string &hour, string &min, string &sec)
{
	hour = "0";
	min = "0";
	sec = "0";
	string			s = str;
	string::size_type	idx;

	idx = s.find(":");
	if (idx == string::npos)
	{
		hour = s;
		return;
	}
	else
	{
		hour = s.substr(0, idx);
	}
	s.erase(0, idx+1);

	idx = s.find(":");
	if (idx == string::npos)
	{
		min = s;
		return;
	}
	else
	{
		min = s.substr(0, idx);
	}
	s.erase(0, idx+1);

	idx = s.find_first_not_of("0123456789");
	if (idx == string::npos)
	{
		sec = s;
		return;
	}
	else
	{
		sec = s.substr(0, idx);
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------
// This is a pathetically long routine for doing what it does, but there's a lot of detail in what it's doing, so be careful!
// ---------------------------------------------------------------------------------------------------------------------------------

static	void	parseDate(string &str)
{
	// My GMT offset (required for calculating when an email is received in our own timezone)

	time_t	a = time(NULL);
	time_t	gmt = mktime(gmtime(&a));
	time_t	loc = mktime(localtime(&a));
	int	myGmtOffset = (loc - gmt) / 6 / 6;

	// Input dates are as follows:
	//
	// [weekday,] dy mon year hh:mm[:ss] [[+/-]hhmm] [timezone]

	string	s = str;
	string::size_type	idx;

	string	weekday;
	idx = s.find(",");
	if (idx != string::npos)
	{
		string	weekday = s.substr(0, idx);
		s.erase(0, idx);
		idx = s.find_first_not_of(" ,");
		if (idx == string::npos) return;
		s.erase(0, idx);
	}

	idx = s.find_first_of(" ,");
	if (idx == string::npos) return;
	string	dayOfMonth = s.substr(0, idx);
	s.erase(0, idx);
	idx = s.find_first_not_of(" ,");
	if (idx == string::npos) return;
	s.erase(0, idx);

	idx = s.find_first_of(" ,");
	if (idx == string::npos) return;
	string	month = s.substr(0, idx);
	s.erase(0, idx);
	idx = s.find_first_not_of(" ,");
	if (idx == string::npos) return;
	s.erase(0, idx);

	idx = s.find_first_of(" ,");
	if (idx == string::npos) return;
	string	year = s.substr(0, idx);
	s.erase(0, idx);
	idx = s.find_first_not_of(" ,");
	if (idx == string::npos) return;
	s.erase(0, idx);

	idx = s.find_first_of(" ,");
	if (idx == string::npos) return;
	string	time = s.substr(0, idx);
	s.erase(0, idx);
	idx = s.find_first_not_of(" ,");
	if (idx == string::npos) return;
	s.erase(0, idx);

	string	hour, min, sec;
	parseTime(time, hour, min, sec);

	idx = s.find_first_of(" ,");
	if (idx != string::npos) s.erase(idx);
	string	offset = s;
	if (offset[0] != '-' && offset[0] != '+') offset = integerToString(myGmtOffset);

	int	hisGmtOffset = atoi(offset.c_str());
	int	offsetTime = myGmtOffset - hisGmtOffset;
	int	offsetHours = offsetTime / 100;
	int	offsetMinutes = offsetTime % 100;
	int	theOffset = offsetHours * 60 * 60 + offsetMinutes * 60;
	struct	tm ts;
	ts.tm_hour = atoi(hour.c_str());
	ts.tm_isdst = -1;
	ts.tm_mday = atoi(dayOfMonth.c_str());
	ts.tm_min = atoi(min.c_str());
	ts.tm_mon = 0;
	     if (areEqualNocase(month, "fe",  2)) ts.tm_mon = 1;
	else if (areEqualNocase(month, "mar", 3)) ts.tm_mon = 2;
	else if (areEqualNocase(month, "ap",  2)) ts.tm_mon = 3;
	else if (areEqualNocase(month, "may", 3)) ts.tm_mon = 4;
	else if (areEqualNocase(month, "jun", 3)) ts.tm_mon = 5;
	else if (areEqualNocase(month, "jul", 3)) ts.tm_mon = 6;
	else if (areEqualNocase(month, "au",  2)) ts.tm_mon = 7;
	else if (areEqualNocase(month, "se",  2)) ts.tm_mon = 8;
	else if (areEqualNocase(month, "oc",  2)) ts.tm_mon = 9;
	else if (areEqualNocase(month, "no",  2)) ts.tm_mon = 10;
	else if (areEqualNocase(month, "de",  2)) ts.tm_mon = 11;
	ts.tm_sec = atoi(sec.c_str());
	ts.tm_wday = 0;
	     if (areEqualNocase(weekday, "mo", 2)) ts.tm_wday = 1;
	else if (areEqualNocase(weekday, "tu", 2)) ts.tm_wday = 2;
	else if (areEqualNocase(weekday, "we", 2)) ts.tm_wday = 3;
	else if (areEqualNocase(weekday, "th", 2)) ts.tm_wday = 4;
	else if (areEqualNocase(weekday, "fr", 2)) ts.tm_wday = 5;
	else if (areEqualNocase(weekday, "sa", 2)) ts.tm_wday = 6;
	ts.tm_yday = 0;
	ts.tm_year = atoi(year.c_str());
	if (ts.tm_year < 100) ts.tm_year += 2000;
	if (ts.tm_year > 1900) ts.tm_year -= 1900;

	// Now generate a time_t value

	time_t	thisTime = mktime(&ts);
	thisTime += theOffset;

	// Convert it to a string

	struct	tm *wee = localtime(&thisTime);
	if (!wee)
	{
		str = "Non-standard time format!";
		return;
	}

	bool	pm = false;
	int	h = wee->tm_hour;
	if (h >= 12) pm = true;
	if (h > 12) h -= 12;
	if (h == 0) h = 12;
	month = "January";
	switch(wee->tm_mon)
	{
		case  1: month="February"; break;
		case  2: month="March"; break;
		case  3: month="April"; break;
		case  4: month="May"; break;
		case  5: month="June"; break;
		case  6: month="July"; break;
		case  7: month="August"; break;
		case  8: month="September"; break;
		case  9: month="October"; break;
		case 10: month="November"; break;
		case 11: month="December"; break;
	}
	char	tmp[256];
	sprintf(tmp, "%s %d, %d @ %d:%02d%cm", month.c_str(), wee->tm_mday, wee->tm_year + 1900, h, wee->tm_min, pm ? 'p':'a');
	str = tmp;
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	convertToUnicode(const string & str, const string & charset)
{
	// Convert the code page string to a value...

	typedef	struct	tag_codepages
	{
		const char *	name;
		unsigned int	cp;
	} CPINFO;

	CPINFO	cpinfo[] =
	{
		//{"ansi_x3.4-1968", 1252}, // Western 
		//{"ansi_x3.4-1986", 1252}, // Western 
		//{"ascii", 1252}, // Western 
		{"big5", 950}, // Traditional Chinese (BIG5) 
		{"chinese", 936}, // Chinese Simplified 
		//{"cp367", 1252}, // Western 
		//{"cp819", 1252}, // Western 
		//{"csascii", 1252}, // Western 
		{"csbig5", 950}, // Traditional Chinese (BIG5) 
		{"cseuckr", 949}, // Korean 
		//{"cseucpkdfmtjapanese", CODE_JPN_EUC}, // Japanese (EUC) 
		{"csgb2312", 936}, // Chinese Simplified (GB2312) 
		//{"csiso2022jp", CODE_JPN_JIS}, // Japanese (JIS-Allow 1 byte Kana) 
		{"csiso2022kr", 50225}, // Korean (ISO) 
		{"csiso58gb231280", 936}, // Chinese Simplified (GB2312) 
		{"csisolatin2", 28592}, // Central European (ISO) 
		{"csisolatinhebrew", 1255}, // Hebrew (ISO-Visual) 
		{"cskoi8r", 20866}, // Cyrillic (KOI8-R) 
		{"csksc56011987", 949}, // Korean 
		{"csshiftjis", 932}, // Shift-JIS 
		{"euc-kr", 949}, // Korean 
		//{"extended_unix_code_packed_format_for_japanese", CODE_JPN_EUC}, // Japanese (EUC) 
		{"gb2312", 936}, // Chinese Simplified (GB2312) 
		{"gb_2312-80", 936}, // Chinese Simplified (GB2312) 
		{"hebrew", 1255}, // Hebrew  
		{"hz-gb-2312", 936}, // Chinese Simplified (HZ) 
		//{"ibm367", 1252}, // Western 
		//{"ibm819", 1252}, // Western 
		{"ibm852", 852}, // Central European (DOS) 
		{"ibm866", 866}, // Cyrillic (DOS) 
		//{"iso-2022-jp", CODE_JPN_JIS}, // Japanese (JIS) 
		{"iso-2022-kr", 50225}, // Korean (ISO) 
		//{"iso-8859-1", 1252}, // Western 
		{"iso-8859-2", 28592}, // Central European (ISO) 
		{"iso-8859-8", 1255}, // Hebrew (ISO-Visual) 
		//{"iso-ir-100", 1252}, // Western 
		{"iso-ir-101", 28592}, // Central European (ISO) 
		{"iso-ir-138", 1255}, // Hebrew (ISO-Visual) 
		{"iso-ir-149", 949}, // Korean 
		{"iso-ir-58", 936}, // Chinese Simplified (GB2312) 
		//{"iso-ir-6", 1252}, // Western 
		//{"iso646-us", 1252}, // Western 
		//{"iso8859-1", 1252}, // Western 
		{"iso8859-2", 28592}, // Central European (ISO) 
		//{"iso_646.irv:1991", 1252}, // Western 
		//{"iso_8859-1", 1252}, // Western 
		//{"iso_8859-1:1987", 1252}, // Western 
		{"iso_8859-2", 28592}, // Central European (ISO) 
		{"iso_8859-2:1987", 28592}, // Central European (ISO) 
		{"iso_8859-8", 1255}, // Hebrew (ISO-Visual) 
		{"iso_8859-8:1988", 1255}, // Hebrew (ISO-Visual) 
		{"koi8-r", 20866}, // Cyrillic (KOI8-R) 
		{"korean", 949}, // Korean 
		{"ks-c-5601", 949}, // Korean 
		{"ks-c-5601-1987", 949}, // Korean 
		{"ks_c_5601", 949}, // Korean 
		{"ks_c_5601-1987", 949}, // Korean 
		{"ks_c_5601-1989", 949}, // Korean 
		{"ksc-5601", 949}, // Korean 
		{"ksc5601", 949}, // Korean 
		{"ksc_5601", 949}, // Korean 
		{"l2", 28592}, // Central European (ISO) 
		//{"latin1", 1252}, // Western 
		{"latin2", 28592}, // Central European (ISO) 
		{"ms_kanji", 932}, // Shift-JIS 
		{"shift-jis", 932}, // Shift-JIS 
		{"shift_jis", 932}, // Shift-JIS 
		//{"us", 1252}, // Western 
		//{"us-ascii", 1252}, // Western 
		{"windows-1250", 1250}, // Central European (Windows) 
		{"windows-1251", 1251}, // Cyrillic (Windows) 
		//{"windows-1252", 1252}, // Western 
		{"windows-1253", 1253}, // Greek (Windows) 
		{"windows-1254", 1254}, // Turkish (Windows) 
		{"windows-1255", 1255}, // Hebrew 
		{"windows-1256", 1256}, // Arabic 
		{"windows-1257", 1257}, // Baltic (Windows) 
		{"windows-1258", 1258}, // Vietnamese 
		{"windows-874", 874}, // Thai 
		{"x-cp1250", 1250}, // Central European (Windows) 
		{"x-cp1251", 1251}, // Cyrillic (Windows) 
		//{"x-euc", CODE_JPN_EUC}, // Japanese (EUC) 
		//{"x-euc-jp", CODE_JPN_EUC}, // Japanese (EUC) 
		{"x-sjis", 932}, // Shift-JIS 
		{"x-x-big5", 950}, // Traditional Chinese (BIG5) 
		{"", 1}, // terminator
	};

	// Start with the default code page

	int		cpvalue = _getmbcp();

	// If there is a specified charset, translate it to a codepage identifier

	if (charset.length())
	{
		string		cset = charset;
		lowerString(cset);

		// Locate the codepage value for the given name

		CPINFO *	ptr = cpinfo;
		for(;;)
		{
			if (cset == string(ptr->name))
			{
				cpvalue = ptr->cp;
				break;
			}
			if (!strlen(ptr->name)) break;
			++ptr;
		}
	}

	// Our wide-char buffer

	const	int	wBufsize = 8096;
	static	wchar_t	wbuffer[wBufsize];
	memset(wbuffer, 0, sizeof(wbuffer));
	wstring	result;

	// Convert it

	int	ret = MultiByteToWideChar(cpvalue, MB_PRECOMPOSED|MB_ERR_INVALID_CHARS, str.c_str(), str.length(), wbuffer, wBufsize);
	if (!ret)
	{
		result = atow(str);
	}
	else
	{
		result = wbuffer;
	}

	return result;
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	decodeString(const string & input, const string & defaultCharset)
{
	// Working copy

	string	str = input;
	wstring	result;

	// Find an rfc1522 boundary

	for(;;)
	{
		// Find the start & end

		string::size_type	start = str.find("=?");
		if (start == string::npos) break;

		// Skipping some non-encoded text?

		if (start > 0) result += convertToUnicode(str.substr(0, start), defaultCharset);

		str.erase(0, start+2);

		string::size_type	end = str.find("?");
		if (end == string::npos) break;
		if (str[end+2] != '?') break;
		end = str.find("?=", end + 3);
		if (end == string::npos) break;

		// Get the string to decode

		string	decStr = str.substr(0, end);
		str.erase(0, end+2);

		// Make sure there are two questionmarks in the decString, with a single character between them

		string::size_type	idx = decStr.find("?");
		string	charset = decStr.substr(0, idx);
		if (idx == string::npos) break;
		if (decStr[idx+2] != '?') break;

		// Base64 encoding

		if (tolower(decStr[idx+1]) == 'b')
		{
			// Strip out everything except the string to be decoded...

			decStr.erase(0, idx+3);

			// Decode it

			unsigned int	size;
			StringList	sl;
			sl.push_back(decStr);
			unsigned char *	data = decodeBase64(sl, size);
			decStr = (const char *) data;
			delete[] data;

			// Replace it

			result += convertToUnicode(decStr, charset);
		}

		// Quoted-printable encoding

		else if (tolower(decStr[idx+1]) == 'q')
		{
			// Strip out everything except the string to be decoded...

			decStr.erase(0, idx+3);

			// Decode it

			StringList	sl;
			sl.push_back(decStr);
			decStr = *decodeQuotedPrintable(sl).begin();

			// Replace it

			result += convertToUnicode(decStr, charset);
		}
		else
		{
			result += convertToUnicode(decStr, defaultCharset);
		}
	}

	// If it wasn't encoded, then convert it using the default charset

	if (str.length()) result += convertToUnicode(str, defaultCharset);

	// Done

	return result;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	escapeString(wstring & str)
{
	wstring::size_type	idx = 0;

	while(idx != wstring::npos)
	{
		idx = str.find(_T("\\"), idx);
		if (idx != wstring::npos) str.erase(idx, 1);
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

	Mime::Mime()
	: _hasAttachment(false), _plainText(false), _htmlText(false), _validEmail(true), _spam(false)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

	Mime::~Mime()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	Mime::parse(const StringList &_header, unsigned int &embeddedSkip)
{
	// Defaults

	string	tReceived;
	string	tDate = "[Unknown]";
	string	tFrom = "[Unknown]";
	string	tTo = "[Unknown]";
	string	tSubj = "[No subject specified]";
	string	tReplyTo = "";
	string	tContentType = "";
	string	tContentDisposition = "";
	string	tContentTransferEncoding = "";

	// Copy it

	header() = _header;
	embeddedSkip = 0;

	// Parse the header into individual fields

	string		*lastString = NULL;
	bool		embeddedMessage = false;
	unsigned int	line = 0;
	bool		spamFlag = false;
	for (StringList::iterator i = header().begin(); i != header().end(); ++i, ++line)
	{
		// We've finished a "Recieved" line... deal with it

		if (!spamFlag && tReceived.length() && i->length() && (*i)[0] != ' ' && (*i)[0] != '\t')
		{
			string	body = tReceived;
			tReceived = "";
			spamFlag = isSpam(body);
		}

		if (!i->length())
		{
			if (embeddedMessage)
			{
				lastString = NULL;
				embeddedMessage = false;
				embeddedSkip = line + 1;
			}
			else
			{
				//header().erase(i);
				break;
			}
		}
		else if (isspace((*i)[0]) && lastString)
		{
			string	temp = *i;
			stripLeadingWS(temp);
			*lastString += " " + temp;
		}
		else if (areEqualNocase(*i, "received:", 9))
		{
			tReceived = i->substr(9);
			stripLeadingWS(tReceived);
			lastString = &tReceived;
		}
		else if (areEqualNocase(*i, "date:", 5))
		{
			tDate = i->substr(5);
			stripLeadingWS(tDate);
			lastString = &tDate;
		}
		else if (areEqualNocase(*i, "from:", 5))
		{
			tFrom = i->substr(5);
			stripLeadingWS(tFrom);
			lastString = &tFrom;
		}
		else if (areEqualNocase(*i, "reply-to:", 9))
		{
			tReplyTo = i->substr(9);
			stripLeadingWS(tReplyTo);
			lastString = &tReplyTo;
		}
		else if (areEqualNocase(*i, "to:", 3))
		{
			tTo = i->substr(3);
			stripLeadingWS(tTo);
			lastString = &tTo;
		}
		else if (areEqualNocase(*i, "subject:", 8))
		{
			tSubj = i->substr(8);
			stripLeadingWS(tSubj);
			lastString = &tSubj;
		}
		else if (areEqualNocase(*i, "content-type:", 13))
		{
			// Is this an embedded message?

			if (findNocase(*i, "message/") != string::npos)
			{
				embeddedMessage = true;
			}
			else
			{
				tContentType = i->substr(13);
				stripLeadingWS(tContentType);
				lastString = &tContentType;
			}
		}
		else if (areEqualNocase(*i, "content-transfer-encoding:", 26))
		{
			tContentTransferEncoding = i->substr(26);
			stripLeadingWS(tContentTransferEncoding);
			lastString = &tContentTransferEncoding;
		}
		else if (areEqualNocase(*i, "content-disposition:", 20))
		{
			tContentDisposition = i->substr(20);
			stripLeadingWS(tContentDisposition);
			lastString = &tContentDisposition;
		}
		else
		{
			lastString = NULL;
		}
	}

	// Locate the charset, or use the default

	string::size_type	idx = findNocase(tContentType, "charset=");
	string			charset = "";
	if (idx != string::npos)
	{
		string	temp = tContentType.substr(idx+8);
		idx = temp.find_first_of(" \f\t\v\r\n");
		if (idx != string::npos) temp.erase(idx);
		if (temp.length()) charset = temp;
		for(;;) {idx = charset.find("\""); if (idx == string::npos) break; charset.erase(idx,1);}
	}

	// Decode the strings, using any charset specified within the possibly encoded string, and if not, use 'charset'

	date()                    = decodeString(tDate, charset);
	from()                    = decodeString(tFrom, charset);
	to()                      = decodeString(tTo, charset);
	subject()                 = decodeString(tSubj, charset);
	replyTo()                 = decodeString(tReplyTo, charset);
	contentType()             = decodeString(tContentType, charset);
	contentDisposition()      = decodeString(tContentDisposition, charset);
	contentTransferEncoding() = decodeString(tContentTransferEncoding, charset);

	// Escape the strings

	escapeString(date());
	escapeString(from());
	escapeString(to());
	escapeString(subject());
	escapeString(replyTo());

	// Mark it as spam?

	if (spamFlag) spam() = true;

	// Set the flags and other variables

	if (findNocase(contentType(), _T("multipart")) != wstring::npos) hasAttachment() = true;
	if (findNocase(contentType(), _T("text/plain")) != wstring::npos) plainText() = true;
	if (findNocase(contentType(), _T("text/html")) != wstring::npos) htmlText() = true;

	idx = findNocase(contentType(), _T("boundary="));
	if (idx != wstring::npos)
	{
		idx += 9;
		if (contentType()[idx] == _T('\"')) idx++;
		hasAttachment() = true;
		contentBoundary() = contentType().substr(idx);
		idx = contentBoundary().find(_T("\""));
		if (idx != wstring::npos) contentBoundary().erase(idx);
	}

	idx = findNocase(contentType(), _T("name="));
	if (idx != wstring::npos)
	{
		contentName() = contentType().substr(idx + 5);
		while(contentName().length())
		{
			idx = contentName().find(_T("\""));
			if (idx != wstring::npos) contentName().erase(idx, 1);
			else break;
		}
	}

	idx = findNocase(contentDisposition(),_T("filename="));
	if (idx != wstring::npos)
	{
		contentFilename() = contentDisposition().substr(idx + 9);
		while(contentFilename().length())
		{
			idx = contentFilename().find(_T("\""));
			if (idx != wstring::npos) contentFilename().erase(idx, 1);
			else break;
		}
	}

	// Fill in the blanks, if necessary

	if (!contentFilename().length() && contentName().length()) contentFilename() = contentName();
	if (contentFilename().length() && !contentName().length()) contentName() = contentFilename();

	// Content-type

	// Commented out because binary attachments were showing up as preview text
	//if (!plainText() && !htmlText()) plainText() = true;

//	if (date().length()) parseDate(date());

	// Is this a valid email?

	bool	validEmail1 = true, validEmail2 = true;
	if (findNocase(from(), _T("MAILER-DAEMON")) != wstring::npos) validEmail1 = false;
	if (findNocase(subject(), _T("DON'T DELETE THIS MESSAGE -- FOLDER INTERNAL DATA")) != wstring::npos) validEmail2 = false;

	if (!validEmail1 && !validEmail2) validEmail() = false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	Mime::addPart(const Mime &m, StringList &data)
{
	// Strip the mime header from the currentBoundaryData

	while(data.size() && data.begin()->length())
	{
		data.erase(data.begin());
	}

	if (!data.begin()->length())
	{
		data.erase(data.begin());
	}

	// Don't bother adding empty parts

	bool	empty = true;
	for (StringList::iterator i = data.begin(); i != data.end(); ++i)
	{
		string	temp = *i;
		stripLeadingWS(temp);
		if (temp.length())
		{
			empty = false;
			break;
		}
	}

	if (empty) return;

	// We'll put our part here

	Parts		p;

	// If it's encoded, don't display it

	if (areEqualNocase(m.contentTransferEncoding(), _T("base64")))
	{
		p.binaryData = decodeBase64(data, p.binaryDataSize);

		// Some embedded messages are also encoded, is this one?

		if (m.plainText() || m.htmlText())
		{
			string		temp = (char *) p.binaryData;
			convertToStringList(temp, p.data);
		}
		else
		{
			StringList	result;
			string		temp("This is an attachment which cannot be displayed.\n\nInstead, click the 'Save Attachment' button.");
			result.push_back(temp);
			p.data = result;
		}
	}
	else if (areEqualNocase(m.contentTransferEncoding(), _T("quoted-printable")))
	{
		p.data = decodeQuotedPrintable(data);
	}
	else
	{
		p.data = data;
	}

	p.description = m.contentType();
	wstring::size_type idx = p.description.find(_T(";"));
	if (idx != string::npos) p.description.erase(idx);
	if (m.contentName().length()) p.description = m.contentName();
	p.fileName = m.contentFilename();
	p.isHtml = m.htmlText();
	parts().push_back(p);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	Mime::parseParts(const string &body)
{
	recursiveParseParts(body);

	// Fail safe!

	if (parts().size() == 0)
	{
		StringList	data;
		convertToStringList(body, data);
		addPart(*this, data);
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	Mime::recursiveParseParts(const string &body)
{
	// Convert the body to a string list

	StringList	data;
	convertToStringList(body, data);

	// If there aren't any attachments, then the default part is the data for this entire email

	parts().clear();
	if (!hasAttachment())
	{
		addPart(*this, data);
		return;
	}

	// Scan for parts

	unsigned int	index = 0;
	bool		inBoundary = false;
	StringList	currentBoundaryData;

	for (StringList::iterator i = data.begin(); i != data.end(); ++i, ++index)
	{
		Mime	m;
		m.contentType() = contentType(); // Default content type so the preview's drop-down isn't empty

		bool	foundBoundaryStart = false;
		bool	foundBoundaryNext = false;
		bool	foundBoundaryEnd = false;

		string	asciiContentBoundary = "--" + wtoa(contentBoundary());

		if (*i == asciiContentBoundary)
		{
			if (!inBoundary)	foundBoundaryStart = true;
			else			foundBoundaryNext = true;
		}
		else if (inBoundary && *i == asciiContentBoundary + "--")
		{
			foundBoundaryEnd = true;
		}

		// Are we starting a boundary?

		if (foundBoundaryStart)
		{
			inBoundary = true;
			currentBoundaryData.clear();
		}

		// Are we ending a boundary?

		else if (foundBoundaryEnd || foundBoundaryNext)
		{
			if (foundBoundaryEnd) inBoundary = false;

			unsigned int	embeddedSkip = 0;
			m.parse(currentBoundaryData, embeddedSkip);

			// Skip to the last embedded header?

			if (embeddedSkip)
			{
				// Copy just the embedded data

				StringList	temp;
				for (StringList::iterator j = currentBoundaryData.begin(); j != currentBoundaryData.end(); ++j)
				{
					if (!embeddedSkip)
					{
						temp.push_back(*j);
					}
					else
					{
						embeddedSkip--;
					}
				}

				currentBoundaryData = temp;
			}

			if (m.hasAttachment())
			{
				m.recursiveParseParts(convertToString(currentBoundaryData));

				for (vector<Parts>::iterator j = m.parts().begin(); j != m.parts().end(); ++j)
				{
					parts().push_back(*j);
				}
			}
			else
			{
				addPart(m, currentBoundaryData);
			}

			currentBoundaryData.clear();
		}

		// If we're in a boundary, tally up the data

		else if (inBoundary)
		{
			currentBoundaryData.push_back(*i);
		}
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------
// Mime.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
