// ---------------------------------------------------------------------------------------------------------------------------------
//   _____       _                          
//  / ____|     | |                         
// | |      __ _| |__       ___ _ __  _ __  
// | |     / _` | '_ \     / __| '_ \| '_ \ 
// | |____| (_| | |_) | _ | (__| |_) | |_) |
//  \_____|\__,_|_.__/ (_) \___| .__/| .__/ 
//                             | |   | |    
//                             |_|   |_|    
//
// Cab file management
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 02/09/2001 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2001, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "3Dme.h"
#include "cabsdk\include\fdi.h"
#include "Cab.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

static	FNALLOC(memAlloc)	{return malloc(cb);}
static	FNFREE(memFree)		{free(pv);}
static	FNOPEN(fileOpen)	{return _open(pszFile, oflag, pmode);}
static	FNREAD(fileRead)	{return _read(hf, pv, cb);}
static	FNWRITE(fileWrite)	{return _write(hf, pv, cb);}
static	FNCLOSE(fileClose)	{return _close(hf);}
static	FNSEEK(fileSeek)	{return _lseek(hf, dist, seektype);}

// ---------------------------------------------------------------------------------------------------------------------------------

static	void	createPath(const wstring &basePath, const wstring &path)
{
	wstring	temp = path;
	wstring	accumPath;
	while(temp.length())
	{
		wstring::size_type idx = temp.find(_T("\\"));
		if (idx == wstring::npos) break;

		wstring	curPath = temp.substr(0, idx);
		temp.erase(0, idx + 1);

		accumPath += _T("\\") + curPath;
		curPath = basePath + accumPath;
		_wmkdir(curPath.c_str());
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

static	FNFDINOTIFY(notificationFunction)
{
	switch (fdint)
	{
		case fdintCOPY_FILE:	// file to be copied
			{
				// Who is it?

				wstring	dest = theApp.programPath() + atow(pfdin->psz1);
				createPath(theApp.programPath(), atow(pfdin->psz1));

				// Modify it from '3dme.exe' to '_3dme.exe' if necessary

				wstring	temp = dest;
				for (unsigned int i = 0; i < temp.length(); i++) temp[i] = towupper(temp[i]);

				// Note that we specifically don't update the UPD.EXE program to a temp file, since the UPD program
				// is reponsible for renaming files, and it can't rename itself.

				if (temp.find(_T(".EXE")) != wstring::npos && temp.find(_T("UPD.EXE")) == wstring::npos)
				{
					wstring::size_type idx = temp.rfind(_T("\\"));
					if (idx == wstring::npos) idx = 0;
					else	idx++;
					dest.insert(idx, _T("_"));
				}
				else if (temp.find(_T(".DLL")) != wstring::npos)
				{
					wstring::size_type idx = temp.rfind(_T("\\"));
					if (idx == wstring::npos) idx = 0;
					else	idx++;
					dest.insert(idx, _T("_"));
				}

				// Open it

				int	ret = fileOpen((char *) wtoa(dest).c_str(), _O_BINARY | _O_CREAT | _O_TRUNC | _O_WRONLY | _O_SEQUENTIAL, _S_IREAD | _S_IWRITE);

				// Track errors

				if (ret == -1)
				{
					string	err = strerror(errno);
					LOG(string("Unable to open file for writing: ") + wtoa(dest) + string("\n\n--> ") + err, Logger::LOG_CRIT);
				}
				return ret;
			}

		case fdintCLOSE_FILE_INFO:	// close the file, set relevant info
			{
				// Close it

				fileClose(pfdin->hf);

				// Who is it?

				wstring	dest = theApp.programPath() + atow(pfdin->psz1);

				// Modify it from '3dme.exe' to '_3dme.exe' if necessary

				wstring	temp = dest;
				for (unsigned int i = 0; i < temp.length(); i++) temp[i] = towupper(temp[i]);
				wstring::size_type idx = temp.find(_T("3DME.EXE"));
				if (idx != wstring::npos) dest.insert(idx, _T("_"));

				// Set date/time

				HANDLE  handle = CreateFile(dest.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

				if (handle != INVALID_HANDLE_VALUE)
				{
					FILETIME    datetime;
					if (DosDateTimeToFileTime(pfdin->date, pfdin->time, &datetime))
					{
						FILETIME	local_filetime;

						if (TRUE == LocalFileTimeToFileTime(&datetime, &local_filetime))
						{
							(void) SetFileTime(handle, &local_filetime, NULL, &local_filetime);
						}
					}

					CloseHandle(handle);
				}

				// Mask out attribute bits other than readonly, hidden, system, and archive, since the other
				// attribute bits are reserved for use by the cabinet format.

				DWORD   attrs = pfdin->attribs;
				attrs &= (_A_RDONLY | _A_HIDDEN | _A_SYSTEM | _A_ARCH);

				(void) SetFileAttributes(dest.c_str(), attrs);

				return 1;
			}

		case fdintCABINET_INFO: // general information about the cabinet
			return 0;

		case fdintPARTIAL_FILE: // first file in cabinet is continuation
		case fdintNEXT_CABINET:	// file continued to next cabinet
			return -1;
	}

	return 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

static	wstring	returnFdiErrorerrorString(FDIERROR err)
{
	switch (err)
	{
		case FDIERROR_NONE:			return _T("[") + integerToWString(err) + _T("] No error");
		case FDIERROR_CABINET_NOT_FOUND:	return _T("[") + integerToWString(err) + _T("] Cabinet not found");
		case FDIERROR_NOT_A_CABINET:		return _T("[") + integerToWString(err) + _T("] Not a cabinet");
		case FDIERROR_UNKNOWN_CABINET_VERSION:	return _T("[") + integerToWString(err) + _T("] Unknown cabinet version");
		case FDIERROR_CORRUPT_CABINET:		return _T("[") + integerToWString(err) + _T("] Corrupt cabinet");
		case FDIERROR_ALLOC_FAIL:		return _T("[") + integerToWString(err) + _T("] Memory allocation failed");
		case FDIERROR_BAD_COMPR_TYPE:		return _T("[") + integerToWString(err) + _T("] Unknown compression type");
		case FDIERROR_MDI_FAIL:			return _T("[") + integerToWString(err) + _T("] Failure decompressing data");
		case FDIERROR_TARGET_FILE:		return _T("[") + integerToWString(err) + _T("] Failure writing to target file");
		case FDIERROR_RESERVE_MISMATCH:		return _T("[") + integerToWString(err) + _T("] Cabinets in set have different RESERVE sizes");
		case FDIERROR_WRONG_CABINET:		return _T("[") + integerToWString(err) + _T("] Cabinet returned on fdintNEXT_CABINET is incorrect");
		case FDIERROR_USER_ABORT:		return _T("[") + integerToWString(err) + _T("] User aborted");
		default:				return _T("[") + integerToWString(err) + _T("] Unknown error");
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	DecryptCabFile(const wstring &cabinetFullpath)
{
	ERF	erf;
	HFDI	hfdi = FDICreate(memAlloc, memFree, fileOpen, fileRead, fileWrite, fileClose, fileSeek, cpu80386, &erf);
	if (hfdi == NULL)	return _T("FDICreate() failed: ") + returnFdiErrorerrorString((FDIERROR) erf.erfOper);

	// Is this file really a cabinet?

	int	hf = fileOpen((char *) wtoa(cabinetFullpath).c_str(), _O_BINARY | _O_RDONLY | _O_SEQUENTIAL, 0);
	if (hf == -1)
	{
		(void) FDIDestroy(hfdi);
		return _T("Unable to open '") + cabinetFullpath + _T(" for input");
	}

	FDICABINETINFO	fdici;
	if (!FDIIsCabinet(hfdi, hf, &fdici))
	{
		// Not a cabinet

		_close(hf);
		(void) FDIDestroy(hfdi);

		return _T("FDIIsCabinet() failed: ") + cabinetFullpath;
	}
	else
	{
		_close(hf);
	}

	wstring	cabinetName = cabinetFullpath;
	wstring	cabinetPath;
	wstring::size_type	idx = cabinetFullpath.rfind(_T("\\"));
	if (idx != wstring::npos)
	{
		cabinetName = cabinetFullpath.substr(idx + 1);
		cabinetPath = cabinetFullpath.substr(0, idx + 1);
	}

	if (!FDICopy(hfdi, (char *) wtoa(cabinetName).c_str(), (char *) wtoa(cabinetPath).c_str(), 0, notificationFunction, NULL, NULL))
	{
		wstring	errString = _T("FDICopy() failed: ") + returnFdiErrorerrorString((FDIERROR) erf.erfOper) + _T("\n\nCheck log file for more specific error information.");
		(void) FDIDestroy(hfdi);
		return errString;
	}

	if (!FDIDestroy(hfdi))
	{
		return _T("FDIDestroy() failed: ") + returnFdiErrorerrorString((FDIERROR) erf.erfOper);
	}

	return _T("");
}

// ---------------------------------------------------------------------------------------------------------------------------------
// Cab.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

