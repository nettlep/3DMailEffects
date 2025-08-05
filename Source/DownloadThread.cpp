// ---------------------------------------------------------------------------------------------------------------------------------
//  _____                       _                 _ _______ _                         _                      
// |  __ \                     | |               | |__   __| |                       | |                     
// | |  | | ___ __      ___ __ | | ___   __ _  __| |  | |  | |__  _ __  ___  __ _  __| |     ___ _ __  _ __  
// | |  | |/ _ \\ \ /\ / / '_ \| |/ _ \ / _` |/ _` |  | |  | '_ \| '__|/ _ \/ _` |/ _` |    / __| '_ \| '_ \ 
// | |__| | (_) |\ V  V /| | | | | (_) | (_| | (_| |  | |  | | | | |  |  __/ (_| | (_| | _ | (__| |_) | |_) |
// |_____/ \___/  \_/\_/ |_| |_|_|\___/ \__,_|\__,_|  |_|  |_| |_|_|   \___|\__,_|\__,_|(_) \___| .__/| .__/ 
//                                                                                              | |   | |    
//                                                                                              |_|   |_|    
//
// Thread for performing downloads
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 03/11/2001 by Paul Nettle
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
#include "DownloadThread.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

IMPLEMENT_DYNCREATE(DownloadThread, CWinThread)

// ---------------------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(DownloadThread, CWinThread)
	//{{AFX_MSG_MAP(DownloadThread)
	ON_THREAD_MESSAGE(DOWNLOADTHREAD_TIMER, OnTimer)
	ON_THREAD_MESSAGE(DOWNLOADTHREAD_TERMINATE, OnTerminate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	DownloadThread::DownloadThread()
	: _currentlyDownloading(false), _pFile(NULL)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

	DownloadThread::~DownloadThread()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	DownloadThread::InitInstance()
{
	AfxSocketInit();
	return TRUE;
}

// ---------------------------------------------------------------------------------------------------------------------------------

int	DownloadThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	DownloadThread::OnTerminate(unsigned int wparam, long lparam)
{
	// Shut down the thread

	AfxEndThread(0);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	DownloadThread::OnTimer(unsigned int wparam, long lparam)
{
	// If we're not supposed to download, don't

	if (!currentlyDownloading()) return;

	try
	{
		// Do we need to initiate a session?

		if (!pFile())
		{
			// Connect and send the request

			wstring	result = httpClient().sendRequest(url());
			if (result.length()) throw_wstring (_T("Unable to send HTTP request:\n\n") + result)

			// Open the destination file

			pFile() = _wfopen(destFile().c_str(), _T("w+b"));
			if (!pFile()) throw_wstring (_T("Unable to create destination file:\n\n") + destFile())
		}

		// Buffer size is dependent upon our transmit speed, minimum of one byte

		static char	buf[MAX_SPEED * 1024];
		unsigned int	bufferSize = speed() * 1024;

		// Done?

		if (bufferSize > httpClient().contentLength() - bytesWritten())
		{
			bufferSize = httpClient().contentLength() - bytesWritten();
		}

		// Get some data from the server

		unsigned int	bytesRead = 0;
		if (bufferSize)
		{
			wstring	result = httpClient().socket().recv(buf, bytesRead, bufferSize);
			if (result.length()) throw_wstring (_T("Unable to receive the file:\n\n") + result)
		}

		// Did we stop downloading while we were waiting?
			
		if (!currentlyDownloading()) return;

		// Write the bytes to the destination file

		if (bytesRead)
		{
			if (fwrite(buf, bytesRead, 1, pFile()) != 1) throw_wstring (_T("Unable to write to destination file:\n\n") + destFile())
		}

		// Track our progress

		bytesWritten() += bytesRead;
		percentComplete() = (float) bytesWritten() / (float) httpClient().contentLength() * 100.0f;

		// Are we done?

		if (bytesWritten() == httpClient().contentLength())
		{
			stopDownloading();
			return;
		}

		// Not done, track our progress

		else
		{
			// Update our stats

			clock_t	clockEnd = clock();

			if (clockStart())
			{
				currentBPS() = (double) bytesRead / (double) ((double) (clockEnd - clockStart()) / (double) CLOCKS_PER_SEC);

				averageTotal() += currentBPS();
				averageCount()++;
				averageBPS() = averageTotal() / averageCount();
			}

			clockStart() = clockEnd;

			Sleep((MAX_SPEED - speed()) * 20);
			PostThreadMessage(DOWNLOADTHREAD_TIMER, 0, 0);
		}
	}
	catch(const wstring & err)
	{
		stopDownloading();
		errorString() = _T("Error downloading update:\n\n") + err;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	DownloadThread::beginDownloading(const string & url, const wstring & destFile)
{
	// If we're already downloading, don't allow another one...

	if (currentlyDownloading()) return false;

	// User-specified

	_url = url;
	_destFile = destFile;

	// User information

	percentComplete() = 0.0f;
	currentlyDownloading() = true;
	errorString().erase();
	setSpeed();
	bytesWritten() = 0;
	averageBPS() = 0;
	currentBPS() = 0;
	pFile() = NULL;
	clockStart() = 0;
	averageTotal() = 0.0;
	averageCount() = 0.0;

	// Start the ball rollin'!

	PostThreadMessage(DOWNLOADTHREAD_TIMER, 0, 0);

	// Okay...

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	DownloadThread::stopDownloading()
{
	// Cleanup

	if (currentlyDownloading())
	{
		// Remove any remnants of the dest file...

		if (pFile())
		{
			fclose(pFile());
			pFile() = NULL;
		}

		// Okay, we're no longer downloading

		currentlyDownloading() = false;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------
// DownloadThread.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

