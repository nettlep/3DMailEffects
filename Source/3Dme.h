// ---------------------------------------------------------------------------------------------------------------------------------
//  ____  _____                     _     
// |___ \|  __ \                   | |    
//   __) | |  | |_ __ ___   ___    | |__  
//  |__ <| |  | | '_ ` _ \ / _ \   | '_ \ 
//  ___) | |__| | | | | | |  __/ _ | | | |
// |____/|_____/|_| |_| |_|\___|(_)|_| |_|
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

#ifndef	_H_3DME
#define _H_3DME

// ---------------------------------------------------------------------------------------------------------------------------------

#include <string>
#include <list>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <strstream>

#include <math.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <direct.h>

#include <afxsock.h>
#include <mmsystem.h>

#include <mbctype.h>

using namespace std;

// ---------------------------------------------------------------------------------------------------------------------------------
// The big "fuck you" :)
// ---------------------------------------------------------------------------------------------------------------------------------

#include "Fubar.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#define	VERSION_STRING		_T("7.0.0")
#define	BETA_VERSION_STRING	_T("7.0.0-beta")

#if	defined(_LITE_)
#define	PROGRAM_NAME_STRING	_T("3D Mail Effects LITE")
#else
#define	PROGRAM_NAME_STRING	_T("3D Mail Effects")
#endif // !_BETA_

#if	!defined(_BETA_)
#define	APP_STRING	PROGRAM_NAME_STRING _T(" v") VERSION_STRING
#else
#define	APP_STRING	PROGRAM_NAME_STRING _T(" (beta) v") BETA_VERSION_STRING
#endif // !_BETA_

#define	HTML_TITLE PROGRAM_NAME_STRING _T(" - HTML Preview")
typedef	list<string> StringList;
typedef	list<wstring> WStringList;

// ---------------------------------------------------------------------------------------------------------------------------------
// Handy, safe macro for throwing strings
// ---------------------------------------------------------------------------------------------------------------------------------

#define	throw_wstring(a)	{static wstring b; b = (a); throw b;}
#define	throw_string(a)		{static wstring b; b = (a); throw b;}

// ---------------------------------------------------------------------------------------------------------------------------------
// Files required by all modules
// ---------------------------------------------------------------------------------------------------------------------------------

#include "resource.h"
#include "logger.h"
#include "prefs.h"

// ---------------------------------------------------------------------------------------------------------------------------------
// Files required by this module
// ---------------------------------------------------------------------------------------------------------------------------------

#include "Timer"

// ---------------------------------------------------------------------------------------------------------------------------------

class	C3DmeApp : public CWinApp
{
public:
	// Construction/Destruction

					C3DmeApp();

	// Implementation

virtual		BOOL			InitInstance();

	// Accessors

inline		Prefs &			prefs()			{return _prefs;}
inline	const	Prefs &			prefs() const		{return _prefs;}

inline		wstring &		modelName()		{return _modelName;}
inline	const	wstring &		modelName() const	{return _modelName;}

inline		wstring &		materialName()		{return _materialName;}
inline	const	wstring &		materialName() const	{return _materialName;}

inline		wstring &		programPath()		{return _programPath;}
inline	const	wstring &		programPath() const	{return _programPath;}

inline		wstring	&		programFilename()	{return _programFilename;}
inline	const	wstring &		programFilename() const	{return _programFilename;}

inline		Timer &			timer()			{return _timer;}
inline	const	Timer &			timer() const		{return _timer;}

	// Utilitarian

inline	const	wstring			soundPath() const	{return programPath() + _T("Sounds\\");}
inline	const	wstring			modelPath() const	{return programPath() + _T("Models\\");}
inline	const	wstring			materialPath() const	{return programPath() + _T("Materials\\");}

protected:
	// Data members

		Prefs			_prefs;
		wstring			_soundPath;
		wstring			_programPath;
		wstring			_programFilename;
		wstring			_modelPath;
		wstring			_modelName;
		wstring			_materialPath;
		wstring			_materialName;
		Timer			_timer;

					DECLARE_MESSAGE_MAP()
public:
	BOOL ExitInstance(void);
};

// ---------------------------------------------------------------------------------------------------------------------------------
// The global application object
// ---------------------------------------------------------------------------------------------------------------------------------

extern	C3DmeApp	theApp;

// ---------------------------------------------------------------------------------------------------------------------------------
// This needs to come after theApp
// ---------------------------------------------------------------------------------------------------------------------------------

#include "utils.h"
#include "3Dme_i.h"

// ---------------------------------------------------------------------------------------------------------------------------------
//{{AFX_INSERT_LOCATION}}

#endif // _H_3DME
// ---------------------------------------------------------------------------------------------------------------------------------
// 3Dme.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
