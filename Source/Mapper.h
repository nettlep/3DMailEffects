// ---------------------------------------------------------------------------------------------------------------------------------
//  __  __                                 _     
// |  \/  |                               | |    
// | \  / | __ _ _ __  _ __   ___ _ __    | |__  
// | |\/| |/ _` | '_ \| '_ \ / _ \ '__|   | '_ \ 
// | |  | | (_| | |_) | |_) |  __/ |    _ | | | |
// |_|  |_|\__,_| .__/| .__/ \___|_|   (_)|_| |_|
//              | |   | |                        
//              |_|   |_|                        
//
// Mapper (STL map extension class)
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 12/29/2000 by Paul Nettle
//
// Copyright 2000, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_MAPPER
#define _H_MAPPER

// ---------------------------------------------------------------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------------------------------------------------------------

typedef	map<wstring, wstring> MapperMap;

// ---------------------------------------------------------------------------------------------------------------------------------
// Forward declarations
// ---------------------------------------------------------------------------------------------------------------------------------

class	Conf;

// ---------------------------------------------------------------------------------------------------------------------------------

class	Mapper
{
public:
	// Construction/Destruction
	
				Mapper(){};
virtual				~Mapper(){};

	// Implementation

virtual		bool		exist(const wstring &title) const;
virtual	const	wstring		&getString(const wstring &title) const;
virtual	const	wstring		&getString(const wstring &title, const wstring &def) const;
virtual	const	int		getInteger(const wstring &title) const;
virtual	const	int		getInteger(const wstring &title, const int def) const;
virtual	const	float		getFloat(const wstring &title) const;
virtual	const	float		getFloat(const wstring &title, const float def) const;
inline		wstring		&operator[](wstring &s) {return _data[s];}

	// Accessors

inline		MapperMap	&data() {return _data;}

protected:
		MapperMap	_data;
		wstring		empty;
};

#endif
// ---------------------------------------------------------------------------------------------------------------------------------
// Mapper.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
