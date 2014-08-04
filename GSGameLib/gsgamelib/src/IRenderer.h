#ifndef _IRENDERER_H_
#define _IRENDERER_H_

#include "Angle.h"
#include "ImageData.h"
#include "ImageFuncs.h"
#include "SmartAssert.h"
#include "TextureId.h"
#include "Color.h"

enum eMatrixMode
{
	MODELVIEW_MODE,
	PROJECTION_MODE
};

enum eGraphicsBuffer
{
	COLOR_BUFFER,
	DEPTH_BUFFER
};

enum eShapeMode
{
	QUADS
};


// Interface for platform-specific graphics systems.
// Subclass for each graphics platform (i.e. OpenGL, D3D),
// but use only through an interface pointer/reference.

class IRenderer
{
public:	
	IRenderer(void) {}
	virtual ~IRenderer(void) {}

	///////////////////////////////
	// General functions
	///////////////////////////////

	// Call to resize the viewport when window size changes
	// Must call at least once when window is created BEFORE any other
	// functions
	virtual void ResizeViewport(int x, int y, int iWidth, int iHeight) = 0;

	// Call once after ResizeViewport() has been called the first time to
	// initialize the graphics system
	virtual void Inititialize() = 0;

	// Sets/gets current color, used by many other functions
	virtual void SetColor(const Color4F& color) = 0;
	virtual Color4F GetColor() = 0;

	// Call to clear color buffer (all values range from [0,1])
	virtual void ClearColorBuffer(const Color4F& color) = 0;

	// Call to clear the depth buffer (fDepth ranges from [0,1])
	virtual void ClearDepthBuffer(float fDepth) = 0;

	///////////////////////////////
	// General matrix functions
	///////////////////////////////

	// Call to change current matrix
	void MatrixMode(eMatrixMode mode, bool bLoadIdentity=true)
	{
		DoMatrixMode(mode);
		if ( bLoadIdentity )
			LoadIdentity();
	}
protected:
	virtual void DoMatrixMode(eMatrixMode mode) = 0;
public:

	// Call to load identity matrix in current matrix
	virtual void LoadIdentity() = 0;

	// Pushes current matrix
	virtual void PushMatrix() = 0;

	// Pops current matrix
	virtual void PopMatrix() = 0;

	///////////////////////////////
	// Modelview matrix functions
	///////////////////////////////

	// Translates to specified (x,y,z) offset from current position
	virtual void Translate(float x, float y, float z) = 0;

	// Translates to specified (x,y) offset from current position
	virtual void Translate(float x, float y) = 0;	

	// Rotates angle amount in degrees around specified axis
	virtual void Rotate(const Angle& rAngle, float x, float y, float z) = 0;
	
	// Rotates angle amount in degrees around z-axis at current position
	virtual void Rotate(const Angle& rAngle) = 0;

	// Scales by input factors, which affects subsequent rendering
	virtual void Scale(float x, float y) = 0;

	///////////////////////////////
	// Projection matrix functions
	///////////////////////////////

	// Sets up orthographic projection
	virtual void Orthographic(float fLeft, float fRight, float fBottom, float fTop, float fNear, float fFar) = 0;

	// Sets up perspective projection where you define a frustrum
	virtual void Perspective(float fLeft, float fRight, float fBottom, float fTop, float fNear, float fFar) = 0;

	// Easier method to setup a perspective projection using field of view angle and aspect ratio
	virtual void Perspective(float fFieldOfViewAngle, float fAspectRatio, float fNear, float fFar) = 0;

	// Helper for 2D
	void Orthographic2D(float fLeft, float fRight, float fBottom, float fTop)
	{
		Orthographic(fLeft, fRight, fBottom, fTop, -1, 1);
	}

	///////////////////////////////
	// Texturing functions
	///////////////////////////////

	// Enables/disables texturing
	virtual void SetTexturing(bool bEnable) = 0;

	// Creates a 2d texture, retuning unique texture id and the texture's size in
	// the output parameter. Consecutive calls guarantee returning consecutive ids
	// (i.e. 3,4,5,etc.). The texture size might be larger if the image size is not
	// a power of two.
	virtual TextureId LoadTexture(const ImageData& rImageData, Size2d<float>& rTextureSize) = 0;

	// Overload that expects input image dimensions to be a power of two,
	// otherwise an exception is thrown.
	TextureId LoadTexture(const ImageData& rImageData)
	{
		if ( ImageFuncs::IsPowerOf2(rImageData.GetSize()) == false )
			throw std::logic_error("Input image dimensions must be a power of two");
		Size2d<float> useless;
		return LoadTexture(rImageData, useless);
	}			

	// Unloads the texture data for the input texture id
	void FreeTexture(TextureId& rTexId)
	{
		if ( rTexId.IsFree() )
			return;
		DoFreeTexture(rTexId); // Forward call
		rTexId.Reset();
	}
protected:
	virtual void DoFreeTexture(TextureId& rTexId) = 0;
public:

	// Select/unselects the input texture
	virtual void SelectTexture(const TextureId& rTexId, bool bSelect=true) = 0;

	///////////////////////////////
	// Rendering functions
	///////////////////////////////

	virtual void BeginRender(eShapeMode mode) = 0;
	virtual void EndRender() = 0;
	virtual void Vertex(float x, float y) = 0;
	virtual void TexCoord(float s, float t) = 0;

	// Useful for debugging, renders the x-y-z axes
	// (x=red, y=blue, z=green)
	virtual void RenderXYZAxes(float fAxisSize=1.0f, float fAxisWidth=1.0f) = 0;

	//////////////////////////////////////
	// Blending/Transparency functions
	//////////////////////////////////////

	// If enabled, alpha channels are used to determine
	// which part of a texture to render.
	virtual void SetAlphaBlending(bool bEnable) = 0;
};

#endif // _IRENDERER_H_
