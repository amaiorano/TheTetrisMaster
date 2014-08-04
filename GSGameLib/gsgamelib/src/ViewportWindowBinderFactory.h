#ifndef _VIEWPORT_WINDOW_BINDER_FACTORY_H_
#define _VIEWPORT_WINDOW_BINDER_FACTORY_H_

#include "IViewportWindowBinder.h"

// Factory that creates ViewportWindowBinders (see below)
class ViewportWindowBinderFactory
{
public:
	// Returns a binder class instance for the current renderer and window type
	static std::auto_ptr<IViewportWindowBinder> CreateBinder();

private:
	ViewportWindowBinderFactory() {}
};

#endif // _VIEWPORT_WINDOW_BINDER_FACTORY_H_
