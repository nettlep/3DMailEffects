// ---------------------------------------------------------------------------------------------------------------------------------
//   _____ _                   _                       _ ______                      _   _                                  
//  / ____| |                 | |                     | |  ____|                    | | (_)                                 
// | (___ | |_ _ __ _   _  ___| |_ _   _ _ __  ___  __| | |__  __  __ ___  ___ _ __ | |_ _  ___  _ __       ___ _ __  _ __  
//  \___ \| __| '__| | | |/ __| __| | | | '__|/ _ \/ _` |  __| \ \/ // __|/ _ \ '_ \| __| |/ _ \| '_ \     / __| '_ \| '_ \ 
//  ____) | |_| |  | |_| | (__| |_| |_| | |  |  __/ (_| | |____ >  <| (__|  __/ |_) | |_| | (_) | | | | _ | (__| |_) | |_) |
// |_____/ \__|_|   \__,_|\___|\__|\__,_|_|   \___|\__,_|______/_/\_\\___|\___| .__/ \__|_|\___/|_| |_|(_) \___| .__/| .__/ 
//                                                                            | |                              | |   | |    
//                                                                            |_|                              |_|   |_|    
//
// Description:
//
//   Converts structured exceptions into usable C++ exceptions, ripped from MSDN "Handling Exceptions, Part 17"
//
// Notes:
//
//   Best viewed with 8-character tabs and (at least) 132 columns
//
// History:
//
//   07/30/2002 by Paul Nettle: Original creation
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2002, Fluid Studios, all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "StructuredException.h"
#include "eh.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

static void	my_translator(unsigned, EXCEPTION_POINTERS *info)
{
	throw StructuredException(*info);
}

// ---------------------------------------------------------------------------------------------------------------------------------

		StructuredException::StructuredException(EXCEPTION_POINTERS const &info) throw()
{
	EXCEPTION_RECORD const &exception = *(info.ExceptionRecord);
	_address = exception.ExceptionAddress;
	_code = exception.ExceptionCode;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void		StructuredException::install() throw()
{
	_set_se_translator(my_translator);
}

// ---------------------------------------------------------------------------------------------------------------------------------

unsigned	StructuredException::what() const throw()
{
	return _code;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void const *	StructuredException::where() const throw()
{
	return _address;
}

// ---------------------------------------------------------------------------------------------------------------------------------
// StructuredException.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
