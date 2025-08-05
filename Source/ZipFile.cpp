// ---------------------------------------------------------------------------------------------------------------------------------
//      _        __ _ _                           
//     (_)      / _(_) |                          
//  _____ _ __ | |_ _| | ___      ___ _ __  _ __  
// |_  / | '_ \|  _| | |/ _ \    / __| '_ \| '_ \ 
//  / /| | |_) | | | | |  __/ _ | (__| |_) | |_) |
// /___|_| .__/|_| |_|_|\___|(_) \___| .__/| .__/ 
//       | |                         | |   | |    
//       |_|                         |_|   |_|    
//
//   ZIP file class
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 03/18/2004 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2004, Fluid Studios, all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "3dme.h"
#include "zipfile.h"
#include "zlib/zlib.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

	ZipFile::ZipFile()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

	ZipFile::~ZipFile()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

static	const wchar_t *	zlibErrorString(const int err)
{
	switch(err)
	{
		case	Z_OK:			return _T("Z_OK");
		case	Z_STREAM_END:		return _T("Z_STREAM_END");
		case	Z_NEED_DICT:		return _T("Z_NEED_DICT");
		case	Z_ERRNO:		return _T("Z_ERRNO");
		case	Z_STREAM_ERROR:		return _T("Z_STREAM_ERROR");
		case	Z_DATA_ERROR:		return _T("Z_DATA_ERROR");
		case	Z_MEM_ERROR:		return _T("Z_MEM_ERROR");
		case	Z_BUF_ERROR:		return _T("Z_BUF_ERROR");
		case	Z_VERSION_ERROR:	return _T("Z_VERSION_ERROR");
	}

	return _T("Unknown");
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	ZipFile::open(const wstring & fname)
{
	close();
	filename() = fname;

	// Open the input file

	FILE *	fp = NULL;
	try
	{
		// Open the input file

		fp = _wfopen(filename().c_str(), _T("rb"));
		if (!fp) throw wstring(_T("Unable to open input file"));

		// Read each header

		for(;;)
		{
			// Read a file header

			FileHeader	thisHeader;
			if (fread(&thisHeader, sizeof(thisHeader), 1, fp) != 1) throw wstring(_T("Unable to read file header"));
			if (thisHeader.signature != 0x02014b50) break;

			// Read the filename

			char	thisFilename[MAX_PATH];
			memset(thisFilename, 0, sizeof(thisFilename));
			if (fread(thisFilename, thisHeader.fnameLength, 1, fp) != 1) throw wstring(_T("Unable to read filename field"));

			// Skip over the extra crap

			if (fseek(fp, thisHeader.extraFieldLength + thisHeader.commentLength, SEEK_CUR)) throw wstring(_T("Unable to seek past comment"));

			// Save this header stuff

			headers().push_back(thisHeader);
			files().push_back(atow(thisFilename));
		}
	}
	catch(const wstring & err)
	{
		if (fp) fclose(fp);
		return err;
	}

	return _T("");
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ZipFile::close()
{
	filename().erase();
	headers().clear();
	files().clear();
}

// ---------------------------------------------------------------------------------------------------------------------------------

int	ZipFile::index(const wstring & fname)
{
	// Find the file they're requesting

	for (unsigned int i = 0; i < files().size(); ++i)
	{
		if (areEqualNocase(files()[i], fname)) return i;
	}

	return -1;
}

// ---------------------------------------------------------------------------------------------------------------------------------

unsigned int	ZipFile::decompressedSize(const wstring & fname)
{
	// Find the file they're requesting

	int	fileIndex = index(fname);
	if (fileIndex == -1) return -1;

	// Return the size

	return headers()[fileIndex].uncompressedSize;
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	ZipFile::readASCIIFile(const wstring & fname, string & contents)
{
	unsigned char *	buffer = NULL;
	try
	{
		// How big is the config file?

		unsigned int	length = decompressedSize(fname);
		if (!length) throw _T("The ") + fname + _T(" file (within ") + filename() + _T(") is either missing, corrupt, or empty");

		// Allocate RAM for the file

		buffer = new unsigned char [length + 1];
		buffer[length - 1] = 0;

		// Read the file from the zipfile

		wstring	rc = read(fname, buffer);
		if (rc.length()) throw _T("Unable to read ") + fname + _T(" (within ") + filename() + _T("): ") + rc;

		// Store it in the provided string

		contents = reinterpret_cast<char *>(buffer);

		// Cleanup

		delete[] buffer;
	}
	catch(const wstring & err)
	{
		delete[] buffer;
		return err;
	}

	return _T("");
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	ZipFile::read(const wstring & fname, unsigned char * outBuffer)
{
	// Find the file they're requesting

	int	fileIndex = index(fname);
	if (fileIndex == -1) return _T("The filename requested was not found within the zip file.");

	if (!outBuffer) return _T("Internal error (outBuffer is null)");

	// The file in question

	FileHeader &	thisHeader = headers()[fileIndex];

	// Open the input file

	FILE *		fp = NULL;
	unsigned char *	buffer = NULL;
	unsigned char *	decompressionBuffer = NULL;
	try
	{
		// Our read/write buffer

		const unsigned int	bufferSize = 1024*64;
		buffer = new unsigned char[bufferSize];
		decompressionBuffer = new unsigned char[bufferSize];

		// For simplicity

		unsigned char *	dst = outBuffer;

		// Jump to the local file header

		if (fseek(fp, thisHeader.relativeOffset, SEEK_SET)) throw wstring(_T("Unable to seek to local file header"));

		// Read the local file header

		LocalFileHeader	localHeader;
		if (fread(&localHeader, sizeof(localHeader), 1, fp) != 1) throw wstring(_T("Unable to read local file header"));
		if (localHeader.signature != 0x04034b50) throw wstring(_T("Local file header signature mismatch -- file corrupt?"));

		// Skip over the extra crap

		if (fseek(fp, localHeader.fnameLength + localHeader.extraFieldLength, SEEK_CUR)) throw wstring(_T("Unable to seek past comment"));

		// Just copy over the non-compressed files directly

		if (localHeader.compressionMethod == 0)
		{
			unsigned int	length = localHeader.uncompressedSize;

			// Write the file out in 64K blocks

			while(length)
			{
				// How much data to run this pass

				unsigned int	thisPass = length;
				if (thisPass > bufferSize) thisPass = bufferSize;
				length -= thisPass;

				// Read the buffer data

				if (fread(buffer, thisPass, 1, fp) != 1) throw wstring(_T("Unable to read flle data"));

				// Write the buffer data

				memcpy(dst, buffer, thisPass);
				dst += thisPass;
			}
		}
		else
		{
			// Here we have the actual file data, compressed

			unsigned int	length = localHeader.compressedSize;

			// Setup the decompression stream

			z_stream	d_stream;
			d_stream.zalloc = (alloc_func) 0;
			d_stream.zfree = (free_func) 0;
			d_stream.opaque = (voidpf) 0;

			if (inflateInit2(&d_stream, -15) != Z_OK) throw wstring(_T("Unable to initialize the decompressor"));

			// Setup the input stream

			d_stream.next_in  = buffer;
			d_stream.avail_in = 0;

			// Process the input stream, writing out data as needed

			for (;;)
			{
				// Read some input data

				if (length)
				{
					// Shift the buffer down

					if (d_stream.avail_in)
					{
						memmove(buffer, d_stream.next_in, d_stream.avail_in);
						d_stream.next_in = buffer;
					}

					// If there's fewer bytes in the file than room in the buffer, clamp

					unsigned int	thisPass = bufferSize - d_stream.avail_in;
					if (thisPass > length) thisPass = length;

					// Read the data

					if (fread(d_stream.next_in + d_stream.avail_in, thisPass, 1, fp) != 1) throw wstring(_T("Unable to read flle data"));

					// Update our counts

					length -= thisPass;
					d_stream.avail_in += thisPass;
				}

				// A full-sized, empty output buffer

				d_stream.next_out = decompressionBuffer;
				d_stream.avail_out = bufferSize;

				// Decompress

				int	rc = inflate(&d_stream, Z_PARTIAL_FLUSH);

				// Error?

				static	wchar_t	errCodeMsg[1024];
				if (rc != Z_STREAM_END && rc != Z_OK)
				{
					swprintf(errCodeMsg, _T("Internal error in the decompressor: %s"), zlibErrorString(rc));
					throw wstring(errCodeMsg);
				}

				// Write the buffer data

				memcpy(dst, decompressionBuffer, bufferSize - d_stream.avail_out);
				dst += bufferSize - d_stream.avail_out;

				// Done?

				if (rc == Z_STREAM_END) break;
			}

			if (inflateEnd(&d_stream) != Z_OK) throw wstring(_T("Unable to end the decompression"));
		}

		// Done

		fclose(fp);
		fp = NULL;

		delete[] buffer;
		buffer = NULL;

		delete[] decompressionBuffer;
		decompressionBuffer = NULL;
	}
	catch(const wstring & err)
	{
		if (fp)
		{
			fclose(fp);
			fp = NULL;
		}

		delete[] buffer;
		buffer = NULL;

		delete[] decompressionBuffer;
		decompressionBuffer = NULL;

		return err;
	}

	return _T("");
}

// ---------------------------------------------------------------------------------------------------------------------------------
// zipfile.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
