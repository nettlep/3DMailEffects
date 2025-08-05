// ---------------------------------------------------------------------------------------------------------------------------------
//   _____ _ _           _     
//  / ____| (_)         | |    
// | |    | |_ _ __     | |__  
// | |    | | | '_ \    | '_ \ 
// | |____| | | |_) | _ | | | |
//  \_____|_|_| .__/ (_)|_| |_|
//            | |              
//            |_|              
//
// 3D Homogenous clipper
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
// Originally created on 12/06/2000 by Paul Nettle
//
// Copyright 2000, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------------------------------
// Files required by this module
// ---------------------------------------------------------------------------------------------------------------------------------

#include "Primitive.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_CLIP
#define	_H_CLIP


class	Clipper
{
public:
		bool		clipPrimitive(primitive<> &p);

private:
		void		nClip(point4 &dst, const point4 &on, const point4 &off, point2 &tdst, const point2 &ton, const point2 &toff);
		void		fClip(point4 &dst, const point4 &on, const point4 &off, point2 &tdst, const point2 &ton, const point2 &toff);
		void		lClip(point4 &dst, const point4 &on, const point4 &off, point2 &tdst, const point2 &ton, const point2 &toff);
		void		rClip(point4 &dst, const point4 &on, const point4 &off, point2 &tdst, const point2 &ton, const point2 &toff);
		void		tClip(point4 &dst, const point4 &on, const point4 &off, point2 &tdst, const point2 &ton, const point2 &toff);
		void		bClip(point4 &dst, const point4 &on, const point4 &off, point2 &tdst, const point2 &ton, const point2 &toff);
		unsigned int	neClip(primitive<> &src);
		unsigned int	feClip(primitive<> &src);
		unsigned int	leClip(primitive<> &src);
		unsigned int	reClip(primitive<> &src);
		unsigned int	teClip(primitive<> &src);
		unsigned int	beClip(primitive<> &src);
};

#endif // _H_CLIP
// ---------------------------------------------------------------------------------------------------------------------------------
// Clip.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

