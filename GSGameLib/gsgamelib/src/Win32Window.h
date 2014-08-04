#ifndef _WIN32_WINDOW_H_
#define _WIN32_WINDOW_H_

// Uncomment this to enable multimonitor debugging
//#define DEBUG_MULTIMONITOR

#include <string>
#include <windows.h> // Win32-specific header
#include "IWindow.h"

class Win32Window : public IWindow	
{
public:
	Win32Window(void);
	virtual ~Win32Window(void);

	// IWindow implementation
	virtual void DoCreate(const std::string& strTitle, int iWidth, int iHeight, int iBitsPerPixel, bool bFullScreen);
	virtual void DoDestroy(bool bIsFullscreen);
	virtual void Show();
	virtual void Hide();
	virtual void DoSetTitle(const std::string& strTitle);
	virtual void Resize(int iNewWidth, int iNewHeight);
	virtual bool ProcessMessages(bool& out_rbQuit);
	virtual void Sleep(int iTimeMS);
	virtual void MsgBoxOk(MsgBoxIcon icon, const std::string& strCaption, const std::string& strMsg);
	virtual bool PollKeyState(VKEY vKey);
	virtual bool IsActive() const;
	virtual void RestoreScreenMode();

	// Win32-specific functions
	HWND GetWindowHandle() const { return m_hWnd; }

private:
	// We have two WndProcs: one required by Win32 (static for C), and one
	// that the static version calls.
	static Win32Window* ms_pInstance; // Singleton pointer needed by static window proc
	static LRESULT CALLBACK sWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HWND m_hWnd;				// Handle to a Win32 window
	HINSTANCE m_hInstance;		// Instance handle to Win32 application

	std::string m_strMonitorName;
};

#endif // _WIN32_WINDOW_H_
