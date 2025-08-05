// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _                        
// |  __ \           / _| |    | |                       
// | |__) |_ __  ___| |_| | ___| |_      ___ _ __  _ __  
// |  ___/| '__|/ _ \  _| |/ _ \ __|    / __| '_ \| '_ \ 
// | |    | |  |  __/ | | |  __/ |_  _ | (__| |_) | |_) |
// |_|    |_|   \___|_| |_|\___|\__|(_) \___| .__/| .__/ 
//                                          | |   | |    
//                                          |_|   |_|    
//
//   Base class for preflet dialogs
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
#include "Preflet.h"
#include ".\preflet.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

IMPLEMENT_DYNAMIC(Preflet, CDialog)
BEGIN_MESSAGE_MAP(Preflet, CDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	Preflet::Preflet()
	: CDialog(), _idd(0), _item(0)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

	Preflet::Preflet(const int idd, const wstring & registryKey, CWnd* pParent /*=NULL*/)
	: CDialog(idd, pParent), _idd(idd), _item(0), _registryKey(registryKey)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

	Preflet::~Preflet()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

int	Preflet::Create(UINT nIDTemplate, CWnd * pParentWnd)
{
	atoms().createControls();
	return CDialog::Create(nIDTemplate, pParentWnd);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	Preflet::OnDestroy()
{
	CDialog::OnDestroy();

	atoms().destroyControls();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	Preflet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	for (PrefletAtomMap::iterator i = atoms().begin(); i != atoms().end(); ++i)
	{
		wstring	name = i->first;
		PrefletAtom &	atom = i->second;
		if (atom.controlType() != PrefletAtom::ControlType::None)
		{
			ASSERT(atom.control() && atom.controlID());
			DDX_Control(pDX, atom.controlID(), *atom.control());
		}
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	Preflet::OnPreInitPreflet()
{
	// Override this member for preflet atom initialization (before controls have been set to their defaults)
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	Preflet::OnPostInitPreflet()
{
	// Override this member for preflet atom initialization (after controls have been set to their defaults)
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	Preflet::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Give the dialog a chance to initialize itself

	OnPreInitPreflet();

	// Update our controls

	atoms().writeToControls();

	// Another chance to init the dialog, post initialization

	OnPostInitPreflet();

	return TRUE;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	Preflet::update()
{
	atoms().writeToRegistry(registryKey());
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	Preflet::apply()
{
	// Apply the changes

	atoms().readFromControls();
	update();
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	Preflet::name()
{
	// Return the name of the preflet (i.e., the last part of the registry key)

	wstring			name = registryKey();
	wstring::size_type	idx = name.rfind(_T("\\"));
	if (idx != wstring::npos) name = name.substr(idx+1);

	return name;
}

// ---------------------------------------------------------------------------------------------------------------------------------
// Preflet.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
