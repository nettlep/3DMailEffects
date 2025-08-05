// ScriptSkin.cpp : Implementation of ScriptSkin

#include "stdafx.h"
#include "ScriptSkin.h"
#include ".\scriptskin.h"


// ScriptSkin


STDMETHODIMP ScriptSkin::TestCallback(BSTR str)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	AfxMessageBox(str);
	return S_OK;
}
