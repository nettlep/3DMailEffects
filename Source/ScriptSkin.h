// ScriptSkin.h : Declaration of the ScriptSkin

#pragma once
#include "resource.h"       // main symbols

#include "3Dme.h"


// ScriptSkin

class ATL_NO_VTABLE ScriptSkin : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<ScriptSkin, &CLSID_ScriptSkin>,
	public IDispatchImpl<IScriptSkin, &IID_IScriptSkin, &LIBID_FS3DMELib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	ScriptSkin()
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_SCRIPTSKIN)
	BEGIN_COM_MAP(ScriptSkin)
		COM_INTERFACE_ENTRY(IScriptSkin)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

public:

	STDMETHOD(TestCallback)(BSTR str);
};

OBJECT_ENTRY_AUTO(__uuidof(ScriptSkin), ScriptSkin)
