// ---------------------------------------------------------------------------------------------------------------------------------
//   _____           _       _                        
//  / ____|         (_)     | |                       
// | (___   ___ _ __ _ _ __ | |_      ___ _ __  _ __  
//  \___ \ / __| '__| | '_ \| __|    / __| '_ \| '_ \ 
//  ____) | (__| |  | | |_) | |_  _ | (__| |_) | |_) |
// |_____/ \___|_|  |_| .__/ \__|(_) \___| .__/| .__/ 
//                    | |                | |   | |    
//                    |_|                |_|   |_|    
//
//   Script management class for the skins
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 03/21/2004 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2004, Fluid Studios, all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "3dme.h"
#include "Script.h"

#include <objbase.h>
#include <tchar.h>

#include "ScriptHost.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

	Script::Script()
	: _host(NULL)
{
//	HMODULE h = LoadLibrary("c:\\Windows\\System32\\jscript.dll");

	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// ActiveX Script Hosting Support -- Create a new, local instance of the script hosting class

	host() = new ScriptHost();
	host()->AddRef();

	// If initialization of the script host is successful, run the script

	wstring	rc;
	rc = host()->init();

	if (rc.length())
	{
		AfxMessageBox(rc.c_str());
		return;
	}

	rc = host()->run();
	if (rc.length())
	{
		AfxMessageBox(rc.c_str());
		return;
	}

	host()->parser()->Release();
	host()->parser() = NULL;

	rc = host()->quit();
	if (rc.length())
	{
		AfxMessageBox(rc.c_str());
		return;
	}

	host()->engine()->Release();
	host()->engine() = NULL;

	host()->Release();
}

// ---------------------------------------------------------------------------------------------------------------------------------

	Script::~Script()
{
	CoUninitialize();
}


// ---------------------------------------------------------------------------------------------------------------------------------
// Script.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
