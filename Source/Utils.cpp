// ---------------------------------------------------------------------------------------------------------------------------------
//  _    _ _   _ _                          
// | |  | | | (_) |                         
// | |  | | |_ _| |___      ___ _ __  _ __  
// | |  | | __| | / __|    / __| '_ \| '_ \ 
// | |__| | |_| | \__ \ _ | (__| |_) | |_) |
//  \____/ \__|_|_|___/(_) \___| .__/| .__/ 
//                             | |   | |    
//                             |_|   |_|    
//
// 3D Mail Effects (Utilitarian functions)
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 12/05/2000 by Paul Nettle
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
#include "Utils.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

#if	defined(_REGISTERED_)
const	unsigned int	userIDIndexList[32] = {3,13,14,15,16,17,28,30,47,49,52,53,62,63,64,65,66,73,74,75,76,82,83,88,89,90,97,98,99,100,105,106};
	unsigned char	oneHundredKeys[100*16];
#endif // _REGISTERED_

// ---------------------------------------------------------------------------------------------------------------------------------

#include "fubar_off.h"
string	integerToString(const int val)
{
static	char	temp[20];
	sprintf(temp, "%d", val);
	return temp;
}

wstring	integerToWString(const int val)
{
static	wchar_t	temp[20];
	_snwprintf(temp, 20, _T("%d"), val);
	return temp;
}

string	floatToString(const float val)
{
static	char	temp[20];
	sprintf(temp, "%f", val);
	return temp;
}

wstring	floatToWString(const float val)
{
static	wchar_t	temp[20];
	_snwprintf(temp, 20, _T("%f"), val);
	return temp;
}
#include "fubar_on.h"

// --------------------------------------------------------------------------------------------------------------------------------

string	stripCommas(const string &s)
{
	string	result = s;
	for(;;)
	{
		string::size_type	idx = result.find(",");
		if (idx == string::npos) break;
		result[idx] = '.';
	}
	return result;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	stripLeadingWS(string &s)
{
	int	start = 0;
	int	end = s.length();
	while(start < end && s[start] >= 0 && isspace(s[start])) ++start;
	if (start) s.erase(0, start);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	stripTrailingWS(string &s)
{
	int	end = s.length() - 1;
	while(end >= 0 && isspace(s[end])) --end;
	if (end < (int) s.length()-1) s.erase(end + 1);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	stripLeadingWS(wstring &s)
{
	int	start = 0;
	int	end = s.length();
	while(start < end && s[start] >= 0 && iswspace(s[start])) ++start;
	if (start) s.erase(0, start);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	stripTrailingWS(wstring &s)
{
	int	end = s.length() - 1;
	while(end >= 0 && iswspace(s[end])) --end;
	if (end < (int) s.length()-1) s.erase(end + 1);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	stripCarriageReturns(string &s)
{
	// Destination string

	string	result;
	result.reserve(s.length());

	string::size_type	idx = 0;
	while(idx < s.length())
	{
		string::size_type	cr = s.find("\r", idx);
		if (cr == string::npos)
		{
			result += s.substr(idx);
			break;
		}

		result += s.substr(idx, cr-idx);
		idx = cr + 1;
	}

	s = result;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	stripCarriageReturns(wstring &s)
{
	// Destination string

	wstring	result;
	result.reserve(s.length());

	wstring::size_type	idx = 0;
	while(idx < s.length())
	{
		wstring::size_type	cr = s.find(_T("\r"), idx);
		if (cr == wstring::npos)
		{
			result += s.substr(idx);
			break;
		}

		result += s.substr(idx, cr-idx);
		idx = cr + 1;
	}

	s = result;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	lowerString(string &s)
{
	for (unsigned int i = 0; i < s.length(); i++) s[i] = tolower(s[i]);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	lowerString(wstring &s)
{
	for (unsigned int i = 0; i < s.length(); i++) s[i] = towlower(s[i]);
}

// ---------------------------------------------------------------------------------------------------------------------------------

string::size_type	findNocase(const string &s1, const string &s2, const string::size_type index)
{
	string	_s1 = s1;
	lowerString(_s1);

	string	_s2 = s2;
	lowerString(_s2);

	if (index != 0xffffffff)	return _s1.find(_s2, index);
	else				return _s1.find(_s2);
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring::size_type	findNocase(const wstring &s1, const wstring &s2, const wstring::size_type index)
{
	wstring	_s1 = s1;
	lowerString(_s1);

	wstring	_s2 = s2;
	lowerString(_s2);

	if (index != 0xffffffff)	return _s1.find(_s2, index);
	else				return _s1.find(_s2);
}

// ---------------------------------------------------------------------------------------------------------------------------------

string::size_type	rfindNocase(const string &s1, const string &s2, const string::size_type index)
{
	string	_s1 = s1;
	lowerString(_s1);

	string	_s2 = s2;
	lowerString(_s2);

	if (index != 0xffffffff)	return _s1.rfind(_s2, index);
	else				return _s1.rfind(_s2);
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring::size_type	rfindNocase(const wstring &s1, const wstring &s2, const wstring::size_type index)
{
	wstring	_s1 = s1;
	lowerString(_s1);

	wstring	_s2 = s2;
	lowerString(_s2);

	if (index != 0xffffffff)	return _s1.rfind(_s2, index);
	else				return _s1.rfind(_s2);
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	areEqualNocase(const string &s1, const string &s2)
{
	if (s1.length() != s2.length()) return false;
	if (s1 == s2) return true;

	for (unsigned int i = 0; i < s1.length(); i++)
	{
		if (toupper(s1[i]) != toupper(s2[i])) return false;
	}

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	areEqualNocase(const string &s1, const string &s2, const unsigned int len)
{
//	if (s1.length() < len || s2.length() < len) return false;
//	if (s1.substr(0, len) == s2.substr(0, len)) return true;

	for (unsigned int i = 0; i < len; i++)
	{
		if (toupper(s1[i]) != toupper(s2[i])) return false;
	}

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	areEqualNocase(const wstring &s1, const wstring &s2)
{
	if (s1.length() != s2.length()) return false;
	if (s1 == s2) return true;

	for (unsigned int i = 0; i < s1.length(); i++)
	{
		if (towupper(s1[i]) != towupper(s2[i])) return false;
	}

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	areEqualNocase(const wstring &s1, const wstring &s2, const unsigned int len)
{
	if (s1.length() < len || s2.length() < len) return false;
	if (s1.substr(0, len) == s2.substr(0, len)) return true;

	for (unsigned int i = 0; i < len; i++)
	{
		if (towupper(s1[i]) != towupper(s2[i])) return false;
	}

	return true;
}

// --------------------------------------------------------------------------------------------------------------------------------

const char * encrypt(const char *input, unsigned int & len)
{
	static	char	result[512];
	unsigned int	resultLength = 0;
	char *		ptr = result;

	memset(result, 0, sizeof(result));

	for (unsigned int i = 0; i < len; ++i)
	{
		*(ptr++) = (rand() & 0xff) | 0x80;
		*(ptr++) = input[i] | 0x80;
		resultLength += 2;
	}

	len = resultLength;
	return result;
}

// --------------------------------------------------------------------------------------------------------------------------------

const char * decrypt(const char *input, unsigned int & len)
{
	static	char	result[512];
	unsigned int	resultLength = 0;
	char *		ptr = result;

	memset(result, 0, sizeof(result));

	for (unsigned int i = 0; i < len; ++i)
	{
		if (i & 1) *(ptr++) = input[i] & 0x7f;
		resultLength += 1;
	}

	len = resultLength;
	return result;
}


// ---------------------------------------------------------------------------------------------------------------------------------

void	allowBackgroundProcessing()
{
	for (int i = 0; i < 10; i++)
	{
		MSG	msg;

		if (!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) break;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

string	getCommaEntry(const string &str, unsigned int index)
{
	string	s = str;

	int	ind = index;
	while(s.length() && ind--)
	{
		string::size_type	idx = s.find(",");
		if (idx != string::npos) s.erase(0, idx+1);
	}

	string::size_type	idx = s.find(",");
	if (idx != string::npos) s.erase(idx);

	stripLeadingWS(s);
	stripTrailingWS(s);

	return s;
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	getCommaEntry(const wstring &str, unsigned int index)
{
	wstring	s = str;

	int	ind = index;
	while(s.length() && ind--)
	{
		wstring::size_type	idx = s.find(_T(","));
		if (idx != string::npos) s.erase(0, idx+1);
	}

	wstring::size_type	idx = s.find(_T(","));
	if (idx != wstring::npos) s.erase(idx);

	stripLeadingWS(s);
	stripTrailingWS(s);

	return s;
}

// ---------------------------------------------------------------------------------------------------------------------------------

int	getCommaEntryInt(const string &str, unsigned int index)
{
	return atoi(getCommaEntry(str, index).c_str());
}

// ---------------------------------------------------------------------------------------------------------------------------------

int	getCommaEntryInt(const wstring &str, unsigned int index)
{
	return _wtoi(getCommaEntry(str, index).c_str());
}

// ---------------------------------------------------------------------------------------------------------------------------------

unsigned int	parseCommaColors(const string &str)
{
	unsigned int	r = (unsigned int) getCommaEntryInt(str, 0);
	unsigned int	g = (unsigned int) getCommaEntryInt(str, 1);
	unsigned int	b = (unsigned int) getCommaEntryInt(str, 2);
	return (r << 16) | (g << 8) | b;
}

// ---------------------------------------------------------------------------------------------------------------------------------

unsigned int	parseCommaColors(const wstring &str)
{
	unsigned int	r = (unsigned int) getCommaEntryInt(str, 0);
	unsigned int	g = (unsigned int) getCommaEntryInt(str, 1);
	unsigned int	b = (unsigned int) getCommaEntryInt(str, 2);
	return (r << 16) | (g << 8) | b;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	convertToStringList(const string &s, StringList & lines)
{
	lines.clear();

	string::size_type	idx = 0;
	unsigned int		length = s.length();

	while(idx < length)
	{
		// We push a blank string onto the list for speed

		string	temp;
		lines.push_back(temp);

		string::size_type	ddx = s.find("\n", idx);
		if (ddx == string::npos)
		{
			lines.back() = &s[idx];
			idx = length;
		}
		else
		{
			char *	ptr = (char *) &s[ddx];
			if (*(ptr-1) == '\r') ptr--;

			char	sav = *ptr;
			*ptr = 0;
			lines.back() = &s[idx];
			*ptr = sav;

			idx = ddx + 1;
		}

		//stripCarriageReturns(lines.back());
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	convertToStringList(const wstring &s, WStringList & lines)
{
	lines.clear();

	wstring::size_type	idx = 0;
	unsigned int		length = s.length();

	while(idx < length)
	{
		// We push a blank string onto the list for speed

		wstring	temp;
		lines.push_back(temp);

		wstring::size_type	ddx = s.find(_T("\n"), idx);
		if (ddx == wstring::npos)
		{
			lines.back() = &s[idx];

			idx = length;
		}
		else
		{
			*(wchar_t *)(&s[ddx]) = 0;
			lines.back() = &s[idx];
			*(wchar_t *)(&s[ddx]) = _T('\n');

			idx = ddx + 1;
		}

		stripCarriageReturns(lines.back());
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

string	convertToString(const StringList &s)
{
	// Blank list?

	if (!s.size()) return "";

	// Collect the entire size of this preview

	unsigned int	totalSize = 0;
	{
		for (StringList::const_iterator i = s.begin(); i != s.end(); ++i)
		{
			totalSize += i->length() + 2; // 2 = space for "\r\n"
		}
	}

	// Allocate the space for this preview

	char	*buffer = new char[totalSize + 1]; // 1 = terminating null
	char	*ptr = buffer;
	if (!buffer) throw string("Out of memory");
	memset(buffer, 0, totalSize + 1);

	for (StringList::const_iterator i = s.begin(); i != s.end(); ++i)
	{
		strcpy(ptr, i->c_str());
		ptr += i->length();
		*(ptr++) = '\r';
		*(ptr++) = '\n';
		ASSERT(ptr <= buffer + totalSize);
	}

	// Copy it to the preview

	string	result = buffer;
	delete[] buffer;
	return result;
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	convertToString(const WStringList &s)
{
	// Blank list?

	if (!s.size()) return _T("");

	// Collect the entire size of this preview

	unsigned int	totalSize = 0;
	{
		for (WStringList::const_iterator i = s.begin(); i != s.end(); ++i)
		{
			totalSize += i->length() + 2; // 2 = space for "\r\n"
		}
	}

	// Allocate the space for this preview

	wchar_t	*buffer = new wchar_t[totalSize + 1]; // 1 = terminating null
	wchar_t	*ptr = buffer;
	if (!buffer) throw string("Out of memory");
	memset(buffer, 0, totalSize + 1);

	for (WStringList::const_iterator i = s.begin(); i != s.end(); ++i)
	{
		wcscpy(ptr, i->c_str());
		ptr += i->length();
		*(ptr++) = _T('\r');
		*(ptr++) = _T('\n');
		ASSERT(ptr <= buffer + totalSize);
	}

	// Copy it to the preview

	wstring	result = buffer;
	delete[] buffer;
	return result;
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	getTempDir()
{
	wstring	path = theApp.programPath() + _T("Temp");

	// Make the Temp dir if it doesn't exist...

	if (_waccess(path.c_str(), 0))
	{
		if (_wmkdir(path.c_str()))
		{
			return path;
		}
	}

	return path;
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	getTempFilename()
{
	wstring	path = getTempDir();

	wchar_t	tmpTemplate[1024];
	wcscpy(tmpTemplate, path.c_str());
	wcscat(tmpTemplate, _T("\\_XXXXX"));
	_wmktemp(tmpTemplate);

	return tmpTemplate;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	clearTempFiles()
{
	wstring	path = getTempDir();

	_wfinddata_t	findInfo;
	wstring		filespec = path + _T("\\*.*");
	long		handle = _wfindfirst((wchar_t *) filespec.c_str(), &findInfo);
	if (handle == -1 || handle == ENOENT || handle == EINVAL) return;

	do
	{
		if (wcscmp(findInfo.name, _T(".")) && wcscmp(findInfo.name, _T("..")))
		{
			wstring	filename = path + _T("\\") + findInfo.name;
			_wunlink(filename.c_str());
		}
	} while(!_wfindnext(handle, &findInfo));

	_findclose(handle);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	recursiveFindFiles(const wstring & path, const wstring & filespec, WStringList &fileList)
{
	_wfinddata_t	findInfo;
	wstring		correctPath = path;
	if (path[path.length() - 1] != _T('\\')) correctPath += _T("\\");

	wstring		spec = correctPath + _T("*.*");
	long		handle = _wfindfirst((wchar_t *) spec.c_str(), &findInfo);
	if (handle == -1 || handle == ENOENT || handle == EINVAL) return;

	do
	{
		wstring	fullName = correctPath + findInfo.name;

		// Stat the file

		struct _stat statbuf;
		if (!_wstat(fullName.c_str(), &statbuf))
		{
			// If it's a dir, keep looking

			wstring	thisFile = findInfo.name;

			if (statbuf.st_mode & _S_IFDIR)
			{
				if (thisFile != _T(".") && thisFile != _T(".."))
				{
					recursiveFindFiles(fullName + _T("\\"), filespec, fileList);
				}
			}

			// Regular file

			else
			{
				// Is it a file we're lookin' for?

				wstring::size_type	idx = findNocase(thisFile, filespec, 0);
				if (idx != wstring::npos && idx == thisFile.length() - filespec.length())
				{
					fileList.push_back(correctPath + thisFile);
				}
			}
		}
	} while(!_wfindnext(handle, &findInfo));

	_findclose(handle);
}

// ---------------------------------------------------------------------------------------------------------------------------------

string	sizeString(const unsigned int bytes)
{
	char	str[90];

	// Gigabytes?

	if (bytes > 1024*1024*1024)
	{
		sprintf(str, "%.2fGB", (float) bytes / (float) (1024*1024*1024));
	}

	// Megabytes?

	else if (bytes > 1024*1024)
	{
		sprintf(str, "%.2fMB", (float) bytes / (float) (1024*1024));
	}

	// Kilobytes?

	else if (bytes > 1024)
	{
		sprintf(str, "%.2fK", (float) bytes / (float) 1024);
	}

	// Bytes

	else
	{
		sprintf(str, "%d bytes", bytes);
	}

	return str;
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	wsizeString(const unsigned int bytes)
{
	wchar_t	str[90];

	// Gigabytes?

	if (bytes > 1024*1024*1024)
	{
		_snwprintf(str, 90, _T("%.2fGB"), (float) bytes / (float) (1024*1024*1024));
	}

	// Megabytes?

	else if (bytes > 1024*1024)
	{
		_snwprintf(str, 90, _T("%.2fMB"), (float) bytes / (float) (1024*1024));
	}

	// Kilobytes?

	else if (bytes > 1024)
	{
		_snwprintf(str, 90, _T("%.2fK"), (float) bytes / (float) 1024);
	}

	// Bytes

	else
	{
		_snwprintf(str, 90, _T("%d bytes"), bytes);
	}

	return str;
}

// ---------------------------------------------------------------------------------------------------------------------------------

unsigned int	getCurrentPatchLevel()
{
#if	defined(_REGISTERED_)
	#if	defined(_BETA_)
		return theApp.prefs().other()[_T("currentBetaRegisteredPatchLevel")].integerValue();
	#else
		return theApp.prefs().other()[_T("currentRegisteredPatchLevel")].integerValue();
	#endif // _BETA_
#else
	#if	defined(_BETA_)
		return theApp.prefs().other()[_T("currentBetaSharewarePatchLevel")].integerValue();
	#else
		return theApp.prefs().other()[_T("currentSharewarePatchLevel")].integerValue();
	#endif // _BETA_
#endif // _REGISTERED_
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	setCurrentPatchLevel(const unsigned int level)
{
#if	defined(_REGISTERED_)
	#if	defined(_BETA_)
		theApp.prefs().other()[_T("currentSharewarePatchLevel")].integerValue() = level;
	#else
		theApp.prefs().other()[_T("currentRegisteredPatchLevel")].integerValue() = level;
	#endif // _BETA_
#else
	#if	defined(_BETA_)
		theApp.prefs().other()[_T("currentBetaSharewarePatchLevel")].integerValue() = level;
	#else
		theApp.prefs().other()[_T("currentSharewarePatchLevel")].integerValue() = level;
	#endif // _BETA_
#endif // _REGISTERED_

	theApp.prefs().other().update();
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	areWeOnline()
{
	// Always return online?

	if (!theApp.prefs().network()[_T("disableAutodial")].booleanValue()) return true;

	// Check the online status

	DWORD		flags;
	return InternetGetConnectedState(&flags, 0) ? true:false;

}

// ---------------------------------------------------------------------------------------------------------------------------------

unsigned char	*decodeBase64(const StringList & sl, unsigned int & size)
{
static	unsigned char	base64Lookup[256] =
{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x3f, // '+' & '/' input(2B & 2F) output (3E & 3F)
	0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // '0' - '9' input(30 - 39) output (34 - 3D)
	0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, // 'A' - 'O' input(41 - 4F) output (00 - 0e)
	0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, // 'P' - 'Z' input(50 - 5A) output (0F - 19)
	0x00, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, // 'a' - 'o' input(51 - 5F) output (1a - 28)
	0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00,	// 'p' - 'z' input(60 - 6A) output (29 - 33)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

	// Strip out everything except the data we're interested in...

	unsigned int	length = 0;
	unsigned char	*buffer;
	{
		string		data = convertToString(sl);
		unsigned int	totalSize = data.length() + 1;
		buffer = new unsigned char[totalSize];
		if (!buffer) throw string("Out of memory");
		memset(buffer, 0, totalSize);
		unsigned char	*dst = buffer;
		const char	*src = data.c_str();

		for(unsigned int i = 0; i < totalSize; i++)
		{
			char	c = *(src++);
			if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '+' || c == '=' || c == '/')
			{
				*(dst++) = c;
				length++;
			}
		}
	}

	// Allocate RAM for our output

	unsigned char	*output = new unsigned char[length];
	if (!output) throw string("Out of memory");

	// Decode it

	size = 0;
	unsigned char	*src = buffer;
	unsigned char	*dst = output;
	while(length)
	{
		// Grab up to four bytes

		unsigned int	len = length;
		if (len > 4) len = 4;

		// Direct pointer to them

		unsigned char	*quantum = src;
		src += 4;

		length -= len;

		// Is it padded?

		if (quantum[0] == '=' || quantum[1] == '=')
		{
			// We'll let quantums start with '=' if it's the last thing

			if (!length) break;

			LOG("Invalid quantum -- starts with '='", Logger::LOG_CRIT);
			delete[] output;
			output = NULL;
			delete[] buffer;
			buffer = NULL;
			size = 0;
			break;
		}

		else if (quantum[2] == '=')
		{
			unsigned int	q0 = base64Lookup[quantum[0]];
			unsigned int	q1 = base64Lookup[quantum[1]];
			unsigned int	val  = (q0 << 18) | (q1 << 12);

			*(dst++) = (val >> 16) & 0xff;
			size += 1;
			break;
		}

		else if (quantum[3] == '=')
		{
			unsigned int	q0 = base64Lookup[quantum[0]];
			unsigned int	q1 = base64Lookup[quantum[1]];
			unsigned int	q2 = base64Lookup[quantum[2]];
			unsigned int	val  = (q0 << 18) | (q1 << 12) | (q2 << 6);

			*(dst++) = (val >> 16) & 0xff;
			*(dst++) = (val >>  8) & 0xff;
			size += 2;
			break;
		}

		// Not padded

		else
		{
			unsigned int	q0 = base64Lookup[quantum[0]];
			unsigned int	q1 = base64Lookup[quantum[1]];
			unsigned int	q2 = base64Lookup[quantum[2]];
			unsigned int	q3 = base64Lookup[quantum[3]];
			unsigned int	val  = (q0 << 18) | (q1 << 12) | (q2 << 6) | q3;

			*(dst++) = (val >> 16) & 0xff;
			*(dst++) = (val >>  8) & 0xff;
			*(dst++) = (val >>  0) & 0xff;
			size += 3;
		}
	}

	delete[] buffer;

	output[size] = 0;
	return output;
}

// ---------------------------------------------------------------------------------------------------------------------------------

string	encodeBase64(const unsigned char *data, const unsigned int size)
{
static	unsigned char	base64Alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	string	result;
	int	idx = 0;
	while(idx < (int) size-2)
	{
		// Grab a 24-bit value

		unsigned int	quantum = (data[idx+0] << 16) | (data[idx+1] <<  8) | (data[idx+2] <<  0);
		idx += 3;

		// Make 4 6-bit values out of it

		result += base64Alphabet[((quantum >> 18) & 0x3f)];
		result += base64Alphabet[((quantum >> 12) & 0x3f)];
		result += base64Alphabet[((quantum >>  6) & 0x3f)];
		result += base64Alphabet[((quantum >>  0) & 0x3f)];
	}

	// Leftover

	if (size - idx == 2)
	{
		unsigned int	quantum = (data[idx+0] << 16) | (data[idx+1] <<  8);

		// Make 4 6-bit values out of it

		result += base64Alphabet[((quantum >> 18) & 0x3f)];
		result += base64Alphabet[((quantum >> 12) & 0x3f)];
		result += base64Alphabet[((quantum >>  6) & 0x3f)];
		result += '=';
	}
	else if (size - idx == 1)
	{
		unsigned int	quantum = (data[idx+0] << 16);

		// Make 4 6-bit values out of it

		result += base64Alphabet[((quantum >> 18) & 0x3f)];
		result += base64Alphabet[((quantum >> 12) & 0x3f)];
		result += '=';
		result += '=';
	}

	return result;
}

// ---------------------------------------------------------------------------------------------------------------------------------

StringList	decodeQuotedPrintable(const StringList & sl)
{
	const string		src = convertToString(sl);
	string::size_type	sidx = 0;
	char *			dst = new char[src.length()+1];
	unsigned int		didx = 0;
	memset(dst, 0, src.length()+1);

	while(sidx < src.length())
	{
		// Find the next code

		string::size_type	idx = src.find_first_of("_=", sidx);
		if (idx == string::npos) idx = src.length();

		// Copy the raw characters

		if (idx > sidx)
		{
			int	len = idx-sidx;
			memcpy(&dst[didx], &src[sidx], len);
			didx += len;
			sidx += len;
		}

		// Done?

		if (sidx >= src.length()) break;

		// Soft line break or hex code?

		if (src[sidx] == '=')
		{
			++sidx;

			// Soft line break?

			if (src[sidx] == '\r' && src[sidx+1] == '\n')
			{
				sidx += 2;
			}

			// Hex value. decode it

			else
			{
				static char hex[3];
				hex[0] = tolower(src[sidx+0]);
				hex[1] = tolower(src[sidx+1]);
				hex[2] = 0; // unneccessary, but it can't hurt!

				     if (hex[0] >= '0' && hex[0] <= '9') dst[didx] = (hex[0] - '0') << 4;
				else if (hex[0] >= 'a' && hex[0] <= 'f') dst[didx] = (hex[0] - 'a' + 10) << 4;

				     if (hex[1] >= '0' && hex[1] <= '9') dst[didx] |= (hex[1] - '0');
				else if (hex[1] >= 'a' && hex[1] <= 'f') dst[didx] |= (hex[1] - 'a' + 10);

				++didx;
				sidx += 2;
			}
		}

		// Space?

		else
		{
			dst[didx] = ' ';
			++didx;
			++sidx;
		}
	}
	
	StringList	result;
	string	temp = dst;
	delete[] dst;
	convertToStringList(temp, result);
	return result;
}

// ---------------------------------------------------------------------------------------------------------------------------------

#include "fubar_off.h"
wstring	atow(const string & str)
{
	int		len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, 0, 0);
	wchar_t	*	wstr = new wchar_t[len+1];
	memset(wstr, 0, sizeof(wchar_t) * (len+1));
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wstr, len);
	wstring	result = wstr;
	delete[] wstr;
	return result;
}

// ---------------------------------------------------------------------------------------------------------------------------------

string	wtoa(const wstring & wstr)
{
	int	len = wstr.length();
	char *	str = new char[len+1];
	memset(str, 0, len+1);
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, str, len, NULL, NULL);
	string	result = str;
	delete[] str;
	return result;
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	systemErrorString(DWORD err)
{
	wstring	result;
	LPVOID	lpMsgBuf;
	if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,	NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL ))
	{
		wchar_t	tmp[128];
		swprintf(tmp, _T("0x%08X"), err);
		result = tmp;
	}
	else
	{
		result = (LPCTSTR) lpMsgBuf;

		// Free the buffer

		LocalFree( lpMsgBuf );
	}

	return result;
}

#include "fubar_on.h"

// ---------------------------------------------------------------------------------------------------------------------------------
// Utils.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

