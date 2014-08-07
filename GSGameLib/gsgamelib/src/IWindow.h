#ifndef _IWINDOW_H_
#define _IWINDOW_H_

#include <string>
#include "WindowEvents.h"

typedef unsigned int VKEY;

enum MsgBoxIcon
{
	INFO_ICON,
	QUESTION_ICON,
	ERROR_ICON
};

// Interface for platform-specific windowing systems.
// Subclass for each window platform (i.e. Win32, X-Win),
// but use only through an interface pointer/reference.
//
// Implementations are also responsible for properly dispatching
// events for all event types defined in WindowEvents.h

class IWindow : public EventNotifier
{
public:
	virtual ~IWindow(void) {}

	// Creates the window
	inline void Create(const std::string& strTitle="My Window", int iWidth=640, int iHeight=480, int iBitsPerPixel=32, bool bFullScreen=false);

protected:
	virtual void DoCreate(const std::string& strTitle, int iWidth, int iHeight, int iBitsPerPixel, bool& bFullScreen) = 0;
public:

	// Destroys the window
	void Destroy()
	{ 		
		DoDestroy(m_bFullScreen);
		m_bIsCreated = false;
	}
protected:	
	// TODO: GET RID OF bIsFullscreen param, can access through IsFullScreen() method
	virtual void DoDestroy(bool bIsFullscreen) = 0;
public:

	// Shows the window
	virtual void Show() = 0;

	// Hides the window
	virtual void Hide() = 0;

	// Sets the window title
	void SetTitle(const std::string& strTitle)
	{ 	
		DoSetTitle(strTitle);
		m_strTitle = strTitle;
	}
protected:
	virtual void DoSetTitle(const std::string& strTitle) = 0;
public:

	// Resizes the window
	virtual void Resize(int iNewWidth, int iNewHeight) = 0;

	// Call in main game loop to let Window process messages.
	// Returns true if message is processed, and sets out_rbQuit to
	// true if quit message is received.
	virtual bool ProcessMessages(bool& out_rbQuit) = 0;

	// Sleeps input amount of time in milliseconds
	virtual void Sleep(int iTimeMS) = 0;

	// Displays a message box with input message
	virtual void MsgBoxOk(MsgBoxIcon icon, const std::string& strCaption, const std::string& strMsg) = 0;

	// Returns true if input key is currently down
	virtual bool PollKeyState(VKEY vKey) = 0;

	// Returns true if this is the currently active window
	virtual bool IsActive() const = 0;

	// Restores screen mode back to what it was before switching to fullscreen
	virtual void RestoreScreenMode() = 0;

	// Accessors
	bool IsCreated() const						{ return m_bIsCreated; }
	const std::string& GetTitle()				{ return m_strTitle; }
	int GetBitsPerPixel()						{ return m_bpp; }
	int GetInititalWidth()						{ return m_initWidth; }
	int GetInitialHeight()						{ return m_initHeight; }
	bool IsFullScreen()							{ return m_bFullScreen; }

protected:
	IWindow(void) {}

private:
	std::string m_strTitle;			// Title of the window
	int m_initWidth, m_initHeight;	// Initial width and height of the window
	int m_bpp;						// Number of bits per pixel (8/16/24/32)
	bool m_bFullScreen;				// Full screen state

	bool m_bIsCreated;				// Is window created or not
};


// Inline member function definitions

inline void IWindow::Create(const std::string& strTitle/*="My Window"*/, int iWidth/*=640*/, int iHeight/*=480*/, int iBitsPerPixel/*=32*/, bool bFullScreen/*=false*/)
{
	DoCreate(strTitle, iWidth, iHeight, iBitsPerPixel, bFullScreen);

	m_bIsCreated = true;
	m_strTitle = strTitle;
	m_initWidth = iWidth;
	m_initHeight = iHeight;
	m_bpp = iBitsPerPixel;
	m_bFullScreen = bFullScreen;
}

#endif // _IWINDOW_H_
