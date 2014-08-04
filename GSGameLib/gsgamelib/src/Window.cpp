#include "Window.h"

// NOTE:
// This is the only file that should ever include the IWindow implementations.
#ifdef _WIN32
#include "Win32Window.h"
#else
#error No IWindow implementation for this platform
#endif

IWindow& Window::Instance()
{
#ifdef _WIN32
	static Win32Window theInstance;
#endif

	return theInstance;
}
