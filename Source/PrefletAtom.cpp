// ---------------------------------------------------------------------------------------------------------------------------------
//  _____             __ _      _            _                                        
// |  __ \           / _| |    | |     /\   | |                                       
// | |__) |_ __  ___| |_| | ___| |_   /  \  | |_  ___  _ __ ___       ___ _ __  _ __  
// |  ___/| '__|/ _ \  _| |/ _ \ __| / /\ \ | __|/ _ \| '_ ` _ \     / __| '_ \| '_ \ 
// | |    | |  |  __/ | | |  __/ |_ / ____ \| |_| (_) | | | | | | _ | (__| |_) | |_) |
// |_|    |_|   \___|_| |_|\___|\__/_/    \_\\__|\___/|_| |_| |_|(_) \___| .__/| .__/ 
//                                                                       | |   | |    
//                                                                       |_|   |_|    
//
//   Manages atoms stored in our various preferences & settings
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 08/25/2003 by Paul Nettle
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
#include "3dme.h"
#include "PrefletAtom.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAtom::createControl()
{
	destroyControl();

	switch(controlType())
	{
		case Button:
			control() = new CButton;
			break;

		case Edit:
			control() = new CEdit;
			break;

		case Slider:
			control() = new CSliderCtrl;
			break;

		case Combo:
			control() = new CComboBox;
			break;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAtom::destroyControl()
{
	delete control();
	control() = NULL;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAtom::readFromRegistry(const wstring & name, const wstring & subkey)
{
	switch(valueType())
	{
		case Boolean:
			booleanValue() = theApp.GetProfileInt(subkey.c_str(), name.c_str(), defaultBooleanValue() ? 1:0) ? true:false;
			break;
		case Integer:
			integerValue() = theApp.GetProfileInt(subkey.c_str(), name.c_str(), defaultIntegerValue());
			break;
		case Float:
			wchar_t	tmp[20];
			swprintf(tmp, _T("%f"), defaultFloatValue());
			floatValue() = static_cast<float>(_wtof(static_cast<LPCTSTR>(theApp.GetProfileString(subkey.c_str(), name.c_str(), tmp))));
			break;
		case String:
			stringValue() = theApp.GetProfileString(subkey.c_str(), name.c_str(), defaultStringValue().c_str());
			break;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAtom::writeToRegistry(const wstring & name, const wstring & subkey) const
{
	switch(valueType())
	{
		case Boolean:
			theApp.WriteProfileInt(subkey.c_str(), name.c_str(), booleanValue() ? 1:0);
			break;
		case Integer:
			theApp.WriteProfileInt(subkey.c_str(), name.c_str(), integerValue());
			break;
		case Float:
		{
			wchar_t	tmp[50];
			swprintf(tmp, _T("%f"), floatValue());
			theApp.WriteProfileString(subkey.c_str(), name.c_str(), tmp);
			break;
		}
		case String:
			theApp.WriteProfileString(subkey.c_str(), name.c_str(), stringValue().c_str());
			break;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAtom::readFromControl(const wstring & name)
{
	// Some atoms are not associated with a control

	if (controlType() == None) return;

	ASSERT(control());

	switch(controlType())
	{
		case Button:
		{
			CButton *	ptr = reinterpret_cast<CButton *>(control());
			bool		bVal = ptr->GetCheck() ? true:false;
			switch(valueType())
			{
				case Boolean:
					booleanValue() = bVal;
					break;
				case Integer:
					integerValue() = bVal ? 1:0;
					break;
				case Float:
					floatValue() = bVal ? 1.0f:0.0f;
					break;
				case String:
					stringValue() = bVal ? _T("1"):_T("0");
					break;
			}
			break;
		}
		case Edit:
		{
			CEdit *		ptr = reinterpret_cast<CEdit *>(control());
			CString		str;
			ptr->GetWindowText(str);

			wstring		sVal = static_cast<LPCTSTR>(str);
			switch(valueType())
			{
				case Boolean:
					booleanValue() = _wtoi(sVal.c_str()) ? true:false;
					break;
				case Integer:
					integerValue() = _wtoi(sVal.c_str());
					break;
				case Float:
					floatValue() = static_cast<float>(_wtof(sVal.c_str()));
					break;
				case String:
					stringValue() = sVal;
					break;
			}
			break;
		}
		case Combo:
		{
			CComboBox *	ptr = reinterpret_cast<CComboBox *>(control());
			CString		str;
			ptr->GetWindowText(str);

			wstring		sVal = static_cast<LPCTSTR>(str);
			switch(valueType())
			{
				case Boolean:
					booleanValue() = _wtoi(sVal.c_str()) ? true:false;
					break;
				case Integer:
					integerValue() = _wtoi(sVal.c_str());
					break;
				case Float:
					floatValue() = static_cast<float>(_wtof(sVal.c_str()));
					break;
				case String:
					stringValue() = sVal;
					break;
			}
			break;
		}
		case Slider:
		{
			// Can't set a slider to a float or string type!
			ASSERT(valueType() != String && valueType() != Float);

			CSliderCtrl *	ptr = reinterpret_cast<CSliderCtrl *>(control());
			int		iVal = ptr->GetPos();
			switch(valueType())
			{
				case Boolean:
					booleanValue() = iVal ? true:false;
					break;
				case Integer:
					integerValue() = iVal;
					break;
			}
			break;
		}
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	PrefletAtom::writeToControl(const wstring & name)
{
	// Some atoms are not associated with a control

	if (controlType() == None) return;

	ASSERT(control());

	switch(controlType())
	{
		case Button:
		{
			CButton *	ptr = reinterpret_cast<CButton *>(control());
			switch(valueType())
			{
				case Boolean:
					ptr->SetCheck(booleanValue() ? TRUE:FALSE);
					break;
				case Integer:
					ptr->SetCheck(integerValue() ? TRUE:FALSE);
					break;
				case Float:
					ptr->SetCheck(floatValue() ? TRUE:FALSE);
					break;
				case String:
					ptr->SetCheck(_wtoi(stringValue().c_str()) ? TRUE:FALSE);
					break;
			}
			break;
		}
		case Edit:
		{
			CEdit *		ptr = reinterpret_cast<CEdit *>(control());
			switch(valueType())
			{
				case Boolean:
					ptr->SetWindowText(booleanValue() ? _T("1"):_T("0"));
					break;
				case Integer:
					ptr->SetWindowText(integerToWString(integerValue()).c_str());
					break;
				case Float:
					ptr->SetWindowText(floatToWString(floatValue()).c_str());
					break;
				case String:
					ptr->SetWindowText(stringValue().c_str());
					break;
			}
			break;
		}
		case Combo:
		{
			CComboBox *	ptr = reinterpret_cast<CComboBox *>(control());
			switch(valueType())
			{
				case String:
					for (int i = 0; i < ptr->GetCount(); ++i)
					{
						CString	str;
						ptr->GetLBText(i, str);
						if (areEqualNocase(stringValue(), wstring(str)))
						{
							ptr->SetCurSel(i);
							break;
						}
					}
					break;
			}
			break;
		}
		case Slider:
		{
			// Can't set a slider to a float or string type!
			ASSERT(valueType() != String && valueType() != Float);

			CSliderCtrl *	ptr = reinterpret_cast<CSliderCtrl *>(control());
			ptr->SetRange(slideRangeMin(), slideRangeMax());
			switch(valueType())
			{
				case Boolean:
					ptr->SetPos(booleanValue() ? 1:0);
					break;
				case Integer:
					ptr->SetPos(integerValue());
					break;
			}
			break;
		}
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------
// PrefletAtom.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
