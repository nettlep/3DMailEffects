// ---------------------------------------------------------------------------------------------------------------------------------
//  ______       _                    _     
// |  ____|     | |                  | |    
// | |__   _   _| |__   __ _ _ __    | |__  
// |  __| | | | | '_ \ / _` | '__|   | '_ \ 
// | |    | |_| | |_) | (_| | |    _ | | | |
// |_|     \__,_|_.__/ \__,_|_|   (_)|_| |_|
//                                          
//                                          
//
// Fubar -- a heavy-handed key-checking system
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

#ifndef	_H_FUBAR
#define _H_FUBAR

#if	defined(_REGISTERED_)

// ---------------------------------------------------------------------------------------------------------------------------------
// This grabs a specific subkey based on (v0 thru v7)
// ---------------------------------------------------------------------------------------------------------------------------------

#define	grab_a_subkey(v0,v1,v2,v3,v4,v5,v6,v7)				\
	((((oneHundredKeys[(v4<<4)+(v0>>3)]<<(v0&7))&0x80)>>0) |	\
	 (((oneHundredKeys[(v4<<4)+(v1>>3)]<<(v1&7))&0x80)>>1) |	\
	 (((oneHundredKeys[(v4<<4)+(v2>>3)]<<(v2&7))&0x80)>>2) |	\
	 (((oneHundredKeys[(v4<<4)+(v3>>3)]<<(v3&7))&0x80)>>3) |	\
	 (((oneHundredKeys[(v4<<4)+(v4>>3)]<<(v4&7))&0x80)>>4) |	\
	 (((oneHundredKeys[(v4<<4)+(v5>>3)]<<(v5&7))&0x80)>>5) |	\
	 (((oneHundredKeys[(v4<<4)+(v6>>3)]<<(v6&7))&0x80)>>6) |	\
	 (((oneHundredKeys[(v4<<4)+(v7>>3)]<<(v7&7))&0x80)>>7))

// ---------------------------------------------------------------------------------------------------------------------------------
// Each of these will return a specific subkey
// ---------------------------------------------------------------------------------------------------------------------------------

#define	key_a grab_a_subkey(17,52,62,65,74, 82, 90,106)
#define	key_b grab_a_subkey( 7, 8,34,35,45, 46, 50, 72)
#define	key_c grab_a_subkey(15,65,73,82,83, 88, 97,100)
#define	key_d grab_a_subkey( 4,29,36,57,59, 60, 67, 68)
#define	key_e grab_a_subkey(13,14,16,49,53, 63, 64, 66)
#define	key_f grab_a_subkey( 3,47,75,76,89, 98, 99,105)
#define	key_g grab_a_subkey( 0, 1,25,40,93, 94, 95,108)
#define	key_h grab_a_subkey(13,14,28,30,62, 63, 64, 66)
#define	key_i grab_a_subkey(26,27,43,44,58, 84, 85, 86)
#define	key_j grab_a_subkey(16,30,64,65,75, 90, 98,105)
#define	key_k grab_a_subkey(31,32,37,48,51, 79, 80, 87)
#define	key_l grab_a_subkey( 3,15,30,63,66, 75, 90,106)
#define	key_m grab_a_subkey(13,16,53,62,73, 75, 98, 99)
#define	key_n grab_a_subkey( 5,23,55,56,71, 96,103,104)
#define	key_o grab_a_subkey( 3,15,16,53,63,100,105,106)
#define	key_p grab_a_subkey(30,47,65,76,83, 88, 98,106)
#define	key_q grab_a_subkey( 2, 6,24,38,39, 92,107,109)
#define	key_r grab_a_subkey(14,17,47,49,62, 74, 88, 99)
#define	key_s grab_a_subkey(33,54,69,70,91,101,102,110)
#define	key_t grab_a_subkey(15,17,66,76,83, 89, 97,105)
#define	key_u grab_a_subkey( 3,28,52,53,83, 88, 90, 99)
#define	key_v grab_a_subkey(18,19,20,41,42, 77, 78, 81)
#define	key_w grab_a_subkey(14,28,49,73,74, 82, 89,100)
#define	key_x grab_a_subkey(13,28,52,74,82, 89, 97,100)
#define	key_y grab_a_subkey(17,47,49,52,64, 73, 76, 97)
#define	key_z grab_a_subkey( 9,10,11,12,21, 22, 61,111)

// ---------------------------------------------------------------------------------------------------------------------------------
// There are 26 subkeys (it just worked out that way), which belong to a formula. There are 10 formulas. Here's how the subkeys
// relate to each of the 10 formulas:
//
// 	a + b         = 0x59
// 	c + d         = 0xA6
// 	e + f - g     = 0x01
// 	h - i         = 0x0A
// 	j + k         = 0x41
// 	l + m - n     = 0xA3
// 	o - p + q     = 0x84
// 	r - s         = 0
// 	t - u + v     = 0x92
// 	w + x + y - z = 0
// ---------------------------------------------------------------------------------------------------------------------------------

#define	keyfunc_0	(unsigned char)(key_a + key_b                ) // = 0x59
#define	keyfunc_1	(unsigned char)(key_c + key_d                ) // = 0xA6
#define	keyfunc_2	(unsigned char)(key_e + key_f - key_g        ) // = 0x01
#define	keyfunc_3	(unsigned char)(key_h - key_i                ) // = 0x0A
#define	keyfunc_4	(unsigned char)(key_j + key_k                ) // = 0x41
#define	keyfunc_5	(unsigned char)(key_l + key_m - key_n        ) // = 0xA3
#define	keyfunc_6	(unsigned char)(key_o - key_p + key_q        ) // = 0x84
#define	keyfunc_7	(unsigned char)(key_r - key_s                ) // = 0x00
#define	keyfunc_8	(unsigned char)(key_t - key_u + key_v        ) // = 0x92
#define	keyfunc_9	(unsigned char)(key_w + key_x + key_y - key_z) // = 0x00

// ---------------------------------------------------------------------------------------------------------------------------------
// Each keyfunc should have a specific result, here they are...
// ---------------------------------------------------------------------------------------------------------------------------------

#define	keyres__0	0x59
#define	keyres__1	0xA6
#define	keyres__2	0x01
#define	keyres__3	0x0A
#define	keyres__4	0x41
#define	keyres__5	0xA3
#define	keyres__6	0x84
#define	keyres__7	0x00
#define	keyres__8	0x92
#define	keyres__9	0x00

// ---------------------------------------------------------------------------------------------------------------------------------
// And rather than use them directly, we'll combine them, so they'll never see the actual required results from each keyfunc_*
//
// These are just a random smattering of combinations of key functions. We'll save one (the last one) for a special occasion...
// We'll use that one very rarely (maybe just once at some low-level crucial place) so that it'll be BURIED in the midst of all
// the other keychecks.
//
// By combining these like this, they never see the actual values (keyres__*) that are required to regenerate the keys. They have
// to literally figure out the entire key in the most difficult way (and the key isn't easy to start with! :)
// ---------------------------------------------------------------------------------------------------------------------------------

#define	fuckfunc_0	((keyfunc_0 + keyfunc_2) - (keyres__0 + keyres__2))
#define	fuckfunc_1	((keyfunc_7 + keyfunc_2) - (keyres__7 + keyres__2))
#define	fuckfunc_2	((keyfunc_3 + keyfunc_6) - (keyres__3 + keyres__6))
#define	fuckfunc_3	((keyfunc_8 * keyfunc_1) - (keyres__8 * keyres__1))
#define	fuckfunc_4	((keyfunc_0 * keyfunc_1) - (keyres__0 * keyres__1))
#define	fuckfunc_5	((keyfunc_6 * keyfunc_5) - (keyres__6 * keyres__5))
#define	fuckfunc_6	((keyfunc_5 - keyfunc_4) - (keyres__5 - keyres__4))
#define	fuckfunc_7	((keyfunc_8 - keyfunc_3) - (keyres__8 - keyres__3))
#define	fuckfunc_8	((keyfunc_4 - keyfunc_7) - (keyres__4 - keyres__7))
#define	fuckfunc_9	((keyfunc_9 * keyfunc_3) - (keyres__9 * keyres__3)) // this one is a little special, use it rarely!

#include "fubar_on.h"

#endif // _REGISTERED_

#if	!defined(_REGISTERED_)
#define	fuckfunc_0	0
#define	fuckfunc_1	0
#define	fuckfunc_2	0
#define	fuckfunc_3	0
#define	fuckfunc_4	0
#define	fuckfunc_5	0
#define	fuckfunc_6	0
#define	fuckfunc_7	0
#define	fuckfunc_8	0
#define	fuckfunc_9	0
#endif // !_REGISTERED_


#endif // _H_FUBAR
// ---------------------------------------------------------------------------------------------------------------------------------
// Fubar.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

