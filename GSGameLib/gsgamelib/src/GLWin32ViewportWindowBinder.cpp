#include "GLWin32ViewportWindowBinder.h"

#include <stdexcept>

// Include platform-specific stuff
#include "GLRenderer.h"

#ifdef _WIN32
#include "Win32Window.h"
#else
#error No implementation for current platform
#endif

namespace // Anonymous, private to this CPP
{
	HDC g_hDC = NULL;
	HGLRC g_hRC = NULL;
}

void GLWin32ViewportWindowBinder::DoBind(IViewport& rViewport, IWindow& rWindow)
{
	// Grab current IWindow implementation and cast it to Win32
	Win32Window& rWin32Window = static_cast<Win32Window&>(Window::Instance());
	HWND hWnd = rWin32Window.GetWindowHandle();

	// Get the window's device context
	g_hDC = GetDC(hWnd);

	if (g_hDC == NULL)
		throw std::runtime_error("GetDC() failed");

	// Set the window's pixel format
	PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof(PIXELFORMATDESCRIPTOR),	// Size of this pixel format descriptor
		1,								// Version number
		PFD_DRAW_TO_WINDOW |			// Format must support window
		PFD_SUPPORT_OPENGL |			// Format must support opengl
		PFD_DOUBLEBUFFER,				// Must support double buffering
		PFD_TYPE_RGBA,					// Request an rgba format
		rWindow.GetBitsPerPixel(),		// Select our color depth
		0, 0, 0, 0, 0, 0,				// Color bpp ignored
		0,								// No alpha buffer
		0,								// Shift bit ignored
		0,								// No accumulation buffer
		0, 0, 0, 0,						// Accumulation bpp ignored
		16,								// 16-bit z-buffer (depth buffer)  
		0,								// Stencil buffer depth
		0,								// Auxiliary buffer: not supported
		PFD_MAIN_PLANE,					// Layer type: ignored by OpenGL now
		0,								// Reserved
		0, 0, 0							// Layer masks ignored
	};

	GLuint pixFormat = ChoosePixelFormat(g_hDC, &pfd);

	if (!pixFormat)
		throw std::runtime_error("ChoosePixelFormat() failed");		

	if ( !SetPixelFormat(g_hDC, pixFormat, &pfd) )
		throw std::runtime_error("SetPixelFormat() failed");

	// Create an OpenGL rendering context
	g_hRC = wglCreateContext(g_hDC);

	if (!g_hRC)
		throw std::runtime_error("wglCreateContext(g_hDC) failed");

	// Make the rendering context active
	if ( !wglMakeCurrent(g_hDC, g_hRC) )
		throw std::runtime_error("wglMakeCurrent() failed");

	// Set up initial projection by calling ResizeScene()
//	ResizeScene(rWindow.GetInititalWidth(), rWindow.GetInitialHeight());	
}


void GLWin32ViewportWindowBinder::DoUnbind(IViewport& rViewport, IWindow& rWindow)
{
	SMART_ASSERT(rWindow.IsCreated()).msg("Do not destroy Window before unbinding it from Viewport");

	Win32Window& rWin32Window = static_cast<Win32Window&>(Window::Instance());
	HWND hWnd = rWin32Window.GetWindowHandle();

	if (g_hRC)
	{
		wglMakeCurrent(NULL, NULL);	// Unset the current rendering context
		wglDeleteContext(g_hRC);	// Delete the rendering context
		g_hRC = NULL;
	}

	if (g_hDC && !ReleaseDC(hWnd, g_hDC))
	{
		g_hDC = NULL;
	}
}

void GLWin32ViewportWindowBinder::SwapBuffers()
{
	::SwapBuffers( g_hDC );
}
