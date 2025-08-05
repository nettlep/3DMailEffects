// ---------------------------------------------------------------------------------------------------------------------------------
//  _______ _                    _____  _           _     
// |__   __| |                  |  __ \| |         | |    
//    | |  | |__  _ __  ___  ___| |  | | | __ _    | |__  
//    | |  | '_ \| '__|/ _ \/ _ \ |  | | |/ _` |   | '_ \ 
//    | |  | | | | |  |  __/  __/ |__| | | (_| | _ | | | |
//    |_|  |_| |_|_|   \___|\___|_____/|_|\__, |(_)|_| |_|
//                                         __/ |          
//                                        |___/           
//
// 3D animation dialog with region masking
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 01/04/2001 by Paul Nettle
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// Copyright 2001, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#ifndef	_H_THREEDLG
#define _H_THREEDLG

// ---------------------------------------------------------------------------------------------------------------------------------
// Required includes
// ---------------------------------------------------------------------------------------------------------------------------------

#include "Render.h"

// ---------------------------------------------------------------------------------------------------------------------------------
// Forward declarations
// ---------------------------------------------------------------------------------------------------------------------------------

class	ThreeThread;

// ---------------------------------------------------------------------------------------------------------------------------------

class	ThreeDlg : public CDialog
{
public:
				ThreeDlg(ThreeThread &owner, CWnd* pParent = NULL);
				~ThreeDlg();
		void		hideWindow();
		void		showWindow();

		bool		hasFocus;

inline		ThreeThread	&owner() {return _owner;}

	//{{AFX_DATA(ThreeDlg)
	enum { IDD = IDD_3D };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(ThreeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:

		Render		*render;
		bool		hidden;
		bool		masked;
		bool		dragging;
		bool		changeView;
		bool		ignoreFocusChanges;
		POINT		dragPos;
		int		currentScale;
		bool		sClickClose;
		bool		sClickEmailClient;
		bool		dClickClose;
		bool		dClickEmailClient;
		bool		mouseMoved;
		bool		softenEdges;
		bool		stayOnTop;
		bool		rotateAlways;
		ThreeThread	&_owner;
		vector<primitive<> > polygonList;
		CRgn		master;
		bool		ignoreClick;

		void		setMask(const bool setFlag = true);
		wstring		askForModelFilename();
		bool		loadModel(const wstring &filename);
		void		setStayOnTopState();
		void		updateMaterial(const wstring & materialFile);

	//{{AFX_MSG(ThreeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnDestroy();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

virtual	void	OnCancel();
virtual	void	OnOK();
};

//{{AFX_INSERT_LOCATION}}

#endif // _H_THREEDLG
// ---------------------------------------------------------------------------------------------------------------------------------
// ThreeDlg.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

