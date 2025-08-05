// ---------------------------------------------------------------------------------------------------------------------------------
//  _______ _                    _______ _                         _     _     
// |__   __| |                  |__   __| |                       | |   | |    
//    | |  | |__  _ __  ___  ___   | |  | |__  _ __  ___  __ _  __| |   | |__  
//    | |  | '_ \| '__|/ _ \/ _ \  | |  | '_ \| '__|/ _ \/ _` |/ _` |   | '_ \ 
//    | |  | | | | |  |  __/  __/  | |  | | | | |  |  __/ (_| | (_| | _ | | | |
//    |_|  |_| |_|_|   \___|\___|  |_|  |_| |_|_|   \___|\__,_|\__,_|(_)|_| |_|
//                                                                             
//                                                                             
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

#ifndef	_H_THREETHREAD
#define _H_THREETHREAD

// ---------------------------------------------------------------------------------------------------------------------------------
// Forward declarations
// ---------------------------------------------------------------------------------------------------------------------------------

class	ThreeDlg;
class	C3DmeDlg;

// ---------------------------------------------------------------------------------------------------------------------------------

class	ThreeThread : public CWinThread
{
	DECLARE_DYNCREATE(ThreeThread)

		ThreeDlg	*threeDlg;
		C3DmeDlg	*dialog;

protected:
				ThreeThread(); // protected constructor used by dynamic creation
virtual				~ThreeThread();

public:
	//{{AFX_VIRTUAL(ThreeThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

protected:
virtual		void		cleanupThread();

	//{{AFX_MSG(ThreeThread)
	afx_msg	void OnTerminateThread(unsigned int wparam, long lparam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}

#endif // _H_THREETHREAD
// ---------------------------------------------------------------------------------------------------------------------------------
// ThreeThread.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

