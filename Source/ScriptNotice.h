// ---------------------------------------------------------------------------------------------------------------------------------
//   _____           _       _   _   _       _   _               _     
//  / ____|         (_)     | | | \ | |     | | (_)             | |    
// | (___   ___ _ __ _ _ __ | |_|  \| | ___ | |_ _  ___  ___    | |__  
//  \___ \ / __| '__| | '_ \| __| . ` |/ _ \| __| |/ __|/ _ \   | '_ \ 
//  ____) | (__| |  | | |_) | |_| |\  | (_) | |_| | (__|  __/ _ | | | |
// |_____/ \___|_|  |_| .__/ \__|_| \_|\___/ \__|_|\___|\___|(_)|_| |_|
//                    | |                                              
//                    |_|                                              
//
// Provide a notice about missing webmail script support
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 03/09/2001 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2001, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_SCRIPTNOTICE
#define _H_SCRIPTNOTICE

// ---------------------------------------------------------------------------------------------------------------------------------

class	ScriptNotice : public CDialog
{
public:
				ScriptNotice(CWnd* pParent = NULL);

	//{{AFX_DATA(ScriptNotice)
	enum { IDD = IDD_SCRIPT_NOTICE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(ScriptNotice)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(ScriptNotice)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
#endif // _H_SCRIPTNOTICE
// ---------------------------------------------------------------------------------------------------------------------------------
// ScriptNotice.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

