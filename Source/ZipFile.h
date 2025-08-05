// ---------------------------------------------------------------------------------------------------------------------------------
//      _        __ _ _          _     
//     (_)      / _(_) |        | |    
//  _____ _ __ | |_ _| | ___    | |__  
// |_  / | '_ \|  _| | |/ _ \   | '_ \ 
//  / /| | |_) | | | | |  __/ _ | | | |
// /___|_| .__/|_| |_|_|\___|(_)|_| |_|
//       | |                           
//       |_|                           
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

#ifndef	_H_ZIPFILE
#define _H_ZIPFILE

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------------------------------

class	ZipFile
{
private:
		#pragma pack(1)
		typedef	struct
		{
			unsigned int	signature; // (0x02014b50)
			unsigned short	versionMadeBy;
			unsigned short	versionNeededToExtract;
			unsigned short	generalPurposeBitFlag;
			unsigned short	compressionMethod;
			unsigned short	lastModFileTime;
			unsigned short	lastModFileDate;
			unsigned int	crc32;
			unsigned int	compressedSize;
			unsigned int	uncompressedSize;
			unsigned short	fnameLength;
			unsigned short	extraFieldLength;
			unsigned short	commentLength;
			unsigned short	diskNumberStart;
			unsigned short	internalFileAttributes;
			unsigned int	externalFileAttributes;
			unsigned int	relativeOffset;
		} FileHeader;

		typedef	struct
		{
			unsigned int	signature; // (0x04034b50)
			unsigned short	versionNeededToExtract;
			unsigned short	generalPurposeBitFlag;
			unsigned short	compressionMethod;
			unsigned short	lastModFileTime;
			unsigned short	lastModFileDate;
			unsigned int	crc32;
			unsigned int	compressedSize;
			unsigned int	uncompressedSize;
			unsigned short	fnameLength;
			unsigned short	extraFieldLength;
		} LocalFileHeader;
		#pragma pack()
public:
	// Construction/Destruction

					ZipFile();
virtual					~ZipFile();

	// Operators

	// Implementation

virtual		wstring			open(const wstring & fname);
virtual		void			close();
virtual		int			index(const wstring & fname);
virtual		wstring			readASCIIFile(const wstring & fname, string & contents);
virtual		unsigned int		decompressedSize(const wstring & fname);
virtual		wstring			read(const wstring & fname, unsigned char * outBuffer);

	// Accessors

inline		wstring &		filename()		{return _filename;}
inline	const	wstring &		filename() const	{return _filename;}
inline		vector<FileHeader> &	headers()		{return _headers;}
inline	const	vector<FileHeader> &	headers() const		{return _headers;}
inline		vector<wstring> &	files()			{return _files;}
inline	const	vector<wstring> &	files() const		{return _files;}

private:
	// Data members

		wstring			_filename;
		vector<FileHeader>	_headers;
		vector<wstring>		_files;
};

#endif // _H_ZIPFILE
// ---------------------------------------------------------------------------------------------------------------------------------
// zipfile.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
