// ---------------------------------------------------------------------------------------------------------------------------------
//  ____  _____                 _____  _           _     
// |___ \|  __ \               |  __ \| |         | |    
//   __) | |  | |_ __ ___   ___| |  | | | __ _    | |__  
//  |__ <| |  | | '_ ` _ \ / _ \ |  | | |/ _` |   | '_ \ 
//  ___) | |__| | | | | | |  __/ |__| | | (_| | _ | | | |
// |____/|_____/|_| |_| |_|\___|_____/|_|\__, |(_)|_| |_|
//                                        __/ |          
//                                       |___/           
//
// 3D Mail Effects (dialog class)
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 11/16/2000 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2000, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_3DMEDLG
#define _H_3DMEDLG

// ---------------------------------------------------------------------------------------------------------------------------------
// Files required by this module
// ---------------------------------------------------------------------------------------------------------------------------------

#include "AppPrefs.h"
#include "EmailListbox.h"

// ---------------------------------------------------------------------------------------------------------------------------------
// Forward declarations
// ---------------------------------------------------------------------------------------------------------------------------------

class	MailThread;
class	ThreeDlg;
class	ThreeThread;

// ---------------------------------------------------------------------------------------------------------------------------------

class	C3DmeDlg : public CDialog
{
public:
				C3DmeDlg(CWnd* pParent = NULL);
				~C3DmeDlg();

inline	const	AppPrefs	&appPrefs() const {return _appPrefs;}
inline		AppPrefs	&appPrefs() {return _appPrefs;}

virtual		void		updateStats(const wstring &stats);
virtual		void		updateTexts();
virtual		void		updateLists();

virtual		void		hide3DWindow();
virtual		void		hideWindow();
virtual		void		showWindow();

virtual		bool		deleteMail(const wstring &id);
virtual		void		emailFromListID(const wstring &id, sAccount **aptr, sEmail **eptr);
virtual		void		previewMail(const wstring &id);
virtual		void		show3D();

		ThreeThread	*threeThread;
		bool		mailThreadTerminated;
		bool		threeThreadTerminated;
		WStringList	autodeleteSenders;
		WStringList	autodeleteSubjects;

	//{{AFX_DATA(C3DmeDlg)
	enum { IDD = IDD_EMAIL_DIALOG };
	CStatic	noSkinText;
	EmailListbox	emailListbox;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(C3DmeDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
		enum
		{
				MAIL_CHECK_TIMER = 54321,
				KEY_BLINK_TIMER,
				TRAY_BLINK_TIMER,
				AUTOUPDATE_TIMER,
				DIE_TIMER
		};

		HICON		m_hIcon, m_hTrayIcon, m_hBlankIcon;

		CRgn		windowRegion;

		int		savedScrollLockState, savedNumLockState, savedCapsLockState;
		int		lastScrollLockState,  lastNumLockState,  lastCapsLockState;

		bool		trayBlinkState;

		MailThread	*currentThread;

		bool		hidden;
		int		windowLeft, windowTop;

		Bitmap		background;
		Bitmap		backgroundMask;

		bool		dragging;
		POINT		dragPos;

		bool		mmInitialized;
		HMONITOR	(WINAPI* pfnMonitorFromWindow)(HWND, BOOL);
		BOOL		(WINAPI* pfnGetMonitorInfo)(HMONITOR, LPMONITORINFO);

		CRect		statusRect;
		CFont		statusFont;
		unsigned int	statusColor;
		wstring		statsString;

		bool		show3DFlag;

		bool		keyBlinkDisabledTemporarily;

		CToolTipCtrl	*toolTip;

		AppPrefs	_appPrefs;

		void		updateRegistryStartup();
		void		setFileAssociations();
		bool		wipeRegistry();
		void		TrayMessage(UINT message);
		LRESULT		WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		int		getScrollLockState();
		int		getNumLockState();
		int		getCapsLockState();
		void		saveScrollLockState();
		void		restoreScrollLockState();
		bool		objectInForeground();
		void		toggleScrollLockState();
		void		toggleNumLockState();
		void		toggleCapsLockState();
		void		editPreferences();
		void		setStayOnTopState();
		void		setShowOnTaskbarState();
		void		setKeyboardLedBlinkingState();
		void		setTrayBlinkingState();
		void		checkForUpdates(const bool quiet);
		void		showUpdatedMessage();

	//{{AFX_MSG(C3DmeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDropMenu();
	afx_msg void OnHelpButton();
	afx_msg void OnCheckNow();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnExit();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDblclkEmailList();
	afx_msg void OnAbout();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg BOOL OnQueryEndSession();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _H_3DMEDLG
// ---------------------------------------------------------------------------------------------------------------------------------
// 3DmeDlg.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
