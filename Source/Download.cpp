// ---------------------------------------------------------------------------------------------------------------------------------
//  _____                       _                 _                      
// |  __ \                     | |               | |                     
// | |  | | ___ __      ___ __ | | ___   __ _  __| |     ___ _ __  _ __  
// | |  | |/ _ \\ \ /\ / / '_ \| |/ _ \ / _` |/ _` |    / __| '_ \| '_ \ 
// | |__| | (_) |\ V  V /| | | | | (_) | (_| | (_| | _ | (__| |_) | |_) |
// |_____/ \___/  \_/\_/ |_| |_|_|\___/ \__,_|\__,_|(_) \___| .__/| .__/ 
//                                                          | |   | |    
//                                                          |_|   |_|    
//
// Update file downloader
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 02/09/2001 by Paul Nettle
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
#if	!defined(_LITE_)
#include "3dme.h"
#include "Download.h"
#include "DownloadThread.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

const	unsigned int	Download::downloadTimer = 54321;

// ---------------------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(Download, CDialog)
	//{{AFX_MSG_MAP(Download)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	Download::Download(const wstring statusLine, const StringList & URLs, const WStringList & filenames, CWnd* pParent /*=NULL*/)
	: _startTime(NULL), _totalFilesToDownload(0), _currentFileToDownload(0), _downloadThread(NULL), CDialog(Download::IDD, pParent)
{
	_statusLine = statusLine;
	_URLs = URLs;
	_filenames = filenames;

	//{{AFX_DATA_INIT(Download)
	//}}AFX_DATA_INIT
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	Download::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Download)
	DDX_Control(pDX, IDC_STATIC_CURRENT_FILE, currentFileStatic);
	DDX_Control(pDX, IDC_OVERALL_PROGRESS, overallProgress);
	DDX_Control(pDX, IDC_FILE_PROGRESS, fileProgress);
	DDX_Control(pDX, IDC_STATIC_RATE_AVERAGE, averageRateStatic);
	DDX_Control(pDX, IDC_DOWNLOAD_URL, downloadUrlStatic);
	DDX_Control(pDX, IDC_STATIC_TIME_LEFT, timeLeftStatic);
	DDX_Control(pDX, IDC_STATIC_TIME, timeStatic);
	DDX_Control(pDX, IDC_STATIC_STATUS, statusStatic);
	DDX_Control(pDX, IDC_STATIC_SIZE, sizeStatic);
	DDX_Control(pDX, IDC_STATIC_RATE, rateStatic);
	DDX_Control(pDX, IDC_STATIC_REMAIN, sizeRemainStatic);
	DDX_Control(pDX, IDC_SPEED_SLIDER, speedSlider);
	//}}AFX_DATA_MAP
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	Download::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Window text

	statusStatic.SetWindowText(statusLine().c_str());

	// Default the speed slider sliders

	speedSlider.SetRange(0, 50);
	speedSlider.SetPos(50);
#if	defined(_DEBUG)
	speedSlider.SetPos(15);
#endif // _DEBUG

	// Progress meters

	fileProgress.SetRange(0, 1000);
	fileProgress.SetPos(0);
	overallProgress.SetRange(0, 1000);
	overallProgress.SetPos(0);

	// For stats

	currentFileToDownload() = 0;
	totalFilesToDownload() = URLs().size();
	startTime() = time(NULL);

	// Start the thread
	
	downloadThread() = (DownloadThread *) AfxBeginThread(RUNTIME_CLASS(DownloadThread));
	if (!downloadThread())
	{
		AfxMessageBox(_T("Unable to create a background thread for downloading"));
		PostMessage(WM_COMMAND, IDCANCEL);
		return TRUE;
	}

	// Start processing the files...

	SetTimer(downloadTimer, 50, NULL);

	return TRUE;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	Download::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == downloadTimer)
	{
		KillTimer(nIDEvent);

		// If we're not downloading something right now, queue up the next file

		if (!downloadThread()->currentlyDownloading())
		{
			// Was there an error?

			if (downloadThread()->errorString().length())
			{
				AfxMessageBox(downloadThread()->errorString().c_str());
				PostMessage(WM_COMMAND, IDCANCEL);
			}

			// Are we done??

			else if (totalFilesToDownload() == currentFileToDownload())
			{
				PostMessage(WM_COMMAND, IDOK);
			}

			// Queue up the file

			else
			{
				// Which file?

				currentFileToDownload()++;
				wstring	xofx = integerToWString(currentFileToDownload()) + _T(" of ") + integerToWString(totalFilesToDownload());
				currentFileStatic.SetWindowText(xofx.c_str());

				// Grab the URL

				string	currentURL = URLs().front();
				URLs().pop_front();
				wstring	currentFile = filenames().front();
				filenames().pop_front();

				// Tell them where the file is coming from

				downloadUrlStatic.SetWindowText(atow(currentURL).c_str());

				// Start that download!

				downloadThread()->beginDownloading(currentURL, currentFile);

				// Set the current speed of the download

				downloadThread()->setSpeed(speedSlider.GetPos());
			}
		}

		// A file is still in progress, update the stats

		else
		{
			// Set the current speed of the download

			downloadThread()->setSpeed(speedSlider.GetPos());

			// The progress bars
			{
				double	percent = downloadThread()->percentComplete() * 10.0;
				fileProgress.SetPos((unsigned int) percent);

				// The two bars are identical with only one file to download

				double	overall1 = (double) (currentFileToDownload()-1) / (double) totalFilesToDownload() * 1000.0;
				double	overall2 = (double)  currentFileToDownload()    / (double) totalFilesToDownload() * 1000.0;
				double	overall  = overall1 + (overall2-overall1) * (percent/1000.0);
				overallProgress.SetPos((int) overall);
			}


			// The total download time so far...

			unsigned int	seconds = time(NULL) - startTime();
			unsigned int	hours = seconds / 60 / 60;	seconds -= hours * 60 * 60;
			unsigned int	minutes = seconds / 60;		seconds -= minutes * 60;
			wchar_t		timeString[90];
			_snwprintf(timeString, 90, _T("%02d:%02d:%02d"), hours, minutes, seconds);
			timeStatic.SetWindowText(timeString);

			// The bytes remaining & total size

			if (downloadThread()->httpClient().contentLength())
			{
				unsigned int	cl = downloadThread()->httpClient().contentLength();
				if (cl == 0xffffffff)
				{
					sizeStatic.SetWindowText(_T("???"));
					sizeRemainStatic.SetWindowText(_T("???"));
				}
				else
				{
					sizeStatic.SetWindowText(wsizeString(cl).c_str());
					wstring	str = wsizeString(cl - downloadThread()->bytesWritten());
					sizeRemainStatic.SetWindowText(str.c_str());
				}
			}

			// Throughput

			if (downloadThread()->currentBPS())
			{
				wstring	str = wsizeString((unsigned int) downloadThread()->currentBPS()) + _T(" per second");
				rateStatic.SetWindowText(str.c_str());
			}
			else
			{
				rateStatic.SetWindowText(_T("???"));
			}

			// Average throughput

			if (downloadThread()->averageBPS())
			{
				wstring	str = wsizeString((unsigned int) downloadThread()->averageBPS()) + _T(" per second");
				averageRateStatic.SetWindowText(str.c_str());
			}
			else
			{
				averageRateStatic.SetWindowText(_T("???"));
			}

			// Time remaining

			if (downloadThread()->currentBPS())
			{
				int	sr = (int) ((double) (downloadThread()->httpClient().contentLength() - downloadThread()->bytesWritten()) / downloadThread()->currentBPS());
				int	hr = sr / 60 / 60; sr -= hr * 60 * 60;
				int	mr = sr / 60;      sr -= mr * 60;
				_snwprintf(timeString, 90, _T("%02d:%02d:%02d"), hr, mr, sr);
				timeLeftStatic.SetWindowText(timeString);
			}
			else
			{
				timeLeftStatic.SetWindowText(_T("??:??:??"));
			}
		}

		// Check again in a short while...

		SetTimer(nIDEvent, 50, NULL);
	}

	CDialog::OnTimer(nIDEvent);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	Download::OnOK() 
{
	cancelDownload();
	CDialog::OnOK();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	Download::OnCancel() 
{
	cancelDownload();
	CDialog::OnCancel();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	Download::cancelDownload()
{
	if (downloadThread())
	{
		KillTimer(downloadTimer);
		downloadThread()->stopDownloading();
		downloadThread()->PostThreadMessage(DOWNLOADTHREAD_TERMINATE, 0, 0);
		Sleep(100);
		downloadThread() = NULL;
	}
}

#endif // !_LITE_

// ---------------------------------------------------------------------------------------------------------------------------------
// Download.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
