// ---------------------------------------------------------------------------------------------------------------------------------
//  ____  _ _                                              
// |  _ \(_) |                                             
// | |_) |_| |_ _ __ ___   __ _ _ __       ___ _ __  _ __  
// |  _ <| | __| '_ ` _ \ / _` | '_ \     / __| '_ \| '_ \ 
// | |_) | | |_| | | | | | (_| | |_) | _ | (__| |_) | |_) |
// |____/|_|\__|_| |_| |_|\__,_| .__/ (_) \___| .__/| .__/ 
//                             | |            | |   | |    
//                             |_|            |_|   |_|    
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

#include "stdafx.h"
#include "3Dme.h"
#include "Bitmap.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

	Bitmap::Bitmap()
	: _width(0), _height(0), _data(NULL)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

	Bitmap::~Bitmap()
{
	deallocate();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	Bitmap::deallocate()
{
	delete[] _data;
	_data = NULL;
	height() = 0;
	width() = 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	Bitmap::read(const wstring &filename, const bool silent)
{
	FILE		*fp = _wfopen(filename.c_str(), _T("rb"));
	unsigned char	*tempBuffer = NULL;
	try
	{
		if (!fp) throw_wstring(_T("Unable to open the BMP file: ") + filename)

		// Check the file header

		BITMAPFILEHEADER	bfh;
		if (fread(&bfh, sizeof(bfh), 1, fp) != 1) throw_wstring (_T("Unable to read the bitmap file header from\nthe file: ") + filename)
		if (bfh.bfType != *(WORD *) "BM") throw_wstring (_T("The file \"") + filename + _T("\" is not a valid BMP file"))

		// Check the info header

		BITMAPINFOHEADER	bih;
		if (fread(&bih, sizeof(bih), 1, fp) != 1) throw_wstring (_T("Unable to read the bitmap info header"))
		if (bih.biSize != sizeof(bih)) throw_wstring (_T("The file \"") + filename + _T("\" is not one of the supported\nformats of BMP files (OS/2 versions not supported)"))
		if (bih.biPlanes != 1) throw_wstring (_T("The file \"") + filename + _T("\" is non standard (biPlanes != 1)"))
		if (bih.biBitCount != 24 && bih.biBitCount != 8) throw_wstring (_T("The file \"") + filename + _T("\" is not the proper\nbit depth (must a 8- or 24-bit image)"))
		if (bih.biCompression != BI_RGB) throw_wstring (_T("The file \"") + filename + _T("\" is not one of the supported\nformats (must not be compressed)"))

		unsigned char	*palette = NULL;
		if (bih.biBitCount == 8)
		{
			int	palSize = bfh.bfOffBits - (sizeof(bih) + sizeof(bfh));
			palette = new unsigned char[palSize];
			if (!palette) throw_wstring (_T("Unable to allocate palette RAM for the file: ") + filename)
			if (!fread(palette, palSize, 1, fp)) throw_wstring (_T("Unable to read bitmap palette from\nthe file: ") + filename)
		}

		// Our stats

		width() = bih.biWidth;
		height() = bih.biHeight;

		// Allocate the buffer

		delete[] _data;
		_data = new unsigned int[width() * height()];
		if (!_data) throw_wstring (_T("Unable to allocate RAM for the file: ") + filename)

		// Padding

		int	paddedWidth = width() * (bih.biBitCount / 8);
		if (paddedWidth & 3) paddedWidth += 4 - (paddedWidth & 3);

		// Read the bitmap data

		unsigned int	bytesToRead = paddedWidth * height();
		tempBuffer = new unsigned char[bytesToRead];
		memset(tempBuffer, 0, bytesToRead);
		if (!tempBuffer) throw_wstring (_T("Unable to allocate temporary RAM for the file: ") + filename)
		if (!fread(tempBuffer, bytesToRead, 1, fp)) throw_wstring (_T("Unable to read bitmap data from\nthe file: ") + filename)

		// Convert the data

		if (bih.biBitCount == 24)
		{
			unsigned int	*oPtr = _data;
			for (unsigned int y = 0; y < height(); ++y)
			{
				unsigned char *iPtr = &tempBuffer[(height()-y-1) * paddedWidth];

				for (unsigned int x = 0; x < width(); ++x, ++oPtr, iPtr += 3)
				{
					*oPtr = (iPtr[2] << 16) + (iPtr[1] << 8) + iPtr[0];
				}
			}
		}
		else if (bih.biBitCount == 8)
		{
			unsigned int	*oPtr = _data;
			for (unsigned int y = 0; y < height(); ++y)
			{
				unsigned char *ptr = &tempBuffer[(height()-y-1) * paddedWidth];

				for (unsigned int x = 0; x < width(); ++x, ++oPtr, ++ptr)
				{
					unsigned char *iPtr = &palette[*ptr * 4];
					*oPtr = (iPtr[2] << 16) + (iPtr[1] << 8) + iPtr[0];
				}
			}
		}

		delete[] tempBuffer;
		delete[] palette;
	}
	catch (const wstring & err)
	{
		delete[] tempBuffer;
		delete[] _data;
		_data = NULL;
		if (fp) fclose(fp);
		if (!silent) AfxMessageBox(err.c_str());
		return false;
	}

	fclose(fp);

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------
// Bitmap.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

