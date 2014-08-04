#include "Win32Window.h"
#include "SmartAssert.h"
#include "MonitorData.h"

#define WND_CLASS_NAME	"GS_Win32Class"
static MonitorData g_monitorData = MonitorData::GetMonitorData(0);

// Utility function to adjust window position and size
void AdjustWindow(Point2d<int>& rPos, Size2d<int>& rSize, 				  
				  int iWidth, int iHeight, bool bCenter,
				  DWORD dwStyle, DWORD dwExStyle)
{
	// Determine window's new size
	RECT rect = {0, 0, iWidth, iHeight};
	::AdjustWindowRectEx(&rect, dwStyle, false, dwExStyle);
	rSize = Size2d<int>(rect.right-rect.left, rect.bottom-rect.top);

	// Deterine window's new position
	rPos = Point2d<int>(CW_USEDEFAULT, 0);

	if ( bCenter )
	{
		Size2d<int> monSize = g_monitorData.GetSize();
		rPos.x = (monSize.w - rSize.w)/2;
		rPos.y = (monSize.h - rSize.h)/2;
	}
}



Win32Window* Win32Window::ms_pInstance = NULL;

Win32Window::Win32Window(void) : m_hWnd(NULL), m_hInstance(NULL)
{
	// Set static instance pointer to this instance
	SMART_ASSERT(ms_pInstance == NULL).msg("There should not be more than one Win32Window instance");
	ms_pInstance = this;
}

Win32Window::~Win32Window(void)
{
}

void Win32Window::DoCreate(const std::string& strTitle, int iWidth, int iHeight, int iBitsPerPixel, bool bFullScreen)
{
	m_hInstance = ::GetModuleHandle(NULL); // Grab an instance for the window

	WNDCLASS wc;
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw on size, and own dc for window.
	wc.lpfnWndProc		= (WNDPROC)Win32Window::sWndProc;	// WndProc handles messages
	wc.cbClsExtra		= 0;									// No extra window data
	wc.cbWndExtra		= 0;									// No extra window data
	wc.hInstance		= m_hInstance;							// Set the instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load the default icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load the arrow pointer
	wc.hbrBackground	= NULL;									// No background required for GL
	wc.lpszMenuName		= NULL;									// We don't want a menu
	wc.lpszClassName	= WND_CLASS_NAME;						// Set the class name

	// Register the window class
	if ( !::RegisterClass(&wc) )
	{
		throw std::runtime_error("Failed to register the window class");
	}

	// g_monitorData should be set to the first monitor
	SMART_ASSERT(g_monitorData.IsAttached()).msg("First monitor must always be attached");

#ifdef DEBUG_MULTIMONITOR
	// Try to get 2nd monitor, if available
	MonitorData monitorData2 = MonitorData::GetMonitorData(1);
	if ( monitorData2.IsAttached() )
		g_monitorData = monitorData2;
#endif

	// If full screen, we need to change the screen's display settings
	if (bFullScreen)
	{
		DEVMODE dmScreenSettings;								// Device mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes sure memory's cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size of the devmode structure
		dmScreenSettings.dmPelsWidth	= iWidth;				// Selected screen width
		dmScreenSettings.dmPelsHeight	= iHeight;				// Selected screen height
		dmScreenSettings.dmBitsPerPel	= iBitsPerPixel;		// Selected bits per pixel
		dmScreenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Set monitor frequency to 60 Hz
		dmScreenSettings.dmDisplayFrequency = 60;
		dmScreenSettings.dmFields |= DM_DISPLAYFREQUENCY;

		// Save monitor name for Destroy()
		m_strMonitorName = g_monitorData.GetName();

		// Try to set selected mode and get results.  NOTE: CDS_FULLSCREEN gets rid of start bar.
		if (::ChangeDisplaySettingsEx(m_strMonitorName.c_str(), &dmScreenSettings, NULL, CDS_FULLSCREEN, NULL) != DISP_CHANGE_SUCCESSFUL)
		{
			// Mode failed, so tell user we're switching to windowed mode

			::MessageBox(NULL,
				"The requested fullscreen mode is not supported by\n"
				"your video card. Will use windowed mode instead",
				"Unsupported Fullscreen Mode", MB_OK|MB_ICONEXCLAMATION);

			bFullScreen = false;
		}
	}

	DWORD dwExStyle;	// Window extended style
	DWORD dwStyle;		// Window style

	if (bFullScreen)
	{
		dwExStyle=WS_EX_APPWINDOW;
		dwStyle=WS_POPUP;
		::ShowCursor(FALSE); // Hide mouse pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle=WS_OVERLAPPEDWINDOW;
	}

	// Determine window position and size
	Point2d<int> wndPos;
	Size2d<int> wndSize;
	bool bCenterWindow = (bFullScreen==false); // Center window in windowed mode only
	AdjustWindow(wndPos, wndSize, iWidth, iHeight, bCenterWindow, dwStyle, dwExStyle);

#ifdef DEBUG_MULTIMONITOR
	// Finally, set the start window position to top-left of second monitor
	// (if available)
	if ( monitorData2.IsAttached() )
	{
		// Since we may have switched to fullscreen mode, we must retrieve the
		// updated monitor data
		monitorData2 = MonitorData::GetMonitorData(1);

		// Update window position
		wndPos = monitorData2.GetPosition();
	}
#endif

	// Finally, create the window
	if (!(m_hWnd=::CreateWindowEx(dwExStyle,			// Extended style for the window
								WND_CLASS_NAME,			// Class name
								strTitle.c_str(),		// Window title
								dwStyle |				// Defined window style
								WS_CLIPSIBLINGS |		// Required window style
								WS_CLIPCHILDREN,		// Required window style
								wndPos.x,				// Window x position
								wndPos.y,				// Window y position
								wndSize.w,				// Window width
								wndSize.h,				// Window height
								NULL,					// No parent window
								NULL,					// No menu
								m_hInstance,			// Instance
								NULL)))					// Don't pass anything to WM_CREATE
	{
		Destroy();
		throw std::runtime_error("Failed to create window");
	}
}

void Win32Window::DoDestroy(bool bIsFullscreen)
{
	if ( bIsFullscreen )
		RestoreScreenMode();

	if (m_hWnd && ::DestroyWindow(m_hWnd))
	{
		m_hWnd = NULL;
	}

	if (m_hInstance && ::UnregisterClass(WND_CLASS_NAME, m_hInstance))
	{
		m_hInstance = NULL;
	}
}

void Win32Window::Show()
{
	::ShowWindow(m_hWnd, SW_SHOW);	// Show the window
	::SetForegroundWindow(m_hWnd);	// Slightly higher priority
	::SetFocus(m_hWnd);				// Sets keyboard focus to the window
}

void Win32Window::Hide()
{
	::ShowWindow(m_hWnd, SW_HIDE);	// Hide the window
}

void Win32Window::DoSetTitle(const std::string& strTitle)
{
	::SetWindowText(m_hWnd, strTitle.c_str());
}

void Win32Window::Resize(int iNewWidth, int iNewHeight)
{
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	DWORD dwExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);

	// Determine window's position and size
	Point2d<int> wndPos;
	Size2d<int> wndSize;
	bool bCenterWindow = true; // NOTE: This will cause problems in fullscreen
	AdjustWindow(wndPos, wndSize, iNewWidth, iNewHeight, bCenterWindow, dwStyle, dwExStyle);

	::SetWindowPos(m_hWnd, NULL, wndPos.x, wndPos.y, wndSize.w, wndSize.h, SWP_NOOWNERZORDER|SWP_NOZORDER);
}

bool Win32Window::ProcessMessages(bool& out_rbQuit)
{
	out_rbQuit = false;
	
	MSG msg;
	if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // Is there a message waiting?
	{
		if (msg.message == WM_QUIT)
		{
			out_rbQuit = true;
		}
		else
		{
			//::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		return true; // Message processed
	}

	return false; // No messages processed
}

void Win32Window::Sleep(int iTimeMS)
{
	::Sleep(iTimeMS);
}

void Win32Window::MsgBoxOk(MsgBoxIcon icon, const std::string& strCaption, const std::string& strMsg)
{
	UINT uType = MB_ICONINFORMATION;
	switch (icon)
	{
	case INFO_ICON:		uType = MB_ICONINFORMATION;	break;
	case QUESTION_ICON:	uType = MB_ICONQUESTION;	break;
	case ERROR_ICON:	uType = MB_ICONERROR;		break;
	}

	::MessageBox(m_hWnd, strMsg.c_str(), strCaption.c_str(), uType);
}

bool Win32Window::PollKeyState(VKEY vKey)
{
	// If most-significant bit set, the key is down
	return (::GetAsyncKeyState(vKey) & 0x8000) != 0;
}

bool Win32Window::IsActive() const
{
#ifdef DEBUG_MULTIMONITOR
	// If we're debugging multimonitor mode, then the game window will
	// be inactive while the focus is in the debugger, so fake it
	return true;
#endif

	return (::GetActiveWindow() == m_hWnd);
}

void Win32Window::RestoreScreenMode()
{
	// Switch back to the desktop
	::ChangeDisplaySettingsEx(m_strMonitorName.c_str(), NULL, 0, 0, NULL);
	::ShowCursor(TRUE);	// Show mouse pointer
}


LRESULT CALLBACK Win32Window::sWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SMART_ASSERT(Win32Window::ms_pInstance != NULL).msg("A valid Win32Window must be created");
	return Win32Window::ms_pInstance->WndProc(hWnd, uMsg, wParam, lParam);
}

LRESULT Win32Window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{		
		case WM_ACTIVATE:
			{
				// In multimonitor mode, don't bother sending activation messages
				// because we want the window to remain active for debugging.
				#ifdef DEBUG_MULTIMONITOR
					break;
				#endif
					

				ActivateWindowEvent event(!HIWORD(wParam) && (LOWORD(wParam) != WA_INACTIVE));
				if ( NotifyListeners(event) )
					return 0;
			}
			break;

		case WM_CLOSE:
			{
				CloseWindowEvent event;
				if ( NotifyListeners(event) )
					return 0;

				// Apparently not handled by client, so let's post a quit message.
				// This will cause ProcessMessages() to return false.
				PostQuitMessage(0);
			}
			break;

		case WM_SIZE:
			{
				ResizeWindowEvent event(LOWORD(lParam), HIWORD(lParam));
				if ( NotifyListeners(event) )
					break;//return 0;
			}
			break;

		case WM_MOVE:
			{
				MoveWindowEvent event(LOWORD(lParam), HIWORD(lParam));
				if ( NotifyListeners(event) )
					break;//return 0;
			}
			break;

		case WM_WINDOWPOSCHANGED:
			{
				LPWINDOWPOS pWinPos = (LPWINDOWPOS)lParam;
				if ( !(pWinPos->flags & SWP_NOSIZE) )
				{
					ResizeWindowEvent event(pWinPos->cx, pWinPos->cy);
					if ( NotifyListeners(event) )
						break;//return 0;
				}
				else if ( !(pWinPos->flags & SWP_NOMOVE) )
				{
					MoveWindowEvent event(pWinPos->x, pWinPos->y);
					if ( NotifyListeners(event) )
						break;//return 0;
				}
			}
			break;
//			return 0;

/*
		case WM_KEYDOWN: case WM_SYSKEYDOWN:
			// Ignore return value - we want default handler to process key-downs
			// so that Alt+F4 will close the window
//			m_pWndCallbackHandler->OnWindowKeyDown( (unsigned int)wParam );
			//return 0;
			break;

		case WM_KEYUP: case WM_SYSKEYUP:
//			m_pWndCallbackHandler->OnWindowKeyUp( (unsigned int)wParam );
//			return 0;
			//if ( m_pWndCallbackHandler->OnWindowKeyUp( (unsigned int)wParam ) )
			//	return 0; // DO WE DO THIS?
			//break;
*/

		case WM_SYSCOMMAND:	// Intercept system commands
			switch (wParam)	// Check system calls
			{
				// Screensaver or monitor power saving trying to go on
				case SC_SCREENSAVE: case SC_MONITORPOWER:					
					return 0; // Prevent from happening
			}
			break;
	} // switch (uMsg)


	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

