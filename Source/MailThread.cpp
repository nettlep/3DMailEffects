// ---------------------------------------------------------------------------------------------------------------------------------
//  __  __       _ _ _______ _                         _                      
// |  \/  |     (_) |__   __| |                       | |                     
// | \  / | __ _ _| |  | |  | |__  _ __  ___  __ _  __| |     ___ _ __  _ __  
// | |\/| |/ _` | | |  | |  | '_ \| '__|/ _ \/ _` |/ _` |    / __| '_ \| '_ \ 
// | |  | | (_| | | |  | |  | | | | |  |  __/ (_| | (_| | _ | (__| |_) | |_) |
// |_|  |_|\__,_|_|_|  |_|  |_| |_|_|   \___|\__,_|\__,_|(_) \___| .__/| .__/ 
//                                                               | |   | |    
//                                                               |_|   |_|    
//
// 3D Mail Effects (Mail communications thread handler class)
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 12/04/2000 by Paul Nettle
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
#include "MailThread.h"
#include "3DmeDlg.h"
#include "pop.h"
#include "Webmail.h"
#include "comdef.h"
#include "StructuredException.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

#define	zzz() {wstring sss = _T(__FILE__) + wstring(_T("[")) + integerToWString(__LINE__) + wstring(_T("]")); LOG(wtoa(sss), Logger::LOG_CRIT);}

// ---------------------------------------------------------------------------------------------------------------------------------

IMPLEMENT_DYNCREATE(MailThread, CWinThread)

// ---------------------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(MailThread, CWinThread)
	//{{AFX_MSG_MAP(MailThread)
	ON_THREAD_MESSAGE(MAILTHREAD_TIMER, OnTimer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	MailThread::MailThread()
{
	lastTime = time(NULL);
	dialog() = NULL;
}

// ---------------------------------------------------------------------------------------------------------------------------------

	MailThread::~MailThread()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	MailThread::InitInstance()
{
	// Init the sockets

	if (!AfxSocketInit())
	{
		AfxMessageBox(_T("Unable to initialize Windows sockets"));
		return FALSE;
	}

	return TRUE;
}

// ---------------------------------------------------------------------------------------------------------------------------------

int	MailThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	MailThread::OnTimer(unsigned int wparam, long lparam)
{
	StructuredException::install();

	try
	{
		try
		{
			while(messageQueue.size())
			{
				// Go grab a message

				sMessage	m = messageQueue.front();

				// Remove it from the queue

				messageQueue.pop_front();

				// Process it

				switch(m.command)
				{
					case CMD_CHECK:
						checkMail(m);
						break;

					case CMD_RETRIEVE:
						retrieveMail(m);
						break;

					case CMD_DELETE:
						deleteMail(m);
						break;

					case CMD_TERMINATE:
						terminate(m);
						break;
				}
			}
		}
//		catch(CMemoryException & except)
//		{
//			wchar_t	err[2048];
//			memset(err, 0, sizeof(err));
//			except.GetErrorMessage(err, 2047, NULL);
//			throw_wstring(_T("CMemoryException: ") + wstring(err));
//		}
//		catch(CNotSupportedException & except)
//		{
//			wchar_t	err[2048];
//			memset(err, 0, sizeof(err));
//			except.GetErrorMessage(err, 2047, NULL);
//			throw_wstring(_T("CNotSupportedException: ") + wstring(err));
//		}
//		catch(CArchiveException & except)
//		{
//			wchar_t	err[2048];
//			memset(err, 0, sizeof(err));
//			except.GetErrorMessage(err, 2047, NULL);
//			throw_wstring(_T("CArchiveException: ") + wstring(err));
//		}
//		catch(CFileException & except)
//		{
//			wchar_t	err[2048];
//			memset(err, 0, sizeof(err));
//			except.GetErrorMessage(err, 2047, NULL);
//			throw_wstring(_T("CFileException: ") + wstring(err));
//		}
//		catch(CResourceException & except)
//		{
//			wchar_t	err[2048];
//			memset(err, 0, sizeof(err));
//			except.GetErrorMessage(err, 2047, NULL);
//			throw_wstring(_T("CResourceException: ") + wstring(err));
//		}
//		catch(COleException & except)
//		{
//			wchar_t	err[2048];
//			memset(err, 0, sizeof(err));
//			except.GetErrorMessage(err, 2047, NULL);
//			throw_wstring(_T("COleException: ") + wstring(err));
//		}
//		catch(CDBException & except)
//		{
//			wchar_t	err[2048];
//			memset(err, 0, sizeof(err));
//			except.GetErrorMessage(err, 2047, NULL);
//			throw_wstring(_T("CDBException: ") + wstring(err));
//		}
//		catch(COleDispatchException & except)
//		{
//			wchar_t	err[2048];
//			memset(err, 0, sizeof(err));
//			except.GetErrorMessage(err, 2047, NULL);
//			throw_wstring(_T("COleDispatchException: ") + wstring(err));
//		}
		catch(COleDispatchException * except)
		{
			wchar_t	err[2048];
			memset(err, 0, sizeof(err));
			except->GetErrorMessage(err, 2047, NULL);
			throw_wstring(_T("COleDispatchException: ") + wstring(err));
		}
//		catch(CUserException & except)
//		{
//			wchar_t	err[2048];
//			memset(err, 0, sizeof(err));
//			except.GetErrorMessage(err, 2047, NULL);
//			throw_wstring(_T("CUserException: ") + wstring(err));
//		}
//		catch(CDaoException & except)
//		{
//			wchar_t	err[2048];
//			memset(err, 0, sizeof(err));
//			except.GetErrorMessage(err, 2047, NULL);
//			throw_wstring(_T("CDaoException: ") + wstring(err));
//		}
//		catch(CInternetException & except)
//		{
//			wchar_t	err[2048];
//			memset(err, 0, sizeof(err));
//			except.GetErrorMessage(err, 2047, NULL);
//			throw_wstring(_T("CInternetException: ") + wstring(err));
//		}
		catch (const _com_error & err)
		{
			throw_wstring(_T("_com_error(") + wstring(err.ErrorMessage()) + _T(")"));
		}
		catch(const wstring & v)
		{
			throw_wstring(_T("wstring(") + v + _T(")"));
		}
		catch(const string & v)
		{
			throw_wstring(_T("string(") + atow(v) + _T(")"));
		}
		catch(const bool & v)
		{
			throw_wstring(_T("bool(") + wstring(v?_T("true"):_T("false")) + _T(")"));
		}
		catch(const int & v)
		{
			throw_wstring(_T("int(") + integerToWString(v) + _T(")"));
		}
		catch(const unsigned int & v)
		{
			throw_wstring(_T("unsigned int(") + integerToWString(v) + _T(")"));
		}
		catch(const char * v)
		{
			throw_wstring(_T("char *(") + atow(v) + _T(")"));
		}
		catch(const wchar_t * v)
		{
			throw_wstring(_T("wchar *(") + wstring(v) + _T(")"));
		}
		catch(std::domain_error & v)
		{
			throw_wstring(_T("domain_error: ") + atow(v.what()));
		}
		catch(std::invalid_argument & v)
		{
			throw_wstring(_T("invalid_argument: ") + atow(v.what()));
		}
		catch(std::length_error & v)
		{
			throw_wstring(_T("length_error: ") + atow(v.what()));
		}
		catch(std::out_of_range & v)
		{
			throw_wstring(_T("out_of_range: ") + atow(v.what()));
		}
		catch(std::range_error & v)
		{
			throw_wstring(_T("range_error: ") + atow(v.what()));
		}
		catch(std::overflow_error & v)
		{
			throw_wstring(_T("overflow_error: ") + atow(v.what()));
		}
		catch(std::underflow_error & v)
		{
			throw_wstring(_T("underflow_error: ") + atow(v.what()));
		}
		catch(const std::exception & except)
		{
			throw_wstring(_T("STL Exception: ") + atow(except.what()));
		}
		catch (StructuredException const & exception)
		{
			char	hexcode[20];
			sprintf(hexcode, "0x%08X", exception.what());
			char	wherecode[20];
			sprintf(wherecode, "0x%08X", exception.where());

			switch (exception.what())
			{
				case EXCEPTION_ACCESS_VIOLATION:
					throw_wstring(_T("Structured Exception [") + atow(hexcode) + _T("]: Access violation\nAt location: ") + atow(wherecode));
				case EXCEPTION_INT_DIVIDE_BY_ZERO:
					throw_wstring(_T("Structured Exception [") + atow(hexcode) + _T("]: Divide by zero\nAt location: ") + atow(wherecode));
				case EXCEPTION_STACK_OVERFLOW:
					throw_wstring(_T("Structured Exception [") + atow(hexcode) + _T("]: Stack overflow\nAt location: ") + atow(wherecode));
				default:
					throw_wstring(_T("Structured Exception [") + atow(hexcode) + _T("]: (unknown type)\nAt location: ") + atow(wherecode));
			}
		}
		catch(...)
		{
			throw_wstring(_T("unknown type(\?\?\?)"));
		}
	}
	catch (const wstring & err)
	{
		wstring	msg = 
			_T("An internal error has occurred:\n")
			_T("\n")
			_T("\"") + err + _T("\"\n")
			_T("\n")
			_T("This error has occurred on the secondary email checking\n")
			_T("thread, which means that 3DME will continue to function.\n")
			_T("However, you may continuously see this error repated over\n")
			_T("and over each time 3DME checks your email.\n")
			_T("\n")
			_T("It's also possible that this error is caused by a specific\n")
			_T("email in your mailbox. You may wish to retrieve your email\n")
			_T("using your email client, and delete the email off the server.\n");
		AfxMessageBox(msg.c_str());
		LOG(wtoa(msg), Logger::LOG_CRIT);
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	MailThread::checkMail(sMessage &message)
{
	AccountList	&accounts = dialog()->appPrefs().accounts();

	// How much time has gone by since the last timer?

	thisTime = time(NULL);
	elapsedTime = thisTime - lastTime;

	bool	statsFlag = false;

	// Go through and decrement the timer off of each entry

	for (AccountList::iterator i = accounts.begin(); i != accounts.end(); ++i)
	{
		if (i->disabled || i->deleted) continue;

		// The current account

		sAccount	&a = *i;

		// Time to check for new mail?

		bool	newFlag = false;

		if (!theApp.prefs().other()[_T("disableChecking")].booleanValue() && areWeOnline())
		{
			// Update the timers

			a.checkMailTimer -= elapsedTime;

			if (a.checkMailTimer <= 0)
			{
				a.checkMailTimer += a.checkMailSeconds;

				// Get out of debt scheme....

				if (a.checkMailTimer <= 0) a.checkMailTimer = 0;

				// Clear the stats before we update them

				dialog()->updateStats(_T(""));
				dialog()->updateStats(_T("Checking for mail on account: ") + a.accountName);
				statsFlag = true;

				wstring		err;
				EmailList	emailList;

				if (a.useWebmail)
				{
					WebMail	wm(a.emailList, emailList);
					wm.userName() = wtoa(a.userName);
					wm.userPass() = wtoa(a.userPass);
					wm.website().timeout() = a.timeout;
					wm.messageIDList() = "[$]";

					for (EmailList::const_iterator i = a.emailList.begin(); i != a.emailList.end(); ++i)
					{
						wm.messageIDList() += i->messageID + "[$]";
					}

					err = wm.execute(theApp.programPath() + _T("Webmail\\") + a.webmailScript + _T(".webmail"), WebMail::EP_RETRIEVE);
				}
				else
				{
					pop p;
					p.setTimeout(a.timeout);
					p.previewLines() = a.previewLines;
					err = p.queryMail(a.checkOnly, wtoa(a.serverAddress), a.serverPort, wtoa(a.userName), wtoa(a.userPass), a.emailList, emailList);
				}

				if (err.length())
				{
					a.lastError = err;
				}
				else
				{
					a.lastError = _T("");
					a.emailList.clear();

					a.emailList.countPendingEmails();

					emailList.countPendingEmails();

					a.emailList = emailList;
					a.emailList.countPendingEmails();

					// Queue autodelete messages for delettion

					a.emailList.markAutoDelete(dialog()->autodeleteSenders, dialog()->autodeleteSubjects);
					for (EmailList::const_iterator j = a.emailList.begin(); j != a.emailList.end(); ++j)
					{
						const sEmail &	email = *j;
						if (email.autoDeleteFlag)
						{
							dialog()->deleteMail(atow(email.messageID));
						}
					}
				}

				newFlag = a.emailList.countNewEmails() ? true:false;
				a.newPending = a.emailList.countPendingEmails() ? true:false;

				dialog()->updateTexts();
				dialog()->updateLists();
			}
		}

		// New mail?

		if (newFlag)
		{
			soundAlarm(a);
			a.reminderTimer = a.reminderSeconds;
		}

		// No new mail, how about a reminder?

		else if (a.newPending && a.reminderSoundEnable)
		{
			// Is it time?

			a.reminderTimer -= elapsedTime;

			if (a.reminderTimer <= 0)
			{
				a.reminderTimer += a.reminderSeconds;
				soundReminder(a);
			}
		}
	}

	// we're done with the stats window...

	if (statsFlag)
	{
		dialog()->updateStats(_T(""));
		dialog()->updateTexts();
		dialog()->updateLists();
	}

	// Update our 'last time' for the next run

	lastTime = time(NULL);
}

// ---------------------------------------------------------------------------------------------------------------------------------
void	MailThread::retrieveMail(sMessage &message)
{
#if 0
	ASSERT(message.account);

	// The account & Email

	sAccount	&account = *message.account;

	// Let the user know what's going on...

	dialog()->updateStats(_T("Retrieving email on account: ") + message.account->accountName);

	// Retrieve it

	pop		p;
	sEmail		newMail;
	wstring		err = p.retrieveMail(wtoa(account.serverAddress), account.serverPort, wtoa(account.userName), wtoa(account.userPass), message.messageID, newMail);
	lastRetrievedEmail() = newMail;
	lastRetrievedEmail().messageID = message.messageID;
	dialog()->updateStats(_T("Retrieving email on account: ") + message.account->accountName);

	// Error?

	if (err.length())
	{
		account.lastError = err;
	}

	// Delete it from the list

	else
	{
		for (EmailList::iterator i = account.emailList.begin(); i != account.emailList.end(); ++i)
		{
			if (i->messageID == message.messageID)
			{
				account.emailList.erase(i);
				dialog()->updateTexts();
				dialog()->updateLists();
				return;
			}
		}
	}
#endif
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	MailThread::deleteMail(sMessage &message)
{
	ASSERT(message.account);

	// The account & Email

	sAccount	&account = *message.account;

	// Let the user know what's going on...

	dialog()->updateStats(_T(""));
	dialog()->updateStats(_T("Deleting email on account: ") + message.account->accountName);

	// Delete it

	wstring		err;

	if (areWeOnline())
	{
		if (account.useWebmail)
		{
			WebMail	wm(account.emailList, account.emailList);
			wm.userName() = wtoa(account.userName);
			wm.userPass() = wtoa(account.userPass);
			wm.deleteMe() = message.messageID;
			wm.website().timeout() = account.timeout;

			wm.messageIDList() = "[$]";

			for (EmailList::const_iterator i = account.emailList.begin(); i != account.emailList.end(); ++i)
			{
				wm.messageIDList() += i->messageID + "[$]";
			}

			err = wm.execute(theApp.programPath() + _T("Webmail\\") + account.webmailScript + _T(".webmail"), WebMail::EP_DELETE);
		}
		else
		{
			pop p;
			err = p.deleteMail(wtoa(account.serverAddress), account.serverPort, wtoa(account.userName), wtoa(account.userPass), message.messageID);
		}
	}
	else
	{
		err = _T("Cannot delete while offline");
	}

	// Error?

	if (err.length())
	{
		account.lastError = err;
	}

	// Delete it from the list

	else
	{
		for (EmailList::iterator i = account.emailList.begin(); i != account.emailList.end(); ++i)
		{
			if (i->messageID == message.messageID)
			{
				account.emailList.erase(i);
				dialog()->updateTexts();
				dialog()->updateLists();
				break;
			}
		}
	}

	dialog()->updateStats(_T(""));
	dialog()->updateTexts();
	dialog()->updateLists();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	MailThread::terminate(sMessage &message)
{
	clearQueue();
	dialog()->mailThreadTerminated = true;
	AfxEndThread(0);
}

// --------------------------------------------------------------------------------------------------------------------------------

void	MailThread::soundAlarm(const sAccount &a)
{
	// Play the sound?

	if (!theApp.prefs().other()[_T("disableAlarms")].booleanValue() && a.alarmSoundEnable)
	{
		sndPlaySound(a.alarmSoundFile.c_str(), SND_ASYNC|SND_NOSTOP);
	}

	if (!theApp.prefs().other()[_T("disableAlarmRuns")].booleanValue())
	{
//		a.alarmRunParms.execute();
	}

	// Open the dialog?

	if (!theApp.prefs().other()[_T("disablePopups")].booleanValue() && a.alarmPopup)
	{
		dialog()->showWindow();
	}

	// Open the 3D popup?

	if (!theApp.prefs().other()[_T("disable3DPopups")].booleanValue() && a.alarm3DPopup)
	{
		dialog()->show3D();
	}
}

// --------------------------------------------------------------------------------------------------------------------------------

void	MailThread::soundReminder(const sAccount &a) const
{
	// Play the sound?

	if (!theApp.prefs().other()[_T("disableReminders")].booleanValue() && a.reminderSoundEnable)
	{
		sndPlaySound(a.reminderSoundFile.c_str(), SND_ASYNC|SND_NOSTOP);
	}

	if (!theApp.prefs().other()[_T("disableReminderRuns")].booleanValue())
	{
//		a.reminderRunParms.execute();
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------
// MailThread.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

