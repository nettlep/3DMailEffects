// ---------------------------------------------------------------------------------------------------------------------------------
//  _______ _                    _______ _                         _                      
// |__   __| |                  |__   __| |                       | |                     
//    | |  | |__  _ __  ___  ___   | |  | |__  _ __  ___  __ _  __| |     ___ _ __  _ __  
//    | |  | '_ \| '__|/ _ \/ _ \  | |  | '_ \| '__|/ _ \/ _` |/ _` |    / __| '_ \| '_ \ 
//    | |  | | | | |  |  __/  __/  | |  | | | | |  |  __/ (_| | (_| | _ | (__| |_) | |_) |
//    |_|  |_| |_|_|   \___|\___|  |_|  |_| |_|_|   \___|\__,_|\__,_|(_) \___| .__/| .__/ 
//                                                                           | |   | |    
//                                                                           |_|   |_|    
//
// Thread for 3D popup
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 01/14/2001 by Paul Nettle
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
#include "ThreeThread.h"
#include "3DmeDlg.h"
#include "ThreeDlg.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

IMPLEMENT_DYNCREATE(ThreeThread, CWinThread)

// ---------------------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(ThreeThread, CWinThread)
	//{{AFX_MSG_MAP(ThreeThread)
	ON_THREAD_MESSAGE(THREETHREAD_TERMINATE, OnTerminateThread)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	ThreeThread::ThreeThread()
{
	threeDlg = NULL;
	dialog = NULL;
}

// ---------------------------------------------------------------------------------------------------------------------------------

	ThreeThread::~ThreeThread()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	ThreeThread::InitInstance()
{
	threeDlg = new ThreeDlg(*this);
	return TRUE;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ThreeThread::cleanupThread()
{
	if (threeDlg)
	{
		threeDlg->DestroyWindow();
		delete threeDlg;
		threeDlg = NULL;
	}
	if (dialog) dialog->threeThread = NULL;
}

// ---------------------------------------------------------------------------------------------------------------------------------

int	ThreeThread::ExitInstance()
{
	cleanupThread();
	return CWinThread::ExitInstance();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ThreeThread::OnTerminateThread(unsigned int wparam, long lparam)
{
	cleanupThread();
	dialog->threeThreadTerminated = true;
	AfxEndThread(0);
}

// ---------------------------------------------------------------------------------------------------------------------------------
// ThreeThread.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

