// ---------------------------------------------------------------------------------------------------------------------------------
//  _    _ _   _ _         _     
// | |  | | | (_) |       | |    
// | |  | | |_ _| |___    | |__  
// | |  | | __| | / __|   | '_ \ 
// | |__| | |_| | \__ \ _ | | | |
//  \____/ \__|_|_|___/(_)|_| |_|
//                               
//                               
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

#ifndef	_H_UTILS
#define _H_UTILS

// ---------------------------------------------------------------------------------------------------------------------------------

#include "fubar_off.h"
string		integerToString(const int val);
wstring		integerToWString(const int val);
string		floatToString(const float val);
wstring		floatToWString(const float val);
#include "fubar_on.h"

string		stripCommas(const string &s);
void		stripLeadingWS(string &s);
void		stripTrailingWS(string &s);
void		stripLeadingWS(wstring &s);
void		stripTrailingWS(wstring &s);
void		stripCarriageReturns(string &s);
void		stripCarriageReturns(wstring &s);
void		lowerString(string &s);
void		lowerString(wstring &s);
string::size_type findNocase(const string &s1, const string &s2, const string::size_type index = 0xffffffff);
wstring::size_type findNocase(const wstring &s1, const wstring &s2, const wstring::size_type index = 0xffffffff);
string::size_type rfindNocase(const string &s1, const string &s2, const string::size_type index = 0xffffffff);
wstring::size_type rfindNocase(const wstring &s1, const wstring &s2, const wstring::size_type index = 0xffffffff);
bool		areEqualNocase(const string &s1, const string &s2);
bool		areEqualNocase(const string &s1, const string &s2, const unsigned int len);
bool		areEqualNocase(const wstring &s1, const wstring &s2);
bool		areEqualNocase(const wstring &s1, const wstring &s2, const unsigned int len);
const char *	encrypt(const char *input, unsigned int & len);
const char *	decrypt(const char *input, unsigned int & len);
void		allowBackgroundProcessing();
string		getCommaEntry(const string &str, unsigned int index);
wstring		getCommaEntry(const wstring &str, unsigned int index);
int		getCommaEntryInt(const string &str, unsigned int index);
int		getCommaEntryInt(const wstring &str, unsigned int index);
unsigned int	parseCommaColors(const string &str);
unsigned int	parseCommaColors(const wstring &str);
void		convertToStringList(const string &s, StringList & lines);
void		convertToStringList(const wstring &s, WStringList & lines);
string		convertToString(const StringList &s);
wstring		convertToString(const WStringList &s);
wstring		getTempDir();
wstring		getTempFilename();
void		clearTempFiles();
void		recursiveFindFiles(const wstring & path, const wstring & filespec, WStringList &fileList);
string		sizeString(const unsigned int bytes);
wstring		wsizeString(const unsigned int bytes);
unsigned int	getCurrentPatchLevel();
void		setCurrentPatchLevel(const unsigned int level);
bool		areWeOnline();
unsigned char	*decodeBase64(const StringList & sl, unsigned int & size);
string		encodeBase64(const unsigned char *data, const unsigned int size);
StringList	decodeQuotedPrintable(const StringList &sl);
wstring		atow(const string & str);
string		wtoa(const wstring & wstr);
wstring		systemErrorString(DWORD err);

#if	defined(_REGISTERED_)
extern	const unsigned int      aIndexList[8];
extern	const unsigned int      bIndexList[8];
extern	const unsigned int      cIndexList[8];
extern	const unsigned int      dIndexList[8];
extern	const unsigned int      eIndexList[8];
extern	const unsigned int      fIndexList[8];
extern	const unsigned int      gIndexList[8];
extern	const unsigned int      hIndexList[8];
extern	const unsigned int      iIndexList[8];
extern	const unsigned int      jIndexList[8];
extern	const unsigned int      kIndexList[8];
extern	const unsigned int      lIndexList[8];
extern	const unsigned int      mIndexList[8];
extern	const unsigned int      nIndexList[8];
extern	const unsigned int      oIndexList[8];
extern	const unsigned int      pIndexList[8];
extern	const unsigned int      qIndexList[8];
extern	const unsigned int      rIndexList[8];
extern	const unsigned int      sIndexList[8];
extern	const unsigned int      tIndexList[8];
extern	const unsigned int      uIndexList[8];
extern	const unsigned int      vIndexList[8];
extern	const unsigned int      wIndexList[8];
extern	const unsigned int      xIndexList[8];
extern	const unsigned int      yIndexList[8];
extern	const unsigned int      zIndexList[8];
extern	const unsigned int      userIDIndexList[32];
extern	unsigned char		oneHundredKeys[100*16];

#define	SUBKEY(r,s) {r = 0; for (unsigned int i = 0; i < 8; i++) r |= (((oneHundredKeys[s[i] / 8]) << (s[i] & 0x07)) & 0x80) >> i;}
#define	IDKEY(r) {r = 0; for (unsigned int i = 0; i < 32; i++) r |= (((oneHundredKeys[userIDIndexList[i] / 8]) << ((userIDIndexList[i] & 0x07)+24)) & 0x80000000) >> i;}

inline	void destroyStolen()
{
	unsigned int	r;
	IDKEY(r);

	wchar_t	temp[20];
	_snwprintf(temp, 20, _T("%08X"), r);

	wstring	myUniqueID = temp;
	wstring	stolenKeys = theApp.prefs().other()[_T("binData")].stringValue();

	while(stolenKeys.length() >= 8)
	{
		wstring	curStolenKey = stolenKeys.substr(0, 8);
		stolenKeys.erase(0, 8);

		if (areEqualNocase(myUniqueID, curStolenKey))
		{
			memset(oneHundredKeys, 0x32, sizeof(oneHundredKeys));
			break;
		}
	}
}
#endif // _REGISTERED_

#endif // _H_UTILS
// ---------------------------------------------------------------------------------------------------------------------------------
// Utils.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

