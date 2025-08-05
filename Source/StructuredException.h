// ---------------------------------------------------------------------------------------------------------------------------------
//   _____ _                   _                       _ ______                      _   _                 _     
//  / ____| |                 | |                     | |  ____|                    | | (_)               | |    
// | (___ | |_ _ __ _   _  ___| |_ _   _ _ __  ___  __| | |__  __  __ ___  ___ _ __ | |_ _  ___  _ __     | |__  
//  \___ \| __| '__| | | |/ __| __| | | | '__|/ _ \/ _` |  __| \ \/ // __|/ _ \ '_ \| __| |/ _ \| '_ \    | '_ \ 
//  ____) | |_| |  | |_| | (__| |_| |_| | |  |  __/ (_| | |____ >  <| (__|  __/ |_) | |_| | (_) | | | | _ | | | |
// |_____/ \__|_|   \__,_|\___|\__|\__,_|_|   \___|\__,_|______/_/\_\\___|\___| .__/ \__|_|\___/|_| |_|(_)|_| |_|
//                                                                            | |                                
//                                                                            |_|                                
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

#ifndef	_H_STRUCTUREDEXCEPTION
#define _H_STRUCTUREDEXCEPTION

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------------------------------

class	StructuredException
{
public:
			StructuredException(EXCEPTION_POINTERS const &) throw();
static	void		install() throw();
	unsigned int	what() const throw();
	void const *	where() const throw();
private:
	void const *	_address;
	unsigned int	_code;
};

#endif // _H_STRUCTUREDEXCEPTION
// ---------------------------------------------------------------------------------------------------------------------------------
// StructuredException.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
