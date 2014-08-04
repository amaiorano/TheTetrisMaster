#include "GLRenderer.h"

#include <boost/smart_ptr.hpp>
#include "SmartAssert.h"
#include "ImageFuncs.h"
#include "GLHelper.h" // Internal

// Uncomment this define to shoot texture memory usage to standard output (cout)
/* #define SHOW_TEX_MEM_USAGE */

#ifdef SHOW_TEX_MEM_USAGE
#include <iostream>
#endif

void GLRenderer::Inititialize()
{
	// Set defaults for OpenGL - many of these will eventually
	// be exposed as functions in IRenderer
	GLHelper::SetShadeModel(true);		// Set smooth shading
	GLHelper::SetDepthBuffer(false);	// No depth testing needed for 2D

	// TODO: From the current viewport, query if Y-axis is invertible so we
	// know which side is front-facing. For now, don't cull anything.
	GLHelper::SetFrontFace(true, false); // Counter-clockwise polygons face front, and cull backfaces
	
	GLHelper::SetWireFrame(false);		// No wireframe

	SetColor( Color3F::White() ); // Default color is white (alpha = 1 implicitly)

	// Ambient and diffuse components of all materials are 
	// the same as their color components (color tracking)
	GLHelper::SetColorMaterial(false);

	/*
	// Set up default lighting
	SetAmbientLight(0.2f, 0.2f, 0.2f); // Default global ambience
	OpenGLLight *pLight = GetOpenGLLight(0); // Get light 0
	pLight->Enable(); // Enable it
	*/
	
	GLHelper::SetLighting(false);

	// Texturing is enabled by default, blending is disabled by default
	GLHelper::SetTexturing(true);
	GLHelper::SetBlending(false);
}

void GLRenderer::DoMatrixMode(eMatrixMode mode)
{
	switch (mode)
	{
	case MODELVIEW_MODE:	glMatrixMode(GL_MODELVIEW); break;
	case PROJECTION_MODE:	glMatrixMode(GL_PROJECTION); break;
	default: SMART_ASSERT(false).msg("Unexpected matrix mode");
	}
}

TextureId GLRenderer::LoadTexture(const ImageData& rImageData, Size2d<float>& rTextureSize)
{
	// Make our own copy of the data because we need to manipulate it
	boost::scoped_array<UBYTE> pDataCopy( new UBYTE[rImageData.GetDataSize()] );
	rImageData.CopyDataTo(pDataCopy.get());	

	// And a copy of the image info
	ImageInfo imageInfoCopy = rImageData.GetImageInfo();	

#ifdef SHOW_TEX_MEM_USAGE
	static int iTotalBytes = 0;
	static int iUsedBytes = 0;
	static int iWastedBytes = 0;
	iUsedBytes += imageInfoCopy.GetDataSize();
#endif

	// NOTE: Not sure if this is necessary anymore. We can always just map textures upside
	// down ourselves.
#if 0
	// OpenGL stores textures upside-down, so we must invert the data...
	ImageFuncs::ReverseRows(pDataCopy.get(), imageInfoCopy); // Modifies pDataCopy
#endif

	// In OpenGL, textures must be a power of 2, so we must check the size of
	// the image and create a texture large enough to fit the image onto it
	UBYTE* pTemp = ImageFuncs::GrowToPowerOf2(pDataCopy.get(), imageInfoCopy); // May modify imageInfoCopy
	if ( pTemp != NULL )
		pDataCopy.reset(pTemp);

	SMART_ASSERT(imageInfoCopy.GetWidth()<=256 && imageInfoCopy.GetHeight()<=256)
		(rImageData.GetFileName())(imageInfoCopy.GetWidth())(imageInfoCopy.GetHeight())
		.warn("Texture width or height > 256, may not be compatible with some video cards");

#ifdef SHOW_TEX_MEM_USAGE
	iWastedBytes += imageInfoCopy.GetDataSize() - rImageData.GetImageInfo().GetDataSize();
	iTotalBytes = iUsedBytes + iWastedBytes;

	using namespace std;
	cout << "Total Bytes: " << iTotalBytes
	<< "\tUsed Bytes: " << iUsedBytes << "(" << ((float)iUsedBytes*100.0f/(float)iTotalBytes) << "%)"
	<< "\tWasted Bytes: " << iWastedBytes << "(" << ((float)iWastedBytes*100.0f/(float)iTotalBytes) << "%)" << endl;
#endif

	// Finally, generate texture id and load data into it	
	GLuint uiTexId;
	glGenTextures(1, &uiTexId);
	TextureId texId = uiTexId;

	glBindTexture(GL_TEXTURE_2D, texId);

	glTexImage2D(GL_TEXTURE_2D, 0, imageInfoCopy.iChannels, imageInfoCopy.imageSize.w,
		imageInfoCopy.imageSize.h, 0, (imageInfoCopy.iChannels==3? GL_RGB:GL_RGBA), 
		GL_UNSIGNED_BYTE, pDataCopy.get());

	// If we assert here, then something went wrong while loading
	// the texture into texture memory
	ASSERT_NO_GL_ERROR();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, /*GL_LINEAR*/GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, /*GL_LINEAR*/GL_NEAREST);

	// Set output texture image size and return texture id
	rTextureSize = static_cast<Size2d<float> >(imageInfoCopy.imageSize);
	return texId;
}

void GLRenderer::BeginRender(eShapeMode mode)
{
	// TODO: This switch is inefficient! Fix it.

	GLenum glMode;
	switch ( mode )
	{
	case QUADS: glMode = GL_QUADS; break;
	default: SMART_ASSERT(false)(mode).msg("Unsupported mode");
	};

	glBegin(glMode);
}
