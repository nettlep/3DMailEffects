// ---------------------------------------------------------------------------------------------------------------------------------
//  ____  _____                                      
// |___ \|  __ \                                     
//   __) | |  | |_ __ ___   ___      ___ _ __  _ __  
//  |__ <| |  | | '_ ` _ \ / _ \    / __| '_ \| '_ \ 
//  ___) | |__| | | | | | |  __/ _ | (__| |_) | |_) |
// |____/|_____/|_| |_| |_|\___|(_) \___| .__/| .__/ 
//                                      | |   | |    
//                                      |_|   |_|    
//
// 3D Mail Effects
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 11/16/2000 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2000, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include ".\3dme.h"
#include "Bitmap.h"
#include "EmailListbox.h"
#include "ThreeDlg.h"
#include "AccountList.h"
#include "3DmeDlg.h"
#include "RegDlg.h"
#include "Cab.h"
#include "comdef.h"
#include "StructuredException.h"
#include "fubar_off.h"  // Specifically turn this off, because the key hasn't been loaded yet!
#include "script.h"
#include <initguid.h>
#include "3Dme_i.c"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------


class CDmeModule :
	public CAtlMfcModule
{
public:
	DECLARE_LIBID(LIBID_FS3DMELib);
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_FS3DME, "{D8B0425E-6639-4CA8-9507-83137B67D944}");};

CDmeModule _AtlModule;

C3DmeApp	theApp;

// ---------------------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(C3DmeApp, CWinApp)
	//{{AFX_MSG_MAP(C3DmeApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	C3DmeApp::C3DmeApp()
{
}

// --------------------------------------------------------------------------------------------------------------------------------

static BOOL CALLBACK getApplicationHandleProc(HWND hwnd, LPARAM lParam)
{
	wchar_t	str[MAX_PATH];
	memset(str, 0, sizeof(str));
	::GetWindowText(hwnd, str, MAX_PATH - 1);
	if (!wcsicmp(str, PROGRAM_NAME_STRING _T(" ")))
	{
		if (lParam)
		{
			int	*t = (int *) lParam;
			*t = 1;
		}
		::PostMessage(hwnd, WM_COMMAND, MENU_VIEW_MAILS, 0);
		return FALSE;
	}

	return TRUE;
}

// ---------------------------------------------------------------------------------------------------------------------------------

#if	defined(_REGISTERED_)
static	bool	validateChecksum(const unsigned char *key)
{
	unsigned short	*ptr = (unsigned short *) key;
	unsigned short	sum = (key[14] << 8) | key[15];

	// NULL checksums should never work... (thanks Tigo)

	if (!sum) return false;

	for (int i = 0; i < 7; i++, ptr++)
	{
		// At least each 16-bit value in the key should have at least one bit set

		if (!*ptr) return false;

		// Checksum

		sum -= *ptr;
	}

	// Valid checksum?

	return sum == 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

static	void	makeCopies(unsigned char *dst, const unsigned char *src, const unsigned int size, const unsigned int count)
{
	for (unsigned int i = 0; i < count; i++, dst += size)
	{
		memcpy(dst, src, size);
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

static	unsigned char	*convertToBinary(const string &key)
{
	// Working copy

	string	temp = key;

	// Strip out anything that's not a valid key value

	for(;;)
	{
		string::size_type	idx = temp.find_first_not_of("0123456789ABCDEFabcdef");
		if (idx == string::npos) break;
		temp.erase(idx, 1);
	}

	// The key should be exactly 32 bytes long

	if (temp.length() != 32) return NULL;

	// Parse it into a series of 16 binary bytes

	int		index = 0;
static	unsigned char	result[16];
	while(temp.length() >= 2)
	{
		unsigned char	nibble0 = tolower(temp[0]);
		     if (nibble0 >= '0' && nibble0 <= '9') nibble0 -= '0';
		else if (nibble0 >= 'a' && nibble0 <= 'f') nibble0 -= 'a' - 10;
		else nibble0 = 0;

		unsigned char	nibble1 = tolower(temp[1]);
		     if (nibble1 >= '0' && nibble1 <= '9') nibble1 -= '0';
		else if (nibble1 >= 'a' && nibble1 <= 'f') nibble1 -= 'a' - 10;
		else nibble1 = 0;

		temp.erase(0, 2);

		result[index++] = ((nibble0 << 4) & 0xf0) | (nibble1 & 0xf);
	}

	return result;
}

// ---------------------------------------------------------------------------------------------------------------------------------

static	bool	checkKey()
{
	int	newKey = 0;
	for(;;)
	{
		wstring	key = theApp.GetProfileString(_T("Other"), _T("key"));

		// Is there a key?

		if (key.length())
		{
			// Convert it to binary

			unsigned char	*binKey = convertToBinary(wtoa(key));
			if (binKey)
			{
				// Is the checksum okay?

				if (validateChecksum(binKey))
				{
					makeCopies(oneHundredKeys, binKey, 16, 100);
					if (newKey) AfxMessageBox(_T("Thank you for registering ") PROGRAM_NAME_STRING _T("."));
					return true;
				}
			}
		}

		// If we get here, and we have a key with a length, then it's an invalid key

		if (key.length())
		{
			AfxMessageBox(_T("The registration code you have entered is invalid.\n"));
		}

		// Ask them for a key

		RegDlg	dlg;
		dlg.regCode = key.c_str();
		if (dlg.DoModal() != IDOK)
		{
			// Cancelled the dialog box

			AfxMessageBox(_T("This version of ") PROGRAM_NAME_STRING _T(" requires a registration code\n")
				      _T("in order to run properly. You will need to rerun the program\n")
				      _T("and enter a valid registration code."));
			return false;
		}

		// We have a new key

		newKey = 1;

		// Put the key in the registry

		theApp.WriteProfileString(_T("Other"), _T("key"), dlg.regCode);
	}

	return false;
}
#endif // _REGISTERED_

// ---------------------------------------------------------------------------------------------------------------------------------
// This function also exists in utils.cpp, but it's wrapped with fubar
// ---------------------------------------------------------------------------------------------------------------------------------

static	string::size_type	findNocase(const string &s1, const string &s2, const string::size_type index)
{
	string	_s1 = s1;
	lowerString(_s1);

	string	_s2 = s2;
	lowerString(_s2);

	if (index != 0xffffffff)	return _s1.find(_s2, index);
	else				return _s1.find(_s2);
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	C3DmeApp::InitInstance()
{
	AfxOleInit();

	// InitCommonControls() is required on Windows XP if an application manifest specifies use of ComCtl32.dll version 6 or
	// later to enable visual styles.  Otherwise, any window creation will fail.

	InitCommonControls();

	// Get the program path

	programFilename() = GetCommandLine();

	// Strip the quotes from the filename

	if (programFilename()[0] == '"')
	{
		programFilename().erase(0, 1);
		wstring::size_type	idx = programFilename().find(_T("\""));
		if (idx != string::npos) programFilename().erase(idx);
	}

	// Strip everything past the .exe

	{
		wstring::size_type	idx = findNocase(programFilename(), _T(".exe"), 0);
		if (idx != string::npos) programFilename().erase(idx + 4);
	}

	// Strip the path from the filename

	programPath() = programFilename();
	wstring::size_type	idx = programPath().find_last_of(_T("\\"));
	if (idx == string::npos)	programPath().erase();
	else				programPath().erase(idx+1);

	// Clear the temp files on entry

	clearTempFiles();

	// "This is who we are" (Millenium fans go nutz here :)

	SetRegistryKey(_T("Fluid Studios"));
	free(const_cast<void *>(static_cast<const void *>(theApp.m_pszProfileName)));
	theApp.m_pszProfileName = _tcsdup(_T("3D Mail Effects"));

	// Get the key, if it exists

#if	defined(_REGISTERED_)
	if (!checkKey()) return FALSE;
#endif // _REGISTERED_

	// Init the prefs

	prefs().init();

	// For the rich-edit control

	AfxEnableControlContainer();

	// Register class factories via CoRegisterClassObject().

	if (FAILED(_AtlModule.RegisterClassObjects(CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE))) return FALSE;

	// Register the COM server

	_AtlModule.UpdateRegistryAppId(TRUE);
	_AtlModule.RegisterServer(TRUE);

//Script	foo;

	AfxInitRichEdit();

	// Setup the logger

	logger.fileSizeLimit() = 1024*1024*4;
	logger.logFile() = wtoa(programPath()) + "transaction.log";
	unlink(logger.logFile().c_str());

	// Paths

//	skinName() = theApp.prefs().skin()[_T("skinName")].stringValue();
	modelName() = theApp.prefs().skin()[_T("modelName")].stringValue();
	materialName() = theApp.prefs().skin()[_T("materialName")].stringValue();

	// Initialize the timer

	timer().calcFrequency();
	timer().startElapsedTickTimer();

	// Check for an installation
	{
		wstring	commandLine = GetCommandLine();
		wstring::size_type	idx = findNocase(commandLine, _T("--install"), 0);
		if (idx != string::npos)
		{
			// We have an installation, get the filename (strip the quotes)

			commandLine.erase(0, idx+10);
			stripLeadingWS(commandLine);
			if (commandLine[0] == _T('\"')) commandLine.erase(0, 1);
			wstring::size_type	idx = commandLine.find(_T("\""));
			if (idx != wstring::npos) commandLine.erase(idx);

			// Install the file

			wstring	result = DecryptCabFile(commandLine);
			if (result.size())
			{
				wstring	error = _T("An error has occurred trying to install the file:\n\n    ");
				error += commandLine;
				error += _T("\n\nThe error is as follows:\n\n    ");
				error += result;
				AfxMessageBox(error.c_str(), MB_ICONEXCLAMATION);
			}
			else
			{
				AfxMessageBox(_T("The enhancement has been installed correctly.\n\nYou may now make use of the enhancement."));
			}

			// Always return, never keep running

			return FALSE;
		}
	}

	// If there's another instance running, bail

	int	flag = 0;
	EnumWindows(getApplicationHandleProc, (LPARAM) &flag);
//	if (flag) return FALSE;

	// Init sockets for this thread...

	AfxSocketInit();

	// Do the dialog thang...

	StructuredException::install();

	try
	{
		try
		{
			C3DmeDlg dlg;
			dlg.DoModal();
		}
//		catch(CMemoryException & except)
//		{
//			wchar_t	err[2048];
//			memset(err, 0, sizeof(err));
//			except.GetErrorMessage(err, 2047, NULL);
//			throw_wstring(_T("CMemoryException: ") + wstring(err));
//		}
//		catch(CNotSupportedException & except)
//		{
//			wchar_t	err[2048];
//			memset(err, 0, sizeof(err));
//			except.GetErrorMessage(err, 2047, NULL);
//			throw_wstring(_T("CNotSupportedException: ") + wstring(err));
//		}
//		catch(CArchiveException & except)
//		{
//			wchar_t	err[2048];
//			memset(err, 0, sizeof(err));
//			except.GetErrorMessage(err, 2047, NULL);
//			throw_wstring(_T("CArchiveException: ") + wstring(err));
//		}
//		catch(CFileException & except)
//		{
//			wchar_t	err[2048];
//			memset(err, 0, sizeof(err));
//			except.GetErrorMessage(err, 2047, NULL);
//			throw_wstring(_T("CFileException: ") + wstring(err));
//		}
//		catch(CResourceException & except)
//		{
//			wchar_t	err[2048];
//			memset(err, 0, sizeof(err));
//			except.GetErrorMessage(err, 2047, NULL);
//			throw_wstring(_T("CResourceException: ") + wstring(err));
//		}
//		catch(COleException & except)
//		{
//			wchar_t	err[2048];
//			memset(err, 0, sizeof(err));
//			except.GetErrorMessage(err, 2047, NULL);
//			throw_wstring(_T("COleException: ") + wstring(err));
//		}
//		catch(CDBException & except)
//		{
//			wchar_t	err[2048];
//			memset(err, 0, sizeof(err));
//			except.GetErrorMessage(err, 2047, NULL);
//			throw_wstring(_T("CDBException: ") + wstring(err));
//		}
//		catch(COleDispatchException & except)
//		{
//			wchar_t	err[2048];
//			memset(err, 0, sizeof(err));
//			except.GetErrorMessage(err, 2047, NULL);
//			throw_wstring(_T("COleDispatchException: ") + wstring(err));
//		}
		catch(COleDispatchException * except)
		{
			wchar_t	err[2048];
			memset(err, 0, sizeof(err));
			except->GetErrorMessage(err, 2047, NULL);
			throw_wstring(_T("COleDispatchException: ") + wstring(err));
		}
//		catch(CUserException & except)
//		{
//			wchar_t	err[2048];
//			memset(err, 0, sizeof(err));
//			except.GetErrorMessage(err, 2047, NULL);
//			throw_wstring(_T("CUserException: ") + wstring(err));
//		}
//		catch(CDaoException & except)
//		{
//			wchar_t	err[2048];
//			memset(err, 0, sizeof(err));
//			except.GetErrorMessage(err, 2047, NULL);
//			throw_wstring(_T("CDaoException: ") + wstring(err));
//		}
//		catch(CInternetException & except)
//		{
//			wchar_t	err[2048];
//			memset(err, 0, sizeof(err));
//			except.GetErrorMessage(err, 2047, NULL);
//			throw_wstring(_T("CInternetException: ") + wstring(err));
//		}
		catch (const _com_error & err)
		{
			throw_wstring(_T("_com_error(") + wstring(err.ErrorMessage()) + _T(")"));
		}
		catch(const wstring & v)
		{
			throw_wstring(_T("wstring(") + v + _T(")"));
		}
		catch(const string & v)
		{
			throw_wstring(_T("string(") + atow(v) + _T(")"));
		}
		catch(const bool & v)
		{
			throw_wstring(_T("bool(") + wstring(v?_T("true"):_T("false")) + _T(")"));
		}
		catch(const int & v)
		{
			throw_wstring(_T("int(") + integerToWString(v) + _T(")"));
		}
		catch(const unsigned int & v)
		{
			throw_wstring(_T("unsigned int(") + integerToWString(v) + _T(")"));
		}
		catch(const char * v)
		{
			throw_wstring(_T("char *(") + atow(v) + _T(")"));
		}
		catch(const wchar_t * v)
		{
			throw_wstring(_T("wchar *(") + wstring(v) + _T(")"));
		}
		catch(std::domain_error & v)
		{
			throw_wstring(_T("domain_error: ") + atow(v.what()));
		}
		catch(std::invalid_argument & v)
		{
			throw_wstring(_T("invalid_argument: ") + atow(v.what()));
		}
		catch(std::length_error & v)
		{
			throw_wstring(_T("length_error: ") + atow(v.what()));
		}
		catch(std::out_of_range & v)
		{
			throw_wstring(_T("out_of_range: ") + atow(v.what()));
		}
		catch(std::range_error & v)
		{
			throw_wstring(_T("range_error: ") + atow(v.what()));
		}
		catch(std::overflow_error & v)
		{
			throw_wstring(_T("overflow_error: ") + atow(v.what()));
		}
		catch(std::underflow_error & v)
		{
			throw_wstring(_T("underflow_error: ") + atow(v.what()));
		}
		catch(const std::exception & except)
		{
			throw_wstring(_T("STL Exception: ") + atow(except.what()));
		}
		catch (StructuredException const & exception)
		{
			char	hexcode[20];
			sprintf(hexcode, "0x%08X", exception.what());
			char	wherecode[20];
			sprintf(wherecode, "0x%08X", exception.where());

			switch (exception.what())
			{
				case EXCEPTION_ACCESS_VIOLATION:
					throw_wstring(_T("Structured Exception [") + atow(hexcode) + _T("]: Access violation\nAt location: ") + atow(wherecode));
				case EXCEPTION_INT_DIVIDE_BY_ZERO:
					throw_wstring(_T("Structured Exception [") + atow(hexcode) + _T("]: Divide by zero\nAt location: ") + atow(wherecode));
				case EXCEPTION_STACK_OVERFLOW:
					throw_wstring(_T("Structured Exception [") + atow(hexcode) + _T("]: Stack overflow\nAt location: ") + atow(wherecode));
				default:
					throw_wstring(_T("Structured Exception [") + atow(hexcode) + _T("]: (unknown type)\nAt location: ") + atow(wherecode));
			}
		}
		catch(...)
		{
			throw_wstring(_T("unknown type(\?\?\?)"));
		}
	}
	catch (const wstring & err)
	{
		wstring	msg = 
			_T("Unhandled exception error:\n")
			_T("\n")
			_T("This error is caused by an exceptional error condition\n")
			_T("that 3DME was unable to recover from. 3DME will now shut\n")
			_T("down.\n")
			_T("\n")
			_T("We have taken every precaution in preventing this kind of\n")
			_T("error. Please contact us to let us know about this one, so\n")
			_T("we can track it down and fix it.\n")
			_T("\n")
			_T("When you contact us, it is VERY IMPORTANT that you give us\n")
			_T("the following information. Please write it down EXACTLY as it\n")
			_T("appears (if you can) and send it to us. Without this information\n")
			_T("we will be unable to locate the source of the problem. Also, it is\n")
			_T("very important that we know exactly what you were doing (within\n")
			_T("3DME) when this error occurred.\n")
			_T("\n") + err + _T("\n")
			_T("\n")
			_T("We are truly very sorry for the inconvenience.");
		AfxMessageBox(msg.c_str());
		LOG(wtoa(msg), Logger::LOG_CRIT);
	}

	// Clear the temp files on exit

	clearTempFiles();

	return FALSE;
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL C3DmeApp::ExitInstance(void)
{
	// Unregister the COM server

	_AtlModule.UpdateRegistryAppId(FALSE);
	_AtlModule.UnregisterServer(TRUE);

	_AtlModule.RevokeClassObjects();
	return CWinApp::ExitInstance();
}

// ---------------------------------------------------------------------------------------------------------------------------------
// 3Dme.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
