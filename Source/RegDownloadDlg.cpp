// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             _____                       _                 _ _____  _                            
// |  __ \           |  __ \                     | |               | |  __ \| |                           
// | |__) | ___  __ _| |  | | ___ __      ___ __ | | ___   __ _  __| | |  | | | __ _      ___ _ __  _ __  
// |  _  / / _ \/ _` | |  | |/ _ \\ \ /\ / / '_ \| |/ _ \ / _` |/ _` | |  | | |/ _` |    / __| '_ \| '_ \ 
// | | \ \|  __/ (_| | |__| | (_) |\ V  V /| | | | | (_) | (_| | (_| | |__| | | (_| | _ | (__| |_) | |_) |
// |_|  \_\\___|\__, |_____/ \___/  \_/\_/ |_| |_|_|\___/ \__,_|\__,_|_____/|_|\__, |(_) \___| .__/| .__/ 
//               __/ |                                                          __/ |        | |   | |    
//              |___/                                                          |___/         |_|   |_|    
//
// Description:
//
//   Registered version downloader
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
#include "3dme.h"
#include "RegDlg.h"
#include "RegDownloadDlg.h"
#include "UpdatesDlg.h"
#include "Download.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(RegDownloadDlg, CDialog)
	//{{AFX_MSG_MAP(RegDownloadDlg)
	ON_BN_CLICKED(IDC_REG_DOWNLOAD, OnRegDownloadDlg)
	ON_BN_CLICKED(IDC_REG_WEBSITE, OnRegWebsite)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	RegDownloadDlg::RegDownloadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(RegDownloadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(RegDownloadDlg)
	//}}AFX_DATA_INIT
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	RegDownloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RegDownloadDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

// ---------------------------------------------------------------------------------------------------------------------------------

static	bool	validateChecksum(const unsigned char *key)
{
	unsigned short	*ptr = (unsigned short *) key;
	unsigned short	sum = (key[14] << 8) | key[15];

	// NULL checksums should never work... (thanks Tigo)

	if (!sum) return false;

	for (int i = 0; i < 7; i++, ptr++)
	{
		// At least each 16-bit value in the key should have at least one bit set

		if (!*ptr) return false;

		// Checksum

		sum -= *ptr;
	}

	// Valid checksum?

	return sum == 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

static	void	destroySingleStolen(unsigned char *binkey)
{
const	unsigned int	userIDIndexList[32] = {3,13,14,15,16,17,28,30,47,49,52,53,62,63,64,65,66,73,74,75,76,82,83,88,89,90,97,98,99,100,105,106};
	unsigned int	r = 0;
	for (unsigned int i = 0; i < 32; i++)
	{
		unsigned char	idIndex = userIDIndexList[i];
		unsigned char	byteIndex = idIndex / 8;
		unsigned char	byteShift = (idIndex & 0x07) + 24;
		unsigned int	byte = binkey[byteIndex];
		r |= ((byte << byteShift) & 0x80000000) >> i;
	}

	wchar_t	temp[20];
	_snwprintf(temp, 20, _T("%08X"), r);

	wstring	myUniqueID = temp;
	wstring	stolenKeys = theApp.prefs().other()[_T("bindata")].stringValue();

	while(stolenKeys.length() >= 8)
	{
		wstring	curStolenKey = stolenKeys.substr(0, 8);
		stolenKeys.erase(0, 8);

		if (areEqualNocase(myUniqueID, curStolenKey))
		{
			memset(binkey, 0x32, 16);
			break;
		}
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

static	unsigned char	*convertToBinary(const wstring &key)
{
	// Working copy

	wstring	temp = key;

	// Strip out anything that's not a valid key value

	for(;;)
	{
		wstring::size_type	idx = temp.find_first_not_of(_T("0123456789ABCDEFabcdef"));
		if (idx == wstring::npos) break;
		temp.erase(idx, 1);
	}

	// The key should be exactly 32 bytes long

	if (temp.length() != 32) return NULL;

	// Parse it into a series of 16 binary bytes

	int		index = 0;
static	unsigned char	result[16];
	while(temp.length() >= 2)
	{
		unsigned char	nibble0 = (unsigned char) towlower(temp[0]);
		     if (nibble0 >= '0' && nibble0 <= '9') nibble0 -= '0';
		else if (nibble0 >= 'a' && nibble0 <= 'f') nibble0 -= 'a' - 10;
		else nibble0 = 0;

		unsigned char	nibble1 = (unsigned char) towlower(temp[1]);
		     if (nibble1 >= '0' && nibble1 <= '9') nibble1 -= '0';
		else if (nibble1 >= 'a' && nibble1 <= 'f') nibble1 -= 'a' - 10;
		else nibble1 = 0;

		temp.erase(0, 2);

		result[index++] = ((nibble0 << 4) & 0xf0) | (nibble1 & 0xf);
	}

	destroySingleStolen(result);
	return result;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	RegDownloadDlg::OnRegDownloadDlg() 
{
#if	!defined(_LITE_)
	// Do they have a reg code?

	{
		RegDlg	dlg;
		if (dlg.DoModal() == IDOK)
		{
			unsigned char	*binKey = convertToBinary((wstring) dlg.regCode);
			if (!binKey || !validateChecksum(binKey))
			{
				AfxMessageBox(_T("You must have a registration code. Sorry."));
				return;
			}
		}
		else
		{
			return;
		}
	}

	// Where do they want it?
	
	wchar_t		fname[MAX_PATH];
	wcscpy(fname, _T("3dme-Registered.zip"));
	wchar_t		filters[] = _T("All files (*.*)\0*.*\0\0");
	OPENFILENAME	of;
	memset(&of, 0, sizeof(OPENFILENAME));

	of.lStructSize  = sizeof(OPENFILENAME);
	of.hwndOwner    = GetSafeHwnd();
	of.lpstrFilter  = filters;
	of.nFilterIndex = 1;
	of.lpstrFile    = fname;
	of.nMaxFile     = MAX_PATH - 1;
	of.lpstrTitle   = _T("Choose destination location");
	of.Flags        = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_OVERWRITEPROMPT;

	if (!GetSaveFileName(&of)) return;

	// Setup to download

	StringList	url;
	url.push_back("http://www.3DMailEffects.com/download/installs/3dme-Registered.zip");

	WStringList	file;
	file.push_back(of.lpstrFile);

	// Download

	Download	dlg(_T("Downloading registered software"), url, file);
	if (dlg.DoModal() == IDOK) MessageBeep(MB_ICONASTERISK);
#endif
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	RegDownloadDlg::OnRegWebsite() 
{
	::ShellExecute(::GetDesktopWindow(), NULL, _T("http://www.3DMailEffects.com/"), NULL, NULL, SW_SHOWNORMAL);
}

// ---------------------------------------------------------------------------------------------------------------------------------
// RegDownloadDlg.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
