// winstandard.cpp

// Contains standard functions and class definitions to create/destroy windows

#include "winstandard.h"
#include <stdarg.h>
#include <locale>

// Function definitions
void ErrorMessage(char message[])
{
	MessageBox(NULL, message, "Error", MB_OK | MB_ICONHAND);
}

void ErrorMessageV(const char* format, ...)
{
	va_list args;
	va_start(args, format);

	static char buff[2048];
	vsprintf(buff, format, args);
	
	ErrorMessage(buff);
}

// Winclass member function definitions
//**********************************************************************
//**********************************************************************
// Constructor

WinClassEx::WinClassEx(LPCSTR className, HINSTANCE hInstance, WNDPROC wndProc)
{
	wndClassEx.cbSize = sizeof(WNDCLASSEX);
	
	wndClassEx.hInstance = hInstance;			// Handle to this instance
	wndClassEx.lpszClassName = className;	// Window class name
	wndClassEx.lpfnWndProc = wndProc;			// Pointer to Windows callback function
	wndClassEx.style = 0;									// Default style

	wndClassEx.hIcon = LoadIcon(hInstance, "LARGEICON");	// Hand-drawn icon
	wndClassEx.hIconSm = NULL; //LoadIcon(NULL, IDI_WINLOGO);		// Small icon
	wndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);		// Cursor style

	wndClassEx.lpszMenuName = NULL;		// No menu
	wndClassEx.cbClsExtra = 0;				// No extra
	wndClassEx.cbWndExtra = 0;				// information needed

	wndClassEx.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
}

//**********************************************************************
// Facilitator to enable double-clicks

void WinClassEx::EnableDoubleClicks(void)
{
	wndClassEx.style = CS_DBLCLKS;
}


//**********************************************************************
// Member function to register class

bool WinClassEx::Register(void)
{
	if (RegisterClassEx(&wndClassEx))
		return true;
	else
		return false;
}



// WinCreate member function definitions
//**********************************************************************
//**********************************************************************
// Constructor

WinCreate::WinCreate(LPCSTR className, HINSTANCE hInstance, LPCSTR caption, int width, int height)
{

	hwnd = CreateWindow(
		className,						// Name of window class
		caption,							// Caption is what will be displayed at the top of the window
		height == CW_USEDEFAULT? WS_OVERLAPPEDWINDOW : WS_OVERLAPPED || WS_CAPTION || WS_SYSMENU || WS_MINIMIZEBOX, 	// Window style - normal
		CW_USEDEFAULT,				// X coordinate - let Windows decide
		CW_USEDEFAULT,				// Y coordinate - let Windows decide
		width + 6,						// Left border + right border = 6 pixels
		height + 44,					// Top border + bottom border = 44 pixels
		HWND_DESKTOP,					// No handle to parent window
		NULL,									// No handle to menu
		hInstance,						// Handle to this instance of the program
		NULL									// No additional arguments
	);
}

//**********************************************************************
// Overloaded Constructor called if a menu is to be loaded
WinCreate::WinCreate(LPCSTR className, HINSTANCE hInstance, LPCSTR caption, LPCSTR lpMenuName, int width, int height)
{
	// Load the menu
	hMenu = LoadMenu(hInstance, lpMenuName);
	
	// Create window using the menu
	hwnd = CreateWindow(
		className,						// Name of window class
		caption,							// Caption is what will be displayed at the top of the window
		height == CW_USEDEFAULT? WS_OVERLAPPEDWINDOW : WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,	// Window style - normal
		CW_USEDEFAULT,				// X coordinate - let Windows decide
		CW_USEDEFAULT,				// Y coordinate - let Windows decide
		width + 6,						// Left border + right border = 6 pixels
		height + 44,					// Top border + bottom border = 44 pixels
		HWND_DESKTOP,					// No handle to parent window
		hMenu,								// Load the new menu
		hInstance,						// Handle to this instance of the program
		NULL									// No additional arguments
	);
}

//**********************************************************************
// Member function to return the window handle (if needed by other functions)

HWND WinCreate::ReturnWinHandle(void)
{
	return hwnd;
}

//**********************************************************************
// Member function to return the menu handle (if needed by other functions)

HMENU WinCreate::ReturnMenuHandle(void)
{
	return hMenu;
}


//**********************************************************************
// Member function to load and return a handle to an Accelerator Table

HACCEL WinCreate::LoadAccelTable(HINSTANCE hInstance, LPCSTR lpMenuName)
{
	return LoadAccelerators(hInstance, lpMenuName);
}

//**********************************************************************
// Member function to display the window

void WinCreate::Show(UINT cmdShow)
{
	ShowWindow(hwnd, cmdShow);
  UpdateWindow(hwnd);
}








// TextObject member function defintions
//**********************************************************************
//**********************************************************************

//**********************************************************************
// Constructor
TextObject::TextObject(void)
{
	// Nothing to initialize
}

//**********************************************************************
// Destructor
TextObject::~TextObject(void)
{
	ReleaseDC(hWindow, hDC);
}

//**********************************************************************
// Member function to set hWindow (call in WinMan())
void TextObject::SetWinHandle(HWND hwnd)
{
	hWindow = hwnd;
	hDC = GetDC(hwnd);
}

//**********************************************************************
// Used to write to window at (x,y)
void TextObject::Write(char string[], int x, int y)
{
	TextOut(hDC, x, y, string, (int)strlen(string));
}

