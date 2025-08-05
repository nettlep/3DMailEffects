// ---------------------------------------------------------------------------------------------------------------------------------
//  ______                 _ _ _____                  _               _____  _                            
// |  ____|               (_) |  __ \                (_)             |  __ \| |                           
// | |__   _ __ ___   __ _ _| | |__) |_ __  _____   ___  _____      __ |  | | | __ _      ___ _ __  _ __  
// |  __| | '_ ` _ \ / _` | | |  ___/| '__|/ _ \ \ / / |/ _ \ \ /\ / / |  | | |/ _` |    / __| '_ \| '_ \ 
// | |____| | | | | | (_| | | | |    | |  |  __/\ V /| |  __/\ V  V /| |__| | | (_| | _ | (__| |_) | |_) |
// |______|_| |_| |_|\__,_|_|_|_|    |_|   \___| \_/ |_|\___| \_/\_/ |_____/|_|\__, |(_) \___| .__/| .__/ 
//                                                                              __/ |        | |   | |    
//                                                                             |___/         |_|   |_|    
//
// Email Preview Dialog
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 12/26/2000 by Paul Nettle
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
#include "EmailPreviewDlg.h"
#include "EmailListbox.h"
#include "3DmeDlg.h"
#include "Pop.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

enum	{BORDER_WIDTH = 10};

// ---------------------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(EmailPreview, CDialog)
	//{{AFX_MSG_MAP(EmailPreview)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	ON_CBN_SELCHANGE(IDC_MULTIPART_COMBO, OnSelchangeMultipartCombo)
	ON_BN_CLICKED(IDC_SAVE, OnSaveAttach)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	ON_BN_CLICKED(IDC_FORWARD, OnForward)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
	ON_BN_CLICKED(IDC_HOME, OnHome)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_WRAP, OnWrap)
	ON_BN_CLICKED(IDC_LINKIFY, OnLinkify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

static	void	unHTML(string & text)
{
	for(;;)
	{
		string::size_type idx;

		idx = findNocase(text, "&gt;");
		if (idx != string::npos)
		{
			text.erase(idx, 4);
			text.insert(idx, ">");
			continue;
		}

		idx = findNocase(text, "&lt;");
		if (idx != string::npos)
		{
			text.erase(idx, 4);
			text.insert(idx, "<");
			continue;
		}

		idx = findNocase(text, "&quot;");
		if (idx != string::npos)
		{
			text.erase(idx, 6);
			text.insert(idx, "\"");
			continue;
		}

		idx = findNocase(text, "&nbsp;");
		if (idx != string::npos)
		{
			text.erase(idx, 6);
			text.insert(idx, " ");
			continue;
		}

		idx = findNocase(text, "&amp;");
		if (idx != string::npos)
		{
			text.erase(idx, 5);
			text.insert(idx, "&");
			continue;
		}

		break;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

static	void	textToHtml(string & text)
{
	string &		src = text;
	string::size_type	sidx = 0;
	char *			dst = new char[text.length() * 6];
	unsigned int		didx = 0;

	memset(dst, 0, text.length() * 6);

	while(sidx < src.length())
	{
		string::size_type idx = src.find_first_of("<>&\"", sidx);
		if (idx == string::npos) idx = src.length();

		// Copy the raw characters

		if (idx > sidx)
		{
			int	len = idx-sidx;
			memcpy(&dst[didx], &src[sidx], len);
			didx += len;
			sidx += len;
		}

		// Done?

		if (sidx >= src.length()) break;

		if (src[idx] == '<')
		{
			memcpy(dst+didx, "&lt;", 4);
			didx += 4;
			sidx++;
		}

		else if (src[idx] == '>')
		{
			memcpy(dst+didx, "&gt;", 4);
			didx += 4;
			sidx++;
		}

		else if (src[idx] == '\"')
		{
			memcpy(dst+didx, "&quot;", 6);
			didx += 6;
			sidx++;
		}

		else //if (src[idx] == '&')
		{
			memcpy(dst+didx, "&amp;", 5);
			didx += 5;
			sidx++;
		}
	}

	text = dst;
	delete[] dst;
}

// ---------------------------------------------------------------------------------------------------------------------------------

static	void	linkify(string & text)
{
	// Find the signs of a link...

	string::size_type	idx = 0;
	string			prefixString;
	string			suffixString;
	string			validChars      = "abcdefghijklmnopqrtsuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.!#$%*+-~=_?&;/";
	string			validEmailChars = "abcdefghijklmnopqrtsuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.!#$%*+-~=_?";

	for(;;)
	{
		string::size_type	start = string::npos;
		string::size_type	end = string::npos;

		// Find first character matches...
		//
		// This is a very specific search for the following strings:
		//
		// http://   https://  ftp://   news://   www.   ftp.   news.   @

		idx = text.find_first_of("hHfFnNwW@", idx);

		if (idx != string::npos)
		{
			// Quick-out for second-character mismatches

			if (text[idx+1] != 't' && text[idx+1] != 'T' &&
			    text[idx+1] != 'e' && text[idx+1] != 'E' &&
			    text[idx+1] != 'w' && text[idx+1] != 'W' &&
			    text[idx+0] != '@')
			{
				++idx;
				continue;
			}

			// Quick-out for third-character mismatches

			if (text[idx+2] != 't' && text[idx+2] != 'T' &&
			    text[idx+2] != 'p' && text[idx+2] != 'P' &&
			    text[idx+2] != 'w' && text[idx+2] != 'W')
			{
				++idx;
				continue;
			}

			// Grab just a portion of the string...

			string	head = text.substr(idx, 16);

			if (areEqualNocase(head, "http://", 7))
			{
				start = idx;
				end = text.find_first_not_of(validChars + "://", idx);
				prefixString = "<a href=\"";
				suffixString = "</a>";
			}
			else if (areEqualNocase(head, "https://", 8))
			{
				start = idx;
				end = text.find_first_not_of(validChars + "://", idx);
				prefixString = "<a href=\"";
				suffixString = "</a>";
			}
			else if (areEqualNocase(head, "ftp://", 6))
			{
				start = idx;
				end = text.find_first_not_of(validChars + "://", idx);
				prefixString = "<a href=\"";
				suffixString = "</a>";
			}
			else if (areEqualNocase(head, "news://", 7))
			{
				start = idx;
				end = text.find_first_not_of(validChars + "://", idx);
				prefixString = "<a href=\"";
				suffixString = "</a>";
			}
			else if (areEqualNocase(head, "www.", 4))
			{
				start = idx;
				end = text.find_first_not_of(validChars, idx);
				prefixString = "<a href=\"http://";
				suffixString = "</a>";
			}
			else if (areEqualNocase(head, "ftp.", 4))
			{
				start = idx;
				end = text.find_first_not_of(validChars, idx);
				prefixString = "<a href=\"ftp://";
				suffixString = "</a>";
			}
			else if (areEqualNocase(head, "news.", 5))
			{
				start = idx;
				end = text.find_first_not_of(validChars, idx);
				prefixString = "<a href=\"news://";
				suffixString = "</a>";
			}

			// A bit more work is required to detect an email address

			else if (head[0] == '@')
			{
				string::size_type	tmp = idx;
				if (tmp < start)
				{
					// Find the beginning

					string::size_type	b = text.find_last_not_of(validEmailChars, tmp == 0 ? 0:tmp - 1);
					if (b != string::npos) b++;

					// Find the end

					string::size_type	e = text.find_first_not_of(validEmailChars, tmp + 1);

					// Is there something on both sides of the @-sign?

					if (b != tmp && e > tmp + 1)
					{
						bool	isEmail = false;
						if (e == string::npos)
						{
							if (text.find(".", tmp) != string::npos) isEmail = true;
						}
						else
						{
							if (text.substr(tmp, e - tmp).find(".") != string::npos) isEmail = true;
						}

						if (isEmail)
						{
							start = b;
							end = e;
							prefixString = "<a href=\"mailto:";
							suffixString = "</a>";
						}
					}
				}
			}
			else
			{
				++idx;
				continue;
			}
		}

		// No links?

		if (start == string::npos) break;

		// Special case: Check for links that end with a period

		if (end != string::npos)
		{
			if (end > 0 && text[end-1] == '.') end--;
		}
		else
		{
			if (text[text.length()-1] == '.') end = text.length() - 1;
		}

		// Grab the link

		string	link;
		if (end == string::npos)
		{
			link = text.substr(start);
		}
		else
		{
			link = text.substr(start, end-start);
		}

		// Need to unHTML the link itself, since it may have been converted to html

		unHTML(link);

		// Put in the link

		text.insert(start, prefixString);
		start += prefixString.length();
		if (end == string::npos)
		{
			string	add = "\">" + link + suffixString;
			text += add;
			break;
		}
		else
		{
			end += prefixString.length();
			string	add = "\">" + link + suffixString;
			text.insert(end, add);
			idx = end + add.length();
		}
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

static	void	wrapText(string & text, const unsigned int columns)
{
	// Strip out the CR

	stripCarriageReturns(text);

	// Start wrapping

	string	result;

	// Reserve about 10% extra just for performance...

	result.reserve(text.length() + text.length() / 10);

	// Start scanning for line breaks

	string::size_type	idx = 0;
	while(idx < text.length())
	{
		// A line of text...

		string	thisLine;

		// Find the next LF

		string::size_type	lf = text.find("\n", idx);
		if (lf == string::npos)
		{
			thisLine = text.substr(idx);
			idx = text.length();
		}
		else
		{
			thisLine = text.substr(idx, lf-idx);
			idx = lf+1;
		}

		// Deal with tabs

		{
			string::size_type	tab = 0;
			for(;;)
			{
				tab = thisLine.find("\t", tab);
				if (tab == string::npos) break;
				thisLine.erase(tab, 1);
				int	count = 7 - (tab&7) + 1;
				string	spaces;
				while(count--) {spaces += " ";}
				thisLine.insert(tab, spaces);
			}
		}

		// Is a split required?

		if (thisLine.length() <= columns)
		{
			result += thisLine + "\r\n";
			continue;
		}

		// Split up 'thisline' into a number of lines that are no longer than 'columns' characters long

		while(thisLine.length() > columns)
		{
			// Try to break on a whitespace character

			string::size_type	space = thisLine.find_last_of(" \t\v\f", columns - 1);
			if (space == string::npos)
			{
				// No space, break at the next space

				space = thisLine.find_first_of(" \t\v\f");

				// If there's no space at all, put one at the end to simplify the following code

				if (space == string::npos)
				{
					space = thisLine.length();
					thisLine += " ";
				}
			}

			// Break the line

			result += thisLine.substr(0, space) + "\r\n";
			thisLine.erase(0, space + 1);
		}

		// Handle the leftover

		if (thisLine.length()) result += thisLine + "\r\n";
	}

	text = result;
}

// ---------------------------------------------------------------------------------------------------------------------------------

	EmailPreview::EmailPreview(C3DmeDlg *dlg, const sAccount &account, const sEmail &email, CWnd* pParent /*=NULL*/)
	: parent(*dlg), _account(account), _email(email), CDialog(EmailPreview::IDD, pParent)
{
	//{{AFX_DATA_INIT(EmailPreview)
	//}}AFX_DATA_INIT

	headerFont.CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_TT_ALWAYS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS, NULL);
	sized = false;
	toolTip = NULL;
	lastEnableState = true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

	EmailPreview::~EmailPreview()
{
	for (unsigned int i = 0; i < bitmaps.size(); i++)
	{
		DeleteObject(bitmaps[i]);
	}
	delete toolTip;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EmailPreview::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EmailPreview)
	DDX_Control(pDX, IDC_STATIC_STATUSBAR, statusStatic);
	DDX_Control(pDX, IDC_LINKIFY, linkifyCheckbox);
	DDX_Control(pDX, IDC_WRAP, wrapCheckbox);
	DDX_Control(pDX, IDC_STATIC_URL, urlStatic);
	DDX_Control(pDX, IDC_STOP, stopButton);
	DDX_Control(pDX, IDC_SAVE, saveButton);
	DDX_Control(pDX, IDC_REFRESH, refreshButton);
	DDX_Control(pDX, IDC_HOME, homeButton);
	DDX_Control(pDX, IDC_FORWARD, forwardButton);
	DDX_Control(pDX, IDC_BACK, backButton);
	DDX_Control(pDX, IDC_MULTIPART_COMBO, multipartCombo);
	DDX_Control(pDX, IDC_CLOSE, closeButton);
	DDX_Control(pDX, IDC_DELETE, deleteButton);
	DDX_Control(pDX, IDC_FROM_EDIT, fromEdit);
	DDX_Control(pDX, IDC_REPLYTO_EDIT, replyToEdit);
	DDX_Control(pDX, IDC_DATE_EDIT, dateEdit);
	DDX_Control(pDX, IDC_SUBJECT_EDIT, subjectEdit);
	DDX_Control(pDX, IDC_FROM_LABEL, fromLabel);
	DDX_Control(pDX, IDC_REPLYTO_LABEL, replyToLabel);
	DDX_Control(pDX, IDC_DATE_LABEL, dateLabel);
	DDX_Control(pDX, IDC_SUBJECT_LABEL, subjectLabel);
	DDX_Control(pDX, IDC_MIMEPART_LABEL, mimePartLabel);
	//}}AFX_DATA_MAP
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	EmailPreview::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Bitmaps for buttons

	bitmaps.push_back(LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BACK)));
	bitmaps.push_back(LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_FORWARD)));
	bitmaps.push_back(LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_STOP)));
	bitmaps.push_back(LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_REFRESH)));
	bitmaps.push_back(LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_HOME)));
	GetDlgItem(IDC_BACK)->SendMessage(BM_SETIMAGE, (WPARAM) IMAGE_BITMAP, (LPARAM) bitmaps[0]);
	GetDlgItem(IDC_FORWARD)->SendMessage(BM_SETIMAGE, (WPARAM) IMAGE_BITMAP, (LPARAM) bitmaps[1]);
	GetDlgItem(IDC_STOP)->SendMessage(BM_SETIMAGE, (WPARAM) IMAGE_BITMAP, (LPARAM) bitmaps[2]);
	GetDlgItem(IDC_REFRESH)->SendMessage(BM_SETIMAGE, (WPARAM) IMAGE_BITMAP, (LPARAM) bitmaps[3]);
	GetDlgItem(IDC_HOME)->SendMessage(BM_SETIMAGE, (WPARAM) IMAGE_BITMAP, (LPARAM) bitmaps[4]);

	int	windowX = theApp.prefs().other()[_T("previewWindowX")].integerValue();
	int	windowY = theApp.prefs().other()[_T("previewWindowY")].integerValue();
	int	windowW = theApp.prefs().other()[_T("previewWindowW")].integerValue();
	int	windowH = theApp.prefs().other()[_T("previewWindowH")].integerValue();

	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
	{
		AfxMessageBox(_T("You were holding down the control key, so 3DME\nhas reset your window position for you."));
		windowX = 12345;
		windowY = 12345;
	}

	if (windowX != 12345 && windowY != 12345)
	{
		CRect	r;
		GetWindowRect(r);
		r.left = windowX;
		r.top = windowY;
		r.bottom = r.top + windowH;
		r.right = r.left + windowW;
		MoveWindow(r);
	}

	// Tooltips

	toolTip = new CToolTipCtrl;

	if(toolTip && toolTip->Create(this))
	{
		toolTip->AddTool(&linkifyCheckbox, _T("Locates links within text emails and enables them (for text-only emails)"));
		toolTip->AddTool(&wrapCheckbox, _T("Force the text to wrap (for text-only emails)"));
		toolTip->AddTool(&backButton, _T("Previous page"));
		toolTip->AddTool(&forwardButton, _T("Next page"));
		toolTip->AddTool(&stopButton, _T("Stop browser activity"));
		toolTip->AddTool(&refreshButton, _T("Refresh the content in the browser"));
		toolTip->AddTool(&homeButton, _T("Go to your home page"));
		toolTip->AddTool(&multipartCombo, _T("Select which part or attachment to view"));
		toolTip->AddTool(&saveButton, _T("Save this part or attachment"));
		toolTip->AddTool(&deleteButton, _T("Delete this email from the server"));
		toolTip->AddTool(&closeButton, _T("Close this window"));
		toolTip->AddTool(&urlStatic, _T("Shows the current URL"));
		toolTip->Activate(TRUE);
	}

	// Add the mime types to the drop-down
	
	int	firstIndex = -1;
	int	curIndex = 0;

	// Add headers...

	multipartCombo.AddString(_T("SMTP Headers (raw)"));
	curIndex++;

	const vector<Parts>	&pList = email().mimeHeader.parts();
	for (vector<Parts>::const_iterator i = pList.begin(); i != pList.end(); ++i)
	{
		// Grab the first HTML-based view, if exists

		if (i->isHtml && firstIndex == -1)
		{
			firstIndex = curIndex;
		}

		wstring	desc = i->description;
		if (i->fileName.length())
		{
			desc = _T("Attachment: ") + desc;
		}

		if (!desc.length())
		{
			desc = _T("Text/plain");
		}

		multipartCombo.AddString(desc.c_str());

		curIndex++;
	}

	if (pList.size() == 0)
	{
		multipartCombo.AddString(_T("Text/plain"));
	}

	if (firstIndex == -1) firstIndex = 1;
	multipartCombo.SetCurSel(firstIndex);

	// Get the preferences

	wrapCheckbox.SetCheck(theApp.prefs().other()[_T("previewWrap")].booleanValue());
	linkifyCheckbox.SetCheck(theApp.prefs().other()[_T("previewLinkify")].booleanValue());

	populatePreviewText();

	wstring	title = _T("Email preview for ") + account().accountName;
	SetWindowText(title.c_str());

	// Set the header fields

	fromEdit.SetWindowText(email().mimeHeader.from().c_str());
	replyToEdit.SetWindowText(email().mimeHeader.replyTo().c_str());
	dateEdit.SetWindowText(email().mimeHeader.date().c_str());
	subjectEdit.SetWindowText(email().mimeHeader.subject().c_str());

	// Set the timer used to update buttons and stuff...

	SetTimer(0, 100, NULL);

	return TRUE;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EmailPreview::OnTimer(UINT nIDEvent) 
{
	// Check for activity

	WebBrowser2 & browser = *(WebBrowser2 *) GetDlgItem(IDC_BROWSER);
	bool	enableStop = browser.get_Busy() ? true:false;
	if (lastEnableState != enableStop)
	{
		lastEnableState = enableStop;
		stopButton.EnableWindow(enableStop ? TRUE:FALSE);
	}

#if 0 // this seems to be useless?!
	try
	{
		string	st = browser.GetStatusText();
		if (lastStatusText != st)
		{
			lastStatusText = st;
			statusStatic.SetWindowText(st.c_str());
		}
	}
	catch(...)
	{
		lastStatusText = "";
		statusStatic.SetWindowText(_T(""));
	}
#endif
	// Check for a new URL

	wstring	url = browser.get_LocationURL();
	if ((url.length() >= 7 && url.substr(0, 7) == _T("file://"))) url = _T("Email contents");

	if (url != lastURL)
	{
		urlStatic.SetWindowText(url.c_str());
		lastURL = url;
	}

	CDialog::OnTimer(nIDEvent);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EmailPreview::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	WebBrowser2 & browser = *(WebBrowser2 *) GetDlgItem(IDC_BROWSER);
	if (deleteButton.GetSafeHwnd() && &browser && browser.GetSafeHwnd())
	{
		int	fieldHeight = 20;

		CRect	labelRect;
		fromLabel.GetClientRect(labelRect);

		CRect	fieldRect;
		fromEdit.GetClientRect(fieldRect);

		CRect	clientRect;
		GetClientRect(clientRect);

		CRect	listRect;
		multipartCombo.GetWindowRect(listRect);

		CRect	wrapRect;
		wrapCheckbox.GetWindowRect(wrapRect);

		CRect	linkifyRect;
		linkifyCheckbox.GetWindowRect(linkifyRect);

		CRect	saveRect;
		saveButton.GetWindowRect(saveRect);

		CRect	deleteRect;
		deleteButton.GetWindowRect(deleteRect);

		CRect	closeRect;
		closeButton.GetWindowRect(closeRect);

		// Close button -- bottom right of the window
		{
			CRect	rect = clientRect;
			rect.right -= BORDER_WIDTH;
			rect.bottom -= BORDER_WIDTH;
			rect.top = rect.bottom - closeRect.Height();
			rect.left = rect.right- closeRect.Width();
			closeButton.MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), TRUE);
			closeRect = rect;
		}

		// Delete button -- bottom left of the window
		{
			CRect	rect = clientRect;
			rect.left += BORDER_WIDTH;
			rect.right = rect.left + deleteRect.Width();
			rect.bottom -= BORDER_WIDTH;
			rect.top = rect.bottom - deleteRect.Height();
			deleteButton.MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), TRUE);
			deleteRect = rect;
		}

		// Status bar -- between delete and close buttons
		{
			CRect	rect = clientRect;
			rect.right = closeRect.left - BORDER_WIDTH;
			rect.left = deleteRect.right + BORDER_WIDTH;
			rect.bottom -= BORDER_WIDTH;
			rect.top = rect.bottom - deleteRect.Height();
			statusStatic.MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), TRUE);
		}

		// From, Reply-To, Date, Subject labels
		{
			CRect	rect = clientRect;
			rect.left += BORDER_WIDTH;
			rect.top += BORDER_WIDTH;
			rect.right = rect.left + labelRect.Width();
			rect.bottom = rect.top + fieldHeight;

			fromLabel.MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), TRUE);

			rect.OffsetRect(0, fieldHeight + 3);
			replyToLabel.MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), TRUE);

			rect.OffsetRect(0, fieldHeight + 3);
			dateLabel.MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), TRUE);

			rect.OffsetRect(0, fieldHeight + 3);
			subjectLabel.MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), TRUE);

			rect.OffsetRect(0, fieldHeight + 3);
			mimePartLabel.MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), TRUE);

			labelRect = rect;
		}

		// From, Reply-To, Date, Subject fields
		{
			CRect	rect = clientRect;
			rect.left = labelRect.right + 3;
			rect.top += BORDER_WIDTH;
			rect.right -= BORDER_WIDTH;
			rect.bottom = rect.top + fieldHeight;

			fromEdit.MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), TRUE);

			rect.OffsetRect(0, fieldHeight + 3);
			replyToEdit.MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), TRUE);

			rect.OffsetRect(0, fieldHeight + 3);
			dateEdit.MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), TRUE);

			rect.OffsetRect(0, fieldHeight + 3);
			subjectEdit.MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), TRUE);

			fieldRect = rect;
		}

		// Combo box
		{
			CRect	rect = clientRect;
			rect.left += labelRect.right + 3;
			rect.top = labelRect.top;
			rect.right -= (BORDER_WIDTH + wrapRect.Width() + 3 + linkifyRect.Width() + 3 + saveRect.Width() + 3);
			rect.bottom = rect.top + listRect.Height();
			multipartCombo.MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), TRUE);
			listRect = rect;
		}

		// Wrap checkbox
		{
			CRect	rect;
			rect.left = listRect.right + 3;
			rect.top = listRect.top;
			rect.right = rect.left + wrapRect.Width();
			rect.bottom = listRect.bottom;
			wrapCheckbox.MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), TRUE);
			wrapRect = rect;
		}

		// Linkify checkbox
		{
			CRect	rect;
			rect.left = wrapRect.right + 3;
			rect.top = wrapRect.top;
			rect.right = rect.left + linkifyRect.Width();
			rect.bottom = wrapRect.bottom;
			linkifyCheckbox.MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), TRUE);
			linkifyRect = rect;
		}

		// Save as button
		{
			CRect	rect;
			rect.left = linkifyRect.right + 3;
			rect.top = linkifyRect.top;
			rect.right = rect.left + saveRect.Width();
			rect.bottom = linkifyRect.bottom;
			saveButton.MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), TRUE);
			saveRect = rect;
		}

		// Browser buttons...

		CRect	buttonRect;
		backButton.GetWindowRect(buttonRect);
		int	buttonWidth = buttonRect.Width();

		// Back button
		{
			CRect	rect = clientRect;
			rect.left += BORDER_WIDTH;
			rect.right = rect.left + buttonWidth;
			rect.top = listRect.bottom + 3;
			rect.bottom = rect.top + buttonRect.Height();
			backButton.MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), TRUE);
			buttonRect = rect;
		}

		// Forward

		buttonRect.left += buttonWidth + 3;
		buttonRect.right += buttonWidth + 3;
		forwardButton.MoveWindow(buttonRect.left, buttonRect.top, buttonRect.Width(), buttonRect.Height(), TRUE);

		// Stop

		buttonRect.left += buttonWidth + 3;
		buttonRect.right += buttonWidth + 3;
		stopButton.MoveWindow(buttonRect.left, buttonRect.top, buttonRect.Width(), buttonRect.Height(), TRUE);

		// Refresh

		buttonRect.left += buttonWidth + 3;
		buttonRect.right += buttonWidth + 3;
		refreshButton.MoveWindow(buttonRect.left, buttonRect.top, buttonRect.Width(), buttonRect.Height(), TRUE);

		// Home

		buttonRect.left += buttonWidth + 3;
		buttonRect.right += buttonWidth + 3;
		homeButton.MoveWindow(buttonRect.left, buttonRect.top, buttonRect.Width(), buttonRect.Height(), TRUE);

		// URL static

		buttonRect.left += buttonWidth + 3;
		buttonRect.right = clientRect.right - BORDER_WIDTH;
		urlStatic.MoveWindow(buttonRect.left, buttonRect.top, buttonRect.Width(), buttonRect.Height(), TRUE);

		// Browser window
		{
			CRect	rect = clientRect;
			rect.left += BORDER_WIDTH;
			rect.right -= BORDER_WIDTH;
			rect.top = buttonRect.bottom + 3;
			rect.bottom -= BORDER_WIDTH + (closeRect.Height() + 6);
			browser.MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), TRUE);
		}

		// Redraw everything, since this seems to confuse Windows... (just redrawing the buttons should work, and won't flicker the window)

		closeButton.RedrawWindow();
		saveButton.RedrawWindow();
		deleteButton.RedrawWindow();
		multipartCombo.RedrawWindow();
		backButton.RedrawWindow();
		forwardButton.RedrawWindow();
		stopButton.RedrawWindow();
		refreshButton.RedrawWindow();
		homeButton.RedrawWindow();
		wrapCheckbox.RedrawWindow();
		linkifyCheckbox.RedrawWindow();
		urlStatic.RedrawWindow();
		browser.RedrawWindow();

		sized = true;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EmailPreview::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	lpMMI->ptMinTrackSize.x = 450;
	lpMMI->ptMinTrackSize.y = 300;
	
	CDialog::OnGetMinMaxInfo(lpMMI);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EmailPreview::OnDelete() 
{
	C3DmeDlg *dialog = (C3DmeDlg *) GetParent();

	if (theApp.prefs().global()[_T("confirmDeletes")].booleanValue() == false || MessageBox(_T("Are you sure you wish to delete this email?\n\nThis will remove the email from your server permanently.\n\nThere is no way to undo this action."), _T("Just makin' sure!"), MB_YESNO) == IDYES)
	{
		ShowWindow(SW_HIDE);
		bool	del = parent.deleteMail(atow(email().messageID));

		if (del)	OnOK();
		else		ShowWindow(SW_SHOW);
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EmailPreview::OnClose() 
{
	OnOK();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EmailPreview::OnOK() 
{
	CRect	r;
	GetWindowRect(r);
	theApp.prefs().other()[_T("previewWindowX")].integerValue() = r.left;
	theApp.prefs().other()[_T("previewWindowY")].integerValue() = r.top;
	theApp.prefs().other()[_T("previewWindowW")].integerValue() = r.Width();
	theApp.prefs().other()[_T("previewWindowH")].integerValue() = r.Height();
	theApp.prefs().other().update();

	KillTimer(0);
	CDialog::OnOK();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EmailPreview::OnCancel() 
{
	OnOK();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EmailPreview::OnBack() 
{
	WebBrowser2 & browser = *(WebBrowser2 *) GetDlgItem(IDC_BROWSER);
	try {browser.GoBack();}catch(...){}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EmailPreview::OnForward() 
{
	WebBrowser2 & browser = *(WebBrowser2 *) GetDlgItem(IDC_BROWSER);
	try {browser.GoForward();}catch(...){}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EmailPreview::OnStop() 
{
	WebBrowser2 & browser = *(WebBrowser2 *) GetDlgItem(IDC_BROWSER);
	try {browser.Stop();}catch(...){}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EmailPreview::OnRefresh() 
{
	WebBrowser2 & browser = *(WebBrowser2 *) GetDlgItem(IDC_BROWSER);
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		COleVariant varRefresh((long) REFRESH_COMPLETELY);
		try {browser.Refresh2(&varRefresh);}catch(...){}
	}
	else
	{
		try {browser.Refresh();}catch(...){}
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EmailPreview::OnHome() 
{
	WebBrowser2 & browser = *(WebBrowser2 *) GetDlgItem(IDC_BROWSER);
	try {browser.GoHome();}catch(...){}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EmailPreview::OnSelchangeMultipartCombo() 
{
	populatePreviewText();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EmailPreview::OnSaveAttach() 
{
	// Which mime part?

	int curSel = multipartCombo.GetCurSel();
	if (curSel == LB_ERR) curSel = 0;

	// Get the filename

	wstring		partFilename;
	if (curSel == 0)
	{
		partFilename = _T("Headers.txt");
	}
	else
	{
		partFilename = email().mimeHeader.parts()[curSel-1].fileName;
	}

	// Ask them where to put it

	wchar_t		fname[MAX_PATH];
	wcscpy(fname, partFilename.c_str());
	wchar_t		filters[] = _T("All files (*.*)\0*.*\0\0");
	OPENFILENAME	of;
	memset(&of, 0, sizeof(OPENFILENAME));

	of.lStructSize  = sizeof(OPENFILENAME);
	of.hwndOwner    = GetSafeHwnd();
	of.lpstrFilter  = filters;
	of.nFilterIndex = 1;
	of.lpstrFile    = fname;
	of.nMaxFile     = MAX_PATH - 1;
	of.lpstrTitle   = _T("Save attachment");
	of.Flags        = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_OVERWRITEPROMPT;

	if (!GetSaveFileName(&of)) return;

	// Strip the path

	FILE	*fp = _wfopen(of.lpstrFile, _T("w+b"));
	if (!fp)
	{
		AfxMessageBox(_T("Unable to create the file for this attachment."));
		return;
	}

	if (curSel > 0)
	{
		// Adjust the index since the list always includes the headers

		--curSel;

		const Parts	&p = email().mimeHeader.parts()[curSel];
		if (p.binaryData)
		{
			if (fwrite(p.binaryData, p.binaryDataSize, 1, fp) != 1)
			{
				AfxMessageBox(_T("Unable to write the binary attachment data to the file."));
			}
		}
		else if (p.data.size())
		{
			string	temp = convertToString(p.data);
			if (fwrite(temp.c_str(), temp.length(), 1, fp) != 1)
			{
				AfxMessageBox(_T("Unable to write the attachment data to the file."));
			}
		}
		else
		{
			AfxMessageBox(_T("Unable to write the attachment data to the file (no attachment data stored.)"));
		}
	}
	else
	{
		StringList	sl = email().mimeHeader.header();
		for (StringList::iterator i = sl.begin(); i != sl.end(); ++i)
		{
			if (!i->length()) break;
			if (fwrite(i->c_str(), i->length(), 1, fp) != 1)
			{
				AfxMessageBox(_T("Unable to write the header data to the file."));
				break;
			}
			if (fwrite("\r\n", 2, 1, fp) != 1)
			{
				AfxMessageBox(_T("Unable to write the header data to the file."));
				break;
			}
		}
	}

	fclose(fp);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EmailPreview::OnWrap() 
{
	// Save this preference

	theApp.prefs().other()[_T("previewWrap")].booleanValue() = wrapCheckbox.GetCheck() ? true:false;
	theApp.prefs().other().update();

	// Force an update

	populatePreviewText();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EmailPreview::OnLinkify() 
{
	// Save this preference

	theApp.prefs().other()[_T("previewLinkify")].booleanValue() = linkifyCheckbox.GetCheck() ? true:false;
	theApp.prefs().other().update();

	// Force an update

	populatePreviewText();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EmailPreview::populatePreviewText()
{
	// Convert the part data to a long string

	body().erase();

	int curSel = multipartCombo.GetCurSel();
	if (curSel == LB_ERR) curSel = 0;

	bool	isImage = false;
	bool	isHtml = false;

	// Header?

	if (curSel == 0)
	{
		StringList	sl = email().mimeHeader.header();
		for (StringList::iterator i = sl.begin(); i != sl.end(); ++i)
		{
			string &	s = *i;
			if (!s.length()) break;
			body() += s;
			body() += "\r\n";
		}
	}
	else
	{
		// Adjust the index since headers are always first

		--curSel;

		if (email().mimeHeader.parts().size())
		{
			const Parts	&p = email().mimeHeader.parts()[curSel];
			saveButton.EnableWindow(TRUE);

			if (p.binaryDataSize > 1 && p.binaryData[0] == 'M' && p.binaryData[1] == 'Z')
			{
				isHtml = true;
				body() =
				"<html>\r\n"
				"	<head>\r\n"
				"		<STYLE type=\"text/css\">\r\n"
				"			BODY { color: 000000; font-style: normal; font-family: verdana, tahoma, helvetica; font-size: 11px; text-decoration: none; font-weight: normal; margin-top: 0px; margin-bottom: 0px; }\r\n"
				"			TD   { color: 000000; font-style: normal; font-family: verdana, tahoma, helvetica; font-size: 11px; text-decoration: none; font-weight: normal; margin-top: 0px; margin-bottom: 0px; }\r\n"
				"			A       { color: 000040; font-weight: normal; font-size: 13; text-decoration: none; }\r\n"
				"			A:hover { text-decoration: underline; }\r\n"
				"			font.s  { font-style: normal; font-size: 11px; font-weight: normal; }\r\n"
				"			font.sb { font-style: normal; font-size: 11px; font-weight: bold;   }\r\n"
				"			font.si { font-style: italic; font-size: 11px; font-weight: normal; }\r\n"
				"			font.m  { font-style: normal; font-size: 14px; font-weight: normal; }\r\n"
				"			font.mb { font-style: normal; font-size: 14px; font-weight: bold;   }\r\n"
				"			font.mi { font-style: italic; font-size: 14px; font-weight: normal; }\r\n"
				"			font.l  { font-style: normal; font-size: 20px; font-weight: normal; }\r\n"
				"			font.lb { font-style: normal; font-size: 20px; font-weight: bold;   }\r\n"
				"			font.li { font-style: italic; font-size: 20px; font-weight: normal; }\r\n"
				"			font.copyright { color: a0a0a0; }\r\n"
				"		</STYLE>\r\n"
				"	</head>\r\n"
				"	<body bgcolor=#ffffff>\r\n"
				"		<table width=100% height=100% border=0><tr valign=middle><td align=center>\r\n"
				"			<table width=75%><tr><td>\r\n"
				"				<center>\r\n"
				"					<font class=mb color=#800000>SECURITY ALERT - EXECUTABLE FILE DETECTED</font>\r\n"
				"				</center>\r\n"
				"				<hr>\r\n"
				"				<br>\r\n"
				"				<font class=lb>The attached file is an executable and may be a virus!</font>\r\n"
				"				<ul>\r\n"
				"					One of the most popular ways to get a virus is to run a program that was attached to\r\n"
				"					an email. For this reason, many email programs automatically delete executable programs\r\n"
				"					from email attachments.\r\n"
				"					<br>\r\n"
				"					<br>\r\n"
				"					With 3DME, you still have the option to save the attachment. Before you do, please\r\n"
				"					heed the following warnings...\r\n"
				"				</ul>\r\n"
				"				<font class=mb>A virus can be very tricky</font>\r\n"
				"				<ul>\r\n"
				"					A virus can disguise itself as an image, a web page, a zip file or a file of any other type. This is\r\n"
				"					possible because by default, Windows hides common file extensions. Virus writers know this and take\r\n"
				"					advantage of this. So for your records, here is the entire filename, without any hidden file extensions:\r\n"
				"					<br>\r\n"
				"					<br>\r\n"
				"					<ul>\r\n"
				"						<font class=m>" + wtoa(p.fileName) + "</font>\r\n"
				"					</ul>\r\n"
				"					<br>\r\n"
				"					If you see more than one file extension on the filename above, consider it a virus. Do not save it. Simply\r\n"
				"					delete this email message. As long as you do not save this attachment and execute it, you cannot get infected by it.\r\n"
				"					<br>\r\n"
				"					<br>\r\n"
				"				</ul>\r\n"
				"				<font class=mb>A virus can pretend to send an email from somebody you know</font>\r\n"
				"				<ul>\r\n"
				"					It's not difficult to forge the sender's address of an email. Spammers do it all the time and so do virii.\r\n"
				"					Some virii are smart enough to choose random subject lines from an infected computer's email history, in\r\n"
				"					an attempt to make the email appear more legit.\r\n"
				"				</ul>\r\n"
				"				<font class=mb>Don't trust people that send you executables and say they are safe</font>\r\n"
				"				<ul>\r\n"
				"					Does this person <i>really</i> know the file is safe? How would they know it? Did they create the executable\r\n"
				"					themselves? Just because a person may know more than you do about computers doesn't mean they understand\r\n"
				"					them well enough to examine the binary code within an executable file to determine if the file is truly a virus\r\n"
				"					or not.\r\n"
				"				</ul>\r\n"
				"				<font class=mb>Of course, there are always exceptions to the rule</font>\r\n"
				"				<ul>\r\n"
				"					For example, if you were expecting this email to arrive with an executable file in it that you requested from\r\n"
				"					a trusted source.\r\n"
				"					<br>\r\n"
				"					<br>\r\n"
				"					If you still feel safe, you may save the attachment like normal.\r\n"
				"					<br>\r\n"
				"					<br>\r\n"
				"					Consider yourself warned! :)\r\n"
				"				</ul>\r\n"
				"				- The Single-Person Staff of 3D Mail Effects\r\n"
				"			</td></tr></table>\r\n"
				"		</td></tr></table>\r\n"
				"	</body>\r\n"
				"</html>\r\n";
			}
			else
			{
				body() = convertToString(p.data);
				isHtml = p.isHtml;

				if (p.fileName.length() && p.binaryData && p.binaryDataSize)
				{
					wstring::size_type	idx = p.fileName.rfind(_T("."));
					if (idx != wstring::npos)
					{
						wstring	ext = p.fileName.substr(idx);
						lowerString(ext);

						if (ext == _T(".jpeg") || ext == _T(".jpg") || ext == _T(".gif") || ext == _T(".bmp") || ext == _T(".png"))
						{
							wstring	tempPath = getTempDir() + _T("\\") + p.fileName;
							FILE	*fp = _wfopen(tempPath.c_str(), _T("w+b"));
							if (fp)
							{
								if (fwrite(p.binaryData, p.binaryDataSize, 1, fp) == 1) isImage = true;
								body() = "<html><body bgcolor=#000000><table width=100% height=100% border=0><tr valign=center><td align=center><img src=\"file://" + wtoa(tempPath) + "\"></td></tr></table></body></html>";
								fclose(fp);
							}
						}
					}
				}
			}
		}
		else
		{
			body() = "This message is empty, was only partially retrieved from the server or the server timed out and did not report the entire email";
		}
	}

	// Only enable the wrap checkbox if it is a text message

	bool	enableCheckbox = false;

	// Write the file...

	wstring	tempFilename = getTempFilename() + _T(".html");
	if (tempFilename.length())
	{
		// Wrap text messages into an HTML page

		if (!isHtml && !isImage)
		{
			// Strip out the excepps crap

			stripLeadingWS(body());
			stripTrailingWS(body());

			// Wrap?

			enableCheckbox = true;
			if (wrapCheckbox.GetCheck()) wrapText(body(), 80);

			// Convert from html to text

			textToHtml(body());

			// Linkify

			if (linkifyCheckbox.GetCheck()) linkify(body());

			// Put it in a webpage

			body() =
				"<html><body><font face=\"FixedSys, Courrier New, Courier\" size=3><pre>\r\n"
				+ body() +
				"</pre></font></body></html>";
		}

		FILE *fp = _wfopen(tempFilename.c_str(), _T("wb"));
		if (fp)
		{
			fwrite(body().c_str(), body().length(), 1, fp);
			fclose(fp);
		}

		static	wstring	url;
		url = _T("file://") + tempFilename + _T("/");
		WebBrowser2 & browser = *(WebBrowser2 *) GetDlgItem(IDC_BROWSER);
		browser.Navigate(url.c_str(), NULL, NULL, NULL, NULL);
	}

	// Enable the wrapping checkbox?

	wrapCheckbox.EnableWindow(enableCheckbox ? TRUE:FALSE);
	linkifyCheckbox.EnableWindow(enableCheckbox ? TRUE:FALSE);
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	EmailPreview::PreTranslateMessage(MSG* pMsg) 
{
	if (NULL != toolTip) toolTip->RelayEvent(pMsg);
	
	return CDialog::PreTranslateMessage(pMsg);
}

// ---------------------------------------------------------------------------------------------------------------------------------
// EmailPreviewDlg.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
