#ifndef _IVIEWPORT_WINDOW_BINDER_H_
#define _IVIEWPORT_WINDOW_BINDER_H_

#include "IViewport.h"
#include "Window.h"

// Abstract class for an object that can bind a
// platform-specific Window to a platform-specific IViewport.
class IViewportWindowBinder
{
public:
	virtual ~IViewportWindowBinder(void) {}

	// Methods to bind and unbind the IViewport to the Window

	void Bind(IViewport& rViewport, IWindow& rWindow)
	{
		DoBind(rViewport, rWindow);
	}

	void Unbind(IViewport& rViewport, IWindow& rWindow)
	{
		DoUnbind(rViewport, rWindow);
	}

	// Swaps front and back buffers
	virtual void SwapBuffers() {}

protected:
	IViewportWindowBinder(void) {}

	// To implement in child classes
	virtual void DoBind(IViewport& rViewport, IWindow& rWindow) = 0;
	virtual void DoUnbind(IViewport& rViewport, IWindow& rWindow) = 0;
};


#endif // _IVIEWPORT_WINDOW_BINDER_H_
