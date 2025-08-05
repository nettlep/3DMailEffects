// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _       _     
// |  __ \           / _| |    | |     | |    
// | |__) |_ __  ___| |_| | ___| |_    | |__  
// |  ___/| '__|/ _ \  _| |/ _ \ __|   | '_ \ 
// | |    | |  |  __/ | | |  __/ |_  _ | | | |
// |_|    |_|   \___|_| |_|\___|\__|(_)|_| |_|
//                                            
//                                            
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

#ifndef	_H_PREFLET
#define _H_PREFLET

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

#include "PrefletAtomMap.h"

// ---------------------------------------------------------------------------------------------------------------------------------

class	Preflet : public CDialog
{
public:
	// Construction/Destruction

					Preflet();
					Preflet(const Preflet & rhs) {operator=(rhs);}
					Preflet(const int idd, const wstring & registryKey, CWnd* pParent = NULL);
virtual					~Preflet();

	// Operators

inline		Preflet &		operator =(const Preflet & rhs)
					{
						if (this != &rhs)
						{
							idd() = rhs.idd();
							registryKey() = rhs.registryKey();
							atoms() = rhs.atoms();
							item() = rhs.item();
						}
						return *this;
					}

inline		PrefletAtom &		operator[](const wstring & idx)
					{
						ASSERT(atoms().find(idx) != atoms().end());
						return atoms()[idx];
					}

	// Implementation

virtual		int			Create(UINT nIDTemplate, CWnd * pParentWnd = 0);
afx_msg		void			OnDestroy();
virtual		void			DoDataExchange(CDataExchange* pDX);
virtual		BOOL			OnInitDialog();
virtual		void			OnPreInitPreflet();
virtual		void			OnPostInitPreflet();
virtual		void			update();
virtual		void			apply();
virtual		wstring			name();

	// Accessors

inline		int &			idd()			{return _idd;}
inline	const	int			idd() const		{return _idd;}
inline		HTREEITEM &		item()			{return _item;}
inline	const	HTREEITEM		item() const		{return _item;}
inline		wstring &		registryKey()		{return _registryKey;}
inline	const	wstring &		registryKey() const	{return _registryKey;}
inline		PrefletAtomMap &	atoms()			{return _atoms;}
inline	const	PrefletAtomMap &	atoms() const		{return _atoms;}

private:
	// Data members

		int			_idd;
		HTREEITEM		_item;
		wstring			_registryKey;
		PrefletAtomMap		_atoms;

					DECLARE_DYNAMIC(Preflet)
					DECLARE_MESSAGE_MAP()
};

#endif // _H_PREFLET
// ---------------------------------------------------------------------------------------------------------------------------------
// Preflet.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------