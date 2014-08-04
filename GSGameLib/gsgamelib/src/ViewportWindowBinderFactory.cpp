#include "ViewportWindowBinderFactory.h"

#include <stdexcept>
#include "SmartAssert.h"

// NOTE:
// This is the only file that should ever include the IViewportWindowBinder implementations.
#include "GLWin32ViewportWindowBinder.h"

std::auto_ptr<IViewportWindowBinder> ViewportWindowBinderFactory::CreateBinder()
{
	// Create platform-specific binder and return it

	return std::auto_ptr<IViewportWindowBinder>(new GLWin32ViewportWindowBinder());
}
