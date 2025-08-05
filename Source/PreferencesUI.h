// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __                                    _    _ _____     _     
// |  __ \           / _|                                  | |  | |_   _|   | |    
// | |__) |_ __  ___| |_  ___ _ __  ___ _ __   ___  ___ ___| |  | | | |     | |__  
// |  ___/| '__|/ _ \  _|/ _ \ '__|/ _ \ '_ \ / __|/ _ \ __| |  | | | |     | '_ \ 
// | |    | |  |  __/ | |  __/ |  |  __/ | | | (__|  __/__ \ |__| |_| |_  _ | | | |
// |_|    |_|   \___|_|  \___|_|   \___|_| |_|\___|\___|___/\____/|_____|(_)|_| |_|
//
//   Main preferences dialog, which houses all of the mini preferences dialogs ("preflets")
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 08/24/2003 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2003, Fluid Studios, all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_PREFERENCESUI
#define _H_PREFERENCESUI

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

#include "PrefletMap.h"

// ---------------------------------------------------------------------------------------------------------------------------------

class	PreferencesUI : public CDialog
{
public:
	// Enumerations

	enum				{ IDD = IDD_PREFERENCESUI };

	// Construction/Destruction

					PreferencesUI(PrefletMap & preflets, CWnd * pParent = NULL);
virtual					~PreferencesUI();

	// Operators

	// Implementation

virtual		void			DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
virtual		BOOL			OnInitDialog();
afx_msg		void			OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult);
afx_msg		void			OnBnClickedOk();
afx_msg		void			OnBnClickedApply();
virtual		void			showPrefletDialog(const HTREEITEM item, const bool show);

	// Accessors

inline		PrefletMap &		preflets()		{return _preflets;}
inline	const	PrefletMap &		preflets() const	{return _preflets;}
inline		wstring &		defaultPreflet()	{return _defaultPreflet;}
inline	const	wstring &		defaultPreflet() const	{return _defaultPreflet;}

private:
	// Data members

		CTreeCtrl		treeControl;
		CStatic			prefletFrame;

		PrefletMap &		_preflets;
		wstring			_defaultPreflet;

					DECLARE_DYNAMIC(PreferencesUI)
					DECLARE_MESSAGE_MAP()
};

#endif // _H_PREFERENCESUI
// ---------------------------------------------------------------------------------------------------------------------------------
// PreferencesUI.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
