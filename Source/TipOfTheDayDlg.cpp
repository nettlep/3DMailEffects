// ---------------------------------------------------------------------------------------------------------------------------------
//  _______ _        ____   __ _______ _          _____              _____  _                            
// |__   __(_)      / __ \ / _|__   __| |        |  __ \            |  __ \| |                           
//    | |   _ _ __ | |  | | |_   | |  | |__   ___| |  | | __ _ _   _| |  | | | __ _      ___ _ __  _ __  
//    | |  | | '_ \| |  | |  _|  | |  | '_ \ / _ \ |  | |/ _` | | | | |  | | |/ _` |    / __| '_ \| '_ \ 
//    | |  | | |_) | |__| | |    | |  | | | |  __/ |__| | (_| | |_| | |__| | | (_| | _ | (__| |_) | |_) |
//    |_|  |_| .__/ \____/|_|    |_|  |_| |_|\___|_____/ \__,_|\__, |_____/|_|\__, |(_) \___| .__/| .__/ 
//           | |                                                __/ |          __/ |        | |   | |    
//           |_|                                               |___/          |___/         |_|   |_|    
//
// Show a tip of the day
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 12/27/2000 by Paul Nettle
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
#include "3dme.h"
#include "TipOfTheDayDlg.h"
#include "WinDIB.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

const	int	TipOfTheDayDlg::borderWidth = 10;
const	int	TipOfTheDayDlg::lineGap = 3;
const	wchar_t	* TipOfTheDayDlg::tips[] =
{
	_T("Welcome to 3D Mail Effects, the only mail program that lets you know ")
	_T("new mail has arrived with cool 3D effects!\n")
	_T("\n")
	_T("Since this is your first time running the program, you'll automatically ")
	_T("be asked to setup your first email account.\n")
	_T("\n")
	_T("Thanks for using 3D Mail Effects!")
	,
	_T("No animals were harmed in the making of this program, especially ")
	_T("not two dogs named Annie & Sami. ;)\n")
	,
	_T("You can click and drag the 3D popups to place them where you want them to ")
	_T("be and they'll remember where you put them. You can right-click on the ")
	_T("object to change its settings or close it. You can also hit the 'escape' ")
	_T("key to close it.")
	,
	_T("When you mark messages as read, it only applies to the messages ")
	_T("that are currently visible in the email list.\n")
	_T("\n")
	_T("If you want to mark all messages as read, make sure you have '-- All ")
	_T("accounts --' selected from within the context-menu.")
	,
	_T("If you know everything is configured correctly but still can't get your ")
	_T("email, even though you specifically tell it to, then make sure you don't ")
	_T("have 'Disable checking for all email' selected in the context menu.")
	,
	_T("If you are feeling brave and dislike the 'Are you sure?' prompts ")
	_T("each time you delete an email, you can disable that in the preferences. ")
	_T("Just deselect the option 'Confirm deletes'.\n")
	_T("\n")
	_T("This is dangerous, because there's no way to undo this, so consider yourself ")
	_T("warned!")
	,
	_T("Check the about-box for a link to find new skins.")
	,
	_T("It's a lot harder than you would think to come up with a lot of good ")
	_T("'tips of the day'!")
	,
	_T("Making skins is fun, and especially easy with 3D Mail Effects. ")
	_T("To get started, just look at one of the existing *.skin files in the 'Skins' ")
	_T("directory (where the program is stored) -- it's a self-documented text file.\n")
	_T("\n")
	_T("You can also add models and materials. Just look in the program directory ")
	_T("to see where they go.")
	,
	_T("After using the mouse or keyboard to select an email (or series ")
	_T("of emails) from the email list, you can use the DEL key to delete ")
	_T("selected emails, or the space bar to preview them.\n")
	_T("\n")
	_T("Note that you can only preview one email at a time.")
	,
	_T("THE SKY IS FALLING!!!")
	_T("\n\n")
	_T("Just kidding. :)")
	,
	_T("You can drag the 3D popup around, or you can right-click on it for options.")
	,
	_T("Switching to another program while the keyboard LEDs are blinking will automatically ")
	_T("stop them from blinking until you switch back to 3D Mail Effects.\n")
	_T("\n")
	_T("Also, num-lock will only blink on WindowsNT or Windows2000 machines.")
	,
	_T("There is an easter-egg in this program.\n")
	_T("\n")
	_T("Can you find it?")
	,
	_T("You can right-click on the tray icon to access most-features, or just ")
	_T("double-click on the tray icon to open the application.")
	,
	_T("You can double-click on an email to preview it, or use the context menu ")
	_T("by right-clicking on it.\n")
	_T("\n")
	_T("Make sure that only one email is selected, or the preview option in the ")
	_T("context menu will not be selectable.")
	,
	_T("It might not look like it, but the preview window is resizable.")
	,
	_T("This program consists of roughly 20,000 lines of carfully written, hand-crafted ")
	_T("source code.")
	,
	NULL
};

// ---------------------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(TipOfTheDayDlg, CDialog)
	//{{AFX_MSG_MAP(TipOfTheDayDlg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_NEXT_TIP, OnNextTip)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	TipOfTheDayDlg::TipOfTheDayDlg(CWnd* pParent /*=NULL*/)
	: CDialog(TipOfTheDayDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(TipOfTheDayDlg)
	showTips = TRUE;
	//}}AFX_DATA_INIT

	background.read(theApp.programPath() + _T("totd.bmp"));
	textRect.left   = 50;
	textRect.top    = 80;
	textRect.right  = 381;
	textRect.bottom = 191;
}

// ---------------------------------------------------------------------------------------------------------------------------------

	TipOfTheDayDlg::~TipOfTheDayDlg()
{
	tipFont.DeleteObject();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	TipOfTheDayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TipOfTheDayDlg)
	DDX_Control(pDX, IDOK, okButton);
	DDX_Control(pDX, IDC_NEXT_TIP, nextTipButton);
	DDX_Control(pDX, IDC_SHOW_TIPS, showTipsButton);
	DDX_Check(pDX, IDC_SHOW_TIPS, showTips);
	//}}AFX_DATA_MAP
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	TipOfTheDayDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	showTipsButton.SetCheck(showTips);

	// Current window position & size

	CRect	clientRect;
	GetClientRect(clientRect);
	CRect	windowRect;
	GetWindowRect(windowRect);

	// New window position & size

	windowRect.right  += background.width()  - (clientRect.right  - clientRect.left);
	windowRect.bottom += background.height() - (clientRect.bottom - clientRect.top); 

	// Account for borders and dialog buttons (at the bottom)

	windowRect.right  += borderWidth * 2;
	windowRect.bottom += borderWidth * 2;
	windowRect.bottom += 40; 
	MoveWindow(windowRect);

	// Move the controls

	{
		CRect	tempRect;
		GetClientRect(tempRect);
		CRect	controlRect;
		showTipsButton.GetWindowRect(controlRect);
		tempRect.top = tempRect.bottom - borderWidth - controlRect.Height();
		tempRect.left += borderWidth;
		tempRect.right = tempRect.left + controlRect.Width();
		tempRect.bottom = tempRect.top + controlRect.Height();
		showTipsButton.MoveWindow(tempRect);
	}

	int	okWidth = 0;
	{
		CRect	tempRect;
		GetClientRect(tempRect);
		CRect	controlRect;
		okButton.GetWindowRect(controlRect);
		tempRect.top = tempRect.bottom - borderWidth - controlRect.Height();
		tempRect.left = tempRect.right - borderWidth - controlRect.Width();
		tempRect.right = tempRect.left + controlRect.Width();
		tempRect.bottom = tempRect.top + controlRect.Height();
		okButton.MoveWindow(tempRect);
		okWidth = controlRect.Width();
	}

	{
		CRect	tempRect;
		GetClientRect(tempRect);
		CRect	controlRect;
		nextTipButton.GetWindowRect(controlRect);
		tempRect.top = tempRect.bottom - borderWidth - controlRect.Height();
		tempRect.left = tempRect.right - borderWidth - controlRect.Width() - borderWidth - okWidth;
		tempRect.right = tempRect.left + controlRect.Width();
		tempRect.bottom = tempRect.top + controlRect.Height();
		nextTipButton.MoveWindow(tempRect);
	}

	LOGFONT	lf;
	memset(&lf, 0, sizeof(lf));
	lf.lfWidth = 0;
	lf.lfHeight = 14;
	lf.lfCharSet = ANSI_CHARSET;
	lf.lfOutPrecision = OUT_TT_ONLY_PRECIS;
	lf.lfQuality = ANTIALIASED_QUALITY;
	lf.lfPitchAndFamily = FF_SWISS|DEFAULT_PITCH;
	lf.lfWeight = FW_NORMAL;
	wcscpy(lf.lfFaceName, _T("Microsoft Sans Serif"));
	tipFont.CreateFontIndirect(&lf);

	// Pick a tip

	OnNextTip();

	// If there was an error, bail immediately

	if (!background.width() || !background.height()) PostMessage(WM_CLOSE);

	return TRUE;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	TipOfTheDayDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// Find our rectangle (hint - it's hard coded because it's a specific placement in the image :)

	wstring		iLine = tips[tipIndex];
	vector<wstring>	lines;
	unsigned int	tallestLine = 0;

	// Setup our DC for text rendering

	CFont *	oldFont = dc.SelectObject(&tipFont);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextAlign(TA_TOP);
	dc.SetTextColor(0);

	// Find the word breaks in the thing and insert carriage returns

	while(iLine.length())
	{
		// Find the actual line break

		wstring			line = iLine;
		wstring::size_type	idx = line.find(_T("\n"));
		if (idx != wstring::npos)
		{
			line.erase(idx);
			iLine.erase(0, idx+1);
		}
		else
		{
			iLine.erase();
		}

		// If it's a blank line, add it anyway...

		if (!line.length())
		{
			lines.push_back(line);
		}

		// Chop it up on word boundaries

		while(line.length())
		{
			// Count how many characters will fit?

			wstring::size_type	idx = 0;
			for (; idx < line.length(); idx++)
			{
				CSize	sz = dc.GetTextExtent(line.substr(0, idx+1).c_str(), idx+1);
				if ((int) sz.cx > textRect.Width()) break;
			}

			// Convert the count into an index of the last char on the line

			if (idx == line.length())
			{
				// We got the whole line, so back up to the index of the last char

				idx--;
			}

			// If we didn't break on an actual breaking char, search for a better one

			else if (line[idx] != _T(' ') && line[idx] != _T('-'))
			{
				wstring::size_type	temp = line.substr(0, idx).find_last_of(_T(" -"));

				// If we didn't find a better breaking point, then back up to the last char that did fit

				if (temp == wstring::npos)
				{
					idx--;
				}

				// We found a better breaking point; back up to it

				else
				{
					idx = temp;
				}
			}

			// Now chop the line where we decided to...

			wstring	newLine = line.substr(0, idx + 1);
			line.erase(0, idx+1);

			// Add the new line

			lines.push_back(newLine);

			// Track the tallest line

			CSize	sz = dc.GetTextExtent(newLine.c_str(), newLine.length());
			if (sz.cy > (int) tallestLine) tallestLine = sz.cy;
		}
	}

	// Setup our DIB

	winDIB	dib(dc);
	unsigned char	*fb = new unsigned char[background.width() * background.height() * sizeof(int)];
	memcpy(fb, background.data(), background.width() * background.height() * sizeof(int));
	dib.frameBuffer((unsigned char *) fb, NULL, background.width(), background.height(), 32);
	dib.srcRect() = CRect(0, 0, background.width(), background.height());
	dib.dstRect() = CRect(borderWidth, borderWidth, borderWidth+background.width(), borderWidth+background.height());
	dib.copyToDisplay();
	delete[] fb;

	// Print each line of the tip of the day

	int	x = textRect.left;
	int	y = textRect.top;

	for (vector<wstring>::const_iterator i = lines.begin(); i != lines.end(); ++i)
	{
		CRect	r;
		r.left = x;
		r.right = x + dib.width();
		r.top = y;
		r.bottom = y + dib.height();

		dc.DrawText(i->c_str(), r, DT_LEFT|DT_NOPREFIX);

		y += tallestLine + lineGap;

	}

	dc.SelectObject(oldFont);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	TipOfTheDayDlg::OnNextTip() 
{
	// Count the tips

	int	tipCount = 0;
	for(int i = 0; ;i++, tipCount++)
	{
		if (tips[i] == NULL) break;
	}

	// Get a tip

	tipIndex = theApp.prefs().other()[_T("currentTip")].integerValue();
	tipIndex++;
	if (tipIndex >= tipCount) tipIndex = 1;
	theApp.prefs().other()[_T("currentTip")].integerValue() = tipIndex;
	theApp.prefs().other().update();

	RedrawWindow();
}

// ---------------------------------------------------------------------------------------------------------------------------------
// TipOfTheDayDlg.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
