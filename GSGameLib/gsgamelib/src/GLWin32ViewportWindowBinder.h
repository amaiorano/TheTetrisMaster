#ifndef _GLWIN32_VIEWPORT_WINDOW_BINDER_H_
#define _GLWIN32_VIEWPORT_WINDOW_BINDER_H_

#include "IViewportWindowBinder.h"

// This binder knows how to bind a Win32 window to an OpenGL viewport.
class GLWin32ViewportWindowBinder : public IViewportWindowBinder
{
protected:
	virtual void DoBind(IViewport& rViewport, IWindow& rWindow);
	virtual void DoUnbind(IViewport& rViewport, IWindow& rWindow);
	virtual void SwapBuffers();
};


#endif // _GLWIN32_VIEWPORT_WINDOW_BINDER_H_
