// winstandard.h

// Contains the standard Windows functions and classes needed to create/destroy windows

#ifndef WINSTANDARD_H
#define WINSTANDARD_H

#include <windows.h>

// Function prototypes
void ErrorMessage(char message[]);
void ErrorMessageV(const char* format, ...);

//**********************************************************************
// Winclass handles the defining and registering of a windows class

class WinClassEx
{
	public:

		// Constructor
		WinClassEx(LPCSTR className, HINSTANCE hInstance, WNDPROC wndProc);

		// Facilitator to enable double-clicks
		void EnableDoubleClicks(void);

		// Member function to register class
		bool Register(void);

	private:

		WNDCLASSEX wndClassEx; // Window class object (Ex = Small icons, etc.)
};


//**********************************************************************
// WinCreate handles the creation and displaying of a registered Window class

class WinCreate
{
	public:

		// Constructor
		WinCreate(LPCSTR className, HINSTANCE hInstance, LPCSTR caption, 
							int width = CW_USEDEFAULT, int height = CW_USEDEFAULT);

		// Overloaded Constructor called if a menu is to be loaded
		WinCreate(LPCSTR className, HINSTANCE hInstance, LPCSTR caption, LPCSTR lpMenuName,
							int width = CW_USEDEFAULT, int height = CW_USEDEFAULT);

		// Member function to return the window handle (if needed by other functions)
		HWND ReturnWinHandle(void);

		// Member function to return the menu handle (if needed by other functions)
		HMENU ReturnMenuHandle(void);

		// Member function to load and return a handle to an Accelerator Table
		HACCEL LoadAccelTable(HINSTANCE hInstance, LPCSTR lpMenuName);

		// Member function to display the window
		void Show(UINT cmdShow);
	
	protected:
		HWND hwnd; // Handle to the Window
		HMENU hMenu; // Handle to the Menu
};


//**********************************************************************
// TextObject handles the simplest form of text output to the window

class TextObject
{
	public:

		TextObject(void); // Constructor
		~TextObject(void); // Destructor

		void SetWinHandle(HWND hwnd); // Member function to set hWindow (call in WinMan())

		void Write(char string[], int x, int y); // Used to write to window at (x,y)

	private:

		HWND hWindow; // handle to the window
		HDC hDC; // device context of window

};

#endif
