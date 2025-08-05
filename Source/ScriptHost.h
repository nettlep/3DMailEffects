// ---------------------------------------------------------------------------------------------------------------------------------
//   _____           _       _   _    _           _       _     
//  / ____|         (_)     | | | |  | |         | |     | |    
// | (___   ___ _ __ _ _ __ | |_| |__| | ___  ___| |_    | |__  
//  \___ \ / __| '__| | '_ \| __|  __  |/ _ \/ __| __|   | '_ \ 
//  ____) | (__| |  | | |_) | |_| |  | | (_) \__ \ |_  _ | | | |
// |_____/ \___|_|  |_| .__/ \__|_|  |_|\___/|___/\__|(_)|_| |_|
//                    | |                                       
//                    |_|                                       
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

#ifndef	_H_ScriptHost
#define _H_ScriptHost

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

#include "activscp.h"
#include "scriptSkin.h"

// ---------------------------------------------------------------------------------------------------------------------------------

class	ScriptHost : public IActiveScriptSite
{
public:
	// Construction/Destruction

					ScriptHost();
virtual					~ScriptHost();

	// Operators

	// Implementation

virtual		wstring			init();
virtual		wstring			run();
virtual		wstring			quit();

	// IUnknown

STDMETHODIMP				QueryInterface(REFIID riid, void * * ppvObj);
STDMETHODIMP_(ULONG)			AddRef();
STDMETHODIMP_(ULONG)			Release();

	// IActiveScriptHost

STDMETHODIMP				GetLCID(LCID *plcid);
STDMETHODIMP				GetItemInfo(LPCOLESTR pstrName, DWORD dwReturnMask, IUnknown **ppunkItem, ITypeInfo **ppTypeInfo);
STDMETHODIMP				GetDocVersionString(BSTR *pbstrVersionString);
STDMETHODIMP				OnScriptTerminate(const VARIANT *pvarResult, const EXCEPINFO *pexcepinfo);
STDMETHODIMP				OnStateChange(SCRIPTSTATE ssScriptState);
STDMETHODIMP				OnScriptError(IActiveScriptError *pase);
STDMETHODIMP				OnEnterScript(void);
STDMETHODIMP				OnLeaveScript(void);

	// Accessors

inline		int &			refCount()			{return _refCount;}
inline	const	int			refCount() const		{return _refCount;}
inline		IActiveScript *&	engine()			{return _engine;}
inline	const	IActiveScript *		engine() const			{return _engine;}
inline		CLSID &			engineCLSID()			{return _engineCLSID;}
inline	const	CLSID &			engineCLSID() const		{return _engineCLSID;}
inline		IActiveScriptParse *&	parser()			{return _parser;}
inline	const	IActiveScriptParse *	parser() const			{return _parser;}
inline		wstring &		script()			{return _script;}
inline	const	wstring &		script() const			{return _script;}
inline		IScriptSkin *&		scriptSkin()			{return _scriptSkin;}
inline	const	IScriptSkin *		scriptSkin() const		{return _scriptSkin;}
inline		CLSID &			scriptSkinCLSID()		{return _scriptSkinCLSID;}
inline	const	CLSID &			scriptSkinCLSID() const		{return _scriptSkinCLSID;}

private:
	// Data members

	int				_refCount;
	IActiveScript *			_engine;
	CLSID				_engineCLSID;
	IActiveScriptParse *		_parser;
	wstring				_script;
	IScriptSkin *			_scriptSkin;
	CLSID				_scriptSkinCLSID;
};

#endif // _H_ScriptHost
// ---------------------------------------------------------------------------------------------------------------------------------
// ScriptHost.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
