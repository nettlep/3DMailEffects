// ---------------------------------------------------------------------------------------------------------------------------------
//  _____                 _                                
// |  __ \               | |                               
// | |__) | ___ _ __   __| | ___ _ __      ___ _ __  _ __  
// |  _  / / _ \ '_ \ / _` |/ _ \ '__|    / __| '_ \| '_ \ 
// | | \ \|  __/ | | | (_| |  __/ |    _ | (__| |_) | |_) |
// |_|  \_\\___|_| |_|\__,_|\___|_|   (_) \___| .__/| .__/ 
//                                            | |   | |    
//                                            |_|   |_|    
//
// Rendering pipeline and vewport manager
//
// ---------------------------------------------------------------------------------------------------------------------------------
//
// Originally released under a custom license.
// This historical re-release is provided under the MIT License.
// See the LICENSE file in the repo root for details.
//
// https://github.com/nettlep
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 12/06/2000 by Paul Nettle
//
// Copyright 2000, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "3Dme.h"
#include "Render.h"
#include "ThreeDlg.h"
#include "Clip.h"
#include "TMap.h"
#include "Bitmap.h"

// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ---------------------------------------------------------------------------------------------------------------------------------
// Rendering constants
// ---------------------------------------------------------------------------------------------------------------------------------

const	float	Render::FOV = 40.0f;
const	float	Render::nearZ = 1.0f;
const	float	Render::farZ =  150.0f;

// --------------------------------------------------------------------------------------------------------------------------------

static	inline		unsigned int	alphaBlend(const unsigned int dst, const unsigned int src)
{
	// src * alpha + dst * alpha, where alpha is src's alpha, resulting alpha is dst alpha
	unsigned int	a = src >> 24;
	unsigned int	aa = 0xff - a;
	unsigned int	rb = (((src & 0x00ff00ff) * a) + ((dst & 0x00ff00ff) * aa)) & 0xff00ff00;
	unsigned int	g  = (((src & 0x0000ff00) * a) + ((dst & 0x0000ff00) * aa)) & 0x00ff0000;
	return (src & 0xff000000) | ((rb | g) >> 8);
}

// ---------------------------------------------------------------------------------------------------------------------------------
// Hidden feature... save a BMP file of the rendered image....
// ---------------------------------------------------------------------------------------------------------------------------------

static	string	writeBMP(unsigned int w, unsigned int h, unsigned int *buf)
{
	FILE		*fp = NULL;
	string	retString;

	try
	{
		static	int	counter = -1;
		counter++;
		char	tmp[20];
		sprintf(tmp, "Temp\\3DME_%05d.bmp", counter);
		
		// Open the file

		fp = fopen(tmp, "wb");
		if (!fp) throw "Unable to open the BMP file for writing";

		// Create the file header

		#pragma pack(1)
		struct BMPHeader
		{
			unsigned short	bfType; 
			unsigned int	bfSize; 
			unsigned short	bfReserved1; 
			unsigned short	bfReserved2; 
			unsigned int	bfOffBits; 
		}; 
		struct	BMPInfoHeader
		{
			unsigned int	biSize; 
			unsigned int	biWidth; 
			unsigned int	biHeight; 
			unsigned short	biPlanes; 
			unsigned short	biBitCount;
			unsigned int	biCompression; 
			unsigned int	biSizeImage; 
			unsigned int	biXPelsPerMeter; 
			unsigned int	biYPelsPerMeter; 
			unsigned int	biClrUsed; 
			unsigned int	biClrImportant; 
		};
		#pragma pack()

		BMPHeader	bfh;
		memset(&bfh, 0, sizeof(bfh));
		bfh.bfType = *(unsigned short *) "BM";
		bfh.bfSize = sizeof(bfh);
		bfh.bfOffBits = sizeof(BMPHeader) + sizeof(BMPInfoHeader);
		if (fwrite(&bfh, sizeof(bfh), 1, fp) != 1) throw "Unable to write the BMP file header";

		// Create the info header

		BMPInfoHeader	bih;
		memset(&bih, 0, sizeof(bih));
		bih.biSize = sizeof(bih);
		bih.biPlanes = 1;
		bih.biBitCount = 24;
		bih.biCompression = 0;
		bih.biWidth = w;
		bih.biHeight = h;
		if (fwrite(&bih, sizeof(bih), 1, fp) != 1) throw "Unable to read the BMP info header";

		// Padding

		int	paddedWidth = 4 - ((w*3) & 3);
		if (paddedWidth == 4) paddedWidth = 0;

		// Write the image data

		unsigned char	*src = (unsigned char *) buf + (h - 1) * w * 4;

		for (unsigned int y = 0; y < h; ++y)
		{
			for (unsigned int x = 0; x < w; ++x)
			{
				if (fwrite(&src[x*4+0], 1, 1, fp) != 1) throw "Unable to write BMP image data";
				if (fwrite(&src[x*4+1], 1, 1, fp) != 1) throw "Unable to write BMP image data";
				if (fwrite(&src[x*4+2], 1, 1, fp) != 1) throw "Unable to write BMP image data";
			}

			// Padding

			unsigned int tmp = 0;
			if (paddedWidth) if (fwrite(&tmp, paddedWidth, 1, fp) != 1) throw "Unable to write BMP image data";

			src -= w * 4;
		}
	}
	catch (const char * err)
	{
		retString = err;
	}

	if (fp) fclose(fp);

	return retString;
}

// ---------------------------------------------------------------------------------------------------------------------------------

		Render::Render(CDC &dc, ThreeDlg &window, const vector<primitive<> > &data, const point3 &cameraPosition)
		:_window(window), _dib(dc), polygonList(data), _cameraPosition(cameraPosition), _captureDesktop(true), _desktop(NULL), _defaultTextureBuffer(NULL)
{
	// Setup the window stuff

	_width = 0;
	_height = 0;
	_frameBuffer = NULL;
	_aaBuffer = NULL;
	_zBuffer = NULL;
	_xrot = 0;
	_yrot = 0;
	extraRotation() = matrix4::identity();

	// Allocate the default texture buffer

	_defaultTextureBuffer = new unsigned int[textureWidth * textureHeight];

	// Start the high resolution timer

	theApp.timer().startElapsedTickTimer();

	// Setup the window's DIB

	updateWindowPosition();
}

// ---------------------------------------------------------------------------------------------------------------------------------

		Render::~Render()
{
	delete[] _frameBuffer;
	delete[] _aaBuffer;
	delete[] _zBuffer;
	delete[] _desktop;
	delete[] _defaultTextureBuffer;

	// Wipe out our managed textures...

	for (vector<unsigned int *>::const_iterator i = _textureArray.begin(); i != _textureArray.end(); ++i)
	{
		delete[] *i;
	}
}

// ---------------------------------------------------------------------------------------------------------------------------------

void		Render::clear(const bool background)
{
	if (background)
	{
		if (captureDesktop())
		{
			captureDesktop() = false;

			// Our window rect

			CRect	wr;
			window().GetWindowRect(wr);

			// Wait until everybody repaints themselves

			Sleep(50);

			// Our screen DC

			CDC	srcDC;
			srcDC.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);

			CDC	memDC;
			memDC.CreateCompatibleDC(&srcDC);

			CBitmap	compatBitmap;
			compatBitmap.CreateCompatibleBitmap(&srcDC, width(), height());

			CBitmap *	oldBitmap = memDC.SelectObject(&compatBitmap);
			memDC.BitBlt(0, 0, width(), height(), &srcDC, wr.left, wr.top, SRCCOPY);
			memDC.SelectObject(oldBitmap);

			BITMAPINFO	bmi;
			bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmi.bmiHeader.biWidth = dib().width();
			bmi.bmiHeader.biHeight = (LONG) (-(int)dib().height());
			bmi.bmiHeader.biPlanes = 1;
			bmi.bmiHeader.biBitCount = dib().depth();
			bmi.bmiHeader.biCompression = BI_RGB;

			delete[] _desktop;
			_desktop = new unsigned int[width() * height()];

			GetDIBits(srcDC.GetSafeHdc(), (HBITMAP) compatBitmap.GetSafeHandle(), 0, height(), desktop(), &bmi, DIB_RGB_COLORS);

			memDC.DeleteDC();
			srcDC.DeleteDC();
			compatBitmap.DeleteObject();
		}

		memcpy(dib().frameBuffer(), desktop(), width() * height() * sizeof(unsigned int));
	}
	else
	{
		memset(frameBuffer(), 0, width() * height() * sizeof(unsigned int));
	}

	memset(_zBuffer, 0, width() * height() *sizeof(float));
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool		Render::updateWindowPosition()
{
	// The new DIB dimensions

	{
		CRect	clientRect;
		window().GetClientRect(clientRect);
		unsigned int	w = clientRect.Width();
		unsigned int	h = clientRect.Height();

		// Bail if we don't have any dimension

		if (!w || !h) return true;

		// Bail if there's no change

		if (width() == w && height() == h && frameBuffer()) return true;

		// Update

		width() = w;
		height() = h;
		screenCenter() = point2((float) width() / 2, (float) height() / 2);
	}

	// Reallocate our frame buffer

	delete[] _frameBuffer;
	_frameBuffer = new unsigned int[width()*height()];
	if (!_frameBuffer) return false;

	// Reallocate our AA frame buffer

	delete[] _aaBuffer;
	_aaBuffer = new unsigned int[width()*height()];
	if (!_aaBuffer) return false;

	// Reallocate our Zbuffer

	delete[] _zBuffer;
	_zBuffer = new float[width()*height()];
	if (!_zBuffer) return false;

	// Setup the dib

	dib().dstRect() = CRect(0, 0, width(), height());
	dib().srcRect() = CRect(0, 0, width(), height());
	dib().frameBuffer((unsigned char *) frameBuffer(), zBuffer(), width(), height());
	dib().depth(32);

	// Done

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	Render::animate()
{
	// Get the speed

	double	speed = 2000.0;
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000) speed = 250.0;
	else if (GetAsyncKeyState(VK_CONTROL) & 0x8000) speed = 20000.0;

	static	double	shitForBrains;
	shitForBrains += theApp.timer().readElapsedTickTimer() / theApp.timer().ticksPerMillisecond() / speed;
	theApp.timer().startElapsedTickTimer();

	// Apply rotation

	xrot() = -0.3f;
	yrot() = shitForBrains;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	Render::renderFrame(const bool softenEdges, const bool changeView, const bool background)
{
	// Clear the frame buffer

	clear(background);

	// "The shape of things to come" (sounds like an advert) -- This is thhe shape of the screen

	const	float	xScale = 0.98f;
	const	float	yScale = 0.98f;

	// Animate

	//if (!changeView) animate();
	animate();

	// Generate a view matrix (in this simple engine, this is just the rotation of the camera

	matrix4	rotation = matrix4::rotation((float) xrot(), (float) yrot(), 0.0f) >> extraRotation();

	// Get the camera in object space (for culling)

	matrix4	iRot = rotation;
	iRot.invert();
	point4	objCam = iRot >> cameraPosition();

	// Add in the transformation to the view matrix

	matrix4	view = rotation >> matrix4::translation(-vector4(cameraPosition().x(), cameraPosition().y(), cameraPosition().z(), 0));

	// Our aspect ratio, and our [F]ield [O]f [V]iew

	float	aspect = ((float) width() * xScale) / ((float) height() * yScale);
	float	fov    = FOV * (float) 3.14159265359 / (float) 180;

	// Generate a perspective projection transformation matrix

	matrix4	perspectiveXform = matrix4::projectPerspectiveBlinn(fov, aspect, nearZ, farZ);

	// And combine the two for our final transform matrix

	matrix4	xform = view >> perspectiveXform;

	// background into the aaBuffer

	if (softenEdges && background)
	{
		memcpy(aaBuffer(), frameBuffer(), sizeof(unsigned int) * width() * height());
	}

	for (unsigned int i = 0; i < polygonList.size(); i++)
	{
		primitive<>	p = polygonList[i];

		// Backface culling

		if (p.plane().distance(objCam) < 0) continue;

		// Transform & code the vertices

		unsigned int	codeOff = -1;
		unsigned int	codeOn = 0;
		for (unsigned int j = 0; j < p.vertexCount(); j++)
		{
			vert<>	&v = p[j];
			point4	&s = v.screen();
			s = xform >> v.world();

			// Generate texture coordinates from rotated normal

			point4	n = xform >> v.normal();
			v.texture().x() = n.x() * 127 + 128;
			v.texture().y() = n.y() * 127 + 128;

			unsigned int	code =	(s.x() >  s.w() ?  1:0) | (s.x() < -s.w() ?  2:0) |
						(s.y() >  s.w() ?  4:0) | (s.y() < -s.w() ?  8:0) |
						(s.z() <    0.0 ? 16:0) | (s.z() >  s.w() ? 32:0);
			codeOff &= code;
			codeOn  |= code;
		}

		// Completely coded off-screen?

		if (codeOff) continue;

		// Only bother trying to clip if it's partially off-screen

		Clipper	clipper;
		unsigned int *	tex = p.texture();
		if (codeOn && !clipper.clipPrimitive(p)) continue;

		// Project

		sVERT	verts[64];
		for (j = 0; j < p.vertexCount(); j++)
		{
			vert<>	&v = p[j];
			float	ow = 1.0f / v.screen().w();
			verts[j].x = screenCenter().x() + v.screen().x() * ow * screenCenter().x() * xScale;
			verts[j].y = screenCenter().y() - v.screen().y() * ow * screenCenter().y() * yScale;
			verts[j].z = ow;
			verts[j].w = ow;
			verts[j].u = v.texture().x();
			verts[j].v = v.texture().y();
			verts[j].next = &verts[j+1];
		}
		verts[j-1].next = NULL;

		// Draw it

		if (!tex) tex = defaultTextureBuffer();
		if (softenEdges && background)
		{
			drawAffineTexturedPolygonAA(verts, aaBuffer(), tex, zBuffer(), width());
		}
		else
		{
			drawAffineTexturedPolygon(verts, frameBuffer(), tex, zBuffer(), width());
		}
	}

	if (softenEdges && background)
	{
		for (unsigned int y = 0; y < height(); ++y)
		{
			int	yIndex = y * width();
			for (unsigned int x = 0; x < width(); ++x)
			{
				unsigned int	src = aaBuffer()[yIndex + x];
				unsigned int *	dst = &frameBuffer()[yIndex + x];

				*dst = alphaBlend(*dst, src);
			}
		}
	}

	if (changeView && background)
	{
		#define invert(xx, yy)\
		{\
			int	index = (yy) * width() + (xx);\
			int	c = frameBuffer()[index];\
			int	r = ((c >> 16) + 96) & 0xff;\
			int	g = ((c >>  8) + 96) & 0xff;\
			int	b = ((c >>  0) + 96) & 0xff;\
			frameBuffer()[index] = (b << 16) | (r << 8) | g;\
		}

		// Horiz lines

		for (unsigned int x = 0; x < width(); ++x)
		{
			if (x & 8)
			{
				invert(x, 0);
				invert(x, 1);
				invert(x, 2);
				invert(x, 3);
				invert(x, height()-1);
				invert(x, height()-2);
				invert(x, height()-3);
				invert(x, height()-4);
			}
		}

		for (unsigned int y = 0; y < height(); ++y)
		{
			if (y & 8)
			{
				invert(0, y);
				invert(1, y);
				invert(2, y);
				invert(3, y);
				invert(width()-1, y);
				invert(width()-2, y);
				invert(width()-3, y);
				invert(width()-4, y);
			}
		}
	}

	// Update the screen

	if (background) flip();

	if (theApp.prefs().other()[_T("animationScreenshots")].booleanValue()) writeBMP(width(), height(), frameBuffer());

	// Done

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

bool	Render::updateDefaultTexture(const wstring &name)
{
	unsigned int	*buf = loadTexture(name, false);

	if (!buf)
	{
		// Frequency: the lower the number, the higher the frequency

		const int	freq = 2;
		const int	fAnd = 1 << freq;

		for (unsigned int y = 0; y < textureHeight; y++)
		{
			int	yIndex = y * textureWidth;

			for (unsigned int x = 0; x < textureWidth; x++)
			{
				int	c = (int) sqrt(static_cast<double>(y*y + x*x));
				if (c < 0) c = 0;
				if (c > 255) c = 255;
//				defaultTextureBuffer()[yIndex + x] = 0xff000000 | (c << 16) | (c << 8) | c;
				defaultTextureBuffer()[yIndex + x] = (c << 16) | (c << 8) | c;
			}
		}

		return false;
	}
	else
	{
		memcpy(defaultTextureBuffer(), buf, textureWidth * textureHeight * sizeof(int));
	}

	return true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

unsigned int *	Render::loadTexture(const wstring &name, const bool silent, const bool store)
{
	// Have we already loaded this sucker?

	map<wstring, unsigned int *>::const_iterator pos = _textureMapMap.find(name);
	if (pos != _textureMapMap.end()) return pos->second;

	// Try to load the bitmap

	Bitmap	bm;
	if (!bm.read(name, silent)) return NULL;

	// Valid bitmap?

	if (bm.width() != textureWidth || bm.height() != textureHeight)
	{
		wchar_t	dsp[256];
		_snwprintf(dsp, 256, _T("The texture (%s) has the wrong dimensions (%dx%d)\nPlease select a bitmap that is 256x256"), name.c_str(), bm.width(), bm.height());
		AfxMessageBox(dsp);
		return NULL;
	}

	// We got the texture, we want to return the buffer, but prevent the bitmap from releasing the memory

	unsigned int	*ret = bm.data();
	bm.data() = NULL;

	// Are we managing this texture?

	if (store)
	{
		_textureArray.push_back(ret);
		_textureMapMap[name] = ret;
	}

	// Set the high-bytes

//	for (int i = 0; i < textureWidth * textureHeight; i++)
//	{
//		ret[i] |= 0xff000000;
//	}

	return ret;
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	Render::mouseMoved(const int relX, const int relY)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

void	Render::mousePressed(const bool left, const bool right)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------
// Render.cpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------
