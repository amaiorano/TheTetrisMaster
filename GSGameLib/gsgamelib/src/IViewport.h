#ifndef _IVIEWPORT_H_
#define _IVIEWPORT_H_

#include <memory>
#include "Window.h"
#include "WindowEvents.h"

class IViewportWindowBinder;

// Represents the viewport for the graphics system within the Window.
// The IViewport instance must be bound to the Window instance.
// This binding is platform-renderer-specific.
class IViewport : protected IEventListener
{
public:
	virtual ~IViewport();

	// Attaches IViewport instance to the IWindow instance
	void Bind(IWindow& rWindow);
	
	// Detaches IViewport instance from IWindow it was previously bound to
	void Unbind(IWindow& rWindow);

	// Returns whether IViewport object is bound to a Window
	bool IsBound() const			{ return m_pBinder.get() != NULL; }

	// Functions that return current viewport width and height
	int GetViewportWidth() const	{ return m_iViewportWidth; }
	int GetViewportHeight() const	{ return m_iViewportHeight; }

	// Functions that return the initial viewport width and height
	int GetInitViewPortWidth() const	{ return m_iInitViewportWidth; }
	int GetInitViewPortHeight() const	{ return m_iInitViewportHeight; }

	// Call before each scene render to setup the viewport
	void PreRenderFrame();

	// Call to swap front and back buffers
	void SwapBuffers();

private:
	virtual bool OnEvent(const Event& rEvent);
	void ResizeViewport(int iWidth, int iHeight);

protected:
	IViewport(); // Must be inherited

	// Called by ResizeViewport to setup projection matrix
	virtual void SetupProjectionMatrix(int iWidth, int iHeight) = 0;
	virtual void TranslateToStartPosition() = 0;

private:
	// Maintains ownership of an IViewportWindowBinder
	std::auto_ptr<IViewportWindowBinder> m_pBinder;

	// Should always contain current viewport dimensions
	// (updated by OnEvent(WMT_RESIZE))
	int m_iViewportWidth, m_iViewportHeight;
	
	// Will contain only initial dimensions
	int m_iInitViewportWidth, m_iInitViewportHeight;
};

#endif // _IVIEWPORT_H_
