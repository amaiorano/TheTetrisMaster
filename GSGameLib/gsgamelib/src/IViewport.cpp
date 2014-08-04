#include "IViewport.h"

#include "Renderer.h"
#include "ViewportWindowBinderFactory.h"
#include "SmartAssert.h"

IViewport::IViewport(void) : m_iInitViewportWidth(0), m_iInitViewportHeight(0)
{
}

IViewport::~IViewport(void)
{
}

void IViewport::Bind(IWindow& rWindow)
{
	// Get the binder for the current graphics and window port
	m_pBinder = ViewportWindowBinderFactory::CreateBinder();

	m_pBinder->Bind(*this, rWindow); // Bind this instance to the Window

	// Now save initial viewport dimensions and resize the viewport once.
	// This must be done to properly initialize OpenGL.
	m_iInitViewportWidth = rWindow.GetInititalWidth();
	m_iInitViewportHeight = rWindow.GetInitialHeight();
	ResizeViewport(rWindow.GetInititalWidth(), rWindow.GetInitialHeight());

	// Register for certain window events...
	rWindow.AddEventListener(*this, WMT_RESIZE);
}

void IViewport::Unbind(IWindow& rWindow)
{
	m_pBinder->Unbind(*this, rWindow);
	m_pBinder.reset(); // Delete the binder instance
}

void IViewport::PreRenderFrame()
{
	static Color4F black = Color3F::Black();
	Renderer::Instance().ClearColorBuffer(black);
	Renderer::Instance().MatrixMode(MODELVIEW_MODE, true);

	// Call pure virtual so child class can translate to initial position
	TranslateToStartPosition();
}

void IViewport::SwapBuffers()
{
	m_pBinder->SwapBuffers(); // Forward the call
}

bool IViewport::OnEvent(const Event& rEvent)
{
	switch (rEvent.type)
	{
	case WMT_RESIZE:
		{
			CAST_EVENT(rResizeEvent, ResizeWindowEvent);

			// Handle the resize
			ResizeViewport(rResizeEvent.iWidth, rResizeEvent.iHeight);

			// Update data members
			m_iViewportWidth = rResizeEvent.iWidth;
			m_iViewportHeight = rResizeEvent.iHeight;
		}
		break;

	default:
		SMART_ASSERT(false)(rEvent.type).msg("Unexpected event type received");
	}
	
	return true;
}

void IViewport::ResizeViewport(int iWidth, int iHeight)
{
	if (iHeight == 0) // Prevent divide by 0
		iHeight = 1;

	// Grab renderer reference
	IRenderer& rRenderer = Renderer::Instance();

	// Reset the viewport by mapping it to fit exactly within the window
	rRenderer.ResizeViewport(0, 0, iWidth, iHeight);

	// Reset projection matrix so we can set it up again
	rRenderer.MatrixMode(PROJECTION_MODE);

	// Call pure virtual so child class can do the projection setup
	SetupProjectionMatrix(iWidth, iHeight);

	// Back to default mode
	rRenderer.MatrixMode(MODELVIEW_MODE);
}
