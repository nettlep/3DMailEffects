// ---------------------------------------------------------------------------------------------------------------------------------
//  _____                 _               _     
// |  __ \               | |             | |    
// | |__) | ___ _ __   __| | ___ _ __    | |__  
// |  _  / / _ \ '_ \ / _` |/ _ \ '__|   | '_ \ 
// | | \ \|  __/ | | | (_| |  __/ |    _ | | | |
// |_|  \_\\___|_| |_|\__,_|\___|_|   (_)|_| |_|
//                                              
//                                              
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

#ifndef	_H_RENDER
#define	_H_RENDER

// ---------------------------------------------------------------------------------------------------------------------------------
// Required includes
// ---------------------------------------------------------------------------------------------------------------------------------

#include "Primitive.h"
#include "WinDIB.h"

// ---------------------------------------------------------------------------------------------------------------------------------
// Forward declarations
// ---------------------------------------------------------------------------------------------------------------------------------

class	ThreeDlg;
class	Bitmap;

// ---------------------------------------------------------------------------------------------------------------------------------

class	Render  
{
public:
	// Construction/Destruction

				Render(CDC &dc, ThreeDlg &window, const vector<primitive<> > &data, const point3 &cameraPosition);
virtual				~Render();

	// Accessors

inline		ThreeDlg	&window()			{return _window;}
inline		winDIB		&dib()				{return _dib;}
inline	const	unsigned int	&width() const			{return _width;}
inline		unsigned int	&width()			{return _width;}
inline	const	unsigned int	&height() const			{return _height;}
inline		unsigned int	&height()			{return _height;}
inline	const	unsigned int	*frameBuffer() const		{return _frameBuffer;}
inline		unsigned int	*frameBuffer()			{return _frameBuffer;}
inline	const	unsigned int	*aaBuffer() const		{return _aaBuffer;}
inline		unsigned int	*aaBuffer()			{return _aaBuffer;}
inline	const	float		*zBuffer() const		{return _zBuffer;}
inline		float		*zBuffer()			{return _zBuffer;}
inline	const	point2		&screenCenter() const		{return _screenCenter;}
inline		point2		&screenCenter() 		{return _screenCenter;}
inline	const	point3		&cameraPosition() const		{return _cameraPosition;}
inline		point3		&cameraPosition() 		{return _cameraPosition;}
inline	const	double		&xrot() const			{return _xrot;}
inline		double		&xrot() 			{return _xrot;}
inline	const	double		&yrot() const			{return _yrot;}
inline		double		&yrot() 			{return _yrot;}
inline	const	matrix4		&extraRotation() const		{return _extraRotation;}
inline		matrix4		&extraRotation() 		{return _extraRotation;}
inline	const	bool		&captureDesktop() const		{return _captureDesktop;}
inline		bool		&captureDesktop() 		{return _captureDesktop;}
inline	const	unsigned int	*desktop() const		{return _desktop;}
inline		unsigned int	*desktop()			{return _desktop;}
inline	const	unsigned int	*defaultTextureBuffer() const	{return _defaultTextureBuffer;}
inline		unsigned int	*defaultTextureBuffer()		{return _defaultTextureBuffer;}


	// Utilitarian

inline		void		flip() {dib().copyToDisplay();}
virtual		void		clear(const bool background = true);
virtual		bool		updateWindowPosition();
virtual		void		animate();
virtual		bool		renderFrame(const bool softenEdges, const bool changeView, const bool background = true);
virtual		bool		updateDefaultTexture(const wstring &name);
virtual		unsigned int *	loadTexture(const wstring &name, const bool silent = false, const bool store = true);
virtual		void		mouseMoved(const int relX, const int relY);
virtual		void		mousePressed(const bool left, const bool right);

		vector<primitive<> >	polygonList;

private:
		ThreeDlg	&_window;
		winDIB		_dib;
		unsigned int	_width, _height;
		unsigned int	*_frameBuffer;
		unsigned int	*_aaBuffer;
		float		*_zBuffer;
		point3		_cameraPosition;
		point2		_screenCenter;
		double		_xrot;
		double		_yrot;
		matrix4		_extraRotation;
		bool		_captureDesktop;
		unsigned int	*_desktop;
		unsigned int	*_defaultTextureBuffer;
		vector<unsigned int *> _textureArray;
		map<wstring, unsigned int *> _textureMapMap;
static	const	float		FOV;
static	const	float		nearZ;
static	const	float		farZ;
};

#endif // _H_RENDER
// ---------------------------------------------------------------------------------------------------------------------------------
// Render.h - End of file
// ---------------------------------------------------------------------------------------------------------------------------------

