// ---------------------------------------------------------------------------------------------------------------------------------
//  __  __ _                    _     
// |  \/  (_)                  | |    
// | \  / |_ _ __ ___   ___    | |__  
// | |\/| | | '_ ` _ \ / _ \   | '_ \ 
// | |  | | | | | | | |  __/ _ | | | |
// |_|  |_|_|_| |_| |_|\___|(_)|_| |_|
//                                    
//                                    
//
// Mime file type manager
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 02/12/2001 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2001, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_MIME
#define _H_MIME

// ---------------------------------------------------------------------------------------------------------------------------------

class	Parts
{
public:
			Parts()
			{
				binaryData = NULL;
				binaryDataSize = 0;
			}

			Parts(const Parts &p)
			{
				description = p.description;
				data = p.data;
				fileName = p.fileName;
				isHtml = p.isHtml;

				if (p.binaryDataSize)
				{
					binaryDataSize = p.binaryDataSize;
					binaryData = new unsigned char[binaryDataSize];
					if (!binaryData) binaryDataSize = 0;
					else memcpy(binaryData, p.binaryData, binaryDataSize);
				}
				else
				{
					binaryDataSize = 0;
					binaryData = NULL;
				}
			}

virtual			~Parts()
			{
				delete[] binaryData;
				binaryData = NULL;
			}

virtual		Parts	&operator =(const Parts &p)
			{
				description = p.description;
				data = p.data;
				fileName = p.fileName;
				isHtml = p.isHtml;

				if (p.binaryDataSize)
				{
					binaryDataSize = p.binaryDataSize;
					binaryData = new unsigned char[binaryDataSize];
					if (!binaryData) binaryDataSize = 0;
					else memcpy(binaryData, p.binaryData, binaryDataSize);
				}
				else
				{
					binaryDataSize = 0;
					binaryData = NULL;
				}

				return *this;
			}

	wstring		description;
	wstring		fileName;
	StringList	data;
	bool		isHtml;
	unsigned char	*binaryData;
	unsigned int	binaryDataSize;
};

// ---------------------------------------------------------------------------------------------------------------------------------

class	Mime
{
public:
	// Construction/Destruction

				Mime();
virtual				~Mime();

	// Operators

	// Implementation

virtual		void		parse(const StringList &_header, unsigned int &embeddedSkip);
virtual		void		addPart(const Mime &m, StringList &data);
virtual		void		parseParts(const string &body);

	// Accessors

inline	const	StringList	&header() const {return _header;}
inline		StringList	&header() {return _header;}

inline	const	wstring		&date() const {return _date;}
inline		wstring		&date() {return _date;}

inline	const	wstring		&from() const {return _from;}
inline		wstring		&from() {return _from;}

inline	const	wstring		&replyTo() const {return _replyTo;}
inline		wstring		&replyTo() {return _replyTo;}

inline	const	wstring		&to() const {return _to;}
inline		wstring		&to() {return _to;}

inline	const	wstring		&subject() const {return _subject;}
inline		wstring		&subject() {return _subject;}

inline	const	wstring		&contentType() const {return _contentType;}
inline		wstring		&contentType() {return _contentType;}

inline	const	wstring		&contentBoundary() const {return _contentBoundary;}
inline		wstring		&contentBoundary() {return _contentBoundary;}

inline	const	wstring		&contentName() const {return _contentName;}
inline		wstring		&contentName() {return _contentName;}

inline	const	wstring		&contentTransferEncoding() const {return _contentTransferEncoding;}
inline		wstring		&contentTransferEncoding() {return _contentTransferEncoding;}

inline	const	wstring		&contentDisposition() const {return _contentDisposition;}
inline		wstring		&contentDisposition() {return _contentDisposition;}

inline	const	wstring		&contentFilename() const {return _contentFilename;}
inline		wstring		&contentFilename() {return _contentFilename;}

inline	const	vector<Parts>	&parts() const {return _parts;}
inline		vector<Parts>	&parts() {return _parts;}

inline	const	bool		&hasAttachment() const {return _hasAttachment;}
inline		bool		&hasAttachment() {return _hasAttachment;}

inline	const	bool		&plainText() const {return _plainText;}
inline		bool		&plainText() {return _plainText;}

inline	const	bool		&htmlText() const {return _htmlText;}
inline		bool		&htmlText() {return _htmlText;}

inline	const	bool		&validEmail() const {return _validEmail;}
inline		bool		&validEmail() {return _validEmail;}

inline	const	bool		&spam() const {return _spam;}
inline		bool		&spam() {return _spam;}

private:
virtual		void		recursiveParseParts(const string &body);

		StringList	_header;
		wstring		_date;
		wstring		_from;
		wstring		_replyTo;
		wstring		_to;
		wstring		_subject;
		wstring		_contentType;
		wstring		_contentBoundary;
		wstring		_contentName;
		wstring		_contentTransferEncoding;
		wstring		_contentDisposition;
		wstring		_contentFilename;
		vector<Parts>	_parts;
		bool		_hasAttachment;
		bool		_plainText;
		bool		_htmlText;
		bool		_validEmail;
		bool		_spam;
	// Data members
};

#endif // _H_MIME
// ---------------------------------------------------------------------------------------------------------------------------------
// Mime.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

