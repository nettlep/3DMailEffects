// ---------------------------------------------------------------------------------------------------------------------------------
//  _______ _                    _____  _                            
// |__   __| |                  |  __ \| |                           
//    | |  | |__  _ __  ___  ___| |  | | | __ _      ___ _ __  _ __  
//    | |  | '_ \| '__|/ _ \/ _ \ |  | | |/ _` |    / __| '_ \| '_ \ 
//    | |  | | | | |  |  __/  __/ |__| | | (_| | _ | (__| |_) | |_) |
//    |_|  |_| |_|_|   \___|\___|_____/|_|\__, |(_) \___| .__/| .__/ 
//                                         __/ |        | |   | |    
//                                        |___/         |_|   |_|    
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

#include "stdafx.h"
#include "3dme.h"
#include "ThreeDlg.h"
#include "Bitmap.h"
#include "3ds.h"
#include "ThreeThread.h"
#include "Render.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------

enum	{RENDER_TIMER = 12345};
enum	{CLICK_TIMER = RENDER_TIMER + 1};
enum	{WINDOW_WIDTH = 256};
enum	{WINDOW_HEIGHT = 256};

// ---------------------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(ThreeDlg, CDialog)
	//{{AFX_MSG_MAP(ThreeDlg)
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_CAPTURECHANGED()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_DESTROY()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ---------------------------------------------------------------------------------------------------------------------------------

	ThreeDlg::ThreeDlg(ThreeThread &owner, CWnd* pParent /*=NULL*/)
	: CDialog(ThreeDlg::IDD, pParent), render(NULL), hidden(false), _owner(owner)
{
	//{{AFX_DATA_INIT(ThreeDlg)
	//}}AFX_DATA_INIT

	masked = false;
	dragging = false;
	changeView = false;
	ignoreFocusChanges = false;
	hasFocus = false;
	sClickClose = false;
	sClickEmailClient = false;
	dClickClose = false;
	dClickEmailClient = false;
	mouseMoved = false;
	ignoreClick = false;
	stayOnTop = false;
	rotateAlways = false;

	// Non-modal

	Create(IDD);

	ShowWindow(SW_SHOW);
}

// ---------------------------------------------------------------------------------------------------------------------------------

	ThreeDlg::~ThreeDlg()
{
	master.DeleteObject();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ThreeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ThreeDlg)
	//}}AFX_DATA_MAP
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	ThreeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Get our window size

	currentScale = theApp.prefs().other()[_T("3DWindowScale")].integerValue();
	float	windowScale = (float) currentScale / 2.0f;
	int	windowWidth = (int) ((float) WINDOW_WIDTH * windowScale);
	int	windowHeight = (int) ((float) WINDOW_HEIGHT * windowScale);

	// Remember our window position

	sClickClose       = theApp.prefs().other()[_T("sClickClose")].booleanValue();
	sClickEmailClient = theApp.prefs().other()[_T("sClickEmailClient")].booleanValue();
	dClickClose       = theApp.prefs().other()[_T("dClickClose")].booleanValue();
	dClickEmailClient = theApp.prefs().other()[_T("dClickEmailClient")].booleanValue();
	softenEdges       = theApp.prefs().other()[_T("softenEdges3D")].booleanValue();
	stayOnTop         = theApp.prefs().other()[_T("stayOnTop3D")].booleanValue();
	rotateAlways      = theApp.prefs().other()[_T("rotateAlways")].booleanValue();
	int	windowX   = theApp.prefs().other()[_T("3DWindowY")].integerValue();
	int	windowY   = theApp.prefs().other()[_T("3DWindowX")].integerValue();

	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
	{
		windowX = 12345;
		windowY = 12345;
	}

	if (windowX != 12345 && windowY != 12345)
	{
		CRect	r;
		GetWindowRect(r);
		int	w = r.Width();
		int	h = r.Height();
		r.left = windowX;
		r.top = windowY;
		r.bottom = r.top + windowWidth;
		r.right = r.left + windowHeight;
		MoveWindow(r);
	}
	else
	{
		CRect	windowRect;
		GetWindowRect(windowRect);
		int	cx = windowRect.left + windowRect.Width() / 2;
		int	cy = windowRect.top + windowRect.Height() / 2;
		windowRect.left = cx - windowWidth / 2;
		windowRect.top = cy - windowHeight / 2;
		windowRect.right = windowRect.left + windowWidth;
		windowRect.bottom = windowRect.top + windowHeight;
		MoveWindow(windowRect);
	}

	render = new Render(*GetDC(), *this, polygonList, point3(0.0f, 0.0f, -65.0f));
	render->captureDesktop() = true;
	render->renderFrame(softenEdges, changeView);

	int	imx = theApp.prefs().other()[_T("3DExtraRotationX")].integerValue();
	int	imy = theApp.prefs().other()[_T("3DExtraRotationY")].integerValue();
	float	mx = *((float *)&imx);
	float	my = *((float *)&imy);
	render->extraRotation() = matrix4::rotation(my, mx, 0);

	if (!loadModel(theApp.modelName()))
	{
		PostMessage(WM_COMMAND, IDOK);
	}
	else
	{
		render->polygonList = polygonList;

		render->updateDefaultTexture(theApp.materialPath() + theApp.materialName());

		setStayOnTopState();

		SetTimer(RENDER_TIMER, 0, NULL);
	}

	return TRUE;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ThreeDlg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == RENDER_TIMER && render)
	{
		KillTimer(RENDER_TIMER);

		if (!masked) render->renderFrame(softenEdges, changeView);

		// Force focus changes, even if Windows is too stupid to send us a message properly

		bool	focus = GetForegroundWindow() == this;

		if (focus && !hasFocus)
		{
			OnSetFocus(NULL);
		}
		else if (!focus && hasFocus)
		{
			OnKillFocus(NULL);
		}

		if (!focus && !masked && !hidden && !rotateAlways)
		{
			setMask();
		}

		SetTimer(RENDER_TIMER, 0, NULL);
	}
	else if (nIDEvent == CLICK_TIMER)
	{
		KillTimer(CLICK_TIMER);

		if (sClickEmailClient)
		{
//			RunParms	rp;
//			rp.startEmail() = true;
//			rp.execute();
		}

		if (sClickClose)
		{
			PostMessage(WM_COMMAND, IDOK);
		}
	}

	CDialog::OnTimer(nIDEvent);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ThreeDlg::hideWindow()
{
	if (!hidden)
	{
		hidden = true;
		ignoreFocusChanges = true;
		ShowWindow(SW_HIDE);
		ignoreFocusChanges = false;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ThreeDlg::showWindow()
{
	if (hidden)
	{
		hidden = false;
		ignoreFocusChanges = true;
		ShowWindow(SW_SHOW);
		SetForegroundWindow();
		SetFocus();
		SetActiveWindow();
		ignoreFocusChanges = false;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ThreeDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (changeView)
	{
		changeView = false;
		ClipCursor(NULL);
	}
	else
	{
		KillTimer(CLICK_TIMER);
		ignoreClick = true;

		if (dClickEmailClient)
		{
//			RunParms	rp;
//			rp.startEmail() = true;
//			rp.execute();
		}

		if (dClickClose)
		{
			PostMessage(WM_COMMAND, IDOK);
		}

		if (!dClickClose && !dClickEmailClient)
		{
			mouseMoved = false;
			dragging = true;
			dragPos = point;
			SetCapture();
		}
	}

	CDialog::OnLButtonDblClk(nFlags, point);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ThreeDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (changeView)
	{
		changeView = false;
		ClipCursor(NULL);
	}
	else
	{
		mouseMoved = false;
		dragging = true;
		dragPos = point;
		SetCapture();
	}

	CDialog::OnLButtonDown(nFlags, point);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ThreeDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (changeView)
	{
		double	rangeX = -3.14159*2 / (double) render->width();
		double	rangeY = 3.14159*2 / (double) render->height();
		float	mx = (float) point.x * (float) rangeX;
		float	my = (float) point.y * (float) rangeY;
		render->extraRotation() = matrix4::rotation(my, mx, 0);
		theApp.prefs().other()[_T("3DExtraRotationX")].integerValue() = *((int *)&mx);
		theApp.prefs().other()[_T("3DExtraRotationY")].integerValue() = *((int *)&my);
		theApp.prefs().other().update();
	}
	else if (dragging)
	{
		CRect	wr;
		GetWindowRect(wr);
		CRect	twr = wr;
		int	w = wr.Width();
		int	h = wr.Height();

		POINT	p;
		GetCursorPos(&p);
		wr.left = p.x - dragPos.x;
		wr.top = p.y - dragPos.y;
		wr.right = wr.left + w;
		wr.bottom = wr.top + h;

		int	dx = twr.left - wr.left; if (dx < 0) dx = -dx;
		int	dy = twr.top  - wr.top; if (dy < 0) dy = -dy;
		if (mouseMoved || dx > 4 || dy > 4)
		{
			setMask();
			MoveWindow(wr);
			mouseMoved = true;
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ThreeDlg::OnCaptureChanged(CWnd *pWnd) 
{
	if (changeView)
	{
		changeView = false;
		ClipCursor(NULL);
	}

	if (dragging)
	{
		dragging = false;	
		if (mouseMoved) setMask(false);
	}
	CDialog::OnCaptureChanged(pWnd);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ThreeDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();

	if (!mouseMoved && !ignoreClick)
	{
		SetTimer(CLICK_TIMER, GetDoubleClickTime(), NULL);
	}

	ignoreClick = false;

	CDialog::OnLButtonUp(nFlags, point);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ThreeDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if (render && render->frameBuffer()) render->flip();
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	ThreeDlg::OnEraseBkgnd(CDC* pDC) 
{
	// DON'T erase the background!

	return 1;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ThreeDlg::setMask(const bool setFlag)
{
	if (masked == setFlag) return;

	if (render)
	{
		if (!setFlag)
		{
			masked = false;
			hideWindow();
			render->captureDesktop() = true;
			render->renderFrame(softenEdges, changeView);
			showWindow();

			CRect	wr;
			GetWindowRect(wr);
			master.DeleteObject();
			master.CreateRectRgn(0, 0, wr.Width(), wr.Height());
			SetWindowRgn((HRGN) master.GetSafeHandle(), TRUE);

			return;
		}

		if (render->frameBuffer())
		{
			// We're masked!

			masked = true;

			// Generate a frame buffer with lots of black

			render->renderFrame(softenEdges, changeView, false);

			master.DeleteObject();
			master.CreateRectRgn(0, 0, 0, 0);

			int	lastX0 = 0;
			int	lastX1 = 0;
			int	lastY0 = 0;

			unsigned int	*mask = render->frameBuffer();
			for (unsigned int y = 0; y < render->height(); y++)
			{
				for (unsigned int x = 0; x < render->width();)
				{
					unsigned int *ptr = mask + x;

					// Find the first non-black pixel

					for (unsigned int x0 = x; x0 < render->width() && !(*ptr); x0++, ptr++);

					// Find the last

					for (unsigned int x1 = x0; x1 < render->width() && *ptr; x1++, ptr++);

					// Update our position

					x = x1;

					// Have we reached the bottom/right corner?

					bool	lastChance = (y >= render->height() - 1 && x >= render->width() - 1);
					bool	differentRect = (lastX0 != x0 || lastX1 != x1);
					bool	foundOne = (x1 != x0);

					// Is this different from our last rect?

					if (differentRect || lastChance)
					{
						// Was there a previous rect?

						if (lastX0 || lastX1 || lastY0)
						{
							CRgn	r;

							// If it's the same scanline, make sure we have some height

							int	y1 = (lastY0 == y) ? y+1:y;

							// Create the region & combine it

							r.CreateRectRgn(lastX0, lastY0, lastX1, y1);
							master.CombineRgn(&master, &r, RGN_OR);
							r.DeleteObject();
						}

						if (foundOne)
						{
							lastX0 = x0;
							lastX1 = x1;
							lastY0 = y;
						}
						else
						{
							lastX0 = 0;
							lastX1 = 0;
							lastY0 = 0;
						}
					}
				}

				mask += render->width();
			}

			SetWindowRgn((HRGN) master.GetSafeHandle(), TRUE);
			render->flip();
		}
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ThreeDlg::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);

	hasFocus = false;

	if (ignoreFocusChanges) return;

	if (!hidden && !rotateAlways) setMask();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ThreeDlg::OnSetFocus(CWnd* pOldWnd) 
{
	CDialog::OnSetFocus(pOldWnd);

	hasFocus = true;

	if (ignoreFocusChanges)	return;

	if (masked) setMask(false);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void ThreeDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (render)
	{
		ReleaseDC(&render->dib().dc());
		delete render;
		render = NULL;
	}

	if (!hidden)
	{
		CRect	r;
		GetWindowRect(r);
		theApp.prefs().other()[_T("3DWindowX")].integerValue() = r.left;
		theApp.prefs().other()[_T("3DWindowY")].integerValue() = r.top;
		theApp.prefs().other().update();
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ThreeDlg::OnRButtonUp(UINT nFlags, CPoint point) 
{
	changeView = false;
	ClipCursor(NULL);

	// "Models" submenu
	CMenu	models;
	{
		models.CreatePopupMenu();

		WStringList	modelFiles;
		recursiveFindFiles(theApp.programPath() + _T("Models\\"), _T(".3ds"), modelFiles);
		modelFiles.sort();

		int	index = 0;
		for (WStringList::iterator i = modelFiles.begin(); i != modelFiles.end(); ++i)
		{
			// Strip out everything except the filename

			wstring	rawname = *i;

			bool	thisOne = false;
			if (rawname == (theApp.modelPath() + theApp.modelName())) thisOne = true;
			wstring::size_type	idx = rawname.find_last_of(_T("\\"));
			if (idx != wstring::npos) rawname.erase(0, idx + 1);

			idx = findNocase(rawname, _T(".3ds"), 0);
			if (idx != wstring::npos) rawname.erase(idx);

			models.AppendMenu(MF_STRING|(thisOne ? MF_CHECKED:MF_UNCHECKED), 2000+index, rawname.c_str());
			index++;
		}

		if (modelFiles.size()) models.AppendMenu(MF_SEPARATOR);
		models.AppendMenu(MF_STRING, MENU3D_LOAD_OBJECT, _T("Select..."));
	}

	// "Materials" submenu
	CMenu	materials;
	{
		materials.CreatePopupMenu();

		WStringList	materialFiles;
		recursiveFindFiles(theApp.programPath() + _T("Materials\\"), _T(".bmp"), materialFiles);
		materialFiles.sort();

		int	index = 0;
		for (WStringList::iterator i = materialFiles.begin(); i != materialFiles.end(); ++i)
		{
			// Strip out everything except the filename

			wstring	rawname = *i;

			bool	thisOne = false;
			if (rawname == (theApp.materialPath() + theApp.materialName())) thisOne = true;
			wstring::size_type	idx = rawname.find_last_of(_T("\\"));
			if (idx != wstring::npos) rawname.erase(0, idx + 1);

			idx = findNocase(rawname, _T(".bmp"), 0);
			if (idx != wstring::npos) rawname.erase(idx);

			materials.AppendMenu(MF_STRING|(thisOne ? MF_CHECKED:MF_UNCHECKED), 3000+index, rawname.c_str());
			index++;
		}

		if (materialFiles.size()) materials.AppendMenu(MF_SEPARATOR);
		materials.AppendMenu(MF_STRING, MENU3D_LOAD_MATERIAL, _T("Select..."));
	}

	CMenu menu;
	menu.CreatePopupMenu();

	wstring	polyString = integerToWString(render->polygonList.size()) + _T(" polygons");
	menu.AppendMenu(MF_STRING|MF_DISABLED|MF_GRAYED, 0, polyString.c_str());

	menu.AppendMenu(MF_SEPARATOR);

	menu.AppendMenu(MF_STRING|MF_POPUP, (unsigned int) models.GetSafeHmenu(), _T("Models"));
	menu.AppendMenu(MF_STRING|MF_POPUP, (unsigned int) materials.GetSafeHmenu(), _T("Materials"));

	menu.AppendMenu(MF_SEPARATOR);

	menu.AppendMenu(MF_STRING|(currentScale == 1 ? MF_CHECKED:MF_UNCHECKED), MENU3D_SIZE_HALF, _T("Half-size (0.5x)"));
	menu.AppendMenu(MF_STRING|(currentScale == 2 ? MF_CHECKED:MF_UNCHECKED), MENU3D_SIZE_1, _T("Normal-size (1x)"));
	menu.AppendMenu(MF_STRING|(currentScale == 4 ? MF_CHECKED:MF_UNCHECKED), MENU3D_SIZE_2, _T("Double-size (2x)"));
	menu.AppendMenu(MF_STRING|(softenEdges       ? MF_CHECKED:MF_UNCHECKED), MENU3D_SOFTEN_EDGES, _T("Soften edges of 3D object"));
	menu.AppendMenu(MF_STRING                                              , MENU3D_CHANGE_VIEW, _T("Change view"));

	menu.AppendMenu(MF_SEPARATOR);

	menu.AppendMenu(MF_STRING|(stayOnTop ? MF_CHECKED:MF_UNCHECKED), MENU3D_STAY_ON_TOP, _T("Stay on top"));
	menu.AppendMenu(MF_STRING|(rotateAlways ? MF_CHECKED:MF_UNCHECKED), MENU3D_ROTATE_ALWAYS, _T("Always rotate"));
	menu.AppendMenu(MF_STRING|(sClickClose ? MF_CHECKED:MF_UNCHECKED), MENU3D_SCLICK_CLOSE, _T("Close on click"));
	menu.AppendMenu(MF_STRING|(sClickEmailClient ? MF_CHECKED:MF_UNCHECKED), MENU3D_SCLICK_EMAIL_CLIENT, _T("Run email client on click"));
	menu.AppendMenu(MF_STRING|(dClickClose ? MF_CHECKED:MF_UNCHECKED), MENU3D_DCLICK_CLOSE, _T("Close on double-click"));
	menu.AppendMenu(MF_STRING|(dClickEmailClient ? MF_CHECKED:MF_UNCHECKED), MENU3D_DCLICK_EMAIL_CLIENT, _T("Run email client on double-click"));

	menu.AppendMenu(MF_SEPARATOR);

	menu.AppendMenu(MF_STRING, MENU3D_EMAIL_CLIENT, _T("Run your default email client"));
	menu.AppendMenu(MF_STRING, MENU3D_HELP, _T("Help"));
	menu.AppendMenu(MF_STRING, MENU3D_CLOSE, _T("Close"));

	POINT	loc;
	GetCursorPos(&loc);

	SetForegroundWindow();
	menu.TrackPopupMenu(TPM_CENTERALIGN, loc.x, loc.y, this, NULL);
	PostMessage(WM_NULL, 0, 0);

	menu.DestroyMenu();

	CDialog::OnRButtonUp(nFlags, point);
}

// ---------------------------------------------------------------------------------------------------------------------------------

BOOL	ThreeDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (!lParam)
	{
		bool	setScale = false;

		if (wParam == MENU3D_LOAD_OBJECT)
		{
			wstring	modelFile = askForModelFilename();

			if (modelFile.length()) loadModel(modelFile);
		}
		else if (wParam == MENU3D_LOAD_MATERIAL)
		{
			wchar_t		fname[MAX_PATH];
			memset(fname, 0, sizeof(fname));
			wchar_t		filters[] = _T("Bitmap files (*.bmp)\0*.bmp\0All files (*.*)\0*.*\0\0");
			OPENFILENAME	of;
			memset(&of, 0, sizeof(OPENFILENAME));

			of.lStructSize  = sizeof(OPENFILENAME);
			of.hwndOwner    = GetSafeHwnd();
			of.lpstrFilter  = filters;
			of.nFilterIndex = 1;
			of.lpstrFile    = fname;
			of.nMaxFile     = MAX_PATH - 1;
			of.lpstrTitle   = _T("Choose a material");
			of.Flags        = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_EXPLORER;
			of.lpstrDefExt  = _T("bmp");
			of.lpstrInitialDir = theApp.materialPath().c_str();

			if (GetOpenFileName(&of)) updateMaterial(of.lpstrFile);
		}
		else if (wParam == MENU3D_SIZE_HALF)
		{
			currentScale = 1;
			setScale = true;
		}
		else if (wParam == MENU3D_SIZE_1)
		{
			currentScale = 2;
			setScale = true;
		}
		else if (wParam == MENU3D_SIZE_2)
		{
			currentScale = 4;
			setScale = true;
		}
		else if (wParam == MENU3D_SOFTEN_EDGES)
		{
			softenEdges = !softenEdges;
			theApp.prefs().other()[_T("softenEdges3D")].booleanValue() = softenEdges;
			theApp.prefs().other().update();
		}
		else if (wParam == MENU3D_CHANGE_VIEW)
		{
			changeView = true;
			RECT	wr;
			GetWindowRect(&wr);
			ClipCursor(&wr);
		}
		else if (wParam == MENU3D_EMAIL_CLIENT)
		{
//			RunParms	rp;
//			rp.startEmail() = true;
//			rp.execute();
		}
		else if (wParam == MENU3D_ROTATE_ALWAYS)
		{
			rotateAlways = !rotateAlways;
			if (rotateAlways)
			{
				AfxMessageBox(
					_T("Note:\n")
					_T("\n")
					_T("Using this feature is not recommended, but put in by\n")
					_T("popular demand.\n")
					_T("\n")
					_T("By enabling this feature, you may experience incorrect\n")
					_T("graphics surrounding or behind the rotating object.\n")
					_T("\n")
					_T("If this happens, simply drag the 3D object.\n")
					_T("\n")
					_T("If you find this behavior too annoying, turn this feature\n")
					_T("off.\n")
				);
			}
			theApp.prefs().other()[_T("rotateAlways")].booleanValue() = rotateAlways;
			theApp.prefs().other().update();
		}
		else if (wParam == MENU3D_STAY_ON_TOP)
		{
			stayOnTop = !stayOnTop;
			theApp.prefs().other()[_T("stayOnTop3D")].booleanValue() = stayOnTop;
			theApp.prefs().other().update();
			setStayOnTopState();
		}
		else if (wParam == MENU3D_SCLICK_EMAIL_CLIENT)
		{
			sClickEmailClient = !sClickEmailClient;
			theApp.prefs().other()[_T("sClickEmailClient")].booleanValue() = sClickEmailClient;
			theApp.prefs().other().update();
		}
		else if (wParam == MENU3D_SCLICK_CLOSE)
		{
			sClickClose = !sClickClose;
			theApp.prefs().other()[_T("sClickClose")].booleanValue() = sClickClose;
			theApp.prefs().other().update();
		}
		else if (wParam == MENU3D_DCLICK_EMAIL_CLIENT)
		{
			dClickEmailClient = !dClickEmailClient;
			theApp.prefs().other()[_T("dClickEmailClient")].booleanValue() = dClickEmailClient;
			theApp.prefs().other().update();
		}
		else if (wParam == MENU3D_DCLICK_CLOSE)
		{
			dClickClose = !dClickClose;
			theApp.prefs().other()[_T("dClickClose")].booleanValue() = dClickClose;
			theApp.prefs().other().update();
		}
		else if (wParam == MENU3D_CLOSE)
		{
			PostMessage(WM_COMMAND, IDOK);
		}
		else if (wParam == MENU3D_HELP)
		{
			wstring	url = _T("file://") + theApp.programPath() + _T("Help\\3d.html");
			::ShellExecute(::GetDesktopWindow(), NULL, url.c_str(), NULL, NULL, SW_SHOWNORMAL);
		}
		else if (wParam >= 3000) // Material
		{
			WStringList	materialFiles;
			recursiveFindFiles(theApp.programPath() + _T("Materials\\"), _T(".bmp"), materialFiles);
			materialFiles.sort();

			unsigned int	index = wParam - 3000;

			if (index < materialFiles.size())
			{
				// Find the skin in the list

				int	ii = 0;
				wstring	materialFound;
				for (WStringList::iterator i = materialFiles.begin(); i != materialFiles.end(); ++i)
				{
					if (ii == index)
					{
						materialFound = *i;
						break;
					}
					ii++;
				}

				updateMaterial(materialFound);
			}
		}
		else if (wParam >= 2000) // Model
		{
			WStringList	modelFiles;
			recursiveFindFiles(theApp.programPath() + _T("Models\\"), _T(".3ds"), modelFiles);
			modelFiles.sort();

			unsigned int	index = wParam - 2000;

			if (index < modelFiles.size())
			{
				// Find the skin in the list

				int	ii = 0;
				wstring	modelFound;
				for (WStringList::iterator i = modelFiles.begin(); i != modelFiles.end(); ++i)
				{
					if (ii == index)
					{
						modelFound = *i;
						break;
					}
					ii++;
				}

				loadModel(modelFound);
			}
		}


		if (setScale)
		{
			CRect	r;
			GetWindowRect(r);

			int	cx = r.left + r.Width() / 2;
			int	cy = r.top + r.Height() / 2;

			float	scale = (float) currentScale / 2;
			int	w = (int)((float)WINDOW_WIDTH * scale);
			int	h = (int)((float)WINDOW_HEIGHT * scale);

			r.left = cx - w / 2;
			r.top = cy - h / 2;
			r.right = cx + w / 2;
			r.bottom = cy + h / 2;

			hideWindow();
			MoveWindow(r);
			render->updateWindowPosition();
			masked = true;
			setMask(false);
			showWindow();

			theApp.prefs().other()[_T("3DWindowScale")].integerValue() = currentScale;
			theApp.prefs().other().update();
		}
	}

	return CDialog::OnCommand(wParam, lParam);
}

// ---------------------------------------------------------------------------------------------------------------------------------

wstring	ThreeDlg::askForModelFilename()
{
	wchar_t		fname[MAX_PATH];
	memset(fname, 0, sizeof(fname));
	wchar_t		filters[] = _T("3D-Studio models (*.3ds)\0*.3ds\0All files (*.*)\0*.*\0\0");
	OPENFILENAME	of;
	memset(&of, 0, sizeof(OPENFILENAME));

	of.lStructSize  = sizeof(OPENFILENAME);
	of.hwndOwner    = GetSafeHwnd();
	of.lpstrFilter  = filters;
	of.nFilterIndex = 1;
	of.lpstrFile    = fname;
	of.nMaxFile     = MAX_PATH - 1;
	of.lpstrTitle   = _T("Choose a model");
	of.Flags        = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_EXPLORER;
	of.lpstrDefExt  = _T("3ds");
	of.lpstrInitialDir = theApp.modelPath().c_str();

	if (GetOpenFileName(&of))
	{
		return of.lpstrFile;
	}

	return _T("");
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	ThreeDlg::loadModel(const wstring &filename)
{
	C3DS	geomFile;
	sD3DS	info;
	wstring	modelFile = filename;

	for(;;)
	{
		memset(&info, 0, sizeof(info));
		if (!geomFile.load(modelFile.c_str(), &info))
		{
			wchar_t	dsp[512];
			_snwprintf(dsp, 512, _T("Cannot open the model file: %s"), modelFile.c_str());
			AfxMessageBox(dsp);

			modelFile = askForModelFilename();

			if (!modelFile.length())
			{
				SendMessage(WM_CLOSE);
				return false;
			}
		}
		else
		{
			break;
		}
	}

	// Share vertices
#if 0
	for (unsigned int i = 0; i < info.meshCount; i++)
	{
		sMSH	&mesh = info.mesh[i];
		for (unsigned int j = 0; j < mesh.vCount; j++)
		{
			sP3D	&v0 = mesh.vList[j];

			// Skip the ones we've already welded

			if (v0.x == -999) continue;

			for (unsigned int k = j+1; k < mesh.vCount; k++)
			{
				sP3D	&v1 = mesh.vList[k];

				float	dx = v1.x - v0.x;
				if (dx > 0.001 || dx < -0.001) continue;

				float	dy = v1.y - v0.y;
				if (dy > 0.001 || dy < -0.001) continue;

				float	dz = v1.z - v0.z;
				if (dz > 0.001 || dz < -0.001) continue;

				for (unsigned int l = 0; l < mesh.fCount; l++)
				{
					if (mesh.fList[l].a == k) mesh.fList[l].a = j;
					if (mesh.fList[l].b == k) mesh.fList[l].b = j;
					if (mesh.fList[l].c == k) mesh.fList[l].c = j;
				}

				// Flag that it's welded

				v1.x = -999;
			}
		}
	}
#endif
	// Convert the polygons

	polygonList.clear();

	for (unsigned int i = 0; i < info.meshCount; i++)
	{
		// Our current mesh

		sMSH	&mesh = info.mesh[i];

		// Get the material

		unsigned int *	texBuf = NULL;
		wstring	meshName(atow(mesh.name));

		// Is there a material listed with this one?

		wstring::size_type	idx = meshName.find(_T(":"));
		if (idx != wstring::npos)
		{
			meshName.erase(0, idx+1);
			meshName += _T(".bmp");
			texBuf = render->loadTexture(theApp.materialPath() +  meshName, true);
		}

		// Convert that mesh!

		vert<>	v0, v1, v2;
		v0.normal() = point4(0,0,0,1);
		v0.screen() = point4(0,0,0,1);
		v0.texture() = point2(0,0);
		v1.normal() = point4(0,0,0,1);
		v1.screen() = point4(0,0,0,1);
		v1.texture() = point2(0,0);
		v2.normal() = point4(0,0,0,1);
		v2.screen() = point4(0,0,0,1);
		v2.texture() = point2(0,0);

		int	startPolyIndex = polygonList.size();

		for (int j = 0; j < mesh.fCount; j++)
		{
			sFACE		&f = mesh.fList[j];
			if (f.a == f.b || f.b == f.c || f.c == f.a) continue;

			primitive<>	p;
			sP3D		&a = mesh.vList[f.a];
			sP3D		&b = mesh.vList[f.b];
			sP3D		&c = mesh.vList[f.c];

			v0.world().x() = a.x;	v0.world().y() = a.y;	v0.world().z() = a.z;
			v1.world().x() = b.x;	v1.world().y() = b.y;	v1.world().z() = b.z;
			v2.world().x() = c.x;	v2.world().y() = c.y;	v2.world().z() = c.z;

			// Skip degenerate polygons

			//if (v0.world() == v1.world() || v0.world() == v2.world() || v1.world() == v2.world()) continue;

			p += v2;
			p += v1;
			p += v0;
			p.calcPlane(false);
			p.texture() = texBuf;
			polygonList.push_back(p);
		}

		// Next, clear out the vectors so we can use them as normals

		memset(mesh.vList, 0, mesh.vCount * sizeof(sP3D));

		// Now, accumulate normals...

		int	polyIndex = startPolyIndex;
		for (int k = 0; k < mesh.fCount; k++)
		{
			sFACE		&f = mesh.fList[k];
			if (f.a == f.b || f.b == f.c || f.c == f.a) continue;

			primitive<>	&p = polygonList[polyIndex++];
			sP3D		&a = mesh.vList[f.a];
			sP3D		&b = mesh.vList[f.b];
			sP3D		&c = mesh.vList[f.c];
			const vector3	&n = p.plane().normal();

			// Skip degenerate polygons

			if (p.vertices()[0].world() == p.vertices()[1].world() || p.vertices()[0].world() == p.vertices()[2].world() || p.vertices()[1].world() == p.vertices()[2].world()) continue;

			a.x += n.x();	a.y += n.y();	a.z += n.z();
			b.x += n.x();	b.y += n.y();	b.z += n.z();
			c.x += n.x();	c.y += n.y();	c.z += n.z();
		}

		// Now normalize them

		polyIndex = startPolyIndex;
		for (int l = 0; l < mesh.fCount; l++)
		{
			sFACE		&f = mesh.fList[l];
			if (f.a == f.b || f.b == f.c || f.c == f.a) continue;

			primitive<>	&p = polygonList[polyIndex++];
			sP3D		&a = mesh.vList[f.a];
			sP3D		&b = mesh.vList[f.b];
			sP3D		&c = mesh.vList[f.c];

			// Skip degenerate polygons

			if (p.vertices()[0].world() == p.vertices()[1].world() || p.vertices()[0].world() == p.vertices()[2].world() || p.vertices()[1].world() == p.vertices()[2].world()) continue;

			vector3	tn0(a.x, a.y, a.z);	tn0.normalize();	p[2].normal() = tn0;
			vector3	tn1(b.x, b.y, b.z);	tn1.normalize();	p[1].normal() = tn1;
			vector3	tn2(c.x, c.y, c.z);	tn2.normalize();	p[0].normal() = tn2;

			// Fixup the normals...

			p[0].normal().w() = 0;
			p[1].normal().w() = 0;
			p[2].normal().w() = 0;
		}
	}

	render->polygonList = polygonList;

	// PDNDEBUG -- anybody that calls loadModel() needs to make sure it calls it with a model filename that resides INSIDE the Models folder
	theApp.modelName() = modelFile;

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ThreeDlg::OnCancel()
{
	DestroyWindow();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ThreeDlg::OnOK()
{
	DestroyWindow();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ThreeDlg::PostNcDestroy() 
{
	_owner.PostThreadMessage(THREETHREAD_TERMINATE, 0, 0);
	CDialog::PostNcDestroy();
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ThreeDlg::setStayOnTopState()
{
	if (stayOnTop)	SetWindowPos(&wndTopMost, 0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	else		SetWindowPos(&wndNoTopMost, 0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	ThreeDlg::updateMaterial(const wstring & materialFile)
{
	if (render->updateDefaultTexture(materialFile))
	{
		// PDNDEBUG -- anybody that calls updateMaterial() needs to make sure it calls it with a material filename that resides INSIDE the Materials folder
		theApp.materialName() = materialFile;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------
// ThreeDlg.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
