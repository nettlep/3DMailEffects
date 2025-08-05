// ---------------------------------------------------------------------------------------------------------------------------------
//  ____  _ _                             _     
// |  _ \(_) |                           | |    
// | |_) |_| |_ _ __ ___   __ _ _ __     | |__  
// |  _ <| | __| '_ ` _ \ / _` | '_ \    | '_ \ 
// | |_) | | |_| | | | | | (_| | |_) | _ | | | |
// |____/|_|\__|_| |_| |_|\__,_| .__/ (_)|_| |_|
//                             | |              
//                             |_|              
//
// Bitmap class (from BMP files)
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 12/29/2000 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2000, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_BITMAP
#define _H_BITMAP

// ---------------------------------------------------------------------------------------------------------------------------------

class	Bitmap
{
public:
	// Construction/Destruction

				Bitmap();
virtual				~Bitmap();

	// Operators

inline		Bitmap		&operator =(const Bitmap &b)
				{
					_data = new unsigned int[b.width() * b.height()];
					memcpy(data(), b.data(), b.width() * b.height() * sizeof(int));
					width() = b.width();
					height() = b.height();
					return *this;
				}

	// Implementation

virtual		void		deallocate();
virtual		bool		read(const wstring &filename, const bool silent = false);

	// Accessors

inline	const	unsigned int	&width() const	{return _width;}
inline		unsigned int	&width()	{return _width;}
inline	const	unsigned int	&height() const	{return _height;}
inline		unsigned int	&height()	{return _height;}
inline	const	unsigned int	*data() const	{return _data;}
inline		unsigned int *&	data()		{return _data;}

private:
	// Data members

		unsigned int	_width;
		unsigned int	_height;
		unsigned int	*_data;
};

#endif // _H_BITMAP
// ---------------------------------------------------------------------------------------------------------------------------------
// Bitmap.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

