// ---------------------------------------------------------------------------------------------------------------------------------
//  _    _           _ _____  _                            
// | |  | |         | |  __ \| |                           
// | |  | |_ __   __| | |  | | | __ _      ___ _ __  _ __  
// | |  | | '_ \ / _` | |  | | |/ _` |    / __| '_ \| '_ \ 
// | |__| | |_) | (_| | |__| | | (_| | _ | (__| |_) | |_) |
//  \____/| .__/ \__,_|_____/|_|\__, |(_) \___| .__/| .__/ 
//        | |                    __/ |        | |   | |    
//        |_|                   |___/         |_|   |_|    
//
// 3D Mail Effects Updater Dialog
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 02/10/2001 by Paul Nettle
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
#include "Upd.h"
#include "UpdDlg.h"
#include <string>
#include <io.h>
using namespace std;

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CUpdDlg, CDialog)
	//{{AFX_MSG_MAP(CUpdDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	CUpdDlg::CUpdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpdDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUpdDlg)
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	CUpdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUpdDlg)
	//}}AFX_DATA_MAP
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	CUpdDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	SetTimer(0, 100, NULL);
	
	return TRUE;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	CUpdDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

HCURSOR	CUpdDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// --------------------------------------------------------------------------------------------------------------------------------

static BOOL CALLBACK getApplicationHandleProc(HWND hwnd, LPARAM lParam)
{
	char	string[256];
	memset(string, 0, sizeof(string));
	::GetWindowText(hwnd, string, sizeof(string) - 1);
	if (!stricmp(string, "3D Mail Effects "))
	{
		if (lParam)
		{
			int	*t = (int *) lParam;
			*t = 1;
		}
		return FALSE;
	}

	return TRUE;
}

// ---------------------------------------------------------------------------------------------------------------------------------

static	string	errorString(const unsigned int error)
{
	switch(error)
	{
		case EPERM 	 : return "EPERM: Not owner";
		case ENOENT 	 : return "ENOENT: No such file or directory";
		case ESRCH	 : return "ESRCH: No such process";
		case EINTR	 : return "EINTR: Interrupted system call";
		case EIO	 : return "EIO: I/O error";
		case ENXIO	 : return "ENXIO: No such device or address";
		case E2BIG	 : return "E2BIG: Argument list too long";
		case ENOEXEC	 : return "ENOEXEC: Exec format error";
		case EBADF	 : return "EBADF: Bad file number";
		case ECHILD	 : return "ECHILD: No spawned processes";
		case EAGAIN	 : return "EAGAIN: No more processes or not enough memory or maximum nesting level reached";
		case ENOMEM	 : return "ENOMEM: Not enough memory";
		case EACCES	 : return "EACCES: Permission denied";
		case EFAULT	 : return "EFAULT: Bad address";
		case EBUSY	 : return "EBUSY: Mount device busy";
		case EEXIST	 : return "EEXIST: File exists";
		case EXDEV	 : return "EXDEV: Cross-device link";
		case ENODEV	 : return "ENODEV: No such device";
		case ENOTDIR	 : return "ENOTDIR: Not a directory";
		case EISDIR	 : return "EISDIR: Is a directory";
		case EINVAL	 : return "EINVAL: Invalid argument";
		case ENFILE	 : return "ENFILE: File table overflow";
		case EMFILE	 : return "EMFILE: Too many open files";
		case ENOTTY	 : return "ENOTTY: Not a teletype";
		case EFBIG	 : return "EFBIG: File too large";
		case ENOSPC	 : return "ENOSPC: No space left on device";
		case ESPIPE	 : return "ESPIPE: Illegal seek";
		case EROFS	 : return "EROFS: Read-only file system";
		case EMLINK	 : return "EMLINK: Too many links";
		case EPIPE	 : return "EPIPE: Broken pipe";
		case EDOM	 : return "EDOM: Math argument";
		case ERANGE	 : return "ERANGE: Result too large";
		case EDEADLK	 : return "EDEADLK: Resource deadlock would occur";
		case ENAMETOOLONG: return "ENAMETOOLONG: Name too long";
		case ENOLCK	 : return "ENOLCK: No lock";
		case ENOSYS	 : return "ENOSYS: No sys";
		case ENOTEMPTY	 : return "ENOTEMPTY: Not empty";
		case EILSEQ	 : return "EILSEQ: Illegal sequence";
		default		 : return "UNKNOWN ERROR";
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	CUpdDlg::OnTimer(UINT nIDEvent) 
{
	int	flag = 0;
	EnumWindows(getApplicationHandleProc, (LPARAM) &flag);

	if (!flag)
	{
		// Get our current working directory

		string	programPath = GetCommandLine();

		// strip the leading quote...

		if (programPath[0] == '"') programPath.erase(0, 1);

		// Find the path in all of this...

		string::size_type	idx = programPath.find_last_of("\\");
		if (idx == string::npos)	programPath.erase();
		else				programPath.erase(idx+1);

		KillTimer(nIDEvent);

		// No longer need this file...

		unlink("oleacc.dll");
		unlink("oleacc.bak");
		unlink("_oleacc.dll");
		unlink("_oleacc.bak");

		bool	allOK = true;
		if (!doRename(programPath, "3dme", "exe")) allOK = false;
		if (!doRename(programPath, "libeay32", "dll")) allOK = false;
		if (!doRename(programPath, "ssleay32", "dll")) allOK = false;
		if (!doRename(programPath, "UnicoWS", "dll")) allOK = false;

		if (allOK)
		{
			::ShellExecute(::GetDesktopWindow(), "open", "3dme.exe", "updated", programPath.c_str(), SW_SHOWNORMAL);
		}
		else
		{
			::ShellExecute(::GetDesktopWindow(), "open", "3dme.exe", NULL, programPath.c_str(), SW_SHOWNORMAL);
		}
		PostMessage(WM_COMMAND, IDOK);
	}
	
	CDialog::OnTimer(nIDEvent);
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	CUpdDlg::doRename(const string & programPath, const string & base, const string & ext)
{
	string	a = programPath + base + "." + ext;
	string	b = programPath + base + ".bak";
	string	c = programPath + "_" + base + "." + ext;

	// Is there a new EXE?

	if (!_access(c.c_str(), 0))
	{
		try
		{
			unlink(b.c_str());

			bool	statusOK = false;
			int	retries = 100;
			while(retries-- > 0)
			{
				if (!rename(a.c_str(), b.c_str()))
				{
					statusOK = true;
					break;
				}
				Sleep(100);
			}

			//if (!statusOK) throw "While trying to rename...\n\n" + a + "\nto\n" + b + "\n\n" + errorString(errno);

			statusOK = false;
			retries = 100;
			while(retries-- > 0)
			{
				if (!rename(c.c_str(), a.c_str()))
				{
					statusOK = true;
					break;
				}
				Sleep(100);
			}

			if (!statusOK) throw "While trying to rename...\n\n" + c + "\nto\n" + a + "\n\n" + errorString(errno);

			statusOK = false;
			retries = 100;
			while(retries-- > 0)
			{
				if (!unlink(b.c_str()))
				{
					statusOK = true;
					break;
				}
				Sleep(100);
			}

			// if (!statusOK) throw "While trying to delete...\n\n" + b + "\n\n" + errorString(errno);

		}
		catch(const string &err)
		{
			MessageBox(err.c_str(), "An error occurred while trying to update 3D Mail Effects", MB_OK|MB_ICONWARNING);
			return false;
		}
	}

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------
// UpdDlg.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

