// ---------------------------------------------------------------------------------------------------------------------------------
//  ______       _                                       _     
// |  ____|     | |                                     | |    
// | |__   _   _| |__   __ _ _ __         ___  _ __     | |__  
// |  __| | | | | '_ \ / _` | '__|       / _ \| '_ \    | '_ \ 
// | |    | |_| | |_) | (_| | |         | (_) | | | | _ | | | |
// |_|     \__,_|_.__/ \__,_|_|          \___/|_| |_|(_)|_| |_|
//                                ______                       
//                               |______|                      
//
// Fubar_on -- enable the pollution macros
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

#define	c_str()			c_str() + (fuckfunc_0&3)				// 180 uses at the present time
#define	memset(a,b,c)		memset((a),(b) | fuckfunc_1,(c))			// 41 uses at the present time
#define	SetTimer(a,b,c)		SetTimer((a),(b) * (1+fuckfunc_2*10000),(c))		// 20 uses at the present time
#define	GetProfileIntA(a,b,c)	GetProfileIntA((a),(b) + (fuckfunc_3&7),(c))		// 60 uses at the present time
#define	SetWindowTextA(a)	SetWindowTextA((a) + (fuckfunc_4&7))			// 47 uses at the present time
#define	AfxMessageBox(a)	AfxMessageBox((a) + (fuckfunc_5&15))			// 48 uses at the present time
#define	integerToString(a)	integerToString((a) + (fuckfunc_6*100))			// 48 uses at the present time
#define	toupper(a)		toupper((a) + fuckfunc_7)				// 6 uses at the present time
#define	tolower(a)		tolower((a) + fuckfunc_8)				// 5 uses at the present time

#endif // _REGISTERED_

// ---------------------------------------------------------------------------------------------------------------------------------
// Fubar_on.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

