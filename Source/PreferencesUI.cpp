/// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __                                    _    _ _____                      
// |  __ \           / _|                                  | |  | |_   _|                     
// | |__) |_ __  ___| |_  ___ _ __  ___ _ __   ___  ___ ___| |  | | | |       ___ _ __  _ __  
// |  ___/| '__|/ _ \  _|/ _ \ '__|/ _ \ '_ \ / __|/ _ \ __| |  | | | |      / __| '_ \| '_ \ 
// | |    | |  |  __/ | |  __/ |  |  __/ | | | (__|  __/__ \ |__| |_| |_  _ | (__| |_) | |_) |
// |_|    |_|   \___|_|  \___|_|   \___|_| |_|\___|\___|___/\____/|_____|(_) \___| .__/| .__/ 
//                                                                               | |   | |    
//                                                                               |_|   |_|    
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

#include "stdafx.h"
#include "3Dme.h"
#include "PreferencesUI.h"
#include "PrefletGlobal.h"
#include "PrefletWindow.h"
#include "PrefletAbout.h"
#include "PrefletSupport.h"
#include "PrefletCredits.h"

// ---------------------------------------------------------------------------------------------------------------------------------

IMPLEMENT_DYNAMIC(PreferencesUI, CDialog)
BEGIN_MESSAGE_MAP(PreferencesUI, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, OnTvnSelchangedTree)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_APPLY, OnBnClickedApply)
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

class	ItemEntry
{
	public:
	wstring		text;
	HTREEITEM	item;
};

// ---------------------------------------------------------------------------------------------------------------------------------

	PreferencesUI::PreferencesUI(PrefletMap & preflets, CWnd* pParent /*=NULL*/)
	: CDialog(PreferencesUI::IDD, pParent), _preflets(preflets)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

	PreferencesUI::~PreferencesUI()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PreferencesUI::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE, treeControl);
	DDX_Control(pDX, IDC_PREFLET_FRAME, prefletFrame);
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	PreferencesUI::OnInitDialog()
{
	CDialog::OnInitDialog();

	// We use this to keep track of the items as we add them, so we can find parents when we need to

	vector<ItemEntry>	items;
	for (PrefletMap::iterator i = preflets().begin(); i != preflets().end(); ++i)
	{
		// The current preflet

		Preflet &	preflet = *(i->second);
		wstring &	regKey = preflet.registryKey();

		// Skip preflets that don't have dialogs

		if (!preflet.idd()) continue;

		// Our new tree item...

		HTREEITEM	item = 0;

		// Is this a root node?

		wstring::size_type	idx = regKey.rfind(_T("\\"));
		wstring			parentName;
		wstring			childName;
		if (idx == string::npos)
		{
			childName = regKey;
			item  = treeControl.InsertItem(regKey.c_str(), 0, 0);
			TRACE(_T("%d - %s\n"), item, regKey.c_str());
		}

		// It's on a branch (i.e., a child of some other node)

		else
		{
			parentName = regKey.substr(0, idx);
			childName = regKey.substr(idx+1);

			// Locate the parent item

			for (unsigned int j = 0; j < items.size(); ++j)
			{
				if (items[j].text != parentName) continue;

				item = treeControl.InsertItem(childName.c_str(), items[j].item, 0);
				TRACE(_T("%d - %s\n"), item, childName.c_str());
				break;
			}
		}

		if (item)
		{
			// Create a dialog for this preflet

			preflet.Create(preflet.idd(), this);
			preflet.ShowWindow(SW_HIDE);
			preflet.item() = item;

			// Move it into position

			CRect	prefletRect;
			prefletFrame.GetWindowRect(prefletRect);
			ScreenToClient(prefletRect);
			preflet.MoveWindow(prefletRect.left, prefletRect.top, prefletRect.Width(), prefletRect.Height());

			// Keep track of the new entry in case it has children (or children's children...)

			ItemEntry	entry;
			entry.text = regKey.c_str();
			entry.item = item;
			items.push_back(entry);
		}
		else
		{
			wstring	err = _T("Unable to locate a place for the entry: ") + regKey;
			AfxMessageBox(err.c_str());
		}
	}

	// Find, hilight and expand the default entry (or the first entry, if no default is specified)

	bool	defaultFlag = defaultPreflet().length() == 0;

	for (PrefletMap::iterator i = preflets().begin(); i != preflets().end(); ++i)
	{
		Preflet &	preflet = *(i->second);

		// Default?

		if (preflet.registryKey() == defaultPreflet() || defaultFlag)
		{
			treeControl.SelectItem(preflet.item());
			treeControl.Expand(preflet.item(), TVE_EXPAND);
			defaultFlag = false;
		}

	}

	return TRUE;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PreferencesUI::OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	showPrefletDialog(pNMTreeView->itemOld.hItem, false);
	showPrefletDialog(pNMTreeView->itemNew.hItem, true);

	*pResult = 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PreferencesUI::OnBnClickedOk()
{
	OnBnClickedApply();
	OnOK();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PreferencesUI::OnBnClickedApply()
{
	preflets().apply();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PreferencesUI::showPrefletDialog(const HTREEITEM item, const bool show)
{
	// Show the dialog for given name

	if (item) preflets().showPreflet(item, show);
}

// ---------------------------------------------------------------------------------------------------------------------------------
// PreferencesUI.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
