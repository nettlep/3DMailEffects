// ---------------------------------------------------------------------------------------------------------------------------------
//  ______       _                              __  __     _     
// |  ____|     | |                            / _|/ _|   | |    
// | |__   _   _| |__   __ _ _ __         ___ | |_| |_    | |__  
// |  __| | | | | '_ \ / _` | '__|       / _ \|  _|  _|   | '_ \ 
// | |    | |_| | |_) | (_| | |         | (_) | | | |   _ | | | |
// |_|     \__,_|_.__/ \__,_|_|          \___/|_| |_|  (_)|_| |_|
//                                ______                         
//                               |______|                        
//
// Fubar_off -- disable the pollution macros
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 03/12/2001 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2001, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#if	defined(_REGISTERED_)

#undef	c_str
#undef	memset
#undef	SetTimer
#undef	GetProfileIntA
#undef	SetWindowTextA
#undef	AfxMessageBox
#undef	integerToString
#undef	toupper
#undef	tolower

#endif // _REGISTERED_

// ---------------------------------------------------------------------------------------------------------------------------------
// Fubar_off.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

