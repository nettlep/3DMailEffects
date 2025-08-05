// ---------------------------------------------------------------------------------------------------------------------------------
//  _____                       _                 _ _______ _                         _     _     
// |  __ \                     | |               | |__   __| |                       | |   | |    
// | |  | | ___ __      ___ __ | | ___   __ _  __| |  | |  | |__  _ __  ___  __ _  __| |   | |__  
// | |  | |/ _ \\ \ /\ / / '_ \| |/ _ \ / _` |/ _` |  | |  | '_ \| '__|/ _ \/ _` |/ _` |   | '_ \ 
// | |__| | (_) |\ V  V /| | | | | (_) | (_| | (_| |  | |  | | | | |  |  __/ (_| | (_| | _ | | | |
// |_____/ \___/  \_/\_/ |_| |_|_|\___/ \__,_|\__,_|  |_|  |_| |_|_|   \___|\__,_|\__,_|(_)|_| |_|
//                                                                                                
//                                                                                                
//
// Thread for performing downloads
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 03/11/2001 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2001, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_DOWNLOADTHREAD
#define _H_DOWNLOADTHREAD

// ---------------------------------------------------------------------------------------------------------------------------------
// Module setup (required includes, macros, etc.)
// ---------------------------------------------------------------------------------------------------------------------------------

#include "http.h"

// ---------------------------------------------------------------------------------------------------------------------------------

class	DownloadThread : public CWinThread
{
				DECLARE_DYNCREATE(DownloadThread)
		enum		{MAX_SPEED = 50};

protected:
				DownloadThread(); // protected constructor used by dynamic creation
virtual				~DownloadThread();

public:

	// Implementation

virtual		bool		beginDownloading(const string & url, const wstring & destFile);
virtual		void		stopDownloading();
inline		void		setSpeed(const unsigned int speed = MAX_SPEED)
				{
					_speed = speed;
					if (_speed > MAX_SPEED) _speed = MAX_SPEED;
				}

	// Accessors

inline		double &	percentComplete()		{return _percentComplete;}
inline	const	double		percentComplete() const		{return _percentComplete;}
inline		bool &		currentlyDownloading()		{return _currentlyDownloading;}
inline	const	bool		currentlyDownloading() const	{return _currentlyDownloading;}
inline		wstring &	errorString()			{return _errorString;}
inline	const	wstring &	errorString() const		{return _errorString;}
inline		unsigned int &	speed()				{return _speed;}
inline	const	unsigned int	speed() const			{return _speed;}
inline		unsigned int &	bytesWritten()			{return _bytesWritten;}
inline	const	unsigned int	bytesWritten() const		{return _bytesWritten;}
inline		double &	averageBPS()			{return _averageBPS;}
inline	const	double		averageBPS() const		{return _averageBPS;}
inline		double &	currentBPS()			{return _currentBPS;}
inline	const	double		currentBPS() const		{return _currentBPS;}
inline		string &	url()				{return _url;}
inline	const	string &	url() const			{return _url;}
inline		wstring &	destFile()			{return _destFile;}
inline	const	wstring &	destFile() const		{return _destFile;}
inline		http &		httpClient()			{return _httpClient;}
inline	const	http &		httpClient() const		{return _httpClient;}

	//{{AFX_VIRTUAL(DownloadThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

protected:
		double		_percentComplete;
		bool		_currentlyDownloading;
		wstring		_errorString;
		unsigned int	_speed;
		unsigned int	_bytesWritten;
		double		_averageBPS;
		double		_currentBPS;
		string		_url;
		wstring		_destFile;
		http		_httpClient;

		// Used internally

inline		FILE *&		pFile()				{return _pFile;}
inline	const	FILE *		pFile() const			{return _pFile;}
inline		clock_t &	clockStart()			{return _clockStart;}
inline	const	clock_t		clockStart() const		{return _clockStart;}
inline		double &	averageTotal()			{return _averageTotal;}
inline	const	double		averageTotal() const		{return _averageTotal;}
inline		double &	averageCount()			{return _averageCount;}
inline	const	double		averageCount() const		{return _averageCount;}

		FILE *		_pFile;
		clock_t		_clockStart;
		double		_averageTotal;
		double		_averageCount;

	//{{AFX_MSG(DownloadThread)
	afx_msg	void OnTimer(unsigned int wparam, long lparam);
	afx_msg	void OnTerminate(unsigned int wparam, long lparam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}

#endif // _H_DOWNLOADTHREAD
// ---------------------------------------------------------------------------------------------------------------------------------
// DownloadThread.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

