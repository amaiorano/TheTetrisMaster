// winmain.cpp

#include "winstandard.h"
#include "globals.h"

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgs, int nWinMode)
{
	char className[] = "MainWindow";

	// Declare a window class
	WinClassEx winClassEx(className, hThisInstance, WindowProc);

	// Select window-specific options
	winClassEx.EnableDoubleClicks();
	//winClassEx.UseMenu("MainMenu");

	// Register the window class
	if (!winClassEx.Register())
		return 0;

	// Create the window
	WinCreate win(className, hThisInstance, "The Tetris Master Theme Editor", "MainMenu", 320*2, 330);

	// Load Key Accelerator Table for the menu
	HACCEL hAccel = win.LoadAccelTable(hThisInstance, "MainMenu");

	// Store this window's instance into global variable hinst
	hinst = hThisInstance;

	// Get the window handle and store into global variable hwnd
	hwnd = win.ReturnWinHandle();

	// Get the menu handle and store into global variable hMenu, then disable menu items
	hMenu = win.ReturnMenuHandle();
	SetupMenuItems(false);

	// Now that we've got the window handle, send it to all objects that need it
	Video.SetWinHandle(hwnd);
	OpenSaveFile.SetWinHandle(hwnd);
	Text.SetWinHandle(hwnd);

	/*
	// Intialize the Status Bar
	StatusBar.Initialize(hwnd, 2);
	StatusBar.SetText(0, "Test");
	StatusBar.SetText(1, "Test 2");
	*/

	// Display the window
	Video.ShowScreen(); // Call this so the opening window is all one color
	win.Show(nWinMode); // Now actually display the window

	// Main message-handling loop
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(hwnd, hAccel, &msg)) // If accelerator key is used, then TranslateMessage and DispatchMessage don't need to be called
		{
			TranslateMessage(&msg);	// Allow use of keyboard
			DispatchMessage(&msg);	// Return control to Windows
		}
	}
	return (int)msg.wParam;
}
