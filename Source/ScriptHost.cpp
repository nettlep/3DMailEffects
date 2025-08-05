// ---------------------------------------------------------------------------------------------------------------------------------
//   _____           _       _   _    _           _                        
//  / ____|         (_)     | | | |  | |         | |                       
// | (___   ___ _ __ _ _ __ | |_| |__| | ___  ___| |_      ___ _ __  _ __  
//  \___ \ / __| '__| | '_ \| __|  __  |/ _ \/ __| __|    / __| '_ \| '_ \ 
//  ____) | (__| |  | | |_) | |_| |  | | (_) \__ \ |_  _ | (__| |_) | |_) |
// |_____/ \___|_|  |_| .__/ \__|_|  |_|\___/|___/\__|(_) \___| .__/| .__/ 
//                    | |                                     | |   | |    
//                    |_|                                     |_|   |_|    
//
//   Implements IActiveScriptSite
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
#include "ScriptHost.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <mshtml.h>

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

static	void ShowBSTR(char *prompt, BSTR pbStr)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

	ScriptHost::ScriptHost()
	: _refCount(0), _engine(NULL), _parser(NULL), _scriptSkin(NULL)
{
	CLSIDFromProgID(_T("JScript"), &engineCLSID());
	CLSIDFromProgID(_T("FS3DME.ScriptSkin"), &scriptSkinCLSID());
}

// ---------------------------------------------------------------------------------------------------------------------------------

	ScriptHost::~ScriptHost()
{
	if (engine())
	{
		engine()->Release();
		engine() = NULL;
	}

	if (parser())
	{
		parser()->Release();
		parser() = NULL;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	ScriptHost::init()
{
	// First, create the scripting engine with a call to CoCreateInstance, placing the created engine in engine()

	HRESULT hr = CoCreateInstance(engineCLSID(), NULL, CLSCTX_INPROC_SERVER, IID_IActiveScript, (void **)&engine());
	if (FAILED(hr)) return _T("Failed to create scripting engine");

	// Now query for the IActiveScriptParse interface of the engine

	hr = engine()->QueryInterface(IID_IActiveScriptParse, (void**)&parser());
	if (FAILED(hr)) return _T("Engine doesn't support IActiveScriptParse");

	// The engine needs to know the host it runs on.

	hr = engine()->SetScriptSite(this);
	if (FAILED(hr)) return _T("Error calling SetScriptSite");

	// Add my internal stuff to the engine

	hr = engine()->AddNamedItem(_T("skin"), SCRIPTITEM_ISVISIBLE);
	if (FAILED(hr)) return _T("Error calling AddNamedItem");

	// Initialize the script engine so it's ready to run.

	hr = parser()->InitNew();
	if (FAILED(hr)) return _T("Error calling InitNew");

#if 1
char	scr[] = "\
function sendMail(recipient)\n\
{\n\
	skin.TestCallback('sending email to: ' + recipient);\n\
//	skin.testProperty = '10';\n\
	cdoConfig = new ActiveXObject('CDO.Configuration');\n\
	cdoConfig.Fields.Item('http://schemas.microsoft.com/cdo/configuration/sendusing') = 1;\n\
	cdoConfig.Fields.Item('http://schemas.microsoft.com/cdo/configuration/smtpserver') = '10.0.0.1';\n\
	cdoConfig.Fields.Update();\n\
	cdoMessage = new ActiveXObject('CDO.Message');\n\
	cdoMessage.Configuration = cdoConfig;\n\
	cdoMessage.From = '3dme@yourcomputer.com';\n\
	cdoMessage.To = recipient;\n\
	cdoMessage.Subject = 'Test from within 3DME...';\n\
	cdoMessage.TextBody = \"This email is being sent from within 3DME using the new Javascript support. This is, in effect, an automated email being sent from within 3DME as a test.\\n\
\\n\
Javascript will be used heavily in the new 3DME Pro. For example, you can write Javascript that gets run whenever \
an email arrives. With something like this, you can create custom programmable filters for emails. If you're clever, \
you might keep watch for an important email and notify yourself immediately by sending a text message to your mobile \
phone with the contents of the email.\\n\
\\n\
With Javascript support threaded completely through 3DME Pro, you can do \
whatever you want. If you want 3DME to act more like an email client, you can add the ability to send emails to 3DME. \
That's a pretty powerful feature for a user to be able to add to a program!\\n\
\\n\
And these are just a couple tiny examples \
of what users will do with 3DME. Hopefully, they'll share their scripts with one another, so those that don't care to code \
in JavaScript can still enjoy these features. Cool, huh? :)\";\n\
	cdoMessage.Send();\n\
}\n\
//sendMail('peter.a.parker@ntlworld.com');\n\
//sendMail('mkhopper@Depotek.com');\n\
sendMail('midnight@FluidStudios.com');\n\
";
#else
char	scr[] = "skin.TestCallback();";
#endif

	script() = atow(scr);

	return _T("");
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	 ScriptHost::run()
{
	// Pass the script to be run to the script engine with a call to ParseScriptText

	HRESULT hr = parser()->ParseScriptText(script().c_str(), _T(""), NULL, NULL, 0, 0, SCRIPTTEXT_ISVISIBLE, NULL, NULL);
	if (FAILED(hr)) return _T("Error calling ParseScriptText");

	// Tell the engine to start processing the script with a call to SetScriptState().

	hr = engine()->SetScriptState(SCRIPTSTATE_CONNECTED);
	if (FAILED(hr)) return _T("Error calling SetScriptState");

//	hr = parser()->ParseScriptText(_T("sendMail('midnight@graphicspapers.com');"), _T(""), NULL, NULL, 0, 0, SCRIPTTEXT_ISEXPRESSION, NULL, NULL);
//	if (FAILED(hr)) return _T("Error calling ParseScriptText");

//	hr = parser()->ParseScriptText(_T("sendMail('paul@graphicspapers.com');"), _T(""), NULL, NULL, 0, 0, SCRIPTTEXT_ISEXPRESSION, NULL, NULL);
//	if (FAILED(hr)) return _T("Error calling ParseScriptText");

	return _T("");
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	ScriptHost::quit()
{
	HRESULT hr = engine()->Close();
	if (FAILED(hr)) return _T("Error calling Close");

	return _T("");
}

// ---------------------------------------------------------------------------------------------------------------------------------

STDMETHODIMP ScriptHost::QueryInterface(REFIID riid, void ** ppvObj)
{
	if (riid == IID_IUnknown)
	{
		*ppvObj = static_cast<IActiveScriptSite*>(this);
	}
	else if (riid == IID_IActiveScriptSite)
	{
		*ppvObj = static_cast<IActiveScriptSite*>(this);
	}
	else
	{
		*ppvObj = NULL;
		return E_NOINTERFACE;
	}

	static_cast<IUnknown*>(*ppvObj)->AddRef();

	return S_OK;
}

// ---------------------------------------------------------------------------------------------------------------------------------

STDMETHODIMP_(ULONG) ScriptHost::AddRef()
{
	return ++refCount();
}

// ---------------------------------------------------------------------------------------------------------------------------------

STDMETHODIMP_(ULONG) ScriptHost::Release()
{
	if (--refCount() == 0)
	{
		delete this;
		return 0;
	}
	return refCount();
}

// ---------------------------------------------------------------------------------------------------------------------------------

STDMETHODIMP ScriptHost::GetLCID( LCID *plcid )
{
	return E_NOTIMPL;
}

// ---------------------------------------------------------------------------------------------------------------------------------

STDMETHODIMP ScriptHost::GetItemInfo(LPCOLESTR pstrName, DWORD dwReturnMask, IUnknown **ppunkItem, ITypeInfo **ppTypeInfo)
{
	// Use logical ANDs to determine which type(s) of pointer the caller wants, and make sure that that placeholder is currently
	// valid.

	if (dwReturnMask & SCRIPTINFO_IUNKNOWN)
	{
		if (!ppunkItem) return E_INVALIDARG;
		*ppunkItem = NULL;

		if (wstring(pstrName) == wstring(_T("skin")))
		{
			static	CComObject<ScriptSkin>* ptr = NULL;

			if (!ptr)
			{
				HRESULT	hr = CComObject<ScriptSkin>::CreateInstance(&ptr);
				if (!ptr || hr != S_OK) return TYPE_E_ELEMENTNOTFOUND;
			}

			HRESULT	hr = ptr->QueryInterface(IID_IUnknown, (void**)ppunkItem);
			if (hr != S_OK) return TYPE_E_ELEMENTNOTFOUND;

			return S_OK;
		}
	}

	if (dwReturnMask & SCRIPTINFO_ITYPEINFO)
	{
		if (!ppTypeInfo) return E_INVALIDARG;
		*ppTypeInfo = NULL;
	}

	return TYPE_E_ELEMENTNOTFOUND;
}

// ---------------------------------------------------------------------------------------------------------------------------------

STDMETHODIMP ScriptHost::GetDocVersionString(BSTR *pbstrVersionString)
{
	return E_NOTIMPL;
}

// ---------------------------------------------------------------------------------------------------------------------------------

STDMETHODIMP ScriptHost::OnScriptTerminate(const VARIANT *pvarResult, const EXCEPINFO *pexcepinfo)
{
	return S_OK;
}

// ---------------------------------------------------------------------------------------------------------------------------------

STDMETHODIMP ScriptHost::OnStateChange( SCRIPTSTATE ssScriptState)
{
	// If something needs to happen when the script enters a certain state, put it here.

	switch (ssScriptState){
		case SCRIPTSTATE_UNINITIALIZED:
			break;
		case SCRIPTSTATE_INITIALIZED:
			break;
		case SCRIPTSTATE_STARTED:
			break;
		case SCRIPTSTATE_CONNECTED:
			break;
		case SCRIPTSTATE_DISCONNECTED:
			break;
		case SCRIPTSTATE_CLOSED:
			break;
		default:
			break;
	}

	return S_OK;
}

// ---------------------------------------------------------------------------------------------------------------------------------

STDMETHODIMP ScriptHost::OnScriptError(IActiveScriptError *pase)
{
	EXCEPINFO exception;
	HRESULT hr = pase->GetExceptionInfo(&exception);
	if (SUCCEEDED(hr))
	{
		BSTR	sourceLine = 0;
		pase->GetSourceLineText(&sourceLine);
		if (sourceLine == 0) sourceLine = _T("[[Source line Unspecified]]");
		DWORD	context, lineNumber;
		LONG	charPosition;
		pase->GetSourcePosition(&context, &lineNumber, &charPosition);

		wstring	errText;

		if (exception.bstrDescription)
		{
			errText = _T("Error on line ") + integerToWString(lineNumber+1) + _T(" at character ") + integerToWString(charPosition) +
			_T("\r\n\r\n") + sourceLine + _T("\r\n\r\n") + exception.bstrDescription;
		}
		else
		{
			wchar_t	tmp[25];
			swprintf(tmp, _T("0x%08X"), exception.scode);
			errText = wstring(_T("Error code: ")) + tmp;
		}

		::MessageBox(NULL, errText.c_str(), _T("Script error"), MB_SETFOREGROUND | MB_OK);
	}

	return S_OK;
}

// ---------------------------------------------------------------------------------------------------------------------------------

STDMETHODIMP ScriptHost::OnEnterScript(void)
{
	return S_OK;
}

// ---------------------------------------------------------------------------------------------------------------------------------

STDMETHODIMP ScriptHost::OnLeaveScript(void)
{
	return S_OK;
}

// ---------------------------------------------------------------------------------------------------------------------------------
// ScriptHost.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
