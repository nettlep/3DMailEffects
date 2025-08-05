// ---------------------------------------------------------------------------------------------------------------------------------
//  ______                 _ _ _      _     _   _                    _     
// |  ____|               (_) | |    (_)   | | | |                  | |    
// | |__   _ __ ___   __ _ _| | |     _ ___| |_| |__   ___ __  __   | |__  
// |  __| | '_ ` _ \ / _` | | | |    | / __| __| '_ \ / _ \\ \/ /   | '_ \ 
// | |____| | | | | | (_| | | | |____| \__ \ |_| |_) | (_) |>  <  _ | | | |
// |______|_| |_| |_|\__,_|_|_|______|_|___/\__|_.__/ \___//_/\_\(_)|_| |_|
//                                                                         
//                                                                         
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

#ifndef	_H_EMAILLISTBOX
#define _H_EMAILLISTBOX

// ---------------------------------------------------------------------------------------------------------------------------------
// Files required by this module
// ---------------------------------------------------------------------------------------------------------------------------------

#include "Bitmap.h"

// ---------------------------------------------------------------------------------------------------------------------------------

class	EmailListbox : public CListBox
{
public:
				EmailListbox();
virtual				~EmailListbox();
virtual		wstring		getMessageID(int index) const;

		POINT		topLeft;
		Bitmap		background;
		unsigned int	itemCount;

	//{{AFX_VIRTUAL(EmailListbox)
	public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual int VKeyToItem(UINT nKey, UINT nIndex);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
		Bitmap		newIcon;
		Bitmap		oldIcon;
		Bitmap		newAttIcon;
		Bitmap		oldAttIcon;
		Bitmap		errIcon;
		CFont		font1;
		CFont		font2;
		CFont		font3;
		int		font1Height;
		int		font2Height;
		int		font3Height;
		unsigned int	dateTextColor;
		unsigned int	nameTextColor;
		unsigned int	sizeTextColor;
		unsigned int	fromTextColor;
		unsigned int	subjTextColor;
		unsigned int	readDateTextColor;
		unsigned int	readNameTextColor;
		unsigned int	readSizeTextColor;
		unsigned int	readFromTextColor;
		unsigned int	readSubjTextColor;
		unsigned int	selectedDateTextColor;
		unsigned int	selectedNameTextColor;
		unsigned int	selectedSizeTextColor;
		unsigned int	selectedFromTextColor;
		unsigned int	selectedSubjTextColor;
		unsigned int	selectionColor;

	//{{AFX_MSG(EmailListbox)
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // _H_EMAILLISTBOX
// ---------------------------------------------------------------------------------------------------------------------------------
// EmailListbox.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

