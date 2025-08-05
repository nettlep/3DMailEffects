// ---------------------------------------------------------------------------------------------------------------------------------
//  ______                 _ _ _      _     _   _                                     
// |  ____|               (_) | |    (_)   | | | |                                    
// | |__   _ __ ___   __ _ _| | |     _ ___| |_| |__   ___ __  __     ___ _ __  _ __  
// |  __| | '_ ` _ \ / _` | | | |    | / __| __| '_ \ / _ \\ \/ /    / __| '_ \| '_ \ 
// | |____| | | | | | (_| | | | |____| \__ \ |_| |_) | (_) |>  <  _ | (__| |_) | |_) |
// |______|_| |_| |_|\__,_|_|_|______|_|___/\__|_.__/ \___//_/\_\(_) \___| .__/| .__/ 
//                                                                       | |   | |    
//                                                                       |_|   |_|    
//
// 3D Mail Effects (Email listbox class)
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 11/20/2000 by Paul Nettle
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
#include "EmailListbox.h"
#include "3DmeDlg.h"
#include "WinDIB.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

#define	LINE_COUNT	3
#define	INDENT_PIXELS   5
#define	BORDER_PIXELS	1
#define	VERT_GAP_PIXELS	1
#define	ICON_GAP	5

// ---------------------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(EmailListbox, CListBox)
	//{{AFX_MSG_MAP(EmailListbox)
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	EmailListbox::EmailListbox()
		: CListBox()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

	EmailListbox::~EmailListbox()
{
	font1.DeleteObject();
	font2.DeleteObject();
	font3.DeleteObject();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EmailListbox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	CRect	client;
	GetClientRect(client);
	lpMeasureItemStruct->itemHeight = background.height();
	lpMeasureItemStruct->itemWidth  = client.Width();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EmailListbox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if ((int) lpDrawItemStruct->itemID < 0) return;

	if (!background.data()) return;

	// The text

	CString	str;
	GetText(lpDrawItemStruct->itemID, str);
	wstring	name = str;

	wstring	accountName;
	wstring::size_type idx = name.find(_T("\n"));
	if (idx == string::npos)
	{
		accountName = name;
		name.erase();
	}
	else
	{
		accountName = name.substr(0, idx);
		name.erase(0, idx+1);
	}

	wchar_t	type = accountName[0];
	accountName.erase(0, 1);

	bool	spam = accountName[0] == _T('y');
	accountName.erase(0, 1);

	wstring	sizeString;
	idx = name.find(_T("\n"));
	if (idx == string::npos)
	{
		sizeString = name;
		name.erase();
	}
	else
	{
		sizeString = name.substr(0, idx);
		name.erase(0, idx+1);
	}

	wchar_t	attachments = sizeString[0];
	sizeString.erase(0, 1);

	// Get the rect for the item's drawn area

	CRect	rect = lpDrawItemStruct->rcItem;

	// Our DC

	CDC *	dc = CDC::FromHandle(lpDrawItemStruct->hDC);
	dc->SetBkMode(TRANSPARENT);
	dc->SetTextAlign(TA_TOP);

	// Our dib

	winDIB	dib(*CDC::FromHandle(lpDrawItemStruct->hDC));
	unsigned int	*fb = new unsigned int[rect.Width() * rect.Height()];
	dib.frameBuffer((unsigned char *) fb, NULL, rect.Width(), rect.Height(), 32);
	dib.dstRect() = rect;
	rect.right -= rect.left;
	rect.bottom -= rect.top;
	rect.left = 0;
	rect.top = 0;
	dib.srcRect() = rect;

	int	totalSize = rect.Width() * rect.Height();

	// The background

	unsigned int	spamColor;
	unsigned int	dateColor;
	unsigned int	nameColor;
	unsigned int	sizeColor;
	unsigned int	fromColor;
	unsigned int	subjColor;
	if (GetCount())
	{
		if (background.data())
		{
			unsigned int	*oPtr = fb;
			const unsigned int	*iPtr = background.data();
			for (unsigned int y = 0; y < dib.height(); y++, iPtr += background.width(), oPtr += dib.width())
			{
				memcpy(oPtr, iPtr, dib.width() * sizeof(int));
			}
		}
		else
		{
			for (int i = 0; i < totalSize; i++)
				fb[i] = 0xffffffff;
		}

		if (lpDrawItemStruct->itemState & ODS_SELECTED)
		{
			for (int i = dib.width(); i < totalSize; i++) fb[i] = selectionColor;
		}

		if (lpDrawItemStruct->itemState & ODS_SELECTED)
		{
			spamColor = 0x800000;
			dateColor = selectedDateTextColor;
			nameColor = selectedNameTextColor;
			sizeColor = selectedSizeTextColor;
			fromColor = selectedFromTextColor;
			subjColor = selectedSubjTextColor;
		}
		else
		{
	 		if (type == 'o')
			{
				spamColor = 0xAB6B6B;
				dateColor = readDateTextColor;
				nameColor = readNameTextColor;
				sizeColor = readSizeTextColor;
				fromColor = readFromTextColor;
				subjColor = readSubjTextColor;
			}
			else
			{
				spamColor = 0x800000;
				dateColor = dateTextColor;
				nameColor = nameTextColor;
				sizeColor = sizeTextColor;
				fromColor = fromTextColor;
				subjColor = subjTextColor;
			}
		}

		// Find the account info and get the user's color selection

		C3DmeDlg *dialog = (C3DmeDlg *) GetParent();
		const AccountList &	accounts = dialog->appPrefs().accounts();
		for (AccountList::const_iterator i = accounts.begin(); i != accounts.end(); ++i)
		{
			if (i->accountName == accountName)
			{
				if (i->useColor) nameColor = ((i->displayColor>>16)&0xff)|((i->displayColor<<16)&0xff0000)|(i->displayColor&0xff00);
				break;
			}
		}
	}

	// The icon

	Bitmap	*icon;
	{
		if      (type == 'n')
		{
			if (attachments == 't')	icon = &newAttIcon;
			else			icon = &newIcon;
		}
		else if (type == 'o')
		{
			if (attachments == 't')	icon = &oldAttIcon;
			else			icon = &oldIcon;
		}
		else
		{
			icon = &errIcon;
		}

		if (icon->data())
		{
			for (unsigned int i = 0; i < icon->height(); i++)
			{
				memcpy(&fb[i * rect.Width()], &icon->data()[i*icon->height()], icon->width() * sizeof(int));
			}
		}
	}

	// Prepare for font rendering

	CFont *	oldFont = dc->SelectObject(&font1);

	// Display the text

	int	date_x, date_y, date_w, date_h, date_color; wstring date_text;
	int	subj_x, subj_y, subj_w, subj_h, subj_color; wstring subj_text;
	int	from_x, from_y, from_w, from_h, from_color; wstring from_text;
	int	acct_x, acct_y, acct_w, acct_h, acct_color; wstring acct_text;
	int	size_x, size_y, size_w, size_h, size_color; wstring size_text;

	int	lineIndex = 0;
	int	vert = 0;
	while(name.length() && lineIndex < 3)
	{
		wstring	line;
		wstring::size_type idx = name.find(_T("\n"));
		if (idx == string::npos)
		{
			line = name;
			name.erase();
		}
		else
		{
			line = name.substr(0, idx);
			name.erase(0, idx+1);
		}

		// Margins

		unsigned int	rightMargin = rect.Width() - BORDER_PIXELS;
		unsigned int	leftMargin = BORDER_PIXELS + icon->width() + ICON_GAP;

		// Right-hand column stuffs

		if (lineIndex == 0)
		{
			dc->SelectObject(&font1);
			CSize	sz = dc->GetOutputTextExtent(accountName.c_str(), accountName.length());
			rightMargin -= sz.cx + 5;
			acct_w = sz.cx, acct_h = rect.Height(), acct_x = rightMargin + 5, acct_y = vert, acct_text = accountName, acct_color = nameColor;
		}
		else if (lineIndex == 1 && type != 'e')
		{
			dc->SelectObject(&font2);
			CSize	sz = dc->GetOutputTextExtent(sizeString.c_str(), sizeString.length());
			rightMargin -= sz.cx + 5;
			leftMargin += INDENT_PIXELS;
			size_w = sz.cx, size_h = rect.Height(), size_x = rightMargin + 5, size_y = vert, size_text = sizeString, size_color = sizeColor;
		}
		else
		{
			leftMargin += INDENT_PIXELS;
		}

		// Trim the line to fit within the margin

		int	maxSize = rightMargin - leftMargin;

		// Remember what to draw in the HDC

		if (lineIndex == 0)	date_w = maxSize, date_h = rect.Height(), date_x = leftMargin, date_y = vert, date_text = line, date_color = dateColor;
		else if (lineIndex == 1)from_w = maxSize, from_h = rect.Height(), from_x = leftMargin, from_y = vert, from_text = line, from_color = fromColor;
		else			subj_w = maxSize, subj_h = rect.Height(), subj_x = leftMargin, subj_y = vert, subj_text = line, subj_color = subjColor;

		if (lineIndex == 0)	vert += font1Height;
		else if (lineIndex == 1)vert += font2Height;
		else			vert += font3Height;

		lineIndex++;
	}

	// Selection
	
	if (lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		for (int y = 1; y < (int) icon->height() - 1; y++)
		{
			for (int x = 0; x < (int) icon->width(); x++)
			{
				if ((y&1) != (x&1)) fb[y*dib.width()+x] = selectionColor;
			}
		}
	}

	// Mark spams in red

	if (spam)
	{
		date_color = spamColor;
		from_color = spamColor;
		subj_color = spamColor;
	}

	dib.copyToDisplay();
	delete[] fb;

	// Account name
	if (acct_text.length())
	{
		dc->SelectObject(&font1);
		dc->SetTextColor(((acct_color >> 16) & 0xff) + ((acct_color << 16) & 0xff0000) + (acct_color & 0xff00));
		CRect	r(acct_x, acct_y, acct_x + acct_w, acct_y + acct_h);
		r.OffsetRect(dib.dstRect().TopLeft());
		dc->DrawText(acct_text.c_str(), r, DT_LEFT|DT_NOPREFIX);
	}
	// Date
	if (date_text.length())
	{
		dc->SelectObject(&font1);
		dc->SetTextColor(((date_color >> 16) & 0xff) + ((date_color << 16) & 0xff0000) + (date_color & 0xff00));
		CRect	r(date_x, date_y, date_x + date_w, date_y + date_h);
		r.OffsetRect(dib.dstRect().TopLeft());
		dc->DrawText(date_text.c_str(), r, DT_LEFT|DT_END_ELLIPSIS|DT_NOPREFIX);
	}

	// Size
	if (size_text.length())
	{
		dc->SelectObject(&font2);
		dc->SetTextColor(((size_color >> 16) & 0xff) + ((size_color << 16) & 0xff0000) + (size_color & 0xff00));
		CRect	r(size_x, size_y, size_x + size_w, size_y + size_h);
		r.OffsetRect(dib.dstRect().TopLeft());
		dc->DrawText(size_text.c_str(), r, DT_LEFT|DT_NOPREFIX);
	}
	// From
	if (from_text.length())
	{
		dc->SelectObject(&font2);
		dc->SetTextColor(((from_color >> 16) & 0xff) + ((from_color << 16) & 0xff0000) + (from_color & 0xff00));
		CRect	r(from_x, from_y, from_x + from_w, from_y + from_h);
		r.OffsetRect(dib.dstRect().TopLeft());
		dc->DrawText(from_text.c_str(), r, DT_LEFT|DT_END_ELLIPSIS|DT_NOPREFIX);
	}

	// Subject
	if (subj_text.length())
	{
		dc->SelectObject(&font3);
		dc->SetTextColor(((subj_color >> 16) & 0xff) + ((subj_color << 16) & 0xff0000) + (subj_color & 0xff00));
		CRect	r(subj_x, subj_y, subj_x + subj_w, subj_y + subj_h);
		r.OffsetRect(dib.dstRect().TopLeft());
		dc->DrawText(subj_text.c_str(), r, DT_LEFT|DT_END_ELLIPSIS|DT_NOPREFIX);
	}

	// Restore stuff

	dc->SelectObject(oldFont);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EmailListbox::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// Force a selection if there is none

	BOOL	outside = TRUE;
	UINT	item = ItemFromPoint(point, outside);
	if (!outside)
	{
		if (GetSel(item) == 0)
		{
			SelItemRange(FALSE, 0, GetCount());
			SetSel(item);
		}
	}

	CListBox::OnRButtonDown(nFlags, point);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	EmailListbox::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// Get the email in question...

	int	emailIndex = point.y / background.height();

	if (emailIndex >= GetCount()) return;

	if (!GetSelCount())
	{
		AfxMessageBox(_T("You must first select at least one item in the list."));
	}
	else
	{
		CMenu	autoDeleteMenu;
		autoDeleteMenu.CreatePopupMenu();

		CMenu menu;
		menu.CreatePopupMenu();

		if (GetSelCount() > 1)
		{
			menu.AppendMenu(MF_STRING|MF_GRAYED, EMAILMENU_ALARM_RUNS, _T("Run alarm programs"));
			menu.AppendMenu(MF_STRING|MF_GRAYED, EMAILMENU_REMINDER_RUNS, _T("Run reminder programs"));
			menu.AppendMenu(MF_STRING|MF_GRAYED, EMAILMENU_EMAIL_CLIENT, _T("Run your default email client"));
			menu.AppendMenu(MF_STRING|MF_GRAYED, EMAILMENU_PREVIEW, _T("Preview this email"));
			menu.AppendMenu(MF_STRING, EMAILMENU_DELETE, _T("Delete these emails"));
		}
		else
		{
			autoDeleteMenu.AppendMenu(MF_STRING, EMAILMENU_AUTODELETE_SENDER, _T("containing this sender"));
			autoDeleteMenu.AppendMenu(MF_STRING, EMAILMENU_AUTODELETE_SUBJECT, _T("containing this subject"));
			menu.AppendMenu(MF_STRING|MF_POPUP, (unsigned int) autoDeleteMenu.GetSafeHmenu(), _T("Auto delete"));

			menu.AppendMenu(MF_STRING, EMAILMENU_ALARM_RUNS, _T("Run alarm programs"));
			menu.AppendMenu(MF_STRING, EMAILMENU_REMINDER_RUNS, _T("Run reminder programs"));
			menu.AppendMenu(MF_STRING, EMAILMENU_EMAIL_CLIENT, _T("Run your default email client"));
			menu.AppendMenu(MF_STRING, EMAILMENU_PREVIEW, _T("Preview this email"));
			menu.AppendMenu(MF_STRING, EMAILMENU_DELETE, _T("Delete this email"));
		}
		POINT	loc;
		GetCursorPos(&loc);
		menu.TrackPopupMenu(TPM_CENTERALIGN, loc.x, loc.y, this, NULL);
		menu.DestroyMenu();
		autoDeleteMenu.DestroyMenu();
	}

	CListBox::OnRButtonUp(nFlags, point);
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	EmailListbox::getMessageID(int index) const
{
	CString	str;
	GetText(index, str);

	// Get the message ID

	wstring	entryID = str;
	wstring::size_type	idx = entryID.find_last_of(_T("\n"));
	if (idx == wstring::npos) return _T("");

	entryID.erase(0, idx + 1);
	return entryID;
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	EmailListbox::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	int	selCount = GetSelCount();
	if (!lParam && selCount)
	{
		int	*selectionList = new int[selCount];
		GetSelItems(selCount, selectionList);

		// Get their IDs

		vector<wstring>	idList;
		for (int i = 0; i < selCount; i++)
		{
			idList.push_back(getMessageID(selectionList[i]));
		}

		delete[] selectionList;

		C3DmeDlg *dialog = (C3DmeDlg *) GetParent();

		if (wParam == EMAILMENU_DELETE)
		{
			wstring	msg = _T("Are you sure you wish to delete this email?\n\nThis will remove the email from your server permanently.\n\nThere is no way to undo this action.");
			if (idList.size() > 1) msg = _T("Are you sure you wish to delete these ") + integerToWString(idList.size()) + _T(" emails?\n\nThis will remove the selected emails from your server permanently.\n\nThere is no way to undo this action.");

			if (theApp.prefs().email()[_T("confirmDeletes")].booleanValue() == false || MessageBox(msg.c_str(), _T("Just makin' sure!"), MB_YESNO) == IDYES)
			{
				C3DmeDlg *dialog = (C3DmeDlg *) GetParent();
				for (unsigned int i = 0; i < idList.size(); i++)
				{
					dialog->deleteMail(idList[i]);
				}
			}
		}
		else if (wParam == EMAILMENU_PREVIEW)
		{
			if (idList.size())
			{
				dialog->previewMail(idList[0]);
			}
		}
		else if (wParam == EMAILMENU_ALARM_RUNS)
		{
			if (idList.size())
			{
				sAccount	*aptr;
				sEmail		*eptr;
				dialog->emailFromListID(idList[0], &aptr, &eptr);

//				if (aptr) aptr->alarmRunParms.execute();
			}
		}
		else if (wParam == EMAILMENU_REMINDER_RUNS)
		{
			if (idList.size())
			{
				sAccount	*aptr;
				sEmail		*eptr;
				dialog->emailFromListID(idList[0], &aptr, &eptr);

//				if (aptr) aptr->reminderRunParms.execute();
			}
		}
		else if (wParam == EMAILMENU_EMAIL_CLIENT)
		{
//			RunParms	rp;
//			rp.startEmail() = true;
//			rp.execute();
		}
		else if (wParam == EMAILMENU_AUTODELETE_SENDER)
		{
			C3DmeDlg *dialog = (C3DmeDlg *) GetParent();
			for (unsigned int i = 0; i < idList.size(); i++)
			{
				sAccount	*aptr;
				sEmail		*eptr;
				dialog->emailFromListID(idList[i], &aptr, &eptr);
				dialog->autodeleteSenders.push_back(eptr->mimeHeader.from());
			}
		}
		else if (wParam == EMAILMENU_AUTODELETE_SUBJECT)
		{
			C3DmeDlg *dialog = (C3DmeDlg *) GetParent();
			for (unsigned int i = 0; i < idList.size(); i++)
			{
				sAccount	*aptr;
				sEmail		*eptr;
				dialog->emailFromListID(idList[i], &aptr, &eptr);
				dialog->autodeleteSubjects.push_back(eptr->mimeHeader.subject());
			}
		}
	}

	return CListBox::OnCommand(wParam, lParam);
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	EmailListbox::OnEraseBkgnd(CDC* pDC) 
{
	if (!background.data())
	{
		return CListBox::OnEraseBkgnd(pDC);
	}

	// Our dib

	winDIB	dib(*pDC);

	CRect	clientRect;
	GetClientRect(clientRect);

	dib.frameBuffer((unsigned char *) background.data(), NULL, background.width(), background.height(), 32);
	dib.srcRect() = CRect(0, 0, background.width(), background.height());

	CRect	outRect = clientRect;
	outRect.bottom = outRect.top + background.height();

	for (int i = 0; i < clientRect.bottom; i+= background.height())
	{
		dib.dstRect() = outRect;
		outRect.top += background.height();
		outRect.bottom += background.height();
		if (outRect.bottom > clientRect.bottom) outRect.bottom = clientRect.bottom;
		dib.copyToDisplay();
	}

	if (!GetCount()) ShowWindow(SW_HIDE);
	return TRUE;
}

// ---------------------------------------------------------------------------------------------------------------------------------

int	EmailListbox::VKeyToItem(UINT nKey, UINT nIndex) 
{
	if (nKey == VK_DELETE)
	{
		SendMessage(WM_COMMAND, EMAILMENU_DELETE);
		return -2;
	}
	else if (nKey == VK_SPACE)
	{
		SendMessage(WM_COMMAND, EMAILMENU_PREVIEW);
		return -2;
	}

	// return -1 = default action
	// return -2 = no further action
	// return index = perform default action for keystroke on item specified by index

	return -1;
}

// ---------------------------------------------------------------------------------------------------------------------------------
// EmailListbox.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
